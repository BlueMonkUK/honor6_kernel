


/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "TafFsm.h"
#include "TafApsCtx.h"
#include "TafApsMain.h"
#include "TafApsPreProcTbl.h"
#include "TafApsFsmMainTbl.h"
#include "TafApsFsmMsActivatingTbl.h"
#include "TafApsFsmNwActivatingTbl.h"
#include "TafApsFsmMsDeactivatingTbl.h"
#include "TafApsFsmMsModifyingTbl.h"
#include "TafLog.h"
#include "PsCommonDef.h"
#include "ExtAppCmInterface.h"
#include "TafApsGetPdpIdList.h"
#include "TafApsProcIpFilter.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C" {
#endif
#endif


/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_TAF_APS_MAIN_C

/******************************************************************************
   2 �ⲿ������������
******************************************************************************/

/******************************************************************************
   3 ˽�ж���
******************************************************************************/


/******************************************************************************
   4 ȫ�ֱ�������
*****************************************************************************/


/******************************************************************************
   5 ����ʵ��
******************************************************************************/

VOS_UINT32 TAF_APS_RegMainFsm( VOS_VOID  )
{
    /* ״̬��ע�� */

    TAF_FSM_RegisterFsm((TAF_APS_GetMainFsmDescAddr()),
                         "TAF:FSM:APS:MAIN",
                         (VOS_UINT32)(TAF_APS_GetMainFsmStaTblSize()),
                         TAF_APS_GetMainFsmStaTbl(),
                         VOS_NULL_PTR,
                         VOS_NULL_PTR);
    return VOS_OK;
}
VOS_UINT32 TAF_APS_RegSubFsm( VOS_VOID  )
{

    /* MS ACTIVATING״̬��ע�� */
    TAF_FSM_RegisterFsm((TAF_APS_GetMsActivatingFsmDescAddr()),
                         "TAF:FSM:APS:Ms Activating",
                         (VOS_UINT32)TAF_APS_GetMsActivatingStaTblSize(),
                         TAF_APS_GetMsActivatingStaTbl(),
                         VOS_NULL_PTR,
                         VOS_NULL_PTR);


    /* NW ACTIVATING״̬��ע�� */
    TAF_FSM_RegisterFsm((TAF_APS_GetNwActivatingFsmDescAddr()),
                         "TAF:FSM:APS:Nw Activating",
                         (VOS_UINT32)TAF_APS_GetNwActivatingStaTblSize(),
                         TAF_APS_GetNwActivatingStaTbl(),
                         VOS_NULL_PTR,
                         VOS_NULL_PTR);

    /*MS DEACTIVATING״̬��ע�� */
    TAF_FSM_RegisterFsm((TAF_APS_GetMsDeactivatingFsmDescAddr()),
                         "TAF:FSM:APS:MS Deactivating",
                         (VOS_UINT32)TAF_APS_GetMsDeactivatingStaTblSize(),
                         TAF_APS_GetMsDeactivatingStaTbl(),
                         VOS_NULL_PTR,
                         VOS_NULL_PTR);

    /* MS MODIFYING״̬��ע�� */
    TAF_FSM_RegisterFsm((TAF_APS_GetMsModifyingFsmDescAddr()),
                         "TAF:FSM:APS:Ms Modifying",
                         (VOS_UINT32)TAF_APS_GetMsModifyingStaTblSize(),
                         TAF_APS_GetMsModifyingStaTbl(),
                         VOS_NULL_PTR,
                         VOS_NULL_PTR);

    return VOS_OK;
}


VOS_UINT32 TAF_APS_RegPreFsm( VOS_VOID  )
{
    /* Ԥ����ע�� */

    TAF_FSM_RegisterFsm((TAF_APS_GetPreFsmDescAddr()),
                         "NAS:FSM:MMC:PreProcess",
                         TAF_APS_GetPreProcessStaTblSize(),
                         TAF_APS_GetPreProcessStaTbl(),
                         VOS_NULL_PTR,
                         VOS_NULL_PTR);
    return VOS_OK;
}
VOS_UINT32  TAF_APS_InitTask( VOS_VOID )
{

    /* ��ʼ��APSģ��CTX */
    TAF_APS_InitCtx();

    /* ��ʼ��DDR��Ƶ��Ϣ */
    TAF_APS_InitSwitchDdrInfo();

    /*ע��״̬��*/
    TAF_APS_RegFsm();

    return VOS_OK;
}
VOS_VOID  TAF_APS_InitEntityFsm( VOS_VOID )
{
    VOS_UINT8                           i;
    TAF_APS_PDP_ENTITY_FSM_CTX_STRU    *pstCurrPdpEntityFsm;

    /* for ѭ������ʼ������ʵ��״̬�������ģ�������״̬������״̬�� */
    for (i = 0; i < TAF_APS_MAX_PDPID; i++)
    {
        /* �������õ�ǰ��ʵ��״̬�� */
        TAF_APS_SetCurrPdpEntityFsmAddr(i);
        pstCurrPdpEntityFsm = TAF_APS_GetCurrPdpEntityFsmAddr();

        /* ��ʼ��ʵ�������� */
        TAF_APS_InitMainFsmCtx(&pstCurrPdpEntityFsm->stMainFsm);
        TAF_APS_InitSubFsmCtx(&pstCurrPdpEntityFsm->stSubFsm);

        pstCurrPdpEntityFsm->ulPsSimRegStatus = VOS_TRUE;                       /* PS��SIM��״̬��Ϣ,VOS_TRUE:����Ч,VOS_FALSE:����Ч */
        pstCurrPdpEntityFsm->ucPdpId = TAF_APS_INVALID_PDPID;                       /* ��ǰ״̬������Ӧ��PDP ID */
        pstCurrPdpEntityFsm->enRatType = MMC_APS_RAT_TYPE_NULL;                 /* PS��SIM��״̬��Ϣ,VOS_TRUE:����Ч,VOS_FALSE:����Ч */
    }

}

VOS_VOID  TAF_APS_InitCtx( VOS_VOID )
{
    TAF_APS_CONTEXT_STRU               *pApsCtx;

    /* ��ʼ��APSģ��CTX */
    pApsCtx   = TAF_APS_GetApsCtxAddr();

    /* ��ʼ�����е�ʵ��״̬�� */
    TAF_APS_InitEntityFsm();

    /* ��ʼ���ڲ�������� */
    TAF_APS_InitInternalBuffer(TAF_APS_GetCachMsgBufferAddr());

    /* ��ʼ��APS��ʱ�������� */
    TAF_APS_InitAllTimers(TAF_APS_GetTimerCtxAddr());

    /* ��ʼ��APS����ͳ�������� */
    TAF_APS_InitDsFlowCtx(TAF_APS_GetDsFlowCtxAddr());

    /* ��ʼ���ڲ���Ϣ���� */
    TAF_APS_InitInternalMsgQueue(&pApsCtx->stInternalMsgQueue);

    /* ��ʼ��AT�������� */
    TAF_APS_InitCmdBufferQueue(TAF_APS_GetCmdBufferQueueAddr());

    /* ��ʼ������NDIS Filter��Ч */
    TAF_APS_SetUlNdisFilterValidFlag(TAF_APS_UL_NDIS_FILTER_VALID);

    /* ��ʼ������NDIS��Ҫʹ�õ�IP Filter���� */
    TAF_APS_IpfUlNdisFilterInit();

    /* ��ʼ��PDP�������ޱ�־����PDP����ʧ�ܴ��������� */
    TAF_APS_InitPdpActLimitInfo();

    /* ��ʼ�����б�����ʱ��ʱ�� */
    TAF_APS_InitCallRemainTmrLen();
    TAF_APS_SET_RAT_TYPE_IN_SUSPEND(MMC_APS_RAT_TYPE_NULL);
}


VOS_VOID  TAF_APS_RegFsm( VOS_VOID )
{
    /* ע��Ԥ����״̬�� */
    TAF_APS_RegPreFsm();

    /* ע����״̬�� */
    TAF_APS_RegMainFsm();

    /* ע����״̬�� */
    TAF_APS_RegSubFsm();

}
VOS_UINT32 TAF_APS_FSM_ProcessEvent(
    VOS_UINT32                          ulCurState,
    VOS_UINT32                          ulEventType,
    VOS_VOID                           *pRcvMsg,
    TAF_FSM_DESC_STRU                  *pstFsmDesc
)
{
    TAF_ACTION_FUNC                     pActFun;
    VOS_UINT32                          ulRet;

    TAF_INFO_LOG2(WUEPS_PID_TAF,"TAF_APS_FSM_ProcessEvent", ulCurState, ulEventType);

    /* ����״̬ת�Ʊ��е���Ӧ�Ķ��������� */
    pActFun = TAF_FSM_FindAct(pstFsmDesc, ulCurState, ulEventType);

    if (VOS_NULL_PTR != pActFun )
    {
        /* ������ص��¼���������Ϊ��,�����������¼����� */
        ulRet   = (*pActFun) ( ulEventType,(struct MsgCB*)pRcvMsg);
        TAF_INFO_LOG1(WUEPS_PID_TAF,"TAF_APS_FSM_ProcessEvent", ulRet);
        return VOS_TRUE;
    }

    /* ��Ϣδ������������������ */
    return VOS_FALSE;
}
VOS_UINT32 TAF_APS_ProcessBufferMsg( VOS_VOID )
{
    /* ��ǰ��ʵ�ֻ����£������л�����Ϣ�����ֻ�����˺����Ŀ�ܣ�
       ��������Ҫ��ʱ���ٽ������� */

    return VOS_TRUE;
}




VOS_UINT32  TAF_APS_PostProcessMsg(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ����ǰ����Ϣ���л��� */
    /* ��ǰӦ�ò�����ֻ�����Ҫ����˴�����ע�͵�����������״̬�����������Ϣ��
       �����ж��� */
    return VOS_TRUE;
}



VOS_UINT32 TAF_APS_PreProcessMsg(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_ACTION_FUNC                     pActFun;
    TAF_FSM_DESC_STRU                  *pstFsmDesc;
    VOS_UINT32                          ulRet;

    /* ��ϢԤ���� */

    TAF_INFO_LOG1(WUEPS_PID_TAF,"TAF_APS_PreProcessMsg", (VOS_INT32)ulEventType);

    pstFsmDesc = TAF_APS_GetPreFsmDescAddr();

    /* ����״̬ת�Ʊ��е���Ӧ�Ķ��������� */
    pActFun = TAF_FSM_FindAct(pstFsmDesc, TAF_APS_STA_PREPROC, ulEventType);

    if (VOS_NULL_PTR != pActFun )
    {
        /* Ԥ������Ϣ,��Ϊ������Ϣ,һ��ΪԤ��������ͷ���,����һ��ΪԤ����
        ������,��Ȼ��Ҫ���е�״̬���м�������,��˴˴�ֱ��ͨ����������Ԥ��
        ��������Ƿ���Ҫ����״̬������*/
        ulRet = (*pActFun) ( ulEventType, pstMsg);
        return ulRet;
    }

    /* ��Ϣδ������������������ */
    return VOS_FALSE;
}
VOS_UINT32 TAF_APS_ProcessMsgInFsm(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRet;
    TAF_APS_PDP_ENTITY_FSM_CTX_STRU    *pstCurPdpEntityFsmCtx;
    VOS_UINT8                           i;
    TAF_APS_FSM_ID_ENUM_UINT32          enPreSubFsmId;
    TAF_APS_PDPID_LIST_STRU             stPdpIdList;

    /* �����Ҫ�ͳ���PDP�����б�ı���pstPdpIdList */
    PS_MEM_SET(&stPdpIdList, 0, sizeof(TAF_APS_PDPID_LIST_STRU));

    /* ��ȡ��Ҫ������Pdp �б� */
    TAF_APS_GetFsmPdpIdList(ulEventType, pstMsg, &stPdpIdList);

    ulRet = VOS_FALSE;

    for (i = 0; i < stPdpIdList.ucPdpNum; i++)
    {
        /* ����PDP ID�����õ�ǰ��ʵ��״̬�� */
        TAF_APS_SetCurrPdpEntityFsmAddr(stPdpIdList.aucPdpId[i]);

        /* ���õ�ǰ״̬��ʵ���PDP ID */
        TAF_APS_SetCurrFsmEntityPdpId(stPdpIdList.aucPdpId[i]);

        pstCurPdpEntityFsmCtx = TAF_APS_GetCurrPdpEntityFsmAddr();

        /* �����ǰ��״̬���Ѿ����ڣ���ֱ�ӽ��뵽��״̬������ */
        if (TAF_APS_FSM_BUTT != pstCurPdpEntityFsmCtx->stSubFsm.enFsmId)
        {

            enPreSubFsmId = pstCurPdpEntityFsmCtx->stSubFsm.enFsmId;
            ulRet     = TAF_APS_FSM_ProcessEvent( pstCurPdpEntityFsmCtx->stSubFsm.ulState,
                                                  ulEventType,
                                                  (VOS_VOID*)pstMsg,
                                                  pstCurPdpEntityFsmCtx->stSubFsm.pstFsmDesc );
            /*
              ��ǰ��״̬����������仯�����Ҳ����˳���״̬����
              ˵����������״̬���л�����Ҫ�ٴν��뵽״̬����������
            */
            if ( (TAF_APS_FSM_BUTT != pstCurPdpEntityFsmCtx->stSubFsm.enFsmId )
               &&( enPreSubFsmId != pstCurPdpEntityFsmCtx->stSubFsm.enFsmId ) )
            {
                ulRet     = TAF_APS_FSM_ProcessEvent( pstCurPdpEntityFsmCtx->stSubFsm.ulState,
                                                      ulEventType,
                                                      (VOS_VOID*)pstMsg,
                                                      pstCurPdpEntityFsmCtx->stSubFsm.pstFsmDesc );
            }

        }
        else
        {
            /* ������״̬��������Ϣ���� */
            ulRet     = TAF_APS_FSM_ProcessEvent( pstCurPdpEntityFsmCtx->stMainFsm.ulState,
                                                  ulEventType,
                                                  (VOS_VOID*)pstMsg,
                                                  pstCurPdpEntityFsmCtx->stMainFsm.pstFsmDesc );
            if (VOS_TRUE == ulRet)
            {
                /* �����ǰ��״̬���Ѿ������˴������Ҵ�ʱ��״̬�����ڣ�˵����״̬״̬��
                   �������У���������״̬�����ٴν��뵽��״̬�����д��� */
                if (TAF_APS_FSM_BUTT != pstCurPdpEntityFsmCtx->stSubFsm.enFsmId)
                {
                    /* ��Ϣ���� */
                    ulRet     = TAF_APS_FSM_ProcessEvent( pstCurPdpEntityFsmCtx->stSubFsm.ulState,
                                                          ulEventType,
                                                          (VOS_VOID*)pstMsg,
                                                          pstCurPdpEntityFsmCtx->stSubFsm.pstFsmDesc );
                }

            }
        }

    }

    return ulRet;
}
VOS_UINT32 TAF_APS_BuildEventType(
    struct MsgCB                       *pstMsg
)
{
    MSG_HEADER_STRU                    *pstMsgHeader;
    REL_TIMER_MSG                      *pstRcvTimerMsg;
    VOS_UINT32                          ulEventType = 0;
    APS_SNDCP_COMMON_MSG               *pstSnCommonMsg;

    /*
       ���������ʱ����Ϣ���ܿ���Ϣͷ���ƽ������֣�������ṹ���ulName���������
       ���Դ˴����зֱ���
    */
    pstMsgHeader = (MSG_HEADER_STRU*)pstMsg;
    if ( VOS_PID_TIMER == pstMsgHeader->ulSenderPid )
    {
        pstRcvTimerMsg = (REL_TIMER_MSG *)pstMsg;

        ulEventType  = TAF_BuildEventType(pstMsgHeader->ulSenderPid, pstRcvTimerMsg->ulName);

    }
    else if (WUEPS_PID_SM == pstMsgHeader->ulSenderPid)
    {
        ulEventType = TAF_BuildEventType((VOS_UINT16)pstMsgHeader->ulSenderPid, (VOS_UINT16)pstMsgHeader->ulMsgName);
    }
    else if (UEPS_PID_SN == pstMsgHeader->ulSenderPid)
    {
        /* SNDCP�ĸ�ʽ��Ҳ��Ҫ���⴦��һ�£�msgname�ĳ��Ȳ�ͬ */
        pstSnCommonMsg = (APS_SNDCP_COMMON_MSG*)pstMsg;
        ulEventType = TAF_BuildEventType((VOS_UINT16)pstMsgHeader->ulSenderPid, (VOS_UINT16)pstSnCommonMsg->usMsgType);
    }
    else
    {
        ulEventType  = TAF_BuildEventType((VOS_UINT16)pstMsgHeader->ulSenderPid, (VOS_UINT16)pstMsgHeader->ulMsgName);

    }


    return ulEventType;
}
VOS_VOID  TAF_APS_ProcMsgEntry(
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulEventType;
    REL_TIMER_MSG                      *pstRcvTimerMsg;


    if ( VOS_NULL_PTR == pstMsg )
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF,"TAF_APS_ProcMsgEntry:null pointer");
        return;
    }

    /* ����ǳ�ʱ��Ϣ����Ҫ��ʽ�������ʱ��������״̬ */
    if ( VOS_PID_TIMER == ((MSG_HEADER_STRU*)pstMsg)->ulSenderPid )
    {
        pstRcvTimerMsg = (REL_TIMER_MSG *)pstMsg;

        TAF_APS_StopTimer(pstRcvTimerMsg->ulName, pstRcvTimerMsg->ulPara);
    }

    /*
       �����������Ϣ���������շ����ߵ�PID����Ϣ�������֣����߹�ͬ
       ����EventType
    */
    ulEventType = TAF_APS_BuildEventType(pstMsg);


    /* �����Ԥ�������Ѿ����������ֱ�ӷ��� */
    if ( VOS_TRUE == TAF_APS_PreProcessMsg(ulEventType, pstMsg))
    {
        return;
    }

    /* ����״̬������ */
    ulRet  = TAF_APS_ProcessMsgInFsm(ulEventType, pstMsg);

    /* �˴��������е����⣬��Ҫ�����ж���ֻ����״̬�����˳���ʱ��
       ����Ҫ������ */

    /* ����Ϣ��״̬�����Ѿ��������,�ɳ��Դ���һ�»��� */
    if ( VOS_TRUE == ulRet)
    {
        TAF_APS_ProcessBufferMsg();
    }
    /* �ڵ�ǰ״̬����δ������ϣ��ж���Ϣ�Ƿ���Ҫ��ϵ�ǰ��L2״̬��,���� */
    else
    {
        TAF_APS_PostProcessMsg(ulEventType, pstMsg);
    }

    return;

}


VOS_VOID  TAF_APS_ProcMsg(
    struct MsgCB                       *pstMsg
)
{
    TAF_APS_INTERNAL_MSG_BUF_STRU      *pstNextMsg;

    /* �ȴ����ⲿ��Ϣ */
    TAF_APS_ProcMsgEntry(pstMsg);

    /* �ⲿ��Ϣ������ɺ����ڲ���Ϣ */
    pstNextMsg = TAF_APS_GetNextInternalMsg();

    while (VOS_NULL_PTR != pstNextMsg)
    {

        /* ������Ϣ������,�Ա�����PSTAS����ʾ */
        OM_TraceMsgHook(pstNextMsg);
        TAF_APS_ProcMsgEntry((struct MsgCB *)pstNextMsg);

        /* Ѱ����һ���ڲ���Ϣ */
        pstNextMsg = TAF_APS_GetNextInternalMsg();
    }

    return;
}



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif



