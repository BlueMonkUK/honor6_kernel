


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include    "NasEmmGIMComm.h"
#include    "NasEmmMrrc.h"
#include    "NasEmmAttDetInclude.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASEMMMRRCREL_C
/*lint +e767*/

/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/
NAS_EMM_MRRC_CTRL_STRU                  g_stEmmMrrcCtrl;


/*****************************************************************************
  3 Function
*****************************************************************************/
/*****************************************************************************
 Function Name  : NAS_EMM_MrrcRelInit
 Discription    : MRRC ����ģ���ʼ������
 Input          :
 Output         : None
 Return         : None
 History:
      1.  zangyalan 57968 2008-10-16  Draft Enact
*****************************************************************************/
VOS_VOID NAS_EMM_MrrcRelInit(VOS_VOID)
{
    g_stEmmMrrcCtrl.ulRelCause = NAS_EMM_NORMALSERVICE_REL_CAUSE_NULL;

    /*����Դ*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_MRRC_WAIT_RRC_REL_CNF);

    /*�ı�����״̬,ͬʱ�Ͽ���ȫ����*/
    NAS_EMM_MrrcChangeRrcStatusToIdle();

    return;
}

/*lint -e960*/
/*lint -e961*/
VOS_VOID    NAS_EMM_SndRrcRelReq(NAS_LMM_BARRED_IND_ENUM_UINT32 enBarStatus)
{
    LRRC_LMM_REL_REQ_STRU                *pRelReqMsg;

    /* �����ڴ�*/
    pRelReqMsg = (VOS_VOID *)NAS_LMM_ALLOC_MSG(sizeof(LRRC_LMM_REL_REQ_STRU));
    if(NAS_EMM_NULL_PTR == pRelReqMsg)
    {
        /* ��ӡ�ڴ����ʧ��log */
        NAS_EMM_GIM_ERROR_LOG("NAS_EMM_SndRrcRelReq:Alloc Msg Fail!");
        return;
    }

    /*���VOS��Ϣͷ*/
    NAS_EMM_GIM_RRC_MSG_HEADER(pRelReqMsg, (sizeof(LRRC_LMM_REL_REQ_STRU) - NAS_EMM_VOS_HEADER_LEN));

    /*�����ϢID*/
    pRelReqMsg->enMsgId    = ID_LRRC_LMM_REL_REQ;

    /*�������*/
    pRelReqMsg->enBarInd   = enBarStatus;

    /*����RRC_MM_EST_REQ*/
    NAS_LMM_SEND_MSG(pRelReqMsg);

    return;
}


VOS_VOID NAS_EMM_WaitNetworkRelInd(VOS_VOID)
{
    NAS_EMM_FSM_STATE_STRU              EmmState;

    /* �쿴RRC�����Ƿ��Ѿ��ͷ� */
    if (NAS_EMM_YES == NAS_LMM_IsRrcConnectStatusIdle())
    {
        /*��ӡ�Ѿ��ͷ�*/
        NAS_EMM_GIM_NORMAL_LOG("NAS_EMM_WaitNetworkRelInd:Rrc Conn release already");

        /*ת���ڲ���REL IND*/
        NAS_EMM_CommProcConn2Ilde();
    }
    else
    {
        NAS_LMM_MEM_SET(&EmmState, 0, sizeof(NAS_EMM_FSM_STATE_STRU));

        /*ԭ״̬ѹջ*/
        NAS_EMM_FSM_PushState();

        /* ����RRC_CONN_RELINIT.WAIT_RRC_REL״̬ */
        EmmState.enFsmId     = NAS_LMM_PARALLEL_FSM_EMM;
        EmmState.enMainState = EMM_MS_RRC_CONN_REL_INIT;
        EmmState.enSubState  = EMM_SS_RRC_CONN_WAIT_REL_CNF;
        EmmState.enStaTId = TI_NAS_EMM_STATE_T3440;
        NAS_LMM_StaTransProc(EmmState);

        /*������ʱ��TI_NAS_EMM_MRRC_WAIT_RRC_REL*/
        NAS_LMM_StartStateTimer(TI_NAS_EMM_STATE_T3440);
    }
    return ;
}



VOS_VOID NAS_EMM_ReattachDelay(VOS_VOID)
{
    NAS_EMM_FSM_STATE_STRU              EmmState;


    NAS_LMM_MEM_SET(&EmmState, 0, sizeof(NAS_EMM_FSM_STATE_STRU));

    /*ԭ״̬ѹջ*/
    NAS_EMM_FSM_PushState();

    /* ����RRC_CONN_RELINIT.WAIT_RRC_REL״̬ */
    EmmState.enFsmId     = NAS_LMM_PARALLEL_FSM_EMM;
    EmmState.enMainState = EMM_MS_RRC_CONN_REL_INIT;
    EmmState.enSubState  = EMM_SS_RRC_CONN_WAIT_REL_CNF;
    EmmState.enStaTId = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);
    NAS_LMM_StartPtlTimer(TI_NAS_EMM_PTL_REATTACH_DELAY);

    return ;
}




VOS_VOID NAS_EMM_RelReq(NAS_LMM_BARRED_IND_ENUM_UINT32 enBarStatus)
{
    NAS_EMM_FSM_STATE_STRU              EmmState;

    /* �쿴RRC�����Ƿ��Ѿ��ͷ� */
    if (NAS_EMM_YES == NAS_LMM_IsRrcConnectStatusIdle())
    {
        /*��ӡ�Ѿ��ͷ�*/
        NAS_EMM_GIM_NORMAL_LOG("NAS_EMM_RelReq:Rrc Conn release already");

        /*ת���ڲ���REL IND*/
        NAS_EMM_CommProcConn2Ilde();
    }
    else
    {
        NAS_LMM_MEM_SET(&EmmState, 0, sizeof(NAS_EMM_FSM_STATE_STRU));

        /*ԭ״̬ѹջ*/
        NAS_EMM_FSM_PushState();

        /*����RRC_MM_REL_REQ*/
        NAS_EMM_SndRrcRelReq(enBarStatus);

        /* ��RRC���ͷź�����״̬��Ϊ�ͷŹ����� */
        NAS_EMM_SetConnState(NAS_EMM_CONN_RELEASING);

        /* ����RRC_CONN_RELINIT.WAIT_RRC_REL״̬ */
        EmmState.enFsmId     = NAS_LMM_PARALLEL_FSM_EMM;
        EmmState.enMainState = EMM_MS_RRC_CONN_REL_INIT;
        EmmState.enSubState  = EMM_SS_RRC_CONN_WAIT_REL_CNF;
        EmmState.enStaTId = TI_NAS_EMM_MRRC_WAIT_RRC_REL_CNF;
        NAS_LMM_StaTransProc(EmmState);

        /*������ʱ��TI_NAS_EMM_MRRC_WAIT_RRC_REL*/
        NAS_LMM_StartStateTimer(TI_NAS_EMM_MRRC_WAIT_RRC_REL_CNF);
    }

    return ;
}


VOS_UINT32  NAS_EMM_MsRrcConnRelInitSsWaitRrcRelCnfMsgRrcMmRelInd
                                 (VOS_UINT32 ulMsgId, const VOS_VOID *pMsgStru)
{
    LRRC_LMM_REL_IND_STRU              *pstRrcRelInd = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    /* ��ӡ����ú�����INFO_LEVEL */
    NAS_EMM_GIM_INFO_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelCnfMsgRrcMmRelInd is entered.");

    /*��μ��*/
    if ( NAS_EMM_NULL_PTR == pMsgStru)
    {
        NAS_EMM_GIM_ERROR_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelCnfMsgRrcMmRelInd:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /*get the RRC Release Cause*/
    pstRrcRelInd = (LRRC_LMM_REL_IND_STRU *)pMsgStru;

    /*ֹͣ��ʱ��*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_MRRC_WAIT_RRC_REL_CNF);

    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);

    NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_REATTACH_DELAY);


    /*״̬��ջ���ָ���һ��״̬ */
    NAS_EMM_FSM_PopState();

    NAS_EMM_CommProcConn2Ilde();
    if((EMM_MS_REG == NAS_LMM_GetEmmCurFsmMS())
        &&((EMM_SS_REG_NORMAL_SERVICE == NAS_LMM_GetEmmCurFsmSS())
            ||(EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM == NAS_LMM_GetEmmCurFsmSS())))
    {
        NAS_EMM_TAU_MsRegMsgRelInd(pstRrcRelInd->enRelCause);
    }

    if (EMM_MS_AUTH_INIT == NAS_LMM_GetEmmCurFsmMS())
    {
        /* AUTHʧ�ܴ�������*/
        NAS_EMM_GetAuthFailTimes() = 0;

        /* ״̬��ջ*/
        NAS_EMM_FSM_PopState();
    }

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32  NAS_EMM_MsRrcConnRelInitSsWaitRrcRelCnfMsgRrcMmRelCnf
(
    VOS_UINT32 ulMsgId,
    const VOS_VOID *pMsgStru
)
{
    (VOS_VOID)ulMsgId;

    /* ��ӡ����ú�����INFO_LEVEL */
    NAS_EMM_GIM_NORMAL_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgRrcMmRelCnf is entered.");

    /*��μ��*/
    if ( NAS_EMM_NULL_PTR == pMsgStru)
    {
        NAS_EMM_GIM_ERROR_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgRrcMmRelCnf:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /*ֹͣ��ʱ��*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_MRRC_WAIT_RRC_REL_CNF);

    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);

    /*״̬��ջ���ָ���һ��״̬ */
    NAS_EMM_FSM_PopState();

    NAS_EMM_CommProcConn2Ilde();
    if (EMM_MS_AUTH_INIT == NAS_LMM_GetEmmCurFsmMS())
    {
        /* AUTHʧ�ܴ�������*/
        NAS_EMM_GetAuthFailTimes() = 0;

        /* ״̬��ջ*/
        NAS_EMM_FSM_PopState();
    }

    return NAS_LMM_MSG_HANDLED;
}

VOS_UINT32  NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgTIWaitRrcRelTO
                                              (VOS_UINT32 ulMsgId, const VOS_VOID *pMsgStru)
{
    (VOS_VOID)ulMsgId;

    /* ��ӡ����ú�����INFO_LEVEL */
    NAS_EMM_GIM_INFO_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgTIWaitRrcRelTO is entered.");

    /*��μ��*/
    if ( NAS_EMM_NULL_PTR == pMsgStru)
    {
        NAS_EMM_GIM_ERROR_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgTIWaitRrcRelTO:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /*ֹͣ��ʱ��*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_MRRC_WAIT_RRC_REL_CNF);

    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);

    /*״̬��ջ���ָ���һ��״̬ */
    NAS_EMM_FSM_PopState();

    NAS_EMM_CommProcConn2Ilde();
    if (EMM_MS_AUTH_INIT == NAS_LMM_GetEmmCurFsmMS())
    {
        /* AUTHʧ�ܴ�������*/
        NAS_EMM_GetAuthFailTimes() = 0;

        /* ״̬��ջ*/
        NAS_EMM_FSM_PopState();
    }
    return NAS_LMM_MSG_HANDLED;
}

VOS_UINT32  NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsg3440Exp(VOS_UINT32 ulMsgId, const VOS_VOID *pMsgStru)
{
    (VOS_VOID)ulMsgId;

    /* ��ӡ����ú�����INFO_LEVEL */
    NAS_EMM_GIM_INFO_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsg3440Exp is entered.");

    /*��μ��*/
    if ( NAS_EMM_NULL_PTR == pMsgStru)
    {
        NAS_EMM_GIM_ERROR_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsg3440Exp:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /*����RRC_MM_REL_REQ*/
    NAS_EMM_SndRrcRelReq(NAS_LMM_NOT_BARRED);

    NAS_EMM_SetConnState(NAS_EMM_CONN_RELEASING);

    /*������ʱ��TI_NAS_EMM_MRRC_WAIT_RRC_REL*/
    NAS_LMM_StartStateTimer(TI_NAS_EMM_MRRC_WAIT_RRC_REL_CNF);

    return NAS_LMM_MSG_HANDLED;
}




VOS_UINT32  NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgReattachDelayTmrExp(VOS_UINT32 ulMsgId, const VOS_VOID *pMsgStru)
{
    (VOS_VOID)ulMsgId;
    (VOS_VOID)pMsgStru;

    /* ����״̬�´����ݲ�����,������TAU��Service������޸� */

    /* ֹͣT3440��ʱ�� */
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);

    NAS_LMM_StopStateTimer(TI_NAS_EMM_MRRC_WAIT_RRC_REL_CNF);

    /*״̬��ջ���ָ���һ��״̬ */
    NAS_EMM_FSM_PopState();

    /*���ڲ�attach���� */
    (VOS_VOID)NAS_EMM_SendIntraAttachReq();

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32  NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgDrbSetupInd
(
    VOS_UINT32      ulMsgId,
    const VOS_VOID *pMsgStru
)

{
    VOS_UINT32                          ulStaAtStackTop= NAS_EMM_NULL;

    /* ��ӡ����ú�����INFO_LEVEL */
    NAS_EMM_GIM_INFO_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgDrbSetupInd is entered.");

    /*��μ��*/
    if ( NAS_EMM_NULL_PTR == pMsgStru)
    {
        NAS_EMM_GIM_ERROR_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgDrbSetupInd:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }
    (VOS_VOID)ulMsgId;

    ulStaAtStackTop = NAS_LMM_FSM_GetStaAtStackTop(NAS_LMM_PARALLEL_FSM_EMM);

    if ((ulStaAtStackTop == NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_REG,EMM_SS_REG_NORMAL_SERVICE))
         ||(ulStaAtStackTop == NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_REG,EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM)))
    {
        /* ���û��RRC���ͷ�������һ��֮ǰ������3440�������ͷ� */
        if(NAS_EMM_CONN_RELEASING != NAS_EMM_GetConnState())
        {
            /*״̬��ջ���ָ���һ��״̬ */
            NAS_EMM_FSM_PopState();

            NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);
            #if 0
            if (NAS_EMM_NORMALSERVICE_REL_CAUSE_EPSONLY_18 == NAS_EMM_GetNormalServiceRelCause())
            {
                NAS_EMM_GIM_INFO_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgDrbSetupInd Rel cause18.");
                NAS_EMM_RecogAndProc18Disable(NAS_LMM_CAUSE_CS_NOT_AVAIL);
                return NAS_LMM_MSG_HANDLED;
            }

            if (NAS_EMM_NORMALSERVICE_REL_CAUSE_EPSONLY_161722 == NAS_EMM_GetNormalServiceRelCause())
            {
                NAS_EMM_GIM_INFO_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgDrbSetupInd Rel cause161722.");
                NAS_EMM_RecogAndProc161722Disable(NAS_LMM_CAUSE_MSC_UNREACHABLE);
                return NAS_LMM_MSG_HANDLED;
            }

            NAS_EMM_GIM_INFO_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgDrbSetupInd Rel cause2 or other disable lte.");
            NAS_EMM_RecogAndProcSmsOnlyCsfbNotPrefDisable();
            #endif
        }
    }
    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32  NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgCnDetatchReq
(
    VOS_UINT32 ulMsgId,
    const VOS_VOID *pMsgStru
)
{
    VOS_UINT32                          ulStaAtStackTop = NAS_EMM_NULL;
    NAS_EMM_CN_DETACH_REQ_MT_STRU      *pRcvEmmMsg;


    /* ��ӡ����ú�����INFO_LEVEL */
    NAS_EMM_GIM_INFO_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgCnDetatchReq is entered.");

    /*��μ��*/
    if ( NAS_EMM_NULL_PTR == pMsgStru)
    {
        NAS_EMM_GIM_ERROR_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgCnDetatchReq:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }
    (VOS_VOID)ulMsgId;

    #if (FEATURE_PTM == FEATURE_ON)
    NAS_LMM_ErrlogInfoProc(pMsgStru, LMM_ERR_LOG_DETACH_PROCEDURE);
    #endif

    pRcvEmmMsg = (NAS_EMM_CN_DETACH_REQ_MT_STRU *)pMsgStru;

    ulStaAtStackTop = NAS_LMM_FSM_GetStaAtStackTop(NAS_LMM_PARALLEL_FSM_EMM);

    if ((ulStaAtStackTop == NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_REG,EMM_SS_REG_NORMAL_SERVICE))
         ||(ulStaAtStackTop == NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_REG,EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM)))
    {
        /* ���û��RRC���ͷ�������֮ǰһ��������3440�������ͷ� */
        if(NAS_EMM_CONN_RELEASING != NAS_EMM_GetConnState())
        {
            /*״̬��ջ���ָ���һ��״̬ */
            NAS_EMM_FSM_PopState();

            NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);

            NAS_EMM_GLO_AD_GetMtDetachAccTxCnt()  = 0;

            /* �����ܴ���TAU���DETACH����TAU���SERVICE�ĳ�������Ϊ������ڳ�ͻ���������
               ACTIVE��ʶ�������TAU�ɹ��󲻻����ȴ�RRC��·�ͷŵ�״̬ */

            /* 24301 5.3.1�½�ָ����״̬���յ�CN DETACHʱ��Ҫֹͣ3440��ʱ��������CN DETACH��Ϣ��
               upon receipt of a DETACH REQUEST message, the UE shall stop timer T3440
               and respond to the network initiated detach as specified in subclause 5.5.2.3.*/

            /* ����IMSI DETACH���͵�CN DETACH */
            if (NAS_EMM_DETACH_TYPE_MT_IMSI_DETACH == pRcvEmmMsg->ucDetType)
            {
                NAS_EMM_GIM_NORMAL_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgImsiDetachReq:Process Imsi Detach");

                NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgImsiDetachReq(pRcvEmmMsg);

                return  NAS_LMM_MSG_HANDLED;
            }

            /* ����re-attach not required���ͣ�ԭ��ֵΪ#2��CN DETACH */
            if ((NAS_EMM_DETACH_TYPE_MT_NOT_REATTACH == pRcvEmmMsg->ucDetType)
                && (NAS_EMM_BIT_SLCT == pRcvEmmMsg->ucBitOpCause)
                && (NAS_LMM_CAUSE_IMSI_UNKNOWN_IN_HSS == pRcvEmmMsg->ucEmmCause))
            {
                NAS_EMM_GIM_NORMAL_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgNotReattach2DetachReq:Process cause2 detach");

                NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgNotReattach2DetachReq(pRcvEmmMsg);

                return  NAS_LMM_MSG_HANDLED;
            }

            /*����TAUģ���ṩ�ĺ���*/
            NAS_EMM_TAU_AbnormalOver();

            /* ��MMC���� MO DETACH ��� LMM_MMC_TAU_RESULT_IND*/
            /*NAS_EMM_MmcSendTauActionResultIndOthertype(MMC_LMM_TAU_RSLT_MT_DETACH_FAILURE);*/

            /*��MRRC����DETACH ACCEPT��Ϣ*/
            NAS_EMM_MrrcSendDetAcpMt();

            /*��ESM����ID_EMM_ESM_STATUS_IND��Ϣ*/
            NAS_EMM_EsmSendStatResult(          EMM_ESM_ATTACH_STATUS_DETACHED);

            /*detach type == re-attached required*/
            if (NAS_EMM_DETACH_TYPE_MT_REATTACH == pRcvEmmMsg->ucDetType)
            {

                NAS_EMM_AdStateConvert( EMM_MS_DEREG,
                                        EMM_SS_DEREG_NORMAL_SERVICE,
                                        TI_NAS_EMM_STATE_NO_TIMER);

                NAS_LMM_WriteEpsSecuContext(NAS_NV_ITEM_UPDATE);
                /*�ȴ�RRC_REL_IND*/
                NAS_EMM_WaitNetworkRelInd();
            }

            /*detach type == re-attached not required*/
            if (NAS_EMM_DETACH_TYPE_MT_NOT_REATTACH == pRcvEmmMsg->ucDetType)
            {
                NAS_EMM_DetCnDetReqNotReattach(pRcvEmmMsg);
            }

            /*��MMC����LMM_MMC_DETACH_IND��Ϣ*/
            NAS_EMM_AppSendDetIndMt(pRcvEmmMsg);

        }
        /*�ȴ���·�ͷŶ�ʱ������,�������������Ϣ*/

    }
    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32  NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgEsmdataReq
(
    VOS_UINT32 ulMsgId,
    const VOS_VOID *pMsgStru
)

{
    EMM_ESM_DATA_REQ_STRU              *pstsmdatareq = NAS_EMM_NULL_PTR;
    VOS_UINT32                          ulStaAtStackTop = NAS_EMM_NULL;

    (VOS_VOID)ulMsgId;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_GIM_NORMAL_LOG( "NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgEsmdataReq is entered.");

    /*��μ��*/
    if ( NAS_EMM_NULL_PTR == pMsgStru)
    {
        NAS_EMM_GIM_ERROR_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgEsmdataReq:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    pstsmdatareq = (EMM_ESM_DATA_REQ_STRU *)pMsgStru;

    ulStaAtStackTop = NAS_LMM_FSM_GetStaAtStackTop(NAS_LMM_PARALLEL_FSM_EMM);

    if ((ulStaAtStackTop == NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_REG,EMM_SS_REG_NORMAL_SERVICE))
         ||(ulStaAtStackTop == NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_REG,EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM)))
    {
        /* ���û��RRC���ͷ�������֮ǰһ��������3440�������ͷ� */
        if(NAS_EMM_CONN_RELEASING != NAS_EMM_GetConnState())
        {
            if (VOS_TRUE == pstsmdatareq->ulIsEmcType)
            {
                NAS_LMM_SetEmmInfoIsEmerPndEsting(VOS_TRUE);
            }

            /* ͸��SM��Ϣ */
            NAS_EMM_SER_SendMrrcDataReq_ESMdata(&pstsmdatareq->stEsmMsg);
        }
    }

    return NAS_LMM_MSG_HANDLED;
}

VOS_UINT32  NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgTcDataReq
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    EMM_ETC_DATA_REQ_STRU              *pstTcdataReq = NAS_EMM_NULL_PTR;
    VOS_UINT32                          ulStaAtStackTop = NAS_EMM_NULL;

    (VOS_VOID)ulMsgId;
    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_GIM_NORMAL_LOG( "NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgTcDataReq is entered.");

    /*��μ��*/
    if ( NAS_EMM_NULL_PTR == pMsgStru)
    {
        NAS_EMM_GIM_ERROR_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgTcDataReq:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    pstTcdataReq = (EMM_ETC_DATA_REQ_STRU*)pMsgStru;

    ulStaAtStackTop = NAS_LMM_FSM_GetStaAtStackTop(NAS_LMM_PARALLEL_FSM_EMM);

    if ((ulStaAtStackTop == NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_REG,EMM_SS_REG_NORMAL_SERVICE))
         ||(ulStaAtStackTop == NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_REG,EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM)))
    {
        if(NAS_EMM_CONN_RELEASING != NAS_EMM_GetConnState())
        {
            /* ͸��TC��Ϣ */
            NAS_EMM_SER_SendMrrcDataReq_Tcdata(pstTcdataReq);
        }
    }
    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32  NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgRabmRelReq
(
    VOS_UINT32                          ulMsgId,
    const VOS_VOID                           *pMsgStru
)
{
    VOS_UINT32                          ulStaAtStackTop = NAS_EMM_NULL;

    (VOID)ulMsgId;
    NAS_EMM_GIM_NORMAL_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgRabmRelReq is entered");

    /*���״̬�Ƿ�ƥ�䣬����ƥ�䣬�˳�*/
    if ( NAS_EMM_NULL_PTR == pMsgStru)
    {
        NAS_EMM_GIM_ERROR_LOG("NAS_EMM_MsRrcConnRelInitSsWaitRrcRelMsgRabmRelReq:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    ulStaAtStackTop = NAS_LMM_FSM_GetStaAtStackTop(NAS_LMM_PARALLEL_FSM_EMM);

    if ((ulStaAtStackTop == NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_REG,EMM_SS_REG_NORMAL_SERVICE))
         ||(ulStaAtStackTop == NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_REG,EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM)))
    {
        if (NAS_EMM_CONN_RELEASING != NAS_EMM_GetConnState())
        {
            NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);

            /*������ʱ��TI_NAS_EMM_MRRC_WAIT_RRC_REL*/
            NAS_LMM_StartStateTimer(TI_NAS_EMM_MRRC_WAIT_RRC_REL_CNF);

            /*��MRRC����NAS_EMM_MRRC_REL_REQ��Ϣ*/
            NAS_EMM_SndRrcRelReq(NAS_LMM_NOT_BARRED);

            /* ��������״̬Ϊ�ͷŹ����� */
            NAS_EMM_SetConnState(NAS_EMM_CONN_RELEASING);

        }
    }
    return NAS_LMM_MSG_HANDLED;
}

VOS_UINT32  NAS_EMM_MsRrcRelInitSsWaitRelCnfMsgMmcCoverageLostInd(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg)
{
    NAS_EMM_GIM_INFO_LOG("NAS_EMM_MsRrcRelInitSsWaitRelCnfMsgMmcCoverageLostInd: Enter.");

    (VOS_VOID)(ulMsgId);
    (VOS_VOID)(pMsg);

    /* ֹͣ��ǰ��ѹջ���� */
    NAS_LMM_StopStateTimer(TI_NAS_EMM_MRRC_WAIT_RRC_REL_CNF);

    /* ��ջ */
    NAS_EMM_FSM_PopState();


    /* EMM��EMM�Լ�����AREA LOST��Ϣ���Ը�ѹջǰ�����̴��� */
    NAS_EMMC_SendEmmCoverageLostInd();
    if (EMM_MS_AUTH_INIT == NAS_LMM_GetEmmCurFsmMS())
    {
        /* AUTHʧ�ܴ�������*/
        NAS_EMM_GetAuthFailTimes() = 0;

        /* ״̬��ջ*/
        NAS_EMM_FSM_PopState();
    }
    return  NAS_LMM_MSG_HANDLED;
}
VOS_VOID  NAS_EMM_MrrcChangeRrcStatusToIdle( VOS_VOID )
{
    NAS_EMM_GIM_NORMAL_LOG("NAS_EMM_MrrcChangeRrcStatusToIdle is enter");

    /*�����ǰ����״̬����IDLE������ MMC�ϱ�*/
    if(NAS_EMM_CONN_IDLE != NAS_EMM_GetConnState())
    {
        NAS_EMM_SendMmcStatusInd(       MMC_LMM_STATUS_TYPE_CONN_STATE,
                                        MMC_LMM_CONN_IDLE);
    }

    /*��RRC����״̬��Ϊ������״̬*/
    NAS_EMM_GetConnState() = NAS_EMM_CONN_IDLE;

    /*����̬��CONN����IDLE�����µ�ǰ��ȫ������״̬�仯*/
    NAS_EMM_ConnToIdleTransSecuSta();

    return;
}
VOS_VOID  NAS_EMM_ProcDeregMmcRelReq(VOS_VOID )
{
    NAS_EMM_FSM_STATE_STRU              EmmState;

    NAS_EMM_GIM_NORMAL_LOG("NAS_EMM_ProcDeregMmcRelReq is enter");

    /* ֹͣATTACH��ʱ�� */
    NAS_LMM_StopStateTimer(TI_NAS_EMM_T3410);
    NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_T3411);
    NAS_LMM_StopStateTimer(TI_NAS_EMM_WAIT_ESM_BEARER_CNF);
    NAS_LMM_StopStateTimer(TI_NAS_EMM_WAIT_ESM_PDN_RSP);
    NAS_LMM_StopStateTimer(TI_NAS_EMM_WAIT_RRC_DATA_CNF);
    /*NAS_LMM_StopStateTimer(TI_NAS_EMM_T3421);*/
    NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_T3416);

    /* ��EMM״̬ת����MS_DEREG + SS_PLMN_SEARCH״̬ */
    EmmState.enFsmId                    = NAS_LMM_PARALLEL_FSM_EMM;
    EmmState.enMainState                = EMM_MS_DEREG;
    EmmState.enSubState                 = EMM_SS_DEREG_PLMN_SEARCH;
    EmmState.enStaTId                   = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    /*��ESM����ID_EMM_ESM_STATUS_IND��Ϣ*/
    NAS_EMM_PUB_SendEsmStatusInd(       EMM_ESM_ATTACH_STATUS_DETACHED);

    /*��MMC�ϱ�attach���*/
    NAS_EMM_AppSendAttOtherType(MMC_LMM_ATT_RSLT_FAILURE);

    /*��RRC����RRC_MM_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);

    return;
}


VOS_VOID  NAS_EMM_ProcDetachMmcRelReq(VOS_VOID )
{
    NAS_EMM_FSM_STATE_STRU              EmmState;

    NAS_EMM_GIM_NORMAL_LOG("NAS_EMM_ProcDetachMmcRelReq is enter");

    /* ֹͣdetach��ʱ�� */
    NAS_LMM_StopStateTimer(TI_NAS_EMM_T3421);
    /* lihong00150010 emergency tau&service begin */
    if (VOS_TRUE == NAS_EMM_GLO_AD_GetUsimPullOutFlag())
    {
        /* ��EMM״̬ת����MS_DEREG + SS_PLMN_SEARCH״̬ */
        EmmState.enFsmId                    = NAS_LMM_PARALLEL_FSM_EMM;
        EmmState.enMainState                = EMM_MS_DEREG;
        EmmState.enSubState                 = EMM_SS_DEREG_NO_IMSI;
        EmmState.enStaTId                   = TI_NAS_EMM_STATE_NO_TIMER;
        NAS_LMM_StaTransProc(EmmState);

        /* ��MMC�ظ�LMM_MMC_USIM_STATUS_CNF */
        NAS_EMM_SendMmcUsimStatusCnf();

        /* ����ο���ʶ */
        NAS_EMM_GLO_AD_GetUsimPullOutFlag() = VOS_FALSE;
    }/* lihong00150010 emergency tau&service end */
    else
    {
        /* ��EMM״̬ת����MS_DEREG + SS_PLMN_SEARCH״̬ */
        EmmState.enFsmId                    = NAS_LMM_PARALLEL_FSM_EMM;
        EmmState.enMainState                = EMM_MS_DEREG;
        EmmState.enSubState                 = EMM_SS_DEREG_NORMAL_SERVICE;
        EmmState.enStaTId                   = TI_NAS_EMM_STATE_NO_TIMER;
        NAS_LMM_StaTransProc(EmmState);
    }

    /*��ESM����ID_EMM_ESM_STATUS_IND��Ϣ*/
    NAS_EMM_PUB_SendEsmStatusInd(       EMM_ESM_ATTACH_STATUS_DETACHED);

    NAS_EMM_SendDetRslt(MMC_LMM_DETACH_RSLT_SUCCESS);

    /* ����DETACH�ͷ���Դ:��̬�ڴ桢����ֵ */
    NAS_LMM_DeregReleaseResource();

    /*��RRC����RRC_MM_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);

    return;
}





VOS_VOID  NAS_EMM_ProcRegMmcRelReq(VOS_VOID )
{
    NAS_EMM_GIM_NORMAL_LOG("NAS_EMM_ProcRegMmcRelReq is enter");

    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_TAU_T3430);
    /*NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_T3411);*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417);
    /*NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_T3416);*/

    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

    /* �ͷŻ����ESM��Ϣ*/
    NAS_EMM_SerClearEsmDataBuf();

    /* ��ս���Bar��Ϣ*/
    NAS_EMM_ClearBarResouce();


    /* ��״̬ת����MS_REG + SS_REG_PLMN_SEARCH״̬ */
    /*
    EmmState.enFsmId                    = NAS_LMM_PARALLEL_FSM_EMM;
    EmmState.enMainState                = EMM_MS_REG;
    EmmState.enSubState                 = EMM_SS_REG_PLMN_SEARCH;
    EmmState.enStaTId                   = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);
    */
    return;
}



VOS_VOID NAS_EMM_ProcRegMmcRelReqWhileTauInit(VOS_VOID)
{
    NAS_LMM_PUBM_LOG_NORM("NAS_EMM_ProcRegMmcRelReqWhileTauInit");

    /*�ͷŴ���*/
    NAS_EMM_ProcRegMmcRelReq();

    /*����TAU�쳣������������*/
    NAS_EMM_TAU_GetEmmTAUAttemptCnt()++;
    NAS_EMM_TAU_ProcAbnormal();
    /* ��MMC����LMM_MMC_TAU_RESULT_IND��Ϣ�ڳ�ͻ�ڲ����� */
    NAS_EMM_TAU_RelIndCollisionProc(NAS_EMM_MmcSendTauActionResultIndFailWithPara,
                                    (VOS_VOID*)NAS_EMM_NULL_PTR);

    /*��RRC����RRC_MM_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
}



VOS_VOID NAS_EMM_ProcRegMmcRelReqWhileSerInit(VOS_VOID)
{
    NAS_LMM_PUBM_LOG_NORM("NAS_EMM_ProcRegMmcRelReqWhileSerInit");

    /*�ͷŴ���*/
    NAS_EMM_ProcRegMmcRelReq();

    /*ת��״̬*/
    NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm();

    /*��RRC����RRC_MM_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
}
VOS_VOID NAS_EMM_ProcRegMmcRelReqWhileImsiDetach(VOS_VOID)
{
    NAS_LMM_PUBM_LOG_NORM("NAS_EMM_ProcRegMmcRelReqWhileImsiDetach");

    /*��IMSI detach���д���*/
    NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_PS);
    NAS_EMM_SendDetRslt(MMC_LMM_DETACH_RSLT_SUCCESS);
    NAS_LMM_ImsiDetachReleaseResource();

    /*����RRC���������ͷ�*/
    NAS_EMM_ProcRegMmcRelReq();

    /*ת��״̬*/
    NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm();

    /*��RRC����RRC_MM_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);

}
/*lint +e961*/
/*lint +e960*/




/***************************************************************************/

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
/* end of ��ϵͳ+ģ��+�ļ���.c */
