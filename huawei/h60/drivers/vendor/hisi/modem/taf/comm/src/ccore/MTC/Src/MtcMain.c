
/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "MtcMain.h"
#include "MtcComm.h"
#include "MtcCtx.h"
#include "MtcIntrusion.h"
#include "MtcPsTransfer.h"
#include "CmmcaMtcInterface.h"
#include "MtcRfLcdIntrusion.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_MTC_MAIN_C


/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/* MTCģ�鴦������Modem0 Mma��Ϣ������Ӧ��*/
const MTC_MSG_PROC_STRU                 g_astMtcMsgProcModem0MmaTab[] =
{
    {ID_MMA_MTC_POWER_STATE_IND,            MTC_RcvMmaPowerStateInd},           /* Modem0���ػ�״̬֪ͨ */
    {ID_MMA_MTC_RAT_MODE_IND,               MTC_RcvMmaRatModeInd},              /* ����ģʽ֪ͨ */

    {ID_MMA_MTC_CURR_CAMP_PLMN_INFO_IND,    MTC_RcvMmaCurrCampPlmnInfoInd},

    {ID_MMA_MTC_EPLMN_INFO_IND,             MTC_RcvMmaEplmnInfoInd},

    {ID_MMA_MTC_REG_STATUS_IND,             MTC_RcvMmaRegStatusInd},
};

/* MTCģ�鴦������Modem1 Mma��Ϣ������Ӧ��*/
const MTC_MSG_PROC_STRU                 g_astMtcMsgProcModem1MmaTab[] =
{
    {ID_MMA_MTC_POWER_STATE_IND,            MTC_RcvMmaPowerStateInd},           /* Modem1���ػ�״̬֪ͨ */
    {ID_MMA_MTC_RAT_MODE_IND,               MTC_RcvMmaRatModeInd},              /* ����ģʽ֪ͨ */

    {ID_MMA_MTC_CURR_CAMP_PLMN_INFO_IND,    MTC_RcvMmaCurrCampPlmnInfoInd},

    {ID_MMA_MTC_EPLMN_INFO_IND,             MTC_RcvMmaEplmnInfoInd},

    {ID_MMA_MTC_REG_STATUS_IND,             MTC_RcvMmaRegStatusInd},
};

/* MTCģ�鴦������Modem0 Taf��Ϣ������Ӧ��*/
const MTC_MSG_PROC_STRU g_astMtcMsgProcModem0TafTab[] =
{
    {ID_TAF_MTC_CS_SERVICE_INFO_IND,        MTC_RcvTafCsSrvInfoInd},            /* CS��ҵ��״̬֪ͨ */
};

/* MTCģ�鴦������Modem1 Taf��Ϣ������Ӧ��*/
const MTC_MSG_PROC_STRU                 g_astMtcMsgProcModem1TafTab[] =
{
    {ID_TAF_MTC_CS_SERVICE_INFO_IND,        MTC_RcvTafCsSrvInfoInd},            /* CS��ҵ��״̬֪ͨ */
};

/* MTCģ�鴦������Modem1 GAS��Ϣ������Ӧ��*/
const MTC_MSG_PROC_STRU                 g_astMtcMsgProcModem1GasTab[] =
{
    {ID_RRC_MTC_INTRUSION_ACTION_SET_CNF,   MTC_RcvIntrusionActionSetCnf},      /* ���ſ��ƿ������ûظ� */
    {ID_RRC_MTC_INTRUSION_BAND_SET_CNF,     MTC_RcvIntrusionBandSetCnf},        /* ���ſ��ƽ���Ƶ�����ûظ� */
    {ID_RRC_MTC_INTRUSION_BAND_INFO_IND,    MTC_RcvIntrusionBandInfoInd},       /* Ƶ����Ϣ�����ϱ� */

    {ID_RRC_MTC_NCELL_INFO_IND,             MTC_RcvRrcNcellInfoInd},
    {ID_RRC_MTC_USING_FREQ_IND,             MTC_RcvRrcUsingFreqInd},            /* ����������ϱ�Ƶ��仯��Ϣ */
    {ID_RRC_MTC_GSM_CELL_INFO_IND,          MTC_RcvGsmCellInfoInd},             /* �ϱ�Ƶ����ź�ǿ����Ϣ */

};

/* MTCģ�鴦������Modem0 TD-SCDMA��Ϣ������Ӧ��*/
const MTC_MSG_PROC_STRU                 g_astMtcMsgProcModem0TdsTab[] =
{
    {ID_RRC_MTC_INTRUSION_ACTION_SET_CNF,   MTC_RcvIntrusionActionSetCnf},      /* ���ſ��ƿ������ûظ� */
    {ID_RRC_MTC_INTRUSION_BAND_SET_CNF,     MTC_RcvIntrusionBandSetCnf},        /* ���ſ��ƽ���Ƶ�����ûظ� */
    {ID_RRC_MTC_INTRUSION_BAND_INFO_IND,    MTC_RcvIntrusionBandInfoInd},       /* Ƶ����Ϣ�����ϱ� */
    {ID_RRC_MTC_AREA_LOST_IND,              MTC_RcvRrcAreaLostInd},             /* ����㶪����Ϣ�����ϱ� */
    {ID_RRC_MTC_USING_FREQ_IND,             MTC_RcvRrcUsingFreqInd},            /* ����������ϱ�Ƶ��仯��Ϣ */

    {ID_RRC_MTC_AREA_AVALIABLE_IND,         MTC_RcvRrcAreaAvaliableInd},        /* ���������ָ���Ϣ�����ϱ� */
};

/* MTCģ�鴦������Modem0 LTE��Ϣ������Ӧ��*/
const MTC_MSG_PROC_STRU                 g_astMtcMsgProcModem0LteTab[] =
{
    {ID_RRC_MTC_INTRUSION_ACTION_SET_CNF,   MTC_RcvIntrusionActionSetCnf},      /* ���ſ��ƿ������ûظ� */
    {ID_RRC_MTC_INTRUSION_BAND_SET_CNF,     MTC_RcvIntrusionBandSetCnf},        /* ���ſ��ƽ���Ƶ�����ûظ� */
    {ID_RRC_MTC_INTRUSION_BAND_INFO_IND,    MTC_RcvIntrusionBandInfoInd},       /* Ƶ����Ϣ�����ϱ� */
    {ID_RRC_MTC_AREA_LOST_IND,              MTC_RcvRrcAreaLostInd},             /* ����㶪����Ϣ�����ϱ� */
    {ID_RRC_MTC_AREA_AVALIABLE_IND,         MTC_RcvRrcAreaAvaliableInd},        /* ���������ָ���Ϣ�����ϱ� */
    {ID_RRC_MTC_USING_FREQ_IND,             MTC_RcvRrcUsingFreqInd},            /* ����������ϱ�Ƶ��仯��Ϣ */
};

/* MTCģ�鴦������Modem0Gas��Ϣ������Ӧ��*/
const MTC_MSG_PROC_STRU                 g_astMtcMsgProcModem0GasTab[] =
{
    {ID_RRC_MTC_USING_FREQ_IND,             MTC_RcvRrcUsingFreqInd},            /* ����������ϱ�Ƶ��仯��Ϣ */
};

/* MTCģ�鴦������Modem0 Was��Ϣ������Ӧ��*/
const MTC_MSG_PROC_STRU                 g_astMtcMsgProcModem0WasTab[] =
{
    {ID_RRC_MTC_USING_FREQ_IND,             MTC_RcvRrcUsingFreqInd},            /* ����������ϱ�Ƶ��仯��Ϣ */
};

/* MTCģ�鴦������CMMCA��Ϣ������Ӧ��*/
const MTC_MSG_PROC_STRU                 g_astMtcMsgProcCmmcaTab[] =
{
    {ID_CMMCA_MTC_OTHER_RAT_INFO_IND,       MTC_RcvCmmcaOtherRatInfoInd},
};


/*****************************************************************************
  3 ��������
*****************************************************************************/

VOS_UINT32 MTC_SearchMsgProcTab(
    VOS_UINT32                          ulMsgCnt,
    VOS_VOID                           *pstMsg,
    const MTC_MSG_PROC_STRU            *pstMsgProcTab
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
            /* �쳣���� */
            if (VOS_NULL_PTR != pstMsgProcTab[ulLoop].pProcMsgFunc)
            {
                pstMsgProcTab[ulLoop].pProcMsgFunc(pstMsg);
            }
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


VOS_VOID MTC_RcvModem0MmaMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtcMsgProcModem0MmaTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtcMsgProcModem0MmaTab) / sizeof(MTC_MSG_PROC_STRU);

    /*g_astMtcMsgProcModem0MmaTab���������Ϣ�ַ�*/
    ulRst = MTC_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtcMsgProcModem0MmaTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTC_ERROR_LOG("MTC_RcvModem0MmaMsg: Msg Id is invalid!");
    }

    return;
}
VOS_VOID MTC_RcvModem1MmaMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtcMsgProcModem1MmaTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtcMsgProcModem1MmaTab) / sizeof(MTC_MSG_PROC_STRU);

    /*g_astMtcMsgProcModem1MmaTab���������Ϣ�ַ�*/
    ulRst = MTC_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtcMsgProcModem1MmaTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTC_ERROR_LOG("MTC_RcvModem1MmaMsg: Msg Id is invalid!");
    }

    return;
}
VOS_VOID MTC_RcvModem0TafMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtcMsgProcModem0TafTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtcMsgProcModem0TafTab) / sizeof(MTC_MSG_PROC_STRU);

    /*g_astMtcMsgProcModem0TafTab���������Ϣ�ַ�*/
    ulRst = MTC_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtcMsgProcModem0TafTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTC_ERROR_LOG("MTC_RcvModem0TafMsg: Msg Id is invalid!");
    }

    return;
}
VOS_VOID MTC_RcvModem1TafMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtcMsgProcModem1TafTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtcMsgProcModem1TafTab) / sizeof(MTC_MSG_PROC_STRU);

    /*g_astMtcMsgProcModem1TafTab���������Ϣ�ַ�*/
    ulRst = MTC_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtcMsgProcModem1TafTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTC_ERROR_LOG("MTC_RcvModem1TafMsg: Msg Id is invalid!");
    }

    return;
}
VOS_VOID MTC_RcvModem1GasMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtcMsgProcModem1GasTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtcMsgProcModem1GasTab) / sizeof(MTC_MSG_PROC_STRU);

    /*g_astMtcMsgProcModem1GasTab���������Ϣ�ַ�*/
    ulRst = MTC_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtcMsgProcModem1GasTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTC_ERROR_LOG("MTC_RcvModem1GasMsg: Msg Id is invalid!");
    }

    return;
}
VOS_VOID MTC_RcvModem0TdsMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtcMsgProcModem0TdsTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtcMsgProcModem0TdsTab) / sizeof(MTC_MSG_PROC_STRU);

    /*g_astMtcMsgProcModem0TdsTab���������Ϣ�ַ�*/
    ulRst = MTC_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtcMsgProcModem0TdsTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTC_ERROR_LOG("MTC_RcvModem0TdsMsg: Msg Id is invalid!");
    }

    return;
}
VOS_VOID MTC_RcvModem0LteMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtcMsgProcModem0LteTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtcMsgProcModem0LteTab)/sizeof(MTC_MSG_PROC_STRU);

    /*g_astMtcMsgProcModem0LteTab���������Ϣ�ַ�*/
    ulRst = MTC_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtcMsgProcModem0LteTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTC_ERROR_LOG("MTC_RcvModem0LteMsg: Msg Id is invalid!");
    }

    return;
}
VOS_VOID MTC_RcvCmmcaMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtcMsgProcCmmcaTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtcMsgProcCmmcaTab)/sizeof(MTC_MSG_PROC_STRU);

    /*g_astMtcMsgProcCmmcaTab���������Ϣ�ַ�*/
    ulRst = MTC_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtcMsgProcCmmcaTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTC_ERROR_LOG("MTC_RcvCmmcaMsg: Msg Id is invalid!");
    }

    return;
}
VOS_VOID MTC_RcvModem0GasMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtcMsgProcModem0GasTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtcMsgProcModem0GasTab)/sizeof(MTC_MSG_PROC_STRU);

    /*g_astMtcMsgProcModem0GasTab���������Ϣ�ַ�*/
    ulRst = MTC_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtcMsgProcModem0GasTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTC_ERROR_LOG("MTC_RcvModem0GasMsg: Msg Id is invalid!");
    }

    return;
}
VOS_VOID MTC_RcvModem0WasMsg(struct MsgCB *pstMsg)
{
    VOS_UINT32                          ulMsgCnt;
    VOS_UINT32                          ulRst;

    /* ��g_astMtcMsgProcModem0WasTab�л�ȡ��Ϣ���� */
    ulMsgCnt = sizeof(g_astMtcMsgProcModem0WasTab)/sizeof(MTC_MSG_PROC_STRU);

    /*g_astMtcMsgProcModem0WasTab���������Ϣ�ַ�*/
    ulRst = MTC_SearchMsgProcTab(ulMsgCnt, pstMsg, g_astMtcMsgProcModem0WasTab);

    /* û���ҵ�ƥ�����Ϣ */
    if (VOS_ERR == ulRst)
    {
        MTC_ERROR_LOG("MTC_RcvModem0WasMsg: Msg Id is invalid!");
    }

    return;
}
VOS_VOID MTC_ProcMsg(MsgBlock* pMsg)
{
    if (VOS_NULL_PTR == pMsg)
    {
        MTC_ERROR_LOG("MTC_ProcMsg: pMsg Null.");
        return;
    }

    /* ��Ϣ�ķַ����� */
    switch (pMsg->ulSenderPid)
    {
        /* ����MMA����Ϣ */
        case I0_WUEPS_PID_MMA:
            MTC_RcvModem0MmaMsg(pMsg);
            break;

        case I1_WUEPS_PID_MMA:
            MTC_RcvModem1MmaMsg(pMsg);
            break;

        /* ����TAF����Ϣ */
        case I0_WUEPS_PID_TAF:
            MTC_RcvModem0TafMsg(pMsg);
            break;

        case I1_WUEPS_PID_TAF:
            MTC_RcvModem1TafMsg(pMsg);
            break;

        /* ����modem1 G��������Ϣ */
        case I1_UEPS_PID_GAS:
            MTC_RcvModem1GasMsg(pMsg);
            return;

        /* ����modem1 G��������Ϣ */
        case I0_UEPS_PID_GAS:
            MTC_RcvModem0GasMsg(pMsg);
            return;

        /* ����modem1 W��������Ϣ */
        case WUEPS_PID_WRR:
            MTC_RcvModem0WasMsg(pMsg);
            return;

#if (FEATURE_ON == FEATURE_UE_MODE_TDS)
        /* ����modem0 T��������Ϣ */
        case TPS_PID_RRC:
            MTC_RcvModem0TdsMsg(pMsg);
            return;
#endif

#if (FEATURE_ON == FEATURE_LTE)
        /* ����modem0 L��������Ϣ */
        case PS_PID_ERRC:
            MTC_RcvModem0LteMsg(pMsg);
            return;
#endif

        /* ����CBPCA����Ϣ */
        case WUEPS_PID_CMMCA:
            MTC_RcvCmmcaMsg(pMsg);
            return;

        default:
            return;
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


