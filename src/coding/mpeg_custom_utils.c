#include "mpeg_decoder.h"

#ifdef VGM_USE_MPEG

/* init config and validate per type */
int mpeg_custom_setup_init_default(STREAMFILE *streamFile, off_t start_offset, mpeg_codec_data *data, coding_t *coding_type) {
    mpeg_frame_info info;


    /* get frame info at offset */
    if ( !mpeg_get_frame_info(streamFile, start_offset, &info))
        goto fail;
    switch(info.layer) {
        case 1: *coding_type = coding_MPEG_layer1; break;
        case 2: *coding_type = coding_MPEG_layer2; break;
        case 3: *coding_type = coding_MPEG_layer3; break;
        default: goto fail;
    }
    data->channels_per_frame = info.channels;
    data->samples_per_frame = info.frame_samples;


    /* extra checks per type */
    switch(data->type) {
        case MPEG_XVAG:
            if (data->config.chunk_size <= 0 || data->config.interleave <= 0)
                goto fail; /* needs external fixed size */
            break;

        case MPEG_FSB:
            if (data->config.fsb_padding != 0
                    && data->config.fsb_padding != 2
                    && data->config.fsb_padding != 4
                    && data->config.fsb_padding != 16)
                goto fail; /* aligned to closest 0/2/4/16 bytes */

            /* get find interleave to stream offsets are set up externally */
            {
                int current_data_size = info.frame_size;
                int current_padding = 0;
                /* FSB padding for Layer III or multichannel Layer II */
                if ((info.layer == 3 && data->config.fsb_padding) || data->config.fsb_padding == 16) {
                    current_padding = (current_data_size % data->config.fsb_padding)
                            ? data->config.fsb_padding - (current_data_size % data->config.fsb_padding)
                            : 0;
                }

                data->config.interleave = current_data_size + current_padding; /* should be constant for all stream */
            }
            break;

        case MPEG_P3D:
            if (data->config.interleave <= 0)
                goto fail; /* needs external fixed size */
            break;

        case MPEG_LYN:
            goto fail; /* not fully implemented */

        case MPEG_STANDARD:
        case MPEG_AHX:
        case MPEG_EA:
            if (data->channels_per_frame != data->config.channels)
                goto fail; /* no multichannel expected */
            break;

        default:
            break;  /* nothing special needed */
    }


    //todo: test more: this improves the output, but seems formats aren't usually prepared
    // (and/or the num_samples includes all possible samples in file, so by discarding some it'll reach EOF)
    // FSBs (with FMOD DLLs) don't seem to need it, even when files contain garbage at the beginning
#if 0
    /* set encoder delay (samples to skip at the beginning of a stream) if needed, which varies with encoder used */
    switch(data->type) {
        case MPEG_AHX: data->skip_samples = 480; break; /* observed default */
        case MPEG_P3D: data->skip_samples = info.frame_samples; break; /* matches Radical ADPCM (PC) output */
        default: break;
    }
    data->samples_to_discard = data->skip_samples;
#endif

    return 1;
fail:
    return 0;
}


/* writes data to the buffer and moves offsets */
int mpeg_custom_parse_frame_default(VGMSTREAMCHANNEL *stream, mpeg_codec_data *data) {
    mpeg_frame_info info;
    size_t current_data_size = 0;
    size_t current_padding = 0;
    size_t current_interleave = 0;


    /* Get data size to give to the decoder, per stream. Usually 1 frame at a time,
     * but doesn't really need to be a full frame (decoder would request more data). */
    switch(data->type) {

        case MPEG_XVAG: /* frames of fixed size (though we could read frame info too) */
            current_data_size = data->config.chunk_size;
            current_interleave = data->config.interleave; /* big interleave */
            break;

        case MPEG_FSB: /* frames with padding + interleave */
            if ( !mpeg_get_frame_info(stream->streamfile, stream->offset, &info) )
                goto fail;
            current_data_size = info.frame_size;

            /* get FSB padding for Layer III or multichannel Layer II (Layer I doesn't seem to be supported)
             * Padding sometimes contains garbage like the next frame header so we can't feed it to mpg123 or it gets confused. */
            if ((info.layer == 3 && data->config.fsb_padding) || data->config.fsb_padding == 16) {
                current_padding = (current_data_size % data->config.fsb_padding)
                        ? data->config.fsb_padding - (current_data_size % data->config.fsb_padding)
                        : 0;
            }

            /* frame interleave (ie. read 1 data-frame, skip 1 data-frame per stream) */
            current_interleave = data->config.interleave; /* constant for multi-stream FSbs */

            VGM_ASSERT(data->streams_size > 1 && current_interleave != current_data_size+current_padding,
                    "MPEG FSB: %i streams with non-constant interleave found @ 0x%08lx\n", data->streams_size, stream->offset);
            break;

        case MPEG_P3D: /* fixed interleave, not frame-aligned (ie. blocks may end/start in part of a frame) */
            current_data_size = data->config.interleave / 4; /* to ensure we don't feed mpg123 too much */
            current_interleave = data->config.interleave; /* fixed interleave (0x400) */
            //todo: last block is smaller that interleave, not sure how it's divided
            break;

        default: /* standard frames (CBR or VBR) */
            if ( !mpeg_get_frame_info(stream->streamfile, stream->offset, &info) )
                goto fail;
            current_data_size = info.frame_size;
            break;
    }
    if (!current_data_size || current_data_size > data->buffer_size) {
        VGM_LOG("MPEG: incorrect data_size 0x%x\n", current_data_size);
        goto fail;
    }


    /* read single frame */
    data->bytes_in_buffer = read_streamfile(data->buffer,stream->offset, current_data_size, stream->streamfile);


    /* update offsets */
    stream->offset += current_data_size + current_padding;

    /* skip interleave once block is done, if defined */
    if (current_interleave && ((stream->offset - stream->channel_start_offset) % current_interleave == 0)) {
        stream->offset += current_interleave * (data->streams_size-1); /* skip a block each stream */
    }

    return 1;
fail:
    return 0;
}


/*****************/
/* FRAME HELPERS */
/*****************/

/**
 * Gets info from a MPEG frame header at offset. Normally you would use mpg123_info but somehow
 * it's wrong at times (maybe because we use an ancient version) so here we do our thing.
 */
int mpeg_get_frame_info(STREAMFILE *streamfile, off_t offset, mpeg_frame_info * info) {
    /* index tables */
    static const int versions[4] = { /* MPEG 2.5 */ 3, /* reserved */ -1,  /* MPEG 2 */ 2, /* MPEG 1 */ 1 };
    static const int layers[4] = { -1,3,2,1 };
    static const int bit_rates[5][16] = { /* [version index ][bit rate index] (0=free, -1=bad) */
            { 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, -1 }, /* MPEG1 Layer I */
            { 0, 32, 48, 56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 384, -1 }, /* MPEG1 Layer II */
            { 0, 32, 40, 48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, -1 }, /* MPEG1 Layer III */
            { 0, 32, 48, 56,  64,  80,  96, 112, 128, 144, 160, 176, 192, 224, 256, -1 }, /* MPEG2/2.5 Layer I */
            { 0,  8, 16, 24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, -1 }, /* MPEG2/2.5 Layer II/III */
    };
    static const int sample_rates[4][4] = { /* [version][sample rate index] */
            { 44100, 48000, 32000, -1}, /* MPEG1 */
            { 22050, 24000, 16000, -1}, /* MPEG2 */
            { 11025, 12000,  8000, -1}, /* MPEG2.5 */
    };
    static const int channels[4] = { 2,2,2, 1 }; /* [channel] */
    static const int frame_samples[3][3] = { /* [version][layer] */
            { 384, 1152, 1152 }, /* MPEG1 */
            { 384, 1152, 576  }, /* MPEG2 */
            { 384, 1152, 576  }  /* MPEG2.5 */
    };

    uint32_t header;
    int idx, padding;


    memset(info, 0, sizeof(*info));

    header = read_32bitBE(offset, streamfile);

    if ((header >> 21) != 0x7FF) /* 31-21: sync */
        goto fail;

    info->version = versions[(header >> 19) & 0x3]; /* 20,19: version */
    if (info->version <= 0) goto fail;

    info->layer = layers[(header >> 17) & 0x3]; /* 18,17: layer */
    if (info->layer <= 0 || info->layer > 3) goto fail;

    //crc       = (header >> 16) & 0x1; /* 16: protected by crc? */

    idx = (info->version==1 ? info->layer-1 : (3 + (info->layer==1 ? 0 : 1)));
    info->bit_rate = bit_rates[idx][(header >> 12) & 0xf]; /* 15-12: bit rate */
    if (info->bit_rate <= 0) goto fail;

    info->sample_rate = sample_rates[info->version-1][(header >> 10) & 0x3]; /* 11-10: sampling rate */
    if (info->sample_rate <= 0) goto fail;

    padding     = (header >>  9) & 0x1; /* 9: padding? */
    //private   = (header >>  8) & 0x1; /* 8: private bit */

    info->channels = channels[(header >>  6) & 0x3]; /* 7,6: channel mode */

    //js_mode   = (header >>  4) & 0x3; /* 5,4: mode extension for joint stereo */
    //copyright = (header >>  3) & 0x1; /* 3: copyrighted */
    //original  = (header >>  2) & 0x1; /* 2: original */
    //emphasis  = (header >>  0) & 0x3; /* 1,0: emphasis */

    info->frame_samples = frame_samples[info->version-1][info->layer-1];

    /* calculate frame length (from hcs's fsb_mpeg) */
    switch (info->frame_samples) {
        case 384:  info->frame_size = (12l  * info->bit_rate * 1000l / info->sample_rate + padding) * 4; break;
        case 576:  info->frame_size = (72l  * info->bit_rate * 1000l / info->sample_rate + padding); break;
        case 1152: info->frame_size = (144l * info->bit_rate * 1000l / info->sample_rate + padding); break;
        default: goto fail;
    }

    return 1;

fail:
    return 0;
}


#endif
