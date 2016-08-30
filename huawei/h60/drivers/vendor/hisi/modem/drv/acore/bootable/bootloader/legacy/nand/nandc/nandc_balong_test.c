
/******************************************************************************
*    Copyright (c) 2013-2016 by  Hisilicon Tech. Co., Ltd.
*
*    All rights reserved.
*
******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif


#ifdef __FASTBOOT__
#include "nandc_balong.h"
#include "nandc_cfg.h"
#include "nandc_mco.h"

#include "ptable_com.h"
#include "nandc_cfg.h"

typedef unsigned char u8 ;
typedef unsigned int  u32;

extern u32 nandc_native_reset_chip(struct nandc_host* host , u32 chipselect);
extern u32 nandc_native_get_id(struct nandc_host* host, u8* id_data, u32 length);
extern void cprintf(const char *fmt, ...);

#define PRINT_DATAS(buffer, x, i); \
    cprintf("\n"); \
    for(i = 1; i <= x; i++) \
    { \
       cprintf("%x ", *buffer); \
       buffer++; \
       if(i % 6 == 0) \
       {  \
            cprintf("\n"); \
       } \
    } \
    cprintf("\n");


#endif /* #ifdef __FASTBOOT__ */

#define PAGESPERBLOCK   64

/*************************************** BEGIN: test nandc ********************************************/
#ifdef __FASTBOOT__

/******************************************************************************
* �� �� ��: nandc_test_case_001
* ��������: ����fastboot��nandc����read flash id����
* �������: ��
* �������: ��
* �� �� ֵ: ��
* ����˵��: ��
*******************************************************************************/
void nandc_test_case_001(void)
{
    u8 id_data[NANDC_READID_SIZE];
    u32 ret, i;
    struct nandc_host* host = NANDC_NULL;

    /* nand init clear */
    nand_init_clear();

    /*use "nandc_init_seed" to initialize host*/
    ret = nandc_nand_create_host(&host);
    if(NANDC_OK != ret)
    {
        goto fail;
    }

    /* reset chip */
    nandc_native_reset_chip(host, host->chipselect);

    /* get id */
    ret = nandc_native_get_id(host, id_data, sizeof(id_data));
    if(NANDC_OK != ret)
    {
        goto fail;
    }

    /* nandc command init */
    if(host->ctrlfunc->init_cmd)
    {
        (void)host->ctrlfunc->init_cmd(&host->bitcmd,  host);
    }

    /* output id */
    cprintf("Nand flash id: ");
    for(i = 0; i < NANDC_READID_SIZE; i++)
    {
        cprintf("0x%2x ",id_data[i]);
    }
    cprintf("\n");

    /* successful */
    cprintf("SUCCESS: nandc_test_case_001\n");
    return;

fail:
    if(host)
        hifree((void*)host);
	cprintf("FAIL: nandc_test_case_001, ret = %d\n", ret);
	return;
}

/******************************************************************************
* �� �� ��: nandc_test_case_002
* ��������: ����fastboot��nandc����д������������ع���
* �������: ��
* �������: ��
* �� �� ֵ: ��
* ����˵��: 1�������������nand��ʼ����read��write�ͻ����ѯ����ǹ���
*           2����������:
*               1 - find a good block from block 8, beyond fastboot partition
*               2 - get two buffers named "buffer" and "buffer_value".
*               3 - write "buffer" which is initialied with 0x00000000 to the block
*               4 - erase the block
*               5 - read the data back from the block to "buffer", which should be 0xFFFFFFFF
*               6 - compare "buffer" with "buffer_value", which is initialied with 0xFFFFFFFF
*******************************************************************************/
void nandc_test_case_002(void)
{
	u32 block_id;
	u32 ret = NANDC_ERROR;
	u32 flash_addr;
    u32 bad_flag;
	u32* buffer = NULL;
    u32* buffer_value = NULL;
	struct nand_spec * spec = NULL;

	/* nand init */
    ret = nand_init();
    if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* get flash spec */
	spec = (struct nand_spec *)himalloc(sizeof(struct nand_spec));
    if(!spec)
    {
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	ret = nand_get_spec(spec);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

    /* find a good block */
    block_id = 8;
    while(1)
    {
        ret = nand_isbad(block_id, &bad_flag);
    	if(ret)
    	{
	        cprintf(" Line %d\n", (int)__LINE__);
    		goto fail;
    	}
        /* good block, break */
    	if(bad_flag != NANDC_BAD_BLOCK)
    	{
    		break;
    	}
        block_id++;
    }
    cprintf("good block found, block id = %d\n", block_id);

	/* get buffer */
	buffer = (u32*)himalloc(spec->blocksize);
    buffer_value = (u32*)himalloc(spec->blocksize);
	if((!buffer) || (!buffer_value))
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* write */
	flash_addr = block_id * spec->blocksize;
	memset(buffer, 0, spec->blocksize);
	ret = nand_write(flash_addr, (u32)buffer, spec->blocksize, 0);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* erase */
	ret = nand_erase(block_id);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* read */
	ret = nand_read(flash_addr, (u32)buffer, spec->blocksize, 0);
	if(ret)
	{
		goto fail;
	}

	/* compare data with "0xFF" */
    memset(buffer_value, 0xFF, spec->blocksize);
	ret = memcmp(buffer, buffer_value, spec->blocksize);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

    hifree((void*)buffer);
    hifree((void*)buffer_value);
    hifree((void*)spec);
    cprintf("SUCCESS: nandc_test_case_002\n\n");
    return;

fail:
	cprintf("FAIL:nandc_test_case_002, ret = %d\n\n", ret);
	if(buffer)
		hifree((void*)buffer);
    if(buffer_value)
		hifree((void*)buffer_value);
	if(spec)
		hifree((void*)spec);

	return ;

}

/******************************************************************************
* �� �� ��: nandc_test_case_003
* ��������: ����fastboot��nandc����bus��ʽ��ecc��дdata
* �������: ��
* �������: ��
* �� �� ֵ: ��
* ����˵��: 1�������ڳ�ʼ���������ѯ�ͱ�ǹ���
*           2��step as follow
*               1 - find a good block form block 8;
*               2 - write "buffer" which is initialized with 0x00000000 to flash
*               3 - read back from flash to "readbuffer" which is initialized with 0xFFFFFFFF
*               4 - compare "buffer" and "readbuffer"
*******************************************************************************/
void nandc_test_case_003(void)
{
	u32 block_id;
	u32 ret = NANDC_ERROR;
	u32 flash_addr;
    u32 bad_flag;
	u32* buffer	= NULL;
	u32* readbuffer = NULL;
	u32 length;
	struct nand_spec *spec = NULL;

	/* nand isinit */
    ret = nand_init();
    if(ret)
    {
	    cprintf(" Line %d\n", (int)__LINE__);
        goto fail;
    }

	/* get flash spec */
	spec = (struct nand_spec *)himalloc(sizeof(struct nand_spec));
    if(!spec)
    {
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}
	ret = nand_get_spec(spec);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

    /* find a good block */
    block_id = 8;
    while(1)
    {
        ret = nand_isbad(block_id, &bad_flag);
    	if(ret)
    	{
	        cprintf(" Line %d\n", (int)__LINE__);
    		goto fail;
    	}
        /* good block, break */
    	if(bad_flag != NANDC_BAD_BLOCK)
    	{
    		break;
    	}
        block_id++;
    }
    cprintf("good block found, block id = %d\n", block_id);

	/* erase  */
	ret = nand_erase(block_id);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* get buffer */
	length = spec->blocksize;
	buffer = (u32*)himalloc(length);
	if(!buffer)
	{
		goto fail;
	}

	/* get read buffer */
	readbuffer = (u32*)himalloc(length);
	if(!readbuffer)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* write */
	flash_addr = block_id * spec->blocksize;
	memset(buffer, 0, length);
	ret = nand_write(flash_addr, (u32)buffer, length, 0);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* read */
	memset(readbuffer, 0xff, length);
	ret = nand_read(flash_addr, (u32)readbuffer, length, 0);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* compare two buffer */
	ret = memcmp(buffer, readbuffer, length);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

    /* erase */
	ret = nand_erase(block_id);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}


    hifree((void*)buffer);
    hifree((void*)spec);
    cprintf("SUCCESS: nandc_test_case_003\n\n");
	return;

fail:
	cprintf("FAIL:nandc_test_case_003, ret = %d\n\n", ret);

	if(spec)
		hifree((void*)spec);
	if(buffer)
		hifree((void*)buffer);
	if(readbuffer)
		hifree((void*)readbuffer);

	return;
}

/******************************************************************************
* �� �� ��: nandc_test_case_004
* ��������: ����fastboot��nandc����bus��ʽ��ecc��дdata+oob
* �������: ��
* �������: ��
* �� �� ֵ: ��
* ����˵��: 1�������ڳ�ʼ���������ѯ�ͱ�ǹ���
*           2��step as follow
*               1 - find a good block form block 8;
*               2 - write "buffer" which is initialized with 0x00000000 to flash
*               3 - read back from flash to "readbuffer" which is initialized with 0xFFFFFFFF
*               4 - compare "buffer" and "readbuffer"
*******************************************************************************/
void nandc_test_case_004(void)
{
	u32 block_id;
	u32 ret = NANDC_ERROR;
	u32 flash_addr;
    u32 bad_flag;
	u32* buffer	= NULL;
	u32* readbuffer = NULL;
	u32 length;
	struct nand_spec *spec = NULL;

	/* nand isinit */
    ret = nand_init();
    if(ret)
    {
	    cprintf(" Line %d\n", (int)__LINE__);
        goto fail;
    }

	/* get flash spec */
	spec = (struct nand_spec *)himalloc(sizeof(struct nand_spec));
    if(!spec)
    {
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}
	ret = nand_get_spec(spec);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

    /* find a good block */
    block_id = 8;
    while(1)
    {
        ret = nand_isbad(block_id, &bad_flag);
    	if(ret)
    	{
	        cprintf(" Line %d\n", (int)__LINE__);
    		goto fail;
    	}
        /* good block, break */
    	if(bad_flag != NANDC_BAD_BLOCK)
    	{
    		break;
    	}
        block_id++;
    }
    cprintf("good block found, block id = %d\n", block_id);

	/* get buffer */
	length = (spec->pagesize + YAFFS_BYTES_PER_SPARE) * (spec->blocksize / spec->pagesize);
	buffer = (u32*)himalloc(length);
	readbuffer = (u32*)himalloc(length);
	if((!buffer) || (!readbuffer))
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* write */
	flash_addr = block_id * spec->blocksize;
	memset(buffer, 0, length);
	ret = nand_write_oob(flash_addr, (u32)buffer, length, YAFFS_BYTES_PER_SPARE, 0);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* read */
	memset(readbuffer, 0xFF, length);
	ret = nand_read_oob(flash_addr, (u32)readbuffer, length, YAFFS_BYTES_PER_SPARE, 0);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* compare two buffer */
	ret = memcmp(buffer, readbuffer, length);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

    /* erase. because write 0 to the first page of a block equals mark it bad */
    ret = nand_erase(block_id);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

    hifree((void*)buffer);
    hifree((void*)readbuffer);
    hifree((void*)spec);
    cprintf("SUCCESS: nandc_test_case_004\n\n");
	return ;

fail:
	cprintf("FAIL:nandc_test_case_004, ret = %d\n\n", ret);

	if(spec)
		hifree((void*)spec);
	if(buffer)
		hifree((void*)buffer);
	if(readbuffer)
		hifree((void*)readbuffer);
	return ;

}

/******************************************************************************
* �� �� ��: nandc_test_case_005
* ��������: ����fastboot��nandc��������ecc��дdata
* �������: ��
* �������: ��
* �� �� ֵ: ��
* ����˵��: 1�������ڳ�ʼ���������ѯ�ͱ�ǹ���
*           2��step as follow
*               1 - find a good block form block 8;
*               2 - write "buffer" which is initialized with 0x00000000 to flash
*               3 - read back from flash to "readbuffer" which is initialized with 0xFFFFFFFF
*               4 - compare "buffer" and "readbuffer"
*******************************************************************************/
void nandc_test_case_005(void)
{
	u32 block_id;
	u32 ret = NANDC_ERROR;
    u32 bad_flag;
	u32* buffer	= NULL;
	u32* readbuffer = NULL;
	u32 length;
	struct nand_spec *spec = NULL;

	/* nand isinit */
	ret = nand_init();
    if(ret)
    {
	    cprintf(" Line %d\n", (int)__LINE__);
        goto fail;
    }

	/* get flash spec */
	spec = (struct nand_spec *)himalloc(sizeof(struct nand_spec));
    if(!spec)
    {
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}
	ret = nand_get_spec(spec);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

    /* find a good block */
    block_id = 8;
    while(1)
    {
        ret = nand_isbad(block_id, &bad_flag);
    	if(ret)
    	{
	        cprintf(" Line %d\n", (int)__LINE__);
    		goto fail;
    	}
        /* good block, break */
    	if(bad_flag != NANDC_BAD_BLOCK)
    	{
    		break;
    	}
        block_id++;
    }
    cprintf("good block found, block id = %d\n", block_id);

	/* get buffer */
	length = (spec->pagesize + spec->sparesize);
	buffer = (u32*)himalloc(length);
    if(!buffer)
    {
        cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
    }

	readbuffer = (u32*)himalloc(length);
	if(!readbuffer)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

    /* erase the block, it's needed, because nand_write_raw() doesn't contain "erase" operation */
    ret = nand_erase(block_id);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* write */
	memset(buffer, 0, length);
	ret = nand_write_raw(block_id * (spec->blocksize / spec->pagesize), 0, (u32)buffer, length, 0);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* read */
 	memset(readbuffer, 0xFF, length);
	ret = nand_read_raw(block_id * (spec->blocksize / spec->pagesize), 0, (u32)readbuffer, length, 0);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* compare two buffer */
	ret = memcmp(buffer, readbuffer, length);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

    /* erase. because write 0 to the first page of a block equals mark it bad */
	ret = nand_erase(block_id);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

    hifree((void*)buffer);
    hifree((void*)spec);
    cprintf("SUCCESS: nandc_test_case_005\n\n");
	return ;

fail:
	cprintf("FAIL:nandc_test_case_005, ret = %d\n\n", ret);

	if(spec)
		hifree((void*)spec);
	if(buffer)
		hifree((void*)buffer);
	if(readbuffer)
		hifree((void*)readbuffer);
	return ;
}

/******************************************************************************
* �� �� ��: nandc_test_case_006
* ��������: ����fastboot��nandc������ǻ��鹦��
* �������: ��
* �������: ��
* �� �� ֵ: ��
* ����˵��: 1������ı�ǺͲ�ѯ���ܲ��Ի���������
*           2��step as follow
*               1 - find a good block form block 8;
*               2 - mark it bad
*               3 - check whether it bad. if bad, success; else, fail.
*******************************************************************************/
void nandc_test_case_006(void)
{
	u32 block_id;
	u32 ret;
	u32 bad_flag;

    /* nand isinit */
	ret = nand_init();
    if(ret)
    {
	    cprintf(" Line %d\n", (int)__LINE__);
        goto fail;
    }

    /* find a good block */
    block_id = 8;
    while(1)
    {
        ret = nand_isbad(block_id, &bad_flag);
    	if(ret)
    	{
	        cprintf(" Line %d\n", (int)__LINE__);
    		goto fail;
    	}
        /* good block, break */
    	if(bad_flag != NANDC_BAD_BLOCK)
    	{
    		break;
    	}
        block_id++;
    }
    cprintf("good block found, block id = %d\n", block_id);

    /* erase the block, it's needed, because nand_write_raw() doesn't contain "erase" operation */
    ret = nand_erase(block_id);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* mark bad */
	ret = nand_bad(block_id);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* test if bad again */
	ret = nand_isbad(block_id, &bad_flag);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* bad, success */
	if(bad_flag != NANDC_BAD_BLOCK)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

    /* erase the block to make it good */
    ret = nand_erase(block_id);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

    cprintf("SUCCESS: nandc_test_case_006\n\n");
	return ;
fail:
	cprintf("FAIL:nandc_test_case_006, ret = %d\n\n", ret);
	return ;
}




/******************************************************************************
* �� �� ��: nandc_test_case_012
* ��������:
* �������: ��
* �������: ��
* �� �� ֵ: ��
* ����˵��: ����fastboot��nandc��������ecc��дdata+oob
*******************************************************************************/
void nandc_test_case_012(void)
{
	u32 block_id;
	u32 ret = NANDC_OK;
    u32 bad_flag;
	u32* buffer	= NULL;
	u32* readbuffer = NULL;
	u32 length;
	struct nand_spec *spec = NULL;

	/* nand isinit */
	ret = nand_init();
    if(ret)
    {
        goto fail;
    }

	/* get flash spec */
	spec = (struct nand_spec *)himalloc(sizeof(struct nand_spec));
    if(!spec)
    {
		goto fail;
	}
	ret = nand_get_spec(spec);
	if(ret)
	{
		goto fail;
	}

    /* find a good block */
    block_id = 0x8;
    while(1)
    {
        ret = nand_isbad(block_id, &bad_flag);
    	if(ret)
    	{
	        cprintf(" Line %d\n", (int)__LINE__);
    		goto fail;
    	}
        /* good block, break */
    	if(bad_flag != NANDC_BAD_BLOCK)
    	{
    		break;
    	}
        block_id++;
    }
    cprintf("good block found, block id = %d\n", block_id);

	/* erase  */
	ret = nand_erase(block_id);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* get buffer */
	length = (spec->pagesize + spec->sparesize);
	buffer = (u32*)himalloc(length);
	if(!buffer)
	{
		goto fail;
	}

	/* get read buffer */
	readbuffer = (u32*)himalloc(length);
	if(!readbuffer)
	{
		goto fail;
	}

	/* write */
	memset(buffer, 0, length);
	ret = nand_write_raw(block_id * (spec->blocksize / spec->pagesize), 0, (u32)buffer, length, 0);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* read */
	memset(readbuffer, 0xFF, length);
	ret = nand_read_raw(block_id * (spec->blocksize / spec->pagesize), 0, (u32)readbuffer, length, 0);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* compare two buffer */
	ret = memcmp(buffer, readbuffer, length);
	if(ret)
	{
		goto fail;
	}

    /* erase the block to mark it good */
    ret = nand_erase(block_id);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

    hifree((void*)buffer);
    hifree((void*)readbuffer);
    hifree((void*)spec);
    cprintf("SUCCESS: nandc_test_case_012\n\n");
	return ;

fail:
	cprintf("FAIL:nandc_test_case_012, ret = %d\n\n", ret);

	if(spec)
		hifree((void*)spec);
	if(buffer)
		hifree((void*)buffer);
	if(readbuffer)
		hifree((void*)readbuffer);
	return ;
}



/******************************************************************************
* �� �� ��: nandc_test_case_029
* ��������:
* �������: ��
* �������: ��
* �� �� ֵ: ��
* ����˵��: ����nandc ecc type = 1��ʱ��ľ�������. ecc type = 1ʱ��nandc�ڲ���
*           ת��Ϊecc type = 4������ʵ����ʹ�õ���4 bit ecc������������������
*           ��8bit / 1024 Bytesʱ��nandcӦ�ö��ܹ�����
*******************************************************************************/
void nandc_test_case_029(void)
{
    u32 block_id = 0x18, size = 0x800+0x40, nand_page_size = 0x800;
	u32 ret, i, j;
	/*��block��ַ�õ�flash�ĵ�ַ*/
    u32 flash_addr = block_id * nand_page_size * 64;
    u32* src_buffer = NULL;
    u32* temp_buffer = NULL;
    u32* buffer = NULL;

    /* nand isinit */
	ret = nand_init();
    if(ret)
    {
        goto erro;
    }

    /*����һҳ�Ŀռ�*/
    src_buffer = (u32 *)himalloc(size);
    temp_buffer = (u32 *)himalloc(size);
    if((!src_buffer) || (!temp_buffer))
    {
	    cprintf("ERROR: get buffer failed, Line %d\n", (int)__LINE__);
		goto erro;
	}
    memset((void *)src_buffer, 0, size);
    ret = nand_erase(block_id);
    if(ret)
    {
        cprintf("ERROR: nand erase error, Line %d\n", (int)__LINE__);
    	goto erro;
    }
    /* make error from 0 bit to 9 bits per 1024 bytes */
    for(i = 1; i <= 9; i++)
    {
        cprintf("Error bits = %d\n", i);
        /*д�����ݵ�nandflash��ȥ*/
        ret = nand_write(flash_addr, (u32)src_buffer, size, 0);
        if(ret)
        {
    	    cprintf("ERROR: nand write error, Line %d\n", (int)__LINE__);
    		goto erro;
    	}

        cprintf("Before nand_write\n");
        buffer =  src_buffer;
        PRINT_DATAS(buffer, 24, j);

        /* read from flash with ecc type = 0 */
        ret = nand_read_raw(flash_addr / nand_page_size, 0, (u32)temp_buffer, size, 0);
        if(ret)
        {
    	    cprintf("ERROR: nand read raw error, Line %d\n", (int)__LINE__);
    		goto erro;
    	}

        cprintf("After nand_read_raw\n");
        buffer =  temp_buffer;
        PRINT_DATAS(buffer, 24, j);

        /* erase */
        ret = nand_erase(block_id);
        if(ret)
        {
    	    cprintf("ERROR: nand erase error, Line %d\n", (int)__LINE__);
    		goto erro;
        }
		/*ʹҪд���������bitλ�Ĵ���*/
        buffer =  temp_buffer;
        for(j = 1; j <= i; j++)
        {
            *buffer = 1;
            buffer++;
        }

        /*�������bitλ������д�뵽nandflash��ȥ*/
        ret = nand_write_raw(flash_addr / nand_page_size, 0, (u32)temp_buffer, size, 0);
        if(ret)
        {
    	    cprintf("ERROR: nand write raw error, Line %d\n", (int)__LINE__);
    		goto erro;
    	}

        cprintf("After nand_write_raw\n");
        buffer =  temp_buffer;
        PRINT_DATAS(buffer, 24, j);

        /*�����������û�о���*/
        ret = nand_read_raw(flash_addr / nand_page_size, 0, (u32)temp_buffer, size, 0);
        if(ret)
        {
    	    cprintf("ERROR: nand read raw error, Line %d\n", (int)__LINE__);
    		goto erro;
    	}

        cprintf("After nand_read_raw\n");
        buffer =  temp_buffer;
        PRINT_DATAS(buffer, 24, j);

        /*������ʹ��ECC�Ķ�����,�����bitλ���п����Ѿ���������*/
        ret = nand_read(flash_addr, (u32)temp_buffer, size, 0);
        if(ret)
        {
    	    cprintf("ERROR: nand read error, Line %d\n", (int)__LINE__);
    		goto erro;
    	}

        cprintf("After nand_read\n");
        buffer =  temp_buffer;
        PRINT_DATAS(buffer, 24, j);

    }

    /* exit */
    cprintf("\nnandc_test_case_029 sueecss\n");
    return;

erro:
    if(src_buffer)
    {
	    hifree((void *)src_buffer);
	}
    if(temp_buffer)
    {
        hifree((void *)temp_buffer);
    }
    cprintf("\nnandc_test_case_029 failed, ret = %d\n", ret);
}

/******************************************************************************
* �� �� ��: nandc_test_case_030
* ��������:
* �������: ��
* �������: ��
* �� �� ֵ: ��
* ����˵��: ����nandc ecc type = 4��ʱ��ľ�������. ecc type = 1ʱ��nandc�ڲ���
*           ת��Ϊecc type = 4������ʵ����ʹ�õ���4 bit ecc������������������
*           ��8bit / 1024 Bytesʱ��nandcӦ�ö��ܹ�������
*           �������������case 29һ��
*******************************************************************************/
void nandc_test_case_030(void)
{
    nandc_test_case_029();
}


/******************************************************************************
* �� �� ��: nandc_test_case_031
* ��������:
* �������: ��
* �������: ��
* �� �� ֵ: ��
* ����˵��: ����nandc ecc type = 8��ʱ��ľ�������. ����������������16bit / 1024 Bytes
*           ʱ��nandcӦ�ö��ܹ�����
*******************************************************************************/
void nandc_test_case_031(void)
{
    u32 size = 0x800, nand_page_size = 0x800;
	u32 ret, i, j, wrong_value;
    u32 flash_addr = 0x100000;
    u32* src_buffer = NULL;
    u32* temp_buffer = NULL;

    /* nand isinit */
	ret = nand_init();
    if(ret)
    {
        goto erro;
    }

    /* get buffer */
    src_buffer = (u32 *)himalloc(size);
    temp_buffer = (u32 *)himalloc(size);
    if((!src_buffer) || (!temp_buffer))
    {
	    cprintf("ERROR: get buffer failed, Line %d\n", (int)__LINE__);
		goto erro;
	}

    /* memory init */
    memset((void *)src_buffer, 0, size);

    /* write to flash with ecc type = 8 */
    ret = nand_write(flash_addr, (u32)src_buffer, size, 0);
    if(ret)
    {
	    cprintf("ERROR: nand write error, Line %d\n", (int)__LINE__);
		goto erro;
	}

    /* read from flash with ecc type = 0 */
    ret = nand_read_raw(flash_addr / nand_page_size, 0, (u32)temp_buffer, size, 0);
    if(ret)
    {
	    cprintf("ERROR: nand read raw error, Line %d\n", (int)__LINE__);
		goto erro;
	}

    /* make error from 0 bit to 17 bits per 1024 bytes */
    for(i = 1; i <= 17; i++)
    {
        cprintf("\terror bits = %d\n", i);
        wrong_value = 0;

        /* make a wrong value contains error bits */
        for(j = 1; j <= i; j++)
        {
            wrong_value |= (1 << (j - 1));
        }

        cprintf("\twrong_value = 0x%x\n", wrong_value);
        memset((void *)temp_buffer, wrong_value, sizeof(u32));

        /* write to flash with ecc type = 0 */
        ret = nand_write_raw(flash_addr / nand_page_size, 0, (u32)temp_buffer, size, 0);
        if(ret)
        {
    	    cprintf("ERROR: nand write raw error, Line %d\n", (int)__LINE__);
    		goto erro;
    	}

        /* read from flash with ecc type = 8 */
        ret = nand_read(flash_addr, (u32)temp_buffer, size, 0);
        if(ret)
        {
    	    cprintf("ERROR: nand read error, Line %d\n", (int)__LINE__);
    		goto erro;
    	}
    }

    /* exit */
    cprintf("\nnandc_test_case_031 sueecss\n");
    return;

erro:
    if(src_buffer)
    {
	    hifree((void *)src_buffer);
	}
    if(temp_buffer)
    {
        hifree((void *)temp_buffer);
    }
    cprintf("\nnandc_test_case_031 failed, ret = %d\n", ret);
}

/******************************************************************************
* �� �� ��: nandc_test_case_032
* ��������:
* �������: ��
* �������: ��
* �� �� ֵ: ��
* ����˵��: ����bootģʽ��nandc�����������Ĺ��ܣ���������ʶ��1-6������
*******************************************************************************/
void nandc_test_case_032(void)
{
    /*
     * ���Գ���:
     * 1. block 0 - block 5���Ϊbad block
     * 2. ��bootloader(m3boot)�յ�block 6��ʼ��ַ��block 6����Ϊbad
     * ��������������bootloader�ܹ���������(�������Ϊ�����ܹ���ӡ123).
     *
     * ��ע��ļ���:
     * 1.mark block bad�Ĺ�����Ҫ��u32 nand_bad(u32 block_id)ʵ�֣�������ӿ�Ĭ��
     * ������block 0 bad�������Ҫ���ýӿ����ж�block_idΪ0�Ĵ���ע����
     * 2.��bootloader�յ�block 6��ʹ�÷��������صķ���ʵ�ֵģ���������:
     * d.load.elf fastboot.img.elf  // load burn tool, whose start address is axi
     * d.load ucos.bin 0x50000000   // load to DDR
     * r.s pc nand_write            // set pc pointer to nand_write
     * r.s r0 0xc0000               // set flash addr = block_size * 6, as block size is 0x20000
     * r.s r1 0x50000000            // set DDR addr
     * r.s r2 0x1000                // set write size two pages, actually one page is enough
     * r.s r3 0                     // set *skip_length NULL
     * set a breakpoint in the end of nand_write
     */
}



/******************************************************************************
* �� �� ��: nandc_test_case_033
* ��������: ����fastboot��nandc����bus��ecc��дdata+oob������Ĳ�����flash��ַ���ǿ����ģ�����Ҳ���ǿ�����
* �������: ��
* �������: ��
* �� �� ֵ: ��
*******************************************************************************/
void nandc_test_case_033(void)
{
	u32 ret = NANDC_ERROR;
	u32 flash_addr;
	u32* buffer	= NULL;
	u32* readbuffer = NULL;
	u32 length;
	struct nand_spec *spec = NULL;

	/* nand isinit */
    ret = nand_init();
    if(ret)
    {
	    cprintf(" Line %d\n", (int)__LINE__);
        goto fail;
    }

	/* get flash spec */
	spec = (struct nand_spec *)himalloc(sizeof(struct nand_spec));
    if(!spec)
    {
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}
	ret = nand_get_spec(spec);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}
    #if 0
    /* find a good block */
    block_id = 8;
    while(1)
    {
        ret = nand_isbad(block_id, &bad_flag);
    	if(ret)
    	{
	        cprintf(" Line %d\n", (int)__LINE__);
    		goto fail;
    	}
        /* good block, break */
    	if(bad_flag != NANDC_BAD_BLOCK)
    	{
    		break;
    	}
        block_id++;
    }
    cprintf("good block found, block id = %d\n", block_id);
    #endif
    ret = nand_erase(0x6c8);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}
    ret = nand_erase(0x6c9);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}
    cprintf(" erase completeLine %d\n", (int)__LINE__);
	/* get buffer */
	length = (spec->pagesize + YAFFS_BYTES_PER_SPARE) * (spec->blocksize / spec->pagesize)+0x810;
	//length=0x810;
	buffer = (u32*)himalloc(length);
	readbuffer = (u32*)himalloc(length);
    cprintf(" length 0x%x\n", length);
	if((!buffer) || (!readbuffer))
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* write */
	flash_addr = 0xd900000+0x800;
    memset(buffer, 0xaa, length);
    #if 0
    for(i=0;i<length/4;i++)
    {
        buffer[i]=i;
    }
    #endif
	ret = nand_write_oob(flash_addr, (u32)buffer, length, YAFFS_BYTES_PER_SPARE, 0);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* read */
	memset(readbuffer, 0xFF, length);
	ret = nand_read_oob(flash_addr, (u32)readbuffer, length, YAFFS_BYTES_PER_SPARE, 0);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* compare two buffer */
	ret = memcmp(buffer, readbuffer, length);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}
    #if 0
    /* erase. because write 0 to the first page of a block equals mark it bad */
    ret = nand_erase(block_id);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}
    #endif
    hifree((void*)buffer);
    hifree((void*)readbuffer);
    hifree((void*)spec);
    cprintf("SUCCESS: nandc_test_case_033\n\n");
	return ;

fail:
	cprintf("FAIL:nandc_test_case_033, ret = %d\n\n", ret);

	if(spec)
		hifree((void*)spec);
	if(buffer)
		hifree((void*)buffer);
	if(readbuffer)
		hifree((void*)readbuffer);
	return ;

}


/******************************************************************************
* �� �� ��: nandc_test_case_034
* ��������: ����fastboot���ecc��дdata,ע�������flashaddr���ǿ����Ҳ����ҳ����
* �������: ��
* �������: ��
* �� �� ֵ: ��
*******************************************************************************/
void nandc_test_case_034(void)
{
	u32 ret = NANDC_ERROR;
	u32 flash_addr;
	u32* buffer	= NULL;
	u32* readbuffer = NULL;
	u32 length;
	struct nand_spec *spec = NULL;

	/* nand isinit */
    ret = nand_init();
    if(ret)
    {
	    cprintf(" Line %d\n", (int)__LINE__);
        goto fail;
    }
	/* get flash spec */
	spec = (struct nand_spec *)himalloc(sizeof(struct nand_spec));
    if(!spec)
    {
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}
	ret = nand_get_spec(spec);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}
    ret = nand_erase(0x6c8);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}
    ret = nand_erase(0x6c9);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}
    cprintf(" erase completeLine %d\n", (int)__LINE__);
	/* get buffer */
	length = spec->blocksize +0x810;
	//length=0x810;
	buffer = (u32*)himalloc(length);
	readbuffer = (u32*)himalloc(length);
    cprintf(" length 0x%x\n", length);
	if((!buffer) || (!readbuffer))
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* write */
	flash_addr = 0xd900000+0x456;
    memset(buffer, 0xaa, length);
    #if 0
    for(i=0;i<length/4;i++)
    {
        buffer[i]=i;
    }
    #endif

	ret = nand_write(flash_addr, (u32)buffer, length, 0);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* read */
	memset(readbuffer, 0xFF, length);
	ret = nand_read(flash_addr, (u32)readbuffer, length, 0);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}

	/* compare two buffer */
	ret = memcmp(buffer, readbuffer, length);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}
    #if 0
    /* erase. because write 0 to the first page of a block equals mark it bad */
    ret = nand_erase(block_id);
	if(ret)
	{
	    cprintf(" Line %d\n", (int)__LINE__);
		goto fail;
	}
    #endif
    hifree((void*)buffer);
    hifree((void*)readbuffer);
    hifree((void*)spec);
    cprintf("SUCCESS: nandc_test_case_034\n\n");
	return ;

fail:
	cprintf("FAIL:nandc_test_case_034, ret = %d\n\n", ret);

	if(spec)
		hifree((void*)spec);
	if(buffer)
		hifree((void*)buffer);
	if(readbuffer)
		hifree((void*)readbuffer);
	return ;

}




#endif /* #ifdef __FASTBOOT__ */

/******************************************************************************
* �� �� ��: test_nandc
* ��������: ���Ժ������
* �������: ��
* �������: ��
* �� �� ֵ: ��
* ����˵��: ��
*******************************************************************************/
void test_nandc(void)
{
    #if 0
    nandc_test_case_001();

	nandc_test_case_006();
	nandc_test_case_002();
	nandc_test_case_003();
	nandc_test_case_004();
	nandc_test_case_005();
    #endif
    cprintf(" wsq nandc_test_case_033 \n");
    nandc_test_case_033();
    cprintf(" wsq nandc_test_case_034 \n");
    nandc_test_case_034();

/*
	nandc_test_case_007();
	nandc_test_case_008();
	nandc_test_case_009();
	nandc_test_case_010();
	nandc_test_case_011();
	nandc_test_case_012();
	nandc_test_case_013();
	nandc_test_case_014();
	nandc_test_case_029();
	nandc_test_case_030();
	nandc_test_case_031();
*/

}
/*************************************** END: test nandc ********************************************/


/*************************************** BEGIN: test nandc "bsp" interface ********************************************/

#ifdef __FASTBOOT__
#define PRINT_INFO cprintf
#define MALLOC(x) alloc(x)
#define KFREE(x)
#endif

/*��������������Ǳ�test_bsp���õ�*/
/*��д����������*/
void test_bsp_nand_read_write(char *partition_name, unsigned int partition_offset, int length)
{
    unsigned char *src_buffer = NULL;
    unsigned char *read_buffer = NULL;
    int ret;

    /* get ram buffer */
	src_buffer = (unsigned char *)MALLOC(length);
	read_buffer = (unsigned char *)MALLOC(length);
	if(!src_buffer || !read_buffer)
	{
		PRINT_INFO("get ram buffer failed!\n");
		goto erro;
	}

    memset(src_buffer, 0xAA, length);
	/*��ʵ���л����nandflashһ�β�Ҫ����������֤����*/
    ret = bsp_nand_write(partition_name, partition_offset, src_buffer, length);
    if(ret)
    {
        PRINT_INFO("ERROR: bsp_nand_write error, ret = %d.\n", ret);
        goto erro;
    }
    else
    {
        PRINT_INFO("SUCCESS: bsp_nand_write success.\n");
    }

    ret = bsp_nand_read(partition_name, partition_offset, read_buffer, length, NULL);
    if(ret)
    {
        PRINT_INFO("ERROR: bsp_nand_read error, ret = %d.\n", ret);
        goto erro;
    }
    else
    {
        PRINT_INFO("SUCCESS: bsp_nand_read success.\n");
    }

    ret = memcmp(src_buffer, read_buffer, length);
    if(ret)
    {
        PRINT_INFO("ERROR: mem_compare error, ret = %d.\n", ret);
        goto erro;
    }
    else
    {
        PRINT_INFO("SUCCESS: mem_compare success.\n");
    }

    KFREE(src_buffer);
    KFREE(read_buffer);
    PRINT_INFO("SUCCESS: bsp_nand_read_write success.\n");
    return ;

erro:
    if(src_buffer)
    {
        KFREE(src_buffer);
    }
    if(read_buffer)
    {
        KFREE(read_buffer);
    }
    PRINT_INFO("ERROR: bsp_nand_read_write failed.\n");
    return ;
}
/*�����������Ĺ��ܲ���*/
void test_bsp_bad_block_mangement(char *partition_name, unsigned int partition_offset)
{
#ifdef __FASTBOOT__
    int error_code;
    int block_id;
	struct nandc_host  * host   = NULL;
    ptentry *p = NULL;

    p = flash_find_ptn(partition_name);

    if(p == 0)
    {
        PRINT_INFO("ERROR: test_bsp_bad_block_mangement can't find partition %s\n", partition_name);
        return;
    }

    host = nandc_nand_host;
    if(!host)
    {
        error_code = -2;
        goto erro;
    }

    /*������һ���ÿ�*/
    while(1)
    {
        error_code = bsp_nand_isbad(partition_name, partition_offset);
        if(!error_code)
        {
            PRINT_INFO("good block found, offset: 0x%x\n", partition_offset);
			/*���ҵ��ÿ���˳�*/
            break;
        }
        else if(error_code == 1) /* bad block, skip */
        {
            partition_offset += host->nandchip->spec.blocksize;
			/*�ǻ���͹��˵�*/
            continue;
        }
        else /* error */
        {
            PRINT_INFO("ERROR: test_bsp_bad_block_mangement check bad error\n");
            goto erro;
        }
    }

    /* get the block id */
    block_id = (p->start + partition_offset) / host->nandchip->spec.blocksize;

    /* mark bad */
    error_code = nand_bad(block_id);
    if(error_code)
    {
        PRINT_INFO("ERROR: test_bsp_bad_block_mangement mark bad error\n");
        goto erro;
    }

    /*������Ƿ��ǻ���*/
    error_code = bsp_nand_isbad(partition_name, partition_offset);
    if(1 != error_code) /* should be bad block */
    {
        PRINT_INFO("ERROR: test_bsp_bad_block_mangement check bad error, again\n");
        goto erro;
    }

    cprintf("SUCCESS: test_bsp_bad_block_mangement successful.\n");
    return;

erro:
    cprintf("ERROR: bsp_nand_isbad failed, error_code = %d.\n", error_code);
#endif
}

#ifdef __FASTBOOT__
/******************************************************************************
* �� �� ��: test_bsp_nv_flag
* ��������:
* �������: ��
* �������: ��
* �� �� ֵ: ��
* ����˵��: ����NV flag�Ķ�д.д������A������fastboot��ֻ���������ӿ�Ҫ��A�˵�test_bsp_nv_flag���ʹ��
*
*******************************************************************************/
void test_bsp_nv_flag(void)
{
    unsigned char flag;
    int ret = NANDC_ERROR;
    char *partition_name = PTABLE_RECOVERYB_NM;
    unsigned int partition_offset;
    struct nand_spec spec;

    ret = bsp_get_nand_info(&spec);
    if(ret)
    {
		cprintf("get nand spec error\n");
        goto erro;
    }

    /*��һ�������ı�־λ*/
    partition_offset = 0;
	/*������4��block�ı�־λ��ֵ*/
    cprintf("\n[1] partition offset 0x%x\n", partition_offset);
    ret = bsp_nand_read_flag_nv(partition_name, partition_offset, &flag);
    if(ret)
    {
		cprintf("read nv flag error, offset = 0x%x, ret = %d.\n", partition_offset, ret);
		goto erro;
    }
    if(flag != NV_WRITE_SUCCESS)
    {
		cprintf("flag value error, offset = 0x%x, flag = %d.\n", partition_offset, flag);
    }
    else
    {
        cprintf("[%s] SUCCESS: test read nv flag success, offset = 0x%x.\n", __FUNCTION__, partition_offset);
    }

    partition_offset += spec.blocksize;
    cprintf("\n[2] partition offset 0x%x\n", partition_offset);
    ret = bsp_nand_read_flag_nv(partition_name, partition_offset, &flag);
    if(ret)
    {
		cprintf("read nv flag error, offset = 0x%x, ret = %d.\n", partition_offset, ret);
		goto erro;
    }
    if(flag != NV_WRITE_SUCCESS)
    {
		cprintf("flag value error, offset = 0x%x, flag = %d.\n", partition_offset, flag);
    }
    else
    {
        cprintf("[%s] SUCCESS: test read nv flag success, offset = 0x%x.\n", __FUNCTION__, partition_offset);
    }

    partition_offset += spec.blocksize;
    cprintf("\n[3] partition offset 0x%x\n", partition_offset);
    ret = bsp_nand_read_flag_nv(partition_name, partition_offset, &flag);
    if(ret)
    {
		cprintf("read nv flag error, offset = 0x%x, ret = %d.\n", partition_offset, ret);
		goto erro;
    }
    if(flag != NV_WRITE_SUCCESS)
    {
		cprintf("flag value error, offset = 0x%x, flag = %d.\n", partition_offset, flag);
    }
    else
    {
        cprintf("[%s] SUCCESS: test read nv flag success, offset = 0x%x.\n", __FUNCTION__, partition_offset);
    }

    partition_offset += spec.blocksize;
    cprintf("\n[4] partition offset 0x%x\n", partition_offset);
    ret = bsp_nand_read_flag_nv(partition_name, partition_offset, &flag);
    if(ret)
    {
		cprintf("read nv flag error, offset = 0x%x, ret = %d.\n", partition_offset, ret);
		goto erro;
    }
    if(flag == NV_WRITE_SUCCESS)
    {
		cprintf("flag value error, offset = 0x%x, flag = %d.\n", partition_offset, flag);
    }
    else
    {
        cprintf("[%s] SUCCESS: test read nv flag success, offset = 0x%x.\n", __FUNCTION__, partition_offset);
    }

    return;
erro:
    return;
}

#endif
/******************************************************************************
* �� �� ��: test_bsp
* ��������:
* �������: ��
* �������: ��
* �� �� ֵ: ��
* ����˵��: ����nandc��bsp��ͷ�Ľӿ�
*******************************************************************************/
/*��һ�������Ĳ���*/
void test_bsp(void)
{
    char *partition_name = PTABLE_RECOVERYB_NM;
    unsigned int partition_offset;
    int length, i = 1;
    int page_size = FLASH_PAGE_SIZE;

    /* just one block */
    PRINT_INFO("\nTest case %d:\n", i++);
    partition_offset = 0;
	/*һ��block�ĳ���*/
    length = page_size * PAGESPERBLOCK;
    PRINT_INFO("partition = %s, offset = 0x%x, length = 0x%x\n", partition_name, partition_offset, length);
    test_bsp_nand_read_write(partition_name, partition_offset, length);

    /* half block + one block + half block */
    PRINT_INFO("\nTest case %d:\n", i++);
    partition_offset = PAGESPERBLOCK * page_size / 2;
    length = page_size * (PAGESPERBLOCK * 2);
    PRINT_INFO("partition = %s, offset = 0x%x, length = 0x%x\n", partition_name, partition_offset, length);
    test_bsp_nand_read_write(partition_name, partition_offset, length);

    /*һҳ���ݵĶ�д����*/
    PRINT_INFO("\nTest case %d:\n", i++);
    partition_offset = 0;
    length = page_size;
    PRINT_INFO("partition = %s, offset = 0x%x, length = 0x%x\n", partition_name, partition_offset, length);
    test_bsp_nand_read_write(partition_name, partition_offset, length);

    /*һҳ���ݵĶ�д*/
    PRINT_INFO("\nTest case %d:\n", i++);
    partition_offset = page_size / 2;
    length = page_size;
    PRINT_INFO("partition = %s, offset = 0x%x, length = 0x%x\n", partition_name, partition_offset, length);
    test_bsp_nand_read_write(partition_name, partition_offset, length);

    /*��ҳ���ݵĶ�д*/
    PRINT_INFO("\nTest case %d:\n", i++);
    partition_offset = page_size / 2;
    length = 1024;
    PRINT_INFO("partition = %s, offset = 0x%x, length = 0x%x\n", partition_name, partition_offset, length);
    test_bsp_nand_read_write(partition_name, partition_offset, length);

    /*���Ի������*/
    PRINT_INFO("\nTest case %d:\n", i++);
    partition_offset = 0;
    PRINT_INFO("partition = %s, offset = 0x%x\n", partition_name, partition_offset);
    test_bsp_bad_block_mangement(partition_name, partition_offset);

}

/*************************************** END: test nandc "bsp" interface ********************************************/

#ifdef __cplusplus
}
#endif
