

/******************************************************************************
   1 ͷ�ļ�����
******************************************************************************/
#include "stddef.h"
#include "om.h"
#include "TTFMem.h"
#include "TTFComm.h"
#include "pslog.h"
#include "TTFMemRB.h"
#include "NVIM_Interface.h"
#include "TtfErrlog.h"

#ifdef  __cplusplus
  #if  __cplusplus
  extern "C"{
  #endif
#endif



/******************************************************************************
   2 �ⲿ������������
******************************************************************************/
extern  VOS_VOID CDS_FreeIpfDlSrcMem(VOS_VOID);

/******************************************************************************
   3 ˽�ж���
******************************************************************************/
/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
/*lint -e767*/
#define    THIS_FILE_ID                    PS_FILE_ID_TTF_MEM_C
/*lint +e767*/

#define    TTF_MEM_ACPU_FREE_MEM_CNT_ADDR  (ECS_TTF_ACPU_FREE_MEM_CNT_ADDR)    /* ���C�˲�ѯA���ڴ��Ԥ����Ŀ�ĵ�ַ */

/*ACPU��û��RLC PID*/
#if (OSA_CPU_ACPU == VOS_OSA_CPU)
#define    WUEPS_PID_RLC                   (112)
#endif

/******************************************************************************
   4 ȫ�ֱ�������
******************************************************************************/
#if (FEATURE_ON == FEATURE_TTF_MEM_DEBUG)
VOS_UINT32                                 g_ulTtfBlkMemCheck = 1;             /* �����Ƿ����TTF BLK MEM */
#endif
VOS_UINT32                                 g_ulTtfBlkMemAlertEvtCnt    =0;
VOS_UINT32                                 g_ulTtfBlkMemAlocFailEvtCnt =0;


/* �㿽���ڴ�س�ʼ����־����ֹ�ظ����ó�ʼ������ */
VOS_UINT32                              g_ulTtfMemInitFlag      = VOS_FALSE;

/* �ڴ�ع������ȫ�ֱ��� */
TTF_MEM_POOL_STRU                       g_astTtfMemPool[TTF_MEM_POOL_ID_BUTT];


/* �ڴ�����ʧ��ָ������ */
unsigned short *const                   g_apusTtfMemAllocFailCnt[TTF_MEM_POOL_ID_BUTT] =
                                        {
                                            &g_astTtfMemPool[TTF_MEM_POOL_ID_DL_SHARE].usTtfMemAllocFailCnt,
                                            &g_astTtfMemPool[TTF_MEM_POOL_ID_DL_PDU].usTtfMemAllocFailCnt,
                                            &g_astTtfMemPool[TTF_MEM_POOL_ID_UL_DATA].usTtfMemAllocFailCnt,
                                            &g_astTtfMemPool[TTF_MEM_POOL_ID_EXT].usTtfMemAllocFailCnt,
                                            &g_astTtfMemPool[TTF_MEM_POOL_ID_UL_IP_DATA].usTtfMemAllocFailCnt
                                        };

/* �ڴ���������ʧ��ָ������ */
unsigned short *const                   g_apusTtfMemConAllocFailCnt[TTF_MEM_POOL_ID_BUTT] =
                                        {
                                            &g_astTtfMemPool[TTF_MEM_POOL_ID_DL_SHARE].usTtfMemConAllocFailCnt,
                                            &g_astTtfMemPool[TTF_MEM_POOL_ID_DL_PDU].usTtfMemConAllocFailCnt,
                                            &g_astTtfMemPool[TTF_MEM_POOL_ID_UL_DATA].usTtfMemConAllocFailCnt,
                                            &g_astTtfMemPool[TTF_MEM_POOL_ID_EXT].usTtfMemConAllocFailCnt,
                                            &g_astTtfMemPool[TTF_MEM_POOL_ID_UL_IP_DATA].usTtfMemConAllocFailCnt
                                        };

/* �ڴ�����ʧ�ܳ������޴��� */
unsigned short *const                   g_apusTtfMemExcThresholdCnt[TTF_MEM_POOL_ID_BUTT] =
                                        {
                                            &g_astTtfMemPool[TTF_MEM_POOL_ID_DL_SHARE].usTtfMemExcThresholdCnt,
                                            &g_astTtfMemPool[TTF_MEM_POOL_ID_DL_PDU].usTtfMemExcThresholdCnt,
                                            &g_astTtfMemPool[TTF_MEM_POOL_ID_UL_DATA].usTtfMemExcThresholdCnt,
                                            &g_astTtfMemPool[TTF_MEM_POOL_ID_EXT].usTtfMemExcThresholdCnt,
                                            &g_astTtfMemPool[TTF_MEM_POOL_ID_UL_IP_DATA].usTtfMemExcThresholdCnt
                                        };

/* ��ȡ�ڴ�ع�����ƽṹ�꣬��TTF_MEM�ڲ�ʹ�� */
#define TTF_MEM_GET_POOL(ulPoolId)      (&g_astTtfMemPool[(ulPoolId)])

/* �����ڴ��ͷſ�ָ����Ϣ */
#define TTF_MEM_FREE_SAVE_NULL_PTR_INFO(stNullPtrInfo,usFileId,usLineNum)   /*lint -e717*/\
            {\
                (stNullPtrInfo).ulTtfMemFreeNullPtrCnt++;\
                (stNullPtrInfo).usTtfMemFreeFileId  = (usFileId);\
                (stNullPtrInfo).usTtfMemFreeLineNum = (usLineNum);\
            }/*lint +e717*/

/* �����ڴ��ͷŴ����ڴ���Ϣ */
#define TTF_MEM_FREE_SAVE_INVALID_MEM_INFO(astTtfMemFreeInvalidMemInfo,pstTtfMem,ucType,usTypeValue,usFileId,usLineNum)  /*lint -e717*/\
            {\
                (astTtfMemFreeInvalidMemInfo).ulInvalidTtfMemCnt++;\
                (astTtfMemFreeInvalidMemInfo).ulInvalidTtfMemAddr   = (VOS_UINT32)pstTtfMem;\
                (astTtfMemFreeInvalidMemInfo).usTtfMemFreeFileId    = (usFileId);\
                (astTtfMemFreeInvalidMemInfo).usTtfMemFreeLineNum   = (usLineNum);\
                (astTtfMemFreeInvalidMemInfo).ucInvalidTtfMemType   = (ucType);\
                (astTtfMemFreeInvalidMemInfo).usInvalidTtfMemValue  = (usTypeValue);\
            } /*lint +e717*/


/* ����MACͷԤ������ */
const VOS_UINT16 TTF_MEM_MAC_HEADER_RSV_LEN = 14;

/* ����SKBUFFͷԤ������ */
const VOS_UINT16 TTF_MEM_SKB_HEAD_PAD_LEN   = 64;

/* ����SKBUFFβ��Ԥ������ */
const VOS_UINT16 TTF_MEM_SKB_TAIL_PAD_LEN   = 288;

/* ÿ�η����ڴ��ϱ��¼�ʱ��ÿ�������ڴ��¼��������� */
VOS_UINT32                              g_ulTtfMemEventRptCntThresholdPerTime = 1;

/* ÿ�η����ڴ��ϱ��¼�ʱ��ÿ�������ڴ��¼��������� */
VOS_UINT16                              g_usTtfMemAllocFailCntThreshold = 0x3FF;

/* �ڴ�����������ڴ�ʧ�ܸ�λ���� */
VOS_UINT16                              g_usTtfMemResetThreshold = 20;

/* �����ڴ���ϸ��Ϣ��ӡ���� */
VOS_UINT8                               g_ucTtfMemPrintDetail      = PS_TRUE;

/* TTF MEM FREE ��ά�ɲ�ʵ�� */
TTF_MEM_FREE_MNTN_INFO_STRU             g_stTtfMemFreeMntnEntity;

TTF_MEM_SOLUTION_CFG_NV_STRU            g_stTtfMemSolution;

TTF_NODE_MEM_POOL_STRU                  g_stTtfNodeMemPool = {0};
TTF_NODE_MEM_NODE_STRU                  g_stNodeMemSpace[TTF_NODE_MEM_POOL_CNT];

/* �ڴ�й¶����ʵ�� */
TTF_MEM_RCOV_ENTITY_STRU                g_stTtfMemRcovEnt;

/******************************************************************************
   5 ����ʵ��
******************************************************************************/


VOS_VOID TTF_NodeMemAbnmlEventRpt
(
    VOS_UINT16   usPid,
    VOS_UINT16   usFileId,
    VOS_UINT16   usLineNo,
    TTF_NODE_MEM_NODE_STRU  *pstNodeMem,
    TTF_NODE_MEM_TRIG_TYPE_ENUM_UINT8 enTrigType
)
{
    VOS_UINT8                          aucData[offsetof(PS_OM_EVENT_IND_STRU, aucData) + sizeof(TTF_OM_EVENT_NODE_MEM_ABNML_STRU)] = {0};
    PS_OM_EVENT_IND_STRU              *pstEventInd;
    TTF_OM_EVENT_NODE_MEM_ABNML_STRU  *pstNodeMemEvent;

    pstEventInd     = (PS_OM_EVENT_IND_STRU *)aucData;
    pstNodeMemEvent = (TTF_OM_EVENT_NODE_MEM_ABNML_STRU *)pstEventInd->aucData;

    pstEventInd->ulLength   = sizeof(aucData) - offsetof( PS_OM_EVENT_IND_STRU, usEventId);
    pstEventInd->usEventId  = TTF_EVENT_NODE_MEM_ABNML_INFO;
    pstEventInd->ulModuleId = WUEPS_PID_RLC;

    pstNodeMemEvent->usPid          = usPid;
    pstNodeMemEvent->usCurFileId    = usFileId;
    pstNodeMemEvent->usCurLineNum   = usLineNo;
    pstNodeMemEvent->ucTrigType     = enTrigType;
    pstNodeMemEvent->pstNodeMemAddr = pstNodeMem;

    /* ��ַ���Ե��п����ǷǷ���ַ,�������� */
    if ((VOS_NULL_PTR != pstNodeMem) && (TTF_NODE_MEM_CHK_ADDR_FAIL != enTrigType))
    {
        PS_MEM_CPY(&(pstNodeMemEvent->stNodeMem), pstNodeMem, sizeof(TTF_NODE_MEM_NODE_STRU));
    }

    /*����OM���¼��ϱ��ӿ�*/
    if ( PS_SUCC != OM_Event(pstEventInd))
    {
        PS_LOG(WUEPS_PID_RLC, 0, PS_PRINT_ERROR, "TTF_NodeMemAbnmlEventRpt: OM Send Event Fail!");
    }

    return;
}


VOS_VOID TTF_NodeMemPoolInit(VOS_VOID)
{
    VOS_INT i = 0;

    /* ����Ѿ���ʼ���Ͳ��ٳ�ʼ�� */
    if (TTF_NODE_MEM_POOL_STATE_ACTIVE == g_stTtfNodeMemPool.ucState)
    {
        return;
    }
    g_stTtfNodeMemPool.ucState      = TTF_NODE_MEM_POOL_STATE_ACTIVE;
    g_stTtfNodeMemPool.usFreeCnt    = TTF_NODE_MEM_POOL_CNT;
    g_stTtfNodeMemPool.usTotalCnt   = g_stTtfNodeMemPool.usFreeCnt;
    g_stTtfNodeMemPool.pBaseAddr    = TTF_NODE_MEM_POOL_BASE_ADDR;
    g_stTtfNodeMemPool.ulTotalSize  = TTF_NODE_MEM_POOL_CNT * sizeof(TTF_NODE_MEM_NODE_STRU);

    for (; i < g_stTtfNodeMemPool.usTotalCnt; i++)
    {
        g_stTtfNodeMemPool.pFreeNode[i] = &g_stNodeMemSpace[i];
#if(TTF_NODE_MEM_DBG == FEATURE_ON)
        g_stNodeMemSpace[i].ucState      = TTF_NODE_MEM_IDLE;
        g_stNodeMemSpace[i].ucMagicNo    = 0x5A;
        g_stNodeMemSpace[i].usFileId     = 0;
        g_stNodeMemSpace[i].usLineNo     = 0;
        g_stNodeMemSpace[i].usFreeFileId = 0;
        g_stNodeMemSpace[i].usFreeLineNo = 0;
#endif
    }
}
VOS_UINT8 *TTF_NodeMemAlloc_Debug
(
    VOS_UINT32  ulPid,
    VOS_UINT16  usLen,
    VOS_UINT16  usFileId,
    VOS_UINT16  usLineNo
)
{
    VOS_INT32                lLockKey;
    TTF_NODE_MEM_NODE_STRU  *pMem = VOS_NULL_PTR;

    if (usLen > TTF_NODE_MEM_POOL_SIZE)
    {
        TTF_LOG3(ulPid, PS_PRINT_ERROR,
            "TTF_NodeMemAlloc_Debug, too long uslen %d, usFileId %d, usLineNo %d! \n",
            (VOS_INT32)usLen, (VOS_INT32)usFileId, (VOS_INT32)usLineNo);

        return VOS_NULL_PTR;
    }

    lLockKey = VOS_SplIMP();
    if (g_stTtfNodeMemPool.usFreeCnt > 0)
    {
        g_stTtfNodeMemPool.usFreeCnt--;
        pMem = (TTF_NODE_MEM_NODE_STRU *)g_stTtfNodeMemPool.pFreeNode[g_stTtfNodeMemPool.usFreeCnt];
    }
    VOS_Splx(lLockKey);

    if (VOS_NULL_PTR != pMem)
    {
#if(TTF_NODE_MEM_DBG == FEATURE_ON)
        if (TTF_NODE_MEM_IDLE != pMem->ucState)
        {
            TTF_NodeMemAbnmlEventRpt((VOS_UINT16)ulPid, usFileId, usLineNo, pMem, TTF_NODE_MEM_DUP_ALLOC);

            vos_printf("TTF_NodeMemAlloc_Debug, dup alloc found usFreeFileId %d usFreeLinNo %d usCurFileId %d, usCurLineNo %d usAllocFileId %d, usAllocLineNo %d pMem 0x%x\n",
                (VOS_INT32)pMem->usFreeFileId,  (VOS_INT32)pMem->usFreeLineNo,
                (VOS_INT32)usFileId, (VOS_INT32)usLineNo,
                (VOS_INT32)pMem->usFileId,  (VOS_INT32)pMem->usLineNo, pMem);

            return VOS_NULL_PTR;
        }

        pMem->usPid     = (VOS_UINT16)ulPid;
        pMem->usFileId  = usFileId;
        pMem->usLineNo  = usLineNo;
        pMem->ucState   = TTF_NODE_MEM_USED;
#endif
        return pMem->aucData;
    }


    TTF_LOG3(ulPid, PS_PRINT_INFO,
        "TTF_NodeMemAlloc_Debug, Alloc Mem Fail, No free mem, usLen %d, usFileId %d, usLineNo %d! \n",
        (VOS_INT32)usLen, (VOS_INT32)usFileId, (VOS_INT32)usLineNo);

    return VOS_NULL_PTR;
}


VOS_UINT32 TTF_NodeMemCheck(VOS_VOID *pMem)
{
    if ( ((VOS_UINT32)pMem < (VOS_UINT32)g_stTtfNodeMemPool.pBaseAddr)
        || ((VOS_UINT32)pMem > (VOS_UINT32)((VOS_UINT8 *)g_stTtfNodeMemPool.pBaseAddr + g_stTtfNodeMemPool.ulTotalSize)))
    {
        return VOS_NO;
    }

    return VOS_YES;
}


VOS_VOID TTF_NodeMemFree_Debug
(
    VOS_UINT32   ulPid,
    VOS_UINT8   *pMem,
    VOS_UINT16   usFileId,
    VOS_UINT16   usLineNo
)
{
    VOS_INT32   lLockKey;
    TTF_NODE_MEM_NODE_STRU *pstTtfNodeMem;

    if (VOS_NULL_PTR == pMem)
    {
        TTF_LOG2(ulPid, PS_PRINT_ERROR,
            "TTF_NodeMemFree_Debug, pMem is NULL, usFileId %d, usLineNo %d \n ",
            (VOS_INT32)usFileId, (VOS_INT32)usLineNo);

        return;
    }

    pMem         -= TTF_NODE_MEM_RSV_LEN;
    pstTtfNodeMem = (TTF_NODE_MEM_NODE_STRU *)pMem;

    if (VOS_NO == TTF_NodeMemCheck(pstTtfNodeMem))
    {
        TTF_LOG4(ulPid, PS_PRINT_ERROR,
            "TTF_NodeMemFree_Debug, Not Ttf node mem free, pMem 0x%x, Node Mem 0x%x, usFileId %d, usLineNo %d \n",
            (VOS_INT32)pMem, (VOS_INT32)g_stTtfNodeMemPool.pBaseAddr, (VOS_INT32)usFileId, (VOS_INT32)usLineNo);

        TTF_NodeMemAbnmlEventRpt((VOS_UINT16)ulPid, usFileId, usLineNo, pstTtfNodeMem, TTF_NODE_MEM_CHK_ADDR_FAIL);

        return;
    }

#if(TTF_NODE_MEM_DBG == FEATURE_ON)
    if (TTF_NODE_MEM_USED != pstTtfNodeMem->ucState)
    {
        TTF_NodeMemAbnmlEventRpt((VOS_UINT16)ulPid, usFileId, usLineNo, pstTtfNodeMem, TTF_NODE_MEM_DUP_FREE);

        vos_printf("TTF_NodeMemFree_Debug, dup free found usMagicNo %d usFreeFileId %d usFreeLinNo %d usCurFileId %d, usCurLineNo %d, usAllocFileId %d, usAllocLineNo %d, pMem 0x%x\n",
            (VOS_INT32)pstTtfNodeMem->ucMagicNo, (VOS_INT32)pstTtfNodeMem->usFreeFileId,
            (VOS_INT32)pstTtfNodeMem->usFreeLineNo, (VOS_INT32)usFileId, (VOS_INT32)usLineNo,
            (VOS_INT32)pstTtfNodeMem->usFileId,  (VOS_INT32)pstTtfNodeMem->usLineNo, pstTtfNodeMem);

        return;
    }

    /* ����ڴ��Ƿ��б��� */
    if (0x5A != pstTtfNodeMem->ucMagicNo)
    {
        TTF_NodeMemAbnmlEventRpt((VOS_UINT16)ulPid, usFileId, usLineNo, pstTtfNodeMem, TTF_NODE_MEM_CHK_MEM_FAIL);

        vos_printf("TTF_NodeMemFree_Debug, MagicNo modified usMagicNo %d usFreeFileId %d usFreeLinNo %d usCurFileId %d, usCurLineNo %d, usAllocFileId %d, usAllocLineNo %d, pMem 0x%x\n",
            (VOS_INT32)pstTtfNodeMem->ucMagicNo, (VOS_INT32)pstTtfNodeMem->usFreeFileId,
            (VOS_INT32)pstTtfNodeMem->usFreeLineNo, (VOS_INT32)usFileId, (VOS_INT32)usLineNo,
            (VOS_INT32)pstTtfNodeMem->usFileId,  (VOS_INT32)pstTtfNodeMem->usLineNo, pstTtfNodeMem);
    }

    pstTtfNodeMem->usPid            = (VOS_UINT16)ulPid;
    pstTtfNodeMem->usFreeFileId     = usFileId;
    pstTtfNodeMem->usFreeLineNo     = usLineNo;
    pstTtfNodeMem->ucState          = TTF_NODE_MEM_IDLE;
#endif

    lLockKey = VOS_SplIMP();
    if (g_stTtfNodeMemPool.usFreeCnt >= g_stTtfNodeMemPool.usTotalCnt)
    {
        VOS_Splx(lLockKey);
        return;
    }

    g_stTtfNodeMemPool.pFreeNode[g_stTtfNodeMemPool.usFreeCnt] = (TTF_NODE_MEM_NODE_STRU *)pMem;
    g_stTtfNodeMemPool.usFreeCnt++;
    VOS_Splx(lLockKey);
}
VOS_VOID  TTF_NodeMemShow(VOS_UINT32 ulStartIndx, VOS_UINT32 ulEndIndx)
{
    VOS_UINT32 i = 0;

    vos_printf("Node mem base addr 0x%x\n", g_stTtfNodeMemPool.pBaseAddr);
    vos_printf("Current free cnt %d \n", g_stTtfNodeMemPool.usFreeCnt);
    vos_printf("Current total cnt %d \n", g_stTtfNodeMemPool.usTotalCnt);

    vos_printf("ALL Mem:\n\n\n");
    for(i = ulStartIndx; i < ulEndIndx; i++)
    {
        vos_printf("g_stTtfNodeMemPool.pFreeNode[%d] = 0x%x ", i, g_stTtfNodeMemPool.pFreeNode[i]);

#if(TTF_NODE_MEM_DBG == FEATURE_ON)
        vos_printf("ucMagicNo = 0x%x, usPid = %d, usFileId = %d, usLineNo = %d, ucState = %d usFreeFileId = %d, usFreeLineNo = %d\n",
        g_stTtfNodeMemPool.pFreeNode[i]->ucMagicNo, g_stTtfNodeMemPool.pFreeNode[i]->usPid, g_stTtfNodeMemPool.pFreeNode[i]->usFileId,
        g_stTtfNodeMemPool.pFreeNode[i]->usLineNo, g_stTtfNodeMemPool.pFreeNode[i]->ucState,
        g_stTtfNodeMemPool.pFreeNode[i]->usFreeFileId, g_stTtfNodeMemPool.pFreeNode[i]->usFreeLineNo);
#endif
    }

    vos_printf("\r\n");
}


VOS_UINT32  TTF_NodeMemLeakCheck(VOS_VOID)
{
    /* �ų�Gʹ�õ�19�龲̬�ڴ� */
    #if (FEATURE_OFF == FEATURE_MULTI_MODEM)
    if ((g_stTtfNodeMemPool.usFreeCnt + TTF_NODE_MEM_G_STATIC_CNT) != g_stTtfNodeMemPool.usTotalCnt)
    #else
    if ((g_stTtfNodeMemPool.usFreeCnt + (TTF_NODE_MEM_G_STATIC_CNT * 2)) != g_stTtfNodeMemPool.usTotalCnt)
    #endif
    {
        return PS_TRUE;
    }

    return PS_FALSE;
}

VOS_UINT32 TTF_GetCipherMemSection(VOS_UINT32 *pulBaseAddr)
{
    BSP_DDR_SECT_QUERY   stQuery;
    BSP_DDR_SECT_INFO    stInfo;
    VOS_UINT32           ulBaseAddr;

    stQuery.enSectType = BSP_DDR_SECT_TYPE_TTF;
    DRV_GET_FIX_DDR_ADDR(&stQuery, &stInfo);

    ulBaseAddr = stInfo.ulSectPhysAddr;

    *pulBaseAddr = TTF_UL_CIPHER_PARA_NODE_ADDR(ulBaseAddr);

    return PS_SUCC;
}


VOS_UINT32 TTF_GetTTFMemSection(VOS_UINT32 *pulBaseAddr, VOS_UINT32 *pulLength)
{
    BSP_DDR_SECT_QUERY             stQuery;
    BSP_DDR_SECT_INFO              stInfo;
    VOS_UINT32                     ulBaseAddr;

    stQuery.enSectType = BSP_DDR_SECT_TYPE_TTF;
    DRV_GET_FIX_DDR_ADDR(&stQuery, &stInfo);

    ulBaseAddr   = stInfo.ulSectPhysAddr;
    *pulBaseAddr = TTF_MEM_POOL_BASE_ADDR(ulBaseAddr);
    *pulLength   = (stInfo.ulSectSize - TTF_MEMCTRL_GetOffsetBaseLen(TTF_MEMCTRL_CCORE_POOL_ADDR_TYPE));

    return PS_SUCC;
}



VOS_UINT32 TTF_MemCheckReportMemInfoStatus
(
    VOS_UINT32 ulPid,
    VOS_UINT32 ulPoolId,
    BLK_MEM_USED_INFO_TRIG_TYPE_ENUM_UINT8 enTrigType
)
{
    VOS_INT32                           lLockKey;
    TTF_MEM_POOL_STRU                  *pstPoolInfo;

    /*
    ������������ϱ��¼�
    1�������ڴ�й¶
    2����һ�η����ڴ�����ʧ��
    3����һ�η����ڴ治��澯
    */
    lLockKey = VOS_SplIMP();
    if ( TRIG_TYPE_LEAK == enTrigType )
    {

    }
    else if ( (TRIG_TYPE_ALLOC_FAIL == enTrigType) && (0 == g_ulTtfBlkMemAlocFailEvtCnt) )
    {
        g_ulTtfBlkMemAlocFailEvtCnt++;
    }
    else if ( (TRIG_TYPE_ALERT == enTrigType) && (0 == g_ulTtfBlkMemAlertEvtCnt) )
    {
        g_ulTtfBlkMemAlertEvtCnt++;
    }
    else
    {
        VOS_Splx(lLockKey);
        return PS_FALSE;
    }
    VOS_Splx(lLockKey);

    pstPoolInfo     = TTF_MEM_GET_POOL(ulPoolId);

    if (PS_TRUE != pstPoolInfo->usUsedFlag)
    {
        TTF_LOG1(ulPid, PS_PRINT_ERROR, "TTF_MemCheckReportMemInfoStatus: Pool %d not used!\n",  (VOS_INT32)ulPoolId);
        return PS_FALSE;
    }

    return PS_TRUE;
}



VOS_VOID TTF_MemTraceInfoRptSaveMemData
(
    VOS_UINT32 ulPid,
    VOS_UINT32 ulPoolId,
    BLK_MEM_TRACE_INFO_STRU *pstBlkMemTraceInfo,
    TTF_MEM_ST *pMem
)
{

    if ( ulPoolId != pMem->ucPoolId )
    {
        TTF_LOG2(ulPid, PS_PRINT_ERROR, "TTF_MemTraceInfoRptSaveMemData, ulPoolId %d, pMem->ucPoolId %d Mismatch! \n", (VOS_INT32)ulPoolId, pMem->ucPoolId);
        return;
    }

    if ( TTF_MEM_POOL_ID_EXT > ulPoolId )
    {
        if ( VOS_NULL_PTR != pMem->pData )
        {
            pstBlkMemTraceInfo->aucMemData[0]   = pMem->pData[0];
            pstBlkMemTraceInfo->aucMemData[1]   = pMem->pData[1];
            pstBlkMemTraceInfo->aucMemData[2]   = pMem->pData[2];
            pstBlkMemTraceInfo->aucMemData[3]   = pMem->pData[3];
        }
        else
        {
            VOS_MemSet(pstBlkMemTraceInfo->aucMemData, 0, sizeof(pstBlkMemTraceInfo->aucMemData));
            TTF_LOG1(ulPid, PS_PRINT_ERROR, "TTF_MemTraceInfoRptSaveMemData, ulPoolId %d Mem Ptr is Null! \n", (VOS_INT32)ulPoolId);
        }

        return;
    }

    if ( TTF_MEM_POOL_ID_EXT == ulPoolId )
    {
        *((VOS_UINT32 *)pstBlkMemTraceInfo->aucMemData) = (VOS_UINT32)pMem->pExtBuffAddr;
    }

    return;
}/* TTF_MemTraceInfoRptSaveMemData */

VOS_VOID TTF_BlkMemUsedInfoEventRpt(VOS_UINT32 ulPid,
    VOS_UINT32  ulPoolId, BLK_MEM_USED_INFO_TRIG_TYPE_ENUM_UINT8 enTrigType )
{
    PS_OM_EVENT_IND_STRU                   *pstTtfOmEventInd = VOS_NULL_PTR;
    VOS_UINT32                              ulLength;
    VOS_UINT32                              ulDataLen;
    VOS_UINT32                              ulTotalCnt = 0;
    TTF_MEM_POOL_STRU                      *pstPoolInfo;
    TTF_OM_EVENT_BLK_MEM_USED_INFO_STRU    *pstTtfOmEventBlkMemUsedInfo;

#if (FEATURE_ON == FEATURE_TTF_MEM_DEBUG)
    TTF_MEM_ST                             *pMem;
    VOS_UINT32                              ulBlkMemNum;
    VOS_UINT16                              usRptCnt = 0;
    VOS_UINT32                              ulOffset = 0;
    VOS_UINT8                               ucRptNo = 0;
    VOS_UINT16                              usRptEventCnt = 0;
    BLK_MEM_TRACE_INFO_STRU                *pstBlkMemTraceInfo;
#endif

    /* ����ϱ�״̬�Ƿ���ȷ */
    if ( PS_TRUE != TTF_MemCheckReportMemInfoStatus(ulPid, ulPoolId, enTrigType) )
    {
        return;
    }

    pstPoolInfo  = TTF_MEM_GET_POOL(ulPoolId);

    ulTotalCnt   = pstPoolInfo->usTtfMemUsedCnt;

    /*========================================*/
    /*û�д��ڴ���Ա��뿪�أ�����û�д��ڴ����ʹ�ÿ��أ����ϱ�TTF Blk MEM й¶�ܿ��� */
#if (FEATURE_ON == FEATURE_TTF_MEM_DEBUG)
    if ( !TTF_BLK_MEM_DEBUG_SWITCH_ON )
#endif
    {
        ulDataLen           = offsetof(TTF_OM_EVENT_BLK_MEM_USED_INFO_STRU, astBlkMemTraceInfo);

        /*��Ϣ�ܳ���*/
        ulLength            = offsetof(PS_OM_EVENT_IND_STRU, aucData) + ulDataLen;

        /*������Ϣ�ڴ�*/
        pstTtfOmEventInd    = (PS_OM_EVENT_IND_STRU *)PS_MEM_ALLOC(ulPid, ulLength);

        if (VOS_NULL_PTR == pstTtfOmEventInd )
        {
            PS_LOG(ulPid, 0, PS_PRINT_ERROR, "TTF_ShowUsedBlkMemInfo: MemAlloc Fail!");
            return;
        }

        /*��д��Ϣ����*/
        pstTtfOmEventInd->ulLength      = ulLength - offsetof( PS_OM_EVENT_IND_STRU, usEventId);
        pstTtfOmEventInd->usEventId     = TTF_EVENT_BLK_MEM_USED_INFO;
        pstTtfOmEventInd->ulModuleId    = WUEPS_PID_RLC;/* ����EventĿǰֻ�ܶ�Ӧһ���̶���PID���ڴ�ֻ�����ݶ�ΪRLC PID */

        pstTtfOmEventBlkMemUsedInfo     = (TTF_OM_EVENT_BLK_MEM_USED_INFO_STRU *)pstTtfOmEventInd->aucData;
        pstTtfOmEventBlkMemUsedInfo->ucPoolId       = (VOS_UINT8)ulPoolId;
        pstTtfOmEventBlkMemUsedInfo->enTrigType     = enTrigType;
        pstTtfOmEventBlkMemUsedInfo->ucNo           = 1;
        pstTtfOmEventBlkMemUsedInfo->usRptCnt       = 0;
        pstTtfOmEventBlkMemUsedInfo->ulTotalUsedCnt = ulTotalCnt;

        /*����OM���¼��ϱ��ӿ�*/
        if ( PS_SUCC != OM_Event((PS_OM_EVENT_IND_STRU*)pstTtfOmEventInd))
        {
            PS_LOG(ulPid, 0, PS_PRINT_ERROR, "TTF_ShowUsedBlkMemInfo: OM Send Event Fail!");
        }
        /*�ͷ�������ڴ�*/
        PS_MEM_FREE(ulPid, pstTtfOmEventInd);
        return;
    }

#if (FEATURE_ON == FEATURE_TTF_MEM_DEBUG)
    /*========================================*/
    /* �����ڴ���Ա����ʹ�ÿ��أ��ϱ��ڴ������Ϣ */
    ulDataLen           = offsetof(TTF_OM_EVENT_BLK_MEM_USED_INFO_STRU, astBlkMemTraceInfo) + ( EVENT_BLK_MEM_USED_INFO_CNT_PER_MSG * sizeof(BLK_MEM_TRACE_INFO_STRU));

    /*��Ϣ�ܳ���*/
    ulLength            = offsetof(PS_OM_EVENT_IND_STRU,aucData) + ulDataLen;

    /*������Ϣ�ڴ�*/
    pstTtfOmEventInd    = (PS_OM_EVENT_IND_STRU *)PS_MEM_ALLOC(ulPid, ulLength);

    if ( VOS_NULL_PTR == pstTtfOmEventInd )
    {
        PS_LOG(ulPid, 0, PS_PRINT_ERROR, "TTF_ShowUsedBlkMemInfo: MemAlloc Fail!");
        return;
    }

    /* �����ڴ�� */
    for (ulBlkMemNum = 0; ulBlkMemNum < pstPoolInfo->usTtfMemTotalCnt; ulBlkMemNum ++)
    {
        pMem = pstPoolInfo->pstTtfMemStStartAddr + ulBlkMemNum;

        if (VOS_NULL_PTR == pMem->pstDbgInfo)
        {
            PS_LOG(ulPid, 0, PS_PRINT_ERROR, "Blk Mem debug info is null!");
            continue;
        }

        if (TTF_BLK_MEM_STATE_FREE != pMem->pstDbgInfo->enMemStateFlag)
        {
            ulOffset    =  offsetof(TTF_OM_EVENT_BLK_MEM_USED_INFO_STRU, astBlkMemTraceInfo)  + ( usRptCnt * sizeof(BLK_MEM_TRACE_INFO_STRU) );

            pstBlkMemTraceInfo  = (BLK_MEM_TRACE_INFO_STRU *)(pstTtfOmEventInd->aucData + ulOffset );
            pstBlkMemTraceInfo->ulAllocTick         = pMem->pstDbgInfo->ulAllocTick;
            pstBlkMemTraceInfo->usAllocFileId       = pMem->pstDbgInfo->usAllocFileId;
            pstBlkMemTraceInfo->usAllocLineNum      = pMem->pstDbgInfo->usAllocLineNum;

            pstBlkMemTraceInfo->ulTraceTick         = pMem->pstDbgInfo->ulTraceTick;
            pstBlkMemTraceInfo->usTraceFileId       = (pMem->pstDbgInfo->usTraceFileId);
            pstBlkMemTraceInfo->usTraceLineNum      = (pMem->pstDbgInfo->usTraceLineNum);

            TTF_MemTraceInfoRptSaveMemData(ulPid, ulPoolId, pstBlkMemTraceInfo, pMem);

            usRptCnt++;

            /* �������ͣ�ÿ����෢��һǧ�� */
            if (  EVENT_BLK_MEM_USED_INFO_CNT_PER_MSG == usRptCnt )
            {
                ucRptNo++;
                usRptCnt = 0;
                /*��д��Ϣ����*/

                pstTtfOmEventInd->ulLength      = ulLength - offsetof( PS_OM_EVENT_IND_STRU, usEventId);
                pstTtfOmEventInd->usEventId     = TTF_EVENT_BLK_MEM_USED_INFO;
                pstTtfOmEventInd->ulModuleId    = WUEPS_PID_RLC;

                pstTtfOmEventBlkMemUsedInfo     = (TTF_OM_EVENT_BLK_MEM_USED_INFO_STRU *)pstTtfOmEventInd->aucData;
                pstTtfOmEventBlkMemUsedInfo->ucPoolId       = (VOS_UINT8)ulPoolId;
                pstTtfOmEventBlkMemUsedInfo->enTrigType     = enTrigType;
                pstTtfOmEventBlkMemUsedInfo->ucNo           = ucRptNo;
                pstTtfOmEventBlkMemUsedInfo->usRptCnt       = EVENT_BLK_MEM_USED_INFO_CNT_PER_MSG;
                pstTtfOmEventBlkMemUsedInfo->ulTotalUsedCnt = ulTotalCnt;

                /*����OM���¼��ϱ��ӿ�*/
                if ( PS_SUCC != OM_Event((PS_OM_EVENT_IND_STRU*)pstTtfOmEventInd))
                {
                    PS_LOG(ulPid, 0, PS_PRINT_ERROR, "TTF_ShowUsedBlkMemInfo: OM Send Event Fail!");
                }

                ++usRptEventCnt;
            }

            if ( g_ulTtfMemEventRptCntThresholdPerTime < usRptEventCnt )
            {
                /* �ͷ�������ڴ� */
                PS_MEM_FREE(ulPid, pstTtfOmEventInd);
                return;
            }
        }
    }

    /* ���һ�����ݿ������� EVENT_BLK_MEM_USED_INFO_CNT_PER_MSG */
    if ( 0 != usRptCnt )
    {
        ucRptNo++;

        ulDataLen   = ( usRptCnt * sizeof(BLK_MEM_TRACE_INFO_STRU))  + offsetof(TTF_OM_EVENT_BLK_MEM_USED_INFO_STRU, astBlkMemTraceInfo) ;

        ulLength    = offsetof(PS_OM_EVENT_IND_STRU,aucData) + ulDataLen;

        /*��д��Ϣ����*/
        pstTtfOmEventInd->ulLength      = ulLength - offsetof( PS_OM_EVENT_IND_STRU, usEventId);
        pstTtfOmEventInd->usEventId     = TTF_EVENT_BLK_MEM_USED_INFO;
        pstTtfOmEventInd->ulModuleId    = WUEPS_PID_RLC;

        pstTtfOmEventBlkMemUsedInfo     = (TTF_OM_EVENT_BLK_MEM_USED_INFO_STRU *)pstTtfOmEventInd->aucData;
        pstTtfOmEventBlkMemUsedInfo->ucPoolId       = (VOS_UINT8)ulPoolId;
        pstTtfOmEventBlkMemUsedInfo->enTrigType     = enTrigType;
        pstTtfOmEventBlkMemUsedInfo->ucNo           = ucRptNo;
        pstTtfOmEventBlkMemUsedInfo->usRptCnt       = usRptCnt;
        pstTtfOmEventBlkMemUsedInfo->ulTotalUsedCnt = ulTotalCnt;

        /*����OM���¼��ϱ��ӿ�*/
        if ( PS_SUCC != OM_Event((PS_OM_EVENT_IND_STRU*)pstTtfOmEventInd))
        {
            PS_LOG(ulPid, 0, PS_PRINT_ERROR, "TTF_ShowUsedBlkMemInfo: OM Send Event Fail!");
        }
    }

    /*�ͷ�������ڴ�*/
    PS_MEM_FREE(ulPid, pstTtfOmEventInd);

    return ;
#endif
}



TTF_MEM_ST *TTF_MemBlkAllocPlus_Debug(VOS_UINT16 usFileID, VOS_UINT16 usLineNum,
     VOS_UINT32 ulPid, VOS_UINT32 ulPoolId, VOS_UINT16 usLen)
{
    TTF_MEM_POOL_STRU              *pstTtfMemPool;
    TTF_MEM_ST                     *pMem;
    TTF_MEM_ST                     *pTailMem;
    VOS_UINT16                      usRemLen;


    if ( ulPoolId >= TTF_MEM_POOL_ID_BUTT )
    {
        PS_LOG1(ulPid, 0, PS_PRINT_WARNING, "TTF_MemBlkAlloc_Debug,Poolid <1> is invalid!",
            (VOS_INT32)ulPoolId);
        return VOS_NULL_PTR;
    }

    /*================================*/    /* ��ȡ�ڴ�� */
    pstTtfMemPool = TTF_MEM_GET_POOL(ulPoolId);

    if (PS_TRUE != pstTtfMemPool->usUsedFlag)
    {
        PS_LOG1(ulPid, 0, PS_PRINT_WARNING, "TTF_MemBlkAlloc_Debug,Pool<1> is not used!",
            (VOS_INT32)ulPoolId);
        return VOS_NULL_PTR;
    }

    /*================================*/    /* ���볤���Ƿ��ڸ��ڴ��� */
    if (0 == usLen )
    {
        PS_LOG1(ulPid, 0, PS_PRINT_WARNING, "TTF_MemBlkAlloc_Debug,usLen exceed the ranger!",
            usLen);
        return VOS_NULL_PTR;
    }
    pMem = VOS_NULL_PTR;

    if (usLen <= pstTtfMemPool->usMaxByteLen)
    {
        pMem = TTF_MemBlkAlloc_Debug(usFileID, usLineNum, ulPid, ulPoolId, usLen);
        return pMem;
    }

    /*malloc more than one block and link the block together*/
    usRemLen = usLen;

    while (usRemLen > pstTtfMemPool->usMaxByteLen)
    {
        pTailMem = TTF_MemBlkAlloc_Debug(usFileID, usLineNum, ulPid, ulPoolId, pstTtfMemPool->usMaxByteLen);
        if (VOS_NULL_PTR == pTailMem)
        {
            PS_LOG(ulPid, 0, PS_PRINT_WARNING, "TTF_MemBlkAllocPlus_Debug,alloc ERR !");
            TTF_MemFree_Debug(usFileID, usLineNum, ulPid, &pTailMem);
            if (VOS_NULL_PTR != pMem)
            {
                TTF_MemFree_Debug(usFileID, usLineNum, ulPid, &pMem);
            }
            return VOS_NULL_PTR;
        }

        TTF_MemLink(ulPid, &pMem, pTailMem);

        usRemLen    -= pstTtfMemPool->usMaxByteLen;

    }

    if (usRemLen > 0)
    {
        pTailMem = TTF_MemBlkAlloc_Debug(usFileID, usLineNum, ulPid, ulPoolId, usRemLen);
        if (VOS_NULL_PTR == pTailMem)
        {
            PS_LOG(ulPid, 0, PS_PRINT_WARNING, "TTF_MemBlkAllocPlus_Debug,alloc ERR !");
            TTF_MemFree_Debug(usFileID, usLineNum, ulPid, &pMem);
            return VOS_NULL_PTR;
        }

        TTF_MemLink(ulPid, &pMem, pTailMem);
    }

    return pMem;
} /* TTF_MemBlkAllocPlus_Debug */


#if (FEATURE_ON == FEATURE_TTF_MEM_DEBUG)

VOS_UINT32 TTF_MemFreeSaveDebugInfo(VOS_UINT16 usFileID, VOS_UINT16 usLineNum,TTF_MEM_ST *pstTtfMem)
{

    if ( NULL != pstTtfMem->pstDbgInfo )
    {
        /*�ж��ڴ�ָ��״̬�Ƿ���ȷ*/
        if ( TTF_BLK_MEM_STATE_ALLOC != pstTtfMem->pstDbgInfo->enMemStateFlag )
        {
            TTF_MEM_FREE_SAVE_INVALID_MEM_INFO(g_stTtfMemFreeMntnEntity.astTtfMemFreeInvalidMemInfo,pstTtfMem,TTF_INVALID_MEM_TYPE_STUTAS,(VOS_UINT16)pstTtfMem->pstDbgInfo->enMemStateFlag,usFileID,usLineNum);
            TTF_LOG2(WUEPS_PID_RLC, PS_PRINT_ERROR, "Error: TTF_MemFreeSaveDebugInfo, Mem blk status is Free! FileID %d, LineNum %d \n",
                        usFileID, usLineNum);

            return PS_FAIL;
        }

        pstTtfMem->pstDbgInfo->enMemStateFlag       = TTF_BLK_MEM_STATE_FREE;
        pstTtfMem->pstDbgInfo->usAllocFileId        = usFileID;
        pstTtfMem->pstDbgInfo->usAllocLineNum       = usLineNum;
        pstTtfMem->pstDbgInfo->ulAllocTick          = OM_GetSlice();
    }
    else
    {
        TTF_LOG2(WUEPS_PID_RLC, PS_PRINT_ERROR, "TTF_MemFreeSaveDebugInfo, Blk Mem debug info is null! FileID %d, LineNum %d \n",
                        usFileID, usLineNum);

    }

    return PS_SUCC;

}/* TTF_MemFreeCtrlHead_Debug */


VOS_VOID TTF_MemRcovMem(VOS_UINT32 ulPoolId, VOS_UINT32 ulPid, VOS_UINT16 usFileId, VOS_UINT16 usLine)
{
    TTF_MEM_ST                     *pstTempMem;
    VOS_UINT16                      usBlkMemNum = 0;
    VOS_UINT16                      usMemRptCnt = 0;
    TTF_BLK_MEM_DEBUG_INFO_STRU    *pstBlkMemDebugInfo;
    TTF_BLK_MEM_DEBUG_INFO_STRU    *pstTempMemDebugInfo;
    TTF_MEM_POOL_STRU              *pstTtfMemPool;

    pstTtfMemPool = TTF_MEM_GET_POOL(ulPoolId);

    pstBlkMemDebugInfo = (TTF_BLK_MEM_DEBUG_INFO_STRU *)PS_MEM_ALLOC(ulPid, TTF_MEM_DEBUG_NUM * sizeof(TTF_BLK_MEM_DEBUG_INFO_STRU));

    if (VOS_NULL_PTR == pstBlkMemDebugInfo)
    {
        PS_LOG(ulPid, 0, PS_PRINT_ERROR, "TTF_MemBlkAlloc_Debug: MemAlloc Fail!");

        return;
    }

    pstTempMemDebugInfo = pstBlkMemDebugInfo;

    for (usBlkMemNum = 0; usBlkMemNum < pstTtfMemPool->usTtfMemTotalCnt; usBlkMemNum ++)
    {
        pstTempMem = pstTtfMemPool->pstTtfMemStStartAddr + usBlkMemNum;

        if ((VOS_NULL_PTR != pstTempMem->pstDbgInfo)
            &&(TTF_BLK_MEM_STATE_FREE != pstTempMem->pstDbgInfo->enMemStateFlag))
        {
            pstTempMemDebugInfo->ulAllocTick       = pstTempMem->pstDbgInfo->ulAllocTick;
            pstTempMemDebugInfo->usAllocFileId     = pstTempMem->pstDbgInfo->usAllocFileId;
            pstTempMemDebugInfo->usAllocLineNum    = pstTempMem->pstDbgInfo->usAllocLineNum;
            pstTempMemDebugInfo->ulTraceTick       = pstTempMem->pstDbgInfo->ulTraceTick;
            pstTempMemDebugInfo->usTraceFileId     = pstTempMem->pstDbgInfo->usTraceFileId;
            pstTempMemDebugInfo->usTraceLineNum    = pstTempMem->pstDbgInfo->usTraceLineNum;

            usMemRptCnt++;
            pstTempMemDebugInfo = pstBlkMemDebugInfo + usMemRptCnt;
        }

        if (usMemRptCnt >= TTF_MEM_DEBUG_NUM)
        {
            break;
        }
    }

    DRV_SYSTEM_ERROR(TTF_MEM_ALLOC_FAIL_SYSTEM_ERROR, usFileId, usLine,
                    (VOS_CHAR *)pstBlkMemDebugInfo, TTF_MEM_DEBUG_NUM * sizeof(TTF_BLK_MEM_DEBUG_INFO_STRU));

}

#endif



VOS_VOID TTF_PrintBlkMemPoolUsedInfo( VOS_UINT8 ucPoolId )
{
    TTF_MEM_POOL_STRU                  *pstTtfMemPoolInfo;
#if (FEATURE_ON == FEATURE_TTF_MEM_DEBUG)
    VOS_UINT16                          ulTtfMemNum;
    TTF_MEM_ST                         *pMem;
    VOS_UINT16                          usAmdPduSn;
#endif


    pstTtfMemPoolInfo = TTF_MEM_GET_POOL(ucPoolId);

    if (PS_TRUE != pstTtfMemPoolInfo->usUsedFlag)
    {
        vos_printf("Pool %d not uesed ! \n", ucPoolId);

        return;
    }

    vos_printf("Pool %d Mem Blk used Cnt = %d \n", ucPoolId, pstTtfMemPoolInfo->usTtfMemUsedCnt );

    if( PS_FALSE == g_ucTtfMemPrintDetail)
    {
        return;
    }

#if (FEATURE_ON == FEATURE_TTF_MEM_DEBUG)
    for (ulTtfMemNum = 0;
              ulTtfMemNum < pstTtfMemPoolInfo->usTtfMemTotalCnt;
              ulTtfMemNum ++)
    {
        pMem = pstTtfMemPoolInfo->pstTtfMemStStartAddr + ulTtfMemNum;

        if (VOS_NULL_PTR == pMem->pstDbgInfo)
        {
            vos_printf("Pool ID %u Blk Mem %u debug info is null", pMem->ucPoolId, ulTtfMemNum);
            continue;
        }

        if (TTF_BLK_MEM_STATE_FREE != pMem->pstDbgInfo->enMemStateFlag)
        {
            vos_printf("Alloc: File %4d L %5d T %11u, Blk Mem Level: %4u, Status: %d, TraceFile:%4d, TraceLine:%5d, TraceTick:%11d, \n ",
                (VOS_INT32)pMem->pstDbgInfo->usAllocFileId,
                (VOS_INT32)pMem->pstDbgInfo->usAllocLineNum,
                pMem->pstDbgInfo->ulAllocTick,
                ulTtfMemNum,
                pMem->pstDbgInfo->enMemStateFlag,
                (VOS_INT32)pMem->pstDbgInfo->usTraceFileId,
                (VOS_INT32)pMem->pstDbgInfo->usTraceLineNum,
                pMem->pstDbgInfo->ulTraceTick);

            if ( TTF_MEM_POOL_ID_EXT == ucPoolId )
            {
              /* ��ӡָ���ַ */
              vos_printf("ExtBuffAddr ptr: \n");
              vos_printf("%d \n", (VOS_UINT32)pMem->pExtBuffAddr);
            }
            else
            {
                /* ����PDU������RLC SN,  */
                if ( 0x80 == (pMem->pData[0]&0x80) )
                {
                    usAmdPduSn = ((VOS_UINT16)(pMem->pData[0] & 0x7f) << 5) | (VOS_UINT16)((pMem->pData[1] & 0xf8) >> 3);
                    vos_printf("Sn: %4d; \n", usAmdPduSn);
                }
                else
                {
                    vos_printf("Sn: null; \n");
                }
            }


        }

    }
#endif

}/* TTF_PrintBlkMemPoolUsedInfo */


/*****************************************************************************
 �� �� ��  : TTF_ShowUsedBlkMemInfo
 ��������  : ��ʾTTF_MEMͳ����Ϣ
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2007��8��2��
    ��    ��   : liukai
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID TTF_ShowUsedBlkMemInfo(VOS_VOID)
{
    VOS_UINT32                           ulPoolId;


    vos_printf("TTF_ShowUsedBlkMemInfo:Begin.\r\n");

    for (ulPoolId = 0; ulPoolId < TTF_MEM_POOL_ID_BUTT; ulPoolId++)
    {

        vos_printf("TTF_ShowUsedBlkMemInfo print pool %d info begin: \n", ulPoolId );
        TTF_PrintBlkMemPoolUsedInfo((VOS_UINT8)ulPoolId);
        vos_printf("TTF_ShowUsedBlkMemInfo print pool %d info end! \n", ulPoolId);
    }

    return ;
}



VOS_VOID TTF_MemPrintDetail(VOS_UINT8 ucChoice)
{
    g_ucTtfMemPrintDetail = ucChoice;

    return;
}


VOS_VOID TTF_ShowTtfUncacheMemInfo(VOS_VOID)
{


    return ;
}




TTF_MEM_ST *TTF_MemBlkSingleFree_Debug(VOS_UINT16 usFileID, VOS_UINT16 usLineNum,TTF_MEM_ST *pstTtfMem)
{
    TTF_MEM_POOL_STRU                  *pstTtfMemPool;
    TTF_MEM_CLUSTER_STRU               *pstTtfMemCluster;
    VOS_INT32                           lLockKey;
    TTF_MEM_ST                         *pRtnMem;
    VOS_VOID                           *pExtMem = VOS_NULL_PTR;

    pRtnMem = pstTtfMem->pNext;

    if ( TTF_MEM_POOL_ID_BUTT <= pstTtfMem->ucPoolId )
    {
        TTF_MEM_FREE_SAVE_INVALID_MEM_INFO(g_stTtfMemFreeMntnEntity.astTtfMemFreeInvalidMemInfo,pstTtfMem,TTF_INVALID_MEM_TYPE_POOLID,pstTtfMem->ucPoolId,usFileID,usLineNum);
        TTF_LOG3(WUEPS_PID_RLC, PS_PRINT_ERROR, "ERROR: TTF_MemBlkSingleFree_Debug, Poolid %d is invalid! FileID %d, LineNum %d \n",
            (VOS_INT32)pstTtfMem->ucPoolId, usFileID, usLineNum);
        return VOS_NULL_PTR;
    }

    pstTtfMemPool = TTF_MEM_GET_POOL(pstTtfMem->ucPoolId);

    if (PS_TRUE != pstTtfMemPool->usUsedFlag)
    {
        TTF_LOG3(WUEPS_PID_RLC, PS_PRINT_WARNING, "TTF_MemBlkSingleFree_Debug, Poolid %d is not used! FileID %d, LineNum %d \n",
            (VOS_INT32)pstTtfMem->ucPoolId, usFileID, usLineNum);
        return pRtnMem;
    }

#if (FEATURE_ON == FEATURE_TTF_MEM_DEBUG)

    if ( PS_SUCC != TTF_MemFreeSaveDebugInfo(usFileID, usLineNum, pstTtfMem) )
    {
        TTF_LOG2(WUEPS_PID_RLC, PS_PRINT_WARNING, "TTF_MemBlkSingleFree_Debug, TTF_MemFreeSaveDebugInfo fail! FileID %d, LineNum %d \n",
                    usFileID, usLineNum);
        return pRtnMem;
    };

#endif

    pstTtfMemCluster = &(pstTtfMemPool->astClusterTable[pstTtfMem->ucClusterId]);

    if ( TTF_MEM_POOL_ID_EXT == pstTtfMem->ucPoolId )
    {
        pExtMem = pstTtfMem->pExtBuffAddr;
        pstTtfMem->pExtBuffAddr = VOS_NULL_PTR;
        /* Ϊ�˺�����ȷά��pstTtfMemPool->ulTtfMemUsedByteSize��ֵ */
        pstTtfMem->usLen        = 0;
        pstTtfMem->pData        = pstTtfMem->pOrigData;
    }

    lLockKey = VOS_SplIMP();

    if (pstTtfMemCluster->usFreeCnt >= pstTtfMemCluster->usTotalCnt)
    {
        VOS_Splx(lLockKey);
        return pRtnMem;
    }

    pstTtfMemCluster->apstFreeStack[pstTtfMemCluster->usFreeCnt] = pstTtfMem;
    pstTtfMemPool->usTtfMemUsedCnt--;
    pstTtfMemCluster->usFreeCnt++;

    pstTtfMemPool->ulTtfMemUsedByteSize -= (pstTtfMem->usLen + TTF_MEM_GET_HDR_RSV_LEN(pstTtfMem));
    VOS_Splx(lLockKey);

    if (( VOS_NULL_PTR != pstTtfMemPool->pMemExtFreeFunc )&&( VOS_NULL_PTR != pExtMem ))
    {
        /* �ͷ��ⲿ�ڴ� */
        pstTtfMemPool->pMemExtFreeFunc(pExtMem);
    }

    if (VOS_NULL_PTR != pstTtfMemPool->pMemFreeEvent)
    {
        pstTtfMemPool->pMemFreeEvent(pstTtfMemPool->usTtfMemUsedCnt, pstTtfMemPool->ulTtfMemUsedByteSize);
    }

    return pRtnMem;
} /* TTF_MemBlkSingleFree_Debug */

TTF_MEM_ST *TTF_MemBlkCopyAllocFromMem_Debug
(
    VOS_UINT16 usFileID, VOS_UINT16 usLineNum, VOS_UINT32 ulPid,
    VOS_UINT32 ulPoolId, TTF_MEM_ST *pMemSrc, VOS_UINT16 usOffset,
    VOS_UINT16 usLen
)
{
    TTF_MEM_ST *pCurrMem        = pMemSrc;
    VOS_UINT16  usCurrOffset    = usOffset;
    VOS_UINT16  usCopyLen       = 0;
    VOS_UINT16  usWorkedLen     = 0;
    VOS_UINT16  usMemSrcLen;
    TTF_MEM_ST *pMem            = VOS_NULL_PTR;


    /*====================*/ /* ������� */
    if (VOS_NULL_PTR == pMemSrc)
    {
        PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning:Input Par pMemSrc is Null!");
        return VOS_NULL_PTR;
    }

    if ( 0 == usLen )
    {
        PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning:Input Par usLen is 0!");
        return VOS_NULL_PTR;
    }

    /*====================*/ /*�ж�TTF�ڴ��ĳ����Ƿ����Ҫ��*/
    usMemSrcLen = TTF_MemGetLen(ulPid, pMemSrc);

    if ( usMemSrcLen < (usOffset + usLen) )
    {
        PS_LOG2(ulPid, 0, PS_PRINT_WARNING, "Warning:MemSrcLen <1> Less Than (Offset + Len) <2>!",
            usMemSrcLen, (usOffset + usLen));
        return VOS_NULL_PTR;
    }

    /*================*/ /* Ѱ�ҵ�ƫ�ƺ�ĵ�һ��TTF_MEM */
    while (VOS_NULL_PTR != pCurrMem)
    {
        /*============*/ /* Ҫ��������ݲ��ڸÿ��ڣ�����һ����м��� */
        if (pCurrMem->usUsed <= usCurrOffset)
        {
            usCurrOffset   -= pCurrMem->usUsed;
            pCurrMem        = pCurrMem->pNext;
        }
        else
        {
            break;
        }
    }

    /*�������ϲ������ڴ�鳤�ȵļ�飬pCurrMem��Ӧ��ΪNULL*/
    if ( VOS_NULL_PTR == pCurrMem )
    {
        PS_LOG(ulPid, 0, PS_PRINT_WARNING,
            "Warning: TTF_MemBlkCopyAllocFromMem, pCurrMem is VOS_NULL_PTR!");
        return VOS_NULL_PTR;
    }


    /*====================*/ /* ����һ�鳤��ΪusLen��TTF�ڴ�� */
    pMem = TTF_MemBlkAlloc_Debug(usFileID, usLineNum, ulPid, ulPoolId, usLen);

    if (VOS_NULL_PTR == pMem)
    {
        PS_LOG(ulPid, 0, PS_PRINT_ERROR, "Error: MemCopy, Alloc Mem is Fail!");
        return VOS_NULL_PTR;
    }


    /*================*/ /* ��һ��Ҫ����ƫ�ƣ����⴦�� */
    usCopyLen   = PS_MIN(pCurrMem->usUsed - usCurrOffset, usLen);
    DRV_RT_MEMCPY(pMem->pData, pCurrMem->pData + usCurrOffset, usCopyLen);
    usWorkedLen = usCopyLen;

    /*================*/ /*  ��������Դ�ͷ��ʼ��������ƫ�� */
    pCurrMem        = pCurrMem->pNext;

    while (usWorkedLen < usLen)
    {
        /*�������ϲ������ڴ�鳤�ȵļ�飬pCurrMem��Ӧ��ΪNULL*/
        usCopyLen   = PS_MIN(pCurrMem->usUsed, usLen - usWorkedLen);
        DRV_RT_MEMCPY(pMem->pData + usWorkedLen, pCurrMem->pData, usCopyLen);
        usWorkedLen    += usCopyLen;
        pCurrMem        = pCurrMem->pNext;
    }

    /*����used����*/
    pMem->usUsed = usLen;

    return pMem;
}   /* TTF_MemBlkCopyAllocFromMem_Debug */


/*****************************************************************************
 Prototype       : TTF_MemBlkCopyAlloc_Debug
 Description     : ����һ���Լ�ģ������TTF�ڴ�飬���������Ѿ���д��ɵ���������
 Input           : ulPid       -- ���ô˺�����ģ���Pid
                   pPdu        -- �Ѿ������д������ͷָ��
                   usLen       -- Ҫ���Ƶ����ݿ鳤��,��λΪ�ֽ�
 Output          : NONE
 Return Value    : TTF_MEM_ST* -- ָ�����õĿ��ָ��
                                  ��VOS_NULL_PTR��ʾʧ��

 History         :
    Date         : 2009-12-13
    Author       : dengqingshan
*****************************************************************************/
TTF_MEM_ST *TTF_MemBlkCopyAlloc_Debug(VOS_UINT16 usFileID, VOS_UINT16 usLineNum,
                    VOS_UINT32 ulPid, VOS_UINT32 ulPoolId, VOS_UINT8 *pData, VOS_UINT16 usLen)
{
    TTF_MEM_ST                         *pMem = VOS_NULL_PTR;
    TTF_MEM_POOL_STRU                  *pstTtfMemPool;


    /*====================*/ /* ������� */
    if (VOS_NULL_PTR == pData)
    {
        PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning:TTF_MemBlkCopyAlloc_Debug MemCopy pData is NULL!\n");
        return VOS_NULL_PTR;
    }

    if (0 == usLen)
    {
        PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning:TTF_MemBlkCopyAlloc_Debug Alloc MemCopy Len is 0!\n");
        return VOS_NULL_PTR;
    }

    if( ulPoolId >= TTF_MEM_POOL_ID_BUTT )
    {
        PS_LOG1(ulPid, 0, PS_PRINT_WARNING, "TTF_MemBlkAlloc_Debug,Poolid <1> is invalid!",
            (VOS_INT32)ulPoolId);
        return VOS_NULL_PTR;
    }

    /*================================*/    /* ��ȡ�ڴ�� */
    pstTtfMemPool = TTF_MEM_GET_POOL(ulPoolId);

    if (PS_TRUE != pstTtfMemPool->usUsedFlag)
    {
        PS_LOG1(ulPid, 0, PS_PRINT_WARNING, "TTF_MemBlkAlloc_Debug,Pool<1> is not used!",
            (VOS_INT32)ulPoolId);
        return VOS_NULL_PTR;
    }

    /*====================*/ /* ����һ�鳤��ΪusLen��TTF�ڴ�� */
    pMem = TTF_MemBlkAlloc_Debug(usFileID, usLineNum, ulPid, ulPoolId, usLen);

    if (VOS_NULL_PTR == pMem)
    {
        PS_LOG(ulPid, 0, PS_PRINT_ERROR, "Error: MemCopy, Alloc Mem is Fail!\n");
        return VOS_NULL_PTR;
    }

    /*====================*/ /* �����Ѿ���д��ɵ��������� */
    DRV_RT_MEMCPY(pMem->pData, pData, usLen);/*COPY���ڴ����ݲ���*/

    /*����used����*/
    pMem->usUsed = usLen;

    return pMem;
} /* TTF_MemBlkCopyAlloc_Debug */


/*****************************************************************************
 Prototype       : TTF_MemBlkCopyAllocPacket_Debug
 Description     : ����һ���Լ�ģ������TTF�ڴ�飬���������Ѿ���д��ɵ���������
 Input           : ulPid       -- ���ô˺�����ģ���Pid
                   pPdu        -- �Ѿ������д������ͷָ��
                   usLen       -- Ҫ���Ƶ����ݿ鳤��,��λΪ�ֽ�
 Output          : NONE
 Return Value    : TTF_MEM_ST* -- ָ�����õĿ��ָ��
                                  ��VOS_NULL_PTR��ʾʧ��

 History         :
    Date         : 2009-12-13
    Author       : dengqingshan
*****************************************************************************/
TTF_MEM_ST *TTF_MemBlkCopyAllocPacket_Debug(VOS_UINT16 usFileID, VOS_UINT16 usLineNum,
                    VOS_UINT32 ulPid, VOS_UINT32 ulPoolId, VOS_UINT8 *pData, VOS_UINT16 usLen)
{
    TTF_MEM_POOL_STRU                  *pstTtfMemPool;
    TTF_MEM_ST                         *pMem;
    TTF_MEM_ST                         *pTailMem;
    VOS_UINT16                          usCopyLen;
    VOS_UINT16                          usWorkedLen;


    /*====================*/ /* ������� */
    if (VOS_NULL_PTR == pData)
    {
        PS_LOG2(ulPid, 0, PS_PRINT_WARNING,
            "TTF_MemBlkCopyAllocPacket_Debug, WARNING, SrcData is NULL! FileId <1>, LineNum <2>",
            (VOS_INT32)usFileID, (VOS_INT32)usLineNum);
        return VOS_NULL_PTR;
    }

    if (0 == usLen)
    {
        PS_LOG2(ulPid, 0, PS_PRINT_WARNING,
            "TTF_MemBlkCopyAllocPacket_Debug, WARNING, Len is 0! FileId <1>, LineNum <2>",
            (VOS_INT32)usFileID, (VOS_INT32)usLineNum);
        return VOS_NULL_PTR;
    }

    if ( ulPoolId >= TTF_MEM_POOL_ID_BUTT )
    {
        PS_LOG3(ulPid, 0, PS_PRINT_WARNING,
            "TTF_MemBlkCopyAllocPacket_Debug, WARNING, Poolid <1> is invalid! FileId <2>, LineNum <3>",
            (VOS_INT32)ulPoolId, (VOS_INT32)usFileID, (VOS_INT32)usLineNum);
        return VOS_NULL_PTR;
    }

    /*================================*/    /* ��ȡ�ڴ�� */
    pstTtfMemPool = TTF_MEM_GET_POOL(ulPoolId);

    if (PS_TRUE != pstTtfMemPool->usUsedFlag)
    {
        PS_LOG3(ulPid, 0, PS_PRINT_WARNING,
            "TTF_MemBlkCopyAllocPacket_Debug, WARNING, Pool<1> is not used!, FileId <2>, LineNum <3>",
            (VOS_INT32)ulPoolId, (VOS_INT32)usFileID, (VOS_INT32)usLineNum);
        return VOS_NULL_PTR;
    }

    /*================================*/    /* �����ڴ棬������ */
    usWorkedLen = 0;
    pMem        = VOS_NULL_PTR;

    while (usWorkedLen < usLen)
    {
        usCopyLen   = PS_MIN( (usLen - usWorkedLen), pstTtfMemPool->usMaxByteLen );

        pTailMem = TTF_MemBlkAlloc_Debug(usFileID, usLineNum, ulPid, ulPoolId, usCopyLen);

        if (VOS_NULL_PTR == pTailMem)
        {
            PS_LOG2(ulPid, 0, PS_PRINT_WARNING,
                "TTF_MemBlkCopyAllocPacket_Debug, WARNING, AllocMem Fail, FileId <1>, LineNum <2>",
                (VOS_INT32)usFileID, (VOS_INT32)usLineNum);

            TTF_MemFree_Debug(usFileID, usLineNum, ulPid, &pMem);
            return VOS_NULL_PTR;
        }

        DRV_RT_MEMCPY(pTailMem->pData, pData + usWorkedLen, usCopyLen);
        pTailMem->usUsed    = usCopyLen;
        usWorkedLen        += usCopyLen;

        TTF_MemLink(ulPid, &pMem, pTailMem);
    }

    return pMem;
} /* TTF_MemBlkCopyAllocPacket_Debug */


/*****************************************************************************
 �� �� ��  : TTF_MemBlkCheckPoolLeak
 ��������  : �����ṩ�ӿ�, �ж�TTF BLK MEM�Ƿ���й©
 �������  : VOS_UINT32 ulPoolId �����ڴ��ID
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2008��7��22��
    ��    ��   : zengfei 57034
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32  TTF_MemBlkCheckPoolLeak(VOS_UINT32 ulPoolId)
{
    TTF_MEM_POOL_STRU              *pstTtfMemPool;


    if ( ulPoolId >= TTF_MEM_POOL_ID_BUTT )
    {
        TTF_LOG1(WUEPS_PID_RLC, PS_PRINT_WARNING, "TTF_MemBlkCheckPoolLeak,Poolid %d is invalid!\n",
            (VOS_INT32)ulPoolId);
        return PS_FALSE;
    }

    pstTtfMemPool = TTF_MEM_GET_POOL(ulPoolId);

    if (PS_TRUE != pstTtfMemPool->usUsedFlag)
    {
        TTF_LOG1(WUEPS_PID_RLC, PS_PRINT_WARNING, "TTF_MemBlkCheckPoolLeak,Pool %d is not used!\n",
            (VOS_INT32)ulPoolId);
        return PS_FALSE;
    }

    if (0 == pstTtfMemPool->usTtfMemUsedCnt)
    {
        return PS_FALSE;
    }

    TTF_LOG2(WUEPS_PID_RLC, PS_PRINT_ERROR, "TTF_MEM_BLK Is Leak,Pool %d,UsedCnt %d \n!",
        (VOS_INT32)ulPoolId, pstTtfMemPool->usTtfMemUsedCnt);

    TTF_BlkMemUsedInfoEventRpt(WUEPS_PID_RLC, ulPoolId, TRIG_TYPE_LEAK);

    return PS_TRUE;
} /* TTF_MemBlkCheckPoolLeak */

VOS_VOID TTF_MemSndNormalEvent(VOS_UINT8 ucPoolId)
{
    VOS_UINT32                              ulLength;
    VOS_UINT32                              ulDataLen;
    PS_OM_EVENT_IND_STRU                   *pstTtfOmEventInd = VOS_NULL_PTR;
    TTF_OM_EVENT_BLK_MEM_USED_INFO_STRU    *pstTtfOmEventBlkMemUsedInfo;


    ulDataLen           = offsetof(TTF_OM_EVENT_BLK_MEM_USED_INFO_STRU, astBlkMemTraceInfo);

    /*��Ϣ�ܳ���*/
    ulLength            = offsetof(PS_OM_EVENT_IND_STRU, aucData) + ulDataLen;

    /*������Ϣ�ڴ�*/
    pstTtfOmEventInd    = (PS_OM_EVENT_IND_STRU *)PS_MEM_ALLOC(WUEPS_PID_RLC, ulLength);

    if (VOS_NULL_PTR == pstTtfOmEventInd )
    {
        PS_LOG(WUEPS_PID_RLC, 0, PS_PRINT_ERROR, "TTF_MemBlkCheckPoolLeak: MemAlloc Fail!");
        return ;
    }

    /*��д��Ϣ����*/
    pstTtfOmEventInd->ulLength      = ulLength - offsetof( PS_OM_EVENT_IND_STRU, usEventId);
    pstTtfOmEventInd->usEventId     = TTF_EVENT_BLK_MEM_USED_INFO;
    pstTtfOmEventInd->ulModuleId    = WUEPS_PID_RLC;/* ����EventĿǰֻ�ܶ�Ӧһ���̶���PID���ڴ�ֻ�����ݶ�ΪRLC PID */

    pstTtfOmEventBlkMemUsedInfo     = (TTF_OM_EVENT_BLK_MEM_USED_INFO_STRU *)pstTtfOmEventInd->aucData;
    pstTtfOmEventBlkMemUsedInfo->ucPoolId       = ucPoolId;
    pstTtfOmEventBlkMemUsedInfo->enTrigType     = TRIG_TYPE_NORMAL;
    pstTtfOmEventBlkMemUsedInfo->ucNo           = 1;
    pstTtfOmEventBlkMemUsedInfo->usRptCnt       = 0;
    pstTtfOmEventBlkMemUsedInfo->ulTotalUsedCnt = 0;

    /*����OM���¼��ϱ��ӿ�*/
    if ( PS_SUCC != OM_Event((PS_OM_EVENT_IND_STRU*)pstTtfOmEventInd))
    {
        PS_LOG(WUEPS_PID_RLC, 0, PS_PRINT_ERROR, "TTF_MemBlkCheckPoolLeak: OM Send Event Fail!");
    }
    /*�ͷ�������ڴ�*/
    PS_MEM_FREE(WUEPS_PID_RLC, pstTtfOmEventInd);
    return;


}



VOS_UINT32 TTF_MemReportMemLeakMsg( VOS_UINT32 ulPid )
{
    TTF_MEM_LEAK_INFO_IND_MSG          *pstTtfMemLeakInfoInd;

    /*������Ϣ  */
    pstTtfMemLeakInfoInd = (TTF_MEM_LEAK_INFO_IND_MSG *)PS_ALLOC_MSG_WITH_HEADER_LEN(
                                               ulPid,
                                               sizeof(TTF_MEM_LEAK_INFO_IND_MSG));

    /* �ڴ�����ʧ�ܣ����� */
    if( VOS_NULL_PTR == pstTtfMemLeakInfoInd )
    {
        return VOS_ERR;
    }

    /*��д��Ϣ����*/
    pstTtfMemLeakInfoInd->ulReceiverPid     = ulPid;
    pstTtfMemLeakInfoInd->ulMsgId           = ID_TRACE_TTF_MEM_LEAK_IND;

    /* ������Ϣ */
    TTF_TRACE_MSG(pstTtfMemLeakInfoInd);

    PS_FREE_MSG(ulPid, pstTtfMemLeakInfoInd);

    return VOS_OK;

}


/*****************************************************************************
 �� �� ��  : TTF_MemBlkCheckLeak
 ��������  : �����ṩ�ӿ�, �ж�TTF BLK MEM�Ƿ���й©
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2008��7��22��
    ��    ��   : zengfei 57034
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32  TTF_MemBlkCheckLeak(VOS_VOID)
{
    VOS_UINT32                          ulPoolId = TTF_MEM_POOL_ID_UL_DATA;
    VOS_UINT32                          ulRet    = PS_FALSE;
    VOS_UINT32                          ulFinal = PS_FALSE;


    ulRet = TTF_MemBlkCheckPoolLeak(ulPoolId);

    if (ulRet == PS_TRUE)
    {
        ulFinal = PS_TRUE;
    }

    ulRet = TTF_MemBlkCheckPoolLeak(TTF_MEM_POOL_ID_EXT);

    if ( ulRet == PS_TRUE)
    {
        ulFinal = PS_TRUE;
    }

    return ulFinal;
} /* TTF_MemBlkCheckLeak */

VOS_VOID *TTF_MemAllocCtrlMem(VOS_UINT32 ulLength)
{
    return VOS_CacheMemAlloc(ulLength);
}


VOS_UINT32 TTF_MemPoolInit
(
    VOS_UINT8                            ucPoolId,
    VOS_UINT8                          **ppucDataMemAddr
)
{
    TTF_MEM_POOL_STRU                  *pstTtfMemPool;
    TTF_MEM_ST                        **ppst1stTtfMem;
    TTF_MEM_ST                         *pstTtfMem;
    TTF_MEM_CLUSTER_STRU               *pstTtfMemCluster;
    VOS_UINT32                          ulTtfMemCnt;
    VOS_UINT8                           ucClusterId;
    TTF_MEM_ST                        **ppTempMem;
#if (FEATURE_ON == FEATURE_TTF_MEM_DEBUG)
    TTF_BLK_MEM_DEBUG_ST               *pstMemDbInfo;
#endif


    pstTtfMemPool = TTF_MEM_GET_POOL(ucPoolId);

    if (PS_TRUE == pstTtfMemPool->usUsedFlag)
    {
        return PS_SUCC;
    }

    /*================================*/
    /* �����Ӧ�ڴ�صĿ����ڴ�������ÿ��POOL����һ�Σ�����ָ��ÿ��clusterʹ�� */
    ppst1stTtfMem = (TTF_MEM_ST **)TTF_MemAllocCtrlMem(sizeof(TTF_MEM_ST *) * pstTtfMemPool->usTtfMemTotalCnt);

    if (VOS_NULL_PTR == ppst1stTtfMem)
    {
        return PS_FAIL;
    }

    /*================================*/
    /* �����Ӧ�ڴ�ص�TTF_MEM��ÿ��POOL����һ�Σ�����ָ��ÿ��clusterʹ�� */
    pstTtfMem = (TTF_MEM_ST *)TTF_MemAllocCtrlMem(sizeof(TTF_MEM_ST) * pstTtfMemPool->usTtfMemTotalCnt);

    if (VOS_NULL_PTR == pstTtfMem)
    {
        return PS_FAIL;
    }


#if (FEATURE_ON == FEATURE_TTF_MEM_DEBUG)
    /* ����TTF_MEM��Ӧ��TTF_BLK_MEM_DEBUG_ST */
    pstMemDbInfo = (TTF_BLK_MEM_DEBUG_ST *)TTF_MemAllocCtrlMem(sizeof(TTF_BLK_MEM_DEBUG_ST) * pstTtfMemPool->usTtfMemTotalCnt);

    if (VOS_NULL_PTR == pstMemDbInfo)
    {
        return PS_FAIL;
    }
#endif

    memset(ppst1stTtfMem, 0, sizeof(TTF_MEM_ST *) * pstTtfMemPool->usTtfMemTotalCnt);
    memset(pstTtfMem, 0, sizeof(TTF_MEM_ST) * pstTtfMemPool->usTtfMemTotalCnt);

#if (FEATURE_ON == FEATURE_TTF_MEM_DEBUG)
    memset(pstMemDbInfo, 0, sizeof(TTF_BLK_MEM_DEBUG_ST) * pstTtfMemPool->usTtfMemTotalCnt);
#endif

    ppTempMem = ppst1stTtfMem;

    /*================================*/    /* ��¼���ڴ������TTF_MEMָ�룬����ڴ�й¶ʹ�� */
    pstTtfMemPool->pstTtfMemStStartAddr    = pstTtfMem;


    /*================================*/    /* ����ÿ��cluster��TTF_MEM */
    for (ucClusterId = 0; ucClusterId < pstTtfMemPool->ucClusterCnt; ucClusterId++)
    {
        pstTtfMemCluster    = &(pstTtfMemPool->astClusterTable[ucClusterId]);

        /* ��������������Ϳ����ڴ�������Ĺ�ϵ */
        pstTtfMemCluster->apstFreeStack = ppTempMem;

        for (ulTtfMemCnt = 0;
             ulTtfMemCnt < pstTtfMemCluster->usFreeCnt;
             ulTtfMemCnt ++)
        {
            TTF_MEM_INIT(pstTtfMem, TTF_MEM_TYPE_BLK, pstTtfMemCluster->usLen);

            pstTtfMem->ucPoolId     = ucPoolId;

            if ( TTF_MEM_POOL_ID_EXT == pstTtfMem->ucPoolId )
            {
                pstTtfMem->pData = VOS_NULL_PTR;
            }
            else
            {
                pstTtfMem->pData        = *ppucDataMemAddr; /* ����TTF_MEM_ST�Ͷ�Ӧ���ݵĹ�ϵ */
            }

            pstTtfMem->ucClusterId  = ucClusterId;
            pstTtfMem->pOrigData    = pstTtfMem->pData;
#if (FEATURE_ON == FEATURE_TTF_MEM_DEBUG)
            pstTtfMem->pstDbgInfo   = pstMemDbInfo;
            pstMemDbInfo++;
#endif
             /* ���������ڴ��������TTF_MEM_ST�Ĺ�ϵ */

           *ppTempMem               = pstTtfMem;

            pstTtfMem++;
            ppTempMem++;

            *ppucDataMemAddr       += pstTtfMemCluster->usLen;
        }

    }

    return PS_SUCC;
} /* TTF_MemPoolInit */

VOS_UINT32 TTF_MemPoolCreate
(
    VOS_UINT8                     ucPoolId,
    TTF_MEM_POOL_CFG_NV_STRU     *pstPoolCfg,
    VOS_UINT8                   **ppucDataMemAddr,
    VOS_UINT32                   *pulLength
)
{
    TTF_MEM_POOL_STRU                  *pstTtfMemPool;
    TTF_MEM_CLUSTER_STRU               *pstMemCluster;
    VOS_UINT32                          usLastClusterMaxLen;
    VOS_UINT32                          usDataLen;
    VOS_UINT8                           ucClusterId;
    VOS_UINT8                          *pucDataMemAddr;
    VOS_UINT8                           ucClusterCnt = 0;
    VOS_UINT32                          ulMemSize;

    pucDataMemAddr  = (VOS_UINT8 *)(*ppucDataMemAddr);

    pstTtfMemPool   = TTF_MEM_GET_POOL(ucPoolId);

    /* ����ӦID���ڴ���Ƿ��б��ظ����� */
    if (PS_TRUE == pstTtfMemPool->usUsedFlag)
    {
        PS_LOG1(WUEPS_PID_RLC, 0, PS_PRINT_ERROR, "TTF_MemPoolCreate Fail, ulPoolId %d is already used \n",
            ucPoolId);
        return PS_FAIL;
    }

    ucClusterCnt                    = pstPoolCfg->ucClusterCnt;

    /* ��ʼ���ڴ�ص�ͨ�ñ��� */
    pstTtfMemPool->usUsedFlag       = PS_FALSE;
    pstTtfMemPool->ucClusterCnt     = ucClusterCnt;
    pstTtfMemPool->usTtfMemTotalCnt = 0;

    pstTtfMemPool->usMaxByteLen     = pstPoolCfg->ausBlkSize[ucClusterCnt - 1];

    PS_MEM_SET(pstTtfMemPool->aucLenIndexTable, 0, TTF_MEM_BLK_MAX_BYTE_LEN);

    PS_MEM_SET(pstTtfMemPool->astClusterTable, 0,
        sizeof(TTF_MEM_CLUSTER_STRU) * TTF_MEM_MAX_CLUSTER_NUM);

    usLastClusterMaxLen   = 1;

    for (ucClusterId = 0; ucClusterId < ucClusterCnt; ucClusterId++)
    {
        pstMemCluster                   = &(pstTtfMemPool->astClusterTable[ucClusterId]);

        pstMemCluster->usTotalCnt       = pstPoolCfg->ausBlkCnt[ucClusterId];
        pstMemCluster->usFreeCnt        = pstPoolCfg->ausBlkCnt[ucClusterId];
        pstMemCluster->usLen            = pstPoolCfg->ausBlkSize[ucClusterId];

        ulMemSize = pstPoolCfg->ausBlkSize[ucClusterId] * pstPoolCfg->ausBlkCnt[ucClusterId] ;

        *pulLength       -= ulMemSize;
        *ppucDataMemAddr += ulMemSize;

        for(usDataLen = usLastClusterMaxLen;
            usDataLen <= pstPoolCfg->ausBlkSize[ucClusterId];
            usDataLen++)
        {
            pstTtfMemPool->aucLenIndexTable[usDataLen] = ucClusterId;
        }

        usLastClusterMaxLen                 = usDataLen;

        pstTtfMemPool->usTtfMemTotalCnt    += pstPoolCfg->ausBlkCnt[ucClusterId];
    }

    if(PS_SUCC != TTF_MemPoolInit(ucPoolId, &pucDataMemAddr))
    {
        return PS_FAIL;
    }

    pstTtfMemPool->usUsedFlag   = PS_TRUE;

    return PS_SUCC;
} /* TTF_MemPoolCreate */

VOS_UINT32 TTF_MemNvCfgTotalLength(TTF_MEM_SOLUTION_CFG_NV_STRU *pstTtfMemSolution)
{
    TTF_MEM_POOL_CFG_NV_STRU      *pstPoolCfg;
    VOS_UINT8                 ucLoop;
    VOS_UINT8                 ucClusterNum;
    VOS_UINT8                 ucClusterLoop;
    VOS_UINT32                ulTotalLength = 0;

    /*�����ڴ�ظ�cluster�Ƿ���ȷ*/
    for (ucLoop = 0; ucLoop < pstTtfMemSolution->ucPoolCnt; ucLoop++)
    {
        pstPoolCfg   = &pstTtfMemSolution->astTtfMemPoolCfgInfo[ucLoop];
        ucClusterNum = pstPoolCfg->ucClusterCnt;

        for(ucClusterLoop = 0 ; ucClusterLoop < ucClusterNum ; ucClusterLoop++)
        {
            ulTotalLength += pstPoolCfg->ausBlkSize[ucClusterLoop] * pstPoolCfg->ausBlkCnt[ucClusterLoop];
        }
    }

    return ulTotalLength;
}
VOS_UINT32 TTF_MemReadNvCfg(TTF_MEM_SOLUTION_CFG_NV_STRU *pstTtfMemSolution)
{
    VOS_UINT32                     ulRet;
    VOS_UINT8                      ucLoop;
    VOS_UINT8                      ucClusterNum;
    VOS_UINT8                      ucClusterLoop;
    TTF_MEM_POOL_CFG_NV_STRU      *pstPoolCfg;

    /*��ȡTTF�ڴ淽��*/
    ulRet = NV_Read (en_NV_Item_TTF_MEM_SOLUTION_CFG, pstTtfMemSolution , sizeof(TTF_MEM_SOLUTION_CFG_NV_STRU));
    if (NV_OK != ulRet)
    {
        PS_LOG1(WUEPS_PID_RLC, 0, PS_PRINT_ERROR, "TTF_MemReadNvCfg Fail, Read NV FAIL, Error Code <1>\n", (VOS_INT32)ulRet);
        return PS_FAIL;
    }

    if (TTF_MEM_POOL_ID_BUTT != pstTtfMemSolution->ucPoolCnt)
    {
        PS_LOG(WUEPS_PID_RLC, 0, PS_PRINT_ERROR, "TTF_MemReadNvCfg Fail, Pool Num Error \n");
        return PS_FAIL;
    }

    /*�����ڴ�ظ�cluster�Ƿ���ȷ*/
    for (ucLoop = 0; ucLoop < pstTtfMemSolution->ucPoolCnt; ucLoop++)
    {
        pstPoolCfg   = &pstTtfMemSolution->astTtfMemPoolCfgInfo[ucLoop];
        ucClusterNum = pstPoolCfg->ucClusterCnt;

        if (ucClusterNum > TTF_MEM_MAX_CLUSTER_NUM)
        {
            PS_LOG(WUEPS_PID_RLC, 0, PS_PRINT_ERROR, "TTF_MemReadNvCfg Fail, Cluster Num Error \n");
            return PS_FAIL;
        }

        for(ucClusterLoop = 0 ; ucClusterLoop < ucClusterNum ; ucClusterLoop++)
        {
            if (pstPoolCfg->ausBlkSize[ucClusterLoop] > TTF_MEM_BLK_MAX_BYTE_LEN)
            {
                PS_LOG(WUEPS_PID_RLC, 0, PS_PRINT_ERROR, "TTF_MemReadNvCfg Fail, Memory Size Error \n");
                return PS_FAIL;
            }
        }
    }


    return PS_SUCC;
}



VOS_VOID TTF_MemPrintSolutionCfg(VOS_VOID)
{

    TTF_MEM_SOLUTION_CFG_NV_STRU  *pstSolutionCfg = &g_stTtfMemSolution;
    TTF_MEM_POOL_CFG_NV_STRU      *pstPoolCfg;

    VOS_UINT8                     ucPoolCnt;
    VOS_UINT8                     ucPoolLoop;
    VOS_UINT8                     ucClusterCnt;
    VOS_UINT8                     ucClusterLoop;

    ucPoolCnt = pstSolutionCfg->ucPoolCnt;

    vos_printf("TTF Mem Pool Solution Print :\r\n");
    vos_printf("TTF Mem Pool Cnt %d \r\n", ucPoolCnt);

    for(ucPoolLoop = 0; ucPoolLoop < ucPoolCnt; ucPoolLoop++)
    {
        pstPoolCfg = &pstSolutionCfg->astTtfMemPoolCfgInfo[ucPoolLoop];

        ucClusterCnt = pstPoolCfg->ucClusterCnt;
        vos_printf("TTF Mem Pool [%d] Cluster Cnt %d \r\n", ucPoolLoop, ucClusterCnt);

        for (ucClusterLoop = 0; ucClusterLoop < ucClusterCnt; ucClusterLoop++)
        {
            vos_printf("TTF Mem Pool [%d] Cluster [%d] -- size [%d] cnt[%d]\r\n",
                ucPoolLoop, ucClusterLoop, pstPoolCfg->ausBlkSize[ucClusterLoop], pstPoolCfg->ausBlkCnt[ucClusterLoop]);
        }

    }

    vos_printf("TTF Mem Pool Solution Print END\r\n");
    return;
}


/*****************************************************************************
 �� �� ��  : TTF_MemBlkIsAlert
 ��������  : �����ṩ�ӿ�, �ж�TTF BLK MEMʣ�����Ƿ��������
 �������  : VOS_UINT32 ulPoolId    �����ڴ��ID
 �������  : ��
 �� �� ֵ  : PS_TRUE : TTF BLK MEMʣ������������
             PS_FALSE: TTF BLK MEMʣ������������
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2008��7��22��
    ��    ��   : zengfei 57034
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32  TTF_MemBlkIsAlert(VOS_UINT32 ulPoolId)
{
    VOS_UINT16 usFreeCnt = 0xFFFF;

    usFreeCnt = g_astTtfMemPool[ulPoolId].usTtfMemTotalCnt
                    - g_astTtfMemPool[ulPoolId].usTtfMemUsedCnt;

    if (usFreeCnt < TTF_BLK_MEM_ALERT_THLD)
    {
        TTF_BlkMemUsedInfoEventRpt(WUEPS_PID_RLC, ulPoolId, TRIG_TYPE_ALERT);
        return PS_TRUE;
    }

    return PS_FALSE;
}


/*****************************************************************************
 �� �� ��  : TTF_MemBlkAddHeadData_Debug
 ��������  : ��TTF_MEM��ͷ�����ָ���ֽ���Ŀ������
 �������  : ulPid       -- ���ô˺�����ģ���Pid
             ppMemDest   -- Ҫ��д��TTF�ڴ��ͷָ��
             pSrcData    -- Ŀ�����ݵ�ַ
             usAddLen    -- ������ݵĳ��ȣ���λΪ�ֽ�
 �������  : ��
 �� �� ֵ  : PS_SUCC -- �ɹ�; PS_FAIL -- ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2006��12��8��
    ��    ��   : �����壬����Ƽ
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 TTF_MemBlkAddHeadData_Debug
(
    VOS_UINT16 usFileID, VOS_UINT16 usLineNum, VOS_UINT32 ulPid,
    VOS_UINT32 ulPoolId, TTF_MEM_ST **ppMemDest, VOS_UINT8 *pSrcData,
    VOS_UINT16 usAddLen
)
{
    TTF_MEM_ST*         pDestMem;
    TTF_MEM_ST*         pHeaderMem;
    PS_BOOL_ENUM_UINT8  enAllocMem;     /*�Ƿ���Ҫ�������ڴ�ı�־λ��*/

    /*====================*/ /* ������� */
    if (VOS_NULL_PTR == ppMemDest)
    {
        PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning: Input Par ppMemDest is Null!\n");
        return PS_FAIL;
    }

    if (VOS_NULL_PTR == pSrcData)
    {
        PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning: Input Par pSrcData is Null!\n");
        return PS_FAIL;
    }

    if ( 0 == usAddLen )
    {
        PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning: Input Par usAddLen is 0!\n");
        return PS_FAIL;
    }

    pDestMem    = *ppMemDest;

    /*====================*/    /* �ж��Ƿ���Ҫ�����µ�TTF_MEM�鱣��ͷ�� */
    if (VOS_NULL_PTR == pDestMem)
    {
        /*����ǰTTF�ڴ�鲻���ڣ������붯̬���ݿ�*/
        enAllocMem    = PS_TRUE;
    }
    else
    {
        if (usAddLen >  (VOS_UINT16)(pDestMem->pData - pDestMem->pOrigData))
        {
            /*ͷ��Ԥ���ĳ��Ȳ���������ͷ����������ר���ڴ����ݿ�*/
            enAllocMem    = PS_TRUE;
        }
        else
        {
            enAllocMem    = PS_FALSE;
        }
    }

    if ( PS_TRUE == enAllocMem )
    {
        pHeaderMem    = TTF_MemBlkAlloc_Debug(usFileID, usLineNum, ulPid, ulPoolId, usAddLen);
        if (VOS_NULL_PTR == pHeaderMem)
        {
            PS_LOG(ulPid, 0, PS_PRINT_ERROR, "Error:Alloc pHeaderMem is Null!\n");
            return PS_FAIL;
        }

        DRV_RT_MEMCPY(pHeaderMem->pData, pSrcData, usAddLen);
        pHeaderMem->usUsed    = usAddLen;

        TTF_MemLink(ulPid, &pHeaderMem, pDestMem);
        *ppMemDest  = pHeaderMem;
    }
    else
    {
        PS_ASSERT_RTN((VOS_NULL_PTR != pDestMem), PS_FAIL);
        TTF_MEM_RESUME_TO_HDR(pDestMem, usAddLen);
        DRV_RT_MEMCPY(pDestMem->pData, pSrcData, usAddLen);
    }
    return PS_SUCC;
} /* TTF_MemBlkAddHeadData_Debug */


/*****************************************************************************
 �� �� ��  : TTF_MemBlkAddTailData_Debug
 ��������  : ��TTF_MEM��β�����ָ���ֽ���Ŀ������
 �������  : VOS_UINT32 ulFileID    �ļ���
             VOS_UINT32 ulFileID    �к�
             VOS_UINT32 ulPid       ���ô˺�����ģ���Pid
             TTF_MEM_ST **ppMemDest Ҫ��д��TTF�ڴ��ͷָ��
             VOS_UINT8  *pSrcData   Ŀ�����ݵ�ַ
             VOS_UINT16 usAddLen    ������ݵĳ��ȣ���λΪ�ֽ�
 �������  : ��
 �� �� ֵ  : PS_SUCC -- �ɹ�; PS_FAIL -- ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2006��12��8��
    ��    ��   : �����壬����Ƽ
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 TTF_MemBlkAddTailData_Debug
(
    VOS_UINT16 usFileID, VOS_UINT16 usLineNum, VOS_UINT32 ulPid,
    VOS_UINT32 ulPoolId, TTF_MEM_ST **ppMemDest, VOS_UINT8  *pSrcData,
    VOS_UINT16  usAddLen
)
{
    TTF_MEM_ST         *pDestMem;
    TTF_MEM_ST         *pTailMem;
    PS_BOOL_ENUM_UINT8  enAllocMem;

    /*====================*/    /* ������� */
    PS_ASSERT_RTN((VOS_NULL_PTR != ppMemDest), PS_FAIL);
    PS_ASSERT_RTN((VOS_NULL_PTR != pSrcData), PS_FAIL);
    PS_ASSERT_RTN((0 != usAddLen), PS_FAIL);

    pDestMem    = *ppMemDest;

    if (VOS_NULL_PTR == pDestMem)
    {
        enAllocMem      = PS_TRUE;
    }
    else
    {
        while (VOS_NULL_PTR != pDestMem->pNext)
        {
            pDestMem    = pDestMem->pNext;
        }

        if ( usAddLen >  (pDestMem->usLen - pDestMem->usUsed) )
        {
            enAllocMem    = PS_TRUE;
        }
        else
        {
            enAllocMem    = PS_FALSE;
        }
    }

    if ( PS_TRUE == enAllocMem )
    {
        pTailMem    = TTF_MemBlkAlloc_Debug(usFileID, usLineNum, ulPid, ulPoolId,usAddLen);
        if (VOS_NULL_PTR == pTailMem)
        {
            PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning: Alloc pTailMem is Null!\n");
            return PS_FAIL;
        }

        DRV_RT_MEMCPY(pTailMem->pData, pSrcData, usAddLen);
        pTailMem->usUsed    = usAddLen;

        TTF_MemLink(ulPid, &pDestMem, pTailMem);

        if (VOS_NULL_PTR == *ppMemDest)
        {
            *ppMemDest  = pDestMem;
        }

    }
    else
    {
        PS_ASSERT_RTN((VOS_NULL_PTR != pDestMem), PS_FAIL);
        DRV_RT_MEMCPY(&pDestMem->pData[pDestMem->usUsed], pSrcData, usAddLen);

        pDestMem->usUsed    += usAddLen;
    }

    return PS_SUCC;
} /* TTF_MemBlkAddTailData_Debug */

/*****************************************************************************
 �� �� ��  : TTF_MemBlkAddTailDataWithLastMem_Debug
 ��������  : ��TTF_MEM��β�����ָ���ֽ���Ŀ������
 �������  : ulFileID    -- File-ID
             ulLineNum   -- File Line
             ulPid       -- ���ô˺�����ģ���Pid
             ppMemDest   -- ���Ҫ��д��TTF�ڴ��ͷָ��ĵ�ַ
             pLastMem    -- Ҫ��д��TTF�ڴ��βָ��
             pSrcData    -- Ŀ�����ݵ�ַ
             usAddLen    -- ������ݵĳ��ȣ���λΪ�ֽ�
 �������  : ppMemDest   -- ���Ҫ��д��TTF�ڴ��ͷָ��ĵ�ַ
 �� �� ֵ  : PS_SUCC -- �ɹ�; PS_FAIL -- ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2008��2��2��
    ��    ��   : liukai
    �޸�����   : Created
*****************************************************************************/
VOS_UINT32 TTF_MemBlkAddTailDataWithLastMem_Debug
(
    VOS_UINT16 usFileID,VOS_UINT16 usLineNum, VOS_UINT32 ulPid,
    VOS_UINT32 ulPoodId,TTF_MEM_ST **ppMemDest, TTF_MEM_ST *pLastMem,
    VOS_UINT8  *pSrcData, VOS_UINT16  usAddLen
)
{
    TTF_MEM_ST *pDestMem;
    TTF_MEM_ST *pTailMem;
    PS_BOOL_ENUM_UINT8  enAllocMem;

    /*====================*/    /* ������� */
    PS_ASSERT_RTN((VOS_NULL_PTR != ppMemDest), PS_FAIL);
    PS_ASSERT_RTN((VOS_NULL_PTR != pSrcData), PS_FAIL);

    pDestMem    = *ppMemDest;

    if (VOS_NULL_PTR == pDestMem)
    {
        enAllocMem    = PS_TRUE;
    }
    else
    {
        pDestMem = pLastMem;

        /* �ж��Ƿ���Ҫ�����µ�TTF_MEM�鱣��ͷ�� */
        if (( TTF_MEM_TYPE_BLK == pDestMem->usType ) || ( TTF_MEM_TYPE_DYN == pDestMem->usType ))
        {
            /*ͷ��Ԥ���ĳ��Ȳ���������ͷ��*/
            if ( usAddLen >  (pDestMem->usLen - pDestMem->usUsed) )
            {
                enAllocMem    = PS_TRUE;
            }
            else
            {
                enAllocMem    = PS_FALSE;
            }
        }
        else
        {
            PS_LOG1(ulPid, 0, PS_PRINT_WARNING, "Warning: TTFMem Type <1> is Wrong!\n",
                pDestMem->usType);
            return PS_FAIL;
        }
    }

    if ( PS_TRUE == enAllocMem )
    {
        pTailMem    = TTF_MemBlkAlloc_Debug(usFileID, usLineNum, ulPid, ulPoodId, usAddLen);

        if (VOS_NULL_PTR == pTailMem)
        {
            PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning: Alloc pTailMem is Null!\n");
            return PS_FAIL;
        }

        DRV_RT_MEMCPY(pTailMem->pData, pSrcData, usAddLen);
        pTailMem->usUsed    = usAddLen;

        if (VOS_NULL_PTR != pLastMem)
        {
            pLastMem->pNext = pTailMem;  /* ��pMemTail������ȥ */
        }
        else
        {
            *ppMemDest = pTailMem;
        }
    }
    else
    {
        PS_ASSERT_RTN((VOS_NULL_PTR != pDestMem), PS_FAIL);
        DRV_RT_MEMCPY(&pDestMem->pData[pDestMem->usUsed], pSrcData, usAddLen);

        pDestMem->usUsed    += usAddLen;
    }

    return PS_SUCC;
}    /* TTF_MemBlkAddTailDataWithLastMem_Debug */


/*****************************************************************************
 �� �� ��  : TTF_MemSingleFree_Debug
 ��������  :
 �������  : VOS_INT8 *cFileName
             VOS_UINT32 ulLineNum
             VOS_UINT32 ulPid
             TTF_MEM_ST *pMem
 �������  : ��
 �� �� ֵ  : TTF_MEM_ST *
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2007��3��24��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
TTF_MEM_ST *TTF_MemSingleFree_Debug(VOS_UINT16 usFileID, VOS_UINT16 usLineNum,
                VOS_UINT32 ulPid, TTF_MEM_ST *pMem)
{
    TTF_MEM_ST         *pRtnMem;

    /* ������� */
    if ( VOS_NULL_PTR == pMem )
    {
        TTF_MEM_FREE_SAVE_NULL_PTR_INFO(g_stTtfMemFreeMntnEntity.astTtfMemFreeNullPtrInfo,usFileID,usLineNum);
        TTF_LOG2(ulPid, PS_PRINT_WARNING, "TTF_MemSingleFree_Debug, pMem is NULL! FileID %d, LineNum %d \n", usFileID, usLineNum);
        return  VOS_NULL_PTR;
    }

    if ( TTF_MEM_TYPE_BLK != pMem->usType )
    {
        TTF_MEM_FREE_SAVE_INVALID_MEM_INFO(g_stTtfMemFreeMntnEntity.astTtfMemFreeInvalidMemInfo,pMem,TTF_INVALID_MEM_TYPE_BLKTYPE,pMem->usType,usFileID,usLineNum);
        TTF_LOG3(ulPid, PS_PRINT_WARNING, "TTF_MemSingleFree_Debug, Mem Type %d is invalid! FileID %d, LineNum %d \n",
            (VOS_INT32)pMem->usType, usFileID, usLineNum);
        return  VOS_NULL_PTR;
    }

    /* �ͷ��ڴ� */
    pRtnMem = TTF_MemBlkSingleFree_Debug(usFileID, usLineNum, pMem);

    return pRtnMem;
}    /* TTF_MemSingleFree_Debug */


/*****************************************************************************
 �� �� ��  : TTF_MemFree_Debug
 ��������  :
 �������  : VOS_INT8 *cFileName
             VOS_UINT32 ulLineNum
             VOS_UINT32 ulPid
             TTF_MEM_ST *pMem
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2007��3��24��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID TTF_MemFree_Debug(VOS_UINT16 usFileID, VOS_UINT16 usLineNum,
             VOS_UINT32 ulPid, TTF_MEM_ST **ppMem)
{
    TTF_MEM_ST     *pFree;

    /*====================*/ /* ������� */
    if(VOS_NULL_PTR == ppMem)
    {
        TTF_MEM_FREE_SAVE_NULL_PTR_INFO(g_stTtfMemFreeMntnEntity.astTtfMemFreeNullPPtrInfo,usFileID,usLineNum);
        TTF_LOG2(ulPid, PS_PRINT_WARNING, "Warning: TTF_MEMFREE ppMem is NullPtr!ulFileID %d,ulLineNum %d", (VOS_INT32)usFileID, (VOS_INT32)usLineNum);
        return;
    }

    pFree   = *ppMem;
    if (VOS_NULL_PTR == *ppMem)
    {
        TTF_MEM_FREE_SAVE_NULL_PTR_INFO(g_stTtfMemFreeMntnEntity.astTtfMemFreeNullPtrInfo,usFileID,usLineNum);
        TTF_LOG2(ulPid, PS_PRINT_WARNING, "Warning: TTF_MEMFREE *ppMem is NullPtr!ulFileID %d,ulLineNum %d", (VOS_INT32)usFileID, (VOS_INT32)usLineNum);
        return;
    }

    while (VOS_NULL_PTR != pFree)
    {
        pFree = TTF_MemSingleFree_Debug(usFileID, usLineNum, ulPid, pFree);
    }

    *ppMem = VOS_NULL_PTR;

}    /* TTF_MemFree_Debug */

/*****************************************************************************
 �� �� ��  : TTF_MemBlkCutHeadData_Debug
 ��������  : ��TTF_MEM��ͷ����ȥָ���ֽ���Ŀ������
 �������  : ulPid    -- ���ô˺�����ģ���Pid
             pMemSrc  -- Ҫ��д��TTF�ڴ��ͷָ��
             pDest    -- Ŀ�����ݵ�ַ
             usLen    -- ��д���ݵĳ��ȣ���λΪ�ֽ�
 �������  : ��
 �� �� ֵ  : ��ȡ���ݳɹ�����PS_SUCC��ʧ�ܷ���PS_FAIL
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2006��12��8��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 TTF_MemBlkCutHeadData_Debug
(
    VOS_UINT16 usFileID, VOS_UINT16 usLineNum, VOS_UINT32 ulPid,
    TTF_MEM_ST **ppMemSrc, VOS_UINT8 *pDest, VOS_UINT16 usLen
)
{
    TTF_MEM_ST *pCurrMem;
    VOS_UINT16  usWorkedLen     = 0;
    VOS_UINT16  usMemSrcLen;

    /*====================*/ /* ������� */
    if (VOS_NULL_PTR == ppMemSrc)
    {
        PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning: Input Par ppMemSrc is Null!\n");
        return PS_FAIL;
    }

    if ( (VOS_NULL_PTR == *ppMemSrc) || (VOS_NULL_PTR == pDest) )
    {
        PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning: Input Par *ppMemSrc Or pDest is Null!\n");
        return PS_FAIL;
    }

    if ( 0 == usLen )
    {
        PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning: Input Par usLen is 0!\n");
        return PS_FAIL;
    }

    /*====================*/ /*�ж�TTF�ڴ��ĳ����Ƿ����Ҫ��*/
    pCurrMem        = *ppMemSrc;
    usMemSrcLen     = TTF_MemGetLen(ulPid, pCurrMem);

    if ( usMemSrcLen < usLen )
    {
        PS_LOG2(ulPid, 0, PS_PRINT_WARNING, "Warning: MemSrcLen <1> Less Than usLen <2>!\n",
            usMemSrcLen, usLen);
        return PS_FAIL;
    }

    /*====================*/ /*��ȡ���賤�ȵ�����*/
    while (VOS_NULL_PTR != pCurrMem)
    {
        if (pCurrMem->usUsed <= (usLen - usWorkedLen))
        {
            /*��ȡ����TTFMem������*/
            DRV_RT_MEMCPY(pDest + usWorkedLen, pCurrMem->pData, pCurrMem->usUsed);

            usWorkedLen    += pCurrMem->usUsed;
            pCurrMem        = TTF_MemSingleFree_Debug(usFileID, usLineNum, ulPid, pCurrMem);

            if ( usLen == usWorkedLen )
            {
                *ppMemSrc   = pCurrMem;
                return PS_SUCC;
            }
        }
        else
        {
            /*��ȡ��ǰTTFMem��Ӧ���ȵ�����*/
            DRV_RT_MEMCPY(pDest + usWorkedLen, pCurrMem->pData, (usLen - usWorkedLen));
            TTF_MEM_REMOVE_FROM_HDR(pCurrMem, (usLen - usWorkedLen));
            usWorkedLen     = usLen;
            *ppMemSrc   = pCurrMem;
            return PS_SUCC;
        }
    }

    PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning: pCurrMem is Null!\n");
    return PS_FAIL;
} /* TTF_MemBlkCutHeadData_Debug */


/*****************************************************************************
 �� �� ��  : TTF_MemBlkCutTailData_Debug
 ��������  : ��TTF_MEM��β����ȥָ���ֽ���Ŀ������
 �������  : ulPid    -- ���ô˺�����ģ���Pid
             ppMemSrc  -- Ҫ��д��TTF�ڴ��ͷָ��
             pDest    -- Ŀ�����ݵ�ַ
             usLen    -- ��д���ݵĳ��ȣ���λΪ�ֽ�
 �������  : ��
 �� �� ֵ  : ��ȡ���ݳɹ�����PS_SUCC��ʧ�ܷ���PS_FAIL
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2006��12��8��
    ��    ��   : ������
    �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT16 TTF_MemBlkCutTailData_Debug(VOS_UINT16 usFileID, VOS_UINT16 usLineNum, VOS_UINT32 ulPid, TTF_MEM_ST **ppMemSrc,
    VOS_UINT8 *pDest, VOS_UINT16 usLen)
{
    TTF_MEM_ST *pCurrMem;
    TTF_MEM_ST *pDelMem = VOS_NULL_PTR;
    VOS_UINT16  usCurrLen;
    VOS_UINT16  usCurrOffset;
    VOS_UINT16  usWorkedLen;

    /*====================*/    /* ������� */
    if (VOS_NULL_PTR == ppMemSrc)
    {
        PS_LOG2(ulPid, 0, PS_PRINT_WARNING,
            "Warning: TTF_MemBlkCutTailData_Debug, ppMemSrc is NULL! File<1> ,Line<2>",
            (VOS_INT32)usFileID, (VOS_INT32)usLineNum);
        return PS_FAIL;
    }

    if (VOS_NULL_PTR == *ppMemSrc)
    {
        PS_LOG2(ulPid, 0, PS_PRINT_WARNING,
            "Warning: TTF_MemBlkCutTailData_Debug, *ppMemSrc is NULL! File<1> ,Line<2>",
            (VOS_INT32)usFileID, (VOS_INT32)usLineNum);
        return PS_FAIL;
    }

    if (VOS_NULL_PTR == pDest)
    {
        PS_LOG2(ulPid, 0, PS_PRINT_WARNING,
            "Warning: TTF_MemBlkCutTailData_Debug, pDest is NULL! File<1> ,Line<2>",
            (VOS_INT32)usFileID, (VOS_INT32)usLineNum);
        return PS_FAIL;
    }

    if (0 == usLen)
    {
        PS_LOG2(ulPid, 0, PS_PRINT_WARNING,
            "Warning: TTF_MemBlkCutTailData_Debug, usLen = 0! File<1> ,Line<2>",
            (VOS_INT32)usFileID, (VOS_INT32)usLineNum);
        return PS_FAIL;
    }

    pCurrMem    = *ppMemSrc;
    pDelMem     = *ppMemSrc;    /*����ȫ����ȡʱ������pDelMem*/

    /*====================*/    /* Ѱ�ҵ���Ҫ��TTF_MEM_ST */
    usCurrLen   = TTF_MemGetLen(ulPid, pCurrMem);

    if (usCurrLen < usLen)
    {
        PS_LOG2(ulPid, 0, PS_PRINT_WARNING, "Warning: usCurrLen <1> Less Than usLen <2>!\n",
            usCurrLen, usLen);
        return PS_FAIL;
    }

    /* Ϊ���Ѱ��Ч�ʣ��Ѷ�β��������λ������ת���ɴ�ͷ����ʼ��λ�� */
    usCurrOffset    = usCurrLen - usLen;

    /*================*/ /* Ѱ�ҵ�ƫ�ƺ�ĵ�һ��TTF_MEM */
    while (VOS_NULL_PTR != pCurrMem)
    {
        /*============*/ /* Ҫ��������ݲ��ڸÿ��ڣ�����һ����м��� */
        if (pCurrMem->usUsed < usCurrOffset)
        {
            usCurrOffset   -= pCurrMem->usUsed;
            pCurrMem        = pCurrMem->pNext;
        }

        /* ��ǰ�鳤�ȵ���ƫ���������ݴ���һ�����һ���ֽڿ�ʼ��֮��Ҫ�����������ͷ� */
        else if (pCurrMem->usUsed == usCurrOffset)
        {
            usCurrOffset   -= pCurrMem->usUsed;
            pDelMem         = pCurrMem->pNext;
            pCurrMem->pNext = VOS_NULL_PTR;
            break;
        }
        /*��ǰ����Ҫ��ȡ����*/
        else
        {
            break;
        }
    }

    /* ����ǰ������㣬pCurrMem�϶���Ϊ�� */
    PS_ASSERT_RTN((VOS_NULL_PTR != pCurrMem), PS_FAIL);

    usWorkedLen     = 0;
    /*================*/    /* ��һ����Ҫ����ƫ�ƣ����⴦�� */
    /*������Ǵӵ�һ����ĵ�һ�����ݿ�ʼ������˵����һ�鲻���ͷ� */
    if (0 != usCurrOffset)
    {
        usWorkedLen         = pCurrMem->usUsed - usCurrOffset;

        DRV_RT_MEMCPY(pDest, &pCurrMem->pData[usCurrOffset], usWorkedLen);

        pCurrMem->usUsed    = usCurrOffset;
        pDelMem             = pCurrMem->pNext;
        pCurrMem->pNext     = VOS_NULL_PTR;
    }

    /* ����Ŀ����ֱ�ӿ������ݺ��ͷ� */
    while (VOS_NULL_PTR != pDelMem)
    {
        DRV_RT_MEMCPY(pDest + usWorkedLen, pDelMem->pData, pDelMem->usUsed);
        usWorkedLen    += pDelMem->usUsed;
        pDelMem        = TTF_MemSingleFree(ulPid, pDelMem);
    }

    /*��ȫ����ȡʱ��Դ�ڴ�鷵�ؿ�ָ�룬�����������*/
    if ( usCurrLen == usLen )
    {
        *ppMemSrc = VOS_NULL_PTR;
    }
    return PS_SUCC;
} /* TTF_MemBlkCutTailData_Debug */

/*****************************************************************************
 �� �� ��  : TTF_MemBlkCutTailDataWithLastMem_Debug
 ��������  : ��TTF_MEM��β����ȥָ���ֽ���Ŀ������
 �������  : ulPid       -- ���ô˺�����ģ���Pid
             ppMemSrc    -- ���Ҫ��ȥ��TTF�ڴ��ͷָ��ĵ�ַ
             pDest    -- Ŀ�����ݵ�ַ
             usLen    -- ��д���ݵĳ���, ��λΪ�ֽ�
 �������  : ppMemSrc    -- ���Ҫ��ȥ��TTF�ڴ��ͷָ��ĵ�ַ
             ppLastMem   -- Ҫ��ȥ��TTF�ڴ��βָ��
 �� �� ֵ  : PS_SUCC -- �ɹ�; PS_FAIL -- ʧ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2008��2��3��
    ��    ��   : liukai
    �޸�����   : Created
*****************************************************************************/
VOS_UINT16 TTF_MemBlkCutTailDataWithLastMem_Debug(VOS_UINT16 usFileID,
               VOS_UINT16 usLineNum, VOS_UINT32 ulPid,
               TTF_MEM_ST **ppMemSrc, TTF_MEM_ST **ppLastMem,
               VOS_UINT8 *pDest, VOS_UINT16 usLen)
{
    TTF_MEM_ST *pCurrMem;
    TTF_MEM_ST *pDelMem = VOS_NULL_PTR;
    VOS_UINT16  usCurrLen;
    VOS_UINT16  usCurrOffset;
    VOS_UINT16  usWorkedLen;

    /*====================*/    /* ������� */
    PS_ASSERT_RTN((VOS_NULL_PTR != ppMemSrc), PS_FAIL);
    PS_ASSERT_RTN((VOS_NULL_PTR != *ppMemSrc), PS_FAIL);
    PS_ASSERT_RTN((VOS_NULL_PTR != pDest), PS_FAIL);

    pCurrMem    = *ppMemSrc;
    pDelMem     = *ppMemSrc;    /*����ȫ����ȡʱ������pDelMem*/

    /*====================*/    /* Ѱ�ҵ���Ҫ��TTF_MEM_ST */
    usCurrLen   = TTF_MemGetLen(ulPid, pCurrMem);

    if (usCurrLen < usLen)
    {
        PS_LOG2(ulPid, 0, PS_PRINT_WARNING, "Warning: usCurrLen <1> Less Than usLen <2>!\n",
            usCurrLen, usLen);
        return PS_FAIL;
    }

    /* Ϊ���Ѱ��Ч�ʣ��Ѷ�β��������λ������ת���ɴ�ͷ����ʼ��λ�� */
    usCurrOffset    = usCurrLen - usLen;

    /*================*/ /* Ѱ�ҵ�ƫ�ƺ�ĵ�һ��TTF_MEM */
    while (VOS_NULL_PTR != pCurrMem)
    {
        /*============*/ /* Ҫ��������ݲ��ڸÿ��ڣ�����һ����м��� */
        if (pCurrMem->usUsed < usCurrOffset)
        {
            usCurrOffset   -= pCurrMem->usUsed;
            pCurrMem        = pCurrMem->pNext;
        }
        /*��ǰ�鳤�ȵ���ƫ���������ݴ���һ�����һ���ֽڿ�ʼ��֮��Ҫ�����������ͷ�*/
        else if (pCurrMem->usUsed == usCurrOffset)
        {
            usCurrOffset   -= pCurrMem->usUsed;
            pDelMem         = pCurrMem->pNext;
            pCurrMem->pNext = VOS_NULL_PTR;
            break;
        }
        /*��ǰ����Ҫ��ȡ����*/
        else
        {
            break;
        }
    }

    /* ����ǰ������㣬pCurrMem�϶���Ϊ�� */
    PS_ASSERT_RTN((VOS_NULL_PTR != pCurrMem), PS_FAIL);

    usWorkedLen     = 0;
    /*================*/    /* ��һ����Ҫ����ƫ�ƣ����⴦�� */
    /*������Ǵӵ�һ����ĵ�һ�����ݿ�ʼ������˵����һ�鲻���ͷ� */
    if (0 != usCurrOffset)
    {
        usWorkedLen         = pCurrMem->usUsed - usCurrOffset;

        DRV_RT_MEMCPY(pDest, &pCurrMem->pData[usCurrOffset], usWorkedLen);

        pCurrMem->usUsed    = usCurrOffset;
        pDelMem             = pCurrMem->pNext;
        pCurrMem->pNext     = VOS_NULL_PTR;
    }

    *ppLastMem = pCurrMem;    /* update last TTF_MEM */

    /* ����Ŀ����ֱ�ӿ������ݺ��ͷ� */
    while (VOS_NULL_PTR != pDelMem)
    {
        if (VOS_NULL_PTR != pDest)
        {
            DRV_RT_MEMCPY(pDest + usWorkedLen, pDelMem->pData, pDelMem->usUsed);
        }
        usWorkedLen    += pDelMem->usUsed;
        pDelMem        = TTF_MemSingleFree_Debug(usFileID, usLineNum, ulPid, pDelMem);
    }

    /*��ȫ����ȡʱ��Դ�ڴ�鷵�ؿ�ָ�룬�����������*/
    if ( usCurrLen == usLen )
    {
        *ppMemSrc = VOS_NULL_PTR;
        *ppLastMem = VOS_NULL_PTR;
    }
    return PS_SUCC;
} /* TTF_MemBlkCutTailDataWithLastMem_Debug */

TTF_MEM_ST *TTF_MemBlkRef_Debug(VOS_UINT16 usFileID, VOS_UINT16 usLineNum,
    VOS_UINT32 ulPid, VOS_UINT32 ulPoolId, TTF_MEM_ST *pMemRef,
    VOS_UINT16 usOffset, VOS_UINT16  usLen)
{
    TTF_MEM_ST *pMem;
    VOS_UINT32  ulRtn;

    pMem    = TTF_MemBlkAlloc_Debug(usFileID, usLineNum, ulPid, ulPoolId, usLen);

    if (VOS_NULL_PTR == pMem)
    {
        PS_LOG(ulPid, 0, PS_PRINT_ERROR, "Error: TTF_MemBlkAlloc_Debug Fail\n");
        return VOS_NULL_PTR;
    }

    ulRtn = TTF_MemGet(ulPid, pMemRef, usOffset, pMem->pData, usLen);

    if (PS_SUCC != ulRtn)
    {
        TTF_MemFree_Debug(usFileID, usLineNum, ulPid, &pMem);
        return VOS_NULL_PTR;
    }

    pMem->usUsed    = usLen;

    return pMem;
}/*TTF_MemBlkRef_Debug*/

TTF_MEM_ST *TTF_MemBlkAllocWithUsed_Debug
(
    VOS_UINT16 usFileID, VOS_UINT16 usLineNum, VOS_UINT32 ulPid,
    VOS_UINT32 ulPoolId, VOS_UINT16 usLen
)
{
    TTF_MEM_ST     *pMem;

    pMem    = TTF_MemBlkAlloc_Debug(usFileID, usLineNum, ulPid, ulPoolId, usLen);

    if (VOS_NULL_PTR == pMem)
    {
        PS_LOG(ulPid, 0, PS_PRINT_ERROR, "Error: TTF_MemBlkAlloc_Debug Fail\n");
        return VOS_NULL_PTR;
    }

    pMem->usUsed    = usLen;

    return pMem;
} /* TTF_MemBlkAllocWithUsed_Debug */

VOS_UINT32 TTF_MemSet(VOS_UINT32 ulPid, TTF_MEM_ST *pMemDest, VOS_UINT16 usOffset,
    VOS_UINT8 *pSrc, VOS_UINT16 usLen)
{
    TTF_MEM_ST *pCurrMem        = pMemDest;
    VOS_UINT16  usCurrOffset    = usOffset;
    VOS_UINT16  usCopyLen       = 0;
    VOS_UINT16  usWorkedLen     = 0;
    VOS_UINT16  usMemDestLen    = 0;

    /*====================*/ /* ������� */
    if ( (VOS_NULL_PTR == pMemDest)||(VOS_NULL_PTR == pSrc) )
    {
        PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning:Input Par pMemDest Or pSrc is Null!\n");
        return PS_FAIL;
    }

    if ( 0 == usLen )
    {
        PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning:Input Par usLen is 0!\n");
        return PS_FAIL;
    }

    /*====================*/ /*�ж�TTF�ڴ��ĳ����Ƿ����Ҫ��*/
    while(VOS_NULL_PTR != pMemDest)
    {
        usMemDestLen   += pMemDest->usLen;
        pMemDest        = pMemDest->pNext;
    }

    if ( usMemDestLen < (usOffset + usLen) )
    {
        PS_LOG2(ulPid, 0, PS_PRINT_WARNING, "Warning:MemDestLen <1> Less Than (Offset + Len) <2>!\n",
            usMemDestLen, (usOffset + usLen));
        return PS_FAIL;
    }

    /*================*/ /* Ѱ�ҵ�ƫ�ƺ�ĵ�һ��TTF_MEM */
    while (VOS_NULL_PTR != pCurrMem)
    {
        /*============*/ /* Ҫ��������ݲ��ڸÿ��ڣ�����һ����м��� */
        if (pCurrMem->usLen <= usCurrOffset)
        {
            usCurrOffset   -= pCurrMem->usLen;
            pCurrMem        = pCurrMem->pNext;
        }
        else
        {
            break;
        }
    }


    /*�������ϲ������ڴ�鳤�ȵļ�飬pCurrMem��Ӧ��ΪNULL*/
    PS_ASSERT_RTN((VOS_NULL_PTR != pCurrMem), PS_FAIL);

    /*================*/ /* ��һ��Ҫ����ƫ�ƣ����⴦�� */
    usCopyLen   = PS_MIN(pCurrMem->usLen - usCurrOffset, usLen);
    DRV_RT_MEMCPY(pCurrMem->pData + usCurrOffset, pSrc, usCopyLen);
    usWorkedLen = usCopyLen;


    /*================*/ /*  ��������Դ�ͷ��ʼ��������ƫ�� */
    pCurrMem        = pCurrMem->pNext;

    while (usWorkedLen < usLen)
    {
        /*�������ϲ������ڴ�鳤�ȵļ�飬pCurrMem��Ӧ��ΪNULL*/
        PS_ASSERT_RTN((VOS_NULL_PTR != pCurrMem), PS_FAIL);

        usCopyLen   = PS_MIN(pCurrMem->usLen, (usLen - usWorkedLen));
        DRV_RT_MEMCPY(pCurrMem->pData, pSrc + usWorkedLen, usCopyLen);
        usWorkedLen    += usCopyLen;
        pCurrMem        = pCurrMem->pNext;
    }

    return PS_SUCC;
} /* TTF_MemSet */


/*****************************************************************************
 Prototype       : TTF_MemGet
 Description     : ��һTTF�ڴ���ȡһ�����ȵ����ݵ�ָ��Ŀ���ַ
 Input           : ulPid    -- ���ô˺�����ģ���Pid
                   pMemSrc  -- Ҫ��ȡ���ݵ�TTF�ڴ��ͷָ��
                   usOffset -- �ڴ������ƫ��
                   pDest    -- ��Ż�ȡ���ݵ�Ŀ���ַ
                   usLen    -- ��ȡ�����ݳ��ȣ���λΪ�ֽ�
 Output          : NONE
 Return Value    : PS_SUCC   -- �ɹ�
                   PS_FAIL  -- ʧ��
                   TTF_CAUSE_INPUT_PARAMS -- �����������

 History         :
    Date         : 2005-04-19
    Author       : w29695
    Modification:  2006-12-04,������(44034)��ʵ�ָú���
*****************************************************************************/
VOS_UINT32 TTF_MemGet(VOS_UINT32 ulPid, TTF_MEM_ST *pMemSrc, VOS_UINT16 usOffset,
    VOS_UINT8 *pDest, VOS_UINT16 usLen)
{
    TTF_MEM_ST *pCurrMem        = pMemSrc;
    VOS_UINT16  usCurrOffset    = usOffset;
    VOS_UINT16  usCopyLen       = 0;
    VOS_UINT16  usWorkedLen     = 0;
    VOS_UINT16  usMemSrcLen;

    /*====================*/ /* ������� */
    if ( (VOS_NULL_PTR == pMemSrc)||(VOS_NULL_PTR == pDest) )
    {
        PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning:Input Par pMemSrc Or pDest is Null!\n");
        return PS_FAIL;
    }

    if ( 0 == usLen )
    {
        PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning:Input Par usLen is 0!\n");
        return PS_FAIL;
    }

    /*====================*/ /*�ж�TTF�ڴ��ĳ����Ƿ����Ҫ��*/
    usMemSrcLen = TTF_MemGetLen(ulPid, pMemSrc);

    if ( usMemSrcLen < (usOffset + usLen) )
    {
        PS_LOG2(ulPid, 0, PS_PRINT_WARNING, "Warning:MemSrcLen <1> Less Than (Offset + Len) <2>!\n",
            usMemSrcLen, (usOffset + usLen));
        return PS_FAIL;
    }

    /*================*/ /* Ѱ�ҵ�ƫ�ƺ�ĵ�һ��TTF_MEM */
    while (VOS_NULL_PTR != pCurrMem)
    {
        /*============*/ /* Ҫ��������ݲ��ڸÿ��ڣ�����һ����м��� */
        if (pCurrMem->usUsed <= usCurrOffset)
        {
            usCurrOffset   -= pCurrMem->usUsed;
            pCurrMem        = pCurrMem->pNext;
        }
        else
        {
            break;
        }
    }

    /*�������ϲ������ڴ�鳤�ȵļ�飬pCurrMem��Ӧ��ΪNULL*/
    PS_ASSERT_RTN((VOS_NULL_PTR != pCurrMem), PS_FAIL);

    /*================*/ /* ��һ��Ҫ����ƫ�ƣ����⴦�� */
    usCopyLen   = PS_MIN(pCurrMem->usUsed - usCurrOffset, usLen);
    DRV_RT_MEMCPY(pDest, pCurrMem->pData + usCurrOffset, usCopyLen);
    usWorkedLen = usCopyLen;


    /*================*/ /*  ��������Դ�ͷ��ʼ��������ƫ�� */
    pCurrMem        = pCurrMem->pNext;

    while (usWorkedLen < usLen)
    {
        /*�������ϲ������ڴ�鳤�ȵļ�飬pCurrMem��Ӧ��ΪNULL*/
        PS_ASSERT_RTN((VOS_NULL_PTR != pCurrMem), PS_FAIL);

        usCopyLen   = PS_MIN(pCurrMem->usUsed, usLen - usWorkedLen);
        DRV_RT_MEMCPY(pDest + usWorkedLen, pCurrMem->pData, usCopyLen);
        usWorkedLen    += usCopyLen;
        pCurrMem        = pCurrMem->pNext;
    }

    return PS_SUCC;
} /* TTF_MemGet */


/*****************************************************************************
 Prototype       : TTF_MemLink
 Description     : ��һ��TTF�ڴ����ӵ���һ��TTF�ڴ�����, ����ӵ��ڴ�����Ϊ��
                   ע��:�ú���ʵ�ֹ���ͬTTF_MemStick���ƣ���Ҫ����Maps3000����
 Input           : ulPid    -- ���ô˺�����ģ���Pid
                   pMemHead -- Ҫճ����TTF�ڴ��ͷָ��
                   pMemTail -- ճ����ͷ�����TTF�ڴ��βָ��
 Output          : pMemHead -- ճ�����TTF�ڴ��ͷָ��
 Return Value    : PS_SUCC   -- �ɹ�
                   PS_FAIL  -- ʧ��
                   TTF_CAUSE_INPUT_PARAMS -- �����������

 History         :
    Date         : 2006-12-04
    Author       : ������(44034)
*****************************************************************************/
VOS_UINT32 TTF_MemLink(VOS_UINT32 ulPid, TTF_MEM_ST **ppMemHead, TTF_MEM_ST *pMemTail)
{
    TTF_MEM_ST *pTmp = VOS_NULL_PTR;

    /*====================*/ /* ������� */
    if ((VOS_NULL_PTR == pMemTail)||(VOS_NULL_PTR == ppMemHead))
    {
        PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning:Input Par pMemTail Or ppMemHead is Null!\n");
        return PS_FAIL;
    }


    if (VOS_NULL_PTR == *ppMemHead)
    {
        *ppMemHead = pMemTail;
        return PS_SUCC;
    }

    pTmp = *ppMemHead;

    /*====================*/ /* �ҵ���TTF�ڴ������һ���ڵ� */
    while (VOS_NULL_PTR != pTmp->pNext)
    {
        pTmp = pTmp->pNext;
    }

    pTmp->pNext = pMemTail;  /* ��pMemTail������ȥ */

    return PS_SUCC;
} /* TTF_MemLink */


/*****************************************************************************
 �� �� ��  : TTF_MemGetHeadData
 ��������  : ��TTF_MEM��ͷ����ȡ���ݣ����������ݲ��ӿ���ɾ��
 �������  : ulPid    -- ���ô˺�����ģ���Pid
             pMemSrc  -- Ҫ��д��TTF�ڴ��ͷָ��
             pDest    -- Ŀ�����ݵ�ַ
             usLen    -- ��ȡ���ݵĳ��ȣ���λΪ�ֽ�
 �������  : ��
 �� �� ֵ  : ��ȡ���ݳɹ�����PS_SUCC��ʧ�ܷ���PS_FAIL
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2006��12��8��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 TTF_MemGetHeadData( VOS_UINT32 ulPid, TTF_MEM_ST *pMemSrc,
    VOS_UINT8 *pDest, VOS_UINT16 usLen)
{
    TTF_MEM_ST *pCurrMem        = pMemSrc;
    VOS_UINT16  usCopyLen       = 0;
    VOS_UINT16  usWorkedLen     = 0;
    VOS_UINT16  usMemSrcLen;

    /*====================*/ /* ������� */
    if ( (VOS_NULL_PTR == pMemSrc) || (VOS_NULL_PTR == pDest) )
    {
        PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning: Input Par pMemSrc Or pDest is Null!\n");
        return PS_FAIL;
    }

    if ( 0 == usLen )
    {
        PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning: Input Par usLen is 0!\n");
        return PS_FAIL;
    }

    /*====================*/ /*�ж�TTF�ڴ��ĳ����Ƿ����Ҫ��*/
    usMemSrcLen = TTF_MemGetLen(ulPid, pMemSrc);

    if ( usMemSrcLen < usLen )
    {
        PS_LOG2(ulPid, 0, PS_PRINT_WARNING, "Warning: MemSrcLen <1> Less Than usLen <2>!\n",
            usMemSrcLen, usLen);
        return PS_FAIL;
    }

    /*====================*/ /*��ȡ���賤�ȵ�����*/
    while (VOS_NULL_PTR != pCurrMem)
    {
        usCopyLen       = PS_MIN(pCurrMem->usUsed, (usLen - usWorkedLen));
        DRV_RT_MEMCPY(pDest+ usWorkedLen, pCurrMem->pData, usCopyLen);
        usWorkedLen    += usCopyLen;

        if ( usWorkedLen == usLen )
        {
            return PS_SUCC;
        }
        else
        {
            pCurrMem        = pCurrMem->pNext;
        }
    }

    PS_LOG(ulPid, 0, PS_PRINT_WARNING, "Warning: pCurrMem is Null!\n");
    return PS_FAIL;
} /* TTF_MemGetHeadData */

/*****************************************************************************
 �� �� ��  : TTF_MemGetTailData
 ��������  : ��TTF_MEM��β����ȡ���ݣ����������ݲ��ӿ���ɾ��
 �������  : ulPid    -- ���ô˺�����ģ���Pid
             pMemSrc  -- Ҫ��д��TTF�ڴ��ͷָ��
             pDest    -- Ŀ�����ݵ�ַ
             usLen    -- ��ȡ���ݵĳ��ȣ���λΪ�ֽ�
 �������  : ��
 �� �� ֵ  : ��ȡ���ݳɹ�����PS_SUCC��ʧ�ܷ���PS_FAIL
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2006��12��8��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 TTF_MemGetTailData( VOS_UINT32 ulPid, TTF_MEM_ST *pMemSrc,
    VOS_UINT8 *pDest, VOS_UINT16 usLen)
{
    TTF_MEM_ST *pCurrMem;
    VOS_UINT16  usCurrLen;
    VOS_UINT16  usCurrOffset;
    VOS_UINT16  usWorkedLen;


    /*====================*/    /* ������� */
    PS_ASSERT_RTN((VOS_NULL_PTR != pMemSrc), PS_FAIL);
    PS_ASSERT_RTN((VOS_NULL_PTR != pDest), PS_FAIL);
    PS_ASSERT_RTN((0 != usLen), PS_FAIL);


    pCurrMem    = pMemSrc;

    /*====================*/    /* Ѱ�ҵ���Ҫ��TTF_MEM_ST */
    usCurrLen   = TTF_MemGetLen(ulPid, pCurrMem);

    if (usCurrLen < usLen)
    {
        PS_LOG2(ulPid, 0, PS_PRINT_WARNING, "Warning: MemSrcLen <1> Less Than usLen <2>!\n",
            usCurrLen, usLen);
        return PS_FAIL;
    }


    /* Ϊ���Ѱ��Ч�ʣ��Ѷ�β��������λ������ת���ɴ�ͷ����ʼ��λ�� */
    usCurrOffset  = usCurrLen - usLen;

    /*================*/ /* Ѱ�ҵ�ƫ�ƺ�ĵ�һ��TTF_MEM */
    while (VOS_NULL_PTR != pCurrMem)
    {
        /*============*/ /* Ҫ��������ݲ��ڸÿ��ڣ�����һ����м��� */
        if (pCurrMem->usUsed <= usCurrOffset)
        {
            usCurrOffset   -= pCurrMem->usUsed;
            pCurrMem        = pCurrMem->pNext;
        }
        else
        {
            break;
        }
    }


    /* ����ǰ������㣬pCurrMem�϶���Ϊ�� */
    PS_ASSERT_RTN((VOS_NULL_PTR != pCurrMem), PS_FAIL);

    usWorkedLen     = 0;
    /*================*/ /* ��һ����Ҫ����ƫ�ƣ����⴦�� */
    if (0 != usCurrOffset)
    {
        usWorkedLen         = pCurrMem->usUsed - usCurrOffset;
        DRV_RT_MEMCPY(pDest, &pCurrMem->pData[usCurrOffset], usWorkedLen);
        pCurrMem            = pCurrMem->pNext;
    }

    /* ����Ŀ����ֱ�ӿ�������*/
    while (VOS_NULL_PTR != pCurrMem)
    {
        DRV_RT_MEMCPY(pDest + usWorkedLen, pCurrMem->pData, pCurrMem->usUsed);
        usWorkedLen    += pCurrMem->usUsed;
        pCurrMem        = pCurrMem->pNext;
    }

    return PS_SUCC;
} /* TTF_MemGetTailData */

/*****************************************************************************
 Prototype       : TTF_MemStick
 Description     : GPRSר�ã�W��ʹ��,��һ��TTF�ڴ����ӵ���һ��TTF�ڴ�����
 Input           : ulPid    -- ���ô˺�����ģ���Pid
                   pMemHead -- Ҫճ����TTF�ڴ��ͷָ��
                   pMemTail -- ճ����ͷ�����TTF�ڴ��βָ��
 Output          : pMemHead -- ճ�����TTF�ڴ��ͷָ��
 Return Value    : PS_SUCC   -- �ɹ�
                   PS_FAIL  -- ʧ��
                   TTF_CAUSE_INPUT_PARAMS -- �����������

 History         :
    Date         : 2005-04-19
    Author       : ����(29695)
    Modification:  2006-12-04,������(44034)��ʵ�ָú���
*****************************************************************************/
VOS_UINT32 TTF_MemStick(VOS_UINT32 ulPid, TTF_MEM_ST *pMemHead, TTF_MEM_ST *pMemTail)
{
    TTF_MEM_ST *pTmp = VOS_NULL_PTR;


    /*====================*/ /* ������� */
    if ((VOS_NULL_PTR == pMemHead) || (VOS_NULL_PTR == pMemTail))
    {
        return PS_FAIL;
    }

    pTmp = pMemHead;

    /*====================*/ /* �ҵ���TTF�ڴ������һ���ڵ� */
    while (VOS_NULL_PTR != pTmp->pNext)
    {
        pTmp = pTmp->pNext;
    }

    pTmp->pNext = pMemTail;  /* ��pMemTail������ȥ */

    return PS_SUCC;
} /* TTF_MemStick */


VOS_UINT32 TTF_MemInsertNext(VOS_UINT32 ulPid, TTF_MEM_ST *pMemCurr, TTF_MEM_ST *pMemNext)
{
    if (VOS_NULL_PTR == pMemCurr || VOS_NULL_PTR == pMemNext)
    {
        return PS_FAIL;
    }

    pMemNext->pNext = pMemCurr->pNext;
    pMemCurr->pNext = pMemNext;

    return PS_SUCC;
}


VOS_UINT32 TTF_MemIsCritical(VOS_VOID)
{
    /*��ʱ��׮�ú�����ʼ�շ���VOS_NO*/

    return VOS_NO;
}
VOS_UINT32 TTF_MemRegEventCallBack
(
    TTF_MEM_POOL_ID_ENUM_UINT32         enPoolId,
    TTF_MEM_EVENT_CALLBACK              pMemAllocEvent,
    TTF_MEM_EVENT_CALLBACK              pMemFreeEvent
)
{
    TTF_MEM_POOL_STRU              *pstTtfMemPool;

    if (enPoolId >= TTF_MEM_POOL_ID_BUTT)
    {
        PS_LOG1(WUEPS_PID_RLC, 0, PS_PRINT_ERROR, "TTF_MemRegEventCallBack Fail, PoolId %d exceed the Ranger\r\n", (VOS_INT32)enPoolId);
        return  PS_FAIL;
    }

    pstTtfMemPool   = TTF_MEM_GET_POOL(enPoolId);

    pstTtfMemPool->pMemAllocEvent   = pMemAllocEvent;
    pstTtfMemPool->pMemFreeEvent    = pMemFreeEvent;

    return PS_SUCC;
} /* TTF_MemRegEventCallBack */

VOS_UINT32 TTF_MemRegExtFreeCallBack
(
    TTF_MEM_EXT_FREE_CALLBACK           pMemExtFreeFunc
)
{
    TTF_MEM_POOL_STRU                   *pstTtfMemPool;


    if ( VOS_NULL_PTR == pMemExtFreeFunc )
    {
        return PS_FAIL;
    }

    pstTtfMemPool = TTF_MEM_GET_POOL(TTF_MEM_POOL_ID_EXT);
    pstTtfMemPool->pMemExtFreeFunc = pMemExtFreeFunc;

    return PS_SUCC;
} /* TTF_MemRegExtFreeCallBack */


/*****************************************************************************
 �� �� ��  : TTF_SetBlkMemFlag
 ��������  :
 �������  : VOS_INT8 *cFileName
             VOS_UINT32 ulLineNum
             VOS_UINT32 ulPid
             VOS_UINT16 usLen
 �������  : ��
 �� �� ֵ  : TTF_MEM_ST *
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2007��3��24��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID TTF_MemSetBlkFlag(TTF_MEM_ST *pstMem, VOS_UINT16 usFileID, VOS_UINT16 usLineNum)
{
    while (VOS_NULL_PTR != pstMem)
    {
        if (VOS_NULL_PTR != pstMem->pstDbgInfo)
        {
            pstMem->pstDbgInfo->usTraceFileId   = usFileID;
            pstMem->pstDbgInfo->usTraceLineNum  = usLineNum;
            pstMem->pstDbgInfo->ulTraceTick     = OM_GetSlice();

            pstMem  = pstMem->pNext;
        }
    }
} /* TTF_MemSetBlkFlag */


VOS_VOID TTF_MemFreeData(VOS_UINT32 ulPid, TTF_MEM_ST *pItem)
{
    if ( VOS_NULL_PTR != pItem )
    {
        TTF_MemFree(ulPid, pItem);
    }

    return;

}


/*lint -esym( 528, TTF_MemGetLen )*/
VOS_UINT16 TTF_MemGetLen(VOS_UINT32 ulPid, TTF_MEM_ST *pMem)
{
    VOS_UINT16      usLen   = 0;
    TTF_MEM_ST     *pTmp    = pMem;

    while(VOS_NULL_PTR != pTmp)
    {
        usLen   += pTmp->usUsed;
        pTmp     = pTmp->pNext;
    }

    return usLen;
} /*TTF_MemGetLen*/



VOS_VOID TTF_MemGetAllocFailCnt(VOS_VOID)
{
    VOS_UINT16                          usLoop;


    vos_printf("TTF_MemGetAllocFailCnt Start: \n");
    vos_printf("TTF Mem Alloc Fail Threshold: %d \n", g_usTtfMemAllocFailCntThreshold);

    for( usLoop = 0; usLoop < TTF_MEM_POOL_ID_BUTT; ++usLoop )
    {
        vos_printf("TTF Mem Pool ID: %d \n", usLoop);
        vos_printf("TTF Mem Alloc Fail Cnt: %d * %d + %d \n",
                    *g_apusTtfMemExcThresholdCnt[usLoop], g_usTtfMemAllocFailCntThreshold, *g_apusTtfMemAllocFailCnt[usLoop]);
    }

    vos_printf("TTF_MemGetAllocFailCnt End. \n");

    return;
}/* TTF_MemGetAllocFailCnt */

VOS_VOID TTF_MemSetAllocFailThreshold(VOS_UINT16 usThreshold)
{
    g_usTtfMemAllocFailCntThreshold = usThreshold;

    return;
}/* TTF_MemSetAllocFailThreshold */

VOS_VOID TTF_MemReSetAllocFailStatus(VOS_VOID)
{
    VOS_UINT16                          usLoop;


    for( usLoop = 0; usLoop < TTF_MEM_POOL_ID_BUTT; ++usLoop )
    {
        *g_apusTtfMemExcThresholdCnt[usLoop]  = 0;
        *g_apusTtfMemAllocFailCnt[usLoop]     = 0;
    }

    return;
}/* TTF_MemReSetAllocFailStatus */

VOS_VOID TTF_MemHelp( VOS_VOID )
{
    vos_printf("********************TTF_MEM�����Ϣ************************\n");
    vos_printf("TTF_MemBlkCheckPoolLeak(ulPoolId):           ���TTF_MEM�ڴ�й¶������ulPoolId:\n");
    vos_printf("                                             0--DL_SHARE; 1--DL_PDU; 2--UL_DATA; 3--EXT; 4--UL_IP_DATA\n");
    vos_printf("TTF_ShowUsedBlkMemInfo(VOS_VOID)             �鿴�ڴ��ʹ����Ϣ(g_ucTtfMemDebugPrint =1ʱ������Ϣ��ӡ)��\n");
    vos_printf("TTF_MemSetTraceFunc(ucChoice)                ���ú������ٹ켣��ӡ��ucChoice = 0 �رչ켣��ӡ��\n");
    vos_printf("TTF_MemGetAllocFailCnt(VOS_VOID)             ��ȡ�ڴ�����ʧ�ܴ�����Ϣ��\n");
    vos_printf("TTF_MemSetAllocFailThreshold(usThreshold)    �����ڴ�����ʧ���ϱ����ޡ�\n");
    vos_printf("TTF_MemGetAllocFailCnt(VOS_VOID)             ��ȡ�ڴ�����ʧ��ͳ����Ϣ��\n");
    vos_printf("TTF_MemReSetAllocFailStatus(VOS_VOID)        �����ڴ�����ʧ��ͳ����Ϣ��\n");
    vos_printf("TTF_MemShowMntnInfo( VOS_VOID )              ��ȡC���ڴ�����ͷſ�ά�ɲ���Ϣ��\n");
    vos_printf("TTF_RbMemPrintIsrCnt                         C���յ�IPF�жϴ���.\n");
    vos_printf("TTF_MemRcovShow                              C��TTF Mem�ڴ�й¶����״̬.\n");

    return;
}/* TTF_MemHelp */

VOS_VOID TTF_MemRcovShow(VOS_VOID)
{
    VOS_UINT32 ulPoolId;

    vos_printf("��ǰ�����ڴ������:             0x%x\r\n", g_stTtfMemRcovEnt.ucRcovPoolMsk);
    vos_printf("��ǰϵͳʱ��(Slice):            %u\r\n", VOS_GetSlice());

    for (ulPoolId = 0; ulPoolId < TTF_MEM_POOL_ID_BUTT; ulPoolId++)
    {
        vos_printf("�ڴ��:%d ��״̬:                       %u\r\n", ulPoolId, g_stTtfMemRcovEnt.astPoolInfo[ulPoolId].ulState);
        vos_printf("�ڴ��:%d �ָ�ʱ��(Slice):              %u\r\n", ulPoolId, g_stTtfMemRcovEnt.astPoolInfo[ulPoolId].ulRcovTimerLen);
        vos_printf("�ڴ��:%d ����ָ�״̬����:             %u\r\n", ulPoolId, g_stTtfMemRcovEnt.astPoolInfo[ulPoolId].ulRcovThres);
        vos_printf("�ڴ��:%d ����ָ�״̬��ʱ��(Slice):    %u\r\n", ulPoolId, g_stTtfMemRcovEnt.astPoolInfo[ulPoolId].ulEnterRcovTs);
    }
}


VOS_VOID TTF_MemRcovEntInit(VOS_VOID)
{
    VOS_UINT32 ulPoolId;

    TTF_MEM_RST_RCOV_ENT();

    TTF_MEM_SET_RCOV_ENT_POOL_MSK(TTF_MEM_RCOV_DEF_POOL_MSK);

    for (ulPoolId = 0; ulPoolId < TTF_MEM_POOL_ID_BUTT; ulPoolId++)
    {
        TTF_MEM_SET_RCOV_ENT_THRES(ulPoolId, TTF_MEM_RCOV_THRES);
        TTF_MEM_SET_RCOV_ENT_TIMER_LEN(ulPoolId, TTF_MEM_RCOV_TIMER_LEN);
        TTF_MEM_SET_RCOV_ENT_STATE(ulPoolId,  TTF_MEM_STATE_NORMAL);
    }
}



VOS_VOID TTF_MemRcovAllocFail(VOS_UINT32 ulPoolId, VOS_UINT32 ulFailCnt, VOS_UINT32 ulPid, VOS_UINT16 usFileId, VOS_UINT16 usLine)
{
    VOS_UINT32  ulCurrTs;
    VOS_UINT32  ulTimerLen;
    VOS_UINT32  ulTimerStart;
    VOS_UINT32  ulPoolMsk;

    ulPoolMsk = TTF_MEM_MASK_POOL(ulPoolId);

    if (!TTF_MEM_CHK_NEED_RCOV_POOL(ulPoolMsk))
    {
        TTF_MEM_SET_RCOV_ENT_STATE(ulPoolId,  TTF_MEM_STATE_ALLOC_FAIL);

        return;
    }

    /* �Ѿ��ڵȴ��ָ�״̬���ж��Ƿ��Ѿ���ʱ */
    if (TTF_MEM_STATE_WAIT_RCOV == TTF_MEM_GET_RCOV_ENT_STATE(ulPoolId))
    {
        ulCurrTs        = VOS_GetSlice();
        ulTimerLen      = TTF_MEM_GET_RCOV_ENT_TIMER_LEN(ulPoolId);
        ulTimerStart    = TTF_MEM_GET_RCOV_ENT_TIMESTAMP(ulPoolId);

        /* �Ѿ���ʱ����λ���� */
        if (TTF_MEM_RCOV_TIMER_EXPIRED(ulCurrTs, (ulTimerStart  + ulTimerLen)))
        {
            TTF_MEM_RCOV_MEM(ulPoolId, ulPid, usFileId, usLine);
        }
    }
    else
    {
        /* �������ޣ�����ȴ��ָ�״̬ */
        if (ulFailCnt >= TTF_MEM_GET_RCOV_ENT_THRES(ulPoolId))
        {
            TTF_MEM_RCOV_ENTER_WAIT_RCOV_STATE(ulPoolId);
        }
        else
        {
            TTF_MEM_SET_RCOV_ENT_STATE(ulPoolId,  TTF_MEM_STATE_ALLOC_FAIL);
        }
    }
}

#if (FEATURE_ON == FEATURE_SKB_EXP) /* V3R3 �汾 */

VOS_VOID TTF_MemSetDefaultNvCfg(TTF_MEM_SOLUTION_CFG_NV_STRU *pstTtfMemSolution)
{
    const   VOS_UINT8   ucPoolMask  = 0x0F;


    pstTtfMemSolution->ucPoolCnt    = TTF_MEM_MAX_POOL_NUM;
    pstTtfMemSolution->ucPoolMask   = ucPoolMask;

    /*DL SHARE*/
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_SHARE].ucClusterCnt  = 2;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_SHARE].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_0] = 864;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_SHARE].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_0]  = 198;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_SHARE].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_1] = 1952;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_SHARE].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_1]  = 158;

    /*DL PDU */
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_PDU].ucClusterCnt  = 3;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_PDU].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_0] = 84;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_PDU].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_0]  = 1024;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_PDU].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_1] = 700;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_PDU].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_1]  = 950;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_PDU].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_2] = 1560;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_PDU].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_2]  = 585;

    /*UL DATA*/
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_DATA].ucClusterCnt  = 4;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_DATA].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_0] = 84;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_DATA].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_0]  = 5120;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_DATA].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_1] = 628;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_DATA].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_1]  = 1700;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_DATA].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_2] = 1652;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_DATA].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_2]  = 300;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_DATA].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_3] = 3700;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_DATA].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_3]  = 10;

    /*UL EXT*/
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_EXT].ucClusterCnt  = 1;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_EXT].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_0] = 0;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_EXT].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_0]  = TTF_MEM_CTRL_MEM_CNT;


    /* UL IP PACKET */
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_IP_DATA].ucClusterCnt  = 0;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_IP_DATA].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_0] = 0;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_IP_DATA].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_0]  = 0;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_IP_DATA].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_1] = 0;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_IP_DATA].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_1]  = 0;

    return;
}
TTF_MEM_ST *TTF_MemBlkAlloc_Debug(VOS_UINT16 usFileID, VOS_UINT16 usLineNum,
     VOS_UINT32 ulPid, VOS_UINT32 ulPoolId, VOS_UINT16 usLen)
{
    TTF_MEM_POOL_STRU              *pstTtfMemPool;
    TTF_MEM_CLUSTER_STRU           *pstTtfMemCluster;
    TTF_MEM_ST                     *pMem;
    VOS_INT32                       lLockKey;
    VOS_UINT8                       ucClusterId;
    VOS_UINT8                       ucMostFitLev;
    VOS_UINT16                      usDataLen;


    if ((ulPoolId >= TTF_MEM_POOL_ID_BUTT) || (TTF_MEM_POOL_ID_UL_IP_DATA == ulPoolId))
    {
        TTF_LOG1(ulPid, PS_PRINT_WARNING, "TTF_MemBlkAlloc_Debug,Poolid %d is invalid! \n",
            (VOS_INT32)ulPoolId);
        return VOS_NULL_PTR;
    }

    /* ��ȡ�ڴ�� */
    pstTtfMemPool = TTF_MEM_GET_POOL(ulPoolId);

    if ( PS_TRUE != pstTtfMemPool->usUsedFlag )
    {
        TTF_LOG1(ulPid, PS_PRINT_WARNING, "TTF_MemBlkAlloc_Debug,Pool %d is not used! \n",
            (VOS_INT32)ulPoolId);
        return VOS_NULL_PTR;
    }

    /* ���볤���Ƿ��ڸ��ڴ��� */
    if ( TTF_MEM_POOL_ID_EXT == ulPoolId )
    {
        /* ������ⲿ�ڴ棬�򽫳�����Ϊ�� */
        usLen = 0;
    }
    else
    {
        /* ���ⲿ�ڴ�أ��������Ϊ�����ӡ�澯 */
        if ( 0 == usLen )
        {
            TTF_LOG2(ulPid, PS_PRINT_WARNING, "TTF_MemBlkAlloc_Debug, Invalid usLen = %d for the Pool %d ! \n", usLen, (VOS_INT32)ulPoolId);
            return VOS_NULL_PTR;
        }
    }

    /* ���Ϊ���й����ڴ�أ���������ƫ�� */
    if ( TTF_MEM_POOL_ID_DL_SHARE == ulPoolId )
    {
        /* ����MBB�ķ�ʽ, �������ڴ�ʱ, ��ͷ����β������pad */
        TTF_MEM_RSV_PAD_LEN(usDataLen, usLen);
    }

    /* ���������󳤶�, ���ӡ�澯 */
    if ( usLen > pstTtfMemPool->usMaxByteLen )
    {
        TTF_LOG2(ulPid, PS_PRINT_WARNING, "TTF_MemBlkAlloc_Debug, usLen = %d exceed the Pool %d ranger! \n", usLen, (VOS_INT32)ulPoolId);
        return VOS_NULL_PTR;
    }

    /* ���ڴ�صĶ������Ѱ�Һ��ʵ�TTF_MEM */
    ucMostFitLev = TTF_GET_LEN_INDEX(pstTtfMemPool, usLen);

    for(ucClusterId = ucMostFitLev;
        ucClusterId < pstTtfMemPool->ucClusterCnt;
        ucClusterId ++)
    {
        pstTtfMemCluster  = &(pstTtfMemPool->astClusterTable[ucClusterId]);

        lLockKey = VOS_SplIMP();

        if (0 != pstTtfMemCluster->usFreeCnt)
        {
            /* ��ȡһ��û��ʹ�õ�TTF_MEM_ST��� */
            pMem    = pstTtfMemCluster->apstFreeStack[--pstTtfMemCluster->usFreeCnt];
            pstTtfMemPool->usTtfMemUsedCnt++;

            VOS_Splx(lLockKey);

            TTF_MEM_RESET(pMem);

            lLockKey = VOS_SplIMP();
            pstTtfMemPool->ulTtfMemUsedByteSize += pMem->usLen;
            VOS_Splx(lLockKey);

            if (VOS_NULL_PTR != pstTtfMemPool->pMemAllocEvent)
            {
                pstTtfMemPool->pMemAllocEvent(pstTtfMemPool->usTtfMemUsedCnt, pstTtfMemPool->ulTtfMemUsedByteSize);
            }

            TTF_MEM_DBG(pMem, usFileID, usLineNum);

            if (TTF_MEM_POOL_ID_DL_SHARE == ulPoolId)
            {
               pMem->pData  = pMem->pOrigData + TTF_MEM_SKB_HEAD_PAD_LEN + TTF_MEM_MAC_HEADER_RSV_LEN;
               pMem->usLen -= TTF_MEM_SKB_HEAD_PAD_LEN + TTF_MEM_MAC_HEADER_RSV_LEN;
            }

            (*g_apusTtfMemConAllocFailCnt[ulPoolId]) = 0;

            TTF_MEM_RCOV_ALLOC_SUCC(ulPoolId);

            return pMem;
        }

        VOS_Splx(lLockKey);
    }

    /* ��һ�γ����ڴ�����ʧ���ϱ�������ÿ�ﵽ�����ϱ�һ�� */
    if ( 0 == ((*g_apusTtfMemAllocFailCnt[ulPoolId]) & g_usTtfMemAllocFailCntThreshold ))
    {
        (*g_apusTtfMemExcThresholdCnt[ulPoolId])++;

        /* ����Errlog�¼���failCnt���ں���ά���ģ���������Ҫ+1 */
        TTF_MNTN_ErrlogTtfMemAllocFail(ulPid, ulPoolId, usFileID, usLineNum, (*g_apusTtfMemAllocFailCnt[ulPoolId]) + 1);

        /* �ϱ��ڴ������ʧ���¼� */
        TTF_BlkMemUsedInfoEventRpt(ulPid, ulPoolId, TRIG_TYPE_ALLOC_FAIL);
    }

    (*g_apusTtfMemAllocFailCnt[ulPoolId])++;
    (*g_apusTtfMemConAllocFailCnt[ulPoolId])++;

    TTF_MEM_RCOV_ALLOC_FAIL(ulPoolId, (*g_apusTtfMemConAllocFailCnt[ulPoolId]), ulPid, usFileID, usLineNum);

    return VOS_NULL_PTR;
} /* TTF_MemBlkAlloc_Debug */

VOS_VOID TTF_ExtMemFree( void **ppstExtMem )
{
    TTF_MEM_POOL_STRU                  *pstTtfMemPool;
    void                               *pstMem = VOS_NULL_PTR;

    TTF_DEBUG_TRACE_FUNC_ENTER();

    if ( VOS_NULL_PTR == ppstExtMem )
    {
        g_stTtfMemFreeMntnEntity.ulTtfMemExtFreeFailCnt++;
        TTF_LOG(WUEPS_PID_RLC, PS_PRINT_WARNING, "WARNING: TTF_RemoteFreeImmZc pstImmZc is NULL !\n");

        return;
    }

    if ( VOS_NULL_PTR == *ppstExtMem )
    {
        g_stTtfMemFreeMntnEntity.ulTtfMemExtFreeFailCnt++;
        TTF_LOG(WUEPS_PID_RLC, PS_PRINT_WARNING, "WARNING: TTF_RemoteFreeImmZc *pstImmZc is NULL !\n");

        return;
    }

    pstTtfMemPool = TTF_MEM_GET_POOL(TTF_MEM_POOL_ID_EXT);
    if ( VOS_NULL_PTR == pstTtfMemPool->pMemExtFreeFunc )
    {
        g_stTtfMemFreeMntnEntity.ulTtfMemExtFreeFailCnt++;
        TTF_LOG(WUEPS_PID_RLC, PS_PRINT_ERROR, "ERROR: TTF_RemoteFreeImmZc EXT free method is not registered !\n");

        return;
    }

    pstMem = *ppstExtMem;
    pstTtfMemPool->pMemExtFreeFunc(pstMem);
    (*ppstExtMem) = VOS_NULL_PTR;


    TTF_DEBUG_TRACE_FUNC_LEAVE();

    return;
}/* TTF_ExtMemFree */

TTF_MEM_ST * TTF_DataTransformTtfMem_Debug(VOS_UINT16 usFileID, VOS_UINT16 usLineNum,VOS_UINT8* pucData,VOS_UINT16 usLen, VOS_VOID*  pstIMM)
{
    TTF_MEM_ST                         *pstMem = VOS_NULL_PTR;

    TTF_DEBUG_TRACE_FUNC_ENTER();

    if ( VOS_NULL_PTR == pstIMM )
    {
        TTF_LOG(WUEPS_PID_RLC, PS_PRINT_ERROR, "ERROR: TTF_DataTransformTtfMem_Debug Input Para pstIMMZc is NULL !\n");
        return VOS_NULL_PTR;
    }

    if ( VOS_NULL_PTR == pucData )
    {
        TTF_LOG(WUEPS_PID_RLC, PS_PRINT_ERROR, "ERROR: TTF_DataTransformTtfMem_Debug Input Para pucDatas is NULL !\n");
        TTF_RemoteFreeImmMem(pstIMM);
        return VOS_NULL_PTR;
    }

    pstMem = TTF_MemBlkAlloc_Debug(usFileID, usLineNum, WUEPS_PID_RLC, TTF_MEM_POOL_ID_EXT, 0);

    if ( VOS_NULL_PTR == pstMem )
    {
        TTF_LOG(WUEPS_PID_RLC, PS_PRINT_ERROR, "ERROR:TTF_DataTransformTtfMem_Debug TTF_MemBlkAlloc_Debug fail !\n");
        TTF_RemoteFreeImmMem(pstIMM);
        return VOS_NULL_PTR;
    }

    pstMem->pOrigData       = (VOS_UINT8*)(pucData - TTF_MEM_MAC_HEADER_RSV_LEN);
    pstMem->pData           = pucData;
    pstMem->usLen           = usLen + TTF_MEM_MAC_HEADER_RSV_LEN;
    pstMem->usUsed          = usLen;
    pstMem->pExtBuffAddr    = pstIMM;

    TTF_DEBUG_TRACE_FUNC_LEAVE();

    return pstMem;
} /* TTF_DataTransformTtfMem_Debug */

VOS_VOID  TTF_MemPoolInfoEventRpt(VOS_UINT8 ucPoolId)
{
    TTF_MEM_POOL_STRU                      *pstTtfMemPool;


    pstTtfMemPool = TTF_MEM_GET_POOL(ucPoolId);

    if ( PS_TRUE != pstTtfMemPool->usUsedFlag )
    {
        TTF_LOG1(WUEPS_PID_RLC, PS_PRINT_WARNING, "TTF_MemPoolInfoEventRpt, Pool %d is not used!\n", ucPoolId);
        return;
    }

    if ( pstTtfMemPool->usTtfMemUsedCnt> 0 )
    {
        TTF_LOG2(WUEPS_PID_RLC, PS_PRINT_ERROR, "TTF_MEM_BLK Is Leak,Pool %d,UsedCnt %d \n!",
            ucPoolId, pstTtfMemPool->usTtfMemUsedCnt);

        TTF_BlkMemUsedInfoEventRpt(WUEPS_PID_RLC, ucPoolId, TRIG_TYPE_LEAK);
        TTF_MemReportMemLeakMsg(WUEPS_PID_RLC);
    }
    else
    {
        TTF_LOG1(WUEPS_PID_RLC, PS_PRINT_NORMAL, "NORMAL: TTF_MemPoolInfoEventRpt, Pool %d Normal\n!",ucPoolId);
        TTF_MemSndNormalEvent(ucPoolId) ;
    }

    return;
} /* TTF_MemPoolInfoEventRpt */

VOS_VOID TTF_MemCcpuCheckPoolLeak( VOS_VOID )
{
    VOS_UINT8                          ucPoolId;

    /* �ͷź˼���еĽڵ� */
    TTF_MemFreeFromQue();

    /* �ͷ�CDS�ڴ��ͷŶ����ڴ� */
    CDS_FreeIpfDlSrcMem();


    for(ucPoolId = TTF_MEM_POOL_ID_DL_SHARE; ucPoolId < TTF_MEM_POOL_ID_EXT; ++ucPoolId )
    {
        TTF_MemPoolInfoEventRpt(ucPoolId);
    }

    return;
}


VOS_UINT32 TTF_GetLocalFreeMemCnt(VOS_UINT8 ucPoolId)
{
    TTF_MEM_POOL_STRU                  *pstTtfMemPool;

    if ( ucPoolId >= TTF_MEM_POOL_ID_UL_IP_DATA )
    {
        TTF_LOG1(WUEPS_PID_RLC, PS_PRINT_ERROR, "TTF_GetLocalFreeMemCnt PoolId fail %d \n", ucPoolId);
        return PS_FAIL;
    }

    pstTtfMemPool = TTF_MEM_GET_POOL(ucPoolId);

    return pstTtfMemPool->astClusterTable[pstTtfMemPool->ucClusterCnt - 1].usFreeCnt;
} /* TTF_GetLocalFreeMemCnt */

VOS_UINT32 TTF_GetDlIpFreeMemCnt (VOS_VOID)
{
    return TTF_GetLocalFreeMemCnt(TTF_MEM_POOL_ID_DL_SHARE);
}



VOS_UINT32 TTF_GetACoreReservedCnt (VOS_VOID)
{
    /*lint --e(778)*/
    return (*(VOS_UINT32 *)(TTF_MEM_ACPU_FREE_MEM_CNT_ADDR));

} /* TTF_GetACoreReservedCnt */



VOS_UINT32 TTF_GetUlIpFreeMemCnt (VOS_VOID)
{
    return TTF_GetACoreReservedCnt();
}


VOS_VOID TTF_MemFreeShowMntnInfo( VOS_VOID )
{
    TTF_MEM_FREE_INVALIED_MEM_INFO_STRU    *pstInvalidMemInfo   = &g_stTtfMemFreeMntnEntity.astTtfMemFreeInvalidMemInfo;
    TTF_MEM_FREE_NULL_PTR_INFO_STRU        *pstNullPtrInfo      = &g_stTtfMemFreeMntnEntity.astTtfMemFreeNullPtrInfo;
    TTF_MEM_FREE_NULL_PTR_INFO_STRU        *pstNullPPtrInfo     = &g_stTtfMemFreeMntnEntity.astTtfMemFreeNullPPtrInfo;

    vos_printf("TTF MEM FREE ��ά�ɲ���Ϣ :\n");
    vos_printf("===========================================\n");
    vos_printf("TTF MEM FREE �Ƿ��ڴ���Ϣ :\n");
    vos_printf("TTF MEM FREE �յ��ķǷ��ڴ����       : %d \n", (VOS_INT32)pstInvalidMemInfo->ulInvalidTtfMemCnt);
    vos_printf("TTF MEM FREE �Ƿ�TTF MEM �ڴ��ַ     : 0x%u \n", (VOS_INT32)pstInvalidMemInfo->ulInvalidTtfMemAddr);
    vos_printf("TTF MEM FREE �Ƿ�TTF MEM �ڴ��ļ�ID   : %d \n", pstInvalidMemInfo->usTtfMemFreeFileId);
    vos_printf("TTF MEM FREE �Ƿ�TTF MEM �ڴ��к���Ϣ : %d \n", pstInvalidMemInfo->usTtfMemFreeLineNum);
    vos_printf("TTF MEM FREE �Ƿ�TTF MEM �ڴ�����     : %d \n", pstInvalidMemInfo->ucInvalidTtfMemType);
    vos_printf("TTF MEM FREE �Ƿ�TTF MEM �ڴ�ֵ       : %d \n", pstInvalidMemInfo->usInvalidTtfMemValue);

    vos_printf("===========================================\n");
    vos_printf("TTF MEM FREE ��ָ����Ϣ :\n");
    vos_printf("TTF MEM FREE һ��ָ��Ϊ�յĴ���       : %d \n", (VOS_INT32)pstNullPtrInfo->ulTtfMemFreeNullPtrCnt);
    vos_printf("TTF MEM FREE ����ָ���ļ�ID           : %d \n", pstNullPtrInfo->usTtfMemFreeFileId);
    vos_printf("TTF MEM FREE ����ָ���ڴ��к���Ϣ     : %d \n", pstNullPtrInfo->usTtfMemFreeLineNum);

    vos_printf("TTF MEM FREE ����ָ��Ϊ�յĴ���       : %d \n", (VOS_INT32)pstNullPPtrInfo->ulTtfMemFreeNullPtrCnt);
    vos_printf("TTF MEM FREE ����ָ���ڴ��ļ�ID       : %d \n", pstNullPPtrInfo->usTtfMemFreeFileId);
    vos_printf("TTF MEM FREE ����ָ���ڴ��к���Ϣ     : %d \n", pstNullPPtrInfo->usTtfMemFreeLineNum);

    vos_printf("TTF MEM Pool Status:\n");
    vos_printf("          TTF_MEM_POOL_ID_DL_SHARE    : %d\n", g_astTtfMemPool[TTF_MEM_POOL_ID_DL_SHARE].usUsedFlag);
    vos_printf("          TTF_MEM_POOL_ID_DL_PDU      : %d\n", g_astTtfMemPool[TTF_MEM_POOL_ID_DL_PDU].usUsedFlag);
    vos_printf("          TTF_MEM_POOL_ID_UL_DATA     : %d\n", g_astTtfMemPool[TTF_MEM_POOL_ID_UL_DATA].usUsedFlag);
    vos_printf("          TTF_MEM_POOL_ID_EXT         : %d\n", g_astTtfMemPool[TTF_MEM_POOL_ID_EXT].usUsedFlag);

    vos_printf("TTF_MEM_POOL_ID_EXT �ͷź���          : 0x%u\n", (VOS_INT32)g_astTtfMemPool[TTF_MEM_POOL_ID_EXT].pMemExtFreeFunc);
    vos_printf("TTF Ext MEM FREE ʧ�ܴ���             : %d \n", (VOS_INT32)g_stTtfMemFreeMntnEntity.ulTtfMemExtFreeFailCnt);

}


/******************************************************************************/
/*******************************V9R1�汾***************************************/
/******************************************************************************/
#else
VOS_VOID TTF_MemSetDefaultNvCfg(TTF_MEM_SOLUTION_CFG_NV_STRU *pstTtfMemSolution)
{
    const   VOS_UINT8   ucPoolMask  = 0x16;

    pstTtfMemSolution->ucPoolCnt    = TTF_MEM_MAX_POOL_NUM;
    pstTtfMemSolution->ucPoolMask   = ucPoolMask;

    /*DL SHARE*/
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_SHARE].ucClusterCnt  = 0;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_SHARE].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_0] = 0;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_SHARE].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_0]  = 0;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_SHARE].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_1] = 0;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_SHARE].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_1]  = 0;

    /*DL PDU */
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_PDU].ucClusterCnt  = 4;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_PDU].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_0] = 84;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_PDU].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_0]  = 2304;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_PDU].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_1] = 700;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_PDU].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_1]  = 824;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_PDU].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_2] = 1560;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_PDU].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_2]  = 1800;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_PDU].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_3] = 8200;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_DL_PDU].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_3]  = 15;

    /*UL DATA*/
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_DATA].ucClusterCnt  = 4;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_DATA].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_0] = 164;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_DATA].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_0]  = 5430;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_DATA].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_1] = 628;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_DATA].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_1]  = 1748;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_DATA].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_2] = 1652;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_DATA].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_2]  = 270;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_DATA].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_3] = 8200;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_DATA].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_3]  = 10;

    /*UL EXT*/
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_EXT].ucClusterCnt  = 0;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_EXT].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_0] = 0;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_EXT].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_0]  = 0;


    /* UL IP PACKET */
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_IP_DATA].ucClusterCnt  = 3;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_IP_DATA].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_0] = 404;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_IP_DATA].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_0]  = 1000;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_IP_DATA].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_1] = 1560;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_IP_DATA].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_1]  = 1000;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_IP_DATA].ausBlkSize[TTF_MEM_POOL_BLK_INDEX_2] = 1628;
    pstTtfMemSolution->astTtfMemPoolCfgInfo[TTF_MEM_POOL_ID_UL_IP_DATA].ausBlkCnt[TTF_MEM_POOL_BLK_INDEX_2]  = 20;


    return;
}
TTF_MEM_ST *TTF_MemBlkAlloc_Debug(VOS_UINT16 usFileID, VOS_UINT16 usLineNum,
     VOS_UINT32 ulPid, VOS_UINT32 ulPoolId, VOS_UINT16 usLen)
{
    TTF_MEM_POOL_STRU              *pstTtfMemPool;
    TTF_MEM_CLUSTER_STRU           *pstTtfMemCluster;
    TTF_MEM_ST                     *pMem;
    VOS_INT32                       lLockKey;
    VOS_UINT8                       ucClusterId;
    VOS_UINT8                       ucMostFitLev;


    /* V9R1�汾��ֻ��TTF_MEM_POOL_ID_DL_PDU��TTF_MEM_POOL_ID_UL_DATA ��
    TTF_MEM_POOL_ID_UL_IP_DATA�����ڴ����Ч */
    if ( (ulPoolId >= TTF_MEM_POOL_ID_BUTT)
        || (TTF_MEM_POOL_ID_EXT == ulPoolId)
        || (TTF_MEM_POOL_ID_DL_SHARE == ulPoolId) )
    {
        TTF_LOG1(ulPid, PS_PRINT_WARNING, "TTF_MemBlkAlloc_Debug,Poolid %d is invalid! \n",
            (VOS_INT32)ulPoolId);
        return VOS_NULL_PTR;
    }

    /* ��ȡ�ڴ�� */
    pstTtfMemPool = TTF_MEM_GET_POOL(ulPoolId);

    if ( PS_TRUE != pstTtfMemPool->usUsedFlag )
    {
        TTF_LOG1(ulPid, PS_PRINT_WARNING, "TTF_MemBlkAlloc_Debug,Pool %d is not used! \n",
            (VOS_INT32)ulPoolId);
        return VOS_NULL_PTR;
    }

    /* �ڴ�أ��������Ϊ�����ӡ�澯 */
    if ( 0 == usLen )
    {
        TTF_LOG2(ulPid, PS_PRINT_WARNING, "TTF_MemBlkAlloc_Debug, Invalid usLen = %d for the Pool %d ! \n", usLen, (VOS_INT32)ulPoolId);
        return VOS_NULL_PTR;
    }

    /* ���������󳤶�, ���ӡ�澯 */
    if ( usLen > pstTtfMemPool->usMaxByteLen )
    {
        TTF_LOG2(ulPid, PS_PRINT_WARNING, "TTF_MemBlkAlloc_Debug, usLen = %d exceed the Pool %d ranger! \n", usLen, (VOS_INT32)ulPoolId);
        return VOS_NULL_PTR;
    }

    /* ���ڴ�صĶ������Ѱ�Һ��ʵ�TTF_MEM */
    ucMostFitLev = TTF_GET_LEN_INDEX(pstTtfMemPool, usLen);

    for(ucClusterId = ucMostFitLev;
        ucClusterId < pstTtfMemPool->ucClusterCnt;
        ucClusterId ++)
    {
        pstTtfMemCluster  = &(pstTtfMemPool->astClusterTable[ucClusterId]);

        lLockKey = VOS_SplIMP();

        if (0 != pstTtfMemCluster->usFreeCnt)
        {
            /* ��ȡһ��û��ʹ�õ�TTF_MEM_ST��� */
            pMem    = pstTtfMemCluster->apstFreeStack[--pstTtfMemCluster->usFreeCnt];
            pstTtfMemPool->usTtfMemUsedCnt++;

            VOS_Splx(lLockKey);

            TTF_MEM_RESET(pMem);

            lLockKey = VOS_SplIMP();
            pstTtfMemPool->ulTtfMemUsedByteSize += pMem->usLen;
            VOS_Splx(lLockKey);


            if (VOS_NULL_PTR != pstTtfMemPool->pMemAllocEvent)
            {
                pstTtfMemPool->pMemAllocEvent(pstTtfMemPool->usTtfMemUsedCnt, pstTtfMemPool->ulTtfMemUsedByteSize);
            }

            TTF_MEM_DBG(pMem, usFileID, usLineNum);

           (*g_apusTtfMemConAllocFailCnt[ulPoolId]) = 0;

            TTF_MEM_RCOV_ALLOC_SUCC(ulPoolId);

           return pMem;
        }

        VOS_Splx(lLockKey);
    }

    /* ��һ�γ����ڴ�����ʧ���ϱ�������ÿ�ﵽ�����ϱ�һ�� */
    if (0 == ((*g_apusTtfMemAllocFailCnt[ulPoolId]) & g_usTtfMemAllocFailCntThreshold ))
    {
        (*g_apusTtfMemExcThresholdCnt[ulPoolId])++;

        /* ����Errlog�¼���failCnt���ں���ά���ģ���������Ҫ+1 */
        TTF_MNTN_ErrlogTtfMemAllocFail(ulPid, (VOS_UINT8)ulPoolId, usFileID, usLineNum, (*g_apusTtfMemAllocFailCnt[ulPoolId]) + 1);

        /* �ϱ��ڴ������ʧ���¼� */
        TTF_BlkMemUsedInfoEventRpt(ulPid, ulPoolId, TRIG_TYPE_ALLOC_FAIL);
    }

    (*g_apusTtfMemAllocFailCnt[ulPoolId])++;
    (*g_apusTtfMemConAllocFailCnt[ulPoolId])++;

    TTF_MEM_RCOV_ALLOC_FAIL(ulPoolId, (*g_apusTtfMemConAllocFailCnt[ulPoolId]), ulPid, usFileID, usLineNum);

    return VOS_NULL_PTR;
} /* TTF_MemBlkAlloc_Debug */

VOS_VOID TTF_ExtMemFree( void **ppstExtMem )
{
    TTF_LOG(WUEPS_PID_RLC, PS_PRINT_ERROR, "call TTF_ExtMemFree is invalid! \n");


    return;

}/* TTF_ExtMemFree */

TTF_MEM_ST * TTF_DataTransformTtfMem_Debug(VOS_UINT16 usFileID, VOS_UINT16 usLineNum,VOS_UINT8* pucData,VOS_UINT16 usLen, VOS_VOID*  pstIMM)
{
    TTF_LOG2(WUEPS_PID_RLC, PS_PRINT_ERROR,
        "call TTF_DataTransformTtfMem_Debug is invalid, File:%d, Line:%d! \n", usFileID, usLineNum);


    return VOS_NULL_PTR;
} /* TTF_DataTransformTtfMem_Debug */

VOS_UINT32 TTF_GetLocalFreeMemCnt (VOS_UINT32 ulPoolId)
{
    TTF_MEM_POOL_STRU                  *pstTtfMemPool;


    if (ulPoolId >= TTF_MEM_POOL_ID_BUTT)
    {
        TTF_LOG1(WUEPS_PID_RLC, PS_PRINT_WARNING, "TTF_GetCCoreReservedCnt,Poolid %d is invalid! \n",
            (VOS_INT32)ulPoolId);
        return PS_FAIL;
    }

    if ((TTF_MEM_POOL_ID_DL_SHARE == ulPoolId) || (TTF_MEM_POOL_ID_EXT == ulPoolId))
    {
        TTF_LOG1(WUEPS_PID_RLC, PS_PRINT_WARNING, "TTF_GetCCoreReservedCnt,Poolid %d is invalid! \n",
            (VOS_INT32)ulPoolId);
        return PS_FAIL;
    }

    /* ��ȡ�ڴ�� */
    pstTtfMemPool = TTF_MEM_GET_POOL(ulPoolId);

    if ((TTF_MEM_POOL_ID_UL_IP_DATA == ulPoolId) || (TTF_MEM_POOL_ID_DL_PDU == ulPoolId))
    {
        /* V9R1�汾�϶�������IP�����ݰ��ڴ�ػ���C-CPU����PDU�ڴ�أ����ظ��ڴ�صڶ�����λʣ���ڴ���� */
        return pstTtfMemPool->astClusterTable[pstTtfMemPool->ucClusterCnt - 2].usFreeCnt;
    }
    else
    {
        /* �ڴ�����λʣ���ڴ���� */
        return pstTtfMemPool->astClusterTable[pstTtfMemPool->ucClusterCnt - 1].usFreeCnt;
    }
} /* TTF_GetLocalFreeMemCnt */



VOS_VOID  TTF_MemPoolInfoEventRpt(VOS_UINT8 ucPoolId)
{
    TTF_MEM_POOL_STRU                      *pstTtfMemPool;
    VOS_UINT16                              usTtfPoolUsedCnt = 0;


    /* C-CPU����IP�������ڴ����Ҫ��ADQԤ��128���ڴ� */
    if (TTF_MEM_POOL_ID_UL_IP_DATA == ucPoolId)
    {
        usTtfPoolUsedCnt = TTF_MEM_UL_IP_DATA_POOL_RSV_CNT;
    }

    pstTtfMemPool = TTF_MEM_GET_POOL(ucPoolId);

    if (PS_TRUE != pstTtfMemPool->usUsedFlag)
    {
        TTF_LOG1(WUEPS_PID_RLC, PS_PRINT_WARNING, "TTF_MemPoolInfoEventRpt, Pool %d is not used!\n", ucPoolId);
        return;
    }

    if (pstTtfMemPool->usTtfMemUsedCnt> usTtfPoolUsedCnt)
    {
        TTF_LOG2(WUEPS_PID_RLC, PS_PRINT_ERROR, "TTF_MEM_BLK Is Leak,Pool %d,UsedCnt %d \n!",
            ucPoolId, pstTtfMemPool->usTtfMemUsedCnt);

        TTF_BlkMemUsedInfoEventRpt(WUEPS_PID_RLC, ucPoolId, TRIG_TYPE_LEAK);
        TTF_MemReportMemLeakMsg(WUEPS_PID_RLC);
    }
    else
    {
        TTF_LOG1(WUEPS_PID_RLC, PS_PRINT_NORMAL, "NORMAL: TTF_MemPoolInfoEventRpt, Pool %d Normal\n!",ucPoolId);
        TTF_MemSndNormalEvent(ucPoolId) ;
    }


    return;
} /* TTF_MemPoolInfoEventRpt */



VOS_VOID TTF_MemCcpuCheckPoolLeak( VOS_VOID )
{
    VOS_UINT8                          ucPoolId;

    /* v9r1 �ڴ���ʱҲ��Ҫ�ͷŹ���cds������е����� */
    CDS_FreeIpfDlSrcMem();

    /* V9R1�汾��ֻ��TTF_MEM_POOL_ID_DL_PDU��TTF_MEM_POOL_ID_UL_DATA ��
    TTF_MEM_POOL_ID_UL_IP_DATA�����ڴ����Ч */
    for (ucPoolId = TTF_MEM_POOL_ID_DL_SHARE; ucPoolId < TTF_MEM_POOL_ID_BUTT; ++ucPoolId)
    {
        if ((TTF_MEM_POOL_ID_DL_SHARE != ucPoolId) && (TTF_MEM_POOL_ID_EXT != ucPoolId))
        {
            TTF_MemPoolInfoEventRpt(ucPoolId);
        }
    }

    return;
}



VOS_UINT32 TTF_GetDlIpFreeMemCnt (VOS_VOID)
{
    return TTF_GetLocalFreeMemCnt(TTF_MEM_POOL_ID_DL_PDU);
}



VOS_UINT32 TTF_GetUlIpFreeMemCnt (VOS_VOID)
{
    return TTF_GetLocalFreeMemCnt(TTF_MEM_POOL_ID_UL_IP_DATA);
}



VOS_VOID TTF_MemFreeShowMntnInfo( VOS_VOID )
{
    TTF_MEM_FREE_INVALIED_MEM_INFO_STRU    *pstInvalidMemInfo   = &g_stTtfMemFreeMntnEntity.astTtfMemFreeInvalidMemInfo;
    TTF_MEM_FREE_NULL_PTR_INFO_STRU        *pstNullPtrInfo      = &g_stTtfMemFreeMntnEntity.astTtfMemFreeNullPtrInfo;
    TTF_MEM_FREE_NULL_PTR_INFO_STRU        *pstNullPPtrInfo     = &g_stTtfMemFreeMntnEntity.astTtfMemFreeNullPPtrInfo;

    vos_printf("TTF MEM FREE ��ά�ɲ���Ϣ :\n");
    vos_printf("===========================================\n");
    vos_printf("TTF MEM FREE �Ƿ��ڴ���Ϣ :\n");
    vos_printf("TTF MEM FREE �յ��ķǷ��ڴ����       : %d \n", (VOS_INT32)pstInvalidMemInfo->ulInvalidTtfMemCnt);
    vos_printf("TTF MEM FREE �Ƿ�TTF MEM �ڴ��ַ     : 0x%u \n", (VOS_INT32)pstInvalidMemInfo->ulInvalidTtfMemAddr);
    vos_printf("TTF MEM FREE �Ƿ�TTF MEM �ڴ��ļ�ID   : %d \n", pstInvalidMemInfo->usTtfMemFreeFileId);
    vos_printf("TTF MEM FREE �Ƿ�TTF MEM �ڴ��к���Ϣ : %d \n", pstInvalidMemInfo->usTtfMemFreeLineNum);
    vos_printf("TTF MEM FREE �Ƿ�TTF MEM �ڴ�����     : %d \n", pstInvalidMemInfo->ucInvalidTtfMemType);
    vos_printf("TTF MEM FREE �Ƿ�TTF MEM �ڴ�ֵ       : %d \n", pstInvalidMemInfo->usInvalidTtfMemValue);

    vos_printf("===========================================\n");
    vos_printf("TTF MEM FREE ��ָ����Ϣ :\n");
    vos_printf("TTF MEM FREE һ��ָ��Ϊ�յĴ���       : %d \n", (VOS_INT32)pstNullPtrInfo->ulTtfMemFreeNullPtrCnt);
    vos_printf("TTF MEM FREE ����ָ���ļ�ID           : %d \n", pstNullPtrInfo->usTtfMemFreeFileId);
    vos_printf("TTF MEM FREE ����ָ���ڴ��к���Ϣ     : %d \n", pstNullPtrInfo->usTtfMemFreeLineNum);

    vos_printf("TTF MEM FREE ����ָ��Ϊ�յĴ���       : %d \n", (VOS_INT32)pstNullPPtrInfo->ulTtfMemFreeNullPtrCnt);
    vos_printf("TTF MEM FREE ����ָ���ڴ��ļ�ID       : %d \n", pstNullPPtrInfo->usTtfMemFreeFileId);
    vos_printf("TTF MEM FREE ����ָ���ڴ��к���Ϣ     : %d \n", pstNullPPtrInfo->usTtfMemFreeLineNum);

    vos_printf("TTF MEM Pool Status:\n");
    vos_printf("          TTF_MEM_POOL_ID_DL_PDU      : %d\n", g_astTtfMemPool[TTF_MEM_POOL_ID_DL_PDU].usUsedFlag);
    vos_printf("          TTF_MEM_POOL_ID_UL_DATA     : %d\n", g_astTtfMemPool[TTF_MEM_POOL_ID_UL_DATA].usUsedFlag);
    vos_printf("          TTF_MEM_POOL_ID_UL_IP_DATA  : %d\n", g_astTtfMemPool[TTF_MEM_POOL_ID_UL_IP_DATA].usUsedFlag);

}


#endif



VOS_UINT32 TTF_MemBlkInit(VOS_VOID)
{
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulBaseAddr;
    VOS_UINT8                          *pucBaseAddr;
    VOS_UINT32                          ulTtfMemPoolLength;
    VOS_UINT8                           ucLoop;
    VOS_UINT32                          ulTtfMemCfgLength;

    /*��ʼ������ֻ�ᱻPID��ʼ������˳�����*/
    if ( VOS_TRUE == g_ulTtfMemInitFlag )
    {
        PS_LOG(WUEPS_PID_RLC, 0, PS_PRINT_WARNING,
               "TTF_MemBlkInit, Warning, Memory pools were already initialized!\n");

        return PS_SUCC;
    }

    /* �����ڴ����Ϣ */
    PS_MEM_SET(g_astTtfMemPool, 0, sizeof(TTF_MEM_POOL_STRU)* TTF_MEM_POOL_ID_BUTT);

    /*��ȡTTF�̶��ڴ��ַ*/
    TTF_GetTTFMemSection(&ulBaseAddr, &ulTtfMemPoolLength);

    /*��ȡ�����ļ�*/
    ulRet = TTF_MemReadNvCfg(&g_stTtfMemSolution);
    if (PS_SUCC ==ulRet)
    {
        ulTtfMemCfgLength = TTF_MemNvCfgTotalLength(&g_stTtfMemSolution);
        if ( ulTtfMemCfgLength > ulTtfMemPoolLength)
        {
            PS_LOG2(WUEPS_PID_RLC, 0, PS_PRINT_ERROR,
                "TTF_MemBlkInit Fail, TTF_MemBlkInit Total Length Invalid <1> <2> \n",
                (VOS_INT32)ulTtfMemCfgLength, (VOS_INT32)ulTtfMemPoolLength);
            ulRet = PS_FAIL;
        }
    }

    if (PS_FAIL == ulRet)
    {
        TTF_MemSetDefaultNvCfg(&g_stTtfMemSolution);
    }

    pucBaseAddr = (VOS_UINT8 *)ulBaseAddr;
    for (ucLoop = 0; ucLoop < g_stTtfMemSolution.ucPoolCnt; ucLoop++ )
    {
        /* C-CPU�ϸ����ڴ���Ƿ���Ч�ж� */
        if ( TTF_MEM_MASK_POOL(ucLoop) == (TTF_MEM_MASK_POOL(ucLoop) & g_stTtfMemSolution.ucPoolMask) )
        {
           ulRet = TTF_MemPoolCreate(ucLoop, &g_stTtfMemSolution.astTtfMemPoolCfgInfo[ucLoop],
                                         &pucBaseAddr, &ulTtfMemPoolLength);
           if (PS_FAIL == ulRet)
           {
               return PS_FAIL;
           }
        }
    }

    /* ʵʼ���ڴ汣��ʵ�� */
    TTF_MemRcovEntInit();

    /*�����ѱ���ʼ���ı�־*/
    g_ulTtfMemInitFlag = VOS_TRUE;

    return PS_SUCC;
} /* TTF_MemBlkInit */

#if (FEATURE_PTM == FEATURE_ON)


VOS_VOID TTF_MNTN_ErrlogTtfMemAllocFail(VOS_UINT32 ulPid, VOS_UINT8 ucPoolId, VOS_UINT16 usFileId, VOS_UINT16 usLine, VOS_UINT32 ulAllocFailCnt)
{
    TTF_MNTN_ERR_LOG_MEM_ALLOC_FAIL_STRU    stErrLogMemFail;
    TTF_MNTN_ERR_LOG_MEM_ALLOC_INFO_STRU   *pstMemAllocInfo;
    TTF_MEM_POOL_STRU                      *pstTtfMemPool;
    TTF_MEM_ST                             *pstMem;
    VOS_UINT32                              ulIndx;
    VOS_UINT8                               ucAlmLev;
    VOS_UINT16                              usMemRptCnt;
    MODEM_ID_ENUM_UINT16                    enModemId;



    ucAlmLev    = TTF_ERR_LOG_GET_ALM_LEV(TTF_ERR_LOG_ALM_ID_TTF_MEM_FAIL);
    enModemId   = VOS_GetModemIDFromPid(ulPid);

    if (!TTF_ERR_LOG_NEED_RPT_LEV(enModemId, ucAlmLev))
    {
        return;
    }

    usMemRptCnt = 0;

    TTF_ERR_LOG_FILL_HEADER(
        &(stErrLogMemFail.stHeader),
        enModemId,
        TTF_ERR_LOG_ALM_ID_TTF_MEM_FAIL,
        ucAlmLev,
        (sizeof(TTF_MNTN_ERR_LOG_MEM_ALLOC_FAIL_STRU) - sizeof(OM_ERR_LOG_HEADER_STRU)));

    stErrLogMemFail.ucPoolId            = ucPoolId;
    stErrLogMemFail.ulAllocFailCnt      = ulAllocFailCnt;
    stErrLogMemFail.usFileId            = usFileId;
    stErrLogMemFail.usLine              = usLine;

    PS_MEM_SET(stErrLogMemFail.aucRsv, 0, sizeof(stErrLogMemFail.aucRsv));

    pstTtfMemPool = TTF_MEM_GET_POOL(ucPoolId);

    for(ulIndx = 0; ulIndx < pstTtfMemPool->usTtfMemTotalCnt;  ulIndx++)
    {
        pstMem          = pstTtfMemPool->pstTtfMemStStartAddr + ulIndx;
        pstMemAllocInfo = &(stErrLogMemFail.astAllocInfo[usMemRptCnt]);

        TTF_ERR_LOG_RCD_DBG(pstMemAllocInfo, pstMem, usMemRptCnt);

        if (usMemRptCnt >= TTF_ERR_LOG_RCD_MEM_CNT)
        {
            break;
        }
    }

    TTF_MNTN_ErrlogBufPut(ulPid, (VOS_CHAR *)&stErrLogMemFail, sizeof(TTF_MNTN_ERR_LOG_MEM_ALLOC_FAIL_STRU));
}

#endif

#ifdef  __cplusplus
  #if  __cplusplus
  }
  #endif
#endif

