
#ifdef __cplusplus
extern "C" {
#endif

#include <vxWorks.h>
#include <logLib.h>
#include <memLib.h>
#include <intLib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cacheLib.h>
#include <vxAtomicLib.h>
#include <semLib.h>
#include <taskLib.h>
#include <product_config.h>
#include <osl_bio.h>
#include <bsp_om.h>
#include "cipher_balong.h"
#include "acc_balong.h"
/*lint --e{661, 831} */
s32 bsp_acc_init(void);
void bsp_acc_debug_show(void);

struct acc_chx_mgr_s st_acc_chx_mgr = {{{0, ACC_STAT_IDLE, 0}, {0, ACC_STAT_IDLE, 0}},
	                                   {0, 0, 0, 0, 0},
	                                   ACC_FIFO_MAX_DEEP,
	                                   0,
	                                   0};


/*****************************************************************************
* �� �� ��  : bsp_acc_init
*
* ��������  : ��ʼ���������ģ��
*
* �������  : ��
* �������  : ��
*
* �� �� ֵ  : �ɹ�/ʧ��
*
* �޸ļ�¼  : 2011��1��8��   wangjing  creat
             2014��1��16��  wangxiandong  modify
*****************************************************************************/
s32 bsp_acc_init(void)
{
    u32 fifo_addr[ACC_FIFO_NUM] = {0};
    u32 size, i;


    if (TRUE == st_acc_chx_mgr.init_flag)
    {
        return OK;
    }
	st_acc_chx_mgr.acc_fifo_deep = ACC_FIFO_MAX_DEEP;

    /* ���㵥��BDFIFO�ĳߴ� */
    size = ACC_FIFO_MAX_DEEP * CIPHER_DESC_SIZE;

    /*ΪACC_FIFO_NUM��BDFIFO�����ڴ�ռ�*/
	fifo_addr[0] = (u32)CIPHER_MALLOC((size + ALIN_BYTES) * ACC_FIFO_NUM + ALIN_BYTES);
	if(0 == fifo_addr[0])
	{
		ACC_ERROR_PRINT(CIPHER_NO_MEM);
        return CIPHER_NO_MEM;
	}
	memset((void*)fifo_addr[0], 0, (size + ALIN_BYTES) * ACC_FIFO_NUM + ALIN_BYTES);
    fifo_addr[0] = ROUND_UP(fifo_addr[0], ALIN_BYTES);
	if(fifo_addr[0] & 7)
	{
		ACC_ERROR_PRINT(CIPHER_ALIGN_ERROR);
		return CIPHER_ALIGN_ERROR;
	}
	fifo_addr[0] += ALIN_BYTES;
	fifo_addr[1] = (fifo_addr[0] + size + ALIN_BYTES);
	if(fifo_addr[1] & 7)
	{
		ACC_ERROR_PRINT(CIPHER_ALIGN_ERROR);
		return CIPHER_ALIGN_ERROR;
	}
	for(i = 0; i < ACC_FIFO_NUM; i++)
	{
		writel(i, (fifo_addr[i] - sizeof(u32)));
	}

    /*��ʼ��ȫ�ֱ���acc_fifo_mgr[2] ��д�����뵽���ڴ���׵�ַ*/
    for (i = 0; i < ACC_FIFO_NUM; i++)
    {
        st_acc_chx_mgr.acc_fifo_mgr[i].fifo_stat = ACC_STAT_IDLE;
        st_acc_chx_mgr.acc_fifo_mgr[i].bd_fifo_addr   = fifo_addr[i];
        st_acc_chx_mgr.acc_fifo_mgr[i].bd_fifo_cfg_num = 0;
    }

    st_acc_chx_mgr.init_flag= TRUE;

    return CIPHER_SUCCESS;
}

/*****************************************************************************
* �� �� ��  : bsp_acc_get_bdfifo_addr
*
* ��������  : ���ڻ�ȡ��ǰ���õ�BDFIFO�׵�ַ
*
* �������  : ��
*
* �������  : ��
* �� �� ֵ  : BDFIFO�׵�ַ
*****************************************************************************/
u32 bsp_acc_get_bdfifo_addr()
{
    u32 fifo_base_addr = ACC_FIFO_ERROR;
    u32 i = 0;
	u32 reg_val = 0;
	u32 reg_fifo_addr = 0;

	if(cipher_open_clk())
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:fail to open clk\n");
		return CIPHER_UNKNOWN_ERROR;
	}
	/* ����fifo״̬ */
	reg_val = readl(CIPHER_REGBASE_ADDR + CIPHER_CHN_ENABLE(ACC_CHN_NUM));
	reg_val &= CHN_STATBIT;
	//reg_fifo_addr = readl(CIPHER_REGBASE_ADDR + CIPHER_CHNBDQ_BASE(ACC_CHN_NUM));
	reg_fifo_addr = st_acc_chx_mgr.acc_fifo_mgr[st_acc_chx_mgr.cur_used_fifo_num].bd_fifo_addr;
	for(i = 0; i < ACC_FIFO_NUM; i++)
	{
		if(st_acc_chx_mgr.acc_fifo_mgr[i].fifo_stat == ACC_STAT_WORK)
		{
			if((i == readl(reg_fifo_addr - sizeof(u32))) && ((!reg_val)))
			{
				continue;
			}
			else
			{
				st_acc_chx_mgr.acc_fifo_mgr[i].fifo_stat = ACC_STAT_IDLE;
			}
		}
	}

	/*��һ��ѭ����Ѱ��ACC_STAT_IDLE״̬FIFO*/
	for(i = 0; i < ACC_FIFO_NUM; i++)
	{
		if(st_acc_chx_mgr.acc_fifo_mgr[i].fifo_stat == ACC_STAT_IDLE)
		{
			fifo_base_addr = st_acc_chx_mgr.acc_fifo_mgr[i].bd_fifo_addr;
			goto GET_FIFO_ADDR_EXIT;
		}
	}
	/*�ڶ���ѭ����Ѱ��ACC_STAT_CFG����ACC_STAT_IDLE״̬FIFO*/
	for(i = 0; i < ACC_FIFO_NUM; i++)
	{
		if((st_acc_chx_mgr.acc_fifo_mgr[i].fifo_stat == ACC_STAT_CFG) ||
			(st_acc_chx_mgr.acc_fifo_mgr[i].fifo_stat == ACC_STAT_IDLE))
		{
			fifo_base_addr = st_acc_chx_mgr.acc_fifo_mgr[i].bd_fifo_addr;
			goto GET_FIFO_ADDR_EXIT;
		}
	}

    if (ACC_FIFO_ERROR == fifo_base_addr)
    {
		ACC_ERROR_PRINT(ACC_ALL_FIFO_WORK);
        return ACC_ALL_FIFO_WORK;
    }

GET_FIFO_ADDR_EXIT:
	st_acc_chx_mgr.acc_fifo_mgr[i].bd_fifo_cfg_num = 0; 
	st_acc_chx_mgr.acc_debug.get_fifo_times++;

    return fifo_base_addr;
}

/*****************************************************************************
* �� �� ��  : bsp_acc_dma
*
* ��������  : Ϊָ��ͨ�����������������ڵ�����DMA����
*
* �������  : bd_fifo_addr: BDFIFO�׵�ַ
*                          p_in_mem_mgr    : ����buffer���ݿ���׵�ַ
*                          p_out_mem_mgr   : ���buffer���ݿ���׵�ַ
* �������  : ��
* �� �� ֵ  : �ɹ�/ʧ��
*****************************************************************************/
s32 bsp_acc_dma(u32 bd_fifo_addr, const void* p_in_mem_mgr, const void* p_out_mem_mgr, ACC_SINGLE_CFG_S *p_cfg)
{/*lint !e18*/
    struct cipher_bd_s *p_w_addr;
    u32 fifo_num = 0;

    /*������飬�жϵ�ǰ����FIFO��FIFO1����FIFO2*/
	fifo_num =  readl(bd_fifo_addr - sizeof(u32));
	if(fifo_num >= ACC_FIFO_NUM)
	{
		ACC_ERROR_PRINT(CIPHER_INVALID_NUM);
        return CIPHER_INVALID_NUM;
	}

    /*�ж����õ�BDFIFO�ĸ����Ƿ񳬹����ֵ������ճ����һ��BD��ʹ�ã���������BDQдָ��ʱ�ᵼ�»���*/
    if (st_acc_chx_mgr.acc_fifo_mgr[fifo_num].bd_fifo_cfg_num < (st_acc_chx_mgr.acc_fifo_deep - 1))
    {
        /*����BD������*/
		p_w_addr = (struct cipher_bd_s *)(st_acc_chx_mgr.acc_fifo_mgr[fifo_num].bd_fifo_addr);/* [false alarm]:�� */
		p_w_addr += st_acc_chx_mgr.acc_fifo_mgr[fifo_num].bd_fifo_cfg_num;/* [false alarm]:�� */
        memset((void*)p_w_addr, 0, sizeof(struct cipher_bd_s));/* [false alarm]:�� */

        /*InputAttr��ֻ���������ݿ鳤�ȣ�OutputAttr���0����*/
        p_w_addr->input_attr  = (u32)p_cfg->u32BlockLen;
        p_w_addr->input_addr  = (u32)p_in_mem_mgr;
        p_w_addr->output_addr = (u32)p_out_mem_mgr;

        /*���¶�ӦFIFOȫ�ֽṹ�壬����enFifoStat��u32BdFifoCfgNum*/
        st_acc_chx_mgr.acc_fifo_mgr[fifo_num].fifo_stat = ACC_STAT_CFG;
        st_acc_chx_mgr.acc_fifo_mgr[fifo_num].bd_fifo_cfg_num += 1;

        st_acc_chx_mgr.acc_debug.cfg_dma_times++;
        return OK;
    }
    else
    {
        /*BDFIFO��������������*/
        st_acc_chx_mgr.acc_debug.drop_pak_times++;
		ACC_ERROR_PRINT(CIPHER_FIFO_FULL);
        return CIPHER_FIFO_FULL;
    }
}

/*****************************************************************************
* �� �� ��  : bsp_acc_cipher
*
* ��������  : Ϊָ��ͨ��������������������ҪCipher���м��ܵĲ���
*
* �������  : bd_fifo_addr: BDFIFO�׵�ַ
*                          p_in_mem_mgr    : ����buffer���ݿ���׵�ַ
*                          p_out_mem_mgr   : ���buffer���ݿ���׵�ַ
*                          p_cfg       : �������������Ϣ
* �������   : ��
* �� �� ֵ  : �ɹ�/ʧ��
*****************************************************************************/
s32 bsp_acc_cipher(u32 bd_fifo_addr, const void* p_in_mem_mgr, const void* p_out_mem_mgr, ACC_SINGLE_CFG_S *p_cfg)
{/*lint !e18*/
    struct cipher_bd_s *p_w_addr;
    u32 fifo_num = 0;

	fifo_num =  readl(bd_fifo_addr - sizeof(u32));
    if(fifo_num >= ACC_FIFO_NUM)
    {
		ACC_ERROR_PRINT(CIPHER_INVALID_NUM);
        return CIPHER_INVALID_NUM;
    }

    /*�ж����õ�BDFIFO�ĸ����Ƿ񳬹����ֵ������ճ����һ��BD��ʹ�ã���������BDQдָ��ʱ�ᵼ�»���*/
	if (st_acc_chx_mgr.acc_fifo_mgr[fifo_num].bd_fifo_cfg_num < (st_acc_chx_mgr.acc_fifo_deep - 1))
    {
        /*����BD������*/
		p_w_addr = (struct cipher_bd_s *)(st_acc_chx_mgr.acc_fifo_mgr[fifo_num].bd_fifo_addr);/* [false alarm]:�� */
		p_w_addr += st_acc_chx_mgr.acc_fifo_mgr[fifo_num].bd_fifo_cfg_num;/* [false alarm]:�� */
		memset(p_w_addr, 0, sizeof(struct cipher_bd_s));/* [false alarm]:�� */

        /*Cipher��������*/
        p_w_addr->cipher_cfg = ((p_cfg->stAlgKeyInfo.u32KeyIndexSec << 20) | /*���ܲ�������Ҫkey�����*/
                                (p_cfg->u32AppdHeaderLen<< 18)             | /*���Ӱ�ͷ�ĳ���*/
                                (p_cfg->u32HeaderLen<< 16)                 | /*��ͷpacker head�ĳ���*/
                                (1 << 12)                                  | /*AES�㷨�ӽ��ܲ���ģʽΪCTRģʽ*/
                                (p_cfg->stAlgKeyInfo.enAlgSecurity << 4)   | /*���ܲ�������*/
                                (1 << 1));                                   /*���м��ܲ���*/

        /*����OutputAttr,InputAttr*/
        if ((p_cfg->u32Offset != 0) || (p_cfg->u32OutLen < p_cfg->u32BlockLen))
        {
            p_w_addr->input_attr  = (1 << 19);       /*ѡȡ����д�offset��ʼ��offset��obuf_len֮�ڵĽ�����*/
            p_w_addr->output_attr = p_cfg->u32Offset << 16;
        }

        p_w_addr->input_attr |= p_cfg->u32BlockLen;
        p_w_addr->input_addr   = (u32)p_in_mem_mgr;
        p_w_addr->output_attr |= p_cfg->u32OutLen;
        p_w_addr->output_addr  = (u32)p_out_mem_mgr;

        /*����Aph*/
        p_w_addr->aph_attr = ((p_cfg->u32BearId << 27) |
                             (p_cfg->u32Aph << ((0x3 - p_cfg->u32AppdHeaderLen) << 0x3) & 0x00FFFFFF));
        p_w_addr->count = p_cfg->u32Count;

        /*���¶�ӦFIFOȫ�ֽṹ��*/
        st_acc_chx_mgr.acc_fifo_mgr[fifo_num].fifo_stat = ACC_STAT_CFG;
        st_acc_chx_mgr.acc_fifo_mgr[fifo_num].bd_fifo_cfg_num += 1;

        st_acc_chx_mgr.acc_debug.cfg_cipher_times++;

        return OK;
    }
    else
    {
        /*BDFIFO��������������*/
        st_acc_chx_mgr.acc_debug.drop_pak_times++;
		ACC_ERROR_PRINT(CIPHER_FIFO_FULL);
        return CIPHER_FIFO_FULL;
    }
}

/*****************************************************************************
* �� �� ��  : bsp_acc_get_status
*
* ��������  : ��ȡ��ǰͨ��״̬
*
* �������  : ��
* �������  : ��
* �� �� ֵ  : ͨ��æ/����
*****************************************************************************/
s32 bsp_acc_get_status()
{
    u32 chx_stat = 0;

	if(cipher_open_clk())
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:fail to open clk\n");
		return CIPHER_UNKNOWN_ERROR;
	}
	chx_stat = readl(CIPHER_REGBASE_ADDR + CIPHER_CHN_ENABLE(ACC_CHN_NUM));
    chx_stat = chx_stat & CHN_STATBIT;
	chx_stat = chx_stat >> 30;
	return (s32)chx_stat;
}

/*****************************************************************************
* �� �� ��  : bsp_acc_enable
*
* ��������  : ʹ���������
*
* �������  : bd_fifo_addr : BDFIFO�׵�ַ
*
* �������  : ��
* �� �� ֵ  : �ɹ�/ʧ��
*****************************************************************************/
s32 bsp_acc_enable(u32 bd_fifo_addr)
{
    u32 chx_stat = 0;
    u32 ptr = 0;
    u32 fifo_num = 0;
    u32 ret;
    u32 last_bd;

	if(cipher_open_clk())
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:fail to open clk\n");
		return CIPHER_UNKNOWN_ERROR;
	}
    /*��ֹ�������CHX_EN�Ĵ����ĵ�30λд0*/
	ret = readl(CIPHER_REGBASE_ADDR + CIPHER_CHN_ENABLE(ACC_CHN_NUM));
	writel(ret & CHN_PACK_ENBITS, CIPHER_REGBASE_ADDR + CIPHER_CHN_ENABLE(ACC_CHN_NUM));

    /*ȥʹ�ܵ�ǰͨ��*/
	chx_stat = readl(CIPHER_REGBASE_ADDR + CIPHER_CHN_ENABLE(ACC_CHN_NUM));
	writel(chx_stat & CHN_DENBITS, CIPHER_REGBASE_ADDR + CIPHER_CHN_ENABLE(ACC_CHN_NUM));

    /*����ͨ��0���üĴ���*/
	chx_stat = readl(CIPHER_REGBASE_ADDR + CIPHER_CHN_CONFIG(ACC_CHN_NUM));
	writel(chx_stat | CHN_CFGUSR, CIPHER_REGBASE_ADDR + CIPHER_CHN_CONFIG(ACC_CHN_NUM));

    /*�������BD���У���λ��ָ��*/
	ptr = readl(CIPHER_REGBASE_ADDR + CIPHER_CHN_RESET(ACC_CHN_NUM));
	writel(ptr | CHN_BDRESET, CIPHER_REGBASE_ADDR + CIPHER_CHN_RESET(ACC_CHN_NUM));

    /*��BDFIFO�׵�ַд��Ĵ���*/
	writel(bd_fifo_addr, CIPHER_REGBASE_ADDR + CIPHER_CHNBDQ_BASE(ACC_CHN_NUM));

    /*��ȡ��ǰ����FIFO���*/
	fifo_num = readl(bd_fifo_addr - sizeof(u32));
	if(fifo_num >= ACC_FIFO_NUM)
	{
		ACC_ERROR_PRINT(CIPHER_INVALID_NUM);
        return CIPHER_INVALID_NUM;
	}

    /*����ȫ�ֽṹ��*/
    st_acc_chx_mgr.acc_fifo_mgr[fifo_num].fifo_stat = ACC_STAT_WORK;
	st_acc_chx_mgr.cur_used_fifo_num = fifo_num;

    /*�������һ��bdΪgen_Int_en ������Ӳ���Ϳ���ͨ���жϽ�CHX_EN�Ĵ����ĵ�30�Զ��÷�*/
    last_bd = bd_fifo_addr + (st_acc_chx_mgr.acc_fifo_mgr[fifo_num].bd_fifo_cfg_num - 1) * sizeof(struct cipher_bd_s);
	ptr = readl(last_bd + 0);
	writel(ptr | (1U << 13), last_bd + 0);
	//cacheFlush(DATA_CACHE,(void*)bd_fifo_addr,st_acc_chx_mgr.acc_fifo_mgr[fifo_num].bd_fifo_cfg_num * sizeof(struct cipher_bd_s));

    /*����дָ��*/
	ptr = readl(CIPHER_REGBASE_ADDR + CIPHER_CHNBDQ_RWPTR(ACC_CHN_NUM));
    ptr = ptr & CHN_WPT;
    ptr = ptr | st_acc_chx_mgr.acc_fifo_mgr[fifo_num].bd_fifo_cfg_num;
	writel(ptr, CIPHER_REGBASE_ADDR + CIPHER_CHNBDQ_RWPTR(ACC_CHN_NUM));

    /*��ͨ����־λ��Ϊ0����ʾͨ��æ��ʹ���������*/
	chx_stat = readl(CIPHER_REGBASE_ADDR + CIPHER_CHN_ENABLE(ACC_CHN_NUM));
	writel(chx_stat | CHN_ENBITS, CIPHER_REGBASE_ADDR + CIPHER_CHN_ENABLE(ACC_CHN_NUM));

    st_acc_chx_mgr.acc_debug.enable_times++;
    return CIPHER_SUCCESS;
}

void bsp_acc_debug_show(void)
{
    printf("\rGetFifoAddrTimes:   %d\n", st_acc_chx_mgr.acc_debug.get_fifo_times);
    printf("\rCfgDmaTimes     :   %d\n", st_acc_chx_mgr.acc_debug.cfg_dma_times);
    printf("\rCfgCipherTimes  :   %d\n", st_acc_chx_mgr.acc_debug.cfg_cipher_times);
    printf("\rEnableTimes     :   %d\n", st_acc_chx_mgr.acc_debug.enable_times);
    printf("\rDropPakTimes    :   %d\n", st_acc_chx_mgr.acc_debug.drop_pak_times);

    return;
}

#ifdef __cplusplus
}
#endif
