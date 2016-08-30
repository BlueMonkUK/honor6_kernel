#ifndef _BALONG_SECURY_COMN_H_
#define _BALONG_SECURY_COMN_H_

#include <bsp_om.h>
#define  security_print(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_SECURITY, "[SECURITY]: <%s> "fmt"\n", __FUNCTION__, ##__VA_ARGS__))
typedef enum
{
    IMG_SEC_OK = 0,               /*����У��ͨ��*/
    IMG_NOT_FOUND,                /*û���ҵ�����*/
    IMG_IS_UNSEC_TYPE,            /*�����Ƿǰ�ȫ��*/
    IMG_READ_FLASH_ERR,           /*��ȡFlash����*/
    IMG_FILE_CAN_NOT_OPEN,        /*�ļ����ܴ�*/
    IMG_FILE_CAN_NOT_SEEK,        /*�ļ������ƶ�ָ��*/
    IMG_FILE_CAN_NOT_FTELL,       /*��ȡ�ļ�����ʧ��*/
    IMG_FILE_READ_ERR,            /*��ȡ�ļ����ݳ���*/

    IMG_MEM_ERR,                  /*�ڴ����*/
    IMG_LEN_ERR,                  /*���񳤶ȴ���*/
    IMG_READ_OTP_ERR,             /*��ȡOTP����*/
    IMG_OTP_ID_NOT_EQUAL,         /*OTP ID��ƥ��*/
    
    IMG_SHA_ERR,                  /*����SHA�������*/
    IMG_RSA_ERR,                  /*����RSA���ܴ���*/
    IMG_SHA_RSA_NOT_EQUAL,        /*����Դ���ݼ����SHA��RSA���ܵ�SHAֵ��һ��*/

    IMG_CA_SHA_ERR,               /*����CA SHA�������*/
    IMG_CA_RSA_ERR,               /*����CA ����RSA����*/
    IMG_CA_SHA_RSA_NOT_EQUAL,     /*����CA��SHA��RSA��ƥ��*/

    IMG_IDIO_SHA_ERR,             /*�������ݼ���SHA����*/
    IMG_IDIO_RSA_ERR,             /*�������ݼ���RSA����*/
    IMG_IDIO_SHA_RSA_NOT_EQUAL,   /*�������ݵ�RSA��SHA��ƥ��*/

    IMG_CARDLOCK_READ_ERR,        /*��ȡCardLock NV����*/
    IMG_IMEI_READ_ERR,            /*��NV���ȡIMEI����*/
    IMG_IMEI_READ_OTP_ERR,        /*��ȡOTP����IMEI��SNR����*/
    IMG_IMEI_SNR_NOT_EQUAL,       /*OTP����IMEI��SNR��NV���������SNR��ƥ��*/ 

	CRYPTO_PTR_INVALID,           /*ָ����Ч*/
	CRYPTO_NV_READ_ERR,           /*��ȡNV��ʧ��*/
	CRYPTO_EFUSE_READ_ERR,        /*efuse��ʧ��*/
	CRYPTO_PARAM_INVALID,         /*��������ȷ*/
	CRYPTO_FASTBOOT_BACKUP_CA_ERR /*fastboot���ݵ�ROOT CA����*/
}IMG_SECCHK_RESULT;

#define SECURITY_BASE_ERR_CODE     0x10000000

#define SECURITY_SHA_ERR                 (SECURITY_BASE_ERR_CODE | IMG_SHA_ERR)
#define SECURITY_IMEI_NV_READ_ERR        (SECURITY_BASE_ERR_CODE | IMG_IMEI_READ_ERR)
#define SECURITY_PTR_NULL                (SECURITY_BASE_ERR_CODE | CRYPTO_PTR_INVALID)
#define SECURITY_NV_READ_ERR             (SECURITY_BASE_ERR_CODE | CRYPTO_NV_READ_ERR)
#define SECURITY_EFUSE_READ_ERR          (SECURITY_BASE_ERR_CODE | CRYPTO_EFUSE_READ_ERR)
#define SECURITY_IN_PARAM_ERR            (SECURITY_BASE_ERR_CODE | CRYPTO_PARAM_INVALID)
#define SECURITY_FASTBOOT_BACKUPCA_ERR   (SECURITY_BASE_ERR_CODE | CRYPTO_FASTBOOT_BACKUP_CA_ERR)

#endif
