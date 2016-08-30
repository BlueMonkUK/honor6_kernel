
/******************************************************************************
*    Copyright (c) 2009-2011 by  Hisilicon Tech. Co., Ltd.
*
*    All rights reserved.
*
******************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif


#ifdef __FASTBOOT__
#include <ptable_com.h>
#include "nandc_inc.h"


/*lint -save -e767*/
#define NFCDBGLVL(LVL)      (NANDC_TRACE_HOST|NANDC_TRACE_##LVL)
/*lint -restore*/

/*nandc�������Ĵ�����ģ�嶨��,�����еļĴ�����bitλ��������صĹ���*/
struct nandc_bit_cluster  nandc_bit_cluster_normal_template =
{
    STU_SET(.cmd)           		{nandc_bits_cmd,            			NANDC_NULL,    NANDC_NULL},
/*
    STU_SET(.cmd2)          		{nandc_bits_cmd2,           			NANDC_NULL,    NANDC_NULL},
    STU_SET(.cmd3)          		{nandc_bits_cmd3,           			NANDC_NULL,    NANDC_NULL},
*/
    STU_SET(.bus_width)     		{nandc_bits_bus_width,      			NANDC_NULL,    NANDC_NULL},
    STU_SET(.addr_high)     		{nandc_bits_addr_high,      			NANDC_NULL,    NANDC_NULL},
    STU_SET(.addr_low)      		{nandc_bits_addr_low,       			NANDC_NULL,    NANDC_NULL},

    STU_SET(.addr_cycle)    		{nandc_bits_addr_cycle,     			NANDC_NULL,    NANDC_NULL},
    STU_SET(.chip_select)   		{nandc_bits_chip_select,    			NANDC_NULL,    NANDC_NULL},
    STU_SET(.operate)       		{nandc_bits_operation,      			NANDC_NULL,    NANDC_NULL},

    STU_SET(.op_stauts)     		{nandc_bits_op_status,      			NANDC_NULL,    NANDC_NULL},
    STU_SET(.int_stauts)    		{nandc_bits_int_status,     			NANDC_NULL,    NANDC_NULL},
    STU_SET(.data_num)      		{nandc_bits_data_num,       			NANDC_NULL,    NANDC_NULL},

    STU_SET(.ecc_type)      		{nandc_bits_ecc_type,       			NANDC_NULL,    NANDC_NULL},
    STU_SET(.ecc_select)    		{nandc_bits_ecc_select,     			NANDC_NULL,    NANDC_NULL},

    STU_SET(.int_enable)    		{nandc_bits_int_enable,     			NANDC_NULL,    NANDC_NULL},

    STU_SET(.op_mode)       		{nandc_bits_op_mode,        			NANDC_NULL,    NANDC_NULL},
    STU_SET(.page_size)     		{nandc_bits_page_size,      			NANDC_NULL,    NANDC_NULL},
    STU_SET(.int_clr)       		{nandc_bits_int_clear,      			NANDC_NULL,    NANDC_NULL},

    STU_SET(.segment)       		{nandc_bits_segment,        			NANDC_NULL,    NANDC_NULL},
    STU_SET(.ecc_result)    		{nandc_bits_ecc_result,     			NANDC_NULL,    NANDC_NULL},
    STU_SET(.nf_status)     		{nandc_bits_nf_status,      			NANDC_NULL,    NANDC_NULL},

#ifdef NANDC_USE_V600
	STU_SET(.async_interface_type)  {nandc_bits_async_interface_type,      	NANDC_NULL,    NANDC_NULL},
    STU_SET(.randomizer)   			{nandc_bits_randomizer,      			NANDC_NULL,    NANDC_NULL},
    STU_SET(.read_id_en) 			{nandc_bits_read_id_en,      			NANDC_NULL,    NANDC_NULL},

	STU_SET(.rw_reg_en)     		{nandc_bits_rw_reg_en,      			NANDC_NULL,    NANDC_NULL},
    STU_SET(.ecc_class)     		{nandc_bits_ecc_class,      			NANDC_NULL,    NANDC_NULL},
    STU_SET(.randomizer_en)     	{nandc_bits_randomizer_en,      		NANDC_NULL,    NANDC_NULL},

	STU_SET(.cs0_done)     			{nandc_bits_cs0_done,      				NANDC_NULL,    NANDC_NULL},
    STU_SET(.ahb_op_err)     		{nandc_bits_ahb_op_err,      			NANDC_NULL,    NANDC_NULL},
    STU_SET(.wr_lock_err)     		{nandc_bits_wr_lock_err,      			NANDC_NULL,    NANDC_NULL},

	STU_SET(.dma_done)     			{nandc_bits_dma_done,      				NANDC_NULL,    NANDC_NULL},
    STU_SET(.dma_err)     			{nandc_bits_dma_err,      				NANDC_NULL,    NANDC_NULL},
    STU_SET(.dma_start)     		{nandc_bits_dma_start,      			NANDC_NULL,    NANDC_NULL},
/*
	STU_SET(.dma_rw_enable)     	{nandc_bits_dma_rw_enable,      		NANDC_NULL,    NANDC_NULL},
    STU_SET(.burst_enable)     		{nandc_bits_burst_enable,      			NANDC_NULL,    NANDC_NULL},
    STU_SET(.dma_addr_num)     		{nandc_bits_dma_addr_num,      			NANDC_NULL,    NANDC_NULL},

	STU_SET(.dma_cs)     			{nandc_bits_dma_cs,      				NANDC_NULL,    NANDC_NULL},
    STU_SET(.dma_wr)     			{nandc_bits_dma_wr,      				NANDC_NULL,    NANDC_NULL},
    STU_SET(.dma_rd_oob)     		{nandc_bits_dma_rd_oob,      			NANDC_NULL,    NANDC_NULL},
*/
	STU_SET(.base_addr_data)     	{nandc_bits_base_addr_data,      		NANDC_NULL,    NANDC_NULL},
    STU_SET(.base_addr_oob)     	{nandc_bits_base_addr_oob,      		NANDC_NULL,    NANDC_NULL},
    STU_SET(.dma_len)     			{nandc_bits_dma_len,      				NANDC_NULL,    NANDC_NULL},

	STU_SET(.errnum0buf0)     		{nandc_bits_errnum0buf0,      			NANDC_NULL,    NANDC_NULL},
    STU_SET(.rb_status)     		{nandc_bits_rb_status,      			NANDC_NULL,    NANDC_NULL},
    STU_SET(.base_addr_d1)     		{nandc_bits_base_addr_d1,      			NANDC_NULL,    NANDC_NULL},

	STU_SET(.base_addr_d2)     		{nandc_bits_base_addr_d2,      			NANDC_NULL,    NANDC_NULL},
    STU_SET(.base_addr_d3)     		{nandc_bits_base_addr_d3,      			NANDC_NULL,    NANDC_NULL},
    STU_SET(.oob_len_sel)     		{nandc_bits_oob_len_sel,      			NANDC_NULL,    NANDC_NULL},

	STU_SET(.dma_ctrl)     			{nandc_bits_dma_ctrl,      				NANDC_NULL,    NANDC_NULL},

    STU_SET(.int_op_done)     		{NANDC_BITS_OP_DONE_EN,      			NANDC_NULL,    NANDC_NULL},
    STU_SET(.int_cs_done)     		{NANDC_BITS_CS_DONE_EN,      			NANDC_NULL,    NANDC_NULL},
    STU_SET(.int_err_invalid)    	{NANDC_BITS_ERR_INVALID_EN,      		NANDC_NULL,    NANDC_NULL},
    STU_SET(.int_err_valid)     	{NANDC_BITS_ERR_VALID_EN,      		    NANDC_NULL,    NANDC_NULL},
    STU_SET(.int_ahb_op)     		{NANDC_BITS_AHD_OP_EN,      			NANDC_NULL,    NANDC_NULL},
    STU_SET(.int_wr_lock)     		{NANDC_BITS_WR_LOCK_EN,      			NANDC_NULL,    NANDC_NULL},
    STU_SET(.int_dma_done)     		{NANDC_BITS_DMA_DONE_EN,      			NANDC_NULL,    NANDC_NULL},
    STU_SET(.int_dma_err)     		{NANDC_BITS_DMA_ERR_EN,      			NANDC_NULL,    NANDC_NULL},
#endif

    STU_SET(.endtable)      		{nandc_bits_enum_end,       			NANDC_NULL,    NANDC_NULL},
};


/**
* ����:����nandc_hostʹ�õ��ǲ���4bitλECC
*
* ����:
* @host      ---��nandc_host��ָ�����
*
* ����:����Ƿ���ʹ�õ�4bit��ECC
*/
int is_nand_id_4bitecc(struct nandc_host* host)
{
    int ret = NANDC_FALSE;
    unsigned char *table = host->nand_4bitecc_table;

    /* ����nandc v400��host->nand_4bitecc_tableΪ�� */
    if(!table || !(*table))
        return ret;

    while(*table != NANDC_NULL)
    {
        if(!memcmp(host->flash_id, table, NANDC_READID_SIZE))
        {
            ret = NANDC_TRUE;
            goto EXIT;
        }
        table += NANDC_READID_SIZE;
    }
    return ret;
EXIT:
    return ret;
}


void set_phyaddr_low_8bit(struct nandc_host* host)
{
    struct ST_PART_TBL* ptable = NULL;
    u32 flash_addr;

    /*
     * �����m3boot����������oob_len_sel = 0; else��oob_len_sel = 1
     */
    ptable = find_partition_by_name(PTABLE_M3BOOT_NM);
    if(!ptable)
    {
        NANDC_TRACE(NFCDBGLVL(ERRO), ("find m3boot partition failed\n"));
        return;
    }

    flash_addr = ptable->offset + ptable->capacity;
    host->addr_to_sel_oob_len = (flash_addr >> host->nandchip->spec.pageshift) << NANDC_COLUMN_SHIFT;

    return;
}


/**
* ����:�õ���nandc_hostʹ�õ�nandc_ecc_info����
*
* ����:
* @host      ---��nandc_host��ָ�����
* @chip      ---nandc_nand��ָ�����
* @maxeccinfo      ---��nandc_ecc_info��ָ�����
* ����:�õ�nandʹ�õ�nandc_ecc_info
*/
u32 nandc_host_get_ecc_ability(struct nandc_host  *host,  struct nandc_nand  *chip,struct nandc_ecc_info** maxeccinfo)
{
    struct nandc_ecc_info* ecc_table =  host->eccinfo;
    u32 oobsize = chip->spec.sparesize ;
    u32 pagesize = chip->spec.pagesize;

    HI_NOTUSED(oobsize);
	/*����ecc_table�������*/
    while(NULL != ecc_table->layout  )
    {
        if((pagesize == ecc_table->pagesize ) &&(oobsize >= ecc_table->sparesize))
        {
        	/*�ҵ���ص�nandc_ecc_info�����ݽṹ*/
            *maxeccinfo = ecc_table;
            return NANDC_OK;
        }
        ecc_table++;
    }
    NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_host_get_ecc_ability error oobsize:%d, pagesize:0x%x \n",
                                        oobsize, pagesize));
    return NANDC_ERROR;
}

/**
* ����:��nandc_bit_reg�ĵ����Ĵ�������ع���ֵ���Ƶ�nandc_reg_desc����ͬ�Ľṹ��ȥ
*
* ����:
* @reg_tbl            ---reg_tbl��ʾ�������еļĴ�����һ������
* @nandc_bit_set      ---��ʾ����Ҫ��д���ݣ�����һ���������ܺŵ��ҵ�Ҫ��д���ݽṹ��ָ��
*
* ����:����nandc_bit_cluster_normal_template����ص����ݽṹ��ֵ
*/
u32 nandc_host_init_regset(struct nandc_reg_desc* reg_tbl, struct nandc_bit_reg* nandc_bit_set )
{
    struct nandc_reg_desc* table = reg_tbl;
    struct nandc_reg_cont* reg_cont = NANDC_NULL;
    u32 target = nandc_bit_set->bits_func;


    while(NANDC_NULL !=  table->content)
    {
        reg_cont = table->content;

        while(nandc_bits_enum_end !=  reg_cont->func  )
        {
            if(target == (u32)reg_cont->func )
            {
                /* set nand controller register's offset */
                nandc_bit_set->reg_offset = table->addr_offset;
                nandc_bit_set->bitset = &reg_cont->set;
                return NANDC_OK;
            }
            reg_cont++;
        }
        table++;
    }
    NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_host_init_regset return error: target = %d\n",target));
	return NANDC_ERROR;
}

/**
* ����:����value��ֵ�Ƿ���bitstbl��������
*
* ����:
* @bitstbl            ---�����Ÿ������ܵĺ�����
* @value              ---Ҫ���ҵĹ��ܵĺ�����
*
* ����:�ڴ�bitstbl�в����Ƿ���valueֵû��
*/
static inline u32 find_in_table(u8* bitstbl, u32 value)
{
    while((u8)nandc_bits_enum_end != *bitstbl)
    {
        if(value == (u32)*bitstbl)
        {
            return NANDC_TRUE;
        }

        bitstbl++;
    }

    return NANDC_FALSE;
}


/**
* ����:����nandc_bit_cluster_normal_template�еĸ��ṹ�����
* ����:
* @host               ---nandc_hostָ��
* @reg_tbl            ---һ������,��ʾ���Ǹ��Ĵ�����ֵ�Ķ���
* @bitstbl            ---��ʾ��һЩ�������ܵ����ֵҪ����
* ����:����nandc_bit_cluster_normal_template����ص����ݽṹ��ֵ,���Ѵ˵�ַ��ֵ��host��normal��ָ��
*/
u32 nandc_host_init_cluster(struct nandc_host *host, struct nandc_reg_desc* reg_tbl, u8 *bitstbl)
{
    struct nandc_bit_reg* bitreg = NANDC_NULL;

    bitreg = (struct nandc_bit_reg*)&nandc_bit_cluster_normal_template;

    while((u32)nandc_bits_enum_end !=  bitreg->bits_func)
    {
        if(find_in_table(bitstbl,bitreg->bits_func))
        {
            if(NANDC_OK != nandc_host_init_regset(reg_tbl, bitreg))
            {
                goto ERRO;
            }
        }
        bitreg++;
    }

    host->normal = &nandc_bit_cluster_normal_template;

    return  NANDC_OK;

ERRO:

    return NANDC_ERROR;

}

/**
* ����:��nand_chipΪĬ��ֵ����N����ָ�����
* ����:
* @chipselect               ---nandc_nand����Ŀ
* @nand_chip                ---nandc_nand��Ĭ��ֵ
* @nand_priv                ---��ʾҪ������nandc_nand�ĵ�ָ���ָ��
*/
static inline u32 nandc_host_get_nand_priv(u32 *chipselect,  struct nandc_nand* nand_chip, struct nandc_nand **nand_priv)
{
	u32 i = nand_chip->chipnum;
	/*��i��nandc_nand�����ݽṹ*/
	while(i > 0)
	{
		/*����Щָ�붼��ֵ����nand_priv������*/
		*nand_priv = (struct nandc_nand *)nand_chip;
		nand_priv++;
		i--;
	}
	/*�õ�оƬ����Ŀ*/
	*chipselect = nand_chip->chipnum;

	return NANDC_OK;
}


/**
* ����:����nandc_host��partition���ݽṹ
*
* ����:
* @host               ---nandc_hostָ��
* @ptable             ---ptable��������Ϣ
* @nr_parts           ---nr_parts��������Ŀ
* ����:����nandc_host�еķ���ָ��
*/
u32  nandc_host_set_partition(struct nandc_host * host, struct mtd_partition* ptable, u32 nr_parts)
{
    FSZ sizesum, sizemax;
    struct mtd_partition*  patbale = NANDC_NULL;
    struct nandc_nand*   p_nand[NANDC_MAX_CHIPS] = {0};
    struct nandc_ecc_info* maxecc = NANDC_NULL;
    struct nandc_partition *ctrl_define = host->usrdefine;
    u32 totalchip ;
    u32 curchip;
    int i;

    nandc_host_get_nand_priv(&totalchip, host->nandchip, p_nand);
	/*�����������Ϣ�����ڵĻ�*/
	if(!ctrl_define)
	{
		ctrl_define = (struct nandc_partition*)himalloc(sizeof(struct nandc_partition));
		NANDC_REJECT_NULL(ctrl_define);
		/*modified for lint e516 */
		memset((void*)ctrl_define, 0x00, (int)sizeof(struct nandc_partition));

		host->usrdefine =  ctrl_define;
	}

	/*���ptable�ǲ���Ļ�*/
    if(NANDC_NULL == ptable)
    {
        /*set default partition patbale*/
        patbale = (struct mtd_partition *)himalloc(totalchip * sizeof(struct mtd_partition));
        NANDC_REJECT_NULL(patbale);
        memset((void*)patbale, 0x00 , (totalchip *sizeof(struct mtd_partition)));

        ctrl_define->partition = patbale;
        ctrl_define->nbparts = totalchip;
        /*if no partition table give, one chip defult to has one partition*/
        NANDC_DO_ASSERT( totalchip <= NANDC_MAX_CHIPS, "too many chips", totalchip);

       for (curchip = 0; curchip < totalchip; curchip++)
       {
            if(NANDC_OK == nandc_host_get_ecc_ability(host, p_nand[curchip],  &maxecc))
            {

               if(host->ecctype_onfi)
                {
                    p_nand[curchip]->ecctype = host->ecctype_onfi;

                }
                else
                {
                    p_nand[curchip]->ecctype = maxecc->ecctype;

                    /* check whether 4 bit ecc */
                    if(is_nand_id_4bitecc(host))
                    {
                        p_nand[curchip]->ecctype = NANDC_ECC_4BIT;
                    }
                }

                if(0 == curchip)
                {
                    patbale->size   =   p_nand[curchip]->spec.chipsize;
                    patbale->name   =   "partition0";
                    patbale->ecclayout = maxecc->layout;

                }
                else
                {

                    patbale->offset =  (patbale - 1)->offset + (patbale - 1)->size;
                    patbale->size =    p_nand[curchip]->spec.chipsize;
                    patbale->name = NANDC_NULL;
                    patbale->ecclayout = maxecc->layout;
                }
                patbale++;
            }
            else
            {
                NANDC_DO_ASSERT(0 ,"best ecc not find", curchip) ;
            }
       }

    }
    else
    {
       sizemax = 0;
       for (curchip = 0; curchip < totalchip; curchip++)
       {
            sizemax += p_nand[curchip]->spec.chipsize;
            if(NANDC_OK == nandc_host_get_ecc_ability(host, p_nand[curchip],  &maxecc))
            {
                if(host->ecctype_onfi)
                {
                    p_nand[curchip]->ecctype = host->ecctype_onfi;

                }
                else
                {
                    p_nand[curchip]->ecctype = maxecc->ecctype;

                    /* check whether 4 bit ecc */
                    if(is_nand_id_4bitecc(host))
                    {
                        p_nand[curchip]->ecctype = NANDC_ECC_4BIT;
                    }
                }
            }
            else
            {
                NANDC_DO_ASSERT(0 ,"best ecc not find", curchip) ;
            }
       }
       patbale = ptable;

        sizesum = 0;
        i = nr_parts;   /*lint !e713 */
        while(i > 0)
		{
			sizesum += patbale[i - 1].size;/*lint !e712*/
			/*modified for lint e794 */
			if (maxecc != NANDC_NULL)
			{
				patbale[i - 1].ecclayout  = maxecc->layout;
			}
			/*modified for lint e794 */
			i--;
		}

        NANDC_DO_ASSERT(((u32)sizesum <= (u32)sizemax), "partition total size too large!", (u32)sizesum);

        ctrl_define->partition = ptable;
        ctrl_define->nbparts = nr_parts;
    }

    host->curpart = ctrl_define->partition;
    return NANDC_OK;

ERRO:
    /* coverity[dead_error_condition] */
    if(NANDC_NULL != patbale)/*lint !e774 !e944 */
    {
        /* coverity[dead_error_line] */
        if(NANDC_NULL != patbale->name)
        {
            hifree(patbale->name);
        }
        hifree(patbale);
    }
    return NANDC_ERROR;
}/*lint !e550 */


/**
* ����:��nandc_dmap_lookup�����в�������ecctype��pagesize��nandc_dmap_lookup
*
* ����:
* @lookuptbl               ---nandc_dmap_lookup������
* @ecctype                 ---����������ecc����
* @pagesize                ---����������ҳ��С
* @badoff                  ---���ػ����ǵ�ƫ��ֵ
* ����:����ecctype��pagesize����nandc_dmap_lookup
*/
struct nandc_pagemap * nandc_host_get_pagemap(struct nandc_dmap_lookup  * lookuptbl,  u32 ecctype, u32 pagesize, u32* badoff)
{
    struct nandc_pagemap * findmap = NANDC_NULL;

    while(NANDC_NULL != lookuptbl->datamap)
    {
        if( ecctype ==  lookuptbl->ecctype)
        {
            switch(pagesize)
            {
                case NANDC_SIZE_2K:
                        findmap = lookuptbl->datamap->pmap2k;
                        *badoff = lookuptbl->datamap->bad2k;
                goto EXIT;

                case NANDC_SIZE_4K:
                        findmap = lookuptbl->datamap->pmap4k;
                        *badoff = lookuptbl->datamap->bad4k;
                goto EXIT;

                case NANDC_SIZE_8K:
                        findmap = lookuptbl->datamap->pmap8k;
                        *badoff = lookuptbl->datamap->bad8k;
                goto EXIT;

                default:

                 goto ERRO;
            }
        }
        lookuptbl++ ;
    }

    if(NANDC_NULL == lookuptbl->datamap)
    {
        goto ERRO;
    }

 EXIT:

    return findmap;

ERRO:
	NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_host_get_pagemap error,ecc: 0x%x, size:0x%x \n",ecctype, pagesize));
	NANDC_DO_ASSERT(0, HICHAR_NULL, 0);
	return NANDC_NULL;

}

/**
* ����:pagesize�Ĵ�Сת��
*
* ����:
* @pagesize               ---ҳ�Ĵ�С
* ����:pagesize�Ĵ�Сת��
*/
u32 nandc_host_get_pageset(u32 pagesize)
{
    u32 pageset ;
    switch(pagesize)
    {
        case NANDC_SIZE_HK:
            pageset = nandc_page_hk;
        break;
        case NANDC_SIZE_2K:
            pageset = nandc_page_2k;
        break;
        case NANDC_SIZE_4K:
            pageset = nandc_page_4k;
        break;
        case NANDC_SIZE_8K:
            pageset = nandc_page_8k;
        break;
        default:
            pageset = nandc_page_2k;
            NANDC_DO_ASSERT(0, HICHAR_NULL, 0);
            break;
      }

    return pageset;
}


/**
* ����:����nandc_nand����ز���
*
* ����:
* @host               ---nandc_host�Ľṹ��ָ��
* ����:����nandc_nand�����ݽṹ��ֵ
*/
u32 nandc_host_set_chipparam(struct nandc_host * host)
{
    /* host->dmaplookup is initialized by "nandc_native_host_create"*/
    struct nandc_dmap_lookup  *lookup   = host->dmaplookup;
    struct nandc_nand    *nand_chip     = host->nandchip;
    u32 ecctype;
    u32 pagesize;

    ecctype   =  nand_chip->ecctype;
    pagesize  =  nand_chip->spec.pagesize;

    nand_chip->datamap = nandc_host_get_pagemap(lookup, ecctype, pagesize, &host->badoffset);
    nand_chip->pageset = nandc_host_get_pageset(nand_chip->spec.pagesize);

    nand_chip->busset   =   (NAND_BUSWIDTH_16 == nand_chip->spec.buswidth )?nandc_bus_16:nandc_bus_08;

    NANDC_TRACE(NFCDBGLVL(NORMAL), ("chip(%d) ,ecc:0x%x, size:0x%x \n",nand_chip->chipnum, ecctype, pagesize));

    return NANDC_OK;
}


/**
* ����:����nandc_nand���ڴ沢��ֵ,Ȼ��Ѵ�nandc_nand��ָ�������ֵ��nandc_host
*
* ����:
* @host               ---nandc_host�Ľṹ��ָ��
* @addnum             ---addnum��ʾ����оƬ��
* @spec               ---nand�Ĺ�����
*/
u32 nandc_host_add_nand_chip(struct nandc_host * host,  u32 addnum, struct nandc_spec *spec)
{
    struct nandc_nand  *nand = NANDC_NULL;
    u32 errocode = NANDC_ERROR;
    u32 pagesize, erasesize, buswidth, sparesize;
	FSZ chipsize;

    pagesize  = spec->pagesize;
    erasesize = spec->blocksize;
    buswidth  = spec->buswidth;
    chipsize  = spec->chipsize;
    sparesize = spec->sparesize;

    nand = (struct nandc_nand*)himalloc(sizeof(struct nandc_nand));
    NANDC_REJECT_NULL(nand);

    nand->spec.name         =   spec->name;
    nand->spec.pagesize     =   NANDC_CHECK_PAGESIZE(pagesize);
    nand->spec.blocksize    =   NANDC_CHECK_ERASESIZE(erasesize);
    nand->spec.buswidth     =   NANDC_CHECK_BUSWIDE(buswidth);
    nand->spec.chipsize     =   NANDC_CHECK_CHIPSIZE(chipsize);
    nand->spec.sparesize    =   NANDC_CHECK_SPARESIZE(sparesize);

    if(0 == nand->spec.pagesize
        || 0 == nand->spec.sparesize
        || 0 == nand->spec.blocksize
        || 0 == nand->spec.chipsize )
    {
        /* coverity[printf_arg_mismatch] */
        NANDC_TRACE(NFCDBGLVL(ERRO), ("create nand error param: pagesize:0x%x, sparesize:0x%x, \n erasesize:0x%x, buswidth:0x%x, chipsize:0x%x\n",
                                                         pagesize, sparesize, erasesize, buswidth, (unsigned int)chipsize));
		errocode = NANDC_E_PARAM;
        goto ERRO;
    }

    nand->spec.offinpage    =   pagesize - 1;
    nand->spec.pagealign    =   ~(nand->spec.offinpage);
    nand->spec.offinblock   =   erasesize - 1;
    nand->spec.blockalign   =   ~(nand->spec.offinblock);
	/*begin lint-Info 732: (Info -- Loss of sign (assignment) (int to unsigned int))*/
	nand->spec.pageshift    =   (u32)(ffs(nand->spec.pagesize) - 1);/*lint !e713 !e732*/
	nand->spec.blockshift   =   (u32)(ffs(nand->spec.blocksize) - 1);/*lint !e713 !e732*/
	/*end*/
    nand->spec.blockpages   =   erasesize >> nand->spec.pageshift; /* page number in one block*/
    nand->spec.blockmask    =   (u32)(chipsize >> nand->spec.blockshift) - 1;
    nand->spec.pagesum      =   nand->spec.blockmask * nand->spec.blockpages;
	nand->spec.pagemask     =   (u32)(chipsize >> nand->spec.pageshift) - 1;
	if (chipsize & 0xffffffff)
    {
    	/*begin lint-Info 732: (Info -- Loss of sign (assignment) (int to unsigned int))*/
		nand->spec.chipshift = (u32)(ffs((unsigned)chipsize) - 1);/*lint !e713 !e732*/
		/*end*/
    }
    else
    {
#ifdef NANDC_NAND_SIZE_WIDTH64
        nand->spec.chipshift = ffs((unsigned)(chipsize >> 32)) + 32 - 1;
#endif

    }
    if(chipsize > nandc_size_1Gbit)
    {
        nand->spec.addrcycle = NANDC_ADDRCYCLE_5;
    }
    else
    {
        nand->spec.addrcycle = NANDC_ADDRCYCLE_4;
    }

    nand->spec.options      =   NANDC_NULL;

    nand->chipnum  = addnum;
    host->nandchip = nand;

    /* just for 8bit ecc */
    set_phyaddr_low_8bit(host);

    return NANDC_OK;

ERRO:
    if(NANDC_NULL != nand)
    {
        hifree(nand);
    }

    NANDC_DO_ASSERT(0, "nandc create nand error!",errocode);
    return errocode;

}

#endif

#ifdef __cplusplus
}
#endif
