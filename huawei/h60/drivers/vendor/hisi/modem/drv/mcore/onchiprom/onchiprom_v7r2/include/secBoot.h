/*************************************************************************
*   ��Ȩ����(C) 1987-2004, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  secBoot.h
*
*   ��    �� :  wuzechun
*
*   ��    �� :  secBoot.c��ͷ�ļ��������붨�弰��������
*
*   �޸ļ�¼ :  2011��6��9��  v1.00  wuzechun  ����
*
*************************************************************************/


#ifndef _SECBOOT_H                                                                  
#define _SECBOOT_H

#include "OcrShare.h"

/*����ӳ�����ͣ�0x01(RAM��ʼ������)��0x02(��ȫ��������ӳ��)��03(USB�Ծ�����ӳ��) */
typedef enum _tagIMAGE_TYPE_E
{
    IMAGE_TYPE_RAM_INIT = 0x01,
    IMAGE_TYPE_DOWNLOAD,
    IMAGE_TYPE_BOOTLOAD
}IMAGE_TYPE_E;

int nandReadBl( UINT32 dest );
void go(FUNCPTR entry);
int idioIdentify(UINT32 dataAddr, UINT32 dataLen, KEY_STRUCT *pubKey, UINT32* pIdio);
int secCheck(UINT32 ulImageAddr, IMAGE_TYPE_E eImageTye);

extern void BSP_UBOOT_StartExec(void);
 
#endif

