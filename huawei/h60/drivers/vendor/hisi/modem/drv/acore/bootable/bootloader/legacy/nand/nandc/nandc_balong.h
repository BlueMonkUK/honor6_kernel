/*******************************************************************************
* Copyright (C), 2008-2011, HUAWEI Tech. Co., Ltd.
*
* Module name: Hisilicon Nand Flash Controller Dirver
*
* Description: nandcģ���ṩ������Ľӿ�
*
* Filename:    nandc_balong.h
* Description: nandcģ���ṩ����Ľӿں���
*
*******************************************************************************/
/******************************************************************************
*    Copyright (c) 2009-2011 by  Hisilicon Tech. Co., Ltd.
*    All rights reserved.
*
*
******************************************************************************/
#ifndef _NANDC_BALONG_H_
#define _NANDC_BALONG_H_

#ifdef __cplusplus
extern "C"
{
#endif
#ifdef __FASTBOOT__
#include "nandc_inc.h"
#include <bsp_nandc.h>
/*enum of nand operation type */
enum NAND_FUNC
{
    READ_NAND,              /*ʹ��ECC���ܵĶ���ҳ����*/
    READ_YAFFS,
    READ_PAGE,				/*ʹ��ECC���ܵĶ�һҳ����*/
    READ_RAW,               /*��ʹ��ECC���ܵ����Flash���ݹ���*/
    WRITE_NAND,             /*ʹ��ECC���ܵ�д��ҳ����*/
    WRITE_YAFFS,
    WRITE_PAGE, 			/*ʹ��ECC���ܵ�дһҳ����*/
    WRITE_RAW,              /*��ʹ��ECC���ܵ���д����*/
    ERASE_BLOCK,            /*����һ��flash*/
    QUARY_BAD,              /*��ѯ�˿�block�Ƿ��ǻ���*/
    MARK_BAD                /*�껵�鹦��*/
};

/*����nandcģ��Ĳ������ݽṹ*/
struct nand_param
{
    u32 number;             /*indicate a number ,may be a flash block number or a flash page number*/
    FSZ addr_flash;         /*flash address to be read from or be writen to */
    u32 addr_data;          /*the ram address contain data that should be writen to flash, or the ram address
                              should store data read from flash */
    u32 addr_oob;           /*the ram address contain oob that should be writen to flash, or the ram address
                              should store oob read from flash */
    u32 size_data;          /*the size of data to be read or writen*/
    u32 size_oob;           /*the size of oob to be read or writen*/
    u32 *skip_length;       /*flash skip length during nand operation*/
};

/*nandcģ��������ݽṹ,������Ƶ������Ƕ����ṩͳһ�Ľӿ�,�����ʵ��ͨ��funcs�еĺ���ָ����ʵ�֣��������funcs��ʵ��
�����仯ʱ�����������ṩ�Ľӿڲ���*/
struct nand_bus
{
    struct nand_interface * funcs;          /*����ָ������ݽṹ*/
    u32 init_flag;                          /*ģ���ʼ����־,��Ҫ��Ϊ���ڲ��Գ����в鿴�Ƿ��Ѿ���ʼ�����*/
};
/*nand�����ľ���ʵ��,�ú���ָ��ĺô����ǵ��������ʵ�ַ����˱仯ʱ���������������ṩ�Ľӿں����������仯*/
struct nand_interface
{
    u32 (*init)(void);
    u32 (*read)(FSZ flashaddr, u32 dstaddr, u32 readsize, u32 oobsize, u32 *skipblks);
    u32 (*get_spec)(struct nand_spec *specs);
	/*�˺�����һҳpage�Ķ�д����*/
    u32 (*read_page_ecc)(FSZ address, u8* databuff, u8* oobbuff, u32 oobsize);
	/*�������,���Ȳ��ܳ���pagesize+sparesize*/
    u32 (*read_page_raw)(u32 pagenum, u32 offset, u32 dst_addr, u32 readsize);

    u32 (*quary_bad)(u32 blkid, u32 * isbad );

    u32 (*write)(FSZ flashaddr, u32 srcaddr, u32 writesize, u32 oobsize, u32 *skiplength);

    u32 (*write_page_ecc)(FSZ address, u8* databuff, u8* oobbuff, u32 oobsize);
    u32 (*write_page_raw)(u32 pagenum, u32 offset, u32 src_addr, u32 writesize);

    u32 (*mark_bad)(u32 blk_id);
    u32 (*erase_by_id)(u32 blk_id);
};


#define NAND_INIT_MAGIC 0x19840815
#define NAND_OK             0
#define NAND_ERROR        1


#if (NANDC_COMPILE_FLAG & NANDC_DEBUG)
#define NAND_TRACE(p)  hiout(p)
#else
#define NAND_TRACE(p)
#endif

#define NAND_IPC_OVERTIME_MS    0xFFFFFFFF


extern struct nandc_host*  nandc_nand_host;

 /**
 * ����:nandcģ��ĳ�ʼ��������Ӳ���Ĵ����ĳ�ʼ����������ݽṹ�ĳ�ʼ�����ṩ��fastboot�׶γ�ʼ��nandc������
 *
 * ����: none
 *
 * ����:�ṩ�����ʼ���������Ľӿ�
 *
 */
u32 nand_init(void);


 /**
 * ����:nandcģ���ṩ�����Flash���ݵĺ����ӿ�
 *
 * ����:
 * @flash_addr    ---Ҫ����flash��ַ
 * @dst_data      ---���ݶ����ڴ�ĵ�ַ
 * @read_size     ---Ҫ�����ݵĴ�С
 * @skip_length   ---���˵ĳ���,���ڶ��Ĺ�������������ʱ����Ҫ���Ŀ飬�˽ṹ��¼�����ĳ���
 *
 *
 * ����:��Flash������,�������ݳ��ȿ��Գ�����һҳ���˽ӿ�ֻ��Flash�е����ݣ�û�ж�OOB�е�����
 *
 */
u32 nand_read(FSZ flash_addr, u32 dst_data, u32 read_size, u32* skip_length);


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
u32 nand_read_oob(FSZ flash_addr, u32 dst_data, u32 data_size, u32 oob_per_page, u32* skip_length);


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
u32 nand_read_raw(u32 src_page, u32 offset_data, u32 dst_data, u32 len_data, u32* skip_length);


/**
 * ����:nandcģ���ṩ�������Flash��ĺ����ӿ�
 *
 * ����:
 * @blockID       ---Ҫ�����Ŀ��
 *
 *
 * ����:��Ҫ������Flash��ַת��Ϊ��ž���
 */
u32 nand_erase(u32 blockID);

/**
 * ����:nandcģ���ṩ�����жϴ�block�Ƿ��ǻ��麯���ӿ�
 *
 * ����:
 * @blockID       ---Ҫ���Ŀ��
 * @flag          ---�Ѽ��ı�־ֵ����ڴ˱�����ȥ,0Ϊ�û���1Ϊ����
 *
 * ����:����block�Ƿ��ǻ��飬���ѱ��ֵ�����flag��־��
 */
u32 nand_isbad(u32 blockID, u32 *flag);


/**
 * ����:nandcģ���ṩ�����ǻ��麯���ӿ�
 *
 * ����:
 * @blockID       ---Ҫ��ǻ���Ŀ��
 *
 * ����:��Ǵ�block�ǻ��飬Ҳ���ǰѴ�block�ĵ�һҳ��OOB�е�ǰ�����ֽ�����
 */
u32 nand_bad(u32 blockID);


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
u32 nand_write(FSZ flash_addr, u32 src_data, u32 write_size, u32* skip_length);


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
u32 nand_write_oob(FSZ flash_addr, u32 src_data, u32 data_size, u32 oob_per_page, u32* skip_length);


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
u32 nand_write_raw(u32 dst_page, u32 offset_data, u32 src_data, u32 len_data, u32* skip_length);


/**
 * ����:nandcģ���ṩ����õ�nandflash��ع������ĺ����ӿ�
 *
 * ����:
 * @spec      ---��nandflash�е���ز�����д�ڴ�ָ�������ȥ
 *
 *
 * ����:�õ�nandflash�Ĺ�����
 */
u32 nand_get_spec(struct nand_spec *spec);

/**
 * ����:��ӡnandflash��ع������ĺ����ӿ�
 *
 * �������:��
 *
 *
 * ����:��ӡnand->spec ����ֵ
 */
void bsp_show_nand_info(void);
/**
 * ����:nandcģ���ṩ�����ж�nand������ģ���Ƿ��ʼ���Ĺ���
 *
 * ����:
 * ��
 *
 *
 * ����:���nandcģ���Ѿ���ʼ���򷵻�1��û�г�ʼ���򷵻�0
 */
u32 nand_isinit(void);



/**
 * ����:nandcģ���ṩ�������nandc���������ݽṹ�ĺ���
 *
 * ����:
 * ��
 *
 *
 * ����:���nandc���������ݽṹ���ڴ�
 */
void nand_init_clear(void);

/**
 * ����:nandcģ���ṩ����ɨ�軵��ĺ����ӿ�
 *
 * ����:
 * @start         ---flash��ʼ�ĵ�ַ
 * @length        ---ɨ��ĳ���
 * @if_erase      ---�������Ǻÿ���ݴ˲����������Ƿ�����˿�
 * ����:ɨ��flash�Ƿ��ǻ��飬���ݲ����������Ƿ����
 */
u32 nand_scan_bad(FSZ start, FSZ length, u32 if_erase);

#endif
#ifdef __cplusplus
}
#endif

#endif



