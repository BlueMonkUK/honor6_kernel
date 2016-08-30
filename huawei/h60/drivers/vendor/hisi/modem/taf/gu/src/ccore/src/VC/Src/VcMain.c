

/*****************************************************************************
   1 ͷ�ļ�����
*****************************************************************************/
#include "VcMain.h"
#include "VcCallInterface.h"
#include "VcPhyInterface.h"
#include "VcComm.h"
#include "MnComm.h"
#include "MnErrorCode.h"
#include "product_config.h"
#include "AppVcApi.h"
#include "om.h"
#include "MM_Share_inc.h"
#include "apminterface.h"
#include "TafOamInterface.h"
#include "VcCodecInterface.h"
#include "VcCtx.h"
#include "VcProcNvim.h"
#include "NVIM_Interface.h"
#include "NasNvInterface.h"
#include "TafNvInterface.h"
#if (FEATURE_ON == FEATURE_IMS)
#include "VcImsaInterface.h"
#endif
#if (FEATURE_ON == FEATURE_PTM)
#include "NasComm.h"
#include "TafSdcCtx.h"
#include "TafSdcLib.h"
#endif
#ifdef  __cplusplus
  #if  __cplusplus
      extern "C"{
  #endif
#endif



/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_VC_MAIN_C
/*lint +e767*/

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
APP_VC_STATE_MGMT_STRU                      g_stVcStateMgmt;
#if (FEATURE_HIFI == FEATURE_ON)
VOS_UINT32                                  g_Vc_VoiceOpenFlg = VOS_TRUE;
#else
VOS_UINT32                                  g_Vc_VoiceOpenFlg = VOS_FALSE;
#endif

/*****************************************************************************
   3 ����ʵ��
*****************************************************************************/
extern VOS_VOID APP_VC_ReadNvimInfo(VOS_VOID);


VOS_VOID NAS_MNTN_FailtoOpenCsChannel(
    VOS_UINT16                          usPort,
    VOS_UINT32                          ulOmOperResult
)
{
    VOS_UINT32                                      ulRet;
    NAS_MNTN_CS_CHANNEL_OPEN_FAILURE_EVENT_STRU     stChannelOpenFailure;

    /* 1����OM������ͨ����ʧ���¼���� */
    /* 1��1����ʼ����־�¼��ֲ����� */
    PS_MEM_SET(&stChannelOpenFailure, 0x00, sizeof(stChannelOpenFailure));

    /* 1��2����ȡNAS���ĵ�SIM����Ϣ�� */
    NAS_MMA_OutputUsimInfo(&stChannelOpenFailure.stUsimInfo);

    /* 1��3����¼Ҫ�򿪵�����ͨ���˿ں�OMִ�н���� */
    stChannelOpenFailure.usPort         = usPort;
    stChannelOpenFailure.ulOmOperResult = ulOmOperResult;

    /* 2�������OM������ͨ����ʧ���¼���Ϣ */
    ulRet = MNTN_RecordErrorLog(NAS_EVT_TYPE_DEF(WUEPS_PID_VC,MNTN_NAS_ERRORLOG_CS_CHANNEL_OPEN_FAILURE),
                                &stChannelOpenFailure,
                                sizeof(stChannelOpenFailure));
    if (VOS_OK != ulRet)
    {
        VC_WARN_LOG("NAS_MNTN_FailtoOpenCsChannel: MNTN_RecordErrorLog fail.");
        vos_printf("NAS_MNTN_FailtoOpenCsChannel: MNTN_RecordErrorLog fail.\n");
    }

    return;
}
VOS_VOID NAS_MNTN_CsMedTimeOut(VOS_UINT32                          ulTimerId)
{
    VOS_UINT32                          ulRet;
    NAS_MNTN_CS_MED_TIMEOUT_EVENT_STRU  stCsMedTimeOut;

    /* ���˲����ĵĶ�ʱ����ʱ�¼�������ʱ����ʱ�¼�����Ϊ APP_VC_TIMER_START��
    APP_VC_TIMER_SET_DEV��APP_VC_TIMER_SET_CODEC��APP_VC_TIMER_STOP��
    ����ERROR LOG�ӿ��������־ */
    if ((APP_VC_TIMER_START     != ulTimerId)
     && (APP_VC_TIMER_SET_DEV   != ulTimerId)
     && (APP_VC_TIMER_SET_CODEC != ulTimerId)
     && (APP_VC_TIMER_STOP      != ulTimerId))
    {
        return;
    }

    /* 1��MED���ֳ�ʱ�¼���� */
    /* 1��1����ʼ����־�¼��ֲ����� */
    PS_MEM_SET(&stCsMedTimeOut, 0x00, sizeof(stCsMedTimeOut));

    /* 1��2����ȡNAS���ĵ�SIM����Ϣ�� */
    NAS_MMA_OutputUsimInfo(&stCsMedTimeOut.stUsimInfo);

    /* 1��3����¼��ʱ����ʱ�¼��� */
    stCsMedTimeOut.ulTimerId = ulTimerId;

    /* 2�������OM������ͨ����ʧ���¼���Ϣ */
    ulRet = MNTN_RecordErrorLog(NAS_EVT_TYPE_DEF(WUEPS_PID_VC,MNTN_NAS_ERRORLOG_CS_MED_TIMEOUT),
                                &stCsMedTimeOut,
                                sizeof(stCsMedTimeOut));
    if (VOS_OK != ulRet)
    {
        VC_WARN_LOG("NAS_MNTN_CsMedTimeOut: MNTN_RecordErrorLog fail.");
        vos_printf("NAS_MNTN_CsMedTimeOut: MNTN_RecordErrorLog fail.\n");
    }
    return;
}
VCVOICE_WORK_TYPE_ENUM_UINT16 APP_VC_GetWorkType(VOS_VOID)
{
    /* ��9235nvֵΪ0x55aa55aaʱΪvcvoice testģʽ */
    if (APP_VC_VOICE_TEST_FLAG_ENABLE == g_stVcStateMgmt.ulVoiceTestFlag)
    {
        return VCVOICE_WORK_TYPE_TEST;
    }
    else
    {
        if (VOS_FALSE == APP_VC_GetPcVoiceSupportFlg())
        {
            if (VOS_TRUE == DRV_GET_CODEC_FUNC_STATUS())
            {
                return VCVOICE_WORK_TYPE_NORMAL;
            }

            return VCVOICE_WORK_TYPE_TEST;
        }
        else
        {
            return VCVOICE_WORK_TYPE_NORMAL;
        }
    }
}
VOS_VOID APP_VC_UpdateState(APP_VC_GLOBAL_STATE_ENUM_U16    enState)
{
    g_stVcStateMgmt.enState = enState;
}


VOS_VOID APP_VC_UpdateCurrDevMode(VC_PHY_DEVICE_MODE_ENUM_U16    enDevMode)
{
    g_stVcStateMgmt.enCurrDevMode = enDevMode;
}


VOS_VOID APP_VC_UpdateCurrVolume(VOS_INT16     sCurrVolume)
{
    g_stVcStateMgmt.sCurrVolume = sCurrVolume;
}


VOS_VOID APP_VC_UpdatePreDevMode(VC_PHY_DEVICE_MODE_ENUM_U16    enDevMode)
{
    g_stVcStateMgmt.enPreDevMode = enDevMode;
}


VOS_VOID APP_VC_UpdatePreVolume(VOS_INT16     sCurrVolume)
{
    g_stVcStateMgmt.sPreVolume = sCurrVolume;
}


VOS_VOID APP_VC_UpdateRadioMode(CALL_VC_RADIO_MODE_ENUM_U8    enRadioMode)
{
    g_stVcStateMgmt.enRadioMode = enRadioMode;
}


VOS_VOID APP_VC_UpdateClientId(MN_CLIENT_ID_T   clientId)
{
    g_stVcStateMgmt.clientId = clientId;
}



VOS_VOID APP_VC_UpdateOpId(MN_OPERATION_ID_T   opId)
{
    g_stVcStateMgmt.opId = opId;
}


VOS_VOID APP_VC_UpdataPreMuteStatus(APP_VC_MUTE_STATUS_ENUM_UINT8 enMuteStatus)
{
    g_stVcStateMgmt.enPreMuteStatus = enMuteStatus;

    return;
}


VOS_VOID APP_VC_UpdataCurrMuteStatus(APP_VC_MUTE_STATUS_ENUM_UINT8 enMuteStatus)
{
    g_stVcStateMgmt.enCurrMuteStatus = enMuteStatus;

    return;
}


VOS_VOID  APP_VC_ReportEvent(
    APP_VC_EVENT_ENUM_U32              enEventType,
    VOS_UINT32                         ulErrCode
)
{
    APP_VC_EVENT_INFO_STRU              stVcEvtInfo;

    PS_MEM_SET(&stVcEvtInfo, 0x00, sizeof(APP_VC_EVENT_INFO_STRU));

    /* �����ֵ��80����ΪAT�Ķ��巶ΧΪ0~92,���������֮�䶨��ķ�Χ
       Ϊ-80~12*/
    stVcEvtInfo.usVolume     = (VOS_UINT16)(g_stVcStateMgmt.sCurrVolume + 80);
    stVcEvtInfo.clientId     = g_stVcStateMgmt.clientId;
    stVcEvtInfo.opId         = g_stVcStateMgmt.opId;
    stVcEvtInfo.enDevMode    = APP_VC_GetCurrDevMode();
    stVcEvtInfo.enMuteStatus = APP_VC_GetCurrMuteStatus();

    if (ulErrCode != MN_ERR_NO_ERROR)
    {
        stVcEvtInfo.bSuccess = VOS_FALSE;
    }
    else
    {
        stVcEvtInfo.bSuccess = VOS_TRUE;
    }
    stVcEvtInfo.ulErrCode = ulErrCode;

    switch(enEventType)
    {
        case APP_VC_EVT_SET_VOLUME:
        case APP_VC_EVT_SET_VOICE_MODE:
        case APP_VC_EVT_GET_VOLUME:
        case APP_VC_EVT_SET_MUTE_STATUS:
        case APP_VC_EVT_GET_MUTE_STATUS:
            MN_SendClientEvent(g_stVcStateMgmt.clientId,MN_CALLBACK_VOICE_CONTROL,enEventType,(VOS_VOID *)&stVcEvtInfo);
            /*if (VOS_TRUE == stVcEvtInfo.bSuccess) */
            /*{ */
            /*    MN_SendClientEvent(MN_CLIENT_ALL,MN_CALLBACK_VOICE_CONTROL,APP_VC_EVT_PARM_CHANGED,(VOS_VOID *)&stVcEvtInfo); */
            /*} */
            break;

        default:
            VC_WARN_LOG1("APP_VC_ReportEvent:Wrong Event Type.", (VOS_INT32)enEventType);
            break;

    }
}


VOS_VOID  APP_VC_InitGlobeVariable(VOS_VOID)
{
    APP_VC_MS_CFG_INFO_STRU             *pstMsCfgInfo;
    VOS_INT16                            sCurVolume;
    VOS_UINT32                           ulVoiceTestFlagValue;
    VOS_UINT32                           ulRlst;

    pstMsCfgInfo            = APP_VC_GetCustomCfgInfo();
    sCurVolume              = 0;
    ulVoiceTestFlagValue    = 0;
    ulRlst                  = 0;

    VOS_MemSet(&g_stVcStateMgmt, 0, sizeof(g_stVcStateMgmt));

    g_stVcStateMgmt.iDevHandle = DRV_CODEC_OPEN("/dev/codec0",0,0);
    if (-1 == g_stVcStateMgmt.iDevHandle)
    {
        VC_ERR_LOG("APP_VC_InitGlobeVariable: Open codec0 Fail");
    }

    g_stVcStateMgmt.enCurrDevMode = VC_PHY_DEVICE_MODE_EARPHONE;

    if (APP_VC_NV_ITEM_ACTIVE == pstMsCfgInfo->usVcPcvoiceSupportFlag)
    {
        g_stVcStateMgmt.ucPcVoiceSupportFlag = VOS_TRUE;
     }
    else
    {
        g_stVcStateMgmt.ucPcVoiceSupportFlag = VOS_FALSE;
    }

    /* ���֧�� PC VOICE, �����豸ģʽĬ��ֵΪ PC VOICE */
    if (VOS_TRUE == g_stVcStateMgmt.ucPcVoiceSupportFlag)
    {
        g_stVcStateMgmt.enCurrDevMode = VC_PHY_DEVICE_MODE_PCVOICE;
    }
    g_stVcStateMgmt.ucVoicePort = APP_VC_VOICE_PORT_BUTT;

    ulRlst = APP_VC_GetCurVolume(&sCurVolume);

    if (VOS_OK == ulRlst)
    {
        g_stVcStateMgmt.sCurrVolume = sCurVolume;
    }
    else
    {
        g_stVcStateMgmt.sCurrVolume = VC_VOLUME_DEFAULT_VALUE;
    }

    ulRlst = APP_VC_GetVoiceTestFlag(&ulVoiceTestFlagValue);

    if (VOS_OK == ulRlst)
    {
        g_stVcStateMgmt.ulVoiceTestFlag = ulVoiceTestFlagValue;
    }
    else
    {
        g_stVcStateMgmt.ulVoiceTestFlag = APP_VC_VOICE_TEST_FLAG_DEFALUT;
    }

    g_stVcStateMgmt.enCurrMuteStatus = APP_VC_MUTE_STATUS_UNMUTED;
    return;
}
VOS_VOID  APP_VC_StartProtectTimer(APP_VC_TIMER_ID_ENUM_U16 enTimerName)
{
    /* ����VOS��ʱ�� */
    if (VOS_OK != VOS_StartRelTimer(&g_stVcStateMgmt.protectTimer,
                                    WUEPS_PID_VC,
                                    1000,
                                    enTimerName,
                                    0,
                                    VOS_RELTIMER_NOLOOP,
                                    VOS_TIMER_PRECISION_5))
    {
        VC_ERR_LOG("VOS_StartRelTimer: failed.");
    }
}


VOS_VOID  APP_VC_StopProtectTimer(VOS_VOID)
{
    /* ����VOS��ʱ�� */
    if (VOS_OK != VOS_StopRelTimer(&g_stVcStateMgmt.protectTimer))
    {
        VC_ERR_LOG("APP_VC_StopProtectTimer: failed.");
    }
}
VOS_UINT32  APP_VC_GetNVIdForGsm(
    VC_PHY_DEVICE_MODE_ENUM_U16         enDevMode
)
{
    switch(enDevMode)
    {
        case VC_PHY_DEVICE_MODE_HANDSET:
            return en_NV_Item_HANDSET_G_PARAM_ID;
        case VC_PHY_DEVICE_MODE_HANDS_FREE:
            return en_NV_Item_HANDHELD_HANDFREE_G_PARAM_ID;
        case VC_PHY_DEVICE_MODE_CAR_FREE:
            return en_NV_Item_VECHILE_HANDFREE_G_PARAM_ID;
        case VC_PHY_DEVICE_MODE_EARPHONE:
            return en_NV_Item_HEADSET_G_PARAM_ID;
        case VC_PHY_DEVICE_MODE_BLUETOOTH:
            return en_NV_Item_BT_G_PARAM_ID;
        case VC_PHY_DEVICE_MODE_PCVOICE:
            return en_NV_Item_PCVOICE_PARAM_ID_W;
        default:
            VC_WARN_LOG1("APP_VC_GetNVIdForGsm: GSM,wrong enDevMode", enDevMode);
            return en_NV_Item_ID_BUTT;
    }
}

VOS_UINT32  APP_VC_GetNVIdForWcdma(
    VC_PHY_DEVICE_MODE_ENUM_U16         enDevMode
)
{
    switch(enDevMode)
    {
        case VC_PHY_DEVICE_MODE_HANDSET:
            return en_NV_Item_HANDSET_W_PARAM_ID;

        case VC_PHY_DEVICE_MODE_HANDS_FREE:
            return en_NV_Item_HANDHELD_HANDFREE_W_PARAM_ID;

        case VC_PHY_DEVICE_MODE_CAR_FREE:
            return en_NV_Item_VECHILE_HANDFREE_W_PARAM_ID;

        case VC_PHY_DEVICE_MODE_EARPHONE:
            return en_NV_Item_HEADSET_W_PARAM_ID;

        case VC_PHY_DEVICE_MODE_BLUETOOTH:
            return en_NV_Item_BT_W_PARAM_ID;

        case VC_PHY_DEVICE_MODE_PCVOICE:
            return en_NV_Item_PCVOICE_PARAM_ID_W;

        default:
            VC_WARN_LOG1("APP_VC_GetNVIdForWcdma: WCDMA,wrong enDevMode", enDevMode);
            return en_NV_Item_ID_BUTT;
    }
}


VOS_UINT32   APP_VC_GetNVId(
    CALL_VC_RADIO_MODE_ENUM_U8          enRadioMode,
    VC_PHY_DEVICE_MODE_ENUM_U16         enDevMode
)
{
    switch(enRadioMode)
    {
        case CALL_VC_MODE_GSM:
            return APP_VC_GetNVIdForGsm(enDevMode);
        case CALL_VC_MODE_WCDMA:
            return APP_VC_GetNVIdForWcdma(enDevMode);
        default:
            VC_WARN_LOG1("APP_VC_GetNVId: wrong enRadioMode", enRadioMode);
            return en_NV_Item_ID_BUTT;
    }
}



VOS_UINT16  APP_VC_GetInDevMode(VC_PHY_DEVICE_MODE_ENUM_U16  enDevMode)
{
    switch(enDevMode)
    {
        case VC_PHY_DEVICE_MODE_HANDSET:
            return APP_VC_IN_DEV_HANDSET_INTERNAL_MIC;

        case VC_PHY_DEVICE_MODE_HANDS_FREE:
            return APP_VC_IN_DEV_HANDFREE_INTERNAL_MIC;

        case VC_PHY_DEVICE_MODE_EARPHONE:
            return APP_VC_IN_DEV_HEADSET_EXTERNAL_MIC;

        case VC_PHY_DEVICE_MODE_HEADPHONE:
            return APP_VC_IN_DEV_HANDFREE_INTERNAL_MIC;

        case VC_PHY_DEVICE_MODE_SUPER_HANDFREE:
            return APP_VC_IN_DEV_HANDFREE_INTERNAL_MIC;

        case VC_PHY_DEVICE_MODE_SMART_TALK:
            return APP_VC_IN_DEV_HANDFREE_INTERNAL_MIC;

        default:
            VC_WARN_LOG1("APP_VC_GetInDevMode: wrong enDevMode", enDevMode);
            return APP_VC_IN_DEV_BUTT;
    }

}


VOS_UINT16  APP_VC_GetOutDevMode(VC_PHY_DEVICE_MODE_ENUM_U16  enDevMode)
{
    switch(enDevMode)
    {
        case VC_PHY_DEVICE_MODE_HANDSET:
            return APP_VC_OUT_DEV_HANDSET_INTERNAL_MIC;

        case VC_PHY_DEVICE_MODE_HANDS_FREE:
            return APP_VC_OUT_DEV_HANDFREE_INTERNAL_MIC;

        case VC_PHY_DEVICE_MODE_EARPHONE:
            return APP_VC_OUT_DEV_HEADSET_EXTERNAL_MIC;

        case VC_PHY_DEVICE_MODE_HEADPHONE:
            return APP_VC_OUT_DEV_HANDFREE_INTERNAL_MIC;

        case VC_PHY_DEVICE_MODE_SUPER_HANDFREE:
            return APP_VC_OUT_DEV_HANDFREE_INTERNAL_MIC;

        case VC_PHY_DEVICE_MODE_SMART_TALK:
            return APP_VC_OUT_DEV_HANDFREE_INTERNAL_MIC;

        default:
            VC_WARN_LOG1("APP_VC_GetOutDevMode: wrong enDevMode", enDevMode);
            return APP_VC_OUT_DEV_BUTT;
    }

}

#if (FEATURE_ON == FEATURE_IMS)

VC_IMSA_EXCEPTION_CAUSE_ENUM_UINT32  APP_VC_ConvertVcOpenChannelFailCauseToImsaExceptionCause(
    APP_VC_OPEN_CHANNEL_FAIL_CAUSE_ENUM_UINT32              enVcCause
)
{
    switch(enVcCause)
    {
        case APP_VC_OPEN_CHANNEL_CAUSE_SUCC:
            return VC_IMSA_EXCEPTION_CAUSE_SUCC;

        case APP_VC_OPEN_CHANNEL_FAIL_CAUSE_STARTED:
            return VC_IMSA_EXCEPTION_CAUSE_STARTED;

        case APP_VC_OPEN_CHANNEL_FAIL_CAUSE_PORT_CFG_FAIL:
            return VC_IMSA_EXCEPTION_CAUSE_PORT_CFG_FAIL;

        case APP_VC_OPEN_CHANNEL_FAIL_CAUSE_SET_DEVICE_FAIL:
            return VC_IMSA_EXCEPTION_CAUSE_SET_DEVICE_FAIL;

        case APP_VC_OPEN_CHANNEL_FAIL_CAUSE_SET_START_FAIL:
            return VC_IMSA_EXCEPTION_CAUSE_SET_START_FAIL;

        case APP_VC_OPEN_CHANNEL_FAIL_CAUSE_SET_VOLUME_FAIL:
            return VC_IMSA_EXCEPTION_CAUSE_SET_VOLUME_FAIL;

        case APP_VC_OPEN_CHANNEL_FAIL_CAUSE_SAMPLE_RATE_FAIL:
            return VC_IMSA_EXCEPTION_CAUSE_SAMPLE_RATE_FAIL;

        case APP_VC_OPEN_CHANNEL_FAIL_CAUSE_TI_START_EXPIRED:
            return VC_IMSA_EXCEPTION_CAUSE_TI_START_EXPIRED;

        default:
            VC_ERR_LOG("APP_VC_ConvertVcOpenChannelFailCauseToImsaExceptionCause: wrong enVcCause");
            return VC_IMSA_EXCEPTION_CAUSE_BUTT;
    }

}


CALL_VC_CODEC_TYPE_ENUM_U8  APP_VC_ConvertImsaCodeTypeToCallCodeType(
    IMSA_VC_CODEC_TYPE_ENUM_UINT8       enImsaCodeType
)
{
    switch(enImsaCodeType)
    {
        case IMSA_VC_CODEC_TYPE_AMR:
            return CALL_VC_CODEC_TYPE_AMR;

        case IMSA_VC_CODEC_TYPE_AMRWB:
            return CALL_VC_CODEC_TYPE_AMRWB;

        case IMSA_VC_CODEC_TYPE_G711:
            return CALL_VC_CODEC_TYPE_G711;

        default:
            VC_ERR_LOG("APP_VC_ConvertImsaCodeTypeToCallCodeType: wrong enImsaCodeType");
            return CALL_VC_CODEC_TYPE_BUTT;
    }

}

#endif

VOS_UINT32  APP_VC_InputOutPutPortCfg(
    CALL_VC_CODEC_TYPE_ENUM_U8          enCodecType
)
{
    /* TODO: ���͵���ȷ�Ͻӿ� */
    APP_VC_IOCTL_AUDIO_IN_DEV_ENUM_U16  enAudioInDev;
    APP_VC_IOCTL_AUDIO_OUT_DEV_ENUM_U16 enAudioOutDev;
    VOS_UINT8                           ucSampleRate;
    VC_OM_PCVOICE_TRANS_STATUS_STRU    *pstPcVoiceTransStatusMsg;
    VOS_UINT32                          ulRet;
    APP_VC_MS_CFG_INFO_STRU            *pstMsCfgInfo;

    pstMsCfgInfo                         = APP_VC_GetCustomCfgInfo();

    pstPcVoiceTransStatusMsg = VOS_NULL_PTR;



    /* �����ǰ��δ������й����� */
    if (VOS_FALSE == g_stVcStateMgmt.bInCall)
    {
        /*PC Voice����δʹ��*/
        if (VOS_FALSE == g_stVcStateMgmt.ucPcVoiceSupportFlag)
        {
            if (VCVOICE_WORK_TYPE_NORMAL == APP_VC_GetWorkType())
            {
                if (APP_VC_INVALID_DEV_HANDLE == APP_VC_GetDevHandle())
                {
                    /* APP_VC_SendEndCallReq();������ͳһ���ͣ������޸���ͬ */
                    VC_WARN_LOG("APP_VC_CallChannelOpenProc: Open Codec Fail" );
                    return VOS_ERR;
                }

                if (VOS_OK != DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_PCM_MODE_SET, 0))
                {
                    VC_WARN_LOG("APP_VC_CallChannelOpenProc: AUDIO_PCM_MODE_SET Fail" );
                    return VOS_ERR;
                }

                if ( CALL_VC_CODEC_TYPE_AMRWB == enCodecType )
                {
                    ucSampleRate = 3;
                }
                else
                {
                    ucSampleRate = 0;
                }

                if (VOS_OK != DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_SAMPLE_RATE_SET, ucSampleRate))
                {
                    DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_VOICE_CLOSE, 0);
                    VC_WARN_LOG("APP_VC_CallChannelOpenProc: Sample rate failed" );
                    return VOS_ERR;
                }

                if (VOS_OK != DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_VOICE_OPEN, 0))
                {
                    VC_WARN_LOG("APP_VC_CallChannelOpenProc: VOICE_OPEN Fail" );
                    return VOS_ERR;
                }

                /* �����ͳ����豸����Ҫ����ģ��codec */
                if ((VC_PHY_DEVICE_MODE_BLUETOOTH != APP_VC_GetCurrDevMode())
                 && (VC_PHY_DEVICE_MODE_CAR_FREE != APP_VC_GetCurrDevMode()))
                {
                    enAudioInDev = APP_VC_GetInDevMode(APP_VC_GetCurrDevMode());
                    enAudioOutDev = APP_VC_GetOutDevMode(APP_VC_GetCurrDevMode());
                    if (APP_VC_IN_DEV_BUTT == enAudioInDev)
                    {
                        VC_WARN_LOG("APP_VC_CallChannelOpenProc: GET IN_OUT_DEV Fail" );
                        return VOS_ERR;
                    }

                    if (VOS_OK != DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_IN_DEV_SELECT, enAudioInDev))
                    {
                        DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_VOICE_CLOSE, 0);
                        VC_WARN_LOG("APP_VC_CallChannelOpenProc: IN_DEV_SELECT Fail" );
                        return VOS_ERR;
                    }

                    if (VOS_OK != DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_OUT_DEV_SELECT, enAudioOutDev))
                    {
                        DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_VOICE_CLOSE, 0);
                        VC_WARN_LOG("APP_VC_CallChannelOpenProc: OUT_DEV_SELECT Fail" );
                        return VOS_ERR;
                    }
                }
            }
        }
        else
        {
             if (APP_VC_NV_ITEM_ACTIVE == pstMsCfgInfo->usVcPcvoiceSupportFlag)
             {
                  pstPcVoiceTransStatusMsg = (VC_OM_PCVOICE_TRANS_STATUS_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                                             sizeof(VC_OM_PCVOICE_TRANS_STATUS_STRU) - VOS_MSG_HEAD_LENGTH);

                 if (VOS_NULL_PTR == pstPcVoiceTransStatusMsg)
                 {
                    VC_ERR_LOG("in APP_VC_InputOutPutPortCfg:malloc error!");
                    return VOS_ERR;
                 }
                 pstPcVoiceTransStatusMsg->ulReceiverCpuId      = VOS_LOCAL_CPUID;
                 pstPcVoiceTransStatusMsg->ulReceiverPid        = WUEPS_PID_TAF;
                 pstPcVoiceTransStatusMsg->ulSenderCpuId        = VOS_LOCAL_CPUID;
                 pstPcVoiceTransStatusMsg->ulSenderPid          = WUEPS_PID_VC;
                 pstPcVoiceTransStatusMsg->usPcVoiceChannelOper = OM_PCV_CHANNEL_OPEN;
                 pstPcVoiceTransStatusMsg->usPort               = g_stVcStateMgmt.ucVoicePort;
                 pstPcVoiceTransStatusMsg->ulLength             = sizeof(VC_OM_PCVOICE_TRANS_STATUS_STRU) - VOS_MSG_HEAD_LENGTH;

                 /*����OM�ӿ����������˿�*/
                 if (APP_VC_VOICE_PORT_BUTT != APP_VC_GetPcVoicePort())
                 {

                     ulRet = APP_VC_SendVcReqToOm(OM_PCV_CHANNEL_OPEN, APP_VC_GetPcVoicePort());
                     if (VOS_OK != ulRet)
                     {
                         VC_WARN_LOG("APP_VC_CallChannelOpenProc: OM_PCV_CHANNEL_OPEN FAIL!" );
                         NAS_MNTN_FailtoOpenCsChannel(g_stVcStateMgmt.ucVoicePort, ulRet);
                         g_stVcStateMgmt.ucVoicePort              = APP_VC_VOICE_PORT_BUTT;
                         pstPcVoiceTransStatusMsg->usOmOperResult = VOS_ERR;
                         OM_TraceMsgHook(pstPcVoiceTransStatusMsg);
                         VOS_FreeMsg(WUEPS_PID_VC, pstPcVoiceTransStatusMsg);
                         return VOS_ERR;
                     }

                 }
                 pstPcVoiceTransStatusMsg->usOmOperResult = VOS_OK;
                 OM_TraceMsgHook(pstPcVoiceTransStatusMsg);
                 VOS_FreeMsg(WUEPS_PID_VC, pstPcVoiceTransStatusMsg);

             }

        }
    }

    return VOS_OK;
}



VOS_UINT32  APP_VC_CallChannelOpenProc(
    CALL_VC_CHANNEL_INFO_STRU                              *pstChanInfo,
    APP_VC_OPEN_CHANNEL_FAIL_CAUSE_ENUM_UINT32             *penCause
)
{
    VC_DEVICE_PARA_STRU                 stDevPara;
    VOS_INT16                           sCurVolume = 0;

    /* �޸ĵ�:
        1.������ӷ���ֵ
        2.��ʧ�ܵ�ԭ��ֵ��Ϊ�������������
        3.�쳣���ʱ����callģ�鷢EndCallReq�����߸�IMSA��ExceptionNtf��������һ�㷢�� */

    /* �����ǰ�ǲ���ģʽ����ʵ�ʷ��Ͳ���ģʽ����Ϣ������� */
    if (VCVOICE_WORK_TYPE_TEST == APP_VC_GetWorkType())
    {
        APP_VC_SendPhyTestModeNotify(pstChanInfo->stChannelParam.enMode);

        APP_VC_UpdateRadioMode(pstChanInfo->stChannelParam.enMode);

        g_stVcStateMgmt.bInCall = VOS_TRUE;
        *penCause = APP_VC_OPEN_CHANNEL_CAUSE_SUCC;
        return VOS_TRUE;
    }

    if (VOS_OK != APP_VC_InputOutPutPortCfg(pstChanInfo->stChannelParam.enCodecType))
    {
        *penCause = APP_VC_OPEN_CHANNEL_FAIL_CAUSE_PORT_CFG_FAIL;
        return VOS_FALSE;
    }

    APP_VC_UpdateRadioMode(pstChanInfo->stChannelParam.enMode);

    stDevPara.enDeviceMode = APP_VC_GetCurrDevMode();
    if (VOS_OK != APP_VC_SendSetDeviceReq(&stDevPara))
    {
        VC_WARN_LOG("APP_VC_CallChannelOpenProc: APP_VC_SendSetDeviceReq fail" );

        *penCause = APP_VC_OPEN_CHANNEL_FAIL_CAUSE_SET_DEVICE_FAIL;

        return VOS_FALSE;
    }

    if (VOS_OK != APP_VC_SendStartReq(pstChanInfo, APP_VC_GetRadioMode()))
    {
        VC_WARN_LOG("APP_VC_CallChannelOpenProc: APP_VC_SendStartReq fail" );

        *penCause = APP_VC_OPEN_CHANNEL_FAIL_CAUSE_SET_START_FAIL;

        return VOS_FALSE;
    }

    /* �������� */
    if (VOS_OK != APP_VC_GetCurVolume(&sCurVolume))
    {
        VC_WARN_LOG("APP_VC_CallChannelOpenProc: APP_VC_GetCurVolume fail" );
    }

    if (VOS_OK != APP_VC_SendSetVolumeReq(VCVOICE_VOLUME_TARGET_DOWN,
                                          sCurVolume))
    {
        VC_WARN_LOG("APP_VC_CallChannelOpenProc: APP_VC_SendSetVolumeReq fail" );

        *penCause = APP_VC_OPEN_CHANNEL_FAIL_CAUSE_SET_VOLUME_FAIL;

        return VOS_FALSE;
    }

    /* ���ݾ���״̬���ö�Ӧ������ֵ */
    sCurVolume = (APP_VC_MUTE_STATUS_UNMUTED == APP_VC_GetCurrMuteStatus()) ?
                 APP_VC_UNMUTED_VOLUME : APP_VC_MUTED_VOLUME;

    if (VOS_OK != APP_VC_SendSetVolumeReq(VCVOICE_VOLUME_TARGET_UP,
                                          sCurVolume))
    {
        VC_WARN_LOG("APP_VC_CallChannelOpenProc: APP_VC_SendSetVolumeReq(mute) fail" );

        *penCause = APP_VC_OPEN_CHANNEL_FAIL_CAUSE_SET_VOLUME_FAIL;

        return VOS_FALSE;
    }

    *penCause = APP_VC_OPEN_CHANNEL_CAUSE_SUCC;
    APP_VC_StartProtectTimer(APP_VC_TIMER_START);
    APP_VC_UpdateState(APP_VC_S_WAIT_INTERNAL_SET_DEV_RSLT);
    g_stVcStateMgmt.bInCall = VOS_TRUE;

    return VOS_TRUE;
}


VOS_VOID  APP_VC_CallChannelCloseProc(VOS_VOID)
{
    /* TODO: ���͵���ȷ�Ͻӿ� */
    VC_OM_PCVOICE_TRANS_STATUS_STRU     *pstPcVoiceTransStatusMsg;
    VOS_UINT32                           ulRst;
    APP_VC_MS_CFG_INFO_STRU             *pstMsCfgInfo;

    pstMsCfgInfo                         = APP_VC_GetCustomCfgInfo();
    /* �����ǰ�ǲ���ģʽ����ֱ���˳� */
    if (VCVOICE_WORK_TYPE_TEST == APP_VC_GetWorkType())
    {
        g_stVcStateMgmt.bInCall = VOS_FALSE;
        return;
    }

    if (VOS_FALSE == g_stVcStateMgmt.ucPcVoiceSupportFlag)
    {
        if (APP_VC_INVALID_DEV_HANDLE == APP_VC_GetDevHandle())
        {
            VC_ERR_LOG("APP_VC_CallChannelCloseProc: Open Codec Fail");
        }
        else
        {
            if (VOS_OK != DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_VOICE_CLOSE, 0))
            {
                VC_WARN_LOG("APP_VC_CallChannelCloseProc: VOICE Close Fail");
            }
        }
    }
    else
    {
        if (APP_VC_NV_ITEM_ACTIVE == pstMsCfgInfo->usVcPcvoiceSupportFlag)
        {
            pstPcVoiceTransStatusMsg = (VC_OM_PCVOICE_TRANS_STATUS_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_VC,
                                                              sizeof(VC_OM_PCVOICE_TRANS_STATUS_STRU));    /* �����ڴ� */

            if (VOS_NULL_PTR == pstPcVoiceTransStatusMsg)
            {
                VC_ERR_LOG("in APP_VC_CallChannelCloseProc:malloc error!");
                return;
            }
            pstPcVoiceTransStatusMsg->ulReceiverCpuId      = VOS_LOCAL_CPUID;
            pstPcVoiceTransStatusMsg->ulReceiverPid        = WUEPS_PID_TAF;
            pstPcVoiceTransStatusMsg->ulSenderCpuId        = VOS_LOCAL_CPUID;
            pstPcVoiceTransStatusMsg->ulSenderPid          = WUEPS_PID_VC;
            pstPcVoiceTransStatusMsg->usPcVoiceChannelOper = OM_PCV_CHANNEL_CLOSE;
            pstPcVoiceTransStatusMsg->usPort               = g_stVcStateMgmt.ucVoicePort;
            pstPcVoiceTransStatusMsg->ulLength             = sizeof(VC_OM_PCVOICE_TRANS_STATUS_STRU) - VOS_MSG_HEAD_LENGTH;

            if (APP_VC_VOICE_PORT_BUTT != g_stVcStateMgmt.ucVoicePort)
            {
                ulRst = APP_VC_SendVcReqToOm(OM_PCV_CHANNEL_CLOSE, g_stVcStateMgmt.ucVoicePort);
                g_stVcStateMgmt.ucVoicePort = APP_VC_VOICE_PORT_BUTT;
                g_stVcStateMgmt.bInCall = VOS_FALSE;
                VC_INFO_LOG1("PC VOICE TEST:APPP_VC_HpaMsgProc: OM_PCV_CHANEL_CLOSE.", g_stVcStateMgmt.ucVoicePort);
                pstPcVoiceTransStatusMsg->usOmOperResult = ulRst;
                OM_TraceMsgHook(pstPcVoiceTransStatusMsg);
            }
            VOS_FreeMsg(WUEPS_PID_VC, pstPcVoiceTransStatusMsg);

        }

    }

    APP_VC_SendStopReq(APP_VC_GetRadioMode());
    APP_VC_StartProtectTimer(APP_VC_TIMER_STOP);
    APP_VC_UpdateState(APP_VC_S_WAIT_INTERNAL_SET_CLOSE_RSLT);
    g_stVcStateMgmt.bInCall = VOS_FALSE;
}
VOS_VOID  APP_VC_CallChannelParaChangeProc(CALL_VC_CHANNEL_INFO_STRU *pstChanInfo)
{
    VOS_UINT8                           ucSampleRate;
    CALL_VC_RADIO_MODE_ENUM_U8          enCurrMode;

    enCurrMode = APP_VC_GetRadioMode();

    /* �����ǰ�ǲ���ģʽ����ϵͳģʽ�����������ʵ�ʷ��Ͳ���ģʽ����Ϣ������� */

    if ( VCVOICE_WORK_TYPE_TEST == APP_VC_GetWorkType())
    {
        if (enCurrMode != pstChanInfo->stChannelParam.enMode)
        {
            APP_VC_SendPhyTestModeNotify(pstChanInfo->stChannelParam.enMode);
            APP_VC_UpdateRadioMode(pstChanInfo->stChannelParam.enMode);
        }
        return;
    }


    APP_VC_UpdateRadioMode(pstChanInfo->stChannelParam.enMode);
    if ( CALL_VC_CODEC_TYPE_AMRWB == pstChanInfo->stChannelParam.enCodecType )
    {
        ucSampleRate = 3;
    }
    else
    {
        ucSampleRate = 0;
    }

    if ( VOS_FALSE == APP_VC_GetPcVoiceSupportFlg() )
    {
        if (VOS_OK != DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_SAMPLE_RATE_SET, ucSampleRate))
        {
            VC_WARN_LOG("APP_VC_CallChannelOpenProc: Sample rate failed" );

#if (FEATURE_ON == FEATURE_IMS)
            if (APP_VC_START_HIFI_ORIG_IMSA == APP_VC_GetStartHifiOrig())
            {
                /* �����IMSA�����������󣬸�IMSA�ظ��쳣 */
                APP_VC_SendImsaExceptionNtf(VC_IMSA_EXCEPTION_CAUSE_SAMPLE_RATE_FAIL);
            }
            else
#endif
            {
                /*�����CALL�����������󣬷�����Ϣ��CALLģ�飬�Ҷϵ绰*/
                APP_VC_SendEndCallReq();
            }

            /* ����ȫ�ֱ���enStartHifiOrig */
            g_stVcStateMgmt.enStartHifiOrig = APP_VC_START_HIFI_ORIG_BUTT;

            return ;
        }
    }


    /* ����ȫ�ֱ���enStartHifiOrig */
    g_stVcStateMgmt.enStartHifiOrig = APP_VC_START_HIFI_ORIG_BUTT;

    APP_VC_SendSetCodecReq(pstChanInfo);
    APP_VC_StartProtectTimer(APP_VC_TIMER_SET_CODEC);
    APP_VC_UpdateState(APP_VC_S_WAIT_INTERNAL_SET_CODEC_RSLT);
}
VOS_VOID  APP_VC_BufferInternalMsg(CALL_VC_CHANNEL_INFO_MSG_STRU *pstChanInfo)
{
    /* ����ڿ���������ʱ�򣬳��ֲ����ı䣬�˴������ǻ��洦�����ڱ���
       ��������: ����������ʱ�򣬷������л�����ʱ���͵Ĵ��������������
       �����壬ʹ�õ�ǰ�ĳ�ʱ��������ʱ�������ʱ���µĲ�������Ӧ���Ѿ�
       �л�������һ��ģ����ô�ٴη��ͣ����û���µĲ�������Ϊ������쳣��
       �Ҷϵ绰
    */
    if ((APP_VC_S_WAIT_INTERNAL_SET_START_RSLT == APP_VC_GetState())
     && (CALL_VC_CHANNEL_PARA_CHANGE == pstChanInfo->enMsgName))
    {
        g_stVcStateMgmt.bParaUpdate = VOS_TRUE;
        g_stVcStateMgmt.enCodecType = pstChanInfo->stChannelInfo.stChannelParam.enCodecType;
        g_stVcStateMgmt.enRadioMode = pstChanInfo->stChannelInfo.stChannelParam.enMode;
        return;
    }

    g_stVcStateMgmt.astBufMsg[g_stVcStateMgmt.ulBuffLen].enMsgOrigin = APP_VC_MSG_ORIGIN_INTERNAL;
    PS_MEM_CPY(g_stVcStateMgmt.astBufMsg[g_stVcStateMgmt.ulBuffLen].aucBufMsg,
                pstChanInfo,
                sizeof(CALL_VC_CHANNEL_INFO_MSG_STRU));
    g_stVcStateMgmt.ulBuffLen++;
}

#if (FEATURE_ON == FEATURE_IMS)

VOS_VOID  APP_VC_BufferImsaMsg(VOS_VOID *pMsg)
{
    MSG_HEADER_STRU                    *pMsgHeader = VOS_NULL_PTR;
    IMSA_VC_HIFI_PARA_CHANGED_NTF_STRU *pstParaChg = VOS_NULL_PTR;
    CALL_VC_CODEC_TYPE_ENUM_U8          enCodecType;

    /* ����Ϣ���зַ� */
    pMsgHeader  = (MSG_HEADER_STRU *)pMsg;
    pstParaChg  = (IMSA_VC_HIFI_PARA_CHANGED_NTF_STRU *)pMsg;
    enCodecType = APP_VC_ConvertImsaCodeTypeToCallCodeType(pstParaChg->enCodeType);

    /* ����ڿ���������ʱ�򣬳��ֲ����ı䣬�˴������ǻ��洦�����ڱ���
       ��������: ����������ʱ�򣬷������л�����ʱ���͵Ĵ��������������
       �����壬ʹ�õ�ǰ�ĳ�ʱ��������ʱ�������ʱ���µĲ�������Ӧ���Ѿ�
       �л�������һ��ģ����ô�ٴη��ͣ����û���µĲ�������Ϊ������쳣��
       �Ҷϵ绰
    */
    if ((APP_VC_S_WAIT_INTERNAL_SET_START_RSLT == APP_VC_GetState())
     && (ID_IMSA_VC_HIFI_PARA_CHANGED_NTF == pMsgHeader->ulMsgName))
    {
        g_stVcStateMgmt.bParaUpdate = VOS_TRUE;
        g_stVcStateMgmt.enCodecType = enCodecType;
        g_stVcStateMgmt.enRadioMode = CALL_VC_MODE_IMS_EUTRAN;
        return;
    }

    g_stVcStateMgmt.astBufMsg[g_stVcStateMgmt.ulBuffLen].enMsgOrigin = APP_VC_MSG_ORIGIN_IMSA;
    PS_MEM_CPY(g_stVcStateMgmt.astBufMsg[g_stVcStateMgmt.ulBuffLen].aucBufMsg,
                pMsg,
                (pMsgHeader->ulLength + VOS_MSG_HEAD_LENGTH));
    g_stVcStateMgmt.ulBuffLen++;
}
#endif


VOS_VOID  APP_VC_CallChannelInfoProc(CALL_VC_CHANNEL_INFO_MSG_STRU *pMsg)
{
    APP_VC_OPEN_CHANNEL_FAIL_CAUSE_ENUM_UINT32              enCause;

    enCause = APP_VC_OPEN_CHANNEL_CAUSE_SUCC;

    /*�����ǰ��״̬����NULL�Ļ�,������Ϣ,����*/
    if (APP_VC_S_NULL != APP_VC_GetState())
    {
        APP_VC_BufferInternalMsg(pMsg);
        return;
    }

    /*Э��ջ�ڲ�ֻ���ϱ�������Ϣ���������������������Ͳ����ı�*/
    switch (pMsg->enMsgName)
    {
        case CALL_VC_CHANNEL_OPEN:
        /*PC�طŵ���ȫ�ֱ���ʹ��*/
#ifndef __PS_WIN32_RECUR__
            NAS_VC_SndOutsideContextData();
#endif
            /* ���HIFI�Ѿ�����������Ҫ��������ֱ�ӷ��� */
            if (VOS_TRUE == APP_VC_GetCallStatus())
            {
                APP_VC_SendEndCallReq();
                APP_VC_LogEndCallReq(APP_VC_OPEN_CHANNEL_FAIL_CAUSE_STARTED);
                return;
            }

            /* ����ȫ�ֱ�������¼��CALLҪ����HIFI */
            g_stVcStateMgmt.enStartHifiOrig = APP_VC_START_HIFI_ORIG_CALL;

            if (VOS_FALSE == APP_VC_CallChannelOpenProc(&(pMsg->stChannelInfo), &enCause))
            {

                /* ��������ŵ�ʱ�쳣����CALL����EndCallReq��Ϣ��
                    ͬʱ����ȫ�ֱ���enStartHifiOrig */
                APP_VC_SendEndCallReq();

                /* ������ά�ɲ���Ϣ */
                APP_VC_LogEndCallReq(enCause);

                g_stVcStateMgmt.enStartHifiOrig = APP_VC_START_HIFI_ORIG_BUTT;
            }
            break;

        case CALL_VC_CHANNEL_CLOSE:
            APP_VC_CallChannelCloseProc();
            break;

        case CALL_VC_CHANNEL_PARA_CHANGE:

            /* ���ŵ��������ʱ��������ʲ���ʧ�ܣ���Ҫ����ͨ����
                �˴�Ҫ����ȫ�ֱ�������¼��CALL����IMSA������֪ͨ��Ӧ��ģ�����ͨ�� */
            g_stVcStateMgmt.enStartHifiOrig = APP_VC_START_HIFI_ORIG_CALL;
            APP_VC_CallChannelParaChangeProc(&(pMsg->stChannelInfo));
            break;

        default:
            VC_WARN_LOG("APP_VC_CallChannelInfoProc: Msg type Err");
            break;
    }
}


VOS_VOID  APP_VC_BufferAtSetMsg(APP_VC_REQ_MSG_STRU *pstAppMsg)
{
    g_stVcStateMgmt.astBufMsg[g_stVcStateMgmt.ulBuffLen].enMsgOrigin = APP_VC_MSG_ORIGIN_AT;
    PS_MEM_CPY(g_stVcStateMgmt.astBufMsg[g_stVcStateMgmt.ulBuffLen].aucBufMsg,
                pstAppMsg,
                sizeof(APP_VC_REQ_MSG_STRU));

    g_stVcStateMgmt.ulBuffLen++;
}


VOS_VOID  APP_VC_AppSetVolumeProc(
    APP_VC_REQ_MSG_STRU                 *pstAppMsg
)
{
    APP_VC_UpdateClientId(pstAppMsg->clientId);
    APP_VC_UpdateOpId(pstAppMsg->opId);

    /* �����ֵ��80����ΪAT�Ķ��巶ΧΪ0~92,���������֮�䶨��ķ�Χ
       Ϊ-80~12*/
    if (VOS_OK != APP_VC_SendSetVolumeReq(VCVOICE_VOLUME_TARGET_DOWN,
                                          pstAppMsg->aucContent[0] - 80))
    {
        /*�ϱ�*/
        APP_VC_ReportEvent(APP_VC_EVT_SET_VOLUME, MN_ERR_UNSPECIFIED);
        return;
    }

    APP_VC_UpdatePreVolume(APP_VC_GetCurrVolume());
    APP_VC_UpdateCurrVolume((VOS_INT16)(pstAppMsg->aucContent[0] - 80));

    APP_VC_UpdateState(APP_VC_S_WAIT_AT_SET_VOLUME_RSLT);
    APP_VC_StartProtectTimer(APP_VC_TIMER_SET_VOLUME);
}



VOS_VOID  APP_VC_AppQryVolumeProc(
    APP_VC_REQ_MSG_STRU                 *pstAppMsg
)
{
    VOS_UINT32                          ulRslt;
    VOS_INT16                           sCurVolume;

    /* ˢ�¿ͻ�����Ϣ */
    APP_VC_UpdateClientId(pstAppMsg->clientId);
    APP_VC_UpdateOpId(pstAppMsg->opId);

    ulRslt = APP_VC_GetCurVolume(&sCurVolume);

    if (VOS_OK == ulRslt)
    {
        APP_VC_ReportEvent(APP_VC_EVT_GET_VOLUME, MN_ERR_NO_ERROR);
    }
    else
    {
        APP_VC_ReportEvent(APP_VC_EVT_GET_VOLUME, MN_ERR_UNSPECIFIED);
    }
}

VOS_VOID  APP_VC_AppSetModeProc(
    APP_VC_REQ_MSG_STRU                 *pstAppMsg
)
{
    VC_DEVICE_PARA_STRU                 stDevPara;
    APP_VC_IOCTL_AUDIO_IN_DEV_ENUM_U16  enAudioInDev;
    APP_VC_IOCTL_AUDIO_OUT_DEV_ENUM_U16 enAudioOutDev;
    VC_PHY_DEVICE_MODE_ENUM_U16         usDeviceMode;
    VOS_INT16                           sCurVolume = 0;

    APP_VC_UpdateClientId(pstAppMsg->clientId);
    APP_VC_UpdateOpId(pstAppMsg->opId);
    if (VOS_FALSE == g_stVcStateMgmt.ucPcVoiceSupportFlag)
    {
        if (APP_VC_INVALID_DEV_HANDLE == APP_VC_GetDevHandle())
        {
            /*�ϱ�����ʧ��*/
            APP_VC_ReportEvent(APP_VC_EVT_SET_VOICE_MODE, MN_ERR_UNSPECIFIED);
            return;
        }

        /* �����ͳ����豸����Ҫ����ģ��codec */
        if ((VC_PHY_DEVICE_MODE_BLUETOOTH != APP_VC_GetCurrDevMode())
         && (VC_PHY_DEVICE_MODE_CAR_FREE != APP_VC_GetCurrDevMode()))
        {
            enAudioInDev = APP_VC_GetInDevMode(APP_VC_GetCurrDevMode());
            enAudioOutDev = APP_VC_GetOutDevMode(APP_VC_GetCurrDevMode());
            if (APP_VC_IN_DEV_BUTT == enAudioInDev)
            {
                /*�ϱ�����ʧ��*/
                APP_VC_ReportEvent(APP_VC_EVT_SET_VOICE_MODE, MN_ERR_UNSPECIFIED);
                return;
            }

            if (VOS_OK != DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_IN_DEV_UNSELECT, enAudioInDev))
            {
                /*�ϱ�����ʧ��*/
                APP_VC_ReportEvent(APP_VC_EVT_SET_VOICE_MODE, MN_ERR_UNSPECIFIED);
                return;
            }

            if (VOS_OK != DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_OUT_DEV_UNSELECT, enAudioOutDev))
            {
                /*�ϱ�����ʧ��*/
                APP_VC_ReportEvent(APP_VC_EVT_SET_VOICE_MODE, MN_ERR_UNSPECIFIED);
                return;
            }
        }

        /* �����ͳ����豸����Ҫ����ģ��codec */
        if ((VC_PHY_DEVICE_MODE_BLUETOOTH != pstAppMsg->aucContent[0])
         && (VC_PHY_DEVICE_MODE_CAR_FREE != pstAppMsg->aucContent[0]))
        {
            enAudioInDev = APP_VC_GetInDevMode(pstAppMsg->aucContent[0]);
            enAudioOutDev = APP_VC_GetOutDevMode(pstAppMsg->aucContent[0]);
            if (APP_VC_IN_DEV_BUTT == enAudioInDev)
            {
                /*�ϱ�����ʧ��*/
                APP_VC_ReportEvent(APP_VC_EVT_SET_VOICE_MODE, MN_ERR_UNSPECIFIED);
                return;
            }

            if (VOS_OK != DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_IN_DEV_SELECT, enAudioInDev))
            {
                /*�ϱ�����ʧ��*/
                APP_VC_ReportEvent(APP_VC_EVT_SET_VOICE_MODE, MN_ERR_UNSPECIFIED);
                return;
            }

            if (VOS_OK != DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_OUT_DEV_SELECT, enAudioOutDev))
            {
                /*�ϱ�����ʧ��*/
                APP_VC_ReportEvent(APP_VC_EVT_SET_VOICE_MODE, MN_ERR_UNSPECIFIED);
                return;
            }
        }

    }

    usDeviceMode = pstAppMsg->aucContent[0];

    APP_VC_UpdatePreDevMode(APP_VC_GetCurrDevMode());
    APP_VC_UpdateCurrDevMode(usDeviceMode);

    stDevPara.enDeviceMode = APP_VC_GetCurrDevMode();
    if (VOS_OK != APP_VC_SendSetDeviceReq(&stDevPara))
    {
        /*�ϱ�����ʧ��*/
        APP_VC_ReportEvent(APP_VC_EVT_SET_VOICE_MODE, MN_ERR_UNSPECIFIED);
        APP_VC_UpdateCurrDevMode(APP_VC_GetPreDevMode());
        return;
    }

    if (VOS_OK != APP_VC_GetCurVolume(&sCurVolume))
    {
        VC_ERR_LOG("APP_VC_AppSetModeProc:WARNING:APP_VC_GetCurVolume");
    }

    if (VOS_OK != APP_VC_SendSetVolumeReq(VCVOICE_VOLUME_TARGET_DOWN,
                                          sCurVolume))
    {
        /*�ϱ�����ʧ��*/
        APP_VC_ReportEvent(APP_VC_EVT_SET_VOICE_MODE, MN_ERR_UNSPECIFIED);
        return;
    }
    APP_VC_UpdateState(APP_VC_S_WAIT_AT_SET_DEV_RSLT);
    APP_VC_StartProtectTimer(APP_VC_TIMER_SET_DEV);
}
VOS_VOID  APP_VC_AppQryModeReqProc(
    APP_VC_REQ_MSG_STRU                *pstAppMsg
)
{
    APP_VC_REQ_MSG_STRU                *pstAppReqMsg;                           /* ������Ϣָ�� */
    MN_AT_IND_EVT_STRU                 *pstMsg;                                 /* ������Ϣָ�� */
    APP_VC_EVENT_INFO_STRU              stAppCnfMsg;
    VOS_UINT32                          ulTmpAddr;
    VOS_UINT16                          usLength;

    /* ��ʼ�� */
    pstAppReqMsg    = pstAppMsg;
    pstMsg          = VOS_NULL_PTR;
    usLength        = 0;

    VOS_MemSet(&stAppCnfMsg, 0x00, sizeof(APP_VC_EVENT_INFO_STRU));

    /* ��ѯ��ǰ�����豸ģʽ */
    stAppCnfMsg.enDevMode               = APP_VC_GetCurrDevMode();

    /* Ϊ��Ϣ������ڴ� */
    usLength        = sizeof(MN_AT_IND_EVT_STRU) + sizeof(APP_VC_EVENT_INFO_STRU) - VOS_MSG_HEAD_LENGTH - 4;
    pstMsg          = (MN_AT_IND_EVT_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC, usLength);
    if (VOS_NULL_PTR == pstMsg)
    {
        VC_ERR_LOG("APP_VC_AppQryModeReqProc:ERROR:Alloc Msg");
        return;
    }

    /* ��װ��Ϣͷ����Ϣ�� */
    pstMsg->ulReceiverCpuId             = VOS_LOCAL_CPUID;
    pstMsg->ulSenderPid                 = WUEPS_PID_VC;
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;

    pstMsg->usMsgName                   = APP_VC_MSG_CNF_QRY_MODE;
    pstMsg->aucReserved1[0]             = 0;
    pstMsg->aucReserved1[1]             = 0;
    pstMsg->clientId                    = MN_GetRealClientId(pstAppReqMsg->clientId, WUEPS_PID_VC);
    pstMsg->usLen                       = usLength;

    /* ��д����Ϣ���� */
    ulTmpAddr  = (VOS_UINT32)pstMsg->aucContent;
    PS_MEM_CPY((VOS_VOID *)ulTmpAddr, &stAppCnfMsg, sizeof(APP_VC_EVENT_INFO_STRU));

    /*������Ϣ��AT_PID;*/
    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_VC, pstMsg))
    {
        VC_ERR_LOG("APP_VC_AppQryModeProc:ERROR");
        VOS_FreeMsg(WUEPS_PID_VC, pstMsg);
        return;
    }

    return;
}


VOS_VOID  APP_VC_AppSetPortReqProc(
    APP_VC_REQ_MSG_STRU                *pstAppMsg
)
{
    APP_VC_REQ_MSG_STRU                *pstAppReqMsg;                           /* ������Ϣָ�� */
    MN_AT_IND_EVT_STRU                 *pstMsg;                                 /* ������Ϣָ�� */
    APP_VC_EVENT_INFO_STRU              stAppCnfMsg;
    APP_VC_VOICE_PORT_ENUM_U8           ucVoicePort;
    VOS_UINT32                          ulTmpAddr;
    VOS_UINT16                          usLength;
    VOS_UINT32                          ulStatus;

    /* ��ʼ�� */
    pstAppReqMsg    = pstAppMsg;
    pstMsg          = VOS_NULL_PTR;
    usLength        = 0;
    ulStatus        = OM_PCV_CHANNEL_BUTT;
    VOS_MemSet(&stAppCnfMsg, 0x00, sizeof(APP_VC_EVENT_INFO_STRU));

    /* ��AT��������Ϣ�У���ȡҪ���õ������豸�˿ں� */
    PS_MEM_CPY(&ucVoicePort, pstAppReqMsg->aucContent, sizeof(APP_VC_VOICE_PORT_ENUM_U8));

    /* Ϊ��Ϣ������ڴ� */
    usLength        = sizeof(MN_AT_IND_EVT_STRU) + sizeof(APP_VC_EVENT_INFO_STRU) - VOS_MSG_HEAD_LENGTH - 4;
    pstMsg          = (MN_AT_IND_EVT_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC, usLength);
    if (VOS_NULL_PTR == pstMsg)
    {
        VC_ERR_LOG("APP_VC_AppQryModeReqProc:ERROR:Alloc Msg");
        return;
    }

    if (APP_VC_VOICE_PORT_BUTT == g_stVcStateMgmt.ucVoicePort)
    {
        /* ��һ�����������˿ڣ�״̬Ϊ���� */
        ulStatus = OM_PCV_CHANNEL_OPEN;
    }
    else if (ucVoicePort == g_stVcStateMgmt.ucVoicePort)
    {
        /* ���õ����Զ˿��뵱ǰ�������˿���ͬ��ֱ�ӷ��سɹ� */
        stAppCnfMsg.bSuccess        = VOS_TRUE;
    }
    else
    {
        /* �����������˿ڣ�״̬Ϊ�ı� */
        ulStatus = OM_PCV_CHANNEL_SWITCH;
    }

    /*����OM�ӿ����������˿�*/
    if (VOS_TRUE == g_stVcStateMgmt.bInCall)
    {
        if (TAF_SUCCESS == APP_VC_SendVcReqToOm(ulStatus, ucVoicePort))
        {
            g_stVcStateMgmt.ucVoicePort = ucVoicePort;
            stAppCnfMsg.bSuccess        = VOS_TRUE;
        }
        else
        {
            stAppCnfMsg.bSuccess        = VOS_FALSE;
        }
    }
    else
    {
        g_stVcStateMgmt.ucVoicePort = ucVoicePort;
        stAppCnfMsg.bSuccess        = VOS_TRUE;
    }

    /* ��װ��Ϣͷ����Ϣ�� */
    pstMsg->ulReceiverCpuId             = VOS_LOCAL_CPUID;
    pstMsg->ulSenderPid                 = WUEPS_PID_VC;
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;

    pstMsg->usMsgName                   = APP_VC_MSG_CNF_SET_PORT;
    pstMsg->aucReserved1[0]             = 0;
    pstMsg->aucReserved1[1]             = 0;
    pstMsg->clientId                    = MN_GetRealClientId(pstAppReqMsg->clientId, WUEPS_PID_VC);
    pstMsg->usLen                       = usLength;

    /* ��д����Ϣ���� */
    ulTmpAddr  = (VOS_UINT32)pstMsg->aucContent;
    PS_MEM_CPY((VOS_VOID *)ulTmpAddr, &stAppCnfMsg, sizeof(APP_VC_EVENT_INFO_STRU));

    /*������Ϣ��AT_PID;*/
    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_VC, pstMsg))
    {
        VC_ERR_LOG("APP_VC_AppQryModeProc:ERROR");
        VOS_FreeMsg(WUEPS_PID_VC, pstMsg);
        return;
    }

    return;
}


VOS_VOID  APP_VC_AppQryPortReqProc(
    APP_VC_REQ_MSG_STRU                *pstAppMsg
)
{
    APP_VC_REQ_MSG_STRU                *pstAppReqMsg;                           /* ������Ϣָ�� */
    MN_AT_IND_EVT_STRU                 *pstMsg;                                 /* ������Ϣָ�� */
    APP_VC_EVENT_INFO_STRU              stAppCnfMsg;
    VOS_UINT32                          ulTmpAddr;
    VOS_UINT16                          usLength;

    /* ��ʼ�� */
    pstAppReqMsg    = pstAppMsg;
    pstMsg          = VOS_NULL_PTR;
    usLength        = 0;
    VOS_MemSet(&stAppCnfMsg, 0x00, sizeof(APP_VC_EVENT_INFO_STRU));

    /* �Ȼ�ȡ��ǰ�����豸�˿�,�ŵ��ط���AT����Ϣ��*/
    stAppCnfMsg.enVoicePort             = APP_VC_GetPcVoicePort();

    /* Ϊ��Ϣ������ڴ� */
    usLength        = sizeof(MN_AT_IND_EVT_STRU) + sizeof(APP_VC_EVENT_INFO_STRU) - VOS_MSG_HEAD_LENGTH - 4;
    pstMsg          = (MN_AT_IND_EVT_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC, usLength);
    if (VOS_NULL_PTR == pstMsg)
    {
        VC_ERR_LOG("APP_VC_AppQryModeReqProc:ERROR:Alloc Msg");
        return;
    }

    /* ��װ��Ϣͷ����Ϣ�� */
    pstMsg->ulReceiverCpuId             = VOS_LOCAL_CPUID;
    pstMsg->ulSenderPid                 = WUEPS_PID_VC;
    pstMsg->ulReceiverPid               = WUEPS_PID_AT;

    pstMsg->usMsgName                   = APP_VC_MSG_CNF_QRY_PORT;
    pstMsg->aucReserved1[0]             = 0;
    pstMsg->aucReserved1[1]             = 0;
    pstMsg->clientId                    = MN_GetRealClientId(pstAppReqMsg->clientId, WUEPS_PID_VC);
    pstMsg->usLen                       = usLength;

    /* ��д����Ϣ���� */
    ulTmpAddr  = (VOS_UINT32)pstMsg->aucContent;
    PS_MEM_CPY((VOS_VOID *)ulTmpAddr, &stAppCnfMsg, sizeof(APP_VC_EVENT_INFO_STRU));

    /*������Ϣ��AT_PID;*/
    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_VC, pstMsg))
    {
        VC_ERR_LOG("APP_VC_AppQryModeProc:ERROR");
        VOS_FreeMsg(WUEPS_PID_VC, pstMsg);
        return;
    }

    return;
}


VOS_VOID APP_VC_AppSetMuteStatusProc(
    APP_VC_REQ_MSG_STRU                *pstAppMsg
)
{
    APP_VC_MUTE_STATUS_ENUM_UINT8       enMuteStatus;
    VOS_INT16                           sVolValue;

    enMuteStatus = pstAppMsg->aucContent[0];

    /* ˢ�¿ͻ�����Ϣ */
    APP_VC_UpdateClientId(pstAppMsg->clientId);
    APP_VC_UpdateOpId(pstAppMsg->opId);
    /* ���ݾ���״̬���ö�Ӧ������ֵ */
    sVolValue = (APP_VC_MUTE_STATUS_UNMUTED == enMuteStatus) ?
                APP_VC_UNMUTED_VOLUME : APP_VC_MUTED_VOLUME;

    if (VOS_OK != APP_VC_SendSetVolumeReq(VCVOICE_VOLUME_TARGET_UP,
                                          sVolValue))
    {
        APP_VC_ReportEvent(APP_VC_EVT_SET_MUTE_STATUS, MN_ERR_UNSPECIFIED);
        return;
    }

    APP_VC_UpdataPreMuteStatus(APP_VC_GetCurrMuteStatus());
    APP_VC_UpdataCurrMuteStatus(enMuteStatus);
    APP_VC_UpdateState(APP_VC_S_WAIT_AT_SET_MUTE_STATUS_RSLT);
    APP_VC_StartProtectTimer(APP_VC_TIMER_SET_VOLUME);

    return;
}


VOS_VOID  APP_VC_AppGetMuteStatusProc(
    APP_VC_REQ_MSG_STRU                *pstAppMsg
)
{
    /* ˢ�¿ͻ�����Ϣ */
    APP_VC_UpdateClientId(pstAppMsg->clientId);
    APP_VC_UpdateOpId(pstAppMsg->opId);

    APP_VC_ReportEvent(APP_VC_EVT_GET_MUTE_STATUS, MN_ERR_NO_ERROR);

    return;
}
VOS_VOID  APP_VC_AppSetForeGroundProc(
    APP_VC_REQ_MSG_STRU                *pstAppMsg
)
{
    APP_VC_UpdateClientId(pstAppMsg->clientId);
    APP_VC_UpdateOpId(pstAppMsg->opId);

    /* �����ڵ绰�����·�CODEC ����������Ϣ */
    if ( VOS_TRUE == APP_VC_GetCallStatus() )
    {
        if (VOS_OK == APP_VC_SendSetForeGroundReq())
        {
            /* ����������ʱ�����ȴ�CODEC�ظ���Ϣ */
            APP_VC_StartProtectTimer(APP_VC_TIMER_SET_FOREGROUND);
        }
        else
        {
            VC_ERR_LOG("APP_VC_AppSetForeGroundProc: SEND FOREGROUND REQ MSG FAILED.");

            APP_VC_SendSetForeGroundCnf( g_stVcStateMgmt.clientId, VCVOICE_EXECUTE_RSLT_FAIL);
        }
    }
    /* �������ڵ绰����ֱ�ӻظ�AT ǰ̨ģʽ����ʧ�� */
    else
    {
        VC_ERR_LOG("APP_VC_AppSetForeGroundProc: NO CALL EXIST.");

        APP_VC_SendSetForeGroundCnf( g_stVcStateMgmt.clientId, VCVOICE_EXECUTE_RSLT_FAIL);
    }
}
VOS_VOID  APP_VC_AppSetBackGroundProc(
    APP_VC_REQ_MSG_STRU                *pstAppMsg
)
{
    APP_VC_UpdateClientId(pstAppMsg->clientId);
    APP_VC_UpdateOpId(pstAppMsg->opId);

    /* VC �жϵ�ǰ�Ƿ��е绰���� */
    if ( VOS_TRUE == APP_VC_GetCallStatus() )
    {
        if ( VOS_OK == APP_VC_SendSetBackGroundReq() )
        {
            /* ����������ʱ�����ȴ�CODEC�ظ���Ϣ */
            APP_VC_StartProtectTimer(APP_VC_TIMER_SET_BACKGROUND);
        }
        else
        {
            VC_ERR_LOG("APP_VC_AppSetBackGroundProc: SEND BACKGROUND REQ MSG FAILED.");

            APP_VC_SendSetBackGroundCnf(g_stVcStateMgmt.clientId, VCVOICE_EXECUTE_RSLT_FAIL);
        }
    }
    else
    {
        VC_ERR_LOG("APP_VC_AppSetBackGroundProc: NO CALL EXIST.");

        APP_VC_SendSetBackGroundCnf(g_stVcStateMgmt.clientId, VCVOICE_EXECUTE_RSLT_FAIL);
    }
}


VOS_VOID  APP_VC_AppQryGroundProc(
    APP_VC_REQ_MSG_STRU                *pstAppMsg
)
{
    APP_VC_UpdateClientId(pstAppMsg->clientId);
    APP_VC_UpdateOpId(pstAppMsg->opId);

    /* VC �жϵ�ǰ�Ƿ��е绰���� */
    if ( VOS_TRUE == APP_VC_GetCallStatus() )
    {
        if ( VOS_OK == APP_VC_SendGroundQry() )
        {
            /* ����������ʱ�����ȴ�CODEC�ظ���Ϣ */
            APP_VC_StartProtectTimer(APP_VC_TIMER_QRY_GROUND);
        }
        else
        {
            VC_ERR_LOG("APP_VC_AppQryGroundProc: SEND GROUND QRY MSG FAILED.");

            APP_VC_SendGroundRsp(g_stVcStateMgmt.clientId, VCVOICE_GROUND_BUTT, VOS_ERR);
        }
    }
    else
    {
        VC_ERR_LOG("APP_VC_AppQryGroundProc: NO CALL EXIST.");

        APP_VC_SendGroundRsp(g_stVcStateMgmt.clientId, VCVOICE_GROUND_BUTT, VOS_ERR);
    }
}


VOS_VOID  APP_VC_AppSetModemLoopProc(
    APP_VC_REQ_MSG_STRU                *pstAppMsg
)
{
    VCVOICE_LOOP_ENUM_UINT16            enModemLoop;
    APP_VC_IOCTL_AUDIO_IN_DEV_ENUM_U16  enAudioInDev;
    APP_VC_IOCTL_AUDIO_OUT_DEV_ENUM_U16 enAudioOutDev;

    /* ��ȡ�����������ص�״̬:�˳�������������� */
    enModemLoop = (VCVOICE_LOOP_ENUM_UINT16)pstAppMsg->aucContent[0];

    if (VCVOICE_WORK_TYPE_NORMAL == APP_VC_GetWorkType())
    {
        if (VCVOICE_LOOP_START == enModemLoop)
        {
            if (APP_VC_INVALID_DEV_HANDLE != APP_VC_GetDevHandle())
            {
                if (VOS_OK != DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_PCM_MODE_SET, 0))
                {
                    VC_WARN_LOG("APP_VC_AppSetModemLoopProc: set PCM mode Fail" );
                }

                if (VOS_OK != DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_VOICE_OPEN, 0))
                {
                    VC_WARN_LOG("APP_VC_AppSetModemLoopProc: Open codec Fail" );
                }

                if ((VC_PHY_DEVICE_MODE_BLUETOOTH != APP_VC_GetCurrDevMode())
                 && (VC_PHY_DEVICE_MODE_CAR_FREE != APP_VC_GetCurrDevMode()))
                {
                    enAudioInDev = APP_VC_GetInDevMode(APP_VC_GetCurrDevMode());
                    enAudioOutDev = APP_VC_GetOutDevMode(APP_VC_GetCurrDevMode());
                    if (APP_VC_IN_DEV_BUTT == enAudioInDev)
                    {
                        VC_WARN_LOG("APP_VC_CallChannelOpenProc: GET IN_OUT_DEV Fail" );
                        enAudioInDev    = APP_VC_IN_DEV_HEADSET_EXTERNAL_MIC;
                        enAudioOutDev   = APP_VC_OUT_DEV_HEADSET_EXTERNAL_MIC;
                    }

                    if (VOS_OK != DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_IN_DEV_SELECT, enAudioInDev))
                    {
                        DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_VOICE_CLOSE, 0);
                        VC_WARN_LOG("APP_VC_CallChannelOpenProc: IN_DEV_SELECT Fail" );
                    }

                    if (VOS_OK != DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_OUT_DEV_SELECT, enAudioOutDev))
                    {
                        DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_VOICE_CLOSE, 0);
                        VC_WARN_LOG("APP_VC_CallChannelOpenProc: OUT_DEV_SELECT Fail" );
                    }
                }
            }
        }
        else
        {
            DRV_CODEC_IOCTL(APP_VC_GetDevHandle(), APP_VC_IOCTL_AUDIO_VOICE_CLOSE, 0);
        }
    }

    if (VOS_OK != APP_VC_SendSetModemLoopReq(enModemLoop))
    {
        VC_ERR_LOG("APP_VC_AppSetModemLoopProc: SEND SET MODEMLOOP REQ MSG FAILED.");
    }

    return;
}

#if (FEATURE_ON == FEATURE_IMS)
VOS_VOID  APP_VC_ImsaStartHifiNtfProc(IMSA_VC_START_HIFI_NTF_STRU* pstMsg)
{
    CALL_VC_CHANNEL_INFO_STRU                               stChanInfo;
    APP_VC_OPEN_CHANNEL_FAIL_CAUSE_ENUM_UINT32              enVcCause;
    VC_IMSA_EXCEPTION_CAUSE_ENUM_UINT32                     enImsaCause;

    enVcCause = APP_VC_OPEN_CHANNEL_CAUSE_SUCC;
    

    /* ����ȫ�ֱ�������¼��IMSAҪ����HIFI */
    g_stVcStateMgmt.enStartHifiOrig = APP_VC_START_HIFI_ORIG_IMSA;


    /* ����һ��CALL_VC_CHANNEL_INFO_STRU��Ϣ */
    PS_MEM_SET(&stChanInfo, 0, sizeof(stChanInfo));
    stChanInfo.stChannelParam.enMode        = CALL_VC_MODE_IMS_EUTRAN;
    stChanInfo.stChannelParam.enCodecType   =
            APP_VC_ConvertImsaCodeTypeToCallCodeType(pstMsg->enCodeType);

    /* �ŵ�����ʧ�ܣ���IMSA�����쳣��Ϣ */
    if (VOS_FALSE == APP_VC_CallChannelOpenProc(&stChanInfo, &enVcCause))
    {
        /* ��VC��ʧ��ԭ��ֵת����IMSA���쳣ԭ��ֵ */
        enImsaCause = APP_VC_ConvertVcOpenChannelFailCauseToImsaExceptionCause(enVcCause);

        /* ��������ŵ�ʱ�쳣����IMSA����ExceptionNtf��Ϣ��
            ͬʱ����ȫ�ֱ���enStartHifiOrig */
        APP_VC_SendImsaExceptionNtf(enImsaCause);

        /* ������ά�ɲ���Ϣ */
        APP_VC_LogEndCallReq(enVcCause);

        g_stVcStateMgmt.enStartHifiOrig = APP_VC_START_HIFI_ORIG_BUTT;
    }

    return;

}
VOS_VOID  APP_VC_ImsaStopHifiNtfProc(IMSA_VC_STOP_HIFI_NTF_STRU* pstMsg)
{
    /* �����쳣����,�����ǰ�Ĺ���ģʽΪGU��������Ϣ */
    if (CALL_VC_MODE_IMS_EUTRAN != APP_VC_GetRadioMode())
    {
        return;
    }


    /* ���ŵ��ر�ʱ�Ĵ�����ͬ��ֱ�ӵ������к��� */
    APP_VC_CallChannelCloseProc();

}


VOS_VOID  APP_VC_ImsaHifiParaChangeNtfProc(IMSA_VC_HIFI_PARA_CHANGED_NTF_STRU* pstMsg)
{
    CALL_VC_CHANNEL_INFO_STRU           stChanInfo;

    /* �ŵ������ı䣬���ʲ���ʧ��ʱ����Ҫ����ͨ������ʱҪ����ȫ�ֱ������ѽ���ͨ�������͸���Ӧ��ģ�� */
    g_stVcStateMgmt.enStartHifiOrig = APP_VC_START_HIFI_ORIG_IMSA;

    /* ����һ��CALL_VC_CHANNEL_INFO_STRU��Ϣ */
    PS_MEM_SET(&stChanInfo, 0, sizeof(stChanInfo));
    stChanInfo.stChannelParam.enMode = CALL_VC_MODE_IMS_EUTRAN;
    stChanInfo.stChannelParam.enCodecType =
        APP_VC_ConvertImsaCodeTypeToCallCodeType(pstMsg->enCodeType);

    /* ���ŵ������ı�ʱ�Ĵ�����ͬ��ֱ�ӵ������к��� */
    APP_VC_CallChannelParaChangeProc(&stChanInfo);

}
#endif


VOS_VOID  APP_VC_AtParaProc(APP_VC_REQ_MSG_STRU *pstAppMsg)
{

/*�طŹ��ָ̻�ȫ�ֱ���*/
#ifdef __PS_WIN32_RECUR__
        if ( VOS_TRUE == NAS_VC_RestoreContextData((struct MsgCB *)pstAppMsg))
        {
            return;
        }
#endif
    /*�����ǰ��״̬����NULL�Ļ�,������Ϣ,����*/
    if (APP_VC_S_NULL != APP_VC_GetState())
    {
        APP_VC_BufferAtSetMsg(pstAppMsg);
        return;
    }

    switch (pstAppMsg->enMsgName)
    {
        case APP_VC_MSG_REQ_SET_VOLUME:
            APP_VC_AppSetVolumeProc(pstAppMsg);
            break;

        case APP_VC_MSG_REQ_QRY_VOLUME:
            APP_VC_AppQryVolumeProc(pstAppMsg);
            break;

        case APP_VC_MSG_REQ_SET_MODE:
            APP_VC_AppSetModeProc(pstAppMsg);
            break;

        case APP_VC_MSG_REQ_QRY_MODE:
            APP_VC_AppQryModeReqProc(pstAppMsg);
            break;

        case APP_VC_MSG_REQ_SET_PORT:
            APP_VC_AppSetPortReqProc(pstAppMsg);
            break;

        case APP_VC_MSG_REQ_QRY_PORT:
            APP_VC_AppQryPortReqProc(pstAppMsg);
            break;

        case APP_VC_MSG_SET_MUTE_STATUS_REQ:
            APP_VC_AppSetMuteStatusProc(pstAppMsg);
            break;

        case APP_VC_MSG_GET_MUTE_STATUS_REQ:
            APP_VC_AppGetMuteStatusProc(pstAppMsg);
            break;

        case APP_VC_MSG_SET_FOREGROUND_REQ:
            APP_VC_AppSetForeGroundProc(pstAppMsg);
            break;

        case APP_VC_MSG_SET_BACKGROUND_REQ:
            APP_VC_AppSetBackGroundProc(pstAppMsg);
            break;

        case APP_VC_MSG_FOREGROUND_QRY:
            APP_VC_AppQryGroundProc(pstAppMsg);
            break;

        case APP_VC_MSG_SET_MODEMLOOP_REQ:
            APP_VC_AppSetModemLoopProc(pstAppMsg);
            break;

        default:
            VC_WARN_LOG1("APP_VC_AtParaSetProc: wrong MsgName.", pstAppMsg->enMsgName);
            break;
    }
}
VOS_VOID  APP_VC_SetDevCnfProc(
    VCVOICE_OP_RSLT_STRU              *pOpRsltMsg
)
{
    if(pOpRsltMsg->enExeRslt != VCVOICE_EXECUTE_RSLT_SUCC)
    {
        if (APP_VC_S_WAIT_INTERNAL_SET_DEV_RSLT == APP_VC_GetState())
        {
            /* ֻ����״̬�����Ҷϵ绰 */
            APP_VC_UpdateState(APP_VC_S_WAIT_INTERNAL_SET_START_RSLT);
            VC_WARN_LOG("APP_VC_HpaMsgProc: fail:S_WAIT_INTERNAL_SET_DEV to S_WAIT_INTERNAL_SET_START.");
            return;
        }
        else
        {
            /*�ϱ�Device�������*/
            APP_VC_UpdateCurrDevMode(APP_VC_GetPreDevMode());
            APP_VC_ReportEvent(APP_VC_EVT_SET_VOICE_MODE, MN_ERR_UNSPECIFIED);
        }
    }
    else
    {
        if (APP_VC_S_WAIT_INTERNAL_SET_DEV_RSLT == APP_VC_GetState())
        {
            APP_VC_UpdateState(APP_VC_S_WAIT_INTERNAL_SET_START_RSLT);
            return;
        }
        else
        {
            APP_VC_ReportEvent(APP_VC_EVT_SET_VOICE_MODE, MN_ERR_NO_ERROR);
        }
    }
    APP_VC_UpdateState(APP_VC_S_NULL);
    APP_VC_StopProtectTimer();
    APP_VC_BuffMsgProc();

}


VOS_VOID  APP_VC_StartCnfProc(
    VCVOICE_OP_RSLT_STRU              *pstMsg
)
{
    CALL_VC_CHANNEL_INFO_STRU           stChanInfo;

    if(pstMsg->enExeRslt != VCVOICE_EXECUTE_RSLT_SUCC)
    {
        /*ֻ����APP_VC_S_WAIT_INTERNAL_SET_DEV_START_RSLT״̬��ʱ��
          �Ż�Ҷϵ绰�����״̬��˵��֮ǰ�Ѿ��Ҷϵ绰��*/
        if ((APP_VC_S_WAIT_INTERNAL_SET_START_RSLT == APP_VC_GetState())
          ||(APP_VC_S_WAIT_INTERNAL_SET_DEV_RSLT == APP_VC_GetState()))
        {
#if (FEATURE_ON == FEATURE_IMS)
            if (APP_VC_START_HIFI_ORIG_IMSA == APP_VC_GetStartHifiOrig())
            {
                /* �����IMSA�����������󣬸�IMSA�ظ��쳣 */
                APP_VC_SendImsaExceptionNtf(VC_IMSA_EXCEPTION_CAUSE_SET_START_FAIL);
            }
            else
#endif
            {
                /*�����CALL�����������󣬷�����Ϣ��CALLģ�飬�Ҷϵ绰*/
                APP_VC_SendEndCallReq();
            }

            APP_VC_LogEndCallReq(APP_VC_OPEN_CHANNEL_FAIL_CAUSE_SET_START_FAIL);

            g_stVcStateMgmt.bInCall = VOS_FALSE;
            g_stVcStateMgmt.bParaUpdate = VOS_FALSE;
        }
    }
    else
    {
        if (VOS_TRUE == g_stVcStateMgmt.bParaUpdate)
        {
            APP_VC_UpdateState(APP_VC_S_NULL);
            stChanInfo.stChannelParam.enMode = g_stVcStateMgmt.enRadioMode;
            stChanInfo.stChannelParam.enCodecType = g_stVcStateMgmt.enCodecType;
            g_stVcStateMgmt.bParaUpdate = VOS_FALSE;
            APP_VC_CallChannelParaChangeProc(&stChanInfo);

            /* ���ȫ�ֱ��� */
            g_stVcStateMgmt.enStartHifiOrig = APP_VC_START_HIFI_ORIG_BUTT;
            return;
        }
    }

    /* ���ȫ�ֱ��� */
    g_stVcStateMgmt.enStartHifiOrig = APP_VC_START_HIFI_ORIG_BUTT;
    APP_VC_UpdateState(APP_VC_S_NULL);
    APP_VC_StopProtectTimer();
    APP_VC_BuffMsgProc();

}
VOS_VOID APP_VC_SetVolumeCnfProc(
    VCVOICE_OP_RSLT_STRU              *pstOpRsltMsg
)
{
    APP_VC_EVENT_ENUM_U32               enEventType;
    VOS_UINT32                          ulErrCode;
    VOS_UINT32                          ulRslt;

    ulErrCode = MN_ERR_NO_ERROR;

    if (APP_VC_S_WAIT_AT_SET_VOLUME_RSLT == APP_VC_GetState())
    {
        enEventType = APP_VC_EVT_SET_VOLUME;

        if (VCVOICE_EXECUTE_RSLT_SUCC == pstOpRsltMsg->enExeRslt)
        {
            /* �������óɹ���д��NV�� */
            ulRslt = APP_VC_SaveCurVolume();
            if (VOS_OK != ulRslt)
            {
                ulErrCode = MN_ERR_UNSPECIFIED;
            }
        }
        else
        {
            ulErrCode = MN_ERR_UNSPECIFIED;

            APP_VC_UpdateCurrVolume(APP_VC_GetPreVolume());
        }
    }
    else if (APP_VC_S_WAIT_AT_SET_MUTE_STATUS_RSLT == APP_VC_GetState())
    {
        enEventType = APP_VC_EVT_SET_MUTE_STATUS;

        if (VCVOICE_EXECUTE_RSLT_SUCC != pstOpRsltMsg->enExeRslt)
        {
            ulErrCode = MN_ERR_UNSPECIFIED;

            APP_VC_UpdataCurrMuteStatus(APP_VC_GetPreMuteStatus());
        }
    }
    else
    {
        /* ����绰ʱ��������������Ҫ��APP�ظ� */
        VC_NORM_LOG("APP_VC_SetVolumeCnfProc:set volume in call!");
        return;
    }

    APP_VC_ReportEvent(enEventType, ulErrCode);
    APP_VC_UpdateState(APP_VC_S_NULL);
    APP_VC_StopProtectTimer();
    APP_VC_BuffMsgProc();

    return;
 }


VOS_VOID  APP_VC_MedMsgProc(VOS_VOID *pMsg)
{
    VCVOICE_OP_RSLT_STRU              *pOpRsltMsg;

    pOpRsltMsg = (VCVOICE_OP_RSLT_STRU*)pMsg;

    /*��һ�����������̴�ӡ��Ϣ*/
    VC_INFO_LOG1("APP_VC_MedMsgProc: msg Name.", pOpRsltMsg->usMsgName);


    if(pOpRsltMsg->enExeRslt != VCVOICE_EXECUTE_RSLT_SUCC)
    {
        /* ��ӡ�쳣��ӡ */
        VC_WARN_LOG1("APP_VC_MedMsgProc: Exe Result.", pOpRsltMsg->enExeRslt);
        VC_WARN_LOG1("APP_VC_MedMsgProc: msg Name.", pOpRsltMsg->usMsgName);

#if (FEATURE_ON == FEATURE_PTM)
        /* ��¼VC�����쳣log */
        APP_VC_AppVCFailErrRecord(pOpRsltMsg->usMsgName, pOpRsltMsg->enExeRslt);
#endif
    }

    switch (pOpRsltMsg->usMsgName)
    {
        case ID_VOICE_VC_START_CNF:
            APP_VC_StartCnfProc(pOpRsltMsg);
            break;

        case ID_VOICE_VC_SET_DEVICE_CNF:
            APP_VC_SetDevCnfProc(pOpRsltMsg);
            break;

        case ID_VOICE_VC_SET_VOLUME_CNF:
            /*�ϱ������������*/
            APP_VC_SetVolumeCnfProc(pOpRsltMsg);
            break;

        case ID_VOICE_VC_STOP_CNF:
        case ID_VOICE_VC_SET_CODEC_CNF:
            APP_VC_UpdateState(APP_VC_S_NULL);
            APP_VC_StopProtectTimer();
            APP_VC_BuffMsgProc();
            break;

        case ID_VOICE_VC_FOREGROUND_CNF:
            /* ���յ�ǰ̨ģʽ����̨ģʽ���ûظ���Ϣ�Ĵ��� */
            APP_VC_SendSetForeGroundCnf(g_stVcStateMgmt.clientId, pOpRsltMsg->enExeRslt);

            APP_VC_StopProtectTimer();
            APP_VC_BuffMsgProc();
            break;

        case ID_VOICE_VC_BACKGROUND_CNF:
            /* ���յ���̨ģʽ���ûظ���Ϣ�Ĵ��� */
            APP_VC_SendSetBackGroundCnf(g_stVcStateMgmt.clientId, pOpRsltMsg->enExeRslt);

            APP_VC_StopProtectTimer();
            APP_VC_BuffMsgProc();
            break;

        case ID_VOICE_VC_GROUND_RSP:
            /* ���յ�ģʽ��ѯ�ظ���Ϣ�Ĵ��� */
            APP_VC_SendGroundRsp(g_stVcStateMgmt.clientId, ((VCVOICE_GROUND_RSP_STRU*)pOpRsltMsg)->enState, VOS_OK);

            APP_VC_StopProtectTimer();
            APP_VC_BuffMsgProc();
            break;

        default:
            VC_WARN_LOG1("APP_VC_MedMsgProc: wrong MsgName.", pOpRsltMsg->usMsgName);
            APP_VC_UpdateState(APP_VC_S_NULL);
            APP_VC_StopProtectTimer();
            APP_VC_BuffMsgProc();
            break;
    }

}

#if (FEATURE_ON == FEATURE_IMS)
VOS_VOID  APP_VC_ImsaMsgProc(VOS_VOID *pMsg)
{
    MSG_HEADER_STRU                     *pMsgHeader = VOS_NULL_PTR;

    pMsgHeader = (MSG_HEADER_STRU *)pMsg;

    /* ����Ѿ�����HIFI,���ұ�����Ϣ����ΪID_IMSA_VC_START_HIFI_NTF������������ */
    if ( (VOS_TRUE == APP_VC_GetCallStatus())
      && (ID_IMSA_VC_START_HIFI_NTF == pMsgHeader->ulMsgName) )
    {
        /* ����Ҫ֪ͨIMSA exception_nty,ֻ������ά�ɲ���Ϣ */
        APP_VC_LogEndCallReq(APP_VC_OPEN_CHANNEL_FAIL_CAUSE_STARTED);
        
        return;
    }


    /* ���VC״̬��ΪNULL����ʾVC���ڴ�������Ϣ���ѵ�ǰ������Ϣ�Ȼ������� */
    if (APP_VC_S_NULL != APP_VC_GetState())
    {
        APP_VC_BufferImsaMsg(pMsg);
        return;
    }


    /* ����Ϣ���зַ� */
    switch (pMsgHeader->ulMsgName)
    {
        case ID_IMSA_VC_START_HIFI_NTF:
            APP_VC_ImsaStartHifiNtfProc(pMsg);
            break;

        case ID_IMSA_VC_STOP_HIFI_NTF:
            APP_VC_ImsaStopHifiNtfProc(pMsg);
            break;

        case ID_IMSA_VC_HIFI_PARA_CHANGED_NTF:
            APP_VC_ImsaHifiParaChangeNtfProc(pMsg);
            break;

        default:
            VC_WARN_LOG("APP_VC_ImsaMsgProc: wrong MsgName");

            break;
    }

}
#endif


VOS_VOID APP_VC_BuffMsgProc(VOS_VOID)
{
    VOS_UINT32 i;

    VC_INFO_LOG1("APP_VC_BuffMsgProc, ulBuffLen is ", (long)g_stVcStateMgmt.ulBuffLen);

    /*��������е���Ϣ���ǿյģ������Ӧ�Ļ�����Ϣ*/
    if (g_stVcStateMgmt.ulBuffLen != 0)
    {
        /*���������1*/
        g_stVcStateMgmt.ulBuffLen--;

        /*ÿ��ֻ����������е�һ����������*/
        switch (g_stVcStateMgmt.astBufMsg[0].enMsgOrigin)
        {
            case APP_VC_MSG_ORIGIN_AT:
                APP_VC_AtParaProc((APP_VC_REQ_MSG_STRU*)(g_stVcStateMgmt.astBufMsg[0].aucBufMsg));
                break;

            case APP_VC_MSG_ORIGIN_INTERNAL:
                APP_VC_CallChannelInfoProc((CALL_VC_CHANNEL_INFO_MSG_STRU*)(g_stVcStateMgmt.astBufMsg[0].aucBufMsg));
                break;

#if (FEATURE_ON == FEATURE_IMS)
            case APP_VC_MSG_ORIGIN_IMSA:
                APP_VC_ImsaMsgProc((VOS_VOID *)(g_stVcStateMgmt.astBufMsg[0].aucBufMsg));
                break;
#endif

            default:
                VC_ERR_LOG("APP_VC_BuffMsgProc: wrong buff msg.");
                break;
        }

        /*�����ʱ���л��棬������Ļ�������ǰ��*/
        if ( 0 != g_stVcStateMgmt.ulBuffLen )
        {
            for ( i = 0; i < g_stVcStateMgmt.ulBuffLen; i++ )
            {
                VOS_MemCpy(&g_stVcStateMgmt.astBufMsg[i],
                        &g_stVcStateMgmt.astBufMsg[i+1],
                        sizeof(APP_VC_BUFFER_MSG_STRU));

                VC_INFO_LOG1("APP_VC_BuffMsgProc, Buff: enMsgOrigin is",
                    (long)g_stVcStateMgmt.astBufMsg[i].enMsgOrigin);
            }
        }
    }
}


VOS_VOID  APP_VC_StartTimeoutProc(VOS_VOID)
{
    CALL_VC_CHANNEL_INFO_STRU                               stChanInfo;
    APP_VC_OPEN_CHANNEL_FAIL_CAUSE_ENUM_UINT32              enVcCause;
#if (FEATURE_ON == FEATURE_IMS)
    VC_IMSA_EXCEPTION_CAUSE_ENUM_UINT32                     enImsaCause;
#endif
    enVcCause   = APP_VC_OPEN_CHANNEL_CAUSE_SUCC;

    if (VOS_TRUE == g_stVcStateMgmt.bParaUpdate)
    {
        APP_VC_UpdateState(APP_VC_S_NULL);
        stChanInfo.stChannelParam.enMode = g_stVcStateMgmt.enRadioMode;
        stChanInfo.stChannelParam.enCodecType = g_stVcStateMgmt.enCodecType;
        g_stVcStateMgmt.bParaUpdate = VOS_FALSE;
        if (VOS_FALSE == APP_VC_CallChannelOpenProc(&stChanInfo, &enVcCause))
        {
#if (FEATURE_ON == FEATURE_IMS)
            if (APP_VC_START_HIFI_ORIG_IMSA == APP_VC_GetStartHifiOrig())
            {
                /* ��VC��ʧ��ԭ��ֵת����IMSA���쳣ԭ��ֵ */
                enImsaCause = APP_VC_ConvertVcOpenChannelFailCauseToImsaExceptionCause(enVcCause);

                /* �����IMSA�����������󣬸�IMSA�ظ��쳣 */
                /* APP_VC_CallChannelOpenProcû��HIFI�Ƿ��Ѿ��������жϣ�����ʧ��ʱ֪ͨIMSA����HIFI�쳣 */
                APP_VC_SendImsaExceptionNtf(enImsaCause);
            }
            else
#endif
            {
                /*�����CALL�����������󣬷�����Ϣ��CALLģ�飬�Ҷϵ绰*/
                APP_VC_SendEndCallReq();
            }

            /* ������ά�ɲ���Ϣ */
            APP_VC_LogEndCallReq(enVcCause);

            /* ����ȫ�ֱ���enStartHifiOrig */
            g_stVcStateMgmt.enStartHifiOrig = APP_VC_START_HIFI_ORIG_BUTT;
        }
    }
    else
    {
#if (FEATURE_ON == FEATURE_IMS)
        if (APP_VC_START_HIFI_ORIG_IMSA == APP_VC_GetStartHifiOrig())
        {
            /* �����IMSA�����������󣬸�IMSA�ظ��쳣 */
            APP_VC_SendImsaExceptionNtf(VC_IMSA_EXCEPTION_CAUSE_TI_START_EXPIRED);
        }
        else
#endif
        {
            /*�����CALL�����������󣬷�����Ϣ��CALLģ�飬�Ҷϵ绰*/
            APP_VC_SendEndCallReq();
        }

        /* ������ά�ɲ���Ϣ */
        APP_VC_LogEndCallReq(APP_VC_OPEN_CHANNEL_FAIL_CAUSE_TI_START_EXPIRED);

        /* ����ȫ�ֱ���enStartHifiOrig */
        g_stVcStateMgmt.enStartHifiOrig = APP_VC_START_HIFI_ORIG_BUTT;

        g_stVcStateMgmt.bInCall = VOS_FALSE;
        APP_VC_UpdateState(APP_VC_S_NULL);
        APP_VC_BuffMsgProc();
    }

    return;

}


VOS_VOID  APP_VC_TimeoutMsgProc(VOS_VOID *pMsg)
{
    REL_TIMER_MSG                       *pTimerMsg;
    VOS_UINT8                           ucRatType;
    pTimerMsg = (REL_TIMER_MSG*)pMsg;

    /* ��ӡ��ʱ��timer���ƣ�״̬��λ*/
    VC_WARN_LOG2("APP_VC_TimeoutMsgProc: Timer Name, state", (VOS_INT32)pTimerMsg->ulName, (VOS_INT32)APP_VC_GetState());

    NAS_MNTN_CsMedTimeOut(pTimerMsg->ulName);

 #if (FEATURE_ON == FEATURE_PTM)
    /* ��¼VC��ʱlog */
    APP_VC_AppVCFailErrRecord((VOS_UINT16)pTimerMsg->ulName, VCVOICE_EXECUTE_RSLT_BUTT);
 #endif

    switch(pTimerMsg->ulName)
    {
        /*������Ϣ��֪ͨcallģģ��Ҷϵ绰��״̬��λ*/
        case APP_VC_TIMER_START:
            APP_VC_StartTimeoutProc();
            return;

        case APP_VC_TIMER_STOP:
            /*�ط���ϢVC_PHY_STOP_REQ�������*/
            MMC_ComGetRatType(&ucRatType);  /*��ȡ��ǰ����ģʽ*/
            APP_VC_StartProtectTimer(APP_VC_TIMER_STOP);
            APP_VC_SendStopReq((CALL_VC_RADIO_MODE_ENUM_U8)ucRatType);
            break;

        case APP_VC_TIMER_SET_CODEC:
            break;

        case APP_VC_TIMER_SET_DEV:
            if (APP_VC_S_WAIT_AT_SET_DEV_RSLT == APP_VC_GetState())
            {
                APP_VC_UpdateCurrDevMode(APP_VC_GetPreDevMode());
                APP_VC_ReportEvent(APP_VC_EVT_SET_VOICE_MODE, MN_ERR_UNSPECIFIED);
            }
            break;

        case APP_VC_TIMER_SET_VOLUME:

            if (APP_VC_S_WAIT_AT_SET_VOLUME_RSLT == APP_VC_GetState())
            {
                APP_VC_UpdateCurrVolume(APP_VC_GetPreVolume());
                APP_VC_ReportEvent(APP_VC_EVT_SET_VOLUME, MN_ERR_UNSPECIFIED);
            }
            else
            {
                APP_VC_UpdataCurrMuteStatus(APP_VC_GetPreMuteStatus());
                APP_VC_ReportEvent(APP_VC_EVT_SET_MUTE_STATUS, MN_ERR_UNSPECIFIED);
            }

            break;

        case APP_VC_TIMER_SET_FOREGROUND:
            APP_VC_SendSetForeGroundCnf( g_stVcStateMgmt.clientId, VCVOICE_EXECUTE_RSLT_FAIL);
            break;

        case APP_VC_TIMER_SET_BACKGROUND:
            APP_VC_SendSetBackGroundCnf( g_stVcStateMgmt.clientId, VCVOICE_EXECUTE_RSLT_FAIL);
            break;

        case APP_VC_TIMER_QRY_GROUND:
            APP_VC_SendGroundRsp( g_stVcStateMgmt.clientId, VCVOICE_GROUND_BUTT, VOS_ERR);
            break;

        default:
            VC_WARN_LOG1("APP_VC_TimeoutMsgProc: Wrong Timer Name.", (VOS_INT32)pTimerMsg->ulName);
            break;
    }

    APP_VC_UpdateState(APP_VC_S_NULL);
    APP_VC_BuffMsgProc();

}
VOS_VOID  APP_VC_MsgProc(MsgBlock* pMsg)
{
    switch(pMsg->ulSenderPid)
    {
        case WUEPS_PID_TAF:
            APP_VC_CallChannelInfoProc((CALL_VC_CHANNEL_INFO_MSG_STRU*)pMsg);
            break;

        case WUEPS_PID_AT:
            APP_VC_AtParaProc((APP_VC_REQ_MSG_STRU*)pMsg);
            break;

        case DSP_PID_VOICE:
            APP_VC_MedMsgProc(pMsg);
            break;

        case VOS_PID_TIMER:
            APP_VC_TimeoutMsgProc(pMsg);
            break;

#if (FEATURE_ON == FEATURE_IMS)
        case PS_PID_IMSA:
            APP_VC_ImsaMsgProc(pMsg);
            break;
#endif

        default:
            VC_WARN_LOG1("APP_VC_MsgProc: Timer Pid.", (VOS_INT32)pMsg->ulSenderPid);
            break;
    }
}


VOS_UINT32 WuepsVCPidInit(enum VOS_INIT_PHASE_DEFINE InitPhrase)
{
    switch( InitPhrase )
    {
        case   VOS_IP_LOAD_CONFIG:
            APP_VC_ReadNvimInfo();
            APP_VC_InitGlobeVariable();
            break;
        case   VOS_IP_FARMALLOC:
        case   VOS_IP_INITIAL:
        case   VOS_IP_ENROLLMENT:
        case   VOS_IP_LOAD_DATA:
        case   VOS_IP_FETCH_DATA:
        case   VOS_IP_STARTUP:
        case   VOS_IP_RIVAL:
        case   VOS_IP_KICKOFF:
        case   VOS_IP_STANDBY:
        case   VOS_IP_BROADCAST_STATE:
        case   VOS_IP_RESTART:
        case   VOS_IP_BUTT:
            break;
    }

    return VOS_OK;

}


VOS_UINT32 NAS_VC_SndOutsideContextData()
{
    NAS_VC_SDT_MSG_ST                      *pSndMsgCB     = VOS_NULL_PTR;
    NAS_VC_OUTSIDE_RUNNING_CONTEXT_ST                 *pstOutsideCtx;

    pSndMsgCB = (NAS_VC_SDT_MSG_ST *)PS_ALLOC_MSG(WUEPS_PID_VC, sizeof(NAS_VC_SDT_MSG_ST));

    if ( VOS_NULL_PTR == pSndMsgCB )
    {
        MN_ERR_LOG("NAS_VC_SndOutsideContextData:ERROR: Alloc Memory Fail.");
        return VOS_FALSE;
    }

    pstOutsideCtx = &pSndMsgCB->stOutsideCtx;

    PS_MEM_CPY(&pstOutsideCtx->pc_g_g_stVcStateMgmt, &g_stVcStateMgmt,sizeof(APP_VC_STATE_MGMT_STRU));

    pSndMsgCB->MsgHeader.ulReceiverPid   = WUEPS_PID_VC;
    pSndMsgCB->MsgHeader.ulSenderPid     = WUEPS_PID_VC;
    pSndMsgCB->MsgHeader.ulSenderCpuId   = VOS_LOCAL_CPUID;
    pSndMsgCB->MsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;

    pSndMsgCB->MsgHeader.ulLength  = sizeof(NAS_VC_OUTSIDE_RUNNING_CONTEXT_ST) + 4;
    pSndMsgCB->MsgHeader.ulMsgName = EVT_NAS_VC_OUTSIDE_RUNNING_CONTEXT_FOR_PC_REPLAY;

    OM_TraceMsgHook(pSndMsgCB);
    PS_FREE_MSG(WUEPS_PID_VC, pSndMsgCB);

    return VOS_TRUE;
}


VOS_UINT32  APP_VC_SendVcReqToOm(VOS_UINT32 ulStatus, VOS_UINT32 ulPort)
{
    MN_APP_CS_SET_TRANS_PORT_MSG_STRU  *pstMsg =
        (MN_APP_CS_SET_TRANS_PORT_MSG_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_VC,
                                              sizeof(MN_APP_CS_SET_TRANS_PORT_MSG_STRU));

    if (VOS_NULL_PTR == pstMsg)
    {
        MN_ERR_LOG("MN_CALL_SendAppRequest: Failed to alloc VOS message.");
        return TAF_FAILURE;
    }

    /* ��дVOS��Ϣͷ */
    pstMsg->ulSenderCpuId   = VOS_LOCAL_CPUID;
    pstMsg->ulSenderPid     = WUEPS_PID_VC;
    pstMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverPid   = ACPU_PID_PCVOICE;

    /* ��дԭ���ײ� */
    pstMsg->usPrimId        = TAF_OAM_SET_TRANS_PORT_REQ;
    pstMsg->ulStatus        = ulStatus;
    pstMsg->ulPort          = ulPort;

    /* ����VOS��Ϣ */
    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_VC, pstMsg))
    {
        VC_ERR_LOG("MN_CALL_SendAppRequest: SEND MSG FAIL.");
        return TAF_FAILURE;
    }

    return TAF_SUCCESS;
}

#ifdef __PS_WIN32_RECUR__
VOS_UINT32 NAS_VC_RestoreContextData(struct MsgCB * pMsg)
{
    NAS_VC_SDT_MSG_ST                      *pRcvMsgCB;
    NAS_VC_OUTSIDE_RUNNING_CONTEXT_ST      *pstOutsideCtx;

    pRcvMsgCB     = (NAS_VC_SDT_MSG_ST *)pMsg;

    if (EVT_NAS_VC_OUTSIDE_RUNNING_CONTEXT_FOR_PC_REPLAY == pRcvMsgCB->MsgHeader.ulMsgName)
    {
        PS_MEM_CPY(&g_stVcStateMgmt, &pstOutsideCtx->pc_g_g_stVcStateMgmt,sizeof(APP_VC_STATE_MGMT_STRU));

        PS_LOG(WUEPS_PID_VC, VOS_NULL, PS_PRINT_INFO,"CALL: NAS_VC_RestoreContextData - data is restored.");
        return VOS_TRUE;
    }
    return VOS_FALSE;
}

#endif


VOS_UINT32 APP_VC_SaveCurVolume(VOS_VOID)
{
    APP_VC_NV_CLVL_VOLUME_STRU          stClvlVolume;
    VOS_INT16                           sCurVolume;
    VC_PHY_DEVICE_MODE_ENUM_U16         enCurrDevMode;

    if (NV_OK != NV_Read(en_NV_Item_Clvl_Volume,
                         &stClvlVolume,
                         sizeof(APP_VC_NV_CLVL_VOLUME_STRU)))
    {
        VC_ERR_LOG("APP_VC_SaveCurVolume: NV Read Fail!");
        return VOS_ERR;
    }

    /* ��ȡ��ǰ���õ����� */
    sCurVolume = APP_VC_GetCurrVolume();

    /* ��ȡ��ǰ�豸 */
    enCurrDevMode = APP_VC_GetCurrDevMode();

    VC_NORM_LOG1("APP_VC_SaveCurVolume: Current volume is !", sCurVolume);

    VC_NORM_LOG1("APP_VC_SaveCurVolume: Current device is !", enCurrDevMode);

    switch(enCurrDevMode)
    {
        case VC_PHY_DEVICE_MODE_HANDSET:
            stClvlVolume.sHandSetVolValue = sCurVolume;
            break;

        case VC_PHY_DEVICE_MODE_HANDS_FREE:
            stClvlVolume.sHandsFreeVolValue = sCurVolume;
            break;

        case VC_PHY_DEVICE_MODE_CAR_FREE:
            stClvlVolume.sCarFreeVolValue = sCurVolume;
            break;

        case VC_PHY_DEVICE_MODE_EARPHONE:
            stClvlVolume.sEarphoneVolValue = sCurVolume;
            break;

        case VC_PHY_DEVICE_MODE_BLUETOOTH:
            stClvlVolume.sBlueToothVolValue = sCurVolume;
            break;

        case VC_PHY_DEVICE_MODE_PCVOICE:
            stClvlVolume.sPcVoiceVolValue = sCurVolume;
            break;

        case VC_PHY_DEVICE_MODE_HEADPHONE:
            stClvlVolume.sHeadPhoneVolValue = sCurVolume;
            break;
        case VC_PHY_DEVICE_MODE_SUPER_HANDFREE:
            stClvlVolume.sSuperFreeVolValue = sCurVolume;
            break;
        case VC_PHY_DEVICE_MODE_SMART_TALK:
            stClvlVolume.sSmartTalkVolValue = sCurVolume;
            break;

        default:
            VC_ERR_LOG("APP_VC_SaveCurVolume: device is invalid!");
            return VOS_ERR;
    }

    if (NV_OK != NV_Write(en_NV_Item_Clvl_Volume,
                         &stClvlVolume,
                         sizeof(APP_VC_NV_CLVL_VOLUME_STRU)))
    {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 APP_VC_GetCurVolume(VOS_INT16 *pCurVolume)
{
    APP_VC_NV_CLVL_VOLUME_STRU          stClvlVolume;
    VC_PHY_DEVICE_MODE_ENUM_U16         enCurrDevMode;


    PS_MEM_SET(&stClvlVolume, 0x00, sizeof(stClvlVolume));

    if (NV_OK != NV_Read(en_NV_Item_Clvl_Volume,
                         &stClvlVolume,
                         sizeof(APP_VC_NV_CLVL_VOLUME_STRU)))
    {
        return VOS_ERR;
    }

    /* ��ȡ��ǰ�豸 */
    enCurrDevMode = APP_VC_GetCurrDevMode();

    switch(enCurrDevMode)
    {
        case VC_PHY_DEVICE_MODE_HANDSET:
            *pCurVolume = stClvlVolume.sHandSetVolValue;
            break;

        case VC_PHY_DEVICE_MODE_HANDS_FREE:
            *pCurVolume = stClvlVolume.sHandsFreeVolValue;
            break;

        case VC_PHY_DEVICE_MODE_CAR_FREE:
            *pCurVolume = stClvlVolume.sCarFreeVolValue;
            break;

        case VC_PHY_DEVICE_MODE_EARPHONE:
            *pCurVolume = stClvlVolume.sEarphoneVolValue;
            break;

        case VC_PHY_DEVICE_MODE_BLUETOOTH:
            *pCurVolume = stClvlVolume.sBlueToothVolValue;
            break;

        case VC_PHY_DEVICE_MODE_PCVOICE:
            *pCurVolume = stClvlVolume.sPcVoiceVolValue;
            break;

        case VC_PHY_DEVICE_MODE_HEADPHONE:
            *pCurVolume = stClvlVolume.sHeadPhoneVolValue;
            break;

        case VC_PHY_DEVICE_MODE_SUPER_HANDFREE:
            *pCurVolume = stClvlVolume.sSuperFreeVolValue;
            break;

        case VC_PHY_DEVICE_MODE_SMART_TALK:
            *pCurVolume = stClvlVolume.sSmartTalkVolValue;
            break;

        default:
            *pCurVolume = 0;
            return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 APP_VC_GetVoiceTestFlag(VOS_UINT32 *pVoiceTestFlag)
{
    VOS_UINT32                          ulLength;
    TAF_NV_VOICE_TEST_FLAG_STRU         stVoiceTestFlag;

    ulLength    = 0;
    VOS_MemSet(&stVoiceTestFlag, 0, sizeof(TAF_NV_VOICE_TEST_FLAG_STRU));

    NV_GetLength(en_NV_Item_VOICE_TEST_FLAG, &ulLength);
    if (ulLength > sizeof(stVoiceTestFlag))
    {
        return VOS_ERR;
    }

    if (NV_OK != NV_Read(en_NV_Item_VOICE_TEST_FLAG,
                         &stVoiceTestFlag, ulLength))
    {
        VC_ERR_LOG("APP_VC_GetVcVoiceFlag() NV read Error!");
        return VOS_ERR;
    }

    *pVoiceTestFlag = stVoiceTestFlag.ulFlagValue;
    return VOS_OK;
}

 #if (FEATURE_ON == FEATURE_PTM)

VOS_VOID APP_VC_AppVCFailErrRecord(
    VOS_UINT16                          usName,
    VOS_UINT16                          usCause
)
{
    NAS_ERR_LOG_VC_FAILURE_EVENT_STRU                       stVcFailEvent;
    VOS_UINT32                                              ulIsLogRecord;
    VOS_UINT32                                              ulLength;
    VOS_UINT32                                              ulResult;
    VOS_UINT16                                              usLevel;

    /* ��ѯ��ӦAlarm Id�Ƿ���Ҫ��¼�쳣��Ϣ */
    usLevel       = NAS_GetErrLogAlmLevel(NAS_ERR_LOG_ALM_VC_OPT_FAIL);
    ulIsLogRecord = TAF_SDC_IsErrLogNeedRecord(usLevel);

    /* ����Ҫ��¼ʱ���������쳣��Ϣ */
    if (VOS_FALSE == ulIsLogRecord)
    {
        return;
    }

    ulLength = sizeof(NAS_ERR_LOG_VC_FAILURE_EVENT_STRU);

    /* ���VC�쳣��Ϣ */
    PS_MEM_SET(&stVcFailEvent, 0x00, ulLength);

    NAS_COMM_BULID_ERRLOG_HEADER_INFO(&stVcFailEvent.stHeader,
                                      VOS_GetModemIDFromPid(WUEPS_PID_VC),
                                      NAS_ERR_LOG_ALM_VC_OPT_FAIL,
                                      usLevel,
                                      VOS_GetSlice(),
                                      (ulLength - sizeof(OM_ERR_LOG_HEADER_STRU)));

    stVcFailEvent.ulOptState = APP_VC_GetState();
    stVcFailEvent.ulCasue    = usCause;
    stVcFailEvent.ulName     = usName;

    /*
       ���쳣��Ϣд��Buffer��
       ʵ��д����ַ�������Ҫд��Ĳ������ӡ�쳣
     */
    ulResult = TAF_SDC_PutErrLogRingBuf((VOS_CHAR *)&stVcFailEvent, ulLength);
    if (ulResult != ulLength)
    {
        NAS_ERROR_LOG(WUEPS_PID_VC, "APP_VC_AppVCFailErrRecord(): Push buffer error.");
    }

    NAS_COM_MntnPutRingbuf(NAS_ERR_LOG_ALM_VC_OPT_FAIL,
                           WUEPS_PID_VC,
                           (VOS_UINT8 *)&stVcFailEvent,
                           sizeof(stVcFailEvent));
    return;
}
#endif

#ifdef  __cplusplus
  #if  __cplusplus
  }
  #endif
#endif


