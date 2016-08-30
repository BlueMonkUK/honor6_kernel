

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "NasEmmTauSerInclude.h"
#include "NasEmmAttDetInclude.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASEMMTAUABNORMAL_C
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

NAS_EMM_BAR_STRU                        g_stEmmRegStateBarInfo;

static NAS_EMM_GRANT_ACT_STRU g_astGrantActTable[] =
{
    {LRRC_LNAS_ACCESS_GRANT_MT,                                 0xfb},
    {LRRC_LNAS_ACCESS_GRANT_MO_CALL,                            0xfd},
    {LRRC_LNAS_ACCESS_GRANT_MO_SIGNAL,                          0xfe},
    {LRRC_LNAS_ACCESS_GRANT_MO_CALL_AND_MT,                     0xf9},
    {LRRC_LNAS_ACCESS_GRANT_MO_SIGNAL_AND_MT,                   0xfa},
    {LRRC_LNAS_ACCESS_GRANT_ALL,                                0xf0},
    {LRRC_LNAS_ACCESS_GRANT_MO_CALL_AND_MO_SIGNAL,              0xfc},
    {LRRC_LNAS_ACCESS_GRANT_MO_CALL_AND_MO_SIGNAL_AND_MT,       0xf8},
    {LRRC_LNAS_ACCESS_GRANT_MO_CSFB,                            0xf7},
    {LRRC_LNAS_ACCESS_GRANT_MO_CALL_AND_MO_CSFB,                0xf5},
    {LRRC_LNAS_ACCESS_GRANT_MO_SIGNAL_AND_MO_CSFB,              0xf6},
    {LRRC_LNAS_ACCESS_GRANT_MO_CALL_AND_MO_SIGNAL_AND_MO_CSFB,  0xf4},
    {LRRC_LNAS_ACCESS_GRANT_MO_CSFB_AND_MT,                     0xf3},
    {LRRC_LNAS_ACCESS_GRANT_MO_CALL_AND_MO_CSFB_AND_MT,         0xf1},
    {LRRC_LNAS_ACCESS_GRANT_MO_SIGNAL_AND_MO_CSFB_AND_MT,       0xf2},

};

VOS_UINT32 ulGrantTypeNum = sizeof(g_astGrantActTable) / sizeof(NAS_EMM_GRANT_ACT_STRU);



/*lint -e960*/
/*lint -e961*/
VOS_UINT32  NAS_EMM_CheckEnterRegNormal(VOS_VOID)
{
    VOS_UINT32                          ulRslt = NAS_EMM_FAIL;
    MMC_LMM_TIN_TYPE_ENUM_UINT32        ulTinType = MMC_LMM_TIN_INVALID;

    ulRslt = NAS_EMM_CheckMutiModeSupport();
    ulTinType = NAS_EMM_GetTinType();

    NAS_LMM_PUBM_LOG1_NORM("NAS_EMM_TIN=", ulTinType);
    NAS_LMM_PUBM_LOG1_NORM("NAS_EMM_UPDATE_STATE=",(NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat));

    /* �����DRX�ı䷢���TAUʧ�ܣ����ܽ���NOR-SERVICE̬�����޸�EUֵ */
    if (NAS_EMM_YES == NAS_LMM_GetEmmInfoDrxNetCapChange())
    {
       return NAS_EMM_FAIL;
    }

    /* �����UE���������ı䷢���TAUʧ�ܣ����ܽ���NOR-SERVICE̬�����޸�EUֵ */
    if (NAS_LMM_UE_RADIO_CAP_NOT_CHG != NAS_LMM_GetEmmInfoUeRadioCapChgFlag())
    {
        return NAS_EMM_FAIL;
    }

    /* ��ϵͳ�任��ʶ��Ч������GPRS��������, ���ܽ���NORMAL-SERVICE̬�����޸�EUֵ*/
    if (   (NAS_EMM_YES == NAS_LMM_GetEmmInfoTriggerTauSysChange())
        && (GMM_LMM_GPRS_SUSPENSION == NAS_LMM_GetEmmInfoPsState()) )
    {
        return NAS_EMM_FAIL;
    }

    /* ��������������в��ܽ�������� */
    /* TA��TAI LIST && EUֵΪEU1 && L��ģ���߶�ģ��TIN����P-TMSI���ɽ��� */
    if(   (NAS_EMM_SUCC == NAS_EMM_TAU_IsCurrentTAInTaList())
        && (EMM_US_UPDATED_EU1 == (NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat))
        && ((NAS_EMM_FAIL == ulRslt)
                || (    (NAS_EMM_SUCC == ulRslt)
                     && (MMC_LMM_TIN_P_TMSI != ulTinType))))
    {
        return NAS_EMM_SUCC;
    }
    return NAS_EMM_FAIL;

}
VOS_VOID    NAS_EMM_TAU_RcvT3440Exp()
{

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO( "Nas_Emm_Tau_RcvT3440Exp is entered.");

    /*��MRRC����NAS_EMM_MRRC_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);

    return;
}


VOS_VOID    NAS_EMM_TAU_RcvT3430Exp()
{
    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO( "Nas_Emm_Tau_RcvT3430Exp is entered.");

    NAS_EMM_TAU_ProcAbnormal();

    /* lihong00150010 emergency tau&service begin */
    if (NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_RcvT3430Exp:CAUSE_ESM_EMC_PDN_REQ");

        NAS_EMM_EmcPndReqTauAbnormalCommProc(   NAS_EMM_MmcSendTauActionResultIndTimerExp,
                                                (VOS_VOID*)NAS_EMM_NULL_PTR,
                                                EMM_SS_DEREG_NORMAL_SERVICE);

        /*��MRRC����NAS_EMM_MRRC_REL_REQ��Ϣ*/
        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);

        return;
    }
    /* lihong00150010 emergency tau&service end */

    /****����ͻ��־������Ӧ�Ĵ���********************/
    NAS_EMM_TAU_RelIndCollisionProc(NAS_EMM_MmcSendTauActionResultIndTimerExp,
                    (VOS_VOID*)NAS_EMM_NULL_PTR);

    #if 0

    /*���T3402��ʱ�����У������ͻ��־*/
    if(NAS_LMM_TIMER_RUNNING == NAS_LMM_IsPtlTimerRunning(TI_NAS_EMM_PTL_T3402))
    {
        /****����ͻ��־������Ӧ�Ĵ���********************/
        NAS_EMM_TAU_RelIndCollisionProc(NAS_EMM_MmcSendTauActionResultIndTimerExp,
                    (VOS_VOID*)NAS_EMM_NULL_PTR);

        /*NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);*/
    }
    else
    {
        NAS_EMM_MmcSendTauActionResultIndTimerExp((VOS_VOID*)NAS_EMM_NULL_PTR);
    }
    #endif

    /*��MRRC����NAS_EMM_MRRC_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);

    return;

}
VOS_VOID  NAS_EMM_TAU_ProcAbnormal(VOS_VOID)
{
    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO( "NAS_EMM_TAU_ProcAbnormal is entered.");

    /* TAU���Դ���С��5 */
    if(NAS_EMM_TAU_ATTEMPT_CNT_MAX > NAS_EMM_TAU_GetEmmTAUAttemptCnt())
    {
        if(NAS_EMM_SUCC == NAS_EMM_CheckEnterRegNormal())
        {
            /*������ʱ��T3411*/
            NAS_LMM_StartPtlTimer(TI_NAS_EMM_PTL_T3411);
			/* lihong00150010 emergency tau&service begin */
            NAS_EMM_TranStatePsNormalServiceOrPsLimitService();
            /* lihong00150010 emergency tau&service end */
            /*ת��EMM״̬��MS_REG+EMM_SS_REG_NORMAL_SERVICE*/
            /*NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_NORMAL_SERVICE, TI_NAS_EMM_PTL_T3411);*/

            return;
        }

        /* ���Դ���С��5���ǲ��ܽ���NORMAL SERVICE̬ */
        /*������ʱ��T3411*/
        NAS_LMM_StartPtlTimer(TI_NAS_EMM_PTL_T3411);

        /*����EPS״̬ΪEU2*/
        NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_NOT_UPDATED_EU2;

        /* lihong00150010 emergency tau&service begin */
        if (NAS_LMM_REG_STATUS_EMC_REGED != NAS_LMM_GetEmmInfoRegStatus())
        {
            /*����PS LOC��Ϣ*/
            NAS_LMM_WritePsLoc(NAS_NV_ITEM_UPDATE);
        }
        /* lihong00150010 emergency tau&service end */

        /*ת��EMM״̬��MS_REG+SS_NORMAL_SERVICE*/
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_ATTEMPTING_TO_UPDATE, TI_NAS_EMM_STATE_NO_TIMER);

        return;
    }

    /* TAU���Դ�������5 */

    /* ������T3402ʱ���ж�PLMN �Ƿ��� EPLMN LIST�У�������ڣ�������ΪĬ��ֵ */
    if (VOS_TRUE== NAS_LMM_CheckModifyT3402DefaultValue())
    {
        /*T3402����ΪĬ��ֵ*/
        NAS_LMM_ModifyPtlTimer(TI_NAS_EMM_PTL_T3402,NAS_LMM_TIMER_T3402_LEN);
    }

    /*������ʱ��T3402*/
    NAS_LMM_StartPtlTimer(TI_NAS_EMM_PTL_T3402);

    /*����EPS״̬ΪEU2*/
    NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_NOT_UPDATED_EU2;

    /* lihong00150010 emergency tau&service begin */
    if (NAS_LMM_REG_STATUS_EMC_REGED != NAS_LMM_GetEmmInfoRegStatus())
    {
        /*����PS LOC��Ϣ*/
        NAS_LMM_WritePsLoc(NAS_NV_ITEM_UPDATE);
    }
    /* lihong00150010 emergency tau&service end */

    /*�����ǰTAUΪ�������ͣ��޸�TAU����ԭ��ֵ*/
    if (NAS_EMM_TAU_START_CAUSE_T3412EXP == NAS_EMM_TAU_GetEmmTAUStartCause())
    {
        NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
    }

    /*ת��EMM״̬��MS_REG+SS_ATTEMPT_TO_UPDATE*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_ATTEMPTING_TO_UPDATE, TI_NAS_EMM_STATE_NO_TIMER);

    return;
}



VOS_VOID  NAS_EMM_TAU_ProcCause161722TauAttemptCont(VOS_VOID )
{
    NAS_EMM_ATTACH_LOG_INFO("NAS_EMM_ProcCause161722TauAttemptCont is entered");
    /* lihong00150010 emergency tau&service begin */
    if (VOS_TRUE == NAS_EMM_IsEnterRegLimitService())
    {
        NAS_EMM_AdStateConvert(         EMM_MS_REG,
                                        EMM_SS_REG_LIMITED_SERVICE,
                                        TI_NAS_EMM_STATE_NO_TIMER);

        NAS_EMM_MmcSendTauActionResultIndSucc();

        return;
    }
    /* lihong00150010 emergency tau&service end */

    if (NAS_EMM_TAU_ATTEMPT_CNT_MAX > NAS_EMM_TAU_GetEmmTAUAttemptCnt())
    {
        /*������ʱ��TI_NAS_EMM_PTL_T3411*/
        NAS_LMM_StartPtlTimer(TI_NAS_EMM_PTL_T3411);

		/* lihong00150010 emergency delete */
		
        /*�޸�״̬��������״̬REG��״̬ATTACH_WAIT_ESM_BEARER_CNF*/
        NAS_EMM_AdStateConvert(         EMM_MS_REG,
                                        EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM,
                                        TI_NAS_EMM_STATE_NO_TIMER);

        NAS_EMM_MmcSendTauActionResultIndSucc();

    }
    else
    {
        /*������ʱ��TI_NAS_EMM_T3402*/
        NAS_LMM_StartPtlTimer(      TI_NAS_EMM_PTL_T3402);
		
		/* lihong00150010 emergency delete */

        /*�޸�״̬��������״̬REG��״̬EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM*/
        NAS_EMM_AdStateConvert(     EMM_MS_REG,
                                    EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM,
                                    TI_NAS_EMM_PTL_T3402);

        NAS_EMM_MmcSendTauActionResultIndSucc();

        /*NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);*/

        /* �����NAS_EMM_TAU_ProcCause161722TauAttemptCont�����ĳ�������IDLE����
           ��TAU������CS/PS1�ĳ������Զ��ȴ��������ͷ���·������LMM�����ͷ���· */
    }

    return;
}


VOS_VOID  NAS_EMM_TAU_ProcEnterForbTA
(
    EMMC_EMM_FORBIDDEN_INFO_ENUM_UINT32 ulForbInfo
)
{

    /* ����Collison��־���� ��ͬ����*/
    switch(NAS_EMM_TAU_GetEmmCollisionCtrl())
    {
        case    NAS_EMM_COLLISION_DETACH   :

                /*��MMC����LMM_MMC_TAU_RESULT_IND��Ϣ*/
                NAS_EMM_MmcSendTauActionResultIndForbType((VOS_VOID*)&ulForbInfo);

                /* �����IMSI DETACH���򱾵�CS��ȥע�ᣬ����REG+LIMIT_SERVICE;
                   ���򱾵�PS��ȥע�ᣬ����DEREG+LIMMIT_SERVICE */
                if (MMC_LMM_MO_DET_CS_ONLY != NAS_EMM_GLO_AD_GetDetTypeMo())
                {
                    NAS_EMM_TAU_LOG_NORM( "NAS_EMM_TAU_ProcEnterForbTA : Process EPS Detach or EPS/IMSI Detach.");

                    /*�޸�״̬��������״̬DEREG��״̬DEREG_NORMAL_SERVICE*/
                    NAS_EMM_AdStateConvert(     EMM_MS_DEREG,
                                                EMM_SS_DEREG_LIMITED_SERVICE,
                                                TI_NAS_EMM_STATE_NO_TIMER);

                    NAS_EMM_TAU_CollisionDetachProc();

                    /* ����DETACH*/
                    NAS_LMM_DeregReleaseResource();
                }
                else
                {
                    NAS_EMM_TAU_LOG_NORM( "NAS_EMM_TAU_ProcEnterForbTA : Process Imsi Detach.");

                    /* ����ע����ΪPS */
                    NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_PS);

                    NAS_EMM_TAU_CollisionDetachProc();

                    /* IMSI DETACH���ͷ���Դ ,������ڸ�APP�ϱ����֮�󣬷���DETACH TYPE
                    �᲻��ȷ*/
                    NAS_LMM_ImsiDetachReleaseResource();
                }

                break;

        case    NAS_EMM_COLLISION_SERVICE    :

                /* ��MMC����LMM_MMC_TAU_RESULT_IND��Ϣ�ŵ���ͻ�����д��� */
                NAS_EMM_TAU_CollisionServiceProc(NAS_EMM_MmcSendTauActionResultIndForbType,
                                (VOS_VOID*)&ulForbInfo,
                                NAS_EMM_TRANSFER_RAT_ALLOW);
                break;

        default :
                NAS_EMM_TAU_LOG_INFO( "NAS_EMM_TAU_ProcEnterForbTA : NO Collision.");

                /*��MMC����LMM_MMC_TAU_RESULT_IND��Ϣ*/
                NAS_EMM_MmcSendTauActionResultIndForbType((VOS_VOID*)&ulForbInfo);
                break;
    }

    return;
}


VOS_VOID    NAS_EMM_TAU_MsRegMsgRelInd( LRRC_LNAS_REL_CAUSE_ENUM_UINT32   enRelCause )
{


    if(LRRC_LNAS_REL_CAUSE_LOAD_BALANCE_REQ         ==    enRelCause)
    {
        /*NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_RRC_REL_LOAD_BALANCE);*/
        NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
        NAS_LMM_SetEmmInfoTriggerTauRrcRel(NAS_EMM_TRIGGER_TAU_RRC_REL_LOAD_BALANCE);
    }
    else if(LRRC_LNAS_REL_CAUSE_CONN_FAIL           ==    enRelCause)
    {
        /* ����ϴ�TAU�ɹ������¸�ֵ
        */
        if(NAS_EMM_TRIGGER_TAU_RRC_REL_NO        ==  NAS_LMM_GetEmmInfoTriggerTauRrcRel())
        {
            /*NAS_EMM_TAU_SaveEmmTAUStartCause(     NAS_EMM_TAU_START_CAUSE_RRC_REL_CONN_FAILURE);*/
            NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
            NAS_LMM_SetEmmInfoTriggerTauRrcRel(NAS_EMM_TRIGGER_TAU_RRC_REL_CONN_FAILURE);
        }
    }
    else
    {
        /* ����ԭ�򲻻ᵼ��TAU�������� */;
        NAS_EMM_TAU_LOG1_INFO("NAS_EMM_TAU_MsRegMsgRelInd: enRelCause = ", enRelCause);

        return;
    }

    /* �ж��Ƿ��������Pending*/
    NAS_EMM_TAU_IsUplinkPending();

    NAS_EMM_TAU_StartTAUREQ();

    return;
}
VOS_UINT32 NAS_EMM_MsTauInitSsWaitCNCnfMsgT3430Exp(VOS_UINT32  ulMsgId,
                                                   VOS_VOID   *pMsgStru
                               )
{
    VOS_UINT32                                  ulRslt              = NAS_EMM_FAIL;

    (VOS_VOID)ulMsgId;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_NORM("Nas_Emm_MsTauInitSsWaitCNCnfMsgT3430Exp is entered.");

    /* ��������ָ��������, ״̬ƥ����,����ƥ��,�˳�*/
    ulRslt = NAS_EMM_TAU_CHKFSMStateMsgp(EMM_MS_TAU_INIT,EMM_SS_TAU_WAIT_CN_TAU_CNF,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        /* ��ӡ�쳣 */
        NAS_EMM_TAU_LOG_WARN( "NAS_EMM_TAUSER_CHKFSMStateMsgp ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*TAU ATTEMPT COUNT ++*/
    NAS_EMM_TAU_GetEmmTAUAttemptCnt()++;

    /*������Ϣ������*/
    NAS_EMM_TAU_RcvT3430Exp();

    return NAS_LMM_MSG_HANDLED;
}
VOS_VOID  NAS_EMM_MsTauInitSsWaitCNCnfProcMsgRrcRelInd( VOS_UINT32 ulCause )
{

    NAS_EMM_TAU_LOG_INFO(                         "NAS_EMM_MsTauInitSsWaitCNCnfProcMsgRrcRelInd is entered.");

    /*common process*/
    NAS_LMM_StopStateTimer(                        TI_NAS_EMM_STATE_TAU_T3430);
    /* lihong00150010 emergency tau&service begin */
    if (NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_MsTauInitSsWaitCNCnfProcMsgRrcRelInd:CAUSE_ESM_EMC_PDN_REQ");

        NAS_EMM_EmcPndReqTauAbnormalCommProc(   NAS_EMM_MmcSendTauActionResultIndFailWithPara,
                                                (VOS_VOID*)NAS_EMM_NULL_PTR,
                                                EMM_SS_DEREG_NORMAL_SERVICE);

        NAS_EMM_CommProcConn2Ilde();

        return;
    }
    /* lihong00150010 emergency tau&service end */
    NAS_EMM_TAU_GetEmmTAUAttemptCnt()++;

    /*������DETACH_COLLISION��־������ݲ�ͬ�ͷ�ԭ��ֵ����*/
    switch(ulCause)
    {
        /* RRC����IDLE̬�����´���TAU*/
        case LRRC_LNAS_REL_CAUSE_NAS_DATA_ABSENT:
            NAS_EMM_TAU_LOG_INFO("NAS_EMM_MsTauInitSsWaitCNCnfProcMsgRrcRelInd:DATA ABSENT.");

            /*send INTRA_CONN2IDLE_REQ����������״̬*/
            NAS_EMM_CommProcConn2Ilde();

            /* �ж��Ƿ��������Pending*/
            NAS_EMM_TAU_IsUplinkPending();

            /* ����TAU*/
            NAS_EMM_TAU_StartTAUREQ();
            return;

        default:

            NAS_EMM_TAU_ProcAbnormal();

            /* ��MMC����LMM_MMC_TAU_RESULT_IND��Ϣ�ڳ�ͻ�ڲ����� */
            NAS_EMM_TAU_RelIndCollisionProc(NAS_EMM_MmcSendTauActionResultIndFailWithPara,
                               (VOS_VOID*)NAS_EMM_NULL_PTR);
            #if 0
            /* ��������쳣������3402�����TAU����Counter */
            if(NAS_LMM_TIMER_RUNNING == NAS_LMM_IsPtlTimerRunning(TI_NAS_EMM_PTL_T3402))
            {
                /*��� Tau Attempt Counter*/
                NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);
            }
            #endif
            break;
    }

    /*send INTRA_CONN2IDLE_REQ����������״̬*/
    NAS_EMM_CommProcConn2Ilde();

    return;
}
VOS_UINT32 NAS_EMM_MsTauInitSsWaitCnCnfMsgAuthFail(
                                                  VOS_UINT32  ulMsgId,
                                                  VOS_VOID   *pMsgStru)
{
    NAS_EMM_INTRA_AUTH_FAIL_STRU        *pMsgAuthFail   = (NAS_EMM_INTRA_AUTH_FAIL_STRU *)pMsgStru;
    VOS_UINT32                          ulCause;

    (VOS_VOID)ulMsgId;

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_MsTauInitSsWaitCnCnfMsgAuthFail is entered.");

    /*���ԭ��ֵ*/
    ulCause                                             =   pMsgAuthFail->ulCause;

    /*����ԭ��ֵ����*/
    if(NAS_EMM_AUTH_REJ_INTRA_CAUSE_NORMAL              ==  ulCause)
    {
        NAS_EMM_MsTauInitSsWaitCnTauCnfProcMsgAuthRej(      ulCause);
    }
    else
    {
        NAS_EMM_MsTauInitSsWaitCNCnfProcMsgRrcRelInd(       ulCause);
    }


    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32 NAS_EMM_MsTauInitSsWaitCnCnfMsgAuthRej(
                                        VOS_UINT32  ulMsgId,
                                        const VOS_VOID   *pMsgStru)
{
    (VOS_VOID)ulMsgId;
    (VOS_VOID)pMsgStru;

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_MsTauInitSsWaitCnCnfMsgAuthRej is entered.");

    NAS_EMM_MsTauInitSsWaitCnTauCnfProcMsgAuthRej(NAS_EMM_AUTH_REJ_INTRA_CAUSE_NORMAL);

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32 NAS_EMM_MsTauInitSsWaitCnCnfMsgRrcRelInd(
                                                  VOS_UINT32  ulMsgId,
                                                  VOS_VOID   *pMsgStru)
{
    LRRC_LMM_REL_IND_STRU                 *pRrcRelInd     = (LRRC_LMM_REL_IND_STRU *)pMsgStru;
    VOS_UINT32                          ulCause;

    (VOS_VOID)ulMsgId;

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_MsTauInitSsWaitCnCnfMsgRrcRelInd is entered.");

    /*���ԭ��ֵ*/
    ulCause                                             =   pRrcRelInd->enRelCause;

    NAS_EMM_MsTauInitSsWaitCNCnfProcMsgRrcRelInd(           ulCause);

    return NAS_LMM_MSG_HANDLED;
}

/*****************************************************************************
 Function Name   : NAS_EMM_MsTauInitSsWaitCnCnfMsgDrbSetupInd
 Description     : �ڵȴ�TAU acp��Ϣ�������յ�Drb�����ɹ���Ϣ�Ĵ���
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.sunbing49683      2010-10-8  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_MsTauInitSsWaitCnCnfMsgDrbSetupInd(
                                                  VOS_UINT32  ulMsgId,
                                                  const VOS_VOID   *pMsgStru)
{
    (VOS_VOID)ulMsgId;
    (VOS_VOID)pMsgStru;

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_MsTauInitSsWaitCnCnfMsgDrbSetupInd is entered.");

    /*TAU�����У��ڵȴ�������Ӧʱ���յ��ײ�Drb�����ɹ�����Ϣ��
      Ϊ�˱�������TAU������ɺ��ͷ���·����Ҫ������TAU������Ϊ��Я����active flag��TAU����*/
    NAS_EMM_TAU_SaveEmmTauReqActiveCtrl(NAS_EMM_TAU_WITH_BEARER_EST_REQ);

    return NAS_LMM_MSG_HANDLED;
}



VOS_VOID  NAS_EMM_SetBarInfo(NAS_EMM_BAR_PROCEDURE_ENUM_UINT32 enBarProc,
                            LRRC_LNAS_EST_RESULT_ENUM_UINT32 enBarType)
{
    /*Coverity �澯���� 2013-9-26 Begin*/
    NAS_EMM_BAR_PROCEDURE_ENUM_UINT32 enBarProcOld;
    /*Coverity �澯���� 2013-9-26 End*/

    /*���Bar������:TAU or SER */
    if ((NAS_EMM_BAR_PROCEDURE_TAU != enBarProc)
      &&(NAS_EMM_BAR_PROCEDURE_SERVICE != enBarProc))
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_SetBarInfo:Input enBarProc Err.");
        return;
    }

    /*Coverity �澯���� 2013-9-26 Begin*/
    enBarProcOld = NAS_EMM_TAU_GetRegBarProcedure();
    /*Coverity �澯���� 2013-9-26 End*/

    /*�����Ч:��¼Bar�����̺�����*/
    NAS_EMM_TAU_SetRegBarProcedure(enBarProc);

    switch (enBarType)
    {
        case LRRC_EST_ACCESS_BARRED_MO_SIGNAL:
            NAS_EMM_TAU_GetRegBarType() |= NAS_EMM_BIT_1;
            break;

        case LRRC_EST_ACCESS_BARRED_ALL:
            NAS_EMM_TAU_GetRegBarType() |= NAS_LMM_LOW_HALF_BYTE_F;
            break;

        case LRRC_EST_ACCESS_BARRED_MO_CALL:
            NAS_EMM_TAU_GetRegBarType() |= NAS_EMM_BIT_2;
            break;

        case LRRC_EST_ACCESS_BARRED_MO_CSFB:
            NAS_EMM_TAU_GetRegBarType() |= NAS_EMM_BIT_4;
            break;

        case LRRC_EST_ACCESS_BARRED_MO_CALL_AND_CSFB:
            NAS_EMM_TAU_GetRegBarType() |= 0x0a;
            break;

         case LRRC_EST_ACCESS_BARRED_MT_CALL:
            NAS_EMM_TAU_GetRegBarType() |= NAS_EMM_BIT_3;
            break;

        default:
            /*Coverity �澯���� 2013-9-26 Begin*/
            NAS_EMM_TAU_SetRegBarProcedure(enBarProcOld);
            NAS_EMM_TAU_LOG_ERR("NAS_EMM_SetBarInfo:Input enBarType Err.");
            /*Coverity �澯���� 2013-9-26 End*/
            break;
    }
    return;
}

/*****************************************************************************
 Function Name   : NAS_EMM_JudgeBarType
 Description     : �ж��Ƿ���ĳ�����͵�bar
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.sunbing49683      2010-8-25  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_JudgeBarType( NAS_EMM_BAR_TYPE_ENUM_UINT32 enBarType )
{
    if( NAS_EMM_BAR_TYPE_MO_SIGNAL == enBarType)
    {
        /*bit1 Ϊ1����ʶMO SIGNAL���ͱ�bar*/
        if(NAS_EMM_BIT_SLCT == (NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_1))
        {
            return NAS_EMM_SUCC;
        }
        else
        {
            return NAS_EMM_FAIL;
        }
    }
    else if(NAS_EMM_BAR_TYPE_MO_CALL == enBarType)
    {
        /*bit2 Ϊ1����ʶMO CALL���ͱ�bar*/
        if(NAS_EMM_BIT_SLCT == ((NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_2)>> NAS_LMM_MOVEMENT_1_BITS))
        {
            return NAS_EMM_SUCC;
        }
        else
        {
            return NAS_EMM_FAIL;
        }
    }
    else if(NAS_EMM_BAR_TYPE_MT == enBarType)
    {
        /*bit3 Ϊ1����ʶMT CALL���ͱ�bar*/
        if(NAS_EMM_BIT_SLCT == ((NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_3)>>NAS_LMM_MOVEMENT_2_BITS))
        {
            return NAS_EMM_SUCC;
        }
        else
        {
            return NAS_EMM_FAIL;
        }

    }
    else if(NAS_EMM_BAR_TYPE_MO_CSFB == enBarType)
    {
        /*bit4 Ϊ1����ʶMO CSFB���ͱ�bar*/
        if(NAS_EMM_BIT_SLCT == ((NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_4)>>NAS_LMM_MOVEMENT_3_BITS))
        {
            return NAS_EMM_SUCC;
        }
        else
        {
            return NAS_EMM_FAIL;
        }

    }
    else if(NAS_EMM_BAR_TYPE_MO_CALL_AND_CSFB == enBarType)
    {
        /*bit4&bit2 Ϊ1����ʶMO CALL& MO CSFB���ͱ�bar*/
        if((NAS_EMM_BIT_SLCT == ((NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_4)>>NAS_LMM_MOVEMENT_3_BITS))
            && (NAS_EMM_BIT_SLCT == ((NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_2)>>NAS_LMM_MOVEMENT_1_BITS)))
        {
            return NAS_EMM_SUCC;
        }
        else
        {
            return NAS_EMM_FAIL;
        }

    }
    else
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_JudgeBarType: Bar type is err.");
        return NAS_EMM_FAIL;
    }
}
/*VOS_UINT32  NAS_EMM_GetBarType (VOS_VOID )
{
    NAS_EMM_BAR_TYPE_ENUM_UINT32 enBarType = NAS_EMM_BAR_TYPE_BUTT;

    if ((NAS_EMM_BIT_SLCT == (NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_1))
        && (NAS_EMM_BIT_NO_SLCT == (NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_2)>>NAS_LMM_MOVEMENT_1_BITS)
        && (NAS_EMM_BIT_NO_SLCT == (NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_3)>>NAS_LMM_MOVEMENT_2_BITS))
    {
        enBarType = NAS_EMM_BAR_TYPE_MO_SIGNAL;
    }
    else if ((NAS_EMM_BIT_NO_SLCT == (NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_1))
             &&(NAS_EMM_BIT_SLCT == ((NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_2)>> NAS_LMM_MOVEMENT_1_BITS))
             && (NAS_EMM_BIT_NO_SLCT == (NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_3)>>NAS_LMM_MOVEMENT_2_BITS))

    {
        enBarType = NAS_EMM_BAR_TYPE_MO_CALL;
    }
    else if ((NAS_EMM_BIT_SLCT == (NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_1))
            && (NAS_EMM_BIT_SLCT == (NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_2)>>NAS_LMM_MOVEMENT_1_BITS)
            && (NAS_EMM_BIT_SLCT ==(NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_3)>>NAS_LMM_MOVEMENT_2_BITS))
    {
        enBarType = NAS_EMM_BAR_TYPE_ALL;
    }
    else if ((NAS_EMM_BIT_SLCT == (NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_1))
            && (NAS_EMM_BIT_SLCT == (NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_2)>>NAS_LMM_MOVEMENT_1_BITS)
            && (NAS_EMM_BIT_NO_SLCT ==(NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_3)>>NAS_LMM_MOVEMENT_2_BITS))
    {
        enBarType = NAS_EMM_BAR_TYPE_MO;
    }
    else if ((NAS_EMM_BIT_NO_SLCT == (NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_1))
            && (NAS_EMM_BIT_NO_SLCT == (NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_2)>>NAS_LMM_MOVEMENT_1_BITS)
            && (NAS_EMM_BIT_SLCT ==(NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_3)>>NAS_LMM_MOVEMENT_2_BITS))
    {
        enBarType = NAS_EMM_BAR_TYPE_MT;
    }
    else if ((NAS_EMM_BIT_NO_SLCT == (NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_1))
            && (NAS_EMM_BIT_SLCT == (NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_2)>>NAS_LMM_MOVEMENT_1_BITS)
            && (NAS_EMM_BIT_SLCT ==(NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_3)>>NAS_LMM_MOVEMENT_2_BITS))
    {
        enBarType = NAS_EMM_BAR_TYPE_MO_CALL_AND_MT;
    }
    else if ((NAS_EMM_BIT_SLCT == (NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_1))
            && (NAS_EMM_BIT_NO_SLCT == (NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_2)>>NAS_LMM_MOVEMENT_1_BITS)
            && (NAS_EMM_BIT_SLCT ==(NAS_EMM_TAU_GetRegBarType() & NAS_EMM_BIT_3)>>NAS_LMM_MOVEMENT_2_BITS))
    {
        enBarType = NAS_EMM_BAR_TYPE_MO_SIGNAL_AND_MT;
    }
    else
    {
        ;
    }

    return enBarType;
}*/


VOS_VOID  NAS_EMM_ClearBarType( LRRC_LNAS_ACCESS_GRANT_ENUM_UINT32 enBarType)
{
    VOS_UINT32                          i = 0;

    for(i = 0; i < ulGrantTypeNum; i++)
    {
        if (enBarType == g_astGrantActTable[i].enGrantType)
        {
            NAS_EMM_TAU_GetRegBarType() &= g_astGrantActTable[i].ucGrantValue;

            break;
        }
    }

}





VOS_VOID  NAS_EMM_ClearBarResouce(VOS_VOID)
{
    NAS_LMM_MEM_SET(NAS_EMM_TAU_GetRegBarInfoAddr(), 0, sizeof(NAS_EMM_BAR_STRU));
    return;
}


VOS_UINT32  NAS_EMM_GetBarProcedure(VOS_VOID )
{
    NAS_EMM_BAR_PROCEDURE_ENUM_UINT32   ulBarProc;

    ulBarProc = NAS_EMM_TAU_GetRegBarProcedure();

    return ulBarProc;
}




VOS_UINT32 NAS_EMM_MsTauInitSsWaitCNCnfMsgIntraConnectFailInd(VOS_UINT32  ulMsgId,
                                                   VOS_VOID   *pMsgStru)
{

    VOS_UINT32                          ulRslt                  = NAS_EMM_FAIL;
    NAS_EMM_MRRC_CONNECT_FAIL_IND_STRU *pMrrcConnectFailRelInd  = NAS_EMM_NULL_PTR;
    MMC_LMM_TAU_RSLT_ENUM_UINT32        enTauRslt               = MMC_LMM_TAU_RSLT_ACCESS_BARED;
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg               = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_MsTauInitSsWaitCNCnfMsgIntraConnectFailInd is entered.");

    /*check the input params and current states*/
    ulRslt = NAS_EMM_TAU_CHKFSMStateMsgp(EMM_MS_TAU_INIT,EMM_SS_TAU_WAIT_CN_TAU_CNF,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_TAU_LOG_WARN( "NAS_EMM_MsTauInitSsWaitCNCnfMsgIntraConnectFailInd ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*get the msg INTRA_REL_IND*/
    pMrrcConnectFailRelInd = (NAS_EMM_MRRC_CONNECT_FAIL_IND_STRU *)pMsgStru;

    /*commom process*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_TAU_T3430);
	/* lihong00150010 emergency tau&service begin */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_MsTauInitSsWaitCNCnfMsgIntraConnectFailInd:CAUSE_ESM_EMC_PDN_REQ");

        if ((LRRC_EST_ACCESS_BARRED_MO_SIGNAL == pMrrcConnectFailRelInd->enEstResult)
            || (LRRC_EST_ACCESS_BARRED_ALL == pMrrcConnectFailRelInd->enEstResult))
        {
            NAS_EMM_MmcSendTauActionResultIndOthertype((VOS_VOID*)&enTauRslt);
        }
        else if (LRRC_EST_CELL_SEARCHING == pMrrcConnectFailRelInd->enEstResult)
        {
            enTauRslt = MMC_LMM_TAU_RSLT_FAILURE;
            NAS_EMM_MmcSendTauActionResultIndOthertype((VOS_VOID*)&enTauRslt);
        }
        else
        {
            NAS_EMM_MmcSendTauActionResultIndFailWithPara((VOS_VOID*)NAS_EMM_NULL_PTR);
        }

        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);

        /* ����DETACH�ͷ���Դ:��̬�ڴ桢����ֵ */
        NAS_LMM_DeregReleaseResource();

        /*��MMC���ͱ���LMM_MMC_DETACH_IND��Ϣ*/
        NAS_EMM_MmcSendDetIndLocal( MMC_LMM_L_LOCAL_DETACH_OTHERS);

        NAS_EMM_CommProcConn2Ilde();

        if (LRRC_EST_CELL_SEARCHING == pMrrcConnectFailRelInd->enEstResult)
        {
            /* ��¼ATTACH����ԭ��ֵ */
            NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;

            NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG,
                                        EMM_SS_DEREG_PLMN_SEARCH,
                                        TI_NAS_EMM_STATE_NO_TIMER);
        }
        else
        {
            /*����ESM��Ϣ*/
            NAS_LMM_MEM_CPY(        (VOS_VOID*)NAS_EMM_GLO_AD_GetEsmMsgAddr(),
                                   &(pstEsmMsg->stEsmMsg),
                                    (pstEsmMsg->stEsmMsg.ulEsmMsgSize)+4);

            NAS_EMM_StartEmergencyAttach();
        }

        return NAS_LMM_MSG_HANDLED;
    }
	/* lihong00150010 emergency tau&service end */
    switch (pMrrcConnectFailRelInd->enEstResult)
    {
        case LRRC_EST_ACCESS_BARRED_MO_SIGNAL:
        case LRRC_EST_ACCESS_BARRED_ALL:

            /*����bar���̺�����*/
            NAS_EMM_SetBarInfo(NAS_EMM_BAR_PROCEDURE_TAU,pMrrcConnectFailRelInd->enEstResult);

            /*ת��EMM״̬��MS_REG+SS_WAIT_ACCESS_GRANT_IND*/
            NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                        EMM_SS_REG_WAIT_ACCESS_GRANT_IND,
                                        TI_NAS_EMM_STATE_NO_TIMER);

            NAS_EMM_TAU_RelIndCollisionProc(NAS_EMM_MmcSendTauActionResultIndOthertype,
                               (VOS_VOID*)&enTauRslt);
            break;

        /* ����ʧ�ܣ�RRC���ڽ���С��������Ǩ�Ƶ�Reg.Plmn_Search*/
        /*
        case LRRC_EST_CELL_SEARCHING:
            NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                        EMM_SS_REG_PLMN_SEARCH,
                                        TI_NAS_EMM_STATE_NO_TIMER);

            NAS_EMM_TAU_RelIndCollisionProc((NAS_LMM_SEND_TAU_RESULT_ACT_FUN)NAS_EMM_NULL_PTR,
                                 (VOS_VOID*)NAS_EMM_NULL_PTR);
            break;
        */

        default:
            NAS_EMM_TAU_GetEmmTAUAttemptCnt()++;

            NAS_EMM_TAU_ProcAbnormal();

            /* ��MMC����LMM_MMC_TAU_RESULT_IND��Ϣ�ڳ�ͻ�ڲ����� */
            NAS_EMM_TAU_RelIndCollisionProc(NAS_EMM_MmcSendTauActionResultIndFailWithPara,
                                 (VOS_VOID*)NAS_EMM_NULL_PTR);
            #if 0
            /* ��������쳣������3402�����TAU����Counter */
            if(NAS_LMM_TIMER_RUNNING == NAS_LMM_IsPtlTimerRunning(TI_NAS_EMM_PTL_T3402))
            {
                /*��� Tau Attempt Counter*/
                NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);
            }
            #endif
            break;

    }

    /*send INTRA_CONN2IDLE_REQ����������״̬*/
    NAS_EMM_CommProcConn2Ilde();

    return NAS_LMM_MSG_HANDLED;
}

#if 0   /*��ʱ��������ͷ�ԭ��Ϊ��������ĳ���*/

static VOS_VOID  NAS_EMM_TAU_RrcRelCauseDrxOrNetCapChange( VOS_VOID )
{
    NAS_EMM_TAU_START_CAUSE_ENUM_UINT8            enTauStartCause;

    NAS_EMM_TAU_LOG_NORM(                         "NAS_EMM_TAU_RrcRelCauseDrxOrNetCapChange is entered.");

    /* ����ԭ��ֵ����ͬ���͵�TAU
    */
    enTauStartCause                               = NAS_EMM_TAU_GetEmmTAUStartCause();

    if(NAS_EMM_TAU_START_CAUSE_RRC_REL_LOAD_BALANCE
                                                  == enTauStartCause)
    {
        NAS_EMM_TAU_StartTAUREQ(                  NAS_EMM_MSG_LOAD_BALANCING_TAU);
    }
    else
    {
        NAS_EMM_TAU_StartTAUREQ(                  NAS_EMM_MSG_NOT_LOAD_BALANCING_TAU);
    }

    return;

}
#endif

VOS_UINT32 NAS_EMM_SndTauCompleteFailProc(VOS_VOID* pMsg,VOS_UINT32 *pulIsDelBuff)
{
    LRRC_LMM_DATA_CNF_STRU              *pstRrcMmDataCnf = VOS_NULL_PTR;
    NAS_EMM_MRRC_DATA_REQ_STRU          *pMrrcDataMsg = VOS_NULL_PTR;
    VOS_UINT32                            ulRrcMmDataReqMsgLen;

    NAS_EMM_MRRC_MGMT_DATA_STRU         *pEmmMrrcMgmtData = NAS_EMM_NULL_PTR;

    *pulIsDelBuff = VOS_TRUE;

    pstRrcMmDataCnf = (LRRC_LMM_DATA_CNF_STRU*) pMsg;

    pEmmMrrcMgmtData = NAS_EMM_FindMsgInDataReqBuffer(pstRrcMmDataCnf->ulOpId);

    if (NAS_EMM_NULL_PTR == pEmmMrrcMgmtData)
    {
        return NAS_EMM_FAIL;
    }
    ulRrcMmDataReqMsgLen = (sizeof(NAS_EMM_MRRC_DATA_REQ_STRU)+
                           pEmmMrrcMgmtData->ulNasMsgLength) -
                           NAS_EMM_4BYTES_LEN;
    /* ������Ϣ�ڴ�*/
    pMrrcDataMsg = (NAS_EMM_MRRC_DATA_REQ_STRU *)(VOS_VOID*)NAS_LMM_MEM_ALLOC(ulRrcMmDataReqMsgLen);

    if(VOS_NULL_PTR == pMrrcDataMsg)
    {
        NAS_EMM_PUBU_LOG_ERR("NAS_EMM_SndTauCompleteFailProc: Mem Alloc Fail");
        return NAS_EMM_FAIL;
    }

    switch (pstRrcMmDataCnf->enSendRslt)
    {
        case LRRC_LMM_SEND_RSLT_FAILURE_HO:
            /*  ��Ҫ����Ӧ�Ļ�����Ϣ��ulHoWaitSysInfoFlag����Ϊtrue */
            pEmmMrrcMgmtData->ulHoWaitSysInfoFlag = VOS_TRUE;
            *pulIsDelBuff = VOS_FALSE;

            break;

        case LRRC_LMM_SEND_RSLT_FAILURE_TXN:
        case LRRC_LMM_SEND_RSLT_FAILURE_RLF:
            pMrrcDataMsg->enDataCnf = LRRC_LMM_DATA_CNF_NEED;

            pMrrcDataMsg->enEmmMsgType = NAS_EMM_MSG_TAU_CMP;
            pMrrcDataMsg->ulMsgId = ID_NAS_LMM_INTRA_MRRC_DATA_REQ;
            pMrrcDataMsg->ulEsmMmOpId = pEmmMrrcMgmtData->ulEsmMmOpId;

            pMrrcDataMsg->stNasMsg.ulNasMsgSize = pEmmMrrcMgmtData->ulNasMsgLength;

            NAS_LMM_MEM_CPY(pMrrcDataMsg->stNasMsg.aucNasMsg,
                             pEmmMrrcMgmtData->aucNasMsgContent,
                             pEmmMrrcMgmtData->ulNasMsgLength);

            NAS_EMM_SndUplinkNasMsg(pMrrcDataMsg);


            break;

        case LRRC_LMM_SEND_RSLT_FAILURE_CONN_REL:
        case LRRC_LMM_SEND_RSLT_FAILURE_CTRL_NOT_CONN:
            NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_T3402);
            NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_T3411);

            NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
            NAS_LMM_SetEmmInfoTriggerTauRrcRel(NAS_EMM_TRIGGER_TAU_RRC_REL_CONN_FAILURE);
            break;

        default:
            break;
    }

    /*�ͷ��������ڴ� */
    NAS_LMM_MEM_FREE(pMrrcDataMsg);
    return NAS_EMM_SUCC;
}
/*lint +e961*/
/*lint +e960*/

VOS_UINT32 NAS_EMM_SndTauReqFailProc(VOS_VOID* pMsg,VOS_UINT32 *pulIsDelBuff)
{
    LRRC_LMM_DATA_CNF_STRU              *pstRrcMmDataCnf = VOS_NULL_PTR;
    NAS_LMM_MAIN_STATE_ENUM_UINT16       enMainState;

    enMainState                         = NAS_LMM_GetEmmCurFsmMS();

    *pulIsDelBuff = VOS_TRUE;

    /*���״̬�Ƿ�ƥ�䣬����ƥ�䣬�˳�*/
    if (EMM_MS_TAU_INIT != enMainState)
    {
        /*��ӡ������Ϣ*/
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_SndTauReqFailProc: STATE ERR!");
        return  NAS_EMM_SUCC;
    }

    pstRrcMmDataCnf = (LRRC_LMM_DATA_CNF_STRU*) pMsg;

    switch (pstRrcMmDataCnf->enSendRslt)
    {
        case LRRC_LMM_SEND_RSLT_FAILURE_HO:
        case LRRC_LMM_SEND_RSLT_FAILURE_TXN:
        case LRRC_LMM_SEND_RSLT_FAILURE_RLF:

            /*����EPS״̬ΪEU2*/
            NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_NOT_UPDATED_EU2;

            if (NAS_LMM_REG_STATUS_EMC_REGED != NAS_LMM_GetEmmInfoRegStatus())
            {
                /*����PS LOC��Ϣ*/
                NAS_LMM_WritePsLoc(NAS_NV_ITEM_UPDATE);
            }
            /*ֹͣT3430��ʱ��*/
            NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_TAU_T3430);

            /*ֹͣT3440��ʱ��*/
            NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);

            NAS_EMM_TAU_StartTAUREQ();
            break;

        default:
            break;
        }

    return NAS_EMM_SUCC;
}


VOS_UINT32 NAS_EMM_SndTauCmpSuccProc(VOS_VOID* pMsg)
{
    (VOS_VOID)pMsg;

    NAS_EMM_PUBU_LOG_NORM("NAS_EMM_SndTauCmpSuccProc: entern");
    NAS_EMM_TAU_SaveEmmTauCompleteFlag(NAS_EMM_TAU_COMPLETE_INVALID);

    return NAS_EMM_SUCC;
}
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif



