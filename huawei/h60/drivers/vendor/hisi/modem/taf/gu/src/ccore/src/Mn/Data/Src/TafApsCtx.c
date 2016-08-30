

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "TafApsCtx.h"
#include "TafLog.h"
#include "TafApsTimerMgmt.h"
#include "TafApsDsFlowStats.h"
#include "PsCommonDef.h"
#include "AtMnInterface.h"
#include "TafApsFsmMsActivatingTbl.h"
#include "TafApsFsmNwActivatingTbl.h"
#include "TafApsFsmMsDeactivatingTbl.h"
#include "TafApsFsmMsModifyingTbl.h"
#include "TafApsApi.h"
#include "Taf_ApsForSndcp.h"
#include "MmcApsInterface.h"
#include "Taf_Aps.h"
#include "TafApsFsmMainTbl.h"
#include "TafApsGetPdpIdList.h"
#include "TafApsProcNvim.h"
#include "MnApsComm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_FILE_ID PS_FILE_ID_TAF_APS_CONTEXT_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

TAF_APS_CONTEXT_STRU                    g_stTafApsCtx;

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


TAF_APS_PDP_ENTITY_FSM_CTX_STRU* TAF_APS_GetPdpEntityFsm(
    VOS_UINT32                          ulMainFsmState,
    TAF_APS_FSM_ID_ENUM_UINT32          enSubFsmId,
    VOS_UINT32                          ulSubFsmState
)
{
    VOS_UINT32                          i;

    for (i = 0; i < TAF_APS_MAX_PDPID; i++)
    {
        if ((ulMainFsmState == g_stTafApsCtx.astPdpEntityFsmCtx[i].stMainFsm.ulState)
         && (enSubFsmId == g_stTafApsCtx.astPdpEntityFsmCtx[i].stSubFsm.enFsmId)
         && (ulSubFsmState == g_stTafApsCtx.astPdpEntityFsmCtx[i].stSubFsm.ulState))
        {
            return &g_stTafApsCtx.astPdpEntityFsmCtx[i];
        }
    }

    return VOS_NULL_PTR;
}


TAF_APS_TIMER_CTX_STRU*  TAF_APS_GetTimerCtxAddr( VOS_VOID )
{
    return g_stTafApsCtx.astApsTimerCtx;
}


VOS_VOID  TAF_APS_InitDsFlowCtx(
    TAF_APS_DSFLOW_STATS_CTX_STRU      *pstApsDsFlowCtx
)
{
    /* ��ʼ������ͳ��������Ϊ0 */
    PS_MEM_SET(pstApsDsFlowCtx, 0, sizeof(TAF_APS_DSFLOW_STATS_CTX_STRU));

    /* ��NV�ж�ȡ����ͳ�������Ϣ */
    TAF_APS_ReadDsFlowInfoFromNv(pstApsDsFlowCtx);

    return;
}



VOS_VOID TAF_APS_InitSwitchDdrInfo(VOS_VOID)
{
    TAF_APS_SWITCH_DDR_RATE_INFO_STRU  *pstSwitchDdrInfo;

    pstSwitchDdrInfo = TAF_APS_GetSwitchDdrRateInfoAddr();

    PS_MEM_SET(pstSwitchDdrInfo, 0, sizeof(TAF_APS_SWITCH_DDR_RATE_INFO_STRU));

    /* APS�����ʼ������������ͶƱCPUƵ�ʵ�75Mhz */
#if defined(INSTANCE_1)

#else

    /* ��NV�ж�ȡ�������� */
    TAF_APS_ReadDsflowRateConfigFromNv(pstSwitchDdrInfo);

    if (0 != TAF_APS_GET_DFS_PROFILE_NUM())
    {
        /* �����DDR��Ƶ��ͶƱ����,��ʼͶƱ��ΪNV���õĵ�һ��Ƶ�� */
        if (PWRCTRL_GET_SUCCESS != PWRCTRL_DfsQosRequest(DFS_QOS_ID_DDR_MINFREQ_E,
                                                         pstSwitchDdrInfo->ulMinBand,
                                                         (BSP_S32 *)&(pstSwitchDdrInfo->iDfsReqId)))
        {
            TAF_APS_DFS_REQUEST_FAIL_CNT(1);
            return;
        }

        TAF_APS_DFS_REQUEST_SUCC_CNT(1);
        pstSwitchDdrInfo->ulReqValue     = pstSwitchDdrInfo->ulMinBand;
        pstSwitchDdrInfo->ulSetMaxFlag   = VOS_FALSE;
    }

#endif

    return;
}
TAF_APS_SWITCH_DDR_RATE_INFO_STRU*  TAF_APS_GetSwitchDdrRateInfoAddr(VOS_VOID)
{
    return &g_stTafApsCtx.stSwitchDdrRateInfo;
}


TAF_APS_DSFLOW_STATS_CTX_STRU*  TAF_APS_GetDsFlowCtxAddr( VOS_VOID )
{
    return &g_stTafApsCtx.stApsDsFlowCtx;
}



TAF_APS_MSG_QUEUE_STRU* TAF_APS_GetCachMsgBufferAddr(VOS_VOID)
{
    return &(g_stTafApsCtx.stBufferEntryMsgQueue);
}


TAF_APS_PS_CALL_ANSWER_MSG_STRU* TAF_APS_GetCachPsCallMsg(
    VOS_UINT8                           ucPdpId
)
{
    return &(g_stTafApsCtx.astPdpEntityFsmCtx[ucPdpId].stSubFsm.unFsmCtx.stNwActivatingCtx.stAnswerMsg);
}


VOS_UINT8  TAF_APS_GetCacheNum( VOS_VOID )
{
    TAF_APS_MSG_QUEUE_STRU             *pstMsgQueue = VOS_NULL_PTR;

    pstMsgQueue                         = TAF_APS_GetCachMsgBufferAddr();

    return pstMsgQueue->ucCacheMsgNum;
}


VOS_VOID  TAF_APS_SaveCacheMsgInMsgQueue(
    VOS_UINT32                          ulEventType,
    VOS_VOID                           *pstMsg
)
{
    TAF_APS_MSG_QUEUE_STRU              *pstMsgQueue   = VOS_NULL_PTR;
    MSG_HEADER_STRU                     *pstMsgHeader  = VOS_NULL_PTR;

    pstMsgHeader = (MSG_HEADER_STRU*)pstMsg;

    pstMsgQueue = TAF_APS_GetCachMsgBufferAddr();

    if ( pstMsgQueue->ucCacheMsgNum >= TAF_APS_MAX_MSG_QUEUE_NUM )
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_APS_SaveCacheMsgInMsgQueue:No Empty buffer");

        /* �������������Ϣ */

        return ;
    }

    pstMsgQueue->astMsgQueue[pstMsgQueue->ucCacheMsgNum].stMsgEntry.ulEventType   = ulEventType;
    PS_MEM_CPY((pstMsgQueue->astMsgQueue[pstMsgQueue->ucCacheMsgNum].stMsgEntry.aucEntryMsgBuffer),
               pstMsg,
               pstMsgHeader->ulLength + VOS_MSG_HEAD_LENGTH);
    pstMsgQueue->ucCacheMsgNum++;

    TAF_INFO_LOG1(WUEPS_PID_TAF, "TAF_APS_SaveCacheMsgInMsgQueue:Cache Num", pstMsgQueue->ucCacheMsgNum);

    /* ���������Ϣ */
}


VOS_VOID  TAF_APS_SaveCachePsCallAnswerMsg(
    AT_MN_MSGTYPE_ENUM_UINT16           enMsgType,
    VOS_UINT8                           ucPdpid
)
{
    TAF_APS_PS_CALL_ANSWER_MSG_STRU     *pstMsg = VOS_NULL_PTR;

    pstMsg = TAF_APS_GetCachPsCallMsg(ucPdpid);

    pstMsg->enMsgType = enMsgType;
    pstMsg->ucPdpid   = ucPdpid;
}


VOS_UINT32  TAF_APS_SaveCacheMsg(
    VOS_UINT32                          ulEventType,
    VOS_VOID                           *pstMsg
)
{
    MSG_HEADER_STRU                     *pstMsgHeader  = VOS_NULL_PTR;

    pstMsgHeader = (MSG_HEADER_STRU*)pstMsg;

    if ( pstMsgHeader->ulLength >= ( TAF_APS_MAX_MSG_BUFFER_LEN - VOS_MSG_HEAD_LENGTH) )
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_APS_SaveCacheMsg:Len too Long");
        return VOS_FALSE;
    }

    /* ����Ϣ�����ڻ����ڴ���� */
    TAF_APS_SaveCacheMsgInMsgQueue(ulEventType, pstMsg);

    return VOS_TRUE;
}
VOS_UINT32  TAF_APS_ClearCacheMsg(
    VOS_UINT32                          ulEventType
)
{
    TAF_APS_MSG_QUEUE_STRU             *pstMsgQueue = VOS_NULL_PTR;
    VOS_INT8                            i;
    VOS_INT8                            cCachePos;

    pstMsgQueue                         = TAF_APS_GetCachMsgBufferAddr();
    cCachePos                           = -1;

    /* ���ҵ���Ҫ������Ļ��� */
    for ( i = 0 ; i < pstMsgQueue->ucCacheMsgNum ; i++ )
    {
        if ( ulEventType == pstMsgQueue->astMsgQueue[i].stMsgEntry.ulEventType )
        {
            PS_MEM_SET(&(pstMsgQueue->astMsgQueue[i].stMsgEntry),
                       0x00,
                       sizeof(TAF_APS_ENTRY_MSG_STRU));
            cCachePos = (VOS_INT8)i;
            break;
        }
    }

    /* δ�ҵ�����Ϣ����ֱ�ӷ��� */
    if ( -1 == cCachePos )
    {
        TAF_INFO_LOG1(WUEPS_PID_TAF,"TAF_APS_ClearCacheMsg:invalid msg type", (VOS_INT32)ulEventType);
        return VOS_FALSE;
    }

    /* Ϊ��֤�����ȳ�,��Ҫ���������Ϣ�Ƶ�ǰ�� */
    for ( i = cCachePos ; i < (pstMsgQueue->ucCacheMsgNum - 1) ; i++ )
    {
        /* COPY��Ϣ���� */
        PS_MEM_MOVE(&(pstMsgQueue->astMsgQueue[i].stMsgEntry),
                   &(pstMsgQueue->astMsgQueue[i + 1].stMsgEntry),
                   sizeof(TAF_APS_ENTRY_MSG_STRU));

    }

    /* �����������һ */
    pstMsgQueue->ucCacheMsgNum--;

    /* ����ǰ�������ӡ���� */
    /* TAF_APS_LogBufferQueueMsg(VOS_FALSE); */

    return VOS_OK;
}
VOS_UINT32  TAF_APS_GetNextCachedMsg(
    TAF_APS_ENTRY_MSG_STRU             *pstEntryMsg
)
{
    TAF_APS_MSG_QUEUE_STRU             *pstMsgQueue = VOS_NULL_PTR;

    pstMsgQueue                         = TAF_APS_GetCachMsgBufferAddr();

    if ( 0 == pstMsgQueue->ucCacheMsgNum )
    {
        TAF_INFO_LOG(WUEPS_PID_TAF,"TAF_APS_GetNextCachedMsg:Empty buffer");

        return VOS_FALSE;
    }

    /* ���������ȳ�ԭ��,Ĭ��ȡ��1���������� */
    PS_MEM_CPY(pstEntryMsg,
               &(pstMsgQueue->astMsgQueue[0].stMsgEntry),
               sizeof(TAF_APS_ENTRY_MSG_STRU));

    /* ��Ϣ�Ѿ�ȡ���������ǰ���� */
    TAF_APS_ClearCacheMsg(pstEntryMsg->ulEventType);

    return VOS_TRUE;
}


VOS_VOID TAF_APS_SetCurrPdpEntityFsmAddr(
    VOS_UINT8                           ucPdpId
)
{
    g_stTafApsCtx.pstCurrPdpEntityFsmCtx = &g_stTafApsCtx.astPdpEntityFsmCtx[ucPdpId];
}




TAF_APS_PDP_ENTITY_FSM_CTX_STRU* TAF_APS_GetCurrPdpEntityFsmAddr(VOS_VOID)
{
    return g_stTafApsCtx.pstCurrPdpEntityFsmCtx;
}


VOS_VOID  TAF_APS_InitCmdBufferQueue(
    TAF_APS_CMD_BUFFER_STRU            *pstCmdBufferQueue
)
{
    VOS_UINT32                          i;

    for ( i = 0 ; i < TAF_APS_MAX_CMD_BUFFER_QUEUE_SIZE; i++ )
    {
        pstCmdBufferQueue[i].pulMsgInfo     = VOS_NULL_PTR;
        pstCmdBufferQueue[i].ulMsgInfoLen   = 0;
        pstCmdBufferQueue[i].enTimerId      = TI_TAF_APS_TIMER_BUTT;
        pstCmdBufferQueue[i].ulPara         = TAF_APS_INVALID_CMD_PARA;
    }

    return;
}


TAF_APS_CMD_BUFFER_STRU  *TAF_APS_GetCmdBufferQueueAddr( VOS_VOID )
{
    return g_stTafApsCtx.astCmdBufferQueue;
}


VOS_VOID  TAF_APS_DelItemInCmdBufferQueue(
    TAF_APS_TIMER_ID_ENUM_UINT32        enTimerId,
    VOS_UINT32                          ulPara
)
{
    VOS_UINT32                          i;
    TAF_APS_CMD_BUFFER_STRU            *pstCmdBufferQueue;

    pstCmdBufferQueue = TAF_APS_GetCmdBufferQueueAddr();

    for ( i = 0 ; i < TAF_APS_MAX_CMD_BUFFER_QUEUE_SIZE; i++ )
    {
        if ((enTimerId == pstCmdBufferQueue[i].enTimerId)
         && (ulPara == pstCmdBufferQueue[i].ulPara))
        {
            /* ����л������Ϣ���ͷ���Ϣ�ڴ� */
            if (VOS_NULL_PTR != pstCmdBufferQueue[i].pulMsgInfo)
            {
                PS_MEM_FREE(WUEPS_PID_TAF, pstCmdBufferQueue[i].pulMsgInfo);
            }

            /* ��ʼ�� */
            pstCmdBufferQueue[i].pulMsgInfo     = VOS_NULL_PTR;
            pstCmdBufferQueue[i].enTimerId      = TI_TAF_APS_TIMER_BUTT;
            pstCmdBufferQueue[i].ulMsgInfoLen   = 0;
            pstCmdBufferQueue[i].ulPara         = TAF_APS_INVALID_CMD_PARA;
            break;
        }
    }

    return;
}


VOS_VOID  TAF_APS_SaveItemInCmdBufferQueue(
    TAF_APS_TIMER_ID_ENUM_UINT32        enTimerId,
    VOS_UINT32                         *pulMsgInfo,
    VOS_UINT32                          ulMsgInfoLen,
    VOS_UINT32                          ulPara
)
{
    VOS_UINT32                          i;
    VOS_UINT32                         *pulMsgBuf;
    TAF_APS_CMD_BUFFER_STRU            *pstCmdBufferQueue;

    /* �����Ϣָ��Ϊ�գ�ֱ�ӷ��� */
    if (VOS_NULL_PTR == pulMsgInfo)
    {
        return;
    }

    pstCmdBufferQueue = TAF_APS_GetCmdBufferQueueAddr();

    for ( i = 0 ; i < TAF_APS_MAX_CMD_BUFFER_QUEUE_SIZE; i++ )
    {
        if (TI_TAF_APS_TIMER_BUTT == pstCmdBufferQueue[i].enTimerId)
        {
            break;
        }
    }

    /* �ж���ǰ��ѭ�������Ƿ��Ѿ����ˣ����������ֱ�ӷ��أ������쳣��ӡ */
    if ( i >= TAF_APS_MAX_CMD_BUFFER_QUEUE_SIZE )
    {
        /* ����������� */
        TAF_ERROR_LOG1(WUEPS_PID_TAF,
                       "TAF_APS_SaveInfoInWaitL4aCnfMsgQueue, Buffer Full, Timer Id :",
                       enTimerId);

        return;
    }

    /* �ڶ���������Ӧ����Ϣ�������ڴ棬������Ϣ��ֵ */
    pulMsgBuf = (VOS_UINT32 *)PS_MEM_ALLOC(WUEPS_PID_TAF, ulMsgInfoLen);
    if (VOS_NULL_PTR == pulMsgBuf)
    {
        /* ����������� */
        TAF_ERROR_LOG1(WUEPS_PID_TAF,
                       "TAF_APS_SaveInfoInWaitL4aCnfMsgQueue, Mem Alloc Fail, Timer Id :",
                       enTimerId);
        return;
    }

    /* ��ֵ */
    PS_MEM_CPY((VOS_UINT8 *)pulMsgBuf, (VOS_UINT8 *)pulMsgInfo, ulMsgInfoLen);
    pstCmdBufferQueue[i].pulMsgInfo     = pulMsgBuf;
    pstCmdBufferQueue[i].enTimerId      = enTimerId;
    pstCmdBufferQueue[i].ulMsgInfoLen   = ulMsgInfoLen;
    pstCmdBufferQueue[i].ulPara         = ulPara;
    return;
}


TAF_APS_CMD_BUFFER_STRU  *TAF_APS_GetItemFromCmdBufferQueue(
    TAF_APS_TIMER_ID_ENUM_UINT32        enTimerId,
    VOS_UINT32                          ulPara
)
{
    VOS_UINT32                          i;
    TAF_APS_CMD_BUFFER_STRU            *pstCmdBufferQueue;


    pstCmdBufferQueue = TAF_APS_GetCmdBufferQueueAddr();

    /* ��������, ��ȡenTimerIdָ������Ϣ�ڵ� */
    for ( i = 0 ; i < TAF_APS_MAX_CMD_BUFFER_QUEUE_SIZE; i++ )
    {
        if ((enTimerId == pstCmdBufferQueue[i].enTimerId)
         && (ulPara == pstCmdBufferQueue[i].ulPara))
        {
            break;
        }
    }

    /* �ж���ǰ��ѭ�������Ƿ��Ѿ����ˣ����������ֱ�ӷ��أ������쳣��ӡ */
    if ( i >= TAF_APS_MAX_CMD_BUFFER_QUEUE_SIZE )
    {
        /* δ�ҵ� */
        TAF_ERROR_LOG1(WUEPS_PID_TAF,
                       "TAF_APS_GetInfoFromWaitL4aCnfMsgQueue, Msg Not Found, Timer Id :",
                       enTimerId);

        return VOS_NULL_PTR;
    }

    /* ���������Ϣָ�뷵�� */
    return &pstCmdBufferQueue[i];
}



VOS_VOID  TAF_APS_SetCurrPdpEntityMainFsmState(
    VOS_UINT32                          ulState
)
{
    g_stTafApsCtx.pstCurrPdpEntityFsmCtx->stMainFsm.ulState = ulState;

    /*  ��ӡ��ǰ��L1��L2״̬ */

    return;
}
VOS_VOID  TAF_APS_SetCurrPdpEntitySubFsmState(
    VOS_UINT32                          ulState
)
{
    g_stTafApsCtx.pstCurrPdpEntityFsmCtx->stSubFsm.ulState = ulState;

    /*  ��ӡ��ǰ��L1��L2״̬ */

    return;
}
VOS_UINT32  TAF_APS_SetPdpIdMainFsmState(
    VOS_UINT8                          ucPdpId,
    VOS_UINT32                         ulState
)
{
    return g_stTafApsCtx.astPdpEntityFsmCtx[ucPdpId].stMainFsm.ulState = ulState;
}



VOS_UINT32  TAF_APS_GetPdpIdMainFsmState(
    VOS_UINT8                          ucPdpId
)
{
    return g_stTafApsCtx.astPdpEntityFsmCtx[ucPdpId].stMainFsm.ulState;
}


VOS_UINT32  TAF_APS_CheckPdpServiceActivated(
    VOS_UINT8                          ucPdpId
)
{
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;

    /* ��ȡ��PDPID��PDPʵ�����ݵ�ַ */
    pstPdpEntity = TAF_APS_GetPdpEntInfoAddr(ucPdpId);

    /* ���NSAPI���, ����Ѿ�δ���, ����ǰ��APSʵ��δ���� */
    if (VOS_FALSE == pstPdpEntity->PdpNsapiFlag)
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}



VOS_UINT32  TAF_APS_SetPdpIdSubFsmState(
    VOS_UINT8                          ucPdpId,
    VOS_UINT32                         ulState
)
{
    return g_stTafApsCtx.astPdpEntityFsmCtx[ucPdpId].stSubFsm.ulState = ulState;
}


VOS_UINT32  TAF_APS_GetPdpIdSubFsmState(
    VOS_UINT8                          ucPdpId
)
{
    return g_stTafApsCtx.astPdpEntityFsmCtx[ucPdpId].stSubFsm.ulState;

}



VOS_VOID  TAF_APS_InitMainFsmCtx(
    TAF_APS_MAIN_FSM_CTX_STRU                *pstCurrFsmCtx
)
{
    pstCurrFsmCtx->enFsmId    = TAF_APS_FSM_MAIN;
    pstCurrFsmCtx->pstFsmDesc = TAF_APS_GetMainFsmDescAddr();
    pstCurrFsmCtx->ulState    = TAF_APS_STA_INACTIVE;
}


VOS_VOID  TAF_APS_InitSubFsmCtx(
    TAF_APS_SUB_FSM_CTX_STRU                *pstCurrFsmCtx
)
{
    pstCurrFsmCtx->enFsmId    = TAF_APS_FSM_BUTT;
    pstCurrFsmCtx->pstFsmDesc = VOS_NULL_PTR;
    PS_MEM_SET(&pstCurrFsmCtx->stEntryMsg,
               0,
               sizeof(pstCurrFsmCtx->stEntryMsg));

}



VOS_VOID TAF_APS_InitSubFsm(
    TAF_APS_FSM_ID_ENUM_UINT32          enFsmId,
    TAF_FSM_DESC_STRU                  *pstFsmDesc,
    VOS_UINT32                          ulState
)
{
    TAF_APS_SUB_FSM_CTX_STRU         *pstCurrSubFsm         = VOS_NULL_PTR;
    TAF_APS_PDP_ENTITY_FSM_CTX_STRU  *pstCurPdpEntityFsmCtx = VOS_NULL_PTR;

    /*  ��ȡ��ǰ��Sub״̬�� */
    pstCurPdpEntityFsmCtx = TAF_APS_GetCurrPdpEntityFsmAddr();
    pstCurrSubFsm         = &pstCurPdpEntityFsmCtx->stSubFsm;

    /* ���õ�ǰSub״̬��״̬���� */
    pstCurrSubFsm->enFsmId     = enFsmId;

    /* ���õ�ǰSub״̬����PDP ID */
    pstCurrSubFsm->pstFsmDesc  = pstFsmDesc;

    /* ���õ�ǰSub״̬����״̬ */
    pstCurrSubFsm->ulState     = ulState;

    /* ��ʼ��Sub״̬�� */
    /* pstCurrSubFsm->pstFsmDesc->pfInitHandle(); */

    /* ��ӡ��ǰ��Sub��Main״̬�� */
    /* TAF_APS_LogMmcFsmInfo(); */
}


VOS_VOID TAF_APS_QuitCurrSubFsm( VOS_VOID )
{
    TAF_APS_SUB_FSM_CTX_STRU         *pstCurrSubFsm         = VOS_NULL_PTR;
    TAF_APS_PDP_ENTITY_FSM_CTX_STRU  *pstCurPdpEntityFsmCtx = VOS_NULL_PTR;

    /*  ��ȡ��ǰ��L2״̬״̬�� */
    pstCurPdpEntityFsmCtx = TAF_APS_GetCurrPdpEntityFsmAddr();
    pstCurrSubFsm         = &pstCurPdpEntityFsmCtx->stSubFsm;

    /* ��ղ��״̬�������� */
    pstCurrSubFsm->pstFsmDesc  = VOS_NULL_PTR;

    /* ��յ�ǰL2״̬��ID */
    pstCurrSubFsm->enFsmId     = TAF_APS_FSM_BUTT;

}
VOS_VOID  TAF_APS_InitInternalBuffer(
    TAF_APS_MSG_QUEUE_STRU             *pstBufferEntryMsgQueue
)
{
    pstBufferEntryMsgQueue->ucCacheMsgNum   = 0;
    pstBufferEntryMsgQueue->ucCurrIndex     = 0;
    PS_MEM_SET(pstBufferEntryMsgQueue->astMsgQueue,
               0x00,
               sizeof(pstBufferEntryMsgQueue->astMsgQueue));
}


TAF_APS_ENTRY_MSG_STRU* TAF_APS_GetCurrSubFsmMsgAddr(VOS_VOID)
{
    TAF_APS_PDP_ENTITY_FSM_CTX_STRU    *pstCurPdpEntityFsmCtx = VOS_NULL_PTR;

    /*  ��ȡ��ǰ��L1״̬����L2״̬״̬�� */
    pstCurPdpEntityFsmCtx = TAF_APS_GetCurrPdpEntityFsmAddr();

    return &(pstCurPdpEntityFsmCtx->stSubFsm.stEntryMsg);
}
VOS_VOID TAF_APS_SaveCurrSubFsmEntryMsg(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_APS_ENTRY_MSG_STRU             *pstEntryMsg  = VOS_NULL_PTR;
    MSG_HEADER_STRU                    *pstMsgHeader = VOS_NULL_PTR;
    VOS_UINT32                          ulLen;

    /* ��õ�ǰ���������Ϣ�Ļ�������ַ */
    /* ��õ�ǰ����APS�����Ϣ�Ļ�������ַ */
    pstEntryMsg                 =   TAF_APS_GetCurrSubFsmMsgAddr();
    pstMsgHeader                =   (MSG_HEADER_STRU*)pstMsg;

    /* ����ǿ���Ϣ */
    if ( VOS_NULL_PTR == pstMsg )
    {
        pstEntryMsg->ulEventType    =   ulEventType;
        TAF_WARNING_LOG( WUEPS_PID_TAF, "TAF_APS_SaveCurrSubFsmEntryMsg Entry Message Pointer is NULL!" );
        return;
    }

    ulLen  = pstMsgHeader->ulLength + VOS_MSG_HEAD_LENGTH;

    /* ����ǰ����������Ϣ������Context��Ӧ�Ļ����� */
    PS_MEM_CPY( pstEntryMsg->aucEntryMsgBuffer, pstMsg, ulLen );

    /* ��дContext�еĲ��� */
    pstEntryMsg->ulEventType    =   ulEventType;

    return;

}
VOS_VOID  TAF_APS_SetCurrFsmEntityPdpId(
    VOS_UINT8                           ucPdpId
)
{
    g_stTafApsCtx.pstCurrPdpEntityFsmCtx->ucPdpId = ucPdpId;
}


VOS_UINT8  TAF_APS_GetCurrFsmEntityPdpId( VOS_VOID )
{
    return g_stTafApsCtx.pstCurrPdpEntityFsmCtx->ucPdpId;
}




VOS_VOID  TAF_APS_SetCurrPdpEntitySimRegStatus(
    VOS_UINT32                          ulPsSimRegStatus
)
{
    g_stTafApsCtx.pstCurrPdpEntityFsmCtx->ulPsSimRegStatus = ulPsSimRegStatus;
}


VOS_UINT32  TAF_APS_GetCurrPdpEntitySimRegStatus( VOS_VOID )
{
    return g_stTafApsCtx.pstCurrPdpEntityFsmCtx->ulPsSimRegStatus;
}


VOS_VOID  TAF_APS_SetCurrPdpEntityRatType(
    MMC_APS_RAT_TYPE_ENUM_UINT32                enRatType
)
{
    g_stTafApsCtx.pstCurrPdpEntityFsmCtx->enRatType = enRatType;
}


MMC_APS_RAT_TYPE_ENUM_UINT32  TAF_APS_GetCurrPdpEntityRatType( VOS_VOID )
{
    return g_stTafApsCtx.pstCurrPdpEntityFsmCtx->enRatType;
}


MMC_APS_RAT_TYPE_ENUM_UINT32 TAF_APS_GetPdpEntityRatType(
    VOS_UINT8                          ucPdpId
)
{
    return g_stTafApsCtx.astPdpEntityFsmCtx[ucPdpId].enRatType;
}


VOS_VOID  TAF_APS_SaveDefaultBearInfo(
    TAF_APS_EPS_BEARER_INFO_IND_STRU       *pstDefaultBearerInfo
)
{
    g_stTafApsCtx.stLteDefaultBearerInfo.stDefaultBearerInfo = *pstDefaultBearerInfo;
    g_stTafApsCtx.stLteDefaultBearerInfo.ulDefaultBearerInfoFlg = VOS_FALSE;
}



TAF_APS_EPS_BEARER_INFO_IND_STRU*  TAF_APS_GetDefaultBearInfo( VOS_VOID )
{
    return &g_stTafApsCtx.stLteDefaultBearerInfo.stDefaultBearerInfo;
}


VOS_VOID  TAF_APS_SetDefaultBearInfoStatus(
    VOS_UINT32                          ulDefaultBearerInfoFlg
)
{
    g_stTafApsCtx.stLteDefaultBearerInfo.ulDefaultBearerInfoFlg = ulDefaultBearerInfoFlg;
}

#if (FEATURE_ON == FEATURE_LTE)

VOS_VOID TAF_APS_FillL4aSdfParaApnInfo(
    APS_L4A_SDF_PARA_STRU       *pstSdfParaInfo,
    TAF_PDP_TABLE_STRU          *pstTafCidInfo
)
{
    if (0 == pstTafCidInfo->CidTab.stApn.ucLength)
    {
        pstSdfParaInfo->bitOpApn    = VOS_FALSE;
    }
    else
    {
        pstSdfParaInfo->bitOpApn    = VOS_TRUE;
        pstSdfParaInfo->stApnInfo.ucApnLen  = pstTafCidInfo->CidTab.stApn.ucLength;
        PS_MEM_CPY(pstSdfParaInfo->stApnInfo.aucApnName,
                   pstTafCidInfo->CidTab.stApn.aucValue,
                   pstSdfParaInfo->stApnInfo.ucApnLen);
    }
}


VOS_VOID TAF_APS_FillL4aSdfParaSdfQosInfo(
    APS_L4A_SDF_PARA_STRU       *pstSdfParaInfo,
    TAF_PDP_TABLE_STRU          *pstTafCidInfo
)
{
    if (APS_FREE == pstTafCidInfo->ucEpsQosInfoFlg)
    {
        pstSdfParaInfo->bitOpSdfQos             = VOS_FALSE;
    }
    else
    {
        pstSdfParaInfo->bitOpSdfQos             = VOS_TRUE;
        pstSdfParaInfo->stSdfQosInfo.ucQCI      = pstTafCidInfo->stEpsQosInfo.ucQCI;
        pstSdfParaInfo->stSdfQosInfo.ulDLGBR    = pstTafCidInfo->stEpsQosInfo.ulDLGBR;
        pstSdfParaInfo->stSdfQosInfo.ulULGBR    = pstTafCidInfo->stEpsQosInfo.ulULGBR;
        pstSdfParaInfo->stSdfQosInfo.ulDLMBR    = pstTafCidInfo->stEpsQosInfo.ulDLMBR;
        pstSdfParaInfo->stSdfQosInfo.ulULMBR    = pstTafCidInfo->stEpsQosInfo.ulULMBR;
    }
}


VOS_VOID TAF_APS_FillL4aSdfParaAuthInfo(
    APS_L4A_SDF_PARA_STRU              *pstSdfParaInfo,
    VOS_UINT8                           ucCid
)
{
    if ( (APS_USED == g_ApsNdisAuthdataTab[ucCid].ucUsed)
      && ( (TAF_PDP_AUTH_TYPE_PAP        == g_ApsNdisAuthdataTab[ucCid].stAuthData.enAuthType)
        || (TAF_PDP_AUTH_TYPE_CHAP       == g_ApsNdisAuthdataTab[ucCid].stAuthData.enAuthType)
        || (TAF_PDP_AUTH_TYPE_MS_CHAP_V2 == g_ApsNdisAuthdataTab[ucCid].stAuthData.enAuthType) ) )
    {
        pstSdfParaInfo->bitOpGwAuthInfo                 = VOS_TRUE;

        /* ��Ȩ���� */
        if (TAF_PDP_AUTH_TYPE_PAP  == g_ApsNdisAuthdataTab[ucCid].stAuthData.enAuthType)
        {
            pstSdfParaInfo->stGwAuthInfo.enGwAuthType   = APS_L4A_GW_AUTH_TYPE_PAP;
        }
        else
        {
            pstSdfParaInfo->stGwAuthInfo.enGwAuthType   = APS_L4A_GW_AUTH_TYPE_CHAP;
        }

        /* Access Number���������ݲ�ʹ�� */
        pstSdfParaInfo->stGwAuthInfo.ucAccNumLen        = 0;

        /* �û��� */
        pstSdfParaInfo->stGwAuthInfo.ucUserNameLen      = (VOS_UINT8)VOS_StrLen((VOS_CHAR*)g_ApsNdisAuthdataTab[ucCid].stAuthData.aucUsername);
        if (0 != pstSdfParaInfo->stGwAuthInfo.ucUserNameLen)
        {
            PS_MEM_CPY(pstSdfParaInfo->stGwAuthInfo.aucUserName,
                       g_ApsNdisAuthdataTab[ucCid].stAuthData.aucUsername,
                       pstSdfParaInfo->stGwAuthInfo.ucUserNameLen);
        }

        /* ���� */
        pstSdfParaInfo->stGwAuthInfo.ucPwdLen           = (VOS_UINT8)VOS_StrLen((VOS_CHAR*)g_ApsNdisAuthdataTab[ucCid].stAuthData.aucPassword);
        if (0 != pstSdfParaInfo->stGwAuthInfo.ucPwdLen)
        {
            PS_MEM_CPY(pstSdfParaInfo->stGwAuthInfo.aucPwd,
                       g_ApsNdisAuthdataTab[ucCid].stAuthData.aucPassword,
                       pstSdfParaInfo->stGwAuthInfo.ucPwdLen);
        }
    }
    else
    {
        pstSdfParaInfo->bitOpGwAuthInfo                 = VOS_FALSE;
    }
}


VOS_UINT32 TAF_APS_GetCidSdfParaInfo(
    VOS_UINT8                           ucCid,
    APS_L4A_SDF_PARA_STRU              *pstSdfParaInfo
)
{
    TAF_PDP_TABLE_STRU                 *pstTafCidInfo;

    if (ucCid > TAF_MAX_CID)
    {
        TAF_PS_ERR_LOG("TAF_APS_GetCidSdfParaInfo: CID is invalid.");
        return VOS_ERR;
    }

    pstTafCidInfo = TAF_APS_GetTafCidInfoAddr(ucCid);

    /* CID */
    pstSdfParaInfo->ulCid = ucCid;

    if (APS_FREE != pstTafCidInfo->ucUsed)
    {
        /* PdpType */
        if (TAF_PDP_PPP == pstTafCidInfo->CidTab.stPdpAddr.enPdpType)
        {
            /* PPP��������ֱ�ӷ���VOS_ERR */
            return VOS_ERR;
        }

        /* Bearer Type & LinkdCID */
        if (APS_FREE == pstTafCidInfo->ucPriCidFlag)
        {
            pstSdfParaInfo->bitOpLinkdCId     = VOS_FALSE;
            pstSdfParaInfo->enBearerCntxtType = APS_L4A_BEARER_TYPE_DEFAULT;

            pstSdfParaInfo->bitOpPdnType      = VOS_TRUE;
            pstSdfParaInfo->enPdnType         = pstTafCidInfo->CidTab.stPdpAddr.enPdpType;
        }
        else
        {
            pstSdfParaInfo->bitOpLinkdCId     = VOS_TRUE;
            pstSdfParaInfo->ulLinkdCid        = pstTafCidInfo->ucPriCid;
            pstSdfParaInfo->enBearerCntxtType = APS_L4A_BEARER_TYPE_DEDICATED;

            pstSdfParaInfo->bitOpPdnType      = VOS_FALSE;
        }

        /* Ipv4AddrAllocType */
        pstSdfParaInfo->bitIpv4AddrAllocType = VOS_TRUE;
        pstSdfParaInfo->enIpv4AddrAllocType  = pstTafCidInfo->CidTab.enIpv4AddrAlloc;

        /* APN */
        TAF_APS_FillL4aSdfParaApnInfo(pstSdfParaInfo, pstTafCidInfo);

    }
    else
    {
        return VOS_ERR;
    }

    /* EPS QOS */
    TAF_APS_FillL4aSdfParaSdfQosInfo(pstSdfParaInfo, pstTafCidInfo);

    /* AuthInfo */
    TAF_APS_FillL4aSdfParaAuthInfo(pstSdfParaInfo, ucCid);

    pstSdfParaInfo->bitOpEmergencyInd   = VOS_TRUE;
    pstSdfParaInfo->enEmergencyInd      = pstTafCidInfo->CidTab.enEmergencyInd;

    pstSdfParaInfo->bitOpImsCnSignalFlag = VOS_TRUE;
    pstSdfParaInfo->enImsCnSignalFlag    = pstTafCidInfo->CidTab.enImCnSignalFlg;

    pstSdfParaInfo->bitOpPcscfDiscovery = VOS_TRUE;
    pstSdfParaInfo->enPcscfDiscovery    = pstTafCidInfo->CidTab.enPcscfDiscovery;

    /* TFT */
    if (APS_FREE != pstTafCidInfo->ucTftTabFlag)
    {
        pstSdfParaInfo->ulPfNum = pstTafCidInfo->ucPfNum;
        PS_MEM_CPY(pstSdfParaInfo->astCntxtTftInfo,
                   pstTafCidInfo->astTftTab,
                   (TAF_MAX_SDF_PF_NUM * sizeof(TAF_PDP_TFT_STRU)));

    }

    return VOS_OK;
}
VOS_UINT32 TAF_APS_GetPdpManageInfo(
    APS_L4A_PDP_MANAGE_INFO_STRU       *pstPdpManageInfo
)
{
    /* ��ȫ�ֱ����л�ȡPDP����ģʽ */
    if (TAF_PDP_ANSWER_MODE_AUTO == g_PsAnsMode.enAnsMode)
    {
        pstPdpManageInfo->enMode = APS_L4A_BEARER_MANAGE_MODE_AUTO;
    }
    else
    {
        pstPdpManageInfo->enMode = APS_L4A_BEARER_MANAGE_MODE_MANUAL;
    }

    if (TAF_PDP_ANSWER_TYPE_ACCEPT == g_PsAnsMode.enAnsType)
    {
        pstPdpManageInfo->enType = APS_L4A_BEARER_MANAGE_TYPE_ACCEPT;
    }
    else
    {
        pstPdpManageInfo->enType = APS_L4A_BEARER_MANAGE_TYPE_REJ;
    }

    return VOS_OK;
}

#endif
TAF_APS_CONTEXT_STRU*  TAF_APS_GetApsCtxAddr( VOS_VOID )
{
    return &(g_stTafApsCtx);
}


VOS_VOID  TAF_APS_InitInternalMsgQueue(
    TAF_APS_INTERNAL_MSG_QUEUE_STRU     *pstInternalMsgQueue                   /* MM�Ӳ���ڲ���Ϣ���� */
)
{
    VOS_UINT32                          i;

    pstInternalMsgQueue->ucHeader   = 0;
    pstInternalMsgQueue->ucTail     = 0;

    for ( i = 0 ; i < TAF_APS_MAX_MSG_QUEUE_SIZE; i++ )
    {
        pstInternalMsgQueue->astApsMsgQueue[i].usMsgID    = TAF_APS_INVALID_INTERNAL_MSG_ID;
        PS_MEM_SET(pstInternalMsgQueue->astApsMsgQueue[i].aucBuf,
                   0x00,
                   TAF_APS_MAX_MSG_BUFFER_LEN);
    }

}
TAF_APS_INTERNAL_MSG_BUF_STRU* TAF_APS_GetNextInternalMsg( VOS_VOID )
{
    TAF_APS_INTERNAL_MSG_BUF_STRU      *pstNextMsg = VOS_NULL_PTR;
    VOS_UINT8                           ucNextIndex;
    TAF_APS_CONTEXT_STRU               *pstApsCtx  =  VOS_NULL_PTR;

    pstApsCtx   = TAF_APS_GetApsCtxAddr();

    if ( pstApsCtx->stInternalMsgQueue.ucHeader != pstApsCtx->stInternalMsgQueue.ucTail)
    {
        ucNextIndex     = pstApsCtx->stInternalMsgQueue.ucHeader;
        pstNextMsg      = (TAF_APS_INTERNAL_MSG_BUF_STRU *)&(pstApsCtx->stInternalMsgQueue.astApsMsgQueue[ucNextIndex]);

        /* ȡ��һ����Ϣ,ͷָ�����ƫ�� */
        (pstApsCtx->stInternalMsgQueue.ucHeader)++;
        (pstApsCtx->stInternalMsgQueue.ucHeader) %= TAF_APS_MAX_MSG_QUEUE_SIZE;
        return pstNextMsg;
    }

    TAF_INFO_LOG(WUEPS_PID_TAF, "TAF_APS_GetNextInternalMsg:Empty Internal Msg");
    return VOS_NULL_PTR;

}
TAF_APS_INTERNAL_MSG_BUF_STRU *TAF_APS_GetIntMsgSendBuf(
    VOS_UINT32                          ulLen
)
{
    TAF_APS_INTERNAL_MSG_BUF_STRU      *pstTafApsMsg        = VOS_NULL_PTR;
    TAF_APS_INTERNAL_MSG_QUEUE_STRU    *pstInternalMsgQueue = VOS_NULL_PTR;

    pstInternalMsgQueue         = &(TAF_APS_GetApsCtxAddr()->stInternalMsgQueue);

    /* ���������л�ȡ��Ϣ������ָ�� */
    pstTafApsMsg = ( TAF_APS_INTERNAL_MSG_BUF_STRU *)&(pstInternalMsgQueue->stSendMsgBuf);

    if ( ulLen > TAF_APS_MAX_MSG_BUFFER_LEN )
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_APS_GetIntMsgSendBuf: Buffer full" );
        return VOS_NULL_PTR;
    }

    /* ��������Ϣ���嵥Ԫ */
    PS_MEM_SET(pstTafApsMsg, 0x00, sizeof(TAF_APS_INTERNAL_MSG_BUF_STRU));

    /* ���ػ�����ָ�� */
    return pstTafApsMsg;
}
VOS_UINT32  TAF_APS_SndInternalMsg(
    VOS_VOID                           *pSndMsg
)
{
    VOS_UINT8                           ucMsgPnt;
    TAF_APS_INTERNAL_MSG_BUF_STRU      *pstApsMsg           = VOS_NULL_PTR;
    TAF_APS_INTERNAL_MSG_QUEUE_STRU    *pstInternalMsgQueue = VOS_NULL_PTR;
    TAF_APS_INTERNAL_MSG_BUF_STRU      *pstSndMsg           = VOS_NULL_PTR;

    pstInternalMsgQueue                 = &(TAF_APS_GetApsCtxAddr()->stInternalMsgQueue);
    pstSndMsg                           = (TAF_APS_INTERNAL_MSG_BUF_STRU*)pSndMsg;

    /* 1. �ж��ڲ���Ϣ�������Ƿ����� */
    ucMsgPnt = pstInternalMsgQueue->ucTail;
    if ( ((ucMsgPnt + 1) % TAF_APS_MAX_MSG_QUEUE_SIZE) == pstInternalMsgQueue->ucHeader )
    {
        TAF_WARNING_LOG(WUEPS_PID_TAF, "TAF_APS_SndInternalMsg:Warning: The Queue is full.");
        return VOS_ERR;
    }

    /* 2. ȡ��һ����Ϣ���嵥Ԫ */
    pstApsMsg = (TAF_APS_INTERNAL_MSG_BUF_STRU *)&(pstInternalMsgQueue->astApsMsgQueue[ucMsgPnt]);

    /* 3. �ж���Ϣ�����Ƿ�Ƿ� */
    if ( pstSndMsg->ulLength > TAF_APS_MAX_MSG_BUFFER_LEN)
    {
        TAF_WARNING_LOG(WUEPS_PID_MMC, "TAF_APS_SndInternalMsg:Warning: ulBufLen is too long.");
        return VOS_ERR;
    }

    TAF_NORMAL_LOG1(WUEPS_PID_TAF, "TAF_APS_SndInternalMsg: usMsgID is ", pstApsMsg->usMsgID);

    /* 4. ���ڲ���Ϣ���͵��ڲ���Ϣ������ */
    PS_MEM_CPY(pstApsMsg, pstSndMsg, sizeof(TAF_APS_INTERNAL_MSG_BUF_STRU));

    /* �����ڲ���Ϣ������ */
    pstInternalMsgQueue->ucTail++;
    pstInternalMsgQueue->ucTail         %= TAF_APS_MAX_MSG_QUEUE_SIZE;

    return VOS_OK;
}


VOS_VOID TAF_APS_InitPdpActLimitInfo(VOS_VOID)
{
    TAF_APS_PDP_ACT_LIMIT_INFO_STRU    *pstPdpActLimitInfo = VOS_NULL_PTR;

    pstPdpActLimitInfo = TAF_APS_GetPdpActLimitInfoAddr();

    /* ��ʼ��PDP�������ƹ��ܿ���flag */
    pstPdpActLimitInfo->ucPdpActLimitFlg  = VOS_FALSE;

    /* ��ʼ������ʧ�ܴ��������� */
    pstPdpActLimitInfo->ucPdpActFailCount = 0;

    /* �ȶ�flag��ʼ�����ٶ�ȡnv��ֵ */
    TAF_APS_ReadPdpActLimitFlgFromNv();

    return;
}
TAF_APS_PDP_ACT_LIMIT_INFO_STRU* TAF_APS_GetPdpActLimitInfoAddr(VOS_VOID)
{
    return &(g_stTafApsCtx.stPdpActLimitInfo);
}



VOS_VOID TAF_APS_FillSdfParaApnInfo(
    TAF_SDF_PARA_STRU           *pstSdfParaInfo,
    TAF_PDP_TABLE_STRU          *pstTafCidInfo
)
{
    if (0 == pstTafCidInfo->CidTab.stApn.ucLength)
    {
        pstSdfParaInfo->bitOpApn    = VOS_FALSE;
    }
    else
    {
        pstSdfParaInfo->bitOpApn    = VOS_TRUE;
        pstSdfParaInfo->stApnInfo.ucLength  = pstTafCidInfo->CidTab.stApn.ucLength;
        PS_MEM_CPY(pstSdfParaInfo->stApnInfo.aucValue,
                   pstTafCidInfo->CidTab.stApn.aucValue,
                   pstSdfParaInfo->stApnInfo.ucLength);
    }
}


VOS_VOID TAF_APS_FillSdfParaSdfQosInfo(
    TAF_SDF_PARA_STRU           *pstSdfParaInfo,
    TAF_PDP_TABLE_STRU          *pstTafCidInfo
)
{
    if (APS_FREE == pstTafCidInfo->ucEpsQosInfoFlg)
    {
        pstSdfParaInfo->bitOpEpsQos             = VOS_FALSE;
    }
    else
    {
        pstSdfParaInfo->bitOpEpsQos             = VOS_TRUE;
        pstSdfParaInfo->stEpsQosInfo.ucQCI      = pstTafCidInfo->stEpsQosInfo.ucQCI;
        pstSdfParaInfo->stEpsQosInfo.ulDLGBR    = pstTafCidInfo->stEpsQosInfo.ulDLGBR;
        pstSdfParaInfo->stEpsQosInfo.ulULGBR    = pstTafCidInfo->stEpsQosInfo.ulULGBR;
        pstSdfParaInfo->stEpsQosInfo.ulDLMBR    = pstTafCidInfo->stEpsQosInfo.ulDLMBR;
        pstSdfParaInfo->stEpsQosInfo.ulULMBR    = pstTafCidInfo->stEpsQosInfo.ulULMBR;
    }
}


VOS_VOID TAF_APS_FillSdfParaAuthInfo(
    TAF_SDF_PARA_STRU                  *pstSdfParaInfo,
    VOS_UINT8                           ucCid
)
{
    if ( (APS_USED == g_ApsNdisAuthdataTab[ucCid].ucUsed)
      && ( (TAF_PDP_AUTH_TYPE_PAP        == g_ApsNdisAuthdataTab[ucCid].stAuthData.enAuthType)
        || (TAF_PDP_AUTH_TYPE_CHAP       == g_ApsNdisAuthdataTab[ucCid].stAuthData.enAuthType)
        || (TAF_PDP_AUTH_TYPE_MS_CHAP_V2 == g_ApsNdisAuthdataTab[ucCid].stAuthData.enAuthType) ) )
    {
        pstSdfParaInfo->bitOpGwAuthInfo                 = VOS_TRUE;

        /* ��Ȩ���� */
        if (TAF_PDP_AUTH_TYPE_PAP  == g_ApsNdisAuthdataTab[ucCid].stAuthData.enAuthType)
        {
            pstSdfParaInfo->stGwAuthInfo.enAuthType     = TAF_PDP_AUTH_TYPE_PAP;
        }
        else
        {
            pstSdfParaInfo->stGwAuthInfo.enAuthType     = TAF_PDP_AUTH_TYPE_CHAP;
        }

        /* Access Number���������ݲ�ʹ�� */
        pstSdfParaInfo->stGwAuthInfo.ucAccNumLen        = 0;

        /* �û��� */
        pstSdfParaInfo->stGwAuthInfo.ucUserNameLen      = (VOS_UINT8)VOS_StrLen((VOS_CHAR*)g_ApsNdisAuthdataTab[ucCid].stAuthData.aucUsername);
        if (0 != pstSdfParaInfo->stGwAuthInfo.ucUserNameLen)
        {
            PS_MEM_CPY(pstSdfParaInfo->stGwAuthInfo.aucUserName,
                       g_ApsNdisAuthdataTab[ucCid].stAuthData.aucUsername,
                       pstSdfParaInfo->stGwAuthInfo.ucUserNameLen);
        }

        /* ���� */
        pstSdfParaInfo->stGwAuthInfo.ucPwdLen           = (VOS_UINT8)VOS_StrLen((VOS_CHAR*)g_ApsNdisAuthdataTab[ucCid].stAuthData.aucPassword);
        if (0 != pstSdfParaInfo->stGwAuthInfo.ucPwdLen)
        {
            PS_MEM_CPY(pstSdfParaInfo->stGwAuthInfo.aucPwd,
                       g_ApsNdisAuthdataTab[ucCid].stAuthData.aucPassword,
                       pstSdfParaInfo->stGwAuthInfo.ucPwdLen);
        }
    }
    else
    {
        pstSdfParaInfo->bitOpGwAuthInfo                 = VOS_FALSE;
    }
}

VOS_UINT32 TAF_APS_GetSdfParaInfo(
    VOS_UINT8                           ucCid,
    TAF_SDF_PARA_STRU                  *pstSdfParaInfo
)
{
    TAF_PDP_TABLE_STRU                 *pstTafCidInfo;

    if (ucCid > TAF_MAX_CID_NV)
    {
        TAF_PS_ERR_LOG("TAF_APS_GetCSdfParaInfo: CID is invalid.");
        return VOS_ERR;
    }

    pstTafCidInfo = TAF_APS_GetTafCidInfoAddr(ucCid);

    if (APS_FREE == pstTafCidInfo->ucUsed)
    {
        return VOS_ERR;
    }

    /* CID */
    pstSdfParaInfo->ucCid = ucCid;

    /* PdpType */
    if (TAF_PDP_PPP == pstTafCidInfo->CidTab.stPdpAddr.enPdpType)
    {
        /* PPP��������ֱ�ӷ���VOS_ERR */
        return VOS_ERR;
    }

    /* LinkdCID */
    if (APS_FREE == pstTafCidInfo->ucPriCidFlag)
    {
        pstSdfParaInfo->bitOpLinkdCid     = VOS_FALSE;
        pstSdfParaInfo->bitOpPdnType      = VOS_TRUE;
        pstSdfParaInfo->enPdnType         = pstTafCidInfo->CidTab.stPdpAddr.enPdpType;
    }
    else
    {
        pstSdfParaInfo->bitOpLinkdCid     = VOS_TRUE;
        pstSdfParaInfo->ucLinkdCid        = pstTafCidInfo->ucPriCid;
        pstSdfParaInfo->bitOpPdnType      = VOS_FALSE;
    }

    /* Ipv4AddrAllocType */
    pstSdfParaInfo->bitOpIpv4AddrAllocType= VOS_TRUE;
    pstSdfParaInfo->enIpv4AddrAllocType   = pstTafCidInfo->CidTab.enIpv4AddrAlloc;

    /* APN */
    TAF_APS_FillSdfParaApnInfo(pstSdfParaInfo, pstTafCidInfo);

    pstSdfParaInfo->bitOpEmergencyInd   = VOS_TRUE;
    pstSdfParaInfo->enEmergencyInd      = pstTafCidInfo->CidTab.enEmergencyInd;

    pstSdfParaInfo->bitOpImCnSignalFlg  = VOS_TRUE;
    pstSdfParaInfo->enImCnSignalFlg     = pstTafCidInfo->CidTab.enImCnSignalFlg;

    pstSdfParaInfo->bitOpPcscfDiscovery = VOS_TRUE;
    pstSdfParaInfo->enPcscfDiscovery    = pstTafCidInfo->CidTab.enPcscfDiscovery;

    /* EPS QOS */
    TAF_APS_FillSdfParaSdfQosInfo(pstSdfParaInfo, pstTafCidInfo);

    /* AuthInfo */
    TAF_APS_FillSdfParaAuthInfo(pstSdfParaInfo, ucCid);

    /* Umts Qos */
    if (VOS_TRUE == pstTafCidInfo->ucQosTabFlag)
    {
        pstSdfParaInfo->bitOpUmtsQos    = VOS_TRUE;
        pstSdfParaInfo->stUmtsQosInfo   = pstTafCidInfo->QosTab;
    }
    /* TFT */
    if (APS_FREE != pstTafCidInfo->ucTftTabFlag)
    {
        pstSdfParaInfo->ucPfNum = pstTafCidInfo->ucPfNum;
        PS_MEM_CPY(pstSdfParaInfo->astCntxtTftInfo,
                   pstTafCidInfo->astTftTab,
                   (TAF_MAX_SDF_PF_NUM * sizeof(TAF_PDP_TFT_STRU)));
    }

    return VOS_OK;
}



VOS_VOID TAF_APS_InitCallRemainTmrLen(VOS_VOID)
{
    TAF_APS_CONTEXT_STRU               *pstTafApsCtx;

    pstTafApsCtx   = TAF_APS_GetApsCtxAddr();
    PS_MEM_SET(pstTafApsCtx->aulCallRemainTmrLen, 0x00, sizeof(pstTafApsCtx->aulCallRemainTmrLen));
    return;
}


VOS_VOID TAF_APS_SetCallRemainTmrLen(
    VOS_UINT8                           ucCid,
    VOS_UINT32                          ulTmrLen
)
{
    TAF_APS_CONTEXT_STRU               *pstTafApsCtx;

    pstTafApsCtx = TAF_APS_GetApsCtxAddr();
    pstTafApsCtx->aulCallRemainTmrLen[ucCid] = ulTmrLen;
    return;
}


VOS_UINT32 TAF_APS_GetCallRemainTmrLen(VOS_UINT8 ucCid)
{
    TAF_APS_CONTEXT_STRU               *pstTafApsCtx;

    pstTafApsCtx = TAF_APS_GetApsCtxAddr();
    return pstTafApsCtx->aulCallRemainTmrLen[ucCid];
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
