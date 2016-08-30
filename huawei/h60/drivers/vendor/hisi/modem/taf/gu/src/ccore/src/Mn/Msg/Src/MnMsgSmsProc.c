

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include  "MnMsgInclude.h"
#include "siapppb.h"
#include "NVIM_Interface.h"
#include "MnMsgCtx.h"
#include "MnMsgProcNvim.h"

#include "NasGmmInterface.h"

#include "NasUsimmApi.h"

#include "MmCmInterface.h"

#include "TafSdcCtx.h"
#include "MnMsgSendSpm.h"
#include "TafStdlib.h"

#if (FEATURE_ON == FEATURE_LTE)
#include "LmmSmsInterface.h"
#endif

#ifdef  __cplusplus
  #if  __cplusplus
  extern "C"{
  #endif
#endif


/*****************************************************************************
  2 ��������
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_MNMSG_SMSPROC_C
#define MN_MSG_ACTIVE_MESSAGE_ENDSTR_NUM                    (3)
#define MN_MSG_ACTIVE_MESSAGE_ENDSTR_MAX_LEN                (3)
/*****************************************************************************
  3 ��������
*****************************************************************************/
LOCAL MN_MSG_TS_DATA_INFO_STRU         f_stMsgTsData;
VOS_UINT8                              g_ucMnMsgAutoReply = 0;
LOCAL VOS_UINT32                       f_ulMsgMoNum = 0;
LOCAL VOS_UINT32                       f_ulMsgMtNum = 0;
#if ( VOS_WIN32 == VOS_OS_VER )
VOS_UINT8                               g_ucSetEfSmssClear = 0;
#endif
/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


LOCAL VOS_VOID  MN_MSG_GetCurrentLineLen(
    VOS_UINT8                           *pucOrgAddr,
    VOS_UINT32                          ulOrgLen,
    MN_MSG_MSG_CODING_ENUM_U8           enMsgCoding,
    VOS_UINT32                          *pulCurrentLineLen,
    VOS_UINT32                          *pulLineLenWithOutEndFlag
)
{
    VOS_UINT32                          ulLoop;
    VOS_UINT8                           aucLineEndStr[MN_MSG_ACTIVE_MESSAGE_ENDSTR_NUM][MN_MSG_ACTIVE_MESSAGE_ENDSTR_MAX_LEN]
                                            = {"\r\n", "\r", "\n"};
    VOS_UINT32                          ulLineLen;
    VOS_UINT8                           *pucEndFlagStr;
    VOS_BOOL                            bEqualFlag;
    VOS_INT8                            cRet;
    VOS_UINT32                          ulLineEndStrLen;

    /*���������ʼ��: �����������򲻰������������Ⱦ���ֵΪ�����ַ������ܳ���*/
    *pulCurrentLineLen          = ulOrgLen;
    *pulLineLenWithOutEndFlag   = ulOrgLen;

    /*ѭ��������ʼ��: �������׵�ַָ�������ַ����׵�ַ���г��ȸ�ֵΪ�����ַ������ܳ���*/
    pucEndFlagStr = pucOrgAddr;
    ulLineLen     = ulOrgLen;

    if (MN_MSG_MSG_CODING_UCS2 == enMsgCoding)
    {
        while (0 != ulLineLen)
        {
            /*UCS2����: ����Ƿ�����н�����<CR><LF>, <CR>, <LF>*/
            for (ulLoop = 0; ulLoop < MN_MSG_ACTIVE_MESSAGE_ENDSTR_NUM; ulLoop++)
            {
                MN_CmpAsciiStrAndUcs2StrCaseInsensitive(pucEndFlagStr,
                                                        ulLineLen,
                                                        aucLineEndStr[ulLoop],
                                                        &bEqualFlag);
                if (VOS_TRUE == bEqualFlag)
                {
                    *pulLineLenWithOutEndFlag = (VOS_UINT32)(pucEndFlagStr - pucOrgAddr);
                    *pulCurrentLineLen        = (VOS_UINT32)(*pulLineLenWithOutEndFlag
                                       + (VOS_UINT32)(VOS_StrLen((VOS_CHAR *)aucLineEndStr[ulLoop]) * 2));
                    return;
                }
            }

            pucEndFlagStr += MN_WORD_UCS2_ENCODE_LEN;
            ulLineLen     -= MN_WORD_UCS2_ENCODE_LEN;
        }

    }
    else
    {
        while (0 != ulLineLen)
        {
            /*ASCII����: ����Ƿ�����н�����<CR><LF>, <CR>, <LF>*/
            for (ulLoop = 0; ulLoop < MN_MSG_ACTIVE_MESSAGE_ENDSTR_NUM; ulLoop++)
            {
                ulLineEndStrLen = VOS_StrLen((VOS_CHAR *)aucLineEndStr[ulLoop]);
                if (ulLineLen < ulLineEndStrLen)
                {
                    continue;
                }

                cRet = VOS_StrNiCmp((VOS_CHAR *)pucEndFlagStr,
                                    (VOS_CHAR *)aucLineEndStr[ulLoop],
                                    ulLineEndStrLen);
                if (0 == cRet)
                {
                    *pulLineLenWithOutEndFlag = (VOS_UINT32)(pucEndFlagStr - pucOrgAddr);
                    *pulCurrentLineLen        = (VOS_UINT32)(*pulLineLenWithOutEndFlag
                                           + VOS_StrLen((VOS_CHAR *)aucLineEndStr[ulLoop]));
                    return;
                }
            }

            pucEndFlagStr++;
            ulLineLen--;
        }


    }

    return;
}


LOCAL VOS_VOID MN_MSG_IdentfyActiveMessage(
    const MN_MSG_DELIVER_STRU           *pstDeliver,
    VOS_BOOL                            *pbActiveMessage
)
{
    /*���ŵ�CLASS���Ͳ���CLASS1��������Ų��Ǽ������*/
    /*���ŵ�TP-PID���Ͳ���TYPE0��������Ų��Ǽ������*/
    if ((MN_MSG_TP_PID_SM_TYPE_0 == pstDeliver->enPid)
     && (MN_MSG_MSG_CLASS_1 == pstDeliver->stDcs.enMsgClass))
    {
        *pbActiveMessage = VOS_TRUE;
    }
    else
    {
        *pbActiveMessage = VOS_FALSE;
    }

    return;
}


LOCAL VOS_UINT32 MN_MSG_IdentifyActiveStatus(
    VOS_UINT8                                                   *pucOrgData,
    VOS_UINT32                                                  ulLineLen,
    MN_MSG_MSG_CODING_ENUM_U8                                   enMsgCoding,
    MN_MSG_ACTIVE_MESSAGE_STATUS_ENUM_UINT8                     *penActiveStatus
)
{
    VOS_BOOL                            pbEqualFlag;
    VOS_INT8                            cRet;

    /*UCS2��������»�ȡ����״̬*/
    if (MN_MSG_MSG_CODING_UCS2 == enMsgCoding)
    {
        MN_CmpAsciiStrAndUcs2StrCaseInsensitive(pucOrgData,
                                                ulLineLen,
                                                (VOS_UINT8 *)"rs:on",
                                                &pbEqualFlag);
        if (VOS_TRUE == pbEqualFlag)
        {
            *penActiveStatus = MN_MSG_ACTIVE_MESSAGE_STATUS_ACTIVE;
            return VOS_OK;
        }

        MN_CmpAsciiStrAndUcs2StrCaseInsensitive(pucOrgData,
                                                ulLineLen,
                                                (VOS_UINT8 *)"rs:off",
                                                &pbEqualFlag);
        if (VOS_TRUE == pbEqualFlag)
        {
            *penActiveStatus = MN_MSG_ACTIVE_MESSAGE_STATUS_DEACTIVE;
            return VOS_OK;
        }

    }
    /*GSM7BIT 8BIT��������»�ȡ����״̬*/
    else
    {
        cRet = VOS_StrNiCmp((VOS_CHAR *)pucOrgData, "rs:on", VOS_StrLen("rs:on"));
        if (0 == cRet)
        {
            *penActiveStatus = MN_MSG_ACTIVE_MESSAGE_STATUS_ACTIVE;
            return VOS_OK;
        }

        cRet = VOS_StrNiCmp((VOS_CHAR *)pucOrgData, "rs:off", VOS_StrLen("rs:off"));
        if (0 == cRet)
        {
            *penActiveStatus = MN_MSG_ACTIVE_MESSAGE_STATUS_DEACTIVE;
            return VOS_OK;
        }

    }

    return VOS_ERR;
}


LOCAL VOS_UINT32 MN_MSG_GetActiveStatus(
    const VOS_UINT8                                             *pucMsgData,
    VOS_UINT32                                                  ulMsgLen,
    MN_MSG_MSG_CODING_ENUM_U8                                   enMsgCoding,
    MN_MSG_ACTIVE_MESSAGE_STATUS_ENUM_UINT8                     *penActiveStatus
)
{
    VOS_UINT32                          ulRet;
    VOS_UINT8                           *pucOrgData;                            /*sms content,not 7bit,all 8bit */
    VOS_UINT32                          ulLineLen;
    VOS_UINT32                          ulCurrentLineLen;
    VOS_UINT32                          ulLineLenWithOutEndFlag;
    VOS_UINT8                           *pucStatusStr;


    ulLineLen = 0;


    pucOrgData = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_TAF, ulMsgLen);
    if (VOS_NULL_PTR == pucOrgData)
    {
        MN_WARN_LOG("MN_MSG_GetActiveStatus:fail to alloc memory. ");
        return MN_ERR_NOMEM;
    }

    /*�Զ������ݽ���7BIT��ASCII���ת��*/
    if (MN_MSG_MSG_CODING_7_BIT == enMsgCoding)
    {
        TAF_STD_ConvertDefAlphaToAscii(pucMsgData, ulMsgLen, pucOrgData, &ulLineLen);
    }
    else
    {
        PS_MEM_CPY(pucOrgData, pucMsgData, ulMsgLen);
        ulLineLen = ulMsgLen;
    }

    /*��ȡ�������״̬�ֶ�: �жϵ�ǰ�ַ����Ƿ�"rs:on"��"rs:off"�������ִ�Сд
      ������"rs:on"��"rs:off", ���ȡ������;
      ����,��ȡ����ʧ��;
    */
    pucStatusStr = pucOrgData;
    while (0 != ulLineLen)
    {
        /*ȷ�ϵ�ǰ�ֶ��Ƿ񼤻����״̬�ֶ�*/
        ulRet = MN_MSG_IdentifyActiveStatus(pucStatusStr,
                                            ulLineLen,
                                            enMsgCoding,
                                            penActiveStatus);
        if (VOS_OK == ulRet)
        {
            PS_MEM_FREE(WUEPS_PID_TAF, pucOrgData);
            return MN_ERR_NO_ERROR;
        }

        /*��ȡ��һ����Ϣ�е����ײ�����ʣ���ֶγ���;*/
        MN_MSG_GetCurrentLineLen(pucStatusStr,
                               ulLineLen,
                               enMsgCoding,
                               &ulCurrentLineLen,
                               &ulLineLenWithOutEndFlag);

        pucStatusStr    += ulCurrentLineLen;
        ulLineLen       -= ulCurrentLineLen;
    }

    PS_MEM_FREE(WUEPS_PID_TAF, pucOrgData);
    return MN_ERR_UNSPECIFIED;
}


LOCAL VOS_UINT32 MN_MSG_GetActiveUrl(
    const VOS_UINT8                     *pucMsgData,
    VOS_UINT32                          ulMsgLen,
    MN_MSG_MSG_CODING_ENUM_U8           enMsgCoding,
    MN_MSG_ACTIVE_MESSAGE_URL_STRU      *pstUrl
)
{
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulStrLen;
    VOS_UINT8                           *pucUrlStr;
    VOS_UINT8                           *pucOrgData;                            /*sms content,not 7bit,all 8bit */
    VOS_UINT32                          ulLineLen;
    VOS_UINT32                          ulCurrentLineLen;
    VOS_UINT32                          ulLineLenWithOutEndFlag;
    VOS_BOOL                            bEqualFlag;
    VOS_INT8                            cRet;


    ulLineLen = 0;


    pucOrgData = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_TAF, ulMsgLen);
    if (VOS_NULL_PTR == pucOrgData)
    {
        return MN_ERR_NOMEM;
    }

    /*�Զ������ݽ���7BIT��ASCII���ת��*/
    if (MN_MSG_MSG_CODING_7_BIT == enMsgCoding)
    {
        TAF_STD_ConvertDefAlphaToAscii(pucMsgData, ulMsgLen, pucOrgData, &ulLineLen);
    }
    else
    {
        PS_MEM_CPY(pucOrgData, pucMsgData, ulMsgLen);
        ulLineLen = ulMsgLen;
    }

    /*��ȡ�������״̬�ֶ�: �жϵ�ǰ�ַ����Ƿ�"url:"�������ִ�Сд
      ������"url:", ���ȡ������;      ����,��ȡ����ʧ��;    */
    bEqualFlag  = VOS_FALSE;
    ulStrLen    = VOS_StrLen("url:");
    pucUrlStr   = pucOrgData;
    while (0 != ulLineLen)
    {
        /*ȷ�ϵ�ǰ�ֶ��Ƿ񼤻����״̬�ֶ�*/
        if (MN_MSG_MSG_CODING_UCS2 == enMsgCoding)
        {
            MN_CmpAsciiStrAndUcs2StrCaseInsensitive(pucUrlStr,
                                                    ulLineLen,
                                                    (VOS_UINT8 *)"url:",
                                                    &bEqualFlag);
            if (VOS_TRUE == bEqualFlag)
            {
                pucUrlStr += (2 * ulStrLen);
                ulLineLen -= (2 * ulStrLen);
                break;
            }

        }
        else
        {
            cRet = VOS_StrNiCmp((VOS_CHAR *)pucUrlStr, "url:", ulStrLen);
            if (0 == cRet)
            {
                bEqualFlag = VOS_TRUE;
                pucUrlStr += ulStrLen;
                ulLineLen -= ulStrLen;
                break;
            }

        }

        /*��ȡ��һ����Ϣ�е����ײ�����ʣ���ֶγ���;*/
        MN_MSG_GetCurrentLineLen(pucUrlStr,
                                 ulLineLen,
                                 enMsgCoding,
                                 &ulCurrentLineLen,
                                 &ulLineLenWithOutEndFlag);

        ulLineLen -= ulCurrentLineLen;
        pucUrlStr += ulCurrentLineLen;
    }

    if (VOS_TRUE == bEqualFlag)
    {
        /*��ȡURL���ܵ���󳤶�*/
        MN_MSG_GetCurrentLineLen(pucUrlStr,
                                 ulLineLen,
                                 enMsgCoding,
                                 &ulCurrentLineLen,
                                 &pstUrl->ulLen);

        /*��дURL���������*/
        PS_MEM_CPY(pstUrl->aucUrl, pucUrlStr, pstUrl->ulLen);
        ulRet = MN_ERR_NO_ERROR;
    }
    else
    {
        ulRet = MN_ERR_UNSPECIFIED;
    }


    PS_MEM_FREE(WUEPS_PID_TAF, pucOrgData);
    return ulRet;
}


VOS_UINT32 MN_MSG_WriteActiveMessage(
    MN_MSG_ACTIVE_MESSAGE_STRU          *pstOrgActiveMessageInfo,
    MN_MSG_ACTIVE_MESSAGE_STRU          *pstActiveMessageInfo
)
{
    VOS_UINT32                          ulRet;
    VOS_INT32                           lRet;
    VOS_UINT8                           *pucActiveMessageInfo;
    VOS_UINT8                           *pucEvaluate;

    /*1.�жϴ�д��ļ�����Ų�����NVIM�еĲ����Ƿ�һ�£�һ������дNVIMֱ���˳�*/
    if (pstOrgActiveMessageInfo->enActiveStatus == pstActiveMessageInfo->enActiveStatus)
    {
        if (pstOrgActiveMessageInfo->stUrl.ulLen == pstActiveMessageInfo->stUrl.ulLen)
        {
            if (pstOrgActiveMessageInfo->enMsgCoding == pstActiveMessageInfo->enMsgCoding)
            {
                lRet = VOS_MemCmp(pstOrgActiveMessageInfo->stUrl.aucUrl,
                                   pstActiveMessageInfo->stUrl.aucUrl,
                                   pstActiveMessageInfo->stUrl.ulLen);
                if (0 == lRet)
                {
                    return MN_ERR_NO_ERROR;
                }
            }
        }
    }

    /*2.д������Ų�����NVIM��*/
    /*2.1 ΪNVIM�洢�������������ڴ�*/
    pucActiveMessageInfo = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_TAF,
                                                     MN_MSG_ACTIVE_MESSAGE_PARA_LEN);
    if (VOS_NULL_PTR == pucActiveMessageInfo)
    {
        return MN_ERR_NOMEM;
    }

    /*2.2 ��������Ų������ݽṹת����NVIM�洢��������*/
    pucEvaluate = pucActiveMessageInfo;
    *pucEvaluate = pstActiveMessageInfo->enActiveStatus;
    pucEvaluate++;

    *pucEvaluate = pstActiveMessageInfo->enMsgCoding;
    pucEvaluate++;

    *pucEvaluate = (VOS_UINT8)(pstActiveMessageInfo->stUrl.ulLen & 0xff);
    pucEvaluate++;
    *pucEvaluate = (VOS_UINT8)((pstActiveMessageInfo->stUrl.ulLen >> 8) & 0xff);
    pucEvaluate++;
    *pucEvaluate = (VOS_UINT8)((pstActiveMessageInfo->stUrl.ulLen >> 16) & 0xff);
    pucEvaluate++;
    *pucEvaluate = (VOS_UINT8)((pstActiveMessageInfo->stUrl.ulLen >> 24) & 0xff);
    pucEvaluate++;

    PS_MEM_CPY(pucEvaluate,
               pstActiveMessageInfo->stUrl.aucUrl,
               pstActiveMessageInfo->stUrl.ulLen);

    /*2.3 д���������Ϣ��NVIM*/
    ulRet = NV_Write(en_NV_Item_SMS_ActiveMessage_Para,
                     pucActiveMessageInfo,
                     MN_MSG_ACTIVE_MESSAGE_PARA_LEN);
    if (NV_OK != ulRet)
    {
        ulRet = MN_ERR_CLASS_SMS_NVIM;
    }
    else
    {
        ulRet = MN_ERR_NO_ERROR;
    }

    PS_MEM_FREE(WUEPS_PID_TAF, pucActiveMessageInfo);
    return ulRet;
}
VOS_UINT32 MN_MSG_ReadActiveMessage(
    MN_MSG_ACTIVE_MESSAGE_STRU          *pstActiveMessage
)
{
    VOS_UINT32                          ulRet;
    VOS_UINT8                           *pucActiveMessageInfo;
    VOS_UINT8                           *pucEvaluate;

    pucActiveMessageInfo = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_TAF,
                                                     MN_MSG_ACTIVE_MESSAGE_PARA_LEN);
    if (VOS_NULL_PTR == pucActiveMessageInfo)
    {
        MN_WARN_LOG("MN_MSG_ReadActiveMessage : fail to alloc memory . ");
        return MN_ERR_NOMEM;
    }

    ulRet = NV_Read(en_NV_Item_SMS_ActiveMessage_Para,
                    pucActiveMessageInfo,
                    MN_MSG_ACTIVE_MESSAGE_PARA_LEN);
    if (NV_OK != ulRet)
    {
        MN_WARN_LOG("MN_MSG_ReadActiveMessage : fail to read NVIM . ");
        PS_MEM_FREE(WUEPS_PID_TAF, pucActiveMessageInfo);
        return MN_ERR_CLASS_SMS_NVIM;
    }

    pucEvaluate = pucActiveMessageInfo;

    pstActiveMessage->enActiveStatus = *pucEvaluate;
    pucEvaluate++;

    pstActiveMessage->enMsgCoding   = *pucEvaluate;
    pucEvaluate++;

    pstActiveMessage->stUrl.ulLen   = *pucEvaluate;
    pucEvaluate++;
    pstActiveMessage->stUrl.ulLen  |= *pucEvaluate << 8;
    pucEvaluate++;
    pstActiveMessage->stUrl.ulLen  |= *pucEvaluate << 16;
    pucEvaluate++;
    pstActiveMessage->stUrl.ulLen  |= *pucEvaluate << 24;
    pucEvaluate++;

    if (pstActiveMessage->stUrl.ulLen > MN_MSG_ACTIVE_MESSAGE_MAX_URL_LEN)
    {
        MN_WARN_LOG("MN_MSG_ReadActiveMessage : fail to read NVIM . ");
        pstActiveMessage->stUrl.ulLen = MN_MSG_ACTIVE_MESSAGE_MAX_URL_LEN;
    }

    PS_MEM_CPY(pstActiveMessage->stUrl.aucUrl,
               pucEvaluate,
               pstActiveMessage->stUrl.ulLen);

    PS_MEM_FREE(WUEPS_PID_TAF, pucActiveMessageInfo);
    return MN_ERR_NO_ERROR;
}


LOCAL VOS_VOID MN_MSG_GetAndSaveActiveMessageInfo(
    const MN_MSG_DELIVER_STRU           *pstDeliver
)
{
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulRemainder;
    VOS_BOOL                            bActiveMessage;
    MN_MSG_ACTIVE_MESSAGE_STRU          stActiveMessageInfo;
    MN_MSG_ACTIVE_MESSAGE_STRU          stOrgActiveMessageInfo;

    /*If (MN_MSG_IdentfyActiveMessageΪ��)        Return; */
    MN_MSG_IdentfyActiveMessage(pstDeliver, &bActiveMessage);
    if (VOS_TRUE != bActiveMessage)
    {
        MN_INFO_LOG("MN_MSG_GetAndSaveActiveMessageInfo :not a active message. ");
        return;
    }

    /*��¼���ŵ����ݵı����ʽ: �˴��������GSM7bit��8BIT��ת��*/
    if (MN_MSG_MSG_CODING_UCS2 == pstDeliver->stDcs.enMsgCoding)
    {
        ulRemainder = pstDeliver->stUserData.ulLen % MN_WORD_UCS2_ENCODE_LEN;
        if (0 != ulRemainder)
        {
            return;
        }

        stActiveMessageInfo.enMsgCoding = MN_MSG_MSG_CODING_UCS2;
    }
    else
    {
        stActiveMessageInfo.enMsgCoding = MN_MSG_MSG_CODING_8_BIT;
    }

    /*MN_MSG_GetActiveStatus��ȡ����״̬��Ϣʧ��        Return; */
    ulRet = MN_MSG_GetActiveStatus(pstDeliver->stUserData.aucOrgData,
                                   pstDeliver->stUserData.ulLen,
                                   pstDeliver->stDcs.enMsgCoding,
                                   &stActiveMessageInfo.enActiveStatus);
    if (MN_ERR_NO_ERROR != ulRet)
    {
        MN_INFO_LOG("MN_MSG_GetAndSaveActiveMessageInfo :no active status. ");
        return;
    }

    ulRet = MN_MSG_ReadActiveMessage(&stOrgActiveMessageInfo);
    if (MN_ERR_NO_ERROR != ulRet)
    {
        return;
    }

    /*MN_MSG_GetActiveUrl��ȡ�������URL��Ϣ*/
    ulRet = MN_MSG_GetActiveUrl(pstDeliver->stUserData.aucOrgData,
                                pstDeliver->stUserData.ulLen,
                                pstDeliver->stDcs.enMsgCoding,
                                &stActiveMessageInfo.stUrl);
    if (MN_ERR_NO_ERROR != ulRet)
    {
        /*��ȡNVIM�м�¼�ļ�����Ų���*/
        PS_MEM_CPY(&stActiveMessageInfo.stUrl,
                   &stOrgActiveMessageInfo.stUrl,
                   sizeof(stActiveMessageInfo.stUrl));
        stActiveMessageInfo.enMsgCoding = stOrgActiveMessageInfo.enMsgCoding;
    }

    /*��¼���ŵ����ݵı����ʽ: �˴��������GSM7bit��8BIT��ת��*/
    /*MN_MSG_WriteActiveMessageд������ŵļ���״̬��URL��Ϣ��NVIM��*/
    ulRet = MN_MSG_WriteActiveMessage(&stOrgActiveMessageInfo, &stActiveMessageInfo);
    if (MN_ERR_NO_ERROR != ulRet)
    {
        MN_WARN_LOG("MN_MSG_GetAndSaveActiveMessageInfo:Fail to Write NVIM.");
    }

    return;

}
LOCAL VOS_VOID MSG_DecodeRpData(
    const VOS_UINT8                     *pucRpduContent,
    MN_MSG_RP_DATA_STRU                 *pstRpData
)
{
    VOS_UINT32                          ucIdx = 0;

    pstRpData->ucRpMsgType = pucRpduContent[ucIdx++];
    pstRpData->ucRpMr = pucRpduContent[ucIdx++];
    if (pucRpduContent[ucIdx] > 0)
    {
        pstRpData->stOrgAddr.ucBcdLen = pucRpduContent[ucIdx++] - 1;
        pstRpData->stOrgAddr.addrType = pucRpduContent[ucIdx++];
        PS_MEM_CPY(pstRpData->stOrgAddr.aucBcdNum,pucRpduContent+ucIdx,pstRpData->stOrgAddr.ucBcdLen);
        ucIdx += pstRpData->stOrgAddr.ucBcdLen;
    }
    else
    {
        pstRpData->stOrgAddr.ucBcdLen = 0;
        ucIdx++;
    }

    if (pucRpduContent[ucIdx] > 0)
    {
        pstRpData->stDestAddr.ucBcdLen = pucRpduContent[ucIdx++] - 1;
        pstRpData->stDestAddr.addrType = pucRpduContent[ucIdx++];
        PS_MEM_CPY(pstRpData->stDestAddr.aucBcdNum,pucRpduContent+ucIdx,pstRpData->stDestAddr.ucBcdLen);
        ucIdx += pstRpData->stDestAddr.ucBcdLen;
    }
    else
    {
        pstRpData->stDestAddr.ucBcdLen = 0;
        ucIdx++;
    }

    pstRpData->ucRpUserDataLen = pucRpduContent[ucIdx++];
    PS_MEM_CPY(pstRpData->aucRpUserData,pucRpduContent + ucIdx,pstRpData->ucRpUserDataLen);
}


/*****************************************************************************
 �� �� ��  : MSG_DecodeRpAck
 ��������  : ��һ���ֽ�������Ϊ24011 RP ACK����Ľṹ
 �������  : pucRpduContent:���緢�͹�����RP-ACK������
             ucRpduvLen:Rp-Ack�ĳ���
             pstRpAck:24011 RP ACK����Ľṹ
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2008��2��19��
    ��    ��   : �ܾ� 40661
    �޸�����   : �����ɺ���

*****************************************************************************/
LOCAL VOS_VOID MSG_DecodeRpAck(
    const VOS_UINT8                     *pucRpduContent,
    VOS_UINT8                           ucRpduvLen,
    MN_MSG_RP_ACK_STRU                  *pstRpAck
)
{
    VOS_UINT32                          ucIdx = 0;

    pstRpAck->ucRpMsgType = pucRpduContent[ucIdx++];
    pstRpAck->ucRpMr = pucRpduContent[ucIdx++];
    if ((ucRpduvLen > 3)
     && (MN_MSG_RP_USER_DATA_IEI == pucRpduContent[ucIdx]))
    {
        pstRpAck->bRpUserDataExist = VOS_TRUE;
        pstRpAck->ucRpUserDataIei = MN_MSG_RP_USER_DATA_IEI;
        ucIdx++;
        pstRpAck->ucRpUserDataLen = pucRpduContent[ucIdx++];
        PS_MEM_CPY(pstRpAck->aucRpUserData,pucRpduContent + ucIdx,pstRpAck->ucRpUserDataLen);
    }
    else
    {
        pstRpAck->bRpUserDataExist = VOS_FALSE;
    }
}
/*****************************************************************************
 �� �� ��  : MSG_DecodeRpErr
 ��������  : ��һ���ֽ�������Ϊ24011 RP ERROR����Ľṹ
 �������  : pucRpduContent:���緢�͹�����RP-ERROR������
             ucRpduvLen:Rp-Error�ĳ���
             pstRpErr:24011 RP ERROR����Ľṹ
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2008��2��19��
    ��    ��   : �ܾ� 40661
    �޸�����   : �����ɺ���

*****************************************************************************/

LOCAL VOS_VOID MSG_DecodeRpErr(
    const VOS_UINT8                     *pucRpduContent,
    VOS_UINT8                           ucRpduvLen,
    MN_MSG_RP_ERR_STRU                  *pstRpErr
)
{
    VOS_UINT32                          ucIdx = 0;

    pstRpErr->ucRpMsgType = pucRpduContent[ucIdx++];
    pstRpErr->ucRpMr = pucRpduContent[ucIdx++];
    pstRpErr->ucRpCauseLen = pucRpduContent[ucIdx++];
    pstRpErr->stRpCause.enRpCause = pucRpduContent[ucIdx++];
    if (1 == pstRpErr->ucRpCauseLen)
    {
        pstRpErr->stRpCause.bDiagnosticsExist = VOS_FALSE;
    }
    else
    {
        pstRpErr->stRpCause.bDiagnosticsExist = VOS_TRUE;
        pstRpErr->stRpCause.ucDiagnostics = pucRpduContent[ucIdx++];
    }
    if ((ucRpduvLen > (ucIdx + 1))
     && (MN_MSG_RP_USER_DATA_IEI == pucRpduContent[ucIdx]))
    {
        pstRpErr->bRpUserDataExist = VOS_TRUE;
        pstRpErr->ucRpUserDataIei = MN_MSG_RP_USER_DATA_IEI;
        ucIdx++;
        pstRpErr->ucRpUserDataLen= pucRpduContent[ucIdx++];
        PS_MEM_CPY(pstRpErr->aucRpUserData,pucRpduContent + ucIdx,pstRpErr->ucRpUserDataLen);
    }
    else
    {
        pstRpErr->bRpUserDataExist = VOS_FALSE;
    }
}


LOCAL MN_MSG_RETRY_FLAG_U8 MSG_JudgeRetryFlg(VOS_VOID)
{
    MN_MSG_RETRY_INFO_STRU              stRetryInfo;
    VOS_UINT32                          ulRemainTimeTick;

    PS_MEM_SET(&stRetryInfo, 0, sizeof(stRetryInfo));

    MN_MSG_GetRetryInfo(&stRetryInfo);

    /* ��ǰû�������ط� */
    if ((0 == stRetryInfo.ulRetryPeriod) || (0 == stRetryInfo.ulRetryInterval))
    {
        return MN_MSG_RETRY_OFF;
    }

    ulRemainTimeTick = MN_MSG_GetTimerRemainTime(MN_MSG_ID_WAIT_RETRY_PERIOD);
    if (ulRemainTimeTick == 0)
    {
        return MN_MSG_RETRY_END;
    }

    if (((VOS_INT32)(ulRemainTimeTick - stRetryInfo.ulRetryInterval)) <= 0)
    {
        return MN_MSG_RETRY_END;
    }

    return MN_MSG_RETRY_BEGIN;
}

LOCAL VOS_BOOL MSG_IsProcessRetry(
    MN_MSG_MO_ENTITY_STRU              *pstMoEntity,
    SMR_SMT_ERROR_ENUM_UINT32           enErrCause
)
{
    MN_MSG_RETRY_FLAG_U8                enRetryFlg = MN_MSG_RETRY_OFF;
    VOS_UINT32                          ulRet;
    MN_MSG_SEND_DOMAIN_ENUM_U8          enOldSendDomain;

    enRetryFlg = MSG_JudgeRetryFlg();


    /* ��ǰNVδ�����ط����ж��Ƿ���Ҫͨ����һ������з��� */
    if (MN_MSG_RETRY_OFF == enRetryFlg)
    {
        /* �ط������������,һ������ʧ��ͨ������һ������,
        ����CS����ʧ�ܺ�,PS���ͳɹ�,��ʱ����ͨ��PS����
        PS����ʧ����,��Ȼ��Ҫͨ��CS����*/
        /* �ط����ܹر�,�ж��Ƿ���Ҫ������һ������ */
        if ( ((MN_MSG_SEND_DOMAIN_PS_PREFERRED == pstMoEntity->enHopeSendDomain)
            && (MN_MSG_SEND_DOMAIN_PS == pstMoEntity->enSendDomain))
          ||((MN_MSG_SEND_DOMAIN_CS_PREFERRED == pstMoEntity->enHopeSendDomain)
            && (MN_MSG_SEND_DOMAIN_CS == pstMoEntity->enSendDomain)))
        {
            enOldSendDomain = pstMoEntity->enSendDomain;
            MN_MSG_GetCurSendDomain(pstMoEntity->enHopeSendDomain,&(pstMoEntity->enSendDomain));
            /* ��ȡ�µķ�������ϵĲ�һ�²���Ҫ�����ط� */
            if (MN_MSG_SEND_DOMAIN_NO != pstMoEntity->enSendDomain)
            {
                if (pstMoEntity->enSendDomain != enOldSendDomain)
                {
                    /* TAF���ط���SUBMIT����TP-RDӦ���޸�Ϊ1��ָʾ�������ľܾ������ظ����� */
                    MN_MSG_SetTpRd(pstMoEntity);

                    /*��һ��������RP-Data���͸�NAS���SMSģ��*/
                    ulRet = MN_MSG_SendSmsRpDataReq(pstMoEntity->enSendDomain,
                                                    pstMoEntity->aucRpDataInfo,
                                                    pstMoEntity->ucRpDataLen,
                                                    pstMoEntity->enMsgSignallingType);

                    if (VOS_OK == ulRet)
                    {
                        /* ����MO���� */
                        MN_MSG_CreateMoInfo(pstMoEntity);
                        return VOS_TRUE;
                    }
                }
            }
        }

        return VOS_FALSE;
    }

    if ( (VOS_TRUE          == MN_MSG_IsGuNeedSmsRetry(enErrCause))
      && (MN_MSG_RETRY_BEGIN == enRetryFlg) )
    {
        /* ��Ҫ�����ط�,�����ط������ʱ��,��ʱ����ʱ���ٽ����ط� */
        MN_MSG_StartTimer(MN_MSG_ID_WAIT_RETRY_INTERVAL, 0);
        
        return VOS_TRUE;
    }

    /* �ط����̽���,����Ҫ�����κβ���,ֹͣ��ʱ�� */
    MN_MSG_StopTimer(MN_MSG_ID_WAIT_RETRY_PERIOD);

    MN_MSG_StopTimer(MN_MSG_ID_WAIT_RETRY_INTERVAL);
    
    return VOS_FALSE;
    
    
}
LOCAL VOS_VOID MSG_CreateEFSmsrContent(
    const MN_MSG_RAW_TS_DATA_STRU       *pstRawData,
    VOS_UINT8                           ucIndex,
    VOS_UINT8                           *pucContent
)
{
    *pucContent = ucIndex;
    if (pstRawData->ulLen > (MN_MSG_EFSMSR_LEN - 1))
    {
        PS_MEM_CPY(pucContent + 1,pstRawData->aucData,MN_MSG_EFSMSR_LEN - 1);
    }
    else
    {
        PS_MEM_CPY(pucContent + 1,pstRawData->aucData,pstRawData->ulLen);
    }
}


LOCAL VOS_UINT32 MSG_FindReplaceSms(
    const MN_MSG_ASCII_ADDR_STRU       *pstOrigAddr,
    MN_MSG_TP_PID_TYPE_ENUM_U8          enPid,
    MN_MSG_MEM_STORE_ENUM_U8            enMemStore,
    VOS_UINT32                         *pulIndex
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulRecCount;
    VOS_UINT32                          ulRet;
    VOS_UINT8                           ucStatus;
    VOS_UINT8                           aucSmContent[MN_MSG_EFSMS_LEN];
    VOS_UINT8                           ucScaLen;                               /* Sca�ĳ��� */
    VOS_UINT8                           *pucTpdu;                               /* ָ��Tpdu��ָ�� */
    MN_MSG_RAW_TS_DATA_STRU             stRawData;
    MN_MSG_TS_DATA_INFO_STRU            *pstTsDataInfo;

    ucStatus = EF_SMS_STATUS_FREE_SPACE;
    pstTsDataInfo = (MN_MSG_TS_DATA_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_TAF,
                                                             sizeof(MN_MSG_TS_DATA_INFO_STRU));
    if (VOS_NULL_PTR == pstTsDataInfo)
    {
        return VOS_ERR;
    }

    ulRecCount = MN_MSG_GetSmCapacity(enMemStore);
    for (i = 0; i < ulRecCount; i++)
    {
        PS_MEM_SET(&stRawData,0X00,sizeof(stRawData));
        PS_MEM_SET(pstTsDataInfo,0X00,sizeof(MN_MSG_TS_DATA_INFO_STRU));
        ulRet = MN_MSG_GetStatus(enMemStore,i,&ucStatus);
        if ((MN_ERR_NO_ERROR == ulRet)
         && (((ucStatus & 0x03) == EF_SMS_STATUS_MT_READ)
          || ((ucStatus & 0x03) == EF_SMS_STATUS_MT_TO_BE_READ)))
        {

            PS_MEM_SET(aucSmContent, 0x00, sizeof(aucSmContent));

            ulRet = MN_MSG_ReadSmsInfo(enMemStore,i,aucSmContent);
            if (MN_ERR_NO_ERROR != ulRet)
            {
                continue;
            }

            ucScaLen = aucSmContent[1];
            pucTpdu = aucSmContent + 1 + 1 + ucScaLen;
            if ( MN_MSG_TP_MTI_DELIVER != ( *pucTpdu & 0x03 ))
            {
                continue;
            }
            stRawData.enTpduType = MN_MSG_TP_MTI_DELIVER;
            stRawData.ulLen = (MN_MSG_EFSMS_LEN - ucScaLen) - 1;


            if ( stRawData.ulLen > sizeof(stRawData.aucData) )
            {
                stRawData.ulLen = sizeof(stRawData.aucData);
            }

            PS_MEM_CPY(stRawData.aucData,pucTpdu,stRawData.ulLen);
            ulRet = MN_MSG_Decode(&stRawData,pstTsDataInfo);
            if (MN_ERR_NO_ERROR != ulRet)
            {
                continue;
            }
            ulRet = (VOS_UINT32)VOS_MemCmp(pstOrigAddr, &(pstTsDataInfo->u.stDeliver.stOrigAddr),sizeof(MN_MSG_ASCII_ADDR_STRU));
            if ((enPid == pstTsDataInfo->u.stDeliver.enPid)
             && (0 == ulRet))
            {
                *pulIndex = i;
                PS_MEM_FREE(WUEPS_PID_TAF, pstTsDataInfo);
                return VOS_OK;
            }
        }
    }

    PS_MEM_FREE(WUEPS_PID_TAF, pstTsDataInfo);

    return VOS_ERR;
}


LOCAL VOS_UINT32 MSG_SearchReplacedSms(
    const MN_MSG_ASCII_ADDR_STRU       *pstOrigAddr,
    MN_MSG_TP_PID_TYPE_ENUM_U8          enPid,
    MN_MSG_MEM_STORE_ENUM_U8           *penMemStore,
    VOS_UINT32                         *pulIndex
)
{
    VOS_UINT32                          ulRet;
    VOS_UINT8                           ucActFlg;

    MN_MSG_GetReplaceFeature(&ucActFlg);
    if (MN_MSG_NVIM_ITEM_ACTIVE != ucActFlg)
    {
        return VOS_ERR;
    }

    ulRet = MSG_FindReplaceSms(pstOrigAddr, enPid, MN_MSG_MEM_STORE_SIM, pulIndex);
    if (VOS_OK == ulRet)
    {
        *penMemStore = MN_MSG_MEM_STORE_SIM;
        return VOS_OK;
    }

    ulRet = MSG_FindReplaceSms(pstOrigAddr, enPid, MN_MSG_MEM_STORE_NV, pulIndex);
    if (VOS_OK == ulRet)
    {
        *penMemStore = MN_MSG_MEM_STORE_NV;
        return VOS_OK;
    }

    return VOS_ERR;
}


VOS_UINT32 MSG_ProcReplaceSm(
    const MN_MSG_CONFIG_PARM_STRU       *pstCfgParm,
    const MN_MSG_BCD_ADDR_STRU          *pstScAddr,
    const MN_MSG_RAW_TS_DATA_STRU       *pstRawData,
    const MN_MSG_TS_DATA_INFO_STRU      *pstTsData
)
{
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulSaveIndex = MN_MSG_NO_AVAILABLE_SMS_REC;
    MN_MSG_MEM_STORE_ENUM_U8            enMemStore;
    VOS_UINT8                           aucSmContent[MN_MSG_EFSMS_LEN];
#if (NAS_FEATURE_SMS_NVIM_SMSEXIST == FEATURE_ON)
    MN_OPERATION_ID_T                   bcopId;
#endif
#if ((NAS_FEATURE_SMS_NVIM_SMSEXIST == FEATURE_ON) && (NAS_FEATURE_SMS_FLASH_SMSEXIST == FEATURE_ON))
    MN_MSG_WRITE_USIM_INFO_STRU         stWriteUsimInfo;
#else
    MN_MSG_USIM_INFO_STRU               stSmaUsimInfo;
#endif
    MN_MSG_DELIVER_EVT_INFO_STRU        stDeliverEvt;

    /* ���Ų���Replace���͵ģ�����VOS_FALSE   �����滻���Զ��ţ���û�п��滻�Ķ��� */
    if (((MN_MSG_TP_PID_REPLACE_SM_1 <= (VOS_UINT8)pstTsData->u.stDeliver.enPid )
       && (MN_MSG_TP_PID_REPLACE_SM_7 >= (VOS_UINT8)pstTsData->u.stDeliver.enPid))
      || (MN_MSG_TP_PID_RETURN_CALL == (VOS_UINT8)pstTsData->u.stDeliver.enPid))
    {
        ;
    }
    else
    {
        return VOS_FALSE;
    }

    /* û�п��滻�Ķ��ţ�����VOS_FALSE   �����滻���Զ��ţ���û�п��滻�Ķ��� */
    /* �˴���Ҫ�Աȱ��ȷ���Ƿ��������MODEM�Ĵ洢���� */
    ulRet = MSG_SearchReplacedSms(&(pstTsData->u.stDeliver.stOrigAddr),
                                  pstTsData->u.stDeliver.enPid,
                                  &enMemStore,
                                  &ulSaveIndex);
    if (VOS_OK != ulRet)
    {
        return VOS_FALSE;
    }

    MN_MSG_CreateEFSmContent(pstScAddr,
                             pstRawData,
                             EF_SMS_STATUS_MT_TO_BE_READ,
                             aucSmContent);

    /* �����յ��Ķ����滻�Ѿ��洢���� */
#if ((NAS_FEATURE_SMS_NVIM_SMSEXIST != FEATURE_ON)\
|| (NAS_FEATURE_SMS_FLASH_SMSEXIST != FEATURE_ON))
    PS_MEM_SET(&stSmaUsimInfo,0X00,sizeof(stSmaUsimInfo));
    stSmaUsimInfo.clientId = MN_CLIENT_ALL;
    stSmaUsimInfo.opId = 0;
    stSmaUsimInfo.usEfId = EF_SMS_SMS_ID;
    stSmaUsimInfo.enDeleteType = MN_MSG_DELETE_MAX;
    stSmaUsimInfo.ucRecIndex = (VOS_UINT8)ulSaveIndex;
    stSmaUsimInfo.enSmaUsimAction = MN_MSG_USIM_RCVNEWMSG;
    ulRet = MN_MSG_WriteSmsFile(enMemStore,VOS_TRUE,&stSmaUsimInfo,aucSmContent);
#else
    if (MN_MSG_MEM_STORE_SIM == enMemStore)
    {
        PS_MEM_SET(&stWriteUsimInfo,0X00,sizeof(stWriteUsimInfo));
        stWriteUsimInfo.stUsimInfo.clientId = MN_CLIENT_ALL;
        stWriteUsimInfo.stUsimInfo.opId = 0;
        stWriteUsimInfo.stUsimInfo.usEfId = EF_SMS_SMS_ID;
        stWriteUsimInfo.stUsimInfo.enDeleteType = MN_MSG_DELETE_MAX;
        stWriteUsimInfo.stUsimInfo.ucRecIndex = (VOS_UINT8)ulSaveIndex;
        stWriteUsimInfo.stUsimInfo.enSmaUsimAction = MN_MSG_USIM_RCVNEWMSG;
        stWriteUsimInfo.bCreateNode = VOS_TRUE;
        ulRet = MN_MSG_WriteSmsFile(enMemStore, VOS_NULL_PTR, &stWriteUsimInfo, aucSmContent);

    }
    else
    {
        ulRet = MN_MSG_WriteSmsFile(enMemStore, &ulSaveIndex, VOS_NULL_PTR, aucSmContent);
    }
#endif

    if (MN_ERR_NO_ERROR != ulRet)
    {
        MN_MSG_Internal_SendRpt(VOS_FALSE,
                                MN_MSG_RP_CAUSE_PROTOCOL_ERR_UNSPECIFIED,
                                MN_MSG_TP_CAUSE_ERR_IN_MS);
        MN_WARN_LOG("MSG_ProcStoreSm:MN_MSG_WriteSmsFile Return Error");
    }
#if (NAS_FEATURE_SMS_NVIM_SMSEXIST == FEATURE_ON)
    else if (MN_MSG_MEM_STORE_NV == pstCfgParm->enSmMemStore)
    {
        MN_MSG_Internal_SendRpt(VOS_TRUE,0,0);
        /*�㲥�ϱ���һ������*/
        bcopId = MN_MSG_GetBCopId();
        stDeliverEvt.ulInex = ulSaveIndex;
        MN_MSG_ReportRcvMsgEvent(bcopId,&stDeliverEvt);
        /*�㲥�ϱ�NVIM�ж����ѷ����˸ı�*/
        MN_MSG_ReportSmChanged(MN_CLIENT_ALL,bcopId,VOS_TRUE,MN_MSG_MEM_STORE_NV);
    }
#endif
    else
    {
    }

    return VOS_TRUE;
}


LOCAL VOS_VOID MSG_ProcClass2Msg(
    const MN_MSG_BCD_ADDR_STRU          *pstScAddr,
    const MN_MSG_RAW_TS_DATA_STRU       *pstRawData
)
{
    VOS_UINT32                          ulSaveIndex;
    VOS_UINT32                          ulRet;
    VOS_UINT8                           aucSmContent[MN_MSG_EFSMS_LEN];
#if ((NAS_FEATURE_SMS_NVIM_SMSEXIST == FEATURE_ON) && (NAS_FEATURE_SMS_FLASH_SMSEXIST == FEATURE_ON))
    MN_MSG_WRITE_USIM_INFO_STRU         stWriteUsimInfo;
#else
    MN_MSG_USIM_INFO_STRU               stSmaUsimInfo;
#endif

    MN_INFO_LOG("MSG_ProcClass2Msg: step into function. ");
    PS_MEM_SET(aucSmContent,(VOS_CHAR)0xFF,MN_MSG_EFSMS_LEN);

    ulSaveIndex = MN_MSG_GetAvailSmRecIndex(MN_MSG_MEM_STORE_SIM);

    /*��ǰUSIM������ (�˴��ο�Э��23038)*/
    MN_NORM_LOG1("MSG_ProcClass2Msg: ucSaveIndex is ", (VOS_INT32)ulSaveIndex);
    if (MN_MSG_NO_AVAILABLE_SMS_REC != ulSaveIndex)
    {
        MN_MSG_CreateEFSmContent(pstScAddr,
                                 pstRawData,
                                 EF_SMS_STATUS_MT_TO_BE_READ,
                                 aucSmContent);

#if ((NAS_FEATURE_SMS_NVIM_SMSEXIST != FEATURE_ON)\
  || (NAS_FEATURE_SMS_FLASH_SMSEXIST != FEATURE_ON))
        PS_MEM_SET(&stSmaUsimInfo,0X00,sizeof(stSmaUsimInfo));
        stSmaUsimInfo.clientId = MN_CLIENT_ALL;
        stSmaUsimInfo.opId = 0;
        stSmaUsimInfo.usEfId = EF_SMS_SMS_ID;
        stSmaUsimInfo.enDeleteType = MN_MSG_DELETE_MAX;
        stSmaUsimInfo.ucRecIndex = ulSaveIndex;
        stSmaUsimInfo.enSmaUsimAction = MN_MSG_USIM_RCVNEWMSG;
        ulRet = MN_MSG_WriteSmsFile(MN_MSG_MEM_STORE_SIM,VOS_TRUE,&stSmaUsimInfo,aucSmContent);
#else
        PS_MEM_SET(&stWriteUsimInfo,0X00,sizeof(stWriteUsimInfo));
        stWriteUsimInfo.stUsimInfo.clientId = MN_CLIENT_ALL;
        stWriteUsimInfo.stUsimInfo.opId = 0;
        stWriteUsimInfo.stUsimInfo.usEfId = EF_SMS_SMS_ID;
        stWriteUsimInfo.stUsimInfo.enDeleteType = MN_MSG_DELETE_MAX;
        stWriteUsimInfo.stUsimInfo.ucRecIndex = (VOS_UINT8)ulSaveIndex;
        stWriteUsimInfo.stUsimInfo.enSmaUsimAction = MN_MSG_USIM_RCVNEWMSG;
        stWriteUsimInfo.bCreateNode = VOS_TRUE;
        ulRet = MN_MSG_WriteSmsFile(MN_MSG_MEM_STORE_SIM,VOS_NULL_PTR,&stWriteUsimInfo,aucSmContent);

#endif

        if (MN_ERR_NO_ERROR != ulRet)
        {
            MN_MSG_Internal_SendRpt(VOS_FALSE,
                                    MN_MSG_RP_CAUSE_PROTOCOL_ERR_UNSPECIFIED,
                                    MN_MSG_TP_CAUSE_ERR_IN_MS);
            MN_WARN_LOG("MSG_ProcClass2Msg:Save Class 2 in Usim Error");
        }
        return;
    }

    MN_MSG_FailToWriteEfsms(MN_MSG_MEM_STORE_SIM, MN_MSG_WRITE_EFSMS_CAUSE_MEM_FULL);
}
LOCAL VOS_VOID MSG_ProcStoreSm(
    const MN_MSG_CONFIG_PARM_STRU       *pstCfgParm,
    const MN_MSG_BCD_ADDR_STRU          *pstScAddr,
    const MN_MSG_RAW_TS_DATA_STRU       *pstRawData,
    const MN_MSG_TS_DATA_INFO_STRU      *pstTsData
)
{
    VOS_UINT32                          ulRet = VOS_ERR;
    VOS_UINT32                          ulSaveIndex = MN_MSG_NO_AVAILABLE_SMS_REC;
    MN_MSG_SMSS_INFO_STRU               stSmssInfo;
    VOS_UINT8                           aucSmContent[MN_MSG_EFSMS_LEN];
#if (NAS_FEATURE_SMS_NVIM_SMSEXIST == FEATURE_ON)
    MN_OPERATION_ID_T                   bcopId;
#endif
#if ((NAS_FEATURE_SMS_NVIM_SMSEXIST == FEATURE_ON) && (NAS_FEATURE_SMS_FLASH_SMSEXIST == FEATURE_ON))
    MN_MSG_WRITE_USIM_INFO_STRU         stWriteUsimInfo;
#else
    MN_MSG_USIM_INFO_STRU               stSmaUsimInfo;
#endif
    MN_MSG_DELIVER_EVT_INFO_STRU        stDeliverEvt;

    PS_MEM_SET(aucSmContent,(VOS_CHAR)0xFF,MN_MSG_EFSMS_LEN);
    PS_MEM_SET(&stSmssInfo,0X00,sizeof(stSmssInfo));
    MSG_BuildDeliverEvtParm(pstCfgParm,pstScAddr,pstRawData,&stDeliverEvt);

    ulSaveIndex = MN_MSG_GetAvailSmRecIndex(pstCfgParm->enSmMemStore);

    /*��ǰ�洢������,û�кϷ���ucSaveIndex*/
    if (MN_MSG_NO_AVAILABLE_SMS_REC == ulSaveIndex)
    {
        MN_MSG_FailToWriteEfsms(pstCfgParm->enSmMemStore,
                                MN_MSG_WRITE_EFSMS_CAUSE_MEM_FULL);
    }
    else
    {
        MN_MSG_CreateEFSmContent(pstScAddr,
                                 pstRawData,
                                 EF_SMS_STATUS_MT_TO_BE_READ,
                                 aucSmContent);

#if ((NAS_FEATURE_SMS_NVIM_SMSEXIST != FEATURE_ON)\
  || (NAS_FEATURE_SMS_FLASH_SMSEXIST != FEATURE_ON))
        PS_MEM_SET(&stSmaUsimInfo,0X00,sizeof(stSmaUsimInfo));
        stSmaUsimInfo.clientId = MN_CLIENT_ALL;
        stSmaUsimInfo.opId = 0;
        stSmaUsimInfo.usEfId = EF_SMS_SMS_ID;
        stSmaUsimInfo.enDeleteType = MN_MSG_DELETE_MAX;
        stSmaUsimInfo.ucRecIndex = (VOS_UINT8)ulSaveIndex;
        stSmaUsimInfo.enSmaUsimAction = MN_MSG_USIM_RCVNEWMSG;
        ulRet = MN_MSG_WriteSmsFile(pstCfgParm->enSmMemStore,VOS_TRUE,&stSmaUsimInfo,aucSmContent);
#else
        if (MN_MSG_MEM_STORE_SIM == pstCfgParm->enSmMemStore)
        {
            PS_MEM_SET(&stWriteUsimInfo,0X00,sizeof(stWriteUsimInfo));
            stWriteUsimInfo.stUsimInfo.clientId = MN_CLIENT_ALL;
            stWriteUsimInfo.stUsimInfo.opId = 0;
            stWriteUsimInfo.stUsimInfo.usEfId = EF_SMS_SMS_ID;
            stWriteUsimInfo.stUsimInfo.enDeleteType = MN_MSG_DELETE_MAX;
            stWriteUsimInfo.stUsimInfo.ucRecIndex = (VOS_UINT8)ulSaveIndex;
            stWriteUsimInfo.stUsimInfo.enSmaUsimAction = MN_MSG_USIM_RCVNEWMSG;
            stWriteUsimInfo.bCreateNode = VOS_TRUE;
            ulRet = MN_MSG_WriteSmsFile(pstCfgParm->enSmMemStore,VOS_NULL_PTR,&stWriteUsimInfo,aucSmContent);

        }
        else
        {
            ulRet = MN_MSG_WriteSmsFile(pstCfgParm->enSmMemStore,&ulSaveIndex,VOS_NULL_PTR,aucSmContent);
        }
#endif

        if (MN_ERR_NO_ERROR != ulRet)
        {
            MN_MSG_Internal_SendRpt(VOS_FALSE,
                                    MN_MSG_RP_CAUSE_PROTOCOL_ERR_UNSPECIFIED,
                                    MN_MSG_TP_CAUSE_ERR_IN_MS);
            MN_WARN_LOG("MSG_ProcStoreSm:MN_MSG_WriteSmsFile Return Error");
        }
#if (NAS_FEATURE_SMS_NVIM_SMSEXIST == FEATURE_ON)
        else if (MN_MSG_MEM_STORE_NV == pstCfgParm->enSmMemStore)
        {
            MN_MSG_Internal_SendRpt(VOS_TRUE,0,0);
            /*�㲥�ϱ���һ������*/
            bcopId = MN_MSG_GetBCopId();
            stDeliverEvt.ulInex = ulSaveIndex;
            MN_MSG_ReportRcvMsgEvent(bcopId,&stDeliverEvt);
            /*�㲥�ϱ�NVIM�ж����ѷ����˸ı�*/
            MN_MSG_ReportSmChanged(MN_CLIENT_ALL,bcopId,VOS_TRUE,MN_MSG_MEM_STORE_NV);
        }
#endif
        else
        {
        }
    }
}
LOCAL VOS_VOID MSG_ProcNoClassSm(
    const MN_MSG_CONFIG_PARM_STRU       *pstCfgParm,
    const MN_MSG_BCD_ADDR_STRU          *pstScAddr,
    const MN_MSG_RAW_TS_DATA_STRU       *pstRawData,
    const MN_MSG_TS_DATA_INFO_STRU      *pstTsData
)
{
    MN_MSG_MEM_FLAG_ENUM_U8             enAppMemStatus;
    MN_OPERATION_ID_T                   bcopId;
    MN_MSG_DELIVER_EVT_INFO_STRU        stDeliverEvt;
    
    MN_MSG_RCVMSG_ACT_ENUM_U8           enRcvSmAct;                             /*action of received msg*/

    MN_INFO_LOG("MSG_ProcNoClassSm: Step into function.");
    MSG_BuildDeliverEvtParm(pstCfgParm,pstScAddr,pstRawData,&stDeliverEvt);
    bcopId = MN_MSG_GetBCopId();
    MN_NORM_LOG1("MSG_ProcNoClassSm: pstCfgParm->enRcvSmAct is ", pstCfgParm->enRcvSmAct);
    


    /* Ĭ��ȡ�����е�SmAct,��<MT>=3,�յ�����CLASS3����ʱ����Ҫ�޸İ���+CMT��ʽ�ϱ� */
    enRcvSmAct = pstCfgParm->enRcvSmAct;


    /* CLASS3���Ͷ��Ű���+CMT��ʽ�ϱ� */
    if ( ( (MN_MSG_RCVMSG_ACT_STORE    == pstCfgParm->enRcvSmAct)
        && (MN_MSG_CNMI_MT_CLASS3_TYPE  == pstCfgParm->enMtType))
      && (MN_MSG_MSG_CLASS_3            == pstTsData->u.stDeliver.stDcs.enMsgClass) )
    {
        enRcvSmAct = MN_MSG_RCVMSG_ACT_TRANSFER_AND_ACK;
        
        if ( MN_MSG_CSMS_MSG_VERSION_PHASE2_PLUS == pstCfgParm->enSmsServVersion )
        {
            enRcvSmAct = MN_MSG_RCVMSG_ACT_TRANSFER_ONLY;
        }        
    }

    if (MN_MSG_RCVMSG_ACT_DISCARD == enRcvSmAct)
    {
        MN_MSG_Internal_SendRpt(VOS_TRUE,0,0);
        MN_INFO_LOG("MSG_ProcNoClassSm:Rcv Sm but enRcvSmAct is MN_MSG_RCVMSG_ACT_DISCARD");
    }
    else if (MN_MSG_RCVMSG_ACT_STORE == enRcvSmAct)
    {
        MSG_ProcStoreSm(pstCfgParm,pstScAddr,pstRawData,pstTsData);
    }
    else if (MN_MSG_RCVMSG_ACT_TRANSFER_ONLY == enRcvSmAct)
    {
        stDeliverEvt.enMemStore = MN_MSG_MEM_STORE_NONE;
        stDeliverEvt.ulInex     = 0;
        
        /*�㲥�ϱ���һ������*/
        MN_MSG_ReportRcvMsgEvent(bcopId,&stDeliverEvt);
        /*�����ն��������Ϣ*/
        MN_MSG_CreateMtInfo();

    }
    else
    {
        MN_MSG_GetCurAppMemStatus(&enAppMemStatus);
        MN_NORM_LOG1("MSG_ProcNoClassSm: enAppMemStatus is ", enAppMemStatus);
        if (MN_MSG_MEM_FULL_SET == enAppMemStatus)
        {
            MN_MSG_Internal_SendRpt(VOS_FALSE,
                                    MN_MSG_RP_CAUSE_MEMORY_EXCEEDED,
                                    MN_MSG_TP_CAUSE_MEMORY_FULL);

            MN_MSG_UpdateMemExceedFlag(MN_MSG_MEM_FULL_SET);

            MN_WARN_LOG("MSG_ProcNoClassSm:Rcv New Msg but App Mem Full");
        }
        else
        {
            MN_MSG_Internal_SendRpt(VOS_TRUE,0,0);


            /* ֪ͨAT����+CMT��ʽ�洢 */
            stDeliverEvt.enMemStore = MN_MSG_MEM_STORE_NONE;
            stDeliverEvt.ulInex     = 0;
            
            MN_MSG_ReportRcvMsgEvent(bcopId,&stDeliverEvt);
        }
    }
}


LOCAL VOS_VOID MN_MSG_RcvAndStoreClass0Msg(
    const MN_MSG_CONFIG_PARM_STRU       *pstCfgParm,
    const MN_MSG_BCD_ADDR_STRU          *pstScAddr,
    const MN_MSG_RAW_TS_DATA_STRU       *pstRawData,
    const MN_MSG_TS_DATA_INFO_STRU      *pstTsData
)
{
    MN_MSG_CLASS0_TAILOR_U8             enClass0Tailor;
    MN_MSG_DELIVER_EVT_INFO_STRU        stDeliverEvt;
    MN_OPERATION_ID_T                   bcopId;

    /* ��ȡClass0���Ŷ��� */
    MSG_BuildDeliverEvtParm(pstCfgParm,pstScAddr,pstRawData,&stDeliverEvt);

    enClass0Tailor = MN_MSG_GetClass0Tailor();
    if (MN_MSG_CLASS0_VODAFONE == enClass0Tailor)
    {
        /* Vodafone Class0 ���Ŷ���,�洢��ʽ��class 2��������,
        �༴������class 0���ŵ�����ʱ��ֱ�ӽ����Ŵ洢��SIM���У�
        ����CNMI�Լ�CPMS���õ�Ӱ�죬ͬʱ���ã�CMTI�ϱ����Ŵ洢λ�á�*/
        MSG_ProcClass2Msg(pstScAddr,pstRawData);
        return;
    }

    /*1: Italy TIM Class0 ���Ŷ��ƣ���H3G Class0 ���Ŷ��ƣ�
        H3G�������TIM Class 0����������ͬ�����Զ��Ž��д洢��
        Ҫ��CLASS 0 ����ֱ�Ӳ���+CMT���������ϱ�������CNMI
        �Լ�CPMS���õ�Ӱ�죬�����̨�Ѿ��򿪣����̨��CLASS 0���Ž�����ʾ��
      2��27005Э��CNMIָ������ʽ
      <MT>Ϊ0�洢����U��SIM��������AT�˿��ϱ���
      <MT>Ϊ1�洢����U��SIM��������AT�˿��ϱ��洢λ�ã���CMTI��ʽ�ϱ�����
      <MT>Ϊ2���洢����U��SIM��������AT�˿��ϱ��������ݣ���CMT��ʽ�ϱ�����
      <MT>Ϊ3�洢����U��SIM��������AT�˿��ϱ��洢λ�ã���CMTI��ʽ�ϱ�����
      ���������ֻ����AT�ϵ���ʾ��һ��,���Դ˴�����Ҫ�����κδ���
    */
    else if (MN_MSG_CLASS0_TIM == enClass0Tailor)
    {
        if (MN_MSG_RCVMSG_ACT_TRANSFER_ONLY != pstCfgParm->enRcvSmAct)
        {
            /* TIM��VIVO���ƵĲ���:��<MT>=1/3ʱ,��ʹCSMS=1��Ҳ����Ҫ+CNMA ACK */
            MN_MSG_Internal_SendRpt(VOS_TRUE,0,0);
        }
        else
        {
            /*�������Ӧ�ûظ�Rp-Report,�����ն��������Ϣ*/
            MN_MSG_CreateMtInfo();
        }
        bcopId                  = MN_MSG_GetBCopId();
        stDeliverEvt.enMemStore = MN_MSG_MEM_STORE_NONE;
        stDeliverEvt.ulInex     = 0;
        MN_MSG_ReportRcvMsgEvent(bcopId, &stDeliverEvt);
    }
    else if (MN_MSG_CLASS0_VIVO == enClass0Tailor)
    {
        if (MN_MSG_CSMS_MSG_VERSION_PHASE2 == pstCfgParm->enSmsServVersion)
        {
            MN_MSG_Internal_SendRpt(VOS_TRUE,0,0);
        }
        else
        {
            /*�������Ӧ�ûظ�Rp-Report,�����ն��������Ϣ*/
            MN_MSG_CreateMtInfo();
        }
        bcopId                  = MN_MSG_GetBCopId();
        stDeliverEvt.enMemStore = MN_MSG_MEM_STORE_NONE;
        stDeliverEvt.ulInex     = 0;
        MN_MSG_ReportRcvMsgEvent(bcopId, &stDeliverEvt);
    }
    else
    {
        MSG_ProcNoClassSm(pstCfgParm, pstScAddr,pstRawData, pstTsData);
    }

    return;
}
LOCAL VOS_UINT32 MSG_IsRequireDownloadMsg(
    const MN_MSG_TS_DATA_INFO_STRU     *pstTsData
)
{
    MN_MSG_CUSTOM_CFG_INFO_STRU        *pstCustomCfgAddr = VOS_NULL_PTR;
    MN_MSG_USIM_EFUST_INFO_STRU         stEfUstInfo;
    
    PS_MEM_SET(&stEfUstInfo,0X00,sizeof(stEfUstInfo));
    MN_MSG_ReadUstInfo(&stEfUstInfo);

    pstCustomCfgAddr                    = MN_MSG_GetCustomCfgInfo();

    MN_NORM_LOG1("MSG_IsRequireDownloadMsg: pstCustomCfgAddr->ucSmsPpDownlodSupportFlg ", (VOS_INT32)pstCustomCfgAddr->ucSmsPpDownlodSupportFlg);
    MN_NORM_LOG1("MSG_IsRequireDownloadMsg: stEfUstInfo.bSmsPpDataFlag is ", (VOS_INT32)stEfUstInfo.bSmsPpDataFlag);
    MN_NORM_LOG1("MSG_IsRequireDownloadMsg: enPid is ", pstTsData->u.stDeliver.enPid);
    MN_NORM_LOG1("MSG_IsRequireDownloadMsg: enMsgClass is ", pstTsData->u.stDeliver.stDcs.enMsgClass);

    /* �˴���Ҫ�������ucSmsPpDownlodSupportFlg��bSmsPpDataFlag��enMsgClass��enPid���� */

    /* NV������ΪMODEM��֧��PP-DOWNLOAD���ܣ�����DOWNLOAD���� */
    if (MN_MSG_NV_ITEM_ACTIVE != pstCustomCfgAddr->ucSmsPpDownlodSupportFlg)
    {
        return VOS_FALSE;
    }

    /* (U)SIM���ò�֧��PP-DOWNLOAD���ܣ�����DOWNLOAD���� */
    if (VOS_TRUE != stEfUstInfo.bSmsPpDataFlag)
    {
        return VOS_FALSE;
    }
    
    /* ��OTA���ţ�Ҫ��DOWNLOAD */
    if ((MN_MSG_MSG_CLASS_2 == pstTsData->u.stDeliver.stDcs.enMsgClass)
     && ((MN_MSG_TP_PID_SIM_DATA_DOWNLOAD == pstTsData->u.stDeliver.enPid)
      || (MN_MSG_TP_PID_ANSI136_R_DATA == pstTsData->u.stDeliver.enPid)))
    {
        return VOS_TRUE;
    }

    /* ��OTA���ţ���Ҫ��DOWNLOAD */
    return VOS_FALSE;
}
LOCAL VOS_UINT32 MSG_IsVsimCtrlDiscardMsg(
    const MN_MSG_TS_DATA_INFO_STRU     *pstTsData
)
{
    VOS_UINT32                          ulRet;

    /* δ����VSIM�������� */
    ulRet = NAS_VSIM_IsRequireVsimCtrl();
    if (VOS_TRUE != ulRet)
    {
        return VOS_FALSE;
    }

    /* Ҫ��DOWNLOAD�Ķ��Ų���VSIM���Կ��� */
    ulRet = MSG_IsRequireDownloadMsg(pstTsData);
    if (VOS_TRUE == ulRet)
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

LOCAL VOS_VOID MSG_ProcSmsDeliver(
    const MN_MSG_BCD_ADDR_STRU          *pstScAddr,
    const MN_MSG_RAW_TS_DATA_STRU       *pstRawData,
    const MN_MSG_TS_DATA_INFO_STRU      *pstTsData,
    VOS_UINT8                            ucRcvDomain
)
{
    VOS_UINT32                          ulRet;
    MN_MSG_CONFIG_PARM_STRU             stCfgParm;
    VOS_BOOL                            bSupportFlag;
    MN_MSG_TP_CAUSE_ENUM_U8             enTpCause;

    VOS_UINT8                           ucCloseSmsFlg;

    PS_MEM_SET(&stCfgParm, 0, sizeof(stCfgParm));

    /*��ȡ��ǰ�Ķ��ŵ����ò���*/
    MN_MSG_GetCurCfgParm(&stCfgParm);

    MN_NORM_LOG1("MSG_ProcSmsDeliver: stCfgParm.enRcvSmAct is ", stCfgParm.enRcvSmAct);
    MN_NORM_LOG1("MSG_ProcSmsDeliver: enPid is ", pstTsData->u.stDeliver.enPid);
    MN_NORM_LOG1("MSG_ProcSmsDeliver: enMsgClass is ", pstTsData->u.stDeliver.stDcs.enMsgClass);

    f_ulMsgMtNum++;

    /* VSIM����Ҫ�����Ķ��ţ��˳����Ž������� */
    /* ����VSIM��������ʱ����DOWNLOAD�Ķ��ţ�ֱ�Ӷ��� */
    ulRet = MSG_IsVsimCtrlDiscardMsg(pstTsData);
    if (VOS_TRUE == ulRet)
    {
        MN_MSG_Internal_SendRpt(VOS_TRUE,0,0);
        MN_NORM_LOG("MSG_DiscardVsimCtrlMsg: VSIM feature discard deliver message.");
        
        return;
    }
    
    MN_MSG_GetAndSaveActiveMessageInfo(&pstTsData->u.stDeliver);

    /* ��ǰ���Ź����Ƿ�֧�� */
    MN_MSG_GetCloseSMSFeature(&ucCloseSmsFlg);

    /*pid��"Short Message Type 0" (����Э��23040����������Ϣ�����б��棬Ҳ����Ӧ�ò��ϱ�)*/
    if ((MN_MSG_TP_PID_SM_TYPE_0 == pstTsData->u.stDeliver.enPid)
     || (MN_MSG_NVIM_ITEM_ACTIVE == ucCloseSmsFlg))
    {
        MN_MSG_Internal_SendRpt(VOS_TRUE,0,0);
        MN_NORM_LOG("MSG_ProcSmsDeliver:Pid = MN_MSG_TP_PID_SM_TYPE_0,Discard it");
        return;
    }

    MN_MSG_DeliverMsgNotSupport(pstTsData, &bSupportFlag, &enTpCause);
    if (VOS_FALSE == bSupportFlag)
    {
        MN_MSG_Internal_SendRpt(VOS_FALSE, MN_MSG_RP_CAUSE_IE_NON_EXISTENT, enTpCause);
        MN_NORM_LOG1("MSG_ProcSmsDeliver: deliver message is not supported. enTpCause is ", enTpCause);
        return;
    }

    /* ����MTʱ�Ľ�����ֻ����״̬��ΪMN_MSG_MT_STATE_NULLʱ����Ч */
    TAF_MSG_UpdateMtRcvDomain(ucRcvDomain);

    /*TP-PIDΪReplace Short Message Type 1-Replace Short Message Type 7��Return Call Message
    ���½��յ��Ķ����滻֮ǰ�洢�Ķ���*/
    if (VOS_TRUE == MSG_ProcReplaceSm(&stCfgParm,pstScAddr,pstRawData,pstTsData))
    {
        return;
    }

    /*(pid��(U)SIM Data download || pid��MN_MSG_TP_PID_ANSI136_R_DATA) && ����������
    Class 2���͵� &&֧��Data download via SMS-PP(��֧��,����Class 2���ű�����USIM��)*/
    ulRet = MSG_IsRequireDownloadMsg(pstTsData);
    if (VOS_TRUE == ulRet)
    {
        MN_MSG_SetMtTpPidAndDcs(pstTsData->u.stDeliver.enPid, pstTsData->u.stDeliver.stDcs);
        ulRet = MN_MSG_SmsPPEnvelopeReq(pstScAddr,pstRawData->aucData, pstRawData->ulLen);
        if (VOS_ERR == ulRet)
        {
            MN_MSG_Internal_SendRpt(VOS_FALSE,
                                    MN_MSG_RP_CAUSE_PROTOCOL_ERR_UNSPECIFIED,
                                    MN_MSG_TP_CAUSE_ERR_IN_MS);
        }
        return;
    }

    /*Class 2���͵Ķ���ֻ�ܴ洢��USIM��*/
    if (MN_MSG_MSG_CLASS_2 == pstTsData->u.stDeliver.stDcs.enMsgClass)
    {
        MSG_ProcClass2Msg(pstScAddr,pstRawData);
    }

    /*Class 0���͵Ķ��ŵĽ��պʹ洢*/
    else if (MN_MSG_MSG_CLASS_0 == pstTsData->u.stDeliver.stDcs.enMsgClass)
    {
        MN_MSG_RcvAndStoreClass0Msg(&stCfgParm,pstScAddr,pstRawData,pstTsData);
    }

    /*��Class���͵Ķ��ŵĽ��պʹ洢*/
    else
    {
        MSG_ProcNoClassSm(&stCfgParm,pstScAddr,pstRawData,pstTsData);
    }

    return;
}
VOS_VOID MN_MSG_GetStatusReportStorageInfo(
    MN_MSG_STORAGE_INFO_STRU            *pstStorageInfo
)
{
    VOS_UINT32                          ulSmsrIndex;
    VOS_UINT32                          ulSmsrCapacity;
    MN_MSG_CONFIG_PARM_STRU             stCfgParm;
    MN_MSG_CUSTOM_CFG_INFO_STRU        *pstCustomCfgAddr;

    /* ��ȡ���Կ���NV��ַ */
    pstCustomCfgAddr                    = MN_MSG_GetCustomCfgInfo();

    PS_MEM_SET(&stCfgParm, 0, sizeof(stCfgParm));

    MN_MSG_GetCurCfgParm(&stCfgParm);
    pstStorageInfo->enRcvSmAct = stCfgParm.enRcvStaRptAct;

    if (MN_MSG_RCVMSG_ACT_STORE != pstStorageInfo->enRcvSmAct)
    {
        return;
    }

    if (MN_MSG_NV_ITEM_DEACTIVE == pstCustomCfgAddr->ucSmsStatusInEfsmsSupportFlg)
    {
        pstStorageInfo->usFileIndex = EF_SMS_SMSR_ID;
        pstStorageInfo->enMemStore  = stCfgParm.enStaRptMemStore;
        pstStorageInfo->ulIndex     = MN_MSG_NO_AVAILABLE_SMS_REC;

        ulSmsrCapacity = MN_MSG_GetSmsrCapacity(stCfgParm.enStaRptMemStore);
        if (0 != ulSmsrCapacity)
        {
            ulSmsrIndex = MN_MSG_GetAvailSmsrRecIndex(stCfgParm.enStaRptMemStore);
            if (MN_MSG_NO_AVAILABLE_SMS_REC == ulSmsrIndex)
            {
                MN_INFO_LOG("MN_MSG_GetStatusReportStorageInfo: Rcv StatusReport No Free smsr record.");
                /*���޿���λ��,��ֱ���ҵ��滻��һ������״̬���������*/
                ulSmsrIndex = MN_MSG_GetReplaceSmsrRecIndex(stCfgParm.enStaRptMemStore);
            }

            pstStorageInfo->ulIndex = ulSmsrIndex;

        }

        if (MN_MSG_NO_AVAILABLE_SMS_REC == pstStorageInfo->ulIndex)
        {
            if (MN_MSG_CSMS_MSG_VERSION_PHASE2 == stCfgParm.enSmsServVersion)
            {
                pstStorageInfo->enRcvSmAct = MN_MSG_RCVMSG_ACT_TRANSFER_AND_ACK;
            }
            else
            {
                pstStorageInfo->enRcvSmAct = MN_MSG_RCVMSG_ACT_TRANSFER_ONLY;
            }

        }

    }
    else
    {
        pstStorageInfo->enMemStore  = stCfgParm.enSmMemStore;
        pstStorageInfo->usFileIndex = EF_SMS_SMS_ID;
        pstStorageInfo->ulIndex     = MN_MSG_GetAvailSmRecIndex(stCfgParm.enSmMemStore);
    }

    return;
}


VOS_UINT32 MN_MSG_StoreStatusReport(
    const MN_MSG_BCD_ADDR_STRU          *pstScAddr,
    const MN_MSG_RAW_TS_DATA_STRU       *pstRawData,
    const MN_MSG_STORAGE_INFO_STRU      *pstStorageInfo
)
{
    VOS_UINT8                           aucSmsrContent[MN_MSG_EFSMSR_LEN];
    VOS_UINT8                           aucSmContent[MN_MSG_EFSMS_LEN];
    VOS_UINT32                          ulRet;
#if ((NAS_FEATURE_SMS_NVIM_SMSEXIST == FEATURE_ON) && (NAS_FEATURE_SMS_FLASH_SMSEXIST == FEATURE_ON))
    MN_MSG_WRITE_USIM_INFO_STRU         stWriteUsimInfo;
#endif
    MN_MSG_USIM_INFO_STRU               stSmaUsimInfo;
    MN_MSG_CUSTOM_CFG_INFO_STRU        *pstCustomCfgAddr;

    /* ��ȡ���Կ���NV��ַ */
    pstCustomCfgAddr                    = MN_MSG_GetCustomCfgInfo();


    /* ��������״̬�����¼ */
    PS_MEM_SET(&stSmaUsimInfo, 0X00, sizeof(stSmaUsimInfo));
    stSmaUsimInfo.clientId          = MN_CLIENT_ALL;
    stSmaUsimInfo.opId              = 0;
    stSmaUsimInfo.usEfId            = pstStorageInfo->usFileIndex;
    stSmaUsimInfo.enDeleteType      = MN_MSG_DELETE_MAX;
    stSmaUsimInfo.ucRecIndex        = (VOS_UINT8)pstStorageInfo->ulIndex;
    stSmaUsimInfo.enSmaUsimAction   = MN_MSG_USIM_RCVSTARPT;

    if (MN_MSG_NV_ITEM_DEACTIVE == pstCustomCfgAddr->ucSmsStatusInEfsmsSupportFlg)
    {
        MN_MSG_CreateEFSmContent(pstScAddr,
                                 pstRawData,
                                 EF_SMS_STATUS_MT_TO_BE_READ,
                                 aucSmContent);

#if ((NAS_FEATURE_SMS_NVIM_SMSEXIST != FEATURE_ON)\
  || (NAS_FEATURE_SMS_FLASH_SMSEXIST != FEATURE_ON))
        ulRet = MN_MSG_WriteSmsFile(pstStorageInfo->enMemStore,VOS_TRUE,&stSmaUsimInfo,aucSmContent);
#else
        if (MN_MSG_MEM_STORE_SIM == pstStorageInfo->enMemStore)
        {
            PS_MEM_SET(&stWriteUsimInfo,0X00,sizeof(stWriteUsimInfo));
            stWriteUsimInfo.stUsimInfo.clientId = MN_CLIENT_ALL;
            stWriteUsimInfo.stUsimInfo.opId = 0;
            stWriteUsimInfo.stUsimInfo.usEfId = EF_SMS_SMS_ID;
            stWriteUsimInfo.stUsimInfo.enDeleteType = MN_MSG_DELETE_MAX;
            stWriteUsimInfo.stUsimInfo.ucRecIndex = (VOS_UINT8)pstStorageInfo->ulIndex;
            stWriteUsimInfo.stUsimInfo.enSmaUsimAction = MN_MSG_USIM_RCVNEWMSG;
            stWriteUsimInfo.bCreateNode = VOS_TRUE;
            ulRet = MN_MSG_WriteSmsFile(pstStorageInfo->enMemStore,
                                        VOS_NULL_PTR,
                                        &stWriteUsimInfo,
                                        aucSmContent);

        }
        else
        {
            ulRet = MN_MSG_WriteSmsFile(pstStorageInfo->enMemStore,
                                        &pstStorageInfo->ulIndex,
                                        VOS_NULL_PTR,
                                        aucSmContent);
        }
#endif

    }
    else
    {
        /* д����״̬�����¼���ļ� */
        if ((MN_MSG_MEM_STORE_SIM == pstStorageInfo->enMemStore)
         && (EF_SMS_SMSR_ID       == pstStorageInfo->usFileIndex))
        {
            /* Э��ջ��֧��SMS��¼��SMSR�Ĺ�����SMSR��¼�ĵ�һ���ֽ�"������SMS��¼����"��0 */
            MSG_CreateEFSmsrContent(pstRawData, 0, aucSmsrContent);
            ulRet = MN_MSG_WriteSmsrFile(pstStorageInfo->enMemStore,
                                         &stSmaUsimInfo,
                                         aucSmsrContent);

        }
        else
        {
            MN_MSG_CreateEFSmContent(pstScAddr,
                                     pstRawData,
                                     EF_SMS_STATUS_MT_TO_BE_READ,
                                     aucSmContent);

#if ( (NAS_FEATURE_SMS_NVIM_SMSEXIST != FEATURE_ON)\
   || (NAS_FEATURE_SMS_FLASH_SMSEXIST != FEATURE_ON))
            ulRet = MN_MSG_WriteSmsFile(pstStorageInfo->enMemStore,VOS_TRUE,&stSmaUsimInfo,aucSmContent);
#else
            if (MN_MSG_MEM_STORE_SIM == pstStorageInfo->enMemStore)
            {
                PS_MEM_SET(&stWriteUsimInfo,0X00,sizeof(stWriteUsimInfo));
                stWriteUsimInfo.stUsimInfo.clientId = MN_CLIENT_ALL;
                stWriteUsimInfo.stUsimInfo.opId = 0;
                stWriteUsimInfo.stUsimInfo.usEfId = EF_SMS_SMS_ID;
                stWriteUsimInfo.stUsimInfo.enDeleteType = MN_MSG_DELETE_MAX;
                stWriteUsimInfo.stUsimInfo.ucRecIndex = (VOS_UINT8)pstStorageInfo->ulIndex;
                stWriteUsimInfo.stUsimInfo.enSmaUsimAction = MN_MSG_USIM_RCVNEWMSG;
                stWriteUsimInfo.bCreateNode = VOS_TRUE;
                ulRet = MN_MSG_WriteSmsFile(pstStorageInfo->enMemStore,
                                            VOS_NULL_PTR,
                                            &stWriteUsimInfo,
                                            aucSmContent);

            }
            else
            {
                ulRet = MN_MSG_WriteSmsFile(pstStorageInfo->enMemStore,
                                            &pstStorageInfo->ulIndex,
                                            VOS_NULL_PTR,
                                            aucSmContent);
            }
#endif
        }

    }

    return ulRet;
}


LOCAL VOS_VOID MSG_ProcSmsStaRpt(
    const MN_MSG_BCD_ADDR_STRU          *pstScAddr,
    const MN_MSG_RAW_TS_DATA_STRU       *pstRawData,
    const MN_MSG_TS_DATA_INFO_STRU      *pstTsData,
    VOS_UINT8                            ucRcvDomain
)
{
    VOS_UINT32                          ulRet;
    MN_MSG_MEM_FLAG_ENUM_U8             enAppMemStatus;
    MN_OPERATION_ID_T                   bcopId;
    MN_MSG_DELIVER_EVT_INFO_STRU        stDeliverEvt;
    MN_MSG_STORAGE_INFO_STRU            stStorageInfo;

    /* ��ȡ����״̬������ô洢���� */
    PS_MEM_SET(&stStorageInfo, 0x00, sizeof(stStorageInfo));
    MN_MSG_GetStatusReportStorageInfo(&stStorageInfo);

    /* ����״̬���治�洢: ������ظ�RP-ACK,����������״̬���� */
    if (MN_MSG_RCVMSG_ACT_DISCARD == stStorageInfo.enRcvSmAct)
    {
        MN_INFO_LOG("MSG_ProcSmsStaRpt: enRcvSmAct is MN_MSG_RCVMSG_ACT_DISCARD");
        MN_MSG_Internal_SendRpt(VOS_TRUE, 0, 0);
        return;
    }

    /* ����MTʱ�Ľ�����ֻ����״̬��ΪMN_MSG_MT_STATE_NULLʱ����Ч */
    TAF_MSG_UpdateMtRcvDomain(ucRcvDomain);

    /* ����״̬�����ϱ��¼��������: �������ĺ��룬����״̬����PDU���㲥OPID */
    PS_MEM_SET(&stDeliverEvt, 0X00, sizeof(stDeliverEvt));
    PS_MEM_CPY(&stDeliverEvt.stRcvMsgInfo.stScAddr,
               pstScAddr,
               sizeof(stDeliverEvt.stRcvMsgInfo.stScAddr));
    PS_MEM_CPY(&stDeliverEvt.stRcvMsgInfo.stTsRawData,
               pstRawData,
               sizeof(stDeliverEvt.stRcvMsgInfo.stTsRawData));
    bcopId = MN_MSG_GetBCopId();

    /* �洢����״̬���洦������ */
    if (MN_MSG_RCVMSG_ACT_STORE == stStorageInfo.enRcvSmAct)
    {

        /* ��ȡ���д洢��¼ʧ��: ������ظ�RP-ERROR��ͬʱ�ϱ�^SMMEMFULL: <mem> */
        if (MN_MSG_NO_AVAILABLE_SMS_REC == stStorageInfo.ulIndex)
        {
            MN_NORM_LOG("MSG_ProcSmsStaRpt: There is no record available.");
            MN_MSG_FailToWriteEfsms(stStorageInfo.enMemStore,
                                    MN_MSG_WRITE_EFSMS_CAUSE_MEM_FULL);
            return;
        }

        /* �洢����״̬���浽�ļ� */
        ulRet = MN_MSG_StoreStatusReport(pstScAddr, pstRawData, &stStorageInfo);
        if (MN_ERR_NO_ERROR != ulRet)
        {
            MN_WARN_LOG("MSG_ProcSmsStaRpt: MN_MSG_WriteSmsFile Return Error");
            MN_MSG_Internal_SendRpt(VOS_FALSE,
                                    MN_MSG_RP_CAUSE_PROTOCOL_ERR_UNSPECIFIED,
                                    MN_MSG_TP_CAUSE_ERR_IN_MS);
            return;
        }

        /* �洢����״̬���浽�ļ� */
        if (MN_MSG_MEM_STORE_NV == stStorageInfo.enMemStore)
        {
            MN_MSG_Internal_SendRpt(VOS_TRUE, 0, 0);

            /*�㲥�ϱ���һ������*/
            stDeliverEvt.enRcvSmAct = MN_MSG_RCVMSG_ACT_STORE;
            stDeliverEvt.enMemStore = stStorageInfo.enMemStore;
            stDeliverEvt.ulInex     = stStorageInfo.ulIndex;
            MN_MSG_ReportRcvMsgEvent(bcopId, &stDeliverEvt);

            /*�㲥�ϱ�NVIM�ж����ѷ����˸ı�*/
            MN_MSG_ReportSmChanged(MN_CLIENT_ALL, bcopId, VOS_TRUE, MN_MSG_MEM_STORE_NV);
        }

    }
    else if (MN_MSG_RCVMSG_ACT_TRANSFER_ONLY == stStorageInfo.enRcvSmAct)
    {
        MN_MSG_ReportRcvMsgEvent(bcopId,&stDeliverEvt);
        /*�����ն��������Ϣ*/
        MN_MSG_CreateMtInfo();
    }
    else
    {
        MN_MSG_GetCurAppMemStatus(&enAppMemStatus);
        if (MN_MSG_MEM_FULL_SET == enAppMemStatus)
        {
            MN_MSG_UpdateMemExceedFlag(MN_MSG_MEM_FULL_SET);

            MN_WARN_LOG("MSG_ProcSmsStaRpt:Rcv New Status Report but App Mem Full");
        }

        MN_MSG_Internal_SendRpt(VOS_TRUE,0,0);
        MN_MSG_ReportRcvMsgEvent(bcopId,&stDeliverEvt);
    }
}


LOCAL VOS_UINT32 MSG_UpdateEfSmsInfo
(
    const MN_MSG_MO_ENTITY_STRU         *pstMoEntity,
    MN_MSG_MEM_STORE_ENUM_U8            *penMemStore
)
{

    VOS_UINT8                           aucSmContent[MN_MSG_EFSMS_LEN];
    VOS_UINT32                          ulRet;
#if ((NAS_FEATURE_SMS_NVIM_SMSEXIST == FEATURE_ON) && (NAS_FEATURE_SMS_FLASH_SMSEXIST == FEATURE_ON))
    MN_MSG_WRITE_USIM_INFO_STRU         stWriteUsimInfo;
#else
    MN_MSG_USIM_INFO_STRU               stSmaUsimInfo;
#endif

    PS_MEM_SET(aucSmContent,(VOS_CHAR)0xFF,MN_MSG_EFSMS_LEN);

    ulRet = MN_MSG_ReadSmsInfo(pstMoEntity->enSaveArea,
                               pstMoEntity->ulSaveIndex,
                               aucSmContent);

    if (MN_ERR_NO_ERROR == ulRet)
    {
        aucSmContent[0] = EF_SMS_STATUS_MO_SEND_TO_NT;

#if ((NAS_FEATURE_SMS_NVIM_SMSEXIST != FEATURE_ON)\
          || (NAS_FEATURE_SMS_FLASH_SMSEXIST != FEATURE_ON))
        PS_MEM_SET(&stSmaUsimInfo,0X00,sizeof(stSmaUsimInfo));
        stSmaUsimInfo.clientId = pstMoEntity->clientId;
        stSmaUsimInfo.opId = pstMoEntity->opId;
        stSmaUsimInfo.usEfId = EF_SMS_SMS_ID;
        stSmaUsimInfo.enDeleteType = MN_MSG_DELETE_MAX;
        stSmaUsimInfo.ucRecIndex = (VOS_UINT8)pstMoEntity->ulSaveIndex;
        stSmaUsimInfo.enSmaUsimAction = MN_MSG_USIM_RCVMORPT;
        ulRet = MN_MSG_WriteSmsFile(pstMoEntity->enSaveArea,VOS_FALSE,&stSmaUsimInfo,aucSmContent);
#else
        if (MN_MSG_MEM_STORE_SIM == pstMoEntity->enSaveArea)
        {
            PS_MEM_SET(&stWriteUsimInfo,0X00,sizeof(stWriteUsimInfo));
            stWriteUsimInfo.stUsimInfo.clientId = pstMoEntity->clientId;
            stWriteUsimInfo.stUsimInfo.opId = pstMoEntity->opId;
            stWriteUsimInfo.stUsimInfo.usEfId = EF_SMS_SMS_ID;
            stWriteUsimInfo.stUsimInfo.enDeleteType = MN_MSG_DELETE_MAX;
            stWriteUsimInfo.stUsimInfo.ucRecIndex = (VOS_UINT8)pstMoEntity->ulSaveIndex;
            stWriteUsimInfo.stUsimInfo.enSmaUsimAction = MN_MSG_USIM_RCVMORPT;
            stWriteUsimInfo.bCreateNode = VOS_FALSE;
            ulRet = MN_MSG_WriteSmsFile(pstMoEntity->enSaveArea,VOS_NULL_PTR,&stWriteUsimInfo,aucSmContent);

        }
        else
        {
            ulRet = MN_MSG_WriteSmsFile(pstMoEntity->enSaveArea,&pstMoEntity->ulSaveIndex,VOS_NULL_PTR,aucSmContent);
        }
#endif

        if (MN_ERR_NO_ERROR != ulRet)
        {
            MN_WARN_LOG("MSG_UpdateEfSmsInfo:Update Sms in Usim or Nvim Error");
            return VOS_ERR;
        }
        return VOS_OK;
    }
    else
    {
        *penMemStore = MN_MSG_MEM_STORE_NONE;
        MN_WARN_LOG("MSG_ProcSmsRpMoRpRpt:Read Sms Failed");
        return VOS_ERR;
    }
}


VOS_UINT32 MSG_GetMoFailCause(
    const MN_MSG_RAW_TS_DATA_STRU       *pstRawData,
    const SMR_SMT_ERROR_ENUM_UINT32      enErrorCode
)
{
    VOS_UINT32                          ulRet;

    if ((SMR_SMT_ERROR_RP_ERROR_BEGIN == (enErrorCode & SMR_SMT_ERROR_RP_ERROR_BEGIN))
     && (pstRawData->ulLen > 0))
    {
        ulRet = MN_MSG_Decode(pstRawData, &f_stMsgTsData);
        if (MN_ERR_NO_ERROR == ulRet)
        {
            return (f_stMsgTsData.u.stSubmitRptErr.enFailCause | TAF_MSG_ERROR_TP_ERROR_BEGIN);
        }

        MN_WARN_LOG("MSG_GetMoFailCause: MN_MSG_Decode Error");
    }

    return enErrorCode;
}


LOCAL VOS_VOID MSG_ProcSmsRpMoRpRpt(
    const MN_MSG_MO_ENTITY_STRU         *pstMoEntity,
    const MN_MSG_RAW_TS_DATA_STRU       *pstRawData ,
    const SMR_SMT_ERROR_ENUM_UINT32      enErrorCode
)
{
    VOS_UINT32                          ulRet = VOS_FALSE;
    MN_MSG_MEM_STORE_ENUM_U8            enCurMemStore;
    MN_MSG_SUBMIT_RPT_EVT_INFO_STRU     stSubmitRptEvt;
#if (NAS_FEATURE_SMS_NVIM_SMSEXIST == FEATURE_ON)
    MN_OPERATION_ID_T                   bcOpId;
#endif

    PS_MEM_SET(&stSubmitRptEvt,0X00,sizeof(stSubmitRptEvt));
    stSubmitRptEvt.ucMr = pstMoEntity->ucMr;

    /* �˴���Ҫ�����µ�TAF-AT�ӿڣ����ͳɹ������ϱ�STCS��Ϣ�������ȡ����ԭ��ֵ */

    if (MN_MSG_TPDU_SUBMIT_RPT_ACK == pstRawData->enTpduType)
    {
        f_ulMsgMoNum++;

        stSubmitRptEvt.enErrorCode = TAF_MSG_ERROR_NO_ERROR;
        stSubmitRptEvt.enSaveArea  = pstMoEntity->enSaveArea;

        enCurMemStore = pstMoEntity->enSaveArea;
#if (NAS_FEATURE_SMS_NVIM_SMSEXIST == FEATURE_ON)
        if ((MN_MSG_MEM_STORE_SIM == pstMoEntity->enSaveArea)
         || (MN_MSG_MEM_STORE_NV == pstMoEntity->enSaveArea))
#else
        if (MN_MSG_MEM_STORE_SIM == pstMoEntity->enSaveArea)
#endif
        {
            ulRet = MSG_UpdateEfSmsInfo(pstMoEntity,&enCurMemStore);
            MN_INFO_LOG1("MSG_ProcSmsRpMoRpRpt: result of MSG_UpdateEfSmsInfo is ",
                         (VOS_INT32)ulRet);
            stSubmitRptEvt.enSaveArea = enCurMemStore;
            stSubmitRptEvt.ulSaveIndex = pstMoEntity->ulSaveIndex;
#if (NAS_FEATURE_SMS_NVIM_SMSEXIST == FEATURE_ON)
            if ((MN_MSG_MEM_STORE_NV == enCurMemStore)
             && (VOS_OK == ulRet))
            {
                bcOpId = MN_MSG_GetBCopId();
                MN_MSG_ReportSmChanged(pstMoEntity->clientId,bcOpId,VOS_TRUE,MN_MSG_MEM_STORE_NV);
            }
#endif
        }
        MN_MSG_ReportSubmitRptEvent(pstMoEntity->clientId,
                                    pstMoEntity->opId,
                                    (MN_MSG_SUBMIT_RPT_EVT_INFO_STRU *)&stSubmitRptEvt,
                                    MN_MSG_EVT_SUBMIT_RPT);
        MN_INFO_LOG("MSG_ProcSmsRpMoRpRpt:Rcv Rp-Ack");
    }
    else if (MN_MSG_TPDU_SUBMIT_RPT_ERR == pstRawData->enTpduType)
    {
        stSubmitRptEvt.enErrorCode = MSG_GetMoFailCause(pstRawData, enErrorCode);
        /* �˴���Ҫ�����µ�TAF-AT�ӿڣ����ͳɹ������ϱ�STCS��Ϣ�������ȡ����ԭ��ֵ */
        stSubmitRptEvt.enSaveArea = pstMoEntity->enSaveArea;
        stSubmitRptEvt.ulSaveIndex = pstMoEntity->ulSaveIndex;
        MN_MSG_ReportSubmitRptEvent(pstMoEntity->clientId,
                                    pstMoEntity->opId,
                                    (MN_MSG_SUBMIT_RPT_EVT_INFO_STRU *)&stSubmitRptEvt,
                                    MN_MSG_EVT_SUBMIT_RPT);
        MN_INFO_LOG("MSG_ProcSmsRpMoRpRpt:Rcv Rp-Error");
    }
    else
    {
    }
}


LOCAL VOS_VOID MSG_ProcSmsRpSmmaRpRpt(
    const MN_MSG_MO_ENTITY_STRU         *pstMoEntity,
    const MN_MSG_RAW_TS_DATA_STRU       *pstRawData ,
    const SMR_SMT_ERROR_ENUM_UINT32      enErrorCode
)
{
    MN_MSG_MEMSTATUS_EVT_INFO_STRU      stMemStatusEvt;
    MN_OPERATION_ID_T                   bcopId;

    PS_MEM_SET(&stMemStatusEvt,0X00,sizeof(stMemStatusEvt));

    if (MN_MSG_TPDU_SUBMIT_RPT_ACK == pstRawData->enTpduType)
    {
        if (VOS_TRUE == pstMoEntity->bReportFlag)
        {
            MN_MSG_UpdateAppMemStatus(MN_MSG_MEM_FULL_UNSET);
            stMemStatusEvt.bSuccess = VOS_TRUE;
            stMemStatusEvt.ulFailCause = MN_ERR_NO_ERROR;
            stMemStatusEvt.enMemFlag = MN_MSG_MEM_FULL_UNSET;
            MN_MSG_ReportMemStatustEvent(pstMoEntity->clientId,
                                         pstMoEntity->opId,
                                         VOS_FALSE,
                                         &stMemStatusEvt,
                                         MN_MSG_EVT_SET_MEMSTATUS);
            bcopId = MN_MSG_GetBCopId();
            MN_MSG_ReportMemStatustEvent(pstMoEntity->clientId,
                                         bcopId,
                                         VOS_TRUE,
                                         &stMemStatusEvt,
                                         MN_MSG_EVT_MEMSTATUS_CHANGED);
        }

        MN_MSG_UpdateMemExceedFlag(MN_MSG_MEM_FULL_UNSET);

        MN_INFO_LOG("MSG_ProcSmsRpSmmaRpRpt:Rcv Rp-Ack");
    }
    else if (MN_MSG_TPDU_SUBMIT_RPT_ERR == pstRawData->enTpduType)
    {
        if (VOS_TRUE == pstMoEntity->bReportFlag)
        {
            stMemStatusEvt.bSuccess = VOS_FALSE;
            stMemStatusEvt.ulFailCause = MSG_GetMoFailCause(pstRawData, enErrorCode);
            stMemStatusEvt.enMemFlag = MN_MSG_MEM_FULL_SET;
            MN_MSG_ReportMemStatustEvent(pstMoEntity->clientId,
                                         pstMoEntity->opId,
                                         VOS_FALSE,
                                         &stMemStatusEvt,
                                         MN_MSG_EVT_SET_MEMSTATUS);
        }
        MN_INFO_LOG("MSG_ProcSmsRpSmmaRpRpt:Rcv Rp-Error");
    }
    else
    {
    }
}


LOCAL VOS_VOID MSG_RcvSmsRpData(
    SMR_SMT_EST_IND_STRU                *pstEstInd
)
{
    VOS_UINT32                          ulRet;
    MN_MSG_BCD_ADDR_STRU                stScAddr;
    MN_MSG_RAW_TS_DATA_STRU             stTsRawData;
    MN_MSG_RP_DATA_STRU                 stRpData;

    const VOS_UINT8                    *pucRcvMsg = VOS_NULL_PTR;

    pucRcvMsg   = pstEstInd->aucData;

    MN_INFO_LOG("MSG_RcvSmsRpData: step into function.");
    ulRet = MN_MSG_CheckUsimStatus();
    if (MN_ERR_NO_ERROR != ulRet)
    {
#if (FEATURE_IMS == FEATURE_ON)
        if (PS_PID_IMSA == pstEstInd->ulSenderPid)
        {
            MN_MSG_SetMtRouteStackType(TAF_MSG_SIGNALLING_TYPE_CS_OVER_IP);
        }
        else
        {
            MN_MSG_SetMtRouteStackType(TAF_MSG_SIGNALLING_TYPE_NAS_SIGNALLING);
        }
#endif

        MN_MSG_Internal_SendRpt(VOS_FALSE,
                                MN_MSG_RP_CAUSE_PROTOCOL_ERR_UNSPECIFIED,
                                MN_MSG_TP_CAUSE_ERR_IN_MS);
        MN_WARN_LOG("MSG_RcvSmsRpData:Usim is not Ready");
        return;
    }

    PS_MEM_SET(&stScAddr,0X00,sizeof(stScAddr));
    PS_MEM_SET(&stTsRawData,0X00,sizeof(stTsRawData));
    PS_MEM_SET(&f_stMsgTsData,0X00,sizeof(f_stMsgTsData));
    PS_MEM_SET(&stRpData,0X00,sizeof(stRpData));

    /*3GPP 24011 RP DATA (Network to Mobile Station)*/
    MSG_DecodeRpData(pucRcvMsg,&stRpData);
    MN_INFO_LOG1("MSG_RcvSmsRpData: message RP-MR is ", stRpData.ucRpMr);
    PS_MEM_CPY(&stScAddr,&stRpData.stOrgAddr,sizeof(stScAddr));

    stTsRawData.ulLen = stRpData.ucRpUserDataLen;
    PS_MEM_CPY(stTsRawData.aucData,stRpData.aucRpUserData,stTsRawData.ulLen);

    MN_NORM_LOG1("MSG_RcvSmsRpData: message TP-MTI is ", ((stTsRawData.aucData[0]) & (0x03)));
    switch ((stTsRawData.aucData[0]) & (0x03))
    {
        case MN_MSG_TP_MTI_DELIVER:
            stTsRawData.enTpduType = MN_MSG_TPDU_DELIVER;
            break;
        case MN_MSG_TP_MTI_STATUS_REPORT:
            stTsRawData.enTpduType = MN_MSG_TPDU_STARPT;
            break;
        default:
            MN_MSG_Internal_SendRpt(VOS_FALSE,
                                    MN_MSG_RP_CAUSE_MSG_TYPE_NON_EXISTENT,
                                    MN_MSG_TP_CAUSE_ERR_IN_MS);
            MN_WARN_LOG("MSG_RcvSmsRpData:Invalid TPDU Type");
            return;
    }

    ulRet = MN_MSG_Decode(&stTsRawData,&f_stMsgTsData);

#if (FEATURE_IMS == FEATURE_ON)
    if (PS_PID_IMSA == pstEstInd->ulSenderPid)
    {
        MN_MSG_SetMtRouteStackType(TAF_MSG_SIGNALLING_TYPE_CS_OVER_IP);
    }
    else
    {
        MN_MSG_SetMtRouteStackType(TAF_MSG_SIGNALLING_TYPE_NAS_SIGNALLING);
    }
#endif

    /*�˴��Ĵ���ԭ��ӦΪMN_MSG_RP_CAUSE_INVALID_MSG_UNSPECIFIED
    �ο�24011 if the message was not an RP ERROR message, the MS shall ignore
    the message and return an RP ERROR message with cause value #95 "
    semantically incorrect message", if an appropriate connection exists*/
    if (MN_ERR_NO_ERROR != ulRet)
    {
        MN_MSG_Internal_SendRpt(VOS_FALSE,
                                MN_MSG_RP_CAUSE_PROTOCOL_ERR_UNSPECIFIED,
                                MN_MSG_TP_CAUSE_TPDU_NOT_SUPPORTED);
        MN_WARN_LOG("MSG_RcvSmsRpData: Fail to decode TPDU.");
        return;
    }

    MN_NORM_LOG1("MSG_RcvSmsRpData: TPDU type: ",stTsRawData.enTpduType);
    if (MN_MSG_TPDU_DELIVER == stTsRawData.enTpduType)
    {
        NAS_EventReport(WUEPS_PID_SMS, NAS_OM_EVENT_SMS_RECEIVE,
                        &stRpData.ucRpMr, NAS_OM_EVENT_SMS_RECEIVE_LEN);
        /* �����ı�:���ӽ����� */
        MSG_ProcSmsDeliver(&stScAddr,&stTsRawData,&f_stMsgTsData, pstEstInd->ucRcvDomain);
    }
    else
    {
        NAS_EventReport(WUEPS_PID_SMS, NAS_OM_EVENT_SMS_MT_NOTIFY,
                        &stRpData.ucRpMr, NAS_OM_EVENT_SMS_MT_NOTIFY_LEN);
        /* �����ı�:���ӽ����� */
        MSG_ProcSmsStaRpt(&stScAddr,&stTsRawData,&f_stMsgTsData, pstEstInd->ucRcvDomain);
    }

}


LOCAL VOS_VOID  MSG_RcvSmsAttachInd(
    VOS_UINT8                           ucDomain,
    VOS_BOOL                            bAttachFlag
)
{
    MN_MSG_SEND_DOMAIN_ENUM_U8          enSendDomain;
    MN_MSG_MO_STATE_ENUM_U8             enMoState;

    MN_MSG_UpdateRegState(ucDomain,bAttachFlag);

    /*�����ǰ��ATTACH��,��鿴�Ƿ���Ҫ���ͻ�����Ϣ*/
    if(VOS_TRUE == bAttachFlag)
    {
        enMoState = MN_MSG_GetMoState();
        if (MN_MSG_MO_STATE_WAIT_REPORT_IND != enMoState)
        {
            MN_MSG_StartMo(&enSendDomain);
        }
        MN_MSG_StartMemNotification();
    }
}

#if (FEATURE_ON == FEATURE_LTE)
VOS_UINT32  MN_MSG_IsLteNeedSmsRetry(
    LMM_SMS_ERR_CAUSE_ENUM_UINT32       enErrCause
)
{
    MN_MSG_RETRY_FLAG_U8                enRetryFlg;

    switch ( enErrCause )
    {
        case LMM_SMS_ERR_CAUSE_RRC_CONN_NOT_EXIST:
        case LMM_SMS_ERR_CAUSE_OTHERS:
            /* ��Ҫ�����ط� */
            enRetryFlg = MSG_JudgeRetryFlg();

            if (MN_MSG_RETRY_BEGIN == enRetryFlg)
            {
                return VOS_TRUE;
            }

            return VOS_FALSE;

        case LMM_SMS_ERR_CAUSE_UE_MODE_PS_ONLY:
        case LMM_SMS_ERR_CAUSE_ACCESS_BARRED:
        case LMM_SMS_ERR_CAUSE_USIM_CS_INVALID:
        case LMM_SMS_ERR_CAUSE_USIM_PS_INVALID:
        case LMM_SMS_ERR_CAUSE_CS_SER_NOT_AVAILABLE:
        case LMM_SMS_ERR_CAUSE_CS_ATTACH_NOT_ALLOWED:
        case LMM_SMS_ERR_CAUSE_PS_ATTACH_NOT_ALLOWED:
        case LMM_SMS_ERR_CAUSE_T3402_RUNNING:
            return VOS_FALSE;

        default:
            MN_WARN_LOG("MN_MSG_IsLteNeedSmsRetry:error cause invalid");
            return VOS_FALSE;
    }


}

#endif
VOS_UINT32  MN_MSG_IsGuNeedSmsRetry(
    SMR_SMT_ERROR_ENUM_UINT32           enErrCause
)
{
    NAS_MMCM_REL_CAUSE_ENUM_UINT32      enMmCmErrCause;

    /* ��CSʧ��ԭ��ֵ�����ж�CAUSEֵ */
    if ( SMR_SMT_ERROR_CS_ERROR_BEGIN == (enErrCause & SMR_SMT_ERROR_CS_ERROR_BEGIN) )
    {
        enMmCmErrCause = (NAS_MMCM_REL_CAUSE_ENUM_UINT32)(enErrCause - SMR_SMT_ERROR_CS_ERROR_BEGIN);

        switch ( enMmCmErrCause )
        {
            /* ��Ҫ���ж����ط� */
            case NAS_MMCM_REL_CAUSE_AS_REJ_LOW_LEVEL_FAIL :
            case NAS_MMCM_REL_CAUSE_MM_WRONG_STATE :
            case NAS_MMCM_REL_CAUSE_MM_NO_SERVICE :
            case NAS_MMCM_REL_CAUSE_MM_LIMIT_SERVICE :
            case NAS_MMCM_REL_CAUSE_MM_TIMER_T3230_EXP :
                
                return VOS_TRUE;

            /* CSʧ������ԭ��ֵ����Ҫ���ж����ط� */
            case NAS_MMCM_REL_CAUSE_AS_REJ_OTHER_CAUSES :
            case NAS_MMCM_REL_CAUSE_MM_REJ_OTHER_CAUSES :
                return VOS_FALSE;

            /* Ĭ����Ҫ�����ط� */
            default:
                MN_WARN_LOG("MN_MSG_IsGuNeedSmsRetry:error cause invalid");
                return VOS_TRUE;
        }
        
    }

    /* ��CS��ʧ�ܣ���Ҫ�����ط� */
    return VOS_TRUE;
    
}



VOS_UINT8 MN_MSG_StartLinkCtrl(VOS_VOID)
{
    VOS_UINT8                           ucInCallFlg;
    VOS_UINT8                           ucRat;

    /* 2G���뼼�����ں����в����������������� */
    ucInCallFlg = TAF_SDC_GetCsCallExistFlg();
    ucRat       = TAF_SDC_GetSysMode();
    if ((VOS_TRUE == ucInCallFlg)
     && (TAF_SDC_SYS_MODE_GSM == ucRat))
    {
        return MN_MSG_LINK_CTRL_DISABLE;
    }
    
    return MN_MSG_GetLinkCtrlParam();
}
VOS_VOID  MN_MSG_RcvSmsRpRpt(SMR_SMT_MO_REPORT_STRU *pstMsg)
{
    VOS_UINT32                          ulRet;
    MN_MSG_RAW_TS_DATA_STRU             stRawData;
    MN_MSG_MO_ENTITY_STRU               stMoEntity;
    MN_MSG_RP_CAUSE_STRU                stRpCause;
    MN_MSG_RP_ACK_STRU                  stRpAck;
    MN_MSG_RP_ERR_STRU                  stRpErr;
    MN_MSG_SEND_DOMAIN_ENUM_U8          enSendDomain;
    MN_MSG_LINK_CTRL_U8                 enLinkCtrl;
    MN_MSG_SEND_FAIL_FLAG_U8            enSendFailFlag;
    VOS_BOOL                            bIsTimerStart;
    VOS_BOOL                            bRetryProcess = VOS_FALSE;

#if (FEATURE_ON == FEATURE_LTE)
    LMM_SMS_ERR_CAUSE_ENUM_UINT32       enErrorCause;
#endif

    PS_MEM_SET(&stRawData,0X00,sizeof(stRawData));
    PS_MEM_SET(&f_stMsgTsData,0X00,sizeof(f_stMsgTsData));
    PS_MEM_SET(&stMoEntity,0X00,sizeof(stMoEntity));
    PS_MEM_SET(&stRpCause,0X00,sizeof(stRpCause));
    PS_MEM_SET(&stRpAck,0X00,sizeof(stRpAck));
    PS_MEM_SET(&stRpErr,0X00,sizeof(stRpErr));

    MN_MSG_GetMoEntity(&stMoEntity);
    if (MN_MSG_MO_STATE_WAIT_REPORT_IND != stMoEntity.enSmaMoState)
    {
        MN_WARN_LOG("MSG_RcvSmsRpRpt:Mo State is NULL");
        return;
    }

    if (SMR_SMT_ERROR_NO_ERROR == pstMsg->enErrorCode)
    {
        MSG_DecodeRpAck(pstMsg->stRpduData.aucData, (VOS_UINT8)pstMsg->stRpduData.ulDataLen, &stRpAck);
        if (VOS_TRUE == stRpAck.bRpUserDataExist)
        {
            stRawData.ulLen = stRpAck.ucRpUserDataLen;
            PS_MEM_CPY(stRawData.aucData,stRpAck.aucRpUserData,stRawData.ulLen);
        }
        stRawData.enTpduType = MN_MSG_TPDU_SUBMIT_RPT_ACK;
        /* �жϵ�ǰ�����ط����ڶ�ʱ���Ƿ�������,����Ѿ�����,��ֹͣ��ʱ�� */
        bIsTimerStart = MN_MSG_IsTimerStarting(MN_MSG_ID_WAIT_RETRY_PERIOD);
        if (VOS_TRUE == bIsTimerStart)
        {
            MN_MSG_StopTimer(MN_MSG_ID_WAIT_RETRY_PERIOD);
        }
        /* ���µ�ǰ�������ͳɹ� */
        enSendFailFlag = MN_MSG_GetSendFailFlag();
        if ( (MN_MSG_SEND_DOMAIN_PS == stMoEntity.enSendDomain)
          && (MN_MSG_SEND_FAIL_PS_DOMAIN == (enSendFailFlag & MN_MSG_SEND_FAIL_PS_DOMAIN)))
        {
            enSendFailFlag ^= MN_MSG_SEND_FAIL_PS_DOMAIN;
            MN_MSG_UpdateSendFailFlag(enSendFailFlag);
        }
        if ( (MN_MSG_SEND_DOMAIN_CS == stMoEntity.enSendDomain)
          && (MN_MSG_SEND_FAIL_CS_DOMAIN == (enSendFailFlag & MN_MSG_SEND_FAIL_CS_DOMAIN)))
        {
            enSendFailFlag ^= MN_MSG_SEND_FAIL_CS_DOMAIN;
            MN_MSG_UpdateSendFailFlag(enSendFailFlag);
        }
    }
    else
    {
        /* ���µ�ǰ��������ʧ��*/
        enSendFailFlag = MN_MSG_GetSendFailFlag();
        if ( (MN_MSG_SEND_DOMAIN_PS == stMoEntity.enSendDomain)
          && (MN_MSG_SEND_FAIL_PS_DOMAIN != (enSendFailFlag & MN_MSG_SEND_FAIL_PS_DOMAIN)))
        {
            enSendFailFlag |= MN_MSG_SEND_FAIL_PS_DOMAIN;
            MN_MSG_UpdateSendFailFlag(enSendFailFlag);
        }
        if ( (MN_MSG_SEND_DOMAIN_CS == stMoEntity.enSendDomain)
          && (MN_MSG_SEND_FAIL_CS_DOMAIN != (enSendFailFlag & MN_MSG_SEND_FAIL_CS_DOMAIN)))
        {
            enSendFailFlag |= MN_MSG_SEND_FAIL_CS_DOMAIN;
            MN_MSG_UpdateSendFailFlag(enSendFailFlag);
        }

#if (FEATURE_ON == FEATURE_LTE)


        if ((NAS_GMM_NET_RAT_TYPE_LTE == GMM_GetCurNetwork())
         && (SMR_SMT_ERROR_EPS_ERROR_BEGIN == (pstMsg->enErrorCode & SMR_SMT_ERROR_EPS_ERROR_BEGIN)))
        {
            enErrorCause = (LMM_SMS_ERR_CAUSE_ENUM_UINT32)(pstMsg->enErrorCode - SMR_SMT_ERROR_EPS_ERROR_BEGIN);
            if (VOS_TRUE == MN_MSG_IsLteNeedSmsRetry(enErrorCause))
            {
                /* ��Ҫ�����ط�,�����ط������ʱ��,��ʱ����ʱ���ٽ����ط� */
                MN_MSG_StartTimer(MN_MSG_ID_WAIT_RETRY_INTERVAL, 0);
                MN_MSG_SaveRpErrInfo(pstMsg);
                return;
            }

            /* �ط����̽���,����Ҫ�����κβ���,ֹͣ��ʱ�� */
            MN_MSG_StopTimer(MN_MSG_ID_WAIT_RETRY_PERIOD);
            stRawData.enTpduType = MN_MSG_TPDU_SUBMIT_RPT_ERR;
            stRawData.ulLen      = 0;
            MN_MNTN_RecordSmsMoFailure(MSG_GetMoFailCause(&stRawData, pstMsg->enErrorCode));
#if (FEATURE_ON == FEATURE_PTM)
            /* ��¼���ŷ����쳣log */
            MN_MSG_FailErrRecord(pstMsg->enErrorCode);
#endif
        }
        else
#endif

        {
            
            /* �жϵ�ǰ�Ƿ���Ҫ�����ط� */
            bRetryProcess = MSG_IsProcessRetry(&stMoEntity, pstMsg->enErrorCode);
            
            if (VOS_TRUE == bRetryProcess)
            {
                MN_MSG_SaveRpErrInfo(pstMsg);
                return;
            }

            MSG_DecodeRpErr(pstMsg->stRpduData.aucData, (VOS_UINT8)pstMsg->stRpduData.ulDataLen, &stRpErr);
            PS_MEM_CPY(&stRpCause,&stRpErr.stRpCause,sizeof(stRpCause));
            if (VOS_TRUE == stRpErr.bRpUserDataExist)
            {
                stRawData.ulLen = stRpErr.ucRpUserDataLen;
                PS_MEM_CPY(stRawData.aucData,stRpErr.aucRpUserData,stRawData.ulLen);
            }
            stRawData.enTpduType = MN_MSG_TPDU_SUBMIT_RPT_ERR;

            MN_MNTN_RecordSmsMoFailure(MSG_GetMoFailCause(&stRawData, pstMsg->enErrorCode));
#if (FEATURE_ON == FEATURE_PTM)
            /* ��¼���ŷ����쳣log */
            MN_MSG_FailErrRecord(pstMsg->enErrorCode);
#endif

        }

    }

    if ((MN_MSG_MO_TYPE_MO == stMoEntity.enSmsMoType)
     || (MN_MSG_MO_TYPE_BUFFER_STK == stMoEntity.enSmsMoType))
    {
        MSG_ProcSmsRpMoRpRpt(&stMoEntity, &stRawData, pstMsg->enErrorCode);
    }
    else
    {
        MSG_ProcSmsRpSmmaRpRpt(&stMoEntity,&stRawData, pstMsg->enErrorCode);
    }

    MN_MSG_DestroyMoInfo();

    MN_MSG_StopTimer(MN_MSG_TID_LINK_CTRL);
    enLinkCtrl = MN_MSG_StartLinkCtrl();
    if (MN_MSG_LINK_CTRL_DISABLE == enLinkCtrl)
    {
        MN_MSG_SendSmsCpAckReq();
        /*�鿴�Ƿ��л�����Ϣ,��������,û�л�����Ϣ����һ������Ϣ��ȥ,�л�����Ϣ����һ���µĶ���*/
        ulRet = MN_MSG_StartMo(&enSendDomain);
    }
    else
    {
        /*�鿴�Ƿ��л�����Ϣ,��������,û�л�����Ϣ����һ������Ϣ��ȥ,�л�����Ϣ����һ���µĶ���*/
        ulRet = MN_MSG_StartMo(&enSendDomain);
        if ((VOS_OK != ulRet) /*��ǰû����Ϣ��Ҫ���ͣ��ȴ��û�����ָʾ*/
         || (MN_MSG_SEND_DOMAIN_CS == enSendDomain))/*��ǰ��Ϣ����ϢҪ���ͣ��ҷ�����ΪCS��*/
        {
            MN_MSG_StartTimer(MN_MSG_TID_LINK_CTRL, 0);
        }
    }

    return;
}


LOCAL VOS_VOID MSG_RcvSmsMtErrInd(SMR_SMT_MT_ERR_STRU *pstMtErr)
{
    MN_MSG_CUSTOM_CFG_INFO_STRU                   *pstMsCfgInfo = VOS_NULL_PTR;

    pstMsCfgInfo = MN_MSG_GetCustomCfgInfo();

    MN_MSG_DestroyMtInfo();

    if ((VOS_TRUE == pstMsCfgInfo->ucLocalStoreFlg)
     && (TAF_MSG_ERROR_TR2M_TIMEOUT == pstMtErr->enErrorCode))
    {
        MN_MSG_InitRcvPath();
    }

    MN_MSG_ReportDeliverErr((TAF_MSG_ERROR_ENUM_UINT32)pstMtErr->enErrorCode);
}


VOS_VOID MN_MSG_ProcSmsMsg(
    VOS_VOID                            *pMsg
)
{
    SMT_SMR_DATA_STRU                  *pstData = (SMT_SMR_DATA_STRU*)pMsg;
    SMR_SMT_ATTACH_STRU                *pstAttachData;

    switch (pstData->ulMsgName)
    {
        case SMR_SMT_ATTACH_IND:
            pstAttachData = (SMR_SMT_ATTACH_STRU *)pstData->aucData;
            MSG_RcvSmsAttachInd(pstAttachData->ucDomain, pstAttachData->bAttachFlag);
            break;
        case SMR_SMT_DATA_IND:
            MSG_RcvSmsRpData((SMR_SMT_EST_IND_STRU *)pMsg);
            break;
        case SMR_SMT_REPORT_IND:
            MN_MSG_RcvSmsRpRpt((SMR_SMT_MO_REPORT_STRU *)pstData);
            break;
        case SMR_SMT_MT_ERR_IND:
            MSG_RcvSmsMtErrInd((SMR_SMT_MT_ERR_STRU *)pstData);
#if (FEATURE_ON == FEATURE_PTM)
            /* ��¼���ŷ����쳣log */
            MN_MSG_FailErrRecord(((SMR_SMT_MT_ERR_STRU *)pstData)->enErrorCode);
#endif
            break;
        case SMR_SMT_LINK_CLOSE_IND:
            MN_MSG_StopTimer(MN_MSG_TID_LINK_CTRL);
            break;

        default:
            MN_WARN_LOG("MN_MSG_ProcSmsMsg:Error MsgName");
            break;
    }

}
VOS_VOID MN_MSG_PrintMoNum(VOS_VOID)
{
    MN_INFO_LOG1("MN_MSG_PrintMoNum: f_ulMsgMoNum is ", (VOS_INT32)f_ulMsgMoNum);
    return;
}


VOS_VOID MN_MSG_PrintMtNum(VOS_VOID)
{
    MN_INFO_LOG1("MN_MSG_PrintMtNum: f_ulMsgMtNum is ", (VOS_INT32)f_ulMsgMtNum);
    return;
}


VOS_UINT32 MN_MSG_AppStorageUnavailable(
    MN_MSG_RCVMSG_ACT_ENUM_U8           enRcvSmAct,
    MN_MSG_MEM_FLAG_ENUM_U8             enAppMemStatus,
    VOS_BOOL                           *pbUnavailableFlag
)
{

    if ((MN_MSG_RCVMSG_ACT_TRANSFER_AND_ACK != enRcvSmAct)
     && (MN_MSG_RCVMSG_ACT_TRANSFER_ONLY != enRcvSmAct)
     && (MN_MSG_RCVMSG_ACT_DISCARD != enRcvSmAct))
    {
        MN_WARN_LOG("MN_MSG_AppStorageUnavailable: invalid input parameter.");
        return MN_ERR_INVALIDPARM;
    }

    if (MN_MSG_RCVMSG_ACT_DISCARD == enRcvSmAct)
    {
        *pbUnavailableFlag = VOS_FALSE;
        return MN_ERR_NO_ERROR;
    }

    if (MN_MSG_MEM_FULL_SET == enAppMemStatus)
    {
        *pbUnavailableFlag = VOS_TRUE;
        return MN_ERR_NO_ERROR;
    }

    *pbUnavailableFlag = VOS_FALSE;
    return MN_ERR_NO_ERROR;
}


VOS_UINT32 MN_MSG_UeStorageUnavailable(
    MN_MSG_MEM_STORE_ENUM_U8            enSmMemStore,
    VOS_BOOL                           *pbUnavailableFlag
)
{
    VOS_UINT32                           ulRecInex;

    if ((MN_MSG_MEM_STORE_SIM != enSmMemStore)
     && (MN_MSG_MEM_STORE_NV != enSmMemStore))
    {
        MN_WARN_LOG("MN_MSG_UeStorageUnavailable: invalid input parameter.");
        return MN_ERR_INVALIDPARM;
    }

    ulRecInex = MN_MSG_GetAvailSmRecIndex(enSmMemStore);
    if (MN_MSG_NO_AVAILABLE_SMS_REC == ulRecInex)
    {
        *pbUnavailableFlag = VOS_TRUE;
        return MN_ERR_NO_ERROR;
    }

   *pbUnavailableFlag = VOS_FALSE;
   return MN_ERR_NO_ERROR;
}


VOS_UINT32 MN_MSG_ProcMoCtrl(
    MN_CLIENT_ID_T                      ClientId,
    MN_OPERATION_ID_T                   OpId,
    const VOS_UINT8                     aucRpDataInfo[]
)
{
    MN_MSG_RP_DATA_STRU                 stRpData;
    MN_MSG_BCD_ADDR_STRU                stDestAddr;
    MN_MSG_RAW_TS_DATA_STRU             stTsRawData;
    VOS_UINT32                          ulRet;
    VOS_BOOL                            bBufferEntity;
    VOS_UINT32                          ulIndex;
    MN_MSG_MO_ENTITY_STRU               stMoEntity;

    ulRet = MN_MSG_GetSpecificStatusMoEntity(MN_MSG_MO_STATE_WAIT_SMS_CTRL_RSP,
                                             &bBufferEntity,
                                             &ulIndex,
                                             &stMoEntity);
    if (MN_ERR_NO_ERROR == ulRet)
    {
        MN_WARN_LOG("MN_MSG_ProcMoCtrl:Usim is now Enveloping");
        return MN_ERR_CLASS_SMS_USIM_ENVELOPEPENDING;
    }

    /* ��ȡSUBMIT��Ϣ����: �������ĺ��룬����Ŀ�ĺ��� */
    PS_MEM_SET(&stRpData, 0x00, sizeof(stRpData));
    MSG_DecodeRpData(aucRpDataInfo, &stRpData);

    PS_MEM_SET(&stTsRawData, 0x00, sizeof(stTsRawData));
    stTsRawData.ulLen       = stRpData.ucRpUserDataLen;
    stTsRawData.enTpduType  = MN_MSG_TPDU_SUBMIT;
    PS_MEM_CPY(stTsRawData.aucData,stRpData.aucRpUserData, stRpData.ucRpUserDataLen);
    ulRet = MN_MSG_Decode(&stTsRawData, &f_stMsgTsData);
    if (MN_ERR_NO_ERROR != ulRet)
    {
        MN_WARN_LOG("MN_MSG_ProcMoCtrl: MN_MSG_Decode fail.");
        return ulRet;
    }

    /* ת������Ŀ�ĺ�������ΪENVELOPE�����BCD������ */
    PS_MEM_SET(&stDestAddr, 0x00, sizeof(stDestAddr));
    ulRet = TAF_STD_ConvertAsciiAddrToBcd(&f_stMsgTsData.u.stSubmit.stDestAddr,
                                  &stDestAddr);
    if (MN_ERR_NO_ERROR != ulRet)
    {
        MN_WARN_LOG("MN_MSG_ProcMoCtrl: get TP-DA fail.");
        return ulRet;
    }

    /* ����LAI��Ϣ����װ������ENVELPE��Ϣ��USIM */
    ulRet = MN_MSG_MoSmsControlEnvelopeReq(ClientId, &stRpData.stDestAddr, &stDestAddr);

    return ulRet;
}
VOS_UINT32 MN_MSG_SendFdnCheckInfo(MN_MSG_MO_ENTITY_STRU *pstMoEntity)
{
    PS_PB_FDN_NUM_STRU                  stFdnInfo;
    MN_MSG_RP_DATA_STRU                 stRpData;
    MN_MSG_BCD_ADDR_STRU                stDestAddr;
    MN_MSG_RAW_TS_DATA_STRU             stTsRawData;
    VOS_UINT32                          ulRet;

    /* ��ȡSUBMIT��Ϣ����: �������ĺ��룬����Ŀ�ĺ��� */
    PS_MEM_SET(&stRpData, 0x00, sizeof(stRpData));
    MSG_DecodeRpData(pstMoEntity->aucRpDataInfo, &stRpData);

    PS_MEM_SET(&stTsRawData,0X00,sizeof(stTsRawData));
    stTsRawData.ulLen       = stRpData.ucRpUserDataLen;
    stTsRawData.enTpduType  = MN_MSG_TPDU_SUBMIT;
    PS_MEM_CPY(stTsRawData.aucData,stRpData.aucRpUserData, stRpData.ucRpUserDataLen);

    ulRet = MN_MSG_Decode(&stTsRawData, &f_stMsgTsData);
    if (MN_ERR_NO_ERROR != ulRet)
    {
        MN_WARN_LOG("MN_MSG_ProcMoCtrl: MN_MSG_Decode fail.");
        return ulRet;
    }

    /* ת������Ŀ�ĺ�������ΪFDN��������BCD������ */
    PS_MEM_SET(&stDestAddr, 0x00, sizeof(stDestAddr));

    ulRet = TAF_STD_ConvertAsciiAddrToBcd(&f_stMsgTsData.u.stSubmit.stDestAddr,
                                  &stDestAddr);
    if (MN_ERR_NO_ERROR != ulRet)
    {
        MN_WARN_LOG("MN_MSG_ProcMoCtrl: get TP-DA fail.");
        return ulRet;
    }

    PS_MEM_SET(&stFdnInfo, 0x00, sizeof(stFdnInfo));

    stFdnInfo.ulNum1Len = stRpData.stDestAddr.ucBcdLen;
    PS_MEM_CPY(stFdnInfo.aucNum1,
               stRpData.stDestAddr.aucBcdNum,
               stFdnInfo.ulNum1Len);

    stFdnInfo.ulNum2Len = stDestAddr.ucBcdLen;
    PS_MEM_CPY(stFdnInfo.aucNum2,
               stDestAddr.aucBcdNum,
               stFdnInfo.ulNum2Len);

    ulRet = NAS_PBAPI_FdnNumCheck(WUEPS_PID_TAF, 0, pstMoEntity->clientId, &stFdnInfo);

    if (VOS_OK != ulRet)
    {
        return ulRet;
    }

    return MN_ERR_NO_ERROR;
}



#if ( VOS_WIN32 == VOS_OS_VER )
VOS_VOID MN_MSG_StubClearEfsmssFlag(VOS_VOID)
{
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulStoreIndex;
    MN_MSG_STORE_MSG_STRU               stStoreMsg;

    MN_MSG_UpdateMemExceedFlag(MN_MSG_MEM_FULL_UNSET);

    /* �ͷŻ����SMMA��Ϣ */
    ulStoreIndex = MN_MSG_GetStoreMsg(&stStoreMsg);
    if ((MN_MSG_NO_AVAILABLE_SMS_REC != ulStoreIndex)
     && (MN_MSG_MO_TYPE_SMMA == stStoreMsg.stMoInfo.enSmsMoType))
    {
        MN_MSG_FreeStoreMsg(ulStoreIndex);
    }

    MN_INFO_LOG("MN_MSG_StubClearEfsmssFlag: SMSS clear stub.");

    return;
}
#endif
VOS_UINT32 MN_MSG_CheckMemAvailable(VOS_VOID)
{
    VOS_UINT32                          ulRet;
    VOS_BOOL                            bUnavailableFlag;
    MN_MSG_CONFIG_PARM_STRU             stCfgParm;
    MN_MSG_MT_CUSTOMIZE_ENUM_UINT8      enMtCustomize;
    VOS_BOOL                            bSmUnavailableFlag;

    MN_INFO_LOG("MN_MSG_CheckMemAvailable: step into function.");

    PS_MEM_SET(&stCfgParm, 0, sizeof(stCfgParm));

    #if ( VOS_WIN32 == VOS_OS_VER )
    if (0 != g_ucSetEfSmssClear)
    {
        MN_MSG_StubClearEfsmssFlag();
        return VOS_ERR;
    }
    #endif

    bUnavailableFlag    = VOS_TRUE;
    bSmUnavailableFlag  = VOS_TRUE;
    enMtCustomize       = MN_MSG_MT_CUSTOMIZE_NONE;
    MN_MSG_GetMtCustomizeInfo(&enMtCustomize);

    /* ��ȡ�û��Ķ��Ž��մ洢���� */
    MN_MSG_GetCurCfgParm(&stCfgParm);

    /* ���û����õĶ��Ŵ洢��ʽΪ�����ݿ��洢������APP���Ž��չ����Ƿ���� */
    if (MN_MSG_RCVMSG_ACT_STORE != stCfgParm.enRcvSmAct)
    {
        ulRet = MN_MSG_AppStorageUnavailable(stCfgParm.enRcvSmAct,
                                            stCfgParm.enAppMemStatus,
                                            &bUnavailableFlag);
    }
    /* ���û����õĶ��Ŵ洢��ʽΪ���ݿ��洢��
       ��������û����õĴ洢���ʼ�����ݿ��洢�����Ž��չ����Ƿ���� */
    else
    {
        ulRet = MN_MSG_UeStorageUnavailable(stCfgParm.enSmMemStore, &bUnavailableFlag);
    }

    if (MN_ERR_NO_ERROR != ulRet)
    {
        return VOS_ERR;
    }

    /*
    DCM����Ҫ��: (U)SIM���û�ָ���Ĵ洢���ʾ�Ϊ����״̬����ʾUE�Ķ��Ŵ洢���ʿ��У�
    ���߰汾Ҫ��: (U)SIM���û�ָ���Ĵ洢���ʿ��У���ʾUE�Ķ��Ŵ洢���ʿ��У�
    */
    ulRet = MN_MSG_UeStorageUnavailable(MN_MSG_MEM_STORE_SIM, &bSmUnavailableFlag);
    if (MN_ERR_NO_ERROR != ulRet)
    {
        return VOS_ERR;
    }

    if (MN_MSG_MT_CUSTOMIZE_DCM == enMtCustomize)
    {
        if ((VOS_TRUE != bSmUnavailableFlag)
         && (VOS_TRUE != bUnavailableFlag))
        {
            return VOS_OK;
        }

    }
    else
    {
        if ((VOS_TRUE != bSmUnavailableFlag)
         || (VOS_TRUE != bUnavailableFlag))
        {
            return VOS_OK;
        }
    }

    return VOS_ERR;

}


VOS_UINT32 MN_MSG_MoCtrlAllowedWithModification(
    SI_STK_MOSMSCTRL_RSP_STRU          *pstMoCtrlRsp,
    MN_MSG_MO_ENTITY_STRU              *pstMoEntity
)
{
    MN_MSG_BCD_ADDR_STRU                stTpBcdAddr;
    MN_MSG_RP_DATA_STRU                 stRpData;
    MN_MSG_RAW_TS_DATA_STRU             stTsRawData;
     VOS_UINT32                         ulRet;

    MSG_DecodeRpData(pstMoEntity->aucRpDataInfo, &stRpData);

    if (0 != pstMoCtrlRsp->OP_Addr1)
    {
        if ((pstMoCtrlRsp->Addr1.ucLen > MN_MSG_MAX_BCD_NUM_LEN)
         || (0 == pstMoCtrlRsp->Addr1.ucLen))
        {
            return MN_ERR_CLASS_SMS_MO_CTRL_USIM_PARA_ERROR;
        }

        stRpData.stDestAddr.ucBcdLen = pstMoCtrlRsp->Addr1.ucLen;
        stRpData.stDestAddr.addrType = pstMoCtrlRsp->Addr1.ucNumType;
        PS_MEM_CPY(stRpData.stDestAddr.aucBcdNum,
                   pstMoCtrlRsp->Addr1.pucAddr,
                   pstMoCtrlRsp->Addr1.ucLen);
    }

    /* ��Ӧ��Ϣ�д���TP-DA���¶���Ŀ�ĺ��� */
    if (0 != pstMoCtrlRsp->OP_Addr2)
    {
        if ((pstMoCtrlRsp->Addr2.ucLen > MN_MSG_MAX_BCD_NUM_LEN)
         || (0 == pstMoCtrlRsp->Addr2.ucLen))
        {
            return MN_ERR_CLASS_SMS_MO_CTRL_USIM_PARA_ERROR;
        }

        stTpBcdAddr.ucBcdLen = pstMoCtrlRsp->Addr2.ucLen;
        stTpBcdAddr.addrType = pstMoCtrlRsp->Addr2.ucNumType;
        PS_MEM_CPY(stTpBcdAddr.aucBcdNum,
                   pstMoCtrlRsp->Addr2.pucAddr,
                   pstMoCtrlRsp->Addr2.ucLen);

        PS_MEM_SET(&stTsRawData,0X00,sizeof(stTsRawData));
        stTsRawData.ulLen = stRpData.ucRpUserDataLen;
        stTsRawData.enTpduType = MN_MSG_TPDU_SUBMIT;
        PS_MEM_CPY(stTsRawData.aucData,stRpData.aucRpUserData, stRpData.ucRpUserDataLen);

        ulRet = MN_MSG_Decode(&stTsRawData, &f_stMsgTsData);
        if (MN_ERR_NO_ERROR != ulRet)
        {
            return ulRet;
        }

        ulRet = MN_MSG_BcdAddrToAscii(&stTpBcdAddr, &f_stMsgTsData.u.stSubmit.stDestAddr);
        if (MN_ERR_NO_ERROR != ulRet)
        {
            return ulRet;
        }

        ulRet = MN_MSG_Encode(&f_stMsgTsData, &stTsRawData);
        if (MN_ERR_NO_ERROR != ulRet)
        {
            return ulRet;
        }

        /*����24011��д��������,7.3.1.2 RP DATA (Mobile Station to Network)*/
        stRpData.ucRpUserDataLen = (VOS_UINT8)stTsRawData.ulLen;
        PS_MEM_CPY(stRpData.aucRpUserData, stTsRawData.aucData, stTsRawData.ulLen);
    }

    MN_MSG_EncodeRpData(&stRpData,
                        pstMoEntity->aucRpDataInfo,
                        &pstMoEntity->ucRpDataLen);

    return MN_ERR_NO_ERROR;
}


VOS_VOID MN_MSG_RcvUsimEnvelopeCnf(
        VOS_VOID                            *pMsg
)
{
    PS_USIM_ENVELOPE_CNF_STRU           *pstEnvelope;

    pstEnvelope = (PS_USIM_ENVELOPE_CNF_STRU*) pMsg;

    /* ucDataType STK����͸��EnvelopeType��Ϣ��ҵ����ģ�� */
    if (SI_STK_ENVELOPE_PPDOWN == pstEnvelope->ucDataType)
    {
        MN_MSG_RcvUsimEnvelopRsp(pstEnvelope);
        return;
    }

    if (SI_STK_ENVELOPE_SMSCRTL == pstEnvelope->ucDataType)
    {
        MN_MSG_RcvUsimMoControlRsp(pstEnvelope);
        return;
    }


    return;
}

#if (FEATURE_ON == FEATURE_IMS)
VOS_VOID TAF_MSG_ProcEfsmsFile(
    MN_MSG_MO_ENTITY_STRU              *pstMoEntity, 
    MN_MSG_SUBMIT_RPT_EVT_INFO_STRU    *pstSubmitRptEvt
)
{
    VOS_UINT32                          ulRet;
    MN_MSG_MEM_STORE_ENUM_U8            enCurMemStore;
#if (NAS_FEATURE_SMS_NVIM_SMSEXIST == FEATURE_ON)
    MN_OPERATION_ID_T                   bcOpId;
#endif
    enCurMemStore = MN_MSG_MEM_STORE_NONE;

#if (NAS_FEATURE_SMS_NVIM_SMSEXIST == FEATURE_ON)
    if ((MN_MSG_MEM_STORE_SIM == pstMoEntity->enSaveArea)
     || (MN_MSG_MEM_STORE_NV == pstMoEntity->enSaveArea))
#else
    if (MN_MSG_MEM_STORE_SIM == pstMoEntity->enSaveArea)
#endif
    {
        ulRet = VOS_FALSE;
        
        ulRet = MSG_UpdateEfSmsInfo(pstMoEntity,&enCurMemStore);
        MN_INFO_LOG1("TAF_MSG_UpdateEfsmsInfo: result of TAF_MSG_UpdateEfsmsInfo is ",
                     (VOS_INT32)ulRet);
                     
        pstSubmitRptEvt->enSaveArea   = enCurMemStore;
        pstSubmitRptEvt->ulSaveIndex = pstMoEntity->ulSaveIndex;
        
#if (NAS_FEATURE_SMS_NVIM_SMSEXIST == FEATURE_ON)
        if ((MN_MSG_MEM_STORE_NV == enCurMemStore)
         && (VOS_OK == ulRet))
        {
            bcOpId = MN_MSG_GetBCopId();
            MN_MSG_ReportSmChanged(pstMoEntity->clientId,bcOpId,VOS_TRUE,MN_MSG_MEM_STORE_NV);
        }
#endif
    }
}


VOS_VOID TAF_MSG_RcvImsaRpRpt(
    SMR_SMT_MO_REPORT_STRU *pstMsg
)
{
    MN_MSG_MO_ENTITY_STRU               stMoEntity;
    MN_MSG_RP_CAUSE_STRU                stRpCause;
    MN_MSG_RP_ERR_STRU                  stRpErr;
    MN_MSG_RP_ACK_STRU                  stRpAck;
    MN_MSG_SUBMIT_RPT_EVT_INFO_STRU     stSubmitRptEvt;

    PS_MEM_SET(&f_stMsgTsData,0X00,sizeof(f_stMsgTsData));
    PS_MEM_SET(&stMoEntity,0X00,sizeof(stMoEntity));
    PS_MEM_SET(&stRpCause,0X00,sizeof(stRpCause));
    PS_MEM_SET(&stRpErr,0X00,sizeof(stRpErr));
    PS_MEM_SET(&stRpAck,0X00,sizeof(stRpAck));
    PS_MEM_SET(&stSubmitRptEvt,0X00,sizeof(stSubmitRptEvt));    

    MN_MSG_GetMoEntity(&stMoEntity);
    if (MN_MSG_MO_STATE_WAIT_REPORT_IND != stMoEntity.enSmaMoState)
    {
        MN_WARN_LOG("MSG_RcvSmsRpRpt:Mo State is NULL");
        return;
    }
    
    if (SMR_SMT_ERROR_NO_ERROR == pstMsg->enErrorCode)
    {
        MSG_DecodeRpAck(pstMsg->stRpduData.aucData, (VOS_UINT8)pstMsg->stRpduData.ulDataLen, &stRpAck);
        if (VOS_TRUE == stRpAck.bRpUserDataExist)
        {
            stSubmitRptEvt.stRawData.ulLen = stRpAck.ucRpUserDataLen;
            PS_MEM_CPY(&(stSubmitRptEvt.stRawData.aucData[0]), 
                       stRpAck.aucRpUserData, 
                       stRpAck.ucRpUserDataLen);
        }
        stSubmitRptEvt.stRawData.enTpduType = MN_MSG_TPDU_SUBMIT_RPT_ACK;

        if ((MN_MSG_MO_TYPE_MO == stMoEntity.enSmsMoType)
         || (MN_MSG_MO_TYPE_BUFFER_STK == stMoEntity.enSmsMoType))
        {
            TAF_MSG_ProcEfsmsFile(&stMoEntity, &stSubmitRptEvt);
        }
        else
        {
            if (VOS_TRUE == stMoEntity.bReportFlag)
            {
                MN_MSG_UpdateAppMemStatus(MN_MSG_MEM_FULL_UNSET);
            }
            
            MN_MSG_UpdateMemExceedFlag(MN_MSG_MEM_FULL_UNSET);
        }
    }
    else
    {        
        MSG_DecodeRpErr(pstMsg->stRpduData.aucData, (VOS_UINT8)pstMsg->stRpduData.ulDataLen, &stRpErr);
        PS_MEM_CPY(&stRpCause,&stRpErr.stRpCause,sizeof(stRpCause));
        if (VOS_TRUE == stRpErr.bRpUserDataExist)
        {
            stSubmitRptEvt.stRawData.ulLen = stRpErr.ucRpUserDataLen;
            PS_MEM_CPY(&(stSubmitRptEvt.stRawData.aucData[0]),
                       stRpErr.aucRpUserData,
                       stSubmitRptEvt.stRawData.ulLen);
        }
        stSubmitRptEvt.stRawData.enTpduType = MN_MSG_TPDU_SUBMIT_RPT_ERR;

        MN_MNTN_RecordSmsMoFailure(MSG_GetMoFailCause(&(stSubmitRptEvt.stRawData), pstMsg->enErrorCode));
    }
    
    stSubmitRptEvt.ucMr        = stMoEntity.ucMr;
    stSubmitRptEvt.ulSaveIndex = stMoEntity.ulSaveIndex;
    stSubmitRptEvt.enSaveArea  = stMoEntity.enSaveArea;
    stSubmitRptEvt.enErrorCode = MSG_GetMoFailCause(&(stSubmitRptEvt.stRawData), pstMsg->enErrorCode);

    /* ֪ͨSPM���ŷ��ͽ�� */
    TAF_MSG_SpmMsgReportInd(&stSubmitRptEvt, &stMoEntity, TAF_MSG_SIGNALLING_TYPE_CS_OVER_IP);

    /* destory MO entity*/
    MN_MSG_DestroyMoInfo();
    
    return;
}


VOS_VOID TAF_MSG_ProcImsaMsg(VOS_VOID *pMsg)
{
    MSG_HEADER_STRU                   *pstSmsMsg = VOS_NULL_PTR;

    pstSmsMsg = (MSG_HEADER_STRU *)pMsg;

    switch (pstSmsMsg->ulMsgName)
    {
        case ID_IMSA_MSG_DATA_IND:
            MSG_RcvSmsRpData((SMR_SMT_EST_IND_STRU *)pMsg);
            break;

        case ID_IMSA_MSG_REPORT_IND:
            TAF_MSG_RcvImsaRpRpt((SMR_SMT_MO_REPORT_STRU *)pMsg);
            break;

        case ID_IMSA_MSG_MT_ERR_IND:
            MSG_RcvSmsMtErrInd((SMR_SMT_MT_ERR_STRU*)pMsg);
            break;

        default:
            MN_WARN_LOG("TAF_MSG_ProcImsaMsg:Error MsgName");
            break;
    }
}
#endif

#ifdef  __cplusplus
  #if  __cplusplus
  }
  #endif
#endif

