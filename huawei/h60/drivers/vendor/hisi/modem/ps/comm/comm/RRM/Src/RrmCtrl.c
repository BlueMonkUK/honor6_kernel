



/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "RrmCtrl.h"
#include "RrmDebug.h"
#include "Rrm.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_RRM_CTRL_C

#if (FEATURE_ON == FEATURE_DSDS)
/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/* RRMģ�鴦������Э��ջ��Ϣ������Ӧ��*/
const RRM_MSG_PROC_STRU                 g_astRrmPsMsgProcTab[] =
{
    {ID_PS_RRM_RADIO_RESOURCE_APPLY_REQ,    RRM_RcvRadioResourceApplyReq},   /* ��Դ�������� */
    {ID_PS_RRM_RADIO_RESOURCE_RELEASE_IND,  RRM_RcvRadioResourceReleaseInd}, /* ��Դ�ͷ�֪ͨ */
    {ID_PS_RRM_REGISTER_IND,                RRM_RcvRegisterInd},             /* ע��֪ͨ */
    {ID_PS_RRM_DEREGISTER_IND,              RRM_RcvDeregisterInd}            /* ȥע��֪ͨ */
};


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_UINT32 RRM_CheckMsgParam
(
    MODEM_ID_ENUM_UINT16                    enModemId,
    RRM_PS_RAT_TYPE_ENUM_UINT8              enRatType
)
{
    /* MODEM ID �Ϸ��Լ�� */
    if (enModemId >= MODEM_ID_BUTT)
    {
        RRM_WARNING_LOG1(UEPS_PID_RRM, "enModemId is error.", enModemId);
        return VOS_ERR;
    }

    /* RAT type �Ϸ��Լ�� */
    if (enRatType >= RRM_PS_RAT_TYPE_BUTT)
    {
        RRM_WARNING_LOG1(UEPS_PID_RRM, "enRatType is error.", enRatType);
        return VOS_ERR;
    }

    return VOS_OK;
}



VOS_VOID RRM_SndPSStatusInd(VOS_UINT32  ulReceiverPid)
{
    VOS_UINT32                          ulLength;
    RRM_PS_STATUS_IND_STRU             *pstPSStatusInd = VOS_NULL_PTR;


    if (RRM_PID_DEFAULT_VALUE == ulReceiverPid)
    {
        RRM_ERROR_LOG(UEPS_PID_RRM, "ulReceiverPid is error!");
        return;
    }


    ulLength = sizeof(RRM_PS_STATUS_IND_STRU) - VOS_MSG_HEAD_LENGTH;

    /* ������Ϣ�ռ� */
    pstPSStatusInd = (RRM_PS_STATUS_IND_STRU *)PS_ALLOC_MSG(UEPS_PID_RRM, ulLength);
    if (VOS_NULL_PTR == pstPSStatusInd)
    {
        RRM_ERROR_LOG(UEPS_PID_RRM, "Alloc msg fail!");
        return;
    }

    /* ����Ϣ�ռ� */
    PS_MEM_SET((VOS_UINT8*)pstPSStatusInd + VOS_MSG_HEAD_LENGTH, 0, ulLength);

    /* �����Ϣ */
    pstPSStatusInd->stMsgHeader.ulReceiverPid   = ulReceiverPid;
    pstPSStatusInd->stMsgHeader.ulSenderPid     = UEPS_PID_RRM;
    pstPSStatusInd->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstPSStatusInd->stMsgHeader.ulSenderCpuId   = VOS_LOCAL_CPUID;
    pstPSStatusInd->stMsgHeader.ulMsgName       = ID_RRM_PS_STATUS_IND;
    pstPSStatusInd->stMsgHeader.ulLength        = sizeof(RRM_PS_RADIO_RESOURCE_APPLY_CNF_STRU) - VOS_MSG_HEAD_LENGTH;

    RRM_DBG_SND_STATUS_IND_NUM(1);

    /* ������Ϣ */
    if (VOS_OK != PS_SEND_MSG(UEPS_PID_RRM, pstPSStatusInd))
    {
        RRM_ERROR_LOG(UEPS_PID_RRM, "Snd Msg err.");
        return;
    }

    return;
}




VOS_VOID RRM_SndRadioResourceApplyCnf
(
    VOS_UINT32                          ulRecievePid,
    PS_RRM_RESULT_ENUM_UINT8            enResult
)
{
    VOS_UINT32                              ulLength;
    RRM_PS_RADIO_RESOURCE_APPLY_CNF_STRU   *pstRadioResourceApplyCnf = VOS_NULL_PTR;


    ulLength = sizeof(RRM_PS_RADIO_RESOURCE_APPLY_CNF_STRU) - VOS_MSG_HEAD_LENGTH;

    /* ������Ϣ�ռ� */
    pstRadioResourceApplyCnf = (RRM_PS_RADIO_RESOURCE_APPLY_CNF_STRU *)PS_ALLOC_MSG(UEPS_PID_RRM, ulLength);
    if (VOS_NULL_PTR == pstRadioResourceApplyCnf)
    {
        RRM_ERROR_LOG(UEPS_PID_RRM, "Alloc msg fail!");
        return;
    }

    /* ����Ϣ�ռ� */
    PS_MEM_SET((VOS_UINT8*)pstRadioResourceApplyCnf + VOS_MSG_HEAD_LENGTH, 0, ulLength);

    /* �����Ϣ */
    pstRadioResourceApplyCnf->stMsgHeader.ulReceiverPid     = ulRecievePid;
    pstRadioResourceApplyCnf->stMsgHeader.ulSenderPid       = UEPS_PID_RRM;
    pstRadioResourceApplyCnf->stMsgHeader.ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstRadioResourceApplyCnf->stMsgHeader.ulSenderCpuId     = VOS_LOCAL_CPUID;
    pstRadioResourceApplyCnf->stMsgHeader.ulMsgName         = ID_RRM_PS_RADIO_RESOURCE_APPLY_CNF;
    pstRadioResourceApplyCnf->stMsgHeader.ulLength          = sizeof(RRM_PS_RADIO_RESOURCE_APPLY_CNF_STRU) - VOS_MSG_HEAD_LENGTH;
    pstRadioResourceApplyCnf->enResult                      = enResult;


    /* ������Ϣ */
    if (VOS_OK != PS_SEND_MSG(UEPS_PID_RRM, pstRadioResourceApplyCnf))
    {
        RRM_ERROR_LOG(UEPS_PID_RRM, "Snd Msg err.");
        return;
    }

    return;

}



VOS_VOID RRM_RcvRadioResourceApplyReq(VOS_VOID * pstMsg)
{
    VOS_UINT32                          ulPid;
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;
    RRM_PS_RAT_TYPE_ENUM_UINT8          enRatType;
    VOS_UINT16                          usRFIDValue;
    PS_BOOL_ENUM_UINT8                  enSameModem;
    RRM_RFID_ENUM_UINT16                enRFIDIndex;
    PS_BOOL_ENUM_UINT8                  enRFIDIsUsed;
    PS_RRM_RADIO_RESOURCE_APPLY_REQ_STRU   *pstRadioResApplyReq;


    /* ������� */
    if (VOS_NULL_PTR == pstMsg)
    {
        RRM_WARNING_LOG(UEPS_PID_RRM, "pMsg is null.");
        return;
    }

    pstRadioResApplyReq = (PS_RRM_RADIO_RESOURCE_APPLY_REQ_STRU *)pstMsg;
    ulPid               = pstRadioResApplyReq->stMsgHeader.ulSenderPid;
    enModemId           = pstRadioResApplyReq->enModemId;
    RRM_DBG_MODEM_RX_RES_APPLY_REQ_NUM(enModemId, 1);
    if (PS_FALSE == RRM_GetModemSupportFlg(enModemId))
    {
        RRM_WARNING_LOG1(UEPS_PID_RRM, "modem don't support.", enModemId);
        RRM_SndRadioResourceApplyCnf(ulPid, PS_RRM_RESULT_FAIL);
        RRM_DBG_MODEM_SND_RES_APPLY_CNF_NUM(enModemId, 1);

        return;
    }

    enRatType   = pstRadioResApplyReq->enRatType;

    /* ��Ϣ�Ϸ��Լ�� */
    ulRslt  = RRM_CheckMsgParam(enModemId, enRatType);
    if (VOS_ERR == ulRslt)
    {
        RRM_WARNING_LOG(UEPS_PID_RRM, "Msg is invalid.");
        RRM_SndRadioResourceApplyCnf(ulPid, PS_RRM_RESULT_FAIL);
        RRM_DBG_MODEM_SND_RES_APPLY_CNF_NUM(enModemId, 1);

        return;
    }

    usRFIDValue = RRM_GetRFIDCfgValue(enModemId, enRatType);
    if (RRM_RFID_DEFAULT_VALUE == usRFIDValue)
    {
        RRM_WARNING_LOG2(UEPS_PID_RRM, "enModemId is <1>, enRatType is <2>.", enModemId, enRatType);
        RRM_SndRadioResourceApplyCnf(ulPid, PS_RRM_RESULT_FAIL);
        RRM_DBG_MODEM_SND_RES_APPLY_CNF_NUM(enModemId, 1);

        return;
    }

    /* RFID��Դ�Ƿ���ʹ���ж� */
    ulRslt  = RRM_JudgeRFIDIsUsed(enModemId, usRFIDValue, &enSameModem, &enRFIDIndex, &enRFIDIsUsed);
    if (VOS_ERR == ulRslt)
    {
        RRM_WARNING_LOG2(UEPS_PID_RRM, "enModemId is <1>, usRFIDValue is <2>.", enModemId, usRFIDValue);
        RRM_SndRadioResourceApplyCnf(ulPid, PS_RRM_RESULT_FAIL);
        RRM_DBG_MODEM_SND_RES_APPLY_CNF_NUM(enModemId, 1);

        return;
    }

    if (PS_TRUE == enRFIDIsUsed)
    {
        /* �Ƿ���ͬһMODEM������Ϣ */
        if (PS_TRUE == enSameModem)
        {
            /* ֹͣVOS��ʱ�� */
            RRM_StopTimer(enRFIDIndex);

            /* RFID��Դ��ǰMODEM����ʹ�ã�ͬһMODEM������Դ�ظ��ɹ� */
            RRM_SndRadioResourceApplyCnf(ulPid, PS_RRM_RESULT_SUCCESS);
            RRM_DBG_MODEM_SND_RES_APPLY_CNF_NUM(enModemId, 1);

            return;
        }
        else
        {
            RRM_WARNING_LOG(UEPS_PID_RRM, "It is not the same modem.");

            /* RFID��Դ��ǰMODEM����ʹ�ã�����MODEM������Դ�ظ�ʧ�� */
            RRM_SndRadioResourceApplyCnf(ulPid, PS_RRM_RESULT_FAIL);
            RRM_DBG_MODEM_SND_RES_APPLY_CNF_NUM(enModemId, 1);

            return;
        }
    }
    else
    {
        /*** RFID��Դû����ʹ�� ***/
        /* ���ø�RFID��ʹ�� */
        ulRslt = RRM_SetRFIDUsedInfo(PS_TRUE, usRFIDValue, enModemId);
        if (VOS_OK == ulRslt)
        {
            /* ��������Դ�ظ��ɹ� */
            RRM_SndRadioResourceApplyCnf(ulPid, PS_RRM_RESULT_SUCCESS);
            RRM_DBG_MODEM_SND_RES_APPLY_CNF_NUM(enModemId, 1);
        }
        else
        {
            /* ��������Դ�ظ�ʧ�� */
            RRM_SndRadioResourceApplyCnf(ulPid, PS_RRM_RESULT_FAIL);
            RRM_DBG_MODEM_SND_RES_APPLY_CNF_NUM(enModemId, 1);
        }

        return;
    }

}
VOS_VOID RRM_RcvRadioResourceReleaseInd(VOS_VOID * pstMsg)
{
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;
    RRM_PS_RAT_TYPE_ENUM_UINT8          enRatType;
    VOS_UINT16                          usRFIDValue;
    PS_BOOL_ENUM_UINT8                  enSameModem;
    RRM_RFID_ENUM_UINT16                enRFIDIndex;
    PS_BOOL_ENUM_UINT8                  enRFIDIsUsed;
    PS_RRM_RADIO_RESOURCE_RELEASE_IND_STRU   *pstRadioResRelInd;


    /* ������� */
    if (VOS_NULL_PTR == pstMsg)
    {
        RRM_WARNING_LOG(UEPS_PID_RRM, "pMsg is null.");
        return;
    }

    pstRadioResRelInd = (PS_RRM_RADIO_RESOURCE_RELEASE_IND_STRU *)pstMsg;
    enModemId   = pstRadioResRelInd->enModemId;
    RRM_DBG_MODEM_RX_RES_RELEASE_NUM(enModemId, 1);
    if (PS_FALSE == RRM_GetModemSupportFlg(enModemId))
    {
        RRM_WARNING_LOG1(UEPS_PID_RRM, "modem don't support.", enModemId);
        return;
    }

    enRatType   = pstRadioResRelInd->enRatType;
    /* ��Ϣ�Ϸ��Լ�� */
    ulRslt  = RRM_CheckMsgParam(enModemId, enRatType);
    if (VOS_ERR == ulRslt)
    {
        RRM_WARNING_LOG(UEPS_PID_RRM, "Msg is invalid.");
        return;
    }

    usRFIDValue = RRM_GetRFIDCfgValue(enModemId, enRatType);
    if (RRM_RFID_DEFAULT_VALUE == usRFIDValue)
    {
        RRM_WARNING_LOG2(UEPS_PID_RRM, "enModemId is <1>, enRatType is <2>.", enModemId, enRatType);
        return;
    }

    /* RFID��Դ�Ƿ���ʹ���ж� */
    ulRslt  = RRM_JudgeRFIDIsUsed(enModemId, usRFIDValue, &enSameModem, &enRFIDIndex, &enRFIDIsUsed);
    if (VOS_ERR == ulRslt)
    {
        RRM_WARNING_LOG2(UEPS_PID_RRM, "enModemId is <1>, usRFIDValue is <2>.", enModemId, usRFIDValue);
        return;
    }

    if (PS_TRUE == enRFIDIsUsed)
    {
        /* �Ƿ���ͬһMODEM������Ϣ */
        if (PS_TRUE == enSameModem)
        {
            /* ������ʱ�� */
            RRM_StartTimer(enRFIDIndex);
            return;
        }
        else
        {
            RRM_WARNING_LOG(UEPS_PID_RRM, "It is not the same modem.");
            return;
        }
    }
    else
    {
        /* RFID��Դû����ʹ�� */
        RRM_WARNING_LOG(UEPS_PID_RRM, "usRFIDValue is not used.");
        return;
    }

}



VOS_VOID RRM_RcvRegisterInd(VOS_VOID * pstMsg)
{
    VOS_UINT32                          ulPid;
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;
    RRM_PS_RAT_TYPE_ENUM_UINT8          enRatType;
    PS_BOOL_ENUM_UINT8                  enSameModem;
    VOS_UINT16                          usRFIDValue;
    VOS_UINT16                          usRegInfoIndex;
    PS_BOOL_ENUM_UINT8                  enRegInfoIsReged;
    PS_RRM_REGISTER_IND_STRU           *pstRegInd;


    /* ������� */
    if (VOS_NULL_PTR == pstMsg)
    {
        RRM_WARNING_LOG(UEPS_PID_RRM, "pMsg is null.");
        return;
    }

    pstRegInd   = (PS_RRM_REGISTER_IND_STRU *)pstMsg;
    enModemId   = pstRegInd->enModemId;
    RRM_DBG_MODEM_RX_REG_NUM(enModemId, 1);
    if (PS_FALSE == RRM_GetModemSupportFlg(enModemId))
    {
        RRM_WARNING_LOG1(UEPS_PID_RRM, "modem don't support.", enModemId);
        return;
    }

    enRatType   = pstRegInd->enRatType;

    /* ��Ϣ�Ϸ��Լ�� */
    ulRslt      = RRM_CheckMsgParam(enModemId, enRatType);
    if (VOS_ERR == ulRslt)
    {
        RRM_WARNING_LOG(UEPS_PID_RRM, "Msg is invalid.");
        return;
    }

    usRFIDValue = RRM_GetRFIDCfgValue(enModemId, enRatType);
    if (RRM_RFID_DEFAULT_VALUE == usRFIDValue)
    {
        RRM_WARNING_LOG2(UEPS_PID_RRM, "enModemId is <1>, enRatType is <2>.", enModemId, enRatType);
        return;
    }

    ulPid       = pstRegInd->stMsgHeader.ulSenderPid;
    /* REG INFO�Ƿ���ע���ж� */
    ulRslt  = RRM_JudgeRegInfoIsReged(enModemId, ulPid, &enSameModem, &usRegInfoIndex, &enRegInfoIsReged);
    if (VOS_ERR == ulRslt)
    {
        RRM_WARNING_LOG2(UEPS_PID_RRM, "enModemId is <1>, ulPid is <2>.", enModemId, ulPid);
        return;
    }

    if (PS_TRUE == enRegInfoIsReged)
    {
        if (PS_TRUE == enSameModem)
        {
            /* ��ͬһ��MODEM��������ע����Ϣ */
            RRM_WARNING_LOG(UEPS_PID_RRM, "It is the same modem.");
            return;
        }
        else
        {
            /* �ǲ�ͬMODEM��������ע����Ϣ */
            RRM_WARNING_LOG(UEPS_PID_RRM, "It is not the same modem.");
            return;
        }
    }
    else
    {
        /*** REG INFOû�б�ע�� ***/

        /* ����REG INFO */
        RRM_SetRegInfo(PS_TRUE, enRatType, enModemId, ulPid);
        return;
    }

}
VOS_VOID RRM_RcvDeregisterInd(VOS_VOID * pstMsg)
{
    VOS_UINT32                          ulPid;
    VOS_UINT32                          ulRslt;
    MODEM_ID_ENUM_UINT16                enModemId;
    RRM_PS_RAT_TYPE_ENUM_UINT8          enRatType;
    PS_BOOL_ENUM_UINT8                  enSameModem;
    VOS_UINT16                          usRFIDValue;
    VOS_UINT16                          usRegInfoIndex;
    PS_BOOL_ENUM_UINT8                  enRegInfoIsReged;
    PS_RRM_DEREGISTER_IND_STRU         *pstDeregInd;


    /* ������� */
    if (VOS_NULL_PTR == pstMsg)
    {
        RRM_WARNING_LOG(UEPS_PID_RRM, "pMsg is null.");
        return;
    }

    pstDeregInd   = (PS_RRM_DEREGISTER_IND_STRU *)pstMsg;
    enModemId   = pstDeregInd->enModemId;
    RRM_DBG_MODEM_RX_DEREG_NUM(enModemId, 1);
    if (PS_FALSE == RRM_GetModemSupportFlg(enModemId))
    {
        RRM_WARNING_LOG1(UEPS_PID_RRM, "modem don't support.", enModemId);
        return;
    }

    enRatType   = pstDeregInd->enRatType;
    /* ��Ϣ�Ϸ��Լ�� */
    ulRslt      = RRM_CheckMsgParam(enModemId, enRatType);
    if (VOS_ERR == ulRslt)
    {
        RRM_WARNING_LOG2(UEPS_PID_RRM, "Msg is invalid.enModemId is <1>, enRatType is <2>", enModemId, enRatType);
        return;
    }

    usRFIDValue = RRM_GetRFIDCfgValue(enModemId, enRatType);
    if (RRM_RFID_DEFAULT_VALUE == usRFIDValue)
    {
        RRM_WARNING_LOG2(UEPS_PID_RRM, "enModemId is <1>, enRatType is <2>.", enModemId, enRatType);
        return;
    }

    ulPid       = pstDeregInd->stMsgHeader.ulSenderPid;
    /* REG INFO�Ƿ���ע���ж� */
    ulRslt  = RRM_JudgeRegInfoIsReged(enModemId, ulPid, &enSameModem, &usRegInfoIndex, &enRegInfoIsReged);
    if (VOS_ERR == ulRslt)
    {
        RRM_WARNING_LOG2(UEPS_PID_RRM, "enModemId is <1>, ulPid is <2>.", enModemId, ulPid);
        return;
    }

    if (PS_TRUE == enRegInfoIsReged)
    {
        if (PS_TRUE == enSameModem)
        {
            /* ��ͬһ��MODEM��������ע����Ϣ, ���ע����Ϣ */
            RRM_ClearRegInfo(usRegInfoIndex);
            return;
        }
        else
        {
            /* �ǲ�ͬMODEM��������ע����Ϣ */
            RRM_WARNING_LOG(UEPS_PID_RRM, "It is not the same modem.");
            return;
        }
    }
    else
    {
        /*** REG INFOû�б�ע�ᣬ����Ϣ������ ***/
        RRM_WARNING_LOG(UEPS_PID_RRM, "Reg info is not exist.");
        return;
    }

}




VOS_VOID RRM_RcvTimeOutProc(RRM_RFID_ENUM_UINT16  enRFIDIndex)
{
    VOS_UINT16                          usIndex;
    VOS_UINT32                          ulRecievePid;
    RRM_REG_INFO_STRU                  *pstRegInfoAddr      = VOS_NULL_PTR;
    RRM_RFID_TIMER_CTRL_STRU           *pstRFIDTimerAddr    = VOS_NULL_PTR;

     /* ��ȡ��״̬��ʱ�����ƿ��ַ */
    pstRFIDTimerAddr       = RRM_GetRFIDTimerAddr(enRFIDIndex);

    /* �޷���ȡ�ö�ʱ�����ƿ��ַ��˵����ʱ��ID�Ƿ� */
    if( VOS_NULL_PTR == pstRFIDTimerAddr)
    {
        /* ��ӡ������Ϣ */
        RRM_ERROR_LOG1(UEPS_PID_RRM, "TimerId is Error!", (VOS_INT32)enRFIDIndex);
        return;
    }

    /* ��ֹ������ֹ֧ͣ��ʱ���󣬻���ĳ�ʱ�������Ϣ���յ��ĳ��� */
    if (RRM_RFID_BUTT == pstRFIDTimerAddr->enRFIDTimerId)
    {
        /* ��ӡ������Ϣ */
        RRM_WARNING_LOG1(UEPS_PID_RRM, "Timer is already stop!", (VOS_INT32)enRFIDIndex);
        return;
    }

    /* ���RF��Դ��Ϣ */
    RRM_ClearRFIDUsedInfo(enRFIDIndex);

    /* ע����Ϣ���� */
    for (usIndex = 0; usIndex < RRM_REG_MAX_NUMBER; usIndex++)
    {
        pstRegInfoAddr = RRM_GetRegInfoAddr(usIndex);
        if (VOS_NULL_PTR != pstRegInfoAddr)
        {
            if (PS_TRUE == pstRegInfoAddr->enRegisteredFlg)
            {
                ulRecievePid   = pstRegInfoAddr->ulPid;

                /* ����ע���Э��ջ������֪ͨ */
                RRM_SndPSStatusInd(ulRecievePid);
            }
        }
    }

}



VOS_VOID RRM_RcvTimerExpireMsg(REL_TIMER_MSG *pTimerMsg)
{
    if (pTimerMsg->ulName < RRM_RFID_BUTT)
    {
        RRM_RcvTimeOutProc((VOS_UINT16)pTimerMsg->ulName);
    }
    else
    {
        RRM_WARNING_LOG1(UEPS_PID_RRM, "TimerMsg name is error.", pTimerMsg->ulName);
    }

    return;
}



VOS_UINT32 RRM_HandleMsgTab
(
    VOS_UINT32                          ulMsgCnt,
    VOS_VOID                           *pstMsg,
    const RRM_MSG_PROC_STRU            *pstMsgProcTab
)
{
    VOS_UINT32                          ulLoop;
    VOS_UINT32                          ulMsgName;


    /* ����Ϣ���л�ȡMSG ID */
    ulMsgName  = ((MSG_HEADER_STRU*)pstMsg)->ulMsgName;

    /* ���������Ϣ�ַ� */
    for (ulLoop = 0; ulLoop < ulMsgCnt; ulLoop++)
    {
        if (pstMsgProcTab[ulLoop].ulMsgType == ulMsgName)
        {
            pstMsgProcTab[ulLoop].pProcMsgFunc(pstMsg);
            break;
        }
    }

    /* û���ҵ�ƥ�����Ϣ */
    if (ulMsgCnt == ulLoop)
    {
        RRM_WARNING_LOG1(UEPS_PID_RRM, "RRM_SearchMsgProcTab: Mismatch Msg, ulMsgName is error.", (VOS_INT32)ulMsgName);
        return VOS_ERR;
    }


    return VOS_OK;
}
VOS_VOID RRM_RcvPSMsg(MsgBlock *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulResult;


    /* ��ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astRrmPsMsgProcTab) / sizeof(RRM_MSG_PROC_STRU);

    /* ���������Ϣ�ַ�*/
    ulResult = RRM_HandleMsgTab(ulMsgCnt, pstMsg, g_astRrmPsMsgProcTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulResult)
    {
        RRM_ERROR_LOG1(UEPS_PID_RRM, "MTC_RcvPSMsg, ulResult is false, ulMsgCnt is error.", (VOS_INT32)ulMsgCnt);
    }


    return;
}
VOS_VOID  RRM_MsgProc(MsgBlock * pstMsg)
{
    if (VOS_NULL_PTR == pstMsg)
    {
        RRM_ERROR_LOG(UEPS_PID_RRM, "RRM_MsgProc, pMsg is NULL.");
        return;
    }

    /* ��ʱ����ʱ��Ϣ */
    if (VOS_PID_TIMER == pstMsg->ulSenderPid)
    {
        RRM_RcvTimerExpireMsg((REL_TIMER_MSG *)pstMsg);
    }
    else
    {
        /* Э��ջ���񷢹�������Ϣ */
        RRM_RcvPSMsg(pstMsg);
    }

    return;
}





#endif /* FEATURE_ON == FEATURE_DSDS */

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

