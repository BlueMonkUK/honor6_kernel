/******************************************************************************

   Copyright(C)2013,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : ImsaProcCdsMsg.c
  Description     : ��C�ļ�ʵ��CDS��Ϣ�����CDS��Ϣ����
  History           :
     1.sunbing 49683      2013-06-19  Draft Enact

******************************************************************************/

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "ImsaProcCdsMsg.h"
#include "ImsaEntity.h"
#include "ImsaPublic.h"

/*lint -e767*/
#define    THIS_FILE_ID   PS_FILE_ID_IMSAPROCCDSMSG_C
/*lint +e767*/

/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#if (FEATURE_ON == FEATURE_IMS)
/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/


/*****************************************************************************
  3 Function
*****************************************************************************/

VOS_VOID IMSA_CONN_SndCdsSetImsBearerReq( VOS_VOID )
{
    IMSA_CDS_SET_IMS_BEARER_REQ_STRU   *pstSetImsBearerReq  = VOS_NULL_PTR;
    IMSA_NORMAL_CONN_STRU              *pstNormalConn       = VOS_NULL_PTR;
    IMSA_EMC_CONN_STRU                 *pstEmcConn          = VOS_NULL_PTR;
    VOS_UINT32                          i                   = IMSA_NULL;
    IMSA_CDS_IMS_BEARER_STRU           *pstCdsImsBearer     = VOS_NULL_PTR;
    IMSA_CONTROL_MANAGER_STRU          *pstControlManager   = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_CONN_SndCdsSetImsBearerReq is entered!");

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstSetImsBearerReq = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_CDS_SET_IMS_BEARER_REQ_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pstSetImsBearerReq)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_CONN_SndCdsSetImsBearerReqS:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstSetImsBearerReq), 0, IMSA_GET_MSG_LENGTH(pstSetImsBearerReq));

    /*��д��Ϣͷ*/
    /*lint -e960*/
    IMSA_WRITE_CDS_MSG_HEAD(pstSetImsBearerReq, ID_IMSA_CDS_SET_IMS_BEARER_REQ);
    /*lint +e960*/

    /*��д��Ӧ���*/
    pstEmcConn                          = IMSA_CONN_GetEmcConnAddr();
    if (IMSA_PDP_STATE_ACTIVE           == pstEmcConn->stSipSignalPdp.enPdpState)
    {
        pstCdsImsBearer                 = &pstSetImsBearerReq->astImsBearerArray[pstSetImsBearerReq->ulImsBearerNum];
        pstCdsImsBearer->ucEpsbId       = pstEmcConn->stSipSignalPdp.ucPdpId;
        pstCdsImsBearer->enBearerType   = IMSA_CDS_IMS_BEARER_TYPE_SIGNAL;

        pstSetImsBearerReq->ulImsBearerNum++;
    }

    for (i = 0; i < pstEmcConn->ulSipMediaPdpNum; i++)
    {
       if (IMSA_PDP_STATE_ACTIVE           == pstEmcConn->astSipMediaPdpArray[i].enPdpState)
        {
            pstCdsImsBearer                 = &pstSetImsBearerReq->astImsBearerArray[pstSetImsBearerReq->ulImsBearerNum];
            pstCdsImsBearer->ucEpsbId       = pstEmcConn->astSipMediaPdpArray[i].ucPdpId;
            pstCdsImsBearer->enBearerType   = IMSA_CDS_IMS_BEARER_TYPE_MEDIA;

            pstSetImsBearerReq->ulImsBearerNum++;
        }
    }

    pstNormalConn                       = IMSA_CONN_GetNormalConnAddr();

    for (i = 0; i < pstNormalConn->ulSipSignalPdpNum; i++)
    {
       if (IMSA_PDP_STATE_ACTIVE           == pstNormalConn->astSipSignalPdpArray[i].enPdpState)
        {
            pstCdsImsBearer                 = &pstSetImsBearerReq->astImsBearerArray[pstSetImsBearerReq->ulImsBearerNum];
            pstCdsImsBearer->ucEpsbId       = pstNormalConn->astSipSignalPdpArray[i].ucPdpId;
            pstCdsImsBearer->enBearerType   = IMSA_CDS_IMS_BEARER_TYPE_SIGNAL;

            pstSetImsBearerReq->ulImsBearerNum++;
        }
    }

    for (i = 0; i < pstNormalConn->ulSipMediaPdpNum; i++)
    {
       if (IMSA_PDP_STATE_ACTIVE           == pstNormalConn->astSipMediaPdpArray[i].enPdpState)
        {
            pstCdsImsBearer                 = &pstSetImsBearerReq->astImsBearerArray[pstSetImsBearerReq->ulImsBearerNum];
            pstCdsImsBearer->ucEpsbId       = pstNormalConn->astSipMediaPdpArray[i].ucPdpId;
            pstCdsImsBearer->enBearerType   = IMSA_CDS_IMS_BEARER_TYPE_MEDIA;

            pstSetImsBearerReq->ulImsBearerNum++;
        }
    }

    /* ��IMS����������0����Я��IMS�˿ںŷ�Χ��Ϣ */
    pstControlManager = IMSA_GetControlManagerAddress();
    if (pstSetImsBearerReq->ulImsBearerNum > 0)
    {
        pstSetImsBearerReq->stImsPortInfo.usMinImsPort
                = pstControlManager->stImsaConfigPara.stImsPortConfig.usImsMinPort;
        pstSetImsBearerReq->stImsPortInfo.usMaxImsPort
                = pstControlManager->stImsaConfigPara.stImsPortConfig.usImsMaxPort;
    }

    /*������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstSetImsBearerReq);
}

/*****************************************************************************
 Function Name  : IMSA_CdsMsgDistr()
 Description    : CDS��Ϣ�ַ�����
 Input          : VOS_VOID *pRcvMsg
 Output         : VOS_VOID
 Return Value   : VOS_VOID

 History        :
      1.sunbing 49683      2013-06-20  Draft Enact
*****************************************************************************/
VOS_VOID IMSA_CdsMsgDistr(const VOS_VOID *pRcvMsg )
{
    /* ������Ϣͷָ��*/
    PS_MSG_HEADER_STRU          *pHeader = VOS_NULL_PTR;
    IMSA_CONTROL_MANAGER_STRU   *pstControlManager = IMSA_GetControlManagerAddress();

    /* ��ȡ��Ϣͷָ��*/
    pHeader = (PS_MSG_HEADER_STRU *) pRcvMsg;

    /* �ػ��������յ�CDS���͵���Ϣ��ֱ�Ӷ��� */
    if(IMSA_STATUS_STOPING == pstControlManager->enImsaStatus)
    {
        IMSA_WARN_LOG("IMSA_CdsMsgDistr: Status is Stoping!");
        return;
    }

    switch(pHeader->ulMsgName)
    {
        /*���ļ����*/
        case ID_IMSA_CDS_SET_IMS_BEARER_CNF:
            IMSA_INFO_LOG("IMSA_CdsMsgDistr: ID_IMSA_CDS_SET_IMS_BEARER_CNF!");

            break;

        default:
            IMSA_ERR_LOG("IMSA_CdsMsgDistr: Msg id is err.");
            break;

    }
}

#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
/* end of ImsaProcCdsMsg.c */



