

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "med_drv_sio.h"
#include "soc_sctrl_interface.h"
#include "ucom_comm.h"
#include "ucom_stub.h"
#include "med_drv_interface.h"
#ifdef _lint
#include "MemoryMap.h"
#endif


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/* V7R2ֻ��һ��SIO����VOICEʹ�� */
UCOM_SEC_TCMBSS
DRV_SIO_CFG_STRU    g_stDrvSioVoiceCfg;                        /* SIO voiceģ�����ȫ�ֱ��� */

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_VOID DRV_SIO_Init(VOS_VOID)
{
    DRV_SIO_CFG_STRU   *pstVoiceCfg;
    VOS_UINT32          uwVoiceChanNum = DRV_SIO_VOICE_MIC_CHANNEL_NUM;

    /* ��ȡSIO���ƽṹ�� */
    pstVoiceCfg       = DRV_SIO_GetVoiceCfgPtr();

    /* ��ʼ�����ƽṹ��Ϊȫ�� */
    UCOM_MemSet(pstVoiceCfg, 0, sizeof(DRV_SIO_CFG_STRU));

    /* ��ַ��ʼ�� */
    pstVoiceCfg->uwSioBaseAddr  = DRV_SIO_BASE_ADDR_VOICE;

    /************************** ����AUDIO SIO *****************************/
    /* �����SIO���ݲ�ʵ�� */

    /************************** ����VOICE SIO *****************************/
    /* ��SIOʱ�� */
    DRV_SIO_Voice_OpenClk();

    /* ��λSIO */
    DRV_SIO_Reset(pstVoiceCfg);

    /* �ر�SIOʱ�� */
    DRV_SIO_Voice_CloseClk();

    pstVoiceCfg->enWorkMode = DRV_SIO_Voice_GetWorkMode(uwVoiceChanNum);

    /************************** ����SMART PA SIO *****************************/
    /* �����SIO���ݲ�ʵ�� */

    /* ��Ϊ��DMA�жϣ����Բ���SIO�жϣ����´��벻�� */
    #ifdef _MED_ENABLE_SIO_INT_
    /* ע��SIO ISR */
    BSP_INT_Connect((VOIDFUNCPTR *)(DRV_SIO_IRQ), (VOIDFUNCPTR)DRV_SIO_Isr, 0);

    /* ʹ���ж� */
    BSP_INT_Enable(DRV_SIO_IRQ);
    #endif

}


VOS_UINT32 DRV_SIO_Open(
                DRV_SIO_CFG_STRU               *pstCfg,
                DRV_SIO_INT_MASK_ENUM_UINT32    enIntMask,
                DRV_SIO_INT_FUNC                pfIntHandleFunc,
                VOS_UINT32                      uwPara)
{
    SOC_SIO_CT_SET_UNION                *pstCtSet;
    SOC_SIO_CT_CLR_UNION                *pstCtClr;
    SOC_SIO_MODE_UNION                  *pstMode;
    SOC_SIO_I2S_POS_MERGE_EN_UNION      *pstMerge;
    SOC_SIO_I2S_START_POS_UNION         *pstStartPos;
    SOC_SIO_DATA_WIDTH_SET_UNION        *pstDataWidth;
    SOC_SIO_SIGNED_EXT_UNION            *pstSignedExt;
    SOC_SIO_INTMASK_UNION               *pstIntMask;
    SOC_SIO_INTCLR_UNION                *pstIntclr;

    VOS_UINT32                           uwSioBaseAddr;

    uwSioBaseAddr = pstCfg->uwSioBaseAddr;

    pstIntclr   = (SOC_SIO_INTCLR_UNION*)SOC_SIO_INTCLR_ADDR(pstCfg->uwSioBaseAddr);
    pstCtSet    = (SOC_SIO_CT_SET_UNION*)SOC_SIO_CT_SET_ADDR(uwSioBaseAddr);
    pstCtClr    = (SOC_SIO_CT_CLR_UNION*)SOC_SIO_CT_CLR_ADDR(uwSioBaseAddr);
    pstMode     = (SOC_SIO_MODE_UNION*)SOC_SIO_MODE_ADDR(uwSioBaseAddr);
    pstMerge    = (SOC_SIO_I2S_POS_MERGE_EN_UNION*)SOC_SIO_I2S_POS_MERGE_EN_ADDR(uwSioBaseAddr);
    pstStartPos = (SOC_SIO_I2S_START_POS_UNION*)SOC_SIO_I2S_START_POS_ADDR(uwSioBaseAddr);
    pstDataWidth= (SOC_SIO_DATA_WIDTH_SET_UNION*)SOC_SIO_DATA_WIDTH_SET_ADDR(uwSioBaseAddr);
    pstSignedExt= (SOC_SIO_SIGNED_EXT_UNION*)SOC_SIO_SIGNED_EXT_ADDR(uwSioBaseAddr);
    pstIntMask  = (SOC_SIO_INTMASK_UNION*)SOC_SIO_INTMASK_ADDR(uwSioBaseAddr);

    /************************** ��ʼ�� *****************************/
    /* ����SIO_CT_SET/SIO_CT_CLR[rx_enable]��SIO_CT_SET/SIO_CT_CLR[tx_enable]Ϊ0 ʹSIO���ڽ�ֹ״̬ */
    pstCtClr->reg.rx_enable = 1; /* SIO_CT_SET[rx_enable]�������0 */
    pstCtClr->reg.tx_enable = 1; /* SIO_CT_SET[tx_enable]�������0 */

    /* ��ֹ�����Լ�����FIFO, ���FIFO�е����� */
    pstCtSet->reg.tx_fifo_disable   = 1;
    pstCtSet->reg.rx_fifo_disable   = 1;

    /* ����SIO_DATA_WIDTH_SET�Ĵ���������SIO_SIGNED_EXT�Ĵ��������趨��ȷ��λ�� */
    pstDataWidth->reg.tx_mode = 1;          /* 16bit */
    pstDataWidth->reg.rx_mode = 1;          /* 16bit */
    pstSignedExt->reg.signed_ext_en = 0;    /* ��ֹ������չ */

    /* ����SIO_MODE[sio_mode],ѡ��I2S��PCMģʽ;*/
    pstMode->reg.sio_mode   = pstCfg->enWorkMode;

    pstMode->reg.ext_rec_en = 0; /* ��׼�Ľ���ģʽ */

    if(DRV_SIO_MODE_PCM == pstCfg->enWorkMode)
    {
        /* �����PCMģʽ, ����SIO_MODE[pcm_mode], ѡ��ʱ������.
           ��·¼����Ҫ����SIO_MODE[ext_rec_en] SIO_MODE[chn_num]��SIO_MODE[clk_edge]*/
        pstMode->reg.pcm_mode = 0;
    }
    else
    {
        /* �����I2Sģʽ, ����ʵ�ʶ�дFIFO���ݵķ�ʽ, ����SIO_I2S_POS_MERGE_EN��SIO_I2S_START_POS*/
        pstMerge->reg.merge_en = 1;             /* ���������ϲ�ʹ�� */
        pstStartPos->reg.start_pos_read  = 0;   /* ��������ʼ���� */
        pstStartPos->reg.start_pos_write = 0;   /* ��������ʼ���� */

        /* ͬʱ, ����SIO_CT_SET[tx_data_merge_en]��SIO_CT_SET[rx_data_merge_en]. ��·¼�����ý��иò�������*/
        pstCtSet->reg.tx_data_merge_en = 1;
        pstCtSet->reg.rx_data_merge_en = 1;

    }

    /* ����SIO_CT_SET[rx_fifo_threshold]��SIO_CT_SET[tx_fifo_threshold], ��:���ý���FIFO�ͷ���FIFO��ˮ�� */
    pstCtSet->reg.tx_fifo_threshold = 8;
    pstCtSet->reg.rx_fifo_threshold = 8;
    pstCtSet->reg.rst_n = 1;

    /* ����SIO��DMAͨ������ΪEDMA_CH16 */
    UCOM_RegBitWr(DRV_SC_CTRL54_ADDR, DRV_SC_SIO_DMA_SEL_BIT, DRV_SC_SIO_DMA_SEL_BIT, 0x0);

    /* �����ж����ε���Ҫ,����SIO���ж����μĴ���SIO_INTMASK,����SIO_CT_SET[intr_en] */
    pstCtClr->reg.intr_en    = 1;               /* ȫ�ֹ��ж� */
    pstIntclr->value         = 0x3f;            /* �������SIO�ж� */
    pstIntMask->value        = 0x3f;            /* ����ȫ���ж� */

    /* ��Ϊ��DMA�жϣ����Բ���SIO�жϣ����´��벻�� */
    #ifdef _MED_ENABLE_SIO_INT_
    /* ���жϻص������ǿգ�����ж�����,����ص�������Ϣ */
    if (VOS_NULL != pfIntHandleFunc)
    {
        /* ��enIntMaskȡ�������ж����� */
        pstIntMask->value        = ~enIntMask;

        /* �����ж�ʹ�� */
        pstCtSet->reg.intr_en    = 1;

        pstCfg->stIsr.pfFunc    = pfIntHandleFunc;
        pstCfg->stIsr.uwPara    = uwPara;
    }
    #endif


    /************************** �������� ****************************/

    /* �����ж����μĴ���SIO_INTMASK[tx_intr]Ϊ1, ���η����ж� */
    pstIntMask->reg.tx_intr  = 1;

    /* ����SIO_CT_SET[tx_enable]Ϊ1, �������� */
    pstCtSet->reg.tx_enable = 1;

    /* ���FIFO��ֹ��־, ʹ�ܷ���FIFO */
    pstCtClr->reg.tx_fifo_disable   = 1;

    /************************** ����¼�� ****************************/

    /* ����SIO_CT_SET[rx_enable]Ϊ1, �������ݽ��� */
    pstCtSet->reg.rx_enable = 1;

    /* ���FIFO��ֹ��־, ʹ�ܽ���FIFO */
    pstCtClr->reg.rx_fifo_disable   = 1;

    return VOS_OK;

}


VOS_VOID DRV_SIO_Close(DRV_SIO_CFG_STRU *pstCfg)
{
    VOS_UINT32            uwSioBaseAddr = pstCfg->uwSioBaseAddr;
    SOC_SIO_CT_CLR_UNION *pstCtClr      = (SOC_SIO_CT_CLR_UNION*)SOC_SIO_CT_CLR_ADDR(uwSioBaseAddr);

    if(DRV_SIO_CLK_STATUS_CLOSE == pstCfg->enClkStatus)
    {
        return;
    }

    /* ����SIO_CT_SET/SIO_CT_CLR[rx_enable]��SIO_CT_SET/SIO_CT_CLR[tx_enable]Ϊ0 ʹSIO���ڽ�ֹ״̬ */
    pstCtClr->reg.rx_enable = 1; /* SIO_CT_SET[rx_enable]�������0 */
    pstCtClr->reg.tx_enable = 1; /* SIO_CT_SET[tx_enable]�������0 */

    /* ��ֹ�ж�  */
    pstCtClr->reg.intr_en    = 1;

    /* ��λSIO */
    DRV_SIO_Reset(pstCfg);

    /* ����жϻص����������Ϣ */
    UCOM_MemSet(&pstCfg->stIsr, 0, sizeof(DRV_SIO_INT_ISR_STRU));
}


VOS_VOID DRV_SIO_SetMaster(
                DRV_SIO_CFG_STRU             *pstCfg,
                DRV_SIO_SAMPLING_ENUM_UINT32  enSamplingRate)
{
    /* ��¼ͬ��ģʽ */
    pstCfg->enSyncMode                  = DRV_SIO_SYNC_MODE_MASTER;

    /* ѡ��Ϊmasterģʽ */
    UCOM_RegBitWr(DRV_SC_CTRL3_ADDR, DRV_SC_SIO_MASTER_MODE_BIT, DRV_SC_SIO_MASTER_MODE_BIT, 0x1);

    /* ���÷�Ƶϵ�� */
    switch(enSamplingRate)
    {
        case DRV_SIO_SAMPLING_8K:
        {
            /* ����SIO masterʱʱ�ӷ�Ƶϵ������խ��ʹ�ã�120000k��60��250= 8k */
            UCOM_RegBitWr(DRV_CRG_CLKDIV1_ADDR,
                          DRV_CRG_PCM_SYNC_DIV_BIT_LO,
                          DRV_CRG_PCM_SYNC_DIV_BIT_HI,
                          0x3c);

            UCOM_RegBitWr(DRV_CRG_CLKDIV1_ADDR,
                          DRV_CRG_PCM_BCLK_DIV_BIT_LO,
                          DRV_CRG_PCM_BCLK_DIV_BIT_HI,
                          0xfa);
        }
        break;

        case DRV_SIO_SAMPLING_16K:
        {
            /* ����SIO masterʱʱ�ӷ�Ƶϵ������խ��ʹ�ã�120000k��60��125= 16k */
            UCOM_RegBitWr(DRV_CRG_CLKDIV1_ADDR,
                          DRV_CRG_PCM_SYNC_DIV_BIT_LO,
                          DRV_CRG_PCM_SYNC_DIV_BIT_HI,
                          0x3c);

            UCOM_RegBitWr(DRV_CRG_CLKDIV1_ADDR,
                          DRV_CRG_PCM_BCLK_DIV_BIT_LO,
                          DRV_CRG_PCM_BCLK_DIV_BIT_HI,
                          0x7d);
        }
        break;

        case DRV_SIO_SAMPLING_48K:
        {
            /* ������Ϊ48kʱ�Ĵ�������SIO masterʱʱ�ӷ�Ƶϵ����24576k��16��32=48k */
            /* ����SIO masterʱʱ�ӷ�Ƶϵ������խ��ʹ�ã�120000k��10��250= 48k */
            UCOM_RegBitWr(DRV_CRG_CLKDIV1_ADDR,
                          DRV_CRG_PCM_SYNC_DIV_BIT_LO,
                          DRV_CRG_PCM_SYNC_DIV_BIT_HI,
                          0x0a);

            UCOM_RegBitWr(DRV_CRG_CLKDIV1_ADDR,
                          DRV_CRG_PCM_BCLK_DIV_BIT_LO,
                          DRV_CRG_PCM_BCLK_DIV_BIT_HI,
                          0xfa);
        }
        break;

        default:
        {
            /* �����߱�֤���ߵ��˷�֧ */
            /* Ĭ�ϲ��ò�����Ϊ8kʱ�Ĵ��� */
            UCOM_RegBitWr(DRV_CRG_CLKDIV1_ADDR,
                          DRV_CRG_PCM_SYNC_DIV_BIT_LO,
                          DRV_CRG_PCM_SYNC_DIV_BIT_HI,
                          0x3c);

            UCOM_RegBitWr(DRV_CRG_CLKDIV1_ADDR,
                          DRV_CRG_PCM_BCLK_DIV_BIT_LO,
                          DRV_CRG_PCM_BCLK_DIV_BIT_HI,
                          0xfa);
        }
        break;
    }

}
VOS_VOID DRV_SIO_SetSlave(DRV_SIO_CFG_STRU   *pstCfg)
{
    /* ��¼ͬ��ģʽ */
    pstCfg->enSyncMode                   = DRV_SIO_SYNC_MODE_SLAVE;

    /* ����SIOʱ����PADʱ��ͬ�� */
    UCOM_RegBitWr(DRV_SC_SIO_CLK_POSITIVE, DRV_SC_SIO_CLK_POST_SEL_BIT, DRV_SC_SIO_CLK_POST_SEL_BIT, 0x0);

    /* ����ΪSLAVEģʽ */
    UCOM_RegBitWr(DRV_SC_CTRL3_ADDR, DRV_SC_SIO_MASTER_MODE_BIT, DRV_SC_SIO_MASTER_MODE_BIT, 0x0);
}
VOS_VOID DRV_SIO_Reset(DRV_SIO_CFG_STRU   *pstCfg)
{
    SOC_SIO_CT_CLR_UNION     *pstCtClr;
    SOC_SIO_INTCLR_UNION     *pstIntclr;
    SOC_SIO_CT_SET_UNION     *pstCtSet;

    pstCtClr  = (SOC_SIO_CT_CLR_UNION*)SOC_SIO_CT_CLR_ADDR(pstCfg->uwSioBaseAddr);
    pstIntclr = (SOC_SIO_INTCLR_UNION*)SOC_SIO_INTCLR_ADDR(pstCfg->uwSioBaseAddr);
    pstCtSet  = (SOC_SIO_CT_SET_UNION*)SOC_SIO_CT_SET_ADDR(pstCfg->uwSioBaseAddr);

    /* дDRV_SIO_CTRL_CLR 0xffff��λSIO����ֹ���䡢��ֹ�ж� */
    pstCtClr->value  = 0xffff;

    /* дDRV_SIO_INTR_CLR 0x3f �������SIO�ж� */
    pstIntclr->value = 0x3f;

    /* ͨ���⸴λ */
    pstCtSet->value  = 0xc088;
}
VOS_UINT32 DRV_SIO_GetTxRegAddr(DRV_SIO_CFG_STRU   *pstCfg)
{
    if(DRV_SIO_MODE_PCM == pstCfg->enWorkMode)
    {
        return DRV_SIO_PCM_XD_ADDR(pstCfg->uwSioBaseAddr);
    }
    else
    {
        return DRV_SIO_I2S_DUAL_TX_CHN_ADDR(pstCfg->uwSioBaseAddr);
    }
}


VOS_UINT32 DRV_SIO_GetRxRegAddr( DRV_SIO_CFG_STRU   *pstCfg )
{
    if(DRV_SIO_MODE_PCM == pstCfg->enWorkMode)
    {
        return DRV_SIO_PCM_RD_ADDR(pstCfg->uwSioBaseAddr);
    }
    else
    {
        return DRV_SIO_I2S_DUAL_RX_CHN_ADDR(pstCfg->uwSioBaseAddr);
    }
}


VOS_UINT32 DRV_SIO_GetSioChannelNum( DRV_SIO_CFG_STRU   *pstCfg )
{
    if(DRV_SIO_MODE_PCM == pstCfg->enWorkMode)
    {
        return DRV_SIO_CHANNEL_NUM_PCM;
    }
    else
    {
        return DRV_SIO_CHANNEL_NUM_I2S;
    }
}


VOS_UINT32 DRV_SIO_Audio_Open(
                DRV_SIO_INT_MASK_ENUM_UINT32    enIntMask,
                DRV_SIO_SAMPLING_ENUM_UINT32    enSamplingType,
                VOS_UINT16                      uhwIsModeMaster,
                DRV_SIO_INT_FUNC                pfIntHandleFunc,
                VOS_UINT32                      uwPara)
{
    return 0;
}


VOS_UINT32 DRV_SIO_Voice_Open(
                DRV_SIO_INT_MASK_ENUM_UINT32    enIntMask,
                DRV_SIO_SAMPLING_ENUM_UINT32    enSamplingType,
                VOS_UINT16                      uhwIsModeMaster,
                DRV_SIO_INT_FUNC                pfIntHandleFunc,
                VOS_UINT32                      uwPara)
{
    VOS_UINT32                             uwRet;
    DRV_SIO_CFG_STRU                      *pstCfg = DRV_SIO_GetVoiceCfgPtr();
    if(CODEC_SWITCH_ON == uhwIsModeMaster)
    {
        DRV_SIO_SetMaster(pstCfg, enSamplingType);
    }
    else
    {
        DRV_SIO_SetSlave(pstCfg);
    }

    DRV_SIO_Voice_OpenClk();

    DRV_SIO_Reset(pstCfg);
    uwRet = DRV_SIO_Open(pstCfg, enIntMask, pfIntHandleFunc, uwPara);

    return uwRet;
}


VOS_UINT32 DRV_SIO_SmartPa_Open(
                DRV_SIO_INT_MASK_ENUM_UINT32    enIntMask,
                DRV_SIO_SAMPLING_ENUM_UINT32    enSamplingType,
                VOS_UINT16                      uhwIsModeMaster,
                DRV_SIO_INT_FUNC                pfIntHandleFunc,
                VOS_UINT32                      uwPara)
{
    return 0;
}


VOS_VOID DRV_SIO_Audio_Close(VOS_VOID)
{
    return;
}


VOS_VOID DRV_SIO_Voice_Close(VOS_VOID)
{
    DRV_SIO_CFG_STRU  *pstCfg = DRV_SIO_GetVoiceCfgPtr();

    if(DRV_SIO_CLK_STATUS_OPEN == pstCfg->enClkStatus)
    {
        DRV_SIO_Close(pstCfg);

        /* �ر�SIOʱ��  */
        DRV_SIO_Voice_CloseClk();
    }
}
VOS_VOID DRV_SIO_SmartPa_Close(VOS_VOID)
{
    return;
}


VOS_VOID DRV_SIO_Audio_OpenClk(VOS_VOID)
{
    return;
}


VOS_VOID DRV_SIO_Voice_OpenClk(VOS_VOID)
{
    DRV_SIO_CFG_STRU                 *pstVoiceCfg = DRV_SIO_GetVoiceCfgPtr();

    /* 1. ��������ʱ�ӿ�ʼʹ�� */
    UCOM_RegBitWr(DRV_CRG_CLKEN3_ADDR,
                  DRV_SC_LTE_SIO_CLK_EN_BIT,
                  DRV_SC_LTE_SIO_CLK_EN_BIT,
                  0x1);

    /* 2. V7R2�������ù���ʱ�� */

    /* 3. �޸�VOICE CFG��־λ�����ʱ��״̬ */
    pstVoiceCfg->enClkStatus = DRV_SIO_CLK_STATUS_OPEN;
}
VOS_VOID DRV_SIO_SmartPa_OpenClk(VOS_VOID)
{
    return;
}


VOS_VOID DRV_SIO_Audio_CloseClk(VOS_VOID)
{
    return;
}


VOS_VOID DRV_SIO_Voice_CloseClk(VOS_VOID)
{
    DRV_SIO_CFG_STRU                  *pstVoiceCfg = DRV_SIO_GetVoiceCfgPtr();

    /* 1. ��������ʱ�ӹر�ʹ�� */
    UCOM_RegBitWr(DRV_CRG_CLKDIS3_ADDR,
                  DRV_SC_LTE_SIO_CLK_DIS_BIT,
                  DRV_SC_LTE_SIO_CLK_DIS_BIT,
                  0x1);

    /* 2. V7R2�������ù���ʱ�� */

    /* 3. �޸�VOICE CFG��־λ�����ʱ��״̬ */
    pstVoiceCfg->enClkStatus = DRV_SIO_CLK_STATUS_CLOSE;
}
VOS_VOID DRV_SIO_SmartPa_CloseClk(VOS_VOID)
{
    return;
}


VOS_UINT32 DRV_SIO_Audio_Loop(DRV_SIO_SAMPLING_ENUM_UINT32  enSamplingType)
{

    /* �ú���û��ʵ�� */

    return UCOM_RET_SUCC;
}
VOS_UINT32 DRV_SIO_Voice_GetWorkMode(VOS_UINT32 uwVoiceChanNum)
{

    if (DRV_SIO_CHANNEL_NUM_PCM == uwVoiceChanNum)
    {
        /* ʹ��PCM������ʽ */
        return DRV_SIO_MODE_PCM;
    }
    else
    {
        /* ʹ��I2S������ʽ */
        return DRV_SIO_MODE_I2S;
    }
}


VOS_UINT32 DRV_SIO_GetVoiceMicChn(VOS_VOID)
{
    return DRV_SIO_VOICE_MIC_CHANNEL_NUM;
}


VOS_UINT32 DRV_SIO_GetVoiceSpkChn(VOS_VOID)
{
    return DRV_SIO_VOICE_SPK_CHANNEL_NUM;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


