

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include  "NasEsmInclude.h"
#include  "msp_diag.h"

#include  "SysNvId.h"
#include  "omnvinterface.h"


/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASESMOMMSGPROC_C
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

VOS_UINT32                  g_NasEsmOmInfoIndFlag = 0;

/* xiongxianghui00253310 modify for ftmerrlog begin */
ESM_FTM_INFO_MANAGE_STRU             g_astEsmFtmInfoManage;
ESM_DATABASE_INFO_STRU               g_astEsmInfo;
/* xiongxianghui00253310 modify for ftmerrlog end   */

/*lifuxin00253982 modify for error log start*/
ESM_ERRLOG_INFO_MANAGE_STRU          g_astEsmErrlogInfoManage;
NAS_ESM_CN_CAUSE_TRANS_STRU          g_astEsmErrlogErrNoMap[] =
{
    /*------------casue-----------------------------------error id---------------------------*/
    {NAS_ESM_CAUSE_OPERATOR_DETERMINE_BARRING,      LNAS_ESM_CAUSE_OPERATOR_DETERMINE_BARRING},
    {NAS_ESM_CAUSE_INSUFFICIENT_RESOURCES,          LNAS_ESM_CAUSE_INSUFFICIENT_RESOURCES},
    {NAS_ESM_CAUSE_UNKNOWN_OR_MISSING_APN,          LNAS_ESM_CAUSE_UNKNOWN_OR_MISSING_APN},
    {NAS_ESM_CAUSE_UNKNOWN_PDN_TYPE,                LNAS_ESM_CAUSE_UNKNOWN_PDN_TYPE},
    {NAS_ESM_CAUSE_USER_AUTH_FAIL,                  LNAS_ESM_CAUSE_USER_AUTH_FAIL},
    {NAS_ESM_CAUSE_REQ_REJ_BY_SGW_OR_PGW,           LNAS_ESM_CAUSE_REQ_REJ_BY_SGW_OR_PGW},
    {NAS_ESM_CAUSE_REQ_REJ_UNSPECITY,               LNAS_ESM_CAUSE_REQ_REJ_UNSPECITY},
    {NAS_ESM_CAUSE_SERVICE_OPTION_NOT_SUPPORT,      LNAS_ESM_CAUSE_SERVICE_OPTION_NOT_SUPPORT},
    {NAS_ESM_CAUSE_REQ_SERVICE_NOT_SUBSCRIBED,      LNAS_ESM_CAUSE_REQ_SERVICE_NOT_SUBSCRIBED},
    {NAS_ESM_CAUSE_SERVICE_OPTION_TEMP_OUT_ORDER,   LNAS_ESM_CAUSE_SERVICE_OPTION_TEMP_OUT_ORDER},
    {NAS_ESM_CAUSE_PTI_ALREADY_IN_USED,             LNAS_ESM_CAUSE_PTI_ALREADY_IN_USED},
    {NAS_ESM_CAUSE_REGULAR_DEACTIVATION,            LNAS_ESM_CAUSE_REGULAR_DEACTIVATION},
    {NAS_ESM_CAUSE_EPS_QOS_NOT_ACCEPT,              LNAS_ESM_CAUSE_EPS_QOS_NOT_ACCEPT},
    {NAS_ESM_CAUSE_NETWORK_FAILURE,                 LNAS_ESM_CAUSE_NETWORK_FAILURE},
    {NAS_ESM_CAUSE_SYNTACTIC_ERR_IN_TFT,            LNAS_ESM_CAUSE_SYNTACTIC_ERR_IN_TFT},
    {NAS_ESM_CAUSE_SEMANTIC_ERR_IN_PACKET_FILTER,   LNAS_ESM_CAUSE_SEMANTIC_ERR_IN_PACKET_FILTER},
    {NAS_ESM_CAUSE_SYNCTACTIC_ERR_IN_PACKET_FILTER, LNAS_ESM_CAUSE_SYNCTACTIC_ERR_IN_PACKET_FILTER},
    {NAS_ESM_CAUSE_BEARER_WITHOUT_TFT_ACT,          LNAS_ESM_CAUSE_BEARER_WITHOUT_TFT_ACT},
    {NAS_ESM_CAUSE_PTI_MISMATCH,                    LNAS_ESM_CAUSE_PTI_MISMATCH},
    {NAS_ESM_CAUSE_LAST_PDN_DISCONN_NOT_ALLOWED,    LNAS_ESM_CAUSE_LAST_PDN_DISCONN_NOT_ALLOWED},
    {NAS_ESM_CAUSE_PDNTYPE_IPV4_ONLY_ALLOWED,       LNAS_ESM_CAUSE_PDNTYPE_IPV4_ONLY_ALLOWED},
    {NAS_ESM_CAUSE_PDNTYPE_IPV6_ONLY_ALLOWED,       LNAS_ESM_CAUSE_PDNTYPE_IPV6_ONLY_ALLOWED},
    {NAS_ESM_CAUSE_SINGLE_ADDR_BEARER_ONLY_ALLOWED, LNAS_ESM_CAUSE_SINGLE_ADDR_BEARER_ONLY_ALLOWED},
    {NAS_ESM_CAUSE_ESM_INFORMATION_NOT_RECEIVED,    LNAS_ESM_CAUSE_ESM_INFORMATION_NOT_RECEIVED},
    {NAS_ESM_CAUSE_PDN_CONNECTION_DOES_NOT_EXIST,   LNAS_ESM_CAUSE_PDN_CONNECTION_DOES_NOT_EXIST},
    {NAS_ESM_CAUSE_SAME_APN_MULTI_PDN_CONNECTION_NOT_ALLOWED, LNAS_ESM_CAUSE_SAME_APN_MULTI_PDN_CONNECTION_NOT_ALLOWED},
    {NAS_ESM_CAUSE_COLLISION_WITH_NETWORK_INITIATED_REQUEST,  LNAS_ESM_CAUSE_COLLISION_WITH_NETWORK_INITIATED_REQUEST},
    {NAS_ESM_CAUSE_UNSUPPORTED_QCI_VALUE,           LNAS_ESM_CAUSE_UNSUPPORTED_QCI_VALUE},
    {NAS_ESM_CAUSE_INVALID_PTI_VALUE,               LNAS_ESM_CAUSE_INVALID_PTI_VALUE},
    {NAS_ESM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE,  LNAS_ESM_CAUSE_SEMANTICALLY_INCORRECT_MESSAGE},
    {NAS_ESM_CAUSE_INVALID_MANDATORY_INFORMATION,   LNAS_ESM_CAUSE_INVALID_MANDATORY_INFORMATION},
    {NAS_ESM_CAUSE_MESSAGE_TYPE_NON_EXIST_OR_NOT_IMPLEMENTED, LNAS_ESM_CAUSE_MESSAGE_TYPE_NON_EXIST_OR_NOT_IMPLEMENTED},
    {NAS_ESM_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROT_STA, LNAS_ESM_CAUSE_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROT_STA},
    {NAS_ESM_CAUSE_INFO_ELEMENT_NON_EXIST_OR_NOT_IMPLEMENTED, LNAS_ESM_CAUSE_INFO_ELEMENT_NON_EXIST_OR_NOT_IMPLEMENTED},
    {NAS_ESM_CAUSE_CONDITIONAL_IE_ERROR,            LNAS_ESM_CAUSE_CONDITIONAL_IE_ERROR},
    {NAS_ESM_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_PROT_STA,      LNAS_ESM_CAUSE_MESSAGE_NOT_COMPATIBLE_WITH_PROT_STA},
    {NAS_ESM_CAUSE_PROTOCOL_ERROR,                  LNAS_ESM_CAUSE_PROTOCOL_ERROR},
    {NAS_ESM_CAUSE_APN_RESTRICTION_VAL_INCOMPATIBLE_WITH_ACT_BEARER, LNAS_ESM_CAUSE_APN_RESTRICTION_VAL_INCOMPATIBLE_WITH_ACT_BEARER},
};

static VOS_UINT32   g_astEsmErrlogErrNum
        = sizeof(g_astEsmErrlogErrNoMap)/sizeof(NAS_ESM_CN_CAUSE_TRANS_STRU);
/*lifuxin00253982 modify for error log end*/

VOS_UINT32               g_ulRptPdpStatus = APP_ESM_DT_REPORT_STATUS_CLOSE;
VOS_UINT32               g_ulNasEsmOmMsgHookFlag = 0;
APP_ESM_DT_STRU          g_stEsmDtInfo = {0};

/*****************************************************************************
  3 Function
*****************************************************************************/
/*****************************************************************************
 Function Name   : NAS_ESM_TransparentMsgProc
 Description     : ͸�������������
 Input           : VOS_VOID* pRcvMsg
 Output          : None
 Return          : VOS_VOID

 History         :
    1.sunbing49683      2008-12-30  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_TransparentMsgProc( VOS_VOID* pRcvMsg )
{
    NAS_ESM_TRANSPARENT_CMD_REQ_STRU  *pAppMsg = VOS_NULL_PTR;

    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG("NAS_ESM_TransparentMsgProc is entered.");

    pAppMsg = (NAS_ESM_TRANSPARENT_CMD_REQ_STRU*)pRcvMsg;

    switch(pAppMsg->enEsmTpCmdType)
    {
        case NAS_ESM_TP_CMD_TYPE_TEST:

            break;

        default:
            NAS_ESM_WARN_LOG("NAS_ESM_TransparentMsgProc:NORM: Transparent Cmd not support!");
            break;
    }

}


/*****************************************************************************
 Function Name   : NAS_ESM_SndEsmOmTpCmdCnfMsg
 Description     : ͸������ظ�
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.sunbing49683      2008-12-30  Draft Enact

*****************************************************************************/
/*lint -e960*/
/*lint -e961*/
VOS_VOID NAS_ESM_SndEsmOmTpCmdCnfMsg(const VOS_UINT8 *pucData,
                                                        VOS_UINT32 ulLength)
{
    PS_OM_TRANSPARENT_CMD_CNF_STRU     *pPsOmTpCmdCnf       = VOS_NULL_PTR;
    VOS_UINT16                          usOriginalId        = 0;
    VOS_UINT16                          usTerminalId        = 0;

    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG("NAS_ESM_SndEsmOmTpCmdCnfMsg is entered.");

    /*���ݿտ���Ϣ�ĳ��ȷ���ռ䣬������Ϣͷ��*/
    if(ulLength > ESM_MIN_SND_MSG_LEN)
    {
        pPsOmTpCmdCnf = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(PS_OM_TRANSPARENT_CMD_CNF_STRU) + (ulLength - ESM_MIN_SND_MSG_LEN));
    }
    else/*�������С��ESM_MIN_SND_MSG_LEN������Ŀռ����ESM_MIN_SND_MSG_LEN*/
    {
        pPsOmTpCmdCnf = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(PS_OM_TRANSPARENT_CMD_CNF_STRU));
    }

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pPsOmTpCmdCnf)
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_SndEsmOmTpCmdCnfMsg:ERROR:Alloc Msg fail!");
        return ;
    }


    /*���*/
    NAS_ESM_MEM_SET( NAS_ESM_GET_MSG_ENTITY(pPsOmTpCmdCnf), 0, NAS_ESM_GET_MSG_LENGTH(pPsOmTpCmdCnf));

    /*���տ���Ϣ��ŵ�EMM_ESM_DATA_REQ_STRU�ṹ��*/
    /*lint -e669*/
    NAS_ESM_MEM_CPY(pPsOmTpCmdCnf->aucTransCmdCnf, pucData, ulLength);
    /*lint +e669*/

    /*���䱾�ι���APP_MSG_HEADER*/
    NAS_ESM_AssignMidHeader(&usOriginalId,&usTerminalId);

    /*��д��Ϣͷ*/
    NAS_ESM_WRITE_APP_MSG_HEAD(pPsOmTpCmdCnf,OM_PS_TRANSPARENT_CMD_CNF,\
                              usOriginalId,usTerminalId,0);

    /*������Ϣ���ͺ��� */
    NAS_ESM_SND_MSG(pPsOmTpCmdCnf);

}



VOS_VOID NAS_ESM_SndAirMsgReportInd
(
    const VOS_UINT8                    *pucData,
    VOS_UINT32                          ulLength,
    NAS_ESM_AIR_MSG_DIR_ENUM_UINT8      enMsgDir,
    OM_PS_AIR_MSG_ENUM_UINT8            enMsgId
)
{
#ifndef PS_ITT_PC_TEST

    DIAG_AIR_MSG_LOG_STRU       stAirMsg;
    VOS_UINT8                  *pucTmpData = VOS_NULL_PTR;

    /*����տ���Ϣ�ռ�*/
    pucTmpData = (VOS_UINT8*)
                        NAS_ESM_MEM_ALLOC(ulLength);

    if(VOS_NULL_PTR== pucTmpData)
    {
        NAS_ESM_ERR_LOG("NAS_ESM_SndAirMsgReportInd: MEM_ALLOC ERR!");
        return;
    }

    NAS_ESM_MEM_CPY(pucTmpData, pucData, ulLength);

    /*���ÿտ���Ϣ����*/
    if(NAS_ESM_AIR_MSG_DIR_ENUM_UP == enMsgDir)
    {
        stAirMsg.ulId = DIAG_AIR_MSG_LOG_ID(PS_PID_ESM, OS_MSG_UL);
        stAirMsg.ulSideId = DIAG_SIDE_NET;
    }
    else
    {
        stAirMsg.ulId = DIAG_AIR_MSG_LOG_ID(PS_PID_ESM, OS_MSG_DL);
        stAirMsg.ulSideId = DIAG_SIDE_UE;
    }
    stAirMsg.ulMessageID = (VOS_UINT32)(enMsgId+ PS_MSG_ID_AIR_BASE);
    stAirMsg.ulDestMod = 0;
    stAirMsg.ulDataSize = ulLength;
    stAirMsg.pData = (void*)pucTmpData;

    /*�ϱ��տ���Ϣ*/
    if(ERR_MSP_SUCCESS != DIAG_ReportAirMessageLog(&stAirMsg))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_SndAirMsgReportInd: Send Msg Fail!");
    }

    /*�ͷſտ���Ϣ�ռ�*/
    NAS_ESM_MEM_FREE(pucTmpData);


#endif

    NAS_ESM_NORM_LOG("ESM->OMT: PS_OMT_AIR_MSG_REPORT_IND\r");

}


VOS_VOID NAS_ESM_SndKeyEventReportInd(OM_PS_KEY_EVENT_ENUM_UINT8 enKeyEvent)
{

#ifndef PS_ITT_PC_TEST
    /*lint -e778*/
    /*lint -e648*/
    if(ERR_MSP_SUCCESS != DIAG_ReportEventLog(MODID(UE_MODULE_ESM_ID, LOG_TYPE_INFO),\
                            (VOS_UINT32)enKeyEvent+OM_PS_KEY_EVENT_BASE_ID))
    {
        NAS_ESM_WARN_LOG("NAS_ESM_SndKeyEventReportInd: Send Msg Fail!");
    }
    /*lint +e648*/
    /*lint +e778*/
#endif

    NAS_ESM_NORM_LOG("ESM->OMT: PS_OMT_KEY_EVT_REPORT_IND\r");
}

VOS_VOID NAS_ESM_OmMsgDistr
(
    VOS_VOID                           *pRcvMsg
)
{
    PS_MSG_HEADER_STRU         *pOmMsg = VOS_NULL_PTR;

    /*��ӡ����ú���*/
    NAS_ESM_INFO_LOG("NAS_ESM_SmMsgDistr is entered.");

    pOmMsg = (PS_MSG_HEADER_STRU*)pRcvMsg;

    /*������Ϣ����������Ӧ����Ϣ������*/
    switch(pOmMsg->ulMsgName)
    {
        /*����յ�����ID_SM_ESM_PDP_CONTEXT_INFO_IND��Ϣ*/
        case ID_OM_ESM_INFO_REPORT_REQ:
            (VOS_VOID)LTE_MsgHook((VOS_VOID*)pRcvMsg);
            NAS_ESM_OmInfoIndMsgProc(pRcvMsg );
			break;

        case ID_APP_ESM_DT_INQ_PDP_INFO_REQ:
            NAS_ESM_AppQueryActPdpInfoMsgProc(pRcvMsg);
            break;

        default:
            /*��ӡ�쳣��Ϣ*/
            NAS_ESM_WARN_LOG("NAS_ESM_OmMsgDistr:WARNING:OM->ESM Message name non-existent!");
            break;
    }
}


VOS_VOID NAS_ESM_OmInfoIndMsgProc(VOS_VOID *pRcvMsg)
{
    OM_ESM_INFO_REPORT_REQ_STRU        *pEsmOmInfoReqMsg;
    OM_ESM_INFO_REPORT_CNF_STRU        *pEsmOmInfoCnfMsg;

    NAS_ESM_NORM_LOG("NAS_ESM_OMInfoIndMsgProc: enter !!");

    pEsmOmInfoReqMsg = (OM_ESM_INFO_REPORT_REQ_STRU *)pRcvMsg;

    if (OM_ESM_INFO_REPORT_OPEN == pEsmOmInfoReqMsg->enRptCtrl)
    {
        g_NasEsmOmInfoIndFlag = OM_ESM_REPORT_INFO_OPEN;
    }
    else
    {
        g_NasEsmOmInfoIndFlag = OM_ESM_REPORT_INFO_CLOSE;

    }
    /* �������ϱ���Ϣ��ȫ�ֱ������ */
    NAS_ESM_MEM_SET(g_stEsmOmInfo,
                    NAS_ESM_NULL,
                    sizeof(OM_ESM_STATE_INFO_STRU)*APP_ESM_MAX_EPSB_NUM);

    /* Ϊ��Ϣ�����ַ */
    pEsmOmInfoCnfMsg = (VOS_VOID *)NAS_ESM_ALLOC_MSG(sizeof(OM_ESM_INFO_REPORT_CNF_STRU));
    if(VOS_NULL_PTR  == pEsmOmInfoCnfMsg)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_AppInfoIndMsgProc: NAS_LMM_ALLOC_MSG alloc NULL_PTR !!");
        return;
    }


    /*���*/
    NAS_ESM_MEM_SET( NAS_ESM_GET_MSG_ENTITY(pEsmOmInfoCnfMsg), 0, NAS_ESM_GET_MSG_LENGTH(pEsmOmInfoCnfMsg));


    /* �����Ϣͷ */
    pEsmOmInfoCnfMsg->ulMsgId       = ID_OM_ESM_INFO_REPORT_CNF;
    pEsmOmInfoCnfMsg->ulSenderCpuId = pEsmOmInfoReqMsg->ulReceiverCpuId;
    pEsmOmInfoCnfMsg->ulSenderPid     = pEsmOmInfoReqMsg->ulReceiverPid;
    pEsmOmInfoCnfMsg->ulReceiverPid   = pEsmOmInfoReqMsg->ulSenderPid;
    pEsmOmInfoCnfMsg->ulReceiverCpuId  = pEsmOmInfoReqMsg->ulSenderCpuId;
    pEsmOmInfoCnfMsg->usOriginalId    = pEsmOmInfoReqMsg->usTerminalId;
    pEsmOmInfoCnfMsg->usTerminalId    = pEsmOmInfoReqMsg->usOriginalId;
    pEsmOmInfoCnfMsg->ulSN            = pEsmOmInfoReqMsg->ulSN;

    /*����OPID*/
    pEsmOmInfoCnfMsg->ulOpId      = pEsmOmInfoReqMsg->ulOpId;


    pEsmOmInfoCnfMsg->ulRslt      = APP_SUCCESS;

    /* ������Ϣ */
    NAS_ESM_SND_MSG(pEsmOmInfoCnfMsg);
    return;
}
/*lint +e961*/
/*lint +e960*/


/* xiongxianghui00253310 add for errlog 20131101 begin */

/*lint -e960*/
/*lint -e961*/
VOS_VOID NAS_ESM_OmMsgDistrForAcpuPidOm( VOS_VOID *pRcvMsg )
{
    VOS_UINT32              ulRst = 0;
    PS_MSG_HEADER_STRU      *pOmMsg  = VOS_NULL_PTR;
    NAS_ESM_INFO_LOG("NAS_ESM_OmMsgDistr is entered.");

    pOmMsg = (PS_MSG_HEADER_STRU*)pRcvMsg;

    switch(pOmMsg->ulMsgName)
    {
    case ID_OM_FTM_CTRL_IND:
        ulRst = NAS_ESM_RevOmFtmCtrlMsg(pRcvMsg);
        break;
    case ID_OM_ERR_LOG_CTRL_IND:
        ulRst = NAS_ESM_RevOmErrlogCtrlMsg(pRcvMsg);
        break;
    case ID_OM_ERR_LOG_REPORT_REQ:
        ulRst = NAS_ESM_RevOmReadErrlogReq(pRcvMsg);
        break;
    default:
        /* ��ӡ�쳣��Ϣ */
        NAS_ESM_WARN_LOG("NAS_ESM_OmMsgDistr:WARNING:OM->ESM Message name non-existent!");
        break;
    }

    if(NAS_ESM_MSG_HANDLED == ulRst)
    {
        NAS_ESM_INFO_LOG("OM MESSAGE PROCESS SUCCESS!");
    }
    else
    {
        NAS_ESM_INFO_LOG("OM MESSAGE PROCESS FAIL!");
    }

    return;
}
VOS_VOID NAS_ESM_OmInfoIndProc(VOS_VOID)
{
    VOS_UINT32 ulFtmRst = NAS_ESM_FAILURE;

    if(NAS_ESM_FTM_ACTION_FLAG_OPEN == NAS_ESM_GetFtmInfoActionFlag())
    {
        ulFtmRst = NAS_ESM_CompareEsmDatabaseInfo();
    }

    NAS_ESM_UpdateEsmDatabaseInfo();

    if(NAS_ESM_SUCCESS == ulFtmRst)
    {
        NAS_ESM_SendOmFtmMsg();
    }
}


VOS_VOID NAS_ESM_FtmInfoInit(VOS_VOID)
{
    NAS_ESM_INFO_LOG("NAS_ESM_FtmInfoInit.");

    NAS_ESM_MEM_SET(NAS_ESM_GetFtmInfoManageAddr(), 0, sizeof(ESM_FTM_INFO_MANAGE_STRU));

    NAS_ESM_MEM_SET(NAS_ESM_GetEsmInfoAddr(), 0, sizeof(ESM_DATABASE_INFO_STRU));

    NAS_ESM_GetFtmInfoActionFlag() = NAS_ESM_FTM_ACTION_FLAG_CLOSE;

    return;
}


VOS_UINT32   NAS_ESM_RevOmFtmCtrlMsg(MsgBlock   *pMsgStru)
{
    OM_FTM_CTRL_IND_STRU  *pstOmFtmCtrlInfo;
    NAS_ESM_INFO_LOG("NAS_ESM_RevOmFtmCtrlMsg.");

    if (VOS_NULL_PTR == pMsgStru)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_RevOmFtmCtrlMsg, input point is null!");

        return NAS_ESM_MSG_DISCARD;
    }

    pstOmFtmCtrlInfo = (OM_FTM_CTRL_IND_STRU *)(VOS_VOID *)pMsgStru;

    NAS_ESM_INFO_LOG1("ESM_OM_ProcOmFtmCtrlInfo, ucActionFlag = ", pstOmFtmCtrlInfo->ucActionFlag);

    if(1 == pstOmFtmCtrlInfo->ucActionFlag)
    {
        /*���ù���ģʽ�ϱ����ܴ�*/
        NAS_ESM_SetFtmOmManageFtmActionFlag(NAS_ESM_FTM_ACTION_FLAG_OPEN);

        /*�ϱ���ǰ��Ϣ */
        NAS_ESM_SendOmFtmMsg();
    }
    else
    {
        /*�رչ���ģʽ�ϱ���*/
        NAS_ESM_SetFtmOmManageFtmActionFlag(NAS_ESM_FTM_ACTION_FLAG_CLOSE);
    }

    return NAS_ESM_MSG_HANDLED;
}
VOS_UINT32 NAS_ESM_CompareEsmDatabaseInfo(VOS_VOID)
{
    NAS_ESM_INFO_LOG("NAS_ESM_CompareEsmDatabaseInfo.");

    if(NAS_ESM_GetFtmInfoEsmCause() != NAS_ESM_GetEsmInfoAddr()->ucEsmCause)
    {
        return NAS_ESM_SUCCESS;
    }

    return NAS_ESM_FAILURE;
}


VOS_VOID NAS_ESM_UpdateEsmDatabaseInfo(VOS_VOID)
{
    NAS_ESM_INFO_LOG("NAS_ESM_UpdateEsmDatabaseInfo.");

    NAS_ESM_GetEsmInfoAddr()->ucEsmCause = NAS_ESM_GetFtmInfoEsmCause();
}


VOS_VOID     NAS_ESM_SendOmFtmMsg(VOS_VOID)
{
    LNAS_OM_FTM_REPROT_IND_STRU *pFtmMsg;

    NAS_ESM_INFO_LOG("NAS_ESM_SendOmFtmMsg.");

    /* ������ģʽ�رգ��򲻷��� */
    if(NAS_ESM_FTM_ACTION_FLAG_OPEN != NAS_ESM_GetFtmInfoActionFlag())
    {
        NAS_ESM_INFO_LOG1("NAS_ESM_SendOmFtmMsg: ActionFlag = ", NAS_ESM_GetFtmInfoActionFlag());
        return;
    }

    /*����ռ䲢��������Ƿ�ɹ�*/
    pFtmMsg = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(LNAS_OM_FTM_REPROT_IND_STRU));
    if ( VOS_NULL_PTR == pFtmMsg )
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_SendOmFtmMsg:ERROR:Alloc Msg fail!");
        return ;
    }

    /*����ID_EMM_ESM_PDN_CON_IND��Ϣ*/
    /*�����Ϣͷ*/
    NAS_ESM_COMP_OM_MSG_HEADER(         pFtmMsg,
                                        (sizeof(LNAS_OM_FTM_REPROT_IND_STRU)-
                                        NAS_ESM_LEN_VOS_MSG_HEADER));

    /*�����ϢID*/
    pFtmMsg->ulMsgName                  = ID_OM_FTM_REPROT_IND;

    /*�����Ϣ����*/
    pFtmMsg->ulMsgType                  = OM_ERR_LOG_MSG_FTM_REPORT;
    pFtmMsg->ulMsgSN                    = (NAS_ESM_GetFtmInfoMsgSN());
    NAS_ESM_GetFtmInfoMsgSN()++;
    pFtmMsg->ulRptlen                   = sizeof(LNAS_OM_FTM_REPROT_CONTENT_STRU);

    pFtmMsg->stLnasFtmContent.ulMsgModuleID  = OM_ERR_LOG_MOUDLE_ID_ESM;
    pFtmMsg->stLnasFtmContent.usModemId      = 0;
    pFtmMsg->stLnasFtmContent.usProjectID    = 0;
    pFtmMsg->stLnasFtmContent.ulLength       = sizeof(LNAS_ESM_FTM_INFO_STRU);
    pFtmMsg->stLnasFtmContent.u.stEsmFtmInfo.ucEsmCause = g_astEsmInfo.ucEsmCause;

    /*��OM������Ϣ*/
    NAS_ESM_SND_MSG( pFtmMsg);


    return;

}


VOS_VOID NAS_ESM_ErrlogInfoInit(VOS_VOID)
{
    VOS_UINT32                         ulRslt;
    NV_ID_ERR_LOG_CTRL_INFO_STRU       stNvErrlogCtrl;

    NAS_ESM_INFO_LOG("NAS_ESM_ErrlogInfoInit.");

    NAS_ESM_MEM_SET(&stNvErrlogCtrl, 0, sizeof(NV_ID_ERR_LOG_CTRL_INFO_STRU));
    NAS_ESM_MEM_SET(NAS_ESM_GetErrlogManageAddr(), 0, sizeof(ESM_ERRLOG_INFO_MANAGE_STRU));
    /*lint -e718*/
    /*lint -e516*/
    /*lint -e732*/
    /*lint -e830*/
    ulRslt = LPs_NvimItem_Read(en_NV_Item_ErrLogCtrlInfo, &stNvErrlogCtrl, sizeof(NV_ID_ERR_LOG_CTRL_INFO_STRU));
    /*lint +e830*/
    /*lint +e732*/
    /*lint +e516*/
    /*lint +e718*/
    if(ERR_MSP_SUCCESS != ulRslt)
    {
        NAS_ESM_GetErrlogActionFlag() = NAS_ESM_ERRLOG_ACTION_FLAG_CLOSE;
        NAS_ESM_GetErrlogAlmLevel() = NAS_ESM_ERRLOG_LEVEL_CRITICAL;
        return;
    }

    if(1 == stNvErrlogCtrl.ucAlmStatus)
    {
        NAS_ESM_GetErrlogActionFlag() = NAS_ESM_ERRLOG_ACTION_FLAG_OPEN;
        NAS_ESM_GetErrlogAlmLevel() = stNvErrlogCtrl.ucAlmLevel;
    }
    else
    {
        NAS_ESM_GetErrlogActionFlag() = NAS_ESM_ERRLOG_ACTION_FLAG_CLOSE;
        NAS_ESM_GetErrlogAlmLevel() = NAS_ESM_ERRLOG_LEVEL_CRITICAL;
    }

    return;
}

VOS_UINT32 NAS_ESM_RevOmErrlogCtrlMsg(MsgBlock   *pMsgStru)
{
    OM_ERROR_LOG_CTRL_IND_STRU  *pstOmErrlogCtlInfo;

    NAS_ESM_INFO_LOG("NAS_LMM_RevOmErrlogCtrlMsg!");

    if (VOS_NULL_PTR == pMsgStru)
    {
        NAS_ESM_WARN_LOG("NAS_ESM_RevOmErrlogCtrlMsg, input point is null!");

        return NAS_ESM_MSG_DISCARD;
    }

    pstOmErrlogCtlInfo = (OM_ERROR_LOG_CTRL_IND_STRU *)(VOS_VOID *)pMsgStru;

    if(1 == pstOmErrlogCtlInfo->ucAlmStatus)
    {
        /*��������Errlog���ܴ�*/
        NAS_ESM_SetErrlogActionFlag(NAS_ESM_ERRLOG_ACTION_FLAG_OPEN);
        NAS_ESM_SetErrlogAlmLevel(pstOmErrlogCtlInfo->ucAlmLevel);
    }
    else
    {
        /*��������Errlog���ܹر�*/
        NAS_ESM_MEM_SET(NAS_ESM_GetErrlogManageAddr(), 0, sizeof(ESM_ERRLOG_INFO_MANAGE_STRU));
        NAS_ESM_SetErrlogActionFlag(NAS_ESM_ERRLOG_ACTION_FLAG_CLOSE);
    }

    return NAS_ESM_MSG_HANDLED;
}

 VOS_UINT32   NAS_ESM_RevOmReadErrlogReq(const MsgBlock   *pMsgStru)
 {
    VOS_UINT8  ucErrIndex = 0;

    NAS_ESM_INFO_LOG("NAS_ESM_RevOmReadErrlogReq is enterd!");

    /* ����ж� */
    if (VOS_NULL_PTR == pMsgStru)
    {
        NAS_ESM_INFO_LOG("NAS_ESM_RevOmReadErrlogReq, input point is null");

        return NAS_ESM_MSG_DISCARD;
    }

    /*����error log��Ϣ��OM*/
    NAS_ESM_SendOmErrlogCnf();

    /*�ϱ�֮��error log buffer���*/
    for(ucErrIndex = 0; ucErrIndex < NAS_ESM_ERRLOG_MAX_NUM; ucErrIndex++)
    {
        NAS_ESM_MEM_SET(&NAS_ESM_GetErrlogInfo(ucErrIndex), 0, sizeof(ESM_ERR_INFO_DETAIL_STRU));
    }

    NAS_ESM_GetErrlogAmount() = 0;
    NAS_ESM_GetErrlogNextNullPos() = 0;

    return NAS_ESM_MSG_HANDLED;
 }
VOS_VOID     NAS_ESM_SendOmErrlogCnf(VOS_VOID)
{
    ESM_OM_ERR_LOG_REPORT_CNF_STRU *pErrLogMsg;
    VOS_UINT8  ucErrIndex = 0;

    NAS_ESM_INFO_LOG1("NAS_ESM_SendOmErrlogCnf: ActionFlag = ", NAS_ESM_GetErrlogActionFlag());


    /*������Ϣ�ڴ�*/
    pErrLogMsg = (VOS_VOID *)NAS_ESM_ALLOC_MSG(sizeof(ESM_OM_ERR_LOG_REPORT_CNF_STRU));

    /*�ж�����������ʧ�ܴ�ӡ�����˳�*/
    if (VOS_NULL_PTR == pErrLogMsg)
    {
        /*��ӡ����*/
        NAS_ESM_INFO_LOG("NAS_ESM_SendOmErrlogCnf: MSG ALLOC ERR!");
        return;
    }

    NAS_ESM_MEM_SET(pErrLogMsg, 0, sizeof(ESM_OM_ERR_LOG_REPORT_CNF_STRU));

    /*����ID_EMM_ESM_PDN_CON_IND��Ϣ*/
    /*�����Ϣͷ*/
    NAS_ESM_COMP_OM_MSG_HEADER(         pErrLogMsg,
                                        (sizeof(ESM_OM_ERR_LOG_REPORT_CNF_STRU)-
                                         NAS_ESM_LEN_VOS_MSG_HEADER));

    /*�����ϢID*/
    pErrLogMsg->ulMsgName                  = ID_OM_ERR_LOG_REPORT_CNF;

    /*�����Ϣ����*/
    pErrLogMsg->ulMsgType                  = OM_ERR_LOG_MSG_ERR_REPORT;
    pErrLogMsg->ulMsgSN                    = NAS_ESM_GetErrlogMsgSN();
    NAS_ESM_GetErrlogMsgSN()++;

    if((NAS_ESM_ERRLOG_ACTION_FLAG_OPEN != NAS_ESM_GetErrlogActionFlag())
        ||(0 == NAS_ESM_GetErrlogAmount()))
    {
        pErrLogMsg->ulRptlen = 0;
    }
    else
    {
        pErrLogMsg->ulRptlen = sizeof(ESM_OM_ERR_LOG_INFO_STRU);
    }

    pErrLogMsg->stEsmErrlogInfo.ulMsgModuleID = OM_ERR_LOG_MOUDLE_ID_ESM;
    pErrLogMsg->stEsmErrlogInfo.usModemId = 0;
    pErrLogMsg->stEsmErrlogInfo.usALMLevel = NAS_ESM_GetErrlogAlmLevel();
    pErrLogMsg->stEsmErrlogInfo.usALMType = NAS_ESM_GetErrlogAlmType();

    pErrLogMsg->stEsmErrlogInfo.ulAlmLowSlice = NAS_ESM_GetErrlogAlmLowSlice();
    pErrLogMsg->stEsmErrlogInfo.ulAlmHighSlice = NAS_ESM_GetErrlogAlmHighSlice();

    pErrLogMsg->stEsmErrlogInfo.ulAlmLength = sizeof(ESM_ALM_INFO_STRU);

    pErrLogMsg->stEsmErrlogInfo.stAlmInfo.ulErrlogNum = NAS_ESM_GetErrlogAmount();

    for(ucErrIndex = 0; ucErrIndex < NAS_ESM_GetErrlogAmount(); ucErrIndex++)
    {
        pErrLogMsg->stEsmErrlogInfo.stAlmInfo.stEsmErrInfoDetail[ucErrIndex].usErrLogID =
            NAS_ESM_GetErrlogInfo(ucErrIndex).usErrLogID;
    }

    /*��OMģ�鷢��״̬�仯��Ϣ*/
    NAS_ESM_SND_MSG( pErrLogMsg);

    return;
}


VOS_VOID NAS_ESM_ErrlogInfoProc(VOS_UINT8 ucCnCause)
{
    VOS_UINT64                          ulCurTime;
    ESM_ERR_INFO_DETAIL_STRU            stErrlogInfo;

    NAS_ESM_INFO_LOG("NAS_ESM_ErrlogInfoProc!");

    if (NAS_ESM_ERRLOG_ACTION_FLAG_OPEN != NAS_ESM_GetErrlogActionFlag())
    {
        NAS_ESM_INFO_LOG("NAS_ESM_GetErrlogActionFlag() = NAS_ESM_ERRLOG_ACTION_FLAG_CLOSE!");

        return;
    }
    (VOS_VOID)BSP_BBPGetCurTime(&ulCurTime);
    NAS_ESM_GetErrlogAlmLowSlice() = ulCurTime & 0xffffffff;
    NAS_ESM_GetErrlogAlmHighSlice() = 0;

    stErrlogInfo.usErrLogID = NAS_ESM_CnCauseProc(ucCnCause);

    NAS_ESM_MEM_CPY(&NAS_ESM_GetErrlogInfo(NAS_ESM_GetErrlogNextNullPos()),
                    &stErrlogInfo,
                    sizeof(ESM_ERR_INFO_DETAIL_STRU));

    if(NAS_ESM_GetErrlogAmount() < NAS_ESM_ERRLOG_MAX_NUM)
    {
        /* �洢������Errlog����С��NAS_ESM_ERRLOG_MAX_NUM */

        NAS_ESM_GetErrlogNextNullPos()++;

        NAS_ESM_GetErrlogAmount()++;

        if(NAS_ESM_GetErrlogAmount() >= NAS_ESM_ERRLOG_MAX_NUM)
        {
            NAS_ESM_GetErrlogNextNullPos() = 0;
        }
    }
    else
    {
        /* �洢������Errlog��������NAS_ESM_ERRLOG_MAX_NUM */

        NAS_ESM_GetErrlogNextNullPos()++;

        if(NAS_ESM_GetErrlogNextNullPos() >= NAS_ESM_ERRLOG_MAX_NUM)
        {
            NAS_ESM_GetErrlogNextNullPos() = 0;
        }
    }

    return;
}



LNAS_OM_ERRLOG_ID_ENUM_UINT16  NAS_ESM_CnCauseProc(VOS_UINT8 ucCnCause)
{
    LNAS_OM_ERRLOG_ID_ENUM_UINT16   ulErrId = 0;
    VOS_UINT32 i;

    NAS_ESM_INFO_LOG("NAS_ESM_CnCauseProc!");

    for(i = 0; i < g_astEsmErrlogErrNum; i++)
    {
        if(ucCnCause == g_astEsmErrlogErrNoMap[i].ulCauseId)
        {
            ulErrId = g_astEsmErrlogErrNoMap[i].ulErrorlogID;
            break;
        }
    }
    if(g_astEsmErrlogErrNum == i)
    {
            ulErrId = LNAS_OM_ERRLOG_ID_CN_CAUSE_OTHERS;
    }

    return ulErrId;
}

/* xiongxianghui00253310 add for errlog 20131101 end  */
/*lint +e961*/
/*lint +e960*/

VOS_VOID NAS_ESM_ReportActPdpInfo( VOS_VOID)
{
    APP_ESM_INQ_PDP_INFO_IND_STRU    *pAppIndMsg;
    NAS_OM_ACT_PDP_INFO_STRU          stActPdpInfo;

    if (APP_ESM_DT_REPORT_STATUS_CLOSE == g_ulRptPdpStatus)
    {
        return;
    }

    /*����ռ䲢��������Ƿ�ɹ�*/
    pAppIndMsg = (VOS_VOID*)NAS_ESM_ALLOC_MSG(sizeof(APP_ESM_INQ_PDP_INFO_IND_STRU));

    /*����Ƿ����ɹ�*/
    if (VOS_NULL_PTR == pAppIndMsg)
    {
        /*��ӡ�쳣��Ϣ*/
        NAS_ESM_ERR_LOG("NAS_ESM_AppReportActPdpInfo:ERROR:ESM->APP ,Memory Alloc FAIL!");
        return;
    }

    /*���*/
    NAS_ESM_MEM_SET( &stActPdpInfo, 0, sizeof(NAS_OM_ACT_PDP_INFO_STRU));


    /*��д��Ϣͷ*/
    NAS_ESM_WRITE_APP_DT_IND_MSG_HEAD(pAppIndMsg,ID_APP_ESM_DT_INQ_PDP_INFO_IND,\
                              NAS_ESM_NULL,\
                              NAS_ESM_NULL,\
                              NAS_ESM_NULL);

    pAppIndMsg->ulOpId = DIAG_CMD_EMM_APP_PDP_INFO_IND;

    /*��ȡ������ص���Ϣ */
    NAS_ESM_GetActPdpInfo(&stActPdpInfo);

    NAS_ESM_MEM_CPY(&pAppIndMsg->stActPdpInfo,
                    &stActPdpInfo,
                    sizeof(NAS_OM_ACT_PDP_INFO_STRU));

    NAS_ESM_MEM_CPY(&g_stEsmDtInfo.stActPdpInfo,&stActPdpInfo,sizeof(NAS_OM_ACT_PDP_INFO_STRU));

#if (VOS_OS_VER != VOS_WIN32)
    if (1 == g_ulNasEsmOmMsgHookFlag)
    {
        (VOS_VOID)LTE_MsgHook((VOS_VOID *)pAppIndMsg);
    }
#endif
    /*������Ϣ���ͺ��� */
    NAS_ESM_SND_MSG(pAppIndMsg);
}




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

