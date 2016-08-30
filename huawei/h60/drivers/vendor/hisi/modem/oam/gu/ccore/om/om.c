/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��      : Om.c
  �� �� ��      : ����
  ��    ��      : ����47350
  ��������      : 2008��5��3��
  ����޸�      :
  ��������      : ��C�ļ�������OMģ���ʵ��
  �����б�      :
  �޸���ʷ      :
  1.��    ��    : 2008��5��3��
    ��    ��    : ����47350
    �޸�����    : �����ļ�

******************************************************************************/
/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "pslog.h"
#include "PsTypeDef.h"
#include "omfilesystem.h"
#include "hpacomm.h"
#include "hpaoperatertt.h"
#include "bbp_wcdma_interface.h"
#include "DspInterface.h"
#include "rfa.h"
#include "siapppb.h"
#include "siapppih.h"
#include "siappstk.h"
#include "v_queue.h"
#include "DrvInterface.h"
#include "GU_MSPComm.h"
#include "OmApp.h"
#include "OamSpecTaskDef.h"
#include "ScInterface.h"
#include "DrvNvInterface.h"

#if (RAT_MODE != RAT_GU)
#include "msp_nvim.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


/*lint -e767 �޸��ˣ�����47350�������ˣ�����46160��ԭ�������LOG���������Ҫ*/
#define    THIS_FILE_ID        PS_FILE_ID_OM_C
/*lint +e767 �޸��ˣ�����47350�������ˣ�lixiao��*/

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/* �Ժ�ÿ���汾���ر�ע������汾��صĺ���޸ģ�ע�ⲻ�ܴ���30���ַ� */
#define OAM_PLATFORM_VERSION "V900R100C00B080"

/*��������TRACE��EVENT�ϱ��Ĺ�����Ϣ*/
OM_TRACE_EVENT_CONFIG_PS_STRU   g_stTraceEventConfig;

VOS_UINT32                      g_aulPidTraceSendNum[OM_MAX_PS_MODULE_NUM] = {0};

VOS_UINT32                      g_aulPidEventSendNum[OM_MAX_PS_MODULE_NUM] = {0};

/*��������͸����Ϣ�Ŀ��أ�Ĭ��Ϊ�ر�*/
VOS_BOOL                        g_bOmTransSwitch    = VOS_FALSE;

/*���кţ�����TRACE,EVENT��Ϣ�����*/
VOS_UINT32                      g_ulOMMsgSN         = 1;

/*���Կ��أ�Ŀǰû�ã���ҪΪMUX����ͨ��*/
VOS_UINT32                      g_OmDebugPrintOnOff = 0;

/*���浱ǰ�Ĺ���ID��*/
VOS_UINT16                      g_usToolId          = 0;

VOS_MSG_HOOK_FUNC               g_apMsgHookFunc[OM_MSG_HOOK_BUTT] ={0,0};

/*OM BUF FULL �����ϱ�CPU��Ϣ��ʱ��*/
HTIMER                          g_stRegularBufFullTmr = VOS_NULL_PTR ;

/* ��¼�յ���Ϣ��Ϣ��buffer����ǰ���� */
OM_RECORD_BUF_STRU g_astRecordInfo[VOS_EXC_DUMP_MEM_NUM_BUTT];

#define NO_CONFIG_BIT                  (0)
#define TRACE_CONFIG_BIT               (BIT_N(0))
#define EVENT_CONFIG_BIT               (BIT_N(1))
#define TRACE_MASK_CONFIG_BIT          (BIT_N(2))

#define W_BBP_AFC_LOCK_IND_MASK                             BIT_N(16)

#define W_BBP_AFC_LOCK_VALUE_MASK                           0xFFF

extern  OM_NV_OPERATE_RECORD_STRU       g_astRecordAcpuToCcpu[SAVE_MAX_SEND_INFO];

#ifndef BIT
#define BIT(x)                  ((unsigned)0x1 << (x))
#endif

VOS_UINT32                      g_ulFlightModeVoteMap = 0;	/*DRX TIMER�ڷ���ģʽͶƱ*/

#ifdef DMT
extern VOS_VOID OM_WriteDataToFile(VOS_UINT8 *pucMsg, VOS_UINT16 usMsgLen);
#endif

extern VOS_VOID Om_CcpuBbpConfigMsgProc(OM_REQ_PACKET_STRU *pRspPacket, OM_RSP_FUNC *pRspFuncPtr);

extern VOS_UINT32 Om_BbpDumpEdmaInit(VOS_VOID);

extern VOS_VOID Om_BbpDbgSelfTask(unsigned long ulPara1, unsigned long ulPara2,
                  unsigned long ulPara3, unsigned long ulPara4);

extern VOS_VOID OM_NoSigFilterMsg(MsgBlock *pMsg);

/*****************************************************************************
 Prototype      : OM_RegisterMsgHook
 Description    :
 Input          :
 Output         :

 Return Value   : void

 History        : ---
    Date        :
    Author      :
    Modification:
 *****************************************************************************/
VOS_VOID OM_RegisterMsgHook(VOS_MSG_HOOK_FUNC pfHookFunc,
                                        OM_MSG_HOOK_ENUM_UINT32 ulFuncType)
{
    g_apMsgHookFunc[ulFuncType] = pfHookFunc;

    return;
}

/*****************************************************************************
 Prototype      :
 Description    :
 Input          :
 Output         :

 Return Value   : void

 History        : ---
    Date        :
    Author      :
    Modification:
 *****************************************************************************/
VOS_VOID OM_MsgHook(VOS_VOID *pMsg)
{
    VOS_UINT32 i;
    VOS_UINT32 ultemp;

    for(i=0; i<OM_MSG_HOOK_BUTT; i++)
    {
        if(VOS_NULL_PTR != g_apMsgHookFunc[i])
        {
            ultemp = g_apMsgHookFunc[i](pMsg);
            /* make PC_Lint Happy */
            ultemp = ultemp;
        }
    }

    return;
}
VOS_VOID OM_AddSNTime(VOS_UINT32 *pulSN, VOS_UINT32 *pulTimeStamp)
{
    VOS_INT   lLockKey;

    lLockKey = VOS_SplIMP();

    *(pulSN) = g_ulOMMsgSN++;

    VOS_Splx(lLockKey);

    if (VOS_NULL_PTR != pulTimeStamp)
    {
        *(pulTimeStamp) = OM_GetSlice();
    }
}

VOS_VOID OM_AddAppHeader (VOS_UINT8 ucFuncType, VOS_UINT8* pucInputMsg,
                                     OM_APP_HEADER_STRU* pstAppHeader, VOS_UINT32 ulTraceMsgLen)
{
    PS_OM_EVENT_IND_STRU   *pstrEvent;
    OM_APP_TRACE_STRU      *pstAppTrace;
    OM_APP_EVENT_STRU      *pstAppEvent;
    OM_APP_LOG_STRU        *pstAppLog;

    pstAppHeader->ucFuncType = ucFuncType;
    pstAppHeader->usLength = (VOS_UINT16)(ulTraceMsgLen - VOS_OM_HEADER_LEN);
    OM_AddSNTime(&(pstAppHeader->ulSn), &(pstAppHeader->ulTimeStamp));
    /*Complete different procedures by function type.*/
    switch (ucFuncType)
    {
        case OM_TRACE_FUNC:
        case OM_AIR_FUNC:
            pstAppTrace = (OM_APP_TRACE_STRU*)pstAppHeader;
            pstAppTrace->usPrimId = OM_APP_TRACE_IND;
            VOS_MemCpy(pstAppTrace->aucPara, pucInputMsg, ulTraceMsgLen - OM_APP_TRACE_LEN);
            break;

        case OM_EVENT_FUNC:
            pstrEvent = (PS_OM_EVENT_IND_STRU*)pucInputMsg;
            pstAppEvent = (OM_APP_EVENT_STRU*)pstAppHeader;
            pstAppEvent->ulModuleId = pstrEvent->ulModuleId;
            pstAppEvent->usEventId =  pstrEvent->usEventId;
            VOS_MemCpy(pstAppEvent->aucPara, pstrEvent->aucData, ulTraceMsgLen - OM_APP_EVENT_LEN);
            break;

        case OM_LOG_FUNC:
            pstAppLog = (OM_APP_LOG_STRU*)pstAppHeader;
            pstAppLog->usPrimId = OM_APP_PRINT_IND;
            VOS_MemCpy(&(pstAppLog->ulModuleId), pucInputMsg,
                         (ulTraceMsgLen - OM_APP_LOG_LEN) + LOG_MODULE_ID_LEN);
            break;
         default:
            break;
    }
    return;
}
VOS_UINT32 OM_SendData(OM_RSP_PACKET_STRU *pucMsg, VOS_UINT16 usMsgLen)
{
    OM_APP_MSG_EX_STRU                 *pstAppMsg;

#ifdef DMT
    OM_WriteDataToFile((VOS_UINT8*)pucMsg, usMsgLen);
#endif

    g_stUeToPcSucRecord.stTotalData.ulDataLen += usMsgLen;
    g_stUeToPcSucRecord.stTotalData.ulNum++;

    /* ������� */
    if ( (0 == usMsgLen) || (VOS_NULL_PTR == pucMsg ) )
    {
        LogPrint("OM_SendData: The input parameter is wrong.\n");

        g_stUeToPcErrRecord.stParaErrData.ulDataLen += usMsgLen;
        g_stUeToPcErrRecord.stParaErrData.ulNum++;

        return VOS_ERR;
    }

    if( VOS_FALSE != VOS_CheckInterrupt() )
    {
        g_stUeToPcErrRecord.usSendInIsrErr++;
        return VOS_ERR;
    }

    pstAppMsg = (OM_APP_MSG_EX_STRU*)pucMsg;

    if ((OM_STATE_ACTIVE == g_ulOMSwitchOnOff)
        || ((OM_QUERY_FUNC == pstAppMsg->ucFuncType)
            &&(OM_APP_ESTABLISH_CNF == pstAppMsg->usPrimId)))
    {
        if(VOS_OK != OMRL_MsgSplit((VOS_UINT8*)pucMsg, usMsgLen))
        {
            g_stUeToPcErrRecord.stLostData.ulDataLen += usMsgLen;
            g_stUeToPcErrRecord.stLostData.ulNum++;
        }

        return VOS_OK;

    }

    g_stUeToPcErrRecord.stLinkErrData.ulDataLen += usMsgLen;
    g_stUeToPcErrRecord.stLinkErrData.ulNum++;

    return VOS_ERR;
}
VOS_VOID OM_SendResult(VOS_UINT8 ucFuncType,
                            VOS_UINT32 ulResult, VOS_UINT16 usReturnPrimId)
{
    OM_APP_MSG_EX_STRU stOmToAppMsg;
    stOmToAppMsg.ucFuncType = ucFuncType;
    stOmToAppMsg.usLength   = OM_APP_MSG_EX_LEN;
    stOmToAppMsg.usPrimId   = usReturnPrimId;
    stOmToAppMsg.usToolId   = g_usToolId;
    *((VOS_UINT32*)(stOmToAppMsg.aucPara)) = ulResult;
    OM_SendData((OM_RSP_PACKET_STRU*)&stOmToAppMsg, (VOS_UINT16)sizeof(OM_APP_MSG_EX_STRU));
    return;
}

VOS_VOID OM_SendContent(VOS_UINT8 ucFuncType,
                             OM_APP_MSG_EX_STRU *pstOmToAppMsg, VOS_UINT16 usReturnPrimId)
{
    pstOmToAppMsg->ucFuncType = ucFuncType;
    pstOmToAppMsg->usPrimId   = usReturnPrimId;
    pstOmToAppMsg->usToolId   = g_usToolId;
    OM_SendData((OM_RSP_PACKET_STRU*)pstOmToAppMsg, (VOS_UINT16)(pstOmToAppMsg->usLength + VOS_OM_HEADER_LEN));
    return;
}
/*****************************************************************************
 �� �� ��  : OM_SendLog
 ��������  : ��LOG����ͨ��OM��������߲ࡣ
 �������  : pucLogData  - ָ�������ݵ�ָ��
             lLength     - �������ݵĳ���
 �������  : ��
 �� �� ֵ  : VOS_OK  - �ɹ�
             VOS_ERR - ʧ��

 �޸���ʷ      :
  1.��    ��   : 2008��9��9��
    ��    ��   : ���� 47350
    �޸�����   : �����ɺ���,��ӿ�ά�ɲ�����׶�����
*****************************************************************************/
VOS_UINT32 OM_SendLog(VOS_UINT8 *pucLogData, VOS_UINT32 ulLength)
{
    VOS_UINT8   *pucLogMsg;
    VOS_UINT32   ulLogMsgLen;

    /*���㷢�͸����߲�LOG��Ϣ�����ܳ��ȣ�����OM_APP_LOG_LEN��
    �Ѿ�������ModuleId�ĳ��ȣ�����Ҫ�����ȥ*/
    ulLogMsgLen = (ulLength + OM_APP_LOG_LEN) - LOG_MODULE_ID_LEN;
    pucLogMsg = (VOS_UINT8*)VOS_MemAlloc(WUEPS_PID_OM,
                                           DYNAMIC_MEM_PT, ulLogMsgLen);
    if (VOS_NULL_PTR == pucLogMsg)
    {
        printf("Om_SendLog, VOS_MemAlloc FAIL!\n");
        return VOS_ERR;
    }
    OM_AddAppHeader(OM_LOG_FUNC, pucLogData,
          (OM_APP_HEADER_STRU*)pucLogMsg, ulLogMsgLen);
    /*Sent to the tool side.*/
    OM_SendData((OM_RSP_PACKET_STRU*)pucLogMsg, (VOS_UINT16)ulLogMsgLen);
    VOS_MemFree(WUEPS_PID_OM, pucLogMsg);
    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : OM_GreenChannel
 ��������  : ��ɫͨ����OM�ṩ���ⲿ�ķ��ͽӿ�
 �������  : ucFuncType  - ����ID
             usPrimId    - ԭ��ֵ
             pucData     - ָ�������ݵ�ָ��
             usLen       - �������ݵĳ���
 �������  : ��
 �� �� ֵ  : VOS_OK  - �ɹ�
             VOS_ERR - ʧ��

 �޸���ʷ      :
  1.��    ��   : 2009��6��22��
    ��    ��   : ���� 47350
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 OM_GreenChannel(VOS_UINT8 ucFuncType, VOS_UINT16 usPrimId,
                            VOS_UINT8 *pucData, VOS_UINT16 usLen)
{
    OM_APP_TRACE_STRU      *pstAppMsg;
    VOS_UINT32              ulResult;

    pstAppMsg = (OM_APP_TRACE_STRU*)VOS_MemAlloc(WUEPS_PID_OM,
                                         DYNAMIC_MEM_PT, usLen + OM_APP_TRACE_LEN);
    if (NULL == pstAppMsg)
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_GreenChannel:VOS_MemAlloc.\n");
        return VOS_ERR;
    }

    pstAppMsg->stAppHeader.ucFuncType = ucFuncType;
    pstAppMsg->stAppHeader.usLength   = (usLen + OM_APP_TRACE_LEN) - VOS_OM_HEADER_LEN;
    pstAppMsg->usPrimId = usPrimId;
    pstAppMsg->usToolId = 0;

    OM_AddSNTime(&(pstAppMsg->stAppHeader.ulSn), &(pstAppMsg->stAppHeader.ulTimeStamp));

    /*�������ݳ���Ϊ0����ʾû�з�������*/
    if ((0 != usLen) && (VOS_NULL_PTR != pucData))
    {
        VOS_MemCpy(pstAppMsg->aucPara, pucData, usLen);
    }

    ulResult = OM_SendData((OM_RSP_PACKET_STRU*)pstAppMsg, usLen + OM_APP_TRACE_LEN);
    VOS_MemFree(WUEPS_PID_OM, pstAppMsg);

    return ulResult;
}
VOS_VOID OM_SavePSConfigInfo(VOS_UINT32* pulModuleIds,VOS_UINT32 ulModuleNum, VOS_UINT32 ulConfigBit)
{
    VOS_UINT32 ulModuleIndex;
    VOS_UINT32 ulModuleId;

    if (OM_ALL_MSG_ON == ulModuleNum)
    {
        for (ulModuleIndex = 0; ulModuleIndex < OM_MAX_PS_MODULE_NUM; ulModuleIndex++)
        {
            g_stTraceEventConfig.aulRptConfigLevl[ulModuleIndex] |= ulConfigBit;
        }

        return;
    }

    /* ����ϴ����� */
    for (ulModuleIndex = 0; ulModuleIndex < OM_MAX_PS_MODULE_NUM; ulModuleIndex++)
    {
        g_stTraceEventConfig.aulRptConfigLevl[ulModuleIndex] &= (~ulConfigBit);
    }

    /* �������� */
    for (ulModuleIndex = 0; ulModuleIndex < ulModuleNum; ulModuleIndex++)
    {
        ulModuleId = pulModuleIds[ulModuleIndex];

        if ((VOS_PID_DOPRAEND <= ulModuleId) && (VOS_PID_BUTT > ulModuleId))
        {
            g_stTraceEventConfig.aulRptConfigLevl[ulModuleId-VOS_PID_DOPRAEND] |= ulConfigBit;
        }
    }

    return;
}


VOS_UINT32 OM_CheckMsgPID(VOS_UINT32 ulModuleId, VOS_UINT32 ulConfigBit)
{
    VOS_UINT32 ulResult;

    ulResult = NO_CONFIG_BIT;

    if ((VOS_PID_DOPRAEND <= ulModuleId) && (VOS_PID_BUTT > ulModuleId))
    {
        ulResult = (g_stTraceEventConfig.aulRptConfigLevl[ulModuleId - VOS_PID_DOPRAEND] & ulConfigBit);
    }

    return ulResult;
}

/*lint -e416 -e661 -e662 -e717 �޸���:����*/

VOS_VOID OM_ConfigMsgProc(OM_REQ_PACKET_STRU *pRspPacket, OM_RSP_FUNC *pRspFuncPtr)
{
    APP_OM_MSG_EX_STRU             *pstAppToOmMsg;
    OM_APP_TRACE_STRU               stOmToAppMsg;
    OM_TRACE_CONFIG_REQ_STRU       *pstTraceConfig;
    OM_TRACE_MASK_CONFIG_REQ_STRU  *pstTraceMaskCfg;
    OM_EVENT_CONFIG_TABLE_STRU     *pstEventConfig;
    VOS_UINT32                      ulModuleNum;

    pstAppToOmMsg = (APP_OM_MSG_EX_STRU*)pRspPacket;
    stOmToAppMsg.stAppHeader.ucFuncType = pstAppToOmMsg->ucFuncType;
    /*Please refer to the OM_APP_MSG_EX_STRU struct.*/
    stOmToAppMsg.stAppHeader.usLength = sizeof(OM_APP_TRACE_STRU) - VOS_OM_HEADER_LEN;
    OM_AddSNTime(&(stOmToAppMsg.stAppHeader.ulSn),
                 &(stOmToAppMsg.stAppHeader.ulTimeStamp));
    stOmToAppMsg.usToolId = pstAppToOmMsg->usToolId;
    *((VOS_UINT32*)stOmToAppMsg.aucPara) = VOS_OK;

    switch (pstAppToOmMsg->ucFuncType)
    {
        case OM_TRACE_FUNC:
            if (APP_OM_TRACE_REQ == pstAppToOmMsg->usPrimId)
            {
                stOmToAppMsg.usPrimId = OM_APP_TRACE_CNF;
                pstTraceConfig = (OM_TRACE_CONFIG_REQ_STRU*)(pstAppToOmMsg->aucPara);
                ulModuleNum = pstTraceConfig->ulModuleNum;

                /* ����trace������Ϣ */
                VOS_MemCpy(&g_stTraceEventConfig, pstTraceConfig, sizeof(VOS_UINT32));
                OM_SavePSConfigInfo(pstTraceConfig->aulModuleId, pstTraceConfig->ulModuleNum, TRACE_CONFIG_BIT);

#ifndef COMM_ITT
                TTF_TraceSet(pstTraceConfig->usL2Msg);
                TTF_TraceModuleSet((VOS_INT32)ulModuleNum, pstTraceConfig->aulModuleId);
#endif
            }

            if (APP_OM_TRACE_MASK_REQ == pstAppToOmMsg->usPrimId)
            {
                stOmToAppMsg.usPrimId = OM_APP_TRACE_MASK_CNF;

                pstTraceMaskCfg = (OM_TRACE_MASK_CONFIG_REQ_STRU*)(pstAppToOmMsg->aucPara);

                OM_SavePSConfigInfo(pstTraceMaskCfg->aulModuleId, pstTraceMaskCfg->ulModuleNum, TRACE_MASK_CONFIG_BIT);
            }

            break;

        case OM_EVENT_FUNC:
            stOmToAppMsg.usPrimId = OM_APP_EVENT_CNF;
            pstEventConfig = (OM_EVENT_CONFIG_TABLE_STRU*)(pstAppToOmMsg->aucPara);
            ulModuleNum = pstEventConfig->ulModuleNum;

            OM_SavePSConfigInfo(pstEventConfig->aulModuleId,pstEventConfig->ulModuleNum, EVENT_CONFIG_BIT);

            break;
        default:
            break;
    }
    OM_SendData((OM_RSP_PACKET_STRU*)&stOmToAppMsg, sizeof(OM_APP_TRACE_STRU));
    return;
}

VOS_VOID OM_TransMsgProc(OM_REQ_PACKET_STRU *pRspPacket, OM_RSP_FUNC *pRspFuncPtr)
{
    MsgBlock            *pstMsg;
    APP_OM_MSG_EX_STRU  *pstAppMsg;

    pstAppMsg = (APP_OM_MSG_EX_STRU*)pRspPacket;

    /*PC���߷�����͸����Ϣ����*/
    if (APP_OM_TRANS_CONFIG == pstAppMsg->usPrimId)
    {
        g_bOmTransSwitch = VOS_TRUE;
        return;
    }

    /*�����ж�*/
    if (VOS_MSG_HEAD_LENGTH >= pRspPacket->usLength)
    {
        PS_LOG1(WUEPS_PID_OM, 0, PS_PRINT_ERROR," OM_TransMsgProc: usLength is err.",
                (VOS_INT32)pRspPacket->usLength);
        return;
    }

    pstMsg = VOS_AllocMsg(WUEPS_PID_OM, pRspPacket->usLength - VOS_MSG_HEAD_LENGTH);
    if (VOS_NULL_PTR == pstMsg)
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_TransMsgProc: VOS_AllocMsg fail.");
        return;
    }
    else
    {
        VOS_MemCpy((VOS_UINT8*)pstMsg, (VOS_UINT8*)pRspPacket->aucCommand, pRspPacket->usLength);

        /* ���ӶԽ�����PID�Ĳ�����顡*/
        if (VOS_OK != VOS_CheckRcvPid(pstMsg->ulReceiverPid))
        {
            PS_LOG1(WUEPS_PID_OM, 0, PS_PRINT_ERROR," OM_TransMsgProc: Receiver Pid is err.",
                (VOS_INT32)pstMsg->ulReceiverPid);

            VOS_FreeMsg(WUEPS_PID_OM, pstMsg);

            return;
        }

        (VOS_VOID)VOS_SendMsg(WUEPS_PID_OM, pstMsg);
    }
    return;
}
VOS_VOID OM_RfMsgProc(OM_REQ_PACKET_STRU *pRspPacket, OM_RSP_FUNC *pRspFuncPtr)
{
    MsgBlock   *pstMsg;
    VOS_UINT16  usLength;

    usLength = pRspPacket->usLength;

    pstMsg = VOS_AllocMsg(WUEPS_PID_OM, usLength);
    if (VOS_NULL_PTR == pstMsg)
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_RfMsgProc: VOS_AllocMsg.\n");
        return;
    }

    if (OM_WRF_FUNC == pRspPacket->ucModuleId)
    {
        pstMsg->ulReceiverPid = DSP_PID_WPHY;
    }
    else
    {
        if(MODEM_ID_1 == OM_GET_FUNCID_MODEMINFO(pRspPacket->ucModuleId))
        {
            pstMsg->ulReceiverPid = I1_DSP_PID_GPHY;   /*���͸���1��PID*/
        }
        else
        {
            pstMsg->ulReceiverPid = I0_DSP_PID_GPHY;   /*���͸���0��PID*/
        }
    }

    VOS_MemCpy((VOS_UINT8*)(pstMsg->aucValue), (VOS_UINT8*)(pRspPacket->aucCommand), usLength);

    (VOS_VOID)VOS_SendMsg(WUEPS_PID_OM, pstMsg);

    return;
}


VOS_VOID OM_TransferMsgProc(OM_REQ_PACKET_STRU *pRspPacket, OM_RSP_FUNC *pRspFuncPtr)
{
    MsgBlock   *pstMsg;
    VOS_UINT16  usLength;

    usLength = pRspPacket->usLength;

    pstMsg = VOS_AllocMsg(WUEPS_PID_OM, usLength + VOS_OM_HEADER_LEN);

    if (VOS_NULL_PTR == pstMsg)
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_RfMsgProc: VOS_AllocMsg.\n");
        return;
    }

    pstMsg->ulReceiverPid = WUEPS_PID_NOSIG;

    VOS_MemCpy((VOS_UINT8*)(pstMsg->aucValue), (VOS_UINT8*)pRspPacket, usLength + VOS_OM_HEADER_LEN);

    (VOS_VOID)VOS_SendMsg(WUEPS_PID_OM, pstMsg);

    return;
}


VOS_UINT32 Om_CheckBufferIsFull(VOS_VOID)
{
    if (DSP_SEND_OM_MSG_MAX_NUM > VOS_GetQueueSizeFromFid(WUEPS_FID_OM))
    {
        return VOS_OK;
    }

    return VOS_ERR;
}

/*Global map table used to find the function according the FuncId.*/
OM_FUNCID_PROC_STRU g_astOmFuncIdProcTbl[OM_FUNCID_MAX_NUM] =
{
    {OM_TRACE_FUNC,    OM_ConfigMsgProc,        LEVEL_NORMAL},
    {OM_AIR_FUNC,      OM_TransMsgProc,         LEVEL_NORMAL},
    {OM_FS_FUNC,       OM_FileSystemMsgProc,    LEVEL_NORMAL},
    {OM_TRANS_FUNC,    OM_TransMsgProc,         LEVEL_NORMAL},
    {OM_EVENT_FUNC,    OM_ConfigMsgProc,        LEVEL_NORMAL},
    {OM_LOG_FUNC,      Log_OmMsgProc,           LEVEL_NORMAL},
    {OM_WRF_FUNC,      OM_RfMsgProc,            LEVEL_NORMAL},
    {OM_GRF_FUNC,      OM_RfMsgProc,            LEVEL_NORMAL},
    {OM_AUTH_NV_FUNC,  OM_QueryMsgProc,         LEVEL_NORMAL},
    {OM_QUERY_FUNC,    OM_QueryMsgProc,         LEVEL_NORMAL},
    {OM_NV_FUNC,       OM_QueryMsgProc,         LEVEL_NORMAL},
    {OM_SIGCAL_FUNC,   OM_TransMsgProc,         LEVEL_NORMAL},
    {OM_GREEN_FUNC,    VOS_NULL_PTR,            LEVEL_NORMAL},
    {OM_PRINTF_FUNC,   VOS_NULL_PTR,            LEVEL_NORMAL},
    {OM_READSD_FUNC,   VOS_NULL_PTR,            LEVEL_NORMAL},
    {OM_NOSIG_FUNC,    OM_TransferMsgProc,      LEVEL_NORMAL},
    {OM_BBP_FUNC,      Om_CcpuBbpConfigMsgProc, LEVEL_NORMAL}
};

VOS_INT32  OM_RegisterRequestProcedure(VOS_UINT8 ucModuleId,
                                                OM_REQUEST_PROCEDURE *pReqPro)
{
    if ((OM_FUNCID_PART_NUM >= ucModuleId) || (OM_FUNCID_ACPU_PART_NUM < ucModuleId)
        || (0 != g_astOmFuncIdProcTbl[ucModuleId - 1].ulFuncType))
    {
        return VOS_ERR;
    }
    g_astOmFuncIdProcTbl[ucModuleId - 1].ulFuncType = (VOS_UINT32)ucModuleId;
    g_astOmFuncIdProcTbl[ucModuleId - 1].pOmFuncProc = pReqPro;
    return VOS_OK;
}


VOS_UINT32 OM_CcpuStartEventTimer(VOS_VOID)
{
    VOS_UINT32          ulRtn;

    ulRtn = VOS_StartRelTimer(&g_stRegularBufFullTmr, WUEPS_PID_OM,
                              OM_BUFF_TIMER_LENTH,
                              OM_BUFF_TIMER, 0,
                              VOS_RELTIMER_NOLOOP, VOS_TIMER_NO_PRECISION);

    if (VOS_OK != ulRtn)
    {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_VOID OM_TimerMsgProc(MsgBlock* pMsg)
{
    VOS_UINT32 ulName;

    ulName = ((REL_TIMER_MSG*)pMsg)->ulName;

    if (OM_FILE_TIMER == ulName)
    {
        /*�յ���ʱ����ʱ��Ϣ�������빤�߲��Ľ���ʧ��*/
        OM_CloseFile();

        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "OM_TimerMsgProc:Recv timer and close file.");
    }
    else if (OM_BUFF_TIMER == ulName)
    {
        OMRL_CcpuReportEvent();

        if(VOS_OK != OM_CcpuStartEventTimer())
        {
            PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "OM_TimerMsgProc: OM_CcpuStartEventTimer failed.\n");

        }
    }
    else
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "OM_TimerMsgProc:Unknown timer.");
    }
}
VOS_VOID OM_MsgProc(MsgBlock* pMsg)
{
    OM_REQ_PACKET_STRU    *pstAppMsg = VOS_NULL_PTR;
    APP_OM_MSG_EX_STRU    *pstAppToOmMsg;
    VOS_UINT16             usPrimId;
    OM_APP_HEADER_STRU    *pstAppHeader;
    OM_REQUEST_PROCEDURE  *pOmFuncProc;
    VOS_UINT32             ulCurrentId;
    VOS_UINT32             ulOriginalId;

    if (VOS_NULL_PTR == pMsg)
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_MsgProc:pMsg is empty.");
        return;
    }

    OM_RecordInfoStart(VOS_EXC_DUMP_MEM_NUM_2, pMsg->ulSenderPid, WUEPS_PID_OM, *((VOS_UINT32*)pMsg->aucValue));

    /*�������Թ��߲����Ϣ*/
    if (UEPS_PID_OMRL == pMsg->ulSenderPid)
    {
        pstAppMsg = (OM_REQ_PACKET_STRU*)pMsg->aucValue;

        /* ��2BitΪ����modem��ʾ */
        ulOriginalId = (VOS_UINT32)(pstAppMsg->ucModuleId&OM_FUNCID_VALUE_BITS);

        /*ID��Ҫ��ΪOM�ڲ�ʹ�� �� ע��ص�ʹ�õ�ID����Ҫ�ֱ���в������*/
        if ((OM_TRACE_FUNC > ulOriginalId)
            || ((OM_FUNC_ID_BUTT <= ulOriginalId) && (OM_FUNCID_PART_NUM >= ulOriginalId))
            || (OM_FUNCID_MAX_NUM < ulOriginalId))
        {
            PS_LOG1(WUEPS_PID_OM, 0, PS_PRINT_WARNING,
                   "OM_MsgProc:ulOriginalId is overflow :", (VOS_INT32)ulOriginalId);

            OM_RecordInfoEnd(VOS_EXC_DUMP_MEM_NUM_2);

            return;
        }

        ulCurrentId = g_astOmFuncIdProcTbl[ulOriginalId - 1].ulFuncType;
        pOmFuncProc = g_astOmFuncIdProcTbl[ulOriginalId - 1].pOmFuncProc;

        if ((VOS_NULL_PTR == pOmFuncProc) || (ulOriginalId != ulCurrentId))
        {
            PS_LOG2(WUEPS_PID_OM, 0, PS_PRINT_INFO,
                    "OM_MsgProc: Func Id: %d, FuncProc :%x.\n",
                    (VOS_INT32)ulOriginalId, (VOS_INT32)pOmFuncProc);

            OM_RecordInfoEnd(VOS_EXC_DUMP_MEM_NUM_2);

            return;
        }

        if (OM_FUNCID_PART_NUM >= ulOriginalId)
        {
            pstAppToOmMsg = (APP_OM_MSG_EX_STRU*)pMsg->aucValue;

            g_usToolId = pstAppToOmMsg->usToolId;

            pOmFuncProc(pstAppMsg, OM_SendData);
        }
        /*����CCPU CallBack����������*/
        else if ((OM_FUNCID_PART_NUM < ulOriginalId) && (OM_FUNCID_ACPU_PART_NUM >= ulOriginalId))
        {
            VOS_ReserveMsg(WUEPS_PID_OM, pMsg);

            pMsg->ulReceiverPid = WUEPS_PID_OM_CALLBACK;

            (VOS_VOID)VOS_SendMsg(WUEPS_PID_OM_CALLBACK, pMsg);
        }
        else
        {
            ;
        }

    }
    else if (VOS_PID_TIMER == pMsg->ulSenderPid)
    {
        OM_TimerMsgProc(pMsg);
    }
    else
    {
        usPrimId = *((VOS_UINT16*)pMsg->aucValue);
        /*If the message is transparent*/
        if (OM_TRANS_PRIMID == usPrimId)
        {
            /*2*sizeof(VOS_UINT16)����PS_OM_TRANS_IND_STRU�е�
             usTransPrimId + usReserve��ռ�ռ��С*/
            pstAppHeader = (OM_APP_HEADER_STRU*)((VOS_UINT8*)(pMsg->aucValue) + (2*sizeof(VOS_UINT16)));

            /*�����ǰ͸����ϢΪ�ر�״̬��ֱ�ӷ���*/
            if ((OM_TRANS_FUNC == pstAppHeader->ucFuncType)
                &&(VOS_FALSE == g_bOmTransSwitch))
            {
                OM_RecordInfoEnd(VOS_EXC_DUMP_MEM_NUM_2);

                return;
            }

            if ((OM_TRANS_FUNC == pstAppHeader->ucFuncType)
                || (OM_AIR_FUNC == pstAppHeader->ucFuncType))
            {
                OM_AddSNTime(&(pstAppHeader->ulSn), VOS_NULL_PTR);
            }
            OM_SendData((OM_RSP_PACKET_STRU*)pstAppHeader, (VOS_UINT16)(pMsg->ulLength - (2*sizeof(VOS_UINT16))));
        }
        /*Recevie the unknown msg, OM should print log info.*/
        else
        {
             PS_LOG2(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "OM_MsgProc : Unknown Msg, Send Pid: %d, Prim Id: %d\n",
                                                     (VOS_INT32)(pMsg->ulSenderPid), (VOS_INT32)usPrimId);
        }
    }

    OM_RecordInfoEnd(VOS_EXC_DUMP_MEM_NUM_2);

    return;
}
/*lint +e416 +e661 +e662 +e717*/

VOS_UINT32 OM_TraceMsgHook(VOS_VOID* pMsg)
{
    MsgBlock                    *pNewMsg;
    VOS_UINT32                   ulTraceMsgLen;
    VOS_UINT32                   ulPidConfigFlag;
#ifndef COMM_ITT
    PS_BOOL_ENUM_UINT8           bResult;
#endif
    OM_APP_TRACE_STRU            stAppTrace;

    ulPidConfigFlag = NO_CONFIG_BIT;

    if (VOS_NULL_PTR == pMsg)
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_TraceMsgHook: Input para is NULL.\n");
        return VOS_OK;
    }

    OM_NoSigFilterMsg(pMsg);

    pNewMsg = (MsgBlock*)pMsg;
    /*Check if the current msg is system msg.*/
    if (pNewMsg->ulSenderPid < VOS_PID_DOPRAEND)
    {
        if (0 == g_stTraceEventConfig.usSysMsg)
        {
            PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_INFO, "OM_TraceMsgHook, System message is filtered.!\n");
            return VOS_OK;
        }
    }
    /*If the current msg is not system msg, we should filter the msgs according the config sent by tools.*/
    else
    {
        /*OM module' msg don't be displayed.*/
        if ((pNewMsg->ulReceiverPid == WUEPS_PID_OM) ||(pNewMsg->ulSenderPid == WUEPS_PID_OM))
        {
            return VOS_OK;
        }

        /*Filter the msg by the pid of msg.*/
        if (TRACE_MASK_CONFIG_BIT == OM_CheckMsgPID(pNewMsg->ulReceiverPid, TRACE_MASK_CONFIG_BIT))
        {
            return VOS_OK;
        }

        ulPidConfigFlag = OM_CheckMsgPID(pNewMsg->ulSenderPid, TRACE_CONFIG_BIT);

        ulPidConfigFlag |= OM_CheckMsgPID(pNewMsg->ulReceiverPid, TRACE_CONFIG_BIT);

        if (NO_CONFIG_BIT == ulPidConfigFlag)
        {
            return VOS_OK;
        }
    }

#ifndef COMM_ITT
    /*Construct the L2 message by the interface provided by TTF group.*/
    pNewMsg = TTF_TraceMsg(pNewMsg);
    /*Filer the msg by the interface by WAS group.*/
    bResult = WAS_MNTN_FilterMsg(pNewMsg);
    if (PS_FALSE == bResult)
    {
        return VOS_OK;
    }
    /*Filer the msg by the interface by TTF group.*/
    bResult =  TTF_FilterMsg(pNewMsg);
    if (PS_FALSE == bResult)
    {
        return VOS_OK;
    }
#endif

    /*�������ⵥ:AT2D04483 ���� HPA ITT׮��Ϣ */
#ifdef HPA_ITT
    if((MAPS_STUB_RCVMSG_PID == pNewMsg->ulSenderPid)
    || (MAPS_STUB_RCVMSG_PID == pNewMsg->ulReceiverPid))
    {
        return VOS_OK;
    }

    if((MAPS_STUB_SENDMSG_PID == pNewMsg->ulSenderPid)
    || (MAPS_STUB_SENDMSG_PID == pNewMsg->ulReceiverPid))
    {
        return VOS_OK;
    }
#endif

    /*Construct the msg sent to tool side.*/
    ulTraceMsgLen = OM_APP_TRACE_LEN + VOS_MSG_HEAD_LENGTH + pNewMsg->ulLength;

    /*��USB֡��ʽ�����Ż����ٿ�������*/
    stAppTrace.stAppHeader.ucFuncType = OM_TRACE_FUNC;
    stAppTrace.stAppHeader.usLength   = (VOS_UINT16)(ulTraceMsgLen - VOS_OM_HEADER_LEN);

    OM_AddSNTime(&(stAppTrace.stAppHeader.ulSn),
                    &(stAppTrace.stAppHeader.ulTimeStamp));
    stAppTrace.usPrimId   = OM_APP_TRACE_IND;
    *((VOS_UINT32*)stAppTrace.aucPara)  = (VOS_UINT32)pNewMsg;

    /*Sent to the tool side.*/
    if(VOS_OK == OM_SendData((OM_RSP_PACKET_STRU*)&stAppTrace, (VOS_UINT16)ulTraceMsgLen))
    {
        /* ��ά�ɲ⣬��ӦPID���ͳɹ������� */
        if ((VOS_PID_DOPRAEND <= pNewMsg->ulSenderPid) && (VOS_PID_BUTT > pNewMsg->ulSenderPid))
        {
            g_aulPidTraceSendNum[pNewMsg->ulSenderPid - VOS_PID_DOPRAEND]++;
        }
    }
    return VOS_OK;
}

VOS_UINT32 OM_Event(PS_OM_EVENT_IND_STRU *pstEvent)
{
    VOS_UINT32  ulTotalLen;
    VOS_UINT8  *pucEventPara = VOS_NULL_PTR;

    if (VOS_NULL_PTR == pstEvent)
    {
        PS_LOG( WUEPS_PID_OM, 0, PS_PRINT_ERROR," OM_Event:Input para is NULL\n");
        return VOS_ERR;
    }

    if (NO_CONFIG_BIT == OM_CheckMsgPID(pstEvent->ulModuleId, EVENT_CONFIG_BIT))
    {
        PS_LOG( WUEPS_PID_OM, 0, PS_PRINT_ERROR," OM_Event: Event Module Id is not configed\n");
        return VOS_ERR;
    }

    /*Construct the event msg sent to tool side.*/
    ulTotalLen = sizeof(OM_APP_HEADER_STRU) + pstEvent->ulLength;
    pucEventPara = (VOS_UINT8 *)VOS_MemAlloc(WUEPS_PID_OM, DYNAMIC_MEM_PT, ulTotalLen);

    if (VOS_NULL_PTR == pucEventPara)
    {
        PS_LOG( WUEPS_PID_OM, 0, PS_PRINT_ERROR," OM_Event:VOS_MemAlloc\n");
        return VOS_ERR;
    }
    OM_AddAppHeader(OM_EVENT_FUNC, (VOS_UINT8*)pstEvent,
                                 (OM_APP_HEADER_STRU*)pucEventPara, ulTotalLen);

    if (VOS_OK == OM_SendData((OM_RSP_PACKET_STRU*)pucEventPara, (VOS_UINT16)ulTotalLen))
    {
        /* ��ά�ɲ⣬��ӦPID���ͳɹ������� */
        if ((VOS_PID_DOPRAEND <= pstEvent->ulModuleId) && (VOS_PID_BUTT > pstEvent->ulModuleId))
        {
            g_aulPidEventSendNum[pstEvent->ulModuleId - VOS_PID_DOPRAEND]++;
        }
    }

    VOS_MemFree(WUEPS_PID_OM, pucEventPara);
    return VOS_OK;
}


VOS_UINT32  OM_GetDevVersionInfo(OM_DEVICE_VERSION_INFO_STRU *pDevVersionInfo )
{
    VOS_UINT8                         aucBuffer[OM_STR_MAX_LEN] = {0};
    VOS_UINT32                        ulRslt;

    if (VOS_NULL_PTR == pDevVersionInfo)
    {
        return VOS_ERR;
    }

    ulRslt = (VOS_UINT32)DRV_MEM_VERCTRL((VOS_INT8*)aucBuffer,
                                 OM_RF_VER_LEN, VER_HARDWARE, VERIONREADMODE);/* ��ȡӲ��RF�汾 */
    if (VOS_OK != ulRslt)
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "Get HardWare RF Version fail!");
    }
    else
    {
        VOS_MemCpy(&pDevVersionInfo->ulHwVersion, aucBuffer, OM_RF_VER_LEN);
    }
    return ulRslt;
}
#if 0
/*****************************************************************************
 Prototype      : OM_WriteChipVersion
 Description    : д��оƬ�İ汾��Ϣ
 Input          : оƬ��Ϣ�ļĴ�����ַ���ĸ�оƬ
 Output         : None
 Return Value   : None
 History        : ---
  1.��    ��  : 2009��1��12��
    ��    ��  : ����
    �޸�����  : ������
*****************************************************************************/
VOS_VOID OM_WriteChipVersion(VOS_UINT32 ulAddress, VOS_UINT8 ucType)
{
    VOS_UINT32     ulRegValue;
    VOS_UINT32     ulRslt;
    VOS_CHAR       acSocVersionInfo[VER_MAX_LENGTH] = {0};

    ulRegValue = HPA_Read32Reg(ulAddress);

    DRV_MEM_VERCONVERT(ulRegValue, acSocVersionInfo);

    ulRslt = (VOS_UINT32)DRV_MEM_VERCTRL((VOS_INT8*)acSocVersionInfo,
                    VER_MAX_LENGTH, ucType, VERIONWRITEMODE);
    if( VOS_OK != ulRslt )
    {
        PS_LOG1( WUEPS_PID_OM,0,PS_PRINT_WARNING,
            " Write %d Version Info fail. \n", ucType);
    }
}
#endif
/*****************************************************************************
 Prototype      : OM_StaVersionInfoInit
 Description    : ��̬�汾��Ϣ��ʼ��
 Input          : pStaVersionInfo ��̬�汾��Ϣ�ṹ��ָ��
 Output         : None
 Return Value   : None
 History        : ---
  1.��    ��  : 2006��11��15��
    ��    ��  : luojian id:60022475
    �޸�����  : �������ⵥ��A32D06616
  2.��    ��   : 2007��02��25��
    ��    ��   : luojian 60022475
    �޸�����   : ���ⵥ�ţ�A32D08962,140оƬ�汾��Ϣ�޸�
*****************************************************************************/
VOS_UINT32  OM_StaVersionInfoInit()
{
#if ( (VOS_OS_VER != VOS_WIN32) && ( !defined __PS_WIN32_RECUR__ ) )
    VOS_UINT32                     ulRslt;
    VOS_CHAR                       acVersionBuffer[VER_MAX_LENGTH]  = {0};

    DRV_INIT_PLATFORM_VER();

    VOS_StrNCpy(acVersionBuffer, OAM_PLATFORM_VERSION, VER_MAX_LENGTH);

    ulRslt = (VOS_UINT32)DRV_MEM_VERCTRL((VOS_INT8*)acVersionBuffer,
                    (VOS_UINT8)VER_MAX_LENGTH, (VOS_UINT8)VER_OAM, (VOS_UINT8)VERIONWRITEMODE);
    if( VOS_OK != ulRslt )
    {
        PS_LOG( WUEPS_PID_OM, 0, PS_PRINT_WARNING," Write OAM Version Info fail. \n");
    }

#endif

    return 0;
}


VOS_UINT32 OM_GetSlice(VOS_VOID)
{
    /* Add for L Only version */
    if (BOARD_TYPE_LTE_ONLY == BSP_OM_GetBoardType())
    {
        return VOS_GetTick();
    }
    /* Add for L Only version */

#if (VOS_WIN32 != VOS_OS_VER)
    return DRV_GET_SLICE();
#else
    return VOS_GetTick();
#endif
}


VOS_UINT32 OmGetUsimTemp(VOS_INT *plUsimTemp)
{
    VOS_INT16                           sTemp;
    VOS_UINT32                          ulResult = 0;

    /*ͨ������ӿڻ�ȡ��ǰSIM���¶�*/
    ulResult = (VOS_UINT32)DRV_HKADC_GET_TEMP(HKADC_TEMP_SIM_CARD, VOS_NULL_PTR, &sTemp, HKADC_CONV_DELAY);

    *plUsimTemp = sTemp;

    if (VOS_OK != ulResult)
    {
        return VOS_ERR;
    }
    return VOS_OK;
}
VOS_UINT32 OmGetAfcLockFlag(VOS_UINT16 *pusAfcCurrVal)
{
    VOS_UINT32 ulRegValue;

    ulRegValue = HPA_Read32Reg(WBBP_AFC_STATUS_ADDR);
    if (0 == (ulRegValue & W_BBP_AFC_LOCK_IND_MASK))
    {
        return VOS_NO;
    }

    *pusAfcCurrVal = (VOS_UINT16)(ulRegValue & W_BBP_AFC_LOCK_VALUE_MASK);

    return VOS_YES;
}


VOS_UINT32 OM_HsicPortInitNotify(VOS_VOID)
{
    OM_HSIC_INIT_NOTIFY_MSG_STRU       *pstOmHsicInitMsg;

    /* �����֧��HSIC������ʼ�� */
    if (BSP_MODULE_SUPPORT != DRV_GET_HSIC_SUPPORT())
    {
        return VOS_OK;
    }

    pstOmHsicInitMsg = (OM_HSIC_INIT_NOTIFY_MSG_STRU *)VOS_AllocMsg(WUEPS_PID_OM_CALLBACK,
                                                                    sizeof(OM_HSIC_INIT_NOTIFY_MSG_STRU) - VOS_MSG_HEAD_LENGTH);

    /* �����ڴ�ʧ�ܣ����ش��� */
    if (VOS_NULL_PTR == pstOmHsicInitMsg)
    {
        return VOS_ERR;
    }

    /* ��Ϣ�յ�OM_CALLBACK������HSIC�ĳ�ʼ�� */
    pstOmHsicInitMsg->ulMsgName     = OM_HSIC_INIT_NOTIFY;
    pstOmHsicInitMsg->ulReceiverPid = WUEPS_PID_OM_CALLBACK;

    return VOS_SendMsg(WUEPS_PID_OM_CALLBACK, pstOmHsicInitMsg);
}

VOS_VOID OM_RecordInfoEnd(VOS_EXC_DUMP_MEM_NUM_ENUM_UINT32 enNumber)
{
    VOS_UINT32 *pulBuf;

    if(VOS_EXC_DUMP_MEM_NUM_BUTT <= enNumber)
    {
        return;
    }

    if(VOS_NULL_PTR == g_astRecordInfo[enNumber].pucBuf)
    {
        return;
    }

    if(0 == g_astRecordInfo[enNumber].ulLen)
    {
        return;
    }

    /* ��start���Ѿ�����˼�¼endslice�ĳ��ȣ���˴˴������ĸ��ֽ���дendslice��ֵ */
    pulBuf = (VOS_UINT32*)(g_astRecordInfo[enNumber].pucBuf + g_astRecordInfo[enNumber].ulLen - sizeof(VOS_UINT32));

    *pulBuf = VOS_GetSlice();

    return;
}

VOS_VOID OM_RecordInfoStart(VOS_EXC_DUMP_MEM_NUM_ENUM_UINT32 enNumber, VOS_UINT32 ulSendPid, VOS_UINT32 ulRcvPid, VOS_UINT32 ulMsgName)
{
    OM_RECORD_INFO_STRU     stRecordInfo;

    if(VOS_EXC_DUMP_MEM_NUM_BUTT <= enNumber)
    {
       return;
    }

    if(VOS_NULL_PTR == g_astRecordInfo[enNumber].pucBuf)
    {
       return;
    }

    g_astRecordInfo[enNumber].ulLen %= VOS_TASK_DUMP_INFO_SIZE;

    stRecordInfo.usSendPid      = (VOS_UINT16)ulSendPid;
    stRecordInfo.usRcvPid       = (VOS_UINT16)ulRcvPid;
    stRecordInfo.ulMsgName      = ulMsgName;
    stRecordInfo.ulSliceStart   = VOS_GetSlice();
    stRecordInfo.ulSliceEnd     = 0;

    VOS_MemCpy((g_astRecordInfo[enNumber].pucBuf + g_astRecordInfo[enNumber].ulLen),
              &stRecordInfo, sizeof(OM_RECORD_INFO_STRU));

    g_astRecordInfo[enNumber].ulLen += sizeof(OM_RECORD_INFO_STRU);

    return;
}



VOS_VOID OM_RecordMemInit(VOS_VOID)
{
   VOS_UINT32 i;

   VOS_MemSet(g_astRecordInfo, 0, sizeof(g_astRecordInfo));

   /* ����ÿ��ģ���¼��ν�ɲ���Ϣ�Ŀռ� */
   for(i = 0; i < VOS_EXC_DUMP_MEM_NUM_BUTT; i++)
   {
      g_astRecordInfo[i].pucBuf = (VOS_UINT8*)VOS_ExcDumpMemAlloc(i);

      if(VOS_NULL_PTR == g_astRecordInfo[i].pucBuf)
      {
          return;
      }
   }

   return;
}
VOS_UINT32 OM_Init(VOS_VOID)
{
    VOS_MemSet(g_astRecordAcpuToCcpu, 0, sizeof(g_astRecordAcpuToCcpu));

    if (VOS_OK != OM_StaVersionInfoInit())
    {
         PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "OM_Init: OM_StaVersionInfoInit.\n");
    }

    OM_RegisterMsgHook(OM_TraceMsgHook, OM_MSG_HOOK_GU);

    /*��ʼ����־�ļ�*/
    Log_InitFile();

    if (VOS_OK != OMRL_Init())
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "OM_Init: RL_Init failed.\n");
        return VOS_ERR;
    }

    if (VOS_OK != OM_HsicPortInitNotify())
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "OM_Init: OM_HsicPortInitNotify failed.\n");
        return VOS_ERR;
    }

    OM_InitAuthVariable();

    /* ��ʼ��PS������Ϣ */
    VOS_MemSet(&g_stTraceEventConfig, 0, sizeof(OM_TRACE_EVENT_CONFIG_PS_STRU));

    if(VOS_OK != OM_CcpuStartEventTimer())
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "OM_Init: OM_CcpuStartEventTimer failed.\n");
        return VOS_ERR;
    }

    if(VOS_OK != OM_FileSystemInit())
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "OM_Init: OM_FileSystemInit failed.\n");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 WuepsOmPidInit(enum VOS_INIT_PHASE_DEFINE ip)
{
    switch( ip )
    {
        case VOS_IP_LOAD_CONFIG:
            return OM_Init();

        case VOS_IP_INITIAL:
            return Om_BbpDumpEdmaInit();

            /* ��ȡ��ɫ��״̬��NAS����ʱ���г�ͻ */
        case VOS_IP_RESTART:
            DRV_LED_STATE_FUNREG(OM_RuningPsStateCheckCb);
            break;

        case VOS_IP_BROADCAST_STATE:
            OM_SendCcpuSocpVote(SOCP_VOTE_FOR_SLEEP);
            break;

        default:
            break;
    }

    return VOS_OK;
}
VOS_UINT32 WuepsOMFidInit(enum VOS_INIT_PHASE_DEFINE ip)
{
    VOS_UINT32 ulRslt;

    VOS_RegisterMsgGetHook((VOS_MSG_HOOK_FUNC)OM_MsgHook);


    switch( ip )
    {
        case VOS_IP_LOAD_CONFIG:
        {
            if (VOS_OK != APM_InitSystemAddr()) /*���������ʼ��HPA��صĻ���ַ��HPA��������ɾ��*/
            {
                return VOS_ERR;
            }

            ulRslt = VOS_RegisterPIDInfo(WUEPS_PID_OM,
                                        (Init_Fun_Type)WuepsOmPidInit,
                                        (Msg_Fun_Type)OM_MsgProc);
            if( VOS_OK != ulRslt )
            {
                return VOS_ERR;
            }

            ulRslt = VOS_RegisterPIDInfo(WUEPS_PID_REG,
                                        (Init_Fun_Type)WuepsRegPidInit,
                                        (Msg_Fun_Type)Reg_MsgProc);
            if( VOS_OK != ulRslt )
            {
                return VOS_ERR;
            }

            ulRslt = VOS_RegisterPIDInfo(WUEPS_PID_AGING,
                                        (Init_Fun_Type)WuepsAgingPidInit,
                                        (Msg_Fun_Type)Aging_MsgProc);
            if( VOS_OK != ulRslt )
            {
                return VOS_ERR;
            }

            ulRslt = VOS_RegisterPIDInfo(WUEPS_PID_RFA,
                                        (Init_Fun_Type)WuepsRfaPidInit,
                                        (Msg_Fun_Type) RFA_MsgProc);
            if( VOS_OK != ulRslt )
            {
                return VOS_ERR;
            }

            ulRslt = VOS_RegisterMsgTaskPrio(WUEPS_FID_OM, VOS_PRIORITY_M5);
            if( VOS_OK != ulRslt )
            {
                return VOS_ERR;
            }

#if ( VOS_WIN32 != VOS_OS_VER )
            ulRslt = VOS_RegisterSelfTaskPrio(WUEPS_FID_OM,
                                         (VOS_TASK_ENTRY_TYPE)Log_SelfTask,
                                         COMM_LOG_SELFTASK_PRIO, LOG_SELF_TASK_STACK_SIZE);
            if ( VOS_NULL_BYTE == ulRslt )
            {
                return VOS_ERR;
            }

            ulRslt = VOS_RegisterSelfTaskPrio(WUEPS_FID_OM,
                                         (VOS_TASK_ENTRY_TYPE)Om_BbpDbgSelfTask,
                                         COMM_BBPDBG_SELFTASK_PRIO, OM_BBP_DBG_TASK_STACK_SIZE);
            if ( VOS_NULL_BYTE == ulRslt )
            {
                return VOS_ERR;
            }
#endif

            break;
        }
        default:
            break;
    }
    return VOS_OK;
}

/*****************************************************************************
 Function   : OM_SetDrxTimerWakeSrc
 Description: ����DRX timer��Ϊ����Դ
 Input      : MODEM_ID_ENUM_UINT16 enModem
 Return     : VOS_VOID
 Other      :
*****************************************************************************/
VOS_VOID OM_SetDrxTimerWakeSrc(MODEM_ID_ENUM_UINT16 enModem)
{
    /*������Modem��������drx timerΪ�����ж�*/
    g_ulFlightModeVoteMap   &= (~ BIT(enModem));

    if ( 0 == g_ulFlightModeVoteMap )
    {
        DRV_SET_DRX_TIMER_WAKE_SRC();
    }

    return;
}

/*****************************************************************************
 Function   : OM_DelDrxTimerWakeSrc
 Description: ����DRX timer����Ϊ����Դ
 Input      : MODEM_ID_ENUM_UINT16 enModem
 Return     : VOS_VOID
 Other      :
*****************************************************************************/
VOS_VOID OM_DelDrxTimerWakeSrc(MODEM_ID_ENUM_UINT16 enModem)
{
    /*ͶƱ��Modem����������drx timerΪ�����ж�*/
    g_ulFlightModeVoteMap   |= BIT(enModem);

    if ( 0 != g_ulFlightModeVoteMap )
    {
        DRV_DEL_DRX_TIMER_WAKE_SRC();
    }

    return;
}

/*****************************************************************************
 �� �� ��  : OM_RegNvInit
 ��������  : ����NVģ�����Ƶ����������������ϲ�Ľӿ���ҪЭ��ջ������ע�ᡣ
 �������  : ��
 �������  : ��
 �� �� ֵ  : VOS_OK - �ɹ�
             VOS_ERR - ʧ��
 �޸���ʷ      :
  1.��    ��   : 2013��2��17��
    ��    ��   : ���� 47350
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 OM_RegNvInit(VOS_VOID)
{

#if (VOS_WIN32 == VOS_OS_VER)
    /* Init NV's code for reading NV */
    if (VOS_OK != NV_Init())
    {
        return VOS_ERR;
    }
#endif

    return VOS_OK;

}
VOS_VOID OM_PIDSendShow(VOS_BOOL bIsTrace)
{
    VOS_UINT32                          i=0;


    if(VOS_TRUE == bIsTrace)
    {
        for (i=0; i<OM_MAX_PS_MODULE_NUM; i++)
        {
            if(0 != g_aulPidTraceSendNum[i])
            {
                vos_printf("Pid Send Trace Show: pid is %d,send num is %d.\r\n",
                            i,
                            g_aulPidTraceSendNum[i]);
            }
        }
    }
    else
    {
        for (i=0; i<OM_MAX_PS_MODULE_NUM; i++)
        {
            if(0 != g_aulPidEventSendNum[i])
            {
                vos_printf("Pid Send Event Show: pid is %d,send num is %d.\r\n",
                            i,
                            g_aulPidEventSendNum[i]);
            }
        }
    }
    return;
}


VOS_VOID OM_OSAEvent(VOS_VOID *pData, VOS_UINT32 ulLength)
{
    PS_OM_EVENT_IND_STRU    *pstEventInd;
    VOS_UINT32               ulEventLenth;

    ulEventLenth = sizeof(PS_OM_EVENT_IND_STRU)
                    + ulLength
                    - 4 * sizeof(VOS_UINT8);/*�ṹ���а�����4byte������������Ҫ��ȥ*/

    pstEventInd = (PS_OM_EVENT_IND_STRU *)VOS_MemAlloc(WUEPS_PID_OM, DYNAMIC_MEM_PT, ulEventLenth);
    if(VOS_NULL_PTR == pstEventInd)
    {
        return;
    }

    pstEventInd->ulLength   = ulEventLenth;

    pstEventInd->usEventId  = (VOS_UINT16)OAM_EVENT_TIMER;

    pstEventInd->ulModuleId = WUEPS_PID_OM;

    /*lint -e419 -e416  */
    VOS_MemCpy((VOS_UINT8 *)pstEventInd->aucData, (VOS_UINT8 *)pData, ulLength);
    /*lint +e419 +e416*/

    if (VOS_OK != OM_Event(pstEventInd))
    {
        PS_LOG(WUEPS_PID_OM, VOS_NULL, PS_PRINT_WARNING, "OM_OSAEvent: send event fail.");
    }

    VOS_MemFree(WUEPS_PID_OM, pstEventInd);

    return;
}
VOS_VOID OM_SLEEPInfoEvent(VOS_VOID *pData, VOS_UINT32 ulLength)
{
    PS_OM_EVENT_IND_STRU    *pstEventInd;
    VOS_UINT32               ulEventLenth;

    ulEventLenth = sizeof(PS_OM_EVENT_IND_STRU)
                    + ulLength
                    - 4 * sizeof(VOS_UINT8);/*�ṹ���а�����4byte������������Ҫ��ȥ*/

    pstEventInd = (PS_OM_EVENT_IND_STRU *)VOS_MemAlloc(WUEPS_PID_OM, DYNAMIC_MEM_PT, ulEventLenth);
    if(VOS_NULL_PTR == pstEventInd)
    {
        return;
    }

    pstEventInd->ulLength   = ulEventLenth;

    pstEventInd->usEventId  = (VOS_UINT16)OAM_DEGUG_EVENT_ID100;

    pstEventInd->ulModuleId = WUEPS_PID_OM;

    /*lint -e419 -e416  */
    VOS_MemCpy((VOS_UINT8 *)pstEventInd->aucData, (VOS_UINT8 *)pData, ulLength);
    /*lint +e419 +e416*/

    if (VOS_OK != OM_Event(pstEventInd))
    {
        PS_LOG(WUEPS_PID_OM, VOS_NULL, PS_PRINT_WARNING, "OM_OSAEvent: send event fail.");
    }

    VOS_MemFree(WUEPS_PID_OM, pstEventInd);

    return;
}
VOS_UINT32 OM_GetLogPath(VOS_CHAR *pucBuf, VOS_CHAR *pucOldPath, VOS_CHAR *pucUnitaryPath)
{
    NV_PRODUCT_TYPE_FOR_LOG_DIRECTORY   stLogDir;

    VOS_MemSet(&stLogDir, 0, sizeof(stLogDir));

    if (VOS_OK != NV_Read(en_NV_Item_ProductTypeForLogDirectory, &stLogDir, sizeof(stLogDir)))
    {
        /* ��NVʧ��,ֱ�ӷ��� */
        return VOS_ERR;
    }

    /*0:MBB V3R3 stick/E5,etc.; 1:V3R3 M2M & V7R2; 2:V9R1 phone; 3:K3V3&V8R1;*/
    if ((1 == stLogDir.usProductType) || (3 == stLogDir.usProductType))
    {
        VOS_StrCpy(pucBuf, pucUnitaryPath);
    }
    else
    {
        VOS_StrCpy(pucBuf, pucOldPath);
    }

    return VOS_OK;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
