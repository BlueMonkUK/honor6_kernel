


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include  "NasLmmPubMInclude.h"
#include  "NasMmlCtx.h"
#include  "NasEmmAttachEsmMsgProcess.h"
#include  "NasEmmAttDetInclude.h"
/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASEMMPLMN_C
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

/*****************************************************************************
  3.1  ��Ϣ���� Function
*****************************************************************************/

/*lint -e960*/
/*lint -e961*/
VOS_UINT32  NAS_EMM_PLMN_SendAppStartCnf(
                    VOS_UINT32                              ulAppRslt,
                    VOS_UINT32                              ulOpId)
{
    LMM_MMC_START_CNF_STRU             *pstAppStartCnfMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulSendResult;

    /* ����MMC�ڲ���Ϣ */
    pstAppStartCnfMsg  = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_START_CNF_STRU));

    if (NAS_EMM_PLMN_NULL_PTR == pstAppStartCnfMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendAppStartCnf: MSG ALLOC ERROR!!!");
        return NAS_EMM_PLMN_ERR;
    }

    NAS_LMM_MEM_SET(pstAppStartCnfMsg,0,sizeof(LMM_MMC_START_CNF_STRU));

    /* ���VOS��Ϣͷ */
    EMM_PUBU_COMP_MMC_MSG_HEADER((pstAppStartCnfMsg),
                             NAS_EMM_PLMN_MSG_LENGTH_NO_HEADER(LMM_MMC_START_CNF_STRU));

    /* �����ϢID */
    pstAppStartCnfMsg->ulMsgId          = ID_LMM_MMC_START_CNF;

    /* �����Ϣ�� */
    pstAppStartCnfMsg->ulOpId           = ulOpId;
    pstAppStartCnfMsg->ulRst            = ulAppRslt;

    /* ����MMC��Ϣ */
    NAS_LMM_SendLmmMmcMsg((VOS_VOID*)pstAppStartCnfMsg);

    ulSendResult                        = NAS_EMM_OK;

    return ulSendResult;
}


VOS_UINT32  NAS_EMM_PLMN_SendAppStopCnf(
                    VOS_UINT32                              ulAppRslt,
                    VOS_UINT32                              ulOpId)
{
    LMM_MMC_STOP_CNF_STRU              *pstAppStopCnfMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulSendResult;

    (void)(ulAppRslt);

    /* ����MMC�ڲ���Ϣ */
    pstAppStopCnfMsg = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_STOP_CNF_STRU));
    if (NAS_EMM_PLMN_NULL_PTR == pstAppStopCnfMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendAppStopCnf: MSG ALLOC ERROR!!!");
        return NAS_EMM_PLMN_ERR;
    }
    NAS_LMM_MEM_SET(pstAppStopCnfMsg,0,sizeof(LMM_MMC_STOP_CNF_STRU));

    /* ���VOS��Ϣͷ */
    EMM_PUBU_COMP_MMC_MSG_HEADER((pstAppStopCnfMsg),
         NAS_EMM_PLMN_MSG_LENGTH_NO_HEADER(LMM_MMC_STOP_CNF_STRU));

    /* �����ϢID */
    pstAppStopCnfMsg->ulMsgId           = ID_LMM_MMC_STOP_CNF;

    /* �����Ϣ�� */
    pstAppStopCnfMsg->ulOpId            = ulOpId;

    /* ����MMC��Ϣ */
    NAS_LMM_SendLmmMmcMsg((VOS_VOID*)pstAppStopCnfMsg);

    ulSendResult                        = NAS_EMM_OK;

    return ulSendResult;
}



VOS_UINT32  NAS_EMM_PLMN_SendRrcStartReq( VOS_VOID )
{
    LRRC_LMM_START_REQ_STRU            *pstRrcStartReqMsg;
    VOS_UINT32                          ulSendResult;


    /* ����DOPRA��Ϣ */
    pstRrcStartReqMsg = (VOS_VOID *) NAS_LMM_ALLOC_MSG(sizeof(LRRC_LMM_START_REQ_STRU));
    if(NAS_EMM_PLMN_NULL_PTR == pstRrcStartReqMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendRrcStartReq: MSG ALLOC ERROR!!!");
        return NAS_EMM_PLMN_ERR;
    }

    /* ����Ϊ0 */
    NAS_LMM_MEM_SET(pstRrcStartReqMsg,0,sizeof(LRRC_LMM_START_REQ_STRU));

    /* ���VOS��Ϣͷ */
    NAS_EMM_PLMN_COMP_RRC_MSG_HEADER((pstRrcStartReqMsg),
                                      NAS_EMM_PLMN_MSG_LENGTH_NO_HEADER(LRRC_LMM_START_REQ_STRU));

    /* �����ϢID */
    pstRrcStartReqMsg->enMsgId          = ID_LRRC_LMM_START_REQ;

    /* �����Ϣ�� */
    /* ��俨״̬ */
    NAS_EMM_SET_LRRCLMM_INF_USIM_STATE(pstRrcStartReqMsg->enSimStatus);


    pstRrcStartReqMsg->enLteRatPrio                             = NAS_LMM_GetCurLtePrio();


    pstRrcStartReqMsg->aucRatPrioList[NAS_LMM_RAT_TYPE_GSM]     = NAS_LMM_GetCurGsmPrio();
    pstRrcStartReqMsg->aucRatPrioList[NAS_LMM_RAT_TYPE_WCDMA]   = NAS_LMM_GetCurWcdmaPrio();
    pstRrcStartReqMsg->aucRatPrioList[NAS_LMM_RAT_TYPE_LTE]     = NAS_LMM_GetCurLtePrio();
    pstRrcStartReqMsg->aucRatPrioList[NAS_LMM_RAT_TYPE_CDMA1X]   = NAS_LMM_GetCurCdma1xPrio();
    pstRrcStartReqMsg->aucRatPrioList[NAS_LMM_RAT_TYPE_HRPD]     = NAS_LMM_GetCurHrpdPrio();

    pstRrcStartReqMsg->enUtranMode = NAS_LMM_GetEmmInfoUtranMode();

    /* ����DOPRA��Ϣ */
    NAS_LMM_SEND_MSG(                   pstRrcStartReqMsg);

    ulSendResult                        = NAS_EMM_OK;


    return ulSendResult;
}



VOS_UINT32  NAS_EMM_PLMN_SendRrcStopReq( VOS_VOID )
{
    RRC_MM_STOP_REQ_STRU                *pstRrcStopReqMsg;
    VOS_UINT32                          ulSendResult;

    /* ����DOPRA��Ϣ */
    pstRrcStopReqMsg = (VOS_VOID *) NAS_LMM_ALLOC_MSG(sizeof(RRC_MM_STOP_REQ_STRU));
    if (NAS_EMM_PLMN_NULL_PTR == pstRrcStopReqMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendRrcStopReq: MSG ALLOC ERROR!!!");
        return NAS_EMM_PLMN_ERR;
    }

    NAS_LMM_MEM_SET(pstRrcStopReqMsg,0,sizeof(RRC_MM_STOP_REQ_STRU));

    /* ���VOS��Ϣͷ */
    NAS_EMM_PLMN_COMP_RRC_MSG_HEADER((pstRrcStopReqMsg),
                                    NAS_EMM_PLMN_MSG_LENGTH_NO_HEADER(RRC_MM_STOP_REQ_STRU))

    /* �����ϢID */
    pstRrcStopReqMsg->enMsgId           = ID_LRRC_LMM_POWER_OFF_REQ;

    /*���廷���£�RRC��Ϣ�����е���Ϣ����10�������Խ�����Ϣ��ʽ���͹ػ���Ϣ*/
    #if (VOS_OS_VER != VOS_WIN32)
    if(VOS_ERR == VOS_CheckTaskQueue(PS_PID_ERMM, NAS_EMM_PLMN_10_MSG_IN_QUEUE))
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendRrcStopReq: Snd Urgent Msg!");

        /*���RRC����Ϣ���г���10����Ϣ�����Խ�����Ϣ��ʽ���͹ػ���Ϣ*/
        NAS_LMM_SEND_URGENT_MSG(         pstRrcStopReqMsg);
    }
    else
    #endif
    {
        /*��������ͨ��Ϣ��ʽ����*/
        NAS_LMM_SEND_MSG(                pstRrcStopReqMsg);
    }

    ulSendResult                        = NAS_EMM_OK;

    return ulSendResult;
}
VOS_UINT32  NAS_EMM_PLMN_SendMmcStartReq(
                    VOS_UINT32                              ulStartCause )
{
    EMMC_EMM_START_REQ_STRU              *pstMmcStartReqMsg;
    VOS_UINT32                          ulMsgLengthNoHeader;
    VOS_UINT32                          ulSendResult;

    /* ����EMMҪ�����ڲ�����ģ����ڲ���Ϣ����, ������VOSͷ */
    ulMsgLengthNoHeader = NAS_EMM_PLMN_INTRAMSG_LENGTH_NO_HEADER(EMMC_EMM_START_REQ_STRU);

    /* ��ȡ�ڲ���Ϣ��ַ */
    pstMmcStartReqMsg = (EMMC_EMM_START_REQ_STRU *) NAS_LMM_GetSendIntraMsgBuffAddr(ulMsgLengthNoHeader);
    if (NAS_EMM_PLMN_NULL_PTR == pstMmcStartReqMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendMmcStartReq: GET INTRAMSG ADDR ERROR!!!");
        return NAS_EMM_PLMN_ERR;
    }

    /* ����ڲ���Ϣͷ */
    NAS_EMM_PLMN_COMP_MM_INTRAMSG_HEADER((pstMmcStartReqMsg), ulMsgLengthNoHeader);

    /* ����ڲ���ϢID */
    pstMmcStartReqMsg ->enMsgId         = ID_EMMC_EMM_START_REQ;

    /* �����Ϣ�� */
    pstMmcStartReqMsg->enCause          = ulStartCause;

    /* PLMN��ģ����MMC����MMC_EMM_START_REQ�ڲ���Ϣ */
    ulSendResult =                      NAS_EMM_PLMN_OK;
    NAS_EMM_SEND_INTRA_MSG(             pstMmcStartReqMsg);

    return ulSendResult;
}
VOS_UINT32  NAS_EMM_PLMN_SendMmcStopReq( VOS_VOID )
{
    EMMC_EMM_STOP_REQ_STRU               *pstMmcStopReqMsg;
    VOS_UINT32                          ulMsgLengthNoHeader;
    VOS_UINT32                          ulSendResult;

    /* ����EMMҪ�����ڲ�����ģ����ڲ���Ϣ����, ������VOSͷ */
    ulMsgLengthNoHeader = NAS_EMM_PLMN_INTRAMSG_LENGTH_NO_HEADER(EMMC_EMM_STOP_REQ_STRU);

    /* ��ȡ�ڲ���Ϣ��ַ */
    pstMmcStopReqMsg = (VOS_VOID *) NAS_LMM_GetSendIntraMsgBuffAddr(ulMsgLengthNoHeader);
    if (NAS_EMM_PLMN_NULL_PTR == pstMmcStopReqMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendMmcStopReq: GET INTRAMSG ADDR ERROR!!!");
        return NAS_EMM_PLMN_ERR;
    }

    /* ����ڲ���Ϣͷ */
    NAS_EMM_PLMN_COMP_MM_INTRAMSG_HEADER((pstMmcStopReqMsg), ulMsgLengthNoHeader);

    /* ����ڲ���ϢID */
    pstMmcStopReqMsg->enMsgId           = ID_EMMC_EMM_STOP_REQ;

    ulSendResult =                      NAS_EMM_PLMN_OK;
    NAS_EMM_SEND_INTRA_MSG(             pstMmcStopReqMsg);

    return ulSendResult;
}

VOS_VOID NAS_EMM_PLMN_SendEmmEsmStautsInd(EMM_ESM_ATTACH_STATUS_ENUM_UINT32 ulStatRst)
{
    EMM_ESM_STATUS_IND_STRU             *pEmmEsmStatMsg;

    NAS_EMM_PUBU_LOG1_NORM("NAS_EMM_PLMN_SendEmmEsmStautsInd", ulStatRst);

    /* ����DOPRA��Ϣ */
    pEmmEsmStatMsg = (VOS_VOID *) NAS_LMM_ALLOC_MSG(sizeof(EMM_ESM_STATUS_IND_STRU));
    if (NAS_EMM_PLMN_NULL_PTR == pEmmEsmStatMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendEmmEsmStautsInd: MSG ALLOC ERROR!!!");
        return;
    }

    /* ���VOS��Ϣͷ */
    NAS_EMM_PLMN_COMP_ESM_MSG_HEADER(pEmmEsmStatMsg,
                                    NAS_EMM_PLMN_MSG_LENGTH_NO_HEADER(EMM_ESM_STATUS_IND_STRU));

    /* �����ϢID */
    pEmmEsmStatMsg->ulMsgId             = ID_EMM_ESM_STATUS_IND;

    /*�����Ϣ����*/
    pEmmEsmStatMsg->enEMMStatus         = ulStatRst;

    /*clear global:EpsContextStatusChange*/
    if(ulStatRst == EMM_ESM_ATTACH_STATUS_DETACHED)
    {
        NAS_EMM_GetEpsContextStatusChange() = NAS_EMM_EPS_BEARER_STATUS_NOCHANGE;
    }
    /*��ESM����ID_EMM_ESM_STATUS_IND��Ϣ*/

    NAS_LMM_SEND_MSG(                   pEmmEsmStatMsg);

    return;

}

VOS_VOID NAS_EMM_PLMN_ImsiPagingProc(VOS_VOID)
{
    NAS_EMM_FSM_STATE_STRU              EmmState;
    VOS_UINT32                          ulRst;

    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_PLMN_ImsiPagingProc is entered!");


    EmmState.enFsmId                    = NAS_LMM_PARALLEL_FSM_EMM;
    EmmState.enMainState                = EMM_MS_DEREG;
    EmmState.enSubState                 = EMM_SS_DEREG_NORMAL_SERVICE;
    EmmState.enStaTId                   = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    /*��APP����APP_MM_DETACH_IND��Ϣ*/
    NAS_EMM_MmcSendDetIndLocal(MMC_LMM_L_LOCAL_DETACH_IMSI_PAGING);
    /* ����DETACH�ͷ���Դ:��̬�ڴ桢����ֵ */
    NAS_LMM_DeregReleaseResource();

    /* ɾ��GUTI����Զ�����EPS_LOC,������Ҫ������STATUS */
    NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_NOT_UPDATED_EU2;

    /*ɾ��GUTI,KSIasme,TAI list,LVR TAI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_NOT_DELETE_RPLMN);

    /*send INTRA ATTACH REQ*/
    ulRst = NAS_EMM_SendIntraAttachReq();
    if(NAS_EMM_PLMN_OK != ulRst)
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_ImsiPagingProc: send INTRA ATTACH REQ ERR !");
    }

    return;
}

VOS_VOID NAS_EMM_PLMN_STmsiPagingProc(VOS_VOID)
{
    VOS_UINT32                          ulCurEmmStat;
    NAS_LMM_EMM_PLMN_LOG_INFO(           "NAS_EMM_PLMN_STmsiPagingProc is entered!");

    ulCurEmmStat                        = NAS_LMM_PUB_COMP_EMMSTATE(
                                        NAS_EMM_CUR_MAIN_STAT,
                                        NAS_EMM_CUR_SUB_STAT);

    if (ulCurEmmStat == NAS_LMM_PUB_COMP_EMMSTATE( EMM_MS_REG, EMM_SS_REG_NORMAL_SERVICE))
    {
        NAS_EMM_SER_RcvRrcStmsiPagingInd();
    }
    else if (ulCurEmmStat == NAS_LMM_PUB_COMP_EMMSTATE( EMM_MS_REG, EMM_SS_REG_WAIT_ACCESS_GRANT_IND))
    {
        if(NAS_EMM_SUCC != NAS_EMM_JudgeBarType(NAS_EMM_BAR_TYPE_MT))
        {
            NAS_EMM_SER_RcvRrcStmsiPagingInd();
        }
    }
    else if (ulCurEmmStat == NAS_LMM_PUB_COMP_EMMSTATE( EMM_MS_REG, EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM))
    {
        /* ��¼UPDATE_MM��ʶ */
        /*NAS_LMM_SetEmmInfoUpdateMmFlag(NAS_EMM_UPDATE_MM_FLAG_VALID);*/

        NAS_EMM_SER_RcvRrcStmsiPagingInd();
    }
    else
    {
       NAS_LMM_EMM_PLMN_LOG_WARN(    "NAS_EMM_PLMN_STmsiPagingProc:Not process the paging with S-TMSI:NAS State Error");
    }

    return;
}



VOS_VOID NAS_EMM_PLMN_CsPagingProc
(
    LRRC_LNAS_PAGING_UE_ID_ENUM_UINT32 enPagingUeId
)
{
    VOS_UINT32                          ulCurEmmStat;
    NAS_LMM_EMM_PLMN_LOG_INFO(           "NAS_EMM_PLMN_CsPagingProc is entered!");

    ulCurEmmStat                        = NAS_LMM_PUB_COMP_EMMSTATE(
                                        NAS_EMM_CUR_MAIN_STAT,
                                        NAS_EMM_CUR_SUB_STAT);
    if(ulCurEmmStat == NAS_LMM_PUB_COMP_EMMSTATE( EMM_MS_REG, EMM_SS_REG_NORMAL_SERVICE))
    {
        NAS_EMM_SER_RcvRrcCsPagingInd(enPagingUeId);
    }
    else if(ulCurEmmStat == NAS_LMM_PUB_COMP_EMMSTATE( EMM_MS_REG, EMM_SS_REG_WAIT_ACCESS_GRANT_IND))
    {
        if(NAS_EMM_SUCC != NAS_EMM_JudgeBarType(NAS_EMM_BAR_TYPE_MT))
        {
            NAS_EMM_SER_RcvRrcCsPagingInd(enPagingUeId);
        }
    }
    else
    {
        NAS_LMM_EMM_PLMN_LOG_WARN(    "NAS_EMM_PLMN_CsPagingProc:NAS State cannot process cs paging!");
    }
    return;
}

/*****************************************************************************
 Function Name   : NAS_EMM_StartRatIsValid
 Description     :
 Input           :
 Output          :
 Return          : NAS_LMM_FAIL:�Ƿ�
                   NAS_LMM_SUCC:�Ϸ�
 History         :
    1.ZHANGCAIXIA      2011-9-9  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_StartRatIsValid(const MMC_LMM_START_REQ_STRU *pstAppStartReq)
{
    RRMM_RAT_PRIO_ENUM_UINT8    usGsmRatPrio   = pstAppStartReq->aucRatPrioList[RRMM_RAT_TYPE_GSM];
    RRMM_RAT_PRIO_ENUM_UINT8    usWcdmaRatPrio = pstAppStartReq->aucRatPrioList[RRMM_RAT_TYPE_WCDMA];
    RRMM_RAT_PRIO_ENUM_UINT8    usLteRatPrio   = pstAppStartReq->aucRatPrioList[RRMM_RAT_TYPE_LTE];
    RRMM_RAT_PRIO_ENUM_UINT8    usCdma1xRatPrio = pstAppStartReq->aucRatPrioList[RRMM_RAT_TYPE_CDMA1X];
    RRMM_RAT_PRIO_ENUM_UINT8    usHrpdRatPrio   = pstAppStartReq->aucRatPrioList[RRMM_RAT_TYPE_HRPD];

    if (  (RRMM_RAT_PRIO_BUTT <= usGsmRatPrio)
        ||(RRMM_RAT_PRIO_BUTT <= usWcdmaRatPrio)
        ||(RRMM_RAT_PRIO_BUTT <= usLteRatPrio)
        ||(RRMM_RAT_PRIO_BUTT <= usCdma1xRatPrio)
        ||(RRMM_RAT_PRIO_BUTT <= usHrpdRatPrio) )
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_StartRatIsExist: MMC START REQ RAT LIST IS VALID ");
        return NAS_LMM_FAIL;
    }

    return NAS_LMM_SUCC;
}
/*****************************************************************************
 Function Name   : NAS_EMM_RatListIsValid
 Description     :
 Input           :
 Output          :
 Return          : NAS_LMM_FAIL:�Ƿ�
                   NAS_LMM_SUCC:�Ϸ�
 History         :
    1.ZHANGCAIXIA      2011-9-9  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_RatListIsValid(RRMM_RAT_PRIO_ENUM_UINT8    usGsmRatPrio,
                                            RRMM_RAT_PRIO_ENUM_UINT8    usWcdmaRatPrio,
                                            RRMM_RAT_PRIO_ENUM_UINT8    usLteRatPrio)
{
    if (  (RRMM_RAT_PRIO_BUTT <= usGsmRatPrio)
        &&(RRMM_RAT_PRIO_BUTT <= usWcdmaRatPrio)
        &&(RRMM_RAT_PRIO_BUTT <= usLteRatPrio) )
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_StartRatIsExist: MMC START REQ RAT LIST IS VALID ");
        return NAS_LMM_FAIL;
    }

    return NAS_LMM_SUCC;
}

/*****************************************************************************
 Function Name   : NAS_EMM_lteRatIsOnlyExist
 Description     :
 Input           :
 Output          :
 Return          : NAS_LMM_FAIL:���뼼����ֻ��L
                   NAS_LMM_SUCC:���뼼��ֻ��L
 History         :
    1.ZHANGCAIXIA      2011-9-9  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_lteRatIsOnlyExist(VOS_VOID)
{
    if (  ( (NAS_LMM_RAT_PRIO_NULL < NAS_LMM_GetCurLtePrio())&&(NAS_LMM_RAT_PRIO_BUTT >NAS_LMM_GetCurLtePrio()))
    &&( (NAS_LMM_RAT_PRIO_NULL == NAS_LMM_GetCurGsmPrio())||(NAS_LMM_RAT_PRIO_BUTT == NAS_LMM_GetCurGsmPrio()))
    &&( (NAS_LMM_RAT_PRIO_NULL == NAS_LMM_GetCurWcdmaPrio())||(NAS_LMM_RAT_PRIO_BUTT ==NAS_LMM_GetCurWcdmaPrio() )))
    {
        return NAS_LMM_SUCC;
    }

    NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_lteRatIsExist: MMC START REQ RAT LIST LTE IS NOT ONLY EXIST ");
    return NAS_LMM_FAIL;
}


/*****************************************************************************
  3.2  ����������̴��� Function
*****************************************************************************/


/*****************************************************************************
 Function Name   : NAS_EMM_MmcStartCheck
 Description     : MMC_LMM_START_REQ_STRU��μ�飬�����ɹ�������������ת��Ϊ
                   EMM�Լ��Ŀ�������
 Input           : pstAppStartReq
 Output          : pstEmmStartInfo
 Return          : NAS_LMM_FAIL: ��鲻ͨ��
                   NAS_LMM_SUCC: ���ͨ��

 History         :
    1.HanLufeng 41410      2011-3-29  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_MmcStartCheck(
                const MMC_LMM_START_REQ_STRU          *pstAppStartReq,
                      NAS_EMM_START_INFO_STRU         *pstEmmStartInfo)
{
    VOS_UINT32 ulCnt                    = NAS_COMM_NULL;


    /* EMM�Լ�����Ľ��뼼������ʼֵ */
    for(ulCnt = 0; ulCnt < NAS_LMM_RAT_TYPE_BUTT; ulCnt++)
    {
        pstEmmStartInfo->aucRatPrioList[ulCnt] = NAS_LMM_RAT_PRIO_BUTT;
    }

    /* ������뼼���б�Ϊ�գ��򷵻ش��� */
    if(NAS_LMM_FAIL == NAS_EMM_StartRatIsValid(pstAppStartReq))
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MmcStartCheck: MMC START REQ RAT LIST IS NULL ");
        return  NAS_LMM_FAIL;
    }

    /* ������뼼�� */
    for(ulCnt = 0; ulCnt < NAS_LMM_RAT_TYPE_BUTT; ulCnt++)
    {
        pstEmmStartInfo->aucRatPrioList[ulCnt] = pstAppStartReq->aucRatPrioList[ulCnt];
    }

    /*--------ת��ΪLMM�Լ��Ĵ������� NAS_EMM_RAT_TYPE_ENUM_UINT8 -------*/
    if(MMC_LMM_TRANSCEIVER_TYPE_SINGLE == pstAppStartReq->ulTransceiverType)
    {
        pstEmmStartInfo->ulTransceiverType = NAS_EMM_TRANSCEIVER_TYPE_SINGLE;
    }
    else if(MMC_LMM_TRANSCEIVER_TYPE_DUAL == pstAppStartReq->ulTransceiverType)
    {
        pstEmmStartInfo->ulTransceiverType = NAS_EMM_TRANSCEIVER_TYPE_DUAL;
    }
    else
    {
        NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_MmcStartCheck: ulTransceiverType = ",
                                 pstAppStartReq->ulTransceiverType);
        return  NAS_LMM_FAIL;
    }



    /*--------��鿨״̬���� -------*/
    if( (MMC_LMM_USIM_VALID   != pstAppStartReq->ulUsimState) &&
        (MMC_LMM_USIM_INVALID != pstAppStartReq->ulUsimState))
    {
        NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_MmcStartCheck: ulUsimState = ",
                                 pstAppStartReq->ulUsimState);
        return  NAS_LMM_FAIL;
    }

    return  NAS_LMM_SUCC;


}
VOS_UINT32    NAS_EMM_MsNullSsWaitAppStartReqMsgAppStartReq(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    NAS_EMM_FSM_STATE_STRU              stEmmState;
    MMC_LMM_START_REQ_STRU             *pstAppStartReq;
    VOS_UINT32                          ulSendResult;
#if (VOS_OS_VER != VOS_WIN32)
    FTM_TMODE_ENUM                      enMspFtmMode = EN_FTM_TMODE_SIGNAL ;
#endif
    NAS_EMM_START_INFO_STRU             stEmmStartInfo;
    VOS_UINT32                          ulRst;

    (VOS_VOID)ulMsgId;

    pstAppStartReq                      = pMsg;

    /* ��ӡ����ú�����INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsNullSsWaitAppStartReqMsgAppStartReq is entered.");

    /* ��μ�飬���ɹ���ת���ӿڿ����������͵�EMM�Ĳ������� */
    ulRst = NAS_EMM_MmcStartCheck(pstAppStartReq, &stEmmStartInfo);
    if(NAS_LMM_FAIL ==  ulRst)
    {   /* ������֪ͨMMC����ʧ��  */

        /* ��APP����APP_MM_START_CNF��Ϣ*/
        ulRst = NAS_EMM_PLMN_SendAppStartCnf(MMC_LMM_FAIL,
                                    pstAppStartReq->ulOpId);
        if (NAS_EMM_PLMN_OK != ulRst)
        {
            /* ��ӡ������ERROR_LEVEL */
            NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_MsNullSsWaitAppStartReqMsgAppStartReq : Send Msg Failure!!! ulSendOuterMsgResult=",
                                     ulRst);
        }

        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitAppStartReqMsgAppStartReq: MSG IE ERROR!!!");
        return  NAS_LMM_MSG_HANDLED;
    }

    /*EMM&MMC ȫ�ֱ������ */
    NAS_LMM_EmmAndMmcGlobInfoClear();

    /* �������ϱ���Ϣ��ȫ�ֱ������ */
    NAS_LMM_MEM_SET(&g_stEmmOmInfo, 0, sizeof(OM_EMM_STATE_INFO_STRU));

    /* EMM Info ����ʼֵ����USIM/NV�޹صĸ�ֵΪ��Чֵ��
    ��USIM/NV�йصģ���ֵΪ��Чֵ���ȶ�USIM/NVʱ�����¸�ֵΪ��Чֵ */
    NAS_LMM_EmmInfoInit();

    /* ���濪��MMC�·���UTRANģʽ */
    NAS_LMM_SetEmmInfoUtranMode(pstAppStartReq->enUtranMode);

    /* ����ģ����Ʊ�������ʼֵ */
    NAS_LMM_EmmSubModGlobInit();

    /* ����App��Ϣ����ز��� */
    /*����յ���OpIdΪ0xffff,��StartReqΪ�յ�ErrInd������ڲ���Ϣ */
    /* �������������������֮ǰ */
    if (NAS_EMM_INTRA_APP_START_OPID != pstAppStartReq->ulOpId)
    {
        NAS_EMM_SaveAppMsgPara(pstAppStartReq->ulMsgId,pstAppStartReq->ulOpId);

        /* ���濪����Ϣ�еĸ���UEģʽ��Ϣ���Դ���RRC���Ϳ�����Ϣʱʹ */
        NAS_EMM_SAVE_START_INFO(stEmmStartInfo);
    }
    /* V7R1������LNAS���ڹ���̬��ҲҪ֪ͨESM��ERABM���𣬲�����Ӧ */
    /* ���� ESM ������Ϣ*/
    NAS_EMM_SendEsmSuspendInd();

    /* ���� ERABM ������Ϣ*/
    NAS_EMM_SendErabmSuspendInd();

#if (VOS_OS_VER != VOS_WIN32)
    /*�������޿�ģʽ����رհ�ȫ*/
    if((LPS_OM_GetTmode(&enMspFtmMode)== ERR_MSP_SUCCESS)&&( EN_FTM_TMODE_SIGNAL_NOCARD == enMspFtmMode))
    {
        NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsNullSsWaitAppStartReqMsgAppStartReq : EN_FTM_TMODE_SIGNAL_NOCARD, Set USIM feature as soft!");
        g_ulSmcControl = NAS_EMM_SMC_CLOSE;
    }
#endif

    /*��IMSI�ɹ�������Ϊ�п�������USIM�ļ���MMC��ȡ�ģ�LMM��MMC�������LMM���� */
    if((MMC_LMM_USIM_VALID == pstAppStartReq->ulUsimState)
       &&(NAS_LMM_USIM_READ_SUCC == NAS_LMM_ReadMmcUsimFile()))
    {
        /* ���ñ�����EMM�еĿ�״̬ȫ�ֱ��� */
        NAS_EMM_SetUsimState(NAS_LMM_SIM_STATUS_AVAILABLE);
        /* Ȼ���ٶ�ȡLTE�Լ����ļ� */
        NAS_LMM_ReadLteUsimFile();

        /* ����TI_NAS_EMM_WAIT_MMC_START_CNF_TIMER */
        NAS_LMM_StartStateTimer(TI_NAS_LMM_TIMER_WAIT_USIM_CNF);

        /* ��״̬ת����MS_NULL + SS_WAIT_MMC_START_CNF״̬ */
        stEmmState.enFsmId                    = NAS_LMM_PARALLEL_FSM_EMM;
        stEmmState.enMainState                = EMM_MS_NULL;
        stEmmState.enSubState                 = EMM_SS_NULL_WAIT_READING_USIM;
        stEmmState.enStaTId                   = TI_NAS_LMM_TIMER_WAIT_USIM_CNF;
        NAS_LMM_StaTransProc(stEmmState);

        return  NAS_LMM_MSG_HANDLED;
    }

    /*�������ã���MMC/RRC���Ϳ�����Ϣ�������޿�����*/
    NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitAppStartReqMsgAppStartReq : Hard Usim UNAVAILABLE:");

    /* ���ñ�����EMM�еĿ�״̬ȫ�ֱ��� */
    NAS_EMM_SetUsimState(NAS_LMM_SIM_STATUS_UNAVAILABLE);


    /* ��MMC����MMC_EMM_START_REQ(��������)��Ϣ */
    ulSendResult = NAS_EMM_PLMN_SendMmcStartReq(EMMC_EMM_START_REQ_CAUSE_NORMAL_START);
    if (NAS_EMM_PLMN_ERR == ulSendResult)
    {
        /* ��ӡ������ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendMmcStartReq : Send IntraMsg Failure!!!");
        return NAS_LMM_MSG_HANDLED;
    }

    /* ����TI_NAS_EMM_WAIT_MMC_START_CNF_TIMER */
    NAS_LMM_StartStateTimer(TI_NAS_EMM_WAIT_MMC_START_CNF_TIMER);

    /* ��״̬ת����MS_NULL + SS_WAIT_MMC_START_CNF״̬ */
    stEmmState.enFsmId                    = NAS_LMM_PARALLEL_FSM_EMM;
    stEmmState.enMainState                = EMM_MS_NULL;
    stEmmState.enSubState                 = EMM_SS_NULL_WAIT_MMC_START_CNF;
    stEmmState.enStaTId                   = TI_NAS_EMM_WAIT_MMC_START_CNF_TIMER;
    NAS_LMM_StaTransProc(stEmmState);

    return NAS_LMM_MSG_HANDLED;
}

#if (VOS_OS_VER == VOS_WIN32)
/*****************************************************************************
 Function Name   : NAS_EMM_SendMmcErrInd
 Description     :
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.HanLufeng 41410      2011-3-4  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_EMM_SendMmcErrInd( VOS_UINT32 ulErrTpye )
{

    LMM_MMC_ERR_IND_STRU      *pLmmMmcErrIndMsg    = NAS_EMM_NULL_PTR;

    NAS_LMM_PUBM_LOG_INFO( "NAS_EMM_SendMmcErrInd: enter. RESET.");

    /*����MMC�ڲ���Ϣ */
    pLmmMmcErrIndMsg = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_ERR_IND_STRU));

    if(NAS_EMM_NULL_PTR == pLmmMmcErrIndMsg)
    {
        NAS_LMM_PUBM_LOG_ERR( "NAS_EMM_SendMmcErrInd: MSG ALLOC ERR !!");
        return;
    }

    NAS_LMM_MEM_SET(pLmmMmcErrIndMsg,0,sizeof(LMM_MMC_ERR_IND_STRU));

    /* ��дLMM_MMC_ERR_IND_STRU ��DOPRA��Ϣͷ */
    EMM_PUBU_COMP_MMC_MSG_HEADER(pLmmMmcErrIndMsg,
            sizeof(LMM_MMC_ERR_IND_STRU) - EMM_LEN_VOS_MSG_HEADER);

    /* ��дEMM_RABM_RRC_CON_REL_IND ����ϢID��ʶ */
    pLmmMmcErrIndMsg->ulMsgId           = ID_LMM_MMC_ERR_IND;

    /* ������Ϣ */
    NAS_LMM_SendLmmMmcMsg(                    pLmmMmcErrIndMsg);

    return;

}
#endif


VOS_UINT32    NAS_EMM_MsNotNullSsAnyStateMsgAppStartReq(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    VOS_UINT32                          ulSendResult;
    NAS_EMM_FSM_STATE_STRU              EmmState;
    MMC_LMM_START_REQ_STRU             *pstAppStartReq;

    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* ��ӡ����ú�����INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsNotNullSsAnyStateMsgAppStartReq is entered.");

    pstAppStartReq = pMsg;

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNotNullSsAnyStateMsgAppStartReq: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if (EMM_MS_NULL == EmmState.enMainState)
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNotNullSsAnyStateMsgAppStartReq: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* ��APP����APP_MM_START_CNF��Ϣ(�ظ������ɹ����Ѿ�����) */
    ulSendResult = NAS_EMM_PLMN_SendAppStartCnf(MMC_LMM_SUCC, pstAppStartReq->ulOpId);
    if (NAS_EMM_PLMN_OK != ulSendResult)
    {
        /* ��ӡ������ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendAppStartCnf : Send Msg Failure!!! ulSendResult=",
                                 ulSendResult);
        return NAS_LMM_MSG_HANDLED;
    }

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32    NAS_EMM_WhenStopingThenMsgAppStartReq(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    VOS_UINT32                          ulSendResult;
    MMC_LMM_START_REQ_STRU             *pstAppStartReq;

    (VOS_VOID)ulMsgId;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_WhenStopingThenMsgAppStartReq is entered.");

    pstAppStartReq                      = pMsg;

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_WhenStopingThenMsgAppStartReq: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ˵���Ѿ��յ��ػ���Ϣ�����ػ�����������û����ɣ���û�лظ�APP_MM_STOP_CNF
    ��Ϣ��ֱ�ӻظ�APP����ʧ�ܣ���APP_MM_START_CNF��Ϣ���ظ�����ʧ�ܣ�RRC�ڹػ���*/
    ulSendResult = NAS_EMM_PLMN_SendAppStartCnf(MMC_LMM_FAIL, pstAppStartReq->ulOpId);
    if (NAS_EMM_PLMN_OK != ulSendResult)
    {
        /* ��ӡ������ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendAppStartCnf : Send Msg Failure!!! ulSendResult=",
                                 ulSendResult);
    }

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32    NAS_EMM_MsNullSsWaitMmcStartCnfMsgMmcStartCnf(
                    VOS_UINT32                              ulMsgId,
                    const VOS_VOID                         *pMsg )
{
    VOS_UINT32                          ulSendResult;
    NAS_EMM_FSM_STATE_STRU              EmmState;

    (VOS_VOID)ulMsgId;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsNullSsWaitMmcStartCnfMsgMmcStartCnf is entered.");

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);


    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNullSsWaitMmcStartCnfMsgMmcStartCnf: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_NULL, EMM_SS_NULL_WAIT_MMC_START_CNF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitMmcStartCnfMsgMmcStartCnf: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*��鿨״̬��������Ч������ȫ�ֱ���*/
    if (NAS_LMM_SIM_STATUS_UNAVAILABLE == NAS_LMM_GetSimState())
    {
        NAS_EMM_NoUsimInit();
    }

    /* ֹͣTI_NAS_EMM_WAIT_MMC_START_CNF_TIMER */
    NAS_LMM_StopStateTimer(TI_NAS_EMM_WAIT_MMC_START_CNF_TIMER);

    /* ��RRC����RRC_MM_START_REQ��Ϣ */
    ulSendResult = NAS_EMM_PLMN_SendRrcStartReq();
    if (NAS_EMM_PLMN_OK != ulSendResult)
    {
        /* ��ӡ������ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendRrcStartReq : Send Msg Failure!!! ulSendResult=",
                                 ulSendResult);
    }

    /* ����TI_NAS_EMM_WAIT_RRC_START_CNF_TIMER */
    NAS_LMM_StartStateTimer(TI_NAS_EMM_WAIT_RRC_START_CNF_TIMER);

    /* ��״̬ת����MS_NULL + SS_NULL_WAIT_RRC_START_CNF״̬ */
    EmmState.enMainState                = EMM_MS_NULL;
    EmmState.enSubState                 = EMM_SS_NULL_WAIT_RRC_START_CNF;
    EmmState.enStaTId                   = TI_NAS_EMM_WAIT_RRC_START_CNF_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32    NAS_EMM_MsNullSsWaitMmcStartCnfMsgTimerExp(
                    VOS_UINT32                              ulMsgId,
                    const VOS_VOID                         *pMsg )
{
    VOS_UINT32                          ulSendResult;
    NAS_EMM_FSM_STATE_STRU              EmmState;

    (VOS_VOID)ulMsgId;


    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNullSsWaitMmcStartCnfMsgTimerExp is entered.");

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNullSsWaitMmcStartCnfMsgTimerExp: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_NULL, EMM_SS_NULL_WAIT_MMC_START_CNF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitMmcStartCnfMsgTimerExp: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    if (NAS_EMM_PLMN_OK == NAS_EMM_CheckAppMsgPara(ID_MMC_LMM_START_REQ))
    {
        /* ��APP����APP_MM_START_CNF��Ϣ���ظ�����ʧ�ܣ��ȴ�MMC������ʱ�� */
        ulSendResult = NAS_EMM_PLMN_SendAppStartCnf(MMC_LMM_FAIL,NAS_EMM_GetAppMsgOpId());

        if (NAS_EMM_PLMN_OK != ulSendResult)
        {
            /* ��ӡ������ERROR_LEVEL */
            NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendAppStartCnf : Send Msg Failure!!! ulSendResult=",
                                     ulSendResult);
            return NAS_LMM_MSG_HANDLED;
        }
    }

    /*���APP����*/
    NAS_EMM_ClearAppMsgPara();

    /* ��״̬ת����MS_NULL + SS_NULL_WAIT_APP_START_REQ״̬ */
    EmmState.enMainState                = EMM_MS_NULL;
    EmmState.enSubState                 = EMM_SS_NULL_WAIT_APP_START_REQ;
    EmmState.enStaTId                   = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    return NAS_LMM_MSG_HANDLED;
}



VOS_UINT32    NAS_EMM_MsNullSsWaitRrcStartCnfMsgRrcStartCnf(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    VOS_UINT32                          ulSendOuterMsgResult;
    VOS_UINT32                          ulSendInnerMsgResult;
    VOS_UINT32                          ulAppRslt;
    NAS_EMM_FSM_STATE_STRU              stEmmState;
    LRRC_LNAS_START_RESULT_ENUM_UINT32    enResult;
    NAS_LMM_SIM_STAT_ENUM_UINT32        ulSimState;
    LRRC_LMM_START_CNF_STRU               *pstRrcMmStartCnfMsg = (LRRC_LMM_START_CNF_STRU*)(pMsg);

    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(stEmmState);

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNullSsWaitRrcStartCnfMsgRrcStartCnf is entered.");

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNullSsWaitRrcStartCnfMsgRrcStartCnf: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_NULL, EMM_SS_NULL_WAIT_RRC_START_CNF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitRrcStartCnfMsgRrcStartCnf: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* ֹͣTI_NAS_EMM_WAIT_RRC_START_CNF_TIMER */
    NAS_LMM_StopStateTimer(TI_NAS_EMM_WAIT_RRC_START_CNF_TIMER);

    /* ����RRCMmStartCnfMsg�������*/
    enResult                            = pstRrcMmStartCnfMsg->enResult;
    if(LRRC_LNAS_START_SUCCESS == enResult)
    {
        ulAppRslt = MMC_LMM_SUCC;
    }
    else
    {
        ulAppRslt = MMC_LMM_FAIL;
    }

    if (NAS_EMM_PLMN_OK == NAS_EMM_CheckAppMsgPara(ID_MMC_LMM_START_REQ))
    {
        /* ��APP����APP_MM_START_CNF��Ϣ*/
        ulSendOuterMsgResult = NAS_EMM_PLMN_SendAppStartCnf(ulAppRslt,
                                    NAS_EMM_GetAppMsgOpId());
        if (NAS_EMM_PLMN_OK != ulSendOuterMsgResult)
        {
            /* ��ӡ������ERROR_LEVEL */
            NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendAppStartCnf : Send Msg Failure!!! ulSendOuterMsgResult=",
                                     ulSendOuterMsgResult);
        }
    }

    /*���APP����*/
    NAS_EMM_ClearAppMsgPara();

    /* �������ΪRRC_NAS_START_SUCCESS�Ĵ���*/
    if(LRRC_LNAS_START_SUCCESS == enResult)
    {
        /*���ݿ��Ƿ���ڣ�ת��EMM��״̬*/
        ulSimState                          = NAS_LMM_GetSimState();

        if(NAS_LMM_SIM_STATUS_AVAILABLE == ulSimState)
        {
            /* ��״̬ת����MS_DEREG + SS_PLMN_SEARCH״̬ */
            stEmmState.enMainState          = EMM_MS_DEREG;
            stEmmState.enSubState           = EMM_SS_DEREG_NO_CELL_AVAILABLE;
            stEmmState.enStaTId             = TI_NAS_EMM_STATE_NO_TIMER;
        }
        else
        {
            /* �·��ڲ�UsimStatusInd��Ϣ��֪ͨ����Ч������AnyС������ */
            /*NAS_EMMC_SendIntraUsimStatusInd(USIMM_CARD_STATUS_ABSENT);*/

            /* ��״̬ת����MS_DEREG + SS_PLMN_SEARCH״̬ */
            stEmmState.enMainState          = EMM_MS_DEREG;
            stEmmState.enSubState           = EMM_SS_DEREG_NO_IMSI;
            stEmmState.enStaTId             = TI_NAS_EMM_STATE_NO_TIMER;
        }
    }

    /* ���������ΪRRC_NAS_START_SUCCESS�Ĵ���*/
    else
    {
        /* ��MMC����MMC_EMM_STOP_REQ��Ϣ */
        ulSendInnerMsgResult = NAS_EMM_PLMN_SendMmcStopReq();
        if (NAS_EMM_PLMN_ERR == ulSendInnerMsgResult)
        {
            /* ��ӡ������ERROR_LEVEL */
            NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendMmcStopReq : Send IntraMsg Failure!!!");
        }

        /* ��״̬ת����EMM_MS_NULL + EMM_SS_NULL_WAIT_APP_START_REQ״̬ */
        stEmmState.enMainState          = EMM_MS_NULL;
        stEmmState.enSubState           = EMM_SS_NULL_WAIT_APP_START_REQ;
        stEmmState.enStaTId             = TI_NAS_EMM_STATE_NO_TIMER;
    }

    /* �ı�״̬ */
    NAS_LMM_StaTransProc(stEmmState);

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32  NAS_EMM_MsNullSsWaitRrcStartCnfMsgTimerExp(
                VOS_UINT32              ulMsgId,
                const VOS_VOID         *pMsg )
{
    VOS_UINT32                          ulRet;

    (VOS_VOID)ulMsgId;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNullSsWaitRrcStartCnfMsgTimerExp");

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNullSsWaitRrcStartCnfMsgTimerExp: MSG PARAM NULL POINTER ERROR!!!");
        return( NAS_LMM_ERR_CODE_PARA_INVALID);
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_NULL, EMM_SS_NULL_WAIT_RRC_START_CNF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitRrcStartCnfMsgTimerExp: STATE ERROR!!!");
        return(NAS_LMM_MSG_DISCARD);
    }

    ulRet = NAS_EMM_MsNullSsWaitRrcStartCnfProcMsgTimerExp(MMC_LMM_FAIL);

    return(ulRet);
}



/*****************************************************************************
  3.3  �ػ�������̴��� Function
*****************************************************************************/

VOS_UINT32    NAS_EMM_MsNullSsAnyReadUsimStateMsgAppStopReq(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    VOS_UINT32                          ulSendResult;
    MMC_LMM_STOP_REQ_STRU              *pstAppStopReq;
    NAS_EMM_FSM_STATE_STRU              EmmState;

    (VOS_VOID)ulMsgId;

    /* ��ӡ����ú�����INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsNullSsAnyReadUsimStateMsgAppStopReq is entered.");

    pstAppStopReq                       = pMsg;

    /*ֹͣ���ж�ʱ��*/
    NAS_LMM_StopAllPtlTimer();
    NAS_LMM_StopAllStateTimer();

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNullSsAnyReadUsimStateMsgAppStopReq: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ��״̬ת����MS_NULL + EMM_SS_NULL_WAIT_APP_START_REQ״̬ */
    EmmState.enFsmId                    = NAS_LMM_PARALLEL_FSM_EMM;
    EmmState.enMainState                = EMM_MS_NULL;
    EmmState.enSubState                 = EMM_SS_NULL_WAIT_APP_START_REQ;
    EmmState.enStaTId                   = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    /* ��APP����APP_MM_STOP_CNF��Ϣ���ظ��ػ��ɹ����Ѿ��ػ��� */
    ulSendResult = NAS_EMM_PLMN_SendAppStopCnf(MMC_LMM_SUCC,pstAppStopReq->ulOpId);
    if (NAS_EMM_PLMN_OK != ulSendResult)
    {
        /* ��ӡ������ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendAppStopCnf : Send Msg Failure!!! ulSendResult=",
                                 ulSendResult);
        return NAS_LMM_MSG_HANDLED;
    }

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32    NAS_EMM_MsNullSsWaitSwitchOffMsgRrcRelInd(
                    VOS_UINT32                              ulMsgId,
                    const VOS_VOID                         *pMsg )
{
    VOS_UINT32                          ulRet;

    (VOS_VOID)ulMsgId;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM(           "NAS_EMM_MsNullSsWaitSwitchOffMsgRrcRelInd is entered.");

    /* ��������ָ�������� */
    if (VOS_NULL_PTR                    == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR(        "NAS_EMM_MsNullSsWaitSwitchOffMsgRrcRelInd: MSG PARAM NULL POINTER ERROR!!!");
        return(                         NAS_LMM_ERR_CODE_PARA_INVALID);
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(  EMM_MS_NULL, EMM_SS_NULL_WAIT_SWITCH_OFF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN(       "NAS_EMM_MsNullSsWaitSwitchOffMsgRrcRelInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* ֹͣTI_NAS_EMM_PTL_SWITCH_OFF_TIMER */
    NAS_LMM_StopPtlTimer(               TI_NAS_EMM_PTL_SWITCH_OFF_TIMER);

    ulRet                               = NAS_EMM_MsNullSsWaitSwitchOffProcMsgRrcRelInd();

    return(                             ulRet);
}
VOS_VOID NAS_EMM_ProcLocalStop( VOS_VOID )
{

    VOS_UINT32                          ulSendResult;
    NAS_EMM_FSM_STATE_STRU              EmmState;
    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /*��ESM����EMM_ESM_STATUS_IND��Ϣ*/
    NAS_EMM_PLMN_SendEmmEsmStautsInd(EMM_ESM_ATTACH_STATUS_DETACHED);

    /*֪ͨ RABM:RRC_CONN_REL_IND*/
    NAS_LMM_SendRabmRrcConRelInd();

    /* ֪ͨETC:REL_IND */
    NAS_EMM_TcSendRelInd();

    /* �����DEREG̬��EMM���������ظ�д���ˣ��ڽ���DEREG̬ʱ��д��ȫ�����ģ���
       ��·�ͷ�ʱ��дPSLOC */
    if (EMM_MS_DEREG != NAS_EMM_CUR_MAIN_STAT)
    {
        /* ���current��ȫ������Ϊmapped ��non-current native�����Ĵ��ڣ�
           ����������Ϊcurrent��ͬʱɾ��current mapped���Ա�ػ�ʱд�뿨��
           ����:      24301Э��     5.5.2.2.1*/
        NAS_EMM_SecuDeregClrSecuCntxt();

        /* д��EMM NV�����Ϣ */
        NAS_EMM_WriteNvMmInfo();
    }

    /* ��MMC����MMC_EMM_STOP_REQ��Ϣ */
    ulSendResult = NAS_EMM_PLMN_SendMmcStopReq();
    if (NAS_EMM_PLMN_ERR == ulSendResult)
    {
        /* ��ӡ������ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_ProcLocalStop : Send Msg Failure!!!");
        return;
    }

    /* ����TI_NAS_EMM_WAIT_MMC_STOP_CNF_TIMER */
    NAS_LMM_StartStateTimer(TI_NAS_EMM_WAIT_MMC_STOP_CNF_TIMER);

    /* ��״̬ת����MS_NULL + SS_NULL_WAIT_MMC_STOP_CNF״̬ */
    EmmState.enMainState                = EMM_MS_NULL;
    EmmState.enSubState                 = EMM_SS_NULL_WAIT_MMC_STOP_CNF;
    EmmState.enStaTId                   = TI_NAS_EMM_WAIT_MMC_STOP_CNF_TIMER;
    NAS_LMM_StaTransProc(EmmState);
    return;
}
VOS_UINT32    NAS_EMM_MsNotNullNotRegMsgAppStopReq(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    MMC_LMM_STOP_REQ_STRU              *pstAttStopReq;

    (VOS_VOID)ulMsgId;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsNotNullNotRegMsgAppStopReq is entered.");

    pstAttStopReq                       = pMsg;

    /*ֹͣ���ж�ʱ��*/
    NAS_LMM_StopAllPtlTimer();
    NAS_LMM_StopAllStateTimer();

    /* ����App��Ϣ����ز��� */
    NAS_EMM_SaveAppMsgPara(pstAttStopReq->ulMsgId,pstAttStopReq->ulOpId);

    NAS_EMM_ProcLocalStop();

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32    NAS_EMM_MsNullSsWaitMmcStopCnfMsgMmcStopCnf(
                    VOS_UINT32                              ulMsgId,
                    const VOS_VOID                         *pMsg )
{
    VOS_UINT32                          ulSendResult;
    NAS_EMM_FSM_STATE_STRU              EmmState;

    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNullSsWaitMmcStopCnfMsgMmcStopCnf is entered.");

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNullSsWaitMmcStopCnfMsgMmcStopCnf: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_NULL, EMM_SS_NULL_WAIT_MMC_STOP_CNF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitMmcStopCnfMsgMmcStopCnf: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* ���������Դ */
    /*NAS_EMM_ClearLocalResource();�ػ������ڴ治���������ٴ��յ� START��Ϣ�������ڴ棬������ */

    /* ֹͣTI_NAS_EMM_WAIT_MMC_STOP_CNF_TIMER */
    NAS_LMM_StopStateTimer(TI_NAS_EMM_WAIT_MMC_STOP_CNF_TIMER);

    /* ��RRC����RRC_MM_STOP_REQ��Ϣ */
    ulSendResult = NAS_EMM_PLMN_SendRrcStopReq();
    if (NAS_EMM_PLMN_OK != ulSendResult)
    {
        /* ��ӡ������ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendRrcStopReq : Send Msg Failure!!! ulSendResult=",
                                 ulSendResult);
    }

    /* ����TI_NAS_EMM_WAIT_RRC_STOP_CNF_TIMER */
    NAS_LMM_StartStateTimer(TI_NAS_EMM_WAIT_RRC_STOP_CNF_TIMER);

    /* ��״̬ת����MS_NULL + SS_NULL_WAIT_RRC_STOP_CNF״̬ */
    EmmState.enMainState                = EMM_MS_NULL;
    EmmState.enSubState                 = EMM_SS_NULL_WAIT_RRC_STOP_CNF;
    EmmState.enStaTId                   = TI_NAS_EMM_WAIT_RRC_STOP_CNF_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32    NAS_EMM_MsNullSsWaitMmcStopCnfMsgTimerExp(
                    VOS_UINT32                              ulMsgId,
                    const VOS_VOID                         *pMsg )
{
    VOS_UINT32                          ulSendResult;
    NAS_EMM_FSM_STATE_STRU              EmmState;

    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNullSsWaitMmcStopCnfMsgTimerExp is entered.");

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNullSsWaitMmcStopCnfMsgTimerExp: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_NULL, EMM_SS_NULL_WAIT_MMC_STOP_CNF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitMmcStopCnfMsgTimerExp: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* ��RRC����RRC_MM_STOP_REQ��Ϣ */
    ulSendResult = NAS_EMM_PLMN_SendRrcStopReq();
    if (NAS_EMM_PLMN_OK != ulSendResult)
    {
        /* ��ӡ������ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_MsNullSsWaitMmcStopCnfMsgTimerExp : Send Msg Failure!!! ulSendResult=",
                                 ulSendResult);
        return NAS_LMM_MSG_HANDLED;
    }

    /* ����TI_NAS_EMM_WAIT_RRC_STOP_CNF_TIMER */
    NAS_LMM_StartStateTimer(TI_NAS_EMM_WAIT_RRC_STOP_CNF_TIMER);

    /* ��״̬ת����MS_NULL + SS_NULL_WAIT_RRC_STOP_CNF״̬ */
    EmmState.enMainState                = EMM_MS_NULL;
    EmmState.enSubState                 = EMM_SS_NULL_WAIT_RRC_STOP_CNF;
    EmmState.enStaTId                   = TI_NAS_EMM_WAIT_RRC_STOP_CNF_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32    NAS_EMM_MsNullSsWaitRrcStopCnfMsgRrcStopCnf(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    VOS_UINT32                          ulSendResult;

    (VOS_VOID)(ulMsgId);
    (VOS_VOID)(pMsg);

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNullSsWaitRrcStopCnfMsgRrcStopCnf is entered.");

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNullSsWaitRrcStopCnfMsgRrcStopCnf: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_NULL, EMM_SS_NULL_WAIT_RRC_STOP_CNF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitRrcStopCnfMsgRrcStopCnf: STATE ERROR!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* ֹͣTI_NAS_EMM_WAIT_RRC_STOP_CNF_TIMER */
    NAS_LMM_StopStateTimer(TI_NAS_EMM_WAIT_RRC_STOP_CNF_TIMER);

    /* ״̬Ǩ�Ƶ� Null.WtAppStartReq*/
    NAS_EMM_ProcRrcStopRslt();

    /* ��APP����APP_MM_STOP_CNF��Ϣ���ظ��ػ��ɹ��������ػ��� */
    if (NAS_EMM_PLMN_OK == NAS_EMM_CheckAppMsgPara(ID_MMC_LMM_STOP_REQ))
    {

        ulSendResult = NAS_EMM_PLMN_SendAppStopCnf(APP_SUCCESS,NAS_EMM_GetAppMsgOpId());

        if (NAS_EMM_PLMN_OK != ulSendResult)
        {
            /* ��ӡ������ERROR_LEVEL */
            NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendAppStopCnf : Send Msg Failure!!! ulSendResult=",
                                     ulSendResult);
        }
    }
    /* ����ػ���ʶ,�ñ�־�ṩ��LRRC,����LRRC�жϿտ��Ƿ��ǹػ�detach �ñ�־
       ��LMM�յ�MMC��stop reqʱ��Ϊ1 */
    NAS_LMM_SetEmmInfoLtePowerOffFlag(NAS_EMM_NO);
    /*���APP����*/
    NAS_EMM_ClearAppMsgPara();

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32    NAS_EMM_MsNullSsWaitRrcStopCnfMsgTimerExp(
                    VOS_UINT32                              ulMsgId,
                    const VOS_VOID                         *pMsg )
{
    VOS_UINT32                          ulResult;

    (VOS_VOID)ulMsgId;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNullSsWaitRrcStopCnfMsgTimerExp is entered.");

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNullSsWaitRrcStopCnfMsgTimerExp: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_NULL, EMM_SS_NULL_WAIT_RRC_STOP_CNF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitRrcStopCnfMsgTimerExp: STATE ERROR!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* ״̬Ǩ�Ƶ� Null.WtAppStartReq*/
    NAS_EMM_ProcRrcStopRslt();

    if (NAS_EMM_PLMN_OK == NAS_EMM_CheckAppMsgPara(ID_MMC_LMM_STOP_REQ))
    {
        /* ��APP����APP_MM_STOP_CNF��Ϣ���ظ��ػ�ʧ�ܣ��ȴ�RRCֹͣ��ʱ�� */
        ulResult = NAS_EMM_PLMN_SendAppStopCnf(MMC_LMM_FAIL,NAS_EMM_GetAppMsgOpId());

        if (NAS_EMM_PLMN_OK != ulResult)
        {
            /* ��ӡ������ERROR_LEVEL */
            NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendAppStopCnf : Send Msg Failure!!! ulSendResult=",
                                     ulResult);
        }
    }
    /* ����ػ���ʶ,�ñ�־�ṩ��LRRC,����LRRC�жϿտ��Ƿ��ǹػ�detach �ñ�־
       ��LMM�յ�MMC��stop reqʱ��Ϊ1 */
    NAS_LMM_SetEmmInfoLtePowerOffFlag(NAS_EMM_NO);
    /*���APP����*/
    NAS_EMM_ClearAppMsgPara();

    return NAS_LMM_MSG_HANDLED;
}

/*****************************************************************************
  3.4  ����������̴��� Function
*****************************************************************************/

VOS_UINT32    NAS_EMM_MsNotNullSsAnyStateMsgMmcPlmnInd(
                    VOS_UINT32                              ulMsgId,
                    const VOS_VOID                         *pMsg)
{
    /*VOS_UINT32                          ulSendResult;*/
    NAS_EMM_FSM_STATE_STRU              EmmState;
    VOS_UINT32                          ulRslt;

    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* ��ӡ����ú�����INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNotNullSsAnyStateMsgMmcPlmnInd is entered.");

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNotNullSsAnyStateMsgMmcPlmnInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if (EMM_MS_NULL == EmmState.enMainState)
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNotNullSsAnyStateMsgMmcPlmnInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*���BAR��ʶ*/
    NAS_EMM_ClearBarResouce();

    /* �ж���Ϊ����̬����֪ͨESM��ERABM�������ȡGU�İ�ȫ���� */
    ulRslt = NAS_EMM_NoCellSuspendMsgMmcPlmnReq();
    if (NAS_LMM_MSG_HANDLED == ulRslt)
    {
        NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNotNullSsAnyStateMsgMmcPlmnInd: MMC ACTIVE LNAS");
        return NAS_LMM_MSG_HANDLED;
    }

    /*�������֧������̬���������˴��������ж�USIM���Ƿ���Ч:�������Ч�Ҳ��������ӣ�
      ״̬ת����DEREG+NO_IMSI�������Ӧ����Դ֪ͨMMC����Ч���������Ч�Ҵ������ӣ�
      ״̬ת����DEREG+WAIT_MRRC_REL+_CNF������RRC_MM_REL_REQ��Ϣ�����յ�RRC_MM_REL_CNF
      ��Ϣ��״̬ת����DEREG+NO_IMSI,�����Ӧ����Դ֪ͨMMC����Ч*/

    /* �ж�EMM��ǰ״̬*/
    /* ���GCF9.2.3.1.5 ������ʱ���� ,UE����,MMC�������� ,�����ڼ�T3412��ʱ,�����TAU������combined TAU with IMSI���ͣ�
       ������������,��������������,UE������������פ��,�����޷�ת��no cell̬���������GCF���Դ˴�EMM��ת״̬ */
    if((PS_SUCC == LPS_OM_IsTestMode())
        &&(EMM_MS_REG == EmmState.enMainState)
        && (EMM_SS_REG_LIMITED_SERVICE == EmmState.enSubState))
    {
        return NAS_LMM_MSG_HANDLED;
    }

    /* ���GCF9.2.3.1.5 ������UE������MMC���������������ڼ�T3412��ʱ�������TAU������combined TAU with IMSI���͵�*/
    if ((EMM_MS_REG == EmmState.enMainState)
        && (EMM_SS_REG_NO_CELL_AVAILABLE == EmmState.enSubState))
    {
        return NAS_LMM_MSG_HANDLED;

    }
    else if(EMM_MS_REG == EmmState.enMainState)
    {
        /* ��״̬ת����MS_REG + SS_REG_PLMN_SEARCH״̬ */
        EmmState.enMainState        = EMM_MS_REG;
        EmmState.enSubState         = EMM_SS_REG_PLMN_SEARCH;
        EmmState.enStaTId           = TI_NAS_EMM_STATE_NO_TIMER;

    }
    else
    {
        if (EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
        {
            /* ֪ͨESM������EMM��Ҫ������ע�ᣬESM����Ҫ�����̬���еĽ���PDN������¼ */
            NAS_EMM_PLMN_SendEmmEsmStautsInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
        }
        else
        {
            /*��ESM����ATTACH���*/
            NAS_EMM_PLMN_SendEmmEsmStautsInd(EMM_ESM_ATTACH_STATUS_DETACHED);
        }
        /* ��״̬ת����MS_DEREG + SS_DEREG_PLMN_SEARCH״̬ */
        EmmState.enMainState        = EMM_MS_DEREG;
        EmmState.enSubState         = EMM_SS_DEREG_PLMN_SEARCH;
        EmmState.enStaTId           = TI_NAS_EMM_STATE_NO_TIMER;
    }

    NAS_LMM_StaTransProc(EmmState);

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32    NAS_EMM_MsRegSsRegAttemptUpdateMmMsgMmcPlmnInd
(
    VOS_UINT32                          ulMsgId,
    const VOS_VOID                     *pMsg
)
{
    /*VOS_UINT32                          ulSendResult;*/
    NAS_EMM_FSM_STATE_STRU              EmmState;

    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* ��ӡ����ú�����INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgMmcPlmnInd is entered.");

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgMmcPlmnInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_REG, EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgMmcPlmnInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*���BAR��ʶ*/
    NAS_EMM_ClearBarResouce();

    /* ��¼UPDATE_MM��ʶ */
    /*NAS_LMM_SetEmmInfoUpdateMmFlag(NAS_EMM_UPDATE_MM_FLAG_VALID);*/

    /* ��״̬ת����MS_REG + SS_REG_PLMN_SEARCH״̬ */
    EmmState.enMainState        = EMM_MS_REG;
    EmmState.enSubState         = EMM_SS_REG_PLMN_SEARCH;
    EmmState.enStaTId           = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    return NAS_LMM_MSG_HANDLED;
}

/*****************************************************************************
 Function Name   : NAS_EMM_MsRegSsRegAttemptUpdateMmMsgIntraMrrcRelInd
 Description     : ������REG+ATTEMPT_TO_UPDATE_MM״̬���յ��ڲ���·�ͷ���Ϣ
 Input           : ulMsgId-------------------------��ϢID
                   pMsgStru------------------------��Ϣָ��
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2011-11-17  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_MsRegSsRegAttemptUpdateMmMsgIntraMrrcRelInd
(
    VOS_UINT32                          ulMsgId,
    const VOS_VOID                     *pMsg
)
{
    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgIntraMrrcRelInd is entered.");
    (VOS_VOID)ulMsgId;

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgIntraMrrcRelInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /*�����ڲ���Ϣ INTRA_CONN2IDLE_REQ,��������״̬ΪIDLE*/
    NAS_EMM_CommProcConn2Ilde();

    return NAS_LMM_MSG_HANDLED;
}
/*****************************************************************************
 Function Name   : NAS_EMM_MsRegSsNormalServiceMsgMrrcRelInd
 Description     : ��reg.normal̬���ڲ��ͷŴ���
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.sunbing49683      2010-12-29  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_MsRegSsNormalServiceMsgMrrcRelInd(
                    VOS_UINT32                              ulMsgId,
                    const VOS_VOID                         *pMsg )
{
    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsRegSsNormalServiceMsgMrrcRelInd is entered.");
    (VOS_VOID)ulMsgId;

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsRegSsNormalServiceMsgMrrcRelInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /*�����ڲ���Ϣ INTRA_CONN2IDLE_REQ,��������״̬ΪIDLE*/
    NAS_EMM_CommProcConn2Ilde();

    return NAS_LMM_MSG_HANDLED;
}



VOS_UINT32    NAS_EMM_MsNotNullSsAnyStateMsgMmcCoverageLostInd(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    NAS_EMM_FSM_STATE_STRU                      EmmState;
    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    (VOS_VOID)(ulMsgId);
    (VOS_VOID)(pMsg);

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNotNullSsAnyStateMsgMmcCoverageLostInd is entered.");

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNotNullSsAnyStateMsgMmcCoverageLostInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if (EMM_MS_NULL == EmmState.enMainState)
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNotNullSsAnyStateMsgMmcCoverageLostInd: STATE ERROR!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*���BAR��ʶ*/
    NAS_EMM_ClearBarResouce();

    /* �رյ�ǰEMM��״̬��ʱ����Э�鶨ʱ�� */
    NAS_LMM_StopAllEmmStateTimer();

    if ((EMM_MS_REG         == EmmState.enMainState) ||
        (EMM_MS_TAU_INIT    == EmmState.enMainState) ||
        (EMM_MS_SER_INIT    == EmmState.enMainState)) /* �Ѿ�ע�� */
    {
        /* ��״̬ת����MS_REG + SS_NO_CELL_AVAILABLE״̬ */
        EmmState.enMainState            = EMM_MS_REG;
        EmmState.enSubState             = EMM_SS_REG_NO_CELL_AVAILABLE;
        EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
    }
    else
    {
        /*��ESM����ID_EMM_ESM_STATUS_IND��Ϣ*/
        NAS_EMM_PLMN_SendEmmEsmStautsInd(      EMM_ESM_ATTACH_STATUS_DETACHED);

        /* ��״̬ת����MS_DEREG + SS_NO_CELL_AVAILABLE״̬ */
        EmmState.enMainState            = EMM_MS_DEREG;
        EmmState.enSubState             = EMM_SS_DEREG_NO_CELL_AVAILABLE;
        EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
    }
    NAS_LMM_StaTransProc(EmmState);

    return NAS_LMM_MSG_HANDLED;
}

/*****************************************************************************
 Function Name   : NAS_EMM_MsRegSsRegAttemptUpdateMmMsgMmcCoverageLostInd
 Description     : ��REG+ATTEMPT_TO_UPDATE_MM״̬���յ��G����Ϣ
 Input           : ulMsgId----------------------��ϢID
                   pMsg-------------------------�G����Ϣָ��
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2011-11-16  Draft Enact
*****************************************************************************/
VOS_UINT32 NAS_EMM_MsRegSsRegAttemptUpdateMmMsgMmcCoverageLostInd
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsg
)
{
    NAS_EMM_FSM_STATE_STRU                      EmmState;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    (VOS_VOID)(ulMsgId);
    (VOS_VOID)(pMsg);

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgMmcCoverageLostInd is entered.");

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgMmcCoverageLostInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_REG, EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgMmcCoverageLostInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*���BAR��ʶ*/
    NAS_EMM_ClearBarResouce();

    /* �رյ�ǰEMM��״̬��ʱ����Э�鶨ʱ�� */
    NAS_LMM_StopAllEmmStateTimer();

    /* ��¼UPDATE_MM��ʶ */
    /*NAS_LMM_SetEmmInfoUpdateMmFlag(NAS_EMM_UPDATE_MM_FLAG_VALID);*/

    /* ��״̬ת����MS_REG + SS_NO_CELL_AVAILABLE״̬ */
    EmmState.enMainState            = EMM_MS_REG;
    EmmState.enSubState             = EMM_SS_REG_NO_CELL_AVAILABLE;
    EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32    NAS_EMM_MsDeregSsAttemptToAttachMsgMmcSysInfoInd(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    NAS_LMM_FSM_STATE_STRU               stEmmState;
    EMMC_EMM_SYS_INFO_IND_STRU         *pstMmcSysInfoInd;
    VOS_UINT32                          ulSendResult;
    NAS_MM_TA_STRU                     *pstLastAttmpRegTa = NAS_EMM_NULL_PTR;
    NAS_MM_TA_STRU                      stCurTa           = {0};
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsDeregSsAttemptToAttachMsgMmcSysInfoInd is entered.");

    /*check the input params*/
    if (VOS_NULL_PTR == pMsg)
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsAttemptToAttachMsgMmcSysInfoInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /*get the sys info*/
    pstMmcSysInfoInd = (EMMC_EMM_SYS_INFO_IND_STRU *) pMsg;

    /* ���ATTACH�Ĵ��������ǽ���PDN�����������ͨע�ᣬ�һ���Ľ���PDN��������
       ��Ϣ��Ч���������ע�� */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsAttemptToAttachMsgMmcSysInfoInd: case ESM_ATTACH_FOR_INIT_EMC_BERER");

        /*����ESM��Ϣ*/
        NAS_LMM_MEM_CPY(        (VOS_VOID*)NAS_EMM_GLO_AD_GetEsmMsgAddr(),
                               &(pstEsmMsg->stEsmMsg),
                                (pstEsmMsg->stEsmMsg.ulEsmMsgSize)+4);

        NAS_EMM_StartEmergencyAttach();

        return NAS_LMM_MSG_HANDLED;
    }

    /*�жϵ�ǰС���Ƿ񱻽�ֹ*/
    if ((EMMC_EMM_NO_FORBIDDEN != pstMmcSysInfoInd->ulForbiddenInfo)
         || (EMMC_EMM_CELL_STATUS_ANYCELL == pstMmcSysInfoInd->ulCellStatus))
    {
        /*��ESM����EMM_ESM_STATUS_IND��Ϣ*/
        NAS_EMM_PLMN_SendEmmEsmStautsInd(EMM_ESM_ATTACH_STATUS_DETACHED);


        stEmmState.enFsmId                = NAS_LMM_PARALLEL_FSM_EMM;
        stEmmState.enMainState            = EMM_MS_DEREG;
        stEmmState.enSubState             = EMM_SS_DEREG_LIMITED_SERVICE;
        stEmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
        NAS_LMM_StaTransProc(stEmmState);

        NAS_EMM_SetAttType();

        /*��MMC����LMM_MMC_ATTACH_CNF��LMM_MMC_ATTACH_IND��Ϣ*/
        NAS_EMM_AppSendAttRsltForb(pstMmcSysInfoInd->ulForbiddenInfo);
        return NAS_LMM_MSG_HANDLED;
    }


    /* ��ȡ��ǰTA���ϴγ���ע���TA��Ϣ */
    NAS_EMM_GetCurrentTa(&stCurTa);
    pstLastAttmpRegTa                  = NAS_LMM_GetEmmInfoNetInfoLastAttmpRegTaAddr();

    /* �ж��Ƿ���Ҫ�ȶ�ʱ����ʱ�ٷ���ע�� */
    if (NAS_LMM_MATCH_SUCCESS == NAS_LMM_TaMatch(pstLastAttmpRegTa, &stCurTa))
    {
        NAS_EMM_ATTACH_LOG_NORM("NAS_EMM_MsDeregSsAttemptToAttachMsgMmcSysInfoInd: The same as last attempt TA.");
        if (   (NAS_LMM_TIMER_RUNNING == NAS_LMM_IsPtlTimerRunning(TI_NAS_EMM_PTL_T3411))
            || (NAS_LMM_TIMER_RUNNING == NAS_LMM_IsPtlTimerRunning(TI_NAS_EMM_PTL_T3402)) )
        {
            return NAS_LMM_MSG_HANDLED;
        }
    }
    else
    {
        /*����attach attempt counter*/
        NAS_EMM_AttResetAttAttempCounter();
    }

    stEmmState.enFsmId                = NAS_LMM_PARALLEL_FSM_EMM;
    stEmmState.enMainState            = EMM_MS_DEREG;
    stEmmState.enSubState             = EMM_SS_DEREG_NORMAL_SERVICE;
    stEmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(stEmmState);

    /*send Msg EMM_INTRA_ATTACH_REQ to ATTACH module*/
    ulSendResult = NAS_EMM_SendIntraAttachReq();

    if (NAS_EMM_PLMN_ERR == ulSendResult)
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsAttemptToAttachMsgMmcSysInfoInd : Send IntraMsg Failure!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32  NAS_EMM_MsDeregSsNormalSrvMsgMmcSysInfoInd(
                                    VOS_UINT32              ulMsgId,
                                    VOS_VOID               *pMsg )
{
    EMMC_EMM_SYS_INFO_IND_STRU          *pstMmcSysInfoInd;
    NAS_EMM_FSM_STATE_STRU              EmmState;
    VOS_UINT32                          ulSendResult;
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsDeregSsNormalSrvMsgMmcSysInfoInd is entered.");

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsNormalSrvMsgMmcSysInfoInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if(NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_DEREG, EMM_SS_DEREG_NORMAL_SERVICE))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsNormalSrvMsgMmcSysInfoInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    pstMmcSysInfoInd = (EMMC_EMM_SYS_INFO_IND_STRU *) pMsg;
	/* lihong00150010 emergency tau&service begin */
    /* ���ATTACH�Ĵ��������ǽ���PDN�����������ͨע�ᣬ�һ���Ľ���PDN��������
       ��Ϣ��Ч���������ע�� */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsNormalSrvMsgMmcSysInfoInd: case ESM_ATTACH_FOR_INIT_EMC_BERER");

        /*����ESM��Ϣ*/
        NAS_LMM_MEM_CPY(        (VOS_VOID*)NAS_EMM_GLO_AD_GetEsmMsgAddr(),
                               &(pstEsmMsg->stEsmMsg),
                                (pstEsmMsg->stEsmMsg.ulEsmMsgSize)+4);

        NAS_EMM_StartEmergencyAttach();

        return NAS_LMM_MSG_HANDLED;
    }
	/* lihong00150010 emergency tau&service end */
    if ((EMMC_EMM_NO_FORBIDDEN != pstMmcSysInfoInd->ulForbiddenInfo) /* TA�ڽ�ֹ��TA�б��� */
        || (EMMC_EMM_CELL_STATUS_ANYCELL == pstMmcSysInfoInd->ulCellStatus))
    {
        /* ��״̬ת����MS_DEREG + SS_DEREG_LIMITED_SERVICE״̬ */
        EmmState.enMainState            = EMM_MS_DEREG;
        EmmState.enSubState             = EMM_SS_DEREG_LIMITED_SERVICE;
        EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
        NAS_LMM_StaTransProc(EmmState);

        NAS_EMM_SetAttType();

        /*��MMC����LMM_MMC_ATTACH_CNF��LMM_MMC_ATTACH_IND��Ϣ*/
        NAS_EMM_AppSendAttRsltForb(pstMmcSysInfoInd->ulForbiddenInfo);
    }
    /*PC REPLAY MODIFY BY LEILI BEGIN*/
    if(NAS_EMM_AUTO_ATTACH_NOT_ALLOW == NAS_EMM_GetPsAttachAllowFlg())
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsNormalSrvMsgMmcSysInfoInd : attach not allowed!!!");
        return NAS_LMM_MSG_HANDLED;
    }
    /*PC REPLAY MODIFY BY LEILI END*/
    /* ��ATTACHģ�鷢��EMM_INTRA_ATTACH_REQ��Ϣ */
    ulSendResult = NAS_EMM_SendIntraAttachReq();
    if (NAS_EMM_PLMN_ERR == ulSendResult)
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsNormalSrvMsgMmcSysInfoInd : Send IntraMsg Failure!!!");
        return NAS_LMM_MSG_HANDLED;
    }

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32 NAS_EMM_MsDeregSsLimitSrvMsgMmcSysInfoInd(
                                    VOS_UINT32              ulMsgId,
                                    VOS_VOID               *pMsg )
{
    VOS_UINT32                          ulSendResult;
    EMMC_EMM_SYS_INFO_IND_STRU         *pstMmcSysInfoInd = NAS_EMM_NULL_PTR;
    NAS_EMM_FSM_STATE_STRU              EmmState;
    NAS_MM_TA_STRU                     *pstLastAttmpRegTa = NAS_EMM_NULL_PTR;
    NAS_MM_TA_STRU                      stCurTa           = {0};
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsDeregSsLimitSrvMsgMmcSysInfoInd is entered.");

    /* ״̬ƥ����,����ƥ��,�˳� */
    if(NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_DEREG, EMM_SS_DEREG_LIMITED_SERVICE))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsLimitSrvMsgMmcSysInfoInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    pstMmcSysInfoInd = (EMMC_EMM_SYS_INFO_IND_STRU *) pMsg;

    /* ���ATTACH�Ĵ��������ǽ���PDN�����������ͨע�ᣬ�һ���Ľ���PDN��������
       ��Ϣ��Ч���������ע�� */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsAttemptToAttachMsgMmcSysInfoInd: case ESM_ATTACH_FOR_INIT_EMC_BERER");

        /*����ESM��Ϣ*/
        NAS_LMM_MEM_CPY(        (VOS_VOID*)NAS_EMM_GLO_AD_GetEsmMsgAddr(),
                               &(pstEsmMsg->stEsmMsg),
                                (pstEsmMsg->stEsmMsg.ulEsmMsgSize)+4);

        NAS_EMM_StartEmergencyAttach();

        return NAS_LMM_MSG_HANDLED;
    }

    if ((EMMC_EMM_NO_FORBIDDEN != pstMmcSysInfoInd->ulForbiddenInfo) /* TA�ڽ�ֹ��TA�б��� */
        || (EMMC_EMM_CELL_STATUS_ANYCELL == pstMmcSysInfoInd->ulCellStatus))
    {
        /* ��״̬ת����MS_DEREG + SS_DEREG_LIMITED_SERVICE״̬ */
        EmmState.enMainState            = EMM_MS_DEREG;
        EmmState.enSubState             = EMM_SS_DEREG_LIMITED_SERVICE;
        EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
        NAS_LMM_StaTransProc(EmmState);

        NAS_EMM_SetAttType();

        /*��MMC����LMM_MMC_ATTACH_CNF��LMM_MMC_ATTACH_IND��Ϣ*/
        NAS_EMM_AppSendAttRsltForb(pstMmcSysInfoInd->ulForbiddenInfo);

        return NAS_LMM_MSG_HANDLED;
    }

    /* ��ȡ��ǰTA���ϴγ���ע���TA��Ϣ */
    NAS_EMM_GetCurrentTa(&stCurTa);
    pstLastAttmpRegTa                  = NAS_LMM_GetEmmInfoNetInfoLastAttmpRegTaAddr();

    /* �ж��Ƿ���Ҫ�ȶ�ʱ����ʱ�ٷ���ע�� */
    if (NAS_LMM_MATCH_SUCCESS == NAS_LMM_TaMatch(pstLastAttmpRegTa, &stCurTa))
    {
        NAS_EMM_ATTACH_LOG_NORM("NAS_EMM_MsDeregSsLimitSrvMsgMmcSysInfoInd: The same as last attempt TA.");

        if(NAS_EMM_YES == NAS_EMM_Attach_IsTimerRunning())
        {
            return NAS_LMM_MSG_HANDLED;
        }
    }
    else
    {
        /*����attach attempt counter*/
        NAS_EMM_AttResetAttAttempCounter();
    }

    /* ��״̬ת����MS_DEREG + SS_DEREG_NORMAL_SERVICE״̬ */
    EmmState.enMainState            = EMM_MS_DEREG;
    EmmState.enSubState             = EMM_SS_DEREG_NORMAL_SERVICE;
    EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    /* ��ATTACHģ�鷢��EMM_INTRA_ATTACH_REQ��Ϣ */
    ulSendResult = NAS_EMM_SendIntraAttachReq();
    if (NAS_EMM_PLMN_ERR == ulSendResult)
    {
        /* ��ӡ������ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsLimitSrvMsgMmcSysInfoInd : Send IntraMsg Failure!!!");
        return NAS_LMM_MSG_HANDLED;
    }

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32 NAS_EMM_MsDeregSsPlmnSearchMsgMmcSysInfoInd(
                                            VOS_UINT32              ulMsgId,
                                            VOS_VOID               *pMsg )
{
    VOS_UINT32                          ulSendResult;
    EMMC_EMM_SYS_INFO_IND_STRU         *pstMmcSysInfoInd = NAS_EMM_NULL_PTR;
    NAS_EMM_FSM_STATE_STRU              EmmState;
    NAS_MM_TA_STRU                     *pstLastAttmpRegTa = NAS_EMM_NULL_PTR;
    NAS_MM_TA_STRU                      stCurTa           = {0};
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsDeregSsPlmnSearchMsgMmcSysInfoInd is entered.");

    /* ״̬ƥ����,����ƥ��,�˳� */
    if(NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_DEREG, EMM_SS_DEREG_PLMN_SEARCH))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsPlmnSearchMsgMmcSysInfoInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    pstMmcSysInfoInd = (EMMC_EMM_SYS_INFO_IND_STRU *) pMsg;

    /* ���ATTACH�Ĵ��������ǽ���PDN�����������ͨע�ᣬ�һ���Ľ���PDN��������
       ��Ϣ��Ч���������ע�� */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsPlmnSearchMsgMmcSysInfoInd: case ESM_ATTACH_FOR_INIT_EMC_BERER");

        /*����ESM��Ϣ*/
        NAS_LMM_MEM_CPY(        (VOS_VOID*)NAS_EMM_GLO_AD_GetEsmMsgAddr(),
                               &(pstEsmMsg->stEsmMsg),
                                (pstEsmMsg->stEsmMsg.ulEsmMsgSize)+4);

        NAS_EMM_StartEmergencyAttach();

        return NAS_LMM_MSG_HANDLED;
    }

    if ((EMMC_EMM_NO_FORBIDDEN != pstMmcSysInfoInd->ulForbiddenInfo) /* TA�ڽ�ֹ��TA�б��� */
        || (EMMC_EMM_CELL_STATUS_ANYCELL == pstMmcSysInfoInd->ulCellStatus))
    {
        NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsDeregSsPlmnSearchMsgMmcSysInfoInd: FORBID SYS INFO");

        /* ��״̬ת����MS_DEREG + SS_DEREG_LIMITED_SERVICE״̬ */
        EmmState.enMainState            = EMM_MS_DEREG;
        EmmState.enSubState             = EMM_SS_DEREG_LIMITED_SERVICE;
        EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
        NAS_LMM_StaTransProc(EmmState);

        NAS_EMM_SetAttType();

        /*��MMC����LMM_MMC_ATTACH_CNF��LMM_MMC_ATTACH_IND��Ϣ*/
        NAS_EMM_AppSendAttRsltForb(pstMmcSysInfoInd->ulForbiddenInfo);

        return NAS_LMM_MSG_HANDLED;
    }

    /* ��ȡ��ǰTA���ϴγ���ע���TA��Ϣ */
    NAS_EMM_GetCurrentTa(&stCurTa);
    pstLastAttmpRegTa                  = NAS_LMM_GetEmmInfoNetInfoLastAttmpRegTaAddr();

    /* �ж��Ƿ���Ҫ�ȶ�ʱ����ʱ�ٷ���ע�� */
    if (NAS_LMM_MATCH_SUCCESS == NAS_LMM_TaMatch(pstLastAttmpRegTa, &stCurTa))
    {
        NAS_EMM_ATTACH_LOG_NORM("NAS_EMM_MsDeregSsPlmnSearchMsgMmcSysInfoInd: The same as last attempt TA.");

        if(NAS_EMM_YES == NAS_EMM_Attach_IsTimerRunning())
        {
            return NAS_LMM_MSG_HANDLED;
        }
    }
    else
    {
        /*����attach attempt counter*/
        NAS_EMM_AttResetAttAttempCounter();
    }

    /* ��״̬ת����MS_DEREG + SS_DEREG_NORMAL_SERVICE״̬ */
    EmmState.enMainState            = EMM_MS_DEREG;
    EmmState.enSubState             = EMM_SS_DEREG_NORMAL_SERVICE;
    EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    /* ��ATTACHģ�鷢��EMM_INTRA_ATTACH_REQ��Ϣ */
    ulSendResult = NAS_EMM_SendIntraAttachReq();
    if (NAS_EMM_PLMN_ERR == ulSendResult)
    {
        /* ��ӡ������ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsPlmnSearchMsgMmcSysInfoInd : Send IntraMsg Failure!!!");
        return NAS_LMM_MSG_HANDLED;
    }

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32 NAS_EMM_MsDeregSsNoCellAvailMsgMmcSysInfoInd
(
    VOS_UINT32              ulMsgId,
    VOS_VOID               *pMsg
)
{
    VOS_UINT32                          ulSendResult;
    EMMC_EMM_SYS_INFO_IND_STRU         *pstMmcSysInfoInd = NAS_EMM_NULL_PTR;
    NAS_EMM_FSM_STATE_STRU              EmmState;
    NAS_MM_TA_STRU                     *pstLastAttmpRegTa = NAS_EMM_NULL_PTR;
    NAS_MM_TA_STRU                      stCurTa           = {0};
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsDeregSsNoCellAvailMsgMmcSysInfoInd is entered.");

    /* ״̬ƥ����,����ƥ��,�˳� */
    if(NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_DEREG, EMM_SS_DEREG_NO_CELL_AVAILABLE))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsNoCellAvailMsgMmcSysInfoInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* �����ǰ���ڹ���̬���򲻴���SYS_INFO,ֱ�Ӷ���*/
    if(NAS_LMM_CUR_LTE_SUSPEND == NAS_EMM_GetCurLteState())
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsNoCellAvailMsgMmcSysInfoInd:LTE SUSPEND.");
        return NAS_LMM_MSG_HANDLED;
    }

    pstMmcSysInfoInd = (EMMC_EMM_SYS_INFO_IND_STRU *) pMsg;

    /* ���ATTACH�Ĵ��������ǽ���PDN�����������ͨע�ᣬ�һ���Ľ���PDN��������
       ��Ϣ��Ч���������ע�� */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsAttemptToAttachMsgMmcSysInfoInd: case ESM_ATTACH_FOR_INIT_EMC_BERER");

        /*����ESM��Ϣ*/
        NAS_LMM_MEM_CPY(        (VOS_VOID*)NAS_EMM_GLO_AD_GetEsmMsgAddr(),
                               &(pstEsmMsg->stEsmMsg),
                                (pstEsmMsg->stEsmMsg.ulEsmMsgSize)+4);

        NAS_EMM_StartEmergencyAttach();

        return NAS_LMM_MSG_HANDLED;
    }

    if ((EMMC_EMM_NO_FORBIDDEN != pstMmcSysInfoInd->ulForbiddenInfo) /* TA�ڽ�ֹ��TA�б��� */
        || (EMMC_EMM_CELL_STATUS_ANYCELL == pstMmcSysInfoInd->ulCellStatus))
    {

        /* ��״̬ת����MS_DEREG + SS_DEREG_LIMITED_SERVICE״̬ */
        EmmState.enMainState            = EMM_MS_DEREG;
        EmmState.enSubState             = EMM_SS_DEREG_LIMITED_SERVICE;
        EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
        NAS_LMM_StaTransProc(EmmState);

        NAS_EMM_SetAttType();

        /*��MMC����LMM_MMC_ATTACH_CNF��LMM_MMC_ATTACH_IND��Ϣ*/
        NAS_EMM_AppSendAttRsltForb(pstMmcSysInfoInd->ulForbiddenInfo);
    }
    else
    {
        /* ��ȡ��ǰTA���ϴγ���ע���TA��Ϣ */
        NAS_EMM_GetCurrentTa(&stCurTa);
        pstLastAttmpRegTa                  = NAS_LMM_GetEmmInfoNetInfoLastAttmpRegTaAddr();

        /* �ж��Ƿ���Ҫ�ȶ�ʱ����ʱ�ٷ���ע�� */
        if (NAS_LMM_MATCH_SUCCESS == NAS_LMM_TaMatch(pstLastAttmpRegTa, &stCurTa))
        {
            NAS_EMM_ATTACH_LOG_NORM("NAS_EMM_MsDeregSsNoCellAvailMsgMmcSysInfoInd: The same as last attempt TA.");

            if(NAS_EMM_YES == NAS_EMM_Attach_IsTimerRunning())
            {
                return NAS_LMM_MSG_HANDLED;
            }
        }
        else
        {
            /*����attach attempt counter*/
            NAS_EMM_AttResetAttAttempCounter();
        }

        /* ��״̬ת����MS_DEREG + SS_DEREG_NORMAL_SERVICE״̬ */
        EmmState.enMainState            = EMM_MS_DEREG;
        EmmState.enSubState             = EMM_SS_DEREG_NORMAL_SERVICE;
        EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
        NAS_LMM_StaTransProc(EmmState);

        /* ��ATTACHģ�鷢��EMM_INTRA_ATTACH_REQ��Ϣ */
        ulSendResult = NAS_EMM_SendIntraAttachReq();
        if (NAS_EMM_PLMN_ERR == ulSendResult)
        {
            NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsNoCellAvailMsgMmcSysInfoInd : Send IntraMsg Failure!!!");
            return NAS_LMM_MSG_HANDLED;
        }
    }

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32    NAS_EMM_MsNotNullSsAnyStateMsgRrcErrInd(
                    VOS_UINT32                              ulMsgId,
                    const VOS_VOID                         *pMsg )
{
    NAS_EMM_FSM_STATE_STRU                      EmmState;

    (VOS_VOID)(ulMsgId);

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNotNullSsAnyStateMsgRrcErrInd is entered.");

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNotNullSsAnyStateMsgRrcErrInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if (EMM_MS_NULL == EmmState.enMainState)
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNotNullSsAnyStateMsgRrcErrInd: STATE ERROR!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*ֹͣ���ж�ʱ��*/
    NAS_LMM_StopAllPtlTimer();
    NAS_LMM_StopAllStateTimer();

    /* �ͷſ��ܴ��ڵĶ�̬�ڴ� */
    NAS_LMM_FreeDynMem();

    /* д��EMM NV�����Ϣ */
    NAS_EMM_WriteNvMmInfo();

    /* д��MMC NV�����Ϣ */
    /*NAS_EMMC_WriteNvMmInfo();*/

    /*״̬����գ���״̬����ֵ*/
    NAS_LMM_PUBM_Init();

    /*��ESM����ID_EMM_ESM_STATUS_IND��Ϣ*/
    NAS_EMM_EsmSendStatResult(          EMM_ESM_ATTACH_STATUS_DETACHED);

    /*֪ͨ RABM:RRC_CONN_REL_IND*/
    NAS_LMM_SendRabmRrcConRelInd();


    /* �����ڲ���AppStartReq */
    /*  V7R1�����Զ����������ǵȴ�MMC�Ŀ���  */
    /*    NAS_LMM_SendStubIntraAppStartReq();*/

    /* ֪ͨMMC */
    NAS_EMM_SendMmcErrInd(0xff);


    /* ״̬ת�� */
    /* ��״̬ת����MS_NULL + EMM_SS_NULL_WAIT_APP_START_REQ״̬ */
    EmmState.enFsmId                    = NAS_LMM_PARALLEL_FSM_EMM;
    EmmState.enMainState                = EMM_MS_NULL;
    EmmState.enSubState                 = EMM_SS_NULL_WAIT_APP_START_REQ;
    EmmState.enStaTId                   = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32    NAS_EMM_MsNotNullSsAnyStateMsgRrcPagingInd(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    LRRC_LMM_PAGING_IND_STRU    * pMsgRrcPagingInd = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsNotNullSsAnyStateMsgRrcPagingInd is entered.");

    /*check the current state*/
    if (EMM_MS_NULL == NAS_EMM_CUR_MAIN_STAT)
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNotNullSsAnyStateMsgRrcPagingInd: STATE ERROR!!");
        return NAS_LMM_MSG_DISCARD;
    }

    if (NAS_EMM_CONN_IDLE != NAS_EMM_GetConnState())
    {
        /*��������״̬*/
        NAS_EMM_MrrcChangeRrcStatusToIdle();
    }

    /*get RRC_PAGING_IND Msg*/
    pMsgRrcPagingInd = (LRRC_LMM_PAGING_IND_STRU *)pMsg;

    #if (FEATURE_PTM == FEATURE_ON)
    NAS_LMM_ErrlogInfoProc(pMsg, LMM_ERR_LOG_IMSI_PAGING_PROCEDURE);
    #endif

    /*check the CN domain:
       LRRC_LNAS_PAGING_TYPE1->CS;
       LRRC_LNAS_PAGING_TYPE2->PS*/
    if(LRRC_LNAS_PAGING_TYPE1 == pMsgRrcPagingInd->enPagingType)
    {
        NAS_EMM_PLMN_CsPagingProc(pMsgRrcPagingInd->enPagingUeId);

        return NAS_LMM_MSG_HANDLED;
    }

    /*Paging with IMSI*/
    if(LRRC_LNAS_IMSI_LTE == pMsgRrcPagingInd->enPagingUeId)
    {
        NAS_EMM_PLMN_ImsiPagingProc();
    }
    /*Paging with S-TMSI*/
    else if(LRRC_LNAS_S_TMSI_LTE == pMsgRrcPagingInd->enPagingUeId)
    {
        NAS_EMM_PLMN_STmsiPagingProc();
    }
    /*Paging with Other UeId*/
    else
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNotNullSsAnyStateMsgRrcPagingInd:Other UeId not support");
    }

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32    NAS_EMM_MsDeregSsNoImsiMsgMmcPlmnInd(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg)
{
    /*VOS_UINT32                          ulSendResult;*/
    NAS_EMM_FSM_STATE_STRU              EmmState;

    (VOS_VOID)(ulMsgId);
    (VOS_VOID)(pMsg);

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* ��ӡ����ú�����INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsDeregSsNoImsiMsgMmcPlmnInd is entered.");

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsNoImsiMsgMmcPlmnInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if (EMM_MS_NULL == EmmState.enMainState)
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsNoImsiMsgMmcPlmnInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }
    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsDeregSsNoImsiMsgMmcPlmnInd: Set NAS_EMM_PLMN_SRCH_NO_IMSI_TRUE!!!");

    /* �ж���Ϊ����̬����֪ͨESM��ERABM���������ȡGU�İ�ȫ���� */
    NAS_EMM_NoImsiSuspendMsgMmcPlmnReq();

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32  NAS_EMM_MsNullSsAnyStateMsgRrcErrInd(
    VOS_UINT32                                              ulMsgId,
    VOS_VOID                                                *pMsg)

{
    (VOS_VOID)(ulMsgId);
    (VOS_VOID)(pMsg);

    NAS_LMM_EMM_PLMN_LOG_NORM(           "NAS_EMM_MsNullSsAnyStateMsgRrcErrInd: enter.");


    return NAS_LMM_MSG_DISCARD;

}



VOS_VOID  NAS_EMM_ProcRrcStopRslt( VOS_VOID )
{
    NAS_EMM_FSM_STATE_STRU              EmmState;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /*�ػ���ʼ�� PUBM*/
    NAS_LMM_PUBM_Init();

    /* ��״̬ת����MS_NULL + SS_WAIT_APP_START_REQ״̬ */
    EmmState.enFsmId                = NAS_LMM_PARALLEL_FSM_EMM;
    EmmState.enMainState            = EMM_MS_NULL;
    EmmState.enSubState             = EMM_SS_NULL_WAIT_APP_START_REQ;
    EmmState.enFsmId                = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    return;
}



VOS_UINT32  NAS_EMM_MsNullSsWaitRrcStartCnfProcMsgTimerExp(VOS_UINT32 ulErrCause)
{
    VOS_UINT32                          ulSendOuterMsgResult;
    VOS_UINT32                          ulSendInnerMsgResult;
    NAS_EMM_FSM_STATE_STRU              EmmState;

    NAS_EMM_PLMN_INIT_EMMSTATE(         EmmState);

    /*ֹͣ����EMM״̬��ʱ��*/
    NAS_LMM_StopAllEmmStateTimer();

    /*ֹͣ����Э�鶨ʱ��*/
    NAS_LMM_StopAllPtlTimer();

    /* ��MMC����MMC_EMM_STOP_REQ��Ϣ */
    ulSendInnerMsgResult = NAS_EMM_PLMN_SendMmcStopReq();
    if (NAS_EMM_PLMN_ERR == ulSendInnerMsgResult)
    {
        /* ��ӡ������ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendMmcStopReq : Send IntraMsg Failure!!!");
    }

    if (NAS_EMM_PLMN_OK == NAS_EMM_CheckAppMsgPara(ID_MMC_LMM_START_REQ))
    {
        /* ��APP����APP_MM_START_CNF��Ϣ���ظ�����ʧ�ܣ��ȴ�RRC������ʱ�� */
        ulSendOuterMsgResult= NAS_EMM_PLMN_SendAppStartCnf(
                                        ulErrCause, NAS_EMM_GetAppMsgOpId());

        if (NAS_EMM_PLMN_OK != ulSendOuterMsgResult)
        {
            /* ��ӡ������ERROR_LEVEL */
            NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendAppStartCnf : Send Msg Failure!!! ulSendOuterMsgResult=",
                                        ulSendOuterMsgResult);
        }
    }

    /*���APP����*/
    NAS_EMM_ClearAppMsgPara();


    /* ��״̬ת����MS_NULL + SS_NULL_WAIT_APP_START_REQ״̬ */
    EmmState.enMainState                = EMM_MS_NULL;
    EmmState.enSubState                 = EMM_SS_NULL_WAIT_APP_START_REQ;
    EmmState.enStaTId                   = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    return(NAS_LMM_MSG_HANDLED);
}



VOS_UINT32  NAS_EMM_MsNullSsWaitSwitchOffProcMsgRrcRelInd(VOS_VOID)
{
    VOS_UINT32                          ulSendResult;
    NAS_EMM_FSM_STATE_STRU              EmmState;

    NAS_EMM_PLMN_INIT_EMMSTATE(         EmmState);

    /* ֪ͨESM�����Դ */
    NAS_EMM_PLMN_SendEmmEsmStautsInd(   EMM_ESM_ATTACH_STATUS_DETACHED);

    /*֪ͨ RABM:RRC_CONN_REL_IND*/
    NAS_LMM_SendRabmRrcConRelInd();

    /* ֪ͨETC:REL_IND */
    NAS_EMM_TcSendRelInd();

    /*ֹͣ��ʱ��*/
    NAS_LMM_StopStateTimer(              TI_NAS_EMM_PTL_SWITCH_OFF_TIMER);

    /* ��MMC����MMC_EMM_STOP_REQ��Ϣ */
    ulSendResult                        = NAS_EMM_PLMN_SendMmcStopReq();
    if (NAS_EMM_PLMN_ERR                == ulSendResult)
    {
        /* ��ӡ������ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR(        "NAS_EMM_MsNullSsWaitSwitchOffMsgRrcRelInd : Send Msg Failure!!!");
        return(                         NAS_LMM_MSG_HANDLED);
    }

    /* ���current��ȫ������Ϊmapped ��non-current native�����Ĵ��ڣ�
       ����������Ϊcurrent��ͬʱɾ��current mapped���Ա�ػ�ʱд�뿨��
       ����:      24301Э��     5.5.2.2.1*/
    NAS_EMM_SecuDeregClrSecuCntxt();

    /* д��EMM NV�����Ϣ */
    NAS_EMM_WriteNvMmInfo();

    /* ����TI_NAS_EMM_WAIT_MMC_STOP_CNF_TIMER */
    NAS_LMM_StartStateTimer(             TI_NAS_EMM_WAIT_MMC_STOP_CNF_TIMER);

    /* ��״̬ת����MS_NULL + SS_NULL_WAIT_MMC_STOP_CNF״̬ */
    EmmState.enMainState                = EMM_MS_NULL;
    EmmState.enSubState                 = EMM_SS_NULL_WAIT_MMC_STOP_CNF;
    EmmState.enStaTId                   = TI_NAS_EMM_WAIT_MMC_STOP_CNF_TIMER;
    NAS_LMM_StaTransProc(                EmmState);

    return(                             NAS_LMM_MSG_HANDLED);
}
VOS_UINT32  NAS_EMM_MsNullSsWaitSwitchOffMsgRrcErrInd(
    VOS_UINT32                                              ulMsgId,
    VOS_VOID                                                *pMsg )
{
    VOS_UINT32                          ulRet;

    (VOS_VOID)(                         ulMsgId);
    (VOS_VOID)(                         pMsg);

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM(           "NAS_EMM_MsNullSsWaitSwitchOffMsgRrcErrInd is entered.");

    /* ��������ָ�������� */
    if (VOS_NULL_PTR                    == pMsg)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR(        "NAS_EMM_MsNullSsWaitSwitchOffMsgRrcErrInd: MSG PARAM NULL POINTER ERROR!!!");
        return(                         NAS_LMM_ERR_CODE_PARA_INVALID);
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(  EMM_MS_NULL, EMM_SS_NULL_WAIT_SWITCH_OFF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN(       "NAS_EMM_MsNullSsWaitSwitchOffMsgRrcErrInd: STATE ERROR!!!");
        return(                         NAS_LMM_MSG_DISCARD);
    }

    /* ֹͣTI_NAS_EMM_PTL_SWITCH_OFF_TIMER */
    NAS_LMM_StopPtlTimer(                TI_NAS_EMM_PTL_SWITCH_OFF_TIMER);

    ulRet                               = NAS_EMM_MsNullSsWaitSwitchOffProcMsgRrcRelInd();

    return(                             ulRet);
}
VOS_UINT32 NAS_EMM_MsDrgSsNoImsiMsgSysInfoInd(
                                    VOS_UINT32              ulMsgId,
                                    VOS_VOID               *pMsg )
{
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;
    (VOS_VOID)pMsg;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsDrgSsNoImsiMsgSysInfoInd is entered.");

    /* ��������ָ�������� */
    if (VOS_NULL_PTR == pMsg)
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDrgSsNoImsiMsgSysInfoInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* ״̬ƥ����,����ƥ��,�˳� */
    if(NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_DEREG, EMM_SS_DEREG_NO_IMSI))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDrgSsNoImsiMsgSysInfoInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* ���ATTACH�Ĵ��������ǽ���PDN�����������ͨע�ᣬ�һ���Ľ���PDN��������
       ��Ϣ��Ч���������ע�� */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDrgSsNoImsiMsgSysInfoInd: case ESM_ATTACH_FOR_INIT_EMC_BERER");

        /*����ESM��Ϣ*/
        NAS_LMM_MEM_CPY(        (VOS_VOID*)NAS_EMM_GLO_AD_GetEsmMsgAddr(),
                               &(pstEsmMsg->stEsmMsg),
                                (pstEsmMsg->stEsmMsg.ulEsmMsgSize)+4);

        NAS_EMM_StartEmergencyAttach();

        return NAS_LMM_MSG_HANDLED;
    }

    NAS_EMM_AppSendAttRstDefaultReqType(MMC_LMM_ATT_RSLT_FAILURE);

    return NAS_LMM_MSG_HANDLED;
}
/*lint +e961*/
/*lint +e960*/
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

