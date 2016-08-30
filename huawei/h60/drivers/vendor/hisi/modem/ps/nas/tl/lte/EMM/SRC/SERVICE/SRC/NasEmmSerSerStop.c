

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "NasEmmTauSerInclude.h"
#include "NasLmmPubMStack.h"

#include "NasEmmPubUSuspend.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASEMMSERVICESERSTOP_C
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




VOS_UINT32 NAS_EMM_MsSerInitMsgRabmDrbSetupInd(VOS_UINT32  ulMsgId,
                                                   VOS_VOID   *pMsgStru)
{
    VOS_UINT32                      ulRslt                  = NAS_EMM_FAIL;

    (VOS_VOID)ulMsgId;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsSerInitMsgRabmDrbSetupInd is entered.");

    /* ��������ָ��������, ״̬ƥ����,����ƥ��,�˳�*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_SER_INIT,EMM_SS_SER_WAIT_CN_SER_CNF,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        /* ��ӡ�쳣 */
        NAS_EMM_SER_LOG_WARN("NAS_EMM_TAUSER_CHKFSMStateMsgp ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*������Ϣ������*/
    NAS_EMM_SER_RcvRabmDrbSetupInd(pMsgStru);

    return NAS_LMM_MSG_HANDLED;
}


VOS_VOID NAS_EMM_SER_SndEsmBufMsg( VOS_VOID )
{
    EMM_ESM_DATA_REQ_STRU                *pstMsg = NAS_LMM_NULL_PTR;
    VOS_UINT32                            i      = 0;
    VOS_UINT32                            ulRslt = 0;
    VOS_UINT32                            ulCurEmmStat;

    /* lihong00150010 emergency tau&service begin */
    ulCurEmmStat = NAS_LMM_PUB_COMP_EMMSTATE(NAS_EMM_CUR_MAIN_STAT, NAS_EMM_CUR_SUB_STAT);
    if ((ulCurEmmStat != NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_REG, EMM_SS_REG_NORMAL_SERVICE))
        && (ulCurEmmStat != NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_REG, EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM)))
    {
        NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SndEsmBufMsg:state is not allowed to send msg.");

        NAS_EMM_SerClearEsmDataBuf();
        return ;
    }
    /* lihong00150010 emergency tau&service end */

    for (i = 0; i < g_stEmmEsmMsgBuf.ulEsmMsgCnt; i++)
    {
       pstMsg = (EMM_ESM_DATA_REQ_STRU *)g_stEmmEsmMsgBuf.apucEsmMsgBuf[i];

       /*��ESM��Ϣ*/
       NAS_EMM_SER_SendMrrcDataReq_ESMdata(&pstMsg->stEsmMsg);
       ulRslt = NAS_COMM_FreeBuffItem(NAS_COMM_BUFF_TYPE_EMM, pstMsg);

       if (NAS_COMM_BUFF_SUCCESS != ulRslt)
       {
           NAS_EMM_SER_LOG_WARN("NAS_EMM_SER_RcvRabmDrbSetupInd, Memory Free is not succ");
       }

       g_stEmmEsmMsgBuf.apucEsmMsgBuf[i] = NAS_LMM_NULL_PTR;
    }

    g_stEmmEsmMsgBuf.ulEsmMsgCnt = 0;
}
VOS_VOID    NAS_EMM_SER_RcvRabmDrbSetupInd(const VOS_VOID   *pMsgStru)
{
    (VOS_VOID)pMsgStru;

    /* Э��涨EXTENDED SERVICE���̳ɹ���ֹ�ı�ʶ����ϵͳ�任��������DRB���� */
    if (VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
    {
        /*ת����ȫ������״̬*/
        NAS_EMM_SecuStateConvert(   EMM_CUR_SECU_EXIST_ACTIVE,
                                    NAS_LMM_GetSecuCurFsmNS());

        return ;
    }
    /*ֹͣ��ʱ��3417*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417);

    /*ת����ȫ������״̬*/
    NAS_EMM_SecuStateConvert(   EMM_CUR_SECU_EXIST_ACTIVE,
                                NAS_LMM_GetSecuCurFsmNS());

    /*����SER�Ľ��ΪNAS_EMM_SER_RESULT_SUCC*/
    /*NAS_EMM_SER_SaveSERresult(NAS_EMM_SER_RESULT_SUCC);*/
	/* lihong00150010 emergency tau&service begin */
    /*SR���̳ɹ������SR����������SMS��������Ҫ�ظ�SMS�����ɹ�*/
    if(NAS_EMM_SER_START_CAUSE_SMS_EST_REQ == NAS_EMM_SER_GetSerStartCause())
    {
        NAS_LMM_SndLmmSmsEstCnf();
        NAS_LMM_SetConnectionClientId(NAS_LMM_CONNECTION_CLIENT_ID_SMS);
    }

    NAS_EMM_TranStatePsNormalServiceOrPsLimitService();

    /* ���ͻ����ESM��Ϣ*/
    NAS_EMM_SER_SndEsmBufMsg();
    
    NAS_EMM_SER_ClearResource();

    /* ���RRC��·�ͷŴ���TAU��ԭ�� */
    NAS_LMM_SetEmmInfoTriggerTauRrcRel(NAS_EMM_TRIGGER_TAU_RRC_REL_NO);
    NAS_LMM_SetEmmInfoT3412ExpCtrl(NAS_EMM_T3412_EXP_NO);

	/* lihong00150010 emergency tau&service end */
    /*ת��EMM״̬��MS_REG+SS_REG_NORMAL_SERVIC*/
    /*NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_NORMAL_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);*/

    /*����в������������߳��ظ����仯��TAUδ��ɣ��򴥷���TAU
      ע:�ó���һ��������̬�´���TAU������Ҫ�ж���������*/
    if (NAS_EMM_YES == NAS_EMM_SER_SuccNeedtoContinueTau())
    {
        NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_RcvRabmDrbSetupInd: Need to Continue Tau");

        if(NAS_LMM_TIMER_STOPED == NAS_LMM_IsPtlTimerRunning(TI_NAS_EMM_PTL_T3411))
        {
            NAS_EMM_TAU_StartTAUREQ();
        }
    }
    else
    {
        if (NAS_EMM_YES != NAS_EMM_TAU_CanTriggerComTauWithIMSI())
        {
            /*���TAU��Դ��ע��:��NAS_EMM_SER_SndEsmBufMsg��ִ��˳���ܵߵ�*/
            NAS_EMM_TAU_ClearResouce();

        }
        #if 0
        if (NAS_EMM_UPDATE_MM_FLAG_VALID != NAS_LMM_GetEmmInfoUpdateMmFlag())
        {
            /*���TAU��Դ��ע��:��NAS_EMM_SER_SndEsmBufMsg��ִ��˳���ܵߵ�*/
            NAS_EMM_TAU_ClearResouce();
        }
        #endif
    }


    return;
}


/*lint -e960*/
/*lint -e961*/
VOS_UINT32  NAS_EMM_PreProcMsgMmCsfbSerAbortNotify( MsgBlock * pMsg )
{
    VOS_UINT32                          ulCurEmmStat    = NAS_EMM_NULL;
    VOS_UINT32                          ulStaAtStackBase= NAS_EMM_NULL;

    NAS_LMM_PUBM_LOG_INFO("NAS_EMM_PreProcMsgMmCsfbSerAbortNotify is entered");

    (VOS_VOID)pMsg;

    /* ��¼CSFB ABORT��ʶ */
    NAS_EMM_SER_SaveEmmSerCsfbAbortFlag(NAS_EMM_CSFB_ABORT_FLAG_VALID);

    /* ���CSFBʱ�Ӷ�ʱ�������У���ֹCSFB���̣�����Ѿ�����TAU��SER�����������ʱ��ABORT��ʶ���д��� */
    NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_CSFB_DELAY);

    ulCurEmmStat = NAS_LMM_PUB_COMP_EMMSTATE(   NAS_EMM_CUR_MAIN_STAT,
                                                NAS_EMM_CUR_SUB_STAT);


    /* ���LMM��ǰ״̬Ϊ����������ʱ�����ȡ�Ѿ�ѹջ��״̬������Ѿ�ѹջ��״̬Ϊ
       ��SERVICE_INIT+WAIT_SER_CN_CNF������CSFB����ʱ����ջ����ֹCSFB���̣�
       ��RRC����·�ͷ���������������¼ABORT��ʶ */
    if (ulCurEmmStat == NAS_LMM_PUB_COMP_EMMSTATE(  EMM_MS_RRC_CONN_EST_INIT,
                                                    EMM_SS_RRC_CONN_WAIT_EST_CNF))
    {

        NAS_LMM_PUBM_LOG_INFO("NAS_EMM_PreProcMsgMmCsfbSerAbortNotify: esting.");

        ulStaAtStackBase = NAS_LMM_FSM_GetStaAtStackBase(NAS_LMM_PARALLEL_FSM_EMM);
        if ((ulStaAtStackBase == NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_SER_INIT,EMM_SS_SER_WAIT_CN_SER_CNF))
            && (VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure()))
        {
            NAS_LMM_PUBM_LOG1_INFO("NAS_EMM_PreProcMsgMmCsfbSerAbortNotify: SubState =",
                                EMM_SS_SER_WAIT_CN_SER_CNF);
            /* ��ջ */
            NAS_EMM_FSM_PopState();

            NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

            NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                        EMM_SS_REG_NORMAL_SERVICE,
                                        TI_NAS_EMM_STATE_NO_TIMER);

            /* ����RRC��·�ͷ� */
            NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
        }
    }

    /* ��״̬�� EMM_MS_SUSPEND + EMM_SS_SUSPEND_RRCORI_WAIT_OTHER_SUSPEND */
    else if (ulCurEmmStat ==
                NAS_LMM_PUB_COMP_EMMSTATE(  EMM_MS_SUSPEND,
                                            EMM_SS_SUSPEND_RRCORI_WAIT_OTHER_SUSPEND))
    {
        NAS_LMM_PUBM_LOG_INFO("NAS_EMM_PreProcMsgMmCsfbSerAbortNotify: WAIT_OTHER_SUSPEND.");

        /* ֹͣ��ʱ�� */
        NAS_LMM_StopStateTimer(TI_NAS_EMM_RRCORI_WAIT_OTHER_SUSPEND_RSP_TIMER);

        /* ����˴ι�����ϲ���Ӧ��¼���ָ����̽�ʹ�� */
        NAS_EMM_GetUplayerCount() = NAS_EMM_SUSPEND_UPLAYER_NUM_INITVALUE;

        /*����LRRC_LMM_SUSPEND_REL_REQ��Ϣ*/
        NAS_EMM_SndLrrcSuspendRelReq();


        NAS_EMM_SetSuspendRelCause(NAS_EMM_SUSPEND_REL_CAUSE_ABORT_CSFB);
        NAS_LMM_PUBM_LOG1_INFO("NAS_EMM_PreProcMsgMmCsfbSerAbortNotify: SuspendRelCause =",
                                NAS_EMM_SUSPEND_REL_CAUSE_ABORT_CSFB);

        /* ״̬Ǩ��*/
        NAS_EMM_PUBU_FSMTranState(      EMM_MS_SUSPEND,
                                        EMM_SS_SUSPEND_WAIT_END,
                                        TI_NAS_EMM_WAIT_SUSPEND_END_TIMER);

    }

    /* ��״̬�� EMM_MS_SUSPEND + EMM_SS_SUSPEND_WAIT_END */
    else if (ulCurEmmStat == NAS_LMM_PUB_COMP_EMMSTATE(  EMM_MS_SUSPEND,
                                                    EMM_SS_SUSPEND_WAIT_END))
    {
        NAS_LMM_PUBM_LOG_INFO("NAS_EMM_PreProcMsgMmCsfbSerAbortNotify: SUSPEND_WAIT_END.");

        /*����LRRC_LMM_SUSPEND_REL_REQ��Ϣ*/
        NAS_EMM_SndLrrcSuspendRelReq();

        NAS_EMM_SetSuspendRelCause(NAS_EMM_SUSPEND_REL_CAUSE_ABORT_CSFB);
        NAS_LMM_PUBM_LOG1_INFO("NAS_EMM_PreProcMsgMmCsfbSerAbortNotify: SuspendRelCause =",
                                NAS_EMM_SUSPEND_REL_CAUSE_ABORT_CSFB);


    }
    else
    {
        NAS_LMM_PUBM_LOG_INFO("NAS_EMM_PreProcMsgMmCsfbSerAbortNotify: no other action.");
    }

    return NAS_LMM_MSG_HANDLED;
}
/*lint +e961*/
/*lint +e960*/


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif




