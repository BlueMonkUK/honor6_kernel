

/*lint --e{740,958,537,767,960,718,746}*/

#include "vos_Id.h"
#include "usimmbase.h"
#include "DrvInterface.h"
#include "usimmapdu.h"
#include "usimmdl.h"
#include "om.h"
#include "usimmvsimauth.h"
#include "FileSysInterface.h"
#include "softcrypto.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */


/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/

#define    THIS_FILE_ID PS_FILE_ID_USIMM_VSIM_C


#if (FEATURE_VSIM == FEATURE_ON)

USIMM_VSIM_FILE_INFO_STRU g_astUSIMMVSimFileInfo[] =
{
    {"EFIMSI",      0x6F07, USIMM_MANDATORY_FILE,       USIMM_VSIM_WRITE_UNAVALIBALE},
    {"EFAD",        0x6FAD, USIMM_MANDATORY_FILE,       USIMM_VSIM_WRITE_UNAVALIBALE},
    {"EFACC",       0x6F78, USIMM_MANDATORY_FILE,       USIMM_VSIM_WRITE_UNAVALIBALE},
    {"EFBCCH",      0x6F74, USIMM_SIM_MANDATORY_FILE,   USIMM_VSIM_WRITE_AVALIBALE},
    {"EFFPLMN",     0x6F7B, USIMM_MANDATORY_FILE,       USIMM_VSIM_WRITE_AVALIBALE},
    {"EFPSLOCI",    0x6F73, USIMM_USIM_MANDATORY_FILE,  USIMM_VSIM_WRITE_AVALIBALE},
    {"EFLOCI",      0x6F7E, USIMM_MANDATORY_FILE,       USIMM_VSIM_WRITE_AVALIBALE},
    {"EFEST",       0x6F56, USIMM_USIM_MANDATORY_FILE,  USIMM_VSIM_WRITE_UNAVALIBALE},
    {"EFUST",       0x6F38, USIMM_MANDATORY_FILE,       USIMM_VSIM_WRITE_UNAVALIBALE},
    {"EFHPPLMN",    0x6F31, USIMM_MANDATORY_FILE,       USIMM_VSIM_WRITE_UNAVALIBALE},
    {"EFNETPAR",    0x6FC4, USIMM_USIM_MANDATORY_FILE,  USIMM_VSIM_WRITE_AVALIBALE},
    {"EFTHRESHOLD", 0x6F5C, USIMM_USIM_MANDATORY_FILE,  USIMM_VSIM_WRITE_UNAVALIBALE},
    {"EFSTARTHFN",  0x6F5B, USIMM_USIM_MANDATORY_FILE,  USIMM_VSIM_WRITE_AVALIBALE},
    {"EFEHPLMN",    0x6FD9, USIMM_OPTIONAL_FILE,        USIMM_VSIM_WRITE_UNAVALIBALE},
    {"EFPLMNWACT",  0x6F60, USIMM_OPTIONAL_FILE,        USIMM_VSIM_WRITE_AVALIBALE},
    {"EFSMSP",      0x6F42, USIMM_OPTIONAL_FILE,        USIMM_VSIM_WRITE_AVALIBALE},
    {"EFSMSS",      0x6F43, USIMM_OPTIONAL_FILE,        USIMM_VSIM_WRITE_AVALIBALE},
    {"EFLOCIGPRS",  0x6F53, USIMM_OPTIONAL_FILE,        USIMM_VSIM_WRITE_AVALIBALE}
};

USIMM_VSIM_AUTH_INFO_STRU g_stUSIMMVSimAuthInfo;
VOS_CHAR *g_pcUSIMMVSimXmlFilePath = VSIM_XML_FILE_PATH;


/*****************************************************************************
�� �� ��  : USIMM_File_Open
��������  : ���ļ�
�������  : pcFileName   --- �ļ���
            pcAccessMode --- �򿪷�ʽ
�������  : ��
�� �� ֵ  : SI_UINT32 ����ִ�н��
���ú���  :
��������  : �ⲿ�ӿ�

�޸���ʷ      :
1.��    ��  : 2013��08��28��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
FILE *USIMM_File_Open(VOS_CHAR *pcFileName, VOS_CHAR *pcAccessMode)
{
    FILE *fp;

    fp = DRV_FILE_OPEN(pcFileName,pcAccessMode);

    if ( VOS_NULL_PTR == fp )
    {
        USIMM_WARNING_LOG("Open File fail!");/* [false alarm]:*/
    }

    return fp;
}

/*****************************************************************************
�� �� ��  : USIMM_File_Close
��������  : �ر��ļ�
�������  : Fp   --- �ļ����
�������  : ��
�� �� ֵ  : SI_UINT32 ����ִ�н��
���ú���  :
��������  : �ⲿ�ӿ�

�޸���ʷ      :
1.��    ��  : 2013��08��28��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_INT32 USIMM_File_Close(FILE *Fp)
{
    VOS_INT32 lRslt = VOS_ERR;

    lRslt = DRV_FILE_CLOSE(Fp);

    if ( VOS_OK != lRslt )
    {
        USIMM_WARNING_LOG("Close File fail!");
    }

    return lRslt;
}

/*****************************************************************************
�� �� ��  : USIMM_File_Write
��������  : д�ļ�
�������  : pBuf     --- д������ָ��
            ulSize   --- д�����ݳ���
            ulCount  --- Ҫд�����ݵ����������
            Fp       --- �ļ����
�������  : ��
�� �� ֵ  : SI_UINT32 ����ִ�н��
���ú���  :
��������  : �ⲿ�ӿ�

�޸���ʷ      :
1.��    ��  : 2013��08��28��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_INT32 USIMM_File_Write(VOS_VOID *pBuf,VOS_UINT32 ulSize,VOS_UINT32 ulCount,FILE *Fp)
{
    VOS_INT32                             lRslt;

    lRslt = DRV_FILE_WRITE(pBuf,ulSize,ulCount,Fp);

    if ( lRslt != (VOS_INT32)ulCount )
    {
        USIMM_WARNING_LOG("Write File fail!");
    }

    return lRslt;

}

/*****************************************************************************
�� �� ��  : USIMM_File_Seek
��������  : �ض�λ�ļ�ָ��
�������  : Fp       --- �ļ����
            lOffset  --- ƫ����
            lWhence  --- ƫ����ʼλ��
�������  : ��
�� �� ֵ  : SI_UINT32 ����ִ�н��
���ú���  :
��������  : �ⲿ�ӿ�

�޸���ʷ      :
1.��    ��  : 2013��08��28��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_INT32 USIMM_File_Seek( FILE *Fp,VOS_INT32 lOffset,VOS_INT32 lWhence)
{
    VOS_INT32 ulRslt = VOS_ERR;

    ulRslt = DRV_FILE_LSEEK(Fp,lOffset,lWhence);

    if ( VOS_OK != ulRslt )
    {
        USIMM_WARNING_LOG("Seek File fail!");
    }

    return ulRslt;
}

/*****************************************************************************
�� �� ��  : USIMM_File_Tell
��������  : ����FILE��ǰָ��λ��
�������  : Fp       --- �ļ����
�������  : ��
�� �� ֵ  : SI_UINT32 ����ִ�н��
���ú���  :
��������  : �ⲿ�ӿ�

�޸���ʷ      :
1.��    ��  : 2013��08��28��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_INT32 USIMM_File_Tell(FILE *Fp)
{
    VOS_INT32 lFileSize = VOS_ERROR;

    lFileSize = DRV_FILE_TELL(Fp);

    if ( VOS_ERROR == lFileSize)
    {
        USIMM_WARNING_LOG("Tell File Size fail!");
    }

    return lFileSize;
}

/*****************************************************************************
�� �� ��  : USIMM_File_Read
��������  : ��һ���ļ��ж�ȡ����
�������  :
            pBuf     --- ���ڽ������ݵ��ڴ��ַ
            ulSize   --- ����Ԫ�صĴ�С
            ulCount  --- Ҫ��ȡ���ݵ����������
            Fp       --- �ļ����
�������  : ��
�� �� ֵ  : SI_UINT32 ����ִ�н��
���ú���  :
��������  : �ⲿ�ӿ�

�޸���ʷ      :
1.��    ��  : 2013��08��28��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_INT32 USIMM_File_Read(VOS_VOID *pBuf,VOS_UINT32 ulSize,VOS_UINT32 ulCount,FILE *Fp)
{
    VOS_INT                             lRslt;

    lRslt = DRV_FILE_READ(pBuf,ulSize,ulCount,Fp);

    if ( (VOS_UINT32)lRslt != ulCount )
    {
        USIMM_WARNING_LOG("Read File fail!");
    }

    return lRslt;
}

/*****************************************************************************
�� �� ��  : USIMM_File_Rename
��������  : �ļ�������
�������  : pcOldFileName   --- ���ļ���
            pcNewFileName   --- ���ļ���
�������  : ��
�� �� ֵ  : SI_UINT32 ����ִ�н��
���ú���  :
��������  : �ⲿ�ӿ�

�޸���ʷ      :
1.��    ��  : 2013��08��28��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_INT32 USIMM_File_Rename(VOS_CHAR *pcOldFileName,VOS_CHAR *pcNewFileName)
{
    VOS_INT32 ulRslt;

    ulRslt = DRV_FILE_RENAME(pcOldFileName,pcNewFileName);

    if(VOS_OK != ulRslt)
    {
        USIMM_WARNING_LOG("Rename file fail!");
    }

    return ulRslt;
}

/*****************************************************************************
�� �� ��  : USIMM_OpenDir
��������  : ���ļ���
�������  : pcDirName   --- �ļ���·��
�������  : ��
�� �� ֵ  : SI_UINT32 ����ִ�н��
���ú���  :
��������  : �ⲿ�ӿ�

�޸���ʷ      :
1.��    ��  : 2013��08��28��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_INT32 USIMM_OpenDir(VOS_CHAR *pcDirName)
{
    DRV_DIR_STRU           *pstTmpDir;

    /* coverity[alloc_fn] */
    pstTmpDir = DRV_FILE_OPENDIR(pcDirName);

    if(VOS_NULL_PTR == pstTmpDir)
    {
        USIMM_WARNING_LOG("Make Dir fail!");
        return VOS_ERR;
    }
    /* coverity[leaked_storage] */
    return VOS_OK;
}

/*****************************************************************************
�� �� ��  : USIMM_Mkdir
��������  : �����ļ���
�������  : pcDirName   --- �ļ���·��
�������  : ��
�� �� ֵ  : SI_UINT32 ����ִ�н��
���ú���  :
��������  : �ⲿ�ӿ�

�޸���ʷ      :
1.��    ��  : 2013��08��28��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_INT32 USIMM_Mkdir(VOS_CHAR *pcDirName)
{
    VOS_INT32 ulRslt;

    ulRslt = DRV_FILE_MKDIR(pcDirName);

    if(VOS_OK != ulRslt)
    {
        USIMM_WARNING_LOG("Make Dir fail!");
    }

    return ulRslt;
}



VOS_VOID USIMM_ClearVsimGlobal(VOS_VOID)
{
    USIMM_PoolDelAll();

    USIMM_InitGlobalVarOnReset();

    VOS_MemSet(&g_stUSIMMVSimAuthInfo, 0, sizeof(g_stUSIMMVSimAuthInfo));

    return;
}


VOS_VOID USIMM_InitVsimGlobal(VOS_VOID)
{
    gstUSIMMPOOL.enPoolStatus = USIMM_POOL_AVAILABLE;   /*���õ�ǰ��POOL��״̬*/

    gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService = USIMM_CARD_SERVIC_AVAILABLE;

    /*��ʼ����ǰ��PIN��������*/
    gstUSIMMBaseInfo.enCurPINType  = USIMM_PIN2;

    /*��ʼ��Ĭ�ϴ���*/
    gstUSIMMADFInfo.stPINInfo.ucPin1RemainTime = 3;

    gstUSIMMADFInfo.stPINInfo.ucPin2RemainTime = 3;

    gstUSIMMADFInfo.stPINInfo.ucPuk1RemainTime = 10;

    gstUSIMMADFInfo.stPINInfo.ucPuk2RemainTime = 10;

    return;
}
VOS_UINT32 USIMM_VsimGetRealKiOpc(VOS_UINT8 *pucKi, VOS_UINT8 *pucOpc)
{
    VOS_UINT8                           aucKey[VSIM_DH_AGREE_KEY] = {0};
    VOS_UINT8                           aucKi[USIMM_VSIM_SECUR_MAX_LEN];
    VOS_UINT8                           aucOpc[USIMM_VSIM_SECUR_MAX_LEN];
    NVIM_VSIM_HVSDH_NV_STRU             stNVDHKey;
    DH_KEY                              stDHPara;

    VOS_MemSet(&stDHPara, 0, sizeof(stDHPara));

    if(VOS_NULL == g_stUSIMMVSimAuthInfo.ucKILen)
    {
        USIMM_ERROR_LOG("USIMM_VsimGetRealKiOpc: Ki Len is 0");
        return VOS_ERR;
    }

    if(NV_OK != NV_Read(en_NV_Item_VSIM_HVSDH_INFO, &stNVDHKey, sizeof(NVIM_VSIM_HVSDH_NV_STRU)))
    {
        USIMM_ERROR_LOG("USIMM_VsimGetRealKiOpc: NV Read Key is Failed");

        return VOS_ERR;
    }

    if(VSIM_ALGORITHM_NULL == stNVDHKey.enAlgorithm)
    {
        USIMM_WARNING_LOG("USIMM_VsimGetRealKiOpc: Algorithm is OFF");

        VOS_MemCpy(pucKi, g_stUSIMMVSimAuthInfo.aucKi, USIMM_AUTH_KI_LEN);

        VOS_MemCpy(pucOpc, g_stUSIMMVSimAuthInfo.aucOpc, USIMM_AUTH_OPC_LEN);

        return VOS_OK;
    }

    VOS_MemCpy(stDHPara.privateValue, stNVDHKey.stCPrivateKey.aucKey, VSIM_DH_PRIVATE_KEY);

    stDHPara.priVallen = VSIM_DH_PRIVATE_KEY;

    DH_FillFixParams(&stDHPara);

    if(VOS_OK != DH_ComputeAgreedKey(aucKey, stNVDHKey.stSPublicKey.aucKey, &stDHPara))
    {
        USIMM_ERROR_LOG("USIMM_VsimGetRealKiOpc: DH_ComputeAgreedKey is Failed");
        return VOS_ERR;
    }

    /* coverity[uninit_use_in_call] */
    if(VOS_FALSE == AESDecrypt(g_stUSIMMVSimAuthInfo.aucKi, g_stUSIMMVSimAuthInfo.ucKILen, aucKey, sizeof(aucKey), aucKi, sizeof(aucKi)))
    {
        USIMM_ERROR_LOG("USIMM_VsimGetRealKiOpc: AESDecrypt Ki is Failed");
        return VOS_ERR;
    }

    VOS_MemCpy(pucKi, aucKi, USIMM_AUTH_KI_LEN);

    if(VOS_NULL == g_stUSIMMVSimAuthInfo.ucOpcLen)
    {
        USIMM_NORMAL_LOG("USIMM_VsimGetRealKiOpc: Only get Ki Data");
        return VOS_OK;
    }

    /* coverity[uninit_use_in_call] */
    if(VOS_FALSE == AESDecrypt(g_stUSIMMVSimAuthInfo.aucOpc, g_stUSIMMVSimAuthInfo.ucOpcLen, aucKey, sizeof(aucKey), aucOpc, sizeof(aucOpc)))
    {
        USIMM_ERROR_LOG("USIMM_VsimGetRealKiOpc: AESDecrypt Opc is Failed");
        return VOS_ERR;
    }

    /* coverity[uninit_use_in_call] */
    VOS_MemCpy(pucOpc, aucOpc, USIMM_AUTH_OPC_LEN);

    USIMM_NORMAL_LOG("USIMM_VsimGetRealKiOpc: Get Ki and Opc Data");

    return VOS_OK;
}
VOS_UINT32 USIMM_SetVsimFile(USIMM_SETFILE_REQ_STRU *pstMsg)
{
    VOS_UINT32                          ulResult;
    VOS_UINT32                          i;
    USIMM_SETCNF_INFO_STRU              stCnfInfo;

    stCnfInfo.ucRecordNum  = pstMsg->ucRecordNum;
    stCnfInfo.usEFLen      = pstMsg->usDataLen;
    stCnfInfo.usEfid       = pstMsg->usFileID;

    for(i=0; i<ARRAYSIZE(g_astUSIMMVSimFileInfo); i++)
    {
        if(pstMsg->usFileID == g_astUSIMMVSimFileInfo[i].usFileID)
        {
            break;
        }
    }

    if(i >= ARRAYSIZE(g_astUSIMMVSimFileInfo))  /*û���ҵ�*/
    {
        USIMM_SetFileCnf(pstMsg->ulSenderPid,
                         pstMsg->ulSendPara,
                         VOS_OK,
                         &stCnfInfo);/*�ظ����½��,Ĭ�ϰ��ճɹ��ظ�*/

        return VOS_OK;
    }

    if(USIMM_VSIM_WRITE_AVALIBALE != g_astUSIMMVSimFileInfo[i].enFileAC)
    {
        USIMM_SetFileCnf(pstMsg->ulSenderPid,
                         pstMsg->ulSendPara,
                         USIMM_SW_SECURITY_ERROR,
                         &stCnfInfo);/*�ظ����½��,Ĭ�ϰ���ʧ�ܻظ�*/

        return VOS_OK;
    }

    ulResult = USIMM_PoolDelOneFile(pstMsg->usFileID, USIMM_UNLIMIT_APP);

    if(VOS_OK == ulResult)/*����ļ������ڻ���������Ҫ����*/
    {
        USIMM_PoolInsertOneFile(pstMsg->usFileID,
                                pstMsg->usDataLen,
                                pstMsg->aucContent,
                                USIMM_UNLIMIT_APP,
                                USIMM_EFSTRUCTURE_TRANSPARENT);
        /* coverity[overrun-buffer-val] */
        USIMM_VsimWriteableFileUpdate(pstMsg->usFileID, pstMsg->aucContent);
    }

    USIMM_SetFileCnf(pstMsg->ulSenderPid,
                     pstMsg->ulSendPara,
                     VOS_OK,
                     &stCnfInfo);/*�ظ����½��,Ĭ�ϰ��ճɹ��ظ�*/

    return VOS_OK;
}



VOS_UINT32 USIMM_GetVsimFile(USIMM_GETFILE_REQ_STRU  *pstMsg)
{
    VOS_UINT32                  ulFileNum = 0;
    USIMM_GETCNF_INFO_STRU      stCnfInfo;

    VOS_MemSet(&stCnfInfo, 0, sizeof(stCnfInfo));

    stCnfInfo.ucRecordNum      = pstMsg->ucRecordNum;
    stCnfInfo.usEfId           = pstMsg->usFileID;

    if(VOS_OK == USIMM_PoolFindFile(pstMsg->usFileID, &ulFileNum, USIMM_UNLIMIT_APP))
    {
        stCnfInfo.usDataLen    = gstUSIMMPOOL.astpoolRecord[ulFileNum].usLen;
        stCnfInfo.usEfLen      = gstUSIMMPOOL.astpoolRecord[ulFileNum].usLen;
        stCnfInfo.pucEf        = gstUSIMMPOOL.astpoolRecord[ulFileNum].pucContent;
        stCnfInfo.ucTotalNum   = 1;

        USIMM_INFO_LOG("USIMM_GetVSimFile: Get File Success from Usimm Pool");

        USIMM_GetFileCnf(pstMsg->ulSenderPid, pstMsg->ulSendPara, USIMM_SW_OK, &stCnfInfo);
    }
    else
    {
        stCnfInfo.usDataLen    = VOS_NULL;
        stCnfInfo.usEfLen      = VOS_NULL;
        stCnfInfo.pucEf        = VOS_NULL_PTR;

        USIMM_INFO_LOG("USIMM_GetVSimFile: Get File Failed from Usimm Pool");

        USIMM_GetFileCnf(pstMsg->ulSenderPid, pstMsg->ulSendPara, USIMM_SW_NOFILE_FOUND, &stCnfInfo);
    }
    return VOS_OK;
}


VOS_VOID USIMM_VsimConfidentialDataVerifyErrHandle(VOS_VOID)
{
    NVIM_VSIM_HVSDH_NV_STRU             stDhNv = {0};
    VSIM_ALGORITHM_TYPE_ENUM_UINT32     enAlgorithm = {0};

    USIMM_ClearVsimGlobal();

    /* �����˽Կ */
    if (NV_OK != NV_Read(en_NV_Item_VSIM_HVSDH_INFO, &stDhNv, sizeof(stDhNv)))
    {
        USIMM_WARNING_LOG("USIMM_VsimConfidentialDataVerify: Get HVSDT Info failed.");
    }

    enAlgorithm = stDhNv.enAlgorithm;

    VOS_MemSet(&stDhNv, 0, sizeof(NVIM_VSIM_HVSDH_NV_STRU));

    stDhNv.enAlgorithm = enAlgorithm;

    if (NV_OK != NV_Write(en_NV_Item_VSIM_HVSDH_INFO, &stDhNv, sizeof(stDhNv)))
    {
        USIMM_WARNING_LOG("USIMM_VsimConfidentialDataVerify: Clean HVSDT Info failed.");
    }

    USIMM_VsimReDhNegotiateInd();

    return;
}
VOS_UINT32 USIMM_VsimBase16Decode(VOS_CHAR *pcSrc, VOS_UINT32 ulSrcLen, VOS_UINT8 *pucDst, VOS_UINT32 *pulDstLen)
{
    VOS_UINT32                          i;
    VOS_UINT32                          j = 0;
    VOS_UINT32                          ulLen = 0;
    VOS_CHAR                            acTemp[2];

    for (i = 0; i < ulSrcLen; i++)
    {
        if ((pcSrc[i] >= '0') && (pcSrc[i] <= '9'))            /*ת�������ַ�*/
        {
            acTemp[j] = pcSrc[i] - '0';

            j++;
        }
        else if((pcSrc[i] >= 'a')&&(pcSrc[i+j] <= 'f'))     /*ת��Сд�ַ�*/
        {
            acTemp[j] = (pcSrc[i] - 'a') + 10;              /*�ַ��������ϲ�ֵ�ָ�*/

            j++;
        }
        else if((pcSrc[i] >= 'A')&&(pcSrc[i] <= 'F'))       /*ת����д�ַ�*/
        {
            acTemp[j] = (pcSrc[i] - 'A') + 10;

            j++;
        }
        else                                                /*����ת�����������ַ�*/
        {
            continue;
        }

        if (2 == j)                                         /*�Ѿ��չ������ַ�*/
        {
            pucDst[ulLen] = (VOS_UINT8)(((VOS_UINT8)acTemp[0]<<4)+acTemp[1]);

            ulLen++;

            j = 0;
        }
    }

    if (1 == j)                                              /*ת�����ַ���Ϊ����*/
    {
        return VOS_ERR;
    }

    *pulDstLen = ulLen;

    return VOS_OK;

}
VOS_VOID USIMM_VsimBase16Encode(VOS_UINT8 *pucSrc, VOS_UINT8 *pucDst, VOS_UINT32 ulLen)
{
    VOS_UINT8   aucNibble[2];
    VOS_UINT32  i;
    VOS_UINT32  j;

    for (i = 0; i < ulLen; i++)
    {
        aucNibble[0] = (pucSrc[i] & 0xF0) >> 4;
        aucNibble[1] = pucSrc[i] & 0x0F;
        for (j = 0; j < 2; j++)
        {
            if (aucNibble[j] < 10)
            {
                aucNibble[j] += 0x30;
            }
            else
            {
                if (aucNibble[j] < 16)
                {
                    aucNibble[j] = aucNibble[j] - 10 + 'A';
                }
            }

            *pucDst++ = aucNibble[j];
        }
    }

    return;
}



VOS_UINT32 USIMM_VsimConfidentialDataVerify(VOS_VOID)
{
    VOS_UINT32                          ulImsiLen;
    VOS_UINT32                          ulSimkeyLen;
    VOS_INT32                           lResult;
    VOS_UINT32                          ulRecordNum;
    VOS_INT                             lDataLen;
    VOS_INT                             lHashLen;
    VOS_UINT8                          *pucImsi;
    VOS_UINT8                           aucData[USIMM_EF_IMSI_LEN * 2 + USIMM_VSIM_SECUR_MAX_LEN * 4];
    VOS_UINT8                           aucHashData[USIMM_VSIM_HASH_LEN];
    VOS_UINT8                           aucKey[VSIM_DH_AGREE_KEY] = {0};
    VOS_UINT8                           aucCipher[USIMM_VSIM_SIM_KEY_HASH_LEN/2];
    VOS_UINT8                           aucSimkey[USIMM_VSIM_SIM_KEY_HASH_LEN/2];
    NVIM_VSIM_HVSDH_NV_STRU             stNVDHKey;
    DH_KEY                              stDHPara;

    VOS_MemSet(&stDHPara, 0, sizeof(stDHPara));

    /* ��ѯ�ļ�λ�ã����ﲻ��ֱ�ӵ���USIMM_GetCachedFile������״̬��ȫ�ֱ���δ���� */
    if (VOS_ERR == USIMM_PoolFindFile(EFIMSI, &ulRecordNum, USIMM_UNLIMIT_APP))
    {
        USIMM_ERROR_LOG("USIMM_VsimConfidentialDataVerify: File Could not Found");

        return VOS_ERR;
    }

    if (VOS_NULL_PTR == gstUSIMMPOOL.astpoolRecord[ulRecordNum].pucContent)
    {
        USIMM_ERROR_LOG("USIMM_VsimConfidentialDataVerify: File Content is Empty");

        return VOS_ERR;
    }

    ulImsiLen = gstUSIMMPOOL.astpoolRecord[ulRecordNum].usLen;

    pucImsi   = gstUSIMMPOOL.astpoolRecord[ulRecordNum].pucContent;

    USIMM_VsimBase16Encode(pucImsi, aucData, ulImsiLen);

    VOS_MemCpy(aucData + ulImsiLen * 2,
               g_stUSIMMVSimAuthInfo.stBase16Ki.aucData,
               g_stUSIMMVSimAuthInfo.stBase16Ki.ulLen);

    VOS_MemCpy(aucData + ulImsiLen * 2 + g_stUSIMMVSimAuthInfo.stBase16Ki.ulLen,
               g_stUSIMMVSimAuthInfo.stBase16Opc.aucData,
               g_stUSIMMVSimAuthInfo.stBase16Opc.ulLen);

    lDataLen = (VOS_INT)(ulImsiLen * 2 + g_stUSIMMVSimAuthInfo.stBase16Ki.ulLen + g_stUSIMMVSimAuthInfo.stBase16Opc.ulLen);

    /* ��IMSI+KI+OPC�ĳ��Ⱥ��������ݼ���HASH */
    lHashLen = USIMM_VSIM_HASH_LEN;

    lResult  = CRYPTO_HASH((VOS_CHAR *)aucData,
                           lDataLen,
                           CRYPTO_ALGORITHM_SHA256,
                           (VOS_CHAR *)aucHashData,
                           &lHashLen);

    if ((VOS_OK != lResult)
      ||(USIMM_VSIM_HASH_LEN != lHashLen))
    {
        USIMM_ERROR_LOG("USIMM_VsimConfidentialDataVerify: CRYPTO_HASH Failed");

        return VOS_ERR;
    }

    if (NV_OK != NV_Read(en_NV_Item_VSIM_HVSDH_INFO, &stNVDHKey, sizeof(NVIM_VSIM_HVSDH_NV_STRU)))
    {
        USIMM_ERROR_LOG("USIMM_VsimConfidentialDataVerify: NV Read Key is Failed");

        return VOS_ERR;
    }


    VOS_MemCpy(stDHPara.privateValue, stNVDHKey.stCPrivateKey.aucKey, VSIM_DH_PRIVATE_KEY);

    stDHPara.priVallen = VSIM_DH_PRIVATE_KEY;

    DH_FillFixParams(&stDHPara);

    if (VOS_OK != DH_ComputeAgreedKey(aucKey, stNVDHKey.stSPublicKey.aucKey, &stDHPara))
    {
        USIMM_ERROR_LOG("USIMM_VsimConfidentialDataVerify: DH_ComputeAgreedKey is Failed");

        return VOS_ERR;
    }

    AESEncrypt(aucHashData, lHashLen, aucKey, sizeof(aucKey), aucCipher, USIMM_VSIM_SIM_KEY_HASH_LEN/2);

    if (VOS_OK != USIMM_VsimBase16Decode((VOS_CHAR *)g_stUSIMMVSimAuthInfo.aucSimKeyHash, USIMM_VSIM_SIM_KEY_HASH_LEN, aucSimkey, &ulSimkeyLen))
    {
        USIMM_ERROR_LOG("USIMM_VsimConfidentialDataVerify: USIMM_VsimBase16Decode Failed");

        return VOS_ERR;
    }

    if (VOS_OK == VOS_MemCmp(aucSimkey, aucCipher, USIMM_VSIM_SIM_KEY_HASH_LEN/2))
    {
        return VOS_OK;
    }

    return VOS_ERR;
}

VOS_UINT32 USIMM_AuthenVirtulUsim(USIMM_AUTH_REQ_STRU *pstMsg)
{
    VOS_UINT8                           ucResult;
    VOS_UINT8                           aucxRes[USIMM_AUTH_XRES_LEN+1]  = {0};    /*����1Byte+����*/
    VOS_UINT8                           aucIK[USIMM_AUTH_IK_LEN+1]      = {0};    /*����1Byte+����*/
    VOS_UINT8                           aucCK[USIMM_AUTH_CK_LEN+1]      = {0};    /*����1Byte+����*/
    VOS_UINT8                           aucGsmKC[USIMM_AUTH_KC_LEN+1]   = {0};    /*����1Byte+����*/
    VOS_UINT8                           aucKi[USIMM_AUTH_KI_LEN]        = {0};
    VOS_UINT8                           aucOpc[USIMM_AUTH_OPC_LEN]      = {0};
    USIMM_TELECOM_AUTH_INFO_STRU        stCnfInfo;
    VOS_UINT8                           ucOffset;

    VOS_MemSet(&stCnfInfo, 0, sizeof(stCnfInfo));

    /* ����ȫ�ֱ����е�Ki Opc */
    if(VOS_OK != USIMM_VsimGetRealKiOpc(aucKi, aucOpc))
    {
        stCnfInfo.enResult = USIMM_AUTH_UMTS_OTHER_FAILURE;

        USIMM_TelecomAuthCnf(pstMsg, &stCnfInfo);

        return VOS_ERR;
    }

    VOS_MemCpy(Rand, &pstMsg->aucData[1], pstMsg->aucData[0]);

    ucOffset = pstMsg->aucData[0] + 1;

    VOS_MemCpy(Autn, &pstMsg->aucData[ucOffset + 1], pstMsg->aucData[ucOffset]);

    if (USIMM_3G_AUTH == pstMsg->enAuthType)
    {
        ucResult = Milenage(aucKi, aucOpc, &aucxRes[1], &aucCK[1], &aucIK[1], &aucGsmKC[1]);

        if(AUTH_OK != ucResult) /*Mac�����ʱ��ŷ���ʧ��*/
        {
            stCnfInfo.enResult = USIMM_AUTH_MAC_FAILURE;

            USIMM_TelecomAuthCnf(pstMsg, &stCnfInfo);

            return VOS_ERR;
        }
        else
        {
            /*���ص����ݳ����ǹ̶��� */
            aucxRes[0]  = USIMM_AUTH_XRES_LEN;
            aucIK[0]    = USIMM_AUTH_IK_LEN;
            aucCK[0]    = USIMM_AUTH_CK_LEN;
            aucGsmKC[0] = USIMM_AUTH_KC_LEN;

            stCnfInfo.pucAuthRes    = aucxRes;
            stCnfInfo.pucIK         = aucIK;
            stCnfInfo.pucCK         = aucCK;
            stCnfInfo.pucGsmKC      = aucGsmKC;
            stCnfInfo.pucAuts       = VOS_NULL_PTR;

            stCnfInfo.enResult      = USIMM_AUTH_UMTS_SUCCESS;

            USIMM_TelecomAuthCnf(pstMsg, &stCnfInfo);

            return VOS_OK;
        }
    }
    else
    {
        Milenage2G(aucKi, aucOpc, &aucxRes[1], &aucGsmKC[1]);

        aucxRes[0]  = USIMM_AUTH_RES_LEN;
        aucGsmKC[0] = USIMM_AUTH_KC_LEN;

        stCnfInfo.pucAuthRes        = aucxRes;
        stCnfInfo.pucIK             = VOS_NULL_PTR;
        stCnfInfo.pucCK             = VOS_NULL_PTR;
        stCnfInfo.pucGsmKC          = aucGsmKC;
        stCnfInfo.pucAuts           = VOS_NULL_PTR;

        stCnfInfo.enResult          = USIMM_AUTH_GSM_SUCCESS;

        USIMM_TelecomAuthCnf(pstMsg, &stCnfInfo);

        return VOS_OK;
    }
}
VOS_UINT32 USIMM_AuthenVirtulSim(USIMM_AUTH_REQ_STRU *pstMsg)
{
    VOS_UINT8                           aucOutPut[USIMM_AUTH_RES_LEN+USIMM_AUTH_KC_LEN] = {0};    /*RES+KC*/
    VOS_UINT8                           aucxRes[USIMM_AUTH_RES_LEN+1]                   = {0};    /*����1Byte+����*/
    VOS_UINT8                           aucGsmKC[USIMM_AUTH_KC_LEN+1]                   = {0};    /*����1Byte+����*/
    VOS_UINT8                           aucKi[USIMM_AUTH_KI_LEN];
    VOS_UINT8                           aucOpc[USIMM_AUTH_OPC_LEN];                               /*��ʹ�ã�������Ҫ����XML������*/
    USIMM_TELECOM_AUTH_INFO_STRU        stCnfInfo;

    VOS_MemSet(&stCnfInfo, 0, sizeof(stCnfInfo));

    /* ����ȫ�ֱ����е�Ki Opc */
    if(VOS_OK != USIMM_VsimGetRealKiOpc(aucKi, aucOpc))
    {
        stCnfInfo.enResult = USIMM_AUTH_GSM_OTHER_FAILURE;

        USIMM_TelecomAuthCnf(pstMsg, &stCnfInfo);

        return VOS_ERR;
    }

    if(USIMM_VSIM_COMPV1 == g_stUSIMMVSimAuthInfo.enAuthType)        /*comp 128 v1*/
    {
        RUNA3A8_V1(aucKi, &pstMsg->aucData[1], aucOutPut);
    }
    else if(USIMM_VSIM_COMPV2 == g_stUSIMMVSimAuthInfo.enAuthType)   /*comp 128 v2*/
    {
        /* coverity[uninit_use_in_call] */
        RUNA3A8_V2(aucKi, &pstMsg->aucData[1], aucOutPut);
    }
    else        /*comp 128 v3*/
    {
        /* coverity[uninit_use_in_call] */
        RUNA3A8_V3(aucKi, &pstMsg->aucData[1], aucOutPut);
    }

    aucxRes[0]  = USIMM_AUTH_RES_LEN;

    VOS_MemCpy(&aucxRes[1], aucOutPut, USIMM_AUTH_RES_LEN);

    aucGsmKC[0] = USIMM_AUTH_KC_LEN;

    VOS_MemCpy(&aucGsmKC[1], &aucOutPut[USIMM_AUTH_RES_LEN], USIMM_AUTH_KC_LEN);

    stCnfInfo.pucAuthRes        = aucxRes;
    stCnfInfo.pucIK             = VOS_NULL_PTR;
    stCnfInfo.pucCK             = VOS_NULL_PTR;
    stCnfInfo.pucGsmKC          = aucGsmKC;
    stCnfInfo.pucAuts           = VOS_NULL_PTR;

    stCnfInfo.enResult          = USIMM_AUTH_GSM_SUCCESS;

    USIMM_TelecomAuthCnf(pstMsg, &stCnfInfo);

    return VOS_OK;
}


VOS_UINT32 USIMM_DeactiveVsim(USIMM_MsgBlock *pMsg)
{
    USIMM_NORMAL_LOG("USIMM_DeactiveVSim: Deactive Card");

    USIMM_ClearVsimGlobal();

    return VOS_OK;
}


VOS_UINT32 USIMM_VsimPoolSearchFile(VOS_UINT16 usFileId, VOS_UINT32 *pulData)
{
    VOS_UINT32 i;

    if(USIMM_POOL_NOINTIAL == gstUSIMMPOOL.enPoolStatus)
    {
        return VOS_ERR;
    }

    if(VOS_NULL == gstUSIMMPOOL.ucNowLen)  /*��ǰ����Ϊ��*/
    {
        if(VOS_NULL == usFileId)       /*��ѯ��λ��*/
        {
            *pulData = 0;

            return VOS_OK;
        }

        return VOS_ERR;
    }

    for(i=0; i<POOLMAXNUM; i++)
    {
        if(gstUSIMMPOOL.astpoolRecord[i].usFId == usFileId)                     /*�ҵ����ļ�*/
        {
            *pulData = i;                                                       /*�����ļ����ڵ�Ԫ��*/

            return VOS_OK;
        }
    }

    return VOS_ERR;
}



VOS_UINT32 USIMM_CheckVsimFileInPool(VOS_VOID)
{
    VOS_UINT32      i;
    VOS_UINT32      ulIndex;
    VOS_UINT32      ulResult;

    ulResult = VOS_OK;

    for(i=0; i<ARRAYSIZE(g_astUSIMMVSimFileInfo); i++)
    {
        if (USIMM_VSIM_IS_FILE_ATTR_VALID(i))
        {
            ulResult = USIMM_VsimPoolSearchFile(g_astUSIMMVSimFileInfo[i].usFileID, &ulIndex);
        }

        if(VOS_OK != ulResult)
        {
            USIMM_ERROR_LOG("USIMM_CheckVSIMFileInPoll: the File is not in POOL!");

            return ulResult;
        }
    }

    if((USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType) /*USIM������߱�KI,OPC, ��Ȩ����Ŀǰ��֧�ֱ�׼��ʽ*/
        &&((VOS_NULL == g_stUSIMMVSimAuthInfo.ucKILen)||(VOS_NULL == g_stUSIMMVSimAuthInfo.ucOpcLen))
        &&(USIMM_VSIM_MILENAGE != g_stUSIMMVSimAuthInfo.enAuthType))
    {
        USIMM_ERROR_LOG("USIMM_CheckVSIMFileInPoll: USIM Auth Para is Error!");

        return VOS_ERR;
    }

    if((USIMM_CARD_SIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)/*SIM������߱�KI����Ȩ����Ϊ��׼*/
        &&(VOS_NULL == g_stUSIMMVSimAuthInfo.ucKILen)
        &&((USIMM_VSIM_AUTH_NULL == g_stUSIMMVSimAuthInfo.enAuthType)||(USIMM_VSIM_MILENAGE == g_stUSIMMVSimAuthInfo.enAuthType)))
    {
        USIMM_ERROR_LOG("USIMM_CheckVSIMFileInPoll: SIM Auth Para is Error!");

        return VOS_ERR;
    }

    return VOS_OK;
}




VOS_UINT32 USIMM_AddVsimFileToPool(VOS_CHAR *pucFileStr, VOS_UINT32 ulStrLen, VOS_CHAR *pcValue, VOS_UINT32 ulValueLen)
{
    VOS_UINT32          i;
    VOS_UINT32          ulFileIndex;
    VOS_UINT8           *pucContent;
    VOS_UINT32          ulContentLen;

    if((VOS_NULL_PTR == pcValue)||(VOS_NULL == ulValueLen))
    {
        USIMM_WARNING_LOG("USIMM_AddVSIMFileToPool: Para is NULL!");

        return VOS_OK;      /*�����ڵ��������ļ�*/
    }

    for(i=0; i<ARRAYSIZE(g_astUSIMMVSimFileInfo); i++)
    {
        if(VOS_OK == VOS_MemCmp(g_astUSIMMVSimFileInfo[i].pcVSIMName, pucFileStr, ulStrLen))
        {
            break;
        }
    }

    if(i >= ARRAYSIZE(g_astUSIMMVSimFileInfo))
    {
        USIMM_WARNING_LOG("USIMM_AddVSIMFileToPoll: the File Name is not support!");

        return VOS_OK;
    }

    pucContent = (VOS_UINT8*)VOS_MemAlloc(WUEPS_PID_USIM, DYNAMIC_MEM_PT, ulValueLen);

    if(VOS_NULL_PTR == pucContent)
    {
        USIMM_ERROR_LOG("USIMM_AddVSIMFileToPoll: VOS_MemAlloc is NULL!");

        return VOS_ERR;
    }

    /*���������ַ�*/
    if(VOS_OK != OAM_XML_GetIntArray(pcValue, ulValueLen, pucContent, &ulContentLen))
    {
        USIMM_ERROR_LOG("USIMM_AddVSIMFileToPoll: OAM_XML_GetIntArray is Failed!");

        VOS_MemFree(WUEPS_PID_USIM, pucContent);

        return VOS_OK;  /*������󣬼���������������ݣ�����ɱ�ѡ�ļ���������״̬*/
    }

    if(VOS_OK == USIMM_VsimPoolSearchFile(g_astUSIMMVSimFileInfo[i].usFileID, &ulFileIndex))
    {
        USIMM_ERROR_LOG("USIMM_AddVSIMFileToPoll: File is Already Exist!");

        VOS_MemFree(WUEPS_PID_USIM, pucContent);

        return VOS_OK;  /*�ظ�����ͱ���֮ǰ��ֵ����*/
    }

    /*����ʧ����Ϊ��POOL�Ѿ�����*/
    if(VOS_OK != USIMM_PoolInsertOneFile(g_astUSIMMVSimFileInfo[i].usFileID, (VOS_UINT16)ulContentLen, pucContent, USIMM_UNLIMIT_APP, USIMM_EFSTRUCTURE_TRANSPARENT))
    {
        USIMM_ERROR_LOG("USIMM_AddVSIMFileToPoll: USIMM_PoolInsertOneFile Error"); /*������󣬼���������������ݣ�����ɱ�ѡ�ļ���������״̬*/
    }

    VOS_MemFree(WUEPS_PID_USIM, pucContent);

    return VOS_OK;
}


VOS_UINT32 USIMM_AddVsimAuthPara(VOS_CHAR *pucFileStr, VOS_UINT32 ulFileLen, VOS_CHAR *pcValue, VOS_UINT32 ulValueLen)
{
    VOS_UINT32      ulContentLen;
    VOS_CHAR        *pcContent;
    VOS_UINT8       aucAuthPara[USIMM_VSIM_SIM_KEY_HASH_LEN];
    VOS_UINT32      ulAuthParaLen = 0;
    VOS_UINT32      ulResult;

    if((VOS_NULL_PTR == pcValue)||(VOS_NULL == ulValueLen))
    {
        USIMM_ERROR_LOG("USIMM_AddVsimAuthPara: Para is NULL!");
        return VOS_ERR;
    }

    pcContent = (VOS_CHAR*)VOS_MemAlloc(WUEPS_PID_USIM, DYNAMIC_MEM_PT, ulValueLen);

    if(VOS_NULL_PTR == pcContent)
    {
        USIMM_ERROR_LOG("USIMM_AddVsimAuthPara: VOS_MemAlloc is NULL!");
        return VOS_ERR;
    }

    /*���������ַ�*/
    OAM_XML_FilterStrInvalidChar(pcValue, ulValueLen, pcContent, &ulContentLen);

    if(VOS_NULL == ulContentLen)
    {
        USIMM_ERROR_LOG("USIMM_AddVsimAuthPara: OAM_XML_GetStrContent return 0!");

        VOS_MemFree(WUEPS_PID_USIM, pcContent);

        return VOS_ERR;
    }

    if((USIMM_VSIM_SECUR_MAX_LEN*2) < ulContentLen) /*���Base16���볤�ȣ����ܳ�����ǰȫ�ֱ��������ֵ*/
    {
        USIMM_ERROR_LOG("USIMM_AddVsimAuthPara: Base16Decode Result is too long!");

        VOS_MemFree(WUEPS_PID_USIM, pcContent);

        return VOS_ERR;
    }

    ulResult = OAM_XML_GetIntArray(pcContent, ulContentLen, aucAuthPara, &ulAuthParaLen);

    if((VOS_ERR == ulResult) || (0 == ulAuthParaLen) || (USIMM_VSIM_SECUR_MAX_LEN < ulAuthParaLen))  /*���ת����Ľ��*/
    {
        USIMM_ERROR_LOG("USIMM_AddVsimAuthPara: Base16Decode is Failed!");

        VOS_MemFree(WUEPS_PID_USIM, pcContent);

        return VOS_ERR;
    }

    /* ����ת��������ݣ�ת��ǰ������ҲҪ�������� */
    if (VOS_OK == VOS_MemCmp(USIMM_VSIM_KI_STR, pucFileStr, ulFileLen))
    {
        g_stUSIMMVSimAuthInfo.ucKILen = (VOS_UINT8)ulAuthParaLen;

        VOS_MemCpy(g_stUSIMMVSimAuthInfo.aucKi, aucAuthPara, ulAuthParaLen);

        g_stUSIMMVSimAuthInfo.stBase16Ki.ulLen = ulContentLen;

        VOS_MemCpy(g_stUSIMMVSimAuthInfo.stBase16Ki.aucData, pcContent, ulContentLen);
    }
    else if(VOS_OK == VOS_MemCmp(USIMM_VSIM_OPC_STR, pucFileStr, ulFileLen))
    {
        g_stUSIMMVSimAuthInfo.ucOpcLen = (VOS_UINT8)ulAuthParaLen;

        VOS_MemCpy(g_stUSIMMVSimAuthInfo.aucOpc, aucAuthPara, ulAuthParaLen);

        g_stUSIMMVSimAuthInfo.stBase16Opc.ulLen = ulContentLen;

        VOS_MemCpy(g_stUSIMMVSimAuthInfo.stBase16Opc.aucData, pcContent, ulContentLen);
    }
    else
    {
        VOS_MemCpy(g_stUSIMMVSimAuthInfo.aucSimKeyHash, aucAuthPara, USIMM_VSIM_SIM_KEY_HASH_LEN);
    }

    VOS_MemFree(WUEPS_PID_USIM, pcContent);

    return VOS_OK;
}


VOS_UINT32 USIMM_AddVsimVerifyHashPara(VOS_CHAR *pucFileStr, VOS_UINT32 ulFileLen, VOS_CHAR *pcValue, VOS_UINT32 ulValueLen)
{
    VOS_UINT32      ulContentLen;
    VOS_CHAR        *pcContent;

    if((VOS_NULL_PTR == pcValue)||(VOS_NULL == ulValueLen))
    {
        USIMM_ERROR_LOG("USIMM_AddVsimVerifyHashPara: Para is NULL!");
        return VOS_ERR;
    }

    pcContent = (VOS_CHAR*)VOS_MemAlloc(WUEPS_PID_USIM, DYNAMIC_MEM_PT, ulValueLen);

    if(VOS_NULL_PTR == pcContent)
    {
        USIMM_ERROR_LOG("USIMM_AddVsimVerifyHashPara: VOS_MemAlloc is NULL!");
        return VOS_ERR;
    }

    /*���������ַ�*/
    OAM_XML_FilterStrInvalidChar(pcValue, ulValueLen, pcContent, &ulContentLen);

    if(VOS_NULL == ulContentLen)
    {
        USIMM_ERROR_LOG("USIMM_AddVsimVerifyHashPara: OAM_XML_GetStrContent return 0!");

        VOS_MemFree(WUEPS_PID_USIM, pcContent);

        return VOS_ERR;
    }

    if(USIMM_VSIM_SIM_KEY_HASH_LEN < ulContentLen) /*���Base16���볤�ȣ����ܳ�����ǰȫ�ֱ��������ֵ*/
    {
        USIMM_ERROR_LOG("USIMM_AddVsimVerifyHashPara: Base16Decode Result is too long!");

        VOS_MemFree(WUEPS_PID_USIM, pcContent);

        return VOS_ERR;
    }

    VOS_MemCpy(g_stUSIMMVSimAuthInfo.aucSimKeyHash, pcContent, ulContentLen);

    VOS_MemFree(WUEPS_PID_USIM, pcContent);

    return VOS_OK;
}




VOS_UINT32 USIMM_DecodeVsimEf(OAM_XML_NODE_STRU *pstXmlNode)
{
    VOS_CHAR                    *pcTemp;
    VOS_UINT32                  ulStrLen;
    VOS_UINT32                  ulResult;
    OAM_XML_NODE_STRU           *pstTmpNode;
    OAM_XML_NODE_ATTRIBUTE_STRU *pstAttr;

    if(POOLMAXNUM < OAM_XML_GetChildCount(pstXmlNode))  /*�ļ�������������أ�Ŀǰ���澯*/
    {
        USIMM_WARNING_LOG("USIMM_AddVSIMFileToPoll: The EF Num is more than Pool Space!");
    }

    pstTmpNode = pstXmlNode->pstFirstChild;

    if(VOS_NULL_PTR == pstTmpNode)  /*��ǰ���ӽڵ㣬�ļ�����ʧ��*/
    {
        USIMM_WARNING_LOG("USIMM_AddVSIMFileToPoll: The EF Num is NULL!");

        return VOS_ERR;
    }

    while (VOS_NULL_PTR != pstTmpNode)
    {
        if(VOS_NULL_PTR == pstTmpNode->pstFirstAttrib)                 /*��Ҫ�����ڵ����Ժͽڵ�ֵ�����ļ�*/
        {
            USIMM_WARNING_LOG("USIMM_AddVSIMFileToPoll: The EF Name is NULL!");

            pstTmpNode = pstTmpNode->pstNexBrother; /*ָ����һ���ڵ�*/

            continue;     /*�սڵ��������*/
        }

        pstAttr = OAM_XML_SearchAttNodeByName(pstTmpNode->pstFirstAttrib, USIMM_VSIM_NAME_STR);

        if(VOS_NULL_PTR == pstAttr)
        {
            USIMM_WARNING_LOG("USIMM_AddVSIMFileToPoll: The XML item NAME Attr is NULL!");

            pstTmpNode = pstTmpNode->pstNexBrother; /*ָ����һ���ڵ�*/

            continue;     /*�սڵ��������*/
        }

        /*�����ļ����Ƶ��ڴ�*/
        pcTemp = (VOS_CHAR*)VOS_MemAlloc(WUEPS_PID_USIM, DYNAMIC_MEM_PT, pstAttr->ulValueLength);

        if(VOS_NULL_PTR == pcTemp)
        {
            USIMM_ERROR_LOG("USIMM_VSIMDecodeCardType: VOS_MemAlloc is NULL!");

            return VOS_ERR;
        }

        /*�����ļ������������ַ�*/
        OAM_XML_GetStrContent(pstAttr->pcAttribValue, pstAttr->ulValueLength, pcTemp, &ulStrLen);

        /*�жϵ�ǰ�ļ�����*/
/*lint -e960 -e418*/
        if((VOS_OK == VOS_MemCmp(USIMM_VSIM_KI_STR, pcTemp, ulStrLen))
            ||(VOS_OK == VOS_MemCmp(USIMM_VSIM_OPC_STR, pcTemp, ulStrLen)))
/*lint +e960 +e418*/
        {
            ulResult = USIMM_AddVsimAuthPara(pcTemp, ulStrLen, pstTmpNode->pcNodeValue, pstTmpNode->ulValueLength);
        }
        else if (VOS_OK == VOS_MemCmp(USIMM_VSIM_VERIFY_HASH_STR, pcTemp, ulStrLen))
        {
            ulResult = USIMM_AddVsimVerifyHashPara(pcTemp, ulStrLen, pstTmpNode->pcNodeValue, pstTmpNode->ulValueLength);
        }
        else
        {
            ulResult = USIMM_AddVsimFileToPool(pcTemp, ulStrLen, pstTmpNode->pcNodeValue, pstTmpNode->ulValueLength);
        }

        VOS_MemFree(WUEPS_PID_USIM, pcTemp);    /*�ͷ��ļ����Ƶ��ڴ�*/

        if(VOS_OK != ulResult)
        {
            USIMM_ERROR_LOG("USIMM_VSIMDecodeCardType: Decode XML File Node is Failed!");
        }

        pstTmpNode = pstTmpNode->pstNexBrother; /*ָ����һ���ڵ�*/
    }

    return VOS_OK;
}


VOS_VOID USIMM_VsimDecodeCardType(VOS_CHAR                  *pucStr,
                                            VOS_UINT32                  ulStrLen,
                                            USIMM_CARD_TYPE_ENUM_UINT32 *pulCardType)
{
    VOS_CHAR                    *pcTemp;
    VOS_UINT32                  ulDataLen;

    *pulCardType = USIMM_CARD_NOCARD;   /*Ĭ�Ϸ����޿�*/

    if(VOS_NULL == ulStrLen)
    {
        USIMM_ERROR_LOG("USIMM_VSIMDecodeCardType: Card Type is NULL!");

        return;
    }

    /*�������ڴ�ſ����͵��ڴ�*/
    pcTemp = (VOS_CHAR*)VOS_MemAlloc(WUEPS_PID_USIM, DYNAMIC_MEM_PT, ulStrLen);

    if(VOS_NULL_PTR == pcTemp)
    {
        USIMM_ERROR_LOG("USIMM_VSIMDecodeCardType: VOS_MemAlloc is NULL!");

        return;
    }

    /*���������ַ�*/
    OAM_XML_GetStrContent(pucStr, ulStrLen, pcTemp, &ulDataLen);

    if(VOS_NULL == ulDataLen)
    {
        USIMM_ERROR_LOG("USIMM_VSIMDecodeCardType: Card Type Content is NULL!");

        VOS_MemFree(WUEPS_PID_USIM, pcTemp);

        return;
    }

/*lint -e418*/
    if(VOS_OK == VOS_MemCmp(USIMM_VSIM_USIM_STR, pcTemp, ulDataLen))
    {
        *pulCardType = USIMM_CARD_USIM;
    }
    else if(VOS_OK == VOS_MemCmp(USIMM_VSIM_SIM_STR, pcTemp, ulDataLen))
/*lint +e418*/
    {
        *pulCardType = USIMM_CARD_SIM;
    }
    else
    {
        USIMM_ERROR_LOG("USIMM_VSIMDecodeCardType: Card Type is not support!");
    }

    VOS_MemFree(WUEPS_PID_USIM, pcTemp);    /*�ͷ����ڴ�ſ����͵��ڴ�*/

    return;
}


VOS_VOID USIMM_VsimDecodeAuthType(VOS_CHAR                  *pucStr,
                                            VOS_UINT32                  ulStrLen,
                                            USIMM_VSIM_AUTH_ENUM_UINT32*pulAuthType)
{
    VOS_CHAR                    *pcTemp;
    VOS_UINT32                  ulDataLen;

    *pulAuthType = USIMM_VSIM_AUTH_NULL;

    if(VOS_NULL == ulStrLen)
    {
        USIMM_ERROR_LOG("USIMM_VSIMDecodeAuthType: Auth Type is NULL!");

        return;
    }

    pcTemp = (VOS_CHAR*)VOS_MemAlloc(WUEPS_PID_USIM, DYNAMIC_MEM_PT, ulStrLen);

    if(VOS_NULL_PTR == pcTemp)
    {
        USIMM_ERROR_LOG("USIMM_VSIMDecodeAuthType: VOS_MemAlloc is NULL!");

        return;
    }

    OAM_XML_GetStrContent(pucStr, ulStrLen, pcTemp, &ulDataLen);

    if(VOS_NULL == ulDataLen)
    {
        USIMM_ERROR_LOG("USIMM_VSIMDecodeAuthType: Auth Type Content is NULL!");

        VOS_MemFree(WUEPS_PID_USIM, pcTemp);

        return;
    }

/*lint -e418*/
    if(VOS_OK == VOS_MemCmp(USIMM_VSIM_AUTH_3G_STR, pcTemp, ulDataLen))
    {
        *pulAuthType = USIMM_VSIM_MILENAGE;
    }
    else if(VOS_OK == VOS_MemCmp(USIMM_VSIM_AUTH_2GV1_STR, pcTemp, ulDataLen))
    {
        *pulAuthType = USIMM_VSIM_COMPV1;
    }
    else if(VOS_OK == VOS_MemCmp(USIMM_VSIM_AUTH_2GV2_STR, pcTemp, ulDataLen))
    {
        *pulAuthType = USIMM_VSIM_COMPV2;
    }
    else if(VOS_OK == VOS_MemCmp(USIMM_VSIM_AUTH_2GV3_STR, pcTemp, ulDataLen))
/*lint +e418*/
    {
        *pulAuthType = USIMM_VSIM_COMPV3;
    }
    else
    {
        USIMM_ERROR_LOG("USIMM_VSIMDecodeAuthType: Auth Type is not support!");
    }

    VOS_MemFree(WUEPS_PID_USIM, pcTemp);

    return;
}



VOS_UINT32 USIMM_DecodeVsimRoot(OAM_XML_NODE_STRU             *pstXmlNode,
                                        USIMM_CARD_TYPE_ENUM_UINT32     *pulCardType,
                                        USIMM_VSIM_AUTH_ENUM_UINT8      *pucAuthType)
{
    OAM_XML_NODE_ATTRIBUTE_STRU *pstAttr;
    USIMM_CARD_TYPE_ENUM_UINT32 ulCardType = USIMM_CARD_NOCARD;
    USIMM_VSIM_AUTH_ENUM_UINT32 ulAuthType = USIMM_VSIM_AUTH_NULL;

    if(VOS_NULL_PTR == pstXmlNode->pstFirstAttrib)  /*�����������Խڵ���������͵�����*/
    {
        USIMM_ERROR_LOG("USIMM_DecodeVsimRoot: Xml Root FirstAttrib is NULL!");

        return VOS_ERR;
    }

    pstAttr = OAM_XML_SearchAttNodeByName(pstXmlNode->pstFirstAttrib, USIMM_VSIM_TYPE_STR);

    if(VOS_NULL_PTR == pstAttr)
    {
        USIMM_ERROR_LOG("USIMM_DecodeVsimRoot: Search TYPE is NULL!");

        return VOS_ERR;
    }

    USIMM_VsimDecodeCardType(pstAttr->pcAttribValue, pstAttr->ulValueLength, &ulCardType);

    pstAttr = OAM_XML_SearchAttNodeByName(pstXmlNode->pstFirstAttrib, USIMM_VSIM_AUTH_STR);

    if(VOS_NULL_PTR == pstAttr)
    {
        USIMM_ERROR_LOG("USIMM_DecodeVsimRoot: Search ALGORITHM is NULL!");

        return VOS_ERR;
    }

    USIMM_VsimDecodeAuthType(pstAttr->pcAttribValue, pstAttr->ulValueLength, &ulAuthType);

    /*�ڵ���Ϣ���߱����ߴ���*/
    if((USIMM_CARD_NOCARD == ulCardType)||(USIMM_VSIM_AUTH_NULL == ulAuthType))
    {
        USIMM_ERROR_LOG("USIMM_DecodeVsimRoot: Card Type or Auth Type is NULL!");

        return VOS_ERR;
    }

    if(VOS_NULL_PTR != pulCardType)
    {
        *pulCardType = ulCardType;
    }

    if(VOS_NULL_PTR != pucAuthType)
    {
        *pucAuthType = (VOS_UINT8)ulAuthType;
    }

    return VOS_OK;
}


VOS_UINT32 USIMM_DecodeVsimFile(VOS_UINT8 *pucData)
{
    OAM_XML_CTX_STRU                    stXmlCtx;
    OAM_XML_NODE_STRU                   *pstXmlNode;

    /*����xml�ļ�*/
    VOS_MemSet(&stXmlCtx, 0x00, sizeof(stXmlCtx));

    /* ����XML������������,����XML�ṹ�� */
    if (OAM_XML_RESULT_SUCCEED != OAM_XML_InitXMLCtx(&stXmlCtx))
    {
        USIMM_ERROR_LOG("USIMM_DecodeVSIMFile: Xml initialization failed!");

        return VOS_ERR;
    }

    pstXmlNode = OAM_XML_BuildXMLTree( (VOS_CHAR*)pucData, &stXmlCtx);

    if (VOS_NULL_PTR == pstXmlNode)
    {
        USIMM_ERROR_LOG("USIMM_DecodeVSIMFile: Build xml tree failed!");

        OAM_XML_ClearXMLCtx(&stXmlCtx);

        return VOS_ERR;
    }

    /*�����ļ����ڵ�*/
    if(VOS_OK != USIMM_DecodeVsimRoot(pstXmlNode, &gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType, &g_stUSIMMVSimAuthInfo.enAuthType))
    {
        USIMM_ERROR_LOG("USIMM_DecodeVSIMFile: USIMM_DecodeVsimRoot failed!");

        OAM_XML_ClearXMLCtx(&stXmlCtx);

        return VOS_ERR;
    }

    /*���ݲ���POOL*/
    if(VOS_OK != USIMM_DecodeVsimEf(pstXmlNode))
    {
        USIMM_ERROR_LOG("USIMM_DecodeVSIMFile: USIMM_AddVSIMFileToPoll failed!");

        OAM_XML_ClearXMLCtx(&stXmlCtx);

        return VOS_ERR;
    }

    OAM_XML_ClearXMLCtx(&stXmlCtx);

    return VOS_OK;
}


VOS_UINT8* USIMM_ReadVsimFile(VOS_UINT32 ulReadPID)
{
    FILE                                *pXmlFile;
    VOS_UINT8                           *pucXMLData;
    VOS_INT32                           lLength;
    VOS_UINT32                          ulResult;

    pXmlFile = USIMM_File_Open(g_pcUSIMMVSimXmlFilePath, USIMM_FILE_OPEN_MODE_R);

    if(VOS_NULL_PTR == pXmlFile)
    {
        USIMM_ERROR_LOG("USIMM_ReadVsimFile: Open vsim.xml Failed");

        return VOS_NULL_PTR;
    }

    /*��ȡvSIM���ļ���С*/
    USIMM_File_Seek(pXmlFile, 0, USIMM_FILE_SEEK_END);

    lLength = USIMM_File_Tell(pXmlFile);

    if(lLength <= 0)
    {
        USIMM_ERROR_LOG("USIMM_ReadVsimFile: vsim.xml is empty");

        USIMM_File_Close(pXmlFile);

        return VOS_NULL_PTR;
    }

    USIMM_File_Seek(pXmlFile, 0, USIMM_FILE_SEEK_SET);

    /*�����ڴ�,������һ���ַ���Ϊ�������*/
    pucXMLData = (VOS_UINT8 *)VOS_MemAlloc(ulReadPID, DYNAMIC_MEM_PT, (VOS_UINT32)(lLength+1));

    if(VOS_NULL_PTR == pucXMLData)
    {
        USIMM_ERROR_LOG("USIMM_ReadVsimFile: VOS_MemAlloc Failed");

        USIMM_File_Close(pXmlFile);

        return VOS_NULL_PTR;
    }

    ulResult = (VOS_UINT32)USIMM_File_Read(pucXMLData, (VOS_UINT32)lLength, VSIM_FILE_READ_COUNT, pXmlFile);

    USIMM_File_Close(pXmlFile);

    if(VSIM_FILE_READ_COUNT != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_ReadVsimFile: NV_File_Read Failed");

        VOS_MemFree(ulReadPID, pucXMLData);

        return VOS_NULL_PTR;
    }

    pucXMLData[lLength] = '\0';    /*���ý�����*/

    return pucXMLData;
}
VOS_UINT32 USIMM_InitVsimCard(USIMM_MsgBlock *pMsg)
{
    VOS_UINT8                           *pucXMLData;
    VOS_UINT32                          ulResult;

    USIMM_InitGlobalVarOnReset(); /*��ʼ������ȫ�ֱ�������*/

    VOS_MemSet(&g_stUSIMMVSimAuthInfo, 0, sizeof(g_stUSIMMVSimAuthInfo));

    gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType     = USIMM_CARD_NOCARD;

    pucXMLData = USIMM_ReadVsimFile(WUEPS_PID_USIM);

    if(VOS_NULL_PTR == pucXMLData)
    {
        USIMM_ERROR_LOG("USIMM_InitVSIMCard: USIMM_ReadVsimFile Failed");

        return VOS_ERR;
    }

    ulResult = USIMM_DecodeVsimFile(pucXMLData);

    VOS_MemFree(WUEPS_PID_USIM, pucXMLData);

    if (VOS_OK != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_InitVSIMCard: USIMM_DecodeVSIMFile Failed");

        USIMM_ClearVsimGlobal();

        return VOS_ERR;
    }

    /* ��IMSI+KI+OPC����У�飬��У�鲻���ϱ��޿���Ҫ��DH��Э�� */
    if (VOS_OK != USIMM_VsimConfidentialDataVerify())
    {
        USIMM_ERROR_LOG("USIMM_InitVSIMCard: USIMM_VsimConfidentialDataVerify Failed");

        USIMM_VsimConfidentialDataVerifyErrHandle();

        return VOS_ERR;
    }

    /*����ѡ�ļ��Ƿ���*/
    if (VOS_OK != USIMM_CheckVsimFileInPool())
    {
        USIMM_ERROR_LOG("USIMM_InitVSIMCard: USIMM_CheckVSIMFileInPoll Failed");

        USIMM_ClearVsimGlobal();

        return VOS_ERR;
    }

    USIMM_VsimWriteableFileInit();

    USIMM_InitVsimGlobal();

    return VOS_OK;
}
VOS_VOID USIMM_VsimWriteableFileInit(VOS_VOID)
{
    VSIM_CARD_LOCI_FILE_NV_STRU         stLociFile;
    VSIM_CARD_PSLOCI_FILE_NV_STRU       stPsLociFile;
    VSIM_CARD_FPLMN_FILE_NV_STRU        stFplmnFile;
    VOS_UINT16                          usPsLociFileId;
    VOS_UINT8                           aucFplmn[VSIM_EF_FPLMN_LEN];
    VOS_UINT8                           aucLoci[VSIM_EF_LOCI_LEN];
    VOS_UINT8                           aucPsLoci[VSIM_EF_PS_LOCI_LEN];

    if (USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
    {
        usPsLociFileId = EFPSLOCI;
    }
    else
    {
        usPsLociFileId = EFLOCIGPRS;
    }

    if (NV_OK == NV_Read(en_NV_Item_VSIM_Fplmn_Info, &stFplmnFile, sizeof(VSIM_CARD_FPLMN_FILE_NV_STRU)))
    {
        /* NV��fplmn��Чʱ��ʹ��XML�ļ��е�fplmn������ʹ��NV���е�fplmn����Ϊ��ʱNV�е�fplmn�Ѿ�������
           XML�ļ��е�fplmn */
        VOS_MemSet(aucFplmn, (VOS_CHAR)0xFF, VSIM_EF_FPLMN_LEN);

        if(VOS_NULL != VOS_MemCmp(aucFplmn, stFplmnFile.aucFplmn, VSIM_EF_FPLMN_LEN))
        {
            if (VOS_OK == USIMM_PoolDelOneFile(EFFPLMN, USIMM_UNLIMIT_APP))
            {
                USIMM_PoolInsertOneFile(EFFPLMN,
                                        sizeof(stFplmnFile.aucFplmn),
                                        stFplmnFile.aucFplmn,
                                        USIMM_UNLIMIT_APP,
                                        USIMM_EFSTRUCTURE_TRANSPARENT);

            }
        }
    }

    if (NV_OK == NV_Read(en_NV_Item_VSIM_Loci_Info, &stLociFile, sizeof(VSIM_CARD_LOCI_FILE_NV_STRU)))
    {
        /* NV��Loci��Чʱ��ʹ��XML�ļ��е�EfLoci������ʹ��NV���е�Loci����Ϊ�״δ�Ӳ��������NV�����ֵ����Ч�ģ�
            ��Ҫʹ��XML�е�ֵ���Լӿ������ٶ� */
        VOS_MemSet(aucLoci, (VOS_CHAR)0xFF, VSIM_EF_LOCI_LEN);

        if(VOS_NULL != VOS_MemCmp(aucLoci, stLociFile.aucEfloci, VSIM_EF_LOCI_LEN))
        {
            if (VOS_OK == USIMM_PoolDelOneFile(EFLOCI, USIMM_UNLIMIT_APP))
            {
                USIMM_PoolInsertOneFile(EFLOCI,
                                        sizeof(stLociFile.aucEfloci),
                                        stLociFile.aucEfloci,
                                        USIMM_UNLIMIT_APP,
                                        USIMM_EFSTRUCTURE_TRANSPARENT);
            }
        }
    }

    if (NV_OK == NV_Read(en_NV_Item_VSIM_PsLoci_Info, &stPsLociFile, sizeof(VSIM_CARD_PSLOCI_FILE_NV_STRU)))
    {
        /* NV��PsLoci��Чʱ��ʹ��XML�ļ��е�EfPsLoci/EfLociGprs������ʹ��NV���е�PsLoci����Ϊ�״δ�Ӳ��������
            NV�����ֵ����Ч�ģ���Ҫʹ��XML�е�ֵ���Լӿ������ٶ� */
        VOS_MemSet(aucPsLoci, (VOS_CHAR)0xFF, VSIM_EF_PS_LOCI_LEN);

        if(VOS_NULL != VOS_MemCmp(aucPsLoci, stPsLociFile.aucPsEfloci, VSIM_EF_PS_LOCI_LEN))
        {
            if (VOS_OK == USIMM_PoolDelOneFile(usPsLociFileId, USIMM_UNLIMIT_APP))
            {
                USIMM_PoolInsertOneFile(usPsLociFileId,
                                        sizeof(stPsLociFile.aucPsEfloci),
                                        stPsLociFile.aucPsEfloci,
                                        USIMM_UNLIMIT_APP,
                                        USIMM_EFSTRUCTURE_TRANSPARENT);
            }
        }
    }

    return;
}


VOS_VOID USIMM_VsimWriteableFileUpdate(VOS_UINT16 usFileId, VOS_UINT8 *pucFileContent)
{

    if (EFFPLMN == usFileId)
    {
        if (VOS_OK != NV_Write(en_NV_Item_VSIM_Fplmn_Info, pucFileContent, sizeof(VSIM_CARD_FPLMN_FILE_NV_STRU)))
        {
            USIMM_WARNING_LOG("USIMM_VsimWriteableFileUpdate: write Fplmn Info failed.");
        }
    }
    else if (EFLOCI == usFileId)
    {
        if (VOS_OK != NV_Write(en_NV_Item_VSIM_Loci_Info, pucFileContent, sizeof(VSIM_CARD_LOCI_FILE_NV_STRU)))
        {
            USIMM_WARNING_LOG("USIMM_VsimWriteableFileUpdate: write Loic Info failed.");
        }
    }
    else if ((EFPSLOCI == usFileId) || (EFLOCIGPRS == usFileId))
    {
        if (VOS_OK != NV_Write(en_NV_Item_VSIM_PsLoci_Info, pucFileContent, sizeof(VSIM_CARD_PSLOCI_FILE_NV_STRU)))
        {
            USIMM_WARNING_LOG("USIMM_VsimWriteableFileUpdate: write PsLoic Info failed.");
        }
    }
    else
    {
        USIMM_WARNING_LOG("USIMM_VsimWriteableFileUpdate: Wrong file ID.");
    }

    return;
}


#endif  /* (FEATURE_VSIM == FEATURE_ON)*/


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */




