/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : audio_player.h
  �� �� ��   : ����
  ��    ��   : C00137131
  ��������   : 2012��7��10��
  ����޸�   :
  ��������   : audio_player.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��7��10��
    ��    ��   : C00137131
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "audio.h"
#include  "xa_comm.h"
#include  "ucom_low_power.h"
#include  "xa_dts_pp.h"

#ifndef __AUDIO_PLAYER_H__
#define __AUDIO_PLAYER_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define AUDIO_PLAYER_BUFF_POOL_SIZE                 (0x9c00)   /* ��������Ϊ������Ϊ192Kʱ��֡26ms��˫ͨ�����ݣ���λbyte */
#define AUDIO_PLAYER_SRC_DOLBY_BUFF_SIZE            (0x3C00)   /* SRC����֮�󻺳�ء�4֡48K��Ϊ20ms˫�������ݳ��ȣ���λ:byte
                                                                * (4 * AUDIO_PLAYER_PCM_20MS_BUFF_SIZE) */
#define AUDIO_PLAYER_PLAY_CHN_NUM                   (2)        /* Audio player������������ */
#define AUDIO_PLAYER_CHUNK_SIZE                     (128)      /* SRCÿ�δ����chunksize */
#define AUDIO_PLAYER_BUFF_SIZE_512K                 (0x80000)  /* ���������ݿ��С512k */
#define AUDIO_PLAYER_BUFF_SIZE_200K                 (0x32000)  /* ���������ݿ��С200k */
#define AUDIO_PLAYER_BUFF_SIZE_50K                  (0xC800)   /* ���������ݿ��С50k */
#define AUDIO_PLAYER_PCM_WIDTH_SIZE                 (16)       /* PCMλ�� */
#define AUDIO_PLAYER_PCM_20MS_BUFF_SIZE             (3840)     /* ÿ20ms PCM��Ҫ�����ߵ���Ƶ���ݣ���λbyte
                                                                *���㷽��:48000K * 20ms * ˫���� * sizeof(VOS_INT16) */
#define AUDIO_PLAYER_DTS_PROC_NUM                   (15)       /* DTSÿ20ms������Ҫ����Ĵ���, ÿ�δ���64�����㣬���㷽��:
                                                                * AUDIO_PLAYER_PCM_20MS_BUFF_SIZE/˫����/sizeof(short)/64 */
#define AUDIO_PLAYER_DTS_CHUNK_SIZE                 (64)       /* DTSÿ�δ����chunksize */
#define AUDIO_PLAYER_OUTPUT_SAMPLE_RATE             (48000)    /* ��Ƶ����Ĳ����ʣ�ģ��codec����Ϊ48K */
#define AUDIO_PLAYER_DTS_BUFF_SIZE                  (3840)     /* DTS buff��С */

/* morphing volue while enable DTS */
#define AUDIO_PLAYER_MORPHING_VOL_FRAME_0           (0)
#define AUDIO_PLAYER_MORPHING_VOL_FRAME_5           (5)
#define AUDIO_PLAYER_MORPHING_VOL_FRAME_6           (6)
#define AUDIO_PLAYER_MORPHING_VOL_FRAME_10          (10)
#define AUDIO_PLAYER_MORPHING_VOL_FRAME_11          (11)
#define AUDIO_PLAYER_MORPHING_VOL_FRAME_21          (21)

/* ����XA related mem buff value are all temporary, as not provided exactly yet */
#define AUDIO_PLAYER_XA_API_OBJ_BUFF_SIZE           (0x2C)      /* XA api struct buffer size */
#define AUDIO_PLAYER_XA_PERSIST_MEM_SIZE            (0x30CC)   /* XA persist memory buffer size */
#define AUDIO_PLAYER_XA_SCRATCH_MEM_SIZE            (0x5C30)   /* XA scratch memory buffer size */
#define AUDIO_PLAYER_XA_INPUT_MEM_SIZE              (0x2FA8)   /* XA input memory buffer size */
#define AUDIO_PLAYER_XA_OUTPUT_MEM_SIZE             (0x3E80)   /* XA output memory buffer size */
#define AUDIO_PLAYER_XA_TABLE_MEM_SIZE              (0xA0)     /* XA table memory buffer size */

/* �õ�������ȫ�ֿ��Ʊ��� */
#define AUDIO_PlayerGetPlayPtr()                    (&g_stAudioPlayerObj)

/* �õ�����������������ƽṹ�� */
#define AUDIO_PlayerGetPlayBuf()                    (&g_stAudioPlayerBuff)
/* �õ����������������bufferָ�� */
#define AUDIO_PlayerGetPlayBufPoolPtr()             (g_stAudioPlayerBuff.scPlayBufPool)
/* �õ����������������buffer��� */
#define AUDIO_PlayerGetPlayBufPoolDepth()           (g_stAudioPlayerBuff.uwPlayBufPoolDepth)
/* ���ò��������������buffer��� */
#define AUDIO_PlayerSetPlayBufPoolDepth(uwSize)\
                                                    (g_stAudioPlayerBuff.uwPlayBufPoolDepth = uwSize)
/* ��ȡ�������뻺��ṹ�� */
#define AUDIO_PlayerGetInputBuf()                   (&g_stAudioPlayerInBuf)
/* �õ�ԭʼ��������bufferָ�� */
#define AUDIO_PlayerGetInputBufPtr()                (g_stAudioPlayerInBuf.ascBuff)
/* �õ��������bufferָ�� */
#define AUDIO_PlayerGetOutputBufPtr()               (g_pvOutputBuff)

/* ���audio player����״̬ */
#define AUDIO_PlayerGetState()                      (g_stAudioPlayerObj.enState)
/* ����audio player����״̬ */
#define AUDIO_PlayerSetState(uhwVar)                (g_stAudioPlayerObj.enState = uhwVar)

/* �õ����ԭʼ��Ƶ���ݵ�DDR��ַָ�� */
#define AUDIO_PlayerGetBufAddr()                    (g_stAudioPlayerObj.uwDecBufAddr)
#define AUDIO_PlayerSetBufAddr(var)                 (g_stAudioPlayerObj.uwDecBufAddr = (var))

#define AUDIO_PlayerGetApBufAddr()                  (g_stAudioPlayerObj.uwApBufAddr)
#define AUDIO_PlayerSetApBufAddr(uwValue)           (g_stAudioPlayerObj.uwApBufAddr = (uwValue))

#define AUDIO_PlayerGetSwapBuf()                    (&g_stAudioPlayerSwapBuf)
#define AUDIO_PlayerGetSwapBufAddr()                (g_stAudioPlayerSwapBuf.ascBuff)

/* ���SRC����ṹ�� */
#define AUDIO_PlayerGetSrcBuff()                    (&g_stAudioPlayerSrcBuff)
/* SRC����֮�󻺳� */
#define AUDIO_PlayerGetSrcBuffPtr()                 (g_stAudioPlayerSrcBuff.scPlayBufPool)
/* �õ�SRC����֮�󻺳��buffer��� */
#define AUDIO_PlayerGetSrcBufPoolDepth()            (g_stAudioPlayerSrcBuff.uwPlayBufPoolDepth)
/* ����SRC����֮�󻺳��buffer��� */
#define AUDIO_PlayerSetSrcBufPoolDepth(uwSize)\
                                                    (g_stAudioPlayerSrcBuff.uwPlayBufPoolDepth = uwSize)
/* ��ȡDTS����ṹ */
#define AUDIO_PlayerGetDtsBuff()                    (&g_stAudioPlayerDtsBuff)
/* ��ȡDTS��Ч����֮��Ļ���buffָ�� */
#define AUDIO_PlayerGetDtsBuffPtr()                 (g_stAudioPlayerDtsBuff.ascBuff)


#define AUDIO_PLAYER_FIR_INIT(uhwSampInBuf, uwLen) fir_init(samp_in_buf, len)
#define AUDIO_PLAYER_FIR_FIX(uhwPcmBuf, uhwSampInBuf, uwCoef, uwFilterLen, uwLen) \
                     fir_fix(PCM_buf, samp_in_buf, coef, filter_len,len)

#define PLAYER_RET_HANDLE(uwRet)   \
    if(uwRet != UCOM_RET_SUCC) \
    { \
        return uwRet; \
    }

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
 ʵ������  : AUDIO_PLAY_PART_ENUM_UINT16
 ��������  : Audio Player��ȡ������������
*****************************************************************************/
enum
{
    AUDIO_PLAY_PART_MAIN = 0,                               /* ��buffer 4920k */
    AUDIO_PLAY_PART_SUB,                                    /* ����buffer 200k */
    AUDIO_PLAY_PART_BUTT
};
typedef VOS_INT16 AUDIO_PLAY_PART_ENUM_UINT16;

/*****************************************************************************
 ʵ������  : AUDIO_PLAYER_STATE_ENUM_UINT16
 ��������  : Audio Player����״̬ö��
*****************************************************************************/
typedef enum
{
    AUDIO_PLAYER_STATE_IDLE = 0,                            /* IDLE״̬ */
    AUDIO_PLAYER_STATE_RUNNING,                             /* RUNNING״̬ */
    AUDIO_PLAYER_STATE_PAUSE,                               /* ��ͣ״̬ */
    AUDIO_PLAYER_STATE_BUTT
} AUDIO_PLAYER_STATE_ENUM;
typedef VOS_UINT16  AUDIO_PLAYER_STATE_ENUM_UINT16;

/*****************************************************************************
 ʵ������  : AUDIO_PLAYER_BUFF_ENUM_UINT16
 ��������  : Audio Player�������漰buff����ö��
*****************************************************************************/
typedef enum
{
    AUDIO_PLAYER_DEC_BUFF = 0,                              /* ����֮�󻺳�� */
    AUDIO_PLAYER_SRC_BUFF,                                  /* �����֮�󻺳�� */
    AUDIO_PLAYER_BUFF_BUTT
} AUDIO_PLAYER_BUFF_ENUM;
typedef VOS_UINT16  AUDIO_PLAYER_BUFF_ENUM_UINT16;

/*****************************************************************************
 ʵ������  : AUDIO_PLAYER_VOL_ENUM_UINT32
 ��������  : Audio Player������������ö��
*****************************************************************************/
typedef enum
{
    AUDIO_PLAYER_NORMAL_VOL = 0,                              /* ��ͨ�������� */
    AUDIO_PLAYER_DTS_ENABLE_VOL,                              /* dtsʹ������ */
    AUDIO_PLAYER_VOL_BUTT
} AUDIO_PLAYER_VOL_ENUM;
typedef VOS_UINT32  AUDIO_PLAYER_VOL_ENUM_UINT32;
/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/

/*****************************************************************************
 �ṹ��    : AUDIO_PLAYER_DECODE_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : ��ʼ/ֹͣ���봦����Ϣͷ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT16                          uhwMsgId;
    VOS_UINT16                          uhwReserve[3];
}AUDIO_PLAYER_DECODE_IND_STRU;

/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/

/* AudioPlayer �ڲ���Ϣ */
enum AUDIO_PLAYER_MSG_ENUM
{
    ID_AUDIO_PLAYER_START_DECODE_IND    = 0xDD51,           /* �������� */
    ID_AUDIO_PLAYER_STOP_DECODE_IND     = 0xDD53,           /* ֹͣ���� */
    ID_AUDIO_PLAYER_SET_DTS_ENABLE_IND  = 0xDD37,           /* audioplayer��dtsģ��ת����ap��Ϣ */
    ID_AUDIO_PLAYER_MSG_BUTT,
};

/*****************************************************************************
  6 STRUCT����
*****************************************************************************/

/*****************************************************************************
 ʵ������  : AUDIO_PLAYER_VOL_STRU
 ��������  : Audio Player ��������
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          uwTarget;                               /* ����Ŀ������ Q15*/
    VOS_UINT32                          uwCurr;                                 /* ��ǰ���� Q15*/
    VOS_UINT32                          uwOriginal;                             /* ԭʼ���� Q15*/
    VOS_UINT32                          uwDtsEnableFrame;                       /* ΪDTSʹ���������仺���������֡�� */
    AUDIO_PLAYER_VOL_ENUM_UINT32        enSetVolEnum;                           /* ������������ */
} AUDIO_PLAYER_VOL_STRU;


/*****************************************************************************
 ʵ������  : AUDIO_PLAYER_OBJ_STRU
 ��������  : Audio Player MP3/AAC��Ƶ����ͨ·������ƽṹ��
*****************************************************************************/
 typedef struct
{
    VOS_UINT32                          uwProtectWord1;
    VOS_UINT32                          uwProtectWord2;

    AUDIO_PLAYER_STATE_ENUM_UINT16      enState;                                /* audio player����״̬ */
    AUDIO_FILE_FORMAT_ENUM_UINT16       enFormat;                               /* ��Ƶ��ʽ */
    VOS_UINT32                          uwStartPos;                             /* ��ʼλ�� */
    VOS_UINT32                          uwFileHeaderRdy;                        /* ���ļ�ͷ�Ƿ��Ѷ�ȡ */
    VOS_UINT32                          uwSampleRate;                           /* ��Ƶ���ݲ����� */
    VOS_UINT32                          uw20msDataSize;                         /* 20ms PCM�������� */

    AUDIO_PLAY_DONE_IND_ENUM_UINT16     enPlayStatus;                           /* ���Ž��ö�� */
    VOS_UINT16                          uhwIsDataBak;                           /* ��ǰ�Ƿ�ʹ�ñ���Buf��Ϊ��������Դ */
    VOS_UINT32                          uwPlayedBytes;                          /* ��ǰ�ѽ�������ݣ���λbyte�����ļ�ͷ */
    VOS_UINT32                          uwDecBufAddr;                           /* ��ǰ���������ݵ�ַ */
    VOS_UINT32                          uwDecDataLeft;                          /* ��ǰʣ����������ݴ�С����λbyte */
    VOS_UINT32                          uwApBufAddr;                            /* AP������������׵�ַ */
    VOS_UINT32                          uwApDataLeft;                           /* AP�������Buff��ʣ��Ĵ��������ݵĴ�С���ڽ�������ж�̬�仯 */
    VOS_UINT32                          uwApBufSize;                            /* AP�������Buff���ܴ�С */
    VOS_UINT32                          uwTotalFileSize;                        /* ����MP3/AAC���ֵĴ�С����λbyte�����ļ�ͷ */
    VOS_UINT32                          uwDecConsumed;
    VOS_UINT32                          uwDecInMemSize;                         /* XA decode input memory size */
    AUDIO_PLAYER_VOL_STRU               stVolume;
    XA_DTS_ENABLE_ENUM_UINT16           enDtsEnable;                            /* DTS�Ƿ�ʹ�ܱ�־λ */
    VOS_UINT16                          uhwReserve;

    VOS_UINT32                          uwProtectWord3;
    VOS_UINT32                          uwProtectWord4;
}AUDIO_PLAYER_OBJ_STRU;

/*****************************************************************************
 ʵ������  : AUDIO_PLAYER_BUF_STRU
 ��������  : audio player MP3/AAC�ļ���������BUFF�ṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          uwPlayBufPoolDepth;                     /* �������������ȣ���λbytes�� */
    VOS_UINT32                          uwProtectWord1;
    VOS_UINT32                          uwProtectWord2;
    VOS_CHAR                            scPlayBufPool[AUDIO_PLAYER_BUFF_POOL_SIZE];
    VOS_UINT32                          uwProtectWord3;
    VOS_UINT32                          uwProtectWord4;
}AUDIO_PLAYER_BUF_STRU;

/*****************************************************************************
 ʵ������  : AUDIO_PLAYER_SRC_BUF_STRU
 ��������  : src����֮��Ļ���ؽṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          uwPlayBufPoolDepth;                     /* �������������ȣ���λbytes�� */
    VOS_UINT32                          uwProtectWord1;
    VOS_UINT32                          uwProtectWord2;
    VOS_CHAR                            scPlayBufPool[AUDIO_PLAYER_SRC_DOLBY_BUFF_SIZE];
    VOS_UINT32                          uwProtectWord3;
    VOS_UINT32                          uwProtectWord4;
}AUDIO_PLAYER_SRC_BUF_STRU;

/*****************************************************************************
 ʵ������  : AUDIO_PLAYER_IN_BUF_STRU
 ��������  : audio player MP3/AAC�ļ���������
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          uwProtectWord1;
    VOS_UINT32                          uwProtectWord2;
    VOS_CHAR                            ascBuff[AUDIO_PLAYER_XA_INPUT_MEM_SIZE];
    VOS_UINT32                          uwProtectWord3;
    VOS_UINT32                          uwProtectWord4;
}AUDIO_PLAYER_IN_BUF_STRU;

/*****************************************************************************
 ʵ������  : AUDIO_PLAYER_SWAP_BUF_STRU
 ��������  : audio player ���ݽ���
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          uwProtectWord1;
    VOS_UINT32                          uwProtectWord2;
    VOS_CHAR                            ascBuff[AUDIO_PLAYER_BUFF_SIZE_200K];
    VOS_UINT32                          uwProtectWord3;
    VOS_UINT32                          uwProtectWord4;
}AUDIO_PLAYER_SWAP_BUF_STRU;

/*****************************************************************************
 ʵ������  : AUDIO_PLAYER_DTS_BUF_STRU
 ��������  : audio player dtsʹ�õĻ���
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          uwProtectWord1;
    VOS_UINT32                          uwProtectWord2;
    VOS_CHAR                            ascBuff[AUDIO_PLAYER_DTS_BUFF_SIZE];
    VOS_UINT32                          uwProtectWord3;
    VOS_UINT32                          uwProtectWord4;
}AUDIO_PLAYER_DTS_BUF_STRU;

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
/* ����MP3/AAC����Ƶͨ·��ؿ�����Ϣ */
extern AUDIO_PLAYER_OBJ_STRU                   g_stAudioPlayerObj;

/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_UINT32 AUDIO_PLAYER_ApiInit(VOS_VOID);
extern VOS_VOID AUDIO_PLAYER_AudioCallBack(
                       VOS_VOID  *pshwBufOut,
                       VOS_VOID  *pshwBufIn,
                       VOS_UINT32 uwSampleRate,
                       VOS_UINT32 uwChannelNum,
                       VOS_UINT32 uwBufSize);
extern VOS_UINT32 AUDIO_PLAYER_CheckBuff( VOS_VOID );
extern VOS_INT32  AUDIO_PLAYER_DoDts(
                       VOS_INT16 *pshwInput,
                       VOS_INT16 *pshwOutput);
extern VOS_UINT32 AUDIO_PLAYER_DoPlay(VOS_VOID);
extern VOS_UINT32 AUDIO_PLAYER_DoSeek(VOS_UINT32 uwSkipSize, CODEC_SEEK_DERECT_ENUM_UINT16 enSeekDirect);
extern VOS_UINT32 AUDIO_PLAYER_DoSrc(
                                       VOS_INT16  *pshwBufIn,
                                       VOS_INT16  *pshwBufOut,
                                       VOS_UINT32  uwProcTime);
extern VOS_UINT32 AUDIO_PLAYER_DoStop(VOS_VOID);
extern VOS_VOID AUDIO_PLAYER_FillBuff(AUDIO_PLAYER_OBJ_STRU *pstPlayer);
extern VOS_UINT32  AUDIO_PLAYER_GetCurrDecPos(AUDIO_FILE_FORMAT_ENUM_UINT16 enFormat);
extern VOS_UINT16 AUDIO_PLAYER_GetCurrVol( VOS_VOID );
extern VOS_UINT32 AUDIO_PLAYER_HandleFile(CODEC_STATE_ENUM_UINT16 *penDecoderState);
extern VOS_UINT32 AUDIO_PLAYER_Init(VOS_VOID);
extern VOS_VOID AUDIO_PLAYER_InitBuff( VOS_VOID );
extern VOS_UINT32  AUDIO_PLAYER_IsIdle(VOS_VOID);
extern VOS_UINT32 AUDIO_PLAYER_MsgPlayDoneSignalReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_PLAYER_MsgQueryStatusReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_PLAYER_MsgQueryTimeReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_PLAYER_MsgSeekReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_PLAYER_MsgSetVolReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32  AUDIO_PLAYER_MsgStartDecodeInd(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_PLAYER_MsgStartReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32  AUDIO_PLAYER_MsgStopDecodeInd(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_PLAYER_MsgStopReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_PLAYER_MsgUpdateApBuffCmd(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_PLAYER_MsgUpdatePcmBuffCmd(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_PLAYER_PcmStart(VOS_VOID);
extern VOS_VOID AUDIO_PLAYER_PcmStop(VOS_VOID);
extern VOS_VOID AUDIO_PLAYER_SendApAudioCnf(VOS_UINT16    uhwMsgId,
                                                         VOS_UINT32    uwRet,
                                                         VOS_VOID     *pvTail,
                                                         VOS_UINT32    uwTailLen);
extern VOS_UINT32 AUDIO_PLAYER_SendPcmSetBufCmd(VOS_UINT32 uwBufPoolAddr,
                                                               VOS_UINT32 uwBufSize);
extern VOS_UINT32 AUDIO_PLAYER_SendPlayDoneInd(AUDIO_PLAY_DONE_IND_ENUM_UINT16   enPlayDone);
extern VOS_VOID AUDIO_PLAYER_SendPlaySeekCnf(AUDIO_PLAY_RESULT_CNF_ENUM_UINT16 enSeekResult,
                                                         VOS_VOID               *pvTail,
                                                         VOS_UINT32              uwTailLen);
extern VOS_VOID AUDIO_PLAYER_SendPlayStopCnf(VOS_VOID *pvTail, VOS_UINT32 uwTailLen);
extern VOS_UINT32  AUDIO_PLAYER_ShiftBuff(
                       AUDIO_PLAYER_BUFF_ENUM_UINT16 enBuffType,
                       VOS_UINT32                    uwShiftSize);
extern VOS_UINT32  AUDIO_PLAYER_MsgSetDtsEnableCmd(VOS_VOID *pvOsaMsg);

extern VOS_UINT32  AUDIO_PLAYER_DtsMorphingVol(VOS_VOID);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of audio_player.h */
