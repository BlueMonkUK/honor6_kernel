/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : OmRl.c
  �� �� ��   : ����
  ��    ��   : ���� 46160
  ��������   : 2008��6��11��
  ����޸�   :
  ��������   : R�ӿ�����ת��ģ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2008��6��11��
    ��    ��   : ���� 46160
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "omrl.h"
#include "Omappagent.h"
#include "OmHdlcInterface.h"
#include "omringbuffer.h"
#include "omprivate.h"
#include "NVIM_Interface.h"
#include "FileSysInterface.h"
#include "pslogadapter.h"

#if (FEATURE_ON == FEATURE_MANUFACTURE_LOG)
#include "OmLittleImageLog.h"
#endif

#include "SCMProc.h"
#include "ombufmngr.h"
#include "OmToolInterface.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#define    THIS_FILE_ID        PS_FILE_ID_OMRL_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/*���ƻ��������ʵĻ����ź���*/
VOS_UINT32                              g_OmTxBuffSem;

/*����OMͨ����·��״̬�������Ƿ��������ͨ��*/
VOS_UINT32                              g_ulOMSwitchOnOff = OM_STATE_IDLE;

/* ��¼CCPU���յ�PC�����ݵ�������Ϣ */
OM_CCPU_PC_UE_SUC_STRU                  g_stPcToUeSucRecord;

/* ��¼CCPU���յ�PC�����ݵ��쳣��Ϣ */
OM_CCPU_PC_UE_FAIL_STRU                 g_stPcToUeErrRecord;

/*��¼��PC�෢�����ݵ�������Ϣ */
OM_UE_PC_SUC_STRU                       g_stUeToPcSucRecord;

/*��¼��PC�෢������ʱ�����ĸ������*/
OM_UE_PC_FAIL_STRU                      g_stUeToPcErrRecord;

/*��¼��ʼʱ��*/
VOS_UINT32                              g_ulLastTick = 0;

/*��¼�ֽڳ���*/
VOS_UINT32                              g_ulLastByteCnt = 0;

/*ָ��OM�ְ�ͷ���ݵ�ָ��*/
OM_APP_MSG_SEG_EX_STRU                 *g_pstSegMsgEx = VOS_NULL_PTR;

/* OM���������ƿ� */
OM_BUF_CTRL_STRU                        g_stOmBufCtrl;
extern OM_VCOM_DEBUG_INFO               g_stVComDebugInfo;

#if (FEATURE_ON == FEATURE_CBT_LOG)
extern OM_VCOM_CBT_LOG_DEBUG_INFO       g_stVComCBTDebugInfo;
#endif

extern OM_NV_OPERATE_RECORD_STRU        g_astRecordAcpuToCcpu[SAVE_MAX_SEND_INFO];
#ifdef OAM_UT_DEBUG
VOS_UINT32                              g_ulOmCcpuDbgFlag = VOS_NULL_DWORD;
#else
VOS_UINT32                              g_ulOmCcpuDbgFlag = VOS_FALSE;
#endif

VOS_VOID SCM_LogToFile(FILE *fp);
VOS_VOID SOCP_LogToFile(FILE *fp);

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_UINT32 OMRL_CcpuReportEvent(VOS_VOID)
{
    PS_OM_EVENT_IND_STRU    *pstEventInd;
    VOS_UINT32               ulEventLenth;
    VOS_UINT32               ulTemp = 0x5a5a5a5a;
    VOS_UINT32               ulIndex;
    VOS_UINT32               ulSendLen;
    OM_SEND_DEBUG_INFO_STRU  astCcpuDebuginfoTab2[3];
    VOS_UINT32               ulAddr;
    VOS_UINT16               usDebugEventID;

    /* ���C�˵�����Ϣ�� ��С�͵�ַ,�ְ��ϱ���Ϣ */
    OM_SEND_DEBUG_INFO_STRU  astCcpuDebuginfoTab1[] = {
        {sizeof(g_stPcToUeSucRecord),                     (VOS_UINT8*)&g_stPcToUeSucRecord},
        {sizeof(g_stPcToUeErrRecord),                     (VOS_UINT8*)&g_stPcToUeErrRecord},
        {sizeof(g_stUeToPcSucRecord),                     (VOS_UINT8*)&g_stUeToPcSucRecord},
        {sizeof(g_stUeToPcErrRecord),                     (VOS_UINT8*)&g_stUeToPcErrRecord},
        {sizeof(g_ulOMSwitchOnOff),                       (VOS_UINT8*)&g_ulOMSwitchOnOff  },
    };

    astCcpuDebuginfoTab2[0].pucAddr = (VOS_UINT8*)SCM_GetDebugLogInfo();
    astCcpuDebuginfoTab2[0].ulLenth = SCM_GetDebugLogInfoLen();

    astCcpuDebuginfoTab2[1].pucAddr = (VOS_UINT8*)SOCP_GetDebugLogInfo(OM_GET_SOCP_STATUS);
    astCcpuDebuginfoTab2[1].ulLenth = SOCP_GetDebugLogInfoLen(OM_GET_SOCP_STATUS);

    astCcpuDebuginfoTab2[2].pucAddr = (VOS_UINT8*)SOCP_GetDebugLogInfo(OM_GET_SOCP_DEBUG_INFO);
    astCcpuDebuginfoTab2[2].ulLenth = SOCP_GetDebugLogInfoLen(OM_GET_SOCP_DEBUG_INFO);

     /* ---------------��һ������ astAcpuDbuginfoTab1[]  begin----------- */
    ulEventLenth = (sizeof(PS_OM_EVENT_IND_STRU)-(4 * sizeof(VOS_UINT8)));
    for (ulIndex=0; ulIndex<(sizeof(astCcpuDebuginfoTab1)/sizeof(astCcpuDebuginfoTab1[0])); ulIndex ++)
    {
        ulEventLenth += astCcpuDebuginfoTab1[ulIndex].ulLenth;
    }

    /* ���Ϸָ����ų��� */
    ulEventLenth += (ulIndex - 1) * sizeof(VOS_UINT32);

    pstEventInd = (PS_OM_EVENT_IND_STRU *)VOS_MemAlloc(WUEPS_PID_OM, DYNAMIC_MEM_PT, ulEventLenth);
    if(VOS_NULL_PTR == pstEventInd)
    {
        LogPrint("OMRL_CcpuReportEvent: mem malloc Fail!\r\n");

        return VOS_ERR;
    }

    pstEventInd->ulLength   = ulEventLenth;
    pstEventInd->usEventId  = OAM_DEGUG_EVENT_ID1;
    pstEventInd->ulModuleId = WUEPS_PID_OM;

    /*lint -e419 -e416 */
    /* pc�ൽue��ʱ��������������Ϣ */
    VOS_MemCpy((VOS_UINT8 *)pstEventInd->aucData,
                  (VOS_UINT8*)&g_stPcToUeSucRecord,
                  sizeof(g_stPcToUeSucRecord));
    /*lint +e419 +e416 */

    ulEventLenth = 0;
    for (ulIndex=1;ulIndex<(sizeof(astCcpuDebuginfoTab1)/sizeof(astCcpuDebuginfoTab1[0])); ulIndex ++)
    {
        /*lint -e419 -e416 */
        /* ���Ϸָ����� */
        ulEventLenth += astCcpuDebuginfoTab1[ulIndex-1].ulLenth;
        VOS_MemCpy((VOS_UINT8 *)pstEventInd->aucData + ulEventLenth,
                   (VOS_UINT8 *)&ulTemp, sizeof(VOS_UINT32));

        ulEventLenth += sizeof(VOS_UINT32);
        VOS_MemCpy((VOS_UINT8 *)pstEventInd->aucData + ulEventLenth,
            astCcpuDebuginfoTab1[ulIndex].pucAddr, astCcpuDebuginfoTab1[ulIndex].ulLenth);
         /*lint +e419 +e416 */
    }

    if (VOS_OK != OM_Event(pstEventInd))
    {
        PS_LOG(WUEPS_PID_OM, VOS_NULL, PS_PRINT_WARNING, "OMRL_CcpuReportEvent: send event fail.");
    }
    VOS_MemFree(WUEPS_PID_OM, pstEventInd);
    /* ---------------��һ������ astAcpuDbuginfoTab1[]  end---------- */

    /* ---------------astAcpuDbuginfoTab2[] ÿ�����ݽϴ������ͣ�ÿ�������2k----------- */
    /* Event Id */
    usDebugEventID  = OAM_DEGUG_EVENT_ID2;

    /* һ�������� 2k �ռ䣬����ͷ� */
    pstEventInd = (PS_OM_EVENT_IND_STRU *)VOS_MemAlloc(WUEPS_PID_OM, DYNAMIC_MEM_PT,
                                          OM_SEND_DEBUG_INFO_MAX_LEN + sizeof(PS_OM_EVENT_IND_STRU)-(4 * sizeof(VOS_UINT8)));
    if (VOS_NULL_PTR == pstEventInd)
    {
        LogPrint("OMRL_AcpuReportEvent:Alloc Memory Failed!\n");
        return VOS_ERR;
    }

    /* ���ݲ�ַ��� */
    for(ulIndex =0; ulIndex<(sizeof(astCcpuDebuginfoTab2)/sizeof(astCcpuDebuginfoTab2[0])); ulIndex++)
    {
        /* �������ݿռ� ���Ϊ 2k*/
        ulSendLen = 0;

        while(ulSendLen < astCcpuDebuginfoTab2[ulIndex].ulLenth)
        {
            if((astCcpuDebuginfoTab2[ulIndex].ulLenth - ulSendLen) > OM_SEND_DEBUG_INFO_MAX_LEN)
            {
                ulEventLenth = OM_SEND_DEBUG_INFO_MAX_LEN;
            }
            else
            {
                ulEventLenth = astCcpuDebuginfoTab2[ulIndex].ulLenth - ulSendLen;
            }

            pstEventInd->ulLength   = ulEventLenth;
            pstEventInd->usEventId  = usDebugEventID++;
            pstEventInd->ulModuleId = WUEPS_PID_OM;

            /* make pclint happy  begin */
            ulAddr = (VOS_UINT32)pstEventInd->aucData;
            VOS_MemCpy((VOS_UINT8 *)ulAddr,
                       astCcpuDebuginfoTab2[ulIndex].pucAddr + ulSendLen, ulEventLenth);
            /* make pclint happy  end */

            ulSendLen += ulEventLenth;
            if (VOS_OK != OM_Event(pstEventInd))
            {
                PS_LOG(WUEPS_PID_OM, VOS_NULL, PS_PRINT_WARNING, "OMRL_AcpuReportEvent: send event fail.");
            }
        }
    }

    /* �ͷ���Ϣ���ݿռ� */
    VOS_MemFree(WUEPS_PID_OM, pstEventInd);

    return VOS_OK;
}


VOS_UINT32 OMRL_ReleaseSocpData(VOS_VOID)
{
    VOS_UINT32                          ulDataAddr;
    VOS_UINT32                          ulDataLen;
    VOS_UINT8                           *pucRealAddr;

#if 0
    VOS_BOOL                            bLoopFlag = VOS_TRUE;

    /* Ϊ�˲��Գ�֣������Ȱ�һ��һ��RD�ͷ� */
    do
    {
        /* ��SCM����ѷ�����Ϣ�ܳ��� */
        if (VOS_OK != SCM_RlsSrcRDFirst(SOCP_CODER_SRC_GU_OM2, &ulDataAddr, &ulDataLen))
        {
            g_stUeToPcErrRecord.usRlsRdErr++;

            return VOS_ERR;
        }

        /* �Ѿ�û��RD */
        if ((0 == ulDataAddr) || (0 == ulDataLen))
        {
            break;
        }

        g_stUeToPcSucRecord.stRealRlsData.ulDataLen += ulDataLen;
        g_stUeToPcSucRecord.stRealRlsData.ulNum++;

        pucRealAddr = (VOS_UINT8*)OM_AddrRealToVirt(&g_stOmBufCtrl, (VOS_UINT8*)ulDataAddr);

        if (VOS_NULL_PTR == pucRealAddr)
        {
            g_stUeToPcErrRecord.usRlsAddrErr++;

            return VOS_ERR;
        }

        /* �ͷ�OM�������ڴ� */
        if (VOS_OK != OM_ReleaseTraceMem(&g_stOmBufCtrl, pucRealAddr, (VOS_INT32)ulDataLen))
        {
            g_stUeToPcErrRecord.usRlsMemErr++;

            return VOS_ERR;
        }

    }while(bLoopFlag);

#else
    /* ��������ѷ���RD */
    if (VOS_OK != SCM_RlsSrcRDAll(SOCP_CODER_SRC_GU_OM2, &ulDataAddr, &ulDataLen))
    {
        g_stUeToPcErrRecord.usRlsRdErr++;

        return VOS_ERR;
    }

    /* �Ѿ�û��RD */
    if ((0 == ulDataAddr) || (0 == ulDataLen))
    {
        return VOS_OK;
    }

    g_stUeToPcSucRecord.stRealRlsData.ulDataLen += ulDataLen;
    g_stUeToPcSucRecord.stRealRlsData.ulNum++;

    pucRealAddr = (VOS_UINT8*)OM_AddrRealToVirt(&g_stOmBufCtrl, (VOS_UINT8*)ulDataAddr);

    if (VOS_NULL_PTR == pucRealAddr)
    {
        g_stUeToPcErrRecord.usRlsAddrErr++;

        return VOS_ERR;
    }

    /* �����ѷ���RD�ܳ����ͷ��ڴ� */
    if (VOS_OK != OM_ReleaseTraceMem(&g_stOmBufCtrl, pucRealAddr, (VOS_INT32)ulDataLen))
    {
        g_stUeToPcErrRecord.usRlsMemErr++;

        g_ulOMSwitchOnOff = OM_STATE_IDLE;

        return VOS_ERR;

    }
#endif

    return VOS_OK;
}
VOS_UINT32 OMRL_SendSocpData(VOS_UINT8 *pucHead, VOS_UINT32 ulHeadLen,
                             VOS_UINT8 *pucData, VOS_UINT32 ulDataLen)
{
    VOS_UINT8                          *pucMemAddr;
    VOS_INT32                           lUsedBytes;
    VOS_UINT8                          *pucRealMemAddr;

    /*lint -e40*/
    OM_CCPU_DEBUG_TRACE(pucHead, ulHeadLen, OM_CCPU_PUT_OM_BUFF);
    OM_CCPU_DEBUG_TRACE(pucData, ulDataLen, OM_CCPU_PUT_OM_BUFF);
    /*lint +e40*/

    /* sizeof(VOS_UINT32)������β�����ݳ��� */
    pucMemAddr = (VOS_UINT8*)OM_AllocTraceMem(&g_stOmBufCtrl, (VOS_INT32)(ulHeadLen + ulDataLen + sizeof(VOS_UINT32)));

    if (VOS_NULL_PTR == pucMemAddr)
    {
        g_stUeToPcErrRecord.usNoMemErr++;

        return VOS_ERR;
    }

    /* ��������ͷ */
    VOS_MemCpy(pucMemAddr, pucHead, ulHeadLen);

    /* ������������ */
    VOS_MemCpy(pucMemAddr + ulHeadLen, pucData, ulDataLen);

    /* ��д��β */
    *(VOS_UINT32*)(pucMemAddr + ulHeadLen + ulDataLen) = OM_USB_FRAME_TAIL;

    VOS_FlushCpuWriteBuf();

    pucRealMemAddr = (VOS_UINT8*)OM_AddrVirtToReal(&g_stOmBufCtrl, pucMemAddr);

    if (VOS_NULL_PTR == pucRealMemAddr)
    {
        g_stUeToPcErrRecord.usSendAddrErr++;

        return VOS_ERR;
    }

    /* ����SCM�ṩ�Ľӿڽ����ݷ��͸�SOCP */
    if(VOS_OK != SCM_SendCoderSrc(SOCP_CODER_SRC_GU_OM2, pucRealMemAddr, ulHeadLen + ulDataLen + sizeof(VOS_UINT32)))
    {
        g_stUeToPcErrRecord.usSendSocpDataErr++;

        return VOS_ERR;
    }

    g_stUeToPcSucRecord.stRealSendData.ulDataLen += (ulHeadLen + ulDataLen + sizeof(VOS_UINT32));
    g_stUeToPcSucRecord.stRealSendData.ulNum++;

    lUsedBytes = OM_TraceMemNBytes(&g_stOmBufCtrl);

    if (g_stUeToPcSucRecord.ulMaxUsedBufSize < (VOS_UINT32)lUsedBytes)
    {
        g_stUeToPcSucRecord.ulMaxUsedBufSize = (VOS_UINT32)lUsedBytes;
    }

    return VOS_OK;
}
VOS_UINT32 OMRL_IsBDEnough(VOS_UINT8 ucMsgCnt)
{
    VOS_UINT32                          ulFreeBdNum = 0;

    /* ��ȡOMԴͨ��ʣ��BD���� */
    if (VOS_OK != SCM_GetBDFreeNum(SOCP_CODER_SRC_GU_OM2, &ulFreeBdNum))
    {
        g_stUeToPcErrRecord.usGetBDErr++;

        return VOS_ERR;
    }

    /* �ж�ʣ��BD�Ƿ�����ְ����� */
    if (ucMsgCnt > ulFreeBdNum)
    {
        g_stUeToPcErrRecord.usBDLackErr++;

        return VOS_ERR;
    }

    return VOS_OK;
}

#if (FEATURE_ON == FEATURE_MANUFACTURE_LOG)

/*****************************************************************************
 �� �� ��  : OMRL_WriteLittleImg
 ��������  : ����С����ʱ������Ϣ��flash
 �������  : pucMsg    -- ָ�������ݵ�ָ��
             usMsgLen  -- �������ݵĳ���
 �������  : ��
 �� �� ֵ  : ture or false

 �޸���ʷ      :
  1.��    ��   : 2009��3��28��
    ��    ��   : ��Ц��
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_BOOL OMRL_WriteLittleImg(VOS_UINT8 *pucMsg, VOS_UINT16 usMsgLen)
{
    OM_APP_TRACE_STRU                  *pstAppTrace;
    VOS_UINT32                          ulTempAddress;
    VOS_UINT16                          usLogSaveLen;
    VOS_UINT32                         *pucLogSaveMsg;

    pstAppTrace = (OM_APP_TRACE_STRU*)pucMsg;

    if ((OM_TRACE_FUNC == pstAppTrace->stAppHeader.ucFuncType) && (OM_APP_TRACE_IND == pstAppTrace->usPrimId))
    {

        /* д��Traceͷ */
        OM_LittleImgWriteLogFile((VOS_CHAR*)pucMsg, OM_APP_TRACE_LEN);

        /* ��ȡ��Ϣ����ָ�� */
        ulTempAddress = *(VOS_UINT32*)(pstAppTrace->aucPara);

        pucLogSaveMsg = (VOS_UINT32*)ulTempAddress;

        usLogSaveLen  = usMsgLen - OM_APP_TRACE_LEN;

        OM_LittleImgWriteLogFile((VOS_CHAR*)pucLogSaveMsg, usLogSaveLen);

        return VOS_TRUE;
    }

    /* ��LOG��Ϣ���浽�ļ�ϵͳ */
    if (VOS_OK != OM_LittleImgWriteLogFile((VOS_CHAR*)pucMsg, usMsgLen))
    {
        PS_LOG(WUEPS_PID_OM, 0, LOG_LEVEL_INFO, "OMRL_MsgSplit: No need to send msg to PC.\n");

        return VOS_TRUE;
    }

    return VOS_FALSE;
}

#endif

/*****************************************************************************
 �� �� ��  : OMRL_ProcessSendData
 ��������  : ����ά�ɲ����ݷ��͵�SOCPģ��
 �������  : pucMsg    -- ָ�������ݵ�ָ��
             usMsgLen  -- �������ݵĳ���
             ucMsgCnt  -- ���������ݰ�����
 �������  : ��
 �� �� ֵ  : VOS_UINT32

 �޸���ʷ      :
  1.��    ��   : 2009��3��28��
    ��    ��   : ���� 47350
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 OMRL_ProcessSendData(VOS_UINT8 *pucMsg, VOS_UINT8 ucMsgCnt, VOS_UINT16 usMsgLen)
{
    VOS_UINT8                           ucCurSegNum = 1; /*��ǰ�����*/
    VOS_UINT32                          ulTempAddress;
    VOS_BOOL                            bIsHead     = VOS_FALSE;
    OM_APP_TRACE_STRU                  *pstAppTrace;

    pstAppTrace = (OM_APP_TRACE_STRU*)pucMsg;

    ulTempAddress = (VOS_UINT32)pucMsg;

    if ((OM_TRACE_FUNC == pstAppTrace->stAppHeader.ucFuncType)
        &&(OM_APP_TRACE_IND == pstAppTrace->usPrimId))
    {
        ulTempAddress = *(VOS_UINT32*)(pstAppTrace->aucPara);
        DRV_RT_MEMCPY(g_pstSegMsgEx + 1, pucMsg, OM_APP_TRACE_LEN);
        bIsHead = VOS_TRUE;
    }

    /* ���SOCPͷ�е�RTCֵ */
    g_pstSegMsgEx->stSocpHdr.ulRTC = (VOS_UINT32)DRV_GET_RTC_VALUE();

    g_pstSegMsgEx->stMsgSeg.ucSegNum = ucMsgCnt;
    g_pstSegMsgEx->stMsgSeg.usSegLen = OM_APP_SPLIT_MSG_LEN;


    /* �������ְ���С�����ݣ��������ְ���С�������ݷ��͵Ĵ��� */
    for (ucCurSegNum = 1; ucCurSegNum < ucMsgCnt; ucCurSegNum++)
    {
        g_pstSegMsgEx->stMsgSeg.ucSegSn = ucCurSegNum;

        if (VOS_TRUE == bIsHead)
        {
            /* ��Ϣͷǰ���ϳ����Ų����뻷�λ�����*/
            if (VOS_OK != OMRL_SendSocpData((VOS_UINT8 *)g_pstSegMsgEx,
                (sizeof(OM_APP_MSG_SEG_EX_STRU) + OM_APP_TRACE_LEN),
                (VOS_UINT8*)ulTempAddress, OM_APP_MSG_SEGMENT_LEN - OM_APP_TRACE_LEN))
            {
                return VOS_ERR;
            }

            bIsHead       =  VOS_FALSE;
            ulTempAddress += (OM_APP_MSG_SEGMENT_LEN - OM_APP_TRACE_LEN);
        }
        else
        {
            /* ��Ϣͷǰ���ϳ����Ų����뻷�λ�����*/
            if (VOS_OK != OMRL_SendSocpData((VOS_UINT8 *)g_pstSegMsgEx,
                 sizeof(OM_APP_MSG_SEG_EX_STRU),
                (VOS_UINT8*)ulTempAddress, OM_APP_MSG_SEGMENT_LEN))
            {
                return VOS_ERR;
            }

            ulTempAddress += OM_APP_MSG_SEGMENT_LEN;
        }

        /* ����ʣ�����ݰ���С */
        usMsgLen -= OM_APP_MSG_SEGMENT_LEN;
    }

    /* ������Ϣͷ���� */
    g_pstSegMsgEx->stMsgSeg.usSegLen = (VOS_UINT16)(usMsgLen + OM_APP_MSG_SEGMENT_HEADER_LENGTH);
    g_pstSegMsgEx->stMsgSeg.ucSegSn  = ucCurSegNum;

    /*Ϊ�˱���Ringbuffer�д�ŵ����ݰ�4�ֽڶ���*/
    usMsgLen = (usMsgLen + 3) & ~0x03;

    if (VOS_TRUE == bIsHead)
    {
        /* ��Ϣͷǰ���ϳ��Ȳ����뻷�λ����� */
        if ( VOS_OK != OMRL_SendSocpData((VOS_UINT8 *)g_pstSegMsgEx,
                            (sizeof(OM_APP_MSG_SEG_EX_STRU) + OM_APP_TRACE_LEN),
                            (VOS_UINT8*)ulTempAddress,
                            usMsgLen - OM_APP_TRACE_LEN))
        {
            return VOS_ERR;
        }
    }
    else
    {
        /* ��Ϣͷǰ���ϳ��Ȳ����뻷�λ����� */
        if ( VOS_OK != OMRL_SendSocpData((VOS_UINT8 *)g_pstSegMsgEx,
                                        sizeof(OM_APP_MSG_SEG_EX_STRU),
                                        (VOS_UINT8*)ulTempAddress,
                                        usMsgLen))
        {
            return VOS_ERR;
        }
    }

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : OMRL_MsgSplit
 ��������  : ����USB�Ͻ���ͨ��ʱ��ʹ�õ���չ�ְ�����
 �������  : pucMsg    -- ָ�������ݵ�ָ��
             usMsgLen  -- �������ݵĳ���
 �������  : ��
 �� �� ֵ  : VOS_UINT32

 �޸���ʷ      :
  1.��    ��   : 2009��3��28��
    ��    ��   : ���� 47350
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 OMRL_MsgSplit(VOS_UINT8 *pucMsg, VOS_UINT16 usMsgLen)
{
    VOS_UINT8                           ucMsgCnt    = 1; /*�ֶε�����*/
    VOS_UINT32                          ulRet;

    /*���л������*/
    if ( VOS_OK != VOS_SmP(g_OmTxBuffSem, OM_PV_TIMEOUT) )
    {
        LogPrint("OMRL_MsgSplit, Error, g_OmTxBuffSem VOS_SmP Failed.\n");

        g_stUeToPcErrRecord.usSemErr++;
        return VOS_ERR;
    }

#if (FEATURE_ON == FEATURE_MANUFACTURE_LOG)

    if ( VOS_TRUE == OMRL_WriteLittleImg(pucMsg, usMsgLen) )
    {
        VOS_SmV(g_OmTxBuffSem);

        return VOS_OK;
    }

#endif
    /* �ͷ��ѷ��������ڴ� */
    if (VOS_OK != OMRL_ReleaseSocpData())
    {
        VOS_SmV(g_OmTxBuffSem);

        return VOS_ERR;
    }

    /* ����ְ����� */
    ucMsgCnt = (VOS_UINT8)(((usMsgLen + OM_APP_MSG_SEGMENT_LEN) - 1)/OM_APP_MSG_SEGMENT_LEN);

    /* �ж�ʣ��BD�Ƿ�����ְ����� */
    if (VOS_OK != OMRL_IsBDEnough(ucMsgCnt))
    {
        VOS_SmV(g_OmTxBuffSem);

        return VOS_ERR;
    }

    ulRet = OMRL_ProcessSendData(pucMsg, ucMsgCnt, usMsgLen);

    VOS_SmV(g_OmTxBuffSem);

    return ulRet;
}
VOS_UINT32 OMRL_UsbFrameInit(VOS_VOID)
{
    /*����ְ��ṹ���ڴ�ռ�*/
    g_pstSegMsgEx = (OM_APP_MSG_SEG_EX_STRU*)VOS_MemAlloc(WUEPS_PID_OM,
                STATIC_MEM_PT, sizeof(OM_APP_MSG_SEG_EX_STRU) + OM_APP_TRACE_LEN);

    if (VOS_NULL_PTR == g_pstSegMsgEx)
    {
        LogPrint("OM_UsbFrameInit:VOS_MemAlloc g_pstSegMsgEx Failed!\n");
        return VOS_ERR;
    }

    g_pstSegMsgEx->stSocpHdr.ucCPUID   = OM_TOOL_CCPU_ID;
    g_pstSegMsgEx->ulFrameHead         = OM_USB_FRAME_HEAD;

    /* ����OM���ͻ����� */
    if (VOS_OK != OM_CreateTraceBuffer(&g_stOmBufCtrl, OM_UPLINK_BUFFER_SIZE))
    {
        VOS_MemFree(ACPU_PID_OM, g_pstSegMsgEx);

        LogPrint("OM_UsbFrameInit:OM_CreateTraceBuffer Failed!\n");

        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_INT OM_SaveTraceBuffer(VOS_INT lFuncType, VOS_UINT8* pucData, VOS_UINT32* pulLen)
{
    /*�������*/
    if ((OM_TRACE_FUNC != lFuncType) || (VOS_NULL_PTR == pucData)
        || (VOS_NULL_PTR == pulLen) || (0 == *pulLen))
    {
        LogPrint("OM_SaveBuffer:Input para. is not correct.\n");

        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_INT OM_SaveLogBuffer(VOS_INT lFuncType, VOS_UINT8* pucData, VOS_UINT32* pulLen)
{
    OM_RING_ID      ringId;
    VOS_INT         lPacketLen = 0;
    VOS_INT         lBufferLen;
    VOS_UINT8       *pucTemp;

    if ((OM_LOG_FUNC != lFuncType) || (VOS_NULL_PTR == pucData)
        || (VOS_NULL_PTR == pulLen) || (0 == *pulLen))
    {
        LogPrint("OM_SaveBuffer:Input para. is not correct.\n");

        return VOS_ERR;
    }

    /*�õ�LOG���ݻ�����*/
    ringId = g_stLogEnt.rngOmRbufId;

    lBufferLen = (VOS_INT)(*pulLen);

    pucTemp = pucData;

    VOS_TaskLock();

    while (!OM_RingBufferIsEmpty(ringId))
    {
        if (sizeof(lPacketLen) != OM_RingBufferGet(ringId,
                                        (VOS_CHAR*)&lPacketLen, sizeof(lPacketLen)))
        {
            LogPrint("OM_SaveBuffer:LOG OM_RingBufferGet fail\n");

            break;
        }

        /*ɾ�������LOG Buffer�е�ģ��ID*/
        if (LOG_MODULE_ID_LEN != OM_RingBufferRemove(ringId, LOG_MODULE_ID_LEN))
        {
            LogPrint("OM_SaveBuffer:LOG OM_RingBufferRemove fail\n");
            break;
        }

        lPacketLen -= LOG_MODULE_ID_LEN;

        /*�жϻ������Ƿ�����*/
        if (lBufferLen < (lPacketLen + 1))
        {
            break;
        }

        /*��Ringbuffer��ɾ������*/
        if (lPacketLen != OM_RingBufferGet(ringId, (VOS_CHAR*)pucTemp, lPacketLen))
        {
            LogPrint1("OM_SaveBuffer:LOG OM_RingBufferGet len : %d\n", lPacketLen);

            break;
        }

        /*Ϊ�˴�㹤�߽�������Ҫ��\n*/
        pucTemp[lPacketLen] = '\n';

        /*��1����Ϊǰ������\n����*/
        pucTemp += (lPacketLen + 1);

        lBufferLen -= (lPacketLen + 1);
    }

    VOS_TaskUnlock();

    *pulLen = (VOS_UINT32)(pucTemp - pucData);

    return VOS_OK;
}


VOS_VOID OmInitVar(VOS_VOID)
{
    VOS_MemSet(&g_stUeToPcErrRecord, 0, sizeof(g_stUeToPcErrRecord));
    VOS_MemSet(&g_stUeToPcSucRecord, 0, sizeof(g_stUeToPcSucRecord));
    VOS_MemSet(&g_stPcToUeErrRecord, 0, sizeof(g_stPcToUeErrRecord));
    VOS_MemSet(&g_stPcToUeSucRecord, 0, sizeof(g_stPcToUeSucRecord));
}


VOS_UINT32 OMRL_SendMsg(VOS_UINT8 *pucData, VOS_UINT32 ulSize, VOS_UINT32 ulSenderPid, VOS_UINT32 ulReceiverPid)
{

    MsgBlock           *pstAppOmMsg = VOS_NULL_PTR;
    VOS_UINT32          ulRslt;

    pstAppOmMsg = VOS_AllocMsg(ulSenderPid, ulSize);
    if (VOS_NULL_PTR == pstAppOmMsg)
    {
        /* ��ӡ���� */
        return VOS_ERR;
    }

    pstAppOmMsg->ulReceiverPid = ulReceiverPid;

    /* copy ��Ϣ���� */
    VOS_MemCpy((VOS_UINT8 *)pstAppOmMsg->aucValue, pucData, ulSize);

    ulRslt = VOS_SendMsg(ulSenderPid, pstAppOmMsg);
    if (VOS_OK != ulRslt )
    {
        /* ��ӡ���� */
        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 OMRL_IccDataRecv(VOS_UINT8 *pucData, VOS_UINT16 uslen)
{

    g_stPcToUeSucRecord.stTotalData.ulDataLen += (VOS_UINT32)uslen;
    g_stPcToUeSucRecord.stTotalData.ulNum++;

    if ((VOS_NULL_PTR == pucData) || (0 == uslen) || (APP_OM_HEADER_LENGTH >= uslen))
    {
        PS_LOG(WUEPS_PID_OM, 0, LOG_LEVEL_ERROR, "OMRL_IccDataRecv: Parameter Error!");
        g_stPcToUeErrRecord.stParaErrData.ulDataLen += uslen;
        g_stPcToUeErrRecord.stParaErrData.ulNum++;

        return VOS_ERR;
    }

    /*lint -e40*/
    OM_CCPU_DEBUG_TRACE(pucData, uslen, OM_CCPU_RECV_ICC);
    /*lint +e40*/

    /* �������ͷ���Ϣ����ʱֹͣ�������� */
    if (0 < g_stUeToPcErrRecord.usRlsMemErr)
    {
        return VOS_OK;
    }

    if (VOS_OK != OMRL_SendMsg(pucData, (VOS_UINT32)uslen, UEPS_PID_OMRL, WUEPS_PID_OM))
    {
        g_stPcToUeErrRecord.stSendErrData.ulDataLen += uslen;
        g_stPcToUeErrRecord.stSendErrData.ulNum++;

        return VOS_ERR;
    }

    g_stPcToUeSucRecord.stRealData.ulDataLen += uslen;
    g_stPcToUeSucRecord.stRealData.ulNum++;

    return VOS_OK;
}
VOS_UINT32 OMRL_Init(VOS_VOID)
{
    OmInitVar();

    if(VOS_OK != VOS_SmMCreate("TXRB", VOS_SEMA4_PRIOR | VOS_SEMA4_INVERSION_SAFE, &g_OmTxBuffSem))
    {
        LogPrint("RL_Init: Error, VOS_SmMCreate Fail\n");
        return VOS_ERR;
    }

    if (VOS_OK != OMRL_UsbFrameInit())
    {
        LogPrint("RL_Init:OM_UsbFrameInit Fail.\n");
        return VOS_ERR;
    }

    if (VOS_OK != DRV_SAVE_REGISTER(OM_TRACE_FUNC, (OM_SAVE_FUNC*)OM_SaveTraceBuffer))
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "RL_Init: DRV_SAVE_REGISTER OM_TRACE_FUNC Fail!");
    }

    if (VOS_OK != DRV_SAVE_REGISTER(OM_LOG_FUNC, (OM_SAVE_FUNC*)OM_SaveLogBuffer))
    {
        PS_LOG(WUEPS_PID_OM, 0, PS_PRINT_WARNING, "RL_Init: DRV_SAVE_REGISTER OM_LOG_FUNC Fail!");
    }

    OM_ComRx_RegisterCB(OM_OM_ICC_CHANNEL, OMRL_IccDataRecv);

    return VOS_OK;
}


VOS_VOID OmSendShow(VOS_VOID)
{
    vos_printf("Total Data Send by Ps: num:%6u, len:%6u\n",
                g_stUeToPcSucRecord.stTotalData.ulNum, g_stUeToPcSucRecord.stTotalData.ulDataLen);

    vos_printf("Real Data send to Socp: num:%6u, len:%6u\n",
                g_stUeToPcSucRecord.stRealSendData.ulNum, g_stUeToPcSucRecord.stRealSendData.ulDataLen);

    vos_printf("Real Data release by Socp: num:%6u, len:%6u\n",
                g_stUeToPcSucRecord.stRealRlsData.ulNum, g_stUeToPcSucRecord.stRealRlsData.ulDataLen);

    vos_printf("Max Used Buffer Size: size:%6u\n", g_stUeToPcSucRecord.ulMaxUsedBufSize);

    return;
}


VOS_VOID OmSendErrShow(VOS_VOID)
{
    vos_printf("Total Lost Data: num:%6u, len:%6u\n",
                g_stUeToPcErrRecord.stLostData.ulNum, g_stUeToPcErrRecord.stLostData.ulDataLen);

    vos_printf("Link Err Data: num:%6u, len:%6u\n",
                g_stUeToPcErrRecord.stLinkErrData.ulNum, g_stUeToPcErrRecord.stLinkErrData.ulDataLen);

    vos_printf("Para Err Data: num:%6u, len:%6u\n",
                g_stUeToPcErrRecord.stParaErrData.ulNum, g_stUeToPcErrRecord.stParaErrData.ulDataLen);

    vos_printf("num of sem take err: num:%6u\n", g_stUeToPcErrRecord.usSemErr);

    vos_printf("num of release RD err: num:%6u\n", g_stUeToPcErrRecord.usRlsRdErr);

    vos_printf("num of release Mem err: num:%6u\n", g_stUeToPcErrRecord.usRlsMemErr);

    vos_printf("num of BD Lack err: num:%6u\n", g_stUeToPcErrRecord.usBDLackErr);

    vos_printf("num of get BD err: num:%6u\n", g_stUeToPcErrRecord.usGetBDErr);

    vos_printf("num of No Mem err: num:%6u\n", g_stUeToPcErrRecord.usNoMemErr);

    vos_printf("num of Send Socp Data err: num:%6u\n", g_stUeToPcErrRecord.usSendSocpDataErr);

    vos_printf("num of transfer sending addr err: num:%6u\n", g_stUeToPcErrRecord.usSendAddrErr);

    vos_printf("num of transfer Release addr err: num:%6u\n", g_stUeToPcErrRecord.usRlsAddrErr);

    return;
}



VOS_VOID OmRcvShow(VOS_VOID)
{
    vos_printf("Total Data Recv From ACPU: num:%6u, len:%6u\n",
                g_stPcToUeSucRecord.stTotalData.ulNum, g_stPcToUeSucRecord.stTotalData.ulDataLen);

    vos_printf("Data send OM:   num:%6u, len:%6u\n",
                g_stPcToUeSucRecord.stRealData.ulNum, g_stPcToUeSucRecord.stRealData.ulDataLen);

    vos_printf("Data Recv From ICC: num:%6u, len:%6u, slice:%6u\n",
                g_stPcToUeSucRecord.stICCRecSUCInfo.ulICCOmRecMsgNum,
                g_stPcToUeSucRecord.stICCRecSUCInfo.ulICCOmRecLen,
                g_stPcToUeSucRecord.stICCRecSUCInfo.ulICCOmRecSlice);

    return;
}


VOS_VOID OmRcvErrShow(VOS_VOID)
{
    vos_printf("Lost Data because para Err: num:%6u, len:%6u\n",
            g_stPcToUeErrRecord.stParaErrData.ulNum, g_stPcToUeErrRecord.stParaErrData.ulDataLen);

    vos_printf("Err data when sending msg to om: num:%6u, len:%6u\n",
            g_stPcToUeErrRecord.stSendErrData.ulNum, g_stPcToUeErrRecord.stSendErrData.ulDataLen);

    vos_printf("Data rcv err from ICC: ParaErrNum:%6u, len:%6u, MsgNum:%6u, slice:%6u\n",
            g_stPcToUeErrRecord.stICCRecFailInfo.ulICCOmParaErrNum,
            g_stPcToUeErrRecord.stICCRecFailInfo.ulICCOmRecErrLen,
            g_stPcToUeErrRecord.stICCRecFailInfo.ulICCOmRecErrMsgNum,
            g_stPcToUeErrRecord.stICCRecFailInfo.ulICCOmRecErrSlice);

    return;
}


VOS_VOID OmStatus(VOS_UINT32 ulFlag)
{
    return;
}


VOS_VOID OmRate(VOS_VOID)
{
#if 0
    VOS_UINT32            ulCurrTick;                    /*��¼����ͳ��ʱ��*/
    VOS_UINT32            ulDeltaTick;                   /*��¼ͳ��ʱ��*/
    VOS_UINT32            ulDelData;                     /*��¼ͳ�����ݴ�С*/
    VOS_UINT32            ulBandWidth;                   /*��¼ͳ�����ݴ�С*/

    vos_printf("OM Rate Show (CCPU To SOCP):\n");

    ulCurrTick = VOS_GetTick();

    if (ulCurrTick < g_ulLastTick)
    {
        ulDeltaTick = (0xFFFFFFFF - g_ulLastTick) + ulCurrTick;
    }
    else
    {
        ulDeltaTick = ulCurrTick - g_ulLastTick;
    }


    ulBandWidth    = (ulDelData/ulDeltaTick)/10;

    /*ͳ��OM��ulDeltaTickʱ��(��λ10ms)�ڷ�����ulTotalLen�ֽ�����*/
    vos_printf("OmRate,Tick <%u> Byte <%u>, BandWidth <%u>.\n",
                 ulDeltaTick, ulDelData, ulBandWidth);

    g_ulLastTick = ulCurrTick;

    vos_printf("OM Rate Show End\n");
#endif
}
VOS_VOID OmOpenCCPULog(VOS_UINT32 ulFlag)
{
    g_ulOmCcpuDbgFlag = ulFlag;

    return;
}

/*****************************************************************************
 Prototype       : GU_OmCcpuSwitchOnOff
 Description     : ON or Off Ccpu Switch
 Input           : VOS_UINT32
 Output          : None
 Return Value    : None

 History         : ---
    Date         :
    Author       :
    Modification : Created function
 *****************************************************************************/
VOS_VOID GU_OmCcpuSwitchOnOff(VOS_UINT32 ulFlag)
{
    g_ulOMSwitchOnOff = ulFlag;
}


VOS_VOID OM_CpuReadNVLog(VOS_VOID)
{
    FILE                               *fp;
    VOS_UINT32                          ulTemp = 0x5a5a5a5a;

#if(VOS_WIN32 == VOS_OS_VER)
    VOS_CHAR                  acLogPath[] = ".\\yaffs0\\C_NV_OmLog.bin";

#else
#if (FEATURE_ON == FEATURE_MULTI_FS_PARTITION)
    VOS_CHAR                  acLogPath[] = "/modem_log/C_NV_OmLog.bin";

#else
    VOS_CHAR                  acLogPath[] = "/yaffs0/C_NV_OmLog.bin";
#endif
#endif

    fp = DRV_FILE_OPEN(acLogPath, "w+");

    if (VOS_NULL_PTR == fp)
    {
        return;
    }

    DRV_FILE_WRITE((VOS_VOID*)g_astRecordAcpuToCcpu, sizeof(VOS_CHAR), sizeof(g_astRecordAcpuToCcpu), fp);
    DRV_FILE_WRITE((VOS_VOID*)&ulTemp, sizeof(VOS_CHAR), sizeof(VOS_UINT32), fp);

    DRV_FILE_CLOSE(fp);

}


VOS_VOID OM_LogShowToFile(VOS_BOOL bIsSendMsg)
{
    FILE                               *fp;
    OM_AUTOCONFIG_CNF_STRU             *pstSendCnf;
    VOS_UINT32                          ulTemp = 0x5a5a5a5a;

#if(VOS_WIN32 == VOS_OS_VER)
    VOS_CHAR                  g_acLogPath[] = ".\\yaffs0\\C_OmLog.bin";

#else
#if (FEATURE_ON == FEATURE_MULTI_FS_PARTITION)
    VOS_CHAR                  g_acLogPath[] = "/modem_log/C_OmLog.bin";

#else
    VOS_CHAR                  g_acLogPath[] = "/yaffs0/C_OmLog.bin";
#endif
#endif

    fp = DRV_FILE_OPEN(g_acLogPath, "w+");

    if (VOS_NULL_PTR == fp)
    {
        return;
    }

    DRV_FILE_WRITE((VOS_VOID*)&g_stPcToUeSucRecord, sizeof(VOS_CHAR), sizeof(g_stPcToUeSucRecord), fp);
    DRV_FILE_WRITE((VOS_VOID*)&ulTemp, sizeof(VOS_CHAR), sizeof(VOS_UINT32), fp);
    DRV_FILE_WRITE((VOS_VOID*)&g_stPcToUeErrRecord, sizeof(VOS_CHAR), sizeof(g_stPcToUeErrRecord), fp);
    DRV_FILE_WRITE((VOS_VOID*)&ulTemp, sizeof(VOS_CHAR), sizeof(VOS_UINT32), fp);

    DRV_FILE_WRITE((VOS_VOID*)&g_stUeToPcSucRecord, sizeof(VOS_CHAR), sizeof(g_stUeToPcSucRecord), fp);
    DRV_FILE_WRITE((VOS_VOID*)&ulTemp, sizeof(VOS_CHAR), sizeof(VOS_UINT32), fp);
    DRV_FILE_WRITE((VOS_VOID*)&g_stUeToPcErrRecord, sizeof(VOS_CHAR), sizeof(g_stUeToPcErrRecord), fp);
    DRV_FILE_WRITE((VOS_VOID*)&ulTemp, sizeof(VOS_CHAR), sizeof(VOS_UINT32), fp);

    DRV_FILE_WRITE((VOS_VOID*)&g_ulOMSwitchOnOff, sizeof(VOS_CHAR), sizeof(g_ulOMSwitchOnOff), fp);
    DRV_FILE_WRITE((VOS_VOID*)&ulTemp, sizeof(VOS_CHAR), sizeof(VOS_UINT32), fp);

    /* SCM ���*/
    SCM_LogToFile(fp);
    DRV_FILE_WRITE((VOS_VOID*)&ulTemp, sizeof(VOS_CHAR), sizeof(VOS_UINT32), fp);

    /* SOCP��� */
    SOCP_LogToFile(fp);
    DRV_FILE_WRITE((VOS_VOID*)&ulTemp, sizeof(VOS_CHAR), sizeof(VOS_UINT32), fp);
    DRV_FILE_CLOSE(fp);

    OM_CpuReadNVLog();

    /* ������Ϣ�o Acpu */
    if(VOS_FALSE == bIsSendMsg)
    {
        return;
    }

    pstSendCnf = (OM_AUTOCONFIG_CNF_STRU*)VOS_AllocMsg(CCPU_PID_OMAGENT,
                            sizeof(OM_AUTOCONFIG_CNF_STRU) - VOS_MSG_HEAD_LENGTH);

    /* ������Ϣʧ�� */
    if (VOS_NULL_PTR == pstSendCnf)
    {
        return;
    }

    pstSendCnf->ulReceiverPid  = ACPU_PID_OMAGENT;
    pstSendCnf->usPrimId       = OM_RECORD_DBU_INFO_REQ;

    (VOS_VOID)VOS_SendMsg(CCPU_PID_OMAGENT, pstSendCnf);

    return;
}

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

