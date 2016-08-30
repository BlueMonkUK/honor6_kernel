/******************************************************************************

        @(#)Copyright(C)2008,Hisilicon Co. LTD.

 ******************************************************************************
    File name   : NasEmmMrrcConnEst.c
    Description : ������EMM-MRRCģ�����������ݹ����д���RRC���ӽ���
                  �����Ѿ�������RRC��·�Ϸ����������ݵĹ���
    History     :
      1.  zangyalan 57968  2008-09-10  Draft Enact

******************************************************************************/


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include  "NasEmmGIMComm.h"
#include  "NasEmmMrrc.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASEMMMRRCCONNEST_C
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

NAS_EMM_MRRC_MGMT_DATA_STRU     g_stEmmMrrcMgmtData[NAS_LMM_RRC_SAVE_MSG_MAX_NUM];

VOS_UINT32              g_ulSaveMsgNum = 0;

static  VOS_UINT32              gulMmRrcOpId = 0;

/*RRC��������ֱ��NAS��Ϣ�������� */
NAS_LMM_SEND_MSG_RESULT_ACT_STRU  gstEmmMrrcSendMsgResultActTbl[]=
{
    {NAS_EMM_MSG_ATTACH_REQ,                NAS_EMM_SndAttachReqFailProc,           VOS_NULL_PTR},
    {NAS_EMM_MSG_ATTACH_CMP,                NAS_EMM_SndAttachCmpFailProc,           NAS_EMM_SndAttachCmpSuccProc},
    {NAS_EMM_MSG_DETACH_REQ,                NAS_EMM_SndDetachReqFailProc,           VOS_NULL_PTR},
    {NAS_EMM_MSG_DETACH_ACP,                NAS_EMM_SndMtDetachAccFailProc,         VOS_NULL_PTR},
    {NAS_EMM_MSG_LOAD_BALANCING_TAU,        NAS_EMM_SndTauReqFailProc,              VOS_NULL_PTR},
    {NAS_EMM_MSG_NOT_LOAD_BALANCING_TAU,    NAS_EMM_SndTauReqFailProc,              VOS_NULL_PTR},
    {NAS_EMM_MSG_TAU_CMP,                   NAS_EMM_SndTauCompleteFailProc,         NAS_EMM_SndTauCmpSuccProc},
    {NAS_EMM_MSG_SMC_COMPL,                 NAS_EMM_SndCommonProcedureMsgFailProc,  VOS_NULL_PTR},
    {NAS_EMM_MAS_SMC_REJ,                   NAS_EMM_SndCommonProcedureMsgFailProc,  VOS_NULL_PTR},
    {NAS_EMM_MSG_AUTH_RSP,                  NAS_EMM_SndCommonProcedureMsgFailProc,  VOS_NULL_PTR},
    {NAS_EMM_MSG_AUTH_FAIL,                 NAS_EMM_SndCommonProcedureMsgFailProc,  VOS_NULL_PTR},
    {NAS_EMM_MSG_IDEN_RSP,                  NAS_EMM_SndCommonProcedureMsgFailProc,  VOS_NULL_PTR},
    {NAS_EMM_NAS_UPLINK_NAS_TRANSPORT,      NAS_EMM_SndSmsdataFailProc,             VOS_NULL_PTR},
    {NAS_EMM_MSG_GUTI_CMP,                  NAS_EMM_SndCommonProcedureMsgFailProc,  VOS_NULL_PTR},
    {NAS_EMM_MSG_SERVICE_REQ,               NAS_EMM_SndServiceReqFailProc,          VOS_NULL_PTR},
    {NAS_EMM_MSG_EXTENDED_SERVICE_REQ,      NAS_EMM_SndExtendedServiceReqFailProc,  VOS_NULL_PTR},
    {NAS_EMM_MSG_ESM,                       NAS_EMM_SndEsmMsgFailProc,              VOS_NULL_PTR}
};
VOS_UINT32        g_ulSendMsgResultActTblLen = sizeof(gstEmmMrrcSendMsgResultActTbl)
                                                / sizeof(NAS_LMM_SEND_MSG_RESULT_ACT_STRU);
/*****************************************************************************
  3 Function
*****************************************************************************/
/*****************************************************************************
 Function Name  : NAS_EMM_MrrcInit
 Discription    : MRRCģ���ʼ������
 Input          :
 Output         : None
 Return         : None
 History:
      1.  zangyalan 57968 2008-10-16  Draft Enact
*****************************************************************************/
VOS_VOID NAS_EMM_MrrcInit(VOS_VOID)
{
    /*���ý������ͷ���·ģ��ĳ�ʼ������ */
    NAS_EMM_MrrcEstInit();
    NAS_EMM_MrrcRelInit();
    NAS_EMM_MrrcMgmtDataInit();
    NAS_EMM_INIT_MM_RRC_OPID();
    return;
}
VOS_VOID  NAS_EMM_FreeMrrcDyn( VOS_VOID )
{
    /* ��ʱΪ�� */
    return;
}


/*****************************************************************************
 Function Name  : NAS_EMM_MrrcEstInit
 Discription    : MRRC ����ģ���ʼ������
 Input          :
 Output         : None
 Return         : None
 History:
      1.  zangyalan 57968 2008-10-16  Draft Enact
*****************************************************************************/
VOS_VOID NAS_EMM_MrrcEstInit(VOS_VOID)
{
    /*����Դ*/
    NAS_LMM_StopStateTimer(TI_NAS_EMM_MRRC_WAIT_RRC_CONN_CNF);

    return;
}
/*lint -e960*/
/*lint -e961*/
VOS_VOID NAS_EMM_MrrcConnectFailInd(LRRC_LNAS_EST_RESULT_ENUM_UINT32 enEstResult)
{
    NAS_EMM_MRRC_CONNECT_FAIL_IND_STRU            *pstMrrcConnectFailIndMsg;
    VOS_UINT32                                    ulMsgLen;

    /*���㳤��*/
    ulMsgLen  = NAS_EMM_MRRC_CONNECT_FAIL_IND_LEN;

    /*��ȡ�ڲ���Ϣ��ַ*/
    pstMrrcConnectFailIndMsg = (NAS_EMM_MRRC_CONNECT_FAIL_IND_STRU *)NAS_LMM_GetSendIntraMsgBuffAddr(ulMsgLen);
    if(NAS_EMM_NULL_PTR == pstMrrcConnectFailIndMsg)
    {
        NAS_EMM_GIM_ERROR_LOG("NAS_EMM_MrrcConnectFailInd: Alloc Msg Fail!");
        return;
    }

    /*���VOS��Ϣͷ*/
    NAS_EMM_INTRA_MSG_HEADER(pstMrrcConnectFailIndMsg, ulMsgLen);

    /*��ֵ��Ϣ ID*/
    pstMrrcConnectFailIndMsg->ulMsgId = ID_NAS_LMM_INTRA_MRRC_CONNECT_FAIL_IND;

    /*��ֵ���ӽ���ʧ�ܵ�ԭ��*/
    pstMrrcConnectFailIndMsg->enEstResult = enEstResult;

    /* �����ڲ���Ϣ����� */
    NAS_EMM_SEND_INTRA_MSG(             pstMrrcConnectFailIndMsg);

    return;
}


VOS_VOID  NAS_EMM_ChangeRrcConnStateToEstSignal( VOS_VOID )
{
    /*�жϵ�ǰ����״̬ΪNOT_EXIST�����ϱ�MMC������������̬*/
    if(MMC_LMM_CONNECTED_SIG != NAS_EMM_GetConnState())
    {
        NAS_EMM_SendMmcStatusInd(       MMC_LMM_STATUS_TYPE_CONN_STATE,
                                        MMC_LMM_CONNECTED_SIG);
    }

    /*��RRC����״̬��Ϊ����״̬*/
    NAS_EMM_GetConnState() = NAS_EMM_CONN_SIG;
}


VOS_VOID  NAS_EMM_ChangeRrcConnStateToEsting( VOS_VOID )
{
    /*�жϵ�ǰ����״̬ΪNOT_EXIST�����ϱ�MMC������������̬*/
    if(MMC_LMM_CONN_ESTING != NAS_EMM_GetConnState())
    {
        NAS_EMM_SendMmcStatusInd(       MMC_LMM_STATUS_TYPE_CONN_STATE,
                                        MMC_LMM_CONN_ESTING);
    }

    /*��RRC����״̬��Ϊ����״̬*/
    NAS_EMM_GetConnState() = NAS_EMM_CONN_ESTING;
}


VOS_VOID  NAS_EMM_ChangeRrcConnStateToEstData( VOS_VOID )
{
    /*�жϵ�ǰ����״̬ΪDATA�����ϱ�MMC������������̬*/
    if(NAS_EMM_CONN_DATA != NAS_EMM_GetConnState())
    {
        NAS_EMM_SendMmcStatusInd(       MMC_LMM_STATUS_TYPE_CONN_STATE,
                                        MMC_LMM_CONNECTED_DATA);
    }

    /*��RRC����״̬��Ϊ����״̬*/
    NAS_EMM_GetConnState() = NAS_EMM_CONN_DATA;
}


VOS_VOID  NAS_EMM_SndUplinkNasMsg
(
    VOS_VOID *pMsgStru
)
{
    NAS_EMM_MRRC_DATA_REQ_STRU         *pstMrrcDataRqMsg = NAS_EMM_NULL_PTR;
    VOS_UINT32                          ulMmRrcOpId = NAS_LMM_RRC_OPID_NOT_VAILID;
	
#if(VOS_WIN32 == VOS_OS_VER)
    #ifdef PS_ITT_PC_TEST_NAS_ST
    VOS_UINT32                          ulRslt = VOS_FALSE;
    #endif
#endif

    NAS_EMM_GIM_NORMAL_LOG("NAS_EMM_SndUplinkNasMsg is entered.");

    /* ��μ�� */
    if ( NAS_EMM_NULL_PTR == pMsgStru)
    {
        NAS_EMM_GIM_ERROR_LOG("NAS_EMM_SndUplinkNasMsg:input ptr null!");
        return;
    }

    /* �ṹת�� */
    pstMrrcDataRqMsg = (NAS_EMM_MRRC_DATA_REQ_STRU *)pMsgStru;

    /* ��ӡ�տ���Ϣ */
    NAS_LMM_PrintCnMsgAndData(           &(pstMrrcDataRqMsg->stNasMsg));

    /* ��������ͷŹ����У����ٸ�RRC��������Ϣ����Ϊ���Լ����ڲ�����ʧ�� */
    if(NAS_EMM_CONN_RELEASING == NAS_EMM_GetConnState())
    {
        NAS_EMM_GIM_ERROR_LOG("NAS_EMM_SndUplinkNasMsg:Releasing,rrc is cell searching,can't est.");

        /*��MRRC_CONNECT_FAIL_IND*/
        NAS_EMM_MrrcConnectFailInd(LRRC_EST_CELL_SEARCHING);
        return;
    }




    /* �������ܺ������Ա���֮ǰ���������������̬��������������̬��
    �򻺴�δ���ܵ�����ֱ����Ϣ,���ܻ�ʹNAS COUNT��1 */
    if((NAS_EMM_CONN_SIG == NAS_EMM_GetConnState()) ||
        (NAS_EMM_CONN_DATA == NAS_EMM_GetConnState()))
    {
        ulMmRrcOpId                 = NAS_EMM_MrrcAllocMmRrcOpId();

        NAS_EMM_SaveMrrcDataReqMsg(pstMrrcDataRqMsg, ulMmRrcOpId);
    }

    /* �Կտ���Ϣ�ӱ���:��������͵��ڲ���Ϣ*/
    if( NAS_EMM_SMC_OPEN == g_ulSmcControl)
    {
        if(NAS_EMM_FAIL == NAS_EMM_SecuProtect(pstMrrcDataRqMsg))
        {
            NAS_EMM_GIM_ERROR_LOG("NAS_EMM_SndUplinkNasMsg:PROT ERR.");
            return;
        }
    }

    /* �쿴RRC�����Ƿ���� */
    if((NAS_EMM_CONN_SIG == NAS_EMM_GetConnState()) ||
        (NAS_EMM_CONN_DATA == NAS_EMM_GetConnState()))
    {
        NAS_EMM_RrcDataReq(pstMrrcDataRqMsg, ulMmRrcOpId);
    }
    else
    {
        NAS_EMM_IntraEstReq(pstMrrcDataRqMsg);
    }

    return;
}

/*leili modify for isr begin*/

VOS_VOID  NAS_EMM_CcoInterSystemSuccProc( VOS_VOID )
{
    if (NAS_EMM_CCO_CHANGE_TIN_VALID == NAS_LMM_GetEmmInfoTinChagFlag())
    {
        NAS_EMM_PUBU_LOG_INFO("NAS_EMM_CcoInterSystemSuccProc: GET MML PS BEARER INFO:");
        NAS_COMM_PrintArray(                NAS_COMM_GET_MM_PRINT_BUF(),
                                        (VOS_UINT8*)NAS_MML_GetPsBearerCtx(),
                                        sizeof(NAS_MML_PS_BEARER_CONTEXT_STRU)
                                        *EMM_ESM_MAX_EPS_BEARER_NUM);
        /*PC REPLAY MODIFY BY LEILI BEGIN*/
        NAS_EMM_UpdateBearISRFlag(NAS_EMM_GetPsBearerCtx());
		/*PC REPLAY MODIFY BY LEILI END*/
        NAS_LMM_SetTempEmmInfoTinType(MMC_LMM_TIN_BUTT);
        NAS_LMM_SetEmmInfoTinChagFlag(NAS_EMM_CCO_CHANGE_TIN_INVALID);

        NAS_EMM_PUBU_LOG_INFO("NAS_EMM_CcoInterSystemSuccProc: UPDATE MML PS BEARER INFO:");
        NAS_COMM_PrintArray(                NAS_COMM_GET_MM_PRINT_BUF(),
                                        (VOS_UINT8*)NAS_MML_GetPsBearerCtx(),
                                        sizeof(NAS_MML_PS_BEARER_CONTEXT_STRU)
                                        *EMM_ESM_MAX_EPS_BEARER_NUM);
    }
    return;
}


VOS_VOID  NAS_EMM_CcoInterSystemFailProc( VOS_VOID )
{

    NAS_EMM_PUBU_LOG_INFO("NAS_EMM_CcoInterSystemFailProc: GET MML PS BEARER INFO:");
    NAS_COMM_PrintArray(                NAS_COMM_GET_MM_PRINT_BUF(),
                                        (VOS_UINT8*)NAS_MML_GetPsBearerCtx(),
                                        sizeof(NAS_MML_PS_BEARER_CONTEXT_STRU)
                                        *EMM_ESM_MAX_EPS_BEARER_NUM);
    if (NAS_EMM_CCO_CHANGE_TIN_VALID == NAS_LMM_GetEmmInfoTinChagFlag())
    {
        NAS_EMM_SetTinType(NAS_LMM_GetTempEmmInfoTinType());
        NAS_LMM_SetTempEmmInfoTinType(MMC_LMM_TIN_BUTT);
        NAS_LMM_SetEmmInfoTinChagFlag(NAS_EMM_CCO_CHANGE_TIN_INVALID);

        NAS_EMM_PUBU_LOG_INFO("NAS_EMM_CcoInterSystemFailProc: UPDATE MML PS BEARER INFO:");
        NAS_COMM_PrintArray(                NAS_COMM_GET_MM_PRINT_BUF(),
                                            (VOS_UINT8*)NAS_MML_GetPsBearerCtx(),
                                            sizeof(NAS_MML_PS_BEARER_CONTEXT_STRU)
                                            *EMM_ESM_MAX_EPS_BEARER_NUM);
    }

    return;
}

/*leili modify for isr end*/

VOS_UINT32  NAS_EMM_MsRrcConnEstInitSsWaitRrcConnMsgRrcMmEstCnf
                                             (VOS_UINT32 ulMsgId, VOS_VOID *pMsgStru)
{
    LRRC_LMM_EST_CNF_STRU                *pstEstCnfMsg;
    VOS_UINT32                          ulResult;

    NAS_EMM_GIM_NORMAL_LOG("NAS_EMM_MsRrcConnEstInitSsWaitRrcConnMsgRrcMmEstCnf is entered.");

    (VOS_VOID)(ulMsgId);

    /* ��μ�� */
    if ( NAS_EMM_NULL_PTR == pMsgStru)
    {
        NAS_EMM_GIM_ERROR_LOG("NAS_EMM_MsRrcConnSetInitSsWaitRrcConnMsgRrcMmEstCnf:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /*��齨���Ƿ�ɹ�*/
    pstEstCnfMsg = (LRRC_LMM_EST_CNF_STRU *)pMsgStru;
    ulResult = pstEstCnfMsg->enResult;

    switch (ulResult)
    {
        case LRRC_EST_SUCCESS:
        case LRRC_EST_PRESENT:
            NAS_EMM_ChangeRrcConnStateToEstSignal();
            NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_T3412);
            /*leili modify for isr begin*/
            /*֪ͨGUģT3412ֹͣ*/
            NAS_EMM_SendTimerStateNotify(TI_NAS_EMM_PTL_T3412, LMM_MMC_TIMER_STOP);

            NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_T3423);

            /*֪ͨGUģT3423ֹͣ*/
            NAS_EMM_SendTimerStateNotify(TI_NAS_EMM_PTL_T3423, LMM_MMC_TIMER_STOP);

            /*leili modify for isr end*/
            NAS_LMM_StopStateTimer(TI_NAS_EMM_MRRC_WAIT_RRC_CONN_CNF);
            NAS_EMM_FSM_PopState();
            /*NAS_LMM_WriteEpsSecuContext(NAS_NV_ITEM_DELETE);*/
            NAS_EMM_ClearBarResouce();
            /*leili modify for isr begin*/
            NAS_EMM_CcoInterSystemSuccProc();
            /*leili modify for isr end*/
            break;

        case LRRC_EST_ESTING:
            NAS_EMM_GIM_WARNING_LOG("MRRC:LRRC_EST_ESTING is ignore!");
            NAS_EMM_ChangeRrcConnStateToEsting();
            break;

        default:
            NAS_LMM_StopStateTimer(TI_NAS_EMM_MRRC_WAIT_RRC_CONN_CNF);
            NAS_EMM_FSM_PopState();

            NAS_EMM_SecuCurrentContextUpNasCountBack();

            /*��MRRC_CONNECT_FAIL_IND*/
            NAS_EMM_MrrcConnectFailInd(ulResult);
            break;
    }

     return NAS_LMM_MSG_HANDLED;
}

/*****************************************************************************
 Function Name  : NAS_EMM_MsRrcConnSetInitSsWaitRrcConnMsgTIWaitRrcConnTO
 Discription    : MRRC�ȴ�RRC���ӽ����Ķ�ʱ��TI_NAS_EMM_WAIT_RRC_CONN��ʱ,
                  ��Ϊ����ʧ��,������Դģ�鷢��MRRC_REL_IND��Ϣ
 Input          : ��ϢID,��Ϣ
 Output         : None
 Return         : None
 History:
      1.  zangyalan 57968  2008-09-10  Draft Enact
*****************************************************************************/
VOS_UINT32  NAS_EMM_MsRrcConnEstInitSsWaitRrcConnMsgTIWaitRrcConnTO
                                                   (VOS_UINT32 ulMsgId, const VOS_VOID *pMsgStru)
{

    NAS_EMM_GIM_INFO_LOG("NAS_EMM_MsRrcConnEstInitSsWaitRrcConnMsgTIWaitRrcConnTO is entered.");

    (VOS_VOID)(ulMsgId);
    (VOS_VOID)(pMsgStru);

    /* ״̬����ջ */
    NAS_EMM_FSM_PopState();   /*����������*/

    NAS_EMM_SecuCurrentContextUpNasCountBack();

    /*��MRRC_CONNECT_FAIL_IND*/
    NAS_EMM_MrrcConnectFailInd(LRRC_EST_EST_CONN_FAIL);

    /* ֪ͨRRC�ͷŻ����NAS������������е�NAS�տ���Ϣ */
    NAS_EMM_SndRrcRelReq(NAS_LMM_NOT_BARRED);

    /* ��RRC���ͷź�����״̬��Ϊ�ͷŹ�����,
    δ��RRC�����ͷŵĳ���������Ϊ�ͷŹ����У���������3440��ʱ�� */
    NAS_EMM_SetConnState(NAS_EMM_CONN_RELEASING);

    return  NAS_LMM_MSG_HANDLED;
}



VOS_UINT32  NAS_EMM_MsRrcConnEstInitSsWaitRrcConnMsgRrcRelInd
                                                   (VOS_UINT32 ulMsgId, VOS_VOID *pMsgStru)
{

    NAS_EMM_GIM_INFO_LOG("NAS_EMM_MsRrcConnEstInitSsWaitRrcConnMsgRrcRelInd is entered.");

    (VOS_VOID)(ulMsgId);
    (VOS_VOID)(pMsgStru);

    /* ֹͣ��ǰ��ѹջ���� */
    NAS_LMM_StopStateTimer(TI_NAS_EMM_MRRC_WAIT_RRC_CONN_CNF);

    /* ״̬����ջ */
    NAS_EMM_FSM_PopState();   /*����������*/

    NAS_EMM_SecuCurrentContextUpNasCountBack();

    NAS_EMM_SetConnState(NAS_EMM_CONN_RELEASING);

    /*��MRRC_CONNECT_FAIL_IND*/
    NAS_EMM_MrrcConnectFailInd(LRRC_EST_EST_CONN_FAIL);

    return  NAS_LMM_MSG_HANDLED;
}

/*****************************************************************************
 Function Name   : NAS_EMM_MsRrcConnEstInitSsWaitRrcConnMsgMmcCoverageLostInd
 Description     : �����������յ�RRC�Ķ�����Ϣ
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.sunbing49683      2009-11-11  Draft Enact

*****************************************************************************/
VOS_UINT32 NAS_EMM_MsRrcConnEstInitSsWaitRrcConnMsgMmcCoverageLostInd
(
    VOS_UINT32 ulMsgId,
    VOS_VOID *pMsgStru
)
{
    NAS_EMM_GIM_NORMAL_LOG("NAS_EMM_MsRrcConnEstInitSsWaitRrcConnMsgMmcCoverageLostInd is entered.");

    (VOS_VOID)(ulMsgId);
    (VOS_VOID)(pMsgStru);

    /* ֹͣ��ǰ��ѹջ���� */
    NAS_LMM_StopStateTimer(TI_NAS_EMM_MRRC_WAIT_RRC_CONN_CNF);

    /* ״̬����ջ */
    NAS_EMM_FSM_PopState();   /*����������*/


    NAS_EMM_SecuCurrentContextUpNasCountBack();

    /*��MRRC_CONNECT_FAIL_IND*/
    NAS_EMM_MrrcConnectFailInd(LRRC_EST_EST_CONN_FAIL);

    /* EMM��EMM�Լ�����AREA LOST��Ϣ���Ը�ѹջǰ�����̴��� */
    NAS_EMMC_SendEmmCoverageLostInd();

    return  NAS_LMM_MSG_HANDLED;

}


/*****************************************************************************
 Function Name  : NAS_EMM_IntraEstReq
 Discription    : �յ�public������mrrc est req��Ϣ��,ѹջ,��rrc_mm_est_req
 Input          :
 Output         : None
 Return         : None
 History:
      1.  zangyalan 57968  2008-09-10  Draft Enact
*****************************************************************************/
VOS_VOID NAS_EMM_IntraEstReq(const NAS_EMM_MRRC_DATA_REQ_STRU *pMsg)
{
    NAS_EMM_FSM_STATE_STRU              EmmState;

    /*ԭ״̬ѹջ*/
    NAS_EMM_FSM_PushState();

    /*����RRC_MM_EST_REQ*/
    NAS_EMM_RrcEstReq(pMsg);

    NAS_EMM_ChangeRrcConnStateToEsting();

    /* ����RRC_CONN_EST_INIT.RRC_CONN_WAIT_EST_CNF״̬ */
    EmmState.enFsmId         = NAS_LMM_PARALLEL_FSM_EMM;
    EmmState.enMainState     = EMM_MS_RRC_CONN_EST_INIT;
    EmmState.enSubState      = EMM_SS_RRC_CONN_WAIT_EST_CNF;
    EmmState.enStaTId        = TI_NAS_EMM_MRRC_WAIT_RRC_CONN_CNF;
    NAS_LMM_StaTransProc(EmmState);

    /*������ʱ��*/
    NAS_LMM_StartStateTimer(TI_NAS_EMM_MRRC_WAIT_RRC_CONN_CNF);

    /* �ı�ӿ�״̬
    */

    return ;
}


VOS_VOID    NAS_EMM_RrcEstReq(const NAS_EMM_MRRC_DATA_REQ_STRU *pMsg)
{
    LRRC_LMM_EST_REQ_STRU              *pstEstReqMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulEstReqLenNoHeader;

    /* ��¼�տ���Ϣ�����ͣ�����Ϣ����ʧ��ʱ�ж�ʹ��*/
    NAS_LMM_GetLastAirMsgType() = pMsg->stNasMsg.aucNasMsg[NAS_EMM_PLAIN_MSG_TYPE_POS];

    /*���㳤��*/
    ulEstReqLenNoHeader             = (sizeof(LRRC_LMM_EST_REQ_STRU)+
                                    (pMsg->stNasMsg.ulNasMsgSize-NAS_EMM_4BYTES_LEN))-
                                    NAS_EMM_LEN_VOS_MSG_HEADER;
    /* �����ڴ�*/
    pstEstReqMsg = (VOS_VOID *)NAS_LMM_ALLOC_MSG(ulEstReqLenNoHeader + NAS_EMM_LEN_VOS_MSG_HEADER);
    if(NAS_EMM_NULL_PTR == pstEstReqMsg)
    {
        /* ��ӡ�ڴ����ʧ��log */
        NAS_EMM_GIM_ERROR_LOG("NAS_EMM_RrcEstReq: Alloc Msg ERR!");
        return;
    }

    /*���VOS��Ϣͷ*/
    NAS_EMM_GIM_RRC_MSG_HEADER(pstEstReqMsg, ulEstReqLenNoHeader);

    /*�����ϢID*/
    pstEstReqMsg->enMsgId    = ID_LRRC_LMM_EST_REQ;

    /*���������ԭ��*/
    pstEstReqMsg->enEstCause = pMsg->enEstCaue;
    pstEstReqMsg->enCallType = pMsg->enCallType;
    /*��� NAS������Ϣ*/
    NAS_LMM_MEM_SET(&(pstEstReqMsg->stEstInfo), 0, sizeof(LRRC_LNAS_EST_INFO_STRU));
    NAS_EMM_FillEstInfo(pstEstReqMsg, pMsg->enEmmMsgType);

    /*�����Ϣ��*/
    pstEstReqMsg->stFirstNasMsg.ulNasMsgSize = pMsg->stNasMsg.ulNasMsgSize;
    NAS_LMM_MEM_CPY(pstEstReqMsg->stFirstNasMsg.aucNasMsg,
                        pMsg->stNasMsg.aucNasMsg,
                        pMsg->stNasMsg.ulNasMsgSize);


    /*����RRC_MM_EST_REQ*/
    NAS_LMM_SEND_MSG(pstEstReqMsg);

    return;
}


VOS_VOID  NAS_EMM_FillEstInfo(
                                LRRC_LMM_EST_REQ_STRU              *pstEstReqMsg,
                                NAS_EMM_MSG_TYPE_ENUM_UINT32        enEmmMsgType )
{
    NAS_EMM_UEID_STRU                  *pstMmUeId = VOS_NULL_PTR;
    NAS_LMM_NETWORK_INFO_STRU           *pMmNetInfo = VOS_NULL_PTR;
    NAS_MM_TA_STRU                      stCurTa;
    NAS_MM_TA_LIST_STRU                 stTaiList;
    NAS_LMM_GUTI_STRU                    stMappedGuti = {0};
    VOS_UINT32                          ulRslt;

    /*��ȡ UE ��ʶ*/
    pstMmUeId                           = NAS_LMM_GetEmmInfoUeidAddr();

    /*TIN = P-TMSI:
      ������� mapped GUTI,��� mapped GUTI�л�ȡ GUMMEI;
      ��������� mapped GUTI,��Ȳ�Я��S_TMSI,Ҳ��Я��GUMMEI*/
    if(NAS_LMM_SUCC ==  NAS_EMM_CheckMutiModeSupport())
    {
        if (MMC_LMM_TIN_P_TMSI == NAS_EMM_GetTinType())
        {
            if(NAS_EMM_SUCC == NAS_EMM_CheckPtmsiAndRaiValidity())
            {
                NAS_EMM_DeriveMappedGuti(&stMappedGuti);
                pstEstReqMsg->stEstInfo.ucBitOpRegGummei = NAS_EMM_BIT_SLCT;
                pstEstReqMsg->stEstInfo.stRegGummei.ucMmec = stMappedGuti.stMmeCode.ucMmeCode;
                pstEstReqMsg->stEstInfo.stRegGummei.usMmeGi = ((stMappedGuti.stMmeGroupId.ucGroupId<< 8)
                                                      | (stMappedGuti.stMmeGroupId.ucGroupIdCnt));

                if (NAS_RELEASE_CTRL)
                {
                    pstEstReqMsg->stEstInfo.stRegGummei.enGummeiType = LRRC_LNAS_GUMMEI_TYPE_MAPPED;
                }
                NAS_LMM_MEM_CPY(            &(pstEstReqMsg->stEstInfo.stRegGummei.stPlmnId),
                                            &(stMappedGuti.stPlmnId),
                                            sizeof(NAS_MM_PLMN_ID_STRU));
            }

            return;
        }
    }

    /*TIN != P-TMSI*/
    if(NAS_EMM_MSG_LOAD_BALANCING_TAU != enEmmMsgType)
    {
        NAS_LMM_MEM_SET(&stTaiList, 0, sizeof(NAS_MM_TA_LIST_STRU));
        NAS_LMM_MEM_SET(&stCurTa, 0, sizeof(NAS_MM_TA_STRU));

        pMmNetInfo                          = NAS_LMM_GetEmmInfoNetInfoAddr();

        /*Current TA*/
        NAS_LMM_MEM_CPY(                     &(stCurTa.stPlmnId),
                                            &(pMmNetInfo->stPresentNetId.stPlmnId),
                                            sizeof(NAS_MM_PLMN_ID_STRU));
        NAS_LMM_MEM_CPY(                     &(stCurTa.stTac),
                                            &(pMmNetInfo->stPresentNetId.stTac),
                                            sizeof(NAS_MM_TAC_STRU));
        /*��ȡ�ϴ�ע���TA List*/
        NAS_LMM_MEM_CPY(                     &(stTaiList),
                                            &(pMmNetInfo->stTaiList),
                                            sizeof(NAS_MM_TA_LIST_STRU));

        ulRslt = NAS_LMM_TaMatchTaList(&stCurTa, &stTaiList);

        /*��ǰTA����ע���TA�����ҵ�ǰ���̲���attach����
          (detach֮��û��ɾ��ta list������ta listֻ��ע��̬������Ч��)*/
        if((NAS_LMM_MATCH_SUCCESS == ulRslt)
            && (NAS_EMM_MSG_ATTACH_REQ != enEmmMsgType))
        {
            NAS_EMM_GIM_NORMAL_LOG("NAS_EMM_RrcEstReq:provide S-TMSI to RRC ");
            pstEstReqMsg->stEstInfo.ucBitOpSTmsi = pstMmUeId->bitOpGuti;
            pstEstReqMsg->stEstInfo.stStmsi.ucMmec = pstMmUeId->stGuti.stMmeCode.ucMmeCode;
            pstEstReqMsg->stEstInfo.stStmsi.ulMTmsi = (pstMmUeId->stGuti.stMTmsi.ucMTmsi<<24)
                                            |(pstMmUeId->stGuti.stMTmsi.ucMTmsiCnt1<<16)
                                            |(pstMmUeId->stGuti.stMTmsi.ucMTmsiCnt2<<8)
                                            |(pstMmUeId->stGuti.stMTmsi.ucMTmsiCnt3);

        }
        else
        {
            NAS_EMM_GIM_NORMAL_LOG("NAS_EMM_RrcEstReq:provide GUMMEI to RRC");
            pstEstReqMsg->stEstInfo.ucBitOpRegGummei = pstMmUeId->bitOpGuti;
            pstEstReqMsg->stEstInfo.stRegGummei.ucMmec = pstMmUeId->stGuti.stMmeCode.ucMmeCode;
            pstEstReqMsg->stEstInfo.stRegGummei.usMmeGi = ((pstMmUeId->stGuti.stMmeGroupId.ucGroupId<< 8)
                                                  | (pstMmUeId->stGuti.stMmeGroupId.ucGroupIdCnt));
            if (NAS_RELEASE_CTRL)
            {
                pstEstReqMsg->stEstInfo.stRegGummei.enGummeiType = LRRC_LNAS_GUMMEI_TYPE_NATIVE;
            }

            NAS_LMM_MEM_CPY(            &(pstEstReqMsg->stEstInfo.stRegGummei.stPlmnId),
                                        &(pstMmUeId->stGuti.stPlmnId),
                                        sizeof(NAS_MM_PLMN_ID_STRU));
        }

    }
}


VOS_VOID    NAS_EMM_RrcDataReq(         const NAS_EMM_MRRC_DATA_REQ_STRU *pMsg,
                                        VOS_UINT32  ulMmRrcOpId)
{
    LRRC_LMM_DATA_REQ_STRU               *pstDataReqMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulRrcMmDataReqMsgLen;

    /* ��¼�տ���Ϣ�����ͣ�����Ϣ����ʧ��ʱ�ж�ʹ��*/
    NAS_LMM_GetLastAirMsgType() = pMsg->stNasMsg.aucNasMsg[NAS_EMM_PLAIN_MSG_TYPE_POS];
    ulRrcMmDataReqMsgLen = (sizeof(LRRC_LMM_DATA_REQ_STRU)+
                           pMsg->stNasMsg.ulNasMsgSize)-
                           NAS_EMM_4BYTES_LEN;

    /*�����ڴ�*/
    pstDataReqMsg   = (VOS_VOID *)NAS_LMM_ALLOC_MSG(ulRrcMmDataReqMsgLen);
    if(NAS_EMM_NULL_PTR == pstDataReqMsg)
    {
        /* ��ӡ�ڴ����ʧ��log */
        NAS_EMM_GIM_ERROR_LOG("NAS_EMM_RrcDataReq: NAS_LMM_ALLOC_MSG ERR!");

        return;
    }

    /*���VOS��Ϣͷ*/
    NAS_EMM_GIM_RRC_MSG_HEADER(pstDataReqMsg,(ulRrcMmDataReqMsgLen-NAS_EMM_LEN_VOS_MSG_HEADER));

    /*�����ϢID*/
    pstDataReqMsg->enMsgId  = ID_LRRC_LMM_DATA_REQ;

    /*���OpId*/
    pstDataReqMsg->ulOpId   = ulMmRrcOpId;

    /*���enDataReqCnf*/
    pstDataReqMsg->enDataCnf = pMsg->enDataCnf;

    /*�����Ϣ�� */
    pstDataReqMsg->stNasMsg.ulNasMsgSize = pMsg->stNasMsg.ulNasMsgSize;
    NAS_LMM_MEM_CPY(pstDataReqMsg->stNasMsg.aucNasMsg,
                       pMsg->stNasMsg.aucNasMsg,
                       pMsg->stNasMsg.ulNasMsgSize);

    NAS_EMM_GIM_INFO_LOG2("NAS_EMM_RrcDataReq,MmRrcOpId ,enDataCnf ",
                                        ulMmRrcOpId,
                                        pMsg->enDataCnf);

    /* ����RRC_MM_DATA_REQ */
    NAS_LMM_SEND_MSG(pstDataReqMsg);

    return;
}
VOS_VOID  NAS_EMM_SaveMrrcDataReqMsg
(
    const NAS_EMM_MRRC_DATA_REQ_STRU        *pstMsg,
    VOS_UINT32                              ulMmRrcOpId
)
{
    NAS_EMM_MRRC_MGMT_DATA_STRU         *pEmmMrrcMgmtData = NAS_EMM_NULL_PTR;



    pEmmMrrcMgmtData = NAS_EMM_GET_MRRC_MGMT_DATA_ADDR();

    if ((pstMsg->stNasMsg.ulNasMsgSize > NAS_LMM_MRRC_NAS_MSG_MAX_LEN)
        || (NAS_EMM_NULL == pstMsg->stNasMsg.ulNasMsgSize))
    {
        NAS_EMM_PUBU_LOG2_INFO("NAS_EMM_SaveMrrcDataReqMsg:NAS MSG LENGTH ERROR!,MaxLen: MsgLen:",
                                NAS_LMM_MRRC_NAS_MSG_MAX_LEN, pstMsg->stNasMsg.ulNasMsgSize);
        return;
    }

    /*TC��Ϣ����¼ */
    if (pstMsg->enEmmMsgType != NAS_EMM_MSG_TC)
    {
        /* ����������Ϣ�����ﵽ���Ի�������ֵ,���������Ϣһ��ǰ��*/
        if (NAS_LMM_RRC_SAVE_MSG_MAX_NUM == g_ulSaveMsgNum)
        {
            /* ����Ϣ����ǰ�ƣ���Ҫ�洢�ķ���ĩβ */
            NAS_LMM_MEM_CPY(pEmmMrrcMgmtData,
                            (pEmmMrrcMgmtData+1),
                            (sizeof(NAS_EMM_MRRC_MGMT_DATA_STRU))*(NAS_LMM_RRC_SAVE_MSG_MAX_NUM -1));

            pEmmMrrcMgmtData = (pEmmMrrcMgmtData +  NAS_LMM_RRC_SAVE_MSG_MAX_NUM) -1;
        }
        else
        {
            pEmmMrrcMgmtData = pEmmMrrcMgmtData +  g_ulSaveMsgNum;
        }

        /*����OpId */
        pEmmMrrcMgmtData->ulEsmMmOpId = pstMsg->ulEsmMmOpId;

        pEmmMrrcMgmtData->ulMmRrcOpId = ulMmRrcOpId;

        /*����NAS�տ���ϢID�Լ����� */
        pEmmMrrcMgmtData->enLastMsgId = pstMsg->enEmmMsgType;

        pEmmMrrcMgmtData->ulNasMsgLength = pstMsg->stNasMsg.ulNasMsgSize;

        NAS_LMM_MEM_CPY(pEmmMrrcMgmtData->aucNasMsgContent,
                        pstMsg->stNasMsg.aucNasMsg,
                        pstMsg->stNasMsg.ulNasMsgSize);

        if (NAS_LMM_RRC_SAVE_MSG_MAX_NUM != g_ulSaveMsgNum)
        {
            g_ulSaveMsgNum ++;
        }

        NAS_EMM_PUBU_LOG2_INFO(      "NAS_EMM_SaveMrrcDataReqMsg,MmRrcOpId , enEmmMsgType",
                            ulMmRrcOpId, pstMsg->enEmmMsgType);

        NAS_EMM_PUBU_LOG1_INFO(      "NAS_EMM_SaveMrrcDataReqMsg,g_ulSaveMsgNum =",
                            g_ulSaveMsgNum);

    }
    return;
}
VOS_VOID  NAS_EMM_MrrcMgmtDataInit( VOS_VOID )
{
    VOS_UINT32                           ulIndex = 0;

    for (ulIndex = 0; ulIndex < NAS_LMM_RRC_SAVE_MSG_MAX_NUM; ulIndex++)
    {
        g_stEmmMrrcMgmtData[ulIndex].ulMmRrcOpId = NAS_LMM_RRC_OPID_NOT_VAILID;
        g_stEmmMrrcMgmtData[ulIndex].ulEsmMmOpId = NAS_LMM_OPID;
        g_stEmmMrrcMgmtData[ulIndex].enLastMsgId = NAS_EMM_CN_MT_BUTT;
        g_stEmmMrrcMgmtData[ulIndex].ulNasMsgLength = NAS_EMM_NULL;
        g_stEmmMrrcMgmtData[ulIndex].ulHoWaitSysInfoFlag = VOS_FALSE;
        NAS_LMM_MEM_SET(g_stEmmMrrcMgmtData[ulIndex].aucNasMsgContent,
                        NAS_EMM_NULL,
                        NAS_LMM_MRRC_NAS_MSG_MAX_LEN);
    }

    g_ulSaveMsgNum = 0;
    #if 0
    NAS_EMM_MRRC_MGMT_DATA_STRU*        pstNasEmmMrrcMgmtData
                                        =   NAS_EMM_NULL_PTR;
    pstNasEmmMrrcMgmtData               =   NAS_EMM_GET_MRRC_MGMT_DATA_ADDR();
    pstNasEmmMrrcMgmtData->enMrrcSendState
                                        =   NAS_EMM_MRRC_SEND_STATE_READY;

    pstNasEmmMrrcMgmtData->ulHoWaitSysInfoFlag = VOS_FALSE;


    pstNasEmmMrrcMgmtData->ulMmRrcOpId  =   NAS_LMM_RRC_OPID_NOT_VAILID;
    pstNasEmmMrrcMgmtData->ulEsmMmOpId  =   NAS_LMM_OPID;
    pstNasEmmMrrcMgmtData->enLastMsgId  =   NAS_EMM_CN_MT_BUTT;
    pstNasEmmMrrcMgmtData->ulNasMsgLength = NAS_EMM_NULL;
    NAS_LMM_MEM_SET(pstNasEmmMrrcMgmtData->aucNasMsgContent,
                    NAS_EMM_NULL,
                    NAS_LMM_MRRC_NAS_MSG_MAX_LEN);
    #endif
}


VOS_UINT32  NAS_EMM_MrrcAllocMmRrcOpId( VOS_VOID )
{
    gulMmRrcOpId++;
    if(gulMmRrcOpId                     == NAS_LMM_RRC_OPID_NOT_VAILID)
    {
        gulMmRrcOpId                    = 0;
    }
    return(                             gulMmRrcOpId);
}


NAS_EMM_MRRC_MGMT_DATA_STRU* NAS_EMM_FindMsgInDataReqBuffer(VOS_UINT32 ulOpId)
{
    VOS_UINT32                          ulIndex;
    VOS_UINT32                          ulRslt = NAS_EMM_FAIL;
    NAS_EMM_MRRC_MGMT_DATA_STRU         *pEmmMrrcMgmtData = NAS_EMM_NULL_PTR;

    pEmmMrrcMgmtData = NAS_EMM_GET_MRRC_MGMT_DATA_ADDR();

    for (ulIndex = 0; ulIndex <g_ulSaveMsgNum; ulIndex++)
    {
        if (ulOpId == pEmmMrrcMgmtData->ulMmRrcOpId)
        {
            ulRslt = NAS_EMM_SUCC;
            break;
        }
        else
        {
            pEmmMrrcMgmtData++;
        }
    }
    if (NAS_EMM_FAIL == ulRslt)
    {
        return NAS_EMM_NULL_PTR;
    }
    return pEmmMrrcMgmtData;
}



VOS_VOID  NAS_EMM_ClrDataReqBufferMsg
(
    VOS_UINT32                          ulMmRrcOpId
)
{
    NAS_EMM_MRRC_MGMT_DATA_STRU         *pEmmMrrcMgmtData = NAS_EMM_NULL_PTR;
    VOS_UINT32                          ulIndex;
    VOS_UINT32                          ulRrcMmDataReqMsgLen;

    NAS_EMM_PUBU_LOG1_INFO(      "NAS_EMM_ClrDataReqBufferMsg,g_ulSaveMsgNum =",
                            g_ulSaveMsgNum);

    pEmmMrrcMgmtData = NAS_EMM_GET_MRRC_MGMT_DATA_ADDR();

    for (ulIndex = 0; ulIndex < g_ulSaveMsgNum; ulIndex++)
    {
        if (ulMmRrcOpId == pEmmMrrcMgmtData->ulMmRrcOpId)
        {
            /* �����ǰ�Ѿ��洢6��������Ҫɾ���Ĳ��Ƿ��������ĩβ��
            ��Ҫ��Ϣǰ�ƣ�����ĩβ����Ϣ��� */
            if ((g_ulSaveMsgNum - 1)!=ulIndex)
            {
                ulRrcMmDataReqMsgLen = (sizeof(NAS_EMM_MRRC_MGMT_DATA_STRU))*(g_ulSaveMsgNum - (ulIndex+1));
                NAS_LMM_MEM_CPY(pEmmMrrcMgmtData,
                                (pEmmMrrcMgmtData+1),
                                ulRrcMmDataReqMsgLen);
            }

            /* ����ǰָ��ָ�򻺴������ĩβ */
            pEmmMrrcMgmtData = pEmmMrrcMgmtData + (g_ulSaveMsgNum - (ulIndex+1));

            pEmmMrrcMgmtData->enLastMsgId = NAS_EMM_CN_MT_BUTT;
            pEmmMrrcMgmtData->ulEsmMmOpId = NAS_LMM_OPID;
            pEmmMrrcMgmtData->ulMmRrcOpId = NAS_LMM_RRC_OPID_NOT_VAILID;
            pEmmMrrcMgmtData->ulHoWaitSysInfoFlag = VOS_FALSE;
            pEmmMrrcMgmtData->ulNasMsgLength = NAS_EMM_NULL;
            NAS_LMM_MEM_SET(pEmmMrrcMgmtData->aucNasMsgContent,
                            NAS_EMM_NULL,
                            NAS_LMM_MRRC_NAS_MSG_MAX_LEN);

            g_ulSaveMsgNum--;

            NAS_EMM_PUBU_LOG1_INFO(      "NAS_EMM_ClrDataReqBufferMsg,g_ulSaveMsgNum =",
                                    g_ulSaveMsgNum);

            for (ulIndex = 0; ulIndex < g_ulSaveMsgNum; ulIndex++)
            {
                NAS_EMM_PUBU_LOG1_INFO("NAS_EMM_ClrDataReqBufferMsg,g_stEmmMrrcMgmtData[].enLastMsgId =",
                                       g_stEmmMrrcMgmtData[ulIndex].enLastMsgId);

                NAS_EMM_PUBU_LOG1_INFO("NAS_EMM_ClrDataReqBufferMsg,g_stEmmMrrcMgmtData[].ulMmRrcOpId =",
                                       g_stEmmMrrcMgmtData[ulIndex].ulMmRrcOpId);

                NAS_EMM_PUBU_LOG1_INFO("NAS_EMM_ClrDataReqBufferMsg,g_stEmmMrrcMgmtData[].ulHoWaitSysInfoFlag =",
                                       g_stEmmMrrcMgmtData[ulIndex].ulHoWaitSysInfoFlag);
            }

            return;
        }
        pEmmMrrcMgmtData++;
    }
    return;
}
VOS_UINT32  NAS_EMM_ClrHoWaitSysInfoBufferMsg( VOS_VOID )
{
    NAS_EMM_MRRC_MGMT_DATA_STRU         *pEmmMrrcMgmtData = NAS_EMM_NULL_PTR;
    VOS_UINT32                          ulIndex;
    VOS_UINT32                          ulCurSaveNum;
    VOS_UINT32                          ulRslt = NAS_EMM_FAIL;
    VOS_UINT32                          ulRrcMmDataReqMsgLen;

    NAS_EMM_PUBU_LOG1_INFO(      "NAS_EMM_ClrHoWaitSysInfoBufferMsg,g_ulSaveMsgNum =",
                            g_ulSaveMsgNum);

    ulCurSaveNum = g_ulSaveMsgNum;

    pEmmMrrcMgmtData = NAS_EMM_GET_MRRC_MGMT_DATA_ADDR();

    for (ulIndex = 0; ulIndex < ulCurSaveNum; ulIndex++)
    {
        if (VOS_TRUE == pEmmMrrcMgmtData->ulHoWaitSysInfoFlag)
        {
            /* �����ǰ�Ѿ��洢6��������Ҫɾ�����Ƿ��������ĩβ��
            ����Ҫ��Ϣǰ�ƣ�ֱ��ɾ������*/
            if ((ulCurSaveNum - 1)!=ulIndex)
            {
                ulRrcMmDataReqMsgLen = (sizeof(NAS_EMM_MRRC_MGMT_DATA_STRU))*(ulCurSaveNum - (ulIndex+1));
                NAS_LMM_MEM_CPY(pEmmMrrcMgmtData,
                                (pEmmMrrcMgmtData+1),
                                ulRrcMmDataReqMsgLen);
            }
            /* ����ǰָ��ָ�򻺴������ĩβ */
            pEmmMrrcMgmtData = pEmmMrrcMgmtData + (ulCurSaveNum - (ulIndex+1));

            pEmmMrrcMgmtData->enLastMsgId = NAS_EMM_CN_MT_BUTT;
            pEmmMrrcMgmtData->ulEsmMmOpId = NAS_LMM_OPID;
            pEmmMrrcMgmtData->ulMmRrcOpId = NAS_LMM_RRC_OPID_NOT_VAILID;
            pEmmMrrcMgmtData->ulHoWaitSysInfoFlag = VOS_FALSE;
            pEmmMrrcMgmtData->ulNasMsgLength = NAS_EMM_NULL;
            NAS_LMM_MEM_SET(pEmmMrrcMgmtData->aucNasMsgContent,
                            NAS_EMM_NULL,
                            NAS_LMM_MRRC_NAS_MSG_MAX_LEN);

            pEmmMrrcMgmtData = pEmmMrrcMgmtData - (ulCurSaveNum - (ulIndex+1));

            g_ulSaveMsgNum--;

            ulRslt = NAS_EMM_SUCC;

        }
        else
        {
            pEmmMrrcMgmtData++;
        }

    }

    NAS_EMM_PUBU_LOG1_INFO(      "NAS_EMM_ClrHoWaitSysInfoBufferMsg,g_ulSaveMsgNum =",
                            g_ulSaveMsgNum);

    for (ulIndex = 0; ulIndex < g_ulSaveMsgNum; ulIndex++)
    {
        NAS_EMM_PUBU_LOG1_INFO("NAS_EMM_ClrHoWaitSysInfoBufferMsg,g_stEmmMrrcMgmtData[].enLastMsgId =",
                               g_stEmmMrrcMgmtData[ulIndex].enLastMsgId);

        NAS_EMM_PUBU_LOG1_INFO("NAS_EMM_ClrHoWaitSysInfoBufferMsg,g_stEmmMrrcMgmtData[].ulMmRrcOpId =",
                               g_stEmmMrrcMgmtData[ulIndex].ulMmRrcOpId);

        NAS_EMM_PUBU_LOG1_INFO("NAS_EMM_ClrHoWaitSysInfoBufferMsg,g_stEmmMrrcMgmtData[].ulHoWaitSysInfoFlag =",
                               g_stEmmMrrcMgmtData[ulIndex].ulHoWaitSysInfoFlag);
    }

    return ulRslt;
}
VOS_VOID  NAS_EMM_ClrAllUlDataReqBufferMsg( VOS_VOID )
{
    NAS_EMM_MRRC_MGMT_DATA_STRU         *pEmmMrrcMgmtData = NAS_EMM_NULL_PTR;
    VOS_UINT32                          ulIndex;

    NAS_EMM_PUBU_LOG1_INFO(      "NAS_EMM_ClrAllUlDataReqBufferMsg,g_ulSaveMsgNum =",
                            g_ulSaveMsgNum);

    pEmmMrrcMgmtData                    = NAS_EMM_GET_MRRC_MGMT_DATA_ADDR();

    for (ulIndex = 0; ulIndex < g_ulSaveMsgNum; ++ulIndex)
    {
        pEmmMrrcMgmtData->enLastMsgId = NAS_EMM_CN_MT_BUTT;
        pEmmMrrcMgmtData->ulEsmMmOpId = NAS_LMM_OPID;
        pEmmMrrcMgmtData->ulMmRrcOpId = NAS_LMM_RRC_OPID_NOT_VAILID;
        pEmmMrrcMgmtData->ulHoWaitSysInfoFlag = VOS_FALSE;
        pEmmMrrcMgmtData->ulNasMsgLength = NAS_EMM_NULL;
        NAS_LMM_MEM_SET(pEmmMrrcMgmtData->aucNasMsgContent,
                        NAS_EMM_NULL,
                        NAS_LMM_MRRC_NAS_MSG_MAX_LEN);
    }
    g_ulSaveMsgNum = 0;

    NAS_EMM_PUBU_LOG1_INFO(      "NAS_EMM_ClrAllUlDataReqBufferMsg,g_ulSaveMsgNum =",
                            g_ulSaveMsgNum);
}

VOS_VOID  NAS_EMM_ClrAllUlDataReqBufferMsgExceptMtDetachAcc( VOS_VOID )
{
    NAS_EMM_MRRC_MGMT_DATA_STRU         *pEmmMrrcMgmtData = NAS_EMM_NULL_PTR;
    VOS_UINT32                          ulIndex;
    VOS_UINT32                          ulCurSaveNum;
    VOS_UINT32                          ulRrcMmDataReqMsgLen;

    NAS_EMM_PUBU_LOG1_INFO(      "NAS_EMM_ClrAllUlDataReqBufferMsgExceptMtDetachAcc,g_ulSaveMsgNum =",
                            g_ulSaveMsgNum);

    ulCurSaveNum = g_ulSaveMsgNum;

    pEmmMrrcMgmtData = NAS_EMM_GET_MRRC_MGMT_DATA_ADDR();

    for (ulIndex = 0; ulIndex < ulCurSaveNum; ulIndex++)
    {
        if (NAS_EMM_MSG_DETACH_ACP != pEmmMrrcMgmtData->enLastMsgId)
        {
            /* �����ǰ�Ѿ��洢6��������Ҫɾ�����Ƿ��������ĩβ��
            ����Ҫ��Ϣǰ�ƣ�ֱ��ɾ������*/
            if ((ulCurSaveNum - 1)!=ulIndex)
            {
                ulRrcMmDataReqMsgLen = (sizeof(NAS_EMM_MRRC_MGMT_DATA_STRU))*(ulCurSaveNum - (ulIndex+1));
                NAS_LMM_MEM_CPY(pEmmMrrcMgmtData,
                                (pEmmMrrcMgmtData+1),
                                ulRrcMmDataReqMsgLen);
            }
            /* ����ǰָ��ָ�򻺴������ĩβ */
            pEmmMrrcMgmtData = pEmmMrrcMgmtData + (ulCurSaveNum - (ulIndex+1));

            pEmmMrrcMgmtData->enLastMsgId = NAS_EMM_CN_MT_BUTT;
            pEmmMrrcMgmtData->ulEsmMmOpId = NAS_LMM_OPID;
            pEmmMrrcMgmtData->ulMmRrcOpId = NAS_LMM_RRC_OPID_NOT_VAILID;
            pEmmMrrcMgmtData->ulHoWaitSysInfoFlag = VOS_FALSE;
            pEmmMrrcMgmtData->ulNasMsgLength = NAS_EMM_NULL;
            NAS_LMM_MEM_SET(pEmmMrrcMgmtData->aucNasMsgContent,
                            NAS_EMM_NULL,
                            NAS_LMM_MRRC_NAS_MSG_MAX_LEN);

            pEmmMrrcMgmtData = pEmmMrrcMgmtData - (ulCurSaveNum - (ulIndex+1));

            g_ulSaveMsgNum--;

        }
        else
        {
            pEmmMrrcMgmtData++;
        }

    }
    NAS_EMM_PUBU_LOG1_INFO(      "NAS_EMM_ClrAllUlDataReqBufferMsgExceptMtDetachAcc,g_ulSaveMsgNum =",
                            g_ulSaveMsgNum);

    for (ulIndex = 0; ulIndex < g_ulSaveMsgNum; ulIndex++)
    {
        NAS_EMM_PUBU_LOG1_INFO("NAS_EMM_ClrAllUlDataReqBufferMsgExceptMtDetachAcc,g_stEmmMrrcMgmtData[].enLastMsgId =",
                               g_stEmmMrrcMgmtData[ulIndex].enLastMsgId);

        NAS_EMM_PUBU_LOG1_INFO("NAS_EMM_ClrAllUlDataReqBufferMsgExceptMtDetachAcc,g_stEmmMrrcMgmtData[].ulMmRrcOpId =",
                               g_stEmmMrrcMgmtData[ulIndex].ulMmRrcOpId);

        NAS_EMM_PUBU_LOG1_INFO("NAS_EMM_ClrAllUlDataReqBufferMsgExceptMtDetachAcc,g_stEmmMrrcMgmtData[].ulHoWaitSysInfoFlag =",
                               g_stEmmMrrcMgmtData[ulIndex].ulHoWaitSysInfoFlag);
    }

    return;
}


VOS_UINT32  NAS_EMM_MsRrcConnEstInitSsWaitRrcConnMsgSysinfo(
                                        VOS_UINT32  ulMsgId,
                                        VOS_VOID   *pMsgStru )
{
    VOS_UINT32                          ulCompStaOfStackBase;
    VOS_UINT32                          ulRslt = NAS_LMM_MSG_DISCARD;
    NAS_EMM_TAU_START_CAUSE_ENUM_UINT8  enTauStartCause;
    EMMC_EMM_SYS_INFO_IND_STRU          *pstsysinfo    = NAS_EMM_NULL_PTR;

    NAS_EMM_GIM_NORMAL_LOG("NAS_EMM_MsRrcConnEstInitSsWaitRrcConnMsgSysinfo entered.");
    (VOS_VOID)(ulMsgId);
#if 0
    /* �洢��ȫ�ֱ����� */
    NAS_LMM_CountSysInfo(EMM_SS_RRC_CONN_WAIT_EST_CNF);

    return  NAS_LMM_STORE_HIGH_PRIO_MSG;
#else
    pstsysinfo                          = (EMMC_EMM_SYS_INFO_IND_STRU*)pMsgStru;

    /*���ϵͳ��Ϣ�������򻺴棬��������̾��崦��*/
    if((EMMC_EMM_NO_FORBIDDEN != pstsysinfo->ulForbiddenInfo)
        || (EMMC_EMM_CELL_STATUS_ANYCELL == pstsysinfo->ulCellStatus))
    {
        return  NAS_LMM_STORE_HIGH_PRIO_MSG;
    }

    /*ϵͳ��Ϣ������������ѹջ��״̬��ϵͳ��Ϣ���в�ͬ����:
      1)ATTACH����:ֱ�Ӷ���
      2)Service��DETACH���̣������ȼ�����
      3)TAU���̣�������TAU��Ta�仯�򻺴棬����ֱ�Ӷ���*/
    ulCompStaOfStackBase = NAS_LMM_FSM_GetStaAtStackBase(NAS_LMM_PARALLEL_FSM_EMM);
    switch(ulCompStaOfStackBase)
    {
        case NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_REG_INIT,
                                      EMM_SS_ATTACH_WAIT_CN_ATTACH_CNF):
            ulRslt = NAS_LMM_MSG_DISCARD;
            break;

        case NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_SER_INIT,
                                      EMM_SS_SER_WAIT_CN_SER_CNF):
        case NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_DEREG_INIT,
                                      EMM_SS_DETACH_WAIT_CN_DETACH_CNF):
            ulRslt = NAS_LMM_STORE_HIGH_PRIO_MSG;
            break;

        case NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_TAU_INIT,
                                      EMM_SS_TAU_WAIT_CN_TAU_CNF):
            /*�ж���������TAU�򻺴�,������*/
            enTauStartCause = NAS_EMM_TAU_GetTauStartCause();
            if (NAS_EMM_TAU_START_CAUSE_T3412EXP == enTauStartCause)
            {
                ulRslt = NAS_LMM_STORE_HIGH_PRIO_MSG;
            }
            else
            {
                ulRslt = NAS_LMM_MSG_DISCARD;
            }
            break;

         default:
            NAS_EMM_GIM_WARNING_LOG("NAS_EMM_MsRrcConnEstInitSsWaitRrcConnMsgSysinfo:\
                                        Sta Err.");
            break;
    }

    return ulRslt;
#endif
}
VOS_UINT32  NAS_EMM_MsRrcConnEstInitSsWaitRrcConnMsgT3412Exp( VOS_UINT32  ulMsgId,
                                                              VOS_VOID   *pMsgStru  )
{
    /* ��ӡ����ú����� NORM_LEVEL */
    NAS_EMM_TAU_LOG_NORM("NAS_EMM_MsRrcConnEstInitSsWaitRrcConnMsgT3412Exp: Receive Message T3412 Exp.");

    (VOS_VOID)(ulMsgId);
    (VOS_VOID)(pMsgStru);

    return  NAS_LMM_STORE_HIGH_PRIO_MSG;
}
VOS_UINT32  NAS_EMM_MsRrcConnEstInitWaitRrcConnMsgEsmDataReq (VOS_UINT32  ulMsgId,
                                                              VOS_VOID   *pMsgStru  )
{
    EMM_ESM_DATA_REQ_STRU       *pstsmdatareq = NAS_EMM_NULL_PTR;

    /* ��ӡ����ú����� NORM_LEVEL */
    NAS_EMM_TAU_LOG_NORM("NAS_EMM_MsRrcConnEstInitWaitRrcConnMsgEsmDataReq: Receive Message ESM DATA REQ.");

    (VOS_VOID)(ulMsgId);


    /* (1)�������֮ǰ��SER_INIT+WAIT_CN_CNF״̬,�򻺴�ESM��Ϣ*/
    /* (2)�������֮ǰ��TAU_INIT+WAIT_CN_CNF״̬,��TAU�Ǵ��SERVICE���̷����,�򻺴�ESM��Ϣ*/
    if ((NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_SER_INIT, EMM_SS_SER_WAIT_CN_SER_CNF)
        == NAS_LMM_FSM_GetStaAtStackBase(NAS_LMM_PARALLEL_FSM_EMM))
        || ((NAS_LMM_PUB_COMP_EMMSTATE(EMM_MS_TAU_INIT, EMM_SS_TAU_WAIT_CN_TAU_CNF)
        == NAS_LMM_FSM_GetStaAtStackBase(NAS_LMM_PARALLEL_FSM_EMM))
        && (NAS_EMM_COLLISION_SERVICE == NAS_EMM_TAU_GetEmmCollisionCtrl())))
    {
        /*SERģ�����л���ESM DATA��Ϣ*/
        pstsmdatareq = (EMM_ESM_DATA_REQ_STRU        *)pMsgStru;
        /* lihong00150010 emergency tau&service begin */
        if (VOS_TRUE == pstsmdatareq->ulIsEmcType)
        {
            NAS_LMM_SetEmmInfoIsEmerPndEsting(VOS_TRUE);

            /* ����������͵�ESM��Ϣ */
            NAS_EMM_SaveEmcEsmMsg(pMsgStru);
        }
        /* lihong00150010 emergency tau&service end */
        NAS_EMM_SER_SaveEsmMsg(pstsmdatareq);
    }

    return  NAS_LMM_MSG_HANDLED;
}
/*lint +e961*/
/*lint +e960*/

VOS_UINT32  NAS_EMM_PreProcMsgRabmDrbSetupInd( MsgBlock * pMsg )
{
    (VOS_VOID)pMsg;
    NAS_EMM_GIM_INFO_LOG("NAS_EMM_PreProcMsgRabmDrbSetupInd entered.");

    /*������,ֻ��RRC��·Ϊ��������̬ʱ,����·״̬�л�����������̬ */
    if (NAS_EMM_NO == NAS_LMM_IsRrcConnectStatusIdle())
    {
        NAS_EMM_ChangeRrcConnStateToEstData();
    }
    else
    {
        NAS_EMM_GIM_ERROR_LOG("NAS_EMM_PreProcMsgRabmDrbSetupInd: idle.");
    }

    return NAS_LMM_MSG_DISCARD;
}



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


/* end of ��ϵͳ+ģ��+�ļ���.c */
