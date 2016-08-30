
/******************************************************************************
*    Copyright (c) 2009-2011 by  Hisilicon Tech. Co., Ltd.
*    All rights reserved.
* ***
*
******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

#include "nandc_inc.h"


/**
* ����:�˺�����Ҫ���ڲ�ʹ��DMA����ʱ����ʹ��ECC����ʱ������ҳ��ʱ����,��Ϊ���ܳ���nandc��Ӳ���������Ŀռ�С��
*	   Flashҳ��Ĵ�Сʱ��Ҫ��δ���
*
* ����: 
* @host                ---nandc_host�Ľṹ��ָ��
* @pagemap             ---һҳ��flash���ݵĸ�ʽ
* @times               ---��ʾ�����Ǵ��͵ĵڼ���
* @access              ---��ʾ�Ƕ���������д����
*/
u32 nandc_data_transmit_page(struct nandc_host *host, struct nandc_pagemap *pagemap, u32 times, u32 access)
{
	/*lint !e578 */
	u32 bufoffset;
	u32 ramoffset;
	u32 copylen;
	u32 i;
	/*�����Ӳ���������ĵ�ַ��ͬ�Ͳ��ø���������*/
	if(host->databuf == host->bufbase)
	{
		return NANDC_OK;
	}

	/*����������*/
	copylen = pagemap[times].data.lens;

	if((0 != copylen) && (NANDC_NULL != host->databuf))
	{
		bufoffset = pagemap[times].data.addr;
		i = 0;
		ramoffset = 0;
		while(i < times)
		{
		    ramoffset += pagemap[i].data.lens;
		    i++;
		}
		if(NAND_CMD_READSTART == access)
		{
			/*����Ƕ�������ô�Ͱ�Ӳ���������е����ݸ��Ƶ�databuf��ȥ*/
		    memcpy((void*)((u32)host->databuf + ramoffset), (const void*)((u32)(host->bufbase) + bufoffset), copylen);
		}
		else 
		{
			/*�����д������ô��Ҫд�����ݸ��Ƶ�Ӳ����������*/
		    memcpy((void*)((u32)(host->bufbase) + bufoffset),(const void*)((u32)host->databuf + ramoffset),  copylen);
		}
	}

	/*����OOB��������*/
	copylen     =   pagemap[times].oob.lens;
	/*OOB�����ݿ�ʼ�ĵط�*/
	bufoffset   =   pagemap[times].oob.addr ;

	if((0 != copylen)&&(NANDC_NULL != host->oobbuf))
	{
		i = 0;
		ramoffset = 0;
		while(i < times)
		{
		    ramoffset += pagemap[i].oob.lens;
		    i++;
		}
		if(NAND_CMD_READSTART == access)
		{
			/*����OOB�����ݵ��û��ڴ���*/
		    memcpy((void*)((u32)host->oobbuf + ramoffset), (const void*)((u32)host->bufbase + bufoffset), copylen);
		}
		else /*NAND_CMD_PAGEPROG*/
		{
			/*��OOB������дӲ����������ȥ*/
		    memcpy((void*)((u32)host->bufbase + bufoffset), (const void*)((u32)host->oobbuf + ramoffset), copylen);
		}
	}
	else if((0 != copylen)&&(NAND_CMD_READSTART != access))
	{
		/*������д(�����spare�� )ģʽ��ȷ�������־��Ϊ0*/
		memset((void*)((u32)host->bufbase + bufoffset), 0xff , copylen);
	}

	return NANDC_OK;
}


/**
* ����:�˺�����Ҫ���ڲ�ʹ��DMA����ʱ���ڲ�ʹ��ECC����ʱ�������ʱ����,��Ϊ���ܳ���nandc��Ӳ���������Ŀռ�С��
*	   Flashҳ��Ĵ�Сʱ��Ҫ��δ���
*
* ����: 
* @host                    ---nandc_host�Ľṹ��ָ��
* @datasize                ---Ҫ���͵����ݵĳ���
* @bufoffset               ---bufoffset��ʾ�û������е�ƫ��
* @access                  ---��ʾ�Ƕ���������д����
*/
u32 nandc_data_transmit_raw(struct nandc_host *host, u32 datasize, u32 bufoffset, u32 access)
{
	/*lint !e578 */
	if(host->databuf == host->bufbase)
	{
		return NANDC_OK;
	}
	/*��nand�Ĳ���*/
	if(NAND_CMD_READSTART == access)
	{
		memcpy(host->databuf + bufoffset, host->bufbase, datasize);
	}
	else /*дnand�Ĳ���*/
	{
		memcpy( host->bufbase, host->databuf + bufoffset, datasize);
	}

	return NANDC_OK;
}

#ifdef __cplusplus
}
#endif


