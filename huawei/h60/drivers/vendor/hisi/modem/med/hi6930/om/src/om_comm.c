/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : om_comm.c
  �� �� ��   : ����
  ��    ��   : ��ׯ�� 59026
  ��������   : 2011��6��16��
  ����޸�   :
  ��������   :
  �����б�   :
              OM_COMM_Init
              OM_COMM_InitFuncTable
              OM_COMM_IsrNmi
              OM_COMM_IsrSysException
              OM_COMM_MsgExtDefReq
              OM_COMM_PidInit
              OM_COMM_PidProc
              OM_COMM_SendTrace
              OM_COMM_SendTrans
  �޸���ʷ   :
  1.��    ��   : 2011��6��16��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "om.h"
#include "fsm.h"
#include "med_drv_timer_hifi.h"
#include "med_drv_mb_hifi.h"
#include "ucom_nv.h"
#include "codec_typedefine.h"


#include "hifidrvinterface.h"
#include "ucom_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    ��ά�ɲ���Ϣ�а�����C�ļ���ź궨��
*****************************************************************************/
/*lint -e(767)*/
#define THIS_FILE_ID                    OM_FILE_ID_OM_COMM_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/* OM��Ϣ״̬��(����,������״̬�л�) */
STA_STRU g_stOmStatusDesc;

/* OM��Ϣ�����,�˴�û������ */
ACT_STRU g_astOmMsgFuncTable[] =
{
    /* ������ʽ: PID(reserve), ��Ϣ����, ��Ӧ������ */
    FSM_ACT_TBL_ITEM(0x0000, ID_OM_CODEC_MSG_HOOK_REQ,    OM_MSGHOOK_MsgCfgMsgHookReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_OM_CODEC_CPU_VIEW_REQ,    OM_CPUVIEW_MsgCfgReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_OM_CODEC_SET_LOG_REQ,     OM_LOG_MsgSetLogReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_OM_CODEC_EXT_DEF_REQ,     OM_COMM_MsgExtDefReq),
    FSM_ACT_TBL_ITEM(0x0000, ID_TIMER_CODEC_CPUVIEW_RPT_IND,  OM_CPUVIEW_MsgRptInd),
    FSM_ACT_TBL_ITEM(0x0000, ID_OM_CODEC_OM_CONNECT_CMD,    OM_COMM_MsgOmConnectCmd),
    FSM_ACT_TBL_ITEM(0x0000, ID_OM_CODEC_OM_DISCONNECT_CMD, OM_COMM_MsgOmDisConnectCmd)
};

/* HIFI�ϱ���Ϣ���кż�¼ȫ�ֱ�������0��ʼ��¼ */
VOS_UINT32 g_uwOmMsgSeqNum = 0;

/* ��¼OM����״̬ */
UCOM_SET_UNINIT
VOS_UINT16   g_OMConnectionStatus = CODEC_SWITCH_OFF;

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

/*****************************************************************************
 �� �� ��  : OM_COMM_PidInit
 ��������  : DSP_PID_OM��PID��ʼ������
 �������  : enum VOS_INIT_PHASE_DEFINE enInitPhrase - ע��PID�Ĺ��̱��
 �������  : ��
 �� �� ֵ  : VOS_UINT32 - VOS_OK / VOS_ERR
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��16��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 OM_COMM_PidInit(enum VOS_INIT_PHASE_DEFINE enInitPhrase)
{
    VOS_UINT32                          uwCheck = VOS_OK;
    CODEC_NV_PARA_HIFI_TRACE_CFG_STRU     stHifiTraceCfg;

    switch (enInitPhrase)
    {
        case VOS_IP_LOAD_CONFIG:
        {
            /* OM״̬����ʼ�� */
            OM_COMM_InitFuncTable();
            break;
        }
        case VOS_IP_FARMALLOC:
        {
            /* ����PID��ʼ��������OMģ�� */

            /* ��ȡHIFI TRACE���NV�� */
            UCOM_MemSet(&stHifiTraceCfg, 0, sizeof(stHifiTraceCfg));

            uwCheck  = UCOM_NV_Read(en_NV_HifiTraceCfg,
                                    &stHifiTraceCfg,
                                     sizeof(stHifiTraceCfg));

            uwCheck += OM_LOG_DefaultCfg(&stHifiTraceCfg.stLogHookCfg);

            uwCheck += OM_MSGHOOK_DefaultCfg(&stHifiTraceCfg.stMsgHookCfg);

            break;
        }
        case VOS_IP_INITIAL:
        case VOS_IP_ENROLLMENT:
        case VOS_IP_LOAD_DATA:
        case VOS_IP_FETCH_DATA:
        case VOS_IP_STARTUP:
        case VOS_IP_RIVAL:
        case VOS_IP_KICKOFF:
        case VOS_IP_STANDBY:
        case VOS_IP_BROADCAST_STATE:
        case VOS_IP_RESTART:
        {
            break;
        }
        default:
        {
            break;
        }
    }

    if (VOS_OK != uwCheck)
    {
        return VOS_ERR;
    }
    else
    {
        return VOS_OK;
    }
}

/*****************************************************************************
 �� �� ��  : OM_COMM_PidProc
 ��������  : DSP_PID_OM����Ϣ����������������Ϊg_astOmMsgFuncTable
 �������  : MsgBlock *pstOsaMsg - �ַ���DSP_PID_OM����Ϣ
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��16��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_COMM_PidProc(MsgBlock *pstOsaMsg)
{
    VOS_UINT16     *puhwMsgId           = VOS_NULL;
    ACT_STRU       *pstRetAct           = VOS_NULL;
    VOS_UINT16      uhwOffset           = 0;
    STA_STRU       *pstMsgStatus        = OM_COMM_GetMsgStatusPtr();
    ACT_STRU        stKeyAct;

    /* ��ȡ��ϢID */
    puhwMsgId = (VOS_UINT16*)pstOsaMsg->aucValue;

    if ( (ID_OM_CODEC_OM_DISCONNECT_CMD  != *puhwMsgId)
       &&(ID_TIMER_CODEC_CPUVIEW_RPT_IND != *puhwMsgId))
    {
        uhwOffset   = 24;
        puhwMsgId = (VOS_UINT16*)((VOS_UINT32)pstOsaMsg->aucValue + uhwOffset);
    }

    stKeyAct.ulEventType = *puhwMsgId;

    /* ���ö��ֲ��Һ���bsearch���¼�������в�����Ӧ���¼������� */
    /* ����Ҳ�����Ӧ���¼�������,����NULL */
    pstRetAct = (ACT_STRU*)FSM_Bsearch((VOS_INT16 *)(&stKeyAct),
                                       (VOS_UINT8 *)(pstMsgStatus->pActTable),
                                       pstMsgStatus->ulSize,
                                       sizeof(ACT_STRU),
                                       FSM_ActCompare);

    /* ������Ϣ��������֮��Ӧ,����֮ */
    if (VOS_NULL != pstRetAct)
    {
        (VOS_VOID)(*pstRetAct->pfActionFun)((VOS_UCHAR *)pstOsaMsg + uhwOffset);
    }
    else
    {
        /* ��¼�쳣,DSP_PID_OM�յ�δ֪��Ϣ,δ���� */
        OM_LogWarning1(OM_COMM_PidProc_UnknownMsg, stKeyAct.ulEventType);
    }
}

/*****************************************************************************
 �� �� ��  : OM_COMM_InitFuncTable
 ��������  : OM��Ϣ״̬����ʼ������
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��16��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_COMM_InitFuncTable(VOS_VOID)
{
    STA_STRU       *pstStatusDec        = OM_COMM_GetMsgStatusPtr();

    /* ��ʼ��״̬������������������*/
    pstStatusDec->pActTable  = OM_COMM_GetMsgFuncTbl();
    pstStatusDec->ulSize     = sizeof(OM_COMM_GetMsgFuncTbl())/sizeof(ACT_STRU);

    /* ����������ϢID�������򣬴ӵ͵���˳������ */
    FSM_Sort((VOS_UINT8 *)pstStatusDec->pActTable,
             pstStatusDec->ulSize,
             sizeof(ACT_STRU),
             FSM_ActCompare);
}

/*****************************************************************************
 �� �� ��  : OM_COMM_Init
 ��������  : ��ά�ɲ�ģ���ʼ������
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��21��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_COMM_Init(VOS_VOID)
{
    OM_COMM_SetMsgSeqNum(0);
    OM_LOG_Init();
    OM_MSGHOOK_Init();
    OM_CPUVIEW_Init();
}

/*****************************************************************************
 �� �� ��  : OM_COMM_SendTrans
 ��������  : ��ָ�����ݿ���͸����Ϣ��ʽͨ��OMͨ������
 �������  : OM_DATA_BLK_STRU *pstDataBlk - ���������ݿ�
             VOS_UINT32 uwBlkCnt          - ���������ݿ���Ŀ
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��18��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_COMM_SendTrans(UCOM_DATA_BLK_STRU *pstDataBlk, VOS_UINT32 uwBlkCnt)
{
    VOS_UINT32                          uwLength;           /* ���������ݳ��� */
    VOS_UINT32                          uwCnt;              /* ѭ������ */

    /* �����Ϸ��Լ�� */
    if (   (VOS_NULL == pstDataBlk)
        || (       0 == uwBlkCnt)
        || (uwBlkCnt >  OM_COMM_TRANS_BLK_MAX_NUM))
    {
        return;
    }

    /* ͳ����Ϣ�ܳ��� */
    uwLength = 0;
    for (uwCnt = 0; uwCnt < uwBlkCnt; uwCnt++)
    {
        uwLength += pstDataBlk[uwCnt].uwSize;
    }

    /* ͨ�����VOS��Ϣ���䷢��͸����Ϣ */
    #ifdef _OM_MAILBOX_REPORT
    {
        OM_TRANS_PASS_HEADER_STRU      *pstTrans = VOS_NULL;
        MsgBlock                       *pstMsg   = VOS_NULL;
        VOS_UCHAR                      *pucDes   = VOS_NULL;

        pstMsg = VOS_AllocMsg(DSP_PID_HIFI_OM, uwLength + sizeof(OM_TRANS_PASS_HEADER_STRU));
        if (VOS_NULL == pstMsg)
        {
            return;
        }

        /* ���͸����Ϣ */
        pstTrans                    = (OM_TRANS_PASS_HEADER_STRU *)pstMsg->aucValue;
        pstTrans->usTransPrimId     = OM_TRANS_PRIM_ID;
        pstTrans->usFuncType        = OM_TRANS_FUNC_TYPE;
        pstTrans->usLength          = (VOS_UINT16)uwLength + OM_APP_HEADER_TAIL_LEN;
        pstTrans->ulSn              = OM_COMM_GetAndIncMsgSeqNum();    /* ������ֱ�ά�����ϱ�����Ϣ���к� */
        pstTrans->ulTimeStamp       = DRV_TIMER_ReadSysTimeStamp();

        pstMsg->uwReceiverPid       = UCOM_PID_PS_OM;
        pstMsg->uwLength            = uwLength + sizeof(OM_TRANS_PASS_HEADER_STRU);

        /* ������Ϣ���� */ /*lint --e(416)*/
        pucDes = pstMsg->aucValue + sizeof(OM_TRANS_PASS_HEADER_STRU);
        for (uwCnt = 0; uwCnt < uwBlkCnt; uwCnt++)
        {
            UCOM_MemCpy(pucDes, pstDataBlk[uwCnt].pucData, pstDataBlk[uwCnt].uwSize);
            pucDes += pstDataBlk[uwCnt].uwSize;
        }

        /* ������Ϣ, ��ʧ��������������֤������ */
        (VOS_VOID)VOS_SendMsg(DSP_PID_HIFI_OM, pstMsg);
    }

    /* ͨ��SOCP����͸����Ϣ */
    #else
    if (CODEC_SWITCH_ON == OM_COMM_GetOMConnStatus())
    {
        UCOM_DATA_BLK_STRU          astDataBlk[OM_COMM_TRANS_BLK_MAX_NUM + 1];
        OM_TRANS_HEADER_STRU        stTransHeader;

        stTransHeader.usFuncType    = OM_TRANS_FUNC_TYPE;
        stTransHeader.usLength      = (VOS_UINT16)uwLength + OM_APP_HEADER_TAIL_LEN;
        stTransHeader.ulSn          = OM_COMM_GetAndIncMsgSeqNum(); /* ������ֱ�ά�����ϱ�����Ϣ���к� */
        stTransHeader.ulTimeStamp   = DRV_TIMER_ReadSysTimeStamp();

        astDataBlk[0].pucData       = (VOS_UCHAR*)&stTransHeader;
        astDataBlk[0].uwSize        = sizeof(OM_TRANS_HEADER_STRU);
        UCOM_MemCpy(&astDataBlk[1], pstDataBlk, uwBlkCnt * sizeof(UCOM_DATA_BLK_STRU));

        /* SOCPд��ʧ�����¼�ڲ����� */
        if (VOS_OK != DRV_SOCP_Write(astDataBlk, uwBlkCnt + 1))
        {
            OM_LOG_InnerRecord(OM_COMM_SendTrans_SocpWriteFailed, (VOS_UINT16)THIS_FILE_ID, (VOS_UINT16)__LINE__);
        }
    }
    #endif

}


/*****************************************************************************
 �� �� ��  : OM_COMM_SendTrace
 ��������  : ��ָ��������TRACE��Ϣ��ʽͨ��OMͨ������
 �������  : VOS_UCHAR *pucTrace - ����������
             VOS_UINT32 uwLength - ���������ݳ���
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��18��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_COMM_SendTrace(VOS_UCHAR *pucTrace, VOS_UINT32 uwLength)
{
    UCOM_DATA_BLK_STRU          astDataBlk[OM_COMM_TRACE_BLK_MAX_NUM];
    OM_TRACE_HEADER_STRU        stTrace;

    /* �����Ϸ��Լ�� */
    if ((VOS_NULL == pucTrace) || (0 == uwLength) || (CODEC_SWITCH_OFF == OM_COMM_GetOMConnStatus()))
    {
        return;
    }

    /* ���TRACEͷ */
    stTrace.usFuncType          = OM_TRACE_FUNC_TYPE;
    stTrace.usLength            = (VOS_UINT16)uwLength + OM_APP_HEADER_TAIL_LEN + OM_TRACE_TAIL_LEN ;
    stTrace.ulSn                = OM_COMM_GetAndIncMsgSeqNum(); 	/* ������ֱ�ά�����ϱ�����Ϣ���к� */
    stTrace.ulTimeStamp         = DRV_TIMER_ReadSysTimeStamp();
    stTrace.usPrimId            = OM_TRACE_PRIM_ID;
    stTrace.usToolId            = 0;

    /* ��֯TRACE���ݿ� */
    astDataBlk[0].pucData       = (VOS_UCHAR*)&stTrace;
    astDataBlk[0].uwSize        = sizeof(OM_TRACE_HEADER_STRU);
    astDataBlk[1].pucData       = pucTrace;
    astDataBlk[1].uwSize        = uwLength;

    /* SOCPд��ʧ�����¼�ڲ����� */
    if (VOS_OK != DRV_SOCP_Write(astDataBlk, OM_COMM_TRACE_BLK_MAX_NUM))
    {
        OM_LOG_InnerRecord(OM_COMM_SendTrace_SocpWriteFailed, (VOS_UINT16)THIS_FILE_ID, (VOS_UINT16)__LINE__);
    }

}

/*****************************************************************************
 �� �� ��  : OM_COMM_MsgExtDefReq
 ��������  : ��չ����ӿڣ�HIFI��OM��PC��SDT����ͨ���˽ӿڿ���չ�������
 �������  : VOS_VOID *pvOsaMsg - OM_MED_EXT_DEF_REQ_STRU��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_UINT32 - UCOM_RET_SUCC
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��18��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 OM_COMM_MsgExtDefReq(VOS_VOID *pvOsaMsg)
{
    CODEC_OM_EXT_DEF_CNF_STRU     stCnfMsg;
    OM_CODEC_EXT_DEF_REQ_STRU    *pstReqMsg;

    UCOM_MemSet(&stCnfMsg, 0, sizeof(stCnfMsg));

    pstReqMsg = (OM_CODEC_EXT_DEF_REQ_STRU*)pvOsaMsg;

    switch (pstReqMsg->enExtCmd)
    {
        /* ��δʵ�� */
        case OM_CODEC_EXT_CMD_DEFAULT:
        {
            stCnfMsg.enExtCmd   = pstReqMsg->enExtCmd;
            stCnfMsg.uhwExtLen  = 0;
            break;
        }
        case OM_CODEC_EXT_ENABLE_OM:
        {
            DRV_SOCP_Init();
            OM_COMM_SetOMConnStatus(CODEC_SWITCH_ON);
            break;
        }
        case OM_CODEC_EXT_AUDIO_ENHANCE_START_VOIP:
        case OM_CODEC_EXT_AUDIO_ENHANCE_STOP_VOIP:
        case OM_CODEC_EXT_AUDIO_ENHANCE_SET_DEVICE:
        case OM_CODEC_EXT_AUDIO_ENHANCE_START_DEFAULT:
        case OM_CODEC_EXT_AUDIO_ENHANCE_STOP_DEFAULT:
        default:
        {
            stCnfMsg.enExtCmd   = pstReqMsg->enExtCmd;
            stCnfMsg.uhwExtLen  = 1;
            stCnfMsg.auhwExtData[0] = UCOM_RET_ERR_MSG;
            break;
        }
    }

    stCnfMsg.uhwMsgId = ID_CODEC_OM_EXT_DEF_CNF;

    /* �ظ���Ϣ */
    OM_COMM_SendTranMsg(&stCnfMsg, sizeof(stCnfMsg));

    OM_LogInfo(OM_COMM_MsgExtDefReq_OK);

    return UCOM_RET_SUCC;

}

/*****************************************************************************
 �� �� ��  : OM_COMM_IsrSysException
 ��������  : �û����쳣������,ע������û�ģʽ�³����쳣ʱ����
 �������  : VOS_UINT32 uwExceptionNo - �쳣��Ϣ
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��18��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_COMM_IsrSysException (VOS_UINT32 uwExceptionNo)
{
    /* reserve */
}

/*****************************************************************************
 �� �� ��  : OM_COMM_IsrNmi
 ��������  : NMI�жϴ�������ע�����NMI�ж�
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��6��18��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID OM_COMM_IsrNmi(VOS_VOID)
{
    /* Reserved */
}

/*****************************************************************************
 �� �� ��  : OM_COMM_ConnectedInd
 ��������  : OMģ�鱨������״̬
 �������  : VOS_VOID *pvOsaMsg
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��8��7��
    ��    ��   : W00164657
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 OM_COMM_MsgOmConnectCmd(VOS_VOID *pvOsaMsg)
{
    OM_COMM_SetOMConnStatus(CODEC_SWITCH_ON);

    return UCOM_RET_SUCC;
}

/*****************************************************************************
 �� �� ��  : OM_COMM_DisConnectedInd
 ��������  : OMģ�鱨������״̬
 �������  : VOS_VOID *pvOsaMsg
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2012��8��7��
    ��    ��   : W00164657
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 OM_COMM_MsgOmDisConnectCmd(VOS_VOID *pvOsaMsg)
{
    OM_COMM_SetOMConnStatus(CODEC_SWITCH_OFF);

    return UCOM_RET_SUCC;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


