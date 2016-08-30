



/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/

#include    "NasEmmAttDetInclude.h"
#include    "NasLmmPubMPrint.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASEMMDETACH_C
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
NAS_EMM_DETACH_CTRL_STRU                g_stEmmDetCtrl;


/*****************************************************************************
  3 Function
*****************************************************************************/


/*lint -e960*/
/*lint -e961*/
VOS_VOID  NAS_EMM_DetachInit()
{
    NAS_EMM_DETACH_LOG_NORM("NAS_EMM_DetachInit                START INIT...");

    /* ��ʼ�� detachȫ�ֱ��� */
    NAS_LMM_MEM_SET(&g_stEmmDetCtrl, 0, sizeof(g_stEmmDetCtrl));

    NAS_EMM_GLO_AD_GetDetTypeMo()       = MMC_LMM_MO_DET_PS_ONLY;

    /*��ʼ�� UE ���� detach ��Ĭ������*/
    NAS_EMM_GLO_AD_GetDetMode()         = NAS_EMM_DETACH_MODE_NOT_SWITCH_OFF;

    return;
}


VOS_VOID  NAS_EMM_FreeDetDyn( VOS_VOID )
{
    /*��ʱ����ز���*/
    return;
}
VOS_VOID NAS_EMM_Detach_ClearResourse(VOS_VOID)
{

    /*��̬�ڴ��ͷ�*/
    NAS_EMM_FreeDetDyn();

    /* ֹͣDETACH��ʱ�� */
    NAS_LMM_StopStateTimer(          TI_NAS_EMM_T3421);

    /*����ֵ*/
    NAS_LMM_MEM_SET(NAS_EMM_GLO_AD_GetDetCtrlAddr(), 0, sizeof(NAS_EMM_DETACH_CTRL_STRU));

    NAS_EMM_GLO_AD_GetDetTypeMo()       = MMC_LMM_MO_DET_PS_ONLY;

    /*��ʼ�� UE ���� detach ��Ĭ������*/
    NAS_EMM_GLO_AD_GetDetMode()         = NAS_EMM_DETACH_MODE_NOT_SWITCH_OFF;

    return;
}


VOS_UINT32  NAS_EMM_IntraDetReqChk(const NAS_LMM_INTRA_DETACH_REQ_STRU *pMsgMsg)
{
    /*��Ϣ���ݼ��*/
    if (ID_NAS_LMM_INTRA_DETACH_REQ == pMsgMsg->ulMsgId)
    {
        return  NAS_EMM_PARA_VALID;
    }
    else
    {
        return  NAS_EMM_PARA_INVALID;
    }
}
VOS_VOID  NAS_EMM_SendDetRslt(MMC_LMM_DETACH_RSLT_ENUM_UINT32 ulAppRslt  )
{
    if (VOS_OK == NAS_EMM_CheckAppMsgPara(ID_MMC_LMM_DETACH_REQ))
    {
        NAS_EMM_DETACH_LOG_NORM("NAS_EMM_SendDetRslt: Send LMM_MMC_DETACH_CNF");
        NAS_EMM_AppSendDetCnf(ulAppRslt);
    }
    else
    {
        NAS_EMM_MmcSendDetIndLocal(MMC_LMM_L_LOCAL_DETACH_OTHERS);
    }
}
/*****************************************************************************
 Function Name   : NAS_EMM_GetDetachType
 Description     : ����DETACH REQ��Ϣʱ����ȡDETACH TYPE

         �û�DetachType                   UE MODE
                                PS                      CS+PS
         EPS detach             EPS DETACH              EPS DETACH
         IMSI detach            ��Ԥ�������Ѵ���        IMSI DETACH
         EPS/IMSI detach        EPS DETACH              EPS DETACH(��ע����ΪPS);
                                                        EPS/IMSI DETACH(��ע����Ϊ����)

 Input           : ��
 Output          : ��
 Return          : MMC_LMM_MO_DETACH_TYPE_ENUM_UINT32

 History         :
    1.lihong00150010      2011-09-29  Draft Enact

*****************************************************************************/
MMC_LMM_MO_DETACH_TYPE_ENUM_UINT32 NAS_EMM_GetDetachType( VOS_VOID )
{
    MMC_LMM_MO_DETACH_TYPE_ENUM_UINT32  enDetachType = MMC_LMM_MO_DETACH_TYPE_BUTT;

    /* ��ȡ�û������DETACH���� */
    enDetachType = NAS_EMM_GLO_AD_GetDetTypeMo();

    /* ���UEģʽΪPS���򷵻�����ΪEPS ONLY */
    if (NAS_EMM_NO == NAS_EMM_IsCsPsUeMode())
    {
        return MMC_LMM_MO_DET_PS_ONLY;
    }

    /* ����û������DETACH���Ͳ�ΪEPS/IMSI���򷵻��û������DETACH���� */
    if (MMC_LMM_MO_DET_CS_PS != enDetachType)
    {
        return enDetachType;
    }

    /*����û��������EPS/IMSI DETACH���ҵ�ǰע����ΪPS���򷵻�EPS ONLY;�����ǰ
      ע����ΪCS+PS���򷵻�EPS/IMSI;�����ǰע����ΪNULL������ʱ��ʱ�ȷ���EPS/IMSI��
      ���ٿ���UE�Ƿ�ԭ��ֵ#2�ܾ�����Ҳ���������յ�DETACH��Ϣ֮ǰ�û��Ƿ�������CS��ע�� */
    if (NAS_LMM_REG_DOMAIN_PS == NAS_LMM_GetEmmInfoRegDomain())
    {
        return MMC_LMM_MO_DET_PS_ONLY;
    }
    else
    {
        return MMC_LMM_MO_DET_CS_PS;
    }
}

/*****************************************************************************
 Function Name   : NAS_EMM_GetSwitchOffDetachType
 Description     : ��Ϊ�ػ����͵�DETACHʱ����ȡDETACH TYPE

 Input           : ��
 Output          : ��
 Return          : MMC_LMM_MO_DETACH_TYPE_ENUM_UINT32

 History         :
    1.lihong00150010      2012-02-02  Draft Enact

*****************************************************************************/
MMC_LMM_MO_DETACH_TYPE_ENUM_UINT32 NAS_EMM_GetSwitchOffDetachType( VOS_VOID )
{
    VOS_UINT32                          ulCurEmmStat;

    /*����û��������EPS/IMSI DETACH���ҵ�ǰע����ΪPS���򷵻�EPS ONLY;�����ǰ
      ע����ΪCS+PS���򷵻�EPS/IMSI;�����ǰע����ΪNULLʱ��Ŀǰֻ��ע������е�
      ����״̬�ᷢ�͹ػ�DETACH��Ϣ����˸���ATTACH��������дDETACH����*/
    if (NAS_LMM_REG_DOMAIN_PS == NAS_LMM_GetEmmInfoRegDomain())
    {
        return MMC_LMM_MO_DET_PS_ONLY;
    }

    if (NAS_LMM_REG_DOMAIN_CS_PS == NAS_LMM_GetEmmInfoRegDomain())
    {
        return MMC_LMM_MO_DET_CS_PS;
    }

    ulCurEmmStat = NAS_LMM_PUB_COMP_EMMSTATE(   NAS_EMM_CUR_MAIN_STAT,
                                                NAS_EMM_CUR_SUB_STAT);

    if ((ulCurEmmStat != NAS_LMM_PUB_COMP_EMMSTATE(    EMM_MS_REG_INIT,
                                                        EMM_SS_ATTACH_WAIT_CN_ATTACH_CNF))
        && (ulCurEmmStat != NAS_LMM_PUB_COMP_EMMSTATE(  EMM_MS_REG_INIT,
                                                        EMM_SS_ATTACH_WAIT_ESM_BEARER_CNF))
        && (ulCurEmmStat != NAS_LMM_PUB_COMP_EMMSTATE(  EMM_MS_REG_INIT,
                                                        EMM_SS_ATTACH_WAIT_RRC_DATA_CNF)))
    {
        NAS_EMM_DETACH_LOG_WARN("NAS_EMM_GetSwitchOffDetachType:Illegal state!");

        return MMC_LMM_MO_DET_PS_ONLY;
    }

    /* �����ATTACH�����еĹػ��������ATTACH������дDETACH���� */
    if (MMC_LMM_ATT_TYPE_COMBINED_EPS_IMSI == NAS_EMM_GLO_GetCnAttReqType())
    {
        return MMC_LMM_MO_DET_CS_PS;
    }
    else
    {
        return MMC_LMM_MO_DET_PS_ONLY;
    }
}
VOS_VOID    NAS_EMM_CompCnDetachReqMoNasMsg(    VOS_UINT8      *pucCnMsg,
                                                VOS_UINT32     *pulIndex)
{
    VOS_UINT32                          ulIndex      = 0;
    VOS_UINT8                           ucKsi;
    MMC_LMM_MO_DETACH_TYPE_ENUM_UINT32  enDetachType = MMC_LMM_MO_DETACH_TYPE_BUTT;
    VOS_UINT32                          ulLen        = 0;

    ulIndex                             = *pulIndex;

    /* ��� Protocol Discriminator + Security header type*/
    pucCnMsg[ulIndex++]                 = EMM_CN_MSG_PD_EMM;

    /* ��� Detach request message identity */
    pucCnMsg[ulIndex++]                 = NAS_EMM_CN_MT_DETACH_REQ_MT;

    /* lihong00150010 swtichoff begin */
    /*���Detach type*/
    if (NAS_EMM_DETACH_MODE_SWITCH_OFF == NAS_EMM_GLO_AD_GetDetMode())
    {
        enDetachType = NAS_EMM_GetSwitchOffDetachType();
    }
    else
    {
        enDetachType = NAS_EMM_GetDetachType();
    }
    pucCnMsg[ulIndex]                   = (VOS_UINT8)(NAS_EMM_GLO_AD_GetDetMode() |
                                                      enDetachType);
    /* lihong00150010 swtichoff end */
    /*���KSIasme*/
    ucKsi = NAS_EMM_SecuGetKSIValue();
    pucCnMsg[ulIndex++] |= (ucKsi & 0x0F)<<4;

    /*���GUTI��IMSI*/
    if (NAS_EMM_AD_BIT_SLCT             == NAS_EMM_GLO_AD_OP_GUTI())
    {
        /*���GUTI*/
        /*============ GUTI ============ begin */
        pucCnMsg[ulIndex++]             = NAS_EMM_GLO_AD_GetLen();
        pucCnMsg[ulIndex++]             =(NAS_EMM_GLO_AD_GetOeToi()|
                                          NAS_EMM_HIGH_HALF_BYTE_F);

        NAS_LMM_MEM_CPY(             &(pucCnMsg[ulIndex]),
                                        NAS_EMM_GLO_AD_GetGutiPlmn(),
                                        NAS_EMM_AD_LEN_PLMN_ID);
        ulIndex                         += NAS_EMM_AD_LEN_PLMN_ID;
        pucCnMsg[ulIndex++]             = NAS_EMM_GLO_AD_GetMmeGroupId();
        pucCnMsg[ulIndex++]             = NAS_EMM_GLO_AD_GetMMeGroupIdCnt();
        pucCnMsg[ulIndex++]             = NAS_EMM_GLO_AD_GetMMeCode();

        NAS_LMM_MEM_CPY(             &(pucCnMsg[ulIndex]),
                                        NAS_EMM_GLO_AD_GetMTmsiAddr(),
                                        sizeof(NAS_EMM_MTMSI_STRU));
        ulIndex                         += sizeof(NAS_EMM_MTMSI_STRU);


        /*============ GUTI ============ end */
    }/* lihong00150010 emergency tau&service begin */
    else if (NAS_EMM_AD_BIT_SLCT == NAS_EMM_GLO_AD_OP_IMSI())
    {/* lihong00150010 emergency tau&service end */
        /*���IMSI*/
        /*============ IMSI ============ begin */

        NAS_LMM_MEM_CPY(             &(pucCnMsg[ulIndex]),
                                        NAS_EMM_GLO_AD_GetImsi(),
                                        NAS_EMM_AD_LEN_CN_MSG_IMSI);

        ulIndex                         += NAS_EMM_AD_LEN_CN_MSG_IMSI;
        /*============ IMSI ============ end */

    }/* lihong00150010 emergency tau&service begin */
    else
    {
        NAS_EMM_CompImei(&(pucCnMsg[ulIndex]),&ulLen);
        ulIndex                         += ulLen;
    }
	/* lihong00150010 emergency tau&service end */
    *pulIndex = ulIndex;

    return;

}
VOS_VOID    NAS_EMM_CompCnDetachReqMo(  NAS_EMM_MRRC_DATA_REQ_STRU *pDetReqMoMsg)
{
    VOS_UINT32                          ulIndex;
    VOS_UINT32                          ulDataReqLenNoHeader;
    NAS_EMM_MRRC_DATA_REQ_STRU         *pTempDetReqMsg = pDetReqMoMsg;

    if (NAS_EMM_NULL_PTR == pDetReqMoMsg)
    {
        /*��ӡ����*/
        NAS_EMM_ATTACH_LOG_ERR("NAS_EMM_CompCnDetachReqMo: Mem Alloc Err!");
        return;
    }

    ulIndex = 0;
    /*����DETACH REQ�տ���Ϣ*/
    NAS_EMM_CompCnDetachReqMoNasMsg(pTempDetReqMsg->stNasMsg.aucNasMsg, &ulIndex);

    /*�ڲ���Ϣ���ȼ���*/
    ulDataReqLenNoHeader             = NAS_EMM_CountMrrcDataReqLen(ulIndex);

    if ( NAS_EMM_INTRA_MSG_MAX_SIZE < ulDataReqLenNoHeader )
    {
        /* ��ӡ������Ϣ */
        NAS_LMM_PUBM_LOG_ERR("NAS_EMM_CompCnDetachReqMo, Size error");
        return ;
    }

    /*�����Ϣ����*/
    pTempDetReqMsg->stNasMsg.ulNasMsgSize    = ulIndex;

    /*�����Ϣͷ*/
    NAS_EMM_COMP_AD_INTRA_MSG_HEADER(pTempDetReqMsg, ulDataReqLenNoHeader);

    /*�����ϢID*/
    pTempDetReqMsg->ulMsgId                  = ID_NAS_LMM_INTRA_MRRC_DATA_REQ;

    pTempDetReqMsg->enDataCnf                = LRRC_LMM_DATA_CNF_NEED;

    /*�����Ϣ����*/
    pTempDetReqMsg->enEstCaue                = LRRC_LNAS_EST_CAUSE_MO_SIGNALLING;
    pTempDetReqMsg->enCallType               = LRRC_LNAS_CALL_TYPE_ORIGINATING_SIGNALLING;
    pTempDetReqMsg->enEmmMsgType             = NAS_EMM_MSG_DETACH_REQ;

    return;
}




VOS_VOID    NAS_EMM_MrrcSendDetReqMo()
{
    NAS_EMM_MRRC_DATA_REQ_STRU          *pIntraMsg;
    NAS_MSG_STRU                        *pstNasMsg;
    MMC_LMM_MO_DETACH_TYPE_ENUM_UINT32  enDetachType = MMC_LMM_MO_DETACH_TYPE_BUTT;

    /*��ӡ����ú���*/
    NAS_EMM_DETACH_LOG_INFO("NAS_EMM_MrrcSendDetReqMo is entered");

    /*����С��Ϣ���ȣ�������Ϣ�ڴ�,��Ҫ�ǿ��Ƿ�����пռ�*/
    pIntraMsg = (VOS_VOID *) NAS_LMM_MEM_ALLOC(NAS_EMM_INTRA_MSG_MAX_SIZE);

    /*�ж�����������ʧ�ܴ�ӡ�����˳�*/
    if (NAS_EMM_NULL_PTR == pIntraMsg)
    {
        /*��ӡ����*/
        NAS_EMM_ATTACH_LOG_ERR("NAS_EMM_MrrcSendDetReqMo: MSG ALLOC ERR!");
        return;

    }

    /*���췢�������DETACH REQUEST ��Ϣ*/
    NAS_EMM_CompCnDetachReqMo(pIntraMsg);

    /* ���������TAU����֪ͨMM����MM IMSI DETACH PENDING״̬ */
    enDetachType = NAS_EMM_GetDetachType();
    if ((NAS_EMM_DETACH_MODE_NOT_SWITCH_OFF == NAS_EMM_GLO_AD_GetDetMode())
        && ((MMC_LMM_MO_DET_CS_PS == enDetachType)
            || (MMC_LMM_MO_DET_CS_ONLY == enDetachType)))
    {
        NAS_EMM_SendMmCombinedStartNotifyReq(MM_LMM_COMBINED_DETACH);
    }

    /*��OM���Ϳտ���ϢDETACH REQUEST*/
    pstNasMsg = (NAS_MSG_STRU *)(&(pIntraMsg->stNasMsg));
    NAS_LMM_SendOmtAirMsg(NAS_EMM_OMT_AIR_MSG_UP, NAS_EMM_DETACH_REQ_MO, pstNasMsg);
    NAS_LMM_SendOmtKeyEvent(             EMM_OMT_KE_DETACH_REQ_MO);

    /*��MRRC����DETACH REQUEST ��Ϣ*/
    NAS_EMM_SndUplinkNasMsg(          	pIntraMsg);

    NAS_LMM_MEM_FREE(pIntraMsg);
    return;

}

VOS_VOID NAS_EMM_SendImsiDetachReqMo( VOS_VOID )
{
    NAS_LMM_FSM_STATE_STRU               stEmmState;

    NAS_LMM_PUBM_LOG_NORM("NAS_EMM_SendImsiDetachReqMo is enter.");

    /*������ʱ��T3421*/
    NAS_LMM_StartStateTimer(         TI_NAS_EMM_T3421);

    /*�޸�״̬��������״̬EMM_MS_REG��״̬EMM_SS_REG_IMSI_DETACH_WATI_CN_DETACH_CNF*/
    stEmmState.enFsmId                    = NAS_LMM_PARALLEL_FSM_EMM;
    stEmmState.enMainState                = EMM_MS_REG;
    stEmmState.enSubState                 = EMM_SS_REG_IMSI_DETACH_WATI_CN_DETACH_CNF;
    stEmmState.enStaTId                   = TI_NAS_EMM_T3421;
    NAS_LMM_StaTransProc(stEmmState);

    /*��MRRC����DETACH REQUEST��Ϣ*/
    NAS_EMM_MrrcSendDetReqMo();

    return;
}
VOS_VOID  NAS_EMM_SendDetachReqMo(VOS_VOID)
{

    NAS_LMM_FSM_STATE_STRU               stEmmState;

    NAS_LMM_PUBM_LOG_NORM("NAS_EMM_SendDetachReqMo is enter.");


    /*������ʱ��T3421*/
    NAS_LMM_StartStateTimer(         TI_NAS_EMM_T3421);

    /*�޸�״̬��������״̬DEREG_INIT��״̬DETACH_WAIT_CN_DETACH_CNF*/
    stEmmState.enFsmId                    = NAS_LMM_PARALLEL_FSM_EMM;
    stEmmState.enMainState                = EMM_MS_DEREG_INIT;
    stEmmState.enSubState                 = EMM_SS_DETACH_WAIT_CN_DETACH_CNF;
    stEmmState.enStaTId                   = TI_NAS_EMM_T3421;
    NAS_LMM_StaTransProc(stEmmState);

    /*��MRRC����DETACH REQUEST��Ϣ*/
    NAS_EMM_MrrcSendDetReqMo();
    return;
}
VOS_VOID NAS_EMM_RrcRelCauseCnDetachSrcClrAttemptToAtt(VOS_VOID)
{
    NAS_EMM_DETACH_LOG_INFO("NAS_EMM_RrcRelCauseCnDetachSrcClrAttemptToAtt is entered!");

    /*delete list of equivalent PLMNs, */
    /*NAS_EMM_INFO_DELETE_EPLMN_LIST();*/

    /* ɾ��GUTI����Զ�����EPS_LOC,������Ҫ������STATUS */
    /*shall set the update status to EU2 NOT UPDATED */
    NAS_EMM_MAIN_CONTEXT_SET_AUX_UPDATE_STAE(EMM_US_NOT_UPDATED_EU2);

    /*ɾ��GUTI,KSIasme,TAI list,GUTI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_NOT_DELETE_RPLMN);

    /*and shall start timer T3402. */
    NAS_LMM_StartPtlTimer(TI_NAS_EMM_PTL_T3402);

    /*clear Attach Attempt Counter*/
    /*NAS_EMM_GLO_AD_GetAttAtmpCnt()      = 0;*/

    return;
}


VOS_VOID    NAS_EMM_SendDetachForPowerOffReq(VOS_VOID)
{
    /* ����DETACH����Ϊ�ػ�DETACH */
    NAS_EMM_GLO_AD_GetDetMode()         = NAS_EMM_DETACH_MODE_SWITCH_OFF;

    /* ��MRRC����DETACH REQUEST��Ϣ */
    NAS_EMM_MrrcSendDetReqMo();
}


VOS_VOID    NAS_EMM_DetProcEnterForbTA(VOS_VOID)
{
    /* ��ֹDETACH����*/
    NAS_LMM_StopStateTimer(           TI_NAS_EMM_T3421);

    /*��ESM����ID_EMM_ESM_STATUS_IND��Ϣ*/
    NAS_EMM_EsmSendStatResult(          EMM_ESM_ATTACH_STATUS_DETACHED);
    /* lihong00150010 emergency tau&service begin */
    if (VOS_TRUE == NAS_EMM_GLO_AD_GetUsimPullOutFlag())
    {
        /*״̬ת����֪ͨMMC����Ч*/
        NAS_EMM_ProcLocalNoUsim();
    }/* lihong00150010 emergency tau&service end */
    else
    {
        /* ״̬Ǩ��:Dereg.Limite_Service*/
        NAS_EMM_AdStateConvert(     EMM_MS_DEREG,
                                    EMM_SS_DEREG_LIMITED_SERVICE,
                                    TI_NAS_EMM_STATE_NO_TIMER);
        /* ����DETACH*/
        NAS_LMM_DeregReleaseResource();
    }

    /*��MMC����LMM_MMC_DETACH_CNF��Ϣ*/
    NAS_EMM_SendDetRslt(MMC_LMM_DETACH_RSLT_SUCCESS);

    NAS_EMM_RelReq(                  NAS_LMM_NOT_BARRED);

    return;
}
/* lihong00150010 emergency tau&service begin */
/*****************************************************************************
 Function Name   : NAS_EMM_ImsiDetProcEnterForbTA
 Description     : REG.IMSI_DETACH_INIT״̬���յ�SYS_INFO����Я����ֹ��Ϣ����ֹDETACH
                   ���̣��ͷ���·
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010     2011-10-18  Draft Enact
    2.lihong00150010     2012-12-19  Modify:Emergency

*****************************************************************************/
VOS_VOID    NAS_EMM_ImsiDetProcEnterForbTA( VOS_VOID )
{
    /* ��ֹDETACH����*/
    NAS_LMM_StopStateTimer(           TI_NAS_EMM_T3421);

    /* ����ע����ΪPS */
    NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_PS);

    NAS_EMM_AdStateConvert(     EMM_MS_REG,
                                EMM_SS_REG_LIMITED_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /*��MMC����LMM_MMC_DETACH_CNF��Ϣ*/
    NAS_EMM_SendDetRslt(MMC_LMM_DETACH_RSLT_SUCCESS);

    /* IMSI DETACH���ͷ���Դ */
    NAS_LMM_ImsiDetachReleaseResource();

    NAS_EMM_RelReq(                  NAS_LMM_NOT_BARRED);

    return;
}
/* lihong00150010 emergency tau&service end */

VOS_VOID  NAS_EMM_MsDrgInitSsWtCnDetCnfProcMsgRrcRelInd( VOS_UINT32 ulCause)
{
    /*��ӡ����ú���*/
    NAS_EMM_DETACH_LOG_INFO("NAS_EMM_MsDrgInitSsWtCnDetCnfProcMsgRrcRelInd is entered");

    (VOS_VOID)(                             ulCause);

    /*ֹͣ��ʱ��T3421*/
    NAS_LMM_StopStateTimer(              TI_NAS_EMM_T3421);

    /*��ESM����ID_EMM_ESM_STATUS_IND��Ϣ*/
     NAS_EMM_EsmSendStatResult(       EMM_ESM_ATTACH_STATUS_DETACHED);
    /* lihong00150010 emergency tau&service begin */
    if (VOS_TRUE == NAS_EMM_GLO_AD_GetUsimPullOutFlag())
    {/* lihong00150010 emergency tau&service end */
        NAS_EMM_DETACH_LOG_NORM("NAS_EMM_MsDrgInitSsWtCnDetCnfProcMsgRrcRelInd: No USIM");

        NAS_EMM_ProcLocalNoUsim();

        NAS_EMM_SendDetRslt(MMC_LMM_DETACH_RSLT_SUCCESS);

        /*send INTRA_CONN2IDLE_REQ����������״̬*/
        NAS_EMM_CommProcConn2Ilde();

        return;
    }

    /* �п����������ͷ�ԭ��ֵ����ɱ���DETACH*/

    /*�޸�״̬��������״̬DEREG��״̬DEREG_NORMAL_SERVICE*/
    NAS_EMM_AdStateConvert(     EMM_MS_DEREG,
                                EMM_SS_DEREG_NORMAL_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);

    NAS_EMM_SendDetRslt(MMC_LMM_DETACH_RSLT_SUCCESS);

    /* ����DETACH�ͷ���Դ:��̬�ڴ桢����ֵ */
    NAS_LMM_DeregReleaseResource();

    /*send INTRA_CONN2IDLE_REQ����������״̬*/
    NAS_EMM_CommProcConn2Ilde();

    return;
}

VOS_UINT32  NAS_EMM_MsDrgInitSsWtCnDetCnfMsgAuthFail(
                                                VOS_UINT32  ulMsgId,
                                                VOS_VOID   *pMsgStru)
{

    NAS_EMM_INTRA_AUTH_FAIL_STRU        *pMsgAuthFail   = (NAS_EMM_INTRA_AUTH_FAIL_STRU *)pMsgStru;
    VOS_UINT32                          ulCause;

    (VOS_VOID)(ulMsgId);
    NAS_EMM_DETACH_LOG_INFO("NAS_EMM_MsDrgInitSsWtCnDetCnfMsgAuthFail is entered.");

    /*���ԭ��ֵ*/
    ulCause                                             =   pMsgAuthFail->ulCause;

    /*����ԭ��ֵ����*/
    if(NAS_EMM_AUTH_REJ_INTRA_CAUSE_NORMAL              ==  ulCause)
    {
        NAS_EMM_ProcDetachAuthRej(                          ulCause);
    }
    else
    {
        NAS_EMM_MsDrgInitSsWtCnDetCnfProcMsgRrcRelInd(      ulCause);
    }

    return   NAS_LMM_MSG_HANDLED;
}

/*****************************************************************************
 Function Name   : NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgAuthFail
 Description     : ��״̬EMM_MS_REG+��״̬EMM_SS_REG_IMSI_DETACH_WATI_CN_DETACH_CNF���յ�AUTH FAIL��Ϣ
                   ��AUTH������,�յ�RRC_REL_IND��CN_AUTH_REJ,AUTHģ�鶼��ת��AUTH FAIL
                   �������յ�AUTH_FAILʱ�ᴦ��RRC_REL_IND��CN_AUTH_REJ������Ϣ
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2011-09-28  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgAuthFail
(
    VOS_UINT32                          ulMsgId,
    const VOS_VOID                     *pMsgStru
)
{

    NAS_EMM_INTRA_AUTH_FAIL_STRU        *pMsgAuthFail   = (NAS_EMM_INTRA_AUTH_FAIL_STRU *)pMsgStru;
    VOS_UINT32                          ulCause;

    NAS_EMM_DETACH_LOG2_INFO("NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgAuthFail is entered",
                                                ulMsgId,
                                                pMsgStru);

    /*���ԭ��ֵ*/
    ulCause = pMsgAuthFail->ulCause;

    /*����ԭ��ֵ����*/
    if(NAS_EMM_AUTH_REJ_INTRA_CAUSE_NORMAL ==  ulCause)
    {
        NAS_EMM_ProcDetachAuthRej(ulCause);
    }
    else
    {
        NAS_EMM_ProcMsRegImsiDetachInitMsgRrcRelInd(ulCause);
    }

    return   NAS_LMM_MSG_HANDLED;
}



VOS_UINT32  NAS_EMM_MsDrgInitSsWtCnDetCnfMsgRrcRelInd(
                                                VOS_UINT32  ulMsgId,
                                                VOS_VOID   *pMsgStru)
{

    LRRC_LMM_REL_IND_STRU                 *pMsgRrcRelInd  = (LRRC_LMM_REL_IND_STRU *)pMsgStru;
    VOS_UINT32                          ulCause;

    (VOS_VOID)(ulMsgId);
    NAS_EMM_DETACH_LOG_INFO("NAS_EMM_MsDrgInitSsWtCnDetCnfMsgRrcRelInd is entered.");

    /*���ԭ��ֵ*/
    ulCause                                             =   pMsgRrcRelInd->enRelCause;

    NAS_EMM_MsDrgInitSsWtCnDetCnfProcMsgRrcRelInd(          ulCause);

    return   NAS_LMM_MSG_HANDLED;
}
VOS_UINT32 NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgIntraConnectFailInd
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    NAS_EMM_MRRC_CONNECT_FAIL_IND_STRU         *pMrrcConnectFailInd = NAS_EMM_NULL_PTR;

    NAS_EMM_DETACH_LOG2_INFO("NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgIntraConnectFailInd is entered",
                                                ulMsgId,
                                                pMsgStru);
    pMrrcConnectFailInd = (NAS_EMM_MRRC_CONNECT_FAIL_IND_STRU *)pMsgStru;

    /*check the input ptr*/
    if (NAS_EMM_NULL_PTR == pMsgStru)
    {
        NAS_EMM_DETACH_LOG_WARN("NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgIntraConnectFailInd: NULL PTR!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*check current state*/
    if (NAS_EMM_AD_CHK_STAT_INVALID(EMM_MS_REG,EMM_SS_REG_IMSI_DETACH_WATI_CN_DETACH_CNF))
    {
        NAS_EMM_DETACH_LOG_WARN("NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgIntraConnectFailInd: STATE ERR!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*ֹͣ��ʱ��T3421*/
    NAS_LMM_StopStateTimer(              TI_NAS_EMM_T3421);

    /* ����ע����ΪPS */
    NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_PS);

    /*�޸�״̬��������״̬REG��״̬EMM_SS_REG_NORMAL_SERVICE*/
    NAS_EMM_AdStateConvert(             EMM_MS_REG,
                                        EMM_SS_REG_NORMAL_SERVICE,
                                        TI_NAS_EMM_STATE_NO_TIMER);
    if ((LRRC_EST_ACCESS_BARRED_MO_SIGNAL == pMrrcConnectFailInd->enEstResult) ||
        (LRRC_EST_ACCESS_BARRED_ALL == pMrrcConnectFailInd->enEstResult))
    {
        /*��MMC����LMM_MMC_DETACH_CNF��Ϣ*/
        NAS_EMM_SendDetRslt(MMC_LMM_DETACH_RSLT_ACCESS_BARED);
    }
    else
    {
        /*��MMC���ͱ���LMM_MMC_DETACH_IND��Ϣ*/
        NAS_EMM_SendDetRslt(MMC_LMM_DETACH_RSLT_SUCCESS);
    }

    /* IMSI DETACH���ͷ���Դ */
    NAS_LMM_ImsiDetachReleaseResource();

    /*send INTRA_CONN2IDLE_REQ����������״̬*/
    NAS_EMM_CommProcConn2Ilde();

    return   NAS_LMM_MSG_HANDLED;
}
VOS_UINT32  NAS_EMM_MsDrgInitSsWtCnDetCnfMsgIntraConnectFailInd(
                                                VOS_UINT32  ulMsgId,
                                                VOS_VOID   *pMsgStru)
{

    NAS_EMM_MRRC_CONNECT_FAIL_IND_STRU         *pMrrcConnectFailInd = NAS_EMM_NULL_PTR;

    NAS_EMM_DETACH_LOG2_INFO("NAS_EMM_MsDrgInitSsWtCnDetCnfMsgIntraConnectFailInd is entered",
                                                ulMsgId,
                                                pMsgStru);
    pMrrcConnectFailInd = (NAS_EMM_MRRC_CONNECT_FAIL_IND_STRU *)pMsgStru;
    /*check the input ptr*/
    if (NAS_EMM_NULL_PTR == pMsgStru)
    {
        NAS_EMM_DETACH_LOG_WARN("NAS_EMM_MsDrgInitSsWtCnDetCnfMsgIntraConnectFailInd: NULL PTR!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*check current state*/
    if (NAS_EMM_AD_CHK_STAT_INVALID(EMM_MS_DEREG_INIT,EMM_SS_DETACH_WAIT_CN_DETACH_CNF))
    {
        NAS_EMM_DETACH_LOG_WARN("NAS_EMM_MsDrgInitSsWtCnDetCnfMsgIntraConnectFailInd: STATE ERR!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*ֹͣ��ʱ��T3421*/
    NAS_LMM_StopStateTimer(              TI_NAS_EMM_T3421);

    /*��ESM����ID_EMM_ESM_STATUS_IND��Ϣ*/
    NAS_EMM_EsmSendStatResult(          EMM_ESM_ATTACH_STATUS_DETACHED);
    /* lihong00150010 emergency tau&service begin */
    if (VOS_TRUE != NAS_EMM_GLO_AD_GetUsimPullOutFlag())
    {/* lihong00150010 emergency tau&service end */
        /*�޸�״̬��������״̬DEREG��״̬DEREG_NORMAL_SERVICE*/
        NAS_EMM_AdStateConvert(             EMM_MS_DEREG,
                                            EMM_SS_DEREG_NORMAL_SERVICE,
                                            TI_NAS_EMM_STATE_NO_TIMER);
    }
    else
    {
        /*NAS_EMM_ProcLocalNoUsim();*/
        /*�޸�״̬��������״̬DEREG��״̬DEREG_NORMAL_SERVICE*/
        NAS_EMM_AdStateConvert(             EMM_MS_DEREG,
                                            EMM_SS_DEREG_NO_IMSI,
                                            TI_NAS_EMM_STATE_NO_TIMER);

        /* ��MMC�ظ�LMM_MMC_USIM_STATUS_CNF */
        NAS_EMM_SendMmcUsimStatusCnf();
		/* lihong00150010 emergency tau&service begin */
        /* ����ο���ʶ */
        NAS_EMM_GLO_AD_GetUsimPullOutFlag() = VOS_FALSE;
		/* lihong00150010 emergency tau&service end */
    }

    if ((LRRC_EST_ACCESS_BARRED_MO_SIGNAL == pMrrcConnectFailInd->enEstResult) ||
        (LRRC_EST_ACCESS_BARRED_ALL == pMrrcConnectFailInd->enEstResult))
    {
        /*��MMC����LMM_MMC_DETACH_CNF��Ϣ*/
        /*NAS_EMM_AppSendDetCnf(MMC_LMM_DETACH_RSLT_ACCESS_BARED);*/
        NAS_EMM_SendDetRslt(MMC_LMM_DETACH_RSLT_ACCESS_BARED);
    }
    else
    {
        /*��MMC���ͱ���LMM_MMC_DETACH_IND��Ϣ*/
        /*NAS_EMM_MmcSendDetIndLocal();*/
        NAS_EMM_SendDetRslt(MMC_LMM_DETACH_RSLT_SUCCESS);
    }

   /* ����DETACH�ͷ���Դ:��̬�ڴ桢����ֵ */
   NAS_LMM_DeregReleaseResource();

   /*send INTRA_CONN2IDLE_REQ����������״̬*/
   NAS_EMM_CommProcConn2Ilde();
   return   NAS_LMM_MSG_HANDLED;
}
VOS_UINT32  NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgT3421Exp
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    /*��ӡ����ú���*/
    NAS_EMM_DETACH_LOG2_INFO("NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgT3421Exp",
                                                    ulMsgId,
                                                    pMsgStru);

    /*���״̬�Ƿ�ƥ�䣬����ƥ�䣬�˳�*/
    if (NAS_EMM_AD_CHK_STAT_INVALID(EMM_MS_REG,EMM_SS_REG_IMSI_DETACH_WATI_CN_DETACH_CNF))
    {
        /*��ӡ������Ϣ*/
        NAS_EMM_DETACH_LOG_WARN("NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgT3421Exp: STATE ERR!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*Detach attempt counter��1*/
    NAS_EMM_GLO_AD_GetDetAtmpCnt()++;

    /*�ж�Detach attempt counter����ΪС��5�ʹ��ڵ���5����*/
    if (NAS_EMM_GLO_AD_GetDetAtmpCnt() < 5)
    {
        /*����DETACH REQUEST��Ϣ*/
        NAS_EMM_SendImsiDetachReqMo();

        return NAS_LMM_MSG_HANDLED;
    }

    NAS_EMM_DETACH_LOG_INFO("NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgT3421Exp 5 times");

    NAS_EMM_GLO_AD_GetDetAtmpCnt()  = 0;

    /* ����ע����ΪPS */
    NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_PS);

    /* �����ǰ����������̬����ֱ��ת��REG+NORMAL_SERVIC̬��������ͷ�RRC��·
       ����ת��REG+NORMAL_SERVIC̬ */
    if (NAS_EMM_CONN_DATA == NAS_EMM_GetConnState())
    {
        NAS_EMM_AdStateConvert(             EMM_MS_REG,
                                            EMM_SS_REG_NORMAL_SERVICE,
                                            TI_NAS_EMM_STATE_NO_TIMER);

        /*��MMC���ͱ���LMM_MMC_DETACH_IND��Ϣ*/
        NAS_EMM_SendDetRslt(MMC_LMM_DETACH_RSLT_SUCCESS);

        /* IMSI DETACH���ͷ���Դ */
        NAS_LMM_ImsiDetachReleaseResource();

        return NAS_LMM_MSG_HANDLED;
    }

    NAS_EMM_AdStateConvert( EMM_MS_REG,
                            EMM_SS_REG_NORMAL_SERVICE,
                            TI_NAS_EMM_STATE_NO_TIMER);

    NAS_EMM_SendDetRslt(MMC_LMM_DETACH_RSLT_SUCCESS);

    /*��MRRC����NAS_EMM_MRRC_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);

    return  NAS_LMM_MSG_HANDLED;
}
VOS_UINT32  NAS_EMM_MsDrgInitSsWtCnDetCnfMsgT3421Exp( VOS_UINT32  ulMsgId,
                                                      VOS_VOID   *pMsgStru )
{
    /*��ӡ����ú���*/
    NAS_EMM_DETACH_LOG2_INFO("NAS_EMM_MsDrgInitSsWtCnDetCnfMsgT3421Exp",
                                                    ulMsgId,
                                                    pMsgStru);

    /*���״̬�Ƿ�ƥ�䣬����ƥ�䣬�˳�*/
    if (NAS_EMM_AD_CHK_STAT_INVALID(EMM_MS_DEREG_INIT,EMM_SS_DETACH_WAIT_CN_DETACH_CNF))
    {
        /*��ӡ������Ϣ*/
        NAS_EMM_DETACH_LOG_WARN("NAS_EMM_MsDrgInitSsWtCnDetCnfMsgT3421Exp: STATE ERR!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*Detach attempt counter��1*/
    NAS_EMM_GLO_AD_GetDetAtmpCnt()++;

    /*�ж�Detach attempt counter����ΪС��5�ʹ��ڵ���5����*/
    if (NAS_EMM_GLO_AD_GetDetAtmpCnt() < 5)
    {
        /*����DETACH REQUEST��Ϣ*/
        NAS_EMM_SendDetachReqMo();
    }
    else
    {
        NAS_EMM_DETACH_LOG_INFO("NAS_EMM_MsDrgInitSsWtCnDetCnfMsgT3421Exp 5 times");

        /*��ESM����ID_EMM_ESM_STATUS_IND��Ϣ*/
        NAS_EMM_EsmSendStatResult(          EMM_ESM_ATTACH_STATUS_DETACHED);
        /* lihong00150010 emergency tau&service begin */
        if (VOS_TRUE == NAS_EMM_GLO_AD_GetUsimPullOutFlag())
        {
            /*״̬ת����֪ͨMMC����Ч*/
            NAS_EMM_ProcLocalNoUsim();
        }/* lihong00150010 emergency tau&service end */
        else
        {
            NAS_EMM_AdStateConvert( EMM_MS_DEREG,
                                    EMM_SS_DEREG_NORMAL_SERVICE,
                                    TI_NAS_EMM_STATE_NO_TIMER);

        }
        NAS_EMM_SendDetRslt(MMC_LMM_DETACH_RSLT_SUCCESS);

        /*��MRRC����NAS_EMM_MRRC_REL_REQ��Ϣ*/
        NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);

    }
    return  NAS_LMM_MSG_HANDLED;
}
VOS_VOID  NAS_EMM_DetachToTAU()
{
    /*ֹͣ��ʱ��T3421*/
    NAS_LMM_StopStateTimer(          TI_NAS_EMM_T3421);

    /*�޸�״̬��������״̬REG��״̬REG_NORMAL_SERVICE*/
    NAS_EMM_AdStateConvert(             EMM_MS_REG,
                                        EMM_SS_REG_NORMAL_SERVICE,
                                        TI_NAS_EMM_STATE_NO_TIMER);

   return;
}



VOS_UINT32  NAS_EMM_MsRegSsRegNmlSrvMsgIntraDetReq(VOS_UINT32  ulMsgId,
                                                   VOS_VOID   *pMsgStru)
{
    MMC_LMM_TAU_RSLT_ENUM_UINT32        ulTauRslt = MMC_LMM_TAU_RSLT_BUTT;
    NAS_LMM_INTRA_DETACH_REQ_STRU              *pRcvEmmMsg;
    pRcvEmmMsg                          = (NAS_LMM_INTRA_DETACH_REQ_STRU *) pMsgStru;

    /*��ӡ����ú���*/
    NAS_EMM_DETACH_LOG1_INFO("NAS_EMM_MsRegSsRegNmlSrvMsgTauIntraMsgDetReq is entered", ulMsgId);


     /*���״̬�Ƿ�ƥ�䣬����ƥ�䣬�˳�*/
    if (NAS_EMM_AD_CHK_STAT_INVALID(EMM_MS_REG,EMM_SS_REG_NORMAL_SERVICE))
    {
        /*��ӡ������Ϣ*/
        NAS_EMM_DETACH_LOG_WARN("NAS_EMM_MsRegSsRegNmlSrvMsgTauIntraMsgDetReq: STATE ERR!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*��Ϣ���ݼ��,���д���ӡ���˳�*/
    if (NAS_EMM_PARA_INVALID == NAS_EMM_IntraDetReqChk(pRcvEmmMsg))
    {
        NAS_EMM_DETACH_LOG_ERR("NAS_EMM_MsRegSsRegNmlSrvMsgTauIntraMsgDetReq: MMC_MM_SYS_INFO_IND_STRU para err!");
        return  NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* �����IMSI DETACH���ͣ�����IMSI DETACH���� */
    if (MMC_LMM_MO_DET_CS_ONLY == NAS_EMM_GLO_AD_GetDetTypeMo())
    {
        NAS_EMM_DETACH_LOG_INFO("NAS_EMM_MsRegSsRegNmlSrvMsgTauIntraMsgDetReq:Proceed Imsi Detach");

        /* IMSI detach��TAU��ϣ����TAU ACCEPT��Ta Updated Only��ԭ��ֵΪ#2����#18
           Ҳ�����NORMAL_SERVICE�������ڲ�DETACH������ʱ��Ӧ���ٷ���IMSI DETACH����*/
        if (NAS_LMM_REG_DOMAIN_PS == NAS_LMM_GetEmmInfoRegDomain())
        {
            /*��APP����DETACH�ɹ�*/
            NAS_EMM_AppSendDetCnf(MMC_LMM_DETACH_RSLT_SUCCESS);

            /* IMSI DETACH���ͷ���Դ */
            NAS_LMM_ImsiDetachReleaseResource();

            return  NAS_LMM_MSG_HANDLED;
        }

        /* ����IMSI DETACH���� */
        NAS_EMM_SendImsiDetachReqMo();
        return  NAS_LMM_MSG_HANDLED;
    }

    NAS_EMM_DETACH_LOG_INFO("NAS_EMM_MsRegSsRegNmlSrvMsgTauIntraMsgDetReq:Proceed EPS Detach or EPS/IMSI Detach");

    /* ���3411�����У���Ҫ��MMC��TAU���������MMC�˳�����״̬����3402��ʱ����
       ��������֪ͨ��MMC�յ�TAU������Դ���Ϊ5ʱ�����˳�����״̬�� */
    if (NAS_LMM_TIMER_RUNNING == NAS_LMM_IsPtlTimerRunning(TI_NAS_EMM_PTL_T3411))
    {
        NAS_EMM_SetTauTypeNoProcedure();

        ulTauRslt = MMC_LMM_TAU_RSLT_MO_DETACH_FAILURE;
        NAS_EMM_MmcSendTauActionResultIndOthertype((VOS_VOID*)&ulTauRslt);

    }

    /*ֹͣ��ʱ��*/
    NAS_LMM_StopPtlTimer(                TI_NAS_EMM_PTL_T3411);
    NAS_LMM_StopPtlTimer(                TI_NAS_EMM_PTL_T3402);

    /*����DETACH REQUEST��Ϣ*/
    NAS_EMM_SendDetachReqMo();

    return   NAS_LMM_MSG_HANDLED;
}

/*****************************************************************************
 Function Name   : NAS_EMM_DetGuAttachRst
 Description     : ����״̬�£�����GU������DETACH���
 Input           :VOS_VOID
 Output          : None
 Return          : VOS_VOID

 History         :
    1.Hanlufeng 41410      2011-5-12  Draft Enact

*****************************************************************************/
VOS_VOID NAS_EMM_DetGuAttachRst( VOS_VOID * pMsg )
{
    MMC_LMM_ACTION_RESULT_REQ_STRU     *pMmcActResult;


    pMmcActResult = (MMC_LMM_ACTION_RESULT_REQ_STRU *)pMsg;


    NAS_EMM_ATTACH_LOG1_NORM("NAS_EMM_AttGuAttachRst: ulActRst =.",
                            pMmcActResult->ulActRst);

    /* ����PS��صģ���ʱ�����������ϲ���ʱ�ٿ��� */
    if(MMC_LMM_IMSI_ONLY == pMmcActResult->ulRstDomain)
    {
        NAS_EMM_ATTACH_LOG1_NORM("NAS_EMM_AttGuAttachRst: ulRstDomain =.",
                            pMmcActResult->ulRstDomain);
        return;
    }

    /* DETACH ���۳ɹ�ʧ�ܣ�����Ϊȥע����� */
    NAS_EMM_AdStateConvert(         EMM_MS_DEREG,
                                    EMM_SS_DEREG_NO_CELL_AVAILABLE,
                                    TI_NAS_EMM_STATE_NO_TIMER);


    /* ֪ͨESM״̬�ı� */
    NAS_EMM_PUB_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_DETACHED);

    return;
}
VOS_UINT32 NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgEsmDataReq
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    EMM_ESM_DATA_REQ_STRU              *pstEsmDataReq = (EMM_ESM_DATA_REQ_STRU*)pMsgStru;

    NAS_EMM_DETACH_LOG2_INFO("NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgEsmDataReq",
                                                            ulMsgId,
                                                            pMsgStru);

    /*���״̬�Ƿ�ƥ�䣬����ƥ�䣬�˳�*/
    if (NAS_EMM_AD_CHK_STAT_INVALID(EMM_MS_REG,EMM_SS_REG_IMSI_DETACH_WATI_CN_DETACH_CNF))
    {
        /*��ӡ������Ϣ*/
        NAS_EMM_DETACH_LOG_WARN("NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgEsmDataReq: STATE ERR!");
        return NAS_LMM_MSG_DISCARD;
    }

    if (VOS_TRUE == pstEsmDataReq->ulIsEmcType)
    {
        NAS_LMM_SetEmmInfoIsEmerPndEsting(VOS_TRUE);
    }

    NAS_EMM_SER_SendMrrcDataReq_ESMdata(&pstEsmDataReq->stEsmMsg);

    return NAS_LMM_MSG_HANDLED;
}



VOS_UINT32 NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgTcDataReq
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    NAS_EMM_DETACH_LOG2_INFO("NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgTcDataReq",
                                                            ulMsgId,
                                                            pMsgStru);

    /*���״̬�Ƿ�ƥ�䣬����ƥ�䣬�˳�*/
    if (NAS_EMM_AD_CHK_STAT_INVALID(EMM_MS_REG,EMM_SS_REG_IMSI_DETACH_WATI_CN_DETACH_CNF))
    {
        /*��ӡ������Ϣ*/
        NAS_EMM_DETACH_LOG_WARN("NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgTcDataReq: STATE ERR!");
        return NAS_LMM_MSG_DISCARD;
    }

    NAS_EMM_SER_SendMrrcDataReq_Tcdata((EMM_ETC_DATA_REQ_STRU *)pMsgStru);

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32 NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgRabmRelReq
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsgStru
)
{
    NAS_EMM_DETACH_LOG2_INFO("NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgRabmRelReq",
                                                            ulMsgId,
                                                            pMsgStru);

    /*���״̬�Ƿ�ƥ�䣬����ƥ�䣬�˳�*/
    if (NAS_EMM_AD_CHK_STAT_INVALID(EMM_MS_REG,EMM_SS_REG_IMSI_DETACH_WATI_CN_DETACH_CNF))
    {
        /*��ӡ������Ϣ*/
        NAS_EMM_DETACH_LOG_WARN("NAS_EMM_MsRegImsiDetachWtCnDetCnfMsgRabmRelReq: STATE ERR!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*ֹͣ��ʱ��T3421*/
    NAS_LMM_StopStateTimer(              TI_NAS_EMM_T3421);

    /* ����ע����ΪPS */
    NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_PS);

    NAS_EMM_AdStateConvert(         EMM_MS_REG,
                                    EMM_SS_REG_NORMAL_SERVICE,
                                    TI_NAS_EMM_STATE_NO_TIMER);
    /*��MMC����LMM_MMC_DETACH_CNF��Ϣ*/
    NAS_EMM_SendDetRslt(MMC_LMM_DETACH_RSLT_SUCCESS);

    /* IMSI DETACH���ͷ���Դ */
    NAS_LMM_ImsiDetachReleaseResource();

    /*��MRRC����NAS_EMM_MRRC_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(NAS_LMM_NOT_BARRED);

    return  NAS_LMM_MSG_HANDLED;
}
VOS_UINT32 NAS_EMM_SndDetachReqFailProc(VOS_VOID* pMsg,VOS_UINT32 *pulIsDelBuff)
{
    LRRC_LMM_DATA_CNF_STRU              *pstRrcMmDataCnf = VOS_NULL_PTR;

    *pulIsDelBuff = VOS_TRUE;

    pstRrcMmDataCnf = (LRRC_LMM_DATA_CNF_STRU*) pMsg;

    if ((EMM_MS_DEREG_INIT != NAS_LMM_GetEmmCurFsmMS())
        && ((EMM_MS_REG != NAS_LMM_GetEmmCurFsmMS())
            && (EMM_SS_REG_IMSI_DETACH_WATI_CN_DETACH_CNF != NAS_LMM_GetEmmCurFsmSS())))
    {
        /*��ӡ������Ϣ*/
        NAS_EMM_SER_LOG_INFO("NAS_EMM_SndExtendedServiceReqFailProc: STATE ERR!");
        return  NAS_EMM_SUCC;
    }
    switch (pstRrcMmDataCnf->enSendRslt)
    {
        case LRRC_LMM_SEND_RSLT_FAILURE_HO:
        case LRRC_LMM_SEND_RSLT_FAILURE_TXN:
        case LRRC_LMM_SEND_RSLT_FAILURE_RLF:
            if (EMM_MS_DEREG_INIT == NAS_LMM_GetEmmCurFsmMS())
            {
                NAS_LMM_StopStateTimer(         TI_NAS_EMM_T3421);

                /*����DETACH REQUEST��Ϣ*/
                NAS_EMM_SendDetachReqMo();
            }
            else
            {
                NAS_LMM_StopStateTimer(         TI_NAS_EMM_T3421);

                /* ����IMSI DETACH���� */
                NAS_EMM_SendImsiDetachReqMo();
            }
            break;
        default:
            break;
        }

    return NAS_EMM_SUCC;
}
VOS_UINT32 NAS_EMM_SndMtDetachAccFailProc(VOS_VOID* pMsg,VOS_UINT32 *pulIsDelBuff)
{
    LRRC_LMM_DATA_CNF_STRU              *pstRrcMmDataCnf = VOS_NULL_PTR;
    NAS_EMM_MRRC_DATA_REQ_STRU          *pMrrcDataMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulRrcMmDataReqMsgLen;
    NAS_EMM_MRRC_MGMT_DATA_STRU         *pEmmMrrcMgmtData = NAS_EMM_NULL_PTR;

    *pulIsDelBuff = VOS_TRUE;

    pstRrcMmDataCnf = (LRRC_LMM_DATA_CNF_STRU*) pMsg;

    pEmmMrrcMgmtData = NAS_EMM_FindMsgInDataReqBuffer(pstRrcMmDataCnf->ulOpId);

    if (NAS_EMM_NULL_PTR == pEmmMrrcMgmtData)
    {
        return NAS_EMM_FAIL;
    }

    ulRrcMmDataReqMsgLen = sizeof(NAS_EMM_MRRC_DATA_REQ_STRU)+
                           pEmmMrrcMgmtData->ulNasMsgLength -
                           NAS_EMM_4BYTES_LEN;
    /* ������Ϣ�ڴ�*/
    pMrrcDataMsg = (NAS_EMM_MRRC_DATA_REQ_STRU *)((VOS_VOID*)NAS_LMM_MEM_ALLOC(ulRrcMmDataReqMsgLen));

    if(VOS_NULL_PTR == pMrrcDataMsg)
    {
        NAS_EMM_PUBU_LOG_ERR("NAS_EMM_SndMtDetachAccFailProc: Mem Alloc Fail");
        return NAS_EMM_FAIL;
    }

    switch (pstRrcMmDataCnf->enSendRslt)
    {
        case LRRC_LMM_SEND_RSLT_FAILURE_HO:
        case LRRC_LMM_SEND_RSLT_FAILURE_TXN:
        case LRRC_LMM_SEND_RSLT_FAILURE_RLF:
            if (NAS_EMM_MAX_MT_DETACH_ACC_TX_NUM > NAS_EMM_GLO_AD_GetMtDetachAccTxCnt())
            {
                NAS_EMM_GLO_AD_GetMtDetachAccTxCnt() ++;

                pMrrcDataMsg->enDataCnf = LRRC_LMM_DATA_CNF_NEED;

                pMrrcDataMsg->enEmmMsgType = NAS_EMM_MSG_DETACH_ACP;
                pMrrcDataMsg->ulMsgId = ID_NAS_LMM_INTRA_MRRC_DATA_REQ;
                pMrrcDataMsg->ulEsmMmOpId = pEmmMrrcMgmtData->ulEsmMmOpId;

                pMrrcDataMsg->stNasMsg.ulNasMsgSize = pEmmMrrcMgmtData->ulNasMsgLength;

                NAS_LMM_MEM_CPY(pMrrcDataMsg->stNasMsg.aucNasMsg,
                                 pEmmMrrcMgmtData->aucNasMsgContent,
                                 pEmmMrrcMgmtData->ulNasMsgLength);

                NAS_EMM_SndUplinkNasMsg(pMrrcDataMsg);
            }
            else
            {
                NAS_EMM_GLO_AD_GetMtDetachAccTxCnt() = 0;
            }

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
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

