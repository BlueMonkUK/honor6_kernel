

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "TafMtaCtx.h"
#include "TafMtaComm.h"
#include "TafMtaMain.h"
#include "AtMtaInterface.h"
#include "TafMtaTimerMgmt.h"
#include "TafMtaAgps.h"
#include "TcNasinterface.h"
#include "TafMtaSecure.h"
#include "TafMtaMntn.h"
#include "MtaPhyInterface.h"
#include "TafMtaPhy.h"
#include "NasMtaInterface.h"
#include "TafMtaUnSolicitedReport.h"

#include "NasErrorLog.h"

#include "MtaMtcInterface.h"
#include "TafMtaModemControl.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_TAF_MTA_MAIN_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/* MTAģ�鴦������AT��Ϣ������Ӧ��*/
const MTA_MSG_PROC_STRU g_astMtaMsgProcAtTab[]=
{
    /* ��ϢID */                                /* ��Ϣ������ */
    {ID_AT_MTA_CPOS_SET_REQ,                    TAF_MTA_RcvAtCposSetReq},
    {ID_AT_MTA_CGPSCLOCK_SET_REQ,               TAF_MTA_RcvAtCgpsClockSetReq},

    {ID_AT_MTA_APSEC_SET_REQ,                   TAF_MTA_RcvAtApSecSetReq},

    {ID_AT_MTA_SIMLOCKUNLOCK_SET_REQ,           TAF_MTA_RcvAtSimlockUnlockSetReq},

    {ID_AT_MTA_QRY_NMR_REQ,                     TAF_MTA_RcvQryNmrReq},

    {ID_AT_MTA_RESEL_OFFSET_CFG_SET_NTF,        TAF_MTA_RcvReselOffsetCfgSetReq},

    {ID_AT_MTA_WRR_AUTOTEST_QRY_REQ,            TAF_MTA_RcvQryWrrAutotestReq},
    {ID_AT_MTA_WRR_CELLINFO_QRY_REQ,            TAF_MTA_RcvQryWrrCellInfoReq},
    {ID_AT_MTA_WRR_MEANRPT_QRY_REQ,             TAF_MTA_RcvQryWrrMeanrptReq},
    {ID_AT_MTA_WRR_FREQLOCK_SET_REQ,            TAF_MTA_RcvSetWrrFreqlockReq},
    {ID_AT_MTA_WRR_RRC_VERSION_SET_REQ,         TAF_MTA_RcvSetWrrRrcVersionReq},
    {ID_AT_MTA_WRR_CELLSRH_SET_REQ,             TAF_MTA_RcvSetWrrCellsrhReq},
    {ID_AT_MTA_WRR_FREQLOCK_QRY_REQ,            TAF_MTA_RcvQryWrrFreqlockReq},
    {ID_AT_MTA_WRR_RRC_VERSION_QRY_REQ,         TAF_MTA_RcvQryWrrRrcVersionReq},
    {ID_AT_MTA_WRR_CELLSRH_QRY_REQ,             TAF_MTA_RcvQryWrrCellsrhReq},

    {ID_AT_MTA_BODY_SAR_SET_REQ,                TAF_MTA_RcvAtBodySarSetReq},

    {ID_AT_MTA_CURC_SET_NOTIFY,                 TAF_MTA_RcvAtSetCurcNotify},
    {ID_AT_MTA_CURC_QRY_REQ,                    TAF_MTA_RcvAtQryCurcReq},
    {ID_AT_MTA_UNSOLICITED_RPT_SET_REQ,         TAF_MTA_RcvAtSetUnsolicitedRptReq},
    {ID_AT_MTA_UNSOLICITED_RPT_QRY_REQ,         TAF_MTA_RcvAtQryUnsolicitedRptReq},
    {ID_TAFAGENT_MTA_GET_SYSMODE_REQ,           TAF_MTA_RcvTafAgentGetSysModeReq},

    {ID_AT_MTA_IMEI_VERIFY_QRY_REQ,             TAF_MTA_RcvQryImeiVerifyReq},
    {ID_AT_MTA_CGSN_QRY_REQ,                    TAF_MTA_RcvQryCgsnReq},

    {ID_AT_MTA_NCELL_MONITOR_SET_REQ,           TAF_MTA_RcvAtSetNCellMonitorReq},
    {ID_AT_MTA_NCELL_MONITOR_QRY_REQ,           TAF_MTA_RcvAtQryNCellMonitorReq},

    {ID_AT_MTA_REFCLKFREQ_SET_REQ,              TAF_MTA_RcvAtRefclkfreqSetReq},
    {ID_AT_MTA_REFCLKFREQ_QRY_REQ,              TAF_MTA_RcvAtRefclkfreqQryReq},

    {ID_TAFAGENT_MTA_GET_ANT_STATE_REQ,         TAF_MTA_RcvTafAgentGetAntStateReq},

    {ID_AT_MTA_HANDLEDECT_SET_REQ,              TAF_MTA_RcvAtSetHandleDectReq},
    {ID_AT_MTA_HANDLEDECT_QRY_REQ,              TAF_MTA_RcvAtQryHandleDectReq},

    {ID_AT_MTA_ECID_SET_REQ,                    TAF_MTA_RcvAtEcidSetReq},

    {ID_AT_MTA_MIPICLK_QRY_REQ,                TAF_MTA_RcvAtQryMipiClkReq},

    {ID_AT_MTA_SET_DPDTTEST_FLAG_REQ,           TAF_MTA_RcvAtSetDpdtTestFlagReq},
    {ID_AT_MTA_SET_DPDT_VALUE_REQ,              TAF_MTA_RcvAtSetDpdtValueReq},
    {ID_AT_MTA_QRY_DPDT_VALUE_REQ,              TAF_MTA_RcvAtQryDpdtValueReq},

    {ID_AT_MTA_SET_JAM_DETECT_REQ,              TAF_MTA_RcvAtSetJamDetectReq},

};

/* MTAģ�鴦������PHY APMģ�����Ϣ������Ӧ��*/
const MTA_MSG_PROC_STRU g_astMtaMsgProcApmTab[]=
{
    /* ��ϢID */                                /* ��Ϣ������ */
    {ID_APM_MTA_BODY_SAR_SET_CNF,               TAF_MTA_RcvPhyBodySarSetCnf},

    {ID_APM_MTA_REFCLOCK_STATUS_IND,            TAF_MTA_RcvPhyRefClockStatusInd},

    {ID_APM_MTA_HANDLE_DETECT_SET_CNF,          TAF_MTA_RcvPhyHandleDectSetCnf},
    {ID_APM_MTA_HANDLE_DETECT_QRY_CNF,          TAF_MTA_RcvPhyHandleDectQryCnf},
};

/* MTAģ�鴦�����Խ����Gģ����Ϣ������Ӧ��*/
const MTA_MSG_PROC_STRU g_astMtaMsgProcGasTab[]=
{
    /* ��ϢID */                                /* ��Ϣ������ */
    {ID_RRC_MTA_MSG_POSITION_REQ,               TAF_MTA_RcvRrcMsgPositionReq},

    {ID_RRC_MTA_QRY_NMR_CNF,                    TAF_MTA_RcvGUAsEcellInfoQryCnf},

    {ID_GRR_MTA_NCELL_MONITOR_SET_CNF,          TAF_MTA_RcvRrcSetNCellMonitorCnf},
    {ID_GRR_MTA_NCELL_MONITOR_QRY_CNF,          TAF_MTA_RcvRrcQryNCellMonitorCnf},
    {ID_GRR_MTA_NCELL_MONITOR_IND,              TAF_MTA_RcvRrcNCellMonitorInd},

    {ID_RRC_MTA_JAM_DETECT_CNF,                 TAF_MTA_RcvRrcJamDetectCnf},
    {ID_RRC_MTA_JAM_DETECT_IND,                 TAF_MTA_RcvRrcJamDetectInd},
};
/* MTAģ�鴦�����Խ����Wģ����Ϣ������Ӧ��*/
const MTA_MSG_PROC_STRU g_astMtaMsgProcWcomTab[]=
{
    /* ��ϢID */                                /* ��Ϣ������ */
    {ID_RRC_MTA_MSG_POSITION_REQ,               TAF_MTA_RcvRrcMsgPositionReq},

};


/* MTAģ�鴦�����Խ����Wrrģ����Ϣ������Ӧ��*/
const MTA_MSG_PROC_STRU g_astMtaMsgProcWrrTab[]=
{
    {ID_RRC_MTA_QRY_NMR_CNF,                    TAF_MTA_RcvGUAsEcellInfoQryCnf},
    {ID_WRR_MTA_AUTOTEST_QRY_CNF,               TAF_MTA_RcvWrrAutotestQryCnf},
    {ID_WRR_MTA_CELLINFO_QRY_CNF,               TAF_MTA_RcvWrrCellInfoQryCnf},
    {ID_WRR_MTA_MEASRPT_QRY_CNF,                TAF_MTA_RcvWrrMeanrptQryCnf},
    {ID_WRR_MTA_FREQLOCK_SET_CNF,               TAF_MTA_RcvWrrFreqlockSetCnf},
    {ID_WRR_MTA_RRC_VERSION_SET_CNF,            TAF_MTA_RcvWrrVersionSetCnf},
    {ID_WRR_MTA_CELLSRH_SET_CNF,                TAF_MTA_RcvWrrCellsrhSetCnf},
    {ID_WRR_MTA_FREQLOCK_QRY_CNF,               TAF_MTA_RcvWrrFreqlockQryCnf},
    {ID_WRR_MTA_RRC_VERSION_QRY_CNF,            TAF_MTA_RcvWrrVersionQryCnf},
    {ID_WRR_MTA_CELLSRH_QRY_CNF,                TAF_MTA_RcvWrrCellsrhQryCnf},
};


/* MTAģ�鴦�����������Gģ����Ϣ������Ӧ��*/
const MTA_MSG_PROC_STRU g_astMtaMsgProcGphyTab[]=
{
    /* ��ϢID */                                /* ��Ϣ������ */
    {ID_GPHY_MTA_SET_GPS_RF_CLOCK_CNF,          TAF_MTA_RcvPhyCgpsClockSetCnf},
    {ID_GUPHY_MTA_QRY_DPDT_VALUE_CNF,           TAF_MTA_RcvGuPhyQryDpdtValueCnf},

};
/* MTAģ�鴦�����������Wģ����Ϣ������Ӧ��*/
const MTA_MSG_PROC_STRU g_astMtaMsgProcWphyTab[]=
{
    /* ��ϢID */                                /* ��Ϣ������ */
    {ID_WPHY_MTA_SET_GPS_RF_CLOCK_CNF,          TAF_MTA_RcvPhyCgpsClockSetCnf},
    {ID_GUPHY_MTA_QRY_DPDT_VALUE_CNF,           TAF_MTA_RcvGuPhyQryDpdtValueCnf},

};
/* MTAģ�鴦������TC��Ϣ������Ӧ��*/
const MTA_MSG_PROC_STRU g_astMtaMsgProcTcTab[]=
{
    /* ��ϢID */                                /* ��Ϣ������ */
    {TCMTA_RESET_UE_POS_STORED_INFO_IND,        TAF_MTA_RcvTcResetUePosStoredInfoInd},

};
/* MTAģ�鴦�����Զ�ʱ����Ϣ������Ӧ��*/
const MTA_MSG_PROC_STRU g_astMtaMsgProcTimerTab[]=
{
    /* ��ϢID */                                /* ��Ϣ������ */
    {TI_TAF_MTA_WAIT_AT_SET_CPOS_CNF,           TAF_MTA_RcvTiCposSetReqExpired},
    {TI_TAF_MTA_WAIT_GPHY_SET_CGPSCLOCK_CNF,    TAF_MTA_RcvTiCgpsClockSetReqExpired},
    {TI_TAF_MTA_WAIT_WPHY_SET_CGPSCLOCK_CNF,    TAF_MTA_RcvTiCgpsClockSetReqExpired},

    {TI_TAF_MTA_WAIT_AS_QRY_NMR_CNF,            TAF_MTA_RcvTiWaitAsQryNmrCnfExpired},

    {TI_TAF_MTA_WAIT_WRR_AUTOTEST_QRY_CNF,      TAF_MTA_RcvTiReqAutotestQryExpired},
    {TI_TAF_MTA_WAIT_WRR_CELLINFO_QRY_CNF,      TAF_MTA_RcvTiReqCellInfoQryExpired},
    {TI_TAF_MTA_WAIT_WRR_MEANRPT_QRY_CNF,       TAF_MTA_RcvTiReqMeanrptQryExpired},
    {TI_TAF_MTA_WAIT_WRR_FREQLOCK_SET_CNF,      TAF_MTA_RcvTiReqFreqlockSetExpired},
    {TI_TAF_MTA_WAIT_WRR_RRC_VERSION_SET_CNF,   TAF_MTA_RcvTiReqRrcVersionSetExpired},
    {TI_TAF_MTA_WAIT_WRR_CELLSRH_SET_CNF,       TAF_MTA_RcvTiReqCellsrhSetExpired},
    {TI_TAF_MTA_WAIT_WRR_FREQLOCK_QRY_CNF,      TAF_MTA_RcvTiReqFreqlockQryExpired},
    {TI_TAF_MTA_WAIT_WRR_RRC_VERSION_QRY_CNF,   TAF_MTA_RcvTiReqRrcVersionQryExpired},
    {TI_TAF_MTA_WAIT_WRR_CELLSRH_QRY_CNF,       TAF_MTA_RcvTiReqCellsrhQryExpired},

    {TI_TAF_MTA_WAIT_PHY_SET_BODYSAR_CNF,       TAF_MTA_RcvTiWaitPhySetBodySarExpired},

    {TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_SET_CNF,  TAF_MTA_RcvTiWaitGrrSetNCellMonitorExpired},
    {TI_TAF_MTA_WAIT_GRR_NCELLMONITOR_QRY_CNF,  TAF_MTA_RcvTiWaitGrrQryNCellMonitorExpired},

    {TI_TAF_MTA_WAIT_PHY_HANDLEDECT_SET_CNF,    TAF_MTA_RcvTiWaitPhySetHandleDectExpired},
    {TI_TAF_MTA_WAIT_PHY_HANDLEDECT_QRY_CNF,    TAF_MTA_RcvTiWaitPhyQryHandleDectExpired},

    {TI_TAF_MTA_WAIT_AS_ECID_SET_CNF,           TAF_MTA_RcvTiEcidSetExpired},

    {TI_TAF_MTA_RF_LCD_MIPI_CLK_REPORT,         TAF_MTA_RcvTiMipiClkExpired},

    {TI_TAF_MTA_WAIT_DPDT_VALUE_QRY_CNF,        TAF_MTA_RcvTiReqDpdtValueQryExpired},

    {TI_TAF_MTA_WAIT_JAM_DETECT_SET_CNF,        TAF_MTA_RcvTiWaitGrrSetJamDetectExpired},

};

/* MTAģ�鴦������MMAģ�����Ϣ������Ӧ��*/
const MTA_MSG_PROC_STRU g_astMtaMsgProcNasTab[]=
{
    /* ��ϢID */                                /* ��Ϣ������ */
    {ID_NAS_MTA_AREA_LOST_IND,                  TAF_MTA_RcvNasAreaLostInd},
};

/* MTAģ�鴦������AT��Ϣ������Ӧ��*/
const MTA_MSG_PROC_STRU g_astMtaMsgProcTafAgentTab[]=
{
    {ID_TAFAGENT_MTA_GET_SYSMODE_REQ,           TAF_MTA_RcvTafAgentGetSysModeReq},

    {ID_TAFAGENT_MTA_GET_ANT_STATE_REQ,         TAF_MTA_RcvTafAgentGetAntStateReq},
};


#if (FEATURE_ON == FEATURE_PTM)
/* MTAģ�鴦������Acpu OMģ�����Ϣ������Ӧ��*/
const MTA_MSG_PROC_STRU g_astMtaMsgProcAcpuOmTab[]=
{
    /* ��ϢID */                                /* ��Ϣ������ */
    {ID_OM_ERR_LOG_CTRL_IND,                    TAF_MTA_RcvAcpuOmErrLogCtrlInd},
    {ID_OM_ERR_LOG_REPORT_REQ,                  TAF_MTA_RcvAcpuOmErrLogRptReq},
};
#endif

/* MTAģ�鴦������MTCģ�����Ϣ������Ӧ��*/
const MTA_MSG_PROC_STRU g_astMtaMsgProcMtcTab[]=
{
    /* ��ϢID */                                /* ��Ϣ������ */
#if (FEATURE_ON == FEATURE_MULTI_MODEM)
    {ID_MTC_MTA_PS_TRANSFER_IND,                TAF_MTA_RcvMtcPsTransferInd},
#endif
    {ID_MTC_MTA_MIPICLK_INFO_IND,               TAF_MTA_RcvMtcMipiClkInfoInd},
};

#if (FEATURE_ON == FEATURE_LTE)
const MTA_MSG_PROC_STRU g_astMtaMsgProcLrrcTab[]=
{
    /* ��ϢID */                                /* ��Ϣ������ */
    {ID_LRRC_MTA_CELLINFO_QRY_CNF,              TAF_MTA_RcvLrrcCellinfoQryCnf},

    {ID_LRRC_MTA_QRY_DPDT_VALUE_CNF,            TAF_MTA_RcvLrrcDpdtValueQryCnf},
};
#endif

/*****************************************************************************
  3 ��������
*****************************************************************************/


VOS_VOID TAF_MTA_SndAtMsg(
    AT_APPCTRL_STRU                    *pAppCtrl,
    VOS_UINT32                          ulMsgId,
    VOS_UINT32                          ulLen,
    VOS_UINT8                          *pData
)
{
    AT_MTA_MSG_STRU                    *pstMtaMsgStr;

    /* �ж�������� */
    if ((VOS_NULL_PTR == pData) || (0 == ulLen))
    {
        MTA_ERROR_LOG("TAF_MTA_SndAtMsg: para is invalid!");
        return;
    }

    /* ��Ϣ�ռ����� */
    pstMtaMsgStr = (AT_MTA_MSG_STRU *)PS_ALLOC_MSG( UEPS_PID_MTA,
                                                    sizeof(AT_APPCTRL_STRU) + sizeof(ulMsgId) + ulLen);
    if (VOS_NULL_PTR == pstMtaMsgStr)
    {
        MTA_ERROR_LOG("TAF_MTA_SndAtMsg: alloc msg fail!");
        return;
    }

    /* ����Ϣ�� */
    pstMtaMsgStr->ulReceiverCpuId       = VOS_LOCAL_CPUID;
    pstMtaMsgStr->ulSenderCpuId         = VOS_LOCAL_CPUID;
    pstMtaMsgStr->ulSenderPid           = UEPS_PID_MTA;
    pstMtaMsgStr->ulReceiverPid         = WUEPS_PID_AT;
    pstMtaMsgStr->ulMsgId               = ulMsgId;
    pstMtaMsgStr->stAppCtrl.usClientId  = MN_GetRealClientId(pAppCtrl->usClientId, UEPS_PID_MTA);
    pstMtaMsgStr->stAppCtrl.ucOpId      = pAppCtrl->ucOpId;
    pstMtaMsgStr->stAppCtrl.aucReserved[0] = 0;
    PS_MEM_CPY(pstMtaMsgStr->aucContent, pData, ulLen);

    /* ������Ϣ��ATģ�� */
    if (VOS_OK != PS_SEND_MSG(UEPS_PID_MTA, pstMtaMsgStr))
    {
        MTA_ERROR_LOG("TAF_MTA_SndAtMsg():WARNING:SEND MSG FIAL");
    }
    return;
}


VOS_UINT32 TAF_MTA_SearchMsgProcTab(
    VOS_UINT32                          ulMsgCnt,
    VOS_VOID                           *pstMsg,
    const MTA_MSG_PROC_STRU            *pstMsgProcTab
)
{
    VOS_UINT32                          ulLoop;
    VOS_UINT32                          ulMsgName;

    /* ����Ϣ���л�ȡMSG ID */
    ulMsgName  = ((MSG_HEADER_STRU*)pstMsg)->ulMsgName;

    /* ���������Ϣ�ַ� */
    for (ulLoop = 0; ulLoop < ulMsgCnt; ulLoop++)
    {
        if (pstMsgProcTab[ulLoop].ulMsgType == ulMsgName)
        {
            pstMsgProcTab[ulLoop].pProcMsgFunc(pstMsg);
            break;
        }
    }

    /* û���ҵ�ƥ�����Ϣ */
    if (ulMsgCnt == ulLoop)
    {
        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_VOID TAF_MTA_RcvAtMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtaMsgProcAtTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtaMsgProcAtTab)/sizeof(MTA_MSG_PROC_STRU);

    /*g_astMtaMsgProcAtTab���������Ϣ�ַ�*/
    ulRst = TAF_MTA_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtaMsgProcAtTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTA_ERROR_LOG("TAF_MTA_RcvAtMsg: Msg Id is invalid!");
    }

    return;
}
VOS_VOID TAF_MTA_RcvApmMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtaMsgProcApmTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtaMsgProcApmTab)/sizeof(MTA_MSG_PROC_STRU);

    /* g_astMtaMsgProcApmTab���������Ϣ�ַ� */
    ulRst = TAF_MTA_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtaMsgProcApmTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTA_ERROR_LOG("TAF_MTA_RcvApmMsg: Msg Id is invalid!");
    }

    return;
}
VOS_VOID TAF_MTA_RcvGasMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtaMsgProcGasTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtaMsgProcGasTab)/sizeof(MTA_MSG_PROC_STRU);

    /* g_astMtaMsgProcGasTab���������Ϣ�ַ� */
    ulRst = TAF_MTA_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtaMsgProcGasTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTA_ERROR_LOG("TAF_MTA_RcvGasMsg: Msg Id is invalid!");
    }

    return;
}
VOS_VOID TAF_MTA_RcvWcomMsg(struct MsgCB *pstMsg)

{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtaMsgProcWcomTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtaMsgProcWcomTab)/sizeof(MTA_MSG_PROC_STRU);

    /* g_astMtaMsgProcWasTab���������Ϣ�ַ� */
    ulRst = TAF_MTA_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtaMsgProcWcomTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTA_ERROR_LOG("TAF_MTA_RcvWcomMsg: Msg Id is invalid!");
    }

    return;
}
VOS_VOID TAF_MTA_RcvWrrMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtaMsgProcWrrTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtaMsgProcWrrTab)/sizeof(MTA_MSG_PROC_STRU);

    /* g_astMtaMsgProcWrrTab���������Ϣ�ַ� */
    ulRst = TAF_MTA_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtaMsgProcWrrTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTA_ERROR_LOG("TAF_MTA_RcvWrrMsg: Msg Id is invalid!");
    }

    return;
}
VOS_VOID TAF_MTA_RcvGphyMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulLoop;
    VOS_UINT32                          ulMsgName;

    /* ��g_astMtaMsgProcGphyTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtaMsgProcGphyTab)/sizeof(MTA_MSG_PROC_STRU);

    /* ����Ϣ���л�ȡMSG ID */
    ulMsgName = (VOS_UINT32)((MTA_PHY_MSG_HEADER_STRU*)pstMsg)->usMsgID;

    /* g_astMtaMsgProcGphyTab���������Ϣ�ַ� */
    for (ulLoop = 0; ulLoop < ulMsgCnt; ulLoop++)
    {
        if (g_astMtaMsgProcGphyTab[ulLoop].ulMsgType == ulMsgName)
        {
            g_astMtaMsgProcGphyTab[ulLoop].pProcMsgFunc(pstMsg);
            break;
        }
    }

    /* û���ҵ�ƥ�����Ϣ */
    if (ulMsgCnt == ulLoop)
    {
        MTA_ERROR_LOG("TAF_MTA_RcvGphyMsg: Msg Id is invalid!");
    }

    return;
}



VOS_VOID TAF_MTA_RcvWphyMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulLoop;
    VOS_UINT32                          ulMsgName;

    /* ��g_astMtaMsgProcWphyTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtaMsgProcWphyTab)/sizeof(MTA_MSG_PROC_STRU);

    /* ����Ϣ���л�ȡMSG ID */
    ulMsgName = (VOS_UINT32)((MTA_PHY_MSG_HEADER_STRU*)pstMsg)->usMsgID;

    /* g_astMtaMsgProcWphyTab���������Ϣ�ַ� */
    for (ulLoop = 0; ulLoop < ulMsgCnt; ulLoop++)
    {
        if (g_astMtaMsgProcWphyTab[ulLoop].ulMsgType == ulMsgName)
        {
            g_astMtaMsgProcWphyTab[ulLoop].pProcMsgFunc(pstMsg);
            break;
        }
    }

    /* û���ҵ�ƥ�����Ϣ */
    if (ulMsgCnt == ulLoop)

    {
        MTA_ERROR_LOG("TAF_MTA_RcvWphyMsg: Msg Id is invalid!");
    }

    return;
}


VOS_VOID TAF_MTA_RcvTcMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtaMsgProcTcTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtaMsgProcTcTab)/sizeof(MTA_MSG_PROC_STRU);

    /* g_astMtaMsgProcTcTab���������Ϣ�ַ� */
    ulRst = TAF_MTA_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtaMsgProcTcTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTA_ERROR_LOG("TAF_MTA_RcvTcMsg: Msg Id is invalid!");
    }

    return;
}
VOS_VOID TAF_MTA_RcvTimerMsg(VOS_VOID *pstMsg)
{
    VOS_UINT32                          ulLoop;
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulTimerId;
    REL_TIMER_MSG                      *pstRcvTimerMsg;

    /* ��g_astMtaMsgProcTimerTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtaMsgProcTimerTab)/sizeof(MTA_MSG_PROC_STRU);

    /* ��ȡTIMER ID���Ѷ�ʱ��״̬��ΪSTOP */
    pstRcvTimerMsg = (REL_TIMER_MSG *)pstMsg;
    ulTimerId      = pstRcvTimerMsg->ulName;
    TAF_MTA_StopTimer(ulTimerId);

    /* g_astMtaMsgProcTimerTab���������Ϣ�ַ� */
    for (ulLoop = 0; ulLoop < ulMsgCnt; ulLoop++)
    {
        if (g_astMtaMsgProcTimerTab[ulLoop].ulMsgType == ulTimerId)
        {
            g_astMtaMsgProcTimerTab[ulLoop].pProcMsgFunc((VOS_VOID *)pstMsg);
            return;
        }
    }

    /* û���ҵ�ƥ�����Ϣ */
    if (ulMsgCnt == ulLoop)
    {
        MTA_ERROR_LOG("TAF_MTA_RcvTimerMsg: Msg Id is invalid!");
    }

    return;
}


VOS_VOID TAF_MTA_RcvNasMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtaMsgProcMmaTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtaMsgProcNasTab)/sizeof(MTA_MSG_PROC_STRU);

    /*g_astMtaMsgProcMmaTab���������Ϣ�ַ�*/
    ulRst = TAF_MTA_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtaMsgProcNasTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTA_ERROR_LOG("TAF_MTA_RcvNasMsg: Msg Id is invalid!");
    }

    return;
}
VOS_VOID TAF_MTA_RcvTafAgentMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtaMsgProcTafAgentTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtaMsgProcTafAgentTab)/sizeof(MTA_MSG_PROC_STRU);

    /*g_astMtaMsgProcTafAgentTab���������Ϣ�ַ�*/
    ulRst = TAF_MTA_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtaMsgProcTafAgentTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTA_ERROR_LOG("TAF_MTA_RcvTafAgentMsg: Msg Id is invalid!");
    }

    return;
}



#if (FEATURE_ON == FEATURE_PTM)
VOS_VOID TAF_MTA_RcvAcpuOmMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtaMsgProcAcpuOmTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtaMsgProcAcpuOmTab)/sizeof(MTA_MSG_PROC_STRU);

    /*g_astMtaMsgProcAcpuOmTab���������Ϣ�ַ�*/
    ulRst = TAF_MTA_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtaMsgProcAcpuOmTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTA_ERROR_LOG("TAF_MTA_RcvAcpuOmMsg: Msg Id is invalid!");
    }

    return;
}
#endif

#if (FEATURE_ON == FEATURE_MULTI_MODEM)
VOS_VOID TAF_MTA_RcvMtcMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtaMsgProcMtcTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtaMsgProcMtcTab)/sizeof(MTA_MSG_PROC_STRU);

    /*g_astMtaMsgProcMtcTab���������Ϣ�ַ�*/
    ulRst = TAF_MTA_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtaMsgProcMtcTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTA_ERROR_LOG("TAF_MTA_RcvMtcMsg: Msg Id is invalid!");
    }

    return;
}
#endif

#if (FEATURE_ON == FEATURE_LTE)
VOS_VOID TAF_MTA_RcvLrrcMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtaMsgProcLrrcTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtaMsgProcLrrcTab)/sizeof(MTA_MSG_PROC_STRU);

    /*g_astMtaMsgProcLrrcTab���������Ϣ�ַ�*/
    ulRst = TAF_MTA_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtaMsgProcLrrcTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTA_ERROR_LOG("TAF_MTA_RcvLrrcMsg: Msg Id is invalid!");
    }

    return;
}
#endif
VOS_VOID  TAF_MTA_ProcMsg (struct MsgCB *pstMsg)
{
    /* ��ϢΪ�գ�ֱ�ӷ��� */
    if (VOS_NULL_PTR == pstMsg)
    {
        MN_ERR_LOG("TAF_MTA_ProcMsg: PstMsg is NULL!");
        return;
    }

    /* ���ݷ��ͷ�PID���ַ���Ϣ */
    switch (pstMsg->ulSenderPid)
    {
        case WUEPS_PID_AT:
            TAF_MTA_RcvAtMsg(pstMsg);
            break;
        case WUEPS_PID_WCOM:
            TAF_MTA_RcvWcomMsg(pstMsg);
            break;
        case UEPS_PID_GAS:
            TAF_MTA_RcvGasMsg(pstMsg);
            break;
        case DSP_PID_GPHY:
            TAF_MTA_RcvGphyMsg(pstMsg);
            break;
        case DSP_PID_WPHY:
            TAF_MTA_RcvWphyMsg(pstMsg);
            break;
        case WUEPS_PID_TC:
            TAF_MTA_RcvTcMsg(pstMsg);
            break;
        case VOS_PID_TIMER:
            TAF_MTA_RcvTimerMsg(pstMsg);
            break;

        case WUEPS_PID_WRR:
            TAF_MTA_RcvWrrMsg(pstMsg);
            break;

        case DSP_PID_APM:
            TAF_MTA_RcvApmMsg(pstMsg);
            break;

        case WUEPS_PID_MMA:
            TAF_MTA_RcvNasMsg(pstMsg);
            break;
        case ACPU_PID_TAFAGENT:
            TAF_MTA_RcvTafAgentMsg(pstMsg);
            break;
#if (FEATURE_ON == FEATURE_PTM)
        case ACPU_PID_OM:
            TAF_MTA_RcvAcpuOmMsg(pstMsg);
            break;
#endif
#if (FEATURE_ON == FEATURE_MULTI_MODEM)
        case UEPS_PID_MTC:
            TAF_MTA_RcvMtcMsg(pstMsg);
            break;
#endif
#if (FEATURE_ON == FEATURE_LTE)
        case PS_PID_ERRC:
            TAF_MTA_RcvLrrcMsg(pstMsg);
            break;
#endif

        default:
            MTA_ERROR_LOG("TAF_MTA_ProcMsg: No opposite MsgPro Function!");
            break;
    }

    return;
}


VOS_VOID  TAF_MTA_InitCtx(TAF_MTA_CONTEXT_STRU *pstMtaCtx)
{
    /* ��ʼ������¼XML������Դ�Ľṹ */
    pstMtaCtx->stAgpsCtx.stXmlText.pcBufHead   = VOS_NULL_PTR;
    pstMtaCtx->stAgpsCtx.stXmlText.pcBufCur    = VOS_NULL_PTR;

    /* ��ʼ�������ж�ʱ����Ϣ */
    TAF_MTA_InitAllTimers(pstMtaCtx->astTimerCtx);

    /* ��ʼ��AT�������� */
    TAF_MTA_InitCmdBufferQueue(TAF_MTA_GetCmdBufferQueueAddr());

    /* ��ʼ��GPS�ο�ʱ��״̬��Ϣ */
    TAF_MTA_InitRefClockInfo(&pstMtaCtx->stAgpsCtx.stRefClockInfo);

    /* ��ʼ��RF&Lcd��������ļ� */
    TAF_MTA_InitRfLcdIntrusionCtx(&pstMtaCtx->stMtaRfLcdCtx);

    return;
}
VOS_UINT32  TAF_MTA_InitTask( VOS_VOID )
{
    TAF_MTA_CONTEXT_STRU                *pstMtaCtx;

    /* ��ʼ��MTAģ��CTX */
    pstMtaCtx   = TAF_MTA_GetMtaCtxAddr();
    TAF_MTA_InitCtx(pstMtaCtx);

    return VOS_OK;
}
VOS_UINT32 TAF_MTA_InitPid(enum VOS_INIT_PHASE_DEFINE ip)
{
    switch ( ip )
    {
        case VOS_IP_LOAD_CONFIG:
            {
                /* ����MTAģ���ʼ������ */
                TAF_MTA_InitTask();
            }break;
        default:
            break;
    }

    return VOS_OK;
}









#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
