
/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "om.h"
#include "SOCPInterface.h"
#include "omprivate.h"
#include "ombbplog.h"
#include "SCMProc.h"
#include "BSP_SOCP_DRV.h"
#include "NvIdList.h"
#include "NVIM_Interface.h"
#include "bbp_wcdma_interface.h"
#include "bbp_comm_interface.h"
#include "gbbp_interface.h"
#include "omnvinterface.h"



#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


/*lint -e767 �޸��ˣ����±� */
#define    THIS_FILE_ID        PS_FILE_ID_OM_BBP_LOG_C
/*lint +e767 �޸��ˣ����±� */

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/* GU BBP ��ά�ɲ��ϱ�SOCP ͨ�����ò��� */
SCM_CODER_SRC_CFG_STRU                  g_astBbpDbgCoderSrcCfg[BBP_DBG_CODER_SRC_NUM]=
{
    {SCM_CHANNEL_UNINIT, SOCP_CODER_SRC_GUBBP1,   SOCP_CODER_DST_GU_OM,   SOCP_DATA_TYPE_1, SOCP_ENCSRC_CHNMODE_CTSPACKET, SOCP_CHAN_PRIORITY_3, VOS_NULL_PTR, VOS_NULL_PTR, BBP_DBG_CODER_SRC_SIZE, VOS_NULL_PTR, VOS_NULL_PTR, VOS_NULL_PTR},
    {SCM_CHANNEL_UNINIT, SOCP_CODER_SRC_GUBBP2,   SOCP_CODER_DST_GU_OM,   SOCP_DATA_TYPE_1, SOCP_ENCSRC_CHNMODE_CTSPACKET, SOCP_CHAN_PRIORITY_3, VOS_NULL_PTR, VOS_NULL_PTR, BBP_DBG_CODER_SRC_SIZE, VOS_NULL_PTR, VOS_NULL_PTR, VOS_NULL_PTR}
};

/* ��¼BBP ��ά�ɲ��ϱ���ʼ���ĵ�����Ϣ */
BBP_DBG_ERR_INFO_STRU                   g_stBbpDbgErrInfo;

/* ��¼BBP ��ά�ɲ��ϱ���ȫ����Ϣ */
BBP_DBG_INFO_STRU                       g_stBbpDbgInfo;

/* ��¼BBP ���ɵĵ�����Ϣ */
BBP_DUMP_ERR_INFO_STRU                  g_stBbpDumpErrInfo;

/* ��¼BBP���ɵ�ȫ����Ϣ */
BBP_DUMP_INFO_STRU                      g_stBbpDumpInfo;

/* ������Ϣ��ַ�������������ж��з�����Ϣ */
MsgBlock*                               g_pstBbpDumpMsg         = VOS_NULL_PTR;

/* ������Ϣ��ַ�������ڿ�ά�ɲ��ж��з�����Ϣ */
MsgBlock*                               g_pstBbpDbgMsg          = VOS_NULL_PTR;

/* ����EDMAѭ������ͷָ�� */
BALONG_DMA_CB                           *g_pstBbpCycBufHead       = VOS_NULL_PTR;

/*����BBP��ά�ɲ�ļ����ź���*/
VOS_UINT32                              g_ulBbpDbgSem = VOS_NULL;

/*****************************************************************************
 3  ��������
*****************************************************************************/
extern VOS_UINT32 Om_ConfigBbpDump(APP_OM_MSG_EX_STRU *pstAppToOmMsg,
                                           VOS_UINT16 usReturnPrimId);

extern VOS_UINT32 Om_EndBbpDump(APP_OM_MSG_EX_STRU *pstAppToOmMsg,
                                           VOS_UINT16 usReturnPrimId);

extern VOS_UINT32 Om_ConfigBbpDbg(APP_OM_MSG_EX_STRU *pstAppToOmMsg,
                                           VOS_UINT16 usReturnPrimId);

extern VOS_UINT32 Om_SendEdmaErrToPc(APP_OM_MSG_EX_STRU *pstAppToOmMsg,
                                           VOS_UINT16 usReturnPrimId);

extern VOS_UINT32 Om_QueryMsgFunc(OM_REQ_PACKET_STRU *pRspPacket,
                            OM_MSG_FUN_STRU *pstMsgFuncTable, VOS_UINT32 ulTotalNum);

/*****************************************************************************
 4 ��Ϣ������ӳ���
*****************************************************************************/
/*PC�·��� BBP�������ά�ɲ���Ϣ������ӳ��� */
OM_MSG_FUN_STRU                         g_astOmBbpMsgFunTbl[]   =
{
    {APP_OM_BBP_DUMP_SET_REQ,      Om_ConfigBbpDump     ,    OM_APP_BBP_DUMP_SET_CNF },
    {APP_OM_BBP_DUMP_END_REQ ,     Om_EndBbpDump        ,    OM_APP_BBP_DUMP_END_CNF },
    {APP_OM_BBP_DBG_SET_REQ,       Om_ConfigBbpDbg      ,    OM_APP_BBP_DBG_SET_CNF  },
    {APP_OM_BBP_EDMA_ERR_REQ ,     Om_SendEdmaErrToPc   ,    OM_APP_BBP_EDMA_ERR_CNF },
};

/*****************************************************************************
  5 ����ʵ��
*****************************************************************************/

VOS_UINT32 Om_CheckDumpLen(VOS_UINT32 ulLen)
{
    VOS_UINT32                          ulSizeTemp;

    /* �ж����ɳ����Ƿ�С����С���� */
    if (ulLen < BBP_DUMP_MIN_LEN)
    {
        return BBP_DUMP_MEM_SMALL;
    }

    /* �ж��Ƿ����2G */
    if (ulLen > BBP_DUMP_MAX_LEN)
    {
        return BBP_DUMP_MEM_BIG;
    }

    /* �ж����ɳ����Ƿ���32KB�������� */
    ulSizeTemp = ulLen % BBP_DUMP_UNIT_LEN;
    if (0 != ulSizeTemp)
    {
        return BBP_DUMP_MEM_NOT_32KB;
    }

    return BBP_DUMP_OK;
}
VOS_UINT32 Om_SendEdmaErrToPc(APP_OM_MSG_EX_STRU *pstAppToOmMsg,
                                           VOS_UINT16 usReturnPrimId)
{
    OM_REQ_PACKET_STRU                 *pstPcMsg;
    BBP_EDMA_ERR_STRU                  *pstEdmaErr;

    pstPcMsg    = (OM_REQ_PACKET_STRU*)pstAppToOmMsg->aucPara;
    pstEdmaErr  = (BBP_EDMA_ERR_STRU*)pstPcMsg->aucCommand;
    OM_SendResult(pstAppToOmMsg->ucFuncType, pstEdmaErr->enEdmaErr, usReturnPrimId);

    return VOS_OK;
}



VOS_VOID Om_CloseBbpDump(VOS_VOID)
{
    /* ���������ݲɼ� */
    OM_BBP_REG_SETBITS(WBBP_DUMP_START_W_ADDR, 0, 1, 0x0);

    /* ���ǿ��ֹͣ���ݲɼ� */
    OM_BBP_REG_SETBITS(WBBP_DUMP_STOP_W_ADDR, 0, 1, 0x1);
}


VOS_UINT32 Om_DumpEdmaCallBack(VOS_UINT32 ulChannelArg, VOS_UINT32 ulFlag)
{
    OM_REQ_PACKET_STRU                 *pstPcMsg;
    BBP_EDMA_ERR_STRU                  *pstEdmaErr;

    LogPrint("Om_DumpEdmaCallBack: Start\r\n");

    /* �ر����� */
    Om_CloseBbpDump();

    if (VOS_NULL_PTR == g_pstBbpDumpMsg)
    {
        LogPrint("Om_DumpEdmaCallBack: g_pstBbpDumpMsg is NULL\r\n");
        return VOS_ERR;
    }

    pstPcMsg = (OM_REQ_PACKET_STRU*)g_pstBbpDumpMsg->aucValue;

    /* DMA��������ж� */
    if ( BALONG_DMA_INT_DONE == ulFlag)
    {
        g_stBbpDumpInfo.usDUMPDone++;
        /* ģ�⹤�߷�����Ϣ֪ͨ CCPU OM�������� */
        *(VOS_UINT16 *)pstPcMsg->aucCommand = (VOS_UINT16)APP_OM_BBP_DUMP_END_REQ;
        (VOS_VOID)VOS_SendMsg(UEPS_PID_OMRL, g_pstBbpDumpMsg);
    }
    else /* EDMA�����ж� */
    {
        pstEdmaErr = (BBP_EDMA_ERR_STRU*)pstPcMsg->aucCommand;

        pstEdmaErr->usMsgName           = (VOS_UINT16)APP_OM_BBP_EDMA_ERR_REQ;

        /* ��ʶ�ǿ�ά�ɲ������EDMA�����ж� */
        pstEdmaErr->enEdmaErr           = OM_BBP_DUMP_EDMA_ERR;

        /* ģ��PC��CCPU OM������Ϣ */
        (VOS_VOID)VOS_SendMsg(UEPS_PID_OMRL, g_pstBbpDumpMsg);
        g_stBbpDumpErrInfo.usEdmaErrCode = (VOS_UINT16)ulFlag;
    }

    g_stBbpDumpInfo.usDUMPStatus = OM_BBP_STOP;

    return VOS_OK;
}


VOS_UINT32 Om_BbpDumpEdmaInit(VOS_VOID)
{
    VOS_INT                             lRet;
    OM_REQ_PACKET_STRU                 *pstPcMsg;
    OM_BBP_DUMP_ENABLE_STRU             stEnableFlag;

    stEnableFlag.usEnable = OM_BBP_DUMP_DISABLE;

    /*��NV��*/
    if(NV_OK != NV_Read(en_NV_Item_BBP_DUMP_ENABLE,
        (VOS_VOID *)(&stEnableFlag), sizeof(OM_BBP_DUMP_ENABLE_STRU)))
    {
        LogPrint("Om_BbpDumpEdmaInit: Read Nv Fail\r\n");
        return VOS_ERR;
    }

    /* �ж��Ƿ�����BBP���� */
    if (OM_BBP_DUMP_DISABLE == stEnableFlag.usEnable)
    {
        LogPrint("Om_BbpDumpEdmaInit: Not Enable BBP DUMP\r\n");
        return VOS_OK;
    }

    VOS_MemSet(&g_stBbpDumpInfo,    0, sizeof(g_stBbpDumpInfo));
    VOS_MemSet(&g_stBbpDumpErrInfo, 0, sizeof(g_stBbpDumpErrInfo));

    /* ����DRV���� ��ʼ��BBP DUMPʹ�õ�EDMA,��ע��ص� */
    g_stBbpDumpInfo.lEdmaChanID = DRV_EDMA_CHANNEL_INIT(EDMA_BBP_GRIF,
                                    (channel_isr)Om_DumpEdmaCallBack,
                                    VOS_NULL, BBP_DUMP_EDMA_INT);
    if (g_stBbpDumpInfo.lEdmaChanID < 0)
    {
        g_stBbpDumpErrInfo.usInitEdmaErr++;
        return VOS_ERR;
    }

    /* ����DRV�������� EDMA , BBP���ɵ�burst����Ϊ16 * 32bit */
    lRet = DRV_EDMA_CHANNEL_CONFIG((VOS_UINT32)g_stBbpDumpInfo.lEdmaChanID, BALONG_DMA_P2M, EDMA_TRANS_WIDTH_32, EDMA_BUR_LEN_16);
    if (BSP_OK != lRet)
    {
        g_stBbpDumpErrInfo.usConfigEdmaErr++;
        return VOS_ERR;
    }

    /* ����һ��������Ϣ�������ж��з��� */
    g_pstBbpDumpMsg = (MsgBlock*)VOS_AllocMsg(UEPS_PID_OMRL, sizeof(OM_REQ_PACKET_STRU));
    if (VOS_NULL_PTR == g_pstBbpDumpMsg)
    {
        g_stBbpDumpErrInfo.usAllocMsgErr++;
        return VOS_ERR;
    }
    pstPcMsg = (OM_REQ_PACKET_STRU*)g_pstBbpDumpMsg->aucValue;
    pstPcMsg->ucModuleId = OM_BBP_FUNC;
    g_pstBbpDumpMsg->ulReceiverPid = WUEPS_PID_OM;

    /* ��Ǹ���Ϣ�����ͷ� */
    VOS_ReserveMsg(UEPS_PID_OMRL, g_pstBbpDumpMsg);

    LogPrint("Om_BbpDumpEdmaInit: ok!\r\n");

    return VOS_OK;

}


VOS_UINT32 Om_ConfigBbpDumpEdma(VOS_UINT32 ulLen,
                            VOS_UINT32* pulPhyAddr, OM_BBP_DUMP_MODE_ENUM_U32 enMode )
{
    BSP_DDR_SECT_INFO                   stSectInfo;
    BSP_DDR_SECT_QUERY                  stSectQuery;
    VOS_INT                             lRet;
    VOS_UINT32                          ulLenByte;

    stSectQuery.enSectType = BSP_DDR_SECT_TYPE_BBPSAMPLE;

    /* ����DRV �ӿڻ�ȡ�ڴ��ַ�Ĵ�С */
    lRet = DRV_GET_FIX_DDR_ADDR(&stSectQuery, &stSectInfo);
    if(BSP_OK != lRet)
    {
        LogPrint("Om_ConfigBbpDumpEdma: Query Mem Err!\r\n");
        return BBP_DUMP_ALLOC_MEM_ERR;
    }

    *pulPhyAddr = stSectInfo.ulSectPhysAddr;

    ulLenByte = ulLen * 1024;
    if(stSectInfo.ulSectSize < ulLenByte)
    {
         LogPrint1("Om_ConfigBbpDumpEdma: Size Small %d!\r\n", (VOS_INT)stSectInfo.ulSectSize);
         return BBP_DUMP_MEM_BIG;
    }

    lRet = DRV_EDMA_CHANNEL_STOP((VOS_UINT32)g_stBbpDumpInfo.lEdmaChanID);
    if(BSP_ERROR == lRet)
    {
        g_stBbpDumpErrInfo.usStopEdmaErr++;
        LogPrint("Om_ConfigBbpDumpEdma:Stop Edma Fail\r\n");
        return BBP_DUMP_STOP_EDMA_ERR;
    }

    if (OM_BBP_DUMP_MODE_W == enMode)
    {
        /* ʹ��EDMA */
        lRet = DRV_EDMA_CHANNEL_ASYNC_START((VOS_UINT32)g_stBbpDumpInfo.lEdmaChanID, WBBP_DUMP_DMA_RDATA_W_ADDR,
                                            stSectInfo.ulSectPhysAddr, ulLenByte);
    }
    else
    {
        /* ʹ��EDMA */
        lRet = DRV_EDMA_CHANNEL_ASYNC_START((VOS_UINT32)g_stBbpDumpInfo.lEdmaChanID, OAM_GBBP_CPU_GDUMP_FIFO_RD_ADDR,
                                            stSectInfo.ulSectPhysAddr, ulLenByte);
    }

    if (BSP_OK != lRet)
    {
        g_stBbpDumpErrInfo.usStartEdmaErr++;
        LogPrint("Om_ConfigBbpDumpEdma:Start Edma Fail\r\n");
        return BBP_DUMP_START_EDMA_ERR;
    }

    return BBP_DUMP_OK;
}
VOS_UINT32 Om_CheckBbpDumpPara(APP_OM_BBP_DUMP_SET_STRU *pstPcMsg)
{
    VOS_UINT32                          ulResult;

    /* �ж��·���ģʽ�Ƿ���ȷ */
    if (pstPcMsg->enMode >= OM_BBP_DUMP_MODE_BUTT)
    {
        return BBP_DUMP_MODE_ERR;
    }

        /* ��鳤���Ƿ����Ҫ�� */
    ulResult = Om_CheckDumpLen(pstPcMsg->ulDataLen);
    if(ulResult != BBP_DUMP_OK)
    {
        LogPrint("Om_ConfigBbpDumpEdma: Lenth err!\r\n");
        return ulResult;
    }

    return BBP_DUMP_OK;
}


VOS_VOID Om_ConfigBbpDumpReg(APP_OM_BBP_DUMP_SET_STRU *pstPcMsg)
{
    VOS_UINT32                          ulSizeTemp;

    /* ����BBP��ά�ɲ�ģʽ */
    OM_BBP_REG_SETBITS(BBPCOMM_BBP_SYS_1CONTROL_ADDR, 0, 2, pstPcMsg->enMode);

    if (OM_BBP_DUMP_MODE_W == pstPcMsg->enMode)
    {
        /* ��ǿ�ƹر�BBP���� */
        OM_BBP_REG_SETBITS(WBBP_DUMP_STOP_W_ADDR, 0, 1, 0x1);

        /* ����ģ����ʹ�ܣ�������ʱ���ſأ���������ģʽ����������Ҫ���ô��ź���Ч */
        OM_BBP_REG_SETBITS(WBBP_DUMP_EN_W_ADDR, 0, 1, 0x1);

        /* ����BBP�ſ�ʱ�ӼĴ��� */
        OM_BBP_REG_SETBITS(WBBP_DUMP_CLKGATE_BYPASS_W_ADDR, 0, 3, 0x07);

        /*����ʹ�� */
        OM_BBP_WRITE_REG(WBBP_WT_DBG_EN_ADDR, pstPcMsg->ulWtDbgEnW);

        /* FIFO�����ź� */
        OM_BBP_REG_SETBITS(WBBP_DUMP_FIFO_CLR_W_ADDR, 0, 1, 0x1);

        /* PC���·������ɳ�����KBΪ��λ����������Ҫ����32bit�������� */
        ulSizeTemp = (pstPcMsg->ulDataLen * 1024) / 4;

        /* �ɼ����ݵĳ��ȣ�����Ҫ�ɼ���32bit���ݸ��� */
        OM_BBP_WRITE_REG(WBBP_DUMP_DATA_LENGTH_W_ADDR, ulSizeTemp);

        /* BBP ѡ��տ����ݵĲ���λ�� */
        OM_BBP_WRITE_REG(WBBP_DBG_AGC_DATA_SEL_ADDR, pstPcMsg->ulDataModelSelW);

        /* BBP ������ʼλ�� */
        OM_BBP_WRITE_REG(WBBP_DUMP_START_TIME_W_ADDR, pstPcMsg->ulStartTimeW);

        /* ˮ����� */
        OM_BBP_REG_SETBITS(WBBP_DUMP_FIFO_WATERMARKER_W_ADDR, 0, 3, pstPcMsg->enFifoWaterW);

        /* �����ݲɼ�ֹͣ�Ĵ��� */
        OM_BBP_REG_SETBITS(WBBP_DUMP_STOP_W_ADDR, 0, 1, 0x0);

        /* ����������ݲɼ� */
        OM_BBP_REG_SETBITS(WBBP_DUMP_START_W_ADDR, 0, 1, 0x1);
    }
    else
    {
        /* ��ǿ�ƹر�BBP���� */
        OM_BBP_REG_SETBITS(OAM_GBBP_CPU_GDUMP_RECV_CONFIG_ADDR, 8, 1, 0x1);

        /* CPU�����첽FIFO�����ź� */
        OM_BBP_REG_SETBITS(OAM_GBBP_CPU_GDUMP_RECV_CONFIG_ADDR, 16, 1, 0x1);

        /* FIFOˮ�����ѡ��,��������Դѡ��,����ģ����������ѡ�� */
        OM_BBP_WRITE_REG(OAM_GBBP_CPU_GDUMP_GEN_CONFIG_ADDR, pstPcMsg->ulGenConfig);

        /* ѡ��ʹ��CPU���ýӿ� */
        OM_BBP_REG_SETBITS(OAM_GBBP_CPU_GDUMP_GEN_CONFIG_ADDR, 8, 1, 0x1);

        /* ����ģ����ʹ�� */
        OM_BBP_REG_SETBITS(OAM_GBBP_CPU_GDUMP_GEN_CONFIG_ADDR, 31, 1, 0x1);

        /* PC���·������ɳ�����KBΪ��λ����������Ҫ����32bit�������� */
        ulSizeTemp = (pstPcMsg->ulDataLen * 1024) / 4;

        /* �ɼ����ݵĳ��ȣ�����Ҫ�ɼ���32bit���ݸ��� */
        OM_BBP_WRITE_REG(OAM_GBBP_CPU_GDUMP_LEN_CONFIG_ADDR, ulSizeTemp);

        /* ������ʱ�̲���ģʽ�£�������ʼλ�� */
        OM_BBP_WRITE_REG(OAM_GBBP_CPU_GDUMP_SAM_LOW_TIMING_ADDR, pstPcMsg->ulStartLowTimeG);

        /* ��������ʱ�̲���ģʽ�£�������ʼλ��*/
        OM_BBP_WRITE_REG(OAM_GBBP_CPU_GDUMP_SAM_HIGH_TIMING_ADDR, pstPcMsg->ulStartHiTimeG);

        /* ����G ģ���� */
        OM_BBP_REG_SETBITS(OAM_GBBP_CPU_GDUMP_RECV_CONFIG_ADDR, 0, 1, 0x1);
    }

    LogPrint("Om_ConfigBbpDumpReg: config BBP success\r\n");

}
VOS_UINT32 Om_ConfigBbpDump(APP_OM_MSG_EX_STRU *pstAppToOmMsg,
                                           VOS_UINT16 usReturnPrimId)
{
    APP_OM_BBP_DUMP_SET_STRU           *pstPcMsg;
    OM_APP_MSG_EX_STRU                 *pstOmToAppMsg;
    OM_APP_BBP_DUMP_EDMA_STRU          *pstResult;
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulLen;
    VOS_UINT32                          ulPhyAddr;
    VOS_UINT16                          usTotalLen;
    OM_BBP_DUMP_ENABLE_STRU             stEnableFlag;

    stEnableFlag.usEnable = OM_BBP_DUMP_DISABLE;

    /*��NV��*/
    if(NV_OK != NV_Read(en_NV_Item_BBP_DUMP_ENABLE,
        (VOS_VOID *)(&stEnableFlag), sizeof(OM_BBP_DUMP_ENABLE_STRU)))
    {
        LogPrint("Om_ConfigBbpDump: Read Nv Fail\r\n");
        OM_SendResult(pstAppToOmMsg->ucFuncType, BBP_DUMP_READ_NV_ERR, usReturnPrimId);
        return VOS_ERR;
    }

    usTotalLen = OM_APP_MSG_EX_LEN + sizeof(OM_APP_BBP_DUMP_EDMA_STRU);

    /* ������Ϣ�ռ� */
    pstOmToAppMsg   = (OM_APP_MSG_EX_STRU*)VOS_MemAlloc(WUEPS_PID_OM, DYNAMIC_MEM_PT, usTotalLen);
    if(VOS_NULL_PTR == pstOmToAppMsg)
    {
        g_stBbpDumpErrInfo.usAllocMsgErr++;
        OM_SendResult(pstAppToOmMsg->ucFuncType, BBP_DUMP_ALLOC_MSG_ERR, usReturnPrimId);
        return VOS_ERR;
    }

    pstOmToAppMsg->usLength = usTotalLen - VOS_OM_HEADER_LEN;
    pstResult               = (OM_APP_BBP_DUMP_EDMA_STRU*)pstOmToAppMsg->aucPara;

    /* �ж��Ƿ�ʹ��BBP���� */
    if (OM_BBP_DUMP_DISABLE == stEnableFlag.usEnable)
    {
        pstResult->enErr = BBP_DUMP_DISABLE;
        LogPrint("Om_ConfigBbpDump: Not Enable BBP DUMP\r\n");
        OM_SendContent(pstAppToOmMsg->ucFuncType, pstOmToAppMsg, usReturnPrimId);
        VOS_MemFree(WUEPS_PID_OM, pstOmToAppMsg);
        return VOS_OK;
    }

    pstPcMsg    = (APP_OM_BBP_DUMP_SET_STRU*)pstAppToOmMsg->aucPara;

    /* �������Ƿ���ȷ */
    ulRet = Om_CheckBbpDumpPara(pstPcMsg);
    if (BBP_DUMP_OK != ulRet)
    {
        pstResult->enErr = ulRet;
        LogPrint1("Om_ConfigBbpDump:para err: %d!\r\n",(VOS_INT)ulRet);
        OM_SendContent(pstAppToOmMsg->ucFuncType, pstOmToAppMsg, usReturnPrimId);
        VOS_MemFree(WUEPS_PID_OM, pstOmToAppMsg);
        return VOS_ERR;
    }

    ulLen       = pstPcMsg->ulDataLen;
    ulRet       = Om_ConfigBbpDumpEdma(ulLen, &ulPhyAddr, pstPcMsg->enMode);
    if(BBP_DUMP_OK != ulRet)
    {
        pstResult->enErr = ulRet;
        LogPrint("Om_ConfigBbpDump:Config Edma fail!\r\n");
        OM_SendContent(pstAppToOmMsg->ucFuncType, pstOmToAppMsg, usReturnPrimId);
        VOS_MemFree(WUEPS_PID_OM, pstOmToAppMsg);
        return VOS_ERR;
    }

    /*����BBP�Ĵ��� */
    Om_ConfigBbpDumpReg(pstPcMsg);

    /* ��BBP ��ʼ������־ */
    g_stBbpDumpInfo.usDUMPStatus = OM_BBP_START;
    g_stBbpDumpInfo.usDUMPMode   = (VOS_UINT16)pstPcMsg->enMode;
    pstResult->enErr             = BBP_DUMP_OK;
    pstResult->ulPhyAddr         = ulPhyAddr;

    OM_SendContent(pstAppToOmMsg->ucFuncType, pstOmToAppMsg, usReturnPrimId);
    VOS_MemFree(WUEPS_PID_OM, pstOmToAppMsg);

    return VOS_OK;

}


VOS_UINT32 Om_EndBbpDump(APP_OM_MSG_EX_STRU *pstAppToOmMsg,
                                           VOS_UINT16 usReturnPrimId)
{
    VOS_INT                             lRet;

    /* ����DRV�����ر�EDMA,����ر�EDMAʧ�ܣ���ȫ��û��Ӱ�죬���Բ��÷��� */
    lRet = DRV_EDMA_CHANNEL_STOP((VOS_UINT32)g_stBbpDumpInfo.lEdmaChanID);
    if (BSP_ERROR == lRet)
    {
        LogPrint1("Om_EndBbpDump: Close EDMA: %d  fail\r\n", g_stBbpDumpInfo.lEdmaChanID);
        g_stBbpDumpErrInfo.usCloseEdmaErr++;
    }

    LogPrint("Om_EndBbpDump:Send Msg To PC\r\n");

    OM_SendResult(pstAppToOmMsg->ucFuncType, BBP_DUMP_END, usReturnPrimId);

    return VOS_OK;
}
VOS_VOID Om_EndBbpDbg(VOS_VOID)
{
    if ( OM_BBP_DBG_START == g_stBbpDbgInfo.ulBbpDbgStatus)
    {
        /* ��DBG���ź� */
        OM_BBP_REG_SETBITS(WBBP_BBP_DBG_START_ADDR, 0, 1, 0x0);

        /* DBGֹͣ�ź� */
        OM_BBP_REG_SETBITS(WBBP_BBP_DBG_STOP_ADDR, 0, 1, 0x1);

        DRV_EDMA_CHANNEL_STOP((VOS_UINT32)g_stBbpDbgInfo.lBbpEdmaChanID);

        OM_BBP_READ_REG(g_stBbpDbgInfo.ulIndexValue,WBBP_DDR_BIT_CNT_ADDR);

        g_stBbpDbgInfo.ulBbpDbgStatus = OM_BBP_DBG_STOP;
    }

    return;
}


VOS_UINT32 Om_CheckBbpDbgPara(APP_OM_BBP_DBG_SET_W_STRU *pstPcMsg)
{
    /* �ж�BBP�ӿ�ѡ���ź��Ƿ���ȷ */
    if (pstPcMsg->enBBPIfSel >= BBP_IF_SEL_BUTT)
    {
        return BBP_DBG_IF_SEL_ERR;
    }

    /* �ж�ʱ��ѡ����Ϣ�Ƿ���ȷ */
    if (pstPcMsg->enClkInSel >= BBP_DBG_CLK_IN_SEL_BUTT)
    {
        return BBP_DBG_CLK_IN_SEL_ERR;
    }

    /* �ж��·���ģʽ�Ƿ���ȷ */
    if (pstPcMsg->enMode >= OM_BBP_DUMP_MODE_BUTT)
    {
        return BBP_DBG_MODE_ERR;
    }

    if ((pstPcMsg->ulMaskAddrEnd0 > MAX_BBP_OFFSET_ADDR )
        || (pstPcMsg->ulMaskAddrEnd1 > MAX_BBP_OFFSET_ADDR)
        || (pstPcMsg->ulMaskAddrEnd2 > MAX_BBP_OFFSET_ADDR)
        || (pstPcMsg->ulMaskAddrEnd3 > MAX_BBP_OFFSET_ADDR))
    {
        return BBP_DBG_ADDR_ERR;
    }

    /* �жϵ�ַ��Χ�Ƿ���ȷ */
    if ((pstPcMsg->ulMaskAddrStart0 > pstPcMsg->ulMaskAddrEnd0)
        || (pstPcMsg->ulMaskAddrStart1 > pstPcMsg->ulMaskAddrEnd1)
        || (pstPcMsg->ulMaskAddrStart2 > pstPcMsg->ulMaskAddrEnd2)
        || (pstPcMsg->ulMaskAddrStart3 > pstPcMsg->ulMaskAddrEnd3))
    {
        return BBP_DBG_ADDR_ERR;
    }

    return BBP_DBG_OK;
}


VOS_UINT32 Om_EnableDBGEdma(VOS_VOID)
{
    VOS_INT                             lRet;

    /* �ر�EDMA */
    lRet = DRV_EDMA_CHANNEL_STOP((VOS_UINT32)g_stBbpDbgInfo.lBbpEdmaChanID);
    if(BSP_ERROR == lRet)
    {
        g_stBbpDbgErrInfo.usStopEdmaErrNum++;;
        return BBP_DBG_STOP_EDMA_ERR;
    }

    /* ����DRV ����ʹ��EDMA*/
    lRet = DRV_EDMA_CHANNEL_lli_ASYNC_START((VOS_UINT32)g_stBbpDbgInfo.lBbpEdmaChanID);
    if (BSP_OK != lRet)
    {
        g_stBbpDbgErrInfo.usStartEdmaErrNum++;
        return BBP_DBG_ENABLE_EDMA_ERR;
    }

    return BBP_DBG_OK;

}



VOS_UINT32 Om_ConfigBbpDbgReg(APP_OM_BBP_DBG_SET_W_STRU *pstPcMsg)
{
    VOS_UINT32                          ulValue;
    VOS_UINT32                          ulResult;

    /* ���PC���·���������Ϣ�Ƿ���ȷ */
    ulResult = Om_CheckBbpDbgPara(pstPcMsg);
    if (BBP_DBG_OK != ulResult)
    {
        return ulResult;
    }

    /* DBG�ϱ�ʹ�ܣ�1��ʾ���ϱ����ܣ�0��ʾ�ر��ϱ����ܡ����ʱ���ſ� */
    OM_BBP_REG_SETBITS(WBBP_BBP_DBG_EN_ADDR, 0, 1, 0x1);

    /* ʱ��bypassʹ�� */
    OM_BBP_REG_SETBITS(WBBP_BBP_DBG_BYPASS_ADDR, 0, 10, 0x3FF);

    /* ��ǿ�ƹرտ�ά�ɲ���ź� */
    OM_BBP_REG_SETBITS(WBBP_BBP_DBG_STOP_ADDR, 0, 1, 0x1);

    /* ����BBP��ά�ɲ�ģʽ */
    OM_BBP_REG_SETBITS(BBPCOMM_BBP_SYS_1CONTROL_ADDR, 0, 2, pstPcMsg->enMode);

    /* ��FIFO */
    OM_BBP_REG_SETBITS(WBBP_DBG_FLOW_CLR_ADDR, 0, 9, 0x1FF);
    OM_BBP_REG_SETBITS(WBBP_DBG_FLOW_CLR_ADDR, 16, 1, 0x1);

    /* BBP�ӿ�ѡ���ź� */
    OM_BBP_REG_SETBITS(WBBP_BBP_IF_SEL_ADDR, 0, 2, pstPcMsg->enBBPIfSel);

    /* DDR��DBG�ռ��С��2KbyteΪ��λ��������Ϊ8��ʾDBG�ռ�Ϊ8��2Kbyte */
    OM_BBP_REG_SETBITS(WBBP_DBG_DDR_2K_NO_ADDR, 0, 4, 0x01);

    /* ��ά�ɲ�����ʱ��ѡ�� */
    OM_BBP_REG_SETBITS(WBBP_DBG_CLK_IN_SEL_ADDR, 0, 2, pstPcMsg->enClkInSel);

    /* ������õİ�ͷ��reserve��Ϣ */
    OM_BBP_REG_SETBITS(WBBP_DBG_HEAD_SERVE_ADDR, 0, 4, OM_TOOL_GUBBP_ID);

    /* ʹ��ʱ���ѡ�� */
    ulValue     = pstPcMsg->ulTimeSel & 0x7F;
    OM_BBP_REG_SETBITS(WBBP_DBG_TIMING_SEL_ADDR, 0, 6, ulValue);

    /* ���ε�ַ����  */
    ulValue     = pstPcMsg->ulMaskAddrStart0 & 0xFFFFF;
    OM_BBP_REG_SETBITS(WBBP_MASK_ADDR0_START_ADDR, 0, 20, ulValue);

    ulValue     = pstPcMsg->ulMaskAddrStart1 & 0xFFFFF;
    OM_BBP_REG_SETBITS(WBBP_MASK_ADDR1_START_ADDR, 0, 20, ulValue);

    ulValue     = pstPcMsg->ulMaskAddrStart2 & 0xFFFFF;
    OM_BBP_REG_SETBITS(WBBP_MASK_ADDR2_START_ADDR, 0, 20, ulValue);

    ulValue     = pstPcMsg->ulMaskAddrStart3 & 0xFFFFF;
    OM_BBP_REG_SETBITS(WBBP_MASK_ADDR3_START_ADDR, 0, 20, ulValue);

    ulValue     = pstPcMsg->ulMaskAddrEnd0 & 0xFFFFF;
    OM_BBP_REG_SETBITS(WBBP_MASK_ADDR0_END_ADDR, 0, 20, ulValue);

    ulValue     = pstPcMsg->ulMaskAddrEnd1 & 0xFFFFF;
    OM_BBP_REG_SETBITS(WBBP_MASK_ADDR1_END_ADDR, 0, 20, ulValue);

    ulValue     = pstPcMsg->ulMaskAddrEnd2 & 0xFFFFF;
    OM_BBP_REG_SETBITS(WBBP_MASK_ADDR2_END_ADDR, 0, 20, ulValue);

    ulValue     = pstPcMsg->ulMaskAddrEnd3 & 0xFFFFF;
    OM_BBP_REG_SETBITS(WBBP_MASK_ADDR3_END_ADDR, 0, 20, ulValue);

    /* ��ά�ɲ� FIFOˮ��ѡ�� */
    ulValue     = pstPcMsg->enFifoWater & 0x7;
    OM_BBP_REG_SETBITS(WBBP_DBG_FIFO_WATERMARKER_ADDR, 0, 3, ulValue);

    /* ��ά�ɲ��ϱ���Ϣ���أ�1��ʾ�ϱ���Ӧ��Ϣ��0��ʾ���ϱ���Ӧ��Ϣ */
    ulValue     = pstPcMsg->ulDbgSwitch & 0x1F;
    OM_BBP_REG_SETBITS(WBBP_BBP_DBG_SWITCH_ADDR, 0, 5, ulValue);

    /* ���ά�ɲ�ֹͣ�ź� */
    OM_BBP_REG_SETBITS(WBBP_BBP_DBG_STOP_ADDR, 0, 1, 0x0);

    /* ��ά�ɲ���ź� */
    OM_BBP_REG_SETBITS(WBBP_BBP_DBG_START_ADDR, 0, 1, 0x1);

    LogPrint("Om_ConfigBbpDbgReg: Config Reg success\r\n");

    return BBP_DBG_OK;
}


VOS_UINT32 Om_ConfigBbpDbg(APP_OM_MSG_EX_STRU *pstAppToOmMsg,
                                           VOS_UINT16 usReturnPrimId)
{
    VOS_UINT32                          ulRet;
    APP_OM_BBP_DBG_SET_W_STRU          *pstPcMsg;

    pstPcMsg        = (APP_OM_BBP_DBG_SET_W_STRU *)pstAppToOmMsg->aucPara;

    /* ����ʼ��EDMA��SOCP�Ƿ�ɹ� */
    if (BBP_DBG_OK != g_stBbpDbgInfo.ulBbpErrCode)
    {
        OM_SendResult(pstAppToOmMsg->ucFuncType, g_stBbpDbgInfo.ulBbpErrCode, usReturnPrimId);
        return VOS_ERR;
    }

    /* �����·�ֹͣ BBP�Ĵ����ϱ� */
    if(OM_BBP_STOP == pstPcMsg->enStatus)
    {
        Om_EndBbpDbg();
        OM_SendResult(pstAppToOmMsg->ucFuncType, BBP_DBG_OK, usReturnPrimId);
        return VOS_OK;
    }

    /* ʹ�� EDMA */
    ulRet = Om_EnableDBGEdma();
    if(BBP_DBG_OK != ulRet)
    {
        LogPrint1("Om_ConfigBbpDbg: Enable EDMA Fail %d\r\n", (VOS_INT)ulRet);
        OM_SendResult(pstAppToOmMsg->ucFuncType, ulRet, usReturnPrimId);
        return ulRet;
    }

    /* ����BBP�Ĵ��� */
    ulRet = Om_ConfigBbpDbgReg(pstPcMsg);
    if(BBP_DBG_OK != ulRet)
    {
        LogPrint1("Om_ConfigBbpDbg: Config Reg Fail %d\r\n", (VOS_INT)ulRet);
        OM_SendResult(pstAppToOmMsg->ucFuncType, ulRet, usReturnPrimId);
        return ulRet;
    }

    OM_SendResult(pstAppToOmMsg->ucFuncType, BBP_DBG_OK, usReturnPrimId);

    g_stBbpDbgInfo.ulBbpDbgStatus  = OM_BBP_DBG_START;

    return VOS_OK;

}


VOS_UINT32 Om_BbpDbgAllocCycBuf(VOS_VOID)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulVirBufAddr;
    VOS_UINT32                          ulPhyBufAddr;
    BALONG_DMA_CB                      *pstCycBuf;
    BALONG_DMA_CB                      *pstCycBufTemp;


    /* ����ռ� */
    ulVirBufAddr = (VOS_UINT32)VOS_UnCacheMemAlloc(BBP_DBG_TOTAL_MEM_LENTH, &ulPhyBufAddr);
    if (VOS_NULL == ulVirBufAddr)
    {
        LogPrint("Om_BbpDbgAllocMem: Alloc Mem err!\r\n");
        g_stBbpDbgErrInfo.usAllocErr++;
        g_stBbpDbgInfo.ulBbpErrCode = BBP_DBG_ALLOC_ERR;
        return VOS_ERR;
    }

    g_stBbpDbgInfo.ulPhyAddr = ulPhyBufAddr;

    /* ����SOCPͨ������Դͨ���������ַ�����ַ */
    for (i = 0; i < BBP_DBG_CODER_SRC_NUM; i++)
    {
        g_astBbpDbgCoderSrcCfg[i].pucSrcBuf = (VOS_UINT8*)ulVirBufAddr + BBP_DBG_CODER_SRC_SIZE * i;
        g_astBbpDbgCoderSrcCfg[i].pucSrcPHY = (VOS_UINT8*)ulPhyBufAddr + BBP_DBG_CODER_SRC_SIZE * i;
    }


    /* Ϊ��֤256bit���룬�����32 byte */
    pstCycBuf = (BALONG_DMA_CB*)VOS_MemAlloc(WUEPS_PID_OM, DYNAMIC_MEM_PT,
                        (BBP_DBG_CODER_SRC_NUM * BBP_DBG_NODE_NUM * sizeof(BALONG_DMA_CB) + 32));
    if (VOS_NULL_PTR == pstCycBuf)
    {
        g_stBbpDbgErrInfo.usAllocCycErr++;
        g_stBbpDbgInfo.ulBbpErrCode = BBP_DBG_ALLOC_CYC_ERR;
        LogPrint("Om_BbpDbgAllocCycBuf: alloc BALONG_DMA_CB fail\r\n");
        return VOS_ERR;
    }

    /* 256 bit���� */
    pstCycBuf =(BALONG_DMA_CB*)(((VOS_UINT32)pstCycBuf + 31)  & ~ 31);

    pstCycBufTemp = (BALONG_DMA_CB*)pstCycBuf;
    g_pstBbpCycBufHead = pstCycBuf;

    /* ����ѭ������ */
    for (i = 0; i < BBP_DBG_CODER_SRC_NUM * BBP_DBG_NODE_NUM; i++)
    {
        pstCycBufTemp->cnt0     = BBP_DBG_NODE_SIZE;
        pstCycBufTemp->cnt1     = 0;
        pstCycBufTemp->bindx    = 0;
        pstCycBufTemp->cindx    = 0;
        pstCycBufTemp->src_addr = (VOS_UINT32)WBBP_BBP_DBG_RDATA_ADDR;
        pstCycBufTemp->des_addr = ulPhyBufAddr
                                    + BBP_DBG_NODE_SIZE
                                    * ((i%BBP_DBG_CODER_SRC_NUM) * BBP_DBG_NODE_NUM
                                    + (i / BBP_DBG_CODER_SRC_NUM));
        pstCycBufTemp->config   = BALONG_DMA_SET_CONFIG(EDMA_BBP_DBG, BALONG_DMA_P2M, EDMA_TRANS_WIDTH_32, EDMA_BUR_LEN_16);
        if(0 == pstCycBufTemp->config)
        {
            LogPrint("Om_BbpDbgAllocCycBuf: Config EDMA fail\r\n");
            g_stBbpDbgErrInfo.usConfigEdmaErr++;
            g_stBbpDbgInfo.ulBbpErrCode = BBP_DBG_CONFIG_EDMA_ERR;
            return VOS_ERR;
        }

        /* ��������һ����㣬�˳� */
        if ((BBP_DBG_CODER_SRC_NUM * BBP_DBG_NODE_NUM - 1) == i)
        {
            break;
        }
        pstCycBufTemp->lli = (((VOS_UINT32)(pstCycBufTemp + 1)) & 0xFFFFFFE0) | 0x02;
        pstCycBufTemp++;
    }

    pstCycBufTemp->lli = (((VOS_UINT32)g_pstBbpCycBufHead) & 0xFFFFFFE0) | 0x02;

    /* ��һ����㣬�轫config��bit0 ��Ϊ0 */
    pstCycBufTemp = g_pstBbpCycBufHead;
    pstCycBufTemp->config &= 0xFFFFFFFE;

    return VOS_OK;

}
VOS_UINT32 Om_BbpDbgEdmaCallBack(VOS_UINT32 ulChannelArg, VOS_UINT32 ulFlag)
{
    OM_REQ_PACKET_STRU                 *pstPcMsg;
    BBP_EDMA_ERR_STRU                  *pstEdmaErr;
    VOS_UINT32                          ulIndex;
    VOS_UINT32                          ulDbgSwitchValue;

    /* DMA��������ж� */
    if (BALONG_DMA_INT_LLT_DONE == ulFlag)
    {
        ulIndex = g_stBbpDbgInfo.ulBbpEdmaCallBackNum% 8;
        g_stBbpDbgInfo.ulLastAddr = (VOS_UINT32)(g_stBbpDbgInfo.ulPhyAddr
                + BBP_DBG_NODE_SIZE * ((ulIndex % BBP_DBG_CODER_SRC_NUM) * BBP_DBG_NODE_NUM
                + (ulIndex / BBP_DBG_CODER_SRC_NUM)));
        if(BBP_DBG_MAGIC_SIGN != *((VOS_UINT32*)g_stBbpDbgInfo.ulLastAddr))
        {
            *((VOS_UINT32*)g_stBbpDbgInfo.ulLastAddr) = (VOS_UINT32)BBP_DBG_MAGIC_SIGN;
            *(((VOS_UINT32*)g_stBbpDbgInfo.ulLastAddr) + 1) = (VOS_UINT32)BBP_DBG_PACKAGE_LENTH;
            *(((VOS_UINT32*)g_stBbpDbgInfo.ulLastAddr) + 2) = (VOS_UINT32)BBP_DBG_ERR_PACKAGE_SIGN;

            /* ֹͣ�ɸ���ά�ɲ� */
            OM_BBP_REG_SETBITS(WBBP_BBP_DBG_STOP_ADDR, 0, 1, 0x1);
            OM_BBP_READ_REG(ulDbgSwitchValue,WBBP_BBP_DBG_SWITCH_ADDR);

            /* ��FIFO */
            OM_BBP_REG_SETBITS(WBBP_DBG_FLOW_CLR_ADDR, 0, 9, 0x1FF);
            OM_BBP_REG_SETBITS(WBBP_DBG_FLOW_CLR_ADDR, 16, 1, 0x1);

            /* BBP��Ҫһ����ƽת���������FIFO */
            OM_BBP_WRITE_REG(WBBP_BBP_DBG_SWITCH_ADDR,ulDbgSwitchValue);

            /* ��ѯBBP�ܰ��� */
            OM_BBP_READ_REG(g_stBbpDbgInfo.ulIndexValue,WBBP_DDR_BIT_CNT_ADDR);

            /* ��������EDMA */
            Om_EnableDBGEdma();

            /* ���¿�����ά�ɲ� */
            OM_BBP_REG_SETBITS(WBBP_BBP_DBG_START_ADDR, 0, 1, 0x1);

            g_stBbpDbgInfo.ulFlowNum++;

        }

        VOS_SmV(g_ulBbpDbgSem);
        g_stBbpDbgInfo.ulBbpEdmaCallBackNum++;
    }
    /* EDMA�����ж� */
    else
    {
        if (VOS_NULL_PTR == g_pstBbpDbgMsg)
        {
            return VOS_ERR;
        }

        pstPcMsg = (OM_REQ_PACKET_STRU*)g_pstBbpDbgMsg->aucValue;
        pstEdmaErr = (BBP_EDMA_ERR_STRU*)pstPcMsg->aucCommand;

        pstPcMsg->ucModuleId  = OM_BBP_FUNC;
        pstEdmaErr->usMsgName = (VOS_UINT16)APP_OM_BBP_EDMA_ERR_REQ;

        /* ��ʶ�ǿ�ά�ɲ������EDMA�����ж� */
        pstEdmaErr->enEdmaErr = OM_BBP_DBG_EDMA_ERR;

        /* ģ��PC��CCPU OM������Ϣ */
        (VOS_VOID)VOS_SendMsg(UEPS_PID_OMRL, g_pstBbpDbgMsg);

        g_stBbpDbgErrInfo.usEdmaErrCode = (VOS_UINT16)ulFlag;
    }

    return VOS_OK;

}



VOS_VOID Om_BbpDbgChanInit(VOS_VOID)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulRet;
    OM_REQ_PACKET_STRU                 *pstPcMsg;
    SOCP_CODER_SRC_CHAN_S               stChannel;   /* ͨ����������Ϣ */
    BALONG_DMA_CB                      *pstConfigAddr;

    /* ��ʼ��ȫ�ֱ��� */
    VOS_MemSet(&g_stBbpDbgErrInfo,  0, sizeof(g_stBbpDbgErrInfo));
    VOS_MemSet(&g_stBbpDbgInfo,     0, sizeof(g_stBbpDbgInfo));

    /* �ÿ�ά�ɲ�״̬��־ */
    g_stBbpDbgInfo.ulBbpDbgStatus = OM_BBP_DBG_STOP;

    /*��ʼ��BBP ��ά�ɲ�ʹ�õ�EDMAͨ�� */
    g_stBbpDbgInfo.lBbpEdmaChanID = DRV_EDMA_CHANNEL_INIT(EDMA_BBP_DBG,
                                    (channel_isr)Om_BbpDbgEdmaCallBack,
                                    VOS_NULL, BBP_DBG_EDMA_INT);
    if (g_stBbpDbgInfo.lBbpEdmaChanID < BSP_OK)
    {
        g_stBbpDbgErrInfo.usInitEdmaErr++;
        g_stBbpDbgInfo.ulBbpErrCode = BBP_DBG_INIT_EDMA_ERR;
        return;
    }

    /* ����ռ䣬�γ�ѭ������ */
    ulRet =  Om_BbpDbgAllocCycBuf();
    if (VOS_ERR == ulRet)
    {
        LogPrint("Om_BbpDbgChanInit: Alloc Mem Error\r\n");
        return ;
    }

    /* ��ȡ��ά�ɲ�EDMAͨ��ѭ���������õ�ַ */
    pstConfigAddr = DRV_EDMA_CHAN_GET_LLI_ADDR((VOS_UINT32)g_stBbpDbgInfo.lBbpEdmaChanID);
    if (VOS_NULL_PTR == pstConfigAddr)
    {
        g_stBbpDbgErrInfo.usGetConAddrErr++;
        g_stBbpDbgInfo.ulBbpErrCode = BBP_DBG_GET_CONFIGADD_ERR;
        LogPrint("Om_BbpDbgChanInit: get lli config add fail\r\n");
        return ;
    }

    /* ��������Ϣд��EDMA���õ�ַ */
    VOS_MemCpy(pstConfigAddr, g_pstBbpCycBufHead, sizeof(BALONG_DMA_CB));

    for (i = 0; i < BBP_DBG_CODER_SRC_NUM; i++)
    {
        VOS_MemSet(&stChannel, 0, sizeof(stChannel));
        stChannel.u32DestChanID = (BSP_U32)g_astBbpDbgCoderSrcCfg[i].enDstCHID;   /*  Ŀ��ͨ��ID */
        stChannel.eDataType     = g_astBbpDbgCoderSrcCfg[i].enDataType;           /*  �������ͣ�ָ�����ݷ�װЭ�飬���ڸ��ö�ƽ̨ */
        stChannel.eMode         = g_astBbpDbgCoderSrcCfg[i].enCHMode;             /*  ͨ������ģʽ */
        stChannel.ePriority     = g_astBbpDbgCoderSrcCfg[i].enCHLevel;            /*  ͨ�����ȼ� */
        stChannel.u32BypassEn   = SOCP_HDLC_ENABLE;                               /*  ͨ��bypassʹ�� */
        stChannel.eDataTypeEn   = SOCP_DATA_TYPE_EN;                              /*  ��������ʹ��λ */
        stChannel.eDebugEn      = SOCP_ENC_DEBUG_DIS;                             /*  ����λʹ�� */

        stChannel.sCoderSetSrcBuf.u32InputStart  = (VOS_UINT32)g_astBbpDbgCoderSrcCfg[i].pucSrcPHY;                               /*  ����ͨ����ʼ��ַ */
        stChannel.sCoderSetSrcBuf.u32InputEnd    = (VOS_UINT32)((g_astBbpDbgCoderSrcCfg[i].pucSrcPHY + g_astBbpDbgCoderSrcCfg[i].ulSrcBufLen)-1);   /*  ����ͨ��������ַ */

        /* ����ͨ�� */
        if (VOS_OK != BSP_SOCP_CoderSetSrcChan(g_astBbpDbgCoderSrcCfg[i].enChannelID, &stChannel))
        {
            g_astBbpDbgCoderSrcCfg[i].enInitState = SCM_CHANNEL_CFG_FAIL;/* ��¼ͨ���������ô��� */
            g_stBbpDbgErrInfo.usInitSocpErr++;
            g_stBbpDbgInfo.ulBbpErrCode = BBP_DBG_INIT_SOCP_ERR;

            return;
        }

        /* ʹ��ͨ�� */
        if(VOS_OK != BSP_SOCP_Start(g_astBbpDbgCoderSrcCfg[i].enChannelID))
        {
            g_astBbpDbgCoderSrcCfg[i].enInitState = SCM_CHANNEL_START_FAIL;  /* ��¼ͨ���������� */
            g_stBbpDbgErrInfo.usStartSocpErr++;
            g_stBbpDbgInfo.ulBbpErrCode = BBP_DBG_START_SOCP_ERR;

            return;
        }

        g_astBbpDbgCoderSrcCfg[i].enInitState = SCM_CHANNEL_INIT_SUCC; /* ��¼ͨ����ʼ��������Ϣ */
    }

    /*�����ź���*/
    if ( VOS_OK != VOS_SmCCreate("BbpD", 0, VOS_SEMA4_FIFO, &g_ulBbpDbgSem))
    {
        g_stBbpDbgErrInfo.usCreateSemErr++;
        g_stBbpDbgInfo.ulBbpErrCode = BBP_DBG_CREATE_SEM_ERR;
        return;
    }

    /* ����һ��������Ϣ�������ж��з��� */
    g_pstBbpDbgMsg = (MsgBlock*)VOS_AllocMsg(UEPS_PID_OMRL, sizeof(OM_REQ_PACKET_STRU));
    if (VOS_NULL_PTR == g_pstBbpDbgMsg)
    {
        g_stBbpDumpErrInfo.usAllocMsgErr++;
        g_stBbpDbgInfo.ulBbpErrCode = BBP_DBG_ALLOC_MSG_ERR;
        return;
    }

    pstPcMsg = (OM_REQ_PACKET_STRU*)g_pstBbpDbgMsg->aucValue;

    pstPcMsg->ucModuleId = OM_BBP_FUNC;

    g_pstBbpDbgMsg->ulReceiverPid = WUEPS_PID_OM;

    /* ��Ǹ���Ϣ�����ͷ� */
    VOS_ReserveMsg(UEPS_PID_OMRL,g_pstBbpDbgMsg);
    return ;
}


VOS_VOID Om_BbpDbgSelfTask(unsigned long ulPara1, unsigned long ulPara2,
                  unsigned long ulPara3, unsigned long ulPara4)
{
    VOS_UINT32                          ulIndex;
    VOS_UINT32                          ulRet;
    SOCP_BUFFER_RW_S                    stRwBuf;

#if (VOS_WIN32 == VOS_OS_VER)
    VOS_UINT32                          i;
    for (i = 0;i < 1; i++)
#else

    for (;;)
#endif
    {
        if(VOS_OK != VOS_SmP(g_ulBbpDbgSem, 0))
        {
            continue;
        }

        /* ��������ж�Ӧ�ô����ͨ�� */
        ulIndex = g_stBbpDbgInfo.ulBbpSelfTaskNum & (BBP_DBG_CODER_SRC_NUM -1);

        /* ��ȡͨ���ı���Դ�ռ� */
        if (VOS_OK != BSP_SOCP_GetWriteBuff(g_astBbpDbgCoderSrcCfg[ulIndex].enChannelID, &stRwBuf))
        {
            g_stBbpDbgErrInfo.astBbpDbgSocpInfo[ulIndex].ulGetWriteBuffErr++;
            continue;
        }

        /* �жϿռ��Ƿ��㹻 */
        if ((stRwBuf.u32RbSize + stRwBuf.u32Size) < BBP_DBG_NODE_SIZE)
        {
            g_stBbpDbgErrInfo.astBbpDbgSocpInfo[ulIndex].ulBuffNotEnough++;
            g_stBbpDbgErrInfo.astBbpDbgSocpInfo[ulIndex].ulWriteBufSize = stRwBuf.u32RbSize + stRwBuf.u32Size;
            continue;
        }

        ulRet = BSP_SOCP_WriteDone(g_astBbpDbgCoderSrcCfg[ulIndex].enChannelID, BBP_DBG_NODE_SIZE );
        if (VOS_OK != ulRet)
        {
            g_stBbpDbgErrInfo.astBbpDbgSocpInfo[ulIndex].ulWriteDoneErr++;

            continue;
        }

        g_stBbpDbgErrInfo.astBbpDbgSocpInfo[ulIndex].ulWriteDoneOk++;

        /* �ص������� 1 */
        g_stBbpDbgInfo.ulBbpSelfTaskNum++;

    }
}


VOS_VOID Om_CcpuBbpConfigMsgProc(OM_REQ_PACKET_STRU *pRspPacket, OM_RSP_FUNC *pRspFuncPtr)
{
    APP_OM_MSG_EX_STRU                 *pstAppToOmMsg;
    VOS_UINT32                          ulIndex;
    VOS_UINT32                          ulTotalNum;
    VOS_UINT16                          usPrimId;
    VOS_UINT16                          usReturnPrimId;
    VOS_UINT32                          ulResult = VOS_ERR;

    pstAppToOmMsg   = (APP_OM_MSG_EX_STRU*)pRspPacket;
    ulTotalNum      = sizeof(g_astOmBbpMsgFunTbl) / sizeof(OM_MSG_FUN_STRU);
    usPrimId = pstAppToOmMsg->usPrimId;
    /* ��ѯ������ */
    for (ulIndex = 0; ulIndex < ulTotalNum; ulIndex++)
    {
        if (usPrimId == (VOS_UINT16)(g_astOmBbpMsgFunTbl[ulIndex].ulPrimId))
        {
            usReturnPrimId = (VOS_UINT16)(g_astOmBbpMsgFunTbl[ulIndex].ulReturnPrimId);
            ulResult = g_astOmBbpMsgFunTbl[ulIndex].pfFun(pstAppToOmMsg, usReturnPrimId);
            break;
        }
    }

    /* û�в�ѯ��������  */
    if (ulIndex == ulTotalNum)
    {
        OM_SendResult(pstAppToOmMsg->ucFuncType, ulResult, usPrimId);
    }
    return;

}


VOS_VOID Om_BbpDbgShow(VOS_VOID)
{
    vos_printf("��ά�ɲ�EDMAͨ����:             %d\r\n",g_stBbpDbgInfo.lBbpEdmaChanID);
    vos_printf("BBP��ά�ɲ�EDMA ��ɻص�����:   %d\r\n",g_stBbpDbgInfo.ulBbpEdmaCallBackNum);
    vos_printf("�Դ���������Ĵ���:           %d\r\n", g_stBbpDbgInfo.ulBbpSelfTaskNum);
    vos_printf("BBP��ά�ɲ��ʼ��������:        %d\r\n",g_stBbpDbgInfo.ulBbpErrCode);
    vos_printf("�������:                       0x%x\r\n", g_stBbpDbgInfo.ulFlowNum);
    vos_printf("�����ڴ�������ַ :            0x%x\r\n",g_stBbpDbgInfo.ulPhyAddr);
    vos_printf("BBP�з�����2K���ݵ�����:        0x%x\r\n",g_stBbpDbgInfo.ulIndexValue);
    vos_printf("ǰһ�����ɵ�ַ g_ulUpAddr:      0x%x\r\n", g_stBbpDbgInfo.ulLastAddr);
}


VOS_VOID Om_BbpDbgErrShow(VOS_VOID)
{
    VOS_UINT32                          i;
    vos_printf("�ڴ����ʧ�ܴ���:       %d \r\n",g_stBbpDbgErrInfo.usAllocErr);
    vos_printf("SOCP��ʼ��ʧ�ܴ���:     %d \r\n",g_stBbpDbgErrInfo.usInitSocpErr);
    vos_printf("����SOCPʧ�ܴ���:       %d \r\n",g_stBbpDbgErrInfo.usStartSocpErr);
    vos_printf("��ʼ��EDMAʧ�ܴ���:     %d \r\n",g_stBbpDbgErrInfo.usInitEdmaErr);
    vos_printf("����EDMAʧ�ܴ���:       %d \r\n",g_stBbpDbgErrInfo.usConfigEdmaErr);
    vos_printf("EDMAʧ�ܻص�����:       %d \r\n",g_stBbpDbgErrInfo.usEdmaErrCode);
    vos_printf("����EDMAʧ�ܵĴ���:     %d \r\n",g_stBbpDbgErrInfo.usStartEdmaErrNum);
    vos_printf("�ر�EDMAʧ�ܵĴ���:     %d \r\n",g_stBbpDbgErrInfo.usStopEdmaErrNum);
    vos_printf("����ѭ������ڵ�ʧ��:   %d \r\n",g_stBbpDbgErrInfo.usAllocCycErr);
    vos_printf("��ȡEDMA����ַʧ��:     %d \r\n",g_stBbpDbgErrInfo.usGetConAddrErr);
    vos_printf("�����ź���ʧ��:         %d \r\n",g_stBbpDbgErrInfo.usCreateSemErr);
    for (i = 0;i < BBP_DBG_CODER_SRC_NUM; i++)
    {
        vos_printf("ͨ��[%d]����Դ�ռ䲻���Ĵ���:    %d\r\n",i,g_stBbpDbgErrInfo.astBbpDbgSocpInfo[i].ulBuffNotEnough);
        vos_printf("ͨ��[%d]����Դ�ռ��С:          %d\r\n",i,g_stBbpDbgErrInfo.astBbpDbgSocpInfo[i].ulWriteBufSize);
        vos_printf("ͨ��[%d]��ȡдbufferʧ�ܵĴ���:  %d\r\n",i,g_stBbpDbgErrInfo.astBbpDbgSocpInfo[i].ulGetWriteBuffErr);
        vos_printf("ͨ��[%d]WriteDoneʧ�ܴ���:       %d\r\n",i,g_stBbpDbgErrInfo.astBbpDbgSocpInfo[i].ulWriteDoneErr);
        vos_printf("ͨ��[%d]WriteDone�ɹ�����:       %d\r\n",i,g_stBbpDbgErrInfo.astBbpDbgSocpInfo[i].ulWriteDoneOk);
    }
}


VOS_VOID Om_BbpDumpShow(VOS_VOID)
{
    vos_printf("������Ϣ�ռ�ʧ��:   %d\r\n", g_stBbpDumpErrInfo.usAllocMsgErr);
    vos_printf("�ر�EDMAʧ��:       %d\r\n", g_stBbpDumpErrInfo.usCloseEdmaErr);
    vos_printf("����EDMAʧ��:       %d\r\n", g_stBbpDumpErrInfo.usConfigEdmaErr);
    vos_printf("�����ж����� :      %d\r\n", g_stBbpDumpErrInfo.usEdmaErrCode);
    vos_printf("��ʼ��EDMAʧ�� :    %d\r\n", g_stBbpDumpErrInfo.usInitEdmaErr);
    vos_printf("����EDMAʧ��:       %d\r\n", g_stBbpDumpErrInfo.usStartEdmaErr);
    vos_printf("ֹͣEDMAʧ��:       %d\r\n", g_stBbpDumpErrInfo.usStopEdmaErr);

    vos_printf("����EDMAͨ����:     %d\r\n", g_stBbpDumpInfo.lEdmaChanID);
    vos_printf("����״̬:           %d\r\n", g_stBbpDumpInfo.usDUMPStatus);
    vos_printf("����EDMA��ɻص�:   %d\r\n", g_stBbpDumpInfo.usDUMPDone);
    vos_printf("����ģʽ:           %d\r\n", g_stBbpDumpInfo.usDUMPMode);
}


VOS_VOID Om_BbpDbgCycShow(VOS_VOID)
{
    VOS_UINT32                          i;
    BALONG_DMA_CB                      *pstCycBufTemp;

    pstCycBufTemp = g_pstBbpCycBufHead;

    /* ����ѭ������ */
    for (i = 0; i < BBP_DBG_CODER_SRC_NUM * BBP_DBG_NODE_NUM; i++)
    {
        vos_printf("Node[%d]lli :       0x%x\r\n",i,pstCycBufTemp->lli);
        vos_printf("Node[%d]bindx :     0x%x\r\n",i,pstCycBufTemp->bindx);
        vos_printf("Node[%d]cindx :     0x%x\r\n",i,pstCycBufTemp->cindx);
        vos_printf("Node[%d]cnt1 :      0x%x\r\n",i,pstCycBufTemp->cnt1);
        vos_printf("Node[%d]cnt0 :      0x%x\r\n",i,pstCycBufTemp->cnt0);
        vos_printf("Node[%d]src_addr :  0x%x\r\n",i,pstCycBufTemp->src_addr);
        vos_printf("Node[%d]des_addr :  0x%x\r\n",i,pstCycBufTemp->des_addr);
        vos_printf("Node[%d]config :    0x%x\r\n",i,pstCycBufTemp->config);
        pstCycBufTemp++;
    }

}
VOS_VOID Om_BbpBaseAddrShow(VOS_VOID)
{
    vos_printf("GBBP_BASE_ADDR:          0x%x\r\n",GBBP_BASE_ADDR);
    vos_printf("WBBP_BASE_ADDR:          0x%x\r\n",WBBP_BASE_ADDR);
    vos_printf("SOC_BBP_COMM_BASE_ADDR:  0x%x\r\n",SOC_BBP_COMM_BASE_ADDR);
    vos_printf("SOC_BBP_WCDMA_BASE_ADDR: 0x%x\r\n",SOC_BBP_WCDMA_BASE_ADDR);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

