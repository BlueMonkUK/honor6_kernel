/*************************************************************************
*   ��Ȩ����(C) 1987-2004, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  OcrShare.h
*
*   ��    �� :  wuzechun
*
*   ��    �� :  OnChipRom��BootLoader����������
*
*   �޸ļ�¼ :  2011��4��15��  v1.00  wuzechun  ����
*
*************************************************************************/

#ifndef __OCRSHARE_H__
#define __OCRSHARE_H__

#include "types.h"
#include "config.h"

/*--------------------------------------------------------------*
 * �궨��                                                    *
 *--------------------------------------------------------------*/

/* ֻ��OEM CA�в���OEM ID��HWID,��CA���� */
#define KEY_OEMID_INDEX        sizeof(KEY_STRUCT)
#define KEY_HWID_INDEX         (KEY_OEMID_INDEX+0x04)

#define ROOT_CA_LEN            260      /*CA֤��ĳ��� 260 Byte*/
#define OEM_CA_LEN             268      /*CA֤��ĳ��� 268 Byte*/
#define IDIO_LEN               128      /*ǩ���ĳ��� 128 Byte*/

#define MAX_N_LENGTH    32

#define SHA256_HASH_SIZE  8       /*HASHǩ��ռ��word��*/

#define MD5_HASH_SIZE   4       /* MD5ժҪ��ռ��word�� */

/*--------------------------------------------------------------*
 * ���ݽṹ                                                     *
 *--------------------------------------------------------------*/

/* ��Կ��ʽ */
typedef struct
{
    UINT16  eLen;    /* E������64bitsΪ��λ��ȡֵ��ΧΪ0~15��0��ʾ16 */
    UINT16  nLen;    /* N������32bitsΪ��λ��ȡֵ��ΧΪ0~31��0��ʾ32 */
    UINT32  e[MAX_N_LENGTH];       /* ��e(��Կ) */
    UINT32  n[MAX_N_LENGTH];       /* ģ�� */
}KEY_STRUCT;


/* ǩ��У�麯��ָ�� */
typedef int (*idioIdentifyPtr)(UINT32 dataAddr, UINT32 dataLen, KEY_STRUCT *pKey, UINT32* pIdio);

/* ����Hashֵ����ָ�� */
typedef int (*SHA256HashPtr)(UINT32 dataAddr, UINT32 dataLen, UINT32* hash);

/* ����RSA����ָ�� */
typedef int (*RSAPtr)(KEY_STRUCT *pKey, UINT32* pIndata, UINT32* pOutdata);

/* �������ݽṹ��,����AXI����,�������¼�Ԫ��(�������ǰ��) */
typedef struct tagOcrShareData
{
    INT32 errno;

    /* ������ָ�� */
    SHA256HashPtr     SHA256Hash;
    RSAPtr          RSA;
    idioIdentifyPtr idioIdentify;

    /* �������ݶ� */
    BOOL    bSecEn;
    BOOL    bRootCaInited;
    KEY_STRUCT *pRootKey;
    UINT32  ulEnumFlag;             /* �Ծ���λ��־,AXI Mem Top - 8 */
    UINT32  ulOcrInitedFlag;        /* AXI Mem Top - 4 */
}tOcrShareData;                  /* AXI Mem Top */

int SHA256Hash(UINT32 dataAddr, UINT32 dataLen, UINT32* hash);

int RSA(KEY_STRUCT *pubKey, UINT32* pIndata, UINT32* pOutdata);

void ocrShareSave( void );

#endif /* OcrShare.h */


