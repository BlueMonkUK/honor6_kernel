/*************************************************************************
*   ��Ȩ����(C) 1987-2020, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  efuse.h
*
*   ��    �� :  wuzechun
*
*   ��    �� :  efuse.c ��ͷ�ļ�
*
*   �޸ļ�¼ :  2011��6��9��  v1.00  wuzechun  ����
*
*************************************************************************/

#ifndef __EFUSE_BALONG_H__
#define __EFUSE_BALONG_H__



#ifdef __cplusplus
extern "C" {
#endif
#include "osl_types.h"
#include "bsp_efuse.h"

int efuse_init(void);

/*****************************************************************************
* �� �� ��  : efuseReadHUK
*
* ��������  : ��ȡefuse huk����
*
* �������  : pBuf   ��ȡ����EFUSE�е�����
*             num    ���鳤��
* �������  : 
*
* �� �� ֵ  : OK �ɹ�
*             ERROR ʧ��
*
* ����˵��  : 
*
*****************************************************************************/
int efuseReadHUK(u32 *pBuf,u32 len);

#ifdef __cplusplus
}
#endif

#endif /* efuse_balong.h */
