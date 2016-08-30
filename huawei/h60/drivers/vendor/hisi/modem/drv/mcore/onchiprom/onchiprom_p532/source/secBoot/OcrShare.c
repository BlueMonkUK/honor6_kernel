/*************************************************************************
*   ��Ȩ����(C) 1987-2020, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  OcrShare.c
*
*   ��    �� :  wuzechun
*
*   ��    �� :  ���湲������
*
*   �޸ļ�¼ :  2011��7��18��  v1.00  wuzechun  ����
*
*************************************************************************/

#include "config.h"
#include "secBoot.h"
#include "OcrShare.h"
#include "string.h"
#include "cipher.h"
#include "nnex.h"
#include "sha2.h"


/******************************************************************************
*  Function:  SHA256Hash
*  Description:
*      SHA256.
*
*
*  Called By:
*      identify
*
*  Input:
*         dataAddr: �������ݵĵ�ַ
*         dataAddr: �������ݵĳ���(�ֽ���)
*
*  Output:
*         hash: ָ���������ݽ���SHA256�����Ĺ�ϣֵ��ָ��(��ϣֵ�ĳ���Ϊ32Byte)
*
*  Return:
*         OK: the function successful returned.
*         !OK: the function encounter OSAL_ERROR while running.
*
********************************************************************************/
int SHA256Hash(UINT32 dataAddr, UINT32 dataLen, UINT32* hash)
{
    UINT32 length = 0;
    UINT32 hmacKey[SHA256_HASH_SIZE];

    memset((void*)hmacKey, 0, sizeof(hmacKey));
    memset((void*)hash, 0, sizeof(hmacKey));

#ifdef CIPHER
    return bsp_calc_hash((UINT32*)dataAddr, dataLen, hmacKey, hash, &length);
#else
    while (dataLen >= 512)
    {
        memcpy(hmacKey, hash, sizeof(hmacKey));
        sha2_hmac((unsigned char*)hmacKey, sizeof(hmacKey), (unsigned char*)dataAddr, 512, (unsigned char *)hash, 0);
        dataLen -= 512;
        dataAddr += 512;
    }
    if (dataLen)
    {
        memcpy(hmacKey, hash, sizeof(hmacKey));
        sha2_hmac((unsigned char*)hmacKey, sizeof(hmacKey), (unsigned char*)dataAddr, dataLen, (unsigned char *)hash, 0);
    }
    return OK;
#endif
}

/******************************************************************************
*  Function:  RSA
*  Description:
*      RSA.
*
*  Calls:
*      SsdmaRsaInit, SetSsdmaRsaMode
*
*  Called By:
*      identify
*
*  Input:
*         pubKey: ��Կ
*         pIndata: �������ݵ�ָ��(�������ݵĳ��Ȳ�����128Byte)
*
*  Output:
*         pOutdata: ������ݵ�ָ��(������ݵĳ��Ȳ�����128Byte)
*
*  Return:
*         OK: the function successful returned.
*         !OK: the function encounter OSAL_ERROR while running.
*
********************************************************************************/
int RSA(KEY_STRUCT *pubKey, UINT32* pIndata, UINT32* pOutdata)
{
    if ((NULL == pubKey) || (NULL == pIndata) || (NULL == pOutdata))
        return !OK;

    NN_ModExpex(pOutdata, pIndata, pubKey->e, MAX_N_LENGTH, pubKey->n, MAX_N_LENGTH);

    return OK;
}

/*****************************************************************************
* �� �� ��  : ocrShareSave
*
* ��������  : ���湲������
*
* �������  :
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
void ocrShareSave( void )
{
    volatile tOcrShareData *pShareData = (tOcrShareData*)M3_SRAM_SHARE_DATA_ADDR;

    /* ���湲���BootLoader�ĺ���ָ��,�����Ƿ�ʹ�ܰ�ȫ,��ЩУ�麯�����ɸ��ϲ�ʹ�� */
    pShareData->idioIdentify = (idioIdentifyPtr)idioIdentify;
    pShareData->SHA256Hash = (SHA256HashPtr)SHA256Hash;
    pShareData->RSA = (RSAPtr)RSA;
    pShareData->ulOcrInitedFlag = OCR_INITED_FLAG_VALUE;
	pShareData->bsp_nand_get_spec_and_save = (bsp_nand_get_spec_and_save_ptr)bsp_nand_get_spec_and_save;	
}

