/*************************************************************************
*   ��Ȩ����(C) 1987-2004, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  types.h
*
*   ��    �� :  wuzechun
*
*   ��    �� :  �������Ͷ���ͷ�ļ�
*
*   �޸ļ�¼ :  2011��4��22��  v1.00  wuzechun  ����
*
*************************************************************************/

#ifndef __TYPES_H__
#define __TYPES_H__

/*--------------------------------------------------------------*
 * �궨��                                                       *
 *--------------------------------------------------------------*/

typedef char            INT8;
typedef short           INT16;
typedef long            INT32;
typedef long long       INT64;
typedef unsigned char   UINT8;
typedef unsigned short  UINT16;
typedef unsigned long   UINT32;
typedef unsigned long long    UINT64;

#define PUCHAR  UINT8*
#define NULL    (void *)0
#define VOID    void

#define BOOL    int
#define STATUS  int

#define ERROR         -1
#define OK            0

#define TRUE 1
#define FALSE 0


typedef void         (*FUNCPTR)();


/* ��һ�������붨�� */
#define ERR_OK                  0
#define ERR_NO_BASE             0x10000000

#define ERR_SEC_BASE            (ERR_NO_BASE + 0)
#define ERR_NAND_BASE           (ERR_NO_BASE + 0x1000)
#define ERR_SPI_BASE            (ERR_NO_BASE + 0x2000)
#define ERR_HSUART_BASE         (ERR_NO_BASE + 0x3000)
#define ERR_USB_BASE            (ERR_NO_BASE + 0x4000)
#define ERR_MMC_BASE            (ERR_NO_BASE + 0x5000)
#define ERR_SYS_BASE            (ERR_NO_BASE + 0x9000)

/* ��ȫУ��ģ������붨�� */
#define SEC_SUCCESS             0
#define SEC_EFUSE_READ_ERROR    (ERR_SEC_BASE + 1)                /*EFUSE ��ȡERROR*/
#define SEC_EFUSE_NOT_WRITE     (ERR_SEC_BASE + 2)                     /*EFUSE δ��д*/
#define SEC_OEMCA_ERROR         (ERR_SEC_BASE + 3)                          /*CA ERROR*/
#define SEC_IMAGE_ERROR         (ERR_SEC_BASE + 4)                  /*ӳ��ERROR*/
#define SEC_ROOT_CA_ERROR       (ERR_SEC_BASE + 5)            /* ��CAУ����� */
#define SEC_IMAGE_LEN_ERROR     (ERR_SEC_BASE + 6)            /*��ȫ�汾ӳ�󳤶ȴ���*/
#define SEC_SHA_CALC_ERROR      (ERR_SEC_BASE + 7)            /* SHA������� */
#define SEC_RSA_CALC_ERROR      (ERR_SEC_BASE + 8)            /* RSA������� */
#define SEC_NO_IMAGE            (ERR_SEC_BASE + 9)          /* û����дBootLoader���� */
#define NAND_NO_IN_BOOTMODE     (ERR_SEC_BASE + 10)          /* Nand���ڷ�Bootģʽ,��Ҫ�����������Bootģʽ*/

/* NANDCģ������붨�� */
#define NAND_ECC_ERR            (ERR_NAND_BASE + 1)            /* Nand Flash����Ecc���ɾ����Ĵ��� */

/* SPIģ������붨�� */
typedef enum _tagSpiReadStatus_t
{
    SPI_READ_OK = 0,
    SPI_READ_ERR_PARA = (ERR_SPI_BASE+1),
    SPI_READ_ERR_RECV,
    SPI_READ_ERR_GETSTATUS,
    SPI_READ_ERR_READ_LENBYTE,
    SPI_READ_ERR_READ_BL,
    SPI_READ_ERR_BL_LEN
}spiReadStatus;

/* AP HSUARTģ������붨�� */
#define ERR_AP_HSUART_TRY_ALL_FAILED    (ERR_HSUART_BASE + 1)

/* ϵͳ�����붨�� */
typedef enum _tagSYS_ERRNO_E
{
    SYS_ERR_USB_BOOT_RETURNS = (ERR_SYS_BASE+1),
    SYS_ERR_AP_DL_RETURNS,
    SYS_ERR_EFUSE_READ,
    SYS_ERR_BOOT_MODE,
    SYS_ERR_HSIC_BOOT_RETURNS,
    SYS_ERR_SEC_UNKNOWN_RET
}SYS_ERRNO_E;

typedef enum _tagDELAY_TIME_MS
{
    TIME_DELAY_MS_1000_FOR_UBOOT_RETURN  = 1000,
    TIME_DELAY_MS_2000_FOR_EFUSE_READERR = 2000,
    TIME_DELAY_MS_3000_FOR_BOOT_MODE     = 3000,
    TIME_DELAY_MS_4000_FOR_AP_RETURNS    = 4000,
    TIME_DELAY_MS_5000_FOR_SEC_UNKNOWN_RET       = 5000,
    TIME_DELAY_MS_6000_FOR_NF_OPBOOT = 6000,
}DELAY_TIME;

#endif /* types.h */

