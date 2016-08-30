/*******************************************************************************
* Copyright (C), 2008-2011, HUAWEI Tech. Co., Ltd.
*
* Module name: Hisilicon Nand Flash Controller Dirver
*
* Description: The Hisilicon Nand Flash Controller physical and logical driver
*
*
* Filename:    nandc_ctrl.c
* Description: nandcģ��Ŀ��ƼĴ����Ĳ���
*
*******************************************************************************/
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
/*lint -save -e506 */
/*lint --e{537}*/

#include <hi_nandc.h>
#include <ptable_com.h>
#include "nandc_inc.h"

/*nandflashģ��Ĵ�ӡ����*/
#define NFCDBGLVL(LVL)       (NANDC_TRACE_CTRL|NANDC_TRACE_##LVL)

static inline u32 nandc_ctrl_program_result(struct nandc_host *host, u32 size_left);
static inline u32 nandc_ctrl_erase_result(struct nandc_host *host);
static inline void nandc_ctrl_recur_snapshot(struct nandc_host *host, struct nandc_reg_list* op_config);
static inline u32 nandc_ctrl_create_snapshot(struct nandc_host *host,
                                                       struct nandc_config_snapshot **snapshot,
                                                       u32 *totalsize,
                                                       u32 *onesize,
                                                       u32 *eccselect);
int g_nand_status;


/**
* ����:����pagesize�Ĵ�С���õ�Ҫ�ּ���������
*
* ����:
* @pagesize          --ҳ��Ĵ�С
* ����:����pagesizeҪ����ּ��δ���
*
*/
static inline u32 nandc_ctrl_divs_shift(u32 pagesize)
{
	u32 shifttime = 0;

	while(pagesize > NANDC_BUFSIZE_BASIC)
	{
		pagesize = pagesize >> 1;
		shifttime++;
	}
	return shifttime;
}


/**
* ����:����nandc_host->option�������õ�һ�δ������ݵĴ�С��ecc�����͵Ȳ���
*
* ����:
* @host          		    --nandc������������ָ��
* @totalsize          		--�ܹ����͵ĳ���
* @ecctype          		--ECC������(1bit,4bit,16bit)
* @eccselect          		--ECC��ѡ������(oob��ECC,oob+data��ECC)
* @onesize          		--һ�δ��͵ĳ���
* ����:����nandc_host�еĲ����õ���������������Ϣ
*
*/
static inline u32 nandc_ctrl_get_param(struct nandc_host *host,
                                                u32 *totalsize,
                                                u32 *ecctype,
                                                u32 *eccselect,
                                                u32 *onesize
                                                )
{
	u32  ofs_in_page , total_size, total_oob, shifttimes,one_size;
	struct nandc_nand       *nand   =   host->nandchip;
	struct nandc_spec       *spec   =   &nand->spec;
	struct nandc_bit_cmd    *ctrlcmd=   &host->bitcmd;
	/*�ܹ���ecc����Ŀ*/
	total_oob   = host->curpart->ecclayout->oobavail;
	ofs_in_page  = ((u32)host->addr_physics[0]) & 0x0000FFFF;

	if(nandc_bus_16 == nand->busset)
	{
	    ofs_in_page <<=  1;
	}

	shifttimes   =  nandc_ctrl_divs_shift(spec->pagesize);
	/*��ʹ��ECCģʽ����*/
	if(host->options & NANDC_OPTION_ECC_FORBID)
	{
		/*total_size��ʾ����һҳ�����ܹ������ݳ���,�������ܹ�Ҫ�������ݳ��ȣ��������������һҳһҳ�Ķ�����*/
		total_size = host->length;

		/*�������*/
		if((ofs_in_page + total_size) > (spec->pagesize + spec->sparesize))
		{
			NANDC_TRACE(NFCDBGLVL(ERRO), ("mode0 not support yet! length:0x%x, offset in page: 0x%x",total_size, ofs_in_page));
			NANDC_DO_ASSERT(0, HICHAR_NULL, 0);
			goto ERRO;
		}
		/*����ʹ��ECCʱһ������ȡ��������Ϊpagesize+sparesize*/
		one_size    =   (spec->pagesize + spec->sparesize) >> shifttimes;
		*ecctype    =   0 ;
		*eccselect  =   ctrlcmd->ecc_none;
	}
	else
	{
	    /*ʹ��ECCʱ�õ�ECC������*/
	    *ecctype    =   nand->ecctype;
	     NANDC_REJECT_NULL(!ofs_in_page);
		/*������OOB���ݵĻ�*/
	    if(host->options & NANDC_OPTION_OOB_ONLY)
	    {
	    	/*�õ���ص�������Ϣ*/
	        total_size  =   total_oob;
	        one_size    =   total_size;
	        *eccselect  =   ctrlcmd->ecc_oob;

	    }
		/*OOB��data��Ҫ����ECC*/
	    else
	    {
	    	/*�ܹ������ݳ�����OOB��pagesize*/
	        total_size  =   spec->pagesize + total_oob;
	        one_size    =   total_size >> shifttimes;
	        *eccselect  =   ctrlcmd->ecc_all;/* nandc_ecc_all */

	    }

	}

	*totalsize = total_size;
	*onesize = one_size;

	return NANDC_OK;

ERRO:

	return NANDC_ERROR;
}


/**
* ����:�����Ĵ���������ݽṹ���ص�������ȥ
*
* ����:
* @host          		    --nandc������������ָ��
* @regaddr          		--�Ĵ����ĵ�ַ
* @listreg          		--�Ĵ�������ָ��
*
* ����:�ڼĴ����������������regaddr��ַ�ı���
*
*/
static inline  void nandc_ctrl_reg_to_list(struct nandc_host *host,u32 regaddr, struct nandc_reg_list** listreg)
{
	HI_NOTUSED(host);

	/*�����ͷΪ���򴴽�*/
	if(0 == *listreg)
	{
		*listreg = (struct nandc_reg_list*)himalloc(sizeof(struct nandc_reg_list));
		NANDC_REJECT_NULL(*listreg);
		(*listreg)->address = regaddr;
		(*listreg)->value   = NANDC_REG_READ32(regaddr);
		(*listreg)->p_next  = NANDC_NULL;
		return ;
	}
	else
	{
		struct nandc_reg_list* list = *listreg;
		struct nandc_reg_list* last;

		do
		{   /*����Ѿ�������*/
		    if(regaddr == list->address)
		    {
		        list->value = NANDC_REG_READ32(regaddr);
		        return;
		    }

		    last = list;
		    list = list->p_next;
		}while(NANDC_NULL != list);

		/*������û��Ҫ����һ�����ݽṹ*/
		last->p_next = (struct nandc_reg_list*)himalloc(sizeof(struct nandc_reg_list));
		NANDC_REJECT_NULL(last->p_next);
		last->p_next->address = regaddr;
		last->p_next->value   = NANDC_REG_READ32(regaddr);
		last->p_next->p_next  = NANDC_NULL;
	}
ERRO:
	return;
}


/**
* ����:�ѼĴ�����ֵ�ŵ�snapshot��ȥ����дӲ���Ĵ���
*
* ����:
* @bitfunc          		--�Ĵ����Ķ���
* @value          		    --Ҫд�ļĴ�����ֵ
* @snapshot          		--snapshot�����ݽṹ
*
*/
#define  nandc_ctrl_write_snapshot(bitfunc,value, snapshot)  \
{                                                                                   \
    if((bitfunc).bitset)                                                            \
    {                                                                               \
        NANDC_SET_REG_BITS(bitfunc, value);                                         \
        nandc_ctrl_reg_to_list(host,(u32)host->regbase + ((bitfunc).reg_offset), &((*snapshot)->listreg));\
    }                                                                               \
}


/**
* ����:���ݲ���������snapshot,һ��������һ��snapshot����һ�ξ�����
*
* ����:
* @host          		    --nandc������������ָ��
* @snapshot          		--snapshot��ָ���ָ��,�Ѵ�����snapshot���ݽṹָ�����ڴ�ֵ��
* @totalsize          		--�ܹ����͵����ݳ���
* @onesize          		--һ�δ��͵����ݳ���
* @eccselect          		--ECC������
* ����:���ݲ���������һ��snapshot
*
*/
static inline u32 nandc_ctrl_create_snapshot(struct nandc_host *host,
                                                       struct nandc_config_snapshot **snapshot,
                                                       u32 *totalsize,
                                                       u32 *onesize,
                                                       u32 *eccselect)
{
    register struct nandc_bit_cluster*   cluster;
    struct nandc_nand           *nand = host->nandchip;
    struct nandc_spec           *spec = &nand->spec;
    u32  ecctype;

    register struct nandc_bit_cmd*       ctrlcmd;


    if(NANDC_OK != nandc_ctrl_get_param(host, totalsize, &ecctype, eccselect, onesize))
    {
        goto ERRO;
    }

    cluster = host->normal;

    *snapshot = (struct nandc_config_snapshot*)himalloc(sizeof(struct nandc_config_snapshot));
    NANDC_REJECT_NULL(*snapshot);
    memset((void*)*snapshot, 0x00 ,sizeof(struct nandc_config_snapshot));

    if(NAND_CMD_READSTART == host->command)
    {
        nandc_ctrl_write_snapshot(cluster->cmd, NAND_CMD_READ0|(NAND_CMD_READSTART<<8)|(NAND_CMD_NOP<<16),snapshot);
    }
    else
    {
        nandc_ctrl_write_snapshot(cluster->cmd, NAND_CMD_SEQIN|(NAND_CMD_PAGEPROG<<8)|(NAND_CMD_STATUS<<16),snapshot);
    }
    nandc_ctrl_write_snapshot(cluster->bus_width,    nand->busset,       snapshot );
    nandc_ctrl_write_snapshot(cluster->page_size,    nand->pageset,      snapshot );
    nandc_ctrl_write_snapshot(cluster->ecc_type,     ecctype,            snapshot );
    nandc_ctrl_write_snapshot(cluster->ecc_select,   *eccselect,         snapshot );



    ctrlcmd = &host->bitcmd;
    NANDC_WRITE_REG_BITS     (cluster->addr_cycle,   spec->addrcycle              );
    nandc_ctrl_write_snapshot(cluster->addr_cycle,   spec->addrcycle,    snapshot );
    nandc_ctrl_write_snapshot(cluster->chip_select,  host->chipselect,   snapshot );
    nandc_ctrl_write_snapshot(cluster->int_enable,   ctrlcmd->inten_all, snapshot );

	/*�����DMA�ķ�ʽ��������ECC0ʱ��Ҫ��������ļĴ���*/
    if((host->options & NANDC_OPTION_DMA_ENABLE) && ((host->options & NANDC_OPTION_ECC_FORBID)))
    {
        nandc_ctrl_write_snapshot(cluster->dma_len,   spec->sparesize,   snapshot );
    }

    (*snapshot)->totalsize = *totalsize;
    (*snapshot)->onesize   = *onesize;
    (*snapshot)->eccselect = *eccselect;

    return NANDC_OK;
ERRO:
    return NANDC_ERROR;
}


/**
* ����:�ѼĴ��������е�����ֵд�뵽Ӳ���Ĵ�����
*
* ����:
* @host          		    --nandc������������ָ��
* @op_config          		--�Ĵ����������ݽṹ

* ����:дӲ���Ĵ�����ֵ
*
*/
static inline void nandc_ctrl_recur_snapshot(struct nandc_host *host, struct nandc_reg_list* op_config)
{
    HI_NOTUSED(host);

    while(NANDC_NULL != op_config)
    {
        NANDC_REG_WRITE32(op_config->address, op_config->value);
        op_config = op_config->p_next;
    }
}/*lint !e438*/

/**
* ����:����nandc_host�еĲ���������Ӳ���Ĵ���
*
* ����:
* @host          		    --nandc������������ָ��
* @snapshot          		--snapshot��ָ��
* @totalsize          		--�ܹ����͵����ݳ���
* @onesize          		--һ�δ��͵����ݳ���
* @eccselect          		--ECC������
* ����:���ݲ�����������ص�Ӳ���Ĵ���
*
*/
static inline void nandc_ctrl_snapshot(struct nandc_host *host, struct nandc_snapshot *snapshot,
                                                u32 *total_size, u32 *one_size, u32* eccselect)
{
	register struct nandc_config_snapshot      *config = NULL;
	struct nandc_bit_cluster*   cluster = host->normal;
	u32 onesize = 0,  totalsize=0 ; /*pc-lint 644*/
	/*ʹ��ECC��ʱ��������Ĵ�������*/
	if((!(host->options & NANDC_OPTION_ECC_FORBID)))
	{
		/*���������nandc_config_snapshotֱ�Ӱ�ֵд��Ӳ���Ĵ���*/
		if(NANDC_NULL != (u32)snapshot->config_ecc)
		{
		    config = snapshot->config_ecc;
		    nandc_ctrl_recur_snapshot(host, config->listreg);
		    totalsize  = config->totalsize;
		    onesize    = config->onesize;
		    *eccselect = config->eccselect;
		}
		else
		{
			/*����nandc_config_ecc�����ݽṹ*/
		    nandc_ctrl_create_snapshot(host, &snapshot->config_ecc, &totalsize, &onesize, eccselect);
		}

#ifdef NANDC_USE_V600
		/* just for 8bit ecc */
		if (NANDC_ECC_8BIT == host->nandchip->ecctype)
		{
            #ifndef NANDC_OOBLENGTH_8BITECCTYPE
		    /* m3boot�����⣬set 1��else set 0 */
		    if((host->addr_physics[0] < host->addr_to_sel_oob_len) && (0 == host->addr_physics[1]))
		    {
		        NANDC_SET_REG_BITS(cluster->oob_len_sel, 0);
		    }
			else
            #endif
			{
		        NANDC_SET_REG_BITS(cluster->oob_len_sel, 1);
		    }
		}
#endif

	}
	/*�����ʹ��ECC*/
	else if(host->options & NANDC_OPTION_ECC_FORBID)
	{
		if(NANDC_NULL != (u32)snapshot->config_raw)
		{
			/*��д����*/
		    config = snapshot->config_raw;
		    nandc_ctrl_recur_snapshot(host, config->listreg);
			/*�����ʾ�˴�Ҫ���д�����ݴ�С*/
		    totalsize  = host->length;
		    onesize    = config->onesize;
		    *eccselect = config->eccselect;
		}
		else
		{
		    nandc_ctrl_create_snapshot(host,&snapshot->config_raw, &totalsize, &onesize, eccselect);
		}
		/*ECC��ʹ�ܵ�ʱ��Ҫ��������ļĴ���*/
		onesize    = ((totalsize <= onesize) ? totalsize : onesize);
		/*��ʹ��ECC��ʱ��ʱ��Ҫ���ô˼Ĵ���,��ʾ���������*/
		NANDC_SET_REG_BITS(cluster->data_num,   onesize );
	}
	/*�������*/
	*total_size = totalsize;
	*one_size   = onesize;

	return ;
}

/**
* ����:����������
*
* ����:
* @host          		    --nandc������������ָ��
* @cmdval1          		--������
* @cmdval2          		--������
* @cmdval3          		--������

* ����:����������
*
*/
static inline void nandc_ctrl_set_cmd(struct nandc_host *host, u32 cmdval1, u32 cmdval2, u32 cmdval3)
{
    struct nandc_bit_cluster*  cluster;

    cluster = host->normal;
	/*дӲ���Ĵ�����ֵ*/
    NANDC_SET_REG_BITS (cluster->cmd,     cmdval1|(cmdval2<<8)|(cmdval3<<16) );

}

/**
* ����:�ȴ�״̬�Ĵ�����ֵΪvalue
*
* ����:
* @host          		    --nandc������������ָ��
* @status_bit          		--״̬�Ĵ���
* @value          		    --�ȴ������ֵΪvalue
* @timeout_ms          		--��ʱʱ��
*
* ����:�ȴ�״̬�Ĵ�����ֵΪvalue
*
*/
static inline u32 nandc_ctrl_register_wait(struct nandc_host *host, struct nandc_bit_reg* status_bit, u32 value, u32 timeout_ms)
{
	u32 count =   NANDC_MS_TO_COUNT(timeout_ms);

	do
	{
		/*�鿴״̬�Ĵ�����ֵ�Ƿ���value��ֵ���*/
		if(value == NANDC_REG_GETBIT32((u32)host->regbase + status_bit->reg_offset,
		                                                    status_bit->bitset->offset ,
		                                                    status_bit->bitset->length))
		{
		    return NANDC_OK;
		}
		count--;
	}
	while(count > 0);

	NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_ctrl_register_wait,timeout! \n"));/*lint !e778*/
	return NANDC_E_TIMEOUT;

}


/**
* ����:��nandc��״̬�Ĵ��������Ƿ����
*
* ����:
* @host          		    --nandc������������ָ��
*
*
* ����:��״̬�Ĵ���
*
*/
static inline u32 nandc_ctrl_read_result(struct nandc_host *host)
{
	struct nandc_bit_cluster*  cluster = NANDC_NULL;
	u32 errorcode = NANDC_ERROR;
	cluster = host->normal;

#ifdef NANDC_DEBUG
	/*nandc_ctrl_dump_reg(host, "nandc read result");*/
#endif
	/*��ѯ����״̬���ж��Ƿ����*/
	if(host->options & NANDC_OPTION_DMA_ENABLE)
	{
		errorcode = nandc_ctrl_register_wait(host, &cluster->dma_start, nandc_status_dma_done, NANDC_RD_RESULT_TIMEOUT);
	}
	else
	{
		errorcode = nandc_ctrl_register_wait(host, &cluster->op_stauts, nandc_status_op_done, NANDC_RD_RESULT_TIMEOUT);
	}

	if(NANDC_OK != errorcode)
	{
	    NANDC_TRACE(NFCDBGLVL(ERRO), ("wait nandc_ctrl_read_result!ERROR \n"));
	    goto ERRO;
	}

#ifdef NANDC_READ_RESULT_DELAY
	{
	    volatile u32 loopcnt = 500;
	    while(loopcnt--)
	    {};           /*wait 500 before read controler status (0x20 bit1)*/
	}
#endif
	if(host->bitcmd.ecc_err_valid == NANDC_GET_REG_BITS(cluster->ecc_result))
	{
		u32 ecc_warning_data = 0;
		ecc_warning_data = NANDC_GET_REG_BITS(cluster->errnum0buf0);
		NANDC_TRACE(NFCDBGLVL(WARNING), ("just ecc warning errnum0buf0  0x%x!\n",ecc_warning_data));
	}
    if(host->bitcmd.ecc_err_invalid == NANDC_GET_REG_BITS(cluster->ecc_result))
    {
        u32 ecc_error_data = 0;
        ecc_error_data = NANDC_GET_REG_BITS(cluster->errnum0buf0);
        if(host->force_readdata)
        {
            g_nand_status =0;
            NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_ctrl_read_result: ecc error invalid errnum0buf0  0x%x continu read data ecc!\n",ecc_error_data));/*lint !e778*/

        }
        else
        {
            g_nand_status = NAND_STATUS_FAIL;
            NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_ctrl_read_result: ecc error invalid errnum0buf0  0x%x!\n",ecc_error_data));/*lint !e778*/
            errorcode = NANDC_E_ECC;
            goto ERRO;
        }
    }
    else
    {
        g_nand_status = 0;
    }
	return NANDC_OK;

ERRO:
	NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_ctrl_read_result:error address[0]:0x%x, address[1]:0x%x!\n",
	                                    host->addr_physics[0],host->addr_physics[1]));
	return errorcode;
}

/**
* ����:nandcģ������ж�
*
* ����:
* @host          		    --nandc������������ָ��
* @value          		    --���nandcģ�����һЩ�ж�
*
* ����:���nandcģ����ж�
*
*/
static inline u32 nandc_ctrl_int_clear(struct nandc_host *host,  u32 value)
{
    struct nandc_bit_cluster*  cluster;
    cluster = (struct nandc_bit_cluster*)host->normal;

    NANDC_SET_REG_BITS (cluster->int_clr,     value);

    return NANDC_OK;
}


/**
* ����:ʹ��nandcģ����ж�
*
* ����:
* @host          		    --nandc������������ָ��
* @value          		    --ʹ��nandcģ����ж�
*
* ����:ʹ��nandcģ����ж�
*
*/
static inline u32 nandc_ctrl_int_set(struct nandc_host *host, u32 value)
{
    writel(value, (unsigned)(host->regbase + HI_NFC_INTEN_OFFSET));     /*lint !e124*/
    return NANDC_OK;
}

/**
* ����:��λnandflash�Ĳ���
*
* ����:
* @host          		    --nandc������������ָ��
*
* ����:��λnandflash�Ĳ���
*
*/
u32 nandc_ctrl_reset(struct nandc_host *host)
{
	register struct nandc_bit_cluster*  cluster;
	register struct nandc_bit_cmd*     ctrlcmd;
	volatile int reset_timeout = 10000;
	u32 errorcode = NANDC_ERROR;

	cluster = (struct nandc_bit_cluster*)host->normal;
	ctrlcmd = (struct nandc_bit_cmd*)&host->bitcmd;

	nandc_ctrl_set_cmd(host, NAND_CMD_RESET, NAND_CMD_NOP, NAND_CMD_NOP );
	nandc_ctrl_int_clear(host,                     ctrlcmd->intmask_all );

	NANDC_SET_REG_BITS (cluster->op_mode,          nandc_op_normal      );
	NANDC_SET_REG_BITS (cluster->ecc_select,       ctrlcmd->ecc_none    );
	/*�Ѵ˼Ĵ�����������bit��ֵ��дΪ��0*/
	NANDC_WRITE_REG_BITS(cluster->addr_cycle,      NANDC_ADDRCYCLE_0    );
	NANDC_SET_REG_BITS (cluster->chip_select,      host->chipselect     );
	nandc_ctrl_int_set(host, ctrlcmd->intdis_all); /* disable all interrupt */

	NANDC_SET_REG_BITS (cluster->operate,          ctrlcmd->op_reset    );

	g_nand_status = 0;

	/*�ȴ�nandflash�Ĳ������*/
	/*begin lint-Info 732: (Info -- Loss of sign (arg. no. 4) (int to unsigned int))*/
	errorcode = nandc_ctrl_register_wait(host, &cluster->int_stauts, nandc_status_int_done, (u32)reset_timeout);
	/*end*/
	if(NANDC_OK != errorcode)
	{
	    return NANDC_ERROR;
	}

	errorcode = nandc_ctrl_register_wait(host, &cluster->op_stauts, nandc_status_op_done, NANDC_RESULT_TIME_OUT);
	if(NANDC_OK != errorcode)
	{
	    return NANDC_ERROR;
	}

	return NANDC_OK;

}


/**
* ����:��nandflash��ID
*
* ����:
* @host          		    --nandc������������ָ��
*
* ����:��nandflash��ID
*
*/
u32 nandc_ctrl_read_id(struct nandc_host *host)
{
    register struct nandc_bit_cluster*  cluster;
    register struct nandc_bit_cmd*     ctrlcmd;

    u32 errorcode;

    cluster = (struct nandc_bit_cluster*)host->normal;
    ctrlcmd = (struct nandc_bit_cmd*)&host->bitcmd;

    nandc_ctrl_int_set(host, ctrlcmd->intdis_all);

    nandc_ctrl_reset(host);

    nandc_ctrl_set_cmd(host, NAND_CMD_READID, NAND_CMD_NOP, NAND_CMD_NOP);

	NANDC_SET_REG_BITS(cluster->addr_high,         0                   );
	NANDC_SET_REG_BITS(cluster->addr_low,          0                   );
	NANDC_SET_REG_BITS(cluster->ecc_type,          0                   );
	NANDC_SET_REG_BITS(cluster->ecc_select,        ctrlcmd->ecc_none   );


    nandc_ctrl_int_clear(host,ctrlcmd->intmask_all);

    NANDC_SET_REG_BITS(cluster->op_mode,           nandc_op_normal     );
    NANDC_SET_REG_BITS(cluster->page_size,         NANDC_PAGE_SIZE_2K_REG);
    NANDC_SET_REG_BITS(cluster->bus_width,         host->probwidth     );

    NANDC_WRITE_REG_BITS(cluster->addr_cycle,      NANDC_ADDRCYCLE_1   );
    NANDC_SET_REG_BITS(cluster->chip_select,       host->chipselect    );
    NANDC_SET_REG_BITS(cluster->operate,           ctrlcmd->op_read_id );

    errorcode = nandc_ctrl_register_wait(host, &cluster->int_stauts, nandc_status_int_done, NANDC_RESULT_TIME_OUT);
    if(NANDC_OK != errorcode)
    {
        goto ERRO;
    }

    errorcode = nandc_ctrl_register_wait(host, &cluster->op_stauts, nandc_status_op_done, NANDC_RESULT_TIME_OUT);
    if(NANDC_OK != errorcode)
    {
        goto ERRO;
    }
	/*��Ӳ���������е����ݶ����ڴ���ȥ*/
    if(NANDC_OK == errorcode)
    {
         nandc_data_transmit_raw(host, NANDC_READID_SIZE, 0, NAND_CMD_READSTART);
    }

    return errorcode;

ERRO:

    return NANDC_ERROR;

}

/**
* ����:ͨ��DMA�ķ�ʽ��������
*
* ����:
* @host          		    --nandc������������ָ��
*
* ����:��DMA�ķ�ʽ�������ݲ���
*
*/
u32 nandc_ctrl_read_dma(struct nandc_host *host)
{
	register struct nandc_bit_cluster*   cluster;
	register struct nandc_bit_cmd*       ctrlcmd;
	struct nandc_nand           *nand = host->nandchip;
	struct nandc_spec           *spec = &nand->spec;
	u32 sizeleft;
	/*���������ΪʲôҪ�þ�̬����*/
	/*�������totalsize��ʾ����һҳ�����ݵĴ�С��������һ�δ��͵����ݵ��ܴ�С*/
	static u32 totalsize, onesize, eccselect;
	u32 errorcode;


	cluster = host->normal;
	ctrlcmd = &host->bitcmd;

	nandc_ctrl_int_set(host, ctrlcmd->intdis_all); /* disable all interrupt */
	/*�Ƿ��Ѿ�д�˲��ֵ�Ӳ���Ĵ��������û��д�Ļ�Ҫдһ��*/
	if(nandc_read_latch_done != host->latched)
	{
		/*���������д�Ĵ�����ֵ*/
		nandc_ctrl_snapshot(host, &host->read_snapshot, &totalsize, &onesize, &eccselect  );

		if(nandc_read_latch_start == host->latched)
		{
			NANDC_TRACE(NFCDBGLVL(NORMAL), ("nandc_ctrl_read_dma, nandc_read_latch_start! \n"));
			host->latched = nandc_read_latch_done;
		}
	}
	else
	{
		NANDC_TRACE(NFCDBGLVL(NORMAL), ("nandc_ctrl_read_dma, nandc_read_latch_done, fast skip!\n"));
	}

	NANDC_SET_REG_BITS (cluster->addr_low,       host->addr_physics[0]		);
	NANDC_SET_REG_BITS (cluster->addr_high,      host->addr_physics[1]		);

	NANDC_SET_REG_BITS (cluster->base_addr_data,  (u32)host->databuf	);
	NANDC_SET_REG_BITS (cluster->base_addr_oob,   (u32)host->oobbuf		);


	NANDC_SET_REG_BITS (cluster->dma_ctrl,      ctrlcmd->dma_read	    );
	NANDC_SET_REG_BITS (cluster->int_clr,       ctrlcmd->intmask_all	);

	if(NANDC_BUFSIZE_BASIC >= spec->pagesize)
	{
		NANDC_SET_REG_BITS (cluster->dma_start,      0x1					);
	}
	else
	{
		NANDC_SET_REG_BITS (cluster->segment,     ctrlcmd->op_segment_start	);
		NANDC_SET_REG_BITS (cluster->dma_start,      0x1					);
	}

	/*��ѯ��ʽ�ȴ�nandflash�Ĳ������*/
	errorcode = nandc_ctrl_read_result(host);
	if(NANDC_OK != errorcode)
	{
		goto EXIT;
	}

	/*pagesie�Ĵ�С��nandc��Ӳ���������Ĵ�С����Ļ�*/
	sizeleft    =   totalsize - onesize;
	while(sizeleft > 0)
	{
		NANDC_SET_REG_BITS (cluster->int_clr,       ctrlcmd->intmask_all);
		sizeleft -= onesize;
		if(0 == sizeleft)
		{
			NANDC_SET_REG_BITS (cluster->segment,   ctrlcmd->op_segment_end);
			NANDC_SET_REG_BITS (cluster->dma_start,      0x1						);
		}
		else
		{
			NANDC_SET_REG_BITS (cluster->dma_start,      0x1						);
		}

		errorcode = nandc_ctrl_read_result(host);
		if(NANDC_OK != errorcode)
		{
			goto EXIT;
		}
	EXIT:
		return errorcode;

	}
	return NANDC_OK;

}

/**
* ����:ͨ�����ߵķ�ʽ��������
*
* ����:
* @host          		    --nandc������������ָ��
*
* ����:�����ߵķ�ʽ�������ݲ���
*
*/
u32 nandc_ctrl_read_normal(struct nandc_host *host)
{
	register struct nandc_bit_cluster*   cluster;
	register struct nandc_bit_cmd*       ctrlcmd;
	struct nandc_pagemap        *pagemap;
	struct nandc_nand           *nand = host->nandchip;
	struct nandc_spec           *spec = &nand->spec;
	u32 copy_offset = 0, read_times = 0, errorcode, sizeleft;
	static u32 totalsize, onesize, eccselect;

	cluster = host->normal;
	ctrlcmd = &host->bitcmd;

	if(nandc_read_latch_done != host->latched)
	{
		nandc_ctrl_snapshot(host, &host->read_snapshot, &totalsize, &onesize, &eccselect  );

		if(nandc_read_latch_start == host->latched)
		{
			NANDC_TRACE(NFCDBGLVL(NORMAL), ("nandc_ctrl_read_normal, nandc_read_latch_start! \n"));
			host->latched = nandc_read_latch_done;
		}
	}
	else
	{
		NANDC_TRACE(NFCDBGLVL(NORMAL), ("nandc_ctrl_read_normal, nandc_read_latch_done, fast skip!\n"));
	}

	NANDC_SET_REG_BITS(cluster->addr_low,       host->addr_physics[0]);
	NANDC_SET_REG_BITS(cluster->addr_high,      host->addr_physics[1]);
	NANDC_SET_REG_BITS(cluster->int_clr,        ctrlcmd->intmask_all );

	if(NANDC_BUFSIZE_BASIC >= spec->pagesize)
	{
		NANDC_SET_REG_BITS(cluster->operate,    ctrlcmd->op_read_start | ctrlcmd->op_read_end);
	}
	else
	{
		NANDC_SET_REG_BITS(cluster->segment,     ctrlcmd->op_segment_start);
		NANDC_SET_REG_BITS(cluster->operate,     ctrlcmd->op_read_start);
	}

	errorcode = nandc_ctrl_read_result(host);
	if(NANDC_OK == errorcode)
	{
		if((ctrlcmd->ecc_none == eccselect )||(ctrlcmd->ecc_oob == eccselect))
		{
			copy_offset = 0;
			nandc_data_transmit_raw(host, onesize, copy_offset, NAND_CMD_READSTART);
		}
		else
		{  /*whole page read with ECC mode*/
			pagemap =   host->nandchip->datamap; /* nandc_host_get_pagemap */
			read_times = 0;
			nandc_data_transmit_page(host, pagemap ,read_times, NAND_CMD_READSTART);
		}
	}
	else
	{
		goto EXIT;
	}

	/* for page size more than 8k bytes nand flash, for example, 16k bytes page size. */
	sizeleft    =   totalsize - onesize;
	while(sizeleft > 0)
	{
		NANDC_SET_REG_BITS (cluster->int_clr,       ctrlcmd->intmask_all);
		sizeleft -= onesize;
		if(0 == sizeleft)
		{
			NANDC_SET_REG_BITS (cluster->segment,   ctrlcmd->op_segment_end);
			NANDC_SET_REG_BITS (cluster ->operate,  ctrlcmd->op_read_end);
		}
		else
		{
			NANDC_SET_REG_BITS (cluster ->operate,  ctrlcmd->op_read_continue);
		}

		errorcode = nandc_ctrl_read_result(host);
		if(NANDC_OK == errorcode)
		{
			if(ctrlcmd->ecc_none== eccselect )
			{
				copy_offset +=  onesize;
				nandc_data_transmit_raw(host, onesize, copy_offset, NAND_CMD_READSTART);
			}
			else
			{
				read_times  +=  1;
				pagemap =   host->nandchip->datamap;   /* nandc_host_get_pagemap */
				nandc_data_transmit_page(host, pagemap ,read_times, NAND_CMD_READSTART);
			}
		}
		else
		{
			goto EXIT;
		}

	}
	return NANDC_OK;

	EXIT:
	return errorcode;
}


/**
* ����:��ѯnandc�������ı��״̬
*
* ����:
* @host          		    --nandc������������ָ��
* @size_left          		--��ʣ���ٸ��ֽڵ�Ҫ��
* ����:��ѯ��̵�״̬
*
*/
static inline u32 nandc_ctrl_program_result(struct nandc_host *host, u32 size_left)
{
	struct nandc_bit_cluster*  cluster = NANDC_NULL;
	u32 errorcode = NANDC_ERROR;
	u32 nand_status_timeout = NANDC_PRG_RESULT_TIMEOUT;

	cluster = host->normal;

#ifdef NANDC_DEBUG
	/* nandc_ctrl_dump_reg(host, "nandc program result"); */
#endif

	if(host->options & NANDC_OPTION_DMA_ENABLE)
	{
		errorcode = nandc_ctrl_register_wait(host, &cluster->dma_start, nandc_status_dma_done, NANDC_PRG_RESULT_TIMEOUT);
	}
	else
	{
		errorcode = nandc_ctrl_register_wait(host, &cluster->op_stauts, nandc_status_op_done, NANDC_PRG_RESULT_TIMEOUT);
	}
	if(NANDC_OK != errorcode)
	{
		NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_ctrl_program_result,wait return ERROR! \n"));/*lint !e778*/
		return errorcode;
	}
	if (0 == size_left)
	{
		while ((NAND_STATUS_FAIL ==  (NAND_STATUS_FAIL & NANDC_GET_REG_BITS(cluster->nf_status))) && (nand_status_timeout))
		{
			nand_status_timeout--;
		}

		g_nand_status = NANDC_GET_REG_BITS(cluster->nf_status);

		if (0 ==  nand_status_timeout)
		{
			NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_ctrl_program_result,status return FAIL! \n"));/*lint !e778*/
			return NANDC_E_TIMEOUT;
		}
	}
	return NANDC_OK;
}

/**
* ����:��ѯnandc�������Ĳ���״̬
*
* ����:
* @host          		    --nandc������������ָ��
*
* ����:��ѯ������״̬
*
*/
static inline u32 nandc_ctrl_erase_result(struct nandc_host *host)
{
	struct nandc_bit_cluster*  cluster = NANDC_NULL;
	u32 errorcode = NANDC_ERROR;
	u32 nand_status;

	cluster = host->normal;

#ifdef NANDC_DEBUG
	/* nandc_ctrl_dump_reg(host, "nandc erase result"); */
#endif
	/*�ȴ��˴β������*/
	errorcode = nandc_ctrl_register_wait(host, &cluster->op_stauts, nandc_status_op_done, NANDC_ERASE_RESULT_TIMEOUT);
	if(NANDC_OK != errorcode)
	{
		NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_ctrl_erase_result,wait return error! \n"));/*lint !e778*/
		goto ERRO;
	}

	if(nandc_status_int_done != NANDC_GET_REG_BITS(cluster->int_stauts))
	{
		errorcode = NANDC_E_ERASE;
		NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_ctrl_erase_result,int return error! \n"));/*lint !e778*/
		goto ERRO;
	}
	/*��nandflash��״̬���жϲ����Ƿ�ִ�гɹ�*/
	nand_status = NANDC_GET_REG_BITS(cluster->nf_status);
	g_nand_status = nand_status;

	if(NAND_STATUS_FAIL & nand_status)
	{
		NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_ctrl_erase_result,status return FAIL!:0x%x \n",nand_status));/*lint !e778*/
		errorcode = NANDC_E_ERASE;
		goto ERRO;
	}

	return NANDC_OK;

ERRO:
	return errorcode;
}

/**
* ����:nandc��������DMA��ʽ�ı��
*
* ����:
* @host          		    --nandc������������ָ��
*
* ����:��DMA�ķ�ʽ���
*
*/
u32 nandc_ctrl_program_dma(struct nandc_host *host)
{
	register struct nandc_bit_cluster*   cluster;
	register struct nandc_bit_cmd*       ctrlcmd;
	struct nandc_nand           *nand = host->nandchip;
	struct nandc_spec           *spec = &nand->spec;
	u32 sizeleft = spec->pagesize;
	static u32 totalsize, onesize, eccselect;
	u32 errorcode;


	cluster = host->normal;
	ctrlcmd = &host->bitcmd;

	nandc_ctrl_int_set(host, ctrlcmd->intdis_all); /* disable all interrupt */

	if(nandc_write_latch_done != host->latched)
	{
		nandc_ctrl_snapshot(host, &host->write_snapshot, &totalsize, &onesize, &eccselect  );

		if(nandc_write_latch_start == host->latched)
		{
		NANDC_TRACE(NFCDBGLVL(NORMAL), ("nandc_ctrl_program_normal, nandc_write_latch_start! \n"));
		host->latched = nandc_write_latch_done;
		}
	}
	else
	{
		NANDC_TRACE(NFCDBGLVL(NORMAL), ("nandc_ctrl_program_normal, nandc_write_latch_done, fast skip!\n"));
	}

	NANDC_SET_REG_BITS (cluster->base_addr_data,  (u32)host->databuf	);
	NANDC_SET_REG_BITS (cluster->base_addr_oob,   (u32)host->oobbuf		);

	NANDC_SET_REG_BITS (cluster->addr_low,      host->addr_physics[0]   );          /* nfc addrl */
	NANDC_SET_REG_BITS (cluster->addr_high,     host->addr_physics[1]   );          /* nfc addrh */
	NANDC_SET_REG_BITS (cluster->dma_ctrl,      ctrlcmd->dma_write		);          /* dma ctrl [1:12] */
	NANDC_SET_REG_BITS (cluster->int_clr,       ctrlcmd->intmask_all	);          /* interupt clear */

	if(NANDC_BUFSIZE_BASIC >= spec->pagesize)
	{
		NANDC_SET_REG_BITS (cluster->dma_start,      0x1						);
	}
	else if(totalsize == onesize)
	{
		NANDC_SET_REG_BITS(cluster->segment,     ctrlcmd->op_segment_start | ctrlcmd->op_segment_end);
		NANDC_SET_REG_BITS (cluster->dma_start,      0x1						);
	}
	else
	{
		NANDC_SET_REG_BITS(cluster->segment,     ctrlcmd->op_segment_start);
		NANDC_SET_REG_BITS (cluster->dma_start,      0x1						);
	}

	errorcode   = nandc_ctrl_program_result(host, sizeleft);
	if(NANDC_OK != errorcode)
	{
		goto EXIT;
	}
	sizeleft    =   totalsize - onesize;
	while(sizeleft > 0)
	{
		NANDC_SET_REG_BITS (cluster->int_clr,       ctrlcmd->intmask_all);
		sizeleft    -=  onesize;
		if(0 == sizeleft)
		{
			NANDC_SET_REG_BITS (cluster->segment,   ctrlcmd->op_segment_end);
			NANDC_SET_REG_BITS (cluster->dma_start,      0x1						);
		}
		else
		{
			NANDC_SET_REG_BITS (cluster->dma_start,      0x1						);
		}

		errorcode   = nandc_ctrl_program_result(host, sizeleft);
		if(NANDC_OK != errorcode)
		{
			goto EXIT;
		}

	EXIT:
		return errorcode;
	}

	return NANDC_OK;
}


/**
* ����:nandc�����������߷�ʽ�ı��
*
* ����:
* @host          		    --nandc������������ָ��
*
* ����:�����ߵķ�ʽ���
*
*/
u32 nandc_ctrl_program_normal(struct nandc_host *host)
{
	register struct nandc_bit_cluster*   cluster;
	register struct nandc_bit_cmd*       ctrlcmd;
	struct nandc_pagemap        *pagemap;
	struct nandc_nand           *nand = host->nandchip;
	struct nandc_spec           *spec = &nand->spec;
	u32 copy_offset = 0, write_times = 0, errorcode, sizeleft;
	static u32 totalsize, onesize, eccselect;

	cluster = host->normal;
	ctrlcmd = &host->bitcmd;
	if(nandc_write_latch_done != host->latched)
	{
		nandc_ctrl_snapshot(host, &host->write_snapshot, &totalsize, &onesize, &eccselect  );

		if(nandc_write_latch_start == host->latched)
		{
			NANDC_TRACE(NFCDBGLVL(NORMAL), ("nandc_ctrl_program_normal, nandc_write_latch_start! \n"));
			host->latched = nandc_write_latch_done;
		}
	}
	else
	{
		NANDC_TRACE(NFCDBGLVL(NORMAL), ("nandc_ctrl_program_normal, nandc_write_latch_done, fast skip!\n"));
	}

	NANDC_SET_REG_BITS (cluster->addr_low,      host->addr_physics[0]   );
	NANDC_SET_REG_BITS (cluster->addr_high,     host->addr_physics[1]   );
	NANDC_SET_REG_BITS (cluster->int_clr,       ctrlcmd->intmask_all	);

	if((ctrlcmd->ecc_none== eccselect )||(ctrlcmd->ecc_oob== eccselect))
	{
		copy_offset =   0;
		nandc_data_transmit_raw(host, onesize, copy_offset, NAND_CMD_PAGEPROG);
	}
	else
	{   /* whole page write with ECC mode */
		pagemap     =   host->nandchip->datamap;  /* nandc_host_get_pagemap */
		write_times =   0;
		nandc_data_transmit_page(host, pagemap ,write_times, NAND_CMD_PAGEPROG);
	}

	if(NANDC_BUFSIZE_BASIC >= spec->pagesize)
	{
		NANDC_SET_REG_BITS(cluster ->operate,    ctrlcmd->op_write_start | ctrlcmd->op_write_end);
	}
	else if(totalsize == onesize)
	{
		NANDC_SET_REG_BITS(cluster->segment,     ctrlcmd->op_segment_start | ctrlcmd->op_segment_end);
		NANDC_SET_REG_BITS(cluster ->operate,    ctrlcmd->op_write_start | ctrlcmd->op_write_end);
	}
	else
	{
		NANDC_SET_REG_BITS(cluster->segment,     ctrlcmd->op_segment_start);
		NANDC_SET_REG_BITS(cluster->operate,     ctrlcmd->op_write_start);
	}

	sizeleft    =   totalsize - onesize;
	errorcode   = nandc_ctrl_program_result(host, sizeleft);
	if(NANDC_OK != errorcode)
	{
		goto EXIT;
	}

	while(sizeleft > 0)
	{

		if(ctrlcmd->ecc_none== eccselect )
		{
			copy_offset += onesize;
			nandc_data_transmit_raw(host, onesize, copy_offset, NAND_CMD_PAGEPROG);
		}
		else
		{
			write_times +=  1;
			pagemap =   host->nandchip->datamap;  /* nandc_host_get_pagemap */
			nandc_data_transmit_page(host, pagemap ,write_times, NAND_CMD_PAGEPROG);
		}

		NANDC_SET_REG_BITS (cluster->int_clr,       ctrlcmd->intmask_all);
		sizeleft    -=  onesize;
		if(0 == sizeleft)
		{
			NANDC_SET_REG_BITS (cluster->segment,   ctrlcmd->op_segment_end);
			NANDC_SET_REG_BITS (cluster ->operate,  ctrlcmd->op_write_end);
		}
		else
		{
			NANDC_SET_REG_BITS (cluster ->operate,  ctrlcmd->op_read_continue);
		}

		errorcode = nandc_ctrl_program_result(host,sizeleft);
		if(NANDC_OK != errorcode)
		{
			goto EXIT;
		}
	}

	return NANDC_OK;

EXIT:
	return errorcode;
}


/**
* ����:nandflash�Ĳ�������
*
* ����:
* @host          		    --nandc������������ָ��
*
* ����:����nandflash�Ŀ�
*
*/
u32 nandc_ctrl_erase_block(struct nandc_host *host)
{
    register struct nandc_bit_cluster    *cluster;
    register struct nandc_bit_cmd        *ctrlcmd;
    struct nandc_nand           *nand = host->nandchip;
    struct nandc_spec           *spec = &nand->spec;
    u32 addrcycle;
    u32 count = 3;
    u32 errocode = NANDC_ERROR;

    ctrlcmd = (struct nandc_bit_cmd*)&host->bitcmd;
    cluster = host->normal;

    addrcycle = spec->addrcycle - 2;

    nandc_ctrl_int_set(host, ctrlcmd->intdis_all); /* disable all interrupt */

    if(0 == host->addr_physics[0])
    {
        NANDC_TRACE(NFCDBGLVL(WARNING), ("nandc_ctrl_earse_block: erase blknum 0.\n"));
    }

    nandc_ctrl_set_cmd  (host,      NAND_CMD_ERASE1, NAND_CMD_ERASE2, NAND_CMD_STATUS);
    nandc_ctrl_int_clear(host,      ctrlcmd->intmask_all                );

    NANDC_SET_REG_BITS(cluster->addr_low,       host->addr_physics[0]   );
    NANDC_SET_REG_BITS(cluster->ecc_select,     ctrlcmd->ecc_none       );
    NANDC_SET_REG_BITS(cluster->ecc_type,       0                       );

    NANDC_WRITE_REG_BITS(cluster->addr_cycle,   addrcycle			    );
    NANDC_SET_REG_BITS(cluster->chip_select,    host->chipselect        );
	/*Ҫ�ظ����������*/
    while((NANDC_OK != errocode)&&(count > 0))
    {
        NANDC_SET_REG_BITS(cluster->operate,        ctrlcmd->op_erase   );
        errocode =  nandc_ctrl_erase_result(host);
        count--;
    }

    if(NANDC_OK != errocode)
    {
       NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_ctrl_earse_block, failed 3 times!\n"));/*lint !e778*/
       return errocode;
    }

    return NANDC_OK;
}


/**
* ����:nandflash�����������
*
* ����:
* @host          		    --nandc������������ָ��
*
* ����:nandflash�����������,���е������ִ�ж��ڴ˺�����
*
*/
u32 nandc_ctrl_entry(struct nandc_host *host)
{
    u32 errorcode = NANDC_ERROR ;
	/*��������֮����ظ����Ĵ���*/
    u32 try_left = NANDC_RD_RETRY_CNT;
    volatile u32 delay_loop;

    NANDC_TRACE(NFCDBGLVL(NORMAL), ("(0x%x)nandc_ctrl_entrycs(%d),real(0x%x), low addr:0x%x, high addr:0x%x\n",
                                               host->command,  host->chipselect, (unsigned int)host->addr_real, host->addr_physics[0],host->addr_physics[1]));
TRY_AGAIN1:

    switch (host->command)
    {
        case NAND_CMD_READSTART:
            if(host->options & NANDC_OPTION_DMA_ENABLE)
            {
                errorcode = nandc_ctrl_read_dma(host);
            }
            else
            {
                errorcode = nandc_ctrl_read_normal(host);
            }
        break;

        case NAND_CMD_READID:
            errorcode = nandc_ctrl_read_id(host);
        break;

        case NAND_CMD_PAGEPROG:

            if(host->options & NANDC_OPTION_DMA_ENABLE)
            {
               errorcode =  nandc_ctrl_program_dma(host);
            }
            else
            {
               errorcode =  nandc_ctrl_program_normal(host);
            }
        break;

        case NAND_CMD_ERASE2:

            errorcode = nandc_ctrl_erase_block(host);

        break;

        case NAND_CMD_SEQIN:
        case NAND_CMD_ERASE1:
        case NAND_CMD_READ0:
        break;

        case NAND_CMD_RESET:
			errorcode = nandc_ctrl_reset(host);
        break;

        default :
            errorcode = NANDC_ERROR;
            NANDC_DO_ASSERT(0, "unknown nand command!", host->command);
        break;
    }

    switch (host->command)
    {
        case NAND_CMD_READSTART:
        {
            if((NANDC_OK != errorcode)&&(try_left))
            {
                try_left--;
                delay_loop = 100000;
                while(delay_loop--)
                {};
                goto TRY_AGAIN1;
            }
        }
        break;
        default:
        break;
    }

    return errorcode;


}

/**
* ����:��ӡnandc�������Ĵ�����ֵ
*
* ����:
* @headstr          		    --�ַ���ͷ
*
* ����:������ڵ��ԵĹ�������������˴�����԰Ѵ˴��������ԼĴ�����ֵ
*
*/
extern struct nandc_host*  nandc_nand_host;
void nandc_ctrl_dump_reg(char* headstr)
{
    struct nandc_host *host = nandc_nand_host;
    void __iomem * dumpaddr = host->regbase;
    u32 regsize =host->regsize;
    u32  i=0;
    u32* buf = NULL;

    buf = (u32 *)himalloc(regsize+0x20);
    if(!buf)
    {
        NANDC_TRACE(NFCDBGLVL(ERRO),("nandc_ctrl_dump_reg Line %d!ERROR \n",(int)__LINE__));
        return;
    }
    memset(buf, 0, regsize+0x20);
    memcpy(buf,dumpaddr,regsize);
    if(headstr)
    {
        NANDC_TRACE(NFCDBGLVL(ERRO),("%s\n",headstr));
    }
    NANDC_TRACE(NFCDBGLVL(ERRO),("nandc_ctrl_dump_reg start:\n"));
    /*���ܻ���һЩֵ����,��regsize����32��������ʱ*/
    for(i=0; i<regsize/4; i+=4)
    {
        NANDC_TRACE(NFCDBGLVL(ERRO),("dump reg:\n0x%x 0x%x 0x%x 0x%x \n",
        *(buf+0),*(buf+1),*(buf+2),*(buf+3)));
        buf+=4;
    }
    NANDC_TRACE(NFCDBGLVL(ERRO),("nandc_ctrl_dump_reg end:\n"));
    hifree(buf);

}

/*lint -restore */

#ifdef COMPILE_BOOTLOADER
u32 nandc_io_bitwidth_set(u32 bitwidth)
{
    u32 ioshare_val;

    /*���üĴ���*/
    if(NAND_BUSWIDTH_16 == bitwidth)
    {
        ioshare_val = 1;
    }
    else    /* 8bit NAND,���������� */
    {
        ioshare_val = 0;
        /* 2_24, 2_25, 2_27, 2_28 ����*/
	    NANDC_REG_WRITE32(INTEGRATOR_SC_BASE + 0x08B0, 0x02000200);
	    NANDC_REG_WRITE32(INTEGRATOR_SC_BASE + 0x08B4, 0x02000000);
	    NANDC_REG_WRITE32(INTEGRATOR_SC_BASE + 0x08B8, 0x00000200);
    }
    NANDC_REG_SETBIT32(INTEGRATOR_SC_BASE + INTEGRATOR_SC_IOS_CTRL98, 2, 1, ioshare_val);
    return NANDC_OK;
}
#endif

#ifdef __cplusplus
}
#endif

