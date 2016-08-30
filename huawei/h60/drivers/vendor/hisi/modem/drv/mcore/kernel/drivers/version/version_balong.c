#include "product_config.h"
#include "osl_common.h"
#include "bsp_memmap.h"
#include "bsp_version.h"
/*****************************************************************************
* ����	: bsp_get_board_mode_type
* ����	: get board type(GUL or LTE)
* ����	: void
* ���	: void
* ����	: BOARD_TYPE_E
*****************************************************************************/
BOARD_TYPE_E bsp_get_board_mode_type(void)
{
#if(defined(FEATURE_MULTIMODE_GUL))
	return BOARD_TYPE_GUL;
#else
	return BOARD_TYPE_LTE_ONLY;
#endif
}

/*****************************************************************************
* ����	: bsp_board_type_is_gutl
* ����	: get board type is GUTL
* ����	: void
* ���	: void
* ����	: BOARD_TYPE_E
*****************************************************************************/
bool bsp_board_type_is_gutl(void)
{
	return (bool)(BOARD_TYPE_GUL == bsp_get_board_mode_type() ? 1 : 0);
}

/*****************************************************************************
* ����	: bsp_get_board_chip_type
* ����	: get board type and chip type
* ����	: void
* ���	: void
* ����	: BOARD_TYPE_E
*****************************************************************************/
u32 bsp_version_get_board_chip_type(void)
{
	u32 type;
 
	type = *(u32 *)HW_VERSION_ADDR;

	if(HW_VER_INVALID==type){
		ver_print_error("[bsp_get_board_chip_type]:get hw version failed!hw_ver:0x%x\n",type);
		return type;
	}

	/*��Ϊudp��ֻ����UDPӲ���汾�ţ�������Ƶ�۰���Ϣ*/
	if(HW_VER_UDP_MASK==(type & HW_VER_UDP_MASK)){
		return HW_VER_PRODUCT_UDP;
	}
	if(HW_VER_V711_UDP_MASK == (type & HW_VER_V711_UDP_MASK)){
		return HW_VER_V711_UDP;
	}
	return type;
}

