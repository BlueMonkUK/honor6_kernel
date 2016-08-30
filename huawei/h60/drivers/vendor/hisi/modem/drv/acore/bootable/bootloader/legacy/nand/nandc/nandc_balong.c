/*******************************************************************************
* Copyright (C), 2008-2011, HUAWEI Tech. Co., Ltd.
*
* Module name: Hisilicon Nand Flash Controller Dirver
*
* Description: the Hisilicon Nand Flash Controller physical and logical driver
*
* Filename:    nand_balong.c
* Description: nandcģ����fastboot�׶��ṩ����Ľӿں���,����ģ��Ҫ���õĽӿں������ڴ˴��ļ���ʵ��
*			   ���ļ������þ����ṩ����ģ���API�ӿں���,�����ļ��о���ĺ���ʵ�����ǵ���nandc_nand.c
*			   �ļ��еľ��庯��
*******************************************************************************/
/* Description:���ļ���nandcģ�����������ļ��Ĺ�ϵͼ������ʾ
*
*
*              nand_balong.c               nandcģ���ṩ����Ľӿں������ڴ��ļ���ʵ��,�ڴ��ļ��е�������ĺ���ָ��
*                   |
*                   |
*              nandc_nand.c                nandcģ���о������nand�Ĳ������������������ʵ�֣�������ĺ���ָ�븳ֵ
*                   |
*                   |
*        ------------------------
*          |                  |
*          |(init)            |(operation)
*       nandc_host.c       nandc_ctrl.c                Layer: ������������
*          |           -----------------------
*      nandc_native.c         |
*                      nandc_vxxx.c(nandc_600.c)       Layer: �����Ӳ������
*
******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __FASTBOOT__


#include <ptable_inc.h>
#include "nandc_balong.h"
extern char* strncpy(char *dest, const char *src, unsigned int count);
/*nandcģ���ȫ�ֱ���,�����˲���nandflash�ľ��庯��*/
struct nand_bus  nand_handle = {0, 0};

/**
* ����:nandcģ���ṩ�����ʼ��nandc�������ĺ����ӿ�
*
* ����:
* ��
* ����:��ʼ��nandc�����������ݽṹ�����õײ��nandcӲ���������ĳ�ʼ������
*
*/
u32 nand_init(void)
{
	struct nand_bus* handle = NULL;
	u32 result = NAND_ERROR;

	handle = &nand_handle;
	/*����Ѿ���ʼ������˾�ֱ���˳���*/
	if(NAND_INIT_MAGIC == handle->init_flag)
	{
		result = NAND_OK;
		NAND_TRACE(("!!nand module already  inited!!\n"));
		goto EXIT;
	}

	memset((void*)handle, 0x00, sizeof(nand_handle));

	/*ָ�����Ĳ���nandflash�Ĳ�������,��������������nandflash�ĺ��������˱仯ʱ,���Ա�֤�������ṩ�����ӿڲ���*/
	handle->funcs  =  &nandc_nand_funcs;

	/*nandflashӲ���������ľ����ʼ������*/
	if(handle->funcs->init)
	{
		result = handle->funcs->init();
	}
	else
	{
		NAND_TRACE(("ERRO!!nand module init is NULL\n"));
		result = NAND_ERROR;
	}
	/*��ʼ����ɺ����ñ�־λ*/
	if(NAND_OK == result)
	{
		handle->init_flag = NAND_INIT_MAGIC;
	}
	else
	{
		NAND_TRACE(("ERRO!!nand module init failed\n"));
	}

EXIT:
	return result;

}


/**
* ����:nandcģ���ṩ����ͨ�������������ҷ�����ĺ����ӿ�
*
* ����:
* @partition_name     ---���������
* ����:ͨ�����������ط�������Ϣ,���ҳɹ����ط�����ָ�룬���Ҳ��ɹ�����NULL
*
*/
struct ST_PART_TBL * find_partition_by_name(const char *partition_name)
{
	struct ST_PART_TBL * ptable = NULL;

	ptable_ensure();

	ptable = ptable_get_ram_data();

	/*�������еķ�������Ϣ*/
	while(0 != strcmp(PTABLE_END_STR, ptable->name))
	{
		/*���ҵ����˳�*/
	    if(0 == strcmp(partition_name, ptable->name))
	    {
	        break;
	    }
	    ptable++;
	}
	/*�����Ϊ�վͱ�ʾ���ҵ���Ҫ�ķ�������Ϣ*/
	if(0 == strcmp(PTABLE_END_STR, ptable->name))
	{
		NAND_TRACE(("ERROR: can't find partition %s, function %s\n", partition_name, __FUNCTION__));
	    return NULL;
	}

	return ptable;

}

/**
* ����:nandcģ�����к�������ͨ��ʵ�ֹ���
*
* ����:
* @param          ---������Ҫ����nandflash�ľ��������Ϣ
* @func_type      ---����������,���ݴ�ֵ֪��Ҫʵ��ʲô����(����д)
*
*
* ����:���еĺ������ܶ���ͨ���������������ͨ��func_type��ȷ����Ҫʵ��ʲô���ܣ�ֱ�ӵ��þ���ĺ���ָ����
* ִ�о���Ĳ���,����Ҫ�����µĺ�������ʱ�����ڴ�ͨ������������������ص����̾���,������湦�ܵ���չ
*
*/
u32 nand_run(struct nand_param* param, u32 func_type)
{
	struct nand_bus* handle;
	struct nand_interface * nandfuncs;
	u32 result = NAND_OK;

	handle = &nand_handle;
	/*�ж��Ƿ��ʼ���ɹ�*/
	if(NAND_INIT_MAGIC != handle->init_flag)
	{
		NAND_TRACE(("error!! balong nand not inited\n"));
		return NAND_ERROR;
	}
	/*�õ�����Ĳ����ĺ���ָ��*/
	nandfuncs = handle->funcs;

	switch (func_type)
	{
		case READ_NAND:
		{
			/*ʹ��ECC���ܵĶ�Flash����*/
			if(nandfuncs->read)
			{
				result = nandfuncs->read(param->addr_flash,
			                         param->addr_data,
			                         param->size_data,
			                         param->size_oob,
			                         param->skip_length);
			}
			else
			{
				NAND_TRACE(("error!! func read_random is NULL\n"));
				result = NAND_ERROR;
			}
		}
		break;

		case READ_RAW:
		{
			/*��ʹ��ECC���ܵ����Flash����*/
			if(nandfuncs->read_page_raw)
			{
				result = nandfuncs->read_page_raw(param->number,
			                                  (u32)param->addr_flash,
			                                  (u32)param->addr_data,
			                                       param->size_data);
			}
			else
			{
				NAND_TRACE(("error!! func read_page_yaffs is NULL\n"));
				result = NAND_ERROR;
			}
		}
		break;

		case QUARY_BAD:
		{
			/*��ѯ����Ĺ��ܺ���*/
			if(nandfuncs->quary_bad)
			{
				result = nandfuncs->quary_bad(param->number, (u32*)param->addr_data);
			}
			else
			{
				NAND_TRACE(("error!! func quary_bad is NULL\n"));
				result = NAND_ERROR;
			}
		}
		break;

		case ERASE_BLOCK:
		{
			/*�������ݿ�Ĺ��ܺ���*/
			if(nandfuncs->erase_by_id)
			{
				result = nandfuncs->erase_by_id(param->number);
			}
			else
			{
				NAND_TRACE(("error!! func erase_by_id is NULL\n"));
				result = NAND_ERROR;
			}
		}
		break;

		case WRITE_NAND:
		{
			/*ʹ��ECC���ܵ�дFlash����,ע��write�л������Ȳ���Flash����д,�����û��Ͳ�����д֮ǰ������*/
			if(nandfuncs->write)
			{
				result = nandfuncs->write(param->addr_flash,
			                          	  param->addr_data,
			                              param->size_data,
			                              param->size_oob,
			                              param->skip_length);
			}
			else
			{
				NAND_TRACE(("error!! func write flash is NULL\n"));
				result = NAND_ERROR;
			}
		}
		break;

		case WRITE_RAW:
		{
			/*��ʹ��ECC���ܵ���дFlash����*/
			if(nandfuncs->write_page_raw)
			{
				result = nandfuncs->write_page_raw(param->number,
			                                    (u32)param->addr_flash,
			                                    (u32)param->addr_data,
			                                    param->size_data);
			}
			else
			{
				NAND_TRACE(("error!! func write_page_raw is NULL\n"));
				result = NAND_ERROR;
			}
		}
		break;

		case MARK_BAD:
		{
			/*�껵��Ĺ���*/
			if(nandfuncs->mark_bad)
			{
				result = nandfuncs->mark_bad(param->number );
			}
			else
			{
				NAND_TRACE(("error!! func mark_bad is NULL\n"));
				result = NAND_ERROR;
			}
		}
		break;
		default:
			NAND_TRACE(("ERRO!! nand function type:%d not defined\n",func_type));
			result = NAND_ERROR;
		break;
	}
	/*���ز����Ľ��,0��ʾ�����ɹ���1��ʾ����ʧ��*/
	return result;
}


/**
* ����:nandcģ��ʹ��ECC���ܵĶ����ݲ���,ע��˺����Ķ��������ܶ�OOB����
*
* ����:
* @flash_addr          ---Ҫ����Flash��ַ
* @dst_data      	   ---Ҫ�������ڴ��ַ
* @read_size      	   ---Ҫ��������
* @skip_length         ---���˵ĳ���,���ڶ��Ĺ�������������ʱ����Ҫ���Ŀ飬�˽ṹ��¼�����ĳ���
*
*
* ����:ʵ�ֵĹ�����ʹ��ECC�Ķ����ݲ���,ע�ⲻ�ܶ���OOB�����ݣ�flash_addr,read_size��Щ������û������
* ���ȹ���nand_param�����������Ҫ����Ĳ�����ֵ,��ͨ��nand_runͨ�ô�������ʵ����ع���
*/
u32 nand_read(FSZ flash_addr, u32 dst_data, u32 read_size, u32* skip_length)
{
    struct nand_param param = {0, 0, 0, 0, 0, 0, 0};
	/*��Ҫ����Ĳ�����ֵ*/
    param.addr_flash    =   flash_addr;
    param.addr_data     =   dst_data;
    param.size_data     =   read_size;
    param.skip_length   =   skip_length;
	/*param.size_oobΪ0�������ֻ�����ݲ���OOB*/
	/*nandflashʹ��ECC���ܵĶ����ݲ���*/
    return nand_run(&param, READ_NAND);

}

/**
* ����:nandcģ���ṩ����õ�nandflash��ع������ĺ����ӿ�
*
* ����:
* @spec      ---��nandflash�е���ز�����д�ڴ�ָ�������ȥ
*
* ����:�õ�nandflash�Ĺ�����
*/
u32 nand_get_spec(struct nand_spec *spec)
{
    struct nand_bus* handle;
    struct nand_interface * nandfuncs;

    handle = &nand_handle;
	/*���û�г�ʼ���ͷ��ش���*/
    if(NAND_INIT_MAGIC != handle->init_flag)
    {
        NAND_TRACE(("error!! balong nand not inited\n"));
        return NAND_ERROR;
    }

    nandfuncs = handle->funcs;
	/*�õ�flash�Ĺ�����*/
    if(nandfuncs->get_spec)
    {
        return  nandfuncs->get_spec(spec);
    }
	/*�ߵ������˵���д������ߴ���������*/
    NAND_TRACE(("error!! get_spec not inited\n"));
    return NAND_ERROR;
}

/**
* ����:nandcģ���ṩ�����OOB���ݺ�Flash���ݵĺ����ӿ�
*
* ����:
* @flash_addr    ---Ҫ����flash��ַ
* @dst_data      ---���ݶ����ڴ�ĵ�ַ,�������ַ�д�Ŷ��ĵ�Flash���ݺ�OOB����
* @data_size     ---Ҫ�����ݵĴ�С,������ݳ��ȱ�ʾ��Ҫ����Flash���ݺ�OOB����֮��
* @oob_per_page  ---ÿһҳ����Ҫ����OOB���ݵĴ�С
* @skip_length   ---���˵ĳ���,���ڶ��Ĺ�������������ʱ����Ҫ���Ŀ飬�˽ṹ��¼�����ĳ���
*
*
* ����:��Flash��OOB�����ݴ�ŵ�dst_data���ڴ���ȥ�����ڴ������ݴ�ŵĸ�ʽ��:pagesize(flashdata)+oob_per_page(oobdata)+pagesize(flashdata)+oob_per_page(oobdata)
* ʹ��ע��:��oob_per_page��Ϊ0ʱ��flash_addrҪҳ��ַ���룬��һ���ʹ��ʱdata_size��(pagesize+oob_per_page)��������
*/
u32 nand_read_oob(FSZ flash_addr, u32 dst_data, u32 data_size, u32 oob_per_page, u32* skip_length)
{
    struct nand_param param = {0, 0, 0, 0, 0, 0, 0};
	/*��Ҫ����Ĳ�����ֵ*/
    param.addr_flash    =   flash_addr;
    param.addr_data     =   dst_data;
    param.size_data     =   data_size;
	/*Ҫ����OOB��С��Ϊ0��ʾ��ʹ��ECC���ܵĴ�OOB���ݵĶ�����*/
    param.size_oob      =   oob_per_page;
    param.skip_length   =   skip_length;
	/*nandflashʹ��ECC���ܵĶ����ݲ���*/
    return nand_run(&param, READ_NAND);

}


/**
* ����:nandcģ���ṩ�������Flash���ݵĺ����ӿ�,�ڶ��Ĺ����в�ʹ��ECC
*
* ����:
* @src_page      ---Ҫ����flashҳ��ַ
* @offset_data   ---Ҫ����flashҳ��ƫ�Ƶ�ַ
* @dst_data      ---���ݶ����ڴ�ĵ�ַ,�������ַ�д�ŵ����ݸ�ʽ��Flash���������ݸ�ʽ
* @len_data      ---Ҫ�����ݵĴ�С
* @skip_length   ---���˵ĳ���,���ڶ��Ĺ�������������ʱ����Ҫ���Ŀ飬�˽ṹ��¼�����ĳ���
*
*
* ����:�˹���Ҫ�껵��ʱ���ú��ڵ���ECC errorʱ�������ô˹��ܰ�flash�е����ݶ���������ecc error��ԭ��,
*һ���ʹ�������offset_dataΪ0��len_dataΪpagesize+sparesize�Ĵ�С��ע��offset_data+len_data���ܳ���pagesize+sparesize�Ĵ�С
*/
/*ע��������ݲ��ܳ���һҳ�Ĵ�С*/
u32 nand_read_raw(u32 src_page, u32 offset_data, u32 dst_data, u32 len_data, u32* skip_length)
{
    struct nand_param param = {0, 0, 0, 0, 0, 0, 0};
	/*ע��src_page��ʾ����flash��ҳ��ַ*/
    param.number        = src_page;
    param.addr_flash    = offset_data;
    param.addr_data     = dst_data;
    param.size_data     = len_data;
    param.skip_length   = skip_length;
	/*nandflash��ʹ��ECC���ܵ�������ݲ���*/
    return nand_run(&param, READ_RAW);

}


/**
* ����:nandcģ���ṩ�����жϴ�block�Ƿ��ǻ��麯���ӿ�
*
* ����:
* @blockID       ---Ҫ���Ŀ��
* @flag          ---�Ѽ��ı�־ֵ����ڴ˱�����ȥ,0Ϊ�û���1Ϊ����
*
* ����:����block�Ƿ��ǻ��飬���ѱ��ֵ�����flag��־��
*/
u32 nand_isbad(u32 blockID, u32 *flag)
{
    struct nand_param param = {0, 0, 0, 0, 0, 0, 0};
	/*���ò�ѯ�Ŀ��*/
    param.number    =   blockID;
	/*�ѱ�־λ�ĵ�ַ����*/
    param.addr_data =   (u32)flag;

    return nand_run(&param, QUARY_BAD);

}

/**
* ����:nandcģ���ṩ�����ǻ��麯���ӿ�
*
* ����:
* @blockID       ---Ҫ��ǻ���Ŀ��
*
* ����:��Ǵ�block�ǻ��飬Ҳ���ǰѴ�block�ĵ�һҳ��OOB�е�ǰ�����ֽ�����
*/
u32 nand_bad(u32 blockID)
{
    struct nand_param param = {0, 0, 0, 0, 0, 0, 0};
    u32 ret;
	/*����Ҫ��Ļ����*/
    param.number    =   blockID;
    ret = nand_run(&param, MARK_BAD);

    return ret;

}

/**
* ����:nandcģ���ṩ�������Flash�麯���ӿ�
*
* ����:
* @blockID       ---Ҫ������Flash��Ŀ��
*
* ����:����Flash�����ݿ�
*/

u32 nand_erase(u32 blockID)
{
    struct nand_param param = {0, 0, 0, 0, 0, 0, 0};
	/*����Ҫ�����Ŀ��*/
    param.number    =   blockID;

    return nand_run(&param, ERASE_BLOCK);

}

/**
 * ����:nandcģ���ṩ��ʹ��ECC���ܵ�дFlash���ݺ����ӿ�
 *
 * ����:
 * @flash_addr    ---Ҫд��flash��ַ
 * @src_data      ---Ҫд���������ڴ��еĵ�ַ,
 * @write_size    ---Ҫд�����ݵĴ�С
 * @skip_length   ---���˵ĳ���,����д�Ĺ�������������ʱ�����˿飬�˽ṹ��¼�����ĳ���
 *
 *
 * ����:��д�ӿ�������ʵ����Flash�Ĳ���,�����д�Ĺ����в���Ҫ�û���֤��дǰ�汣֤Flash�Ƿ��Ѿ�������,д��flash_ddr��write_size����
   û������,ע��˹���û��дOOB�Ĺ���
 */
u32 nand_write(FSZ flash_addr, u32 src_data, u32 write_size, u32* skip_length)
{
    struct nand_param param = {0, 0, 0, 0, 0, 0, 0};
	/*�������*/
    param.addr_flash    =   flash_addr;
    param.addr_data     =   src_data;
    param.size_data     =   write_size;
    param.skip_length   =   skip_length;
	/*param.size_oobΪ0��ʾ�ǲ���OOB��д����*/
    return nand_run(&param, WRITE_NAND);

}

/**
 * ����:nandcģ���ṩ��ʹ��ECC���ܵ�дFlash���ݺ�OOB���ݵĺ����ӿ�
 *
 * ����:
 * @flash_addr    ---Ҫд��flash��ַ
 * @src_data      ---Ҫд���������ڴ��еĵ�ַ,
 * @data_size     ---Ҫд�����ݵĴ�С,�˴�С������Ҫд�����ݺ�OOB֮��
 * @oob_per_page  ---дÿһҳ����Ҫд��OOB�ĳ���
 * @skip_length   ---���˵ĳ���,����д�Ĺ�������������ʱ�����˿飬�˽ṹ��¼�����ĳ���
 *
 *
 * ����:��д�ӿ�������ʵ����Flash�Ĳ���,�����д�Ĺ����в���Ҫ�û���֤��дǰ�汣֤Flash�Ƿ��Ѿ�������,д��flash_ddr��write_size����
   ������,data_size��(pagesize+sparesize)��������,ע��˹���������ĺ����������Ǵ�OOB��д����
   src_data�е����ݸ�ʽ��pagesize(flashdata)+sparesize(OOBdata)
 */
 u32 nand_write_oob(FSZ flash_addr, u32 src_data, u32 data_size, u32 oob_per_page, u32* skip_length)
{
    struct nand_param param = {0, 0, 0, 0, 0, 0, 0};
	/*�������*/
    param.addr_flash    =   flash_addr;
    param.addr_data     =   src_data;
    param.size_data     =   data_size;
	/*param.size_oob��Ϊ0��ʾ�Ǵ�OOB��д����*/
    param.size_oob      =   oob_per_page;
    param.skip_length   =   skip_length;

    return nand_run(&param, WRITE_NAND);

}

/**
 * ����:nandcģ���ṩ������дFlash���ݵĺ����ӿ�,��д�Ĺ����в�ʹ��ECC
 *
 * ����:
 * @dst_page      ---Ҫд��flashҳ��ַ
 * @offset_data   ---Ҫд��flashҳ��ƫ�Ƶ�ַ
 * @src_data      ---Ҫд��Flash�����ݵĵ�ַ
 * @len_data      ---Ҫд���ݵĴ�С
 * @skip_length   ---���˵ĳ���,���ڶ��Ĺ�������������ʱ����Ҫ���Ŀ飬�˽ṹ��¼�����ĳ���
 *
 *
 * ����:�˹��ܰ�������д��flash��ȥ,flash�����е����ݸ�ʽ����Ҫд�����ݸ�ʽ
 *һ���ʹ�������offset_dataΪ0��len_dataΪpagesize+sparesize�Ĵ�С��ע��offset_data+len_data���ܳ���pagesize+sparesize�Ĵ�С
 */
u32 nand_write_raw(u32 dst_page, u32 offset_data, u32 src_data, u32 len_data ,u32* skip_length)
{
    struct nand_param param = {0, 0, 0, 0, 0, 0, 0};
	/*�������*/
    param.number        = dst_page;
    param.addr_flash    = offset_data;
    param.addr_data     = src_data;
    param.size_data     = len_data;
    param.skip_length   =   skip_length;
	/*��ʹ��ECC���ܵ�д����*/
    return  nand_run(&param, WRITE_RAW);
}

/**
 * ����:nandcģ���ṩ����ɨ�軵��ĺ����ӿ�
 *
 * ����:
 * @start         ---flash��ʼ�ĵ�ַ
 * @length        ---ɨ��ĳ���
 * @if_erase      ---�������Ǻÿ���ݴ˲����������Ƿ�����˿�
 * ����:ɨ��flash�Ƿ��ǻ��飬���ݲ����������Ƿ����
 */
u32 nand_scan_bad(FSZ start, FSZ length, u32 if_erase)
{
	u32 addr_block_align;
	u32 length_block_align;
	u32 blk_id, is_bad, times, ret;
	struct nand_spec spec;
	/*��Ϊ����fastboot��ʵ�֣��ڴ���ͷ���û��ʵ�ֵģ�ʹ�þ�̬������ʾһ�η����Ժ󶼲�������*/
	static u32 block_buf = 0 ;

	memset(&spec, 0, sizeof(struct nand_spec));
	/*ͨ���˺����õ�nandflash�Ĺ�����,֪��blocksize�Ĵ�С*/
	if(NAND_OK != nand_get_spec(&spec))
	{
		NAND_TRACE(("nand_scan_bad: nand_get_spec failed\n"));
		return NAND_ERROR;
	}

	/*ɨ���ʱҪ�����*/
	addr_block_align = start - start%(spec.blocksize);
	/*�õ�Ҫɨ��Ŀ鳤��*/
	length_block_align = nandc_alige_size(start + length, spec.blocksize) - addr_block_align;
	/*���û�з����ڴ�ͷ����ڴ�*/
	if(!block_buf)
	{
		block_buf = (u32)himalloc(spec.blocksize);
		if(NULL == (void*)block_buf)//pclint 58
		{
			NAND_TRACE(("nand_scan_bad: himalloc failed\n"));
			goto ERRO;
		}
	}
	/*���п鳤����Ҫɨ��*/
	while(length_block_align > 0)
	{
		/*�õ���ŵ�ַ*/
		blk_id = addr_block_align/spec.blocksize;
		if(NAND_OK != nand_isbad(blk_id, &is_bad))
		{
			NAND_TRACE(("nand_scan_bad: nand_isbad failed\n"));
			goto ERRO;
		}
		if(NANDC_GOOD_BLOCK == is_bad)
		{
			times = 1;
			/*�������������û�г���Ļ���ʾ�Ǻû�*/
			while((NANDC_E_READ == nand_read((FSZ)addr_block_align, block_buf, spec.blocksize, 0)) && (times))
			{
			    times--;
			}
			if(0 == times)
			{
			    NAND_TRACE(("nand_scan_bad: find and read error, address:0x%x\n",addr_block_align));

			    ret = nand_bad(blk_id);
			    if(ret)
			    {
			        NAND_TRACE(("nand_scan_bad:nand check bad failed, ret = 0x%x\n",ret));
			    }
			}
			/*�Ǻÿ����Ҫ�����Ļ��Ͳ���*/
			else if(NANDC_TRUE == if_erase)
			{
			    ret = nand_erase(blk_id);
			    if(ret)
			    {
			        NAND_TRACE(("nand_scan_bad:nand erase failed, ret = 0x%x\n",ret));
			    }
			}
			else
			{
			}
		}
		else
		{
			NAND_TRACE(("nand_scan_bad:find bad block: 0x%x\n",addr_block_align));
		}
		length_block_align -= spec.blocksize;
		addr_block_align   += spec.blocksize;
	}

	return NAND_OK;
ERRO:
	return NAND_ERROR;
}


/**
* ����:nandcģ���ṩ�����ж�nand������ģ���Ƿ��ʼ���Ĺ���
*
* ����:
* ��
*
*
* ����:���nandcģ���Ѿ���ʼ���򷵻�1��û�г�ʼ���򷵻�0
*/
u32 nand_isinit(void)
{
    return (nand_handle.init_flag == NAND_INIT_MAGIC);
}

/**
* ����:nandcģ���ṩ�������nandc���������ݽṹ�ĺ���
*
* ����:
* ��
*
*
* ����:���nandc���������ݽṹ���ڴ�
*/
void nand_init_clear(void)
{
    nandc_nand_host = NULL;
	/*���ݽṹ����*/
    memset((void*)&nand_handle, 0x00 ,sizeof(nand_handle));
}


/*����ĺ�����Ҫ��Ϊ������ģ���ṩ�Ľӿ�*/


/**
* ����:��ȡ�������ƫ�Ƶ�ַ�ı�־ֵ����Ҫ��ΪNVģ���ã���ѯ�˷�����ƫ�Ƶ�ַ�ı�־λ��ֵ
*
* ����:
* @partition_name         	---������
* @partition_offset         ---���������ƫ��
* @flag                     ---�Ѽ��ķ����ı�־ֵ����ڴ�flag��
* ����:NVģ���һ��������һ��block�����һҳ��OOB�д���ض����ֵ�����˱��ֵ�����flag��
*/
u32 bsp_nand_read_flag_nv(const char *partition_name, u32 partition_offset, unsigned char *flag)
{
    u32 flash_addr;
    u32 ret = NANDC_ERROR;
    static unsigned char *buffer = NULL;
    struct nand_spec spec;
    struct ST_PART_TBL * ptable = find_partition_by_name((char *)partition_name);
	/*��������*/
    if(!ptable)
    {
        goto ERRO;
    }
    if(!flag)
    {
        NAND_TRACE(("argu error.\n"));
        goto ERRO;
    }
	/*�õ��˷�����nandflash�Ĺ�����*/
    ret = bsp_get_nand_info(&spec);
    if(ret)
    {
        goto ERRO;
    }
	/*û���ڴ�ʱ�����ڴ�*/
    if(!buffer)
    {
        buffer = (unsigned char *)himalloc(spec.pagesize + YAFFS_BYTES_PER_SPARE);
        if(!buffer)
        {
    		NAND_TRACE(("get ram buffer failed!\n"));
    		goto ERRO;
        }
    }

    memset(buffer, 0xFF, spec.pagesize + YAFFS_BYTES_PER_SPARE);

    flash_addr = ptable->offset + partition_offset;
	/*ʹ��ECC���ܵĴ�OOB�����ݹ���*/
    ret = nand_read_oob((flash_addr + spec.blocksize - spec.pagesize),
          (unsigned int)buffer, (spec.pagesize + YAFFS_BYTES_PER_SPARE),YAFFS_BYTES_PER_SPARE ,NULL);
    if(ret)
    {
		NAND_TRACE(("nand read oob failed!\n"));
		goto ERRO;
    }
    *flag = (*(buffer + spec.pagesize) == NV_WRITE_SUCCESS) ? NV_WRITE_SUCCESS : (~NV_WRITE_SUCCESS);

    return NANDC_OK;
ERRO:
    return ret;
}

/****************************************************************
��������: ��fastboot�׶λ�ȡ������С
�������: partition_name-��������
�������: none
���ز���: ����size
ע������: ��fastboot�׶ε���
****************************************************************/
unsigned int bsp_get_part_cap( const char *partition_name )
{
    unsigned int ret_size = 0;
    struct ST_PART_TBL * ptable = ( void* )( 0 );

    if ( ( void* )( 0 ) == partition_name )
    {
        cprintf( "fastboot: nv dload partition_name is NULL!\n" );
        goto ERRO;
    }

    ptable = find_partition_by_name(partition_name);
    if(!ptable)
    {
        cprintf( "fastboot: nv dload get ptable fail!\n" );
        goto ERRO;
    }

    ret_size = ptable->capacity;
    cprintf( "fastboot: nv dload cap is 0x%x.\n", ret_size );

ERRO:
    return ret_size;
}

/**
* ����:nandcģ�鰴��������ʹ��ECC���ܵĶ����ݲ���,ע��˺����Ķ��������ܶ�OOB����
*
* ����:
* @partition_name          		---Ҫ���ķ�����
* @partition_offset      	    ---Ҫ�����������ƫ�Ƶ�ַ
* @ptr_ram_addr      	        ---�����ڴ�ĵ�ַ
* @length      	        		---�������ݳ���
* @skip_len                     ---���˵ĳ���,���ڶ��Ĺ�������������ʱ����Ҫ���Ŀ飬�˽ṹ��¼�����ĳ���
*
*
* ����:���ݷ������ͷ�����ƫ�Ƶ�ַ��ȷ��Flash�ĵ�ַ,�Ѷ��������ݴ����ptr_ram_addr��ȥ
*/
int bsp_nand_read(const char *partition_name, u32 partition_offset, void* ptr_ram_addr, u32 length, u32 *skip_len)
{
    u32 flash_addr;
    u32 ret = NANDC_ERROR;
    struct ST_PART_TBL * ptable = find_partition_by_name(partition_name);

    if(!ptable)
    {
        goto ERRO;
    }
	/*ͨ����������ת��Flash�ĵ�ַ*/
    flash_addr = ptable->offset + partition_offset;
	/*ҪFlash�����ݵ��ڴ���*/
	return nand_read(flash_addr, (u32)ptr_ram_addr, length, skip_len);
ERRO:
    return ret;
}

/**
* ����:nandcģ�鰴�������ͷ���ƫ�Ƶ�ַ������flash����
*
* ����:
* @partition_name          		---Ҫ�����ķ�����
* @partition_offset      	    ---Ҫ���������ƫ�Ƶ�ַ
*
*
* ����:���ݷ������ͷ�����ƫ�Ƶ�ַ��ȷ��Flash�ĵ�ַ,��������һ��block,ע���ǲ���һ��block���ݿ�
*/
int bsp_nand_erase(const char *partition_name, u32 partition_offset)
{
    u32 flash_addr;
    u32 block_id, bad_flag;
    u32 ret = NANDC_ERROR;
	struct nandc_host  * host   = NULL;
    struct ST_PART_TBL * ptable = find_partition_by_name(partition_name);
	/*��������ȷ*/
    if(!ptable)
    {
        goto ERRO;
    }

    /*�õ�flash�ĵ�ַ��Ϣ*/
    flash_addr = ptable->offset + partition_offset;

    host = nandc_nand_host;
    if(!host)
    {
        NAND_TRACE(("ERROR: function %s, line %d\n", __FUNCTION__, __LINE__));
        goto ERRO;
    }

    /*�õ���Ų��ж��Ƿ��ǻ���*/
    block_id = flash_addr / host->nandchip->spec.blocksize;
    ret = nand_isbad(block_id, &bad_flag);
    if(ret)
    {
        NAND_TRACE(("ERROR: nand quary bad failed, function %s, line %d\n", __FUNCTION__, __LINE__));
        goto ERRO;
    }

    if(NANDC_BAD_BLOCK == bad_flag)
    {
        NAND_TRACE(("ERROR: try to erase a bad block, function %s, line %d\n", __FUNCTION__, __LINE__));
        goto ERRO;
    }
	/*����flash����*/
	return nand_erase(block_id);

ERRO:
    return ret;
}


/**
* ����:nandcģ�鰴�������ͷ������ƫ����ʹ��ECC���ܵ�д���ݲ���,ע��˺�����д������OOB��������д�Ĺ����л����nandflash
*
* ����:
* @partition_name          		---Ҫд���ݵķ�����
* @partition_offset      	    ---Ҫд���ݵķ������ƫ�Ƶ�ַ
* @ptr_ram_addr          		---Ҫд���ݵĵ�ַ
* @length      	    			---Ҫд���ݵĳ���
*
* ����:���ݷ������ͷ�����ƫ�Ƶ�ַ��ȷ��Flash�ĵ�ַ,����дFlash����
*/
s32 bsp_nand_write(const char *partition_name, u32 partition_offset, void* ptr_ram_addr, u32 length)
{
    u32 flash_addr, ret = NANDC_ERROR;
    struct ST_PART_TBL * ptable = find_partition_by_name(partition_name);

    if(!ptable)
    {
        goto ERRO;
    }

    /*�õ�ҪдFlash�ĵ�ַ*/
    flash_addr = ptable->offset + partition_offset;
	/*��д�Ĺ����л����nandflash,��˲���Ҫ��������������һ��*/
	return nand_write(flash_addr, (u32)ptr_ram_addr, length, NULL);

 ERRO:
    return ret;
}


/**
* ����:nandcģ�鰴�������ͷ������ƫ����ȷ���˿��ǲ��ǻ���
*
* ����:
* @partition_name          		---��ѯ���ݿ�ķ�����
* @partition_offset      	    ---��ѯ���ݿ�ķ������ƫ�Ƶ�ַ
*
* ����:���ݷ������ͷ�����ƫ�Ƶ�ַ��ȷ��Flash�ĵ�ַ,�����жϴ�block�ǲ��ǻ���
����ֵ:
* 0��ʾ�˿��Ǻÿ�
* 1��ʾ�ǻ���
* �����ı�ʾ�д���
*/
int bsp_nand_isbad(const char *partition_name, u32 partition_offset)
{
    u32 flash_addr;
    u32 bad_flag;
    u32 ret = NANDC_ERROR;
	struct nandc_host  * host   = NULL;
    struct ST_PART_TBL * ptable = find_partition_by_name(partition_name);

    if(!ptable)
    {
        goto ERRO;
    }

    /*�õ�Flash��ַ*/
    flash_addr = ptable->offset + partition_offset;

    host = nandc_nand_host;
    if(!host)
    {
        NAND_TRACE(("ERROR: function %s, line %d\n", __FUNCTION__, __LINE__));
        goto ERRO;
    }
	/*�жϴ�block�Ƿ��ǻ���*/
	ret = nand_isbad(flash_addr / host->nandchip->spec.blocksize, &bad_flag);
    if(ret == 1)
    {
        return -6;
    }
    else if(ret > 1)
    {
        return ret;
    }

    if(bad_flag == 1)  /* bad block */
    {
        return 1;
    }
    else               /* good block */
    {
        return 0;
    }

ERRO:
    return ret;
}

/**
* ����:���ڴ��еķ�������Ϣ���µ�nandc_nand_host�е����ݽṹ��ȥ
*
* ����:
* ��
*
* ����:���ڴ��еķ�������Ϣ���µ�nandc_nand_host�����ݽṹ��ȥ����fastboot������ʹ��
*/
void bsp_update_ptable_to_nandc(void)
{
    nandc_init_mtd_partition(nandc_nand_host);
}


/**
 * ����:�õ�nandflash��ع������ĺ����ӿ�
 *
 * ����:
 * @spec      ---��nandflash�е���ع�������д�ڴ�ָ�������ȥ
 *
 *
 * ����:�õ�nandflash�Ĺ�����
 */
int bsp_get_nand_info(struct nand_spec *spec)
{
	struct nandc_host 	*host 		= nandc_nand_host;
	/*�������Ƿ���ȷ*/
	if(!host)
	{
	    NAND_TRACE(("ERROR: get nand info error.\n"));
	    goto erro;
	}

	if(!nand_isinit())
	{
	    NAND_TRACE(("ERROR: nandc driver not init yet!\n"));
	    goto erro;
	}
	/*����nandflash����ع�����*/
	spec->pagesize 	 		= host->nandchip->spec.pagesize;
	spec->pagenumperblock	= (host->nandchip->spec.blocksize / spec->pagesize);
	spec->addrnum      		= host->nandchip->spec.addrcycle;
	spec->ecctype 	 		= host->nandchip->ecctype;
	spec->buswidth   		= host->nandchip->spec.buswidth;
	spec->blocksize         = host->nandchip->spec.blocksize;
	spec->sparesize         = host->nandchip->spec.sparesize;
	spec->chipsize          = host->nandchip->spec.chipsize;

	return NANDC_OK;
erro:
	return NANDC_ERROR;
}
/**
 * ����:��ӡnandflash��ع������ĺ����ӿ�
 *
 * �������:��
 *
 *
 * ����:��ӡnand->spec ����ֵ
 */
void bsp_show_nand_info(void)
{
    struct nand_spec spec;
    if(NAND_ERROR == bsp_get_nand_info(&spec))
    {
        NAND_TRACE(("ERROR: nand get spec error.\n"));
        return;
    }
    NAND_TRACE((" ------begin show nand info----------\n"));
    NAND_TRACE((" chipsize=%dMB,blocksize=%dKB,pagesize=%dKB,sparesize=%d\n",
    	                   (spec.chipsize/1024/1024),spec.blocksize/1024,spec.pagesize/1024,spec.sparesize));
    NAND_TRACE((" pagenumperblock=%d,ecctype=%d,buswidth=%d,addrnum=%d\n",
    	                   spec.pagenumperblock,spec.ecctype,(0==spec.buswidth?8:16),spec.addrnum));
    NAND_TRACE((" ------end  show nand info----------\n"));
}

/******************************************************************************************
 * FUNC NAME:
 * @bsp_update_size_of_lastpart() - external API:
 *
 * PARAMETER:
 * @new_ptable - addr of new ptable
 *
 * RETURN:
 *    null
 *
 * DESCRIPTION:
 *    update the size of last partition to shared memory
 *    ���������һ��������flash���һ��ռ���ȫռ��,�Ӷ����¾�̬�����ptable.bin�����ʵ��ʹ�õ�ptable��һ����
 *    �����������ʱ�����Ϊ����������һ�����Ӷ����¶�yaffs�����Ĳ������˴���ʵ��ʹ�õķ��������һ��������С
 *    ������ptable��������С
 *
 * CALL FUNC:
 *
 *****************************************************************************************/
void bsp_update_size_of_lastpart(struct ST_PART_TBL *new_ptable)
{
    struct ST_PART_TBL * old_ptable = NULL;
    if(!new_ptable)
    {
        NAND_TRACE(("[%s]argu error\n", __FUNCTION__));
        return;
    }

    /*���ݷ������õ��������ݽṹ*/
    while(0 != strcmp(PTABLE_END_STR, new_ptable->name))
    {
        new_ptable++;
    }

    new_ptable--;

    old_ptable = find_partition_by_name(new_ptable->name);
    if(old_ptable)
    {
        new_ptable->capacity = old_ptable->capacity;
    }

}

/**
 * ����:�������е�yaffs�ļ�ϵͳ����
 *
 * ����:
 * ��
 *
 *
 * ����:�������е�yaffs�ļ�ϵͳ����,�������Ĺ�����ʹ��,�������е�yaffs�ļ�ϵͳ�ķ���
 */
void bsp_erase_yaffs_partitons(void)
{
    struct ST_PART_TBL * ptable = NULL;
    struct ptentry ptn;
    int ret = NANDC_ERROR;

    ptable_ensure();

    ptable = ptable_get_ram_data();
	/*�������еķ�����Ϣ*/
    while(0 != strcmp(PTABLE_END_STR, ptable->name))
    {
    	/*����YAFFS�ļ�ϵͳ�ķ���*/
        if(ptable->property & DATA_YAFFS)
        {
            /* coverity[buffer_size_warning] */
            strncpy(ptn.name, ptable->name, 16);
            ptn.start = ptable->offset;
            ptn.length = ptable->capacity;
            NAND_TRACE(("earsing %s, start 0x%x, length 0x%x\n", ptn.name, ptn.start, ptn.length));
            ret = flash_erase(&ptn);
            if(ret)
            {
                NAND_TRACE(("[%s] ERROR: erase %s failed, ret = %d\n", __FUNCTION__, ptn.name, ret));
                return;
            }
        }
        ptable++;
    }
}


/**
 * ����:���ַ���ת��Ϊ����
 *
 * ����:
 * @str          ---Ҫת�����ַ���
 * @digit        ---�õ�������ֵ
 *
 * ����:���ַ���ת��Ϊ����
 */
unsigned int str2ul(char *str, unsigned int *digit)
{
    int ret = NANDC_ERROR;
    unsigned int tmp_digit = 0;
    char *temp = str;

    /*��������ȷ*/
    if(!str || !digit)
    {
        NAND_TRACE(("[%s]ERROR: input is NULL.\n", __FUNCTION__));
        goto ERRO;
    }

    /*��stringת��Ϊ����*/
    while(*temp != 0)
    {
        if(*temp < '0' || *temp > '9')
        {
            NAND_TRACE(("[%s]ERROR: string input is illegal.\n", __FUNCTION__));
            goto ERRO;
        }
        tmp_digit = tmp_digit * 10 + (*temp - '0');
        temp++;
    }

    *digit = tmp_digit;

    return NANDC_OK;

ERRO:
    return ret;
}

/**
 * ����:�������
 *
 * ����:
 * @argu          ---�ַ�������
 *
 *
 * ����:������
 */
char *bsp_nand_argument_check(char *argu)
{
    char *temp;

    /* ignore while space at the start position*/
    while(*argu == ' ')
    {
        argu++;
    }

    /* if partition name is NULL, error. ps: 0x0D is '\n' */
    if(*argu == 0x0D)
    {
        return NULL;
    }

    /* must end with 0 */
    for(temp = argu; (*temp != ' ') && (*temp != 0x0D); temp++)
    {
        /* NOP */
    }
    *temp = 0;
    return argu;

}

/**
 * ����:��nand�껵��
 *
 * ����:
 * @blockid          ---block����ַ���
 *
 *
 * ����:nand�껵��
 */
void bsp_nand_markbad(char *blockid)
{
    int ret = NANDC_ERROR;
    unsigned int id, bad_flag;
    char *blk_id = bsp_nand_argument_check(blockid);

    /* string to int  */
    ret= str2ul(blk_id, &id);
    if(ret)
    {
        NAND_TRACE(("ERROR: string to integer failed, ret = 0x%x.\n", ret));
        goto EXIT;
    }

    /* check block bad. if good, mark bad; else return */
    ret = nand_isbad(id, &bad_flag);
    if(ret)
    {
        NAND_TRACE(("[%s]ERROR: nand check bad failed, ret = 0x%x.\n", __FUNCTION__, ret));
        goto EXIT;
    }

    if(NANDC_GOOD_BLOCK == bad_flag)
    {
        ret = nand_bad(id);
        if(ret)
        {
            NAND_TRACE(("[%s]ERROR: nand mark bad failed, ret = 0x%x.\n", __FUNCTION__, ret));
            goto EXIT;
        }
    }
    else
    {
        NAND_TRACE(("[%s]WARNING: block 0x%x is already bad.\n", __FUNCTION__));
        goto EXIT;
    }

    return;

EXIT:
    return;
}

/**
 * ����:��nand��ǿ�Ʋ���
 *
 * ����:
 * @blockid          ---block����ַ���
 *
 *
 * ����:nandǿ�Ʋ�������,��������������ʱҲ�����ô˺�����ǿ�Ʋ���
 */
void bsp_nand_erase_force(char *blockid)
{
    int ret = NANDC_ERROR;
    unsigned int id;

    char *blk_id = bsp_nand_argument_check(blockid);

    /* string to int  */
    ret= str2ul(blk_id, &id);
    if(ret)
    {
        NAND_TRACE(("[%s]ERROR: string to integer failed, ret = 0x%x.\n", __FUNCTION__, ret));
        goto ERRO;
    }

    /*ֱ�Ӳ���*/
    ret = nand_erase(id);
    if(ret)
    {
        NAND_TRACE(("[%s]ERROR: nand check bad failed, ret = 0x%x.\n", __FUNCTION__, ret));
        goto ERRO;
    }
    else
    {
        NAND_TRACE(("SUCCESS: erase block %x forcely.\n", id));
    }

    return;

ERRO:
    return;
}

#endif

#ifdef __cplusplus
}
#endif

