


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include    "NasEsmNwMsgEncode.h"
#include    "NasEsmNwMsgProc.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASESMNWMSGENCODE_C
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
VOS_UINT32                              g_ulPcscfFlag = NAS_ESM_FUN_CTRL_OFF;
VOS_UINT32                              g_ulBcmFlag = NAS_ESM_FUN_CTRL_OFF;
NAS_ESM_GET_IPV6_DNS_CTRL_ENUM_UINT32   g_enIpv6DnsFlag = NAS_ESM_GET_IPV6_DNS_CTRL_ON;


/*****************************************************************************
  3 Function
*****************************************************************************/

/*lint -e960*/
/*lint -e961*/
VOS_UINT32  NAS_ESM_IsExistSamePrecedenceBeforeEncodeMsg
(
    const NAS_ESM_SDF_PARA_STRU              *pstSdfPara,
    VOS_UINT8                                 ucPfNum,
    const NAS_ESM_PF_PRCDNC_VLDT_STRU        *pstPfPVList
)
{
    VOS_UINT8                           ucCnt1      = NAS_ESM_NULL;
    VOS_UINT8                           ucCnt2      = NAS_ESM_NULL;

    for (ucCnt1 = NAS_ESM_NULL; ucCnt1 < pstSdfPara->ulPfNum; ucCnt1++)
    {
        for (ucCnt2 = NAS_ESM_NULL; ucCnt2 < ucPfNum; ucCnt2++)
        {
            if (pstSdfPara->astCntxtTftInfo[ucCnt1].ucPrecedence !=
                                            pstPfPVList[ucCnt2].ucPrecedence)
            {
                continue;
            }

            if (pstSdfPara->ulCId == pstPfPVList[ucCnt2].ucCid)
            {
                continue;
            }
            NAS_ESM_INFO_LOG2("Precedence Collision in these CID:", pstSdfPara->ulCId, pstPfPVList[ucCnt2].ucCid);
            return NAS_ESM_SUCCESS;
        }
    }

    return NAS_ESM_FAILURE;
}

VOS_UINT32  NAS_ESM_ValidatePfPrecedenceBeforeEncodeMsg
(
    VOS_UINT32                         ulLinkEpsbId,
    const NAS_ESM_SDF_PARA_STRU       *pstSdfPara
)
{
    VOS_UINT8                           ucPfNum     = NAS_ESM_NULL;
    VOS_UINT32                          ulRslt      = NAS_ESM_FAILURE;
    NAS_ESM_PF_PRCDNC_VLDT_STRU        *pstPfPVList = VOS_NULL_PTR;

    /* �������ȼ���֤�б�洢�ռ� */
    pstPfPVList = NAS_ESM_MEM_ALLOC(NAS_ESM_MAX_EPSB_NUM * NAS_ESM_MAX_SDF_PF_NUM
                                    * sizeof(NAS_ESM_PF_PRCDNC_VLDT_STRU));

    if (VOS_NULL_PTR == pstPfPVList)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ValidatePfPrecedenceBeforeEncodeMsg:NAS_ESM_MEM_ALLOC failed!");

        return NAS_ESM_FAILURE;
    }

    /* �γ����ȼ���֤�б� */
    ulRslt = NAS_ESM_CreatePfPrecedenceValidateListInPdn(ulLinkEpsbId,
                                                         &ucPfNum,
                                                         pstPfPVList);

    if (ulRslt != NAS_ESM_SUCCESS)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ValidatePfPrecedenceBeforeEncodeMsg:NAS_ESM_CreatePfPrecedenceValidateListInPdn failed!");

        /* �ͷ����ȼ���֤�б�洢�ռ� */
        NAS_ESM_MEM_FREE(pstPfPVList);

        return NAS_ESM_FAILURE;
    }

    ulRslt = NAS_ESM_IsExistSamePrecedenceBeforeEncodeMsg(pstSdfPara,
                                                          ucPfNum,
                                                          pstPfPVList);

    /* �ͷ����ȼ���֤�б�洢�ռ� */
    NAS_ESM_MEM_FREE(pstPfPVList);

    if (ulRslt == NAS_ESM_SUCCESS)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ValidatePfPrecedenceBeforeEncodeMsg:precedence not unique!");
        return NAS_ESM_FAILURE;
    }

    return NAS_ESM_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_EncodeBearerResAllocReqMsg
 Description     : SMģ����װBearer resource allocation request��Ϣ����
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2009-7-20  Draft Enact
    2.lihong00150010      2010-3-11  Modify

*****************************************************************************/
VOS_UINT32  NAS_ESM_EncodeBearerResAllocReqMsg
(
    NAS_ESM_ENCODE_INFO_STRU            stEncodeInfo,
    VOS_UINT8                          *pSendMsg,
    VOS_UINT32                         *pulLength
)
{
    VOS_UINT32                          ulIndex             = NAS_ESM_NULL;
    VOS_UINT32                          ulSum               = NAS_ESM_NULL;
    VOS_UINT32                          ulRslt              = NAS_ESM_FAILURE;
    VOS_UINT8                          *pTmpSendMsg         = pSendMsg;
    NAS_ESM_SDF_PARA_STRU              *pstSdfPara          = VOS_NULL_PTR;
    NAS_ESM_CONTEXT_LTE_QOS_STRU        stEpsQosInfo        = { NAS_ESM_NULL };

    NAS_ESM_INFO_LOG("NAS_ESM_EncodeBearerResAllocReqMsg:INFO:NAS_ESM_EncodeBearerResAllocReqMsg been entered!");

    pstSdfPara = NAS_ESM_GetSdfParaAddr(stEncodeInfo.ulCid);

    /*�жϱ�ѡ������APP���Ƿ���ڣ��������򷵻ش���*/
    if((NAS_ESM_NULL == pstSdfPara->ulPfNum) ||
        (NAS_ESM_OP_FALSE == pstSdfPara->bitOpSdfQos))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeBearerResAllocReqMsg:WARNING:NAS->ESM The must para is not selected!");

        return NAS_ESM_FAILURE;
    }

    /* ��鷢�����Ϣ���ȼ��Ƿ�Ψһ */
    ulRslt = NAS_ESM_ValidatePfPrecedenceBeforeEncodeMsg(stEncodeInfo.ulLinkedEpsbId,
                                                         pstSdfPara);
    if (NAS_ESM_FAILURE == ulRslt)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeBearerResAllocReqMsg: precedence is not unique!");

        return NAS_ESM_FAILURE;
    }

    /*���Protocol discriminator��EpsbId����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = ((VOS_UINT8)(NAS_ESM_UNASSIGNED_EPSB_ID << NAS_ESM_MOVEMENT_4_BITS)\
            & NAS_ESM_MSG_HIGH_BIT) | NAS_ESM_MSG_PD;

    /*���PTI����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = stEncodeInfo.ucPti;

    /*���Message type: NAS_ESMCN_MSG_TYPE_BEARER_RES_ALLOC_REQ����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESMCN_MSG_TYPE_BEARER_RES_ALLOC_REQ;

    /* ��Ź���ȱʡ���غ� */
    pTmpSendMsg[ulSum++] = stEncodeInfo.ulLinkedEpsbId & NAS_ESM_MSG_LOW_BIT;

    /* ����TFT��Ϣ */
    if(NAS_ESM_FAILURE == NAS_ESM_EncodeTftCreateTftOrReplacePf(NAS_ESM_TFT_CREATE_TFT,
                                                                stEncodeInfo.ulCid,
                                                                pTmpSendMsg+ulSum,
                                                                &ulIndex))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeBearerResAllocReqMsg:WARNING:NAS->ESM Message Tft encode failure!");
        return NAS_ESM_FAILURE;
    }

    ulSum += ulIndex;

    /*ulIndex����*/
    ulIndex = NAS_ESM_NULL;

    /*SDF QoS*/
    NAS_ESM_MEM_CPY(&stEpsQosInfo.stQosInfo,
                    &pstSdfPara->stSdfQosInfo,
                    sizeof(APP_ESM_EPS_QOS_INFO_STRU));
    stEpsQosInfo.ucNwQCI = pstSdfPara->stSdfQosInfo.ucQCI;

    if(NAS_ESM_FAILURE == NAS_ESM_EncodeEpsQos(&stEpsQosInfo,
                                               pTmpSendMsg+ulSum,
                                               &ulIndex,
                                               NAS_MSG_FORMAT_LV))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeBearerResAllocReqMsg:WARNING:NAS->ESM Message SDF QOS encode failure!");
        return NAS_ESM_FAILURE;
    }

    ulSum += ulIndex;

    /*ulIndex����*/
    ulIndex = NAS_ESM_NULL;

    #if 0  /*Ŀǰ����ֻ��ȱʡ���ؼ������ҪЯ��PCO��Ϣ����������*/
    /*���ݳ��ز�����Ÿ��ֶΣ���¼��Ϣ����*/
    if(NAS_ESM_OP_TRUE == pstSdfPara->bitOpPco)
    {
        NAS_ESM_INFO_LOG("NAS_ESM_EncodeBearerResAllocReqMsg:INFO:NAS->SM Message contain PCO!");
        if(NAS_ESM_FAILURE == NAS_ESM_EncodePco(&pstSdfPara->stPcoInfo,
                                               pTmpSendMsg+ulSum,
                                               &ulIndex,
                                               NAS_MSG_FORMAT_TLV))
        {
             /*��ӡ�쳣��Ϣ*/
            NAS_ESM_WARN_LOG("NAS_ESM_EncodeBearerResAllocReqMsg:WARNING:NAS->SM Message PCO encode failure!");
            return NAS_ESM_FAILURE;
        }
    }
    else
    {
        NAS_ESM_INFO_LOG("NAS_ESM_EncodeBearerResAllocReqMsg:INFO:NAS->SM Message not contain PCO!");
    }
    #endif

    *pulLength = ulSum + ulIndex;
    return NAS_ESM_SUCCESS;
}
VOS_UINT32  NAS_ESM_ValidateSdfParaBeforeEncodeMsg
(
    NAS_ESM_RES_MOD_ENCODE_INFO_STRU          stResModEncodeInfo
)
{
    NAS_ESM_EPSB_CNTXT_INFO_STRU       *pstEpsbCntxtInfo    = VOS_NULL_PTR;
    NAS_ESM_SDF_PARA_STRU              *pstSdfPara          = VOS_NULL_PTR;
    VOS_UINT32                          ulRslt              = NAS_ESM_FAILURE;

    pstSdfPara = NAS_ESM_GetSdfParaAddr(stResModEncodeInfo.ulCid);

    /*�жϱ�ѡ������APP���Ƿ���ڣ��������򷵻ش���*/
    if (NAS_ESM_NULL == pstSdfPara->ulPfNum)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ValidateSdfParaBeforeEncodeMsg:NAS->ESM The must para is not selected!");

        return NAS_ESM_FAILURE;
    }

    /* ɾ��SDF���޸�Qos����ҪЯ�����ȼ�,������֤���ȼ� */
    if ((NAS_ESM_SDF_OP_TYPE_DELETE_SDF != stResModEncodeInfo.enSdfOpType)
     && (NAS_ESM_SDF_OP_TYPE_MOD_QOS != stResModEncodeInfo.enSdfOpType))
    {
        pstEpsbCntxtInfo = NAS_ESM_GetEpsbCntxtInfoAddr(stResModEncodeInfo.ulEpsbId);

        /* ��鷢�����Ϣ���ȼ��Ƿ�Ψһ */
        ulRslt = NAS_ESM_ValidatePfPrecedenceBeforeEncodeMsg(pstEpsbCntxtInfo->ulLinkedEpsbId,
                                                             pstSdfPara);
        if (NAS_ESM_FAILURE == ulRslt)
        {
            NAS_ESM_WARN_LOG("NAS_ESM_ValidateSdfParaBeforeEncodeMsg: precedence is not unique!");

            return NAS_ESM_FAILURE;
        }
    }

    return NAS_ESM_SUCCESS;
}
VOS_VOID NAS_ESM_GetBearerAllPacketFilter
(
    VOS_UINT32                           ulEpsbId,
    VOS_UINT32                          *pulPfNum,
    NAS_ESM_CONTEXT_TFT_STRU            *pstTFTInfo
)
{
    NAS_ESM_EPSB_CNTXT_INFO_STRU       *pstEspbCntxtInfo    = VOS_NULL_PTR;
    NAS_ESM_SDF_CNTXT_INFO_STRU        *pstSdfCntxtInfo     = VOS_NULL_PTR;
    VOS_UINT32                          ulCnt1              = NAS_ESM_NULL;

    *pulPfNum = NAS_ESM_NULL;

    pstEspbCntxtInfo = NAS_ESM_GetEpsbCntxtInfoAddr(ulEpsbId);

    /* ��������SDF,��������ĳ�����ulEpsbId*/
    /* �����SDF�µ�����Packetfilter������pstTFTInfo*/
    for(ulCnt1 = NAS_ESM_MIN_CID; ulCnt1 <= NAS_ESM_MAX_CID; ulCnt1++)
    {
        if(NAS_ESM_OP_TRUE !=((pstEspbCntxtInfo->ulBitCId >> ulCnt1 )& NAS_ESM_BIT_0))
        {
            continue;
        }

        pstSdfCntxtInfo = NAS_ESM_GetSdfCntxtInfo(ulCnt1);

        if (((*pulPfNum) + pstSdfCntxtInfo->ulSdfPfNum) <= NAS_ESM_MAX_SDF_PF_NUM)
        {
            NAS_ESM_MEM_CPY(&pstTFTInfo[*pulPfNum],
                            pstSdfCntxtInfo->astSdfPfInfo,
                            (pstSdfCntxtInfo->ulSdfPfNum * sizeof(NAS_ESM_CONTEXT_TFT_STRU)));


            (*pulPfNum) += pstSdfCntxtInfo->ulSdfPfNum;
        }
        else
        {
            NAS_ESM_WARN_LOG("NAS_ESM_GetBearerAllPacketFilter:there are more than 16 packeter in one bearer !");
        }
    }
}


VOS_UINT32  NAS_ESM_EncodeBearerResModReqMsgRelBearer
(
    NAS_ESM_RES_MOD_ENCODE_INFO_STRU    stResModEncodeInfo,
    VOS_UINT8                          *pSendMsg,
    VOS_UINT32                         *pulLength
)
{
    VOS_UINT32                          ulPfArrayLen        = NAS_ESM_NULL;
    VOS_UINT8                          *pTmpSendMsg         = pSendMsg;
    VOS_UINT32                          ulIndex             = NAS_ESM_NULL;
    VOS_UINT32                          ulSum               = NAS_ESM_NULL;
    VOS_UINT32                          ulPfNum             = NAS_ESM_NULL;
    NAS_ESM_CONTEXT_TFT_STRU           *pstSdfPfArray       = VOS_NULL_PTR;

    NAS_ESM_INFO_LOG("NAS_ESM_EncodeBearerResModReqMsgRelBearer is entered.");

    pTmpSendMsg[ulSum++] = ((VOS_UINT8)(NAS_ESM_UNASSIGNED_EPSB_ID << NAS_ESM_MOVEMENT_4_BITS)\
                            & NAS_ESM_MSG_HIGH_BIT) | NAS_ESM_MSG_PD;

    /*���PTI����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = stResModEncodeInfo.ucPti;

    /*���Message type: NAS_ESMCN_BEARER_RES_MODIFICATION_REQ����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESMCN_MSG_TYPE_BEARER_RES_MOD_REQ;

    pTmpSendMsg[ulSum++] = stResModEncodeInfo.ulLinkedEpsbId & NAS_ESM_MSG_LOW_BIT;

    ulPfArrayLen = sizeof(NAS_ESM_CONTEXT_TFT_STRU) * NAS_ESM_MAX_SDF_PF_NUM;

    /*����ռ䲢��������Ƿ�ɹ�*/
     pstSdfPfArray = (VOS_VOID*)NAS_ESM_MEM_ALLOC(ulPfArrayLen);

     /*����Ƿ����ɹ�*/
     if (VOS_NULL_PTR == pstSdfPfArray)
     {
         /*��ӡ�쳣��Ϣ*/
         NAS_ESM_ERR_LOG("NAS_ESM_EncodeBearerResModReqMsgRelBearer:Memory Alloc FAIL!");
         return NAS_ESM_FAILURE;
     }

    NAS_ESM_MEM_SET(pstSdfPfArray, NAS_ESM_NULL, ulPfArrayLen);


    NAS_ESM_GetBearerAllPacketFilter(stResModEncodeInfo.ulEpsbId,
                                     &ulPfNum,
                                      pstSdfPfArray);

    NAS_ESM_EncodeTftDeletePf(pstSdfPfArray,
                              ulPfNum,
                              pTmpSendMsg + ulSum,
                             &ulIndex);

    ulSum = ulSum + ulIndex;

    /*���ESM cause��IEI*/
    pTmpSendMsg[ulSum++] = NAS_ESM_MSG_ESM_CAU_IEI;
    pTmpSendMsg[ulSum++] = stResModEncodeInfo.ucESMCau;

    *pulLength = ulSum;

    /* �ͷ��ڴ�ռ� */
    NAS_ESM_MEM_FREE(pstSdfPfArray);

    return NAS_ESM_SUCCESS;
}
VOS_UINT32 NAS_ESM_EncodeBearerResModReqMsg
(
    NAS_ESM_RES_MOD_ENCODE_INFO_STRU    stResModEncodeInfo,
    VOS_UINT8                          *pSendMsg,
    VOS_UINT32                         *pulLength
)
{
    VOS_UINT32                          ulIndex             = NAS_ESM_NULL;
    VOS_UINT32                          ulSum               = NAS_ESM_NULL;
    VOS_UINT32                          ulRet               = NAS_ESM_NULL;
    VOS_UINT8                          *pTmpSendMsg         = pSendMsg;
    VOS_UINT32                          ulRslt              = NAS_ESM_FAILURE;

    NAS_ESM_INFO_LOG("NAS_ESM_EncodeBearerResModReqMsg:INFO:NAS_ESM_EncodeBearerResModReqMsg been entered!");

    ulRslt = NAS_ESM_ValidateSdfParaBeforeEncodeMsg(stResModEncodeInfo);
    if (NAS_ESM_FAILURE == ulRslt)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ValidateSdfParaBeforeEncodeMsg: precedence is not unique!");

        return NAS_ESM_FAILURE;
    }


    pTmpSendMsg[ulSum++] = ((VOS_UINT8)(NAS_ESM_UNASSIGNED_EPSB_ID << NAS_ESM_MOVEMENT_4_BITS)\
        & NAS_ESM_MSG_HIGH_BIT) | NAS_ESM_MSG_PD;

    /*���PTI����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = stResModEncodeInfo.ucPti;

    /*���Message type: NAS_ESMCN_BEARER_RES_MODIFICATION_REQ����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESMCN_MSG_TYPE_BEARER_RES_MOD_REQ;

    pTmpSendMsg[ulSum++] = stResModEncodeInfo.ulLinkedEpsbId & NAS_ESM_MSG_LOW_BIT;

    switch(stResModEncodeInfo.enSdfOpType)
    {
        case NAS_ESM_SDF_OP_TYPE_ADD_SDF:
            ulRet = NAS_ESM_EncodeBearerResModReqMsgAddSdf(stResModEncodeInfo,
                                                pTmpSendMsg + ulSum,
                                                &ulIndex);
            break;
        case NAS_ESM_SDF_OP_TYPE_DELETE_SDF:
            ulRet = NAS_ESM_EncodeBearerResModReqMsgDeleteSdf(stResModEncodeInfo,
                                                pTmpSendMsg + ulSum,
                                                &ulIndex);
            break;
        case NAS_ESM_SDF_OP_TYPE_MOD_QOS:
            ulRet = NAS_ESM_EncodeBearerResModReqMsgModQos(stResModEncodeInfo,
                                                pTmpSendMsg + ulSum,
                                                &ulIndex);
            break;


        case NAS_ESM_SDF_OP_TYPE_MOD_PF:
            ulRet = NAS_ESM_EncodeBearerResModReqMsgModPf(  stResModEncodeInfo,
                                                            pTmpSendMsg + ulSum,
                                                            &ulIndex);
            break;

        case NAS_ESM_SDF_OP_TYPE_MOD_QOS_AND_PF:
            ulRet = NAS_ESM_EncodeBearerResModReqMsgModQosAndPf(stResModEncodeInfo,
                                                pTmpSendMsg + ulSum,
                                                &ulIndex);
            break;

        default:
            NAS_ESM_WARN_LOG("NAS_ESM_EncodeBearerResModReqMsg:WARNING:No Support Operation!");
            return NAS_ESM_FAILURE;
    }

    if (NAS_ESM_FAILURE == ulRet)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeBearerResModReqMsg:WARNING:Encode Tad failed!");
        return NAS_ESM_FAILURE;
    }

    ulSum += ulIndex;

    /*ulIndex����*/
    ulIndex = NAS_ESM_NULL;

    if( NAS_ESM_OP_TRUE == stResModEncodeInfo.bitOpESMCau)
    {
        /*���ESM cause��IEI*/
        pTmpSendMsg[ulSum++] = NAS_ESM_MSG_ESM_CAU_IEI;
        pTmpSendMsg[ulSum++] = stResModEncodeInfo.ucESMCau;
    }

    #if 0  /*Ŀǰ����ֻ��ȱʡ���ؼ������ҪЯ��PCO��Ϣ����������*/
    /*���ݳ��ز�����Ÿ��ֶΣ���¼��Ϣ����*/
    if(NAS_ESM_OP_TRUE == pstSdfPara->bitOpPco)
    {
        NAS_ESM_INFO_LOG("NAS_ESM_EncodeBearerResModReqMsg:INFO:NAS->SM Message contain PCO!");
        if(NAS_ESM_FAILURE == NAS_ESM_EncodePco(&pstSdfPara->stPcoInfo,
                                                pTmpSendMsg+ulSum,
                                                &ulIndex,
                                               NAS_MSG_FORMAT_TLV))
        {
             /*��ӡ�쳣��Ϣ*/
            NAS_ESM_WARN_LOG("NAS_ESM_EncodeBearerResModReqMsg:WARNING:NAS->SM Message PCO encode failure!");
            return NAS_ESM_FAILURE;
        }
    }
    else
    {
        NAS_ESM_INFO_LOG("NAS_ESM_EncodeBearerResModReqMsg:INFO:NAS->SM Message not contain PCO!");
    }
    #endif

    *pulLength = ulSum + ulIndex;
    return NAS_ESM_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_EncodeBearerResModReqMsgAddSdf
 Description     : ���������Դ�޸��е����SDF
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.sunbing49683      2009-4-17  Draft Enact
    2.lihong00150010    2010-3-11  Modify

*****************************************************************************/
VOS_UINT32 NAS_ESM_EncodeBearerResModReqMsgAddSdf
(
    NAS_ESM_RES_MOD_ENCODE_INFO_STRU    stResModEncodeInfo,
    VOS_UINT8                           *pSendMsg,
    VOS_UINT32                          *pulLength
)
{
    VOS_UINT32                          ulIndex             = NAS_ESM_NULL;
    VOS_UINT32                          ulSum               = *pulLength;
    VOS_UINT8                          *pTmpSendMsg         = pSendMsg;
    NAS_ESM_SDF_PARA_STRU              *pstSdfPara          = VOS_NULL_PTR;
    NAS_ESM_CONTEXT_LTE_QOS_STRU        stEpsQosInfo        = { NAS_ESM_NULL };

    pstSdfPara = NAS_ESM_GetSdfParaAddr(stResModEncodeInfo.ulCid);

    /* ����TFT��Ϣ */
    if(NAS_ESM_FAILURE == NAS_ESM_EncodeTftCreateTftOrReplacePf(NAS_ESM_TFT_CREATE_TFT,
                                                                stResModEncodeInfo.ulCid,
                                                                pTmpSendMsg+ulSum,
                                                                &ulIndex))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeBearerResModReqMsgAddSdf:WARNING:NAS->ESM Message Tft encode failure!");
        return NAS_ESM_FAILURE;
    }

    ulSum += ulIndex;

    /*ulIndex����*/
    ulIndex = NAS_ESM_NULL;

    /*SDF QoS*/
    if(NAS_ESM_OP_TRUE == pstSdfPara->bitOpSdfQos)
    {
        NAS_ESM_INFO_LOG("NAS_ESM_EncodeBearerResModReqMsgAddSdf:INFO:NAS->ESM Message contain SDF QOS!");


        NAS_ESM_MEM_CPY(&stEpsQosInfo.stQosInfo,
                        &pstSdfPara->stSdfQosInfo,
                        sizeof(APP_ESM_EPS_QOS_INFO_STRU));
        stEpsQosInfo.ucNwQCI = pstSdfPara->stSdfQosInfo.ucQCI;
        if(NAS_ESM_FAILURE == NAS_ESM_EncodeEpsQos(&stEpsQosInfo,
                                               pTmpSendMsg+ulSum,
                                               &ulIndex,
                                               NAS_MSG_FORMAT_TLV))
        {
            NAS_ESM_WARN_LOG("NAS_ESM_EncodeBearerResModReqMsgAddSdf:WARNING:NAS->ESM Message SDF QOS encode failure!");
            return NAS_ESM_FAILURE;
        }

        ulSum = ulSum + ulIndex;
        ulIndex = NAS_ESM_NULL;
    }
    else
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeBearerResModReqMsgAddSdf:WARNING:NAS->ESM The must para is not selected!");

        return NAS_ESM_FAILURE;

    }

    *pulLength = ulSum;
    return NAS_ESM_SUCCESS;
}
VOS_VOID NAS_ESM_ModEpsQos
(
    const APP_ESM_EPS_QOS_INFO_STRU                      *pstEpsQoSInfoOld,
    const APP_ESM_EPS_QOS_INFO_STRU                      *pstEpsQoSInfoNew,
    APP_ESM_EPS_QOS_INFO_STRU                            *pstEpsQoSInfo
)
{
    /* �޸� ULMaxRate */
    if (pstEpsQoSInfoOld->ulULMaxRate < pstEpsQoSInfoNew->ulULMaxRate)
    {
        pstEpsQoSInfo->ulULMaxRate += (pstEpsQoSInfoNew->ulULMaxRate -
                                                pstEpsQoSInfoOld->ulULMaxRate);
    }
    else
    {
        pstEpsQoSInfo->ulULMaxRate -= (pstEpsQoSInfoOld->ulULMaxRate -
                                                pstEpsQoSInfoNew->ulULMaxRate);
    }

    /* �޸� DLMaxRate */
    if (pstEpsQoSInfoOld->ulDLMaxRate < pstEpsQoSInfoNew->ulDLMaxRate)
    {
        pstEpsQoSInfo->ulDLMaxRate += (pstEpsQoSInfoNew->ulDLMaxRate -
                                                pstEpsQoSInfoOld->ulDLMaxRate);
    }
    else
    {
        pstEpsQoSInfo->ulDLMaxRate -= (pstEpsQoSInfoOld->ulDLMaxRate -
                                                pstEpsQoSInfoNew->ulDLMaxRate);
    }

    /* �޸� ULGMaxRate */
    if (pstEpsQoSInfoOld->ulULGMaxRate < pstEpsQoSInfoNew->ulULGMaxRate)
    {
        pstEpsQoSInfo->ulULGMaxRate += (pstEpsQoSInfoNew->ulULGMaxRate -
                                                pstEpsQoSInfoOld->ulULGMaxRate);
    }
    else
    {
        pstEpsQoSInfo->ulULGMaxRate -= (pstEpsQoSInfoOld->ulULGMaxRate -
                                                pstEpsQoSInfoNew->ulULGMaxRate);
    }

    /* �޸� DLGMaxRate */
    if (pstEpsQoSInfoOld->ulDLGMaxRate < pstEpsQoSInfoNew->ulDLGMaxRate)
    {
        pstEpsQoSInfo->ulDLGMaxRate += (pstEpsQoSInfoNew->ulDLGMaxRate -
                                                pstEpsQoSInfoOld->ulDLGMaxRate);
    }
    else
    {
        pstEpsQoSInfo->ulDLGMaxRate -= (pstEpsQoSInfoOld->ulDLGMaxRate -
                                                pstEpsQoSInfoNew->ulDLGMaxRate);
    }
}


VOS_UINT32  NAS_ESM_GetEpsQosAfterMod
(
    NAS_ESM_RES_MOD_ENCODE_INFO_STRU    stResModEncodeInfo,
    NAS_ESM_CONTEXT_LTE_QOS_STRU       *pstEpsQoSInfo
)
{
    NAS_ESM_SDF_CNTXT_INFO_STRU        *pstSdfCntxtInfo  = VOS_NULL_PTR;
    NAS_ESM_EPSB_CNTXT_INFO_STRU       *pstEpsbCntxtInfo = VOS_NULL_PTR;
    NAS_ESM_SDF_PARA_STRU              *pstSdfPara       = VOS_NULL_PTR;

    pstSdfPara = NAS_ESM_GetSdfParaAddr(stResModEncodeInfo.ulCid);

    if (NAS_ESM_OP_TRUE == pstSdfPara->bitOpSdfQos)
    {
        /*����EpsIdȡ��������Ϣ*/
        pstEpsbCntxtInfo = NAS_ESM_GetEpsbCntxtInfoAddr(stResModEncodeInfo.ulEpsbId);

        NAS_ESM_MEM_CPY(pstEpsQoSInfo,
                        &pstEpsbCntxtInfo->stEpsQoSInfo,
                        sizeof(NAS_ESM_CONTEXT_LTE_QOS_STRU));

        if (PS_TRUE == NAS_ESM_IsGbrBearer(pstEpsbCntxtInfo->stEpsQoSInfo.stQosInfo.ucQCI))
        {
            pstSdfCntxtInfo = NAS_ESM_GetSdfCntxtInfo(stResModEncodeInfo.ulCid);

            /* ����QOS�ı仯�����Ӧ��Eps Qos ֵ */
            NAS_ESM_ModEpsQos(&pstSdfCntxtInfo->stSdfQosInfo.stQosInfo,
                              &pstSdfPara->stSdfQosInfo,
                              &pstEpsQoSInfo->stQosInfo);
        }
    }
    else
    {
        NAS_ESM_WARN_LOG("NAS_ESM_GetEpsQosAfterMod:WARNING:NAS->ESM The must para is not selected!");
        return NAS_ESM_FAILURE;
    }

    return NAS_ESM_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_EncodeBearerResModReqMsgDeleteSdf
 Description     : ���������Դ�޸��е�ɾ��SDF
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.sunbing49683      2009-4-17  Draft Enact

*****************************************************************************/
VOS_UINT32 NAS_ESM_EncodeBearerResModReqMsgDeleteSdf
(
    NAS_ESM_RES_MOD_ENCODE_INFO_STRU    stResModEncodeInfo,
    VOS_UINT8                           *pSendMsg,
    VOS_UINT32                          *pulLength
)
{

    VOS_UINT8                           *pTmpSendMsg      = pSendMsg;
    VOS_UINT32                           ulIndex          = NAS_ESM_NULL;
    VOS_UINT32                           ulSum            = NAS_ESM_NULL;
    VOS_UINT32                           ulActCidNum      = NAS_ESM_NULL;
    APP_ESM_EPS_QOS_INFO_STRU           *pstSdfQosInfo    = VOS_NULL_PTR;
    NAS_ESM_CONTEXT_LTE_QOS_STRU         stEpsQosInfo;
    NAS_ESM_EPSB_CNTXT_INFO_STRU        *pstEpsbCntxtInfo = VOS_NULL_PTR;
    NAS_ESM_SDF_CNTXT_INFO_STRU         *pstSdfCntxtInfo  = VOS_NULL_PTR;

    pstSdfCntxtInfo  = NAS_ESM_GetSdfCntxtInfo(stResModEncodeInfo.ulCid);

    NAS_ESM_EncodeTftDeletePf(&pstSdfCntxtInfo->astSdfPfInfo[0],
                               pstSdfCntxtInfo->ulSdfPfNum,
                               pTmpSendMsg,
                              &ulIndex);

    ulSum = ulSum + ulIndex;
    ulIndex = NAS_ESM_NULL;

    if (NAS_ESM_DEL_SDF_WITH_QOS_ON == NAS_ESM_GetDelSdfWithQosFlag())
    {
        pstEpsbCntxtInfo = NAS_ESM_GetEpsbCntxtInfoAddr(stResModEncodeInfo.ulEpsbId);

        ulActCidNum = NAS_ESM_GetBearerLinkedCidNum(stResModEncodeInfo.ulEpsbId);

        /* ���ֻ��Ӧһ�������Cid,��Я��Qos */
        if (ulActCidNum == NAS_ESM_BEARER_ACT_CID_NUM_ONE)
        {
            /*��¼����*/
            *pulLength = ulSum;
            return NAS_ESM_SUCCESS;
        }

        /* �����µ�EPS QOS */
        if (PS_TRUE == NAS_ESM_IsGbrBearer(pstEpsbCntxtInfo->stEpsQoSInfo.stQosInfo.ucQCI))
        {
            pstSdfQosInfo = &pstSdfCntxtInfo->stSdfQosInfo.stQosInfo;

            NAS_ESM_MEM_CPY(&stEpsQosInfo,
                            &pstEpsbCntxtInfo->stEpsQoSInfo,
                            sizeof(NAS_ESM_CONTEXT_LTE_QOS_STRU));

            stEpsQosInfo.stQosInfo.ulULMaxRate -= pstSdfQosInfo->ulULMaxRate;
            stEpsQosInfo.stQosInfo.ulDLMaxRate -= pstSdfQosInfo->ulDLMaxRate;
            stEpsQosInfo.stQosInfo.ulULGMaxRate -= pstSdfQosInfo->ulULGMaxRate;
            stEpsQosInfo.stQosInfo.ulDLGMaxRate -= pstSdfQosInfo->ulDLGMaxRate;

            if(NAS_ESM_FAILURE == NAS_ESM_EncodeEpsQos(&stEpsQosInfo,
                                                       pTmpSendMsg+ulSum,
                                                      &ulIndex,
                                                       NAS_MSG_FORMAT_TLV))
            {
                NAS_ESM_WARN_LOG("NAS_ESM_EncodeBearerResModReqMsgDeleteSdf:NAS->ESM Message SDF QOS encode failure!");
                return NAS_ESM_FAILURE;
            }

            ulSum = ulSum + ulIndex;
            ulIndex = NAS_ESM_NULL;
        }
    }

    /*��¼����*/
    *pulLength = ulSum;

    return NAS_ESM_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_EncodeBearerResModReqMsgModPf
 Description     : ���������Դ�޸��е��޸�PF
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.sunbing49683      2009-4-17  Draft Enact
    2.lihong00150010    2010-3-11  Modify

*****************************************************************************/
VOS_UINT32 NAS_ESM_EncodeBearerResModReqMsgModPf
(
    NAS_ESM_RES_MOD_ENCODE_INFO_STRU    stResModEncodeInfo,
    VOS_UINT8                           *pSendMsg,
    VOS_UINT32                          *pulLength
)
{
    VOS_UINT32                          ulIndex             = NAS_ESM_NULL;
    VOS_UINT32                          ulSum               = *pulLength;
    VOS_UINT8                          *pTmpSendMsg         = pSendMsg;

    /* ����TFT��Ϣ */
    if(NAS_ESM_FAILURE == NAS_ESM_EncodeTftCreateTftOrReplacePf(NAS_ESM_TFT_REPLACE_FILTER,
                                                                stResModEncodeInfo.ulCid,
                                                                pTmpSendMsg+ulSum,
                                                                &ulIndex))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeBearerResModReqMsgAddSdf:WARNING:NAS->ESM Message Tft encode failure!");
        return NAS_ESM_FAILURE;
    }

    ulSum += ulIndex;

    *pulLength = ulSum;

    return NAS_ESM_SUCCESS;
}


/*****************************************************************************
 Function Name   : NAS_ESM_EncodeBearerResModReqMsgModQos
 Description     : ���������Դ�޸��е��޸�Qos
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2009-05-18  Draft Enact

*****************************************************************************/
VOS_UINT32 NAS_ESM_EncodeBearerResModReqMsgModQos
(
    NAS_ESM_RES_MOD_ENCODE_INFO_STRU    stResModEncodeInfo,
    VOS_UINT8                          *pSendMsg,
    VOS_UINT32                         *pulLength
)
{
    VOS_UINT32                          ulIndex             = NAS_ESM_NULL;
    VOS_UINT32                          ulSum               = *pulLength;
    VOS_UINT32                          ulCnt               = NAS_ESM_NULL;
    VOS_UINT8                          *pTmpSendMsg         = pSendMsg;
    VOS_UINT8                           aucNwPacketFilterId[NAS_ESM_MAX_SDF_PF_NUM] = {0};
    NAS_ESM_SDF_PARA_STRU              *pstSdfPara          = VOS_NULL_PTR;
    NAS_ESM_CONTEXT_LTE_QOS_STRU        stEpsQoSInfo        = { NAS_ESM_NULL };

    pstSdfPara = NAS_ESM_GetSdfParaAddr(stResModEncodeInfo.ulCid);

    for ( ulCnt = NAS_ESM_NULL ; ulCnt < pstSdfPara->ulPfNum ; ulCnt++ )
    {
        (VOS_VOID)NAS_ESM_QueryNWPacketFilterID(pstSdfPara->astCntxtTftInfo[ulCnt].ucPacketFilterId,
                                                stResModEncodeInfo.ulCid,
                                               &aucNwPacketFilterId[ulCnt]);
    }

    /* ��ֵ��ʱTFT��Ϣ���� */
    /* �����޸�SDF��QOS��Ϣ��TFT���� */
    NAS_ESM_EncodeTftNoOperation(aucNwPacketFilterId,
                                  pstSdfPara->ulPfNum,
                                  pSendMsg + ulSum,
                                  &ulIndex);


    /*��¼����*/
    ulSum = ulSum + ulIndex;

    /*ulIndex����*/
    ulIndex = NAS_ESM_NULL;

    /*SDF QoS*/
    if (NAS_ESM_SUCCESS == NAS_ESM_GetEpsQosAfterMod(stResModEncodeInfo,&stEpsQoSInfo))
    {
        NAS_ESM_INFO_LOG("NAS_ESM_EncodeBearerResModReqMsgModQos:INFO:NAS->ESM Message contain SDF QOS!");
        if(NAS_ESM_FAILURE == NAS_ESM_EncodeEpsQos(&stEpsQoSInfo,
                                                    pTmpSendMsg+ulSum,
                                                    &ulIndex,
                                                    NAS_MSG_FORMAT_TLV))
        {
            NAS_ESM_WARN_LOG("NAS_ESM_EncodeBearerResModReqMsgModQos:WARNING:NAS->ESM Message SDF QOS encode failure!");
            return NAS_ESM_FAILURE;
        }

        ulSum = ulSum + ulIndex;
        ulIndex = NAS_ESM_NULL;
    }
    else
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeBearerResModReqMsgModQos:WARNING:NAS->ESM get Modify Eps Qos error!");

        return NAS_ESM_FAILURE;

    }

    *pulLength = ulSum;
    return NAS_ESM_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_EncodeBearerResModReqMsgModQosAndPf
 Description     : ���������Դ�޸��е��޸�Qos��PF
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2009-05-18  Draft Enact
    2.lihong00150010      2010-3-11   Modify

*****************************************************************************/
VOS_UINT32 NAS_ESM_EncodeBearerResModReqMsgModQosAndPf
(
    NAS_ESM_RES_MOD_ENCODE_INFO_STRU    stResModEncodeInfo,
    VOS_UINT8                           *pSendMsg,
    VOS_UINT32                          *pulLength
)
{
    VOS_UINT32                          ulIndex             = NAS_ESM_NULL;
    VOS_UINT32                          ulSum               = *pulLength;
    VOS_UINT8                          *pTmpSendMsg         = pSendMsg;
    NAS_ESM_CONTEXT_LTE_QOS_STRU        stEpsQoSInfo        = { NAS_ESM_NULL };

    /* ����TFT��Ϣ */
    if(NAS_ESM_FAILURE == NAS_ESM_EncodeTftCreateTftOrReplacePf(NAS_ESM_TFT_REPLACE_FILTER,
                                                                stResModEncodeInfo.ulCid,
                                                                pTmpSendMsg+ulSum,
                                                                &ulIndex))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeBearerResModReqMsgModQosAndPf:WARNING:NAS->ESM Message Tft encode failure!");
        return NAS_ESM_FAILURE;
    }

    ulSum += ulIndex;

    /*ulIndex����*/
    ulIndex = NAS_ESM_NULL;

    /*SDF QoS*/
    if (NAS_ESM_SUCCESS == NAS_ESM_GetEpsQosAfterMod(stResModEncodeInfo,&stEpsQoSInfo))
    {
        NAS_ESM_INFO_LOG("NAS_ESM_EncodeBearerResModReqMsgModQosAndPf:INFO:NAS->ESM Message contain SDF QOS!");
        if(NAS_ESM_FAILURE == NAS_ESM_EncodeEpsQos(&stEpsQoSInfo,
                                               pTmpSendMsg+ulSum,
                                               &ulIndex,
                                               NAS_MSG_FORMAT_TLV))
        {
            NAS_ESM_WARN_LOG("NAS_ESM_EncodeBearerResModReqMsgModQosAndPf:WARNING:NAS->ESM Message SDF QOS encode failure!");
            return NAS_ESM_FAILURE;
        }

        ulSum = ulSum + ulIndex;
        ulIndex = NAS_ESM_NULL;
    }
    else
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeBearerResModReqMsgModQosAndPf:WARNING:NAS->ESM get Modify Eps Qos error!");

        return NAS_ESM_FAILURE;

    }

    *pulLength = ulSum;
    return NAS_ESM_SUCCESS;
}
VOS_VOID NAS_ESM_EncodeActDedictdEpsbCntxtAccMsg( NAS_ESM_ENCODE_INFO_STRU stEncodeInfo,
                                                     VOS_UINT8  *pSendMsg,
                                                     VOS_UINT32 *pulLength)
{
    VOS_UINT8        *pTmpSendMsg  = pSendMsg;
    VOS_UINT32        ulSum        = NAS_ESM_NULL;
    VOS_UINT32        ulEpsbId     = stEncodeInfo.ulEpsbId;

    NAS_ESM_INFO_LOG("NAS_ESM_EncodeActDedictdEpsbCntxtAccMsg:INFO:NAS_ESM_EncodeActDedictdEpsbCntxtAccMsg been entered!");

    /*���Protocol discriminator��EpsbId����¼��Ϣ����*/
    pTmpSendMsg[ulSum] = ((VOS_UINT8)(ulEpsbId << NAS_ESM_MOVEMENT_4_BITS)\
            & NAS_ESM_MSG_HIGH_BIT) | NAS_ESM_MSG_PD;

    /*��¼��Ϣ����*/
    ulSum++;

    /*���PTI����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESM_PTI_UNASSIGNED_VALUE;

    /*���Message type: NAS_ESMCN_ACT_DEDICTD_EPS_BEARER_CNTXT_ACC����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESMCN_MSG_TYPE_ACT_DEDICTD_EPS_BEARER_CNTXT_ACC;

    *pulLength = ulSum ;
    return ;
}

VOS_VOID NAS_ESM_EncodeActDedictdEpsbCntxtRejMsg( NAS_ESM_ENCODE_INFO_STRU stEncodeInfo,
                                                     VOS_UINT8  *pSendMsg,
                                                     VOS_UINT32 *pulLength)
{
    VOS_UINT8        *pTmpSendMsg  = pSendMsg;
    VOS_UINT32        ulSum        = NAS_ESM_NULL;
    VOS_UINT32        ulEpsbId     = stEncodeInfo.ulEpsbId;

    NAS_ESM_INFO_LOG("NAS_ESM_EncodeActDedictdEpsbCntxtRejMsg:INFO:NAS_ESM_EncodeActDedictdEpsbCntxtRejMsg been entered!");

    /*���Protocol discriminator��EpsbId����¼��Ϣ����*/
    pTmpSendMsg[ulSum] = ((VOS_UINT8)(ulEpsbId << NAS_ESM_MOVEMENT_4_BITS)\
            & NAS_ESM_MSG_HIGH_BIT) | NAS_ESM_MSG_PD;

    /*��¼��Ϣ����*/
    ulSum++;

    /*���PTI����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESM_PTI_UNASSIGNED_VALUE;

    /*���Message type: NAS_ESMCN_ACT_DEDICTD_EPS_BEARER_CNTXT_REJ����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESMCN_MSG_TYPE_ACT_DEDICTD_EPS_BEARER_CNTXT_REJ;

    /*���ESM cause*/
    pTmpSendMsg[ulSum++] = stEncodeInfo.ucESMCau;

    *pulLength = ulSum ;

}
VOS_VOID NAS_ESM_EncodeModEpsbCntxtAccMsg( const NAS_ESM_ENCODE_INFO_STRU *pstEncodeInfo,
                                                            VOS_UINT8  *pSendMsg,
                                                            VOS_UINT32 *pulLength)
{
    VOS_UINT8        *pTmpSendMsg  = pSendMsg;
    VOS_UINT32        ulSum        = NAS_ESM_NULL;

    NAS_ESM_INFO_LOG("NAS_ESM_EncodeModEpsbCntxtAccMsg:INFO:NAS_ESM_EncodeModEpsbCntxtAccMsg been entered!");


    /*���Protocol discriminator��EpsbId����¼��Ϣ����*/
    pTmpSendMsg[ulSum] = ((VOS_UINT8)(pstEncodeInfo->ulEpsbId<< NAS_ESM_MOVEMENT_4_BITS)\
            & NAS_ESM_MSG_HIGH_BIT) | NAS_ESM_MSG_PD;

    /*��¼��Ϣ����*/
    ulSum++;

    /*���PTI����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESM_PTI_UNASSIGNED_VALUE;

    /*���Message type: NAS_ESMCN_MOD_EPS_BEARER_CNTXT_ACC����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESMCN_MSG_TYPE_MOD_EPS_BEARER_CNTXT_ACC;

    *pulLength = ulSum ;
    return;
}


VOS_VOID NAS_ESM_EncodeModEpsbCntxtRejMsg( NAS_ESM_ENCODE_INFO_STRU stEncodeInfo,
                                                     VOS_UINT8  *pSendMsg,
                                                     VOS_UINT32 *pulLength)
{
    VOS_UINT8        *pTmpSendMsg  = pSendMsg;
    VOS_UINT32        ulIndex      = NAS_ESM_NULL;
    VOS_UINT32        ulSum        = NAS_ESM_NULL;

    NAS_ESM_INFO_LOG("NAS_ESM_EncodeModEpsbCntxtRejMsg:INFO:NAS_ESM_EncodeModEpsbCntxtRejMsg been entered!");

    /*���Protocol discriminator��EpsbId����¼��Ϣ����*/
    pTmpSendMsg[ulSum] = ((VOS_UINT8)(stEncodeInfo.ulEpsbId<< NAS_ESM_MOVEMENT_4_BITS)\
            & NAS_ESM_MSG_HIGH_BIT) | NAS_ESM_MSG_PD;

    /*��¼��Ϣ����*/
    ulSum++;

    /*���PTI����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESM_PTI_UNASSIGNED_VALUE;

    /*���Message type: NAS_ESMCN_MOD_EPS_BEARER_CNTXT_REJ����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESMCN_MSG_TYPE_MOD_EPS_BEARER_CNTXT_REJ;

    /*���ESM cause*/
    pTmpSendMsg[ulSum++] = stEncodeInfo.ucESMCau;

    *pulLength = ulSum + ulIndex;
}
VOS_UINT32 NAS_ESM_EncodePdnConReqMsg( NAS_ESM_ENCODE_INFO_STRU stEncodeInfo,
                                                     VOS_UINT8  *pSendMsg,
                                                     VOS_UINT32 *pulLength)
{
    VOS_UINT8                           ucPti        = NAS_ESM_NULL;
    VOS_UINT8                           ucPDNType    = NAS_ESM_NULL;
    VOS_UINT32                          ulIndex      = NAS_ESM_NULL;
    VOS_UINT32                          ulSum        = NAS_ESM_NULL;
    VOS_UINT8                          *pTmpSendMsg  = pSendMsg;
    NAS_ESM_SDF_PARA_STRU              *pstSdfPara   = VOS_NULL_PTR;

    NAS_ESM_INFO_LOG("NAS_ESM_EncodePdnConReqMsg:INFO:NAS_ESM_EncodePdnConReqMsg been entered!");

    ucPti = (VOS_UINT8)stEncodeInfo.ucPti;

    pstSdfPara = NAS_ESM_GetSdfParaAddr(stEncodeInfo.ulCid);

    /*���Protocol discriminator��EpsbId����¼��Ϣ����*/
    pTmpSendMsg[ulSum] = ((VOS_UINT8)(NAS_ESM_UNASSIGNED_EPSB_ID<< NAS_ESM_MOVEMENT_4_BITS)\
            & NAS_ESM_MSG_HIGH_BIT) | NAS_ESM_MSG_PD;

    /*��¼��Ϣ����*/
    ulSum++;

    /*���PTI����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = ucPti;

    /*���Message type: NAS_ESMCN_PDN_CONNECT_REQ����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESMCN_MSG_TYPE_PDN_CONNECT_REQ;

    if (pstSdfPara->bitOpPdnType == NAS_ESM_OP_TRUE)
    {
        ucPDNType = (VOS_UINT8)pstSdfPara->enPdnType;
    }
    else
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodePdnConReqMsg:The sdf doesn't have pdn type!");

        ucPDNType = APP_ESM_PDN_TYPE_IPV4;
    }
    /*���Request typeΪ��4�ֽڵ���λ�����PDN type��Ϊ�ֽڸ���λ
    Request type���ܻ�ɾ����ĿǰĬ��Ϊinit attach״̬ */
    if(NAS_ESM_PDN_REQ_TYPE_HANDOVER == stEncodeInfo.enPdnReqType)
    {
        pTmpSendMsg[ulSum++] = (VOS_UINT8)(NAS_ESM_MSG_PDNCONREQ_HANDOVER\
                        |((ucPDNType & NAS_ESM_MSG_LOW_BIT)<< NAS_ESM_MOVEMENT_4_BITS));

    }
    else if (APP_ESM_BEARER_TYPE_EMERGENCY == pstSdfPara->enBearerCntxtType)
    {
        pTmpSendMsg[ulSum++] = (VOS_UINT8)(NAS_ESM_MSG_PDNCONREQ_EMERGENCY\
                |((ucPDNType & NAS_ESM_MSG_LOW_BIT)<< NAS_ESM_MOVEMENT_4_BITS));
    }
    else
    {
        pTmpSendMsg[ulSum++] = (VOS_UINT8)(NAS_ESM_MSG_PDNCONREQ_INIT\
                |((ucPDNType & NAS_ESM_MSG_LOW_BIT)<< NAS_ESM_MOVEMENT_4_BITS));
    }

    /* set ESM information transfer flag when MM is attaching */
    if (NAS_ESM_PS_REGISTER_STATUS_DETACHED == NAS_ESM_GetEmmStatus())
    {
        if (NAS_ESM_OP_TRUE == pstSdfPara->bitOpApn)
        {
            NAS_ESM_INFO_LOG("NAS_ESM_EncodePdnConReqMsg:NAS->SM Message encode esm info flag!");

            pTmpSendMsg[ulSum++] = NAS_ESM_MSG_ESMTXFG_IEI
                                  |NAS_ESM_INFO_FLAG_ON;
        }
        else /*����û�û������APN�����ؼ�Ȩ��Ϣ������ֱ����PDN����������Я��PCO��Ϣ*/
        {
            if(NAS_ESM_FAILURE == NAS_ESM_EncodePco(pstSdfPara,
                                                    pTmpSendMsg+ulSum,
                                                    &ulIndex,
                                                    NAS_MSG_FORMAT_TLV))
            {
                 /*��ӡ�쳣��Ϣ*/
                NAS_ESM_WARN_LOG("NAS_ESM_EncodePdnConReqMsg:WARNING:NAS->SM Message PCO encode failure!");
                return NAS_ESM_FAILURE;
            }
        }

        *pulLength = ulSum + ulIndex;
        return NAS_ESM_SUCCESS;
    }

    /*���UE�Ѿ�ע��ɹ�������PDN Connect Req��ϢЯ��APN��PCO��Ԫ*/
    if ((NAS_ESM_OP_TRUE == pstSdfPara->bitOpApn) &&
        (APP_ESM_BEARER_TYPE_EMERGENCY != pstSdfPara->enBearerCntxtType))
    {
        NAS_ESM_INFO_LOG("NAS_ESM_EncodePdnConReqMsg:INFO:NAS->SM Message contain APN!");

        if(NAS_ESM_FAILURE == NAS_ESM_EncodeAPN(&pstSdfPara->stApnInfo,
                                                pTmpSendMsg+ulSum,
                                                &ulIndex,
                                                NAS_MSG_FORMAT_TLV))
        {
             /*��ӡ�쳣��Ϣ*/
            NAS_ESM_WARN_LOG("NAS_ESM_EncodePdnConReqMsg:WARNING:NAS->SM Message APN encode failure!");
            return NAS_ESM_FAILURE;
        }
    }

    ulSum = ulSum + ulIndex;
    ulIndex = NAS_ESM_NULL;

    if(NAS_ESM_FAILURE == NAS_ESM_EncodePco(pstSdfPara,
                                            pTmpSendMsg+ulSum,
                                            &ulIndex,
                                            NAS_MSG_FORMAT_TLV))
    {
         /*��ӡ�쳣��Ϣ*/
        NAS_ESM_WARN_LOG("NAS_ESM_EncodePdnConReqMsg:WARNING:NAS->SM Message PCO encode failure!");
        return NAS_ESM_FAILURE;
    }

    *pulLength = ulSum + ulIndex;
    return NAS_ESM_SUCCESS;

}


VOS_UINT32 NAS_ESM_EncodePdnConReqMsg_PPP( const APP_ESM_PDP_SETUP_REQ_STRU *pstAppEsmSetupReqMsg,
                                                     NAS_ESM_ENCODE_INFO_STRU stEncodeInfo,
                                                     VOS_UINT8  *pSendMsg,
                                                     VOS_UINT32 *pulLength)
{
    VOS_UINT8                           ucPti        = NAS_ESM_NULL;
    VOS_UINT8                           ucPDNType    = NAS_ESM_NULL;
    VOS_UINT32                          ulIndex      = NAS_ESM_NULL;
    VOS_UINT32                          ulSum        = NAS_ESM_NULL;
    VOS_UINT8                          *pTmpSendMsg  = pSendMsg;
    NAS_ESM_SDF_PARA_STRU              *pstSdfPara   = VOS_NULL_PTR;


    NAS_ESM_INFO_LOG("NAS_ESM_EncodePdnConReqMsg_PPP:INFO:NAS_ESM_EncodePdnConReqMsg_PPP been entered!");

    ucPti = (VOS_UINT8)stEncodeInfo.ucPti;

    pstSdfPara = NAS_ESM_GetSdfParaAddr(stEncodeInfo.ulCid);

    /*���Protocol discriminator��EpsbId����¼��Ϣ����*/
    pTmpSendMsg[ulSum] = ((VOS_UINT8)(NAS_ESM_UNASSIGNED_EPSB_ID<< NAS_ESM_MOVEMENT_4_BITS)\
            & NAS_ESM_MSG_HIGH_BIT) | NAS_ESM_MSG_PD;

    /*��¼��Ϣ����*/
    ulSum++;

    /*���PTI����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = ucPti;

    /*���Message type: NAS_ESMCN_PDN_CONNECT_REQ����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESMCN_MSG_TYPE_PDN_CONNECT_REQ;

    /*PPP����Ŀǰ������IPv4����*/
    if (pstSdfPara->bitOpPdnType != NAS_ESM_OP_TRUE)

    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodePdnConReqMsg_PPP:The sdf doesn't have pdn type!");
        pstSdfPara->bitOpPdnType = NAS_ESM_OP_TRUE;
    }

    if (pstSdfPara->enPdnType!= APP_ESM_PDN_TYPE_IPV4)

    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodePdnConReqMsg_PPP:The pdn type is not IPv4!");
        pstSdfPara->enPdnType = APP_ESM_PDN_TYPE_IPV4;
    }

    /* PPP���ž�����IPv4����*/
    ucPDNType = APP_ESM_PDN_TYPE_IPV4;

    /*���Request typeΪ��4�ֽڵ���λ�����PDN type��Ϊ�ֽڸ���λ
    Request type���ܻ�ɾ����ĿǰĬ��Ϊinit attach״̬*/
    if (APP_ESM_BEARER_TYPE_EMERGENCY == pstSdfPara->enBearerCntxtType)
    {
        pTmpSendMsg[ulSum++] = (VOS_UINT8)(NAS_ESM_MSG_PDNCONREQ_EMERGENCY\
                |((ucPDNType & NAS_ESM_MSG_LOW_BIT)<< NAS_ESM_MOVEMENT_4_BITS));
    }
    else
    {
        pTmpSendMsg[ulSum++] = (VOS_UINT8)(NAS_ESM_MSG_PDNCONREQ_INIT\
                |((ucPDNType & NAS_ESM_MSG_LOW_BIT)<< NAS_ESM_MOVEMENT_4_BITS));
    }


    /*���UE�Ѿ�ע��ɹ�������PDN Connect Req��ϢЯ��APN��PCO��Ԫ*/
    if((NAS_ESM_OP_TRUE == pstSdfPara->bitOpApn)
        && (NAS_ESM_PS_REGISTER_STATUS_ATTACHED == NAS_ESM_GetEmmStatus())
        && (APP_ESM_BEARER_TYPE_EMERGENCY != pstSdfPara->enBearerCntxtType))
    {
        NAS_ESM_INFO_LOG("NAS_ESM_EncodePdnConReqMsg_PPP:INFO:NAS->SM Message contain APN!");

        if(NAS_ESM_FAILURE == NAS_ESM_EncodeAPN(&pstSdfPara->stApnInfo,
                                                pTmpSendMsg+ulSum,
                                                &ulIndex,
                                                NAS_MSG_FORMAT_TLV))
        {
             /*��ӡ�쳣��Ϣ*/
            NAS_ESM_WARN_LOG("NAS_ESM_EncodePdnConReqMsg_PPP:WARNING:NAS->SM Message APN encode failure!");
            return NAS_ESM_FAILURE;
        }
    }

    ulSum = ulSum + ulIndex;
    ulIndex = NAS_ESM_NULL;

    if(NAS_ESM_FAILURE == NAS_ESM_EncodePco_PPP(pstSdfPara,
                                            &pstAppEsmSetupReqMsg->stPppInfo,
                                            pTmpSendMsg+ulSum,
                                            &ulIndex,
                                            NAS_MSG_FORMAT_TLV))
    {
         /*��ӡ�쳣��Ϣ*/
        NAS_ESM_WARN_LOG("NAS_ESM_EncodePdnConReqMsg_PPP:WARNING:NAS->SM Message PCO encode failure!");
        return NAS_ESM_FAILURE;
    }

    *pulLength = ulSum + ulIndex;
    return NAS_ESM_SUCCESS;

}
VOS_UINT32 NAS_ESM_EncodeEsmInformationRspMsg( NAS_ESM_ENCODE_INFO_STRU stEncodeInfo,
                                                     VOS_UINT8  *pSendMsg,
                                                     VOS_UINT32 *pulLength )
{
    VOS_UINT8                           ucPti               = NAS_ESM_NULL;
    VOS_UINT32                          ulIndex             = NAS_ESM_NULL;
    VOS_UINT32                          ulSum               = NAS_ESM_NULL;
    VOS_UINT32                          ulStateTblIndex     = NAS_ESM_NULL;
    VOS_UINT8                          *pTmpSendMsg         = pSendMsg;
    NAS_ESM_SDF_PARA_STRU              *pstSdfPara          = VOS_NULL_PTR;
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr        = VOS_NULL_PTR;

    NAS_ESM_INFO_LOG("NAS_ESM_EncodeEsmInformationRspMsg:INFO: NAS_ESM_EncodeEsmInformationRspMsg entered!");

    ucPti = stEncodeInfo.ucPti;

    /*����PTI���ҵ�ǰPDN�����������*/
    if(NAS_ESM_FAILURE == NAS_ESM_QueryStateTblIndexByPti(ucPti,&ulStateTblIndex))
    {
        /*��ӡ������Ϣ*/
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeEsmInformationRspMsg:WARNING:NAS->SM Message Can't find Default Bearer Cntxt!");
        return NAS_ESM_FAILURE;
    }

    pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

    /*�����������Ƿ���ȱʡ����*/
    if(PS_FALSE == NAS_ESM_IsDefaultEpsBearerType(pstStateAddr->enBearerCntxtType))
    {
        /*��ӡ������Ϣ*/
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeEsmInformationRspMsg:WARNING:NAS->SM Message Bearer Type Error!");
        return NAS_ESM_FAILURE;
    }

    /*����ȱʡ����CID����ö�ӦCID�������ñ��ַ*/
    pstSdfPara = NAS_ESM_GetSdfParaAddr(pstStateAddr->ulCid);

    /*���Protocol discriminator��EpsbId����¼��Ϣ����*/
    pTmpSendMsg[ulSum] = ((VOS_UINT8)(NAS_ESM_UNASSIGNED_EPSB_ID<< NAS_ESM_MOVEMENT_4_BITS)\
            & NAS_ESM_MSG_HIGH_BIT) | NAS_ESM_MSG_PD;

    /*��¼��Ϣ����*/
    ulSum++;

    /*���PTI����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = ucPti;

    /*���Message type: NAS_ESMCN_MSG_TYPE_ESM_INFORMATION_RSP����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESMCN_MSG_TYPE_ESM_INFORMATION_RSP;

    if(NAS_ESM_OP_TRUE == pstSdfPara->bitOpApn)
    {
        NAS_ESM_INFO_LOG("NAS_ESM_EncodeEsmInformationRspMsg:INFO:NAS->SM Message contain APN!");

        if(NAS_ESM_FAILURE == NAS_ESM_EncodeAPN(&pstSdfPara->stApnInfo,
                                               pTmpSendMsg+ulSum,
                                               &ulIndex,
                                               NAS_MSG_FORMAT_TLV))
        {
             /*��ӡ�쳣��Ϣ*/
            NAS_ESM_WARN_LOG("NAS_ESM_EncodeEsmInformationRspMsg:WARNING:NAS->SM Message APN encode failure!");
            return NAS_ESM_FAILURE;
        }
    }

    ulSum = ulSum + ulIndex;
    ulIndex = NAS_ESM_NULL;
    if(NAS_ESM_FAILURE == NAS_ESM_EncodePco(pstSdfPara,
                                            pTmpSendMsg+ulSum,
                                            &ulIndex,
                                            NAS_MSG_FORMAT_TLV))
    {
         /*��ӡ�쳣��Ϣ*/
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeEsmInformationRspMsg:WARNING:NAS->SM Message PCO encode failure!");
        return NAS_ESM_FAILURE;
    }

    *pulLength = ulSum + ulIndex;
    return NAS_ESM_SUCCESS;

}
VOS_VOID NAS_ESM_EncodeEsmStatus( NAS_ESM_ENCODE_INFO_STRU stEncodeInfo,
                                                     VOS_UINT8  *pSendMsg,
                                                     VOS_UINT32 *pulLength)
{
    VOS_UINT8        *pTmpSendMsg  = pSendMsg;
    VOS_UINT32        ulSum        = NAS_ESM_NULL;

    NAS_ESM_INFO_LOG("NAS_ESM_EncodeEsmStatus:INFO:NAS_ESM_EncodeEsmStatus been entered!");

    /*���Protocol discriminator��EpsbId����¼��Ϣ����*/
    pTmpSendMsg[ulSum] = ((VOS_UINT8)(stEncodeInfo.ulEpsbId << NAS_ESM_MOVEMENT_4_BITS)\
            & NAS_ESM_MSG_HIGH_BIT) | NAS_ESM_MSG_PD;

    /*��¼��Ϣ����*/
    ulSum++;

    /*���PTI����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = stEncodeInfo.ucPti;

    /*���Message type: NAS_ESMCN_MSG_TYPE_ESM_STATUS��*/
    pTmpSendMsg[ulSum++] = NAS_ESMCN_MSG_TYPE_ESM_STATUS;

    /*���ESM cause*/
    pTmpSendMsg[ulSum++] = stEncodeInfo.ucESMCau;

    *pulLength = ulSum ;
}
VOS_VOID NAS_ESM_EncodeActDefltEpsbCntxtAccMsg( NAS_ESM_ENCODE_INFO_STRU stEncodeInfo,
                                                     VOS_UINT8  *pSendMsg,
                                                     VOS_UINT32 *pulLength)
{
    VOS_UINT8        *pTmpSendMsg  = pSendMsg;
    VOS_UINT32        ulIndex      = NAS_ESM_NULL;
    VOS_UINT32        ulSum        = NAS_ESM_NULL;

    NAS_ESM_INFO_LOG("NAS_ESM_EncodeActDefltEpsbCntxtAccMsg:INFO:NAS_ESM_EncodeActDefltEpsbCntxtAccMsg been entered!");

    /*���Protocol discriminator��EpsbId����¼��Ϣ����*/
    pTmpSendMsg[ulSum] = ((VOS_UINT8)(stEncodeInfo.ulEpsbId << NAS_ESM_MOVEMENT_4_BITS)\
            & NAS_ESM_MSG_HIGH_BIT) | NAS_ESM_MSG_PD;

    /*��¼��Ϣ����*/
    ulSum++;

    /*���PTI����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESM_PTI_UNASSIGNED_VALUE;

    /*���Message type: NAS_ESMCN_ACT_DEFLT_EPS_BEARER_CNTXT_ACC����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESMCN_MSG_TYPE_ACT_DEFLT_EPS_BEARER_CNTXT_ACC;

    *pulLength = ulSum + ulIndex;
    return ;

}


VOS_VOID NAS_ESM_EncodeActDefltEpsbCntxtRejMsg( NAS_ESM_ENCODE_INFO_STRU stEncodeInfo,
                                                     VOS_UINT8  *pSendMsg,
                                                     VOS_UINT32 *pulLength)
{
    VOS_UINT8        *pTmpSendMsg  = pSendMsg;
    VOS_UINT32        ulSum        = NAS_ESM_NULL;

    NAS_ESM_INFO_LOG("NAS_ESM_EncodeActDefltEpsbCntxtRejMsg:INFO:NAS_ESM_EncodeActDefltEpsbCntxtRejMsg been entered!");


    /*���Protocol discriminator��EpsbId����¼��Ϣ����*/
    pTmpSendMsg[ulSum] = ((VOS_UINT8)(stEncodeInfo.ulEpsbId << NAS_ESM_MOVEMENT_4_BITS)\
            & NAS_ESM_MSG_HIGH_BIT) | NAS_ESM_MSG_PD;

    /*��¼��Ϣ����*/
    ulSum++;

    /*���PTI����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESM_PTI_UNASSIGNED_VALUE;

    /*���Message type: NAS_ESMCN_ACT_DEDICTD_EPS_BEARER_CNTXT_REJ����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESMCN_MSG_TYPE_ACT_DEFLT_EPS_BEARER_CNTXT_REJ;

    /*���ESM cause*/
    pTmpSendMsg[ulSum++] = stEncodeInfo.ucESMCau;

    *pulLength = ulSum ;
}
VOS_UINT32 NAS_ESM_EncodePdnDisconReqMsg( NAS_ESM_ENCODE_INFO_STRU stEncodeInfo,
                                                     VOS_UINT8  *pSendMsg,
                                                     VOS_UINT32 *pulLength)
{
    VOS_UINT8         ucPti        = NAS_ESM_NULL;
    VOS_UINT8        *pTmpSendMsg  = pSendMsg;
    VOS_UINT32        ulSum        = NAS_ESM_NULL;

    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG("NAS_ESM_EncodePdnDisconReqMsg is entered.");

    ucPti = stEncodeInfo.ucPti;

    /*���Protocol discriminator��û�з���ĳ���ID����¼��Ϣ����*/
    pTmpSendMsg[ulSum] = ((VOS_UINT8)(NAS_ESM_UNASSIGNED_EPSB_ID << NAS_ESM_MOVEMENT_4_BITS)\
            & NAS_ESM_MSG_HIGH_BIT) | NAS_ESM_MSG_PD;

    /*��¼��Ϣ����*/
    ulSum++;

    /*���PTI����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = ucPti;

    /*���Message type: NAS_ESMCN_PDN_DISCONNECT_REQ����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESMCN_MSG_TYPE_PDN_DISCONNECT_REQ;

    /*pTmpSendMsg[ulSum++] = pstEpsbCntxtInfo->ulLinkedEpsbId & NAS_ESM_MSG_LOW_BIT;*/
    /*Э��Ҫ��Linked IdΪȱʡ����Id*/
    pTmpSendMsg[ulSum++] =(VOS_UINT8)(stEncodeInfo.ulEpsbId & NAS_ESM_MSG_LOW_BIT);

    *pulLength = ulSum ;
    return NAS_ESM_SUCCESS;
}
VOS_VOID NAS_ESM_EncodeDeactEpsbCntxtAccMsg( NAS_ESM_ENCODE_INFO_STRU stEncodeInfo,
                                                     VOS_UINT8  *pSendMsg,
                                                     VOS_UINT32 *pulLength)
{
    VOS_UINT8        *pTmpSendMsg  = pSendMsg;
    VOS_UINT32        ulSum        = NAS_ESM_NULL;

    NAS_ESM_INFO_LOG("NAS_ESM_EncodeDeactEpsbCntxtAccMsg:INFO:NAS_ESM_EncodeDeactEpsbCntxtAccMsg been entered!");

    /*���Protocol discriminator��EpsbId����¼��Ϣ����*/
    pTmpSendMsg[ulSum] = ((VOS_UINT8)(stEncodeInfo.ulEpsbId << NAS_ESM_MOVEMENT_4_BITS)\
            & NAS_ESM_MSG_HIGH_BIT) | NAS_ESM_MSG_PD;

    /*��¼��Ϣ����*/
    ulSum++;

    /*���PTI����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESM_PTI_UNASSIGNED_VALUE;

    /*���Message type: NAS_ESMCN_DEACT_EPS_BEARER_CNTXT_ACC����¼��Ϣ����*/
    pTmpSendMsg[ulSum++] = NAS_ESMCN_MSG_TYPE_DEACT_EPS_BEARER_CNTXT_ACC;

    *pulLength = ulSum ;

}


VOS_VOID  NAS_ESM_EncodePapContent
(
    VOS_UINT8                          *pSendMsg,
    APP_ESM_GW_AUTH_INFO_STRU          *pstGwAuthInfo,
    VOS_UINT32                         *pulLength
)
{
    VOS_UINT8                          *pTmpSendMsg     = pSendMsg;
    APP_ESM_GW_AUTH_INFO_STRU          *pstTmpGwAuthInfo=pstGwAuthInfo;
    VOS_UINT32                          ulSum           = *pulLength;
    VOS_UINT8                           ucGwAuthId      = NAS_ESM_NULL;
    VOS_UINT32                          ulIndex         = NAS_ESM_NULL;
    VOS_UINT32                          ulIndex1        = NAS_ESM_NULL;
    VOS_UINT16                          usItemId        = NAS_ESM_NULL;

    NAS_ESM_NORM_LOG("NAS_ESM_EncodePapContent:enter ");

    /*��ֵ Protocol ID */
    usItemId                            = NAS_ESM_PCO_ITEM_TYPE_PAP;
    pTmpSendMsg[ulSum++] = (VOS_UINT8)((usItemId & NAS_ESM_HIGH_BYTE) >> NAS_ESM_MOVEMENT_8_BITS);
    pTmpSendMsg[ulSum++] = (VOS_UINT8)(usItemId & NAS_ESM_LOW_BYTE);

    /*��ֵLength of protocol ID contents*/
    /*����PAP��Ԫ����index��ͳ�Ƴ��Ⱥ����*/
    ulIndex                             = ulSum++;

    /*��ֵProtocol ID content*/
    pTmpSendMsg[ulSum++]                = NAS_ESM_PCO_AUTH_REQ_TYPE;
    NAS_ESM_AssignGwIpId(&ucGwAuthId);
    pTmpSendMsg[ulSum++]                = ucGwAuthId;

    /*����PAP����������Ԫ����index��ͳ�Ƴ��Ⱥ���룬����ռ��2���ֽڣ�
     ʵ�ʲ��ᳬ��һ���ֽڣ���˼�¼��index�ڵ�2���ֽڴ�*/
    pTmpSendMsg[ulSum++]                = NAS_ESM_NULL;
    ulIndex1                            = ulSum++;

    pTmpSendMsg[ulSum++]                = pstTmpGwAuthInfo->ucUserNameLen;
    NAS_ESM_MEM_CPY(                    &pTmpSendMsg[ulSum],
                                        pstTmpGwAuthInfo->aucUserName,
                                        pstTmpGwAuthInfo->ucUserNameLen);
    ulSum                               += pstTmpGwAuthInfo->ucUserNameLen;

    pTmpSendMsg[ulSum++]                = pstTmpGwAuthInfo->ucPwdLen;
    NAS_ESM_MEM_CPY(                    &pTmpSendMsg[ulSum],
                                        pstTmpGwAuthInfo->aucPwd,
                                        pstTmpGwAuthInfo->ucPwdLen);
    ulSum                               += pstTmpGwAuthInfo->ucPwdLen;

    /*�����Ӧ������Ϣ*/
    pTmpSendMsg[ulIndex1]               = (VOS_UINT8)((ulSum-ulIndex)-1);
    pTmpSendMsg[ulIndex]                = (VOS_UINT8)((ulSum-ulIndex)-1);

    *pulLength                          = ulSum;

    return;
}
VOS_VOID  NAS_ESM_EncodeChapContent
(
    VOS_UINT8                          *pSendMsg,
    APP_ESM_GW_AUTH_INFO_STRU          *pstGwAuthInfo,
    VOS_UINT32                         *pulLength
)

{

    VOS_UINT8                          *pTmpSendMsg     = pSendMsg;
    APP_ESM_GW_AUTH_INFO_STRU          *pstTmpGwAuthInfo=pstGwAuthInfo;
    VOS_UINT32                          ulSum           = *pulLength;
    VOS_UINT8                           ucGwAuthId      = NAS_ESM_NULL;
    VOS_UINT8                           aucChlgVal[NAS_ESM_AUTH_CHALLENGE_VALUE_LEN];
    VOS_UINT8                           aucRspVal[NAS_ESM_AUTH_RESPONSE_VALUE_LEN];
    VOS_UINT32                           ulIndex         = NAS_ESM_NULL;
    VOS_UINT32                           ulIndex1        = NAS_ESM_NULL;
    VOS_UINT32                          usItemId     = NAS_ESM_NULL;

    NAS_ESM_NORM_LOG("NAS_ESM_EncodeChapContent:enter ");

    /*���CHAP Challenge message*/
    /*��ֵ Protocol ID */
    usItemId                            = NAS_ESM_PCO_ITEM_TYPE_CHAP;
    pTmpSendMsg[ulSum++] = (VOS_UINT8)((usItemId & NAS_ESM_HIGH_BYTE) >> NAS_ESM_MOVEMENT_8_BITS);
    pTmpSendMsg[ulSum++] = (VOS_UINT8)(usItemId & NAS_ESM_LOW_BYTE);

    /*��ֵLength of protocol ID contents*/
    /*����CHAP��Ԫ����index��ͳ�Ƴ��Ⱥ����*/
    ulIndex                             = ulSum ++;

    /*��ֵProtocol ID content*/
    pTmpSendMsg[ulSum++]                = NAS_ESM_PCO_AUTH_REQ_TYPE;
    NAS_ESM_AssignGwIpId(&ucGwAuthId);
    pTmpSendMsg[ulSum++]                = ucGwAuthId;

    /*����CHAP����������Ԫ����index��ͳ�Ƴ��Ⱥ���룬����ռ��2���ֽڣ�
      ʵ�ʲ��ᳬ��һ���ֽڣ���˼�¼��index�ڵ�2���ֽڴ�*/
    pTmpSendMsg[ulSum++]                = NAS_ESM_NULL;
    ulIndex1                            = ulSum++;


    pTmpSendMsg[ulSum++]                = NAS_ESM_AUTH_CHALLENGE_VALUE_LEN;

    NAS_ESM_GetGwAuthChallengeValue(aucChlgVal);
    NAS_ESM_MEM_CPY(                    &pTmpSendMsg[ulSum],
                                        aucChlgVal,
                                        NAS_ESM_AUTH_CHALLENGE_VALUE_LEN);
    ulSum                               += NAS_ESM_AUTH_CHALLENGE_VALUE_LEN;

    NAS_ESM_MEM_CPY(                    &pTmpSendMsg[ulSum],
                                        pstTmpGwAuthInfo->aucUserName,
                                        pstTmpGwAuthInfo->ucUserNameLen);
    ulSum                               += pstTmpGwAuthInfo->ucUserNameLen;

    /*�����Ӧ������Ϣ*/
    pTmpSendMsg[ulIndex1]               = (VOS_UINT8)((ulSum-ulIndex)-1);
    pTmpSendMsg[ulIndex]                = (VOS_UINT8)((ulSum-ulIndex)-1);


    /*���CHAP Response message*/
    /*ulIndex                             = NAS_ESM_NULL;*/
    ulIndex1                            = NAS_ESM_NULL;
    /*��ֵ Protocol ID */
    usItemId                            = NAS_ESM_PCO_ITEM_TYPE_CHAP;
    pTmpSendMsg[ulSum++] = (VOS_UINT8)((usItemId & NAS_ESM_HIGH_BYTE) >> NAS_ESM_MOVEMENT_8_BITS);
    pTmpSendMsg[ulSum++] = (VOS_UINT8)(usItemId & NAS_ESM_LOW_BYTE);

    /*��ֵLength of protocol ID contents*/
    /*����CHAP��Ԫ����index��ͳ�Ƴ��Ⱥ����*/
    ulIndex                             = ulSum ++;

    /*��ֵProtocol ID content*/
    pTmpSendMsg[ulSum++]                = NAS_ESM_PCO_AUTH_RSP_TYPE;
    pTmpSendMsg[ulSum++]                = ucGwAuthId;

    /*����CHAP����������Ԫ����index��ͳ�Ƴ��Ⱥ���룬����ռ��2���ֽڣ�
      ʵ�ʲ��ᳬ��һ���ֽڣ���˼�¼��index�ڵ�2���ֽڴ�*/
    pTmpSendMsg[ulSum++]                = NAS_ESM_NULL;
    ulIndex1                            = ulSum++;

    pTmpSendMsg[ulSum++]                = NAS_ESM_AUTH_RESPONSE_VALUE_LEN;

    NAS_ESM_GetGwAuthRpsValue(          pstTmpGwAuthInfo->aucPwd,
                                        pstTmpGwAuthInfo->ucPwdLen,
                                        ucGwAuthId,
                                        aucChlgVal,
                                        aucRspVal);

    NAS_ESM_MEM_CPY(                    &pTmpSendMsg[ulSum],
                                        aucRspVal,
                                        NAS_ESM_AUTH_RESPONSE_VALUE_LEN);
    ulSum                               += NAS_ESM_AUTH_RESPONSE_VALUE_LEN;

    NAS_ESM_MEM_CPY(                    &pTmpSendMsg[ulSum],
                                        pstTmpGwAuthInfo->aucUserName,
                                        pstTmpGwAuthInfo->ucUserNameLen);
    ulSum                               += pstTmpGwAuthInfo->ucUserNameLen;

    /*�����Ӧ������Ϣ*/
    pTmpSendMsg[ulIndex1]               = (VOS_UINT8)((ulSum-ulIndex)-1);
    pTmpSendMsg[ulIndex]                = (VOS_UINT8)((ulSum-ulIndex)-1);

    *pulLength                          = ulSum;
    return;
}
VOS_VOID  NAS_ESM_EncodeChapContent_PPP
(
    VOS_UINT8                          *pSendMsg,
    const APP_ESM_AUTH_INFO_STRU       *pstAuthInfo,
    VOS_UINT32                         *pulLength
)

{

    VOS_UINT8                          *pTmpSendMsg     = pSendMsg;
    VOS_UINT32                          ulSum           = *pulLength;

    VOS_UINT32                          ulIndex         = NAS_ESM_NULL;
    VOS_UINT32                          ulIndex1        = NAS_ESM_NULL;
    VOS_UINT32                          usItemId     = NAS_ESM_NULL;

    /*���CHAP Challenge message*/
    /*��ֵ Protocol ID */
    usItemId                            = NAS_ESM_PCO_ITEM_TYPE_CHAP;
    pTmpSendMsg[ulSum++] = (VOS_UINT8)((usItemId & NAS_ESM_HIGH_BYTE) >> NAS_ESM_MOVEMENT_8_BITS);
    pTmpSendMsg[ulSum++] = (VOS_UINT8)(usItemId & NAS_ESM_LOW_BYTE);

    /*��ֵLength of protocol ID contents*/
    /*����CHAP��Ԫ����index��ͳ�Ƴ��Ⱥ����*/
    ulIndex                             = ulSum ++;

    /*��ֵProtocol ID content*/
    pTmpSendMsg[ulSum++]                = NAS_ESM_PCO_AUTH_REQ_TYPE;

    pTmpSendMsg[ulSum++]                = pstAuthInfo->ucAuthId;

    /*����CHAP����������Ԫ����index��ͳ�Ƴ��Ⱥ���룬����ռ��2���ֽڣ�
      ʵ�ʲ��ᳬ��һ���ֽڣ���˼�¼��index�ڵ�2���ֽڴ�*/
    pTmpSendMsg[ulSum++]                = NAS_ESM_NULL;
    ulIndex1                            = ulSum++;


    pTmpSendMsg[ulSum++]                = (VOS_UINT8)pstAuthInfo->unAuthParam.stChapParam.ulChallengeLen;

    NAS_ESM_MEM_CPY(                    &pTmpSendMsg[ulSum],
                                        pstAuthInfo->unAuthParam.stChapParam.aucChallenge,
                                        pstAuthInfo->unAuthParam.stChapParam.ulChallengeLen);
    ulSum                               += pstAuthInfo->unAuthParam.stChapParam.ulChallengeLen;

    NAS_ESM_MEM_CPY(                    &pTmpSendMsg[ulSum],
                                        pstAuthInfo->unAuthParam.stChapParam.aucChallengeName,
                                        pstAuthInfo->unAuthParam.stChapParam.ulChallengeNameLen);
    ulSum                               += pstAuthInfo->unAuthParam.stChapParam.ulChallengeNameLen;

    /*�����Ӧ������Ϣ*/
    pTmpSendMsg[ulIndex1]               = (VOS_UINT8)((ulSum-ulIndex)-1);
    pTmpSendMsg[ulIndex]                = (VOS_UINT8)((ulSum-ulIndex)-1);


    /*���CHAP Response message*/
    /*ulIndex                             = NAS_ESM_NULL;*/
    /*ulIndex1                            = NAS_ESM_NULL;*/
    /*��ֵ Protocol ID */
    usItemId                            = NAS_ESM_PCO_ITEM_TYPE_CHAP;
    pTmpSendMsg[ulSum++] = (VOS_UINT8)((usItemId & NAS_ESM_HIGH_BYTE) >> NAS_ESM_MOVEMENT_8_BITS);
    pTmpSendMsg[ulSum++] = (VOS_UINT8)(usItemId & NAS_ESM_LOW_BYTE);

    /*��ֵLength of protocol ID contents*/
    /*����CHAP��Ԫ����index��ͳ�Ƴ��Ⱥ����*/
    ulIndex                             = ulSum ++;

    /*��ֵProtocol ID content*/
    pTmpSendMsg[ulSum++]                = NAS_ESM_PCO_AUTH_RSP_TYPE;
    pTmpSendMsg[ulSum++]                = pstAuthInfo->ucAuthId;

    /*����CHAP����������Ԫ����index��ͳ�Ƴ��Ⱥ���룬����ռ��2���ֽڣ�
      ʵ�ʲ��ᳬ��һ���ֽڣ���˼�¼��index�ڵ�2���ֽڴ�*/
    pTmpSendMsg[ulSum++]                = NAS_ESM_NULL;
    ulIndex1                            = ulSum++;

    pTmpSendMsg[ulSum++]                = (VOS_UINT8)pstAuthInfo->unAuthParam.stChapParam.ulResponseLen;

    NAS_ESM_MEM_CPY(                    &pTmpSendMsg[ulSum],
                                        pstAuthInfo->unAuthParam.stChapParam.aucResponse,
                                        pstAuthInfo->unAuthParam.stChapParam.ulResponseLen);
    ulSum                               += pstAuthInfo->unAuthParam.stChapParam.ulResponseLen;

    NAS_ESM_MEM_CPY(                    &pTmpSendMsg[ulSum],
                                        pstAuthInfo->unAuthParam.stChapParam.aucResponseName,
                                        pstAuthInfo->unAuthParam.stChapParam.ucResponseNameLen);
    ulSum                               += pstAuthInfo->unAuthParam.stChapParam.ucResponseNameLen;

    /*�����Ӧ������Ϣ*/
    pTmpSendMsg[ulIndex1]               = (VOS_UINT8)((ulSum-ulIndex)-1);
    pTmpSendMsg[ulIndex]                = (VOS_UINT8)((ulSum-ulIndex)-1);

    *pulLength                          = ulSum;
    return;
}


/*****************************************************************************
 Function Name   : NAS_ESM_EncodeIpcpContent
 Description     : IPCP��������
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.sunbing49683      2010-7-15  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ESM_EncodeIpcpContent
(
    const NAS_ESM_SDF_PARA_STRU        *pstSdfPara,
    VOS_UINT8                          *pSendMsg,
    VOS_UINT32                         *pulLength
)
{
    VOS_UINT8                          *pTmpSendMsg     = pSendMsg;
    VOS_UINT32                          ulTmpIndex      = NAS_ESM_NULL;
    VOS_UINT32                          ulTmpIndex2     = NAS_ESM_NULL;
    VOS_UINT16                          usItemId        = NAS_ESM_NULL;
    VOS_UINT32                          ulSum           = *pulLength;
    VOS_UINT8                           ucGwAuthId      = NAS_ESM_NULL;

    if ((NAS_ESM_OP_TRUE == pstSdfPara->bitOpPdnType)
        && (APP_ESM_PDN_TYPE_IPV6 == pstSdfPara->enPdnType))
    {
        NAS_ESM_NORM_LOG("NAS_ESM_EncodeIpcpContent:PDN type is ipv6!");

        *pulLength = ulSum;

        return ;
    }

    /*IPCP for DNS Server begin*/
    usItemId = NAS_ESM_PCO_ITEM_TYPE_IPCP;
    pTmpSendMsg[ulSum++] = (VOS_UINT8)((usItemId & NAS_ESM_HIGH_BYTE)
                                                    >> NAS_ESM_MOVEMENT_8_BITS);
    pTmpSendMsg[ulSum++] = (VOS_UINT8)(usItemId & NAS_ESM_LOW_BYTE);

    /*����IPCP��Ԫ����index��ͳ�Ƴ��Ⱥ����*/
    ulTmpIndex = ulSum++;

    /*��дIPCP����*/
    /*
     0                   1                   2                   3
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |     Code      |  Identifier   |            Length             |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    | Options ...
    +-+-+-+-+
    */
    pTmpSendMsg[ulSum++] = NAS_ESM_IPCP_MSG_TYPE_CONFIGURE_REQ;

    NAS_ESM_AssignGwIpId(&ucGwAuthId);
    pTmpSendMsg[ulSum++] = ucGwAuthId;/*id*/

    /*����IPCP����������Ԫ����index��ͳ�Ƴ��Ⱥ���룬����ռ��2���ֽڣ�
      ʵ�ʲ��ᳬ��һ���ֽڣ���˼�¼��index�ڵ�2���ֽڴ�*/
    pTmpSendMsg[ulSum++] = NAS_ESM_NULL;
    ulTmpIndex2 = ulSum++;

    /*��дIPCP Options����*/
    /*
     0                   1                   2                   3
     0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |     Type      |    Length     |           IP-Address
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
            IP-Address (cont)       |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    */
    pTmpSendMsg[ulSum++] = NAS_ESM_IPCP_OPTIONS_PRI_DNS_ADDR;
    pTmpSendMsg[ulSum++] = NAS_ESM_IPCP_OPTION_DNS_LEN;
    pTmpSendMsg[ulSum++] = 0;
    pTmpSendMsg[ulSum++] = 0;
    pTmpSendMsg[ulSum++] = 0;
    pTmpSendMsg[ulSum++] = 0;
    pTmpSendMsg[ulSum++] = NAS_ESM_IPCP_OPTIONS_SEC_DNS_ADDR;
    pTmpSendMsg[ulSum++] = NAS_ESM_IPCP_OPTION_DNS_LEN;
    pTmpSendMsg[ulSum++] = 0;
    pTmpSendMsg[ulSum++] = 0;
    pTmpSendMsg[ulSum++] = 0;
    pTmpSendMsg[ulSum++] = 0;

    /*������Ӧ������Ϣ*/
    pTmpSendMsg[ulTmpIndex2] = (VOS_UINT8)((ulSum - ulTmpIndex) -1);
    pTmpSendMsg[ulTmpIndex]  = (VOS_UINT8)((ulSum - ulTmpIndex) -1);
    /*IPCP for DNS Server end*/

    *pulLength = ulSum;
}
/*****************************************************************************
 Function Name   : NAS_ESM_EncodeIpv4AllocMode
 Description     : ����IPV4 ��ȡ��ʽ
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2011-03-30  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ESM_EncodeIpv4AllocMode
(
    const NAS_ESM_SDF_PARA_STRU        *pstSdfPara,
    VOS_UINT8                          *pSendMsg,
    VOS_UINT32                         *pulLength
)
{
    VOS_UINT16                          usItemId        = NAS_ESM_NULL;
    VOS_UINT32                          ulSum           = *pulLength;
    VOS_UINT8                          *pTmpSendMsg     = pSendMsg;

    if ((NAS_ESM_OP_TRUE == pstSdfPara->bitOpPdnType)
        && (APP_ESM_PDN_TYPE_IPV6 == pstSdfPara->enPdnType))
    {
        NAS_ESM_NORM_LOG("NAS_ESM_EncodeIpv4AllocMode:PDN type is ipv6!");

        *pulLength = ulSum;

        return ;
    }

    /*��ֵContainer ID:IP address allocation via NAS signalling */
    usItemId = NAS_ESM_PCO_ITEM_TYPE_NAS_SIGNALING;
    pTmpSendMsg[ulSum++] = (VOS_UINT8)((usItemId & NAS_ESM_HIGH_BYTE)
                                                    >> NAS_ESM_MOVEMENT_8_BITS);
    pTmpSendMsg[ulSum++] = (VOS_UINT8)(usItemId & NAS_ESM_LOW_BYTE);

    /*��ֵContainer LEN:IP address allocation via NAS signalling */
    pTmpSendMsg[ulSum++] = NAS_ESM_NULL;

    *pulLength                          = ulSum;
    return;
}


/*****************************************************************************
 Function Name   : NAS_ESM_EncodeIpv4DnsReq
 Description     : ����IPV4 DNS����
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2011-03-30  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ESM_EncodeIpv4DnsReq
(
    const NAS_ESM_SDF_PARA_STRU              *pstSdfPara,
    VOS_UINT8                          *pSendMsg,
    VOS_UINT32                         *pulLength
)
{
    VOS_UINT16                          usItemId        = NAS_ESM_NULL;
    VOS_UINT32                          ulSum           = *pulLength;
    VOS_UINT8                          *pTmpSendMsg     = pSendMsg;

    if ((NAS_ESM_OP_TRUE == pstSdfPara->bitOpPdnType)
        && (APP_ESM_PDN_TYPE_IPV6 == pstSdfPara->enPdnType))
    {
        NAS_ESM_NORM_LOG("NAS_ESM_EncodeIpv4DnsReq:PDN type is ipv6!");

        *pulLength = ulSum;

        return ;
    }

    /*��ֵContainer ID:DNS Server IPv4 Address Request */
    usItemId = NAS_ESM_PCO_ITEM_TYPE_DNS_SERVER_IPV4;
    pTmpSendMsg[ulSum++] = (VOS_UINT8)((usItemId & NAS_ESM_HIGH_BYTE)
                                                    >> NAS_ESM_MOVEMENT_8_BITS);
    pTmpSendMsg[ulSum++] = (VOS_UINT8)(usItemId & NAS_ESM_LOW_BYTE);

    /*��ֵContainer LEN:DNS Server IPv4 Address Request */
    pTmpSendMsg[ulSum++] = NAS_ESM_NULL;

    *pulLength                          = ulSum;
    return;
}

/*****************************************************************************
 Function Name   : NAS_ESM_EncodeIpv4PcscfReq
 Description     : ����IPV4 P-CSCF����
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2011-03-30  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ESM_EncodeIpv4PcscfReq
(
    const NAS_ESM_SDF_PARA_STRU              *pstSdfPara,
    VOS_UINT8                          *pSendMsg,
    VOS_UINT32                         *pulLength
)
{
    VOS_UINT16                          usItemId        = NAS_ESM_NULL;
    VOS_UINT32                          ulSum           = *pulLength;
    VOS_UINT8                          *pTmpSendMsg     = pSendMsg;

    if ((NAS_ESM_OP_TRUE == pstSdfPara->bitOpPdnType)
        && (APP_ESM_PDN_TYPE_IPV6 == pstSdfPara->enPdnType))
    {
        NAS_ESM_NORM_LOG("NAS_ESM_EncodeIpv4PcscfReq:PDN type is ipv6!");

        *pulLength = ulSum;

        return ;
    }

    /*��ֵContainer ID:P-CSCF IPv4 Address Request */
    usItemId = NAS_ESM_PCO_ITEM_TYPE_PCSCF_IPV4;
    pTmpSendMsg[ulSum++] = (VOS_UINT8)((usItemId & NAS_ESM_HIGH_BYTE)
                                                    >> NAS_ESM_MOVEMENT_8_BITS);
    pTmpSendMsg[ulSum++] = (VOS_UINT8)(usItemId & NAS_ESM_LOW_BYTE);

    /*��ֵContainer LEN:P-CSCF IPv4 Address Request */
    pTmpSendMsg[ulSum++] = NAS_ESM_NULL;

    *pulLength                          = ulSum;
    return;
}
/* lihong00150010 incorporation st end */

/* lihong00150010 ims begin */
/*****************************************************************************
 Function Name   : NAS_ESM_EncodeIpv6PcscfReq
 Description     : ����IPV6 P-CSCF����
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2013-07-11  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ESM_EncodeIpv6PcscfReq
(
    const NAS_ESM_SDF_PARA_STRU        *pstSdfPara,
    VOS_UINT8                          *pSendMsg,
    VOS_UINT32                         *pulLength
)
{
    VOS_UINT16                          usItemId        = NAS_ESM_NULL;
    VOS_UINT32                          ulSum           = *pulLength;
    VOS_UINT8                          *pTmpSendMsg     = pSendMsg;

    if ((NAS_ESM_OP_TRUE == pstSdfPara->bitOpPdnType)
        && (APP_ESM_PDN_TYPE_IPV4 == pstSdfPara->enPdnType))
    {
        NAS_ESM_NORM_LOG("NAS_ESM_EncodeIpv6PcscfReq:PDN type is ipv4!");

        *pulLength = ulSum;

        return ;
    }

    /*��ֵContainer ID:P-CSCF IPv6 Address Request */
    usItemId = NAS_ESM_PCO_ITEM_TYPE_PCSCF_IPV6;
    pTmpSendMsg[ulSum++] = (VOS_UINT8)((usItemId & NAS_ESM_HIGH_BYTE)
                                                    >> NAS_ESM_MOVEMENT_8_BITS);
    pTmpSendMsg[ulSum++] = (VOS_UINT8)(usItemId & NAS_ESM_LOW_BYTE);

    /*��ֵContainer LEN:P-CSCF IPv6 Address Request */
    pTmpSendMsg[ulSum++] = NAS_ESM_NULL;

    *pulLength                          = ulSum;
    return;
}

/* lihong00150010 ims end */


/*****************************************************************************
 Function Name   : NAS_ESM_EncodePcoBcm
 Description     : ����BCM
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2011-04-27  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ESM_EncodePcoBcm
(
    VOS_UINT8                          *pSendMsg,
    VOS_UINT32                         *pulLength
)
{
    VOS_UINT16                          usItemId        = NAS_ESM_NULL;
    VOS_UINT32                          ulSum           = *pulLength;
    VOS_UINT8                          *pTmpSendMsg     = pSendMsg;
    /*EMM_ESM_RAT_TYPE_ENUM_UINT8         enRatType       = EMM_ESM_RAT_BUTT;*/

    if (NAS_ESM_GetBcmFlag() == NAS_ESM_FUN_CTRL_OFF)
    {
        return ;
    }

    /* ���ֻ֧��LTE����֧��A/Gb����Iuʱ��������BCM */
    if (NAS_ESM_FAILURE == NAS_EMM_lteRatIsExist())
    {
        return ;
    }

    /*��ֵMS Support of Network Requested Bearer Control indicator */
    usItemId = NAS_ESM_PCO_ITEM_TYPE_BCM;
    pTmpSendMsg[ulSum++] = (VOS_UINT8)((usItemId & NAS_ESM_HIGH_BYTE)
                                                    >> NAS_ESM_MOVEMENT_8_BITS);
    pTmpSendMsg[ulSum++] = (VOS_UINT8)(usItemId & NAS_ESM_LOW_BYTE);

    /*��ֵContainer LEN:MS Support of Network Requested Bearer Control indicator */
    pTmpSendMsg[ulSum++] = NAS_ESM_NULL;

    *pulLength                          = ulSum;
    return;
}

/*****************************************************************************
 Function Name   : NAS_ESM_EncodeIpv6DnsReq
 Description     : ����IPV6 DNS����
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2011-03-30  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ESM_EncodeIpv6DnsReq
(
    const NAS_ESM_SDF_PARA_STRU              *pstSdfPara,
    VOS_UINT8                          *pSendMsg,
    VOS_UINT32                         *pulLength
)
{
    VOS_UINT16                          usItemId        = NAS_ESM_NULL;
    VOS_UINT32                          ulSum           = *pulLength;
    VOS_UINT8                          *pTmpSendMsg     = pSendMsg;

    if ((NAS_ESM_OP_TRUE == pstSdfPara->bitOpPdnType)
        && ((APP_ESM_PDN_TYPE_IPV6 == pstSdfPara->enPdnType)
            || (APP_ESM_PDN_TYPE_IPV4_IPV6 == pstSdfPara->enPdnType)))
    {
        /*��ֵContainer ID:DNS Server IPv6 Address Request */
        usItemId = NAS_ESM_PCO_ITEM_TYPE_DNS_SERVER_IPV6;
        pTmpSendMsg[ulSum++] = (VOS_UINT8)((usItemId & NAS_ESM_HIGH_BYTE)
                                                        >> NAS_ESM_MOVEMENT_8_BITS);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)(usItemId & NAS_ESM_LOW_BYTE);

        /*��ֵContainer LEN:DNS Server IPv6 Address Request */
        pTmpSendMsg[ulSum++] = NAS_ESM_NULL;
    }

    *pulLength                          = ulSum;
    return;
}

/* lihong00150010 ims begin */
/*****************************************************************************
 Function Name   : NAS_ESM_EncodeImCnSignalFlag
 Description     : ����"IM CN Subsystem Signaling Flag"
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2013-07-11  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ESM_EncodeImCnSignalFlag
(
    const NAS_ESM_SDF_PARA_STRU        *pstSdfPara,
    VOS_UINT8                          *pSendMsg,
    VOS_UINT32                         *pulLength
)
{
    VOS_UINT16                          usItemId        = NAS_ESM_NULL;
    VOS_UINT32                          ulSum           = *pulLength;
    VOS_UINT8                          *pTmpSendMsg     = pSendMsg;

    if ((NAS_ESM_OP_TRUE != pstSdfPara->bitOpImsCnSignalFlag)
        || (APP_ESM_PDP_FOR_IMS_CN_SIG_ONLY != pstSdfPara->enImsCnSignalFlag))
    {
        NAS_ESM_NORM_LOG("NAS_ESM_EncodeImCnSignalFlag:not ims bearer!");

        *pulLength = ulSum;

        return ;
    }

    /*��ֵContainer ID:IM CN Subsystem Signaling Flag */
    usItemId                = NAS_ESM_PCO_ITEM_TYPE_IM_CN_SIGNAL_FLAG;
    pTmpSendMsg[ulSum++]    = (VOS_UINT8)((usItemId & NAS_ESM_HIGH_BYTE)
                                                    >> NAS_ESM_MOVEMENT_8_BITS);
    pTmpSendMsg[ulSum++]    = (VOS_UINT8)(usItemId & NAS_ESM_LOW_BYTE);

    /*��ֵContainer LEN:IM CN Subsystem Signaling Flag */
    pTmpSendMsg[ulSum++]    = NAS_ESM_NULL;

    *pulLength              = ulSum;
    return;
}
/* lihong00150010 ims end */

/*****************************************************************************
 Function Name  : NAS_ESM_IsNeedEncodePapOrChap
 Description    : �ж��Ƿ���Ҫ����PAP����CHAP
 Input          : NAS_ESM_SDF_PARA_STRU              *pstSdfPara
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.lihong00150010      2012-11-30  Draft Enact

*****************************************************************************/
VOS_UINT32 NAS_ESM_IsNeedEncodePapOrChap
(
    const NAS_ESM_SDF_PARA_STRU        *pstSdfPara
)
{
    if ((APP_ESM_BEARER_TYPE_EMERGENCY != pstSdfPara->enBearerCntxtType) &&
        (NAS_ESM_OP_TRUE == pstSdfPara->bitOpGwAuthInfo) &&
        (pstSdfPara->stGwAuthInfo.ucUserNameLen > NAS_ESM_NULL) &&
        (NAS_ESM_OP_TRUE == pstSdfPara->bitOpApn))
    {
        return VOS_TRUE;
    }
    else
    {
        return VOS_FALSE;
    }
}


VOS_UINT32 NAS_ESM_EncodePco
(
    NAS_ESM_SDF_PARA_STRU              *pstSdfPara,
    VOS_UINT8                          *pSendMsg,
    VOS_UINT32                         *pulLength,
    NAS_MSG_FORMAT_ENUM_UINT8           enEncodeTp
)
{
    VOS_UINT8                          *pTmpSendMsg     = pSendMsg;
    VOS_UINT32                          ulSum           = *pulLength;
    VOS_UINT32                          ulIndex         = NAS_ESM_NULL;
    VOS_UINT32                          ulTmpIndex      = NAS_ESM_NULL;
    APP_ESM_GW_AUTH_INFO_STRU          *pstTmpGwAuthInfo =&pstSdfPara->stGwAuthInfo;


    NAS_ESM_INFO_LOG("NAS_ESM_EncodePco:INFO:NAS_ESM_EncodePCO been entered!");

    if( VOS_NULL_PTR == pTmpSendMsg)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodePco:ERROR: No SmMsg!");
        return NAS_ESM_FAILURE;
    }

    /* ���PCO�����ѹرգ���ֱ�ӷ��سɹ� */
    if (NAS_ESM_PCO_OFF == NAS_ESM_GetPcoFlag())
    {
        NAS_ESM_NORM_LOG("NAS_ESM_EncodePco:PCO fuction is closed!");
        return NAS_ESM_SUCCESS;
    }

    if(NAS_MSG_FORMAT_TLV == enEncodeTp)
    {
        /*��ѡ����,���PCO��ϢIEI����*/
        pTmpSendMsg[ulSum++] = NAS_ESM_MSG_PCO_IEI;
    }

    /*���泤��Index������ͳ�ƺ��ٴ��*/
    ulIndex = ulSum++;

    /*����PPP for use with IP PDP type����*/
    pTmpSendMsg[ulSum++] = NAS_ESM_PCO_CONFIG_PROTOCOL;

    /* Ŀǰ�ж�bitOpGwAuthInfoΪ1�����û������ȴ���0������Ϊ���ڼ�Ȩ��Ϣ ,�Ҵ���APNʱ�Ż��ڱ���PCO��Ԫ�д�������*/
    if (VOS_TRUE == NAS_ESM_IsNeedEncodePapOrChap(pstSdfPara))
    {
        if ((APP_ESM_MAX_USER_NAME_LEN < pstSdfPara->stGwAuthInfo.ucUserNameLen)
            || (APP_ESM_MAX_PASSWORD_LEN < pstSdfPara->stGwAuthInfo.ucPwdLen))
        {
             NAS_ESM_NORM_LOG("NAS_ESM_EncodePco:Input gw auth len is err!");

        }
        else
        {
            if (APP_ESM_AUTH_TYPE_PAP == pstTmpGwAuthInfo->enGwAuthType)
            {
                ulTmpIndex                      = NAS_ESM_NULL;
                NAS_ESM_EncodePapContent(pTmpSendMsg + ulSum,pstTmpGwAuthInfo,&ulTmpIndex);
                ulSum                           = ulSum + ulTmpIndex;
            }

            if (APP_ESM_AUTH_TYPE_CHAP == pstTmpGwAuthInfo->enGwAuthType)
            {

                NAS_ESM_EncodeChapContent(pTmpSendMsg + ulSum,pstTmpGwAuthInfo,&ulTmpIndex);
                ulSum                           = ulSum + ulTmpIndex;
            }
        }
    }

    /*#if (NAS_ESM_RELEASE_CTRL_R8 == NAS_ESM_RELEASE_CTRL)*/
    ulTmpIndex                          = NAS_ESM_NULL;
    NAS_ESM_EncodeIpcpContent(pstSdfPara, (pTmpSendMsg + ulSum),&ulTmpIndex);
    ulSum                               = ulSum + ulTmpIndex;
    /*#endif*/

    if (NAS_ESM_GET_IPV6_DNS_CTRL_ON == NAS_ESM_GetIpv6DnsFlag())
    {
       ulTmpIndex                          = NAS_ESM_NULL;

       /*��ֵContainer ID:IPv6 DNS Request */
       NAS_ESM_EncodeIpv6DnsReq(pstSdfPara, (pTmpSendMsg + ulSum),&ulTmpIndex);
       ulSum                               = ulSum + ulTmpIndex;

       /*Ϊ��ȡ�ڶ���IPv6 DNS������ʵ��������֤����ҪЯ���ڶ���IPv6��DNS���� */
       /*��ֵContainer ID:IPv6 DNS Request */
       /*ulTmpIndex                          = NAS_ESM_NULL;
       NAS_ESM_EncodeIpv6DnsReq(pstSdfPara, (pTmpSendMsg + ulSum),&ulTmpIndex);
       ulSum                               = ulSum + ulTmpIndex;*/
    }

    ulTmpIndex                          = NAS_ESM_NULL;
    NAS_ESM_EncodeIpv4AllocMode(pstSdfPara, (pTmpSendMsg + ulSum),&ulTmpIndex);
    ulSum                               = ulSum + ulTmpIndex;

    /* lihong00150010 ims begin */
    /* �����IMS���أ���Я��"0002H (IM CN Subsystem Signaling Flag)" */
    ulTmpIndex                          = NAS_ESM_NULL;
    NAS_ESM_EncodeImCnSignalFlag(pstSdfPara, (pTmpSendMsg + ulSum),&ulTmpIndex);
    ulSum                               = ulSum + ulTmpIndex;

    if ((NAS_ESM_OP_TRUE == pstSdfPara->bitOpImsCnSignalFlag)
        && (APP_ESM_PDP_FOR_IMS_CN_SIG_ONLY == pstSdfPara->enImsCnSignalFlag))
    {
        ulTmpIndex                          = NAS_ESM_NULL;
        NAS_ESM_EncodeIpv4PcscfReq(pstSdfPara, (pTmpSendMsg + ulSum),&ulTmpIndex);
        ulSum                               = ulSum + ulTmpIndex;

        /*Ϊ��ȡ����IPV4 P-CSCF�����ڱ���ڶ���IPV4 P-CSCF����
          �ƶ�VoLTE���ԣ��������粻֧�ַ�������P-CSCF����Э��û����ȷ������ɾ�����λ�ȡ
        */
        /*ulTmpIndex                          = NAS_ESM_NULL;
        NAS_ESM_EncodeIpv4PcscfReq(pstSdfPara, (pTmpSendMsg + ulSum),&ulTmpIndex);
        ulSum                               = ulSum + ulTmpIndex;*/

        ulTmpIndex                          = NAS_ESM_NULL;
        NAS_ESM_EncodeIpv6PcscfReq(pstSdfPara, (pTmpSendMsg + ulSum),&ulTmpIndex);
        ulSum                               = ulSum + ulTmpIndex;

        /*Ϊ��ȡ����IPV6 P-CSCF�����ڱ���ڶ���IPV6 P-CSCF����
          �ƶ�VoLTE���ԣ��������粻֧�ַ�������P-CSCF����Э��û����ȷ������ɾ�����λ�ȡ
        */
        /*ulTmpIndex                          = NAS_ESM_NULL;
        NAS_ESM_EncodeIpv6PcscfReq(pstSdfPara, (pTmpSendMsg + ulSum),&ulTmpIndex);
        ulSum                               = ulSum + ulTmpIndex;*/
        /* lihong00150010 ims end */
    }


    /*#if (NAS_ESM_RELEASE_CTRL_R9 == NAS_ESM_RELEASE_CTRL)*/
    ulTmpIndex                          = NAS_ESM_NULL;
    NAS_ESM_EncodeIpv4DnsReq(pstSdfPara, (pTmpSendMsg + ulSum),&ulTmpIndex);
    ulSum                               = ulSum + ulTmpIndex;
    /*Ϊ��ȡ����DNS�����ڱ���ڶ���DNS���� */
    /*ulTmpIndex                          = NAS_ESM_NULL;
    NAS_ESM_EncodeIpv4DnsReq(pstSdfPara, (pTmpSendMsg + ulSum),&ulTmpIndex);
    ulSum                               = ulSum + ulTmpIndex;*/
    /*#endif*/


    ulTmpIndex                          = NAS_ESM_NULL;
    NAS_ESM_EncodePcoBcm((pTmpSendMsg + ulSum), &ulTmpIndex);
    ulSum                               = ulSum + ulTmpIndex;

    /*��ֵLength of protocol config. options contents*/
    pTmpSendMsg[ulIndex] = (VOS_UINT8)((ulSum - ulIndex) - 1);

    if(ulSum <= NAS_ESM_MAX_PCO_BYTE)
    {
        *pulLength = ulSum;
    }
    else
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodePco:WARNING:NAS->SM Message PCO Length slop over!");
        return NAS_ESM_FAILURE;
    }

    return NAS_ESM_SUCCESS;
}

VOS_UINT32 NAS_ESM_EncodePco_PPP
(
    const NAS_ESM_SDF_PARA_STRU              *pstSdfPara,
    const APP_ESM_PPP_INFO_STRU        *pstPppInfo,
    VOS_UINT8                          *pSendMsg,
    VOS_UINT32                         *pulLength,
    NAS_MSG_FORMAT_ENUM_UINT8           enEncodeTp
)
{
    VOS_UINT8                          *pTmpSendMsg     = pSendMsg;
    VOS_UINT32                          ulSum           = *pulLength;
    VOS_UINT32                          ulIndex         = NAS_ESM_NULL;
    VOS_UINT32                          ulTmpIndex      = NAS_ESM_NULL;
    APP_ESM_GW_AUTH_INFO_STRU          *pstTmpGwAuthInfo = {NAS_ESM_NULL};


    NAS_ESM_INFO_LOG("NAS_ESM_EncodePco_PPP:INFO:NAS_ESM_EncodePco_PPP been entered!");

    if( VOS_NULL_PTR == pTmpSendMsg)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodePco_PPP:ERROR: No SmMsg!");
        return NAS_ESM_FAILURE;
    }

    /* ���PCO�����ѹرգ���ֱ�ӷ��سɹ� */
    if (NAS_ESM_PCO_OFF == NAS_ESM_GetPcoFlag())
    {
        NAS_ESM_NORM_LOG("NAS_ESM_EncodePco_PPP:PCO fuction is closed!");
        return NAS_ESM_SUCCESS;
    }

    /*������ʱ�����ڴ� */
    pstTmpGwAuthInfo = NAS_ESM_MEM_ALLOC(sizeof(APP_ESM_GW_AUTH_INFO_STRU));
    if (VOS_NULL_PTR == pstTmpGwAuthInfo)
    {
        NAS_ESM_ERR_LOG("NAS_ESM_EncodePco_PPP:Mem Alloc Fail!");
        return NAS_ESM_FAILURE;
    }

    if(NAS_MSG_FORMAT_TLV == enEncodeTp)
    {
        /*��ѡ����,���PCO��ϢIEI����*/
        pTmpSendMsg[ulSum++] = NAS_ESM_MSG_PCO_IEI;
    }

    /*���泤��Index������ͳ�ƺ��ٴ��*/
    ulIndex = ulSum++;

    /*����PPP for use with IP PDP type����*/
    pTmpSendMsg[ulSum++] = NAS_ESM_PCO_CONFIG_PROTOCOL;

    /* ��Ȩ��ʽ��ΪNONEʱ��APN����ʱ�ű����Ȩ��Ϣ*/
    if ((APP_ESM_AUTH_TYPE_NONE != pstPppInfo->stAuthInfo.enAuthType)
         && (NAS_ESM_OP_TRUE == pstSdfPara->bitOpApn)
         && (VOS_TRUE == NAS_ESM_VerifyAppAuthParam_PPP(&(pstPppInfo->stAuthInfo)))
         && (APP_ESM_BEARER_TYPE_EMERGENCY != pstSdfPara->enBearerCntxtType))
    {
        /*niuxiufan modify begin */
        if (APP_ESM_AUTH_TYPE_PAP == pstPppInfo->stAuthInfo.enAuthType)
        {

            pstTmpGwAuthInfo->ucUserNameLen = pstPppInfo->stAuthInfo.unAuthParam.stPapParam.ucUsernameLen;
            pstTmpGwAuthInfo->ucPwdLen = pstPppInfo->stAuthInfo.unAuthParam.stPapParam.ucPasswordLen;

            NAS_ESM_MEM_CPY(pstTmpGwAuthInfo->aucUserName,
                            pstPppInfo->stAuthInfo.unAuthParam.stPapParam.aucUsername,
                            pstTmpGwAuthInfo->ucUserNameLen);

            NAS_ESM_MEM_CPY(pstTmpGwAuthInfo->aucPwd,
                            pstPppInfo->stAuthInfo.unAuthParam.stPapParam.aucPassword,
                            pstTmpGwAuthInfo->ucPwdLen);

            ulTmpIndex                      = NAS_ESM_NULL;
            NAS_ESM_EncodePapContent(pTmpSendMsg + ulSum,pstTmpGwAuthInfo,&ulTmpIndex);
            ulSum                           = ulSum + ulTmpIndex;

        }

        if (APP_ESM_AUTH_TYPE_CHAP == pstPppInfo->stAuthInfo.enAuthType)
        {

            NAS_ESM_EncodeChapContent_PPP(pTmpSendMsg + ulSum,&(pstPppInfo->stAuthInfo),&ulTmpIndex);
            ulSum                           = ulSum + ulTmpIndex;
        }
        /* niuxiufan modify end */
    }

    /*�ͷ��ڴ� */
    NAS_ESM_MEM_FREE(pstTmpGwAuthInfo);

    #if (NAS_ESM_RELEASE_CTRL_R8 == NAS_ESM_RELEASE_CTRL)
    ulTmpIndex                          = NAS_ESM_NULL;
    NAS_ESM_EncodeIpcpContent(pstSdfPara, (pTmpSendMsg + ulSum),&ulTmpIndex);
    ulSum                               = ulSum + ulTmpIndex;
    #endif


    ulTmpIndex                          = NAS_ESM_NULL;
    NAS_ESM_EncodeIpv4AllocMode(pstSdfPara, (pTmpSendMsg + ulSum),&ulTmpIndex);
    ulSum                               = ulSum + ulTmpIndex;

    if (NAS_ESM_GetPcscfFlag() == NAS_ESM_FUN_CTRL_ON)
    {
        ulTmpIndex                          = NAS_ESM_NULL;
        NAS_ESM_EncodeIpv4PcscfReq(pstSdfPara, (pTmpSendMsg + ulSum),&ulTmpIndex);
        ulSum                               = ulSum + ulTmpIndex;
    }


    #if (NAS_ESM_RELEASE_CTRL_R9 == NAS_ESM_RELEASE_CTRL)
    ulTmpIndex                          = NAS_ESM_NULL;
    NAS_ESM_EncodeIpv4DnsReq(pstSdfPara, (pTmpSendMsg + ulSum),&ulTmpIndex);
    ulSum                               = ulSum + ulTmpIndex;
    /*Ϊ��ȡ����DNS�����ڱ���ڶ���DNS���� */
    ulTmpIndex                          = NAS_ESM_NULL;
    NAS_ESM_EncodeIpv4DnsReq(pstSdfPara, (pTmpSendMsg + ulSum),&ulTmpIndex);
    ulSum                               = ulSum + ulTmpIndex;
    #endif


    ulTmpIndex                          = NAS_ESM_NULL;
    NAS_ESM_EncodePcoBcm((pTmpSendMsg + ulSum), &ulTmpIndex);
    ulSum                               = ulSum + ulTmpIndex;

    /*��ֵLength of protocol config. options contents*/
    pTmpSendMsg[ulIndex] = (VOS_UINT8)((ulSum - ulIndex) - 1);

    if(ulSum <= NAS_ESM_MAX_PCO_BYTE)
    {
        *pulLength = ulSum;
    }
    else
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodePco:WARNING:NAS->SM Message PCO Length slop over!");
        return NAS_ESM_FAILURE;
    }

    return NAS_ESM_SUCCESS;
}
VOS_UINT32  NAS_ESM_IsApnCharacterValid
(
    VOS_UINT8 ucCharacter,
    NAS_ESM_APN_VERIFY_CHARACTER_TYPE_ENUM_UINT8 enApnVerifyCharacterType
)
{
    PS_BOOL_ENUM_UINT8                  enIsUpperAToZ = PS_FALSE;
    PS_BOOL_ENUM_UINT8                  enIsLowerAToZ = PS_FALSE;
    PS_BOOL_ENUM_UINT8                  enIsDigit     = PS_FALSE;
    PS_BOOL_ENUM_UINT8                  enIsHyphen    = PS_FALSE;

    /* �ж��Ƿ�Ϊ�ַ�A-Z */
    enIsUpperAToZ = ((ucCharacter >= 'A') && (ucCharacter <= 'Z'));

    /* �ж��Ƿ�Ϊ�ַ�a-z */
    enIsLowerAToZ = ((ucCharacter >= 'a') && (ucCharacter <= 'z'));

    /* �ж��Ƿ�Ϊ�ַ�0-9 */
    enIsDigit = ((ucCharacter >= '0') && (ucCharacter <= '9'));

    /* �ж��Ƿ�Ϊ���ӷ�- */
    enIsHyphen = (ucCharacter == '-');

    /* ��ǰ�ַ���ĳ��LABEL�ĵ�һ�ַ������һ���ַ� */
    if((NAS_ESM_APN_VERIFY_CHARACTER_TYPE_BEGIN == enApnVerifyCharacterType) ||
        (NAS_ESM_APN_VERIFY_CHARACTER_TYPE_END == enApnVerifyCharacterType))
    {
        /* �ж��Ƿ�Ϊ�ַ�A-Z����a-z����0-9 */
        if((PS_TRUE == enIsUpperAToZ) || (PS_TRUE == enIsLowerAToZ) ||
            (PS_TRUE == enIsDigit))
        {
            return NAS_ESM_SUCCESS;
        }
        else
        {
            return NAS_ESM_FAILURE;
        }
    }

    /* ��ǰ�ַ���ĳ��LABEL���м��ַ� */
    if(NAS_ESM_APN_VERIFY_CHARACTER_TYPE_MIDDLE == enApnVerifyCharacterType)
    {
        /* �ж��Ƿ�Ϊ�ַ�A-Z����a-z����0-9����- */
        if((PS_TRUE == enIsUpperAToZ) || (PS_TRUE == enIsLowerAToZ) ||
            (PS_TRUE == enIsDigit) || (PS_TRUE == enIsHyphen))
        {
            return NAS_ESM_SUCCESS;
        }
        else
        {
            return NAS_ESM_FAILURE;
        }
    }

    return NAS_ESM_FAILURE;
}



VOS_UINT32  NAS_ESM_VerifyApnCharacter
(
    VOS_UINT32 ulCur,
    const VOS_UINT8* pucApnName,
    const VOS_UINT8  ucApnLen
)
{
    /* �ж���������Ƿ�Ϸ� */
    if((VOS_NULL_PTR == pucApnName) || (ulCur >= ucApnLen))
    {
        NAS_ESM_INFO_LOG("NAS_ESM_VerifyApnCharacter:Error:Input parameter is null!");
        return NAS_ESM_FAILURE;
    }

    /* �жϵ�ǰ�ַ��Ƿ�Ϊ����APN�Ŀ�ʼ�ַ� */
    if(NAS_ESM_NULL == ulCur)
    {
        return NAS_ESM_IsApnCharacterValid(pucApnName[ulCur],
                                           NAS_ESM_APN_VERIFY_CHARACTER_TYPE_BEGIN);
    }

    /* �жϵ�ǰ�ַ��Ƿ�Ϊ����APN�Ľ����ַ� */
    if((ucApnLen - 1) == ulCur)
    {
        return NAS_ESM_IsApnCharacterValid(pucApnName[ulCur],
                                           NAS_ESM_APN_VERIFY_CHARACTER_TYPE_END);
    }

    /* �жϵ�ǰ�ַ��Ƿ�Ϊ�ָ�����ǰһ���ַ� */
    if(NAS_ESM_APN_LABEL_INTERVAL_CHARACTER == pucApnName[ulCur + 1])
    {
        return NAS_ESM_IsApnCharacterValid(pucApnName[ulCur],
                                           NAS_ESM_APN_VERIFY_CHARACTER_TYPE_END);
    }

    /* �жϵ�ǰ�ַ��Ƿ�Ϊ�ָ����ĺ�һ���ַ� */
    if(NAS_ESM_APN_LABEL_INTERVAL_CHARACTER == pucApnName[ulCur - 1])
    {
        return NAS_ESM_IsApnCharacterValid(pucApnName[ulCur],
                                           NAS_ESM_APN_VERIFY_CHARACTER_TYPE_BEGIN);
    }

    /* ��ǰ�ַ�Ϊ�м��ַ� */
    return NAS_ESM_IsApnCharacterValid(pucApnName[ulCur],
                                           NAS_ESM_APN_VERIFY_CHARACTER_TYPE_MIDDLE);
}



VOS_UINT32  NAS_ESM_ApnReverseTransformation
(
    APP_ESM_APN_INFO_STRU  *pstAPNInfoAfterTrans,
    const NAS_ESM_CONTEXT_APN_STRU  *pstAPNInfoBeforeTrans
)
{
    VOS_UINT32                          ulPointIndex            = NAS_ESM_NULL;
    VOS_UINT32                          ulTemp                  =  NAS_ESM_NULL;
    PS_BOOL_ENUM_UINT8                  enIsApnLabelLenValid    = PS_FALSE;
    VOS_UINT32                          ulCur                   = NAS_ESM_NULL;
    APP_ESM_APN_INFO_STRU               stTmpApnInfo;

    /* ��������Ϸ����ж� */
    if((VOS_NULL_PTR == pstAPNInfoBeforeTrans) || (VOS_NULL_PTR == pstAPNInfoAfterTrans)
        || (NAS_ESM_NULL == pstAPNInfoBeforeTrans->ucApnLen)
        || (NAS_ESM_MAX_APN_NUM < pstAPNInfoBeforeTrans->ucApnLen))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ApnReverseTransformation:Error:Input parameter is invalid!");
        return NAS_ESM_FAILURE;
    }

    NAS_ESM_MEM_SET(&stTmpApnInfo, NAS_ESM_NULL, sizeof(APP_ESM_APN_INFO_STRU));

    /*APN NameΪ��*/
    if(APP_APN_NULL_APN_LEN == pstAPNInfoBeforeTrans->ucApnLen)
    {
        stTmpApnInfo.ucApnLen = 0;
        return NAS_ESM_SUCCESS;
    }

    /* ת���󳤶�Ϊת��ǰ���ȼ�1 */
    stTmpApnInfo.ucApnLen = pstAPNInfoBeforeTrans->ucApnLen - 1;

    NAS_ESM_MEM_CPY(    stTmpApnInfo.aucApnName,
                        (pstAPNInfoBeforeTrans->aucApnName + 1),
                        stTmpApnInfo.ucApnLen);

    /* �ж�LABEL�ĳ����Ƿ���Ч */
    /* �ж�APN��һ�����ȣ�����һ��Ϊ0�������APNΪ�� */
    NAS_ESM_IsApnLabelLenValid(pstAPNInfoBeforeTrans->aucApnName[0], enIsApnLabelLenValid);
    if(PS_TRUE != enIsApnLabelLenValid)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ApnReverseTransformation:NAS_ESM_IsApnLabelLenValid fail!");

        NAS_ESM_MEM_SET(pstAPNInfoAfterTrans,
                        0,
                        sizeof(APP_ESM_APN_INFO_STRU));

        return NAS_ESM_FAILURE;
    }
    /* �滻LABEL�ĳ���Ϊ�ָ��ַ�'.' */
    ulPointIndex = pstAPNInfoBeforeTrans->aucApnName[0];
    while(ulPointIndex < stTmpApnInfo.ucApnLen)
    {
        /* �ж�LABEL�ĳ����Ƿ���Ч */
        NAS_ESM_IsApnLabelLenValid(stTmpApnInfo.aucApnName[ulPointIndex], enIsApnLabelLenValid);
        if(PS_TRUE != enIsApnLabelLenValid)
        {
            NAS_ESM_WARN_LOG("NAS_ESM_ApnReverseTransformation:Error:NAS_ESM_IsApnLabelLenValid fail in while loop!");
            /* return NAS_ESM_FAILURE;*/
            stTmpApnInfo.ucApnLen = (unsigned char)ulPointIndex;
            break;

        }

        ulTemp = ulPointIndex;
        ulPointIndex = (ulTemp + stTmpApnInfo.aucApnName[ulTemp]) + 1;
        stTmpApnInfo.aucApnName[ulTemp] = NAS_ESM_APN_LABEL_INTERVAL_CHARACTER;
    }
    /*stTmpApnInfo.ucApnLen = (unsigned char)ulPointIndex;*/
    /* �жϸ�LABEL��֮�ͣ��ټ��ϸ��ָ����ĸ������Ƿ�����ܳ��� */
    /*
    if(ulPointIndex != stTmpApnInfo.ucApnLen)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ApnReverseTransformation:APN length mismatch!");
        return NAS_ESM_FAILURE;
    }
    */

    /* �ж�ÿ���ַ�����Ч�� */
    for(ulCur = 0; ulCur < stTmpApnInfo.ucApnLen; ulCur++)
    {
        /* �жϵ�ǰ�ַ��Ƿ�Ϊ�ָ��ַ� */
        if(NAS_ESM_APN_LABEL_INTERVAL_CHARACTER != stTmpApnInfo.aucApnName[ulCur])
        {
            /* �жϵ�ǰ�ַ��Ƿ���Ч */
            if(NAS_ESM_FAILURE == NAS_ESM_VerifyApnCharacter(ulCur, stTmpApnInfo.aucApnName, stTmpApnInfo.ucApnLen))
            {
                NAS_ESM_WARN_LOG("NAS_ESM_ApnReverseTransformation:NAS_ESM_VerifyApnCharacter fail!");
                return NAS_ESM_FAILURE;
            }
        }
        else
        {
            /* ��һ���ַ������һ���ַ�������Ϊ�ָ��ַ�'.' */
            if((NAS_ESM_NULL == ulCur) || ((stTmpApnInfo.ucApnLen - 1) == ulCur))
            {
                NAS_ESM_WARN_LOG("NAS_ESM_ApnReverseTransformation:separator position error!");
                return NAS_ESM_FAILURE;
            }
        }
    }

    /* ����ת����Ľ����pstAPNInfoAfterTrans */
    NAS_ESM_MEM_CPY(    pstAPNInfoAfterTrans,
                        &stTmpApnInfo,
                        sizeof(APP_ESM_APN_INFO_STRU));

    return NAS_ESM_SUCCESS;
}
VOS_UINT32  NAS_ESM_ApnTransformaton
(
    NAS_ESM_CONTEXT_APN_STRU  *pstAPNInfoAfterTrans,
    const APP_ESM_APN_INFO_STRU  *pstAPNInfoBeforeTrans
)
{
    VOS_UINT32                          ulSum               = NAS_ESM_NULL;
    VOS_UINT32                          ulCur               = NAS_ESM_NULL;
    VOS_UINT32                          ulLabelLenPostion   = NAS_ESM_NULL;
    VOS_INT32                           lLastPointIndex     = -1;

    /* ��������Ϸ����ж� */
    if(APP_MAX_APN_LEN <  pstAPNInfoBeforeTrans->ucApnLen)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ApnTransformaton:Error:Input parameter is invalid!");
        return APP_ERR_SM_APN_LEN_ILLEGAL;
    }

    /*APN����Ϊ��*/
    if(NAS_ESM_NULL == pstAPNInfoBeforeTrans->ucApnLen)
    {
        NAS_ESM_NORM_LOG("NAS_ESM_ApnTransformaton:APN length is 0");
        pstAPNInfoAfterTrans->ucApnLen = 1;
        pstAPNInfoAfterTrans->aucApnName[0] = 0;
        return APP_SUCCESS;
    }

    /* �ж�APN��һ���ַ��Ƿ���Ч */
    if(NAS_ESM_FAILURE == NAS_ESM_VerifyApnCharacter(NAS_ESM_NULL, pstAPNInfoBeforeTrans->aucApnName, pstAPNInfoBeforeTrans->ucApnLen))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ApnTransformaton:Error:APN Character Invalid!");
        return APP_ERR_SM_APN_SYNTACTICAL_ERROR;
    }

    /* �ж�APN���һ���ַ��Ƿ���Ч */
    if(NAS_ESM_FAILURE == NAS_ESM_VerifyApnCharacter((pstAPNInfoBeforeTrans->ucApnLen - 1), pstAPNInfoBeforeTrans->aucApnName, pstAPNInfoBeforeTrans->ucApnLen))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ApnTransformaton:Error:APN Character Invalid!");
        return APP_ERR_SM_APN_SYNTACTICAL_ERROR;
    }

    pstAPNInfoAfterTrans->ucApnLen = pstAPNInfoBeforeTrans->ucApnLen + 1;

    /* ����APN�е�һ��LABEL�ĳ��ȴ��λ�� */
    ulLabelLenPostion = ulSum++;
    for(ulCur = 0; ulCur < pstAPNInfoBeforeTrans->ucApnLen; ulCur++)
    {
        /* �жϵ�ǰ�ַ��Ƿ�Ϊ�ָ��ַ� */
        if(NAS_ESM_APN_LABEL_INTERVAL_CHARACTER != pstAPNInfoBeforeTrans->aucApnName[ulCur])
        {
            /* �жϵ�ǰ�ַ��Ƿ���Ч */
            if(NAS_ESM_FAILURE == NAS_ESM_VerifyApnCharacter(ulCur, pstAPNInfoBeforeTrans->aucApnName, pstAPNInfoBeforeTrans->ucApnLen))
            {
                NAS_ESM_WARN_LOG("NAS_ESM_ApnTransformaton:Error:APN Character Invalid!");
                return APP_ERR_SM_APN_SYNTACTICAL_ERROR;
            }

            /* ��ŵ�ǰ�ַ� */
            pstAPNInfoAfterTrans->aucApnName[ulSum++] = pstAPNInfoBeforeTrans->aucApnName[ulCur];
        }
        else
        {
            /* ����APN�е�ǰLabel�ĳ��� */
            pstAPNInfoAfterTrans->aucApnName[ulLabelLenPostion] = (VOS_UINT8)(((VOS_INT32)ulCur - lLastPointIndex) -1);
            lLastPointIndex = (VOS_INT)ulCur;
            ulLabelLenPostion = ulSum++;
        }
    }
    /* ����APN�����һ��Label�ĳ��� */
    pstAPNInfoAfterTrans->aucApnName[ulLabelLenPostion] = (VOS_UINT8)(((VOS_INT32)ulCur - lLastPointIndex) -1);

    return APP_SUCCESS;
}



VOS_UINT32 NAS_ESM_EncodeAPN( const APP_ESM_APN_INFO_STRU*pstAPNInfo,
                                      VOS_UINT8  *pSendMsg,
                                      VOS_UINT32 *pulLength,
                                      NAS_MSG_FORMAT_ENUM_UINT8  enEncodeTp)
{
    VOS_UINT8       *pTmpSendMsg = pSendMsg;
    VOS_UINT32       ulSum       = *pulLength;
    VOS_UINT32       i           = NAS_ESM_NULL;
    NAS_ESM_CONTEXT_APN_STRU     stTmpApnInfo  = {NAS_ESM_NULL};
    VOS_UINT32                   ulRslt = NAS_ESM_NULL;

    NAS_ESM_INFO_LOG("NAS_ESM_EncodeAPN:INFO:NAS_ESM_EncodeAPN been entered!");


    ulRslt = NAS_ESM_ApnTransformaton(&stTmpApnInfo,
                                         pstAPNInfo);
    if(NAS_ESM_SUCCESS != ulRslt)
    {
        NAS_ESM_ERR_LOG("NAS_ESM_EncodeAPN:ERROR: APN transformation fail!");
        return NAS_ESM_FAILURE;
    }

    if( VOS_NULL_PTR == pTmpSendMsg)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeAPN:ERROR: No SmMsg!");
        return NAS_ESM_FAILURE;
    }

    if(NAS_MSG_FORMAT_TLV == enEncodeTp)
    {
        pTmpSendMsg[ulSum++] = NAS_ESM_MSG_APN_IEI;
    }

    if((NAS_ESM_NULL == stTmpApnInfo.ucApnLen)
        ||(stTmpApnInfo.ucApnLen > NAS_ESM_MAX_APN_NUM))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeAPN:ERROR: APN Length wrong!");
        return NAS_ESM_FAILURE;
    }

    pTmpSendMsg[ulSum++] = stTmpApnInfo.ucApnLen;

    for(i = 0; i < stTmpApnInfo.ucApnLen; i++)
    {
        pTmpSendMsg[ulSum++] = stTmpApnInfo.aucApnName[i];
    }

    *pulLength = ulSum;

    return NAS_ESM_SUCCESS;
}



VOS_VOID NAS_ESM_EpsQosRateTranTo16
(
    VOS_UINT32                          ulRate32,
    VOS_UINT16                         *pusRate16
)
{
    VOS_UINT32                          ulRateTmp = NAS_ESM_NULL;

    *pusRate16 = NAS_ESM_NULL;

    if ((NAS_ESM_NULL == ulRate32))
    {
        return;
    }

    if (ulRate32 < NAS_ESM_QOS_RATE_64KB)
    {
        *pusRate16 = (VOS_UINT16)(VOS_UINT8)ulRate32;
        return;
    }

    /* ulRate32ȡֵ64-568kb��pusRate16��8λ: ȡֵλ��01000000 - 01111111 */
    if (ulRate32 <= NAS_ESM_QOS_RATE_568KB)
    {

        ulRateTmp = (VOS_UINT32)((ulRate32 - NAS_ESM_QOS_RATE_64KB)
                                        / NAS_ESM_QOS_RATE_GRANULARITY_8KB);
        ulRateTmp += (1 << NAS_ESM_MOVEMENT_6_BITS);
        *pusRate16 = (VOS_UINT16)(VOS_UINT8)ulRateTmp;
        return;
    }

    /* ulRate32ȡֵ569-575kb, pusRate16��8λ:0x7F */
    if (ulRate32 < NAS_ESM_QOS_RATE_576KB)
    {
        *pusRate16 = NAS_ESM_QOS_RATE_RANGE_568KB_GRANULARITY_8KB;
        return;
    }

    /* ulRate32ȡֵ576-8640kb, pusRate16��8λ: ȡֵλ��10000000 - 11111110 */
    if (ulRate32 <= NAS_ESM_QOS_RATE_8640KB)
    {
        ulRateTmp = (VOS_UINT32)((ulRate32 - NAS_ESM_QOS_RATE_576KB)
                                        / NAS_ESM_QOS_RATE_GRANULARITY_64KB);
        ulRateTmp += (1 << NAS_ESM_MOVEMENT_7_BITS);
        *pusRate16 = (VOS_UINT16)(VOS_UINT8)ulRateTmp;
        return;
    }

    /* ulRate32ȡֵ8641-8699kb, pusRate16��8λ:0xFE */
    if (ulRate32 < NAS_ESM_QOS_RATE_8700KB)
    {
        *pusRate16 = NAS_ESM_QOS_RATE_RANGE_8640KB_GRANULARITY_64KB;
        return;
    }

    if (ulRate32 <= NAS_ESM_QOS_RATE_16000KB)
    {
        ulRateTmp = (VOS_UINT32)((ulRate32 - NAS_ESM_QOS_RATE_8600KB)
                                        / NAS_ESM_QOS_RATE_GRANULARITY_100KB);
        *pusRate16 = (((VOS_UINT16)(VOS_UINT8)ulRateTmp) << NAS_ESM_MOVEMENT_8_BITS)
                      | NAS_ESM_QOS_RATE_RANGE_8640KB_GRANULARITY_64KB;
        return;
    }

    if (ulRate32 < NAS_ESM_QOS_RATE_17MB)
    {
        *pusRate16 = NAS_ESM_QOS_RATE_RANGE_16000KB_GRANULARITY_64KB;
        *pusRate16 = ((VOS_UINT16)(*pusRate16 << NAS_ESM_MOVEMENT_8_BITS))
                      | NAS_ESM_QOS_RATE_RANGE_8640KB_GRANULARITY_64KB;
        return;
    }

    /* ulRate32ȡֵ17-128Mb, pusRate16��8λ: ȡֵλ��01001011 - 10111010 */
    if (ulRate32 <= NAS_ESM_QOS_RATE_128MB)
    {
        ulRateTmp = (VOS_UINT32)((ulRate32 - NAS_ESM_QOS_RATE_16MB)
                                    / NAS_ESM_QOS_RATE_GRANULARITY_1MB) + NAS_ESM_QOS_RATE_RANGE_16000KB_GRANULARITY_64KB;
        *pusRate16 = (((VOS_UINT16)(VOS_UINT8)ulRateTmp) << NAS_ESM_MOVEMENT_8_BITS)
                      | NAS_ESM_QOS_RATE_RANGE_8640KB_GRANULARITY_64KB;
        return;
    }

    /* ulRate32ȡֵ128-130Mb, pusRate16��8λ:0xFE */
    if (ulRate32 < NAS_ESM_QOS_RATE_130MB)
    {
        *pusRate16 = NAS_ESM_QOS_RATE_RANGE_128MB_GRANULARITY_1MB;
        *pusRate16 = ((VOS_UINT16)(*pusRate16 << NAS_ESM_MOVEMENT_8_BITS))
                     | NAS_ESM_QOS_RATE_RANGE_8640KB_GRANULARITY_64KB;
        return;
    }

    /* ulRate32ȡֵ130-256Mb, pusRate16��8λ: ȡֵλ��10111011 - 11111010 */
    if (ulRate32 <= NAS_ESM_QOS_RATE_256MB)
    {
        ulRateTmp = (VOS_UINT32)((ulRate32 - NAS_ESM_QOS_RATE_128MB)
                                    / NAS_ESM_QOS_RATE_GRANULARITY_2MB) + NAS_ESM_QOS_RATE_RANGE_128MB_GRANULARITY_1MB;
        *pusRate16 = (((VOS_UINT16)(VOS_UINT8)ulRateTmp) << NAS_ESM_MOVEMENT_8_BITS)
                     | NAS_ESM_QOS_RATE_RANGE_8640KB_GRANULARITY_64KB;;
        return;
    }

    /* ulRate32ȡֵ����256Mb, pusRate16��8λ: 0xFA */
    *pusRate16 = NAS_ESM_QOS_RATE_RANGE_256MB_GRANULARITY_2MB;
    *pusRate16 = ((VOS_UINT16)(*pusRate16 << NAS_ESM_MOVEMENT_8_BITS))
                 | NAS_ESM_QOS_RATE_RANGE_8640KB_GRANULARITY_64KB;
}


VOS_UINT32 NAS_ESM_EncodeEpsQos
(
    NAS_ESM_CONTEXT_LTE_QOS_STRU       *pstEpsQosInfo,
    VOS_UINT8                          *pSendMsg,
    VOS_UINT32                         *pulLength,
    NAS_MSG_FORMAT_ENUM_UINT8           enEncodeTp
)
{
    VOS_UINT32                          ulSum               = *pulLength;
    VOS_UINT32                          ulTmpSum            = NAS_ESM_NULL;
    VOS_UINT32                          ulLength            = NAS_ESM_NULL;
    VOS_UINT16                          usULMaxRateTmp      = NAS_ESM_NULL;
    VOS_UINT16                          usDLMaxRateTmp      = NAS_ESM_NULL;
    VOS_UINT16                          usULGMaxRateTmp     = NAS_ESM_NULL;
    VOS_UINT16                          usDLGMaxRateTmp     = NAS_ESM_NULL;
    VOS_UINT8                          *pTmpSendMsg         = pSendMsg;
    APP_ESM_EPS_QOS_INFO_STRU          *pstTmpEpsQosInfo    = VOS_NULL_PTR;

    NAS_ESM_INFO_LOG("NAS_ESM_EncodeEpsQos:INFO:NAS_ESM_EncodeEpsQos been entered!");

    if((VOS_NULL_PTR == pTmpSendMsg) || (VOS_NULL_PTR == pstEpsQosInfo))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeEpsQos:ERROR: No SmMsg or No Qos Info!");
        return NAS_ESM_FAILURE;
    }

    pstTmpEpsQosInfo = &pstEpsQosInfo->stQosInfo;

    /*  ����ǰ��QOS���кϷ��Լ�� */
    if (NAS_ESM_FAILURE == NAS_ESM_IsSdfQosValid(pstTmpEpsQosInfo))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeEpsQos:ERROR: Check qos failed!");
        return NAS_ESM_FAILURE;
    }
    if(NAS_MSG_FORMAT_TLV == enEncodeTp)
    {
        /*��ѡ����,���EPS_QOS��ϢIEI����*/
        pTmpSendMsg[ulSum++] = NAS_ESM_MSG_EPS_QOS_IEI;
    }

    /*��¼SDF QOS����λ����*/
    ulTmpSum = ulSum++;

    pTmpSendMsg[ulSum++] = pstEpsQosInfo->ucNwQCI;

    if (PS_TRUE == NAS_ESM_IsGbrBearer(pstTmpEpsQosInfo->ucQCI))
    {
        /* ת��ULMaxRate */
        NAS_ESM_EpsQosRateTranTo16(pstTmpEpsQosInfo->ulULMaxRate, &usULMaxRateTmp);

        /* ת��DLMaxRate */
        NAS_ESM_EpsQosRateTranTo16(pstTmpEpsQosInfo->ulDLMaxRate, &usDLMaxRateTmp);

        /* ת��ULGMaxRate */
        NAS_ESM_EpsQosRateTranTo16(pstTmpEpsQosInfo->ulULGMaxRate, &usULGMaxRateTmp);

        /* ת��DLGMaxRate */
        NAS_ESM_EpsQosRateTranTo16(pstTmpEpsQosInfo->ulDLGMaxRate, &usDLGMaxRateTmp);

        pTmpSendMsg[ulSum++] = (VOS_UINT8)(usULMaxRateTmp & NAS_ESM_LOW_BYTE);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)(usDLMaxRateTmp & NAS_ESM_LOW_BYTE);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)(usULGMaxRateTmp & NAS_ESM_LOW_BYTE);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)(usDLGMaxRateTmp & NAS_ESM_LOW_BYTE);

        pTmpSendMsg[ulSum++] = (VOS_UINT8)((usULMaxRateTmp & NAS_ESM_HIGH_BYTE)  \
                                                    >> NAS_ESM_MOVEMENT_8_BITS);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)((usDLMaxRateTmp & NAS_ESM_HIGH_BYTE)  \
                                                    >> NAS_ESM_MOVEMENT_8_BITS);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)((usULGMaxRateTmp & NAS_ESM_HIGH_BYTE)  \
                                                    >> NAS_ESM_MOVEMENT_8_BITS);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)((usDLGMaxRateTmp & NAS_ESM_HIGH_BYTE)  \
                                                    >> NAS_ESM_MOVEMENT_8_BITS);
    }


    ulLength = ulSum - (ulTmpSum + 1);

    pTmpSendMsg[ulTmpSum] = (VOS_UINT8)ulLength;

    *pulLength = ulSum;

    return NAS_ESM_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_EncodeTftNoOperation
 Description     : �����޸�SDF��QOS��Ϣ��TFT����
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2009-05-18  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_EncodeTftNoOperation
(
    const VOS_UINT8                    *pucNwPfId,
    VOS_UINT32                          ulPFNum,
    VOS_UINT8                          *pSendMsg,
    VOS_UINT32                         *pulLength
)
{
    VOS_UINT8       *pTmpSendMsg = pSendMsg;
    VOS_UINT32       ulSum       = *pulLength;
    VOS_UINT32       ulTmpSum    = NAS_ESM_NULL;
    VOS_UINT32       ulCnt       = NAS_ESM_NULL;

    NAS_ESM_INFO_LOG("NAS_ESM_EncodeTftNoOperation:INFO:NAS_ESM_EncodeTftNoOperation been entered!");

    ulTmpSum = ulSum++;

    /*����TFT��3�ֽڵ����ݣ�������һ����filter����*/
    pTmpSendMsg[ulSum++] = NAS_ESM_TFT_NO_OPERATION;

    /* ����һ��Parameter */
    pTmpSendMsg[ulSum++] = NAS_ESM_TFT_PARA_LIST_ID;

    pTmpSendMsg[ulSum++] = ( VOS_UINT8 )ulPFNum;

    for ( ulCnt=0; ulCnt < ulPFNum; ulCnt++ )
    {
        pTmpSendMsg[ulSum++] = pucNwPfId[ulCnt];
    }

    /*��¼TFT��Ϣ���ݵĳ���,Ϊ����TFT��һ����Ϣ��Ԫ��ulSum��+��1*/
    pTmpSendMsg[ulTmpSum] = (VOS_UINT8)(ulSum - (ulTmpSum + 1));

    *pulLength = ulSum;
}

/*****************************************************************************
 Function Name   : NAS_ESM_EncodeOneFilterContent
 Description     : ����һ��packet filter����
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.sunbing49683      2009-4-21  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_EncodeOneFilterContent
(
    NAS_ESM_CONTEXT_TFT_STRU           *pstTFTInfo,
    VOS_UINT8                          *pSendMsg,
    VOS_UINT32                         *pulLength
)
{
    VOS_UINT8       *pTmpSendMsg = pSendMsg;
    VOS_UINT32       ulSum       = *pulLength;
    VOS_UINT32       ulIndex     = NAS_ESM_NULL;
    NAS_ESM_CONTEXT_TFT_STRU    *pstTmpTFTInfo = pstTFTInfo;

    NAS_ESM_INFO_LOG("NAS_ESM_EncodeOneFilterContent:INFO:NAS_ESM_EncodeOneFilterContent been entered!");

    /*Ŀǰֻ��һ��filter,������ʾ*/
    /* OCTET4 Packet filter identifier 1 */

    pTmpSendMsg[ulSum++] = pstTmpTFTInfo->ucNwPacketFilterId
                          |((pstTmpTFTInfo->enDirection & NAS_ESM_LOW_2_BIT_MASK)
                         << NAS_ESM_MOVEMENT_4_BITS);

    /* OCTET5 ,Packet filter evaluation precedence 1 */
    pTmpSendMsg[ulSum++] = pstTmpTFTInfo->ucPrecedence;
    /* ����Length of Pactket fileter contents1*/
    ulIndex = ulSum++;

    if(NAS_ESM_OP_TRUE == pstTmpTFTInfo->bitOpRmtIpv4AddrAndMask)
    {
        pTmpSendMsg[ulSum++] = NAS_ESM_TFT_IPV4_ADDR;
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv4Address[0];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv4Address[1];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv4Address[2];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv4Address[3];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv4Mask[0];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv4Mask[1];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv4Mask[2];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv4Mask[3];
    }

    if (NAS_ESM_OP_TRUE == pstTmpTFTInfo->bitOpRmtIpv6AddrAndMask)
    {
        pTmpSendMsg[ulSum++] = NAS_ESM_TFT_IPV6_ADDR;
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Address[0];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Address[1];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Address[2];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Address[3];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Address[4];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Address[5];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Address[6];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Address[7];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Address[8];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Address[9];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Address[10];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Address[11];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Address[12];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Address[13];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Address[14];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Address[15];

        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Mask[0];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Mask[1];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Mask[2];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Mask[3];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Mask[4];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Mask[5];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Mask[6];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Mask[7];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Mask[8];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Mask[9];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Mask[10];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Mask[11];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Mask[12];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Mask[13];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Mask[14];
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->aucRmtIpv6Mask[15];
    }
    if(NAS_ESM_OP_TRUE == pstTmpTFTInfo->bitOpProtocolId)
    {
        pTmpSendMsg[ulSum++] = NAS_ESM_TFT_PROTOCOL_ID;
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->ucProtocolId;
    }

    if(NAS_ESM_OP_TRUE == pstTmpTFTInfo->bitOpSingleLocalPort)
    {
        pTmpSendMsg[ulSum++] = NAS_ESM_TFT_SINGLE_LOCAL_PORT;
        pTmpSendMsg[ulSum++] = (VOS_UINT8)((pstTmpTFTInfo->usSingleLcPort & NAS_ESM_HIGH_BYTE)
                                      >> NAS_ESM_MOVEMENT_8_BITS);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)(pstTmpTFTInfo->usSingleLcPort & NAS_ESM_LOW_BYTE);
    }

    if(NAS_ESM_OP_TRUE == pstTmpTFTInfo->bitOpLocalPortRange)
    {
        pTmpSendMsg[ulSum++] = NAS_ESM_TFT_LOCAL_PORT_RANG;
        pTmpSendMsg[ulSum++] = (VOS_UINT8)((pstTmpTFTInfo->usLcPortLowLimit & NAS_ESM_HIGH_BYTE)
                                      >> NAS_ESM_MOVEMENT_8_BITS);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)(pstTmpTFTInfo->usLcPortLowLimit & NAS_ESM_LOW_BYTE);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)((pstTmpTFTInfo->usLcPortHighLimit & NAS_ESM_HIGH_BYTE)
                                      >> NAS_ESM_MOVEMENT_8_BITS);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)(pstTmpTFTInfo->usLcPortHighLimit & NAS_ESM_LOW_BYTE);
    }

    if(NAS_ESM_OP_TRUE == pstTmpTFTInfo->bitOpSingleRemotePort)
    {
        pTmpSendMsg[ulSum++] = NAS_ESM_TFT_SINGLE_REMOTE_PORT;
        pTmpSendMsg[ulSum++] = (VOS_UINT8)((pstTmpTFTInfo->usSingleRmtPort & NAS_ESM_HIGH_BYTE)
                                      >> NAS_ESM_MOVEMENT_8_BITS);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)(pstTmpTFTInfo->usSingleRmtPort & NAS_ESM_LOW_BYTE);
    }

    if(NAS_ESM_OP_TRUE == pstTmpTFTInfo->bitOpRemotePortRange)
    {
        pTmpSendMsg[ulSum++] = NAS_ESM_TFT_REMOTE_PORT_RANG;
        pTmpSendMsg[ulSum++] = (VOS_UINT8)((pstTmpTFTInfo->usRmtPortLowLimit & NAS_ESM_HIGH_BYTE)
                                      >> NAS_ESM_MOVEMENT_8_BITS);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)(pstTmpTFTInfo->usRmtPortLowLimit & NAS_ESM_LOW_BYTE);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)((pstTmpTFTInfo->usRmtPortHighLimit & NAS_ESM_HIGH_BYTE)
                                      >> NAS_ESM_MOVEMENT_8_BITS);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)(pstTmpTFTInfo->usRmtPortHighLimit & NAS_ESM_LOW_BYTE);
    }

    if(NAS_ESM_OP_TRUE == pstTmpTFTInfo->bitOpSecuParaIndex)
    {
        pTmpSendMsg[ulSum++] = NAS_ESM_TFT_SPI;
        pTmpSendMsg[ulSum++] = (VOS_UINT8)((pstTmpTFTInfo->ulSecuParaIndex & NAS_ESM_FOURTH_BYTE)
                                      >> NAS_ESM_MOVEMENT_24_BITS);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)((pstTmpTFTInfo->ulSecuParaIndex & NAS_ESM_THIRD_BYTE)
                                      >> NAS_ESM_MOVEMENT_16_BITS);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)((pstTmpTFTInfo->ulSecuParaIndex & NAS_ESM_HIGH_BYTE)
                                      >> NAS_ESM_MOVEMENT_8_BITS);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)(pstTmpTFTInfo->ulSecuParaIndex & NAS_ESM_LOW_BYTE);
    }

    if(NAS_ESM_OP_TRUE == pstTmpTFTInfo->bitOpTypeOfService)
    {
        pTmpSendMsg[ulSum++] = NAS_ESM_TFT_TOS;
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->ucTypeOfService;
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->ucTypeOfServiceMask;
    }

    if(NAS_ESM_OP_TRUE == pstTmpTFTInfo->bitOpFlowLabelType)
    {
        pTmpSendMsg[ulSum++] = NAS_ESM_TFT_FLOW_LABLE;
        pTmpSendMsg[ulSum++] = (VOS_UINT8)(((pstTmpTFTInfo->ulFlowLabelType & NAS_ESM_THIRD_BYTE)
                                      >> NAS_ESM_MOVEMENT_16_BITS) & NAS_ESM_MSG_LOW_BIT);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)((pstTmpTFTInfo->ulFlowLabelType & NAS_ESM_HIGH_BYTE)
                                      >> NAS_ESM_MOVEMENT_8_BITS);
        pTmpSendMsg[ulSum++] = (VOS_UINT8)(pstTmpTFTInfo->ulFlowLabelType & NAS_ESM_LOW_BYTE);
    }

    /*��¼tft packet filter contents����*/
    pTmpSendMsg[ulIndex] = (VOS_UINT8)(ulSum - (ulIndex + 1));

    *pulLength = ulSum;
}
VOS_VOID  NAS_ESM_EncodeTftDeletePf
(
    NAS_ESM_CONTEXT_TFT_STRU            *pstTFTInfo,
    VOS_UINT32                           ulPFNum,
    VOS_UINT8                           *pSendMsg,
    VOS_UINT32                          *pulLength
)
{
    VOS_UINT8                           *pTmpSendMsg   = pSendMsg;
    VOS_UINT32                           ulSum         = *pulLength;
    VOS_UINT32                           ulTmpSum      = NAS_ESM_NULL;
    VOS_UINT32                           ulCnt         = NAS_ESM_NULL;
    NAS_ESM_CONTEXT_TFT_STRU            *pstTmpTFTInfo = pstTFTInfo;

    NAS_ESM_INFO_LOG("NAS_ESM_EncodeTftDeletePf:INFO:NAS_ESM_EncodeDeleteFilterTTF been entered!");

    ulTmpSum = ulSum++;

    /*����TFT��3�ֽڵ����ݣ�������һ����filter����*/
    pTmpSendMsg[ulSum++] =(NAS_ESM_TFT_DELETE_FILTER
                          |((VOS_UINT8)ulPFNum
                             & NAS_ESM_TFT_LOW_4_BIT_MASK));

    /*Ŀǰֻ��һ��filter,������ʾ*/
    /* OCTET4 Packet filter identifier 1 */
    for (ulCnt = NAS_ESM_NULL; ulCnt < ulPFNum; ulCnt++)
    {
        pTmpSendMsg[ulSum++] = pstTmpTFTInfo->ucNwPacketFilterId;
        pstTmpTFTInfo++;
    }

    /*��¼TFT��Ϣ���ݵĳ���,Ϊ����TFT��һ����Ϣ��Ԫ��ulSum��+��1*/
    pTmpSendMsg[ulTmpSum] = (VOS_UINT8)(ulSum - (ulTmpSum + 1));

    *pulLength = ulSum;
}


VOS_UINT32 NAS_ESM_IsSdfQueryNwPfIdSucc
(
    const NAS_ESM_RES_MOD_ENCODE_INFO_STRU    *pstResModEncodeInfo
)
{

    VOS_UINT32                          ulCnt               = NAS_ESM_NULL;
    VOS_UINT32                          ulRslt              = NAS_ESM_FAILURE;
    VOS_UINT8                           ucNwPacketFilterId  = NAS_ESM_NULL;
    NAS_ESM_SDF_PARA_STRU              *pstSdfPara          = VOS_NULL_PTR;


    pstSdfPara = NAS_ESM_GetSdfParaAddr(pstResModEncodeInfo->ulCid);

    for (ulCnt = NAS_ESM_NULL; ulCnt < pstSdfPara->ulPfNum; ulCnt++)
    {
        ulRslt = NAS_ESM_QueryNWPacketFilterID( pstSdfPara->astCntxtTftInfo[ulCnt].ucPacketFilterId,
                                                pstResModEncodeInfo->ulCid,
                                                &ucNwPacketFilterId);

        if (NAS_ESM_FAILURE == ulRslt)
        {
            NAS_ESM_WARN_LOG("NAS_ESM_IsSdfQueryNwPfIdSuc:WARNING:NAS->ESM no network TFT matched!");

            return NAS_ESM_FAILURE;
        }
    }

    return NAS_ESM_SUCCESS;
}

/*****************************************************************************
 Function Name   : NAS_ESM_EncodeTftCreateTftOrReplacePf
 Description     : ����TFT��Ϣ,TAD��ΪCREATE_TFT����REPLACE_FILTER
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2010-3-9  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_ESM_EncodeTftCreateTftOrReplacePf
(
    VOS_UINT8                           ucTadType,
    VOS_UINT32                          ulCid,
    VOS_UINT8                          *pSendMsg,
    VOS_UINT32                         *pulLength
)
{
    VOS_UINT32                          ulIndex             = NAS_ESM_NULL;
    VOS_UINT32                          ulSum               = NAS_ESM_NULL;
    VOS_UINT32                          ulTmpSum            = NAS_ESM_NULL;
    VOS_UINT32                          ulCnt               = NAS_ESM_NULL;
    NAS_ESM_CONTEXT_TFT_STRU            stTFTInfo;
    VOS_UINT8                          *pTmpSendMsg         = pSendMsg;
    NAS_ESM_SDF_PARA_STRU              *pstSdfPara          = VOS_NULL_PTR;

    NAS_ESM_INFO_LOG("NAS_ESM_EncodeTftCreateTftOrReplacePf:INFO:NAS_ESM_EncodeTftCreateTftOrReplacePf been entered!");

    if( VOS_NULL_PTR == pTmpSendMsg)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_EncodeTftCreateTftOrReplacePf:ERROR: No SmMsg!");
        return NAS_ESM_FAILURE;
    }

    pstSdfPara = NAS_ESM_GetSdfParaAddr(ulCid);

    /* ��¼�洢TFT���ȵ�λ�� */
    ulTmpSum = ulSum++;

    if (ucTadType == NAS_ESM_TFT_CREATE_TFT)
    {
        /*����TFT��3�ֽڵ����ݣ������������filter����*/
        pTmpSendMsg[ulSum++] =(NAS_ESM_TFT_CREATE_TFT
                               |((VOS_UINT8)pstSdfPara->ulPfNum
                                  & NAS_ESM_TFT_LOW_4_BIT_MASK));
    }
    else
    {
        /*����TFT��3�ֽڵ����ݣ������������filter����*/
        pTmpSendMsg[ulSum++] =(NAS_ESM_TFT_REPLACE_FILTER
                               |((VOS_UINT8)pstSdfPara->ulPfNum
                                  & NAS_ESM_TFT_LOW_4_BIT_MASK));
    }

    /* add 1,the  byte length of TFT operation code locate in */
    pTmpSendMsg[ulTmpSum]++;

    for (ulCnt = NAS_ESM_NULL; ulCnt < pstSdfPara->ulPfNum; ulCnt++)
    {
        NAS_ESM_MEM_CPY(&stTFTInfo,
                        &pstSdfPara->astCntxtTftInfo[ulCnt],
                        sizeof(NAS_ESM_CONTEXT_TFT_STRU));

        if (ucTadType == NAS_ESM_TFT_CREATE_TFT)
        {
            stTFTInfo.ucNwPacketFilterId = NAS_ESM_NULL;
        }
        else
        {
            (VOS_VOID)NAS_ESM_QueryNWPacketFilterID(stTFTInfo.ucPacketFilterId,
                                                    ulCid,
                                                    &stTFTInfo.ucNwPacketFilterId);
        }

        NAS_ESM_EncodeOneFilterContent(&stTFTInfo,
                                        pTmpSendMsg + ulSum,
                                        &ulIndex);

        /* �ж�TFT�ĳ����Ƿ񳬳����������ķ�Χ */
        if ((ulIndex + pTmpSendMsg[ulTmpSum]) > NAS_ESM_MAX_TFT_BYTE)
        {
            NAS_ESM_WARN_LOG("NAS_ESM_EncodeTftCreateTftOrReplacePf:ERROR:TFT length is longer than 255!");
            return NAS_ESM_FAILURE;
        }

        pTmpSendMsg[ulTmpSum] += (VOS_UINT8)ulIndex;

        /*��¼����*/
        ulSum = ulSum + ulIndex;

        /*ulIndex����*/
        ulIndex = NAS_ESM_NULL;
    }

    *pulLength = ulSum;

    return NAS_ESM_SUCCESS;
}
/*lint +e961*/
/*lint +e960*/

VOS_UINT32 NAS_ESM_VerifyAppAuthParam_PPP( const APP_ESM_AUTH_INFO_STRU *pstAuthInfo )
{
    if (APP_ESM_AUTH_TYPE_PAP == pstAuthInfo->enAuthType)
    {
        if ((APP_ESM_PPP_MAX_USERNAME_LEN < pstAuthInfo->unAuthParam.stPapParam.ucUsernameLen)
             || (APP_ESM_PPP_MAX_PASSWORD_LEN < pstAuthInfo->unAuthParam.stPapParam.ucPasswordLen))
        {
             NAS_ESM_NORM_LOG("NAS_ESM_VerifyAppAuthParam_PPP:PAP Input gw auth len is err!");
             return VOS_FALSE;
        }
    }

    if (APP_ESM_AUTH_TYPE_CHAP == pstAuthInfo->enAuthType)
    {
        if ((APP_ESM_PPP_CHAP_CHALLNGE_LEN != pstAuthInfo->unAuthParam.stChapParam.ulChallengeLen)
             || (APP_ESM_PPP_CHAP_RESPONSE_LEN != pstAuthInfo->unAuthParam.stChapParam.ulResponseLen)
             || (APP_ESM_PPP_MAX_USERNAME_LEN < pstAuthInfo->unAuthParam.stChapParam.ucResponseNameLen)
             || (APP_ESM_PPP_CHAP_CHALLNGE_NAME_LEN < pstAuthInfo->unAuthParam.stChapParam.ulChallengeNameLen))
        {

             NAS_ESM_NORM_LOG("NAS_ESM_VerifyAppAuthParam_PPP:Input CHAP gw auth len is err!");
             return VOS_FALSE;
        }
    }
    return VOS_TRUE;
}

#ifdef __cplusplus
    #if __cplusplus
                                                }
    #endif
#endif

