/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��      : OmOutside.c
  �� �� ��      : ����
  ��    ��      : ����47350
  ��������      : 2008��8��2��
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
#include "omprivate.h"
#include "DrvInterface.h"
#include "NVIM_Interface.h"
#include "ScInterface.h"
#include "DspInterface.h"
#include "omerrorlog.h"

#ifdef  __LDF_FUNCTION__
#include "apminterface.h"
#endif

#if (VOS_WIN32 == VOS_OS_VER)
#include "OamSpecTaskDef.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

/*lint -e767 �޸���:���� 47350;ԭ��:Log��ӡ */
#define    THIS_FILE_ID        PS_FILE_ID_OM_OUTSIDE_C
/*lint +e767 �޸���:���� 47350;*/

/*****************************************************************************
  2 �ⲿ���ñ�������
*****************************************************************************/
/* DSP CS-ERRLOG PrimId */
#define OM_RECORD_DSP_ERRORLOG         0x5100

#define OM_DSP_DCXO_UPDATE_NV_DATA     0x5101

/* DSP CS-ERRLOG MAX LENGTH */
#define OM_ERRORLOG_DSP_LEN_MAX        (1024)

typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT16 usPrimId;
    VOS_UINT16 usResv;
    VOS_UINT32 ulTotalLength;
    VOS_UINT32 ulErrNo;
    VOS_UINT8  aucData[4];
}DSP_ERROR_LOG_STRU;

typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT16 usPrimId;
    VOS_UINT16 usCount;
    VOS_UINT8  aucNVData[4];
}DSP_WRITE_NV_STRU;

typedef struct
{
    VOS_UINT16 usNvId;
    VOS_UINT16 usNvLen;
    VOS_UINT8  aucNVData[4];
}DSP_WRITE_NV_CONTEXT_STRU;

extern VOS_VOID OM_MmaMsgProc(MsgBlock *pstMsg);

#if(FEATURE_ON == FEATURE_PTM)
/* ���ڱ�������Err LOG ע��ص�����ָ�� */
OM_REGISTER_PROJECT_CTX_STRU            g_astFTMCallBackFuncCtx[]={
                                        {OM_ERR_LOG_MOUDLE_ID_IMS,  VOS_NULL_PTR},
                                        {OM_ERR_LOG_MOUDLE_ID_IMSA, VOS_NULL_PTR},
                                       };

/******************************************************************************
������  ��OM_RegisterGetData
����������OM Ccpu���ո�����ϱ�����ģʽ���ݵ�APP�ص�����ע��ӿ�
���������enModuleProjectCallback���ص��齨��ţ��̶�ΪOM_VOLTE_MOUDLE_PROJECT
          pFuncProjectCallbak    ���ص�����ָ��
���������NA
����ֵ  ��VOS_UINT32
�޸���ʷ��
1.  ����    ��2014��1��2��
����    ��
�޸����ݣ������ɺ���
˵��:
    1������ģʽ�����ϱ�����֪ͨ����������ṹ��APP_OM_CTRL_STATUS_STRU��Data��ȥOMHead��ulMsgModuleID����������
    2������ģʽ�����ϱ���Ϣ֪ͨ�������Data�����������OmHeard��OMPayload
*****************************************************************************/
VOS_UINT32 OM_RegisterGetData(OM_ERR_LOG_MOUDLE_ID_ENUM_UINT32 enProjectModule, pFuncOMGetData pFuncProjectCallback)
{
    VOS_UINT32                          ulIndex;
    
    if (VOS_NULL_PTR == enProjectModule)
    {        
        OM_CCPU_ERR_LOG1("OM_RegisterGetData:ModuleID is %d, func name is null.\r\n", enProjectModule);
        return OM_APP_MSG_MODULE_ID_ERR;
    }

    for (ulIndex=0; ulIndex<(sizeof(g_astFTMCallBackFuncCtx) / sizeof(g_astFTMCallBackFuncCtx[0])); ulIndex++)
    {
        if (g_astFTMCallBackFuncCtx[ulIndex].enProjectModule == enProjectModule)
        {
            break;
        }
    }

    if( (sizeof(g_astFTMCallBackFuncCtx) / sizeof(g_astFTMCallBackFuncCtx[0])) <= ulIndex )
    {
        OM_CCPU_ERR_LOG1("OM_RegisterGetData:ModuleID is error %d\r\n", enProjectModule);
        return OM_APP_MSG_MODULE_ID_ERR;
    }

    if (VOS_NULL_PTR == g_astFTMCallBackFuncCtx[ulIndex].pSendUlAtFunc)
    {
        g_astFTMCallBackFuncCtx[ulIndex].pSendUlAtFunc = pFuncProjectCallback;
        return VOS_OK;
    }
    
    OM_CCPU_ERR_LOG1("OM_RegisterGetData:callback Func is Registed,ModuleID is %d\r\n", enProjectModule);
    return VOS_ERR;
    
}

/******************************************************************************
������  ��OM_GetData
����������OM Ccpu���ո�����ϱ�����ģʽ����
���������enProjectModule    ���齨���
          pData              ����������
          ulLen              ���������ݳ���
���������NA
����ֵ  ��VOS_OK/VOS_ERR
�޸���ʷ��
1.  ����    ��2014��1��2��
����    ��
�޸����ݣ������ɺ���
˵����pData��OMHeader��MsgType��ʼ���൱�ڵ������Ԥ������Header���֣�����Header���ֵ�SN�ֶ���COMM��д
*****************************************************************************/
VOS_UINT32 OM_GetData(OM_ERR_LOG_MOUDLE_ID_ENUM_UINT32 enProjectModule, VOS_VOID *pData, VOS_UINT32 ulLen)
{
    OM_ALARM_MSG_HEAD_STRU             *pstOmHead;
    OM_FTM_REPROT_IND_STRU             *pstOmFtmReportInd;
    VOS_UINT32                          ulAddr;
    VOS_UINT32                          ulIndex;

    for (ulIndex=0; ulIndex<(sizeof(g_astFTMCallBackFuncCtx) / sizeof(g_astFTMCallBackFuncCtx[0])); ulIndex++)
    {
        /* �ϱ���Ϣʱ����������û��ע����ص��������Ͳ�����������Ϣ */
        if ( (g_astFTMCallBackFuncCtx[ulIndex].enProjectModule == enProjectModule)
             && (g_astFTMCallBackFuncCtx[ulIndex].pSendUlAtFunc  != VOS_NULL_PTR) )
        {
            break;
        }
    }

    if ((sizeof(g_astFTMCallBackFuncCtx) / sizeof(g_astFTMCallBackFuncCtx[0])) <= ulIndex)
    {
        OM_CCPU_ERR_LOG1("OM_GetData:ModuleID is error %d\r\n", enProjectModule);
        return OM_APP_MSG_MODULE_ID_ERR;
    }

    if ((VOS_NULL_PTR == pData) || ((sizeof(OM_ALARM_MSG_HEAD_STRU) + sizeof(OM_FTM_HEADER_STRU)) > ulLen))
    {
        OM_CCPU_ERR_LOG2("\r\n OM_GetData: Module ID is %d, Send data is NULL or len is error:%d\n", enProjectModule, ulLen);
        return OM_APP_MSG_LENGTH_ERR;
    }

    /*lint -e40*/
    OM_CCPU_DEBUG_TRACE((VOS_UINT8*)pData, ulLen, OM_CCPU_ERRLOG_RCV);
    /*lint +e40*/

    /* ����ģʽ�����Ϣ�ϱ���OM */
    pstOmFtmReportInd  = (OM_FTM_REPROT_IND_STRU*)VOS_AllocMsg(WUEPS_PID_OM_CALLBACK, (ulLen + sizeof(VOS_UINT32)));

    if (VOS_NULL_PTR == pstOmFtmReportInd)
    {
        OM_CCPU_ERR_LOG1("OM_GetData:Module ID is %d, malloc msg is fail\r\n", enProjectModule);
        return OM_APP_OMCCPU_ALLOC_MSG_ERR;
    }
    
    pstOmFtmReportInd->ulReceiverPid = ACPU_PID_OM;
    pstOmHead = (OM_ALARM_MSG_HEAD_STRU*)pData;

    /* ��ϢTYPE���ж�*/
    if (OM_ERR_LOG_MSG_FTM_REPORT == pstOmHead->ulMsgType)
    {
        pstOmFtmReportInd->ulMsgName     = ID_OM_FTM_REPROT_IND;
    }
    else if (OM_ERR_LOG_MSG_FTM_CNF == pstOmHead->ulMsgType)
    {
        pstOmFtmReportInd->ulMsgName     = ID_OM_FTM_REQUIRE_CNF;
    }
    else
    {
        OM_CCPU_ERR_LOG2("OM_GetData:Module ID is %d, Msg Type is Err %d\r\n", 
                     enProjectModule, pstOmHead->ulMsgType);

        VOS_FreeMsg(WUEPS_PID_OM_CALLBACK, pstOmFtmReportInd);

        return OM_APP_MSG_TYPE_ERR;
    }

    /* make pclint happy  begin */
    ulAddr = (VOS_UINT32)(&pstOmFtmReportInd->ulMsgType);
    VOS_MemCpy((VOS_VOID *)ulAddr, (VOS_VOID*)pData, ulLen);
    /* make pclint happy  end */

    (VOS_VOID)VOS_SendMsg(WUEPS_PID_OM_CALLBACK, pstOmFtmReportInd);

    return OM_APP_MSG_OK;
}

/******************************************************************************
������  ��OM_ErrLogMsgProc
����������OM Ccpu���ո�����ϱ�����ģʽ����
���������pMsg    ���յ�����ErrLog����ģʽ����
          
���������NA
����ֵ  ��VOS_VOID
�޸���ʷ��
1.  ����    ��2014��1��2��
����    ��
�޸����ݣ������ɺ���
*****************************************************************************/
VOS_VOID OM_ErrLogMsgProc(MsgBlock* pMsg)
{
    OM_FTM_REQUIRE_STRU                *pstOmFTMMsg;
    OM_FTM_CTRL_IND_STRU               *pstOmFtmCtrlInd; /* ����ģʽ������Ϣ:���ش򿪣��²������ϱ� */
    APP_OM_FTM_REQ_STRU                *pstAppOmFtmReq;  /* ����ģʽ������Ϣ:����һ�Σ��²��ϱ�һ�� */
    APP_OM_CTRL_STATUS_STRU             stAppOmCtrlStatus;
    VOS_UINT32                          ulIndex;
    VOS_UINT32                          ulResult;
  
    pstOmFTMMsg = (OM_FTM_REQUIRE_STRU*)pMsg;
    switch( pstOmFTMMsg->ulMsgName )
    {
        case ID_OM_FTM_CTRL_IND:
        {
            pstOmFtmCtrlInd = (OM_FTM_CTRL_IND_STRU*)pMsg;
        
            /* ����ģʽ�����ϱ���� */
            stAppOmCtrlStatus.stOmHeader.ulMsgType = OM_ERR_LOG_MSG_ON_OFF;
            stAppOmCtrlStatus.stOmHeader.ulMsgLen  = sizeof(VOS_UINT32) + sizeof(VOS_UINT16) + 2*sizeof(VOS_UINT8); /* �ýṹ�峤�� */
            stAppOmCtrlStatus.usModemID            = pstOmFTMMsg->usModemID;
            stAppOmCtrlStatus.ucAlmStatus          = pstOmFtmCtrlInd->ucActionFlag;

            /*lint -e40*/
            OM_CCPU_DEBUG_TRACE(((VOS_UINT8*)&stAppOmCtrlStatus), sizeof(stAppOmCtrlStatus), OM_CCPU_ERRLOG_RCV);
            /*lint +e40*/

            for (ulIndex=0; ulIndex<(sizeof(g_astFTMCallBackFuncCtx) / sizeof(g_astFTMCallBackFuncCtx[0])); ulIndex++)
            {
                if (VOS_NULL_PTR == g_astFTMCallBackFuncCtx[ulIndex].pSendUlAtFunc)
                {
                    continue;
                }
                
                stAppOmCtrlStatus.ulMsgModuleID = g_astFTMCallBackFuncCtx[ulIndex].enProjectModule;
                ulResult = g_astFTMCallBackFuncCtx[ulIndex].pSendUlAtFunc(stAppOmCtrlStatus.ulMsgModuleID, 
                                                                        (VOS_VOID *)&stAppOmCtrlStatus, 
                                                                        sizeof(stAppOmCtrlStatus));
                if (VOS_OK != ulResult)
                {
                    OM_CCPU_ERR_LOG2("OM_ErrLogMsgProc:Module ID is %d, return value fail: %d\r\n", 
                                 stAppOmCtrlStatus.ulMsgModuleID, ulResult);
                }
            }

            break;
        }

        case ID_OM_FTM_REQUIRE_IND:
        {
            pstAppOmFtmReq = (APP_OM_FTM_REQ_STRU*)pstOmFTMMsg->aucContent;

            /*lint -e40*/
            OM_CCPU_DEBUG_TRACE((VOS_UINT8*)pstAppOmFtmReq, 
                                (sizeof(APP_OM_FTM_REQ_STRU) - 4*sizeof(VOS_UINT8) + pstAppOmFtmReq->ulProjectLength), 
                                OM_CCPU_ERRLOG_RCV);
            /*lint +e40*/

            /* ����ģʽ�����ϱ���� */
            for (ulIndex=0; ulIndex<(sizeof(g_astFTMCallBackFuncCtx) / sizeof(g_astFTMCallBackFuncCtx[0])); ulIndex++)
            {
                if (g_astFTMCallBackFuncCtx[ulIndex].enProjectModule != pstAppOmFtmReq->ulMsgModuleID)
                {
                    continue;
                }

                if (VOS_NULL_PTR == g_astFTMCallBackFuncCtx[ulIndex].pSendUlAtFunc)
                {
                    continue;
                }

                ulResult = g_astFTMCallBackFuncCtx[ulIndex].pSendUlAtFunc(pstAppOmFtmReq->ulMsgModuleID, 
                                                                          (VOS_VOID *)pstAppOmFtmReq, 
                                                                          (sizeof(APP_OM_FTM_REQ_STRU) + pstAppOmFtmReq->ulProjectLength - sizeof(VOS_UINT32)));

                if (VOS_OK != ulResult)
                {
                    OM_CCPU_ERR_LOG2("OM_ErrLogMsgProc:Module ID is %d, return value fail: %d\r\n", 
                                 pstAppOmFtmReq->ulMsgModuleID, ulResult);
                }
            }
            break;
        }

        default:
            OM_CCPU_ERR_LOG1("OM_ErrLogMsgProc:msg name is err %d\r\n", pstOmFTMMsg->ulMsgName);
            break;
    }

    return ;
}

#endif
VOS_UINT32 WuepsOmCallBackPidInit(enum VOS_INIT_PHASE_DEFINE ip)
{
    return VOS_OK;
}


VOS_VOID OM_WriteDspNv(MsgBlock* pMsg)
{
    VOS_UINT32                  usIndex;
    VOS_UINT16                  usCount;
    VOS_UINT16                  usNvId;
    VOS_UINT16                  usNvLen;
    VOS_UINT32                  ulResult;
    DSP_WRITE_NV_STRU           *pstWriteNVMsg;
    DSP_WRITE_NV_CONTEXT_STRU   *pstWrteNvContext;
    VOS_UINT8                   *pucNvData;
    MODEM_ID_ENUM_UINT16        enModemID;

    enModemID = VOS_GetModemIDFromPid(pMsg->ulSenderPid); /*���DCXOȷ��˫���ٵ���*/

    pstWriteNVMsg = (DSP_WRITE_NV_STRU *)pMsg;

    usCount = pstWriteNVMsg->usCount;

    pstWrteNvContext = (DSP_WRITE_NV_CONTEXT_STRU *)(pstWriteNVMsg->aucNVData);

    for ( usIndex = 0; usIndex < usCount; usIndex++ )
    {
        usNvId = pstWrteNvContext->usNvId;

        usNvLen = pstWrteNvContext->usNvLen;

        pucNvData = pstWrteNvContext->aucNVData;

        ulResult = NV_WriteEx(enModemID, usNvId, pucNvData, (VOS_UINT32)usNvLen);

        if ( NV_OK != ulResult )
        {
            PS_LOG2(WUEPS_PID_OM, 0, PS_PRINT_ERROR,
                "OM_WriteDspNv:NV_Write, NV id: ErrCode :", (VOS_INT32)usNvId, (VOS_INT32)ulResult);

            return;
        }

        pstWrteNvContext = (DSP_WRITE_NV_CONTEXT_STRU *)(pstWrteNvContext->aucNVData + usNvLen);
    }

    return;
}
VOS_VOID OM_DspMsgProc(MsgBlock* pMsg)
{
    VOS_UINT16         usPrimId;
    DSP_ERROR_LOG_STRU *pstMsg;

    usPrimId = *(VOS_UINT16 *)(pMsg->aucValue);

    if (OM_RECORD_DSP_ERRORLOG == usPrimId)
    {
        pstMsg  = (DSP_ERROR_LOG_STRU *)pMsg;

        if(pstMsg->ulTotalLength > OM_ERRORLOG_DSP_LEN_MAX)
        {
            return;
        }

        MNTN_RecordErrorLog(pstMsg->ulErrNo, &pstMsg->ulTotalLength, pstMsg->ulTotalLength);
    }
    else if ( OM_DSP_DCXO_UPDATE_NV_DATA == usPrimId )
    {
        Print(" Start DCXO NV UPDATE!\n");

        OM_WriteDspNv(pMsg);
    }
    else
    {
        /* Make  Pclint happy */
        /* blank */
    }

    return;
}


VOS_VOID OM_HsicInitMsgProc(MsgBlock* pMsg)
{
    SC_PORT_STATUS_ENUM_UINT32          enStatus;

    /* ��ȡDK�ļ��ж˿ڹ������� */
    if (SC_ERROR_CODE_NO_ERROR != SC_FAC_GetPortAttrib(&enStatus))
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_HsicInitMsgProc:SC_FAC_GetPortAttrib error.");

        return;
    }

    /* DK�ļ����ò���OM��HSIC�˿ڵĹ��� */
    if (SC_PORT_STATUS_ON != enStatus)
    {
        return;
    }

    /* ���������󷢵�A�˵�OMAGENTȥ���� */
    if (VOS_OK != OM_HsicConnectReq())
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR, "OM_HsicInitMsgProc:OM_HsicConnectReq error.");

        return;
    }

    return;
}
VOS_VOID OM_CallBackMsgProc(MsgBlock* pMsg)
{
    OM_REQ_PACKET_STRU                 *pstAppMsg;
    OM_REQUEST_PROCEDURE               *pOmFuncProc;

    if (UEPS_PID_APM == pMsg->ulSenderPid)
    {
#ifdef  __LDF_FUNCTION__
        if ( VOS_FALSE == pMsg->aucValue[0])
        {
            SHPA_Ldf_Check();
            VOS_ProtectionReboot(DSP_REPORT_ERROR, 0, 0, VOS_NULL_PTR, 0);
        }
        else
        {
            SHPA_Ldf_Hifi_Saving();
            VOS_ProtectionReboot(HIFI_REPORT_ERROR, 0, 0, VOS_NULL_PTR, 0);
        }
#endif
        return;
    }

    if ((DSP_PID_WPHY == pMsg->ulSenderPid)
        || (I0_DSP_PID_GPHY == pMsg->ulSenderPid)
        || (I1_DSP_PID_GPHY == pMsg->ulSenderPid))
    {
        OM_DspMsgProc(pMsg);

        return;
    }

    if ((I0_WUEPS_PID_MMA == pMsg->ulSenderPid)||(I1_WUEPS_PID_MMA == pMsg->ulSenderPid))
    {
        OM_MmaMsgProc(pMsg);

        return;
    }

    if (WUEPS_PID_OM_CALLBACK == pMsg->ulSenderPid)
    {
        OM_HsicInitMsgProc(pMsg);

        return;
    }

#if(FEATURE_ON == FEATURE_PTM)
    if (ACPU_PID_OM == pMsg->ulSenderPid)
    {
        OM_ErrLogMsgProc(pMsg);
        return;
    }
#endif

    if (UEPS_PID_OMRL != pMsg->ulSenderPid)
    {
        VOS_UnreserveMsg(WUEPS_PID_OM, pMsg);
        PS_LOG1(WUEPS_PID_OM, 0, PS_PRINT_ERROR,
             "OM_OutMsgProc: Unknown Send Pid: %d.\n", (VOS_INT32)(pMsg->ulSenderPid));
        return;
    }

    pstAppMsg = (OM_REQ_PACKET_STRU*)pMsg->aucValue;

    /*�������*/
    /* ��2BitΪ����modem��ʾ */
    if ((OM_FUNCID_PART_NUM >= (pstAppMsg->ucModuleId&OM_FUNCID_VALUE_BITS))
        || (OM_FUNCID_MAX_NUM < (pstAppMsg->ucModuleId&OM_FUNCID_VALUE_BITS)))
    {
        VOS_UnreserveMsg(WUEPS_PID_OM, pMsg);
        PS_LOG1(WUEPS_PID_OM, 0, PS_PRINT_WARNING,
        "OM_CallBackMsgProc:ucModuleId is overflow :", (VOS_INT32)pstAppMsg->ucModuleId);
        return;
    }

    pOmFuncProc = g_astOmFuncIdProcTbl[pstAppMsg->ucModuleId - 1].pOmFuncProc;
    pOmFuncProc(pstAppMsg, OM_SendData);

    VOS_UnreserveMsg(WUEPS_PID_OM, pMsg);
    return;
}

VOS_UINT32 OMCallBackFidInit(enum VOS_INIT_PHASE_DEFINE ip)
{
    VOS_UINT32 ulRslt;

    /* Add for L Only version */
    if (BOARD_TYPE_LTE_ONLY == BSP_OM_GetBoardType())
    {
        return 0;
    }
    /* Add for L Only version */

    switch( ip )
    {
        case VOS_IP_LOAD_CONFIG:
        {
            ulRslt = VOS_RegisterPIDInfo(WUEPS_PID_OM_CALLBACK,
                        VOS_NULL_PTR,
                        (Msg_Fun_Type)OM_CallBackMsgProc);

            if( VOS_OK != ulRslt )
            {
                return VOS_ERR;
            }

            if(VOS_OK != VOS_RegisterMsgTaskPrio(WUEPS_FID_OM_CALLBACK, VOS_PRIORITY_NULL))
            {
                PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_ERROR,
                    "OMCallBackFidInit:VOS_RegisterMsgTaskPrio failure !\n");

                return VOS_ERR;
            }

#if (VOS_WIN32 == VOS_OS_VER)
#ifndef __PS_WIN32_RECUR__

            /* ����SOCKET���ݵ��Դ������� */
            ulRslt = VOS_RegisterSelfTaskPrio(WUEPS_FID_OM_CALLBACK,
                             (VOS_TASK_ENTRY_TYPE)Sock_ServerTask,
                             COMM_SOCK_SELFTASK_PRIO, RL_SOCK_TASK_STACK_SIZE);
            if ( VOS_NULL_BYTE == ulRslt )
            {
                return VOS_ERR;
            }

#endif
#endif
            break;
        }
        default:
            break;
    }
    return VOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
