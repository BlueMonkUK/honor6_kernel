


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include  "NasERabmAppMsgProc.h"
#include  "NasERabmEsmMsgProc.h"
#include  "NasERabmEmmMsgProc.h"
#include  "NasERabmTest.h"
#include  "LUPZeroCopy.h"
#include  "NasIpInterface.h"
#include  "NasERabmIpFilter.h"
#include  "NasERabmPublic.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASRABMAPPMSGPROC_C
/*lint +e767*/


/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/
/*�������ݰ�ƥ����ƿ���:�����ǰֻ��һ�����ؼ����û������TFT
  g_ulSrcIpMatchSwitch = 1,�����ԴIP��ƥ��
  g_ulSrcIpMatchSwitch = 0,�򲻽���ԴIPƥ��*/
VOS_UINT32                              g_ulSrcIpMatchSwitch = 1;

#if 0
#ifdef PS_ITT_PC_TEST_NAS
    VOS_VOID  LPDCP_DataReq (LPDCP_RABM_DATA_STRU *pstData)
    {
       return ;
    }

    VOS_UINT32 APPITF_SaveDataToRabmBuf( LUP_MEM_ST *pstMem )
    {
        return PS_SUCC;
    }

#endif

#ifdef PS_ITT_PC_RRC_AND_NAS
    VOS_VOID  LPDCP_DataReq (LPDCP_RABM_DATA_STRU *pstData)
    {
        return ;
    }
#endif
#endif


/*****************************************************************************
  3 Function
*****************************************************************************/

VOS_VOID  NAS_ERABM_AppMsgDistr( VOS_VOID *pRcvMsg )
{
    PS_MSG_HEADER_STRU            *pRrcMsg;

    pRrcMsg = (PS_MSG_HEADER_STRU*)pRcvMsg;

    /*��ӡ����ú���*/
    NAS_ERABM_INFO_LOG("NAS_ERABM_AppMsgDistr is entered.");

    if (NAS_ERABM_L_MODE_STATUS_NORMAL != NAS_ERABM_GetLModeStatus())
    {
        NAS_ERABM_WARN_LOG("NAS_ERABM_AppMsgDistr:Rabm is already suspended!");
        return ;
    }

    /*������Ϣ����������Ӧ����Ϣ������*/
    switch (pRrcMsg->ulMsgName)
    {
        /*����յ�����APP_ERABM_DATA_TRANS_REQ��Ϣ������NAS_ERABM_RcvRabmAppDataReq����*/
        case ID_APP_ERABM_DATA_TRANS_REQ:
            NAS_ERABM_RcvRabmAppDataReq((APP_ERABM_DATA_TRANS_REQ_STRU *) pRcvMsg);
            break;
        #if 0
        /*����յ�����APP_ERABM_SERVICE_REQ��Ϣ������NAS_ERABM_RcvRabmAppServiceReq����*/
        case ID_APP_ERABM_SERVICE_REQ:
            NAS_ERABM_RcvRabmAppServiceReq((APP_ERABM_SERVICE_REQ_STRU *) pRcvMsg);
            break;
        #endif

        default:
            NAS_ERABM_WARN_LOG("NAS_ERABM_AppMsgDistr:WARNING:APP->RABM Message name non-existent!");
            break;
    }

}

/*lint -e960*/
/*lint -e961*/
VOS_VOID NAS_ERABM_RcvRabmAppDataReq( const APP_ERABM_DATA_TRANS_REQ_STRU *pRcvMsg )
{
    NAS_ERABM_APP_DATA_TRANS_SET_RESLT_STRU             stAppRabmDataTrans;

    /*��ӡ����ú���*/
    NAS_ERABM_INFO_LOG("NAS_ERABM_RcvRabmAppDataReq is entered.");

    if ( (NAS_ERABM_OP_TRUE == pRcvMsg->stDataTransInfo.bitOpIpv4)
          || (NAS_ERABM_OP_TRUE == pRcvMsg->stDataTransInfo.bitOpIpv6))
    {
        NAS_ERABM_MEM_CPY((NAS_ERABM_GetAppDataInfoAddr()),\
           &(pRcvMsg->stDataTransInfo), sizeof(APP_ERABM_DATA_TRANS_INFO_STRU));

        stAppRabmDataTrans.enSetRslt = APP_ERABM_DATA_TRANS_SET_SUCCESS;
    }
    else
    {
        NAS_ERABM_WARN_LOG("NAS_ERABM_RcvRabmAppDataReq:WARNING:APP->RABM IP Bit error!");

        stAppRabmDataTrans.enSetRslt = APP_ERABM_DATA_TRANS_SET_FAIL;
    }

    stAppRabmDataTrans.ulMsgId = ID_APP_ERABM_DATA_TRANS_CNF;
    stAppRabmDataTrans.usOriginalId = pRcvMsg->usTerminalId;
    stAppRabmDataTrans.usTerminalId = pRcvMsg->usOriginalId;
    stAppRabmDataTrans.ulSn = pRcvMsg->ulSN;

    NAS_ERABM_SndRabmAppDataCnf(&stAppRabmDataTrans);
}
 VOS_VOID  NAS_ERABM_SndRabmAppDataCnf( const NAS_ERABM_APP_DATA_TRANS_SET_RESLT_STRU
                                                      *pstRabmAppDataTrans )
{
    APP_ERABM_DATA_TRANS_CNF_STRU        *pstRabmAppDataCnf = VOS_NULL_PTR;

    /*��ӡ����ú���*/
    NAS_ERABM_INFO_LOG("NAS_ERABM_SndRabmAppDataCnf is entered.");

    /*��ڲ������*/
    if ((APP_ERABM_DATA_TRANS_SET_SUCCESS != pstRabmAppDataTrans->enSetRslt)
         &&(APP_ERABM_DATA_TRANS_SET_FAIL != pstRabmAppDataTrans->enSetRslt))
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_SndRabmAppDataCnf:ERROR: Input para error!");
        return;
    }

    /*����ռ�ͼ���Ƿ����ɹ�*/
    pstRabmAppDataCnf = (VOS_VOID *)NAS_ERABM_ALLOC_MSG(
                                        sizeof(APP_ERABM_DATA_TRANS_CNF_STRU));
    if ( VOS_NULL_PTR == pstRabmAppDataCnf )
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_SndRabmAppDataCnf:ERROR:Alloc msg fail!" );
        return;
    }

    /*���*/
    NAS_ERABM_MEM_SET(NAS_ERABM_GET_MSG_ENTITY(pstRabmAppDataCnf), NAS_ERABM_NULL,\
                     NAS_ERABM_GET_MSG_LENGTH(pstRabmAppDataCnf));

    /*��д��Ϣͷ*/
    NAS_ERABM_WRITE_APP_MSG_HEAD(pstRabmAppDataCnf, pstRabmAppDataTrans->ulMsgId,\
           pstRabmAppDataTrans->usOriginalId,pstRabmAppDataTrans->usTerminalId,\
                              pstRabmAppDataTrans->ulSn);
    /*��д��Ϣʵ��*/
    pstRabmAppDataCnf->enSetOpRslt  = pstRabmAppDataTrans->enSetRslt;

    /* ������Ϣ���ͺ��� */
    NAS_ERABM_SND_MSG(pstRabmAppDataCnf);

}

/*****************************************************************************
 Function Name   : NAS_ERABM_AppAssignMidHeader
 Description     : ����APP��ϢͷAPP_PS_MSG_HEADER�е�MID����
 Input           : VOS_UINT32  *pulOriMid,
                    VOS_UINT32 *pulTerMid
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-4-12  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ERABM_AppAssignMidHeader
(
    VOS_UINT16                         *pusOriMid,
    VOS_UINT16                         *pusTerMid
)
{
    /*��ʼ��MID����ֵ*/
    *pusOriMid = UE_APP_SUBSYS_ID;  /*FFS*/
    *pusTerMid = UE_MODULE_RABM_ID;
}
#if 0
/*****************************************************************************
 Function Name   : NAS_ERABM_SndAppRabmServiceReq
 Description     : ��RABM����ID_APP_ERABM_SERVICE_REQ��Ϣ
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-4-12  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ERABM_SndAppRabmServiceReq( VOS_VOID )
{
    APP_ERABM_SERVICE_REQ_STRU          *pstSerReq = VOS_NULL_PTR;
    VOS_UINT16                          usOriginalId        = 0;
    VOS_UINT16                          usTerminalId        = 0;

    /*����ռ�ͼ���Ƿ����ɹ�*/
    pstSerReq = (VOS_VOID*)NAS_ERABM_ALLOC_MSG(sizeof(APP_ERABM_SERVICE_REQ_STRU));
    if (VOS_NULL_PTR == pstSerReq)
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_SndAppRabmServiceReq:ERROR:Alloc msg fail!" );
        return;
    }

    /*���*/
    NAS_ERABM_MEM_SET(NAS_ERABM_GET_MSG_ENTITY(   pstSerReq),
                                                NAS_ERABM_NULL,
                                                NAS_ERABM_GET_MSG_LENGTH(pstSerReq));

    /*���䱾�ι���APP_MSG_HEADER*/
    NAS_ERABM_AppAssignMidHeader(&usOriginalId, &usTerminalId);

    /*��д��Ϣͷ*/
    NAS_ERABM_APP_WRITE_RABM_MSG_HEAD(   pstSerReq, ID_APP_ERABM_SERVICE_REQ,\
                                        usOriginalId, usTerminalId, 0);

    /* ������Ϣ���ͺ���*/
    NAS_ERABM_SND_MSG(pstSerReq);
}

/*****************************************************************************
 Function Name   : NAS_ERABM_RcvRabmAppServiceReq
 Description     :
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-04-12  Draft Enact
    2.sunbing 49683       2010-08-26  Modify
    3.sunbing 49683       2010-09-16  Modify  ��������ظ�����Service������RABM��Ϣ�����������
*****************************************************************************/
VOS_VOID  NAS_ERABM_RcvRabmAppServiceReq
(
    APP_ERABM_SERVICE_REQ_STRU          *pstServiceReq
)
{
    (VOS_VOID)pstServiceReq;

    /* ����û�ж�Ӧ��DRB�ĳ����£����Ҷ�ʱ��û������ʱ����EMM�������� */
    if (NAS_ERABM_SUCCESS == NAS_ERABM_IsAllActtiveBearerWithoutDrb())
    {
        if(NAS_ERABM_TIMER_STATE_STOPED != NAS_ERABM_IsTimerRunning(NAS_ERABM_WAIT_RB_REEST_TIMER))
        {
            NAS_ERABM_NORM_LOG("NAS_ERABM_RcvRabmAppServiceReq:Reest timer started!" );
            return ;
        }

        /*����EMM_ERABM_REEST_REQ��Ϣ�����÷�����������*/
        NAS_ERABM_SetEmmSrState(NAS_ERABM_SERVICE_STATE_INIT);

        NAS_ERABM_SndRabmEmmReestReq();

        NAS_ERABM_TimerStart(NAS_ERABM_WAIT_RB_REEST_LENGTH, NAS_ERABM_WAIT_RB_REEST_TIMER);

        /*֪ͨAPPֹͣ���ݴ���,����APP�ṩ�ĺ���*/
        /*AppDataStatusFun( ulEpsbId, NAS_ERABM_APP_DATA_STATUS_STOP, 0xFF );*/
    }
    else
    {
        NAS_ERABM_SetEmmSrState(NAS_ERABM_SERVICE_STATE_TERMIN);
        NAS_ERABM_NORM_LOG("NAS_ERABM_RcvRabmAppServiceReq:Already in Connect state or Reest timer started!" );
    }
}
#endif


PS_BOOL_ENUM_UINT8  NAS_ERABM_IsExistUplinkPfinTft( VOS_UINT32 ulEpsbId)
{
    VOS_UINT32                          ulTftPfCnt = NAS_ERABM_NULL;
    ESM_ERABM_TFT_PF_STRU               *pstTftPf   = VOS_NULL_PTR;

    for(ulTftPfCnt = 0; ulTftPfCnt < NAS_ERABM_GetEpsbTftPfNum(ulEpsbId); ulTftPfCnt++)
    {
        pstTftPf = NAS_ERABM_GetEpsbTftAddr(ulEpsbId, ulTftPfCnt);

        if ((APP_ESM_TRANSFER_DIRECTION_UPLINK == pstTftPf->enDirection)
         || (APP_ESM_TRANSFER_DIRECTION_UPLINK_DOWNLINK == pstTftPf->enDirection))
        {
            return PS_TRUE;
        }
    }

    return PS_FALSE;
}


VOS_VOID NAS_ERABM_SaveIpv4SegDataInfo
(
    const NAS_ERABM_IPV4_HEADER_STRU   *pstIpv4HeaderInfo,
    VOS_UINT8                           ucBearerId
)
{
    static VOS_UINT16                   usIndex         = NAS_ERABM_NULL;
    VOS_UINT32                          ulLoop          = NAS_ERABM_NULL;
    NAS_ERABM_IPV4_SEG_BUFF_ITEM_STRU  *pstSegDataInfo  = VOS_NULL_PTR;

    /* ������Ƭ��Ϣ���飬�鿴�Ƿ��Ƭ��Ϣ�Ѵ��ڣ���������ֱ�ӷ��� */
    for (ulLoop = NAS_ERABM_NULL; ulLoop < NAS_ERABM_GetIpv4SegBuffItemNum(); ulLoop++)
    {
        pstSegDataInfo = NAS_ERABM_GetIpv4SegBuffItemAddr(ulLoop);
        if ((pstIpv4HeaderInfo->ulIdentifier == pstSegDataInfo->ulIdentifier)
           && (0 == NAS_ERABM_MEM_CMP(  pstIpv4HeaderInfo->aucSrcIpV4Addr,
                                        pstSegDataInfo->aucSrcIpV4Addr,
                                        NAS_ERABM_IPV4_ADDR_LEN))
           && (0 == NAS_ERABM_MEM_CMP(  pstIpv4HeaderInfo->aucDesIpV4Addr,
                                        pstSegDataInfo->aucDesIpV4Addr,
                                        NAS_ERABM_IPV4_ADDR_LEN)))
        {
            pstSegDataInfo->ucBearerId = ucBearerId;
            return;
        }
    }

    /* �洢��Ƭ��Ϣ */
    pstSegDataInfo = NAS_ERABM_GetIpv4SegBuffItemAddr(usIndex);
    pstSegDataInfo->ulIdentifier    = pstIpv4HeaderInfo->ulIdentifier;
    NAS_ERABM_MEM_CPY(  pstSegDataInfo->aucSrcIpV4Addr,
                        pstIpv4HeaderInfo->aucSrcIpV4Addr,
                        NAS_ERABM_IPV4_ADDR_LEN);
    NAS_ERABM_MEM_CPY(  pstSegDataInfo->aucDesIpV4Addr,
                        pstIpv4HeaderInfo->aucDesIpV4Addr,
                        NAS_ERABM_IPV4_ADDR_LEN);
    pstSegDataInfo->ucBearerId = ucBearerId;

    usIndex = (usIndex + 1) % NAS_ERABM_MAX_SEG_BUFF_ITEM_NUM;

    if (NAS_ERABM_GetIpv4SegBuffItemNum() < NAS_ERABM_MAX_SEG_BUFF_ITEM_NUM)
    {
        NAS_ERABM_AddSelfIpv4SegBuffItemNum();
    }
}



VOS_VOID NAS_ERABM_DecodeIpV4Data
(
    VOS_UINT8 *pucSrcData,
    NAS_ERABM_IPV4_HEADER_STRU *pstDestData
)
{
    VOS_UINT8                            *pSrcData    = pucSrcData;
    VOS_UINT32                            ulIndex     = NAS_ERABM_NULL;
    NAS_ERABM_IPV4_HEADER_STRU           *pstIpv4Data = pstDestData;

    /* ��תIdentifier����ʼλ�� */
    ulIndex = ulIndex + 4;

    pstIpv4Data->ulIdentifier = (pSrcData[ulIndex] << NAS_ERABM_MOVEMENT_8_BITS)\
                                | pSrcData[ulIndex+1];

    /* ��תsource ip address ����ʼλ�� */
    ulIndex = ulIndex + 8;
    NAS_ERABM_MEM_CPY(  pstIpv4Data->aucSrcIpV4Addr,
                        &pSrcData[ulIndex],
                        NAS_ERABM_IPV4_ADDR_LEN);

    /* ��תdestination ip address ����ʼλ�� */
    ulIndex = ulIndex + 4;
    NAS_ERABM_MEM_CPY(  pstIpv4Data->aucDesIpV4Addr,
                        &pSrcData[ulIndex],
                        NAS_ERABM_IPV4_ADDR_LEN);
}
VOS_VOID  NAS_ERABM_DecodeIpV6Data
(
    VOS_UINT8 *pucSrcData,
    NAS_ERABM_IPV6_HEADER_STRU *pstDestData
)
{


    VOS_UINT8                            *pSrcData    = pucSrcData;
    VOS_UINT32                            ulIndex     = NAS_ERABM_NULL;
    NAS_ERABM_IPV6_HEADER_STRU           *pstIpv6Data = pstDestData;

    /* ��ת��IPV6Դ��ַ��ʼλ�� */
    ulIndex = ulIndex + 8;

    pSrcData = pSrcData + ulIndex;
    NAS_ERABM_MEM_CPY(pstIpv6Data->aucSrcIpV6Addr, pSrcData, APP_MAX_IPV6_ADDR_LEN);
}
/*lint +e961*/
/*lint +e960*/
#if 0
/*****************************************************************************
 Function Name   : NAS_ERABM_EpsBearLinkedDrbConnectedProc
 Description     : EPS���ع�����DRB����CONNECTED̬�����а�����
 Input           : ulDataLen----------------���ݰ�����
                   pstSdu-------------------���ݰ�
                   ulEpsbId-----------------���غ�
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2010-3-23  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_ERABM_EpsBearLinkedDrbConnectedProc
(
    VOS_UINT32                          ulDataLen,
    LUP_MEM_ST                         *pstSdu,
    VOS_UINT32                          ulEpsbId
)
{
    VOS_UINT32                          ulRbId          = NAS_ERABM_ILL_RB_ID;
    LPDCP_RABM_DATA_STRU                stRabmPdcpDataReq;

    ulRbId = NAS_ERABM_GetEpsbRbIdInfo(ulEpsbId);
    stRabmPdcpDataReq.ucRbId   = (VOS_UINT8)ulRbId;
    stRabmPdcpDataReq.ulSduLen = ulDataLen;
    stRabmPdcpDataReq.pstSdu   = pstSdu;

    /*����PDCP�ṩ�ĺ���*/
    LPDCP_DataReq(&stRabmPdcpDataReq);

    /* ���з������ݰ�������1 */
    NAS_ERABM_AddUlSendPackageNum();

    /* ���ط������ݰ�����1 */
    NAS_ERABM_AddBearerSendPackageNum(ulEpsbId);

    return APP_ERABM_TRANSFER_RESULT_SUCC;
}

/*****************************************************************************
 Function Name   : NAS_ERABM_EpsBearLinkedDrbSuspendedProc
 Description     : EPS���ع�����DRB����SUSPENDED̬�����а�����
 Input           : pstSdu-------------------���ݰ�
                   ulEpsbId-----------------���غ�
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2010-3-23  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_ERABM_EpsBearLinkedDrbSuspendedProc
(
    LUP_MEM_ST                         *pstSdu,
    VOS_UINT32                          ulEpsbId
)
{
    VOS_UINT32                              ulSaveDataRslt  = PS_FAIL;

    /*֪ͨAPPֹͣ���ݴ���,����APP�ṩ�ĺ���*/
    /*AppDataStatusFun( ulEpsbId, NAS_ERABM_APP_DATA_STATUS_STOP, 0xFF );*/

    /* ֪ͨL2�������ݰ� */
    ulSaveDataRslt = APPITF_SaveDataToRabmBuf(pstSdu);

    if (ulSaveDataRslt == PS_FAIL)
    {
        /* ���ж������ݰ�������1 */
        NAS_ERABM_AddUlDiscardPackageNum();

        /* ���ض������ݰ�����1 */
        NAS_ERABM_AddBearerDiscardPackageNum(ulEpsbId);

        return APP_ERABM_TRANSFER_RESULT_FAIL;
    }
    else
    {
        /* ������������������־*/
        NAS_ERABM_SetUpDataPending(EMM_ERABM_UP_DATA_PENDING);

        /* ���л������ݰ�������1 */
        NAS_ERABM_AddUlSavePackageNum();

        /* ���ػ������ݰ�����1 */
        NAS_ERABM_AddBearerSavePackageNum(ulEpsbId);

        return APP_ERABM_TRANSFER_RESULT_SUCC;
    }
}


VOS_UINT32  NAS_ERABM_EpsBearLinkedDrbNotExistProc
(
    LUP_MEM_ST                         *pstSdu,
    VOS_UINT32                          ulEpsbId
)
{
    VOS_UINT32                              ulSaveDataRslt  = PS_FAIL;

    /* RABM��û����EMM���ͽ���SERVICE���� */
    if (NAS_ERABM_SERVICE_STATE_TERMIN == NAS_ERABM_GetEmmSrState())
    {
        NAS_ERABM_SndAppRabmServiceReq();

        /*����SR��Ϣ�����÷����������������������ظ����ͽ���DRB����*/
        NAS_ERABM_SetEmmSrState(NAS_ERABM_SERVICE_STATE_INIT);
    }

    /* ֪ͨL2�������ݰ� */
    ulSaveDataRslt = APPITF_SaveDataToRabmBuf(pstSdu);

    if (ulSaveDataRslt == PS_FAIL)
    {
        /* ���ж������ݰ�������1 */
        NAS_ERABM_AddUlDiscardPackageNum();

        /* ���ض������ݰ�����1 */
        NAS_ERABM_AddBearerDiscardPackageNum(ulEpsbId);

        return APP_ERABM_TRANSFER_RESULT_FAIL;
    }
    else
    {
        /* ������������������־*/
        NAS_ERABM_SetUpDataPending(EMM_ERABM_UP_DATA_PENDING);

        /* ���л������ݰ�������1 */
        NAS_ERABM_AddUlSavePackageNum();

        /* ���ػ������ݰ�����1 */
        NAS_ERABM_AddBearerSavePackageNum(ulEpsbId);

        return APP_ERABM_TRANSFER_RESULT_SUCC;
    }
}

APP_ERABM_TRANSFER_RESULT_ENUM_UINT32 Nas_DataReq
(
    VOS_UINT32                          ulDataLen,
    LUP_MEM_ST                         *pstSdu
)
{
    VOS_UINT32      ulEpsbId = NAS_ERABM_ILL_EPSB_ID;

    NAS_ERABM_AddUlReceivePackageNum();

    if (NAS_ERABM_L_MODE_STATUS_NORMAL != NAS_ERABM_GetLModeStatus())
    {
        /* ���ж������ݰ�������1 */
        NAS_ERABM_AddUlDiscardPackageNum();

        return APP_ERABM_TRANSFER_RESULT_FAIL;
    }

    if(PS_FAIL == NAS_ERABM_IpfExtractEpsbId(pstSdu, &ulEpsbId))
    {
        /* ���ж������ݰ�������1 */
        NAS_ERABM_AddUlDiscardPackageNum();

        return APP_ERABM_TRANSFER_RESULT_FAIL;
    }

    return TC_DataReq(ulDataLen, pstSdu, ulEpsbId);
}


APP_ERABM_TRANSFER_RESULT_ENUM_UINT32 TC_DataReq
(
    VOS_UINT32                          ulDataLen,
    LUP_MEM_ST                         *pstSdu,
    VOS_UINT32                          ulEpsbId
)
{
    if ((ulEpsbId < NAS_ERABM_MIN_EPSB_ID) || (ulEpsbId > NAS_ERABM_MAX_EPSB_ID))
    {
        /* ���ж������ݰ�������1 */
        NAS_ERABM_AddUlDiscardPackageNum();

        /* ����δ�ҵ��������ݰ�������1 */
        NAS_ERABM_AddUlUnfoundBearerPackageNum();

        return APP_ERABM_TRANSFER_RESULT_FAIL;
    }

    /* �ж��Ƿ�������̬ */
    if (NAS_ERABM_SUCCESS == NAS_ERABM_IsAllActtiveBearerWithoutDrb())
    {
        /* IDLE̬�´���SERVICE */
        return NAS_ERABM_EpsBearLinkedDrbNotExistProc(  pstSdu,
                                                       ulEpsbId);
    }

    if (NAS_ERABM_RB_CONNECTED == NAS_ERABM_GetRbStateInfo(ulEpsbId))
    {
        /* EPS���ع�����DRB����CONNECTED̬�����а����� */
        return NAS_ERABM_EpsBearLinkedDrbConnectedProc(  ulDataLen,
                                                        pstSdu,
                                                        ulEpsbId);
    }
    else if(NAS_ERABM_RB_SUSPENDED == NAS_ERABM_GetRbStateInfo(ulEpsbId))
    {
        /* EPS���ع�����DRB����SUSPENDED̬�����а����� */
        return NAS_ERABM_EpsBearLinkedDrbSuspendedProc(  pstSdu,
                                                        ulEpsbId);
    }
    else
    {
        /* ���ж������ݰ�������1 */
        NAS_ERABM_AddUlDiscardPackageNum();

        /* ���ض������ݰ�����1 */
        NAS_ERABM_AddBearerDiscardPackageNum(ulEpsbId);

        /* ����ع���DRB�����ڶ����������ݰ�������1 */
        NAS_ERABM_AddBearerNoDrbDiscardPackageNum(ulEpsbId);

        return APP_ERABM_TRANSFER_RESULT_FAIL;
    }
}
#endif



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

