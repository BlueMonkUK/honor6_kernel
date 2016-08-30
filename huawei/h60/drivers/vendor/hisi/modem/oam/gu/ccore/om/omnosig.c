/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : omnosig.c
  �� �� ��   : ����
  ��    ��   : ���� 47350
  ��������   : 2011��9��27��
  ����޸�   :
  ��������   :
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��9��27��
    ��    ��   : ���� 47350
    �޸�����   : �����ļ�

******************************************************************************/


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "omprivate.h"
#include "omnosig.h"
#include "hpacomm.h"
#include "wasphycomminterface.h"
#include "PhyGrmInterface.h"
#include "PhyGasInterface.h"
#include "WttfDspInterface.h"
/*#include "NVIM_Interface.h"
 */
#include "hpamailboxdef.h"
#include "TtfOamInterface.h"

#define    THIS_FILE_ID        PS_FILE_ID_OM_NO_SIG_C


/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

#if (VOS_WIN32 == VOS_OS_VER)

VOS_UINT8 g_aucPhyDataReqAddr[1024];
#define  WTTFPHY_MAC_PHY_DATA_REQ_ADDR  (VOS_UINT32)(&g_aucPhyDataReqAddr[0])

#endif


/* ����R99 Mailbox VTB �������� */
VOS_UINT8   g_aucOmR99MBData[OM_R99_MAILBOX_MAX_PHY_DATA_IND_LEN];

/*�����ʽ��ʶ����*/
VOS_UINT16  g_ausTrchId[OM_UL_TRANS_CHANNEL_NUM] = {1 , 5};

/*����SN��*/
VOS_UINT32  g_ulOmSerialNum = 0;

VOS_UINT32  g_ulReportFlag = VOS_FALSE;



OM_MSG_FUN_STRU g_astOmNoSigProcTbl[] =
{
    {APP_OM_NON_SIG_BT_TRAN_REQ, OM_BtTransProc,     OM_APP_NON_SIG_BT_TRAN_CNF},
    {APP_OM_W_SYNC_STATUS_REQ,   OM_WSyncStatusProc, OM_APP_W_SYNC_STATUS_CNF},
    {APP_OM_GE_DL_STATUS_REQ,    OM_GeDlStatusProc,  OM_APP_GE_DL_STATUS_CNF},
    {APP_OM_EDGE_BLER_REQ,       OM_EdgeBlerProc,    OM_APP_EDGE_BLER_CNF},
    {APP_OM_W_BER_DATA_REQ,      OM_BerConfigProc,   OM_APP_W_BER_DATA_CNF}
};


/*��������ƽṹ��*/
OM_NOSIG_CTRL_STRU  g_stOmNoSigCtrl;

extern VOS_UINT32 OM_TraceMsgHook(VOS_VOID* pMsg);

/*BT������֧�ֱ��*/
VOS_UINT32  g_ulOmNosigEnable = 0;

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


/*****************************************************************************
 �� �� ��  : OM_BtTransProc
 ��������  : BT������͸��ͨ������
 �������  : pstAppToOmMsg�� ָ�򹤾߷���������ָ��
             usReturnPrimId: ���ظ�PC���ߵ�PRIMID

 �������  : ��
 �� �� ֵ  : VOS_OK:�ɹ�������Ϊʧ��

*****************************************************************************/
VOS_UINT32 OM_BtTransProc(APP_OM_MSG_EX_STRU *pstAppToOmMsg, VOS_UINT16 usReturnPrimId)
{
    VOS_UINT16                  usNoSigMsgCnt;
    VOS_UINT16                  usIndex;
    VOS_UINT32                  ulMsgDataAddr;
    OM_NOSIG_MSG_HEAD_STRU      *pstNoSigMsgHead;
    MsgBlock                    *pstSndMsg;

    /* ���� FuncType */
    g_stOmNoSigCtrl.ucFuncType = pstAppToOmMsg->ucFuncType;

    /* �����Ƿ�ʹ����Ϣ�ϱ� */
    g_stOmNoSigCtrl.usEnableReport = *(VOS_UINT16*)(pstAppToOmMsg->aucPara);

    /* ������ģʽ�µĽ�����Ϣ�� */
    usNoSigMsgCnt = *(VOS_UINT16*)(pstAppToOmMsg->aucPara + sizeof(VOS_UINT16));

    ulMsgDataAddr =  (VOS_UINT32)(pstAppToOmMsg->aucPara + sizeof(VOS_UINT32));

    VOS_TaskLock();

    for (usIndex = 1; usIndex <= usNoSigMsgCnt; usIndex++)
    {
        pstNoSigMsgHead = (OM_NOSIG_MSG_HEAD_STRU*)ulMsgDataAddr;

        /* �������·���Ϣ�����VOS�ӿڷ��ͳ�ȥ */
        if (VOS_FALSE == pstNoSigMsgHead->usNsMsgType)
        {
            pstSndMsg = VOS_AllocMsg(WUEPS_PID_NOSIG, pstNoSigMsgHead->usNsMsgLen - VOS_MSG_HEAD_LENGTH);

            if (VOS_NULL_PTR == pstSndMsg)
            {
                VOS_TaskUnlock();

                return VOS_ERR;
            }

            VOS_MemCpy(pstSndMsg, &(pstNoSigMsgHead->usRcvMsgPrimId), pstNoSigMsgHead->usNsMsgLen);

            (VOS_VOID)VOS_SendMsg(pstSndMsg->ulSenderPid, pstSndMsg);

            /* ǰ4���ֽ�Ϊ��Ϣ���ͺ���Ϣ�����ֶ� */
            ulMsgDataAddr += (sizeof(VOS_UINT32) + pstNoSigMsgHead->usNsMsgLen);
        }
        /* ����Ϊ�ȴ���Ϣ */
        else
        {
            g_stOmNoSigCtrl.usRcvMsgPrimId = pstNoSigMsgHead->usRcvMsgPrimId;

            VOS_TaskUnlock();

            if (VOS_OK != VOS_SmP(g_stOmNoSigCtrl.ulNoSigSyncSem, OM_NOSIG_WAIT_TIME_LEN))
            {
                OM_SendResult(pstAppToOmMsg->ucFuncType, usIndex, usReturnPrimId);

                return VOS_ERR;
            }

            VOS_TaskLock();
            ulMsgDataAddr += sizeof(OM_NOSIG_MSG_HEAD_STRU);
        }
    }

    VOS_TaskUnlock();
    OM_SendResult(pstAppToOmMsg->ucFuncType, VOS_OK, usReturnPrimId);

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : OM_WSyncStatusProc
 ��������  : ��ѯW��ͬ��ʧ��״̬
 �������  : pstAppToOmMsg�� ָ�򹤾߷���������ָ��
             usReturnPrimId: ���ظ�PC���ߵ�PRIMID

 �������  : ��
 �� �� ֵ  : VOS_OK:�ɹ�������Ϊʧ��

*****************************************************************************/
VOS_UINT32 OM_WSyncStatusProc(APP_OM_MSG_EX_STRU *pstAppToOmMsg, VOS_UINT16 usReturnPrimId)
{
    /*��ͬ��״̬���͸�PC����*/
    OM_SendResult(pstAppToOmMsg->ucFuncType, (VOS_UINT32)g_stOmNoSigCtrl.usSyncStatus, usReturnPrimId);

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : OM_EdgeBlerProc
 ��������  : ��ѯBLER
 �������  : pstAppToOmMsg�� ָ�򹤾߷���������ָ��
             usReturnPrimId: ���ظ�PC���ߵ�PRIMID

 �������  : ��
 �� �� ֵ  : VOS_OK:�ɹ�������Ϊʧ��

*****************************************************************************/

VOS_UINT32 OM_EdgeBlerProc (APP_OM_MSG_EX_STRU *pstAppToOmMsg, VOS_UINT16 usReturnPrimId)
{
    OM_EDGE_BLER_STRU                   stOmEdgeBlerData;
    MODEM_ID_ENUM_UINT16                enModemID;

    enModemID = OM_GET_FUNCID_MODEMINFO(pstAppToOmMsg->ucFuncType);

    if(MODEM_ID_BUTT <= enModemID)
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_EdgeBlerProc : Modem ID Error!");
        return VOS_ERR;
    }

    stOmEdgeBlerData.ulFirstErrNum    = HPA_Read32Reg(g_aulGBBPRegAddr[BLER_ERR][enModemID]);
    stOmEdgeBlerData.ulFirstTotalNum  = HPA_Read32Reg(g_aulGBBPRegAddr[BLER_TOTAL][enModemID]);

    if( VOS_OK != VOS_TaskDelay(OM_EDGE_BLER_TASK_DELAY))
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_EdgeBlerProc : Task Delay Fail!");
        return VOS_ERR;
    }

    stOmEdgeBlerData.ulSecondErrNum   = HPA_Read32Reg(g_aulGBBPRegAddr[BLER_ERR][enModemID]);
    stOmEdgeBlerData.ulSecondTotalNum = HPA_Read32Reg(g_aulGBBPRegAddr[BLER_TOTAL][enModemID]);
    stOmEdgeBlerData.usLength         = (VOS_UINT16)(sizeof(OM_EDGE_BLER_STRU)- VOS_OM_HEADER_LEN);

    OM_SendContent(pstAppToOmMsg->ucFuncType, (OM_APP_MSG_EX_STRU *)(&stOmEdgeBlerData), usReturnPrimId);

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  :   OM_GeDlStatusProc
 ��������  : ���GSM/EDGE�µ�BER���
 �������  : pstAppToOmMsg�� ָ�򹤾߷���������ָ��
             usReturnPrimId: ���ظ�PC���ߵ�PRIMID

 �������  : ��
 �� �� ֵ  : VOS_OK:�ɹ�������Ϊʧ��

*****************************************************************************/
VOS_UINT32 OM_GeDlStatusProc (APP_OM_MSG_EX_STRU *pstAppToOmMsg, VOS_UINT16 usReturnPrimId)
{
    VOS_UINT32                          ulPreFN;
    VOS_UINT32                          ulCurrentFN;
    VOS_UINT32                          ulRealFN;
    PHP_RRBP_FN_REQ                     *pstPhpRrbpReq;
    MsgBlock                            *pstSndMsg;
    OM_GE_DL_STATUS_REQ_STRU            *pstOmGeDlStatReq;
    MODEM_ID_ENUM_UINT16                enModemID;

    pstOmGeDlStatReq = (OM_GE_DL_STATUS_REQ_STRU*)pstAppToOmMsg;

    enModemID = OM_GET_FUNCID_MODEMINFO(pstAppToOmMsg->ucFuncType);

    if(MODEM_ID_BUTT <= enModemID)
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_GeDlStatusProc :Modem ID Error!");

        return VOS_ERR;
    }

    /* �������Ϣ�Ƿ��ϱ���� */
    g_ulReportFlag = VOS_TRUE;

    /* GSM CS ״̬�£������·�������������ϱ�״̬ */
    if (VOS_TRUE == pstOmGeDlStatReq->usModeType)
    {
        return VOS_OK;
    }

    pstSndMsg = VOS_AllocMsg(UEPS_PID_GRM, sizeof(PHP_RRBP_FN_REQ) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstSndMsg)
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_GeDlStatusProc :DIAG_DataProc Alloc Msg Failed!");

        return VOS_ERR;
    }

    if(MODEM_ID_1 == enModemID)
    {
        pstSndMsg->ulSenderPid      = I1_UEPS_PID_GRM;
        pstSndMsg->ulReceiverPid    = I1_DSP_PID_GPHY;   /*���͸���1��PID*/
    }
    else
    {
        pstSndMsg->ulSenderPid      = I0_UEPS_PID_GRM;
        pstSndMsg->ulReceiverPid    = I0_DSP_PID_GPHY;   /*���͸���0��PID*/
    }
    /*ͨ����ȡ�Ĵ�����õ�ǰ֡��*/
    ulPreFN  = HPA_Read32Reg(g_aulGBBPRegAddr[GTC_FN][enModemID]);

    /*��ǰ֡������������ɣ���16λ*1326 + ��16λ�Ĵ���ֵ*/
    ulCurrentFN  = ((ulPreFN >> 16)& OM_FRAME_NUM_REG_MASKBIT)* OM_HIHG_ADDR_FRAME_NUM;
    ulCurrentFN  += (ulPreFN & OM_FRAME_NUM_REG_MASKBIT);

    /*ʵ��֡��Ϊ��ǰ֡�������·���Ϣʱ�����ֵ*/
    ulRealFN     = (ulCurrentFN + OM_DL_MSG_FRAME_NUM ) % OM_FRAME_MAX_NUM;

    pstPhpRrbpReq            = (PHP_RRBP_FN_REQ*)pstSndMsg;
    pstPhpRrbpReq->enRptMeas = VOS_TRUE;
    pstPhpRrbpReq->enMsgID   = ID_PHP_RRBP_FN_REQ ;
    pstPhpRrbpReq->ulFN      = ulRealFN;

    if(VOS_OK != VOS_SendMsg(UEPS_PID_GRM, pstSndMsg))
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_GeDlStatusProc :VOS_SendMsg Failed");

        return VOS_ERR;
    }
    return VOS_OK;

}

/*****************************************************************************
 �� �� ��  : OM_BerConfigProc
 ��������  : ����BER�ϱ�����
 �������  : pstAppToOmMsg�� ָ�򹤾߷���������ָ��
             usReturnPrimId: ���ظ�PC���ߵ�PRIMID

 �������  : ��
 �� �� ֵ  : VOS_OK:�ɹ�������Ϊʧ��

*****************************************************************************/
VOS_UINT32 OM_BerConfigProc(APP_OM_MSG_EX_STRU *pstAppToOmMsg, VOS_UINT16 usReturnPrimId)
{
    APP_OM_W_BER_DATA_REQ_STRU          *pstBerConfig;

    pstBerConfig = (APP_OM_W_BER_DATA_REQ_STRU*)pstAppToOmMsg;

    OM_SendResult(pstAppToOmMsg->ucFuncType, VOS_OK, usReturnPrimId);

    VOS_TaskLock();

    g_stOmNoSigCtrl.ulTotalFrameNum = pstBerConfig->usFrameNum;
    g_stOmNoSigCtrl.ulCurFrameNum   = 0;

    VOS_TaskUnlock();

    return VOS_OK;
}


VOS_VOID OM_NoSigFilterMsg(MsgBlock *pMsg)
{
    VOS_UINT16                             usPrimId;
    OM_GE_DL_STATUS_STRU                   stGeDlStatus;
    MPH_CS_DEDICATED_MEAS_IND_STRU        *pstMphMeasInd;
    MPHP_EGPRS_CHANNEL_QUALITY_REPORT_ST  *stMphpEgrsReport;
    MODEM_ID_ENUM_UINT16                   enModemID;

    usPrimId = *(VOS_UINT16*)(pMsg->aucValue);

    if (MAIL_BOX_PROTECTWORD_SND != g_ulOmNosigEnable)
    {
        return;
    }

    enModemID = OM_GET_FUNCID_MODEMINFO(g_stOmNoSigCtrl.ucFuncType);
    if(MODEM_ID_BUTT <= enModemID)
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_NoSigFilterMsg : Modem ID Error!");
        return;
    }

    if(MODEM_ID_1 == enModemID)
    {
        if ((I1_DSP_PID_GPHY > pMsg->ulSenderPid)
           || (I1_DSP_PID_APM < pMsg->ulSenderPid))
        {
            return;
        }
    }
    else
    {
        if ((DSP_PID_GU_RESERVED > pMsg->ulSenderPid)
           || (DSP_PID_DRX < pMsg->ulSenderPid))
        {
            return;
        }
    }

    /*͸��ͨ���ȴ�����Ϣ,�ͷŵȴ��ź���*/
    if (usPrimId == g_stOmNoSigCtrl.usRcvMsgPrimId)
    {
        if (VOS_TRUE == g_stOmNoSigCtrl.usEnableReport)
        {
#if (FEATURE_ON == FEATURE_CBT_LOG)            
            OM_GreenChannel(OM_CBT_TRANS_FUNC, OM_APP_TRACE_IND, (VOS_UINT8*)pMsg, (VOS_UINT16)(pMsg->ulLength+VOS_MSG_HEAD_LENGTH));
#else
            OM_GreenChannel(OM_TRANS_FUNC, OM_APP_TRACE_IND, (VOS_UINT8*)pMsg, (VOS_UINT16)(pMsg->ulLength+VOS_MSG_HEAD_LENGTH));
#endif
        }

        VOS_SmV(g_stOmNoSigCtrl.ulNoSigSyncSem);
    }

    /*������յ�W��ͬ����ʧ����Ϣ, ��¼��ȫ�ֱ�����*/
    if ( ID_PHY_RRC_SYNC_IND == usPrimId  )
    {
        g_stOmNoSigCtrl.usSyncStatus = OM_W_SYNC_STATUS_SYNC;

        return;
    }

    if ( ID_PHY_RRC_OUT_OF_SYNC_IND == usPrimId)
    {
        g_stOmNoSigCtrl.usSyncStatus = OM_W_SYNC_STATUS_OUT_OF_SYNC;

        return;
    }

    /*������յ�GSM/EDGE��BER�ظ���Ϣ ��ȡ����Ϣ���ݣ�����������͸�PC�๤��*/
    if ( (ID_MPH_CS_DEDICATED_MEAS_IND == usPrimId) && (VOS_TRUE == g_ulReportFlag))
    {
        pstMphMeasInd = (MPH_CS_DEDICATED_MEAS_IND_STRU*)pMsg;

        stGeDlStatus.usBerValue[0] = pstMphMeasInd->usMeanBep;
        stGeDlStatus.usBerValue[1] = pstMphMeasInd->usCvBep;
        stGeDlStatus.usLength      = (VOS_UINT16)(sizeof(stGeDlStatus)- VOS_OM_HEADER_LEN);

        /*ֻ�ϱ�һ�� */
        g_ulReportFlag = VOS_FALSE;

        OM_SendContent(g_stOmNoSigCtrl.ucFuncType, (OM_APP_MSG_EX_STRU *)(&stGeDlStatus), OM_APP_GE_DL_STATUS_CNF);

        return;
    }

    if( (ID_MPHP_EGPRS_CHANNEL_QUALITY_REPORT == usPrimId) && (VOS_TRUE == g_ulReportFlag))
    {
        stMphpEgrsReport = (MPHP_EGPRS_CHANNEL_QUALITY_REPORT_ST*)pMsg;

        stGeDlStatus.usBerValue[0] = stMphpEgrsReport->usMeanBep8Psk;
        stGeDlStatus.usBerValue[1] = stMphpEgrsReport->usCVBep8Psk;
        stGeDlStatus.usLength      = (VOS_UINT16)(sizeof(stGeDlStatus)- VOS_OM_HEADER_LEN);

        /*ֻ�ϱ�һ�� */
        g_ulReportFlag = VOS_FALSE;

        OM_SendContent(g_stOmNoSigCtrl.ucFuncType, (OM_APP_MSG_EX_STRU *)(&stGeDlStatus), OM_APP_GE_DL_STATUS_CNF);

        return;
    }

    return;
}
VOS_VOID OM_NoSigMsgProc(MsgBlock *pMsg)
{
    APP_OM_MSG_EX_STRU  *pstAppToOmMsg;
    VOS_UINT16          usReturnPrimId;
    VOS_UINT32          ulTotalNum;
    VOS_UINT32          ulIndex;
    VOS_UINT32          ulResult = VOS_ERR;

    if (WUEPS_PID_OM == pMsg->ulSenderPid)
    {
        pstAppToOmMsg = (APP_OM_MSG_EX_STRU*)pMsg->aucValue;

        ulTotalNum = sizeof(g_astOmNoSigProcTbl)/sizeof(OM_MSG_FUN_STRU);

        /* ����PrimId���Ҷ�Ӧ�Ĵ����� */
        for (ulIndex = 0; ulIndex < ulTotalNum; ulIndex++)
        {
            if (pstAppToOmMsg->usPrimId == g_astOmNoSigProcTbl[ulIndex].ulPrimId)
            {
                usReturnPrimId = (VOS_UINT16)(g_astOmNoSigProcTbl[ulIndex].ulReturnPrimId);

                ulResult = g_astOmNoSigProcTbl[ulIndex].pfFun(pstAppToOmMsg, usReturnPrimId);

                break;
            }
        }

        if (ulIndex == ulTotalNum)
        {
            PS_LOG2(WUEPS_PID_OM, 0, PS_PRINT_INFO, "OM_NoSigMsgProc: Result :%d, PrimId : %d can't be Found\n",
                    (VOS_INT32)ulResult, (VOS_INT32)pstAppToOmMsg->usPrimId);
        }

    }

    return;
}
VOS_VOID OM_R99MailboxResetMutexFlag(VOS_UINT32  ulMailboxBufIndex)
{

    VOS_UINT32 ulVtbDataFlag;

    ulVtbDataFlag = HPA_Read16Reg(g_aulR99MailboxVtbAddr[ulMailboxBufIndex]);

    /* ���VTB��TURBO������ָʾ��־ */
    if ( OM_R99_MAILBOX_MUTEX_DATA_EXIST == ulVtbDataFlag)
    {
        HPA_Write16Reg(g_aulR99MailboxVtbAddr[ulMailboxBufIndex],
                            OM_R99_MAILBOX_MUTEX_DATA_NOT_EXIST);
    }

    /* �������ȡ��������Ķ�д������ */
    if (OM_R99_DECODE_BUF_INDEX_FIRST == ulMailboxBufIndex)
    {
        HPA_Write32Reg(WTTFPHY_MAC_DATA_IND_FIRST_MUTEX_ADDR,
                            OM_R99_MAILBOX_MUTEX_DATA_NOT_EXIST);
    }
    else if (OM_R99_DECODE_BUF_INDEX_SECOND == ulMailboxBufIndex)
    {
        HPA_Write32Reg(WTTFPHY_MAC_DATA_IND_SECOND_MUTEX_ADDR,
                            OM_R99_MAILBOX_MUTEX_DATA_NOT_EXIST);
    }
    else
    {

    }

    return;
}

/*****************************************************************************
 �� �� ��  : OM_GetR99MailboxIndex
 ��������  : ��������������ֳ�0��1���飬ÿ�鶼����VITERBI��TURBO�������ݣ�
              DSPÿ���յ������ж�,���ȿ�0��BUFF�Ƿ�ɶ�����ɶ������ȡBUFF index
              �����ٿ�1��BUFF�Ƿ�ɶ�����ɶ������ȡBUFF index ;
              ������BUFF�����ɶ����򷵻�0��
              ͬһ������VTB���ݵ������Turbo���ݵ����һ��, ����ȡ��1�����ɡ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : 0:��Buf�ɶ�;1,2������Ӧ��Buf Index�ɶ�
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   :
    ��    ��   :
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 OM_GetR99MailboxIndex(VOS_VOID)
{

    VOS_UINT32 ulVtbDataFlag;
    VOS_UINT32 ulFirstSerialNum     = 0;
    VOS_UINT32 ulSecondSerialNum    = 0;

    /* �ñ�����Ϊ��ʾ����1��2���ڵ����룬���������ַ�����±� */
    VOS_UINT32 ulMailboxIndex       = 0;

    VOS_UINT32 ulFirstMailboxMuteFlag;   /* ������������1��д������ */
    VOS_UINT32 ulSecondMailboxMuteFlag;  /* ������������2��д������ */

    /* ��ȡVTB��TURBO BUFFER������ָʾ��־ */
    ulFirstMailboxMuteFlag  = HPA_Read32Reg(WTTFPHY_MAC_DATA_IND_FIRST_MUTEX_ADDR);
    ulSecondMailboxMuteFlag = HPA_Read32Reg(WTTFPHY_MAC_DATA_IND_SECOND_MUTEX_ADDR);

    /* ��������BUFFER�Ļ����־����ulMailDataInd */
    if (OM_R99_MAILBOX_MUTEX_DATA_EXIST == ulFirstMailboxMuteFlag)
    {
        ulMailboxIndex |= OM_R99_DECODE_BUF_INDEX_FIRST;
    }
    if (OM_R99_MAILBOX_MUTEX_DATA_EXIST == ulSecondMailboxMuteFlag)
    {
        ulMailboxIndex |= OM_R99_DECODE_BUF_INDEX_SECOND;
    }

    /* ���������Ƿ���ڵ�����ֵ���ж϶�ȡ�ĸ����������(��ulMailDataInd��������ַ�����±�) */

    /* ���ֻ��1�����������ݣ��򲻸ı�ulMailDataInd�������������±꣬ȡ�����ַ */

    /* ����������䶼�����ݣ� */
    if ((OM_R99_DECODE_BUF_INDEX_FIRST|OM_R99_DECODE_BUF_INDEX_SECOND) == ulMailboxIndex)
    {
        /* ���������ж������ݵ���� */

        /* ͬһ������VTB���ݵ������Turbo���ݵ����һ��, ����ȡ��1������ */
        ulVtbDataFlag = HPA_Read16Reg(g_aulR99MailboxVtbAddr[OM_R99_DECODE_BUF_INDEX_FIRST]);

        if (OM_R99_MAILBOX_MUTEX_DATA_EXIST == ulVtbDataFlag)
        {
            /*lint -e413*/
            ulFirstSerialNum = HPA_Read32Reg(WTTFPHY_MAC_DATA_IND_FIRST_VTB_ADDR
                                  + OM_OFFSET_OF(WTTFPHY_MAC_DATA_IND_CTRL_PARAM_STRU, ulSerialNum));
        }
        else
        {
            ulFirstSerialNum = HPA_Read32Reg(WTTFPHY_MAC_DATA_IND_FIRST_TURBO_ADDR
                                   + OM_OFFSET_OF(WTTFPHY_MAC_DATA_IND_CTRL_PARAM_STRU, ulSerialNum));
        }

        ulVtbDataFlag = HPA_Read16Reg(g_aulR99MailboxVtbAddr[OM_R99_DECODE_BUF_INDEX_SECOND]);

        if (OM_R99_MAILBOX_MUTEX_DATA_EXIST == ulVtbDataFlag)
        {
            ulSecondSerialNum = HPA_Read32Reg(WTTFPHY_MAC_DATA_IND_SECOND_VTB_ADDR
                                          + OM_OFFSET_OF(WTTFPHY_MAC_DATA_IND_CTRL_PARAM_STRU, ulSerialNum));
        }
        else
        {
            ulSecondSerialNum = HPA_Read32Reg(WTTFPHY_MAC_DATA_IND_SECOND_TURBO_ADDR + OM_OFFSET_OF(WTTFPHY_MAC_DATA_IND_CTRL_PARAM_STRU, ulSerialNum));
            /*lint +e413*/
        }

        /* �Ƚ����������е�������ţ����������С������ */
        if (ulFirstSerialNum < ulSecondSerialNum)
        {
            ulMailboxIndex = OM_R99_DECODE_BUF_INDEX_FIRST;
        }
        else
        {
            ulMailboxIndex = OM_R99_DECODE_BUF_INDEX_SECOND;
        }
    }

    return ulMailboxIndex;
}
VOS_UINT32 OM_R99MailboxRead(VOS_UINT8 *pucDataIndBuf,
                                        VOS_UINT16 *pusDataLen,
                                        VOS_UINT32  ulMailboxBufIndex)
{
    VOS_UINT32                          ulMailboxBufAddr;
    VOS_UINT32                          ulVtbDataFlag;
    VOS_UINT16                          usVtbDataLen = 0;

    ulVtbDataFlag = HPA_Read16Reg(g_aulR99MailboxVtbAddr[ulMailboxBufIndex]);

    /* ����ȵ����ŵ�һ��ΪVTB����, �������� ,���VTB����RAM�����ݣ��ж����ݳ��ȵ���Ч��*/
    if (OM_R99_MAILBOX_MUTEX_DATA_EXIST == ulVtbDataFlag)
    {
        ulMailboxBufAddr    = (g_aulR99MailboxVtbAddr[ulMailboxBufIndex]);

        usVtbDataLen        = HPA_Read16Reg(ulMailboxBufAddr + sizeof(VOS_UINT16));

        if (((usVtbDataLen*4) <= WTTFPHY_MAX_PHY_DATA_IND_VTB_DATA_LEN) && (usVtbDataLen > 0))
        {
            HPA_GetDspMem((VOS_CHAR *)ulMailboxBufAddr, (VOS_CHAR *)pucDataIndBuf,
                                    WTTFPHY_MAX_PHY_DATA_IND_VTB_HEAD_LEN+(usVtbDataLen*4));

            *pusDataLen = (VOS_UINT16)(usVtbDataLen*4);

            return VOS_TRUE;
        }
    }

    return  VOS_FALSE;
}
VOS_VOID OM_LoopTestProc(VOS_VOID)
{
    VOS_UINT32                              ulMailboxBufIndex;
    VOS_UINT32                              ulResult;
    VOS_UINT16                              usDataLen;
    WTTFPHY_PHY_DATA_REQ_BUF_STRU           *pstMacPhyDataReqBuf;
    WTTFPHY_MAC_DATA_IND_CTRL_PARAM_STRU    *pstPhyDataIndHead;
/*lint -e813 �޸���:dongtinghuan;������:���� 51137 */
    APP_OM_W_BER_DATA_IND_STRU              stBerDataInd;
/*lint +e813 �޸���:dongtinghuan;������:���� 51137 */

    if (MAIL_BOX_PROTECTWORD_SND != g_ulOmNosigEnable)
    {
        return;
    }

    /*��������������, ���Ȼ�ȡ������������*/

    ulMailboxBufIndex = OM_GetR99MailboxIndex();

    if ( OM_R99_DECODE_BUF_INDEX_INIT == ulMailboxBufIndex )
    {
       return ;
    }

    ulResult = OM_R99MailboxRead(g_aucOmR99MBData, &usDataLen, ulMailboxBufIndex);

    if (VOS_TRUE == ulResult)
    {
        /*���п��Ʋ���д�������ŵ���������*/
        pstMacPhyDataReqBuf = (WTTFPHY_PHY_DATA_REQ_BUF_STRU *)(WTTFPHY_MAC_PHY_DATA_REQ_ADDR);

        pstPhyDataIndHead = (WTTFPHY_MAC_DATA_IND_CTRL_PARAM_STRU *)&g_aucOmR99MBData[0];

        pstMacPhyDataReqBuf->enMutexFlag = WTTFPHY_MUTEX_DATA_EXIST;
        pstMacPhyDataReqBuf->enCctrchID  = WTTFPHY_UL_DPCH;
        pstMacPhyDataReqBuf->enMsgName   = ID_MAC_PHY_DATA_REQ;
        pstMacPhyDataReqBuf->usUlTrchCnt = OM_UL_TRANS_CHANNEL_NUM ;

        /*�����ŵ�CFNΪ��ǰCFN+1*/
        pstMacPhyDataReqBuf->usCFN = (g_ucHpaCfnRead + 1);

        /*Tfiȡֵ[1,32],�����ŵ���ʶ*/
        pstMacPhyDataReqBuf->astUlTrchDataInfo[0].usTfi = pstPhyDataIndHead->astDlTrchDataInfo[0].ucTfi;
        /* TrchIdȡֵ[0,63],�����ʽ��ʶ������̶�Ϊ1 */
        pstMacPhyDataReqBuf->astUlTrchDataInfo[0].usTrchId = g_ausTrchId[0];


        pstMacPhyDataReqBuf->astUlTrchDataInfo[1].usTfi = pstPhyDataIndHead->astDlTrchDataInfo[1].ucTfi ;
        pstMacPhyDataReqBuf->astUlTrchDataInfo[1].usTrchId = g_ausTrchId[1];

        pstMacPhyDataReqBuf->ulSerialNum = g_ulOmSerialNum;
        g_ulOmSerialNum++;

        pstMacPhyDataReqBuf->usDataLen = usDataLen;

        if (g_stOmNoSigCtrl.ulCurFrameNum < g_stOmNoSigCtrl.ulTotalFrameNum)
        {
            stBerDataInd.usTotalFrameNum = (VOS_UINT16)g_stOmNoSigCtrl.ulTotalFrameNum;
            stBerDataInd.usCurFrameNum   = (VOS_UINT16)g_stOmNoSigCtrl.ulCurFrameNum;
            g_stOmNoSigCtrl.ulCurFrameNum++;

            if (OM_BER_DATA_MAX_SIZE < (usDataLen + WTTFPHY_MAX_PHY_DATA_IND_VTB_HEAD_LEN))
            {
                stBerDataInd.ulDataLen = OM_BER_DATA_MAX_SIZE;

                LogPrint1("OM_LoopTestProc: BER data is too large %d.\r\n", usDataLen);
            }
            else
            {
                stBerDataInd.ulDataLen = (VOS_UINT32)usDataLen + WTTFPHY_MAX_PHY_DATA_IND_VTB_HEAD_LEN;
            }

            VOS_MemCpy(stBerDataInd.aucData, g_aucOmR99MBData, stBerDataInd.ulDataLen);

            stBerDataInd.usLength = sizeof(stBerDataInd) - VOS_OM_HEADER_LEN;
            OM_SendContent(g_stOmNoSigCtrl.ucFuncType, (OM_APP_MSG_EX_STRU*)&stBerDataInd, OM_APP_W_BER_DATA_IND);
        }

        HPA_PutDspMem((VOS_CHAR*)&g_aucOmR99MBData[WTTFPHY_MAX_PHY_DATA_IND_VTB_HEAD_LEN],
                                      (VOS_CHAR*)pstMacPhyDataReqBuf->aucData, usDataLen);

    }

    OM_R99MailboxResetMutexFlag(ulMailboxBufIndex);

    return;
}

/*****************************************************************************
 �� �� ��  : OM_NoSigCtrlInit
 ��������  : ��������ƽṹ��ȫ�ֱ���ʼ��
 �������  : ��

 �������  : ��
 �� �� ֵ  : VOID
*****************************************************************************/
VOS_VOID OM_NoSigCtrlInit(VOS_VOID)
{
    g_stOmNoSigCtrl.usEnableReport = VOS_FALSE;
    g_stOmNoSigCtrl.usRcvMsgPrimId = 0;
    g_stOmNoSigCtrl.ucFuncType     = OM_NOSIG_FUNC;
    g_stOmNoSigCtrl.usSyncStatus   = OM_W_SYNC_STATUS_UNKNOWN;
    g_stOmNoSigCtrl.ulTotalFrameNum= 0;
    g_stOmNoSigCtrl.ulCurFrameNum  = 0;
}


VOS_UINT32 OM_NoSigPidInit(enum VOS_INIT_PHASE_DEFINE ip)
{
    if (VOS_IP_LOAD_CONFIG == ip)
    {
        /* ���以���ź��� */
        if (VOS_OK != VOS_SmBCreate( "NOSIG", 0, VOS_SEMA4_FIFO, &(g_stOmNoSigCtrl.ulNoSigSyncSem)))
        {
            return VOS_ERR;
        }

        /* ��ʼ����������ƽṹ */
        OM_NoSigCtrlInit();
    }

    return VOS_OK;
}


VOS_UINT32 OM_NoSigFidInit(enum VOS_INIT_PHASE_DEFINE ip)
{
    VOS_UINT32 ulRslt;

    if (VOS_IP_LOAD_CONFIG == ip)
    {
        ulRslt = VOS_RegisterPIDInfo(WUEPS_PID_NOSIG,
                                    (Init_Fun_Type)OM_NoSigPidInit,
                                    (Msg_Fun_Type)OM_NoSigMsgProc);
        if( VOS_OK != ulRslt )
        {
            return VOS_ERR;
        }

        ulRslt = VOS_RegisterTaskPrio(WUEPS_FID_NOSIG, COMM_NOSIG_TASK_PRIO);

        if( VOS_OK != ulRslt )
        {
            return VOS_ERR;
        }
    }

    return VOS_OK;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

