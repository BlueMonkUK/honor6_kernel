
#ifndef __AUDIO_ENHANCE_H__
#define __AUDIO_ENHANCE_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "audio.h"
#include "audio_pcm.h"
#include "xa_src_pp.h"
#include "hifidrvinterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/

/* ENHANCE֡�����ֵ(������) */
#define AUDIO_ENHANCE_MAX_FRAME_LENGTH  (AUDIO_PCM_MAX_FRAME_LENGTH/2)

/* Ĭ�ϴ���Ĳ����� */
#define AUDIO_ENHANCE_DEFAULT_SAMPLE_RATE   (48000)

/* ��ȡ���ƽṹ��ָ�� */
#define AUDIO_ENHANCE_GetCtrl()             (&g_stAudioEnhanceCtrl)
/* ��ȡģ�����ָ�� */
#define AUDIO_ENHANCE_GetModuleCtrl(enMod)  (&g_stAudioEnhanceCtrl.astModule[enMod])
/* ��ȡģ��ע��ָ�� */
#define AUDIO_ENHANCE_GetModuleReg(enMod)   (&g_astAudioEnhanceModuleTab[enMod])

/* ��ȡ����ṹ��ָ�� */
#define AUDIO_ENHANCE_GetBuff()             (&g_stAudioEnhanceBuff)
/* ��ȡMicIn������ */
#define AUDIO_ENHANCE_GetMicInL(enBuff)     (g_stAudioEnhanceBuff.ashwMicInL[enBuff])
/* ��ȡMicIn������ */
#define AUDIO_ENHANCE_GetMicInR(enBuff)     (g_stAudioEnhanceBuff.ashwMicInR[enBuff])
/* ��ȡSpkOut������ */
#define AUDIO_ENHANCE_GetSpkOutL(enBuff)    (g_stAudioEnhanceBuff.ashwSpkOutL[enBuff])
/* ��ȡSpkOut������ */
#define AUDIO_ENHANCE_GetSpkOutR(enBuff)    (g_stAudioEnhanceBuff.ashwSpkOutR[enBuff])

/* ���㵥����֡��(Byte) */
#define AUDIO_ENHANCE_GetMonoFrmSize(Rate)  (((Rate) / 8000) * 320)

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/* ģ������״̬ */
enum AUDIO_ENHANCE_STATUS_ENUM
{
    AUDIO_ENHANCE_STATUS_IDLE           = 0,
    AUDIO_ENHANCE_STATUS_RUNNING,
    AUDIO_ENHANCE_STATUS_BUTT,
};
typedef VOS_UINT32 AUDIO_ENHANCE_STATUS_ENUM_UINT32;

/* ����ö�� */
enum AUDIO_ENHANCE_BUFF_ENUM
{
    AUDIO_ENHANCE_BUFF_IN               = 0,                /* ���뻺�� */
    AUDIO_ENHANCE_BUFF_OUT,                                 /* ������� */
    AUDIO_ENHANCE_BUFF_SWAP,                                /* ��������(��Ϊ��ʱ����ʹ��) */
    AUDIO_ENHANCE_BUFF_BUTT,
};
typedef VOS_UINT32 AUDIO_ENHANCE_BUFF_ENUM_UINT32;

/* �ⲿ����״̬(SAMRT_PA) */
enum AUDIO_ENHANCE_EX_REF_ENUM
{
    AUDIO_ENHANCE_EX_REF_OFF            = 0,                /* ��ʹ���ⲿ�źŷ��� */
    AUDIO_ENHANCE_EX_REF_ON,                                /* ʹ���ⲿ�źŷ��� */
    AUDIO_ENHANCE_EX_REF_BUTT
};
typedef VOS_UINT32  AUDIO_ENHANCE_EX_REF_ENUM_UINT32;

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/

/* ENHANCE ģ����ƽṹ�� */
typedef struct
{
    VOS_UINT32                          uwRefCnt;                               /* ���ü��� */
    AUDIO_ENHANCE_STATUS_ENUM_UINT32    enStatus;                               /* ��ѧ������״̬ */
    VOS_UINT32                          uwSampleRate;                           /* ������ */

} AUDIO_ENHANCE_MODULE_CTRL_STRU;

/* ENHANCE ���ƽṹ�� */
typedef struct
{
    VOS_UINT32                          uwProtectWord1;
    VOS_UINT32                          uwProtectWord2;

    AUDIO_ENHANCE_DEVICE_ENUM_UINT32    enDevice;                               /* �豸ģʽ */
    AUDIO_ENHANCE_EX_REF_ENUM_UINT32    enExRef;                                /* �ⲿ�ο����� */
    VOS_UINT32                          uwSrcRefFlag;                           /* ��������ñ�� */

    AUDIO_ENHANCE_MODULE_ENUM_UINT32    enCurrModule;                           /* ��ǰ����ģ�� */
    AUDIO_ENHANCE_MODULE_CTRL_STRU      astModule[AUDIO_ENHANCE_MODULE_BUTT];   /* ģ�鹤����Ϣ */

    VOS_UINT32                          uwProtectWord3;
    VOS_UINT32                          uwProtectWord4;
} AUDIO_ENHANCE_CTRL_STRU;

/* ENHANCE ����ṹ�� */
typedef struct
{
    VOS_UINT32                          uwProtectWord1;
    VOS_UINT32                          uwProtectWord2;

    VOS_INT16                           ashwMicInL[AUDIO_ENHANCE_BUFF_BUTT][AUDIO_ENHANCE_MAX_FRAME_LENGTH];
    VOS_INT16                           ashwMicInR[AUDIO_ENHANCE_BUFF_BUTT][AUDIO_ENHANCE_MAX_FRAME_LENGTH];

    VOS_INT16                           ashwSpkOutL[AUDIO_ENHANCE_BUFF_BUTT][AUDIO_ENHANCE_MAX_FRAME_LENGTH];
    VOS_INT16                           ashwSpkOutR[AUDIO_ENHANCE_BUFF_BUTT][AUDIO_ENHANCE_MAX_FRAME_LENGTH];

    VOS_UINT32                          uwProtectWord3;
    VOS_UINT32                          uwProtectWord4;
} AUDIO_ENHANCE_BUFF_STRU;

/* ENHANCE ģ��ע��ṹ */
typedef struct
{
    /* MIC���� */
    VOS_UINT32                          uwMicInPathID;
    /* SPK���� */
    VOS_UINT32                          uwSpkOutPathID;

} AUDIO_ENHANCE_MODULE_REG_STRU;

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

extern VOS_UINT32  AUDIO_ENHANCE_Init( VOS_VOID );
extern VOS_UINT32  AUDIO_ENHANCE_InitSrc(
                        AUDIO_ENHANCE_MODULE_ENUM_UINT32    enModule,
                        VOS_UINT32                          uwSampleRate );
extern VOS_UINT32 AUDIO_ENHANCE_MsgOmGetNvReq(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 AUDIO_ENHANCE_MsgOmSetNvReq(VOS_VOID *pstOsaMsg);
extern VOS_UINT32  AUDIO_ENHANCE_MsgSetDeviceInd( VOS_VOID *pvOsaMsg );
extern VOS_UINT32  AUDIO_ENHANCE_MsgSetDeviceReqRt( VOS_VOID *pvOsaMsg );
extern VOS_UINT32 AUDIO_ENHANCE_MsgSetParaInd(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 AUDIO_ENHANCE_MsgSetParaIndRt(VOS_VOID *pstOsaMsg);
extern VOS_UINT32  AUDIO_ENHANCE_MsgStartInd( VOS_VOID *pvOsaMsg );
extern VOS_UINT32  AUDIO_ENHANCE_MsgStartReqRt( VOS_VOID *pvOsaMsg );
extern VOS_UINT32  AUDIO_ENHANCE_MsgStopInd( VOS_VOID *pvOsaMsg );
extern VOS_UINT32  AUDIO_ENHANCE_MsgStopReqRt( VOS_VOID *pvOsaMsg );
extern VOS_VOID AUDIO_ENHANCE_MsgSyncConfirm(VOS_UINT16 uhwMsgId,
                                                           VOS_UINT32 uwRet,
                                                           VOS_VOID   *pvTail,
                                                           VOS_UINT32  uwTailLen);
extern VOS_UINT32 AUDIO_ENHANCE_ProcClear( AUDIO_ENHANCE_MODULE_ENUM_UINT32 enModule );
extern VOS_UINT32 AUDIO_ENHANCE_ProcInit( AUDIO_ENHANCE_MODULE_ENUM_UINT32 enModule );
extern VOS_UINT32 AUDIO_ENHANCE_ProcMicIn(
                       VOS_VOID               *pshwBufOut,
                       VOS_VOID               *pshwBufIn,
                       VOS_UINT32              uwSampleRate,
                       VOS_UINT32              uwChannelNum,
                       VOS_UINT32              uwBufSize);
extern VOS_UINT32 AUDIO_ENHANCE_ProcSetPara(
                       AUDIO_ENHANCE_MODULE_ENUM_UINT32            enModule,
                       AUDIO_ENHANCE_DEVICE_ENUM_UINT32            enDevice );
extern VOS_UINT32 AUDIO_ENHANCE_ProcSpkOut(
                       VOS_VOID               *pshwBufOut,
                       VOS_VOID               *pshwBufIn,
                       VOS_UINT32              uwSampleRate,
                       VOS_UINT32              uwChannelNum,
                       VOS_UINT32              uwBufSize);
extern VOS_VOID AUDIO_ENHANCE_RegisterPcmCallback( VOS_VOID );
extern VOS_UINT32 AUDIO_ENHANCE_SetDevice( AUDIO_ENHANCE_DEVICE_ENUM_UINT32 enDevice );
extern VOS_UINT32 AUDIO_ENHANCE_Start(
                       AUDIO_ENHANCE_MODULE_ENUM_UINT32    enModule,
                       VOS_UINT32                          uwSampleRate );
extern VOS_UINT32 AUDIO_ENHANCE_Stop( AUDIO_ENHANCE_MODULE_ENUM_UINT32 enModule );
extern VOS_UINT32  AUDIO_ENHANCE_DoSrc(
                XA_SRC_PROC_ID_ENUM_UINT32 enProcId,
                VOS_VOID                  *pInputBuff,
                VOS_VOID                  *pOutputBuff,
                VOS_UINT32                *puwOutputSize);
extern VOS_VOID  AUDIO_ENHANCE_CloseSrc( AUDIO_ENHANCE_MODULE_ENUM_UINT32 enModule );
extern VOS_VOID  AUDIO_ENHANCE_SetExRefPort( AUDIO_ENHANCE_DEVICE_ENUM_UINT32 enDevice);
extern VOS_VOID AUDIO_ENHANCE_ProcEcRef( VOS_UINT32 uwSampleRate );
extern VOS_UINT32  AUDIO_ENHANCE_GetCurrentModule( VOS_VOID );


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of audio_voip.h */
