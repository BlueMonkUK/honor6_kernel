/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : VOICE_mc.h
  �� �� ��   : ����
  ��    ��   : л���� 58441
  ��������   : 2011��5��6��
  ����޸�   :
  ��������   : VOICE_mc.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��5��6��
    ��    ��   : л���� 58441
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __VOICE_MC_H__
#define __VOICE_MC_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "codec_typedefine.h"
#include "HifiOmInterface.h"
#include "OmCodecInterface.h"
#include "VcCodecInterface.h"
#include "med_drv_timer_hifi.h"
#include "voice_proc.h"
#include "med_drv_dma.h"

#include <stdlib.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/
typedef VOS_UINT32 (*VOICE_MC_CALLBACK)(
                VOS_INT16 *pshwTx,
                VOS_INT16 *pshwRx,
                VOS_UINT32 uwSmpRate);                      /* ֪ͨhifi¼��ͨ������buff�Ļص����� */

#define VOICE_TX_TO_RX_FRAME_THD        (3)                 /*����������ж�֡���֡�������ֵ*/

#define VOICE_MC_DMAC_CHN_FR_RING_BUF   (0)
#define VOICE_MC_DMAC_CHN_TO_RING_BUF   (1)

#ifdef _MED_ERRORLOG
#define VOICE_ERRLOG_FQI_QUALITY_BAD    (0)
#endif

#define VOICE_UMTS_TX_BEGIN_TIMESTAMP   (130)                /*UMTS��ż��֡12ms�жϾ�������֡ͷ�ļ������λ0.1ms*/
#define VOICE_UMTS_RX_BEGIN_TIMESTAMP   (100)                /*UMTS��ż��֡11ms�жϾ�������֡ͷ�ļ������λ0.1ms*/
#define VOICE_TDS_TX_BEGIN_TIMESTAMP    (80)                 /*TDS-CDMA��ż��֡�����жϾ���ż��֡ͷ�ļ������λ0.1ms*/
#define VOICE_TDS_RX_BEGIN_TIMESTAMP    (20)                 /*TDS-CDMA��ż��֡�����жϾ���ż��֡ͷ�ļ������λ0.1ms*/
#define VOICE_IMS_TX_BEGIN_TIMESTAMP    (40)                 /*IMS��ż��֡5ms�жϾ�������֡ͷ�ļ������λ0.1ms*/
#define VOICE_IMS_RX_BEGIN_TIMESTAMP    (50)                 /*IMS��ż��֡4ms�жϾ�������֡ͷ�ļ������λ0.1ms*/

#define VOICE_GSM_TX_DMA_TIMESTAMP      (120)                                   /*GSM������DMA��ʱ�䣬��λ0.1ms */
#define VOICE_GSM_RX_DMA_TIMESTAMP      (180)                                   /*GSM������DMA��ʱ�䣬��λ0.1ms */

#define VOICE_GSM_QB_PARA               (4615)                                  /*GSM��ͨ��QB����ʱ��Ĳ�������λ0.1ms(ԭ��λΪ��s������Ҫ�������100)*/
#define VOICE_GSM_MIN_OFFSET            (60)                                    /*GSM��ʱ϶�����ƫ�ƣ���λ0.1ms*/
#define VOICE_GSM_MAX_OFFSET            (170)                                   /*GSM���ϱ����ݾ�GPHYҪ���ϱ�ʱ����������ƫ�ƣ���λ0.1ms 4*46*/
#define VOICE_GSM_ONE_FRAME_OFFSET      (200)                                   /*GSM��֡��(��λ0.1ms)*/

#define VOICE_SLOT_NUMBER_BY_FRAME      (15)                 /*UMTS��TD��ÿ֡��slot��*/
#define VOICE_CHIP_NUMBER_BY_FRAME      (38400)              /*UMTS��TD��ÿ֡��chip��*/
#define VOICE_CHIP_NUMBER_BY_SLOT       (2560)               /*UMTS��TD��ÿʱ϶��chip��*/
#define VOICE_UMTS_MAX_ENC_OFFSET       (195)                /*UMTS��ż��֡�ϱ�֡��������ʱ�̾����ϸ�����֡֡ͷ��ʱ��,��λ0.1ms */
#define VOICE_TDS_MAX_ENC_OFFSET        (155)                /*TDS-CDMA������֡�ϱ�֡��������ʱ�̾����ϸ�ż��֡֡ͷ��ʱ��,��λ0.1ms */
#define VOICE_TIMESTAMP_MS_RATIO        (10000)              /*1s��0.1ms��ת��*/
#define VOICE_LAST_FRAME_OFFSET         (300)                /*�뻺���ǰһ֡ʱ����λ0.1ms*/

#define VOICE_MC_MAX_PRIM               (26)                 /*���ԭ�ﴦ������*/
#define VOICE_MC_MSG_BUF_MAX_LEN        (10)                 /*������Ϣ��󳤶�  */

/*������SIO��DTCM��������ݵ�DMAͨ����*/
#define VOICE_MC_DMAC_CHN_MIC           (DRV_DMA_GetSioTxChn())
#define VOICE_MC_DMAC_CHN_SPK           (DRV_DMA_GetSioRxChn())

#define VOICE_SMART_PA_L_CHAN_VALID     (0)                  /* SMART PA�����I2S�����У�������Ϊ��Ч���� */
#define VOICE_SMART_PA_R_CHAN_VALID     (1)                  /* SMART PA�����I2S�����У�������Ϊ��Ч���� */

#define VOICE_McGetMcObjsPtr()          (&g_stVoiceMcObjs)
#define VOICE_McGetInoutPtr()           (&g_stVoiceMcObjs.stInout)                     /* ��ȡ��ǰ������������������ֳ֡����ᡢPC VOICE�� */
#define VOICE_McGetDmaFlagPtr()         (&g_stVoiceMcObjs.stDMAFlag)
#define VOICE_McGetTimerPtr()           (&g_stVoiceMcObjs.stTimer)
#define VOICE_McGetDeviceMode()         (g_stVoiceMcObjs.enDevMode)                    /* ��ȡ�����豸���� */
#define VOICE_McSetDeviveMode(uhwDevMode)  (g_stVoiceMcObjs.enDevMode = uhwDevMode)    /* ���������豸���� */
#define VOICE_McGetCaptureCallBackfunc()   (g_stVoiceMcObjs.pfunCallBack)                 /* ��ȡ֪ͨhifi¼��ͨ������buff�Ļص����� */
#define VOICE_McSetCaptureCallBack(pVar) (g_stVoiceMcObjs.pfunCallBack = pVar)         /* ����֪ͨhifi¼��ͨ������buff�Ļص����� */
#define VOICE_McGetModemObjPtr(uhwModemNo) &(g_stVoiceMcObjs.stModemMcObj[uhwModemNo]) /* ��ȡͨ·���� */
#define VOICE_McGetForeGroundNum()      (g_stVoiceMcObjs.enActiveModemNo)           /* ��ȡǰ̨ͨ·�� */
#define VOICE_McSetForeGroundNum(uhwVar) \
                (g_stVoiceMcObjs.enActiveModemNo = uhwVar)                             /* ��ȡǰ̨ͨ·�� */
#define VOICE_McGetModemVoiceState(uhwModemNo) \
                (g_stVoiceMcObjs.stModemMcObj[uhwModemNo].enState)                     /* ��ȡ����״̬ */
#define VOICE_McSetVoiceState(uhwModemNo, uhwVar) \
                (g_stVoiceMcObjs.stModemMcObj[uhwModemNo].enState = uhwVar)            /* ��������״̬ */
#define VOICE_McGetModemNetMode(uhwModemNo) \
                (g_stVoiceMcObjs.stModemMcObj[uhwModemNo].enNetMode)                   /* ��ȡ����ģʽ */
#define VOICE_McGetModemCodecPtr(uhwModemNo) \
                (&g_stVoiceMcObjs.stModemMcObj[uhwModemNo].stCodec)                    /* ��ȡ��������ָ�� */
#define VOICE_McSetModemNetMode(uhwModemNo, uhwVar) \
                (g_stVoiceMcObjs.stModemMcObj[uhwModemNo].enNetMode = uhwVar)          /* ��������ģʽ */
#define VOICE_McSetGsmCodecTypeMismatch(uhwModemNo, uhwVar) \
                (g_stVoiceMcObjs.stModemMcObj[uhwModemNo].uhwGsmCodecTypeMismatch = uhwVar) /* GSM�����������Ƿ���Ҫ���¶�û�и��� */
#define VOICE_McGetGsmCodecTypeMismatch(uhwModemNo) \
                (g_stVoiceMcObjs.stModemMcObj[uhwModemNo].uhwGsmCodecTypeMismatch)     /* ��ȡGSM�����������Ƿ���Ҫ���¶�û�и��� */
#define VOICE_McGetModemState(uhwModemNo) \
                (g_stVoiceMcObjs.stModemMcObj[uhwModemNo].enModemState)                 /* ��ȡͨ·״̬ */
#define VOICE_McSetModemState(uhwModemNo, uhwVar) \
                (g_stVoiceMcObjs.stModemMcObj[uhwModemNo].enModemState = uhwVar)        /* ����ͨ·״̬ */

#define VOICE_McGetSmartPaEn()          (g_stVoiceMcObjs.stSmartPaCfg.enEnable)         /* Smart PA�Ƿ�ʹ�� */
#define VOICE_McGetSmartPaValidChan()   (g_stVoiceMcObjs.stSmartPaCfg.uhwValidChannel)  /* Smart PA Valid Chn */
#define VOICE_McGetSmartPaMaster()      (g_stVoiceMcObjs.stSmartPaCfg.uhwIsMaster)      /* Smart PA Is Master Or Slave */

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
/* ����״̬ö�� */
enum VOICE_MC_STATE_ENUM
{
    VOICE_MC_STATE_IDLE              = 0,          /* IDLE״̬ */
    VOICE_MC_STATE_RUNNING,                        /* RUNNING״̬ */
    VOICE_MC_STATE_PAUSE,                          /* ˫ģ�л��й���̬ */
    VOICE_MC_STATE_BUTT
};
typedef VOS_UINT16  VOICE_MC_STATE_ENUM_UINT16;

/* ͨ·����ö�� */
enum VOICE_MC_MODEM_OPT_ENUM
{
    VOICE_MC_FOREGROUND_OPT   = 0,                  /* �˳���Ĭ */
    VOICE_MC_BACKGROUND_OPT,                        /* ���뾲Ĭ */
    VOICE_MC_MODEM_OPT_BUTT
};
typedef VOS_UINT16  VOICE_MC_MODEM_OPT_ENUM_UINT16;

/* ͨ·״̬ö�� */
enum VOICE_MC_MODEM_STATE_ENUM
{
    VOICE_MC_MODEM_STATE_FOREGROUND   = 0,            /* ǰ̨״̬,��VOICE_MC_FOREGROUND_OPT��Ӧ */
    VOICE_MC_MODEM_STATE_BACKGROUND,                  /* ��̨״̬,��VOICE_MC_BACKGROUND_OPT��Ӧ */
    VOICE_MC_MODEM_STATE_UNSET,                       /* δ����״̬ */
    VOICE_MC_MODEM_STATE_BUTT
};
typedef VOS_UINT16  VOICE_MC_MODEM_STATE_ENUM_UINT16;

/* ͨ·��ö�� */
enum VOICE_MC_MODEM_NUM_ENUM
{
    VOICE_MC_MODEM0              = 0,                 /* MODEM-0 */
    VOICE_MC_MODEM1,                                  /* MODEM-1 */
    VOICE_MC_MODEM_NUM_BUTT
};
typedef VOS_UINT16  VOICE_MC_MODEM_NUM_ENUM_UINT16;

/* PID��Χö�� */
enum VOICE_MC_PID_RANGE_ENUM
{
    VOICE_MC_MODEM0_RANGE              = 0,            /* ��MODEM-0��Ӧ */
    VOICE_MC_MODEM1_RANGE,                             /* ��MODEM-1��Ӧ */
    VOICE_MC_CODEC_RANGE,
    VOICE_MC_OM_RANGE,
    VOICE_MC_PID_RANGE_NUM_BUTT
};
typedef VOS_UINT16  VOICE_MC_PID_RANGE_ENUM_UINT16;

/* PIDת�����±�ö�� */
enum VOICE_MC_PIDTBL_INDEX_ENUM
{
    VOICE_MC_NAS_INDEX                = 0,              /* NAS��PID */
    VOICE_MC_GPHY_INDEX,                                /* G������PID */
    VOICE_MC_WPHY_INDEX,                                /* W������PID */
    VOICE_MC_TDPHY_INDEX,                               /* TD������PID */
    VOICE_MC_WTTF_INDEX,                                /* WTTF��PID */
    VOICE_MC_TDMAC_INDEX,                               /* TdMac��PID */
    VOICE_MC_IMSA_INDEX,                                /* IMSA��PID */
    VOICE_MC_INDEX_BUTT
};
typedef VOS_UINT16  VOICE_MC_PIDTBL_INDEX_ENUM_UINT16;

/* EC Reference������Դ */
enum VOICE_SMART_PA_EN_ENUM
{
    VOICE_SMART_PA_EN_DISABLE         = 0,                /* Smart PA�����ã�HIFI���д������� */
    VOICE_SMART_PA_EN_ENABLE,                             /* Smart PA���ã�����ѡ��ʹ��Smart PA����������� */
    VOICE_SMART_PA_EC_BUTT
};
typedef VOS_UINT16 VOICE_SMART_PA_EN_UINT16;

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
 ʵ������  : VOICE_VOICE_TX_PROC_REQ_STRU
 ��������  : G/U���д���ָʾ��ΪDMA�жϴ������з�����PID_VOICE�ı�����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT16                          uhwMsgId;                               /*0xDD03*/ /*_H2ASN_Skip*/
    VOS_UINT16                          uhwReserved;
}VOICE_VOICE_TX_PROC_REQ_STRU;

/*****************************************************************************
 ʵ������  : VOICE_VOICE_RX_PROC_REQ_STRU
 ��������  : G/U���д���ָʾ��ΪDMA�жϴ������з�����PID_VOICE�Ľ�����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT16                          uhwMsgId;                               /*0xDD03*/ /*_H2ASN_Skip*/
    VOS_UINT16                          uhwReserved;
}VOICE_VOICE_RX_PROC_REQ_STRU;

/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
/*��ʱ������ṹ��*/
typedef struct
{
    VOS_UINT32                          uwTimer3ms;                             /*Wż��֡3ms��ʱ�� */
    VOS_UINT32                          uwTimer6ms;                             /*Wż��֡6ms��ʱ�� */
    VOS_UINT32                          uwTimer20ms;                            /*20ms��ʱ��*/
    VOS_UINT32                          uwTimer20msRx;                          /*20ms��ʱ��*/
    VOS_UINT32                          uwTimer01msTx;                          /*0.1ms��ʱ��*/
    VOS_UINT32                          uwTimer01msRx;                          /*0.1ms��ʱ��*/
    VOS_UINT32                          uwTdTxTimer;                            /*TD�������ж�ʱ��*/
    VOS_UINT32                          uwTdRxTimer;                            /*TD�������ж�ʱ��*/
    VOS_UINT32                          uwTimer30msDiag;                        /*30ms��ʱ��,���������DMA�ж��Ƿ�����*/
}VOICE_MC_TIMER_STRU;

/*DMA����flag�ṹ��*/
typedef struct
{
    VOS_UINT16                          usMicInRun;                             /* SIO->dTCM MIC_DMA����ָʾ  */
    VOS_UINT16                          usSpkOutRun;                            /* dTCM->SIO SPK_DMA����ָʾ  */
    VOS_UINT16                          usMicInSwEnable;                        /* SIO->dTCM MIC_DMA�л����������־ */
    VOS_UINT16                          usSpkOutSwEnable;                       /* dTCM->SIO SPK_DMA�л����������־ */
    VOS_UINT32                          uwSyncOffset;                           /* UMTS��ʱ��ͬ��ʱ��ƫ��,��λ0.1ms */
    VOS_UINT32                          uwSyncTimestamp;                        /* UMTS��ʱ��ͬ��ʱ��ϵͳʱ��,��λ1s/32767 */
    VOS_UINT32                          uwChanCodeTime;                         /* GPHYҪ���ϱ��������ݵ�ʱ��ƫ���λ0.1ms */
}VOICE_MC_DMA_FLAG_STRU;

/*****************************************************************************
 ʵ������  : VOICE_MC_MODEM_STRU
 ��������  : ���������������ģ����Ҫ״̬������ modem(����modem���Բ�ͬ)
*****************************************************************************/
typedef struct
{
    VOICE_MC_MODEM_STATE_ENUM_UINT16    enModemState;              /* ͨ·״̬ */
    VOS_UINT16                          uhwGsmCodecTypeMismatch;  /* GSM�����������Ƿ���Ҫ���¶�û�и��� */
    VOICE_MC_STATE_ENUM_UINT16          enState;                  /* �����������״̬ */
    /* ��PROC���� */
    CODEC_NET_MODE_ENUM_UINT16          enNetMode;                /* ������ʽ */
    VOS_INT16                           shwVolumeTx;              /* �������� */
    VOS_INT16                           shwVolumeRx;              /* �������� */
    VOICE_PROC_CODEC_STRU               stCodec;                  /* �������� */
} VOICE_MC_MODEM_STRU;

/*****************************************************************************
 ʵ������  : VOICE_MC_OBJS_STRU
 ��������  : ���������������ģ����Ҫ״̬�������� modem(����modem���е�һЩ����)
*****************************************************************************/
typedef struct
{
    VCVOICE_DEV_MODE_ENUM_UINT16        enDevMode;                /* �����豸���� */
    VOICE_MC_MODEM_NUM_ENUM_UINT16      enActiveModemNo;          /* ����״̬ͨ���� */

    CODEC_INOUT_STRU                    stInout;                  /* ������������� */
    VOICE_MC_DMA_FLAG_STRU              stDMAFlag;                /* DMA���˲���/¼�����ݱ�־*/
    VOICE_MC_TIMER_STRU                 stTimer;                  /* ��ʱ����ؽṹ */
    /* Modem ����״̬ȫ�ֱ��� */
    VOICE_MC_MODEM_STRU                 stModemMcObj[VOICE_MC_MODEM_NUM_BUTT];
    /* �ص�������֪ͨHifi¼��ͨ����Buff���ݸ��� */
    VOICE_MC_CALLBACK                   pfunCallBack;
    VOICE_SMART_PA_CFG_NV_STRU          stSmartPaCfg;
} VOICE_MC_OBJS_STRU;

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern VOICE_MC_OBJS_STRU            g_stVoiceMcObjs;

/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_UINT32 VOICE_McApiIsIdle(VOS_VOID);
extern VOS_VOID VOICE_McChkMicConflick(VOS_VOID);
extern VOS_VOID VOICE_McChkSpkConflick(VOS_VOID);
extern VOS_VOID VOICE_McDestory( );
extern VOS_UINT16 VOICE_McGetForeGroundNetMode( VOS_UINT16 uhwActiveModemNo );
extern VOICE_MC_MODEM_STRU* VOICE_McGetForeGroundObj(VOS_VOID);
extern VOICE_MC_STATE_ENUM_UINT16 VOICE_McGetForeGroundVoiceState(VOS_VOID);
extern VOS_UINT16 VOICE_McGetModemNum( VOS_UINT32 uwSenderPid );
extern CODEC_NET_MODE_ENUM_UINT16 VOICE_McGetNetMode(VOS_UINT32 uwSenderPid);
extern VOICE_MC_STATE_ENUM_UINT16 VOICE_McGetVoiceState(VOS_UINT32 uwSenderPid);
extern VOS_UINT32 VOICE_McHandleFsmErr(VOS_UINT16 uhwSenderPid, VOS_UINT16 uhwMsgId);
extern VOS_UINT32 VOICE_McHandleRtFsmErr(VOS_UINT16 uhwSenderPid, VOS_UINT16 uhwMsgId);
extern VOS_VOID VOICE_McInformGphySetCodec(VOS_UINT32 uwReceiverPid);
extern VOS_VOID VOICE_McInit(VOS_VOID);
extern VOS_VOID VOICE_McModemInformToPhy(
                       CODEC_NET_MODE_ENUM_UINT16 enNetMode,
                       VOICE_MC_MODEM_NUM_ENUM_UINT16 enModemNo,
                       VOICE_MC_MODEM_OPT_ENUM_UINT16 enModemOpt
                       );
extern VOS_VOID VOICE_McModemObjInit( VOICE_MC_MODEM_STRU *pstMcModemObj );
extern VOS_UINT32 VOICE_McPlay(VOS_VOID);
extern VOS_VOID VOICE_McPostCfgCodecType(VOS_UINT16 uhwCodecType,VOS_UINT16 uhwModemNo);
extern VOS_UINT32 VOICE_McPostTxProc( VOS_VOID );
extern VOS_UINT32 VOICE_McPreCfgCodecType(VOS_UINT16 uhwNetMode, VOS_UINT16 *puhwCodecType);
extern VOS_UINT32 VOICE_McPreTxProc( VOS_VOID );
extern VOS_UINT32 VOICE_McRecord(VOS_VOID);
extern VOS_UINT32 VOICE_McRegisterCaptureCallBack(VOICE_MC_CALLBACK pfunCallBack);
extern VOS_VOID VOICE_McResume(VCVOICE_TYPE_ENUM_UINT16 enCodecType, VOS_UINT16 uhwModemNo);
extern VOS_UINT32 VOICE_McRxPreProc(VOS_UINT16 *puhwRxFrmLost, VOS_INT16 *pshwVolumeRx, VOS_INT16 **ppshwDecData);
extern VOS_UINT32 VOICE_McRxProc(VOS_VOID);
extern VOS_VOID VOICE_McSendCnfMsgToVc(VOS_UINT16 uhwMsgId,VOS_UINT32 uwReceiverPid, VOS_UINT32 uwRet);
extern VOS_VOID VOICE_McSendModemMsgToGUPhy(
                       VOICE_MC_MODEM_OPT_ENUM_UINT16 enLinkOpt,
                       VOS_UINT32 uwReceiverPid,
                       VOS_UINT16 uhwMsgId);
extern VOS_VOID VOICE_McSendModemMsgToTDPhy(
                       VOICE_MC_MODEM_OPT_ENUM_UINT16 enLinkOpt,
                       VOS_UINT32                     uwReceiverPid,
                       VOS_UINT16                     uhwMsgId
                       );
extern VOS_VOID VOICE_McSendQryCnfMsgToVc(VOS_UINT16 uhwMsgId,VOS_UINT32 uwReceiverPid, VOS_UINT32 uwRet);
extern VOS_UINT32 VOICE_McSetCodec(VCVOICE_SET_CODEC_REQ_STRU *pstMsg, VOS_UINT16 uhwModemNo);
extern VOS_VOID VOICE_McSetCodecType(VOS_UINT16 uhwNetMode, VOS_UINT16 uhwCodecType, VOS_UINT16 uhwModemNo);
extern VOS_UINT32 VOICE_McSetDev(VOS_UINT16 uhwDevMode, VOS_UINT32 uwUpdate, VOS_UINT16 uhwModemNo);
extern VOS_VOID VOICE_McSetDevChannel(VOS_UINT16 uhwDevMode);
extern VOS_UINT32 VOICE_McSetLoopDisable(VOS_VOID);
extern VOS_UINT32 VOICE_McSetLoopEnable(VOS_VOID);
extern VOS_UINT32 VOICE_McStart(VCVOICE_START_REQ_STRU *pstMsg);
extern VOS_VOID VOICE_McStop(VOS_UINT16 uhwModemNo);
extern VOS_VOID VOICE_McTdChkMicConflick(VOS_VOID);
extern VOS_VOID VOICE_McTxDataToGphy(VOS_VOID);
extern VOS_VOID VOICE_McTxDataToTDphy(VOS_VOID);
extern VOS_VOID VOICE_McTxDataToWphy(VOS_VOID);
extern VOS_UINT32 VOICE_McTxProc(VOS_VOID);
extern VOS_VOID VOICE_McUnRegisterCaptureCallBack(VOS_VOID);
extern VOS_UINT32 VOICE_McUpdate(VOS_UINT16 uhwModemNo);
extern VOS_UINT32 VOICE_MsgApNoteCCPUResetRP(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgGphyPlayIndR(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgGphyRecordIndR(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgGphyRxDataIndR(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgGphyUpdateParaCmdRP(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgGUPhyBackGroundCnfRP( VOS_VOID *pstOsaMsg );
extern VOS_UINT32 VOICE_MsgGUPhyForeGroundCnfRP( VOS_VOID *pstOsaMsg );
extern VOS_UINT32 VOICE_MsgOmGetNvReqRP(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgOmQueryStatusReqIRP(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgOmSetNvReqRP(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgPhySuspendCmdR(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgTdPhyBackGroundCnfRP( VOS_VOID *pstOsaMsg );
extern VOS_UINT32 VOICE_MsgTdPhyForeGroundCnfRP( VOS_VOID *pstOsaMsg );
extern VOS_UINT32 VOICE_MsgTDphyRxDataIndR(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgTdSyncIndR(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgVcBackGroundReqRP( VOS_VOID *pstOsaMsg  );
extern VOS_UINT32 VOICE_MsgVcForeGroundReqRP( VOS_VOID *pstOsaMsg  );
extern VOS_UINT32 VOICE_MsgVcGroundQryReqRP( VOS_VOID *pstOsaMsg );
extern VOS_UINT32 VOICE_MsgVcSetCodecReqP(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgVcSetCodecReqR(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgVcSetDevReqI(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgVcSetDevReqRP(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgVcSetVolReqIRP(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgVcStartReqI(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgVcStopReqRP(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgVoiceLoopReqIRP(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgVoicePlayIndR(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgVoiceRecordIndR(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgVoiceTxProcReqR(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgVoiceRxProcReqR(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgWphyRxDataIndR(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgWphySyncIndR(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgImsaRxDataIndR( VOS_VOID *pstOsaMsg );
extern VOS_VOID VOICE_SyncTdSubFrm5msIsr(VOS_VOID);
extern VOS_VOID VOICE_TdsRxTimerIsr(VOS_UINT32 uwTimer, VOS_UINT32 uwPara);
extern VOS_VOID VOICE_TdsTxTimerIsr(VOS_UINT32 uwTimer, VOS_UINT32 uwPara);
extern VOS_VOID VOICE_Timer20msIsr(VOS_UINT32 uwTimer, VOS_UINT32 uwPara);
extern VOS_VOID VOICE_Timer3msIsr(VOS_UINT32 uwTimer, VOS_UINT32 uwPara);
extern VOS_VOID VOICE_Timer6msIsr(VOS_UINT32 uwTimer, VOS_UINT32 uwPara);
extern VOS_UINT32 VOICE_MsgGphySyncIndR(VOS_VOID *pstOsaMsg);
extern VOS_UINT16 VOICE_McDetectRxFrmLost(VOS_VOID);
extern VOS_VOID VOICE_Timer20msRxIsr(VOS_UINT32 uwTimer, VOS_UINT32 uwPara);
extern VOS_UINT32 VOICE_McTxEnc( VOS_VOID );
extern VOS_VOID VOICE_McGsmChkMicConflick(VOS_VOID);
extern VOS_VOID VOICE_McSetBackGround(VOICE_MC_MODEM_NUM_ENUM_UINT16      enModemNo);
extern VOS_VOID VOICE_McTxDataToImsa(VOS_VOID);
extern VOS_UINT32 VOICE_MsgImsaCfgReqR( VOS_VOID *pstOsaMsg );
extern VOS_UINT32 VOICE_MsgGphyChannelQualityIndR( VOS_VOID *pstOsaMsg  );
extern VOS_UINT32 VOICE_MsgWphyChannelQualityIndR( VOS_VOID *pstOsaMsg  );
extern VOS_VOID VOICE_Timer30msDiagIsr(VOS_UINT32 uwTimer, VOS_UINT32 uwPara);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif /* end of VOICE_mc.h */
