

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
#include <osl_bio.h>
#include <osl_types.h>
#include <bsp_om.h>
#include <bsp_clk.h>
#include <bsp_lowpower_mntn.h>
#include <drv_dpm.h>
#ifdef CONFIG_MODULE_VIC
#include "bsp_vic.h"
#endif
#ifdef CONFIG_CCORE_PM
#include "bsp_dpm.h"
#endif
#include "cipher_balong.h"
#include "acc_balong.h"

/*lint --e{737, 958} */
//declare functions
s32 cipher_init(void);
s32 is_kdf_bdq_full(void);
void cipher_move_kdf_chx_bdq_ptr(void);
void cipher_move_kdf_chx_rd_reg_ptr(void);
s32 cipher_kdf_bd_config(struct kdf_bdconfig_info_s *key_cfg_info);
s32 cipher_start_kdf_channel(void);


struct cipher_mdl_mgr_s sg_module_mgr = {CIPHER_REGBASE_ADDR,
										CIPHER_STAT_NULL,
										0,
										{{0}, {0}, {0}, {0}},
										{0},
										{CIPHER_KEY_LEN_BUTTOM},
										0,
										{0},
										{0},
										{0},
										{0},
										{0},
										{0},
										0,
										0,
										{0x10, 0x18, 0x20},
										{0x2, 0x3, 0x4},
										{1, 1, 0, 0, 0, 0, 0, 0},
										{0, 0, 1, 1, 1, 1, 1, 1},
										{0, 1, 0, 0, 1, 1, 1, 1},
										{1, 1, 0, 0, 0, 0, 0, 0},
										{0, 0, 1, 1, 1, 1, 1, 1},
										{0, 0, 0, 0, 1, 0, 1, 0},
										{0, 0, 1, 0, 1, 1, 0, 0},
										{0, 1, 0, 1},
										{1, 1, 1, 1},
										{1, 1, 1, 1},
										{1, 0, 0 ,1},
										{0, 0, 1, 1},
										{0, 1, 2, 3, 4, 5},
										{0, 1, 2, 3, 4, 5},
										{0, 0, 1},
										{0, 1, 1, 2, 1, 1, 2},
										{0, 1, 1, 2, 1, 1, 2},
										{0, 1, 1, 1, 0, 0, 0},
										{0, 0x1f, 0x7f, 0xfff, 0x1f, 0x7f, 0xfff},
										NULL,
										{0},
										0,
										0,
										{0},
										{0}};
#ifdef CONFIG_CCORE_PM
struct dpm_device cipher_device =
{
	.device_name = "balong cipher driver",
	.suspend = bsp_cipher_suspend,
	.resume = bsp_cipher_resume,
	.prepare = NULL,
	.suspend_late = NULL,
	.complete = NULL,
	.resume_early = NULL,
};
#endif
static s32 cipher_getrd_readable_depth(u32 chx)
{
	s32 ret_val = 0x0;
	u32 reg_val_1, reg_val_2;

	reg_val_1 = readl(sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_RWPTR(chx));
	reg_val_2 = reg_val_1 & 0x3FF;
	reg_val_1 &= 0x3FF0000;
	reg_val_1 = reg_val_1 >> 0x10;
	ret_val = (s32)reg_val_1 - (s32)reg_val_2;
	reg_val_2 = readl(sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_SIZE(chx));
	ret_val = ((s32)reg_val_2 + 0x1 - ret_val) % ((s32)reg_val_2 + 0x1);/*ʵ�ʵ�RDQ������CHx_RDQ_SIZE�Ĵ�����ֵ��0x1*/

	return ret_val;
}

static void cipher_move_chx_bdq_ptr(u32 chx)
{
    u32 reg_val_1, reg_val_2;

    /* ��ȡBDQ��дָ��Ĵ���*/
	reg_val_1 = readl(sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_RWPTR(chx));
    /* ��ȡBD����дָ��*/
    reg_val_1 &= 0x3FF;
	reg_val_2 = readl(sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_SIZE(chx));
    /* �޸�дָ���ֵ*/
	writel((reg_val_1 + 0x1) % (reg_val_2 + 0x1), sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_RWPTR(chx));
}

static void cipher_move_chx_rd_regptr(u32 chx)
{
	u32 reg_val_rdq_size, reg_val_rdq_ptr;
	u32 rdq_wptr, rdq_rptr;

	reg_val_rdq_ptr = readl(sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_RWPTR(chx));
	rdq_rptr = reg_val_rdq_ptr & 0x3FF0000;
	rdq_rptr = rdq_rptr >> 0x10;
	rdq_wptr = reg_val_rdq_ptr & 0x3FF;
	if(rdq_rptr == rdq_wptr)
	{
		CIPHER_ERROR_PRINT(CIPHER_RDQ_NULL);
		return;
	}
	reg_val_rdq_size = readl(sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_SIZE(chx));
	rdq_rptr = (rdq_rptr + 0x1) % (reg_val_rdq_size + 0x1);
	rdq_rptr = rdq_rptr << 0x10;
	writel(rdq_rptr, sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_RWPTR(chx));
}

static void cipher_chn_mag_reset(u32 chx)
{
	writel(0, sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_RWPTR(chx));
	writel(0, sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_RWPTR(chx));
}

/*****************************************************************************
* �� �� ��  : cipher_isr
*
* ��������  : cipher�жϴ�����
*
* �������  : void
* �������  : ��
*
* �� �� ֵ  : ��
*
* ����˵��  : ��
*
*****************************************************************************/
static void cipher_isr(u32 param)
{
    u32 chx;
    u32 status;
    s32 i;
    s32 rd_number;
    struct cipher_rd_s *p_cur_rd_addr = NULL;

    /* ��ȡ�ж�״̬ */
	status = readl(sg_module_mgr.reg_base_addr + CIPHER_INT0_MSSTAT_OFFSET);
    /* ���ж� */
	writel(status, sg_module_mgr.reg_base_addr + CIPHER_INT0_MSSTAT_OFFSET);
    for(chx = CIPHER_MIN_CHN; chx < CIPHER_MAX_CHN; chx++)
    {
        /* ���㵱ǰû�д����RD����*/
        rd_number = cipher_getrd_readable_depth(chx);
        p_cur_rd_addr = (struct cipher_rd_s *)readl(sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_RDPTR(chx));
        //cacheInvalidate(DATA_CACHE, (void*)p_cur_rd_addr,sizeof(struct cipher_rd_s));

        /* ͨ���Ƿ������ж�*/
	    /*���û�з����жϣ���RD�Ƿ�ʹ�����ж�ģʽ*/
        if(CIPHER_INTR_CHN_COMPL(chx, status) || (p_cur_rd_addr->cipher_cfg & 0x2000))
        {
            /* ѭ�����ûص�����֪ͨ�ϲ������ɣ�ֱ������δ�����RDȫ���������*/
            for(i = 0; i < rd_number; i++)
            {
            	p_cur_rd_addr = (struct cipher_rd_s *)readl(sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_RDPTR(chx));
                /* ���ûص�����*/
                sg_module_mgr.p_func_notify(
                     chx,
                     p_cur_rd_addr->usr_field1,
                     p_cur_rd_addr->usr_field2,
                     (CIPHER_NOTIFY_STAT_E)((p_cur_rd_addr->cipher_cfg & 0x60000000) >> 29),
                     p_cur_rd_addr->usr_field3
                     );

                /* ���RD��Чλ*/
                p_cur_rd_addr->cipher_cfg &= (~0x80000000);
                /* �ƶ��Ĵ����е�RD��ַ*/
                cipher_move_chx_rd_regptr(chx);
            }
        }
    }
}

/*****************************************************************************
* �� �� ��  : cipher_init_chx_bdq_mem
*
* ��������  : BDQ�ռ䴴��
*
* �������  : u32 chx
* �������  : ��
*
* �� �� ֵ  : �����ռ��Ƿ�ɹ�
*
* ����˵��  : ��
*
*****************************************************************************/
static s32 cipher_init_chx_bdq_mem(u32 chx, u32 * org_bdq_adrr)
{
    u32 size;
    void *p_malloc_addr;

    if (NULL != sg_module_mgr.chx_mgr[chx].p_bd_base)
    {
		CIPHER_ERROR_PRINT(CIPHER_UNKNOWN_ERROR);
        return CIPHER_UNKNOWN_ERROR;
    }

    /*ͨ��BDQ�ռ��ȡ*/
    size = sizeof(struct cipher_bd_s) * (sg_module_mgr.chx_mgr[chx].chx_bd_number);
    p_malloc_addr = (void *)CIPHER_MALLOC(size + 8);
    if(NULL == p_malloc_addr)
    {
		CIPHER_ERROR_PRINT(CIPHER_NO_MEM);
        return CIPHER_NO_MEM;
    }
	*org_bdq_adrr = (u32)p_malloc_addr;
	p_malloc_addr = (void *)ROUND_UP((unsigned int)p_malloc_addr, 8);
	if((u32)p_malloc_addr & 7)
	{
		CIPHER_ERROR_PRINT(CIPHER_ALIGN_ERROR);
		return CIPHER_ALIGN_ERROR;
	}
    sg_module_mgr.chx_mgr[chx].p_bd_base = (struct cipher_bd_s*)p_malloc_addr;
    memset((void *)sg_module_mgr.chx_mgr[chx].p_bd_base, 0, size);

    return CIPHER_SUCCESS;
}

/*****************************************************************************
* �� �� ��  : cipher_init_chx_cdlist_mem
*
* ��������  : CD����ռ䴴��
*
* �������  : u32 chx
* �������  : ��
*
* �� �� ֵ  : �����ռ��Ƿ�ɹ�
*
* ����˵��  : ��
*
*****************************************************************************/
static s32 cipher_init_chx_cdlist_mem(u32 chx, u32 * org_cdlist_adrr)
{
    u32   size;
	u32   mask_len;
    struct cipher_cd_s **pp_buflist_base = NULL;

    pp_buflist_base = &sg_module_mgr.chx_mgr[chx].p_cdlist_base;

    if(NULL != *pp_buflist_base)
    {
		CIPHER_ERROR_PRINT(CIPHER_UNKNOWN_ERROR);
        return CIPHER_UNKNOWN_ERROR;
    }

    /*CD List �ռ�����*/
	mask_len = sg_module_mgr.chx_mgr[chx].chx_bd_number * sg_module_mgr.chx_mgr[chx].cd_number_per_bd;
    size = sizeof(struct cipher_cd_s) * mask_len;
	*pp_buflist_base = (struct cipher_cd_s *)CIPHER_MALLOC(size + 8);
    if(!(*pp_buflist_base))
    {
		CIPHER_ERROR_PRINT(CIPHER_NO_MEM);
        return CIPHER_NO_MEM;
    }
	*org_cdlist_adrr = (u32)(*pp_buflist_base);
	*pp_buflist_base = (struct cipher_cd_s *)ROUND_UP((unsigned int)(*pp_buflist_base), 8);
	if((u32)(*pp_buflist_base) & 7)
	{
		CIPHER_ERROR_PRINT(CIPHER_ALIGN_ERROR);
		return CIPHER_ALIGN_ERROR;
	}
    memset((void*)(*pp_buflist_base), 0, size);

    sg_module_mgr.chx_mgr[chx].cd_list_start_addr =  (u32)(*pp_buflist_base);
    sg_module_mgr.chx_mgr[chx].cd_list_end_addr =  ((u32)((u8*)(*pp_buflist_base) + size));


    return CIPHER_SUCCESS;
}

/*****************************************************************************
* �� �� ��  : cipher_init_chx_rdq_mem
*
* ��������  : RDQ����ռ䴴��
*
* �������  : u32 chx
* �������  : ��
*
* �� �� ֵ  : �����ռ��Ƿ�ɹ�
*
* ����˵��  : ��
*
*****************************************************************************/
static s32 cipher_init_chx_rdq_mem(u32 chx, u32 * org_rdq_addr)
{
    u32  size;
    void *p_malloc_addr;

    if (NULL != sg_module_mgr.chx_mgr[chx].p_rd_base)
    {
		CIPHER_ERROR_PRINT(CIPHER_UNKNOWN_ERROR);
        return CIPHER_UNKNOWN_ERROR;
    }

    /*ͨ��RDQ�ռ��ȡ*/
    size = sizeof(struct cipher_rd_s) * (sg_module_mgr.chx_mgr[chx].chx_bd_number/*RD������BD��ͬ*/);
    p_malloc_addr = CIPHER_MALLOC(size + 8);
    if(NULL == p_malloc_addr)
    {
		CIPHER_ERROR_PRINT(CIPHER_NO_MEM);
        return CIPHER_NO_MEM;
    }
	*org_rdq_addr = (u32)p_malloc_addr;
	p_malloc_addr = (void *)ROUND_UP((unsigned int)p_malloc_addr, 8);
	if((u32)p_malloc_addr & 7)
	{
		CIPHER_ERROR_PRINT(CIPHER_ALIGN_ERROR);
		return CIPHER_ALIGN_ERROR;
	}
    sg_module_mgr.chx_mgr[chx].p_rd_base = (struct cipher_rd_s*)p_malloc_addr;
    memset((void *)sg_module_mgr.chx_mgr[chx].p_rd_base, 0, size);

    return CIPHER_SUCCESS;
}

/*****************************************************************************
* �� �� ��  : cipher_cfg_cdlist
*
* ��������  : CD List ����
*
* �������  : chx
* �������  : ��
*
* �� �� ֵ  : ��
*
* ����˵��  : ��
*
*****************************************************************************/
static s32 cipher_cfg_cdlist(u32 chx, void *p_in_mem_mgr, struct cipher_cd_s *p_cur_cd_list,
                                             u32 *p_total_sz, u32 cd_number)
{
	u32 i;
	u8* p_in_buf_ptr = NULL;
	u32 buf_len = 0;
	u8* p_next = (u8*)p_in_mem_mgr;

	*p_total_sz = 0x0;

	/* �������ݸ�ʽΪ����*/
    for (i = 0x0; (i < cd_number) && p_next; i++)
    {
        /* ��ȡ�����bufferָ�� */
        p_in_buf_ptr = CIPHER_BUFLIST_GET_INBUF_PTR(p_next, chx);

        /* ��ȡ�����buffer���� */
        buf_len = CIPHER_BUFLIST_GET_INBUF_LEN(p_next, chx);

        /* ���������ܳ���*/
        *p_total_sz += buf_len;

		/* ����CD��������������ָ�븳ֵ�������������ԣ��ڵ�Ϊ�м�ڵ� */
		CIPHER_CFG_BUF_LIST(p_cur_cd_list[i], p_in_buf_ptr, buf_len, 0x0);

        /* ��ȡ��һ�ڵ� */
        p_next = CIPHER_BUFLIST_GET_INBUFNEXT_PTR(p_next, chx);
    }
	if (NULL == p_next)
	{
		/* ����CD��������������ָ�븳ֵ�������������ԣ��޸Ľڵ�Ϊ���һ���ڵ� */
		CIPHER_CFG_BUF_LIST(p_cur_cd_list[i - 0x1], p_in_buf_ptr, buf_len, 0x1);
		return (s32)i;/*���ʹ���·���������ֵҪ���ģ��������������ѭ��?????????????*/
		/*return 0;*/
	}/*
	else
		return -1;*/

    /* ����������˷�֧����ʾCDList��ʼ��ʱ��ʮ����Ա������*/
    for(; i < CIPHER_CHECK_CDLIST_CNT; i++)
    {
        p_next = CIPHER_BUFLIST_GET_INBUFNEXT_PTR(p_next, chx);
        if (NULL == p_next)
        {
            return (s32)(i + 0x1);
        }
    }

	CIPHER_ERROR_PRINT(CIPHER_INVALID_NUM);
    return CIPHER_INVALID_NUM;
}

/*****************************************************************************
* �� �� ��  : cipher_cfg_script
*
* ��������  : Cipher Config������
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  : ��
*
*****************************************************************************/
static s32 cipher_cfg_script(u32 chx, const void* p_in_mem_mgr, const void* p_out_mem_mgr,
                     void *cfg_info, s8 signle_or_rela, CIPHER_SUBMIT_TYPE_E subm_attr, u32 Private)
{
	s32 ret_val = CIPHER_SUCCESS;
	struct cipher_bd_s *p_w_addr = NULL;
	u32 aph_count;

	CIHPER_SINGLE_CFG_S * p_cfg = (CIHPER_SINGLE_CFG_S *)cfg_info;

	/* д�뵽APH���APH����*/
    aph_count = ((p_cfg->u32Count& sg_module_mgr.aph_mast_tbl[p_cfg->enAppdHeaderLen]) <<
           ((u32)(0x3 - (sg_module_mgr.aph_len_tbl[p_cfg->enAppdHeaderLen])) << 0x3)) & (0xffffff);

    /* ִ�в�����BD��ַ*/
	p_w_addr = (struct cipher_bd_s *)readl(sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_WRPTR(chx));
	if((u32)p_w_addr & 7)
	{
		CIPHER_ERROR_PRINT(CIPHER_ALIGN_ERROR);
		return CIPHER_ALIGN_ERROR;
	}

    /* ����Cipher Config�ֶι�������*/
    p_w_addr->cipher_cfg = (0x1 << 0x1c)                 |
                           p_cfg->stAlgKeyInfo.u32KeyIndexInt<<  0x18   |
                           p_cfg->stAlgKeyInfo.u32KeyIndexSec<< 0x14   |
                           sg_module_mgr.aph_len_tbl[p_cfg->enAppdHeaderLen] << 0x12 |
                           sg_module_mgr.herder_len_tbl[p_cfg->enHeaderLen] << 0x10 |
                           (sg_module_mgr.int_enable_tbl[subm_attr] << 0xd) |
                           (CIPHER_CONFIG_AEC_CTR << 0xa)        |
                           (sg_module_mgr.inte_alg[p_cfg->stAlgKeyInfo.enAlgIntegrity] << 0x7) |
                           (sg_module_mgr.secu_alg[p_cfg->stAlgKeyInfo.enAlgSecurity] << 0x4);
	if(0 > signle_or_rela)/*������*/
	{
		p_w_addr->cipher_cfg |= (sg_module_mgr.mac_length_single[p_cfg->enOpt] << 0xf)    	|
								(sg_module_mgr.mac_position_single[p_cfg->enOpt] << 0xe)  	|
								(sg_module_mgr.sig_op_integrity_tbl[p_cfg->enOpt] << 0x2)	|
								(sg_module_mgr.sig_op_security_tbl[p_cfg->enOpt] << 0x1)	|
								(sg_module_mgr.single_dirt_tbl[p_cfg->enOpt]);
	}
	else/*��������*/
	{
		p_w_addr->cipher_cfg |= (CIPHER_CONFIG_MAC_LEN << 0xf)								|
								(sg_module_mgr.mac_position_rela[p_cfg->enOpt] << 0xe )		|
								(sg_module_mgr.rela_op_first[p_cfg->enOpt] << 0x3)			|
								(sg_module_mgr.rela_op_integrity_tbl[p_cfg->enOpt] << 0x2)	|
								(sg_module_mgr.rela_op_security_tbl[p_cfg->enOpt] << 0x1)	|
								(sg_module_mgr.rela_dirt_tbl[p_cfg->enOpt]);
	}

    /* usrField������*/
    p_w_addr->usr_field1 = (u32)p_in_mem_mgr;
    p_w_addr->usr_field2 = (u32)p_out_mem_mgr;
    p_w_addr->usr_field3 = (u32)Private;

    /* APH����*/
    p_w_addr->aph_attr = (p_cfg->u8BearId<< 27) | /*lint !e701 */
                         (p_cfg->u8Direction<< 26) | /*lint !e701 */
                         (sg_module_mgr.aph_attr_tbl[p_cfg->enAppdHeaderLen] << 24) | /*lint !e701 */
                         aph_count;

    /* Count����*/
    p_w_addr->count = p_cfg->u32Count;

    //cacheFlush(DATA_CACHE,(void*)p_w_addr,sizeof(struct cipher_bd_s));

	return ret_val;
}

/*****************************************************************************
* �� �� ��  : cipher_prepare_start_chn
*
* ��������  : ��������ʱCipher Config������
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  : ��
*
*****************************************************************************/
static s32 cipher_prepare_start_chn(u32 chx, const void *p_in_mem_mgr, const void *p_out_mem_mgr,
                                       int b_mem_block, u32 block_len)
{
        s32 ret = 0;
        u32 total_buf_size = 0;
        u8 *p_out_buf_ptr = NULL;
        struct cipher_bd_s *p_w_addr = NULL;
        struct cipher_chx_mgr_s *p_chx_mgr = NULL;
        struct cipher_cd_s *p_cur_cd_list = NULL;
        u8* p_cur_out_node = (u8 *)p_out_mem_mgr;
		static struct cipher_cd_s * exral_buff = 0;/*��P1��*/
		u32 reg_val;
#if 0
		u32 i;
		u8 * next_ptr = NULL;
		struct cipher_bd_s * temp_bd_addr = NULL;
		struct cipher_cd_s * temp_cd_addr_w = NULL;
		struct cipher_cd_s * temp_cd_addr_r = NULL;
		u32 in_block_num;
#endif
        p_chx_mgr = &sg_module_mgr.chx_mgr[chx];
		/* ����Purge,ֱ�ӷ��ش��� */
        if (p_chx_mgr->b_purging)
        {
			CIPHER_ERROR_PRINT(CIPHER_PURGING);
            return CIPHER_PURGING;
        }

        /* �ռ䲻��, ���ش���*/
		reg_val = readl(sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_RWPTR(chx));
		reg_val &= (0x1UL << 31);
        if(reg_val)
        {
            sg_module_mgr.bd_full_count[chx]++;
			//CIPHER_ERROR_PRINT(CIPHER_FIFO_FULL);
            return CIPHER_FIFO_FULL;
        }

		/* ��ǰҪ������BD�ĵ�ַ*/
        p_w_addr = (struct cipher_bd_s *)readl(sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_WRPTR(chx));
        /* BD ����ָ��������������ز�����䣬��BD��CD��ϵ���� */
        /* �����ǵ���*/
        if(b_mem_block)
        {
            p_w_addr->input_addr = (u32)(p_in_mem_mgr);
            p_w_addr->input_attr = (u32)((0x0 << 16) | block_len);
			p_w_addr->output_addr = (u32)(p_out_mem_mgr);
        }
        else
        {
        #if 0/*Ҫ�ĳ�0��ֻ��Ϊ�˺�д�������1�����֮�����ʹ��*/
			/*ͳ�ƴ������ݵĿ���*/
			next_ptr = (u8 *)p_in_mem_mgr;
			for(i = 0; next_ptr; i++)
			{
				next_ptr = CIPHER_BUFLIST_GET_INBUFNEXT_PTR(next_ptr, chx);
			}
			in_block_num = i;
			/*�ж�BDQ�շ�*/
			reg_val = readl(sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_RWPTR(chx));
			reg_val &= (0x1 << 30);
			if(reg_val)/*bdq is empty*/
			{
				p_cur_cd_list = p_chx_mgr->p_cdlist_base;
			}
			else
			{
				/*��ȡ�������BD��CDLIST�׵�ַ�ͳ���*/
				reg_val = readl(sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_WRPTR(chx));
				temp_bd_addr = (struct cipher_bd_s *)reg_val - 0x1;
				temp_bd_addr = temp_bd_addr < p_chx_mgr->p_bd_base ? p_chx_mgr->p_bd_base +
					p_chx_mgr->chx_bd_number - 0x1 : temp_bd_addr;
				i = 0x1;
				while(!(temp_bd_addr->input_attr & (0x1 << 16)))
				{/*���˵���һ��ʹ��CD���BD������*/
					i++;
					if(temp_bd_addr == p_chx_mgr->p_bd_base)
					{
						temp_bd_addr = p_chx_mgr->p_bd_base + p_chx_mgr->chx_bd_number - 0x1;
						continue;
					}
					if(i > p_chx_mgr->chx_bd_number)
					{
						break;
					}
					temp_bd_addr--;
				}
				if(i > p_chx_mgr->chx_bd_number)
				{/*BDQ�е�BD��û��ʹ��CD��*/
					p_cur_cd_list = p_chx_mgr->cd_list_start_addr;
				}
				else
				{
					temp_cd_addr_w = (struct cipher_cd_s *)(temp_bd_addr->input_addr);
					for(i = 0x1; !(temp_cd_addr_w->pt_attr & (0x1 << 16)); i++, temp_cd_addr_w++);
					if(i >= p_chx_mgr->chx_bd_number * p_chx_mgr->cd_number_per_bd)
					{
						return CIPHER_NO_MEM;/*�Ѿ�û��CD�����*/
					}/*���ˣ��ɹ���ȡ���������BD��CDLIST���׵�ַ�ͳ���*/

					++temp_cd_addr_w;
					/*�õ��˵�һ�����õ�CD���ַ*/
					temp_cd_addr_w = temp_cd_addr_w >= p_chx_mgr->cd_list_end_addr ?
						p_chx_mgr->cd_list_start_addr : temp_cd_addr_w;


					/*�ҵ����һ������CD��*/
					reg_val = readl(sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_RDPTR(chx));
					temp_bd_addr = (struct cipher_bd_s *)reg_val;
					while(!(temp_bd_addr->input_attr & (0x1 << 16)))
					{/*���ѭ��һ�����ҵ�һ��ʹ��CD���BD������*/
						if(temp_bd_addr >= p_chx_mgr->p_bd_base + p_chx_mgr->chx_bd_number)
						{
							temp_bd_addr = p_chx_mgr->p_bd_base;
						}
					}
					temp_cd_addr_r = (struct cipher_cd_s *)temp_bd_addr->input_addr;
					if(temp_cd_addr_r == temp_cd_addr_w)
					{
						return CIPHER_NO_MEM;/*�Ѿ�û��CD�����*/
					}
					if((temp_cd_addr_w > temp_cd_addr_r) &&
						((p_chx_mgr->cd_list_end_addr - temp_cd_addr_w) >= in_block_num))
					{
						p_cur_cd_list = temp_cd_addr_w;
					}
					else if((temp_cd_addr_r - p_chx_mgr->cd_list_start_addr) >= in_block_num)
					{
						p_cur_cd_list = p_chx_mgr->cd_list_start_addr;
					}
					else
					{/*�п���CD�飬��������CD����������*/
						return CIPHER_NO_MEM;
					}
				}
			}
			/*-------------------------*/
			ret = cipher_cfg_cdlist(chx, (void*)p_in_mem_mgr, p_cur_cd_list, &total_buf_size,
				p_chx_mgr->chx_bd_number * p_chx_mgr->cd_number_per_bd);
			if(ret)
			{
				CIPHER_PRINT_ERR(ret);
				return ret;
			}
		#else
            /*CD List�������*/
            /* BD��Ӧ��CD�׵�ַ*/
            p_cur_cd_list = (struct cipher_cd_s *)((u32)p_chx_mgr->p_cdlist_base
                     + ((u32)(p_w_addr - p_chx_mgr->p_bd_base)) * (sizeof(struct cipher_cd_s) * p_chx_mgr->cd_number_per_bd));

            ret = cipher_cfg_cdlist(chx, (void*)p_in_mem_mgr, p_cur_cd_list, &total_buf_size, p_chx_mgr->cd_number_per_bd);
            if(ret < 0)
            {
                CIPHER_PRINT_ERR(ret);
                return ret;
            }
            else
            {/*����Э��ջȷ�ϣ�CD����������ٷ���ռ���ܻ�Ӱ��Ч�ʣ�������������ֱ�ӷ��ش��󣬵�Ԥ��
              *������ٸ�CD��Ҫ�ٶ�
              */
                if((u32)ret > p_chx_mgr->cd_number_per_bd)
                {
                	if(exral_buff)
                	{
						CIPHER_FREE(exral_buff);
						exral_buff = NULL;
                	}
					exral_buff = (struct cipher_cd_s *)CIPHER_MALLOC(sizeof(struct cipher_cd_s) * (u32)(ret + 1));
					p_cur_cd_list = exral_buff;

                    if (NULL == p_cur_cd_list)
                    {
						CIPHER_ERROR_PRINT(CIPHER_NO_MEM);
                        return CIPHER_NO_MEM;
                    }
					p_cur_cd_list = (struct cipher_cd_s *)ROUND_UP((unsigned int)p_cur_cd_list, 8);

                    /* ��������CDList*/
                    ret = cipher_cfg_cdlist(chx, (void*)p_in_mem_mgr, p_cur_cd_list, &total_buf_size, (u32)ret);
                    if(ret < 0)
                    {
                        CIPHER_PRINT_ERR(ret);
                        return ret;
                    }
                    //cacheFlush(DATA_CACHE,(void*)p_cur_cd_list,sizeof(struct cipher_cd_s)*ret);
                }
                else
                {
                	//cacheFlush(DATA_CACHE,(void*)p_cur_cd_list,sizeof(struct cipher_cd_s)*ret);
                }
            }
            #endif
            p_w_addr->input_addr = (u32)(p_cur_cd_list);
			if((p_w_addr->input_addr) & 7)
			{
				CIPHER_ERROR_PRINT(CIPHER_ALIGN_ERROR);
				return CIPHER_ALIGN_ERROR;
			}
            p_w_addr->input_attr = (u32)((0x1 << 16) |  (total_buf_size));/*����Ϊ����ģʽ*/
        }

        /* ��ʼ�������Buffer���,������ǵ����ݿ�*/
		if(0 == b_mem_block)
        {
            /* ���ָ������ */
			/*p_out_buf_ptr = CIPHER_BUFLIST_GET_OUTBUF_PTR(p_cur_out_node, chx);*//*�ĳ��±�����������*/
			if(0 != p_cur_out_node)/*�����жϣ�������*/
			{
            	p_out_buf_ptr = CIPHER_BUFLIST_GET_OUTBUF_PTR(p_cur_out_node, chx);
			}
			else
			{
				p_out_buf_ptr = 0;
			}

            p_w_addr->output_addr = (u32)(p_out_buf_ptr);
        }

        /* ���������е�segmen_en��optr_nvalid��out_chain������Ϊ0b
         * �������������е�segmen_en��Ϊ0������OutputAttr����������
         */
        p_w_addr->input_attr &= (~0xe0000);

        return CIPHER_SUCCESS;
}

static s32 cipher_start_channel(u32 chx, CIPHER_SUBMIT_TYPE_E subm_attr)
{
    u32 reg_value;
    struct cipher_rd_s * cur_rd_addr = NULL;
    s32 ret = CIPHER_SUCCESS;
    s32 loops = CIPHER_TIMEOUT_TICKS + CIPHER_WAIT_THREHLD*2;
    s32 loop_wait_cnt = CIPHER_WAIT_THREHLD;

    /* ʹ��Cipher����ʼ����*/
	writel(0x1, sg_module_mgr.reg_base_addr + CIPHER_CHN_ENABLE(chx));

    /* �ƶ��Ĵ���дָ�� */
    cipher_move_chx_bdq_ptr(chx);

    /* ������жϴ�����߲����κ�֪ͨ��ʽ��ֱ�ӷ���*/
    if((CIPHER_SUBM_NONE == subm_attr) || (CIPHER_SUBM_CALLBACK == subm_attr))
    {
        return CIPHER_SUCCESS;
    }

    reg_value = readl(sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_RWPTR(chx));
    while((((reg_value & 0x3ff0000) >> 16) != (reg_value & 0x3ff)) && (loops-- > 0))
	{
        if(loops <=  CIPHER_WAIT_THREHLD)
		{
			taskDelay(1);
            loop_wait_cnt = CIPHER_WAIT_THREHLD;
            do{
                reg_value = readl(sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_RWPTR(chx));
            }while((((reg_value & 0x3ff0000) >> 16) != (reg_value & 0x3ff)) && (loop_wait_cnt-- > 0));
		}
        reg_value = readl(sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_RWPTR(chx));
    }

	if(loops <= 0)
	{
		sg_module_mgr.time_out_bd1[chx]++;
		CIPHER_ERROR_PRINT(CIPHER_TIME_OUT);
		return CIPHER_TIME_OUT;
	}

	/*
	 *�ύ������ʱ�����������������Ǵ���ִ�У������������BD֮�󲻻������µ�BD
	 */
    loops = CIPHER_TIMEOUT_TICKS + CIPHER_WAIT_THREHLD*2;
    /* ѭ����ѯ�ȴ���ʽ�£���ѭ�ȴ��������*/
	reg_value = readl(sg_module_mgr.reg_base_addr + CIPHER_CHN_ENABLE(chx));
    while((reg_value & (0x1UL << 31)) && (loops-- > 0))
    {
        if(loops <=  CIPHER_WAIT_THREHLD)
        {
            taskDelay(1);
            loop_wait_cnt = CIPHER_WAIT_THREHLD;
            do{
                reg_value = readl(sg_module_mgr.reg_base_addr + CIPHER_CHN_ENABLE(chx));
            }while((reg_value & (0x1UL << 31)) && (loop_wait_cnt-- > 0));
        }
        reg_value = readl(sg_module_mgr.reg_base_addr + CIPHER_CHN_ENABLE(chx));
    }

    /* ��ʱ����*/
    if(loops == 0)
    {
        sg_module_mgr.time_out_bd2[chx]++;
		CIPHER_ERROR_PRINT(CIPHER_TIME_OUT);
        return CIPHER_TIME_OUT;
    }

    /* ��ȡ��ǰ��RD������*/
	reg_value = readl(sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_RDPTR(chx));
	cur_rd_addr = (struct cipher_rd_s *)reg_value;
	//cacheInvalidate(DATA_CACHE, (void*)cur_rd_addr,sizeof(struct cipher_rd_s));

    /* �ύ����Ϊ��������ʱ����Ҫ�ƶ�RD*/
    /* ���RD��Чλ*/
    cur_rd_addr->cipher_cfg &= (~0x80000000);

    /* �ж��Ƿ��������Լ�����*/
    if(CIPHER_STAT_CHECK_ERR == ((cur_rd_addr->cipher_cfg >> 29) & 0x3))
    {
		CIPHER_ERROR_PRINT(CIPHER_CHECK_ERROR);
        ret = CIPHER_CHECK_ERROR;
    }

    /* �ƶ��Ĵ����е�RD��ַ*/
    cipher_move_chx_rd_regptr(chx);

    return ret;
}

/*****************************************************************************
* �� �� ��  : cipher_init
*
* ��������  : Cipher��ʼ������
*
* �������  : VOID
* �������  : ��
*
* �� �� ֵ  : �ɹ�/ʧ��
*
* ����˵��  : ��
*
*****************************************************************************/
s32 cipher_init(void)
{
    u32 i;
    s32 ret = CIPHER_SUCCESS;
	u32 org_fifo_addr[3 * 3 + 2] = {0};/*for chn 1, 2 and 3,each chn have 3 addrs. chn 4 owns 2 addrs*/

    if(sg_module_mgr.status)
    {
        return CIPHER_SUCCESS;
    }

    sg_module_mgr.cifer_clk = clk_get(NULL, "cipher_clk");
	if(IS_ERR(sg_module_mgr.cifer_clk))
	{
		CIPHER_ERROR_PRINT(CIPHER_UNKNOWN_ERROR);
		return CIPHER_UNKNOWN_ERROR;
	}

	#ifdef CONFIG_MODULE_VIC
	(void)bsp_vic_disable(INT_LVL_CIPHER);
	#else
	intDisable(INT_LVL_CIPHER);
	#endif

    sg_module_mgr.reg_base_addr = CIPHER_REGBASE_ADDR;
	sg_module_mgr.chx_mgr[1].chx_bd_number = 0x20;
	sg_module_mgr.chx_mgr[2].chx_bd_number = 0x20;
	sg_module_mgr.chx_mgr[3].chx_bd_number = 0x400;/* 1024��cipher֧�ֵ����BD�� */
	sg_module_mgr.chx_mgr[1].cd_number_per_bd = 0xA;
	sg_module_mgr.chx_mgr[2].cd_number_per_bd = 0xA;
	sg_module_mgr.chx_mgr[3].cd_number_per_bd = 0xA;

    for(i = CIPHER_MIN_CHN; i < CIPHER_MAX_CHN; i++)
    {
        /*BDQ��ʼ��*/
        ret = cipher_init_chx_bdq_mem(i, &org_fifo_addr[(i - 1) * 3 + 0]);
        CIPHER_ERR_HANDLER(ret, INIT_MEM_ERR);

        /*CD List��ʼ��*/
        ret = cipher_init_chx_cdlist_mem(i, &org_fifo_addr[(i - 1) * 3 + 1]);
        CIPHER_ERR_HANDLER(ret, INIT_MEM_ERR);

        /*RDQ��ʼ��*/
        ret = cipher_init_chx_rdq_mem(i, &org_fifo_addr[(i - 1) * 3 + 2]);
        CIPHER_ERR_HANDLER(ret, INIT_MEM_ERR);

        /* �ź�������*/
        CIPHER_MTX_CREATE(sg_module_mgr.chx_mgr[i].mtx_chn_opt);
    }

    /* KDF��BDQ��ʼ��*/
    ret = cipher_init_kdf_chx_bdq_mem(&org_fifo_addr[(i - 1) * 3 + 0]);
    CIPHER_ERR_HANDLER(ret, INIT_MEM_ERR);

    /* KDF��RD��ʼ��*/
    ret = cipher_init_kdf_chx_rdq_mem(&org_fifo_addr[(i - 1) * 3 + 1]);
    CIPHER_ERR_HANDLER(ret, INIT_MEM_ERR);

    /* �ź�������*/
    CIPHER_MTX_CREATE(sg_module_mgr.kdf_chx_mgr.mtx_chn_opt);


    /* ������Key�ĳ�������Ϊ��Чֵ*/
    for(i = 0; i < CIPHER_KEY_NUM; i++)
    {
        sg_module_mgr.key_type_len[i] = CIPHER_KEY_LEN_BUTTOM;
    }

	spin_lock_init(&sg_module_mgr.spin_lock_irq);
	spin_lock_init(&sg_module_mgr.spin_lock_reg_conf);
    
#ifdef CONFIG_CCORE_PM
	if(bsp_device_pm_add(&cipher_device))
	{
		CIPHER_ERROR_PRINT(CIPHER_UNKNOWN_ERROR);
		return CIPHER_UNKNOWN_ERROR;
	}
#endif

	if(cipher_open_clk())
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:fail to open clk\n");
		return CIPHER_UNKNOWN_ERROR;
	}
	if(cipher_reg_set())
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:fail to set regs\n");
		return CIPHER_UNKNOWN_ERROR;
	}
	(void)cipher_close_clk();

    /* �ҽ��ж� */
	#ifdef CONFIG_MODULE_VIC
	(void)bsp_vic_connect((VOIDFUNCPTR *)INT_LVL_CIPHER, (VOIDFUNCPTR)cipher_isr, 0);
	(void)bsp_vic_enable(INT_LVL_CIPHER);
	#else
	intConnect((VOIDFUNCPTR *)INT_LVL_CIPHER, (VOIDFUNCPTR)cipher_isr, 0);
	intEnable(INT_LVL_CIPHER);
	#endif

	sg_module_mgr.status = CIPHER_STAT_INIT;
    return CIPHER_SUCCESS;

    /* ������ */
INIT_MEM_ERR:
    for (i = CIPHER_MIN_CHN; i < CIPHER_MAX_CHN; i++)
    {
		sg_module_mgr.chx_mgr[i].p_bd_base = NULL;
		sg_module_mgr.chx_mgr[i].p_rd_base = NULL;
		sg_module_mgr.chx_mgr[i].p_cdlist_base = NULL;
    }
	sg_module_mgr.kdf_chx_mgr.p_bd_base = NULL;
	sg_module_mgr.kdf_chx_mgr.p_rd_base = NULL;

	for(i = 0; i < 11; i++)/*11 is the element number in variable org_fifo_addr*/
		CIPHER_FREE((void *)org_fifo_addr[i]);

    sg_module_mgr.status = CIPHER_STAT_NULL;

    return ret;
}

/*****************************************************************************
* �� �� ��  : bsp_cipher_regist_notify_func
*
* ��������  : ע��֪ͨ�ص�����
*
* �������  : fun_notify_func: ֪ͨ�ص�����
*
* �������  : ��
* �� �� ֵ  : �ɹ�/ʧ��
*****************************************************************************/
s32 bsp_cipher_regist_notify_func(CIPHER_NOTIFY_CB_T fun_notify_func)
{
    s32 lock_key;

    /* CallBack��������ΪNULL */
    CIPHER_CHECK_INIT();

    lock_key = intLock();
    sg_module_mgr.p_func_notify = fun_notify_func;
    intUnlock(lock_key);

    return CIPHER_SUCCESS;

}

/*****************************************************************************
* �� �� ��  : bsp_cipher_regist_freemem_func
*
* ��������  : ע��Memory�ͷŻص�����
*
* �������  : chx: ͨ����
*             b_src:   0:����Buffer / 1:���Bufffer
*             func_freemem_func: Memory�ͷŻص�����
*
* �������  : ��
* �� �� ֵ  : �ɹ�/ʧ��
*****************************************************************************/
s32 bsp_cipher_regist_freemem_func(u32 chx, s32 b_src, CIPHER_FREEMEM_CB_T func_freemem_func)
{
    s32 lock_key;
#ifdef CIPHER_IS_CHECK_PARAM
    CIPHER_CHECK_INIT();
    CIPHER_CHECK_CHN_NUM(chx);
#endif
    lock_key = intLock();

    /* ����Buffer�ͷ�Callback����ע��*/
    if(!b_src)
    {
        sg_module_mgr.chx_mgr[chx].p_func_free_inmem = func_freemem_func;
    }
    else /* ���Buffer�ͷ�Callback����ע��*/
    {
        sg_module_mgr.chx_mgr[chx].p_func_free_outmem = func_freemem_func;
    }
    intUnlock(lock_key);

    return CIPHER_SUCCESS;
}

/*****************************************************************************
* �� �� ��  : bsp_cipher_set_data_buf_para
*
* ��������  : ��ȡ�ϲ�������Ϣ��������Ϣ
*
* �������  : chx        : ͨ����
*             b_src          : TRUE:Դ��ַ���������� / FALSE:Ŀ�ĵ�ַ��������
*             buf_oft     : ����bufferָ��ƫ��
*             len_oft     : Buffer����ƫ��
*             NextOft       : ��һ�ڵ�ƫ��
* �������  : ��
* �� �� ֵ  : �ɹ�/ʧ��
*****************************************************************************/
s32 bsp_cipher_set_data_buf_para(u32 chx, int b_src, u32 buf_oft,u32 len_oft, u32 next_oft)
{
#ifdef CIPHER_IS_CHECK_PARAM
    CIPHER_CHECK_INIT();
    CIPHER_CHECK_CHN_NUM(chx);
#endif
    /* Դ���ݵ�ַ��������*/
    if(b_src)
    {
        sg_module_mgr.chx_mgr[chx].in_buff_addr_oft = buf_oft;
        sg_module_mgr.chx_mgr[chx].in_buff_len_oft  = len_oft;
        sg_module_mgr.chx_mgr[chx].in_buff_next_oft = next_oft;
    }
    else /* Ŀ�ĵ�ַ��������*/
    {
        sg_module_mgr.chx_mgr[chx].out_buff_addr_oft = buf_oft;
        sg_module_mgr.chx_mgr[chx].out_buff_len_oft  = len_oft;
        sg_module_mgr.chx_mgr[chx].out_buff_next_oft = next_oft;
    }

    return CIPHER_SUCCESS;
}

/*****************************************************************************
* �� �� ��  : bsp_cipher_setkey
*
* ��������  : ������Կֵ
*
* �������  : p_key_addr   : ��Կ��ַ
*             key_len   : ��Կ����
*             key_index: ��Կ����
* �������  : ��
* �� �� ֵ  : �ɹ�/ʧ��
*****************************************************************************/
s32 bsp_cipher_setkey(const void* p_key_addr, CIPHER_KEY_LEN_E key_len, u32 key_index)
{/*lint !e18*/
    u32 i = 0;
    u32 *p_key_dest_addr = NULL;
    u32 key_value[CIPHER_KEY_LEN/4] = {0};


	if(cipher_open_clk())
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:fail to open clk\n");
		return CIPHER_UNKNOWN_ERROR;
	}
#ifdef CIPHER_IS_CHECK_PARAM
    /* �������*/
    CIPHER_CHECK_INIT();
    CIPHER_CHECK_ENUM(key_len, CIPHER_KEY_LEN_BUTTOM);
    CIPHER_CHECK_KEYINDEX(key_index);
#endif
    /* ����Key�ĳ��ȵ�ȫ�ֹ���ṹ����*/
    sg_module_mgr.key_type_len[key_index] = key_len;

    memcpy((void *)key_value, (void *)p_key_addr, sg_module_mgr.key_length_table[key_len]);

    /* ����Index�ҵ�����Keyֵ�õ�ַ*/
    p_key_dest_addr = (u32 *)(sg_module_mgr.reg_base_addr + CIPHER_KEYRAM_OFFSET \
                   + (key_index * CIPHER_KEY_LEN) \
                   + (CIPHER_KEY_LEN - sg_module_mgr.key_length_table[key_len]));

    for(i = 0; i < ((sg_module_mgr.key_length_table[key_len])/4); i++)
    {
        key_value[i] = CIPHER_BSWAP32(key_value[i]);

        /* ����Keyֵ*/
        *(p_key_dest_addr) = key_value[i];

        p_key_dest_addr ++;
    }

    return CIPHER_SUCCESS;
}
/*****************************************************************************
* �� �� ��  : bsp_cipher_getkey
*
* ��������  : ����KeyIndex��ȡKey�ĵ�ַ
*
* �������  : key_index: ��Կ����
* �������  : ��
* �� �� ֵ  : �ɹ�:����KeyIndex�õ���Key�ĵ�ַ/ʧ��:��ȡKeyֵ���ڵ�ַʧ��
*****************************************************************************/
s32 bsp_cipher_getkey(u32 key_index, KEY_GET_S *p_key_get)
{
    u32 i;
    CIPHER_KEY_LEN_E key_len;
    u32 key_dest_addr;
    u32 key_value[CIPHER_KEY_LEN/4] = {0};

	if(cipher_open_clk())
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:fail to open clk\n");
		return CIPHER_UNKNOWN_ERROR;
	}
    CIPHER_CHECK_INIT();    /* ��ʼ�����*/
    CIPHER_CHECK_ENUM(key_index, CIPHER_KEY_NUM); /* KeyIndex���*/
    CIPHER_CHECK_PTR(p_key_get);  /* ��ָ����*/
    CIPHER_CHECK_PTR(p_key_get->pKeyAddr);  /* ��ָ����*/
    CIPHER_CHECK_PTR(p_key_get->penOutKeyLen);  /* ��ָ����*/

    /* ����Index��ȫ�ֹ���ṹ���л�ȡKey�ĳ���*/
    key_len = sg_module_mgr.key_type_len[key_index];

    /* ��Keyֵ��ʵ�ʳ��ȷ������ϲ�*/
    (*(p_key_get->penOutKeyLen)) = (u32)key_len;

    /* ���Key�ĳ���Ϊ��Чֵ�����ش���*/
    if(key_len == CIPHER_KEY_LEN_BUTTOM)
    {
		CIPHER_ERROR_PRINT(CIPHER_NO_KEY);
        return CIPHER_NO_KEY;
    }

    /* ���ʵ��Key�ĳ��ȴ����ϲ�ϣ���ĳ��ȣ����ش���*/
    if((u32)key_len > (p_key_get->enKeyLen))
    {
		CIPHER_ERROR_PRINT(CIPHER_KEYLEN_ERROR);
        return CIPHER_KEYLEN_ERROR;
    }

    /* ��ȡKey�ĵ�ַ*/
    key_dest_addr = (sg_module_mgr.reg_base_addr + CIPHER_KEYRAM_OFFSET \
    + (key_index * CIPHER_KEY_LEN) + (CIPHER_KEY_LEN - sg_module_mgr.key_length_table[key_len]));

    for(i = 0; i < (sg_module_mgr.key_length_table[key_len]/4); i++)
    {
        /* ��ȡKeyֵ*/
        key_value[i] = (*(u32 *)(key_dest_addr + (4*i)));
        key_value[i] = CIPHER_BSWAP32(key_value[i]);

        /* ��Keyֵ���õ�Ŀ�ĵ�ַ*/
        (*(u32 *)((u32)(p_key_get->pKeyAddr) + (4*i))) = key_value[i];
    }

    return CIPHER_SUCCESS;
}

/*****************************************************************************
* �� �� ��  : bsp_cipher_single_submit_task
*
* ��������  : ��ʼCIPHER ָ��ͨ���Ĺ��������ڵ���������
*
* �������  : chx: ͨ����
*             p_in_mem_mgr: ����buffer�ڴ����ṹָ��
*             p_out_mem_mgr:���buffer�ڴ����ṹָ��
*             p_cfg: Cipher������������
*             pstSubmAttr:�ύ������
*             Private:˽������
* �������  : ��
* �� �� ֵ  : �ɹ�:(>0)����������/ʧ��
*****************************************************************************/
s32 bsp_cipher_single_submit_task(u32 chx, const void* p_in_mem_mgr, const void* p_out_mem_mgr,
                  CIHPER_SINGLE_CFG_S *p_cfg, CIPHER_SUBMIT_TYPE_E subm_attr, u32 Private)
{/*lint !e18*/
    s32 ret = CIPHER_SUCCESS;
    u32 block_length;

	if(cipher_open_clk())
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:fail to open clk\n");
		return CIPHER_UNKNOWN_ERROR;
	}
    /* �������*/
#ifdef CIPHER_IS_CHECK_PARAM
    CIPHER_CHECK_INIT();
    CIPHER_CHECK_CHN_NUM(chx);
    CIPHER_CHECK_PTR(p_in_mem_mgr);
    CIPHER_CHECK_PTR(p_cfg);
    CIPHER_CHECK_ENUM(subm_attr, CIPHER_SUBM_BUTTOM);
    CIPHER_CHECK_ENUM(p_cfg->enOpt, CIPHER_SINGLE_OPT_BUTTOM);
    CIPHER_CHECK_ENUM(p_cfg->enAppdHeaderLen, CIPHER_HDR_BIT_TYPE_BUTT);
    CIPHER_CHECK_ENUM(p_cfg->enHeaderLen, CIPHER_HDR_BIT_TYPE_BUTT);
    if(sg_module_mgr.security_op[p_cfg->enOpt])
    {
        CIPHER_CHECK_ENUM(p_cfg->stAlgKeyInfo.enAlgSecurity, CIPHER_ALG_BUTTOM);
    }
    if(sg_module_mgr.integrity_op[p_cfg->enOpt])
    {
        CIPHER_CHECK_ENUM(p_cfg->stAlgKeyInfo.enAlgIntegrity, CIPHER_ALG_BUTTOM);
    }
#endif
    /* �ź�������*/
    if(chx != CIPHER_DRB_CHN)
    {
        (void)CIPHER_MTX_ENTER(sg_module_mgr.chx_mgr[chx].mtx_chn_opt);
    }
    sg_module_mgr.single_count[chx]++;
    block_length = p_cfg->u32BlockLen;

    ret = cipher_prepare_start_chn(chx, p_in_mem_mgr, p_out_mem_mgr, p_cfg->bMemBlock, block_length);
    CIPHER_ERR_HANDLER_BDFULL(ret, CIPHER_SINGLESUBM_LEAVE_MTX);

    /* ����Cipher Config�ֶ�*/
	ret = cipher_cfg_script(chx, p_in_mem_mgr, p_out_mem_mgr, p_cfg, -1/*<0��ʾ��������*/, subm_attr, Private);
    CIPHER_ERR_HANDLER(ret, CIPHER_SINGLESUBM_LEAVE_MTX);

    ret = cipher_start_channel(chx, subm_attr);
    CIPHER_ERR_HANDLER(ret, CIPHER_SINGLESUBM_LEAVE_MTX);
    if(chx != CIPHER_DRB_CHN)
    {
        CIPHER_MTX_LEAVE(sg_module_mgr.chx_mgr[chx].mtx_chn_opt);
    }
    return CIPHER_SUCCESS;

CIPHER_SINGLESUBM_LEAVE_MTX:
    if(chx != CIPHER_DRB_CHN)
    {
        CIPHER_MTX_LEAVE(sg_module_mgr.chx_mgr[chx].mtx_chn_opt);
    }
    return ret;
}

/*****************************************************************************
* �� �� ��  : bsp_cipher_rela_submit_task
*
* ��������  : ��ʼCIPHER ָ��ͨ���Ĺ��������ڹ���������
*
* �������  : chx: ͨ����
*             p_in_mem_mgr: ����buffer�ڴ����ṹָ��
*             p_out_mem_mgr:���buffer�ڴ����ṹָ��
*             p_cfg: Cipher������������
*             subm_attr:�ύ������
*             Private:˽������
* �������  : ��
* �� �� ֵ  : �ɹ�:(>0)����������/ʧ��
*****************************************************************************/
s32 bsp_cipher_rela_submit_task(u32 chx, const void* p_in_mem_mgr, const void* p_out_mem_mgr,
                    CIHPER_RELA_CFG_S *p_cfg, CIPHER_SUBMIT_TYPE_E subm_attr, u32 Private)
{/*lint !e18*/
    s32 ret;
    int b_block;
    u32 block_length;


	if(chx == CIPHER_DRB_CHN)
	{
		return CIPHER_INVALID_CHN;
	}
	if(cipher_open_clk())
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:fail to open clk\n");
		return CIPHER_UNKNOWN_ERROR;
	}
#ifdef CIPHER_IS_CHECK_PARAM
    CIPHER_CHECK_INIT();
    CIPHER_CHECK_CHN_NUM(chx);
    CIPHER_CHECK_BUFLIST_PTR(p_in_mem_mgr);
    CIPHER_CHECK_BUFLIST_PTR(p_out_mem_mgr);
    CIPHER_CHECK_PTR(p_cfg);
    CIPHER_CHECK_ENUM(subm_attr, CIPHER_SUBM_BUTTOM);
    CIPHER_CHECK_ENUM(p_cfg->enOpt, CIPHER_RELA_OPT_BUTTOM);
    CIPHER_CHECK_ENUM(p_cfg->enAppdHeaderLen, CIPHER_HDR_BIT_TYPE_BUTT);
    CIPHER_CHECK_ENUM(p_cfg->enHeaderLen, CIPHER_HDR_BIT_TYPE_BUTT);
    CIPHER_CHECK_ENUM(p_cfg->stAlgKeyInfo.enAlgSecurity, CIPHER_ALG_BUTTOM);
    CIPHER_CHECK_ENUM(p_cfg->stAlgKeyInfo.enAlgIntegrity, CIPHER_ALG_BUTTOM);
#endif
    /* �ź�������*/
    if(chx != CIPHER_DRB_CHN)
    {
        (void)CIPHER_MTX_ENTER(sg_module_mgr.chx_mgr[chx].mtx_chn_opt);
    }
    sg_module_mgr.rela_count[chx]++;
    b_block = p_cfg->bMemBlock;
    block_length = p_cfg->u32BlockLen;

    ret = cipher_prepare_start_chn(chx, p_in_mem_mgr, p_out_mem_mgr, b_block, block_length);
    CIPHER_ERR_HANDLER_BDFULL(ret, CIPHER_RELASUBM_LEAVE_MTX);

    /* ����Cipher Config�ֶ�*/
	ret = cipher_cfg_script(chx, p_in_mem_mgr, p_out_mem_mgr, p_cfg, 1/*>=0��ʾ��������*/, subm_attr, Private);
    CIPHER_ERR_HANDLER(ret, CIPHER_RELASUBM_LEAVE_MTX);

    ret = cipher_start_channel(chx, subm_attr);
    CIPHER_ERR_HANDLER(ret, CIPHER_RELASUBM_LEAVE_MTX);
    if(chx != CIPHER_DRB_CHN)
    {
        CIPHER_MTX_LEAVE(sg_module_mgr.chx_mgr[chx].mtx_chn_opt);
    }
    return CIPHER_SUCCESS;

CIPHER_RELASUBM_LEAVE_MTX:
    if(chx != CIPHER_DRB_CHN)
    {
        CIPHER_MTX_LEAVE(sg_module_mgr.chx_mgr[chx].mtx_chn_opt);
    }
    return ret;

}

/*****************************************************************************
* �� �� ��  : bsp_cipher_get_cmpl_src_buff
*
* ��������  : ��ȡ�Ѿ�������ɵ�Դ���ݵ�ַ
*
* �������  : u32ChNum     : ͨ����
* �������  : p_sour_addr : ������ɵ�Դ��ַ;
*             p_dest_addr : ������ɵ�Ŀ�ĵ�ַ
*             p_status    : ������ɵ�״̬
*             Private  : ���ص�˽������
* �� �� ֵ  : �ɹ�/ʧ��
*****************************************************************************/
s32 bsp_cipher_get_cmpl_src_buff(u32 chx, u32 *p_sour_addr,u32 *p_dest_addr, CIPHER_NOTIFY_STAT_E *p_status, u32 *Private)
{
    struct cipher_rd_s *p_cur_rd_addr;
    u32 rd_valid;
    s32 lock_key = 0;
    s32 rd_number;

	if(cipher_open_clk())
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:fail to open clk\n");
		return CIPHER_UNKNOWN_ERROR;
	}
#ifdef CIPHER_IS_CHECK_PARAM
    CIPHER_CHECK_INIT();
    CIPHER_CHECK_CHN_NUM(chx);
    CIPHER_CHECK_PTR(p_sour_addr);
    CIPHER_CHECK_PTR(p_dest_addr);
    CIPHER_CHECK_PTR(p_status);
    CIPHER_CHECK_PTR(Private);
#endif
    /* ʹ���ж���*/
    if(chx != CIPHER_DRB_CHN)
    {
        lock_key = intLock();
    }
	p_cur_rd_addr = (struct cipher_rd_s *)readl(sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_RDPTR(chx));

    rd_number = cipher_getrd_readable_depth(chx);
    if(rd_number <= 0)
    {
        if(chx != CIPHER_DRB_CHN)
        {
            intUnlock(lock_key);
        }
        return CIPHER_RDQ_NULL;
    }
    //cacheInvalidate(DATA_CACHE, (void*)p_cur_rd_addr,sizeof(struct cipher_rd_s));

    /* ��ȡRD�Ƿ���Ч*/
    rd_valid = ((p_cur_rd_addr->cipher_cfg) & 0x80000000);

    /* RD��Ч*/
    if(!rd_valid)
    {
        sg_module_mgr.invalid_rd_number[chx]++;
  		if(chx != CIPHER_DRB_CHN)
		{
        	intUnlock(lock_key);
		}
		CIPHER_ERROR_PRINT(CIPHER_INVALID_RD);
        return CIPHER_INVALID_RD;
    }

    *p_sour_addr = p_cur_rd_addr->usr_field1;
    *p_dest_addr = p_cur_rd_addr->usr_field2;
    *p_status    = (CIPHER_NOTIFY_STAT_E)((p_cur_rd_addr->cipher_cfg & 0x60000000) >> 29);
    *Private  = p_cur_rd_addr->usr_field3;

    /* ���RD��Чλ*/
    p_cur_rd_addr->cipher_cfg &= (~0x80000000);

    /* �ƶ��Ĵ����е�RD��ַ*/
    cipher_move_chx_rd_regptr(chx);
    /* ���ж���*/
    if(chx != CIPHER_DRB_CHN)
    {
        intUnlock(lock_key);
    }
    return CIPHER_SUCCESS;
}

/*****************************************************************************
* �� �� ��  : BSP_CIPHER_Purge
*
* ��������  :
*
* �������  : u32 chx
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
s32 bsp_cipher_purge(u32 chx)
{
    u32 reg_value = 0;
    u32 loops = CIPHER_TIMEOUT_TICKS + CIPHER_WAIT_THREHLD*2;
    s32 loop_wait_cnt = CIPHER_WAIT_THREHLD;
    u32 cur_w_addr = 0;
    struct cipher_chx_mgr_s *chn_mgr = NULL;
    struct cipher_rd_s *p_cur_w_addr, *rdq_rptr;
    void *p_in_memory;
    void *p_out_memory;
    s32 lock_key;

	if(cipher_open_clk())
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:fail to open clk\n");
		return CIPHER_UNKNOWN_ERROR;
	}
#ifdef CIPHER_IS_CHECK_PARAM
    CIPHER_CHECK_INIT();
    CIPHER_CHECK_CHN_NUM(chx);
#endif


    /* �ź�����������ֹPurge����ʱ�����������ύ����*/
    if(chx != CIPHER_DRB_CHN)
    {
        (void)CIPHER_MTX_ENTER(sg_module_mgr.chx_mgr[chx].mtx_chn_opt);
    }
    chn_mgr = &sg_module_mgr.chx_mgr[chx];
    chn_mgr->b_purging = CIPHER_TRUE;

    /* �ȴ�ͨ������ */
	reg_value = readl(sg_module_mgr.reg_base_addr + CIPHER_CHN_ENABLE(chx));
    while((reg_value & (0x1UL << 31)) && (loops-- > 0))
    {
         /* �ȴ�ʱ����ڷ�ֵ,��Ҫ����CPU */
        if(loops <=  CIPHER_WAIT_THREHLD)
        {
            taskDelay(1);
            loop_wait_cnt = CIPHER_WAIT_THREHLD;
            do{
                reg_value = readl(sg_module_mgr.reg_base_addr + CIPHER_CHN_ENABLE(chx));
            }while((reg_value & (0x1UL << 31)) && (loop_wait_cnt-- > 0));
        }
        reg_value = readl(sg_module_mgr.reg_base_addr + CIPHER_CHN_ENABLE(chx));
    }

    /* �˳��ź�������*/
    if(chx != CIPHER_DRB_CHN)
    {
        CIPHER_MTX_LEAVE(sg_module_mgr.chx_mgr[chx].mtx_chn_opt);
    }
    /* ��ʱ����*/
    if(loops == 0)
    {
		CIPHER_ERROR_PRINT(CIPHER_TIME_OUT);
        return CIPHER_TIME_OUT;
    }
    /* �ж�����������ֹ������������жϴ�� */
    lock_key = intLock();
	cur_w_addr = readl(sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_WRPTR(chx));
    p_cur_w_addr = (struct cipher_rd_s *)(cur_w_addr);

	rdq_rptr = (struct cipher_rd_s *)readl(sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_RDPTR(chx));

    while(rdq_rptr < p_cur_w_addr)/*�ж������᲻��������??????*/
    {
        p_in_memory = (void *)(rdq_rptr->usr_field1);
        p_out_memory = (void *)(rdq_rptr->usr_field2);
         /* ����Buffer�ͷ�*/
        if(NULL != sg_module_mgr.chx_mgr[chx].p_func_free_inmem)
        {
            sg_module_mgr.chx_mgr[chx].p_func_free_inmem(p_in_memory);
        }
        /* ���Buffer�ͷ�*/
        if(NULL != sg_module_mgr.chx_mgr[chx].p_func_free_outmem)
        {
            sg_module_mgr.chx_mgr[chx].p_func_free_outmem(p_out_memory);
        }
        /* ���RD��Чλ*/
        rdq_rptr->cipher_cfg &= (~0x80000000);

        /* �ƶ��Ĵ����е�RD��ַ*/
        cipher_move_chx_rd_regptr(chx);
		rdq_rptr = (struct cipher_rd_s *)readl(sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_RDPTR(chx));
    }

    /* ͨ����λ*/
	writel(0x7, sg_module_mgr.reg_base_addr + CIPHER_CHN_RESET(chx));
    /* ʹ��ͨ��*/
	writel(0x1, sg_module_mgr.reg_base_addr + CIPHER_CHN_ENABLE(chx));
    /* �ָ�Purge״̬*/
    cipher_chn_mag_reset(chx);
    chn_mgr->b_purging = CIPHER_FALSE;
    intUnlock(lock_key);

    return CIPHER_SUCCESS;

}

/*============================kdf start==================================*/

/*����ֵ:
*		1:������
*		0:����(���ܿգ����ܲ���)
*/
s32 is_kdf_bdq_full(void)
{
	u32 reg_val_bdq_ptr;

	reg_val_bdq_ptr = readl(sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_RWPTR(KDF_CHN_NUM));
	reg_val_bdq_ptr= reg_val_bdq_ptr & (0x1UL << 0x1F);
	reg_val_bdq_ptr = reg_val_bdq_ptr >> 0x1F;

	return (s32)reg_val_bdq_ptr;
}


static s32 cipher_init_kdf_chx_bdq_mem(u32 * org_kdf_bdq_addr)
{
    u32 size;
    void *p_malloc_addr = NULL;

    if (NULL != sg_module_mgr.kdf_chx_mgr.p_bd_base)
    {
		CIPHER_ERROR_PRINT(CIPHER_UNKNOWN_ERROR);
        return CIPHER_UNKNOWN_ERROR;
    }

    /*ͨ��BDQ�ռ��ȡ*/
    size = sizeof(struct kdf_bd_s) * KDF_CHN_BD_NUM;
    p_malloc_addr = (void *)CIPHER_MALLOC(size + 8);
    if(NULL == p_malloc_addr)
    {
		CIPHER_ERROR_PRINT(CIPHER_NO_MEM);
        return CIPHER_NO_MEM;
    }
	*org_kdf_bdq_addr = (u32)p_malloc_addr;
	p_malloc_addr = (void *)ROUND_UP((unsigned int)p_malloc_addr, 8);
    sg_module_mgr.kdf_chx_mgr.p_bd_base = (struct kdf_bd_s*)p_malloc_addr;
    memset((void *)sg_module_mgr.kdf_chx_mgr.p_bd_base, 0x0, size);

    return CIPHER_SUCCESS;
}

static s32 cipher_init_kdf_chx_rdq_mem(u32 * org_kdf_rdq_addr)
{
    u32 size;
    void *p_malloc_addr = NULL;

    if (NULL != sg_module_mgr.kdf_chx_mgr.p_rd_base)
    {
		CIPHER_ERROR_PRINT(CIPHER_UNKNOWN_ERROR);
        return CIPHER_UNKNOWN_ERROR;
    }

    /*ͨ��RDQ�ռ��ȡ*/
    size = sizeof(struct kdf_rd_s) * KDF_CHN_BD_NUM;
    p_malloc_addr = CIPHER_MALLOC(size + 8);
    if(NULL == p_malloc_addr)
    {
		CIPHER_ERROR_PRINT(CIPHER_NO_MEM);
        return CIPHER_NO_MEM;
    }
	*org_kdf_rdq_addr = (u32)p_malloc_addr;
	p_malloc_addr = (void *)ROUND_UP((unsigned int)p_malloc_addr, 8);
    sg_module_mgr.kdf_chx_mgr.p_rd_base = (struct kdf_rd_s*)p_malloc_addr;
    memset((void *)sg_module_mgr.kdf_chx_mgr.p_rd_base, 0x0, size);

    return CIPHER_SUCCESS;
}

/* �ƶ��Ĵ���дָ��*/
void cipher_move_kdf_chx_bdq_ptr(void)
{
    u32 bd_write_addr;

    /* ��ȡBDQ��дָ��Ĵ���*/
	bd_write_addr = readl(sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_RWPTR(KDF_CHN_NUM));
    /* ��ȡBD����дָ��*/
    bd_write_addr = (bd_write_addr & 0x3FF);
	++bd_write_addr;
	bd_write_addr = bd_write_addr % KDF_CHN_BD_NUM;
    /* �޸�дָ���ֵ*/
	writel(bd_write_addr, sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_RWPTR(KDF_CHN_NUM));
}

void cipher_move_kdf_chx_rd_reg_ptr(void)
{
    u32 rd_read_addr;

    /* ��ȡBDQ��дָ��Ĵ���*/
	rd_read_addr = readl(sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_RWPTR(KDF_CHN_NUM));
    /* ��ȡRD���ж�ָ��*/
    rd_read_addr = ((rd_read_addr & 0x3FF0000) >> 0x10);
	++rd_read_addr;
	rd_read_addr = rd_read_addr % KDF_CHN_RD_NUM;
    rd_read_addr = (rd_read_addr << 0x10);
    /* �޸�дָ���ֵ*/
	writel(rd_read_addr, sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_RWPTR(KDF_CHN_NUM));
}


s32 cipher_kdf_bd_config(struct kdf_bdconfig_info_s *key_cfg_info)
{
    struct kdf_bd_s *p_w_addr =
		(struct kdf_bd_s *)readl(sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_WRPTR(KDF_CHN_NUM));

    p_w_addr->kdf_config= \
                    (key_cfg_info->opt_type & 0x3)              |
                    (key_cfg_info->sha_key_source & 0x3) << 0x2 |
                    (key_cfg_info->sha_s_source & 0x1)   << 0x4 |
                    (key_cfg_info->sha_key_index & 0xf) << 0x7 |
                    (key_cfg_info->sha_s_index & 0xf)   << 0xb |
                    (key_cfg_info->length & 0x1ff)    << 0xf |
                    (key_cfg_info->dest_index & 0xf)   << 0x18;

    p_w_addr->address = (u32)(key_cfg_info->p_address);
    //cacheFlush(DATA_CACHE,(void*)p_w_addr,sizeof(struct kdf_bd_s));

    return CIPHER_SUCCESS;
}

s32 cipher_start_kdf_channel(void)
{
    u32 reg_value;
    struct kdf_rd_s *p_cur_rd_addr = NULL;
    s32 ret = CIPHER_SUCCESS;
    u32 loops = CIPHER_TIMEOUT_TICKS + CIPHER_WAIT_THREHLD*2;
    s32 loop_wait_cnt = CIPHER_WAIT_THREHLD;

    /* ʹ��Cipher����ʼ����*/
	writel(0x1, sg_module_mgr.reg_base_addr + CIPHER_CHN_ENABLE(KDF_CHN_NUM));
    /* �ƶ��Ĵ���дָ�� */
    cipher_move_kdf_chx_bdq_ptr();

    /* ѭ����ѯ�ȴ���ʽ�£���ѭ�ȴ��������*/
	reg_value = readl(sg_module_mgr.reg_base_addr + CIPHER_CHN_ENABLE(KDF_CHN_NUM));
    while((reg_value & (0x1UL << 31)) && (loops-- > 0))
    {
        if(loops <=  CIPHER_WAIT_THREHLD)
        {
            taskDelay(1);
            loop_wait_cnt = CIPHER_WAIT_THREHLD;
            do{
                reg_value = readl(sg_module_mgr.reg_base_addr + CIPHER_CHN_ENABLE(KDF_CHN_NUM));
            }while((reg_value & (0x1UL << 31)) && (loop_wait_cnt-- > 0));
        }
        reg_value = readl(sg_module_mgr.reg_base_addr + CIPHER_CHN_ENABLE(KDF_CHN_NUM));
    }

    /* ��ʱ����*/
    if(loops == 0)
    {
        sg_module_mgr.time_out_kdf2++;
		CIPHER_ERROR_PRINT(CIPHER_TIME_OUT);
        return CIPHER_TIME_OUT;
    }

    /* ��ȡ��ǰ��RD������*/
    p_cur_rd_addr = (struct kdf_rd_s *)readl(sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_RDPTR(KDF_CHN_NUM));
    //cacheInvalidate(DATA_CACHE, (void*)p_cur_rd_addr,sizeof(struct kdf_rd_s));

    /* �ύ����Ϊ��������ʱ����Ҫ�ƶ�RD*/
    /* ���RD��Чλ*/
    p_cur_rd_addr->kdf_config &= (~0x80000000);

    /* �ж��Ƿ��������Լ�����*/
    if(CIPHER_STAT_CHECK_ERR == ((p_cur_rd_addr->kdf_config >> 29) & 0x3))
    {
		CIPHER_ERROR_PRINT(CIPHER_CHECK_ERROR);
        ret = CIPHER_CHECK_ERROR;
    }

    /* �ƶ��Ĵ����е�RD��ַ*/
    cipher_move_kdf_chx_rd_reg_ptr();
    return ret;
}


/*****************************************************************************
* �� �� ��  : bsp_kdf_key_ram_read()
*
* ��������  : ��Key Ram�ж�ȡ����
*
* �������  : enKeyIndex: Key�������Ӹ���������ʼ��ȡ����
*             length : ��ȡ���ݳ��ȣ���ByteΪ��λ
* �������  : p_dest_addr : Ŀ�ĵ�ַ������KeyRam�ж�ȡ�����ݴ洢����λ��
* �� �� ֵ  : �ɹ�: OK
*           : ʧ��: CIPHER_ERROR
*****************************************************************************/
s32 bsp_kdf_key_ram_read(void *p_dest_addr, u32 key_index ,u32 read_length)
{
    s32 ret = OK;
    struct kdf_bdconfig_info_s bd_cfg_info = {0};/*lint !e64*/

	if(cipher_open_clk())
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:fail to open clk\n");
		return CIPHER_UNKNOWN_ERROR;
	}
	/* �ź�������*/
	CIPHER_MTX_ENTER(sg_module_mgr.kdf_chx_mgr.mtx_chn_opt);/*lint !e534*/

    /* �ռ䲻��, ���ش���*/
	ret = is_kdf_bdq_full();
    if(0x1 == ret)
    {
		CIPHER_ERROR_PRINT(CIPHER_FIFO_FULL);
		CIPHER_MTX_LEAVE(sg_module_mgr.kdf_chx_mgr.mtx_chn_opt);
        return CIPHER_FIFO_FULL;
    }

    bd_cfg_info.opt_type = kdf_op_read;
    bd_cfg_info.length = read_length;
    bd_cfg_info.dest_index = key_index;
    bd_cfg_info.p_address = p_dest_addr;

    cipher_kdf_bd_config(&bd_cfg_info);/*lint !e534*/
    ret = cipher_start_kdf_channel();

	CIPHER_MTX_LEAVE(sg_module_mgr.kdf_chx_mgr.mtx_chn_opt);

    return ret;
}

/*****************************************************************************
* �� �� ��  : bsp_kdf_key_ram_write
*
* ��������  : ��KeyRam��д������
*
* �������  : enKeyIndex: Key�������Ӹ���������ʼд������
*             p_sour_addr : Դ��ַ��д�뵽KeyRam�е����ݵ�ַ
*             length : ���ݳ���
*
* �������  : ��
* �� �� ֵ  : �ɹ�: OK
*           : ʧ��: CIPHER_ERROR
*****************************************************************************/
s32 bsp_kdf_key_ram_write(u32 key_index, void *p_sour_addr, u32 w_length)
{
    s32 ret = OK;
    struct kdf_bdconfig_info_s bd_cfg_info = {0};/*lint !e64*/

	if(cipher_open_clk())
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:fail to open clk\n");
		return CIPHER_UNKNOWN_ERROR;
	}
	/* �ź�������*/
	CIPHER_MTX_ENTER(sg_module_mgr.kdf_chx_mgr.mtx_chn_opt);/*lint !e534*/

    /* �ռ䲻��, ���ش���*/
	ret = is_kdf_bdq_full();
    if(0x1 == ret)
    {
		CIPHER_ERROR_PRINT(CIPHER_FIFO_FULL);
		CIPHER_MTX_LEAVE(sg_module_mgr.kdf_chx_mgr.mtx_chn_opt);
        return CIPHER_FIFO_FULL;
    }

    bd_cfg_info.opt_type = kdf_op_write;
    bd_cfg_info.length = w_length;
    bd_cfg_info.dest_index = key_index;
    bd_cfg_info.p_address = p_sour_addr;

    cipher_kdf_bd_config(&bd_cfg_info);/*lint !e534*/
    ret = cipher_start_kdf_channel();

	CIPHER_MTX_LEAVE(sg_module_mgr.kdf_chx_mgr.mtx_chn_opt);

    return ret;

}

/*****************************************************************************
* �� �� ��  : bsp_kdf_key_make
*
* ��������  : KDF���㣬���ڲ����µ�Key
*
* �������  : key_cfg_info: KDF����ʱ����sha_key��������
            : key_cfg_info: KDF����ʱ����sha_s��������
            : enDestIndex : KDF���������Key������KeyRam�е�λ��
* �������  : ��
* �� �� ֵ  : �ɹ�:OK
*           : ʧ��:CIPHER_ERROR
*****************************************************************************/
s32 bsp_kdf_key_make(KEY_CONFIG_INFO_S *p_key_cfg_info, S_CONFIG_INFO_S *p_s_cfg_info,
                        u32 dest_index, KEY_MAKE_S *p_key_make)
{
    u32 i;
    u32 key_dest_addr = 0;
    s32 ret = CIPHER_SUCCESS;
    void * p_malloc_addr = NULL;
    struct kdf_bdconfig_info_s bd_cfg_info = {0};  /*lint !e64*/
    u32 key_value[CIPHER_KEY_LEN/4] = {0};


	if(cipher_open_clk())
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:fail to open clk\n");
		return CIPHER_UNKNOWN_ERROR;
	}
    CIPHER_CHECK_INIT();                           /* ��ʼ�����*/
    CIPHER_CHECK_ENUM(dest_index, CIPHER_KEY_NUM); /* KeyIndex���*/
    CIPHER_CHECK_PTR(p_key_cfg_info);              /* ��ָ����*/
    CIPHER_CHECK_PTR(p_s_cfg_info);                /* ��ָ����*/
    CIPHER_CHECK_PTR(p_key_make);                  /* ��ָ����*/

	/* �ź�������*/
	CIPHER_MTX_ENTER(sg_module_mgr.kdf_chx_mgr.mtx_chn_opt);/*lint !e534*/

    /* �ռ䲻��, ���ش���*/
	ret = is_kdf_bdq_full();
    if(0x1 == ret)
    {
		CIPHER_ERROR_PRINT(CIPHER_FIFO_FULL);
        return CIPHER_FIFO_FULL;
    }

    p_malloc_addr = (void *)CIPHER_MALLOC(KDF_SHA_KEY_LENGTH + KDF_SHA_S_LENGTH_MAX);
    if(NULL == p_malloc_addr)
    {
		CIPHER_ERROR_PRINT(CIPHER_NO_MEM);
        return CIPHER_NO_MEM;
    }

    if((p_key_cfg_info->enShaKeySource == SHA_KEY_SOURCE_DDR) &&
      (p_s_cfg_info->enShaSSource == SHA_S_SOURCE_DDR))
    {
        memcpy(p_malloc_addr, p_key_cfg_info->pKeySourceAddr, KDF_SHA_KEY_LENGTH);
        memcpy((void *)((u32)p_malloc_addr + KDF_SHA_KEY_LENGTH),
                             (void *)(p_s_cfg_info->pSAddr), (p_s_cfg_info->u32ShaSLength));
    }
    else if(p_key_cfg_info->enShaKeySource == SHA_KEY_SOURCE_DDR)
    {
        memcpy(p_malloc_addr, p_key_cfg_info->pKeySourceAddr, KDF_SHA_KEY_LENGTH);
    }
    else if(p_s_cfg_info->enShaSSource == SHA_S_SOURCE_DDR)
    {
        memcpy(p_malloc_addr, p_s_cfg_info->pSAddr, KDF_SHA_S_LENGTH_MAX);
    }
    //cacheFlush(DATA_CACHE,(void*)p_malloc_addr,KDF_SHA_KEY_LENGTH + KDF_SHA_S_LENGTH_MAX);

    bd_cfg_info.opt_type      = kdf_op_keymake;
    bd_cfg_info.sha_key_source = p_key_cfg_info->enShaKeySource;
    bd_cfg_info.sha_s_source   = p_s_cfg_info->enShaSSource;
    bd_cfg_info.sha_key_index  = p_key_cfg_info->u32ShaKeyIndex;
    bd_cfg_info.sha_s_index    = p_s_cfg_info->u32ShaSIndex;
    bd_cfg_info.length      = p_s_cfg_info->u32ShaSLength- 1;/*��Ϊֻ9bit����S�������512�������߼��Զ���������ֵ��1*/
    bd_cfg_info.dest_index   = dest_index;
    bd_cfg_info.p_address       = p_malloc_addr;

    cipher_kdf_bd_config(&bd_cfg_info);/*lint !e534*/
    ret = cipher_start_kdf_channel();
    if(CIPHER_SUCCESS == ret)
    {
        /* ����Key�ĳ��ȵ�ȫ�ֹ���ṹ����*/
        sg_module_mgr.key_type_len[dest_index] = CIPHER_KEY_L256;

        /* �ϲ���Ҫ����Keyֵ��Ŀ�ĵ�ַ*/
        if(p_key_make->enKeyOutput == CIPHER_KEY_OUTPUT)
        {
            if(NULL == (p_key_make->stKeyGet.pKeyAddr))
            {
				CIPHER_ERROR_PRINT(CIPHER_NULL_PTR);
                CIPHER_FREE(p_malloc_addr);
                return CIPHER_NULL_PTR;
            }

            if(NULL == (p_key_make->stKeyGet.penOutKeyLen))
            {
				CIPHER_ERROR_PRINT(CIPHER_NULL_PTR);
                CIPHER_FREE(p_malloc_addr);
                return CIPHER_NULL_PTR;
            }

            /* ��Key�ĳ��ȷ������ϲ�*/
            (*(p_key_make->stKeyGet.penOutKeyLen)) = (u32)CIPHER_KEY_L256;

            /* ���ʵ��Key�ĳ��ȴ����ϲ�ϣ���ĳ��ȣ����ش���*/
            if((u32)CIPHER_KEY_L256 > (p_key_make->stKeyGet.enKeyLen))
            {
				CIPHER_ERROR_PRINT(CIPHER_KEYLEN_ERROR);
                CIPHER_FREE(p_malloc_addr);
                return CIPHER_KEYLEN_ERROR;
            }

            /* ���KeyMake���ɵ�Key�ĵ�ַ*/
            key_dest_addr = (sg_module_mgr.reg_base_addr + CIPHER_KEYRAM_OFFSET \
            + (dest_index * CIPHER_KEY_LEN) + (CIPHER_KEY_LEN - sg_module_mgr.key_length_table[CIPHER_KEY_L256]));

            for(i = 0; i < (sg_module_mgr.key_length_table[CIPHER_KEY_L256]/4); i++)
            {
                /* ��ȡKeyֵ*/
                key_value[i] = (*(u32 *)(key_dest_addr + (4*i)));
                key_value[i] = CIPHER_BSWAP32(key_value[i]);

                /* ��Keyֵ���õ�Ŀ�ĵ�ַ*/
                (*(u32 *)((u32)(p_key_make->stKeyGet.pKeyAddr) + (4*i))) = key_value[i];
            }
        }
    }
    CIPHER_FREE(p_malloc_addr);

	CIPHER_MTX_LEAVE(sg_module_mgr.kdf_chx_mgr.mtx_chn_opt);

    return ret;
}
/*============================kdf end====================================*/
int cipher_open_clk(void)
{
	unsigned long spin_irq_flag = 0;

	spin_lock_irqsave(&(sg_module_mgr.spin_lock_irq), spin_irq_flag);
	if(sg_module_mgr.enabled >= 1)
	{
		spin_unlock_irqrestore(&(sg_module_mgr.spin_lock_irq), spin_irq_flag);
		return 0;
	}
#ifdef CONFIG_CIPHER_ENABLE_BBPPLL
	if(DRV_PWRCTRL_PLLENABLE(PWC_COMM_MODE_LCIPHER, PWC_COMM_MODULE_BBP_DRX, PWC_COMM_MODEM_0))
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:DRV_PWRCTRL_PLLENABLE fails in BSP_CIPHER_Enable\n");
		spin_unlock_irqrestore(&(sg_module_mgr.spin_lock_irq), spin_irq_flag);
		return CIPHER_UNKNOWN_ERROR;
	}
#endif

#ifndef BSP_CONFIG_HI3630
	/*��ʱ�Ӹĳ�5��Ƶ��V7R2оƬĬ��ֵ*/
	(void)clk_set_rate(sg_module_mgr.cifer_clk, PERIPLL_FOUNTPOST / 5);
#endif
    //open the clock of cipher
	if(clk_enable(sg_module_mgr.cifer_clk))
	{
		CIPHER_ERROR_PRINT(CIPHER_UNKNOWN_ERROR);
		spin_unlock_irqrestore(&(sg_module_mgr.spin_lock_irq), spin_irq_flag);
		return CIPHER_UNKNOWN_ERROR;
	}

	sg_module_mgr.enabled += 1;
	spin_unlock_irqrestore(&(sg_module_mgr.spin_lock_irq), spin_irq_flag);
    return 0;
}
int cipher_close_clk(void)
{
	unsigned long spin_irq_flag = 0;

	spin_lock_irqsave(&(sg_module_mgr.spin_lock_irq), spin_irq_flag);
	if(sg_module_mgr.enabled <= 0)
	{
		spin_unlock_irqrestore(&(sg_module_mgr.spin_lock_irq), spin_irq_flag);
		return 0;
	}
	clk_disable(sg_module_mgr.cifer_clk);
#ifdef CONFIG_CIPHER_ENABLE_BBPPLL
	(void)DRV_PWRCTRL_PLLDISABLE(PWC_COMM_MODE_LCIPHER, PWC_COMM_MODULE_BBP_DRX, PWC_COMM_MODEM_0);
#endif
	sg_module_mgr.enabled -= 1;
	spin_unlock_irqrestore(&(sg_module_mgr.spin_lock_irq), spin_irq_flag);
    return 0;
}
int cipher_reg_save(void)
{
    int i = 0;
	u32 reg_val;
	unsigned long spin_irq_flag = 0;
	spin_lock_irqsave(&(sg_module_mgr.spin_lock_reg_conf), spin_irq_flag);
	if(0 == sg_module_mgr.suspend_enable)
	{
		spin_unlock_irqrestore(&(sg_module_mgr.spin_lock_reg_conf), spin_irq_flag);
		return 0;
	}

	/*check wether cipher is busy. status + read&write ptr*/
	reg_val = readl(CIPHER_REGBASE_ADDR + 0);
	if(reg_val & (0x1U << 31))
	{
		spin_unlock_irqrestore(&(sg_module_mgr.spin_lock_reg_conf), spin_irq_flag);
		return CIPHER_ERROR;
	}
	for(i = CIPHER_MIN_CHN; i <= CIPHER_MAX_CHN; i++)
	{
		reg_val = readl(CIPHER_REGBASE_ADDR + CIPHER_CHNBDQ_RWPTR(i));
		if((reg_val & 0x3FF) != ((reg_val >> 16) & 0X3FF))
		{
			spin_unlock_irqrestore(&(sg_module_mgr.spin_lock_reg_conf), spin_irq_flag);
			return CIPHER_ERROR;
		}
		reg_val = readl(CIPHER_REGBASE_ADDR + CIPHER_CHNRDQ_RWPTR(i));
		if((reg_val & 0x3FF) != ((reg_val >> 16) & 0X3FF))
		{
			spin_unlock_irqrestore(&(sg_module_mgr.spin_lock_reg_conf), spin_irq_flag);
			return CIPHER_ERROR;
		}
	}
	reg_val = readl(CIPHER_REGBASE_ADDR + CIPHER_CHNBDQ_RWPTR(0));
	if((reg_val & 0x3FF) != ((reg_val >> 16) & 0X3FF))
	{
		spin_unlock_irqrestore(&(sg_module_mgr.spin_lock_reg_conf), spin_irq_flag);
		return CIPHER_ERROR;
	}
	/*disable channels 1/2/3/4*/
	for(i = 0; i <= CIPHER_MAX_CHN; i++)
	{
		writel(0x40000000, sg_module_mgr.reg_base_addr + CIPHER_CHN_ENABLE(i));
	}

	for(i = 0; i < (int)(CIPHER_KEY_NUM * CIPHER_KEY_LEN / sizeof(u32)); i++)
	{
		reg_val = readl(CIPHER_REGBASE_ADDR + CIPHER_KEYRAM_OFFSET + i * 4);
		sg_module_mgr.key_ram_bak_buf[i] = reg_val;
	}
	sg_module_mgr.suspend_enable = 0;
	spin_unlock_irqrestore(&(sg_module_mgr.spin_lock_reg_conf), spin_irq_flag);
	return 0;
}
int cipher_reg_set(void)
{
    int i = 0;
    u32 value;
	unsigned long spin_irq_flag = 0;
	spin_lock_irqsave(&(sg_module_mgr.spin_lock_reg_conf), spin_irq_flag);
	if(1 == sg_module_mgr.suspend_enable)
	{
		spin_unlock_irqrestore(&(sg_module_mgr.spin_lock_reg_conf), spin_irq_flag);
		return 0;
	}

    //reset cipher IP module, and enable its automatic clock gating
	writel(0x3, CIPHER_REGBASE_ADDR + HI_CIPHER_CTRL_OFFSET);

    //set registers on bd of channel 1/2/3
    for(i = CIPHER_MIN_CHN; i < CIPHER_MAX_CHN; i++)
    {
        //bdq
	    writel((u32)(sg_module_mgr.chx_mgr[i].p_bd_base), sg_module_mgr.reg_base_addr + (u32)CIPHER_CHNBDQ_BASE(i));
	    writel(sg_module_mgr.chx_mgr[i].chx_bd_number - 1, sg_module_mgr.reg_base_addr + (u32)CIPHER_CHNBDQ_SIZE(i));
	    writel(0, sg_module_mgr.reg_base_addr + (u32)CIPHER_CHNBDQ_RWPTR(i));
        //rdq
	    writel((u32)(sg_module_mgr.chx_mgr[i].p_rd_base), sg_module_mgr.reg_base_addr + (u32)CIPHER_CHNRDQ_BASE(i));
	    writel(sg_module_mgr.chx_mgr[i].chx_bd_number - 1, sg_module_mgr.reg_base_addr + (u32)CIPHER_CHNRDQ_SIZE(i));/*RD��BD����ͬ*/
	    writel(0, sg_module_mgr.reg_base_addr + (u32)CIPHER_CHNRDQ_RWPTR(i));
    }
    //kdf bdq
    writel((u32)(sg_module_mgr.kdf_chx_mgr.p_bd_base), sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_BASE(KDF_CHN_NUM));
    writel(KDF_CHN_BD_NUM - 0x1, sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_SIZE(KDF_CHN_NUM));
    writel(0x0, sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_RWPTR(KDF_CHN_NUM));
    //kdf rdq
    writel((u32)(sg_module_mgr.kdf_chx_mgr.p_rd_base), sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_BASE(KDF_CHN_NUM));
	writel(KDF_CHN_RD_NUM - 0x1, sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_SIZE(KDF_CHN_NUM));
	writel(0x0, sg_module_mgr.reg_base_addr + CIPHER_CHNRDQ_RWPTR(KDF_CHN_NUM));
	//ACC
	writel(ACC_FIFO_MAX_DEEP - 1, sg_module_mgr.reg_base_addr + CIPHER_CHNBDQ_SIZE(0));

    /*����ͨ�����ȼ���ͨ�����ط�ֵ cipher_pri*/
    value = (CIPHER_CHN_PRI << CHN_PRI_BIT)|(CIPHER_CHN_STRMTHRESHHLD);
	writel(value, sg_module_mgr.reg_base_addr + CIPHER_PRI_OFFSET);

    for(i = CIPHER_MIN_CHN; i < CIPHER_MAX_CHN; i++)
    {
        /* ����ͨ�����üĴ���*/
        value = CHN_IV_SEL \
                   | (CHN_RDQ_CTRL << CHN_RDQCTRL_BIT) \
                   | CHN_USRFIELD_LEN << CHN_USRFIELDLEN_BIT;
		writel(value, sg_module_mgr.reg_base_addr + (u32)CIPHER_CHN_CONFIG(i));
    }
    /* ����KDFͨ��config�Ĵ���*/
    value = CHN_IV_SEL \
               | (CHN_RDQ_CTRL << CHN_RDQCTRL_BIT) \
               | KDF_CHN_USRFIELD_LEN << CHN_USRFIELDLEN_BIT;
	writel(value, sg_module_mgr.reg_base_addr + CIPHER_CHN_CONFIG(KDF_CHN_NUM));

    /* ��������ж�*/
	writel(0x3f3f3f3f, sg_module_mgr.reg_base_addr + CIPHER_INT0_STAT_OFFSET);
	writel(0x3f3f3f3f, sg_module_mgr.reg_base_addr + CIPHER_INT0_MSSTAT_OFFSET);
	writel(0x07070707, sg_module_mgr.reg_base_addr + CIPHER_INT1_STAT_OFFSET);
	writel(0x07070707, sg_module_mgr.reg_base_addr + CIPHER_INT1_MSSTAT_OFFSET);
    /* Channel4���ж����*/
	writel(0xff, sg_module_mgr.reg_base_addr + CIPHER_INT2_STAT_OFFSET);
	writel(0xff, sg_module_mgr.reg_base_addr + CIPHER_INT2_MSSTAT_OFFSET);
    /* �����ж����μĴ���,
       ����ʹ�õ��ж���BD�ڵ��������жϣ������ж�ȫ������*/
    /*ACC�ж����Σ�����0��ͨ���ж�Ӱ������ͨ�����жϴ���*/
	writel(0x3737373f, sg_module_mgr.reg_base_addr + CIPHER_INT0_MASK_OFFSET);
	writel(0x0707070f, sg_module_mgr.reg_base_addr + CIPHER_INT1_MASK_OFFSET);
	writel(0xff, sg_module_mgr.reg_base_addr + CIPHER_INT2_MASK_OFFSET);

    /* restore KeyRam�����˶��ڲ�ram��ӳ��Ϊ�Ĵ�����ֻ�ܰ��ֵ�λд�� */
    for(i = 0; i < (int)(CIPHER_KEY_NUM * CIPHER_KEY_LEN / sizeof(u32)); i++)
	{
		writel(sg_module_mgr.key_ram_bak_buf[i], CIPHER_REGBASE_ADDR + CIPHER_KEYRAM_OFFSET + i * 4);
	}

    //enable all channels except the 0th one
    writel(0x1, sg_module_mgr.reg_base_addr + CIPHER_CHN_ENABLE(1));
    writel(0x1, sg_module_mgr.reg_base_addr + CIPHER_CHN_ENABLE(2));
    writel(0x1, sg_module_mgr.reg_base_addr + CIPHER_CHN_ENABLE(3));
    writel(0x1, sg_module_mgr.reg_base_addr + CIPHER_CHN_ENABLE(KDF_CHN_NUM));

	sg_module_mgr.suspend_enable = 1;
	spin_unlock_irqrestore(&(sg_module_mgr.spin_lock_reg_conf), spin_irq_flag);
	return 0;
}

#ifdef CONFIG_CCORE_PM
s32 bsp_cipher_suspend(struct dpm_device *dev)
{
	if(cipher_open_clk())
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:fail to open clk\n");
		return CIPHER_UNKNOWN_ERROR;
	}
	if(cipher_reg_save())
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:stop suspend\n");
		return CIPHER_UNKNOWN_ERROR;
	}
	(void)cipher_close_clk();
	return 0;
}

s32 bsp_cipher_resume(struct dpm_device *dev)
{
	if(cipher_open_clk())
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:fail to open clk\n");
		return CIPHER_UNKNOWN_ERROR;
	}
	if(cipher_reg_set())
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_CIPHER,"CIPHER:fail to set regs\n");
		return CIPHER_UNKNOWN_ERROR;
	}
	(void)cipher_close_clk();
    return 0;
}
#endif
