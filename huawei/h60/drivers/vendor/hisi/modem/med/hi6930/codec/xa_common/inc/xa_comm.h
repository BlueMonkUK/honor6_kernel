/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : xa_com.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2012��7��9��
  ����޸�   :
  ��������   : xa_com.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��7��9��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __XA_COMM_H__
#define __XA_COMM_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "ucom_comm.h"
#include  "hifidrvinterface.h"
#include  "xa_apicmd_standards.h"
#include  "xa_error_handler.h"
#include  "xa_error_standards.h"
#include  "xa_memory_standards.h"
#include  "xa_type_def.h"

#include  "om_log.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
/* tensilica mp3/aac��������Ҫ��MEMORY��С */
/* ����һ���ṹ�壬ָ��MEMORY TABLE */
#define CODEC_API_MEM_SIZE          (4)
/* �����������Ҫ�ĸ����ڴ��ָ�룬20bytes��׼ȷ */
#define CODEC_TABLE_MEM_SIZE        (20)
/* ������������Ϣ */
#define CODEC_PERSIST_MEM_SIZE      (19100)
/* Ϊִ�в����ṩ��ʱ�ռ� */
#define CODEC_SCRATCH_MEM_SIZE      (47200)
/* ������������ÿ����Ҫ�������������ܻ᷵��nonfatal error */
#define CODEC_INPUT_MEM_SIZE        (6100)
/* �������ÿ�ε��ý���ǰ�ɸ��� */
#define CODEC_OUTPUT_MEM_SIZE       (32000)

#define CODEC_DECODE_FINISH_FLAG    (1)
#define CODEC_DECODE_UNFINISH_FLAG  (0)

/* ����ʼ������ */
#define XA_COMM_MAX_INIT_CNT        (20)

/* tensilica��������ڴ�������ֵ */
#define XA_COMM_MAX_ALIGN_NUM       (8)

#define XA_ERROR_HANDLE(err_code)   \
    if(err_code != XA_NO_ERROR) \
    { \
        OM_LogError(CODEC_TENSILICA_ERR); \
        if((VOS_UINT32)err_code & XA_FATAL_ERROR) \
        { \
            return VOS_ERR; \
        } \
    }

#define XA_FATAL_ERROR_HANDLE(err_code) \
    if((VOS_UINT32)err_code & XA_FATAL_ERROR) \
    {  \
        OM_LogError(CODEC_TENSILICA_ERR); \
        return;  \
    }

#define XA_ERROR_HANDLE_WITH_STATUS_CODE(err_code,pStatus)   \
    if(err_code != XA_NO_ERROR) \
    { \
        if((VOS_UINT32)err_code & XA_FATAL_ERROR) \
        { \
            *pStatus = CODEC_STATE_ERROR; \
        } \
        OM_LogError(CODEC_TENSILICA_ERR); \
        return VOS_ERR; \
    }

#define CODEC_XA_ERRCODE_CHECK(err_code) \
    if(err_code != XA_NO_ERROR) \
    { \
        OM_LogError1(CODEC_TENSILICA_ERR, err_code); \
        if((VOS_UINT32)err_code & XA_FATAL_ERROR)\
        {\
            return err_code; \
        }\
    }

#define CODEC_XA_GetStreamStatus()          (g_uwStreamStatus)
#define CODEC_XA_SetStreamStatus(uwStatus)  (g_uwStreamStatus = (uwStatus))


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
/*****************************************************************************
 ʵ������  : CODEC_STATE_ENUM
 ��������  : MP3��AAC������״̬
*****************************************************************************/
enum
{
    CODEC_STATE_UNINITIALIZED = 0,
    CODEC_STATE_INITIALIZED,
    CODEC_STATE_FINISH,                                                         /* AAC/MP3��������finish״̬ */
    CODEC_STATE_ERROR,
    CODEC_STATE_BUTT
};
typedef VOS_UINT16 CODEC_STATE_ENUM_UINT16;

/*****************************************************************************
 ʵ������  : CODEC_SEEK_DIRECT_ENUM
 ��������  : SEEK����
*****************************************************************************/
enum CODEC_SEEK_DERECT_ENUM
{
    CODEC_SEEK_FORWARDS = 0,
    CODEC_SEEK_BACKWARDS,
    CODEC_SEEK_BUT
};

typedef VOS_UINT16 CODEC_SEEK_DERECT_ENUM_UINT16;


/*****************************************************************************
 ʵ������  : XA_COM_CUSTOM_ERROR_ENUM
 ��������  : �Զ��������
*****************************************************************************/
enum XA_COM_CUSTOM_ERROR_ENUM
{
    XA_COM_CUSTOM_NULL_POINT_ERROR       = XA_ERROR_CODE(xa_severity_fatal, xa_class_api, XA_CODEC_GENERIC, 10),
    XA_COM_CUSTOM_MEM_BLOCK_SIZE_ERROR   = XA_ERROR_CODE(xa_severity_nonfatal, xa_class_api, XA_CODEC_GENERIC, 11),
    XA_COM_CUSTOM_READ_DATA_ERROR        = XA_ERROR_CODE(xa_severity_fatal, xa_class_api, XA_CODEC_GENERIC, 12)
};

/*****************************************************************************
 ʵ������  : CODEC_POS_STATE_ENUM_UINT16
 ��������  : ��ǰ֡λ��ö��
*****************************************************************************/
typedef enum
{
    AUDIO_PLAYER_POS_NORMAL = 0,                            /* ���ļ���β */
    AUDIO_PLAYER_POS_END_OF_STEAM,                          /* �ļ���β */
    AUDIO_PLAYER_POS_BUTT
} CODEC_POS_STATE_ENUM;
typedef VOS_UINT16  CODEC_POS_STATE_ENUM_UINT32;

/*****************************************************************************
 ʵ������  : AUDIO_EFFECT_SWITCH_ENUM_UINT16
 ��������  : ��Чʹ��ö��
*****************************************************************************/
/* ��Чʹ�ܿ��� */
enum AUDIO_EFFECT_SWITCH_ENUM
{
    AUDIO_EFFECT_SWITCH_OFF          = 0,
    AUDIO_EFFECT_SWITCH_ON
};
typedef VOS_UINT16 AUDIO_EFFECT_SWITCH_ENUM_UINT16;


/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
/*****************************************************************************
 ʵ������  : AUDIO_DEC_PARAMS_STRU
 ��������  : PCM������Ϣ
*****************************************************************************/
typedef struct
{
    VOS_INT32 swBitRate;            /* ������ */
    VOS_INT32 swPcmWidthSize;       /* ����λ�� */
    VOS_INT32 swChannel;            /* ������ */
    VOS_INT32 swSampleRate;         /* ������ */
    VOS_INT32 swAacBitStreamType;   /* aac���������뷽ʽ��mp3��ʹ�� */
}AUDIO_DEC_PARAMS_STRU;

/*****************************************************************************
 ʵ������  : XA_COMM_MEMORY_CFG_STRU
 ��������  : tensilica���ʼ��ʱ�ڴ����ýṹ�壬�ڴ��ͷ�ʱ����
*****************************************************************************/
typedef struct
{
    VOS_INT32  swApiObjSize;                                                    /* �ڴ��С����byteΪ��λ */
    VOS_INT32  swTableMemSize;
    VOS_INT32  swPersistMemSize;                                                /* �����ڴ��׵�ַ��Tensilica Lib�ڲ�ʹ�� */
    VOS_INT32  swScratchMemSize;                                                /* �����ڴ��׵�ַ��Tensilica Lib�ڲ�ʹ�� */
    VOS_INT32  swHeaderMemSize;                                                 /* size of header memory, aac/mp3 dec exclusive */
    VOS_INT32  swInputMemSize;                                                  /* �����С */
    VOS_INT32  swOutputMemSize;                                                 /* �����С */

    VOS_VOID*  pApiStructBuff;                                                  /* api struct */
    VOS_VOID*  pTableBuff;                                                      /* table memory */
    VOS_VOID*  pPersistBuff;                                                    /* persist memory */
    VOS_VOID*  pScratchBuff;                                                    /* scratch memory */
    VOS_VOID*  pHeaderBuff;                                                     /* file header memory, aac/mp3 dec exclusive */
    VOS_VOID*  pInputBuff;                                                      /* input memory */
    VOS_VOID*  pOutputBuff;                                                     /* output memory */
}XA_COMM_MEMORY_CFG_STRU;

/*****************************************************************************
 ʵ������  : XA_AUDIO_DEC_USER_CFG_STRU
 ��������  : ��Ƶ��ʼ���û����ڴ����ýṹ��
*****************************************************************************/
typedef struct
{
    VOS_VOID      *pvInBuff;
    VOS_VOID      *pvOutBuff;
    VOS_VOID      *pvHeadBuff;
    VOS_INT32      swInSize;
    VOS_INT32      swConsumed;
    VOS_INT32      swOutputSize;
    VOS_INT32      swHeaderMemSize;
    VOS_UINT32     enIsEndOfStream;
    VOS_UINT32     uwDecoderState;
    VOS_UINT32     uwPcmWidthSize;
}XA_AUDIO_DEC_USER_CFG_STRU;

/*****************************************************************************
 ʵ������  : XA_AUDIO_EFFECT_USER_CFG_STRU
 ��������  : ��Ч��ʼ���û����ڴ����ýṹ��
*****************************************************************************/
typedef struct
{
    /* �û������� */
    AUDIO_EFFECT_SWITCH_ENUM_UINT16        enDm3Enable;        /* DTS��Ч�Ƿ�ʹ�� */
    AUDIO_EFFECT_SWITCH_ENUM_UINT16        enDtsEnable;        /* �ű���Ч�Ƿ�ʹ�� */
    AUDIO_EFFECT_DEVICE_UINT16             uhwCurDevice;       /* ��ǰ�豸 */
    VOS_INT16                              shwReverse;
    VOS_UINT32                             uwSampleRate;       /* ��������� */
    VOS_UINT32                             uwChannel;          /* ���������� */

    /* �ڴ����ӿ� -- �ڴ�ָ�� */
    VOS_VOID                              *pvInBuff;           /* ����BUFFָ�� */
    VOS_VOID                              *pvOutBuff;          /* ���BUFFָ�� */

    /* �ڴ����ӿ� -- �ڴ��С */
    VOS_INT32                              swInputSize;        /* �������ݴ�С����λBytes */
}XA_AUDIO_EFFECT_USER_CFG_STRU;

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_INT32 XA_COMM_Decode(xa_codec_func_t*  pfuncProcess,
                                xa_codec_handle_t pstApiObj,
                                VOS_INT32         swInputSize,
                                VOS_UINT32        enIsEndOfStream,
                                VOS_INT32*        pswOutputSize,
                                VOS_INT32*        pswConsumeSize,
                                VOS_INT32*        pswDecodeDone);
extern VOS_INT32 XA_COMM_Init(xa_codec_func_t*   pfuncProcess,
                              xa_codec_handle_t  pstApiObj,
                              VOS_VOID*          pvInputBuff,
                              VOS_INT32          swInputSize,
                              VOS_VOID*          pvHeaderBuff,
                              VOS_INT32          swHeaderSize,
                              VOS_INT32         *pswLastConsumed);
extern VOS_INT32 XA_COMM_MemFree(XA_COMM_MEMORY_CFG_STRU* pstMemoryCfg);
extern VOS_INT32  XA_COMM_MemoryAlloc (xa_codec_func_t*           pfuncProcess,
                                                 xa_codec_handle_t           pstApiObj,
                                                 XA_COMM_MEMORY_CFG_STRU*  pstMemoryCfg);
extern VOS_INT32 XA_COMM_SetBlksMemory(xa_codec_func_t*           pfuncProcess,
                                       xa_codec_handle_t          pstApiObj,
                                       XA_COMM_MEMORY_CFG_STRU* pstMemoryCfg);
extern VOS_VOID XA_COMM_ShiftBuff(VOS_CHAR* pchBuff, VOS_INT32 swBuffSize, VOS_INT32 swConsumeBytes);
extern VOS_INT32 XA_COMM_Startup(xa_codec_func_t*           pfuncProcess,
                                 xa_codec_handle_t*       ppstApiObj,
                                 XA_COMM_MEMORY_CFG_STRU* pstMemCfg);
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of xa_comm.h */
