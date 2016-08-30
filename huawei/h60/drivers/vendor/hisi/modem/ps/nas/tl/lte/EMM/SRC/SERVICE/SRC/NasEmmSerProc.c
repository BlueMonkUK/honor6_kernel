

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include  "NasEmmTauSerInclude.h"
#include  "NasLmmPubMPrint.h"
#include  "EmmTcInterface.h"
#include  "NasLmmPubMOm.h"
/* lihong00150010 emergency tau&service begin */
#include  "NasEmmAttDetInclude.h"
/* lihong00150010 emergency tau&service end */

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASEMMSERVICEPROC_C
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
NAS_EMM_SER_CTRL_STRU                   g_stEmmSERCtrlTbl;
NAS_EMM_SER_ESM_MSG_BUF_STRU            g_stEmmEsmMsgBuf;


/*lint -e960*/
/*lint -e961*/
VOS_VOID NAS_EMM_SER_Init(VOS_VOID)
{
    NAS_EMM_SER_LOG_NORM("NAS_EMM_SER_Init                   START INIT...");

    NAS_LMM_MEM_SET(&g_stEmmSERCtrlTbl, 0, sizeof(g_stEmmSERCtrlTbl));

    NAS_LMM_MEM_SET(&g_stEmmEsmMsgBuf, 0, sizeof(g_stEmmEsmMsgBuf));

    return;
}


VOS_VOID  NAS_EMM_SER_ClearResource( VOS_VOID )
{
    NAS_EMM_SER_LOG_NORM("NAS_EMM_SER_ClearResource entered.");

    /* ���SERȫ�ֱ���*/
    NAS_LMM_MEM_SET(&g_stEmmSERCtrlTbl, 0, sizeof(g_stEmmSERCtrlTbl));

    /* �ͷŻ����ESM��Ϣ*/
    NAS_EMM_SerClearEsmDataBuf();

    /* ��ս���Bar��Ϣ*/
    NAS_EMM_ClearBarResouce();

    /* ֹͣService������ص����ж�ʱ��*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417);
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

    return;
}


VOS_VOID  NAS_EMM_FreeSerDyn( VOS_VOID )
{
    /* �ͷ�SERVICEģ�黺���ESM����Ϣ */
    NAS_EMM_SerClearEsmDataBuf();

    return;
}
VOS_VOID NAS_EMM_SER_CompCnServiceReq(  LRRC_LNAS_MSG_STRU   *pNasMsg)
{
    VOS_UINT32                          ulIndex     = 0;
    VOS_UINT8                           ucKsiSn;
    VOS_UINT8                           ucKsi;
    VOS_UINT8                           ucSequenceNum;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO("NAS_EMM_SER_CompCnServiceReq is entered.");

    /* ��ս����CN��Ϣ�Ŀռ� */
    /* xiongxianghui00253310 delete memset 2013-11-26 begin */
    /* NAS_LMM_MEM_SET(pNasMsg->aucNasMsg, 0, pNasMsg->ulNasMsgSize); */
    /* xiongxianghui00253310 delete memset 2013-11-26 end   */

    /* ��� Security header type  +  Protocol discriminator */
    pNasMsg->aucNasMsg[ulIndex++]       = EMM_CN_SECURITY_HEADER_TYPE_SER_REQ;

    /* ���� KsiSn*/
    ucKsi = NAS_EMM_SecuGetKSIValue();
    ucSequenceNum = (VOS_UINT8)NAS_EMM_SecuGetUlNasCountVAlue();
    ucSequenceNum = ucSequenceNum & EMM_TAUSER_LOW5_BITS;
    ucKsiSn = ((VOS_UINT8)(ucKsi << 5)) | ucSequenceNum;


    /* ��� KSI and sequence number   */
    pNasMsg->aucNasMsg[ulIndex++]       = ucKsiSn;


    /* ��� Message authentication code */
    /*����ֻ��Ҫ��ʼ��Ϊ0���������Ա���ʱ������������ֽ�*/
    pNasMsg->aucNasMsg[ulIndex++]       = 0;
    pNasMsg->aucNasMsg[ulIndex]         = 0;
    return;
}

/*lint -e669*/

VOS_VOID    NAS_EMM_SER_SendMrrcDataReq_ServiceReq()
{
    NAS_EMM_MRRC_DATA_REQ_STRU          *pMrrcDataReqMsg    = NAS_EMM_NULL_PTR;
    VOS_UINT32                          ulDataReqMsgLenNoHeader;
    VOS_UINT32                          ulCnMsgLen;

    NAS_EMM_SER_LOG_INFO("NAS_EMM_SER_SendMrrcDataReq_ServiceReq is entered.");

    /* ����NAS_MSG_STRU��Ϣ���� */
    ulCnMsgLen                              = NAS_EMM_COUNT_CN_SER_REQ_LEN;
    ulDataReqMsgLenNoHeader                 = EMM_COUNT_DATA_REQ_LEN(ulCnMsgLen);

    /* ����DOPRA��Ϣ */
    pMrrcDataReqMsg = (VOS_VOID *)NAS_LMM_MEM_ALLOC(NAS_EMM_INTRA_MSG_MAX_SIZE);

    if(NAS_EMM_NULL_PTR == pMrrcDataReqMsg)
    {
        NAS_EMM_SER_LOG_ERR( "Nas_Emm_SER_SendMrrcDataReq_ServiceReq: MSG ALLOC ERR !!");
        return;
    }

    /* xiongxianghui00253310 add memset 2013-11-26 begin */
    NAS_LMM_MEM_SET(pMrrcDataReqMsg, 0, NAS_EMM_INTRA_MSG_MAX_SIZE);
    /* xiongxianghui00253310 add memset 2013-11-26 end   */

    /* ���CSFB ABORT��ʶ */
    NAS_EMM_SER_SaveEmmSerCsfbAbortFlag(NAS_EMM_CSFB_ABORT_FLAG_INVALID);
    /* ��дMRRC_DATA_REQ ��DOPRA��Ϣͷ */
    EMM_COMP_MM_MSG_HEADER(pMrrcDataReqMsg, ulDataReqMsgLenNoHeader);

    /* ��дMRRC_DATA_REQ ����ϢID��ʶ */
    pMrrcDataReqMsg->ulMsgId                = ID_NAS_LMM_INTRA_MRRC_DATA_REQ;

    /*��д����ԭ��*/
    switch(NAS_EMM_SER_GetEmmSERStartCause())
    {
        case NAS_EMM_SER_START_CAUSE_RRC_PAGING:
            pMrrcDataReqMsg->enEstCaue = LRRC_LNAS_EST_CAUSE_MT_ACCESS;
            pMrrcDataReqMsg->enCallType = LRRC_LNAS_CALL_TYPE_TERMINATING_CALL;
            break;

        case NAS_EMM_SER_START_CAUSE_RABM_REEST:
            pMrrcDataReqMsg->enEstCaue = LRRC_LNAS_EST_CAUSE_MO_DATA;
            pMrrcDataReqMsg->enCallType = LRRC_LNAS_CALL_TYPE_ORIGINATING_CALL;
            break;

        case NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ:
            pMrrcDataReqMsg->enEstCaue = LRRC_LNAS_EST_CAUSE_MO_DATA;
            pMrrcDataReqMsg->enCallType = LRRC_LNAS_CALL_TYPE_ORIGINATING_CALL;
            break;

        case NAS_ESM_SER_START_CAUSE_UPLINK_PENDING:
            pMrrcDataReqMsg->enEstCaue = LRRC_LNAS_EST_CAUSE_MO_DATA;
            pMrrcDataReqMsg->enCallType = LRRC_LNAS_CALL_TYPE_ORIGINATING_CALL;
            break;

        case NAS_EMM_SER_START_CAUSE_SMS_EST_REQ:
            pMrrcDataReqMsg->enEstCaue = LRRC_LNAS_EST_CAUSE_MO_DATA;
            pMrrcDataReqMsg->enCallType = LRRC_LNAS_CALL_TYPE_ORIGINATING_CALL;
            break;

        /* lihong00150010 emergency tau&service begin */
        case NAS_EMM_SER_START_CAUSE_ESM_DATA_REQ_EMC:
            pMrrcDataReqMsg->enEstCaue = LRRC_LNAS_EST_CAUSE_EMERGENCY_CALL;
            pMrrcDataReqMsg->enCallType = LRRC_LNAS_CALL_TYPE_EMERGENCY_CALL;
            break;

        case NAS_EMM_SER_START_CAUSE_RABM_REEST_EMC:
            pMrrcDataReqMsg->enEstCaue = LRRC_LNAS_EST_CAUSE_EMERGENCY_CALL;
            pMrrcDataReqMsg->enCallType = LRRC_LNAS_CALL_TYPE_EMERGENCY_CALL;
            break;
        /* lihong00150010 emergency tau&service end */

        default:
            NAS_EMM_SER_LOG_ERR("Nas_Emm_SER_SendMrrcDataReq_ServiceReq:StartCause ERR");
            break;
    }
    pMrrcDataReqMsg->enEmmMsgType       = NAS_EMM_MSG_SERVICE_REQ;

    /* ��дMRRC_DATA_REQ ��NAS��Ϣ���ȱ�ʶ */
    pMrrcDataReqMsg->stNasMsg.ulNasMsgSize  = ulCnMsgLen;

    pMrrcDataReqMsg->enDataCnf              = LRRC_LMM_DATA_CNF_NEED;

    /* ����MRRC_DATA_REQ �е� NAS_MSG_STRU,��CN��Ϣ(Compose the msg of): SER_REQ */
    NAS_EMM_SER_CompCnServiceReq(               &(pMrrcDataReqMsg->stNasMsg));

    /*�տ���Ϣ�ϱ�SER REQ*/
    NAS_LMM_SendOmtAirMsg(NAS_EMM_OMT_AIR_MSG_UP, NAS_EMM_SER_REQ,  (NAS_MSG_STRU*)&(pMrrcDataReqMsg->stNasMsg));

    /*�ؼ��¼��ϱ�SER REQ*/
    NAS_LMM_SendOmtKeyEvent(         EMM_OMT_KE_SER_REQ);

    /* ������Ϣ(Send the msg of) MRRC_DATA_REQ(SER_REQ) */
    NAS_EMM_SndUplinkNasMsg((VOS_VOID*)pMrrcDataReqMsg);

    NAS_LMM_MEM_FREE(pMrrcDataReqMsg);

    return;
}
/*lint +e669*/


/*****************************************************************************
 Function Name   : NAS_EMM_CompCnExtendedSerReqMoNasMsg
 Description     : ��װEXTENDED SERVICE REQ��Ϣ
 Input           : pulIndex
 Output          : pucCnMsg
                   pulIndex
 Return          : VOS_VOID

 History         :
    1.lihong      2012-02-23  Draft Enact

*****************************************************************************/
VOS_VOID    NAS_EMM_CompCnExtendedSerReqMoNasMsg
(
    VOS_UINT8                          *pucCnMsg,
    VOS_UINT32                         *pulIndex
)
{
    VOS_UINT32                          ulIndex      = NAS_EMM_NULL;
    VOS_UINT8                           ucKsi        = NAS_EMM_NULL;
    NAS_LMM_PUB_INFO_STRU              *pstEmmInfo   = VOS_NULL_PTR;
    VOS_UINT32                          ulIeLength          = 0;

    ulIndex                 = *pulIndex;
    pstEmmInfo              = NAS_LMM_GetEmmInfoAddr();

    /* ��� Protocol Discriminator + Security header type*/
    pucCnMsg[ulIndex++]     = EMM_CN_MSG_PD_EMM;

    /* ��� Detach request message identity */
    pucCnMsg[ulIndex++]     = NAS_EMM_CN_MT_EXT_SER_REQ;

    /* ���service type */
    if (NAS_EMM_SER_START_CAUSE_MO_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
    {
        pucCnMsg[ulIndex]   = MMC_LMM_SERVICE_MO_CSFB_1XCSFB;
    }
    else if (NAS_EMM_SER_START_CAUSE_MO_EMERGENCY_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
    {
        pucCnMsg[ulIndex]   = MMC_LMM_SERVICR_MO_CSFB_1XCSFB_EMERGENCY;
    }
    else
    {
        pucCnMsg[ulIndex]   = MMC_LMM_SERVICR_MT_CSFB_1XCSFB;
    }

    /*���KSIasme*/
    ucKsi = NAS_EMM_SecuGetKSIValue();
    pucCnMsg[ulIndex++]     |= (ucKsi & 0x0F)<<4;

    /*���M-TSMI*/

    /* ��дLENGTHΪ5 */
    pucCnMsg[ulIndex++]     = 5;

    /*  ��дTYPE OF IDENTITY����EVEN/ODD��ʶ������Э��ǰ4BITΪ'1111' */
    pucCnMsg[ulIndex]       = NAS_EMM_TYPE_OF_IDENTITY_TMSI_PTMSI_MTMSI;
    pucCnMsg[ulIndex++]     |= NAS_EMM_HIGH_4_BIT;

    NAS_LMM_MEM_CPY(        &(pucCnMsg[ulIndex]),
                            &pstEmmInfo->stMmUeId.stGuti.stMTmsi,
                            sizeof(NAS_EMM_MTMSI_STRU));
    ulIndex                 += sizeof(NAS_EMM_MTMSI_STRU);

    /* ֻ��MT CSFB�����CSFB RESPONSE */
    if (NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
    {
        pucCnMsg[ulIndex++] = NAS_EMM_EXTENDED_SER_REQ_CSFB_RSP_IEI | NAS_EMM_SER_GetEmmSerCsfbRsp();
    }

    /* ���EPS bearer context status */
    /*leili modify for isr begin*/
    NAS_EMM_EncodeEpsBearStatus(        &(pucCnMsg[ulIndex]),
                                        &ulIeLength);
    ulIndex                             += ulIeLength;
    /*leili modify for isr end*/
    #if 0
    pucCnMsg[ulIndex++]     = NAS_EMM_TAU_REQ_EPS_STATUS_IEI;
    pucCnMsg[ulIndex++]     = NAS_EMM_CN_EPS_CONTEXT_LEN;

    for(ulLoop = 0; ulLoop < NAS_EMM_GetEpsIdNum(); ulLoop++)
    {
        ucEsmEPSNum         = (VOS_UINT8)(NAS_EMM_GetEpsId(ulLoop));
        if( NAS_EMM_EPS_BEARER_STATUS_NUM_SEVEN < ucEsmEPSNum)
        {
            ucEsmEPSNum     = ucEsmEPSNum - NAS_EMM_EPS_BEARER_STATUS_NUM_EIGNT;
            ucEsmEPS2       = ucEsmEPS2 | NAS_EMM_TAU_ESM_BEARER_LEFT_N(ucEsmEPSNum);
        }
        else
        {
            ucEsmEPS1       = ucEsmEPS1 | NAS_EMM_TAU_ESM_BEARER_LEFT_N(ucEsmEPSNum);
        }
    }
    pucCnMsg[ulIndex++]     = ucEsmEPS1;
    pucCnMsg[ulIndex++]     = ucEsmEPS2;
    #endif
    *pulIndex               = ulIndex;

    return;

}
VOS_VOID    NAS_EMM_SER_SendMrrcDataReq_ExtendedServiceReq(VOS_VOID)
{
    NAS_EMM_MRRC_DATA_REQ_STRU         *pMrrcDataReqMsg         = NAS_EMM_NULL_PTR;
    VOS_UINT32                          ulDataReqMsgLenNoHeader = NAS_EMM_NULL;
    VOS_UINT32                          ulIndex                 = NAS_EMM_NULL;

    NAS_EMM_SER_LOG_INFO("NAS_EMM_SER_SendMrrcDataReq_ExtendedServiceReq is entered.");

    /*����С��Ϣ���ȣ�������Ϣ�ڴ�,��Ҫ�ǿ��Ƿ�����пռ�*/
    pMrrcDataReqMsg = (VOS_VOID *) NAS_LMM_MEM_ALLOC(NAS_EMM_INTRA_MSG_MAX_SIZE);

    /*�ж�����������ʧ�ܴ�ӡ�����˳�*/
    if (NAS_EMM_NULL_PTR == pMrrcDataReqMsg)
    {
        /*��ӡ����*/
        NAS_EMM_ATTACH_LOG_ERR("NAS_EMM_SER_SendMrrcDataReq_ExtendedServiceReq: MSG ALLOC ERR!");
        return;

    }

    /* ���CSFB ABORT��ʶ */
    NAS_EMM_SER_SaveEmmSerCsfbAbortFlag(NAS_EMM_CSFB_ABORT_FLAG_INVALID);

    /*��װEXTENDED SERVICE REQ��Ϣ*/
    NAS_EMM_CompCnExtendedSerReqMoNasMsg(pMrrcDataReqMsg->stNasMsg.aucNasMsg, &ulIndex);

    /*�ڲ���Ϣ���ȼ���*/
    ulDataReqMsgLenNoHeader = NAS_EMM_CountMrrcDataReqLen(ulIndex);

    if ( NAS_EMM_INTRA_MSG_MAX_SIZE < ulDataReqMsgLenNoHeader )
    {
        /* ��ӡ������Ϣ */
        NAS_LMM_PUBM_LOG_ERR("NAS_EMM_SER_SendMrrcDataReq_ExtendedServiceReq, Size error");
        NAS_LMM_MEM_FREE(pMrrcDataReqMsg);
        return ;
    }

    /*�����Ϣ����*/
    pMrrcDataReqMsg->stNasMsg.ulNasMsgSize = ulIndex;

    /*�����Ϣͷ*/
    EMM_COMP_MM_MSG_HEADER(pMrrcDataReqMsg, ulDataReqMsgLenNoHeader);

    /* ��дMRRC_DATA_REQ ����ϢID��ʶ */
    pMrrcDataReqMsg->ulMsgId            = ID_NAS_LMM_INTRA_MRRC_DATA_REQ;
    /*��д����ԭ��*/
    switch(NAS_EMM_SER_GetEmmSERStartCause())
    {
        case NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ:
            pMrrcDataReqMsg->enEstCaue  = LRRC_LNAS_EST_CAUSE_MT_ACCESS;
            pMrrcDataReqMsg->enCallType = LRRC_LNAS_CALL_TYPE_TERMINATING_CALL;
            break;

        case NAS_EMM_SER_START_CAUSE_MO_CSFB_REQ:
            pMrrcDataReqMsg->enEstCaue  = LRRC_LNAS_EST_CAUSE_MO_DATA;
            pMrrcDataReqMsg->enCallType = LRRC_LNAS_CALL_TYPE_MO_CSFB;
            break;

        case NAS_EMM_SER_START_CAUSE_MO_EMERGENCY_CSFB_REQ:
            pMrrcDataReqMsg->enEstCaue  = LRRC_LNAS_EST_CAUSE_EMERGENCY_CALL;
            pMrrcDataReqMsg->enCallType = LRRC_LNAS_CALL_TYPE_EMERGENCY_CALL;
            break;

        default:
            NAS_EMM_SER_LOG_ERR("Nas_Emm_SER_SendMrrcDataReq_ServiceReq:StartCause ERR");
            break;
    }
    pMrrcDataReqMsg->enEmmMsgType       = NAS_EMM_MSG_EXTENDED_SERVICE_REQ;

    pMrrcDataReqMsg->enDataCnf          = LRRC_LMM_DATA_CNF_NEED;

    /*�տ���Ϣ�ϱ�SER REQ*/
    NAS_LMM_SendOmtAirMsg(NAS_EMM_OMT_AIR_MSG_UP, NAS_EMM_EXTENDED_SER_REQ,  (NAS_MSG_STRU*)&(pMrrcDataReqMsg->stNasMsg));

    /*�ؼ��¼��ϱ�SER REQ*/
    NAS_LMM_SendOmtKeyEvent(            EMM_OMT_KE_EXTENDED_SER_REQ);

    /* ������Ϣ(Send the msg of) MRRC_DATA_REQ(SER_REQ) */
    NAS_EMM_SndUplinkNasMsg(             pMrrcDataReqMsg);

    NAS_LMM_MEM_FREE(pMrrcDataReqMsg);

    return;
}


VOS_VOID    NAS_EMM_SER_SendMrrcDataReq_ESMdata
(
    EMM_ESM_MSG_STRU                   *pstEsmMsg
)
{
    VOS_UINT32                          ulMsgLenthNoHeader;
    NAS_EMM_MRRC_DATA_REQ_STRU          *pMrrcDataReqMsg    = NAS_EMM_NULL_PTR;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG1_INFO( "NAS_EMM_SER_SendMrrcDataReq_ESMdata is entered.", pstEsmMsg);

    /*������Ϣ*/
    pMrrcDataReqMsg = (VOS_VOID *)NAS_LMM_MEM_ALLOC(NAS_EMM_INTRA_MSG_MAX_SIZE);
    if ( NAS_EMM_NULL_PTR == pMrrcDataReqMsg )
    {
        NAS_EMM_SER_LOG_ERR( "Nas_Emm_SER_SendMrrcDataReq_ESMdata: NAS_LMM_ALLOC_MSG err !!");
        return;
    }

    ulMsgLenthNoHeader                  = NAS_EMM_CountMrrcDataReqLen(pstEsmMsg->ulEsmMsgSize);

    /* ������Ϣ */
    pMrrcDataReqMsg->stNasMsg.ulNasMsgSize = pstEsmMsg->ulEsmMsgSize;

    NAS_LMM_MEM_CPY(                    (pMrrcDataReqMsg->stNasMsg.aucNasMsg),
                                        (pstEsmMsg->aucEsmMsg),
                                        (pstEsmMsg->ulEsmMsgSize));
    /* ��дDOPRA��Ϣͷ */
    EMM_COMP_MM_MSG_HEADER(             (pMrrcDataReqMsg),
                                        ulMsgLenthNoHeader);

    /*��д��ϢID*/
    pMrrcDataReqMsg->ulMsgId            = ID_NAS_LMM_INTRA_MRRC_DATA_REQ;

    /*��д EST CAUSE*/
    pMrrcDataReqMsg->enEstCaue          = LRRC_LNAS_EST_CAUSE_MO_DATA;
    pMrrcDataReqMsg->enCallType         = LRRC_LNAS_CALL_TYPE_ORIGINATING_CALL;
    pMrrcDataReqMsg->enEmmMsgType       = NAS_EMM_MSG_ESM;

    pMrrcDataReqMsg->enDataCnf          = LRRC_LMM_DATA_CNF_NEED;

    /* ��MRRCת��MMESM_DATA_REQ��Ϣ */
    NAS_EMM_SndUplinkNasMsg(             pMrrcDataReqMsg);

    NAS_LMM_MEM_FREE(pMrrcDataReqMsg);
    return;
}

/*******************************************************************************
  Module   :
  Function : NAS_EMM_SER_SendMrrcDataReq_Tcdata
  Input    : pMsg------------TC��Ϣ
  Output   :
  NOTE     : ��RRCת��TC_DATA_REQ��Ϣ
  Return   : VOS_VOID
  History  :
      1.lihong00150010      2009-10-16  Draft Enact
*******************************************************************************/
VOS_VOID    NAS_EMM_SER_SendMrrcDataReq_Tcdata
(
    EMM_ETC_DATA_REQ_STRU               *pMsg
)
{
    VOS_UINT32                          ulMsgLenthNoHeader;
    NAS_EMM_MRRC_DATA_REQ_STRU         *pMrrcDataReqMsg    = NAS_EMM_NULL_PTR;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG1_INFO( "NAS_EMM_SER_SendMrrcDataReq_Tcdata is entered.", pMsg);


    /*������Ϣ*/
    pMrrcDataReqMsg = (VOS_VOID *)NAS_LMM_MEM_ALLOC(NAS_EMM_INTRA_MSG_MAX_SIZE);
    if ( NAS_EMM_NULL_PTR == pMrrcDataReqMsg )
    {
        NAS_EMM_SER_LOG_ERR( "NAS_EMM_SER_SendMrrcDataReq_Tcdata: NAS_LMM_ALLOC_MSG err !!");
        return;
    }
    ulMsgLenthNoHeader                  = NAS_EMM_CountMrrcDataReqLen(pMsg->stTcMsg.ulTcMsgSize);

    /* ������Ϣ */
    pMrrcDataReqMsg->stNasMsg.ulNasMsgSize = pMsg->stTcMsg.ulTcMsgSize;

    NAS_LMM_MEM_CPY(                        (pMrrcDataReqMsg->stNasMsg.aucNasMsg),
                                        (pMsg->stTcMsg.aucTcMsg),
                                        (pMsg->stTcMsg.ulTcMsgSize));
    /* ��дDOPRA��Ϣͷ */
    EMM_COMP_MM_MSG_HEADER(             (pMrrcDataReqMsg),
                                        ulMsgLenthNoHeader);

    /*��д��ϢID*/
    pMrrcDataReqMsg->ulMsgId            = ID_NAS_LMM_INTRA_MRRC_DATA_REQ;

    /*��д EST CAUSE*/
    pMrrcDataReqMsg->enEstCaue          = LRRC_LNAS_EST_CAUSE_MO_DATA;
    pMrrcDataReqMsg->enCallType         = LRRC_LNAS_CALL_TYPE_ORIGINATING_CALL;
    pMrrcDataReqMsg->enEmmMsgType       = NAS_EMM_MSG_TC;

    pMrrcDataReqMsg->enDataCnf          = LRRC_LMM_DATA_CNF_NOT_NEED;

    /* ��MRRCת��TC_DATA_REQ��Ϣ */
    NAS_EMM_SndUplinkNasMsg(pMrrcDataReqMsg);

    NAS_LMM_MEM_FREE(pMrrcDataReqMsg);

    return;
}
VOS_VOID    NAS_EMM_SER_SendRabmReestInd(EMM_ERABM_REEST_STATE_ENUM_UINT32
                                                                enRabmReestState)
{
    EMM_ERABM_REEST_IND_STRU             *pRabmReestIndMsg    = NAS_EMM_NULL_PTR;

    NAS_EMM_SER_LOG_INFO("NAS_EMM_SER_SendRabmReestInd is entered.");

    /* ����DOPRA��Ϣ */
    pRabmReestIndMsg = (VOS_VOID *)NAS_LMM_ALLOC_MSG(sizeof(EMM_ERABM_REEST_IND_STRU));

    if(NAS_EMM_NULL_PTR == pRabmReestIndMsg)
    {
        NAS_EMM_SER_LOG_ERR( "NAS_EMM_SER_SendRabmReestInd: MSG ALLOC ERR !!");
        return;
    }

    /* ��дEMM_ERABM_REEST_IND ��DOPRA��Ϣͷ */
    EMM_COMP_ERABM_MSG_HEADER(pRabmReestIndMsg, sizeof(EMM_ERABM_REEST_IND_STRU) -
                                        EMM_LEN_VOS_MSG_HEADER);

    /* ��дEMM_ERABM_REEST_IND ����ϢID��ʶ */
    pRabmReestIndMsg->ulMsgId           = ID_EMM_ERABM_REEST_IND;

    /* ����EMM_ERABM_REEST_IND �е��ؽ�״̬ */
    pRabmReestIndMsg->enReEstState      = enRabmReestState;

    /* ������Ϣ(Send the msg of) EMM_ERABM_REEST_IND */
    NAS_LMM_SEND_MSG(                   pRabmReestIndMsg);

    return;

}
VOS_VOID NAS_EMM_SER_AbnormalOver(VOS_VOID)
{
    NAS_EMM_SER_LOG_INFO( "NAS_EMM_SER_AbnormalOver is entered.");

    /*ֹͣT3417��ʱ��*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417);

    /*ֹͣT3440��ʱ��*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_T3440);

    /*ֹͣT3417ext��ʱ��*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_STATE_SERVICE_T3417_EXT);

    /*���SR����������SMS��������Ҫ�ظ�SMS����ʧ�ܣ��������SR�ķ���ԭ��*/
    if(NAS_EMM_SER_START_CAUSE_SMS_EST_REQ == NAS_EMM_SER_GetSerStartCause())
    {
        /* SER�쳣��ԭ��ֵ�ϱ���ʱ��LMM_SMS_ERR_CAUSE_OTHERS��
           �Ժ����Ҫ������Ӧ��ԭ��ֵ���о���ϸ�ִ����ϱ�׼ȷ��ԭ��ֵ
           �˴���Ϊ�������� */
        NAS_LMM_SndLmmSmsErrInd(LMM_SMS_ERR_CAUSE_OTHERS);
        NAS_EMM_SER_SaveEmmSERStartCause(NAS_EMM_SER_START_CAUSE_BUTT);
    }

    if (VOS_TRUE != NAS_EMM_SER_IsCsfbProcedure())
    {
        /*Inform RABM that SER fail*/
         NAS_EMM_SER_SendRabmReestInd(EMM_ERABM_REEST_STATE_FAIL);
    }

    /*���ESM_DATA����*/
    NAS_EMM_SerClearEsmDataBuf();

    return;
}


VOS_UINT32    NAS_EMM_SER_CHKFSMStateMsgp(
                                    NAS_EMM_MAIN_STATE_ENUM_UINT16  stMS,
                                    NAS_EMM_SUB_STATE_ENUM_UINT16   stSS,
                                    VOS_VOID   *pMsgStru)
{

    NAS_EMM_SER_LOG1_INFO( "NAS_EMM_SER_CHKFSMStateMsgp is entered.", pMsgStru);

    if(NAS_EMM_CHK_STAT_INVALID(stMS, stSS))
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_TAUSER_CHKFSMState: STATE ERR !!");
        return NAS_EMM_FAIL;
    }

    if ( NAS_EMM_NULL_PTR == pMsgStru )
    {
        NAS_EMM_SER_LOG_WARN( "NAS_EMM_TAUSER_CHKMsgp: pMsgStru is NULL_PTR !!");
        return NAS_EMM_FAIL;
    }

    return NAS_EMM_SUCC;
}




VOS_VOID  NAS_EMM_MmcSendSerResultIndRej
(
    NAS_EMM_CN_CAUSE_ENUM_UINT8       ucCnCause
)
{
    LMM_MMC_SERVICE_RESULT_IND_STRU     *pstRsltInd = NAS_EMM_NULL_PTR;

    /* ��ӡ����ú����� INFO_LEVEL */
    NAS_EMM_SER_LOG_INFO(               "NAS_EMM_MmcSendSerResultIndRej is entered.");

    /* ����MMC�ڲ���Ϣ */
    pstRsltInd  = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_SERVICE_RESULT_IND_STRU));

    if(NAS_EMM_NULL_PTR                 == pstRsltInd)
    {
        NAS_EMM_SER_LOG_ERR(            "NAS_EMM_SER_SendMmcActionResultReq: MSG ALLOC ERR !!");
        return;
    }

    NAS_LMM_MEM_SET(pstRsltInd, 0, sizeof(LMM_MMC_SERVICE_RESULT_IND_STRU));

    /* ����RRC��DOPRAͷ */
    EMM_PUBU_COMP_MMC_MSG_HEADER(            pstRsltInd,
                                        sizeof(LMM_MMC_SERVICE_RESULT_IND_STRU) -
                                        EMM_LEN_VOS_MSG_HEADER);
    /*���OPID*/


    /* �����ϢID */
    pstRsltInd->ulMsgId                 = ID_LMM_MMC_SERVICE_RESULT_IND;

    /* �����Ϣ����----SERVCIE��� */
    pstRsltInd->ulServiceRst            = MMC_LMM_SERVICE_RSLT_CN_REJ;

    /*�����Ϣ����----�ܾ�ԭ��ֵ*/
    pstRsltInd->bitOpCnCause            = NAS_EMM_BIT_SLCT;
    pstRsltInd->ucCnCause               = ucCnCause;

    /*�����Ϣ����----��������*/
    /* ���ARBOT��ʶ��Ч������CSFB���̣�����д�������� */
    if (NAS_EMM_CSFB_ABORT_FLAG_VALID   != NAS_EMM_SER_GetEmmSerCsfbAbortFlag())
    {
        if (NAS_EMM_SER_START_CAUSE_MO_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
        {
            pstRsltInd->bitOpReqType    = NAS_EMM_BIT_SLCT;
            pstRsltInd->ulReqType       = MMC_LMM_SERVICE_MO_CSFB_1XCSFB;
        }
        else if (NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
        {
            pstRsltInd->bitOpReqType    = NAS_EMM_BIT_SLCT;
            pstRsltInd->ulReqType       = MMC_LMM_SERVICR_MT_CSFB_1XCSFB;
        }
        else if (NAS_EMM_SER_START_CAUSE_MO_EMERGENCY_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
        {
            pstRsltInd->bitOpReqType    = NAS_EMM_BIT_SLCT;
            pstRsltInd->ulReqType       = MMC_LMM_SERVICR_MO_CSFB_1XCSFB_EMERGENCY;
        }
        else
        {
            /* ��CSFB���Ͳ���дulReqType */
        }
    }

    /* ����LMM_MMC_SERVICE_RESULT_IND��Ϣ */
    NAS_LMM_SendLmmMmcMsg(             pstRsltInd);

    return;
}
VOS_VOID  NAS_EMM_MmcSendSerResultIndOtherType
(
    MMC_LMM_SERVICE_RSLT_ENUM_UINT32    ulSerRslt
)
{
    LMM_MMC_SERVICE_RESULT_IND_STRU     *pstRsltInd = NAS_EMM_NULL_PTR;

    /* ��ӡ����ú���*/
    NAS_EMM_SER_LOG_INFO("NAS_EMM_MmcSendSerResultIndOtherType is entered.");

    /* ����MMC�ڲ���Ϣ*/
    pstRsltInd  = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_SERVICE_RESULT_IND_STRU));

    if(NAS_EMM_NULL_PTR                 == pstRsltInd)
    {
        NAS_EMM_SER_LOG_ERR("NAS_EMM_MmcSendSerResultIndOtherType: MSG ALLOC ERR !!");
        return;
    }

    NAS_LMM_MEM_SET(pstRsltInd, 0, sizeof(LMM_MMC_SERVICE_RESULT_IND_STRU));

    /* ����RRC��DOPRAͷ */
    EMM_PUBU_COMP_MMC_MSG_HEADER(       pstRsltInd,
                                        sizeof(LMM_MMC_SERVICE_RESULT_IND_STRU) -
                                        EMM_LEN_VOS_MSG_HEADER);
    /*���OPID*/


    /* �����ϢID */
    pstRsltInd->ulMsgId                 = ID_LMM_MMC_SERVICE_RESULT_IND;

    /* �����Ϣ����----SERVCIE��� */
    if ((MMC_LMM_SERVICE_RSLT_FAILURE== ulSerRslt) ||
        (MMC_LMM_SERVICE_RSLT_ACCESS_BARED == ulSerRslt) ||
        (MMC_LMM_SERVICE_RSLT_AUTH_REJ == ulSerRslt))
    {

        NAS_EMM_SER_LOG1_NORM("NAS_EMM_MmcSendSerResultIndOtherType: ulSerRslt = ",
                                ulSerRslt);
        pstRsltInd->ulServiceRst            = ulSerRslt;
    }
    else
    {
        NAS_EMM_SER_LOG_WARN("NAS_EMM_MmcSendSerResultIndOtherType: ulSerRslt is err! ");
    }

    /*�����Ϣ����----��������*/
    /* ���ARBOT��ʶ��Ч������CSFB���̣�����д�������� */
    if (NAS_EMM_CSFB_ABORT_FLAG_VALID   != NAS_EMM_SER_GetEmmSerCsfbAbortFlag())
    {
        if (NAS_EMM_SER_START_CAUSE_MO_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
        {
            pstRsltInd->bitOpReqType    = NAS_EMM_BIT_SLCT;
            pstRsltInd->ulReqType       = MMC_LMM_SERVICE_MO_CSFB_1XCSFB;
        }
        else if (NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
        {
            pstRsltInd->bitOpReqType    = NAS_EMM_BIT_SLCT;
            pstRsltInd->ulReqType       = MMC_LMM_SERVICR_MT_CSFB_1XCSFB;
        }
        else if (NAS_EMM_SER_START_CAUSE_MO_EMERGENCY_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
        {
            pstRsltInd->bitOpReqType    = NAS_EMM_BIT_SLCT;
            pstRsltInd->ulReqType       = MMC_LMM_SERVICR_MO_CSFB_1XCSFB_EMERGENCY;
        }
        else
        {
            /* ��CSFB���Ͳ���дulReqType */
        }
    }

    /* ����LMM_MMC_SERVICE_RESULT_IND��Ϣ */
    NAS_LMM_SendLmmMmcMsg(             pstRsltInd);

    return;
}



NAS_EMM_SER_START_CAUSE_ENUM_UINT8  NAS_EMM_SER_GetSerStartCause(
                                        VOS_VOID)
{
    return NAS_EMM_SER_GetEmmSERStartCause();
}


VOS_UINT32 NAS_EMM_SER_IsCsfbProcedure( VOS_VOID )
{
    if ((NAS_EMM_SER_START_CAUSE_MO_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
        || (NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
        || (NAS_EMM_SER_START_CAUSE_MO_EMERGENCY_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause()))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_EMM_SER_IsMoCsfbProcedure( VOS_VOID )
{
    if (NAS_EMM_SER_START_CAUSE_MO_CSFB_REQ == NAS_EMM_SER_GetEmmSERStartCause())
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_VOID  NAS_EMM_MmSendCsfbSerPaingInd
(
    NAS_EMM_MT_CSFB_TYPE_ENUM_UINT8     enMtCsfbType,
    const NAS_EMM_CN_CS_SER_NOTIFICAIOTN_STRU *pstCsSerNotification,
    LRRC_LNAS_PAGING_UE_ID_ENUM_UINT32 enPagingUeId
)
{
    LMM_MM_CSFB_SERVICE_PAGING_IND_STRU   *pstMmCsfbSerPaingInd = VOS_NULL_PTR;

    /* ����DOPRA��Ϣ */
    pstMmCsfbSerPaingInd = (VOS_VOID *) NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MM_CSFB_SERVICE_PAGING_IND_STRU));
    if (NAS_LMM_NULL_PTR == pstMmCsfbSerPaingInd)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_EMM_SER_LOG_ERR("NAS_EMM_MmSendCsfbSerPaingInd: MSG ALLOC ERROR!!!");
        return ;
    }

    /* ��� */
    NAS_LMM_MEM_SET(pstMmCsfbSerPaingInd, 0, sizeof(LMM_MM_CSFB_SERVICE_PAGING_IND_STRU));

    /* ���VOS��Ϣͷ */
    EMM_PUBU_COMP_MM_MSG_HEADER((pstMmCsfbSerPaingInd),
                             NAS_EMM_GET_MSG_LENGTH_NO_HEADER(LMM_MM_CSFB_SERVICE_PAGING_IND_STRU));

    /* �����ϢID */
    pstMmCsfbSerPaingInd->ulMsgId    = ID_LMM_MM_CSFB_SERVICE_PAGING_IND;

    /* �����Ϣ�� */
    pstMmCsfbSerPaingInd->ulOpId     = NAS_EMM_OPID_MM;

    if (NAS_EMM_MT_CSFB_TYPE_CS_SER_NOTIFICATION == enMtCsfbType)
    {
        /* ���CLI */
        pstMmCsfbSerPaingInd->bitOpCli = pstCsSerNotification->bitOpCli;
        pstMmCsfbSerPaingInd->ucCliLength = pstCsSerNotification->ucCliLength;
        NAS_LMM_MEM_CPY(    pstMmCsfbSerPaingInd->aucCliValue,
                            pstCsSerNotification->aucCliValue,
                            pstCsSerNotification->ucCliLength);

        /* ���SS CODE */
        pstMmCsfbSerPaingInd->bitOpSsCodeValue = pstCsSerNotification->bitOpSsCode;
        pstMmCsfbSerPaingInd->ucSsCodeValue = pstCsSerNotification->ucSsCodeValue;
        if (0 == pstCsSerNotification->ucPagingIdenity)
        {
            pstMmCsfbSerPaingInd->enPagingUeId = LMM_MM_PAGING_IND_UE_ID_IMSI;
        }
        else
        {
            pstMmCsfbSerPaingInd->enPagingUeId = LMM_MM_PAGING_IND_UE_ID_TMSI;
        }
        
    }
    else
    {
        if(LRRC_LNAS_S_TMSI_LTE == enPagingUeId)
        {
            pstMmCsfbSerPaingInd->enPagingUeId = LMM_MM_PAGING_IND_UE_ID_S_TMSI;
        }
        else if(LRRC_LNAS_IMSI_LTE == enPagingUeId)
        {
            pstMmCsfbSerPaingInd->enPagingUeId = LMM_MM_PAGING_IND_UE_ID_IMSI;
        }
        else
        {

        }
    }

    /* ����DOPRA��Ϣ */
    NAS_LMM_SendLmmMmcMsg((VOS_VOID*)pstMmCsfbSerPaingInd);

    return;
}
VOS_VOID  NAS_EMM_MmSendCsfbSerEndInd
(
    MM_LMM_CSFB_SERVICE_RSLT_ENUM_UINT32     enCsfbSrvRslt
)
{
    LMM_MM_CSFB_SERVICE_END_IND_STRU   *pstMmCsfbSerEndInd = VOS_NULL_PTR;

    /* ����Ѿ��յ�ABORT��Ϣ������֪ͨMM��ֹCSFB���� */
    if (NAS_EMM_CSFB_ABORT_FLAG_VALID == NAS_EMM_SER_GetEmmSerCsfbAbortFlag())
    {
        NAS_EMM_SER_LOG_NORM("NAS_EMM_MmSendCsfbSerEndInd:CSFB is already aborted!");
        return ;
    }

    /* ����DOPRA��Ϣ */
    pstMmCsfbSerEndInd = (VOS_VOID *) NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MM_CSFB_SERVICE_END_IND_STRU));
    if (NAS_LMM_NULL_PTR == pstMmCsfbSerEndInd)
    {
        /* ��ӡ�쳣��ERROR_LEVEL */
        NAS_EMM_SER_LOG_ERR("NAS_EMM_MmSendCsfbSerEndInd: MSG ALLOC ERROR!!!");
        return ;
    }

    /* ��� */
    NAS_LMM_MEM_SET(pstMmCsfbSerEndInd, 0, sizeof(LMM_MM_CSFB_SERVICE_END_IND_STRU));

    /* ���VOS��Ϣͷ */
    EMM_PUBU_COMP_MM_MSG_HEADER((pstMmCsfbSerEndInd),
                             NAS_EMM_GET_MSG_LENGTH_NO_HEADER(LMM_MM_CSFB_SERVICE_END_IND_STRU));

    /* �����ϢID */
    pstMmCsfbSerEndInd->ulMsgId    = ID_LMM_MM_CSFB_SERVICE_END_IND;

    /* �����Ϣ�� */
    pstMmCsfbSerEndInd->ulOpId     = NAS_EMM_OPID_MM;

    pstMmCsfbSerEndInd->enCsfbSrvRslt = enCsfbSrvRslt;

    /* ����DOPRA��Ϣ */
    NAS_LMM_SendLmmMmcMsg((VOS_VOID*)pstMmCsfbSerEndInd);

    return;
}
/*lint +e961*/
/*lint +e960*/

VOS_VOID NAS_EMM_RestartSerProcedural(VOS_VOID)
{
    /* ����SERVICE���� */
    switch (NAS_EMM_SER_GetEmmSERStartCause())
    {
        case NAS_EMM_SER_START_CAUSE_MT_CSFB_REQ:
        case NAS_EMM_SER_START_CAUSE_MO_CSFB_REQ:
        case NAS_EMM_SER_START_CAUSE_MO_EMERGENCY_CSFB_REQ:

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

        default :

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
    }

    return ;
}


/* lihong00150010 emergency tau&service begin */

VOS_VOID  NAS_EMM_EmcPndReqSerAbnormalCommProc
(
    NAS_EMM_SUB_STATE_ENUM_UINT16       ucSs
)
{
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

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


