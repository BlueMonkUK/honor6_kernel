/******************************************************************************

   Copyright(C)2013,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : ImsaProcSpmMsg.c
  Description     : ��C�ļ�ʵ��Spm��Ϣ�����Spm��Ϣ����
  History           :
     1.sunbing 49683      2013-06-19  Draft Enact

******************************************************************************/

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "ImsaEntity.h"
#include "ImsaPublic.h"
#include "ImsaCallManagement.h"
#include "ImsaProcSpmMsg.h"
#include "ImsaImsAdaption.h"

#include "ImsaProcImsCallMsg.h"

/* xiongxianghui00253310 for PC-LINT 20140210 begin */
#include "ImsaProcUssdMsg.h"
/* xiongxianghui00253310 for PC-LINT 20140210 end */


/*lint -e767*/
#define    THIS_FILE_ID   PS_FILE_ID_IMSAPROCSPMMSG_C
/*lint +e767*/


/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (FEATURE_ON == FEATURE_IMS)
/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/

extern VOS_UINT32 IMSA_CallSendSpmClprCnf
(
    const IMSA_SPM_CALL_GET_CLPR_CNF_STRU  *pstMsg
);

/*****************************************************************************
  3 Function
*****************************************************************************/
/*lint -e960*/
/*lint -e961*/

VOS_UINT32 IMSA_CallSendSpmClprCnf
(
    const IMSA_SPM_CALL_GET_CLPR_CNF_STRU  *pstMsg
)
{
    IMSA_SPM_CALL_GET_CLPR_CNF_STRU *pstSpmCallMsg = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallSendSpmClprCnf is entered!");

    /* ����ռ䲢��������Ƿ�ɹ� */
    pstSpmCallMsg = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_CALL_GET_CLPR_CNF_STRU));
    if (VOS_NULL_PTR == pstSpmCallMsg)
    {
        IMSA_ERR_LOG("IMSA_CallSendSpmClprCnf: alloc memory fail");
        return VOS_FALSE;
    }

    /* ������Ϣ���� */
    IMSA_MEM_CPY(IMSA_GET_MSG_ENTITY(pstSpmCallMsg), IMSA_GET_MSG_ENTITY(pstMsg), IMSA_GET_MSG_LENGTH(pstSpmCallMsg));

    /* ��д��Ϣͷ */
    IMSA_WRITE_SPM_MSG_HEAD(pstSpmCallMsg, ID_IMSA_SPM_CALL_GET_CLPR_CNF);

    /* ������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstSpmCallMsg);

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallProcSpmMsgGetClprInfo
(
    const SPM_IMSA_CALL_GET_CLPR_REQ_STRU     *pstAppMsg
)
{
    IMSA_SPM_CALL_GET_CLPR_CNF_STRU     stGetClprCnf    = {0};
    IMSA_CALL_ENTITY_STRU              *pstCallEntity   = VOS_NULL_PTR;
    VOS_UINT32                          ulResult        = VOS_FALSE;

    IMSA_MEM_SET(&stGetClprCnf, 0x00, sizeof(IMSA_SPM_CALL_GET_CLPR_CNF_STRU));

    /* ����MN_CALL_EVT_CLPR_GET_CNF��Ϣ */
    stGetClprCnf.usClientId     = pstAppMsg->usClientId;
    stGetClprCnf.ulRet          = TAF_ERR_ERROR;

    /* ��������CALLID�Ƿ����,����Ϊ���� */
    pstCallEntity = IMSA_CallEntityGetUsedByCallId(pstAppMsg->callId);
    if (VOS_NULL_PTR != pstCallEntity)
    {
        IMSA_CallReadCallEntityRedirectInfo(pstCallEntity, &stGetClprCnf);
    }

    ulResult = IMSA_CallSendSpmClprCnf(&stGetClprCnf);

    return ulResult;
}


VOS_UINT32 IMSA_CallSendSpmClccInfoCnf
(
    const IMSA_SPM_CALL_GET_CALL_INFO_CNF_STRU *pstMsg
)
{
    IMSA_SPM_CALL_GET_CALL_INFO_CNF_STRU *pstSpmCallMsg = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallSendSpmClccInfoCnf is entered!");

    /* ����ռ䲢��������Ƿ�ɹ� */
    pstSpmCallMsg = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_CALL_GET_CALL_INFO_CNF_STRU));
    if (VOS_NULL_PTR == pstSpmCallMsg)
    {
        IMSA_ERR_LOG("IMSA_CallSendSpmClccInfoCnf: alloc memory fail");
        return VOS_FALSE;
    }

    /* ������Ϣ���� */
    IMSA_MEM_CPY(IMSA_GET_MSG_ENTITY(pstSpmCallMsg), IMSA_GET_MSG_ENTITY(pstMsg), IMSA_GET_MSG_LENGTH(pstSpmCallMsg));

    /* ��д��Ϣͷ */
    IMSA_WRITE_SPM_MSG_HEAD(pstSpmCallMsg, ID_IMSA_SPM_CALL_GET_CALL_INFO_CNF);

    /* ������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstSpmCallMsg);

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallProcSpmMsgGetInfo
(
    const SPM_IMSA_CALL_GET_CALL_INFO_REQ_STRU *pstAppMsg
)
{
    IMSA_CALL_MANAGER_STRU *pstCallCtx = IMSA_CallCtxGet();
    IMSA_SPM_CALL_GET_CALL_INFO_CNF_STRU stQryCnf = {0};
    VOS_UINT32 ulCount = 0;
    VOS_UINT32 i = 0;
    VOS_UINT32 ulResult = VOS_FALSE;

    IMSA_INFO_LOG("IMSA_CallProcSpmMsgGetInfo is entered!");

    for (i = 0; i < IMSA_CALL_MAX_NUM; i++)
    {
        if (VOS_TRUE == pstCallCtx->astCallEntity[i].bitOpIsUsed)
        {
            IMSA_CallEntity2SpmCallInfoParam(&pstCallCtx->astCallEntity[i], &stQryCnf.astCallInfo[ulCount]);
            ulCount++;
        }
    }

    stQryCnf.ucNumOfCalls = (VOS_UINT8)ulCount;
    stQryCnf.usClientId = pstAppMsg->usClientId;
    stQryCnf.enReqType = pstAppMsg->enReqType;


    ulResult = IMSA_CallSendSpmClccInfoCnf(&stQryCnf);

    return ulResult;
}

VOS_UINT32 IMSA_SpmGetCallInfo(IMSA_SPM_CALL_GET_CALL_INFO_IND_STRU *pstCallInfo)
{
    IMSA_CALL_MANAGER_STRU *pstCallCtx = IMSA_CallCtxGet();
    VOS_UINT32 ulCount = 0;
    VOS_UINT32 i = 0;

    IMSA_INFO_LOG("IMSA_SpmGetCallInfo is entered!");

    if (VOS_NULL_PTR == pstCallInfo)
    {
        IMSA_ERR_LOG("IMSA_SpmGetCallInfo: pstCallInfo is null");
        return IMSA_FAIL;
    }

    for (i = 0; i < IMSA_CALL_MAX_NUM; i++)
    {
        if (VOS_TRUE == pstCallCtx->astCallEntity[i].bitOpIsUsed)
        {
            IMSA_CallEntity2SpmCallInfoParam(&pstCallCtx->astCallEntity[i], &(pstCallInfo->astCallInfo[ulCount]));
            ulCount++;
        }
    }

    pstCallInfo->ucNumOfCalls = (VOS_UINT8)ulCount;
    return IMSA_SUCC;
}


VOS_UINT32 IMSA_CallSendSpmStartDtmfCnfMsg
(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucCallId,
    TAF_CS_CAUSE_ENUM_UINT32            ulResult
)
{
    IMSA_SPM_CALL_START_DTMF_CNF_STRU *pstStartDtmfCnf = VOS_NULL_PTR;

    (VOS_VOID)ucCallId;

    IMSA_INFO_LOG("IMSA_CallSendSpmStartDtmfCnfMsg is entered!");

    /* ����ռ䲢��������Ƿ�ɹ� */
    pstStartDtmfCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_CALL_START_DTMF_CNF_STRU));
    if (VOS_NULL_PTR == pstStartDtmfCnf)
    {
        IMSA_ERR_LOG("IMSA_CallSendSpmStartDtmfCnfMsg: alloc memory fail");
        return VOS_FALSE;
    }

    /* ��� */
    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstStartDtmfCnf), 0, IMSA_GET_MSG_LENGTH(pstStartDtmfCnf));

    /* ��д��Ϣͷ */
    IMSA_WRITE_SPM_MSG_HEAD(pstStartDtmfCnf, ID_IMSA_SPM_CALL_START_DTMF_CNF);

    pstStartDtmfCnf->usClientId = usClientId;
    pstStartDtmfCnf->ucOpId     = ucOpId;
    pstStartDtmfCnf->enCause    = ulResult;

    /* ������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstStartDtmfCnf);

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallSendSpmStopDtmfCnfMsg
(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucCallId,
    TAF_CS_CAUSE_ENUM_UINT32            ulResult
)
{
    IMSA_SPM_CALL_STOP_DTMF_CNF_STRU   *pstStopDtmfCnf = VOS_NULL_PTR;

    (VOS_VOID)ucCallId;

    IMSA_INFO_LOG("IMSA_CallSendSpmStopDtmfCnfMsg is entered!");

    /* ����ռ䲢��������Ƿ�ɹ� */
    pstStopDtmfCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_CALL_STOP_DTMF_CNF_STRU));
    if (VOS_NULL_PTR == pstStopDtmfCnf)
    {
        IMSA_ERR_LOG("IMSA_CallSendSpmStopDtmfCnfMsg: alloc memory fail");
        return VOS_FALSE;
    }

    /* ��� */
    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstStopDtmfCnf), 0, IMSA_GET_MSG_LENGTH(pstStopDtmfCnf));

    /* ��д��Ϣͷ */
    IMSA_WRITE_SPM_MSG_HEAD(pstStopDtmfCnf, ID_IMSA_SPM_CALL_STOP_DTMF_CNF);

    pstStopDtmfCnf->usClientId = usClientId;
    pstStopDtmfCnf->ucOpId     = ucOpId;
    pstStopDtmfCnf->enCause    = ulResult;

    /* ������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstStopDtmfCnf);

    return VOS_TRUE;
}



VOS_VOID IMSA_CallProcSpmMsgStartDtmf
(
    const SPM_IMSA_CALL_START_DTMF_REQ_STRU           *pstAppMsg
)
{
    TAF_CALL_DTMF_PARAM_STRU            stStartDtmfParam    = {0};
    VOS_UINT32                          ulResult            = VOS_FALSE;
    IMSA_CALL_MANAGER_STRU             *pstCallCtx          = IMSA_CallCtxGet();

    IMSA_INFO_LOG("IMSA_CallProcSpmMsgStartDtmf is entered!");

    IMSA_MEM_CPY(   &stStartDtmfParam,
                    &pstAppMsg->stDtmf,
                    sizeof(TAF_CALL_DTMF_PARAM_STRU));

    /* DTMF��Ϣ�Ϸ����ж� */
    if ( (VOS_OK != IMSA_CallCheckDtmfKey(&stStartDtmfParam.cKey))
      || (stStartDtmfParam.usOnLength < IMSA_DTMF_MIN_ONLENGTH) )
    {
        (VOS_VOID)IMSA_CallSendSpmStartDtmfCnfMsg(  pstAppMsg->usClientId,
                                                    pstAppMsg->ucOpId,
                                                    pstAppMsg->stDtmf.CallId,
                                                    TAF_CS_CAUSE_INVALID_PARAMETER);
        return ;
    }

    /* ���жϵ�ǰ�Ƿ�������DTMF���ٻ�ȡ���п��Է���DTMF�ĺ���ID */
    ulResult    = IMSA_CallCheckUserDtmfCallId(stStartDtmfParam.CallId);
    if (TAF_CS_CAUSE_SUCCESS != ulResult)
    {
        (VOS_VOID)IMSA_CallSendSpmStartDtmfCnfMsg(  pstAppMsg->usClientId,
                                                    pstAppMsg->ucOpId,
                                                    pstAppMsg->stDtmf.CallId,
                                                    ulResult);
        return;
    }

    /* �ж�DTMF״̬�Ƿ����� */
    if (IMSA_CallGetDtmfState() >= IMSA_CALL_DTMF_STATE_BUTT)
    {
        (VOS_VOID)IMSA_CallSendSpmStartDtmfCnfMsg(  pstAppMsg->usClientId,
                                                    pstAppMsg->ucOpId,
                                                    pstAppMsg->stDtmf.CallId,
                                                    TAF_CS_CAUSE_STATE_ERROR);
        return;
    }

    /* DTMF���ڷǿ���״̬���򻺴��DTMF */
    if (IMSA_CallGetDtmfState() != IMSA_CALL_DTMF_IDLE)
    {
        ulResult = IMSA_CallSaveDtmfInfo(pstAppMsg->stDtmf.CallId,
                                         pstAppMsg->stDtmf.cKey,
                                         pstAppMsg->stDtmf.usOnLength,
                                         pstAppMsg->usClientId,
                                         pstAppMsg->ucOpId);

        (VOS_VOID)IMSA_CallSendSpmStartDtmfCnfMsg(pstAppMsg->usClientId,
                                                  pstAppMsg->ucOpId,
                                                  pstAppMsg->stDtmf.CallId,
                                                  ulResult);
        return;
    }

    /* DTMF��ǰ���ڿ���̬�����͵�IMSЭ��ջ */
    if (VOS_TRUE != IMSA_CallSendImsMsgStartDtmf(   pstAppMsg->stDtmf.CallId,
                                                    stStartDtmfParam.cKey,
                                                    (VOS_UINT32)stStartDtmfParam.usOnLength))
    {
        IMSA_ERR_LOG("IMSA_CallProcSpmMsgStartDtmf: error occurs, should return failure to SPM");

        (VOS_VOID)IMSA_CallSendSpmStartDtmfCnfMsg(  pstAppMsg->usClientId,
                                                    pstAppMsg->ucOpId,
                                                    pstAppMsg->stDtmf.CallId,
                                                    TAF_CS_CAUSE_IMSA_ERROR);

        return ;
    }

    /* ���ͳɹ�������DTMF״̬ */
    IMSA_StartTimer(&pstCallCtx->stDtmfCtx.stDtmfProtectTimer);

    IMSA_CallFillCurrentDtmfInfo(   pstAppMsg->stDtmf.CallId,
                                    stStartDtmfParam.cKey,
                                    stStartDtmfParam.usOnLength,
                                    pstAppMsg->usClientId,
                                    pstAppMsg->ucOpId);

    IMSA_CallSetDtmfState(IMSA_CALL_DTMF_WAIT_START_CNF);

    (VOS_VOID)IMSA_CallSendSpmStartDtmfCnfMsg(  pstAppMsg->usClientId,
                                                pstAppMsg->ucOpId,
                                                pstAppMsg->stDtmf.CallId,
                                                TAF_CS_CAUSE_SUCCESS);

    return;
}


VOS_VOID IMSA_CallProcSpmMsgStopDtmf
(
    const SPM_IMSA_CALL_STOP_DTMF_REQ_STRU         *pstAppMsg
)
{
    VOS_UINT32                          ulResult            = VOS_FALSE;
    IMSA_CALL_MANAGER_STRU             *pstCallCtx          = IMSA_CallCtxGet();
    TAF_CALL_DTMF_PARAM_STRU            stStopDtmfParam     = {0};
    IMSA_CALL_DTMF_STATE_ENUM_UINT8     enDtmfState         = IMSA_CALL_DTMF_STATE_BUTT;

    IMSA_INFO_LOG("IMSA_CallProcSpmMsgStopDtmf is entered!");

    IMSA_MEM_CPY(   &stStopDtmfParam,
                    &pstAppMsg->stDtmf,
                    sizeof(TAF_CALL_DTMF_PARAM_STRU));

    /* STOP_DTMF����ʱ��OnLengthֵ��Ϊ0 */
    if (0 != stStopDtmfParam.usOnLength)
    {
        (VOS_VOID)IMSA_CallSendSpmStopDtmfCnfMsg(   pstAppMsg->usClientId,
                                                    pstAppMsg->ucOpId,
                                                    pstAppMsg->stDtmf.CallId,
                                                    TAF_CS_CAUSE_INVALID_PARAMETER);
        return;
    }

    /* �ж�DTMF״̬�Ƿ����� */
    enDtmfState = IMSA_CallGetDtmfState();
    if ( (IMSA_CALL_DTMF_STATE_BUTT <= enDtmfState)
      || (IMSA_CALL_DTMF_IDLE == enDtmfState) )
    {
        (VOS_VOID)IMSA_CallSendSpmStopDtmfCnfMsg(   pstAppMsg->usClientId,
                                                    pstAppMsg->ucOpId,
                                                    pstAppMsg->stDtmf.CallId,
                                                    TAF_CS_CAUSE_STATE_ERROR);
        return;
    }

    /* ���жϵ�ǰ�Ƿ�������DTMF���Լ��û�����ĺ���ID�Ƿ���Է���DTMF */
    ulResult    = IMSA_CallCheckUserDtmfCallId(stStopDtmfParam.CallId);
    if (TAF_CS_CAUSE_SUCCESS != ulResult)
    {
        (VOS_VOID)IMSA_CallSendSpmStopDtmfCnfMsg(   pstAppMsg->usClientId,
                                                    pstAppMsg->ucOpId,
                                                    pstAppMsg->stDtmf.CallId,
                                                    ulResult);
        return;
    }

    /* �����ǰû�л����Ҵ��ڵȴ���ʱ����ʱ״̬����ͣ��ʱ��������STOP_DTMF�����򻺴� */
    if ((IMSA_CALL_DTMF_WAIT_TIME_OUT == enDtmfState)
        && (0 == IMSA_CallGetDtmfCnt()))
    {
        /* ͣDTMF duration��ʱ�� */
        IMSA_StopTimer(&pstCallCtx->stDtmfCtx.stDtmfDurationTimer);

        if (VOS_TRUE != IMSA_CallSendImsMsgStopDtmf(pstAppMsg->stDtmf.CallId,
                                                    stStopDtmfParam.cKey))
        {
            IMSA_ERR_LOG("IMSA_CallProcSpmMsgStopDtmf: error occurs, should return failure to SPM");

            (VOS_VOID)IMSA_CallSendSpmStopDtmfCnfMsg(   pstAppMsg->usClientId,
                                                        pstAppMsg->ucOpId,
                                                        pstAppMsg->stDtmf.CallId,
                                                        TAF_CS_CAUSE_IMSA_ERROR);
            return ;
        }

        /* ���ͳɹ�������DTMF״̬ */
        IMSA_StartTimer(&pstCallCtx->stDtmfCtx.stDtmfProtectTimer);

        IMSA_CallFillCurrentDtmfInfo(   pstAppMsg->stDtmf.CallId,
                                        stStopDtmfParam.cKey,
                                        stStopDtmfParam.usOnLength,
                                        pstAppMsg->usClientId,
                                        pstAppMsg->ucOpId);

        IMSA_CallSetDtmfState(IMSA_CALL_DTMF_WAIT_STOP_CNF);

        (VOS_VOID)IMSA_CallSendSpmStopDtmfCnfMsg(   pstAppMsg->usClientId,
                                                    pstAppMsg->ucOpId,
                                                    pstAppMsg->stDtmf.CallId,
                                                    TAF_CS_CAUSE_SUCCESS);
        return;
    }

    /* ��ǰ���ܷ���STOP_DTMF�����򻺴� */
    ulResult = IMSA_CallSaveDtmfInfo(pstAppMsg->stDtmf.CallId,
                                     pstAppMsg->stDtmf.cKey,
                                     pstAppMsg->stDtmf.usOnLength,
                                     pstAppMsg->usClientId,
                                     pstAppMsg->ucOpId);

    (VOS_VOID)IMSA_CallSendSpmStopDtmfCnfMsg(pstAppMsg->usClientId,
                                             pstAppMsg->ucOpId,
                                             pstAppMsg->stDtmf.CallId,
                                             ulResult);

    return ;
}
VOS_UINT32 IMSA_CallSendSpmSupsCnfMsg(VOS_UINT16 usClientId,
                                      VOS_UINT32 ulOpId,
                                      VOS_UINT32 ulCallId,
                                      TAF_CS_CAUSE_ENUM_UINT32 ulResult)
{
    IMSA_INFO_LOG("IMSA_CallSendSpmSupsCnfMsg is entered!");

    return IMSA_CallSendSpmEvtAndCallInfo(usClientId,
                                          ulOpId,
                                          ulCallId,
                                          MN_CALL_EVT_SUPS_CMD_CNF,
                                          ulResult);
}


VOS_UINT32 IMSA_CallProcSpmMsgSups(const SPM_IMSA_CALL_SUPS_CMD_REQ_STRU *pstAppMsg)
{
    VOS_UINT32 ulResult = VOS_TRUE;
    VOS_UINT32 ulCallId = 0;

    IMSA_INFO_LOG("IMSA_CallProcSpmMsgSups is entered!");


    ulCallId = pstAppMsg->stCallMgmtCmd.callId;

    IMSA_INFO_LOG1("IMSA_CallProcSpmMsgSups", pstAppMsg->stCallMgmtCmd.enCallSupsCmd);

    switch (pstAppMsg->stCallMgmtCmd.enCallSupsCmd)
    {
        /* CHLD=0, Releases all held calls or sets User Determined User Busy (UDUB) for a waiting call */
        case MN_CALL_SUPS_CMD_REL_HELD_OR_UDUB:
            ulResult = IMSA_CallProcSpmMsgSupsRelHeldOrUdub(ulCallId, pstAppMsg);
            break;
        /* CHLD=1, Releases all active calls (if any exist) and accepts the other (held or waiting) call */
        case MN_CALL_SUPS_CMD_REL_ACT_ACPT_OTH:
            ulResult = IMSA_CallProcSpmMsgSupsRelActAcptOth(ulCallId, pstAppMsg);
            break;
        /* CHLD=1x, Releases a specific active call X*/
        case MN_CALL_SUPS_CMD_REL_CALL_X:
            ulResult = IMSA_CallProcSpmMsgSupsRelCallX(ulCallId, pstAppMsg);
            break;
        /* ATH, Release all calls */
        case MN_CALL_SUPS_CMD_REL_ALL_CALL:
            ulResult = IMSA_CallProcSpmMsgSupsRelAllCall(ulCallId, pstAppMsg);
            break;
        /* CHLD=2, Places all active calls (if any exist) on hold and accepts the other (held or waiting) call */
        case MN_CALL_SUPS_CMD_HOLD_ACT_ACPT_OTH:
            ulResult = IMSA_CallProcSpmMsgSupsHoldActAcptOth(ulCallId, pstAppMsg);
            break;
        /* CHLD=2x, Places all active calls on hold except call X with which communication shall be supported */
        case MN_CALL_SUPS_CMD_HOLD_ALL_EXCPT_X:
            ulResult = IMSA_CallProcSpmMsgSupsHoldAllExpectX(ulCallId, pstAppMsg);
            break;
        /* CHLD=3, Adds a held call to the conversation */
        case MN_CALL_SUPS_CMD_BUILD_MPTY:
            ulResult = IMSA_CallProcSpmMsgSupsBuidMpty(ulCallId, pstAppMsg);
            break;
        /* reject unsupported SPM command */
        default:
            ulResult = VOS_FALSE;
            IMSA_ERR_LOG("IMSA_CallProcSpmMsgSups: not support sups message");

            (VOS_VOID)IMSA_CallSendSpmSupsCnfMsg(pstAppMsg->usClientId,
                                                 pstAppMsg->ucOpId,
                                                 pstAppMsg->stCallMgmtCmd.callId,
                                                 TAF_CS_CAUSE_IMSA_NOT_SUPPORTED_CMD);
    }

    return ulResult;
}


VOS_UINT32 IMSA_CallSendSpmOrigCnfMsg(VOS_UINT16 usClientId,
                                      VOS_UINT32 ulOpId,
                                      VOS_UINT32 ulCallId,
                                      TAF_CS_CAUSE_ENUM_UINT32 ulResult)
{
    IMSA_INFO_LOG("IMSA_CallSendSpmOrigCnfMsg is entered!");

    return IMSA_CallSendSpmEvtAndCallInfo(usClientId,
                                          ulOpId,
                                          ulCallId,
                                          MN_CALL_EVT_CALL_ORIG_CNF,
                                          ulResult);
}


VOS_UINT32 IMSA_CallProcSpmMsgOrig(SPM_IMSA_CALL_ORIG_REQ_STRU *pstAppMsg)
{
    VOS_UINT32                          ulCheckRet      = 0;
    VOS_UINT32                          ulResult        = 0;
    MN_CALL_ORIG_PARAM_STRU            *pstOrigParam    = VOS_NULL_PTR;
    IMSA_CALL_ENTITY_STRU              *pstCallEntity   = VOS_NULL_PTR;
    IMSA_CALL_MANAGER_STRU             *pstCallCtx      = IMSA_CallCtxGet();
    IMSA_EMC_CALL_TYPE_ENUM_UINT32      enEmcType       = IMSA_EMC_CALL_TYPE_BUTT;

    IMSA_INFO_LOG("IMSA_CallProcSpmMsgOrig is entered!");

    pstOrigParam = &pstAppMsg->stOrig;

    /* Ŀǰֻ֧���������кͽ��������������� */
    if ((MN_CALL_TYPE_EMERGENCY != pstOrigParam->enCallType) &&
        (MN_CALL_TYPE_VOICE != pstOrigParam->enCallType))
    {
        (VOS_VOID)IMSA_CallSendSpmOrigCnfMsg(pstAppMsg->usClientId,
                                             pstAppMsg->ucOpId,
                                             IMSA_CALL_INVALID_ID,
                                             TAF_CS_CAUSE_IMSA_NOT_SUPPORTED_CALL_TYPE);

        return VOS_FALSE;
    }

    /* ���к��п����Լ�� */
    ulCheckRet = IMSA_CallAvailabilityCheck(pstOrigParam->enCallType, &enEmcType);

    IMSA_INFO_LOG1("IMSA_CallProcSpmMsgOrig: IMSA_CallAvailabilityCheck result", ulCheckRet);

    switch (ulCheckRet)
    {
    /* ����ִ�к������� */
    case IMSA_CALL_AVAILABILITY_CHECK_RESULT_CONTINUE:
        {
            if (VOS_FALSE == IMSA_CallCheckIsAllowNewOrig())
            {
                (VOS_VOID)IMSA_CallSendSpmOrigCnfMsg(pstAppMsg->usClientId,
                                                     pstAppMsg->ucOpId,
                                                     IMSA_CALL_INVALID_ID,
                                                     TAF_CS_CAUSE_IMSA_ERROR);
                break;
            }

            pstCallEntity = IMSA_CallEntityAlloc();
            if (VOS_NULL_PTR != pstCallEntity)
            {
                /* �����Ƿ��Ѿ���CS���Թ��ı�ʶ */
                pstCallEntity->bitOpRetryCsCall = pstOrigParam->ucCsCallRetryFlg;

                /* �������ģʽ */
                pstCallEntity->enMode = pstOrigParam->enCallMode;

                /* ������������� */
                pstCallEntity->enEmcType = enEmcType;

                /* ����CLIENT ID��OPID */
                pstCallEntity->usClientId = pstAppMsg->usClientId;
                pstCallEntity->ulOpId = pstAppMsg->ucOpId;

                /* ����CALL���� */
                if (MN_CALL_TYPE_EMERGENCY == pstAppMsg->stOrig.enCallType)
                {
                    pstCallEntity->enType = IMSA_CALL_TYPE_EMC;
                }
                else
                {
                    pstCallEntity->enType = IMSA_CALL_TYPE_NORMAL;
                }

                /* Send IMS command */
                ulResult = IMSA_CallSendImsMsgDial(IMSA_CALL_INVALID_ID, pstAppMsg);
                if (VOS_TRUE == ulResult)
                {
                    /* �������Id */
                    pstCallEntity->ucId = (VOS_UINT8)IMSA_CALL_INVALID_ID;

                    /* ���津���˺��ж�Ӧ��SPM���������б��ܾ�����Ҫ����ע���������ʱʹ�� */
                    IMSA_MEM_CPY(   &pstCallEntity->stSpmMsg,
                                    &pstCallCtx->stSpmMsg,
                                    sizeof(IMSA_CALL_SPM_MSG_STRU));

                    /* ����������ʱ�� */
                    IMSA_StartTimer(&pstCallCtx->stProctectTimer);

                    /* �����ز����ʱ����ʱ�� */
                    pstCallCtx->stRedialMaxTimer.ulTimerLen = IMSA_GetImsRedialCfgAddress()->ulCallRedialMaxTime;
                    pstCallCtx->stRedialMaxTimer.usPara = pstCallEntity->ucCallEntityIndex;
                    IMSA_StartTimer(&pstCallCtx->stRedialMaxTimer);
                    IMSA_INFO_LOG1("Start redial max timer tick:", PS_GET_TICK());
                }
                else
                {
                    /**
                     * NOTE: ��D2 IMS����ʧ�ܣ���ʱֻ��ӡLog��������
                     */
                    IMSA_ERR_LOG("IMSA_CallProcSpmMsgOrig: send IMS msg failure");

                    IMSA_CallEntityFree(pstCallEntity);
                }
            }
            else
            {
                /* ������Դʧ�ܣ��ܾ��ú������� */
                (VOS_VOID)IMSA_CallSendSpmOrigCnfMsg(pstAppMsg->usClientId,
                                                     pstAppMsg->ucOpId,
                                                     IMSA_CALL_INVALID_ID,
                                                     TAF_CS_CAUSE_IMSA_ALLOC_ENTITY_FAIL);
            }
            break;
        }
    /* ���ȷ��������Ҫ����������������ɹ����ٴ�������Ϣ */
    case IMSA_CALL_AVAILABILITY_CHECK_RESULT_CACHED:
        {
            IMSA_CallSpmOrigReqMsgSave(pstAppMsg);
            break;
        }
    /* ���б��ܾ���֪ͨSPM���н���ʧ�� */
    default:
        (VOS_VOID)IMSA_CallSendSpmOrigCnfMsg(pstAppMsg->usClientId,
                                             pstAppMsg->ucOpId,
                                             IMSA_CALL_INVALID_ID,
                                             TAF_CS_CAUSE_IMSA_SERVICE_NOT_AVAILABLE);

        break;
    }

    return VOS_TRUE;
}

/* xiongxianghui00253310 add for conference 20140210 begin */

VOS_UINT32 IMSA_CallSendSpmInviteNewPtptCnfMsg
(
    VOS_UINT16                          usClientId,
    VOS_UINT32                          ulOpId,
    VOS_UINT32                          ulCallId,
    TAF_CS_CAUSE_ENUM_UINT32            ulResult
)
{
    IMSA_SPM_CALL_INVITE_NEW_PTPT_CNF_STRU *pstInviteNewPtptCnf = VOS_NULL_PTR;

    (VOS_VOID)ulCallId;

    IMSA_INFO_LOG("IMSA_CallSendSpmInviteNewPtptCnfMsg is entered!");

    /* ����ռ䲢��������Ƿ�ɹ� */
    pstInviteNewPtptCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_SPM_CALL_INVITE_NEW_PTPT_CNF_STRU));
    if (VOS_NULL_PTR == pstInviteNewPtptCnf)
    {
        IMSA_ERR_LOG("IMSA_CallSendSpmInviteNewPtptCnfMsg: alloc memory fail");
        return VOS_FALSE;
    }

    /* ��� */
    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstInviteNewPtptCnf), 0, IMSA_GET_MSG_LENGTH(pstInviteNewPtptCnf));

    /* ��д��Ϣͷ */
    IMSA_WRITE_SPM_MSG_HEAD(pstInviteNewPtptCnf, ID_IMSA_SPM_CALL_INVITE_NEW_PTPT_CNF);

    pstInviteNewPtptCnf->usClientId = usClientId;
    pstInviteNewPtptCnf->ucOpId     = (MN_OPERATION_ID_T)ulOpId;
    pstInviteNewPtptCnf->enCause    = ulResult;

    /* ������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstInviteNewPtptCnf);

    return VOS_TRUE;
}


/*****************************************************************************
 Function Name  : IMSA_CallProcSpmMsgInviteNewPtpt
 Description    : SPM�·��������������������
 Input          : pstAppMsg      SPM�·����������
 Output         : VOS_VOID
 Return Value   : ������

 History        :
      1.xiongxianghui00253310      2014-07-10  Draft Enact
*****************************************************************************/
VOS_UINT32 IMSA_CallProcSpmMsgInviteNewPtpt(const SPM_IMSA_CALL_INVITE_NEW_PTPT_REQ_STRU *pstAppMsg)
{
    VOS_UINT32 ulResult     = VOS_FALSE;
    VOS_UINT32 ulCheckRst   = VOS_FALSE;
    IMSA_CALL_ENTITY_STRU  *pstCallEntity   = VOS_NULL_PTR;
    IMSA_CALL_MANAGER_STRU *pstCallCtx = IMSA_CallCtxGet();

    IMSA_INFO_LOG("IMSA_CallProcSpmMsgInviteNewPtpt is entered!");

    /* ����������������Լ�� */
    ulCheckRst = IMSA_CallInviteNewPtptAvailableCheck();
    if (VOS_FALSE == ulCheckRst)
    {
        IMSA_INFO_LOG1("IMSA_CallProcSpmMsgInviteNewPtpt: IMSA_CallInviteNewPtptAvailableCheck result", ulCheckRst);

        (VOS_VOID)IMSA_CallSendSpmInviteNewPtptCnfMsg(pstAppMsg->usClientId,
                                                      pstAppMsg->ucOpId,
                                                      IMSA_CALL_INVALID_ID,
                                                      TAF_CS_CAUSE_IMSA_ERROR);

        return ulResult;
    }

    pstCallEntity = IMSA_CallEntityAlloc();
    if (VOS_NULL_PTR != pstCallEntity)
    {
        /* ����CLIENT ID��OPID */
        pstCallEntity->usClientId = pstAppMsg->usClientId;
        pstCallEntity->ulOpId = pstAppMsg->ucOpId;

        ulResult = IMSA_CallSendImsMsgInviteNewPtpt(IMSA_CALL_INVALID_ID, pstAppMsg);
        if (VOS_TRUE == ulResult)
        {
            /* �������Id */
            pstCallEntity->ucId = (VOS_UINT8)IMSA_CALL_INVALID_ID;

            /* ���津���˺��ж�Ӧ��SPM���������б��ܾ�����Ҫ����ע���������ʱʹ�� */
            IMSA_MEM_CPY(   &pstCallEntity->stSpmMsg,
                            &pstCallCtx->stSpmMsg,
                            sizeof(IMSA_CALL_SPM_MSG_STRU));

            /* ����������ʱ�� */
            IMSA_StartTimer(&pstCallCtx->stProctectTimer);
        }
        else
        {
            IMSA_ERR_LOG("IMSA_CallProcSpmMsgInviteNewPtpt: send IMS msg failure");

            IMSA_CallEntityFree(pstCallEntity);
        }
    }
    else
    {
        /* ������Դʧ�ܣ��ܾ������� */
        (VOS_VOID)IMSA_CallSendSpmInviteNewPtptCnfMsg(pstAppMsg->usClientId,
                                                      pstAppMsg->ucOpId,
                                                      IMSA_CALL_INVALID_ID,
                                                      TAF_CS_CAUSE_IMSA_ALLOC_ENTITY_FAIL);
    }

    return ulResult;
}
/* xiongxianghui00253310 add for conference 20140210 end */

VOS_UINT32 IMSA_InsertSrvccBuff
(
    const VOS_UINT8                    *pucData,
    VOS_UINT32                          ulDataLen
)
{
    IMSA_SRVCC_BUFFER_STRU             *pstSrvccBuff = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_InsertSrvccBuff  entered!");

    pstSrvccBuff = IMSA_GetSrvccBuffAddress();

    if (ulDataLen > sizeof(IMSA_SRVCC_BUFF_MSG_UNION))
    {
        IMSA_WARN_LOG("IMSA_InsertSrvccBuff:data len illegal");

        return IMSA_FAIL;
    }

    if (IMSA_CALL_MSG_SYNC_MAX_NUM <= pstSrvccBuff->ucBuffMsgNum)
    {
        IMSA_WARN_LOG("IMSA_InsertSrvccBuff:buffer full");

        return IMSA_FAIL;
    }

    IMSA_MEM_CPY(   &pstSrvccBuff->aucBuffMsgArray[pstSrvccBuff->ucBuffMsgNum],
                    pucData,
                    ulDataLen);

    pstSrvccBuff->ucBuffMsgNum++;

    return IMSA_SUCC;
}


VOS_VOID IMSA_CallOrigReqSrvccingProc
(
    const SPM_IMSA_CALL_ORIG_REQ_STRU  *pstCallOrigReq
)
{
    if (IMSA_FAIL == IMSA_InsertSrvccBuff( (VOS_UINT8*)pstCallOrigReq,
                                            sizeof(SPM_IMSA_CALL_ORIG_REQ_STRU)))
    {
        IMSA_INFO_LOG("IMSA_CallOrigReqSrvccingProc:buffer fail");

        (VOS_VOID)IMSA_CallSendSpmOrigCnfMsg(pstCallOrigReq->usClientId,
                                             pstCallOrigReq->ucOpId,
                                             IMSA_CALL_INVALID_ID,
                                             TAF_CS_CAUSE_IMSA_SRVCCING_BUFF_MSG_FAIL);
    }

    IMSA_INFO_LOG("IMSA_CallOrigReqSrvccingProc:buffer succ");
}


VOS_VOID IMSA_CallSupsCmdReqSrvccingProc
(
    const SPM_IMSA_CALL_SUPS_CMD_REQ_STRU  *pstCallSupsCmdReq
)
{
    if (IMSA_FAIL == IMSA_InsertSrvccBuff( (VOS_UINT8*)pstCallSupsCmdReq,
                                            sizeof(SPM_IMSA_CALL_SUPS_CMD_REQ_STRU)))
    {
        IMSA_INFO_LOG("IMSA_CallSupsCmdReqSrvccingProc:buffer fail");

        (VOS_VOID)IMSA_CallSendSpmSupsCnfMsg(   pstCallSupsCmdReq->usClientId,
                                                pstCallSupsCmdReq->ucOpId,
                                                pstCallSupsCmdReq->stCallMgmtCmd.callId,
                                                TAF_CS_CAUSE_IMSA_SRVCCING_BUFF_MSG_FAIL);
    }

    IMSA_INFO_LOG("IMSA_CallSupsCmdReqSrvccingProc:buffer succ");
}


VOS_VOID IMSA_CallStartDtmfReqSrvccingProc
(
    const SPM_IMSA_CALL_START_DTMF_REQ_STRU  *pstCallStartDtmfReq
)
{
    if (IMSA_FAIL == IMSA_InsertSrvccBuff( (VOS_UINT8*)pstCallStartDtmfReq,
                                            sizeof(SPM_IMSA_CALL_START_DTMF_REQ_STRU)))
    {
        IMSA_INFO_LOG("IMSA_CallStartDtmfReqSrvccingProc:buffer fail");

        (VOS_VOID)IMSA_CallSendSpmStartDtmfCnfMsg(  pstCallStartDtmfReq->usClientId,
                                                    pstCallStartDtmfReq->ucOpId,
                                                    pstCallStartDtmfReq->stDtmf.CallId,
                                                    TAF_CS_CAUSE_IMSA_SRVCCING_BUFF_MSG_FAIL);
    }

    IMSA_INFO_LOG("IMSA_CallStartDtmfReqSrvccingProc:buffer succ");
}


VOS_VOID IMSA_CallStopDtmfReqSrvccingProc
(
    const SPM_IMSA_CALL_STOP_DTMF_REQ_STRU  *pstCallStopDtmfReq
)
{
    if (IMSA_FAIL == IMSA_InsertSrvccBuff( (VOS_UINT8*)pstCallStopDtmfReq,
                                            sizeof(SPM_IMSA_CALL_STOP_DTMF_REQ_STRU)))
    {
        IMSA_INFO_LOG("IMSA_CallStopDtmfReqSrvccingProc:buffer fail");

        (VOS_VOID)IMSA_CallSendSpmStopDtmfCnfMsg(   pstCallStopDtmfReq->usClientId,
                                                    pstCallStopDtmfReq->ucOpId,
                                                    pstCallStopDtmfReq->stDtmf.CallId,
                                                    TAF_CS_CAUSE_IMSA_SRVCCING_BUFF_MSG_FAIL);
    }

    IMSA_INFO_LOG("IMSA_CallStopDtmfReqSrvccingProc:buffer succ");
}


VOS_VOID IMSA_UssdProcUssdReqSrvccingProc
(
    const SPM_IMSA_PROCESS_USSD_REQ_STRU  *pstProcUssdReq
)
{
    if (IMSA_FAIL == IMSA_InsertSrvccBuff( (VOS_UINT8*)pstProcUssdReq,
                                            sizeof(SPM_IMSA_PROCESS_USSD_REQ_STRU)))
    {
        IMSA_INFO_LOG("IMSA_UssdProcUssdReqSrvccingProc:buffer fail");
        (VOS_VOID)IMSA_SendSpmUssdErrorEvt( pstProcUssdReq->usClientId,
                                            pstProcUssdReq->ucOpId,
                                            TAF_ERR_IMSA_SRVCCING_BUFF_MSG_FAIL);
    }

    IMSA_INFO_LOG("IMSA_UssdProcUssdReqSrvccingProc:buffer succ");
}


VOS_VOID IMSA_ClearSrvccBuff( VOS_VOID )
{
    IMSA_MEM_SET(   IMSA_GetSrvccBuffAddress(),
                    0,
                    sizeof(IMSA_SRVCC_BUFFER_STRU));
}


VOS_UINT32 IMSA_SrvccingBufferMsgProc( const VOS_VOID *pBuffMsg )
{
    /* ������Ϣͷָ��*/
    PS_MSG_HEADER_STRU          *pstHeader = VOS_NULL_PTR;

    /* ��ȡ��Ϣͷָ��*/
    pstHeader = (PS_MSG_HEADER_STRU *) pBuffMsg;

    /* �������SRVCC�����У���ֱ���˳� */
    if (IMSA_TRUE != IMSA_CallGetSrvccFlag())
    {
        return IMSA_MSG_NOT_PROC;
    }

    switch (pstHeader->ulMsgName)
    {
        case ID_SPM_IMSA_CALL_ORIG_REQ:

            IMSA_CallOrigReqSrvccingProc(pBuffMsg);
            break;

        case ID_SPM_IMSA_CALL_SUPS_CMD_REQ:

            IMSA_CallSupsCmdReqSrvccingProc(pBuffMsg);
            break;

        case ID_SPM_IMSA_CALL_START_DTMF_REQ:

            IMSA_CallStartDtmfReqSrvccingProc(pBuffMsg);
            break;

        case ID_SPM_IMSA_CALL_STOP_DTMF_REQ:

            IMSA_CallStopDtmfReqSrvccingProc(pBuffMsg);
            break;

        case ID_SPM_IMSA_PROCESS_USSD_REQ:

            IMSA_UssdProcUssdReqSrvccingProc(pBuffMsg);
            break;

        default:

            return IMSA_MSG_NOT_PROC;

    }

    return IMSA_MSG_PROCED;
}
VOS_VOID IMSA_CallSendCallMsgSyncInd
(
    VOS_UINT32                          ulSynMsgNum,
    const VOS_UINT32                   *pulSynMsgIndexArray
)
{
    IMSA_SRVCC_BUFFER_STRU             *pstSrvccBuff    = VOS_NULL_PTR;
    IMSA_CALL_MSG_SYNC_IND_STRU        *pstMsgSyncInd   = VOS_NULL_PTR;
    VOS_UINT32                          i               = IMSA_NULL;

    if ((ulSynMsgNum == 0) || (ulSynMsgNum > IMSA_CALL_MSG_SYNC_MAX_NUM)
        || (VOS_NULL_PTR == pulSynMsgIndexArray))
    {
        return ;
    }

    IMSA_INFO_LOG("IMSA_CallSendCallMsgSyncInd is entered!");

    pstSrvccBuff = IMSA_GetSrvccBuffAddress();

    pstMsgSyncInd = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_CALL_MSG_SYNC_IND_STRU));

    if (VOS_NULL_PTR == pstMsgSyncInd)
    {
        IMSA_ERR_LOG("IMSA_CallSendCallMsgSyncInd: alloc memory fail");
        return ;
    }

    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstMsgSyncInd), 0, IMSA_GET_MSG_LENGTH(pstMsgSyncInd));

    for (i = 0; i < ulSynMsgNum; i++)
    {
        IMSA_MEM_CPY(   &pstMsgSyncInd->astMsgArray[pstMsgSyncInd->ucMsgNum],
                        &pstSrvccBuff->aucBuffMsgArray[pulSynMsgIndexArray[i]],
                        sizeof(IMSA_CALL_MSG_UNION));

        pstMsgSyncInd->ucMsgNum++;
    }

    /* ��д��Ϣͷ */
    IMSA_WRITE_CALL_MSG_HEAD(pstMsgSyncInd, ID_IMSA_CALL_MSG_SYNC_IND);

    /* ������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstMsgSyncInd);
}


VOS_VOID IMSA_SrvccSuccBuffProc( VOS_VOID )
{
    IMSA_SRVCC_BUFFER_STRU             *pstSrvccBuff    = VOS_NULL_PTR;
    PS_MSG_HEADER_STRU                 *pstHeader       = VOS_NULL_PTR;
    SPM_IMSA_PROCESS_USSD_REQ_STRU     *pstUssdReq      = VOS_NULL_PTR;
    VOS_UINT32                          ulSynMsgNum     = IMSA_NULL;
    VOS_UINT32                          aulSynMsgIndexArray[IMSA_CALL_MSG_SYNC_MAX_NUM] = {0};
    VOS_UINT32                          i               = IMSA_NULL;

    IMSA_INFO_LOG("IMSA_SrvccSuccBuffProc  entered!");

    pstSrvccBuff = IMSA_GetSrvccBuffAddress();

    if (pstSrvccBuff->ucBuffMsgNum == 0)
    {
        IMSA_INFO_LOG("IMSA_SrvccSuccBuffProc:no buffer msg");
        return ;
    }

    for (i = 0; i < pstSrvccBuff->ucBuffMsgNum; i++)
    {
        pstHeader = (PS_MSG_HEADER_STRU *)(&pstSrvccBuff->aucBuffMsgArray[i]);

        if (pstHeader->ulMsgName != ID_SPM_IMSA_PROCESS_USSD_REQ)
        {
            aulSynMsgIndexArray[ulSynMsgNum] = i;
            ulSynMsgNum++;
            continue;
        }

        /* ��USSD֮ǰ�Ļ�����Ϣ����CALLģ�� */
        IMSA_CallSendCallMsgSyncInd(ulSynMsgNum, aulSynMsgIndexArray);
        ulSynMsgNum = 0;

        /* �ظ�ʧ�ܣ�ԭ��ֵΪSRVCC_SUCC������SPM��CS���ز� */
        pstUssdReq = (SPM_IMSA_PROCESS_USSD_REQ_STRU*)pstHeader;
        (VOS_VOID)IMSA_SendSpmUssdErrorEvt( pstUssdReq->usClientId,
                                            pstUssdReq->ucOpId,
                                            TAF_ERR_IMSA_SRVCCING_BUFF_MSG);
    }

    IMSA_CallSendCallMsgSyncInd(ulSynMsgNum, aulSynMsgIndexArray);
    ulSynMsgNum = 0;

    /* �建�� */
    IMSA_ClearSrvccBuff();
}
VOS_VOID IMSA_SrvccFailBuffProc( VOS_VOID )
{
    IMSA_SRVCC_BUFFER_STRU             *pstSrvccBuff    = VOS_NULL_PTR;
    PS_MSG_HEADER_STRU                 *pstHeader       = VOS_NULL_PTR;
    VOS_UINT32                          i               = IMSA_NULL;

    IMSA_INFO_LOG("IMSA_SrvccFailBuffProc  entered!");

    pstSrvccBuff = IMSA_GetSrvccBuffAddress();

    if (pstSrvccBuff->ucBuffMsgNum == 0)
    {
        IMSA_INFO_LOG("IMSA_SrvccFailBuffProc:no buffer msg");
        return ;
    }

    for (i = 0; i < pstSrvccBuff->ucBuffMsgNum; i++)
    {
         pstHeader = (PS_MSG_HEADER_STRU *)(&pstSrvccBuff->aucBuffMsgArray[i]);

         switch (pstHeader->ulMsgName)
         {
            case ID_SPM_IMSA_CALL_ORIG_REQ:

                (VOS_VOID)IMSA_CallProcSpmMsgOrig((SPM_IMSA_CALL_ORIG_REQ_STRU *)&pstSrvccBuff->aucBuffMsgArray[i]);
                break;

            case ID_SPM_IMSA_CALL_SUPS_CMD_REQ:

                (VOS_VOID)IMSA_CallProcSpmMsgSups((SPM_IMSA_CALL_SUPS_CMD_REQ_STRU *)&pstSrvccBuff->aucBuffMsgArray[i]);
                break;

            case ID_SPM_IMSA_CALL_START_DTMF_REQ:

                IMSA_CallProcSpmMsgStartDtmf((SPM_IMSA_CALL_START_DTMF_REQ_STRU *)&pstSrvccBuff->aucBuffMsgArray[i]);
                break;

            case ID_SPM_IMSA_CALL_STOP_DTMF_REQ:

                IMSA_CallProcSpmMsgStopDtmf((SPM_IMSA_CALL_STOP_DTMF_REQ_STRU *)&pstSrvccBuff->aucBuffMsgArray[i]);
                break;

            case ID_SPM_IMSA_PROCESS_USSD_REQ:

                IMSA_SsProcSpmMsgProcessUssdReq((SPM_IMSA_PROCESS_USSD_REQ_STRU *)&pstSrvccBuff->aucBuffMsgArray[i]);
                break;

            default:

                IMSA_WARN_LOG("IMSA_SrvccFailBuffProc:illegal buffer msg type!");
                break;
         }
    }

    /* �建�� */
    IMSA_ClearSrvccBuff();
}
VOS_VOID IMSA_SrvccAbormalClearBuff
(
    IMSA_SRVCC_ABNORMAL_TYPE_ENUM_UINT32    enAbnormalType
)
{
    IMSA_SRVCC_BUFFER_STRU             *pstSrvccBuff    = VOS_NULL_PTR;
    PS_MSG_HEADER_STRU                 *pstHeader       = VOS_NULL_PTR;
    VOS_UINT32                          i               = IMSA_NULL;
    TAF_CS_CAUSE_ENUM_UINT32            ulTafCsCause    = TAF_CS_CAUSE_IMSA_SRVCC_ABNORMAL;
    VOS_UINT16                          usUssdError     = TAF_ERR_IMSA_SRVCC_ABNORMAL;

    pstSrvccBuff = IMSA_GetSrvccBuffAddress();

    if (pstSrvccBuff->ucBuffMsgNum == 0)
    {
        IMSA_INFO_LOG("IMSA_SrvccAbormalClearBuff:no buffer msg");
        return ;
    }

    IMSA_INFO_LOG("IMSA_SrvccAbormalClearBuff  entered!");

    if (IMSA_SRVCC_ABNORMAL_DEREG_REQ == enAbnormalType)
    {
        ulTafCsCause    = TAF_CS_CAUSE_IMSA_SRVCC_ABNORMAL_DEREG;
        usUssdError     = TAF_ERR_IMSA_SRVCC_ABNORMAL_DEREG;
    }

    for (i = 0; i < pstSrvccBuff->ucBuffMsgNum; i++)
    {
         pstHeader = (PS_MSG_HEADER_STRU *)(&pstSrvccBuff->aucBuffMsgArray[i]);

         switch (pstHeader->ulMsgName)
         {
            case ID_SPM_IMSA_CALL_ORIG_REQ:

                (VOS_VOID)IMSA_CallSendSpmOrigCnfMsg(((SPM_IMSA_CALL_ORIG_REQ_STRU*)pstHeader)->usClientId,
                                                    ((SPM_IMSA_CALL_ORIG_REQ_STRU*)pstHeader)->ucOpId,
                                                    IMSA_CALL_INVALID_ID,
                                                    ulTafCsCause);
                break;

            case ID_SPM_IMSA_CALL_SUPS_CMD_REQ:

                (VOS_VOID)IMSA_CallSendSpmSupsCnfMsg(((SPM_IMSA_CALL_SUPS_CMD_REQ_STRU*)pstHeader)->usClientId,
                                                    ((SPM_IMSA_CALL_SUPS_CMD_REQ_STRU*)pstHeader)->ucOpId,
                                                    ((SPM_IMSA_CALL_SUPS_CMD_REQ_STRU*)pstHeader)->stCallMgmtCmd.callId,
                                                    ulTafCsCause);
                break;

            case ID_SPM_IMSA_CALL_START_DTMF_REQ:

                (VOS_VOID)IMSA_CallSendSpmStartDtmfCnfMsg(  ((SPM_IMSA_CALL_START_DTMF_REQ_STRU*)pstHeader)->usClientId,
                                                            ((SPM_IMSA_CALL_START_DTMF_REQ_STRU*)pstHeader)->ucOpId,
                                                            ((SPM_IMSA_CALL_START_DTMF_REQ_STRU*)pstHeader)->stDtmf.CallId,
                                                            ulTafCsCause);
                break;

            case ID_SPM_IMSA_CALL_STOP_DTMF_REQ:

                (VOS_VOID)IMSA_CallSendSpmStopDtmfCnfMsg(   ((SPM_IMSA_CALL_STOP_DTMF_REQ_STRU*)pstHeader)->usClientId,
                                                            ((SPM_IMSA_CALL_STOP_DTMF_REQ_STRU*)pstHeader)->ucOpId,
                                                            ((SPM_IMSA_CALL_STOP_DTMF_REQ_STRU*)pstHeader)->stDtmf.CallId,
                                                            ulTafCsCause);
                break;

            case ID_SPM_IMSA_PROCESS_USSD_REQ:

                /* �ظ�ʧ�ܣ�ԭ��ֵΪSRVICE NOT AVAILABLE */
                (VOS_VOID)IMSA_SendSpmUssdErrorEvt( ((SPM_IMSA_PROCESS_USSD_REQ_STRU*)pstHeader)->usClientId,
                                                    ((SPM_IMSA_PROCESS_USSD_REQ_STRU*)pstHeader)->ucOpId,
                                                    usUssdError);
                break;

            default:

                IMSA_WARN_LOG("IMSA_SrvccAbormalClearBuff:illegal buffer msg type!");
                break;
         }
    }

    /* �建�� */
    IMSA_ClearSrvccBuff();
}


VOS_VOID IMSA_ProcSpmCallMsg(const VOS_VOID *pRcvMsg)
{
    /* ������Ϣͷָ��*/
    PS_MSG_HEADER_STRU                 *pstHeader = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_ProcSpmCallMsg is entered!");

    /* ��ȡ��Ϣͷָ��*/
    pstHeader = (PS_MSG_HEADER_STRU *) pRcvMsg;

    if (VOS_NULL_PTR == pstHeader)
    {
        IMSA_ERR_LOG("IMSA_ProcSpmCallMsg: NULL message");
        return;
    }

    IMSA_INFO_LOG1("IMSA_ProcSpmCallMsg: msg id", pstHeader->ulMsgName);

    if (IMSA_MSG_PROCED == IMSA_SrvccingBufferMsgProc(pRcvMsg))
    {
        IMSA_INFO_LOG("IMSA_ProcSpmCallMsg: Srvccing buffer!");
        return ;
    }

    switch(pstHeader->ulMsgName)
    {
        /* ����������� */
        case ID_SPM_IMSA_CALL_ORIG_REQ:
            (VOS_VOID)IMSA_CallProcSpmMsgOrig((SPM_IMSA_CALL_ORIG_REQ_STRU*)pRcvMsg);
            break;
        case ID_SPM_IMSA_CALL_SUPS_CMD_REQ:
            (VOS_VOID)IMSA_CallProcSpmMsgSups((SPM_IMSA_CALL_SUPS_CMD_REQ_STRU*)pRcvMsg);
            break;
        case ID_SPM_IMSA_CALL_START_DTMF_REQ:
            IMSA_CallProcSpmMsgStartDtmf((SPM_IMSA_CALL_START_DTMF_REQ_STRU*)pRcvMsg);
            break;
        case ID_SPM_IMSA_CALL_STOP_DTMF_REQ:
            IMSA_CallProcSpmMsgStopDtmf((SPM_IMSA_CALL_STOP_DTMF_REQ_STRU*)pRcvMsg);
            break;
        case ID_SPM_IMSA_CALL_GET_CALL_INFO_REQ:
            (VOS_VOID)IMSA_CallProcSpmMsgGetInfo((SPM_IMSA_CALL_GET_CALL_INFO_REQ_STRU *)pRcvMsg);
            break;
        case ID_SPM_IMSA_CALL_GET_CLPR_REQ:
            (VOS_VOID)IMSA_CallProcSpmMsgGetClprInfo((SPM_IMSA_CALL_GET_CLPR_REQ_STRU *)pRcvMsg);
            break;
        case ID_SPM_IMSA_PROCESS_USSD_REQ:
            IMSA_SsProcSpmMsgProcessUssdReq((SPM_IMSA_PROCESS_USSD_REQ_STRU *)pRcvMsg);
            break;
        case ID_SPM_IMSA_RELEASE_REQ:
            IMSA_SsProcSpmMsgReleaseReq((SPM_IMSA_RELEASE_REQ_STRU*)pRcvMsg);
            break;
        /* xiongxianghui00253310 add for conference 20140210 begin */
        case ID_SPM_IMSA_CALL_INVITE_NEW_PTPT_REQ:
            (VOS_VOID)IMSA_CallProcSpmMsgInviteNewPtpt((SPM_IMSA_CALL_INVITE_NEW_PTPT_REQ_STRU *)pRcvMsg);
            break;
        /* xiongxianghui00253310 add for conference 20140210 end */
        default:
            IMSA_ERR_LOG("IMSA_ProcSpmCallMsg: Not support call message");
            break;
            /*(VOS_VOID)IMSA_CallSendSpmErrorInd(pstAppReq->usClientId,
                                               pstAppReq->ucOpId,
                                               pstAppReq->callId,
                                               TAF_CS_CAUSE_IMSA_NOT_SUPPORTED_CMD);*/
    }
}
/*lint +e960*/
/*lint +e961*/


#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
/* end of ImsaProcSpmMsg.c */



