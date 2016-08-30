/*************************************************************************
*   ��Ȩ����(C) 1987-2020, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  sec_balong.h
*
*   ��    �� :  wangxiandong
*
*   ��    �� :  sec_balong.c ��ͷ�ļ�
*
*   �޸ļ�¼ :  
*
*************************************************************************/
#ifndef _INTER_SEC_BALONG_H_
#define _INTER_SEC_BALONG_H_

#include <osl_types.h>


/* EFUSE�ռ仮��*/
#define EFUSE_GRP_ROOT_CA       0x00
/* ��ȫУ��ʹ�ܱ�־ */
#define EFUSE_GRP_SECURITY      15
#define EFUSEC_SEC_EN_POS       4
#define EFUSEC_SEC_EN_WIDTH     1
#define EFUSEC_SEC_EN_MASK      (((1<<EFUSEC_SEC_EN_WIDTH)-1)<<EFUSEC_SEC_EN_POS)
#define EFUSEC_SEC_EN           (1<<EFUSEC_SEC_EN_POS)


#define UNSECBOOT_FLAG          (0x00)
#define SECBOOT_FLAG            (0x01)

#define P531_M3_LEN_INDEX       (45 * 4)
#define P531_ROOT_CA_INDEX      (P531_M3_LEN_INDEX + 4)


/*extern int efuse_read(u32 group, u32* pBuf, u32 num);
extern int efuse_write( u32 *pBuf, u32 group, u32 len );*/



#define SEC_OK 0
#define SEC_ERROR -1

enum sec_err_code
{
	SEC_ERR_NULL_PTR = 1,
	SEC_ERR_SHA_ERR,
	SEC_ERR_EFUSE_READ_ERR,
	SEC_ERR_NV_READ_ERR,
	SEC_ERR_IMG_SECURY_NOT_SUPPORT
};

#define SEC_ERROR_BASE_CODE              0x10000000
#define SEC_ERROR_NULL_PTR               (SEC_ERROR_BASE_CODE | SEC_ERR_NULL_PTR)
#define SEC_ERROR_SHA_ERR                (SEC_ERROR_BASE_CODE | SEC_ERR_SHA_ERR)
#define SEC_ERROR_EFUSE_READ_ERR         (SEC_ERROR_BASE_CODE | SEC_ERR_EFUSE_READ_ERR)
#define SEC_ERROR_NV_READ_ERR            (SEC_ERROR_BASE_CODE | SEC_ERR_NV_READ_ERR)
#define SEC_ERROR_IMG_SECURY_NOT_SUPPORT (SEC_ERROR_BASE_CODE | SEC_ERR_IMG_SECURY_NOT_SUPPORT)

#endif
