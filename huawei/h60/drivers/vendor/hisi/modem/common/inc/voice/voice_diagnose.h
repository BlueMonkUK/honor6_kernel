

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "om_log.h"
#include "CodecInterface.h"
#include "HifiOmInterface.h"
#include "omnvinterface.h"

#ifndef __VOICE_DIAGNOSE_H__
#define __VOICE_DIAGNOSE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define VOICE_DIAG_CHECK_POINT_NUM              (8)                             /* ������8���������� */
#define VOICE_DIAG_AMR_SID_NUM                  (8)                             /* ����DTX��ʱ��AMR�������¾���֡�������BAD֡��(����SID֡����) */
#define VOICE_DIAG_FR_EFR_SID_NUM               (24)                            /* ����DTX��ʱ��FR/EFR�������¾���֡�������BAD֡��(����SID֡����) */
#define VOICE_DIAG_HR_SID_NUM                   (12)                            /* ����DTX��ʱ��HR�������¾���֡�������BAD֡��(����SID֡����) */
#define VOICE_DIAG_TIME_INTERVAL                (2)                             /* ÿ2s���һ�� */
#define VOICE_DIAG_FRAME_NUM_PER_SECOND         (50)                            /* 1s����50֡�������� */
#define VOICE_DIAG_MAX_SILENCE_NUM              (3)                             /* ����3�μ�⵽�������ϱ���ͨ */
#define VOICE_DIAG_GOOD_FRAME_PERCENT_THD       (26213)                         /* Q15 (0.8*32767) */
#define VOICE_DIAG_CHANNEL_QUALITY_LEN          (16)
#define VOICE_DIAG_GSM_RX_LEVEL_OFFSET          (316)

#define VOICE_DIAG_REPORT_LINEIN                (0)
#define VOICE_DIAG_REPORT_LINEOUT               (1)
#define VOICE_DIAG_REPORT_MICIN                 (2)

#define VOICE_DIAG_TIMESTAMP_MS_RATIO           (10000)              /*1s��0.1ms��ת��*/

#define ERR_LOG_ALARM_REPORT_ENABLE             (0x80)             /* 10000000 �����ϱ�ʹ�� */
#define ERR_LOG_VOICE_ALARM_ENABLE              (0x01)

/* ��ȡ������������������ */
#define VOICE_DiagGetCfgPtr()                   (&g_stVoiceDiagCfg)
#define VOICE_DiagGetFrameNumPtr()              (&g_stDiagFrameNum)
#define VOICE_DiagGetDLSilenceNum()             (g_uwDiagDLSilenceNum)
#define VOICE_DiagSetDLSilenceNum(uwValue)      (g_uwDiagDLSilenceNum = uwValue)
#define VOICE_DiagAddDLSilenceNum()             (g_uwDiagDLSilenceNum++)
#define VOICE_DiagGetReportFlag(uwValue)        (g_uhwVoiceDiagReportFalg[uwValue])
#define VOICE_DiagGetChannelQualityPtr()        (&g_stChannelQuality)
/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
/* ͨ·��ö�� */
enum VOICE_DIAG_MODEM_NUM_ENUM
{
    VOICE_DIAG_MODEM0              = 0,                 /* MODEM-0 */
    VOICE_DIAG_MODEM1,                                  /* MODEM-1 */
    VOICE_DIAG_MODEM_NUM_BUTT
};
typedef VOS_UINT16  VOICE_DIAG_MODEM_NUM_ENUM_UINT16;

enum VOICE_DIAG_FLAG_ENUM
{
    VOICE_DIAG_FALSE              = 0,
    VOICE_DIAG_TRUE,
    VOICE_DIAG_BUTT
};
typedef VOS_UINT16  VOICE_DIAG_FLAG_ENUM_UINT16;

/* ����ͨ���澯ԭ��ö�� */
enum VOICE_DIAG_ERR_CAUSE_ENUM
{
    VOICE_RX_NO_SOUND           = 0,
    VOICE_TX_NO_SOUND,
    VOICE_CONTROL_FAULT,
    VOICE_SUSPEND_SLOW,
    VOICE_ERR_CAUSE_BUTT
};
typedef VOS_UINT16 VOICE_DIAG_ERR_CAUSE_ENUM_UINT16;

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
typedef struct
{
    VOS_UINT16              uhwGoodFrameNum;
    VOS_UINT16              uhwTotalFrameNum;
} VOICE_DIAG_FRAMENUM_STRU;

/* ����LineIn����������֡��Ϣ */
typedef struct
{
    VCVOICE_DEV_MODE_ENUM_UINT16        enDevMode;                              /* �����豸���� */
    CODEC_NET_MODE_ENUM_UINT16          enNetMode;
    CODEC_ENUM_UINT16                   enCodecType;                            /* ����������: 0-AMR, 1-EFR, 2-FR, 3-HR */
    VOS_UINT16                          usIsEncInited;                          /*�������Ƿ��ѳ�ʼ��*/
    VOS_UINT16                          usIsDecInited;                          /*�������Ƿ��ѳ�ʼ��*/
    CODEC_BFI_ENUM_UINT16               enBfi;                                  /* ��֡��־,HR/FR/EFRʹ�� 0 dis 1 enable,bad*/
    CODEC_SID_ENUM_UINT16               enSid;                                  /* HR/FR/EFRʹ�� */
    CODEC_AMR_TYPE_RX_ENUM_UINT16       enAmrFrameType;                         /* AMR֡����, TX_SPEECH_GOOD/TX_SID_FIRST/TX_SID_UPDATA/TX_NO_DATA */
    CODEC_AMR_FQI_QUALITY_ENUM_UINT16   enQualityIdx;                           /* W/TD��֡���� */
    VOS_UINT16                          uhwReserve;
} VOICE_DIAG_RX_DATA_STRU;

typedef struct
{
    VOS_UINT16 uhwIsBadCell;                                                    /* ��ǰС���ź��Ƿ�ܲ� */
    VOS_UINT16 uhwReserve;
    VOS_UINT32 auwChannelQuality[VOICE_DIAG_CHANNEL_QUALITY_LEN];
}VOICE_DIAG_CHANNEL_QUALITY_STRU;

typedef struct
{
    VOS_UINT16 uhwTxDmaIsrFlag;     /* �Ƿ��յ�����DMA�ж� */
    VOS_UINT16 uhwRxDmaIsrFlag;     /* �Ƿ��յ�����DMA�ж� */
}VOICE_DIAG_DMA_ISR_STRU;

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern VOICE_DIAG_FRAMENUM_STRU   g_stDiagFrameNum;
extern VOICE_DIAG_NV_STRU         g_stVoiceDiagCfg;
extern VOS_UINT32                 g_uwDiagDLSilenceNum;
extern VOS_UINT16                 g_uhwVoiceDiagReportFalg[VOICE_DIAG_MODEM_NUM_BUTT][VOICE_ERR_CAUSE_BUTT];
extern VOICE_DIAG_CHANNEL_QUALITY_STRU g_stChannelQuality;

extern VOICE_DIAG_DMA_ISR_STRU      g_stDiagDmaIsr;
extern VOS_UINT32                   g_auwSuspendBeginTs[VOICE_DIAG_MODEM_NUM_BUTT];
extern NV_ID_ERR_LOG_CTRL_INFO_STRU g_stSysErrLogCfg;
/*****************************************************************************
  10 ��������
*****************************************************************************/

extern VOS_VOID VOICE_DiagCalcFrameNum(VOICE_DIAG_RX_DATA_STRU *pstRxData);
extern VOS_VOID VOICE_DiagInit(VOICE_DIAG_NV_STRU *pstVoiceDiagCfg);
extern VOS_VOID VOICE_DiagLineIn(
                       VOICE_DIAG_MODEM_NUM_ENUM_UINT16 enActiveModemNo,
                       VOICE_DIAG_RX_DATA_STRU         *pstRxData);
extern VOS_VOID VOICE_DiagReset( VOS_VOID );
extern VOS_VOID VOICE_DiagMsgReport(
                       VOICE_DIAG_ERR_CAUSE_ENUM_UINT16         uhwErrCause,
                       VOS_UINT16                               uhwModemNo,
                       HIFI_ERROR_EVENT_ONE_WAY_NO_SOUND_STRU  *pstOneWayNoSound);

extern VOS_VOID VOICE_DiagGsmGoodFrmNum( VOICE_DIAG_RX_DATA_STRU *pstRxData );
extern VOS_VOID VOICE_DiagAlarmReport(
                        VOS_UINT16                               uhwModemNo,
                        VOICE_DIAG_ERR_CAUSE_ENUM_UINT16         uhwAlarmCause,
                        VOS_VOID                                *pstReportData,
                        VOS_UINT32                               uwDataSize,
                        HIFI_ERR_LOG_ALARM_ID_ENUM_UINT16        uhwAlarmId);
extern VOS_VOID VOICE_DiagSuspend(
                        VOS_UINT16              uhwModemNo,
                        VOS_UINT16              uhwCurNetMode,
                        VOS_UINT16              uhwPreNetMode);




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of voice_diagnose.h */
