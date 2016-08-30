/******************************************************************************

   Copyright(C)2013,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : ImsaProcImsCallMsg.c
  Description     : ��C�ļ�ʵ��IMS������Ϣ�����IMS������Ϣ����
  History           :
     1.sunbing 49683      2013-06-19  Draft Enact

******************************************************************************/

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "ImsaProcImsCallMsg.h"
#include "ImsaEntity.h"
#include "ImsaImsEvent.h"
#include "ImsaImsApi.h"
#include "ImsaEntity.h"
#include "ImsaCallManagement.h"
#include "ImsaPublic.h"
#include "ImsaRegManagement.h"
#include "ImsaImsInterface.h"
#include "CallImsaInterface.h"
#include "MnClient.h"
#include "ImsaProcSpmMsg.h"


/*lint -e767*/
#define    THIS_FILE_ID    PS_FILE_ID_IMSAPROCIMSCALLMSG_C
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

VOS_UINT32 IMSA_CallProcImsStateActive(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary);
VOS_UINT32 IMSA_CallProcImsStateHeld(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary);
VOS_UINT32 IMSA_CallProcImsStateTrying(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary);
VOS_UINT32 IMSA_CallProcImsStateDialing(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary);
VOS_UINT32 IMSA_CallProcImsStateAlerting(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary);
VOS_UINT32 IMSA_CallProcImsStateIncoming(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary);
VOS_UINT32 IMSA_CallProcImsStateWaiting(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary);
VOS_UINT32 IMSA_CallProcImsStateInitializing(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary);

VOS_UINT32 IMSA_CallProcImsState(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary);
MN_CALL_STATE_ENUM_UINT8 IMSA_CallImsSrvccState2TafCallState(IMSA_IMS_CALL_STATE_ENUM_UINT8  enCallState);

extern VOS_VOID IMSA_CallEntityUpdateByCallSummary
(
    IMSA_CALL_ENTITY_STRU *pstCallEntity,
    const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary
);
extern VOS_VOID  IMSA_CallRcvImsStartDtmfOk( VOS_VOID );
extern VOS_VOID  IMSA_CallRcvImsStopDtmfOk( VOS_VOID );
extern VOS_UINT32 IMSA_CallProcImsMsgOk(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent);
extern VOS_VOID IMSA_CallEntitySaveErrorInfo
(
    IMSA_CALL_ENTITY_STRU                  *pstCallEntity,
    const IMSA_IMS_OUTPUT_CALL_EVENT_STRU  *pstOutputCallEvent
);
extern VOS_VOID  IMSA_CallRcvImsStartDtmfError( VOS_VOID );
extern VOS_UINT32 IMSA_CallProcImsMsgError(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent);
extern VOS_UINT32 IMSA_CallProcImsMsgIncoming(IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent);
extern VOS_UINT32 IMSA_CallEntityProcErrorInfo380ForNormal
(
    IMSA_CALL_ENTITY_STRU *pstCallEntity,
    const IMSA_IMS_CALL_ERROR_STRU *pstImsError
);
extern VOS_UINT32 IMSA_CallEntityProcErrorInfo380ForEmc
(
    const IMSA_CALL_ENTITY_STRU        *pstCallEntity,
    const IMSA_IMS_CALL_ERROR_STRU     *pstImsError
);
extern VOS_UINT32 IMSA_CallEntityProcErrorInfo380
(
    IMSA_CALL_ENTITY_STRU *pstCallEntity,
    const IMSA_IMS_CALL_ERROR_STRU *pstImsError
);
extern VOS_UINT32 IMSA_CallEntityProcErrorInfo503
(
    IMSA_CALL_ENTITY_STRU *pstCallEntity,
    const IMSA_IMS_CALL_ERROR_STRU *pstImsError
);
extern VOS_UINT32 IMSA_CallEntityProcErrorInfo504
(
    IMSA_CALL_ENTITY_STRU *pstCallEntity,
    const IMSA_IMS_CALL_ERROR_STRU *pstImsError
);
extern VOS_UINT32 IMSA_CallEntityProcErrorInfo
(
    IMSA_CALL_ENTITY_STRU *pstCallEntity,
    const IMSA_IMS_CALL_ERROR_STRU *pstImsError
);
/*extern TAF_CS_CAUSE_ENUM_UINT32 IMSA_CallTransImsErr2SpmErr
(
    VOS_UINT16                          usImsErr
);
extern VOS_VOID IMSA_CallReleaseCallCommonProc
(
    IMSA_CALL_ENTITY_STRU              *pstCallEntity,
    TAF_CS_CAUSE_ENUM_UINT32            ulResult
);*/
extern VOS_UINT32 IMSA_CallProcImsMsgDisconnect(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent);
extern VOS_UINT32 IMSA_CallProcImsMsgWaiting(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent);
extern VOS_UINT32 IMSA_CallProcImsMsgCallEarlyMedia(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent);
extern VOS_UINT32 IMSA_CallProcImsMsgInitializing(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent);
extern VOS_VOID IMSA_CallProcImsMsgSrvccCallList(IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent);
extern VOS_UINT32 IMSA_CallProcImsMsgEmergencyInd(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent);
extern VOS_UINT32 IMSA_CallIsProccessCallSwap(VOS_VOID);
extern VOS_UINT32 IMSA_CallIsAllCallOnHold(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent);
extern VOS_UINT32 IMSA_CallIsAllCallInConference(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent);
extern VOS_UINT32 IMSA_CallProcImsMsgCallList(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent);
extern VOS_UINT32 IMSA_CallProcImsStateInvalid
(
    IMSA_CALL_ENTITY_STRU *pstCallEntity,
    const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary
);
extern IMSA_CALL_STATUS_ENUM_UINT8 IMSA_CallImsState2ImsaState
(
    IMSA_IMS_CALL_STATE_ENUM_UINT8 enImsState,
    IMSA_CALL_STATUS_ENUM_UINT8 enOldState
);
extern VOS_UINT32 IMSA_CallAllStateWillBeHeld(VOS_UINT32 ulWillChangedId);

/*****************************************************************************
  3 Function
*****************************************************************************/
/*lint -e960*/
/*lint -e961*/

VOS_VOID IMSA_CallEntityUpdateByCallSummary(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary)
{

    /* ���º���Id */
    pstCallEntity->ucId = (VOS_UINT8)pstCallSummary->ulCallIndex;

    /* ���º��з��� */
    if (IMSA_IMS_CALL_DIR_MOBILE_ORIGINATED == pstCallSummary->enCallDirection)
    {
        pstCallEntity->enDir = MN_CALL_DIR_MO;
    }
    else
    {
        pstCallEntity->enDir = MN_CALL_DIR_MT;
    }

    /* ���º��з�ʽ */
    /**
     * ����ģʽʹ��ORIG REQ�����������ֵ��
     * ���ﲻ���и���
     */

    /* ���¶෽���б�ʶ */
    if (IMSA_IMS_CALL_SINGLE_PARTY == pstCallSummary->enMultiParty)
    {
        pstCallEntity->enMpty = MN_CALL_NOT_IN_MPTY;
    }
    else
    {
        pstCallEntity->enMpty = MN_CALL_IN_MPTY;
    }

    /* �������к�����߱��к��� */
    if (IMSA_IMS_CALL_DIR_MOBILE_ORIGINATED == pstCallSummary->enCallDirection)
    {
        IMSA_CallEntityUpdateCalledNumberByAscii((VOS_CHAR *)pstCallSummary->acNumber, &pstCallEntity->stCalledNumber);
        IMSA_CallEntityUpdateCallNumberByAscii((VOS_CHAR *)pstCallSummary->acConnectNumber, &pstCallEntity->stConnectNumber);
    }
    else
    {
        IMSA_CallEntityUpdateCallNumberByAscii((VOS_CHAR *)pstCallSummary->acNumber, &pstCallEntity->stCallNumber);

        if (IMSA_IMS_CALL_STATE_INCOMING == pstCallSummary->enCallState)
        {
            /* ֻ����INCOMMING״̬�£��Ÿ��º���ת�ƺ��룬����״̬�£��ú��벻������û�б�Ҫ���� */
            IMSA_CallEntityUpdateCallNumberByAscii((VOS_CHAR *)pstCallSummary->acRedirectNumber, &pstCallEntity->stRedirectNumber);
        }
    }
}
VOS_VOID IMSA_CallDtmfInfoInit(VOS_VOID)
{
    VOS_UINT8                          ucIndex = 0;

    for(ucIndex = 0; ucIndex < IMSA_CALL_DTMF_BUF_MAX_NUM; ucIndex++)
    {
        IMSA_MEM_SET(&IMSA_CallGetDtmfBufList(ucIndex), 0, sizeof(IMSA_CALL_DTMF_INFO_STRU));
    }

    IMSA_MEM_SET(IMSA_CallGetCurrentDtmfInfo(), 0, sizeof(IMSA_CALL_DTMF_INFO_STRU));

    IMSA_CallSetDtmfState(IMSA_CALL_DTMF_IDLE);

    IMSA_CallGetDtmfCnt() = 0;

    return;

}



TAF_CS_CAUSE_ENUM_UINT32 IMSA_CallSaveDtmfInfo(VOS_UINT8 ucCallId,
                                                      VOS_CHAR  cKey,
                                                      VOS_UINT16 usDuration,
                                                      VOS_UINT16 usClientId,
                                                      VOS_UINT8  ucSpmOpid)
{
    IMSA_CALL_DTMF_INFO_STRU stDtmfInfo;

    IMSA_MEM_SET(&stDtmfInfo, 0, sizeof(IMSA_CALL_DTMF_INFO_STRU));

    /* �жϻ����Ƿ��Ѿ����� */
    if(IMSA_CallGetDtmfCnt() >= IMSA_CALL_DTMF_BUF_MAX_NUM)
    {
        return TAF_CS_CAUSE_DTMF_BUF_FULL;
    }

    /* �ж��Ƿ�������������STOP DTMF����*/
    if((0 == usDuration) && (IMSA_CallGetDtmfCnt() >= 1))
    {
        if(0 == IMSA_CallGetDtmfBufList(IMSA_CallGetDtmfCnt() - 1).usOnLength)
        {
            return TAF_CS_CAUSE_DTMF_REPEAT_STOP;
        }
    }

    /* ����ɹ� */
    stDtmfInfo.ucCallId = ucCallId;
    stDtmfInfo.cKey = cKey;
    stDtmfInfo.usOnLength = usDuration;
    stDtmfInfo.usClientId = usClientId;
    stDtmfInfo.ucSpmOpId = ucSpmOpid;

    IMSA_MEM_CPY(&IMSA_CallGetDtmfBufList(IMSA_CallGetDtmfCnt()),
                 &stDtmfInfo,
                 sizeof(IMSA_CALL_DTMF_INFO_STRU));
    IMSA_CallGetDtmfCnt()++;

    return TAF_CS_CAUSE_SUCCESS;
}
VOS_VOID IMSA_CallDeleteSavedDtmfInfo(VOS_UINT8 ucIndex)
{
    VOS_UINT8 i = 0;

    if(ucIndex >= IMSA_CallGetDtmfCnt())
    {
        return;
    }
    for(i = 0; i < ((IMSA_CallGetDtmfCnt() - ucIndex) - 1); i++)
    {
        IMSA_MEM_CPY(&IMSA_CallGetDtmfBufList(ucIndex + i),
                     &IMSA_CallGetDtmfBufList(ucIndex + i + 1),
                     sizeof(IMSA_CALL_DTMF_INFO_STRU));
    }

    IMSA_MEM_SET(&IMSA_CallGetDtmfBufList(IMSA_CallGetDtmfCnt() - 1), 0, sizeof(IMSA_CALL_DTMF_INFO_STRU));

    IMSA_CallGetDtmfCnt()--;

    return;
}


VOS_VOID IMSA_CallClearDtmfInfo(TAF_CS_CAUSE_ENUM_UINT32 enCause)
{
    IMSA_CALL_MANAGER_STRU            *pstCallCtx  = IMSA_CallCtxGet();
    VOS_UINT8                          ucIndex = 0;

    IMSA_INFO_LOG("IMSA_CallClearDtmfInfo is entered!");

    /* ��������DTMF��Ϣ */
    for(ucIndex = 0; ucIndex < IMSA_CallGetDtmfCnt(); ucIndex++)
    {
        if(0 == IMSA_CallGetDtmfBufList(ucIndex).usOnLength)
        {
            (VOS_VOID)IMSA_CallSendSpmStopDtmfRsltMsg(IMSA_CallGetDtmfBufList(ucIndex).usClientId,
                                                      IMSA_CallGetDtmfBufList(ucIndex).ucSpmOpId,
                                                      IMSA_CallGetDtmfBufList(ucIndex).ucCallId,
                                                      enCause);
        }
        else
        {
            (VOS_VOID)IMSA_CallSendSpmStartDtmfRsltMsg(IMSA_CallGetDtmfBufList(ucIndex).usClientId,
                                                       IMSA_CallGetDtmfBufList(ucIndex).ucSpmOpId,
                                                       IMSA_CallGetDtmfBufList(ucIndex).ucCallId,
                                                       enCause);
        }

        IMSA_MEM_SET(&IMSA_CallGetDtmfBufList(ucIndex), 0, sizeof(IMSA_CALL_DTMF_INFO_STRU));
    }

    /* �����ǰ��DTMF��Ϣ */
    switch(IMSA_CallGetDtmfState())
    {
    case IMSA_CALL_DTMF_WAIT_START_CNF:
        IMSA_StopTimer(&pstCallCtx->stDtmfCtx.stDtmfProtectTimer);
        (VOS_VOID)IMSA_CallSendSpmStartDtmfRsltMsg(IMSA_CallGetCurrentDtmfInfo()->usClientId,
                                                   IMSA_CallGetCurrentDtmfInfo()->ucSpmOpId,
                                                   IMSA_CallGetCurrentDtmfInfo()->ucCallId,
                                                   enCause);
        break;
    case IMSA_CALL_DTMF_WAIT_STOP_CNF:
        IMSA_StopTimer(&pstCallCtx->stDtmfCtx.stDtmfProtectTimer);
        (VOS_VOID)IMSA_CallSendSpmStopDtmfRsltMsg(IMSA_CallGetCurrentDtmfInfo()->usClientId,
                                                  IMSA_CallGetCurrentDtmfInfo()->ucSpmOpId,
                                                  IMSA_CallGetCurrentDtmfInfo()->ucCallId,
                                                  enCause);
        break;
    case IMSA_CALL_DTMF_WAIT_AUTO_STOP_CNF:
        IMSA_StopTimer(&pstCallCtx->stDtmfCtx.stDtmfProtectTimer);
        break;
    case IMSA_CALL_DTMF_WAIT_TIME_OUT:
        IMSA_StopTimer(&pstCallCtx->stDtmfCtx.stDtmfDurationTimer);
        break;
    default:
        IMSA_INFO_LOG("IMSA_CallClearDtmfInfo: nothing process on this state");
        break;
    }

    IMSA_MEM_SET(IMSA_CallGetCurrentDtmfInfo(), 0, sizeof(IMSA_CALL_DTMF_INFO_STRU));

    IMSA_CallSetDtmfState(IMSA_CALL_DTMF_IDLE);

    IMSA_CallGetDtmfCnt() = 0;

    return;
}


VOS_VOID IMSA_CallSrvccSuccClearDtmfInfo(TAF_CS_CAUSE_ENUM_UINT32 enCause)
{
    IMSA_CALL_MANAGER_STRU            *pstCallCtx  = IMSA_CallCtxGet();
    VOS_UINT8                          ucIndex = 0;

    IMSA_INFO_LOG("IMSA_CallSrvccSuccClearDtmfInfo is entered!");

    /* ��������DTMF��Ϣ�����ûظ������SRVCC�ɹ�����CS��������� */
    for(ucIndex = 0; ucIndex < IMSA_CallGetDtmfCnt(); ucIndex++)
    {
        IMSA_MEM_SET(&IMSA_CallGetDtmfBufList(ucIndex), 0, sizeof(IMSA_CALL_DTMF_INFO_STRU));
    }

    /* �����ǰ��DTMF��Ϣ */
    switch(IMSA_CallGetDtmfState())
    {
    case IMSA_CALL_DTMF_WAIT_START_CNF:
        IMSA_StopTimer(&pstCallCtx->stDtmfCtx.stDtmfProtectTimer);
        (VOS_VOID)IMSA_CallSendSpmStartDtmfRsltMsg(IMSA_CallGetCurrentDtmfInfo()->usClientId,
                                                   IMSA_CallGetCurrentDtmfInfo()->ucSpmOpId,
                                                   IMSA_CallGetCurrentDtmfInfo()->ucCallId,
                                                   enCause);
        break;
    case IMSA_CALL_DTMF_WAIT_STOP_CNF:
        IMSA_StopTimer(&pstCallCtx->stDtmfCtx.stDtmfProtectTimer);
        (VOS_VOID)IMSA_CallSendSpmStopDtmfRsltMsg(IMSA_CallGetCurrentDtmfInfo()->usClientId,
                                                  IMSA_CallGetCurrentDtmfInfo()->ucSpmOpId,
                                                  IMSA_CallGetCurrentDtmfInfo()->ucCallId,
                                                  enCause);
        break;
    case IMSA_CALL_DTMF_WAIT_AUTO_STOP_CNF:
        IMSA_StopTimer(&pstCallCtx->stDtmfCtx.stDtmfProtectTimer);
        break;
    case IMSA_CALL_DTMF_WAIT_TIME_OUT:
        IMSA_StopTimer(&pstCallCtx->stDtmfCtx.stDtmfDurationTimer);
        break;
    default:
        IMSA_INFO_LOG("IMSA_CallSrvccSuccClearDtmfInfo: nothing process on this state");
        break;
    }

    IMSA_MEM_SET(IMSA_CallGetCurrentDtmfInfo(), 0, sizeof(IMSA_CALL_DTMF_INFO_STRU));

    IMSA_CallSetDtmfState(IMSA_CALL_DTMF_IDLE);

    IMSA_CallGetDtmfCnt() = 0;

    return;
}



/*****************************************************************************
 Function Name  : IMSA_CallRcvImsStartDtmfOk
 Description    : start dtmf�ɹ���Ϣ����
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : ������

 History        :
      1.lihong 0015001     2013-10-14  Draft Enact
*****************************************************************************/
VOS_VOID  IMSA_CallRcvImsStartDtmfOk( VOS_VOID )
{
    IMSA_CALL_DTMF_INFO_STRU           *pstDtmfCurInfo  = VOS_NULL_PTR;
    IMSA_CALL_MANAGER_STRU             *pstCallCtx      = IMSA_CallCtxGet();

    VOS_UINT32                          ulResult = VOS_FALSE;

    /* �ж�״̬�Ƿ��쳣 */
    if (IMSA_CallGetDtmfState() != IMSA_CALL_DTMF_WAIT_START_CNF)
    {
        IMSA_WARN_LOG("IMSA_CallRcvImsStartDtmfOk: Dtmf State Error!");
        return;
    }

    /* ֹͣDTMF������ʱ�� */
    IMSA_StopTimer(&pstCallCtx->stDtmfCtx.stDtmfProtectTimer);

    /* �ظ�DTMF��ʽ��Ӧ�¼� */
    pstDtmfCurInfo  = IMSA_CallGetCurrentDtmfInfo();
    (VOS_VOID)IMSA_CallSendSpmStartDtmfRsltMsg( pstDtmfCurInfo->usClientId,
                                                pstDtmfCurInfo->ucSpmOpId,
                                                pstDtmfCurInfo->ucCallId,
                                                TAF_CS_CAUSE_SUCCESS);

    /* ����л�����ΪSTOP_DTMF������ֱ�ӷ��� */
    if(IMSA_CallGetDtmfCnt() > 0)
    {
        /* �ж��Ƿ��к��ʵ�CALL������DTMF�� */
        ulResult = IMSA_CallCheckUserDtmfCallId(0);
        if(TAF_CS_CAUSE_SUCCESS != ulResult)
        {
            IMSA_WARN_LOG("IMSA_CallRcvImsStartDtmfOk: Call Check User Dtmf Fail!");

            IMSA_CallClearDtmfInfo(TAF_CS_CAUSE_UNKNOWN);
            return;
        }

        if(0 == IMSA_CallGetDtmfBufList(0).usOnLength)
        {
            /* ��IMS����STOP DTMF */
            if (VOS_TRUE != IMSA_CallSendImsMsgStopDtmf(0, IMSA_CallGetDtmfBufList(0).cKey))
            {
                IMSA_WARN_LOG("IMSA_CallRcvImsStartDtmfOk: Send Stop Dtmf Fail!");

                IMSA_CallClearDtmfInfo(TAF_CS_CAUSE_UNKNOWN);
                return;
            }

            /* ���ͳɹ�������������ʱ��������״̬*/
            IMSA_StartTimer(&pstCallCtx->stDtmfCtx.stDtmfProtectTimer);
            IMSA_CallSetDtmfState(IMSA_CALL_DTMF_WAIT_STOP_CNF);
            IMSA_MEM_CPY(IMSA_CallGetCurrentDtmfInfo(),
                         &IMSA_CallGetDtmfBufList(0),
                         sizeof(IMSA_CALL_DTMF_INFO_STRU));
            IMSA_CallGetCurrentDtmfInfo()->ulImsOpid = IMSA_GetImsOpId();

            /* ɾ������STOP DTMF����*/
            IMSA_CallDeleteSavedDtmfInfo(0);
            return;
        }
    }

    /* û�л��棬�򻺴�ΪSTART_DTMF����������dtmf duration��ʱ��������DTMF״̬ */
    pstCallCtx->stDtmfCtx.stDtmfDurationTimer.ulTimerLen = (VOS_UINT32)pstDtmfCurInfo->usOnLength;
    IMSA_StartTimer(&pstCallCtx->stDtmfCtx.stDtmfDurationTimer);
    IMSA_CallSetDtmfState(IMSA_CALL_DTMF_WAIT_TIME_OUT);

    return;
}

/*****************************************************************************
 Function Name  : IMSA_CallRcvImsStopDtmfOk
 Description    : stop dtmf�ɹ���Ϣ����
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : ������

 History        :
      1.lihong 0015001     2013-10-14  Draft Enact
*****************************************************************************/
VOS_VOID  IMSA_CallRcvImsStopDtmfOk( VOS_VOID )
{
    IMSA_CALL_DTMF_INFO_STRU           *pstDtmfCurInfo  = VOS_NULL_PTR;
    IMSA_CALL_MANAGER_STRU             *pstCallCtx      = IMSA_CallCtxGet();
    IMSA_CALL_DTMF_STATE_ENUM_UINT8     enDtmfState     = IMSA_CALL_DTMF_STATE_BUTT;

    VOS_UINT32                          ulResult = VOS_FALSE;

    /* �ж�״̬�Ƿ��쳣 */
    enDtmfState = IMSA_CallGetDtmfState();
    if ( (enDtmfState != IMSA_CALL_DTMF_WAIT_STOP_CNF)
      && (enDtmfState != IMSA_CALL_DTMF_WAIT_AUTO_STOP_CNF) )
    {
        IMSA_WARN_LOG("IMSA_CallRcvImsStopDtmfOk: Dtmf State Error!");
        return;
    }

    /* ֹͣDTMF������ʱ�� */
    IMSA_StopTimer(&pstCallCtx->stDtmfCtx.stDtmfProtectTimer);

    /* ������û�������STOP_DTMF��Ҫ�ظ���ʽ��Ӧ�¼� */
    pstDtmfCurInfo  = IMSA_CallGetCurrentDtmfInfo();
    if (enDtmfState != IMSA_CALL_DTMF_WAIT_AUTO_STOP_CNF)
    {
        (VOS_VOID)IMSA_CallSendSpmStopDtmfRsltMsg(  pstDtmfCurInfo->usClientId,
                                                    pstDtmfCurInfo->ucSpmOpId,
                                                    pstDtmfCurInfo->ucCallId,
                                                    TAF_CS_CAUSE_SUCCESS);
    }

    IMSA_CallSetDtmfState(IMSA_CALL_DTMF_IDLE);

    /* �����棬�����׶�ʵ�� */
    if(IMSA_CallGetDtmfCnt() > 0)
    {
        /* �жϻ���ĵ�һ����STOP DTMF*/
        if(0 == IMSA_CallGetDtmfBufList(0).usOnLength)
        {
            (VOS_VOID)IMSA_CallSendSpmStopDtmfRsltMsg(IMSA_CallGetDtmfBufList(0).usClientId,
                                                      IMSA_CallGetDtmfBufList(0).ucSpmOpId,
                                                      IMSA_CallGetDtmfBufList(0).ucCallId,
                                                      TAF_CS_CAUSE_SUCCESS);

            /* ɾ������STOP DTMF���� */
            IMSA_CallDeleteSavedDtmfInfo(0);
        }

        /* �ж��Ƿ��л��� */
        if(0 == IMSA_CallGetDtmfCnt())
        {
            return;
        }

        /* ����ĵ�һ����STRAT DTMF */
        ulResult = IMSA_CallCheckUserDtmfCallId(0);
        if(TAF_CS_CAUSE_SUCCESS != ulResult)
        {
            IMSA_WARN_LOG("IMSA_CallRcvImsStopDtmfOk: Call Check User Dtmf Fail!");
            IMSA_CallClearDtmfInfo(TAF_CS_CAUSE_UNKNOWN);
            return;
        }

        if (VOS_TRUE != IMSA_CallSendImsMsgStartDtmf(IMSA_CallGetDtmfBufList(0).ucCallId,
                                                     IMSA_CallGetDtmfBufList(0).cKey,
                                                     IMSA_CallGetDtmfBufList(0).usOnLength))
        {
            IMSA_WARN_LOG("IMSA_CallRcvImsStopDtmfOk: Send Start Dtmf Fail!");

            IMSA_CallClearDtmfInfo(TAF_CS_CAUSE_UNKNOWN);
            return;
        }

        IMSA_StartTimer(&pstCallCtx->stDtmfCtx.stDtmfProtectTimer);
        IMSA_CallSetDtmfState(IMSA_CALL_DTMF_WAIT_START_CNF);
        IMSA_MEM_CPY(IMSA_CallGetCurrentDtmfInfo(),
                     &IMSA_CallGetDtmfBufList(0),
                     sizeof(IMSA_CALL_DTMF_INFO_STRU));
        IMSA_CallGetCurrentDtmfInfo()->ulImsOpid = IMSA_GetImsOpId();

        /* ɾ������START DTMF���� */
        IMSA_CallDeleteSavedDtmfInfo(0);
    }

    return;
}
VOS_UINT32 IMSA_CallProcImsMsgOk(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent)
{
    VOS_UINT32                          ulResult        = VOS_FALSE;
    IMSA_CALL_MANAGER_STRU             *pstCallCtx      = IMSA_CallCtxGet();
    IMSA_CALL_ENTITY_STRU              *pstCallEntity   = VOS_NULL_PTR;


    /* ���REASON OK��Ӧ��opid���ʧ�ܣ���������Ϣ */
    ulResult = IMSA_CallImsMsgFind(pstOutputCallEvent->ulOpId);
    if (IMSA_CALL_FIND_FAIL == ulResult)
    {
        IMSA_ERR_LOG1("IMSA_CallProcImsMsgOk: not found op id", pstOutputCallEvent->ulOpId);
        return VOS_TRUE;
    }

    if (IMSA_CALL_FIND_SUCC_DTMF == ulResult)
    {
        if (IMSA_CallGetDtmfState() == IMSA_CALL_DTMF_WAIT_START_CNF)
        {
            IMSA_CallRcvImsStartDtmfOk();
        }
        else
        {
            IMSA_CallRcvImsStopDtmfOk();
        }

        return VOS_TRUE;
    }

    /* ֹͣ������ʱ�� */
    IMSA_StopTimer(&pstCallCtx->stProctectTimer);

    /* ���ݲ�ͬ����ظ�SPM */
    switch (pstCallCtx->stSpmMsg.ulSpmMsgId)
    {
    case ID_SPM_IMSA_CALL_ORIG_REQ:

        /* ������ز�ʱ�ظ���OK���򲻱���SPM */
        if ((VOS_TRUE == IMSA_IsTimerRunning(&pstCallCtx->stRedialMaxTimer))
            && (pstCallCtx->stRedialMaxTimer.usPara < IMSA_CALL_MAX_NUM))
        {
            pstCallEntity = &pstCallCtx->astCallEntity[pstCallCtx->stRedialMaxTimer.usPara];
            if (pstCallEntity->ucRedialTimes > 0)
            {
                break;
            }
        }

        /* ��ΪSPM ��ORIG REQ���CALL IDΪ0����IMSA�ڷ���CALLʵ��Ҳ��CALL IDĬ��
           ��ʼ��Ϊ��ЧCALL ID����0�������IMSA_CallSendSpmEvtAndCallInfo������
           ���ܹ����ҵ�CALLʵ�壬�Ӷ��ܹ���ȷ��дCALL TYPE���ڻظ�ORIG CNFʱ��
           ĿǰSPMֻ��ҪIMSA��д��ȷ��CALL TYPE */
        ulResult = IMSA_CallSendSpmOrigCnfMsg(pstCallCtx->stSpmMsg.usClientId,
                                              pstCallCtx->stSpmMsg.ulOpId,
                                              pstCallCtx->stSpmMsg.ulCallId,
                                              TAF_CS_CAUSE_SUCCESS);
        break;
    case ID_SPM_IMSA_CALL_SUPS_CMD_REQ:
        ulResult = IMSA_CallSendSpmSupsResultMsg(pstCallCtx->stSpmMsg.usClientId,
                                              pstCallCtx->stSpmMsg.ulOpId,
                                              pstCallCtx->stSpmMsg.ulCallId,
                                              MN_CALL_SS_RES_SUCCESS);
        break;
    case ID_SPM_IMSA_CALL_GET_CALL_INFO_REQ:
        IMSA_INFO_LOG("IMSA_CallProcImsMsgOk: this command has handled by IMSA");
        break;
    case ID_SPM_IMSA_CALL_GET_CUDR_REQ:
        IMSA_INFO_LOG("IMSA_CallProcImsMsgOk: this command is not supported by IMSA");
        break;
    /* xiongxianghui00253310 add for conference 20140210 begin */
    case ID_SPM_IMSA_CALL_INVITE_NEW_PTPT_REQ:
        ulResult = IMSA_CallSendSpmInviteNewPtptCnfMsg(pstCallCtx->stSpmMsg.usClientId,
                                                       pstCallCtx->stSpmMsg.ulOpId,
                                                       pstCallCtx->stSpmMsg.ulCallId,
                                                       TAF_CS_CAUSE_SUCCESS);
        break;
    /* xiongxianghui00253310 add for conference 20140210 end */
    default:
        ulResult = VOS_FALSE;
        IMSA_ERR_LOG1("IMSA_CallProcImsMsgOk: invalid call request", pstCallCtx->stSpmMsg.ulSpmMsgId);
    }

    /* ����������� */
    IMSA_CallImsMsgClear();
    IMSA_CallSpmMsgClear();

    return ulResult;
}
VOS_VOID IMSA_CallEntitySaveErrorInfo
(
    IMSA_CALL_ENTITY_STRU                  *pstCallEntity,
    const IMSA_IMS_OUTPUT_CALL_EVENT_STRU  *pstOutputCallEvent
)
{
    pstCallEntity->bitOpErrorInfo = IMSA_OP_TRUE;

    IMSA_MEM_CPY(   &pstCallEntity->stImsError,
                    &pstOutputCallEvent->stCallError,
                    sizeof(IMSA_IMS_CALL_ERROR_STRU));

    IMSA_INFO_LOG1("IMSA_CallEntitySaveErrorInfo:errorCode:", pstCallEntity->stImsError.stErrorCode.usSipStatusCode);
}

/*****************************************************************************
 Function Name  : IMSA_CallRcvImsStartDtmfError
 Description    : start dmtfʧ�ܴ���
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : ������

 History        :
      1.lihong 0015001     2013-10-14  Draft Enact
*****************************************************************************/
VOS_VOID  IMSA_CallRcvImsStartDtmfError( VOS_VOID )
{
    IMSA_CALL_DTMF_INFO_STRU           *pstDtmfCurInfo  = VOS_NULL_PTR;
    IMSA_CALL_MANAGER_STRU             *pstCallCtx      = IMSA_CallCtxGet();

    VOS_UINT32                          ulResult = VOS_FALSE;

    /* �ж�״̬�Ƿ��쳣 */
    if (IMSA_CallGetDtmfState() != IMSA_CALL_DTMF_WAIT_START_CNF)
    {
        IMSA_WARN_LOG("IMSA_CallRcvImsStartDtmfError: Dtmf State Error!");
        return;
    }

    /* ֹͣDTMF������ʱ�� */
    IMSA_StopTimer(&pstCallCtx->stDtmfCtx.stDtmfProtectTimer);

    /* �ظ�DTMF��ʽ��Ӧ�¼� */
    pstDtmfCurInfo  = IMSA_CallGetCurrentDtmfInfo();
    (VOS_VOID)IMSA_CallSendSpmStartDtmfRsltMsg( pstDtmfCurInfo->usClientId,
                                                pstDtmfCurInfo->ucSpmOpId,
                                                pstDtmfCurInfo->ucCallId,
                                                TAF_CS_CAUSE_DTMF_REJ);

    IMSA_CallSetDtmfState(IMSA_CALL_DTMF_IDLE);

    /* ���洦��*/
    if(IMSA_CallGetDtmfCnt() > 0)
    {
        /* �жϻ���ĵ�һ����STOP DTMF*/
        if(0 == IMSA_CallGetDtmfBufList(0).usOnLength)
        {
            (VOS_VOID)IMSA_CallSendSpmStopDtmfCnfMsg( IMSA_CallGetDtmfBufList(0).usClientId,
                                                      IMSA_CallGetDtmfBufList(0).ucSpmOpId,
                                                      IMSA_CallGetDtmfBufList(0).ucCallId,
                                                      TAF_CS_CAUSE_DTMF_REJ);

            /* ɾ������STOP DTMF���� */
            IMSA_CallDeleteSavedDtmfInfo(0);
        }

        /* �ж��Ƿ��л��� */
        if(0 == IMSA_CallGetDtmfCnt())
        {
            return;
        }

        /* ����ĵ�һ����START DTMF */
        ulResult = IMSA_CallCheckUserDtmfCallId(0);
        if(TAF_CS_CAUSE_SUCCESS != ulResult)
        {
            IMSA_WARN_LOG("IMSA_CallRcvImsStartDtmfError: Call Check User Dtmf Fail!");
            IMSA_CallClearDtmfInfo(TAF_CS_CAUSE_UNKNOWN);
            return;
        }

        if (VOS_TRUE != IMSA_CallSendImsMsgStartDtmf(IMSA_CallGetDtmfBufList(0).ucCallId,
                                                     IMSA_CallGetDtmfBufList(0).cKey,
                                                     IMSA_CallGetDtmfBufList(0).usOnLength))
        {
            IMSA_WARN_LOG("IMSA_CallRcvImsStartDtmfError: Send Start Dtmf Fail!");

            IMSA_CallClearDtmfInfo(TAF_CS_CAUSE_UNKNOWN);
            return;
        }

        IMSA_StartTimer(&pstCallCtx->stDtmfCtx.stDtmfProtectTimer);
        IMSA_CallSetDtmfState(IMSA_CALL_DTMF_WAIT_START_CNF);
        IMSA_MEM_CPY(IMSA_CallGetCurrentDtmfInfo(),
                     &IMSA_CallGetDtmfBufList(0),
                     sizeof(IMSA_CALL_DTMF_INFO_STRU));
        IMSA_CallGetCurrentDtmfInfo()->ulImsOpid = IMSA_GetImsOpId();

        /* ɾ������START DTMF���� */
        IMSA_CallDeleteSavedDtmfInfo(0);
    }

    return;
}

/* lihong00150010 2014-04-21 ��ȫ���Ӷ� begin */

VOS_UINT32 IMSA_CallProcImsMsgErrorWhenFindOpidSucc
(
    const IMSA_IMS_OUTPUT_CALL_EVENT_STRU  *pstOutputCallEvent,
    VOS_UINT32                              ulFindOpidResult
)
{
    IMSA_CALL_MANAGER_STRU             *pstCallCtx      = IMSA_CallCtxGet();
    IMSA_CALL_ENTITY_STRU              *pstCallEntity   = VOS_NULL_PTR;
    VOS_UINT32                          ulResult        = VOS_FALSE;
    /* xiongxianghui00253310 add for conference 20140210 begin */
    TAF_CS_CAUSE_ENUM_UINT32            enSpmErr        = IMSA_NULL;
    /* xiongxianghui00253310 add for conference 20140210 end */
    VOS_UINT16                          usRedialCallIndex = IMSA_NULL;

    if (IMSA_CALL_FIND_SUCC_DTMF == ulFindOpidResult)
    {
        if (IMSA_CallGetDtmfState() == IMSA_CALL_DTMF_WAIT_START_CNF)
        {
            IMSA_CallRcvImsStartDtmfError();
        }
        else
        {
            /* ��IMSЭ��ջ�ӿ�Լ����STOP DTMFû��ʧ�� */
            IMSA_INFO_LOG("IMSA_CallProcImsMsgErrorWhenFindOpidSucc: stop dtmf can not error!");
        }

        return VOS_TRUE;
    }

    /* ֹͣ������ʱ�� */
    IMSA_StopTimer(&pstCallCtx->stProctectTimer);

    /* ���ݲ�ͬ����ظ�SPM */
    switch (pstCallCtx->stSpmMsg.ulSpmMsgId)
    {
    case ID_SPM_IMSA_CALL_ORIG_REQ:

        /* ������ز�ʱ�ظ���ERROR�������ز����� */
        if (VOS_TRUE == IMSA_IsTimerRunning(&pstCallCtx->stRedialMaxTimer))
        {
            usRedialCallIndex   = pstCallCtx->stRedialMaxTimer.usPara;
            if (usRedialCallIndex < IMSA_CALL_MAX_NUM)
            {
                pstCallEntity = &pstCallCtx->astCallEntity[usRedialCallIndex];
                if (pstCallEntity->ucRedialTimes > 0)
                {
                    IMSA_INFO_LOG("IMSA_CallProcImsMsgError:redial orig error");
                    IMSA_CallEntitySaveErrorInfo(pstCallEntity, pstOutputCallEvent);
                    IMSA_CallInterruptRedial(pstCallEntity);
                    break;
                }
            }
        }

        ulResult = IMSA_CallSendSpmOrigCnfMsg(pstCallCtx->stSpmMsg.usClientId,
                                              pstCallCtx->stSpmMsg.ulOpId,
                                              pstCallCtx->stSpmMsg.ulCallId,
                                              TAF_CS_CAUSE_IMSA_ERROR);

        pstCallEntity = IMSA_CallEntityGetUsedByCallId(IMSA_CALL_INVALID_ID);
        if (VOS_NULL_PTR != pstCallEntity)
        {
            IMSA_CallEntityFree(pstCallEntity);
        }

        break;

    case ID_SPM_IMSA_CALL_SUPS_CMD_REQ:
        ulResult = IMSA_CallSendSpmSupsResultMsg(pstCallCtx->stSpmMsg.usClientId,
                                              pstCallCtx->stSpmMsg.ulOpId,
                                              pstCallCtx->stSpmMsg.ulCallId,
                                              MN_CALL_SS_RES_FAIL);
        break;
    /* xiongxianghui00253310 add for conference 20140210 begin */
    case ID_SPM_IMSA_CALL_INVITE_NEW_PTPT_REQ:
        enSpmErr = IMSA_CallTransImsErr2SpmErr(pstOutputCallEvent->stCallError.stErrorCode.usSipStatusCode);
        ulResult = IMSA_CallSendSpmInviteNewPtptCnfMsg(pstCallCtx->stSpmMsg.usClientId,
                                                       pstCallCtx->stSpmMsg.ulOpId,
                                                       pstCallCtx->stSpmMsg.ulCallId,
                                                       enSpmErr);

        pstCallEntity = IMSA_CallEntityGetUsedByCallId(IMSA_CALL_INVALID_ID);
        if (VOS_NULL_PTR != pstCallEntity)
        {
            IMSA_CallEntityFree(pstCallEntity);
        }
        break;
    /* xiongxianghui00253310 add for conference 20140210 end */
    default:
        ulResult = VOS_FALSE;
        IMSA_ERR_LOG1("IMSA_CallProcImsMsgError: invalid call request", pstCallCtx->stSpmMsg.ulSpmMsgId);
    }

    /* ����������� */
    IMSA_CallImsMsgClear();
    IMSA_CallSpmMsgClear();

    return ulResult;
}
VOS_UINT32 IMSA_CallProcImsMsgError(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent)
{
    IMSA_CALL_ENTITY_STRU              *pstCallEntity   = VOS_NULL_PTR;
    VOS_UINT32                          ulResult        = VOS_FALSE;
    VOS_UINT32                          ulCallId        = 0;

    /* ���REASON ERROR��Ӧ��opid���ʧ�ܣ���˵����IMS�ظ�OK֮������ܾ�UE�ĳ�����
       ���OPID���ҳɹ�������û�з���տ����̣�IMSֱ�ӻظ���ERROR */
    ulResult = IMSA_CallImsMsgFind(pstOutputCallEvent->ulOpId);
    if (IMSA_CALL_FIND_FAIL == ulResult)
    {
        ulCallId = (VOS_UINT32)pstOutputCallEvent->stCallError.ulCallIndex;
        /**
         * TODO:
         * 1. ����Ǻ���ʧ�ܵĳ�������ҪD2��REASON ERROR��ҲЯ������Id
         * 2. ֮�����Ҳ���csmid������Ϊ��֮ǰ�յ���REASON OK���Ѿ��������csmid���
         * 3. ���ｫErrorInfo���浽ÿ������ʵ�忼�ǵ��ж������ͬʱ��������ʱ������ȷ����
         */
        pstCallEntity = IMSA_CallEntityGetUsedByCallId(ulCallId);
        if (VOS_NULL_PTR != pstCallEntity)
        {
            IMSA_CallEntitySaveErrorInfo(pstCallEntity, pstOutputCallEvent);
        }
        else
        {
            IMSA_ERR_LOG1("IMSA_CallProcImsMsgError: invalid err call id", ulCallId);
        }

        return VOS_TRUE;
    }

    return IMSA_CallProcImsMsgErrorWhenFindOpidSucc(pstOutputCallEvent, ulResult);
}
/* lihong00150010 2014-04-21 ��ȫ���Ӷ� end */

VOS_UINT32 IMSA_CallProcImsMsgIncoming(IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent)
{
    IMSA_NORM_LOG("IMSA_CallProcImsMsgIncoming: enterd");
#if 0
    /* ����Incoming��ϢЯ����Remote��ַ */
    IMSA_CallSaveIncomingAddr(pstOutputCallEvent->address);
#else
    /**
     * 1. MTʱ��Incoming�¼������ǵ�һ���¼�(��Precondition)��Ҳ�������м��¼�(��Precondition)
     * 2. Incoming�¼���ֻ������remote address��������Ϣ��������������¼�/״̬֪ͨ��Ҳ����
     * 3. Incoming�¼�����Call Id����ʹ����һЩ�쳣(��Call Entity����ʧ��)��IMSAҲ�޷�ȥ�ܾ��ú���
     *
     * ����ԭ��Incoming�¼������κδ���ͬ��Waiting�¼�Ҳ�����κδ���
     */
#endif

    (VOS_VOID)pstOutputCallEvent;

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallEntityProcErrorInfo380ForNormal(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_ERROR_STRU *pstImsError)
{
    (VOS_VOID)pstCallEntity;
    (VOS_VOID)pstImsError;

    /**
     * �����ͨ�����յ�380��Ӧ�����alternative-service��type������emergency���������ע��
     *
     * �ⷨһ����IMSA�ڲ��ܻ��沢ʵ�ֵڶ��κ��еķ���
     * ������������MO����ʱ��ȥ���𣬶��ں���ʵ��Ҳֻ��MOʱ�ű���SPM�·��ĺ��з�������
     * ���������ٽ��ò������浽���й��������ĵĻ��������������յ�����������õ�֪ͨʱ��
     * ������һ�η���ú���(����������Ҫ���ĺ�������??)
     *
     * �ⷨ��: IMSA���ض��ĺ��н�����ظ�SPM(��TAF_CS_CAUSE_IMSA_RETRY_VIA_IMS_EMC)����SPM�������ڶ��κ���
     * �����ƺ�������ɾ�һЩ������Ҫ��GU�����ӿڣ�
     *     - ���ֽⷨ������һ�����Ǿ��ǣ������ֳ��������ز������ز��Ƿ���SPMʵ��
     *     - SPM�Ѿ���ͨ��CS���ز��Ļ��ƣ��ټ�һ��ͨ��IMS��������еķ�ʽ�ز����޸�Ӧ��ֻ�Ǹ������еĻ���
     */

    /* Ŀǰ�Ȱ���������� */

    return VOS_TRUE;
}
VOS_UINT32 IMSA_CallEntityProcErrorInfo380ForEmc
(
    const IMSA_CALL_ENTITY_STRU        *pstCallEntity,
    const IMSA_IMS_CALL_ERROR_STRU     *pstImsError
)
{
    IMSA_CALL_MANAGER_STRU             *pstCallCtx = IMSA_CallCtxGet();

    switch (pstCallEntity->enEmcType)
    {
        case IMSA_EMC_CALL_TYPE_IN_NORMAL_SRV:

            /* ����Action��ֵ������ */
            if (IMSA_IMS_3GPP_TYPE_ACTION_EMERGENCY_REG == pstImsError->stErrorCode.usIms3gppAction)
            {
                /* ����CALL MANAGER�еĻ���SPM��Ϣ���յ�OKʱ������ˣ�����������Ҫ
                   ��CALLʵ���л����SPM��Ϣ������CALL MANAGER�� */
                IMSA_MEM_CPY(   &pstCallCtx->stSpmMsg,
                                &pstCallEntity->stSpmMsg,
                                sizeof(IMSA_CALL_SPM_MSG_STRU));

                /* ֪ͨ����ģ����н���ע�� */
                (VOS_VOID)IMSA_CallSendIntraResultAction(IMSA_CALL_TYPE_EMC, IMSA_RESULT_ACTION_REG_WITH_FIRST_ADDR_PAIR);

                /* ����RELEASED�¼��������CALLʵ�壬��IMS����ע��ɹ����ٳ��Խ����� */
                return VOS_FALSE;
            }

            break;
        case IMSA_EMC_CALL_TYPE_EMC_CONN_EMC_REG:

            if ((VOS_FALSE == pstCallEntity->bitOpRetryCsCall)
                && (IMSA_IMS_3GPP_TYPE_ACTION_EMERGENCY_REG == pstImsError->stErrorCode.usIms3gppAction))
            {
                /* ����CALL MANAGER�еĻ���SPM��Ϣ���յ�OKʱ������ˣ�����������Ҫ
                   ��CALLʵ���л����SPM��Ϣ������CALL MANAGER�� */
                IMSA_MEM_CPY(   &pstCallCtx->stSpmMsg,
                                &pstCallEntity->stSpmMsg,
                                sizeof(IMSA_CALL_SPM_MSG_STRU));

                /* ֪ͨ����ģ�鳢��ʹ����һ���������ע�� */
                (VOS_VOID)IMSA_CallSendIntraResultAction(pstCallEntity->enType, IMSA_RESULT_ACTION_REG_WITH_NEXT_ADDR_PAIR);

                /* ����RELEASED�¼��������CALLʵ�壬��������������IMS����ע��ɹ����ٳ��Խ����� */
                return VOS_FALSE;
            }
            break;

        default:

            IMSA_ERR_LOG1("IMSA_CallEntityProcErrorInfo380ForEmc: invalid emergency type", pstCallEntity->enEmcType);
            break;
    }

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallEntityProcErrorInfo380(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_ERROR_STRU *pstImsError)
{
    VOS_UINT32 ulResult = VOS_TRUE;

    switch (pstCallEntity->enType)
    {
    case IMSA_CALL_TYPE_NORMAL:
        ulResult = IMSA_CallEntityProcErrorInfo380ForNormal(pstCallEntity, pstImsError);
        break;
    case IMSA_CALL_TYPE_EMC:
        ulResult = IMSA_CallEntityProcErrorInfo380ForEmc(pstCallEntity, pstImsError);
        break;
    default:
        ulResult = VOS_TRUE;
        IMSA_ERR_LOG1("IMSA_CallEntityProcErrorInfo380: invalid call type", pstCallEntity->enType);
    }

    return ulResult;
}


VOS_UINT32 IMSA_CallEntityProcErrorInfo503(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_ERROR_STRU *pstImsError)
{
    (VOS_VOID)pstCallEntity;
    (VOS_VOID)pstImsError;

    /**
     * ���503��Ӧ�а���Retry-After��Ӧ��֪ͨSPM������ͨ����֪ͨ����Ϊ��������ȥ��
     * ������Ҫ��GU���۽ӿ�
     *
     * ��ʱ�Ȳ�ʵ�ִ˹���
     */

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallEntityProcErrorInfo504(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_ERROR_STRU *pstImsError)
{
    (VOS_VOID)pstCallEntity;

    if (IMSA_IMS_3GPP_TYPE_ACTION_RESTORATION_INIT_REG == pstImsError->stErrorCode.usIms3gppAction)
    {
        /* ֪ͨService����Restoration���� */
        (VOS_VOID)IMSA_CallSendIntraResultAction(pstCallEntity->enType, IMSA_RESULT_ACTION_REG_RESTORATION);
    }
    else
    {
        IMSA_ERR_LOG("IMSA_CallEntityProcErrorInfo504: not defined procedure");
    }

    return VOS_TRUE;
}
VOS_UINT32 IMSA_CallEntityProcErrorInfo(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_ERROR_STRU *pstImsError)
{
    VOS_UINT32 ulResult = VOS_TRUE;

    IMSA_INFO_LOG1("IMSA_CallProcErrorInfo: process error code", pstImsError->stErrorCode.usSipStatusCode);

    switch (pstImsError->stErrorCode.usSipStatusCode)
    {
    /* 380 */
    case IMSA_SIP_NW_ERROR_CAUSE_ALTERNATIVE_SERVICE:
        ulResult = IMSA_CallEntityProcErrorInfo380(pstCallEntity, pstImsError);
        break;
    /* 503 */
    case IMSA_SIP_NW_ERROR_CAUSE_SERVICE_UNAVAILABLE:
        ulResult = IMSA_CallEntityProcErrorInfo503(pstCallEntity, pstImsError);
        break;
    /* 504 */
    case IMSA_SIP_NW_ERROR_CAUSE_SERVER_TIMEOUT:
        ulResult = IMSA_CallEntityProcErrorInfo504(pstCallEntity, pstImsError);
        break;
    default:
        break;
    }

    return ulResult;
}
TAF_CS_CAUSE_ENUM_UINT32 IMSA_CallTransImsErr2SpmErr
(
    VOS_UINT16                          usImsErr
)
{
    TAF_CS_CAUSE_ENUM_UINT32            enSpmErr = TAF_CS_CAUSE_BUTT;

    IMSA_INFO_LOG1("IMSA_CallTransImsErr2SpmErr: ims error code:", usImsErr);

    if ((usImsErr < 300) || (usImsErr > 699))
    {
        enSpmErr = TAF_CS_CAUSE_IMSA_ERROR;

        return enSpmErr;
    }

    enSpmErr = usImsErr + TAF_CS_CAUSE_IMS_BEGIN;

    if (enSpmErr >= TAF_CS_CAUSE_IMSA_BEGIN)
    {
        IMSA_ERR_LOG("IMSA_CallTransImsErr2SpmErr:ims err is not illegal");
        enSpmErr = TAF_CS_CAUSE_IMS_FORBIDDEN;
    }

    return enSpmErr;
}



VOS_VOID IMSA_CallReleaseCallCommonProc
(
    IMSA_CALL_ENTITY_STRU              *pstCallEntity,
    TAF_CS_CAUSE_ENUM_UINT32            ulResult
)
{
    IMSA_CALL_MANAGER_STRU             *pstCallCtx  = IMSA_CallCtxGet();
    VOS_UINT32                          i           = IMSA_NULL;

    /* �����CALL��δ�ϱ���INCOMING�¼���SPM��MT CALL����IsUsedΪTRUE��
       ��״̬ΪIDLE������ΪMT��������Ҫ�ϱ�releasd�¼� */
    if ((IMSA_CALL_STATUS_IDLE == pstCallEntity->enStatus)
        && (MN_CALL_DIR_MT == pstCallEntity->enDir))
    {
        IMSA_INFO_LOG("IMSA_CallReleaseCallCommonProc:MT,IDLE");
        IMSA_CallEntityFree(pstCallEntity);

        return ;
    }

    /*���SRVCC���̣��õ绰��ת��CS�����ϱ���SPM������Ҫ�ͷ�call��Ϣ*/
    if(pstCallEntity->bitOpTransToCs == VOS_TRUE)
    {
        IMSA_INFO_LOG("IMSA_CallReleaseCallCommonProc:Call transfered to CS.");
        IMSA_CallEntityFree(pstCallEntity);

        return;
    }

    (VOS_VOID)IMSA_CallSendSpmReleasedEvt(  pstCallEntity,
                                            ulResult);

    IMSA_CallEntityFree(pstCallEntity);

    /* ���������CALL����IsUsed��ΪFALSE��������ֻ����δ�ϱ�INCOMING�¼���SPM��
       MT CALL����IsUsedΪTRUE����״̬ΪIDLE������ΪMT�������ϱ�
       MN_CALL_EVT_ALL_RELEASED�¼� */
    for (i = 0; i < IMSA_CALL_MAX_NUM; i++)
    {
        if ((VOS_FALSE == pstCallCtx->astCallEntity[i].bitOpIsUsed)
          ||((IMSA_CALL_STATUS_IDLE == pstCallCtx->astCallEntity[i].enStatus)
                && (MN_CALL_DIR_MT == pstCallCtx->astCallEntity[i].enDir)))
        {
            continue;
        }

        break;
    }

    if (IMSA_CALL_MAX_NUM == i)
    {
        (VOS_VOID)IMSA_CallSendSpmAllReleasedEvt();
    }
}

/*****************************************************************************
 Function Name  : IMSA_CallClearLocalAlertInfo
 Description    : ����HIFI���������������Ϣ
 Input          : VOS_VOID
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-10-11  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_CallClearLocalAlertInfo(VOS_VOID )
{
    VOS_UINT32 i = 0;
    IMSA_CALL_MANAGER_STRU *pstCallCtx = IMSA_CallCtxGet();

    for (i = 0; i < IMSA_CALL_MAX_NUM; i++)
    {
        if ((VOS_TRUE == pstCallCtx->astCallEntity[i].bitOpIsUsed) &&
            (VOS_FALSE== pstCallCtx->astCallEntity[i].bitOpEarlyMedia))
        {
            IMSA_INFO_LOG("IMSA_CallClearLocalAlertInfo: Clear Local Alert Info!");

            pstCallCtx->astCallEntity[i].bitOpEarlyMedia = VOS_TRUE;
        }
    }

}


VOS_UINT32 IMSA_CallIsNeedCsRedialSrvccSucc
(
    const IMSA_CALL_ENTITY_STRU        *pstCallEntity
)
{
    if (IMSA_TRUE != IMSA_CallGetNotReportAllReleasedFlag())
    {
        return IMSA_FALSE;
    }

    if (MN_CALL_DIR_MO != pstCallEntity->enDir)
    {
        return IMSA_FALSE;
    }

    if ((IMSA_CALL_STATUS_DIALING != pstCallEntity->enStatus)
        && (IMSA_CALL_STATUS_TRYING != pstCallEntity->enStatus))
    {
        return IMSA_FALSE;
    }

    return IMSA_TRUE;
}


VOS_UINT32 IMSA_CallFindCauseInRedialCauseList
(
    VOS_UINT16                          usSipStatusCode
)
{
    IMSA_IMS_REDIAL_CFG_STRU           *pstImsRedialCfg = VOS_NULL_PTR;
    VOS_UINT32                          i               = IMSA_NULL;

    pstImsRedialCfg = IMSA_GetImsRedialCfgAddress();

    for (i = 0; i < pstImsRedialCfg->ucRedialCauseNum; i++)
    {
        if (usSipStatusCode == pstImsRedialCfg->ausRedialCauseList[i])
        {
            return IMSA_SUCC;
        }
    }

    return IMSA_FAIL;
}


VOS_UINT32 IMSA_CallIsNeedRedial
(
    const IMSA_CALL_ENTITY_STRU        *pstCallEntity,
    VOS_UINT16                          usSipStatusCode
)
{
    IMSA_IMS_REDIAL_CFG_STRU           *pstImsRedialCfg         = VOS_NULL_PTR;
    VOS_UINT32                          ulRedialMaxTimeRemainLen= IMSA_NULL;
    IMSA_CALL_MANAGER_STRU             *pstCallCtx              = IMSA_CallCtxGet();

    /* IMS�ز�����δ�򿪣����ز� */
    pstImsRedialCfg = IMSA_GetImsRedialCfgAddress();
    if (IMSA_FALSE == pstImsRedialCfg->ucIsCallRedialSupportFlg)
    {
        IMSA_INFO_LOG("IMSA_CallIsNeedRedial: ims redial not support");
        return IMSA_FALSE;
    }

    /* ��������ͷţ����ز� */
    if (VOS_TRUE == pstCallEntity->stDiscDir.ucDiscCallFlag)
    {
        IMSA_INFO_LOG("IMSA_CallIsNeedRedial: releasing");
        return IMSA_FALSE;
    }

    /* �������MO�����ز� */
    if (MN_CALL_DIR_MO != pstCallEntity->enDir)
    {
        IMSA_INFO_LOG("IMSA_CallIsNeedRedial: not mo");
        return IMSA_FALSE;
    }

    /* ״̬��ΪDIALING��TRYING�����ز� */
    if ((IMSA_CALL_STATUS_DIALING != pstCallEntity->enStatus)
        && (IMSA_CALL_STATUS_TRYING != pstCallEntity->enStatus))
    {
        IMSA_INFO_LOG("IMSA_CallIsNeedRedial: call state is not dailing or trying");
        return IMSA_FALSE;
    }

    /* �����ز�����ԭ��ֵ�б��У����ز� */
    if (IMSA_FAIL == IMSA_CallFindCauseInRedialCauseList(usSipStatusCode))
    {
        IMSA_INFO_LOG("IMSA_CallIsNeedRedial: not in redial cause list");
        return IMSA_FALSE;
    }

    /* �ز������Ѵ������������ز� */
    if (pstCallEntity->ucRedialTimes >= pstImsRedialCfg->ulCallRedialMaxNum)
    {
        IMSA_INFO_LOG("IMSA_CallIsNeedRedial: redial times is already max");
        return IMSA_FALSE;
    }

    /* ��ȡ��ǰ��ʱ����ʣ��ʱ�� */
    IMSA_GetRemainTimeLen(&pstCallCtx->stRedialMaxTimer, &ulRedialMaxTimeRemainLen);

    IMSA_INFO_LOG1("Get redial max timer remain time tick:", PS_GET_TICK());

    IMSA_INFO_LOG1("redial max timer length:", pstCallCtx->stRedialMaxTimer.ulTimerLen);

    IMSA_INFO_LOG1("remain time:", ulRedialMaxTimeRemainLen);

    /* ���ʣ���ز�ʱ��С���ز���������ز� */

    if (ulRedialMaxTimeRemainLen < pstImsRedialCfg->ulCallRedialInterval)
    {
        IMSA_INFO_LOG("IMSA_CallIsNeedRedial: remain time < intervel time");
        return IMSA_FALSE;
    }

    return IMSA_TRUE;
}


VOS_UINT32 IMSA_CallProcImsMsgDisconnect
(
    const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent
)
{
    IMSA_CALL_ENTITY_STRU              *pstCallEntity   = VOS_NULL_PTR;
    VOS_UINT32                          ulResult        = VOS_TRUE;
    TAF_CS_CAUSE_ENUM_UINT32            enSpmErr        = IMSA_NULL;
    IMSA_CALL_MANAGER_STRU             *pstCallCtx      = IMSA_CallCtxGet();
    IMSA_IMS_REDIAL_CFG_STRU           *pstImsRedialCfg = VOS_NULL_PTR;

    pstCallEntity = IMSA_CallEntityGetUsedByCallId((VOS_UINT32)pstOutputCallEvent->stCallReport.astCalls[0].ulCallIndex);
    if (VOS_NULL_PTR == pstCallEntity)
    {
        IMSA_ERR_LOG1("IMSA_CallProcImsMsgDisconnect: not found call entity ",
            pstOutputCallEvent->stCallReport.astCalls[0].ulCallIndex);

        return VOS_TRUE;
    }

    if (IMSA_FALSE != pstCallEntity->bitOpErrorInfo)
    {
        ulResult = IMSA_CallEntityProcErrorInfo(    pstCallEntity,
                                                    &pstCallEntity->stImsError);

        /* ���ݷ���ֵ�������Ƿ���SPM��released�¼� */
        if (VOS_TRUE != ulResult)
        {
            IMSA_INFO_LOG("IMSA_CallProcImsMsgDisconnect:Not report released event to spm!");
            return VOS_TRUE;
        }

        /* �ж��Ƿ���Ҫ�ز� */
        if (IMSA_TRUE == IMSA_CallIsNeedRedial( pstCallEntity,
                                                pstCallEntity->stImsError.stErrorCode.usSipStatusCode))
        {
            IMSA_INFO_LOG("IMSA_CallProcImsMsgDisconnect:redial!");

            pstImsRedialCfg = IMSA_GetImsRedialCfgAddress();
            pstCallCtx->stRedialIntervelTimer.ulTimerLen = pstImsRedialCfg->ulCallRedialInterval;

            /* ������CALL ID���÷ŵ�para�У���8λ����������8λ��CALL ID��
               ��¼CALL ID��Ϣ��Ϊ�˷�ֹ�ڼ����ʱ������ʱ���û����ͷ����CALL�Ķ��� */
            pstCallCtx->stRedialIntervelTimer.usPara = pstCallEntity->ucId << IMSA_MOVEMENT_8;
            pstCallCtx->stRedialIntervelTimer.usPara |= pstCallEntity->ucCallEntityIndex;
            IMSA_StartTimer(&pstCallCtx->stRedialIntervelTimer);

            /* ���CALL ID�������ز�ʱIMS�ϱ�DAILING״̬����CALLʵ��ʱ����Ҳ���;
               �����ز�����IMS���ɼ������IMS���ܽ��ز���CALL ID���·����INCOMING
               CALL��������ﲻ��CALL ID����INCOMING CALL��ֱ�����ҵ�CALLʵ�壬
               ������Ϊpreconditon������
               �����������CALL ID������Ҫ�ڴ���ز�����ʱ������IMSA_CallInterruptRedial
               ���������½������ʱ���м�¼��CALLIDд��CALLʵ���У����ܱ�֤�ϱ�
               RELEASD�¼�ʱ��д��CALL ID��ȷ */
            pstCallEntity->ucId = IMSA_CALL_INVALID_ID;

            return VOS_TRUE;
        }

        /* ��IMS����ԭ��ֵת��ΪSPM����ԭ��ֵ */
        enSpmErr = IMSA_CallTransImsErr2SpmErr(pstCallEntity->stImsError.stErrorCode.usSipStatusCode);

        IMSA_CallReleaseCallCommonProc(pstCallEntity, enSpmErr);

        if (IMSA_CallEntityGetUsedCount() == 0)
        {
            IMSA_SrvccSuccBuffProc();
        }
    }
    else
    {
        /* �����SRVCC�ɹ�������MO CALL���ҵ�ǰת��ΪDIALING��TRYING��
           ��ԭ��ֵ��ΪSRVCC_SUCC����SPM�����Ƿ�CS���ز� */
        if (IMSA_TRUE == IMSA_CallIsNeedCsRedialSrvccSucc(pstCallEntity))
        {
            enSpmErr = TAF_CS_CAUSE_IMSA_SRVCC_SUCC;
        }
        else
        {
            enSpmErr = TAF_CS_CAUSE_SUCCESS;
        }

        IMSA_CallReleaseCallCommonProc(pstCallEntity, enSpmErr);

        if (IMSA_CallEntityGetUsedCount() == 0)
        {
            IMSA_SrvccSuccBuffProc();
        }
    }

    /* ���DTMF��Ϣ */
    if(TAF_CS_CAUSE_SUCCESS != IMSA_CallCheckUserDtmfCallId(0))
    {
        IMSA_CallClearDtmfInfo(TAF_CS_CAUSE_CALL_RELEASE);
    }

    return VOS_TRUE;
}
VOS_UINT32 IMSA_CallProcImsMsgWaiting(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent)
{
#if 0
    /* ����Waiting��ϢЯ����Remote��ַ */
    IMSA_CallSaveWaitingAddr(pstOutputCallEvent->address);
#else
    /**
     * ͬIncoming�¼��Ĵ���#IMSA_CallProcImsMsgIncomings
     */
#endif

    (VOS_VOID)pstOutputCallEvent;

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallProcImsMsgCallEarlyMedia(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent)
{
    IMSA_CALL_ENTITY_STRU *pstCallEntity = VOS_NULL_PTR;


    pstCallEntity = IMSA_CallEntityGetUsedByCallId((VOS_UINT32)pstOutputCallEvent->stCallReport.astCalls[0].ulCallIndex);
    if (pstCallEntity)
    {
        pstCallEntity->bitOpEarlyMedia = IMSA_OP_TRUE;
    }
    else
    {
        IMSA_ERR_LOG1("IMSA_CallProcImsMsgCallEarlyMedia: not found call entity ",
            pstOutputCallEvent->stCallReport.astCalls[0].ulCallIndex);
    }

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallProcImsMsgInitializing(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent)
{
    IMSA_CALL_MANAGER_STRU             *pstCallCtx = IMSA_CallCtxGet();
    IMSA_CALL_ENTITY_STRU              *pstCallEntity = VOS_NULL_PTR;
    VOS_UINT32                          ulResult = VOS_FALSE;
    IMSA_CALL_ENTITY_STRU              *pstRedailCallEntity = VOS_NULL_PTR;
    VOS_UINT8                           ucRedialCallIndex   = IMSA_NULL;


    pstCallEntity = IMSA_CallEntityGetUsedByCallId((VOS_UINT32)pstOutputCallEvent->stCallReport.astCalls[0].ulCallIndex);

    /* ����ҵ���Ӧ�ĺ���ʵ�壬����MO���е�INITIALIZING�¼� */
    if (pstCallEntity)
    {
        pstCallEntity->ulResRspOpId = pstOutputCallEvent->ulOpId;

        /* �ݲ�ʵ�ֳ����޸ĳ����µ�precondition���̣�ֻҪ��ý����ش��ھ���Ϊ
           ��Դ�Ѿ߱������ڽ��ý����ؽ�����ims���̿�ĳ��� */
        if (((IMSA_TRUE == pstCallCtx->ulIsMediaPdpReady) && (IMSA_CALL_TYPE_NORMAL == pstCallEntity->enType))
            || ((IMSA_TRUE == pstCallCtx->ulIsEmcMediaPdpReady) && (IMSA_CALL_TYPE_EMC == pstCallEntity->enType)))
        {
            IMSA_INFO_LOG("IMSA_CallProcImsMsgInitializing:call already exist,resource already!");

            (VOS_VOID)IMSA_CallSendImsMsgResRsp(pstCallEntity->ucId,
                                                pstCallEntity->ulResRspOpId,
                                                VOS_TRUE);
        }
        else
        {
            IMSA_INFO_LOG("IMSA_CallProcImsMsgInitializing:call already exist,wati for resource!");

            /* ���ú��ж�Ӧ����Դ��־λ */
            pstCallEntity->bitOpResReady = IMSA_FALSE;

            /* ˢ�µȴ���Դ������ʱ�� */
            IMSA_RegTimerRefresh(&pstCallCtx->stResReadyTimer, pstCallEntity->ucCallEntityIndex);
        }

        ulResult = VOS_TRUE;
    }
    /* ����Ҳ�����Ӧ�ĺ���ʵ�壬����MT���е�INITIALIZING�¼� */
    else
    {
        pstCallEntity = IMSA_CallEntityAlloc();
        if (pstCallEntity)
        {
            /* ��������ز����̣������ز����� */
            if (VOS_TRUE == IMSA_IsTimerRunning(&pstCallCtx->stRedialIntervelTimer))
            {
                ucRedialCallIndex   = (VOS_UINT8)pstCallCtx->stRedialIntervelTimer.usPara;
                if (ucRedialCallIndex < IMSA_CALL_MAX_NUM)
                {
                    pstRedailCallEntity = &pstCallCtx->astCallEntity[ucRedialCallIndex];
                    IMSA_CallInterruptRedial(pstRedailCallEntity);
                }
            }

            /* ���º���ʵ�� */
            IMSA_CallEntityUpdateByCallSummary(pstCallEntity, &(pstOutputCallEvent->stCallReport.astCalls[0]));

            pstCallEntity->ucId = (VOS_UINT8)pstOutputCallEvent->stCallReport.astCalls[0].ulCallIndex;
            pstCallEntity->ulResRspOpId = pstOutputCallEvent->ulOpId;

            /* ֻҪ��ý����ش��ھ���Ϊ��Դ�Ѿ߱������ڽ��ý����ؽ�����ims���̿�ĳ��� */
            if (IMSA_TRUE == pstCallCtx->ulIsMediaPdpReady)
            {
                IMSA_INFO_LOG("IMSA_CallProcImsMsgInitializing:call not exist,resource already!");

                (VOS_VOID)IMSA_CallSendImsMsgResRsp(pstCallEntity->ucId,
                                                    pstCallEntity->ulResRspOpId,
                                                    VOS_TRUE);
            }
            else
            {
                IMSA_INFO_LOG("IMSA_CallProcImsMsgInitializing:call not exist,wait for resource!");

                /* ���ú��ж�Ӧ����Դ��־λ */
                pstCallEntity->bitOpResReady = IMSA_FALSE;

                /* ˢ�µȴ���Դ������ʱ�� */
                IMSA_RegTimerRefresh(&pstCallCtx->stResReadyTimer, pstCallEntity->ucCallEntityIndex);
            }

            ulResult = VOS_TRUE;
        }
        else
        {
            /* ����������ʵ��ʧ�ܣ���ܾ��ú��� */
            (VOS_VOID)IMSA_CallImsCmdRel((VOS_UINT32)pstOutputCallEvent->stCallReport.astCalls[0].ulCallIndex);
        }
    }

    return ulResult;
}

/*****************************************************************************
 Function Name  : IMSA_CallImsSrvccState2TafCallState
 Description    : SRVCC����״̬��IMSA����״̬��ӳ��
 Input          : enCallState   ����״̬
 Output         :
 Return Value   : ӳ�����״̬�����û�ж�Ӧ���ԭ״̬����

 History        :
      1.sunbing 49683      2013-10-12  Draft Enact
*****************************************************************************/
CALL_IMSA_SRVCC_CALL_STATE_ENUM_UINT8 IMSA_CallImsSrvccState2TafCallState
(
    IMSA_IMS_CALL_STATE_ENUM_UINT8  enCallState
)
{
    CALL_IMSA_SRVCC_CALL_STATE_ENUM_UINT8  enMnCallState = 0;

    switch(enCallState)
    {
        case IMSA_IMS_CALL_STATE_ACTIVE:
            enMnCallState = CALL_IMSA_SRVCC_CALL_ACTIVE;
            break;

        case IMSA_IMS_CALL_STATE_HOLD:
            enMnCallState = CALL_IMSA_SRVCC_CALL_HELD;
            break;
        case IMSA_IMS_CALL_STATE_TRYING:
        case IMSA_IMS_CALL_STATE_DIALING:
            enMnCallState = CALL_IMSA_SRVCC_CALL_DIALING;
            break;

        case IMSA_IMS_CALL_STATE_ALERTING:
            enMnCallState = CALL_IMSA_SRVCC_CALL_ALERTING;
            break;

        case IMSA_IMS_CALL_STATE_INCOMING:
            enMnCallState = CALL_IMSA_SRVCC_CALL_INCOMING;
            break;

        case IMSA_IMS_CALL_STATE_WAITING:
            enMnCallState = CALL_IMSA_SRVCC_CALL_WAITING;
            break;

        case IMSA_IMS_CALL_STATE_ACTIVING:
            enMnCallState = CALL_IMSA_SRVCC_CALL_ACTVING;
            break;

        case IMSA_IMS_CALL_STATE_INITIALIZING:
        case IMSA_IMS_CALL_STATE_INVALID:
        default:
            IMSA_ERR_LOG1("IMSA_CallImsSrvccState2TafCallState: invalid mapping state", enCallState);

    }

    return enMnCallState;
}


/*****************************************************************************
 Function Name  : CALL_IMSA_GetSrvccCallInfo
 Description    :  IMSA�ṩ��API����TAF CALLģ����á�
                   CALLģ��֪ͨIMSA SRVCC����ʱ��CALLģ����øú�����
                   ���ڻ�ȡSRVCC������ת��CS��ĺ�����Ϣ��
                   ����ڴ��ɵ��÷����䣬pstCallInfoָ����ڴ��СΪCALL_IMSA_MAX_ENTITY_NUM*sizeof(CALL_IMSA_SRVCC_CALL_INFO_STRU)

 Input          : pucCallNumָ����call�������ݣ�pstCallInfoָ��call��Ϣ
 Output         :
 Return Value   : 0 ʧ�ܣ�1 �ɹ�

 History        :
      1.sunbing 49683     2013-12-11  Draft Enact
*****************************************************************************/
VOS_UINT32 CALL_IMSA_GetSrvccCallInfo(VOS_UINT8* pucCallNum,
                                      CALL_IMSA_SRVCC_CALL_INFO_STRU* pstCallInfo)
{
    VOS_UINT8                               ucCount;
    IMSA_CALL_ENTITY_STRU                  *pstCallEntity     = VOS_NULL_PTR;
    IMSA_IMS_SRVCC_CALL_INFO_STRU          *pstSrvccCallInfo  = VOS_NULL_PTR;
    CALL_IMSA_SRVCC_CALL_INFO_STRU         *pstTmpCallInfo    = VOS_NULL_PTR;
    IMSA_IMS_SRVCC_CALL_LIST_STRU           stImsSrvccCallList= {0};

    if((pucCallNum == VOS_NULL_PTR)||(pstCallInfo == VOS_NULL_PTR))
    {
        IMSA_ERR_LOG("CALL_IMSA_GetSrvccCallInfo:Para is err!");
        return VOS_FALSE;
    }

    /*��ʼ��call����*/
    *pucCallNum = 0;

    /*��IMSЭ��ջ��ȡcall�б�*/
    stImsSrvccCallList.ulCallNum = 0;
    (VOS_VOID)IMSA_ImsWriteSrvccCallList((void*)&stImsSrvccCallList);

    /*�жϻ�ȡ�б���Ϣ�Ƿ�Ϸ������Ϸ����ȡʧ��*/
    if((stImsSrvccCallList.ulCallNum > IMSA_IMS_EVENT_MAX_CALL_LIST_SIZE)
        ||(stImsSrvccCallList.ulCallNum == 0))
    {
        IMSA_ERR_LOG("CALL_IMSA_GetSrvccCallInfo:call not exist!");

        return VOS_FALSE;
    }

    pstTmpCallInfo = pstCallInfo;

    /*ѭ������ת��CS���call��Ϣ*/
    for(ucCount = 0; ucCount < stImsSrvccCallList.ulCallNum; ucCount++)
    {
        pstSrvccCallInfo = &stImsSrvccCallList.astCallInfo[ucCount];

        /*����call id�ҵ�����ʵ��*/
        pstCallEntity = IMSA_CallEntityGetUsedByCallId(pstSrvccCallInfo->ulCallIndex);
        if(VOS_NULL_PTR == pstCallEntity)
        {
            IMSA_ERR_LOG("CALL_IMSA_GetSrvccCallInfo: Call info err.");
            continue;
        }

        pstTmpCallInfo->ucCallId    = (VOS_UINT8)pstSrvccCallInfo->ulCallIndex;
        pstTmpCallInfo->enCallState = \
              IMSA_CallImsSrvccState2TafCallState(pstSrvccCallInfo->enCallState);
        pstTmpCallInfo->ucTi        = pstSrvccCallInfo->ucTi;

        /*��¼��callת��CS��*/
        pstCallEntity->bitOpTransToCs                       = VOS_TRUE;

        /*��call��Ϣת��CS����Ҫ����Ϣ*/
        pstTmpCallInfo->clientId    = pstCallEntity->usClientId;
        pstTmpCallInfo->opId        = (VOS_UINT8)pstCallEntity->ulOpId;
        pstTmpCallInfo->enCallDir   = pstCallEntity->enDir;
        pstTmpCallInfo->enMptyState = pstCallEntity->enMpty;
        pstTmpCallInfo->enCallMode  = pstCallEntity->enMode;

        if(IMSA_CALL_TYPE_EMC == pstCallEntity->enType)
        {
            pstTmpCallInfo->enCallType = MN_CALL_TYPE_EMERGENCY;
        }
        else
        {
            pstTmpCallInfo->enCallType = MN_CALL_TYPE_VOICE;
        }

        /*û��EarlyMedia��ʾ��˵��HIFIû������������û���������壬��Ҫ��������*/
        if(VOS_FALSE== pstCallEntity->bitOpEarlyMedia)
        {
            pstTmpCallInfo->ucLocalAlertedFlag = VOS_TRUE;
        }

        /* �������к��� */
        IMSA_CallCallNumberImsa2CS(&pstCallEntity->stCallNumber, \
                                   &pstTmpCallInfo->stCallNumber);

        /* ���±��к��� */
        IMSA_CallCalledNumberImsa2CS(&pstCallEntity->stCalledNumber, \
                                   &pstTmpCallInfo->stCalledNumber);

        /*Call�������ӣ�call��Ϣָ��ƫ�Ƶ���һ���ṹ*/
        *pucCallNum += 1;
        pstTmpCallInfo ++;
    }

    return VOS_TRUE;
}


/*****************************************************************************
 Function Name  : IMSA_CallProcImsMsgEmergencyInd
 Description    : SRVCC�����У�IMS�ϱ�ת��CS���call��Ϣ
 Input          : pstOutputCallEvent      s����¼�
 Output         : VOS_VOID
 Return Value   : ������

 History        :
      1.sunbing 49683     2013-10-11  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_CallProcImsMsgSrvccCallList(IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent)
{
    VOS_UINT8                               ucCount;
    IMSA_CALL_ENTITY_STRU                  *pstCallEntity     = VOS_NULL_PTR;
    IMSA_IMS_SRVCC_CALL_INFO_STRU          *pstSrvccCallInfo  = VOS_NULL_PTR;
    CALL_IMSA_SRVCC_CALL_INFO_NOTIFY_STRU  *pstCallInfoNotify = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CallProcImsMsgSrvccCallList is entered!");

    if((pstOutputCallEvent->stSrvccCallList.ulCallNum > IMSA_IMS_EVENT_MAX_CALL_LIST_SIZE)
       ||(pstOutputCallEvent->stSrvccCallList.ulCallNum == 0))
    {
        IMSA_ERR_LOG1("IMSA_CallProcImsMsgSrvccCallList: Error Call Num. ",
            pstOutputCallEvent->stSrvccCallList.ulCallNum);
        return;
    }

    pstCallInfoNotify = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(CALL_IMSA_SRVCC_CALL_INFO_NOTIFY_STRU));

    if (VOS_NULL_PTR == pstCallInfoNotify)
    {
        IMSA_ERR_LOG("IMSA_CallProcImsMsgSrvccCallList: alloc memory fail");
        return ;
    }

    IMSA_MEM_SET(IMSA_GET_MSG_ENTITY(pstCallInfoNotify), 0, IMSA_GET_MSG_LENGTH(pstCallInfoNotify));

    pstCallInfoNotify->ucCallNum = (VOS_UINT8)pstOutputCallEvent->stSrvccCallList.ulCallNum;

    /*ѭ������ת��CS���call��Ϣ*/
    for(ucCount = 0; ucCount < pstOutputCallEvent->stSrvccCallList.ulCallNum; ucCount++)
    {
        pstSrvccCallInfo = &pstOutputCallEvent->stSrvccCallList.astCallInfo[ucCount];

        /*����call id�ҵ�����ʵ��*/
        pstCallEntity = IMSA_CallEntityGetUsedByCallId(pstSrvccCallInfo->ulCallIndex);
        if(VOS_NULL_PTR == pstCallEntity)
        {
            IMSA_ERR_LOG("IMSA_CallProcImsMsgSrvccCallList: Call info err.");
            continue;
        }

        pstCallInfoNotify->astCallInfo[ucCount].ucCallId    = (VOS_UINT8)pstSrvccCallInfo->ulCallIndex;
        pstCallInfoNotify->astCallInfo[ucCount].enCallState = \
              IMSA_CallImsSrvccState2TafCallState(pstSrvccCallInfo->enCallState);
        pstCallInfoNotify->astCallInfo[ucCount].ucTi        = pstSrvccCallInfo->ucTi;

        /*��¼��callת��CS��*/
        pstCallEntity->bitOpTransToCs                       = VOS_TRUE;

        /*��call��Ϣת��CS����Ҫ����Ϣ*/
        pstCallInfoNotify->astCallInfo[ucCount].clientId    = pstCallEntity->usClientId;
        pstCallInfoNotify->astCallInfo[ucCount].opId        = (VOS_UINT8)pstCallEntity->ulOpId;
        pstCallInfoNotify->astCallInfo[ucCount].enCallDir   = pstCallEntity->enDir;
        pstCallInfoNotify->astCallInfo[ucCount].enMptyState = pstCallEntity->enMpty;
        pstCallInfoNotify->astCallInfo[ucCount].enCallMode  = pstCallEntity->enMode;

        if(IMSA_CALL_TYPE_EMC == pstCallEntity->enType)
        {
            pstCallInfoNotify->astCallInfo[ucCount].enCallType = MN_CALL_TYPE_EMERGENCY;
        }
        else
        {
            pstCallInfoNotify->astCallInfo[ucCount].enCallType = MN_CALL_TYPE_VOICE;
        }

        /*û��EarlyMedia��ʾ��˵��HIFIû������������û���������壬��Ҫ��������*/
        if(VOS_FALSE== pstCallEntity->bitOpEarlyMedia)
        {
            pstCallInfoNotify->astCallInfo[ucCount].ucLocalAlertedFlag = VOS_TRUE;
        }

        /* �������к��� */
        IMSA_CallCallNumberImsa2CS(&pstCallEntity->stCallNumber, \
                                   &pstCallInfoNotify->astCallInfo[ucCount].stCallNumber);

        /* ���±��к��� */
        IMSA_CallCalledNumberImsa2CS(&pstCallEntity->stCalledNumber, \
                                   &pstCallInfoNotify->astCallInfo[ucCount].stCalledNumber);

        /* �������Ӻ��� */
        IMSA_CallCallNumberImsa2CS(&pstCallEntity->stConnectNumber, \
                                   &pstCallInfoNotify->astCallInfo[ucCount].stConnectNumber);

        /* ����ת�ƺ��� */
        IMSA_CallCallNumberImsa2CS(&pstCallEntity->stRedirectNumber, \
                                   &pstCallInfoNotify->astCallInfo[ucCount].stRedirectNumber);
    }

    /* ��ֵDTMF���� */
    pstCallInfoNotify->stDtmfBuffInfo.ucNum = IMSA_CallGetDtmfCnt();
    for(ucCount = 0; ucCount < IMSA_CallGetDtmfCnt(); ucCount++)
    {
        pstCallInfoNotify->stDtmfBuffInfo.astDtmf[ucCount].CallId
                = IMSA_CallGetDtmfBufList(ucCount).ucCallId;
        pstCallInfoNotify->stDtmfBuffInfo.astDtmf[ucCount].cKey
                = IMSA_CallGetDtmfBufList(ucCount).cKey;
        pstCallInfoNotify->stDtmfBuffInfo.astDtmf[ucCount].usOnLength
                = IMSA_CallGetDtmfBufList(ucCount).usOnLength;
        pstCallInfoNotify->stDtmfBuffInfo.astDtmf[ucCount].usClientId
                = IMSA_CallGetDtmfBufList(ucCount).usClientId;
        pstCallInfoNotify->stDtmfBuffInfo.astDtmf[ucCount].opId
                = IMSA_CallGetDtmfBufList(ucCount).ucSpmOpId;
    }

    /* ��д��Ϣͷ */
    IMSA_WRITE_CALL_MSG_HEAD(pstCallInfoNotify, ID_IMSA_CALL_SRVCC_CALL_INFO_NOTIFY);

    /* ������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstCallInfoNotify);

}


VOS_UINT32 IMSA_CallProcImsMsgEmergencyInd(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent)
{
    IMSA_CALL_ENTITY_STRU *pstCallEntity = VOS_NULL_PTR;


    pstCallEntity = IMSA_CallEntityGetUsedByCallId((VOS_UINT32)pstOutputCallEvent->stCallReport.astCalls[0].ulCallIndex);
    if (pstCallEntity)
    {
        pstCallEntity->enType = IMSA_CALL_TYPE_EMC;
    }
    else
    {
        IMSA_ERR_LOG1("IMSA_CallProcImsMsgEmergencyInd: not found call entity ",
            pstOutputCallEvent->stCallReport.astCalls[0].ulCallIndex);
    }

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallIsProccessCallSwap(VOS_VOID)
{
    IMSA_CALL_MANAGER_STRU *pstCallCtx = IMSA_CallCtxGet();

    if ((IMSA_OP_TRUE == pstCallCtx->stSpmMsg.bitOpSpmMsg) &&
        (ID_SPM_IMSA_CALL_SUPS_CMD_REQ == pstCallCtx->stSpmMsg.ulSpmMsgId) &&
        (MN_CALL_SUPS_CMD_HOLD_ACT_ACPT_OTH == pstCallCtx->stSpmMsg.stParam.stSupsParam.enCallSupsCmd))
    {
        return VOS_TRUE;
    }
    else
    {
        return VOS_FALSE;
    }
}


VOS_UINT32 IMSA_CallIsAllCallOnHold(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent)
{
    VOS_UINT32 i = 0;

    for (i = 0; i < pstOutputCallEvent->stCallReport.ulNumCalls; i++)
    {
        if (IMSA_IMS_CALL_STATE_HOLD != pstOutputCallEvent->stCallReport.astCalls[i].enCallState)
        {
            return VOS_FALSE;
        }
    }

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallIsAllCallInConference(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent)
{
    VOS_UINT32 i = 0;

    for (i = 0; i < pstOutputCallEvent->stCallReport.ulNumCalls; i++)
    {
        if (IMSA_IMS_CALL_SINGLE_PARTY == pstOutputCallEvent->stCallReport.astCalls[i].enMultiParty)
        {
            return VOS_FALSE;
        }
    }

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallProcImsStateInvalid(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary)
{
    /* �յ�Invalid״̬֪ͨ�������� */

    (VOS_VOID)pstCallEntity;
    (VOS_VOID)pstCallSummary;

    return VOS_TRUE;
}
IMSA_CALL_STATUS_ENUM_UINT8 IMSA_CallImsState2ImsaState(IMSA_IMS_CALL_STATE_ENUM_UINT8 enImsState, IMSA_CALL_STATUS_ENUM_UINT8 enOldState)
{
    IMSA_CALL_STATUS_ENUM_UINT8 enNewState = enOldState;

    switch (enImsState)
    {
    case IMSA_IMS_CALL_STATE_ACTIVE:
        enNewState = IMSA_CALL_STATUS_ACTIVE;
        break;
    case IMSA_IMS_CALL_STATE_HOLD:
        enNewState = IMSA_CALL_STATUS_HELD;
        break;
    case IMSA_IMS_CALL_STATE_TRYING:
        enNewState = IMSA_CALL_STATUS_TRYING;
        break;
    case IMSA_IMS_CALL_STATE_DIALING:
        enNewState = IMSA_CALL_STATUS_DIALING;
        break;
    case IMSA_IMS_CALL_STATE_ALERTING:
        enNewState = IMSA_CALL_STATUS_ALERTING;
        break;
    case IMSA_IMS_CALL_STATE_INCOMING:
        enNewState = IMSA_CALL_STATUS_INCOMING;
        break;
    case IMSA_IMS_CALL_STATE_WAITING:
        enNewState = IMSA_CALL_STATUS_WAITING;
        break;
    case IMSA_IMS_CALL_STATE_INVALID:
    case IMSA_IMS_CALL_STATE_INITIALIZING:
    default:
        IMSA_ERR_LOG1("IMSA_CallImsState2ImsaState: invalid mapping state", enImsState);
    }

    return enNewState;
}



VOS_UINT32 IMSA_CallProcImsStateActive(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary)
{
    IMSA_CALL_STATUS_ENUM_UINT8         enPreStatus = IMSA_CALL_STATUS_BUTT;
    IMSA_CALL_STATUS_ENUM_UINT8         enNewStatus = IMSA_CALL_STATUS_BUTT;
    IMSA_CALL_MANAGER_STRU             *pstCallCtx  = IMSA_CallCtxGet();


    /**
     * ���û���ҵ���Ӧ�ĺ���ʵ�壬��Ҷϸú���
     * ���ֳ����£��Ƿ���Ҫ����IMS�ĺ��в�����������ʱ����
     * Ŀǰ�ȼ������������棬������������ʱ��
     */

    /* ���û���ҵ���Ӧ�ĺ���ʵ�壬ֱ�ӷ���ʧ�� */
    if (VOS_NULL_PTR == pstCallEntity)
    {
        return VOS_FALSE;
    }

    /* ����ز���CALL����ACTIVE״̬����ֹͣ�ز���ض�ʱ�� */
    if ((VOS_TRUE == IMSA_IsTimerRunning(&pstCallCtx->stRedialMaxTimer))
        && (pstCallCtx->stRedialMaxTimer.usPara < IMSA_CALL_MAX_NUM))
    {
        if (pstCallEntity == &pstCallCtx->astCallEntity[pstCallCtx->stRedialMaxTimer.usPara])
        {
            IMSA_INFO_LOG("IMSA_CallProcImsStateActive stop redial timer");

            /* ��Ҫ�����ԭ��ֵ������CALL�ͷ�ʱ�ϱ�RELEASE�¼�ʱЯ��ԭ��ֵ����� */
            pstCallEntity->bitOpErrorInfo = IMSA_OP_FALSE;
            IMSA_MEM_SET(   &pstCallEntity->stImsError,
                            0,
                            sizeof(IMSA_IMS_CALL_ERROR_STRU));
            pstCallEntity->ucRedialTimes = 0;

            IMSA_StopTimer(&pstCallCtx->stRedialMaxTimer);
            IMSA_StopTimer(&pstCallCtx->stRedialIntervelTimer);
        }
    }

    /* ����֮ǰ��״̬ */
    enPreStatus = pstCallEntity->enStatus;

    /* IMS�ϱ�CALL״̬ΪACTIVE��ACTIVING״̬ʱ����������ú����´���IMS�ϱ�ACTIVING״̬ʱ����Ҫֱ�ӵ���
    ACTIVE���������״̬ת��ʱ�����ֱ�Ӵ���IMSA_IMS_CALL_STATE_ACTIVE */
    enNewStatus = IMSA_CallImsState2ImsaState(IMSA_IMS_CALL_STATE_ACTIVE, enPreStatus);

    if (enPreStatus != enNewStatus)
    {
        /* ���º���״̬ */
        pstCallEntity->enStatus = enNewStatus;

        /* xiongxianghui00253310 modify for conference 20140220 begin */
        IMSA_CallEntityUpdateByCallSummary(pstCallEntity, pstCallSummary);
        /* xiongxianghui00253310 modify for conference 20140220 end */

        IMSA_INFO_LOG1("IMSA_CallProcImsStateActive: MptyState1", pstCallEntity->enMpty);

        /* ���ԭ״̬��HELD����֪ͨSPM Retrieve�¼� */
        if (enPreStatus == IMSA_CALL_STATUS_HELD)
        {
            (VOS_VOID)IMSA_CallSendSpmStateEvt( pstCallEntity->ucId,
                                                pstCallEntity->usClientId,
                                                pstCallEntity->ulOpId,
                                                MN_CALL_EVT_RETRIEVE);
        }
        /* ���״̬�����仯����ԭ״̬����Active����֪ͨSPM CONNECT�¼� */
        else
        {
            (VOS_VOID)IMSA_CallSendSpmStateEvt( pstCallEntity->ucId,
                                                pstCallEntity->usClientId,
                                                pstCallEntity->ulOpId,
                                                MN_CALL_EVT_CONNECT);
        }
    }
    /* xiongxianghui00253310 modify for conference 20140220 begin */
    else
    {
        /* �෽�������ˣ�ԭ״̬��ACTIVE����״̬����ACTIVE��ֻ����IMSA�ڲ�����ʵ�� */
        if (IMSA_IMS_CALL_CONFERENCE == pstCallSummary->enMultiParty)
        {
            IMSA_CallEntityUpdateByCallSummary(pstCallEntity, pstCallSummary);

            (VOS_VOID)IMSA_CallSendSpmStateEvt( pstCallEntity->ucId,
                                                pstCallEntity->usClientId,
                                                pstCallEntity->ulOpId,
                                                MN_CALL_EVT_CONNECT);
        }
    }
    /* xiongxianghui00253310 modify for conference 20140220 end */

    return VOS_TRUE;
}



VOS_UINT32 IMSA_CallAllStateWillBeHeld(VOS_UINT32 ulWillChangedId)
{
    VOS_UINT32 ulUsedCallCount = 0;
    VOS_UINT32 ulHeldCallCount = 0;
    VOS_UINT32 ulNonHeldCallId = 0;
    VOS_UINT32 i = 0;
    IMSA_CALL_MANAGER_STRU *pstCallCtx = IMSA_CallCtxGet();

    /* �������к��в�ͳ����Ϣ */
    for (i = 0; i < IMSA_CALL_MAX_NUM; i++)
    {
        if (VOS_TRUE == pstCallCtx->astCallEntity[i].bitOpIsUsed)
        {
            ulUsedCallCount++;
            if (IMSA_CALL_STATUS_HELD == pstCallCtx->astCallEntity[i].enStatus)
            {
                ulHeldCallCount++;
            }
            else
            {
                ulNonHeldCallId = i;
            }
        }
    }

    /**
     * ������ú��кʹ���HELD״̬���������1��
     * �������������Id�պõ��ڽ�Ҫ���״̬��Id�����������������򷵻�ʧ��
     */
    if ((ulUsedCallCount > 1) &&
        (ulUsedCallCount == ulHeldCallCount + 1) &&
        (ulNonHeldCallId == ulWillChangedId))
    {
        return VOS_TRUE;
    }
    else
    {
        return VOS_FALSE;
    }
}


VOS_UINT32 IMSA_CallProcImsStateHeld(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary)
{
    IMSA_CALL_STATUS_ENUM_UINT8 enPreStatus = IMSA_CALL_STATUS_BUTT;
    IMSA_CALL_STATUS_ENUM_UINT8 enNewStatus = IMSA_CALL_STATUS_BUTT;


    /* ���û���ҵ���Ӧ�ĺ���ʵ�壬ֱ�ӷ���ʧ�� */
    if (VOS_NULL_PTR == pstCallEntity)
    {
        return VOS_FALSE;
    }

    enPreStatus = pstCallEntity->enStatus;
    enNewStatus = IMSA_CallImsState2ImsaState(pstCallSummary->enCallState, enPreStatus);

    /* ���ԭ״̬����״̬��һ�£���֪ͨSPM HELD�¼� */
    if (enPreStatus != enNewStatus)
    {
        /* ���º���״̬ */
        pstCallEntity->enStatus = enNewStatus;

        (VOS_VOID)IMSA_CallSendSpmStateEvt( pstCallEntity->ucId,
                                            pstCallEntity->usClientId,
                                            pstCallEntity->ulOpId,
                                            MN_CALL_EVT_HOLD);
    }

    if(IMSA_CALL_STATUS_HELD == pstCallEntity->enStatus)
    {
        /* ���DTMF��Ϣ */
        IMSA_CallClearDtmfInfo(TAF_CS_CAUSE_CALL_ON_HOLD);
    }

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallProcImsStateTrying(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary)
{
    IMSA_CALL_STATUS_ENUM_UINT8 enPreStatus = IMSA_CALL_STATUS_BUTT;
    IMSA_CALL_STATUS_ENUM_UINT8 enNewStatus = IMSA_CALL_STATUS_BUTT;


    /* ���û���ҵ���Ӧ�ĺ���ʵ�壬ֱ�ӷ���ʧ�� */
    if (VOS_NULL_PTR == pstCallEntity)
    {
        return VOS_FALSE;
    }

    enPreStatus = pstCallEntity->enStatus;
    enNewStatus = IMSA_CallImsState2ImsaState(pstCallSummary->enCallState, enPreStatus);

    /* ���ԭ״̬����״̬��һ�£���֪ͨSPM PROC�¼� */
    if (enPreStatus != enNewStatus)
    {
        /* ���º���״̬ */
        pstCallEntity->enStatus = enNewStatus;

        /* ֪ͨSPM */
        (VOS_VOID)IMSA_CallSendSpmStateEvt( pstCallEntity->ucId,
                                            pstCallEntity->usClientId,
                                            pstCallEntity->ulOpId,
                                            MN_CALL_EVT_CALL_PROC);
    }

    return VOS_TRUE;
}


VOS_UINT32 IMSA_CallProcImsStateDialing(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary)
{
    IMSA_CALL_STATUS_ENUM_UINT8 enPreStatus = IMSA_CALL_STATUS_BUTT;
    IMSA_CALL_STATUS_ENUM_UINT8 enNewStatus = IMSA_CALL_STATUS_BUTT;


    if (pstCallEntity)
    {
        /* �������֮ǰ״̬ */
        enPreStatus = pstCallEntity->enStatus;
        enNewStatus = IMSA_CallImsState2ImsaState(pstCallSummary->enCallState, enPreStatus);

        /* ���º��е��������� */
        IMSA_CallEntityUpdateByCallSummary(pstCallEntity, pstCallSummary);

        /* ���ԭ״̬����״̬��һ�£���֪ͨSPM ORIG�¼� */
        if (enPreStatus != enNewStatus)
        {
            /* ���º���״̬ */
            pstCallEntity->enStatus = enNewStatus;

            /* ֪ͨSPM */
            (VOS_VOID)IMSA_CallSendSpmStateEvt( pstCallEntity->ucId,
                                                pstCallEntity->usClientId,
                                                pstCallEntity->ulOpId,
                                                MN_CALL_EVT_ORIG);
        }
    }
    else
    {
        IMSA_ERR_LOG1("IMSA_CallProcImsStateDialing: not found call ", pstCallSummary->ulCallIndex);
    }

    return VOS_TRUE;
}
VOS_UINT32 IMSA_CallProcImsStateAlerting(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary)
{
    IMSA_CALL_STATUS_ENUM_UINT8         enPreStatus = IMSA_CALL_STATUS_BUTT;
    IMSA_CALL_STATUS_ENUM_UINT8         enNewStatus = IMSA_CALL_STATUS_BUTT;
    IMSA_CALL_MANAGER_STRU             *pstCallCtx  = IMSA_CallCtxGet();


    /* ���û���ҵ���Ӧ�ĺ���ʵ�壬ֱ�ӷ���ʧ�� */
    if (VOS_NULL_PTR == pstCallEntity)
    {
        return VOS_FALSE;
    }

    /* ����ز���CALL����ALERTING״̬����ֹͣ�ز���ض�ʱ�� */
    if ((VOS_TRUE == IMSA_IsTimerRunning(&pstCallCtx->stRedialMaxTimer))
        && (pstCallCtx->stRedialMaxTimer.usPara < IMSA_CALL_MAX_NUM))
    {
        if (pstCallEntity == &pstCallCtx->astCallEntity[pstCallCtx->stRedialMaxTimer.usPara])
        {
            IMSA_INFO_LOG("IMSA_CallProcImsStateAlerting stop redial timer");

            /* ��Ҫ�����ԭ��ֵ������CALL�ͷ�ʱ�ϱ�RELEASE�¼�ʱЯ��ԭ��ֵ����� */
            pstCallEntity->bitOpErrorInfo = IMSA_OP_FALSE;
            IMSA_MEM_SET(   &pstCallEntity->stImsError,
                            0,
                            sizeof(IMSA_IMS_CALL_ERROR_STRU));
            pstCallEntity->ucRedialTimes = 0;
            IMSA_StopTimer(&pstCallCtx->stRedialMaxTimer);
            IMSA_StopTimer(&pstCallCtx->stRedialIntervelTimer);
        }
    }

    enPreStatus = pstCallEntity->enStatus;
    enNewStatus = IMSA_CallImsState2ImsaState(pstCallSummary->enCallState, enPreStatus);

    /* ���ԭ״̬����״̬��һ�£���֪ͨSPM ALERT�¼� */
    if (enPreStatus != enNewStatus)
    {
        /* ���º���״̬ */
        pstCallEntity->enStatus = enNewStatus;

        /* ֪ͨ�Ƿ񱾵����� */
        if (VOS_FALSE == pstCallEntity->bitOpEarlyMedia)
        {
            IMSA_CallSendSpmLocalAlertingInd(MN_CLIENT_ID_BROADCAST, VOS_TRUE);
        }
        /* lihong00150010 �ϱ������������AT�޸�CODEC�������� begin */
        else
        {
            /* IMSA_CallSendSpmLocalAlertingInd(MN_CLIENT_ID_BROADCAST, VOS_FALSE); */
            IMSA_INFO_LOG("IMSA_CallProcImsStateAlerting: do not send ID_IMSA_SPM_CHANNEL_INFO_IND");
        }
        /* lihong00150010 �ϱ������������AT�޸�CODEC�������� end */

        /* ֪ͨSPM */
        (VOS_VOID)IMSA_CallSendSpmStateEvt( pstCallEntity->ucId,
                                            pstCallEntity->usClientId,
                                            pstCallEntity->ulOpId,
                                            MN_CALL_EVT_ALERTING);
    }

    return VOS_TRUE;

}


VOS_UINT32 IMSA_CallProcImsStateIncoming(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary)
{
    VOS_UINT32                          ulResult            = VOS_TRUE;
    IMSA_CALL_STATUS_ENUM_UINT8         enPreStatus         = IMSA_CALL_STATUS_BUTT;
    IMSA_CALL_STATUS_ENUM_UINT8         enNewStatus         = IMSA_CALL_STATUS_BUTT;
    IMSA_CALL_ENTITY_STRU              *pstRedailCallEntity = VOS_NULL_PTR;
    VOS_UINT8                           ucRedialCallIndex   = IMSA_NULL;
    IMSA_CALL_MANAGER_STRU             *pstCallCtx          = IMSA_CallCtxGet();


    /* ����ܹ��ҵ�����ʵ�壬����Ϊ֮ǰ���յ���INITIALIZING EVENT������������˺���ʵ�� */
    if (pstCallEntity)
    {
        enPreStatus = pstCallEntity->enStatus;
        enNewStatus = IMSA_CallImsState2ImsaState(pstCallSummary->enCallState, enPreStatus);

        if (enPreStatus != enNewStatus)
        {
            pstCallEntity->enStatus = enNewStatus;

            /* ֪ͨSPM */
            (VOS_VOID)IMSA_CallSendSpmStateEvt( pstCallEntity->ucId,
                                                pstCallEntity->usClientId,
                                                pstCallEntity->ulOpId,
                                                MN_CALL_EVT_INCOMING);
        }
    }
    else
    /* ���û���ҵ�����ʵ�壬���Է������ʵ�� */
    {
        /* ��������ز����̣������ز����� */
        if (VOS_TRUE == IMSA_IsTimerRunning(&pstCallCtx->stRedialIntervelTimer))
        {
            ucRedialCallIndex   = (VOS_UINT8)pstCallCtx->stRedialIntervelTimer.usPara;
            if (ucRedialCallIndex < IMSA_CALL_MAX_NUM)
            {
                pstRedailCallEntity = &pstCallCtx->astCallEntity[ucRedialCallIndex];
                IMSA_CallInterruptRedial(pstRedailCallEntity);
            }
        }

        pstCallEntity = IMSA_CallEntityAlloc();
        if (pstCallEntity)
        {
            pstCallEntity->enStatus = IMSA_CallImsState2ImsaState(  pstCallSummary->enCallState,
                                                                    pstCallEntity->enStatus);

            /* �������ʵ�� */
            IMSA_CallEntityUpdateByCallSummary(pstCallEntity, pstCallSummary);


            /* ֪ͨSPM */
            (VOS_VOID)IMSA_CallSendSpmStateEvt( pstCallEntity->ucId,
                                                pstCallEntity->usClientId,
                                                pstCallEntity->ulOpId,
                                                MN_CALL_EVT_INCOMING);
        }
        else
        {
            /* ���ʵ�����ʧ�ܣ���ܾ��ú��� */
            (VOS_VOID)IMSA_CallImsCmdRel((VOS_UINT32)pstCallSummary->ulCallIndex);

            ulResult = VOS_FALSE;
        }
    }

    return ulResult;
}
VOS_UINT32 IMSA_CallProcImsStateWaiting(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary)
{
    return IMSA_CallProcImsStateIncoming(pstCallEntity, pstCallSummary);
}


VOS_UINT32 IMSA_CallProcImsStateInitializing(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary)
{
    /**
     * �ݲ������߼�����INITIALIZING EVENT�н���
     */

    (VOS_VOID)pstCallEntity;
    (VOS_VOID)pstCallSummary;

    return VOS_TRUE;
}

/**
 * �յ�IMS�ϱ��ĺ���״̬�����������ݺ���״̬���зֱ���
 *
 * @param[in]   pstCallEntity   �ú��ж�Ӧ��ʵ��
 * @param[in]   pstCallSummary  �ú��ж�Ӧ�ϱ�����Ϣ
 *
 * \note
 * �����pstCallEntity���ܻ��ָ�룬��Ӧ���޷��ҵ���Ӧ����ʵ��ĳ�����
 * ��״̬��������Ҫ���ݸ��Ե��߼����������ֳ���
 */
VOS_UINT32 IMSA_CallProcImsState(IMSA_CALL_ENTITY_STRU *pstCallEntity, const IMSA_IMS_CALL_SUMMARY_STRU *pstCallSummary)
{
    VOS_UINT32 ulResult = VOS_FALSE;


    switch(pstCallSummary->enCallState)
    {
    case IMSA_IMS_CALL_STATE_INVALID:
        ulResult = IMSA_CallProcImsStateInvalid(pstCallEntity, pstCallSummary);
    	break;
    /* IMS�ϱ�״̬ΪACTIVING����������д�������CLCC��ѯCALL״̬ʱ��CALL��ΪWAITING״̬��
    ������·CALLʱ��CLCC��ѯ�������һ·CALL����HOLD״̬��һ·����WAITING״̬�������
    ��ACTVING״̬�Ĵ��� */
    case IMSA_IMS_CALL_STATE_ACTIVE:
    case IMSA_IMS_CALL_STATE_ACTIVING:
        ulResult = IMSA_CallProcImsStateActive(pstCallEntity, pstCallSummary);
    	break;
    case IMSA_IMS_CALL_STATE_HOLD:
        ulResult = IMSA_CallProcImsStateHeld(pstCallEntity, pstCallSummary);
        break;
    case IMSA_IMS_CALL_STATE_TRYING:
        ulResult = IMSA_CallProcImsStateTrying(pstCallEntity, pstCallSummary);
        break;
    case IMSA_IMS_CALL_STATE_DIALING:
        ulResult = IMSA_CallProcImsStateDialing(pstCallEntity, pstCallSummary);
    	break;
    case IMSA_IMS_CALL_STATE_ALERTING:
        ulResult = IMSA_CallProcImsStateAlerting(pstCallEntity, pstCallSummary);
    	break;
    case IMSA_IMS_CALL_STATE_INCOMING:
        ulResult = IMSA_CallProcImsStateIncoming(pstCallEntity, pstCallSummary);
        break;
    case IMSA_IMS_CALL_STATE_WAITING:
        ulResult = IMSA_CallProcImsStateWaiting(pstCallEntity, pstCallSummary);
        break;
    case IMSA_IMS_CALL_STATE_INITIALIZING:
        ulResult = IMSA_CallProcImsStateInitializing(pstCallEntity, pstCallSummary);
        break;
    default:
        IMSA_ERR_LOG1("IMSA_CallProcImsState: invalid call state", pstCallSummary->enCallState);
        break;
    }

    return ulResult;
}


VOS_UINT32 IMSA_CallProcImsMsgCallList(const IMSA_IMS_OUTPUT_CALL_EVENT_STRU *pstOutputCallEvent)
{
    IMSA_CALL_ENTITY_STRU *pstCallEntity = VOS_NULL_PTR;
    VOS_UINT32 i = 0;
    VOS_UINT32 ulResult = VOS_FALSE;
    /*IMSA_CALL_MANAGER_STRU *pstCallCtx = IMSA_CallCtxGet();*/

    /**
     * �������ִ��Call Swap�������ұ���״̬���»ᵼ�����к���״̬��ΪHELD
     * �򲻽��к���״̬�ĸ��£�������֪ͨ���ٽ��и���
     */
    if ((VOS_TRUE == IMSA_CallIsProccessCallSwap()) &&
        (pstOutputCallEvent->stCallReport.ulNumCalls > 1) &&
        (VOS_TRUE == IMSA_CallIsAllCallOnHold(pstOutputCallEvent)) &&
        (VOS_FALSE == IMSA_CallIsAllCallInConference(pstOutputCallEvent)))
    {
        IMSA_INFO_LOG("IMSA_CallProcImsMsgCallList: skip temporary state notification");

        ulResult = VOS_TRUE;
    }
    else
    {
        for (i = 0; i < pstOutputCallEvent->stCallReport.ulNumCalls; i++)
        {
            if (IMSA_IMS_CALL_STATE_DIALING == pstOutputCallEvent->stCallReport.astCalls[i].enCallState)
            {
                pstCallEntity = IMSA_CallEntityGetUsedByCallId(IMSA_CALL_INVALID_ID);
            }
            else
            {
                pstCallEntity = IMSA_CallEntityGetUsedByCallId((VOS_UINT32)pstOutputCallEvent->stCallReport.astCalls[i].ulCallIndex);
            }
            ulResult = IMSA_CallProcImsState(pstCallEntity, &pstOutputCallEvent->stCallReport.astCalls[i]);
        }
    }

    /* ֻ���ڷ�������ҵ��ĺ���ʵ����ڣ�����Ҫ�ϱ� */
    if (VOS_TRUE == pstOutputCallEvent->bitOpSupsrvInfo)
    {
        pstCallEntity = IMSA_CallEntityGetUsedByCallId((VOS_UINT32)pstOutputCallEvent->stSupsrvInfo.ulCallIndex);
        if (VOS_NULL_PTR != pstCallEntity)
        {
            IMSA_CallProcImsSupsrvInfo(&pstOutputCallEvent->stSupsrvInfo);
        }
    }

    return ulResult;
}
MN_CALL_SS_NOTIFY_CODE_ENUM_U8 IMSA_CallImsSupsrvInfo2TafSupsrvInfo
(
    IMSA_IMS_SUPSRV_CALL_NOTICE_ENUM_UINT32 enSupsrvNotification
)
{
    MN_CALL_SS_NOTIFY_CODE_ENUM_U8  enMnSupsrvNotification = MN_CALL_SS_NTFY_BUTT;

    IMSA_INFO_LOG1("IMSA_CallImsSupsrvInfo2TafSupsrvInfo: enSupsrvNotification = ", enSupsrvNotification);

    switch(enSupsrvNotification)
    {
        case IMSA_IMS_SUPSRV_MO_CALL_BEING_FORWARDED:
            enMnSupsrvNotification = MN_CALL_SS_NTFY_BE_FORWORDED;
            break;

        case IMSA_IMS_SUPSRV_MO_CALL_IS_WAITING:
            enMnSupsrvNotification = MN_CALL_SS_NTFY_IS_WAITING;
            break;

        case IMSA_IMS_SUPSRV_MO_CALL_BE_DEFLECTED:
            enMnSupsrvNotification = MN_CALL_SS_NTFY_BE_DEFLECTED;
            break;

        case IMSA_IMS_SUPSRV_MT_CALL_IS_FORWARDED:
            enMnSupsrvNotification = MN_CALL_SS_NTFY_FORWORDED_CALL;
            break;

        case IMSA_IMS_SUPSRV_MT_CALL_IS_HELD:
            enMnSupsrvNotification = MN_CALL_SS_NTFY_ON_HOLD;
            break;

        case IMSA_IMS_SUPSRV_MT_CALL_IS_UNHELD:
            enMnSupsrvNotification = MN_CALL_SS_NTFY_RETRIEVED;
            break;

        case IMSA_IMS_SUPSRV_MT_CALL_JOINED_CONFCALL:
            enMnSupsrvNotification = MN_CALL_SS_NTFY_ENTER_MPTY;
            break;

        case IMSA_IMS_SUPSRV_MT_CALL_EXPLICIT_CALL_TRANSFER:
            enMnSupsrvNotification = MN_CALL_SS_NTFY_EXPLICIT_CALL_TRANSFER;
            break;

        case IMSA_IMS_SUPSRV_MT_CALL_IS_DEFLECTED_CALL:
            enMnSupsrvNotification = MN_CALL_SS_NTFY_DEFLECTED_CALL;
            break;

        default:
            IMSA_ERR_LOG1("IMSA_CallImsSupsrvInfo2TafSupsrvInfo: invalid SupsrvInfo", enSupsrvNotification);

    }
    return enMnSupsrvNotification;

}


VOS_VOID IMSA_CallProcImsSupsrvInfo(const IMSA_IMS_CALL_SUPSRV_INFO_STRU  *pstSupsrvInfo)
{
    MN_CALL_SS_NOTIFY_CODE_ENUM_U8  enMnSupsrvNotification = 0;

    IMSA_INFO_LOG("IMSA_CallProcImsSupsrvInfo entered");

    enMnSupsrvNotification = IMSA_CallImsSupsrvInfo2TafSupsrvInfo(pstSupsrvInfo->enSupsrvNotification);

    /* ���SPM�ϱ��Ĳ���ҵ�����ʹ��󣬲���Ҫ����SPM */
    if (MN_CALL_SS_NTFY_BUTT != enMnSupsrvNotification)
    {
        /* ͬGUȷ�ϣ�CLIENT ID��Ϊ�㲥��OPID ��Ϊ0 */
        (VOS_VOID)IMSA_CallSendSpmSsNotifyEvt(MN_CLIENT_ALL,
                                              0,
                                              pstSupsrvInfo->ulCallIndex,
                                              MN_CALL_EVT_SS_NOTIFY,
                                              TAF_CS_CAUSE_SUCCESS,
                                              enMnSupsrvNotification);
    }

}


/*****************************************************************************
 Function Name  : IMSA_ImsMsgCallEvent()
 Description    : D2IMS�������͵��¼�������
 Input          : VOS_VOID *pRcvMsg
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-06-21  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_ImsMsgCallEvent(VOS_VOID *pCallEvent)
{
    IMSA_IMS_OUTPUT_CALL_EVENT_STRU      *pstOutCall;

    pstOutCall = (IMSA_IMS_OUTPUT_CALL_EVENT_STRU *)pCallEvent;

    if (VOS_NULL_PTR == pstOutCall)
    {
        IMSA_ERR_LOG("IMSA_ImsMsgCallEvent: NULL message");
        return;
    }

    IMSA_INFO_LOG1("IMSA_ImsMsgCallEvent: ", pstOutCall->enOutputCallReason);

    /*ͨ��CSMID���Ҷ�Ӧ��IMSA��ǰ���͵���Ϣ*/
    switch (pstOutCall->enOutputCallReason)
    {
        /**
         * TODO: ��Ҫ���Ǻ��ж�Ӧ��REASON OK��ERROR��SMS��Ӧ��REASON OK������ִ���
         */
        case IMSA_IMS_OUTPUT_CALL_REASON_OK:
            (VOS_VOID)IMSA_CallProcImsMsgOk(pstOutCall);
            break;
        case IMSA_IMS_OUTPUT_CALL_REASON_ERROR:
            (VOS_VOID)IMSA_CallProcImsMsgError(pstOutCall);
            break;
        case IMSA_IMS_OUTPUT_CALL_REASON_CALL_LIST:
            (VOS_VOID)IMSA_CallProcImsMsgCallList(pstOutCall);
            break;
        case IMSA_IMS_OUTPUT_CALL_REASON_DISCONNECT_EVENT:
            (VOS_VOID)IMSA_CallProcImsMsgDisconnect(pstOutCall);
            break;
        case IMSA_IMS_OUTPUT_CALL_REASON_INCOMING_EVENT:
            (VOS_VOID)IMSA_CallProcImsMsgIncoming(pstOutCall);
            break;
        case IMSA_IMS_OUTPUT_CALL_REASON_WAITING_EVENT:
            (VOS_VOID)IMSA_CallProcImsMsgWaiting(pstOutCall);
            break;
        case IMSA_IMS_OUTPUT_CALL_REASON_EARLY_MEDIA:
            (VOS_VOID)IMSA_CallProcImsMsgCallEarlyMedia(pstOutCall);
            break;
        case IMSA_IMS_OUTPUT_CALL_REASON_INITIALIZING_EVENT:
            (VOS_VOID)IMSA_CallProcImsMsgInitializing(pstOutCall);
            break;
        case IMSA_IMS_OUTPUT_CALL_REASON_SRVCC_CALL_LIST:
            IMSA_CallProcImsMsgSrvccCallList(pstOutCall);
            break;
        case IMSA_IMS_OUTPUT_CALL_REASON_EMERGENCY_INDICATION:
            (VOS_VOID)IMSA_CallProcImsMsgEmergencyInd(pstOutCall);
            break;

        case IMSA_IMS_OUTPUT_CALL_REASON_EXTRA_INFO:
            IMSA_INFO_LOG1("IMSA_ImsMsgCallEvent: these reasons SHOULD not occurs", pstOutCall->enOutputCallReason);
            break;
        default:
            IMSA_ERR_LOG1("IMSA_ImsMsgCallEvent: invalid call reason", pstOutCall->enOutputCallReason);
    }
}
/*lint +e961*/
/*lint +e960*/


#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
/* end of ImsaProcImsCallMsg.c */



