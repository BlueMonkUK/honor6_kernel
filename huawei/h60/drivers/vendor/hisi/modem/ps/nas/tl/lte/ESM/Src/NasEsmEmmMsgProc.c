

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include    "NasEsmEmmMsgProc.h"
#include    "NasEsmNwMsgProc.h"
#include    "NasEsmInclude.h"
#include    "NasEsmIpMsgProc.h"
/*#include	"IpDhcpv4Server.h"*/
#include "NasEsmAppMsgParaProc.h"


/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASESMEMMMSGPROC_C
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



/*****************************************************************************
  3 Function
*****************************************************************************/

VOS_VOID NAS_ESM_EmmMsgDistr( VOS_VOID *pRcvMsg )
{
    PS_MSG_HEADER_STRU         *pEmmMsg  = VOS_NULL_PTR;

    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG("NAS_ESM_EmmMsgDistr is entered.");

    pEmmMsg = (PS_MSG_HEADER_STRU*)pRcvMsg;

    /*������Ϣ����������Ӧ����Ϣ������*/
    switch(pEmmMsg->ulMsgName)
    {
        case ID_EMM_ESM_SUSPEND_IND:
            NAS_ESM_RcvEsmEmmSuspendInd( (EMM_ESM_SUSPEND_IND_STRU *) pRcvMsg );
            break;

        case ID_EMM_ESM_RESUME_IND:
            NAS_ESM_RcvEsmEmmResumeInd( (EMM_ESM_RESUME_IND_STRU *) pRcvMsg );
            break;
        /*����յ�����ID_EMM_ESM_PDN_CON_IND��Ϣ*/
        case ID_EMM_ESM_PDN_CON_IND:

            /*����Sm_RcvEsmEmmDataInd����*/
            NAS_ESM_RcvEsmEmmPdnConInd( (EMM_ESM_PDN_CON_IND_STRU *) pRcvMsg );
            break;

        /*����յ�����ID_EMM_ESM_DATA_IND��Ϣ*/
        case ID_EMM_ESM_DATA_IND:

            /*����Sm_RcvEsmEmmDataInd����*/
            NAS_ESM_RcvEsmEmmDataInd( (EMM_ESM_DATA_IND_STRU *) pRcvMsg );
            break;

        /*����յ�����ID_EMM_ESM_EST_CNF��Ϣ*/
        case ID_EMM_ESM_EST_CNF:

            /*����Sm_RcvEsmEmmDataInd����*/
            NAS_ESM_RcvEsmEmmEstCnf( (EMM_ESM_EST_CNF_STRU *) pRcvMsg );
            break;

        /*����յ�����ID_EMM_ESM_STATUS_IND��Ϣ*/
        case ID_EMM_ESM_STATUS_IND:

            /*����Sm_RcvEsmEmmStatusInd����*/
            NAS_ESM_RcvEsmEmmStatusInd( (EMM_ESM_STATUS_IND_STRU *) pRcvMsg );
            break;

        /*����յ�����ID_EMM_ESM_BEARER_STATUS_IND��Ϣ*/
        case ID_EMM_ESM_BEARER_STATUS_IND:

            /*����Sm_RcvEsmEmmBearerStatusInd����*/
            NAS_ESM_RcvEsmEmmBearerStatusInd( (EMM_ESM_BEARER_STATUS_IND_STRU *) pRcvMsg );
            break;

        /*����յ�����ID_EMM_ESM_REL_IND��Ϣ*/
        case ID_EMM_ESM_REL_IND:

            /* ���ESM���ڷ�ע��̬����ֱ�ӷ��� */
            if (NAS_ESM_PS_REGISTER_STATUS_DETACHED == NAS_ESM_GetEmmStatus())
            {
                NAS_ESM_WARN_LOG("NAS_ESM_EmmMsgDistr:WARNING:Msg is discard cause ESM state is detached!");
                return ;
            }

            NAS_ESM_RcvEmmDetachMsg();

            break;
        case ID_EMM_ESM_DEACT_NON_EMC_BEARER_IND:
            NAS_ESM_RcvEsmEmmDeactAllNonEmcBearerInd();
            break;
        /*PC REPLAY MODIFY BY LEILI END*/
        #if(VOS_WIN32 == VOS_OS_VER)
        case ID_EMM_ESM_REPLAY_EXPORT_CTX_IND:
            NAS_ESM_WARN_LOG("NAS_ESM_EmmMsgDistr:WARNING:EMM->SM Message ID_EMM_ESM_REPLAY_EXPORT_CTX_IND!");
            (void)NAS_ESM_ExportContextData();
            (void)NAS_ERABM_ExportContextData();

            break;
        #endif
        /*PC REPLAY MODIFY BY LEILI END*/
        default:
            NAS_ESM_WARN_LOG("NAS_ESM_EmmMsgDistr:WARNING:EMM->SM Message name non-existent!");
            break;
    }
}

/*lint -e960*/
/*lint -e961*/
VOS_VOID  NAS_ESM_RcvEsmEmmSuspendInd(const EMM_ESM_SUSPEND_IND_STRU *pRcvMsg )
{
    VOS_UINT32                          ulCnt               = NAS_ESM_NULL;
    NAS_ESM_BUFF_MANAGE_INFO_STRU      *pstEsmBuffManInfo   = NAS_ESM_GetEsmBufferManageInfoAddr();

    (VOS_VOID)pRcvMsg;

    /* ����״̬Ϊ����̬ */
    NAS_ESM_SetLModeStatus(NAS_ESM_L_MODE_STATUS_SUSPENDED);

    /*����ӳ��ͷŷǽ������ض�ʱ�����ڣ���ֹͣ*/
    NAS_ESM_TimerStop(0, TI_NAS_ESM_REL_NON_EMC_BEARER);

    /* ��ն�̬�������APP��Ϣ����ظ�APP��Ӧ����ʧ�ܣ�ԭ��ֵΪ���� */
    NAS_ESM_ClearStateTable(APP_ERR_SM_SUSPENDED, PS_FALSE);

    /* �ͷ�ATTACHING���ͻ����¼ */
    for (ulCnt = NAS_ESM_NULL; ulCnt < NAS_ESM_MAX_ATTACHING_BUFF_ITEM_NUM; ulCnt++)
    {
        if (pstEsmBuffManInfo->paAttBuffer[ulCnt] != VOS_NULL_PTR)
        {
            NAS_ESM_ClearEsmBuffItem(NAS_ESM_BUFF_ITEM_TYPE_ATTACHING, ulCnt);
        }
    }

    /* ��ESM��ǰ״̬Ϊע���У���״̬תΪȥע��̬����GUģע��ɹ���֪ͨLMM��
       ����LMM֪ͨESM����ע��̬�� */
    if (NAS_ESM_PS_REGISTER_STATUS_ATTACHING == NAS_ESM_GetEmmStatus())
    {
        NAS_ESM_SetEmmStatus(NAS_ESM_PS_REGISTER_STATUS_DETACHED);
    }

    /* ���֧��˫APN����ֹͣע������ؽ���ʱ�� */
    if (NAS_ESM_OP_TRUE == NAS_ESM_AttachBearerReestEnable())
    {
        NAS_ESM_TimerStop(0, TI_NAS_ESM_ATTACH_BEARER_REEST);
    }

    /* �ظ�EMM����ɹ� */
    NAS_ESM_SndEsmEmmSuspendRsp(EMM_ESM_RSLT_TYPE_SUCC);
}


VOS_VOID  NAS_ESM_RcvEsmEmmResumeInd(const EMM_ESM_RESUME_IND_STRU *pRcvMsg )
{
    (VOS_VOID)pRcvMsg;

    /* ����״̬Ϊ����̬ */
    NAS_ESM_SetLModeStatus(NAS_ESM_L_MODE_STATUS_NORMAL);

    /* �ظ�EMM��ҳɹ� */
    NAS_ESM_SndEsmEmmResumeRsp(EMM_ESM_RSLT_TYPE_SUCC);

    /* ���֧��˫APN�����ж��Ƿ���Ҫ����ע������ؽ���ʱ�� */
    if (NAS_ESM_OP_TRUE == NAS_ESM_AttachBearerReestEnable())
    {
        if ((NAS_ESM_GetCurPdnNum() > 0) &&
            (NAS_ESM_UNASSIGNED_EPSB_ID == NAS_ESM_GetAttachBearerId()))
        {
            /* ���ע������ؽ���ʱ��ʱ����Ϊ0�����������ؽ����� */
            if (0 != NAS_ESM_GetAttachBearerReestTimerLen())
            {
                NAS_ESM_TimerStart(0, TI_NAS_ESM_ATTACH_BEARER_REEST);
            }
        }
    }
}


VOS_VOID  NAS_ESM_RcvEsmEmmPdnConInd(const EMM_ESM_PDN_CON_IND_STRU *pRcvMsg )
{
    VOS_UINT32                          ulPti               = NAS_ESM_NULL;
    VOS_UINT32                          ulRegCId            = NAS_ESM_NULL;
    VOS_UINT32                          ulStateTblId        = NAS_ESM_NULL;
    VOS_UINT32                          ulCur               = NAS_ESM_NULL;
    VOS_UINT32                          ulSdfNum            = NAS_ESM_NULL;
    NAS_ESM_SDF_PARA_STRU              *pstSdfPara          = VOS_NULL_PTR;
    APP_ESM_SDF_PARA_STRU              *pstAppSdfPara       = VOS_NULL_PTR;
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr        = VOS_NULL_PTR;
    NAS_ESM_ENCODE_INFO_STRU            stEncodeInfo;
    NAS_ESM_PDP_MANAGE_INFO_STRU       *pEpsbManageInfo     = VOS_NULL_PTR;


    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG("NAS_ESM_RcvEsmEmmPdnConInd is entered.");

    (VOS_VOID)pRcvMsg;

    if (NAS_ESM_L_MODE_STATUS_NORMAL != NAS_ESM_GetLModeStatus())
    {
        NAS_ESM_WARN_LOG("NAS_ESM_RcvEsmEmmPdnConInd: L mode is already suspended!");

        NAS_ESM_SetLModeStatus(NAS_ESM_L_MODE_STATUS_NORMAL);
    }

    if (NAS_ESM_PS_REGISTER_STATUS_DETACHED != NAS_ESM_GetEmmStatus())
    {
        NAS_ESM_NORM_LOG("NAS_ESM_RcvEsmEmmPdnConInd:NORM: received abnormal Pdn Con Ind !");

        if (PS_TRUE == NAS_ESM_HasEmergencyPdn())
        {
            /*����н������أ���������ͷ����зǽ�������*/
            NAS_ESM_ClearEsmResoureWhenEstingEmcPdn();
        }
        else
        {
            /*ESMȥע�ᣬ�ͷ���Դ*/
            NAS_ESM_RcvEmmDetachMsg();
        }
    }

    pstAppSdfPara = NAS_ESM_MEM_ALLOC(sizeof(APP_ESM_SDF_PARA_STRU));

    if (VOS_NULL_PTR == pstAppSdfPara)
    {
         /*��ӡ���Ϸ���Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_RcvEsmEmmPdnConInd:ERROR: Mem alloc fail!");

        /* �ظ�EMM ID_EMM_ESM_PDN_CON_RSP��FAIL�� */
        NAS_ESM_SndEsmEmmPdnConRspMsg(EMM_ESM_PDN_CON_RSLT_FAIL, 0,0);

        return;
    }

    /* ��һ��ȡ12��NV���� */
    for (ulCur = NAS_ESM_MIN_CID; ulCur < NAS_ESM_CID_NV_NUM; ulCur++)
    {
        pstAppSdfPara->ulCId = ulCur;
        /*PC REPLAY MODIFY BY LEILI BEGIN*/
        if (APP_FAILURE == NAS_ESM_GetSdfPara(&ulSdfNum, pstAppSdfPara))
        {
            continue;
        }

        pstSdfPara = NAS_ESM_GetSdfParaAddr(ulCur);
        NAS_ESM_MEM_CPY(pstSdfPara, pstAppSdfPara, sizeof(APP_ESM_SDF_PARA_STRU));
    }

    NAS_ESM_MEM_FREE(pstAppSdfPara);

    /* ��APP��ȡ���ع���ģʽ���������ȡ�Զ�����ģʽ */
    pEpsbManageInfo = NAS_ESM_GetBearerManageInfoAddr();
    if (APP_FAILURE == NAS_ESM_GetPdpManageInfo(pEpsbManageInfo))
    {
        /*��ӡ������Ϣ*/
        NAS_ESM_WARN_LOG("NAS_ESM_RcvEsmEmmPdnConInd:WARN: Fail to get PDP Manage Info from APP!");
        pEpsbManageInfo->enMode = APP_ESM_BEARER_MANAGE_MODE_AUTO;
        pEpsbManageInfo->enType = APP_ESM_BEARER_MANAGE_TYPE_ACCEPT;
    }
    /*PC REPLAY MODIFY BY LEILI END*/
    /* ��ȡ����ע���CID�����ȼ�˳���ɸߵ���ΪCID0 > CID1-CID11 > CID12-CID20 */
    /* ���˸�����Ӫ����������, ��ҪGU NV��CID0��ʼ��Ϊר�����ͣ�CID11Ϊȱʡ����*/
    ulRegCId = NAS_ESM_GetRegistCid();

    if (NAS_ESM_ILL_CID == ulRegCId)
    {
        /*��ӡ���Ϸ���Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_RcvEsmEmmPdnConInd: There is no default bearer type sdf");

        /* �ظ�EMM ID_EMM_ESM_PDN_CON_RSP��FAIL�� */
        NAS_ESM_SndEsmEmmPdnConRspMsg(EMM_ESM_PDN_CON_RSLT_FAIL, 0,0);

        return ;
    }

    NAS_ESM_LOG1("NAS_ESM_RcvEsmEmmPdnConInd:The cid of the found sdf is :", ulRegCId);

    /*�ڶ�̬���У���ȡ���̱�ʶ*/
    if(NAS_ESM_SUCCESS != NAS_ESM_GetStateTblIndexByCid(NAS_ESM_ATTACH_CID,&ulStateTblId))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_RcvEsmEmmPdnConInd:WARNING:insufficient resource!");

        /* �ظ�EMM ID_EMM_ESM_PDN_CON_RSP��FAIL��*/
        NAS_ESM_SndEsmEmmPdnConRspMsg(EMM_ESM_PDN_CON_RSLT_FAIL, 0,0);

        return ;
    }

    /*����ѯ����ulRegCId����������CID0�У�����ע��*/
    pstSdfPara = NAS_ESM_GetSdfParaAddr(NAS_ESM_ATTACH_CID);
    NAS_ESM_MEM_CPY(    pstSdfPara,
                        NAS_ESM_GetSdfParaAddr(ulRegCId),
                        sizeof(NAS_ESM_SDF_PARA_STRU));

    /*���ó�ʼ״̬*/
    pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblId);
    pstStateAddr->enBearerCntxtState = NAS_ESM_BEARER_STATE_INACTIVE;
    pstStateAddr->enBearerCntxtType  = NAS_ESM_BEARER_TYPE_DEFAULT;
    if(NAS_ESM_PDN_REQ_TYPE_HANDOVER == pRcvMsg->enPdnReqType)
    {
        pstStateAddr->enPdnReqType = NAS_ESM_PDN_REQ_TYPE_HANDOVER;
    }
    else
    {
        pstStateAddr->enPdnReqType = NAS_ESM_PDN_REQ_TYPE_INITIAL;
    }

    /*����PTI,����洢��������Ϣ��*/
    NAS_ESM_AssignPTI(&ulPti);
    pstStateAddr->stNwMsgRecord.ucPti = (VOS_UINT8)ulPti;

    /*���NAS_ESM_ENCODE_INFO_STRU*/
    NAS_ESM_MEM_SET((VOS_VOID*)&stEncodeInfo, 0, sizeof(NAS_ESM_ENCODE_INFO_STRU));
    stEncodeInfo.bitOpESMCau        = NAS_ESM_OP_FALSE;
    stEncodeInfo.ulCid              = pstStateAddr->ulCid;
    stEncodeInfo.ucPti              = (VOS_UINT8)ulPti;
    stEncodeInfo.ulEpsbId           = pstStateAddr->ulEpsbId;
    stEncodeInfo.enPdnReqType       = pstStateAddr->enPdnReqType;

    /*��װPDN_Connect_Req��Ϣ*/
    if( NAS_ESM_FAILURE == NAS_ESM_EncodePdnConReqMsg(stEncodeInfo, \
                                                      pstStateAddr->stNwMsgRecord.aucMsgBuff,\
                                                      &pstStateAddr->stNwMsgRecord.ulMsgLength))
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_RcvEsmEmmPdnConInd:ERROR:EncodePdnConReqMsg fail!");

        /* �ظ�EMM ID_EMM_ESM_PDN_CON_RSP��FAIL��*/
        NAS_ESM_SndEsmEmmPdnConRspMsg(EMM_ESM_PDN_CON_RSLT_FAIL, 0,0);

        NAS_ESM_RelStateTblResource(ulStateTblId);

        return ;
    }

    /*��¼�տ���Ϣ����*/
    pstStateAddr->bitOpNwMsgRecord = NAS_ESM_OP_TRUE;
    pstStateAddr->stNwMsgRecord.enEsmCnMsgType \
            = NAS_ESMCN_MSG_TYPE_PDN_CONNECT_REQ;

    /*��EMM����ID_EMM_ESM_PDN_CON_RSP*/
    NAS_ESM_SndEsmEmmPdnConRspMsg(EMM_ESM_PDN_CON_RSLT_SUCC, \
                                  pstStateAddr->stNwMsgRecord.ulMsgLength,\
                                  pstStateAddr->stNwMsgRecord.aucMsgBuff);

    /*ת��״̬*/
    NAS_ESM_SetProcTransState(ulStateTblId, NAS_ESM_BEARER_PROC_TRANS_STATE_PENDING);
    NAS_ESM_SetEmmStatus(NAS_ESM_PS_REGISTER_STATUS_ATTACHING);
}

/*****************************************************************************
 Function Name   : NAS_ESM_BufferDecodedNwMsg
 Description     : ����ESM��Ϣ
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2009-8-19  Draft Enact
    2.lihong00150010      2010-02-09 Modify
    3.lihong00150010      2010-04-22 Modify

*****************************************************************************/
VOS_VOID  NAS_ESM_BufferDecodedNwMsg
(
    const EMM_ESM_INTRA_DATA_IND_STRU  *pstEmmEsmIntraDataIndMsg,
    NAS_ESM_CAUSE_ENUM_UINT8            enEsmCause
)
{
    NAS_ESM_ATTACHING_BUFF_ITEM_STRU   *pstDecodedNwMsg     = VOS_NULL_PTR;
    NAS_ESM_BUFF_MANAGE_INFO_STRU      *pstEsmBuffManInfo   = NAS_ESM_GetEsmBufferManageInfoAddr();
    VOS_UINT32                          ulBuffItemLen       = NAS_ESM_NULL;
    VOS_UINT32                          ulCnt               = NAS_ESM_NULL;

    /* �ж��Ƿ��ѵ���ϵͳ�ܹ��洢�����¼�ļ��� */
    if (pstEsmBuffManInfo->ucAttBuffItemNum >= NAS_ESM_MAX_ATTACHING_BUFF_ITEM_NUM)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_BufferDecodedNwMsg:WARN:Attaching Buffer is full!");
        return;
    }

    /* ���㻺���¼���� */
    ulBuffItemLen = sizeof(NAS_ESM_ATTACHING_BUFF_ITEM_STRU);

    /* ��ȡ�����¼�ռ� */
    pstDecodedNwMsg = (NAS_ESM_ATTACHING_BUFF_ITEM_STRU *)
                                NAS_COMM_AllocBuffItem( NAS_COMM_BUFF_TYPE_ESM,
                                                        ulBuffItemLen);
    if (VOS_NULL_PTR == pstDecodedNwMsg)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_BufferDecodedNwMsg:WARN:NAS_COMM_AllocBuffItem is failed!");
        return;
    }

    /* ���������Ժ��������Ϣ */
    NAS_ESM_MEM_CPY(&pstDecodedNwMsg->stDecodedNwMsg,
                    pstEmmEsmIntraDataIndMsg,
                    sizeof(EMM_ESM_INTRA_DATA_IND_STRU));

    /* ���������� */
    pstDecodedNwMsg->enDecodeRslt = enEsmCause;

    /* �������ڴ洢�����¼�׵�ַ��ָ�� */
    for (ulCnt = NAS_ESM_NULL; ulCnt < NAS_ESM_MAX_ATTACHING_BUFF_ITEM_NUM; ulCnt++)
    {
        if (pstEsmBuffManInfo->paAttBuffer[ulCnt] == VOS_NULL_PTR)
        {
            break;
        }
    }

    if (ulCnt >= NAS_ESM_MAX_ATTACHING_BUFF_ITEM_NUM)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_BufferDecodedNwMsg:Get the valuable which stores buffer item addr failed!");
        return ;
    }

    /* ��¼�洢�����¼���׵�ַ */
    pstEsmBuffManInfo->paAttBuffer[ulCnt] = (VOS_VOID *)pstDecodedNwMsg;

    /* ���ӻ����¼���� */
    pstEsmBuffManInfo->ucAttBuffItemNum++;

    NAS_ESM_INFO_LOG1("NAS_ESM_BufferDecodedNwMsg:INFO:Buffer network msg success!", ulCnt);
}


VOS_VOID NAS_ESM_RcvEsmEmmDataInd( EMM_ESM_DATA_IND_STRU *pRcvMsg )
{
    NAS_ESM_CAUSE_ENUM_UINT8            enEsmCause          = NAS_ESM_CAUSE_SUCCESS;
    VOS_UINT32                          ulRcvNwMsgLen       = NAS_ESM_NULL;
    EMM_ESM_INTRA_DATA_IND_STRU        *pstEmmEsmIntraDataIndMsg = VOS_NULL_PTR;
    VOS_UINT8                          *pucTmpSmMsg         = VOS_NULL_PTR;
    NAS_ESM_SND_NW_MSG_STRU            *pstEsmSndNwMsg      = VOS_NULL_PTR;
    NAS_ESM_NW_MSG_STRU                *pstEsmNwMsgIE       = VOS_NULL_PTR;

    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG("NAS_ESM_RcvEsmEmmDataInd is entered.");

    if (NAS_ESM_L_MODE_STATUS_NORMAL != NAS_ESM_GetLModeStatus())
    {
        NAS_ESM_WARN_LOG("NAS_ESM_RcvEsmEmmDataInd: L mode is already suspended!");
        return ;
    }

    /* ���ESM���ڷ�ע��̬����ֱ�ӷ��� */
    if (NAS_ESM_PS_REGISTER_STATUS_DETACHED == NAS_ESM_GetEmmStatus())
    {
        NAS_ESM_WARN_LOG("NAS_ESM_RcvEsmEmmDataInd:WARNING:Msg is discard cause ESM state is detached!");
        return ;
    }

    pstEmmEsmIntraDataIndMsg = NAS_ESM_MEM_ALLOC(sizeof(EMM_ESM_INTRA_DATA_IND_STRU));
    if(VOS_NULL_PTR == pstEmmEsmIntraDataIndMsg)
    {
        NAS_ESM_ERR_LOG("NAS_ESM_RcvEsmEmmDataInd:Alloc Mem fail!");
        return;
    }

    NAS_ESM_MEM_SET(pstEmmEsmIntraDataIndMsg, 0, sizeof(EMM_ESM_INTRA_DATA_IND_STRU));

    /*�����ڲ���ID_EMM_ESM_DATA_IND��Ϣ(Я��һ���ѽ����Ŀտ���Ϣ)*/
    pstEmmEsmIntraDataIndMsg->ulSenderPid = PS_PID_MM;

    pstEsmNwMsgIE = &(pstEmmEsmIntraDataIndMsg->stEsmNwMsgIE);

    /*ȡ������Ϣ��������SM��Ϣ�ĳ���*/
    ulRcvNwMsgLen = pRcvMsg->stEsmMsg.ulEsmMsgSize;
    if( ulRcvNwMsgLen > ESM_MAX_MSG_CONTAINER_CNTNTS_LEN )
    {
        NAS_ESM_WARN_LOG("NAS_ESM_RcvEsmEmmDataInd:WARNING:EMM->SM Message NW Msg over-long!");
        ulRcvNwMsgLen = ESM_MAX_MSG_CONTAINER_CNTNTS_LEN;
    }

    /*ȡ��SM��Ϣ�׵�ַ*/
    pucTmpSmMsg = pRcvMsg->stEsmMsg.aucEsmMsg;

    pstEmmEsmIntraDataIndMsg->ulMsgId = ID_EMM_ESM_DATA_IND;

    /*���NAS_ESM_NW_MSG_STRU*/
    NAS_ESM_MEM_SET((VOS_VOID*)pstEsmNwMsgIE, 0, sizeof(NAS_ESM_NW_MSG_STRU));

    /*���ÿտ���Ϣ��������, �������ʧ��ֱ�ӷ���*/
    enEsmCause = NAS_ESM_DecodeNwMsg( pucTmpSmMsg,
                                   &ulRcvNwMsgLen,
                                    pstEsmNwMsgIE );

    /* ��SM��״̬ΪNAS_ESM_PS_REGISTER_STATUS_ATTACHING���Ҵ���Ϣ��Ϊ
       ESM INFORMATION REQ��Ϣ����ȱʡ���ؼ���������Ϣ�������Ϣ���л��棻
       ����ֱ�Ӵ�����Ϣ */
    if((NAS_ESM_PS_REGISTER_STATUS_ATTACHING == NAS_ESM_GetEmmStatus())
        && (NAS_ESMCN_MSG_TYPE_ESM_INFORMATION_REQ != pstEsmNwMsgIE->enEsmCnMsgType)
        && (NAS_ESMCN_MSG_TYPE_ACT_DEFLT_EPS_BEARER_CNTXT_REQ != pstEsmNwMsgIE->enEsmCnMsgType)
        && (NAS_ESMCN_MSG_TYPE_PDN_CONNECT_REJ != pstEsmNwMsgIE->enEsmCnMsgType))
    {
        /* ����ESM��Ϣ */
        NAS_ESM_BufferDecodedNwMsg(pstEmmEsmIntraDataIndMsg, enEsmCause);

        NAS_ESM_MEM_FREE(pstEmmEsmIntraDataIndMsg);

        return;
    }

    /*����ռ�ͼ���Ƿ����ɹ�,���ڴ��SM׼���ظ����������Ϣ*/
    pstEsmSndNwMsg = NAS_ESM_GetEsmSndNwMsgAddr();
    pstEsmSndNwMsg->pucNwMsg = (VOS_UINT8 *)NAS_ESM_MEM_ALLOC(ESM_MAX_MSG_CONTAINER_CNTNTS_LEN);
    if ( VOS_NULL_PTR == pstEsmSndNwMsg->pucNwMsg )
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_RcvEsmEmmDataInd:ERROR:Alloc Snd Nw Msg fail!");

        NAS_ESM_MEM_FREE(pstEmmEsmIntraDataIndMsg);
        return;
    }

    /*����������󣬷��ؾܾ�����*/
    if( NAS_ESM_CAUSE_SUCCESS != enEsmCause )
    {
        NAS_ESM_ProcNwMsgForDecodeError(pstEsmNwMsgIE,enEsmCause);

        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_WARN_LOG("NAS_ESM_RcvEsmEmmDataInd:WARNING: NW Message Error!");
    }
    else/*�����Ϣ�����ɹ�*/
    {
        /*��������տ���Ϣ*/
        NAS_ESM_NwMsgProcess(pstEmmEsmIntraDataIndMsg);
    }

    /*�ͷſտ���Ϣռ�õĿռ�*/
    NAS_ESM_MEM_FREE(pstEsmSndNwMsg->pucNwMsg);
    pstEsmSndNwMsg->pucNwMsg = VOS_NULL_PTR;
    pstEsmSndNwMsg->ulNwMsgSize = NAS_ESM_NULL;

    NAS_ESM_MEM_FREE(pstEmmEsmIntraDataIndMsg);
}



VOS_VOID NAS_ESM_RcvEsmEmmBearerStatusInd(const EMM_ESM_BEARER_STATUS_IND_STRU *pRcvMsg )
{
    VOS_UINT32                  ulCnt                       = NAS_ESM_NULL;
    VOS_UINT32                  ulQueryRslt                 = NAS_ESM_FAILURE;
    VOS_UINT32                  ulEpsbId                    = NAS_ESM_NULL;
    VOS_UINT32                  ulEpsbIdNum                 = NAS_ESM_NULL;
    VOS_UINT32                  aulEpsbId[NAS_ESM_MAX_EPSB_NUM] = {NAS_ESM_NULL};
    VOS_UINT32                  ulStateTblIndex             =  NAS_ESM_NULL;
    NAS_ESM_EPSB_CNTXT_INFO_STRU *pstEpsbCntxtInfo          = VOS_NULL_PTR;

    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG("NAS_ESM_RcvEsmEmmBearerStatusInd is entered.");

    if (NAS_ESM_L_MODE_STATUS_NORMAL != NAS_ESM_GetLModeStatus())
    {
        NAS_ESM_WARN_LOG("NAS_ESM_RcvEsmEmmBearerStatusInd: L mode is already suspended!");
        return ;
    }

    /* ���ESM���ڷ�ע��̬��ע����̬����ֱ�ӷ��� */
    if (NAS_ESM_PS_REGISTER_STATUS_ATTACHED != NAS_ESM_GetEmmStatus())
    {
        NAS_ESM_WARN_LOG("NAS_ESM_RcvEsmEmmBearerStatusInd:WARNING:Msg is discard cause ESM state is not attached!");
        return ;
    }

    /*ȡ��ID_EMM_ESM_BEARER_STATUS_IND��Ϣ�е�EPSID��Ϣ*/
    ulEpsbIdNum = pRcvMsg->ulEpsIdNum;
    NAS_ESM_MEM_CPY(aulEpsbId, pRcvMsg->aulEsmEpsId, ulEpsbIdNum*(sizeof(VOS_UINT32)/sizeof(VOS_UINT8)));

    /*��ѯÿһ��������Ϣ*/
    for( ulEpsbId = NAS_ESM_MIN_EPSB_ID; ulEpsbId <= NAS_ESM_MAX_EPSB_ID; ulEpsbId++ )
    {
        /*ȡ��������Ϣ*/
        pstEpsbCntxtInfo = NAS_ESM_GetEpsbCntxtInfoAddr(ulEpsbId);

        /*����˳���δ����*/
        if( NAS_ESM_BEARER_STATE_INACTIVE == pstEpsbCntxtInfo->enBearerCntxtState )
        {
            continue;
        }

        ulQueryRslt = NAS_ESM_FAILURE;

        /*��ѯID_EMM_ESM_BEARER_STATUS_IND��Ϣ��EPSID��Ϣ���Ƿ�����˳���ID*/
        for( ulCnt = 0; ulCnt < ulEpsbIdNum; ulCnt++ )
        {
            if( ulEpsbId == aulEpsbId[ulCnt] )
            {
                ulQueryRslt = NAS_ESM_SUCCESS;
                break;
            }
        }

        /*���������*/
        if( NAS_ESM_FAILURE == ulQueryRslt )
        {
            /*֪ͨAPP�ͷų��أ����ͷ���س��غ���Դ*/
            NAS_ESM_DeactBearerAndInformApp(ulEpsbId);

            /*�������ֹ��ulEpsbId������������Ϣ*/
            if(NAS_ESM_SUCCESS == NAS_ESM_QueryStateTblIndexByEpsbId(ulEpsbId, &ulStateTblIndex))
            {
                NAS_ESM_RelStateTblResource(ulStateTblIndex);
            }
        }
    }

    /*֪ͨEMM��ǰ����״̬��Ϣ������ID_EMM_ESM_BEARER_STATUS_REQ*/
    NAS_ESM_SndEsmEmmBearerStatusReqMsg(EMM_ESM_BEARER_CNTXT_MOD_MUTUAL);

    /*���֧��˫APN������ע����أ������д���*/
    if (NAS_ESM_OP_TRUE == NAS_ESM_AttachBearerReestEnable())
    {
        NAS_ESM_TimerStop(0, TI_NAS_ESM_ATTACH_BEARER_REEST);
        if (NAS_ESM_UNASSIGNED_EPSB_ID == NAS_ESM_GetAttachBearerId())
        {
            NAS_ESM_ReestablishAttachBearer();
        }
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_BufferedNwMsgProc
 Description     : ����ESM������Ϣ
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2009-8-19  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_BufferedNwMsgProc()
{
    NAS_ESM_ATTACHING_BUFF_ITEM_STRU   *pstDecodedNwMsg     = VOS_NULL_PTR;
    NAS_ESM_SND_NW_MSG_STRU            *pstEsmSndNwMsg      = VOS_NULL_PTR;
    VOS_UINT32                          ulCnt               = NAS_ESM_NULL;

    /*����ռ�ͼ���Ƿ����ɹ�,���ڴ��SM׼���ظ����������Ϣ*/
    pstEsmSndNwMsg = NAS_ESM_GetEsmSndNwMsgAddr();
    pstEsmSndNwMsg->pucNwMsg = (VOS_UINT8 *)NAS_ESM_MEM_ALLOC(ESM_MAX_MSG_CONTAINER_CNTNTS_LEN);

    if ( VOS_NULL_PTR == pstEsmSndNwMsg->pucNwMsg )
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_BufferedNwMsgProc:ERROR:Alloc Snd Nw Msg fail!");
        return;
    }

    for (ulCnt = NAS_ESM_NULL; ulCnt < NAS_ESM_MAX_ATTACHING_BUFF_ITEM_NUM; ulCnt++)
    {
        /* ��ȡ�����¼ */
        pstDecodedNwMsg = (NAS_ESM_ATTACHING_BUFF_ITEM_STRU*)
                                NAS_ESM_GetBuffItemAddr(NAS_ESM_BUFF_ITEM_TYPE_ATTACHING, ulCnt);

        if (pstDecodedNwMsg == VOS_NULL_PTR)
        {
            continue;
        }

        NAS_ESM_INFO_LOG1("NAS_ESM_BufferedNwMsgProc:NORM:Buffer Itmem Index:", ulCnt);

        pstEsmSndNwMsg->ulNwMsgSize = NAS_ESM_NULL;
        NAS_ESM_MEM_SET(    pstEsmSndNwMsg->pucNwMsg,
                            NAS_ESM_NULL,
                            ESM_MAX_MSG_CONTAINER_CNTNTS_LEN);

        /*����������󣬷��ؾܾ�����*/
        if( NAS_ESM_CAUSE_SUCCESS != pstDecodedNwMsg->enDecodeRslt)
        {
            NAS_ESM_ProcNwMsgForDecodeError(&pstDecodedNwMsg->stDecodedNwMsg.stEsmNwMsgIE,
                                            pstDecodedNwMsg->enDecodeRslt);

            /*��ӡ�쳣��Ϣ*/
            NAS_ESM_WARN_LOG("NAS_ESM_BufferedNwMsgProc:WARNING: NW Message Error!");
        }
        else/*�����Ϣ�����ɹ�*/
        {
            /*��������տ���Ϣ*/
            NAS_ESM_NwMsgProcess(&pstDecodedNwMsg->stDecodedNwMsg);
        }

        /* ��������¼ */
        NAS_ESM_ClearEsmBuffItem(NAS_ESM_BUFF_ITEM_TYPE_ATTACHING, ulCnt);
    }

    /*�ͷſտ���Ϣռ�õĿռ�*/
    NAS_ESM_MEM_FREE(pstEsmSndNwMsg->pucNwMsg);
    pstEsmSndNwMsg->pucNwMsg = VOS_NULL_PTR;
    pstEsmSndNwMsg->ulNwMsgSize = NAS_ESM_NULL;
}
VOS_VOID NAS_ESM_RcvEsmEmmEstCnf(const EMM_ESM_EST_CNF_STRU *pRcvMsg )
{
    EMM_ESM_ATTACH_RESULT_ENUM_UINT32   ulEstRst         = NAS_ESM_NULL;
    NAS_ESM_STATE_INFO_STRU             *pstStateTblInfo = NAS_ESM_NULL;
    VOS_UINT32                          ulStateTblIndex  = 0;

    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG2("NAS_ESM_RcvEsmEmmEstCnf is entered.", NAS_ESM_GetLModeStatus(), pRcvMsg->ulEstRst);

    if (NAS_ESM_L_MODE_STATUS_NORMAL != NAS_ESM_GetLModeStatus())
    {
        NAS_ESM_WARN_LOG("NAS_ESM_RcvEsmEmmEstCnf: L mode is already suspended!");
        return ;
    }

    /*ȡ����Ϣ����*/
    ulEstRst = ((EMM_ESM_EST_CNF_STRU *)pRcvMsg)->ulEstRst;

    /*�жϵ�ǰEMMע��״̬�������Attaching��������Ϣ���ݣ�����ע��״̬��������*/
    if( NAS_ESM_PS_REGISTER_STATUS_ATTACHING == NAS_ESM_GetEmmStatus())
    {
        /*ע��ɹ�*/
        if( EMM_ESM_ATTACH_RESULT_SUCC == ulEstRst )
        {
            /*����ע��״̬*/
            NAS_ESM_SetEmmStatus(NAS_ESM_PS_REGISTER_STATUS_ATTACHED);

            /* ����ESM������Ϣ */
            NAS_ESM_BufferedNwMsgProc();
        }
        else
        {
            /*EMMע��ʧ�ܻ�ȥע�ᣬ�ͷ���Դ*/
            NAS_ESM_RcvEmmDetachMsg();
        }
    }
    else if (NAS_ESM_PS_REGISTER_STATUS_ATTACHED == NAS_ESM_GetEmmStatus())
    {
        /* �������PDN���ӽ��������EMM StatusInd�Գ�ʱ����ֵ��˷�֧��
         * �Գ�ʱEMM��Խ���PDN��EstReq������ʧ�ܣ�ESM������������:
         *      1. ���ı�ESM��¼��Emmע��״̬
         *      2. ������Ľ���PDN���ӽ�����Ϣʹ��DataReq�����ط���������T3482
         */
        if ((EMM_ESM_ATTACH_RESULT_FAIL == ulEstRst) &&
            (NAS_ESM_SUCCESS == NAS_ESM_QueryStateTblIndexForEmc(&ulStateTblIndex)))
        {
            pstStateTblInfo = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

            NAS_ESM_LOG1("NAS_ESM_RcvEsmEmmEstCnf", pstStateTblInfo->ulCid);

            NAS_ESM_SndEsmEmmDataReqMsg(pstStateTblInfo->stNwMsgRecord.ulOpId,
                                        pstStateTblInfo->stNwMsgRecord.ucIsEmcPdnReq,
                                        pstStateTblInfo->stNwMsgRecord.ulMsgLength,
                                        pstStateTblInfo->stNwMsgRecord.aucMsgBuff);

            /*����T3482��ʱ��*/
            NAS_ESM_TimerStart(ulStateTblIndex, TI_NAS_ESM_T3482);

            /*״̬ת��*/
            /*֮ǰ�Ѿ����ģ����ﲻ������״̬*/
        }
    }
    else
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_WARN_LOG("NAS_ESM_RcvEsmEmmEstCnf:Register Status is not attachingl!");
    }
}


VOS_VOID NAS_ESM_RcvEsmEmmStatusInd(const EMM_ESM_STATUS_IND_STRU *pRcvMsg )
{
    EMM_ESM_ATTACH_STATUS_ENUM_UINT32   ulEstRst;

    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG("NAS_ESM_RcvEsmEmmStatusInd is entered.");

    /*ȡ����Ϣ����*/
    ulEstRst = ((EMM_ESM_STATUS_IND_STRU *)pRcvMsg)->enEMMStatus;

    /*�����Ϣ����ΪATTACHED����¼ע��״̬*/
    if( EMM_ESM_ATTACH_STATUS_ATTACHED == ulEstRst )
    {
        /*����ע��״̬*/
        NAS_ESM_SetEmmStatus(NAS_ESM_PS_REGISTER_STATUS_ATTACHED);

        /* ����ESM������Ϣ */
        NAS_ESM_BufferedNwMsgProc();
    }
    /*��Ϣ����ΪDETACHED���ͷ���Դ*/
    else if(EMM_ESM_ATTACH_STATUS_DETACHED == ulEstRst)
    {
        /*����ӳ��ͷŷǽ������ض�ʱ�����ڣ���ֹͣ*/
        NAS_ESM_TimerStop(0, TI_NAS_ESM_REL_NON_EMC_BEARER);

        /*EMMע��ʧ�ܻ�ȥע�ᣬ�ͷ���Դ*/
        NAS_ESM_RcvEmmDetachMsg();
    }
    else if (EMM_ESM_ATTACH_STATUS_EMC_ATTACHING == ulEstRst)
    {
        /* �ɽ���PDN���ӵ���EMM��TAU/Service/Attachʧ�ܣ��Һ�����Ҫ���н���ע�ᣬ
         * ESM�������ע���������Դ
         */
        NAS_ESM_ClearEsmResoureWhenEstingEmcPdn();
    }
    else
    {
        /*bsun ��鶯̬����������*/
        NAS_ESM_NORM_LOG("NAS_ESM_RcvEsmEmmStatusInd:NORM:UE re-entry Sevice NW!");
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_RcvEmmDetachMsg
 Description     : SM�յ�EMMע��ʧ�ܻ�ȥע����Ϣ���ظ�APP��Ϣ���ͷű�����Դ
 Input           : VOS_VOID
 Output          : None
 Return          : VOS_VOID

 History         :
    1.sunbing      2008-10-17  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_RcvEmmDetachMsg( VOS_VOID )
{
    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG("NAS_ESM_RcvEmmDetachMsg is entered.");

    /* ֪ͨRABM�����Դ */
    NAS_ESM_SndEsmRabmRelIndMsg();
#if 0

    if (IP_DHCPV4SERVER_ON == IP_DHCPV4SERVER_GET_DHCPV4SERVER_FLAG())
    {
        /* ֪ͨIP�����Դ */
        NAS_ESM_SndEsmIpRelInd();
    }
#endif


    /*����Դ*/
    NAS_ESM_ClearEsmResource();
}
/*****************************************************************************
 Function Name   : NAS_ESM_SndEsmEmmSuspendRsp
 Description     : ESMģ��ظ�ID_EMM_ESM_SUSPEND_RSP��Ϣ
 Input           : EMM_ESM_RSLT_TYPE_ENUM_UINT32       enRslt
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2011-05-02  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_SndEsmEmmSuspendRsp
(
    EMM_ESM_RSLT_TYPE_ENUM_UINT32       enRslt
)
{
    EMM_ESM_SUSPEND_RSP_STRU           *pstEsmEmmSuspendRsp  = VOS_NULL_PTR;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstEsmEmmSuspendRsp = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(EMM_ESM_SUSPEND_RSP_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pstEsmEmmSuspendRsp)
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_SndEsmEmmSuspendRsp:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    NAS_ESM_MEM_SET( NAS_ESM_GET_MSG_ENTITY(pstEsmEmmSuspendRsp), 0, NAS_ESM_GET_MSG_LENGTH(pstEsmEmmSuspendRsp));

    /*��д��Ϣͷ*/
    NAS_ESM_WRITE_EMM_MSG_HEAD(pstEsmEmmSuspendRsp, ID_EMM_ESM_SUSPEND_RSP);

    /*��д��Ӧ���*/
    pstEsmEmmSuspendRsp->enRslt= enRslt;

    /*������Ϣ���ͺ��� */
    NAS_ESM_SND_MSG(pstEsmEmmSuspendRsp);
}

/*****************************************************************************
 Function Name   : NAS_ESM_SndEsmEmmResumeRsp
 Description     : ESMģ��ظ�ID_EMM_ESM_RESUME_RSP��Ϣ
 Input           : EMM_ESM_RSLT_TYPE_ENUM_UINT32       enRslt
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2011-05-02  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_SndEsmEmmResumeRsp
(
    EMM_ESM_RSLT_TYPE_ENUM_UINT32       enRslt
)
{
    EMM_ESM_RESUME_RSP_STRU            *pstEsmEmmResumeRsp  = VOS_NULL_PTR;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstEsmEmmResumeRsp = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(EMM_ESM_SUSPEND_RSP_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pstEsmEmmResumeRsp)
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_SndEsmEmmResumeRsp:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    NAS_ESM_MEM_SET( NAS_ESM_GET_MSG_ENTITY(pstEsmEmmResumeRsp), 0, NAS_ESM_GET_MSG_LENGTH(pstEsmEmmResumeRsp));

    /*��д��Ϣͷ*/
    NAS_ESM_WRITE_EMM_MSG_HEAD(pstEsmEmmResumeRsp, ID_EMM_ESM_RESUME_RSP);

    /*��д��Ӧ���*/
    pstEsmEmmResumeRsp->enRslt= enRslt;

    /*������Ϣ���ͺ��� */
    NAS_ESM_SND_MSG(pstEsmEmmResumeRsp);
}
VOS_VOID NAS_ESM_SndEsmEmmBearerStatusReqMsg(
                    EMM_ESM_BEARER_CNTXT_MOD_ENUM_UINT32 enBearerCntxtChangeMod)
{
    VOS_UINT32                  ulCnt                       = NAS_ESM_NULL;
    VOS_UINT32                  ulEpsbIdNum                 = NAS_ESM_NULL;
    VOS_UINT32                  aulEpsbId[NAS_ESM_MAX_EPSB_NUM]  = {NAS_ESM_NULL};
    EMM_ESM_BEARER_STATUS_REQ_STRU *pSmEmmBearStatusReqMsg   = VOS_NULL_PTR;

    /*�ҳ��Ѽ�����أ�����¼��ID*/
    for( ulCnt = NAS_ESM_MIN_EPSB_ID; ulCnt <= NAS_ESM_MAX_EPSB_ID; ulCnt++ )
    {
        /*�˳����Ѽ���*/
        if( NAS_ESM_BEARER_STATE_ACTIVE == NAS_ESM_GetBearCntxtState(ulCnt) )
        {
            /*�����Ѽ������ID,׼�����͸�RABM*/
            aulEpsbId[ulEpsbIdNum++] = ulCnt;
        }
    }

    /*����ռ䲢��������Ƿ�ɹ�*/
    pSmEmmBearStatusReqMsg = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(EMM_ESM_BEARER_STATUS_REQ_STRU));

    if ( VOS_NULL_PTR == pSmEmmBearStatusReqMsg )
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_SndEsmEmmBearerStatusReqMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    NAS_ESM_MEM_SET(NAS_ESM_GET_MSG_ENTITY(pSmEmmBearStatusReqMsg), 0, \
               NAS_ESM_GET_MSG_LENGTH(pSmEmmBearStatusReqMsg));

    /*�˴��޸ĳ��������ĵķ�ʽ*/
    pSmEmmBearStatusReqMsg->enBearerCntxtMod = enBearerCntxtChangeMod;

    /*��ulEpsbIdNum��pulEpsbId����pSmEmmBearStatusReqMsg*/
    pSmEmmBearStatusReqMsg->ulEpsIdNum = ulEpsbIdNum;
    NAS_ESM_MEM_CPY((VOS_UINT8*)pSmEmmBearStatusReqMsg->aulEsmEpsId, (VOS_UINT8*)aulEpsbId,
               (pSmEmmBearStatusReqMsg->ulEpsIdNum)*(sizeof(VOS_UINT32)/sizeof(VOS_UINT8)));

    /*��д��ǰ�Ƿ��н���PDN����*/
    pSmEmmBearStatusReqMsg->ulExistEmcPdnFlag = NAS_ESM_HasExistedEmergencyPdn();

    /*��д��Ϣͷ*/
    NAS_ESM_WRITE_EMM_MSG_HEAD(pSmEmmBearStatusReqMsg,ID_EMM_ESM_BEARER_STATUS_REQ);

    /* ������Ϣ���ͺ��� */
    NAS_ESM_SND_MSG(pSmEmmBearStatusReqMsg);

}
VOS_VOID NAS_ESM_SndEsmEmmPdnConRspMsg( EMM_ESM_PDN_CON_RSLT_ENUM_UINT32        ulRst,
                                                          VOS_UINT32 ulLength, const VOS_UINT8 *pucSendMsg)
{
    VOS_UINT32                          ulTmpLength       = NAS_ESM_NULL;
    EMM_ESM_PDN_CON_RSP_STRU           *pSmEmmPdnConRsp   = VOS_NULL_PTR;

    /*���ݿտ���Ϣ�ĳ��ȷ���ռ䣬������Ϣͷ��*/
    /* lint -e831 */
    if(ulLength > ESM_MIN_SND_MSG_LEN)
    {
        ulTmpLength = ulLength - ESM_MIN_SND_MSG_LEN ;
        pSmEmmPdnConRsp = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(EMM_ESM_PDN_CON_RSP_STRU) + ulTmpLength);
    }
    /* lint +e831 */
    else/*�������С��ESM_MIN_SND_MSG_LEN������Ŀռ����ESM_MIN_SND_MSG_LEN*/
    {
        pSmEmmPdnConRsp = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(EMM_ESM_PDN_CON_RSP_STRU));
    }


    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pSmEmmPdnConRsp)
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_SndEsmEmmPdnConRspMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    NAS_ESM_MEM_SET( NAS_ESM_GET_MSG_ENTITY(pSmEmmPdnConRsp), 0, NAS_ESM_GET_MSG_LENGTH(pSmEmmPdnConRsp));

    /*��д��Ϣͷ*/
    NAS_ESM_WRITE_EMM_MSG_HEAD(pSmEmmPdnConRsp,ID_EMM_ESM_PDN_CON_RSP);

    /*��д��Ӧ���*/
    pSmEmmPdnConRsp->ulRst = ulRst;

    /*�����Ӧ���Ϊ�ɹ�����д�տ���Ϣ*/

    if( EMM_ESM_PDN_CON_RSLT_SUCC == ulRst )
    {
        /*��д�տ���Ϣͷ����*/
        pSmEmmPdnConRsp->stEsmMsg.ulEsmMsgSize = ulLength;

        /*���տ���Ϣ��ŵ�EMM_ESM_PDN_CON_RSP_STRU�ṹ��*/
        /*lint -e669*/
        NAS_ESM_MEM_CPY(pSmEmmPdnConRsp->stEsmMsg.aucEsmMsg, pucSendMsg, ulLength);/*lint !e669*/
        /*lint +e669*/
    }


    /*������Ϣ���ͺ��� */
    NAS_ESM_SND_MSG(pSmEmmPdnConRsp);

}

/*****************************************************************************
 Function Name   : NAS_ESM_SndEsmEmmEstReqMsg
 Description     : �������пտ���Ϣ������MM����ע��
 Input           : VOS_UINT32 ulLength
                   VOS_UINT8 *pucSendMsg
 Output          : None
 Return          : VOS_VOID

 History         :
    1.sunbing49683      2009-2-28  Draft Enact

*****************************************************************************/

/*lint -e669*/
/*lint -e831*/
VOS_VOID NAS_ESM_SndEsmEmmEstReqMsg(VOS_UINT32 ulOpId,
                                      VOS_UINT8  ucIsEmcPdnType,
                                      VOS_UINT32 ulLength,
                                      const VOS_UINT8 *pucSendMsg)
{
    VOS_UINT32                          ulTmpLength         = NAS_ESM_NULL;
    EMM_ESM_EST_REQ_STRU               *pSmEmmEstReq        = VOS_NULL_PTR;

    /*���ݿտ���Ϣ�ĳ��ȷ���ռ�*/
    if( NAS_ESM_NULL == ulLength)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_SndEsmEmmEstReqMsg: WARNING: Msg Length is zero");
        return ;
    }
    if(ulLength > ESM_MIN_SND_MSG_LEN)
    {
        ulTmpLength = ulLength - ESM_MIN_SND_MSG_LEN ;
        pSmEmmEstReq = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(EMM_ESM_EST_REQ_STRU) + ulTmpLength);
    }
    else/*�������С��ESM_MIN_SND_MSG_LEN������Ŀռ����ESM_MIN_SND_MSG_LEN*/
    {
        pSmEmmEstReq = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(EMM_ESM_EST_REQ_STRU));
    }

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pSmEmmEstReq)
    {
         /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_SndEsmEmmEstReqMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    NAS_ESM_MEM_SET( NAS_ESM_GET_MSG_ENTITY(pSmEmmEstReq), 0, NAS_ESM_GET_MSG_LENGTH(pSmEmmEstReq));

    /*��д�տ���Ϣͷ����*/
    pSmEmmEstReq->stEsmMsg.ulEsmMsgSize    = ulLength;

    /*���տ���Ϣ��ŵ�EMM_ESM_EST_REQ_STRU�ṹ��*/
    NAS_ESM_MEM_CPY(pSmEmmEstReq->stEsmMsg.aucEsmMsg, pucSendMsg, ulLength);

    /*��д�Ƿ��ǽ���PDN������������*/
    pSmEmmEstReq->ulIsEmcType = ucIsEmcPdnType;

    /*��дOpId*/
    pSmEmmEstReq->ulOpId = ulOpId;

    /*��д��Ϣͷ*/
    NAS_ESM_WRITE_EMM_MSG_HEAD(pSmEmmEstReq,ID_EMM_ESM_EST_REQ);

    /*������Ϣ���ͺ��� */
    NAS_ESM_SND_MSG(pSmEmmEstReq);

}
/*lint +e831*/
/*lint +e669*/


/*****************************************************************************
 Function Name   : NAS_ESM_SndEsmEmmPdnConSuccReqMsg
 Description     : ATTACH�����У�ESM����ȱʡ���سɹ���
                   ��EMM����EMM_ESM_PDN_CON_SUCC_REQ_STRU��Ϣ��
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2009-8-18  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ESM_SndEsmEmmPdnConSuccReqMsg
(
     VOS_UINT32                         ulLength,
     const VOS_UINT8                   *pucSendMsg
)
{
    VOS_UINT32                          ulTmpLength         = NAS_ESM_NULL;
    EMM_ESM_PDN_CON_SUCC_REQ_STRU      *pSmEmmPdnConSucc    = VOS_NULL_PTR;

    /*���ݿտ���Ϣ�ĳ��ȷ���ռ�*/
    if( NAS_ESM_NULL == ulLength)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_SndEsmEmmPdnConSuccReqMsg: WARNING: Msg Length is zero");
        return ;
    }
    else if(ulLength > ESM_MIN_SND_MSG_LEN)
    {
        ulTmpLength = ulLength - ESM_MIN_SND_MSG_LEN ;
        pSmEmmPdnConSucc = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(EMM_ESM_PDN_CON_SUCC_REQ_STRU) + ulTmpLength);
    }
    else/*�������С��ESM_MIN_SND_MSG_LEN������Ŀռ����ESM_MIN_SND_MSG_LEN*/
    {
        pSmEmmPdnConSucc = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(EMM_ESM_PDN_CON_SUCC_REQ_STRU));
    }

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pSmEmmPdnConSucc)
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_SndEsmEmmPdnConSuccReqMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    NAS_ESM_MEM_SET( NAS_ESM_GET_MSG_ENTITY(pSmEmmPdnConSucc), 0, NAS_ESM_GET_MSG_LENGTH(pSmEmmPdnConSucc));

    /*��д�տ���Ϣͷ����*/
    pSmEmmPdnConSucc->stEsmMsg.ulEsmMsgSize = ulLength;

    /*���տ���Ϣ��ŵ�EMM_ESM_DATA_REQ_STRU�ṹ��*/
    /*lint -e669 */
    NAS_ESM_MEM_CPY(pSmEmmPdnConSucc->stEsmMsg.aucEsmMsg, pucSendMsg, ulLength);/*lint !e669 !e670*/
    /*lint +e669*/

    /*��д��Ϣͷ*/
    NAS_ESM_WRITE_EMM_MSG_HEAD(pSmEmmPdnConSucc,ID_EMM_ESM_PDN_CON_SUCC_REQ);

    /*������Ϣ���ͺ��� */
    NAS_ESM_SND_MSG(pSmEmmPdnConSucc);

}


/*****************************************************************************
 Function Name   : NAS_ESM_SndEsmEmmDataReqMsg
 Description     : ��MM����DATA_REQ��Ϣ��Я�����пտ���Ϣ
 Input           : VOS_UINT32 ulLength
                   VOS_UINT8 *pucSendMsg
 Output          : None
 Return          : VOS_VOID

 History         :
    1.sunbing49683      2009-2-28  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ESM_SndEsmEmmDataReqMsg( VOS_UINT32 ulOpId,
                                       VOS_UINT8 ulIsEmcPdnType,
                                       VOS_UINT32 ulLength,
                                       const VOS_UINT8 *pucSendMsg)
{
    VOS_UINT32                          ulTmpLength         = NAS_ESM_NULL;
    EMM_ESM_DATA_REQ_STRU              *pSmEmmDataReq       = VOS_NULL_PTR;

    /*���ݿտ���Ϣ�ĳ��ȷ���ռ�*/
    if( NAS_ESM_NULL == ulLength)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_SndEsmEmmDataReqMsg: WARNING: Msg Length is zero");
        return ;
    }
    else if(ulLength > ESM_MIN_SND_MSG_LEN)
    {
        ulTmpLength = ulLength - ESM_MIN_SND_MSG_LEN ;
        pSmEmmDataReq = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(EMM_ESM_DATA_REQ_STRU) + ulTmpLength);
    }
    else/*�������С��ESM_MIN_SND_MSG_LEN������Ŀռ����ESM_MIN_SND_MSG_LEN*/
    {
        pSmEmmDataReq = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(EMM_ESM_DATA_REQ_STRU));
    }

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pSmEmmDataReq)
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_SndEsmEmmDataReqMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    NAS_ESM_MEM_SET( NAS_ESM_GET_MSG_ENTITY(pSmEmmDataReq), 0, NAS_ESM_GET_MSG_LENGTH(pSmEmmDataReq));

    /*��д�տ���Ϣͷ����*/
    pSmEmmDataReq->stEsmMsg.ulEsmMsgSize = ulLength;

    /*���տ���Ϣ��ŵ�EMM_ESM_DATA_REQ_STRU�ṹ��*/
    /*lint -e669*/
    NAS_ESM_MEM_CPY(pSmEmmDataReq->stEsmMsg.aucEsmMsg, pucSendMsg, ulLength);/*lint !e669 !e670*/
    /*lint +e669*/

    /*��д�Ƿ��ǽ���PDN������������*/
    pSmEmmDataReq->ulIsEmcType = ulIsEmcPdnType;

    /*��дOpId*/
    pSmEmmDataReq->ulOpId = ulOpId;

    /*��д��Ϣͷ*/
    NAS_ESM_WRITE_EMM_MSG_HEAD(pSmEmmDataReq,ID_EMM_ESM_DATA_REQ);

    /*������Ϣ���ͺ��� */
    NAS_ESM_SND_MSG(pSmEmmDataReq);

}

#if 0

VOS_VOID NAS_ESM_SndEsmEmmDataReqMsgMultiNwMsg( VOS_VOID )
{
    VOS_UINT32                          ulNwMsgLength   = NAS_ESM_NULL;
    VOS_UINT32                          ulTmpLength     = NAS_ESM_NULL;
    EMM_ESM_DATA_REQ_STRU              *pSmEmmDataReq   = VOS_NULL_PTR;
    NAS_ESM_SND_NW_MSG_STRU            *pstEsmSndNwMsg  = VOS_NULL_PTR;

    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG("NAS_ESM_SndEsmEmmDataReqMsgMultiNwMsg is entered.");

    /*ȡ�ÿտ���Ϣ��ŵ�ַ���ܳ���*/
    pstEsmSndNwMsg = NAS_ESM_GetEsmSndNwMsgAddr();
    ulNwMsgLength = pstEsmSndNwMsg->ulNwMsgSize;

    /*���û��Ҫ���͵Ŀտ���Ϣ��ֱ�ӷ���*/
    if( NAS_ESM_NULL == ulNwMsgLength )
    {
        /*��ӡ��Ϣ*/
        NAS_ESM_NORM_LOG("NAS_ESM_SndEsmEmmDataReqMsgMultiNwMsg:NORM:No Msg!");
        return ;
    }

    /*���ݿտ���Ϣ�ĳ��ȷ���ռ䣬������Ϣͷ��*/
    if( ulNwMsgLength > ESM_MIN_SND_MSG_LEN)
    {
        ulTmpLength = ulNwMsgLength - ESM_MIN_SND_MSG_LEN ;
        pSmEmmDataReq = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(EMM_ESM_DATA_REQ_STRU) + ulTmpLength);
    }
    else/*�������С��ESM_MIN_SND_MSG_LEN������Ŀռ����ESM_MIN_SND_MSG_LEN*/
    {
        pSmEmmDataReq = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(EMM_ESM_DATA_REQ_STRU));
    }

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pSmEmmDataReq)
    {
         /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_SndEsmEmmDataReqMsgMultiNwMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    NAS_ESM_MEM_SET( NAS_ESM_GET_MSG_ENTITY(pSmEmmDataReq), 0, NAS_ESM_GET_MSG_LENGTH(pSmEmmDataReq));

    /*��д�տ���Ϣͷ����*/
    pSmEmmDataReq->stEsmMsg.ulEsmMsgSize = NAS_ESM_GetEsmSndNwMsgLen();

    /*���տ���Ϣ��ŵ�EMM_ESM_DATA_REQ_STRU�ṹ��*/
    /*lint -e669*/
    NAS_ESM_MEM_CPY(pSmEmmDataReq->stEsmMsg.aucEsmMsg, pstEsmSndNwMsg->pucNwMsg, ulNwMsgLength);
    /*lint +e669*/

    /*��д��Ϣͷ*/
    NAS_ESM_WRITE_EMM_MSG_HEAD(pSmEmmDataReq,ID_EMM_ESM_DATA_REQ);

    /*������Ϣ���ͺ��� */
    NAS_ESM_SND_MSG(pSmEmmDataReq);

}
#endif

/*leili modify for isr begin*/

VOS_VOID NAS_ESM_SndEsmEmmBearerModifyReq( VOS_UINT32 ulEpsId)
{
    EMM_ESM_BEARER_MODIFY_REQ_STRU  *pSmEmmBearModReq   = VOS_NULL_PTR;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pSmEmmBearModReq = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(EMM_ESM_BEARER_MODIFY_REQ_STRU));

    if ( VOS_NULL_PTR == pSmEmmBearModReq )
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_SndEsmEmmRelReqMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    NAS_ESM_MEM_SET(NAS_ESM_GET_MSG_ENTITY(pSmEmmBearModReq), 0, NAS_ESM_GET_MSG_LENGTH(pSmEmmBearModReq));

    /*��д��Ϣͷ*/
    NAS_ESM_WRITE_EMM_MSG_HEAD(pSmEmmBearModReq,ID_EMM_ESM_BEARER_MODIFY_REQ);

    pSmEmmBearModReq->ulEpsId = ulEpsId;

    /*������Ϣ���ͺ��� */
    NAS_ESM_SND_MSG(pSmEmmBearModReq);

}
/*leili modify for isr end*/


VOS_VOID NAS_ESM_SndEsmEmmRelReqMsg( VOS_VOID )
{
    EMM_ESM_REL_REQ_STRU  *pSmEmmRelReq   = VOS_NULL_PTR;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pSmEmmRelReq = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(EMM_ESM_REL_REQ_STRU));

    if ( VOS_NULL_PTR == pSmEmmRelReq )
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_SndEsmEmmRelReqMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    NAS_ESM_MEM_SET(NAS_ESM_GET_MSG_ENTITY(pSmEmmRelReq), 0, NAS_ESM_GET_MSG_LENGTH(pSmEmmRelReq));

    /*��д��Ϣͷ*/
    NAS_ESM_WRITE_EMM_MSG_HEAD(pSmEmmRelReq,ID_EMM_ESM_REL_REQ);

    /*������Ϣ���ͺ��� */
    NAS_ESM_SND_MSG(pSmEmmRelReq);

}
VOS_VOID  NAS_ESM_ClearStateTable( VOS_UINT32 ulAppErrType, VOS_UINT32 ulKeepEmc )
{
    APP_ESM_MSG_TYPE_ENUM_UINT32        enAppMsgType        = NAS_ESM_NULL;
    VOS_UINT32                          ulStateTblId        = NAS_ESM_NULL;
    NAS_ESM_STATE_INFO_STRU            *pstStateAddr        = VOS_NULL_PTR;

    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG2("NAS_ESM_ClearStateTable is entered.", ulAppErrType, ulKeepEmc);

    /*����״̬�������APP��Ϣ����ظ�APP*/
    for( ulStateTblId = 0; ulStateTblId < NAS_ESM_MAX_STATETBL_NUM; ulStateTblId++ )
    {
        pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblId);

        /*����������������ض�Ӧ�Ķ�̬�������������*/
        if ((NAS_ESM_BEARER_TYPE_EMERGENCY == pstStateAddr->enBearerCntxtType) &&
            (PS_TRUE == ulKeepEmc))
        {
			/* lihong00150010 emergency tau&service begin */
            NAS_ESM_INFO_LOG("NAS_ESM_ClearStateTable:stop 3482");

            /* ����ǽ���PDN����TAU����SERVICE��ʧ�ܺ���Ҫ�������ע��ĳ�����
               ��Ҫֹͣ3482��ʱ�����������ע�������3482���ܳ�ʱ */
            NAS_ESM_TimerStop(ulStateTblId, TI_NAS_ESM_T3482);
			/* lihong00150010 emergency tau&service ends */
            continue;
        }

        /* �����״̬�����APP��Ϣ��¼�����ݼ�¼�ظ���Ϣ*/
        if( NAS_ESM_OP_TRUE == pstStateAddr->bitOpAppMsgRecord )
        {
            /*ȡ���˳����м�¼��APP��Ϣ����*/
            enAppMsgType = pstStateAddr->stAppMsgRecord.enAppMsgType;

            /*��������¼����Ϣ���ͻظ���Ϣ*/
            switch( enAppMsgType )
            {
                /*�����ID_APP_ESM_PDP_SETUP_REQ*/

                case ID_APP_ESM_PDP_SETUP_REQ:
                    /*��APP����ID_APP_ESM_PDP_SETUP_CNF(ʧ��)��Ϣ*/

                    NAS_ESM_SndEsmAppSdfSetupCnfFailMsg(    ulStateTblId, \
                                                            ulAppErrType);
                    break;

                case ID_APP_ESM_PDP_RELEASE_REQ:
                    if (APP_ERR_SM_SUSPENDED == ulAppErrType)
                    {
                        /*��APP����ID_APP_ESM_PDP_RELEASE_CNF(ʧ��)��Ϣ*/
                        NAS_ESM_SndEsmAppSdfRelCnfFailMsg(ulStateTblId, ulAppErrType);
                    }
                    else
                    {
                        /*��APP����ID_APP_ESM_PDP_RELEASE_CNF(�ɹ�)��Ϣ*/
                        NAS_ESM_SndEsmAppSdfRelCnfSuccMsg(ulStateTblId);
                    }
                    break;

                case ID_APP_ESM_PDP_MODIFY_REQ:
                    /*��APP����ID_APP_ESM_PDP_MODIFY_CNF(ʧ��)��Ϣ*/
                    NAS_ESM_SndEsmAppSdfModCnfFailMsg(  ulStateTblId, \
                                                        ulAppErrType);
                    break;

                case ID_APP_ESM_NDISCONN_REQ:
                    /*��APP����ID_APP_ESM_NDISCONN_CNF(ʧ��)��Ϣ*/
                    NAS_ESM_LOG1("NAS_ESM_ClearStateTable:ERROR =", ulAppErrType);
                    NAS_ESM_SndEsmAppNdisConnCnfFailMsg(    ulStateTblId, \
                                                            ulAppErrType);
                    break;

                default:
                    break;
            }
        }

        NAS_ESM_RelStateTblResource(ulStateTblId);
    }

}


EMM_ESM_UP_SINGNAL_PENDING_ENUM_UINT32 NAS_ESM_IsUpSingnalPending( VOS_VOID )
{
    VOS_UINT32                          ulStateTblIndex = NAS_ESM_NULL;
    VOS_UINT32                          ulTimerMaxExpTimer = NAS_ESM_NULL;
    NAS_ESM_TIMER_STRU                 *pstTimerInfo = VOS_NULL_PTR;

    NAS_ESM_INFO_LOG("NAS_ESM_IsUpSingnalPending is entered.");

    /* �ж��Ƿ��������ڵȴ�������Ӧ*/
    for (ulStateTblIndex = 0; ulStateTblIndex < NAS_ESM_MAX_STATETBL_NUM; ulStateTblIndex++)
    {
        if (NAS_ESM_BEARER_PROC_TRANS_STATE_PENDING
                == NAS_ESM_GetProcTransState(ulStateTblIndex))
        {
            /* �жϸ����̶�Ӧ��ʱ�ĳ�ʱ����*/
            pstTimerInfo = NAS_ESM_GetStateTblTimer(ulStateTblIndex);

            /* �ж����̶�Ӧ��ʱ��û�����У���ʾ����*/
            if(VOS_NULL_PTR == pstTimerInfo->hTimer)
            {
                NAS_ESM_WARN_LOG("NAS_ESM_IsUpSingnalPending:warn: Timer not running.");
                continue;
            }

            /* ��ȡ���̶�Ӧ��ʱ�������ʱ����*/
            ulTimerMaxExpTimer = NAS_ESM_GetTimerMaxExpNum(pstTimerInfo->enPara);

            /* �����ʱ����С����������1�����ش���������������*/
            if((pstTimerInfo->ucExpireTimes) < (ulTimerMaxExpTimer-1))
            {
                NAS_ESM_INFO_LOG("NAS_ESM_IsUpSingnalPending:Uplink singnal pending.");
                return EMM_ESM_UP_SINGNAL_PENDING;
            }
            else
            {
                /*��ʱ����ʱ�����Ѿ�������������1������Ϊ������������*/
                NAS_ESM_INFO_LOG("NAS_ESM_IsUpSingnalPending:ExpireTimex >= Max-1");
                continue;
            }
        }
    }

    NAS_ESM_INFO_LOG("NAS_ESM_IsUpSingnalPending:no uplink signal pending.");
    return EMM_ESM_NO_UP_SINGNAL_PENDING;
}

VOS_UINT32 NAS_ESM_GetRegistCid( VOS_VOID )
{
    VOS_UINT32                          ulCur = NAS_ESM_ILL_CID;
    NAS_ESM_SDF_PARA_STRU              *pstSdfPara  = VOS_NULL_PTR;

    /* �����ж�CID0�Ƿ�Ϊȱʡ���� */
    pstSdfPara = NAS_ESM_GetSdfParaAddr(NAS_ESM_NULL);
    if (NAS_ESM_BEARER_TYPE_DEFAULT == pstSdfPara->enBearerCntxtType)
    {
       return NAS_ESM_NULL;
    }

    /* ��CID11-CID1Ѱ��ȱʡ�������͵�CID */
    for (ulCur = 1; ulCur < NAS_ESM_CID_NV_NUM; ulCur++ )
    {
       pstSdfPara = NAS_ESM_GetSdfParaAddr(ulCur);

       if (NAS_ESM_BEARER_TYPE_DEFAULT == pstSdfPara->enBearerCntxtType)
       {
           return ulCur;
       }
    }

    /* ���֮ǰ������CID�����������������CID12-CID20Ѱ��ȱʡ�������͵�CID */
    for (ulCur = NAS_ESM_CID_NV_NUM; ulCur < NAS_ESM_MAX_UE_CONTRUL_CID_NUM; ulCur++ )
    {
       pstSdfPara = NAS_ESM_GetSdfParaAddr(ulCur);

       if (NAS_ESM_BEARER_TYPE_DEFAULT == pstSdfPara->enBearerCntxtType)
       {
           return ulCur;
       }
    }

    return NAS_ESM_ILL_CID;
}
VOS_UINT32 NAS_ESM_ValidCidAttachBearerReest(VOS_UINT32 ulCid)
{
    VOS_UINT32 ulEpsbId = 0;
    VOS_UINT32 ulStateTblId = 0;
    VOS_UINT32 ulResult = PS_FALSE;

    if (NAS_ESM_FAILURE == NAS_ESM_QueryEpsbCntxtTblByCid(ulCid, &ulEpsbId))
    {
        if (NAS_ESM_FAILURE == NAS_ESM_QueryStateTblIndexByCid(ulCid, &ulStateTblId))
        {
            ulResult = PS_TRUE;
        }
    }
    else
    {
        if (NAS_ESM_BEARER_STATE_INACTIVE == NAS_ESM_GetBearCntxtState(ulEpsbId))
        {
            ulResult = PS_TRUE;
        }
    }

    return ulResult;
}


VOS_VOID NAS_ESM_ReestablishAttachBearer( VOS_VOID )
{
     VOS_UINT32                          ulPti               = NAS_ESM_NULL;
     VOS_UINT32                          ulRegCId            = NAS_ESM_NULL;
     VOS_UINT32                          ulStateTblId        = NAS_ESM_NULL;
     VOS_UINT32                          ulCur               = NAS_ESM_NULL;
     VOS_UINT32                          ulSdfNum            = NAS_ESM_NULL;
     NAS_ESM_SDF_PARA_STRU              *pstSdfPara          = VOS_NULL_PTR;
     APP_ESM_SDF_PARA_STRU              *pstAppSdfPara       = VOS_NULL_PTR;
     NAS_ESM_STATE_INFO_STRU            *pstStateAddr        = VOS_NULL_PTR;
     NAS_ESM_ENCODE_INFO_STRU            stEncodeInfo;
     NAS_ESM_PDP_MANAGE_INFO_STRU       *pEpsbManageInfo     = VOS_NULL_PTR;


    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG("NAS_ESM_ReestablishAttachBearer is entered.");

    if (NAS_ESM_L_MODE_STATUS_NORMAL != NAS_ESM_GetLModeStatus())
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ReestablishAttachBearer: L mode is already suspended!");
        return;
    }

    if (NAS_ESM_PS_REGISTER_STATUS_ATTACHED != NAS_ESM_GetEmmStatus())
    {
        NAS_ESM_NORM_LOG("NAS_ESM_ReestablishAttachBearer: ESM status is not ATTACHED !");
        return;
    }

    pstAppSdfPara = NAS_ESM_MEM_ALLOC(sizeof(APP_ESM_SDF_PARA_STRU));

    if (VOS_NULL_PTR == pstAppSdfPara)
    {
         /*��ӡ���Ϸ���Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_ReestablishAttachBearer:ERROR: Mem alloc fail!");
        return;
    }

    /* ��һ��ȡ12��NV���� */
    for (ulCur = NAS_ESM_MIN_CID; ulCur < NAS_ESM_CID_NV_NUM; ulCur++)
    {
        pstAppSdfPara->ulCId = ulCur;
        /*PC REPLAY MODIFY BY LEILI BEGIN*/
        if (APP_FAILURE == NAS_ESM_GetSdfPara(&ulSdfNum, pstAppSdfPara))
        {
            continue;
        }

        pstSdfPara = NAS_ESM_GetSdfParaAddr(ulCur);
        NAS_ESM_MEM_CPY(pstSdfPara, pstAppSdfPara, sizeof(APP_ESM_SDF_PARA_STRU));
    }

    NAS_ESM_MEM_FREE(pstAppSdfPara);

    /* ��APP��ȡ���ع���ģʽ���������ȡ�Զ�����ģʽ */
    pEpsbManageInfo = NAS_ESM_GetBearerManageInfoAddr();
    if (APP_FAILURE == NAS_ESM_GetPdpManageInfo(pEpsbManageInfo))
    {
        /*��ӡ������Ϣ*/
        NAS_ESM_WARN_LOG("NAS_ESM_ReestablishAttachBearer:WARN: Fail to get PDP Manage Info from APP!");
        pEpsbManageInfo->enMode = APP_ESM_BEARER_MANAGE_MODE_AUTO;
        pEpsbManageInfo->enType = APP_ESM_BEARER_MANAGE_TYPE_ACCEPT;
    }
    /*PC REPLAY MODIFY BY LEILI END*/
    /* ��ȡ����ע���CID�����ȼ�˳���ɸߵ���ΪCID0 > CID1-CID11 > CID12-CID20 */
    /* ���˸�����Ӫ����������, ��ҪGU NV��CID0��ʼ��Ϊר�����ͣ�CID11Ϊȱʡ����*/
    ulRegCId = NAS_ESM_GetRegistCid();
    if (NAS_ESM_ILL_CID == ulRegCId)
    {
        /*��ӡ���Ϸ���Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_ReestablishAttachBearer: There is no default bearer type sdf");
        return ;
    }

    NAS_ESM_LOG1("NAS_ESM_ReestablishAttachBearer:The cid of the found sdf is :", ulRegCId);

    /* ���CID�Ƿ����ע������ؽ������� */
    if (PS_FALSE == NAS_ESM_ValidCidAttachBearerReest(ulRegCId))
    {
        NAS_ESM_ERR_LOG("NAS_ESM_ReestablishAttachBearer: Invalid CID");
        return;
    }

    /*�ڶ�̬���У���ȡ���̱�ʶ*/
    if(NAS_ESM_SUCCESS != NAS_ESM_GetStateTblIndexByCid(NAS_ESM_ATTACH_CID, &ulStateTblId))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_ReestablishAttachBearer:WARNING:insufficient resource!");
        return ;
    }

    /*����ѯ����ulRegCId����������CID0�У�����ע��*/
    pstSdfPara = NAS_ESM_GetSdfParaAddr(NAS_ESM_ATTACH_CID);
    NAS_ESM_MEM_CPY(    pstSdfPara,
                        NAS_ESM_GetSdfParaAddr(ulRegCId),
                        sizeof(NAS_ESM_SDF_PARA_STRU));

    /*���ó�ʼ״̬*/
    pstStateAddr = NAS_ESM_GetStateTblAddr(ulStateTblId);
    pstStateAddr->enBearerCntxtState = NAS_ESM_BEARER_STATE_INACTIVE;
    pstStateAddr->enBearerCntxtType  = NAS_ESM_BEARER_TYPE_DEFAULT;

    /*����PTI,����洢��������Ϣ��*/
    NAS_ESM_AssignPTI(&ulPti);
    pstStateAddr->stNwMsgRecord.ucPti = (VOS_UINT8)ulPti;

    /*���NAS_ESM_ENCODE_INFO_STRU*/
    NAS_ESM_MEM_SET((VOS_VOID*)&stEncodeInfo, 0, sizeof(NAS_ESM_ENCODE_INFO_STRU));
    stEncodeInfo.bitOpESMCau        = NAS_ESM_OP_FALSE;
    stEncodeInfo.ulCid              = pstStateAddr->ulCid;
    stEncodeInfo.ucPti              = (VOS_UINT8)ulPti;
    stEncodeInfo.ulEpsbId           = pstStateAddr->ulEpsbId;
    stEncodeInfo.enPdnReqType       = pstStateAddr->enPdnReqType;

    /*��װPDN_Connect_Req��Ϣ*/
    if( NAS_ESM_FAILURE == NAS_ESM_EncodePdnConReqMsg(stEncodeInfo, \
                                                      pstStateAddr->stNwMsgRecord.aucMsgBuff,\
                                                      &pstStateAddr->stNwMsgRecord.ulMsgLength))
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_ReestablishAttachBearer:ERROR:EncodePdnConReqMsg fail!");

        NAS_ESM_RelStateTblResource(ulStateTblId);
        return ;
    }

    /*��¼�տ���Ϣ����*/
    pstStateAddr->bitOpNwMsgRecord = NAS_ESM_OP_TRUE;
    pstStateAddr->stNwMsgRecord.enEsmCnMsgType = NAS_ESMCN_MSG_TYPE_PDN_CONNECT_REQ;
    pstStateAddr->stNwMsgRecord.ucIsEmcPdnReq = PS_FALSE;
    pstStateAddr->stNwMsgRecord.ulOpId = pstStateAddr->ulCid;

    /*���ú�����SM->EMM������Ϣ:ID_EMM_ESM_DATA_REQ(NAS_ESMCN_MSG_TYPE_PDN_CONNECT_REQ)*/
    NAS_ESM_SndEsmEmmDataReqMsg(pstStateAddr->stNwMsgRecord.ulOpId,
                                pstStateAddr->stNwMsgRecord.ucIsEmcPdnReq,
                                pstStateAddr->stNwMsgRecord.ulMsgLength,
                                pstStateAddr->stNwMsgRecord.aucMsgBuff);

    /* ��¼�տ���Ϣ */
    NAS_ESM_SndAirMsgReportInd(pstStateAddr->stNwMsgRecord.aucMsgBuff,
                               pstStateAddr->stNwMsgRecord.ulMsgLength,
                               NAS_ESM_AIR_MSG_DIR_ENUM_UP,
                               ESM_PDN_CONNECT_REQ);

    /*����T3482��ʱ��*/
    NAS_ESM_TimerStart(ulStateTblId,TI_NAS_ESM_T3482);

    /*ת��״̬*/
    NAS_ESM_SetProcTransState(ulStateTblId, NAS_ESM_BEARER_PROC_TRANS_STATE_PENDING);
}
VOS_VOID NAS_ESM_RcvEsmEmmDeactAllNonEmcBearerInd(VOS_VOID)
{
    NAS_ESM_INFO_LOG("NAS_ESM_RcvEsmEmmDeactAllNonEmcBearerInd: enter");

    if (PS_TRUE == NAS_ESM_HasExistedEmergencyPdn())
    {
        NAS_ESM_INFO_LOG("NAS_ESM_RcvEsmEmmDeactAllNonEmcBearerInd: existed emergency PDN");

        /*�����ǰ���Ѿ������Ľ���PDN���ӣ��������ͷ����зǽ���PDN����*/
        NAS_ESM_DeactAllNonEmcPdn();
    }
    else if (PS_TRUE == NAS_ESM_HasEstingEmergencyPdn())
    {
        NAS_ESM_INFO_LOG("NAS_ESM_RcvEsmEmmDeactAllNonEmcBearerInd: esting emergency PDN");

        /*�����ǰ�����ڽ����Ľ���PDN���ӣ����ӳ��ͷ����зǽ���PDN����*/
        NAS_ESM_TimerStart(0, TI_NAS_ESM_REL_NON_EMC_BEARER);
    }
    else
    {   /*�����ǰû�н���PDN����������*/
        NAS_ESM_INFO_LOG("NAS_ESM_RcvEsmEmmDeactAllNonEmcBearerInd: no emergency PDN");
    }

    return;
}
VOS_VOID NAS_ESM_SndEsmEmmClrEsmProcResNtyMsg(VOS_UINT32 ulOpId, VOS_UINT8 ucIsEmcPdnType)
{
    EMM_ESM_CLR_ESM_PROC_RES_NOTIFY_STRU *pSmEmmNotify      = VOS_NULL_PTR;

    NAS_ESM_INFO_LOG2("NAS_ESM_SndEsmEmmClrEsmProcResNtyMsg: enter", ulOpId, ucIsEmcPdnType);

    /*���ݿտ���Ϣ�ĳ��ȷ���ռ�*/
    pSmEmmNotify= (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(EMM_ESM_CLR_ESM_PROC_RES_NOTIFY_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pSmEmmNotify)
    {
         /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_SndEsmEmmClrEsmProcResNtyMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*���*/
    NAS_ESM_MEM_SET( NAS_ESM_GET_MSG_ENTITY(pSmEmmNotify), 0, NAS_ESM_GET_MSG_LENGTH(pSmEmmNotify));

    /*��д�Ƿ��ǽ���PDN������������*/
    pSmEmmNotify->ulIsEmcType = ucIsEmcPdnType;

    /*��дOpId*/
    pSmEmmNotify->ulOpId = ulOpId;

    /*��д��Ϣͷ*/
    NAS_ESM_WRITE_EMM_MSG_HEAD(pSmEmmNotify, ID_EMM_ESM_CLR_ESM_PROC_RES_NOTIFY);

    /*������Ϣ���ͺ��� */
    NAS_ESM_SND_MSG(pSmEmmNotify);
}
/*lint +e961*/
/*lint +e960*/



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
/* end of NasEsmEmmMsgProc.c */
