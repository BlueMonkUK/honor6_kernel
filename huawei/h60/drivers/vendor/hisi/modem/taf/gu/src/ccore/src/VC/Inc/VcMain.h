
#ifndef _APP_VC_MAIN_H_
#define _APP_VC_MAIN_H_

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "vos.h"
#include  "PsTypeDef.h"
#include "AppVcApi.h"
#include "VcComm.h"
#if (FEATURE_ON == FEATURE_IMS)
#include "VcImsaInterface.h"
#endif
#include "VcCallInterface.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  1 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define VC_MAX_BUFF_MSG_NUM                 4

/*�ڲ��궨��*/
#define APP_VC_GetState()                   (g_stVcStateMgmt.enState)
#define APP_VC_GetCurrDevMode()             (g_stVcStateMgmt.enCurrDevMode)
#define APP_VC_GetPreDevMode()              (g_stVcStateMgmt.enPreDevMode)
#define APP_VC_GetDevHandle()               (g_stVcStateMgmt.iDevHandle)
#define APP_VC_GetRadioMode()               (g_stVcStateMgmt.enRadioMode)
#define APP_VC_GetCurrVolume()              (g_stVcStateMgmt.sCurrVolume)
#define APP_VC_GetPreVolume()               (g_stVcStateMgmt.sPreVolume)
#define APP_VC_GetPcVoiceSupportFlg()       (g_stVcStateMgmt.ucPcVoiceSupportFlag)
#define APP_VC_GetPcVoicePort()             (g_stVcStateMgmt.ucVoicePort)
#define APP_VC_GetPreMuteStatus()           (g_stVcStateMgmt.enPreMuteStatus)
#define APP_VC_GetCurrMuteStatus()          (g_stVcStateMgmt.enCurrMuteStatus)

#define APP_VC_MUTED_VOLUME                 (-80)
#define APP_VC_UNMUTED_VOLUME               (0)

#define APP_VC_GetCallStatus()              (g_stVcStateMgmt.bInCall)

#define APP_VC_GetStartHifiOrig()           (g_stVcStateMgmt.enStartHifiOrig)

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

enum APP_VC_STATE_ENUM
{
    APP_VC_S_NULL,                                                              /* ��״̬ */
    APP_VC_S_WAIT_INTERNAL_SET_DEV_RSLT,                                        /* �ȴ�DEVICE���ý��״̬ */
    APP_VC_S_WAIT_INTERNAL_SET_START_RSLT,                                      /* �ȴ�START���ý��״̬ */
    APP_VC_S_WAIT_INTERNAL_SET_CLOSE_RSLT,                                      /* �ȴ�CLOSE���ý��״̬ */
    APP_VC_S_WAIT_INTERNAL_SET_CODEC_RSLT,                                      /* �ȴ�CODEC���ý��״̬ */
    APP_VC_S_WAIT_AT_SET_DEV_RSLT,                                              /* �ȴ�AT����DEVICE���״̬ */
    APP_VC_S_WAIT_AT_SET_VOLUME_RSLT,                                           /* �ȴ�AT����VOLUME���״̬ */
    APP_VC_S_WAIT_AT_SET_MUTE_STATUS_RSLT,                                      /* �ȴ�AT���þ������״̬ */

    APP_VC_S_BUTT
};
typedef VOS_UINT16  APP_VC_GLOBAL_STATE_ENUM_U16;

enum APP_VC_MSG_ORIGIN_ENUM
{
    APP_VC_MSG_ORIGIN_AT,                                                           /* ����AT���õ���Ϣ */
    APP_VC_MSG_ORIGIN_INTERNAL,                                                     /* �����ڲ���Ϣ */
    APP_VC_MSG_ORIGIN_IMSA,                                                         /* ����IMSA����Ϣ */
    APP_VC_MSG_ORIGIN_BUTT
};
typedef VOS_UINT16  APP_VC_MSG_ORIGIN_ENUM_U16;

/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  7 STRUCT����
*****************************************************************************/


/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/



/*****************************************************************************
  10 ��������
*****************************************************************************/
VOS_VOID APP_VC_BuffMsgProc(VOS_VOID);

VOS_UINT32 APP_VC_SaveCurVolume(VOS_VOID);
VOS_UINT32 APP_VC_GetCurVolume(VOS_INT16 *pCurVolume);

VOS_VOID  APP_VC_AppQryModeReqProc(APP_VC_REQ_MSG_STRU  *pstAppMsg);
VOS_VOID  APP_VC_AppSetPortReqProc(APP_VC_REQ_MSG_STRU *pstAppMsg);
VOS_VOID  APP_VC_AppQryPortReqProc(APP_VC_REQ_MSG_STRU *pstAppMsg);
VOS_UINT32 APP_VC_GetVoiceTestFlag(VOS_UINT32 *pVoiceTestFlag);

#if (FEATURE_ON == FEATURE_IMS)
VC_IMSA_EXCEPTION_CAUSE_ENUM_UINT32  APP_VC_ConvertVcOpenChannelFailCauseToImsaExceptionCause(
    APP_VC_OPEN_CHANNEL_FAIL_CAUSE_ENUM_UINT32              enVcCause
);
CALL_VC_CODEC_TYPE_ENUM_U8  APP_VC_ConvertImsaCodeTypeToCallCodeType(
    IMSA_VC_CODEC_TYPE_ENUM_UINT8       enImsaCodeType
);
VOS_VOID  APP_VC_ImsaStartHifiNtfProc(IMSA_VC_START_HIFI_NTF_STRU* pstMsg);
VOS_VOID  APP_VC_ImsaStopHifiNtfProc(IMSA_VC_STOP_HIFI_NTF_STRU* pstMsg);
VOS_VOID  APP_VC_ImsaHifiParaChangeNtfProc(IMSA_VC_HIFI_PARA_CHANGED_NTF_STRU* pstMsg);
VOS_VOID  APP_VC_ImsaMsgProc(VOS_VOID *pMsg);

#endif
 #if (FEATURE_ON == FEATURE_PTM)
VOS_VOID APP_VC_AppVCFailErrRecord(
    VOS_UINT16                          usName,
    VOS_UINT16                          usCause
);
 #endif

#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of MnCallCtx.h */

