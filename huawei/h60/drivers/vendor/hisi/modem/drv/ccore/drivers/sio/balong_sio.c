/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : balong_sio.c
  �� �� ��   :
  ��    ��   :
  ��������   :
  ����޸�   :
  ��������   : SIO�������
  �����б�   :
              bsp_sio_close
              bsp_sio_init
              bsp_sio_isr
              bsp_sio_open

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include <vxWorks.h>
#include <intLib.h>
#include <logLib.h>
#include <stdio.h>
#include <string.h>
#include <memLib.h>
#include "hi_base.h"
#include "hi_sio.h"
#include "bsp_sio.h"
#include "soc_interrupts.h"
#include "hi_syssc_interface.h"
#include "hi_syscrg_interface.h"
#include "bsp_edma.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
drv_sio_ctrl_stru   g_stdrvsioctrl;    /* SIOģ�����ȫ�ֱ��� */

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

void dma_spk_out_isr(unsigned int uwPara);
void dma_mic_in_isr(unsigned int uwPara);

/*****************************************************************************
 �� �� ��  : bsp_sio_dma_init
 ��������  : SIO���������ʼ��, Ӧ���������ǰ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
int bsp_sio_dma_init()
{
    drv_sio_ctrl_stru  *pstCfg = DRV_SIO_GET_CFG_PTR();
    int rx_channel_id = 0;
    int tx_channel_id = 0;

    memset((void*)&pstCfg->dma_cfg, 0 ,sizeof(dma_sio_cfg_stru));

    rx_channel_id = bsp_edma_channel_init((enum edma_req_id)EDMA_LTESIO_RX, (channel_isr)dma_mic_in_isr, (u32)0 , (u32)EDMA_INT_DONE | EDMA_INT_LLT_DONE);
    if(rx_channel_id < 0){
        logMsg("balong_dma_channel_init: req EDMA_LTESIO_RX failed\n",1,2,3,4,5,6);
        return ERROR;
    }

    pstCfg->dma_cfg.rx_channel_id = rx_channel_id;

    tx_channel_id = bsp_edma_channel_init((enum edma_req_id)EDMA_LTESIO_TX, (channel_isr)dma_spk_out_isr, (u32)0 , (u32)EDMA_INT_DONE | EDMA_INT_LLT_DONE);
    if(rx_channel_id < 0){
        logMsg("balong_dma_channel_init: req EDMA_LTESIO_TX failed\n",1,2,3,4,5,6);
        return ERROR;
    }

    pstCfg->dma_cfg.rx_channel_id = tx_channel_id;

    return OK;
}

/*****************************************************************************
 �� �� ��  : bsp_sio_dma_cfg
 ��������  : SIO���������ʼ��, Ӧ���������ǰ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
int bsp_sio_record_dma_cfg(unsigned int dst, unsigned int dst1)
{
    drv_sio_ctrl_stru  *pstCfg = DRV_SIO_GET_CFG_PTR();
    unsigned int src = 0;

    src = (DRV_SIO_MODE_I2S == pstCfg->enMode)? HI_SIO_REG_ADDR(HI_SIO_I2S_LEFT_RD_OFFSET):HI_SIO_REG_ADDR(HI_SIO_PCM_RD_OFFSET);

    pstCfg->dma_cfg.record_cfg[0].uwLli = DRV_SIO_DMA_LLI_LINK((unsigned int)&pstCfg->dma_cfg.record_cfg[1]);
    pstCfg->dma_cfg.record_cfg[0].uhwACount = 320;  /* ��AMRÿ10ms����80���� */
    pstCfg->dma_cfg.record_cfg[0].uwSrcAddr = src;
    pstCfg->dma_cfg.record_cfg[0].uwDstAddr = dst;
    pstCfg->dma_cfg.record_cfg[0].uwConfig  = DRV_SIO_DMA_SIO_MEM_CFG;

    pstCfg->dma_cfg.record_cfg[1].uwLli = DRV_SIO_DMA_LLI_LINK((unsigned int)&pstCfg->dma_cfg.record_cfg[0]);
    pstCfg->dma_cfg.record_cfg[1].uhwACount = 320;  /* խ��AMRÿ10ms����80���� */
    pstCfg->dma_cfg.record_cfg[1].uwSrcAddr = HI_SIO_REG_ADDR(HI_SIO_I2S_RIGHT_RD_OFFSET);
    pstCfg->dma_cfg.record_cfg[1].uwDstAddr = dst1;
    pstCfg->dma_cfg.record_cfg[1].uwConfig  = DRV_SIO_DMA_SIO_MEM_CFG;

    return OK;
}

/*****************************************************************************
 �� �� ��  : bsp_sio_dma_cfg
 ��������  : SIO���������ʼ��, Ӧ���������ǰ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
int bsp_sio_play_dma_cfg(unsigned int src, unsigned int src1)
{
    drv_sio_ctrl_stru  *pstCfg = DRV_SIO_GET_CFG_PTR();

    pstCfg->dma_cfg.play_cfg[0].uwLli = DRV_SIO_DMA_LLI_LINK((unsigned int)&pstCfg->dma_cfg.play_cfg[1]);
    pstCfg->dma_cfg.play_cfg[0].uhwACount = 320;  /* ��AMRÿ10ms����80���� */
    pstCfg->dma_cfg.play_cfg[0].uwSrcAddr = HI_SIO_REG_ADDR(HI_SIO_I2S_LEFT_XD_OFFSET);
    pstCfg->dma_cfg.play_cfg[0].uwDstAddr = src;
    pstCfg->dma_cfg.play_cfg[0].uwConfig  = DRV_SIO_DMA_SIO_MEM_CFG;

    pstCfg->dma_cfg.play_cfg[1].uwLli = DRV_SIO_DMA_LLI_LINK((unsigned int)&pstCfg->dma_cfg.play_cfg[0]);
    pstCfg->dma_cfg.play_cfg[1].uhwACount = 320;  /* խ��AMRÿ10ms����80���� */
    pstCfg->dma_cfg.play_cfg[1].uwSrcAddr = HI_SIO_REG_ADDR(HI_SIO_I2S_RIGHT_XD_OFFSET);
    pstCfg->dma_cfg.play_cfg[1].uwDstAddr = src1;
    pstCfg->dma_cfg.play_cfg[1].uwConfig  = DRV_SIO_DMA_SIO_MEM_CFG;

    return OK;
}

/*****************************************************************************
 �� �� ��  : bsp_sio_dma_start
 ��������  : SIO���������ʼ��, Ӧ���������ǰ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
int bsp_sio_dma_start(unsigned int channel_id)
{
    /*
    drv_sio_ctrl_stru  *pstCfg = DRV_SIO_GET_CFG_PTR();

    (void)balong_dma_channel_get_lli_addr(channel_id);

    balong_dma_channel_set_config(channel_id,0,0,0);

    ����EDMA���䣬�ȴ���������ź����ͷź󷵻�
    balong_dma_channel_lli_start(channel_id);
    */

    return OK;
}


/*****************************************************************************
 �� �� ��  : bsp_sio_dma_stop
 ��������  : SIO���������ʼ��, Ӧ���������ǰ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
int bsp_sio_dma_stop(unsigned int channel_id)
{
    /* drv_sio_ctrl_stru  *pstCfg = DRV_SIO_GET_CFG_PTR(); */

    /* bsp_edma_channel_stop(channel_id); */

    return OK;
}

/*****************************************************************************
 �� �� ��  : dma_spk_out_isr
 ��������  : SIO���������ʼ��, Ӧ���������ǰ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void dma_spk_out_isr(unsigned int uwPara)
{
    return ;
}

/*****************************************************************************
 �� �� ��  : dma_mic_in_isr
 ��������  : SIO���������ʼ��, Ӧ���������ǰ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void dma_mic_in_isr(unsigned int uwPara)
{
    return ;
}

/*****************************************************************************
 �� �� ��  : bsp_sio_set_mode
 ��������  : SIO���������ʼ��, Ӧ���������ǰ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_sio_set_mode(drv_sio_mode_e mode)
{
    drv_sio_ctrl_stru  *pstCfg = DRV_SIO_GET_CFG_PTR();

    pstCfg->enMode = mode;
}

/*****************************************************************************
 �� �� ��  : bsp_sio_set_sync_mode
 ��������  : SIO���������ʼ��, Ӧ���������ǰ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_sio_sync_mode(drv_sio_sync_mode_e mode)
{
    drv_sio_ctrl_stru  *pstCfg = DRV_SIO_GET_CFG_PTR();
    unsigned int sync_div = 0;
    unsigned int bclk_div = 0;
    struct clk *sio_bclk;
    struct clk *sio_sync;

    pstCfg->enSyncMode = mode;

    if(DRV_SIO_SYNC_MODE_MASTER == mode)
    {
        sio_bclk = clk_get(NULL, "sio_bclk");
        sio_sync = clk_get(NULL, "sio_syncclk");

        if((NULL == sio_bclk) || (NULL == sio_sync))
        {
            logMsg("sio bclk clk or sio clk get failed\n",1,2,3,4,5,6);
            return;
        }

        /* ����PCMʱ��Դ 19.2M */
        hi_syscrg_pcm_clk_sel();

        /* ����SIO ʱ����PADʱ��ͬ�� */
        hi_syscrg_sio_clk_positive();

        /* ��ģʽ */
        hi_syssc_pcm_mode_master();

        /* ����xfs��Ƶ */
        switch(pstCfg->sample_rate)
        {
            case DRV_SIO_SAMPLING_8K:
            {
                sync_div = 96;
                bclk_div = 25;
            }
            break;
            case DRV_SIO_SAMPLING_16K:
            {
                sync_div = 48;
                bclk_div = 25;
            }
            break;
            default:
            {
                sync_div = 96;
                bclk_div = 25;
            }
            break;
        }

        /* ����SIO masterʱʱ�ӷ�Ƶϵ��, 19200k��96��25=8k */
        if(0 != clk_set_rate(sio_bclk, bclk_div))
        {
            logMsg("line %d: clk_set_rate is failed, sio_bclk=0x%x,bclk_div is %d.\n",__LINE__, sync_div, bclk_div,4,5,6);
            return;
        }
        if(0 != clk_set_rate(sio_sync, sync_div))
        {
            logMsg("line %d: clk_set_rate is failed, sio_bclk=0x%x,bclk_div is %d.\n",__LINE__, sync_div, bclk_div,4,5,6);
            return;
        }
    }
    else
    {
        /* ����PCMʱ��Դ 19.2M */
        hi_syscrg_pcm_clk_sel();

        /* ����SIO ʱ����PADʱ��ͬ�� */
        hi_syscrg_sio_clk_positive();

        /* ��ģʽ */
        hi_syssc_pcm_mode_slave();
    }
}

/*****************************************************************************
 �� �� ��  : bsp_sio_set_sample
 ��������  : SIO���������ʼ��, Ӧ���������ǰ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_sio_set_sample(drv_sio_sample_e sample)
{
    drv_sio_ctrl_stru  *pstCfg = DRV_SIO_GET_CFG_PTR();

    if(sample < DRV_SIO_SAMPLING_BUTT){
        pstCfg->sample_rate = sample;
    }
}

/*****************************************************************************
 �� �� ��  : bsp_sio_set_chn
 ��������  : SIO���������ʼ��, Ӧ���������ǰ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_sio_set_chn(drv_sio_chn_num_e chn_num)
{
    drv_sio_ctrl_stru  *pstCfg = DRV_SIO_GET_CFG_PTR();

    if(chn_num < DRV_SIO_CHN_BUTT){
        pstCfg->chn_num = chn_num;
    }
}

/*****************************************************************************
 �� �� ��  : bsp_sio_isr_init
 ��������  : SIO���������ʼ��, Ӧ���������ǰ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_sio_isr_init()
{
    HI_SIO_INTCLR_T                *pstIntClr;
    HI_SIO_CT_CLR_T                *pstSioCtlClr;

    pstIntClr   = (HI_SIO_INTCLR_T*)HI_SIO_REG_ADDR(HI_SIO_INTCLR_OFFSET);
    pstSioCtlClr  = (HI_SIO_CT_CLR_T*)HI_SIO_REG_ADDR(HI_SIO_I2S_CT_CLR_OFFSET);

    /* ��ֹȫ���ж� */
    pstSioCtlClr->bits.intr_en = 1;

    /* ��ֹ����ͨ������ */
    pstSioCtlClr->bits.tx_fifo_disable = 1;
    pstSioCtlClr->bits.rx_fifo_disable = 1;
    pstSioCtlClr->bits.tx_enable = 1;
    pstSioCtlClr->bits.rx_enable = 1;

    /* �������SIO�ж� */
    pstIntClr->u32 = 0x3f;

    /*ע��SIO ISR*/
    intConnect ((VOIDFUNCPTR *)INT_LVL_LTESIO_SIO, bsp_sio_isr, 0);

    /*ʹ��SIO��� �ж�*/
    intEnable(INT_LVL_LTESIO_SIO);

    /*��ֹsio rx�ж�*/

    /* ʹ��ȫ���ж� */
    pstSioCtlClr->bits.intr_en = 1;
}

/*****************************************************************************
 �� �� ��  : bsp_sio_clk_get
 ��������  : SIO���������ʼ��, Ӧ���������ǰ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_sio_clk_get()
{
	struct clk          *clk_sio;
    drv_sio_ctrl_stru  *pstCfg = DRV_SIO_GET_CFG_PTR();

    clk_sio = clk_get(NULL, "sio_clk");

    pstCfg->sio_clk = clk_sio;

    logMsg("clk_sio 0x%x  pstCfg 0x%x\n", (u32)clk_sio, (u32)pstCfg, 3,4,5,6);

    return;
}

/*****************************************************************************
 �� �� ��  : bsp_sio_clk_enable
 ��������  : SIO���������ʼ��, Ӧ���������ǰ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_sio_clk_enable()
{
    drv_sio_ctrl_stru  *pstCfg = DRV_SIO_GET_CFG_PTR();

    if(!pstCfg->sio_clk)
    {
        logMsg("sio clk is null\n",1,2,3,4,5,6);
        return;
    }

    clk_enable(pstCfg->sio_clk);
}

/*****************************************************************************
 �� �� ��  : bsp_sio_clk_disable
 ��������  : SIO���������ʼ��, Ӧ���������ǰ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_sio_clk_disable()
{
    drv_sio_ctrl_stru  *pstCfg = DRV_SIO_GET_CFG_PTR();

    if(!pstCfg->sio_clk)
    {
        logMsg("sio clk is null\n",1,2,3,4,5,6);
        return;
    }

    clk_disable(pstCfg->sio_clk);
}

/*****************************************************************************
 �� �� ��  : bsp_sio_slave_cfg
 ��������  : SIO���������ʼ��, Ӧ���������ǰ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_sio_pcm_cfg()
{
    HI_SIO_MODE_T   *pstSioMode;
    pstSioMode      = (HI_SIO_MODE_T*)HI_SIO_REG_ADDR(HI_SIO_MODE_OFFSET);

    /* ѡ��ʱ������.���óɱ�׼ģʽ */
    pstSioMode->bits.pcm_mode = 0;

    /* ��·¼����Ҫ����SIO_MODE[ext_rec_en] SIO_MODE[chn_num]��SIO_MODE[clk_edge] */
    if(pstSioMode->bits.chn_num > DRV_SIO_CHN_2)
    {
        pstSioMode->bits.clk_edge = 1; /* ����ʱ�����ó���������Ч */
        pstSioMode->bits.ext_rec_en = 1; /* ��չģʽ */
    }
}

/*****************************************************************************
 �� �� ��  : bsp_sio_master_cfg
 ��������  : SIO���������ʼ��, Ӧ���������ǰ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_sio_i2s_cfg()
{
    HI_SIO_I2S_POS_MERGE_EN_T      *pstMerge;
    HI_SIO_I2S_START_POS_T         *pstStartPos;
    HI_SIO_CT_SET_T                *pstCtSet;
    HI_SIO_SIGNED_EXT_T            *pstSignedExt;
    HI_SIO_MODE_T                  *pstSioMode;

    pstSioMode  = (HI_SIO_MODE_T*)HI_SIO_REG_ADDR(HI_SIO_MODE_OFFSET);
    pstMerge    = (HI_SIO_I2S_POS_MERGE_EN_T*)HI_SIO_REG_ADDR(HI_SIO_I2S_POS_MERGE_EN_OFFSET);
    pstStartPos = (HI_SIO_I2S_START_POS_T*)HI_SIO_REG_ADDR(HI_SIO_I2S_START_POS_OFFSET);
    pstSignedExt= (HI_SIO_SIGNED_EXT_T*)HI_SIO_REG_ADDR(HI_SIO_SIGNED_EXT_OFFSET);
    pstCtSet   =  (HI_SIO_CT_SET_T*)HI_SIO_REG_ADDR(HI_SIO_I2S_CT_SET_OFFSET);

    /* ����ʵ�ʶ�дFIFO���ݵķ�ʽ, ����SIO_I2S_POS_MERGE_EN��SIO_I2S_START_POS*/
    pstMerge->bits.merge_en = 1;             /* ���������ϲ�ʹ�� */
    pstStartPos->bits.start_pos_read  = 0;   /* ��������ʼ���� */
    pstStartPos->bits.start_pos_write = 0;   /* ��������ʼ���� */

    /* ����SIO_CT_SET[tx_data_merge_en]��SIO_CT_SET[rx_data_merge_en]. ��·¼�����ý��иò�������*/
    pstCtSet->bits.tx_data_merge_en = 1;
    pstCtSet->bits.rx_data_merge_en = 1;
}

/*****************************************************************************
 �� �� ��  : bsp_sio_init
 ��������  : SIO���������ʼ��, Ӧ���������ǰ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
int bsp_sio_init(void)
{
    drv_sio_int_isr_stru                *pstIntIsr = DRV_SIO_GET_INT_ISR_PTR();
    drv_sio_status_stru                 *pstStatus = DRV_SIO_GET_STATUS_PTR();
    drv_sio_ctrl_stru                   *pstCfg = DRV_SIO_GET_CFG_PTR();

    memset(pstIntIsr,0,sizeof(drv_sio_int_isr_stru));
    memset(pstStatus,0,sizeof(drv_sio_status_stru));

#if 0
    /* ��ʼ�����պͷ���dma */
    if(OK != (ret = bsp_sio_dma_init()));
    {
        logMsg("drv_sio_dma_init failed \n",1,2,3,4,5,6);
        return ERROR;
    }
#endif

    bsp_sio_clk_get();

    if(!pstCfg->sio_clk)
    {
        logMsg("bsp_sio_clk_get failed \n",1,2,3,4,5,6);
        return ERROR ;
    }

    bsp_sio_clk_enable();

    bsp_sio_reset();

    /* ��ʼ��Ĭ������ SLAVEģʽ��PCM�ӿڡ�������Ϊ8K */
    bsp_sio_sync_mode(DRV_SIO_SYNC_MODE_SLAVE);

    bsp_sio_set_mode(DRV_SIO_MODE_PCM);

    bsp_sio_set_sample(DRV_SIO_SAMPLING_8K);

    bsp_sio_set_chn(DRV_SIO_CHN_2);

     /* �ж����� */
    bsp_sio_isr_init();

    bsp_sio_unreset();

    /* ���Ϊ�ر� */
    pstStatus->enOpen = DRV_SIO_STATE_CLOSE;

    bsp_sio_clk_disable();
    return OK;
}

/*****************************************************************************
 �� �� ��  : bsp_sio_open
 ��������  : ��SIO����ע��ص�����
 �������  : enIntMask       - �ж�ʹ����
             pfIntHandleFunc - �ص�����ָ��
             uwPara          - �ص�����
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
int bsp_sio_open(
                drv_sio_int_mask    enIntMask,
                drv_sio_int_func    pfIntHandleFunc,
                unsigned int        uwPara)
{
    drv_sio_int_isr_stru                *pstIntIsr      = DRV_SIO_GET_INT_ISR_PTR();
    drv_sio_status_stru                 *pstStatus      = DRV_SIO_GET_STATUS_PTR();
    drv_sio_ctrl_stru                   *pstCfg          = DRV_SIO_GET_CFG_PTR();
    HI_SIO_MODE_T                       *pstSioMode;
    HI_SIO_DATA_WIDTH_SET_T             *pstSioWidth;
    HI_SIO_CT_CLR_T                     *pstSioCtlClr;
    HI_SIO_INTMASK_T                    *pstSioIntMask;
    HI_SIO_CT_SET_T                     *pstSioCtlSet;
    HI_SIO_SIGNED_EXT_T                 *pstSioExt;

    pstSioMode = (HI_SIO_MODE_T*)HI_SIO_REG_ADDR(HI_SIO_MODE_OFFSET);
    pstSioIntMask = (HI_SIO_INTMASK_T*)HI_SIO_REG_ADDR(HI_SIO_INTMASK_OFFSET);
    pstSioCtlClr = (HI_SIO_CT_CLR_T*)HI_SIO_REG_ADDR(HI_SIO_INTCLR_OFFSET);
    pstSioWidth = (HI_SIO_DATA_WIDTH_SET_T*)HI_SIO_REG_ADDR(HI_SIO_DATA_WIDTH_SET_OFFSET);
    pstSioCtlSet = (HI_SIO_CT_SET_T*)HI_SIO_REG_ADDR(HI_SIO_I2S_CT_SET_OFFSET);
    pstSioExt = (HI_SIO_SIGNED_EXT_T*)HI_SIO_REG_ADDR(HI_SIO_SIGNED_EXT_OFFSET);

    if (DRV_SIO_STATE_OPEN == pstStatus->enOpen)
    {
        logMsg("sio already opened \n",1,2,3,4,5,6);
        return ERROR;
    }

    bsp_sio_clk_enable();

    /* ���Ϊ�� */
    pstStatus->enOpen = DRV_SIO_STATE_OPEN;

    /*дDRV_SIO_CTRL_CLRs 0xfff0��λSIO����ֹ���䡢��ֹ�ж�*/
    pstSioCtlClr->u32 = 0xffff;

    /* ��������SIO�ж� */
    pstSioIntMask->u32 = 0x3f;

    /* �⸴λ */
    pstSioCtlSet->bits.rst_n = 1;

   /*����ΪPCM/I2S�ӿ�ģʽ*/
    pstSioMode->bits.sio_mode = pstCfg->enMode;

    /* ���ý���FIFO�ͷ���FIFO��ˮ�� */
    pstSioCtlSet->bits.tx_fifo_threshold = 8;
    pstSioCtlSet->bits.rx_fifo_threshold = 8;

    /* ����ͨ���� */
    pstSioMode->bits.chn_num = pstCfg->chn_num;

    /*����Ϊ���͡�����16bitλ��*/
    pstSioWidth->bits.tx_mode = 1;
    pstSioWidth->bits.rx_mode = 1;

    /* ��ͨ����������ó���չģʽ */
    if(pstSioMode->bits.chn_num > DRV_SIO_CHN_2)
    {
        pstSioMode->bits.ext_rec_en = 1; /* ��չģʽ */
    }

    if (DRV_SIO_MODE_I2S == pstCfg->enMode)
    {
        bsp_sio_i2s_cfg();
    }
    else
    {
        bsp_sio_pcm_cfg();
    }

    /*���÷�����չ��ֹ*/
    pstSioExt->u32 = 0;

    if (NULL != pfIntHandleFunc)
    {
        /* �����ж����Σ�ʹ���ж�*/
        pstSioIntMask->u32 = ~enIntMask;

        pstIntIsr->pfFunc = pfIntHandleFunc;
        pstIntIsr->uwPara = uwPara;
        pstSioCtlSet->bits.intr_en = 1;
    }

    /*ʹ��SIO���ա�����*/
    pstSioCtlSet->bits.rx_enable = 1;
    pstSioCtlSet->bits.tx_enable = 1;

    /* bsp_sio_clk_disable(); */

    return OK;

}

/*****************************************************************************
 �� �� ��  : bsp_sio_play_start
 ��������  : �ر�SIO������ص�������Ϣ
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_sio_play_start()
{
    HI_SIO_CT_SET_T                     *pstSioCtlSet;
    HI_SIO_I2S_LEFT_XD_T                *pstSioLxd;
    HI_SIO_I2S_RIGHT_XD_T               *pstSioRxd;

    pstSioCtlSet = (HI_SIO_CT_SET_T*)HI_SIO_REG_ADDR(HI_SIO_I2S_CT_SET_OFFSET);
    pstSioRxd = (HI_SIO_I2S_RIGHT_XD_T*)HI_SIO_REG_ADDR(HI_SIO_PCM_XD_OFFSET);     /* PCM ��ʽ������������ */
    pstSioLxd = (HI_SIO_I2S_LEFT_XD_T*)HI_SIO_REG_ADDR(HI_SIO_I2S_LEFT_XD_OFFSET);

    /* EDMA���� */

    /* д���ʼ����0 */
    pstSioRxd->u32 = 0;
    pstSioLxd->u32 = 0;

    /* �������� */
    pstSioCtlSet->bits.tx_enable = 1;
}

/*****************************************************************************
 �� �� ��  : bsp_sio_record_start
 ��������  : �ر�SIO������ص�������Ϣ
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_sio_record_start()
{
    HI_SIO_CT_SET_T                     *pstSioCtlSet;

    pstSioCtlSet = (HI_SIO_CT_SET_T*)HI_SIO_REG_ADDR(HI_SIO_I2S_CT_SET_OFFSET);

    /* EDMA���� */

    /* �������� */
    pstSioCtlSet->bits.rx_enable = 1;
}

/*****************************************************************************
 �� �� ��  : bsp_sio_play_stop
 ��������  : �ر�SIO������ص�������Ϣ
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_sio_play_stop()
{
    HI_SIO_CT_CLR_T          *pstSioCtlClr;
    drv_sio_ctrl_stru        *pstCfg = DRV_SIO_GET_CFG_PTR();

    pstSioCtlClr = (HI_SIO_CT_CLR_T*)HI_SIO_REG_ADDR(HI_SIO_I2S_CT_CLR_OFFSET);

    /* ֹͣ���� */
    pstSioCtlClr->bits.tx_enable = 1;

    bsp_sio_dma_stop(pstCfg->dma_cfg.tx_channel_id);
}

/*****************************************************************************
 �� �� ��  : bsp_sio_record_stop
 ��������  : �ر�SIO������ص�������Ϣ
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_sio_record_stop()
{
    HI_SIO_CT_CLR_T             *pstSioCtlClr;
    drv_sio_ctrl_stru           *pstCfg = DRV_SIO_GET_CFG_PTR();

    pstSioCtlClr = (HI_SIO_CT_CLR_T*)HI_SIO_REG_ADDR(HI_SIO_I2S_CT_CLR_OFFSET);

    /* ֹͣ¼�� */
    pstSioCtlClr->bits.rx_enable = 1;

    bsp_sio_dma_stop(pstCfg->dma_cfg.rx_channel_id);
}

/*****************************************************************************
 �� �� ��  : bsp_sio_close
 ��������  : �ر�SIO������ص�������Ϣ
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_sio_close(void)
{
    drv_sio_int_isr_stru                *pstIntIsr = DRV_SIO_GET_INT_ISR_PTR();
    drv_sio_status_stru                 *pstStatus = DRV_SIO_GET_STATUS_PTR();

    HI_SIO_CT_CLR_T                     *pstSioCtlClr;
    HI_SIO_INTMASK_T                    *pstSioIntMask;
    HI_SIO_CT_SET_T                     *pstSioCtlSet;
    HI_SIO_INTCLR_T                 *pstSioIntClr;

    pstSioIntMask = (HI_SIO_INTMASK_T*)HI_SIO_REG_ADDR(HI_SIO_INTMASK_OFFSET);
    pstSioCtlClr = (HI_SIO_CT_CLR_T*)HI_SIO_REG_ADDR(HI_SIO_INTCLR_OFFSET);
    pstSioCtlSet = (HI_SIO_CT_SET_T*)HI_SIO_REG_ADDR(HI_SIO_I2S_CT_SET_OFFSET);
    pstSioIntClr = (HI_SIO_INTCLR_T*)HI_SIO_REG_ADDR(HI_SIO_INTCLR_OFFSET);

    if(DRV_SIO_STATE_CLOSE == pstStatus->enOpen)
    {
        logMsg("drv sio already opened \n", 1,2,3,4,5,6);
        return;
    }

    /*��λSIO����ֹ���䡢��ֹ�ж�*/
    pstSioCtlClr->u32 = 0xffff;

    /*��������SIO�ж�*/
    pstSioIntMask->u32 = 0xffffffff;

    /*���SIO�ж�*/
    pstSioIntClr->u32 = 0x3f;

    /*�⸴λSIO*/
    pstSioCtlSet->bits.rst_n = 1;

    /*����жϻص����������Ϣ*/
    memset(pstIntIsr, 0, sizeof(drv_sio_int_isr_stru));

    bsp_sio_clk_disable();

    /* ���Ϊ�ر� */
    pstStatus->enOpen = DRV_SIO_STATE_CLOSE;

}

/*****************************************************************************
 �� �� ��  : bsp_sio_isr
 ��������  : SIO�жϴ���ص�������ʹ��VOS�ṩ���ж�ע��ӿ�ע�᱾����
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_sio_isr(void)
{
    unsigned int                   uwIntState;
    HI_SIO_INTSTATUS_T             *pstIntStat;
    HI_SIO_INTCLR_T                *pstIntClr;
    drv_sio_int_isr_stru           *pstIntIsr = DRV_SIO_GET_INT_ISR_PTR();

    pstIntStat   = (HI_SIO_INTSTATUS_T*)HI_SIO_REG_ADDR(HI_SIO_INTSTATUS_OFFSET);
    pstIntClr   = (HI_SIO_INTCLR_T*)HI_SIO_REG_ADDR(HI_SIO_INTCLR_OFFSET);

    /*��ȡSIO�ж�״̬*/
    uwIntState  = pstIntStat->u32;

    /*�����ѯ�����ж�*/
    pstIntClr->u32 = uwIntState;

    /*���ж�״̬Ϊ0��ֱ�ӷ���*/
    if (0 == uwIntState)
    {
        return;
    }

    /*��SIO�жϻص������ǿ�,�����*/
    if (NULL != pstIntIsr->pfFunc)
    {
        pstIntIsr->pfFunc((drv_sio_int_mask)uwIntState, pstIntIsr->uwPara);
    }
}

/*****************************************************************************
 �� �� ��  : bsp_sio_reset
 ��������  : SIO�жϴ���ص�������ʹ��VOS�ṩ���ж�ע��ӿ�ע�᱾����
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_sio_reset()
{
    HI_SIO_CT_CLR_T                *pstSioCtlClr;

    pstSioCtlClr   = (HI_SIO_CT_CLR_T*)HI_SIO_REG_ADDR(HI_SIO_I2S_CT_CLR_OFFSET);

    /*��λSIO */
    pstSioCtlClr->bits.rst_n = 1;
}

/*****************************************************************************
 �� �� ��  : bsp_sio_unreset
 ��������  : SIO�жϴ���ص�������ʹ��VOS�ṩ���ж�ע��ӿ�ע�᱾����
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_sio_unreset()
{
    HI_SIO_CT_SET_T                *pstSioCtrlSet;

    pstSioCtrlSet   = (HI_SIO_CT_SET_T*)HI_SIO_REG_ADDR(HI_SIO_I2S_CT_SET_OFFSET);

    /*�⸴λSIO*/
    pstSioCtrlSet->bits.rst_n = 1;
}
#ifdef __cplusplus
}
#endif
