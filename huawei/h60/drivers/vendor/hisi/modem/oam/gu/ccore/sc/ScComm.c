
/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "ScInterface.h"
#include "ScComm.h"
#include "FileSysInterface.h"
#include "ScCtx.h"
#include "om.h"

#ifdef __cplusplus
  #if __cplusplus
  extern "C"{
  #endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_SC_COMM_C

#define SC_LOG_MAX_LEN                  (512) 
#define SC_LOG_FILE_MAX_SIZE            (SC_LOG_MAX_LEN*100)    /* the max size of log file. */
#define SC_FILE_SEEK_END                2

#if(VOS_WIN32 == VOS_OS_VER)
#define SC_IMEI_LOG_FILE_PATH           ".\\yaffs0\\OAM_IMEI_Log.bin"
#define SC_IMEI_UNITARY_LOG_FILE_PATH   ".\\yaffs0\\OAM_IMEI_Log.bin"
#else

#if defined (INSTANCE_1)
#if (FEATURE_ON == FEATURE_MULTI_FS_PARTITION)
#define SC_IMEI_LOG_FILE_PATH           "/mnvm3:0/OAM_IMEI_Log1.bin"
#define SC_IMEI_UNITARY_LOG_FILE_PATH   "/modem_log/OAM_IMEI_Log1.bin"
#else
#define SC_IMEI_LOG_FILE_PATH           "/yaffs0/OAM_IMEI_Log1.bin"
#define SC_IMEI_UNITARY_LOG_FILE_PATH   "/modem_log/OAM_IMEI_Log1.bin"
#endif

#else
#if (FEATURE_ON == FEATURE_MULTI_FS_PARTITION)
#define SC_IMEI_LOG_FILE_PATH           "/mnvm3:0/OAM_IMEI_Log.bin"
#define SC_IMEI_UNITARY_LOG_FILE_PATH   "/modem_log/OAM_IMEI_Log.bin"
#else
#define SC_IMEI_LOG_FILE_PATH           "/yaffs0/OAM_IMEI_Log.bin"
#define SC_IMEI_UNITARY_LOG_FILE_PATH   "/modem_log/OAM_IMEI_Log.bin"
#endif
#endif
#endif

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  3 ��������
*****************************************************************************/


SC_ERROR_CODE_ENUM_UINT32 SC_COMM_ReadFile(
    VOS_CHAR                           *pcFilePath,
    VOS_UINT8                          *pucContent,
    VOS_UINT32                          ulContentLen
)
{
    VOS_INT32                          lReadSize;
    FILE                               *pFile;

    /* ��ָ�����͵�ǩ���ļ� */
    pFile = DRV_FILE_OPEN(pcFilePath, "rb");

    if (VOS_NULL_PTR == pFile)
    {
        SC_ERROR_LOG("SC_COMM_ReadFile: DRV_FILE_OPEN is failed.");

        return SC_ERROR_CODE_OPEN_FILE_FAIL;
    }

    /* ��ȡ�ļ����� */
    lReadSize = DRV_FILE_READ((VOS_CHAR*)pucContent, sizeof(VOS_CHAR), ulContentLen, pFile);

    if (ulContentLen != lReadSize)
    {
        DRV_FILE_CLOSE(pFile);

        SC_ERROR_LOG("SC_COMM_ReadFile: DRV_FILE_READ is failed.");

        return SC_ERROR_CODE_READ_FILE_FAIL;
    }

    /* �ر��ļ� */
    DRV_FILE_CLOSE(pFile);

    return SC_ERROR_CODE_NO_ERROR;
}
SC_ERROR_CODE_ENUM_UINT32 SC_COMM_WriteFile(
    VOS_CHAR                           *pcFilePath,
    VOS_UINT8                          *pucContent,
    VOS_UINT32                          ulContentLen
)
{
    VOS_INT32                           lWriteSize;
    FILE                               *pFile;

    /* ������ȫ�ļ� */
    pFile = DRV_FILE_OPEN(pcFilePath, "wb+");

    /* ���ؿ�ָ�룬���ش���ֵ */
    if (VOS_NULL_PTR == pFile)
    {
        SC_ERROR_LOG("SC_COMM_WriteFile: DRV_FILE_OPEN is failed.");

        return SC_ERROR_CODE_OPEN_FILE_FAIL;
    }

    /* д�밲ȫ�ļ����������س�����ʵ�ʳ��Ȳ�һ�£����ش���ֵ���ر��ļ� */
    lWriteSize = DRV_FILE_WRITE((VOS_CHAR*)pucContent, sizeof(VOS_CHAR), ulContentLen, pFile);

    if (lWriteSize != ulContentLen)
    {
        DRV_FILE_CLOSE(pFile);

        SC_ERROR_LOG("SC_COMM_WriteFile: DRV_FILE_WRITE is failed.");

        return SC_ERROR_CODE_WRITE_FILE_FAIL;
    }

    /* �ر��ļ� */
    DRV_FILE_CLOSE(pFile);

    return SC_ERROR_CODE_NO_ERROR;
}


SC_ERROR_CODE_ENUM_UINT32 SC_COMM_GenerateKey(
    VOS_CHAR                           *pcData,
    VOS_UINT32                          ulLen,
    VOS_UINT8                          *pucKey,
    VOS_UINT32                          ulKeyLen
)
{
    VOS_INT32                           lResult;
    VOS_INT                             lNewKeyLen;

    lNewKeyLen = (VOS_INT)ulKeyLen;

    /* ���õ���ӿ����� */
    lResult = CREATE_CRYPTO_KEY(pcData,
                                (VOS_INT)ulLen,
                                CREATE_CRYPTO_KEY_ALGORITHM_SHA256,
                                (VOS_CHAR*)pucKey,
                                &lNewKeyLen);

    /* ����㷨����ʧ�ܣ��ظ�������SC_ERROR_CODE_CREAT_KEY_FAIL */
    if ((VOS_OK != lResult) || (ulKeyLen != lNewKeyLen))
    {
        SC_ERROR2_LOG("SC_COMM_GenerateKey: CREATE_CRYPTO_KEY is failed.", lResult, lNewKeyLen);

        return SC_ERROR_CODE_CREATE_KEY_FAIL;
    }

    return SC_ERROR_CODE_NO_ERROR;
}


SC_ERROR_CODE_ENUM_UINT32 SC_COMM_GenerateCryptoPwd(
    SC_CRYPTO_PASSWORD_TYPE_ENUM_UINT8  enPwdType,
    VOS_UINT8                          *pucPwd,
    VOS_UINT8                           ucPwdLen,
    VOS_UINT8                          *pucCryptoPwd,
    VOS_UINT32                          ulCryptoPwdLen
)
{
    VOS_UINT8                           aucKey[SC_KEY_LEN];
    VOS_INT32                           lResult;
    VOS_UINT32                          ulResult;
    VOS_UINT32                          ulKeyLen;
    VOS_INT                             lNewCryptoPwdLen;

    ulKeyLen = SC_KEY_LEN;

    /* �����CK/UK */
    if (SC_CRYPTO_PASSWORD_TYPE_CK == enPwdType)
    {
        /* ����SSK */
        ulResult = SC_COMM_GenerateKey(SC_SSK_STRING, SC_SSK_STRLEN, aucKey, ulKeyLen);
    }
    /* �����DK */
    else if (SC_CRYPTO_PASSWORD_TYPE_DK == enPwdType)
    {
        /* ����DSSK */
        ulResult = SC_COMM_GenerateKey(SC_DSSK_STRING, SC_DSSK_STRLEN, aucKey, ulKeyLen);
    }
    else
    {
        SC_WARNING1_LOG("SC_COMM_GenerateCryptoPwd: Password Type is not support.", enPwdType);

        return SC_ERROR_CODE_NO_ERROR;
    }

    if (SC_ERROR_CODE_NO_ERROR != ulResult)
    {
        SC_ERROR2_LOG("SC_COMM_GenerateCryptoPwd: SC_COMM_GenerateKey is failed.", enPwdType, ulResult);

        return ulResult;
    }

    lNewCryptoPwdLen = (VOS_INT)ulCryptoPwdLen;
    /* ���õ���ӿڼ������� */
    lResult = CRYPTO_ENCRYPT((VOS_CHAR*)pucPwd,
                             ucPwdLen,
                             CRYPTO_ENCRYPT_ALGORITHM_AES_ECB,
                             (VOS_CHAR*)aucKey,
                             (VOS_INT)ulKeyLen,
                             (VOS_CHAR*)pucCryptoPwd,
                             &lNewCryptoPwdLen);

    /* �������ʧ�� */
    if ((VOS_OK != lResult) || (ulCryptoPwdLen != lNewCryptoPwdLen))
    {
        SC_ERROR2_LOG("SC_COMM_GenerateCryptoPwd: CRYPTO_ENCRYPT is failed.", lResult, lNewCryptoPwdLen);

        return SC_ERROR_CODE_AES_ECB_ENCRYPT_FAIL;
    }

    return SC_ERROR_CODE_NO_ERROR;
}


SC_ERROR_CODE_ENUM_UINT32 SC_COMM_GenerateSignCode(
    VOS_UINT8                          *pucContent,
    VOS_UINT32                          ulContentLen,
    VOS_UINT8                          *pucSignCode,
    VOS_UINT32                          ulSignCodeLen
)
{
    VOS_UINT8                           aucKey[SC_KEY_LEN];
    VOS_UINT8                           aucHashData[SC_HASH_LEN];
    VOS_INT                             lHashLen;
    VOS_UINT32                          ulKeyLen;
    VOS_UINT32                          ulResult;
    VOS_INT32                           lResult;
    VOS_INT                             lNewSignCodeLen;

    ulKeyLen  = SC_KEY_LEN;

    /* ����IPK */
    ulResult = SC_COMM_GenerateKey(SC_IPK_STRING, SC_IPK_STRLEN, aucKey, ulKeyLen);
    if (SC_ERROR_CODE_NO_ERROR != ulResult)
    {
        SC_ERROR1_LOG("SC_COMM_GenerateSignCode: SC_COMM_GenerateKey is failed.", ulResult);

        return ulResult;
    }

    /* SHA256(�ļ�����) */
    lHashLen = SC_HASH_LEN;

    lResult = CRYPTO_HASH((VOS_CHAR*)pucContent,
                          (VOS_INT)ulContentLen,
                          CRYPTO_ALGORITHM_SHA256,
                          (VOS_CHAR*)aucHashData,
                          &lHashLen);

    /* �㷨����ʧ�ܻ򳤶Ȳ�Ϊ32�ֽ� */
    if ((VOS_OK != lResult) || (SC_HASH_LEN != lHashLen))
    {
        SC_ERROR2_LOG("SC_COMM_GenerateSignCode: CRYPTO_HASH is failed.", lResult, lHashLen);

        return SC_ERROR_CODE_GENERATE_HASH_FAIL;
    }

    lNewSignCodeLen = (VOS_INT)ulSignCodeLen;
    /* AES_ECB(hash,IPK) */
    lResult = CRYPTO_ENCRYPT((VOS_CHAR*)aucHashData,
                             lHashLen,
                             CRYPTO_ENCRYPT_ALGORITHM_AES_ECB,
                             (VOS_CHAR*)aucKey,
                             (VOS_INT)ulKeyLen,
                             (VOS_CHAR*)pucSignCode,
                             &lNewSignCodeLen);

    /* �㷨����ʧ�ܻ򳤶Ȳ�Ϊ32�ֽ� */
    if ((VOS_OK != lResult) || (ulSignCodeLen != lNewSignCodeLen))
    {
        SC_ERROR2_LOG("SC_COMM_GenerateSignCode: CRYPTO_HASH is failed.", lResult, lNewSignCodeLen);

        return SC_ERROR_CODE_AES_ECB_ENCRYPT_FAIL;
    }

    return SC_ERROR_CODE_NO_ERROR;
}
SC_ERROR_CODE_ENUM_UINT32 SC_COMM_CreateSignFile(
    SC_SECRET_FILE_TYPE_ENUM_UINT8      enFileType,
    VOS_UINT8                          *pucContent,
    VOS_UINT32                          ulContentLen
)
{
    VOS_UINT32                          ulResult;
    VOS_UINT8                           aucSignCode[SC_HASH_LEN];
    SC_COMM_GLOBAL_STRU                *pstCommGlobal;

    /* �����ļ���ǩ�� */
    ulResult = SC_COMM_GenerateSignCode(pucContent,
                                        ulContentLen,
                                        aucSignCode,
                                        SC_HASH_LEN);

    if (SC_ERROR_CODE_NO_ERROR != ulResult)
    {
        SC_ERROR1_LOG("SC_COMM_CreateSignFile: SC_COMM_GenerateSignCode is failed.", ulResult);

        return ulResult;
    }

    pstCommGlobal = SC_CTX_GetCommGlobalVarAddr();

    ulResult = SC_COMM_WriteFile(pstCommGlobal->acSignFileName[enFileType],
                                 aucSignCode,
                                 SC_HASH_LEN);

    SC_INFO1_LOG("SC_COMM_CreateSignFile: SC_COMM_WriteFile result is", ulResult);

    return ulResult;
}
SC_ERROR_CODE_ENUM_UINT32 SC_COMM_ReadSignFile(
    SC_SECRET_FILE_TYPE_ENUM_UINT8      enFileType,
    VOS_UINT8                          *pucContent
)
{
    VOS_UINT32                          ulResult;
    SC_COMM_GLOBAL_STRU                *pstCommGlobal;

    pstCommGlobal = SC_CTX_GetCommGlobalVarAddr();

    ulResult = SC_COMM_ReadFile(pstCommGlobal->acSignFileName[enFileType],
                                pucContent,
                                SC_SIGNATURE_LEN);

    SC_INFO1_LOG("SC_COMM_ReadSignFile: SC_COMM_ReadFile result is", ulResult);

    return ulResult;
}


SC_ERROR_CODE_ENUM_UINT32 SC_COMM_WriteSecretFile(
    SC_SECRET_FILE_TYPE_ENUM_UINT8      enFileType,
    VOS_UINT8                          *pucContent,
    VOS_UINT32                          ulContentLen
)
{
    VOS_UINT32                          ulResult;
    SC_COMM_GLOBAL_STRU                *pstCommGlobal;

    pstCommGlobal = SC_CTX_GetCommGlobalVarAddr();

    ulResult = SC_COMM_WriteFile(pstCommGlobal->acSecretFileName[enFileType],
                                 pucContent,
                                 ulContentLen);

    if (SC_ERROR_CODE_NO_ERROR != ulResult)
    {
        SC_ERROR1_LOG("SC_COMM_WriteSecretFile: SC_COMM_WriteFile is failed.", ulResult);

        return ulResult;
    }

    /* �����ļ���Ӧ��ǩ���ļ� */
    ulResult = SC_COMM_CreateSignFile(enFileType, pucContent, ulContentLen);

    SC_INFO1_LOG("SC_COMM_WriteSecretFile: SC_COMM_CreateSignFile result is", ulResult);

    return ulResult;
}
SC_ERROR_CODE_ENUM_UINT32 SC_COMM_ReadSCFileAndCmpSign(
    SC_SECRET_FILE_TYPE_ENUM_UINT8      enFileType,
    VOS_UINT8                          *pucContent,
    VOS_UINT32                          ulContentLen
)
{
    VOS_UINT32                          ulResult;
    VOS_UINT8                           aucSignCode[SC_SIGNATURE_LEN];
    SC_SIGN_FILE_STRU                   stSignFile;
    SC_COMM_GLOBAL_STRU                *pstCommGlobal;

    pstCommGlobal = SC_CTX_GetCommGlobalVarAddr();

    ulResult = SC_COMM_ReadFile(pstCommGlobal->acSecretFileName[enFileType],
                                pucContent,
                                ulContentLen);

    if (SC_ERROR_CODE_NO_ERROR != ulResult)
    {
        SC_ERROR1_LOG("SC_COMM_ReadSCFileAndCmpSign: SC_COMM_ReadFile is failed.", ulResult);

        return ulResult;
    }

    /* �����ļ���ǩ������ */
    ulResult = SC_COMM_GenerateSignCode(pucContent,
                                        ulContentLen,
                                        aucSignCode,
                                        SC_SIGNATURE_LEN);

    if (SC_ERROR_CODE_NO_ERROR != ulResult)
    {
        SC_ERROR1_LOG("SC_COMM_ReadSCFileAndCmpSign: SC_COMM_GenerateSignCode is failed.", ulResult);

        return ulResult;
    }

    /* ��ȡǩ���ļ� */
    ulResult = SC_COMM_ReadSignFile(enFileType, stSignFile.aucSignature);
    if (SC_ERROR_CODE_NO_ERROR != ulResult)
    {
        SC_ERROR1_LOG("SC_COMM_ReadSCFileAndCmpSign: SC_COMM_ReadSignFile is failed.", ulResult);

        return ulResult;
    }

    /* �Ƚ������ļ�ǩ�� */
    if (VOS_OK != VOS_MemCmp(aucSignCode, stSignFile.aucSignature, SC_SIGNATURE_LEN))
    {
        SC_ERROR_LOG("SC_COMM_ReadSCFileAndCmpSign: Signature compare is failed.");

        return SC_ERROR_CODE_VERIFY_SIGNATURE_FAIL;
    }

    return SC_ERROR_CODE_NO_ERROR;
}
SC_ERROR_CODE_ENUM_UINT32 SC_COMM_RsaDecrypt(
    VOS_UINT8                          *pucPwd,
    VOS_UINT32                          ulCipherLen,
    VOS_UINT8                          *pucPubContent,
    VOS_UINT32                         *pulPubLen
)
{
    VOS_UINT32                          ulResult;
    VOS_INT32                           lResult;
    VOS_UINT32                          ulLen;
    SC_AK_FILE_STRU                    *pstAKFile;

    pstAKFile = (SC_AK_FILE_STRU*)VOS_MemAlloc(WUEPS_PID_OM, DYNAMIC_MEM_PT, sizeof(SC_AK_FILE_STRU));

    if (VOS_NULL_PTR == pstAKFile)
    {
        SC_ERROR_LOG("SC_COMM_RsaDecrypt: VOS Alloc Memory is failed.");
        /* �����ڴ�����ʧ�� */
        return SC_ERROR_CODE_ALLOC_MEM_FAIL;
    }

    /* ��ȡAK-FILE��У��ǩ�� */
    ulResult = SC_COMM_ReadSCFileAndCmpSign(SC_SECRET_FILE_TYPE_AK,
                                            pstAKFile->aucAuthPubKey,
                                            sizeof(SC_AK_FILE_STRU));

    if (SC_ERROR_CODE_NO_ERROR != ulResult)
    {
        VOS_MemFree(WUEPS_PID_OM, pstAKFile);

        SC_ERROR1_LOG("SC_COMM_RsaDecrypt: SC_COMM_ReadSCFileAndCmpSign is failed.", ulResult);

        return ulResult;
    }

    ulLen = *pulPubLen;

    /* ����������Ľ���RSA���� */
    lResult = CRYPTO_RSA_DECRYPT((VOS_CHAR*)pucPwd,
                                 (VOS_INT)ulCipherLen,
                                 (VOS_CHAR*)pstAKFile->aucAuthPubKey,
                                 sizeof(SC_AK_FILE_STRU),
                                 (VOS_CHAR*)pucPubContent,
                                 (VOS_INT*)pulPubLen);

    /* ������ʧ�� */
    if ((VOS_OK != lResult) || (ulLen != *pulPubLen))
    {
        VOS_MemFree(WUEPS_PID_OM, pstAKFile);

        SC_ERROR2_LOG("SC_COMM_RsaDecrypt: CRYPTO_RSA_DECRYPT is failed.", lResult, *pulPubLen);

        return SC_ERROR_CODE_RSA_DECRYPT_FAIL;
    }

    VOS_MemFree(WUEPS_PID_OM, pstAKFile);

    return SC_ERROR_CODE_NO_ERROR;
}


SC_ERROR_CODE_ENUM_UINT32 SC_COMM_RsaEncrypt(
    VOS_UINT8                          *pucRawData,
    VOS_UINT32                          ulLen,
    VOS_UINT8                          *pucCipherData,
    VOS_UINT32                         *pulCipherLen
)
{
    VOS_UINT32                          ulResult;
    VOS_INT32                           lResult;
    SC_AK_FILE_STRU                    *pstAKFile;

    pstAKFile = (SC_AK_FILE_STRU*)VOS_MemAlloc(WUEPS_PID_OM, DYNAMIC_MEM_PT, sizeof(SC_AK_FILE_STRU));

    if (VOS_NULL_PTR == pstAKFile)
    {
        SC_ERROR_LOG("SC_COMM_RsaDecrypt: VOS Alloc Memory is failed.");
        /* �����ڴ�����ʧ�� */
        return SC_ERROR_CODE_ALLOC_MEM_FAIL;
    }

    /* ��ȡAK-FILE��У��ǩ�� */
    ulResult = SC_COMM_ReadSCFileAndCmpSign(SC_SECRET_FILE_TYPE_AK,
                                            pstAKFile->aucAuthPubKey,
                                            sizeof(SC_AK_FILE_STRU));

    if (SC_ERROR_CODE_NO_ERROR != ulResult)
    {
        VOS_MemFree(WUEPS_PID_OM, pstAKFile);

        SC_ERROR1_LOG("SC_COMM_RsaEncrypt: SC_COMM_ReadSCFileAndCmpSign is failed.", ulResult);

        return ulResult;
    }

    *pulCipherLen = SC_RSA_ENCRYPT_LEN;

    /* ����������Ľ���RSA���� */
    lResult = CRYPTO_RSA_ENCRYT((VOS_CHAR*)pucRawData,
                                (VOS_INT)ulLen,
                                (VOS_CHAR*)pstAKFile->aucAuthPubKey,
                                sizeof(SC_AK_FILE_STRU),
                                (VOS_CHAR*)pucCipherData,
                                (VOS_INT*)pulCipherLen);

    /* ������ʧ�� */
    if ((VOS_OK != lResult) || (SC_RSA_ENCRYPT_LEN != *pulCipherLen))
    {
        VOS_MemFree(WUEPS_PID_OM, pstAKFile);

        SC_ERROR2_LOG("SC_COMM_RsaEncrypt: CRYPTO_RSA_DECRYPT is failed.", lResult, *pulCipherLen);

        return SC_ERROR_CODE_RSA_ENCRYPT_FAIL;
    }

    VOS_MemFree(WUEPS_PID_OM, pstAKFile);

    return SC_ERROR_CODE_NO_ERROR;
}


VOS_UINT32 SC_COMM_GetUsimmCachedFile(
    VOS_UINT16                          usFileID,
    VOS_UINT32                         *pulDataLen,
    VOS_UINT8                         **ppucData,
    USIMM_APP_TYPE_ENUM_UINT32          enAppType,
    MODEM_ID_ENUM_UINT16                enModemID)
{
    if (VOS_TRUE == USIMM_IsSvlte())
    {
        return USIMM_GetCachedFile_Instance(MODEM_ID_0, usFileID, pulDataLen, ppucData, enAppType);
    }

    return USIMM_GetCachedFile_Instance(enModemID, usFileID, pulDataLen, ppucData, enAppType);
}


VOS_UINT32 SC_COMM_IsUsimServiceAvailable(
    UICC_SERVICES_TYPE_ENUM_UINT32  enService,
    MODEM_ID_ENUM_UINT16            enModemID)
{
    if (VOS_TRUE == USIMM_IsSvlte())
    {
        return USIMM_IsServiceAvailable_Instance(MODEM_ID_0, enService);
    }

    return USIMM_IsServiceAvailable_Instance(enModemID, enService);
}

/*****************************************************************************
�� �� ��  :SC_Printf
��������  :Log��ӡ,�ɱ����
*****************************************************************************/
/*lint -e960 */
VOS_VOID SC_Printf(const VOS_CHAR *pcformat, ...)
/*lint +e960 */
{
    VOS_CHAR                            aucIMEILogFilePath[100];
    VOS_CHAR                            acOutput[SC_LOG_MAX_LEN];
    va_list                             argument;
    VOS_UINT32                          ulRetLen;
    FILE                               *fp;
    VOS_UINT32                          ulFileSize;

    /* ����ʽ���ַ����Ϳɱ����ת��Ϊ�ַ��� */
    va_start(argument, pcformat);
    ulRetLen = (VOS_UINT32)vsnprintf(acOutput, SC_LOG_MAX_LEN, pcformat, argument);
    va_end(argument);

    if ((SC_LOG_MAX_LEN < ulRetLen) || (0 == ulRetLen))
    {
        return ;
    }

    if (VOS_OK != OM_GetLogPath(aucIMEILogFilePath, SC_IMEI_LOG_FILE_PATH, SC_IMEI_UNITARY_LOG_FILE_PATH))
    {
        return ;
    }

#if ((VOS_WIN32 == VOS_OS_VER)||(VOS_VXWORKS == VOS_OS_VER)||(VOS_RTOSCK == VOS_OS_VER))
    fp = DRV_FILE_OPEN(aucIMEILogFilePath, "a");

    if (VOS_NULL_PTR == fp)
    {
        return ;
    }

    DRV_FILE_LSEEK(fp, 0, SC_FILE_SEEK_END);

    ulFileSize = (VOS_UINT32)DRV_FILE_TELL(fp);

    /* If the log file is too large, we need empty it. */
    if (ulFileSize > SC_LOG_FILE_MAX_SIZE)
    {
        DRV_FILE_CLOSE(fp);
        fp = DRV_FILE_OPEN(aucIMEILogFilePath, "w");

        if (VOS_NULL_PTR == fp)
        {
            return ;
        }
    }

    DRV_FILE_WRITE(acOutput, ulRetLen, 1, fp);

    DRV_FILE_CLOSE(fp);

#endif

    return ;
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

