

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "med_drv_dma.h"
#include "ucom_comm.h"



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/* DMAͨ���жϴ�����ָ������, ��DRV_DMA_RegisterIsr��ֵ */
DRV_DMA_CXISR_STRU g_astDrvDmaCxIntIsr[DRV_DMA_MAX_CHANNEL_NUM];

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_VOID DRV_DMA_Init( VOS_VOID )
{
    DRV_DMA_CXISR_STRU                 *pstDmaCxIsr = DRV_DMA_GetCxIsrPtr();

    /*��ʼ��g_astDrvDmaCxIntIsrΪȫ��*/
    UCOM_MemSet(pstDmaCxIsr, 0, sizeof(DRV_DMA_CXISR_STRU)*DRV_DMA_MAX_CHANNEL_NUM);

    /*����д0�����¼Ĵ���Ĭ������HiFi DMA�ж�*/
    UCOM_RegWr(DRV_DMA_INT_ERR1_MASK_HIFI, 0);
    UCOM_RegWr(DRV_DMA_INT_ERR2_MASK_HIFI, 0);
    UCOM_RegWr(DRV_DMA_INT_ERR3_MASK_HIFI, 0);
    UCOM_RegWr(DRV_DMA_INT_TC1_MASK_HIFI, 0);
    UCOM_RegWr(DRV_DMA_INT_TC2_MASK_HIFI, 0);

    /*ע��DMA ISR*/
    VOS_ConnectInterrupt(DRV_DMA_INT_NO_HIFI, DRV_DMA_Isr);

    /*ʹ��EMAC�ж�*/
    VOS_EnableInterrupt(DRV_DMA_INT_NO_HIFI);

}


VOS_UINT32 DRV_DMA_StartWithCfg(
                VOS_UINT16              uhwChannelNo,
                DRV_DMA_CXCFG_STRU     *pstCfg,
                DRV_DMA_INT_FUNC        pfIntHandleFunc,
                VOS_UINT32              uwPara)
{
    VOS_UINT32                          uwChannelMask   = (0x1L << uhwChannelNo);
    DRV_DMA_CXISR_STRU                 *pstCxIsr        = DRV_DMA_GetCxIsrPtr();

    /* �������Ƿ�Ƿ� */
    if (uhwChannelNo >= DRV_DMA_MAX_CHANNEL_NUM)
    {
        return VOS_ERR;
    }

    /*д0��DRV_DMA_CX_CONFIG(uhwChannelNo) bit0��ֹͨ��*/
    UCOM_RegBitWr(DRV_DMA_CX_CONFIG((VOS_UINT32)uhwChannelNo),
                  UCOM_BIT0,
                  UCOM_BIT0,
                  0);

    /*дͨ��X��ǰһά����ʣ���Byte��,[15,0]*/
    UCOM_RegBitWr(DRV_DMA_CX_CNT0((VOS_UINT32)uhwChannelNo),
                  UCOM_BIT0,
                  UCOM_BIT15,
                  pstCfg->uhwACount);

    /*дͨ��X��ǰ��ά����ʣ���Array����,[31,16]*/
    UCOM_RegBitWr(DRV_DMA_CX_CNT0((VOS_UINT32)uhwChannelNo),
                  UCOM_BIT16,
                  UCOM_BIT31,
                  pstCfg->uhwBCount);

    /*дͨ��X��ǰ��ά����ʣ���Frame��,[15,0]*/
    UCOM_RegBitWr(DRV_DMA_CX_CNT1((VOS_UINT32)uhwChannelNo),
                  UCOM_BIT0,
                  UCOM_BIT15,
                  pstCfg->uhwCCount);

    /*дͨ��X�Ķ�άԴ��ַƫ����[31,16]��Ŀ�ĵ�ַƫ����[15,0]*/
    UCOM_RegBitWr(DRV_DMA_CX_BINDX((VOS_UINT32)uhwChannelNo),
                  UCOM_BIT0,
                  UCOM_BIT15,
                  pstCfg->uhwDstBIndex);
    UCOM_RegBitWr(DRV_DMA_CX_BINDX((VOS_UINT32)uhwChannelNo),
                  UCOM_BIT16,
                  UCOM_BIT31,
                  pstCfg->uhwSrcBIndex);

    /*дͨ��X����άԴ��ַƫ����[31,16]��Ŀ�ĵ�ַƫ����[15,0]*/
    UCOM_RegBitWr(DRV_DMA_CX_CINDX((VOS_UINT32)uhwChannelNo),
                  UCOM_BIT0,
                  UCOM_BIT15,
                  pstCfg->uhwDstCIndex);
    UCOM_RegBitWr(DRV_DMA_CX_CINDX((VOS_UINT32)uhwChannelNo),
                  UCOM_BIT16,
                  UCOM_BIT31,
                  pstCfg->uhwSrcCIndex);

    /*дͨ��X��Դ��ַ[31,0]��Ŀ�ĵ�ַ[31,0]*/
    UCOM_RegWr(DRV_DMA_CX_DES_ADDR((VOS_UINT32)uhwChannelNo), pstCfg->uwDstAddr);
    UCOM_RegWr(DRV_DMA_CX_SRC_ADDR((VOS_UINT32)uhwChannelNo), pstCfg->uwSrcAddr);

    /*дͨ��X�������ַ����*/
    UCOM_RegWr(DRV_DMA_CX_LLI((VOS_UINT32)uhwChannelNo), pstCfg->uwLli);

    /*���ͨ��X�ĸ����ж�״̬*/
    UCOM_RegWr(DRV_DMA_INT_TC1_RAW, uwChannelMask);
    UCOM_RegWr(DRV_DMA_INT_TC2_RAW, uwChannelMask);
    UCOM_RegWr(DRV_DMA_INT_ERR1_RAW, uwChannelMask);
    UCOM_RegWr(DRV_DMA_INT_ERR2_RAW, uwChannelMask);
    UCOM_RegWr(DRV_DMA_INT_ERR3_RAW, uwChannelMask);

    /*���ص������ǿ�,�򱣴��ֵ*/
    if (VOS_NULL != pfIntHandleFunc)
    {
        pstCxIsr[uhwChannelNo].pfFunc  = pfIntHandleFunc;
        pstCxIsr[uhwChannelNo].uwPara  = uwPara;

        /*����д(uhwChannelNo��ӦbitΪ1)�����¼Ĵ�����HiFi��ӦDMAͨ���ж�����*/
        UCOM_RegBitWr(DRV_DMA_INT_ERR1_MASK_HIFI, uhwChannelNo, uhwChannelNo, 1);
        UCOM_RegBitWr(DRV_DMA_INT_ERR2_MASK_HIFI, uhwChannelNo, uhwChannelNo, 1);
        UCOM_RegBitWr(DRV_DMA_INT_ERR3_MASK_HIFI, uhwChannelNo, uhwChannelNo, 1);
        UCOM_RegBitWr(DRV_DMA_INT_TC1_MASK_HIFI, uhwChannelNo, uhwChannelNo, 1);
        UCOM_RegBitWr(DRV_DMA_INT_TC2_MASK_HIFI, uhwChannelNo, uhwChannelNo, 1);

    }

    /*дͨ��X������*/
    UCOM_RegWr(DRV_DMA_CX_CONFIG((VOS_UINT32)uhwChannelNo), pstCfg->uwConfig);
    return VOS_OK;

}
VOS_UINT32 DRV_DMA_MemCpy(
                VOS_UINT16              uhwChannelNo,
                VOS_UCHAR              *pucDes,
                VOS_UCHAR              *pucSrc,
                VOS_UINT32              uwLen,
                DRV_DMA_INT_FUNC        pfIntHandleFunc,
                VOS_UINT32              uwPara)
{
    VOS_UINT32                          uwRet;
    DRV_DMA_CXCFG_STRU                  stCfg;

    /*��ʼ�����ýṹ��*/
    UCOM_MemSet(&stCfg, 0, sizeof(DRV_DMA_CXCFG_STRU));

    /*��һά����������ͨ�����ӷ�ʽ���ò���*/
    stCfg.uhwACount     = (VOS_UINT16)uwLen;
    stCfg.uwDstAddr     = (VOS_UINT32)pucDes;
    stCfg.uwSrcAddr     = (VOS_UINT32)pucSrc;
    stCfg.uwConfig      = DRV_DMA_MEM_MEM_CFG;     /*��������(M2M/��ַ����)*/

    /*�����ڲ��ӿڿ�ʼ����*/
    uwRet   = DRV_DMA_StartWithCfg(uhwChannelNo, &stCfg, pfIntHandleFunc, uwPara);

    return uwRet;
}


VOS_VOID DRV_DMA_Stop(VOS_UINT16 uhwChannelNo)
{
    DRV_DMA_CXISR_STRU                 *pstCxIsr        = DRV_DMA_GetCxIsrPtr();

    /* д0��DRV_DMA_CX_CONFIG(uhwChannelNo) bit0 ֹͣ��ӦDMAͨ�� */
    UCOM_RegBitWr(DRV_DMA_CX_CONFIG((VOS_UINT32)uhwChannelNo), UCOM_BIT0, UCOM_BIT0, 0);

    /*����д(uhwChannelNo��ӦbitΪ0)�����¼Ĵ�������HiFi��ӦDMAͨ���ж�*/
    UCOM_RegBitWr(DRV_DMA_INT_ERR1_MASK_HIFI, uhwChannelNo, uhwChannelNo, 0);
    UCOM_RegBitWr(DRV_DMA_INT_ERR2_MASK_HIFI, uhwChannelNo, uhwChannelNo, 0);
    UCOM_RegBitWr(DRV_DMA_INT_ERR3_MASK_HIFI, uhwChannelNo, uhwChannelNo, 0);
    UCOM_RegBitWr(DRV_DMA_INT_TC1_MASK_HIFI, uhwChannelNo, uhwChannelNo, 0);
    UCOM_RegBitWr(DRV_DMA_INT_TC2_MASK_HIFI, uhwChannelNo, uhwChannelNo, 0);

    /*���ȫ�ֱ����ж�Ӧͨ����������*/
    UCOM_MemSet(&pstCxIsr[uhwChannelNo],0,sizeof(DRV_DMA_CXISR_STRU));
}
VOS_VOID DRV_DMA_Isr(VOS_VOID)
{
    VOS_UINT32                          i;
    VOS_UINT32                          uwErr1;
    VOS_UINT32                          uwErr2;
    VOS_UINT32                          uwErr3;
    VOS_UINT32                          uwTc1;
    VOS_UINT32                          uwTc2;
    VOS_UINT32                          uwIntMask;
    VOS_UINT32                          uwIntState;
    DRV_DMA_INT_TYPE_ENUM_UINT16        enIntType;
    DRV_DMA_CXISR_STRU                 *pstDmaCxIsr = DRV_DMA_GetCxIsrPtr();

    /*��ȡHIFI CPU��DMAͨ���ж�״̬*/
    uwIntState  = UCOM_RegRd(DRV_DMA_INT_STAT_HIFI);

    /*��ͨ��״̬ȫ0��ʾȫͨ�����жϻ��ж��Ѵ���,ֱ���˳�*/
    if (0 == uwIntState)
    {
        return;
    }

    /*��ȡ���жϼĴ���,��ѯ���ж�״̬,����Tc1/Tc2/Err1/Err2/Err3*/
    uwErr1      = UCOM_RegRd(DRV_DMA_INT_ERR1_HIFI);
    uwErr2      = UCOM_RegRd(DRV_DMA_INT_ERR2_HIFI);
    uwErr3      = UCOM_RegRd(DRV_DMA_INT_ERR3_HIFI);
    uwTc1       = UCOM_RegRd(DRV_DMA_INT_TC1_HIFI);
    uwTc2       = UCOM_RegRd(DRV_DMA_INT_TC2_HIFI);

    /*дuwIntState���ε����Ĵ�������ж�,������β�ѯ����ͨ���ж�*/
    UCOM_RegWr(DRV_DMA_INT_TC1_RAW, uwIntState);
    UCOM_RegWr(DRV_DMA_INT_TC2_RAW, uwIntState);
    UCOM_RegWr(DRV_DMA_INT_ERR1_RAW, uwIntState);
    UCOM_RegWr(DRV_DMA_INT_ERR2_RAW, uwIntState);
    UCOM_RegWr(DRV_DMA_INT_ERR3_RAW, uwIntState);

    /*�������ø�ͨ��ע��Ļص�������*/
    for ( i = 0; i < DRV_DMA_MAX_CHANNEL_NUM; i++)
    {
        uwIntMask = 0x1L << i;

        /*����ͨ�����жϲ���(��Ӧ����Ϊ1)*/
        if (uwIntState & uwIntMask)
        {
            if (VOS_NULL != pstDmaCxIsr[i].pfFunc)
            {
                /*��ERR1�ж�״̬��Ӧ����Ϊ1*/
                if (uwErr1 & uwIntMask)
                {
                    enIntType = DRV_DMA_INT_TYPE_ERR1;
                }
                /*��ERR2�ж�״̬��Ӧ����Ϊ1*/
                else if (uwErr2 & uwIntMask)
                {
                    enIntType = DRV_DMA_INT_TYPE_ERR2;
                }
                /*��ERR3�ж�״̬��Ӧ����Ϊ1*/
                else if (uwErr3 & uwIntMask)
                {
                    enIntType = DRV_DMA_INT_TYPE_ERR3;
                }
                /*��TC1�ж�״̬��Ӧ����Ϊ1*/
                else if (uwTc1 & uwIntMask)
                {
                    enIntType = DRV_DMA_INT_TYPE_TC1;
                }
                /*��TC2�ж�״̬��Ӧ����Ϊ1*/
                else if (uwTc2 & uwIntMask)
                {
                    enIntType = DRV_DMA_INT_TYPE_TC2;
                }
                /*δ֪�ж�*/
                else
                {
                    enIntType = DRV_DMA_INT_TYPE_BUTT;
                }

                /*����ע����жϴ�����*/
                pstDmaCxIsr[i].pfFunc(enIntType, pstDmaCxIsr[i].uwPara);
            }
        }
    }
}
VOS_UINT32 DRV_DMA_GetVoicePcmSioMemCfg(VOS_VOID)
{
    return DRV_DMA_VOICE_PCM_SIO_MEM_CFG;
}


VOS_UINT32 DRV_DMA_GetVoicePcmMemSioCfg(VOS_VOID)
{
    return DRV_DMA_VOICE_PCM_MEM_SIO_CFG;
}


VOS_UINT32 DRV_DMA_GetVoiceI2sSioMemCfg(VOS_VOID)
{
    return DRV_DMA_VOICE_I2S_SIO_MEM_CFG;
}


VOS_UINT32 DRV_DMA_GetVoiceI2sMemSioCfg(VOS_VOID)
{
    return DRV_DMA_VOICE_I2S_MEM_SIO_CFG;
}


VOS_UINT32 DRV_DMA_GetAudioSioMemCfg(VOS_VOID)
{
    return DRV_DMA_AUDIO_SIO_MEM_CFG;
}


VOS_UINT32 DRV_DMA_GetAudioMemSioCfg(VOS_VOID)
{
    return DRV_DMA_AUDIO_MEM_SIO_CFG;
}


VOS_UINT32 DRV_DMA_GetCxSrcAddr(VOS_UINT32 uwChn)
{
    return DRV_DMA_CX_SRC_ADDR(uwChn);
}


VOS_UINT32 DRV_DMA_GetCxDesAddr(VOS_UINT32 uwChn)
{
    return DRV_DMA_CX_DES_ADDR(uwChn);
}


VOS_UINT16 DRV_DMA_GetSioTxChn(VOS_VOID)
{
    return DRV_DMA_SIO_TX_CHN;
}


VOS_UINT16 DRV_DMA_GetSioRxChn(VOS_VOID)
{
    return DRV_DMA_SIO_RX_CHN;
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

