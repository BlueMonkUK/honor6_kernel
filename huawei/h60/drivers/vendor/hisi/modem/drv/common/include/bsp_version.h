#ifndef __BSP_VERSION_H__
#define __BSP_VERSION_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <bsp_shared_ddr.h>
#include "drv_version.h"

#include "bsp_om.h"

#define CFG_PRODUCT_TAG_ID "193c25990102eeb27f5ed9243d9a01e144e3efc4"
#define DLOAD_VERSION               "2.0"                   /* ����Э��汾 */
#define VERSION_MAX_LEN 32

#define HW_VERSION_ADDR SHM_MEM_HW_VER_ADDR

#ifndef VER_ERROR
#define VER_ERROR (-1)
#endif

#ifndef VER_OK
#define VER_OK 0
#endif

/*log*/
#define  ver_print_error(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_VERSION, "[version]: <%s> "fmt, __FUNCTION__, ##__VA_ARGS__))
#define  ver_print_info(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_INFO, BSP_MODU_VERSION, "[version]: <%s> "fmt, __FUNCTION__, ##__VA_ARGS__))

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a)/(sizeof((a)[0])))
#endif

struct dload_info_type{
	char software_ver[VERSION_MAX_LEN];
	char product_name[VERSION_MAX_LEN];
	char iso_ver[VERSION_MAX_LEN];
};

/*****************************************************************************
* �� �� ��  	: bsp_get_hardware_version
*
* ��������  	: ��ȡӲ���汾��
*
* �������  	: ��
* �������  	: Ӳ���汾���ַ���ָ��
*
* �� �� ֵ  	: 0��ȡ�ɹ�
			-1��ȡʧ��
*
* �޸ļ�¼  :
*
*****************************************************************************/
char * bsp_version_get_hardware(void);

/*****************************************************************************
* �� �� ��  : bsp_get_product_inner_name
*
* ��������  : ��ȡ�ڲ���Ʒ����
*
* �������  : ��
* �������  :
*
 �� �� ֵ  : �ڲ���Ʒ�����ַ���ָ��
* �޸ļ�¼  :
*
*****************************************************************************/
char * bsp_version_get_product_inner_name(void);

/*****************************************************************************
* �� �� ��  : bsp_get_product_out_name
*
* ��������  : ��ȡ�ڲ���Ʒ����
*
* �������  : ��
* �������  :
*
 �� �� ֵ  : �ⲿ��Ʒ�����ַ���ָ��
* �޸ļ�¼  :
*
*****************************************************************************/
char * bsp_version_get_product_out_name(void);

/*****************************************************************************
* �� �� ��  : bsp_get_pcb_version
*
* ��������  : ��ȡPCB
*
* �������  : ��
* �������  :
*
 �� �� ֵ  : PCB�ַ���ָ��
* �޸ļ�¼  :
*
*****************************************************************************/
char * bsp_version_get_pcb(void);

/*****************************************************************************
* �� �� ��  : bsp_get_dloadid_version
*
* ��������  : ��ȡdloadid
*
* �������  : ��
* �������  :
*
 �� �� ֵ  : dloadid�ַ���ָ��
* �޸ļ�¼  :
*
*****************************************************************************/
char * bsp_version_get_dloadid(void);

/*****************************************************************************
* �� �� ��  : bsp_get_build_date_time
*
* ��������  : ��ȡ�������ں�ʱ��
*
* �������  : ��
* �������  :
*
 �� �� ֵ  : ����汾���ַ���ָ��
* �޸ļ�¼  :
*
*****************************************************************************/
char * bsp_version_get_build_date_time(void);

/*****************************************************************************
* �� �� ��  : bsp_get_chip_version
*
* ��������  : ��ȡоƬ�汾��
*
* �������  : ��
* �������  :
*
 �� �� ֵ  :оƬ�汾���ַ�����ָ��
* �޸ļ�¼  :
*
*****************************************************************************/
char * bsp_version_get_chip(void);

/*****************************************************************************
* �� �� ��  : bsp_get_firmware_version
*
* ��������  : ��ȡ����汾��
*
* �������  : ��
* �������  :
*
 �� �� ֵ  : ����汾���ַ���ָ��
* �޸ļ�¼  :
*
*****************************************************************************/
char * bsp_version_get_firmware(void);

/*****************************************************************************
* �� �� ��  : bsp_get_version_hash
*
* ��������  : ��ȡ������Ŀ��hashֵ
*
* �������  : ��
* �������  :
*
 �� �� ֵ  : ������Ŀ�hashֵ�ַ�����ָ��
* �޸ļ�¼  :
*
*****************************************************************************/
char * bsp_version_get_hash(void);

/*****************************************************************************
* �� �� ��  : bsp_get_iso_version
*
* ��������  : ��ȡiso_version
*
* �������  : ��
* �������  :
*
 �� �� ֵ  : iso_version�ַ�����ָ��
* �޸ļ�¼  :
*
*****************************************************************************/
char * bsp_version_get_iso(void);

/*****************************************************************************
* �� �� ��  : bsp_get_webui_version
*
* ��������  : ��ȡwebui_version
*
* �������  : ��
* �������  :
*
 �� �� ֵ  : webui_version�ַ�����ָ��
* �޸ļ�¼  :
*
*****************************************************************************/
char * bsp_version_get_webui(void);

/*****************************************************************************
* �� �� ��  : bsp_get_release_version
*
* ��������  : ��ȡrelease_version
*
* �������  : ��
* �������  :
*
 �� �� ֵ  : release_version�ַ�����ָ��
* �޸ļ�¼  :
*
*****************************************************************************/
char * bsp_version_get_release(void);

int bsp_version_get_hwversion_index(void);

/*****************************************************************************
* ����	: bsp_get_board_chip_type
* ����	: get board type and chip type
* ����	: void
* ���	: void
* ����	: BOARD_TYPE_E
*****************************************************************************/
u32 bsp_version_get_board_chip_type(void);

int bsp_dload_get_dload_info(struct dload_info_type* dload_info);

bool bsp_board_type_is_gutl(void);


int bsp_version_debug(void);


void bsp_version_init(void);


int bsp_productinfo_init(void);

#ifdef __cplusplus
}
#endif

#endif

