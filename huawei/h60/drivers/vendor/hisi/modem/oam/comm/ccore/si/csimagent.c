/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : csimagent.c
  �� �� ��   : ����
  ��    ��   :
  ��������   :
  ����޸�   :
  ��������   :
  �����б�   :
  �޸���ʷ   :

  1.��    ��   :
    ��    ��   :
    �޸�����   :
******************************************************************************/
/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "product_config.h"
#include "csimagent.h"
#include "CbpaPsInterface.h"
#include "AtCsimagent.h"
#include "TafNvInterface.h"
#include "siappstk.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_CSIMAGENT_C

TAF_NV_LC_CTRL_PARA_STRU                g_stLCEnableFlag;

#if (FEATURE_ON == FEATURE_CL_INTERWORK)
/*****************************************************************************
  2 ������������
*****************************************************************************/
CSIMA_CBP_STATUS_IND_MSG_STRU           g_stCBPCardStatus;
CSIMA_EVENTLIST_RESEND_ENUM_UINT32      g_enEventListReSendFlag = CSIMA_EVENTLIST_RESEND_INIT;


VOS_UINT32  CSIMA_UiccResAccRspMsg(PS_USIM_RESTRICTED_ACCESS_CNF_STRU  *pstMsg)
{
    VOS_UINT32                          ulResult;
    CSIMA_CBP_RESACC_RSP_MSG_STRU      *pstResAccRsp;
    VOS_UINT32                          ulMemLen;
    VOS_UINT8                           ucSW1 = pstMsg->ucSW1;
    VOS_UINT8                           ucSW2 = pstMsg->ucSW2;

    if (pstMsg->usLen >= CSIMA_CBP_MSG_DATA_RSV)
    {
        ulMemLen = sizeof(CSIMA_CBP_RESACC_RSP_MSG_STRU) + pstMsg->usLen - CSIMA_CBP_MSG_DATA_RSV;
    }
    else
    {
        ulMemLen = sizeof(CSIMA_CBP_RESACC_RSP_MSG_STRU);
    }

    pstResAccRsp = (CSIMA_CBP_RESACC_RSP_MSG_STRU *)VOS_MemAlloc(WUEPS_PID_CSIMA, DYNAMIC_MEM_PT,
                                               ulMemLen);

    if (VOS_NULL_PTR == pstResAccRsp)
    {
        CSIMA_ERROR_LOG("CSIMA_UiccResAccRspMsg: Alloc mem fail!");
        return VOS_ERR;
    }

    VOS_MemSet(pstResAccRsp, 0, ulMemLen);

    pstResAccRsp->enIPCMsgId = CSIMA_CBP_UICC_RESACC_RSP_MSG;

    if (VOS_OK != pstMsg->ulResult)
    {
        pstResAccRsp->usSWStatus = UIM_ERR_TIMEOUT;
        pstResAccRsp->usDataLen  = 0;
    }
    else
    {
        /* CBP���·�fetch�����˵����ظ�0x91XX��ʱ����Ҫת����0x9000 */
        if (0x91 == pstMsg->ucSW1)
        {
            ucSW1 = 0x90;
            ucSW2 = 0;
        }

        pstResAccRsp->usSWStatus = ucSW1;
        pstResAccRsp->usSWStatus = (VOS_UINT16)(pstResAccRsp->usSWStatus << 8) + ucSW2;

        pstResAccRsp->usDataLen  =  pstMsg->usLen;

        if (0 != pstResAccRsp->usDataLen)
        {
            VOS_MemCpy(pstResAccRsp->aucData, pstMsg->aucContent, pstMsg->usLen);
        }
    }

    /* ����CBPCAģ��ӿڽ����ݷ���ȥ */
    ulResult = CBPCA_SndDataToCbpca(WUEPS_PID_CSIMA,
                                    CSIMA_CBPCA_DATA_REQ,
                                    (VOS_UINT8 *)pstResAccRsp,
                                    ulMemLen);

    if (VOS_OK != ulResult)
    {
        CSIMA_ERROR_LOG("CSIMA_UiccResAccRspMsg: send RESACC RSP fail");
    }

    VOS_MemFree(WUEPS_PID_CSIMA, pstResAccRsp);

    return ulResult;
}


VOS_UINT32  CSIMA_UiccRtpduMsg(VOS_UINT32     ulResult,
                                         VOS_UINT8      ucSW1,
                                         VOS_UINT8      ucSW2,
                                         VOS_UINT8      ucDataLen,
                                         VOS_UINT8      *pucData)
{
    VOS_UINT32                          ulRslt;
    CSIMA_CBP_RTPDU_MSG_STRU           *pstRtpdu;
    VOS_UINT32                          ulMemLen;
    VOS_UINT8                           ucTmpSW1 = ucSW1;
    VOS_UINT8                           ucTmpSW2 = ucSW2;

    if (ucDataLen >= CSIMA_CBP_MSG_DATA_RSV)
    {
        ulMemLen = sizeof(CSIMA_CBP_RTPDU_MSG_STRU) + ucDataLen - CSIMA_CBP_MSG_DATA_RSV;
    }
    else
    {
        ulMemLen = sizeof(CSIMA_CBP_RTPDU_MSG_STRU);
    }

    pstRtpdu = (CSIMA_CBP_RTPDU_MSG_STRU *)VOS_MemAlloc(WUEPS_PID_CSIMA, DYNAMIC_MEM_PT,
                                               ulMemLen);

    if (VOS_NULL_PTR == pstRtpdu)
    {
        CSIMA_ERROR_LOG("CSIMA_UiccRtpduMsg: Alloc mem fail!");
        return VOS_ERR;
    }

    VOS_MemSet(pstRtpdu, 0, ulMemLen);

    pstRtpdu->enIPCMsgId = CSIMA_CBP_UICC_RTPDU_MSG;

    if (VOS_OK != ulResult)
    {
        pstRtpdu->usSWStatus = UIM_ERR_TIMEOUT;
        pstRtpdu->usDataLen  = 0;
    }
    else
    {
        /* CBP���·�fetch�����˵����ظ�0x91XX��ʱ����Ҫת����0x9000 */
        if (0x91 == ucSW1)
        {
            ucTmpSW1 = 0x90;
            ucTmpSW2 = 0;
        }

        pstRtpdu->usSWStatus = ucTmpSW1;
        pstRtpdu->usSWStatus = (VOS_UINT16)(pstRtpdu->usSWStatus << 8) + ucTmpSW2;

        pstRtpdu->usDataLen = ucDataLen;

        if (0 != ucDataLen)
        {
            VOS_MemCpy(pstRtpdu->aucData, pucData, ucDataLen);
        }
    }

    /* ����CBPCAģ��ӿڽ����ݷ���ȥ */
    ulRslt = CBPCA_SndDataToCbpca(WUEPS_PID_CSIMA,
                                    CSIMA_CBPCA_DATA_REQ,
                                    (VOS_UINT8 *)pstRtpdu,
                                    ulMemLen);

    if (VOS_OK != ulRslt)
    {
        CSIMA_ERROR_LOG("CSIMA_UiccRtpduMsg: send RTPDU fail");
    }

    VOS_MemFree(WUEPS_PID_CSIMA, pstRtpdu);

    if (CSIMA_EVENTLIST_NEED_RESEND == g_enEventListReSendFlag)
    {
        g_enEventListReSendFlag = CSIMA_EVENTLIST_RESENT;
        SI_STK_CbpSetupEventListCmdSnd();
    }

    return ulRslt;
}


VOS_UINT32 CSIMA_CBP_HandshakeReqMsg(VOS_VOID)
{
    VOS_UINT32                          ulRslt;
    CSIMA_CBP_UICC_MSGID_ENUM_UINT32    enIPCMsgId;

    enIPCMsgId = CSIMA_CBP_UICC_HANDSHAKE_MSG;

    /* ����CBPCAģ��ӿڽ����ݷ���ȥ */
    ulRslt = CBPCA_SndDataToCbpca(WUEPS_PID_CSIMA,
                                  CSIMA_CBPCA_DATA_REQ,
                                  (VOS_UINT8 *)&enIPCMsgId,
                                  sizeof(enIPCMsgId));

    if (VOS_OK != ulRslt)
    {
        CSIMA_ERROR_LOG("CSIMA_CBP_HandshakeReqMsg: send Data fail");
    }


    return ulRslt;
}
VOS_UINT32  CSIMA_UiccResAccReqMsg(CSIMA_CBP_RESACC_REQ_MSG_STRU *pstMsg)
{
    VOS_UINT32                          ulResult;
    TAF_SIM_RESTRIC_ACCESS_STRU         stResAcc;
    CSIMA_CBP_RESACC_RSP_MSG_STRU       stResAccRsp;
    VOS_UINT8                           i;
    VOS_UINT8                           *pucFilePath;

    VOS_MemSet(&stResAccRsp, 0, sizeof(stResAccRsp));
    VOS_MemSet(&stResAcc, 0, sizeof(stResAcc));

    stResAccRsp.enIPCMsgId = CSIMA_CBP_UICC_RESACC_RSP_MSG;

    stResAcc.usCmdType = pstMsg->ucCmd;
    stResAcc.usEfId    = pstMsg->usFileId;
    stResAcc.ucP1      = pstMsg->ucP1;
    stResAcc.ucP2      = pstMsg->ucP2;
    stResAcc.ucP3      = pstMsg->ucP3;
    stResAcc.usCmdLen  = pstMsg->ucLen;
    stResAcc.usPathLen = 0;

    if (0 != pstMsg->usPathLen % 2)
    {
        /* ��CBP�ظ�����ִ�д��� */
        stResAccRsp.usSWStatus = UIM_ERR_TIMEOUT;

        /* ����CBPCAģ��ӿڽ����ݷ���ȥ */
        ulResult = CBPCA_SndDataToCbpca(WUEPS_PID_CSIMA,
                                        CSIMA_CBPCA_DATA_REQ,
                                        (VOS_UINT8 *)&stResAccRsp,
                                        sizeof(stResAccRsp));
        return ulResult;
    }

    if (0 != pstMsg->ucLen)
    {
        VOS_MemCpy(stResAcc.aucCommand, pstMsg->aucData, pstMsg->ucLen);
    }

    if (pstMsg->usPathLen > 2)
    {
        stResAcc.usPathLen = (pstMsg->usPathLen - 2) /((VOS_UINT16)(sizeof(VOS_UINT16))); /* ��ȥ���ֽڵ��ļ�ID */
    }

    pucFilePath = &(pstMsg->aucData[pstMsg->ucLen]);

    for (i = 0; i < stResAcc.usPathLen; i++)
    {
        stResAcc.ausPath[i] = ((pucFilePath[i*2] << 0x08) & 0xFF00) + pucFilePath[(i*2)+1];
    }

    ulResult = USIMM_RestrictedAccessReq(WUEPS_PID_CSIMA, 0, &stResAcc);

    if (VOS_OK != ulResult)
    {
        /* ��CBP�ظ�����ִ�д��� */
        stResAccRsp.usSWStatus = UIM_ERR_TIMEOUT;

        /* ����CBPCAģ��ӿڽ����ݷ���ȥ */
        ulResult = CBPCA_SndDataToCbpca(WUEPS_PID_CSIMA,
                                        CSIMA_CBPCA_DATA_REQ,
                                        (VOS_UINT8 *)&stResAccRsp,
                                        sizeof(stResAccRsp));
    }

    return ulResult;
}
VOS_UINT32  CSIMA_GetUiccAidAndChanNum(USIMM_AID_TYPE_ENUM_UINT32 enAidType,
                                                      VOS_UINT8 *pucAid, VOS_UINT8 *pucChanNum)
{
    VOS_UINT32                          ulResult;
    VOS_UINT32                          ulAIDLen = 0;

    ulResult = USIMM_GetAid(enAidType, &ulAIDLen, pucAid);

    if (VOS_OK != ulResult)
    {
        return ulResult;
    }

    ulResult = USIMM_GetUiccChanNumByAid(pucAid, ulAIDLen, pucChanNum);

    return ulResult;
}



VOS_UINT32 CSIMA_CBPEnvelopeSend(CSIMA_CBP_CTPDU_MSG_STRU *pstCtpduMsg)
{
    VOS_UINT32                          ulResult;
    CSIMA_CBP_RTPDU_MSG_STRU            stRtpdu;

    VOS_MemSet(&stRtpdu, VOS_NULL, sizeof(stRtpdu));

    stRtpdu.enIPCMsgId = CSIMA_CBP_UICC_RTPDU_MSG;

    if ((SI_STK_ENVELOPE_EVENTDOWN == pstCtpduMsg->aucData[0])
        && (TAF_NVIM_LC_RAT_COMBINED_CL != g_stLCEnableFlag.enRatCombined))
    {
       /* ��CBP�ظ�����ִ�гɹ� */
       stRtpdu.usSWStatus = 0x9000;

       /* ����CBPCAģ��ӿڽ����ݷ���ȥ */
       ulResult = CBPCA_SndDataToCbpca(WUEPS_PID_CSIMA,
                                       CSIMA_CBPCA_DATA_REQ,
                                       (VOS_UINT8 *)&stRtpdu,
                                       sizeof(stRtpdu));
        return ulResult;
    }

    ulResult = CSIMA_SendTPDUOnBaseChan(pstCtpduMsg);

    if (USIMM_API_SUCCESS != ulResult)
    {
        CSIMA_WARNING_LOG("CSIMA_CBPEnvelopeSend: CSIMA_SendTPDUOnBaseChan is Fail");
    }

    return ulResult;
}


VOS_UINT32 CSIMA_SendTPDUOnBaseChan(CSIMA_CBP_CTPDU_MSG_STRU *pstCtpduMsg)
{
    CSIMA_CBP_RTPDU_MSG_STRU            stRtpdu;
    VOS_UINT32                          ulResult;
    USIMM_TPDU_DATA_STRU                stTPDU;

    VOS_MemSet(&stRtpdu, VOS_NULL, sizeof(stRtpdu));

    VOS_MemSet(&stTPDU, VOS_NULL, sizeof(stTPDU));

    stRtpdu.enIPCMsgId = CSIMA_CBP_UICC_RTPDU_MSG;

    /* ������� */
    if (g_stCBPCardStatus.ucCSIMChanNum != pstCtpduMsg->ucChanNum)
    {
       /* ��CBP�ظ�����ִ�д��� */
       stRtpdu.usSWStatus = UIM_ERR_TIMEOUT;

       /* ����CBPCAģ��ӿڽ����ݷ���ȥ */
       ulResult = CBPCA_SndDataToCbpca(WUEPS_PID_CSIMA,
                                       CSIMA_CBPCA_DATA_REQ,
                                       (VOS_UINT8 *)&stRtpdu,
                                       sizeof(stRtpdu));

        return ulResult;
    }

    VOS_MemCpy(stTPDU.aucTPDUHead, pstCtpduMsg->aucCTPDUHdr, USIMM_TPDU_HEAD_LEN);

    stTPDU.ulDataLen = pstCtpduMsg->usDataLen;
    stTPDU.pucData   = pstCtpduMsg->aucData;

    ulResult = USIMM_SendTPDUReq(WUEPS_PID_CSIMA, VOS_NULL, VOS_NULL_PTR, &stTPDU);

    if (VOS_OK != ulResult)
    {
        /* ��CBP�ظ�����ִ�д��� */
        stRtpdu.usSWStatus = UIM_ERR_TIMEOUT;

        /* ����CBPCAģ��ӿڽ����ݷ���ȥ */
        ulResult = CBPCA_SndDataToCbpca(WUEPS_PID_CSIMA,
                                        CSIMA_CBPCA_DATA_REQ,
                                        (VOS_UINT8 *)&stRtpdu,
                                        sizeof(stRtpdu));
    }

    return ulResult;
}

VOS_UINT32 CSIMA_SendTPDUOnLogicChan(CSIMA_CBP_CTPDU_MSG_STRU *pstCtpduMsg)
{
    VOS_UINT32                          ulResult;
    VOS_UINT32                          ulSessionID;
    CSIMA_CBP_RTPDU_MSG_STRU            stRtpdu;
    VOS_UINT8                           ucChanNum;
    VOS_UINT8                           aucAid[USIMM_AID_LEN_MAX];
    VOS_UINT8                           aucData[256]={0};
    VOS_UINT32                          ulDataLen;

    VOS_MemSet(&stRtpdu, 0, sizeof(stRtpdu));

    stRtpdu.enIPCMsgId = CSIMA_CBP_UICC_RTPDU_MSG;

    /* ��ȡCSIM��AID��ͨ���� */
    ulResult = CSIMA_GetUiccAidAndChanNum(USIMM_AID_TYPE_CSIM, aucAid, &ucChanNum);

    if (VOS_OK != ulResult)
    {
        /* ��CBP�ظ�����ִ�д��� */
        stRtpdu.usSWStatus = UIM_ERR_TIMEOUT;

        /* ����CBPCAģ��ӿڽ����ݷ���ȥ */
        ulResult = CBPCA_SndDataToCbpca(WUEPS_PID_CSIMA,
                                        CSIMA_CBPCA_DATA_REQ,
                                        (VOS_UINT8 *)&stRtpdu,
                                        sizeof(stRtpdu));

        return ulResult;
    }

    /* ������� */
    if (ucChanNum != pstCtpduMsg->ucChanNum)
    {
       /* ��CBP�ظ�����ִ�д��� */
       stRtpdu.usSWStatus = UIM_ERR_TIMEOUT;

       /* ����CBPCAģ��ӿڽ����ݷ���ȥ */
       ulResult = CBPCA_SndDataToCbpca(WUEPS_PID_CSIMA,
                                       CSIMA_CBPCA_DATA_REQ,
                                       (VOS_UINT8 *)&stRtpdu,
                                       sizeof(stRtpdu));


        return ulResult;
    }

    /* ͨ��ͨ���Ż�ȡsessionID */
    USIMM_ChanNumToSessionId(pstCtpduMsg->ucChanNum, &ulSessionID);

    VOS_MemCpy(aucData, pstCtpduMsg->aucCTPDUHdr, USIMM_APDU_HEADLEN);

    VOS_MemCpy(aucData+USIMM_APDU_HEADLEN, pstCtpduMsg->aucData, pstCtpduMsg->usDataLen);

    ulDataLen = USIMM_APDU_HEADLEN + pstCtpduMsg->usDataLen;

    ulResult = USIMM_AccessChannelReq(WUEPS_PID_CSIMA, ulSessionID, ulDataLen, aucData);

    if (VOS_OK != ulResult)
    {
        /* ��CBP�ظ�����ִ�д��� */
        stRtpdu.usSWStatus = UIM_ERR_TIMEOUT;

        /* ����CBPCAģ��ӿڽ����ݷ���ȥ */
        ulResult = CBPCA_SndDataToCbpca(WUEPS_PID_CSIMA,
                                        CSIMA_CBPCA_DATA_REQ,
                                        (VOS_UINT8 *)&stRtpdu,
                                        sizeof(stRtpdu));

    }

    return ulResult;
}
VOS_UINT32  CSIMA_UiccCtpduMsg(CSIMA_CBP_CTPDU_MSG_STRU *pstCtpduMsg)
{
    VOS_UINT32                          ulResult;

    /* EVENLOP����ͨ������ͨ���·� */
    if (CMD_INS_ENVELOPE == pstCtpduMsg->aucCTPDUHdr[INS])
    {
        ulResult = CSIMA_CBPEnvelopeSend(pstCtpduMsg);
    }
    /********* ��ENVELOP����ͨ��CGLA����TPDU�·���ȥ *********/
    else if (0 == g_stCBPCardStatus.ucCSIMChanNum)
    {
        ulResult = CSIMA_SendTPDUOnBaseChan(pstCtpduMsg);
    }
    else
    {
        ulResult = CSIMA_SendTPDUOnLogicChan(pstCtpduMsg);
    }

    return ulResult;
}


VOS_UINT32 CSIMA_CBP_TerminalResponseMsgProc(CSIMA_CBP_TERMINALRESPONSE_MSG_STRU *pstMsg)
{
    VOS_UINT32                          ulRslt;
    VOS_UINT8                           ucCardStatus;
    VOS_UINT8                           ucCardType;

    USIMM_GetCardType(&ucCardStatus, &ucCardType);

    /* �޿�ʱֱ�ӷ���USIMMģ�鴦�� */
    if (USIMM_CARD_NOCARD == ucCardType)
    {
        ulRslt = USIMM_CbpTerminalResponseReq(WUEPS_PID_CSIMA, (VOS_UINT8)pstMsg->usDataLen, pstMsg->aucData);
    }
    /* �п�ʱֱ�ӷ���STKģ�鴦����Ϊ��Ҫ���ͷ�STK��������ж�̬������ڴ� */
    else
    {
        ulRslt = SI_STK_CLSndTrMsg(pstMsg->usDataLen, pstMsg->aucData);
    }

    return ulRslt;
}


VOS_UINT32  CSIMA_CBPCADataIndProc(CBPCA_DATA_IND_MSG_STRU *pstMsg)
{
    VOS_UINT32                          *pulMsgId;
    VOS_UINT32                          ulResult = VOS_ERR;
    CSIMA_CBP_TERMINALRESPONSE_MSG_STRU *pstTR;

    pulMsgId = (VOS_UINT32 *)pstMsg->aucData;

    if (CSIMA_CBP_UICC_TERMINALRESPONSE_MSG == *pulMsgId)
    {
        pstTR = (CSIMA_CBP_TERMINALRESPONSE_MSG_STRU *)(pstMsg->aucData);

        /* �����ݷ��͵�STK��USIMMģ�鴦�� */
        ulResult = CSIMA_CBP_TerminalResponseMsgProc(pstTR);

        return ulResult;
    }

    if (CSIMA_CBP_UICC_RST_REQ_MSG == *pulMsgId)
    {
        /* ��CBP�����²�������RESET���� */
        if (TAF_NVIM_LC_RAT_COMBINED_CL != g_stLCEnableFlag.enRatCombined)
        {
            return VOS_OK;
        }

        /* ��RESET����USIMMģ�鴦�� */
        ulResult = USIMM_ActiveCardReq(WUEPS_PID_CSIMA);

        return ulResult;
    }

    if (CSIMA_CBP_UICC_CTPDU_MSG == *pulMsgId)
    {
        /* CBP����csimӦ�� */
        ulResult = CSIMA_UiccCtpduMsg((CSIMA_CBP_CTPDU_MSG_STRU *)(pstMsg->aucData));

        return ulResult;
    }

    if (CSIMA_CBP_UICC_RESACC_REQ_MSG == *pulMsgId)
    {
        /* CBP����usimӦ�� */
        ulResult = CSIMA_UiccResAccReqMsg((CSIMA_CBP_RESACC_REQ_MSG_STRU *)(pstMsg->aucData));

        return ulResult;
    }

    /* ���ִ��� */
    if (CSIMA_CBP_UICC_HANDSHAKE_MSG == *pulMsgId)
    {
        ulResult = CSIMA_CBP_HandshakeReqMsg();

        return ulResult;
    }

    /* ��¼���� */

    return ulResult;
}
VOS_UINT32  CSIMA_CBPCAMsgProc(CSIMA_CBP_MSG_STRU *pstMsg)
{
    VOS_UINT32                          ulResult;

    switch (pstMsg->ulIPCMsgId)
    {
        case CBPCA_CSIMA_DATA_IND:
            ulResult = CSIMA_CBPCADataIndProc((CBPCA_DATA_IND_MSG_STRU *)pstMsg);
            break;
        default:
            ulResult = VOS_ERR;
            break;
    }

    return ulResult;
}


VOS_UINT32  CSIMA_OpenChanCnfProc(PS_USIM_OPEN_CHANNEL_CNF_STRU *pstMsg)
{
    VOS_UINT32                          ulResult;
    VOS_UINT32                          ulChanNum;
    VOS_UINT16                          usCardType = 0;

    /* ��ͨ��ʧ�ܣ��ϱ���״̬������ */
    if (VOS_OK != pstMsg->ulResult)
    {
        CSIMA_CardStatusMsgSend((VOS_UINT16)CSIMA_CBP_CARD_NOT_READY, ((VOS_UINT16)CSIMA_CBP_USIM_CARD |(VOS_UINT16)CSIMA_CBP_UICC_CARD));

        return VOS_ERR;
    }

    /* ����sessionID��ȡͨ���� */
    USIMM_SessionIdToChanNum(pstMsg->ulSessionId, &ulChanNum);

    /* ͨ���ŷǷ� */
    if (USIMM_CHANNEL_INVALID_NUM == ulChanNum)
    {
        /* �ϱ���״̬������ */
        CSIMA_CardStatusMsgSend((VOS_UINT16)CSIMA_CBP_CARD_READY, ((VOS_UINT16)CSIMA_CBP_USIM_CARD |(VOS_UINT16)CSIMA_CBP_UICC_CARD));

        return VOS_ERR;
    }

    ulResult = CSIMA_GetUiccAidAndChanNum(USIMM_AID_TYPE_CSIM,
                                          g_stCBPCardStatus.aucCSIMAID,
                                          &g_stCBPCardStatus.ucCSIMChanNum);

    /* ���û��CSIM��Ӧ�� */
    if (VOS_OK != ulResult)
    {
        /* �ϱ���״̬������ */
        CSIMA_CardStatusMsgSend((VOS_UINT16)CSIMA_CBP_CARD_READY, ((VOS_UINT16)CSIMA_CBP_USIM_CARD |(VOS_UINT16)CSIMA_CBP_UICC_CARD));

        return VOS_ERR;
    }

    /* �õ�USIMӦ��*/
    ulResult = CSIMA_GetUiccAidAndChanNum(USIMM_AID_TYPE_USIM,
                                          g_stCBPCardStatus.aucUSIMAID,
                                          &g_stCBPCardStatus.ucUSIMChanNum);

    /* ���û��USIM��Ӧ�� */
    if (VOS_OK != ulResult)
    {
        /* �ϱ���״̬������ */
        CSIMA_CardStatusMsgSend((VOS_UINT16)CSIMA_CBP_CARD_NOT_READY, (VOS_UINT16)CSIMA_CBP_UNKNOWN_CARD);

        return VOS_ERR;
    }

    usCardType = ((VOS_UINT16)CSIMA_CBP_UICC_CARD | (VOS_UINT16)CSIMA_CBP_USIM_CARD| (VOS_UINT16)CSIMA_CBP_CSIM_CARD);

    ulResult = CSIMA_GetUiccAidAndChanNum(USIMM_AID_TYPE_ISIM,
                                          g_stCBPCardStatus.aucISIMAID,
                                          &g_stCBPCardStatus.ucISIMChanNum);

    /* ���û��ISIM��Ӧ�� */
    if (VOS_OK != ulResult)
    {
        /* ISIM�����õĻ���0xff���� */
        VOS_MemSet(g_stCBPCardStatus.aucISIMAID, (VOS_CHAR)VOS_NULL_BYTE, sizeof(g_stCBPCardStatus.aucISIMAID));

        g_stCBPCardStatus.ucISIMChanNum = CSIMA_CARD_CHANEL_NULL;
    }
    else
    {
        usCardType = usCardType | CSIMA_CBP_ISIM_CARD;
    }

    /* ����CBPCAģ��ӿڽ����ݷ���ȥ */
    CSIMA_CardStatusMsgSend((VOS_UINT16)CSIMA_CBP_CARD_READY, usCardType);

    return VOS_OK;
}

/*****************************************************************************
�� �� ��  : CSIMA_SendTPDUCnfMsg
��������  : ����USIMģ�鷵�ص�TPDU���
�������  : pstMsg -- ָ��TPDU�������ݽṹ
�������  : ��
�� �� ֵ  : VOS_OK/VOS_ERR
History     :
1.��    ��  : 2014��01��16��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/

VOS_UINT32 CSIMA_SendTPDUCnfMsg(PS_USIM_SENDTPDU_CNF_STRU *pstMsg)
{
    VOS_UINT32              ulResult = VOS_OK;

    if (CSIMA_CBP_CARD_STATUS_BUTT == g_stCBPCardStatus.enStatus)   /*��һ���ϱ�*/
    {
        if(pstMsg->aucTPDUHead[INS] != CMD_INS_SELECT)
        {
            CSIMA_WARNING_LOG("CSIMA_SendTPDUCnfMsg: The TPDU Cnf is Error");

            return VOS_ERR;
        }

        if((VOS_OK == pstMsg->ulResult)
            &&(((0x90 == pstMsg->ucSw1)&&(0x00 == pstMsg->ucSw2))||(0x91 == pstMsg->ucSw1)))
        {
            if(USIMM_SIM_CLA == pstMsg->aucTPDUHead[CLA])
            {
                g_stCBPCardStatus.ucCSIMChanNum = 0;

                CSIMA_CardStatusMsgSend(CSIMA_CBP_CARD_READY, CSIMA_CBP_UIM_CARD);
            }
            else
            {
                g_stCBPCardStatus.ucCSIMChanNum = (pstMsg->aucTPDUHead[CLA]&0x1F);

                CSIMA_CardStatusMsgSend(CSIMA_CBP_CARD_READY, ((VOS_UINT16)CSIMA_CBP_CSIM_CARD|(VOS_UINT16)CSIMA_CBP_UICC_CARD));
            }
        }
        else
        {
            CSIMA_CardStatusMsgSend(CSIMA_CBP_CARD_NOT_READY, CSIMA_CBP_UNKNOWN_CARD);
        }
    }
    else
    {
        CSIMA_NORMAL_LOG("CSIMA_SendTPDUCnfMsg: The TPDU Cnf send to CBP");

        ulResult = CSIMA_UiccRtpduMsg(pstMsg->ulResult, pstMsg->ucSw1, pstMsg->ucSw2, (VOS_UINT8)pstMsg->ulLen, pstMsg->aucContent);
    }

    return ulResult;
}
VOS_UINT32  CSIMA_UsimMsgProc(CSIMA_CBP_MSG_STRU *pstMsg)
{
    VOS_UINT32                          ulResult;
    PS_USIM_ENVELOPE_CNF_STRU          *pstEnvelopCnf;
    PS_USIM_ACCESS_CHANNEL_CNF_STRU    *pstAccChanCnf;

    switch (pstMsg->ulIPCMsgId)
    {
        case PS_USIM_RESTRICTED_ACCESS_CNF:
            ulResult = CSIMA_UiccResAccRspMsg((PS_USIM_RESTRICTED_ACCESS_CNF_STRU*)pstMsg);
            break;

        case PS_USIMM_OPENCHANNEL_CNF:
            ulResult = CSIMA_OpenChanCnfProc((PS_USIM_OPEN_CHANNEL_CNF_STRU *)pstMsg);
            break;

        case PS_USIMM_ACCESSCHANNEL_CNF:
            pstAccChanCnf = (PS_USIM_ACCESS_CHANNEL_CNF_STRU *)pstMsg;
            ulResult = CSIMA_UiccRtpduMsg(pstAccChanCnf->ulResult, pstAccChanCnf->ucSw1,
                pstAccChanCnf->ucSw2, (VOS_UINT8)pstAccChanCnf->usLen, pstAccChanCnf->aucContent);
            break;

        case PS_USIM_ENVELOPE_CNF:
            pstEnvelopCnf = (PS_USIM_ENVELOPE_CNF_STRU *)pstMsg;

            ulResult = CSIMA_UiccRtpduMsg(pstEnvelopCnf->ulResult, pstEnvelopCnf->ucSW1,
                pstEnvelopCnf->ucSW2, pstEnvelopCnf->ucDataLen, pstEnvelopCnf->aucData);
            break;

        /* ���¿�֮���ȫ�ֱ������ */
        case PS_USIMM_ACTIVE_CNF:
            ulResult = CSIMA_Init();
            break;

        case PS_USIM_SENDTPDU_CNF:
            ulResult = CSIMA_SendTPDUCnfMsg((PS_USIM_SENDTPDU_CNF_STRU *)pstMsg);
            break;

        case PS_USIM_TERMINALRESPONSE_CNF:
            ulResult = CSIMA_UiccRtpduMsg(((PS_USIM_TERMINALRESPONSE_CNF_STRU *)pstMsg)->ulResult,
                                          ((PS_USIM_TERMINALRESPONSE_CNF_STRU *)pstMsg)->ucSW1,
                                          ((PS_USIM_TERMINALRESPONSE_CNF_STRU *)pstMsg)->ucSW2,
                                          VOS_NULL,
                                          VOS_NULL_PTR);
            break;

        default:
            /* ��ӡ���� */
            ulResult = VOS_ERR;
            break;
    }

    return ulResult;
}


VOS_VOID CSIMA_CBPResetInd(CSIMA_CBPCA_MODEM_RESET_ENUM_UINT32 enModemReset)
{
    CSIMA_CBPCA_MODEM_RESET_IND_MSG_STRU    *pstResetInd;

    pstResetInd = (CSIMA_CBPCA_MODEM_RESET_IND_MSG_STRU *)VOS_AllocMsg(WUEPS_PID_CSIMA,
                                                                       sizeof(CSIMA_CBPCA_MODEM_RESET_IND_MSG_STRU));

    if (VOS_NULL_PTR == pstResetInd)
    {
        CSIMA_ERROR_LOG("CSIMA_CBPResetInd: Alloc msg fail!");

        return;
    }

    /* �����Ϣ���� */
    pstResetInd->ulReceiverPid      = WUEPS_PID_CBPCA;
    pstResetInd->enMsgType          = CSIMA_CBPCA_MODEM_RESET_IND;
    pstResetInd->enModemReset       = enModemReset;

    if (VOS_OK != VOS_SendMsg(WUEPS_PID_CSIMA, pstResetInd))
    {
        CSIMA_ERROR_LOG("CSIMA_CBPResetInd: Send msg fail!");

        return;
    }

    return;
}
VOS_UINT32  CSIMA_ATResetIndProc(AT_CSIMA_RESET_STATUS_IND_MSG_STRU *pstMsg)
{
    VOS_UINT32                          ulResult;

    /* �յ�CBP��λָʾ��Ҫ͸����CBPCA */
    CSIMA_CBPResetInd(pstMsg->enVIAModemStatus);

    /* ��λ�ɹ���CBP�ϱ���״̬ */
    if (CBP_MODEM_RESET_SUCC == pstMsg->enVIAModemStatus)
    {
        /* δ�ϱ�����״̬�Ļ������PIH�ϱ��˿�״̬���ϱ� */
        if (CSIMA_CBP_CARD_STATUS_BUTT == g_stCBPCardStatus.enStatus)
        {
            CSIMA_INFO_LOG("CSIMA_ATResetIndProc: have no card status Ind");

            return VOS_OK;
        }

        /* ����CBPCAģ��ӿڽ����ݷ���ȥ */
        ulResult = CBPCA_SndDataToCbpca(WUEPS_PID_CSIMA,
                                    CSIMA_CBPCA_DATA_REQ,
                                    (VOS_UINT8 *)&g_stCBPCardStatus,
                                    sizeof(g_stCBPCardStatus));

        return ulResult;
    }

    return VOS_OK;
}


VOS_UINT32  CSIMA_ATMsgProc(CSIMA_CBP_MSG_STRU *pstMsg)
{
    VOS_UINT32                          ulResult = VOS_ERR;

    switch (pstMsg->ulIPCMsgId)
    {
        case AT_CSIMA_RESET_IND_MSG:
            ulResult = CSIMA_ATResetIndProc((AT_CSIMA_RESET_STATUS_IND_MSG_STRU *)pstMsg);
            break;

        default:
            ulResult = VOS_ERR;
            break;
    }

    return ulResult;
}


VOS_UINT32  CSIMA_ProactiveIndProc(CSIMA_STK_PROACTIVE_CMD_MSG_STRU *pstMsg)
{
    VOS_UINT32                          ulResult;
    CSIMA_CBP_PROACTIVE_CMD_MSG_STRU    stProactiveCmd;

    VOS_MemSet(&stProactiveCmd, 0, sizeof(stProactiveCmd));

    stProactiveCmd.enIPCMsgId = CSIMA_CBP_UICC_PROACTIVE_CMD_MSG;
    stProactiveCmd.usDataLen  = (VOS_UINT16)pstMsg->ulLen;

    if (0 != stProactiveCmd.usDataLen)
    {
        VOS_MemCpy(stProactiveCmd.aucData, pstMsg->aucProactiveCmd, stProactiveCmd.usDataLen);
    }

    ulResult = CBPCA_SndDataToCbpca(WUEPS_PID_CSIMA,
                                    CSIMA_CBPCA_DATA_REQ,
                                    (VOS_UINT8 *)&stProactiveCmd,
                                    (sizeof(CSIMA_CBP_PROACTIVE_CMD_MSG_STRU) - STK_PROACTIVE_DATA_LEN + stProactiveCmd.usDataLen));

    return ulResult;
}


VOS_UINT32  CSIMA_STKMsgProc(CSIMA_CBP_MSG_STRU *pstMsg)
{
    VOS_UINT32                          ulResult = VOS_ERR;

    switch (pstMsg->ulIPCMsgId)
    {
        case CSIMA_CBP_UICC_PROACTIVE_CMD_MSG:
            /* ��������ֱ�ӱ���CBPCAģ�� */
            ulResult = CSIMA_ProactiveIndProc((CSIMA_STK_PROACTIVE_CMD_MSG_STRU *)pstMsg);
            break;

        default:
            ulResult = VOS_ERR;
            break;
    }

    return ulResult;
}

/*****************************************************************************
�� �� ��  : CSIMA_OpenCSIMOnLogicChannel
��������  : CSIMAģ���CSIM���߼�ͨ��
�������  : ulAIDLen -- CSIM AID����
            pucAID -- CSIM AID
�������  : ��
�� �� ֵ  : ��
History     :
1.��    ��  : 2014��01��16��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/

VOS_VOID CSIMA_OpenCSIMOnLogicChannel(VOS_UINT32 ulAIDLen, VOS_UINT8 *pucAID)
{
    VOS_UINT32                          ulResult;

    if (g_stCBPCardStatus.ucCSIMChanNum != CSIMA_CARD_CHANEL_NULL)  /*֮ǰ��ͨ��*/
    {
        CSIMA_CardStatusMsgSend(CSIMA_CBP_CARD_READY, g_stCBPCardStatus.usCardType);
    }
    else
    {
        /* ����USIM API���߼�ͨ�� */
        ulResult = USIMM_OpenChannelReq(WUEPS_PID_CSIMA, ulAIDLen, pucAID);

        if (USIMM_API_SUCCESS != ulResult)
        {
            /* �ϱ���״̬������ */
            CSIMA_CardStatusMsgSend(CSIMA_CBP_CARD_NOT_READY, CSIMA_CBP_UNKNOWN_CARD);
        }
    }

    return;
}

/*****************************************************************************
�� �� ��  : CSIMA_OpenCSIMOnBaseChannel
��������  : CSIMAģ���CSIM�Ļ����߼�ͨ��
�������  : ulAIDLen -- CSIM AID����
            pucAID -- CSIM AID
�������  : ��
�� �� ֵ  : ��
History     :
1.��    ��  : 2014��01��16��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/

VOS_VOID CSIMA_OpenCSIMOnBaseChannel(VOS_UINT32 ulAIDLen, VOS_UINT8 *pucAID)
{
    VOS_UINT32                          ulResult;
    USIMM_TPDU_DATA_STRU                stTPDU;

    /*����Э���·�TPDU����*/
    stTPDU.aucTPDUHead[CLA] = 0x00;
    stTPDU.aucTPDUHead[INS] = 0xA4;
    stTPDU.aucTPDUHead[P1]  = 0x04;
    stTPDU.aucTPDUHead[P2]  = 0x04;
    stTPDU.aucTPDUHead[P3]  = (VOS_UINT8)ulAIDLen;
    stTPDU.pucData          = pucAID;
    stTPDU.ulDataLen        = ulAIDLen;

    /* ����USIM API���·����� */
    ulResult = USIMM_SendTPDUReq(WUEPS_PID_CSIMA, 0, VOS_NULL_PTR, &stTPDU);

    if (VOS_OK != ulResult)
    {
        /* �ϱ���״̬������ */
        CSIMA_CardStatusMsgSend(CSIMA_CBP_CARD_NOT_READY, CSIMA_CBP_UNKNOWN_CARD);
    }

    return;
}

/*****************************************************************************
�� �� ��  : CSIMA_SelectDFCdma
��������  : CSIMAģ���UIM��DF
�������  : ��
�������  : ��
�� �� ֵ  : ��
History     :
1.��    ��  : 2014��01��16��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/

VOS_VOID CSIMA_SelectDFCdma(VOS_VOID)
{
    VOS_UINT32                          ulResult;
    USIMM_TPDU_DATA_STRU                stTPDU;
    VOS_UINT8                           aucData[2] = {0x7F, 0x25};

    /*����Э����д����*/
    stTPDU.aucTPDUHead[CLA]= USIMM_SIM_CLA;
    stTPDU.aucTPDUHead[INS]= CMD_INS_SELECT;
    stTPDU.aucTPDUHead[P1] = 0x00;
    stTPDU.aucTPDUHead[P2] = 0x00;
    stTPDU.aucTPDUHead[P3] = sizeof(aucData);

    stTPDU.pucData = aucData;

    stTPDU.ulDataLen = sizeof(aucData);

    /* ����USIM API���·����� */
    ulResult = USIMM_SendTPDUReq(WUEPS_PID_CSIMA, 0, VOS_NULL_PTR, &stTPDU);

    if (VOS_OK != ulResult)
    {
        /* �ϱ���״̬������ */
        CSIMA_CardStatusMsgSend(CSIMA_CBP_CARD_NOT_READY, CSIMA_CBP_UNKNOWN_CARD);
    }

    return;
}

/*****************************************************************************
�� �� ��  : CSIMA_CardStatusMsgSend
��������  : CSIMAģ���ϱ���״̬��CBP
�������  : enCardStatus:��״̬
            enCardType:������
�������  : ��
�� �� ֵ  : ��
History     :
1.��    ��  : 2014��01��16��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/

VOS_VOID CSIMA_CardStatusMsgSend(CSIMA_CBP_CARD_STATUS_ENUM_UINT16 enCardStatus,
                                              CSIMA_CBP_CARD_TYPE_ENUM_UINT16   enCardType)
{
    VOS_UINT32                          ulResult;

    g_stCBPCardStatus.enStatus      = enCardStatus;

    g_stCBPCardStatus.usCardType    = enCardType;

    /* ����CBPCAģ��ӿڽ����ݷ���ȥ */
    ulResult = CBPCA_SndDataToCbpca(WUEPS_PID_CSIMA,
                                    CSIMA_CBPCA_DATA_REQ,
                                    (VOS_UINT8 *)&g_stCBPCardStatus,
                                    sizeof(g_stCBPCardStatus));

    if (VOS_OK != ulResult)
    {
        CSIMA_ERROR_LOG("CSIMA_CardStatusMsgSend: Send Msg To CBPCA fail");
    }

    if (CSIMA_CBP_CARD_READY == enCardStatus)
    {
        g_enEventListReSendFlag = CSIMA_EVENTLIST_NEED_RESEND;
    }

    return;
}

/*****************************************************************************
�� �� ��  : CSIMA_FirstCardStatusInd
��������  : CSIMAģ���״��յ���״̬��Ϣ
�������  : pstMsg:��״̬��Ϣ
�������  : ��
�� �� ֵ  : ��
History     :
1.��    ��  : 2014��01��16��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/

VOS_VOID CSIMA_FirstCardStatusInd(PS_USIM_STATUS_IND_STRU *pstMsg)
{
    VOS_UINT32                          ulCSIMExistFlag;
    VOS_UINT8                           aucCsimAid[USIMM_AID_LEN_MAX];
    VOS_UINT32                          ulAidLen;

    ulCSIMExistFlag = USIMM_GetAid(USIMM_AID_TYPE_CSIM, &ulAidLen, aucCsimAid);

    if (USIMM_CARD_NOCARD == pstMsg->enCardType)    /*�����޿������ǵ�CSIM������UIM*/
    {
        if (VOS_OK == ulCSIMExistFlag)  /*CSIM���ڣ��ڻ���ͨ����Ӧ��*/
        {
            CSIMA_OpenCSIMOnBaseChannel(ulAidLen, aucCsimAid);
        }
        else                            /*CSIM�����ڣ����԰���UIMѡ��*/
        {
            CSIMA_SelectDFCdma();
        }
    }
    else if (USIMM_CARD_USIM == pstMsg->enCardType) /*������UICC�����ǵ�USIM*/
    {
        if (VOS_OK == ulCSIMExistFlag)  /*CSIM���ڣ����߼�ͨ����Ӧ��*/
        {
            if (USIMM_CARD_SERVIC_AVAILABLE != pstMsg->enCardStatus)
            {
                /* �ϱ���״̬��ʼ���� */
                CSIMA_CardStatusMsgSend(CSIMA_CBP_CARD_BOOTUP, ((VOS_UINT16)CSIMA_CBP_UICC_CARD|(VOS_UINT16)CSIMA_CBP_USIM_CARD|(VOS_UINT16)CSIMA_CBP_CSIM_CARD));
            }
            else
            {
                CSIMA_OpenCSIMOnLogicChannel(ulAidLen, aucCsimAid);
            }
        }
        else                            /*CSIM�����ڣ��ϱ��޿�*/
        {
            /* �ϱ���״̬������ */
            CSIMA_CardStatusMsgSend(CSIMA_CBP_CARD_NOT_READY, ((VOS_UINT16)CSIMA_CBP_UICC_CARD|(VOS_UINT16)CSIMA_CBP_USIM_CARD));

            USIMM_DeactiveCardReq(WUEPS_PID_CSIMA);
        }
    }
    else if (USIMM_CARD_SIM == pstMsg->enCardType)  /*SIM����Ӧ���ǹ��ʿ�����GUL��ʼ�����ϱ��޿�*/
    {
        /* �ϱ���״̬������ */
        CSIMA_CardStatusMsgSend(CSIMA_CBP_CARD_NOT_READY, CSIMA_CBP_SIM_CARD);
    }
    else                                            /*����������*/
    {
        /* �ϱ���״̬������ */
        CSIMA_CardStatusMsgSend(CSIMA_CBP_CARD_NOT_READY, CSIMA_CBP_UNKNOWN_CARD);
    }

    return;
}

/*****************************************************************************
�� �� ��  : CSIMA_CardStatusInd
��������  : CSIMAģ����״��յ���״̬��Ϣ
�������  : pstMsg:��״̬��Ϣ
�������  : ��
�� �� ֵ  : ��
History     :
1.��    ��  : 2014��01��16��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/

VOS_VOID CSIMA_CardStatusInd(PS_USIM_STATUS_IND_STRU *pstMsg)
{
    VOS_UINT32                          ulCSIMExistFlag;
    VOS_UINT8                           aucCsimAid[USIMM_AID_LEN_MAX];
    VOS_UINT32                          ulAidLen;

    /*ֻ����USIMУ��ɹ�PIN֮��Ż��ϱ����״̬*/
    if ((CSIMA_CBP_CARD_BOOTUP == g_stCBPCardStatus.enStatus)&&(USIMM_CARD_SERVIC_AVAILABLE == pstMsg->enCardStatus))
    {
        ulCSIMExistFlag = USIMM_GetAid(USIMM_AID_TYPE_CSIM, &ulAidLen, aucCsimAid);

        if(VOS_OK == ulCSIMExistFlag)
        {
            CSIMA_OpenCSIMOnLogicChannel(ulAidLen, aucCsimAid);
        }
        else
        {
            CSIMA_CardStatusMsgSend(CSIMA_CBP_CARD_NOT_READY, g_stCBPCardStatus.usCardType);
        }
    }
    else
    {
        if (USIMM_CARD_NOCARD == pstMsg->enCardType)    /*����*/
        {
            if(VOS_OK != CSIMA_Init())
            {
                CSIMA_ERROR_LOG("CSIMA_CardStatusInd: CSIMA_Init fail");
            }

            CSIMA_CardStatusMsgSend(CSIMA_CBP_CARD_NOT_READY, CSIMA_CBP_UNKNOWN_CARD);
        }
        else if (USIMM_CARD_SERVIC_AVAILABLE != pstMsg->enCardStatus)      /*PIN�ֿ���*/
        {
            CSIMA_CardStatusMsgSend(CSIMA_CBP_CARD_BOOTUP, g_stCBPCardStatus.usCardType);
        }
        else    /*����״̬����*/
        {
            CSIMA_ERROR_LOG("CSIMA_CardStatusInd: Receive Card status");
        }
    }

    return;
}


VOS_UINT32  CSIMA_PIHMsgProc(PS_USIM_STATUS_IND_STRU *pstMsg)
{
    if (PS_USIM_GET_STATUS_IND != pstMsg->ulMsgName)
    {
        /* ��¼����Ŀǰֻ����PIHģ�鷢�����Ŀ�״̬��Ϣ */
        return VOS_ERR;
    }

    if (CSIMA_CBP_CARD_STATUS_BUTT == g_stCBPCardStatus.enStatus)   /*��һ���ϱ�*/
    {
        CSIMA_FirstCardStatusInd(pstMsg);
    }
    else
    {
        CSIMA_CardStatusInd(pstMsg);
    }

    return VOS_OK;
}


VOS_VOID CSIMA_PidMsgProc(CSIMA_CBP_MSG_STRU *pstMsg)
{
    VOS_UINT32                          ulResult = VOS_ERR;

    if (VOS_TRUE != g_stLCEnableFlag.ucLCEnableFlg)
    {
        CSIMA_WARNING_LOG("CSIMA_PidMsgProc:feature LC is disable");

        return;
    }

    switch (pstMsg->ulSenderPid)
    {
        case WUEPS_PID_AT:
            ulResult = CSIMA_ATMsgProc(pstMsg);
            break;

        case WUEPS_PID_USIM:
            ulResult = CSIMA_UsimMsgProc(pstMsg);
            break;

        case MAPS_STK_PID:
            ulResult = CSIMA_STKMsgProc(pstMsg);
            break;

        case MAPS_PIH_PID:
            ulResult = CSIMA_PIHMsgProc((PS_USIM_STATUS_IND_STRU *)pstMsg);
            break;

        case WUEPS_PID_CBPCA:
            ulResult = CSIMA_CBPCAMsgProc(pstMsg);
            break;

        default:
            CSIMA_WARNING_LOG("CSIMA_PidMsgProc:Default Unknow PID Msg");
            break;
    }

    if (VOS_OK != ulResult)
    {
        /* ��¼���� */
        CSIMA_ERROR_LOG("CSIMA_PidMsgProc:FAIL to proc msg");
    }

    return;
}
VOS_UINT32 CSIMA_Init(VOS_VOID)
{
    VOS_UINT32                          ulResult;

    /* ��PIHģ��ע�Ῠ״̬�ϱ�PID */
    ulResult = PIH_RegUsimCardStatusIndMsg(WUEPS_PID_CSIMA);

    if (VOS_OK != ulResult)
    {
        CSIMA_ERROR_LOG("CSIMA_Init: REG card status Ind fail");
        return VOS_ERR;
    }

    /* ģ��ȫ�ֱ�����ʼ�� */
    VOS_MemSet(&g_stCBPCardStatus, 0, sizeof(g_stCBPCardStatus));

    g_stCBPCardStatus.enIPCMsgId    = CSIMA_CBP_UICC_STATUS_IND_MSG;
    g_stCBPCardStatus.enStatus      = CSIMA_CBP_CARD_STATUS_BUTT;
    g_stCBPCardStatus.ucCSIMChanNum = CSIMA_CARD_CHANEL_NULL;
    g_stCBPCardStatus.ucUSIMChanNum = CSIMA_CARD_CHANEL_NULL;
    g_stCBPCardStatus.ucISIMChanNum = CSIMA_CARD_CHANEL_NULL;

    if (NV_OK != NV_Read(en_NV_Item_LC_Ctrl_PARA, &g_stLCEnableFlag, sizeof(TAF_NV_LC_CTRL_PARA_STRU)))
    {
        g_stLCEnableFlag.ucLCEnableFlg = VOS_FALSE;
        g_stLCEnableFlag.enRatCombined = TAF_NVIM_LC_RAT_COMBINED_GUL;
    }

    return VOS_OK;
}


VOS_UINT32 CSIMA_PidInit(enum VOS_INIT_PHASE_DEFINE ip)
{
    switch( ip )
    {
        case VOS_IP_INITIAL:
            return CSIMA_Init();

        default:
            break;
    }

    return VOS_OK;
}
#endif /* FEATURE_ON == FEATURE_CL_INTERWORK */

VOS_UINT32 CSIMA_FidInit(enum VOS_INIT_PHASE_DEFINE ip)
{
#if (FEATURE_ON == FEATURE_CL_INTERWORK)
    VOS_UINT32      ulRslt;

    switch (ip)
    {
        case VOS_IP_LOAD_CONFIG:
        {
            ulRslt = VOS_RegisterPIDInfo(WUEPS_PID_CSIMA,
                                        (Init_Fun_Type)CSIMA_PidInit,
                                        (Msg_Fun_Type)CSIMA_PidMsgProc);
            if (VOS_OK != ulRslt)
            {
                return VOS_ERR;
            }

            /* ע���������ȼ� */
            ulRslt = VOS_RegisterMsgTaskPrio(WUEPS_FID_CSIMA, VOS_PRIORITY_M5);

            if (VOS_OK != ulRslt)
            {
                return VOS_ERR;
            }

            break;
        }
        default:
            break;
    }
#endif /* FEATURE_ON == FEATURE_CL_INTERWORK */

    return VOS_OK;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

