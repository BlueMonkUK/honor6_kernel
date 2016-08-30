/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��      : OmAgent.c
  �� �� ��      : ����
  ��    ��      : ����47350
  ��������      : 2011��7��4��
  ����޸�      :
  ��������      : ��C�ļ�������OM AGENTģ���ʵ��
  �����б�      :
  �޸���ʷ      :
  1.��    ��    : 2011��7��1��
    ��    ��    : ����47350
    �޸�����    : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "Omappagent.h"
#include "NVIM_Interface.h"
#include "pslog.h"
#include "om.h"
#include "FileSysInterface.h"
#include "omfilesystem.h"
#include "omnvinterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

/*lint -e767 �޸��ˣ�����47350�������ˣ�����46160��ԭ�������LOG���������Ҫ*/
#define    THIS_FILE_ID        PS_FILE_ID_OM_AGENT_C
/*lint +e767 �޸��ˣ�����47350�������ˣ�lixiao��*/

typedef VOS_VOID (*OM_AGENT_PROC)(MsgBlock *pMsg);

typedef struct
{
    VOS_UINT32      ulPrimId;           /*Indicates current msg type.*/
    OM_AGENT_PROC   pfFun;              /*Reserves the pointer of function handles current msg.*/
}OM_AGENT_ITME_STRU;

#if (RAT_GU == RAT_MODE)
extern VOS_UINT32 NV_RestoreResult(VOS_VOID);
extern void  MNTN_RecordVersionInfo(void);
#endif


VOS_VOID OM_SendConfigMsg(VOS_UINT32 ulRcvPid, MsgBlock* pstMsg)
{
    MsgBlock    *pstDestMsg;

    pstDestMsg = VOS_AllocMsg(WUEPS_PID_OM, pstMsg->ulLength);

    if (VOS_NULL_PTR == pstDestMsg)
    {
        return;
    }

    VOS_MemCpy(pstDestMsg, pstMsg, VOS_MSG_HEAD_LENGTH + pstMsg->ulLength);

    pstDestMsg->ulSenderPid   = WUEPS_PID_OM;
    pstDestMsg->ulReceiverPid = ulRcvPid;

    (VOS_VOID)VOS_SendMsg(WUEPS_PID_OM, pstDestMsg);

    return;
}


VOS_UINT32 OM_OledClear(VOS_VOID)
{
    OM_OLED_CLEAR_REQ_STRU    *pstOledCLearMsg;

    /* ������Ϣ�ڴ� */
    pstOledCLearMsg = (OM_OLED_CLEAR_REQ_STRU*)VOS_AllocMsg(CCPU_PID_OMAGENT,
                    sizeof(OM_OLED_CLEAR_REQ_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstOledCLearMsg)
    {
        return VOS_ERR;
    }

    pstOledCLearMsg->ulReceiverPid = ACPU_PID_OMAGENT;
    pstOledCLearMsg->usPrimId      = OM_OLED_CLEAR_REQ;

    /* ����Ϣ���͸�ACPU */
    return VOS_SendMsg(CCPU_PID_OMAGENT, pstOledCLearMsg);
}


VOS_UINT32 OM_OledDisplay(VOS_UINT16 usXPos, VOS_UINT16 usYPos, VOS_CHAR *pcString)
{
    VOS_UINT32                  ulLen;
    OM_OLED_DISPLAY_REQ_STRU    *pstOledMsg;

    /* ������� */
    if (VOS_NULL_PTR == pcString)
    {
        return VOS_ERR;
    }

    /* ��ȡ�ַ����ĳ��� */
    ulLen = VOS_StrLen(pcString);

    /* ��Ϣ��������Ҫ����\0 */
    ulLen++;

    /* ������Ϣ�ڴ� */
    pstOledMsg = (OM_OLED_DISPLAY_REQ_STRU*)VOS_AllocMsg(CCPU_PID_OMAGENT,
                    (sizeof(OM_OLED_DISPLAY_REQ_STRU) - VOS_MSG_HEAD_LENGTH) + ulLen);

    if (VOS_NULL_PTR == pstOledMsg)
    {
        return VOS_ERR;
    }

    pstOledMsg->ulReceiverPid = ACPU_PID_OMAGENT;
    pstOledMsg->usPrimId      = OM_OLED_DISPLAY_REQ;
    pstOledMsg->usXPos        = usXPos;
    pstOledMsg->usYPos        = usYPos;

    VOS_MemCpy((VOS_UINT8*)pstOledMsg + sizeof(OM_OLED_DISPLAY_REQ_STRU), pcString, ulLen);

    /* ����Ϣ���͸�ACPU */
    return VOS_SendMsg(CCPU_PID_OMAGENT, pstOledMsg);
}
VOS_UINT32 OM_HsicConnectReq(VOS_VOID)
{
    OM_HSIC_CONNECT_REQ_STRU           *pstOmHsicMsg;

    /* ������Ϣ�ڴ� */
    pstOmHsicMsg = (OM_HSIC_CONNECT_REQ_STRU*)VOS_AllocMsg(CCPU_PID_OMAGENT,
                                                           sizeof(OM_HSIC_CONNECT_REQ_STRU) - VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstOmHsicMsg)
    {
        return VOS_ERR;
    }

    pstOmHsicMsg->ulReceiverPid = ACPU_PID_OMAGENT;
    pstOmHsicMsg->usPrimId      = OM_HSIC_CONNECT_REQ;

    /* ����Ϣ���͸�ACPU */
    return VOS_SendMsg(CCPU_PID_OMAGENT, pstOmHsicMsg);
}


VOS_VOID OM_ErrorLogProc(MsgBlock* pMsg)
{
    ERRORLOG_REQ_STRU       *pstErrorLogReq;
    ERRORLOG_CNF_STRU       *pstErrorLogCnf;
    VOS_UINT32              ulResult;

    pstErrorLogCnf = (ERRORLOG_CNF_STRU*)VOS_AllocMsg(CCPU_PID_OMAGENT,
                            sizeof(ERRORLOG_CNF_STRU) - VOS_MSG_HEAD_LENGTH);

    /* ������Ϣʧ�� */
    if (VOS_NULL_PTR == pstErrorLogCnf)
    {
        return;
    }

    pstErrorLogReq = (ERRORLOG_REQ_STRU*)pMsg;

    /* ִ��ACPU������ErrorLog���� */
    ulResult = MNTN_ErrorLog((VOS_CHAR*)pstErrorLogReq->aucFileName, pstErrorLogReq->ulFileId,
                            pstErrorLogReq->ulLine, pstErrorLogReq->ulErrNo,
                            pstErrorLogReq->aucData,  pstErrorLogReq->ulLen);

    pstErrorLogCnf->ulReceiverPid  = ACPU_PID_OMAGENT;
    pstErrorLogCnf->usPrimId       = ERRORLOG_CNF;
    pstErrorLogCnf->usResult       = (VOS_UINT16)ulResult;

    (VOS_VOID)VOS_SendMsg(CCPU_PID_OMAGENT, pstErrorLogCnf);

    return;
}

/*****************************************************************************
 Prototype       : OM_WriteLogProc
 Description     : ����ACPU������write Log����
 Input           : pMsg - ָ����Ϣ��ַ
 Output          : None
 Return Value    : VOS_VOID

 History         : ---
    Date         : 2012-05-08
    Author       : zhuli
    Modification : Created function
 *****************************************************************************/
VOS_VOID OM_WriteLogProc(MsgBlock* pMsg)
{
    OM_WRITELOG_REQ_STRU    *pstLogReq;
    VOS_UINT32              ulLen;
    FILE                    *fp;

    pstLogReq = (OM_WRITELOG_REQ_STRU*)pMsg;

    fp = DRV_FILE_OPEN((VOS_CHAR*)pstLogReq->aucFileName, "ab+");  /*����׷�ӷ�ʽ���ļ�*/

    if(VOS_NULL_PTR == fp)                      /*����ļ���ʧ��˵�������ļ�Ҳ���ɹ�*/
    {
        return;
    }

    DRV_FILE_LSEEK(fp, 0, DRV_SEEK_END);

    ulLen = (VOS_UINT32)DRV_FILE_TELL(fp);

    if(ulLen >= OM_LOG_FILE_MAX_SIZE)            /*�ļ���С��������*/
    {
        DRV_FILE_CLOSE(fp);

        fp = DRV_FILE_OPEN((VOS_CHAR*)pstLogReq->aucFileName, "wb+");  /*����ļ�����*/
    }

    if(VOS_NULL_PTR == fp)                      /*����ļ���ʧ��˵�������ļ�Ҳ���ɹ�*/
    {
        return;
    }

    DRV_FILE_WRITE(pstLogReq->aucData, sizeof(VOS_CHAR), pstLogReq->ulLen, fp);

    DRV_FILE_CLOSE(fp);

    return;
}
VOS_VOID OM_SdConfigProc(MsgBlock* pMsg)
{
    OM_AUTOCONFIG_CNF_STRU       *pstSdConfigCnf;

    pstSdConfigCnf = (OM_AUTOCONFIG_CNF_STRU*)VOS_AllocMsg(CCPU_PID_OMAGENT,
                            sizeof(OM_AUTOCONFIG_CNF_STRU) - VOS_MSG_HEAD_LENGTH);

    /* ������Ϣʧ�� */
    if (VOS_NULL_PTR == pstSdConfigCnf)
    {
        return;
    }

    pstSdConfigCnf->ulReceiverPid  = ACPU_PID_OMAGENT;
    pstSdConfigCnf->usPrimId       = OM_AUTOCONFIG_CNF;

    (VOS_VOID)VOS_SendMsg(CCPU_PID_OMAGENT, pstSdConfigCnf);

#if (RAT_GU == RAT_MODE)
    /* PID��ʼ����󣬲鿴�Ƿ�������������errorlog��Ҫд�汾��Ϣ */
    if (VOS_OK == NV_RestoreResult())
    {
        MNTN_RecordVersionInfo();
    }
#endif

    return;
}


VOS_VOID OM_LogShowToFileProc(MsgBlock* pMsg)
{
    /* ��debug ��Ϣ��¼���ļ�ϵͳ�� */
    OM_LogShowToFile(VOS_FALSE);

    return;
}

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/* ����ACPU�������������Ķ�Ӧ�� */
OM_AGENT_ITME_STRU g_astOmAgentProcTbl[] =
{
    {ERRORLOG_REQ,          OM_ErrorLogProc},
    {OM_AUTOCONFIG_REQ,     OM_SdConfigProc},
    {OM_WRITE_LOG_REQ,      OM_WriteLogProc},
    {OM_RECORD_DBU_INFO_REQ,OM_LogShowToFileProc},
};
VOS_VOID OM_AgentMsgProc(MsgBlock *pMsg)
{
    VOS_UINT16  usPrimId;
    VOS_UINT32  ulTotalNum;
    VOS_UINT32  ulIndex;

    usPrimId = *(VOS_UINT16*)(pMsg->aucValue);

    ulTotalNum = sizeof(g_astOmAgentProcTbl)/sizeof(OM_AGENT_ITME_STRU);

    /* ����PrimId���Ҷ�Ӧ�Ĵ����� */
    for (ulIndex = 0; ulIndex < ulTotalNum; ulIndex++)
    {
        if (usPrimId == g_astOmAgentProcTbl[ulIndex].ulPrimId)
        {
            g_astOmAgentProcTbl[ulIndex].pfFun(pMsg);
            return;
        }
    }

    return;
}
VOS_UINT32 Om_AgentPidInit(enum VOS_INIT_PHASE_DEFINE ip)
{
    return VOS_OK;
}


VOS_UINT32 OM_AgentFidInit(enum VOS_INIT_PHASE_DEFINE ip)
{
    VOS_UINT32 ulRslt;

    if( VOS_IP_LOAD_CONFIG == ip )
    {
        ulRslt = VOS_RegisterPIDInfo(CCPU_PID_OMAGENT,
                                    (Init_Fun_Type)Om_AgentPidInit,
                                    (Msg_Fun_Type)OM_AgentMsgProc);
        if( VOS_OK != ulRslt )
        {
            return VOS_ERR;
        }

        ulRslt = VOS_RegisterMsgTaskPrio(AGENT_FID_OM, VOS_PRIORITY_P6);

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
