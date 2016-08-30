

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "ucom_pcm.h"
#include "ucom_comm.h"
#include "om_log.h"
#include "med_drv_sio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    ��ά�ɲ���Ϣ�а�����C�ļ���ź궨��
*****************************************************************************/
/*lint -e(767)*/
#define THIS_FILE_ID                    OM_FILE_ID_UCOM_PCM_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
UCOM_ALIGN(32)
DRV_DMA_CXCFG_STRU                      g_astSmartPaInDmaPara[2] = {0};


UCOM_PCM_BUFFER_STRU                    g_stUcomPcmBuff = {0};

UCOM_PCM_CFG_STRU                       g_stSmartPaCfg = {0};

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_VOID UCOM_PCM_SmartPaPcmInit(VOS_VOID)
{
    UCOM_PCM_CFG_STRU      *pstSmartPaCfg;
    VOS_VOID               *pvSpaInDma;                                         /* SMART PA DMA����ָ�� */
    VOS_VOID               *pvPcmBuff;

    pstSmartPaCfg           = UCOM_PCM_GetSpaCfgPtr();

    /* 1. Smart PA�ѳ�ʼ���������в��������ش��� */
    if (VOS_YES == pstSmartPaCfg->enIsInit)
    {
        OM_LogError(UCOM_PCM_SmartPaPcmInitFail);
        return;
    }

    /* 2. ��ʼ��EMACͨ������ʵ��͹���PCM����BUFF�ṹ�壬���ݳ�ʼ��Ϊȫ0 */
    pvPcmBuff              = (VOS_VOID *)&g_stUcomPcmBuff;
    pvSpaInDma             = (VOS_VOID *)&g_astSmartPaInDmaPara[0];

    UCOM_MemSet(pvPcmBuff, 0, sizeof(g_stUcomPcmBuff));
    UCOM_MemSet(pvSpaInDma, 0, sizeof(g_astSmartPaInDmaPara));

    pstSmartPaCfg->pshwDestVirtualAddr = g_stUcomPcmBuff.ashwSmartPaBuffA;

    /* 3. ����־��ΪSMART PA�ѳ�ʼ�� */
    pstSmartPaCfg->enIsInit = VOS_YES;

    OM_LogInfo(UCOM_PCM_SmartPaPcmInitOk);

    return;
}


VOS_UINT32 UCOM_PCM_SmartPaStartLoopDma(
                VOS_UINT16              uhwFrameLenBytes,
                VOS_UINT32              uwSampleRate,
                VOS_UINT16              uhwSmartPaMaster,
                VOS_UINT16              usChNum,
                DRV_DMA_INT_FUNC        pfuncIsr)
{
    VOS_UINT32                    uwRet;
    VOS_UINT32                    uwDestAddrA, uwDestAddrB;   /* SmartPA�����DMAͨ����Ŀ�ĵ�ַ */
    VOS_UINT32                    uwLliAddrA, uwLliAddrB;     /* SmartPA�����DMAͨ����LLI��ַ */
    DRV_DMA_CXCFG_STRU           *pstDmaCfgA     = VOS_NULL;
    DRV_DMA_CXCFG_STRU           *pstDmaCfgB     = VOS_NULL;
    UCOM_PCM_BUFFER_STRU         *pstPcmBuffTmp  = VOS_NULL;
    UCOM_PCM_CFG_STRU            *pstSmartPaCfg  = UCOM_PCM_GetSpaCfgPtr();
    DRV_SIO_INT_MASK_ENUM_UINT32  enSioMask     = 0;

    DRV_SIO_SmartPa_Close();

    DRV_SIO_SmartPa_Open(enSioMask,
                         uwSampleRate,
                         uhwSmartPaMaster,
                         VOS_NULL,
                         0);

    /* ���Smart PA�Ƿ��ѳ�ʼ�� */
    if (VOS_NO == pstSmartPaCfg->enIsInit)
    {
        OM_LogError(UCOM_PCM_SmartPaStartLoopDma_ErrStatus);
        return UCOM_RET_ERR_STATE;
    }

    /* �������Ƿ�Ƿ�,ͨ����0-15 */
    if (usChNum >= DRV_DMA_MAX_CHANNEL_NUM)
    {
        OM_LogError(UCOM_PCM_SmartPaStartLoopDma_ErrChanNum);
        return UCOM_RET_ERR_PARA;
    }

    /* �����˳��ȵĺϷ��� */
    if (   (0 == uhwFrameLenBytes)
        || (uhwFrameLenBytes > ((UCOM_PCM_MAX_FRAME_LENGTH * sizeof(VOS_INT16)) * UCOM_PCM_I2S_CHANNEL_NUM)))
    {
        OM_LogError(UCOM_PCM_SmartPaStartLoopDma_InvalidFrameLen);
        return UCOM_RET_ERR_PARA;
    }

    /* ����SMART PA���ƽṹ���е�֡���ȣ��ó��Ⱦ�����ÿ�ΰ��˵ĳ��� */
    pstSmartPaCfg->uhwDataLenBytes = uhwFrameLenBytes;

    /* ��ȡDMA����ȫ�ֱ�����Uncache���ʵ�ַ */
    pstDmaCfgA   = (DRV_DMA_CXCFG_STRU *)UCOM_GetUncachedAddr((VOS_UINT32)&g_astSmartPaInDmaPara[0]);
    pstDmaCfgB   = (DRV_DMA_CXCFG_STRU *)UCOM_GetUncachedAddr((VOS_UINT32)&g_astSmartPaInDmaPara[1]);

    pstPcmBuffTmp= (&g_stUcomPcmBuff);

    /* ��ֹSmartPA��DMA */
    DRV_DMA_Stop(usChNum);

    /*�л�buffer������DestAddr��ֵ*/
    if (pstSmartPaCfg->pshwDestVirtualAddr == pstPcmBuffTmp->ashwSmartPaBuffA)
    {
        uwDestAddrA = UCOM_GetUncachedAddr((VOS_UINT32)&pstPcmBuffTmp->ashwSmartPaBuffB[0]);
        uwDestAddrB = UCOM_GetUncachedAddr((VOS_UINT32)&pstPcmBuffTmp->ashwSmartPaBuffA[0]);
    }
    else
    {
        uwDestAddrA = UCOM_GetUncachedAddr((VOS_UINT32)&pstPcmBuffTmp->ashwSmartPaBuffA[0]);
        uwDestAddrB = UCOM_GetUncachedAddr((VOS_UINT32)&pstPcmBuffTmp->ashwSmartPaBuffB[0]);
    }

    /*LLI����ĵ�ַ*/
    uwLliAddrA = (VOS_UINT32)pstDmaCfgA;
    uwLliAddrB = (VOS_UINT32)pstDmaCfgB;
    UCOM_MemSet(pstDmaCfgA, 0, sizeof(g_astSmartPaInDmaPara));

    /*����DMA���ò���,����Ϊ�������ӣ�ʹ��SMART_PAͨ��LOOP���ã�ÿ���ڵ��ϱ�һ���ж�*/
    pstDmaCfgA->uwLli            = DRV_DMA_LLI_LINK(uwLliAddrB);
    pstDmaCfgA->uhwACount        = uhwFrameLenBytes;
    pstDmaCfgA->uwSrcAddr        = DRV_SIO_GetRxRegAddr(DRV_SIO_GetSmartPaCfgPtr());
    pstDmaCfgA->uwDstAddr        = uwDestAddrA;
    pstDmaCfgA->uwConfig         = DRV_DMA_SMARTPA_SIO_MEM_CFG;

    pstDmaCfgB->uwLli            = DRV_DMA_LLI_LINK(uwLliAddrA);
    pstDmaCfgB->uhwACount        = uhwFrameLenBytes;
    pstDmaCfgB->uwSrcAddr        = DRV_SIO_GetRxRegAddr(DRV_SIO_GetSmartPaCfgPtr());
    pstDmaCfgB->uwDstAddr        = uwDestAddrB;
    pstDmaCfgB->uwConfig         = DRV_DMA_SMARTPA_SIO_MEM_CFG;

    /*����ͨ������SmartPA��HIFI�����ݰ���, ��ע��DMA�жϴ�����*/
    uwRet = DRV_DMA_StartWithCfg(usChNum,
                                 pstDmaCfgA,
                                 pfuncIsr,
                                 0);

    return uwRet;
}


VOS_UINT32 UCOM_PCM_SmartPaGetAvailVirtAddr(VOS_VOID)
{
    VOS_UINT32              ulAvailableVirtualAddr;
    VOS_UINT32              ulDestAddr, ulDestAddrB;   /* SPA���ŵ�DMAͨ����Ŀ�ĵ�ַ */
    UCOM_PCM_BUFFER_STRU   *pstPcmBuffTmp = (&g_stUcomPcmBuff);
    UCOM_PCM_CFG_STRU      *pstSmartPaCfg = UCOM_PCM_GetSpaCfgPtr();

    ulDestAddrB  = UCOM_GetUncachedAddr((VOS_UINT32)&pstPcmBuffTmp->ashwSmartPaBuffB[0]);

    /*��ȡͨ��5Ŀ�ĵ�ַ�Ĵ���*/
    ulDestAddr   = UCOM_RegRd(DRV_DMA_CX_DES_ADDR(UCOM_PCM_DMAC_CHN_SPA));

    /*��Ŀ�ĵ�ַ�Ѿ�ΪBufferB��Χ��(LOOPģʽ)��Ŀ�ĵ�ַΪBufferA�����һ��λ��(��LOOPģʽ)*/
    /*���߼�������BufferA��BufferB��ַ��������BufferA��ǰ*/
    if((ulDestAddr >= (ulDestAddrB - sizeof(VOS_UINT32)))
        &&(ulDestAddr < (ulDestAddrB + (((VOS_UINT32)pstSmartPaCfg->uhwDataLenBytes)
                                - sizeof(VOS_UINT32)))))
    {
        pstSmartPaCfg->pshwDestVirtualAddr = pstPcmBuffTmp->ashwSmartPaBuffA;
    }
    else
    {
        pstSmartPaCfg->pshwDestVirtualAddr = pstPcmBuffTmp->ashwSmartPaBuffB;
    }

    ulAvailableVirtualAddr = (VOS_UINT32)pstSmartPaCfg->pshwDestVirtualAddr;

    return ulAvailableVirtualAddr;

}


VOS_VOID UCOM_PCM_SmartPaStop(VOS_VOID)
{
    /* ֹͣSmart PA��DMA */
    DRV_DMA_Stop(UCOM_PCM_DMAC_CHN_SPA);

    /* ֹͣSIO */
    DRV_SIO_SmartPa_Close();
}


VOS_UINT16 UCOM_PCM_GetSmartPaDmacChn(VOS_VOID)
{
    return UCOM_PCM_DMAC_CHN_SPA;
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

