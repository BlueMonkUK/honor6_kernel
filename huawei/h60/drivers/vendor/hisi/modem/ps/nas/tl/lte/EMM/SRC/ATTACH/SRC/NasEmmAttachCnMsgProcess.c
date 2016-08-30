



/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/

#include    "NasEmmAttDetInclude.h"
#include    "NasLmmPubMPrint.h"
#include    "LNasStkInterface.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASEMMATTACHCNMSGPROC_C
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


/*****************************************************************************
  3 Function
*****************************************************************************/
/*lint -e960*/

VOS_UINT32  NAS_EMM_CnAttachAcpMsgChk(const NAS_EMM_CN_ATTACH_ACP_STRU  *pMsgMsg)
{

    NAS_LMM_UEID_STRU                   *pstEmmInfoUeIdAddr = VOS_NULL_PTR;

    /* EPS attach result�Ϸ��Լ��*/
    if ((NAS_EMM_CN_MSG_ATTACH_TYPE_PS != pMsgMsg->ucAttachRst)
       &&(NAS_EMM_CN_MSG_ATTACH_TYPE_PS_CS != pMsgMsg->ucAttachRst))
    {
        return  NAS_EMM_PARA_INVALID;
    }

    /* UEû��GUTI������Ҳû�з���GUTI*/
    pstEmmInfoUeIdAddr =                NAS_LMM_GetEmmInfoUeidAddr();
    if((NAS_EMM_BIT_NO_SLCT == pMsgMsg->ucBitOpGuti)
      &&(NAS_EMM_BIT_NO_SLCT == pstEmmInfoUeIdAddr->bitOpGuti))
    {
        /*����EMM STATUS, ���յ�conditional IE�������Ϣ */
        NAS_EMM_SendMsgEmmStatus( NAS_LMM_CAUSE_CONDITIONAL_IE_ERROR );
        return  NAS_EMM_PARA_INVALID;
    }

    return NAS_EMM_PARA_VALID;

}
VOS_UINT32  NAS_EMM_CnAttachRejMsgChk(const NAS_EMM_CN_ATTACH_REJ_STRU *pMsgMsg)
{

    /*��Ϣ���ݼ�� */

    (VOS_VOID)pMsgMsg;
    return  NAS_EMM_PARA_VALID;


}
/*lint -e960*/
VOS_VOID    NAS_EMM_EsmSendEstCnf (EMM_ESM_ATTACH_STATUS_ENUM_UINT32 ucEstRst)
{
    EMM_ESM_EST_CNF_STRU                 *pEmmEsmEstCnfMsg;


    /*������Ϣ�ڴ�*/
    pEmmEsmEstCnfMsg   = (VOS_VOID *)NAS_LMM_ALLOC_MSG(sizeof(EMM_ESM_EST_CNF_STRU));

    /*�ж�����������ʧ�ܴ�ӡ�����˳�*/
    if (NAS_EMM_NULL_PTR == pEmmEsmEstCnfMsg)
    {
        /*��ӡ����*/
        NAS_EMM_ATTACH_LOG_ERR("NAS_EMM_EsmSendEstCnf: MSG ALLOC ERR!");
        return;

    }

    /*����ID_EMM_ESM_EST_CNF��Ϣ*/
    /*�����Ϣͷ*/
    NAS_EMM_COMP_AD_ESM_MSG_HEADER(      pEmmEsmEstCnfMsg,
                                        (sizeof(EMM_ESM_EST_CNF_STRU)-
                                        NAS_EMM_LEN_VOS_MSG_HEADER));

    /*�����ϢID*/
    pEmmEsmEstCnfMsg->ulMsgId            = ID_EMM_ESM_EST_CNF;

    /*�����Ϣ����*/
    pEmmEsmEstCnfMsg->ulEstRst           = ucEstRst;

    /*��ESM����ID_EMM_ESM_EST_CNF��Ϣ*/
    NAS_LMM_SEND_MSG(                   pEmmEsmEstCnfMsg);

    return;

}
VOS_VOID    NAS_EMM_EsmSendDataInd (const NAS_EMM_CN_MSGCON_STRU *pstEsmBearerReq)
{
    EMM_ESM_DATA_IND_STRU                *pEmmEsmBearerReqMsg;
    VOS_UINT32                          ulEsmMsgLen;
    VOS_UINT32                          ulDataIndLenNoHeader;


    ulEsmMsgLen                          = pstEsmBearerReq->ulMsgLen;
    ulDataIndLenNoHeader                = NAS_EMM_CountEsmDataIndLen(ulEsmMsgLen);


    /*������Ϣ�ڴ�*/
    if (ulEsmMsgLen < 4)
    {
        pEmmEsmBearerReqMsg = (VOS_VOID *)NAS_LMM_ALLOC_MSG(sizeof(EMM_ESM_DATA_IND_STRU));

    }
    else
    {
        pEmmEsmBearerReqMsg = (VOS_VOID *)NAS_LMM_ALLOC_MSG(
                                        ulDataIndLenNoHeader + NAS_EMM_LEN_VOS_MSG_HEADER);
    }

    /*�ж�����������ʧ�ܴ�ӡ�����˳�*/
    if (NAS_EMM_NULL_PTR == pEmmEsmBearerReqMsg)
    {
        /*��ӡ����*/
        NAS_EMM_ATTACH_LOG_ERR("NAS_EMM_EsmSendDataInd: MSG ALLOC ERR!");
        return;

    }

    /*����ID_EMM_ESM_DATA_IND��Ϣ*/
    /*�����Ϣͷ*/
    NAS_EMM_COMP_AD_ESM_MSG_HEADER(pEmmEsmBearerReqMsg, ulDataIndLenNoHeader);

    /*�����ϢID*/
    pEmmEsmBearerReqMsg->ulMsgId         = ID_EMM_ESM_DATA_IND;

    /*�����Ϣ����*/
    pEmmEsmBearerReqMsg->stEsmMsg.ulEsmMsgSize = pstEsmBearerReq->ulMsgLen;

    NAS_LMM_MEM_CPY(                 pEmmEsmBearerReqMsg->stEsmMsg.aucEsmMsg,
                                        pstEsmBearerReq->aucMsg,
                                        pstEsmBearerReq->ulMsgLen);

    /*��ESM����ID_EMM_ESM_DATA_IND��Ϣ*/
    NAS_LMM_SEND_MSG(                   pEmmEsmBearerReqMsg);

    return;

}
/*****************************************************************************
 Function Name   : NAS_EMM_TcSendDataInd
 Description     : ��TC����ID_EMM_ETC_DATA_IND��Ϣ
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2009-10-16  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_EMM_TcSendDataInd
(
    const NAS_EMM_CN_MSGCON_STRU       *pstTcMsg
)
{
    EMM_ETC_DATA_IND_STRU               *pEmmTcDataIndMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulTcMsgLen;
    VOS_UINT32                          ulDataIndLenNoHeader;


    ulTcMsgLen                          = pstTcMsg->ulMsgLen;
    ulDataIndLenNoHeader                = NAS_EMM_CountTcDataIndLen(ulTcMsgLen);


    /*������Ϣ�ڴ�*/
    if (ulTcMsgLen < 4)
    {
        pEmmTcDataIndMsg = (VOS_VOID *)NAS_LMM_ALLOC_MSG(sizeof(EMM_ETC_DATA_IND_STRU));

    }
    else
    {
        pEmmTcDataIndMsg = (VOS_VOID *)NAS_LMM_ALLOC_MSG(
                                        ulDataIndLenNoHeader + NAS_EMM_LEN_VOS_MSG_HEADER);
    }

    /*�ж�����������ʧ�ܴ�ӡ�����˳�*/
    if (NAS_EMM_NULL_PTR == pEmmTcDataIndMsg)
    {
        /*��ӡ����*/
        NAS_EMM_ATTACH_LOG_ERR("NAS_EMM_TcSendDataInd: MSG ALLOC ERR!");
        return;

    }

    /*����ID_EMM_ETC_DATA_IND��Ϣ*/
    /*�����Ϣͷ*/
    NAS_EMM_COMP_AD_TC_MSG_HEADER(pEmmTcDataIndMsg, ulDataIndLenNoHeader);

    /*�����ϢID*/
    pEmmTcDataIndMsg->ulMsgId         = ID_EMM_ETC_DATA_IND;

    /*�����Ϣ����*/
    pEmmTcDataIndMsg->stTcMsg.ulTcMsgSize = pstTcMsg->ulMsgLen;

    NAS_LMM_MEM_CPY(                 pEmmTcDataIndMsg->stTcMsg.aucTcMsg,
                                        pstTcMsg->aucMsg,
                                        pstTcMsg->ulMsgLen);

    /*��TC����ID_EMM_ETC_DATA_IND��Ϣ*/
    NAS_LMM_SEND_MSG(                   pEmmTcDataIndMsg);

    /* ��ӡ������Ϣ */
    NAS_EMM_ATTACH_LOG_NORM("NAS_EMM_TcSendDataInd:NORM:Send ID_EMM_ETC_DATA_IND!");
}
VOS_UINT32  NAS_EMM_CountEsmDataIndLen(VOS_UINT32 ulNasEsmMsgLen)
{
    VOS_UINT32                          ulMsgLen;

    ulMsgLen                          = NAS_EMM_AD_LEN_MSG_ID                      +
                                        NAS_EMM_AD_LEN_IE_MSGSIZE_OF_NAS_MSG_STRU  +
                                        (ulNasEsmMsgLen);

    return ulMsgLen;

}
/*****************************************************************************
 Function Name   : NAS_EMM_CountTcDataIndLen
 Description     : ����ID_EMM_ETC_DATA_IND��Ϣ����

 Input           : ulNasTcMsgLen------TC��Ϣ����
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010         2009-10-16  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_CountTcDataIndLen(VOS_UINT32 ulNasTcMsgLen)
{
    VOS_UINT32                          ulMsgLen;

    ulMsgLen                          = NAS_EMM_AD_LEN_MSG_ID                      +
                                        NAS_EMM_AD_LEN_IE_MSGSIZE_OF_NAS_MSG_STRU  +
                                        (ulNasTcMsgLen);

    return ulMsgLen;

}


VOS_VOID    NAS_EMM_AttDataUpdate(VOS_UINT32 ulDeleteRplmn)
{

    /*���� EPS update statusΪEU3*/
    NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_ROAMING_NOT_ALLOWED_EU3;

    /*ɾ��GUTI*/
    NAS_EMM_ClearGuti();

    /*ɾ��LVR TAI*/
    if (NAS_EMM_DELETE_RPLMN == ulDeleteRplmn)
    {
        NAS_EMM_ClearLVRTai();
    }
    else
    {
        NAS_EMM_SetLVRTacInvalid();
    }

    /*ɾ��KSIasme*/
    /*NAS_EMM_ClearCurSecuCntxt();*/
    NAS_EMM_ClearAllSecuCntxt();

    if((NAS_LMM_CAUSE_PLMN_NOT_ALLOW == NAS_EMM_GLO_AD_GetAttRejCau()) ||
       (NAS_LMM_CAUSE_TA_NOT_ALLOW == NAS_EMM_GLO_AD_GetAttRejCau()) ||
       (NAS_LMM_CAUSE_ROAM_NOT_ALLOW == NAS_EMM_GLO_AD_GetAttRejCau()) ||
       (NAS_LMM_CAUSE_EPS_SERV_NOT_ALLOW_IN_PLMN == NAS_EMM_GLO_AD_GetAttRejCau()) ||
       (NAS_LMM_CAUSE_NO_SUITABL_CELL == NAS_EMM_GLO_AD_GetAttRejCau()))
    {

        /*���ATTACH ATTEMPTING COUNTER������*/
        NAS_EMM_GLO_AD_GetAttAtmpCnt()      = 0;
    }

    /* ����Ч�İ�ȫ������ֱ���ں����ڲ�д�� */
    NAS_LMM_WriteEpsSecuContext(NAS_NV_ITEM_UPDATE);
    /*����PS LOC��Ϣ*/
    NAS_LMM_WritePsLoc(NAS_NV_ITEM_UPDATE);

    return;

}


VOS_VOID  NAS_EMM_ProcRejCauseVal2(VOS_VOID)
{

    /*���ATTACH ATTEMPTING COUNTER��TRACKING AREA UPDATING ATTEMPT������*/
    NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);

    /*����EPS UPDATE STAUSΪEU1*/
    NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_UPDATED_EU1;


    /*������ʱ��TI_NAS_EMM_WAIT_ESM_BEARER_CNF*/
    NAS_LMM_StartStateTimer(            TI_NAS_EMM_WAIT_ESM_BEARER_CNF);

    /*�޸�״̬��������״̬REG_INIT��״̬ATTACH_WAIT_ESM_BEARER_CNF*/
    NAS_EMM_AdStateConvert(             EMM_MS_REG_INIT,
                                        EMM_SS_ATTACH_WAIT_ESM_BEARER_CNF,
                                        TI_NAS_EMM_WAIT_ESM_BEARER_CNF);
    return;
}


VOS_VOID    NAS_EMM_ProcRejCauseVal5()
{
    /*��ESM����ATTACH���*/
    NAS_EMM_AttSendEsmResult(EMM_ESM_ATTACH_STATUS_DETACHED);

    /*�޸�״̬��������״̬DEREG��״̬DEREG_LIMITED_SERVICE*/
    NAS_EMM_AdStateConvert(     EMM_MS_DEREG,
                                EMM_SS_DEREG_NO_IMSI,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /* ��MMC���ͽ�� */
    NAS_EMM_AppSendAttRej();

    /*��MRRC����NAS_EMM_MRRC_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);

    /* ֪ͨIMSA���ٳ���CS�� */

    return;
}
VOS_VOID    NAS_EMM_ProcRejCauseVal3678()
{

    NAS_EMM_AttDataUpdate(NAS_EMM_DELETE_RPLMN);

    NAS_LMM_SetPsSimValidity(NAS_LMM_SIM_INVALID);

    if (EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
    {
        /* ֪ͨESM������EMM��Ҫ������ע�ᣬESM����Ҫ�����̬���еĽ���PDN������¼ */
        NAS_EMM_AttSendEsmResult(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        /*��ESM����ATTACH���*/
        NAS_EMM_AttSendEsmResult(EMM_ESM_ATTACH_STATUS_DETACHED);
    }

    /*�޸�״̬��������״̬DEREG��״̬DEREG_LIMITED_SERVICE*/
    NAS_EMM_AdStateConvert(     EMM_MS_DEREG,
                                EMM_SS_DEREG_NO_IMSI,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /* ��MMC���ͽ�� */
    NAS_EMM_AppSendAttRej();

    /*��LRRC����LRRC_LMM_NAS_INFO_CHANGE_REQЯ��USIM��״̬*/
    NAS_EMM_SendUsimStatusToRrc(LRRC_LNAS_USIM_PRESENT_INVALID);

    /*��MRRC����NAS_EMM_MRRC_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);

    if (EMM_ATTACH_CAUSE_ESM_EMC_ATTACH == NAS_EMM_GLO_AD_GetAttCau())
    {
        /* ֪ͨIMSA�ٳ���CS�� */
    }

    /* ���֮ǰ��¼��attachԭ��ֵ */
    NAS_LMM_ClearEmmInfoMmcAttachReason();

    return;
}



VOS_VOID    NAS_EMM_ProcRejCauseVal11()
{

    NAS_EMM_AttDataUpdate(NAS_EMM_NOT_DELETE_RPLMN);

    if (EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
    {
        /* ֪ͨESM������EMM��Ҫ������ע�ᣬESM����Ҫ�����̬���еĽ���PDN������¼ */
        NAS_EMM_AttSendEsmResult(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        /*��ESM����ATTACH���*/
        NAS_EMM_AttSendEsmResult(EMM_ESM_ATTACH_STATUS_DETACHED);
    }

    /*�޸�״̬��������״̬DEREG��״̬DEREG_LIMITED_SERVICE*/
    NAS_EMM_AdStateConvert(     EMM_MS_DEREG,
                                EMM_SS_DEREG_LIMITED_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /* ��MMC���ͽ�� */
    NAS_EMM_AppSendAttRej();

    NAS_EMM_WaitNetworkRelInd();

    if (EMM_ATTACH_CAUSE_ESM_EMC_ATTACH == NAS_EMM_GLO_AD_GetAttCau())
    {
        /* ֪ͨIMSA�ٳ���CS�� */
    }

    return;
}
VOS_VOID    NAS_EMM_ProcRejCauseVal12()
{
    NAS_MM_TA_STRU                      stTa;

    /* ��ȡ��ǰTA */
    NAS_EMM_GetCurrentTa(&stTa);

    NAS_EMM_AttDataUpdate(NAS_EMM_NOT_DELETE_RPLMN);

    NAS_EMM_AddForbTa(&stTa,NAS_LMM_GetEmmInfoNetInfoForbTaForRposAddr());

    /* ����REJ cause 12���,������MMC�ϱ����������Ƶ����Ʒ���״̬ 2011-07-27*/
    NAS_LMM_SetNasRejCause12Flag(NAS_LMM_REJ_CAUSE_IS_12);

    if (EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
    {
        /* ֪ͨESM������EMM��Ҫ������ע�ᣬESM����Ҫ�����̬���еĽ���PDN������¼ */
        NAS_EMM_AttSendEsmResult(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        /*��ESM����ATTACH���*/
        NAS_EMM_AttSendEsmResult(EMM_ESM_ATTACH_STATUS_DETACHED);
    }

    /*�޸�״̬��������״̬DEREG��״̬DEREG_LIMITED_SERVICE*/
    NAS_EMM_AdStateConvert(     EMM_MS_DEREG,
                                EMM_SS_DEREG_LIMITED_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /* ��MMC���ͽ�� */
    NAS_EMM_AppSendAttRej();

    NAS_EMM_WaitNetworkRelInd();

    if (EMM_ATTACH_CAUSE_ESM_EMC_ATTACH == NAS_EMM_GLO_AD_GetAttCau())
    {
        /* ֪ͨIMSA�ٳ���CS�� */
    }

    return;

}
VOS_VOID    NAS_EMM_ProcRejCauseVal13()
{
    NAS_MM_TA_STRU                      stTa;

    /* ��ȡ��ǰTA */
    NAS_EMM_GetCurrentTa(&stTa);

    NAS_EMM_AttDataUpdate(NAS_EMM_NOT_DELETE_RPLMN);

    NAS_EMM_AddForbTa(&stTa,NAS_LMM_GetEmmInfoNetInfoForbTaForRoamAddr());
    NAS_EMMC_SendRrcCellSelectionReq(LRRC_LNAS_FORBTA_CHANGE);

    if (EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
    {
        /* ֪ͨESM������EMM��Ҫ������ע�ᣬESM����Ҫ�����̬���еĽ���PDN������¼ */
        NAS_EMM_AttSendEsmResult(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        /*��ESM����ATTACH���*/
        NAS_EMM_AttSendEsmResult(EMM_ESM_ATTACH_STATUS_DETACHED);
    }

    /*�޸�״̬��������״̬DEREG��״̬DEREG_LIMITED_SERVICE*/
    NAS_EMM_AdStateConvert(     EMM_MS_DEREG,
                                EMM_SS_DEREG_LIMITED_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /* ��MMC���ͽ�� */
    NAS_EMM_AppSendAttRej();

    NAS_EMM_WaitNetworkRelInd();

    if (EMM_ATTACH_CAUSE_ESM_EMC_ATTACH == NAS_EMM_GLO_AD_GetAttCau())
    {
        /* ֪ͨIMSA�ٳ���CS�� */
    }

    return;
}



VOS_VOID    NAS_EMM_ProcRejCauseVal14()
{

    NAS_EMM_AttDataUpdate(NAS_EMM_NOT_DELETE_RPLMN);

    if (EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
    {
        /* ֪ͨESM������EMM��Ҫ������ע�ᣬESM����Ҫ�����̬���еĽ���PDN������¼ */
        NAS_EMM_AttSendEsmResult(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        /*��ESM����ATTACH���*/
        NAS_EMM_AttSendEsmResult(EMM_ESM_ATTACH_STATUS_DETACHED);
    }

    /*�޸�״̬��������״̬DEREG��״̬DEREG_LIMITED_SERVICE*/
    NAS_EMM_AdStateConvert(     EMM_MS_DEREG,
                                EMM_SS_DEREG_LIMITED_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /* ��MMC���ͽ�� */
    NAS_EMM_AppSendAttRej();

    NAS_EMM_WaitNetworkRelInd();

    if (EMM_ATTACH_CAUSE_ESM_EMC_ATTACH == NAS_EMM_GLO_AD_GetAttCau())
    {
        /* ֪ͨIMSA�ٳ���CS�� */
    }

    return;
}
VOS_VOID    NAS_EMM_ProcRejCauseVal15()
{
    NAS_MM_TA_STRU                      stTa;

    /* ��ȡ��ǰTA */
    NAS_EMM_GetCurrentTa(&stTa);

    NAS_EMM_AttDataUpdate(NAS_EMM_NOT_DELETE_RPLMN);

    if (NAS_EMMC_USER_SPEC_PLMN_YES == NAS_EMMC_GetUserSpecPlmnFlag())
    {
        NAS_EMM_AddForbTa(&stTa,NAS_LMM_GetEmmInfoNetInfoForbTaForRej15Addr());
    }
    else
    {
        NAS_EMM_AddForbTa(&stTa,NAS_LMM_GetEmmInfoNetInfoForbTaForRoamAddr());
        NAS_EMMC_SendRrcCellSelectionReq(LRRC_LNAS_FORBTA_CHANGE);
    }

    if (EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
    {
        /* ֪ͨESM������EMM��Ҫ������ע�ᣬESM����Ҫ�����̬���еĽ���PDN������¼ */
        NAS_EMM_AttSendEsmResult(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        /*��ESM����ATTACH���*/
        NAS_EMM_AttSendEsmResult(EMM_ESM_ATTACH_STATUS_DETACHED);
    }

    /*�޸�״̬��������״̬DEREG��״̬DEREG_LIMITED_SERVICE*/
    NAS_EMM_AdStateConvert(     EMM_MS_DEREG,
                                EMM_SS_DEREG_LIMITED_SERVICE,
                                TI_NAS_EMM_STATE_NO_TIMER);

    /* ��MMC���ͽ�� */
    NAS_EMM_AppSendAttRej();

    NAS_EMM_WaitNetworkRelInd();

    if (EMM_ATTACH_CAUSE_ESM_EMC_ATTACH == NAS_EMM_GLO_AD_GetAttCau())
    {
        /* ֪ͨIMSA�ٳ���CS�� */
    }

    return;

}



VOS_VOID  NAS_EMM_ProcRejCauseVal161722(VOS_VOID )
{

    /*����GCFЭ��36523 9.2.1.2.3�½������������ATTACH ATTEMPTING COUNTER��
      TRACKING AREA UPDATING ATTEMPT���������ٶ�TRACKING AREA UPDATING ATTEMPT
      ��������1*/
    NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);

    /*tracking area updating attempt counter shall be incremented*/
    NAS_EMM_TAU_GetEmmTAUAttemptCnt() ++;

     /*����EPS UPDATE STAUSΪEU1*/
    NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_UPDATED_EU1;


    /*������ʱ��TI_NAS_EMM_WAIT_ESM_BEARER_CNF*/
    NAS_LMM_StartStateTimer(         TI_NAS_EMM_WAIT_ESM_BEARER_CNF);

    /*�޸�״̬��������״̬REG_INIT��״̬ATTACH_WAIT_ESM_BEARER_CNF*/
    NAS_EMM_AdStateConvert(             EMM_MS_REG_INIT,
                                        EMM_SS_ATTACH_WAIT_ESM_BEARER_CNF,
                                        TI_NAS_EMM_WAIT_ESM_BEARER_CNF);
    return;
}



VOS_VOID  NAS_EMM_ProcRejCauseVal18(VOS_VOID)
{

    /*���ATTACH ATTEMPTING COUNTER��TRACKING AREA UPDATING ATTEMPT������*/
    NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);

    /*����EPS UPDATE STAUSΪEU1*/
    NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_UPDATED_EU1;


    /*������ʱ��TI_NAS_EMM_WAIT_ESM_BEARER_CNF*/
    NAS_LMM_StartStateTimer(         TI_NAS_EMM_WAIT_ESM_BEARER_CNF);

    /*�޸�״̬��������״̬REG_INIT��״̬ATTACH_WAIT_ESM_BEARER_CNF*/
    NAS_EMM_AdStateConvert(             EMM_MS_REG_INIT,
                                        EMM_SS_ATTACH_WAIT_ESM_BEARER_CNF,
                                        TI_NAS_EMM_WAIT_ESM_BEARER_CNF);

    /*���������Ӹ���UE����ģʽ���в�ͬ�Ĵ���*/

    return;
}
VOS_VOID    NAS_EMM_ProcRejCauseValOther(NAS_EMM_CN_CAUSE_ENUM_UINT8 ucRejCauseVal)
{
    if((NAS_LMM_CAUSE_SEMANTICALLY_INCORRECT_MSG == ucRejCauseVal) ||
       (NAS_LMM_CAUSE_INVALID_MANDATORY_INF == ucRejCauseVal)       ||
       (NAS_LMM_CAUSE_MSG_NONEXIST_NOTIMPLEMENTE == ucRejCauseVal)  ||
       (NAS_LMM_CAUSE_IE_NONEXIST_NOTIMPLEMENTED == ucRejCauseVal)  ||
       (NAS_LMM_CAUSE_PROTOCOL_ERROR == ucRejCauseVal)||
       (VOS_TRUE == NAS_MML_IsRoamingRejectNoRetryFlgActived(ucRejCauseVal)))
    {
        NAS_EMM_GLO_AD_GetAttAtmpCnt()  = 5;
    }
    else if(NAS_LMM_CAUSE_ESM_FAILURE == ucRejCauseVal)
    {
        /*CL multimode ��EMMԭ��ֵΪ#19��ESMԭ��ֵΪ#54ʱattach���Դ�����1 2014-02-14*/
        if(NAS_LMM_ESM_CAUSE_PDN_CONNECTION_DOES_NOT_EXIST == NAS_EMM_GLO_AD_GetAttRejEsmCau())
        {
            /* ���֮ǰ�����attachԭ��ֵ */
            NAS_LMM_ClearEmmInfoMmcAttachReason();
            NAS_EMM_GLO_AD_GetAttAtmpCnt() ++;
        }
        else
        {
            NAS_EMM_GLO_AD_GetAttAtmpCnt() = 5;
        }
    }
    else
    {
        NAS_EMM_GLO_AD_GetAttAtmpCnt() ++;
    }

    if (EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
    {
        /* ֪ͨESM������EMM��Ҫ������ע�ᣬESM����Ҫ�����̬���еĽ���PDN������¼ */
        NAS_EMM_AttSendEsmResult(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
    }
    else
    {
        /*��ESM����ATTACH���*/
        NAS_EMM_AttSendEsmResult(EMM_ESM_ATTACH_STATUS_DETACHED);
    }

    NAS_EMM_ProcRejCauOtherAbnormal();

    /*��MRRC����NAS_EMM_MRRC_REL_REQ��Ϣ*/
    NAS_EMM_RelReq(                     NAS_LMM_NOT_BARRED);

    if (EMM_ATTACH_CAUSE_ESM_EMC_ATTACH == NAS_EMM_GLO_AD_GetAttCau())
    {
        /* ֪ͨIMSA�ٳ���CS�� */
    }

    return;
}
VOS_VOID    NAS_EMM_RejCauseValProc(NAS_EMM_CN_CAUSE_ENUM_UINT8 ucRejCauseVal)
{
    /*���ݲ�ͬ�ľܾ�ԭ��ֵ���д���*/
    switch (ucRejCauseVal)
    {
        case    NAS_LMM_CAUSE_IMEI_NOT_ACCEPTED:
                NAS_EMM_ProcRejCauseVal5();
                break;
        case    NAS_LMM_CAUSE_ILLEGAL_UE:
        case    NAS_LMM_CAUSE_ILLEGAL_ME:
        case    NAS_LMM_CAUSE_EPS_SERV_NOT_ALLOW:
        case    NAS_LMM_CAUSE_EPS_SERV_AND_NON_EPS_SERV_NOT_ALLOW:
                NAS_EMM_ProcRejCauseVal3678();
                break;

        case    NAS_LMM_CAUSE_PLMN_NOT_ALLOW:
                NAS_EMM_ProcRejCauseVal11();
                break;
        case    NAS_LMM_CAUSE_REQUESTED_SER_OPTION_NOT_AUTHORIZED_IN_PLMN:
                if (NAS_RELEASE_CTRL)
                {
                    NAS_EMM_ProcRejCauseVal11();
                }
                else
                {
                    NAS_EMM_ProcRejCauseValOther(ucRejCauseVal);
                }
                break;
        case    NAS_LMM_CAUSE_TA_NOT_ALLOW:
                NAS_EMM_ProcRejCauseVal12();
                break;

        case    NAS_LMM_CAUSE_ROAM_NOT_ALLOW:
                NAS_EMM_ProcRejCauseVal13();
                break;

        case    NAS_LMM_CAUSE_EPS_SERV_NOT_ALLOW_IN_PLMN:
                NAS_EMM_ProcRejCauseVal14();
                break;

        case    NAS_LMM_CAUSE_NO_SUITABL_CELL:
                NAS_EMM_ProcRejCauseVal15();
                break;
/* CSG������δʵ�֣��յ�REJ #25����CSGС����������default�����֧*/
#if 0
        case    NAS_LMM_CAUSE_NOT_AUTHORIZED_FOR_THIS_CSG:
                NAS_EMM_ProcRejCauseVal25();
                break;
#endif
        default:
                /*NAS_EMM_GLO_AD_GetConnRelCau() = EMM_CONN_REL_CAUSE_REJVAL_OTHER;*/
                NAS_EMM_ProcRejCauseValOther(ucRejCauseVal);
                break;

    }

}
VOS_VOID  NAS_EMM_ProcEpsOnlySucc
(
    VOS_VOID                *pstRcvMsg
)
{
    NAS_EMM_CN_ATTACH_ACP_STRU          *pstAttAcp;

    /*��ӡ����ú���*/
    NAS_EMM_ATTACH_LOG_NORM("NAS_EMM_ProcEpsOnlySucc is entered!");

    pstAttAcp = (NAS_EMM_CN_ATTACH_ACP_STRU *)pstRcvMsg;
    /*lint -e961*/
    if (NAS_EMM_BIT_SLCT == pstAttAcp->ucBitOpEmmCau)
    {
        NAS_EMM_ATTACH_LOG1_NORM("NAS_EMM_ProcEpsOnlySucc EmmCause:",pstAttAcp->ucEMMCause);
        NAS_EMM_ProcEpsOnlyWithCause(pstRcvMsg);
    }
    else
    {
        NAS_EMM_ATTACH_LOG_NORM("NAS_EMM_ProcEpsOnlySucc No EmmCause");
        NAS_EMM_ProcEpsOnlyNoCause();
    }
    /*lint +e961*/
    return;
}
VOS_VOID  NAS_EMM_ProcEpsOnlyWithCause
(
    VOS_VOID                *pstRcvMsg
)
{
    NAS_EMM_CN_ATTACH_ACP_STRU          *pstAttAcp;

    pstAttAcp = (NAS_EMM_CN_ATTACH_ACP_STRU *)pstRcvMsg;

    NAS_EMM_GLO_AD_GetAttRejCau() = pstAttAcp->ucEMMCause;

    switch (pstAttAcp->ucEMMCause)
    {
        case    NAS_LMM_CAUSE_IMSI_UNKNOWN_IN_HSS:
                NAS_EMM_ProcRejCauseVal2();
                break;

        case    NAS_LMM_CAUSE_CS_NOT_AVAIL:
                NAS_EMM_ProcRejCauseVal18();
                break;
        default:
                NAS_EMM_ProcRejCauseVal161722();
                break;
        #if 0
        case    NAS_LMM_CAUSE_MSC_UNREACHABLE:
        case    NAS_LMM_CAUSE_NETWORK_FAILURE:
        case    NAS_LMM_CAUSE_PROCEDURE_CONGESTION:
                NAS_EMM_ProcRejCauseVal161722();
                break;

        default :
                /*��ESM����ATTACH���*/
                NAS_EMM_AttSendEsmResult(EMM_ESM_ATTACH_STATUS_DETACHED);

                /*NAS_EMM_GLO_AD_GetConnRelCau() = EMM_CONN_REL_CAUSE_EPS_ONLY_REJVAL_OTHER;*/
                NAS_EMM_ProcEpsOnlyOtherCause(pstAttAcp->ucEMMCause);
                break;
      #endif
    }
    return;
}


VOS_VOID  NAS_EMM_ProcEpsOnlyNoCause( VOS_VOID)
{

    NAS_EMM_ProcRejCauseVal161722();

    #if 0
    /*��ESM����ATTACH���*/
    NAS_EMM_AttSendEsmResult(EMM_ESM_ATTACH_STATUS_DETACHED);

    NAS_EMM_GLO_AD_GetAttAtmpCnt() ++;

    NAS_EMM_AttProcEpsOnlyRejValueOther();
    #endif

    return;
}
#if 0
VOS_VOID  NAS_EMM_ProcEpsOnlyOtherCause( NAS_EMM_CN_CAUSE_ENUM_UINT8 ucRejCauseVal )
{
    if((NAS_LMM_CAUSE_ESM_FAILURE == ucRejCauseVal) ||
       (NAS_LMM_CAUSE_SEMANTICALLY_INCORRECT_MSG == ucRejCauseVal) ||
       (NAS_LMM_CAUSE_INVALID_MANDATORY_INF == ucRejCauseVal)       ||
       (NAS_LMM_CAUSE_MSG_NONEXIST_NOTIMPLEMENTE == ucRejCauseVal)  ||
       (NAS_LMM_CAUSE_IE_NONEXIST_NOTIMPLEMENTED == ucRejCauseVal)  ||
       (NAS_LMM_CAUSE_PROTOCOL_ERROR == ucRejCauseVal))
    {
        NAS_EMM_GLO_AD_GetAttAtmpCnt()  = 5;
    }
    else
    {
        NAS_EMM_GLO_AD_GetAttAtmpCnt() ++;
    }

    NAS_EMM_AttProcEpsOnlyRejValueOther();

    return;
}
#endif



VOS_UINT32 NAS_EMM_TimerValTransfer(NAS_EMM_TIMER_UNIT_ENUM_UINT8        ucUnit,
                                  VOS_UINT8                           ucTimerVal)
{
    VOS_UINT32                          ulTimerValue = 0;

    switch(ucUnit)
    {
        case NAS_EMM_TIMER_UNIT_2SENDS:
             ulTimerValue = ucTimerVal*2*NAS_EMM_SECOND_TRANSFER_MILLISECOND;
             break;

        case NAS_EMM_TIMER_UNIT_1MINUTE:
             ulTimerValue = ucTimerVal*NAS_EMM_MINUTE_TRANSFER_MILLISECOND;
             break;

        case NAS_EMM_TIMER_UNIT_6MINUTES:
             ulTimerValue = ucTimerVal*6*NAS_EMM_MINUTE_TRANSFER_MILLISECOND;
             break;

             /*��ʱ��ʱ����Ϊ�㣬���������ö�ʱ��*/
        case NAS_EMM_TIMER_DEACTIVATE:
             ulTimerValue = 0;
             break;

             /*����ֵ����λΪ1min*/
        default:
             ulTimerValue = ucTimerVal*NAS_EMM_MINUTE_TRANSFER_MILLISECOND;
             break;
    }

    return ulTimerValue;

}

VOS_VOID NAS_EMM_AttSetLai(VOS_VOID                *pstRcvMsg)
{
    NAS_EMM_CN_ATTACH_ACP_STRU          *pstAttAcp;

    if (NAS_EMM_NULL_PTR == pstRcvMsg)
    {
        /*��ӡ����*/
        NAS_EMM_ATTACH_LOG_ERR("NAS_EMM_AppSendAttSucc: Input para is invalid!");
        return;
    }

    pstAttAcp = (NAS_EMM_CN_ATTACH_ACP_STRU *)pstRcvMsg;

    if(pstAttAcp->ucBitOpLai == NAS_EMM_AD_BIT_SLCT)
    {
        NAS_EMM_GLO_AD_OP_GetLai()      = NAS_EMM_BIT_SLCT;
        NAS_LMM_MEM_CPY(                NAS_EMM_GLO_AD_GetLaiAddr(),
                                        &pstAttAcp->stLai,
                                        sizeof(MMC_LMM_LAI_STRU));
    }
    else
    {
        NAS_EMM_GLO_AD_OP_GetLai()   = NAS_EMM_AD_BIT_NO_SLCT;
        NAS_LMM_MEM_SET(                NAS_EMM_GLO_AD_GetLaiAddr(),
                                        0,
                                        sizeof(MMC_LMM_LAI_STRU));
    }
    return;
}
VOS_VOID NAS_EMM_AttSetEplms(VOS_VOID                *pstRcvMsg)
{
    NAS_EMM_CN_ATTACH_ACP_STRU          *pstAttAcp;

    if (NAS_EMM_NULL_PTR == pstRcvMsg)
    {
        /*��ӡ����*/
        NAS_EMM_ATTACH_LOG_ERR("NAS_EMM_AppSendAttSucc: Input para is invalid!");
        return;
    }

    pstAttAcp = (NAS_EMM_CN_ATTACH_ACP_STRU *)pstRcvMsg;

    if(pstAttAcp->ucBitOpPlmnList == NAS_EMM_AD_BIT_SLCT)
    {
        NAS_LMM_GetEmmInfoNetInfoOpEplmnListAddr()   = NAS_EMM_AD_BIT_SLCT;
        NAS_LMM_MEM_CPY(                NAS_LMM_GetEmmInfoNetInfoEplmnListAddr(),
                                        &pstAttAcp->stPlmnList,
                                        sizeof(MMC_LMM_EPLMN_STRU));
    }
    else
    {
        NAS_LMM_GetEmmInfoNetInfoOpEplmnListAddr()   = NAS_EMM_AD_BIT_NO_SLCT;

        NAS_LMM_MEM_SET(                NAS_LMM_GetEmmInfoNetInfoEplmnListAddr(),
                                        0,
                                        sizeof(MMC_LMM_EPLMN_STRU));
    }
    return;
}
VOS_VOID NAS_EMM_AttSetMsId(VOS_VOID                *pstRcvMsg)
{
    NAS_EMM_CN_ATTACH_ACP_STRU          *pstAttAcp;

    if (NAS_EMM_NULL_PTR == pstRcvMsg)
    {
        /*��ӡ����*/
        NAS_EMM_ATTACH_LOG_ERR("NAS_EMM_AppSendAttSucc: Input para is invalid!");
        return;
    }

    pstAttAcp = (NAS_EMM_CN_ATTACH_ACP_STRU *)pstRcvMsg;

    if(pstAttAcp->ucBitOpMsId == NAS_EMM_AD_BIT_SLCT)
    {
        NAS_EMM_GLO_AD_OP_GetMsId()   = NAS_EMM_AD_BIT_SLCT;
        NAS_LMM_MEM_CPY(                NAS_EMM_GLO_AD_GetMsIdAddr(),
                                        &pstAttAcp->stMsId,
                                        sizeof(NAS_EMM_MSID_STRU));
    }
    else
    {
        NAS_EMM_GLO_AD_OP_GetMsId()   = NAS_EMM_AD_BIT_NO_SLCT;
        NAS_LMM_MEM_SET(                NAS_EMM_GLO_AD_GetMsIdAddr(),
                                        0,
                                        sizeof(NAS_EMM_MSID_STRU));
    }
    return;
}

/*****************************************************************************
 Function Name   : NAS_EMM_SaveAttachAcpIe
 Description     : �洢ATTACH ACCPET��Я������Ԫ
 Input           : pMsgStru------------ATTACH ACCEPT��Ϣָ��
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2012-02-27  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_EMM_SaveAttachAcpIe
(
    VOS_VOID                           *pMsgStru
)
{
    NAS_EMM_CN_ATTACH_ACP_STRU         *pRcvEmmMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulTimerLen = NAS_EMM_NULL;

    pRcvEmmMsg                          = (NAS_EMM_CN_ATTACH_ACP_STRU *)pMsgStru;

    /*����ATTACH���*/
    NAS_EMM_GLO_AD_SetAttRslt(pRcvEmmMsg->ucAttachRst);

    /*����T3412 VALUE*/
    ulTimerLen = NAS_EMM_TimerValTransfer(pRcvEmmMsg->stT3412Value.ucUnit,
                                           pRcvEmmMsg->stT3412Value.ucTimerVal);
    NAS_LMM_ModifyPtlTimer(TI_NAS_EMM_PTL_T3412,ulTimerLen);

     /*����TAI LIST*/
    NAS_LMM_MEM_CPY(                    NAS_EMM_GLO_AD_GetTaiListAddr(),
                                        &(pRcvEmmMsg->stTaiList),
                                        sizeof(NAS_EMM_TA_LIST_STRU));

    /*�����µ�TAI List��ͬʱ����ForbTA for Roaming��ForbTA for RPOS*/
    NAS_EMMC_UpdateForbTaList();

    /*��ESM����ACTIVATE DEFAULT EPS BEARER REQUEST��Ϣ*/
    NAS_EMM_EsmSendDataInd(              &(pRcvEmmMsg->stMsgCon));

    /* ATTACH ACPЯ��GUTI,����GUTI��EMM��ȫ�ֱ�����*/
    if(NAS_EMM_BIT_SLCT == pRcvEmmMsg->ucBitOpGuti)
    {
        NAS_EMM_SetGuti(&(pRcvEmmMsg->stGuti));

    }

    /* Location area identification,MS identity,EMM cause,EPLMNS�յ�ATTACH ACCEPT���棬
        ����ATTACH COMPLETE�����ATTACHʧ�ܴӱ��ر���Ķ�ȡ��Я����MMC */

    NAS_EMM_AttSetLai(pMsgStru);

    NAS_EMM_AttSetEplms(pMsgStru);

    NAS_EMM_AttSetMsId(pMsgStru);

    if(pRcvEmmMsg->ucBitOpEmmCau == NAS_EMM_AD_BIT_SLCT)
    {
        NAS_EMM_GLO_AD_OP_GetEmmCau() = NAS_EMM_AD_BIT_SLCT;
        NAS_EMM_GLO_AD_GetEmmCau()    = pRcvEmmMsg->ucEMMCause;
    }
    else
    {
        NAS_EMM_GLO_AD_OP_GetEmmCau()   = NAS_EMM_AD_BIT_NO_SLCT;
        NAS_EMM_GLO_AD_GetEmmCau()      = 0;
    }

    /*ATTACH ACPЯ��T3402,�޸Ķ�ʱ��ʱ��*/
    if(pRcvEmmMsg->ucBitOpT3402 == NAS_EMM_AD_BIT_SLCT)
    {
        ulTimerLen = NAS_EMM_TimerValTransfer(pRcvEmmMsg->stT3402Value.ucUnit,
                                            pRcvEmmMsg->stT3402Value.ucTimerVal);
        NAS_LMM_ModifyPtlTimer(TI_NAS_EMM_PTL_T3402,ulTimerLen);

    }
    else
    {
        /*���ATTACH ACP��Ϣ�в�Я��T3402��Ԫ����ָ�Ĭ��ֵ12min*/
        NAS_LMM_ModifyPtlTimer(TI_NAS_EMM_PTL_T3402,NAS_LMM_TIMER_T3402_LEN);
    }
    /*leili modify for isr begin*/
    /*ATTACH ACPЯ��T3423,�޸Ķ�ʱ��ʱ��*/
    if(pRcvEmmMsg->ucBitOpT3423 == NAS_EMM_AD_BIT_SLCT)
    {
        ulTimerLen = NAS_EMM_TimerValTransfer(pRcvEmmMsg->stT3423Value.ucUnit,
                                            pRcvEmmMsg->stT3423Value.ucTimerVal);
        NAS_LMM_ModifyPtlTimer(TI_NAS_EMM_PTL_T3423,ulTimerLen);

    }
    else
    {
        /*���ATTACH ACP��Ϣ�в�Я��T3423��Ԫ����ָ�Ĭ��ֵΪT3412ʱ��*/
        NAS_LMM_GetPtlTimerLen(TI_NAS_EMM_PTL_T3412, &ulTimerLen);
        NAS_LMM_ModifyPtlTimer(TI_NAS_EMM_PTL_T3423,ulTimerLen);
    }
    /*leili modify for isr end*/

    /*����Emergency number list��ȫ�ֱ�����*/
    NAS_EMM_SetEmergencyList(   pRcvEmmMsg->ucBitOpEmergencyList,
                                &(pRcvEmmMsg->stEmergencyNumList));

    /*����EPS network feature support��ȫ�ֱ�����*/
    NAS_EMM_SetEpsNetFeature(   pRcvEmmMsg->ucBitOpEpsNetFeature,
                                pRcvEmmMsg->ucEpsNetFeature);

    /*����Additional update result��ȫ�ֱ�����*/
    if(EMM_ATTACH_RST_PS == pRcvEmmMsg->ucAttachRst)
    {
        /* ��EPS ONLY�ɹ� ��ulAdditionUpRslt����ΪNAS_LMM_ADDITIONAL_UPDATE_NOT_ATTACHED */
        NAS_EMM_SetAddUpdateRslt(NAS_EMM_BIT_SLCT, NAS_LMM_ADDITIONAL_UPDATE_NOT_ATTACHED);
    }
    else
    {
        NAS_EMM_SetAddUpdateRslt(   pRcvEmmMsg->ucBitOpAdditionalUpRslt,
                                    pRcvEmmMsg->ulAdditionUpRslt);
    }

    /* lihong00150010 ims begin */
    /* ֪ͨMMC�����������б������Ƿ�֧��IMS VOICE�ͽ��������Լ�LTE��CS������ */
    /* NAS_EMM_SendGmmInfoChangeNotify(); */
    NAS_EMM_SendMmcInfoChangeNotify();
    /*NAS_EMM_SendMmInfoChangeNotify();*/
    /* lihong00150010 ims end */
}
/*lint +e960*/

VOS_UINT32  NAS_EMM_MsRegInitSsWtCnAttCnfMsgCnAttachAcp(VOS_UINT32  ulMsgId,
                                                VOS_VOID   *pMsgStru)
{
    NAS_EMM_CN_ATTACH_ACP_STRU         *pRcvEmmMsg;
    NAS_LMM_NETWORK_INFO_STRU          *pMmNetInfo = VOS_NULL_PTR;
#if (VOS_OS_VER != VOS_WIN32)
    static VOS_UINT32  s_ulAttachAcpCnt = 1;
#endif

    (VOS_VOID)ulMsgId;

    pRcvEmmMsg                          = (NAS_EMM_CN_ATTACH_ACP_STRU *)pMsgStru;

    /*��ӡ����ú���*/
    NAS_EMM_ATTACH_LOG_NORM("NAS_EMM_MsRegInitSsWtCnAttCnfMsgCnAttachAcp is entered");

    /*���״̬�Ƿ�ƥ�䣬����ƥ�䣬�˳�*/
    if (NAS_EMM_AD_CHK_STAT_INVALID(EMM_MS_REG_INIT,EMM_SS_ATTACH_WAIT_CN_ATTACH_CNF))
    {
        /*��ӡ������Ϣ*/
        NAS_EMM_ATTACH_LOG_WARN("NAS_EMM_MsRegInitSsWtCnAttCnfMsgCnAttachAcp: STATE ERR!");
        return  NAS_LMM_MSG_DISCARD;
    }

    /* ���ATTACH ACP��Ϣ���ݲ��Ϸ�,��������Ϣ��ֱ�Ӷ���*/
    if (NAS_EMM_PARA_INVALID == NAS_EMM_CnAttachAcpMsgChk(pRcvEmmMsg))
    {
        NAS_EMM_ATTACH_LOG_ERR("NAS_EMM_MsRegInitSsWtCnAttCnfMsgCnAttachAcp: NAS_EMM_CN_ATTACH_ACC_STRU para err!");
        return  NAS_LMM_ERR_CODE_PARA_INVALID;

    }


    /*ֹͣ��ʱ��T3410*/
    NAS_LMM_StopStateTimer(          TI_NAS_EMM_T3410);

    /* �洢ATTACH ACCPET��Я������Ԫ */
    NAS_EMM_SaveAttachAcpIe(pMsgStru);


    /*���ATTACH ATTEMPTING COUNTER��TRACKING AREA UPDATING ATTEMPT������*/
    /*NAS_EMM_GLO_AD_GetAttAtmpCnt()      = 0;*/
    NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);

    /*ע��ɹ���ע��ǰ��TAU/SERVICE/DETACHģ���¼����ʷ��Ϣ��Ӧ��Ӱ���������
      ���TAU/service/detach��Դ
    */
    NAS_EMM_TAU_ClearResouce();
    NAS_EMM_SER_ClearResource();
    NAS_EMM_Detach_ClearResourse();

    /*����ע���EPS ONLY�ɹ�*/
    if ((MMC_LMM_ATT_TYPE_COMBINED_EPS_IMSI == NAS_EMM_GLO_GetCnAttReqType()) &&
        (EMM_ATTACH_RST_PS == pRcvEmmMsg->ucAttachRst))
    {
        NAS_EMM_ProcEpsOnlySucc(pMsgStru);
    }
    else
    {
        /*����EPS UPDATE STAUSΪEU1*/
        NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_UPDATED_EU1;

        /*������ʱ��TI_NAS_EMM_WAIT_ESM_BEARER_CNF*/
        NAS_LMM_StartStateTimer(         TI_NAS_EMM_WAIT_ESM_BEARER_CNF);

        /*�޸�״̬��������״̬REG_INIT��״̬ATTACH_WAIT_ESM_BEARER_CNF*/
        NAS_EMM_AdStateConvert(             EMM_MS_REG_INIT,
                                            EMM_SS_ATTACH_WAIT_ESM_BEARER_CNF,
                                            TI_NAS_EMM_WAIT_ESM_BEARER_CNF);
    }

    /* lihong00150010 emergency tau&service begin */
    /* ���ձ�����������յ�ATTACH ACCEPTʱ�͸���GUTI, LAST VISIT TAI��EUֵ��
       ��д������NV */
    pMmNetInfo                          = NAS_LMM_GetEmmInfoNetInfoAddr();
    NAS_EMM_SetLVRTai(                  &pMmNetInfo->stPresentNetId);

    /* �ж�û�зŵ�����NAS_LMM_WritePsLoc�������������ڽ���ע�ᱻ�ܻ��߳��Դ���
       �ﵽ5��ʱ����Ҫɾ������ ��Э��ֻ�涨�ǽ���ע��ɹ���Ų�д������NV�� */
    if (NAS_LMM_REG_STATUS_EMC_REGING != NAS_LMM_GetEmmInfoRegStatus())
    {
        /*����PS LOC��Ϣ*/
        NAS_LMM_WritePsLoc(NAS_NV_ITEM_UPDATE);
    }
    /* lihong00150010 emergency tau&service end */

    /* ������뼼���仯��ʶ�����������������ATTACHʧ�ܵ�ԭ��ֵ
          ��ΪҪô��Ȼת����DEREG̬��ҪôMME�Ѿ���ѯ��UE��������*/
    NAS_EMM_ClearUeRadioAccCapChgFlag();

#if (VOS_OS_VER != VOS_WIN32)
    if(1 == s_ulAttachAcpCnt)
    {
        (VOS_VOID)ddmPhaseScoreBoot("UE attach success",__LINE__);
        s_ulAttachAcpCnt++;
    }
#endif

    return NAS_LMM_MSG_HANDLED;
}


/*lint -e960*/
VOS_VOID    NAS_EMM_SendRejEventToSTK(VOS_UINT8 ucCauseCode,
                      LNAS_STK_UPDATE_TYPE_ENUM_UINT8          enUpdateType)
{
    LNAS_STK_NETWORK_REJECTION_EVENT_STRU       *pEmmNetworkRejectMsg;
    NAS_MM_PLMN_ID_STRU     *pEmmPlmnID;
    NAS_MM_TAC_STRU         *pEmmTac;

    NAS_EMM_ATTACH_LOG_NORM("NAS_EMM_SendRejEventToSTK is entered");
    /*������Ϣ�ڴ�*/
    pEmmNetworkRejectMsg   = (VOS_VOID *)NAS_LMM_ALLOC_MSG(sizeof(LNAS_STK_NETWORK_REJECTION_EVENT_STRU));

    /*�ж�����������ʧ�ܴ�ӡ�����˳�*/
    if (NAS_EMM_NULL_PTR == pEmmNetworkRejectMsg)
    {
        /*��ӡ����*/
        NAS_EMM_ATTACH_LOG_ERR("NAS_EMM_SendRejEventToSTK: MSG ALLOC ERR!");
        return;

    }

    /*����ID_LNAS_STK_NETWORK_REJECTION_EVENT��Ϣ*/
    /*�����Ϣͷ*/
    NAS_EMM_STK_MSG_HEADER(      pEmmNetworkRejectMsg,
                                        (sizeof(LNAS_STK_NETWORK_REJECTION_EVENT_STRU)-
                                        NAS_EMM_LEN_VOS_MSG_HEADER));

    /*�����ϢID*/
    pEmmNetworkRejectMsg->ulMsgName = ID_LNAS_STK_NETWORK_REJECTION_EVENT;

    /*�����Ϣ����*/
    pEmmNetworkRejectMsg->stNetworkRejectionEvent.ucCauseCode = ucCauseCode;

    /*�����Ϣplmn*/
    pEmmPlmnID = NAS_LMM_GetEmmInfoPresentPlmnAddr();
    pEmmNetworkRejectMsg->stNetworkRejectionEvent.stTaiInfo.stCurPlmnID.aucPlmnId[0] =
                        pEmmPlmnID->aucPlmnId[0];
    pEmmNetworkRejectMsg->stNetworkRejectionEvent.stTaiInfo.stCurPlmnID.aucPlmnId[1] =
                        pEmmPlmnID->aucPlmnId[1];
    pEmmNetworkRejectMsg->stNetworkRejectionEvent.stTaiInfo.stCurPlmnID.aucPlmnId[2] =
                        pEmmPlmnID->aucPlmnId[2];

    /*�����ϢTAC*/
    pEmmTac = NAS_LMM_GetEmmInfoPresentTacAddr();
    pEmmNetworkRejectMsg->stNetworkRejectionEvent.stTaiInfo.stTac.ucTac    = pEmmTac->ucTac;
    pEmmNetworkRejectMsg->stNetworkRejectionEvent.stTaiInfo.stTac.ucTacCnt = pEmmTac->ucTacCnt;

    /*�����ϢenUpdateAttachType*/
    pEmmNetworkRejectMsg->stNetworkRejectionEvent.enUpdateAttachType = enUpdateType;

    /*��stk����rejection��Ϣ*/
    NAS_LMM_SEND_MSG(pEmmNetworkRejectMsg);

    return;
}
/*lint +e960*/

VOS_UINT32  NAS_EMM_MsRegInitSsWtCnAttCnfMsgCnAttachReject(
                                                VOS_UINT32  ulMsgId,
                                                VOS_VOID   *pMsgStru )
{
    NAS_EMM_CN_ATTACH_REJ_STRU          *pRcvEmmMsg;
    LNAS_STK_UPDATE_TYPE_ENUM_UINT8     uAttachType;

    (VOS_VOID)ulMsgId;

    pRcvEmmMsg                          = (NAS_EMM_CN_ATTACH_REJ_STRU *) pMsgStru;

    /*��ӡ����ú���*/
    NAS_EMM_ATTACH_LOG_NORM("NAS_EMM_MsRegInitSsWtCnAttCnfMsgCnAttachReject is entered");

    /*���״̬�Ƿ�ƥ�䣬����ƥ�䣬�˳�*/
    if (NAS_EMM_AD_CHK_STAT_INVALID(EMM_MS_REG_INIT,EMM_SS_ATTACH_WAIT_CN_ATTACH_CNF))
    {
        /*��ӡ������Ϣ*/
        NAS_EMM_ATTACH_LOG_WARN("NAS_EMM_MsRegInitSsWtCnAttCnfMsgCnAttachReject: STATE ERR!");
        return  NAS_LMM_MSG_DISCARD;
    }

    /*��Ϣ���ݼ��*/
    if (NAS_EMM_PARA_INVALID == NAS_EMM_CnAttachRejMsgChk(pRcvEmmMsg))
    {
        NAS_EMM_ATTACH_LOG_ERR("NAS_EMM_MsRegInitSsWtCnAttCnfMsgCnAttachReject: NAS_EMM_CN_ATTACH_REJ_STRU para err!");
        return  NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /*ֹͣ��ʱ��T3410*/
    NAS_LMM_StopStateTimer(              TI_NAS_EMM_T3410);

    /*����ATTACH�ܾ�ԭ��*/
    NAS_EMM_GLO_AD_GetAttRejCau()       = pRcvEmmMsg->ucCause;
    NAS_EMM_GLO_AD_GetAttRejEsmCau()    = pRcvEmmMsg->ucEsmCause;

    /* xiongxianghui00253310 modify for ftmerrlog begin */
    #if (FEATURE_PTM == FEATURE_ON)
    NAS_LMM_ErrlogInfoProc(pMsgStru, LMM_ERR_LOG_ATTACH_PROCEDURE);
    #endif
    /* xiongxianghui00253310 modify for ftmerrlog end   */

    /* ���ݵ�ǰ���������Ƿ��޸�ԭ��ֵ */
    NAS_LMM_AdaptRegRejCau(&NAS_EMM_GLO_AD_GetAttRejCau());

    if(pRcvEmmMsg->ucBitOpMsgCon == NAS_EMM_AD_BIT_SLCT)
    {
        /*��ESM������Ϣ*/
        NAS_EMM_EsmSendDataInd(          &(pRcvEmmMsg->stMsgCon));
    }

    switch(NAS_EMM_GLO_GetCnAttReqType())
    {
        case MMC_LMM_ATT_TYPE_EPS_ONLY:
                uAttachType = LNAS_STK_UPDATE_TYPE_EPS_ATTACH;
                break;

        case  MMC_LMM_ATT_TYPE_COMBINED_EPS_IMSI:
                uAttachType = LNAS_STK_UPDATE_TYPE_COMBINED_ATTACH;
                break;

        default:
                uAttachType = LNAS_STK_UPDATE_TYPE_EPS_ATTACH;
                break;

    }

    /*��ATTACH�����г���rej����rej�¼������STK*/
    NAS_EMM_SendRejEventToSTK(NAS_EMM_GLO_AD_GetAttRejCau(), uAttachType);

    /*�жϾܾ�ԭ��ֵ,���ݲ�ͬԭ��ֵ���д���*/
    NAS_EMM_RejCauseValProc(NAS_EMM_GLO_AD_GetAttRejCau());

    return  NAS_LMM_MSG_HANDLED;
}
/*lint +e960*/


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

