

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "NasEmmTauSerInclude.h"
#include  "NasLmmPubMPrint.h"
#include "NasMmlCtx.h"
/* lihong00150010 emergency tau&service begin */
#include "NasEmmAttDetInclude.h"
/* lihong00150010 emergency tau&service end */


/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASEMMTAUPROC_C
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
EMM_TAU_CTRL_STRU                g_stEmmTAUCtrlTbl;




/*lint -e589*/
/*lint -e669*/
/*lint -e831*/
/*lint -e960*/
/*lint -e961*/
VOS_VOID NAS_EMM_TAU_Init(VOS_VOID)
{
    NAS_EMM_TAU_LOG_NORM("NAS_EMM_TAU_Init                   START INIT...");

    /* ��ʼ��TAUȫ�ֱ��� */
    NAS_LMM_MEM_SET(&g_stEmmTAUCtrlTbl, 0, sizeof(g_stEmmTAUCtrlTbl));

    g_stEmmTAUCtrlTbl.enEmmTauType = NAS_EMM_CN_TAU_TYPE_TA_UPDATING;

    /* ��ʼ��BAR��Ϣ*/
    NAS_EMM_ClearBarResouce();

    return;
}
VOS_VOID  NAS_EMM_TAU_ClearResouce( VOS_VOID )
{
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_ClearResouce entered.");

    /* ��ʼ��TAUȫ�ֱ��� */
    NAS_LMM_MEM_SET(&g_stEmmTAUCtrlTbl, 0, sizeof(g_stEmmTAUCtrlTbl));

    g_stEmmTAUCtrlTbl.enEmmTauType = NAS_EMM_CN_TAU_TYPE_TA_UPDATING;

    /* �ͷŻ����ESM��Ϣ*/
    NAS_EMM_SerClearEsmDataBuf();

    /* ��ս���Bar��Ϣ*/
    NAS_EMM_ClearBarResouce();

    /*ֹͣ��ض�ʱ��*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_TAU_T3430);
    NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_T3402);
    NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_T3411);
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);

    return;
}
/*****************************************************************************
 Function Name   : NAS_EMM_TAU_SuspendInitClearResouce
 Description     : UE��������TAU����������Դ
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.FTY       2012-02-24  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_EMM_TAU_SuspendInitClearResouce( VOS_VOID )
{
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_ClearResouce entered.");

    /* ��ʼ��TAUȫ�ֱ��� */
    NAS_LMM_MEM_SET(&g_stEmmTAUCtrlTbl, 0, sizeof(g_stEmmTAUCtrlTbl));

    g_stEmmTAUCtrlTbl.enEmmTauType = MMC_LMM_TA_UPDATING;

    /* �ͷŻ����ESM��Ϣ*/
    NAS_EMM_SerClearEsmDataBuf();

    /* ��ս���Bar��Ϣ*/
    NAS_EMM_ClearBarResouce();

    /*ֹͣ��ض�ʱ��*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_TAU_T3430);
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);

    return;
}

VOS_VOID  NAS_EMM_FreeTauDyn( VOS_VOID )
{
    /*��Ҫʱ��Ӳ���*/

    return;
}
VOS_VOID    NAS_EMM_TAUSER_FSMTranState(
                                    NAS_EMM_MAIN_STATE_ENUM_UINT16  stMS,
                                    NAS_EMM_SUB_STATE_ENUM_UINT16   stSS,
                                    NAS_EMM_STATE_TI_ENUM_UINT16    stTI

)
{

    NAS_LMM_FSM_STATE_STRU              EmmStat;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAUSER_FSMTranState is entered.");

    EmmStat.enFsmId                   = NAS_LMM_PARALLEL_FSM_EMM;
    EmmStat.enMainState               = stMS;
    EmmStat.enSubState                = stSS;
    EmmStat.enStaTId                  = stTI;
    NAS_LMM_StaTransProc(EmmStat);

    return;
}
VOS_VOID    NAS_EMM_TauUpdateGutiFromCnMsg(
                                const NAS_EMM_CN_TAU_ACP_STRU     *pstTAUAcp)
{
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_TauUpdateGutiFromCnMsg is entered");

    if(NAS_EMM_BIT_SLCT == pstTAUAcp->ucBitOpGuti)
    {
        NAS_LMM_GetEmmInfoUeidAddr()->bitOpGuti = NAS_EMM_BIT_SLCT;
        NAS_LMM_MEM_CPY(                NAS_LMM_GetEmmInfoUeidGutiAddr() ,
                                        &(pstTAUAcp->stGuti),
                                        sizeof(NAS_EMM_GUTI_STRU));

    }

    return;
}


VOS_VOID NAS_EMM_TAUSER_TaiListSet( const NAS_EMM_TA_LIST_STRU *pstTALIST)
{
    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO( "NAS_EMM_TAUSER_TaiListSet is entered.");

    /* ����TAILIST��ȫ�ֱ��� */
    NAS_LMM_MEM_CPY(                            &(g_stEmmInfo.stNetInfo.stTaiList),
                                                pstTALIST,
                                                sizeof(NAS_EMM_TA_LIST_STRU));
    g_stEmmInfo.stNetInfo.bitOpTaiList =        NAS_EMM_BIT_SLCT;

    return;
}


NasMmIntraMsgBlock *    NAS_EMM_TAUSER_GetSendIntraMsgBuffAddr(VOS_UINT32 ulBuffSize)
{
    NasMmIntraMsgBlock *                    pIntraMsg = NAS_EMM_NULL_PTR;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO( "NAS_EMM_TAUSER_GetSendIntraMsgBuffAddr is entered.");

    pIntraMsg = NAS_LMM_GetSendIntraMsgBuffAddr(ulBuffSize );

    return pIntraMsg;
}
VOS_VOID* NAS_EMM_TauRsltCommProc( VOS_VOID )
{
    LMM_MMC_TAU_RESULT_IND_STRU     *pstRsltInd = NAS_EMM_NULL_PTR;

    /* ����MMC�ڲ���Ϣ */
    pstRsltInd  = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_TAU_RESULT_IND_STRU));

    if(NAS_EMM_NULL_PTR == pstRsltInd)
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_TauRsltCommProc: MSG ALLOC ERR !!");
        return NAS_EMM_NULL_PTR;
    }

    NAS_LMM_MEM_SET(pstRsltInd,0,sizeof(LMM_MMC_TAU_RESULT_IND_STRU));

    /* ����MMC��DOPRAͷ */
    EMM_PUBU_COMP_MMC_MSG_HEADER(       pstRsltInd,
                                        sizeof(LMM_MMC_TAU_RESULT_IND_STRU) -
                                        EMM_LEN_VOS_MSG_HEADER);

    /*���OPID*/


    /* �����ϢID */
    pstRsltInd->ulMsgId                 = ID_LMM_MMC_TAU_RESULT_IND;

    return pstRsltInd;
}

VOS_VOID NAS_EMM_TauRstLeadSndAttachRst(MMC_LMM_ATTACH_RSLT_ENUM_UINT32 ulAttRst)
{
    if (NAS_EMM_OK == NAS_EMM_CheckAppMsgPara(ID_MMC_LMM_ATTACH_REQ))
    {
        if( (MMC_LMM_ATT_REQ_TYPE_CS_ONLY == NAS_EMM_GLO_GetAttReqType())
          ||(MMC_LMM_ATT_REQ_TYPE_CS_PS == NAS_EMM_GLO_GetAttReqType()))
        {
           NAS_EMM_AppSendAttRstInTau(ulAttRst);
        }
    }
    return;
}

VOS_VOID NAS_EMM_TauRstSuccLeadSndAttachRst( VOS_VOID)
{
    /* �����ATTACH�����TAU����TAU�������ϱ�ATTACH */
    if (NAS_EMM_OK == NAS_EMM_CheckAppMsgPara(ID_MMC_LMM_ATTACH_REQ))
    {
        if( (MMC_LMM_ATT_REQ_TYPE_CS_ONLY == NAS_EMM_GLO_GetAttReqType())
          ||(MMC_LMM_ATT_REQ_TYPE_CS_PS == NAS_EMM_GLO_GetAttReqType()))
        {
            /* CS_PS��TAU�ɹ�,���ؽ��Ϊ�ɹ�*/
            if( (NAS_EMM_EPS_UPDATE_RST_COMBINED_TA_LA_UPDATED == NAS_EMM_TAU_GetEpsUpdateRslt())
              ||(NAS_EMM_EPS_UPDATE_RST_COMBINED_TA_LA_UPDATED_AND_ISR == NAS_EMM_TAU_GetEpsUpdateRslt()))
            {
                NAS_EMM_AppSendAttRstInTau(MMC_LMM_ATT_RSLT_SUCCESS);
            }
            /* ֻ��PS�ɹ������Ϊʧ�� */
            else
            {
                NAS_EMM_AppSendAttRstInTau(MMC_LMM_ATT_RSLT_FAILURE);
            }
        }
    }
    return;
}
MMC_LMM_TAU_TYPE_ENUM_UINT32  NAS_EMM_MmcTauReqTypeInTauRslt(VOS_VOID)
{
    NAS_EMM_CN_TAU_TYPE_ENUM_UINT32     ulCnTauType;

    ulCnTauType = NAS_EMM_TAU_GetTAUtype();
    if(ulCnTauType == NAS_EMM_CN_TAU_TYPE_TA_UPDATING)
    {
        return  MMC_LMM_TA_UPDATING;
    }
    if(ulCnTauType == NAS_EMM_CN_TAU_TYPE_COMBINED_TA_LA_UPDATING)
    {
        return  MMC_LMM_COMBINED_TA_LA_UPDATING;
    }
    if(ulCnTauType == NAS_EMM_CN_TAU_TYPE_COMBINED_TA_LA_WITH_IMSI)
    {
        return  MMC_LMM_COMBINED_TA_LA_WITH_IMSI;
    }

    if( NAS_LMM_REG_DOMAIN_PS == NAS_EMM_TAU_GetPeriodTauRegDomain())
    {
        return  MMC_LMM_PS_PERIODIC_UPDATING;
    }
    else if(NAS_LMM_REG_DOMAIN_CS_PS == NAS_EMM_TAU_GetPeriodTauRegDomain())
    {
        return  MMC_LMM_CS_PS_PERIODIC_UPDATING;
    }
    else
    {

    }

    NAS_EMM_TAU_LOG_ERR("NAS_EMM_MmcPeriodTauReqType: RegState is WRONG !!");
    return  MMC_LMM_TAU_TYPE_BUTT;

}



VOS_VOID  NAS_EMM_MmcSendTauActionResultIndSucc(VOS_VOID)
{
    LMM_MMC_TAU_RESULT_IND_STRU     *pstRsltInd = NAS_EMM_NULL_PTR;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_MmcSendTauActionResultIndSucc is entered.");

    pstRsltInd = NAS_EMM_TauRsltCommProc();
    if (NAS_EMM_NULL_PTR == pstRsltInd)
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_MmcSendTauActionResultIndSucc: MSG ALLOC ERR !!");
        return;
    }

    /* �����Ϣ���� */
    pstRsltInd->ulTauRst                = MMC_LMM_TAU_RSLT_SUCCESS;
    pstRsltInd->ulReqType               = NAS_EMM_MmcTauReqTypeInTauRslt();

    pstRsltInd->bitOpCnRst              = NAS_EMM_BIT_SLCT;
    pstRsltInd->ulCnRst                 = NAS_EMM_TAU_GetEpsUpdateRslt();


    /*���EPLMN*/
    pstRsltInd->bitOpEplmn              = NAS_EMM_BIT_SLCT;
    if (NAS_EMM_BIT_SLCT == NAS_LMM_GetEmmInfoNetInfoOpEplmnListAddr())
    {
        NAS_EMM_TAU_LOG1_INFO("NAS_EMM_MmcSendTauActionResultIndSucc: EPMN NUM ==",
                            NAS_LMM_GetEmmInfoNetInfoEplmnListAddr()->ulPlmnNum);
        NAS_LMM_MEM_CPY(                &pstRsltInd->stEplmnList,
                                        NAS_LMM_GetEmmInfoNetInfoEplmnListAddr(),
                                        sizeof(MMC_LMM_EPLMN_STRU));
    }

    /*���LAI*/
    if (NAS_EMM_BIT_SLCT == NAS_EMM_TAU_GetOpLai())
    {
        pstRsltInd->bitOpLai            = NAS_EMM_BIT_SLCT;
        NAS_LMM_MEM_CPY(                &pstRsltInd->stLai,
                                        NAS_EMM_TAU_GetLaiAddr(),
                                        sizeof(MMC_LMM_LAI_STRU));
    }
    /*���TMSI*/
    if (NAS_EMM_BIT_SLCT == NAS_EMM_TAU_GetOpMsId())
    {
        pstRsltInd->bitOpMsIdentity           = NAS_EMM_BIT_SLCT;
        NAS_LMM_MEM_CPY(                 pstRsltInd->stMsIdentity.aucMsIdentity,
                                        NAS_EMM_TAU_GetMsIdAddr()->aucMsId,
                                        NAS_MAX_SIZE_MS_IDENTITY);
    }
    /*�����Ϣ����----EMM CAUSE*/
    if (NAS_EMM_BIT_SLCT == NAS_EMM_TAU_GetOpCnCause())
    {
        NAS_EMM_ATTACH_LOG_NORM("NAS_EMM_AppSendAttSucc:EMM CAUSE");
        pstRsltInd->bitOpCnCause        = NAS_EMM_BIT_SLCT;
        pstRsltInd->ucCnCause           = NAS_EMM_TAU_GetCnCause();
    }
    /* ���TAU ATTEMPT COUTER */
    if (NAS_EMM_NULL != NAS_EMM_TAU_GetEmmTAUAttemptCnt())
    {
        pstRsltInd->bitOpAtmpCnt        = NAS_EMM_BIT_SLCT;
        pstRsltInd->ulAttemptCount      = NAS_EMM_TAU_GetEmmTAUAttemptCnt();
    }
    /* ����LMM_MMC_TAU_RESULT_IND��Ϣ */
    NAS_LMM_SendLmmMmcMsg(                    pstRsltInd);

    NAS_EMM_TauRstSuccLeadSndAttachRst();
    return;
}


VOS_VOID  NAS_EMM_MmcSendTauActionResultIndRejTaUpdatedOnly
(
    const VOS_VOID                     *pvTauRslt
)
{
    LMM_MMC_TAU_RESULT_IND_STRU     *pstRsltInd = NAS_EMM_NULL_PTR;
    VOS_UINT32                      ulTaRslt;

    (VOS_VOID)pvTauRslt;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_MmcSendTauActionResultIndRejTaUpdatedOnly is entered.");

    pstRsltInd = NAS_EMM_TauRsltCommProc();
    if (NAS_EMM_NULL_PTR == pstRsltInd)
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_MmcSendTauActionResultIndRejTaUpdatedOnly: MSG ALLOC ERR !!");
        return;
    }

    /* �����Ϣ���� */
    pstRsltInd->ulTauRst                = MMC_LMM_TAU_RSLT_CN_REJ;
    pstRsltInd->ulReqType               = NAS_EMM_MmcTauReqTypeInTauRslt();

    pstRsltInd->bitOpCnRst              = NAS_EMM_BIT_SLCT;
    pstRsltInd->ulCnRst                 = NAS_EMM_TAU_GetEpsUpdateRslt();

    /*���EPLMN*/
    pstRsltInd->bitOpEplmn              = NAS_EMM_BIT_SLCT;
    if (NAS_EMM_BIT_SLCT == NAS_LMM_GetEmmInfoNetInfoOpEplmnListAddr())
    {
        NAS_EMM_TAU_LOG1_INFO("NAS_EMM_MmcSendTauActionResultIndRejTaUpdatedOnly: EPMN NUM ==",
                            NAS_LMM_GetEmmInfoNetInfoEplmnListAddr()->ulPlmnNum);
        NAS_LMM_MEM_CPY(                &pstRsltInd->stEplmnList,
                                        NAS_LMM_GetEmmInfoNetInfoEplmnListAddr(),
                                        sizeof(MMC_LMM_EPLMN_STRU));
    }

    /*���LAI*/
    if (NAS_EMM_BIT_SLCT == NAS_EMM_TAU_GetOpLai())
    {
        pstRsltInd->bitOpLai            = NAS_EMM_BIT_SLCT;
        NAS_LMM_MEM_CPY(                &pstRsltInd->stLai,
                                        NAS_EMM_TAU_GetLaiAddr(),
                                        sizeof(MMC_LMM_LAI_STRU));
    }

    /*���TMSI*/
    if (NAS_EMM_BIT_SLCT == NAS_EMM_TAU_GetOpMsId())
    {
        pstRsltInd->bitOpMsIdentity           = NAS_EMM_BIT_SLCT;
        NAS_LMM_MEM_CPY(                 pstRsltInd->stMsIdentity.aucMsIdentity,
                                        NAS_EMM_TAU_GetMsIdAddr()->aucMsId,
                                        NAS_MAX_SIZE_MS_IDENTITY);
    }

    /* ԭ��ֵ��Ϊ#2#16#17#18#22ʱ����MMC�ϱ�ԭ��ֵ */
    /*�����Ϣ����----EMM CAUSE*/
    pstRsltInd->bitOpCnCause            = NAS_EMM_BIT_NO_SLCT;
    pstRsltInd->ucCnCause               = NAS_EMM_CAUSE_BUTT;

    /* ���TAU ATTEMPT COUTER */
    if (NAS_EMM_NULL != NAS_EMM_TAU_GetEmmTAUAttemptCnt())
    {
        pstRsltInd->bitOpAtmpCnt        = NAS_EMM_BIT_SLCT;
        pstRsltInd->ulAttemptCount      = NAS_EMM_TAU_GetEmmTAUAttemptCnt();

        /* ���TAU���Դ���С��5����Я��EPS update status��TAI�Ƿ���TAI LIST�б�ʶ */
        if (NAS_EMM_TAU_ATTEMPT_CNT_MAX > pstRsltInd->ulAttemptCount)
        {
            pstRsltInd->bitOpEpsUpdateStuts = NAS_EMM_BIT_SLCT;
            pstRsltInd->enEmmUpStat         = NAS_EMM_UPDATE_STAE;
            pstRsltInd->bitOpTaiInListFlag  = NAS_EMM_BIT_SLCT;

            ulTaRslt                        = NAS_EMM_TAU_IsCurrentTAInTaList();
            if (NAS_EMM_SUCC == ulTaRslt)
            {
                pstRsltInd->enTaiInListFlag = MMC_LMM_TAI_IN_TAI_LIST;
            }
            else
            {
                pstRsltInd->enTaiInListFlag = MMC_LMM_TAI_NOT_IN_TAI_LIST;
            }
        }
    }

    /* ����LMM_MMC_TAU_RESULT_IND��Ϣ */
    NAS_LMM_SendLmmMmcMsg(                    pstRsltInd);

    /* �����ATTACH�����TAU����TAU�������ϱ�ATTACH */
    NAS_EMM_TauRstLeadSndAttachRst(MMC_LMM_ATT_RSLT_FAILURE);

    return;
}
VOS_VOID  NAS_EMM_MmcSendTauActionResultIndSuccWithoutPara( VOS_VOID )
{
    LMM_MMC_TAU_RESULT_IND_STRU     *pstRsltInd = NAS_EMM_NULL_PTR;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_MmcSendTauActionResultIndSuccWithoutPara is entered.");

    pstRsltInd = NAS_EMM_TauRsltCommProc();
    if (NAS_EMM_NULL_PTR == pstRsltInd)
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_MmcSendTauActionResultIndSuccWithoutPara: MSG ALLOC ERR !!");
        return;
    }

    /* �����Ϣ���� */
    pstRsltInd->ulTauRst                = MMC_LMM_TAU_RSLT_SUCCESS;
    pstRsltInd->ulReqType               = NAS_EMM_MmcTauReqTypeInTauRslt();


    /* ����LMM_MMC_TAU_RESULT_IND��Ϣ */
    NAS_LMM_SendLmmMmcMsg(                    pstRsltInd);

    /* �����ATTACH�����TAU����TAU�������ϱ�ATTACH */
    NAS_EMM_TauRstLeadSndAttachRst(MMC_LMM_ATT_RSLT_FAILURE);

    return;
}
/*****************************************************************************
 Function Name   : NAS_EMM_IsTauRejCauseOthers
 Description     : �ж�TAU REJԭ��ֵ�Ƿ�Ϊ5.5.3.2.5�½�����
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2011-11-18  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_IsTauRejCauseOthers
(
    VOS_UINT8                           ucCnCause
)
{
    VOS_UINT32                          ulRslt = NAS_EMM_YES;

    switch(ucCnCause)
    {
        case    NAS_LMM_CAUSE_ILLEGAL_UE   :
        case    NAS_LMM_CAUSE_ILLEGAL_ME   :
        case    NAS_LMM_CAUSE_EPS_SERV_NOT_ALLOW   :
        case    NAS_LMM_CAUSE_EPS_SERV_AND_NON_EPS_SERV_NOT_ALLOW:
        case    NAS_LMM_CAUSE_UE_ID_NOT_DERIVED    :
        case    NAS_LMM_CAUSE_IMPLICIT_DETACHED   :
        case    NAS_LMM_CAUSE_PLMN_NOT_ALLOW    :
        case    NAS_LMM_CAUSE_TA_NOT_ALLOW   :
        case    NAS_LMM_CAUSE_ROAM_NOT_ALLOW    :
        case    NAS_LMM_CAUSE_EPS_SERV_NOT_ALLOW_IN_PLMN    :
        case    NAS_LMM_CAUSE_NO_SUITABL_CELL   :
        case    NAS_LMM_CAUSE_NOT_AUTHORIZED_FOR_THIS_CSG   :
        case    NAS_LMM_CAUSE_NO_EPS_BEARER_CONTEXT_ACTIVATED:

                ulRslt = NAS_EMM_NO;
                break;
        default :

                ulRslt = NAS_EMM_YES;
                break;
    }

    return ulRslt;
}



VOS_VOID  NAS_EMM_MmcSendTauActionResultIndRej
(
    const VOS_VOID                     *pvTauRslt
)
{
    LMM_MMC_TAU_RESULT_IND_STRU     *pstRsltInd  = NAS_EMM_NULL_PTR;
    VOS_UINT32                          ulTaRslt = NAS_EMM_FAIL;
    NAS_LMM_CN_CAUSE_ENUM_UINT8         ucCnCause  = NAS_EMM_CAUSE_BUTT;

    (VOS_VOID)pvTauRslt;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_MmcSendTauActionResultIndRej is entered.");

    pstRsltInd = NAS_EMM_TauRsltCommProc();
    if (NAS_EMM_NULL_PTR == pstRsltInd)
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_MmcSendTauActionResultIndRej: MSG ALLOC ERR !!");
        return;
    }

    ucCnCause                             = NAS_EMM_TAU_GetEmmTauCnRejCause();
    /* �����Ϣ���� */
    pstRsltInd->ulTauRst                = MMC_LMM_TAU_RSLT_CN_REJ;
    pstRsltInd->ulReqType               = NAS_EMM_MmcTauReqTypeInTauRslt();


    /*���ܾ�ԭ��ֵ*/
    pstRsltInd->bitOpCnCause            = NAS_EMM_BIT_SLCT;
    pstRsltInd->ucCnCause               = ucCnCause;

    if (NAS_EMM_NULL != NAS_EMM_TAU_GetEmmTAUAttemptCnt())
    {
        pstRsltInd->bitOpAtmpCnt        = NAS_EMM_BIT_SLCT;
        pstRsltInd->ulAttemptCount      = NAS_EMM_TAU_GetEmmTAUAttemptCnt();

        /* ���TAU REJԭ��ֵ��Ϊ5.5.3.2.5�½����У���TAU���Դ���С��5��
           ��Я��EPS update status��TAI�Ƿ���TAI LIST�б�ʶ */
        if ((NAS_EMM_YES == NAS_EMM_IsTauRejCauseOthers(ucCnCause))
            && (NAS_EMM_TAU_ATTEMPT_CNT_MAX > pstRsltInd->ulAttemptCount))
        {
            pstRsltInd->bitOpEpsUpdateStuts = NAS_EMM_BIT_SLCT;
            pstRsltInd->enEmmUpStat         = NAS_EMM_UPDATE_STAE;
            pstRsltInd->bitOpTaiInListFlag  = NAS_EMM_BIT_SLCT;

            ulTaRslt                        = NAS_EMM_TAU_IsCurrentTAInTaList();
            if (NAS_EMM_SUCC == ulTaRslt)
            {
                pstRsltInd->enTaiInListFlag = MMC_LMM_TAI_IN_TAI_LIST;
            }
            else
            {
                pstRsltInd->enTaiInListFlag = MMC_LMM_TAI_NOT_IN_TAI_LIST;
            }
        }
    }

    /* ����LMM_MMC_TAU_RESULT_IND��Ϣ */
    NAS_LMM_SendLmmMmcMsg(                    pstRsltInd);

    /* �����ATTACH�����TAU����TAU�������ϱ�ATTACH */
    NAS_EMM_TauRstLeadSndAttachRst(MMC_LMM_ATT_RSLT_FAILURE);

    return;
}


VOS_VOID  NAS_EMM_MmcSendTauActionResultIndTimerExp
(
    const VOS_VOID                     *pvTauRslt
)
{
    LMM_MMC_TAU_RESULT_IND_STRU     *pstRsltInd  = NAS_EMM_NULL_PTR;
    VOS_UINT32                          ulTaRslt = NAS_EMM_FAIL;

    (VOS_VOID)pvTauRslt;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_MmcSendTauActionResultIndTimerExp is entered.");

    pstRsltInd = NAS_EMM_TauRsltCommProc();
    if (NAS_EMM_NULL_PTR == pstRsltInd)
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_MmcSendTauActionResultIndTimerExp: MSG ALLOC ERR !!");
        return;
    }

    /* �����Ϣ���� */
    pstRsltInd->ulTauRst                = MMC_LMM_TAU_RSLT_TIMER_EXP;
    pstRsltInd->ulReqType               = NAS_EMM_MmcTauReqTypeInTauRslt();


    /*��䳢�Դ���*/
    pstRsltInd->bitOpAtmpCnt            = NAS_EMM_BIT_SLCT;
    pstRsltInd->ulAttemptCount          = NAS_EMM_TAU_GetEmmTAUAttemptCnt();

   /* ���TAU���Դ���С��5����Я��EPS update status��TAI�Ƿ���TAI LIST�б�ʶ */
    if (NAS_EMM_TAU_ATTEMPT_CNT_MAX > pstRsltInd->ulAttemptCount)
    {
        pstRsltInd->bitOpEpsUpdateStuts = NAS_EMM_BIT_SLCT;
        pstRsltInd->enEmmUpStat         = NAS_EMM_UPDATE_STAE;
        pstRsltInd->bitOpTaiInListFlag  = NAS_EMM_BIT_SLCT;

        ulTaRslt                        = NAS_EMM_TAU_IsCurrentTAInTaList();
        if (NAS_EMM_SUCC == ulTaRslt)
        {
            pstRsltInd->enTaiInListFlag = MMC_LMM_TAI_IN_TAI_LIST;
        }
        else
        {
            pstRsltInd->enTaiInListFlag = MMC_LMM_TAI_NOT_IN_TAI_LIST;
        }
    }

    /* ����LMM_MMC_TAU_RESULT_IND��Ϣ */
    NAS_LMM_SendLmmMmcMsg(                    pstRsltInd);

    /* �����ATTACH�����TAU����TAU�������ϱ�ATTACH */
    NAS_EMM_TauRstLeadSndAttachRst(MMC_LMM_ATT_RSLT_FAILURE);

    return;
}



VOS_VOID  NAS_EMM_MmcSendTauActionResultIndFailWithPara
(
    const VOS_VOID                     *pvTauRslt
)
{
    LMM_MMC_TAU_RESULT_IND_STRU     *pstRsltInd  = NAS_EMM_NULL_PTR;
    VOS_UINT32                          ulTaRslt = NAS_EMM_FAIL;

    (VOS_VOID)pvTauRslt;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_MmcSendTauActionResultIndFailWithPara is entered.");

    pstRsltInd = NAS_EMM_TauRsltCommProc();
    if (NAS_EMM_NULL_PTR == pstRsltInd)
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_MmcSendTauActionResultIndFailWithPara: MSG ALLOC ERR !!");
        return;
    }

    /* �����Ϣ���� */
    pstRsltInd->ulTauRst                = MMC_LMM_TAU_RSLT_FAILURE;

    pstRsltInd->ulReqType               = NAS_EMM_MmcTauReqTypeInTauRslt();


    if (NAS_EMM_NULL != NAS_EMM_TAU_GetEmmTAUAttemptCnt())
    {
        pstRsltInd->bitOpAtmpCnt        = NAS_EMM_BIT_SLCT;
        pstRsltInd->ulAttemptCount      = NAS_EMM_TAU_GetEmmTAUAttemptCnt();

        /* ���TAU���Դ���С��5����Я��EPS update status��TAI�Ƿ���TAI LIST�б�ʶ */
        if (NAS_EMM_TAU_ATTEMPT_CNT_MAX > pstRsltInd->ulAttemptCount)
        {
            pstRsltInd->bitOpEpsUpdateStuts = NAS_EMM_BIT_SLCT;
            pstRsltInd->enEmmUpStat         = NAS_EMM_UPDATE_STAE;
            pstRsltInd->bitOpTaiInListFlag  = NAS_EMM_BIT_SLCT;

            ulTaRslt                        = NAS_EMM_TAU_IsCurrentTAInTaList();
            if (NAS_EMM_SUCC == ulTaRslt)
            {
                pstRsltInd->enTaiInListFlag = MMC_LMM_TAI_IN_TAI_LIST;
            }
            else
            {
                pstRsltInd->enTaiInListFlag = MMC_LMM_TAI_NOT_IN_TAI_LIST;
            }
        }
    }

    /* ����LMM_MMC_TAU_RESULT_IND��Ϣ */
    NAS_LMM_SendLmmMmcMsg(                    pstRsltInd);

    /* �����ATTACH�����TAU����TAU�������ϱ�ATTACH */
    NAS_EMM_TauRstLeadSndAttachRst(MMC_LMM_ATT_RSLT_FAILURE);

    return;
}
VOS_VOID  NAS_EMM_MmcSendTauActionResultIndOthertype
(
    const VOS_VOID                     *pvTauRslt
)
{
    LMM_MMC_TAU_RESULT_IND_STRU     *pstRsltInd = NAS_EMM_NULL_PTR;
    MMC_LMM_TAU_RSLT_ENUM_UINT32     ulTauRslt;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_MmcSendTauActionResultIndOthertype is entered.");

    ulTauRslt = *(MMC_LMM_TAU_RSLT_ENUM_UINT32*)pvTauRslt;


    pstRsltInd = NAS_EMM_TauRsltCommProc();
    if (NAS_EMM_NULL_PTR == pstRsltInd)
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_MmcSendTauActionResultIndOthertype: MSG ALLOC ERR !!");
        return;
    }

    /* �����Ϣ���� */
    if ((MMC_LMM_TAU_RSLT_ACCESS_BARED == ulTauRslt) ||
        (MMC_LMM_TAU_RSLT_AUTH_REJ == ulTauRslt) ||
        (MMC_LMM_TAU_RSLT_FAILURE == ulTauRslt)||
        (MMC_LMM_TAU_RSLT_T3402_RUNNING == ulTauRslt)||
        (MMC_LMM_TAU_RSLT_MO_DETACH_FAILURE == ulTauRslt)||
        (MMC_LMM_TAU_RSLT_MT_DETACH_FAILURE == ulTauRslt)||
        (MMC_LMM_TAU_RSLT_T3402_RUNNING == ulTauRslt))
    {
        NAS_EMM_TAU_LOG1_NORM("NAS_EMM_MmcSendTauActionResultIndOthertype:ulTauRslt =",
                            ulTauRslt);
        pstRsltInd->ulTauRst            = ulTauRslt;
    }
    else
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_MmcSendTauActionResultIndOthertype:\
                                Input TAU Result Type is err !!");
    }

    pstRsltInd->ulReqType               = NAS_EMM_MmcTauReqTypeInTauRslt();


    /* ����LMM_MMC_TAU_RESULT_IND��Ϣ */
    NAS_LMM_SendLmmMmcMsg(                    pstRsltInd);

    /* �����ATTACH�����TAU����TAU�������ϱ�ATTACH */
    NAS_EMM_TauRstLeadSndAttachRst(MMC_LMM_ATT_RSLT_FAILURE);

    return;
}


VOS_VOID  NAS_EMM_MmcSendTauActionResultIndForbType
(
    const VOS_VOID                     *pvForbInfo
)
{
    LMM_MMC_TAU_RESULT_IND_STRU     *pstRsltInd = NAS_EMM_NULL_PTR;
    EMMC_EMM_FORBIDDEN_INFO_ENUM_UINT32 ulForbInfo;

    ulForbInfo = *(EMMC_EMM_FORBIDDEN_INFO_ENUM_UINT32*)pvForbInfo;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_MmcSendTauActionResultIndForbType is entered.");

    pstRsltInd = NAS_EMM_TauRsltCommProc();
    if (NAS_EMM_NULL_PTR == pstRsltInd)
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_MmcSendTauActionResultIndForbType: MSG ALLOC ERR !!");
        return;
    }

    NAS_EMM_TAU_LOG1_NORM("NAS_EMM_MmcSendTauActionResultIndForbType:ulForbInfo =",
                            ulForbInfo);

    /* �����Ϣ���� */
    if (EMMC_EMM_FORBIDDEN_PLMN == ulForbInfo)
    {
        pstRsltInd->ulTauRst            = MMC_LMM_TAU_RSLT_FORBID_PLMN;
    }
    else if (EMMC_EMM_FORBIDDEN_PLMN_FOR_GPRS == ulForbInfo)
    {
        pstRsltInd->ulTauRst            = MMC_LMM_TAU_RSLT_FORBID_PLMN_FOR_GPRS;
    }
    else if (EMMC_EMM_FORBIDDEN_TA_FOR_ROAMING == ulForbInfo)
    {
        pstRsltInd->ulTauRst            = MMC_LMM_TAU_RSLT_FORBID_TA_FOR_ROAMING;
    }
    else if (EMMC_EMM_FORBIDDEN_TA_FOR_RPOS == ulForbInfo)
    {
        pstRsltInd->ulTauRst            = MMC_LMM_TAU_RSLT_FORBID_TA_FOR_RPOS;
    }
    else if(EMMC_EMM_NO_FORBIDDEN == ulForbInfo)
    {
        /* ϵͳ��Ϣδ���������ǵ��ô˺����ϱ������һ����ANY CELL��ϵͳ��Ϣ */
        pstRsltInd->ulTauRst            = MMC_LMM_TAU_RSLT_FAILURE;
    }
    else
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_MmcSendTauActionResultIndForbType:\
                                Input TAU Result Type is err !!");
        pstRsltInd->ulTauRst            = MMC_LMM_TAU_RSLT_FAILURE;
    }

    pstRsltInd->ulReqType               =  NAS_EMM_MmcTauReqTypeInTauRslt();


    /* ����LMM_MMC_TAU_RESULT_IND��Ϣ */
    NAS_LMM_SendLmmMmcMsg(                    pstRsltInd);

    /* �����ATTACH�����TAU����TAU�������ϱ�ATTACH */
    NAS_EMM_TauRstLeadSndAttachRst(MMC_LMM_ATT_RSLT_FAILURE);

    return;
}
VOS_UINT32    NAS_EMM_SendIntraAttachReq(VOS_VOID)
{
    NAS_LMM_INTRA_ATTACH_REQ_STRU        *pIntraAttachReqMsg = NAS_EMM_NULL_PTR;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO( "NAS_EMM_SendAttachReq is entered.");

    /* ����DOPRA��Ϣ */
    pIntraAttachReqMsg = (VOS_VOID *)NAS_EMM_TAUSER_GetSendIntraMsgBuffAddr(
                                        sizeof(NAS_LMM_INTRA_ATTACH_REQ_STRU) -
                                        EMM_LEN_VOS_MSG_HEADER);

    if(NAS_EMM_NULL_PTR == pIntraAttachReqMsg)
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_SendAttachReq: MSG ALLOC ERR !!");
        return NAS_EMM_FAIL;
    }

    /* ��дNAS_EMM_INTRA_ATTACH_REQ ��DOPRA��Ϣͷ */
    EMM_COMP_MM_MSG_HEADER(pIntraAttachReqMsg, sizeof(NAS_LMM_INTRA_ATTACH_REQ_STRU) -
                                        EMM_LEN_VOS_MSG_HEADER);

    /* ��дNAS_EMM_INTRA_ATTACH_REQ ����ϢID��ʶ */
    pIntraAttachReqMsg->ulMsgId         = ID_NAS_LMM_INTRA_ATTACH_REQ;

    /* ������Ϣ(Send the msg of) NAS_EMM_INTRA_ATTACH_REQ */
    NAS_EMM_SEND_INTRA_MSG(             pIntraAttachReqMsg);

    return NAS_EMM_SUCC;

}
VOS_VOID NAS_EMM_TAU_AbnormalOver( )
{
    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO( "NAS_EMM_TAU_AbnormalOver is entered.");

    /*ֹͣT3430��ʱ��*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_TAU_T3430);

    /*ֹͣT3440��ʱ��*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);

    /* �������TAU��ԭ��ֵ*/
    NAS_EMM_TAU_SaveEmmTAUStartCause(   NAS_EMM_TAU_START_CAUSE_DEFAULT);

    /* ���Tau Attempt Counter*/
    NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);

    return;
}
NAS_EMM_UPDATE_STATE_ENUM_UINT8 NAS_EMM_TAU_GetEmmEPSUPState()
{
    NAS_EMM_UPDATE_STATE_ENUM_UINT8   ucupdatastate;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO( "NAS_EMM_TAU_GetEmmEPSUPState is entered.");

    ucupdatastate = NAS_EMM_TAUSER_GetAuxFsmUpStat();

    return ucupdatastate;
}
VOS_VOID    NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_ENUM_UINT32  stATTACHStatus)
{


    EMM_ESM_STATUS_IND_STRU                *pstsmstatusInd            = NAS_EMM_NULL_PTR;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG1_INFO( "NAS_EMM_TAU_SendEsmStatusInd is entered.", stATTACHStatus);

    /* ����DOPRA��Ϣ */
    pstsmstatusInd  = (VOS_VOID *)NAS_LMM_ALLOC_MSG(sizeof(EMM_ESM_STATUS_IND_STRU));

    if(NAS_EMM_NULL_PTR == pstsmstatusInd)
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_TAU_SendEsmStatusInd: MSG ALLOC ERR !!");
        return;
    }

    /* ����RRC��DOPRAͷ */
    EMM_COMP_ESM_MSG_HEADER(pstsmstatusInd, sizeof(EMM_ESM_STATUS_IND_STRU) -
                                        EMM_LEN_VOS_MSG_HEADER);

    /* �����ϢID */
    pstsmstatusInd->ulMsgId = ID_EMM_ESM_STATUS_IND;

    /* �����Ϣ���� -- stATTACHStatus */
    pstsmstatusInd->enEMMStatus = stATTACHStatus;

    /*clear global:EpsContextStatusChange*/
    if(stATTACHStatus == EMM_ESM_ATTACH_STATUS_DETACHED)
    {
        NAS_EMM_GetEpsContextStatusChange() = NAS_EMM_EPS_BEARER_STATUS_NOCHANGE;
    }

    /* ����DOPRA��Ϣ */
    NAS_LMM_SEND_MSG(pstsmstatusInd);

    return;

}
/*****************************************************************************
 Function Name   : NAS_EMM_TAU_GetEpsUpdateType
 Description     : ����TAU REQ��Ϣʱ����ȡEps Update Type��ֵ
 Input           : ��
 Output          : ��
 Return          : MMC_LMM_TAU_TYPE_ENUM_UINT32

 History         :
    1.lihong00150010      2011-11-18    Draft Enact

*****************************************************************************/
NAS_EMM_CN_TAU_TYPE_ENUM_UINT32 NAS_EMM_TAU_GetEpsUpdateType
(
    NAS_EMM_TAU_START_CAUSE_ENUM_UINT8      enTauStartCause
)
{
    NAS_LMM_REG_DOMAIN_ENUM_UINT32          enRegDomain     = NAS_LMM_REG_DOMAIN_BUTT;


    /* ��ȡLģע���� */
    enRegDomain = NAS_LMM_GetEmmInfoRegDomain();

    if (NAS_EMM_TAU_START_CAUSE_T3412EXP == enTauStartCause)
    {
        /* ����������TAU��ע����
            ������MMC�ϱ����ʱ������PS_PERIOD_TAU����CS_PS_PERIOD_TAU */
        NAS_EMM_TAU_SavePeriodTauRegDomain(enRegDomain);
        return  NAS_EMM_CN_TAU_TYPE_PERIODIC_UPDATING;
    }

    /* �ж��Ƿ�ΪCP/PS1����CS/PS2 */
    if (NAS_EMM_NO == NAS_EMM_IsCsPsUeMode())
    {
        return  NAS_EMM_CN_TAU_TYPE_TA_UPDATING;
    }

    /* �ж��Ƿ�ԭ��ֵ2�ܾ��� */
    if (NAS_EMM_REJ_YES == NAS_LMM_GetEmmInfoRejCause2Flag())
    {
        return  NAS_EMM_CN_TAU_TYPE_TA_UPDATING;
    }

    /* �ж��Ƿ�ԭ��ֵ18�ܾ��� */
    if (NAS_EMM_REJ_YES == NAS_EMMC_GetRejCause18Flag())
    {
        return  NAS_EMM_CN_TAU_TYPE_TA_UPDATING;
    }

    /* �ж�����DETACH�Ƿ�TAU��� */
    if (NAS_EMM_COM_DET_BLO_YES == NAS_EMM_TAU_GetEmmCombinedDetachBlockFlag())
    {
        return  NAS_EMM_CN_TAU_TYPE_COMBINED_TA_LA_UPDATING;
    }
    /*PC REPLAY MODIFY BY LEILI BEGIN*/
    /* �ж��Ƿ�����CS��ע�� */
    if (NAS_EMM_AUTO_ATTACH_NOT_ALLOW == NAS_EMM_GetCsAttachAllowFlg())
    {
    /*PC REPLAY MODIFY BY LEILI END*/
        return  NAS_EMM_CN_TAU_TYPE_TA_UPDATING;
    }

    if(NAS_LMM_REG_DOMAIN_PS == enRegDomain)
    {
        return  NAS_EMM_CN_TAU_TYPE_COMBINED_TA_LA_WITH_IMSI;
    }
    else if(NAS_LMM_REG_DOMAIN_CS_PS == enRegDomain)
    {
        return  NAS_EMM_CN_TAU_TYPE_COMBINED_TA_LA_UPDATING;
    }
    else
    {
        /* ��ӡ�쳣 */
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_TAU_GetEpsUpdateType:enRegDomain = NAS_LMM_REG_DOMAIN_NULL");

        return  NAS_EMM_CN_TAU_TYPE_COMBINED_TA_LA_UPDATING;
    }
}


VOS_VOID    NAS_EMM_TAU_CompCnTAUReqNasMsg
(
    VOS_UINT8      *pucCnNasMsg,
    VOS_UINT32     *pulIndex
)
{
    VOS_UINT32                          ulIndex             = 0;
    VOS_UINT32                          ulIeLength          = 0;
    VOS_UINT8                           ucKsi;
    NAS_EMM_CN_TAU_TYPE_ENUM_UINT32     enTauUpdateType     = NAS_EMM_CN_TAU_TYPE_BUTT;
    NAS_EMM_TAU_START_CAUSE_ENUM_UINT8  enTauStartCause     = NAS_EMM_TAU_START_CAUSE_BUTT;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG1_INFO("NAS_EMM_TAU_CompCnTAUReqNasMsg is entered.",*pulIndex);


    /* ��� Security header type  +  Protocol Discriminator */
    pucCnNasMsg[ulIndex++]              = EMM_TAU_REQ_MSG_PD_AND_SEC_TY;

    /* ��� TAU request message identity   */
    pucCnNasMsg[ulIndex++]              = NAS_EMM_CN_MT_TAU_REQ;

    /* ��ȡTAU�������� */
    enTauStartCause = NAS_EMM_TAU_GetEmmTAUStartCause();
    enTauUpdateType = NAS_EMM_TAU_GetEpsUpdateType(enTauStartCause);
    NAS_EMM_TAU_LOG1_NORM("Tau Type: ", enTauUpdateType);
    NAS_EMM_TAU_SaveTAUtype(enTauUpdateType);

    /* ��� EPS update type */
    pucCnNasMsg[ulIndex] = (VOS_UINT8)NAS_EMM_TAU_GetTAUtype();
    pucCnNasMsg[ulIndex] |= (NAS_EMM_TAU_GetEmmTauReqActiveCtrl()&0x1f)<<3;

    /* �����ԪNAS key set identifierASME */
    ucKsi = NAS_EMM_SecuGetKSIValue();
    pucCnNasMsg[ulIndex] |= (VOS_UINT8)(ucKsi << 4);
    ulIndex++;

    /*���OLD GUTI*/
    NAS_EMM_EncodeOldGuti(              &(pucCnNasMsg[ulIndex]),
                                        &ulIeLength);
    ulIndex                             += ulIeLength;

    /*���Non-current native NAS key set identifier*/
    NAS_EMM_EncodeNonCurrentNativeKSI(  &(pucCnNasMsg[ulIndex]),
                                        &ulIeLength);
    ulIndex                             += ulIeLength;

    /*���GPRS ciphering key sequence number*/
    NAS_EMM_EncodeGprsCkSn(             &(pucCnNasMsg[ulIndex]),
                                        &ulIeLength);
    ulIndex                             += ulIeLength;

    /* ���Old P-TMSI signature*/
    NAS_EMM_EncodePtmsiSignature(       &(pucCnNasMsg[ulIndex]),
                                        &ulIeLength);
    ulIndex                             += ulIeLength;

    /*���Additional GUTI*/
    NAS_EMM_EncodeAdditionalGuti(       &(pucCnNasMsg[ulIndex]),
                                        &ulIeLength);
    ulIndex                             += ulIeLength;

    /*���NonceUE*/
    NAS_EMM_EncodeNonceUE(              &(pucCnNasMsg[ulIndex]),
                                        &ulIeLength);
    ulIndex                             += ulIeLength;

    /* ���UE network capability*/
    if((EMM_TAU_AD_BIT_SLCT == NAS_EMM_TAU_UE_NET_CAP_BIT())
        && ( NAS_EMM_CN_TAU_TYPE_PERIODIC_UPDATING != NAS_EMM_TAU_GetTAUtype()))
    {
        pucCnNasMsg[ulIndex++]          = NAS_EMM_TAU_REQ_UE_NET_CAP_IEI;

        /* UE NETWORK CAPABILITY �Ǳ�ѡIE, LV��3~14*/
        NAS_EMM_EncodeUeNetCapa(        &(pucCnNasMsg[ulIndex]),
                                        NAS_LMM_IE_FORMAT_LV,
                                        &ulIeLength);
        ulIndex += ulIeLength;

    }

    /* ���Last visited registered TAI */
    NAS_EMM_EncodeLRVTai(               &(pucCnNasMsg[ulIndex]),
                                        &ulIeLength);
    ulIndex                             += ulIeLength;

    /* ��� DRX parameter */
    NAS_EMM_EncodeDrxPara(              &(pucCnNasMsg[ulIndex]),
                                        &ulIeLength,
                                        NAS_EMM_CN_MT_TAU_REQ);
    ulIndex                             += ulIeLength;


    /*���UE radio capability information update needed*/
    NAS_EMM_EncodeURCUpdateNeed(        &(pucCnNasMsg[ulIndex]),
                                        &ulIeLength);
    ulIndex                             += ulIeLength;

    /* ���EPS bearer context status */
    /*leili modify for isr begin*/
    NAS_EMM_EncodeEpsBearStatus(        &(pucCnNasMsg[ulIndex]),
                                        &ulIeLength);
    ulIndex                             += ulIeLength;
    /*leili modify for isr end*/
    /* ��� MS network capability */
    NAS_EMM_EncodeMsNetworkCapability(  &(pucCnNasMsg[ulIndex]),
                                        &ulIeLength,
                                        NAS_EMM_CN_MT_TAU_REQ);
    ulIndex                             += ulIeLength;


    /* ���Old location area identification */
    NAS_EMM_EncodeLai(                  &(pucCnNasMsg[ulIndex]),
                                        &ulIeLength,
                                        NAS_EMM_CN_MT_TAU_REQ);
    ulIndex                             += ulIeLength;


    /*���TMSI STATUS*/
    NAS_EMM_EncodeTmsiStatus(           &(pucCnNasMsg[ulIndex]),
                                        &ulIeLength,
                                        NAS_EMM_CN_MT_TAU_REQ);
    ulIndex                             += ulIeLength;

    /*���Mobile station classmark 2*/
    NAS_EMM_EncodeMsClassMark2(         &(pucCnNasMsg[ulIndex]),
                                        &ulIeLength,
                                        NAS_EMM_CN_MT_TAU_REQ);
    ulIndex                             += ulIeLength;

    /*���Mobile station classmark 3*/
    NAS_EMM_EncodeMsClassMark3(         &(pucCnNasMsg[ulIndex]),
                                        &ulIeLength);
    ulIndex                             += ulIeLength;

    /*���Supported Codecs*/
    NAS_EMM_EncodeSupportedCodecs(      &(pucCnNasMsg[ulIndex]),
                                        &ulIeLength);
    ulIndex                             += ulIeLength;

    /*���Additional update type*/
    if ((NAS_EMM_CN_TAU_TYPE_COMBINED_TA_LA_UPDATING  == NAS_EMM_TAU_GetTAUtype()) ||
        (NAS_EMM_CN_TAU_TYPE_COMBINED_TA_LA_WITH_IMSI == NAS_EMM_TAU_GetTAUtype()))
    {
        NAS_EMM_EncodeAdditionUpdateType(   &(pucCnNasMsg[ulIndex]),
                                            &ulIeLength);
        ulIndex                             += ulIeLength;
    }

    /*���Voice domain preference and UE's usage setting*/
    NAS_EMM_EncodeVoiceDomainAndUsageSetting(&(pucCnNasMsg[ulIndex]),
                                        &ulIeLength);
    ulIndex                             += ulIeLength;
    if (NAS_RELEASE_CTRL)
    {
        /* ���Old GUTI type */
        NAS_EMM_EncodeGutiType(             &(pucCnNasMsg[ulIndex]),
                                            &ulIeLength);
        ulIndex                            += ulIeLength;
    }
    *pulIndex += ulIndex;

    return;
}
VOS_VOID  NAS_EMM_EncodeOldGuti
(
    VOS_UINT8       *pMsg,
    VOS_UINT32      *pulIeLength
)
{
    VOS_UINT8                           *pucReqMsg;
    VOS_UINT32                          ulIndex = NAS_EMM_NULL;
    VOS_UINT32                          ulLen = NAS_EMM_NULL;

    if((VOS_NULL_PTR == pMsg) ||
        (VOS_NULL_PTR == pulIeLength))
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_EncodeOldGuti: Input para is invalid");
        return;
    }

    pucReqMsg                           = pMsg;
    *pulIeLength                        = NAS_EMM_NULL;

    if(NAS_EMM_SUCC == NAS_EMM_CheckMutiModeSupport())
    {
        NAS_EMM_TAU_LOG_NORM("NAS_EMM_EncodeOldGuti:SUPPORT GU MODE ");
        if (MMC_LMM_TIN_P_TMSI == NAS_EMM_GetTinType())
        {
            NAS_EMM_TAU_LOG_NORM("NAS_EMM_EncodeOldGuti:TIN IS P-TMSI ");
            if (NAS_EMM_SUCC == NAS_EMM_CheckPtmsiAndRaiValidity())
            {
                NAS_EMM_TAU_LOG_NORM("NAS_EMM_EncodeOldGuti:P-TMSI AND RAI VALID ");
                NAS_EMM_CompMappedGuti(&(pucReqMsg[ulIndex]),&ulLen);
                *pulIeLength                = ulLen;
            }
        }
        else if ((MMC_LMM_TIN_GUTI == NAS_EMM_GetTinType()) ||
            (MMC_LMM_TIN_RAT_RELATED_TMSI == NAS_EMM_GetTinType()))
        {
            NAS_EMM_TAU_LOG_NORM("NAS_EMM_EncodeOldGuti:TIN IS GUTI or RAT RELATED TMSI");
            if (NAS_EMM_BIT_SLCT == NAS_EMM_TAUSER_OP_GUTI())
            {
                NAS_EMM_TAU_LOG_NORM("NAS_EMM_EncodeOldGuti: NATIVE GUTI VALID");
                NAS_EMM_CompNativeGuti(&(pucReqMsg[ulIndex]),&ulLen);
                *pulIeLength            = ulLen;
            }
        }
        else
        {
            NAS_EMM_TAU_LOG_ERR("NAS_EMM_EncodeOldGuti: TIN INVALID");
        }
    }
    else
    {
        NAS_EMM_TAU_LOG_NORM("NAS_EMM_EncodeOldGuti:NOT SUPPORT GU MODE ");
        if (NAS_EMM_BIT_SLCT == NAS_EMM_TAUSER_OP_GUTI())
        {
            NAS_EMM_TAU_LOG_NORM("NAS_EMM_EncodeOldGuti:NATIVE GUTI INVALID ");
            NAS_EMM_CompNativeGuti(&(pucReqMsg[ulIndex]),&ulLen);
            *pulIeLength                = ulLen;
        }
    }

    return;
}




VOS_UINT32  NAS_EMM_IsConnChangeToLte( VOS_VOID )
{
    if((MMC_LMM_TIN_P_TMSI == NAS_EMM_GetTinType())
        || (MMC_LMM_TIN_RAT_RELATED_TMSI == NAS_EMM_GetTinType()))
    {   /* �մ�GU���� */

        if( (NAS_LMM_SYS_CHNG_ORI_LRRC == NAS_EMM_GetResumeOri()) &&
            (NAS_LMM_SYS_CHNG_TYPE_HO   == NAS_EMM_GetResumeType())
           )
        {   /* HO,���� */

            return  NAS_EMM_YES;
        }
    }

    return  NAS_EMM_NO;
}


/*****************************************************************************
 Function Name   : NAS_EMM_IsPtmsiAndIdle
 Description     : Ԥ����Ϊ��CCO���ض���ʱ������ڵ�䲻���ڰ�ȫ�����ĵĴ��ݣ�
                   ���CCO���ض�����IDLE����������
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.Hanlufeng 41410      2011-6-24  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_IsPtmsiAndIdle( VOS_VOID )
{
    if(MMC_LMM_TIN_P_TMSI == NAS_EMM_GetTinType())
    {   /* �մ�GU���� */

        if( (NAS_LMM_SYS_CHNG_ORI_LRRC == NAS_EMM_GetResumeOri()) &&
            (   (NAS_LMM_SYS_CHNG_TYPE_RSL  == NAS_EMM_GetResumeType()) ||
                (NAS_LMM_SYS_CHNG_TYPE_CCO  == NAS_EMM_GetResumeType()) ||
                (NAS_LMM_SYS_CHNG_TYPE_REDIR== NAS_EMM_GetResumeType())
            ))
        {   /* ��ѡ/CCO/�ض��� ���� */

            return  NAS_EMM_YES;
        }
        else if(NAS_LMM_SYS_CHNG_ORI_MMC == NAS_EMM_GetResumeOri())
        {   /* MMC���������� */

            return  NAS_EMM_YES;
        }
        else
        {

        }
    }

    return  NAS_EMM_NO;
}
VOS_VOID  NAS_EMM_EncodeNonCurrentNativeKSI
(
    VOS_UINT8       *pMsg,
    VOS_UINT32      *pulIeLength
)
{
    VOS_UINT8                          *pucReqMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulIndex = NAS_EMM_NULL;

    /* ��κϷ��Լ��*/
    if((VOS_NULL_PTR == pMsg) ||
        (VOS_NULL_PTR == pulIeLength))
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_EncodeNonCurrentNativeKSI: Input para is invalid");
        return;
    }

    pucReqMsg                           = pMsg;
   *pulIeLength                         = NAS_EMM_NULL;

    /* ��������� non-current native security context,��ֱ�ӷ��أ���������Ԫ*/
    if(NAS_LMM_NKSI_NO_KEY_IS_AVAILABLE == NAS_EMM_SecuGetNonCurKSIasme())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_EncodeNonCurrentNativeKSI: no para.");
        return;
    }

    /* ���Ǵ�GU����������CONNECT ̬�����Ԥ�н�������Ϊ�ض���Ҳ������̬.
    8.2.29.2    Non-current native NAS key set identifier
    The UE shall include this IE if the UE has a valid non-current native EPS
    security context when the UE performs an A/Gb mode or Iu mode to S1 mode
    inter-system change in EMM-CONNECTED mode and the UE uses a mapped EPS
    security context to protect the TRACKING AREA UPDATE REQUEST message.
    */
    if(NAS_EMM_YES == NAS_EMM_IsConnChangeToLte())
    {
        NAS_EMM_TAU_LOG2_INFO("NAS_EMM_EncodeNonCurrentNativeKSI: TinType = , ResumeType = ",
                                NAS_EMM_GetTinType(),
                                NAS_EMM_GetResumeType());

        /* ���Non-current native NAS key set identifier IEI*/
        pucReqMsg[ulIndex]              = NAS_EMM_TAU_REQ_NON_CURRENT_NATIVE_KSI;

        /* ��� Non-current native NAS key set identifier�� TSC */
        pucReqMsg[ulIndex]              |= NAS_EMM_SecuGetNonCurKSIasme();
        ulIndex++;

        /* ��Ԫ����*/
        *pulIeLength = ulIndex;
    }

    /* non-current native security context���ڣ�������Ԫ*/

    return;

}


VOS_VOID  NAS_EMM_EncodeGprsCkSn
(
    VOS_UINT8       *pMsg,
    VOS_UINT32      *pulIeLength
)
{
    VOS_UINT8                           *pucReqMsg;
    VOS_UINT32                          ulIndex = NAS_EMM_NULL;
    VOS_UINT8                           ucGprsCkSn = NAS_EMM_NULL;

    if((VOS_NULL_PTR == pMsg) ||
        (VOS_NULL_PTR == pulIeLength))
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_EncodeGprsCkSn: Input para is invalid");
        return;
    }

    pucReqMsg                           = pMsg;
    *pulIeLength                        = NAS_EMM_NULL;

    ucGprsCkSn = (VOS_UINT8)NAS_EMM_SecuGetUmtsKsi();

    /*
    8.2.29.3    GPRS ciphering key sequence number
    The UE shall include this IE if the UE performs an A/Gb mode or Iu mode
    to S1 mode inter-system change in EMM-IDLE mode and the TIN indicates "P-TMSI"
    */
    if (NAS_EMM_YES == NAS_EMM_IsPtmsiAndIdle() )
    {
        /*���GPRS ciphering key sequence number IEI*/
        pucReqMsg[ulIndex]              = NAS_EMM_TAU_REQ_GPRS_CK_SN;

        /* ��� GPRS ciphering key sequence number�� spare */
        pucReqMsg[ulIndex]              &= NAS_EMM_TAU_REQ_IE_SPARE;

        /* ���GPRS ciphering key sequence number��key sequence */
        pucReqMsg[ulIndex]              |= ucGprsCkSn;

        ulIndex ++;
        *pulIeLength                = ulIndex;

    }

    return;
}
VOS_VOID  NAS_EMM_EncodeNonceUE
(
    VOS_UINT8       *pMsg,
    VOS_UINT32      *pulIeLength
)
{

    VOS_UINT8                           *pucReqMsg;
    VOS_UINT32                          ulIndex = NAS_EMM_NULL;

    if((VOS_NULL_PTR == pMsg) ||
        (VOS_NULL_PTR == pulIeLength))
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_EncodeNonceUE: Input para is invalid");
        return;
    }

    pucReqMsg                           = pMsg;
    *pulIeLength                        = NAS_EMM_NULL;


    /*
    8.2.29.6    NonceUE
    This IE is included if the UE performs an A/Gb mode or Iu mode to S1 mode
    inter-system change in idle mode.
    */
    if (NAS_EMM_YES == NAS_EMM_IsPtmsiAndIdle() )
    {

        /*���NONCE UE IEI*/
        pucReqMsg[ulIndex++]                = NAS_EMM_TAU_REQ_Nonce_UE_IEI;

        /* ��� NONCE UE�� VALUE */
        NAS_LMM_MEM_CPY(                     &pucReqMsg[ulIndex],
                                            NAS_EMM_SecuGetNonceUE(),
                                            sizeof(VOS_UINT32));

        ulIndex                             += sizeof(VOS_UINT32);

        *pulIeLength                        = ulIndex;
    }

    return;

}


VOS_VOID  NAS_EMM_EncodeURCUpdateNeed
(
    VOS_UINT8       *pMsg,
    VOS_UINT32      *pulIeLength
)
{

    VOS_UINT8                           *pucReqMsg;
    VOS_UINT32                          ulIndex = NAS_EMM_NULL;

    if((VOS_NULL_PTR == pMsg) ||
        (VOS_NULL_PTR == pulIeLength))
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_EncodeURCUpdateNeed: Input para is invalid");
        return;
    }

    pucReqMsg                           = pMsg;
    *pulIeLength                        = NAS_EMM_NULL;


    /* �������TAUʱUE��GU���������ı��ʶ�Դ��ڣ���Ӧ���URC_UPDATE_NEED
       ����UE���������ϱ�������MME��UE�����������²�ѯ */
    if(NAS_LMM_UE_RADIO_CAP_GU_CHG == NAS_LMM_GetEmmInfoUeRadioCapChgFlag())
    {
        NAS_LMM_SetEmmInfoFirstTauFlag(NAS_EMM_FIRST_TAU);
    }


    /*if (MMC_LMM_TIN_P_TMSI == NAS_EMM_GetTinType())*/
    if (NAS_EMM_FIRST_TAU == NAS_LMM_GetEmmInfoFirstTauFlag())
    {
        /*���UE radio capability information update needed IEI*/
        pucReqMsg[ulIndex]            = NAS_EMM_TAU_REQ_RADIO_CAP_UPDATE_NEED_IEI;

        /* ��� UE radio capability information update needed IEI��spare*/
        pucReqMsg[ulIndex]              &= NAS_EMM_TAU_REQ_IE_SPARE;

        /* ���UE radio capability information update needed IEI��update needed */
        pucReqMsg[ulIndex]              |= NAS_EMM_URC_UPDATE_NEED;

        ulIndex ++;

        *pulIeLength                        = ulIndex;

    }

    return;
}


/*leili modify for isr begin*/

VOS_VOID  NAS_EMM_EncodeEpsBearStatus
(
    VOS_UINT8       *pMsg,
    VOS_UINT32      *pulIeLength
)
{
    VOS_UINT8                           *pucReqMsg;
    VOS_UINT32                          ulIndex = NAS_EMM_NULL;
    VOS_UINT32                          i;
    NAS_MML_PS_BEARER_CONTEXT_STRU      *pstEpsBearerCxt;
    VOS_UINT32                          ucEpsId;
    VOS_UINT8                           ucEsmEPS1           = 0;
    VOS_UINT8                           ucEsmEPS2           = 0;

    if((VOS_NULL_PTR == pMsg) ||
        (VOS_NULL_PTR == pulIeLength))
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_EncodeEpsBearStatus: Input para is invalid");
        return;
    }

    pucReqMsg                           = pMsg;
    *pulIeLength                        = NAS_EMM_NULL;
    /*PC REPLAY MODIFY BY LEILI BEGIN*/
    pstEpsBearerCxt = NAS_EMM_GetPsBearerCtx();
    /*PC REPLAY MODIFY BY LEILI END*/
    pucReqMsg[ulIndex++]          = NAS_EMM_TAU_REQ_EPS_STATUS_IEI;
    pucReqMsg[ulIndex++]          = NAS_EMM_CN_EPS_CONTEXT_LEN;
    for(i = 0; i < EMM_ESM_MAX_EPS_BEARER_NUM; i++)
    {
        if (NAS_MML_PS_BEARER_STATE_ACTIVE == pstEpsBearerCxt[i].enPsBearerState)
        {
            ucEpsId = i + NAS_EMM_MIN_EPS_ID;
            if( NAS_EMM_EPS_BEARER_STATUS_NUM_SEVEN < ucEpsId)
            {
                ucEpsId             = ucEpsId - NAS_EMM_EPS_BEARER_STATUS_NUM_EIGNT;
                ucEsmEPS2 = ucEsmEPS2 | NAS_EMM_TAU_ESM_BEARER_LEFT_N(ucEpsId);
            }
            else
            {
                ucEsmEPS1 = ucEsmEPS1 | NAS_EMM_TAU_ESM_BEARER_LEFT_N(ucEpsId);
            }

        }
    }
    pucReqMsg[ulIndex++]                = ucEsmEPS1;
    pucReqMsg[ulIndex++]                = ucEsmEPS2;
    *pulIeLength                        = ulIndex;


    return;
}
/*leili modify for isr end*/

VOS_VOID    NAS_EMM_TAU_SendMrrcDataReq_TAUReq(NAS_EMM_MSG_TYPE_ENUM_UINT32 ulEmmMsgType)
{
    NAS_EMM_MRRC_DATA_REQ_STRU          *pMrrcDataReqMsg    = NAS_EMM_NULL_PTR;
    VOS_UINT32                          uldataReqMsgLenNoHeader;
    VOS_UINT32                          ulIndex             =0;


    /* ��ӡ����ú����� INFO_LEVEL */
    if(NAS_EMM_MSG_LOAD_BALANCING_TAU == ulEmmMsgType)
    {
        NAS_EMM_TAU_LOG_NORM( "NAS_EMM_TAU_SendMrrcDataReq_TAUReq is entered,TAUType:NAS_EMM_MSG_LOAD_BALANCING_TAU");
    }
    else
    {
        NAS_EMM_TAU_LOG_NORM( "NAS_EMM_TAU_SendMrrcDataReq_TAUReq is entered,TAUType:NAS_EMM_MSG_NOT_LOAD_BALANCING_TAU");
    }

    /* ����NAS_MSG_STRU��Ϣ���� */


    /* �������Ϣ��������DOPRA��Ϣ */
    pMrrcDataReqMsg = (VOS_VOID *)NAS_LMM_MEM_ALLOC( NAS_EMM_INTRA_MSG_MAX_SIZE);
    if(NAS_EMM_NULL_PTR == pMrrcDataReqMsg)
    {
        NAS_EMM_TAU_LOG_ERR( "NAS_EMM_TAU_SendMrrcDataReq_TAUReq: MSG ALLOC ERR !!");
        return;
    }
	/* lihong00150010 emergency tau&service begin */
    /* �����������Pending���ߴ���COLLISION��־����Я��'ACTIVE'��־*/
    if((NAS_EMM_UPLINK_PENDING == NAS_EMM_TAU_GetEmmUplinkPending())
     || (NAS_EMM_COLLISION_NONE != NAS_EMM_TAU_GetEmmCollisionCtrl())
     || (NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause()))
    {/* lihong00150010 emergency tau&service end */
        NAS_EMM_TAU_SaveEmmTauReqActiveCtrl(NAS_EMM_TAU_WITH_BEARER_EST_REQ);
    }
    else
    {
        NAS_EMM_TAU_SaveEmmTauReqActiveCtrl(NAS_EMM_TAU_NO_BEARER_EST_REQ);
    }

    /* ����MRRC_DATA_REQ �е� NAS_MSG_STRU,��CN��Ϣ(Compose the msg of): TAU_REQ */
    NAS_EMM_TAU_CompCnTAUReqNasMsg(pMrrcDataReqMsg->stNasMsg.aucNasMsg,&ulIndex);

    NAS_EMM_TAU_SaveEmmTauImsiDetachFlag(NAS_EMM_IMSI_DETACH_INVALID);

    uldataReqMsgLenNoHeader              = EMM_COUNT_DATA_REQ_LEN(ulIndex);

    if ( NAS_EMM_INTRA_MSG_MAX_SIZE < uldataReqMsgLenNoHeader )
    {
        /* ��ӡ������Ϣ */
        NAS_LMM_PUBM_LOG_ERR("NAS_EMM_TAU_CompCnTAUReqNasMsg, Size error ");

        NAS_LMM_MEM_FREE(pMrrcDataReqMsg);
        return ;
    }

    /* ��дMRRC_DATA_REQ ��DOPRA��Ϣͷ */
    EMM_COMP_MM_MSG_HEADER(pMrrcDataReqMsg, uldataReqMsgLenNoHeader);

    /* ��дMRRC_DATA_REQ ����ϢID��ʶ */
    pMrrcDataReqMsg->ulMsgId                 = ID_NAS_LMM_INTRA_MRRC_DATA_REQ;

    /*��� TAU_REQ_EST_CAUSE*/
    /* lihong00150010 emergency tau&service begin */
    /*��� TAU_REQ_EST_CAUSE*/
    if ((NAS_LMM_REG_STATUS_EMC_REGED == NAS_LMM_GetEmmInfoRegStatus())
        || (NAS_LMM_REG_STATUS_NORM_REGED_AND_EMC_BEAR == NAS_LMM_GetEmmInfoRegStatus())
        || (NAS_EMM_TAU_START_CAUSE_ESM_EMC_PDN_REQ == NAS_EMM_TAU_GetEmmTAUStartCause()))
    {
        pMrrcDataReqMsg->enEstCaue = LRRC_LNAS_EST_CAUSE_EMERGENCY_CALL;
        pMrrcDataReqMsg->enCallType= LRRC_LNAS_CALL_TYPE_EMERGENCY_CALL;
    }
    else
    {
        pMrrcDataReqMsg->enEstCaue = LRRC_LNAS_EST_CAUSE_MO_SIGNALLING;
        pMrrcDataReqMsg->enCallType= LRRC_LNAS_CALL_TYPE_ORIGINATING_SIGNALLING;
    }
    /* lihong00150010 emergency tau&service end */
    pMrrcDataReqMsg->enEmmMsgType = ulEmmMsgType;

    pMrrcDataReqMsg->enDataCnf               = LRRC_LMM_DATA_CNF_NEED;

    /* ��дMRRC_DATA_REQ ��NAS��Ϣ���ȱ�ʶ */
    pMrrcDataReqMsg->stNasMsg.ulNasMsgSize   = ulIndex;

    /*����TAU����ʱ������״̬*/
    NAS_EMM_TAU_SaveEmmTauStartConnSta(NAS_EMM_GetConnState());

    /* ���������TAU����֪ͨMM����MM LOCATION UPDATING PENDING״̬ */
    if ((NAS_EMM_CN_TAU_TYPE_COMBINED_TA_LA_UPDATING == NAS_EMM_TAU_GetTAUtype())
        || (NAS_EMM_CN_TAU_TYPE_COMBINED_TA_LA_WITH_IMSI == NAS_EMM_TAU_GetTAUtype()))
    {
        NAS_EMM_SendMmCombinedStartNotifyReq(MM_LMM_COMBINED_TAU);
    }

    /*NAS_EMM_TAU_SaveEmmT3412ExpCtrl(NAS_EMM_NO);*/

    /*�տ���Ϣ�ϱ�TAU REQ*/
    NAS_LMM_SendOmtAirMsg(NAS_EMM_OMT_AIR_MSG_UP, NAS_EMM_TAU_REQ,(NAS_MSG_STRU *)&(pMrrcDataReqMsg->stNasMsg));

    /*�ؼ��¼��ϱ�TAU REQ*/
    NAS_LMM_SendOmtKeyEvent(         EMM_OMT_KE_TAU_REQ);

    /* ������Ϣ(Send the msg of) MRRC_DATA_REQ(TAU_REQ) */
    NAS_EMM_SndUplinkNasMsg(             pMrrcDataReqMsg);

    NAS_LMM_MEM_FREE(pMrrcDataReqMsg);

    return;

}
VOS_VOID    NAS_EMM_TAU_SendEsmBearerStatusInd(const NAS_EMM_CN_TAU_ACP_STRU   *pMsgStru )
{
    EMM_ESM_BEARER_STATUS_IND_STRU       *psmbearerstatusind  =  NAS_EMM_NULL_PTR;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO( "NAS_EMM_TAU_SendEsmBearerStatusInd is entered.");

    /* ����DOPRA��Ϣ */
    psmbearerstatusind = (VOS_VOID *)NAS_LMM_ALLOC_MSG(sizeof(EMM_ESM_BEARER_STATUS_IND_STRU));

    if(NAS_EMM_NULL_PTR == psmbearerstatusind)
    {
        NAS_EMM_TAU_LOG_ERR( "NAS_EMM_TAU_SendEsmBearerStatusInd: MSG ALLOC ERR !!");
        return;
    }

    /* ��дDOPRA��Ϣͷ */
    EMM_COMP_ESM_MSG_HEADER(psmbearerstatusind, sizeof(EMM_ESM_BEARER_STATUS_IND_STRU) -
                                        EMM_LEN_VOS_MSG_HEADER);

    /* ��д��ϢID��ʶ */
    psmbearerstatusind->ulMsgId    = ID_EMM_ESM_BEARER_STATUS_IND;

    /* ��д��ǰ�Ѽ����EPSID����  */
    psmbearerstatusind->ulEpsIdNum = pMsgStru->stEPSBearerstatus.ulEpsIdNum;

    /* ��д �����Ѽ����EPS BID  */
    NAS_LMM_MEM_CPY(                        &(psmbearerstatusind->aulEsmEpsId[0]),
                                        &(pMsgStru->stEPSBearerstatus.aulEsmEpsId[0]),
                                        EMM_ESM_MAX_EPS_BEARER_NUM*sizeof(VOS_UINT32));


    /* ������Ϣ*/
    NAS_LMM_SEND_MSG(psmbearerstatusind);

    return;


}
VOS_VOID    NAS_EMM_TAU_SendDetachReq()
{
    NAS_LMM_INTRA_DETACH_REQ_STRU             *pIntraDetachReqMsg = NAS_EMM_NULL_PTR;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_SendDetachReq is entered.");

    /* ����DOPRA��Ϣ */
    pIntraDetachReqMsg = (VOS_VOID *)NAS_EMM_TAUSER_GetSendIntraMsgBuffAddr(
                                        sizeof(NAS_LMM_INTRA_DETACH_REQ_STRU) -
                                        EMM_LEN_VOS_MSG_HEADER);

    if(NAS_EMM_NULL_PTR == pIntraDetachReqMsg)
    {
        NAS_EMM_TAU_LOG_ERR( "Nas_Emm_Sau_SendAttachReq: MSG ALLOC ERR !!");
        return;
    }

    /* ��дNAS_EMM_INTRA_DETACH_REQ ��DOPRA��Ϣͷ */
    EMM_COMP_MM_MSG_HEADER(pIntraDetachReqMsg, sizeof(NAS_LMM_INTRA_DETACH_REQ_STRU) -
                                        EMM_LEN_VOS_MSG_HEADER);

    /* ��дNAS_EMM_INTRA_DETACH_REQ ����ϢID��ʶ */
    pIntraDetachReqMsg->ulMsgId                 = ID_NAS_LMM_INTRA_DETACH_REQ;

    /* ������Ϣ(Send the msg of) NAS_EMM_INTRA_DETACH_REQ */
/*    NAS_EMM_TAUSER_SendIntraMsg(pIntraDetachReqMsg);*/
    NAS_EMM_SEND_INTRA_MSG(             pIntraDetachReqMsg);

    return;
}




VOS_VOID    NAS_EMM_TAU_CompCnTAUCmpl(LRRC_LNAS_MSG_STRU  *pNasMsg,
                                                        VOS_UINT32 *pulIndex )
{
    VOS_UINT32                          ulIndex             = 0;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO( "NAS_EMM_TAU_CompCnTAUCmpl is entered.");

    /* ��ս����CN��Ϣ�Ŀռ� */
    /* xiongxianghui00253310 delete memset 2013-11-26 begin */
    /* NAS_LMM_MEM_SET(pNasMsg->aucNasMsg, 0, pNasMsg->ulNasMsgSize); */
    /* xiongxianghui00253310 delete memset 2013-11-26 end   */

    /* ��� Skip indicator  +  Protocol Discriminator */
    pNasMsg->aucNasMsg[ulIndex++]       = EMM_CN_MSG_PD_EMM;

    /* ��� TAU CMPL message identity   */
    pNasMsg->aucNasMsg[ulIndex++]       = NAS_EMM_CN_MT_TAU_CMP;

    *pulIndex = ulIndex;

    return;

}
VOS_VOID    NAS_EMM_TAU_SendMrrcDataReqTauCmpl( )
{
    NAS_EMM_MRRC_DATA_REQ_STRU          *pMrrcDataReqMsg    = NAS_EMM_NULL_PTR;
    VOS_UINT32                          uldataReqMsgLenNoHeader;
    VOS_UINT32                          ulIndex             = 0;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_TAU_SendMrrcDataReqTauCmpl is entered.");


    /* ������Ϣ���� */
    pMrrcDataReqMsg = (VOS_VOID *)NAS_LMM_MEM_ALLOC( NAS_EMM_INTRA_MSG_MAX_SIZE);
    if(NAS_EMM_NULL_PTR == pMrrcDataReqMsg)
    {
        NAS_EMM_TAU_LOG_ERR("NAS_EMM_TAU_SendMrrcDataReqTauCmpl: MSG ALLOC ERR !!");
        return;
    }

    /* xiongxianghui00253310 add memset 2013-11-26 begin */

    NAS_LMM_MEM_SET(pMrrcDataReqMsg, 0, NAS_EMM_INTRA_MSG_MAX_SIZE);
    /* xiongxianghui00253310 add memset 2013-11-26 end   */

    /* ����MRRC_DATA_REQ �е� NAS_MSG_STRU,��CN��Ϣ(Compose the msg of): TAU_REQ */
    NAS_EMM_TAU_CompCnTAUCmpl(            &(pMrrcDataReqMsg->stNasMsg),&ulIndex);

    uldataReqMsgLenNoHeader              = EMM_COUNT_DATA_REQ_LEN(ulIndex);

    if ( NAS_EMM_INTRA_MSG_MAX_SIZE < uldataReqMsgLenNoHeader )
    {
        /* ��ӡ������Ϣ */
        NAS_LMM_PUBM_LOG_ERR("NAS_EMM_TAU_CompCnTAUCmpl, Size error ");
        NAS_LMM_MEM_FREE(pMrrcDataReqMsg);
        return ;
    }

    /* ��дMRRC_DATA_REQ ��DOPRA��Ϣͷ */
    EMM_COMP_MM_MSG_HEADER(pMrrcDataReqMsg, uldataReqMsgLenNoHeader);

    /* ��дMRRC_DATA_REQ ����ϢID��ʶ */
    pMrrcDataReqMsg->ulMsgId            = ID_NAS_LMM_INTRA_MRRC_DATA_REQ;

    /*��� TAU_COMPL_EST_CAUSE*/
    pMrrcDataReqMsg->enEstCaue          = LRRC_LNAS_EST_CAUSE_MO_SIGNALLING;
    pMrrcDataReqMsg->enCallType         = LRRC_LNAS_CALL_TYPE_ORIGINATING_SIGNALLING;
    pMrrcDataReqMsg->enEmmMsgType       = NAS_EMM_MSG_TAU_CMP;

    pMrrcDataReqMsg->enDataCnf          = LRRC_LMM_DATA_CNF_NEED;

    /* ��дMRRC_DATA_REQ ��NAS��Ϣ���ȱ�ʶ */
    pMrrcDataReqMsg->stNasMsg.ulNasMsgSize   = ulIndex;

    /*�տ���Ϣ�ϱ�TAU COMPLETE*/
    NAS_LMM_SendOmtAirMsg(NAS_EMM_OMT_AIR_MSG_UP, NAS_EMM_TAU_CMP, (NAS_MSG_STRU *)&(pMrrcDataReqMsg->stNasMsg));

    /*�ؼ��¼��ϱ�TAU COMPLETE*/
    NAS_LMM_SendOmtKeyEvent(         EMM_OMT_KE_TAU_CMP);

    NAS_EMM_SndUplinkNasMsg(             pMrrcDataReqMsg);

    NAS_LMM_MEM_FREE(pMrrcDataReqMsg);
    return;

}




VOS_UINT32    NAS_EMM_TAU_CHKFSMStateMsgp(
                                    NAS_EMM_MAIN_STATE_ENUM_UINT16  stMS,
                                    NAS_EMM_SUB_STATE_ENUM_UINT16   stSS,
                                    VOS_VOID   *pMsgStru)
{
    (VOS_VOID)(pMsgStru);
    if(NAS_EMM_CHK_STAT_INVALID(stMS, stSS))
    {
        NAS_EMM_TAU_LOG_WARN( "NAS_EMM_TAUSER_CHKFSMState: STATE ERR !!");
        return NAS_EMM_FAIL;
    }
    if ( NAS_EMM_NULL_PTR == pMsgStru )
    {
        /* ��ӡ�쳣 */
        NAS_EMM_TAU_LOG_WARN( "NAS_EMM_TAUSER_CHKMsgp: pMsgStru is NULL_PTR !!");
        return NAS_EMM_FAIL;
    }
    return NAS_EMM_SUCC;
}
NAS_EMM_TAU_START_CAUSE_ENUM_UINT8  NAS_EMM_TAU_GetTauStartCause(
                                        VOS_VOID)
{
    return NAS_EMM_TAU_GetEmmTAUStartCause();
}


NAS_EMM_IMSI_DETACH_FLAG_ENUM_UINT8  NAS_EMM_TAU_GetImsiDetachFlag( VOS_VOID )
{
    return NAS_EMM_TAU_GetEmmTauImsiDetachFlag();
}


VOS_VOID NAS_EMM_TAU_SetImsiDetachFlag
(
    NAS_EMM_IMSI_DETACH_FLAG_ENUM_UINT8 enImsiDetachFlag
)
{
    NAS_EMM_TAU_SaveEmmTauImsiDetachFlag(enImsiDetachFlag);
}


VOS_VOID  NAS_EMM_TAU_IsUplinkPending( VOS_VOID )
{
   if((EMM_ESM_UP_SINGNAL_PENDING == NAS_ESM_IsUpSingnalPending())
    ||(EMM_ERABM_UP_DATA_PENDING == NAS_ERABM_IsDataPending()))
   {
        NAS_EMM_TAU_SetEmmUplinkPending(NAS_EMM_UPLINK_PENDING);
   }
   else
   {
        NAS_EMM_TAU_SetEmmUplinkPending(NAS_EMM_NO_UPLINK_PENDING);
   }

   return;
}
/*****************************************************************************
 Function Name   : NAS_EMM_TAU_CanTriggerComTauWithIMSI
 Description     :
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.FTY               2012-02-25  Draft Enact
    2.lihong00150010    2012-12-19  Modify:Emergency

*****************************************************************************/
VOS_UINT32 NAS_EMM_TAU_CanTriggerComTauWithIMSI(VOS_VOID)
{/* lihong00150010 emergency tau&service begin */
    if((NAS_LMM_REG_DOMAIN_PS == NAS_LMM_GetEmmInfoRegDomain())
        && ( NAS_EMM_CN_TAU_TYPE_COMBINED_TA_LA_WITH_IMSI == NAS_EMM_TAU_GetEpsUpdateType(NAS_EMM_TAU_START_CAUSE_OTHERS))
        && (NAS_LMM_REG_STATUS_EMC_REGED != NAS_LMM_GetEmmInfoRegStatus()))
    {/* lihong00150010 emergency tau&service end */
        return NAS_EMM_YES;
    }

    return NAS_EMM_NO;
}
/* lihong00150010 emergency tau&service begin */
#if 0
/*****************************************************************************
 Function Name   : NAS_EMM_TimerRunningStateChng
 Description     :
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.FTY         2012-02-25  Draft Enact

*****************************************************************************/
VOS_VOID    NAS_EMM_TAU_TimerRunningStateChng( NAS_LMM_STATE_TI_ENUM_UINT16 enStateTimerId )
{
    if ((EMM_US_UPDATED_EU1 == NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat))
    {
        /* �ж��ܷ�������TAU */
        if(NAS_EMM_YES == NAS_EMM_TAU_CanTriggerComTauWithIMSI())
        {

            /* ��״̬ת����MS_REG + EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM״̬ */
            NAS_EMM_TAUSER_FSMTranState(   EMM_MS_REG,
                                           EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM,
                                           enStateTimerId);
         }
         else
         {
             /* ��״̬ת����MS_REG + SS_REG_NORMAL_SERVICE״̬ */
             NAS_EMM_TAUSER_FSMTranState(   EMM_MS_REG,
                                            EMM_SS_REG_NORMAL_SERVICE,
                                            enStateTimerId);

         }
    }
    else
    {
        /* ��״̬ת����MS_REG + EMM_SS_REG_ATTEMPTING_TO_UPDATE״̬ */
        NAS_EMM_TAUSER_FSMTranState(   EMM_MS_REG,
                                       EMM_SS_REG_ATTEMPTING_TO_UPDATE,
                                       enStateTimerId);

    }
}
#endif
/* lihong00150010 emergency tau&service end */

VOS_UINT32  NAS_EMM_TAU_RrcRelCauseTriggerTau(VOS_VOID)
{
    NAS_LMM_NETWORK_INFO_STRU           *pMmNetInfo = NAS_EMM_NULL_PTR;

    if(NAS_EMM_TRIGGER_TAU_RRC_REL_LOAD_BALANCE == NAS_LMM_GetEmmInfoTriggerTauRrcRel())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_RrcRelCauseTriggerTau:load balance");
        /*NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_RRC_REL_LOAD_BALANCE);*/
        NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
        NAS_EMM_TAU_StartTAUREQ();
        return NAS_EMM_YES;
    }
    else if(NAS_EMM_TRIGGER_TAU_RRC_REL_CONN_FAILURE ==
                                        NAS_LMM_GetEmmInfoTriggerTauRrcRel())
    {
         /* ����ɾ�������������PENDING,��ᷢ��SERVICE */
         NAS_EMM_TAU_IsUplinkPending();

         if(NAS_EMM_UPLINK_PENDING == NAS_EMM_TAU_GetEmmUplinkPending())
         {
             /*������������pendingʱ,����SR֮ǰ��Ҫ��MMC�ϱ�TAU�����ʹMMC֪ͨCDS�л���LTEģʽ */
             pMmNetInfo                          = NAS_LMM_GetEmmInfoNetInfoAddr();
             NAS_EMM_SetLVRTai(&pMmNetInfo->stPresentNetId);
             NAS_EMM_SetTauTypeNoProcedure();
             NAS_EMM_MmcSendTauActionResultIndSuccWithoutPara();
             NAS_EMM_TAUSER_SaveAuxFsmUpStat(EMM_US_UPDATED_EU1);
             /* lihong00150010 emergency tau&service begin */
             if (NAS_LMM_REG_STATUS_EMC_REGED != NAS_LMM_GetEmmInfoRegStatus())
             {
                 /*����PS LOC��Ϣ*/
                 NAS_LMM_WritePsLoc(NAS_NV_ITEM_UPDATE);
             }
             /* lihong00150010 emergency tau&service end */
             NAS_EMM_TAU_LOG_INFO("NAS_EMM_RrcRelCauseTriggerTau:Init Ser.");
             NAS_EMM_SER_UplinkPending();
         }
         else
         {
             NAS_EMM_TAU_LOG_INFO("NAS_EMM_RrcRelCauseTriggerTau:RRC connection failure");
             /*NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_RRC_REL_CONN_FAILURE );*/
             NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
             NAS_EMM_TAU_StartTAUREQ();
         }

         return NAS_EMM_YES;
    }
    else
    {

    }

    return NAS_EMM_NO;
}
/*****************************************************************************
 Function Name   : NAS_EMM_RegSomeStateMsgSysInfoT3412ExpProc
 Description     : Reg.Limite_Service
                   Reg.Plmn_Search
                   Reg.No_Cell_Available
                   ״̬���յ�SYS_INFO_INDʱ����T3412��ʱ����ʱ
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010  2011-08-31  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_RegSomeStateMsgSysInfoT3412ExpProc( VOS_VOID )
{
    /* Э��24301 5.3.5�½���������REG+NO_AVAILABLE_CELL״̬�£�T3412��ʱ�����
       ISR���������T3423��ʱ��;���ISRû�м�����
       ��ǰ������ע��ɹ�����ص�REG+NORMAL_SERVICE̬�󣬷�������TAU��TAU����Ϊ
       combined TA/LA updating with IMSI attach�������������ע��ɹ�������������TAU
   */
    if (NAS_EMM_T3412_EXP_YES_REG_NO_AVALABLE_CELL ==  NAS_LMM_GetEmmInfoT3412ExpCtrl())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_RegSomeStateMsgSysInfoT3412ExpProc:REG+NO_AVAILABE_CELL T3412exp.");

        if (NAS_LMM_REG_DOMAIN_CS_PS == NAS_LMM_GetEmmInfoRegDomain())
        {
             /* ��ע�����ΪPS����Ϊ�˻ص�REG+NORMAL_SERVICE̬�����Ҫ��������TAU����
                TAU����Ӧ����Ϊcombined TA/LA updating with IMSI attach */
            NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_PS);
             /*NAS_EMM_TAU_SaveEmmTAUStartCause(     NAS_EMM_TAU_START_CAUSE_SYSINFO);*/
             NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
        }
        else
        {
            NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_T3412EXP);
        }

        NAS_EMM_TAU_StartTAUREQ();
        return NAS_LMM_MSG_HANDLED;
    }

    /* Э��24301 5.3.5�½�����������ڷ�REG+NO_AVAILABLE_CELL״̬�£�T3412��ʱ��
       �򵱻ص�REG+NORMAL_SERVICE̬��Ӧ���� Periodic TAU*/
    if(NAS_EMM_T3412_EXP_YES_OTHER_STATE ==  NAS_LMM_GetEmmInfoT3412ExpCtrl())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_RegSomeStateMsgSysInfoT3412ExpProc:Other State T3412exp.");
        NAS_EMM_TAU_SaveEmmTAUStartCause( NAS_EMM_TAU_START_CAUSE_T3412EXP);
        NAS_EMM_TAU_StartTAUREQ();

        return NAS_LMM_MSG_HANDLED;
    }

    return NAS_LMM_MSG_DISCARD;
}




VOS_UINT32  NAS_EMM_RegSomeStateStartTAUNeeded( VOS_VOID )
{
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_RegSomeStateStartTAUNeeded is entered.");

    /* ����ڼ䷢����ESM�����޸ĳ���������*/
    if(NAS_EMM_EPS_BEARER_STATUS_CHANGEED == NAS_EMM_GetEpsContextStatusChange())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_RegSomeStateStartTAUNeeded:Bearer Status Change.");
        NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
        NAS_EMM_TAU_StartTAUREQ();
        return NAS_EMM_YES;
    }

    /* ����ڼ䷢����UE_NET_CAP or DRX�ı䣬����TA-Updating TAU*/
    if(NAS_EMM_YES == NAS_LMM_GetEmmInfoDrxNetCapChange())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_RegSomeStateStartTAUNeeded:DrxOrNetCap Change.");
        NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
        NAS_EMM_TAU_StartTAUREQ();
        return NAS_EMM_YES;
    }

    /*����ϵͳ��ʶ�����ж�*/
    if(NAS_EMM_YES == NAS_LMM_GetEmmInfoTriggerTauSysChange() )
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_RegSomeStateStartTAUNeeded: Sys Change flag");
        NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
        NAS_EMM_TAU_StartTAUREQ();
        return NAS_EMM_YES;
    }

    /* �����ǰע����ΪPS���ҵ�ǰ����������CS���ע�ᣬ��������TAU */
    if (NAS_EMM_YES == NAS_EMM_TAU_CanTriggerComTauWithIMSI())
    {
        NAS_EMM_TAU_LOG_NORM("NAS_EMM_RegSomeStateStartTAUNeeded: with imsi attach");
        NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
        NAS_EMM_TAU_StartTAUREQ();
        return NAS_EMM_YES;
    }

    /* �жϽ��뼼���仯��ʶ������仯 */
    if (NAS_LMM_UE_RADIO_CAP_NOT_CHG != NAS_LMM_GetEmmInfoUeRadioCapChgFlag())
    {
        NAS_EMM_TAU_LOG_NORM("NAS_EMM_RegSomeStateStartTAUNeeded: UE radio capability is changed !");
        NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
        NAS_EMM_TAU_StartTAUREQ();
        return NAS_EMM_YES;
    }

    /*��RRC Relԭ������ж�*/
    if(NAS_EMM_YES == NAS_EMM_TAU_RrcRelCauseTriggerTau())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_RegSomeStateStartTAUNeeded:rrc rel cause");
        return NAS_EMM_YES;
    }

    /* e)when the UE enters EMM-REGISTERED.NORMAL-SERVICE and the UE's TIN indicates "P-TMSI"
       ��Ҫ������Lģ�¼���ISR��������Lģ��3412��3423��ʱ������ʱ�ĳ������˳������ܷ���������TAU */
    if (MMC_LMM_TIN_P_TMSI == NAS_EMM_GetTinType())
    {
        NAS_EMM_TAU_LOG_NORM("NAS_EMM_RegSomeStateStartTAUNeeded: TIN is P-TMSI !");
        NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
        NAS_EMM_TAU_StartTAUREQ();
        return NAS_EMM_YES;
    }

    /* ����ʱ��T3412��ʱ */
    if (NAS_LMM_MSG_HANDLED == NAS_EMM_RegSomeStateMsgSysInfoT3412ExpProc())
    {
        return NAS_EMM_YES;
    }

    return NAS_EMM_NO;

}

/* lihong00150010 emergency tau&service begin */

VOS_VOID  NAS_EMM_RegSomeStateMsgSysInfoTaiNotInTaiList( VOS_VOID )
{
    MMC_LMM_TAU_RSLT_ENUM_UINT32        ulTauRslt = MMC_LMM_TAU_RSLT_BUTT;
    NAS_MM_TA_STRU                     *pstLastAttmpRegTa = NAS_EMM_NULL_PTR;
    NAS_MM_TA_STRU                      stCurTa           = {0};
    NAS_LMM_PTL_TI_ENUM_UINT16          enPtlTimerId     = NAS_LMM_PTL_TI_BUTT;

    NAS_EMM_TAU_LOG_NORM("NAS_EMM_RegSomeStateMsgSysInfoTaiNotInTaiList is entered!");

    /* ��ȡ��ǰTA���ϴγ���ע���TA��Ϣ */
    NAS_EMM_GetCurrentTa(&stCurTa);
    pstLastAttmpRegTa                  = NAS_LMM_GetEmmInfoNetInfoLastAttmpRegTaAddr();

    /* TAû��&&�ж�ʱ��&&EUֵ����EU1���ȳ�ʱ; ������������TAU*/
    if( (NAS_LMM_MATCH_SUCCESS == NAS_LMM_TaMatch(pstLastAttmpRegTa, &stCurTa))
        && (NAS_EMM_YES == NAS_EMM_IsT3411orT3402Running(&enPtlTimerId))
        && (EMM_US_UPDATED_EU1 != NAS_EMM_TAUSER_GetAuxFsmUpStat()))
    {
        NAS_EMM_TAU_LOG_NORM("NAS_EMM_RegSomeStateMsgSysInfoTaiNotInTaiList: wait timer expired.");
        NAS_EMM_TAUSER_FSMTranState( EMM_MS_REG,
                                     EMM_SS_REG_ATTEMPTING_TO_UPDATE,
                                     TI_NAS_EMM_STATE_NO_TIMER);

        /* �ж�ʱ��3402������, ���ϱ�TAU��� */
        if ( TI_NAS_EMM_PTL_T3402 == enPtlTimerId )
        {
            NAS_EMM_SetTauTypeNoProcedure();

            ulTauRslt = MMC_LMM_TAU_RSLT_T3402_RUNNING;
            NAS_EMM_MmcSendTauActionResultIndOthertype((VOS_VOID*)&ulTauRslt);


        }
        return;
    }

    /* ����TAU����������TAU Procedure */
    NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);
    NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
    NAS_EMM_TAU_StartTAUREQ();
    return;
}
/* lihong00150010 emergency tau&service end */

VOS_VOID  NAS_EMM_RegSomeStateMsgSysInfoCommProc( VOS_VOID )
{
    MMC_LMM_TAU_RSLT_ENUM_UINT32        ulTauRslt = MMC_LMM_TAU_RSLT_BUTT;
    NAS_MM_NETWORK_ID_STRU             *pstPresentNetInfo = NAS_EMM_NULL_PTR;
    NAS_LMM_PTL_TI_ENUM_UINT16          enPtlTimerId     = NAS_LMM_PTL_TI_BUTT;

    /* ��ȡ��ǰTA���ϴγ���ע���TA��Ϣ */
    pstPresentNetInfo                  = NAS_LMM_GetEmmInfoNetInfoPresentNetAddr();

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_RegSomeStateMsgSysInfoCommProc is entered.");

    /* �����ǰTA����TAI LIST */
    if (NAS_EMM_FAIL == NAS_EMM_TAU_IsCurrentTAInTaList())
    {
        NAS_EMM_RegSomeStateMsgSysInfoTaiNotInTaiList();

        return ;
    }

    /* TA��TAI LIST && EUֵ��ΪEU1 */
    if (EMM_US_UPDATED_EU1 != NAS_EMM_TAUSER_GetAuxFsmUpStat())
    {
        NAS_EMM_TAU_LOG_NORM("NAS_EMM_RegSomeStateMsgSysInfoCommProc: TA is in TAI LIST, not EU1.");

        /* �ϴγ��Է���ע���TAU��TA����TAI LIST�����ü�������TAU */
        if (NAS_EMM_NO == NAS_LMM_IsLastAttemptRegTaInTaiList())
        {
            NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);
            NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
            NAS_EMM_TAU_StartTAUREQ();
            return;
        }

        /*�ж�ʱ�� && ��ǰTA���ϴγ��Է���ע���TA��Ч���ȳ�ʱ*/
        if (NAS_EMM_YES == NAS_EMM_IsT3411orT3402Running(&enPtlTimerId))
        {
                NAS_EMM_TAU_LOG_NORM("NAS_EMM_RegSomeStateMsgSysInfoCommProc: Equivalent TA, wait timer expired.");
                NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                            EMM_SS_REG_ATTEMPTING_TO_UPDATE,
                                            TI_NAS_EMM_STATE_NO_TIMER);

            /* �ж�ʱ����3402������, ���ϱ�TAU��� */
            if ( TI_NAS_EMM_PTL_T3402 == enPtlTimerId )
            {
                NAS_EMM_SetTauTypeNoProcedure();

                ulTauRslt = MMC_LMM_TAU_RSLT_T3402_RUNNING;
                NAS_EMM_MmcSendTauActionResultIndOthertype((VOS_VOID*)&ulTauRslt);

            }
            return;
        }

        /* û�ж�ʱ�� && ��ǰTA���ϴγ��Է���ע���TA��Ч������TAU�������ó��Դ��� */
        NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
        NAS_EMM_TAU_StartTAUREQ();
        return;
    }

    /* TA��TAI LIST && EUֵΪEU1*/
    /* ����ж�ʱ�������жϸ�ת��NORMAL SEVICE����UPDATE MM̬���ȳ�ʱ */
    /* lihong00150010 emergency tau&service begin */
    if ((NAS_EMM_YES == NAS_EMM_IsT3411orT3402Running(&enPtlTimerId))
        && (VOS_TRUE != NAS_EMM_IsEnterRegLimitService()))
    /* lihong00150010 emergency tau&service end */
    {
        NAS_EMM_TAU_LOG_NORM("NAS_EMM_RegSomeStateMsgSysInfoCommProc: Check enter Nor-service or Upt-MM state.");

        if (NAS_EMM_YES == NAS_EMM_TAU_CanTriggerComTauWithIMSI())
        {
            /* ��״̬ת����MS_REG + EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM״̬ */
            NAS_EMM_TAUSER_FSMTranState(   EMM_MS_REG,
                                           EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM,
                                           TI_NAS_EMM_STATE_NO_TIMER);
        }
        else
        {
            /* ��״̬ת����MS_REG + SS_REG_NORMAL_SERVICE״̬ */
            NAS_EMM_TAUSER_FSMTranState(   EMM_MS_REG,
                                           EMM_SS_REG_NORMAL_SERVICE,
                                           TI_NAS_EMM_STATE_NO_TIMER);
        }

        NAS_EMM_SetTauTypeNoProcedure();

        /*��MMC����LMM_MMC_TAU_RESULT_IND��Ϣ*/
        NAS_EMM_MmcSendTauActionResultIndSuccWithoutPara();

        return ;
    }

    /* TA��TAI LIST && EUֵΪEU1*/
    /* ���û�ж�ʱ�� */
    /* ������㷢��TAU��������ֱ�ӷ��𣬷���תNORMAL SERVICE */
    if (NAS_EMM_NO == NAS_EMM_RegSomeStateStartTAUNeeded())
    {
        NAS_EMM_TAU_LOG_NORM("NAS_EMM_RegSomeStateMsgSysInfoCommProc: TAU not needed, return Nor-service.");

        /* TA��TAI List�У����� L.V.R TAI*/
        NAS_EMM_SetLVRTai(pstPresentNetInfo);
		/* lihong00150010 emergency tau&service begin */
        if (NAS_LMM_REG_STATUS_EMC_REGED != NAS_LMM_GetEmmInfoRegStatus())
        {
            /*����PS LOC��Ϣ*/
            NAS_LMM_WritePsLoc(NAS_NV_ITEM_UPDATE);
        }

        NAS_EMM_TranStateRegNormalServiceOrRegLimitService();

        /* ת��EMM״̬��MS_REG+SS_NORMAL_SEARVICE*/
        /*NAS_EMM_TAUSER_FSMTranState(    EMM_MS_REG,
                                        EMM_SS_REG_NORMAL_SERVICE,
                                        TI_NAS_EMM_STATE_NO_TIMER);*/
        /* lihong00150010 emergency tau&service end */
        NAS_EMM_SetTauTypeNoProcedure();

        /*��MMC����LMM_MMC_TAU_RESULT_IND��Ϣ*/
        NAS_EMM_MmcSendTauActionResultIndSuccWithoutPara();
    }

    return;
}

/* lihong00150010 emergency tau&service begin */
/*****************************************************************************
 Function Name   : NAS_EMM_RegStateMmcOriResumeSysInfoWaitTimerExp3411or3402Running
 Description     : �յ�ϵͳ��Ϣʱʶ���Ҫ�ȴ�T3402��ʱ�����
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010    2014-01-23  Draft Enact
*****************************************************************************/
VOS_UINT32  NAS_EMM_RegStateMmcOriResumeSysInfoWaitTimerExp3411or3402Running( VOS_VOID )
{
    NAS_EMM_TAU_LOG_NORM("NAS_EMM_RegStateMmcOriResumeSysInfoRecogniseWaitTimerExp: Check enter Nor-service or Upt-MM state.");

    if (MMC_LMM_TIN_P_TMSI == NAS_EMM_GetTinType())
    {
        NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_NOT_UPDATED_EU2;

        /* lihong00150010 emergency tau&service begin */
        if (NAS_LMM_REG_STATUS_EMC_REGED != NAS_LMM_GetEmmInfoRegStatus())
        {
            /*����PS LOC��Ϣ*/
            NAS_LMM_WritePsLoc(NAS_NV_ITEM_UPDATE);
        }
        /* lihong00150010 emergency tau&service end */

        /* ��״̬ת����MS_REG + EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM״̬ */
        NAS_EMM_TAUSER_FSMTranState(   EMM_MS_REG,
                                       EMM_SS_REG_ATTEMPTING_TO_UPDATE,
                                       TI_NAS_EMM_STATE_NO_TIMER);
    }
    else
    {   /* lihong00150010 emergency tau&service begin */
        if (VOS_TRUE == NAS_EMM_IsEnterRegLimitService())
        {
            return NAS_EMM_FAIL;
        }
        /* lihong00150010 emergency tau&service end */
        if (NAS_EMM_YES == NAS_EMM_TAU_CanTriggerComTauWithIMSI())
        {
            /* ��״̬ת����MS_REG + EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM״̬ */
            NAS_EMM_TAUSER_FSMTranState(   EMM_MS_REG,
                                           EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM,
                                           TI_NAS_EMM_STATE_NO_TIMER);
        }
        else
        {
            /* ��״̬ת����MS_REG + SS_REG_NORMAL_SERVICE״̬ */
            NAS_EMM_TAUSER_FSMTranState(   EMM_MS_REG,
                                           EMM_SS_REG_NORMAL_SERVICE,
                                           TI_NAS_EMM_STATE_NO_TIMER);
        }
    }

    NAS_EMM_SetTauTypeNoProcedure();

    /*��MMC����LMM_MMC_TAU_RESULT_IND��Ϣ*/
    NAS_EMM_MmcSendTauActionResultIndSuccWithoutPara();

    return NAS_EMM_SUCC;
}
/* lihong00150010 emergency tau&service end */

/*****************************************************************************
 Function Name   : NAS_EMM_RegStateMmcOriResumeSysInfoRecogniseWaitTimerExp
 Description     : �յ�ϵͳ��Ϣʱʶ���Ҫ�ȴ�T3402��ʱ�����
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.wangchen          2012-10-31  Draft Enact
    2.lihong00150010    2012-12-18  Modify:Emergency
*****************************************************************************/
VOS_UINT32  NAS_EMM_RegStateMmcOriResumeSysInfoRecogniseWaitTimerExp( VOS_VOID )
{
    NAS_MM_TA_STRU                     *pstLastAttmpRegTa = NAS_EMM_NULL_PTR;
    NAS_MM_TA_STRU                      stCurTa           = {0};
    NAS_LMM_PTL_TI_ENUM_UINT16          enPtlTimerId     = NAS_LMM_PTL_TI_BUTT;
    MMC_LMM_TAU_RSLT_ENUM_UINT32        ulTauRslt = MMC_LMM_TAU_RSLT_BUTT;

    /* ��ȡ��ǰTA���ϴγ���ע���TA��Ϣ */
    NAS_EMM_GetCurrentTa(&stCurTa);
    pstLastAttmpRegTa                  = NAS_LMM_GetEmmInfoNetInfoLastAttmpRegTaAddr();

    /* �����ǰTA����TAI LIST */
    if (NAS_EMM_FAIL == NAS_EMM_TAU_IsCurrentTAInTaList())
    {
        /* TAû��&&�ж�ʱ��&&EUֵ����EU1���ȳ�ʱ; ������������TAU*/
        if( (NAS_LMM_MATCH_SUCCESS == NAS_LMM_TaMatch(pstLastAttmpRegTa, &stCurTa))
            && (NAS_EMM_YES == NAS_EMM_IsT3411orT3402Running(&enPtlTimerId))
            && (EMM_US_UPDATED_EU1 != NAS_EMM_TAUSER_GetAuxFsmUpStat()))
        {
            NAS_EMM_TAU_LOG_NORM("NAS_EMM_RegStateMmcOriResumeSysInfoRecogniseWaitTimerExp: TA is not in TAI LIST, wait timer expired.");
            NAS_EMM_TAUSER_FSMTranState( EMM_MS_REG,
                                         EMM_SS_REG_ATTEMPTING_TO_UPDATE,
                                         TI_NAS_EMM_STATE_NO_TIMER);

            /* �ж�ʱ��3402������, ���ϱ�TAU��� */
            if ( TI_NAS_EMM_PTL_T3402 == enPtlTimerId )
            {
                NAS_EMM_SetTauTypeNoProcedure();

                ulTauRslt = MMC_LMM_TAU_RSLT_T3402_RUNNING;
                NAS_EMM_MmcSendTauActionResultIndOthertype((VOS_VOID*)&ulTauRslt);


            }
            return NAS_EMM_SUCC;
        }
        return NAS_EMM_FAIL;

    }

    if (EMM_US_UPDATED_EU1 != NAS_EMM_TAUSER_GetAuxFsmUpStat())
    {
        NAS_EMM_TAU_LOG_NORM("NAS_EMM_RegStateMmcOriResumeSysInfoRecogniseWaitTimerExp: TA is in TAI LIST, not EU1.");

        /*�ж�ʱ�� && ��ǰTA���ϴγ��Է���ע���TA��Ч���ȳ�ʱ*/
        if ((NAS_EMM_YES == NAS_LMM_IsLastAttemptRegTaInTaiList())
            && (NAS_EMM_YES == NAS_EMM_IsT3411orT3402Running(&enPtlTimerId)))
        {
                NAS_EMM_TAU_LOG_NORM("NAS_EMM_RegStateMmcOriResumeSysInfoRecogniseWaitTimerExp: Equivalent TA, wait timer expired.");
                NAS_EMM_TAUSER_FSMTranState(EMM_MS_REG,
                                            EMM_SS_REG_ATTEMPTING_TO_UPDATE,
                                            TI_NAS_EMM_STATE_NO_TIMER);

            /* �ж�ʱ��3402������, ���ϱ�TAU��� */
            if ( TI_NAS_EMM_PTL_T3402 == enPtlTimerId )
            {
                NAS_EMM_SetTauTypeNoProcedure();

                ulTauRslt = MMC_LMM_TAU_RSLT_T3402_RUNNING;
                NAS_EMM_MmcSendTauActionResultIndOthertype((VOS_VOID*)&ulTauRslt);

            }
            return NAS_EMM_SUCC;
        }
        return NAS_EMM_FAIL;
    }
    /* TA��TAI LIST && EUֵΪEU1*/

    /* ����ж�ʱ�������жϸ�ת��NORMAL SEVICE����UPDATE MM̬���ȳ�ʱ */
    if (NAS_EMM_YES == NAS_EMM_IsT3411orT3402Running(&enPtlTimerId))
    {
        /* lihong00150010 emergency tau&service begin */
        return NAS_EMM_RegStateMmcOriResumeSysInfoWaitTimerExp3411or3402Running();
        /* lihong00150010 emergency tau&service end */
    }

    return NAS_EMM_FAIL;
}


/*****************************************************************************
 Function Name   : NAS_EMM_RegStateMmcOriResumeSysInfoNeednotWaitTimerExpProc
 Description     : ע��̬���յ�ϵͳ��Ϣ,����Ҫ��T3411����T3402��ʱ�Ĺ�������
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.wangchen          2012-10-31  Draft Enact
    2.lihong00150010    2012-12-18  Modify:Emergency
*****************************************************************************/
VOS_VOID  NAS_EMM_RegStateMmcOriResumeSysInfoNeednotWaitTimerExpProc( VOS_VOID )
{
    NAS_MM_NETWORK_ID_STRU             *pstPresentNetInfo = NAS_EMM_NULL_PTR;
    GMM_LMM_GPRS_MM_STATE_ENUM_UINT32   enGprsMmState   = GMM_LMM_GPRS_MM_STATE_BUTT;


    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_RegStateMmcOriResumeSysInfoNeednotWaitTimerExpProc is entered.");

    /*c)when the UE enters EMM-REGISTERED.NORMAL-SERVICE and the UE's TIN indicates "P-TMSI"*/
    if (MMC_LMM_TIN_P_TMSI == NAS_EMM_GetTinType())
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_RegStateMmcOriResumeSysInfoNeednotWaitTimerExpProc:TIN is p-tmsi");

        if ((NAS_EMM_T3412_EXP_YES_REG_NO_AVALABLE_CELL == NAS_LMM_GetEmmInfoT3412ExpCtrl())
            && (NAS_LMM_REG_DOMAIN_CS_PS == NAS_LMM_GetEmmInfoRegDomain()))
        {
            NAS_EMM_TAU_LOG_INFO("NAS_EMM_RegStateMmcOriResumeSysInfoNeednotWaitTimerExpProc:combined succ");

            /* ��ע�����ΪPS����Ϊ�˻ص�REG+NORMAL_SERVICE̬�����Ҫ��������TAU����
                TAU����Ӧ����Ϊcombined TA/LA updating with IMSI attach */
            NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_PS);

        }
        /*NAS_EMM_TAU_SaveEmmTAUStartCause(     NAS_EMM_TAU_START_CAUSE_SYSINFO);*/
        NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
        NAS_LMM_SetEmmInfoTriggerTauSysChange(NAS_EMM_YES);
        NAS_EMM_TAU_StartTAUREQ();
        return;
    }

    /* d)when the UE performs an intersystem change from A/Gb or Iu mode to S1 mode,
     and the UE previously either performed a location area update procedure
     or a combined routing area update procedure in A/Gb or Iu mode,
     in order to re-establish the SGs association. In this case the EPS update
     type IE shall be set to "combined TA/LA updating with IMSI attach"; */
    if ((NAS_EMM_LAU_OR_COMBINED_RAU_HAPPENED == NAS_LMM_GetEmmInfoLauOrComRauFlag())
        || (NAS_EMM_SRVCC_HAPPENED == NAS_LMM_GetEmmInfoSrvccFlag()))
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_RegStateMmcOriResumeSysInfoNeednotWaitTimerExpProc:FIRST TAU");

        /*����TAU����*/
        /*NAS_EMM_TAU_SaveEmmTAUStartCause(     NAS_EMM_TAU_START_CAUSE_SYSINFO);*/
        NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
        NAS_LMM_SetEmmInfoTriggerTauSysChange(NAS_EMM_YES);
        NAS_EMM_TAU_StartTAUREQ();

        return;
    }
    /* ��ȡGģ����״̬ */
    enGprsMmState = NAS_LMM_GetEmmInfoGConnState();

    /* l)when the UE reselects an E-UTRAN cell while it was in GPRS READY state or PMM-CONNECTED mode; */
    if (GMM_LMM_GPRS_MM_STATE_READY  == enGprsMmState)
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_RegStateMmcOriResumeSysInfoNeednotWaitTimerExpProc:GPRS READY init tau");
        NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
        NAS_LMM_SetEmmInfoTriggerTauSysChange(NAS_EMM_YES);
        NAS_EMM_TAU_StartTAUREQ();
        return ;
    }

    /* ����TAU */
    if ((GMM_LMM_GPRS_SUSPENSION == NAS_LMM_GetEmmInfoPsState())
        && (NAS_EMM_YES == NAS_EMM_IsCsPsUeMode()))
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_RegStateMmcOriResumeSysInfoNeednotWaitTimerExpProc:EPS service suspended init tau");
        NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
        NAS_LMM_SetEmmInfoTriggerTauSysChange(NAS_EMM_YES);
        NAS_EMM_TAU_StartTAUREQ();
        return ;
    }

    /*q)When the UE performs an intersystem change from A/Gb mode to S1 mode and the TIN indicates "RAT-related TMSI",
    but the UE is required to perform tracking area updating for IMS voice termination as specified in 3GPP TS 24.008 [13],
    annex P.4*/
    if ((NAS_LMM_RSM_SYS_CHNG_DIR_G2L == NAS_EMM_GetResumeDir())
        && (NAS_EMM_YES == NAS_EMM_IsAnnexP4ConditionSatisfied())
        && (MMC_LMM_TIN_RAT_RELATED_TMSI == NAS_EMM_GetTinType()))
    {
        NAS_EMM_TAU_LOG_INFO("NAS_EMM_ProcSysWhenRsmGu2LRegRedirect:ISR act and P.4 init tau");
        NAS_EMM_TAU_StartTauForInterRat();
        return;
    }

    /* �����ǰTA����TAI LIST */
    if (NAS_EMM_FAIL == NAS_EMM_TAU_IsCurrentTAInTaList())
    {
        /* ����TAU����������TAU Procedure */
        NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);
        NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
        NAS_EMM_TAU_StartTAUREQ();
        return;

    }

    /* TA��TAI LIST && EUֵ��ΪEU1 */
    if (EMM_US_UPDATED_EU1 != NAS_EMM_TAUSER_GetAuxFsmUpStat())
    {
        NAS_EMM_TAU_LOG_NORM("NAS_EMM_RegStateMmcOriResumeSysInfoNeednotWaitTimerExpProc: TA is in TAI LIST, not EU1.");

        /* �ϴγ��Է���ע���TAU��TA����TAI LIST�����ü�������TAU */
        if (NAS_EMM_NO == NAS_LMM_IsLastAttemptRegTaInTaiList())
        {
            NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);
            NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
            NAS_EMM_TAU_StartTAUREQ();
            return;
        }

        /* û�ж�ʱ�� && ��ǰTA���ϴγ��Է���ע���TA��Ч������TAU�������ó��Դ��� */
        NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
        NAS_EMM_TAU_StartTAUREQ();
        return;
    }


    /* TA��TAI LIST && EUֵΪEU1*/
    /* ���û�ж�ʱ�� */
    /* ������㷢��TAU��������ֱ�ӷ��𣬷���תNORMAL SERVICE */
    if (NAS_EMM_NO == NAS_EMM_RegSomeStateStartTAUNeeded())
    {
        NAS_EMM_TAU_LOG_NORM("NAS_EMM_RegStateMmcOriResumeSysInfoNeednotWaitTimerExpProc: TAU not needed, return Nor-service.");

        /* TA��TAI List�У����� L.V.R TAI*/
        NAS_EMM_SetLVRTai(pstPresentNetInfo);
        /* lihong00150010 emergency tau&service begin */
        if (NAS_LMM_REG_STATUS_EMC_REGED != NAS_LMM_GetEmmInfoRegStatus())
        {
            /*����PS LOC��Ϣ*/
            NAS_LMM_WritePsLoc(NAS_NV_ITEM_UPDATE);
        }

        NAS_EMM_TranStateRegNormalServiceOrRegLimitService();

        /* ת��EMM״̬��MS_REG+SS_NORMAL_SEARVICE*/
        /*NAS_EMM_TAUSER_FSMTranState(    EMM_MS_REG,
                                        EMM_SS_REG_NORMAL_SERVICE,
                                        TI_NAS_EMM_STATE_NO_TIMER);*/
        /* lihong00150010 emergency tau&service end */

        NAS_EMM_SetTauTypeNoProcedure();

        NAS_EMM_ProcHoWaitSysInfoBufferMsg();

        /*��MMC����LMM_MMC_TAU_RESULT_IND��Ϣ*/
        NAS_EMM_MmcSendTauActionResultIndSuccWithoutPara();

        return;
    }

    (VOS_VOID)NAS_EMM_ClrHoWaitSysInfoBufferMsg();

    return;
}



VOS_VOID  NAS_EMM_RegNormalServiceOrUpdateMMMsgSysInfoCommProc( VOS_VOID)
{
    NAS_MM_NETWORK_ID_STRU             *pstPresentNetInfo = NAS_EMM_NULL_PTR;
    NAS_MM_TA_STRU                      stCurTa           = {0};
    NAS_LMM_PTL_TI_ENUM_UINT16          enPtlTimerId     = NAS_LMM_PTL_TI_BUTT;
    VOS_UINT32                          ulRslt = NAS_EMM_FAIL;

    /* ��ȡ��ǰTA��Ϣ */
    NAS_EMM_GetCurrentTa(&stCurTa);
    pstPresentNetInfo                  = NAS_LMM_GetEmmInfoNetInfoPresentNetAddr();

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_TAU_LOG_INFO("NAS_EMM_RegNormalServiceOrUpdateMMMsgSysInfoCommProc is entered.");

    /* �����ǰTA����TAI LIST */
    if (NAS_EMM_FAIL == NAS_EMM_TAU_IsCurrentTAInTaList())
    {
        /* ����TAU����������TAU Procedure */
        NAS_EMM_TAU_SaveEmmTAUAttemptCnt(NAS_EMM_TAU_ATTEMPT_CNT_ZERO);
        NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
        ulRslt = NAS_EMM_ClrHoWaitSysInfoBufferMsg();
        if (NAS_EMM_SUCC == ulRslt)
        {
            NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_NOT_UPDATED_EU2;

            /*����PS LOC��Ϣ*/
            NAS_LMM_WritePsLoc(NAS_NV_ITEM_UPDATE);
        }
        NAS_EMM_TAU_StartTAUREQ();
        return;

    }
     /* TA��TAI LIST */
    /* ����ж�ʱ�����ȳ�ʱ */
    /* lihong00150010 emergency tau&service begin */
    if ((NAS_EMM_YES == NAS_EMM_IsT3411orT3402Running(&enPtlTimerId))
        && (VOS_TRUE != NAS_EMM_IsEnterRegLimitService()))
    /* lihong00150010 emergency tau&service end */
    {
        NAS_EMM_TAU_LOG_NORM("NAS_EMM_RegNormalServiceOrUpdateMMMsgSysInfoCommProc: Check enter Nor-service or Upt-MM state.");

        NAS_EMM_ProcHoWaitSysInfoBufferMsg();

        return ;
    }

    /* TA��TAI LIST && û�ж�ʱ��  */
    /* ������㷢��TAU��������ֱ�ӷ��𣬷���״̬����(UPDATE MM̬һ���ж�ʱ��) */
    if (NAS_EMM_NO == NAS_EMM_RegSomeStateStartTAUNeeded())
    {
        NAS_EMM_TAU_LOG_NORM("NAS_EMM_RegNormalServiceOrUpdateMMMsgSysInfoCommProc: TAU not needed, return Nor-service.");

        /* TA��TAI List�У����� L.V.R TAI*/
        NAS_EMM_SetLVRTai(pstPresentNetInfo);

        /* lihong00150010 emergency tau&service begin */
        if (NAS_LMM_REG_STATUS_EMC_REGED != NAS_LMM_GetEmmInfoRegStatus())
        {
            /*����PS LOC��Ϣ*/
            NAS_LMM_WritePsLoc(NAS_NV_ITEM_UPDATE);
        }

        if (VOS_TRUE == NAS_EMM_IsEnterRegLimitService())
        {
            NAS_EMM_TAUSER_FSMTranState(    EMM_MS_REG,
                                            EMM_SS_REG_LIMITED_SERVICE,
                                            TI_NAS_EMM_STATE_NO_TIMER);
        }
        /* lihong00150010 emergency tau&service end */

        NAS_EMM_ProcHoWaitSysInfoBufferMsg();

        return;
    }

    (VOS_VOID)NAS_EMM_ClrHoWaitSysInfoBufferMsg();

    return;

}
VOS_VOID  NAS_EMM_TAU_StartTauForInterRat( VOS_VOID )
{
    if (NAS_LMM_SYS_CHNG_TYPE_HO != NAS_EMM_GetResumeType())
    {
        /* ��ȡGU��UMTS��ȫ�����ģ�����LTE�İ�ȫ�����������ð�ȫ״̬*/
        NAS_EMM_SecuResumeIdle();
    }

    /*NAS_EMM_TAU_SaveEmmTAUStartCause(     NAS_EMM_TAU_START_CAUSE_SYSINFO);*/
    NAS_EMM_TAU_SaveEmmTAUStartCause(NAS_EMM_TAU_START_CAUSE_OTHERS);
    NAS_LMM_SetEmmInfoTriggerTauSysChange(NAS_EMM_YES);
    NAS_EMM_TAU_StartTAUREQ();

    return;
}

/* lihong00150010 emergency tau&service begin */
#if 0
VOS_VOID  NAS_EMM_TAU_SetT3412Exp
(
    NAS_EMM_T3412_EXP_ENUM_UINT32       enEmmT3412ExpCtrl
)
{
    NAS_LMM_SetEmmInfoT3412ExpCtrl(enEmmT3412ExpCtrl);
}


VOS_UINT32  NAS_EMM_QueryEmergencyBear(VOS_VOID)
{

    return NAS_LMM_FAIL;
}
#endif
/* lihong00150010 emergency tau&service end */

VOS_UINT32  NAS_LMM_PreProcMsgSrvccNofify(MsgBlock *    pMsg )
{
    MM_LMM_SRVCC_STATUS_NOTIFY_STRU            *pstSrvccNotify;

    pstSrvccNotify = (VOS_VOID*)pMsg;

    if (MM_LMM_SRVCC_STATUS_SUCC == pstSrvccNotify->enSrvccStatus)
    {
        NAS_LMM_SetEmmInfoSrvccFlag(NAS_EMM_SRVCC_HAPPENED);

    }
    else
    {
        NAS_LMM_SetEmmInfoSrvccFlag(NAS_EMM_SRVCC_NOT_HAPPENED);
    }
    return NAS_LMM_MSG_HANDLED;
}


/* lihong00150010 emergency tau&service begin */
/*****************************************************************************
 Function Name   : NAS_EMM_EmcPndReqTauAbnormalCommProc
 Description     : ����PDN���ӽ��������TAU�쳣��������
 Input           : pfTauRslt----------------TAU����ϱ�����ָ��
                   pvPara-------------------TAU����ϱ����������
                   ucSs---------------------��״̬
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2012-12-31  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_EMM_EmcPndReqTauAbnormalCommProc
(
    NAS_LMM_SEND_TAU_RESULT_ACT_FUN     pfTauRslt,
    const VOS_VOID                     *pvPara,
    NAS_EMM_SUB_STATE_ENUM_UINT16       ucSs
)
{
    /* �ϱ�TAU��� */
    NAS_EMM_TAU_SEND_MMC_RESULT_IND(pfTauRslt, pvPara);

    NAS_EMM_TAU_SendEsmStatusInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);

    /* ����DETACH�ͷ���Դ:��̬�ڴ桢����ֵ */
    NAS_LMM_DeregReleaseResource();

    /*��MMC���ͱ���LMM_MMC_DETACH_IND��Ϣ*/
    NAS_EMM_MmcSendDetIndLocal( MMC_LMM_L_LOCAL_DETACH_OTHERS);

    /* ��¼ATTACH����ԭ��ֵ */
    NAS_EMM_GLO_AD_GetAttCau() = EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER;

    NAS_EMM_AdStateConvert(     EMM_MS_DEREG,
                                ucSs,
                                TI_NAS_EMM_STATE_NO_TIMER);
}
/* lihong00150010 emergency tau&service end */

/*lint +e961*/
/*lint +e960*/
/*lint +e831*/
/*lint +e669*/
/*lint +e589*/



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif



