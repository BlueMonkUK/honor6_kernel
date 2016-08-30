


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include  "NasEsmNwMsgProc.h"
#include  "NasCommBuffer.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASESMNWMSGPROC_C
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
 Function Name   : NAS_ESM_ValidateTftOpCreateTftRsltProc
 Description     : �Գ����޸ĵ�TAD��ΪCreate TFT ������Ĵ���
 Input           : enRslt          -- �����
                   pstEsmNwMsgIE   -- �����Ŀտ���Ϣָ��
                   ulStateTblIndex -- ��̬������
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.liuwenyu00143951      2010-5-14  Draft Enact

*****************************************************************************/
VOS_UINT32 NAS_ESM_ValidateTftOpCreateTftRsltProc
(
    const NAS_ESM_NW_MSG_STRU          *pstEsmNwMsgIE,
    NAS_ESM_ENCODE_INFO_STRU           *pstEncodeInfo,
    NAS_ESM_QUERY_CID_RSLT_ENUM_UINT8   enRslt,
    VOS_UINT32                          ulStateTblIndex
)
{
    (VOS_VOID)enRslt;

    /* Create TFTʱ,��Ӧ��ר�г��ص�TFT���ܴ��� */
    if (NAS_ESM_BEARER_TYPE_DEDICATED == NAS_ESM_GetBearCntxtType(pstEsmNwMsgIE->ucEpsbId))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ValidateTftOpCreateTftRsltProc:The created Ttf already exist!");
        pstEncodeInfo->ucESMCau = NAS_ESM_CAUSE_SEMANTIC_ERR_IN_TFT;
        NAS_ESM_BearModFailProc(pstEncodeInfo, ulStateTblIndex);
        return NAS_ESM_MSG_HANDLED;
    }

    return NAS_ESM_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_ValidateTftOpNoOpRsltProc
 Description     : �Գ����޸ĵ�TAD��ΪNo operation ������Ĵ���
 Input           : enRslt          -- �����
                   pstEsmNwMsgIE   -- �����Ŀտ���Ϣָ��
                   ulStateTblIndex -- ��̬������
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.liuwenyu00143951      2010-5-14  Draft Enact

*****************************************************************************/
VOS_UINT32 NAS_ESM_ValidateTftOpNoOpRsltProc
(
    const NAS_ESM_NW_MSG_STRU          *pstEsmNwMsgIE,
    NAS_ESM_ENCODE_INFO_STRU           *pstEncodeInfo,
    NAS_ESM_QUERY_CID_RSLT_ENUM_UINT8   enRslt,
    VOS_UINT32                          ulStateTblIndex
)
{
    (VOS_VOID)pstEsmNwMsgIE;

    /* �޸�Qosʱ,��Ӧ��filter����ȫ���ҵ����߲����ҵ���������ͬһCID�� */
    if (NAS_ESM_QUERY_CID_RSLT_ALL_UNFOUND == enRslt)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ValidateTftOpNoOpRsltProc:Tad code error!");
        pstEncodeInfo->ucESMCau = NAS_ESM_CAUSE_SEMANTIC_ERR_IN_TFT;
        NAS_ESM_BearModFailProc(pstEncodeInfo, ulStateTblIndex);
        return NAS_ESM_MSG_HANDLED;
    }

    return NAS_ESM_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_CreatePfPrecedenceValidateList
 Description     : ��ȡĳ������packet filter���ȼ���֤�б�
 Input           : ucEpsbId------------------���غ�
 Output          : pucPfNum------------------packet filter��
                   pstPfPVList---------------���ȼ���֤�б��׵�ַ
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-5-27  Draft Enact

*****************************************************************************/
/*lint -e960*/
/*lint -e961*/
VOS_VOID  NAS_ESM_GetPfPrecedenceValidateListInBearer
(
    VOS_UINT8                           ucEpsbId,
    VOS_UINT8                          *pucPfNum,
    NAS_ESM_PF_PRCDNC_VLDT_STRU        *pstPfPVList
)
{
    NAS_ESM_EPSB_CNTXT_INFO_STRU       *pstEspbCntxtInfo    = VOS_NULL_PTR;
    NAS_ESM_CONTEXT_TFT_STRU           *pstPacketFilter     = VOS_NULL_PTR;
    NAS_ESM_SDF_CNTXT_INFO_STRU        *pstSdfCntxtInfo     = VOS_NULL_PTR;
    VOS_UINT32                          ulCnt1              = NAS_ESM_NULL;
    VOS_UINT32                          ulCnt2              = NAS_ESM_NULL;

    pstEspbCntxtInfo = NAS_ESM_GetEpsbCntxtInfoAddr(ucEpsbId);

    NAS_ESM_INFO_LOG1("Show all PF of specefic EPSB ID below:",ucEpsbId);

    for(ulCnt1 = NAS_ESM_MIN_CID; ulCnt1 <= NAS_ESM_MAX_CID; ulCnt1++)
    {
        if(NAS_ESM_OP_TRUE !=((pstEspbCntxtInfo->ulBitCId >> ulCnt1 )& NAS_ESM_BIT_0))
        {
            continue;
        }

        pstSdfCntxtInfo = NAS_ESM_GetSdfCntxtInfo(ulCnt1);

        for (ulCnt2 = NAS_ESM_NULL; ulCnt2 < pstSdfCntxtInfo->ulSdfPfNum; ulCnt2++)
        {
            pstPacketFilter = &pstSdfCntxtInfo->astSdfPfInfo[ulCnt2];
            pstPfPVList[*pucPfNum].ucEpsbId = (VOS_UINT8)pstSdfCntxtInfo->ulEpsbId;
            pstPfPVList[*pucPfNum].ucPfId = pstPacketFilter->ucNwPacketFilterId;
            pstPfPVList[*pucPfNum].ucPrecedence = pstPacketFilter->ucPrecedence;
            /* ����TFT��ʱ����֤���ȼ���Ƚ�Cid */
            pstPfPVList[*pucPfNum].ucCid = (VOS_UINT8)ulCnt1;
            NAS_ESM_INFO_LOG2("CID: Precedence:", ulCnt1, pstPacketFilter->ucPrecedence);
            (*pucPfNum)++;
        }
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_ValidateDelPfRenderEmpty
 Description     : ��֤delete packet filter�Ƿ�ᵼ��ר�г��ص�TFTΪ��
 Input           : pstNwMsg-------------------------�����Ŀտ���Ϣ
 Output          : None
 Return          : NAS_ESM_CAUSE_ENUM_UINT8

 History         :
    1.lihong00150010      2010-5-28  Draft Enact

*****************************************************************************/
NAS_ESM_CAUSE_ENUM_UINT8  NAS_ESM_ValidateDelPfRenderEmpty
(
    const NAS_ESM_NW_MSG_STRU          *pstNwMsg
)
{
    VOS_UINT8                           ucPfPVNum       = NAS_ESM_NULL;
    VOS_UINT8                           ucCnt1          = NAS_ESM_NULL;
    VOS_UINT8                           ucCnt2          = NAS_ESM_NULL;
    VOS_UINT8                           ucNumTmp        = NAS_ESM_NULL;
    NAS_ESM_PF_PRCDNC_VLDT_STRU         astPfPVListTmp[NAS_ESM_MAX_SDF_PF_NUM]= {0};
    const NAS_ESM_CONTEXT_TFT_STRU     *pstPacketFilter = VOS_NULL_PTR;

    if (PS_TRUE == NAS_ESM_IsDefaultEpsBearerType(NAS_ESM_GetBearCntxtType(pstNwMsg->ucEpsbId)))
    {
        return NAS_ESM_CAUSE_SUCCESS;
    }

    if (NAS_ESM_TFT_OP_TYPE_DELETE_FILTER != pstNwMsg->stTadInfo.enTftOpType)
    {
        return NAS_ESM_CAUSE_SUCCESS;
    }

    /* ��ȡ�˳�����packet filter���ȼ���֤�б� */
    NAS_ESM_GetPfPrecedenceValidateListInBearer(pstNwMsg->ucEpsbId,
                                                &ucPfPVNum,
                                                astPfPVListTmp);

    /* �տ���Ϣ��Я����packet filter��ĿС�ڳ�������ӵ�е�packet filter������
       �����ܽ������µ�����packet filterȫ��ɾ�� */
    if (pstNwMsg->stTadInfo.ucSdfPfNum < ucPfPVNum)
    {
        return NAS_ESM_CAUSE_SUCCESS;
    }

    /* �жϳ�������ӵ�е�����packet filter�Ƿ�ᱻȫ��ɾ�� */
    for (ucCnt1 = NAS_ESM_NULL; ucCnt1 < pstNwMsg->stTadInfo.ucSdfPfNum; ucCnt1++)
    {
        pstPacketFilter = &pstNwMsg->stTadInfo.astSdfPfInfo[ucCnt1];
        for (ucCnt2 = NAS_ESM_NULL; ucCnt2 < ucPfPVNum; ucCnt2++)
        {
            if (pstPacketFilter->ucNwPacketFilterId == astPfPVListTmp[ucCnt2].ucPfId)
            {
                ucNumTmp++;
            }
        }
    }

    if (ucNumTmp == ucPfPVNum)
    {
        return NAS_ESM_CAUSE_SEMANTIC_ERR_IN_TFT;
    }

    return NAS_ESM_CAUSE_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_ValidateTftOpDelTftOrDelPfRsltProc
 Description     : �Գ����޸ĵ�TAD��ΪDelete filter,Delete TFT
                   ������Ĵ���
 Input           : enRslt          -- �����
                   pstEsmNwMsgIE   -- �����Ŀտ���Ϣָ��
                   ulStateTblIndex -- ��̬������
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.liuwenyu00143951      2010-5-14  Draft Enact

*****************************************************************************/
VOS_UINT32 NAS_ESM_ValidateTftOpDelTftOrDelPfRsltProc
(
    const NAS_ESM_NW_MSG_STRU          *pstEsmNwMsgIE,
    NAS_ESM_ENCODE_INFO_STRU           *pstEncodeInfo,
    NAS_ESM_QUERY_CID_RSLT_ENUM_UINT8   enRslt,
    VOS_UINT32                          ulStateTblIndex
)
{
    VOS_UINT32                          ulSndNwMsgLen   = NAS_ESM_NULL;
    NAS_ESM_CAUSE_ENUM_UINT8            enEsmCause      = NAS_ESM_NULL;


    /* ���û�ҵ�CID���ҵ�ǰTAD������ΪDELETE_FILTER����DELETE TFT����ظ��޸ĳ��سɹ� */
    if (NAS_ESM_QUERY_CID_RSLT_ALL_UNFOUND == enRslt)
    {
        NAS_ESM_INFO_LOG("NAS_ESM_ValidateTftOpDelTftOrDelPfRsltProc: accept delete not exist filter !");

        /*������ظ�Modify EPS bearer context accept*/
        NAS_ESM_EncodeModEpsbCntxtAccMsg(pstEncodeInfo, \
                                 NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                                &ulSndNwMsgLen);
        NAS_ESM_SndAirMsgReportInd(NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                                   ulSndNwMsgLen, NAS_ESM_AIR_MSG_DIR_ENUM_UP,\
                                   ESM_MOD_EPS_BEARER_CNTXT_ACP);
        NAS_ESM_SndEsmEmmDataReqMsg(NAS_ESM_ILLEGAL_OPID, PS_FALSE, ulSndNwMsgLen, NAS_ESM_GetCurEsmSndNwMsgAddr());
        NAS_ESM_UpdateEsmSndNwMsgLen(ulSndNwMsgLen);

        return NAS_ESM_MSG_HANDLED;
    }

    /* ר�г��ز���ɾ��TFT */
    if ((NAS_ESM_TFT_OP_TYPE_DELETE_TFT == pstEsmNwMsgIE->stTadInfo.enTftOpType)
        && (NAS_ESM_BEARER_TYPE_DEDICATED == NAS_ESM_GetBearCntxtType(pstEsmNwMsgIE->ucEpsbId)))
    {
        /*��ӡ��Ϣ*/
        NAS_ESM_WARN_LOG("NAS_ESM_ValidateTftOpDelTftOrDelPfRsltProc:Illegal TAD Operation !");

        pstEncodeInfo->ucESMCau = NAS_ESM_CAUSE_SEMANTIC_ERR_IN_TFT;
        NAS_ESM_BearModFailProc(pstEncodeInfo, ulStateTblIndex);
        return NAS_ESM_MSG_HANDLED;
    }

    /* ��֤delete packet filter�Ƿ�ᵼ��ר�г��ص�TFTΪ�գ����ǻظ��ܾ���Ϣ��
       ԭ��ֵΪNAS_ESM_CAUSE_SEMANTIC_ERR_IN_TFT */
    enEsmCause = NAS_ESM_ValidateDelPfRenderEmpty(pstEsmNwMsgIE);
    if (enEsmCause != NAS_ESM_CAUSE_SUCCESS)
    {
        /*��ӡ��Ϣ*/
        NAS_ESM_WARN_LOG("NAS_ESM_ValidateTftOpDelTftOrDelPfRsltProc:NAS_ESM_ValidateDelPfRenderEmpty failed!");

        pstEncodeInfo->ucESMCau = enEsmCause;
        NAS_ESM_BearModFailProc(pstEncodeInfo, ulStateTblIndex);
        return NAS_ESM_MSG_HANDLED;
    }

    return NAS_ESM_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_ValidateTftOpRsltProc
 Description     : �Գ����޸ĵ�TAD�������Ĵ���
 Input           : enRslt          -- �����
                   pstEsmNwMsgIE   -- �����Ŀտ���Ϣָ��
                   ulStateTblIndex -- ��̬������
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.liuwenyu00143951      2010-5-14  Draft Enact

*****************************************************************************/
VOS_UINT32 NAS_ESM_ValidateTftOpRsltProc
(
    const NAS_ESM_NW_MSG_STRU          *pstEsmNwMsgIE,
    NAS_ESM_QUERY_CID_RSLT_ENUM_UINT8   enRslt,
    NAS_ESM_ENCODE_INFO_STRU           *pstEncodeInfo,
    VOS_UINT32                          ulStateTblIndex
)
{
    /* �޸ĳ���,�޸ĵ�filter������ͬһ��Sdf��*/
    if ((NAS_ESM_QUERY_CID_RSLT_ALL_FOUND_UNEQUAL_CID == enRslt)
     || (NAS_ESM_QUERY_CID_RSLT_PART_FOUND_UNEQUAL_CID == enRslt))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ValidateTftOpRsltProc:Mod filter not in Same Cid!");
        pstEncodeInfo->ucESMCau = NAS_ESM_CAUSE_SEMANTIC_ERR_IN_TFT;
        NAS_ESM_BearModFailProc(pstEncodeInfo, ulStateTblIndex);
        return NAS_ESM_MSG_HANDLED;
    }

    if (NAS_ESM_OP_TRUE != pstEsmNwMsgIE->bitOpTadInfo)
    {
        return NAS_ESM_SUCCESS;
    }

    switch(pstEsmNwMsgIE->stTadInfo.enTftOpType)
    {
        case NAS_ESM_TFT_OP_TYPE_CREATE_TFT:
            return NAS_ESM_ValidateTftOpCreateTftRsltProc(pstEsmNwMsgIE,
                                                          pstEncodeInfo,
                                                          enRslt,
                                                          ulStateTblIndex);
        case NAS_ESM_TFT_OP_TYPE_DELETE_TFT:
        case NAS_ESM_TFT_OP_TYPE_DELETE_FILTER:
            return NAS_ESM_ValidateTftOpDelTftOrDelPfRsltProc(pstEsmNwMsgIE,
                                                              pstEncodeInfo,
                                                              enRslt,
                                                              ulStateTblIndex);

        case NAS_ESM_TFT_OP_TYPE_NO_OP:
            return NAS_ESM_ValidateTftOpNoOpRsltProc(pstEsmNwMsgIE,
                                                     pstEncodeInfo,
                                                     enRslt,
                                                     ulStateTblIndex);
        default:
             break;
    }

    return NAS_ESM_SUCCESS;
}
VOS_UINT32 NAS_ESM_ValidateTftOpInModMsg
(
    const NAS_ESM_NW_MSG_STRU          *pstEsmNwMsgIE,
    NAS_ESM_ENCODE_INFO_STRU           *pstEncodeInfo,
    VOS_UINT32                          ulStateTblIndex
)
{
    VOS_UINT32                          ulCId               = NAS_ESM_NULL;
    NAS_ESM_QUERY_CID_RSLT_ENUM_UINT8   enRslt              = NAS_ESM_QUERY_CID_RSLT_BUTT;

    enRslt = NAS_ESM_QuerySdfId(pstEsmNwMsgIE,&ulCId);


    return NAS_ESM_ValidateTftOpRsltProc(pstEsmNwMsgIE,
                                         enRslt,
                                         pstEncodeInfo,
                                         ulStateTblIndex);
}

VOS_VOID NAS_ESM_ProcEsmStatusCauseInvalidPtiOrNonExistMsg
(
    const NAS_ESM_NW_MSG_STRU          *pstEsmNwMsgIE
)
{
    VOS_UINT32                          ulStateTblId        = NAS_ESM_NULL;

    if (pstEsmNwMsgIE->ucPti == NAS_ESM_PTI_UNASSIGNED_VALUE)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ProcEsmStatusCauseInvalidPtiOrNonExistMsg:WARN:PTI is unassigned!");
        return ;
    }

    /*����PTI���ҵ���̬��Index*/
    if(NAS_ESM_SUCCESS == NAS_ESM_QueryStateTblIndexByPti(  pstEsmNwMsgIE->ucPti,
                                                            &ulStateTblId))
    {
        switch(NAS_ESM_GetStateTblAppMsgType(ulStateTblId))
        {
            case ID_APP_ESM_PDP_SETUP_REQ:
                NAS_ESM_SndEsmAppSdfSetupCnfFailMsg(ulStateTblId,
                        NAS_ESM_GetAppErrorByEsmCause(pstEsmNwMsgIE->enEsmCau));
                break;
            case ID_APP_ESM_PDP_RELEASE_REQ:
                NAS_ESM_SndEsmAppSdfRelCnfFailMsg(  ulStateTblId,
                        NAS_ESM_GetAppErrorByEsmCause(pstEsmNwMsgIE->enEsmCau));
                break;
            case ID_APP_ESM_PDP_MODIFY_REQ:
                NAS_ESM_SndEsmAppSdfModCnfFailMsg(  ulStateTblId,
                        NAS_ESM_GetAppErrorByEsmCause(pstEsmNwMsgIE->enEsmCau));
                break;
            case ID_APP_ESM_NDISCONN_REQ:
                NAS_ESM_LOG1("NAS_ESM_ProcEsmStatusCauseInvalidPtiOrNonExistMsg:ERROR =", NAS_ESM_GetAppErrorByEsmCause(pstEsmNwMsgIE->enEsmCau));
                NAS_ESM_SndEsmAppNdisConnCnfFailMsg(ulStateTblId,
                        NAS_ESM_GetAppErrorByEsmCause(pstEsmNwMsgIE->enEsmCau));
                break;
            default:
                NAS_ESM_NORM_LOG("NAS_ESM_ProcEsmStatusCauseInvalidPtiOrNonExistMsg: Error: Illegal App Msg Type!");
                break;
        }

        /*�ͷ�״̬����Դ*/
        NAS_ESM_RelStateTblResource(ulStateTblId);
    }
}


VOS_VOID NAS_ESM_ProcEsmStatusCauseInvalidEpsbId
(
    const NAS_ESM_NW_MSG_STRU          *pstEsmNwMsgIE
)
{
    VOS_UINT32                          ulStateTblId        = NAS_ESM_NULL;

    /* ��PTI�Ѵ�����״̬������ֹͣ���� */
    NAS_ESM_ProcEsmStatusCauseInvalidPtiOrNonExistMsg(pstEsmNwMsgIE);

    if((pstEsmNwMsgIE->ucEpsbId < NAS_ESM_MIN_EPSB_ID)
        ||(pstEsmNwMsgIE->ucEpsbId > NAS_ESM_MAX_EPSB_ID))
    {
        return ;
    }

    /*֪ͨAPP�ͷ�ȱʡ���ؼ����������*/
    NAS_ESM_DeactBearerAndInformApp(pstEsmNwMsgIE->ucEpsbId);

    /*֪ͨEMM��ǰ����״̬��Ϣ������ID_EMM_ESM_BEARER_STATUS_REQ*/
    NAS_ESM_SndEsmEmmBearerStatusReqMsg(EMM_ESM_BEARER_CNTXT_MOD_MUTUAL);

    /*����ulEpsbId����StatTbl��ID*/
    if(NAS_ESM_SUCCESS == NAS_ESM_QueryStateTblIndexByEpsbId(pstEsmNwMsgIE->ucEpsbId,&ulStateTblId))
    {
        /*�ͷ�״̬����Դ*/
        NAS_ESM_RelStateTblResource(ulStateTblId);
    }
}
VOS_VOID  NAS_ESM_ProcValidNwsMsgEsmStatus
(
    const NAS_ESM_NW_MSG_STRU          *pstEsmNwMsgIE
)
{
    NAS_ESM_NORM_LOG("NAS_ESM_ProcValidNwsMsgEsmStatus is entered!");

    /* ������ESM CAUSE��Ԫ����ֱ�ӷ��� */
    if(NAS_ESM_OP_TRUE != pstEsmNwMsgIE->bitOpEsmCause)
    {
        return;
    }

    switch (pstEsmNwMsgIE->enEsmCau)
    {
        case NAS_ESM_CAUSE_MESSAGE_TYPE_NON_EXIST_OR_NOT_IMPLEMENTED:
        case NAS_ESM_CAUSE_INVALID_PTI_VALUE:
            NAS_ESM_ProcEsmStatusCauseInvalidPtiOrNonExistMsg(pstEsmNwMsgIE);
            break;
        case NAS_ESM_CAUSE_INVALID_EPS_BEARER_IDENTITY:
            NAS_ESM_ProcEsmStatusCauseInvalidEpsbId(pstEsmNwMsgIE);
            break;
        default:
            break;
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_ProcNwMsgDeactProcedureCollision
 Description     : ���ͬһ���أ����緢���������UE��������̲�����ͻ��
                   ������ʵ�����̳�ͻ�����ܣ���״̬�������в��ٴ����ͻ����
 Input           : EMM_ESM_INTRA_DATA_IND_STRU *pRcvMsg
 Output          : None
 Return          : VOS_VOID

 History         :
    1.sunbing49683      2009-2-20  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_ProcNwMsgDeactProcedureCollision
(
    VOS_UINT32                          ulStateTblId,
    const NAS_ESM_NW_MSG_STRU           *pRcvMsg
)
{
    VOS_UINT32                          ulEpsbId            = NAS_ESM_NULL;
    VOS_UINT32                          ulSndNwMsgLen       = NAS_ESM_NULL;
    NAS_ESM_ENCODE_INFO_STRU            stEncodeInfo;
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr        = VOS_NULL_PTR;

    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG("NAS_ESM_ProcNwMsgDeactProcedureCollision is entered.");

    ulEpsbId = (VOS_UINT32)pRcvMsg->ucEpsbId;

    pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblId);

    /*�����ǰ���ش���PDNȥ���ӹ����У�����ȥ�������̣���ֹPDNȥ��������*/
    if(NAS_ESMCN_MSG_TYPE_PDN_DISCONNECT_REQ == \
                                    pstStateAddr->stNwMsgRecord.enEsmCnMsgType)

    {
        /*֪ͨAPP�ͷ�ȱʡ���ؼ����������*/
        NAS_ESM_DeactBearerAndInformApp(ulEpsbId);

        /*֪ͨEMM��ǰ����״̬��Ϣ������ID_EMM_ESM_BEARER_STATUS_REQ*/
        NAS_ESM_SndEsmEmmBearerStatusReqMsg(EMM_ESM_BEARER_CNTXT_MOD_MUTUAL);

        /*�ͷ�״̬����Դ*/
        NAS_ESM_RelStateTblResource(ulStateTblId);
    }
    /*�����ǰ���ش��ڳ����޸����������(ר�г����ͷ�)������ȥ�������̣���ֹ������Դ�޸�����*/
    else if(NAS_ESMCN_MSG_TYPE_BEARER_RES_MOD_REQ == \
                                pstStateAddr->stNwMsgRecord.enEsmCnMsgType)
    {
        /*֪ͨAPP�ͷ�ȱʡ���ؼ����������*/
        NAS_ESM_DeactBearerAndInformApp(ulEpsbId);

        /*֪ͨEMM��ǰ����״̬��Ϣ������ID_EMM_ESM_BEARER_STATUS_REQ*/
        NAS_ESM_SndEsmEmmBearerStatusReqMsg(EMM_ESM_BEARER_CNTXT_MOD_MUTUAL);

        /*�ͷ�״̬����Դ*/
        NAS_ESM_RelStateTblResource(ulStateTblId);
    }
    else
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgDeactProcedureCollision:WARNNING:Undefine Collision Process!");
        return;
    }

    /*��ʼ���������NAS_ESM_ENCODE_INFO_STRU*/
    NAS_ESM_MEM_SET((VOS_VOID*)&stEncodeInfo, 0, sizeof(NAS_ESM_ENCODE_INFO_STRU));
    stEncodeInfo.bitOpESMCau = NAS_ESM_OP_FALSE;
    stEncodeInfo.ucPti       = pRcvMsg->ucPti;
    stEncodeInfo.ulEpsbId    = ulEpsbId;

    /*������ظ�Deactivate EPS bearer context accept*/
    NAS_ESM_EncodeDeactEpsbCntxtAccMsg(stEncodeInfo, \
                               NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                              &ulSndNwMsgLen);
    /*�տ���Ϣ�ϱ�*/
    NAS_ESM_SndAirMsgReportInd(NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                                       ulSndNwMsgLen, NAS_ESM_AIR_MSG_DIR_ENUM_UP,\
                                       ESM_DEACT_EPS_BEARER_CNTXT_ACP);
    NAS_ESM_SndEsmEmmDataReqMsg(NAS_ESM_ILLEGAL_OPID, PS_FALSE, ulSndNwMsgLen, NAS_ESM_GetCurEsmSndNwMsgAddr());
    NAS_ESM_UpdateEsmSndNwMsgLen(ulSndNwMsgLen);

}

/*****************************************************************************
 Function Name   : NAS_ESM_SndRejAirMsg
 Description     : ���͸���REJ�տ���Ϣ
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-4-12  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ESM_SndRejAirMsg
(
    NAS_ESMCN_MSG_TYPE_ENUM_UINT8       enEsmCnMsgType,
    const NAS_ESM_ENCODE_INFO_STRU     *pstEncodeInfo
)
{
    VOS_UINT32                          ulSndNwMsgLen       = NAS_ESM_NULL;

    switch(enEsmCnMsgType)
    {
        case NAS_ESMCN_MSG_TYPE_ACT_DEDICTD_EPS_BEARER_CNTXT_REQ:
            NAS_ESM_EncodeActDedictdEpsbCntxtRejMsg(    *pstEncodeInfo,\
                                                        NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                                                        &ulSndNwMsgLen);
            NAS_ESM_SndAirMsgReportInd(NAS_ESM_GetCurEsmSndNwMsgAddr(),ulSndNwMsgLen,
                                       NAS_ESM_AIR_MSG_DIR_ENUM_UP,
                                       ESM_ACT_DEDICT_EPS_BEARER_CNTXT_REJ);
            break;

        case NAS_ESMCN_MSG_TYPE_ACT_DEFLT_EPS_BEARER_CNTXT_REQ:
            NAS_ESM_EncodeActDefltEpsbCntxtRejMsg(  *pstEncodeInfo,\
                                                    NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                                                    &ulSndNwMsgLen);
            NAS_ESM_SndAirMsgReportInd(NAS_ESM_GetCurEsmSndNwMsgAddr(),ulSndNwMsgLen,
                                       NAS_ESM_AIR_MSG_DIR_ENUM_UP,
                                       ESM_ACT_DEFLT_EPS_BEARER_CNTXT_REJ);
            break;

        case NAS_ESMCN_MSG_TYPE_MOD_EPS_BEARER_CNTXT_REQ:
            NAS_ESM_EncodeModEpsbCntxtRejMsg(   *pstEncodeInfo, \
                                                NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                                                &ulSndNwMsgLen);
            NAS_ESM_SndAirMsgReportInd(NAS_ESM_GetCurEsmSndNwMsgAddr(),ulSndNwMsgLen,
                                       NAS_ESM_AIR_MSG_DIR_ENUM_UP,
                                       ESM_MOD_EPS_BEARER_CNTXT_REJ);
            break;
        case NAS_ESMCN_MSG_TYPE_ESM_INFORMATION_REQ:
            NAS_ESM_EncodeEsmStatus(    *pstEncodeInfo, \
                                        NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                                        &ulSndNwMsgLen);
            NAS_ESM_SndAirMsgReportInd(NAS_ESM_GetCurEsmSndNwMsgAddr(),ulSndNwMsgLen,
                                       NAS_ESM_AIR_MSG_DIR_ENUM_UP,
                                       ESM_STUS_MO);
            break;
        default:
            NAS_ESM_NORM_LOG("NAS_ESM_SndRejAirMsg:NORM: Ignore NW msg");
            return;
    }

    NAS_ESM_SndEsmEmmDataReqMsg(NAS_ESM_ILL_CID, PS_FALSE, ulSndNwMsgLen, NAS_ESM_GetCurEsmSndNwMsgAddr());
    NAS_ESM_UpdateEsmSndNwMsgLen(ulSndNwMsgLen);
}

/*sunbing 49683 2013-10-14 VoLTE begin*/
/*****************************************************************************
 Function Name   : NAS_ESM_SndApsEsmNotificationIndMsg
 Description     : ��APS����ESM Notification ind��Ϣ
 Input           :
 Output          : None
 Return          : VOS_VOID

 History         :
    1.sunbing 49683      2013-10-14  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_SndApsEsmNotificationIndMsg
(
    ESM_APS_NOTIFICATION_IND_ENUM_UINT32   enNotificationIndValue
)
{
    ESM_APS_NOTIFICATION_IND_STRU    *pstNotificationInd  = VOS_NULL_PTR;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstNotificationInd = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(ESM_APS_NOTIFICATION_IND_STRU));

    if ( VOS_NULL_PTR == pstNotificationInd )
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_SndApsEsmNotificationIndMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    NAS_ESM_MEM_SET(NAS_ESM_GET_MSG_ENTITY(pstNotificationInd), 0, NAS_ESM_GET_MSG_LENGTH(pstNotificationInd));

    pstNotificationInd->enNotificationIndicator = enNotificationIndValue;

    /*��д��Ϣͷ*/
    NAS_ESM_WRITE_SM_MSG_HEAD(pstNotificationInd, ID_ESM_APS_ESM_NOTIFICATION_IND);

    /* ������Ϣ���ͺ��� */
    NAS_ESM_SND_MSG(pstNotificationInd);
}
/*sunbing 49683 2013-10-14 VoLTE begin*/


/*****************************************************************************
 Function Name   : NAS_ESM_ProcNwMsgForIllegalPti
 Description     : ��������Я����PTI���Ϸ���UE����������Ϣ���ݷ��ܾ���Ϣ��ֱ
                   �Ӻ���
 Input           : NAS_ESM_NW_MSG_STRU stEsmNwMsgIE
 Output          : None
 Return          : VOS_VOID

 History         :
    1.sunbing49683      2009-2-19  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_ProcNwMsgForIllegalPti
(
    const NAS_ESM_NW_MSG_STRU          *pstEsmNwMsgIE,
    NAS_ESM_CAUSE_ENUM_UINT8            enEsmCause
)
{
    NAS_ESM_ENCODE_INFO_STRU            stEncodeInfo;
    VOS_UINT32                          ulEsmBuffIndex      = NAS_ESM_NULL;
    VOS_UINT8                           ucPti               = pstEsmNwMsgIE->ucPti;
    VOS_UINT32                          ulMsgId             = NAS_ESM_NULL;
    NAS_ESM_PTI_BUFF_ITEM_STRU         *pstPtiBuffItem      = VOS_NULL_PTR;

    /*��ʼ���������NAS_ESM_ENCODE_INFO_STRU*/
    NAS_ESM_MEM_SET((VOS_VOID*)&stEncodeInfo, 0, sizeof(NAS_ESM_ENCODE_INFO_STRU));
    stEncodeInfo.bitOpESMCau = NAS_ESM_OP_TRUE;
    stEncodeInfo.ucESMCau    = enEsmCause;
    stEncodeInfo.ucPti       = pstEsmNwMsgIE->ucPti;
    stEncodeInfo.ulEpsbId    = (VOS_UINT32)pstEsmNwMsgIE->ucEpsbId;

    if (enEsmCause != NAS_ESM_CAUSE_PTI_MISMATCH)
    {
        /*����REJ��Ϣ*/
        NAS_ESM_SndRejAirMsg(pstEsmNwMsgIE->enEsmCnMsgType, &stEncodeInfo);

        return ;
    }

    /* ����PTI����ESM�����¼ */
    if (NAS_ESM_SUCCESS != NAS_ESM_QueryPtiBuffItemByPti(   ucPti,
                                                            &ulEsmBuffIndex))
    {
        NAS_ESM_LOG1("NAS_ESM_ProcNwMsgForIllegalPti:WARNING:PTI not matched!",ucPti);
    
        /*����REJ��Ϣ*/
        NAS_ESM_SndRejAirMsg(pstEsmNwMsgIE->enEsmCnMsgType, &stEncodeInfo);

        return ;
    }

    NAS_ESM_INFO_LOG2("NAS_ESM_ProcNwMsgForIllegalPti:NORM:PTI is found in Esm Buffer!",
                 ucPti, ulEsmBuffIndex);

    /* ��ȡ�����¼ */
    pstPtiBuffItem = (NAS_ESM_PTI_BUFF_ITEM_STRU*)
                            NAS_ESM_GetBuffItemAddr(NAS_ESM_BUFF_ITEM_TYPE_PTI, ulEsmBuffIndex);

    if (pstPtiBuffItem == VOS_NULL_PTR)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgForIllegalPti:WARN:Get buffer item failed!");
        return ;
    }

    /* ԭ������Ϣ���ʧ�ܣ���ԭ��ֵΪNAS_ESM_CAUSE_ERRORʱ������û�лظ���Ϣ */
    if (pstPtiBuffItem->stEsmMsg.ulEsmMsgSize == NAS_ESM_NULL)
    {
        return ;
    }

    /* ��ȡ��ϢID */
    ulMsgId = pstPtiBuffItem->stEsmMsg.aucEsmMsg[NAS_ESM_MSGID_OFFSET];

    /* �����ǰ״̬ΪATTACHING̬���һظ���ϢΪȱʡ���ؼ���ɹ���Ϣ�����
       MM����EMM_ESM_PDN_CON_SUCC_REQ��Ϣ��������EMM_ESM_DATA_REQ��Ϣ*/
    if ((ulMsgId == NAS_ESMCN_MSG_TYPE_ACT_DEFLT_EPS_BEARER_CNTXT_ACC)
        && (NAS_ESM_PS_REGISTER_STATUS_ATTACHING == NAS_ESM_GetEmmStatus()))
    {
        NAS_ESM_SndEsmEmmPdnConSuccReqMsg(pstPtiBuffItem->stEsmMsg.ulEsmMsgSize,
                                          pstPtiBuffItem->stEsmMsg.aucEsmMsg);
    }
    else
    {
        NAS_ESM_SndEsmEmmDataReqMsg(NAS_ESM_ILLEGAL_OPID,
                                    PS_FALSE,
                                    pstPtiBuffItem->stEsmMsg.ulEsmMsgSize,
                                    pstPtiBuffItem->stEsmMsg.aucEsmMsg);
    }
}


VOS_VOID  NAS_ESM_ProcNwMsgForIllegalEpsbId
(
    const NAS_ESM_NW_MSG_STRU          *pstEsmNwMsgIE
)
{
    NAS_ESM_ENCODE_INFO_STRU            stEncodeInfo;
    VOS_UINT32                          ulSndNwMsgLen = NAS_ESM_NULL;

    /*��ʼ���������NAS_ESM_ENCODE_INFO_STRU*/
    NAS_ESM_MEM_SET((VOS_VOID*)&stEncodeInfo, 0, sizeof(NAS_ESM_ENCODE_INFO_STRU));
    stEncodeInfo.bitOpESMCau = NAS_ESM_OP_TRUE;
    stEncodeInfo.ucPti       = pstEsmNwMsgIE->ucPti;
    stEncodeInfo.ulEpsbId    = (VOS_UINT32)pstEsmNwMsgIE->ucEpsbId;
    stEncodeInfo.ucESMCau    = NAS_ESM_CAUSE_INVALID_EPS_BEARER_IDENTITY;

    if (pstEsmNwMsgIE->enEsmCnMsgType == NAS_ESMCN_MSG_TYPE_DEACT_EPS_BEARER_CNTXT_REQ)
    {
        /*������ظ�Deact EPS bearer context accept*/
        NAS_ESM_EncodeDeactEpsbCntxtAccMsg(stEncodeInfo, \
                                 NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                                &ulSndNwMsgLen);
        NAS_ESM_SndAirMsgReportInd(NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                                   ulSndNwMsgLen, NAS_ESM_AIR_MSG_DIR_ENUM_UP,\
                                   ESM_DEACT_EPS_BEARER_CNTXT_ACP);
        NAS_ESM_SndEsmEmmDataReqMsg(NAS_ESM_ILLEGAL_OPID, PS_FALSE, ulSndNwMsgLen, NAS_ESM_GetCurEsmSndNwMsgAddr());
        NAS_ESM_UpdateEsmSndNwMsgLen(ulSndNwMsgLen);
    }
    else
    {
        /*�����෢�ܾ���Ϣ*/
        NAS_ESM_SndRejAirMsg(pstEsmNwMsgIE->enEsmCnMsgType, &stEncodeInfo);
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_ProcNwMsgForDecodeError
 Description     : �տ���Ϣ���������
 Input           : NAS_ESM_NW_MSG_STRU *pstMsgIE
                   NAS_ESM_CAUSE_ENUM_UINT8  enEsmCause
 Output          : None
 Return          : VOS_VOID

 History         :
    1.sunbing49683      2009-2-24  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_ProcNwMsgForDecodeError(const NAS_ESM_NW_MSG_STRU *pstEsmNwMsgIE,
                                                     NAS_ESM_CAUSE_ENUM_UINT8  enEsmCause)
{
    VOS_UINT32                          ulSndNwMsgLen       = NAS_ESM_NULL;
    NAS_ESM_ENCODE_INFO_STRU            stEncodeInfo;

    NAS_ESM_INFO_LOG("NAS_ESM_ProcNwMsgForDecodeError is entered.");

    /* ��ԭ��ֵΪ��Ϣ̫�̣���������Ϣ */
    if (NAS_ESM_CAUSE_MSG_TOO_SHORT == enEsmCause)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgForDecodeError:Msg too short!");
        return ;
    }

    /*��ʼ��NAS_ESM_ENCODE_INFO_STRU*/
    NAS_ESM_MEM_SET((VOS_VOID*)&stEncodeInfo, 0, sizeof(NAS_ESM_ENCODE_INFO_STRU));

    /*���NAS_ESM_ENCODE_INFO_STRU*/
    stEncodeInfo.bitOpESMCau = NAS_ESM_OP_TRUE;
    stEncodeInfo.ulEpsbId    = (VOS_UINT32)pstEsmNwMsgIE->ucEpsbId;
    stEncodeInfo.ucPti       = pstEsmNwMsgIE->ucPti;
    stEncodeInfo.ucESMCau    = enEsmCause;

    /*���ݽ��յ��Ŀտ���Ϣ���ʹ���*/
    switch(pstEsmNwMsgIE->enEsmCnMsgType)
    {
        /*���������Ϣ����ΪACT_DEFLT_EPS_BEARER_CNTXT_REQ*/
        case NAS_ESMCN_MSG_TYPE_ACT_DEFLT_EPS_BEARER_CNTXT_REQ:
            /*�ظ��ܾ�*/
            NAS_ESM_EncodeActDefltEpsbCntxtRejMsg(stEncodeInfo,\
                                         NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                                        &ulSndNwMsgLen);

            NAS_ESM_SndAirMsgReportInd(NAS_ESM_GetCurEsmSndNwMsgAddr(),ulSndNwMsgLen,\
                                   NAS_ESM_AIR_MSG_DIR_ENUM_UP,\
                                   ESM_ACT_DEFLT_EPS_BEARER_CNTXT_REJ);

            break;

        /*���������Ϣ����ΪACT_DEDICTD_EPS_BEARER_CNTXT_REQ*/
        case NAS_ESMCN_MSG_TYPE_ACT_DEDICTD_EPS_BEARER_CNTXT_REQ:
            /*�ظ��ܾ�*/
            NAS_ESM_EncodeActDedictdEpsbCntxtRejMsg(stEncodeInfo,\
                                           NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                                          &ulSndNwMsgLen);

            NAS_ESM_SndAirMsgReportInd(NAS_ESM_GetCurEsmSndNwMsgAddr(),ulSndNwMsgLen,\
                                   NAS_ESM_AIR_MSG_DIR_ENUM_UP,\
                                   ESM_ACT_DEDICT_EPS_BEARER_CNTXT_REJ);

            break;

        /*���������Ϣ����ΪMOD_EPS_BEARER_CNTXT_REQ*/
        case NAS_ESMCN_MSG_TYPE_MOD_EPS_BEARER_CNTXT_REQ:
            /*�ظ��ܾ�*/
            NAS_ESM_EncodeModEpsbCntxtRejMsg(stEncodeInfo,\
                                    NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                                   &ulSndNwMsgLen);

            NAS_ESM_SndAirMsgReportInd(NAS_ESM_GetCurEsmSndNwMsgAddr(),ulSndNwMsgLen,\
                                   NAS_ESM_AIR_MSG_DIR_ENUM_UP,\
                                   ESM_MOD_EPS_BEARER_CNTXT_REJ);

            break;

        default:
            /*������Ϣ������󣬷���ESM STATUS*/
            NAS_ESM_EncodeEsmStatus(stEncodeInfo,\
                                    NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                                   &ulSndNwMsgLen);

            NAS_ESM_SndAirMsgReportInd(NAS_ESM_GetCurEsmSndNwMsgAddr(),ulSndNwMsgLen,\
                                   NAS_ESM_AIR_MSG_DIR_ENUM_UP,\
                                   ESM_STUS_MO);


            /*��ӡ�쳣��Ϣ*/
            NAS_ESM_WARN_LOG("NAS_ESM_RcvEsmEmmDataInd:WARNING: NW Message Protocol Error!");
            break;
    }

    NAS_ESM_SndEsmEmmDataReqMsg(NAS_ESM_ILLEGAL_OPID, PS_FALSE, ulSndNwMsgLen, NAS_ESM_GetCurEsmSndNwMsgAddr());
    NAS_ESM_UpdateEsmSndNwMsgLen(ulSndNwMsgLen);
}


VOS_VOID  NAS_ESM_ProcValidNwMsgEsmInfoReq
(
    const NAS_ESM_NW_MSG_STRU          *pstEsmNwMsgIE
)
{
    VOS_UINT32                          ulSndNwMsgLen       = NAS_ESM_NULL;
    NAS_ESM_ENCODE_INFO_STRU            stEncodeInfo;

    NAS_ESM_INFO_LOG("NAS_ESM_ProcValidNwMsgEsmInfoReq is entered.");

    /*���NAS_ESM_ENCODE_INFO_STRU*/
    NAS_ESM_MEM_SET((VOS_VOID*)&stEncodeInfo, 0, sizeof(NAS_ESM_ENCODE_INFO_STRU));
    stEncodeInfo.ucPti = pstEsmNwMsgIE->ucPti;/*bsun Ӧ�ø���PDN���������е�PTI����*/

    /*����ESM INFORMATION RESPONSE��Ϣ*/
    if(NAS_ESM_FAILURE == NAS_ESM_EncodeEsmInformationRspMsg(stEncodeInfo, \
                                  NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                                 &ulSndNwMsgLen))
    {
        /*��ӡ������Ϣ��ESM INFORMATION RESPONSE��Ϣ�������*/
        NAS_ESM_WARN_LOG("NAS_ESM_ProcValidNwMsgEsmInfoReq:WARNING:EncodeEsmInformationRsp fail!");
        return;
    }

    /*�տ���Ϣ�ϱ�*/
    NAS_ESM_SndAirMsgReportInd(NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                               ulSndNwMsgLen, NAS_ESM_AIR_MSG_DIR_ENUM_UP,\
                               ESM_INFO_RSP);

    /*����ESM INFORMATION RESPONSE��Ϣ*/
    NAS_ESM_SndEsmEmmDataReqMsg(NAS_ESM_ILLEGAL_OPID, PS_FALSE, ulSndNwMsgLen, NAS_ESM_GetCurEsmSndNwMsgAddr());
    NAS_ESM_UpdateEsmSndNwMsgLen(ulSndNwMsgLen);
}


/*****************************************************************************
 Function Name   : NAS_ESM_ValidatePtiInActDefltOrInfoOrRejMsg
 Description     : ��ȱʡ���ؼ��������ESM INFO��REJ��Ϣ�е�PTI���кϷ��Լ��
 Input           : ulPti-------------------------PTI
 Output          : None
 Return          : NAS_ESM_CAUSE_ENUM_UINT8

 History         :
    1.lihong00150010      2010-5-13  Draft Enact

*****************************************************************************/
NAS_ESM_CAUSE_ENUM_UINT8  NAS_ESM_ValidatePtiInActDefltOrInfoOrRejMsg
(
    VOS_UINT8                           ucPti
)
{
    VOS_UINT32                          ulStateTblId        = NAS_ESM_NULL;

    NAS_ESM_LOG1("NAS_ESM_ValidatePtiInActDefltOrInfoOrRejMsg, Pti : ", ucPti);

    if ((ucPti == NAS_ESM_PTI_UNASSIGNED_VALUE)
         || (ucPti == NAS_ESM_PTI_RESERVED_VALUE))
    {
        return NAS_ESM_CAUSE_INVALID_PTI_VALUE;
    }

    /* ����PTI���ҵ���̬��Index */
    if(NAS_ESM_SUCCESS != NAS_ESM_QueryStateTblIndexByPti(ucPti, &ulStateTblId))
    {
        return NAS_ESM_CAUSE_PTI_MISMATCH;
    }

    return NAS_ESM_CAUSE_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_ValidatePtiInActDedictOrModOrDeactOrStatusMsg
 Description     : ��ר�г��ؼ������������޸Ļ����ȥ�����ESM STATUS��Ϣ
                   �е�PTI���кϷ��Լ��
 Input           : ulPti-------------------------PTI
 Output          : None
 Return          : NAS_ESM_CAUSE_ENUM_UINT8

 History         :
    1.lihong00150010      2010-5-13  Draft Enact

*****************************************************************************/
NAS_ESM_CAUSE_ENUM_UINT8  NAS_ESM_ValidatePtiInActDedictOrModOrDeactOrStatusMsg
(
    VOS_UINT32                          ulPti,
    VOS_UINT32                         *pulStateTblId
)
{
    VOS_UINT32                          ulStateTblId        = NAS_ESM_NULL;

    *pulStateTblId = NAS_ESM_ILL_STATETAL_INDEX;

    if (ulPti == NAS_ESM_PTI_RESERVED_VALUE)
    {
        return NAS_ESM_CAUSE_INVALID_PTI_VALUE;
    }

    if (ulPti == NAS_ESM_PTI_UNASSIGNED_VALUE)
    {
        return NAS_ESM_CAUSE_SUCCESS;
    }

    /* ����PTI���ҵ���̬��Index */
    if(NAS_ESM_SUCCESS != NAS_ESM_QueryStateTblIndexByPti((VOS_UINT8)ulPti, &ulStateTblId))
    {
        return NAS_ESM_CAUSE_PTI_MISMATCH;
    }

    *pulStateTblId = ulStateTblId;
    return NAS_ESM_CAUSE_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_ValidateEpsbIdInActMsg
 Description     : ��ȱʡ���ؼ��������ר�г��ؼ���������Ϣ
                   �еĳ��غŽ��кϷ��Լ��
 Input           : ulEpsbId-------------------------���غ�
 Output          : None
 Return          : NAS_ESM_CAUSE_ENUM_UINT8

 History         :
    1.lihong00150010      2010-5-14  Draft Enact

*****************************************************************************/
NAS_ESM_CAUSE_ENUM_UINT8  NAS_ESM_ValidateEpsbIdInActMsg
(
    VOS_UINT8                           ucEpsbId
)
{
    if((ucEpsbId < NAS_ESM_MIN_EPSB_ID)||(ucEpsbId > NAS_ESM_MAX_EPSB_ID))
    {
        return NAS_ESM_CAUSE_INVALID_EPS_BEARER_IDENTITY;
    }

    return NAS_ESM_CAUSE_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_ValidateEpsbIdInRejOrInfoMsg
 Description     : ������ܾ���ESM INFO��Ϣ�еĳ��غŽ��кϷ��Լ��
 Input           : ulEpsbId-------------------------���غ�
 Output          : None
 Return          : NAS_ESM_CAUSE_ENUM_UINT8

 History         :
    1.lihong00150010      2010-5-14  Draft Enact

*****************************************************************************/
NAS_ESM_CAUSE_ENUM_UINT8  NAS_ESM_ValidateEpsbIdInRejOrInfoMsg
(
    VOS_UINT8                           ucEpsbId
)
{
    if(ucEpsbId != NAS_ESM_UNASSIGNED_EPSB_ID)
    {
        return NAS_ESM_CAUSE_INVALID_EPS_BEARER_IDENTITY;
    }

    return NAS_ESM_CAUSE_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_ValidateEpsbIdInModOrDeactMsg
 Description     : �Գ����޸Ļ����ȥ������Ϣ�еĳ��غŽ��кϷ��Լ��
 Input           : ulEpsbId-------------------------���غ�
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2010-5-14  Draft Enact

*****************************************************************************/
NAS_ESM_CAUSE_ENUM_UINT8  NAS_ESM_ValidateEpsbIdInModOrDeactMsg
(
    VOS_UINT8                           ucEpsbId
)
{
    if(NAS_ESM_BEARER_STATE_ACTIVE != NAS_ESM_GetBearCntxtState(ucEpsbId))
    {
        return NAS_ESM_CAUSE_INVALID_EPS_BEARER_IDENTITY;
    }

    return NAS_ESM_CAUSE_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_ValidateLinkedEpsbId
 Description     : �Թ������غŽ��кϷ��Լ��
 Input           : ucLinkedEpsbId-------------------------�������غ�
 Output          : None
 Return          : NAS_ESM_CAUSE_ENUM_UINT8

 History         :
    1.lihong00150010      2010-5-14  Draft Enact

*****************************************************************************/
NAS_ESM_CAUSE_ENUM_UINT8  NAS_ESM_ValidateLinkedEpsbId
(
    VOS_UINT8                           ucLinkedEpsbId
)
{
    VOS_UINT32                          ulStateTblIndex  = NAS_ESM_NULL;
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr     = VOS_NULL_PTR;

    if((NAS_ESM_BEARER_STATE_ACTIVE != NAS_ESM_GetBearCntxtState(ucLinkedEpsbId))
        || (PS_FALSE == NAS_ESM_IsDefaultEpsBearerType(NAS_ESM_GetBearCntxtType(ucLinkedEpsbId))))
    {
        return NAS_ESM_CAUSE_INVALID_EPS_BEARER_IDENTITY;
    }

    /*Linked EPS Bearer������PDN DISCONNECT�����У�ֱ�ӷ���ERROR��������Ϣ������*/
    if(NAS_ESM_SUCCESS == NAS_ESM_QueryStateTblIndexByEpsbId(ucLinkedEpsbId,&ulStateTblIndex))
    {
        pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);
        if(NAS_ESMCN_MSG_TYPE_PDN_DISCONNECT_REQ == pstStateAddr->stNwMsgRecord.enEsmCnMsgType)
        {
            /*��ӡ��Ϣ*/
            NAS_ESM_NORM_LOG("NAS_ESM_ValidateLinkedEpsbId:NORMAL:Link Bearer In Procedure of PDN DISCONNECT !");

            return NAS_ESM_CAUSE_ERROR;
        }
    }

    return NAS_ESM_CAUSE_SUCCESS;
}



VOS_VOID  NAS_ESM_CreatePfPVListInBearCreateTft
(
    const NAS_ESM_NW_MSG_STRU          *pstNwMsg,
    VOS_UINT8                          *pucPfNum,
    NAS_ESM_PF_PRCDNC_VLDT_STRU        *pstPfPVList
)
{
    VOS_UINT32                          ulCnt           = NAS_ESM_NULL;
    const NAS_ESM_CONTEXT_TFT_STRU     *pstPacketFilter = VOS_NULL_PTR;

    for (ulCnt = NAS_ESM_NULL; ulCnt < pstNwMsg->stTadInfo.ucSdfPfNum; ulCnt++)
    {
        pstPacketFilter = &pstNwMsg->stTadInfo.astSdfPfInfo[ulCnt];
        pstPfPVList[*pucPfNum].ucEpsbId = pstNwMsg->ucEpsbId;
        pstPfPVList[*pucPfNum].ucPfId = pstPacketFilter->ucNwPacketFilterId;
        pstPfPVList[*pucPfNum].ucPrecedence = pstPacketFilter->ucPrecedence;
        (*pucPfNum)++;
    }
}


VOS_VOID  NAS_ESM_CreatePfPVListInBearAddOrReplaceFilter
(
    const NAS_ESM_NW_MSG_STRU          *pstNwMsg,
    VOS_UINT8                          *pucPfNum,
    NAS_ESM_PF_PRCDNC_VLDT_STRU        *pstPfPVList
)
{
    VOS_UINT32                          ulCnt1          = NAS_ESM_NULL;
    VOS_UINT32                          ulCnt2          = NAS_ESM_NULL;
    VOS_UINT8                           ucPfPVNumTmp    = NAS_ESM_NULL;
    VOS_UINT8                           ucPfPVNum       = NAS_ESM_NULL;
    NAS_ESM_PF_PRCDNC_VLDT_STRU         astPfPVListTmp[NAS_ESM_MAX_SDF_PF_NUM]= {0};
    const NAS_ESM_CONTEXT_TFT_STRU     *pstPacketFilter = VOS_NULL_PTR;

    /* ��ȡ�˳�����packet filter���ȼ���֤�б� */
    NAS_ESM_GetPfPrecedenceValidateListInBearer(pstNwMsg->ucEpsbId,
                                                &ucPfPVNum,
                                                astPfPVListTmp);

    ucPfPVNumTmp = ucPfPVNum;

    for (ulCnt1 = NAS_ESM_NULL; ulCnt1 < pstNwMsg->stTadInfo.ucSdfPfNum; ulCnt1++)
    {
        pstPacketFilter = &pstNwMsg->stTadInfo.astSdfPfInfo[ulCnt1];

        for (ulCnt2 = NAS_ESM_NULL; ulCnt2 < ucPfPVNumTmp; ulCnt2++)
        {
            if (pstPacketFilter->ucNwPacketFilterId == astPfPVListTmp[ulCnt2].ucPfId)
            {
                astPfPVListTmp[ulCnt2].ucPrecedence = pstPacketFilter->ucPrecedence;
                break;
            }
        }

        if (ulCnt2 >= ucPfPVNumTmp)
        {
            astPfPVListTmp[ucPfPVNum].ucEpsbId = pstNwMsg->ucEpsbId;
            astPfPVListTmp[ucPfPVNum].ucPfId = pstPacketFilter->ucNwPacketFilterId;
            astPfPVListTmp[ucPfPVNum].ucPrecedence = pstPacketFilter->ucPrecedence;
            ucPfPVNum++;
        }
    }

    NAS_ESM_MEM_CPY(&pstPfPVList[*pucPfNum],
                    astPfPVListTmp,
                    ucPfPVNum * sizeof(NAS_ESM_PF_PRCDNC_VLDT_STRU));

    (*pucPfNum) += ucPfPVNum;

}
VOS_UINT32  NAS_ESM_CreatePfPrecedenceValidateListInPdn
(
    VOS_UINT32                          ulEpsbId,
    VOS_UINT8                          *pucPfNum,
    NAS_ESM_PF_PRCDNC_VLDT_STRU        *pstPfPVList
)
{
    VOS_UINT32                          ulCnt           = NAS_ESM_NULL;
    VOS_UINT8                           aucEpsbId[NAS_ESM_MAX_EPSB_NUM] = {0};
    VOS_UINT8                           ucEpsBearNum    = NAS_ESM_NULL;

    *pucPfNum = NAS_ESM_NULL;

    /* ��ȡĳPDN�µ����г��غ� */
    if (NAS_ESM_SUCCESS != NAS_ESM_GetAllEpsBearerInPdn((VOS_UINT8)ulEpsbId,
                                                        &ucEpsBearNum,
                                                        aucEpsbId))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_CreatePfPrecedenceValidateListInPdn:NAS_ESM_GetAllEpsBearerInPdn failed!");
        return NAS_ESM_FAILURE;
    }

    /* ���ͬһPDN�µ��������ص�packet filter���ȼ��б� */
    for (ulCnt= NAS_ESM_NULL; ulCnt < ucEpsBearNum; ulCnt++)
    {

        /* ��ȡ�˳�����packet filter���ȼ���֤�б� */
        NAS_ESM_GetPfPrecedenceValidateListInBearer(aucEpsbId[ulCnt],
                                                    pucPfNum,
                                                    pstPfPVList);
    }

    return NAS_ESM_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_ValidateTftInActDedictMsg
 Description     : ��ר�г��ؼ���������Ϣ�е�TFT���кϷ��Լ��
 Input           : pstTft-------------------------TFTָ��
 Output          : None
 Return          : NAS_ESM_CAUSE_ENUM_UINT8

 History         :
    1.lihong00150010      2010-5-14  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_ESM_ValidateTftInActDedictMsg
(
    const NAS_ESM_NW_MSG_STRU          *pstNwMsg,
    VOS_UINT32                          ulStateTblIndex
)
{
    NAS_ESM_ENCODE_INFO_STRU            stEncodeInfo = { NAS_ESM_NULL };

    /*��ʼ���������NAS_ESM_ENCODE_INFO_STRU*/
    NAS_ESM_MEM_SET((VOS_VOID*)&stEncodeInfo, 0, sizeof(NAS_ESM_ENCODE_INFO_STRU));
    stEncodeInfo.bitOpESMCau = NAS_ESM_OP_TRUE;
    stEncodeInfo.ucPti       = pstNwMsg->ucPti;
    stEncodeInfo.ulEpsbId    = (VOS_UINT32)pstNwMsg->ucEpsbId;

    if (NAS_ESM_TFT_OP_TYPE_CREATE_TFT != pstNwMsg->stTadInfo.enTftOpType)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ValidateTftInActDedictMsg:Tad op is not create new tft!");

        stEncodeInfo.ucESMCau = NAS_ESM_CAUSE_SEMANTIC_ERR_IN_TFT;
        NAS_ESM_ActDediBearFailProc(&stEncodeInfo, ulStateTblIndex);
        return NAS_ESM_MSG_HANDLED;
    }

    return NAS_ESM_CAUSE_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_ValidateTftInActDedictMsg
 Description     : �Գ����޸�������Ϣ�е�TFT���кϷ��Լ��
 Input           : pstNwMsg-------------------------�����Ŀտ���Ϣָ��
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2010-5-14  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_ESM_ValidateTftInModMsg
(
    const NAS_ESM_NW_MSG_STRU          *pstNwMsg,
    VOS_UINT32                          ulStateTblIndex
)
{
    NAS_ESM_ENCODE_INFO_STRU            stEncodeInfo;

    /*��ʼ���������NAS_ESM_ENCODE_INFO_STRU*/
    NAS_ESM_MEM_SET((VOS_VOID*)&stEncodeInfo, 0, sizeof(NAS_ESM_ENCODE_INFO_STRU));
    stEncodeInfo.bitOpESMCau = NAS_ESM_OP_TRUE;
    stEncodeInfo.ucPti       = pstNwMsg->ucPti;
    stEncodeInfo.ulEpsbId    = (VOS_UINT32)pstNwMsg->ucEpsbId;

    if (NAS_ESM_SUCCESS != NAS_ESM_ValidateTftOpInModMsg(pstNwMsg,&stEncodeInfo,ulStateTblIndex))
    {
        return NAS_ESM_MSG_HANDLED;
    }

    return NAS_ESM_SUCCESS;
}
NAS_ESM_COMPARE_RESULT_ENUM_UINT8 NAS_ESM_ComPareQosRate
(
    VOS_UINT32 ulData1,
    VOS_UINT32 ulData2
)
{
   if(ulData1 > ulData2)
   {
        return NAS_ESM_COMPARE_RESULT_BIG;
   }
   else if(ulData1 == ulData2)
   {
        return NAS_ESM_COMPARE_RESULT_EQUAL;
   }
   else
   {
        return NAS_ESM_COMPARE_RESULT_SMALL;
   }
}


VOS_UINT32 NAS_ESM_IsEpsQosEqual
(
    NAS_ESM_COMPARE_RESULT_ENUM_UINT8   enUlMaxRateComRslt,
    NAS_ESM_COMPARE_RESULT_ENUM_UINT8   enDlMaxRateComRslt,
    NAS_ESM_COMPARE_RESULT_ENUM_UINT8   enULGMaxRateComRslt,
    NAS_ESM_COMPARE_RESULT_ENUM_UINT8   enDLGMaxRateComRslt
)
{
    /* ������ȣ��򷵻�PS_TRUE */
    if((NAS_ESM_COMPARE_RESULT_EQUAL == enUlMaxRateComRslt)
        && (NAS_ESM_COMPARE_RESULT_EQUAL == enDlMaxRateComRslt)
        && (NAS_ESM_COMPARE_RESULT_EQUAL == enULGMaxRateComRslt)
        && (NAS_ESM_COMPARE_RESULT_EQUAL == enDLGMaxRateComRslt))
    {
        return PS_TRUE;
    }

    return PS_FALSE;
}
VOS_UINT32 NAS_ESM_IsEpsQosBiggerOrEqual
(
    NAS_ESM_COMPARE_RESULT_ENUM_UINT8   enUlMaxRateComRslt,
    NAS_ESM_COMPARE_RESULT_ENUM_UINT8   enDlMaxRateComRslt,
    NAS_ESM_COMPARE_RESULT_ENUM_UINT8   enULGMaxRateComRslt,
    NAS_ESM_COMPARE_RESULT_ENUM_UINT8   enDLGMaxRateComRslt
)
{
    /* ���������ߵ��ڣ��򷵻�PS_TRUE */
    if(((NAS_ESM_COMPARE_RESULT_BIG == enUlMaxRateComRslt) || (NAS_ESM_COMPARE_RESULT_EQUAL == enUlMaxRateComRslt))
        && ((NAS_ESM_COMPARE_RESULT_BIG == enDlMaxRateComRslt) || (NAS_ESM_COMPARE_RESULT_EQUAL == enDlMaxRateComRslt))
        && ((NAS_ESM_COMPARE_RESULT_BIG == enULGMaxRateComRslt) || (NAS_ESM_COMPARE_RESULT_EQUAL == enULGMaxRateComRslt))
        && ((NAS_ESM_COMPARE_RESULT_BIG == enDLGMaxRateComRslt) || (NAS_ESM_COMPARE_RESULT_EQUAL == enDLGMaxRateComRslt)))
    {
        return PS_TRUE;
    }

    return PS_FALSE;
}
VOS_UINT32 NAS_ESM_IsEpsQosSmallerOrEqual
(
    NAS_ESM_COMPARE_RESULT_ENUM_UINT8   enUlMaxRateComRslt,
    NAS_ESM_COMPARE_RESULT_ENUM_UINT8   enDlMaxRateComRslt,
    NAS_ESM_COMPARE_RESULT_ENUM_UINT8   enULGMaxRateComRslt,
    NAS_ESM_COMPARE_RESULT_ENUM_UINT8   enDLGMaxRateComRslt
)
{
    /* �����С���򷵻�PS_TRUE */
    if(((NAS_ESM_COMPARE_RESULT_SMALL == enUlMaxRateComRslt) || (NAS_ESM_COMPARE_RESULT_EQUAL == enUlMaxRateComRslt))
        && ((NAS_ESM_COMPARE_RESULT_SMALL == enDlMaxRateComRslt) || (NAS_ESM_COMPARE_RESULT_EQUAL == enDlMaxRateComRslt))
        && ((NAS_ESM_COMPARE_RESULT_SMALL == enULGMaxRateComRslt) || (NAS_ESM_COMPARE_RESULT_EQUAL == enULGMaxRateComRslt))
        && ((NAS_ESM_COMPARE_RESULT_SMALL == enDLGMaxRateComRslt) || (NAS_ESM_COMPARE_RESULT_EQUAL == enDLGMaxRateComRslt)))
    {
        return PS_TRUE;
    }

    return PS_FALSE;
}
NAS_ESM_COMPARE_RESULT_ENUM_UINT8 NAS_ESM_CompareEpsQos
(
    const APP_ESM_EPS_QOS_INFO_STRU                      *pstEpsQoSInfo1,
    const APP_ESM_EPS_QOS_INFO_STRU                      *pstEpsQoSInfo2
)
{
    NAS_ESM_COMPARE_RESULT_ENUM_UINT8   enUlMaxRateComRslt;
    NAS_ESM_COMPARE_RESULT_ENUM_UINT8   enDlMaxRateComRslt;
    NAS_ESM_COMPARE_RESULT_ENUM_UINT8   enULGMaxRateComRslt;
    NAS_ESM_COMPARE_RESULT_ENUM_UINT8   enDLGMaxRateComRslt;

    /* �Ƚ�UlMaxRate */
    enUlMaxRateComRslt = NAS_ESM_ComPareQosRate(pstEpsQoSInfo1->ulULMaxRate,
                                                pstEpsQoSInfo2->ulULMaxRate);

    /* �Ƚ�DlMaxRate */
    enDlMaxRateComRslt = NAS_ESM_ComPareQosRate(pstEpsQoSInfo1->ulDLMaxRate,
                                                pstEpsQoSInfo2->ulDLMaxRate);

    /* �Ƚ�UlGMaxRate */
    enULGMaxRateComRslt = NAS_ESM_ComPareQosRate(pstEpsQoSInfo1->ulULGMaxRate,
                                                pstEpsQoSInfo2->ulULGMaxRate);

    /* �Ƚ�DlGMaxRate */
    enDLGMaxRateComRslt = NAS_ESM_ComPareQosRate(pstEpsQoSInfo1->ulDLGMaxRate,
                                                pstEpsQoSInfo2->ulDLGMaxRate);

    /* ������ȣ��򷵻�NAS_ESM_COMPARE_RESULT_EQUAL */
    if(PS_TRUE == NAS_ESM_IsEpsQosEqual(enUlMaxRateComRslt, enDlMaxRateComRslt,
                                        enULGMaxRateComRslt, enDLGMaxRateComRslt))
    {
        return NAS_ESM_COMPARE_RESULT_EQUAL;
    }

    /* ����������ʶ���С�ڻ��ߵ��ڣ��򷵻�NAS_ESM_COMPARE_RESULT_SMALL */
    if(PS_TRUE == NAS_ESM_IsEpsQosSmallerOrEqual(enUlMaxRateComRslt, enDlMaxRateComRslt,
                                                 enULGMaxRateComRslt, enDLGMaxRateComRslt))
    {
        return NAS_ESM_COMPARE_RESULT_SMALL;
    }

    /* ����������ʶ��Ǵ��ڻ��ߵ��ڣ��򷵻�NAS_ESM_COMPARE_RESULT_BIG */
    if(PS_TRUE == NAS_ESM_IsEpsQosBiggerOrEqual(enUlMaxRateComRslt, enDlMaxRateComRslt,
                                                enULGMaxRateComRslt, enDLGMaxRateComRslt))
    {
        return NAS_ESM_COMPARE_RESULT_BIG;
    }

    return NAS_ESM_COMPARE_RESULT_OTHER;
}
NAS_ESM_CAUSE_ENUM_UINT8 NAS_ESM_ValidateEpsQos
(
    NAS_ESM_TFT_OP_TYPE_ENUM_UINT8                           enTftOpType,
    const NAS_ESM_CONTEXT_LTE_QOS_STRU                      *pstEpsQoSInfoOld,
    const NAS_ESM_CONTEXT_LTE_QOS_STRU                      *pstEpsQoSInfoNew
)
{
    NAS_ESM_COMPARE_RESULT_ENUM_UINT8                   enQosCompareResult;

    enQosCompareResult = NAS_ESM_CompareEpsQos(&pstEpsQoSInfoOld->stQosInfo,
                                               &pstEpsQoSInfoNew->stQosInfo);

    if(NAS_ESM_TFT_OP_TYPE_ADD_FILTER == enTftOpType)
    {
        /* �������SDF����ǰ�����QOSӦС�ڻ��ߵ����յ���Ϣ�е�QOS */
        if ((NAS_ESM_COMPARE_RESULT_SMALL == enQosCompareResult)
          ||(NAS_ESM_COMPARE_RESULT_EQUAL == enQosCompareResult))
        {
            return NAS_ESM_CAUSE_SUCCESS;
        }
        else
        {
            NAS_ESM_WARN_LOG("NAS_ESM_ValidateEpsQos:WARNING:Qos is not increased !");
            return NAS_ESM_CAUSE_PROTOCOL_ERROR;
        }
    }
    else if (NAS_ESM_TFT_OP_TYPE_DELETE_FILTER == enTftOpType)
    {
        /* ����ɾ��SDF����ǰ�����QOSӦ���ڻ��ߵ����յ���Ϣ�е�QOS */
        if ((NAS_ESM_COMPARE_RESULT_BIG == enQosCompareResult)
          ||(NAS_ESM_COMPARE_RESULT_EQUAL == enQosCompareResult))
        {
            return NAS_ESM_CAUSE_SUCCESS;
        }
        else
        {
            NAS_ESM_WARN_LOG("NAS_ESM_ValidateEpsQos:WARNING:Qos is not decreased !");
            return NAS_ESM_CAUSE_PROTOCOL_ERROR;
        }
    }
    else
    {
        return NAS_ESM_CAUSE_SUCCESS;
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_ValidateQosInModMsg
 Description     : �Գ����޸������е�QOS��Ϣ���кϷ��Լ��
 Input           : pstMsgIE -- ������������Ϣ
 Output          : None
 Return          : NAS_ESM_CAUSE_ENUM_UINT8

 History         :
    1.lihong00150010      2009-12-2  Draft Enact

*****************************************************************************/
NAS_ESM_CAUSE_ENUM_UINT8  NAS_ESM_ValidateQosInModMsg
(
    const NAS_ESM_NW_MSG_STRU               *pstMsgIE,
    const NAS_ESM_EPSB_CNTXT_INFO_STRU      *pstEpsbCntxtInfo
)
{
    NAS_ESM_CAUSE_ENUM_UINT8            enEsmCause       = NAS_ESM_CAUSE_SUCCESS;

    /* �������QOS */
    if (NAS_ESM_OP_FALSE == pstMsgIE->bitOpEpsQos)
    {
        /* ȱʡ���ؿ��Բ���Qos */
        if(PS_TRUE == NAS_ESM_IsDefaultEpsBearerType(NAS_ESM_GetBearCntxtType(pstMsgIE->ucEpsbId)))
        {
            NAS_ESM_INFO_LOG("NAS_ESM_ValidateQosInModMsg:INFO:Default Bearer No QoS Information !");
            return NAS_ESM_CAUSE_SUCCESS;
        }

        /* ��TAD��ΪNO OPERATIONʱ������Я��QOS��������ΪGBR���أ���TAD��ΪADD SDF,
           �����Я��QOS */
        if (((NAS_ESM_OP_TRUE == pstMsgIE->bitOpTadInfo)
          && (NAS_ESM_TFT_OP_TYPE_NO_OP == pstMsgIE->stTadInfo.enTftOpType)))

        {
            NAS_ESM_WARN_LOG("NAS_ESM_ValidateQosInModMsg:WARNING:No QoS Information !");
            return NAS_ESM_CAUSE_PROTOCOL_ERROR;
        }
        else
        {
            return NAS_ESM_CAUSE_SUCCESS;
        }
    }

    /* �ж��޸ĺ��Qos�Ƿ���� */
    if (PS_TRUE == NAS_ESM_IsGbrBearer(pstEpsbCntxtInfo->stEpsQoSInfo.stQosInfo.ucQCI))

    {
        enEsmCause = NAS_ESM_ValidateEpsQos(pstMsgIE->stTadInfo.enTftOpType,
                                            &pstEpsbCntxtInfo->stEpsQoSInfo,
                                            &pstMsgIE->stSdfQosInfo);
        if (NAS_ESM_CAUSE_SUCCESS != enEsmCause)
        {
            NAS_ESM_WARN_LOG("NAS_ESM_ValidateQosInModMsg:WARNING: Qos invalid!");
            return enEsmCause;
        }
    }

    return NAS_ESM_CAUSE_SUCCESS;
}

#if 0
/*****************************************************************************
 Function Name   : NAS_ESM_ValidateTftInActDedictMsg
 Description     : �Գ����޸�������Ϣ�е�APN-AMBR���кϷ��Լ��
 Input           : pstNwMsg-------------------------�����Ŀտ���Ϣָ��
 Output          : None
 Return          : NAS_ESM_CAUSE_ENUM_UINT8

 History         :
    1.lihong00150010      2010-5-14  Draft Enact

*****************************************************************************/
NAS_ESM_CAUSE_ENUM_UINT8  NAS_ESM_ValidateApnAmbrInModMsg
(
    const NAS_ESM_NW_MSG_STRU          *pstNwMsg
)
{
    if( (NAS_ESM_OP_TRUE == pstNwMsg->bitOpApnAmbr)
        && (NAS_ESM_BEARER_TYPE_DEFAULT != NAS_ESM_GetBearCntxtType(pstNwMsg->ucEpsbId)))
    {
        /*��ӡ��Ϣ*/
        NAS_ESM_NORM_LOG("NAS_ESM_ValidateApnAmbrInModMsg:NORMAL:Dedicated bearer can not have APN AMBR !");
        return NAS_ESM_CAUSE_PROTOCOL_ERROR;
    }

    return NAS_ESM_CAUSE_SUCCESS;
}
#endif

/*****************************************************************************
 Function Name   : NAS_ESM_ValidateActDefltEpsbReqMsg
 Description     : �Լ���ȱʡ����������Ϣ���кϷ��Լ��
 Input           : pstNwMsg-------------------------�����Ŀտ���Ϣָ��
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2010-5-12  Draft Enact

*****************************************************************************/
static VOS_UINT32  NAS_ESM_ValidateActDefltEpsbReqMsg
(
    const NAS_ESM_NW_MSG_STRU          *pstNwMsg
)
{
    NAS_ESM_CAUSE_ENUM_UINT8            enEsmCause = NAS_ESM_NULL;

    /* ���PTI��PTI��Ϊ0����ΪRESERVED����ͨ����PTI�ܼ�����״̬��
       �еļ�¼������ظ��ܾ���Ϣ */
    enEsmCause = NAS_ESM_ValidatePtiInActDefltOrInfoOrRejMsg(pstNwMsg->ucPti);
    if (enEsmCause != NAS_ESM_CAUSE_SUCCESS)
    {
        NAS_ESM_LOG1("NAS_ESM_ValidateActDefltEpsbReqMsg:WARN:Validate Pti failed, cause : ", enEsmCause);
        NAS_ESM_ProcNwMsgForIllegalPti(pstNwMsg, enEsmCause);
        return NAS_ESM_MSG_HANDLED;
    }

    /* �����غŷ�Χ������λ��5-15֮�䣬����ظ��ܾ���Ϣ */
    enEsmCause = NAS_ESM_ValidateEpsbIdInActMsg(pstNwMsg->ucEpsbId);
    if (enEsmCause != NAS_ESM_CAUSE_SUCCESS)
    {
        NAS_ESM_LOG1("NAS_ESM_ValidateActDefltEpsbReqMsg:WARN:Validate EpsbId failed, cause : ", enEsmCause);
        NAS_ESM_ProcNwMsgForIllegalEpsbId(pstNwMsg);
        return NAS_ESM_MSG_HANDLED;
    }

    return NAS_ESM_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_ValidateActDedictEpsbReqMsg
 Description     : �Լ���ר�г���������Ϣ���кϷ��Լ��
 Input           : pstNwMsg-------------------------�����Ŀտ���Ϣָ��
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2010-5-12  Draft Enact

*****************************************************************************/
static VOS_UINT32  NAS_ESM_ValidateActDedictEpsbReqMsg
(
    const NAS_ESM_NW_MSG_STRU          *pstNwMsg
)
{
    NAS_ESM_CAUSE_ENUM_UINT8            enEsmCause      = NAS_ESM_NULL;
    VOS_UINT32                          ulStateTblId    = NAS_ESM_NULL;
    NAS_ESM_ENCODE_INFO_STRU            stEncodeInfo;

    /* ���PTI��PTI��ΪRESERVED����PTI��Ϊ0ʱ��ͨ����PTI�ܼ�����״̬��
       �еļ�¼������ظ��ܾ���Ϣ*/
    enEsmCause = NAS_ESM_ValidatePtiInActDedictOrModOrDeactOrStatusMsg(pstNwMsg->ucPti,
                                                                       &ulStateTblId);
    if (enEsmCause != NAS_ESM_CAUSE_SUCCESS)
    {
        NAS_ESM_LOG1("NAS_ESM_ValidateActDedictEpsbReqMsg:WARN:Validate Pti failed, cause : ", enEsmCause);
        NAS_ESM_ProcNwMsgForIllegalPti(pstNwMsg, enEsmCause);
        return NAS_ESM_MSG_HANDLED;
    }

    /* �����غŷ�Χ������λ��5-15֮�䣬����ظ��ܾ���Ϣ */
    enEsmCause = NAS_ESM_ValidateEpsbIdInActMsg(pstNwMsg->ucEpsbId);
    if (enEsmCause != NAS_ESM_CAUSE_SUCCESS)
    {
        NAS_ESM_LOG1("NAS_ESM_ValidateActDedictEpsbReqMsg:WARN:Validate EpsbId failed, cause : ", enEsmCause);
        NAS_ESM_ProcNwMsgForIllegalEpsbId(pstNwMsg);
        return NAS_ESM_MSG_HANDLED;
    }

    /*��ʼ���������NAS_ESM_ENCODE_INFO_STRU*/
    NAS_ESM_MEM_SET((VOS_VOID*)&stEncodeInfo, 0, sizeof(NAS_ESM_ENCODE_INFO_STRU));
    stEncodeInfo.bitOpESMCau = NAS_ESM_OP_TRUE;
    stEncodeInfo.ucPti       = pstNwMsg->ucPti;
    stEncodeInfo.ulEpsbId    = (VOS_UINT32)pstNwMsg->ucEpsbId;

    /* ���������غŷ�Χ������λ��5-15֮�䣬����Ϊ�����ȱʡ���أ�����
       �ظ��ܾ���Ϣ������ȱʡ���ش���PDN DISCONNECT�����У���������Ϣ */
    enEsmCause = NAS_ESM_ValidateLinkedEpsbId(pstNwMsg->ucLinkedEpsbId);
    if (enEsmCause != NAS_ESM_CAUSE_SUCCESS)
    {
        NAS_ESM_LOG1("NAS_ESM_ValidateActDedictEpsbReqMsg:WARN:Validate Linked EpsbId failed, cause : ", enEsmCause);
        stEncodeInfo.ucESMCau = enEsmCause;
        NAS_ESM_ActDediBearFailProc(&stEncodeInfo, ulStateTblId);
        return NAS_ESM_MSG_HANDLED;
    }

    /* ���TFT */
    if (NAS_ESM_SUCCESS != NAS_ESM_ValidateTftInActDedictMsg(pstNwMsg,
                                                             ulStateTblId))
    {
        return NAS_ESM_MSG_HANDLED;
    }

    return NAS_ESM_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_ValidateModEpsbReqMsg
 Description     : �Գ����޸�������Ϣ���кϷ��Լ��
 Input           : pstNwMsg-------------------------�����Ŀտ���Ϣָ��
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2010-5-12  Draft Enact

*****************************************************************************/
static VOS_UINT32  NAS_ESM_ValidateModEpsbReqMsg
(
    const NAS_ESM_NW_MSG_STRU          *pstNwMsg
)
{
    NAS_ESM_CAUSE_ENUM_UINT8            enEsmCause          = NAS_ESM_NULL;
    VOS_UINT32                          ulStateTblId        = NAS_ESM_NULL;
    NAS_ESM_ENCODE_INFO_STRU            stEncodeInfo;
    NAS_ESM_EPSB_CNTXT_INFO_STRU       *pstTmpEpsbCntxtInfo = VOS_NULL_PTR;


    /* ���PTI��PTI��ΪRESERVED����PTI��Ϊ0ʱ��ͨ����PTI�ܼ�����״̬��
          �еļ�¼������ظ��ܾ���Ϣ*/
    enEsmCause = NAS_ESM_ValidatePtiInActDedictOrModOrDeactOrStatusMsg(pstNwMsg->ucPti,
                                                                       &ulStateTblId);
    if (enEsmCause != NAS_ESM_CAUSE_SUCCESS)
    {
        NAS_ESM_LOG1("NAS_ESM_ValidateModEpsbReqMsg:WARN:Validate Pti failed, cause : ", enEsmCause);
        NAS_ESM_ProcNwMsgForIllegalPti(pstNwMsg, enEsmCause);
        return NAS_ESM_MSG_HANDLED;
    }

    /* �����غŷ�Χ������λ��5-15֮�䣬����Ϊ����̬������ظ��ܾ���Ϣ */
    enEsmCause = NAS_ESM_ValidateEpsbIdInModOrDeactMsg(pstNwMsg->ucEpsbId);
    if (enEsmCause != NAS_ESM_CAUSE_SUCCESS)
    {
        NAS_ESM_LOG1("NAS_ESM_ValidateModEpsbReqMsg:WARN:Validate EpsbId failed, cause : ", enEsmCause);
        NAS_ESM_ProcNwMsgForIllegalEpsbId(pstNwMsg);
        return NAS_ESM_MSG_HANDLED;
    }

    /*��ʼ���������NAS_ESM_ENCODE_INFO_STRU*/
    NAS_ESM_MEM_SET((VOS_VOID*)&stEncodeInfo, 0, sizeof(NAS_ESM_ENCODE_INFO_STRU));
    stEncodeInfo.bitOpESMCau = NAS_ESM_OP_TRUE;
    stEncodeInfo.ucPti       = pstNwMsg->ucPti;
    stEncodeInfo.ulEpsbId    = (VOS_UINT32)pstNwMsg->ucEpsbId;

    /* ������Ϣ�Ƿ���PDN DISCONNECT���̳�ͻ��������������Ϣ */
    pstTmpEpsbCntxtInfo = NAS_ESM_GetEpsbCntxtInfoAddr(pstNwMsg->ucEpsbId);
    if(NAS_ESM_BEARER_TYPE_DEDICATED == pstTmpEpsbCntxtInfo->enBearerCntxtType)
    {
        enEsmCause = NAS_ESM_ValidateLinkedEpsbId((VOS_UINT8)pstTmpEpsbCntxtInfo->ulLinkedEpsbId);
    }
    else
    {
        enEsmCause = NAS_ESM_ValidateLinkedEpsbId(pstNwMsg->ucEpsbId);
    }

    if (enEsmCause != NAS_ESM_CAUSE_SUCCESS)
    {
        NAS_ESM_LOG1("NAS_ESM_ValidateActDedictEpsbReqMsg:WARN:Validate Linked EpsbId failed, cause : ", enEsmCause);
        stEncodeInfo.ucESMCau = enEsmCause;
        NAS_ESM_BearModFailProc(&stEncodeInfo, ulStateTblId);
        return NAS_ESM_MSG_HANDLED;
    }

    /* ���TFT */
    if (NAS_ESM_SUCCESS != NAS_ESM_ValidateTftInModMsg(pstNwMsg, ulStateTblId))
    {
        return NAS_ESM_MSG_HANDLED;
    }

    /* ���QOS */
    enEsmCause = NAS_ESM_ValidateQosInModMsg(   pstNwMsg,
                                                NAS_ESM_GetEpsbCntxtInfoAddr(pstNwMsg->ucEpsbId));
    if (enEsmCause != NAS_ESM_CAUSE_SUCCESS)
    {
        NAS_ESM_LOG1("NAS_ESM_ValidateActDedictEpsbReqMsg:WARN:NAS_ESM_ValidateQosInModMsg failed! ", enEsmCause);
        stEncodeInfo.ucESMCau = enEsmCause;
        NAS_ESM_BearModFailProc(&stEncodeInfo, ulStateTblId);
        return NAS_ESM_MSG_HANDLED;
    }

    #if 0
    /* ���APN-AMBR */
    enEsmCause = NAS_ESM_ValidateApnAmbrInModMsg(pstNwMsg);
    if (enEsmCause != NAS_ESM_CAUSE_SUCCESS)
    {
        NAS_ESM_LOG1("NAS_ESM_ValidateActDedictEpsbReqMsg:WARN:Validate Linked EpsbId failed, cause : ", enEsmCause);
        stEncodeInfo.ucESMCau = enEsmCause;
        NAS_ESM_BearModFailProc(&stEncodeInfo, ulStateTblId);
        return NAS_ESM_MSG_HANDLED;
    }
    #endif

    return NAS_ESM_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_ValidateDeactEpsbReqMsg
 Description     : �Գ���ȥ����������Ϣ���кϷ��Լ��
 Input           : pstNwMsg-------------------------�����Ŀտ���Ϣָ��
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2010-5-12  Draft Enact

*****************************************************************************/
static VOS_UINT32  NAS_ESM_ValidateDeactEpsbReqMsg
(
    const NAS_ESM_NW_MSG_STRU          *pstNwMsg
)
{
    NAS_ESM_CAUSE_ENUM_UINT8            enEsmCause      = NAS_ESM_NULL;
    VOS_UINT32                          ulStateTblId    = NAS_ESM_NULL;

    /* ���PTI����PTI��Ϊ0ʱ����ͨ����PTI���ܼ�����״̬���¼��������PTIΪ
       RESERVED����������Ϣ*/
    enEsmCause = NAS_ESM_ValidatePtiInActDedictOrModOrDeactOrStatusMsg( pstNwMsg->ucPti,
                                                                        &ulStateTblId);
    if (enEsmCause != NAS_ESM_CAUSE_SUCCESS)
    {
        NAS_ESM_LOG1("NAS_ESM_ValidateDeactEpsbReqMsg:WARN:Validate Pti failed, cause : ", enEsmCause);
        return NAS_ESM_MSG_HANDLED;
    }

    /* �����غŷ�Χ������λ��5-15֮�䣬����Ϊ����̬������ֱ�ӻظ�ACCEPT��Ϣ */
    enEsmCause = NAS_ESM_ValidateEpsbIdInModOrDeactMsg(pstNwMsg->ucEpsbId);
    if (enEsmCause != NAS_ESM_CAUSE_SUCCESS)
    {
        NAS_ESM_LOG1("NAS_ESM_ValidateDeactEpsbReqMsg:WARN:Validate EpsbId failed, cause : ", enEsmCause);
        NAS_ESM_ProcNwMsgForIllegalEpsbId(pstNwMsg);
        return NAS_ESM_MSG_HANDLED;
    }

    return NAS_ESM_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_ValidateNwRejMsg
 Description     : ������ܾ���Ϣ���кϷ��Լ��
 Input           : pstNwMsg-------------------------�����Ŀտ���Ϣָ��
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2010-5-12  Draft Enact

*****************************************************************************/
static VOS_UINT32  NAS_ESM_ValidateNwRejMsg
(
    const NAS_ESM_NW_MSG_STRU          *pstNwMsg
)
{
    NAS_ESM_CAUSE_ENUM_UINT8            enEsmCause = NAS_ESM_NULL;

    /* ���PTI��PTI��Ϊ0����ΪRESERVED��ͨ����PTI�ܼ�����״̬��
       �еļ�¼������������Ϣ */
    enEsmCause = NAS_ESM_ValidatePtiInActDefltOrInfoOrRejMsg(pstNwMsg->ucPti);
    if (enEsmCause != NAS_ESM_CAUSE_SUCCESS)
    {
        NAS_ESM_LOG1("NAS_ESM_ValidateNwRejMsg:WARN:Validate Pti failed, cause : ", enEsmCause);
        return NAS_ESM_MSG_HANDLED;
    }

    /* �����غţ�����Ϊ0������������Ϣ */
    enEsmCause = NAS_ESM_ValidateEpsbIdInRejOrInfoMsg(pstNwMsg->ucEpsbId);
    if (enEsmCause != NAS_ESM_CAUSE_SUCCESS)
    {
        NAS_ESM_LOG1("NAS_ESM_ValidateNwRejMsg:WARN:Validate EpsbId failed, cause : ", enEsmCause);
        return NAS_ESM_MSG_HANDLED;
    }

    return NAS_ESM_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_ValidateEsmInfoReqMsg
 Description     : ��ESM INFO������Ϣ���кϷ��Լ��
 Input           : pstNwMsg-------------------------�����Ŀտ���Ϣָ��
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2010-5-12  Draft Enact

*****************************************************************************/
static VOS_UINT32  NAS_ESM_ValidateEsmInfoReqMsg
(
    const NAS_ESM_NW_MSG_STRU          *pstNwMsg
)
{
    NAS_ESM_CAUSE_ENUM_UINT8            enEsmCause = NAS_ESM_NULL;

    /* ���PTI��PTI��Ϊ0������ΪRESERVED����ֱ�Ӷ�������Ϣ����ͨ����PTI����
       ������״̬���еļ�¼����ظ�ESM STATUS��Ϣ */
    enEsmCause = NAS_ESM_ValidatePtiInActDefltOrInfoOrRejMsg(pstNwMsg->ucPti);
    if (enEsmCause != NAS_ESM_CAUSE_SUCCESS)
    {
        NAS_ESM_LOG1("NAS_ESM_ValidateEsmInfoReqMsg:WARN:Validate Pti failed, cause : ", enEsmCause);
        if (enEsmCause == NAS_ESM_CAUSE_PTI_MISMATCH)
        {
            NAS_ESM_ProcNwMsgForIllegalPti(pstNwMsg, enEsmCause);
        }
        return NAS_ESM_MSG_HANDLED;
    }

    /* �����غţ�����Ϊ0������ظ�ESM STATUS��Ϣ */
    enEsmCause = NAS_ESM_ValidateEpsbIdInRejOrInfoMsg(pstNwMsg->ucEpsbId);
    if (enEsmCause != NAS_ESM_CAUSE_SUCCESS)
    {
        NAS_ESM_LOG1("NAS_ESM_ValidateEsmInfoReqMsg:WARN:Validate EpsbId failed, cause : ", enEsmCause);
        NAS_ESM_ProcNwMsgForIllegalEpsbId(pstNwMsg);
        return NAS_ESM_MSG_HANDLED;
    }

    return NAS_ESM_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_ValidateEsmStatusMsg
 Description     : ��ESM STATUS��Ϣ���кϷ��Լ��
 Input           : pstNwMsg-------------------------�����Ŀտ���Ϣָ��
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2010-5-12  Draft Enact

*****************************************************************************/
static VOS_UINT32  NAS_ESM_ValidateEsmStatusMsg
(
    const NAS_ESM_NW_MSG_STRU          *pstNwMsg
)
{
    NAS_ESM_CAUSE_ENUM_UINT8            enEsmCause      = NAS_ESM_NULL;
    VOS_UINT32                          ulStateTblId    = NAS_ESM_NULL;

    /* ���PTI��PTIΪRESERVED�����ߵ�PTI��Ϊ0����ͨ����PTI���ܼ�����״̬��
       �еļ�¼����������Ϣ */
    enEsmCause = NAS_ESM_ValidatePtiInActDedictOrModOrDeactOrStatusMsg( pstNwMsg->ucPti,
                                                                        &ulStateTblId);
    if (enEsmCause != NAS_ESM_CAUSE_SUCCESS)
    {
        NAS_ESM_LOG1("NAS_ESM_ValidateEsmStatusMsg:WARN:Validate Pti failed, cause : ", enEsmCause);
        return NAS_ESM_MSG_HANDLED;
    }

    return NAS_ESM_SUCCESS;
}


VOS_UINT32  NAS_ESM_UpdateStatetblNwInitProcedure
(
    const NAS_ESM_NW_MSG_STRU           *pRcvMsg,
    VOS_UINT32                          *pulStateTblId
)
{
    VOS_UINT32                          ulRslt              = NAS_ESM_NULL;
    VOS_UINT32                          ulCId               = NAS_ESM_NULL;
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr        = VOS_NULL_PTR;

    NAS_ESM_INFO_LOG("NAS_ESM_UpdateStatetblNwInitProcedure is entered.");

    ulRslt = NAS_ESM_QuerySdfId(pRcvMsg,&ulCId);

    if ((NAS_ESM_QUERY_CID_RSLT_ALL_FOUND_EQUAL_CID != ulRslt)
     && (NAS_ESM_QUERY_CID_RSLT_PART_FOUND_EQUAL_CID != ulRslt))
    {
        ulCId = NAS_ESM_GetCid(pRcvMsg->ucEpsbId);
    }

    if (NAS_ESM_SUCCESS == NAS_ESM_GetStateTblIndexByCid(ulCId,pulStateTblId))
    {
        pstStateAddr = NAS_ESM_GetStateTblAddr(*pulStateTblId);

        /*��д״̬����Ϣ����������״̬������Id����������*/
        pstStateAddr->ulEpsbId = pRcvMsg->ucEpsbId;
        pstStateAddr->enBearerCntxtState = NAS_ESM_GetBearCntxtState(pstStateAddr->ulEpsbId);
        pstStateAddr->enBearerCntxtType  = NAS_ESM_GetBearCntxtType(pstStateAddr->ulEpsbId);
        pstStateAddr->ulCid = ulCId;
    }
    else
    {
        NAS_ESM_WARN_LOG("NAS_ESM_UpdateStatetblNwInitProcedure:WARNING:insufficient resource!");

        /*������Ϣ���ݣ��ظ��ܾ������*/
        return NAS_ESM_MSG_HANDLED;
    }

    return NAS_ESM_SUCCESS;
}



VOS_VOID  NAS_ESM_ProcNwMsgActDedictdEpsbReq
(
    const EMM_ESM_INTRA_DATA_IND_STRU        *pRcvMsg
)
{
    VOS_UINT32                          ulCId               = NAS_ESM_NULL;
    VOS_UINT32                          ulStateTblIndex     = NAS_ESM_NULL;
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr        = VOS_NULL_PTR;

    /* ��֤�տ���Ϣ,����֤TFT ���ȼ���ͻ����� */
    if (NAS_ESM_MSG_HANDLED == NAS_ESM_ValidateActDedictEpsbReqMsg(&pRcvMsg->stEsmNwMsgIE))
    {
        NAS_ESM_NORM_LOG("NAS_ESM_ProcNwMsgActDedictdEpsbReq: Msg is Validated and return!");
        return;
    }

    /* ��������Ѿ�����ȱ���ȥ���� */
    if(NAS_ESM_BEARER_STATE_ACTIVE == NAS_ESM_GetBearCntxtState(pRcvMsg->stEsmNwMsgIE.ucEpsbId))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgActDedictdEpsbReq : release already active bearer!");
        NAS_ESM_DeactBearerAndInformApp(pRcvMsg->stEsmNwMsgIE.ucEpsbId);
    }


    /* ����̬���Cid */
    if (NAS_ESM_PTI_UNASSIGNED_VALUE == pRcvMsg->stEsmNwMsgIE.ucPti )
    {
        ulCId = NAS_ESM_GetCid(pRcvMsg->stEsmNwMsgIE.ucEpsbId);

        if (NAS_ESM_SUCCESS == NAS_ESM_GetStateTblIndexByCid(ulCId,&ulStateTblIndex))
        {
            pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

            /*��д״̬����Ϣ����������״̬������Id����������*/
            pstStateAddr->ulEpsbId = pRcvMsg->stEsmNwMsgIE.ucEpsbId;
            pstStateAddr->enBearerCntxtState = NAS_ESM_GetBearCntxtState(pstStateAddr->ulEpsbId);
            pstStateAddr->enBearerCntxtType  = NAS_ESM_GetBearCntxtType(pstStateAddr->ulEpsbId);
            pstStateAddr->ulCid = ulCId;
        }
        else
        {
            NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgActDedictdEpsbReq:state table not enough!");
            return;
        }
    }
    else
    {
        if(NAS_ESM_FAILURE == NAS_ESM_QueryStateTblIndexByPti(pRcvMsg->stEsmNwMsgIE.ucPti,
                                                             &ulStateTblIndex))
        {
            NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgActDedictdEpsbReq:WARN:PTI mismatach!");
            return ;
        }

        pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

        /* ���������·���PTI�ѷ��䣬���ǳ��غŲ�ͬ����� */
        NAS_ESM_ProcNwMsgActDediBearSamePtiWithDiffEBI(pRcvMsg,ulStateTblIndex,pstStateAddr);

    }

    /* �����Ѿ���֤û������Ŀտ���Ϣ */
    NAS_ESM_ProcValidNwMsgActDedictdEpsbReq(ulStateTblIndex,pRcvMsg);
}


VOS_VOID  NAS_ESM_ProcNwMsgModEpsbReq
(
    const EMM_ESM_INTRA_DATA_IND_STRU        *pRcvMsg
)
{
    VOS_UINT32                          ulStateTblIndex     = NAS_ESM_NULL;
    VOS_UINT32                          ulRslt              = NAS_ESM_NULL;
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr        = VOS_NULL_PTR;

    /* ��֤�տ���Ϣ,����֤TFT ���ȼ���ͻ����� */
    if (NAS_ESM_MSG_HANDLED == NAS_ESM_ValidateModEpsbReqMsg(&pRcvMsg->stEsmNwMsgIE))
    {
        NAS_ESM_NORM_LOG("NAS_ESM_ProcNwMsgModEpsbReq: Msg is Validated and return!");
        return;
    }

    /*���ݳ���ID��������緢���������UE����������Ƿ��г�ͻ*/
    if(NAS_ESM_SUCCESS == NAS_ESM_QueryStateTblIndexByEpsbId(pRcvMsg->stEsmNwMsgIE.ucEpsbId,
                                                            &ulStateTblIndex))
    {
        /*�����UEͬʱ��ͬһ���ؽ��в�����UE��������ĳ����޸�����*/
        if(NAS_ESM_PTI_UNASSIGNED_VALUE == pRcvMsg->stEsmNwMsgIE.ucPti )
        {
            NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgModEpsbReq:Collision of NW orig procedure!");
            return;
        }
    }

    /* ����̬���Cid */
    if (NAS_ESM_PTI_UNASSIGNED_VALUE == pRcvMsg->stEsmNwMsgIE.ucPti )
    {
        ulRslt = NAS_ESM_UpdateStatetblNwInitProcedure(&pRcvMsg->stEsmNwMsgIE,
                                                       &ulStateTblIndex);
        if (NAS_ESM_MSG_HANDLED == ulRslt)
        {
            NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgModEpsbReq:Update state table error!");
            return ;
        }
    }
    else
    {
        NAS_ESM_NORM_LOG("NAS_ESM_ProcNwMsgModEpsbReq:NORM:Ue Init procedure!");

        if(NAS_ESM_FAILURE == NAS_ESM_QueryStateTblIndexByPti(pRcvMsg->stEsmNwMsgIE.ucPti,
                                                              &ulStateTblIndex))
        {
            NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgModEpsbReq:WARN:PTI mismatach!");
            return ;
        }

        pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

        /* �û�����ĳ����޸ĺ�ȥ��������¶�̬����Ϣ */

        NAS_ESM_ProcNwMsgModDediBearSamePtiWithDiffEBI(pRcvMsg,ulStateTblIndex,pstStateAddr);

    }

    /* �����Ѿ���֤û������Ŀտ���Ϣ */
    NAS_ESM_ProcValidNwMsgModEpsbReq(ulStateTblIndex,pRcvMsg);
}


VOS_VOID  NAS_ESM_ProcNwMsgActDefltEpsbReq
(
    const EMM_ESM_INTRA_DATA_IND_STRU        *pRcvMsg
)
{
    VOS_UINT32                          ulStateTblIndex     = NAS_ESM_NULL;
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr        = VOS_NULL_PTR;

    /* ��֤�տ���Ϣ�Ϸ��� */
    if (NAS_ESM_MSG_HANDLED == NAS_ESM_ValidateActDefltEpsbReqMsg(&pRcvMsg->stEsmNwMsgIE))
    {
        NAS_ESM_NORM_LOG("NAS_ESM_ProcNwMsgActDefltEpsbReq: Msg is Validated and return!");
        return;
    }

    /* ��������Ѿ�����ȱ���ȥ���� */
    if(NAS_ESM_BEARER_STATE_ACTIVE == NAS_ESM_GetBearCntxtState(pRcvMsg->stEsmNwMsgIE.ucEpsbId))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgActDefltEpsbReq : release already active bearer!");
        NAS_ESM_DeactBearerAndInformApp(pRcvMsg->stEsmNwMsgIE.ucEpsbId);
    }

    /*����PTI���ҵ���̬��Index����û��ѯ��������쳣����*/
    if(NAS_ESM_FAILURE == NAS_ESM_QueryStateTblIndexByPti(pRcvMsg->stEsmNwMsgIE.ucPti, &ulStateTblIndex))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgActDefltEpsbReq:WARN:PTI mismatach!");
        return ;
    }

    pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

    pstStateAddr->ulEpsbId = pRcvMsg->stEsmNwMsgIE.ucEpsbId;

    /* �����Ѿ���֤û������Ŀտ���Ϣ */
    NAS_ESM_ProcValidNwMsgActDefltEpsbReq(ulStateTblIndex,pRcvMsg);
}


VOS_VOID  NAS_ESM_ProcNwMsgDeactEpsbReq
(
    const EMM_ESM_INTRA_DATA_IND_STRU        *pRcvMsg
)
{
    VOS_UINT32                          ulStateTblIndex     = NAS_ESM_NULL;
    VOS_UINT32                          ulCId               = NAS_ESM_NULL;
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr        = VOS_NULL_PTR;

    /* ��֤�տ���Ϣ */
    if (NAS_ESM_MSG_HANDLED == NAS_ESM_ValidateDeactEpsbReqMsg(&pRcvMsg->stEsmNwMsgIE))
    {
        NAS_ESM_NORM_LOG("NAS_ESM_ProcNwMsgDeactEpsbReq: Msg is Validated and return!");
        return;
    }

    /*���ݳ���ID��������緢���������UE����������Ƿ��г�ͻ*/
    if(NAS_ESM_SUCCESS == NAS_ESM_QueryStateTblIndexByEpsbId(pRcvMsg->stEsmNwMsgIE.ucEpsbId,
                                                            &ulStateTblIndex))
    {
        if(NAS_ESM_PTI_UNASSIGNED_VALUE == pRcvMsg->stEsmNwMsgIE.ucPti )
        {
            NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgDeactEpsbReq:Collision of NW orig procedure!");

            NAS_ESM_ProcNwMsgDeactProcedureCollision(ulStateTblIndex,&pRcvMsg->stEsmNwMsgIE);
            return;
        }
    }


    /* ����̬���Cid */
    if (NAS_ESM_PTI_UNASSIGNED_VALUE == pRcvMsg->stEsmNwMsgIE.ucPti )
    {
        (VOS_VOID)NAS_ESM_QuerySdfId(&pRcvMsg->stEsmNwMsgIE,&ulCId);
        if (NAS_ESM_SUCCESS == NAS_ESM_GetStateTblIndexByCid(ulCId,&ulStateTblIndex))
        {
            pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

            /*��д״̬����Ϣ����������״̬������Id����������*/
            pstStateAddr->ulEpsbId = pRcvMsg->stEsmNwMsgIE.ucEpsbId;
            pstStateAddr->enBearerCntxtState = NAS_ESM_GetBearCntxtState(pstStateAddr->ulEpsbId);
            pstStateAddr->enBearerCntxtType  = NAS_ESM_GetBearCntxtType(pstStateAddr->ulEpsbId);
            pstStateAddr->ulCid = ulCId;
        }
        else
        {
            NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgDeactEpsbReq:state table not enough!");
            return;
        }
    }
    else
    {
        if(NAS_ESM_FAILURE == NAS_ESM_QueryStateTblIndexByPti(pRcvMsg->stEsmNwMsgIE.ucPti,
                                                             &ulStateTblIndex))
        {
            NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgDeactEpsbReq:WARN:PTI mismatach!");
            return ;
        }
        pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);
        NAS_ESM_ProcNwMsgDeaDediBearSamePtiWithDiffEBI(pRcvMsg,ulStateTblIndex,pstStateAddr);
    }


    /* �����Ѿ���֤û������Ŀտ���Ϣ */
    NAS_ESM_ProcValidNwMsgDeactEpsbReq(ulStateTblIndex,pRcvMsg);
}
VOS_VOID  NAS_ESM_ProcNwMsgPdnConnRej
(
    const EMM_ESM_INTRA_DATA_IND_STRU        *pRcvMsg
)
{
    VOS_UINT32                          ulStateTblIndex     = NAS_ESM_NULL;

    /* ��֤�տ���Ϣ,����֤TFT ���ȼ���ͻ����� */
    if (NAS_ESM_MSG_HANDLED == NAS_ESM_ValidateNwRejMsg(&pRcvMsg->stEsmNwMsgIE))
    {
        NAS_ESM_NORM_LOG("NAS_ESM_ProcNwMsgPdnConnRej: Msg is Validated and return!");
        return;
    }

    /*����PTI���ҵ���̬��Index����û��ѯ������*/
    if(NAS_ESM_FAILURE == NAS_ESM_QueryStateTblIndexByPti(pRcvMsg->stEsmNwMsgIE.ucPti, &ulStateTblIndex))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgPdnConnRej:WARN:PTI mismatach!");
        return ;
    }

    /* xiongxianghui00253310 modify for ftmerrlog begin */
    #if (FEATURE_PTM == FEATURE_ON)
    NAS_ESM_ErrlogInfoProc(pRcvMsg->stEsmNwMsgIE.enEsmCau);
    #endif
    /* xiongxianghui00253310 modify for ftmerrlog begin */

    /* �����Ѿ���֤û������Ŀտ���Ϣ */
    NAS_ESM_ProcValidNwMsgPdnConnRej(ulStateTblIndex,pRcvMsg);
}
VOS_VOID  NAS_ESM_ProcNwMsgPdnDisconRej
(
    const EMM_ESM_INTRA_DATA_IND_STRU        *pRcvMsg
)
{
    VOS_UINT32                          ulStateTblIndex     = NAS_ESM_NULL;

    /* ��֤�տ���Ϣ,����֤TFT ���ȼ���ͻ����� */
    if (NAS_ESM_MSG_HANDLED == NAS_ESM_ValidateNwRejMsg(&pRcvMsg->stEsmNwMsgIE))
    {
        NAS_ESM_NORM_LOG("NAS_ESM_ProcNwMsgPdnDisconRej: Msg is Validated and return!");
        return;
    }

    /*����PTI���ҵ���̬��Index����û��ѯ������*/
    if(NAS_ESM_FAILURE == NAS_ESM_QueryStateTblIndexByPti(pRcvMsg->stEsmNwMsgIE.ucPti, &ulStateTblIndex))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgPdnDisconRej:WARN:PTI mismatach!");
        return ;
    }

    /* xiongxianghui00253310 modify for ftmerrlog begin */
    #if (FEATURE_PTM == FEATURE_ON)
    NAS_ESM_ErrlogInfoProc(pRcvMsg->stEsmNwMsgIE.enEsmCau);
    #endif
    /* xiongxianghui00253310 modify for ftmerrlog begin */

    /* �����Ѿ���֤û������Ŀտ���Ϣ */
    NAS_ESM_ProcValidNwMsgPdnDisconRej(ulStateTblIndex,pRcvMsg);
}
VOS_VOID  NAS_ESM_ProcNwMsgResModRej
(
    const EMM_ESM_INTRA_DATA_IND_STRU        *pRcvMsg
)
{
    VOS_UINT32                          ulStateTblIndex     = NAS_ESM_NULL;

    /* ��֤�տ���Ϣ,����֤TFT ���ȼ���ͻ����� */
    if (NAS_ESM_MSG_HANDLED == NAS_ESM_ValidateNwRejMsg(&pRcvMsg->stEsmNwMsgIE))
    {
        NAS_ESM_NORM_LOG("NAS_ESM_ProcNwMsgResModRej: Msg is Validated and return!");
        return;
    }

    /*����PTI���ҵ���̬��Index����û��ѯ������*/
    if(NAS_ESM_FAILURE == NAS_ESM_QueryStateTblIndexByPti(pRcvMsg->stEsmNwMsgIE.ucPti, &ulStateTblIndex))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgResModRej:WARN:PTI mismatach!");
        return ;
    }

    /* xiongxianghui00253310 modify for ftmerrlog begin */
    #if (FEATURE_PTM == FEATURE_ON)
    NAS_ESM_ErrlogInfoProc(pRcvMsg->stEsmNwMsgIE.enEsmCau);
    #endif
    /* xiongxianghui00253310 modify for ftmerrlog begin */

    /* �����Ѿ���֤û������Ŀտ���Ϣ */
    NAS_ESM_ProcValidNwMsgResModRej(ulStateTblIndex,pRcvMsg);
}
VOS_VOID  NAS_ESM_ProcNwMsgResAllocRej
(
    const EMM_ESM_INTRA_DATA_IND_STRU        *pRcvMsg
)
{
    VOS_UINT32                          ulStateTblIndex     = NAS_ESM_NULL;

    /* ��֤�տ���Ϣ,����֤TFT ���ȼ���ͻ����� */
    if (NAS_ESM_MSG_HANDLED == NAS_ESM_ValidateNwRejMsg(&pRcvMsg->stEsmNwMsgIE))
    {
        NAS_ESM_NORM_LOG("NAS_ESM_ProcNwMsgResAllocRej: Msg is Validated and return!");
        return;
    }

    /*����PTI���ҵ���̬��Index����û��ѯ������*/
    if(NAS_ESM_FAILURE == NAS_ESM_QueryStateTblIndexByPti(pRcvMsg->stEsmNwMsgIE.ucPti, &ulStateTblIndex))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgResAllocRej:WARN:PTI mismatach!");
        return ;
    }

    /* xiongxianghui00253310 modify for ftmerrlog begin */
    #if (FEATURE_PTM == FEATURE_ON)
    NAS_ESM_ErrlogInfoProc(pRcvMsg->stEsmNwMsgIE.enEsmCau);
    #endif
    /* xiongxianghui00253310 modify for ftmerrlog begin */

    /* �����Ѿ���֤û������Ŀտ���Ϣ */
    NAS_ESM_ProcValidNwMsgResAllocRej(ulStateTblIndex,pRcvMsg);
}
VOS_VOID  NAS_ESM_ProcNwMsgEsmInfoReq
(
    const EMM_ESM_INTRA_DATA_IND_STRU        *pRcvMsg
)
{
    /* ��֤�տ���Ϣ,����֤TFT ���ȼ���ͻ����� */
    if (NAS_ESM_MSG_HANDLED == NAS_ESM_ValidateEsmInfoReqMsg(&pRcvMsg->stEsmNwMsgIE))
    {
        NAS_ESM_NORM_LOG("NAS_ESM_ProcNwMsgEsmInfoReq: Msg is Validated and return!");
        return;
    }

    /* �����Ѿ���֤û������Ŀտ���Ϣ */
    NAS_ESM_ProcValidNwMsgEsmInfoReq(&pRcvMsg->stEsmNwMsgIE);
}



VOS_VOID  NAS_ESM_ProcNwsMsgEsmStatus
(
    const EMM_ESM_INTRA_DATA_IND_STRU        *pRcvMsg
)
{
    /* ��֤�տ���Ϣ,����֤TFT ���ȼ���ͻ����� */
    if (NAS_ESM_MSG_HANDLED == NAS_ESM_ValidateEsmStatusMsg(&pRcvMsg->stEsmNwMsgIE))
    {
        NAS_ESM_NORM_LOG("NAS_ESM_ProcNwsMsgEsmStatus: Msg is Validated and return!");
        return;
    }

    /* �����Ѿ���֤û������Ŀտ���Ϣ */
    NAS_ESM_ProcValidNwsMsgEsmStatus(&pRcvMsg->stEsmNwMsgIE);
}


VOS_VOID NAS_ESM_ProcNwMsgEsmNotification
(
    const EMM_ESM_INTRA_DATA_IND_STRU        *pRcvMsg
)
{
    VOS_UINT32 ulStateId;
    VOS_UINT8 ucRcvPti;
    VOS_UINT8 ucRcvEbi;
    NAS_ESM_ENCODE_INFO_STRU stEncodeInfo = {0};
    VOS_UINT32 ulMsgLen = 0;

    NAS_ESM_INFO_LOG("NAS_ESM_ProcNwMsgEsmNotification: entered");

    if (NAS_ESM_NULL == pRcvMsg)
    {
        NAS_ESM_ERR_LOG("NAS_ESM_ProcNwMsgEsmNotification: invalid parameter");
        return;
    }

    ucRcvPti = pRcvMsg->stEsmNwMsgIE.ucPti;
    ucRcvEbi = pRcvMsg->stEsmNwMsgIE.ucEpsbId;

    stEncodeInfo.bitOpESMCau = NAS_ESM_OP_TRUE;
    stEncodeInfo.ucPti       = ucRcvPti;
    stEncodeInfo.ulEpsbId    = ucRcvEbi;

    if (NAS_ESM_PTI_RESERVED_VALUE == ucRcvPti)
    {
        /* If the PTI is a reserved value, the UE shall respond with an ESM
           STATUS message including ESM cause #81 "invalid PTI value" */
        stEncodeInfo.ucESMCau = NAS_ESM_CAUSE_INVALID_PTI_VALUE;

        NAS_ESM_EncodeEsmStatus(stEncodeInfo, NAS_ESM_GetCurEsmSndNwMsgAddr(), &ulMsgLen);
        NAS_ESM_SndEsmEmmDataReqMsg(NAS_ESM_ILLEGAL_OPID, PS_FALSE, ulMsgLen, NAS_ESM_GetCurEsmSndNwMsgAddr());
    }
    else if ((NAS_ESM_PTI_UNASSIGNED_VALUE != ucRcvPti) &&
             (NAS_ESM_SUCCESS != NAS_ESM_QueryStateTblIndexByPti(ucRcvPti, &ulStateId)))
    {
        /* If the PTI is an assigned value that does not match any PTI in use,
           the UE shall respond with an ESM STATUS message including ESM cause
           #47 "PTI mismatch" */
        stEncodeInfo.ucESMCau = NAS_ESM_CAUSE_PTI_MISMATCH;

        NAS_ESM_EncodeEsmStatus(stEncodeInfo, NAS_ESM_GetCurEsmSndNwMsgAddr(), &ulMsgLen);
        NAS_ESM_SndEsmEmmDataReqMsg(NAS_ESM_ILLEGAL_OPID, PS_FALSE, ulMsgLen, NAS_ESM_GetCurEsmSndNwMsgAddr());
    }
    else if (((ucRcvEbi > NAS_ESM_UNASSIGNED_EPSB_ID) && (ucRcvEbi < NAS_ESM_MIN_EPSB_ID)) ||
             (NAS_ESM_SUCCESS != NAS_ESM_QueryStateTblIndexByEpsbId(ucRcvEbi, &ulStateId)) ||
             ((NAS_ESM_UNASSIGNED_EPSB_ID == ucRcvEbi) && (NAS_ESM_PTI_UNASSIGNED_VALUE == ucRcvPti)))
    {
        /* 1. includes a reserved EPS bearer identity value
           2. includes a assigned EPS bearer identity value that does not match
              an existing EPS bearer context
           3. includes the combination of an unassigned PTI value and an
              unassigned EPS bearer identity value  */
        stEncodeInfo.ucESMCau = NAS_ESM_CAUSE_INVALID_EPS_BEARER_IDENTITY;

        NAS_ESM_EncodeEsmStatus(stEncodeInfo, NAS_ESM_GetCurEsmSndNwMsgAddr(), &ulMsgLen);
        NAS_ESM_SndEsmEmmDataReqMsg(NAS_ESM_ILLEGAL_OPID, PS_FALSE, ulMsgLen, NAS_ESM_GetCurEsmSndNwMsgAddr());
    }
    else
    {
        /*sunbing 49683 2013-10-14 VoLTE begin*/
        if(pRcvMsg->stEsmNwMsgIE.stNotificationInd.ucValue == NAS_ESM_NOTIFICATION_INDICATOR_SRVCC_HO_CANCEL)
        {
            NAS_ESM_SndApsEsmNotificationIndMsg(ESM_APS_NOTIFICATION_IND_SRVCC_HO_CANCELLED);
        }
        else
        {
            NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgEsmNotification: Err para!");
        }
        /*sunbing 49683 2013-10-14 VoLTE end*/
        /* TODO: Notify ESM user */
    }
}
/*lint +e961*/
/*lint +e960*/
/*****************************************************************************
 Function Name  : NAS_ESM_NwMsgProcess
 Description    : ESMģ��Գ�������������տ���Ϣ���д���
 Input          : EMM_ESM_INTRA_DATA_IND_STRU *pRcvMsg
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2009-02-05  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_NwMsgProcess
(
    const  EMM_ESM_INTRA_DATA_IND_STRU      *pRcvMsg
)
{
    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG("NAS_ESM_NwMsgProcess is entered.");

    switch(pRcvMsg->stEsmNwMsgIE.enEsmCnMsgType)
    {
       case NAS_ESMCN_MSG_TYPE_ACT_DEFLT_EPS_BEARER_CNTXT_REQ:
           NAS_ESM_ProcNwMsgActDefltEpsbReq(pRcvMsg);
           break;
       case NAS_ESMCN_MSG_TYPE_ACT_DEDICTD_EPS_BEARER_CNTXT_REQ:
           NAS_ESM_ProcNwMsgActDedictdEpsbReq(pRcvMsg);
           break;
       case NAS_ESMCN_MSG_TYPE_MOD_EPS_BEARER_CNTXT_REQ:
           NAS_ESM_ProcNwMsgModEpsbReq(pRcvMsg);
           break;
       case NAS_ESMCN_MSG_TYPE_DEACT_EPS_BEARER_CNTXT_REQ:
           NAS_ESM_ProcNwMsgDeactEpsbReq(pRcvMsg);
           break;
       case NAS_ESMCN_MSG_TYPE_PDN_CONNECT_REJ:
           NAS_ESM_ProcNwMsgPdnConnRej(pRcvMsg);
           break;
       case NAS_ESMCN_MSG_TYPE_PDN_DISCONNECT_REJ:
           NAS_ESM_ProcNwMsgPdnDisconRej(pRcvMsg);
           break;
       case NAS_ESMCN_MSG_TYPE_BEARER_RES_MOD_REJ:
           NAS_ESM_ProcNwMsgResModRej(pRcvMsg);
           break;
       case NAS_ESMCN_MSG_TYPE_BEARER_RES_ALLOC_REJ:
           NAS_ESM_ProcNwMsgResAllocRej(pRcvMsg);
           break;
       case NAS_ESMCN_MSG_TYPE_ESM_INFORMATION_REQ:
           NAS_ESM_ProcNwMsgEsmInfoReq(pRcvMsg);
           break;
       case NAS_ESMCN_MSG_TYPE_ESM_STATUS:
           NAS_ESM_ProcNwsMsgEsmStatus(pRcvMsg);
           break;
       case NAS_ESMCN_MSG_TYPE_ESM_NOTIFICATION:
           NAS_ESM_ProcNwMsgEsmNotification(pRcvMsg);
           break;
       default:
           NAS_ESM_WARN_LOG("NAS_ESM_NwMsgDistr:WARNNING: Illegal Msg Type!");
           break;
    }
}
VOS_VOID NAS_ESM_SndEsmAppSdfProcCnfSucc
(
    VOS_UINT32                          ulStateTblIndex,
    APP_ESM_BEARER_MODIFY_ENUM_UINT32   enModfyType
)
{
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr = VOS_NULL_PTR;

    pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

    /* �ظ�App �޸ĳɹ� *//* ����û���޸������޷���֪�޸ĵ����ݣ�ͳһ�ظ�TFT&QOS
    ����stAppMsgRecord�е�enAppMsgType�ж� */
    if (ID_APP_ESM_PDP_MODIFY_REQ == pstStateAddr->stAppMsgRecord.enAppMsgType)
    {
        NAS_ESM_SndEsmAppSdfModCnfSuccMsg(ulStateTblIndex, enModfyType);
    }
    else if (ID_APP_ESM_PDP_SETUP_REQ == pstStateAddr->stAppMsgRecord.enAppMsgType)
    {
        NAS_ESM_SndEsmAppSdfSetupCnfSuccMsg(ulStateTblIndex);
    }
    else if (ID_APP_ESM_PDP_RELEASE_REQ == pstStateAddr->stAppMsgRecord.enAppMsgType)
    {
        NAS_ESM_SndEsmAppSdfRelCnfSuccMsg(ulStateTblIndex);
    }
    else
    {
        NAS_ESM_NORM_LOG("NAS_ESM_SndEsmAppSdfProcCnfSucc: Modfy Cnf Type IS Error!");
    }

    return;
}


VOS_VOID NAS_ESM_ProcNwMsgActDediBearSamePtiWithDiffEBI
(
    const EMM_ESM_INTRA_DATA_IND_STRU        *pRcvMsg,
    VOS_UINT32                                ulStateTblIndex,
    NAS_ESM_STATE_INFO_STRU                  *pstStateAddr

)
{
    VOS_UINT32                                ulCId = NAS_ESM_NULL;

    if (NAS_ESM_UNASSIGNED_EPSB_ID == pstStateAddr->ulEpsbId)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgActDediBearSamePtiWithDiffEBI:WARNING:Local Eps Id is Unassigned!");

        pstStateAddr->ulEpsbId = pRcvMsg->stEsmNwMsgIE.ucEpsbId;

        return;
    }

    /* ��ѯ��ͬ��PTI��Ӧ��EBI�Ƿ���ͬ */
    if (pstStateAddr->ulEpsbId != pRcvMsg->stEsmNwMsgIE.ucEpsbId)
    {
        /* �ظ�App �޸ĳɹ� *//* ����û���޸������޷���֪�޸ĵ����ݣ�ͳһ�ظ�TFT&QOS */
        NAS_ESM_SndEsmAppSdfProcCnfSucc(ulStateTblIndex, APP_ESM_BEARER_MODIFY_TFT_QOS);

        /* �ͷ�״̬�� */
        NAS_ESM_RelStateTblResource(ulStateTblIndex);

        /* ����1��CID */
        ulCId = NAS_ESM_GetCid(pRcvMsg->stEsmNwMsgIE.ucEpsbId);

        if (NAS_ESM_SUCCESS == NAS_ESM_GetStateTblIndexByCid(ulCId,&ulStateTblIndex))
        {
            NAS_ESM_INFO_LOG("NAS_ESM_ProcNwMsgActDediBearSamePtiWithDiffEBI:Get Cid Succ");

            pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

            /*��д״̬����Ϣ����������״̬������Id����������*/
            pstStateAddr->ulEpsbId = pRcvMsg->stEsmNwMsgIE.ucEpsbId;
            pstStateAddr->enBearerCntxtState = NAS_ESM_GetBearCntxtState(pstStateAddr->ulEpsbId);
            pstStateAddr->enBearerCntxtType  = NAS_ESM_GetBearCntxtType(pstStateAddr->ulEpsbId);
            pstStateAddr->ulCid = ulCId;
        }
        else
        {
            NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgActDediBearSamePtiWithDeffEBI: state table not enough!");
        }
    }

    return;
}
VOS_VOID NAS_ESM_ProcNwMsgModDediBearSamePtiWithDiffEBI
(
    const EMM_ESM_INTRA_DATA_IND_STRU        *pRcvMsg,
    VOS_UINT32                                ulStateTblIndex,
    NAS_ESM_STATE_INFO_STRU                  *pstStateAddr

)
{
    VOS_UINT32                                ulCId  = NAS_ESM_NULL;
    VOS_UINT32                                ulRslt = NAS_ESM_NULL;

    if (NAS_ESM_UNASSIGNED_EPSB_ID == pstStateAddr->ulEpsbId)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgModDediBearSamePtiWithDiffEBI:WARNING:Local Eps Id is Unassigned!");

        pstStateAddr->ulEpsbId = pRcvMsg->stEsmNwMsgIE.ucEpsbId;

        return;
    }

    /* ��ѯ��ͬ��PTI��Ӧ��EBI�Ƿ���ͬ */
    if (pstStateAddr->ulEpsbId != pRcvMsg->stEsmNwMsgIE.ucEpsbId)
    {
        /* �ظ�App �޸ĳɹ� *//* ����û���޸������޷���֪�޸ĵ����ݣ�ͳһ�ظ�TFT&QOS */
        NAS_ESM_SndEsmAppSdfProcCnfSucc(ulStateTblIndex, APP_ESM_BEARER_MODIFY_TFT_QOS);

        /* �ͷ�״̬�� */
        NAS_ESM_RelStateTblResource(ulStateTblIndex);

        /* ����CID */
        ulRslt = NAS_ESM_QuerySdfId(&(pRcvMsg->stEsmNwMsgIE),&ulCId);

        if ((NAS_ESM_QUERY_CID_RSLT_ALL_FOUND_EQUAL_CID != ulRslt)
         && (NAS_ESM_QUERY_CID_RSLT_PART_FOUND_EQUAL_CID != ulRslt))
        {
            NAS_ESM_INFO_LOG("NAS_ESM_ProcNwMsgModDediBearSamePtiWithDiffEBI:Not Found!");

            /* �Ƿ������ظ�?�����Ƿ��м��? */
            ulCId = NAS_ESM_GetCid(pRcvMsg->stEsmNwMsgIE.ucEpsbId);
        }

        if (NAS_ESM_SUCCESS == NAS_ESM_GetStateTblIndexByCid(ulCId,&ulStateTblIndex))
        {
            NAS_ESM_INFO_LOG("NAS_ESM_ProcNwMsgModDediBearSamePtiWithDiffEBI:Get Cid Succ!");

            pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

            /*��д״̬����Ϣ����������״̬������Id����������*/
            pstStateAddr->ulEpsbId = pRcvMsg->stEsmNwMsgIE.ucEpsbId;
            pstStateAddr->enBearerCntxtState = NAS_ESM_GetBearCntxtState(pstStateAddr->ulEpsbId);
            pstStateAddr->enBearerCntxtType  = NAS_ESM_GetBearCntxtType(pstStateAddr->ulEpsbId);
            pstStateAddr->ulCid = ulCId;
        }
        else
        {
            NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgModDediBearSamePtiWithDeffEBI:WARNING:insufficient resource!");
        }
    }

    return;
}


VOS_VOID NAS_ESM_ProcNwMsgDeaDediBearSamePtiWithDiffEBI
(
    const EMM_ESM_INTRA_DATA_IND_STRU        *pRcvMsg,
    VOS_UINT32                                ulStateTblIndex,
    NAS_ESM_STATE_INFO_STRU                  *pstStateAddr

)
{
    VOS_UINT32                                ulCId  = NAS_ESM_NULL;

    if (NAS_ESM_UNASSIGNED_EPSB_ID == pstStateAddr->ulEpsbId)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgDeaDediBearSamePtiWithDiffEBI:WARNING:Local Eps Id is Unassigned!");

        pstStateAddr->ulEpsbId = pRcvMsg->stEsmNwMsgIE.ucEpsbId;

        return;
    }

    /* ��ѯ��ͬ��PTI��Ӧ��EBI�Ƿ���ͬ */
    if (pstStateAddr->ulEpsbId != pRcvMsg->stEsmNwMsgIE.ucEpsbId)
    {
        /* �ظ�App �޸ĳɹ� *//* ����û���޸������޷���֪�޸ĵ����ݣ�ͳһ�ظ�TFT&QOS */
        NAS_ESM_SndEsmAppSdfProcCnfSucc(ulStateTblIndex, APP_ESM_BEARER_MODIFY_TFT_QOS);

        /* �ͷ�״̬�� */
        NAS_ESM_RelStateTblResource(ulStateTblIndex);

        (VOS_VOID)NAS_ESM_QuerySdfId(&(pRcvMsg->stEsmNwMsgIE),&ulCId);
        if (NAS_ESM_SUCCESS == NAS_ESM_GetStateTblIndexByCid(ulCId,&ulStateTblIndex))
        {
            NAS_ESM_INFO_LOG("NAS_ESM_ProcNwMsgDeaDediBearSamePtiWithDiffEBI:Get Cid Succ!");

            pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

            /*��д״̬����Ϣ����������״̬������Id����������*/
            pstStateAddr->ulEpsbId = pRcvMsg->stEsmNwMsgIE.ucEpsbId;
            pstStateAddr->enBearerCntxtState = NAS_ESM_GetBearCntxtState(pstStateAddr->ulEpsbId);
            pstStateAddr->enBearerCntxtType  = NAS_ESM_GetBearCntxtType(pstStateAddr->ulEpsbId);
            pstStateAddr->ulCid = ulCId;
        }
        else
        {
            NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgDeaDediBearSamePtiWithDeffEBI:state table not enough!");
        }
    }

    return;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

