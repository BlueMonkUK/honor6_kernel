
/*lint  --e{537,767,718,746,740,958}*/

#include "usimmbase.h"
#include "DrvInterface.h"
#include "usimmapdu.h"
#include "usimmdl.h"
#include "om.h"
#include "usimmt1dl.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/

#define    THIS_FILE_ID        PS_FILE_ID_USIMM_COMMON_C


#ifdef USIMM_DEBUG_PEINT

VOS_VOID USIMM_PRINT(VOS_UINT32 ulPrintLev,VOS_INT8 *pucString)
{
    switch(ulPrintLev)
    {
        case USIMM_NORMAL:
            vos_printf("\r\nNORMAL:%s\r\n",pucString);
            break;
        case USIMM_INFO:
            vos_printf("\r\nINFO:%s\r\n",pucString);
            break;
        case USIMM_WARNING:
            vos_printf("\r\nWARNING:%s\r\n",pucString);
            break;
        default:
            vos_printf("\r\nERROR:%s\r\n",pucString);
            break;
    }

    return;
}
#endif



VOS_UINT32 USIMM_UpdateLFFile(VOS_UINT8 ucRecordNum, VOS_UINT8 *pucData, VOS_UINT8 ucLen)
{
    VOS_UINT32 ulResult;
    VOS_UINT8  ucP2 = 0x04;    /*ͳһ���վ��Է�ʽ����*/

    ulResult = USIMM_UpdateRecord_APDU(ucRecordNum, ucP2, ucLen, pucData);

    if(VOS_OK != ulResult)/*�жϷ��ͽ��*/
    {
        USIMM_ERROR_LOG("USIMM_UpdateLFFile:USIMM_UpdateRecord_APDU error");

        return USIMM_SW_SENDCMD_ERROR;
    }

    ulResult = USIMM_CheckSW(&gstUSIMMAPDU);

    if(USIMM_SW_OK != ulResult)/*�жϸ��½��*/
    {
        USIMM_WARNING_LOG("USIMM_UpdateLFFile:USIMM_CheckSW error");
    }

    return ulResult;
}
VOS_UINT32 USIMM_UpdateTFFile(VOS_UINT16 usLen, VOS_UINT8 *pucData)
{
    VOS_UINT32 ulResult = VOS_ERR;
    VOS_UINT16 usUpdateLen;
    VOS_UINT8  ucP3 = 0;

    for(usUpdateLen=0; usUpdateLen<usLen;usUpdateLen+=ucP3)
    {
        if(usLen > (0xFF+usUpdateLen))   /*�жϵ�ǰ��ʣ�����ݳ���*/
        {
            ucP3 = 0xFF;                            /*ʣ�����ݴ���255*/
        }
        else
        {
            ucP3 = (VOS_UINT8)(usLen-usUpdateLen);   /*ʣ�����ݿ���һ�θ������*/
        }

        ulResult = USIMM_UpdateBinary_APDU(usUpdateLen, ucP3, pucData+usUpdateLen);

        if(VOS_OK != ulResult)         /*�жϷ��ͽ��*/
        {
            USIMM_ERROR_LOG("USIMM_UpdateTFFile:USIMM_UpdateBinary_APDU error");

            return USIMM_SW_SENDCMD_ERROR;
        }

        ulResult = USIMM_CheckSW(&gstUSIMMAPDU);

        if(USIMM_SW_OK!= ulResult)  /*�жϼ����*/
        {
            USIMM_WARNING_LOG("USIMM_UpdateTFFile:USIMM_CheckSW error");
        }
    }

    return ulResult;
}
VOS_UINT32 USIMM_UpdateCLFFile(VOS_UINT8 *pucData, VOS_UINT8 ucDataLen)
{
    VOS_UINT32 ulResult;

    ulResult = USIMM_UpdateRecord_APDU(0x00, 0x03, ucDataLen,  pucData);

    if(VOS_OK != ulResult)/*�жϷ��ͽ��*/
    {
        USIMM_ERROR_LOG("USIMM_UpdateCLFFile:USIMM_UpdateRecord_APDU error");

        return USIMM_SW_SENDCMD_ERROR;
    }

    ulResult = USIMM_CheckSW(&gstUSIMMAPDU);

    if(USIMM_SW_OK != ulResult)/*�жϼ����*/
    {
        USIMM_WARNING_LOG("USIMM_UpdateCLFFile:USIMM_CheckSW error");
    }

    return ulResult;
}


VOS_UINT32 USIMM_ReadLFFile(USIMM_EFFCP_ST *pstCurEFFcp, VOS_UINT8 ucRecordNum, VOS_UINT8 *pucContent)
{
    VOS_UINT32                          i    = 1;
    VOS_UINT32                          ulTotalNum;
    VOS_UINT32                          ulResult;

    if(USIMM_READ_ALLRECORD == ucRecordNum)
    {
        ulTotalNum = pstCurEFFcp->ucRecordNum;
    }
    else
    {
        i = ucRecordNum;

        ulTotalNum = i;
    }

    for(; i<=ulTotalNum; i++)
    {
        if(VOS_OK != USIMM_ReadRecord_APDU((VOS_UINT8)i, 0x04, pstCurEFFcp->ucRecordLen))/*�жϷ��ͽ��*/
        {
            USIMM_ERROR_LOG("USIMM_ReadLFFile: USIMM_ReadRecord_APDU Error");

            if (VOS_OK != USIMM_ReadRecord_APDU((VOS_UINT8)i, 0x04, pstCurEFFcp->ucRecordLen))
            {
                USIMM_ERROR_LOG("USIMM_ReadLFFile:Resend USIMM_ReadRecord_APDU Error");

                return USIMM_SW_SENDCMD_ERROR;
            }
        }

        ulResult = USIMM_CheckSW(&gstUSIMMAPDU);

        if(USIMM_SW_OK != ulResult)/*�жϼ����*/
        {
            USIMM_WARNING_LOG("USIMM_ReadLFFile: USIMM_CheckSW Error");

            return ulResult;
        }

        VOS_MemCpy(pucContent, gstUSIMMAPDU.aucRecvBuf, pstCurEFFcp->ucRecordLen);

        pucContent += pstCurEFFcp->ucRecordLen;
    }

    return USIMM_SW_OK;
}


VOS_UINT32 USIMM_ReadTFFile(VOS_UINT16 usLen, VOS_UINT8 *pucContent)
{
    VOS_UINT32 ulResult;
    VOS_UINT16 usReadedLen;
    VOS_UINT8  ucP3 = 0;

    for(usReadedLen=0; usReadedLen<usLen; usReadedLen+=ucP3)
    {
        if((usLen-usReadedLen) > 0xFF)      /*�����³���*/
        {
            ucP3 = 0xFF;
        }
        else
        {
            ucP3 = (VOS_UINT8)(usLen-usReadedLen);  /*ʣ����³���*/
        }

        ulResult = USIMM_ReadBinary_APDU(usReadedLen, ucP3);

        if(VOS_OK != ulResult)/*�жϸ��·��ͽ��*/
        {
            USIMM_ERROR_LOG("USIMM_ReadTFFile: USIMM_ReadBinary_APDU Error");

            if (VOS_OK != USIMM_ReadBinary_APDU(usReadedLen, ucP3))
            {
                USIMM_ERROR_LOG("USIMM_ReadTFFile:Resend USIMM_ReadBinary_APDU Error");

                return USIMM_SW_SENDCMD_ERROR;
            }
        }

        ulResult = USIMM_CheckSW(&gstUSIMMAPDU);

        if(USIMM_SW_OK != ulResult)/*�жϼ����*/
        {
            USIMM_WARNING_LOG("USIMM_ReadTFFile: USIMM_CheckSW Error");

            return ulResult;
        }

        VOS_MemCpy(pucContent+usReadedLen, gstUSIMMAPDU.aucRecvBuf, ucP3);
    }

    return USIMM_SW_OK;
}



VOS_UINT32  USIMM_GetTFFile(USIMM_APP_TYPE_ENUM_UINT32 enAppType, VOS_UINT16 usFileId, VOS_UINT16 usReadLen,VOS_UINT8 *pucData)
{
    VOS_UINT32  ulResult;
    VOS_UINT16  usDataLen;

    ulResult = USIMM_SelectFile(enAppType, USIMM_NEED_FCP, 1, &usFileId);

    if(VOS_OK != ulResult)
    {
        return VOS_ERR;
    }

    if(gstUSIMMCurFileInfo.stEFInfo.enFileType != USIMM_EFSTRUCTURE_TRANSPARENT)
    {
        return VOS_ERR;
    }

    if(USIMM_READ_ALL_FILE_CONTENT == usReadLen)
    {
        usDataLen = gstUSIMMCurFileInfo.stEFInfo.usFileLen;
    }
    else
    {
        usDataLen = usReadLen;
    }

    ulResult = USIMM_ReadTFFile(usDataLen, pucData);

    return ulResult;
}


VOS_UINT32  USIMM_WriteTFFile(USIMM_APP_TYPE_ENUM_UINT32 enFileApp, VOS_UINT16 usFileId, VOS_UINT16 usDataLen,VOS_UINT8 *pucData)
{
    VOS_UINT32  ulResult;

    ulResult = USIMM_SelectFile(enFileApp, USIMM_NEED_FCP, 1, &usFileId);

    if(VOS_OK != ulResult)
    {
        return VOS_ERR;
    }

    if(gstUSIMMCurFileInfo.stEFInfo.enFileType != USIMM_EFSTRUCTURE_TRANSPARENT)
    {
        return VOS_ERR;
    }

    return USIMM_UpdateTFFile(usDataLen, pucData);
}


VOS_UINT32 USIMM_STATUSProc(VOS_VOID)
{
    VOS_UINT32                          ulResult;
    VOS_UINT16                          usCurDfID   = gstUSIMMCurFileInfo.usCurDFID;
    VOS_UINT16                          usCurFcpLen = gstUSIMMCurFileInfo.usCurDFFcpLen;

    ulResult = USIMM_Status_APDU(USIMM_STATUS_NO_INDICATION, USIMM_STATUS_FCP_RETURNED, (VOS_UINT8)gstUSIMMCurFileInfo.usCurDFFcpLen);

    if(VOS_OK != ulResult)
    {
        return USIMM_SW_SENDCMD_ERROR;
    }

    ulResult = USIMM_CheckSW(&gstUSIMMAPDU);

    if(USIMM_SW_OK != ulResult)
    {
        USIMM_WARNING_LOG("USIMM_StatusHandle: Command SW is Error");

        /* ֻ����GCF����ʱ���ϸ��жϸ��ִ��󣬷���ֻҪstatus apdu��ȥ������Ӧ����Ϊ����λ */
        if(VOS_TRUE != USIMM_IsTestCard())
        {
            return VOS_OK;
        }

        return ulResult;
    }

    USIMM_INFO_LOG("USIMM_StatusHandle: The Status Command is OK");

    ulResult = USIMM_DecodeDFFcp(gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType, &gstUSIMMAPDU, &gstUSIMMCurFileInfo);

    if(ulResult != VOS_OK)
    {
        /* ������Ҫ��ԭ֮ǰ��ֵ�������´η��ʹ��� */
        gstUSIMMCurFileInfo.usCurDFFcpLen   = usCurFcpLen;

        gstUSIMMCurFileInfo.usCurDFID       = usCurDfID;

        /* ֻ����GCF����ʱ���ϸ��жϸ��ִ��󣬷���ֻҪstatus apdu��ȥ������Ӧ����Ϊ����λ */
        if(VOS_TRUE != USIMM_IsTestCard())
        {
            return VOS_OK;
        }

        return USIMM_SW_SENDCMD_ERROR;
    }

    if (USIMM_CARD_SIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)  /*��ǰΪSIM*/
    {
        USIMM_SaveDFPinInfo();

        gstUSIMMCurFileInfo.usCurDFID = gstUSIMMCurFileInfo.stDFInfo.usDFID;
    }
    else
    {
        if(VOS_OK == VOS_MemCmp(gstUSIMMADFInfo.aucAID, gstUSIMMCurFileInfo.stDFInfo.aucName, gstUSIMMADFInfo.ulAIDLen))
        {
            gstUSIMMCurFileInfo.usCurDFID = ADF;
        }
        else
        {
            gstUSIMMCurFileInfo.usCurDFID = gstUSIMMCurFileInfo.stDFInfo.usDFID;
        }
    }

    if((USIMM_NULL_ID != usCurDfID)&&(usCurDfID != gstUSIMMCurFileInfo.usCurDFID))
    {
        USIMM_ERROR_LOG("USIMM_StatusHandle:The Status Command is not Same,The Card Should be Changed");

        return USIMM_SW_OTHER_ERROR;
    }

    return VOS_OK;
}


VOS_UINT32 USIMM_GetPINRemainTime(VOS_UINT8 ucPINNo)
{
    VOS_UINT32  ulResult;
    VOS_UINT8   ucRemain;

    ulResult = USIMM_PINVerify_APDU(CMD_INS_VERIFY,ucPINNo, VOS_NULL_PTR);

    if(VOS_OK != ulResult)/*�жϽ��*/
    {
        USIMM_ERROR_LOG("USIMM_GetPINRemainTime: USIMM_PINVerify_APDU Error");

        return VOS_ERR;
    }

    if(0x63 == gstUSIMMAPDU.ucSW1)
    {
        ucRemain = gstUSIMMAPDU.ucSW2&0x0F;
    }
    else
    {
        ucRemain = 0;
    }

    if(gstUSIMMADFInfo.ucPIN1RefNum == ucPINNo)
    {
        gstUSIMMADFInfo.stPINInfo.ucPin1RemainTime = ucRemain;
    }
    else if(gstUSIMMADFInfo.ucPIN2RefNum == ucPINNo)
    {
        gstUSIMMADFInfo.stPINInfo.ucPin2RemainTime = ucRemain;
    }
    else
    {
        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_GetPUKRemainTime(VOS_UINT8 ucPINNo)
{
    VOS_UINT32 ulResult;
    VOS_UINT8  ucRemain;

    ulResult = USIMM_UnblockPIN_APDU(ucPINNo, VOS_NULL_PTR,VOS_NULL_PTR);

    if(VOS_OK != ulResult)/*�жϽ��*/
    {
        USIMM_ERROR_LOG("USIMM_GetPUKRemainTime: USIMM_UnblockPIN_APDU Error");

        return VOS_ERR;
    }

    if(0x63 == gstUSIMMAPDU.ucSW1)
    {
        ucRemain = gstUSIMMAPDU.ucSW2&0x0F;
    }
    else
    {
        ucRemain = 0;
    }

    if(gstUSIMMADFInfo.ucPIN1RefNum == ucPINNo)
    {
        gstUSIMMADFInfo.stPINInfo.ucPuk1RemainTime = ucRemain;
    }
    else if(gstUSIMMADFInfo.ucPIN2RefNum == ucPINNo)
    {
        gstUSIMMADFInfo.stPINInfo.ucPuk2RemainTime = ucRemain;
    }
    else
    {
        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_AdjudgeCardState(VOS_VOID)
{
    USIMM_CARD_SERVIC_ENUM_UINT32 enProCardService;

    enProCardService = gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService;    /*���浱ǰ�Ŀ�״̬*/

    /*�ж�����״̬*/
    if(0 == gstUSIMMADFInfo.stPINInfo.ucPin1RemainTime)    /*PIN1ʣ��0��*/
    {
        USIMM_NORMAL_LOG("USIMM_AdjudgeCardState: The PIN of the Card is Block");

        gstUSIMMADFInfo.enPin1Verified  = USIMM_PUK_NEED;
    }

    if(0 == gstUSIMMADFInfo.stPINInfo.ucPin2RemainTime)    /*PIN2ʣ��0��*/
    {
        USIMM_NORMAL_LOG("USIMM_AdjudgeCardState: The PIN2 of the Card is Block");

        gstUSIMMADFInfo.enPin2Verified  = USIMM_PUK_NEED;
    }

    if(0 == gstUSIMMADFInfo.stPINInfo.ucPuk1RemainTime)    /*PUK1ʣ��0��*/
    {
        USIMM_WARNING_LOG("USIMM_AdjudgeCardState: The Card PUK is Block, Card Could Not Used");

        gstUSIMMADFInfo.enPin1Verified  = USIMM_PUK_BLOCK;
    }

    /*��������״̬�жϿ�״̬*/
    if(USIMM_PUK_BLOCK == gstUSIMMADFInfo.enPin1Verified)/*�ж�PUK BLOCK״̬*/
    {
        USIMM_NORMAL_LOG("USIMM_AdjudgeCardState: PUK is Block");

        gstUSIMMBaseInfo.enCurPINType   = USIMM_PUK;

        gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService   = USIMM_CARD_SERVIC_UNAVAILABLE;
    }
    else if(USIMM_PUK_NEED == gstUSIMMADFInfo.enPin1Verified)/*�ж�PUK״̬*/
    {
        USIMM_NORMAL_LOG("USIMM_AdjudgeCardState: The Card Need the PUK Password");

        if (USIMM_POOL_AVAILABLE == gstUSIMMPOOL.enPoolStatus)
        {
            gstUSIMMPOOL.enPoolStatus = USIMM_POOL_NEED_PASSWORD;
        }

        gstUSIMMBaseInfo.enCurPINType = USIMM_PUK;

        gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService = USIMM_CARD_SERVIC_SIM_PUK;
    }
    else if(USIMM_PIN_NEED == gstUSIMMADFInfo.enPin1Verified) /*���жϵ�ǰPIN1״̬*/
    {
        USIMM_NORMAL_LOG("USIMM_AdjudgeCardState: The Card Need the PIN Password");

        if (USIMM_POOL_AVAILABLE == gstUSIMMPOOL.enPoolStatus)
        {
            gstUSIMMPOOL.enPoolStatus = USIMM_POOL_NEED_PASSWORD;
        }

        gstUSIMMBaseInfo.enCurPINType = USIMM_PIN;

        gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService = USIMM_CARD_SERVIC_SIM_PIN;

    }
    else if(USIMM_PUK_NEED == gstUSIMMADFInfo.enPin2Verified)   /*���ж�PUK2״̬*/
    {
        USIMM_NORMAL_LOG("USIMM_AdjudgeCardState: The Card Need the PUK2 Password");

        gstUSIMMBaseInfo.enCurPINType = USIMM_PUK2;

        gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService = USIMM_CARD_SERVIC_AVAILABLE;
    }
    else  /*���ж�PIN2״̬*/                                                                                  /*���Ĭ����ҪPIN2*/
    {
        USIMM_NORMAL_LOG("USIMM_AdjudgeCardState: The Card Need the PIN2 Password");

        gstUSIMMBaseInfo.enCurPINType = USIMM_PIN2;

        gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService = USIMM_CARD_SERVIC_AVAILABLE;
    }

    if(enProCardService != gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService )
    {
        USIMM_NORMAL_LOG("USIMM_AdjudgeCardState: The Card State is Changed, Need Report the Card State");

        gastUSIMMCardAppInfo[USIMM_UICC_USIM].enStateChange = USIMM_CARD_STATE_CHANGED;
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_ReCountPINRemainTime(VOS_UINT32 ulResult,
                                                VOS_UINT32  ulCmdType,
                                                USIMM_PIN_TYPE_ENUM_UINT32 enPINType)
{
    if((USIMM_SW_SECURITY_ERROR == ulResult)||(USIMM_SW_CMD_REMAINTIME == ulResult)
       ||(USIMM_SW_PIN_BLOCK == ulResult))
    {
        if(USIMM_PINCMD_UNBLOCK == ulCmdType)
        {
            USIMM_WARNING_LOG("USIMM_ReCountPINRemainTime: The PUK is Wrong, Verify is failed");

            if(USIMM_PIN == enPINType)
            {
                return USIMM_INCORRECT_PASSWORD;
            }

            if(USIMM_PIN2 == enPINType)
            {
                return USIMM_INCORRECT_PASSWORD;
            }
        }
        else
        {
            USIMM_WARNING_LOG("USIMM_ReCountPINRemainTime: The PIN is Wrong, Verify is failed");

            if(USIMM_PIN == enPINType)
            {
                return ((gstUSIMMADFInfo.stPINInfo.ucPin1RemainTime == 0)?USIMM_SIM_PUK_REQUIRED:USIMM_INCORRECT_PASSWORD);/* [false alarm]: ����Fortify ���� */
            }

            if(USIMM_PIN2 == enPINType)
            {
                return ((gstUSIMMADFInfo.stPINInfo.ucPin2RemainTime == 0)?USIMM_SIM_PUK2_REQUIRED:USIMM_INCORRECT_PASSWORD);/* [false alarm]: ����Fortify ���� */
            }
        }
    }

    USIMM_WARNING_LOG("USIMM_ReCountPINRemainTime: The PIN or PUK Status is Wrong, Reamin Time should not Changed");

    return USIMM_SIM_FAILURE;
}


VOS_UINT32 USIMM_ChangePIN(USIMM_PIN_TYPE_ENUM_UINT32 enPINType, VOS_UINT8 *pucOldPIN, VOS_UINT8 *pucNewPIN)
{
    VOS_UINT32  ulResult;
    VOS_UINT8   ucPINType;

    if(USIMM_PIN == enPINType)
    {
        ucPINType = gstUSIMMADFInfo.ucPIN1RefNum;
    }
    else /*��ʱ����Ҫ֧������PIN*/
    {
        ucPINType = gstUSIMMADFInfo.ucPIN2RefNum;
    }

    ulResult = USIMM_ChangePIN_APDU(ucPINType, pucOldPIN, pucNewPIN);

    if(VOS_OK != ulResult)/*�����*/
    {
        USIMM_ERROR_LOG("USIMM_ChangePIN: USIMM_ChangePIN_APDU Error");

        return VOS_ERR;
    }

    ulResult = USIMM_CheckSW(&gstUSIMMAPDU);

    if(USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
    {
        USIMM_GetPINRemainTime(ucPINType);
    }
    else
    {
        USIMM_STATUSProc();
    }


    if(USIMM_SW_OK != ulResult)
    {
        ulResult = USIMM_ReCountPINRemainTime(ulResult, USIMM_PINCMD_CHANGE, enPINType);
    }

    return ulResult;        /*���Ĳ��ɹ��򷵻�VOS_ERR*/
}
VOS_UINT32 USIMM_UnblockPIN(USIMM_PIN_TYPE_ENUM_UINT32 enPINType, VOS_UINT8 *pucPUK, VOS_UINT8 *pucNewPIN)
{
    VOS_UINT32 ulResult;
    VOS_UINT8  ucPINType;

    if((VOS_NULL_PTR == pucNewPIN)||(VOS_NULL_PTR == pucPUK)
        ||((USIMM_PIN != enPINType)&&(USIMM_PIN2 != enPINType)))/*�������*/
    {
        USIMM_ERROR_LOG("USIMM_UnblockPIN: Input Parameter Error");

        return VOS_ERR;
    }

    if(USIMM_PIN == enPINType)
    {
        if(USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
        {
            ucPINType = gstUSIMMADFInfo.ucPIN1RefNum;
        }
        else
        {
            ucPINType = 0x00;
        }
    }
    else
    {
        ucPINType = gstUSIMMADFInfo.ucPIN2RefNum;
    }

    ulResult = USIMM_UnblockPIN_APDU(ucPINType, pucPUK, pucNewPIN);

    if(VOS_OK != ulResult)/*�����*/
    {
        USIMM_ERROR_LOG("USIMM_UnblockPIN: USIMM_UnblockPIN_APDU Error");

        return VOS_ERR;
    }

    ulResult = USIMM_CheckSW(&gstUSIMMAPDU);

    if(USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
    {
        USIMM_GetPINRemainTime(ucPINType);

        if (VOS_OK != USIMM_GetPUKRemainTime(ucPINType))
        {
            USIMM_INFO_LOG("USIMM_UnblockPIN: Get PUK Remain Time fail");
        }
    }
    else
    {
        USIMM_STATUSProc();
    }

    /*���ݽ�����ĵ�ǰ��PIN����*/
    if(USIMM_SW_OK == ulResult)
    {
        if(USIMM_PIN == enPINType)
        {
            gstUSIMMADFInfo.enPin1Verified = USIMM_NONEED;

            gstUSIMMADFInfo.stPINInfo.enPin1Enable = USIMM_PIN_ENABLED;
        }
        else
        {
            gstUSIMMADFInfo.enPin2Verified = USIMM_PIN_NEED;
        }
    }
    else        /*�ж�ʣ���������PIN״̬*/
    {
        ulResult = USIMM_ReCountPINRemainTime(ulResult, USIMM_PINCMD_UNBLOCK, enPINType);
    }

    return ulResult;        /*���Ĳ��ɹ��򷵻�VOS_ERR*/
}
VOS_UINT32 USIMM_PINVerify(USIMM_PIN_TYPE_ENUM_UINT32 enPINType,VOS_UINT8 ucPINCmd,VOS_UINT8 *pucPIN)
{
    VOS_UINT32 ulResult;
    VOS_UINT8  ucPINType;

    if(USIMM_PIN == enPINType)
    {
        ucPINType = gstUSIMMADFInfo.ucPIN1RefNum;
    }
    else            /*��ʱ����Ҫ֧����������PIN*/
    {
        ucPINType = gstUSIMMADFInfo.ucPIN2RefNum;
    }

    ulResult = USIMM_PINVerify_APDU(ucPINCmd,ucPINType, pucPIN);

    if(VOS_OK != ulResult)/*�����*/
    {
        USIMM_ERROR_LOG("USIMM_PINVerify: USIMM_PINVerify_APDU Error");

        return VOS_ERR;
    }

    ulResult = USIMM_CheckSW(&gstUSIMMAPDU);

    if(USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)  /*���³�ʼ����ǰ��PINʣ�����*/
    {
        USIMM_GetPINRemainTime(ucPINType);
    }
    else
    {
        USIMM_STATUSProc();
    }

    if(USIMM_SW_OK == ulResult) /*��ǰ�����ɹ�*/
    {
        if(USIMM_PIN2 == enPINType)     /*����PIN2����Ҫ�޸�״̬*/
        {
            return VOS_OK;
        }

        if(USIMM_PINCMD_ENABLE == ucPINCmd)    /*���ݵ�ǰ�Ĳ��������޸�PIN1״̬*/
        {
            gstUSIMMADFInfo.stPINInfo.enPin1Enable  = USIMM_PIN_ENABLED;
        }
        else if(USIMM_PINCMD_DISABLE== ucPINCmd)
        {
            gstUSIMMADFInfo.stPINInfo.enPin1Enable  = USIMM_PIN_DISABLED;

            gstUSIMMADFInfo.enPin1Verified          = USIMM_NONEED;
        }
        else
        {
            gstUSIMMADFInfo.enPin1Verified          = USIMM_NONEED;
        }
    }
    else        /*�ж�ʣ�����*/
    {
        ulResult = USIMM_ReCountPINRemainTime(ulResult, ucPINCmd, enPINType);
    }

    return ulResult;        /*���Ĳ��ɹ��򷵻�VOS_ERR*/
}


VOS_UINT32 USIMM_InitUsimFDNBDNStatus(VOS_VOID)
{
    VOS_UINT32                  ulResult;
    VOS_UINT8                   ucData;

    ulResult = USIMM_GetTFFile(USIMM_UMTS_APP, EFEST, sizeof(ucData), &ucData);

    if(VOS_OK == ulResult) /*���ļ��Ĵ��ڲ��Ǳ����*/
    {
        /*�ж����ݳ�ʼ����Ӧ��ȫ�ֱ���*/
        if (PS_USIM_SERVICE_AVAILIABLE == USIMM_IsServiceAvailable(USIM_SVR_FDN))
        {
            gstUSIMMBaseInfo.enFDNStatus = (ucData&0x01)?USIMM_FDNSTATUS_ON:USIMM_FDNSTATUS_OFF;
        }
        else
        {
            gstUSIMMBaseInfo.enFDNStatus = USIMM_FDNSTATUS_OFF;
        }

        if (PS_USIM_SERVICE_AVAILIABLE == USIMM_IsServiceAvailable(USIM_SVR_BDN))
        {
            gstUSIMMBaseInfo.enBDNStatus = (ucData&0x02)?USIMM_BDNSTATUS_ON:USIMM_BDNSTATUS_OFF;
        }
        else
        {
            gstUSIMMBaseInfo.enBDNStatus = USIMM_BDNSTATUS_OFF;
        }
    }
    else
    {
        USIMM_ERROR_LOG("USIMM_InitUsimFDNBDNStatus: USIMM_InitUsimFDNBDNStatus Error");
    }

    return ulResult;
}


VOS_UINT32 USIMM_SimFDNBDNProcess(VOS_VOID)
{
    VOS_UINT32 ulResult;
    VOS_UINT16 ausPath[3] = {MF, DFGSM, EFIMSI};

    ulResult = USIMM_SelectFile(USIMM_GSM_APP, USIMM_NEED_FCP, ARRAYSIZE(ausPath), ausPath);

    if(VOS_OK != ulResult)/*�жϽ��*/
    {
        USIMM_ERROR_LOG("USIMM_SimFDNBDNProcess: USIMM_SelectFileWithPath(EFIMSI) Error");

        return VOS_ERR;
    }

    if(USIMM_EFSTATUS_DEACTIVE == gstUSIMMCurFileInfo.stEFInfo.enFileStatus)
    {
        USIMM_ActivateFile_APDU();
    }

    ausPath[2] = EFLOCI;

    ulResult = USIMM_SelectFile(USIMM_GSM_APP, USIMM_NEED_FCP, ARRAYSIZE(ausPath), ausPath);

    if(VOS_OK != ulResult)/*�жϽ��*/
    {
        USIMM_ERROR_LOG("USIMM_SimFDNBDNProcess: USIMM_SelectFileWithPath(EFLOCI) Error");

        return VOS_ERR;
    }

    if(USIMM_EFSTATUS_DEACTIVE == gstUSIMMCurFileInfo.stEFInfo.enFileStatus)
    {
        USIMM_ActivateFile_APDU();
    }

    return VOS_OK;
}


VOS_UINT32 USIMM_InitSimFDNBDNStatus(VOS_VOID)
{
    VOS_UINT32                  ulResult;
    VOS_UINT16                  ausPath[] = {MF,DFGSM,EFIMSI};
    USIMM_EFSTATUS_ENUM_UINT32  imsiState = USIMM_EFSTATUS_ACTIVE;
    USIMM_EFSTATUS_ENUM_UINT32  lociState = USIMM_EFSTATUS_ACTIVE;

    /* PHASE1��SIM������Ҫ����BDN FDN���̣�ֻ�� PHASE2 PHASE2+�Ŀ�����Ҫ */
    if(gstUSIMMBaseInfo.ulSimPhase < USIMM_SIM_PHASE_TWO)
    {
        USIMM_WARNING_LOG("USIMM_SimProfileDownload: SIM Not Support SAT");

        return VOS_OK;
    }

    ulResult = USIMM_SelectFile(USIMM_GSM_APP, USIMM_NEED_FCP, ARRAYSIZE(ausPath), ausPath);

    if(VOS_OK != ulResult)/*�жϽ�����ļ�״̬*/
    {
        USIMM_ERROR_LOG("USIMM_InitSimFDNBDNStatus: USIMM_SelectFileWithPath(EFIMSI) Error");

        return VOS_ERR;
    }

    imsiState = gstUSIMMCurFileInfo.stEFInfo.enFileStatus;

    ausPath[2] = EFLOCI;

    ulResult = USIMM_SelectFile(USIMM_GSM_APP, USIMM_NEED_FCP, ARRAYSIZE(ausPath), ausPath);

    if(VOS_OK != ulResult)/*�жϽ�����ļ�״̬*/
    {
        USIMM_ERROR_LOG("USIMM_InitSimFDNBDNStatus: USIMM_SelectFileWithPath(EFLOCI) Error");

        return VOS_ERR;
    }

    lociState = gstUSIMMCurFileInfo.stEFInfo.enFileStatus;

    /* IMSI��LOCI��ͬʱ��Чʱ������ִ�в���Լ������ */
    if((USIMM_EFSTATUS_DEACTIVE != imsiState)||(USIMM_EFSTATUS_DEACTIVE != lociState))
    {
        return VOS_OK;
    }

    /* BDN�������� */
    USIMM_BdnCapabilityRequest(&gstUSIMMBaseInfo.enBDNStatus);

    /* FDN�������� */
    USIMM_FdnCapabilityRequest(&gstUSIMMBaseInfo.enFDNStatus);

    return USIMM_SimFDNBDNProcess();
}


VOS_VOID USIMM_FdnCapabilityRequest(USIMM_FDNSTATUS_ENUM_UINT32 *pulFdnStatus)
{
    VOS_UINT16                          ausPath[] = {MF, DFTELCOM, EFADN};
    VOS_UINT32                          ulResult;

    /* ��FDN״̬��SST�ļ���δallocated and activated,FDNδ���� */
    if (PS_USIM_SERVICE_NOT_AVAILIABLE == USIMM_IsServiceAvailable(SIM_SVR_FDN))
    {
        *pulFdnStatus = USIMM_FDNSTATUS_OFF;

        return;
    }

    /* ��ADN״̬��SST�ļ���δallocated and activated,FDN���� */
    if (PS_USIM_SERVICE_NOT_AVAILIABLE == USIMM_IsServiceAvailable(SIM_SVR_ADN))
    {
        *pulFdnStatus = USIMM_FDNSTATUS_ON;

        return;
    }

    /* ADN�ļ�������,FDN���� */
    ulResult = USIMM_SelectFile(USIMM_GSM_APP, USIMM_NEED_FCP, ARRAYSIZE(ausPath), ausPath);
    if (VOS_OK != ulResult)
    {
        *pulFdnStatus = USIMM_FDNSTATUS_ON;
    }
    else
    {
        /* ADN�ļ���Ч,FDN���� */
        if (USIMM_EFSTATUS_DEACTIVE == gstUSIMMCurFileInfo.stEFInfo.enFileStatus)
        {
            *pulFdnStatus = USIMM_FDNSTATUS_ON;
        }
        /* ADN�ļ���Ч,FDNδ���� */
        else
        {
            *pulFdnStatus = USIMM_FDNSTATUS_OFF;
        }
    }

    return;
}
VOS_VOID USIMM_BdnCapabilityRequest(USIMM_BDNSTATUS_ENUM_UINT32 *pulBdnStatus)
{
    VOS_UINT16                          ausPath[] = {MF, DFTELCOM, EFBDN};
    VOS_UINT32                          ulResult;

    /* �����֧��Call Control���ܣ�BDNδ���� */
    if (PS_USIM_SERVICE_NOT_AVAILIABLE == USIMM_IsServiceAvailable(SIM_SVR_CALL_CONTROL))
    {
        *pulBdnStatus = USIMM_BDNSTATUS_OFF;

        return;
    }

    /* BDN��SST�в����ڻ�BDN״̬��SST�ļ���δallocated and activated,BDNδ���� */
    if (PS_USIM_SERVICE_NOT_AVAILIABLE == USIMM_IsServiceAvailable(SIM_SVR_BDN))
    {
        *pulBdnStatus = USIMM_BDNSTATUS_OFF;

        return;
    }

    /* BDN�ļ�������,BDNδ���� */
    ulResult = USIMM_SelectFile(USIMM_GSM_APP, USIMM_NEED_FCP, ARRAYSIZE(ausPath), ausPath);
    if (VOS_OK != ulResult)
    {
        *pulBdnStatus = USIMM_BDNSTATUS_OFF;
    }
    else
    {
        /* BDN�ļ���Ч,BDN���� */
        if (USIMM_EFSTATUS_ACTIVE == gstUSIMMCurFileInfo.stEFInfo.enFileStatus)
        {
            *pulBdnStatus = USIMM_BDNSTATUS_ON;
        }
        /* BDN�ļ���Ч,BDNδ���� */
        else
        {
            *pulBdnStatus = USIMM_BDNSTATUS_OFF;
        }
    }

    return;
}
VOS_UINT32 USIMM_FDNEnable(VOS_VOID)
{
    VOS_UINT32                          ulResult;
    VOS_UINT16                          ausPath[]={MF,DFTELCOM,EFADN};
    VOS_UINT8                           ucData;


    if(USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
    {
        ulResult = USIMM_PoolReadOneFile(EFEST, sizeof(VOS_UINT8), &ucData, USIMM_UMTS_APP);

        if(VOS_ERR == ulResult)/*���������û������Ҫ�ӿ��ж�ȡ*/
        {
            return VOS_ERR;
        }

        ucData = ucData|0x01;

        ulResult = USIMM_WriteTFFile(USIMM_UMTS_APP, EFEST, sizeof(ucData), &ucData);

        if(USIMM_SW_OK != ulResult)/*�жϽ��*/
        {
            USIMM_ERROR_LOG("USIMM_FDNEnable: USIMM_UpdateTFFile Error");

            return VOS_ERR;
        }

        USIMM_PoolUpdateOneFile(EFEST, USIMM_UMTS_APP, sizeof(ucData), &ucData);
    }
    else
    {
        ulResult = USIMM_SelectFile(USIMM_GSM_APP, USIMM_NEED_FCP, ARRAYSIZE(ausPath), ausPath);

        if(VOS_OK != ulResult)/*�жϽ��*/
        {
            USIMM_ERROR_LOG("USIMM_FDNEnable: USIMM_SelectFileWithPath(EFADN) Error");

            return VOS_ERR;
        }

        ulResult = USIMM_DeactivateFile_APDU();

        if(VOS_OK != ulResult)/*�жϽ��*/
        {
            USIMM_ERROR_LOG("USIMM_FDNEnable: USIMM_DeactivateFile_APDU Error");

            return VOS_ERR;
        }

        ulResult = USIMM_CheckSW(&gstUSIMMAPDU);

        if(USIMM_SW_OK != ulResult)/*�жϽ��*/
        {
            return VOS_ERR;
        }
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_FDNDisable(VOS_VOID)
{
    VOS_UINT32  ulResult;
    VOS_UINT8   ucData;
    VOS_UINT16  ausPath[] = {MF, DFTELCOM,EFADN};

    if(USIMM_CARD_USIM == gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)/*USIM��*/
    {
        ulResult = USIMM_PoolReadOneFile(EFEST, sizeof(VOS_UINT8), &ucData, USIMM_UMTS_APP);

        if(VOS_ERR == ulResult)/*���������û������Ҫ�ӿ��ж�ȡ*/
        {
            USIMM_ERROR_LOG("USIMM_FDNDisable: USIMM_PoolReadOneFile Failed");

            return VOS_ERR;
        }

        ucData = ucData&0xFE;

        ulResult = USIMM_WriteTFFile(USIMM_UMTS_APP, EFEST, sizeof(VOS_UINT8),&ucData);

        if(VOS_OK != ulResult)/*�жϽ��*/
        {
            USIMM_ERROR_LOG("USIMM_FDNDisable: USIMM_UpdateTFFile Failed");

            return VOS_ERR;
        }

        USIMM_PoolUpdateOneFile(EFEST, USIMM_UMTS_APP, sizeof(ucData), &ucData);
    }
    else
    {
        /* ����ADN�绰��*/
        ulResult = USIMM_SelectFile(USIMM_GSM_APP, USIMM_NEED_FCP, ARRAYSIZE(ausPath), ausPath);

        if(VOS_OK != ulResult)/*�жϽ��*/
        {
            USIMM_ERROR_LOG("USIMM_FDNDisable: USIMM_SelectFileWithPath(EFADN) Failed");

            return VOS_ERR;
        }

        ulResult = USIMM_ActivateFile_APDU();

        if(VOS_OK != ulResult)/*�жϽ��*/
        {
            USIMM_ERROR_LOG("USIMM_FDNDisable: USIMM_ActivateFile_APDU Failed");

            return VOS_ERR;
        }

        ulResult = USIMM_CheckSW(&gstUSIMMAPDU);

        if(USIMM_SW_OK != ulResult)/*�жϽ��*/
        {
            return VOS_ERR;
        }
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_FindTagInSMPTLV(VOS_UINT8 *pucDataBuf, VOS_UINT8 ucTag, VOS_UINT32 ulDataLen)
{
    VOS_UINT32 i;

    for(i=0; i<ulDataLen;)
    {
        if((pucDataBuf[i]&0x7F) == (ucTag&0x7F))
        {
            if(pucDataBuf[i+1] == 0x81)
            {
                return (i+2);   /*���س����ֽڵ�ƫ��*/
            }
            else
            {
                return (i+1);
            }
        }
        else
        {
            if(pucDataBuf[i+1] == 0x81)      /*������һ��Tag*/
            {
                i += pucDataBuf[i+2] + 3;
            }
            else
            {
                i += pucDataBuf[i+1] + 2;
            }
        }
    }

    return USIMM_TAGNOTFOUND;
}



VOS_UINT32 USIMM_FindTagInBERTLV(VOS_UINT8 *pucDataBuf, VOS_UINT8 ucTag, VOS_UINT32 ulDataLen)
{
    VOS_UINT32 i;

    for(i=0; i<ulDataLen;)
    {
        if((pucDataBuf[i]&0x7F) == (ucTag&0x7F))   /*���س����ֽ�ƫ��*/
        {
            return (i+1);
        }
        else
        {
            i += pucDataBuf[i+1] + 2; /*������һ��Tag*/
        }
    }

    return USIMM_TAGNOTFOUND;
}


VOS_UINT32 USIMM_FindTagInCTLV(VOS_UINT8 *pucDataBuf, VOS_UINT8 ucTag, VOS_UINT32 ulDataLen)
{
    VOS_UINT32 i;

    for(i=0; i<ulDataLen;)
    {
        if((pucDataBuf[i]&0xF0) == (ucTag&0xF0))    /*���ص�ǰ���ݵ�һ���ֽ�ƫ��*/
        {
            return (i+1);
        }
        else
        {
            i += (pucDataBuf[i]&0x0F) + 1 ;   /*������һ��Tag*/
        }
    }

    return USIMM_TAGNOTFOUND;
}


VOS_UINT32 USIMM_GetBitFromBuf(VOS_UINT8 *pucDataBuf, VOS_UINT32 ulBitNo,VOS_UINT32 ulBufLen)
{
    VOS_UINT32  ulOffset;
    VOS_UINT8   ucBit;

    ulOffset = (ulBitNo - 1)/ 8;

    ucBit     = (VOS_UINT8)((ulBitNo - 1)%8);

    if ( ulOffset >=  ulBufLen )
    {
        return USIMM_BITNOFOUNE;
    }

    return ((pucDataBuf[ulOffset]>>ucBit)&0x00000001);
}



VOS_UINT32 USIMM_DecideTMode(SCI_ATRInfo *pstSCIATRInfo)
{
    NV_USIMM_T1_CTRL_PARA_STRU                              stT1CtrlPara;
    USIMM_T1_BLOCK_EPILOGUE_FIELD_ENUM_UINT32               enEDM;
    VOS_UINT32                                              ulResult;
    VOS_UINT32                                              ulResult2 = VOS_ERR;
    VOS_INT32                                               lVoltageSwitchRst;

    VOS_MemSet(&stT1CtrlPara, 0, sizeof(stT1CtrlPara));

    /* ��NV�� */
    if(VOS_OK != NV_Read(en_NV_Item_Usimm_T1_Ctrl_PARA, &stT1CtrlPara, sizeof(stT1CtrlPara)))
    {
        /* ��ӡ���� */
        USIMM_ERROR_LOG("USIMM_DecideTMode:USIM Read T1 PARA NV error");

        /* NV��ȡʧ�ܣ���ʽ��ֵΪT0 */
        stT1CtrlPara.enTMode = USIMM_DL_T_MODE_T0;

    }

    /* ���NVָʾ֧��T=1,�ҵ���API����֧��T=1 */
    if((USIMM_DL_T_MODE_T1 == stT1CtrlPara.enTMode)
        && (T1_PROTOCOL_MODE == pstSCIATRInfo->Tbyte))
    {
        /* ����T=1ģʽ */
        g_enUSIMMTMode = USIMM_DL_T_MODE_T1;

        /* TS_102221�淶��EDCֻʹ��LRC����ISO_IEC 7816-3 2006�У���涨��ATR�е�ָʾ������EDCʹ��LRC��CRC��
            ΪЭ����ݿ��ǣ�����֧�������ֹ淶����NV��en_NV_Item_Usimm_T1_Ctrl_PARA����������ѭ��һ�ֹ淶 */
        if(USIMM_T1_ERR_DETECT_MODE_TS102221 == stT1CtrlPara.enEDM)
        {
            enEDM = USIMM_T1_BLOCK_EPILOGUE_FIELD_LRC;
        }
        else
        {
            enEDM = pstSCIATRInfo->errDetectMode;
        }

        /* ����ATR�е����T=1��Ϣ�����ڿ���λ��ָ�T=1������ʹ�� */
        g_stUSIMMT1Para.enErrDetectMode = enEDM;
        g_stUSIMMT1Para.ulIFSC          = pstSCIATRInfo->valIFSC;
        g_stUSIMMT1Para.ulIFSD          = pstSCIATRInfo->valIFSD;

        /* ����T=1���� */
        ulResult = USIMM_T1DLUpdateParaByATR(enEDM, pstSCIATRInfo->valIFSC, pstSCIATRInfo->valIFSD);

        if(VOS_OK == ulResult)
        {
            OM_RecordInfoStart(VOS_EXC_DUMP_MEM_NUM_1, WUEPS_PID_USIM, WUEPS_PID_USIM, USIMMDL_DRV_USIMMSCI_T1_PRTCL_SWITCH_2);
            if(VOS_OK != DRV_USIMMSCI_PROTOCOL_SWITCH((PROTOCOL_MODE)g_enUSIMMTMode))
            {
                USIMM_WARNING_LOG("USIMM_DecodeATR:SET T MODE FAILED");

                /* ��ά�ɲ���Ϣͳ�� */
                g_stT1StatisticInfo.ulProtocolSwitchFailCnt++;
            }
            OM_RecordInfoEnd(VOS_EXC_DUMP_MEM_NUM_1);

            /* ����IFS����,ȷ���Ƿ�ȷʵ֧��T=1 */
            ulResult2 = USIMM_T1SendIFSD_APDU((VOS_UINT8)stT1CtrlPara.ulDefaultIFSD);
        }

        /* ���²���ʧ�ܻ��߷���IFSʧ�ܶ�Ҫ�е�T=0 */
        if((VOS_OK != ulResult) || (VOS_OK != ulResult2))
        {
            if(USIMM_DL_T1_GCF_ENABLE == stT1CtrlPara.enGcfFlag)
            {
                USIMM_ERROR_LOG("USIMM_DecideTMode: it is GCF test");

                return VOS_ERR;
            }

            g_enUSIMMTMode = USIMM_DL_T_MODE_T0;

            /* ����IFSʧ���л���T=0 */
            ulResult = USIMM_DLResetCard(USIMM_RESET_CARD, &lVoltageSwitchRst);

            if(VOS_OK != ulResult)
            {
                USIMM_ERROR_LOG("USIMM_DecideTMode: Reset Card is Failed");

                return VOS_ERR;
            }
        }
    }
    else
    {
        g_enUSIMMTMode = USIMM_DL_T_MODE_T0;
    }

    return VOS_OK;
}


VOS_UINT32 USIMM_DecodeATR(VOS_VOID)
{
    VOS_INT32                           lSCIResult;
    VOS_UINT32                          ulATRLen     = 0;
    VOS_UINT8                           aucATR[USIMM_ATR_MAX_LEN] = {0};
    VOS_UINT32                          ulHistoryLen;
    VOS_UINT8                           aucAtrStub[2] = {0x3B, 0x00};
    OM_SIM_ATR_FLAG_STRU                stFlag;
    SCI_ATRInfo                                             stSCIATRInfo;

    stFlag.usSimAtrFlag = VOS_FALSE;

    lSCIResult = DRV_USIMMSCI_GET_ATR(&ulATRLen, aucATR, &stSCIATRInfo);

	/*lint --e{831,669} */
    if((USIMM_SCI_SUCCESS != lSCIResult)||(ulATRLen > sizeof(aucATR)))
    {
        USIMM_ERROR_LOG("USIMM_DecodeATR:USIMMSCIGetATR get ATR Error");

        return VOS_ERR;
    }

    if(VOS_OK != USIMM_DecideTMode(&stSCIATRInfo))
    {
        USIMM_ERROR_LOG("USIMM_DecodeATR:USIMM_DecideTMode Error");

        return VOS_ERR;
    }

    /* ֪ͨ����ǰ֧��Tģʽ,ʧ��ֻ����ӡ */
    OM_RecordInfoStart(VOS_EXC_DUMP_MEM_NUM_1, WUEPS_PID_USIM, WUEPS_PID_USIM, USIMMDL_DRV_USIMMSCI_T1_PRTCL_SWITCH_2);
    if(VOS_OK != DRV_USIMMSCI_PROTOCOL_SWITCH((PROTOCOL_MODE)g_enUSIMMTMode))
    {
        USIMM_WARNING_LOG("USIMM_DecodeATR:SET T MODE FAILED");

        /* ��ά�ɲ���Ϣͳ�� */
        g_stT1StatisticInfo.ulProtocolSwitchFailCnt++;
    }
    OM_RecordInfoEnd(VOS_EXC_DUMP_MEM_NUM_1);

    USIMM_SciDataHook(ulATRLen, aucATR);

    ulHistoryLen = aucATR[USIMM_ATR_T0_OFFSET]&0x0F;  /*��ȡATR��ʷ�ֽڳ���*/

    if(ulATRLen < (ulHistoryLen+1))
    {
        USIMM_ERROR_LOG("USIMM_DecodeATR:USIM ATR Length Error");

        return VOS_ERR;
    }

    gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType = USIMM_CARD_USIM;

    /* ���´�����GCF����׮����NV���ʱ����ATRΪ3B 00ʱ����Ҫ����Ϊ��ǰ��SIM�� */
    if (VOS_OK != NV_Read(en_NV_Item_Sim_ATR_Flag, &stFlag, sizeof(stFlag)))
    {
        return VOS_OK;
    }

    if (VOS_FALSE == stFlag.usSimAtrFlag)
    {
        return VOS_OK;
    }

    if (ulATRLen != sizeof(aucAtrStub))
    {
        return VOS_OK;
    }

    if (VOS_OK == VOS_MemCmp(aucATR, aucAtrStub, sizeof(aucAtrStub)))
    {
        gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType = USIMM_CARD_SIM;

        /* Ĭ�ϴ�7F20��ʼѡ���ļ� */
        gstUSIMMCurFileInfo.usCurDFID      = MF;
        gstUSIMMCurFileInfo.usCurEFID      = USIMM_NULL_ID;
        gstUSIMMCurFileInfo.usCurDFFcpLen  = VOS_NULL;
        gstUSIMMCurFileInfo.usFilePathLen  = 1;
        gstUSIMMCurFileInfo.ausFilePath[0] = MF;
    }

    return VOS_OK;
}


VOS_VOID USIMM_DecodeATRSecondStep(VOS_VOID)
{
    VOS_UINT32                          ulATRLen     = 0;
    VOS_UINT8                           aucATR[USIMM_ATR_MAX_LEN]   = {0};
    VOS_UINT32                          ulOffset;
    VOS_UINT32                          ulDataOffset;
    VOS_UINT32                          ulHistoryLen;
    VOS_INT32                           lSCIResult;
    VOS_UINT32                          ulResult;
    SCI_ATRInfo                         stSCIATRInfo;

    lSCIResult = DRV_USIMMSCI_GET_ATR(&ulATRLen, aucATR, &stSCIATRInfo);

    if ((USIMM_SCI_SUCCESS != lSCIResult)
        ||(ulATRLen > sizeof(aucATR)))
    {
        USIMM_ERROR_LOG("USIMM_DecodeATRSecondStep:USIMMSCIGetATR get ATR Error");

        return;
    }

    USIMM_SciDataHook(ulATRLen, aucATR);

    /* ��ȡATR��ʷ�ֽڳ��� */
    ulHistoryLen    = aucATR[USIMM_ATR_T0_OFFSET]&0x0F;

    /* ��ȡATR��ʷ�ֽڵ���ʼλ�� */
    if ((ulATRLen < ulHistoryLen) || (VOS_NULL == ulHistoryLen))
    {
        return;
    }

    ulOffset        = ulATRLen - ulHistoryLen;

    /* 3G������ʷ�ֽ� */
    if(CTLV_IND_TAG == aucATR[ulOffset])
    {
        ulOffset++;                             /* ָ�����ĵ�һ��Tag */

        if (ulOffset >= sizeof(aucATR))
        {
            return;
        }

        ulHistoryLen--;

        /*�����������ֽ�*/
        ulResult = USIMM_FindTagInCTLV(&aucATR[ulOffset], CARD_CPB_TAG, ulHistoryLen);

        if(USIMM_TAGNOTFOUND != ulResult)
        {
            if (ulResult > ulHistoryLen)
            {
                return;
            }

            ulDataOffset = ulResult + ulOffset;

            if (ulDataOffset >= sizeof(aucATR))
            {
                return;
            }

            gstUSIMMBaseInfo.enSupportPath = USIMM_GetBitFromBuf(&aucATR[ulDataOffset], 6, sizeof(VOS_UINT8));

            gstUSIMMBaseInfo.enSupportSFI  = USIMM_GetBitFromBuf(&aucATR[ulDataOffset], 3, sizeof(VOS_UINT8));

            if(CARD_CPB3_TAG == aucATR[ulDataOffset-1])
            {
                if ((ulDataOffset + 1) >= sizeof(aucATR))
                {
                    return;
                }

                gstUSIMMBaseInfo.enSupportChannel = (((aucATR[ulDataOffset+1]&0x10) != 0)?USIMM_SUPPORT_CHANNEL:USIMM_NOTSUPPORT_CHANNEL);
            }
        }
    }

    return;
}
VOS_UINT32 USIMM_DecodeDFPSDO(VOS_UINT8 *pucPSDO, VOS_UINT32 ulDataLen,USIMM_DFFCP_ST *pstDfInfo)
{
    VOS_UINT32  ulOffset = 0;
    VOS_UINT8   ucPINMapOffset = 2;
    VOS_UINT32  i ;
    VOS_UINT8   ucPinMapMask=0x80;
    VOS_UINT32  ulPinStatus;

    i = pucPSDO[ulOffset+1]*8;       /*�ܹ����ܵ�����������ں���λͼ����*/

    ulOffset = pucPSDO[ulOffset+1] + 2;

    while(((i > 0) && (ulOffset < ulDataLen)))
    {
        if(pucPSDO[ulOffset] != USIMM_FCP_PINRER_TAG)
        {
            ulOffset += 3;

            continue;
        }

        ulPinStatus = (pucPSDO[ucPINMapOffset]&ucPinMapMask)?1:0; /*���Bit��Ϊ0*/

        if((pucPSDO[ulOffset+2] <= 0x88) && (pucPSDO[ulOffset+2] >= 0x81))
        {
            pstDfInfo->ucPIN2RefNum                 = pucPSDO[ulOffset+2];

            pstDfInfo->stPINInfo.enPin2Enable = USIMM_PIN_ENABLED&ulPinStatus;
        }
        else if((pucPSDO[ulOffset+2] <= 0x08) && (pucPSDO[ulOffset+2] >= 0x01))
        {
            pstDfInfo->ucPIN1RefNum                 = pucPSDO[ulOffset+2];

            pstDfInfo->stPINInfo.enPin1Enable = USIMM_PIN_ENABLED&ulPinStatus;
        }
        else if(pucPSDO[ulOffset+2] == 0x11)
        {
            gstUSIMMBaseInfo.enUniversalPINStatus = USIMM_PIN_ENABLED&ulPinStatus;
        }
        else
        {
            USIMM_NORMAL_LOG("USIMM_DecodeDFPSDO: The PSDO Could Not Support");
        }

        i--;

        ulOffset += 3;

        if( 0 == (i % 8) )/*���bitmap����һ���ֽڣ�ȡ���ֽڵ�bitmap*/
        {
            ucPINMapOffset++;

            ucPinMapMask = 0x80;
        }
        else
        {
            ucPinMapMask = ucPinMapMask>>1;
        }
    }

    return VOS_OK;
}
VOS_VOID USIMM_DecodeCharacter(VOS_UINT8 ucFileChara)
{
    if(0x00 == (ucFileChara&0x0D))
    {
        gstUSIMMDrvInfo.enCLKStop = USIMM_CLKSTOP_NOALLOW;

        return;
    }

    if(0x00 == (ucFileChara&0x0C))/*����Level ��ȡֵHIGH,LOW,NONE*/
    {
         gstUSIMMDrvInfo.enCLKLevel = USIMM_CLKLEVEL_LOW;/*�����ǰ��ƽ��ȷ����Ĭ�ϰ��յ͵�ƽ����*/
    }
    else if(0x04 == (ucFileChara&0x0C))
    {
        gstUSIMMDrvInfo.enCLKLevel = USIMM_CLKLEVEL_HIGH;/*�ߵ�ƽ����ʱ��ֹͣ*/
    }
    else if(0x08 == (ucFileChara&0x0C))
    {
        gstUSIMMDrvInfo.enCLKLevel = USIMM_CLKLEVEL_LOW;/*�͵�ƽ����ʱ��ֹͣ*/
    }
    else
    {
        USIMM_WARNING_LOG("USIMM_DecodeCharacter:Input Error");

        gstUSIMMDrvInfo.enCLKLevel = USIMM_CLKLEVEL_NONE;
    }

    return;
}


VOS_UINT32 USIMM_DecodeUsimDFFcp(VOS_UINT32 ulLen, VOS_UINT8 *pucData, USIMM_DFFCP_ST *pstDfInfo)
{
    VOS_UINT32 ulResult = VOS_OK;
    VOS_UINT32 ulStart;
    VOS_UINT32 ulOffset;
    VOS_UINT32 ulSubOffset;
    VOS_UINT32 ulDataLen;

    ulStart = USIMM_FindTagInSMPTLV(pucData, USIMM_FCP_TAG, ulLen);/*��ѯ��ʼTag*/

    if(USIMM_TAGNOTFOUND == ulStart)
    {
        USIMM_ERROR_LOG("USIMM_DecodeUsimDFFcp:FCP Tag not found");

        return VOS_ERR;
    }

    ulDataLen = pucData[ulStart];   /*��Ч���ݳ���*/

    ulStart++;                      /*ָ����Ҫ����������ͷ*/

    /*�����ļ������ֶ�*/
    ulOffset = USIMM_FindTagInBERTLV(&pucData[ulStart], USIMM_FCP_FILE_DES_TAG, ulDataLen);

    if(USIMM_TAGNOTFOUND == ulOffset)   /*���ֶα������*/
    {
        USIMM_ERROR_LOG("USIMM_DecodeUsimDFFcp:File Describer Tag not found");

        return VOS_ERR;
    }

    ulOffset +=ulStart + 1;

    if((pucData[ulOffset]&0x38) == 0) /*�ļ����ʹ���*/
    {
        USIMM_ERROR_LOG("USIMM_DecodeUsimDFFcp:File Type Error");

        return VOS_ERR;
    }

    ulOffset = USIMM_FindTagInBERTLV(&pucData[ulStart], USIMM_FCP_FID_TAG, ulDataLen);

    if(USIMM_TAGNOTFOUND != ulOffset)
    {
        ulOffset += ulStart + 1;

        pstDfInfo->usDFID = (VOS_UINT16)((pucData[ulOffset]<<0x08)&0xFF00)+pucData[ulOffset+1];
    }

    ulOffset = USIMM_FindTagInBERTLV(&pucData[ulStart], USIMM_FCP_NAME_TAG, ulDataLen);

    if(USIMM_TAGNOTFOUND != ulOffset)
    {
        ulOffset += ulStart;

        if(pucData[ulOffset] > 16)
        {
            USIMM_ERROR_LOG("USIMM_DecodeUsimDFFcp:File Name Error");

            return VOS_ERR;
        }

        VOS_MemCpy(pstDfInfo->aucName, &pucData[ulOffset+1], pucData[ulOffset]);

        pstDfInfo->ucNameLen = pucData[ulOffset];
    }

    /*����File Characteristic ����й�CLockֹͣ״̬��Level��Ϣ*/
    ulOffset = USIMM_FindTagInBERTLV(&pucData[ulStart], USIMM_FCP_PRIVITE_TAG, ulDataLen);

    if(USIMM_TAGNOTFOUND != ulOffset)
    {
       ulSubOffset = USIMM_FindTagInBERTLV(&pucData[ulOffset+ulStart+1], USIMM_FCP_CHARA_TAG, pucData[ulOffset+ulStart]);

       if ( USIMM_TAGNOTFOUND != ulSubOffset )
       {
            USIMM_DecodeCharacter(pucData[ulOffset+ulStart+ulSubOffset+2]);/*����������Ϣָ��Ҫ����������*/

            pstDfInfo->ucCharaByte = pucData[ulOffset+ulStart+ulSubOffset+2];
       }

       ulSubOffset = USIMM_FindTagInBERTLV(&pucData[ulOffset+ulStart+1], USIMM_FCP_SUPSYSCMD_TAG, pucData[ulOffset+ulStart]);

       if ( USIMM_TAGNOTFOUND != ulSubOffset )
       {
           pstDfInfo->ucSupCmd = pucData[ulOffset+ulStart+ulSubOffset+2];
       }
    }

    /*ȷ��DF��ȫ��ʶ����*/
    pstDfInfo->enSCMode  = USIMM_SC_ATTRIB_NO;

    ulOffset = USIMM_FindTagInBERTLV(&pucData[ulStart], USIMM_FCP_SA_CF_TAG, ulDataLen);

    if(USIMM_TAGNOTFOUND != ulOffset)
    {
        pstDfInfo->enSCMode = USIMM_SC_ATTRIB_COMPACT;
    }

    ulOffset = USIMM_FindTagInBERTLV(&pucData[ulStart], USIMM_FCP_SA_REF_TAG, ulDataLen);

    if(USIMM_TAGNOTFOUND != ulOffset)
    {
        pstDfInfo->enSCMode = USIMM_SC_ATTRIB_REFERENCE;
    }

    ulOffset = USIMM_FindTagInBERTLV(&pucData[ulStart], USIMM_FCP_SA_EF_TAG, ulDataLen);

    if(USIMM_TAGNOTFOUND != ulOffset)
    {
        pstDfInfo->enSCMode = USIMM_SC_ATTRIB_EXPANDED;
    }

    /*����PIN״̬���ݶ��������ֶ�*/
    ulOffset = USIMM_FindTagInBERTLV(&pucData[ulStart], USIMM_FCP_PIN_TO_TAG, ulDataLen);

    if(USIMM_TAGNOTFOUND == ulOffset)
    {
        USIMM_ERROR_LOG("USIMM_DecodeUsimDFFcp:FCP Pin Tag not found");
        return VOS_ERR;
    }

    ulResult = USIMM_DecodeDFPSDO(&pucData[ulOffset+ulStart+1], pucData[ulOffset+ulStart],pstDfInfo);

    return ulResult;
}


VOS_UINT32 USIMM_DecodeSimDFFcp(VOS_UINT32 ulLen, VOS_UINT8 *pucData, USIMM_DFFCP_ST *pstDfInfo)
{
    USIMM_SIMDF_FCP_ST            stSimDFFcp;

    if(ulLen < sizeof(USIMM_SIMDF_FCP_ST))
    {
        USIMM_ERROR_LOG("USIMM_DecodeSimDFFcp:Input Length Error");

        return VOS_ERR;
    }

    VOS_MemCpy((VOS_UINT8 *)&stSimDFFcp, pucData, sizeof(USIMM_SIMDF_FCP_ST));

    /* coverity[uninit_use] */
    stSimDFFcp.usFileID = ((stSimDFFcp.usFileID<<8)&0xff00)|((stSimDFFcp.usFileID>>8)&0x00ff);

    pstDfInfo->usDFID = stSimDFFcp.usFileID;

    /*�ļ����ʹ���*/
    /* coverity[uninit_use] */
    if ((USIMM_SIM_FILE_TYPE_MF != stSimDFFcp.ucFileType)
     && (USIMM_SIM_FILE_TYPE_DF != stSimDFFcp.ucFileType))
    {
        USIMM_ERROR_LOG("USIMM_DecodeSimDFFcp:File Type Error");

        return VOS_ERR;
    }

    /* coverity[uninit_use] */
    pstDfInfo->ucCharaByte = stSimDFFcp.ucFileChara;

    /*����File Characteristic ����й�CLockֹͣ״̬��Level��Ϣ*/
    USIMM_DecodeCharacter(stSimDFFcp.ucFileChara);

    if(0x00 == (stSimDFFcp.ucFileChara&0x80))    /*��ȡ��ǰ��PIN״̬*/
    {
        pstDfInfo->stPINInfo.enPin1Enable = USIMM_PIN_ENABLED;
    }
    else
    {
        pstDfInfo->stPINInfo.enPin1Enable = USIMM_PIN_DISABLED;
    }

    pstDfInfo->stPINInfo.enPin2Enable = USIMM_PIN_ENABLED;

    /*��ʼ��PINʣ�����*/
    /* coverity[uninit_use] */
    pstDfInfo->stPINInfo.ucPin1RemainTime = stSimDFFcp.ucPIN1RemainTime&0x0F;

    /* coverity[uninit_use] */
    pstDfInfo->stPINInfo.ucPuk1RemainTime = stSimDFFcp.ucPUK1RemainTime&0x0F;
    
    /* coverity[uninit_use] */
    pstDfInfo->stPINInfo.ucPin2RemainTime = stSimDFFcp.ucPIN2RemainTime&0x0F;

    /* coverity[uninit_use] */
    pstDfInfo->stPINInfo.ucPuk2RemainTime = stSimDFFcp.ucPUK2RemainTime&0x0F;

    /*��ʼ��Ĭ������*/
    pstDfInfo->ucPIN1RefNum = 0x01;

    pstDfInfo->ucPIN2RefNum = 0x02;

    pstDfInfo->ucNameLen    = 0;

    pstDfInfo->enSCMode     = USIMM_SC_ATTRIB_NO;

    return VOS_OK;
}


VOS_UINT32 USIMM_DecodeUsimEFFcp(VOS_UINT32 ulLen, VOS_UINT8 *pucData,USIMM_EFFCP_ST *pstEFInfo)
{
    VOS_UINT32 ulStart;
    VOS_UINT32 ulOffset;
    VOS_UINT32 ulDataLen;
    VOS_UINT8  ucTemp;

    ulStart = USIMM_FindTagInSMPTLV(pucData, USIMM_FCP_TAG, ulLen);/*��ѯ��ʼTag*/

    if(USIMM_TAGNOTFOUND == ulStart)
    {
        USIMM_ERROR_LOG("USIMM_DecodeUsimEFFcp:Data Length Error");

        return VOS_ERR;
    }

    ulDataLen = pucData[ulStart];   /*��Ч���ݳ���*/

    ulStart++;                     /*ָ����Ҫ����������ͷ*/

    /*�����ļ������ֶ�*/
    ulOffset = USIMM_FindTagInBERTLV(&pucData[ulStart], USIMM_FCP_FILE_DES_TAG, ulDataLen);

    if(USIMM_TAGNOTFOUND == ulOffset)   /*���ֶα������*/
    {
        USIMM_ERROR_LOG("USIMM_DecodeUsimEFFcp:File Describer not found");

        return VOS_ERR;
    }

    ulOffset += ulStart;

    ucTemp = pucData[ulOffset+1]&0x07;

    if(0x01 == ucTemp) /*�ļ�����TRANSPARENT*/
    {
        pstEFInfo->enFileType = USIMM_EFSTRUCTURE_TRANSPARENT;
    }
    else if(0x02 == ucTemp)/*�ļ�ΪFIXED*/
    {
        pstEFInfo->enFileType       = USIMM_EFSTRUCTURE_FIXED;

        pstEFInfo->ucRecordLen    = pucData[ulOffset+4];

        pstEFInfo->ucRecordNum   = pucData[ulOffset+5];
    }
    else if(0x06 == ucTemp)/*�ļ�ΪCYCLE*/
    {
        pstEFInfo->enFileType = USIMM_EFSTRUCTURE_CYCLE;

        pstEFInfo->ucRecordLen    = pucData[ulOffset+4];

        pstEFInfo->ucRecordNum   = pucData[ulOffset+5];
    }
    else
    {
        USIMM_ERROR_LOG("USIMM_DecodeUsimEFFcp:File Type Error");

        return VOS_ERR;
    }

    ulOffset = USIMM_FindTagInBERTLV(&pucData[ulStart], USIMM_FCP_FID_TAG, ulDataLen);

    if(USIMM_TAGNOTFOUND != ulOffset)
    {
        ulOffset += ulStart + 1;/* [false alarm]:*/
    }
    else
    {
        USIMM_ERROR_LOG("USIMM_DecodeUsimEFFcp:File ID Tag not Found");

        return VOS_ERR;
    }

    /*�����ļ���С*/
    ulOffset = USIMM_FindTagInBERTLV(&pucData[ulStart], USIMM_FCP_FILESIZE_TAG, ulDataLen);

    if(USIMM_TAGNOTFOUND == ulOffset)   /*�ֶα������*/
    {
        USIMM_ERROR_LOG("USIMM_DecodeUsimEFFcp:File Size Tag not Found");

        return VOS_ERR;
    }

    ulOffset += ulStart;

    pstEFInfo->usFileLen = (VOS_UINT16)(((pucData[ulOffset+1]<<0x08)&0xFF00) + pucData[ulOffset+2]);

    /*�����ļ�״̬�ֶ�*/
    ulOffset = USIMM_FindTagInBERTLV(&pucData[ulStart], USIMM_FCP_LCSI_TAG, ulDataLen);

    if(USIMM_TAGNOTFOUND == ulOffset)
    {
        USIMM_ERROR_LOG("USIMM_DecodeUsimEFFcp:Life Cycle Tag not Found");

        return VOS_ERR;
    }

    /* ����b1Ϊ0��b3Ϊ1ʱ���ļ�����DEACTIVE�� */
    if  ((0x0 == (pucData[ulOffset+ulStart+1]&0x01))
        && (0x4 == (pucData[ulOffset+ulStart+1]&0x04)))
    {
        pstEFInfo->enFileStatus = USIMM_EFSTATUS_DEACTIVE;
    }
    else
    {
        pstEFInfo->enFileStatus = USIMM_EFSTATUS_ACTIVE;
    }

    pstEFInfo->enFileReadUpdateFlag = USIMM_EF_NOTREADABLEORUPDATABLE;

    /*�����ļ�����*/
    ulOffset = USIMM_FindTagInBERTLV(&pucData[ulStart], USIMM_FCP_SFI_TAG, ulDataLen);

    if(USIMM_TAGNOTFOUND != ulOffset)
    {
        pstEFInfo->ucSFI = pucData[ulOffset+ulStart+1];
    }

    /*ȷ��EF��ȫ��ʶ����*/
    pstEFInfo->enSCMode  = USIMM_SC_ATTRIB_NO;

    pstEFInfo->usEFArrID = 0;

    pstEFInfo->ucArrNo   = 0;

    ulOffset = USIMM_FindTagInBERTLV(&pucData[ulStart], USIMM_FCP_SA_CF_TAG, ulDataLen);

    if(USIMM_TAGNOTFOUND != ulOffset)
    {
        pstEFInfo->enSCMode = USIMM_SC_ATTRIB_COMPACT;
    }

    ulOffset = USIMM_FindTagInBERTLV(&pucData[ulStart], USIMM_FCP_SA_REF_TAG, ulDataLen);

    if(USIMM_TAGNOTFOUND != ulOffset)
    {
        ulOffset += ulStart;

        pstEFInfo->enSCMode = USIMM_SC_ATTRIB_REFERENCE;

        pstEFInfo->usEFArrID = (VOS_UINT16)((pucData[ulOffset+1]<<0x08)&0xFF00)+pucData[ulOffset+2];

        pstEFInfo->ucArrNo    = pucData[ulOffset+3];
    }

    ulOffset = USIMM_FindTagInBERTLV(&pucData[ulStart], USIMM_FCP_SA_EF_TAG, ulDataLen);

    if(USIMM_TAGNOTFOUND != ulOffset)
    {
        pstEFInfo->enSCMode = USIMM_SC_ATTRIB_EXPANDED;
    }

    return VOS_OK;
}
VOS_UINT32 USIMM_DecodeSimEFFcp(VOS_UINT32 ulLen, VOS_UINT8 *pucData,USIMM_EFFCP_ST *pstEFInfo)
{
    USIMM_SIMEF_FCP_ST          stSimEFFcp;

    if(ulLen < (sizeof(USIMM_SIMEF_FCP_ST) - 1))/*FCP�ĵ�15λ����͸���ļ�����mandatory ��*/
    {
        USIMM_ERROR_LOG("USIMM_DecodeSimEFFcp:Input Length Error");

        return VOS_ERR;
    }

    VOS_MemCpy((VOS_UINT8 *)&stSimEFFcp, pucData, sizeof(USIMM_SIMEF_FCP_ST));

    /* coverity[uninit_use] */
    stSimEFFcp.usFileID = ((stSimEFFcp.usFileID>>8)&0x00ff)|((stSimEFFcp.usFileID<<8)&0xff00);

    /* coverity[uninit_use] */
    stSimEFFcp.usFileLen = ((stSimEFFcp.usFileLen>>8)&0x00ff)|((stSimEFFcp.usFileLen<<8)&0xff00);

    pstEFInfo->usFileLen = stSimEFFcp.usFileLen;/*��ȡ�ļ���С*/

    /* coverity[uninit_use] */
    if(0x04 != stSimEFFcp.ucFileType)/*�ļ����ʹ���*/
    {
        USIMM_ERROR_LOG("USIMM_DecodeSimEFFcp:File Type Error");

        return VOS_ERR;
    }

    /* coverity[uninit_use] */
    pstEFInfo->enFileStatus         = (stSimEFFcp.ucFileStatus&0x01);/*��ȡ�ļ�״̬*/

    pstEFInfo->enFileReadUpdateFlag = (stSimEFFcp.ucFileStatus&0x04);/*��ȡ�ļ���ȡ�͸��±�־*/

    /* coverity[uninit_use] */
    if(0x00 == stSimEFFcp.ucEFStruct)   /*�ļ�ΪTRANSPARENT*/
    {
        pstEFInfo->enFileType  = USIMM_EFSTRUCTURE_TRANSPARENT;
    }
    else if(0x01 == stSimEFFcp.ucEFStruct)  /*�ļ�����ΪFIXED*/
    {
        pstEFInfo->enFileType  = USIMM_EFSTRUCTURE_FIXED;

        /* coverity[uninit_use] */
        pstEFInfo->ucRecordLen = stSimEFFcp.ucRecordLen;

        pstEFInfo->ucRecordNum = (VOS_UINT8)(pstEFInfo->usFileLen/pstEFInfo->ucRecordLen);
    }
    else if(0x03 == stSimEFFcp.ucEFStruct)  /*�ļ�����ΪCYCLE*/
    {
        pstEFInfo->enFileType  = USIMM_EFSTRUCTURE_CYCLE;

        /* coverity[uninit_use] */
        pstEFInfo->ucRecordLen = stSimEFFcp.ucRecordLen;

        pstEFInfo->ucRecordNum = (VOS_UINT8)(pstEFInfo->usFileLen/pstEFInfo->ucRecordLen);
    }
    else    /*�ļ����ʹ���*/
    {
        USIMM_WARNING_LOG("USIMM_DecodeSimEFFcp: EF Struct Error");

        return VOS_ERR;
    }

    /*��ʼ����������*/
    pstEFInfo->enSCMode  = USIMM_SC_ATTRIB_NO;

    pstEFInfo->usEFArrID = 0;

    pstEFInfo->ucArrNo   = 0;

    pstEFInfo->ucSFI     = 0;

    return VOS_OK;
}


VOS_UINT32 USIMM_DecodeEFFcp(
    USIMM_CARD_TYPE_ENUM_UINT32         enCurCardType,
    USIMM_APDU_ST                      *pstApduData,
    USIMM_CURFILEINFO_ST               *pstCurFileInfo)
{
    VOS_UINT32 ulResult;

    USIMM_ClearCurEFInfo(pstCurFileInfo);

    if(USIMM_CARD_USIM == enCurCardType)  /*��ǰΪUSIM*/
    {
        ulResult = USIMM_DecodeUsimEFFcp((VOS_UINT8)pstApduData->ulRecDataLen, pstApduData->aucRecvBuf, &pstCurFileInfo->stEFInfo);
    }
    else
    {
        ulResult = USIMM_DecodeSimEFFcp((VOS_UINT8)pstApduData->ulRecDataLen, pstApduData->aucRecvBuf, &pstCurFileInfo->stEFInfo);
    }

    if(VOS_OK != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_DecodeEFFcp:Decode FCP error");

        VOS_MemSet(&pstCurFileInfo->stEFInfo, 0, sizeof(USIMM_EFFCP_ST));

        return USIMM_SW_ERR;
    }

    return ulResult;
}
VOS_UINT32 USIMM_DecodeDFFcp(
    USIMM_CARD_TYPE_ENUM_UINT32         enCurCardType,
    USIMM_APDU_ST                      *pstApduData,
    USIMM_CURFILEINFO_ST               *pstCurFileInfo)
{
    VOS_UINT32 ulResult;

    USIMM_ClearCurDFInfo(pstCurFileInfo);

    if(USIMM_CARD_USIM == enCurCardType)  /*��ǰΪUSIM*/
    {
        ulResult = USIMM_DecodeUsimDFFcp(pstApduData->ulRecDataLen, pstApduData->aucRecvBuf, &pstCurFileInfo->stDFInfo);
    }
    else
    {
        ulResult = USIMM_DecodeSimDFFcp(pstApduData->ulRecDataLen, pstApduData->aucRecvBuf, &pstCurFileInfo->stDFInfo);
    }

    if(VOS_OK != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_DecodeDFFcp:Decode Fcp error");

        return USIMM_SW_ERR;
    }

    pstCurFileInfo->usCurDFFcpLen = (VOS_UINT16)pstApduData->ulRecDataLen;

    return ulResult;
}
VOS_UINT32 USIMM_DecodeFileFcp(USIMM_CARD_TYPE_ENUM_UINT32     enCardType,
                                        USIMM_U16_LVDATA_STRU          *pstFilePath,
                                        USIMM_APDU_ST                  *pstApduData,
                                        USIMM_CURFILEINFO_ST           *pstCurFileInfo)
{
    VOS_UINT32              ulResult = VOS_ERR;
    VOS_UINT16              usFileId;

    if(pstFilePath->ulDataLen < 1)
    {
        USIMM_ERROR_LOG("USIMM_DecodeFileFcp: file path len is 0");

        return VOS_ERR;
    }

    usFileId = pstFilePath->pusData[pstFilePath->ulDataLen-1];

    if (USIMM_IS_EF_FILE(usFileId))/* ����EF�ļ�FCP */
    {
        ulResult = USIMM_DecodeEFFcp(enCardType, pstApduData, pstCurFileInfo);
    }
    else/* ����DF�ļ�FCP */
    {
        ulResult = USIMM_DecodeDFFcp(enCardType, pstApduData, pstCurFileInfo);
    }

    if (VOS_OK != ulResult)
    {
        USIMM_ERROR_LOG("USIMM_DecodeFileFcp: Fail to Decode FCP");
    }

    return ulResult;
}


VOS_UINT32 USIMM_DecodeEFDIR(VOS_VOID)
{
    VOS_UINT32                          ulResult;
    VOS_UINT32                          ulAidLen;
    VOS_UINT32                          i;
    VOS_UINT8                          *pucdata;
    VOS_UINT8                           aucUsimRidAppCode[USIMM_RID_LEN + USIMM_APP_CODE_LEN] = {0xA0,0x00,0x00,0x00,0x87,0x10,0x02};
    VOS_UINT8                           aucIsimRidAppCode[USIMM_RID_LEN + USIMM_APP_CODE_LEN] = {0xA0,0x00,0x00,0x00,0x87,0x10,0x04};
    VOS_UINT8                           aucCsimRidAppCode[USIMM_RID_LEN + USIMM_APP_CODE_LEN] = {0xA0,0x00,0x00,0x03,0x43,0x10,0x02};
    VOS_UINT16                          ausPath[] = {MF, EFDIR};
    VOS_BOOL                            ulUsimAidHit = VOS_FALSE;

    ulResult = USIMM_SelectFile(USIMM_UMTS_APP, USIMM_NEED_FCP, ARRAYSIZE(ausPath), ausPath);

    if(VOS_OK != ulResult)/*ѡ���ļ�*/
    {
        USIMM_ERROR_LOG("USIMM_DecodeEFDIR: EFDIR could not Found");/*��ӡ����*/

        return VOS_ERR;
    }

    pucdata = (VOS_UINT8 *)VOS_MemAlloc(WUEPS_PID_USIM,DYNAMIC_MEM_PT,gstUSIMMCurFileInfo.stEFInfo.usFileLen);

    if(VOS_NULL_PTR == pucdata)   /*�жϽ��*/
    {
        USIMM_ERROR_LOG("USIMM_DecodeEFDIR: VOS_MemAlloc Error");/*��ӡ����*/

        return VOS_ERR;
    }

    if(USIMM_EFSTRUCTURE_FIXED == gstUSIMMCurFileInfo.stEFInfo.enFileType)
    {
        if (USIMM_SW_OK != USIMM_ReadLFFile(&gstUSIMMCurFileInfo.stEFInfo, USIMM_READ_ALLRECORD, pucdata))      /*��ȡ��¼����*/
        {
            USIMM_INFO_LOG("USIMM_DecodeEFDIR: USIMM_ReadLFFile Error");
        }
    }
    else
    {
        USIMM_ERROR_LOG("USIMM_DecodeEFDIR: EFDIR File Type Error");/*��ӡ����*/

        VOS_MemFree(WUEPS_PID_USIM, pucdata);

        return VOS_ERR;                                              /*����102.221 13.1 �ڲ�֧�ֶ����Ƹ�ʽ�ļ�*/
    }

    ulResult = VOS_ERR;

    for(i=0; i<gstUSIMMCurFileInfo.stEFInfo.usFileLen; i+=gstUSIMMCurFileInfo.stEFInfo.ucRecordLen)
    {
        if ( (APP_TMP_TAG != pucdata[i + APP_TMP_TAG_OFFSET])
          || (APP_IND_TAG != pucdata[i + APP_IND_TAG_OFFSET]))    /*�ж����ݺϷ���*/
        {
            USIMM_WARNING_LOG("USIMM_DecodeEFDIR: EFDIR Content not Right");/*��ӡ����*/

            continue;
        }

        ulAidLen = (pucdata[i + APP_AID_LEN_OFFSET] > USIMM_AID_LEN_MAX)?USIMM_AID_LEN_MAX:pucdata[i + APP_AID_LEN_OFFSET];

        /* ֻҪƥ�䵽A0000000871002����Ϊ�ҵ���USIM��AID */
        if ((VOS_OK == VOS_MemCmp(pucdata + i + APP_AID_VALUE_OFFSET, aucUsimRidAppCode, sizeof(aucUsimRidAppCode)))
            && (VOS_FALSE == ulUsimAidHit))
        {

            VOS_MemCpy(gstUSIMMADFInfo.aucAID, pucdata + i + APP_AID_VALUE_OFFSET, ulAidLen);    /*���������Ϣ*/

            gstUSIMMADFInfo.ulAIDLen = ulAidLen;

            ulResult = VOS_OK;

            g_astAidInfo[USIMM_AID_TYPE_USIM].ulAIDLen = ulAidLen;
            VOS_MemCpy(g_astAidInfo[USIMM_AID_TYPE_USIM].aucAID, pucdata + i + APP_AID_VALUE_OFFSET, ulAidLen);    /*���������Ϣ*/
			
            ulUsimAidHit = VOS_TRUE;
        }

        /* ֻҪƥ�䵽A0000000871004����Ϊ�ҵ���ISIM��AID */
        if (VOS_OK == VOS_MemCmp(pucdata + i + APP_AID_VALUE_OFFSET, aucIsimRidAppCode, sizeof(aucIsimRidAppCode)))
        {
            g_astAidInfo[USIMM_AID_TYPE_ISIM].ulAIDLen = ulAidLen;
            VOS_MemCpy(g_astAidInfo[USIMM_AID_TYPE_ISIM].aucAID, pucdata + i + APP_AID_VALUE_OFFSET, ulAidLen);    /*���������Ϣ*/
        }

        /* ֻҪƥ�䵽A0000003431002����Ϊ�ҵ���CSIM��AID */
        if (VOS_OK == VOS_MemCmp(pucdata + i + APP_AID_VALUE_OFFSET, aucCsimRidAppCode, sizeof(aucCsimRidAppCode)))
        {
            g_astAidInfo[USIMM_AID_TYPE_CSIM].ulAIDLen = ulAidLen;
            VOS_MemCpy(g_astAidInfo[USIMM_AID_TYPE_CSIM].aucAID, pucdata + i + APP_AID_VALUE_OFFSET, ulAidLen);    /*���������Ϣ*/
        }
    }

    VOS_MemFree(WUEPS_PID_USIM, pucdata);

    g_stUSIMMCardVersionType.enCardMode     = USIMM_CARD_UICC_TYPE;
    g_stUSIMMCardVersionType.ucHasCModule   = (VOS_NULL != g_astAidInfo[USIMM_AID_TYPE_CSIM].ulAIDLen);
    g_stUSIMMCardVersionType.ucHasWGModule  = (VOS_NULL != g_astAidInfo[USIMM_AID_TYPE_USIM].ulAIDLen);

    /*����Ҫ������ܹ�֧��CDMAӦ��*/
    if(VOS_TRUE == USIMM_IsCLEnable())
    {
        if ((VOS_NULL == g_astAidInfo[USIMM_AID_TYPE_USIM].ulAIDLen)&&(VOS_NULL == g_astAidInfo[USIMM_AID_TYPE_CSIM].ulAIDLen))
        {
            return VOS_ERR;
        }

        if ((VOS_NULL == g_astAidInfo[USIMM_AID_TYPE_USIM].ulAIDLen)||(VOS_NULL == g_astAidInfo[USIMM_AID_TYPE_CSIM].ulAIDLen))
        {
            gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType = USIMM_CARD_NOCARD;

            return USIMM_INIT_FATAL_EXIT;
        }
    }

    return ulResult;
}
VOS_VOID USIMM_ReportEvent(VOS_UINT16 usEventId)
{
    PS_OM_EVENT_IND_STRU stEventInd;

    stEventInd.ulLength   = sizeof(PS_OM_EVENT_IND_STRU) - sizeof(stEventInd.ulLength);

    stEventInd.usEventId  = (VOS_UINT16)usEventId;

    stEventInd.ulModuleId = WUEPS_PID_MM;

    (VOS_VOID)OM_Event(&stEventInd);

    return;
}


VOS_UINT32 USIMM_RefrehFilePath(VOS_UINT8 *pucDstPath,
                                       VOS_UINT16 *pusDstLen,
                                       VOS_UINT8 *pucSrcPath,
                                       VOS_UINT16 usSrcLen)
{
    VOS_UINT16 usLen = 0;
    VOS_UINT8  ucPreDfFlag = VOS_FALSE;
    VOS_UINT8  ucPreDfType = MFTAG;

    switch (pucSrcPath[0])
    {
        case EFUNDERGRANDADF:
            ucPreDfType = DFUNDERDF;
            break;
        case EFUNDERMF:
        case DFUNDERMF:
            ucPreDfType = MFTAG;
            break;
        case DFUNDERDF:
        case EFUNDERDF:
            ucPreDfType = DFUNDERMF;
            break;
        case MFTAG:
            usLen = *pusDstLen;
            break;
        default:
            USIMM_ERROR_LOG("USIMM_RefrehFilePath: Unknown File Type");
            return VOS_ERR;
    }

    while (usLen < *pusDstLen)
    {
        if (ucPreDfType == pucDstPath[usLen])
        {
            usLen += sizeof(VOS_UINT16);
            ucPreDfFlag = VOS_TRUE;
            break;
        }

        usLen += sizeof(VOS_UINT16);
    }

    if (VOS_FALSE == ucPreDfFlag)
    {
        VOS_MemCpy(pucDstPath, pucSrcPath, usSrcLen);
        *pusDstLen = usSrcLen;
    }
    else
    {
        if ((usSrcLen + usLen) > USIMM_MAX_PATH_LEN*2)
        {
            return VOS_ERR;
        }

        VOS_MemCpy(pucDstPath+usLen, pucSrcPath, usSrcLen);

        *pusDstLen = usSrcLen + usLen;
    }

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : USIMM_CardFilePathCMP
 ��������  :
 �������  :
 �������  : ��
 �� �� ֵ  : ��
 �޸���ʷ  : zhuli
             2010-03-19
*****************************************************************************/
VOS_UINT32 USIMM_CardFilePathCMP(VOS_UINT32 ulPath1Len, VOS_UINT16 *pusPath1,VOS_UINT32 ulPath2Len, VOS_UINT16 *pusPath2)
{
    if(ulPath2Len != ulPath1Len)
    {
        return VOS_ERR;
    }

    if(VOS_OK != VOS_MemCmp(pusPath1, pusPath2, ulPath1Len*sizeof(VOS_UINT16)))
    {
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT16 USIMM_UsimEFIDToIsimEFID(VOS_UINT16 usFileID)
{
    VOS_UINT32                           i;

    /* ����USIM�ļ�ID���Ҷ�Ӧ��ISIM�ļ�ID */
    for (i = 0; i < (sizeof(gastIsimUsimEFConTBL)/sizeof(USIMM_ISIM_USIM_CONVERSION_ST)); i++)
    {
        if (gastIsimUsimEFConTBL[i].usUsimEFID == usFileID)
        {
            return gastIsimUsimEFConTBL[i].usIsimEFID;
        }
    }

    /* �Ҳ����򷵻ؿ� */
    return VOS_NULL_WORD;
}


VOS_UINT16 USIMM_IsimEFIDToUsimEFID(VOS_UINT16 usFileID)
{
    VOS_UINT32                           i;

    /* ����ISIM�ļ�ID���Ҷ�Ӧ��USIM�ļ�ID */
    for (i = 0; i < sizeof(gastIsimUsimEFConTBL)/sizeof(USIMM_ISIM_USIM_CONVERSION_ST); i++)
    {
        if (gastIsimUsimEFConTBL[i].usIsimEFID == usFileID)
        {
            return gastIsimUsimEFConTBL[i].usUsimEFID;
        }
    }

    /* �Ҳ����򷵻ؿ� */
    return VOS_NULL_WORD;
}


VOS_UINT8 USIMM_GetMncLen(VOS_UINT8 *pucAdContent, VOS_UINT32 ulFileSize)
{
    VOS_UINT8                           ucHplmnMncLen;

    ucHplmnMncLen = USIMM_MNC_TWO_BYTES_LEN;

    /* �ļ���С��С����Ĭ��MNCΪ2�ֽڳ��� */
    if(USIMM_EF_AD_MIN_LEN >= ulFileSize)
    {
        return ucHplmnMncLen;
    }

    /* �ж�MNC����,MNC����Ϊ2λ��3λ */
    if ((USIMM_MNC_TWO_BYTES_LEN == (pucAdContent[USIMM_AD_MNC_LEN_POS] & 0x0F))
        || (USIMM_MNC_THREE_BYTES_LEN == (pucAdContent[USIMM_AD_MNC_LEN_POS] & 0x0F)))
    {
        ucHplmnMncLen = pucAdContent[USIMM_AD_MNC_LEN_POS] & 0x0F;
    }

    return ucHplmnMncLen;
}


VOS_VOID USIMM_BcdNumToAsciiNum(VOS_UINT8 *pucAsciiNum, VOS_UINT8 *pucBcdNum, VOS_UINT8 ucBcdNumLen)
{
    VOS_UINT8       ucTmp;
    VOS_UINT8       ucFirstNumber;
    VOS_UINT8       ucSecondNumber;

    for (ucTmp = 0; ucTmp < ucBcdNumLen; ucTmp++)
    {
        ucFirstNumber = (VOS_UINT8)((*(pucBcdNum + ucTmp)) & 0x0f);             /*low four bits*/
        ucSecondNumber = (VOS_UINT8)(((*(pucBcdNum + ucTmp)) >> 4) & 0x0f);   /*high four bits*/
        if(ucFirstNumber <= 9)
        {
            *pucAsciiNum = ucFirstNumber + 0x30;
            pucAsciiNum++;
        }
        else if(0xa == ucFirstNumber)            /* the key is '*' */
        {
            *pucAsciiNum = 0x2a;
            pucAsciiNum++;
        }
        else if(0xb == ucFirstNumber)            /* the key is '#' */
        {
            *pucAsciiNum = 0x23;
            pucAsciiNum++;
        }
        else                                     /* the key is 'a','b'or 'c' */
        {
            *pucAsciiNum = ucFirstNumber + 0x57;
            pucAsciiNum++;
        }

        /* the following proceed the second number */
        if(0xf == ucSecondNumber)
        {
            break;
        }
        if(ucSecondNumber <= 9)
        {
            *pucAsciiNum = ucSecondNumber + 0x30;
            pucAsciiNum++;
        }
        else if(0xa == ucSecondNumber)            /* the key is '*' */
        {
            *pucAsciiNum = 0x2a;
            pucAsciiNum++;
        }
        else if(0xb == ucSecondNumber)            /* the key is '#' */
        {
            *pucAsciiNum = 0x23;
            pucAsciiNum++;
        }
        else                                     /* the key is 'a','b'or 'c' */
        {
            *pucAsciiNum = ucSecondNumber + 0x57;
            pucAsciiNum++;
        }

    }

    return;
}
VOS_UINT32 USIMM_ImsiBcd2Ascii(VOS_UINT8 *pucAsciiStr)
{
    VOS_UINT32                          ulI = 0;
    VOS_UINT8                           ucTmp;
    VOS_UINT8                           aucImsi[USIMM_EF_IMSI_LEN];

    if (USIMM_API_SUCCESS != USIMM_GetCardIMSI(aucImsi))
    {
        USIMM_ERROR_LOG("USIMM_DecodeImsi: USIMM_GetCardIMSI fail.");
        return USIMM_SW_ERR;
    }

    /* �ж�IMSI�����Ƿ�Ϸ� */
    /* coverity[uninit_use] */
    if (USIMM_IMSI_MAX_LEN < aucImsi[0])
    {
        USIMM_ERROR_LOG("USIMM_DecodeImsi: IMSI length is error.");
        return USIMM_SW_ERR;
    }

    /*�����ƶ��û�ʶ����*/
    /*��մ���ַ����Ļ�����*/
    for ( ulI = 0; ulI < (VOS_UINT32)(aucImsi[0]*2); ulI++ )
    {
        pucAsciiStr[ulI] = 0;
    }
    ucTmp = (aucImsi[1] >> 4) & 0x0f;

    /* ȡ����һλBCD�����ת�� */
    USIMM_BcdNumToAsciiNum(pucAsciiStr, &ucTmp, 1);

    /* ������Bcdת��ΪAscii�� */
    USIMM_BcdNumToAsciiNum(pucAsciiStr + 1, aucImsi + 2, (VOS_UINT8)((aucImsi[0]*2) - 1)/2);

    return VOS_OK;
}


VOS_VOID USIMM_GetAsciiMCCAndMNC(VOS_UINT8 *pucAsciiImsi, VOS_UINT8 *pucAsciiMCC, VOS_UINT8 *pucAsciiMNC)
{
    VOS_UINT8                           ucMncLen;
    VOS_UINT32                          ulDataLen;
    VOS_UINT8                           *pucData = VOS_NULL_PTR;

    /* ��AD�ļ��У���ȡHPLMN MNC���� */
    if (USIMM_API_SUCCESS != USIMM_GetCachedFile(EFAD, &ulDataLen, &pucData, USIMM_UMTS_APP))
    {
       USIMM_ERROR_LOG("USIMM_DecodeImsi: USIMM_GetCachedFile fail.");
       return;
    }

    /* ��ȡMNC���� */
    ucMncLen = USIMM_GetMncLen(pucData, ulDataLen);

    /* ��ȡMCC */
    pucAsciiMCC[0] = pucAsciiImsi[0];
    pucAsciiMCC[1] = pucAsciiImsi[1];
    pucAsciiMCC[2] = pucAsciiImsi[2];

    /* ��ȡMNC */
    if (USIMM_MNC_TWO_BYTES_LEN == ucMncLen)
    {
        pucAsciiMNC[0] = '0';
        pucAsciiMNC[1] = pucAsciiImsi[3];
        pucAsciiMNC[2] = pucAsciiImsi[4];
    }
    else
    {
        pucAsciiMNC[0] = pucAsciiImsi[3];
        pucAsciiMNC[1] = pucAsciiImsi[4];
        pucAsciiMNC[2] = pucAsciiImsi[5];
    }

    return;
}


VOS_UINT32 USIMM_IsSIMServiceAvailable(UICC_SERVICES_TYPE_ENUM_UINT32 enService)
{
    VOS_UINT32                          ulResult;
    VOS_UINT32                          ulFileNum;
    VOS_UINT8                           ucBitNo;

    if ((USIMM_CARD_SIM != gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
        || (USIMM_CARD_SERVIC_AVAILABLE != gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService))
    {
        USIMM_WARNING_LOG("USIMM_IsSIMServiceAvailable: The Card Type is Wrong");

        return PS_USIM_SERVICE_NOT_AVAILIABLE;
    }

    /* ��ѯ��ǰ���ļ����� */
    ulResult = USIMM_PoolFindFile(EFSST, &ulFileNum, USIMM_UNLIMIT_APP);

    if(ulResult != USIMM_API_SUCCESS)
    {
        USIMM_WARNING_LOG("USIMM_IsServiceAvailable: Read File is Failed");

        return PS_USIM_SERVICE_NOT_AVAILIABLE;
    }

    ucBitNo = (VOS_UINT8)((enService * 2) - 1);

    /* ��ȡ�����Bit��Ϣ */
    ulResult = USIMM_GetBitFromBuf(gstUSIMMPOOL.astpoolRecord[ulFileNum].pucContent,
                                    ucBitNo,
                                    gstUSIMMPOOL.astpoolRecord[ulFileNum].usLen);

    /* SIM�����ж�ACTIVEλ */
    if (PS_USIM_SERVICE_AVAILIABLE == ulResult)
    {
        ulResult = USIMM_GetBitFromBuf(gstUSIMMPOOL.astpoolRecord[ulFileNum].pucContent,
                                        (ucBitNo+1),
                                        gstUSIMMPOOL.astpoolRecord[ulFileNum].usLen);
    }

    if(USIMM_BITNOFOUNE == ulResult)
    {
        USIMM_WARNING_LOG("USIMM_IsServiceAvailable: The Service is Out of File");

        return PS_USIM_SERVICE_NOT_AVAILIABLE;
    }

    return ulResult;
}
VOS_UINT32 USIMM_IsUSIMServiceAvailable(UICC_SERVICES_TYPE_ENUM_UINT32 enService)
{
    VOS_UINT32                          ulResult;
    VOS_UINT32                          ulFileNum;
    VOS_UINT8                           ucBitNo;

    if (((USIMM_CARD_USIM != gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType)
        && (USIMM_CARD_ROM_SIM != gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardType))
        || (USIMM_CARD_SERVIC_AVAILABLE != gastUSIMMCardAppInfo[USIMM_UICC_USIM].enCardService))
    {
        USIMM_WARNING_LOG("USIMM_IsUSIMServiceAvailable: The Card Type is Wrong");

        return PS_USIM_SERVICE_NOT_AVAILIABLE;
    }

    /* ��ѯ��ǰ���ļ����� */
    ulResult = USIMM_PoolFindFile(EFUST, &ulFileNum, USIMM_UNLIMIT_APP);

    if(ulResult != USIMM_API_SUCCESS)
    {
        USIMM_WARNING_LOG("USIMM_IsUSIMServiceAvailable: Read File is Failed");

        return PS_USIM_SERVICE_NOT_AVAILIABLE;
    }

    ucBitNo = (VOS_UINT8)((enService - SIM_SVR_BUTT) + 1);

    /* ��ȡ�����Bit��Ϣ */
    ulResult = USIMM_GetBitFromBuf(gstUSIMMPOOL.astpoolRecord[ulFileNum].pucContent,
                                    ucBitNo,
                                    gstUSIMMPOOL.astpoolRecord[ulFileNum].usLen);

    if(USIMM_BITNOFOUNE == ulResult)
    {
        USIMM_WARNING_LOG("USIMM_IsUSIMServiceAvailable: The Service is Out of File");

        return PS_USIM_SERVICE_NOT_AVAILIABLE;
    }

    return ulResult;

}


VOS_UINT32 USIMM_IsISIMServiceAvailable(UICC_SERVICES_TYPE_ENUM_UINT32 enService)
{
    VOS_UINT32                          ulResult;
    VOS_UINT8                           ucBitNo;

    /* ����û����ʵ��ISIM����Ҫ��ȡUSIM���е�UST��ʵ�� */
    if ((USIMM_CARD_ISIM != gastUSIMMCardAppInfo[USIMM_UICC_ISIM].enCardType)
        || ((USIMM_CARD_SERVIC_AVAILABLE != gastUSIMMCardAppInfo[USIMM_UICC_ISIM].enCardService)))
    {
        USIMM_WARNING_LOG("USIMM_IsISIMServiceAvailable: The Card Type is Wrong");

        return PS_USIM_SERVICE_NOT_AVAILIABLE;
    }

    /* SM-over-IP ��Ӧ��USIM�ϵ����������� */
    if (ISIM_SVR_SOI == enService)
    {
        ulResult = USIMM_IsUSIMServiceAvailable(USIM_SVR_DATA_DL_SMSPP);

        if (PS_USIM_SERVICE_NOT_AVAILIABLE == ulResult)
        {
            return PS_USIM_SERVICE_NOT_AVAILIABLE;
        }

        ulResult = USIMM_IsUSIMServiceAvailable(USIM_SVR_SM_VOER_IP);
    }
    else
    {
        /* ����USIM�϶�ӦISIM����λλ�� */
        ucBitNo = g_aucServIsimToUsim[enService - USIM_SVR_BUTT];

        /* û�ж�Ӧ����λ�򷵻ز�֧�� */
        if (0 == ucBitNo)
        {
            return PS_USIM_SERVICE_NOT_AVAILIABLE;
        }

        ulResult = USIMM_IsUSIMServiceAvailable(ucBitNo);
    }

    return ulResult;
}


VOS_UINT32 USIMM_ReadFile(USIMM_EFFCP_ST               *pstCurEFFcp,
                                 VOS_UINT8                   ucRecordNum,
                                 VOS_UINT16                  usDataLen,
                                 VOS_UINT8                   *pucData)
{
    VOS_UINT32                          ulResult;

    switch(pstCurEFFcp->enFileType)
    {
        case USIMM_EFSTRUCTURE_TRANSPARENT:       /*��ȡ�������ļ�*/
            ulResult = USIMM_ReadTFFile(usDataLen, pucData);
            break;

        case USIMM_EFSTRUCTURE_FIXED:                   /*��ȡ��¼�ļ�*/
        case USIMM_EFSTRUCTURE_CYCLE:
            ulResult = USIMM_ReadLFFile(pstCurEFFcp, ucRecordNum, pucData);
            break;

        default:
            USIMM_WARNING_LOG("USIMM_GetFileHandle: The File Type is Not Support");
            ulResult = USIMM_SW_OTHER_ERROR;
            break;
    }

    return ulResult;
}


VOS_UINT32 USIMM_GetReadFilePara(VOS_UINT8 ucRecordNum, USIMM_EFFCP_ST *pstEFInfo, USIMM_GETCNF_INFO_STRU *pstCnfInfo)
{
    VOS_UINT32                          ulResult = USIMM_SW_OK;

    switch(pstEFInfo->enFileType)
    {
        case USIMM_EFSTRUCTURE_TRANSPARENT:       /*��ȡ�������ļ�*/
            pstCnfInfo->usDataLen   = (pstEFInfo->usFileLen > USIMM_MAX_FILE_SIZE) ? USIMM_MAX_FILE_SIZE : pstEFInfo->usFileLen;

            pstCnfInfo->usEfLen     = pstCnfInfo->usDataLen;                                /*��ȡ���Ⱥ��ļ�����һ��*/

            break;

        case USIMM_EFSTRUCTURE_FIXED:                   /*��ȡ��¼�ļ�*/
        case USIMM_EFSTRUCTURE_CYCLE:
            if(ucRecordNum > pstEFInfo->ucRecordNum)/*����ȡ�ļ�¼���Ƿ�Խ��*/
            {
                USIMM_ERROR_LOG("USIMM_GetReadFilePara: The Record Index is Out of the File Range");

                ulResult = USIMM_SW_OUTOF_RANGE;

                break;
            }

            pstCnfInfo->usEfLen = pstEFInfo->ucRecordLen;     /*��ʹ��ȡȫ����¼Ҳ��Ϊ��¼����*/

            pstCnfInfo->ucRecordNum = ucRecordNum;

            if(USIMM_READ_ALLRECORD == ucRecordNum)
            {
                pstCnfInfo->ucTotalNum = pstEFInfo->ucRecordNum;

                pstCnfInfo->usDataLen  = pstEFInfo->usFileLen;
            }
            else
            {
                pstCnfInfo->ucTotalNum = 1;

                pstCnfInfo->usDataLen  = pstEFInfo->ucRecordLen;
            }

            break;

        default:
            USIMM_WARNING_LOG("USIMM_GetReadFilePara: The File Type is Not Support");

            ulResult = USIMM_SW_OTHER_ERROR;

            break;
    }

    return ulResult;
}
VOS_VOID USIMM_CardStatusRegCbFuncProc(VOS_VOID)
{
    NVIM_USIM_CARD_STATUS_CB_TYPE_STRU  stType;

    if (NV_OK != NV_Read(en_NV_Item_Card_Status_Cb_Type, &stType, sizeof(NVIM_USIM_CARD_STATUS_CB_TYPE_STRU)))
    {
        stType.enType = USIMM_CARD_STATUS_REG_TYPE_M2M;
    }

    if (USIMM_CARD_STATUS_REG_TYPE_M2M == stType.enType)
    {
        DRV_USIMMSCI_FUNC_REGISTER(USIMM_SciCardStatusIndCbFunc);
    }
    else if (USIMM_CARD_STATUS_REG_TYPE_E5 == stType.enType)
    {
        DRV_USIMMSCI_FUNC_REGISTER(USIMM_SCICardOutNotify);
    }
    else
    {
        return;
    }

    return;
}


VOS_BOOL USIMM_IsCLEnable(VOS_VOID)
{
#if defined (INSTANCE_1)
    return VOS_FALSE;
#else
    TAF_NV_LC_CTRL_PARA_STRU            stNVData;

    if (NV_OK != NV_Read(en_NV_Item_LC_Ctrl_PARA, &stNVData, sizeof(TAF_NV_LC_CTRL_PARA_STRU)))
    {
        return VOS_FALSE;
    }

    if (VOS_TRUE == stNVData.ucLCEnableFlg)
    {
        return VOS_TRUE;
    }
    else
    {
        return VOS_FALSE;
    }
#endif
}


TAF_NVIM_LC_RAT_COMBINED_ENUM_UINT8 USIMM_GetRATMode(VOS_VOID)
{
#if defined (INSTANCE_1)
    return TAF_NVIM_LC_RAT_COMBINED_GUL;
#else
    TAF_NV_LC_CTRL_PARA_STRU    stNVData;

    if (NV_OK != NV_Read(en_NV_Item_LC_Ctrl_PARA, &stNVData, sizeof(TAF_NV_LC_CTRL_PARA_STRU)))
    {
        return TAF_NVIM_LC_RAT_COMBINED_GUL;
    }

    if ((TAF_NVIM_LC_RAT_COMBINED_CL == stNVData.enRatCombined)&&(VOS_TRUE == stNVData.ucLCEnableFlg))
    {
        return TAF_NVIM_LC_RAT_COMBINED_CL;
    }
    else
    {
        return TAF_NVIM_LC_RAT_COMBINED_GUL;
    }
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */




