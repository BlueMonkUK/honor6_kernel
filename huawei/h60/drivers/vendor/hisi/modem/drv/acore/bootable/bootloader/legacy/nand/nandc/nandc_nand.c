/*******************************************************************************
* Copyright (C), 2008-2011, HUAWEI Tech. Co., Ltd.
*
* Module name: Hisilicon Nand Flash Controller Dirver
*
*
* Filename:    nandc_nand.c
* Description: nandcģ���ʵ�ֶ���д���ݡ��������ܡ��껵�鶼�ڴ��ļ���ʵ��
*
*******************************************************************************/
/******************************************************************************
*    Copyright (c) 2009-2011 by  Hisilicon Tech. Co., Ltd.
*
*    All rights reserved.
*
*
******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __FASTBOOT__

/*lint -save -e506 */
#include "nandc_inc.h"
#include "nandc_balong.h"
#include "hi_base.h"
#include "hi_syscrg_interface.h"
#include "hi_syssc_interface.h"
#include <bsp_shared_ddr.h>
#include <bsp_nandc.h>
extern u32 ptable_parse_mtd_partitions(struct mtd_partition** mtd_parts, u32 *nr_parts);
extern u32  nandc_host_set_partition(struct nandc_host * host, struct mtd_partition* ptable, u32 nr_parts);
/*lint -save -e767*/
#define NFCDBGLVL(LVL)      (NANDC_TRACE_NAND|NANDC_TRACE_##LVL)
/*lint -restore*/
/*ȫ��ָ�����*/
struct nandc_host*  nandc_nand_host = NANDC_NULL;



/**
* ����:��Ӳ���������е�OOB���ݸ��Ƶ��û��ڴ���ȥ
*
* ����:
* @host                  ---nandc_host�Ľṹ��ָ��
* @oobdst                ---�û����OOB���ݵ��ڴ��ַ
* @oobsrc                ---Ӳ���������е�OOB���ݵĵ�ַ
* @len                   ---���Ƶĳ���
* ����:��Ϊ��Ӳ���е�OOB�����ݸ�ʽ���û���Ҫ��OOB���ݸ�ʽ��һ��,�����Ҫһ����ʽת���ĸ��Ʋ���
*/
__inline u32 nandc_nand_transfer_oob(struct nandc_host* host, u8 *oobdst,  u8 *oobsrc, u32 len)
{
    struct nand_oobfree *free  ;    /*lint !e578 */
    u32 boffs = 0;
    u32 bytes = 0;

    free =  host->curpart->ecclayout->oobfree;

    for(; free->length && len; free++, len -= bytes)
    {
        bytes = min_t(u32, len, free->length);
        boffs = free->offset;

        memcpy((void*)oobdst, (const void*)(oobsrc + boffs), bytes);
        oobdst += bytes;
    }

    return NANDC_OK;
}


/**
* ����:�û�дOOB�����ݵ�nandc��������buf��ȥ
*
* ����:
* @host                  ---nandc_host�Ľṹ��ָ��
* @oobdst                ---Ӳ���������е�OOB���ݵĵ�ַ
* @oobsrc                ---�û����OOB���ݵ��ڴ��ַ
* @len                   ---���Ƶĳ���
* ����:��Ϊ��Ӳ���е�OOB�����ݸ�ʽ���û���Ҫ��OOB���ݸ�ʽ��һ��,�����Ҫһ����ʽת���ĸ��Ʋ���
*/
__inline u32 nandc_nand_fill_oob(struct nandc_host* host, u8 *oobdst,  u8 *oobsrc, u32 len)
{
    struct nand_oobfree *free  ;    /*lint !e578 */
    u32 boffs = 0;
    u32 bytes = 0;

    free =  host->curpart->ecclayout->oobfree;

    for(; free->length && len; free++, len -= bytes)
    {
        bytes = min_t(u32, len, free->length);
        boffs = free->offset;

        memcpy((void *)((u32)oobdst + boffs), (const void*)oobsrc , bytes);
        oobsrc += bytes;
    }

    return NANDC_OK;
}

/**
* ����:nandcģ�����ô���Ĺ���
*
* ����:
* @host                  ---nandc_host�Ľṹ��ָ��
* @access                ---��д�ı�־ֵ
* ����:����nandcģ��Ĵ������
*/
__inline void nandc_nand_set_latch(struct nandc_host* host, u32 access)
{
	/*lint !e578 */
	/*������*/
	if(NAND_CMD_READSTART == access)
	{
		if(nandc_read_latch_done != host->latched)
		{
			host->latched = nandc_read_latch_start;
		}
	}
	/*д����*/
	else if(NAND_CMD_PAGEPROG == access)
	{
		if(nandc_write_latch_done != host->latched)
		{
			host->latched = nandc_write_latch_start;
		}
	}
}

/**
* ����:nandcģ���������Ĺ���
*
* ����:
* @host                  ---nandc_host�Ľṹ��ָ��
* ����:���nandcģ��Ĵ������
*/
__inline void nandc_nand_clear_latch(struct nandc_host* host)
{
    host->latched = nandc_latch_none;
}

/**
* ����:nandcģ��Flash��д��ַת���Ĺ���
*
* ����:
* @host                  ---nandc_host�Ľṹ��ָ��
* ����:nandcģ��Ķ�дFlash��ַ��ת������
*/
__inline u32 nandc_nand_set_address(struct nandc_host *host)
{
    struct nandc_spec* spec = &host->nandchip->spec;
    u32 page_offset, column_offset;
	u32 column_addr;

    if(NANDC_ADDR_INVALID == host->addr_real)
    {
		host->addr_real = 0;
    }
    else
    {
        /*�ѵ�ַת��Ϊҳ��ַ��ҳ�ڵ�ַ*/
        column_addr= (u32)host->addr_real & (spec->offinpage);

        if(NAND_BUSWIDTH_16 == spec->buswidth)
        {
           column_offset = column_addr >> 1;
        }
        else
        {
           column_offset = column_addr;
        }

        page_offset = (u32)(host->addr_real >> spec->pageshift);
		/*ת��ΪҪд�뵽�Ĵ����ĵ�ֵַ*/
        host->addr_physics[0] = column_offset;
        host->addr_physics[0] |= page_offset << NANDC_COLUMN_SHIFT;

        host->addr_physics[1] = page_offset >> NANDC_COLUMN_SHIFT;

    }

    NANDC_DO_ASSERT(((u32)host->addr_real < (u32)spec->chipsize), "virtual address too large!", (u32)host->addr_real);

    return NANDC_OK;
}


/**
* ����:nandcģ��Flash����д�������ļĴ���ʵ�ֹ���
*
* ����:
* @host                  ---nandc_host�Ľṹ��ָ��
* @databuff              ---���ݵ�ַ
* @oobbuff               ---OOB��ַ
* @datalen               ---���ݳ���
* @ooblen                ---OOB�ĳ���
* @access                ---����д����������
* ����:nandcģ��Ķ�дFlash��ַ��ת������
* ע��:
* < ECC mode select by parameter combination: >
*-------------------------------------------------------------------------------------------------
*  | @databuff    |  @oobbuff  |  @datalen | @ooblen  |   ECC mode
*  ------------------------------------------------------------------------------------------------
*  |    YES       |    ANY     |  YES      |  ANY     |   no ECC random
*  ------------------------------------------------------------------------------------------------
*  |    YES       |    NULL    |  NULL     |  ANY     |   data only with ECC
*  ------------------------------------------------------------------------------------------------
*  |    YES       |    YES     |  NULL     |  YES     |   data and spare with ECC
*  ------------------------------------------------------------------------------------------------
*  |    NULL      |    YES     |  NULL     |  YES     |   spare only with ECC
*  ------------------------------------------------------------------------------------------------
*  |  O T H E R S                                     |   INVALID
*  ------------------------------------------------------------------------------------------------
*/
__inline u32 nandc_nand_access(struct nandc_host* host, u8* databuff, u8* oobbuff, u32 datalen, u32 ooblen, u32 access)
{
	/*lint !e578 */
    u32 errorcode = NANDC_ERROR;
	/*����Ҫת���ĵ�ַ��Ϣ*/
    if(NANDC_OK != nandc_nand_set_address(host))
    {
        goto ERRO;
    }
	/*��д����������ͨ�ô�������*/
    switch(access)
    {
        case NAND_CMD_PAGEPROG:
        case NAND_CMD_READSTART:
        {
            NANDC_REJECT_NULL((u32)databuff|(u32)oobbuff);
			/*������ʹ��ECC���ܵĶ�д���ݲ���*/
            if(0 == datalen)
            {
            	/*ecc mode,read whole page data and oob data*/
                host->databuf   = databuff;
#ifdef NANDC_USE_DMA_ENABLE
				host->options   =   NANDC_OPTION_DMA_ENABLE;
#else
				host->options   =   0 ;
#endif
				/*��ֱ�Ӱ�host>oobbuf����Ϊoobbuf��ԭ������Ϊ���ݵ������ǲ�һ����*/
				NANDC_REJECT_NULL(host->buffers);
				host->oobbuf    = host->buffers->ecccode;

                if(host->addr_physics[0] & 0xFFFF)
                {
                    errorcode = NANDC_ERROR;
                    NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_nand_access: invalide ecc access address:(%d) \n", host->addr_physics[0]));
                    goto ERRO;
                }
            }
            else
            {   /*no ecc mode, raw data in one page*/
                host->databuf   =   databuff;
				host->oobbuf	=	oobbuff;

                host->options   =   NANDC_OPTION_ECC_FORBID;             /*use default ecc type */
#ifdef NANDC_USE_DMA_ENABLE
				host->options   |=  NANDC_OPTION_DMA_ENABLE ;
#endif
                host->length    =   datalen;
            }
			/*�����д��������Ҫд��OOB������ô��дOOB����*/
            if((NAND_CMD_PAGEPROG == access)&&(NANDC_NULL != oobbuff))
            {
                nandc_nand_fill_oob(host, host->oobbuf, oobbuff, ooblen);
            }
        }
        break ;
        case NAND_CMD_ERASE2:
        {
			/*Ҫ������ҳ��ַ*/
            host->addr_physics[0]   = (host->addr_physics[0] >> NANDC_COLUMN_SHIFT) | (host->addr_physics[1] << NANDC_COLUMN_SHIFT);
        }
        break;
        default:
			errorcode = NANDC_ERROR;
            NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_nand_access: invalide access type:(%d) \n", access));
            goto EXIT;
    }

    host->command = access;
    errorcode = nandc_ctrl_entry(host);
    if(NANDC_OK !=  errorcode)
    {
        goto EXIT;
    }
	/*����Ƕ�������Ҫ��OOB��������ô�Ͱ�OOB�����ݶ�����*/
    if((NAND_CMD_READSTART == access)&&(NANDC_NULL != oobbuff))
    {
        nandc_nand_transfer_oob(host, oobbuff, host->oobbuf, ooblen);
    }

EXIT:
    return errorcode;

ERRO:
    return NANDC_ERROR;
}


/**
* ����:nandcģ�鲻ʹ��ECC���ܵ��������
*
* ����:
* @pagenum                ---Flashҳ��
* @offsetinpage           ---Flash��ҳ��ƫ�Ƶ�ַ
* @dst_addr               ---������ݵ�Ŀ�ĵ�ַ
* @readsize               ---�������ݳ���
* ����:nandcģ�鲻ʹ��ECC���������
*/
static u32 nandc_nand_read_raw(u32 pagenum, u32 offsetinpage, u32 dst_addr, u32 readsize)
{
    struct nandc_host* host = nandc_nand_host ;
    struct nandc_spec* spec;
    int ret = NANDC_ERROR;

    spec = &host->nandchip->spec ;
	/*Ҫ����Flash��ַת��*/
    host->addr_real= (pagenum & spec->pagemask) << spec->pageshift;

    host->chipselect = pagenum >> (spec->chipshift - spec->pageshift);
	/*����ҳ��pagedata+sparesize����һ�������buffer����*/
    if(NANDC_NULL == host->buffers)
    {
        /* coverity[printf_arg_mismatch] */
        NANDC_TRACE(NFCDBGLVL(ERRO), ("read raw error!! host->buffers is NULL \n"));
		goto ERRO;
    }
	if((offsetinpage + readsize) > (spec->pagesize + spec->sparesize))
    {
        /* coverity[printf_arg_mismatch] */
        NANDC_TRACE(NFCDBGLVL(ERRO), ("read raw error!! offsetinpage 0x%x,readsize 0x%x \n",offsetinpage,readsize));
		goto ERRO;
    }
	/*��ʹ��ECC���ܵ�������ݲ���,ע���4��������Ϊ0Ϊpagesize+sparesize*/
    ret = nandc_nand_access(host, host->buffers->databuf, host->buffers->databuf + spec->pagesize, spec->pagesize + spec->sparesize, 0, NAND_CMD_READSTART);
    if(NANDC_OK == ret)
    {
    	/*�ٰѶ����������ݸ��Ƶ��û��ڴ���ȥ*/
        memcpy((void*)dst_addr, host->buffers->databuf + offsetinpage,  readsize);
    }
    else
    {
         goto ERRO;
    }
    return NANDC_OK;
ERRO:
    return ret;
}



/**
* ����:nandcģ�鲻ʹ��ECC���ܵĶ�����ҳ�������,��Ҫ���ṩ��usbload.c��ʹ��
*
* ����:
* @flash_addr             ---Flash��ַ
* @dst_addr               ---������ݵ�Ŀ�ĵ�ַ
* @readsize               ---�������ݳ���
* ����:��Ҫ�Ǹ���pc����ͨ��fastboot.exe������һ��ҳ��������
*/
static u32 nandc_read_raw_wholepage(FSZ flash_addr, u32 dst_data, u32 read_size)
{
    struct nandc_host* host = nandc_nand_host ;
    struct nandc_spec* spec;
    int ret = NANDC_ERROR;
    u32 pageoffset=0,pagenum=0;
    u32 raw_whole_pagesize=0;

    spec = &host->nandchip->spec ;
    pageoffset  =   (u32)(flash_addr & spec->offinpage);
    pagenum     =   flash_addr >> spec->pageshift;
    raw_whole_pagesize = spec->pagesize + spec->sparesize;
	if(( pageoffset != 0 ) ||(read_size != raw_whole_pagesize))
    {
        /* coverity[printf_arg_mismatch] */
        NANDC_TRACE(NFCDBGLVL(ERRO), ("read raw error!! flash_addr 0x%x,read_size 0x%x \n",flash_addr,read_size));
		goto ERRO;
    }
    return nandc_nand_read_raw(pagenum,0,dst_data,read_size);
ERRO:
    return ret;
}



/**
* ����:nandcģ��ʹ��ECC���ܵĶ���ҳ������
*
* ����:
* @address                ---Flash��ַ
* @databuff           	  ---�����ݴ�ŵ��ڴ��ַ
* @oobbuff                ---OOB���ݴ�ŵ��ڴ��ַ
* @oobsize                ---Ҫ����OOB���ݳ���
* ����:nandcģ��ʹ��ECC�Ķ���ҳ���ݣ�ͨ��OOBBUFF�������Ƿ�Ҫ��OOB����
*/
static u32 nandc_nand_read_page(FSZ address, u8* databuff, u8* oobbuff, u32 oobsize)
{
    struct nandc_host* host = nandc_nand_host ;

    NANDC_REJECT_NULL(host);
	/*���õ�ַ��Ϣ*/
    if(NANDC_OK != nandc_native_location_by_address( host, address))
    {
        return NANDC_E_NOFIND;
    }

    host->addr_real &= host->nandchip->spec.pagealign;
	/*ʹ��ECC���ܵĶ�һҳ����,ע���4��������Ϊ0*/
    return nandc_nand_access(host, databuff,  oobbuff, 0, oobsize, NAND_CMD_READSTART);

ERRO:
	return NANDC_E_NULL;
}

/**
* ����:nandcģ��ʹ��ECC���������ecc errorʱҲ�������ҳ������ݶ�����,��Ҫ���ṩ��usbload.c��ʹ��
*
* ����:
* @flash_addr             ---Flash��ַ
* @dst_addr               ---������ݵ�Ŀ�ĵ�ַ
* @readsize               ---�������ݳ���
* ����:��Ҫ�Ǹ���pc����ͨ��fastboot.exe������һ��ҳ������
*/
static u32 nandc_read_force_wholepage(FSZ flash_addr, u32 dst_data, u32 read_size)
{
    struct nandc_host* host = nandc_nand_host ;
    struct nandc_spec* spec;
    u32 ret = NANDC_ERROR;
    u32 pageoffset=0;
    u32 whole_pagesize=0;

    spec = &host->nandchip->spec ;
    pageoffset  =   (u32)(flash_addr & spec->offinpage);
    whole_pagesize = spec->pagesize;
    if((pageoffset != 0 ) ||(read_size != whole_pagesize))
    {
        /* coverity[printf_arg_mismatch] */
        NANDC_TRACE(NFCDBGLVL(ERRO), ("read raw error!! flash_addr 0x%x,read_size 0x%x \n",flash_addr,read_size));
        goto EXIT;
    }
    host->force_readdata =1;
    ret = nandc_nand_read_page(flash_addr,(u8*)dst_data,NULL,0);
    host->force_readdata =0;
EXIT:
    return ret;
}


/**
* ����:nandcģ���ṩ��usbloader.c�Ľӿڣ�����ecc errorʱ����ͨ��fastboot.exe��pc����dump flash������
*
* ����:
* @flash_addr             ---Flash��ַ
* @dst_addr               ---������ݵ�Ŀ�ĵ�ַ
* @readsize               ---�������ݳ���
* @ecc_enable             ---eccʹ��
* ����:��Ҫ�Ǹ���pc����ͨ��fastboot.exe������һ��ҳ������
*/
u32 nand_read_wholepage_usbloader(FSZ flash_addr, u32 dst_data, u32 read_size,unsigned int ecc_enable)
{
    if(ecc_enable==1)
    {
        return nandc_read_force_wholepage(flash_addr,dst_data,read_size);
    }
    else if(ecc_enable==0)
    {
        return nandc_read_raw_wholepage(flash_addr,dst_data,read_size);
    }
    /* coverity[printf_arg_mismatch] */
    NANDC_TRACE(NFCDBGLVL(ERRO), ("nand_read_wholepage_usbloaderw error!! ecc_enable 0x%x \n",ecc_enable));
    return NANDC_ERROR;
}

/**
* ����:nandcģ��ͨ����ź�ҳ��ַ������Ҫ��дFlash�ĵ�ַ��Ϣ
*
* ����:
* @host                   ---nandc_host�Ľṹ��ָ��
* @blk_id           	  ---Ҫ����Flash�Ŀ��
* @pageoffset             ---Ҫ����blk_id�Ŀ���ҳ���ַ
*/
static __inline u32 nandc_nand_flag_address(struct nandc_host* host, u32 blk_id, u32 pageoffset)
{
    u32 flagoffset;
	/*���ÿ��ַ*/
    if(NANDC_OK != nandc_native_location_by_blocknum( host, blk_id))
    {
        goto ERRO;
    }
	/*���ÿ���ҳ���ַ*/
    host->addr_real +=  pageoffset << host->nandchip->spec.pageshift;

    if(NANDC_OK != nandc_nand_set_address(host))
    {
        goto ERRO;
    }
	/*spare���Ļ�������ƫ�Ƶ�ַ*/
    flagoffset = host->badoffset;
    /* coverity[printf_arg_mismatch] */
	NANDC_TRACE(NFCDBGLVL(NORMAL),("blk_id:0x%x,pageoffset:0x%x\n",blk_id,pageoffset));

    if(NAND_BUSWIDTH_16 == host->nandchip->spec.buswidth)
    {
        host->addr_physics[0] += flagoffset>>1;
    }
    else
    {
        host->addr_physics[0] += flagoffset;
    }
    host->addr_real = NANDC_ADDR_INVALID;
    host->addr_physics[0] &= 0xFFFF0000;

    return NANDC_OK;

ERRO:
    return NANDC_ERROR;
}


/**
* ����:nandcģ��ͨ����źͿ��ڵ�ҳƫ�Ƶ�ַ����OOB����
*
* ����:
* @blk_id                   ---Flash���
* @pageoffset           	---���ڵ�ҳƫ�Ƶ�ַ
* @oobbuf             		---OOB��ŵ��ڴ��ַ
* @size             		---Ҫ����OOB�Ĵ�С
*/
u32 nandc_nand_quary_flag(u32 blk_id, u32 pageoffset, u8* oobbuf ,u32 size)
{
    struct nandc_host* host = nandc_nand_host ;

    NANDC_REJECT_NULL(host);

	struct nandc_spec	*spec 	 = &(host->nandchip->spec);
	u32 sparesize 	= spec->sparesize;
	u32 pagesize  	= spec->pagesize;
    u32 errcode     = NANDC_ERROR;
	u8 *databuf 	= host->buffers->databuf;
	/*���õ�ַ*/
    if(NANDC_OK ==nandc_nand_flag_address(host, blk_id, pageoffset))
    {
    	/*��ʹ��ECC���ܵĶ�Flash�е�����*/
    	errcode = nandc_nand_access(host, (u8*)databuf, (u8*)(databuf + pagesize), pagesize + sparesize, 0, NAND_CMD_READSTART);
        if(NANDC_OK == errcode)
        {
        	/*��OOB�����ݸ��Ƴ���*/
            memcpy((void*)oobbuf, (const void*)(databuf + pagesize), size);
            return errcode;
        }
        else
        {
            goto ERRO;
        }
    }

 ERRO:
     return NANDC_ERROR;
}


/**
* ����:nandcģ��ͨ���������ѯ�˿��Ƿ��ǻ���
*
* ����:
* @blk_id                   ---Flash���
* @isbad           	        ---��Ż����־�ĵ�ַ
*/
static u32 nandc_nand_quary_bad(u32 blk_id, u32 * isbad )
{
    u8 badflag[NANDC_BADBLOCK_FLAG_SIZE] = {0};
    u32 errcode = NANDC_ERROR, cnt;
    struct nandc_host *host = nandc_nand_host;

    /*���ݱ�־λ����ѯ�����־λ*/
	if (host->bbt_options & NAND_BBT_SCANLASTPAGE)
    {
        /*�����һҳ�Ļ����Ƕ�����*/
        errcode = nandc_nand_quary_flag(blk_id, (host->nandchip->spec.blocksize / host->nandchip->spec.pagesize - 1),
            badflag, sizeof(badflag));
        if(NANDC_OK !=  errcode)
        {
            NANDC_TRACE(NFCDBGLVL(ERRO), ("quary last page failed, ret = 0x%x\n", errcode));
            goto ERRO;
        }
		/*��ΪȫFF�Ļ����ǻ���*/
        if((0xFF != badflag[0]))
        {
            *isbad = NANDC_BAD_BLOCK;
        }
        else
        {
            *isbad = NANDC_GOOD_BLOCK;
        }

    }
	/*��ѯ��һҳ�͵ڶ�ҳ�Ƿ��л�����*/
    else if(host->bbt_options & NAND_BBT_SCAN2NDPAGE)
    {
    	/*��ѯ��һҳ�͵ڶ�ҳ�Ļ�����*/
        errcode = nandc_nand_quary_flag(blk_id, NANDC_BAD_FLAG_FIRSTPAGE_OFFSET,  badflag, sizeof(badflag));
        if(NANDC_OK !=  errcode)
        {
            NANDC_TRACE(NFCDBGLVL(ERRO), ("quary page0 failed, ret = 0x%x\n", errcode));
            goto ERRO;
        }
        if((0xFF != badflag[0]))        /* bad block, return directly */
        {
            *isbad = NANDC_BAD_BLOCK;
            return errcode;
        }

        errcode = nandc_nand_quary_flag(blk_id, NANDC_BAD_FLAG_SECONDPAGE_OFFSET,  badflag, sizeof(badflag));
        if(NANDC_OK !=  errcode)
        {
            NANDC_TRACE(NFCDBGLVL(ERRO), ("quary last page failed, ret = 0x%x\n", errcode));
            goto ERRO;
        }
        if((0xFF != badflag[0]))
        {
            *isbad = NANDC_BAD_BLOCK;
        }
        else
        {
            *isbad = NANDC_GOOD_BLOCK;
        }
    }
    else
    {
        NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_nand_quary_bad: unknown bad block offset, host->bbt_options = 0x%x\n",
            host->bbt_options));
        goto ERRO;
    }

    return errcode;

ERRO:
	for(cnt=0;cnt<NANDC_BADBLOCK_FLAG_SIZE;cnt++)
	{
		NANDC_TRACE(NFCDBGLVL(NORMAL),("badflag[%d]=0x%x\n",cnt,badflag[cnt]));
	}
    return errcode;
}

/**
* ����:nandcģ��ʹ��ECC���ܵĶ�����ҳ������
*
* ����:
* @host                 ---nandc_host�Ľṹ��ָ��
* @pageoffset           ---Ҫ����Flash��ҳ��ƫ�Ƶ�ַ
* @dstaddr              ---��Ŷ������ݵ��ڴ��ַ
* @pageleft             ---Ҫ�����ݳ���
*/
static __inline u32 nandc_nand_read_inpage(struct nandc_host* host, u32 pageoffset, u32 dstaddr, u32 pageleft)
{
    u8* pagebuf     =   host->buffers->databuf;
    u32 pagealign   =   host->nandchip->spec.pagealign;
	/*�ڵ��ô˺���ǰ�Ѿ���Ҫ����Flash��ַ������*/
    if (NANDC_OK != nandc_bbm_real_addr((host->addr_logic & pagealign), &host->addr_real))
    {
        goto ERRO;
    }
	/*ʹ��ECC���ܵĶ���ҳ������*/
    if(NANDC_OK !=  nandc_nand_access( host, (u8 *)pagebuf, NANDC_NULL, 0, 0, NAND_CMD_READSTART))
    {
        /* coverity[printf_arg_mismatch] */
        NANDC_TRACE(NFCDBGLVL(ERRO), ("error read in page ,chip:%d addrglob:0x%x\n",
        (u32)host->chipselect, (u32)host->addr_real));
		goto ERRO;
    }
	/*���Ʋ���ҳ�����ݵ�Ŀ���ڴ�*/
    memcpy((void*)dstaddr, pagebuf + pageoffset, pageleft);

    return  NANDC_OK;

ERRO:

    return NANDC_ERROR;

}


/**
* ����:nandcģ��ʹ��ECC���ܵĶ���ҳ����
*
* ����:
* @flashaddr                 ---Ҫ����Flash��ַ
* @dstaddr           		 ---���Ҫ�������ݵ��ڴ��ַ
* @readsize                  ---�����ݵĳ��ȣ����Գ���һ��page�ĳ���
* @oobsize             		 ---��һҳ����Ҫ����OOB����
* @skiplength                ---���˵������ݳ���
*/
static u32 nandc_nand_mass_read(FSZ flashaddr, u32 dstaddr, u32 readsize, u32 oobsize, u32 *skiplength)
{
    struct mtd_partition    *thispart   = NANDC_NULL;
    struct nandc_host       *host       = nandc_nand_host ;
    struct nandc_spec       *spec       = NANDC_NULL;
    FSZ addrglob;
    u32 sizeleft , blockleft,blockoffset, pageleft, pageoffset, badflag , skips = 0;
    u8* oobbuff;
    u32 errorcode = NANDC_ERROR;

    NANDC_REJECT_NULL(host);
    spec = &host->nandchip->spec;

    /*���OOBsize��Ϊ0��Ҫ��flashaddr��ҳ�����*/
    if(0 != oobsize)
    {
        errorcode = nandc_native_location_by_address(host, flashaddr);
        if(NANDC_OK == errorcode)
        {
        	/*readsizeһ����Ҫpagesize+oobsize��������*/
           if(0 != (readsize % (spec->pagesize + oobsize)))
            {
                NANDC_TRACE(NFCDBGLVL(ERRO), ("ERRO! the writesize(0x%x) is not page aligned!\n",
                                              readsize));
                errorcode = NANDC_E_PARAM;
				goto EXIT;
            }
            readsize = readsize - readsize/(spec->pagesize + oobsize) * oobsize;
            if(0 != (readsize & spec->offinpage))
            {
                /* coverity[printf_arg_mismatch] */
                NANDC_TRACE(NFCDBGLVL(ERRO), ("ERRO! the writesize(0x%x) is not page aligned!\n",
                                              readsize));
                errorcode = NANDC_E_PARAM;
				goto EXIT;
            }
        }
        else
        {
            goto EXIT;
        }
    }

    /* coverity[printf_arg_mismatch] */
    NANDC_TRACE(NFCDBGLVL(NORMAL), ("nandc_nand_mass_read flash(0x%x), dst(0x%x), size(0x%x),oobsize(0x%x), skiplength(0x%x) \n",
                                      flashaddr, dstaddr, readsize, oobsize, (u32)skiplength));

    sizeleft = readsize;
    addrglob = flashaddr;
	/*����Ҫ��������*/
    while(0 < sizeleft )
    {
    	/*���ݶ��ĳ��Ȳ����Գ���һ�������ĳ���*/
        errorcode = nandc_native_location_by_address( host, addrglob);
        if(NANDC_OK != errorcode)
        {
            /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(ERRO), ("read addrglob:0x%x not find\n", addrglob));
			goto EXIT;
        }

        if(NANDC_NULL == thispart)
        {
            thispart = host->curpart;
        }
        else if(thispart != host->curpart)
        {
            /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(ERRO), ("ERRO! the read addr(0x%x) beyond the end of pattition(%s):0x%x\n",
                                      addrglob, thispart->name, (FSZ)thispart->size));
			errorcode = NANDC_E_ADDR_OVERSTEP;
            goto EXIT;
        }

        /*��ѯ�Ƿ��ǻ���*/
        errorcode = nandc_nand_quary_bad((u32)(addrglob>>spec->blockshift), &badflag);
        if(NANDC_OK != errorcode)
        {
            /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(ERRO), ("quary block failed:0x%x\n", addrglob));

			goto EXIT;
        }
		/*����ǻ���ͻ������˿�*/
        if(NANDC_BAD_BLOCK == badflag)
        {
            /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(WARNING), ("warning: bad block detected :addrglob:0x%x\n",
                                                                addrglob));
            addrglob += host->nandchip->spec.blocksize;
            skips++;
            continue;
        }
        else
        {
            /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(NORMAL), ("quary block good:0x%x\n", addrglob));
		}

        /*����Ҫ�������ݳ���*/
        pageoffset  =   (u32)(addrglob & spec->offinpage);
        pageleft    =   spec->pagesize - pageoffset;
        pageleft    =   ((sizeleft < pageleft)  ? sizeleft : pageleft);

        /*����Ƕ�����ҳ�Ļ�*/
        if(pageleft < spec->pagesize)
		{
			/* coverity[printf_arg_mismatch] */
			NANDC_TRACE(NFCDBGLVL(NORMAL), ("warning: start page size not aligned :addr_logic:0x%x, \
			pageleft:0x%x\n", host->addr_logic, pageleft));
			/*�����ֵ�page����*/
			errorcode = nandc_nand_read_inpage(host, pageoffset,dstaddr, pageleft);
			if(NANDC_OK != errorcode)
			{
				errorcode = NANDC_E_READ;
				goto EXIT;
			}

			dstaddr    +=  pageleft;
			addrglob   +=  pageleft;
			sizeleft   -=  pageleft;
			host->addr_logic += pageleft;
			/*���ݶ����˾��˳�*/
			if(0 == sizeleft)
			{
				errorcode = NANDC_OK;
				goto EXIT;
			}
		}
		/*������ѯ�˿��Ƿ���Ҫ��������*/
        blockoffset = (u32)(addrglob & spec->offinblock);
        blockleft = spec->blocksize - blockoffset;
        blockleft = ((sizeleft < blockleft) ? sizeleft : blockleft);
		/*��ַת��*/
        errorcode = nandc_bbm_real_addr(host->addr_logic , &host->addr_real);
        if (NANDC_OK != errorcode)
        {
            goto EXIT;
        }
		/*����Ϊ�����ݵĽ׶�*/
        nandc_nand_set_latch(host, NAND_CMD_READSTART);
		/*����block������Ҫ��ȡ*/
        while(0 != blockleft)
		{
			/*����block�Ĳ���ҳ�Ļ�*/
			if(blockleft < spec->pagesize)
			{
				/* coverity[printf_arg_mismatch] */
				NANDC_TRACE(NFCDBGLVL(WARNING), ("warning: end page size not aligned :addr_logic:0x%x,\
				blockleft:0x%x\n",host->addr_logic, blockleft));
				/*������ҳ�Ļ�*/
				errorcode = nandc_nand_read_inpage(host, 0, dstaddr, blockleft);
				if(NANDC_OK != errorcode)
				{
					nandc_nand_clear_latch(host);
					errorcode = NANDC_E_READ;
					goto EXIT;
				}
				/*��block�����ݶ����˾��˳���*/
				addrglob    +=  blockleft;
				sizeleft    -=  blockleft;
				break;
			}
			/*�����OOB���ݵ�*/
			if(oobsize)
			{
				oobbuff = (u8*)(dstaddr + spec->pagesize);
			}
			else
			{
				oobbuff = 0;
			}
            /*�������DMA�ķ�ʽ����ʱҪ��4�ֽڶ���*/
            #ifdef NANDC_USE_DMA_ENABLE
            if(( dstaddr&0x3 )&&( oobsize == 0 ))
            {
                NANDC_TRACE(NFCDBGLVL(ERRO), ("use dma but dstaddr is not align 4 dstaddr:0x%x\n",dstaddr));
                errorcode = nandc_nand_read_inpage(host, 0,dstaddr, spec->pagesize);
                if(NANDC_OK != errorcode)
                {
                    errorcode = NANDC_E_READ;
                    goto EXIT;
                }
            }
            else
            #endif
            {
                /*ʹ��ECC���ܵĶ�һҳ���ݸ��ݲ���ѡ��Ķ�OOB����*/
                errorcode = nandc_nand_access( host, (u8 *)dstaddr, oobbuff, 0, oobsize, NAND_CMD_READSTART);
                if(NANDC_OK != errorcode)
                {
                    /* coverity[printf_arg_mismatch] */
                    NANDC_TRACE(NFCDBGLVL(ERRO), ("error read one page,chip:%d addrglob:0x%x\n", host->chipselect,host->addr_real));
                    nandc_nand_clear_latch(host);
                    errorcode = NANDC_E_READ;
                    goto EXIT;
                }
            }
			/*�ٴ����ж��Ƿ���OOB����Ҫ��*/
			if(0 == oobsize)
			{
				dstaddr += spec->pagesize;
			}
			else
			{
				dstaddr += spec->pagesize + oobsize;
			}

			addrglob    +=  spec->pagesize;
			sizeleft    -=  spec->pagesize;
			blockleft   -=  spec->pagesize;
			host->addr_logic    +=   spec->pagesize;
			host->addr_real     +=   spec->pagesize;
		}
		/*�����д�Ĺ���*/
        nandc_nand_clear_latch(host);
    }

EXIT:
    if(NANDC_NULL != skiplength)
    {
        *skiplength =  skips << spec->blockshift;
        if(*skiplength)
        {
            /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_nand_mass_read bad blk skip length:0x%X\n", *skiplength));
		}
    }

    return errorcode;

ERRO:
    return NANDC_ERROR;
}

/**
* ����:nandcģ��ʹ��ECC���ܵĶ�һ��block������
*
* ����:
* @flashaddr                 ---Ҫ����Flash��ַ
* @dstaddr           		 ---���Ҫ�������ݵ��ڴ��ַ
* @oobsize             		 ---��һҳ����Ҫ����OOB����
*/
static u32 nandc_nand_read_block(FSZ flashaddr, u32 dstaddr, u32 oobsize)
{
    struct mtd_partition    *thispart  = NANDC_NULL;
    struct nandc_host       *host       = nandc_nand_host;
    struct nandc_spec       *spec       = NANDC_NULL;
    FSZ addrglob;
    u32 sizeleft, blockleft, badflag;
    u8* oobbuff;
    u32 errorcode = NANDC_ERROR;

    NANDC_REJECT_NULL(host);
    spec = &host->nandchip->spec;

    /* coverity[printf_arg_mismatch] */
    NANDC_TRACE(NFCDBGLVL(NORMAL), ("nandc_nand_read_block flash(0x%x), dst(0x%x) \n",
                                      flashaddr, dstaddr));

    sizeleft = spec->blocksize;
    addrglob = flashaddr & spec->blockalign;
    while(0 < sizeleft )
    {
        errorcode = nandc_native_location_by_address( host, addrglob);
        if(NANDC_OK != errorcode)
        {
            /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(ERRO), ("read addrglob:0x%x not find\n", addrglob));
            goto EXIT;
        }

        if(NANDC_NULL == thispart)
        {
            thispart = host->curpart;
        }
        else if(thispart != host->curpart)
        {
            /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(ERRO), ("ERRO! the read addr(0x%x) beyond the end of partition(%s):0x%x\n",
                                      addrglob, thispart->name, (FSZ)thispart->size));
            errorcode = NANDC_E_ADDR_OVERSTEP;
            goto EXIT;
        }

        /* query bad block flag*/
        errorcode = nandc_nand_quary_bad((u32)(addrglob>>spec->blockshift), &badflag);
        if(NANDC_OK != errorcode)
        {
            /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(ERRO), ("quary block failed:0x%x\n", addrglob));
            goto EXIT;
        }

        if(NANDC_BAD_BLOCK == badflag)
        {
            /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(WARNING), ("warning: bad block detected :addrglob:0x%x\n",
                                                                addrglob));
            goto ERRO;
        }
        else
        {
            /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(NORMAL), ("quary block good:0x%x\n", addrglob));
        }

        blockleft = sizeleft;
        errorcode = nandc_bbm_real_addr(host->addr_logic , &host->addr_real);
        if (NANDC_OK != errorcode)
        {
            goto EXIT;
        }

        nandc_nand_set_latch(host, NAND_CMD_READSTART);
        while(0 != blockleft)
        {
            /* read data page by page in one block,when in the end the data may not
               enough for one page.we need to call "nandc_nand_read_inpage" */
           oobbuff = (u8 *)(dstaddr + spec->pagesize);

           errorcode = nandc_nand_access( host, (u8 *)dstaddr, oobbuff, 0, oobsize, NAND_CMD_READSTART);
           if(NANDC_OK != errorcode)
           {
                /* coverity[printf_arg_mismatch] */
                NANDC_TRACE(NFCDBGLVL(ERRO), ("error read one page,chip:%d addrglob:0x%x\n", host->chipselect,host->addr_real));
                nandc_nand_clear_latch(host);
				errorcode = NANDC_E_READ;
                goto EXIT;
           }

            dstaddr += (oobsize)?(spec->pagesize+oobsize):spec->pagesize;

            addrglob    +=  spec->pagesize;
            sizeleft    -=  spec->pagesize;
            blockleft   -=  spec->pagesize;
            host->addr_logic    +=   spec->pagesize;
            host->addr_real     +=   spec->pagesize;
        }
        nandc_nand_clear_latch(host);
    }

EXIT:
    return errorcode;

ERRO:
    return NANDC_ERROR;
}


/**
* ����:nandcģ�����nandc_host�����ݽṹ
*
* ����:
* @host                      ---���nandc_host��ָ���ָ��
*/
u32 nandc_nand_create_host(struct nandc_host  **host)
{
	/*����nandc_init_seed����������nandc_host���ݽṹ*/
    nandc_nand_host = nandc_native_host_create(nandc_init_seed);

    if(NANDC_NULL == nandc_nand_host)
    {
        *host = NANDC_NULL;
        return NANDC_ERROR;
    }
    else
    {
        *host = nandc_nand_host;
        return NANDC_OK;
    }
}

/**
* ����:nandcģ���ʼ����������Ϣ
*
* ����:
* @host                      ---���nandc_host��ָ���ָ��
*/
u32 nandc_init_mtd_partition(struct nandc_host* host)
{
    struct mtd_partition* ptable = NANDC_NULL;
    u32 errorcode = NANDC_ERROR;
    u32 nr_parts;
	/*������������Ϣ*/
    if(NANDC_OK == ptable_parse_mtd_partitions(&ptable, &nr_parts))
    {
        errorcode = nandc_host_set_partition(host, ptable, nr_parts);
    }
    else
    {
        errorcode = nandc_host_set_partition(host, NANDC_NULL, 0);
    }

    return errorcode;
}

/*****************************************************************************
* �� �� ��  : nand_spec_info_save
*
* ��������  : ��nand spec info���浽shared memory
*
* �������  : @spec_real - nand spec info
*			  @spec_shmem - buffer to save nand spec info
* �������  : null
*
* �� �� ֵ  : null
*
* ����˵��  :
*
*
*****************************************************************************/
void nand_spec_info_save(struct nandc_host *host, struct nand_spec_shared_mem *spec_dst)
{
    struct nandc_spec *spec_src = NULL;

	if(!host || !host->nandchip || !spec_dst)
	{
	    NANDC_TRACE(NFCDBGLVL(ERRO), ("[%s]ERRIR:argu null\n", __FUNCTION__));
		return;
	}

    spec_src = &host->nandchip->spec;

    spec_dst->page_size = spec_src->pagesize;
    spec_dst->ecc_type = host->nandchip->ecctype;
    spec_dst->addr_num = spec_src->addrcycle;
    spec_dst->block_size = spec_src->blocksize;
    spec_dst->spare_size = spec_src->sparesize;
    spec_dst->chip_size = spec_src->chipsize;

    NANDC_TRACE(NFCDBGLVL(ERRO), ("pagesize %d  ecctype %d  addrnum %d  blocksize 0x%x  sparesize %d chipsize 0x%x\n",
        spec_dst->page_size, spec_dst->ecc_type, spec_dst->addr_num, spec_dst->block_size, spec_dst->spare_size, spec_dst->chip_size));/*lint !e778*/

    spec_dst->flag = NAND_SPEC_ALREADY_SAVED;
	return;
}

void nand_shared_mem_clear(void)
{
    memset((void *)SHM_MEM_NAND_SPEC_ADDR, 0, sizeof(struct nand_spec_shared_mem));
    return;
}

/**
* ����:nandcģ���ʼ������
*
* ����:
* @��
* ����:nandcģ��ĳ�ʼ��������Ӳ���Ĵ����ĳ�ʼ����nandc_host���ݽṹ��ʼ����������ĳ�ʼ������
*/
u32 nandc_nand_init(void)
{
    struct nandc_host* host = NANDC_NULL;
    u32 errorcode = NANDC_ERROR;
    nand_shared_mem_clear();
    /* set ebi normal mode to avoid fault when sharing ebi with emi, only for nandc v600 */
    hi_syssc_ebi_mode_normal();

    /*nandcģ���ʱ��ʹ��*/
    hi_syscrg_nandc_clk_enable();

    /*����nandc_host���ݽṹ*/
    if(NANDC_OK !=  nandc_nand_create_host(&host))
    {
        goto ERRO;
    }

    /*��ʼ��������*/
    if(host->ctrlfunc->init_cmd)
    {
        /*modified for lint e522 */
        (void)host->ctrlfunc->init_cmd(&host->bitcmd,  host);
    }
	/*̽��nandflash*/
    /*ֻ֧��8bit��nandflash*/
    host->probwidth = nandc_bus_08;
    if(NANDC_OK != nandc_native_nand_prob(host))
    {
        goto ERRO;
    }
	/*����nandflash�ķ�������Ϣ*/
    if(NANDC_OK == nandc_init_mtd_partition(host))
    {
        if(NANDC_OK != nandc_host_set_chipparam(host))
        {
            errorcode = NANDC_ERROR;
            goto ERRO;
        }
    }

    /* save nand spec info to shared memory */
    nand_spec_info_save(host, (struct nand_spec_shared_mem *)SHM_MEM_NAND_SPEC_ADDR);

    return NANDC_OK;
ERRO:

    return errorcode;
}



/**
* ����:nandcģ����д����
*
* ����:
* @pagenum                      ---Ҫ��д��ҳ��
* @offsetinpage                 ---ҳ���ƫ�Ƶ�ַ
* @src_addr                     ---Դ��ַ
* @writesize                    ---Ҫд�����ݳ���
* ����:nandcģ�����д����
*/
static u32 nandc_nand_write_raw(u32 pagenum, u32 offsetinpage, u32 src_addr, u32 writesize)
{
    struct nandc_host* host = nandc_nand_host ;
    struct nandc_spec* spec;
    int ret;

    spec = &host->nandchip->spec ;
	/*����Flash�ĵ�ַ��Ϣ*/
    host->addr_real  = (pagenum & spec->pagemask) << spec->pageshift;
    host->chipselect = pagenum >> (spec->chipshift - spec->pageshift);

    if(NANDC_NULL == host->buffers)
    {
        /* coverity[printf_arg_mismatch] */
        NANDC_TRACE(NFCDBGLVL(ERRO), ("write raw error!! host->buffers is NULL \n"));
        goto ERRO;
    }
    /*���Ҫд������*/
    memset((void*)(host->buffers->databuf), 0xff, sizeof(host->buffers->databuf)); /*clean the pclint 516*/
    memcpy(host->buffers->databuf + offsetinpage, (const void *)src_addr, writesize);
    /*д����Flash�����ݳ���*/
    ret = nandc_nand_access(host, host->buffers->databuf, host->buffers->databuf + spec->pagesize, spec->pagesize + spec->sparesize, 0, NAND_CMD_PAGEPROG);
    if(NANDC_OK != ret)
    {
        return ret;
    }

    return NANDC_OK;
ERRO:
    return NANDC_ERROR;
}


/**
* ����:nandcģ��ʹ��ECC���ܵ�дһҳ����
*
* ����:
* @address                      ---Ҫ��д��Flash��ַ
* @databuff                     ---Ҫд���������ڴ��еĵ�ַ
* @oobbuff                      ---Ҫд��OOB�������ڴ��еĵ�ַ
* @oobsize                      ---һҳ����Ҫд��OOB����

* ����:nandcģ���ʹ��ECC���ܵ�д���ݹ���,������д���ݵĹ�����д��OOB��Ϣ
*/
static u32 nandc_nand_write_page(FSZ address, u8* databuff, u8* oobbuff, u32 oobsize)
{

    struct nandc_host* host = nandc_nand_host ;

    NANDC_REJECT_NULL(host);
	/*��ַת��*/
    if(NANDC_OK != nandc_native_location_by_address( host, address))
    {
        return NANDC_E_NOFIND;
    }
	/*ʹ��ECC���ܵ�дһҳ���ݣ�ע���4������Ϊ0*/
    return nandc_nand_access(host, databuff,  oobbuff, 0, oobsize, NAND_CMD_PAGEPROG);

 ERRO:

    return NANDC_E_PARAM;
}

/**
* ����:nandcģ����block�ı��ֵ
*
* ����:
* @blk_id                      ---block���
* @pageoffset                  ---��block���ڵ�ҳ��ַ

* ����:nandcģ����block��ŵı�־ֵ
*/
u32 nandc_nand_mark_flag(u32 blk_id, u32 pageoffset)
{
    struct nandc_host 	*host 	 = nandc_nand_host ;

    NANDC_REJECT_NULL(host);

	struct nandc_spec	*spec 	 = &(host->nandchip->spec);
	u32 sparesize 	= spec->sparesize;
	u32 pagesize  	= spec->pagesize;
	u8 *databuf 	= host->buffers->databuf;
	/*����Ҫд��Flashҳ��ַ*/
    if(NANDC_OK ==nandc_nand_flag_address(host, blk_id, pageoffset))
    {
		memset((void *)databuf, 0, (pagesize + sparesize));
		/*��д���ݲ���*/
        return nandc_nand_access(host, (u8*)databuf, (u8*)(databuf + pagesize), pagesize + sparesize, 0, NAND_CMD_PAGEPROG);
    }

 ERRO:
     return NANDC_ERROR;
}


/**
* ����:nandcģ���ǻ���Ĺ���
*
* ����:
* @blk_id                      ---block���

* ����:���nandflash�Ļ��鹦��
*/
u32 nandc_nand_mark_bad(u32 blk_id)
{
    u32 errcode = NANDC_ERROR;
    struct nandc_host *host = nandc_nand_host;

    if(0 == blk_id)
    {
        /* coverity[printf_arg_mismatch] */
        NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_nand_mark_bad: try to mark block 0, ignore\n"));
		return NANDC_OK;
	}

    /* coverity[printf_arg_mismatch] */
    NANDC_TRACE(NFCDBGLVL(WARNING), ("nandc_nand_mark_bad: 0x%x.\n",blk_id));

    /*��Ǵ�block�����һҳ*/
	if (host->bbt_options & NAND_BBT_SCANLASTPAGE)
    {
        errcode = nandc_nand_mark_flag(blk_id, (host->nandchip->spec.blocksize / host->nandchip->spec.pagesize - 1) );
        if(NANDC_OK != errcode)
        {
            NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_nand_mark_bad: mark last page failed\n"));
            goto ERRO;
        }
    }
    else if(host->bbt_options & NAND_BBT_SCAN2NDPAGE)
    {
        errcode = nandc_nand_mark_flag(blk_id, NANDC_BAD_FLAG_FIRSTPAGE_OFFSET );
        if(NANDC_OK != errcode)
        {
            NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_nand_mark_bad:  mark first page failed\n"));
            goto ERRO;
        }
        errcode = nandc_nand_mark_flag(blk_id, NANDC_BAD_FLAG_SECONDPAGE_OFFSET);
        if(NANDC_OK != errcode)
        {
            NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_nand_mark_bad:  mark second page failed\n"));
            goto ERRO;
        }
    }
    else
    {
        NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_nand_mark_bad: unknown bad block offset, host->bbt_options = 0x%x\n",
            host->bbt_options));
        goto ERRO;
    }

    return NANDC_OK;
ERRO:
    NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_nand_mark_bad: mark block 0x%x error, ret = 0x%x\n", blk_id, errcode));
    return errcode;
}

/**
* ����:nandcģ�������Ĺ���
*
* ����:
* @blk_id                      ---block���

* ����:����nandflash�����ݿ�
*/
static u32 nandc_nand_erase_block(u32 blknum)
{
    struct nandc_host* host = nandc_nand_host;
    u32 errorcode = NANDC_OK;

    NANDC_REJECT_NULL(host);

    if(0 == blknum)
    {
        /* coverity[printf_arg_mismatch] */
        NANDC_TRACE(NFCDBGLVL(WARNING), ("nandc_nand_erase_block: erase blknum 0.\n"));
    }
	/*�ѿ��ַת��*/
    errorcode = nandc_native_location_by_blocknum(host, blknum);

    if(NANDC_OK != errorcode)
    {
        /* coverity[printf_arg_mismatch] */
        NANDC_TRACE(NFCDBGLVL(ERRO), ("erase blknum:0x%x not find\n", blknum));
        goto ERRO;
    }
	/*ִ�в�����Ĺ���*/
    return nandc_nand_access(host, NANDC_NULL, NANDC_NULL, 0, 0, NAND_CMD_ERASE2);

ERRO:
    return NANDC_ERROR;
}

/**
* ����:nandcģ��ʹ��ECC���ܵ�д��ҳ����,��д�Ĺ����л����Flash�������߿��Բ��ò���nandflash
*
* ����:
* @flashaddr                 ---Ҫд��Flash��ַ
* @srcaddr           		 ---���Ҫд�����ݵ��ڴ��ַ
* @writesize                 ---Ҫд���ݵĳ��ȣ����Գ���һ��page�ĳ��ȣ���ҪдOOB����ʱ,writesize������Ҫд�����ݳ��Ⱥ�OOB����
* @oobsize             		 ---дһҳ����Ҫд��OOB����
* @skiplength                ---���˵������ݳ���
*/
static u32 nandc_nand_mass_write(FSZ flashaddr, u32 srcaddr, u32 writesize, u32 oobsize, u32 *skiplength)
{
    struct mtd_partition    *thispart   =   NANDC_NULL;
    struct nandc_spec       *spec       =   NANDC_NULL;
    struct nandc_host       *host       =   nandc_nand_host;
    FSZ addr_glob;
    u32 sizeleft, sizeinblock, offsetinblock, badflag , blockleft ,skips = 0;
    u8* oobbuff;
    static u8* blockbuf    =   NANDC_NULL;
    static u8* rdblockbuf  =   NANDC_NULL;
    u32 errorcode   =   NANDC_OK;
    u32 ramaddr     =   srcaddr;
    u32 tempaddr, tempbuf;
    u32 rdbackaddr  =   NANDC_NULL;
    u32 rdbacklen   =   0;
    u32 buflen    = 0;
    u32 rdbuflen    = 0;
    u32 nextsrcaddr    = 0;

    NANDC_REJECT_NULL(host);
    spec = &host->nandchip->spec;

    /*���oobsize��Ϊ0��ô���ݳ��Ⱥ�flash��ַ��Ҫ��*/
    if(0 != oobsize)
    {
        errorcode = nandc_native_location_by_address(host, flashaddr);
        if(NANDC_OK == errorcode)
        {
            writesize = writesize - writesize/(spec->pagesize + oobsize) * oobsize;
			/*Ҫд�����ݴ�С�ǲ�����������pagesize+oobsize*/
            if((0 != (writesize & spec->offinpage)))
            {
                /* coverity[printf_arg_mismatch] */
                NANDC_TRACE(NFCDBGLVL(ERRO), ("ERRO! the writesize(0x%x) and flashaddr(0x%x) is not block aligned!\n",
                                              writesize, flashaddr));
                errorcode = NANDC_E_PARAM;
				goto EXIT;
            }
        }
        else
        {
            goto EXIT;
        }
    }

    /* coverity[printf_arg_mismatch] */
    NANDC_TRACE(NFCDBGLVL(NORMAL), ("nandc_nand_mass_write flash(0x%x), src(0x%x), size(0x%x),oobsize(0x%x), skiplength(0x%x) \n",
                                      flashaddr, srcaddr, writesize, oobsize, (u32)skiplength));

    sizeleft = writesize;
    addr_glob = flashaddr;
	/*�����ڴ�ռ�,�ռ�Ĵ�СΪblocksize+oobsize*ҳ��Ŀ*/
    buflen = spec->blocksize/spec->pagesize*(spec->pagesize + YAFFS_BYTES_PER_SPARE);  /* YAFFS_BYTES_PER_SPARE" is the size of oob data length that used by yaffs */
    if(!blockbuf)
    {
        blockbuf = (u8 *)himalloc(buflen);
    }
    NANDC_REJECT_NULL(blockbuf);

    rdbuflen = buflen;
    if(!rdblockbuf)
    {
        rdblockbuf = (u8 *)himalloc(rdbuflen);
    }
    NANDC_REJECT_NULL(rdblockbuf);
	/*������Ҫд*/
    while(0 < sizeleft)
    {
        errorcode = nandc_native_location_by_address(host, addr_glob);
        if(NANDC_OK != errorcode)
        {
            /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(ERRO), ("read addr_glob:0x%x not find\n", addr_glob));
			goto EXIT;
        }

        if(NANDC_NULL == thispart)
        {
            thispart = host->curpart;
        }
        else if(thispart != host->curpart)
        {
            /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(ERRO), ("ERRO! the write addr(0x%x) beyond the end of pattition(%s)\n",
                                      addr_glob, thispart->name));

            errorcode = NANDC_E_ADDR_OVERSTEP;
            goto EXIT;
        }
		/*��ѯ�Ƿ��ǻ���*/
        errorcode = nandc_nand_quary_bad((u32)(addr_glob>>spec->blockshift), &badflag);
        if(NANDC_OK != errorcode)
        {
            /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(ERRO), ("quary block failed:0x%x\n", addr_glob));
			goto EXIT;
        }

        if(NANDC_BAD_BLOCK == badflag)
        {
            /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(WARNING), ("warning: bad block at:(0x%x) detected ,skip one block.\n", addr_glob));

            addr_glob += host->nandchip->spec.blocksize;
            skips++;
            goto LOOP_AGAIN;
        }
        else
        {
            /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(NORMAL), ("quary block good:0x%x\n", addr_glob));
        }

        offsetinblock = (u32)addr_glob & spec->offinblock;
        sizeinblock = spec->blocksize - offsetinblock;
        sizeinblock = (sizeleft < sizeinblock) ? sizeleft : sizeinblock;

        errorcode = nandc_bbm_real_addr(host->addr_logic , &host->addr_real);
        if (NANDC_OK != errorcode)
        {
            goto EXIT;
        }

        tempaddr = addr_glob;
        tempbuf  = ramaddr;
        if((sizeinblock != spec->blocksize))
        {
            errorcode = nandc_nand_read_block(addr_glob & spec->blockalign, (u32)blockbuf, oobsize);
            if(NANDC_OK != errorcode)
            {
                /* coverity[printf_arg_mismatch] */
                NANDC_TRACE(NFCDBGLVL(WARNING), ("warning: read error, mark as bad block:0x%x,offset:0x%x,size:0x%x,blocksize:0x%x,line %d\n",
                                                    addr_glob, offsetinblock, sizeinblock, spec->blocksize, __LINE__));

                (void)nandc_nand_mark_bad((u32)(addr_glob>>spec->blockshift));
                addr_glob += spec->blocksize;
                skips++;
                goto LOOP_AGAIN;
            }

            /* always erase */
            ramaddr   =   (u32)blockbuf;
            addr_glob &=  spec->blockalign;
            errorcode =  nandc_nand_erase_block((u32)(addr_glob>>spec->blockshift));
            blockleft = spec->blocksize;
            if(0 != oobsize)
            {
                memcpy((void*)((u32)blockbuf + (offsetinblock/spec->pagesize)*(spec->pagesize+oobsize)), (const void*)srcaddr, (sizeinblock/spec->pagesize)*(spec->pagesize+oobsize));
            }
            else
            {
                memcpy((void*)((u32)blockbuf + offsetinblock), (const void*)srcaddr, sizeinblock);
            }
        }
        else
        {
            errorcode = nandc_nand_read_block(addr_glob & spec->blockalign, (u32)blockbuf, oobsize);
            if(NANDC_OK != errorcode)
            {
                /* coverity[printf_arg_mismatch] */
                NANDC_TRACE(NFCDBGLVL(WARNING), ("warning: read error, mark as bad block:0x%x,offset:0x%x,size:0x%x,blocksize:0x%x line:%d\n",
                                                    addr_glob, offsetinblock, sizeinblock, spec->blocksize, __LINE__));
				/*����������˴����Ҫ�껵��*/
                (void)nandc_nand_mark_bad((u32)(addr_glob>>spec->blockshift));
                addr_glob += spec->blocksize;
                skips++;
                goto LOOP_AGAIN;
            }

            ramaddr = srcaddr;
            errorcode =  nandc_nand_erase_block((u32)(addr_glob>>spec->blockshift));
            blockleft =  (sizeleft < spec->blocksize) ? sizeleft : spec->blocksize;
        }
        /*ȷ����һ��Ҫ���Ƶ����ݵ�ַ*/
        if(oobsize)
        {
            nextsrcaddr=srcaddr+(sizeinblock/spec->pagesize)*(spec->pagesize+oobsize);
        }
        else
        {
            nextsrcaddr=srcaddr+sizeinblock;
        }
        if(NANDC_OK !=  errorcode)
        {
            /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(WARNING), ("warning: erase block at:(0x%x) return error, skip one block.\n", addr_glob));
            (void)nandc_nand_mark_bad((u32)(addr_glob>>spec->blockshift));
            addr_glob = tempaddr + spec->blocksize;
            skips++;
            goto LOOP_AGAIN;
        }

        nandc_nand_set_latch(host, NAND_CMD_PAGEPROG);
        rdbackaddr  = ramaddr;
        rdbacklen   = (oobsize)?(blockleft/spec->pagesize*(spec->pagesize + oobsize)):blockleft;
        while(0 != blockleft)
        {
            if(oobsize)
            {
                oobbuff = (u8*)(ramaddr + spec->pagesize);
            }
            else
            {
                oobbuff = 0;
            }

            errorcode = nandc_nand_access(host, (u8 *)ramaddr, oobbuff, 0, oobsize, NAND_CMD_PAGEPROG);
            if(NANDC_OK != errorcode)
            {
                /* coverity[printf_arg_mismatch] */
                NANDC_TRACE(NFCDBGLVL(ERRO), ("error in write ,chip:%d addr_real:0x%x\n", host->chipselect,host->addr_real));
				(void)nandc_nand_mark_bad((u32)(addr_glob>>spec->blockshift));

                skips++;
                addr_glob = tempaddr + spec->blocksize;
                ramaddr   = tempbuf;
                goto LOOP_AGAIN;
            }

            if(0 == oobsize)
            {
                 ramaddr += spec->pagesize;
            }
            else
            {
                 ramaddr += spec->pagesize + oobsize;
            }

            blockleft       -=  spec->pagesize;
            host->addr_real +=  spec->pagesize;
        }

        errorcode = nandc_nand_read_block(addr_glob , (u32)rdblockbuf, oobsize);
        if((NANDC_OK != errorcode)
			|| memcmp((const void *)rdbackaddr, (const void *)rdblockbuf, rdbacklen))
        {
            /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(WARNING), ("warning: read back error, mark as bad block:0x%x,offset:0x%x,size:0x%x,blocksize:0x%x\n",
                                                                                        addr_glob, offsetinblock, sizeinblock, spec->blocksize));

            (void)nandc_nand_mark_bad((u32)(addr_glob>>spec->blockshift));

            skips++;
            addr_glob = tempaddr + spec->blocksize;
            ramaddr   = tempbuf;
            goto LOOP_AGAIN;
        }

        sizeleft    -=  sizeinblock;
        addr_glob   =  tempaddr + sizeinblock;

        srcaddr = nextsrcaddr;

LOOP_AGAIN:
        nandc_nand_clear_latch(host);
    }

    if(NANDC_NULL != skiplength)
    {
        *skiplength =  skips << spec->blockshift;
        if(*skiplength)
        {
           /* coverity[printf_arg_mismatch] */
            NANDC_TRACE(NFCDBGLVL(ERRO), ("nandc_nand_mass_write bad blk skip length:0x%X\n", *skiplength));

		}
    }


EXIT:
    if(blockbuf)
        hifree((void*)blockbuf);
    if(rdblockbuf)
        hifree((void*)rdblockbuf);

    return errorcode;

ERRO:
    if(blockbuf)
        hifree((void*)blockbuf);
    if(rdblockbuf)
        hifree((void*)rdblockbuf);

    return NANDC_ERROR;
}

/**
* ����:�õ�nandflash�Ĺ�����
*
* ����:
* @specs                      ---��nandflash�Ĺ���������ڴ����ݽṹ��
* ����:�õ�nandflash�Ĺ�����
*/
static u32 nandc_nand_spec(struct nand_spec *specs)
{
    struct nandc_host* host = nandc_nand_host ;
    struct nandc_spec*  nand_spec;

    NANDC_REJECT_NULL(host);

    nand_spec = &host->nandchip->spec;

    specs->blocksize =   nand_spec->blocksize;
    specs->pagesize  =   nand_spec->pagesize;
    specs->chipsize  =   nand_spec->chipsize;
    specs->sparesize  =   nand_spec->sparesize;

    return NANDC_OK;

ERRO:
    return NANDC_ERROR;

}

struct nand_interface nandc_nand_funcs=
{
    nandc_nand_init,
    nandc_nand_mass_read,
    nandc_nand_spec,
    nandc_nand_read_page,
    nandc_nand_read_raw,
    nandc_nand_quary_bad,
    nandc_nand_mass_write,

    nandc_nand_write_page,
    nandc_nand_write_raw,
    nandc_nand_mark_bad,
    nandc_nand_erase_block

/*lint -restore */
};

#endif
#ifdef __cplusplus
}
#endif
