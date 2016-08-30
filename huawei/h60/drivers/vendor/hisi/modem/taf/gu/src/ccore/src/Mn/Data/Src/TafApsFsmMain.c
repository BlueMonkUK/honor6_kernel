
/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "TafTypeDef.h"
#include "TafApsApi.h"
#include "Taf_Aps.h"
#include "MnApsComm.h"
#include "MmcApsInterface.h"
#include "TafLog.h"
#include "TafApsCtx.h"
#include "TafApsFsmMain.h"
#include "TafApsFsmMainTbl.h"
#include "TafApsFsmMsActivatingTbl.h"
#include "TafApsFsmMsDeactivatingTbl.h"
#include "TafApsFsmMsModifyingTbl.h"
#include "TafApsFsmNwActivatingTbl.h"
#include "TafApsProcIpFilter.h"
#include "TafApsSndAt.h"
#include "TafApsSndInternalMsg.h"
#include "TafApsSndRabm.h"
#if (FEATURE_ON == FEATURE_LTE)
#include "SmEsmInterface.h"
#include "MnApsMultiMode.h"
#include "ApsL4aInterface.h"
#include "TafApsSndL4a.h"
#endif

#include "ApsNdInterface.h"
#include "TafApsSndNd.h"
#include "TafApsComFunc.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_TAF_APS_FSM_MAIN_C


/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_UINT32 TAF_APS_RcvAtSetPdpContextStateReq_Inactive(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_SET_PDP_STATE_REQ_STRU      *pstSetPdpCtxStateReq;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg            = (TAF_PS_MSG_STRU*)pstMsg;
    pstSetPdpCtxStateReq = (TAF_PS_SET_PDP_STATE_REQ_STRU*)(pstAppMsg->aucContent);

    /* PDP����������� */
    if (TAF_CGACT_ACT == pstSetPdpCtxStateReq->stCidListStateInfo.ucState)
    {
        /*-------------------------------------------------------
           ����TAF_APS_STA_MS_ACTIVATING״̬��
           ���غ���״̬�л�ΪTAF_APS_MS_ACTIVATING_SUBSTA_INIT

           ��TAF_APS_MS_ACTIVATING_SUBSTA_INIT��״̬�д���
           ID_MSG_TAF_PS_SET_PDP_CONTEXT_STATE_REQ��Ϣ
        -------------------------------------------------------*/
        TAF_APS_InitSubFsm(TAF_APS_FSM_MS_ACTIVATING,
                           TAF_APS_GetMsActivatingFsmDescAddr(),
                           TAF_APS_MS_ACTIVATING_SUBSTA_INIT);
    }
    else
    {
        /* ָ��ȥ�����CID����δ����״̬, ��Ԥ�������Ѿ�����, ���ﲻ�ٴ��� */
        TAF_WARNING_LOG(WUEPS_PID_TAF,
            "TAF_APS_RcvAtSetPdpContextStateReq_Inactive: Wrong operation");
    }

    return VOS_TRUE;
}
VOS_UINT32 TAF_APS_RcvAtPsPppDialOrigReq_Inactive(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /*-------------------------------------------------------
       ����TAF_APS_STA_MS_ACTIVATING״̬��
       ���غ���״̬�л�ΪTAF_APS_MS_ACTIVATING_SUBSTA_INIT

       ��TAF_APS_MS_ACTIVATING_SUBSTA_INIT��״̬�д���
       ID_MSG_TAF_PS_PPP_DIAL_ORIG_REQ��Ϣ
    -------------------------------------------------------*/
    TAF_APS_InitSubFsm(TAF_APS_FSM_MS_ACTIVATING,
                       TAF_APS_GetMsActivatingFsmDescAddr(),
                       TAF_APS_MS_ACTIVATING_SUBSTA_INIT);

    return VOS_TRUE;
}


VOS_UINT32 TAF_APS_RcvAtPsCallOrigReq_Inactive(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /*-------------------------------------------------------
       ����TAF_APS_STA_MS_ACTIVATING״̬��
       ���غ���״̬�л�ΪTAF_APS_MS_ACTIVATING_SUBSTA_INIT

       ��TAF_APS_MS_ACTIVATING_SUBSTA_INIT��״̬�д���
       ID_MSG_TAF_PS_CALL_ORIG_REQ��Ϣ
    -------------------------------------------------------*/
    TAF_APS_InitSubFsm(TAF_APS_FSM_MS_ACTIVATING,
                       TAF_APS_GetMsActivatingFsmDescAddr(),
                       TAF_APS_MS_ACTIVATING_SUBSTA_INIT);

    return VOS_TRUE;
}


VOS_UINT32 TAF_APS_RcvSmPdpActivateInd_Inactive(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_APS_RAT_TYPE_ENUM_UINT32        enCurrRatType;

    /* ��ȡ��ǰ����ģʽ*/
    enCurrRatType                       = TAF_APS_GetCurrPdpEntityRatType();

    /* ��鵱ǰģ�Ƿ�ΪGU, �������, ��������Ϣ */
    if ( (MMC_APS_RAT_TYPE_GSM   != enCurrRatType)
      && (MMC_APS_RAT_TYPE_WCDMA != enCurrRatType) )
    {
        TAF_WARNING_LOG(WUEPS_PID_TAF,
            "TAF_APS_RcvSmPdpActivateInd_Inactive: Current RAT is not GSM/WCDMA!");
        return VOS_TRUE;
    }

     /* Ǩ�Ƶ���״̬TAF_APS_NW_ACTIVATING_SUBSTA_INIT,����״̬
        TAF_APS_NW_ACTIVATING_SUBSTA_INIT�д��� ID_SMREG_PDP_ACTIVATE_IND
        ��Ϣ
     */
    TAF_APS_InitSubFsm(TAF_APS_FSM_NW_ACTIVATING,
                       TAF_APS_GetNwActivatingFsmDescAddr(),
                       TAF_APS_NW_ACTIVATING_SUBSTA_INIT);

    return VOS_TRUE;

}

#if (FEATURE_ON == FEATURE_LTE)

VOS_UINT32 TAF_APS_RcvL4aPdpActivateInd_Inactive(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_APS_RAT_TYPE_ENUM_UINT32        enCurrRatType;

    /* ��ȡ��ǰ����ģʽ*/
    enCurrRatType                       = TAF_APS_GetCurrPdpEntityRatType();

    /* ��鵱ǰģ�Ƿ�ΪLTE, �������, ��������Ϣ */
    if (MMC_APS_RAT_TYPE_LTE != enCurrRatType)
    {
        TAF_WARNING_LOG(WUEPS_PID_TAF,
            "TAF_APS_RcvL4aPdpActivateInd_Inactive: Current RAT is not LTE!");
        return VOS_TRUE;
    }

    /* Ǩ�Ƶ���״̬TAF_APS_NW_ACTIVATING_SUBSTA_INIT,����״̬
       TAF_APS_NW_ACTIVATING_SUBSTA_INIT�д��� ID_L4A_APS_PDP_ACTIVATE_IND
       ��Ϣ
    */
    TAF_APS_InitSubFsm(TAF_APS_FSM_NW_ACTIVATING,
                       TAF_APS_GetNwActivatingFsmDescAddr(),
                       TAF_APS_NW_ACTIVATING_SUBSTA_INIT);

    return VOS_TRUE;
}


VOS_UINT32 TAF_APS_RcvEsmSmEpsBearerInfoInd_Inactive(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_APS_EPS_BEARER_INFO_IND_STRU   *pstBearerInfo;
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;
#if (FEATURE_ON == FEATURE_IPV6)
    MMC_APS_RAT_TYPE_ENUM_UINT32        enCurrRatType;
#endif
    VOS_UINT8                           ucPdpId;

    pstBearerInfo = (TAF_APS_EPS_BEARER_INFO_IND_STRU*)pstMsg;

#if (FEATURE_ON == FEATURE_IPV6)
    /* ��ȡ��ǰ����ģʽ*/
    enCurrRatType = TAF_APS_GetCurrPdpEntityRatType();
#endif
    ucPdpId       = TAF_APS_GetCurrFsmEntityPdpId();

    /* ��ȡ��PDPID��PDPʵ�����ݵ�ַ */
    pstPdpEntity = TAF_APS_GetPdpEntInfoAddr(ucPdpId);

    /* ����EPS���صĲ�������(Activate, Deactivate, Modify),
       ������Ӧ�Ĵ����� */
    if (SM_ESM_PDP_OPT_ACTIVATE == pstBearerInfo->enPdpOption)
    {
        MN_APS_ProcEsmBearerInfoIndOptActivate(pstPdpEntity, pstBearerInfo);

        if (TAF_APS_DEFAULT_CID == TAF_APS_GetCidFromLteBitCid(pstBearerInfo->ulBitCid))
        {
            TAF_APS_SndInterAttachBearerActivateInd();
        }

#if (FEATURE_ON == FEATURE_IMS)
        /* ����IMSר�г��� */
        TAF_APS_ProcImsBearerInfoIndOptActivate(pstPdpEntity, pstBearerInfo);
#endif

#if (FEATURE_ON == FEATURE_IPV6)
        /* �����ַ������IPv6, ��Ҫͬ����ND Client */
        if ( (MMC_APS_RAT_TYPE_NULL != enCurrRatType)
          && (TAF_APS_CheckPrimaryPdp(ucPdpId))
          && (TAF_APS_CheckPdpAddrTypeIpv6(ucPdpId)) )
        {
            TAF_APS_SndNdPdpActInd(pstPdpEntity->ucNsapi,
                                   pstPdpEntity->PdpAddr.aucIpV6Addr);

            pstPdpEntity->ulNdClientActiveFlg = VOS_TRUE;
        }
#endif
    }
    else
    {
        TAF_WARNING_LOG(WUEPS_PID_TAF,"TAF_APS_RcvEsmSmEpsBearerInfoInd_Inactive: Wrong option.");
    }

    /* ��״̬Ǩ����TAF_APS_STA_ACTIVE */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_ACTIVE);

    return VOS_TRUE;
}
#endif
VOS_UINT32 TAF_APS_RcvMmcServiceStatusInd_Inactive(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_APS_SERVICE_STATUS_IND_STRU    *pstMmcServiceStatusInd;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstMmcServiceStatusInd = (MMC_APS_SERVICE_STATUS_IND_STRU*)pstMsg;

    /* ���½��뼼�� */
    TAF_APS_SetCurrPdpEntityRatType(pstMmcServiceStatusInd->enRatType);

    /* ����PS��SIM��״̬ */
    TAF_APS_SetCurrPdpEntitySimRegStatus(pstMmcServiceStatusInd->ulPsSimRegStatus);

    /* ����״̬��ֻ�������ָʾ */
    if (MMC_APS_RAT_TYPE_NULL != pstMmcServiceStatusInd->enRatType)
    {
        TAF_WARNING_LOG(WUEPS_PID_TAF,
            "TAF_APS_RcvMmcServiceStatusInd_InactiveSuspend: Wrong RAT type!");
    }

    return VOS_TRUE;
}


VOS_UINT32 TAF_APS_RcvAtSetPdpContextStateReq_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_SET_PDP_STATE_REQ_STRU      *pstSetPdpCtxStateReq;
#if (FEATURE_ON == FEATURE_IPV6)
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;
#endif
    VOS_UINT32                          ulBearerActivateFlg;
    VOS_UINT8                           ucCidValue;
    VOS_UINT8                           ucPdpId;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg                           = (TAF_PS_MSG_STRU*)pstMsg;
    pstSetPdpCtxStateReq                = (TAF_PS_SET_PDP_STATE_REQ_STRU*)(pstAppMsg->aucContent);
    ucPdpId                             = TAF_APS_GetCurrFsmEntityPdpId();

    /* ���ü����� */
    ulBearerActivateFlg = (TAF_CGACT_ACT == pstSetPdpCtxStateReq->stCidListStateInfo.ucState) ? VOS_TRUE : VOS_FALSE;

    /* ��ǰ�Ȳ�֧��һ���Լ�����PDP�ĳ�����һ�μ���һ��������ҵ�һ��Ҫ����� CID */
    ucCidValue = TAF_APS_GetCidValue(ulBearerActivateFlg, pstSetPdpCtxStateReq->stCidListStateInfo.aucCid);

    /* PDP����������� */
    if (TAF_CGACT_ACT == pstSetPdpCtxStateReq->stCidListStateInfo.ucState)
    {
        /* ���CID��Ӧ��ʣ�ඨʱ��ʱ��Ϊ0, ��Ҫ�ϱ���Ӧ�¼� */
        if (0 == TAF_APS_GetCallRemainTmrLen(ucCidValue))
        {
            TAF_APS_SndSetPdpCtxStateCnf(&(pstSetPdpCtxStateReq->stCtrl),
                                         TAF_PS_CAUSE_SUCCESS);
        }

        /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_ACTIVATE_CNF�¼� */
        TAF_APS_SndPdpActivateCnf(ucPdpId, ucCidValue);

#if (FEATURE_ON == FEATURE_IPV6)
        pstPdpEntity = TAF_APS_GetPdpEntInfoAddr(ucPdpId);
        if (VOS_TRUE == pstPdpEntity->bitOpIpv6RaInfo)
        {
            /* �ϱ�ID_EVT_TAF_PS_IPV6_INFO_IND�¼� */
            TAF_APS_SndIpv6RaInfoNotifyInd(&(pstSetPdpCtxStateReq->stCtrl),
                                           ucCidValue,
                                           pstPdpEntity->ucNsapi,
                                           (TAF_PDP_IPV6_RA_INFO_STRU *)&pstPdpEntity->stIpv6RaInfo);
        }
#endif

        /* ����ɹ�����������ͳ�� */
        TAF_APS_StartDsFlowStats(TAF_APS_GetPdpEntNsapi(ucPdpId));
    }
    else
    {
        /*------------------------------------------------------------------
           ��״̬Ǩ����TAF_APS_STA_MS_DEACTIVATING
           ��״̬Ǩ����TAF_APS_MS_DEACTIVATING_SUBSTA_INIT

           ��TAF_APS_MS_DEACTIVATING_SUBSTA_INIT��״̬��
           ����ID_MSG_TAF_PS_SET_PDP_CONTEXT_STATE_REQ��Ϣ
        ------------------------------------------------------------------*/
        TAF_APS_InitSubFsm(TAF_APS_FSM_MS_DEACTIVATING,
                           TAF_APS_GetMsDeactivatingFsmDescAddr(),
                           TAF_APS_MS_DEACTIVATING_SUBSTA_INIT);
    }

    return VOS_TRUE;
}


VOS_UINT32 TAF_APS_RcvAtPsPppDialOrigReq_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_PPP_DIAL_ORIG_REQ_STRU      *pstPppDialOrigReq;
#if (FEATURE_ON == FEATURE_IPV6)
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;
#endif
    VOS_UINT8                           ucPdpId;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg                           = (TAF_PS_MSG_STRU*)pstMsg;
    pstPppDialOrigReq                   = (TAF_PS_PPP_DIAL_ORIG_REQ_STRU*)(pstAppMsg->aucContent);
    ucPdpId                             = TAF_APS_GetCurrFsmEntityPdpId();

    /* ���CID��Ӧ��ʣ�ඨʱ��ʱ��Ϊ0, ��Ҫ�ϱ���Ӧ�¼� */
    if (0 == TAF_APS_GetCallRemainTmrLen(pstPppDialOrigReq->stPppDialParaInfo.ucCid))
    {
        TAF_APS_SndPppDialOrigCnf(&(pstPppDialOrigReq->stCtrl),
                                  TAF_PS_CAUSE_SUCCESS);
    }

    /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_ACTIVATE_CNF�¼� */
    TAF_APS_SndPdpActivateCnf(ucPdpId, pstPppDialOrigReq->stPppDialParaInfo.ucCid);

#if (FEATURE_ON == FEATURE_IPV6)
    pstPdpEntity = TAF_APS_GetPdpEntInfoAddr(ucPdpId);
    if (VOS_TRUE == pstPdpEntity->bitOpIpv6RaInfo)
    {
        /* �ϱ�ID_EVT_TAF_PS_IPV6_INFO_IND�¼� */
        TAF_APS_SndIpv6RaInfoNotifyInd(&(pstPppDialOrigReq->stCtrl),
                                       pstPppDialOrigReq->stPppDialParaInfo.ucCid,
                                       pstPdpEntity->ucNsapi,
                                       (TAF_PDP_IPV6_RA_INFO_STRU *)&pstPdpEntity->stIpv6RaInfo);
    }
#endif

    /* ����ɹ�����������ͳ�� */
    TAF_APS_StartDsFlowStats(TAF_APS_GetPdpEntNsapi(ucPdpId));

    return VOS_TRUE;
}
VOS_UINT32 TAF_APS_RcvAtPsCallOrigReq_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_CALL_ORIG_REQ_STRU          *pstCallOrigReq;
#if (FEATURE_ON == FEATURE_IPV6)
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType;
#endif


    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg                           = (TAF_PS_MSG_STRU*)pstMsg;
    pstCallOrigReq                      = (TAF_PS_CALL_ORIG_REQ_STRU*)(pstAppMsg->aucContent);
    ucPdpId                             = TAF_APS_GetCurrFsmEntityPdpId();

    /* ���CID��Ӧ��ʣ�ඨʱ��ʱ��Ϊ0, ��Ҫ�ϱ���Ӧ�¼� */
    if (0 == TAF_APS_GetCallRemainTmrLen(pstCallOrigReq->stDialParaInfo.ucCid))
    {
        TAF_APS_SndCallOrigCnf(&(pstCallOrigReq->stCtrl),
                               pstCallOrigReq->stDialParaInfo.ucCid,
                               TAF_PS_CAUSE_SUCCESS);
    }

    /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_ACTIVATE_CNF�¼� */
    TAF_APS_SndPdpActivateCnf(ucPdpId, pstCallOrigReq->stDialParaInfo.ucCid);

#if (FEATURE_ON == FEATURE_IPV6)
    enPdpType    = pstCallOrigReq->stDialParaInfo.enPdpType;
    pstPdpEntity = TAF_APS_GetPdpEntInfoAddr(ucPdpId);

    if ( ((TAF_PDP_IPV6 == enPdpType) || (TAF_PDP_IPV4V6 == enPdpType))
      && (VOS_TRUE == pstPdpEntity->bitOpIpv6RaInfo) )
    {
        /* �ϱ�ID_EVT_TAF_PS_IPV6_INFO_IND�¼� */
        TAF_APS_SndIpv6RaInfoNotifyInd(&(pstCallOrigReq->stCtrl),
                                       pstCallOrigReq->stDialParaInfo.ucCid,
                                       pstPdpEntity->ucNsapi,
                                       (TAF_PDP_IPV6_RA_INFO_STRU *)&pstPdpEntity->stIpv6RaInfo);
    }
#endif

    /* ����ɹ�����������ͳ�� */
    TAF_APS_StartDsFlowStats(TAF_APS_GetPdpEntNsapi(ucPdpId));

    return VOS_TRUE;
}


VOS_UINT32 TAF_APS_RcvAtPsCallEndReq_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /*------------------------------------------------------------------
       ��״̬Ǩ����TAF_APS_STA_MS_DEACTIVATING
       ��״̬Ǩ����TAF_APS_MS_DEACTIVATING_SUBSTA_INIT

       ��TAF_APS_MS_DEACTIVATING_SUBSTA_INIT��״̬��
       ����ID_MSG_TAF_PS_CALL_END_REQ��Ϣ
    ------------------------------------------------------------------*/
    TAF_APS_InitSubFsm(TAF_APS_FSM_MS_DEACTIVATING,
                       TAF_APS_GetMsDeactivatingFsmDescAddr(),
                       TAF_APS_MS_DEACTIVATING_SUBSTA_INIT);

    return VOS_TRUE;
}


VOS_UINT32 TAF_APS_RcvAtPsCallModifyReq_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /*------------------------------------------------------------------
       ����TAF_APS_STA_MS_MODIFYING״̬��
       ���غ���״̬�л�ΪTAF_APS_MS_MODIFYING_SUBSTA_INIT

       ��TAF_APS_MS_MODIFYING_SUBSTA_INIT��״̬�д���
       ID_MSG_TAF_PS_CALL_MODIFY_REQ��Ϣ
    ------------------------------------------------------------------*/
    TAF_APS_InitSubFsm(TAF_APS_FSM_MS_MODIFYING,
                       TAF_APS_GetMsModifyingFsmDescAddr(),
                       TAF_APS_MS_MODIFYING_SUBSTA_INIT);

    return VOS_TRUE;
}


VOS_UINT32 TAF_APS_RcvAtPsCallAnswerReq_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_CALL_ANSWER_REQ_STRU        *pstCallAnswerReq;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg                           = (TAF_PS_MSG_STRU*)pstMsg;
    pstCallAnswerReq                    = (TAF_PS_CALL_ANSWER_REQ_STRU*)(pstAppMsg->aucContent);

    /* PS��MT��������, ����δȷ��, Ŀǰֱ�ӷ���ERROR */
    TAF_APS_SndCallAnswerCnf(&(pstCallAnswerReq->stCtrl),
                            pstCallAnswerReq->stAnsInfo.ucCid,
                            TAF_ERR_UNSPECIFIED_ERROR);

    return VOS_TRUE;
}


VOS_UINT32 TAF_APS_RcvAtPsCallHangupReq_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PS_MSG_STRU                    *pstAppMsg;
    TAF_PS_CALL_HANGUP_REQ_STRU        *pstCallHangupReq;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstAppMsg                           = (TAF_PS_MSG_STRU*)pstMsg;
    pstCallHangupReq                    = (TAF_PS_CALL_HANGUP_REQ_STRU*)(pstAppMsg->aucContent);

    /* PS��MT��������, ����δȷ��, Ŀǰֱ�ӷ���ERROR */
    TAF_APS_SndCallHangupCnf(&(pstCallHangupReq->stCtrl),
                            TAF_ERR_UNSPECIFIED_ERROR);

    return VOS_TRUE;
}


VOS_UINT32 TAF_APS_RcvSmPdpModifyInd_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulResult;
    VOS_UINT8                           ucPdpId;
    MMC_APS_RAT_TYPE_ENUM_UINT32        enCurrRatType;
    APS_MDFIND_PARA_ST                  stParam;
    SMREG_PDP_MODIFY_IND_STRU          *pstSmPdpModifyInd;
#if (FEATURE_ON == FEATURE_LTE)
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;
#endif

    /* ��ȡ��Ϣ���� */
    pstSmPdpModifyInd = (SMREG_PDP_MODIFY_IND_STRU *)pstMsg;
    ucPdpId           = TAF_APS_GetCurrFsmEntityPdpId();
    enCurrRatType     = TAF_APS_GetCurrPdpEntityRatType();
#if (FEATURE_ON == FEATURE_LTE)
    pstPdpEntity      = TAF_APS_GetPdpEntInfoAddr(ucPdpId);
#endif

    ulResult = Aps_PdpMdfIndParaCheck(pstSmPdpModifyInd, &stParam);
    if (APS_PARA_INVALID  == ulResult)
    {
        TAF_WARNING_LOG(WUEPS_PID_TAF,
            "TAF_APS_RcvSmPdpModifyInd_Active: Check para failed! ");

        /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_DEACTIVATE_IND�¼� */
        TAF_APS_SndPdpDeActivateInd(ucPdpId, TAF_PS_CAUSE_SM_NW_PROTOCOL_ERR_UNSPECIFIED);

        /* �����ڲ���Ϣ, ����PDPȥ�������� */
        TAF_APS_SndInterPdpDeactivateReq(ucPdpId, SM_TAF_CAUSE_SM_NW_PROTOCOL_ERR_UNSPECIFIED);

        return VOS_TRUE;
    }

    /* ���QOS�Ƿ�����MIN */
    if (APS_PARA_VALID == Aps_CheckQosSatisify(ucPdpId, &stParam.NewQos))
    {
        /* ����MINQOS */
        Aps_PdpMdfIndQosSatisfy(ucPdpId, &stParam, pstSmPdpModifyInd);

#if (FEATURE_ON == FEATURE_LTE)
        /* ͬ��PDP��Ϣ��ESM */
        MN_APS_SndEsmPdpInfoInd(pstPdpEntity, SM_ESM_PDP_OPT_MODIFY);
#endif

        /* �����ǰģΪGSM, Ǩ��״̬ */
        if (MMC_APS_RAT_TYPE_GSM == enCurrRatType)
        {
            /*------------------------------------------------------------------
               ����TAF_APS_STA_MS_MODIFYING״̬��
               ���غ���״̬�л�ΪTAF_APS_MS_MODIFYING_SUBSTA_INIT

               ��TAF_APS_MS_MODIFYING_SUBSTA_INIT��״̬��
               ����D_PMC_SMREG_PDP_MODIFY_IND��Ϣ
            ------------------------------------------------------------------*/
            TAF_APS_InitSubFsm(TAF_APS_FSM_MS_MODIFYING,
                               TAF_APS_GetMsModifyingFsmDescAddr(),
                               TAF_APS_MS_MODIFYING_SUBSTA_INIT);
        }
    }
    else
    {
        /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_DEACTIVATE_IND�¼� */
        TAF_APS_SndPdpDeActivateInd(ucPdpId, TAF_PS_CAUSE_SM_NW_QOS_NOT_ACCEPTED);

        /* �����ڲ���Ϣ, ����PDPȥ�������� */
        TAF_APS_SndInterPdpDeactivateReq(ucPdpId, SM_TAF_CAUSE_SM_NW_QOS_NOT_ACCEPTED);
    }

    return VOS_TRUE;
}


VOS_UINT32 TAF_APS_RcvSmPdpDeactivateInd_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;
    SMREG_PDP_DEACTIVATE_IND_STRU      *pstSmPdpDeactivateInd;
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstSmPdpDeactivateInd               = (SMREG_PDP_DEACTIVATE_IND_STRU*)pstMsg;
    ucPdpId                             = TAF_APS_GetCurrFsmEntityPdpId();
    pstPdpEntity                        = TAF_APS_GetPdpEntInfoAddr(ucPdpId);

    /* ֹͣ����ͳ�� */
    TAF_APS_StopDsFlowStats(pstPdpEntity->ucNsapi);

    /* ͬ��PDP״̬����Ϣ��ESM */
#if (FEATURE_ON == FEATURE_LTE)
    MN_APS_SndEsmPdpInfoInd(pstPdpEntity, SM_ESM_PDP_OPT_DEACTIVATE);
#endif

#if (FEATURE_ON == FEATURE_IPV6)
    /* �����ȱʡ�����ҵ�ַ������IPv6, ��Ҫͬ����ND Client */
    if ( (TAF_APS_CheckPrimaryPdp(ucPdpId))
      && (TAF_APS_CheckPdpAddrTypeIpv6(ucPdpId)) )
    {
        TAF_APS_SndNdPdpDeactInd(TAF_APS_GetPdpEntNsapi(ucPdpId));
    }
#endif

    /* ���SNDCP�Ѿ�����, �ͷ�SNDCP��Դ */
    if (APS_SNDCP_ACT == pstPdpEntity->PdpProcTrack.ucSNDCPActOrNot)
    {
        Aps_ReleaseSndcpResource(ucPdpId);
    }

    /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_DEACTIVATE_IND�¼� */
    TAF_APS_SndPdpDeActivateInd(ucPdpId, TAF_APS_MapSmCause(pstSmPdpDeactivateInd->enCause));

    /* �ͷ�APS��Դ */
    Aps_ReleaseApsResource(ucPdpId);

    /* ����IP������ */
    TAF_APS_IpfConfigUlFilter(ucPdpId);

    /* ��״̬Ǩ����TAF_APS_STA_INACTIVE */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);

    return VOS_TRUE;
}

#if (FEATURE_ON == FEATURE_LTE)
VOS_UINT32 TAF_APS_RcvL4aPdpModifyInd_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;
    APS_L4A_PDP_MODIFY_IND_STRU        *pstPdpModify;
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;

    pstPdpModify = (APS_L4A_PDP_MODIFY_IND_STRU*)pstMsg;

    /* ��ʼ��, ��ȡPDP ID */
    ucPdpId      = TAF_APS_GetCurrFsmEntityPdpId();
    pstPdpEntity = TAF_APS_GetPdpEntInfoAddr(ucPdpId);

    /* �������Ĭ�ϳ��أ����ϱ���Ĭ�ϳ��ز��ϱ� */
    if (TAF_APS_DEFAULT_CID != pstPdpModify->ucCid)
    {
        if (VOS_TRUE == pstPdpModify->bitOpEpsQos)
        {
            pstPdpEntity->bitOpEpsQos   = VOS_TRUE;
            PS_MEM_CPY((VOS_UINT8 *)&pstPdpEntity->stEpsQos,
                       (VOS_UINT8 *)&pstPdpModify->stEpsQosInfo,
                       sizeof(APS_L4A_EPS_QOS_STRU));
        }

        /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_MODIFIED_IND�¼� */
        TAF_APS_SndPdpModifyInd(ucPdpId);
    }

    return VOS_TRUE;
}
VOS_UINT32 TAF_APS_RcvL4aPdpDeactivateInd_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;
    APS_L4A_PDP_DEACTIVATE_IND_STRU    *pstL4aPdpDeactivateInd;

    /* ��ȡ��Ϣ���� */
    pstL4aPdpDeactivateInd = (APS_L4A_PDP_DEACTIVATE_IND_STRU*)pstMsg;

    /* ��ȡ��ǰ״̬����APS���� */
    ucPdpId                = TAF_APS_GetCurrFsmEntityPdpId();

    /* ��ȡ��������APSʵ�����ݵ�ַ */
    pstPdpEntity           = TAF_APS_GetPdpEntInfoAddr(ucPdpId);

    /* ֹͣ����ͳ�� */
    TAF_APS_StopDsFlowStats(pstPdpEntity->ucNsapi);

    /* �������Ĭ�ϳ��أ����ϱ���Ĭ�ϳ��ز��ϱ� */
    if (TAF_APS_DEFAULT_CID != pstL4aPdpDeactivateInd->ucCid)
    {
        /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_DEACTIVATE_IND�¼��ϱ� */
        TAF_APS_SndPdpDeActivateInd(ucPdpId, TAF_APS_MapL4aCause(pstL4aPdpDeactivateInd->ulEsmCause));
    }

    /* ���SNDCP�Ѿ�����, �ͷ�SNDCP��Դ */
    if (APS_SNDCP_ACT == pstPdpEntity->PdpProcTrack.ucSNDCPActOrNot)
    {
        Aps_ReleaseSndcpResource(ucPdpId);
    }

    /* �ͷ�APS��Դ */
    Aps_ReleaseApsResource(ucPdpId);

    /* ��״̬Ǩ����TAF_APS_STA_INACTIVE */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);

    return VOS_TRUE;
}



VOS_UINT32 TAF_APS_RcvEsmSmEpsBearerInfoInd_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;
    TAF_APS_EPS_BEARER_INFO_IND_STRU   *pstBearerInfo;

    pstBearerInfo = (TAF_APS_EPS_BEARER_INFO_IND_STRU*)pstMsg;
    ucPdpId       = TAF_APS_GetCurrFsmEntityPdpId();

    /* ��ȡ��PDPID��PDPʵ�����ݵ�ַ */
    pstPdpEntity = TAF_APS_GetPdpEntInfoAddr(ucPdpId);


    /* ����EPS���صĲ�������(Activate, Deactivate, Modify),
       ������Ӧ�Ĵ����� */
    if (SM_ESM_PDP_OPT_MODIFY == pstBearerInfo->enPdpOption)
    {
        MN_APS_ProcEsmBearerInfoIndOptModify(pstPdpEntity, pstBearerInfo);
    }
    else if (SM_ESM_PDP_OPT_DEACTIVATE == pstBearerInfo->enPdpOption)
    {
        MN_APS_ProcEsmBearerInfoIndOptDeactivate(pstPdpEntity, pstBearerInfo);
    }
    else
    {
        TAF_WARNING_LOG(WUEPS_PID_TAF,"TAF_APS_RcvEsmSmEpsBearerInfoInd_Inactive: Wrong option.");
    }

    return VOS_TRUE;
}
#endif


VOS_UINT32 TAF_APS_RcvSndcpActivateRsp_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulResult;
    VOS_UINT8                           ucPdpId;
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;
    APS_SNDCP_ACTIVATE_RSP_ST          *pstSnActivateRsp;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    ulResult         = VOS_OK;
    pstSnActivateRsp = &((APS_SNDCP_ACTIVATE_RSP_MSG*)pstMsg)->ApsSnActRsp;
    ucPdpId          = TAF_APS_GetCurrFsmEntityPdpId();
    pstPdpEntity     = TAF_APS_GetPdpEntInfoAddr(ucPdpId);

    /* �����Ϣ���� */
    ulResult = Aps_SnMsgModSnActRspParaCheck(pstSnActivateRsp);
    if (APS_PARA_VALID != ulResult)
    {
        TAF_WARNING_LOG(WUEPS_PID_TAF,
            "TAF_APS_RcvSndcpActivateRsp_Active: Check para failed!");

        /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_DEACTIVATE_IND�¼� */
        TAF_APS_SndPdpDeActivateInd(ucPdpId, TAF_PS_CAUSE_SM_NW_PROTOCOL_ERR_UNSPECIFIED);

        /* �����ڲ���Ϣ, ����PDPȥ�������� */
        TAF_APS_SndInterPdpDeactivateReq(ucPdpId, SM_TAF_CAUSE_SM_NW_PROTOCOL_ERR_UNSPECIFIED);

        return VOS_TRUE;
    }

    /* �յ�SN_ACT_RSP���޸�APSʵ�����, ����XID����, TRANSMODE */
    Aps_SnActRspChngEntity(pstSnActivateRsp, ucPdpId);

    /* ����RABM�Ĵ���ģʽ */
    TAF_APS_SndRabmSetTransModeMsg(pstSnActivateRsp->ucNsapi,
                                   pstPdpEntity->GprsPara.TransMode);

    return VOS_TRUE;
}


VOS_UINT32 TAF_APS_RcvSndcpDeactivateRsp_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��TAF_APS_STA_ACTIVE״̬��, Ŀǰ����Ҫ����APS_SN_DEACT_RSP_MSG_TYPE */

    return VOS_TRUE;
}
VOS_UINT32 TAF_APS_RcvSndcpStatusReq_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRslt;
    VOS_UINT8                           ucPdpId;
    APS_SNDCP_STATUS_REQ_ST            *pstSnStatusReq;

    pstSnStatusReq = &((APS_SNDCP_STATUS_REQ_MSG*)pstMsg)->ApsSnStatusReq;
    ucPdpId        = TAF_APS_GetCurrFsmEntityPdpId();

    ulRslt = TAF_APS_ValidatePdpForSnStatusReq(ucPdpId, pstSnStatusReq);
    if (VOS_TRUE == ulRslt)
    {
        /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_DEACTIVATE_IND�¼� */
        TAF_APS_SndPdpDeActivateInd(ucPdpId, TAF_PS_CAUSE_SM_NW_LLC_OR_SNDCP_FAILURE);

        /* �����ڲ���Ϣ, ����PDPȥ�������� */
        TAF_APS_SndInterPdpDeactivateReq(ucPdpId, SM_TAF_CAUSE_SM_NW_PROTOCOL_ERR_UNSPECIFIED);
    }

    return VOS_TRUE;
}


VOS_UINT32 TAF_APS_RcvRabmSysChgToGprs_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;

    /* ��ȡ��ǰ״̬����APSʵ������ */
    ucPdpId = TAF_APS_GetCurrFsmEntityPdpId();

    /* ���SNDCP�����ʶ, ����SNDCP */
    if (APS_SNDCP_INACT == TAF_APS_GetPdpEntSndcpActFlg(ucPdpId))
    {
        Aps_RabmMsgActSndcp(ucPdpId);
    }

    return VOS_TRUE;
}


VOS_UINT32 TAF_APS_RcvRabmLocalPdpDeactivateReq_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPdpId;
    MMC_APS_RAT_TYPE_ENUM_UINT32        enCurrRatType;

    /* ��ȡ��ǰ״̬���Ľ��뼼�� */
    enCurrRatType = TAF_APS_GetCurrPdpEntityRatType();

    /* ��ȡ��ǰ״̬����APSʵ������ */
    ucPdpId       = TAF_APS_GetCurrFsmEntityPdpId();

    /* ��GUģ�´������Ϣ, LTEģֱ�Ӷ��� */
    if (MMC_APS_RAT_TYPE_WCDMA == enCurrRatType)
    {
        /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_DEACTIVATE_IND�¼� */
        TAF_APS_SndPdpDeActivateInd(ucPdpId, TAF_PS_CAUSE_SM_RAB_SETUP_FAILURE);

        /* �����ڲ���Ϣ, ȥ����PDP */
        TAF_APS_SndInterPdpDeactivateReq(ucPdpId, SM_TAF_CAUSE_SM_NW_REGULAR_DEACTIVATION);
    }

    return VOS_TRUE;
}
VOS_UINT32 TAF_APS_RcvApsInternalPdpDeactivateReq_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /*------------------------------------------------------------------
       ����TAF_APS_STA_MS_DEACTIVATING״̬��
       ���غ���״̬�л�ΪTAF_APS_MS_DEACTIVATING_SUBSTA_INIT

       ��TAF_APS_MS_DEACTIVATING_SUBSTA_INIT��״̬�д���
       ID_APS_APS_INTER_PDP_DEACTIVATE_REQ��Ϣ
    ------------------------------------------------------------------*/
    TAF_APS_InitSubFsm(TAF_APS_FSM_MS_DEACTIVATING,
                       TAF_APS_GetMsDeactivatingFsmDescAddr(),
                       TAF_APS_MS_DEACTIVATING_SUBSTA_INIT);

    return VOS_TRUE;
}


VOS_UINT32 TAF_APS_RcvApsLocalPdpDeactivateInd_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                               ucPdpId;
    TAF_APS_INTER_PDP_DEACTIVATE_REQ_STRU  *pstLocalMsg;
    APS_PDP_CONTEXT_ENTITY_ST              *pstPdpEntity;

    /* ��ʼ��, ��ȡ��Ϣ���� */
    pstLocalMsg                         = (TAF_APS_INTER_PDP_DEACTIVATE_REQ_STRU *)pstMsg;
    ucPdpId                             = pstLocalMsg->ucPdpId;
    pstPdpEntity                        = TAF_APS_GetPdpEntInfoAddr(ucPdpId);

    /* ֹͣ����ͳ�� */
    TAF_APS_StopDsFlowStats(pstPdpEntity->ucNsapi);

    /* ͬ��PDP״̬����Ϣ��ESM */
#if (FEATURE_ON == FEATURE_LTE)
    MN_APS_SndEsmPdpInfoInd(pstPdpEntity, SM_ESM_PDP_OPT_DEACTIVATE);
#endif

#if (FEATURE_ON == FEATURE_IPV6)
    /* �����ȱʡ�����ҵ�ַ������IPv6, ��Ҫͬ����ND Client */
    if ( (TAF_APS_CheckPrimaryPdp(ucPdpId))
      && (TAF_APS_CheckPdpAddrTypeIpv6(ucPdpId)) )
    {
        TAF_APS_SndNdPdpDeactInd(TAF_APS_GetPdpEntNsapi(ucPdpId));
    }
#endif

    /* ���SNDCP�Ѿ�����, �ͷ�SNDCP��Դ */
    if (APS_SNDCP_ACT == pstPdpEntity->PdpProcTrack.ucSNDCPActOrNot)
    {
        Aps_ReleaseSndcpResource(ucPdpId);
    }
    /* �ϱ�ID_EVT_TAF_PS_CALL_PDP_DEACTIVATE_IND�¼� */
    TAF_APS_SndPdpDeActivateInd(ucPdpId, TAF_APS_MapSmCause(pstLocalMsg->enCause));

    /* �ͷ�APS��Դ */
    Aps_ReleaseApsResource(ucPdpId);

    /* ����IP������ */
    TAF_APS_IpfConfigUlFilter(ucPdpId);

    /* ��״̬Ǩ����TAF_APS_STA_INACTIVE */
    TAF_APS_SetCurrPdpEntityMainFsmState(TAF_APS_STA_INACTIVE);

    return VOS_TRUE;
}
VOS_UINT32 TAF_APS_RcvMmcServiceStatusInd_Active(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_APS_SERVICE_STATUS_IND_STRU    *pstMmcServiceStatusInd;
#if (FEATURE_ON == FEATURE_IPV6)
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;
#endif
    VOS_UINT8                           ucPdpId;

    /* ��ȡ��Ϣ���� */
    pstMmcServiceStatusInd = (MMC_APS_SERVICE_STATUS_IND_STRU*)pstMsg;

    /* ��ȡ��ǰ״̬����APSʵ������ */
    ucPdpId                = TAF_APS_GetCurrFsmEntityPdpId();

#if (FEATURE_ON == FEATURE_IPV6)
    /* ��ȡ��PDPID��PDPʵ�����ݵ�ַ */
    pstPdpEntity           = TAF_APS_GetPdpEntInfoAddr(ucPdpId);
#endif

    /* ���½��뼼�� */
    TAF_APS_SetCurrPdpEntityRatType(pstMmcServiceStatusInd->enRatType);

    /* ����PS��SIM��״̬ */
    TAF_APS_SetCurrPdpEntitySimRegStatus(pstMmcServiceStatusInd->ulPsSimRegStatus);

#if (FEATURE_ON == FEATURE_IPV6)
    /* �����ַ������IPv6, ��Ҫͬ����ND Client */
    if ( (MMC_APS_RAT_TYPE_NULL != pstMmcServiceStatusInd->enRatType)
      && (TAF_APS_CheckPrimaryPdp(ucPdpId))
      && (TAF_APS_CheckPdpAddrTypeIpv6(ucPdpId)) )
    {
        if (VOS_TRUE != pstPdpEntity->ulNdClientActiveFlg)
        {
            TAF_APS_SndNdPdpActInd(pstPdpEntity->ucNsapi,
                                   pstPdpEntity->PdpAddr.aucIpV6Addr);

            pstPdpEntity->ulNdClientActiveFlg = VOS_TRUE;
        }
    }
#endif

    switch (pstMmcServiceStatusInd->enRatType)
    {
        /* ��ǰģΪGSMʱ, ����������Ϣ����SNDCP */
        case MMC_APS_RAT_TYPE_GSM:
            break;

        /* ��ǰģΪWCDMA/LTEʱ, ��Ҫȥ����SNDCP */
        case MMC_APS_RAT_TYPE_WCDMA:
        case MMC_APS_RAT_TYPE_LTE:
            if (APS_SNDCP_ACT == TAF_APS_GetPdpEntSndcpActFlg(ucPdpId))
            {
                Aps_RabmMsgDeActSndcp(ucPdpId);
            }
            break;

        case MMC_APS_RAT_TYPE_NULL:
        default:
            break;
    }

    return VOS_TRUE;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

