

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include    "ImsaProcAtMsg.h"
#include    "ImsaEntity.h"
#include    "ImsaPublic.h"
#include    "ImsaServiceManagement.h"
#include    "ImsaImsAdaption.h"

/*lint -e767*/
#define    THIS_FILE_ID    PS_FILE_ID_IMSAPROCATMSG_C
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
extern VOS_VOID IMSA_SndMsgAtCiregSetCnf (VOS_UINT8 ucOpId, VOS_UINT16 usClientId,VOS_UINT32 ulRslt);
extern VOS_VOID IMSA_SndMsgAtCiregQryCnf
(
    VOS_UINT8 ucOpId,
    VOS_UINT16 usClientId,
    VOS_UINT32 ulRegInfo
);
extern VOS_VOID IMSA_SndMsgAtCirepSetCnf (VOS_UINT8 ucOpId, VOS_UINT16 usClientId, VOS_UINT32 ulRslt);
extern VOS_VOID IMSA_SndMsgAtCirepQryCnf
(
    VOS_UINT8 ucOpId,
    VOS_UINT16 usClientId,
    AT_IMSA_IMSVOPS_CAPABILITY_ENUM_UINT32      enImsvops
);


/*****************************************************************************
  3 Function
*****************************************************************************/
/*lint -e960*/
/*lint -e961*/

VOS_VOID IMSA_ProcMsgCiregSetReq (const AT_IMSA_CIREG_SET_REQ_STRU *pstCiregSetReq)
{
    IMSA_AT_CONTROL_STRU                *pstImsaAtControl;
    VOS_UINT32                          ulRslt;

    IMSA_INFO_LOG("IMSA_ProcMsgCiregSetReq is entered! ");

    if (VOS_NULL_PTR == pstCiregSetReq)
    {
        IMSA_WARN_LOG("IMSA_ProcMsgCiregSetReq: Input is null! ");
        return;
    }

    pstImsaAtControl = IMSA_GetAtControlAddress();

    if (AT_IMSA_IMS_REG_STATE_REPROT_BUTT <= pstCiregSetReq->enCireg )
    {
        ulRslt = VOS_ERR;
    }
    else
    {
        /*�����Ƿ������ϱ�����*/
        pstImsaAtControl->enCireg = pstCiregSetReq->enCireg;

        ulRslt = VOS_OK;
    }

    /* ��AT���ͻظ���Ϣ*/
    IMSA_SndMsgAtCiregSetCnf(pstCiregSetReq->ucOpId,pstCiregSetReq->usClientId,ulRslt);

    return;
}


VOS_VOID IMSA_SndMsgAtCiregSetCnf (VOS_UINT8 ucOpId, VOS_UINT16 usClientId,VOS_UINT32 ulRslt)
{

    IMSA_AT_CIREG_SET_CNF_STRU          *pstCiregSetCnf;

    /*������Ϣ�ռ�*/
    pstCiregSetCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_AT_CIREG_SET_CNF_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pstCiregSetCnf)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndMsgAtCiregSetCnf:ERROR:Alloc Msg fail!");
        return ;
    }

    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstCiregSetCnf), 0, IMSA_GET_MSG_LENGTH(pstCiregSetCnf));

    IMSA_WRITE_AT_MSG_MSG_HEAD(pstCiregSetCnf,ID_IMSA_AT_CIREG_SET_CNF);

    /*��д��Ϣ����*/
    pstCiregSetCnf->ucOpId = ucOpId;
    pstCiregSetCnf->usClientId = usClientId;
    pstCiregSetCnf->ulResult = ulRslt;

    /*������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstCiregSetCnf);

    return;
}
VOS_VOID IMSA_SndMsgAtCireguInd (VOS_UINT32 ulRegInfo)
{
    IMSA_AT_CIREGU_IND_STRU             *pstCireguInd;
    IMSA_AT_CONTROL_STRU                *pstImsaAtControl;

    pstImsaAtControl = IMSA_GetAtControlAddress();

    if (AT_IMSA_IMS_REG_STATE_DISABLE_REPORT == pstImsaAtControl->enCireg)
    {
        IMSA_INFO_LOG("IMSA_SndMsgAtCireguInd: NOT SEND CIREGU IND MSG!");
        return;
    }

    /*������Ϣ�ռ�*/
    pstCireguInd = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_AT_CIREGU_IND_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pstCireguInd)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndMsgAtCireguInd:ERROR:Alloc Msg fail!");
        return ;
    }

    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstCireguInd), 0, IMSA_GET_MSG_LENGTH(pstCireguInd));

    IMSA_WRITE_AT_MSG_MSG_HEAD(pstCireguInd,ID_IMSA_AT_CIREGU_IND);

    /*��д��Ϣ����*/
    pstCireguInd->ucOpId = 0;
    pstCireguInd->usClientId = 0x3fff;

    pstCireguInd->ulRegInfo = ulRegInfo;

    if (AT_IMSA_IMS_REG_STATE_ENABLE_EXTENDED_REPROT == pstImsaAtControl->enCireg)
    {
        pstCireguInd->bitOpExtInfo = VOS_TRUE;
        pstCireguInd->ulExtInfo = 0;
    }

    /*������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstCireguInd);
    return;
}
VOS_VOID IMSA_ProcMsgCiregQryReq (const AT_IMSA_CIREG_QRY_REQ_STRU *pstCiregQryReq)
{
    VOS_UINT32                          ulRegInfo;

    IMSA_INFO_LOG("IMSA_ProcMsgCiregQryReq is entered! ");

    if (VOS_NULL_PTR == pstCiregQryReq)
    {
        IMSA_WARN_LOG("IMSA_ProcMsgCiregQryReq: Input is null! ");
        return;
    }

    ulRegInfo = IMSA_SRV_GetAtRegStatusFromSrvStatus(IMSA_SRV_GetNormalSrvStatus());


    /* ��AT���ͻظ���Ϣ*/
    IMSA_SndMsgAtCiregQryCnf(pstCiregQryReq->ucOpId,pstCiregQryReq->usClientId,ulRegInfo);

    return;
}
VOS_VOID IMSA_SndMsgAtCiregQryCnf
(
    VOS_UINT8 ucOpId,
    VOS_UINT16 usClientId,
    VOS_UINT32 ulRegInfo
)
{
    IMSA_AT_CIREG_QRY_CNF_STRU          *pstCiregQryCnf;
    IMSA_AT_CONTROL_STRU                *pstImsaAtControl;

    pstImsaAtControl = IMSA_GetAtControlAddress();

    /*������Ϣ�ռ�*/
    pstCiregQryCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_AT_CIREG_QRY_CNF_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pstCiregQryCnf)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndMsgAtCiregQryCnf:ERROR:Alloc Msg fail!");
        return ;
    }

    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstCiregQryCnf), 0, IMSA_GET_MSG_LENGTH(pstCiregQryCnf));

    IMSA_WRITE_AT_MSG_MSG_HEAD(pstCiregQryCnf,ID_IMSA_AT_CIREG_QRY_CNF);

    /*��д��Ϣ����*/
    pstCiregQryCnf->ucOpId = ucOpId;
    pstCiregQryCnf->usClientId = usClientId;

    pstCiregQryCnf->ulRegInfo = ulRegInfo;

    if (AT_IMSA_IMS_REG_STATE_ENABLE_EXTENDED_REPROT == pstImsaAtControl->enCireg)
    {
        pstCiregQryCnf->bitOpExtInfo = VOS_TRUE;
        pstCiregQryCnf->ulExtInfo = 0;
    }
    pstCiregQryCnf->enCireg = pstImsaAtControl->enCireg;
    pstCiregQryCnf->ulResult = VOS_OK;

    /*������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstCiregQryCnf);
    return;
}
VOS_VOID IMSA_ProcMsgCirepSetReq (const AT_IMSA_CIREP_SET_REQ_STRU *pstCirepSetReq)
{
    IMSA_AT_CONTROL_STRU                *pstImsaAtControl;
    VOS_UINT32                          ulRslt;

    IMSA_WARN_LOG("IMSA_ProcMsgCirepSetReq is entered! ");

    if (VOS_NULL_PTR == pstCirepSetReq)
    {
        IMSA_WARN_LOG("IMSA_ProcMsgCirepSetReq: Input is null! ");
        return;
    }

    pstImsaAtControl = IMSA_GetAtControlAddress();

    if (AT_IMSA_IMSVOPS_CAPABILITY_BUTT <= pstCirepSetReq->enReport)
    {
        ulRslt = VOS_ERR;
    }
    else
    {
        /*�����Ƿ������ϱ�����*/
        pstImsaAtControl->enReport = pstCirepSetReq->enReport;
        ulRslt = VOS_OK;
    }

    /* ��AT���ͻظ���Ϣ*/
    IMSA_SndMsgAtCirepSetCnf(pstCirepSetReq->ucOpId,pstCirepSetReq->usClientId,ulRslt);

    return;
}


VOS_VOID IMSA_SndMsgAtCirepSetCnf (VOS_UINT8 ucOpId, VOS_UINT16 usClientId, VOS_UINT32 ulRslt)
{
    IMSA_AT_CIREP_SET_CNF_STRU          *pstCirepSetCnf;

    /*������Ϣ�ռ�*/
    pstCirepSetCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_AT_CIREP_SET_CNF_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pstCirepSetCnf)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndMsgAtCirepSetCnf:ERROR:Alloc Msg fail!");
        return ;
    }

    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstCirepSetCnf), 0, IMSA_GET_MSG_LENGTH(pstCirepSetCnf));

    IMSA_WRITE_AT_MSG_MSG_HEAD(pstCirepSetCnf,ID_IMSA_AT_CIREP_SET_CNF);

    /*��д��Ϣ����*/
    pstCirepSetCnf->ucOpId = ucOpId;
    pstCirepSetCnf->usClientId = usClientId;
    pstCirepSetCnf->ulResult = ulRslt;

    /*������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstCirepSetCnf);
    return;
}
VOS_VOID IMSA_SndMsgAtCirephInd (AT_IMSA_SRVCC_HANDVOER_ENUM_UINT32   enHandover)
{
    IMSA_AT_CIREPH_IND_STRU             *pstCirephInd;
    IMSA_AT_CONTROL_STRU                *pstImsaAtControl;

    pstImsaAtControl = IMSA_GetAtControlAddress();

    if (AT_IMSA_CIREP_REPORT_DISENABLE == pstImsaAtControl->enReport)
    {
        IMSA_INFO_LOG("IMSA_SndMsgAtCirephInd: NOT SEND CIREPH IND MSG!");
        return;
    }

    /*������Ϣ�ռ�*/
    pstCirephInd = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_AT_CIREPH_IND_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pstCirephInd)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndMsgAtCirephInd:ERROR:Alloc Msg fail!");
        return ;
    }

    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstCirephInd), 0, IMSA_GET_MSG_LENGTH(pstCirephInd));

    IMSA_WRITE_AT_MSG_MSG_HEAD(pstCirephInd,ID_IMSA_AT_CIREPH_IND);

    /*��д��Ϣ����*/
    pstCirephInd->ucOpId = 0;
    pstCirephInd->usClientId = 0x3fff;


    pstCirephInd->enHandover = enHandover;


    /*������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstCirephInd);
    return;
}
VOS_VOID IMSA_SndMsgAtCirepiInd (AT_IMSA_IMSVOPS_CAPABILITY_ENUM_UINT32      enImsvops)
{
    IMSA_AT_CIREPI_IND_STRU             *pstCirepiInd;
    IMSA_AT_CONTROL_STRU                *pstImsaAtControl;

    pstImsaAtControl = IMSA_GetAtControlAddress();

    if (AT_IMSA_CIREP_REPORT_DISENABLE == pstImsaAtControl->enReport)
    {
        IMSA_INFO_LOG("IMSA_SndMsgAtCirephInd: NOT SEND CIREPH IND MSG!");
        return;
    }

    /*������Ϣ�ռ�*/
    pstCirepiInd = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_AT_CIREPI_IND_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pstCirepiInd)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndMsgAtCirephInd:ERROR:Alloc Msg fail!");
        return ;
    }

    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstCirepiInd), 0, IMSA_GET_MSG_LENGTH(pstCirepiInd));

    IMSA_WRITE_AT_MSG_MSG_HEAD(pstCirepiInd,ID_IMSA_AT_CIREPI_IND);


    /*��д��Ϣ����*/
    pstCirepiInd->ucOpId = 0;
    pstCirepiInd->usClientId = 0x3fff;


    pstCirepiInd->enImsvops = enImsvops;


    /*������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstCirepiInd);
    return;
}
VOS_VOID IMSA_ProcMsgCirepQryReq (const AT_IMSA_CIREP_QRY_REQ_STRU *pstCiregQryReq)
{
    AT_IMSA_IMSVOPS_CAPABILITY_ENUM_UINT32      enImsvops;

    IMSA_INFO_LOG("IMSA_ProcMsgCirepQryReq is entered! ");

    if (VOS_NULL_PTR == pstCiregQryReq)
    {
        IMSA_WARN_LOG("IMSA_ProcMsgCirepQryReq: Input is null! ");
        return;
    }

    enImsvops = IMSA_GetNwImsVoiceCap();

    if (AT_IMSA_IMSVOPS_CAPABILITY_BUTT == enImsvops)
    {
        enImsvops = AT_IMSA_NW_NOT_SUPORT_IMSVOPS;
    }

    /* ��AT���ͻظ���Ϣ*/
    IMSA_SndMsgAtCirepQryCnf(pstCiregQryReq->ucOpId,pstCiregQryReq->usClientId,enImsvops);

    return;
}
VOS_VOID IMSA_ProcMsgVolteImpuQryReq(const AT_IMSA_VOLTEIMPU_QRY_REQ_STRU *pstVolteImpuQryReq)
{
    IMSA_AT_CONTROL_STRU                *pstImsaAtControl;

    pstImsaAtControl = IMSA_GetAtControlAddress();

    IMSA_INFO_LOG("IMSA_ProcMsgVolteImpuQryReq is entered! ");

    /* ��Ҫ����CLENT ID��OPID�����ڻظ�ʱʹ�� */
    pstImsaAtControl->usClientId = pstVolteImpuQryReq->usClientId;
    pstImsaAtControl->ucOpId = pstVolteImpuQryReq->ucOpId;

    /* ��IMSA���Ͳ�ѯ���� */
    IMSA_SendImsVolteImpuQryReq();
}

VOS_VOID IMSA_ProcMsgCcwaiSetReq(const AT_IMSA_CCWAI_SET_REQ_STRU *pstCcwaiSetReq)
{
    IMSA_AT_CONTROL_STRU                *pstImsaAtControl;

    pstImsaAtControl = IMSA_GetAtControlAddress();


    IMSA_INFO_LOG("IMSA_ProcMsgCcwaiSetReq is entered! ");

    /* ��Ҫ����CLENT ID��OPID�����ڻظ�ʱʹ�� */
    pstImsaAtControl->usClientId = pstCcwaiSetReq->usClientId;
    pstImsaAtControl->ucOpId = pstCcwaiSetReq->ucOpId;

    /* ��IMSA���Ͳ�ѯ���� */
    IMSA_SendImsCcwaiSetReq(pstCcwaiSetReq->enMode);

}

VOS_VOID IMSA_SndMsgAtCcwaiSetCnf
(
    VOS_UINT32 ulResult
)
{
    IMSA_AT_CCWAI_SET_CNF_STRU          *pstAtCcwaiSetCnf;

    IMSA_AT_CONTROL_STRU                    *pstImsaAtControl;

    pstImsaAtControl = IMSA_GetAtControlAddress();

    /*������Ϣ�ռ�*/
    pstAtCcwaiSetCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_AT_CCWAI_SET_CNF_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pstAtCcwaiSetCnf)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndMsgAtCcwaiSetCnf:ERROR:Alloc Msg fail!");
        return ;
    }

    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstAtCcwaiSetCnf), 0, IMSA_GET_MSG_LENGTH(pstAtCcwaiSetCnf));

    IMSA_WRITE_AT_MSG_MSG_HEAD(pstAtCcwaiSetCnf,ID_IMSA_AT_CCWAI_SET_CNF);

    /*��д��Ϣ����*/
    pstAtCcwaiSetCnf->ucOpId = pstImsaAtControl->ucOpId;
    pstAtCcwaiSetCnf->usClientId = pstImsaAtControl->usClientId;

    pstAtCcwaiSetCnf->ulResult = ulResult;

    /*������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstAtCcwaiSetCnf);

    return;
}
/*lint -e818*/
/*lint -e830*/
VOS_VOID IMSA_SndMsgAtQryVolteImpuCnf
(
    VOS_UINT32 ulResult,
    VOS_UINT32 ulImpuLen,
    VOS_CHAR *pucImpu
)
{
    IMSA_AT_VOLTEIMPU_QRY_CNF_STRU          *pstVolteImpuQryCnf;

    IMSA_AT_CONTROL_STRU                    *pstImsaAtControl;

    pstImsaAtControl = IMSA_GetAtControlAddress();

    /*������Ϣ�ռ�*/
    pstVolteImpuQryCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_AT_VOLTEIMPU_QRY_CNF_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pstVolteImpuQryCnf)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_ProcImsMsgQryVolteImpuCnf:ERROR:Alloc Msg fail!");
        return ;
    }

    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstVolteImpuQryCnf), 0, IMSA_GET_MSG_LENGTH(pstVolteImpuQryCnf));

    IMSA_WRITE_AT_MSG_MSG_HEAD(pstVolteImpuQryCnf,ID_IMSA_AT_VOLTEIMPU_QRY_CNF);

    /*��д��Ϣ����*/
    pstVolteImpuQryCnf->ucOpId = pstImsaAtControl->ucOpId;
    pstVolteImpuQryCnf->usClientId = pstImsaAtControl->usClientId;

    pstVolteImpuQryCnf->ulResult = ulResult;

    pstVolteImpuQryCnf->ulImpuLen = ulImpuLen;

    if (VOS_NULL_PTR != pucImpu)
    {
        IMSA_UtilStrNCpy(pstVolteImpuQryCnf->aucImpu, pucImpu, AT_IMSA_IMPU_MAX_LENGTH);
    }

    /*������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstVolteImpuQryCnf);

    return;
}
/*lint +e830*/
/*lint +e818*/

VOS_VOID IMSA_SndMsgAtCirepQryCnf
(
    VOS_UINT8 ucOpId,
    VOS_UINT16 usClientId,
    AT_IMSA_IMSVOPS_CAPABILITY_ENUM_UINT32      enImsvops
)
{
    IMSA_AT_CIREP_QRY_CNF_STRU          *pstCirepQryCnf;
    IMSA_AT_CONTROL_STRU                *pstImsaAtControl;

    pstImsaAtControl = IMSA_GetAtControlAddress();

    /*������Ϣ�ռ�*/
    pstCirepQryCnf = (VOS_VOID*)IMSA_ALLOC_MSG(sizeof(IMSA_AT_CIREP_QRY_CNF_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pstCirepQryCnf)
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndMsgAtCirepQryCnf:ERROR:Alloc Msg fail!");
        return ;
    }

    IMSA_MEM_SET( IMSA_GET_MSG_ENTITY(pstCirepQryCnf), 0, IMSA_GET_MSG_LENGTH(pstCirepQryCnf));

    IMSA_WRITE_AT_MSG_MSG_HEAD(pstCirepQryCnf,ID_IMSA_AT_CIREP_QRY_CNF);


    /*��д��Ϣ����*/
    pstCirepQryCnf->ucOpId = ucOpId;
    pstCirepQryCnf->usClientId = usClientId;
    pstCirepQryCnf->enReport = pstImsaAtControl->enReport;
    pstCirepQryCnf->enImsvops = enImsvops;
    pstCirepQryCnf->ulResult = VOS_OK;

    /*������Ϣ���ͺ��� */
    IMSA_SND_MSG(pstCirepQryCnf);
    return;
}
/*lint +e961*/
/*lint +e960*/
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
/* end of ImsaProcAtMsg.c */




