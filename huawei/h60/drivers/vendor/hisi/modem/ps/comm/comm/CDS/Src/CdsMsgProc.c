


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/

#include "CDS.h"
#include "CdsUlProc.h"
#include "CdsIpfCtrl.h"
#include "CdsMsgProc.h"
#include "FcFlowCtrlMana.h"
#include "QosFcRabStatus.h"
#include "CdsImsProc.h"
#include "CdsDebug.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_CDS_MSG_PROC_C
/*lint +e767*/

/*****************************************************************************
  2 �ⲿ��������
*****************************************************************************/

/******************************************************************************
   3 ˽�ж���
******************************************************************************/

/******************************************************************************
   4 ȫ�ֱ�������
******************************************************************************/
VOS_UINT32      g_astUlIpfBidToNdisType[] = {
                        CDS_ADS_IP_PACKET_TYPE_DHCP_SERVERV4,
                        CDS_ADS_IP_PACKET_TYPE_ND_SERVERDHCPV6,
                        CDS_ADS_IP_PACKET_TYPE_BUTT,
                        CDS_ADS_IP_PACKET_TYPE_ICMPV6,
                        CDS_ADS_IP_PACKET_TYPE_LINK_FE80,
                        CDS_ADS_IP_PACKET_TYPE_LINK_FF,
                        CDS_ADS_IP_PACKET_TYPE_BUTT};

/*��BID���IP������.ע������Խ��*/
#define  CDS_UL_GET_IPPKT_TYPE_BY_BID(UlIpfBid)    \
                       (g_astUlIpfBidToNdisType[(UlIpfBid) - CDS_UL_IPF_BEARER_ID_DHCPV4])


/******************************************************************************
   5 ����ʵ��
******************************************************************************/




VOS_VOID CDS_UlDataIndToAds(TTF_MEM_ST *pstIpPkt)
{
    CDS_ADS_DATA_IND_STRU                *pstDataInd;
    VOS_UINT32                            ulMsgLen;
    IPF_RESULT_STRU                      *pstIpfRlst;
    VOS_UINT16                            usIpfResult;

    if (VOS_NULL_PTR == pstIpPkt)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_UlDataIndToAds : Input Para Error.");
        return;
    }

    /*������Ϣ�ڴ�*/
    ulMsgLen = sizeof(CDS_ADS_DATA_IND_STRU) + (pstIpPkt->usUsed - 4);
    pstDataInd = CDS_ALLOC_MSG_WITH_HDR(ulMsgLen);
    if (VOS_NULL_PTR == pstDataInd)
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_UlDataIndToAds : Alloc Msg Fail. Size=.",ulMsgLen);
        TTF_MemFree(UEPS_PID_CDS,pstIpPkt);
        return;
    }

    /*��д��Ϣ����*/
    CDS_CFG_MSG_HDR(pstDataInd,ACPU_PID_ADS_UL);
    pstDataInd->enMsgId     = ID_CDS_ADS_IP_PACKET_IND;

    /*��дModemId/RabId/Len*/
    pstDataInd->usLen       = pstIpPkt->usUsed;
    pstDataInd->enModemId   = CDS_UL_GET_MODEMID_FROM_TTF(pstIpPkt);
    pstDataInd->ucRabId     = CDS_UL_GET_RABID_FROM_TTF(pstIpPkt);

    /*����IPF�����BEEARID����д����*/
    usIpfResult = CDS_UL_GET_IPF_RESULT_FORM_TTF(pstIpPkt);
    pstIpfRlst = (IPF_RESULT_STRU *)(&usIpfResult);
    if ((CDS_UL_IPF_BEARER_ID_DHCPV4 <= pstIpfRlst->usBearedId)
         &&(CDS_UL_IPF_BEARER_ID_LL_FF >= pstIpfRlst->usBearedId))
    {
        pstDataInd->enIpPacketType = (VOS_UINT8)CDS_UL_GET_IPPKT_TYPE_BY_BID(pstIpfRlst->usBearedId);
    }
    else
    {
        pstDataInd->enIpPacketType = CDS_ADS_IP_PACKET_TYPE_BUTT;
    }

    /*���ݿ������ͷ�Դ�ڴ�*/
    PS_MEM_CPY(pstDataInd->aucData,pstIpPkt->pData,pstIpPkt->usUsed);
    TTF_MemFree(UEPS_PID_CDS,pstIpPkt);

    /*������Ϣ*/
    CDS_SEND_MSG(pstDataInd);

    CDS_INFO_LOG1(UEPS_PID_CDS,"CDS_UlDataIndToAds: Send Data Succ.Type ",pstIpfRlst->usBearedId);

    return;
}




VOS_UINT32  CDS_GetRanMode(VOS_VOID)
{
    CDS_ENTITY_STRU         *pstCdsEntity;

    /*���CDS��Ӧʵ����Ϣ*/
    pstCdsEntity = CDS_GetCdsEntity(MODEM_ID_0);
    if (VOS_NULL_PTR == pstCdsEntity)
    {
        return MMC_CDS_MODE_BUTT;
    }

    /*����CDSʵ���ģʽ��Ϣ*/
    return pstCdsEntity->enRanMode;
}


VOS_VOID CDS_ChangeRanModeToNULL(CDS_ENTITY_STRU *pstCdsEntity)
{

    CDS_ASSERT(VOS_NULL_PTR != pstCdsEntity);

    CDS_INFO_LOG2(UEPS_PID_CDS,"Enter CDS_ChangeRanModeToNULL.ModemId, PreRanMode :",pstCdsEntity->usModemId,pstCdsEntity->enRanMode);

    /* ���ؽ���NULLģʽ���� */
    if (MODEM_ID_0 == pstCdsEntity->usModemId)
    {
        Fc_SwitchToNull();
    }

    /*���ý���ģʽ*/
    pstCdsEntity->enRanMode = MMC_CDS_MODE_NULL;

    /*���Service Request��־*/
    pstCdsEntity->ulServiceReqFlg = PS_FALSE;

    /*����GU SR��־*/
    CDS_CLR_GU_ALL_RAB_SR_FLG(pstCdsEntity);

    /*����10ms�����Զ�ʱ��*/
    if (PS_SUCC != CDS_StartTimer(pstCdsEntity,CDS_TMR_ID_DL_10MS_PERIODIC_TMR))
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_ChangeRanModeToNULL:Start Tmr Fail.");
        return;
    }

    CDS_ERROR_LOG(UEPS_PID_CDS,"Leave CDS_ChangeRanModeToNULL Succ.");

    return;
}
VOS_VOID CDS_ChangeRanModeToLTE(CDS_ENTITY_STRU *pstCdsEntity)
{

    CDS_ASSERT(VOS_NULL_PTR != pstCdsEntity);

    CDS_INFO_LOG2(UEPS_PID_CDS,"Enter CDS_ChangeRanModeToLTE. ModemId,PreRanMode :",pstCdsEntity->usModemId,pstCdsEntity->enRanMode);

    if (MODEM_ID_0 == pstCdsEntity->usModemId)
    {
        if (PS_FALSE == pstCdsEntity->ulTestModeFlg)
        {
            Fc_SwitchToLte();
        }
        else
        {
            /* ���ؽ��뻷��ģʽ���� */
            Fc_SwitchToNull();
        }
    }

    /*���ý���ģʽ*/
    pstCdsEntity->enRanMode = MMC_CDS_MODE_LTE;

    /*���Service Request��־*/
    pstCdsEntity->ulServiceReqFlg = PS_FALSE;

    /*����GU SR��־*/
    CDS_CLR_GU_ALL_RAB_SR_FLG(pstCdsEntity);

    /*ֹͣNULL����10ms�����Զ�ʱ��*/
    CDS_StopTimer(pstCdsEntity,CDS_TMR_ID_DL_10MS_PERIODIC_TMR);

    /*�������л�������*/
    CDS_SendUlBuffData(pstCdsEntity);

    CDS_ERROR_LOG(UEPS_PID_CDS,"Leave CDS_ChangeRanModeToLTE Succ.");

    return;
}


VOS_VOID CDS_ChangeRanModeToGU(CDS_ENTITY_STRU *pstCdsEntity)
{

    CDS_ASSERT(VOS_NULL_PTR != pstCdsEntity);

    CDS_INFO_LOG2(UEPS_PID_CDS,"Enter CDS_ChangeRanModeToGU. ModemId,PreRanMode :",pstCdsEntity->usModemId,pstCdsEntity->enRanMode);

    if (MODEM_ID_0 == pstCdsEntity->usModemId)
    {
        if (PS_FALSE == pstCdsEntity->ulTestModeFlg)
        {
            Fc_SwitchToGu();
        }
        else
        {
            /* ���ؽ��뻷��ģʽ���� */
            Fc_SwitchToNull();
        }
    }

    /*���ý���ģʽ*/
    pstCdsEntity->enRanMode = MMC_CDS_MODE_GU;

    /*���Service Request��־*/
    pstCdsEntity->ulServiceReqFlg = PS_FALSE;

    /*�����л��淢�͵�RABM*/
    CDS_SendUlBuffDataToRabm(pstCdsEntity,CDS_NAS_ALL_BEARER_ID);

    /*����GU SR��־*/
    CDS_CLR_GU_ALL_RAB_SR_FLG(pstCdsEntity);

    /*ֹͣNULL����10ms�����Զ�ʱ��*/
    CDS_StopTimer(pstCdsEntity,CDS_TMR_ID_DL_10MS_PERIODIC_TMR);

    CDS_ERROR_LOG(UEPS_PID_CDS,"Leave CDS_ChangeRanModeToGU Succ.");

    return;
}



VOS_VOID CDS_MMC_MsgProc(MsgBlock  *pstMsg)
{
    MMC_CDS_MODE_CHANGE_IND_STRU        *pstModeChangeInd;
    CDS_ENTITY_STRU                     *pstCdsEntity;
    VOS_UINT16                           usModemId;

    /*�����Ч���ж�*/
    if (VOS_NULL_PTR == pstMsg)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_MMC_MsgProc : Input NULL Para.");
        return;
    }

    /*Proc Msg*/
    pstModeChangeInd = (MMC_CDS_MODE_CHANGE_IND_STRU *)((VOS_UINT32)pstMsg);

    /*����Pid���ModemId,����ModemId���Cdsʵ��ָ��*/
    usModemId = VOS_GetModemIDFromPid(pstModeChangeInd->ulSenderPid);
    pstCdsEntity = CDS_GetCdsEntity(usModemId);
    if (VOS_NULL_PTR == pstCdsEntity)
    {
        CDS_ERROR_LOG2(UEPS_PID_CDS,
                       "CDS_MMC_MsgProc : CDS_GetCdsEntity Fail.SenderPid,ModemId",
                       pstModeChangeInd->ulSenderPid,
                       usModemId);
        return;
    }

    switch(pstModeChangeInd->enMode)
    {
    case MMC_CDS_MODE_NULL:
        CDS_ChangeRanModeToNULL(pstCdsEntity);
        break;

    case MMC_CDS_MODE_GU:
        CDS_ChangeRanModeToGU(pstCdsEntity);
        break;

    case MMC_CDS_MODE_LTE:
        CDS_ChangeRanModeToLTE(pstCdsEntity);
        break;

    default:
        CDS_ERROR_LOG2(UEPS_PID_CDS,
                       "CDS_MMC_MsgProc:Change Ran Mode Error.ModemId,RanMode:",
                       pstCdsEntity->usModemId,
                       pstModeChangeInd->enMode);
        break;
    }

    return;
}
VOS_VOID CDS_NdStartFilterMsgProc(const MsgBlock  *pstMsg)
{
    NDCLIENT_CDS_START_FILTER_REQ_STRU   *pstReqMsg;
    CDS_NDCLIENT_START_FILTER_CNF_STRU   *pstCnfMsg;
    VOS_UINT32                            ulMsgLen;

    /*����ж�*/
    if (VOS_NULL_PTR == pstMsg)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_NdStartFilterMsgProc : Input Para Error.");
        return;
    }

    pstReqMsg = (NDCLIENT_CDS_START_FILTER_REQ_STRU *)((VOS_UINT32)pstMsg);
    if (pstReqMsg->enModemId >= CDS_MAX_MODEM_NUM)
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_NdStartFilterMsgProc : ModemId Error. ModemID.",pstReqMsg->enModemId);
        return;
    }

    ulMsgLen = sizeof(CDS_NDCLIENT_START_FILTER_CNF_STRU);
    pstCnfMsg = CDS_ALLOC_MSG_WITH_HDR(ulMsgLen);
    if (VOS_NULL_PTR == pstCnfMsg)
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_NdStartFilterMsgProc : Alloc Msg Fail. Size .",ulMsgLen);
        return;
    }

    /*��д��Ϣ����*/
    CDS_CFG_MSG_HDR(pstCnfMsg,UEPS_PID_NDCLIENT);
    pstCnfMsg->ulMsgId = ID_CDS_NDCLIENT_START_FILTER_CNF;

    /*����ModemId*/
    pstCnfMsg->enModemId = pstReqMsg->enModemId;
    pstCnfMsg->enRslt    = CDS_EnableDlIPFFilter(pstReqMsg->enModemId);

    /*������Ϣ*/
    CDS_SEND_MSG(pstCnfMsg);

    CDS_INFO_LOG(UEPS_PID_CDS,"CDS_NdStartFilterMsgProc: Send Cnf Msg Succ.");

    return;
}



VOS_VOID CDS_NdStopFilterMsgProc(const MsgBlock  *pstMsg)
{
    NDCLIENT_CDS_STOP_FILTER_REQ_STRU    *pstReqMsg;
    CDS_NDCLIENT_STOP_FILTER_CNF_STRU    *pstCnfMsg;
    VOS_UINT32                            ulMsgLen;

    /*����ж�*/
    if (VOS_NULL_PTR == pstMsg)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_NdStopFilterMsgProc : Input Para Error.");
        return;
    }

    pstReqMsg = (NDCLIENT_CDS_STOP_FILTER_REQ_STRU *)((VOS_UINT32)pstMsg);
    if (pstReqMsg->enModemId >= CDS_MAX_MODEM_NUM)
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_NdStopFilterMsgProc : ModemId Error. ModemID.",pstReqMsg->enModemId);
        return;
    }

    ulMsgLen = sizeof(CDS_NDCLIENT_STOP_FILTER_CNF_STRU);
    pstCnfMsg = CDS_ALLOC_MSG_WITH_HDR(ulMsgLen);
    if (VOS_NULL_PTR == pstCnfMsg)
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_NdStopFilterMsgProc : Alloc Msg Fail. Size .",ulMsgLen);
        return;
    }

    /*��д��Ϣ����*/
    CDS_CFG_MSG_HDR(pstCnfMsg,UEPS_PID_NDCLIENT);
    pstCnfMsg->ulMsgId = ID_CDS_NDCLIENT_STOP_FILTER_CNF;

    /*����ModemId*/
    pstCnfMsg->enModemId = pstReqMsg->enModemId;
    pstCnfMsg->enRslt    = CDS_DisableDlIPFFilter(pstReqMsg->enModemId);

    /*������Ϣ*/
    CDS_SEND_MSG(pstCnfMsg);

    CDS_INFO_LOG(UEPS_PID_CDS,"CDS_NdStopFilterMsgProc: Send Cnf Msg Succ.");

    return;

}


VOS_VOID CDS_NdDataReqMsgProc(MsgBlock  *pstMsg)
{
    NDCLIENT_CDS_DATA_REQ_STRU      *pstDataReq;
    CDS_ENTITY_STRU                 *pstCdsEntity;
    TTF_MEM_ST                      *pstNdPkt;

    /*����ж�*/
    if (VOS_NULL_PTR == pstMsg)
    {
        CDS_INFO_LOG(UEPS_PID_CDS,"CDS_NdDataReqMsgProc: Input Para Error.");
        return;
    }

    pstDataReq = (NDCLIENT_CDS_DATA_REQ_STRU *)((VOS_UINT32)pstMsg);

    /*��Ϣ������Ч�ж�*/
    if (VOS_NULL_PTR == pstDataReq->pstIpPacket)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_NdDataReqMsgProc: Packet Pointer NULL.");
        return;
    }

    if ((CDS_NAS_MIN_BEARER_ID > pstDataReq->ucRabId)
         || (CDS_NAS_MAX_BEARER_ID < pstDataReq->ucRabId))
    {
        TTF_MemFree(UEPS_PID_CDS,pstDataReq->pstIpPacket);
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_NdDataReqMsgProc: Rab Id Error.",pstDataReq->ucRabId);
        return;
    }

    CDS_DBG_UL_RECV_ND_PKT_NUM(1);

    /*����ModemId��ȡCDSʵ��*/
    pstCdsEntity = CDS_GetCdsEntity(pstDataReq->enModemId);
    if (VOS_NULL_PTR == pstCdsEntity)
    {
        TTF_MemFree(UEPS_PID_CDS,pstDataReq->pstIpPacket);
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_NdDataReqMsgProc: CDS_GetCdsEntity Error.",pstDataReq->enModemId);
        return;
    }

    /*�������Ϣ���浽TTF,������RabId����Ϊ���˵Ľ��*/
    pstNdPkt = pstDataReq->pstIpPacket;
    CDS_UL_SAVE_IPFRSLT_MODEMID_RABID_TO_TTF(pstNdPkt,
                                             pstDataReq->ucRabId,
                                             pstDataReq->enModemId,
                                             pstDataReq->ucRabId);
    /*���ýӿڽ����ݷ��͵��տ�*/
    CDS_UlDispatchDataByRanMode(pstCdsEntity,pstNdPkt);
    return;
}
VOS_VOID CDS_NDCLIENT_MsgProc(MsgBlock  *pstMsg)
{
    /*����ж�*/
    if (VOS_NULL_PTR == pstMsg)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_NDCLIENT_MsgProc : Input NULL Para.");
        return;
    }

    switch(TTF_GET_MSG_NAME(pstMsg))
    {
    case ID_NDCLIENT_CDS_START_FILTER_REQ:
        /*�������й���*/
        CDS_NdStartFilterMsgProc(pstMsg);
        break;

    case ID_NDCLIENT_CDS_STOP_FILTER_REQ:
        /*�ر����й���*/
        CDS_NdStopFilterMsgProc(pstMsg);
        break;

    case ID_NDCLIENT_CDS_DATA_REQ:
        /*ת�����ݵ��տ�*/
        CDS_NdDataReqMsgProc(pstMsg);
        break;

    default:
        CDS_INFO_LOG1(UEPS_PID_CDS,"CDS_NDCLIENT_MsgProc: Msg Id Error.MsgID=",TTF_GET_MSG_NAME(pstMsg));
        break;

    }

    return;
}


VOS_VOID CDS_RabmSerivceRequestNotify(VOS_UINT8 ucRabId, CDS_ENTITY_STRU *pstCdsEntity)
{
    CDS_RABM_SERVICE_IND_STRU   *pstIndMsg;
    VOS_UINT32                   ulMsgLen;

    /*����ж�*/
    if (VOS_NULL_PTR == pstCdsEntity)
    {
        CDS_INFO_LOG(UEPS_PID_CDS,"CDS_RabmSerivceRequestNotify Input Para Error.");
        return ;
    }

    if ((CDS_NAS_MIN_BEARER_ID > ucRabId) || (CDS_NAS_MAX_BEARER_ID < ucRabId))
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_RabmSerivceRequestNotify: Rab Id Error.",ucRabId);
        return;
    }

    /*�Ѿ�������ֱ�ӷ���*/
    if (PS_TRUE == CDS_GET_GU_RAB_SR_FLG(pstCdsEntity,ucRabId))
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_RabmSerivceRequestNotify: Has Trigger SR..",ucRabId);
        return;
    }

    CDS_INFO_LOG2(UEPS_PID_CDS,"Enter CDS_RabmSerivceRequestNotify.ModemId,RabId",pstCdsEntity->usModemId,ucRabId);

    ulMsgLen = sizeof(CDS_RABM_SERVICE_IND_STRU);
    pstIndMsg = CDS_ALLOC_MSG_WITH_HDR(ulMsgLen);
    if (VOS_NULL_PTR == pstIndMsg)
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_RabmSerivceRequestNotify : Alloc Msg Fail. Size .",ulMsgLen);
        return;
    }

    /*��д��Ϣ����*/
    if (MODEM_ID_0 == pstCdsEntity->usModemId)
    {
        CDS_CFG_MSG_HDR(pstIndMsg,I0_WUEPS_PID_RABM);
    }
    else
    {
        CDS_CFG_MSG_HDR(pstIndMsg,I1_WUEPS_PID_RABM);
    }

    pstIndMsg->enMsgId = ID_CDS_RABM_SERVICE_IND;
    pstIndMsg->ucRabId = ucRabId;

    /*������Ϣ*/
    CDS_SEND_MSG(pstIndMsg);

    /*����SR��־*/
    CDS_SET_GU_RAB_SR_FLG(pstCdsEntity,ucRabId);

    CDS_INFO_LOG(UEPS_PID_CDS,"Leave CDS_RabmSerivceRequestNotify: Send Service Ind Msg Succ.");

    return;

}
VOS_VOID CDS_RabmSendBuffIndMsgProc(const MsgBlock  *pstMsg)
{
    VOS_UINT16                           usModemId;
    CDS_ENTITY_STRU                     *pstCdsEntity;
    CDS_RABM_SEND_BUFF_DATA_IND_STRU    *pstIndMsg;

    CDS_INFO_LOG(UEPS_PID_CDS,"Enter CDS_RabmSendBuffIndMsgProc.");

    /*����ж�*/
    if (VOS_NULL_PTR == pstMsg)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_RabmSendBuffIndMsgProc : Input NULL Para.");
        return;
    }

    /*����Pid���ModemId,����ModemId���Cdsʵ��ָ��*/
    usModemId = VOS_GetModemIDFromPid(pstMsg->ulSenderPid);
    pstCdsEntity = CDS_GetCdsEntity(usModemId);
    if (VOS_NULL_PTR == pstCdsEntity)
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_RabmSendBuffIndMsgProc : Get CDS Enttiy Fail.",usModemId);
        return;
    }

    pstIndMsg = (CDS_RABM_SEND_BUFF_DATA_IND_STRU *)((VOS_UINT32)pstMsg);

    /*RABID�ж�*/
    if ((CDS_NAS_MIN_BEARER_ID > pstIndMsg->ucRabId)
         || (CDS_NAS_MAX_BEARER_ID < pstIndMsg->ucRabId))
    {
        CDS_ERROR_LOG2(UEPS_PID_CDS,"CDS_RabmSendBuffIndMsgProc : Rab Id Error.ModemId,RbId",usModemId,pstIndMsg->ucRabId);
        return;
    }

    /*���RAB SR��־*/
    CDS_CLR_GU_RAB_SR_FLG(pstCdsEntity,pstIndMsg->ucRabId);

    /*�����л��淢�͵�RABM*/
    CDS_SendUlBuffDataToRabm(pstCdsEntity,pstIndMsg->ucRabId);

    CDS_INFO_LOG1(UEPS_PID_CDS,"Leave CDS_RabmSendBuffIndMsgProc. ModemId:",usModemId);

    return;
}


VOS_VOID CDS_RabmFreeBuffIndMsgProc(const MsgBlock  *pstMsg)
{
    VOS_UINT16                           usModemId;
    CDS_ENTITY_STRU                     *pstCdsEntity;
    CDS_RABM_FREE_BUFF_DATA_IND_STRU    *pstIndMsg;

    CDS_INFO_LOG(UEPS_PID_CDS,"Enter CDS_RabmFreeBuffIndMsgProc.");

    /*����ж�*/
    if (VOS_NULL_PTR == pstMsg)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_RabmFreeBuffIndMsgProc : Input Null Para.");
        return;
    }

    /*����Pid���ModemId,����ModemId���Cdsʵ��ָ��*/
    usModemId = VOS_GetModemIDFromPid(pstMsg->ulSenderPid);
    pstCdsEntity = CDS_GetCdsEntity(usModemId);
    if (VOS_NULL_PTR == pstCdsEntity)
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_RabmFreeBuffIndMsgProc : Get CDS Entity Fail.",usModemId);
        return;
    }

    pstIndMsg = (CDS_RABM_FREE_BUFF_DATA_IND_STRU *)((VOS_UINT32)pstMsg);

    /*RABID�ж�*/
    if ((CDS_NAS_MIN_BEARER_ID > pstIndMsg->ucRabId)
         || (CDS_NAS_MAX_BEARER_ID < pstIndMsg->ucRabId))
    {
        CDS_ERROR_LOG2(UEPS_PID_CDS,"CDS_RabmFreeBuffIndMsgProc : Rab Id Error.ModemId,RabId",usModemId,pstIndMsg->ucRabId);
        return;
    }

    /*���ָ��RABID SR��־*/
    CDS_CLR_GU_RAB_SR_FLG(pstCdsEntity,pstIndMsg->ucRabId);

    /*���ָ��RABID���л���*/
    CDS_UlGUClearBuffData(pstCdsEntity,pstIndMsg->ucRabId);

    CDS_INFO_LOG1(UEPS_PID_CDS,"Leave CDS_RabmFreeBuffIndMsgProc.ModemID:",usModemId);

    return;
}
VOS_VOID CDS_RABM_MsgProc(const MsgBlock  *pstMsg)
{
    /*����ж�*/
    if (VOS_NULL_PTR == pstMsg)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_RABM_MsgProc : Input Para Null.");
        return;
    }

    switch(TTF_GET_MSG_NAME(pstMsg))
    {
    case ID_RABM_CDS_FREE_BUFF_DATA_IND:
        CDS_RabmFreeBuffIndMsgProc(pstMsg);
        break;

    case ID_RABM_CDS_SEND_BUFF_DATA_IND:
        CDS_RabmSendBuffIndMsgProc(pstMsg);
        break;

    case ID_QOS_FC_RABM_RAB_CREATE_IND:
        QosFc_RabCreate(pstMsg);
        break;

    case ID_QOS_FC_RABM_RAB_RELEASE_IND:
        QosFc_RabRelease(pstMsg);
        break;

    default:
        CDS_INFO_LOG1(UEPS_PID_CDS,"CDS_RABM_MsgProc : Recv Rabm Msg Id Error.",TTF_GET_MSG_NAME(pstMsg));
        break;
    }

    return;
}
VOS_VOID CDS_TmrMsgProc(const MsgBlock  *pstMsg)
{
    REL_TIMER_MSG         *pstTmrMsg;

    if (VOS_NULL_PTR == pstMsg)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_TmrMsgProc : Input Para Error.");
        return;
    }

    pstTmrMsg = (REL_TIMER_MSG *)((VOS_UINT32)pstMsg);

    switch(pstTmrMsg->ulName)
    {
    case CDS_TMR_ID_DL_10MS_PERIODIC_TMR:
        CDS_Dl10msPeridicTmrTimeoutProc(pstTmrMsg);
        break;

    case CDS_TMR_ID_FC_CHECK_TMR:
        /* ����״̬check */
        QosFc_FlowCtrl();
        break;

    case CDS_TMR_ID_LB_MODE_B_TMR:
        CDS_LoopBackModeBTimeoutProc(pstTmrMsg);
        break;

    case CDS_TMR_ID_ADQ_EMPTY_PROCTECT:
        CDS_AdqEmptyProctectTimeOutProc();
        break;

    case CDS_TMR_ID_UL_DATA_PROCTECT:
        CDS_UlDataProtectTmrTimeoutProc(pstTmrMsg);
        break;

    default:
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_TmrMsgProc : Tmr Msg Name Error.",pstTmrMsg->ulName);
        break;
    }
    return;
}


#if (CDS_FEATURE_ON == CDS_FEATURE_LTE)
VOS_VOID CDS_ERabmSerivceRequestNotify(VOS_UINT8 ucEpsbId)
{
    CDS_ERABM_SERVICE_NOTIFY_STRU   *pstNotifyMsg;
    VOS_UINT32                       ulMsgLen;

    /*����ж�*/
    if ((CDS_NAS_MIN_BEARER_ID > ucEpsbId) || (CDS_NAS_MAX_BEARER_ID < ucEpsbId))
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_ERabmSerivceRequestNotify: EpsbId Error.",ucEpsbId);
        return;
    }

    CDS_INFO_LOG1(UEPS_PID_CDS,"Enter CDS_ERabmSerivceRequestNotify.EpsbId:",ucEpsbId);

    ulMsgLen = sizeof(CDS_ERABM_SERVICE_NOTIFY_STRU);
    pstNotifyMsg = CDS_ALLOC_MSG_WITH_HDR(ulMsgLen);
    if (VOS_NULL_PTR == pstNotifyMsg)
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_ERabmSerivceRequestNotify : Alloc Msg Fail. Size .",ulMsgLen);
       return;
    }

    /*��д��Ϣ����*/
    CDS_CFG_MSG_HDR(pstNotifyMsg,PS_PID_RABM);
    pstNotifyMsg->ulMsgId = ID_CDS_ERABM_SERVICE_NOTIFY;

    /* lihong00150010 emergency tau&service begin */
    pstNotifyMsg->ucRabId = ucEpsbId;
    /* lihong00150010 emergency tau&service end */

    /*������Ϣ*/
    CDS_SEND_MSG(pstNotifyMsg);

    CDS_INFO_LOG(UEPS_PID_CDS,"Leave CDS_ERabmSerivceRequestNotify: Send Service Ind Msg Succ.");

    return;


}
VOS_VOID CDS_ERabmSendBuffIndMsgProc(MsgBlock  *pstMsg)
{
    ERABM_CDS_SEND_BUFF_DATA_IND_STRU    *pstSendBufInd;
    CDS_ENTITY_STRU                      *pstCdsEntity;

    /*����ж�*/
    if (VOS_NULL_PTR == pstMsg)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_ERabmSendBuffIndMsgProc : Input Null Para.");
        return;
    }

    pstSendBufInd = (ERABM_CDS_SEND_BUFF_DATA_IND_STRU*)((VOS_UINT32)pstMsg);

    CDS_INFO_LOG1(UEPS_PID_CDS,
                  "CDS_ERabmSendBuffIndMsgProc: Send Data Allowed Type .",
                  pstSendBufInd->enSndBuffDataAllowedType);

    /*����ModemId���CDSʵ��ָ�룬LNASĬ�ϲ�����Modem0*/
    pstCdsEntity = CDS_GetCdsEntity(MODEM_ID_0);
    if (VOS_NULL_PTR == pstCdsEntity)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_ERabmSendBuffIndMsgProc : Get CDS Entity Fail.");
        return;
    }

    /*���Service Request��־*/
    pstCdsEntity->ulServiceReqFlg = PS_FALSE;

    /*�������л�������*/
    CDS_SendUlBuffData(pstCdsEntity);

    return;
}


VOS_VOID CDS_ERabmFreeBuffIndMsgProc(const MsgBlock  *pstMsg)
{
    CDS_ENTITY_STRU                      *pstCdsEntity;

    /*����ж�*/
    if (VOS_NULL_PTR == pstMsg)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_ERabmFreeBuffIndMsgProc : Input Null Para");
        return;
    }

    CDS_INFO_LOG(UEPS_PID_CDS,"Enter CDS_ERabmFreeBuffIndMsgProc");

    /*����ModemId���CDSʵ��ָ�룬LNASĬ�ϲ�����Modem0*/
    pstCdsEntity = CDS_GetCdsEntity(MODEM_ID_0);
    if (VOS_NULL_PTR == pstCdsEntity)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_ERabmFreeBuffIndMsgProc : Get CDS Entity Fail.");
        return;
    }

    /*���Service Request��־*/
    pstCdsEntity->ulServiceReqFlg = PS_FALSE;

    /*������л�������*/
    CDS_ClearUlBuffData(pstCdsEntity);

    return;
}


VOS_VOID CDS_ERABM_MsgProc(MsgBlock  *pstMsg)
{
    /*����ж�*/
    if (VOS_NULL_PTR == pstMsg)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_ERABM_MsgProc : Input Para Error.");
        return;
    }

    switch(TTF_GET_MSG_NAME(pstMsg))
    {
    case ID_ERABM_CDS_FREE_BUFF_DATA_IND:
        CDS_ERabmFreeBuffIndMsgProc(pstMsg);
        break;

    case ID_ERABM_CDS_SEND_BUFF_DATA_IND:
        CDS_ERabmSendBuffIndMsgProc(pstMsg);
        break;

    case ID_QOS_FC_ERABM_RAB_CREATE_IND:
        QosFc_RabCreate(pstMsg);
        break;

    case ID_QOS_FC_ERABM_RAB_RELEASE_IND:
        QosFc_RabRelease(pstMsg);
        break;

    default:
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_ERABM_MsgProc : Recv Erabm Msg Id Error.",TTF_GET_MSG_NAME(pstMsg));
        break;
    }

    return;
}
CDS_TEST_MODE_ENUM_UINT32 CDS_GetTestMode(VOS_VOID)
{
    CDS_ENTITY_STRU         *pstCdsEntity;

    /*����ModemId���CDSʵ��ָ�룬LBĬ�ϲ�����Modem0*/
    pstCdsEntity = CDS_GetCdsEntity(MODEM_ID_0);
    if (VOS_NULL_PTR == pstCdsEntity)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_GetTestMode : Get CDS Entity Fail.");
        return CDS_TEST_MODE_BUTT;
    }

    /*����CDSʵ��Ļ���ģʽ*/
    if (PS_TRUE == pstCdsEntity->ulTestModeFlg)
    {
        return CDS_TEST_MODE_ACTIVATED;
    }
    else
    {
        return CDS_TEST_MODE_DEACTIVATED;
    }
}


VOS_VOID CDS_ActLookBackMsgProc(const MsgBlock  *pstMsg)
{
    CDS_ENTITY_STRU         *pstCdsEntity;

    /*�����Ч���ж�*/
    if (VOS_NULL_PTR == pstMsg)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_ActLookBackMsgProc : Input Para Error.");
        return;
    }

    CDS_INFO_LOG(UEPS_PID_CDS,"Enter CDS_ActLookBackMsgProc");

    /*����ModemId���CDSʵ��ָ�룬LBĬ�ϲ�����Modem0*/
    pstCdsEntity = CDS_GetCdsEntity(MODEM_ID_0);
    if (VOS_NULL_PTR == pstCdsEntity)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_ActLookBackMsgProc : Get CDS Entity Fail.");
        return ;
    }

    /* ���ؽ��뻷��ģʽ���� */
    Fc_SwitchToNull();

    /*���û���ģʽ��־λΪPS_TRUE*/
    pstCdsEntity->ulTestModeFlg = PS_TRUE;
    pstCdsEntity->ulLoopBackState = CDS_LB_STATE_ACTIVE;

    /*���Service Request��־*/
    pstCdsEntity->ulServiceReqFlg = PS_FALSE;

    /*������л���*/
    CDS_ClearUlBuffData(pstCdsEntity);

    /*��ջ��ض���*/
    CDS_ClearLoopBackQue(pstCdsEntity);

    CDS_INFO_LOG(UEPS_PID_CDS,"Leave CDS_ActLookBackMsgProc");

    return;
}
VOS_VOID CDS_DeactLookBackMsgProc(const MsgBlock  *pstMsg)
{
    CDS_ENTITY_STRU         *pstCdsEntity;

    /*�����Ч���ж�*/
    if (VOS_NULL_PTR == pstMsg)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_DeactLookBackMsgProc : Input Para Error.");
        return;
    }

    CDS_INFO_LOG(UEPS_PID_CDS,"Enter CDS_DeactLookBackMsgProc");

    /*����ModemId���CDSʵ��ָ�룬LBĬ�ϲ�����Modem0*/
    pstCdsEntity = CDS_GetCdsEntity(MODEM_ID_0);
    if (VOS_NULL_PTR == pstCdsEntity)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_DeactLookBackMsgProc : Get CDS Entity Fail.");
        return ;
    }

    /* �����˳�����ģʽ���� */
    if (MMC_CDS_MODE_LTE == pstCdsEntity->enRanMode)
    {
        Fc_SwitchToLte();
    }
    else if (MMC_CDS_MODE_GU == pstCdsEntity->enRanMode)
    {
        Fc_SwitchToGu();
    }
    else
    {
        /* ������ */
    }

    /*���û���ģʽ��־λΪPS_FALSE*/
    pstCdsEntity->ulTestModeFlg = PS_FALSE;
    pstCdsEntity->ulLoopBackState = CDS_LB_STATE_DEACTIVE;

    /*���Service Request��־*/
    pstCdsEntity->ulServiceReqFlg = PS_FALSE;

    /*������л���*/
    CDS_ClearUlBuffData(pstCdsEntity);

    /*��ջ��ض���*/
    CDS_ClearLoopBackQue(pstCdsEntity);

    CDS_INFO_LOG(UEPS_PID_CDS,"Leave CDS_DeactLookBackMsgProc");

    return;
}


VOS_VOID CDS_StartLBTestLoopMsgProc(const MsgBlock  *pstMsg)
{
    ETC_CDS_START_TEST_LOOP_NOTIFY_STRU  *pstStartTLMsg;
    CDS_ENTITY_STRU                      *pstCdsEntity;

    /*�����Ч���ж�*/
    CDS_ASSERT(VOS_NULL_PTR != pstMsg);

    CDS_INFO_LOG(UEPS_PID_CDS,"Enter CDS_StartLBTestLoopMsgProc");

    /*����ModemId���CDSʵ��ָ�룬LBĬ�ϲ�����Modem0*/
    pstCdsEntity = CDS_GetCdsEntity(MODEM_ID_0);
    if (VOS_NULL_PTR == pstCdsEntity)
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_StartLBTestLoopMsgProc : Get CDS_GetCdsEntity Fail.ModemId:",MODEM_ID_0);
        return ;
    }

    pstStartTLMsg = (ETC_CDS_START_TEST_LOOP_NOTIFY_STRU*)((VOS_VOID*)pstMsg);

    CDS_INFO_LOG2(UEPS_PID_CDS,"CDS_StartLBTestLoopMsgProc : LB Mode,Timer Len.",pstStartTLMsg->ulLBMode,pstStartTLMsg->ulLBModeBTmrLen);

    /*���ֻ���ģʽ��Ϣ*/
    if (CDS_ETC_LB_MODE_B == pstStartTLMsg->ulLBMode)
    {
        pstCdsEntity->ulLoopBackMode = CDS_LB_MODE_B;
        if (0 != pstStartTLMsg->ulLBModeBTmrLen)
        {
            CDS_SET_TMR_LEN(pstCdsEntity,
                            CDS_TMR_ID_LB_MODE_B_TMR,
                            pstStartTLMsg->ulLBModeBTmrLen * 1000);
        }
        else
        {
            /*��ʱ��ʱ��Ϊ0����10ms����*/
            CDS_SET_TMR_LEN(pstCdsEntity,
                            CDS_TMR_ID_LB_MODE_B_TMR,
                            CDS_TMR_LEN);
        }
    }
    else
    {
        pstCdsEntity->ulLoopBackMode = CDS_LB_MODE_A;
        CDS_SET_TMR_LEN(pstCdsEntity,CDS_TMR_ID_LB_MODE_B_TMR,0);
    }

    /*���Service Request��־*/
    pstCdsEntity->ulServiceReqFlg = PS_FALSE;
    pstCdsEntity->ulLoopBackState = CDS_LB_STATE_START;

    /*��ջ��ض���*/
    CDS_ClearLoopBackQue(pstCdsEntity);

    CDS_INFO_LOG(UEPS_PID_CDS,"Leave CDS_StartLBTestLoopMsgProc");

    return;
}


VOS_VOID CDS_StopLBTestLoopMsgProc(const MsgBlock  *pstMsg)
{
    CDS_ENTITY_STRU                      *pstCdsEntity;

    /*�����Ч���ж�*/
    if (VOS_NULL_PTR == pstMsg)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_StopLBTestLoopMsgProc : Input Para Error.");
        return;
    }

    CDS_INFO_LOG(UEPS_PID_CDS,"Enter CDS_StopLBTestLoopMsgProc");

    /*����ModemId���CDSʵ��ָ�룬LBĬ�ϲ�����Modem0*/
    pstCdsEntity = CDS_GetCdsEntity(MODEM_ID_0);
    if (VOS_NULL_PTR == pstCdsEntity)
    {
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_StartLBTestLoopMsgProc : Get CDS_GetCdsEntity Fail.ModemId:",MODEM_ID_0);
        return ;
    }

    /*���LB��Ӧ��Ϣ*/
    pstCdsEntity->ulLoopBackMode = CDS_LB_MODE_BUTT;
    CDS_SET_TMR_LEN(pstCdsEntity,CDS_TMR_ID_LB_MODE_B_TMR,0);

    /*���Service Request��־*/
    pstCdsEntity->ulServiceReqFlg = PS_FALSE;
    pstCdsEntity->ulLoopBackState = CDS_LB_STATE_STOP;

    /*��ջ��ض���*/
    CDS_ClearLoopBackQue(pstCdsEntity);

    CDS_INFO_LOG(UEPS_PID_CDS,"Enter CDS_StopLBTestLoopMsgProc");
    return;
}
VOS_VOID CDS_ETC_MsgProc(const MsgBlock  *pstMsg)
{
    /*����ж�*/
    if (VOS_NULL_PTR == pstMsg)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_ETC_MsgProc : Input Para Error.");
        return;
    }

    switch(TTF_GET_MSG_NAME(pstMsg))
    {
    case ID_ETC_CDS_ACT_TEST_MODE_NOTIFY:
        CDS_ActLookBackMsgProc(pstMsg);
        break;

    case ID_ETC_CDS_DEACT_TEST_MODE_NOTIFY:
        CDS_DeactLookBackMsgProc(pstMsg);
        break;

    case ID_ETC_CDS_START_TEST_LOOP_NOTIFY:
        CDS_StartLBTestLoopMsgProc(pstMsg);
        break;

    case ID_ETC_CDS_STOP_TEST_LOOP_NOTIFY:
        CDS_StopLBTestLoopMsgProc(pstMsg);
        break;

    default:
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_ETC_MsgProc : Recv ETC Msg Id Error.",TTF_GET_MSG_NAME(pstMsg));
        break;
    }

    return;
}

#endif

/*****************************************************************************
 �� �� ��  : CDS_UMTS_TrigEvenTresMsgProc
 ��������  : ��ȡ��Ϣ�����е�ulDlTrigEventThres
 �������  :
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ  :
    1.��    ��   : 2012��6��14��
      ��    ��   : t148005
      �޸�����   : �����ɺ���
*****************************************************************************/
VOS_VOID CDS_UMTS_TrigEvenTresMsgProc(const MsgBlock  *pstMsg)
{
    (VOS_VOID)pstMsg;
    return;
}

/*****************************************************************************
 �� �� ��  : CDS_UMTS_TrigEvenTresMsgProc
 ��������  : FcACore��Ϣ������
 �������  :
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ  :
    1.��    ��   : 2012��6��14��
      ��    ��   : t148005
      �޸�����   : �����ɺ���
*****************************************************************************/
VOS_VOID CDS_FcACore_MsgProc(const MsgBlock  *pstMsg)
{
    if (VOS_NULL_PTR == pstMsg)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_FcA_MsgProc : Input Para Error.");
        return;
    }

    switch(TTF_GET_MSG_NAME(pstMsg))
    {
    case ID_FC_CDS_DL_THRES_CHG_IND:
        CDS_UMTS_TrigEvenTresMsgProc(pstMsg);
        break;

    default:
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_FcA_MsgProc : Recv FcACore Msg Id Error.",TTF_GET_MSG_NAME(pstMsg));
        break;
    }

    return;
}



VOS_VOID CDS_RecvMsgProc(MsgBlock  *pstMsg)
{
    if (VOS_NULL_PTR == pstMsg)
    {
        CDS_ERROR_LOG(UEPS_PID_CDS,"CDS_RecvMsgProc : Input Para NULL.");
        return;
    }

    switch(TTF_GET_MSG_SENDER_PID(pstMsg))
    {
    case I0_WUEPS_PID_MMC:
    case I1_WUEPS_PID_MMC:
        CDS_MSG_HOOK(pstMsg);
        CDS_MMC_MsgProc(pstMsg);
        break;

    case UEPS_PID_NDCLIENT:
        CDS_MSG_HOOK(pstMsg);
        CDS_NDCLIENT_MsgProc(pstMsg);
        break;

    case I0_WUEPS_PID_RABM:
    case I1_WUEPS_PID_RABM:
        CDS_MSG_HOOK(pstMsg);
        CDS_RABM_MsgProc(pstMsg);
        break;

    case DOPRA_PID_TIMER:
        CDS_TmrMsgProc(pstMsg);
        break;

    case ACPU_PID_FC:
        CDS_FcACore_MsgProc(pstMsg);
        break;

    case PS_PID_IMSA:
        CDS_MSG_HOOK(pstMsg);
        CDS_IMSA_MsgProc(pstMsg);
        break;

#if (CDS_FEATURE_ON == CDS_FEATURE_LTE)
    case PS_PID_RABM:
        CDS_MSG_HOOK(pstMsg);
        CDS_ERABM_MsgProc(pstMsg);
        break;

    case PS_PID_TC:
        CDS_MSG_HOOK(pstMsg);
        CDS_ETC_MsgProc(pstMsg);
        break;
#endif

    default:
        CDS_ERROR_LOG1(UEPS_PID_CDS,"CDS_RecvMsgProc : CDS Msg Proc Error : Unkown Sender.",TTF_GET_MSG_SENDER_PID(pstMsg));
        break;
    }

    return;
}



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif



