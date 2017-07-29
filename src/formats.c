#include "formats.h"

//#define VGM_REGISTER_TYPE(extension) ...
//#define VGM_REGISTER_TYPE_COMMON(extension) ... /* for common extensions like aiff */


/* some extensions could be #ifdef but no really needed */
/* some formats marked as "not parsed" mean they'll go through FFmpeg, the header/extension is not parsed */

static const char* extension_list[] = {
    "04sw",
    "2dx9",
    "2pfs",

    //"aac", //common, also tri-Ace's
    "aa3", //FFmpeg, not parsed (ATRAC3/ATRAC3PLUS/MP3/LPCM/WMA)
    "aaap",
    "aax",
    //"ac3", //FFmpeg, not parsed //common?
    "ace", //fake, for tri-Ace's formats
    "acm",
    "adm",
    "adp",
    "adpcm",
    "ads",
    "adx",
    "afc",
    "agsc",
    "ahx",
    "aifc",
    "aifcl",
    //"aiff", //common
    "aix",
    "akb",
    "amts",
    "as4",
    "asd",
    "asf",
    "asr",
    "ass",
    "ast",
    "at3",
    "aud",
    "aus",

    "b1s",
    "baf",
    "baka",
    "bar",
    "bcstm",
    "bcwav",
    "bdsp",
    "bfstm",
    "bfwav",
    "bfwavnsmbu",
    "bg00",
    "bgm",
    "bgw",
    "bh2pcm",
    "bik",
    "bika",
    "bik2",
    "bik2a",
    "bk2",
    "bk2a",
    "bmdx",
    "bms",
    "bnk",
    "bns",
    "bnsf",
    "bo2",
    "brstm",
    "brstmspm",
    "btsnd",
    "bvg",

    "caf",
    "capdsp",
    "cbd2",
    "ccc",
    "cfn",
    "ckd",
    "cnk",
    "cps",
    "cxs",

    "dbm",
    "dcs",
    "ddsp",
    "de2",
    "dmsg",
    "dsp",
    "dspw",
    "dtk",
    "dvi",
    "dxh",

    "eam",
    "emff",
    "enth",
    "exa",

    "fag",
    "ffw",
    "filp",
    "fsb",
    "fwav",

    "g1l",
    "gbts",
    "gca",
    "gcm",
    "gcub",
    "gcw",
    "genh",
    "gms",
    "gsb",
    "gtd",

    "hca",
    "hgc1",
    "his",
    "hlwav",
    "hps",
    "hsf",
    "hwas",

    "iab",
    "iadp",
    "idsp",
    "idvi",
    "ikm",
    "ild",
    "int",
    "isd",
    "isws",
    "ivaud",
    "ivag",
    "ivb",

    "joe",
    "jstm",

    "kces",
    "kcey",
    "khv",
    "kovs",
    "kraw",

    "laac", //fake extension, for tri-Ace/FFmpeg
    "leg",
    "lmp4", //fake extension, for looping
    "logg", //fake extension, for looping
    "lpcm",
    "lps",
    "lsf",
    "lwav", //fake extension, for looping

    "matx",
    "mc3",
    "mca",
    "mcg",
    "mds",
    "mdsp",
    "med",
    "mi4",
    "mib",
    "mic",
    "mihb",
    "mnstr",
    //"mp4", //common
    "mpdsp",
    "mpds",
    "msa",
    "msf",
    "mss",
    "msvp",
    "mta2",
    "mtaf",
    "mus",
    "musc",
    "musx",
    "mwv",
    "mxst",
    "myspd",

    "ndp",
    "ngca",
    "nps",
    "npsf",
    "nus3bank",
    "nwa",

    //"ogg", //common
    "ogl",
    "oma", //FFmpeg, not parsed (ATRAC3/ATRAC3PLUS/MP3/LPCM/WMA)
    "omu",
    "otm",

    "p1d", //txth/reserved [Farming Simulator 18 (3DS)]
    "p2bt",
    "p3d",
    "past",
    "pcm",
    "pdt",
    "pnb",
    "pona",
    "pos",
    "ps2stm",
    "psh",
    "psnd",
    "psw",

    "rak",
    "ras",
    "raw",
    "rkv",
    "rnd",
    "rrds",
    "rsd",
    "rsf",
    "rstm",
    "rvws",
    "rwar",
    "rwav",
    "rws",
    "rwsd",
    "rwx",
    "rxw",

    "s14",
    "sab",
    "sad",
    "sap",
    "sc",
    "scd",
    "sck",
    "sd9",
    "sdt",
    "seg",
    "sf0",
    "sfl",
    "sfs",
    "sfx",
    "sgb",
    "sgd",
    "sgx",
    "sl3",
    "sli",
    "smp",
    "smpl",
    "snd",
    "snds",
    "sng",
    "sns",
    "spd",
    "spm",
    "sps",
    "spsd",
    "spw",
    "ss2",
    "ss3",
    "ss7",
    "ssm",
    "sss",
    "ster",
    "sth",
    //"stm", //common
    "stma",
    "str",
    "strm",
    "sts",
    "stx",
    "svag",
    "svs",
    "swag",
    "swav",
    "swd",
    "sx",
    "sxd",
    "sxd2",

    "tec",
    "thp",
    "tk1",
    "tk5",
    "tra",
    "tun",
    "tydsp",

    "ulw",
    "um3",

    "vag",
    "vas",
    "vawx",
    "vb",
    "vbk",
    "vgs",
    "vgv",
    "vig",
    "vds",
    "vdm",
    "vms",
    "voi",
    "vpk",
    "vs",
    "vsf",

    "waa",
    "wac",
    "wad",
    "wam",
    "was",
    //"wav", //common
    "wavm",
    "wb",
    "wem",
    "wii",
    "wip", //txth/reserved [Colin McRae DiRT (PC)]
    "wmus",
    "wp2",
    "wpd",
    "wsd",
    "wsi",
    "wv2", //txth/reserved [Slave Zero (PC)]
    "wvs",

    "xa",
    "xa2",
    "xa30",
    "xag",
    "xau",
    "xma",
    "xma2",
    "xmu",
    "xnb",
    "xsf",
    "xss",
    "xvag",
    "xvas",
    "xwav",
    "xwb",
    "xwm", //FFmpeg, not parsed (XWMA)
    "xwma", //FFmpeg, not parsed (XWMA)
    "xws",
    "xwv",

    "ydsp",
    "ymf",

    "zsd",
    "zwdsp",

    "vgmstream"

    //, NULL //end mark
};

/**
 * List of supported formats.
 *
 * For plugins that need to know (test.exe doesn't use it)
 */
const char ** vgmstream_get_formats() {
    return extension_list;
}

/**
 * Number of elements in the list.
 */
int vgmstream_get_formats_length() {
    return sizeof(extension_list) / sizeof(char*);
}


/* internal description info */

typedef struct {
    coding_t type;
    const char *description;
} coding_info;

typedef struct {
    layout_t type;
    const char *description;
} layout_info;

typedef struct {
    meta_t type;
    const char *description;
} meta_info;


static const coding_info coding_info_list[] = {
        {coding_PCM16BE,            "Big Endian 16-bit PCM"},
        {coding_PCM16LE,            "Little Endian 16-bit PCM"},
        {coding_PCM16LE_int,        "Little Endian 16-bit PCM with 2 byte interleave"},
        {coding_PCM16LE_XOR_int,    "Little Endian 16-bit PCM with 2 byte interleave and XOR obfuscation"},
        {coding_PCM8,               "8-bit PCM"},
        {coding_PCM8_U,             "8-bit unsigned PCM"},
        {coding_PCM8_U_int,         "8-bit unsigned PCM with 1 byte interleave"},
        {coding_PCM8_int,           "8-bit PCM with 1 byte interleave"},
        {coding_PCM8_SB_int,        "8-bit PCM with sign bit, 1 byte interleave"},
        {coding_ULAW,               "8-bit u-Law"},
        {coding_CRI_ADX,            "CRI ADX 4-bit ADPCM"},
        {coding_CRI_ADX_exp,        "CRI ADX 4-bit ADPCM with exponential scale"},
        {coding_CRI_ADX_fixed,      "CRI ADX 4-bit ADPCM with fixed coefficients"},
        {coding_CRI_ADX_enc_8,      "CRI ADX 4-bit ADPCM (type 8 encryption)"},
        {coding_CRI_ADX_enc_9,      "CRI ADX 4-bit ADPCM (type 9 encryption)"},
        {coding_NGC_DSP,            "Nintendo DSP 4-bit ADPCM"},
        {coding_NGC_DTK,            "Nintendo DTK 4-bit ADPCM"},
        {coding_NGC_AFC,            "Nintendo AFC 4-bit ADPCM"},
        {coding_CRI_HCA,            "CRI HCA"},
        {coding_NDS_IMA,            "NDS-style 4-bit IMA ADPCM"},
        {coding_DAT4_IMA,           "Eurocom DAT4 4-bit IMA ADPCM"},
        {coding_G721,               "CCITT G.721 4-bit ADPCM"},
        {coding_PSX,                "Playstation 4-bit ADPCM"},
        {coding_PSX_badflags,       "Playstation 4-bit ADPCM (bad flags)"},
        {coding_PSX_bmdx,           "Playstation 4-bit ADPCM (BMDX encryption)"},
        {coding_PSX_cfg,            "Playstation 4-bit ADPCM (configurable)"},
        {coding_HEVAG,              "Playstation Vita HEVAG 4-bit ADPCM"},
        {coding_XA,                 "CD-ROM XA 4-bit ADPCM"},
        {coding_XBOX,               "XBOX 4-bit IMA ADPCM"},
        {coding_XBOX_int,           "XBOX 4-bit IMA ADPCM (interleaved)"},
        {coding_EA_XA,              "Electronic Arts EA-XA 4-bit ADPCM (v1)"},
        {coding_EA_XA_int,          "Electronic Arts EA-XA 4-bit ADPCM (v1) (interleaved)"},
        {coding_EA_XA_V2,           "Electronic Arts EA-XA 4-bit ADPCM (v2)"},
        {coding_MAXIS_XA,           "Maxis EA-XA 4-bit ADPCM"},
        {coding_SDX2,               "Squareroot-delta-exact (SDX2) 8-bit DPCM"},
        {coding_SDX2_int,           "Squareroot-delta-exact (SDX2) 8-bit DPCM with 1 byte interleave"},
        {coding_CBD2,               "Cuberoot-delta-exact (CBD2) 8-bit DPCM"},
        {coding_CBD2_int,           "Cuberoot-delta-exact (CBD2) 8-bit DPCM with 1 byte interleave"},
        {coding_DVI_IMA,            "Intel DVI 4-bit IMA ADPCM"},
        {coding_DVI_IMA_int,        "Intel DVI 4-bit IMA ADPCM (interleaved)"},
        {coding_EACS_IMA,           "EACS 4-bit IMA ADPCM"},
        {coding_IMA_int,            "IMA 4-bit ADPCM (interleaved)"},
        {coding_IMA,                "IMA 4-bit ADPCM"},
        {coding_MS_IMA,             "Microsoft 4-bit IMA ADPCM"},
        {coding_RAD_IMA,            "Radical 4-bit IMA ADPCM"},
        {coding_RAD_IMA_mono,       "Radical 4-bit IMA ADPCM (mono)"},
        {coding_APPLE_IMA4,         "Apple Quicktime 4-bit IMA ADPCM"},
        {coding_SNDS_IMA,           "Heavy Iron .snds 4-bit IMA ADPCM"},
        {coding_OTNS_IMA,           "Omikron: The Nomad Soul 4-bit IMA ADPCM"},
        {coding_FSB_IMA,            "FSB multichannel 4-bit IMA ADPCM"},
        {coding_WWISE_IMA,          "Audiokinetic Wwise 4-bit IMA ADPCM"},
        {coding_REF_IMA,            "Reflections 4-bit IMA ADPCM"},
        {coding_WS,                 "Westwood Studios VBR ADPCM"},
        {coding_ACM,                "InterPlay ACM"},
        {coding_NWA0,               "NWA DPCM Level 0"},
        {coding_NWA1,               "NWA DPCM Level 1"},
        {coding_NWA2,               "NWA DPCM Level 2"},
        {coding_NWA3,               "NWA DPCM Level 3"},
        {coding_NWA4,               "NWA DPCM Level 4"},
        {coding_NWA5,               "NWA DPCM Level 5"},
        {coding_MSADPCM,            "Microsoft 4-bit ADPCM"},
        {coding_AICA,               "Yamaha AICA 4-bit ADPCM"},
        {coding_NDS_PROCYON,        "Procyon Studio Digital Sound Elements NDS 4-bit APDCM"},
        {coding_L5_555,             "Level-5 0x555 4-bit ADPCM"},
        {coding_SASSC,              "Activision / EXAKT SASSC 8-bit DPCM"},
        {coding_LSF,                "lsf 4-bit ADPCM"},
        {coding_MTAF,               "Konami MTAF 4-bit ADPCM"},
        {coding_MTA2,               "Konami MTA2 4-bit ADPCM"},
        {coding_MC3,                "Paradigm MC3 3-bit ADPCM"},
        
#ifdef VGM_USE_VORBIS
        {coding_ogg_vorbis,         "Ogg Vorbis"},
        {coding_VORBIS_custom,      "Custom Vorbis"},
#endif
#ifdef VGM_USE_MPEG
        {coding_MPEG_custom,        "Custom MPEG Audio"},
        {coding_MPEG_layer1,        "MPEG Layer I Audio (MP1)"},
        {coding_MPEG_layer2,        "MPEG Layer II Audio (MP2)"},
        {coding_MPEG_layer3,        "MPEG Layer III Audio (MP3)"},
#endif
#ifdef VGM_USE_G7221
        {coding_G7221,              "ITU G.722.1 (Polycom Siren 7)"},
        {coding_G7221C,             "ITU G.722.1 annex C (Polycom Siren 14)"},
#endif
#ifdef VGM_USE_G719
        {coding_G719,               "ITU G.719 annex B (Polycom Siren 22)"},
#endif
#ifdef VGM_USE_MAIATRAC3PLUS
        {coding_AT3plus,            "ATRAC3plus"},
#endif
#ifdef VGM_USE_FFMPEG
        {coding_FFmpeg,             "FFmpeg"},
#endif
};

static const layout_info layout_info_list[] = {
        {layout_none,                   "flat (no layout)"},
        {layout_interleave,             "interleave"},
        {layout_interleave_shortblock,  "interleave with short last block"},
        {layout_interleave_byte,        "sub-frame interleave"},
        {layout_mxch_blocked,           "MxCh blocked"},
        {layout_ast_blocked,            "AST blocked"},
        {layout_halpst_blocked,         "HALPST blocked"},
        {layout_xa_blocked,             "CD-ROM XA"},
        {layout_ea_blocked,             "Electronic Arts Audio Blocks"},
        {layout_eacs_blocked,           "Electronic Arts (Old Version) Audio Blocks"},
        {layout_caf_blocked,            "CAF blocked"},
        {layout_wsi_blocked,            ".wsi blocked"},
        {layout_xvas_blocked,           ".xvas blocked"},
        {layout_str_snds_blocked,       ".str SNDS blocked"},
        {layout_ws_aud_blocked,         "Westwood Studios .aud blocked"},
        {layout_matx_blocked,           "Matrix .matx blocked"},
        {layout_de2_blocked,            "de2 blocked"},
        {layout_vs_blocked,             "vs blocked"},
        {layout_emff_ps2_blocked,       "EMFF (PS2) blocked"},
        {layout_emff_ngc_blocked,       "EMFF (NGC/WII) blocked"},
        {layout_gsb_blocked,            "GSB blocked"},
        {layout_thp_blocked,            "THP Movie Audio blocked"},
        {layout_filp_blocked,           "FILp blocked"},
        {layout_psx_mgav_blocked,       "MGAV blocked"},
        {layout_ps2_adm_blocked,        "ADM blocked"},
        {layout_dsp_bdsp_blocked,       "DSP blocked"},
        {layout_ivaud_blocked,          "GTA IV blocked"},
        {layout_ps2_iab_blocked,        "IAB blocked"},
        {layout_ps2_strlr_blocked,      "The Bouncer STR blocked"},
        {layout_rws_blocked,            "RWS blocked"},
        {layout_hwas_blocked,           "HWAS blocked"},
        {layout_tra_blocked,            "TRA blocked"},
        {layout_acm,                    "ACM blocked"},
        {layout_mus_acm,                "multiple ACM files, ACM blocked"},
        {layout_aix,                    "AIX interleave, internally 18-byte interleaved"},
        {layout_aax,                    "AAX blocked, 18-byte interleaved"},
        {layout_scd_int,                "SCD multistream interleave"},
#ifdef VGM_USE_VORBIS
        {layout_ogg_vorbis,             "Ogg"},
#endif
#ifdef VGM_USE_MPEG
        {layout_mpeg_custom,            "Custom MPEG Audio"},
#endif
};

static const meta_info meta_info_list[] = {
        {meta_RSTM,                 "Nintendo RSTM header"},
        {meta_STRM,                 "Nintendo STRM header"},
        {meta_ADX_03,               "CRI ADX header type 03"},
        {meta_ADX_04,               "CRI ADX header type 04"},
        {meta_ADX_05,               "CRI ADX header type 05"},
        {meta_AIX,                  "CRI AIX header"},
        {meta_AAX,                  "CRI AAX header"},
        {meta_UTF_DSP,              "CRI ADPCM_WII header"},
        {meta_DSP_AGSC,             "Retro Studios AGSC header"},
        {meta_DSP_CSMP,             "Retro Studios CSMP header"},
        {meta_NGC_ADPDTK,           "assumed Nintendo ADP by .adp extension and valid first frame"},
        {meta_RSF,                  "assumed Retro Studios RSF by .rsf extension and valid first bytes"},
        {meta_AFC,                  "Nintendo AFC header"},
        {meta_AST,                  "Nintendo AST header"},
        {meta_HALPST,               "HAL Laboratory HALPST header"},
        {meta_DSP_RS03,             "Retro Studios RS03 header"},
        {meta_DSP_STD,              "Standard Nintendo DSP header"},
        {meta_DSP_CSTR,             "Namco Cstr header"},
        {meta_GCSW,                 "GCSW header"},
        {meta_PS2_SShd,             "SShd header"},
        {meta_PS2_NPSF,             "Namco Production Sound File (NPSF) header"},
        {meta_RWSD,                 "Nintendo RWSD header (single stream)"},
        {meta_RWAR,                 "Nintendo RWAR header (single RWAV stream)"},
        {meta_RWAV,                 "Nintendo RWAV header"},
        {meta_CWAV,                 "Nintendo CWAV header"},
        {meta_FWAV,                 "Nintendo FWAV header"},
        {meta_PSX_XA,               "RIFF/CDXA header"},
        {meta_PS2_RXWS,             "Sony RXWS header"},
        {meta_PS2_RAW,              "assumed RAW Interleaved PCM by .int extension"},
        {meta_PS2_OMU,              "Alter Echo OMU Header"},
        {meta_DSP_STM,              "Nintendo STM header"},
        {meta_PS2_EXST,             "EXST header"},
        {meta_PS2_SVAG,             "Konami SVAG header"},
        {meta_PS2_MIB,              "assumed MIB Interleaved file by .mib extension"},
        {meta_PS2_MIB_MIH,          "assumed MIB with MIH Info Header file by .mib+.mih extension"},
        {meta_DSP_MPDSP,            "Single DSP header stereo by .mpdsp extension"},
        {meta_PS2_MIC,              "assume KOEI MIC file by .mic extension"},
        {meta_DSP_JETTERS,          "Double DSP header stereo by _lr.dsp extension"},
        {meta_DSP_MSS,              "Double DSP header stereo by .mss extension"},
        {meta_DSP_GCM,              "Double DSP header stereo by .gcm extension"},
        {meta_DSP_WII_IDSP,         "Wii IDSP Double DSP header"},
        {meta_RSTM_SPM,             "Nintendo RSTM header and .brstmspm extension"},
        {meta_RAW,                  "assumed RAW PCM file by .raw extension"},
        {meta_PS2_VAGi,             "Sony VAG Interleaved header (VAGi)"},
        {meta_PS2_VAGp,             "Sony VAG Mono header (VAGp)"},
        {meta_PS2_VAGs,             "Sony VAG Stereo header (VAGp)"},
        {meta_PS2_VAGm,             "Sony VAG Mono header (VAGm)"},
        {meta_PS2_pGAV,             "Sony VAG Stereo Little Endian header (pGAV)"},
        {meta_PSX_GMS,              "assumed Grandia GMS file by .gms extension"},
        {meta_PS2_STR,              "assumed STR + STH File by .str & .sth extension"},
        {meta_PS2_ILD,              "ILD header"},
        {meta_PS2_PNB,              "assumed PNB (PsychoNauts Bgm File) by .pnb extension"},
        {meta_XBOX_WAVM,            "assumed Xbox WAVM file by .wavm extension"},
        {meta_XBOX_RIFF,            "Microsoft XWAV RIFF header"},
        {meta_DSP_STR,              "assumed Conan Gamecube STR File by .str extension"},
        {meta_EA_SCHL,              "Electronic Arts SCHl header (variable)"},
        {meta_EA_SCHL_fixed,        "Electronic Arts SCHl header (fixed)"},
        {meta_CFN,                  "tri-Crescendo CAF Header"},
        {meta_PS2_VPK,              "VPK Header"},
        {meta_GENH,                 "GENH Generic Header"},
        {meta_DSP_SADB,             "sadb header"},
        {meta_SADL,                 "sadl header"},
        {meta_PS2_BMDX,             "Beatmania .bmdx header"},
        {meta_DSP_WSI,              ".wsi header"},
        {meta_AIFC,                 "Audio Interchange File Format AIFF-C"},
        {meta_AIFF,                 "Audio Interchange File Format"},
        {meta_STR_SNDS,             ".str SNDS SHDR chunk"},
        {meta_WS_AUD,               "Westwood Studios .aud header"},
        {meta_WS_AUD_old,           "Westwood Studios .aud (old) header"},
        {meta_PS2_IVB,              "IVB/BVII header"},
        {meta_PS2_SVS,              "Square SVS header"},
        {meta_RIFF_WAVE,            "RIFF WAVE header"},
        {meta_RIFF_WAVE_POS,        "RIFF WAVE header and .pos for looping"},
        {meta_NWA,                  "Visual Art's NWA header"},
        {meta_NWA_NWAINFOINI,       "Visual Art's NWA header and NWAINFO.INI for looping"},
        {meta_NWA_GAMEEXEINI,       "Visual Art's NWA header and Gameexe.ini for looping"},
        {meta_XSS,                  "Dino Crisis 3 XSS File"},
        {meta_HGC1,                 "Knights of the Temple 2 hgC1 Header"},
        {meta_AUS,                  "Capcom AUS Header"},
        {meta_RWS,                  "RenderWare RWS header"},
        {meta_EACS_PC,              "Electronic Arts EACS header (PC)"},
        {meta_EACS_PSX,             "Electronic Arts EACS header (PSX)"},
        {meta_EACS_SAT,             "Electronic Arts EACS header (SATURN)"},
        {meta_SL3,                  "SL3 Header"},
        {meta_FSB1,                 "FMOD Sample Bank (FSB1) Header"},
        {meta_FSB2,                 "FMOD Sample Bank (FSB2) Header"},
        {meta_FSB3,                 "FMOD Sample Bank (FSB3) Header"},
        {meta_FSB4,                 "FMOD Sample Bank (FSB4) Header"},
        {meta_FSB5,                 "FMOD Sample Bank (FSB5) Header"},
        {meta_RWX,                  "RWX Header"},
        {meta_XWB,                  "Microsoft XWB Header"},
        {meta_PS2_XA30,             "Reflections XA30 PS2 header"},
        {meta_MUSC,                 "MUSC Header"},
        {meta_MUSX_V004,            "MUSX / Version 004 Header"},
        {meta_MUSX_V005,            "MUSX / Version 005 Header"},
        {meta_MUSX_V006,            "MUSX / Version 006 Header"},
        {meta_MUSX_V010,            "MUSX / Version 010 Header"},
        {meta_MUSX_V201,            "MUSX / Version 201 Header"},
        {meta_LEG,                  "Legaia 2 - Duel Saga LEG Header"},
        {meta_FILP,                 "Bio Hazard - Gun Survivor FILp Header"},
        {meta_IKM,                  "Zwei!! IKM Header"},
        {meta_SFS,                  "Baroque SFS Header"},
        {meta_DVI,                  "DVI Header"},
        {meta_KCEY,                 "KCEYCOMP Header"},
        {meta_BG00,                 "Falcom BG00 Header"},
        {meta_PS2_RSTM,             "Rockstar Games RSTM Header"},
        {meta_ACM,                  "InterPlay ACM Header"},
        {meta_MUS_ACM,              "MUS playlist and multiple InterPlay ACM Headered files"},
        {meta_PS2_KCES,             "Konami KCES Header"},
        {meta_PS2_DXH,              "Tokobot Plus DXH Header"},
        {meta_PS2_PSH,              "Dawn of Mana - Seiken Densetsu 4 PSH Header"},
        {meta_RIFF_WAVE_labl,       "RIFF WAVE header with loop markers"},
        {meta_RIFF_WAVE_smpl,       "RIFF WAVE header with sample looping info"},
        {meta_RIFX_WAVE,            "RIFX WAVE header"},
        {meta_RIFX_WAVE_smpl,       "RIFX WAVE header with sample looping info"},
        {meta_XNBm,                 "XNBm header"},
        {meta_PCM_SCD,              "PCM file with custom header (SCD)"},
        {meta_PCM_PS2,              "PCM file with custom header (PS2)"},
        {meta_PS2_RKV,              "Legacy of Kain - Blood Omen 2 RKV Header"},
        {meta_PS2_PSW,              "Rayman Raving Rabbids Riff Container File"},
        {meta_PS2_VAS,              "Pro Baseball Spirits 5 VAS Header"},
        {meta_PS2_TEC,              "assumed TECMO badflagged stream by .tec extension"},
        {meta_XBOX_WVS,             "Metal Arms WVS Header (XBOX)"},
        {meta_NGC_WVS,              "Metal Arms WVS Header (GameCube)"},
        {meta_XBOX_STMA,            "Midnight Club 2 STMA Header"},
        {meta_XBOX_MATX,            "assumed Matrix file by .matx extension"},
        {meta_DE2,                  "gurumin .de2 with embedded funky RIFF"},
        {meta_VS,                   "Men in Black VS Header"},
        {meta_DC_STR,               "Sega Stream Asset Builder header"},
        {meta_DC_STR_V2,            "variant of Sega Stream Asset Builder header"},
        {meta_XBOX_XMU,             "XMU header"},
        {meta_XBOX_XVAS,            "assumed TMNT file by .xvas extension"},
        {meta_PS2_XA2,              "Acclaim XA2 Header"},
        {meta_DC_IDVI,              "IDVI Header"},
        {meta_NGC_YMF,              "YMF DSP Header"},
        {meta_PS2_CCC,              "CCC Header"},
        {meta_PSX_FAG,              "FAG Header"},
        {meta_PS2_MIHB,             "Merged MIH+MIB"},
        {meta_DSP_WII_MUS,          "mus header"},
        {meta_WII_SNG,              "SNG DSP Header"},
        {meta_RSD2VAG,              "RSD2/VAG Header"},
        {meta_RSD2PCMB,             "RSD2/PCMB Header"},
        {meta_RSD2XADP,             "RSD2/XADP Header"},
        {meta_RSD3VAG,              "RSD3/VAG Header"},
        {meta_RSD3GADP,             "RSD3/GADP Header"},
        {meta_RSD3PCM,              "RSD3/PCM Header"},
        {meta_RSD3PCMB,             "RSD3/PCMB Header"},
        {meta_RSD4PCMB,             "RSD4/PCMB Header"},
        {meta_RSD4PCM,              "RSD4/PCM Header"},
        {meta_RSD4RADP,             "RSD4/RADP Header"},
        {meta_RSD4VAG,              "RSD4/VAG Header"},
        {meta_RSD6XADP,             "RSD6/XADP Header"},
        {meta_RSD6VAG,              "RSD6/VAG Header"},
        {meta_RSD6WADP,             "RSD6/WADP Header"},
        {meta_RSD6RADP,             "RSD6/RADP Header"},
        {meta_RSD6XMA,              "RSD6/XMA Header"},
        {meta_DC_ASD,               "ASD Header"},
        {meta_NAOMI_SPSD,           "SPSD Header"},
        {meta_FFXI_BGW,             "BGW BGMStream header"},
        {meta_FFXI_SPW,             "SPW SeWave header"},
        {meta_PS2_ASS,              "ASS Header"},
        {meta_IDSP,                 "IDSP Header"},
        {meta_WAA_WAC_WAD_WAM,      "WAA/WAC/WAD/WAM RIFF Header"},
        {meta_PS2_SEG,              "SEG (PS2) Header"},
        {meta_XBOX_SEG,             "SEG (XBOX) Header"},
        {meta_NDS_STRM_FFTA2,       "Final Fantasy Tactics A2 RIFF Header"},
        {meta_STR_ASR,              "Donkey Kong Jet Race KNON/WII Header"},
        {meta_ZWDSP,                "Zack and Wiki custom DSP Header"},
        {meta_GCA,                  "GCA DSP Header"},
        {meta_SPT_SPD,              "SPT+SPD DSP Header"},
        {meta_ISH_ISD,              "ISH+ISD DSP Header"},
        {meta_GSP_GSB,              "Tecmo GSP+GSB Header"},
        {meta_YDSP,                 "Yuke's DSP (YDSP) Header"},
        {meta_MSVP,                 "MSVP Header"},
        {meta_NGC_SSM,              "SSM DSP Header"},
        {meta_PS2_JOE,              "Disney/Pixar JOE Header"},
        {meta_VGS,                  "Guitar Hero Encore Rocks the 80's Header"},
        {meta_DC_DCSW_DCS,          "Evil Twin DCS file with helper"},
        {meta_WII_SMP,              "SMP DSP Header"},
        {meta_EMFF_PS2,             "Eidos Music File Format Header"},
        {meta_EMFF_NGC,             "Eidos Music File Format Header"},
        {meta_THP,                  "THP Movie File Format Header"},
        {meta_STS_WII,              "Shikigami no Shiro (WII) Header"},
        {meta_PS2_P2BT,             "Pop'n'Music 7 Header"},
        {meta_PS2_GBTS,             "Pop'n'Music 9 Header"},
        {meta_NGC_DSP_IADP,         "IADP Header"},
        {meta_RSTM_shrunken,        "Nintendo RSTM header, corrupted by Atlus"},
        {meta_RIFF_WAVE_MWV,        "RIFF WAVE header with .mwv flavoring"},
        {meta_RIFF_WAVE_SNS,        "RIFF WAVE header with .sns flavoring"},
        {meta_FFCC_STR,             "Final Fantasy: Crystal Chronicles STR header"},
        {meta_SAT_BAKA,             "BAKA header from Crypt Killer"},
        {meta_NDS_SWAV,             "SWAV Header"},
        {meta_PS2_VSF,              "Musashi: Samurai Legend VSF Header"},
        {meta_NDS_RRDS,             "Ridger Racer DS Header"},
        {meta_PS2_TK5,              "Tekken 5 Stream Header"},
        {meta_PS2_SND,              "Might and Magic SSND Header"},
        {meta_PS2_VSF_TTA,          "VSF with SMSS Header"},
        {meta_ADS,                  "dhSS Header"},
        {meta_WII_STR,              "HOTD Overkill - STR+STH WII Header"},
        {meta_PS2_MCG,              "Gunvari MCG Header"},
        {meta_ZSD,                  "ZSD Header"},
        {meta_RedSpark,             "RedSpark Header"},
        {meta_PC_IVAUD,             "assumed GTA IV Audio file by .ivaud extension"},
        {meta_DSP_WII_WSD,          "Standard Nintendo DSP headers in .wsd"},
        {meta_WII_NDP,              "Vertigo NDP Header"},
        {meta_PS2_SPS,              "Ape Escape 2 SPS Header"},
        {meta_PS2_XA2_RRP,          "Acclaim XA2 Header"},
        {meta_NDS_HWAS,             "Vicarious Visions HWAS header"},
        {meta_NGC_LPS,              "Rave Master LPS Header"},
        {meta_NAOMI_ADPCM,          "NAOMI/NAOMI2 Arcade games ADPCM header"},
        {meta_SD9,                  "beatmania IIDX SD9 header"},
        {meta_2DX9,                 "beatmania IIDX 2DX9 header"},
        {meta_DSP_YGO,              "Konami custom DSP Header"},
        {meta_PS2_VGV,              "Rune: Viking Warlord VGV Header"},
        {meta_NGC_GCUB,             "GCub Header"},
        {meta_NGC_SCK_DSP,          "The Scorpion King SCK Header"},
        {meta_NGC_SWD,              "PSF + Standard DSP Headers"},
        {meta_CAFF,                 "Apple Core Audio Format Header"},
        {meta_PC_MXST,              "Lego Island MxSt Header"},
        {meta_PC_SOB_SAB,           "Worms 4: Mayhem SOB/SAB Header"},
        {meta_MAXIS_XA,             "Maxis XAI/XAJ Header"},
        {meta_EXAKT_SC,             "assumed Activision / EXAKT SC by extension"},
        {meta_WII_BNS,              "Nintendo BNS header"},
        {meta_WII_WAS,              "WAS (iSWS) DSP header"},
        {meta_XBOX_HLWAV,           "Half Life 2 bgm header"},
        {meta_STX,                  "Nintendo .stx header"},
        {meta_PS2_STM,              "Red Dead Revolver .stm (.ps2stm)"},
        {meta_MYSPD,                "U-Sing .myspd header"},
        {meta_HIS,                  "Her Interactive Sound header"},
        {meta_PS2_AST,              "KOEI AST header"},
        {meta_CAPDSP,               "Capcom DSP header"},
        {meta_DMSG,                 "RIFF/DMSGsegh header"},
        {meta_PONA_3DO,             "Policenauts BGM header"},
        {meta_PONA_PSX,             "Policenauts BGM header"},
        {meta_NGC_DSP_AAAP,         "Double standard DSP header in 'AAAp'"},
        {meta_NGC_DSP_KONAMI,       "Konami DSP header"},
        {meta_PS2_STER,             "STER Header"},
        {meta_BNSF,                 "Namco Bandai BNSF header"},
        {meta_PS2_WB,               "Shooting Love. ~TRIZEAL~ WB header"},
        {meta_S14,                  "assumed Polycom Siren 14 by .s14 extension"},
        {meta_SSS,                  "assumed Polycom Siren 14 by .sss extension"},
        {meta_PS2_GCM,              "GCM 'MCG' Header"},
        {meta_PS2_SMPL,             "Homura 'SMPL' Header"},
        {meta_PS2_MSA,              "Psyvariar -Complete Edition- MSA header"},
        {meta_PC_SMP,               "Ghostbusters .smp Header"},
        {meta_NGC_PDT,              "PDT DSP header"},
        {meta_NGC_BO2,              "Blood Omen 2 DSP header"},
        {meta_P3D,                  "Radical P3D Header"},
        {meta_PS2_TK1,              "Tekken TK5STRM1 Header"},
        {meta_PS2_ADSC,             "ADSC Header"},
        {meta_NGC_DSP_MPDS,         "MPDS DSP header"},
        {meta_DSP_STR_IG,           "Infogrames dual dsp header"},
        {meta_PSX_MGAV,             "Electronic Arts RVWS header"},
        {meta_PS2_B1S,              "B1S header"},
        {meta_PS2_WAD,              "WAD header"},
        {meta_DSP_XIII,             "XIII dsp header"},
        {meta_NGC_DSP_STH_STR,      "STH dsp header"},
        {meta_DSP_CABELAS,          "Cabelas games dsp header"},
        {meta_PS2_LPCM,             "LPCM header"},
        {meta_PS2_VMS,              "VMS Header"},
        {meta_XAU,                  "XPEC XAU header"},
        {meta_GH3_BAR,              "Guitar Hero III Mobile .bar"},
        {meta_FFW,                  "Freedom Fighters BGM header"},
        {meta_DSP_DSPW,             "DSPW dsp header"},
        {meta_PS2_JSTM,             "JSTM Header"},
        {meta_PS3_XVAG,             "XVAG Header"},
        {meta_PS3_CPS,              "tri-Crescendo CPS Header"},
        {meta_SQEX_SCD,             "Square-Enix SCD header"},
        {meta_NGC_NST_DSP,          "Animaniacs NST header"},
        {meta_BAF,                  ".baf WAVE header"},
        {meta_PS3_MSF,              "Sony MSF header"},
        {meta_NUB_VAG,              "Namco NUB VAG header"},
        {meta_PS3_PAST,             "SNDP header"},
        {meta_SGXD,                 "Sony SGXD header"},
        {meta_NGCA,                 "NGCA header"},
        {meta_WII_RAS,              "RAS header"},
        {meta_PS2_SPM,              "SPM header"},
        {meta_X360_TRA,             "assumed DefJam Rapstar Audio File by .tra extension"},
        {meta_PS2_VGS,              "Princess Soft VGS header"},
        {meta_PS2_IAB,              "IAB header"},
        {meta_PS2_STRLR,            "STR L/R header"},
        {meta_LSF_N1NJ4N,           ".lsf !n1nj4n header"},
        {meta_VAWX,                 "feelplus VAWX header"},
        {meta_PC_SNDS,              "assumed Heavy Iron IMA by .snds extension"},
        {meta_PS2_WMUS,             "assumed The Warriors Sony ADPCM by .wmus extension"},
        {meta_HYPERSCAN_KVAG,       "Mattel Hyperscan KVAG"},
        {meta_IOS_PSND,             "PSND Header"},
        {meta_BOS_ADP,              "ADP! header"},
        {meta_OTNS_ADP,             "Omikron: The Nomad Soul ADP header"},
        {meta_EB_SFX,               "Excitebots .sfx header"},
        {meta_EB_SF0,               "assumed Excitebots .sf0 by extension"},
        {meta_PS3_KLBS,             "klBS Header"},
        {meta_PS2_MTAF,             "Konami MTAF header"},
        {meta_PS2_VAG1,             "Konami VAG Mono header (VAG1)"},
        {meta_PS2_VAG2,             "Konami VAG Stereo header (VAG2)"},
        {meta_TUN,                  "TUN 'ALP' header"},
        {meta_WPD,                  "WPD 'DPW' header"},
        {meta_MN_STR,               "Mini Ninjas 'STR' header"},
        {meta_MSS,                  "Guerilla MCSS header"},
        {meta_PS2_HSF,              "Lowrider 'HSF' header"},
        {meta_PS3_IVAG,             "PS3 'IVAG' Header"},
        {meta_PS2_2PFS,             "Konami 2PFS header"},
        {meta_RSD6OOGV,             "RSD6/OOGV Header"},
        {meta_UBI_CKD,              "Ubisoft CKD RIFF header"},
        {meta_PS2_VBK,              "PS2 VBK Header"},
        {meta_OTM,                  "Otomedius OTM Header"},
        {meta_CSTM,                 "Nintendo 3DS CSTM Header"},
        {meta_FSTM,                 "Nintendo Wii U FSTM Header"},
        {meta_KT_WIIBGM,            "Koei Tecmo WiiBGM Header"},
        {meta_3DS_IDSP,             "Nintendo IDSP Header"},
        {meta_WIIU_BTSND,           "Nintendo Wii U Menu Boot Sound"},
        {meta_MCA,                  "Capcom MCA header"},
        {meta_XB3D_ADX,             "Xenoblade 3D ADX header"},
        {meta_HCA,                  "CRI MiddleWare HCA Header"},
        {meta_PS2_SVAG_SNK,         "SNK SVAG header"},
        {meta_PS2_VDS_VDM,          "Graffiti Kingdom VDS/VDM header"},
        {meta_X360_CXS,             "tri-Crescendo CXS header"},
        {meta_AKB,                  "Square-Enix AKB header"},
        {meta_NUB_XMA,              "Namco NUB XMA header"},
        {meta_X360_PASX,            "Namco PASX header"},
        {meta_XMA_RIFF,             "Microsoft XMA RIFF header"},
        {meta_X360_AST,             "Capcom AST header"},
        {meta_WWISE_RIFF,           "Audiokinetic Wwise RIFF header"},
        {meta_UBI_RAKI,             "Ubisoft RAKI header"},
        {meta_SXD,                  "Sony SXD header"},
        {meta_OGL,                  "Shin'en OGL header"},
        {meta_MC3,                  "Paradigm MC3 header"},
        {meta_GTD,                  "GTD/GHS header"},
        {meta_TA_AAC_X360,          "tri-Ace AAC (X360) header"},
        {meta_TA_AAC_PS3,           "tri-Ace AAC (PS3) header"},
        {meta_PS3_MTA2,             "Konami MTA2 header"},
        {meta_NGC_ULW,              "Criterion ULW raw header"},
        {meta_PC_XA30,              "Reflections XA30 PC header"},
        {meta_WII_04SW,             "Reflections 04SW header"},
        {meta_TXTH,                 "TXTH Generic Header"},
        {meta_EA_BNK,               "Electronic Arts BNK header"},
        {meta_SK_AUD,               "Silicon Knights AUD header"},
        {meta_AHX,                  "CRI AHX header"},

#ifdef VGM_USE_VORBIS
        {meta_OGG_VORBIS,           "Ogg Vorbis"},
        {meta_OGG_SLI,              "Ogg Vorbis with .sli (start,length) for looping"},
        {meta_OGG_SLI2,             "Ogg Vorbis with .sli (from,to) for looping"},
        {meta_OGG_SFL,              "Ogg Vorbis with SFPL for looping"},
        {meta_OGG_UM3,              "Ogg Vorbis, Ultramarine3 'encryption'"},
        {meta_OGG_KOVS,             "Ogg Vorbis, KOVS header"},
        {meta_OGG_PSYCH,            "Ogg Vorbis, Psychic Software obfuscation"},
    #endif
#ifdef VGM_USE_MP4V2
        {meta_MP4,                  "AAC header"},
#endif
#ifdef VGM_USE_FFMPEG
        {meta_FFmpeg,               "FFmpeg supported file format"},
#endif
};


const char * get_vgmstream_coding_description(coding_t coding_type) {
    int i, list_length;

    list_length = sizeof(coding_info_list) / sizeof(coding_info);
    for (i=0; i < list_length; i++) {
        if (coding_info_list[i].type == coding_type)
            return coding_info_list[i].description;
    }

    return NULL;
}
const char * get_vgmstream_layout_description(layout_t layout_type) {
    int i, list_length;

    list_length = sizeof(layout_info_list) / sizeof(layout_info);
    for (i=0; i < list_length; i++) {
        if (layout_info_list[i].type == layout_type)
            return layout_info_list[i].description;
    }

    return NULL;
}
const char * get_vgmstream_meta_description(meta_t meta_type) {
    int i, list_length;

    list_length = sizeof(meta_info_list) / sizeof(meta_info);
    for (i=0; i < list_length; i++) {
        if (meta_info_list[i].type == meta_type)
            return meta_info_list[i].description;
    }

    return NULL;
}
