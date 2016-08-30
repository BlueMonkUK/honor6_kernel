

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include  "TafCbaProcUsim.h"
#include  "TafCbaMntn.h"
#include  "TafCbaComFunc.h"
#include  "TafCbaProcNvim.h"

#include  "UsimPsInterface.h"
#include  "MnMsgSmCommProc.h"
#include  "MnComm.h"
#include  "MnMsgTimerProc.h"
#include  "TafCbsInterface.h"
#include  "TafLog.h"

#include "NasUsimmApi.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_TAF_CBA_PROC_USIM_C

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


/* Deleted TAF_CBA_ProcUsimGetFileRsp */
/* Deleted TAF_CBA_SndUsimGetFileReq */
/* Deleted TAF_CBA_RcvUsimGetEfCbmiRsp TAF_CBA_RcvUsimGetEfCbmirRsp TAF_CBA_RcvUsimGetEfCbmidRsp */

VOS_VOID TAF_CBA_ParseUsimFileEfCbmiData(
    VOS_UINT8                           ucEfCbmiExistFlg,
    VOS_UINT8                          *pucEf,
    VOS_UINT16                          usEfLen
)
{
    TAF_CBA_USIM_EFCBMI_FILE_INFO_STRU *pstEfCbmiFileInfo;
    VOS_UINT32                          ulLoop;
    TAF_CBA_CBMI_RANGE_LIST_STRU       *pstCbmRangeids;
    VOS_UINT8                           ucMsgIdHigh;
    VOS_UINT8                           ucMsgIdLow;
    VOS_UINT16                          usMsgId;
    VOS_UINT8                          *pucCbmiOct;

    pstEfCbmiFileInfo = TAF_CBA_GetEfCbmiInfo();

    pstEfCbmiFileInfo->ucEfCbmiExistFlg = ucEfCbmiExistFlg;

    /* �ļ������ڣ�������ȫ�ֱ����Ĵ��ڱ�־ */
    if (VOS_FALSE == ucEfCbmiExistFlg)
    {
        return;
    }

    /* �ļ����ڣ������ļ����� */
    pstEfCbmiFileInfo->usEfCbmiFileLen  = usEfLen;

    /* ��USIM��CBMI�ļ���¼��ID������д�ڴ��� */
    pstCbmRangeids = (TAF_CBA_CBMI_RANGE_LIST_STRU *)PS_MEM_ALLOC(WUEPS_PID_TAF, sizeof(TAF_CBA_CBMI_RANGE_LIST_STRU));
    if (VOS_NULL_PTR == pstCbmRangeids)
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_CBA_ParseUsimFileEfCbmiData: Fail to Alloc memory.");
        return;
    }

    PS_MEM_SET(pstCbmRangeids, 0x00, sizeof(TAF_CBA_CBMI_RANGE_LIST_STRU));

    /* ����USIM EFCBMI��CBMI�ĸ��� */
    pucCbmiOct                  = pucEf;
    pstCbmRangeids->usCbmirNum  = TAF_CBA_CountCbmiNum((VOS_UINT16 *)pucEf, usEfLen);

    /* ���ݸ������ */
    if ( pstCbmRangeids->usCbmirNum >= TAF_CBA_MAX_CBMID_RANGE_NUM )
    {
        pstCbmRangeids->usCbmirNum = TAF_CBA_MAX_CBMID_RANGE_NUM;
    }

    for (ulLoop = 0; ulLoop < pstCbmRangeids->usCbmirNum; ulLoop++)
    {
        ucMsgIdHigh = *(pucCbmiOct);
        pucCbmiOct++;

        ucMsgIdLow  = *(pucCbmiOct);
        pucCbmiOct++;

        usMsgId     = (VOS_UINT16)( (ucMsgIdHigh << 8) | (ucMsgIdLow));

        pstCbmRangeids->astCbmiRangeInfo[ulLoop].usMsgIdFrom = usMsgId;
        pstCbmRangeids->astCbmiRangeInfo[ulLoop].usMsgIdTo   = usMsgId;
        pstCbmRangeids->astCbmiRangeInfo[ulLoop].enRcvMode   = TAF_CBA_CBMI_RANGE_RCV_MODE_ACCEPT;
    }

    /* �����ڴ��б� */
    TAF_CBA_AddCbMiRangeListToActiveList(pstCbmRangeids);

    PS_MEM_FREE(WUEPS_PID_TAF, pstCbmRangeids);

}


VOS_VOID TAF_CBA_ParseUsimFileEfCbmirData(
    VOS_UINT8                           ucEfCbmirExistFlg,
    VOS_UINT8                          *pucEf,
    VOS_UINT16                          usEfLen
)
{
    TAF_CBA_USIM_EFCBMIR_FILE_INFO_STRU                    *pstEfCbmirFileInfo;
    VOS_UINT32                                              ulLoop;
    TAF_CBA_CBMI_RANGE_LIST_STRU                           *pstCbmirList;
    VOS_UINT8                                               ucMsgIdHigh;
    VOS_UINT8                                               ucMsgIdLow;
    VOS_UINT16                                              usMsgId;
    VOS_UINT8                                              *pucCbmirOct;

    pstEfCbmirFileInfo = TAF_CBA_GetEfCbmiRangeInfo();

    pstEfCbmirFileInfo->ucEfCbmirExistFlg = ucEfCbmirExistFlg;

    if (VOS_FALSE == ucEfCbmirExistFlg)
    {
        return;
    }

    pstEfCbmirFileInfo->usEfCbmirFileLen  = usEfLen;

    /* ��USIM��CBMIR�ļ���¼��ID������д��ʱ�ڴ��� */
    pstCbmirList = (TAF_CBA_CBMI_RANGE_LIST_STRU *)PS_MEM_ALLOC(WUEPS_PID_TAF, sizeof(TAF_CBA_CBMI_RANGE_LIST_STRU));
    if (VOS_NULL_PTR == pstCbmirList)
    {
        TAF_WARNING_LOG(WUEPS_PID_TAF, "TAF_CBA_ParseUsimFileEfCbmirData: Fail to Alloc memory.");
        return;
    }

    /* ����USIM EFCBMIR��CBMIR�ĸ��� */
    pstCbmirList->usCbmirNum = TAF_CBA_CountCbmirNum((VOS_UINT16 *)pucEf, usEfLen);

    pucCbmirOct = pucEf;

    /* ���ݸ������ */
    if ( pstCbmirList->usCbmirNum >= TAF_CBA_MAX_CBMID_RANGE_NUM )
    {
        pstCbmirList->usCbmirNum = TAF_CBA_MAX_CBMID_RANGE_NUM;
    }

    for (ulLoop = 0; ulLoop < pstCbmirList->usCbmirNum; ulLoop++)
    {
        ucMsgIdHigh = *(pucCbmirOct);
        pucCbmirOct++;

        ucMsgIdLow  = *(pucCbmirOct);
        pucCbmirOct++;

        usMsgId     = (VOS_UINT16)( (ucMsgIdHigh << 8) | (ucMsgIdLow));
        pstCbmirList->astCbmiRangeInfo[ulLoop].usMsgIdFrom = usMsgId;

        ucMsgIdHigh = *(pucCbmirOct);
        pucCbmirOct++;

        ucMsgIdLow  = *(pucCbmirOct);
        pucCbmirOct++;

        usMsgId     = (VOS_UINT16)( (ucMsgIdHigh << 8) | (ucMsgIdLow));
        pstCbmirList->astCbmiRangeInfo[ulLoop].usMsgIdTo = usMsgId;

        pstCbmirList->astCbmiRangeInfo[ulLoop].enRcvMode = TAF_CBA_CBMI_RANGE_RCV_MODE_ACCEPT;
    }

    /* �ȶ�range list���� */
    TAF_CBA_SortCbmiRangeList(pstCbmirList);

    /* �����ڴ���CBMIR�ķ�Χ */
    TAF_CBA_AddCbMiRangeListToActiveList(pstCbmirList);

    PS_MEM_FREE(WUEPS_PID_TAF, pstCbmirList);
}
VOS_VOID TAF_CBA_ApplyUsimInfo(VOS_VOID)
{
    VOS_UINT32                          ulIsActiveListEmpty;

    /* CBS�����б��Ƿ�Ϊ�� */
    ulIsActiveListEmpty = TAF_CBA_IsCbsActiveListEmpty();

    /* CBS�����б�Ϊ�ղ�����Ҫ��AS����CBS����ʱ��AS����CBS���� */
    if ((VOS_TRUE  == TAF_CBA_IsNeedActAsCbs())
     && (VOS_FALSE == ulIsActiveListEmpty))
    {
        TAF_CBA_SndAsActiveCbsReq(TAF_CBS_CFG_ACTIVE);
    }

    /* ����NV��USIM���ļ�, ʹUSIM���к�UE��NV�е�CBMI��Ϣ����һ�� */
    if ( TAF_CBA_GetCbmiRangNum() > 0)
    {
        /* ����NV���е�MSGID �б� */
        TAF_CBA_WriteCbmiRangeListNvim();

        /* ����USIM�е�MSGID �ļ� */
        TAF_CBA_UpdateUsimFile();
    }

    /* ��ά�ɲ���Ϣ */
    TAF_CBA_SndOmCbmiDownloadListInfo();

    /* ��ά�ɲ���Ϣ */
    TAF_CBA_SndOmCbmiRangeList();

    return;

}
VOS_VOID TAF_CBA_ParseSpecUsimFileData (
    VOS_UINT16                          usFileId,
    VOS_UINT8                           ucFileExistFlg,
    VOS_UINT8                          *pucEf,
    VOS_UINT16                          ulEfLen
)
{
    switch (usFileId)
    {
        case TAF_CBA_EF_CBMI_ID:
             TAF_CBA_ParseUsimFileEfCbmiData(ucFileExistFlg, pucEf, ulEfLen);
             break;

        case TAF_CBA_EF_CBMIR_ID:
             TAF_CBA_ParseUsimFileEfCbmirData(ucFileExistFlg, pucEf, ulEfLen);
             break;

        case TAF_CBA_EF_CBMID_ID:
             TAF_CBA_ParseUsimFileEfCbmidData(ucFileExistFlg, pucEf, ulEfLen);
             break;
             
        default:
            TAF_WARNING_LOG(WUEPS_PID_TAF, "TAF_CBA_ParseSpecUsimFileData: The input is invalid.");
            break;
    }
}


VOS_VOID TAF_CBA_ReadSpecificFileListRefreshFile(
    VOS_UINT16                          usFileId,
    NAS_USIMM_SERVICES_TYPE_ENUM_UINT32 ulUsimmService,
    PS_USIM_REFRESH_IND_STRU           *pstRefreshInd
)
{
    VOS_UINT32                          ulEfLen;
    VOS_UINT32                          ulResult;
    USIMM_APP_TYPE_ENUM_UINT32          enAppType;      /*APP����*/
    VOS_UINT8                           ucFileExistFlg;
    VOS_UINT8                          *pucEf   = VOS_NULL_PTR;

    if (PS_USIM_SERVICE_NOT_AVAILIABLE != NAS_USIMMAPI_IsServiceAvailable(ulUsimmService))
    {
        if (VOS_TRUE == TAF_GetSpecificFileListRefreshFileInfo(usFileId, pstRefreshInd, &enAppType))
        {
            /* ��ȡ TAF_CBA_EF_CBMI_ID USIM�ļ� */
            ucFileExistFlg = VOS_TRUE;
            ulResult = NAS_USIMMAPI_GetCachedFile(usFileId, &ulEfLen, &pucEf, enAppType);
            if (USIMM_API_SUCCESS != ulResult)
            {
                ucFileExistFlg = VOS_FALSE;
            }
            TAF_CBA_ParseSpecUsimFileData(usFileId, ucFileExistFlg, pucEf, (VOS_UINT16)ulEfLen);
        }
    }

    return;
}


VOS_VOID TAF_CBA_ReadUsimFile(
    VOS_UINT16                          usFileId,
    NAS_USIMM_SERVICES_TYPE_ENUM_UINT32 ulUsimmService
)
{
    VOS_UINT32                          ulEfLen;
    VOS_UINT32                          ulResult;
    VOS_UINT8                           ucFileExistFlg;
    VOS_UINT8                          *pucEf   = VOS_NULL_PTR;

    if (PS_USIM_SERVICE_NOT_AVAILIABLE != NAS_USIMMAPI_IsServiceAvailable(ulUsimmService))
    {
        ucFileExistFlg = VOS_TRUE;
        ulResult = NAS_USIMMAPI_GetCachedFile(usFileId, &ulEfLen, &pucEf, USIMM_UNLIMIT_APP);
        if (USIMM_API_SUCCESS != ulResult)
        {
            ucFileExistFlg = VOS_FALSE;
        }

        TAF_CBA_ParseSpecUsimFileData(usFileId, ucFileExistFlg, pucEf, (VOS_UINT16)ulEfLen);

    }

    return;
}


VOS_VOID TAF_CBA_ProcRefreshInd(PS_USIM_REFRESH_IND_STRU *pstRefreshInd)
{
    if ((USIMM_REFRESH_FILE_LIST != pstRefreshInd->enRefreshType)
     && (USIMM_REFRESH_3G_SESSION_RESET != pstRefreshInd->enRefreshType)
     && (USIMM_REFRESH_ALL_FILE != pstRefreshInd->enRefreshType))
    {
        return;
    }

    if (USIMM_REFRESH_FILE_LIST == pstRefreshInd->enRefreshType)
    {
        TAF_CBA_ReadSpecificFileListRefreshFile(TAF_CBA_EF_CBMI_ID, NAS_USIM_SVR_CBMI, pstRefreshInd);

        TAF_CBA_ReadSpecificFileListRefreshFile(TAF_CBA_EF_CBMIR_ID, NAS_USIM_SVR_CBMIR, pstRefreshInd);

        TAF_CBA_ReadSpecificFileListRefreshFile(TAF_CBA_EF_CBMID_ID, NAS_USIM_SVR_DATA_DL_SMSCB, pstRefreshInd);
    }
    else
    {
        TAF_CBA_ReadUsimFile(TAF_CBA_EF_CBMI_ID, NAS_USIM_SVR_CBMI);

        TAF_CBA_ReadUsimFile(TAF_CBA_EF_CBMIR_ID, NAS_USIM_SVR_CBMIR);

        TAF_CBA_ReadUsimFile(TAF_CBA_EF_CBMID_ID, NAS_USIM_SVR_DATA_DL_SMSCB);

    }

    /* Ӧ��USIM�ļ���Ϣ�����غ�֪ͨ�ײ����ø��� */
    TAF_CBA_ApplyUsimInfo();

    return;
}
VOS_VOID TAF_CBA_ParseUsimFileEfCbmidData(
    VOS_UINT8                           ucEfCbmidExistFlg,
    VOS_UINT8                          *pucEf,
    VOS_UINT16                          usEfLen
)
{
    TAF_CBA_USIM_EFCBMID_FILE_INFO_STRU                    *pstEfCbmidFileInfo;
    TAF_CBA_CBMI_DOWNLOAD_LIST_STRU                        *pstCfgData;
    VOS_UINT32                                              ulLoop;
    VOS_UINT8                                               ucMsgIdHigh;
    VOS_UINT8                                               ucMsgIdLow;
    VOS_UINT16                                              usMsgId;
    VOS_UINT8                                              *pucCbmiOct;

    pstEfCbmidFileInfo  = TAF_CBA_GetEfCbmidInfo();
    pstEfCbmidFileInfo->ucEfCbmidExistFlg = ucEfCbmidExistFlg;
    if (VOS_FALSE == ucEfCbmidExistFlg)
    {
        return;
    }

    pstCfgData          = TAF_CBA_GetCbmiDownloadList();

    pstEfCbmidFileInfo->usEfCbmidFileLen  = usEfLen;

    /* ����USIM EFCBMID��CBMI�ĸ��� */
    pucCbmiOct              = pucEf;
    pstCfgData->usCbmidNum  = TAF_CBA_CountCbmiNum((VOS_UINT16 *)pucEf, usEfLen);

    /* ���ݸ������ */
    if ( pstCfgData->usCbmidNum >= TAF_CBA_MAX_CBMID_NUM )
    {
        pstCfgData->usCbmidNum = TAF_CBA_MAX_CBMID_NUM;
    }

    for (ulLoop = 0; ulLoop < pstCfgData->usCbmidNum; ulLoop++)
    {
        ucMsgIdHigh = *(pucCbmiOct);
        pucCbmiOct++;

        ucMsgIdLow  = *(pucCbmiOct);
        pucCbmiOct++;

        usMsgId     = (VOS_UINT16)( (ucMsgIdHigh << 8) | (ucMsgIdLow));
        pstCfgData->ausMsgId[ulLoop] = usMsgId;
    }
}

/*****************************************************************************
 �� �� ��  : TAF_CBA_CountCbmiNum
 ��������  : ����USIM���д洢��CBMI��CBMID ����
 �������  : pusCbmi : ����������ʼָ�룻
             ulLength: ���������ȣ�
 �������  : ��
 �� �� ֵ  : CB Message Identifier�ĸ���; ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2006��12��20��
    ��    ��   : ��ӳ�� 62575
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT16 TAF_CBA_CountCbmiNum(
    VOS_UINT16                         *pusCbmi,
    VOS_UINT32                          ulLength
)
{
    VOS_UINT8                           ucCounter;
    VOS_UINT32                          ulRealLen;
    VOS_UINT8                           ucOffset;

    /* ��������ʵ�ʵĳ���  TAF_CBA_CBMI_UNIT_LEN * N */
    ulRealLen = ulLength;
    ucOffset  = ulLength%TAF_CBA_CBMI_UNIT_LEN;
    if (0 != ucOffset)
    {
        ulRealLen -= ucOffset;
    }

    /* �μ� 3GPP 31.102 4.2.14, 4.2.20 �ڵ����� , Unused entries shall be set to 'FF FF'. */
    ucCounter = (VOS_UINT8)(ulRealLen/TAF_CBA_CBMI_UNIT_LEN);
    while (ucCounter >= 1)
    {
        if (TAF_CBA_EF_MSGID_PADDING != pusCbmi[ucCounter - 1])
        {
            break;
        }
        ucCounter--;
    }
    return ucCounter;
}


VOS_UINT16 TAF_CBA_CountCbmirNum(
    VOS_UINT16                         *pusCbmir,
    VOS_UINT32                          ulLength
)
{
    VOS_UINT8                           ucCounter;
    VOS_UINT16                          usTemp1;
    VOS_UINT16                          usTemp2;
    VOS_UINT32                          ulRealLen;
    VOS_UINT8                           ucOffset;

    /* ��������ʵ�ʵĳ���  TAF_CBA_CBMIR_UNIT_LEN*N */
    ulRealLen = ulLength;
    ucOffset  = ulLength%TAF_CBA_CBMIR_UNIT_LEN;
    if ( 0 != ucOffset  )
    {
        ulRealLen -= ucOffset;
    }

    /* �μ� 3GPP 31.102 4.2.22�ڵ����� , Unused entries shall be set to 'FF FF FF FF'. */
    ucCounter = (VOS_UINT8)(ulRealLen/TAF_CBA_CBMI_UNIT_LEN);
    while (2 <= ucCounter)
    {
        usTemp1 = pusCbmir[ucCounter - 1];
        usTemp2 = pusCbmir[ucCounter - 2];
        if ((TAF_CBA_EF_MSGID_PADDING != usTemp1)
         || (TAF_CBA_EF_MSGID_PADDING != usTemp2))
        {
            break;
        }
        ucCounter -= 2;
    }

    ucCounter /=2;

    return ucCounter;
}


VOS_VOID TAF_CBA_BuildUsimEfCbmiContent(
    VOS_UINT16                         *pusMsgId
)
{
    TAF_CBA_CBMI_RANGE_LIST_STRU       *pstCbmidRangeList;
    TAF_CBA_CBMI_RANGE_STRU            *pstCbmidRange;
    TAF_CBA_USIM_EFCBMI_FILE_INFO_STRU *pstEfCbmiFileInfo;
    VOS_UINT32                          ulEfCbmiLen;
    VOS_UINT8                           ucMsgIdHigh;
    VOS_UINT8                           ucMsgIdLow;
    VOS_UINT32                          ulCbmirRangeIdx;
    VOS_UINT16                          usCbmiNum;

    ulEfCbmiLen       = 0;
    pstCbmidRangeList = TAF_CBA_GetCbMiRangeList();
    usCbmiNum         = TAF_CBA_GetCbmiRangNum();

    if(usCbmiNum > TAF_CBA_MAX_CBMID_RANGE_NUM)
    {
        usCbmiNum = TAF_CBA_MAX_CBMID_RANGE_NUM;
    }

    pstEfCbmiFileInfo = TAF_CBA_GetEfCbmiInfo();

    for ( ulCbmirRangeIdx = 0; ulCbmirRangeIdx < usCbmiNum; ulCbmirRangeIdx++ )
    {
        pstCbmidRange = &(pstCbmidRangeList->astCbmiRangeInfo[ulCbmirRangeIdx]);

        if ( (TAF_CBA_CBMI_RANGE_RCV_MODE_ACCEPT == pstCbmidRange->enRcvMode)
          && (pstCbmidRange->usMsgIdFrom == pstCbmidRange->usMsgIdTo)
          && (ulEfCbmiLen < (pstEfCbmiFileInfo->usEfCbmiFileLen/TAF_CBA_CBMI_UNIT_LEN)) )
        {
            ucMsgIdHigh = (pstCbmidRange->usMsgIdFrom)>>8;
            ucMsgIdLow  = (pstCbmidRange->usMsgIdFrom) & 0xFF;
            pusMsgId[ulEfCbmiLen] = ((ucMsgIdLow << 8) | ucMsgIdHigh);
            ulEfCbmiLen++;
        }

        if ( ulEfCbmiLen >= TAF_CBA_MAX_CBMID_RANGE_NUM )
        {
            break;
        }
    }

}


VOS_VOID TAF_CBA_BuildUsimEfCbmirContent(
    TAF_CBA_USIM_CBMIR_STRU                                *pstMsgRange
)
{
    TAF_CBA_CBMI_RANGE_LIST_STRU                           *pstCbmidRangeList;
    TAF_CBA_CBMI_RANGE_STRU                                *pstCbmidRange;
    TAF_CBA_USIM_EFCBMIR_FILE_INFO_STRU                    *pstEfCbmirFileInfo;
    VOS_UINT32                                              ulEfCbmirlLen;
    VOS_UINT8                                               ucMsgIdHigh;
    VOS_UINT8                                               ucMsgIdLow;
    VOS_UINT32                                              ulCbmidRangeIdx;
    VOS_UINT16                                              usCbmiNum;


    ulEfCbmirlLen      = 0;
    pstCbmidRangeList  = TAF_CBA_GetCbMiRangeList();
    usCbmiNum          = TAF_CBA_GetCbmiRangNum();

    if (usCbmiNum > TAF_CBA_MAX_CBMID_RANGE_NUM)
    {
        usCbmiNum = TAF_CBA_MAX_CBMID_RANGE_NUM;
    }

    pstEfCbmirFileInfo = TAF_CBA_GetEfCbmiRangeInfo();

    for ( ulCbmidRangeIdx = 0; ulCbmidRangeIdx < usCbmiNum; ulCbmidRangeIdx++ )
    {
        pstCbmidRange = &(pstCbmidRangeList->astCbmiRangeInfo[ulCbmidRangeIdx]);

        if ( (TAF_CBA_CBMI_RANGE_RCV_MODE_ACCEPT == pstCbmidRange->enRcvMode)
          && (pstCbmidRange->usMsgIdFrom != pstCbmidRange->usMsgIdTo)
          && (ulEfCbmirlLen < (pstEfCbmirFileInfo->usEfCbmirFileLen/TAF_CBA_CBMIR_UNIT_LEN)) )
        {
            ucMsgIdHigh = (pstCbmidRange->usMsgIdFrom)>>8;
            ucMsgIdLow  = (pstCbmidRange->usMsgIdFrom) & 0xFF;
            pstMsgRange[ulEfCbmirlLen].usMsgIdFrom = ((ucMsgIdLow << 8) | ucMsgIdHigh);

            ucMsgIdHigh = (pstCbmidRange->usMsgIdTo)>>8;
            ucMsgIdLow  = (pstCbmidRange->usMsgIdTo) & 0xFF;
            pstMsgRange[ulEfCbmirlLen].usMsgIdTo = ((ucMsgIdLow << 8) | ucMsgIdHigh);
            ulEfCbmirlLen++;
        }

        if ( ulEfCbmirlLen >= TAF_CBA_MAX_CBMID_RANGE_NUM )
        {
            break;
        }
    }

}


VOS_VOID TAF_CBA_UpdateUsimFileEfCbmiReq(VOS_VOID)
{
    TAF_CBA_USIM_EFCBMI_FILE_INFO_STRU *pstEfCbmiFileInfo;
    VOS_UINT32                          ulSndCbmiFileLen;
    VOS_UINT16                          ausMsgId[TAF_CBA_MAX_CBMID_RANGE_NUM];
    VOS_UINT32                          ulRet;
    USIMM_SET_FILE_INFO_STRU            stSetFileInfo;

    /* CBMI�ļ��������򷵻� */
    pstEfCbmiFileInfo = TAF_CBA_GetEfCbmiInfo();
    if ( VOS_TRUE != pstEfCbmiFileInfo->ucEfCbmiExistFlg )
    {
        TAF_INFO_LOG(WUEPS_PID_TAF, "TAF_CBA_UpdateUsimFileEfCbmiReq: File not exist in sim!");
        return;
    }

    PS_MEM_SET(ausMsgId, (VOS_CHAR)0xFF, sizeof(ausMsgId));

    /* ����ȫ�ֱ����е�MSG ID����EF�ļ����� */
    TAF_CBA_BuildUsimEfCbmiContent(ausMsgId);

    /* ���͸�USIM���ļ��������Զ̵�Ϊ��, */
    ulSndCbmiFileLen = pstEfCbmiFileInfo->usEfCbmiFileLen;
    if ( ulSndCbmiFileLen > sizeof(ausMsgId) )
    {
        ulSndCbmiFileLen = sizeof(ausMsgId);
    }

    /* ���͸�USIM */
    TAF_COMM_BUILD_USIM_SET_FILE_INFO(&stSetFileInfo,
                                    USIMM_UNLIMIT_APP,
                                    TAF_CBA_EF_CBMI_ID,
                                    0,
                                    (VOS_UINT8)ulSndCbmiFileLen,
                                    (VOS_UINT8 *)ausMsgId);

    ulRet = NAS_USIMMAPI_SetFileReq(WUEPS_PID_TAF, 0, &stSetFileInfo);

    if (VOS_OK != ulRet)
    {
        TAF_WARNING_LOG(WUEPS_PID_TAF, "TAF_CBA_UpdateUsimFileEfCbmiReq: Write USIM failed!");
    }
}


VOS_VOID TAF_CBA_UpdateUsimFileEfCbmirReq(VOS_VOID)
{
    TAF_CBA_USIM_EFCBMIR_FILE_INFO_STRU                    *pstEfCbmirFileInfo;
    VOS_UINT32                                              ulSndCbmirFileLen;
    TAF_CBA_USIM_CBMIR_STRU                                 astMsgRange[TAF_CBA_MAX_CBMID_RANGE_NUM];
    VOS_UINT32                                              ulRet;
    USIMM_SET_FILE_INFO_STRU                                stSetFileInfo;

    /* CBMIR�ļ��������򷵻� */
    pstEfCbmirFileInfo = TAF_CBA_GetEfCbmiRangeInfo();
    if ( VOS_TRUE != pstEfCbmirFileInfo->ucEfCbmirExistFlg )
    {
        TAF_INFO_LOG(WUEPS_PID_TAF, "TAF_CBA_UpdateUsimFileEfCbmirReq: File not exist in sim!");
        return;
    }

    PS_MEM_SET(astMsgRange, (VOS_CHAR)0xFF, sizeof(astMsgRange));

    /* ����ȫ�ֱ����е�MSG ID����EF�ļ����� */
    TAF_CBA_BuildUsimEfCbmirContent(astMsgRange);

    /* ���͸�USIM���ļ��������Զ̵�Ϊ��, */
    ulSndCbmirFileLen = pstEfCbmirFileInfo->usEfCbmirFileLen;
    if ( ulSndCbmirFileLen > sizeof(astMsgRange) )
    {
        ulSndCbmirFileLen = sizeof(astMsgRange);
    }

    /* ���͸�USIM */
    TAF_COMM_BUILD_USIM_SET_FILE_INFO(&stSetFileInfo,
                                    USIMM_UNLIMIT_APP,
                                    TAF_CBA_EF_CBMIR_ID,
                                    0,
                                    (VOS_UINT8)ulSndCbmirFileLen,
                                    (VOS_UINT8 *)astMsgRange);

    ulRet = NAS_USIMMAPI_SetFileReq(WUEPS_PID_TAF, 0, &stSetFileInfo);

    if (VOS_OK != ulRet)
    {
        TAF_WARNING_LOG(WUEPS_PID_TAF, "TAF_CBA_UpdateUsimFileEfCbmirReq: Write USIM failed!");
    }
}


VOS_VOID  TAF_CBA_UpdateUsimFile(VOS_VOID)
{
    /* ���ڻ�ȡUSIM�ļ��Ĺ�����,����д�ļ� */

    /* ���� CBMI �ļ� */
    TAF_CBA_UpdateUsimFileEfCbmiReq();

    /* ���� CBMIR �ļ� */
    TAF_CBA_UpdateUsimFileEfCbmirReq();
}
VOS_UINT32  TAF_CBA_ProcUsimSetFileRsp(
    PS_USIM_SET_FILE_CNF_STRU           *pstSetFileRsp
)
{
    if ((TAF_CBA_EF_CBMI_ID  != pstSetFileRsp->usEfId)
     && (TAF_CBA_EF_CBMIR_ID != pstSetFileRsp->usEfId)
     && (TAF_CBA_EF_CBMID_ID != pstSetFileRsp->usEfId))
    {
        return VOS_ERR;
    }

    if (VOS_OK != pstSetFileRsp->ulResult)
    {
        TAF_WARNING_LOG1(WUEPS_PID_TAF, "TAF_CBA_ProcUsimSetFileRsp:Write Files Error",
                    (VOS_INT32)(pstSetFileRsp->usEfId));
    }

    return VOS_OK;
}


VOS_VOID TAF_CBA_SndUsimCbMsg(
    VOS_UINT32                          ulSendPID,
    MN_OPERATION_ID_T                   opId,
    MN_MSG_CBRAW_TS_DATA_STRU          *pstCbMsg
)
{
    VOS_UINT32                          ulRet;
    SI_STK_ENVELOPE_STRU                stDataDownLoad;

    PS_MEM_SET(&stDataDownLoad, 0, sizeof(stDataDownLoad));

    /* ��д���͸�USIM�Ĺ̶��ֶ� */
    stDataDownLoad.EnvelopeType    = SI_STK_ENVELOPE_CBDOWN;
    stDataDownLoad.DeviceId.ucSDId = SI_DEVICE_NETWORK;
    stDataDownLoad.DeviceId.ucDDId = SI_DEVICE_UICC;

    stDataDownLoad.uEnvelope.CBDown.OP_CBPage        = 1;

    /* Խ�籣�� */
    if (pstCbMsg->ulLen > SI_STK_DATA_MAX_LEN)
    {
        pstCbMsg->ulLen = SI_STK_DATA_MAX_LEN;
    }

    stDataDownLoad.uEnvelope.CBDown.CBPage.ulLen     = (VOS_UINT8)pstCbMsg->ulLen;

    PS_MEM_CPY(stDataDownLoad.aucData, pstCbMsg->aucData, pstCbMsg->ulLen);

    stDataDownLoad.uEnvelope.CBDown.CBPage.pucCBPage = stDataDownLoad.aucData;

    /* ֪ͨUSIM����Envelope */

    ulRet = NAS_STKAPI_EnvelopeDownload(ulSendPID, MN_CLIENT_ID_BROADCAST,&stDataDownLoad);


    if (VOS_OK != ulRet)
    {
        TAF_WARNING_LOG(WUEPS_PID_TAF, "MN_MSG_EnvelopeCbm: Fail to envelope download Cbm in USIM!");
    }

    return;
}

/* Deleted TAF_CBA_IsUsimFileReadFinished */

#endif /* ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS)) */


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
