


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include    "NasEsmInclude.h"
#include    "NasEsmIpMsgProc.h"
/*#include    "IpDhcpv4Server.h"*/
#include    "LRabmPdcpInterface.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASESMNWMSGPDNCONPROC_C
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
/*extern VOS_UINT32  APPITF_SendRequestArp(const VOS_UINT8* pucReqIPAddr);
extern VOS_UINT32 APPITF_ConfigBoardIPAddr(const VOS_UINT8* pucCfgIP);*/



/*lint -e960*/
/*lint -e961*/
VOS_VOID NAS_ESM_ProcNwMsgEsmCause54
(
    VOS_UINT32                          ulStateTblIndex,
    const EMM_ESM_INTRA_DATA_IND_STRU  *pRcvMsg
)
{

    NAS_ESM_ENCODE_INFO_STRU            stEncodeInfo;
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr    = VOS_NULL_PTR;

    (VOS_VOID)pRcvMsg;
    /*��ӡ����ú���*/
    NAS_ESM_NORM_LOG("NAS_ESM_ProcNwMsgEsmCause54 is entered.");

    pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

    /* �Ѿ�ע�ᣬ��APP���Ŵ���PDN��������ĳ��������ظ�APP���ֱ�����·���PDN��������
       ������������Ϊinitial request */
    if((NAS_ESM_PS_REGISTER_STATUS_ATTACHED == NAS_ESM_GetEmmStatus())
        &&(ID_APP_ESM_NDISCONN_REQ == pstStateAddr->stAppMsgRecord.enAppMsgType))
    {
        /* �޸�״̬���м�¼��PDN�������ͣ����·���PDN������������֮ǰ��״̬��δ�ͷţ�
           �������±��뼴�� */
        pstStateAddr->enPdnReqType = NAS_ESM_PDN_REQ_TYPE_INITIAL;

        /*���NAS_ESM_ENCODE_INFO_STRU*/
        NAS_ESM_MEM_SET((VOS_VOID*)&stEncodeInfo, 0, sizeof(NAS_ESM_ENCODE_INFO_STRU));
        stEncodeInfo.bitOpESMCau        = NAS_ESM_OP_FALSE;
        stEncodeInfo.ulCid              = pstStateAddr->ulCid;
        stEncodeInfo.ucPti              = pstStateAddr->stNwMsgRecord.ucPti;
        stEncodeInfo.ulEpsbId           = pstStateAddr->ulEpsbId;
        stEncodeInfo.enPdnReqType       = pstStateAddr->enPdnReqType;

        /*��װPDN_Connect_Req��Ϣ*/
        if(NAS_ESM_FAILURE == NAS_ESM_EncodePdnConReqMsg(stEncodeInfo, \
                                                         pstStateAddr->stNwMsgRecord.aucMsgBuff,\
                                                         &pstStateAddr->stNwMsgRecord.ulMsgLength))
        {
            NAS_ESM_WARN_LOG("NAS_ESM_ProcNwMsgEsmCause54:WARNING:EncodePdnConReqMsg fail!");

            /*��APP�ظ�APP_ESM_BEARER_SETUP_CNF(ʧ��)��Ϣ*/
            NAS_ESM_SndEsmAppNdisConnCnfFailMsg(ulStateTblIndex, APP_ERR_UNSPECIFIED_ERROR);
            NAS_ESM_RelStateTblResource(ulStateTblIndex);
            return ;
        }

        /*���ú�����SM->EMM������Ϣ:ID_EMM_ESM_DATA_REQ(NAS_ESMCN_MSG_TYPE_PDN_CONNECT_REQ)*/
        NAS_ESM_SndEsmEmmDataReqMsg(pstStateAddr->stNwMsgRecord.ulOpId,
                                    pstStateAddr->stNwMsgRecord.ucIsEmcPdnReq,
                                    pstStateAddr->stNwMsgRecord.ulMsgLength,
                                    pstStateAddr->stNwMsgRecord.aucMsgBuff);

        /* ��¼�տ���Ϣ */
        NAS_ESM_SndAirMsgReportInd(pstStateAddr->stNwMsgRecord.aucMsgBuff,
                                   pstStateAddr->stNwMsgRecord.ulMsgLength,
                                   NAS_ESM_AIR_MSG_DIR_ENUM_UP,
                                   ESM_PDN_CONNECT_REQ);

        /*����T3482��ʱ��*/
        NAS_ESM_TimerStart(ulStateTblIndex,TI_NAS_ESM_T3482);

        /*״̬ת��*/
        NAS_ESM_SetProcTransState(ulStateTblIndex,NAS_ESM_BEARER_PROC_TRANS_STATE_PENDING);
        return;

    }

    return;

}

/*****************************************************************************
 Function Name   : NAS_ESM_ProcValidNwMsgPdnConnRej
 Description     : ����PDN���Ӿܾ�������Ϣ
 Input           : ulStateTblIndex -- ��̬������
                   pRcvMsg         -- �յ��Ŀտ���Ϣ
 Output          : None
 Return          : VOS_VOID

 History         :
    1.sunbing49683      2009-4-16   Draft Enact
    2.lihong00150010    2010-08-12  Modify
    3.liuwenyu00143951  2010-11-11  Modify
*****************************************************************************/
/*lint -e960*/
/*lint -e961*/
VOS_VOID NAS_ESM_ProcValidNwMsgPdnConnRej
(
    VOS_UINT32                          ulStateTblIndex,
    const EMM_ESM_INTRA_DATA_IND_STRU  *pRcvMsg
)
{
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr    = VOS_NULL_PTR;

    /*��ӡ����ú���*/
    NAS_ESM_NORM_LOG("NAS_ESM_ProcValidNwMsgPdnConnRej is entered.");

    pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

    /* ����������handover��PDN���������յ�ԭ��ֵ54 */
    if((NAS_ESM_OP_TRUE == pRcvMsg->stEsmNwMsgIE.bitOpEsmCause)
        &&(NAS_ESM_CAUSE_PDN_CONNECTION_DOES_NOT_EXIST == pRcvMsg->stEsmNwMsgIE.enEsmCau)
        &&(NAS_ESM_PDN_REQ_TYPE_HANDOVER == pstStateAddr->enPdnReqType))
    {
        NAS_ESM_ProcNwMsgEsmCause54(ulStateTblIndex, pRcvMsg);
        return;
    }

    /*��APP�ظ���Ϣ*/
    if(ID_APP_ESM_PDP_SETUP_REQ == pstStateAddr->stAppMsgRecord.enAppMsgType )
    {
        /*��APP����ID_APP_ESM_PDP_SETUP_CNF(ʧ��)��Ϣ*/
        NAS_ESM_SndEsmAppSdfSetupCnfFailMsg(ulStateTblIndex, \
                  NAS_ESM_GetAppErrorByEsmCause(pRcvMsg->stEsmNwMsgIE.enEsmCau));
    }
    /*��APP�ظ���Ϣ*/
    else if(ID_APP_ESM_NDISCONN_REQ == pstStateAddr->stAppMsgRecord.enAppMsgType )
    {
        /*��APP����ID_APP_ESM_NDISCONN_CNF(ʧ��)��Ϣ*/
        NAS_ESM_LOG1("NAS_ESM_ProcValidNwMsgPdnConnRej:ERROR =", NAS_ESM_GetAppErrorByEsmCause(pRcvMsg->stEsmNwMsgIE.enEsmCau));
        NAS_ESM_SndEsmAppNdisConnCnfFailMsg(ulStateTblIndex, \
                                      NAS_ESM_GetAppErrorByEsmCause(pRcvMsg->stEsmNwMsgIE.enEsmCau));
    }
    else
    {
        /*��APP����ID_APP_ESM_PDP_SETUP_IND(ʧ��)��Ϣ*/
        NAS_ESM_SndEsmAppSdfSetupIndMsg(ulStateTblIndex, \
                  NAS_ESM_GetAppErrorByEsmCause(pRcvMsg->stEsmNwMsgIE.enEsmCau));
    }

    /*�ͷ�״̬����Դ*/
    NAS_ESM_RelStateTblResource(ulStateTblIndex);

}



VOS_VOID  NAS_ESM_ProcValidNwMsgActDefltEpsbReq
(
    VOS_UINT32                                  ulStateTblIndex,
    const EMM_ESM_INTRA_DATA_IND_STRU          *pRcvMsg
)
{
    VOS_UINT32                          ulEpsbId         = NAS_ESM_NULL;
    EMM_ESM_INTRA_DATA_IND_STRU        *pMsg             = VOS_NULL_PTR;
    NAS_ESM_EPSB_CNTXT_INFO_STRU       *pstEpsbCntxtInfo = VOS_NULL_PTR;
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr     = VOS_NULL_PTR;
    NAS_ESM_ENCODE_INFO_STRU            stEncodeInfo;

    /*��ӡ����ú���*/
    NAS_ESM_NORM_LOG("NAS_ESM_ProcValidNwMsgActDefltEpsbReq is entered.");

    pMsg = (EMM_ESM_INTRA_DATA_IND_STRU *) pRcvMsg;

    /*����PTI�ҵ�״̬���ulStateTblIndex������״̬�����PTI�ܹ��ҵ���Ӧ״̬��*/
    if(NAS_ESM_FAILURE == NAS_ESM_QueryStateTblIndexByPti(pMsg->stEsmNwMsgIE.ucPti,&ulStateTblIndex))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ProcValidNwMsgActDefltEpsbReq:WARNING: Tbl index Query Fail!");
        return;
    }

    pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

    /*ȡ��������Ϣ*/
    ulEpsbId = (VOS_UINT32)(pMsg->stEsmNwMsgIE.ucEpsbId);
    pstEpsbCntxtInfo = NAS_ESM_GetEpsbCntxtInfoAddr(ulEpsbId);
    pstEpsbCntxtInfo->ulLinkedEpsbId = 0;

    /*��ʼ���������NAS_ESM_ENCODE_INFO_STRU*/
    NAS_ESM_MEM_SET((VOS_VOID*)&stEncodeInfo, 0, sizeof(NAS_ESM_ENCODE_INFO_STRU));
    stEncodeInfo.bitOpESMCau = NAS_ESM_OP_FALSE;
    stEncodeInfo.ucPti       = pstStateAddr->stNwMsgRecord.ucPti;
    stEncodeInfo.ulEpsbId    = ulEpsbId;

    /*ֹͣ��ʱ��*/
    NAS_ESM_TimerStop(ulStateTblIndex, TI_NAS_ESM_T3482);

    /* �Զ�����ȱʡ���ؼ���Ĵ��� */
    NAS_ESM_DefltBearActMsgAutoAccept(ulStateTblIndex,
                                      stEncodeInfo,
                                      &(pMsg->stEsmNwMsgIE));
}


VOS_VOID NAS_ESM_SaveNwActDefltEpsbReqInfo
(
    const  NAS_ESM_NW_MSG_STRU          *pstMsgIE,
    VOS_UINT32                           ulStateTblIndex
)
{
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr      = VOS_NULL_PTR;
    NAS_ESM_SDF_CNTXT_INFO_STRU        *pstSdfCntxtInfo   = VOS_NULL_PTR;
    NAS_ESM_EPSB_CNTXT_INFO_STRU       *pstEpsbCntxtInfo  = VOS_NULL_PTR;

    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG("NAS_ESM_SaveNwActDefltEpsbReqInfo is entered.");

    pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

    pstSdfCntxtInfo = NAS_ESM_GetSdfCntxtInfo(pstStateAddr->ulCid);

    /* ����SDF��QOS���� */
    NAS_ESM_MEM_CPY(&pstSdfCntxtInfo->stSdfQosInfo,
                    &pstMsgIE->stSdfQosInfo,
                    sizeof(NAS_ESM_CONTEXT_LTE_QOS_STRU));

    /* ����SDF�������е�PF��Ϣ�͹���������Ϣ */
    pstSdfCntxtInfo->ulSdfPfNum = NAS_ESM_NULL;
    pstSdfCntxtInfo->ulEpsbId = pstMsgIE->ucEpsbId;

    pstEpsbCntxtInfo = NAS_ESM_GetEpsbCntxtInfoAddr(pstMsgIE->ucEpsbId);

    /* ����EPS QOS��ʶλ��EPS QOS���� */
    pstEpsbCntxtInfo->bitOpEpsQos = NAS_ESM_OP_TRUE;
    NAS_ESM_MEM_CPY(&(pstEpsbCntxtInfo->stEpsQoSInfo), \
                    &(pstSdfCntxtInfo->stSdfQosInfo), \
                    sizeof(NAS_ESM_CONTEXT_LTE_QOS_STRU));

    /*�洢PDN��Ϣ������PDN��ʶλ*/
    pstEpsbCntxtInfo->bitOpPdnAddr = NAS_ESM_OP_TRUE;
    NAS_ESM_MEM_CPY(&pstEpsbCntxtInfo->stPdnAddrInfo,
                    &pstMsgIE->stPdnAddrInfo,
                    sizeof(NAS_ESM_CONTEXT_IP_ADDR_STRU));
    if ((NAS_ESM_PDN_IPV4 == pstEpsbCntxtInfo->stPdnAddrInfo.ucIpType)
        || (NAS_ESM_PDN_IPV4_IPV6 == pstEpsbCntxtInfo->stPdnAddrInfo.ucIpType))
    {
        #if (VOS_OS_VER != VOS_WIN32)
        /*������������IP��ַ����CPE��IP��ַ��Ŀǰֻ������IPV4*/
        pstEpsbCntxtInfo->bitOpGateWayAddrInfo = NAS_ESM_OP_TRUE;

        /* ����IP��ַ����������� */
        pstEpsbCntxtInfo->stSubnetMask.ucIpType = NAS_ESM_PDN_IPV4;
        NAS_ESM_DHCPGetIPv4Mask(pstEpsbCntxtInfo->stPdnAddrInfo.aucIpV4Addr,
                                        pstEpsbCntxtInfo->stSubnetMask.aucIpV4Addr);

        /* ����IP��ַ������ص�ַ */
        pstEpsbCntxtInfo->stGateWayAddrInfo.ucIpType = NAS_ESM_PDN_IPV4;
        NAS_ESM_DHCPGetIPv4GateWay(pstEpsbCntxtInfo->stPdnAddrInfo.aucIpV4Addr,
                                        pstEpsbCntxtInfo->stSubnetMask.aucIpV4Addr,
                                        pstEpsbCntxtInfo->stGateWayAddrInfo.aucIpV4Addr);
        #else
        /*������������IP��ַ����CPE��IP��ַ��Ŀǰֻ������IPV4*/
        pstEpsbCntxtInfo->bitOpGateWayAddrInfo = NAS_ESM_OP_TRUE;
        NAS_ESM_MEM_CPY(&pstEpsbCntxtInfo->stGateWayAddrInfo,
                        &pstEpsbCntxtInfo->stPdnAddrInfo,
                        sizeof(NAS_ESM_CONTEXT_IP_ADDR_STRU));
        if(pstEpsbCntxtInfo->stGateWayAddrInfo.aucIpV4Addr[APP_MAX_IPV4_ADDR_LEN-1] == NAS_ESM_GATEWAYE_IP1)
        {
            pstEpsbCntxtInfo->stGateWayAddrInfo.aucIpV4Addr[APP_MAX_IPV4_ADDR_LEN-1] = NAS_ESM_GATEWAYE_IP2;
        }
        else
        {
            pstEpsbCntxtInfo->stGateWayAddrInfo.aucIpV4Addr[APP_MAX_IPV4_ADDR_LEN-1] = NAS_ESM_GATEWAYE_IP1;
        }

        NAS_ESM_SetSubnetMask(pstEpsbCntxtInfo->stSubnetMask);
        #endif
    }
    /*����APN��ʶλ*/
    pstEpsbCntxtInfo->bitOpApn= NAS_ESM_OP_TRUE;
    NAS_ESM_MEM_CPY(&pstEpsbCntxtInfo->stApnInfo,
                    &pstMsgIE->stApnInfo,
                    sizeof(APP_ESM_APN_INFO_STRU));

    /*���TI(�������)��ֱ�Ӵ洢*/
    pstEpsbCntxtInfo->bitOpTransId = pstMsgIE->bitOpTransId;
    NAS_ESM_MEM_CPY(    &pstEpsbCntxtInfo->stTransId,
                        &pstMsgIE->stTransId,
                        sizeof(NAS_ESM_CONTEXT_TRANS_ID_STRU));

    /*���PDP QOS(�������)��ֱ�Ӵ洢*/
    pstEpsbCntxtInfo->bitOpNegQos = pstMsgIE->bitOpNegQos;
    NAS_ESM_MEM_CPY(    &pstEpsbCntxtInfo->stCntxtQosInfo,
                        &pstMsgIE->stCntxtQosInfo,
                        sizeof(NAS_ESM_CONTEXT_QOS_STRU));

    /*���LLC SAPI(�������)��ֱ�Ӵ洢*/
    pstEpsbCntxtInfo->bitOpLlcSapi = pstMsgIE->bitOpLlcSapi;
    pstEpsbCntxtInfo->ucLlcSapi = pstMsgIE->ucLlcSapi;

    /*���Radio Priority(�������)��ֱ�Ӵ洢*/
    pstEpsbCntxtInfo->bitOpRadioPriority = pstMsgIE->bitOpRadioPriority;
    pstEpsbCntxtInfo->ucRadioPriority = pstMsgIE->ucRadioPriority;

    /*���Packet Flow Identifer(�������)��ֱ�Ӵ洢*/
    pstEpsbCntxtInfo->bitOpPacketFlowId = pstMsgIE->bitOpPacketFlowId;
    pstEpsbCntxtInfo->ucPacketFlowId = pstMsgIE->ucPacketFlowId;

    /*���APN-AMBR(�������)��ֱ�Ӵ洢*/
    pstEpsbCntxtInfo->bitOpApnAmbr = pstMsgIE->bitOpApnAmbr;
    NAS_ESM_MEM_CPY(&pstEpsbCntxtInfo->stApnAmbrInfo,
                    &pstMsgIE->stApnAmbrInfo,
                    sizeof(NAS_ESM_CONTEXT_APN_AMBR_STRU));

    /*���ESM Cause(�������)��ֱ�Ӵ洢*/
    pstEpsbCntxtInfo->bitOpEsmCause = pstMsgIE->bitOpEsmCause;
    pstEpsbCntxtInfo->enEsmCau = pstMsgIE->enEsmCau;

    /*���ESM PCO(�������)��ֱ�Ӵ洢*/
    pstEpsbCntxtInfo->bitOpPco = pstMsgIE->bitOpPco;
    NAS_ESM_MEM_CPY(&pstEpsbCntxtInfo->stPcoInfo,
                    &pstMsgIE->stPcoInfo,
                    sizeof(NAS_ESM_CONTEXT_PCO_STRU));
}
/*****************************************************************************
 Function Name   : NAS_ESM_SndEsmAppPdpManageIndMsg
 Description     : ��APP����
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2009-12-3  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ESM_SndEsmAppPdpManageIndMsg
(
    const APP_ESM_PDP_MANAGER_IND_STRU       *pstPdpManInd
)
{
    APP_ESM_PDP_MANAGER_IND_STRU    *pstPdpManIndTmp = VOS_NULL_PTR;

    /*����ռ�ͼ���Ƿ����ɹ�*/
    pstPdpManIndTmp = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(APP_ESM_PDP_MANAGER_IND_STRU));
    if (VOS_NULL_PTR == pstPdpManIndTmp)
    {
        NAS_ESM_ERR_LOG("NAS_ESM_SndEsmAppPdpManageIndMsg:ERROR:Alloc msg fail!" );
        return;
    }

    /*���*/
    NAS_ESM_MEM_SET(NAS_ESM_GET_MSG_ENTITY(pstPdpManIndTmp), NAS_ESM_NULL, NAS_ESM_GET_MSG_LENGTH(pstPdpManIndTmp));

    /*��д��Ϣͷ*/
    NAS_ESM_WRITE_APP_MSG_HEAD(pstPdpManIndTmp,
                               ID_APP_ESM_PDP_MANAGER_IND,
                               pstPdpManInd->usOriginalId,
                               pstPdpManInd->usTerminalId,
                               pstPdpManInd->ulSN);

    /* ��д��Ϣ���� */
    pstPdpManIndTmp->ulCid           = pstPdpManInd->ulCid;
    pstPdpManIndTmp->ulOpId          = pstPdpManInd->ulOpId;
    pstPdpManIndTmp->enOperateType   = pstPdpManInd->enOperateType;

    pstPdpManIndTmp->bitOpLinkCid    = pstPdpManInd->bitOpLinkCid;
    pstPdpManIndTmp->ulLinkCid       = pstPdpManInd->ulLinkCid;

    pstPdpManIndTmp->bitOpModifyTpye = pstPdpManInd->bitOpModifyTpye;
    pstPdpManIndTmp->enBearerModifyType = pstPdpManInd->enBearerModifyType;

    /* ������Ϣ���ͺ���*/
    NAS_ESM_SND_MSG(pstPdpManIndTmp);

}

/*****************************************************************************
 Function Name   : NAS_ESM_IsDhcpProcedureNeeded
 Description     : �ж��Ƿ���Ҫ����DHCP���̻�ȡ�������
 Input           : ucEpsbId-----------------���غ�
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2010-11-30  Draft Enact

*****************************************************************************/
VOS_UINT32 NAS_ESM_IsDhcpProcedureNeeded
(
    VOS_UINT32                          ucEpsbId
)
{
    NAS_ESM_EPSB_CNTXT_INFO_STRU       *pstEpsbCntxtInfo = VOS_NULL_PTR;

    pstEpsbCntxtInfo = NAS_ESM_GetEpsbCntxtInfoAddr(ucEpsbId);

    /* ���PDN����ΪIPV6������Ҫ����DHCP���� */
    if (NAS_ESM_PDN_IPV6 == pstEpsbCntxtInfo->stPdnAddrInfo.ucIpType)
    {
        NAS_ESM_NORM_LOG("NAS_ESM_IsDhcpProcedureNeeded:PDN type is ipv6!");
        return PS_FALSE;
    }

    /* ���IPv4��ַΪ0.0.0.0������Ҫ����DHCP���� */
    if (NAS_ESM_IS_IPV4_ADDR_INVALID(pstEpsbCntxtInfo->stPdnAddrInfo.aucIpV4Addr))
    {
        NAS_ESM_NORM_LOG("NAS_ESM_IsDhcpProcedureNeeded:PDN address is 0.0.0.0!");
        return PS_TRUE;
    }

    /* ���������DNS������������Ҫ����DHCP���� */
    if ((NAS_ESM_OP_TRUE != pstEpsbCntxtInfo->bitOpPco)
        || (NAS_ESM_NULL == pstEpsbCntxtInfo->stPcoInfo.ucIpv4DnsSerNum))
    {
        NAS_ESM_NORM_LOG("NAS_ESM_IsDhcpProcedureNeeded:DNS address is not exist!");
        return PS_TRUE;
    }

    return PS_FALSE;
}

/*****************************************************************************
 Function Name   : NAS_ESM_SndEsmAppSdfSetupSuccMsg
 Description     : ��APP�ظ��ɹ���Ϣ
 Input           : ucEpsbId-----------------���غ�
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-11-30  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_SndEsmAppSdfSetupSuccMsg
(
    VOS_UINT32                          ulStateTblIndex
)
{
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr     = VOS_NULL_PTR;

    pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);
    if((NAS_ESM_OP_TRUE == pstStateAddr->bitOpAppMsgRecord)
        && (ID_APP_ESM_PDP_SETUP_REQ == pstStateAddr->stAppMsgRecord.enAppMsgType))
    {
        /*��APP����ID_APP_ESM_PDP_SETUP_CNF(�ɹ�)��Ϣ*/
        NAS_ESM_SndEsmAppSdfSetupCnfSuccMsg(ulStateTblIndex);
    }
    else if((NAS_ESM_OP_TRUE == pstStateAddr->bitOpAppMsgRecord)
            && (ID_APP_ESM_NDISCONN_REQ == pstStateAddr->stAppMsgRecord.enAppMsgType))
    {
        /*��APP����ID_APP_ESM_NDISCONN_CNF(�ɹ�)��Ϣ*/
        NAS_ESM_SndEsmAppNdisConnCnfSuccMsg(ulStateTblIndex);
    }
    else
    {
        NAS_ESM_SndEsmAppSdfSetupIndMsg(ulStateTblIndex, APP_SUCCESS);
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_SetEpsBearTftInfo
 Description     : ��ֵ����TFT��Ϣ
 Input           : ulBitCId-----------------���ع���CID
                   pstEpsBearInfoInd--------ESM��SM�ӿڵĳ�����Ϣָ��
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2011-04-30  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_SetEpsBearTftInfo
(
    VOS_UINT32                          ulBitCId,
    SM_ESM_EPS_BEARER_INFO_IND_STRU    *pstEpsBearInfoInd
)
{
    VOS_UINT32                          ulCnt1          = NAS_ESM_NULL;
    VOS_UINT32                          ulCnt2          = NAS_ESM_NULL;
    NAS_ESM_SDF_CNTXT_INFO_STRU        *pstSdfCntxtInfo = VOS_NULL_PTR;

    pstEpsBearInfoInd->bitOpTft         = NAS_ESM_OP_TRUE;
    pstEpsBearInfoInd->stTft.ulPfNum    = NAS_ESM_NULL;

    for ( ulCnt1 = NAS_ESM_MIN_CID; ulCnt1 <= NAS_ESM_MAX_CID; ulCnt1++ )
    {
        if(NAS_ESM_OP_FALSE == ((ulBitCId >> ulCnt1) & NAS_ESM_OP_TRUE))
        {
            continue;
        }

        pstSdfCntxtInfo = NAS_ESM_GetSdfCntxtInfo(ulCnt1);
        for ( ulCnt2 = NAS_ESM_NULL; ulCnt2 < pstSdfCntxtInfo->ulSdfPfNum; ulCnt2++ )
        {
            pstEpsBearInfoInd->stTft.astCidPf[pstEpsBearInfoInd->stTft.ulPfNum].ulCid = ulCnt1;
            NAS_ESM_MEM_CPY(    &pstEpsBearInfoInd->stTft.astCidPf[pstEpsBearInfoInd->stTft.ulPfNum].stPf,
                                &pstSdfCntxtInfo->astSdfPfInfo[ulCnt2],
                                sizeof(NAS_ESM_CONTEXT_TFT_STRU));
            pstEpsBearInfoInd->stTft.ulPfNum++;
        }
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_SetEpsBearPcoIpv4Item
 Description     : ��ֵ����PCO IPV4��
 Input           : pstEpsbCntxtInfo-----------------ESM������Ϣָ��
                   pstEpsBearInfoInd----------------ESM��SM�ӿ��еĳ�����Ϣָ��
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2011-04-30  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_SetEpsBearPcoIpv4Item
(
    const NAS_ESM_EPSB_CNTXT_INFO_STRU *pstEpsbCntxtInfo,
    SM_ESM_EPS_BEARER_INFO_IND_STRU    *pstEpsBearInfoInd
)
{
    if (pstEpsbCntxtInfo->stPcoInfo.ucIpv4DnsSerNum == 1)
    {
        pstEpsBearInfoInd->bitOpPcoIpv4Item = NAS_ESM_OP_TRUE;
        pstEpsBearInfoInd->stPcoIpv4Item.bitOpPriDns = NAS_ESM_OP_TRUE;
        NAS_ESM_MEM_CPY(    pstEpsBearInfoInd->stPcoIpv4Item.aucPriDns,
                            pstEpsbCntxtInfo->stPcoInfo.astIpv4DnsServer[0].aucIpV4Addr,
                            APP_MAX_IPV4_ADDR_LEN);
        pstEpsBearInfoInd->stPcoIpv4Item.bitOpSecDns = NAS_ESM_OP_FALSE;
    }

    if (pstEpsbCntxtInfo->stPcoInfo.ucIpv4DnsSerNum == 2)
    {
        pstEpsBearInfoInd->bitOpPcoIpv4Item = NAS_ESM_OP_TRUE;
        pstEpsBearInfoInd->stPcoIpv4Item.bitOpPriDns = NAS_ESM_OP_TRUE;
        NAS_ESM_MEM_CPY(    pstEpsBearInfoInd->stPcoIpv4Item.aucPriDns,
                            pstEpsbCntxtInfo->stPcoInfo.astIpv4DnsServer[0].aucIpV4Addr,
                            APP_MAX_IPV4_ADDR_LEN);
        pstEpsBearInfoInd->stPcoIpv4Item.bitOpSecDns = NAS_ESM_OP_TRUE;
        NAS_ESM_MEM_CPY(    pstEpsBearInfoInd->stPcoIpv4Item.aucSecDns,
                            pstEpsbCntxtInfo->stPcoInfo.astIpv4DnsServer[1].aucIpV4Addr,
                            APP_MAX_IPV4_ADDR_LEN);
    }

    if (pstEpsbCntxtInfo->stPcoInfo.ucIpv4PcscfNum == 1)
    {
        pstEpsBearInfoInd->bitOpPcoIpv4Item = NAS_ESM_OP_TRUE;
        pstEpsBearInfoInd->stPcoIpv4Item.bitOpPriPcscf = NAS_ESM_OP_TRUE;
        NAS_ESM_MEM_CPY(    pstEpsBearInfoInd->stPcoIpv4Item.aucPriPcscf,
                            pstEpsbCntxtInfo->stPcoInfo.astIpv4Pcscf[0].aucIpV4Addr,
                            APP_MAX_IPV4_ADDR_LEN);
        pstEpsBearInfoInd->stPcoIpv4Item.bitOpSecPcscf = NAS_ESM_OP_FALSE;
    }

    if (pstEpsbCntxtInfo->stPcoInfo.ucIpv4PcscfNum == 2)
    {
        pstEpsBearInfoInd->bitOpPcoIpv4Item = NAS_ESM_OP_TRUE;
        pstEpsBearInfoInd->stPcoIpv4Item.bitOpPriPcscf = NAS_ESM_OP_TRUE;
        NAS_ESM_MEM_CPY(    pstEpsBearInfoInd->stPcoIpv4Item.aucPriPcscf,
                            pstEpsbCntxtInfo->stPcoInfo.astIpv4Pcscf[0].aucIpV4Addr,
                            APP_MAX_IPV4_ADDR_LEN);
        pstEpsBearInfoInd->stPcoIpv4Item.bitOpSecPcscf = NAS_ESM_OP_TRUE;
        NAS_ESM_MEM_CPY(    pstEpsBearInfoInd->stPcoIpv4Item.aucSecPcscf,
                            pstEpsbCntxtInfo->stPcoInfo.astIpv4Pcscf[1].aucIpV4Addr,
                            APP_MAX_IPV4_ADDR_LEN);
    }

    if (pstEpsbCntxtInfo->stPcoInfo.ucIpv4NbnsNum == 1)
    {
        pstEpsBearInfoInd->bitOpPcoIpv4Item = NAS_ESM_OP_TRUE;
        pstEpsBearInfoInd->stPcoIpv4Item.bitOpPriNbns = NAS_ESM_OP_TRUE;
        NAS_ESM_MEM_CPY(    pstEpsBearInfoInd->stPcoIpv4Item.aucPriNbns,
                            pstEpsbCntxtInfo->stPcoInfo.astIpv4Nbns[0].aucIpV4Addr,
                            APP_MAX_IPV4_ADDR_LEN);
        pstEpsBearInfoInd->stPcoIpv4Item.bitOpSecNbns = NAS_ESM_OP_FALSE;
    }

    if (pstEpsbCntxtInfo->stPcoInfo.ucIpv4NbnsNum == 2)
    {
        pstEpsBearInfoInd->bitOpPcoIpv4Item = NAS_ESM_OP_TRUE;
        pstEpsBearInfoInd->stPcoIpv4Item.bitOpPriNbns = NAS_ESM_OP_TRUE;
        NAS_ESM_MEM_CPY(    pstEpsBearInfoInd->stPcoIpv4Item.aucPriNbns,
                            pstEpsbCntxtInfo->stPcoInfo.astIpv4Nbns[0].aucIpV4Addr,
                            APP_MAX_IPV4_ADDR_LEN);
        pstEpsBearInfoInd->stPcoIpv4Item.bitOpSecNbns = NAS_ESM_OP_TRUE;
        NAS_ESM_MEM_CPY(    pstEpsBearInfoInd->stPcoIpv4Item.aucSecNbns,
                            pstEpsbCntxtInfo->stPcoInfo.astIpv4Nbns[1].aucIpV4Addr,
                            APP_MAX_IPV4_ADDR_LEN);
    }
}

VOS_VOID NAS_ESM_SetEpsBearPcoIpv6Item
(
    const NAS_ESM_EPSB_CNTXT_INFO_STRU *pstEpsbCntxtInfo,
    SM_ESM_EPS_BEARER_INFO_IND_STRU    *pstEpsBearInfoInd
)
{
    if (pstEpsbCntxtInfo->stPcoInfo.ucIpv6DnsSerNum == 1)
    {
        pstEpsBearInfoInd->bitOpPcoIpv6Item = NAS_ESM_OP_TRUE;
        pstEpsBearInfoInd->stPcoIpv6Item.bitOpPriDns = NAS_ESM_OP_TRUE;
        NAS_ESM_MEM_CPY(    pstEpsBearInfoInd->stPcoIpv6Item.aucPriDns,
                            pstEpsbCntxtInfo->stPcoInfo.astIpv6DnsServer[0].aucIpV6Addr,
                            APP_MAX_IPV6_ADDR_LEN);
        pstEpsBearInfoInd->stPcoIpv6Item.bitOpSecDns = NAS_ESM_OP_FALSE;
    }

    if (pstEpsbCntxtInfo->stPcoInfo.ucIpv6DnsSerNum == 2)
    {
        pstEpsBearInfoInd->bitOpPcoIpv6Item = NAS_ESM_OP_TRUE;
        pstEpsBearInfoInd->stPcoIpv6Item.bitOpPriDns = NAS_ESM_OP_TRUE;
        NAS_ESM_MEM_CPY(    pstEpsBearInfoInd->stPcoIpv6Item.aucPriDns,
                            pstEpsbCntxtInfo->stPcoInfo.astIpv6DnsServer[0].aucIpV6Addr,
                            APP_MAX_IPV6_ADDR_LEN);
        pstEpsBearInfoInd->stPcoIpv6Item.bitOpSecDns = NAS_ESM_OP_TRUE;
        NAS_ESM_MEM_CPY(    pstEpsBearInfoInd->stPcoIpv6Item.aucSecDns,
                            pstEpsbCntxtInfo->stPcoInfo.astIpv6DnsServer[1].aucIpV6Addr,
                            APP_MAX_IPV6_ADDR_LEN);
    }

    if (pstEpsbCntxtInfo->stPcoInfo.ucIpv6PcscfNum == 1)
    {
        pstEpsBearInfoInd->bitOpPcoIpv6Item = NAS_ESM_OP_TRUE;
        pstEpsBearInfoInd->stPcoIpv6Item.bitOpPriPcscf = NAS_ESM_OP_TRUE;
        NAS_ESM_MEM_CPY(    pstEpsBearInfoInd->stPcoIpv6Item.aucPriPcscf,
                            pstEpsbCntxtInfo->stPcoInfo.astIpv6Pcscf[0].aucIpV6Addr,
                            APP_MAX_IPV6_ADDR_LEN);
        pstEpsBearInfoInd->stPcoIpv6Item.bitOpSecPcscf = NAS_ESM_OP_FALSE;
    }

    if (pstEpsbCntxtInfo->stPcoInfo.ucIpv6PcscfNum == 2)
    {
        pstEpsBearInfoInd->bitOpPcoIpv6Item = NAS_ESM_OP_TRUE;
        pstEpsBearInfoInd->stPcoIpv6Item.bitOpPriPcscf = NAS_ESM_OP_TRUE;
        NAS_ESM_MEM_CPY(    pstEpsBearInfoInd->stPcoIpv6Item.aucPriPcscf,
                            pstEpsbCntxtInfo->stPcoInfo.astIpv6Pcscf[0].aucIpV6Addr,
                            APP_MAX_IPV6_ADDR_LEN);
        pstEpsBearInfoInd->stPcoIpv6Item.bitOpSecPcscf = NAS_ESM_OP_TRUE;
        NAS_ESM_MEM_CPY(    pstEpsBearInfoInd->stPcoIpv6Item.aucSecPcscf,
                            pstEpsbCntxtInfo->stPcoInfo.astIpv6Pcscf[1].aucIpV6Addr,
                            APP_MAX_IPV6_ADDR_LEN);
    }

}

/*****************************************************************************
 Function Name   : NAS_ESM_SetEpsBearInfo
 Description     : ��ֵ������Ϣ
 Input           : pstEpsbCntxtInfo-----------------ESM������Ϣָ��
                   pstEpsBearInfoInd----------------ESM��SM�ӿ��еĳ�����Ϣָ��
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2011-04-30  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_SetEpsBearInfo
(
    const NAS_ESM_EPSB_CNTXT_INFO_STRU *pstEpsbCntxtInfo,
    SM_ESM_EPS_BEARER_INFO_IND_STRU    *pstEpsBearInfoInd
)
{
    NAS_ESM_CONTEXT_APN_STRU            stTmpApnInfo    = {NAS_ESM_NULL};
    VOS_UINT32                          ulRslt          = NAS_ESM_NULL;

    /* ��ֵEPS QOS */
    pstEpsBearInfoInd->bitOpEpsQos = NAS_ESM_OP_TRUE;
    NAS_ESM_MEM_CPY(    &pstEpsBearInfoInd->stEpsQos,
                        &pstEpsbCntxtInfo->stEpsQoSInfo.stQosInfo,
                        sizeof(APP_ESM_EPS_QOS_INFO_STRU));

    /* ��ֵ����CID */
    pstEpsBearInfoInd->ulBitCid = pstEpsbCntxtInfo->ulBitCId;

    /* ��ֵTFT��Ϣ */
    NAS_ESM_SetEpsBearTftInfo(pstEpsbCntxtInfo->ulBitCId, pstEpsBearInfoInd);

    /* ��ֵLLC SAPI */
    pstEpsBearInfoInd->bitOpLlcSapi = pstEpsbCntxtInfo->bitOpLlcSapi;
    pstEpsBearInfoInd->ucLlcSapi = pstEpsbCntxtInfo->ucLlcSapi;

    /* ��ֵPDP QOS */
    pstEpsBearInfoInd->bitOpNegQos = pstEpsbCntxtInfo->bitOpNegQos;
    NAS_ESM_MEM_CPY(    &pstEpsBearInfoInd->stNegQos,
                        &pstEpsbCntxtInfo->stCntxtQosInfo,
                        sizeof(NAS_ESM_CONTEXT_QOS_STRU));

    /* ��ֵpacket flow id */
    pstEpsBearInfoInd->bitOpPacketFlowId = pstEpsbCntxtInfo->bitOpPacketFlowId;
    pstEpsBearInfoInd->ucPacketFlowId = pstEpsbCntxtInfo->ucPacketFlowId;

    /* ��ֵradio priority */
    pstEpsBearInfoInd->bitOpRadioPriority = pstEpsbCntxtInfo->bitOpRadioPriority;
    pstEpsBearInfoInd->ucRadioPriority = pstEpsbCntxtInfo->ucRadioPriority;

    /* ��ֵtransaction id */
    pstEpsBearInfoInd->bitOpTransId = pstEpsbCntxtInfo->bitOpTransId;
    NAS_ESM_MEM_CPY(    &pstEpsBearInfoInd->stTransId,
                        &pstEpsbCntxtInfo->stTransId,
                        sizeof(NAS_ESM_CONTEXT_TRANS_ID_STRU));

    if (PS_TRUE == NAS_ESM_IsDefaultEpsBearerType(pstEpsbCntxtInfo->enBearerCntxtType))
    {
        /* �������APN��Ϣת��δ�����APN��Ϣ */
        ulRslt = NAS_ESM_ApnTransformaton(&stTmpApnInfo,
                                          &pstEpsbCntxtInfo->stApnInfo);
        if(APP_SUCCESS != ulRslt)
        {
            pstEpsBearInfoInd->bitOpApn = NAS_ESM_OP_FALSE;
        }
        else
        {
            /* ��ֵAPN */
            pstEpsBearInfoInd->bitOpApn = NAS_ESM_OP_TRUE;
            pstEpsBearInfoInd->stApn.ucLength = stTmpApnInfo.ucApnLen;
            NAS_ESM_MEM_CPY(    pstEpsBearInfoInd->stApn.aucValue,
                                stTmpApnInfo.aucApnName,
                                APP_MAX_APN_LEN);
        }

        /* ��ֵPDN ADDRESS */
        NAS_ESM_MEM_CPY( &pstEpsBearInfoInd->stPdnAddr,
                         &pstEpsbCntxtInfo->stPdnAddrInfo,
                         sizeof(NAS_ESM_CONTEXT_IP_ADDR_STRU));

        /* ��ֵAPN-AMBR */
        pstEpsBearInfoInd->bitOpApnAmbr = pstEpsbCntxtInfo->bitOpApnAmbr;
        NAS_ESM_MEM_CPY(    &pstEpsBearInfoInd->stEpsApnAmbr,
                            &pstEpsbCntxtInfo->stApnAmbrInfo,
                            sizeof(NAS_ESM_CONTEXT_APN_AMBR_STRU));

        if (NAS_ESM_OP_TRUE == pstEpsbCntxtInfo->bitOpPco)
        {
            /* ��ֵBCM */
            pstEpsBearInfoInd->bitOpBcm = pstEpsbCntxtInfo->stPcoInfo.bitOpBcm;
            pstEpsBearInfoInd->enBcm = pstEpsbCntxtInfo->stPcoInfo.enBcm;

            /* ��ֵ����PCO IPV4�� */
            NAS_ESM_SetEpsBearPcoIpv4Item(pstEpsbCntxtInfo, pstEpsBearInfoInd);

            /* ��ֵ����PCO IPV6�� */
            NAS_ESM_SetEpsBearPcoIpv6Item(pstEpsbCntxtInfo, pstEpsBearInfoInd);
            /* lihong00150010 ims begin */
            pstEpsBearInfoInd->bitOpImCnSigalFlag   = pstEpsbCntxtInfo->stPcoInfo.bitOpImsCnSignalFlag;
            pstEpsBearInfoInd->enImCnSignalFlag     = (VOS_UINT8)pstEpsbCntxtInfo->stPcoInfo.enImsCnSignalFlag;
        }


        /* ����ǽ������أ������ý�����ʶ */
        if (NAS_ESM_BEARER_TYPE_EMERGENCY == pstEpsbCntxtInfo->enBearerCntxtType)
        {
            pstEpsBearInfoInd->bitOpEmcInd = NAS_ESM_OP_TRUE;
            pstEpsBearInfoInd->enEmergencyInd = SM_ESM_PDP_FOR_EMC;
        }
        /* lihong00150010 ims end */
    }
    else
    {
        /* ��ֵLink EpabId */
        pstEpsBearInfoInd->bitOpLinkedEpsbId = NAS_ESM_OP_TRUE;
        pstEpsBearInfoInd->ulLinkedEpsbId = pstEpsbCntxtInfo->ulLinkedEpsbId;

        /* ��ֵPDN ADDRESS */
        pstEpsbCntxtInfo = NAS_ESM_GetEpsbCntxtInfoAddr(pstEpsbCntxtInfo->ulLinkedEpsbId);
        NAS_ESM_MEM_CPY( &pstEpsBearInfoInd->stPdnAddr,
                         &pstEpsbCntxtInfo->stPdnAddrInfo,
                         sizeof(NAS_ESM_CONTEXT_IP_ADDR_STRU));
    }
}
/* lihong00150010 ims end */

/*****************************************************************************
 Function Name   : NAS_ESM_SndSmEspBearerActIndMsg
 Description     : ��SM���ͳ��ؼ�����Ϣ
 Input           : ucEpsbId-----------------���غ�
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2011-04-30  Draft Enact

*****************************************************************************/
/*lint -specific(-e433)*/

VOS_VOID NAS_ESM_SndSmEspBearerActIndMsg
(
    VOS_UINT32                          ulEpsbId
)
{
    NAS_ESM_EPSB_CNTXT_INFO_STRU       *pstEpsbCntxtInfo   = VOS_NULL_PTR;
    SM_ESM_EPS_BEARER_INFO_IND_STRU    *pstEpsBearInfoInd  = VOS_NULL_PTR;

    /*����ռ䲢��������Ƿ�ɹ�*/
    /*lint -e433 -e826*/
    pstEpsBearInfoInd = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(SM_ESM_EPS_BEARER_INFO_IND_STRU));
    /*lint +e433 +e826*/

    if ( VOS_NULL_PTR == pstEpsBearInfoInd )
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_SndSmEspBearerActIndMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    NAS_ESM_MEM_SET(NAS_ESM_GET_MSG_ENTITY(pstEpsBearInfoInd), 0, NAS_ESM_GET_MSG_LENGTH(pstEpsBearInfoInd));

    /*��ȡ��ӦEpsbCntxtTbl���ַ*/
    pstEpsbCntxtInfo = NAS_ESM_GetEpsbCntxtInfoAddr(ulEpsbId);

    pstEpsBearInfoInd->enPdpOption  = SM_ESM_PDP_OPT_ACTIVATE;
    pstEpsBearInfoInd->ulEpsbId     = ulEpsbId;

    /* ��ֵ������Ϣ */
    NAS_ESM_SetEpsBearInfo(pstEpsbCntxtInfo, pstEpsBearInfoInd);
    
    if( NAS_ESM_OP_TRUE == pstEpsbCntxtInfo->bitOpEsmCause)
    {
        /*���������Ϣ����ԭ���룬��֪ͨSM*/
        pstEpsBearInfoInd->bitOpErrorCode = NAS_ESM_OP_TRUE;
        pstEpsBearInfoInd->ulErrorCode    = NAS_ESM_GetAppErrorByEsmCause(pstEpsbCntxtInfo->enEsmCau);
    }
    else
    {
        pstEpsBearInfoInd->bitOpErrorCode = NAS_ESM_OP_FALSE;
    }

    /*��д��Ϣͷ*/
    NAS_ESM_WRITE_SM_MSG_HEAD(pstEpsBearInfoInd, ID_ESM_SM_EPS_BEARER_INFO_IND);

    /* ������Ϣ���ͺ��� */
    NAS_ESM_SND_MSG(pstEpsBearInfoInd);
}
/*lint -specific(+e433)*/


VOS_VOID NAS_ESM_DefltBearActMsgAutoAccept
(
    VOS_UINT32                          ulStateTblIndex,
    NAS_ESM_ENCODE_INFO_STRU            stEncodeInfo,
    const NAS_ESM_NW_MSG_STRU          *pstDecodedNwMsg
)
{
    VOS_UINT32                          ulSndNwMsgLen    = NAS_ESM_NULL;
    VOS_UINT32                          ulCurPdnNum      = NAS_ESM_NULL;
    VOS_UINT32                          ulRslt           = NAS_ESM_NULL;
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr     = VOS_NULL_PTR;

    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG("NAS_ESM_DefltBearActMsgAutoAccept is entered.");

    pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

    /* ����ȱʡ���ؼ������������Ϣ */
    NAS_ESM_SaveNwActDefltEpsbReqInfo(pstDecodedNwMsg,ulStateTblIndex);

    /*������ظ�Activate default bearer context accept*/
    NAS_ESM_EncodeActDefltEpsbCntxtAccMsg(stEncodeInfo, \
                                  NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                                 &ulSndNwMsgLen);
    NAS_ESM_SndAirMsgReportInd(NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                               ulSndNwMsgLen, NAS_ESM_AIR_MSG_DIR_ENUM_UP,\
                               ESM_ACT_DEFLT_EPS_BEARER_CNTXT_ACP);

    /*�����ǰ���̴���ATTACH�����У���֪ͨEMM PDN Conn SUCC������֪ͨEMM Data Req*/
    if(NAS_ESM_PS_REGISTER_STATUS_ATTACHED != NAS_ESM_GetEmmStatus())
    {
        NAS_ESM_SndEsmEmmPdnConSuccReqMsg(ulSndNwMsgLen, NAS_ESM_GetCurEsmSndNwMsgAddr());
    }
    else
    {
        NAS_ESM_SndEsmEmmDataReqMsg(NAS_ESM_ILLEGAL_OPID, PS_FALSE, ulSndNwMsgLen, NAS_ESM_GetCurEsmSndNwMsgAddr());
    }

    /* ���֧��˫APN�����¼ע����� */
    if (NAS_ESM_OP_TRUE == NAS_ESM_AttachBearerReestEnable())
    {
        if (NAS_ESM_ATTACH_CID == pstStateAddr->ulCid)
        {
            NAS_ESM_SetAttachBearerId(stEncodeInfo.ulEpsbId);
        }
    }

    NAS_ESM_UpdateEsmSndNwMsgLen(ulSndNwMsgLen);

    /* ����PTI�ͻظ���Ϣ */
    ulRslt = NAS_ESM_SavePtiBuffItem(stEncodeInfo.ucPti,
                                     (VOS_UINT8)stEncodeInfo.ulEpsbId,
                                     ulSndNwMsgLen,
                                     NAS_ESM_GetCurEsmSndNwMsgAddr());
    if (ulRslt != NAS_ESM_SUCCESS)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_DefltBearActMsgAutoAccept:WARN:Save pti buffer item failed!");
    }

    /*ת��״̬*/
    NAS_ESM_SetBearCntxtState(stEncodeInfo.ulEpsbId, NAS_ESM_BEARER_STATE_ACTIVE);
    if (NAS_ESM_BEARER_TYPE_EMERGENCY == pstStateAddr->enBearerCntxtType)
    {
        NAS_ESM_SetBearCntxtType(stEncodeInfo.ulEpsbId, NAS_ESM_BEARER_TYPE_EMERGENCY);
    }
    else
    {
        NAS_ESM_SetBearCntxtType(stEncodeInfo.ulEpsbId, NAS_ESM_BEARER_TYPE_DEFAULT);
    }
    NAS_ESM_SetBearCntxtLinkCid(stEncodeInfo.ulEpsbId,pstStateAddr->ulCid);

    /*��ǰPDN����������1��*/
    ulCurPdnNum = NAS_ESM_GetCurPdnNum()+1;

    /*���浱ǰPDN������*/
    NAS_ESM_SetCurPdnNum(ulCurPdnNum);

    /*�ؼ��¼��ϱ�*/
    NAS_ESM_SndKeyEventReportInd(NAS_ESM_EVENT_BEARER_SETUP(stEncodeInfo.ulEpsbId));

    /*��APP�ظ��ɹ���Ϣ*/
    NAS_ESM_SndEsmAppSdfSetupSuccMsg(ulStateTblIndex);
#if 0
    if (IP_DHCPV4SERVER_ON == IP_DHCPV4SERVER_GET_DHCPV4SERVER_FLAG())
    {
        if (1 == NAS_ESM_GetCurPdnNum())
        {
            /* ����ȡ�����������֪ͨIPģ�� */
            NAS_ESM_SndEsmIpNwParaIndMsg(pstDecodedNwMsg->ucEpsbId);
        }
    }
#endif

    /*��EMM����ID_EMM_ESM_BEARER_STATUS_REQ*/
    NAS_ESM_SndEsmEmmBearerStatusReqMsg(EMM_ESM_BEARER_CNTXT_MOD_MUTUAL);

    /*��RABM����ID_ESM_ERABM_ACT_IND*/
    NAS_ESM_SndEsmRabmActIndMsg(stEncodeInfo.ulEpsbId);

    NAS_ESM_PrintEpsbInfo(stEncodeInfo.ulEpsbId);

    /*�ͷ�״̬����Դ*/
    NAS_ESM_RelStateTblResource(ulStateTblIndex);

    /* ֪ͨSM���ؼ��� */
    NAS_ESM_SndSmEspBearerActIndMsg(stEncodeInfo.ulEpsbId);
}

/*****************************************************************************
 Function Name   : NAS_ESM_DefltBearActMsgManualProc
 Description     : ȱʡ���ؼ����ֶ�����
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2009-12-3  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ESM_DefltBearActMsgManualProc
(
    VOS_UINT32                          ulStateTblIndex,
    const NAS_ESM_NW_MSG_STRU          *pstDecodedNwMsg
)
{
    APP_ESM_PDP_MANAGER_IND_STRU        stPdpManInd;
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr     = VOS_NULL_PTR;

    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG("NAS_ESM_DefltBearActMsgManualProc is entered.");

    pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

    /* ���ٿռ䣬���ڻ���������������Ϣ */
    pstStateAddr->pstDecodedNwMsg = (VOS_VOID *)NAS_ESM_MEM_ALLOC(sizeof(NAS_ESM_NW_MSG_STRU));

    if (VOS_NULL_PTR == pstStateAddr->pstDecodedNwMsg)
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_DefltBInactPPendMsgNwActDefltEpsbReq:ERROR:SM->APP,Memory Alloc FAIL!");

        /*�ͷ�״̬����Դ*/
        NAS_ESM_RelStateTblResource(ulStateTblIndex);

        return ;
    }

     /* ��������������Ϣ������������ */
    NAS_ESM_MEM_CPY(pstStateAddr->pstDecodedNwMsg,
                    pstDecodedNwMsg,
                    sizeof(NAS_ESM_NW_MSG_STRU));

    /* �����ʱ����stPdpManInd */
    NAS_ESM_MEM_SET(&stPdpManInd,
                     NAS_ESM_NULL,
                     sizeof(APP_ESM_PDP_MANAGER_IND_STRU));

    stPdpManInd.bitOpLinkCid = NAS_ESM_OP_FALSE;
    stPdpManInd.bitOpModifyTpye = NAS_ESM_OP_FALSE;

    /* ��дCID */
    stPdpManInd.ulCid = pstStateAddr->ulCid;

    /* ��дOpid, OriginalId��TerminalId*/
    if((NAS_ESM_OP_TRUE == pstStateAddr->bitOpAppMsgRecord)
        &&(ID_APP_ESM_PDP_SETUP_REQ == pstStateAddr->stAppMsgRecord.enAppMsgType))
    {
        stPdpManInd.ulOpId = pstStateAddr->stAppMsgRecord.ulAppMsgOpId;
        stPdpManInd.usTerminalId = pstStateAddr->stAppMsgRecord.usOriginalId;
        stPdpManInd.usOriginalId = pstStateAddr->stAppMsgRecord.usTerminalId;
    }
    else
    {
        stPdpManInd.ulOpId = NAS_ESM_OPID_TYPE_MANAGER_IND;

        /* ���䱾�ι���APP_MSG_HEADER*/
        /* ��Ϊ��NAS_ESM_SndEsmAppPdpManageIndMsg�л�Ҫ����OriginalId��uTerminalId */
        /* �������ﴫ��Ĳ����Ƿ��� */
        NAS_ESM_AssignMidHeader(&stPdpManInd.usOriginalId,&stPdpManInd.usTerminalId);
    }

    /* ��д�������� */
    stPdpManInd.enOperateType = APP_ESM_BEARER_OPERATE_TYPE_DEF_ACT;

    /* ��APP����ID_APP_ESM_PDP_MANAGER_IND��Ϣ */
    NAS_ESM_SndEsmAppPdpManageIndMsg(&stPdpManInd);

    /* ������ʱ���ȴ�APP�ظ���Ϣ */
    NAS_ESM_TimerStart(ulStateTblIndex, TI_NAS_ESM_WAIT_APP_CNF);

    /* ���õȴ�App�ظ���־λ */
    pstStateAddr->ucIsWaitForUserAnswer = PS_TRUE;
}

/*****************************************************************************
 Function Name   : NAS_ESM_ActDefltBearFailProc
 Description     : ����ȱʡ����ʧ�ܴ���
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2009-12-4  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ESM_ActDefltBearFailProc
(
    const NAS_ESM_ENCODE_INFO_STRU     *pstEncodeInfo,
    VOS_UINT32                          ulStateTblIndex
)
{
    VOS_UINT32                          ulSndNwMsgLen    = NAS_ESM_NULL;
    VOS_UINT32                          ulRslt           = NAS_ESM_NULL;
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr     = VOS_NULL_PTR;

    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG("NAS_ESM_ActDefltBearFailProc is entered.");

    pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

    /*������ظ�Activate default bearer context reject*/
    NAS_ESM_EncodeActDefltEpsbCntxtRejMsg(*pstEncodeInfo, \
                                  NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                                 &ulSndNwMsgLen);

    NAS_ESM_SndAirMsgReportInd(NAS_ESM_GetCurEsmSndNwMsgAddr(),\
                                   ulSndNwMsgLen, NAS_ESM_AIR_MSG_DIR_ENUM_UP,\
                                   ESM_ACT_DEFLT_EPS_BEARER_CNTXT_REJ);
    NAS_ESM_SndEsmEmmDataReqMsg(NAS_ESM_ILLEGAL_OPID, PS_FALSE, ulSndNwMsgLen, NAS_ESM_GetCurEsmSndNwMsgAddr());
    NAS_ESM_UpdateEsmSndNwMsgLen(ulSndNwMsgLen);

    /* ����PTI�ͻظ���Ϣ */
    ulRslt =  NAS_ESM_SavePtiBuffItem(pstEncodeInfo->ucPti,
                                      (VOS_UINT8)pstEncodeInfo->ulEpsbId,
                                      ulSndNwMsgLen,
                                      NAS_ESM_GetCurEsmSndNwMsgAddr());
    if (ulRslt != NAS_ESM_SUCCESS)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ActDefltBearFailProc:WARN:Save pti buffer item failed!");
    }

    /*��APP�ظ���Ϣ*/
    if(ID_APP_ESM_PDP_SETUP_REQ == pstStateAddr->stAppMsgRecord.enAppMsgType )
    {
        /*��APP����ID_APP_ESM_PDP_SETUP_CNF(ʧ��)��Ϣ*/
        NAS_ESM_SndEsmAppSdfSetupCnfFailMsg(ulStateTblIndex, \
                                      NAS_ESM_GetAppErrorByEsmCause(pstEncodeInfo->ucESMCau));
    }

    /*��APP�ظ���Ϣ*/
    if(ID_APP_ESM_NDISCONN_REQ == pstStateAddr->stAppMsgRecord.enAppMsgType )
    {
        /*��APP����ID_APP_ESM_NDISCONN_CNF(ʧ��)��Ϣ*/
        NAS_ESM_LOG1("NAS_ESM_ActDefltBearFailProc:ERROR =", NAS_ESM_GetAppErrorByEsmCause(pstEncodeInfo->ucESMCau));
        NAS_ESM_SndEsmAppNdisConnCnfFailMsg(ulStateTblIndex, \
                                      NAS_ESM_GetAppErrorByEsmCause(pstEncodeInfo->ucESMCau));
    }


   /*�ͷ�״̬����Դ*/
    NAS_ESM_RelStateTblResource(ulStateTblIndex);
}
/*lint +e961*/
/*lint +e960*/

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
/* end of NasEsmNwMsgPdnConProc.c */
