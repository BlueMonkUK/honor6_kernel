

#ifndef _BSP_AUDIO_H_
#define _BSP_AUDIO_H_

#ifdef __cplusplus
extern "C" { /* allow C++ to use these headers */
#endif /* __cplusplus */

#define AUDIO_OK        (0)
#define AUDIO_ERROR     (-1)



typedef struct
{
    UINT16 * usBufA;
    UINT32   ulBufALen;
    UINT16 * usBufB;
    UINT32   ulBufBLen;

}AUDIO_PCM_DATA;

/*usBufA��usBufBΪͬ���ɼ������ݣ�usBufC��usBufDΪͬ�����ŵ�����*/
typedef struct
{
    UINT16 * usBufA;
    UINT32   ulBufALen;
    UINT16 * usBufB;
    UINT32   ulBufBLen;
    UINT16 * usBufC;
    UINT32   ulBufCLen;
    UINT16 * usBufD;
    UINT32   ulBufDLen;

}AUDIO_READ_WRITE_DATA;

typedef enum
{
    AUDIO_VER_NV_ID=0x4008,
    AUDIO_VOICE_HEADSET_NV_ID  = 12299,
    AUDIO_VOICE_HANDSET_NV_ID  = 12300,
    AUDIO_VOICE_HANDFREE_NV_ID = 12301,
    AUDIO_MEDIA_HEADSET_NV_ID,
    AUDIO_MEDIA_HANDSET_NV_ID,
    AUDIO_MEDIA_HANDFREE_NV_ID,
    AUDIO_LINE_NV_ID
}AUDIO_NV_ID;

typedef union
{
    unsigned int dword;
    struct _bits_
    {
        unsigned int dac_sb_filt :1;  /* 0=normal mode, 1= sloping stopband mode */
        unsigned int adc_hpf_ena :1;  /* 0=disable, 1=enable */
        unsigned int adc_hpf_cut :2;  /* cut-off freq */
    }bits;
}FILTER;


/*codec�汾��*/
typedef struct _AUDIO_VER_NV_STRU_
{
      unsigned int uCodecVersion;

    /*���߻ػ�����ʱ���ػ��������ӳ�ʱ�䣬��λ250ms.����ȡ1~47����С�ӳ�250ms������ӳ�ʱ��Լ12s*/
    /*��Ҫ�����buffer�ռ�Ϊ0x1000*48��0x30000Byte*/
    unsigned int tCBDelayTime;
}AUDIO_VER_NV_STRU;

/*����HEADSETģʽ�µ�NV������*/
typedef struct _AUDIO_VOICE_HEADSET_NV_STRU_
{
    unsigned int down_lr_outvol;
    unsigned int up_rin12vol;
    unsigned int up_r12mnbst;
    FILTER       filter;
}AUDIO_VOICE_HEADSET_NV_STRU;

/*����HANDSETģʽ�µ�NV������*/
typedef struct _AUDIO_VOICE_HANDSET_NV_STRU_
{
    unsigned int down_r_outvol;
    unsigned int down_r_opgavol;
    unsigned int down_out4attn;
    unsigned int up_lin12vol;
    unsigned int up_l12mnbst;
    FILTER       filter;
}AUDIO_VOICE_HANDSET_NV_STRU;

/*����HANDFREEģʽ�µ�NV������*/
typedef struct _AUDIO_VOICE_HANDFREE_NV_STRU_
{
    unsigned int down_lr_opgavol;
    unsigned int down_spkattn;
    unsigned int down_spkvol;
    unsigned int down_dcgain;
    unsigned int down_acgain;
    unsigned int up_lin12vol;
    unsigned int up_l12mnbst;
    FILTER       filter;
}AUDIO_VOICE_HANDFREE_NV_STRU;

/*��ƵHEASETģʽ�µ�NV������*/
typedef struct _AUDIO_MEDIA_HEADSET_NV_STRU_
{
    unsigned int down_lr_outvol;
    unsigned int up_rin12vol;
    unsigned int up_r12mnbst;
    FILTER       filter;
}AUDIO_MEDIA_HEADSET_NV_STRU;

/*��ƵHANDSETģʽ�µ�NV������*/
typedef struct _AUDIO_MEDIA_HANDSET_NV_STRU_
{
    unsigned int down_r_outvol;
    unsigned int down_r_opgavol;
    unsigned int down_out4attn;
    unsigned int up_lin12vol;
    unsigned int up_l12mnbst;
    FILTER       filter;
}AUDIO_MEDIA_HANDSET_NV_STRU;

/*��ƵHANDFREEģʽ�µ�NV������*/
typedef struct _AUDIO_MEDIA_HANDFREE_NV_STRU_
{
    unsigned int down_lr_opgavol;
    unsigned int down_spkattn;
    unsigned int down_spkvol;
    unsigned int down_dcgain;
    unsigned int down_acgain;
    unsigned int up_lin12vol;
    unsigned int up_l12mnbst;
    FILTER       filter;
}AUDIO_MEDIA_HANDFREE_NV_STRU;


/*LINEIN��LINEOUTģʽ�µ�NV������*/
typedef struct _ADUIO_LINE_NV_STRU_
{
    unsigned int linein_gain;
    unsigned int lineout_gain;
}AUDIO_LINE_NV_STRU;



/* NV ������Ч����ṹ��*/
  /*��������ֵ*/
typedef union _NV_UP_VOL_
{
    struct _up_hp_
    {
        unsigned int rin12vol;
        unsigned int r12mnbst;
    }up_hp;
    struct _up_hs_
    {
        unsigned int lin12vol;
        unsigned int l12mnbst;
    }up_hs;
    struct _up_hf_
    {
        unsigned int lin12vol;
        unsigned int l12mnbst;
    }up_hf;
}NV_UP_VOL;

  /*��������ֵ*/
typedef union _NV_DN_VOL_
{
    struct _dn_hp_
    {
        unsigned int lr_outvol;
    }dn_hp;
    struct _dn_hs_
    {
        unsigned int routvol;
        unsigned int ropgavol;
        unsigned int out4attn;
    }dn_hs;
    struct _dn_hf_
    {
        unsigned int lr_opgavol;
        unsigned int spkattn;
        unsigned int spkvol;
        unsigned int dcgain;
        unsigned int acgain;
    }dn_hf;
}NV_DN_VOL;

/*������Ч������ͨ�ýṹ��*/
typedef struct _AUDIO_NV_EFFECTIVE_
{
    unsigned int nvcmd;       /*NV��д���1����2д*/
    unsigned int scene;       /*����*/
    unsigned int indev;       /*�����豸*/
    unsigned int outdev;      /*����豸*/
    /*unsigned int upvol[2];*/    /*��������*/
    NV_UP_VOL upvol;
    /*unsigned int downvol[5]; */ /*��������*/
    NV_DN_VOL downvol;
    FILTER filter;      /*�˲���*/
} AUDIO_NV_EFFECTIVE;


extern AUDIO_VOICE_HANDSET_NV_STRU  struVHandsetNV;
extern AUDIO_VOICE_HANDFREE_NV_STRU struVHandfreeNV;
extern AUDIO_VOICE_HEADSET_NV_STRU  struVHeadsetNV;
extern AUDIO_MEDIA_HANDSET_NV_STRU  struMHandsetNV;
extern AUDIO_MEDIA_HANDFREE_NV_STRU struMHandfreeNV;
extern AUDIO_MEDIA_HEADSET_NV_STRU  struMHeadsetNV;
extern AUDIO_LINE_NV_STRU     struLineNV;
extern AUDIO_VER_NV_STRU      struVerNV;
extern unsigned int bEnAudioNV;

typedef enum{
    HI_CODEC_ERR = 0,
    HI_CODEC_INFO,
    HI_CODEC_WARN,
    HI_CODEC_DEBUG,
    HI_CODEC_MAX
}codec_level_e;

#define codec_trace(level, format, args...) do { \
	if((level) <= HI_CODEC_ERR) { \
		printf("%s:%d: "format,  __FUNCTION__,  __LINE__, ## args); \
	} \
}while(0)

/*ERROR CODE */
#define S_AUDIO_NODEV     0x80
#define S_AUDIO_OPENED    0x81
#define S_AUDIO_NO_MEM    0x82
#define S_AUDIO_FRAME_NOT_CONTINUE  0x83

STATUS audio_create(const char * devname, UINT32 arg);
int audio_open(const char *devname, UINT32 flags, UINT32 mode);
int audio_read(int devid, char * const pbuf, UINT32 nbytes);
int audio_write(int devid, char * const pbuf, UINT32 nbytes);
STATUS audio_ioctl(int devid, UINT32 cmd, UINT32 arg);
STATUS audio_close(int devid);
STATUS audio_delete(const char *devname);
int codec_open(const char *devname, int flags, int mode);
int codec_close(int devid);
int codec_ioctl(int devid, int cmd, int arg);
STATUS audio_nv_init(void);
void disp_audio_nv(void);
void audio_nv_enbale(unsigned int arg);
int audio_nv_read(void);
void audio_init(void);
#ifdef __cplusplus
} /* allow C++ to use these headers */
#endif /* __cplusplus */

#endif /* _AUDIOD_NV_H_ */

