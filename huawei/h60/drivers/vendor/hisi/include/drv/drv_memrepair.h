/*************************************************************************
*   ��Ȩ����(C) 1987-2011, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  DrvInterface.h
*
*   ��    �� :  yangzhi
*
*   ��    �� :  ���ļ�����Ϊ"DrvInterface.h", ����V7R1�����Э��ջ֮���API�ӿ�ͳ��
*
*   �޸ļ�¼ :  2011��1��18��  v1.00  yangzhi����
*************************************************************************/

#ifndef __DRV_MEM_REPAIR_H__
#define __DRV_MEM_REPAIR_H__

#include "drv_comm.h"

extern int bsp_get_memrepair_time(void);

#define DRV_GET_MEM_REPAIR_TIME() bsp_get_memrepair_time()

#endif

