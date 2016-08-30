

#ifndef __VOICE_LOG_H__
#define __VOICE_LOG_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "OmCodecInterface.h"
#include "codec_typedefine.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#ifdef _MED_ERRORLOG
/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define VOICE_LOG_CNTER_TOP                                   0x7F00              /* ������־��ʼ�� */
#define VOICE_LOG_CNTER_MAX_NUM                               (15)                /* ��־������������, <0xFF */

#define VOICE_ERRLOG_ERRNO_BASE                               (0x00000601)        /* VOICE ��� ErrorLog ��ʼ������� */
#define VOICE_ERRLOG_EVENT_REPORT_MAX                         (1)                 /* ÿ��ͨ���е����¼������ϱ��������� */
#define VOICE_ERRLOG_EVENT_DEFAULT_THD                        (50)                /* Ĭ�ϵ��¼��������� */
#define VOICE_ERRLOG_EVENT_MAXIMUM_THD                        (502)               /* �¼������������ֵ */
#define VOICE_ERRLOG_LAST_CYCLE_INI_VAL                       (-2)                /* ���һ���¼��������ڳ�ʼ����ֵ */

#define MSG_VOICE_OM_ERRLOG_PID                               (0x5100)            /* ErrorLog PID �� */
#define VOICE_ERRLOG_CHECK_PERIOD                             (250)               /* 5�������� */

#define VOICE_ERRLOG_CONFIG_HEAD_LEN                          (2)                 /* ERROR�����¼�������ʼ��ַƫ�Ƴ���, ��λVOICE_INT16 */

#define VOICE_ERRLOG_EVENTS_WCDMA_DEC_INT_RECEIVE \
                CODEC_ERRLOG_EVENTS_WCDMA_DEC_INT_LOST                            /* ת���� LOST �¼� */
#define VOICE_LOG_PERIOD                                      (250)

/* ��װȫ�ֱ��� */
#define VOICE_ERRLOG_GetErrLogMsgPtr()                        (&g_stVoiceErrLogMsg)
#define VOICE_ERRLOG_GetRecordHeadPtr()                       (&g_stVoiceErrLogMsg.stRecordHead)
#define VOICE_ERRLOG_GetEventsInfoPtr()                       (g_stVoiceErrLogStatus.astEventsInfo)

#endif

#ifdef _MED_ERRORLOG

/*****************************************************************************
 ��    ��  : VOICE_VsErrLogEventRec
 ��������  : ��¼ErrorLog�¼�
 �������  : enEvent, �¼����
 �������  : ��
 �� �� ֵ  : ��

 ****************************************************************************/
#define VOICE_ErrLogEventRec(enEvent) \
{ \
    VOICE_ErrLogRecord((enEvent)); \
}

/*****************************************************************************
 ��    ��  : VOICE_VsErrLogEventCycle
 ��������  : ���ڴ������¼��ϱ���麯��
 �������  : shwPeriod, ����¼�������, ������shwPeriod�����ú�����,�����¼����
 �������  : ��
 �� �� ֵ  : ��

 ****************************************************************************/
#define VOICE_ErrLogEventCycle(shwPeriod) \
{ \
    VOICE_ErrLogCycle((shwPeriod)); \
}

/*****************************************************************************
 ��    ��  : VOICE_VsErrLogReset
 ��������  : ���ü�����
 �������  : shwPeriod, ����¼�������, ������shwPeriod�����ú�����,�����¼����
 �������  : ��
 �� �� ֵ  : ��

 ****************************************************************************/
#define VOICE_ErrLogReset() \
{ \
    VOICE_ErrLogResetStatus(); \
}
#else
#define VOICE_ErrLogEventRec(enEvent)
#define VOICE_ErrLogEventCycle(shwPeriod)
#define VOICE_ErrLogReset()
#endif

#ifdef _MED_ERRORLOG
/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/

/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
/* ErrorLog �¼����� */
typedef struct
{
    CODEC_ERRLOG_EVENT_TYPE_ENUM_UINT16                       enType;             /* �¼����� */
    CODEC_ERRLOG_EVENT_MODE_ENUM_UINT16                       enMode;             /* �¼����� */
    VOS_UINT16                                              uhwThd;             /* �¼����� */
    VOS_UINT16                                              uhwReserve;

} VOICE_ERRLOG_EVENT_RULE_STRU;

/* ErrorLog �¼�������Ϣ */
typedef struct
{
    VOICE_ERRLOG_EVENT_RULE_STRU                              stRules;            /* �¼�Լ������ */
    VOS_UINT16                                              uhwCnt;             /* �¼����� */
    VOS_INT16                                               shwLastCyc;         /* ���һ�γ��ִ����Cycle */
    VOS_UINT16                                              uhwRprtd;           /* �����ϱ���־ */
    VOS_UINT16                                              uhwReserve;

} VOICE_ERRLOG_EVENT_INFO_STRU;

/* ErrorLog ����ʱ״̬ */
typedef struct
{
    VOS_UINT16                                              uhwEnable;          /* ErrorLog ʹ�ܱ�־ */
    VOS_INT16                                               shwCycleCnt;        /* ���д��������� */
    VOICE_ERRLOG_EVENT_INFO_STRU                              astEventsInfo[CODEC_ERRLOG_EVENTS_ENUM_BUTT]; /* ͳ�Ƹ�������¼�����Ϣ */

} VOICE_ERRLOG_STATUS_STRU;

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
extern VOS_VOID VOICE_ErrLogInit( CODEC_ERRLOG_CONFIG_STRU *pstErrlogCfg );
extern VOS_VOID VOICE_ErrLogResetEventCnts( VOICE_ERRLOG_EVENT_INFO_STRU *pstEvent );
extern VOS_UINT16 VOICE_ErrLogGetEventThd( CODEC_ERRLOG_EVENTS_ENUM_UINT16 enEvent );
extern VOS_VOID VOICE_ErrLogRecord( CODEC_ERRLOG_EVENTS_ENUM_UINT16 enEvent );
extern VOS_VOID VOICE_ErrLogUpdateEnv( VOS_VOID );
extern VOS_VOID VOICE_ErrLogPackLogHead(MNTN_HEAD_INFO_STRU *pstMsgMntnHead,
                                       CODEC_ERRLOG_EVENTS_ENUM_UINT16 enEvent );
extern VOS_VOID VOICE_ErrLogReport( CODEC_ERRLOG_EVENTS_ENUM_UINT16 enEvent );
extern VOS_UINT32 VOICE_ErrLogCheckEvent( CODEC_ERRLOG_EVENTS_ENUM_UINT16 enEvent );
extern VOS_VOID VOICE_ErrLogCycle( VOS_INT16 shwPeriod );
extern VOS_VOID VOICE_ErrLogResetStatus( VOS_VOID );
#endif
extern VOS_VOID VOICE_ErrLogCheckDecStatus(VOS_VOID *pstDecObj);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif  /* end of med_vs_log.h */

