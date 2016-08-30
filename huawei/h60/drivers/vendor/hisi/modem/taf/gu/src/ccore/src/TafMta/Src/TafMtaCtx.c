

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_TAF_MTA_CTX_C

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "NVIM_Interface.h"

#include "TafMtaCtx.h"
#include "TafMtaComm.h"



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/* MTAģ��������������Դ */
TAF_MTA_CONTEXT_STRU            g_stTafMtaCtx;

/*****************************************************************************
  3 ��������
*****************************************************************************/

TAF_MTA_AGPS_CTX_STRU*  TAF_MTA_GetMtaAgpsCtxAddr( VOS_VOID )
{
    return &(g_stTafMtaCtx.stAgpsCtx);
}


TAF_MTA_CONTEXT_STRU*  TAF_MTA_GetMtaCtxAddr( VOS_VOID )
{
    return &(g_stTafMtaCtx);
}


TAF_MTA_TIMER_CTX_STRU*  TAF_MTA_GetTimerCtxAddr( VOS_VOID )
{
    return g_stTafMtaCtx.astTimerCtx;
}


TAF_MTA_RF_LCD_CTX_STRU* TAF_MTA_GetRfLcdCtxAddr( VOS_VOID )
{
    return &g_stTafMtaCtx.stMtaRfLcdCtx;
}


VOS_VOID  TAF_MTA_InitRfLcdIntrusionCtx(
    TAF_MTA_RF_LCD_CTX_STRU            *pstRfLcdCtx
)
{
    VOS_UINT32                          ulRet;
    MTC_RF_LCD_TIMER_INTERVAL_STRU      stRfLcdTimerInterval;
    MTC_NVIM_RF_LCD_CFG_STRU            stRfLcdCfg;

    /* ��ʼ�� */
    PS_MEM_SET(&stRfLcdCfg, 0x0, sizeof(MTC_NVIM_RF_LCD_CFG_STRU));
    PS_MEM_SET(&stRfLcdTimerInterval, 0x0, sizeof(MTC_RF_LCD_TIMER_INTERVAL_STRU));
    PS_MEM_SET(pstRfLcdCtx, 0x0, sizeof(TAF_MTA_RF_LCD_CTX_STRU));


    /* ��ȡen_NV_Item_MTC_RF_LCD_TIMER_INTERVAL_CFG NV�� */
    ulRet = NV_Read(en_NV_Item_MTC_RF_LCD_TIMER_INTERVAL_CFG,
                    &stRfLcdTimerInterval,
                    sizeof(MTC_RF_LCD_TIMER_INTERVAL_STRU));
    if (VOS_OK != ulRet)
    {
        MTA_WARNING1_LOG("MTC_ReadRfLcdCfgNv: Read Nv Fail.", en_NV_Item_MTC_RF_LCD_TIMER_INTERVAL_CFG);
        pstRfLcdCtx->ulTimerInterVal = TAF_MTA_RF_LCD_TIMER_INTERVAL_DEFAULT;
    }
    else
    {
        pstRfLcdCtx->ulTimerInterVal = stRfLcdTimerInterval.ulTimerInterval * TIMER_S_TO_MS_1000;
    }

    /* ��ȡen_NV_Item_MTC_RF_LCD_CFG NV�� */
    ulRet = NV_Read(en_NV_Item_MTC_RF_LCD_CFG,
                    &stRfLcdCfg,
                    sizeof(MTC_NVIM_RF_LCD_CFG_STRU));
    if (VOS_OK != ulRet)
    {
        pstRfLcdCtx->usEnableBitMap = 0;
    }
    else
    {
        pstRfLcdCtx->usEnableBitMap = stRfLcdCfg.usEnableBitMap;
    }

    pstRfLcdCtx->usNewMipiClk    = 0x0000;
    pstRfLcdCtx->usReportMipiClk = 0x0000;

    return;
}
VOS_VOID  TAF_MTA_InitCmdBufferQueue(
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBufferQueue
)
{
    VOS_UINT32                          i;

    for ( i = 0; i < TAF_MTA_MAX_CMD_BUFFER_QUEUE_SIZE; i++ )
    {
        pstCmdBufferQueue[i].enTimerId      = TI_TAF_MTA_TIMER_BUTT;
        pstCmdBufferQueue[i].pucMsgInfo     = VOS_NULL_PTR;
        pstCmdBufferQueue[i].ulMsgInfoLen   = 0;
    }

    return;
}


TAF_MTA_CMD_BUFFER_STRU  *TAF_MTA_GetCmdBufferQueueAddr( VOS_VOID )
{
    return g_stTafMtaCtx.astCmdBufferQueue;
}


VOS_VOID  TAF_MTA_DelItemInCmdBufferQueue(
    TAF_MTA_TIMER_ID_ENUM_UINT32        enTimerId
)
{
    VOS_UINT32                          i;
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBufferQueue;

    pstCmdBufferQueue = TAF_MTA_GetCmdBufferQueueAddr();

    for ( i = 0; i < TAF_MTA_MAX_CMD_BUFFER_QUEUE_SIZE; i++ )
    {
        if (enTimerId == pstCmdBufferQueue[i].enTimerId)
        {
            /* ����л������Ϣ���ͷ���Ϣ�ڴ� */
            if (VOS_NULL_PTR != pstCmdBufferQueue[i].pucMsgInfo)
            {
                PS_MEM_FREE(UEPS_PID_MTA, pstCmdBufferQueue[i].pucMsgInfo);
            }

            /* ��ʼ�� */
            pstCmdBufferQueue[i].pucMsgInfo     = VOS_NULL_PTR;
            pstCmdBufferQueue[i].enTimerId      = TI_TAF_MTA_TIMER_BUTT;
            pstCmdBufferQueue[i].ulMsgInfoLen   = 0;

            break;
        }
    }

    return;
}


VOS_VOID  TAF_MTA_SaveItemInCmdBufferQueue(
    TAF_MTA_TIMER_ID_ENUM_UINT32        enTimerId,
    VOS_UINT8                          *pucMsgInfo,
    VOS_UINT32                          ulMsgInfoLen
)
{
    VOS_UINT32                          i;
    VOS_UINT8                          *pucMsgBuf;
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBufferQueue;

    /* �����Ϣָ��Ϊ�գ�ֱ�ӷ��� */
    if (VOS_NULL_PTR == pucMsgInfo)
    {
        return;
    }

    pstCmdBufferQueue = TAF_MTA_GetCmdBufferQueueAddr();

    for ( i = 0; i < TAF_MTA_MAX_CMD_BUFFER_QUEUE_SIZE; i++ )
    {
        if (TI_TAF_MTA_TIMER_BUTT == pstCmdBufferQueue[i].enTimerId)
        {
            break;
        }
    }

    /* �ж���ǰ��ѭ�������Ƿ��Ѿ����ˣ����������ֱ�ӷ��أ������쳣��ӡ */
    if ( i >= TAF_MTA_MAX_CMD_BUFFER_QUEUE_SIZE )
    {
        /* ����������� */
        MTA_WARNING2_LOG("TAF_MTA_SaveItemInCmdBufferQueue, Buffer Full, Timer Id :",
                         UEPS_PID_MTA,
                         enTimerId);

        return;
    }

    /* �ڶ���������Ӧ����Ϣ�������ڴ棬������Ϣ��ֵ */
    pucMsgBuf = (VOS_UINT8 *)PS_MEM_ALLOC(UEPS_PID_MTA, ulMsgInfoLen);
    if (VOS_NULL_PTR == pucMsgBuf)
    {
        /* �ڴ����ʧ�� */
        MTA_WARNING2_LOG("TAF_MTA_SaveItemInCmdBufferQueue, Mem Alloc Fail, Timer Id :",
                         UEPS_PID_MTA,
                         enTimerId );
        return;
    }

    /* ��ֵ */
    PS_MEM_CPY((VOS_UINT8 *)pucMsgBuf, pucMsgInfo, ulMsgInfoLen);
    pstCmdBufferQueue[i].pucMsgInfo     = pucMsgBuf;
    pstCmdBufferQueue[i].enTimerId      = enTimerId;
    pstCmdBufferQueue[i].ulMsgInfoLen   = ulMsgInfoLen;

    return;
}


TAF_MTA_CMD_BUFFER_STRU  *TAF_MTA_GetItemFromCmdBufferQueue(
    TAF_MTA_TIMER_ID_ENUM_UINT32                enTimerId
)
{
    VOS_UINT32                          i;
    TAF_MTA_CMD_BUFFER_STRU            *pstCmdBufferQueue;


    pstCmdBufferQueue = TAF_MTA_GetCmdBufferQueueAddr();

    /* ��������, ��ȡenTimerIdָ������Ϣ�ڵ� */
    for ( i = 0; i < TAF_MTA_MAX_CMD_BUFFER_QUEUE_SIZE; i++ )
    {
        if (enTimerId == pstCmdBufferQueue[i].enTimerId)
        {
            break;
        }
    }

    /* �ж���ǰ��ѭ�������Ƿ��Ѿ����ˣ����������ֱ�ӷ��أ������쳣��ӡ */
    if ( i >= TAF_MTA_MAX_CMD_BUFFER_QUEUE_SIZE )
    {
        /* δ�ҵ� */
        MTA_WARNING2_LOG("TAF_MTA_GetItemFromCmdBufferQueue, Msg Not Found, Timer Id :",
                         UEPS_PID_MTA,
                         enTimerId);

        return VOS_NULL_PTR;
    }

    /* ���������Ϣָ�뷵�� */
    return &pstCmdBufferQueue[i];
}


VOS_VOID TAF_MTA_InitRefClockInfo(
    TAF_MTA_REFCLOCK_INFO_STRU         *pstRefClockInfo
)
{
    PS_MEM_SET(pstRefClockInfo, 0, sizeof(TAF_MTA_REFCLOCK_INFO_STRU));

    pstRefClockInfo->enRptFlg       = AT_MTA_CMD_RPT_FLG_OFF;
    pstRefClockInfo->ulFreq         = TAF_MTA_REFCLOCK_DEFAULT_FREQ;
    pstRefClockInfo->ulPrecision    = TAF_MTA_REFCLOCK_DEFAULT_PRECISION;
    pstRefClockInfo->enStatus       = PHY_MTA_REFCLOCK_UNLOCKED;

    return;
}


TAF_MTA_REFCLOCK_INFO_STRU* TAF_MTA_GetRefClockInfo(VOS_VOID)
{
    return &(g_stTafMtaCtx.stAgpsCtx.stRefClockInfo);
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
