

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include  "NasEmmcRcvMsg.h"
#include  "NasEmmcPublic.h"
#include  "NasEmmcSendMsg.h"
#include  "NasEmmcMain.h"
#include  "NasEmmPubUResume.h"
#include  "NasLmmPubMPrint.h"
#include  "NasLmmPubMTimer.h"
#include  "NasEmmTauSerInclude.h"
#include  "NasEmmAttDetInclude.h"

#include "DrvInterface.h"
/*end*/

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASEMMCRCVMSG_C
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


VOS_UINT32  NAS_EMMC_RcvEmmStartReq( MsgBlock *pMsg )
{
    NAS_LMM_EMMC_LOG_NORM("NAS_EMMC_RcvEmmStartReq is entered.");

	(void)pMsg;

    /* ��μ�� */
    if ( NAS_EMMC_NULL_PTR == pMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvEmmStartReq:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    NAS_EMMC_Init();

    /*����EMMC_EMM_START_CNF��Ϣ*/
    NAS_EMMC_SendEmmStartCnf();

    return NAS_LMM_MSG_HANDLED;
}



VOS_UINT32  NAS_EMMC_RcvEmmStopReq( MsgBlock *pMsg )
{
    NAS_LMM_EMMC_LOG_NORM("NAS_EMMC_RcvEmmStopReq is entered.");
    (void)pMsg;

    /* ��μ�� */
    if ( NAS_EMMC_NULL_PTR == pMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvEmmStopReq:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /*����EMMC_EMM_START_CNF��Ϣ*/
    NAS_EMMC_SendEmmStopCnf();

    return NAS_LMM_MSG_HANDLED;
}



/*lint -e960*/
/*lint -e961*/
VOS_UINT32  NAS_EMMC_RcvMmcPlmnSrchReq(MsgBlock *pMsg )
{

    MMC_LMM_PLMN_SRCH_REQ_STRU         *pstMmcPlmnReq = NAS_EMMC_NULL_PTR;
    LRRC_LNAS_PLMN_SEARCH_TYPE_ENUM_UINT32   ulPlmnType;
#if (VOS_OS_VER != VOS_WIN32)
    static VOS_UINT32  s_ulPlmnSrchReqCnt = 1;
#endif

    NAS_LMM_EMMC_LOG_NORM("NAS_EMMC_RcvMmcPlmnSrchReq is entered.");

    /* ��μ�� */
    if ( NAS_EMMC_NULL_PTR == pMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvMmcPlmnSrchReq:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /* �ṹת�� */
    pstMmcPlmnReq = (MMC_LMM_PLMN_SRCH_REQ_STRU *)pMsg;

    /* ����������� */
    ulPlmnType = NAS_LMM_MmcPlmnReqTypeCheck(pstMmcPlmnReq->enSrchType);
    if (LRRC_LNAS_PLMN_SEARCH_TYPE_BUTT == ulPlmnType)
    {
        NAS_LMM_EMMC_LOG_ERR(" NAS_EMMC_RcvMmcPlmnSrchReq: PLMN TYPE IS ERR!");
        return NAS_LMM_MSG_HANDLED;
    }

    /*�ǿ���̬ʱ���ظ�LIST�����ܾ�.
    LMM�����Ƿ��ڿ���̬��MMC����ֱ���·�MMC_LMM_PLMN_SEARCH_REQҪ��LMM����
    LIST������
    1) ��LMM���ڷǿ���̬��LMM�ظ�LMM_MMC_PLMN_SEARCH_CNF��
    ���Ϊ LIST�����ܾ�;
    2) ���MMC�ڿ���̬����MMC_LMM_PLMN_SEARCH_REQ�������LMM�յ�����Ϣʱת����
    ���������У�����������ΪLIST,��LMM�ظ�MMC_LMM_PLMN_SEARCH_CNF�����Ϊ
    LIST�����ܾ���
    20130603 BEGIN */
    /*
    if((LRRC_LNAS_PLMN_SEARCH_LIST == ulPlmnType)
        && (NAS_EMM_CONN_ESTING == NAS_EMM_GetConnState()))
    {
        NAS_LMM_EMMC_LOG_INFO(" NAS_EMMC_RcvMmcPlmnSrchReq: PLMN LIST TYPE IS REJECT!");
        NAS_EMMC_SendMmcPlmnListSrchRej();
        return NAS_LMM_MSG_HANDLED;
    }
    */

    /* �����ǰ����IDLE̬ �� NAS_EMM_CONN_RELEASING */
    /*lint -e539*/
    /*lint -e830*/
    if ((NAS_EMM_CONN_IDLE != NAS_EMM_GetConnState())
        && (NAS_EMM_CONN_RELEASING != NAS_EMM_GetConnState()))
    {
         /* ����list���� , ��ظ�list�ܾ� */
        if(LRRC_LNAS_PLMN_SEARCH_LIST == ulPlmnType)
        {
            NAS_LMM_EMMC_LOG_INFO(" NAS_EMMC_RcvMmcPlmnSrchReq: PLMN LIST TYPE IS REJECT!");
            NAS_EMMC_SendMmcPlmnListSrchRej();
        }
        else
        {   /* ������ list �������� ����������������Ϣ */
            NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvMmcPlmnSrchReq:Not Idle,Discard!");
        }

        return NAS_LMM_MSG_HANDLED;
    }
    NAS_LMM_EMMC_LOG_INFO(" NAS_EMMC_RcvMmcPlmnSrchReq: PLMN SEARCH BEGIN!");
    /*lint +e830*/
    /*lint +e539*/

#if (VOS_OS_VER != VOS_WIN32)
    if(1 == s_ulPlmnSrchReqCnt)
    {
        (VOS_VOID) ddmPhaseScoreBoot("LTE Cell search start",__LINE__);
        s_ulPlmnSrchReqCnt++;
    }
#endif
    /*end*/

    /* �ṹת�� */
    pstMmcPlmnReq = (MMC_LMM_PLMN_SRCH_REQ_STRU *)pMsg;

    /* �����ǰ���뼼��û��L����LTE��DISABLED����ֻ��ӦLIST��������������ֱ�ӷ��� */
    if((NAS_LMM_FAIL == NAS_EMM_lteRatIsExist())
     ||(NAS_LMM_SUCC == NAS_EMM_IsLteDisabled()))
    {
        if(MMC_LMM_PLMN_SRCH_LIST != pstMmcPlmnReq->enSrchType)
        {
            NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvMmcPlmnSrchReq: LTE IS NOT EXIST or LTE DISABLED");
            return NAS_LMM_MSG_HANDLED;
        }
    }

    if (MMC_LMM_PLMN_SRCH_USER_SPEC == pstMmcPlmnReq->enSrchType)
    {
        NAS_EMMC_SetUserSpecPlmnFlag(NAS_EMMC_USER_SPEC_PLMN_YES);
    }

    if (pstMmcPlmnReq->enSrchType != MMC_LMM_PLMN_SRCH_LIST)
    {
        /*����EMMC_EMM_PLMN_IND��Ϣ*/
        NAS_EMMC_SendEmmPlmnInd();
    }

    /*֮ǰ���߼���ϵͳ��Ϣ�����仯����ϱ�λ�ñ����Ϣ��ͨ��ע��״̬�ϱ���Ϣ����
      ����SVLTE��ĿPS���ڲ�ͬmodem��Ǩ������������ΪֻҪMMC�������������Ҳ���list�ѣ�����
      �ѵ�С���Ƿ�仯����Ҫ�ϱ�ע��״̬��
      �����MMC������ʱ������������ǣ��յ�ϵͳ��Ϣ���ϱ�ע��״̬��������������*/
    if((MMC_LMM_PLMN_SRCH_LIST != pstMmcPlmnReq->enSrchType)
        || (MMC_LMM_PLMN_SRCH_TYPE_BUTT != pstMmcPlmnReq->enSrchType))
    {
        NAS_LMM_EMMC_LOG_INFO("NAS_EMMC_RcvMmcPlmnSrchReq: set single plmn search flag!");
        NAS_EMMC_SetSinglePlmnSrchFlag(NAS_EMM_YES);
    }

    /*����RRC_MM_PLMN_SEARCH_REQ��Ϣ*/
    NAS_EMMC_SendRrcPlmnSrchReq(pstMmcPlmnReq);

    /* ����lIST��ʱ������������� */
    if (VOS_FALSE == NAS_EMMC_IsBgListSearch(pstMmcPlmnReq->enSrchType))
    {
        NAS_EMMC_SetPlmnSrchFlag(NAS_EMM_PLMN_SRCH_FLAG_VALID);
        NAS_LMM_StartStateTimer(TI_NAS_EMMC_STATE_WAIT_PLMN_SRCH_CNF_TIMER);
    }

    return NAS_LMM_MSG_HANDLED;

}






VOS_UINT32  NAS_EMMC_RcvRrcPlmnSrchCnf (MsgBlock *pMsg)
{
    LRRC_LMM_PLMN_SEARCH_CNF_STRU         *pstPlmnCnf = NAS_EMMC_NULL_PTR;

    NAS_LMM_EMMC_LOG_NORM("NAS_EMMC_RcvRrcPlmnSrchCnf is entered.");

    /* ��μ�� */
    if ( NAS_EMMC_NULL_PTR == pMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvRrcPlmnSrchCnf:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /* ���������������ʶ������� */
    if (NAS_EMM_PLMN_SRCH_FLAG_VALID == NAS_EMMC_GetPlmnSrchFlag())
    {
        NAS_EMMC_SetPlmnSrchFlag(NAS_EMM_PLMN_SRCH_FLAG_INVALID);
        NAS_LMM_StopStateTimer(TI_NAS_EMMC_STATE_WAIT_PLMN_SRCH_CNF_TIMER);
    }

    /* �ṹת�� */
    pstPlmnCnf = (LRRC_LMM_PLMN_SEARCH_CNF_STRU *)pMsg;

    /*����LMM_MMC_PLMN_SRCH_CNF��Ϣ*/
    NAS_EMMC_SendMmcPlmnSrchCnf(pstPlmnCnf);

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32 NAS_LMM_PreProcMmcLteSysInfoInd(MsgBlock *pMsg )
{
    MMC_LMM_LTE_SYS_INFO_IND_STRU      *pstLteSysInfoInd = NAS_EMMC_NULL_PTR;
    EMMC_EMM_CHANGE_INFO_ENUM_UINT32    ulChangeInfo;
    VOS_UINT32                          ulTaCellIdChangeInfo;
    NAS_EMM_REG_CHANGE_TYPE_ENUM_UINT32 ulChangeType;
    NAS_EMM_REJ_FLAG_ENUM_UINT32        ulMatchRslt;
    /* lihong00150010 emergency tau&service begin */
    EMMC_EMM_CELL_STATUS_ENUM_UINT32    enCellStatus = EMMC_EMM_CELL_STATUS_NORMAL;
    EMMC_EMM_FORBIDDEN_INFO_ENUM_UINT32 enForbdInfo = EMMC_EMM_NO_FORBIDDEN;
    /* lihong00150010 emergency tau&service end */

    NAS_LMM_EMMC_LOG_NORM("NAS_LMM_PreProcMmcLteSysInfoInd is entered.");

    /* ��μ�� */
    if ( NAS_EMMC_NULL_PTR == pMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_LMM_PreProcMmcLteSysInfoInd:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /* ����Ǵ�ģ������ */
    if(NAS_EMM_IS_SUSPEND == NAS_EMM_IsSuspended())
    {
        NAS_LMM_EMMC_LOG_NORM("NAS_LMM_PreProcMmcLteSysInfoInd:EMM_MS_RRC_CONN_REL_INIT is discard");
        return NAS_LMM_MSG_HANDLED;
    }

    /* �ṹת�� */
    pstLteSysInfoInd = (MMC_LMM_LTE_SYS_INFO_IND_STRU *)pMsg;

    /* ĿǰRRC��NAS�ϱ���ϵͳ��Ϣ�����г�����astSuitPlmnList����ֻ�ϱ�1��PLMN,
       stSpecPlmnIdList�е�ulSuitPlmnNum Ҳֻ��Ĺ̶�ֵΪ1*/

    /*TA����CELL ID �ı�*/
    NAS_EMMC_ProcessMmcLteSysInfoIndTaOrCellID( &pstLteSysInfoInd->stLteSysInfo,
                                                &ulTaCellIdChangeInfo);

    /*ϵͳ��Ϣ�е�PLMN�Ƿ��ھܾ�#18�б���*/
    ulMatchRslt = NAS_EMMC_PlmnMatchRej18PlmnList((NAS_MM_PLMN_ID_STRU *)&pstLteSysInfoInd->stLteSysInfo.stSpecPlmnIdList.astSuitPlmnList[0]);

    /*���õ�ǰPLMN�Ƿ��ھܾ�#18�б��еı�ʶ*/
    NAS_EMMC_SetRejCause18Flag(ulMatchRslt);

    NAS_LMM_EMMC_LOG1_NORM("NAS_LMM_PreProcMmcLteSysInfoInd:ulMatchRslt = ",
                                ulMatchRslt);

    if ((NAS_EMM_REJ_YES == ulMatchRslt) &&
        (NAS_LMM_REG_DOMAIN_CS_PS == NAS_LMM_GetEmmInfoRegDomain()))
    {
        NAS_LMM_EMMC_LOG_NORM("NAS_LMM_PreProcMmcLteSysInfoInd:Reg Domain update");
        NAS_LMM_SetEmmInfoRegDomain(NAS_LMM_REG_DOMAIN_PS);
    }

    /* �ж�ϵͳ��Ϣ�Ƿ����仯*/
    /* lihong00150010 emergency tau&service begin */
    NAS_EMMC_ProcessMmcLteSysInfoInd(   &pstLteSysInfoInd->stLteSysInfo,
                                        &ulChangeInfo,
                                        &enCellStatus,
                                        &enForbdInfo);
    /* lihong00150010 emergency tau&service end */

/* lihong00150010 csfb begin */
    /* �����ǰPLMN��MCC��MML�д洢��MCC��ͬ����ɾ�������������б�ͬʱָʾMMģ��
       �����������б����Ϊ0��*/
    if (VOS_TRUE == NAS_EMM_IsMccChanged())
    {
        NAS_EMM_ClearEmergencyList();

        /* lihong00150010 ims begin */
        /* ֪ͨMMC�����������б������Ƿ�֧��IMS VOICE�ͽ��������Լ�LTE��CS������ */
        /* NAS_EMM_SendGmmInfoChangeNotify(); */
        NAS_EMM_SendMmcInfoChangeNotify();
        /*NAS_EMM_SendMmInfoChangeNotify();*/
        /* lihong00150010 ims end */
    }
/* lihong00150010 csfb end */
    /* lihong00150010 emergency tau&service begin */
    NAS_EMMC_SendEmmSysInfoInd(ulChangeInfo,enCellStatus,enForbdInfo);
    /* lihong00150010 emergency tau&service end */

    if (EMMC_EMM_NO_CHANGE != ulChangeInfo)
    {
        NAS_LMM_EMMC_LOG_NORM("NAS_LMM_PreProcMmcLteSysInfoInd:Sys info is change");

        /*��APP����APP_MM_REG_STAT_IND��Ϣ*/
        NAS_EMM_AppRegStatInd(NAS_LMM_GetNasAppRegState(),NAS_LMM_GetEmmCurFsmMS());
    }

    /* ���TA����CELL ID �ı䣬��MMC�ϱ�ע��״̬�ı�ָʾ��Ϣ */
    if ( ( NAS_EMMC_LA_INFO_MASK_SEC_BIT == (ulTaCellIdChangeInfo & NAS_EMMC_LA_INFO_MASK_SEC_BIT))
       ||( NAS_EMMC_LA_INFO_MASK_THD_BIT == (ulTaCellIdChangeInfo & NAS_EMMC_LA_INFO_MASK_THD_BIT))
       /*ֻҪ��������list���͵�����������С���Ƿ�仯�����ϱ�ע��״̬*/
       ||( NAS_EMM_YES == NAS_EMMC_GetSinglePlmnSrchFlag()))
    {
        ulChangeType = NAS_EMM_REG_CHANGE_TYPE_LOCATION_INFO;

        NAS_LMM_EMMC_LOG_NORM("NAS_EMMC_RcvRrcSysInfoInd:TAC OR CellId info is change");

        /*��MMC�ϱ�ID_LMM_MMC_REGISTER_STATUS_IND��Ϣ*/
        NAS_EMM_SendMmcRegisterStatInd(ulChangeType);

        /*�ϱ���ע��״̬��Ϣ������������*/
        NAS_EMMC_SetSinglePlmnSrchFlag(NAS_EMM_NO);
    }

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32  NAS_EMMC_RcvRrcSysInfoInd(MsgBlock *pMsg )
{
    LRRC_LMM_SYS_INFO_IND_STRU         *pstSysInfoInd = NAS_EMMC_NULL_PTR;

    NAS_LMM_EMMC_LOG_NORM("NAS_EMMC_RcvRrcSysInfoInd is entered.");

    /* ��μ�� */
    if ( NAS_EMMC_NULL_PTR == pMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvRrcSysInfoInd:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /* ��·�ͷŹ����к�AUTH_INIT״̬���������յ�ϵͳ��Ϣ��ֱ�Ӷ��� */
    if ((EMM_MS_RRC_CONN_REL_INIT == NAS_LMM_GetEmmCurFsmMS())
        || (EMM_MS_AUTH_INIT == NAS_LMM_GetEmmCurFsmMS()))
    {
        NAS_LMM_EMMC_LOG_NORM("NAS_EMMC_RcvRrcSysInfoInd:EMM_MS_RRC_CONN_REL_INIT is discard");
        return NAS_LMM_MSG_HANDLED;
    }

    /* �ṹת�� */
    pstSysInfoInd = (LRRC_LMM_SYS_INFO_IND_STRU *)pMsg;

    /* ���Not Camp On��ʶ */
    NAS_EMMC_SetNotCampOnFlag(VOS_FALSE);

    /*����LMM_MMC_SYS_INFO_IND��Ϣ*/
    NAS_EMMC_SendMmcSysInfo(pstSysInfoInd);

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32  NAS_EMMC_RcvMmcEplmnNotifyReq(MsgBlock *pMsg)
{
    MMC_LMM_EPLMN_NOTIFY_REQ_STRU       *pstEplmnReq;

    NAS_LMM_EMMC_LOG_NORM("NAS_EMMC_RcvMmcEplmnNotifyReq is entered.");

    /* ��μ�� */
    if ( NAS_EMMC_NULL_PTR == pMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvMmcEplmnNotifyReq:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    pstEplmnReq                        = (MMC_LMM_EPLMN_NOTIFY_REQ_STRU *)pMsg;


    /*����RRC_MM_EQU_PLMN_NOTIFY_REQ��Ϣ*/
    NAS_EMMC_SendRrcEplmnNotifyReq(pstEplmnReq);

    if(NAS_LMM_CUR_LTE_SUSPEND == NAS_EMM_GetCurLteState())
    {
        NAS_LMM_EMMC_LOG_INFO("NAS_EMMC_RcvMmcEplmnNotifyReq: SEND FORB_TA TO LRRC.");
        NAS_EMMC_SendRrcCellSelectionReq(LRRC_LNAS_FORBTA_CHANGE);
    }

    return NAS_LMM_MSG_HANDLED;
}



VOS_UINT32  NAS_EMMC_RcvMmcStopSrchReq( MsgBlock *pMsg)
{
    NAS_LMM_EMMC_LOG_NORM("NAS_EMMC_RcvMmcStopSrchReq is entered.");

    (void)pMsg;

    /* ��μ�� */
    if ( NAS_EMMC_NULL_PTR == pMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvMmcStopSrchReq:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /* ���������������ʶ������� */
    if (NAS_EMM_PLMN_SRCH_FLAG_VALID == NAS_EMMC_GetPlmnSrchFlag())
    {
        NAS_EMMC_SetPlmnSrchFlag(NAS_EMM_PLMN_SRCH_FLAG_INVALID);
        NAS_LMM_StopStateTimer(TI_NAS_EMMC_STATE_WAIT_PLMN_SRCH_CNF_TIMER);
    }

    /*����RRC_MM_PLMN_SEARCH_STOP_REQ��Ϣ*/
    NAS_EMMC_SendRrcPlmnSrchStopReq();

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32  NAS_EMMC_RcvRrcPlmnSrchStopCnf(MsgBlock *pMsg)
{

    NAS_LMM_EMMC_LOG_NORM("NAS_EMMC_RcvRrcPlmnSrchStopCnf is entered.");

    (void)pMsg;

    /* ��μ�� */
    if ( NAS_EMMC_NULL_PTR == pMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvRrcPlmnSrchStopCnf:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /*����LMM_MMC_STOP_SRCH_CNF��Ϣ*/
    NAS_EMMC_SendMmcStopSrchCnf();

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32  NAS_EMMC_RcvRrcAreaLostInd(MsgBlock *pMsg )
{
    NAS_LMM_EMMC_LOG_NORM("NAS_EMMC_RcvRrcAreaLostInd is entered.");

    (void)pMsg;

    /* ��μ�� */
    if ( NAS_EMMC_NULL_PTR == pMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvRrcAreaLostInd:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /* �յ�area lost ����*/
    NAS_EMMC_ProcessRrcAreaLostInd();

    /*����ID_EMMC_EMM_COVERAGE_LOST_IND��Ϣ*/
    NAS_EMMC_SendEmmCoverageLostInd();

    /* �����ǰCSFB��ʱ��ʱ�������У�˵����REG-NORMAL̬���ͷŹ������յ�CSFB��
       �������ͷź���С�����ֶ�������ʱӦ����ȥGU��������CSFB */
    if((NAS_LMM_TIMER_RUNNING == NAS_LMM_IsPtlTimerRunning(TI_NAS_EMM_PTL_CSFB_DELAY))
        &&(VOS_TRUE == NAS_EMM_SER_IsCsfbProcedure()))
    {
        /* ֹͣCSFBʱ�Ӷ�ʱ�� */
        NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_CSFB_DELAY);

        /* ��MMC�ϱ�SERVICEʧ�ܴ�������ȥGU */
        NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);
    }

    /*����LMM_MMC_AREA_LOST_IND��Ϣ*/
    NAS_EMMC_SendMmcAreaLostInd();

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32  NAS_EMMC_RcvMmcUserPlmnEndNotify(MsgBlock *pMsg )
{
    VOS_UINT32                          i = 0;
    NAS_LMM_NETWORK_INFO_STRU           *pstNetInfo = NAS_LMM_NULL_PTR;

    NAS_EMM_PUBU_LOG_NORM("NAS_EMMC_RcvMmcUserPlmnEndNotify is entered");

    (void)pMsg;

     /* ��μ�� */
    if ( NAS_EMM_NULL_PTR == pMsg)
    {
        NAS_LMM_PUBM_LOG_ERR("NAS_EMMC_RcvMmcUserPlmnEndNotify:input ptr null!");
        return  NAS_LMM_MSG_HANDLED;
    }

    pstNetInfo = (VOS_VOID*)NAS_EMM_GetEmmGlobleAddr(NAS_LMM_GLOBLE_NET_INFO);

    NAS_EMM_PUBU_LOG1_NORM("NAS_LMM_PreProcMmcUserPlmnEndNotify:Rej15 TA NUM =, ",
                               pstNetInfo->stForbTaForRej15.ulTaNum);

    for (i = 0; i < pstNetInfo->stForbTaForRej15.ulTaNum; i++)
    {
        if (NAS_LMM_MATCH_FAIL == NAS_LMM_TaMatchTaList(&pstNetInfo->stForbTaForRej15.astTa[i],&pstNetInfo->stForbTaForRoam))
        {
            NAS_LMM_AddTaInTaList( &pstNetInfo->stForbTaForRej15.astTa[i],
                                &pstNetInfo->stForbTaForRoam,
                                NAS_MM_MAX_TA_NUM);

        }

    }

    /*ɾ���ܾ�15 TA�б�*/
    NAS_LMM_MEM_SET(&pstNetInfo->stForbTaForRej15, 0, sizeof(NAS_MM_FORB_TA_LIST_STRU));

    /*����û�ָ��������ʶ*/
    NAS_EMMC_SetUserSpecPlmnFlag(NAS_EMMC_USER_SPEC_PLMN_NO);

    NAS_EMM_PUBU_LOG1_NORM("NAS_LMM_PreProcMmcUserPlmnEndNotify:FORB TA NUM =, ",
                                  pstNetInfo->stForbTaForRoam.ulTaNum);

    /*����LRRC_LMM_CELL_SELECTION_CTRL_REQ��Ϣ��RRC*/
    NAS_EMMC_SendRrcCellSelectionReq(LRRC_LNAS_FORBTA_CHANGE);

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32  NAS_EMMC_RcvMmcBgPlmnSearchReq(MsgBlock *pMsg)
{
    MMC_LMM_BG_PLMN_SEARCH_REQ_STRU  *pstMmcMsg = NAS_EMMC_NULL_PTR;

    NAS_LMM_EMMC_LOG_NORM("NAS_EMMC_RcvMmcBgPlmnSearchReq: entered.");

    /* ��ڲ������ */
    if (NAS_EMMC_NULL_PTR == pMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvMmcBgPlmnSearchReq: input ptr null!");
        return NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /* �����ǰ���ǿ���̬�������˱��������� */
    if ((NAS_EMM_CONN_IDLE != NAS_EMM_GetConnState())
        && (NAS_EMM_CONN_RELEASING != NAS_EMM_GetConnState()))
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvMmcBgPlmnSearchReq: not idle, discard!");
        return NAS_LMM_MSG_DISCARD;
    }

    pstMmcMsg = (MMC_LMM_BG_PLMN_SEARCH_REQ_STRU *)pMsg;

    /* ��LRRC���ͱ��������� */
    NAS_EMMC_SendRrcBgPlmnSearchReq(pstMmcMsg);

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32  NAS_EMMC_RcvMmcStopBgPlmnSearchReq(MsgBlock *pMsg)
{
    MMC_LMM_STOP_BG_PLMN_SEARCH_REQ_STRU *pstMmcMsg;

    NAS_LMM_EMMC_LOG_NORM("NAS_EMMC_RcvMmcStopBgPlmnSearchReq: entered.");

    /* ��μ�� */
    if ( NAS_EMMC_NULL_PTR == pMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvMmcStopBgPlmnSearchReq: input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    pstMmcMsg = (MMC_LMM_STOP_BG_PLMN_SEARCH_REQ_STRU *)pMsg;

    /* ��LRRC����ֹͣ���������� */
    NAS_EMMC_SendRrcBgPlmnSearchStopReq(pstMmcMsg);

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32  NAS_EMMC_RcvRrcBgPlmnSearchCnf(MsgBlock *pMsg)
{
    LRRC_LMM_BG_PLMN_SEARCH_CNF_STRU *pLrrcMsg = NAS_LMM_NULL_PTR;

    NAS_LMM_EMMC_LOG_NORM("NAS_EMMC_RcvRrcBgPlmnSearchCnf: entered");

    /* ��μ�� */
    if (NAS_LMM_NULL_PTR == pMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvRrcBgPlmnSearchCnf: input null");
        return NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /* �ṹת�� */
    pLrrcMsg = (LRRC_LMM_BG_PLMN_SEARCH_CNF_STRU *)pMsg;

    /* ����LMM_MMC_BG_PLMN_SEARCH_CNF��Ϣ */
    NAS_EMMC_SendMmcBgPlmnSearchCnf(pLrrcMsg);

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32  NAS_EMMC_RcvRrcBgPlmnSearchStopCnf(MsgBlock *pMsg)
{
    LRRC_LMM_BG_PLMN_SEARCH_STOP_CNF_STRU *pLrrcMsg = NAS_LMM_NULL_PTR;

    NAS_LMM_EMMC_LOG_NORM("NAS_EMMC_RcvRrcBgPlmnSearchStopCnf: entered.");

    /* ��μ�� */
    if (NAS_EMMC_NULL_PTR == pMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvRrcBgPlmnSearchStopCnf: input null");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /* �ṹת�� */
    pLrrcMsg = (LRRC_LMM_BG_PLMN_SEARCH_STOP_CNF_STRU *)pMsg;

    /* ����LMM_MMC_STOP_BG_PLMN_SEARCH_CNF��Ϣ */
    NAS_EMMC_SendMmcBgPlmnSearchStopCnf(pLrrcMsg);

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32  NAS_EMMC_RcvRrcNotCampOnInd(MsgBlock *pMsg)
{
    NAS_LMM_EMMC_LOG_NORM("NAS_EMMC_RcvRrcNotCampOnInd: entered");

    (VOS_VOID)pMsg;

    /* ��μ�� */
    if ( NAS_EMMC_NULL_PTR == pMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvRrcNotCampOnInd: input null");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /* �յ�NOT_CAMP_ON����*/
    NAS_EMMC_ProcessRrcNotCampOnInd();

    /* ����ID_EMMC_EMM_COVERAGE_LOST_IND��Ϣ */
    /* NAS_EMMC_SendEmmCoverageLostInd(); */

    /* ����LMM_MMC_NOT_CAMP_ON_IND��Ϣ */
    NAS_EMMC_SendMmcNotCampOnInd();

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32  NAS_EMMC_RcvMmcCellSelectionReq(  MsgBlock *pMsg )
{
    MMC_LMM_CELL_SELECTION_CTRL_REQ_STRU    *pstCellSelReq;
    LRRC_LNAS_RESEL_TYPE_ENUM_UINT32          ulReselType;

    NAS_LMM_EMMC_LOG_NORM("NAS_EMMC_RcvMmcCellSelectionReq is entered.");

    /* ��μ�� */
    if ( NAS_EMMC_NULL_PTR == pMsg)
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvMmcCellSelectionReq:input ptr null!");
        return  NAS_LMM_ERR_CODE_PTR_NULL;
    }

    /* �ṹת�� */
    pstCellSelReq = (MMC_LMM_CELL_SELECTION_CTRL_REQ_STRU *)pMsg;

    if (MMC_LMM_RESEL_SUITABLE == pstCellSelReq->ulSelecType)
    {
        NAS_LMM_EMMC_LOG1_NORM("NAS_EMMC_RcvMmcCellSelectionReq: Cell type = ",
                                pstCellSelReq->ulSelecType);
        ulReselType = LRRC_LNAS_RESEL_SUITABLE;
    }
    else
    {
        NAS_LMM_EMMC_LOG_ERR("NAS_EMMC_RcvMmcCellSelectionReq:Input Cell type is err! ");
        return  NAS_LMM_ERR_CODE_PARA_INVALID;
    }


    /*����RRC_MM_CELL_SELECTION_CTRL_REQ��Ϣ*/
    NAS_EMMC_SendRrcCellSelectionReq(ulReselType);

    return NAS_LMM_MSG_HANDLED;
}
/*lint +e961*/
/*lint +e960*/

VOS_UINT32 NAS_EMMC_RcvMmcGuSysInfoInd(MsgBlock *pMsg )
{
    (VOS_VOID)pMsg;

    NAS_EMM_PUBU_LOG_NORM("NAS_EMMC_RcvMmcGuSysInfoInd is entered.");

    /* ���Lģ��ǰ���ڼ���̬���򲻴���GU��SYS_INFO,ֱ�Ӷ���*/
    if(NAS_LMM_CUR_LTE_SUSPEND != NAS_EMM_GetCurLteState())
    {
        NAS_EMM_PUBU_LOG_NORM("NAS_EMMC_RcvMmcGuSysInfoNotify:LTE is not SUSPEND.");
        return NAS_LMM_MSG_HANDLED;
    }

    /*�յ�GUϵͳ��Ϣ,ֹͣ���е�T3402�Լ�T3411 */
    NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_T3411);

/*lint -e960*/
/* GCF������������3402�����в�ֹͣ3402��Ҳ�������������3402��ʱ����Ȼ���壬
   3411��ʱ���Ȳ��޸ģ��ݻ�����ԭ����ֹͣ���� */
#if (VOS_OS_VER != VOS_WIN32)
    if(PS_SUCC == LPS_OM_IsTestMode())
    {
        if(NAS_LMM_TIMER_RUNNING == NAS_LMM_IsPtlTimerRunning(TI_NAS_EMM_PTL_T3402))
        {
           return NAS_LMM_MSG_HANDLED;
        }
    }
#endif
/*lint +e960*/

    NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_T3402);

    NAS_EMM_TAU_SaveEmmTAUAttemptCnt(0);

    NAS_EMM_GLO_AD_GetAttAtmpCnt() = 0;


    return NAS_LMM_MSG_HANDLED;
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif









