#include "stdafx.h"
#include "Vag.h"

#define SAMPLE_BUFFER_SIZE 0x8000

static size_t make_wav_header(uint8_t * buf, size_t buf_size, int32_t sample_count, int32_t sample_rate, int channels);
void apply_fade(sample_t * buf, VGMSTREAM * vgmstream, int to_get, int i, int len_samples, int fade_samples, int channels);

CLI::Vag::Vag(String ^ filename)
{
	const char* str = (const char*)(Marshal::StringToHGlobalAnsi(filename).ToPointer());
	STREAMFILE* streamFile = open_stdio_streamfile(str);
	if (streamFile)
	{
		VGMSTREAM * vgmstream = NULL;
		FILE * outfile = NULL;

		sample_t * buf = NULL;
		int channels, input_channels;
		int32_t len_samples;
		int32_t fade_samples;

		vgmstream = init_vgmstream_from_STREAMFILE(streamFile);
		close_streamfile(streamFile);
		if (!vgmstream)
		{
			//throw exception
		}

		double loop_count = 2.0;
		double fade_time = 10.0;
		double fade_delay = 0.0;

		int only_stereo = -1;
		int i, j;

		channels = vgmstream->channels;
		input_channels = vgmstream->channels;

		//outfile = fopen();

		len_samples = get_vgmstream_play_samples(loop_count, fade_time, fade_delay, vgmstream);
		fade_samples = (int32_t)(fade_time < 0 ? 0 : fade_time * vgmstream->sample_rate);

		buf = (sample_t*)malloc(SAMPLE_BUFFER_SIZE * sizeof(sample_t) * input_channels);
		if (!buf) {
			fprintf(stderr, "failed allocating output buffer\n");
			//throw exception
		}

		{
			uint8_t wav_buf[0x100];
			int channels_write = channels;
			size_t bytes_done;

			bytes_done = make_wav_header(wav_buf, 0x100,
				len_samples, vgmstream->sample_rate, channels_write);

			fwrite(wav_buf, sizeof(uint8_t), bytes_done, outfile);
		}

		/* decode */
		for (i = 0; i < len_samples; i += SAMPLE_BUFFER_SIZE) {
			int to_get = SAMPLE_BUFFER_SIZE;
			if (i + SAMPLE_BUFFER_SIZE > len_samples)
				to_get = len_samples - i;

			render_vgmstream(buf, to_get, vgmstream);

			apply_fade(buf, vgmstream, to_get, i, len_samples, fade_samples, channels);

			swap_samples_le(buf, channels * to_get); /* write PC endian */
			if (only_stereo != -1) {
				for (j = 0; j < to_get; j++) {
					fwrite(buf + j * channels + (only_stereo * 2), sizeof(sample_t), 2, outfile);
				}
			}
			else {
				fwrite(buf, sizeof(sample_t) * channels, to_get, outfile);
			}
		}

		fclose(outfile);
		outfile = NULL;

		close_vgmstream(vgmstream);
		free(buf);
	}
}

/* make a RIFF header for .wav */
static size_t make_wav_header(uint8_t * buf, size_t buf_size, int32_t sample_count, int32_t sample_rate, int channels) {
	size_t data_size, header_size;

	data_size = sample_count * channels * sizeof(sample_t);
	header_size = 0x2c;

	if (header_size > buf_size)
		goto fail;

	memcpy(buf + 0x00, "RIFF", 4); /* RIFF header */
	put_32bitLE(buf + 4, (int32_t)(header_size - 0x08 + data_size)); /* size of RIFF */

	memcpy(buf + 0x08, "WAVE", 4); /* WAVE header */

	memcpy(buf + 0x0c, "fmt ", 4); /* WAVE fmt chunk */
	put_32bitLE(buf + 0x10, 0x10); /* size of WAVE fmt chunk */
	put_16bitLE(buf + 0x14, 1); /* compression code 1=PCM */
	put_16bitLE(buf + 0x16, channels); /* channel count */
	put_32bitLE(buf + 0x18, sample_rate); /* sample rate */
	put_32bitLE(buf + 0x1c, sample_rate*channels * sizeof(sample_t)); /* bytes per second */
	put_16bitLE(buf + 0x20, (int16_t)(channels * sizeof(sample_t))); /* block align */
	put_16bitLE(buf + 0x22, sizeof(sample_t) * 8); /* significant bits per sample */

	memcpy(buf + 0x24, "data", 0x04); /* WAVE data chunk */
	put_32bitLE(buf + 0x28, (int32_t)data_size); /* size of WAVE data chunk */

/* could try to add channel_layout, but would need to write WAVEFORMATEXTENSIBLE (maybe only if arg flag?) */

	return header_size;
fail:
	return 0;
}

void apply_fade(sample_t * buf, VGMSTREAM * vgmstream, int to_get, int i, int len_samples, int fade_samples, int channels) {
	int is_fade_on = vgmstream->loop_flag;

	if (is_fade_on && fade_samples > 0) {
		int samples_into_fade = i - (len_samples - fade_samples);
		if (samples_into_fade + to_get > 0) {
			int j, k;
			for (j = 0; j < to_get; j++, samples_into_fade++) {
				if (samples_into_fade > 0) {
					double fadedness = (double)(fade_samples - samples_into_fade) / fade_samples;
					for (k = 0; k < channels; k++) {
						buf[j*channels + k] = (sample_t)buf[j*channels + k] * fadedness;
					}
				}
			}
		}
	}
}