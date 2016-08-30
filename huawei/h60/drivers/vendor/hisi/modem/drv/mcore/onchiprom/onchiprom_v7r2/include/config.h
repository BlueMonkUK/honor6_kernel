/*************************************************************************
*   ��Ȩ����(C) 1987-2020, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  config.h
*
*   ��    �� :  wuzechun
*
*   ��    �� :  TCM�ռ仮��
*
*   �޸ļ�¼ :  2011��6��9��  v1.00  wuzechun  ����
*
*************************************************************************/

#ifndef __CONFIG_H__
#define __CONFIG_H__

/***********************************************************************
 | V7R2 OnChipRom|
***********************************************************************/

#define ONCHIPROM_VER       "OnChipRom Ver 1.0"

#define M3_TCM_ADDR                    (0x10000000)	/*TCM��ʼ��ַ*/
#define M3_TCM_SIZE                    (64*1024)    /* 64K */

#define SRAM_ADDR                      (0x4FE00000)     /* AXI Memory��ʼ��ַ */
#define SRAM_SIZE                      (128*1024)       /* AXI Memory��С */

#define BOOTLOAD_SIZE_MAX       (60*1024)      /* BootLoader��󳤶� */

/* BootLoader��RAM��ʼ��������TCM�ϱ���λ�� */
#define M3_TCM_BL_ADDR     M3_TCM_ADDR
#define BOOT_RST_ADDR_OFFEST 0x04       /*�洢boot������ַ�ĵ�ַƫ��*/
#define BOOT_RST_ADDR_PP     (M3_TCM_ADDR + BOOT_RST_ADDR_OFFEST)  /*thumbָ�ѰַҪ���ַ����Ϊ����*/


/* ��������������TCM����,BootLoader��������Ҫ������ */
#define M3_TCM_SHARE_DATA_SIZE sizeof(tOcrShareData)
#define M3_TCM_SHARE_DATA_ADDR (SRAM_ADDR + SRAM_SIZE - M3_TCM_SHARE_DATA_SIZE)

/* OnChipRom���й���־���Ծٱ�־����tOcrShareData�ṹ���� */
#define OCR_INITED_FLAG_VALUE	0x23456789

#define AUTO_ENUM_FLAG_VALUE   0x82822828

#endif /* config.h */

