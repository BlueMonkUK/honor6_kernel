


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "NasEmmTauSerInclude.h"
#include "NasEmmAttDetInclude.h"


/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASEMMSERVICEABNORMAL_C
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

/*******************************************************************************
  Module   : NAS_EMM_SER_AbnormalNeedtoContinueTau
  Function :
  Input    :
  Output   :
  NOTE     : ��
  Return   : ��
  History  :
    1.FTY    2012-02-27  �¹�����
*******************************************************************************/
VOS_UINT32 NAS_EMM_SER_AbnormalNeedtoContinueTau(VOS_VOID)
{
    if((NAS_EMM_T3412_EXP_NO < NAS_LMM_GetEmmInfoT3412ExpCtrl())
      &&(NAS_EMM_T3412_EXP_BUTT > NAS_LMM_GetEmmInfoT3412ExpCtrl()))
    {
        return NAS_EMM_YES;
    }

    if(NAS_EMM_EPS_BEARER_STATUS_CHANGEED == NAS_EMM_GetEpsContextStatusChange())
    {
        return NAS_EMM_YES;
    }

    if(NAS_EMM_YES == NAS_LMM_GetEmmInfoDrxNetCapChange())
    {
        return NAS_EMM_YES;
    }

    if(NAS_EMM_TRIGGER_TAU_RRC_REL_LOAD_BALANCE == NAS_LMM_GetEmmInfoTriggerTauRrcRel())
    {
        return NAS_EMM_YES;
    }
    if(NAS_EMM_TRIGGER_TAU_RRC_REL_CONN_FAILURE == NAS_LMM_GetEmmInfoTriggerTauRrcRel())
    {
        return NAS_EMM_YES;
    }
    if(NAS_EMM_YES == NAS_LMM_GetEmmInfoTriggerTauSysChange())
    {
        return NAS_EMM_YES;
    }

    if(NAS_LMM_UE_RADIO_CAP_NOT_CHG != NAS_LMM_GetEmmInfoUeRadioCapChgFlag())
    {
        return NAS_EMM_YES;
    }


    return NAS_EMM_NO;

}

/*******************************************************************************
  Module   : NAS_EMM_SER_SuccNeedtoContinueTau
  Function :
  Input    :
  Output   :
  NOTE     : ��
  Return   : ��
  History  :
    1.FTY    2012-02-27  �¹�����
*******************************************************************************/
VOS_UINT32 NAS_EMM_SER_SuccNeedtoContinueTau(VOS_VOID)
{
    if(NAS_EMM_EPS_BEARER_STATUS_CHANGEED == NAS_EMM_GetEpsContextStatusChange())
    {
        return NAS_EMM_YES;
    }

    if(NAS_EMM_YES == NAS_LMM_GetEmmInfoDrxNetCapChange())
    {
        return NAS_EMM_YES;
    }

    if(NAS_EMM_YES == NAS_LMM_GetEmmInfoTriggerTauSysChange())
    {
        return NAS_EMM_YES;
    }

    return NAS_EMM_NO;

}
#if 0

VOS_VOID NAS_EMM_SER_RrcRelCause_SERREJ25(VOS_VOID)
{
    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_RrcRelCause_SERREJ25 is entered.");

    /*����EU3 ROAMING NOT ALLOWED*/
    NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_ROAMING_NOT_ALLOWED_EU3;

     /*����EMM_ESM_STATUS_IND(ע��)*/
    NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_ATTACHED);

    /*ת��EMM״̬��MS_REG+SS_DEREG_LIMITED_SERVICE*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_LIMITED_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);

    /*����CELL_SELECTION_CTRL_REQ�ڲ���Ϣ��PUBLICģ��*/
    /*NAS_EMM_TAUSER_SendCellSelectionReq();*/

    return;
}

#endif

VOS_VOID    NAS_EMM_SER_RcvRrcRelInd(VOS_VOID)
{
    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO("NAS_EMM_SER_RcvRrcRelInd is entered.");

    /*����SER�Ľ��Ϊ NAS_EMM_SER_RESULT_ABNORMAL*/
    /*NAS_EMM_SER_SaveSERresult(NAS_EMM_SER_RESULT_ABNORMAL);*/
    /* lihong00150010 emergency tau&service begin */
    NAS_EMM_TranStatePsNormalServiceOrPsLimitService();
    /* lihong00150010 emergency tau&service end */
    /*ת��EMM״̬��MS_REG+EMM_SS_REG_NORMAL_SERVICE*/
    /*NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_NORMAL_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);*/

    if (VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
    {
        /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
        NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);

        /*send INTRA_CONN2IDLE_REQ����������״̬*/
        NAS_EMM_CommProcConn2Ilde();
        return ;
    }

    /*send INTRA_CONN2IDLE_REQ����������״̬*/
    NAS_EMM_CommProcConn2Ilde();

    /* �����δ��ɵ�TAU�������ڴ˴������ж�ʱ���ȶ�ʱ����ʱ���޶�ʱ���Ⱥ����յ�ϵͳ��Ϣ�ٴ��� */

#if 0
    /*�����δ��ɵ�TAU������Ҫ���´���TAU*/
    if(NAS_EMM_YES == NAS_EMM_SER_AbnormalNeedtoContinueTau())
    {
        /*���T3411�Ѿ�ֹͣ���У�������������(�����TAU��Ba�󴥷���Service,��T3411û������)
          ��֮�����T3411�������У������ﲻ��������T3411��ʱ���ٴ���*/
        if(NAS_LMM_TIMER_STOPED == NAS_LMM_IsPtlTimerRunning(TI_NAS_EMM_PTL_T3411))
        {
            NAS_EMM_TAU_StartTAUREQ();
        }
    }
#endif
    return;
}


VOS_VOID  NAS_EMM_ServiceReqRejectOtherCause
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)

{
    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_ServiceReqRejectOtherCause is entered.");

	/* lihong00150010 emergency tau&service begin */
    /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_ServiceReqRejectOtherCause: CAUSE_ESM_DATA_REQ_EMC.");

        NAS_EMM_EmcPndReqSerAbnormalCommProc(EMM_SS_DEREG_NORMAL_SERVICE);
    }
    else
    {
        NAS_EMM_TranStatePsNormalServiceOrPsLimitService();
    }
	/* lihong00150010 emergency tau&service end */
    /* �����δ��ɵ�TAU������REG+NORMAL_SERVICE״̬���յ�ϵͳ��Ϣ���ٴ��� */

    /*��MRRC����NAS_EMM_MRRC_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);


    return;
}


/*lint -e960*/
/*lint -e961*/
VOS_VOID NAS_EMM_SerClearEsmDataBuf( VOS_VOID )
{
    VOS_UINT32                          i      = 0;
    VOS_UINT32                          ulRslt = 0;

     NAS_EMM_SER_LOG_INFO("NAS_EMM_SerClearEsmDataBuf entered!");

     /**����������ݵı�־��������**************/
     for (i = 0; i < g_stEmmEsmMsgBuf.ulEsmMsgCnt; i++)
     {
         if (NAS_LMM_NULL_PTR != g_stEmmEsmMsgBuf.apucEsmMsgBuf[i])
         {
            ulRslt = NAS_COMM_FreeBuffItem(NAS_COMM_BUFF_TYPE_EMM,
                                (VOS_VOID *)g_stEmmEsmMsgBuf.apucEsmMsgBuf[i]);

            if (NAS_COMM_BUFF_SUCCESS != ulRslt)
            {
                NAS_EMM_SER_LOG_WARN("NAS_EMM_SerClearEsmDataBuf, Memory Free is not succ");
            }

            g_stEmmEsmMsgBuf.apucEsmMsgBuf[i] = NAS_LMM_NULL_PTR;
         }
         else
         {
             NAS_EMM_SER_LOG1_WARN("NAS_EMM_SerClearEsmDataBuf, Buffer item is null:",i);
         }
     }

     g_stEmmEsmMsgBuf.ulEsmMsgCnt = 0;

}
VOS_UINT32 NAS_EMM_SER_IsNotEmergencyCsfb(VOS_VOID)
{
    if (NAS_EMM_SER_START_CAUSE_MO_EMERGENCY_CSFB_REQ != NAS_EMM_SER_GetEmmSERStartCause())
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_VOID  NAS_EMM_SER_SERREJ367
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)

{
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ367 entered.");

    /* ɾ��GUTI����Զ�����EPS_LOC,������Ҫ������STATUS */
    /* set the EPS update status to EU3 ROAMING NOT ALLOWED */
    NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_ROAMING_NOT_ALLOWED_EU3;

    /*ɾ��GUTI,KSIasme,TAI list,GUTI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_DELETE_RPLMN);

    NAS_LMM_SetPsSimValidity(NAS_LMM_SIM_INVALID);
    /*USIM��Ч��ֱ���ػ���ο�*/
    /*�ݲ�����*/
	/* lihong00150010 emergency tau&service begin */
    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);

        /* ��¼ATTACH����ԭ��ֵ */
        NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
	/* lihong00150010 emergency tau&service end */
    /*ת��EMM״̬��MS_DEREG+SS_DEREG_LIMITED_SERVICE*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG, EMM_SS_DEREG_NO_IMSI, TI_NAS_EMM_STATE_NO_TIMER);

    /* �����CSFB���̣��Ҳ��ǽ���CSFB�����MM����ֹ��Ϣ */
    if ((VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
        && (VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb()))
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_CN_REJ);
    }


    /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    /*��LRRC����LRRC_LMM_NAS_INFO_CHANGE_REQЯ��USIM��״̬*/
    NAS_EMM_SendUsimStatusToRrc(LRRC_LNAS_USIM_PRESENT_INVALID);

    /*��MRRC����NAS_EMM_MRRC_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);

    return;
}
VOS_VOID  NAS_EMM_SER_SERREJ9
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)
{
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ9 entered.");

    /* ɾ��GUTI����Զ�����EPS_LOC,������Ҫ������STATUS */
    /* ��������EU2 NOT UPDATED*/
    NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_NOT_UPDATED_EU2;

    /* ɾ��GUTI,KSIasme,TAI list,L.V.R TAI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_NOT_DELETE_RPLMN);
	/* lihong00150010 emergency tau&service begin */
    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
	/* lihong00150010 emergency tau&service end */
    /* �����CSFB���̣��Ҳ��ǽ���CSFB�����MM����ֹ��Ϣ */
    if ((VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
        && (VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb()))
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_CN_REJ);
    }

    /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG,
                                EMM_SS_DEREG_NORMAL_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);
	/* lihong00150010 emergency tau&service begin */
    /* ����ǽ�����PDN���������SERVICE����ֱ�ӷ������ע�᣻
       ����ǽ���CSFB,ֱ���ͷ���·�������ڲ�ATTACH; */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_EMM_SER_LOG_INFO("NAS_EMM_SER_SERREJ9: CAUSE_ESM_DATA_REQ_EMC");

        /*����ESM��Ϣ*/
        NAS_LMM_MEM_CPY(        (VOS_VOID*)NAS_EMM_GLO_AD_GetEsmMsgAddr(),
                               &(pstEsmMsg->stEsmMsg),
                                (pstEsmMsg->stEsmMsg.ulEsmMsgSize)+4);

        NAS_EMM_StartEmergencyAttach();
    }
    else if (VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb())
    {/* lihong00150010 emergency tau&service end */
        /* ��ʱ����attach���� */
        NAS_EMM_ReattachDelay();
    }
    else
    {
        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
    }

    /* ���SER��TAU������Դ*/
    NAS_EMM_SER_ClearResource();
    NAS_EMM_TAU_ClearResouce();
    NAS_EMM_ClearAppMsgPara();

    return;

}


VOS_VOID  NAS_EMM_SER_SERREJ10
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)
{
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ10 is entered.");
	/* lihong00150010 emergency tau&service begin */
    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
	/* lihong00150010 emergency tau&service end */
    /* �����CSFB���̣��Ҳ��ǽ���CSFB�����MM����ֹ��Ϣ */
    if ((VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
        && (VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb()))
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_CN_REJ);
    }

    /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    /* ת��EMM״̬��MS_DEREG+SS_DEREG_NORMAL_SERVICE:״̬ת������ִ��ɾ���°�ȫ������*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG,
                                EMM_SS_DEREG_NORMAL_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /* ����DEREG̬����дͳһд��ȫ�����ģ����ձ�ˣ���SER��#10�ܾ�������ȫ������д�� */
    NAS_LMM_WriteEpsSecuContext(NAS_NV_ITEM_UPDATE);
    /* lihong00150010 emergency tau&service end */
    /* ����ǽ�����PDN���������SERVICE����ֱ�ӷ������ע�᣻
       ����ǽ���CSFB,ֱ���ͷ���·�������ڲ�ATTACH; */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_EMM_SER_LOG_INFO("NAS_EMM_SER_SERREJ10: CAUSE_ESM_DATA_REQ_EMC");

        /*����ESM��Ϣ*/
        NAS_LMM_MEM_CPY(        (VOS_VOID*)NAS_EMM_GLO_AD_GetEsmMsgAddr(),
                               &(pstEsmMsg->stEsmMsg),
                                (pstEsmMsg->stEsmMsg.ulEsmMsgSize)+4);

        NAS_EMM_StartEmergencyAttach();
    }
    else if (VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb())
    {/* lihong00150010 emergency tau&service end */
        /* ��ʱ����attach���� */
        NAS_EMM_ReattachDelay();

    }
    else
    {
        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
    }

    /* ���SER��TAU������Դ*/
    NAS_EMM_SER_ClearResource();
    NAS_EMM_TAU_ClearResouce();
    NAS_EMM_ClearAppMsgPara();

    return;

}


VOS_VOID  NAS_EMM_SER_SERREJ11
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)

{
     /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ11 is entered.");

    /* ɾ��GUTI����Զ�����EPS_LOC,������Ҫ������STATUS */
    /*����EU3 ROAMING NOT ALLOWED*/
    NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_ROAMING_NOT_ALLOWED_EU3;

    /*ɾ��GUTI,KSIasme,TAI list,L.V.R TAI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_NOT_DELETE_RPLMN);
	/* lihong00150010 emergency tau&service begin */
    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);

        /* ��¼ATTACH����ԭ��ֵ */
        NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
	/* lihong00150010 emergency tau&service end */
    /*ת��EMM״̬��MS_DEREG+EMM_SS_DEREG_LIMITED_SERVICE*/
    /*�յ�MMC����ָ�����ת��PLMN_SEARCH̬*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG, EMM_SS_DEREG_LIMITED_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);

    /* �����CSFB���̣��Ҳ��ǽ���CSFB�����MM����ֹ��Ϣ */
    if ((VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
        && (VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb()))
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_CN_REJ);
    }

    /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    /*�ȴ�RRC_REL_IND*/
    NAS_EMM_WaitNetworkRelInd();

    return;
}


VOS_VOID  NAS_EMM_SER_SERREJ12
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)

{

    NAS_MM_TA_STRU                      stTa;

    /* ��ȡ��ǰTA */
    NAS_EMM_GetCurrentTa(&stTa);

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ12 is entered.");

    /* ɾ��GUTI����Զ�����EPS_LOC,������Ҫ������STATUS */
    /*����EU3 ROAMING NOT ALLOWED*/
    NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_ROAMING_NOT_ALLOWED_EU3;

    /*ɾ��GUTI,KSIasme,TAI list,L.V.R TAI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_NOT_DELETE_RPLMN);

    /*store the current TAI in the list of
                  "forbidden tracking areas for regional provision of service"*/
    NAS_EMM_AddForbTa(&stTa,NAS_LMM_GetEmmInfoNetInfoForbTaForRposAddr());
	/* lihong00150010 emergency tau&service begin */
    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);

        /* ��¼ATTACH����ԭ��ֵ */
        NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
	/* lihong00150010 emergency tau&service end */

    /* ����REJ cause 12���,������MMC�ϱ����������Ƶ����Ʒ���״̬ 2011-07-27*/
    NAS_LMM_SetNasRejCause12Flag(NAS_LMM_REJ_CAUSE_IS_12);

    /*ת��EMM״̬��MS_DEREG+SS_DEREG_LIMITED_SERVICE*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG, EMM_SS_DEREG_LIMITED_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);

    /* �����CSFB���̣��Ҳ��ǽ���CSFB�����MM����ֹ��Ϣ */
    if ((VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
        && (VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb()))
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_CN_REJ);
    }

    /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    /*�ȴ�RRC_REL_IND*/
    NAS_EMM_WaitNetworkRelInd();

    return;
}


VOS_VOID  NAS_EMM_SER_SERREJ13
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)

{

    NAS_MM_TA_STRU                      stCurrentTa;
    NAS_MM_TA_LIST_STRU                *pstTaiList;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ13 is entered.");

    /*����EU3 ROAMING NOT ALLOWED*/
    NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_ROAMING_NOT_ALLOWED_EU3;
	/* lihong00150010 emergency tau&service begin */
    if (NAS_LMM_REG_STATUS_EMC_REGED != NAS_LMM_GetEmmInfoRegStatus())
    {
        /*����PS LOC��Ϣ*/
        NAS_LMM_WritePsLoc(NAS_NV_ITEM_UPDATE);
    }
    /* lihong00150010 emergency tau&service end */
    /*Remove current TA from TAI List*/
    NAS_EMM_GetCurrentTa(&stCurrentTa);

    pstTaiList = NAS_LMM_GetEmmInfoNetInfoTaiListAddr();
    NAS_LMM_DeleteTaFromTaList(&stCurrentTa, pstTaiList, NAS_MM_MAX_TA_NUM);

    /*store the current TAI in the list of
                "forbidden tracking areas for roaming"*/
    NAS_EMM_AddForbTa(&stCurrentTa,NAS_LMM_GetEmmInfoNetInfoForbTaForRoamAddr());

    NAS_EMMC_SendRrcCellSelectionReq(LRRC_LNAS_FORBTA_CHANGE);

    /* lihong00150010 emergency tau&service begin */
    /* �����CSFB���̣��Ҳ��ǽ���CSFB�����MM����ֹ��Ϣ */
    if ((VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
        && (VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb()))
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_CN_REJ);
    }

    /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_SER_SERREJ13: CAUSE_ESM_DATA_REQ_EMC.");

        NAS_EMM_EmcPndReqSerAbnormalCommProc(EMM_SS_DEREG_LIMITED_SERVICE);
    }
    else
    {
        /*Ϊ�ϱ�����״̬,ת��EMM״̬��MS_REG+SS_REG_LIMITED_SERVICE�յ�MMC����ָ��
          ����ת��PLMN_SEARCH̬*/
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_LIMITED_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);
    }
	/* lihong00150010 emergency tau&service end */
    /*�ȴ�RRC_REL_IND*/
    NAS_EMM_WaitNetworkRelInd();

    return;
}


VOS_VOID  NAS_EMM_SER_SERREJ15
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)

{

    NAS_MM_TA_STRU                      stCurrentTa;
    NAS_MM_TA_LIST_STRU                *pstTaiList;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ15 is entered.");

    /*����EU3 ROAMING NOT ALLOWED*/
    NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_ROAMING_NOT_ALLOWED_EU3;
	/* lihong00150010 emergency tau&service begin */
    if (NAS_LMM_REG_STATUS_EMC_REGED != NAS_LMM_GetEmmInfoRegStatus())
    {
        /*����PS LOC��Ϣ*/
        NAS_LMM_WritePsLoc(NAS_NV_ITEM_UPDATE);
    }
	/* lihong00150010 emergency tau&service end */
    /*Remove current TA from TAI List*/
    NAS_EMM_GetCurrentTa(&stCurrentTa);

    pstTaiList = NAS_LMM_GetEmmInfoNetInfoTaiListAddr();
    NAS_LMM_DeleteTaFromTaList(&stCurrentTa, pstTaiList, NAS_MM_MAX_TA_NUM);

    /*store the current TAI in the list of
                   "forbidden tracking areas for roaming"*/
    NAS_EMM_AddForbTa(&stCurrentTa,NAS_LMM_GetEmmInfoNetInfoForbTaForRoamAddr());

    NAS_EMMC_SendRrcCellSelectionReq(LRRC_LNAS_FORBTA_CHANGE);

	/* lihong00150010 emergency tau&service begin */
    /* �����CSFB���̣��Ҳ��ǽ���CSFB�����MM����ֹ��Ϣ */
    if ((VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
        && (VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb()))
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_CN_REJ);
    }

    /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_SER_SERREJ15: CAUSE_ESM_DATA_REQ_EMC.");

        NAS_EMM_EmcPndReqSerAbnormalCommProc(EMM_SS_DEREG_LIMITED_SERVICE);
    }
    else
    {
        /*Ϊ�ϱ�����״̬,ת��EMM״̬��MS_REG+SS_REG_LIMITED_SERVICE�յ�MMC����ָ��
          ����ת��PLMN_SEARCH̬*/
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_LIMITED_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);
    }
	/* lihong00150010 emergency tau&service end */
    /*�ȴ�RRC_REL_IND*/
    NAS_EMM_WaitNetworkRelInd();

    return;
}


VOS_VOID  NAS_EMM_SER_SERREJ18
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)
{
    NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_PS);

    /* �����CSFB���̣��Ҳ��ǽ���CSFB�����MM����ֹ��Ϣ */
    if ((VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
        && (VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb()))
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_CN_REJ);
    }

    /*ת��EMM״̬��MS_REG+SS_REG_NORMAL_SERVIC*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_NORMAL_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);

    /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    /* ����ǽ���CSFB���߲�����������̬,�����ͷ� */
    if ((VOS_FALSE == NAS_EMM_SER_IsNotEmergencyCsfb())
        || (NAS_EMM_CONN_DATA != NAS_EMM_GetConnState()))
    {
        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
    }

    return;

}

VOS_VOID  NAS_EMM_SER_SERREJ40
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)
{
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ40 is entered.");

    (VOS_VOID)pMsgStru;

    /* ����EMM_ESM_STATUS_IND(ȥע��)*/
    NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);

    /* ת��EMM״̬��MS_DEREG+SS_DEREG_NORMAL_SERVICE:״̬ת������ִ��ɾ���°�ȫ������*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG,
                                EMM_SS_DEREG_NORMAL_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    /* �����CSFB���̻����CSFB������ѡ��GUģ */
    if ((VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
        || (VOS_FALSE == NAS_EMM_SER_IsNotEmergencyCsfb()))
    {
        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
    }
    else
    {
        (VOS_VOID)NAS_EMM_SendIntraAttachReq();
    }

    /* ���SER��TAU������Դ*/
    NAS_EMM_SER_ClearResource();
    NAS_EMM_TAU_ClearResouce();
    NAS_EMM_ClearAppMsgPara();

    return;
}
VOS_VOID  NAS_EMM_SER_SERREJ39
(
    const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru
)
{
    VOS_UINT32                      ulTimerLen      = NAS_EMM_NULL;

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_SERREJ39 is entered.");

    /* �����CSFB���̣��Ҳ��ǽ���CSFB�����MM����ֹ��Ϣ */
    if ((VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
        && (VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb()))
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_CN_REJ);
    }

    /*ת��EMM״̬��MS_REG+SS_REG_NORMAL_SERVIC*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_NORMAL_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);

    /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
    NAS_EMM_MmcSendSerResultIndRej(pMsgStru->ucEMMCause);

    /* ����ǽ���CSFB���߲�����������̬,�����ͷ� */
    if ((VOS_FALSE == NAS_EMM_SER_IsNotEmergencyCsfb())
        || (NAS_EMM_CONN_DATA != NAS_EMM_GetConnState()))
    {
        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
    }

    if (NAS_EMM_BIT_SLCT != pMsgStru->ucBitOpT3442)
    {
        NAS_EMM_SER_LOG_WARN("NAS_EMM_SER_SERREJ39:No T3442!");
        return ;
    }

    ulTimerLen = NAS_EMM_TimerValTransfer(  pMsgStru->stT3442.ucUnit,
                                            pMsgStru->stT3442.ucTimerVal);

    if (NAS_EMM_NULL != ulTimerLen)
    {
        NAS_LMM_ModifyStateTimer(TI_NAS_EMM_STATE_SERVICE_T3442,ulTimerLen);
        NAS_LMM_StartStateTimer(TI_NAS_EMM_STATE_SERVICE_T3442);
    }

    return;
}



VOS_VOID    NAS_EMM_SER_RcvServiceRejectMsg(const NAS_EMM_CN_SER_REJ_STRU   *pMsgStru)
{
    NAS_EMM_CN_CAUSE_ENUM_UINT8     ucSerRejcause   = NAS_LMM_CAUSE_PROTOCOL_ERROR;

    NAS_EMM_SER_LOG_INFO( "Nas_Emm_Ser_RcvServiceRejectMsg is entered.");

    /*�쳣ֹͣ SER*/
    NAS_EMM_SER_AbnormalOver();

    /*����SER�Ľ��Ϊ NAS_EMM_SER_RESULT_REJ*/
    /*NAS_EMM_SER_SaveSERresult(NAS_EMM_SER_RESULT_REJ);*/

    /*��ȡSERVICE REJԭ��ֵ*/
    ucSerRejcause = pMsgStru->ucEMMCause;


    /*����SER REJԭ��*/
    /*NAS_EMM_SER_SaveEmmSERRejCause(ucSerRejcause);*/



    /*SER REJ #9&#10���ͷ����ӣ�֧�ִ���
      ����ԭ��ֵͳһ����:�ͷ�����*/
    switch(ucSerRejcause)
    {
        case    NAS_LMM_CAUSE_ILLEGAL_UE   :
        case    NAS_LMM_CAUSE_ILLEGAL_ME   :
        case    NAS_LMM_CAUSE_EPS_SERV_NOT_ALLOW   :

                NAS_EMM_SER_SERREJ367(pMsgStru);
                break;

        case    NAS_LMM_CAUSE_UE_ID_NOT_DERIVED    :


                NAS_EMM_SER_SERREJ9(pMsgStru);
               break;

        case    NAS_LMM_CAUSE_IMPLICIT_DETACHED   :
                /*NAS_EMM_TAU_SaveEmmRRCRelCause(NAS_EMM_RRC_REL_CAUSE_SERREJ10);
                break;*/
                NAS_EMM_SER_SERREJ10(pMsgStru);
                break;

        case    NAS_LMM_CAUSE_PLMN_NOT_ALLOW    :
                NAS_EMM_SER_SERREJ11(pMsgStru);
                break;
        case    NAS_LMM_CAUSE_REQUESTED_SER_OPTION_NOT_AUTHORIZED_IN_PLMN:
                if (NAS_RELEASE_CTRL)
                {
                    NAS_EMM_SER_SERREJ11(pMsgStru);
                }
                else
                {
                    NAS_EMM_ServiceReqRejectOtherCause(pMsgStru);
                }
                break;
        case    NAS_LMM_CAUSE_TA_NOT_ALLOW   :
                NAS_EMM_SER_SERREJ12(pMsgStru);
                break;

        case    NAS_LMM_CAUSE_ROAM_NOT_ALLOW    :
                NAS_EMM_SER_SERREJ13(pMsgStru);
                break;

        case    NAS_LMM_CAUSE_NO_SUITABL_CELL   :
                NAS_EMM_SER_SERREJ15(pMsgStru);
                break;

        case    NAS_LMM_CAUSE_NO_EPS_BEARER_CONTEXT_ACTIVATED:
                if (NAS_RELEASE_CTRL)
                {
                    NAS_EMM_SER_SERREJ40(pMsgStru);
                }
                else
                {
                    NAS_EMM_ServiceReqRejectOtherCause(pMsgStru);
                }
                break;

/* CSG������δʵ�֣��յ�REJ #25����CSG��������default�����֧*/
#if 0
        case    NAS_LMM_CAUSE_NOT_AUTHORIZED_FOR_THIS_CSG:
                NAS_EMM_TAU_SaveEmmRRCRelCause(NAS_EMM_RRC_REL_CAUSE_SERREJ25);
                break;
#endif
        case    NAS_LMM_CAUSE_CS_NOT_AVAIL:
                NAS_EMM_SER_SERREJ18(pMsgStru);
                break;

        case    NAS_LMM_CAUSE_CS_DOMAIN_TMP_NOT_ALLOWED:
                NAS_EMM_SER_SERREJ39(pMsgStru);
                break;

        default:

                NAS_EMM_ServiceReqRejectOtherCause(pMsgStru);
                break;

    }


    return;

}
VOS_VOID NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm(VOS_VOID)
{
    NAS_LMM_PTL_TI_ENUM_UINT16  enPtlTimerId = NAS_LMM_STATE_TI_BUTT;

    if (NAS_EMM_YES == NAS_EMM_IsT3411orT3402Running(&enPtlTimerId))
    {
        if (NAS_EMM_YES == NAS_EMM_TAU_CanTriggerComTauWithIMSI())
        {
            NAS_EMM_TAU_LOG_NORM("NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm:Upt-MM state.");

            /* ��״̬ת����MS_REG + EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM״̬ */
            NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                        EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM,
                                        TI_NAS_EMM_STATE_NO_TIMER);
        }
        else
        {
            /* ��״̬ת����MS_REG + SS_REG_NORMAL_SERVICE״̬ */
            NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                        EMM_SS_REG_NORMAL_SERVICE,
                                        TI_NAS_EMM_STATE_NO_TIMER);
        }

    }
    else
    {
        if (NAS_EMM_YES == NAS_EMM_TAU_CanTriggerComTauWithIMSI())
        {
            NAS_EMM_TAU_LOG_NORM("NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm: with imsi attach");
            NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                        EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM,
                                        TI_NAS_EMM_STATE_NO_TIMER);

            NAS_EMM_SendIntraTauReq(    ID_APP_MM_MSG_TYPE_BUTT,
                                    NAS_LMM_INTRA_TAU_TYPE_UPDATE_MM);
            return;
        }

        NAS_EMM_TAU_LOG_NORM("NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm: normal service state.");
        /* ��״̬ת����MS_REG + SS_REG_NORMAL_SERVICE״̬ */
        NAS_EMM_TAUSER_FSMTranState(    EMM_MS_REG,
                                        EMM_SS_REG_NORMAL_SERVICE,
                                        TI_NAS_EMM_STATE_NO_TIMER);

    }
    return;

}

/* lihong00150010 emergency tau&service begin */

VOS_VOID NAS_EMM_TranStatePsNormalServiceOrPsLimitService(VOS_VOID)
{
    if (VOS_TRUE == NAS_EMM_IsEnterRegLimitService())
    {
        NAS_EMM_TAUSER_FSMTranState(    EMM_MS_REG,
                                        EMM_SS_REG_LIMITED_SERVICE,
                                        TI_NAS_EMM_STATE_NO_TIMER);
    }
    else
    {
        NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm();
    }
}
/* lihong00150010 emergency tau&service end */

VOS_VOID  NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgRrcRelInd( VOS_UINT32 ulCause)
{
    NAS_EMM_SER_LOG_INFO("NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgRrcRelInd is entered.");

    /*ֹͣSERVICE����*/
    NAS_EMM_SER_AbnormalOver();

    /* CSFB���̵Ĵ��� */
    if (VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
    {
        NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgRrcRelInd:CSFB");

        /*ת��EMM״̬��MS_REG+SS_REG_NORMAL_SERVICE*/
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_NORMAL_SERVICE, TI_NAS_EMM_STATE_NO_TIMER);
        if ((LRRC_LNAS_REL_CAUSE_L2GU_REDIRECT != ulCause) && (LRRC_LNAS_REL_CAUSE_CSFB_HIGH_PRIOR != ulCause))
        {
            /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
            NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);
        }

        /*send INTRA_CONN2IDLE_REQ����������״̬*/
        NAS_EMM_CommProcConn2Ilde();
        return;
    }
	/* lihong00150010 emergency tau&service begin */
    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgRrcRelInd:CAUSE_ESM_DATA_REQ_EMC");

        NAS_EMM_EmcPndReqSerAbnormalCommProc(EMM_SS_DEREG_NORMAL_SERVICE);

        /*send INTRA_CONN2IDLE_REQ����������״̬*/
        NAS_EMM_CommProcConn2Ilde();
        return;
    }
    /* lihong00150010 emergency tau&service end */

    /*upon different cause value, enter different dealing*/
    switch (ulCause)
    {
        case LRRC_LNAS_REL_CAUSE_LOAD_BALANCE_REQ:
            /*NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_RRC_REL_LOAD_BALANCE);*/
            NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
            NAS_LMM_SetEmmInfoTriggerTauRrcRel(NAS_EMM_TRIGGER_TAU_RRC_REL_LOAD_BALANCE);
            NAS_EMM_TAU_SaveEmmCollisionCtrl(  NAS_EMM_COLLISION_SERVICE);
            /*NAS_EMM_TAU_StartTAUREQ(             NAS_EMM_MSG_LOAD_BALANCING_TAU);*/
            /*
            NAS_EMM_TAUSER_FSMTranState(       EMM_MS_REG,
                                                EMM_SS_REG_PLMN_SEARCH,
                                                TI_NAS_EMM_STATE_NO_TIMER);
            */
            NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm();

            /*send INTRA_CONN2IDLE_REQ����������״̬*/
            NAS_EMM_CommProcConn2Ilde();
            break;

        case LRRC_LNAS_REL_CAUSE_CONN_FAIL:
            /* NAS_EMM_TAU_SaveEmmTAUStartCause(    NAS_EMM_TAU_START_CAUSE_RRC_REL_CONN_FAILURE); */
            NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
            NAS_LMM_SetEmmInfoTriggerTauRrcRel(NAS_EMM_TRIGGER_TAU_RRC_REL_CONN_FAILURE);
            /*NAS_EMM_TAU_SaveEmmCollisionCtrl(    NAS_EMM_COLLISION_SERVICE);*/
            /*NAS_EMM_TAU_StartTAUREQ(             NAS_EMM_MSG_LOAD_BALANCING_TAU);*/
            /*
            NAS_EMM_TAUSER_FSMTranState(       EMM_MS_REG,
                                                EMM_SS_REG_PLMN_SEARCH,
                                                TI_NAS_EMM_STATE_NO_TIMER);
            */
            NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm();

            /*send INTRA_CONN2IDLE_REQ����������״̬*/
            NAS_EMM_CommProcConn2Ilde();
            break;

        default:
            NAS_EMM_SER_RcvRrcRelInd();
            break;
    }

    return;
}
VOS_UINT32 NAS_EMM_MsSerInitSsWaitCnSerCnfMsgAuthFail(
                                                            VOS_UINT32  ulMsgId,
                                                            VOS_VOID   *pMsgStru)
{
    NAS_EMM_INTRA_AUTH_FAIL_STRU        *pMsgAuthFail   = (NAS_EMM_INTRA_AUTH_FAIL_STRU *)pMsgStru;
    VOS_UINT32                          ulCause;

    (VOS_VOID)ulMsgId;

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_MsSerInitSsWaitCnSerCnfMsgAuthFail is entered.");

    /*���ԭ��ֵ*/
    ulCause                                             =   pMsgAuthFail->ulCause;

    /*����ԭ��ֵ����*/
    if(NAS_EMM_AUTH_REJ_INTRA_CAUSE_NORMAL              ==  ulCause)
    {
        NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgAuthRej(      ulCause);

    }
    else
    {
        NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgRrcRelInd(    ulCause);
    }

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32  NAS_EMM_MsSerInitSsWaitCnSerCnfMsgAuthRej(
                                        VOS_UINT32  ulMsgId,
                                        const VOS_VOID   *pMsgStru)

{
    (VOS_VOID)ulMsgId;
    (VOS_VOID)pMsgStru;

    NAS_EMM_SER_LOG_INFO("NAS_EMM_MsSerInitSsWaitCnSerCnfMsgAuthRej enter!");

    NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgAuthRej(NAS_EMM_AUTH_REJ_INTRA_CAUSE_NORMAL);


    return NAS_LMM_MSG_HANDLED;

}


VOS_UINT32 NAS_EMM_MsSerInitSsWaitCnSerCnfMsgRrcRelInd(
                                                            VOS_UINT32  ulMsgId,
                                                            VOS_VOID   *pMsgStru)
{
    LRRC_LMM_REL_IND_STRU                 *pRrcRelInd     = (LRRC_LMM_REL_IND_STRU *)pMsgStru;
    VOS_UINT32                          ulCause;

    (VOS_VOID)ulMsgId;

    NAS_EMM_TAU_LOG_INFO("NAS_EMM_MsSerInitSsWaitCnSerCnfMsgRrcRelInd is entered.");

    /*���ԭ��ֵ*/
    ulCause                                             =   pRrcRelInd->enRelCause;

    NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgRrcRelInd(        ulCause);

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32 NAS_EMM_MsSerInitSsWaitCnSerCnfMsgTimer3417Exp(  VOS_UINT32  ulMsgId,
                                                             VOS_VOID   *pMsgStru
                                                          )
{
    VOS_UINT32                      ulRslt          = NAS_EMM_FAIL;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsSerInitSsWaitCnSerCnfMsgTimer3417Exp is entered.");

    /* ��������ָ��������, ״̬ƥ����,����ƥ��,�˳�*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_SER_INIT,EMM_SS_SER_WAIT_CN_SER_CNF,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_MsSerInitSsWaitCnSerCnfMsgTimer3417Exp ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*���SR����������SMS��������Ҫ�ظ�SMS����ʧ�ܣ��������SR�ķ���ԭ��*/
    if(NAS_EMM_SER_START_CAUSE_SMS_EST_REQ == NAS_EMM_SER_GetSerStartCause())
    {
        NAS_LMM_SndLmmSmsErrInd(LMM_SMS_ERR_CAUSE_OTHERS);
        NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_BUTT);
    }

    /*���ESM_DATA����*/
    NAS_EMM_SerClearEsmDataBuf();
    /*����SER�Ľ��Ϊ NAS_EMM_SER_RESULT_ABNORMAL*/
    /*NAS_EMM_SER_SaveSERresult(NAS_EMM_SER_RESULT_ABNORMAL);*/

    /*���ñ���MMC�����ԭ��ֵ*/
    /*NAS_EMM_SER_SaveEmmSERRejCause(NAS_LMM_CAUSE_NULL);*/

    /*����RRC�����ͷŵ�ԭ��*/


    /*Inform RABM that SER fail*/
    NAS_EMM_SER_SendRabmReestInd(EMM_ERABM_REEST_STATE_FAIL);
	/* lihong00150010 emergency tau&service begin */
    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_MsSerInitSsWaitCnSerCnfMsgTimer3417Exp: CAUSE_ESM_DATA_REQ_EMC.");

        NAS_EMM_EmcPndReqSerAbnormalCommProc(EMM_SS_DEREG_NORMAL_SERVICE);
    }
    else
    {
        NAS_EMM_TranStatePsNormalServiceOrPsLimitService();
    }
	/* lihong00150010 emergency tau&service end */
    /*��MRRC����NAS_EMM_MRRC_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);

    return NAS_LMM_MSG_HANDLED;

}

VOS_UINT32 NAS_EMM_MsSerInitSsWaitCnSerCnfMsgTimer3417ExtExp
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    VOS_UINT32                      ulRslt          = NAS_EMM_FAIL;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsSerInitSsWaitCnSerCnfMsgTimer3417Exp is entered.");

    /* ��������ָ��������, ״̬ƥ����,����ƥ��,�˳�*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_SER_INIT,EMM_SS_SER_WAIT_CN_SER_CNF,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_TAUSER_CHKFSMStateMsgp ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    NAS_EMM_SER_AbnormalOver();

	/* ״̬Ǩ�Ƶ�REG.NORMAL_SERVICE*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                            EMM_SS_REG_NORMAL_SERVICE,
                            TI_NAS_EMM_STATE_NO_TIMER);

    if (NAS_EMM_CSFB_ABORT_FLAG_VALID == NAS_EMM_SER_GetEmmSerCsfbAbortFlag())
    {

        if (NAS_EMM_CONN_DATA != NAS_EMM_GetConnState())
        {
            /* �����δ��ɵ�TAU������REG+NORMAL_SERVICE״̬���յ�ϵͳ��Ϣ���ٴ��� */
            NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
        }
    }
    /* ������ǽ���CSFB��MO CSFB��MM����ֹ��Ϣ,�����MMC�ϱ�SER����ͷŵȹ��� */
    else if ((VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb())
         && (VOS_FALSE == NAS_EMM_SER_IsMoCsfbProcedure()))
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_T3417EXT_TIME_OUT);
        if (NAS_EMM_CONN_DATA != NAS_EMM_GetConnState())
    	{
        	/* �����δ��ɵ�TAU������REG+NORMAL_SERVICE״̬���յ�ϵͳ��Ϣ���ٴ��� */
        	NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
    	}
    }
    else
    {
        /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
        NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);
        NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);
    }

    return NAS_LMM_MSG_HANDLED;

}


VOS_UINT32 NAS_EMM_MsSerInitSsWaitCNSerCnfMsgServiceReject(  VOS_UINT32  ulMsgId,
                                                                  VOS_VOID   *pMsgStru
                                                              )
{
    VOS_UINT32                      ulRslt          = NAS_EMM_FAIL;
    NAS_EMM_CN_SER_REJ_STRU         *pstserrej      = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsSerInitSsWaitCNSerCnfMsgServiceReject is entered.");

    /* ��������ָ��������, ״̬ƥ����,����ƥ��,�˳�*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_SER_INIT,EMM_SS_SER_WAIT_CN_SER_CNF,pMsgStru);

    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_TAUSER_CHKFSMStateMsgp ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*ֹͣT3416��ɾ��RAND,RES*/
    NAS_LMM_StopPtlTimer(                TI_NAS_EMM_PTL_T3416);
    NAS_EMM_SecuClearRandRes();

    #if (FEATURE_PTM == FEATURE_ON)
    NAS_LMM_ErrlogInfoProc(pMsgStru, LMM_ERR_LOG_SERVICE_PROCEDURE);
    #endif

    /*������Ϣ������*/
    pstserrej  =  (NAS_EMM_CN_SER_REJ_STRU*)pMsgStru;
    NAS_EMM_SER_RcvServiceRejectMsg(pstserrej);

    return NAS_LMM_MSG_HANDLED;
}
VOS_VOID NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgAuthRej(VOS_UINT32  ulCause)
{
    NAS_EMM_SER_LOG_INFO(              "NAS_EMM_MsSerInitSsWaitCnSerCnfProcMsgAuthRej is entered.");

    (VOS_VOID)ulCause;

    /*��ֹSER*/
    NAS_EMM_SER_AbnormalOver();
    NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_T3416);
    NAS_EMM_SecuClearRandRes();

    /* �����CSFB���̣��Ҳ��ǽ���CSFB�����MM����ֹ��Ϣ */
    if ((VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
        && (VOS_TRUE == NAS_EMM_SER_IsNotEmergencyCsfb()))
    {
        NAS_EMM_MmSendCsfbSerEndInd(MM_LMM_CSFB_SERVICE_RSLT_AUTH_REJ);
    }

    /* ɾ��GUTI����Զ�����EPS_LOC,������Ҫ������STATUS */
    /* set the EPS update status to EU3 ROAMING NOT ALLOWED */
    NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_ROAMING_NOT_ALLOWED_EU3;

    /*ɾ��GUTI,KSIasme,TAI list,GUTI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_DELETE_RPLMN);

    NAS_LMM_SetPsSimValidity(NAS_LMM_SIM_INVALID);

    /*USIM��Ч��ֱ���ػ���ο�*/
    /*�ݲ�����*/
	/* lihong00150010 emergency tau&service begin */
    if (NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
    {
        /* ��¼ATTACH����ԭ��ֵ */
        NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;

        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);
    }
	/* lihong00150010 emergency tau&service end */
    /*ת��EMM״̬��MS_DEREG+SS_DEREG_LIMITED_SERVICE*/
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_DEREG, EMM_SS_DEREG_NO_IMSI, TI_NAS_EMM_STATE_NO_TIMER);

    /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
    NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_AUTH_REJ);

    /*��LRRC����LRRC_LMM_NAS_INFO_CHANGE_REQЯ��USIM��״̬*/
    NAS_EMM_SendUsimStatusToRrc(LRRC_LNAS_USIM_PRESENT_INVALID);

    /*��MRRC����NAS_EMM_MRRC_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);


    return;
}
/*lint +e961*/
/*lint +e960*/

VOS_VOID NAS_EMM_SerbarCommProc(VOS_VOID)
{
    /*ֹͣSERVICE����*/
    NAS_EMM_SER_AbnormalOver();
    NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_WAIT_ACCESS_GRANT_IND, TI_NAS_EMM_STATE_NO_TIMER);

    /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
    NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_ACCESS_BARED);

    /*send INTRA_CONN2IDLE_REQ����������״̬*/
    NAS_EMM_CommProcConn2Ilde();

    return;
 }
VOS_VOID  NAS_EMM_SER_ProcMoCallAccessBar(VOS_VOID)

{
    NAS_EMM_SER_LOG_NORM("NAS_EMM_SER_ProcMoCallAccessBar is entered!");
    if((NAS_EMM_SER_START_CAUSE_RRC_PAGING == NAS_EMM_SER_GetEmmSERStartCause())
        || (NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
        || (NAS_EMM_SER_START_CAUSE_MO_EMERGENCY_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause()))
    {
       /*��ӦѰ����SERVICE���̲�����RRC_EST_ACCESS_BARRED_MO_CALL�����*/
        NAS_EMM_SER_LOG_WARN("NAS_EMM_SER_ProcMoCallAccessBar:Barred Type ERR!!");
        return;
    }

    NAS_EMM_SerbarCommProc();

    return;
}
VOS_VOID  NAS_EMM_SER_ProcBarAllAccessBar(VOS_VOID)

{
    NAS_EMM_SER_LOG_NORM("NAS_EMM_SER_ProcBarAllAccessBar is entered!");
    if (NAS_EMM_SER_START_CAUSE_MO_EMERGENCY_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
    {
        NAS_EMM_SER_LOG_WARN("NAS_EMM_SER_ProcBarAllAccessBar:emergency Barred Type ERR!!");
        return;
    }

    NAS_EMM_SerbarCommProc();

    return;
}


VOS_VOID  NAS_EMM_SER_ProcMoCsfbAccessBar(VOS_VOID)

{
    NAS_EMM_SER_LOG_NORM("NAS_EMM_SER_ProcMoCsfbAccessBar is entered!");
    if(NAS_EMM_SER_START_CAUSE_MO_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
    {
        /*ֹͣSERVICE����*/
        NAS_EMM_SER_AbnormalOver();

        /*�����MMC�ϱ������������Lģ����bar�����´���CSFB����*/
        NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG, EMM_SS_REG_WAIT_ACCESS_GRANT_IND, TI_NAS_EMM_STATE_NO_TIMER);

        /*send INTRA_CONN2IDLE_REQ����������״̬*/
        NAS_EMM_CommProcConn2Ilde();
    }
    return;
}
VOS_VOID  NAS_EMM_SER_ProcMoCallAndCsfbAccessBar(VOS_VOID)

{
    NAS_EMM_SER_LOG_NORM("NAS_EMM_SER_ProcMoCallAndCsfbAccessBar is entered!");
    if((NAS_EMM_SER_START_CAUSE_RRC_PAGING == NAS_EMM_SER_GetEmmSERStartCause())
        || (NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
        || (NAS_EMM_SER_START_CAUSE_MO_EMERGENCY_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause()))
    {
        /*��ӦѰ����SERVICE���̲�����RRC_EST_ACCESS_BARRED_MO_CALL_AND_CSFB�����*/
        NAS_EMM_SER_LOG_WARN("NAS_EMM_SER_ProcMoCallAndCsfbAccessBar:mo call and csfb Barred Type ERR!!");
        return;
    }
    NAS_EMM_SerbarCommProc();

    return;
}
VOS_VOID  NAS_EMM_SER_ProcCellSearchFail(VOS_VOID)

{
    NAS_EMM_SER_LOG_NORM("NAS_EMM_SER_ProcCellSearchFail is entered!");
    /*ֹͣSERVICE����*/
    NAS_EMM_SER_AbnormalOver();

    if (VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure())
    {
        /*��MMC����LMM_MMC_SERVICE_RESULT_IND��Ϣ*/
        NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);
    }

    NAS_EMM_TranStateRegNormalServiceOrRegUpdateMm();

    /*send INTRA_CONN2IDLE_REQ����������״̬*/
    NAS_EMM_CommProcConn2Ilde();

    return;
}
VOS_UINT32 NAS_EMM_MsSerInitSsWaitCnSerCnfMsgIntraConnectFailInd(   VOS_UINT32  ulMsgId,
                                                         VOS_VOID   *pMsgStru)
{

    VOS_UINT32                          ulRslt;
    NAS_EMM_MRRC_CONNECT_FAIL_IND_STRU *pMrrcConnectFailRelInd       = NAS_EMM_NULL_PTR;
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    NAS_EMM_SER_LOG_INFO( "NAS_EMM_MsSerInitSsWaitCnSerCnfMsgIntraConnectFailInd is entered.");
    pMrrcConnectFailRelInd              = (NAS_EMM_MRRC_CONNECT_FAIL_IND_STRU *)pMsgStru;

    /* ��������ָ��������, ״̬ƥ����,����ƥ��,�˳�*/
    ulRslt = NAS_EMM_SER_CHKFSMStateMsgp(EMM_MS_SER_INIT,EMM_SS_SER_WAIT_CN_SER_CNF,pMsgStru);
    if ( NAS_EMM_SUCC != ulRslt )
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_MsSerInitSsWaitCnSerCnfMsgIntraConnectFailInd ERROR !!");
        return NAS_LMM_MSG_DISCARD;
    }
    #if 0
    /*ֹͣSERVICE����*/
    NAS_EMM_SER_AbnormalOver();
    #endif

    /* lihong00150010 emergency tau&service begin */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC == NAS_EMM_SER_GetEmmSERStartCause())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        /*ֹͣSERVICE����*/
        NAS_EMM_SER_AbnormalOver();

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

            NAS_EMM_AdStateConvert( EMM_MS_DEREG,
                                    EMM_SS_DEREG_PLMN_SEARCH,
                                    TI_NAS_EMM_STATE_NO_TIMER);
        }
        else
        {
            /*����ESM��Ϣ*/
            /*lint -e960*/
            NAS_LMM_MEM_CPY(        (VOS_VOID*)NAS_EMM_GLO_AD_GetEsmMsgAddr(),
                                   &(pstEsmMsg->stEsmMsg),
                                    (pstEsmMsg->stEsmMsg.ulEsmMsgSize)+4);
            /*lint +e960*/
            NAS_EMM_StartEmergencyAttach();
        }

        return  NAS_LMM_MSG_HANDLED;
    }
    /* lihong00150010 emergency tau&service end */

    /*Process according different EST result*/
    switch(pMrrcConnectFailRelInd->enEstResult)
    {
        case    LRRC_EST_ACCESS_BARRED_MO_CALL:

                /*����bar���̺�����*/
                NAS_EMM_SetBarInfo(NAS_EMM_BAR_PROCEDURE_SERVICE, pMrrcConnectFailRelInd->enEstResult);

                NAS_EMM_SER_ProcMoCallAccessBar();

                break;
        case    LRRC_EST_ACCESS_BARRED_ALL:

                /*����bar���̺�����*/
                NAS_EMM_SetBarInfo(NAS_EMM_BAR_PROCEDURE_SERVICE, pMrrcConnectFailRelInd->enEstResult);

                NAS_EMM_SER_ProcBarAllAccessBar();
                break;

        case    LRRC_EST_ACCESS_BARRED_MO_CSFB:
                /*����bar���̺�����*/
                NAS_EMM_SetBarInfo(NAS_EMM_BAR_PROCEDURE_SERVICE, pMrrcConnectFailRelInd->enEstResult);

                NAS_EMM_SER_ProcMoCsfbAccessBar();
                break;

        case    LRRC_EST_ACCESS_BARRED_MO_CALL_AND_CSFB:

                /*����bar���̺�����*/
                NAS_EMM_SetBarInfo(NAS_EMM_BAR_PROCEDURE_SERVICE, pMrrcConnectFailRelInd->enEstResult);

                NAS_EMM_SER_ProcMoCallAndCsfbAccessBar();
                break;
                /* ����ʧ�ܣ�RRC���ڽ���С��������Ǩ�Ƶ�Reg.Plmn_Search*/
        case    LRRC_EST_CELL_SEARCHING:

                NAS_EMM_SER_ProcCellSearchFail();
                break;

        default:
                /*ֹͣSERVICE����*/
                NAS_EMM_SER_AbnormalOver();
                NAS_EMM_SER_RcvRrcRelInd();

                break;

    }
    return  NAS_LMM_MSG_HANDLED;
}
VOS_UINT32 NAS_EMM_SndServiceReqFailProc(VOS_VOID* pMsg,VOS_UINT32 *pulIsDelBuff)
{

    LRRC_LMM_DATA_CNF_STRU              *pstRrcMmDataCnf = VOS_NULL_PTR;
    NAS_EMM_MRRC_MGMT_DATA_STRU         *pEmmMrrcMgmtData = NAS_EMM_NULL_PTR;

    *pulIsDelBuff = VOS_TRUE;

    /*���״̬�Ƿ�ƥ�䣬����ƥ�䣬�˳�*/
    if (NAS_EMM_AD_CHK_STAT_INVALID(EMM_MS_SER_INIT,EMM_SS_SER_WAIT_CN_SER_CNF))
    {
        /*��ӡ������Ϣ*/
        NAS_EMM_SER_LOG_INFO("NAS_EMM_SndServiceReqFailProc: STATE ERR!");
        return  NAS_EMM_SUCC;
    }
    pstRrcMmDataCnf = (LRRC_LMM_DATA_CNF_STRU*) pMsg;

    pEmmMrrcMgmtData = NAS_EMM_FindMsgInDataReqBuffer(pstRrcMmDataCnf->ulOpId);

    if (NAS_EMM_NULL_PTR == pEmmMrrcMgmtData)
    {
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
            /*ֹͣT3417��ʱ��*/
            NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417);

            /*ֹͣT3440��ʱ��*/
            NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);

            /*ֹͣT3417ext��ʱ��*/
            NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

            /*������ʱ��3417*/
            NAS_LMM_StartStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417);

            /*ת��EMM״̬��MS_SER_INIT+SS_SER_WAIT_CN_CNF*/
            NAS_EMM_TAUSER_FSMTranState(EMM_MS_SER_INIT, EMM_SS_SER_WAIT_CN_SER_CNF, TI_NAS_EMM_STATE_SERVICE_T3417);

            /*��ϲ�����MRRC_DATA_REQ(SERVICE_REQ)*/
            NAS_EMM_SER_SendMrrcDataReq_ServiceReq();

            break;

        default:
            break;
        }

    return NAS_EMM_SUCC;
}
VOS_UINT32 NAS_EMM_SndExtendedServiceReqFailProc(VOS_VOID* pMsg,VOS_UINT32 *pulIsDelBuff)
{
    LRRC_LMM_DATA_CNF_STRU              *pstRrcMmDataCnf = VOS_NULL_PTR;
    NAS_EMM_MRRC_MGMT_DATA_STRU         *pEmmMrrcMgmtData = NAS_EMM_NULL_PTR;


    *pulIsDelBuff = VOS_TRUE;

    /*���״̬�Ƿ�ƥ�䣬����ƥ�䣬�˳�*/
    if (NAS_EMM_AD_CHK_STAT_INVALID(EMM_MS_SER_INIT,EMM_SS_SER_WAIT_CN_SER_CNF))
    {
        /*��ӡ������Ϣ*/
        NAS_EMM_SER_LOG_INFO("NAS_EMM_SndExtendedServiceReqFailProc: STATE ERR!");
        return  NAS_EMM_SUCC;
    }

    if (NAS_EMM_CSFB_ABORT_FLAG_VALID == NAS_EMM_SER_GetEmmSerCsfbAbortFlag())
    {
        /*��ӡ������Ϣ*/
        NAS_EMM_SER_LOG_INFO("NAS_EMM_SndExtendedServiceReqFailProc: ABORT!");
        return  NAS_EMM_SUCC;
    }

    pstRrcMmDataCnf = (LRRC_LMM_DATA_CNF_STRU*) pMsg;

    pEmmMrrcMgmtData = NAS_EMM_FindMsgInDataReqBuffer(pstRrcMmDataCnf->ulOpId);

    if (NAS_EMM_NULL_PTR == pEmmMrrcMgmtData)
    {
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
            /* ����SERVICE ���� */
            /*ֹͣT3417��ʱ��*/
            NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417);

            /*ֹͣT3440��ʱ��*/
            NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);

            /*ֹͣT3417ext��ʱ��*/
            NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

            /*������ʱ��3417*/
            NAS_LMM_StartStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

            /*ת��EMM״̬��MS_SER_INIT+SS_SER_WAIT_CN_CNF*/
            NAS_EMM_TAUSER_FSMTranState(EMM_MS_SER_INIT, EMM_SS_SER_WAIT_CN_SER_CNF, TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

            /*��ϲ�����MRRC_DATA_REQ(SERVICE_REQ)*/
            NAS_EMM_SER_SendMrrcDataReq_ExtendedServiceReq();

            break;

        default:
            break;
        }

    return NAS_EMM_SUCC;
}




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


