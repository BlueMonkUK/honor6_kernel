
/*lint --e{958,718,746,767}*/

#include "usimmapdu.h"
#include "usimmdl.h"
#include "usimmt1dl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/

#define    THIS_FILE_ID        PS_FILE_ID_USIMM_APDUMODULE_C



VOS_UINT32 USIMM_CheckSW(USIMM_APDU_ST *pstUSIMMAPDU)
{
    USIMM_SWCHECK_ST                   *pstSWCheck;
    VOS_UINT32                          ulLen;
    VOS_UINT32                          i;
    VOS_UINT8                           ucSW1;
    VOS_UINT8                           ucSW2;
    USIMM_SWCHECK_ENUM_UINT16           enResult = USIMM_SW_OTHER_ERROR;

    ucSW1 = pstUSIMMAPDU->ucSW1;
    ucSW2 = pstUSIMMAPDU->ucSW2;

    if (USIMM_SIM_CLA == pstUSIMMAPDU->aucAPDU[CLA])  /*���ݿ������ж�*/
    {
        pstSWCheck = gausSimSWCheck;

        ulLen      = USIMM_SIMSW_MAXNUMBER;
    }
    else
    {
        pstSWCheck = gausUsimSWCheck;

        ulLen      = USIMM_USIMSW_MAXNUMBER;
    }

    for(i=0; i<ulLen; i++)
    {
        if(pstSWCheck[i].ucSW1 != ucSW1)        /*���ж�SW1ƥ��*/
        {
            continue;
        }

        if((0xC0 == (ucSW2&0xF0))&&(0x63 == pstSWCheck[i].ucSW1))
        {
            enResult = pstSWCheck[i].enResult;

            break;
        }

        if((0xFF == pstSWCheck[i].ucSW2)||(ucSW2 == pstSWCheck[i].ucSW2))/*����Ҫ�ж�SW2����SW2ƥ��*/
        {
            enResult = pstSWCheck[i].enResult;

            break;
        }
    }

    if (USIMM_SW_OK_WITH_SAT == enResult)    /*SAT��Ҫ���⴦��*/
    {
        USIMM_INFO_LOG("USIMM_CheckSW: There is SAT Data Need Fetch");

        gstUSIMMBaseInfo.enSATState = USIMM_SAT_STATE_NEEDFETCH;

        if (0x00 == ucSW2)           /*����ǰ��ȡ���������256*/
        {
            gstUSIMMBaseInfo.usSATLen = 0x100;
        }
        else
        {
            gstUSIMMBaseInfo.usSATLen = ucSW2;
        }

        enResult = USIMM_SW_OK;
    }

    if (VOS_TRUE == g_stUsimmFeatureCfg.unCfg.stFeatureCfg.ulAtt_flg)
    {
        if ((0x6F == ucSW1)&&(0x00 == ucSW2))
        {
            g_ulATTSpecErrSWCnt++;
        }
        else
        {
            g_ulATTSpecErrSWCnt = 0;
        }

        if (g_ulATTSpecErrSWCnt >= 3)                                 /*��������3���ϱ��޿�*/
        {
            gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType        = USIMM_CARD_NOCARD;        /*��ǰ״̬Ϊ�޿�*/
            gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService     = USIMM_CARD_SERVIC_ABSENT; /*��ǰ״̬Ϊ�޿�*/

            gastUSIMMCardAppInfo[USIMM_UICC_USIM].enStateChange     = USIMM_CARD_STATE_CHANGED;
        }
    }

    return (VOS_UINT32)enResult;
}



VOS_UINT32 USIMM_SelectFileByChannelID_APDU(VOS_UINT8 ucChannelID, USIMM_APDU_ST *pstApduInfo)
{
    VOS_UINT32                          ulResult;

    pstApduInfo->aucAPDU[CLA]   = pstApduInfo->aucAPDU[CLA]|ucChannelID; /*���APDU����ͷ*/

    pstApduInfo->aucAPDU[INS]   = CMD_INS_SELECT;

    pstApduInfo->ulLcValue      = pstApduInfo->aucAPDU[P3];                  /*���������������*/

    if(USIMM_SELECT_RETURN_FCP_TEMPLATE == pstApduInfo->aucAPDU[P2])
    {
        /* ���ֳ�����Ϊcase4 */
        pstApduInfo->ulLeValue      = USIMM_LE_MAX_LEN;
    }
    else
    {
        /* case3 */
        pstApduInfo->ulLeValue      = VOS_NULL;
    }

    ulResult = USIMM_DLHandle(pstApduInfo);/*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_SelectFile_APDU:USIMM_DLHandle error");

        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_SelectFile_APDU(USIMM_APDU_ST   *pstApduInfo)
{
    return USIMM_SelectFileByChannelID_APDU(0, pstApduInfo);
}


VOS_UINT32 USIMM_ReselectFileAPDU(USIMM_APDU_ST   *pstApduInfo)
{
    VOS_UINT32                          ulIndex = 0;
    VOS_UINT32                          ulResult = 0;

    for (ulIndex = 0; ulIndex < USIMM_SELECT_MAX_TIME; ulIndex++)
    {
        /* ����FCP�����������������ô�FCP�ķ�ʽ���� */
        if ((USIMM_SELECT_NO_DATA_RETURNED == pstApduInfo->aucAPDU[P2])
            && ((USIMM_SELECT_MAX_TIME - 1) == ulIndex))
        {
            pstApduInfo->aucAPDU[P2] = USIMM_SELECT_RETURN_FCP_TEMPLATE;
        }

        ulResult = USIMM_SelectFile_APDU(pstApduInfo);

        if (VOS_OK != ulResult)
        {
            USIMM_ERROR_LOG("USIMM_ReselectFileAPDU: USIMM_SelectFile_APDU fail.");

            return USIMM_SW_SENDCMD_ERROR;
        }

        ulResult = USIMM_CheckSW(pstApduInfo);

        /* ״̬��ΪWarning��Technical Problem���ٴν���ѡ�ļ����� */
        if ((USIMM_SW_WARNING == ulResult) || (USIMM_SW_TECH_ERROR == ulResult))
        {
            continue;
        }

        break;
    }

    return ulResult;
}


VOS_UINT32 USIMM_StatusByChannelID_APDU(VOS_UINT8 ucChannelID, VOS_UINT8 ucP1,VOS_UINT8 ucP2,VOS_UINT8 ucP3)
{
    VOS_UINT32 ulResult;

    gstUSIMMAPDU.aucAPDU[CLA] = gstUSIMMBaseInfo.ucCLA|ucChannelID|0x80; /*���APDU����ͷ*/

    gstUSIMMAPDU.aucAPDU[INS] = CMD_INS_STATUS;

    gstUSIMMAPDU.aucAPDU[P1]  = ucP1;

    gstUSIMMAPDU.aucAPDU[P2]  = ucP2;

    gstUSIMMAPDU.aucAPDU[P3]  = ucP3;

    gstUSIMMAPDU.ulLcValue = 0x00;                              /*���������������*/

    if((ucP3 == 0x00)&&(ucP2 != 0x0C))
    {
        gstUSIMMAPDU.ulLeValue = 0x100;
    }
    else
    {
        gstUSIMMAPDU.ulLeValue = ucP3;
    }


    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_STATUS_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 USIMM_Status_APDU(VOS_UINT8 ucP1,VOS_UINT8 ucP2,VOS_UINT8 ucP3)
{
    return USIMM_StatusByChannelID_APDU(0, ucP1, ucP2, ucP3);
}


VOS_UINT32 USIMM_ReadBinary_APDU(VOS_UINT16 usOffset, VOS_UINT8 ucP3)
{
    VOS_UINT32 ulResult;

    gstUSIMMAPDU.aucAPDU[CLA] = gstUSIMMBaseInfo.ucCLA; /*���APDU����ͷ*/

    gstUSIMMAPDU.aucAPDU[INS] = CMD_INS_READ_BINARY;

    gstUSIMMAPDU.aucAPDU[P1]  = (VOS_UINT8)((usOffset>>8)&0xFF);

    gstUSIMMAPDU.aucAPDU[P2]  = (VOS_UINT8)(usOffset&0xFF);

    gstUSIMMAPDU.aucAPDU[P3]  = ucP3;

    gstUSIMMAPDU.ulLcValue    = 0x00;           /*���������������*/

    gstUSIMMAPDU.ulLeValue    = ucP3;

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_Status_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 USIMM_UpdateBinary_APDU(VOS_UINT16 usOffset, VOS_UINT8 ucP3, VOS_UINT8* pucData)
{
    VOS_UINT32 ulResult;

    /*���APDU����ͷ*/
    gstUSIMMAPDU.aucAPDU[CLA] = gstUSIMMBaseInfo.ucCLA;

    gstUSIMMAPDU.aucAPDU[INS] = CMD_INS_UPDATE_BINARY;

    gstUSIMMAPDU.aucAPDU[P1]  = (VOS_UINT8)((usOffset>>0x08)&0xFF);

    gstUSIMMAPDU.aucAPDU[P2]  = (VOS_UINT8)(usOffset&0xFF);

    gstUSIMMAPDU.aucAPDU[P3]  = ucP3;

    VOS_MemCpy(gstUSIMMAPDU.aucSendBuf, pucData, ucP3);/*���APDU��������*/

    gstUSIMMAPDU.ulLcValue   = ucP3;                  /*���������������*/

    gstUSIMMAPDU.ulLeValue   = 0x00;

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);/*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_UpdateBinary_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_ReadRecord_APDU(VOS_UINT8 ucRecordNum, VOS_UINT8 ucMode, VOS_UINT8 ucLen)
{
    VOS_UINT32 ulResult;

    gstUSIMMAPDU.aucAPDU[CLA] = gstUSIMMBaseInfo.ucCLA; /*���APDU����ͷ*/

    gstUSIMMAPDU.aucAPDU[INS] = CMD_INS_READ_RECORD;

    gstUSIMMAPDU.aucAPDU[P1]  = ucRecordNum;

    gstUSIMMAPDU.aucAPDU[P2]  = ucMode;

    gstUSIMMAPDU.aucAPDU[P3]  = ucLen;

    gstUSIMMAPDU.ulLcValue    = 0x00;           /*���������������*/

    gstUSIMMAPDU.ulLeValue    = ucLen;

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_ReadRecord_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 USIMM_UpdateRecord_APDU(VOS_UINT8 ucRecordNum, VOS_UINT8 ucMode, VOS_UINT8 ucLen, VOS_UINT8* pucData)
{
    VOS_UINT32 ulResult;

    gstUSIMMAPDU.aucAPDU[CLA] = gstUSIMMBaseInfo.ucCLA; /*���APDU����ͷ*/

    gstUSIMMAPDU.aucAPDU[INS] = CMD_INS_UPDATE_RECORD;

    gstUSIMMAPDU.aucAPDU[P1]  = ucRecordNum;

    gstUSIMMAPDU.aucAPDU[P2]  = ucMode;

    gstUSIMMAPDU.aucAPDU[P3]  = ucLen;

    VOS_MemCpy(gstUSIMMAPDU.aucSendBuf, pucData, ucLen);/*���APDU��������*/

    gstUSIMMAPDU.ulLcValue = ucLen;                              /*���������������*/

    gstUSIMMAPDU.ulLeValue = 0x00;

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_UpdateRecord_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_Search_APDU(VOS_UINT8 ucP1, VOS_UINT8 ucP2, VOS_UINT8 ucP3, VOS_UINT8* pucData)
{
    VOS_UINT32 ulResult;

    gstUSIMMAPDU.aucAPDU[CLA] = gstUSIMMBaseInfo.ucCLA; /*���APDU����ͷ*/

    gstUSIMMAPDU.aucAPDU[INS] = CMD_INS_SEARCH_RECORD;

    gstUSIMMAPDU.aucAPDU[P1]  = ucP1;

    gstUSIMMAPDU.aucAPDU[P2]  = ucP2;

    gstUSIMMAPDU.aucAPDU[P3]  = ucP3;

    VOS_MemCpy(gstUSIMMAPDU.aucSendBuf, pucData, ucP3);/*���APDU��������*/

    gstUSIMMAPDU.ulLcValue = ucP3;                              /*���������������*/

    gstUSIMMAPDU.ulLeValue = 0x00;

    /* ���ֳ�����Ϊcase4 */
    gstUSIMMAPDU.ulLeValue = USIMM_LE_MAX_LEN;

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_Search_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 USIMM_Increase_APDU(VOS_UINT8 ucLen, VOS_UINT8* pucData)
{
    VOS_UINT32 ulResult;

    gstUSIMMAPDU.aucAPDU[CLA] = gstUSIMMBaseInfo.ucCLA; /*���APDU����ͷ*/

    gstUSIMMAPDU.aucAPDU[INS] = CMD_INS_INCREASE;

    gstUSIMMAPDU.aucAPDU[P1]  = 0x00;

    gstUSIMMAPDU.aucAPDU[P2]  = 0x00;

    gstUSIMMAPDU.aucAPDU[P3]  = ucLen;

    VOS_MemCpy(gstUSIMMAPDU.aucSendBuf, pucData, ucLen);/*���APDU��������*/

    gstUSIMMAPDU.ulLcValue = ucLen;                              /*���������������*/

    /* ���ֳ�����Ϊcase4 */
    gstUSIMMAPDU.ulLeValue = USIMM_LE_MAX_LEN;

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_Increase_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 USIMM_PINVerify_APDU(VOS_UINT8 ucCmdType,VOS_UINT8 ucPINType, VOS_UINT8* pucPINData)
{
    VOS_UINT32 ulResult;
    VOS_UINT8  ucLen;

    if(VOS_NULL_PTR == pucPINData)  /*��ȡ��ǰ��PINʣ�����*/
    {
        ucLen = 0x00;
    }
    else                                                /*У��PIN*/
    {
        ucLen = 0x08;
    }

    gstUSIMMAPDU.aucAPDU[CLA] = gstUSIMMBaseInfo.ucCLA; /*���APDU����ͷ*/

    gstUSIMMAPDU.aucAPDU[INS] = ucCmdType;

    gstUSIMMAPDU.aucAPDU[P1]  = 0x00;

    gstUSIMMAPDU.aucAPDU[P2]  = ucPINType;

    gstUSIMMAPDU.aucAPDU[P3]  = ucLen;

    if(8 == ucLen)
    {
        VOS_MemCpy(gstUSIMMAPDU.aucSendBuf, pucPINData, 0x08);/*���APDU��������*/
    }

    gstUSIMMAPDU.ulLcValue = ucLen;                              /*���������������*/

    gstUSIMMAPDU.ulLeValue = 0x00;

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_PINVerify_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_ChangePIN_APDU(VOS_UINT8 ucPINType, VOS_UINT8* pucOldPIN, VOS_UINT8* pucNewPIN)
{
    VOS_UINT32 ulResult;

    gstUSIMMAPDU.aucAPDU[CLA] = gstUSIMMBaseInfo.ucCLA; /*���APDU����ͷ*/

    gstUSIMMAPDU.aucAPDU[INS] = CMD_INS_CHANGE_PIN;

    gstUSIMMAPDU.aucAPDU[P1]  = 0x00;

    gstUSIMMAPDU.aucAPDU[P2]  = ucPINType;

    gstUSIMMAPDU.aucAPDU[P3]  = 0x10;

    VOS_MemCpy(&gstUSIMMAPDU.aucSendBuf[0], pucOldPIN, 0x08);/*���APDU��������*/

    VOS_MemCpy(&gstUSIMMAPDU.aucSendBuf[8], pucNewPIN, 0x08);

    gstUSIMMAPDU.ulLcValue = 0x10;                              /*���������������*/

    gstUSIMMAPDU.ulLeValue = 0x00;

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_ChangePIN_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_UnblockPIN_APDU(VOS_UINT8 ucPINType, VOS_UINT8* pucPUKData, VOS_UINT8* pucPINData)
{
    VOS_UINT32 ulResult;
    VOS_UINT8  ucP3;

    if(VOS_NULL_PTR == pucPUKData)
    {
        ucP3 = 0x00;
    }
    else
    {
        ucP3 = 0x10;
    }

    gstUSIMMAPDU.aucAPDU[CLA] = gstUSIMMBaseInfo.ucCLA; /*���APDU����ͷ*/

    gstUSIMMAPDU.aucAPDU[INS] = CMD_INS_UNBLOCK_PIN;

    gstUSIMMAPDU.aucAPDU[P1]  = 0x00;

    gstUSIMMAPDU.aucAPDU[P2]  = ucPINType;

    gstUSIMMAPDU.aucAPDU[P3]  = ucP3;

    if(0x00  != ucP3)
    {
        VOS_MemCpy(&gstUSIMMAPDU.aucSendBuf[0], pucPUKData, 0x08);/*���APDU��������*/

        VOS_MemCpy(&gstUSIMMAPDU.aucSendBuf[8], pucPINData, 0x08);
    }

    gstUSIMMAPDU.ulLcValue = ucP3;                              /*���������������*/

    gstUSIMMAPDU.ulLeValue = 0;

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_UnblockPIN_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_DeactivateFile_APDU(VOS_VOID)
{
    VOS_UINT32 ulResult;

    gstUSIMMAPDU.aucAPDU[CLA] = gstUSIMMBaseInfo.ucCLA; /*���APDU����ͷ*/

    gstUSIMMAPDU.aucAPDU[INS] = CMD_INS_DEACTIVATE_FILE;

    gstUSIMMAPDU.aucAPDU[P1]  = 0x00;

    gstUSIMMAPDU.aucAPDU[P2]  = 0x00;

    gstUSIMMAPDU.aucAPDU[P3]  = 0x00;

    gstUSIMMAPDU.ulLcValue    = 0x00;                /*���������������*/

    gstUSIMMAPDU.ulLeValue    = 0x00;

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_DeactivateFile_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 USIMM_ActivateFile_APDU(VOS_VOID)
{
    VOS_UINT32 ulResult;

    gstUSIMMAPDU.aucAPDU[CLA] = gstUSIMMBaseInfo.ucCLA; /*���APDU����ͷ*/

    gstUSIMMAPDU.aucAPDU[INS] = CMD_INS_ACTIVATE_FILE;

    gstUSIMMAPDU.aucAPDU[P1]  = 0x00;

    gstUSIMMAPDU.aucAPDU[P2]  = 0x00;

    gstUSIMMAPDU.aucAPDU[P3]  = 0x00;

    gstUSIMMAPDU.ulLcValue    = 0x00;           /*���������������*/

    gstUSIMMAPDU.ulLeValue    = 0x00;

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_ActivateFile_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 USIMM_Authentication_APDU(VOS_UINT8 ucMode, VOS_UINT8 *pucData, VOS_UINT32 ulDataLen)
{
    VOS_UINT32                          ulResult;

    /* ������� */
    if (USIMM_APDU_MAXLEN <= ulDataLen)
    {
        USIMM_ERROR_LOG("USIMM_Authentication_APDU: ulDataLen is too big.");

        return VOS_ERR;
    }

    gstUSIMMAPDU.aucAPDU[CLA] = gstUSIMMBaseInfo.ucCLA; /*���APDU����ͷ*/

    gstUSIMMAPDU.aucAPDU[INS] = CMD_INS_AUTHENTICATE;

    gstUSIMMAPDU.aucAPDU[P1]  = 0x00;

    gstUSIMMAPDU.aucAPDU[P2]  = ucMode;

    gstUSIMMAPDU.aucAPDU[P3]  = (VOS_UINT8)ulDataLen;

    VOS_MemCpy(&gstUSIMMAPDU.aucSendBuf[0], pucData, ulDataLen);/*������������*/

    gstUSIMMAPDU.ulLcValue = gstUSIMMAPDU.aucAPDU[P3];      /*���������������*/

    /* ���ֳ�����Ϊcase4 */
    gstUSIMMAPDU.ulLeValue = USIMM_LE_MAX_LEN;

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_Authentication_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_TermimalProfile_APDU(VOS_UINT8 ucLen, VOS_UINT8* pucData)
{
    VOS_UINT32 ulResult;

    gstUSIMMAPDU.aucAPDU[CLA] = gstUSIMMBaseInfo.ucCLA|0x80; /*���APDU����ͷ*/

    gstUSIMMAPDU.aucAPDU[INS] = CMD_INS_TERMINAL_PROFILE;

    gstUSIMMAPDU.aucAPDU[P1]  = 0x00;

    gstUSIMMAPDU.aucAPDU[P2]  = 0x00;

    gstUSIMMAPDU.aucAPDU[P3]  = ucLen;

    VOS_MemCpy(&gstUSIMMAPDU.aucSendBuf[0], pucData, ucLen);/*���APDU��������*/

    gstUSIMMAPDU.ulLcValue = ucLen;                              /*���������������*/

    gstUSIMMAPDU.ulLeValue = 0x00;

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_TermimalProfile_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_Envelope_APDU(VOS_UINT8 ucLen, VOS_UINT8* pucData)
{
    VOS_UINT32 ulResult;

    gstUSIMMAPDU.aucAPDU[CLA] = gstUSIMMBaseInfo.ucCLA|0x80; /*���APDU����ͷ*/

    gstUSIMMAPDU.aucAPDU[INS] = CMD_INS_ENVELOPE;

    gstUSIMMAPDU.aucAPDU[P1]  = 0x00;

    gstUSIMMAPDU.aucAPDU[P2]  = 0x00;

    gstUSIMMAPDU.aucAPDU[P3]  = ucLen;

    VOS_MemCpy(&gstUSIMMAPDU.aucSendBuf[0], pucData, ucLen);/*���APDU��������*/

    gstUSIMMAPDU.ulLcValue = ucLen;                              /*���������������*/

    /* ���ֳ�����Ϊcase4 */
    gstUSIMMAPDU.ulLeValue = USIMM_LE_MAX_LEN;

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_Envelope_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 USIMM_Fetch_APDU(VOS_UINT8 ucLen)
{
    VOS_UINT32 ulResult;

    gstUSIMMAPDU.aucAPDU[CLA] = gstUSIMMBaseInfo.ucCLA|0x80; /*���APDU����ͷ*/

    gstUSIMMAPDU.aucAPDU[INS] = CMD_INS_FETCH;

    gstUSIMMAPDU.aucAPDU[P1]  = 0x00;

    gstUSIMMAPDU.aucAPDU[P2]  = 0x00;

    gstUSIMMAPDU.aucAPDU[P3]  = ucLen;

    gstUSIMMAPDU.ulLcValue = 0x00;                              /*���������������*/

    if(ucLen == 0x00)
    {
        gstUSIMMAPDU.ulLeValue = 0x100;
    }
    else
    {
        gstUSIMMAPDU.ulLeValue = ucLen;
    }

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_Fetch_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}



VOS_UINT32 USIMM_TerminalResponse_APDU(VOS_UINT8 ucLen, VOS_UINT8* pucData)
{
    VOS_UINT32 ulResult;

    gstUSIMMAPDU.aucAPDU[CLA] = gstUSIMMBaseInfo.ucCLA|0x80; /*���APDU����ͷ*/

    gstUSIMMAPDU.aucAPDU[INS] = CMD_INS_TERMINAL_RESPONSE;

    gstUSIMMAPDU.aucAPDU[P1]  = 0x00;

    gstUSIMMAPDU.aucAPDU[P2]  = 0x00;

    gstUSIMMAPDU.aucAPDU[P3]  = ucLen;

    VOS_MemCpy(&gstUSIMMAPDU.aucSendBuf[0], pucData, ucLen);/*���APDU��������*/

    gstUSIMMAPDU.ulLcValue = ucLen;                              /*���������������*/

    gstUSIMMAPDU.ulLeValue = 0x00;

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_TerminalResponse_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_FormatCsimApdu(USIMM_GACCESS_REQ_STRU *pstMsg,
                                            USIMM_APDU_ST          *pstUsimmApdu)
{
    /* ������ݳ���С�����ֽڣ����ǳ���ĳ��� */
    if(pstMsg->usDataLen < T1_CASE1_APDU_LEN)
    {
        /* ��ӡ���� */
        USIMM_ERROR_LOG("USIMM_FormatCsimApdu: data len is less than 4 bytes");

        return VOS_ERR;
    }

    /* CASE1��CASE2����� */
    if ( (T1_CASE1_APDU_LEN == pstMsg->usDataLen)
        || (pstMsg->usDataLen == sizeof(pstUsimmApdu->aucAPDU)))/*��Lc�ֶ�*/
    {
        USIMM_INFO_LOG("USIMM_FormatCsimApdu: The Command have any Lc Data");

        /* copy apdu */
        VOS_MemCpy(pstUsimmApdu->aucAPDU, pstMsg->aucContent, pstMsg->usDataLen);

        pstUsimmApdu->ulLcValue = 0;

        pstUsimmApdu->ulLeValue = pstUsimmApdu->aucAPDU[P3];  /*Le�����ݿ���Ϊ0*/

        return VOS_OK;
    }

    /* ���������Ӧ�Ľṹ�� */
    VOS_MemCpy(pstUsimmApdu->aucAPDU, pstMsg->aucContent, sizeof(pstUsimmApdu->aucAPDU));

    /* �ж�P3�ֽ�ָʾ�Ƿ���ȷ */
    if(pstMsg->usDataLen < (pstUsimmApdu->aucAPDU[P3] + USIMM_APDU_HEADLEN))
    {
        USIMM_ERROR_LOG("USIMM_FormatCsimApdu: LC is wrong");/*��ӡ����*/

        return VOS_ERR;
    }

    VOS_MemCpy(pstUsimmApdu->aucSendBuf, (pstMsg->aucContent + USIMM_APDU_HEADLEN), pstUsimmApdu->aucAPDU[P3]);

    pstUsimmApdu->ulLcValue = pstUsimmApdu->aucAPDU[P3];

    /* ���ݹ��� */
    if(pstMsg->usDataLen > (USIMM_APDU_HEADLEN + pstUsimmApdu->aucAPDU[P3] + 1))
    {
        USIMM_ERROR_LOG("USIMM_FormatCsimApdu: the data len is more");/*��ӡ����*/

        return VOS_ERR;
    }

    /* case4��������һ�ֽڵ�LE�ֶ� */
    if(pstMsg->usDataLen == (USIMM_APDU_HEADLEN + pstUsimmApdu->aucAPDU[P3] + 1))
    {
        pstUsimmApdu->ulLeValue = pstMsg->aucContent[pstMsg->usDataLen - 1];
    }
    else
    {
        /* case3���� */
        pstUsimmApdu->ulLeValue = 0;
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_FormatIsdbApdu(
    USIMM_ISDB_ACCESS_REQ_STRU         *pstMsg,
    USIMM_APDU_ST                      *pstUsimmApdu)
{
    /* ������ݳ���С�����ֽڣ����ǳ���ĳ��� */
    if(pstMsg->usDataLen < T1_CASE1_APDU_LEN)
    {
        /* ��ӡ���� */
        USIMM_ERROR_LOG("USIMM_FormatIsdbApdu: data len is less than 4 bytes");

        return VOS_ERR;
    }

    /* CASE1��CASE2����� */
    if ( (T1_CASE1_APDU_LEN == pstMsg->usDataLen)
        || (pstMsg->usDataLen == sizeof(pstUsimmApdu->aucAPDU)))/*��Lc�ֶ�*/
    {
        USIMM_INFO_LOG("USIMM_FormatIsdbApdu: The Command have any Lc Data");

        /* copy apdu */
        VOS_MemCpy(pstUsimmApdu->aucAPDU, pstMsg->aucContent, pstMsg->usDataLen);

        pstUsimmApdu->ulLcValue = 0;

        pstUsimmApdu->ulLeValue = pstUsimmApdu->aucAPDU[P3];  /*Le�����ݿ���Ϊ0*/

        return VOS_OK;
    }

    /* ���������Ӧ�Ľṹ�� */
    /*lint -e420*/
    VOS_MemCpy(pstUsimmApdu->aucAPDU, pstMsg->aucContent, sizeof(pstUsimmApdu->aucAPDU));
    /*lint +e420*/

    /* �ж�P3�ֽ�ָʾ�Ƿ���ȷ */
    if(pstMsg->usDataLen < (pstUsimmApdu->aucAPDU[P3] + USIMM_APDU_HEADLEN))
    {
        USIMM_ERROR_LOG("USIMM_FormatIsdbApdu: LC is wrong");/*��ӡ����*/

        return VOS_ERR;
    }

    /*lint -e416*/
    VOS_MemCpy(pstUsimmApdu->aucSendBuf, (pstMsg->aucContent + USIMM_APDU_HEADLEN), pstUsimmApdu->aucAPDU[P3]);
    /*lint +e416*/

    pstUsimmApdu->ulLcValue = pstUsimmApdu->aucAPDU[P3];

    /* ���ݹ��� */
    if(pstMsg->usDataLen > (USIMM_APDU_HEADLEN + pstUsimmApdu->aucAPDU[P3] + 1))
    {
        USIMM_ERROR_LOG("USIMM_FormatIsdbApdu: the data len is more");/*��ӡ����*/

        return VOS_ERR;
    }

    /* case4��������һ�ֽڵ�LE�ֶ� */
    if(pstMsg->usDataLen == (USIMM_APDU_HEADLEN + pstUsimmApdu->aucAPDU[P3] + 1))
    {
        pstUsimmApdu->ulLeValue = pstMsg->aucContent[pstMsg->usDataLen - 1];/* [false alarm]:fortify */
    }
    else
    {
        /* case3���� */
        pstUsimmApdu->ulLeValue = 0;
    }

    return VOS_OK;
}


VOS_UINT32 USIMM_FormatStandardApdu(VOS_UINT8 *pucData, VOS_UINT16 usLen, USIMM_APDU_ST *pstUsimmApdu)
{
    /* ��Lc�ֶ� */
    if (usLen <= sizeof(pstUsimmApdu->aucAPDU))
    {
        USIMM_INFO_LOG("USIMM_FormatStandardApdu: The Command have any Lc Data");

        /* ��APDUΪcase1ʱ��Le�ֶ�Ϊ0 */
        pstUsimmApdu->aucAPDU[P3] = 0;

        VOS_MemCpy(pstUsimmApdu->aucAPDU, pucData, usLen);

        pstUsimmApdu->ulLcValue = 0;

        /* Le�����ݿ���Ϊ0 */
        pstUsimmApdu->ulLeValue = pstUsimmApdu->aucAPDU[P3];
    }
    /* ��Lc�ֶ� */
    else if ((usLen > sizeof(pstUsimmApdu->aucAPDU))
             &&(usLen <= (sizeof(pstUsimmApdu->aucAPDU)+USIMM_APDU_MAXLEN)))
    {
        USIMM_INFO_LOG("USIMM_FormatStandardApdu: The Command have Lc Data");

        /* �ж�Lc�ֶ��Ƿ���ȷ */
        if ((sizeof(pstUsimmApdu->aucAPDU) + pucData[P3]) > usLen)/* [false alarm]:*/
        {
            USIMM_ERROR_LOG("USIMM_FormatStandardApdu: Lc is Error");

            return VOS_ERR;
        }

        VOS_MemCpy(pstUsimmApdu->aucAPDU, pucData, sizeof(pstUsimmApdu->aucAPDU));

        /*lint -e416*/
        VOS_MemCpy(pstUsimmApdu->aucSendBuf,
                   pucData+sizeof(pstUsimmApdu->aucAPDU),
                   pstUsimmApdu->aucAPDU[P3]);
        /*lint +e416*/
        pstUsimmApdu->ulLcValue = pstUsimmApdu->aucAPDU[P3];

        /* ���ݹ��� */
        if(usLen > (USIMM_APDU_HEADLEN + pstUsimmApdu->aucAPDU[P3] + 1))
        {
            USIMM_ERROR_LOG("USIMM_FormatCsimApdu: the data len is more");/*��ӡ����*/

            return VOS_ERR;
        }

        if(usLen == (USIMM_APDU_HEADLEN + pstUsimmApdu->aucAPDU[P3] + 1))
        {
            /* case4 */
            pstUsimmApdu->ulLeValue = pucData[usLen - 1];/* [false alarm]:fortify */
        }
        else
        {
            /* case3 */
            pstUsimmApdu->ulLeValue = 0;
        }
    }
    else
    {
        USIMM_ERROR_LOG("USIMM_FormatStandardApdu: The Command Type is Error");

        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_Manage_Channel_APDU(VOS_UINT8 ucP1, VOS_UINT8 ucP2, VOS_UINT8 ucP3)
{
    VOS_UINT32 ulResult;

    /* �ڶ�Ӧ���߼�ͨ���Ϸ� �ڴ��߼�ͨ��ʱ:p2Ϊ0���ر��߼�ͨ��ʱ:p2Ϊ��Ӧ�߼�ͨ���� */
    gstUSIMMAPDU.aucAPDU[CLA] = USIMM_USIM_CLA | ucP2;

    gstUSIMMAPDU.aucAPDU[INS] = CMD_INS_MANAGE_CHANNEL;

    gstUSIMMAPDU.aucAPDU[P1]  = ucP1;

    gstUSIMMAPDU.aucAPDU[P2]  = ucP2;

    gstUSIMMAPDU.aucAPDU[P3]  = ucP3;

    gstUSIMMAPDU.ulLcValue = 0x00;

    gstUSIMMAPDU.ulLeValue = ucP3;

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_Manage_Channel_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_Terminal_Capability_APDU(VOS_UINT8 ucP1, VOS_UINT8 ucP2, USIMM_TERMINAL_CAPABILITY_STRU *pstTerminalCapability)
{
    VOS_UINT32 ulResult;
    VOS_UINT32 ulLen = 0;

    gstUSIMMAPDU.aucAPDU[CLA] = gstUSIMMBaseInfo.ucCLA|0x80; /*���APDU����ͷ*/

    gstUSIMMAPDU.aucAPDU[INS] = CMD_INS_TERMINAL_CAPABILITY;

    gstUSIMMAPDU.aucAPDU[P1]  = ucP1;

    gstUSIMMAPDU.aucAPDU[P2]  = ucP2;

    VOS_MemCpy(gstUSIMMAPDU.aucSendBuf, (VOS_UINT8 *)pstTerminalCapability->aucData,
                           pstTerminalCapability->ulLen);/*���APDU��������*/

    ulLen = pstTerminalCapability->ulLen;

    gstUSIMMAPDU.aucAPDU[P3]  = (VOS_UINT8)ulLen;

    gstUSIMMAPDU.ulLcValue = ulLen;                  /*���������������*/

    gstUSIMMAPDU.ulLeValue = 0x00;

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_STATUS_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_T1SendIFSD_APDU(VOS_UINT8 ucData)
{
    VOS_UINT32      ulResult;

    VOS_MemSet(gstUSIMMAPDU.aucAPDU, 0, USIMM_APDU_HEADLEN);

    VOS_MemSet(gstUSIMMAPDU.aucSendBuf, 0, USIMM_APDU_MAXLEN);

    /* ��Ҫ����һ���ֽڵ�IFS��С */
    gstUSIMMAPDU.ulLcValue = 0x01;

    gstUSIMMAPDU.ulLeValue = 0x00;

    gstUSIMMAPDU.aucSendBuf[0] = ucData;

    /* ��ǵ�ǰҪ����IFS REQ */
    g_bSendIFSReqFlag = VOS_TRUE;

    ulResult = USIMM_DLHandle(&gstUSIMMAPDU);   /*������·�㴦����*/

    g_bSendIFSReqFlag = VOS_FALSE;

    if(USIMM_DL_SUCESS != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_T1SendIFSD_APDU: USIMM_DLHandle is Failed");/*��ӡ����*/

        return VOS_ERR;
    }

    return VOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

