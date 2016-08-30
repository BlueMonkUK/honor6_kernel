#ifndef VERSION_H
#define VERSION_H

#include <balongv7r2/types.h>
#include "drv_version.h"
/*****************************************************************************
* �� �� ��  	: bsp_save_hw_version
*
* ��������  : ��AXI MEMORY�д洢hw_id
*
* �������  : ��
* �������  :
*
* �� �� ֵ  	:
				0:�ɹ�
				-1:ʧ��
* �޸ļ�¼  :
*
*****************************************************************************/
int bsp_save_hw_version(void);
/*****************************************************************************
* ����	: bsp_board_type_is_gutl
* ����	: get board type is GUTL
* ����	: void
* ���	: void
* ����	: BOARD_TYPE_E
*****************************************************************************/
bool bsp_board_type_is_gutl(void);

/*****************************************************************************
* ����	: bsp_get_board_chip_type
* ����	: get board type and chip type
* ����	: void
* ���	: void
* ����	: BOARD_TYPE_E
*****************************************************************************/
u32 bsp_version_get_board_chip_type(void);

#endif
