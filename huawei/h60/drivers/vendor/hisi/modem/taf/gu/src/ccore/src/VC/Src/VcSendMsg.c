

/*****************************************************************************
   1 ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "Ps.h"
#include "VcPhyInterface.h"
#include "VcCallInterface.h"
#include "VcCodecInterface.h"
#include "VcComm.h"

#include "NasUtranCtrlInterface.h"

#include "MnComm.h"
#if (FEATURE_ON == FEATURE_IMS)
#include "VcImsaInterface.h"
#endif
#include "TafOamInterface.h"

#ifdef  __cplusplus
  #if  __cplusplus
      extern "C"{
  #endif
#endif


/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_VC_SEND_MSG_C
/*lint +e767*/

extern  VCVOICE_WORK_TYPE_ENUM_UINT16 APP_VC_GetWorkType(VOS_VOID);

/*****************************************************************************
   2 ����ʵ��
*****************************************************************************/

VOS_UINT32  APP_VC_SendStartReq(
    CALL_VC_CHANNEL_INFO_STRU           *pstChanInfo,
    CALL_VC_RADIO_MODE_ENUM_U8          enRadioMode
)
{
    /*������ϢVC_PHY_START_REQ�����͸������*/
    VCVOICE_START_REQ_STRU             *pstStartReq;
    VOS_UINT32                          ulRet;

    VCVOICE_NET_MODE_ENUM_UINT16        enMode;
    NAS_UTRANCTRL_UTRAN_MODE_ENUM_UINT8 enUtranMode;


#if (defined (NAS_STUB) || defined(__PS_WIN32_RECUR__))
    VCVOICE_OP_RSLT_STRU               *pstRstMsg;
    VOS_UINT32                          ulSenderPid;
#endif

    /* ������Ϣ */
    pstStartReq = (VCVOICE_START_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                 sizeof(VCVOICE_START_REQ_STRU) - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstStartReq)
    {
        return VOS_ERR;
    }

    pstStartReq->usMsgName    = ID_VC_VOICE_START_REQ;
    pstStartReq->ulReceiverPid           = DSP_PID_VOICE;

#if (defined(__PS_WIN32_RECUR__))
    ulSenderPid = pstStartReq->ulReceiverPid;
#endif
    enMode = pstChanInfo->stChannelParam.enMode;
    enUtranMode = NAS_UTRANCTRL_GetCurrUtranMode();

    /* �����Ϣ�е���ʽΪWCDMA�������һ���жϵ�ǰ�Ƿ��ǹ�����TD-SCDMAģʽ��
        �����ǰ������TD-SCDMAģʽ������TD-SCDMA��ʽ֪ͨHIFI������������ */
    if ( (VCVOICE_NET_MODE_WCDMA       == enMode)
      && (NAS_UTRANCTRL_UTRAN_MODE_TDD == enUtranMode ) )
    {
        enMode = VCVOICE_NET_MODE_TDSCDMA;
    }

    /* �������ģʽ��EUTRAN_IMS,����EUTRAN_IMS��ʽ֪ͨHIFI������������ */
    if (CALL_VC_MODE_IMS_EUTRAN == pstChanInfo->stChannelParam.enMode)
    {
        enMode = VCVOICE_NET_MODE_IMS_EUTRAN;
    }

    pstStartReq->enMode       = enMode;

    pstStartReq->enCodecType  = pstChanInfo->stChannelParam.enCodecType;
    pstStartReq->enWorkType   = APP_VC_GetWorkType();

    ulRet = PS_SEND_MSG(WUEPS_PID_VC, pstStartReq);

#if (defined(__PS_WIN32_RECUR__))

    /*������ϢPHY_VC_START_CNF�����͸�VC*/
    pstRstMsg = (VCVOICE_OP_RSLT_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                 sizeof(VCVOICE_OP_RSLT_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstRstMsg)
    {
        return VOS_ERR;
    }

    pstRstMsg->usMsgName          = ID_VOICE_VC_START_CNF;
    pstRstMsg->enExeRslt          = VCVOICE_EXECUTE_RSLT_SUCC;
    pstRstMsg->ulReceiverPid      = WUEPS_PID_VC;
    pstRstMsg->ulSenderPid        = ulSenderPid;

    PS_SEND_MSG(pstStartReq->ulReceiverPid, pstRstMsg);

#endif

    return ulRet;

}


VOS_UINT32  APP_VC_SendStopReq(
    CALL_VC_RADIO_MODE_ENUM_U8          enRadioMode
)
{
    /*������ϢVC_PHY_STOP_REQ�����͸������*/
    VCVOICE_STOP_REQ_STRU              *pstStopReq;
    VOS_UINT32                          ulRet;

#if (defined(__PS_WIN32_RECUR__))
    VCVOICE_OP_RSLT_STRU               *pstRstMsg;
    VOS_UINT32                          ulSenderPid;
#endif

    /* ������Ϣ */
    pstStopReq = (VCVOICE_STOP_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                 sizeof(VCVOICE_STOP_REQ_STRU) - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstStopReq)
    {
        return VOS_ERR;
    }

    pstStopReq->usMsgName = ID_VC_VOICE_STOP_REQ;
    pstStopReq->ulReceiverPid       = DSP_PID_VOICE;


#if (defined(__PS_WIN32_RECUR__))
    ulSenderPid = pstStopReq->ulReceiverPid;
#endif

    ulRet = PS_SEND_MSG(WUEPS_PID_VC, pstStopReq);

#if (defined(__PS_WIN32_RECUR__))

    /*������ϢPHY_VC_STOP_CNF�����͸�VC*/
    pstRstMsg = (VCVOICE_OP_RSLT_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                 sizeof(VCVOICE_OP_RSLT_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstRstMsg)
    {
        return VOS_ERR;
    }

    pstRstMsg->usMsgName              = ID_VOICE_VC_STOP_CNF;
    pstRstMsg->enExeRslt              = VCVOICE_EXECUTE_RSLT_SUCC;
    pstRstMsg->ulSenderPid            = ulSenderPid;

    PS_SEND_MSG(pstStopReq->ulReceiverPid, pstRstMsg);

#endif

    return ulRet;
}
VOS_UINT32  APP_VC_SendSetDeviceReq(
    VC_DEVICE_PARA_STRU                 *pstDevPara
)
{
    /*������ϢVC_PHY_SET_DEVICE_REQ�����͸������*/
    VCVOICE_SET_DEVICE_REQ_STRU        *pstSetDeviceReq;
    VOS_UINT32                          ulRet;

#if (defined(__PS_WIN32_RECUR__))
    VCVOICE_OP_RSLT_STRU               *pstRstMsg;
    VOS_UINT32                          ulSenderPid;
#endif

    /* ������Ϣ */
    pstSetDeviceReq = (VCVOICE_SET_DEVICE_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                 sizeof(VCVOICE_SET_DEVICE_REQ_STRU) - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstSetDeviceReq)
    {
        return VOS_ERR;
    }

    pstSetDeviceReq->usMsgName      = ID_VC_VOICE_SET_DEVICE_REQ;
    pstSetDeviceReq->ulReceiverPid  = DSP_PID_VOICE;
    pstSetDeviceReq->usReserve      = 0;
    pstSetDeviceReq->usReserve2     = 0;

#if (defined(__PS_WIN32_RECUR__))
    ulSenderPid = pstSetDeviceReq->ulReceiverPid;
#endif

    pstSetDeviceReq->usDeviceMode    = pstDevPara->enDeviceMode;

    ulRet = PS_SEND_MSG(WUEPS_PID_VC, pstSetDeviceReq);

#if (defined(__PS_WIN32_RECUR__))

    /*������ϢVCCODEC_EXECUTE_RSLT_SUCC�����͸�VC*/
    pstRstMsg = (VCVOICE_OP_RSLT_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                 sizeof(VCVOICE_OP_RSLT_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstRstMsg)
    {
        return VOS_ERR;
    }

    pstRstMsg->usMsgName          = ID_VOICE_VC_SET_DEVICE_CNF;
    pstRstMsg->enExeRslt          = VCVOICE_EXECUTE_RSLT_SUCC;
    pstRstMsg->ulReceiverPid      = WUEPS_PID_VC;
    pstRstMsg->ulSenderPid        = ulSenderPid;

    PS_SEND_MSG(pstSetDeviceReq->ulReceiverPid, pstRstMsg);

#endif

    return ulRet;
}
VOS_UINT32  APP_VC_SendSetVolumeReq(
    VOS_UINT16                          usVolTarget,
    VOS_INT16                           sVolValue
)
{
    /*������ϢVC_PHY_SET_VOLUME_REQ�����͸������*/
    VCVOICE_SET_VOLUME_REQ_STRU        *pstSetVolumeReq;
    VOS_UINT32                          ulRet;

#if (defined(__PS_WIN32_RECUR__))
    VCVOICE_OP_RSLT_STRU               *pstRstMsg;
    VOS_UINT32                          ulSenderPid;
#endif

    /* ������Ϣ */
    pstSetVolumeReq = (VCVOICE_SET_VOLUME_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                 sizeof(VCVOICE_SET_VOLUME_REQ_STRU) - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstSetVolumeReq)
    {
        return VOS_ERR;
    }

    pstSetVolumeReq->usMsgName      = ID_VC_VOICE_SET_VOLUME_REQ;
    pstSetVolumeReq->enVolTarget    = usVolTarget;
    pstSetVolumeReq->sVolValue      = sVolValue;
    pstSetVolumeReq->ulReceiverPid  = DSP_PID_VOICE;
    pstSetVolumeReq->usReserve      = 0;

#if (defined(__PS_WIN32_RECUR__))
    ulSenderPid = pstSetVolumeReq->ulReceiverPid;
#endif

    ulRet = PS_SEND_MSG(WUEPS_PID_VC, pstSetVolumeReq);

#if (defined(__PS_WIN32_RECUR__))

    /*������ϢID_CODEC_VC_SET_VOLUME_CNF�����͸�VC*/
    pstRstMsg = (VCVOICE_OP_RSLT_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                 sizeof(VCVOICE_OP_RSLT_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstRstMsg)
    {
        return VOS_ERR;
    }

    pstRstMsg->usMsgName          = ID_VOICE_VC_SET_VOLUME_CNF;
    pstRstMsg->enExeRslt          = VCVOICE_EXECUTE_RSLT_SUCC;
    pstRstMsg->ulReceiverPid      = WUEPS_PID_VC;
    pstRstMsg->ulSenderPid        = ulSenderPid;

    PS_SEND_MSG(pstSetVolumeReq->ulReceiverPid, pstRstMsg);

#endif

    return ulRet;

}
VOS_UINT32  APP_VC_SendSetCodecReq(
    CALL_VC_CHANNEL_INFO_STRU           *pstChanInfo
)
{
    /*������ϢVC_PHY_SET_CODEC_REQ�����͸������*/
    VCVOICE_SET_CODEC_REQ_STRU         *pstSetCodecReq;
    VOS_UINT32                          ulRet;
    VCVOICE_NET_MODE_ENUM_UINT16        enMode;
    NAS_UTRANCTRL_UTRAN_MODE_ENUM_UINT8 enUtranMode;


#if (defined(__PS_WIN32_RECUR__))
    VCVOICE_OP_RSLT_STRU              *pstRstMsg;
    VOS_UINT32                          ulSenderPid;
#endif

    /* ������Ϣ */
    pstSetCodecReq = (VCVOICE_SET_CODEC_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                 sizeof(VCVOICE_SET_CODEC_REQ_STRU) - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstSetCodecReq)
    {
        return VOS_ERR;
    }

    pstSetCodecReq->usMsgName   = ID_VC_VOICE_SET_CODEC_REQ;

    pstSetCodecReq->enCodecType = pstChanInfo->stChannelParam.enCodecType;
    pstSetCodecReq->ulReceiverPid             = DSP_PID_VOICE;

    enMode = pstChanInfo->stChannelParam.enMode;
    enUtranMode = NAS_UTRANCTRL_GetCurrUtranMode();

    /* �����Ϣ�е���ʽΪWCDMA�������һ���жϵ�ǰ�Ƿ��ǹ�����TD-SCDMAģʽ��
        �����ǰ������TD-SCDMAģʽ������TD-SCDMA��ʽ֪ͨHIFI������������ */
    if ( (VCVOICE_NET_MODE_WCDMA       == enMode)
      && (NAS_UTRANCTRL_UTRAN_MODE_TDD == enUtranMode ) )
    {
        enMode = VCVOICE_NET_MODE_TDSCDMA;
    }

    /* �������ģʽ��EUTRAN_IMS,����EUTRAN_IMS��ʽ֪ͨHIFI������������ */
    if (CALL_VC_MODE_IMS_EUTRAN == pstChanInfo->stChannelParam.enMode)
    {
        enMode = VCVOICE_NET_MODE_IMS_EUTRAN;
    }
    pstSetCodecReq->enMode       = enMode;


#if (defined(__PS_WIN32_RECUR__))
    ulSenderPid = pstSetCodecReq->ulReceiverPid;
#endif

    ulRet = PS_SEND_MSG(WUEPS_PID_VC, pstSetCodecReq);

#if (defined(__PS_WIN32_RECUR__))

    /*������ϢPHY_VC_SET_CODEC_CNF�����͸�VC*/
    pstRstMsg = (VCVOICE_OP_RSLT_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                 sizeof(VCVOICE_OP_RSLT_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstRstMsg)
    {
        return VOS_ERR;
    }

    pstRstMsg->usMsgName          = ID_VOICE_VC_SET_CODEC_CNF;
    pstRstMsg->enExeRslt          = VCVOICE_EXECUTE_RSLT_SUCC;
    pstRstMsg->ulReceiverPid      = WUEPS_PID_VC;
    pstRstMsg->ulSenderPid        = ulSenderPid;

    PS_SEND_MSG(ulSenderPid, pstRstMsg);

#endif

    return ulRet;
}


VOS_UINT32  APP_VC_SendEndCallReq(VOS_VOID)
{
    VC_CALL_MSG_STRU            *pstMsg;

    /*������Ϣ�����͸�CALLģ��*/
    /* ������Ϣ */
    pstMsg = (VC_CALL_MSG_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                 sizeof(VC_CALL_MSG_STRU) - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstMsg)
    {
        return VOS_ERR;
    }

    PS_MEM_SET(((VOS_UINT8*)pstMsg + VOS_MSG_HEAD_LENGTH),
               0,
               (sizeof(VC_CALL_MSG_STRU) - VOS_MSG_HEAD_LENGTH));

    pstMsg->ulReceiverPid = WUEPS_PID_TAF;
    pstMsg->enMsgName     = VC_CALL_END_CALL;

    return PS_SEND_MSG(WUEPS_PID_VC, pstMsg);

}


VOS_UINT32  APP_VC_SendPhyTestModeNotify(
    CALL_VC_RADIO_MODE_ENUM_U8              enMode
)
{
    VC_PHY_TEST_MODE_NOTIFY_STRU            *pstMsg;

    /* ������Ϣ */
    pstMsg = (VC_PHY_TEST_MODE_NOTIFY_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                 sizeof(VC_PHY_TEST_MODE_NOTIFY_STRU) - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstMsg)
    {
        return VOS_ERR;
    }

    /*
        ��ͬ����ģʽ����Ҫ���Ͳ�ͬ�Ĳ���ģʽ��Ϣ������㣬��ǰ�Ĳ���ģʽ
        ֻ��G DSP��Ҫ������Ŀǰ��ʵ����ֻ���͸�GDSP����������ֱ�ӷ��أ�����
        ����ģ��DSP����Ҫ��ʱ�������
    */
    if (CALL_VC_MODE_GSM == enMode)
    {
        pstMsg->usMsgName       = ID_VC_GPHY_TEST_MODE_NOTIFY;
        pstMsg->ulReceiverPid   = DSP_PID_GPHY;
    }
    else
    {
        PS_FREE_MSG(WUEPS_PID_VC, pstMsg);
        return VOS_OK;
    }

    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_VC, pstMsg))
    {
        VC_WARN_LOG("APP_VC_SendPhyTestModeNotify():WARNING:SEND MSG FIAL");
    }

    return VOS_OK;
}



VOS_UINT32 APP_VC_SendSetForeGroundReq(VOS_VOID)
{
    VCVOICE_FOREGROUND_REQ_STRU        *pstCodecMsg = VOS_NULL_PTR;

    pstCodecMsg = (VCVOICE_FOREGROUND_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                                        sizeof(VCVOICE_FOREGROUND_REQ_STRU) - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstCodecMsg)
    {
        return VOS_ERR;
    }

    pstCodecMsg->usMsgName       = ID_VC_VOICE_FOREGROUND_REQ;
    pstCodecMsg->usReserve       = 0;
    pstCodecMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstCodecMsg->ulSenderPid     = WUEPS_PID_VC;
    pstCodecMsg->ulSenderCpuId   = VOS_LOCAL_CPUID;
    pstCodecMsg->ulReceiverPid   = DSP_PID_VOICE;

    if (VOS_OK != PS_SEND_MSG(pstCodecMsg->ulSenderPid, pstCodecMsg))
    {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 APP_VC_SendSetBackGroundReq(VOS_VOID)
{
    VCVOICE_BACKGROUND_REQ_STRU        *pstCodecMsg = VOS_NULL_PTR;

    pstCodecMsg = (VCVOICE_BACKGROUND_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                                        sizeof(VCVOICE_BACKGROUND_REQ_STRU) - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstCodecMsg)
    {
        return VOS_ERR;
    }

    pstCodecMsg->usMsgName       = ID_VC_VOICE_BACKGROUND_REQ;
    pstCodecMsg->usReserve       = 0;
    pstCodecMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstCodecMsg->ulSenderPid     = WUEPS_PID_VC;
    pstCodecMsg->ulSenderCpuId   = VOS_LOCAL_CPUID;
    pstCodecMsg->ulReceiverPid   = DSP_PID_VOICE;

    if (VOS_OK != PS_SEND_MSG(pstCodecMsg->ulSenderPid, pstCodecMsg))
    {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 APP_VC_SendGroundQry(VOS_VOID)
{
    VCVOICE_GROUND_QRY_STRU    *pstCodecMsg = VOS_NULL_PTR;

    pstCodecMsg = (VCVOICE_GROUND_QRY_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                                        sizeof(VCVOICE_GROUND_QRY_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstCodecMsg)
    {
        return VOS_ERR;
    }

    pstCodecMsg->usMsgName       = ID_VC_VOICE_GROUND_QRY;
    pstCodecMsg->usReserve       = 0;
    pstCodecMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstCodecMsg->ulSenderPid     = WUEPS_PID_VC;
    pstCodecMsg->ulSenderCpuId   = VOS_LOCAL_CPUID;
    pstCodecMsg->ulReceiverPid   = DSP_PID_VOICE;

    if (VOS_OK != PS_SEND_MSG(pstCodecMsg->ulSenderPid, pstCodecMsg))
    {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32  APP_VC_SendSetForeGroundCnf(
    MN_CLIENT_ID_T                      usClientId,
    VCVOICE_EXECUTE_RSLT_ENUM_UINT16    enExeRslt
)
{
    MN_AT_IND_EVT_STRU                 *pstAtMsg = VOS_NULL_PTR;

    pstAtMsg = (MN_AT_IND_EVT_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                                        sizeof(MN_AT_IND_EVT_STRU) - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstAtMsg)
    {
        return VOS_ERR;
    }

    PS_MEM_SET(pstAtMsg, 0x00, sizeof(MN_AT_IND_EVT_STRU));

    pstAtMsg->ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstAtMsg->ulSenderPid       = WUEPS_PID_VC;
    pstAtMsg->ulReceiverPid     = WUEPS_PID_AT;
    pstAtMsg->ulSenderCpuId     = VOS_LOCAL_CPUID;
    pstAtMsg->ulLength          = sizeof(MN_AT_IND_EVT_STRU) - VOS_MSG_HEAD_LENGTH;
    pstAtMsg->clientId          = MN_GetRealClientId(usClientId, WUEPS_PID_VC);
    pstAtMsg->usMsgName         = APP_VC_MSG_SET_FOREGROUND_CNF;

    /* �ظ���Ϣ�����е�һ���ֽ�Ϊ���ý�� */
    if ( VCVOICE_EXECUTE_RSLT_SUCC == enExeRslt )
    {
        pstAtMsg->aucContent[0] = VOS_OK;
    }
    else
    {
        pstAtMsg->aucContent[0] = VOS_ERR;
    }

    if (VOS_OK != PS_SEND_MSG(pstAtMsg->ulSenderPid, pstAtMsg) )
    {
        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32  APP_VC_SendSetBackGroundCnf(
    MN_CLIENT_ID_T                      usClientId,
    VCVOICE_EXECUTE_RSLT_ENUM_UINT16    enExeRslt
)
{
    MN_AT_IND_EVT_STRU                 *pstAtMsg = VOS_NULL_PTR;

    pstAtMsg = (MN_AT_IND_EVT_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                                        sizeof(MN_AT_IND_EVT_STRU) - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstAtMsg)
    {
        return VOS_ERR;
    }

    PS_MEM_SET(pstAtMsg, 0x00, sizeof(MN_AT_IND_EVT_STRU));

    pstAtMsg->ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstAtMsg->ulSenderPid       = WUEPS_PID_VC;
    pstAtMsg->ulReceiverPid     = WUEPS_PID_AT;
    pstAtMsg->ulSenderCpuId     = VOS_LOCAL_CPUID;
    pstAtMsg->ulLength          = sizeof(MN_AT_IND_EVT_STRU) - VOS_MSG_HEAD_LENGTH;
    pstAtMsg->clientId          = MN_GetRealClientId(usClientId, WUEPS_PID_VC);
    pstAtMsg->usMsgName         = APP_VC_MSG_SET_BACKGROUND_CNF;

    /* �ظ���Ϣ�����е�һ���ֽ�Ϊ���ý�� */
    if ( VCVOICE_EXECUTE_RSLT_SUCC == enExeRslt )
    {
        pstAtMsg->aucContent[0] = VOS_OK;
    }
    else
    {
        pstAtMsg->aucContent[0] = VOS_ERR;
    }

    if (VOS_OK != PS_SEND_MSG(pstAtMsg->ulSenderPid, pstAtMsg) )
    {
        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32  APP_VC_SendGroundRsp(
    MN_CLIENT_ID_T                      usClientId,
    VCVOICE_GROUND_ENUM_UINT16          enState ,
    VOS_UINT8                           ucQryRslt
)
{
    MN_AT_IND_EVT_STRU                 *pstAtMsg = VOS_NULL_PTR;
    APP_VC_QRY_GROUNG_RSP_STRU          stQryRsp;

    pstAtMsg = (MN_AT_IND_EVT_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                                        sizeof(MN_AT_IND_EVT_STRU) - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstAtMsg)
    {
        return VOS_ERR;
    }

    PS_MEM_SET(pstAtMsg, 0x00, sizeof(MN_AT_IND_EVT_STRU));

    pstAtMsg->ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstAtMsg->ulSenderPid       = WUEPS_PID_VC;
    pstAtMsg->ulReceiverPid     = WUEPS_PID_AT;
    pstAtMsg->ulSenderCpuId     = VOS_LOCAL_CPUID;
    pstAtMsg->ulLength          = sizeof(MN_AT_IND_EVT_STRU) - VOS_MSG_HEAD_LENGTH;
    pstAtMsg->clientId          = MN_GetRealClientId(usClientId, WUEPS_PID_VC);
    pstAtMsg->usMsgName         = APP_VC_MSG_FOREGROUND_RSP;

    /* ģʽ��ѯ��� */
    stQryRsp.ucQryRslt   = ucQryRslt;
    stQryRsp.aucReserved = 0;
    stQryRsp.enGround    = (APP_VC_QRY_GROUND_ENUM_U16)enState;

    /* ��д����Ϣ���� */
    PS_MEM_CPY(&pstAtMsg->aucContent[0], &stQryRsp, sizeof(APP_VC_QRY_GROUNG_RSP_STRU));

    /*������Ϣ��AT_PID;*/
    if (VOS_OK != PS_SEND_MSG(pstAtMsg->ulSenderPid, pstAtMsg))
    {
        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 APP_VC_SendSetModemLoopReq(VCVOICE_LOOP_ENUM_UINT16 enVoiceLoop)
{
    VCVOICE_LOOP_REQ_STRU    *pstMsg = VOS_NULL_PTR;

    pstMsg = (VCVOICE_LOOP_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                                        sizeof(VCVOICE_LOOP_REQ_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstMsg)
    {
        return VOS_ERR;
    }

    PS_MEM_SET((VOS_UINT8 *)pstMsg + VOS_MSG_HEAD_LENGTH, 0x00, sizeof(VCVOICE_LOOP_REQ_STRU) - VOS_MSG_HEAD_LENGTH);

    /* ��д����Ϣ���� */
    pstMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->ulSenderPid     = WUEPS_PID_VC;
    pstMsg->ulSenderCpuId   = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverPid   = DSP_PID_VOICE;

    pstMsg->usMsgName       = ID_VC_VOICE_LOOP_REQ;
    pstMsg->usReserve1      = 0;
    pstMsg->enMode          = enVoiceLoop;
    pstMsg->usReserve2      = 0;

    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_VC, pstMsg))
    {
        return VOS_ERR;
    }

    return VOS_OK;

}


#if (FEATURE_ON == FEATURE_IMS)
VOS_UINT32  APP_VC_SendImsaExceptionNtf(
    VC_IMSA_EXCEPTION_CAUSE_ENUM_UINT32                     enCause
)
{
    VC_IMSA_HIFI_EXCEPTION_NTF_STRU    *pstMsg;

    pstMsg = (VC_IMSA_HIFI_EXCEPTION_NTF_STRU *)PS_ALLOC_MSG(WUEPS_PID_VC,
                                        sizeof(VC_IMSA_HIFI_EXCEPTION_NTF_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstMsg)
    {
        return VOS_ERR;
    }

    PS_MEM_SET(((VOS_UINT8*)pstMsg) + VOS_MSG_HEAD_LENGTH, 0, sizeof(VC_IMSA_HIFI_EXCEPTION_NTF_STRU) - VOS_MSG_HEAD_LENGTH);

    pstMsg->ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstMsg->ulSenderPid       = WUEPS_PID_VC;
    pstMsg->ulReceiverPid     = PS_PID_IMSA;
    pstMsg->ulSenderCpuId     = VOS_LOCAL_CPUID;
    pstMsg->ulLength          = sizeof(VC_IMSA_HIFI_EXCEPTION_NTF_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMsg->ulMsgName         = ID_VC_IMSA_HIFI_EXCEPTION_NTF;
    pstMsg->enCause           = enCause;

    /* ������Ϣ��IMSA */
    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_VC, pstMsg))
    {
        return VOS_ERR;
    }

    return VOS_OK;
}
#endif
VOS_VOID  APP_VC_LogEndCallReq(
    APP_VC_OPEN_CHANNEL_FAIL_CAUSE_ENUM_UINT32              enVcCause
)
{
    TAF_OAM_LOG_END_CALL_REQ_STRU      *pstMsg = VOS_NULL_PTR;

    pstMsg = (TAF_OAM_LOG_END_CALL_REQ_STRU*)PS_MEM_ALLOC(WUEPS_PID_VC,
                                         sizeof(TAF_OAM_LOG_END_CALL_REQ_STRU));

    if (VOS_NULL_PTR == pstMsg)
    {
        return;
    }

    PS_MEM_SET(((VOS_UINT8*)pstMsg) + VOS_MSG_HEAD_LENGTH, 0, sizeof(TAF_OAM_LOG_END_CALL_REQ_STRU) - VOS_MSG_HEAD_LENGTH);

    pstMsg->ulSenderCpuId   = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->ulSenderPid     = WUEPS_PID_VC;
    pstMsg->ulReceiverPid   = WUEPS_PID_VC;
    pstMsg->ulLength        = sizeof(TAF_OAM_LOG_END_CALL_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMsg->ulMsgId         = TAF_OAM_LOG_END_CALL_REQ;
    pstMsg->ulCause         = enVcCause;

    OM_TraceMsgHook(pstMsg);

    PS_MEM_FREE(WUEPS_PID_VC, pstMsg);

    return;
}


#ifdef  __cplusplus
  #if  __cplusplus
  }
  #endif
#endif


