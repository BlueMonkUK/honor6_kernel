

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "pslog.h"
#include "PsCommonDef.h"
#include "rabminclude.h"
#include "NasRabmMain.h"
#include "NasRabmMsgProc.h"
#include "AtRabmInterface.h"

#include "NasUtranCtrlCommFunc.h"

#include "NasNvInterface.h"
#include "TafNvInterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#define    THIS_FILE_ID        PS_FILE_ID_NAS_RABM_MGR_C



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/* ��ǰϵͳģʽ */
GMM_RABM_NET_RAT_ENUM_UINT32            g_enNasRabmSysMode;

/* ����ϵͳģʽ�ĺ�������ָ��� */
NAS_RABM_PROC_FUNC_TBL_STRU            *g_pstNasRabmProcFuncTbl[NAS_MML_NET_RAT_TYPE_BUTT];

NAS_RABM_FASTDORM_CTX_STRU              g_stNasRabmFastDormCtx;

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_VOID NAS_RABM_RegProcFuncTbl(
    GMM_RABM_NET_RAT_ENUM_UINT32         enSysMode,
    NAS_RABM_PROC_FUNC_TBL_STRU        *pstProcFucTbl
)
{
    /* ϵͳģʽ��Ч�Լ�� */
    if (NAS_MML_NET_RAT_TYPE_BUTT == enSysMode)
    {
        NAS_ERROR_LOG(WUEPS_PID_RABM,
            "NAS_RABM_RegProcFuncTbl: Wrong system mode.");
        return;
    }

    NAS_RABM_SetProFuncTblAddr(enSysMode, pstProcFucTbl);

}
VOS_VOID NAS_RABM_CtxInit(VOS_VOID)
{
    VOS_UINT32                          i;

#if (FEATURE_ON == FEATURE_LTE)
    /* ��ʼ��ϵͳģʽ: Ĭ��ΪLTE */
    NAS_RABM_SetSysMode(NAS_MML_NET_RAT_TYPE_LTE);
#else
    /* ��ʼ��ϵͳģʽ: Ĭ��ΪWCDMA */
    NAS_RABM_SetSysMode(NAS_MML_NET_RAT_TYPE_WCDMA);
#endif

    /* ��ʼ����������ָ���ΪNULL */
    for ( i = 0; i < NAS_MML_NET_RAT_TYPE_BUTT; i++)
    {
        NAS_RABM_SetProFuncTblAddr(i, VOS_NULL_PTR);
    }

    /* ��ʼ��RABMʵ�� */
    Rabm_Init();
    NAS_Rabm2GInit();

    /* ��ʼ�������־ */
    NAS_RABM_ClearDataSuspendFlg();

    /* ��ʼ���������� */
    NAS_RABM_InitProcFuncTbl();

}


VOS_VOID NAS_RABM_MsgProc(
    struct MsgCB                       *pMsg
)
{
    MSG_HEADER_STRU                    *pstMsg;
    struct MsgCB                       *pstDestMsg;


    pstDestMsg = VOS_NULL_PTR;

    /* �����Ϣ�ǿռ�� */
    if (VOS_NULL_PTR == pMsg)
    {
        NAS_WARNING_LOG(WUEPS_PID_RABM,
            "NAS_RABM_MsgProc: Message is NULL.");
        return;
    }

    /* ֧��TD-SCDMA����ʱ�������Ϣ�������䴦����TDģ�ķ���PID����ΪWģ��PID */
    if ( VOS_TRUE == NAS_UTRANCTRL_MsgProc(pMsg, &pstDestMsg) )
    {
        /* ��Ϣ��UTRANCTRLģ�鴦����ɣ�ֱ�ӷ��أ�������������� */
        return;
    }

    /* �滻��Ϣָ�� */
    pstMsg = (MSG_HEADER_STRU*)pstDestMsg;

    /* ������Ϣ */
    switch (pstMsg->ulSenderPid)
    {
        case WUEPS_PID_SM:
            NAS_RABM_RcvSmMsg(pstDestMsg);
            break;

        case WUEPS_PID_GMM:
            NAS_RABM_RcvGmmMsg(pstDestMsg);
            break;

        case WUEPS_PID_AT:
            NAS_RABM_RcvAtMsg(pstDestMsg);
            break;

        case UEPS_PID_CDS:
            NAS_RABM_RcvCdsMsg(pstDestMsg);
            break;

        default:
            NAS_RABM_RcvCommMsg(pstDestMsg);
            break;
    }
}


VOS_VOID NAS_RABM_RcvSmMsg(
    struct MsgCB                       *pMsg
)
{
    MSG_HEADER_STRU                    *pstMsg;

    pstMsg = (MSG_HEADER_STRU*)pMsg;

    switch (pstMsg->ulMsgName)
    {
        case RABMSM_ACTIVATE_IND:
            NAS_RABM_RcvSmActivateInd((RABMSM_ACTIVATE_IND_STRU*)pMsg);
            break;

        case RABMSM_DEACTIVATE_IND:
            NAS_RABM_RcvSmDeactivateInd((RABMSM_DEACTIVATE_IND_STRU*)pMsg);
            break;

        case RABMSM_MODIFY_IND:
            NAS_RABM_RcvSmModifyInd((RABMSM_MODIFY_IND_STRU*)pMsg);
            break;

#if (FEATURE_ON == FEATURE_LTE)
        case ID_SM_RABM_BEARER_ACTIVATE_IND:
            NAS_RABM_RcvSmBearerActivateInd((SM_RABM_BEARER_ACTIVATE_IND_STRU*)pMsg);
            break;

        case ID_SM_RABM_BEARER_DEACTIVATE_IND:
            NAS_RABM_RcvSmBearerDeactivateInd((SM_RABM_BEARER_DEACTIVATE_IND_STRU*)pMsg);
            break;

        case ID_SM_RABM_BEARER_MODIFY_IND:
            NAS_RABM_RcvSmBearerModifyInd((SM_RABM_BEARER_MODIFY_IND_STRU*)pMsg);
            break;
#endif

        default:
            NAS_WARNING_LOG(WUEPS_PID_RABM,
                "NAS_RABM_RcvSmMsg: Unknown message from SM.");
            break;
    }
}


VOS_VOID NAS_RABM_RcvGmmMsg(
    struct MsgCB                       *pMsg
)
{
    MSG_HEADER_STRU                    *pstMsg;

    pstMsg = (MSG_HEADER_STRU*)pMsg;

    switch (pstMsg->ulMsgName)
    {
        case ID_GMM_RABM_REESTABLISH_CNF:
            NAS_RABM_RcvGmmReestablishCnf((GMMRABM_REESTABLISH_CNF_STRU*)pMsg);
            break;

        case ID_GMM_RABM_SYS_SRV_CHG_IND:
            NAS_RABM_RcvGmmSysSrvChgInd((GMM_RABM_SYS_SRV_CHG_IND_STRU*)pMsg);
            break;

        case ID_GMM_RABM_MML_PROC_STATUS_QRY_CNF:
            NAS_RABM_RcvGmmMmlProcStatusQryCnf(pMsg);
            break;

        default:
            NAS_RABM_RcvCommMsg(pMsg);
            break;
    }
}


VOS_VOID NAS_RABM_RcvSmActivateInd(
    RABMSM_ACTIVATE_IND_STRU           *pstSmActivateInd
)
{
    NAS_RABM_PROC_FUNC_TBL_STRU        *pstProcFuncTbl;
    GMM_RABM_NET_RAT_ENUM_UINT32        enCurrentSysMode;

    /* �жϵ�ǰ��ϵͳģʽ�Ƿ���Ч */
    enCurrentSysMode = NAS_RABM_GetCurrentSysMode();
    if (NAS_MML_NET_RAT_TYPE_BUTT == enCurrentSysMode)
    {
        NAS_ERROR_LOG(WUEPS_PID_RABM,
            "NAS_RABM_RcvSmActivateInd: Invalid System Mode.");
        return;
    }

    /* ���ݵ�ǰϵͳģʽ��ȡ��Ӧ�Ĵ��������ָ�� */
    pstProcFuncTbl = NAS_RABM_GetProFuncTblAddr(enCurrentSysMode);

    /* ȷ�Ϻ���������Ѿ�ע�� */
    if (VOS_NULL_PTR != pstProcFuncTbl)
    {
        if (VOS_NULL_PTR != pstProcFuncTbl->pSmActivateIndProcFunc)
        {
            /* ���ö�Ӧģʽ�Ĵ����� */
            pstProcFuncTbl->pSmActivateIndProcFunc(pstSmActivateInd);
        }
        else
        {
            NAS_WARNING_LOG(WUEPS_PID_RABM,
                "NAS_RABM_RcvSmActivateInd: pSmActivateIndProcFunc is not registered.");
        }
    }
    else
    {
        NAS_ERROR_LOG(WUEPS_PID_RABM,
            "NAS_RABM_RcvSmActivateInd: Msg is received in wrong system mode.");
    }
}


VOS_VOID NAS_RABM_RcvSmDeactivateInd(
    RABMSM_DEACTIVATE_IND_STRU         *pstSmDeactivateInd
)
{
    NAS_RABM_PROC_FUNC_TBL_STRU        *pstProcFuncTbl;
    GMM_RABM_NET_RAT_ENUM_UINT32        enCurrentSysMode;

    /* �жϵ�ǰ��ϵͳģʽ�Ƿ���Ч */
    enCurrentSysMode = NAS_RABM_GetCurrentSysMode();
    if (NAS_MML_NET_RAT_TYPE_BUTT == enCurrentSysMode)
    {
        NAS_ERROR_LOG(WUEPS_PID_RABM,
            "NAS_RABM_RcvSmDeactivateInd: Invalid System Mode.");
        return;
    }

    /* ���ݵ�ǰϵͳģʽ��ȡ��Ӧ�Ĵ��������ָ�� */
    pstProcFuncTbl = NAS_RABM_GetProFuncTblAddr(enCurrentSysMode);

    /* ȷ�Ϻ���������Ѿ�ע�� */
    if (VOS_NULL_PTR != pstProcFuncTbl)
    {
        if (VOS_NULL_PTR != pstProcFuncTbl->pSmDeactivateIndProcFunc)
        {
            /* ���ö�Ӧģʽ�Ĵ����� */
            pstProcFuncTbl->pSmDeactivateIndProcFunc(pstSmDeactivateInd);
        }
        else
        {
            NAS_WARNING_LOG(WUEPS_PID_RABM,
                "NAS_RABM_RcvSmDeactivateInd: pSmDeactivateIndProcFunc is not registered.");
        }
    }
    else
    {
        NAS_ERROR_LOG(WUEPS_PID_RABM,
            "NAS_RABM_RcvSmDeactivateInd: Msg is received in wrong system mode.");
    }
}


VOS_VOID NAS_RABM_RcvSmModifyInd(
    RABMSM_MODIFY_IND_STRU             *pstSmModifyInd
)
{
    NAS_RABM_PROC_FUNC_TBL_STRU        *pstProcFuncTbl;
    GMM_RABM_NET_RAT_ENUM_UINT32        enCurrentSysMode;

    /* �жϵ�ǰ��ϵͳģʽ�Ƿ���Ч */
    enCurrentSysMode = NAS_RABM_GetCurrentSysMode();
    if (NAS_MML_NET_RAT_TYPE_BUTT == enCurrentSysMode)
    {
        NAS_ERROR_LOG(WUEPS_PID_RABM,
            "NAS_RABM_RcvSmModifyInd: Invalid System Mode.");
        return;
    }

    /* ���ݵ�ǰϵͳģʽ��ȡ��Ӧ�Ĵ��������ָ�� */
    pstProcFuncTbl = NAS_RABM_GetProFuncTblAddr(enCurrentSysMode);

    /* ȷ�Ϻ���������Ѿ�ע�� */
    if (VOS_NULL_PTR != pstProcFuncTbl)
    {
        if (VOS_NULL_PTR != pstProcFuncTbl->pSmModifyIndProcFunc)
        {
            /* ���ö�Ӧģʽ�Ĵ����� */
            pstProcFuncTbl->pSmModifyIndProcFunc(pstSmModifyInd);
        }
        else
        {
            NAS_WARNING_LOG(WUEPS_PID_RABM,
                "NAS_RABM_RcvSmModifyInd: pSmModifyIndProcFunc is not registered.");
        }
    }
    else
    {
        NAS_ERROR_LOG(WUEPS_PID_RABM,
            "NAS_RABM_RcvSmModifyInd: Msg is received in wrong system mode.");
    }
}

#if (FEATURE_ON == FEATURE_LTE)

VOS_VOID NAS_RABM_RcvSmBearerActivateInd(
    SM_RABM_BEARER_ACTIVATE_IND_STRU   *pstSmBearerActivateInd
)
{
    RABM_ENTITY_PS_STRU                *pstPsEnt;
    VOS_UINT8                           ucEntId;

    /* ��ȡPS��RABMʵ��ָ�� */
    ucEntId  = (VOS_UINT8)(pstSmBearerActivateInd->ulNsapi - RABM_NSAPI_OFFSET);
    pstPsEnt = NAS_RABM_GetWPsEntAddr(ucEntId);

    /* ����QOS */
    pstPsEnt->QoS.ulQosLength   = pstSmBearerActivateInd->stQos.ulLength;
    PS_MEM_CPY(pstPsEnt->QoS.aucQosValue,
               pstSmBearerActivateInd->stQos.aucQosValue,
               NAS_RABM_MAX_QOS_LEN);

    /* ����PPP��ʶ */
    pstPsEnt->ucPppFlg          = RABM_SM_IP_PROT;

    /* ����RAB_MAPʵ�� */
    NAS_RABM_CreateRabMapEntity((VOS_UINT8)pstSmBearerActivateInd->ulNsapi,
                                (VOS_UINT8)pstSmBearerActivateInd->ulLinkdNsapi,
                                (VOS_UINT8)pstSmBearerActivateInd->ulNsapi);

    /* ����RABM״̬������״̬ */
    RABM_SetWState(ucEntId, RABM_DATA_TRANSFER_STOP);

    return;
}
VOS_VOID NAS_RABM_RcvSmBearerModifyInd(
    SM_RABM_BEARER_MODIFY_IND_STRU     *pstSmBearerModifyInd
)
{
    RABM_ENTITY_PS_STRU                *pstPsEnt;
    VOS_UINT8                           ucEntId;

    /* ��ȡPS��RABMʵ������ */
    ucEntId = (VOS_UINT8)(pstSmBearerModifyInd->ulNsapi - RABM_NSAPI_OFFSET);

    /* ���PS��RABMʵ��״̬�Ƿ���ȷ */
    if (RABM_NULL == NAS_RABM_GetWPsEntState(ucEntId))
    {
        NAS_ERROR_LOG1(WUEPS_PID_RABM,
            "NAS_RABM_RcvSmBearerModifyInd: Wrong state of RABM entity. NSAPI:",
            pstSmBearerModifyInd->ulNsapi);
        return;
    }

    /* ��ȡPS��RABMʵ��ָ�� */
    pstPsEnt = NAS_RABM_GetWPsEntAddr(ucEntId);

    /* ����QOS */
    pstPsEnt->QoS.ulQosLength = pstSmBearerModifyInd->stQos.ulLength;
    PS_MEM_CPY(pstPsEnt->QoS.aucQosValue,
               pstSmBearerModifyInd->stQos.aucQosValue,
               NAS_RABM_MAX_QOS_LEN);

    return;
}


VOS_VOID NAS_RABM_RcvSmBearerDeactivateInd(
    SM_RABM_BEARER_DEACTIVATE_IND_STRU *pstSmBearerDeactivateInd
)
{
    VOS_UINT8                           ucEntId;

    /* ��ȡPS��RABMʵ��ָ�� */
    ucEntId = (VOS_UINT8)(pstSmBearerDeactivateInd->ulNsapi - RABM_NSAPI_OFFSET);

    if (NAS_MML_NET_RAT_TYPE_GSM == NAS_RABM_GetCurrentSysMode())
    {
        /* Gʵ��״̬��ΪRABM_2G_NULL */
        RABM_SetGState((RABM_NSAPI_ENUM)ucEntId, RABM_2G_NULL);

        /* ���Gʵ����Ϣ */
        NAS_RABM_ClearRabmGPsEnt(ucEntId);
    }
    else
    {
        /* Wʵ��״̬��ΪRABM_NULL */
        RABM_SetWState(ucEntId, RABM_NULL);

        /* ���Gʵ����Ϣ */
        NAS_RABM_ClearRabmWPsEnt(ucEntId);
    }

    /* ɾ��RAB_MAPʵ�� */
    RABM_DelRabMap((VOS_UINT8)pstSmBearerDeactivateInd->ulNsapi);
}
#endif
VOS_VOID NAS_RABM_RcvGmmReestablishCnf(
    GMMRABM_REESTABLISH_CNF_STRU       *pstGmmReestablishCnf
)
{
    NAS_RABM_PROC_FUNC_TBL_STRU        *pstProcFuncTbl;
    GMM_RABM_NET_RAT_ENUM_UINT32        enCurrentSysMode;

    /* �жϵ�ǰ��ϵͳģʽ�Ƿ���Ч */
    enCurrentSysMode = NAS_RABM_GetCurrentSysMode();
    if (NAS_MML_NET_RAT_TYPE_BUTT == enCurrentSysMode)
    {
        NAS_ERROR_LOG(WUEPS_PID_RABM,
            "NAS_RABM_RcvGmmReestablishCnf: Invalid System Mode.");
        return;
    }

    /* ���ݵ�ǰϵͳģʽ��ȡ��Ӧ�Ĵ��������ָ�� */
    pstProcFuncTbl = NAS_RABM_GetProFuncTblAddr(enCurrentSysMode);

    /* ȷ�Ϻ���������Ѿ�ע�� */
    if (VOS_NULL_PTR != pstProcFuncTbl)
    {
        if (VOS_NULL_PTR != pstProcFuncTbl->pGmmReestablishCnfProcFunc)
        {
            /* ���ö�Ӧģʽ�Ĵ����� */
            pstProcFuncTbl->pGmmReestablishCnfProcFunc(pstGmmReestablishCnf);
        }
        else
        {
            NAS_WARNING_LOG(WUEPS_PID_RABM,
                "NAS_RABM_RcvGmmReestablishCnf: pGmmReestablishCnfProcFunc is not registered.");
        }
    }
    else
    {
        NAS_ERROR_LOG(WUEPS_PID_RABM,
            "NAS_RABM_RcvGmmReestablishCnf: Message is received in wrong system mode.");
    }
}


VOS_VOID NAS_RABM_RcvGmmSysSrvChgInd(
    GMM_RABM_SYS_SRV_CHG_IND_STRU      *pstGmmSysSrvChgInd
)
{
    GMM_RABM_NET_RAT_ENUM_UINT32        enOldSysMode;

    /* �жϵ�ǰ��ϵͳģʽ�Ƿ���Ч */
    enOldSysMode = NAS_RABM_GetCurrentSysMode();

    /* ���ݹ���ָʾ��־�Ƿ�����, ִ�й����ָ����� */
    if (VOS_TRUE == pstGmmSysSrvChgInd->bDataSuspendFlg)
    {
        NAS_RABM_RcvDataSuspendInd(pstGmmSysSrvChgInd->bRatChangeFlg);
        NAS_RABM_RcvSysModeChgInd(pstGmmSysSrvChgInd->enSysMode);
    }
    else
    {
        NAS_RABM_RcvSysModeChgInd(pstGmmSysSrvChgInd->enSysMode);

        /* GMM֪ͨRABMģ����лָ�ʱ��Ҫ֪ͨRABMģ���Ƿ���Ҫ����RAB�Ļָ���
           RABMģ����лָ�ʱ���ж��Ƿ���Ҫ�ȴ���������RAB�ָ� */
        NAS_RABM_RcvDataResumeInd(pstGmmSysSrvChgInd->ucRebuildRabFlag);
    }

    NAS_RABM_SysModeChgProcFastDorm(enOldSysMode,pstGmmSysSrvChgInd->enSysMode);
}


VOS_VOID NAS_RABM_RcvDataSuspendInd(
    VOS_BOOL                            bRatChangeFlg
)
{
    NAS_RABM_PROC_FUNC_TBL_STRU        *pstProcFuncTbl;
    GMM_RABM_NET_RAT_ENUM_UINT32        enCurrentSysMode;

    /* �жϵ�ǰ��ϵͳģʽ�Ƿ���Ч */
    enCurrentSysMode = NAS_RABM_GetCurrentSysMode();
    if (NAS_MML_NET_RAT_TYPE_BUTT == enCurrentSysMode)
    {
        NAS_ERROR_LOG(WUEPS_PID_RABM,
            "NAS_RABM_RcvDataSuspendInd: Invalid System Mode.");
        return;
    }

    /* ���ݵ�ǰϵͳģʽ��ȡ��Ӧ�Ĵ��������ָ�� */
    pstProcFuncTbl = NAS_RABM_GetProFuncTblAddr(enCurrentSysMode);

    /* ȷ�Ϻ���������Ѿ�ע�� */
    if (VOS_NULL_PTR != pstProcFuncTbl)
    {
        if (VOS_NULL_PTR != pstProcFuncTbl->pDataSuspendIndProcFunc)
        {
            /* ���ö�Ӧģʽ�Ĵ����� */
            pstProcFuncTbl->pDataSuspendIndProcFunc(bRatChangeFlg);
        }
        else
        {
            NAS_WARNING_LOG(WUEPS_PID_RABM,
                "NAS_RABM_RcvDataSuspendInd: pDataSuspendIndProcFunc is not registered.");
        }
    }
    else
    {
        NAS_ERROR_LOG(WUEPS_PID_RABM,
            "NAS_RABM_RcvDataSuspendInd: Message is received in wrong system mode.");
    }
}


VOS_VOID NAS_RABM_RcvDataResumeInd(
    VOS_UINT8                           ucRebuildRabFlag
)
{
    NAS_RABM_PROC_FUNC_TBL_STRU        *pstProcFuncTbl;
    GMM_RABM_NET_RAT_ENUM_UINT32        enCurrentSysMode;

    /* �жϵ�ǰ��ϵͳģʽ�Ƿ���Ч */
    enCurrentSysMode = NAS_RABM_GetCurrentSysMode();
    if (NAS_MML_NET_RAT_TYPE_BUTT == enCurrentSysMode)
    {
        NAS_ERROR_LOG(WUEPS_PID_RABM,
            "NAS_RABM_RcvDataResumeInd: Invalid System Mode.");
        return;
    }

    /* ���ݵ�ǰϵͳģʽ��ȡ��Ӧ�Ĵ��������ָ�� */
    pstProcFuncTbl = NAS_RABM_GetProFuncTblAddr(enCurrentSysMode);

    /* ȷ�Ϻ���������Ѿ�ע�� */
    if (VOS_NULL_PTR != pstProcFuncTbl)
    {
        if (VOS_NULL_PTR != pstProcFuncTbl->pDataResumeIndProcFunc)
        {
            /* ���ö�Ӧģʽ�Ĵ����� */
            pstProcFuncTbl->pDataResumeIndProcFunc(ucRebuildRabFlag);
        }
        else
        {
            NAS_WARNING_LOG(WUEPS_PID_RABM,
                "NAS_RABM_RcvDataResumeInd: pDataResumeIndProcFunc is not registered.");
        }
    }
    else
    {
        NAS_ERROR_LOG(WUEPS_PID_RABM,
            "NAS_RABM_RcvDataResumeInd: Message is received in wrong system mode.");
    }
}


VOS_VOID NAS_RABM_RcvSysModeChgInd(
    GMM_RABM_NET_RAT_ENUM_UINT32        enNewSysMode
)
{
    NAS_RABM_PROC_FUNC_TBL_STRU        *pstProcFuncTbl;
    GMM_RABM_NET_RAT_ENUM_UINT32        enOldSysMode;

    /* �жϵ�ǰ��ϵͳģʽ�Ƿ���Ч */
    enOldSysMode = NAS_RABM_GetCurrentSysMode();

    /* ���ϵͳģʽ��Ч��, ����Ч, ������ϵͳģʽ */
    if (NAS_MML_NET_RAT_TYPE_BUTT == enNewSysMode)
    {
        NAS_ERROR_LOG(WUEPS_PID_RABM,
            "NAS_RABM_RcvSysModeChgInd: Invalid System Mode.");
        return;
    }

    /* ����ϵͳģʽ */
    NAS_RABM_SetSysMode(enNewSysMode);

    /* ���ݵ�ǰϵͳģʽ��ȡ��Ӧ�Ĵ��������ָ�� */
    pstProcFuncTbl = NAS_RABM_GetProFuncTblAddr(enOldSysMode);

    /* ȷ�Ϻ���������Ѿ�ע�� */
    if (VOS_NULL_PTR != pstProcFuncTbl)
    {
        if (VOS_NULL_PTR != pstProcFuncTbl->pSysModeChgIndProcFunc)
        {
            /* ���ö�Ӧģʽ�Ĵ����� */
            pstProcFuncTbl->pSysModeChgIndProcFunc(enOldSysMode, enNewSysMode);
        }
        else
        {
            NAS_WARNING_LOG(WUEPS_PID_RABM,
                "NAS_RABM_RcvSysModeChgInd: pDataResumeIndProcFunc is not registered.");
        }
    }
    else
    {
        NAS_WARNING_LOG(WUEPS_PID_RABM,
            "NAS_RABM_RcvSysModeChgInd: Message is received in wrong system mode.");
    }
}


VOS_VOID NAS_RABM_RcvCommMsg(
    struct MsgCB                       *pMsg
)
{
    NAS_RABM_PROC_FUNC_TBL_STRU        *pstProcFuncTbl;
    GMM_RABM_NET_RAT_ENUM_UINT32        enCurrentSysMode;

    /* �жϵ�ǰ��ϵͳģʽ�Ƿ���Ч */
    enCurrentSysMode = NAS_RABM_GetCurrentSysMode();
    if (NAS_MML_NET_RAT_TYPE_BUTT == enCurrentSysMode)
    {
        NAS_ERROR_LOG(WUEPS_PID_RABM,
            "NAS_RABM_ProcSysSrvChgInd: Invalid System Mode.");
        return;
    }

    /* ���ݵ�ǰϵͳģʽ��ȡ��Ӧ�Ĵ��������ָ�� */
    pstProcFuncTbl = NAS_RABM_GetProFuncTblAddr(enCurrentSysMode);
    if (VOS_NULL_PTR != pstProcFuncTbl)
    {
        if (VOS_NULL_PTR != pstProcFuncTbl->pTaskEntry)
        {
            /* ���ö�Ӧģʽ�Ĵ����� */
            pstProcFuncTbl->pTaskEntry(pMsg);
        }
        else
        {
            NAS_WARNING_LOG(WUEPS_PID_RABM,
                "NAS_RABM_ProcessCommMsg: TaskEntry is not registered.");
        }
    }
    else
    {
        NAS_ERROR_LOG(WUEPS_PID_RABM,
            "NAS_RABM_ProcessCommMsg: Msg is received in wrong system mode.");
    }
}
VOS_UINT32    NAS_RABM_IsPdpActiveInWCDMA(VOS_VOID)
{
    VOS_UINT32                          ulEntId;
    VOS_UINT8                           ucState;

    for (ulEntId = 0; ulEntId < RABM_3G_PS_MAX_ENT_NUM; ulEntId++)
    {
        ucState = NAS_RABM_GetWPsEntState(ulEntId);
        if ( (RABM_NSAPI_ACTIVE_NO_RAB == ucState)
          || (RABM_NSAPI_ACTIVE_WITH_RAB == ucState)
          || (RABM_DATA_TRANSFER_STOP == ucState) )
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}

VOS_VOID NAS_RABM_ConvertAtFasTDormTypeToWrrFasTDormType(
    AT_RABM_FASTDORM_OPERATION_ENUM_UINT32                enAtFastDormType,
    RRC_FASTDORM_START_TYPE_ENUM_UINT32                 *penWrrFastDormType
)
{
    switch(enAtFastDormType)
    {
        case AT_RABM_FASTDORM_START_FD_ONLY:
            *penWrrFastDormType = RRC_FASTDORM_START_TYPE_FD;
            break;

        case AT_RABM_FASTDORM_START_ASCR_ONLY:
            *penWrrFastDormType = RRC_FASTDORM_START_TYPE_ASCR;
            break;

        case AT_RABM_FASTDORM_START_FD_ASCR:
            *penWrrFastDormType = RRC_FASTDORM_START_TYPE_FD_ASCR;
            break;

        default:
            NAS_WARNING_LOG(WUEPS_PID_RABM,
                        "NAS_RABM_ConvertAtFasTDormTypeToWrrFasTDormType: Wrong Type.");

            break;

    }
}



VOS_VOID NAS_RABM_RcvSetFastDormParaReq(
    AT_RABM_SET_FASTDORM_PARA_REQ_STRU *pstFastDormPara
)
{
    NAS_RABM_NVIM_FASTDORM_ENABLE_FLG_STRU                  stEnableFlg;
    VOS_UINT32                                              ulLength;

    ulLength                 = 0;
    stEnableFlg.ucEnableFlag = VOS_FALSE;

    /* ��NVʧ�ܣ���Ϊ������Fastdormancy */
    NV_GetLength(en_NV_Item_Fastdorm_Enable_Flag, &ulLength);

    if (NV_OK != NV_Read(en_NV_Item_Fastdorm_Enable_Flag, &stEnableFlg,
                          ulLength))
    {
        NAS_WARNING_LOG(WUEPS_PID_RABM, "NAS_RABM_RcvSetFastDormParaReq:WARNING:NV_Read faild!");
    }

    /* ������Fastdormancy */
    if (VOS_FALSE == stEnableFlg.ucEnableFlag)
    {
        /* �ظ���AT ID_RABM_AT_FASTDORM_START_CNF */
        NAS_RABM_SndAtSetFastDormParaCnf(pstFastDormPara->usClientId,
                                         pstFastDormPara->ucOpId,
                                         AT_RABM_PARA_SET_RSLT_FAIL);

        return;
    }

    if (AT_RABM_FASTDORM_STOP_FD_ASCR == pstFastDormPara->stFastDormPara.enFastDormOperationType)
    {
        /* ���ú�����ֹͣFAST DORMANCY */
        NAS_RABM_RcvAtFastDormStopReq(pstFastDormPara);
    }
    else
    {
        /* ���ú��� ������������FAST DORMANCY */
        NAS_RABM_RcvAtFastDormStartReq(pstFastDormPara);
    }
}
VOS_VOID NAS_RABM_RcvGetFastDormParaReq(
    AT_RABM_QRY_FASTDORM_PARA_REQ_STRU *pstFastDormPara
)
{
    NAS_RABM_NVIM_FASTDORM_ENABLE_FLG_STRU                  stEnableFlag;
    VOS_UINT32                                              ulLength;

    ulLength                  = 0;
    stEnableFlag.ucEnableFlag = VOS_FALSE;

    /* ��NVʧ�ܣ���Ϊ������Fastdormancy */
    NV_GetLength(en_NV_Item_Fastdorm_Enable_Flag, &ulLength);

    if (NV_OK != NV_Read(en_NV_Item_Fastdorm_Enable_Flag, &stEnableFlag,
                          ulLength))
    {
        NAS_WARNING_LOG(WUEPS_PID_RABM, "NAS_RABM_RcvGetFastDormParaReq:WARNING:NV_Read faild!");
    }

    /* �ظ���ѯ��� */
    NAS_RABM_SndAtQryFastDormParaCnf(pstFastDormPara->usClientId,
                                     pstFastDormPara->ucOpId,
                                     stEnableFlag.ucEnableFlag);

}



VOS_VOID NAS_RABM_SuspendFastDorm(VOS_VOID)
{
    GMM_RABM_NET_RAT_ENUM_UINT32        enCurrentSysMode;

    /* �����ǰ��NAS_RABM_FASTDORM_RUNNING״̬��������W�����£���Ҫ��WRR����ֹͣ
       ��������ֹͣFAST DORMANCY���� */
    /* �жϵ�ǰ��ϵͳģʽ�Ƿ���Ч */
    enCurrentSysMode = NAS_RABM_GetCurrentSysMode();
    if ((NAS_RABM_FASTDORM_RUNNING == NAS_RABM_GetCurrFastDormStatus())
     && (NAS_MML_NET_RAT_TYPE_WCDMA == enCurrentSysMode))
    {
        /* ��WAS����RRRABM_FASTDORM_STOP_REQ */
        NAS_RABM_SndWasFastDormStopReq();
    }

    /* ��ǰ����NAS_RABM_FASTDORM_STOP״̬������Ҫ����ǰ�����У�����
       SUSPEND״̬ */
    if (NAS_RABM_FASTDORM_STOP != NAS_RABM_GetCurrFastDormStatus())
    {
        /* ������ʱ��������0 */
        NAS_RABM_SetFastDormCurrNoFluxCntValue(0);

        /* ֹͣ��ʱ��RABM_TIMER_FASTDORM_FLUX_DETECT */
        /* ֹͣ��ʱ��RABM_TIMER_FASTDORM_RETRY */
        /* ֹͣ��ʱ��RABM_TIMER_FASTDORM_WAIT_GMM_QRY_RESULT*/

        NAS_RabmStopTimer(RABM_TIMER_NAME_FD_FLUX_DETECT,RABM_TIMER_FASTDORM_FLUX_DETECT);

        NAS_RabmStopTimer(RABM_TIMER_NAME_FD_RETRY,RABM_TIMER_FASTDORM_RETRY);
        NAS_RabmStopTimer(RABM_TIMER_NAME_COMMON,RABM_TIMER_FASTDORM_WAIT_GMM_QRY_RESULT);

        /* ����ΪNAS_RABM_FASTDORM_SUSPEND״̬ */
        NAS_RABM_SetCurrFastDormStatus(NAS_RABM_FASTDORM_SUSPEND);
        NAS_RABM_SndOmFastdormStatus();

    }
}
VOS_VOID NAS_RABM_ResumeFastDorm(VOS_VOID)
{
    /* �����ϵͳ�󣬷��ֵ�ǰPDP�Ǽ���ģ�����Ҫ��ʼ������⣬׼������
       FAST DORMANCY��������Ȼ��SUSPEND״̬ */
    if (VOS_TRUE == NAS_RABM_IsPdpActiveInWCDMA())
    {
        /* �����ǰ��NAS_RABM_FASTDORM_SUSPEND״̬��˵��֮ǰ������FAST DORMANCY */
        if (NAS_RABM_FASTDORM_SUSPEND == NAS_RABM_GetCurrFastDormStatus())
        {
            /* ����ΪNAS_RABM_FASTDORM_INIT_DETECT״̬ */
            NAS_RABM_SetCurrFastDormStatus(NAS_RABM_FASTDORM_INIT_DETECT);
            NAS_RABM_SndOmFastdormStatus();


            /* FD���ݼ��������� */
            NAS_RABM_ClrFastDormUlDataCnt();
            NAS_RABM_ClrFastDormDlDataCnt();


            /* ����������������ⶨʱ�� */
            NAS_RabmStartTimer( RABM_TIMER_NAME_FD_FLUX_DETECT, RABM_TIMER_FASTDORM_FLUX_DETECT );

        }
    }
}




VOS_VOID NAS_RABM_SysModeChgProcFastDorm(
    GMM_RABM_NET_RAT_ENUM_UINT32        enOldSysMode,
    GMM_RABM_NET_RAT_ENUM_UINT32        enNewSysMode
)
{

    /* ϵͳ�仯���ж������ǰ��Wģ������ģ������FAST DORMANCY */
    if ( (NAS_MML_NET_RAT_TYPE_WCDMA == enOldSysMode)
      && (NAS_MML_NET_RAT_TYPE_WCDMA != enNewSysMode)
      && (NAS_MML_NET_RAT_TYPE_BUTT != enNewSysMode))
    {
        NAS_RABM_SuspendFastDorm();
    }

    /* ϵͳ�仯���ж������ǰ������ģ��Wģ���ָ�FAST DORMANCY */
    if ( (NAS_MML_NET_RAT_TYPE_WCDMA != enOldSysMode)
      && (NAS_MML_NET_RAT_TYPE_WCDMA == enNewSysMode) )
    {
        NAS_RABM_ResumeFastDorm();
    }

}


VOS_VOID NAS_RABM_RcvAtFastDormStartReq(
    AT_RABM_SET_FASTDORM_PARA_REQ_STRU *pstFastDormPara
)
{
    GMM_RABM_NET_RAT_ENUM_UINT32        enCurrentSysMode;

    /* ��ȡ��ǰ�������� */
    enCurrentSysMode = NAS_RABM_GetCurrentSysMode();

    /* ������� */
    NAS_RABM_SetFastDormOperationType(pstFastDormPara->stFastDormPara.enFastDormOperationType);
    NAS_RABM_SetFastDormUserDefNoFluxCntValue(pstFastDormPara->stFastDormPara.ulTimeLen);

    /* ����FAST DORMANCYΪW�����ԣ��������W�����£���һ���趨ΪSUSPEND״̬ */
    if (NAS_MML_NET_RAT_TYPE_WCDMA != enCurrentSysMode)
    {
        /* ����ΪNAS_RABM_FASTDORM_SUSPEND״̬ */
        NAS_RABM_SetCurrFastDormStatus(NAS_RABM_FASTDORM_SUSPEND);
    }
    else
    {
        /* ��ǰ��PDP�Ǽ���״̬��һ������Ϊ��NAS_RABM_FASTDORM_SUSPEND״̬,���
           �Ѿ���PDP����״̬������ݲ�ͬ״̬������ͬ�Ĵ��� */
        if (VOS_TRUE == NAS_RABM_IsPdpActiveInWCDMA())
        {
            /* ��ǰ��NAS_RABM_FASTDORM_INIT_DETECT��˵��֮ǰ�Ѿ�������FAST DORMANCY */
            if (NAS_RABM_FASTDORM_INIT_DETECT == NAS_RABM_GetCurrFastDormStatus())
            {
                /* ���������ʱ���Ѿ������µ��趨ʱ������ֵ����ʱ��Ϊ����
                   ���Է���FAST DORMANCY */
                if (NAS_RABM_GetFastDormUserDefNoFluxCntValue()
                        < NAS_RABM_GetFastDormCurrNoFluxCntValue())
                {
                    /* ��GMM���Ͳ�ѯ��Ϣ */
                    NAS_RABM_SndGmmMmlProcStatusQryReq(RABM_FASTDORM_ENUM);

                    /* ������ʱ��������0 */
                    NAS_RABM_SetFastDormCurrNoFluxCntValue(0);

                    /* ����ΪNAS_RABM_FASTDORM_WAIT_GMM_QRY_RESULT״̬ */
                    NAS_RABM_SetCurrFastDormStatus(NAS_RABM_FASTDORM_WAIT_GMM_QRY_RESULT);

                    /* �����ȴ���ѯ����ı�����ʱ�� */
                    NAS_RabmStartTimer( RABM_TIMER_NAME_COMMON, RABM_TIMER_FASTDORM_WAIT_GMM_QRY_RESULT );
                }
            }


            /* if (��ǰ��NAS_RABM_FASTDORM_STOP״̬) */
            if (NAS_RABM_FASTDORM_STOP == NAS_RABM_GetCurrFastDormStatus())
            {
                /* ����ΪNAS_RABM_FASTDORM_INIT_DETECT״̬ */
                NAS_RABM_SetCurrFastDormStatus(NAS_RABM_FASTDORM_INIT_DETECT);


                /* FD���ݼ��������� */
                NAS_RABM_ClrFastDormUlDataCnt();
                NAS_RABM_ClrFastDormDlDataCnt();


                /* ����������������ⶨʱ�� */
                NAS_RabmStartTimer( RABM_TIMER_NAME_FD_FLUX_DETECT, RABM_TIMER_FASTDORM_FLUX_DETECT );
            }

            /* if (��ǰ��NAS_RABM_FASTDORM_RUNNING״̬) */
            if (NAS_RABM_FASTDORM_RUNNING == NAS_RABM_GetCurrFastDormStatus())
            {
                /* ֱ�ӷ�����Ϣ */
                NAS_RABM_SndWasFastDormStartReq(RABM_FASTDORM_ENUM);
            }


        }
        else
        {
            /* ����ΪNAS_RABM_FASTDORM_SUSPEND״̬ */
            NAS_RABM_SetCurrFastDormStatus(NAS_RABM_FASTDORM_SUSPEND);
        }
    }

    /* �ظ���AT ID_RABM_AT_FASTDORM_START_CNF */
    NAS_RABM_SndAtSetFastDormParaCnf(pstFastDormPara->usClientId,
                                     pstFastDormPara->ucOpId,
                                     AT_RABM_PARA_SET_RSLT_SUCC);

    NAS_RABM_SndOmFastdormStatus();
}



VOS_VOID NAS_RABM_RcvAtFastDormStopReq(
    AT_RABM_SET_FASTDORM_PARA_REQ_STRU *pstFastDormPara
)
{

    /* �����ǰ��NAS_RABM_FASTDORM_RUNNING״̬����Ҫ��WRR����ֹͣ
       ��������ֹͣFAST DORMANCY���� */
    if (NAS_RABM_FASTDORM_RUNNING == NAS_RABM_GetCurrFastDormStatus())
    {
        /* ��WAS����RRRABM_FASTDORM_STOP_REQ */
        NAS_RABM_SndWasFastDormStopReq();
    }

    /* ������ʱ��������0 */
    NAS_RABM_SetFastDormCurrNoFluxCntValue(0);

    /* ֹͣ��ʱ��RABM_TIMER_FASTDORM_FLUX_DETECT */
    /* ֹͣ��ʱ��RABM_TIMER_FASTDORM_RETRY */
    /* ֹͣ��ʱ��RABM_TIMER_FASTDORM_WAIT_GMM_QRY_RESULT*/

    NAS_RabmStopTimer(RABM_TIMER_NAME_FD_FLUX_DETECT,RABM_TIMER_FASTDORM_FLUX_DETECT);

    NAS_RabmStopTimer(RABM_TIMER_NAME_FD_RETRY,RABM_TIMER_FASTDORM_RETRY);
    NAS_RabmStopTimer(RABM_TIMER_NAME_COMMON,RABM_TIMER_FASTDORM_WAIT_GMM_QRY_RESULT);

    NAS_RABM_SetFastDormOperationType(pstFastDormPara->stFastDormPara.enFastDormOperationType);

    /* ����ΪNAS_RABM_FASTDORM_STOP״̬ */
    NAS_RABM_SetCurrFastDormStatus(NAS_RABM_FASTDORM_STOP);
    NAS_RABM_SndOmFastdormStatus();

    /* �ظ���AT ID_RABM_AT_SET_FASTDORM_PARA_CNF */
    NAS_RABM_SndAtSetFastDormParaCnf(pstFastDormPara->usClientId,
                                     pstFastDormPara->ucOpId,
                                     AT_RABM_PARA_SET_RSLT_SUCC);
}
VOS_VOID NAS_RABM_RcvAtMsg(
    struct MsgCB                       *pstMsg
)
{
    MSG_HEADER_STRU                     *pstMsgHeader;

    pstMsgHeader = (MSG_HEADER_STRU*)pstMsg;

    switch (pstMsgHeader->ulMsgName)
    {
        case ID_AT_RABM_SET_FASTDORM_PARA_REQ:
            NAS_RABM_RcvSetFastDormParaReq((AT_RABM_SET_FASTDORM_PARA_REQ_STRU*)pstMsg);
            break;

        case ID_AT_RABM_QRY_FASTDORM_PARA_REQ:
            NAS_RABM_RcvGetFastDormParaReq((AT_RABM_QRY_FASTDORM_PARA_REQ_STRU*)pstMsg);
            break;

        case ID_AT_RABM_SET_RELEASE_RRC_REQ:
            NAS_RABM_RcvReleaseRrcReq((AT_RABM_RELEASE_RRC_REQ_STRU *)pstMsg);
            break;
        default:
            break;
    }
}


VOS_VOID NAS_RABM_RcvWasFastDormInfoInd(
    struct MsgCB                       *pMsg
)
{
    VOS_UINT8                               ucEntId;

    RRRABM_FASTDORM_INFO_IND_STRU      *pstInfoInd;

    pstInfoInd = (RRRABM_FASTDORM_INFO_IND_STRU*)pMsg;

    /* �����NAS_RABM_FASTDORM_RUNNING״̬������Ҫ����RETRY״̬ */
    if ((NAS_RABM_FASTDORM_RUNNING == NAS_RABM_GetCurrFastDormStatus())
      &&(RRRABM_FASTDORM_INFO_NEED_RETRY == pstInfoInd->enInfoType))
    {

        /* ��ǰֻ�д���RAB��ʱ�򣬲���Ҫ����retry��ʱ��������Ӧ�ÿ���ֱ��
            return */
        for ( ucEntId = 0; ucEntId < RABM_PS_MAX_ENT_NUM; ucEntId++ )
        {
            if (g_aRabmPsEnt[ucEntId].ucState == RABM_NSAPI_ACTIVE_WITH_RAB)
            {
                break;
            }
        }

        if (ucEntId >= RABM_PS_MAX_ENT_NUM)
        {

            return;
        }

        /* ��ǰ��NAS_RABM_FASTDORM_RETRY״̬ */;
        NAS_RABM_SetCurrFastDormStatus(NAS_RABM_FASTDORM_RETRY);
        NAS_RABM_SndOmFastdormStatus();
        /* ����RABM_TIMER_FASTDORM_RETRY��ʱ�� */
        NAS_RabmStartTimer( RABM_TIMER_NAME_FD_RETRY, RABM_TIMER_FASTDORM_RETRY );
    }
}


VOS_VOID NAS_RABM_SetFastDormUserDefNoFluxCntValue(
    VOS_UINT32                          ulNoFluxCnt
)
{
    g_stNasRabmFastDormCtx.ulUserDefNoFluxCnt = ulNoFluxCnt;
}


VOS_UINT32 NAS_RABM_GetFastDormUserDefNoFluxCntValue(VOS_VOID)
{
    return g_stNasRabmFastDormCtx.ulUserDefNoFluxCnt;
}


VOS_VOID NAS_RABM_SetFastDormCurrNoFluxCntValue(
    VOS_UINT32                          ulNoFluxCnt
)
{
    g_stNasRabmFastDormCtx.ulCurrNoFluxCnt = ulNoFluxCnt;
}


VOS_UINT32 NAS_RABM_GetFastDormCurrNoFluxCntValue(VOS_VOID)
{
    return g_stNasRabmFastDormCtx.ulCurrNoFluxCnt;
}


VOS_VOID NAS_RABM_SetFastDormOperationType(
    AT_RABM_FASTDORM_OPERATION_ENUM_UINT32      enFastDormOperationType
)
{
    g_stNasRabmFastDormCtx.enFastDormOperationType = enFastDormOperationType;
}


AT_RABM_FASTDORM_OPERATION_ENUM_UINT32 NAS_RABM_GetFastDormOperationType(VOS_VOID)
{
    return g_stNasRabmFastDormCtx.enFastDormOperationType;
}


NAS_RABM_FASTDORM_STATUS_ENUM_UINT32 NAS_RABM_GetCurrFastDormStatus(VOS_VOID)
{
    return g_stNasRabmFastDormCtx.enCurrFastDormStatus;
}


VOS_VOID NAS_RABM_SetCurrFastDormStatus(
    NAS_RABM_FASTDORM_STATUS_ENUM_UINT32    enCurrFastDormStatus
)
{
    g_stNasRabmFastDormCtx.enCurrFastDormStatus = enCurrFastDormStatus;
}


VOS_VOID NAS_RABM_InitFastDormCtx(VOS_VOID)
{
    g_stNasRabmFastDormCtx.enCurrFastDormStatus    = NAS_RABM_FASTDORM_STOP;
    g_stNasRabmFastDormCtx.ulCurrNoFluxCnt         = 0;
    g_stNasRabmFastDormCtx.enFastDormOperationType = AT_RABM_FASTDORM_STOP_FD_ASCR;
    g_stNasRabmFastDormCtx.ulUserDefNoFluxCnt      = 0;

    g_stNasRabmFastDormCtx.ulDlDataCnt             = 0;
    g_stNasRabmFastDormCtx.ulUlDataCnt             = 0;
}




VOS_VOID NAS_RABM_IncFastDormUlDataCnt(VOS_VOID)
{
    g_stNasRabmFastDormCtx.ulUlDataCnt++;
}


VOS_VOID NAS_RABM_IncFastDormDlDataCnt(VOS_VOID)
{
    g_stNasRabmFastDormCtx.ulDlDataCnt++;
}


VOS_UINT32 NAS_RABM_GetFastDormUlDataCnt(VOS_VOID)
{
    return g_stNasRabmFastDormCtx.ulUlDataCnt;
}


VOS_UINT32 NAS_RABM_GetFastDormDlDataCnt(VOS_VOID)
{
    return g_stNasRabmFastDormCtx.ulDlDataCnt;
}


VOS_VOID NAS_RABM_ClrFastDormUlDataCnt(VOS_VOID)
{
    g_stNasRabmFastDormCtx.ulUlDataCnt = 0;
}


VOS_VOID NAS_RABM_ClrFastDormDlDataCnt(VOS_VOID)
{
    g_stNasRabmFastDormCtx.ulDlDataCnt = 0;
}



VOS_VOID NAS_RABM_RcvCdsMsg(
    struct MsgCB                       *pstMsg
)
{
    MSG_HEADER_STRU                     *pstMsgHeader;

    pstMsgHeader = (MSG_HEADER_STRU*)pstMsg;

    switch (pstMsgHeader->ulMsgName)
    {
        case ID_CDS_RABM_SERVICE_IND:
            NAS_RABM_RcvCdsServiceInd((CDS_RABM_SERVICE_IND_STRU *)pstMsg);
            break;

        default:
            break;
    }
}


VOS_UINT32 NAS_RABM_IsDataServiceRequestPending(VOS_VOID)
{
    if (VOS_TRUE == NAS_RABM_GetRabRsestTimerFlg())
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_RABM_IsRabReestablishPending(VOS_VOID)
{
    VOS_UINT8                           ucEntId;

    if (VOS_TRUE == NAS_RABM_GetRabRsestTimerFlg())
    {
        return VOS_FALSE;
    }

    for (ucEntId = 0; ucEntId < RABM_3G_PS_MAX_ENT_NUM; ucEntId++)
    {
        if (VOS_TRUE == NAS_RABM_GetWPsEntRabReestFlg(ucEntId))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_VOID NAS_RABM_RcvCdsServiceInd(
    CDS_RABM_SERVICE_IND_STRU          *pstCdsServiceInd
)
{
    /* ���RABID��Ч�� */
    if (!RAB_MAP_RAB_IS_VALID(pstCdsServiceInd->ucRabId))
    {
        NAS_ERROR_LOG1(WUEPS_PID_RABM,
            "NAS_RABM_RcvCdsServiceInd: Invalid RABID. <RABID>", pstCdsServiceInd->ucRabId);
        return;
    }

    switch (NAS_RABM_GetWPsEntState(pstCdsServiceInd->ucRabId - RABM_NSAPI_OFFSET))
    {
        case RABM_NSAPI_ACTIVE_WITH_RAB:
            NAS_NORMAL_LOG1(WUEPS_PID_RABM,
                "NAS_RABM_RcvCdsServiceInd: RABID is already active. <RABID>", pstCdsServiceInd->ucRabId);
            NAS_RABM_SndCdsSendBuffDataInd(pstCdsServiceInd->ucRabId, CDS_RABM_SEND_BUFF_DATA_ALLOWED_TYPE_SERVICE_SUCC);
            break;

        case RABM_NSAPI_ACTIVE_NO_RAB:
            if (VOS_TRUE == NAS_RABM_GetWPsEntRabReestFlg(pstCdsServiceInd->ucRabId - RABM_NSAPI_OFFSET))
            {
                NAS_NORMAL_LOG1(WUEPS_PID_RABM,
                    "NAS_RABM_RcvCdsServiceInd: Reestablish is in process. <RABID>", pstCdsServiceInd->ucRabId);
            }
            else if (VOS_TRUE == NAS_RABM_IsDataServiceRequestPending())
            {
                NAS_NORMAL_LOG1(WUEPS_PID_RABM,
                    "NAS_RABM_RcvCdsServiceInd: Data service request is sent, set reestablish flag. <RABID>", pstCdsServiceInd->ucRabId);
                NAS_RABM_SetWPsEntRabReestFlg(pstCdsServiceInd->ucRabId - RABM_NSAPI_OFFSET);
            }
            else if (VOS_TRUE == NAS_RABM_IsRabReestablishPending())
            {
                NAS_NORMAL_LOG1(WUEPS_PID_RABM,
                    "NAS_RABM_RcvCdsServiceInd: Start RB setup protect timer. <RABID>", pstCdsServiceInd->ucRabId);
                NAS_RABM_SetWPsEntRabReestFlg(pstCdsServiceInd->ucRabId - RABM_NSAPI_OFFSET);
                NAS_RABM_StartReestRabPendingTmr(pstCdsServiceInd->ucRabId);
            }
            else
            {
                NAS_NORMAL_LOG1(WUEPS_PID_RABM,
                    "NAS_RABM_RcvCdsServiceInd: Send data service request.  <RABID>", pstCdsServiceInd->ucRabId);
                NAS_RABM_SetWPsEntRabReestFlg(pstCdsServiceInd->ucRabId - RABM_NSAPI_OFFSET);
                NAS_RABM_SetRabRsestTimerFlg();
                RABM_TimerStart(RABM_REESTABLISH_REQ_SENT, RABM_RABM_REEST_PROT_TIMER_LEN);
                RABM_SndRabReestReq();
            }
            break;

        default:
            NAS_WARNING_LOG1(WUEPS_PID_RABM,
                "NAS_RABM_RcvCdsServiceInd: Can't reestablish RAB. <RABID>", pstCdsServiceInd->ucRabId);
            NAS_RABM_SndCdsFreeBuffDataInd(pstCdsServiceInd->ucRabId);
            break;
    }

    return;
}
VOS_VOID NAS_RABM_RcvReleaseRrcReq(
    AT_RABM_RELEASE_RRC_REQ_STRU *pstMsg
)
{
    GMM_RABM_NET_RAT_ENUM_UINT32        enCurrentSysMode;

    /* ��ȡ��ǰ�������� */
    enCurrentSysMode = NAS_RABM_GetCurrentSysMode();

    /* �������W�����£������ͷ�RRC���� */
    if (NAS_MML_NET_RAT_TYPE_WCDMA != enCurrentSysMode)
    {
        NAS_RABM_SndAtReleaseRrcCnf(pstMsg->usClientId, pstMsg->ucOpId, VOS_ERR);
    }
    else
    {
        /* ֱ�ӻظ�rrc�ͷųɹ� */
        NAS_RABM_SndAtReleaseRrcCnf(pstMsg->usClientId, pstMsg->ucOpId, VOS_OK);

        /* ��GMM���Ͳ�ѯ��Ϣ */
        NAS_RABM_SndGmmMmlProcStatusQryReq(RABM_RELEASE_RRC_ENUM);

    }

    return;
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


