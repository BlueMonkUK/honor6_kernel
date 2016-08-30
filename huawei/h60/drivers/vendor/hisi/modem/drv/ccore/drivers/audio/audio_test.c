/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include <vxWorks.h>
#include <intLib.h>
#include <logLib.h>
#include <memLib.h>
#include "semLib.h"
#include "drv_audio.h"
#include "bsp_audio.h"
#include "audio_test.h"
#include "bsp_sio.h"
#include "soc_interrupts.h"
#include "hi_sio.h"

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

signed short   g_mic_in_audio_data_a[160] = {0};
signed short   g_mic_in_audio_data_b[160] = {0};
signed short   g_spk_out_audio_data_a[160] = {0};
signed short   g_spk_out_audio_data_b[160] = {0};

int   *g_psMicPcm = NULL;
int   *g_psSpkPcm = NULL;

unsigned short *record_buff = NULL;
unsigned short *record_buff_pos = NULL;
unsigned short *play_buff_pos = NULL;
unsigned int work_mode = DRV_SIO_MODE_PCM;

DRV_DMA_CXCFG_STRU g_mic_in_dma_cfg[2];
DRV_DMA_CXCFG_STRU g_spk_out_dma_cfg[2];

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
void micin_isr(drv_dma_int_type enIntType, unsigned int uwPara)
{
	unsigned int ulDestAddr, ulDestAddrB;
	ulDestAddrB = (unsigned int)&g_mic_in_audio_data_b[0];

	ulDestAddr = DMA_REG_READ(DRV_DMA_CX_DES_ADDR(15));

	if((ulDestAddr >= (ulDestAddrB - sizeof(unsigned int)))
	   &&(ulDestAddr < (ulDestAddrB + (((unsigned int)(int)160 * sizeof(signed short))
	   - sizeof(unsigned int)))))
	   {
	        g_psMicPcm = (int*)g_mic_in_audio_data_a;
	   }
	   else
	   {
	        g_psMicPcm = (int*)g_mic_in_audio_data_b;
	   }

}

void spkout_isr(drv_dma_int_type enIntType, unsigned int uwPara)
{
	unsigned int ulSrcAddr, ulSrcAddrB;

	ulSrcAddrB = (unsigned int)&g_spk_out_audio_data_b[0];

	ulSrcAddr = DMA_REG_READ(DRV_DMA_CX_SRC_ADDR(14));

	if((ulSrcAddr >= (ulSrcAddrB - sizeof(unsigned int)))
	   &&(ulSrcAddr < (ulSrcAddrB + (((unsigned short)160 * sizeof(signed short))
	   - sizeof(unsigned int)))))
	   {
	        g_psSpkPcm = (int*)g_spk_out_audio_data_a;
	   }
	   else
	   {
	        g_psSpkPcm = (int*)g_spk_out_audio_data_b;
	   }

}

/* porting��֤�������� */
void mux_cfg(void)
{
    unsigned int tmp = 0;

    tmp = *(volatile int *)0x9001000c;
    tmp &= ~(0xfc0);
    *(volatile int *)0x9001000c = tmp;

    tmp = *(volatile int *)0x90010410;
    tmp |= (0x18);
    *(volatile int *)0x90010410 = tmp;
}

/* 1:I2S,0:PCM */
unsigned int audio_test_init(int mode, int chn_num)
{
    int ret = BSP_OK;
    int fd = NULL;
    int tmp = IOCTL_AUDIO_PCM_MODE_SET;

#if 0
    mux_cfg();

    /*open audio dev*/
    ret = audio_create("/dev/codec0", 0);
    if (BSP_OK != ret)
    {
        logMsg("\r audio_create  fail, result = 0x%x\n",ret,0,0,0,0,0);
        return BSP_ERROR;
    }
#endif
    ret = audio_open("/dev/codec0", 0, 0);
    if (BSP_OK != ret)
    {
        logMsg("\r audio_create  fail, result = 0x%x\n",ret,0,0,0,0,0);
        return BSP_ERROR;
    }

    tmp = mode?(IOCTL_AUDIO_I2S_MODE_SET):(IOCTL_AUDIO_PCM_MODE_SET);
    /* audio_ioctl(fd, IOCTL_AUDIO_PCM_MODE_SET, 0); */
    audio_ioctl(fd, tmp, 0);

    audio_ioctl(fd, IOCTL_AUDIO_SAMPLE_RATE_SET, (AUDIO_SAMPLE_RATE)SAMPLE_RATE_8K);
    audio_ioctl(fd, IOCTL_AUDIO_VOICE_OPEN, NULL);

    audio_ioctl(fd, IOCTL_AUDIO_IN_DEV_SELECT, HEADSET);
    audio_ioctl(fd, IOCTL_AUDIO_OUT_DEV_SELECT, HEADSET);

    /* sio ��ʼ�� */
    ret = bsp_sio_init();
    if (BSP_OK != ret)
    {
        logMsg("\r bsp_sio_init  fail, result = 0x%x\n",ret,0,0,0,0,0);
        return BSP_ERROR;
    }

    work_mode= mode;
    tmp = mode?(DRV_SIO_MODE_I2S):(DRV_SIO_MODE_PCM);
    /* bsp_sio_set_mode(DRV_SIO_MODE_I2S); */
    bsp_sio_set_mode(tmp);

    bsp_sio_set_chn(chn_num);

    ret = bsp_sio_open(DRV_SIO_INT_MASK_RX_RIGHT_FIFO_OVER|DRV_SIO_INT_MASK_TX_RIGHT_FIFO_UNDER, 0, 0);
    if (BSP_OK != ret)
    {
        logMsg("\r bsp_sio_init  fail, result = 0x%x\n",ret,0,0,0,0,0);
        return BSP_ERROR;
    }

    /* dma ��ʼ�� */
    bsp_dma_init();

}

int audio_record_isr(drv_dma_int_type enIntType, unsigned int uwPara)
{
    int uwRet;
    DRV_DMA_CXCFG_STRU   stCfg;
    unsigned int tmp_reg = 0;

    if(DRV_DMA_INT_TYPE_TC1 != enIntType)
    {
        logMsg("drv_dma_int_type: 0x%x\n",enIntType,0,0,0,0,0);
        return -2;
    }
    record_buff_pos= record_buff_pos + 160;

    tmp_reg = work_mode?(HI_SIO_I2S_DUAL_RX_CHN_OFFSET):(HI_SIO_PCM_RD_OFFSET);

    memset(&stCfg, 0, sizeof(DRV_DMA_CXCFG_STRU));

    if((record_buff_pos - record_buff) >= 0x200000 / 4)
        return -1;

    stCfg.uhwACount = 320;

    stCfg.uwSrcAddr = HI_SIO_REG_ADDR(tmp_reg);
    /* stCfg.uwSrcAddr = HI_SIO_REG_ADDR(HI_SIO_PCM_RD_OFFSET); */

    stCfg.uwDstAddr = (unsigned int)record_buff_pos;
    stCfg.uwConfig  = DRV_DMA_SIO_MEM_CFG;

    uwRet = bsp_dma_startwithcfg(15,
              &stCfg,
              audio_record_isr,
              0);

    return uwRet;
}

int audio_play_isr(drv_dma_int_type enIntType, unsigned int uwPara)
{
    int uwRet;
    DRV_DMA_CXCFG_STRU   stCfg = {0};
    unsigned int tmp_reg = 0;

    if(DRV_DMA_INT_TYPE_TC1 != enIntType)
    {
        logMsg("drv_dma_int_type: 0x%x\n",enIntType,0,0,0,0,0);
        return -2;
    }

    play_buff_pos= play_buff_pos + 160;

    tmp_reg = work_mode?(HI_SIO_I2S_DUAL_TX_CHN_OFFSET):(HI_SIO_PCM_XD_OFFSET);

    memset(&stCfg, 0, sizeof(DRV_DMA_CXCFG_STRU));

    if((play_buff_pos - record_buff) >= 0x200000 / 4)
        return -1;

    stCfg.uhwACount = 320;
    stCfg.uwSrcAddr = (unsigned int)play_buff_pos;
    /* stCfg.uwDstAddr = HI_SIO_REG_ADDR(HI_SIO_I2S_DUAL_TX_CHN_OFFSET); */
    stCfg.uwDstAddr = HI_SIO_REG_ADDR(tmp_reg);
    stCfg.uwConfig  = DRV_DMA_MEM_SIO_CFG;

    uwRet = bsp_dma_startwithcfg(14,
              &stCfg,
              audio_play_isr,
              0);

    return uwRet;
}

unsigned int audio_play_test()
{
    unsigned int uwRet;
    DRV_DMA_CXCFG_STRU                  stCfg;
    unsigned int tmp_reg = 0;

    memset(&stCfg, 0, sizeof(DRV_DMA_CXCFG_STRU));
    play_buff_pos = record_buff;

    tmp_reg = work_mode?(HI_SIO_I2S_DUAL_TX_CHN_OFFSET):(HI_SIO_PCM_XD_OFFSET);

    stCfg.uhwACount = 320;
    stCfg.uwSrcAddr = (unsigned int)play_buff_pos;

    stCfg.uwDstAddr = HI_SIO_REG_ADDR(tmp_reg);
    /* stCfg.uwSrcAddr = HI_SIO_REG_ADDR(HI_SIO_PCM_XD_OFFSET); */
    stCfg.uwConfig  = DRV_DMA_MEM_SIO_CFG;

    uwRet = bsp_dma_startwithcfg(14,
              &stCfg,
              audio_play_isr,
              0);
    return uwRet;
}

int audio_record_test()
{
    unsigned int uwRet;
    DRV_DMA_CXCFG_STRU                  stCfg;
    unsigned int tmp_reg = 0;

    record_buff = (unsigned short *)malloc(2*1024*1024);

    memset((unsigned char*)record_buff, 0, 2*1024*1024);
    record_buff_pos = record_buff;

    tmp_reg = work_mode?(HI_SIO_I2S_DUAL_RX_CHN_OFFSET):(HI_SIO_PCM_RD_OFFSET);

    memset(&stCfg, 0, sizeof(DRV_DMA_CXCFG_STRU));

    if(NULL == record_buff)
    {
        return -1;
    }

    stCfg.uhwACount = 320;
    stCfg.uwSrcAddr = HI_SIO_REG_ADDR(tmp_reg);
    stCfg.uwDstAddr = (unsigned int)record_buff_pos;
    stCfg.uwConfig  = DRV_DMA_SIO_MEM_CFG;

    uwRet = bsp_dma_startwithcfg(15,
              &stCfg,
              audio_record_isr,
              0);
    return uwRet;
}

int audio_pcm_loop_test()
{
    int uwRet;

    g_psMicPcm = g_mic_in_audio_data_a;
    g_psSpkPcm = g_spk_out_audio_data_a;

    memset(g_mic_in_dma_cfg,0,sizeof(g_mic_in_dma_cfg));

    g_mic_in_dma_cfg[0].uwLli = DRV_DMA_LLI_LINK((unsigned int)&g_mic_in_dma_cfg[1]);
    g_mic_in_dma_cfg[0].uhwACount = 320;  /*խ��AMRÿ10ms����80����*/
    g_mic_in_dma_cfg[0].uwSrcAddr = HI_SIO_REG_ADDR(HI_SIO_PCM_RD_OFFSET);
    g_mic_in_dma_cfg[0].uwDstAddr = (unsigned int)&g_mic_in_audio_data_a[0];
    g_mic_in_dma_cfg[0].uwConfig  = DRV_DMA_SIO_MEM_CFG;

    g_mic_in_dma_cfg[1].uwLli = DRV_DMA_LLI_LINK((unsigned int)&g_mic_in_dma_cfg[0]);
    g_mic_in_dma_cfg[1].uhwACount = 320;  /*խ��AMRÿ10ms����80����*/
    g_mic_in_dma_cfg[1].uwSrcAddr = HI_SIO_REG_ADDR(HI_SIO_PCM_RD_OFFSET);
    g_mic_in_dma_cfg[1].uwDstAddr = (unsigned int)&g_mic_in_audio_data_b[0];
    g_mic_in_dma_cfg[1].uwConfig  = DRV_DMA_SIO_MEM_CFG;


    memset(g_spk_out_dma_cfg,0,sizeof(g_spk_out_dma_cfg));

    g_spk_out_dma_cfg[0].uwLli = DRV_DMA_LLI_LINK((unsigned int)&g_spk_out_dma_cfg[1]);
    g_spk_out_dma_cfg[0].uhwACount = 320;  /*խ��AMRÿ10ms����80����*/
    g_spk_out_dma_cfg[0].uwSrcAddr = (unsigned int)&g_mic_in_audio_data_b[0];
    g_spk_out_dma_cfg[0].uwDstAddr = HI_SIO_REG_ADDR(HI_SIO_PCM_XD_OFFSET);

    g_spk_out_dma_cfg[0].uwConfig  = DRV_DMA_MEM_SIO_CFG;

    g_spk_out_dma_cfg[1].uwLli = DRV_DMA_LLI_LINK((unsigned int)&g_spk_out_dma_cfg[0]);
    g_spk_out_dma_cfg[1].uhwACount = 320;  /*խ��AMRÿ10ms����80����*/
    g_spk_out_dma_cfg[1].uwSrcAddr = (unsigned int)&g_mic_in_audio_data_a[0];
    g_spk_out_dma_cfg[1].uwDstAddr = HI_SIO_REG_ADDR(HI_SIO_PCM_XD_OFFSET);

    g_spk_out_dma_cfg[1].uwConfig  = DRV_DMA_MEM_SIO_CFG;
	uwRet = bsp_dma_startwithcfg(15,
              &g_mic_in_dma_cfg[0],
              0,
              0);

    uwRet = bsp_dma_startwithcfg(14,
              &g_spk_out_dma_cfg[0],
              0,
              0);

    return uwRet;
}

int audio_i2s_loop_test()
{
    int uwRet;

    g_psMicPcm = g_mic_in_audio_data_a;
    g_psSpkPcm = g_spk_out_audio_data_a;

    memset(g_mic_in_dma_cfg,0,sizeof(g_mic_in_dma_cfg));

    g_mic_in_dma_cfg[0].uwLli = DRV_DMA_LLI_LINK((unsigned int)&g_mic_in_dma_cfg[1]);
    g_mic_in_dma_cfg[0].uhwACount = 320;
    g_mic_in_dma_cfg[0].uwSrcAddr = HI_SIO_REG_ADDR(HI_SIO_I2S_DUAL_RX_CHN_OFFSET);
    g_mic_in_dma_cfg[0].uwDstAddr = (unsigned int)&g_mic_in_audio_data_a[0];
    g_mic_in_dma_cfg[0].uwConfig  = DRV_DMA_SIO_MEM_CFG;

    g_mic_in_dma_cfg[1].uwLli = DRV_DMA_LLI_LINK((unsigned int)&g_mic_in_dma_cfg[0]);
    g_mic_in_dma_cfg[1].uhwACount = 320;
    g_mic_in_dma_cfg[1].uwSrcAddr = HI_SIO_REG_ADDR(HI_SIO_I2S_DUAL_RX_CHN_OFFSET);
    g_mic_in_dma_cfg[1].uwDstAddr = (unsigned int)&g_mic_in_audio_data_b[0];
    g_mic_in_dma_cfg[1].uwConfig  = DRV_DMA_SIO_MEM_CFG;


    memset(g_spk_out_dma_cfg,0,sizeof(g_spk_out_dma_cfg));


    g_spk_out_dma_cfg[0].uwLli = DRV_DMA_LLI_LINK((unsigned int)&g_spk_out_dma_cfg[1]);
    g_spk_out_dma_cfg[0].uhwACount = 320;
    g_spk_out_dma_cfg[0].uwSrcAddr = (unsigned int)&g_mic_in_audio_data_b[0];
    g_spk_out_dma_cfg[0].uwDstAddr = HI_SIO_REG_ADDR(HI_SIO_I2S_DUAL_TX_CHN_OFFSET);

    g_spk_out_dma_cfg[0].uwConfig  = DRV_DMA_MEM_SIO_CFG;

    g_spk_out_dma_cfg[1].uwLli = DRV_DMA_LLI_LINK((unsigned int)&g_spk_out_dma_cfg[0]);
    g_spk_out_dma_cfg[1].uhwACount = 320;
    g_spk_out_dma_cfg[1].uwSrcAddr = (unsigned int)&g_mic_in_audio_data_a[0];
    g_spk_out_dma_cfg[1].uwDstAddr = HI_SIO_REG_ADDR(HI_SIO_I2S_DUAL_TX_CHN_OFFSET);

    g_spk_out_dma_cfg[1].uwConfig  = DRV_DMA_MEM_SIO_CFG;
	uwRet = bsp_dma_startwithcfg(15,
              &g_mic_in_dma_cfg[0],
              0,
              0);

    uwRet = bsp_dma_startwithcfg(14,
              &g_spk_out_dma_cfg[0],
              0,
              0);

    return uwRet;
}

void bsp_dma_init( void )
{
    DRV_DMA_CXISR_STRU                 *pstDmaCxIsr = DRV_DMA_GetCxIsrPtr();

    /*��ʼ��g_astDrvDmaCxIntIsrΪȫ��*/
    memset(pstDmaCxIsr, 0, sizeof(DRV_DMA_CXISR_STRU)*DRV_DMA_MAX_CHANNEL_NUM);

    DMA_REG_WRITEBIT(DRV_SC_DMA_SEL_CTRL_ADDR, 2, 2, 1);

    /* ���ж�״̬ */
    DMA_REG_WRITE(DRV_DMA_INT_ERR1, 0xffff);
    DMA_REG_WRITE(DRV_DMA_INT_ERR2, 0xffff);
    DMA_REG_WRITE(DRV_DMA_INT_ERR3, 0xffff);
    DMA_REG_WRITE(DRV_DMA_INT_TC1, 0xffff);
    DMA_REG_WRITE(DRV_DMA_INT_TC2, 0xffff);

    /* �������� */
    DMA_REG_WRITE(DRV_DMA_INT_ERR1_MASK, 0xc000);
    DMA_REG_WRITE(DRV_DMA_INT_ERR2_MASK, 0xc000);
    DMA_REG_WRITE(DRV_DMA_INT_ERR3_MASK, 0xc000);
    DMA_REG_WRITE(DRV_DMA_INT_TC1_MASK, 0xc000);
    DMA_REG_WRITE(DRV_DMA_INT_TC2_MASK, 0xc000);

    /*ע��DMA ISR*/
    (void)intConnect ((VOIDFUNCPTR *)INT_LVL_EDMAC0, (VOIDFUNCPTR)bsp_dma_isr, 0);

    (void)intEnable(INT_LVL_EDMAC0);
}

/*****************************************************************************
 �� �� ��  : bsp_dma_startwithcfg
 ��������  : ������ϸ���õ�DMAͨ������
 �������  : uhwChannelNo   - ͨ����, ȡֵ0-15
             *pstCfg        - ���ò���, ��ʹ����������������Ҫ32bytes����
             pfIntHandleFunc- DMA�жϻص�����ָ��
             uwPara         - DMA�жϻص������Ļص�����
 �������  : ��
 �� �� ֵ  : unsigned int     - VOS��׼����ֵ��BSP_ERROR(ʧ��)/BSP_OK(�ɹ�)
 ���ú���  :
 ��������  :
*****************************************************************************/
unsigned int bsp_dma_startwithcfg(
                unsigned short              uhwChannelNo,
                DRV_DMA_CXCFG_STRU     *pstCfg,
                drv_dma_int_func        pfIntHandleFunc,
                unsigned int              uwPara)
{
    unsigned int                          uwChannelMask   = (0x1L << uhwChannelNo);
    DRV_DMA_CXISR_STRU                 *pstCxIsr        = DRV_DMA_GetCxIsrPtr();

    /* �������Ƿ�Ƿ� */
    if (uhwChannelNo >= DRV_DMA_MAX_CHANNEL_NUM)
    {
        return BSP_ERROR;
    }

    /*д0��DRV_DMA_CX_CONFIG(uhwChannelNo) bit0��ֹͨ��*/
    DMA_REG_WRITEBIT(DRV_DMA_CX_CONFIG((unsigned int)uhwChannelNo),
                  DMA_BIT0,
                  DMA_BIT0,
                  0);

    /*дͨ��X��ǰһά����ʣ���Byte��,[15,0]*/
    DMA_REG_WRITEBIT(DRV_DMA_CX_CNT0((unsigned int)uhwChannelNo),
                  DMA_BIT0,
                  DMA_BIT15,
                  pstCfg->uhwACount);

    /*дͨ��X��ǰ��ά����ʣ���Array����,[31,16]*/
    DMA_REG_WRITEBIT(DRV_DMA_CX_CNT0((unsigned int)uhwChannelNo),
                  DMA_BIT16,
                  DMA_BIT31,
                  pstCfg->uhwBCount);

    /*дͨ��X��ǰ��ά����ʣ���Frame��,[15,0]*/
    DMA_REG_WRITEBIT(DRV_DMA_CX_CNT1((unsigned int)uhwChannelNo),
                  DMA_BIT0,
                  DMA_BIT15,
                  pstCfg->uhwCCount);

    /*дͨ��X�Ķ�άԴ��ַƫ����[31,16]��Ŀ�ĵ�ַƫ����[15,0]*/
    DMA_REG_WRITEBIT(DRV_DMA_CX_BINDX((unsigned int)uhwChannelNo),
                  DMA_BIT0,
                  DMA_BIT15,
                  pstCfg->uhwDstBIndex);
    DMA_REG_WRITEBIT(DRV_DMA_CX_BINDX((unsigned int)uhwChannelNo),
                  DMA_BIT16,
                  DMA_BIT31,
                  pstCfg->uhwSrcBIndex);

    /*дͨ��X����άԴ��ַƫ����[31,16]��Ŀ�ĵ�ַƫ����[15,0]*/
    DMA_REG_WRITEBIT(DRV_DMA_CX_CINDX((unsigned int)uhwChannelNo),
                  DMA_BIT0,
                  DMA_BIT15,
                  pstCfg->uhwDstCIndex);
    DMA_REG_WRITEBIT(DRV_DMA_CX_CINDX((unsigned int)uhwChannelNo),
                  DMA_BIT16,
                  DMA_BIT31,
                  pstCfg->uhwSrcCIndex);

    /*дͨ��X��Դ��ַ[31,0]��Ŀ�ĵ�ַ[31,0]*/
    DMA_REG_WRITE(DRV_DMA_CX_DES_ADDR((unsigned int)uhwChannelNo), pstCfg->uwDstAddr);
    DMA_REG_WRITE(DRV_DMA_CX_SRC_ADDR((unsigned int)uhwChannelNo), pstCfg->uwSrcAddr);

    /*дͨ��X�������ַ����*/
    DMA_REG_WRITE(DRV_DMA_CX_LLI((unsigned int)uhwChannelNo), pstCfg->uwLli);

    /*���ͨ��X�ĸ����ж�״̬*/
    DMA_REG_WRITE(DRV_DMA_INT_TC1_RAW, uwChannelMask);
    DMA_REG_WRITE(DRV_DMA_INT_TC2_RAW, uwChannelMask);
    DMA_REG_WRITE(DRV_DMA_INT_ERR1_RAW, uwChannelMask);
    DMA_REG_WRITE(DRV_DMA_INT_ERR2_RAW, uwChannelMask);
    DMA_REG_WRITE(DRV_DMA_INT_ERR3_RAW, uwChannelMask);

    /*���ص������ǿ�,�򱣴��ֵ*/
    if (NULL != pfIntHandleFunc)
    {
        pstCxIsr[uhwChannelNo].pfFunc  = pfIntHandleFunc;
        pstCxIsr[uhwChannelNo].uwPara  = uwPara;

        /*����д(uhwChannelNo��ӦbitΪ1)�����¼Ĵ�����HiFi��ӦDMAͨ���ж�����*/
        DMA_REG_WRITEBIT(DRV_DMA_INT_ERR1_MASK, uhwChannelNo, uhwChannelNo, 1);
        DMA_REG_WRITEBIT(DRV_DMA_INT_ERR2_MASK, uhwChannelNo, uhwChannelNo, 1);
        DMA_REG_WRITEBIT(DRV_DMA_INT_ERR3_MASK, uhwChannelNo, uhwChannelNo, 1);
        DMA_REG_WRITEBIT(DRV_DMA_INT_TC1_MASK, uhwChannelNo, uhwChannelNo, 1);
        DMA_REG_WRITEBIT(DRV_DMA_INT_TC2_MASK, uhwChannelNo, uhwChannelNo, 1);

    }

    /*дͨ��X������*/
    DMA_REG_WRITE(DRV_DMA_CX_CONFIG((unsigned int)uhwChannelNo), pstCfg->uwConfig);
    return BSP_OK;

}

/*****************************************************************************
 �� �� ��  : bsp_dma_stop
 ��������  : ������ϸ���õ�DMAͨ������
 �������  :

 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_dma_stop(unsigned short uhwChannelNo)
{
    DRV_DMA_CXISR_STRU                 *pstCxIsr        = DRV_DMA_GetCxIsrPtr();

    bsp_edma_channel_stop(uhwChannelNo);

    /* д0��DRV_DMA_CX_CONFIG(uhwChannelNo) bit0 ֹͣ��ӦDMAͨ�� */
    DMA_REG_WRITEBIT(DRV_DMA_CX_CONFIG((unsigned int)uhwChannelNo), DMA_BIT0, DMA_BIT0, 0);

    /*����д(uhwChannelNo��ӦbitΪ0)�����¼Ĵ�������HiFi��ӦDMAͨ���ж�*/
    DMA_REG_WRITEBIT(DRV_DMA_INT_ERR1_MASK, uhwChannelNo, uhwChannelNo, 0);
    DMA_REG_WRITEBIT(DRV_DMA_INT_ERR2_MASK, uhwChannelNo, uhwChannelNo, 0);
    DMA_REG_WRITEBIT(DRV_DMA_INT_ERR3_MASK, uhwChannelNo, uhwChannelNo, 0);
    DMA_REG_WRITEBIT(DRV_DMA_INT_TC1_MASK, uhwChannelNo, uhwChannelNo, 0);
    DMA_REG_WRITEBIT(DRV_DMA_INT_TC2_MASK, uhwChannelNo, uhwChannelNo, 0);

    /*���ȫ�ֱ����ж�Ӧͨ����������*/
    memset(&pstCxIsr[uhwChannelNo],0,sizeof(DRV_DMA_CXISR_STRU));
}

/*****************************************************************************
 �� �� ��  : bsp_dma_isr
 ��������  : DMAͨ���ж�
 �������  :

 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :
*****************************************************************************/
void bsp_dma_isr(void)
{
    unsigned int                          i;
    unsigned int                          uwErr1;
    unsigned int                          uwErr2;
    unsigned int                          uwErr3;
    unsigned int                          uwTc1;
    unsigned int                          uwTc2;
    unsigned int                          uwIntMask;
    unsigned int                          uwIntState;
    drv_dma_int_type                    enIntType;
    DRV_DMA_CXISR_STRU                 *pstDmaCxIsr = DRV_DMA_GetCxIsrPtr();

    /*��ȡHIFI CPU��DMAͨ���ж�״̬*/
    uwIntState  = DMA_REG_READ(DRV_DMA_INT_STAT);

    /*��ͨ��״̬ȫ0��ʾȫͨ�����жϻ��ж��Ѵ���,ֱ���˳�*/
    if (0 == uwIntState)
    {
        return;
    }

    /*��ȡ���жϼĴ���,��ѯ���ж�״̬,����Tc1/Tc2/Err1/Err2/Err3*/
    uwErr1      = DMA_REG_READ(DRV_DMA_INT_ERR1);
    uwErr2      = DMA_REG_READ(DRV_DMA_INT_ERR2);
    uwErr3      = DMA_REG_READ(DRV_DMA_INT_ERR3);
    uwTc1       = DMA_REG_READ(DRV_DMA_INT_TC1);
    uwTc2       = DMA_REG_READ(DRV_DMA_INT_TC2);

    /*дuwIntState���ε����Ĵ�������ж�,������β�ѯ����ͨ���ж�*/
    DMA_REG_WRITE(DRV_DMA_INT_TC1_RAW, uwIntState);
    DMA_REG_WRITE(DRV_DMA_INT_TC2_RAW, uwIntState);
    DMA_REG_WRITE(DRV_DMA_INT_ERR1_RAW, uwIntState);
    DMA_REG_WRITE(DRV_DMA_INT_ERR2_RAW, uwIntState);
    DMA_REG_WRITE(DRV_DMA_INT_ERR3_RAW, uwIntState);

    /*�������ø�ͨ��ע��Ļص�������*/
    for ( i = 0; i < 16; i++)
    {
        uwIntMask = 0x1L << i;

        /*����ͨ�����жϲ���(��Ӧ����Ϊ1)*/
        if (uwIntState & uwIntMask)
        {
            if (NULL != pstDmaCxIsr[i].pfFunc)
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

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

