

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "pslog.h"
#include "om.h"
#include "TafTypeDef.h"
#include "MmaMmcInterface.h"
#include "TafMmaFsmPhoneMode.h"
#include "TafMmaCtx.h"
#include "TafSdcCtx.h"
#include "TafOamInterface.h"
#include "Taf_Status.h"
#include "MmaAppLocal.h"
#include "rfa.h"
#include "NasUsimmApi.h"
#include "msp_nvim.h"
#include "TTFMemInterface.h"
#include "TafMmaMain.h"
#include "TafMmaFsmPhoneModeTbl.h"
#include "TafMmaSndInternalMsg.h"
#include "TafLog.h"
#include "TafMmaTimerMgmt.h"
#include "TafAppMma.h"
#include "TafMmaSndImsa.h"
#include "TafMmaSndTaf.h"
#include "TafMmaProcNvim.h"
#include "TafMmaSndApp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 ��������
*****************************************************************************/

#define    THIS_FILE_ID        PS_FILE_ID_TAF_MMA_FSM_PHONE_MODE_C

/*****************************************************************************
  3 ���Ͷ���
*****************************************************************************/

/*****************************************************************************
  4 ��������
*****************************************************************************/

/*****************************************************************************
  5 ��������
*****************************************************************************/
extern VOS_UINT8                                g_ucMmaOmConnectFlg;

extern VOS_UINT8                                g_ucMmaOmPcRecurEnableFlg;

extern MMA_DEACT_SIM_WHEN_POWEROFF_ENUM_UINT8   g_ucMmaDeactSimWhenPoweroff;

extern MMA_ME_PERSONALISATION_STATUS_STRU       g_stMmaMePersonalisationStatus;

extern STATUS_CONTEXT_STRU                      g_StatusContext;

extern TAF_MMA_GLOBAL_VALUE_ST                  gstMmaValue;

extern MMA_TIMER_ST                             g_stPnnListMemProtectTimer;

extern MMA_TIMER_ST                             g_stPowerDownDelayTimer;

extern MN_PH_REG_TIME_INFO_STRU                 g_stRegTimeInfo;

extern VOS_VOID                                 SLEEP_GBBPIsReady(VOS_VOID);

/*****************************************************************************
  6 ����ʵ��
*****************************************************************************/

VOS_UINT32 TAF_MMA_RcvMmaInterPowerInit_PhoneMode_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ����������Ϣ */
    TAF_MMA_SaveCurEntryMsg(ulEventType, pstMsg);

    /* Ǩ��״̬���ȴ�USIM�Ŀ�״̬�ϱ� */
    TAF_MMA_FSM_SetCurrState(TAF_MMA_PHONE_MODE_STA_WAIT_PIH_USIM_STATUS_IND);

    /* ����������ʱ��,��Ҫ������ʱ��������ֹͣ���� */
    TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_PIH_USIM_STATUS_IND, TI_TAF_MMA_WAIT_PIH_USIM_STATUS_IND_LEN);

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvMmaInterUsimStatusChangeInd_PhoneMode_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ����������Ϣ */
    TAF_MMA_SaveCurEntryMsg(ulEventType, pstMsg);

    TAF_MMA_ProcPowerOffReq_PhoneMode();

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvOMPhoneModeSet_PhoneMode_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PH_OP_MODE_STRU                *pstPhModeSet    = VOS_NULL_PTR;
    MN_APP_REQ_MSG_STRU                *pstRcvMsg       = VOS_NULL_PTR;

    pstRcvMsg           = (MN_APP_REQ_MSG_STRU*)pstMsg;
    pstPhModeSet        = (TAF_PH_OP_MODE_STRU *)pstRcvMsg->aucContent;

    /* ����������Ϣ */
    TAF_MMA_SaveCurEntryMsg(ulEventType, pstMsg);

    /* ����ΪFULLģʽ���п��� */
    if (TAF_PH_MODE_FULL == pstPhModeSet->PhMode)
    {
         /* PC�ط�ʱ���ж�USIM�Ŀ�״̬ */
#if defined(__PC_TRACE_RECUR__)
#else
        if (TAF_SDC_USIM_STATUS_BUTT == TAF_SDC_GetSimStatus())
        {
            /* Ǩ��״̬�ȴ���״̬�ϱ� */
            TAF_MMA_FSM_SetCurrState(TAF_MMA_PHONE_MODE_STA_WAIT_PIH_USIM_STATUS_IND);

            /* ������״̬�ϱ�������ʱ�� */
            TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_PIH_USIM_STATUS_IND, TI_TAF_MMA_WAIT_PIH_USIM_STATUS_IND_LEN);

            return VOS_TRUE;
        }
#endif

        /* ���п������� */
        TAF_MMA_ProcStartReq_PhoneMode();

        return VOS_TRUE;
    }

    /* �����߹ػ����� */
    TAF_MMA_ProcPowerOffReq_PhoneMode();

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvTafPhoneModeSet_PhoneMode_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_PHONE_MODE_SET_REQ_STRU    *pstPhModeSet    = VOS_NULL_PTR;

    pstPhModeSet = (TAF_MMA_PHONE_MODE_SET_REQ_STRU*)pstMsg;

    /* ����������Ϣ */
    TAF_MMA_SaveCurEntryMsg(ulEventType, pstMsg);

    /* ����ΪFULLģʽ���п��� */
    if (TAF_PH_MODE_FULL == pstPhModeSet->stPhoneModePara.PhMode)
    {
         /* PC�ط�ʱ���ж�USIM�Ŀ�״̬ */
#if defined(__PC_TRACE_RECUR__)
#else
        if (TAF_SDC_USIM_STATUS_BUTT == TAF_SDC_GetSimStatus())
        {
            /* Ǩ��״̬�ȴ���״̬�ϱ� */
            TAF_MMA_FSM_SetCurrState(TAF_MMA_PHONE_MODE_STA_WAIT_PIH_USIM_STATUS_IND);

            /* ������״̬�ϱ�������ʱ�� */
            TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_PIH_USIM_STATUS_IND, TI_TAF_MMA_WAIT_PIH_USIM_STATUS_IND_LEN);

            return VOS_TRUE;
        }
#endif

        /* ���п������� */
        TAF_MMA_ProcStartReq_PhoneMode();

        return VOS_TRUE;
    }

    /* �����߹ػ����� */
    TAF_MMA_ProcPowerOffReq_PhoneMode();

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvPihUsimStatusInd_PhoneMode_WaitPihUsimStatusInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֹͣ������ʱ�� */
    TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_PIH_USIM_STATUS_IND);

    /* �ϵ��һ���յ���״̬�仯,�Զ�����������ҪͶƱ��ֹ˯�� */
    TAF_DRVAPI_PWRCTRL_SLEEPVOTE_LOCK(PWRCTRL_SLEEP_NAS);

    if (VOS_TRUE == TAF_MMA_GetMmaLogInfoFlag())
    {
        vos_printf("\n*TAF_MMA_RcvPihUsimStatusInd_PhoneMode_WaitPihUsimStatusInd:PWRCTRL_SLEEP_NAS,TAF_DRVAPI_PWRCTRL_SLEEPVOTE_LOCK\n");
    }

#ifndef __PS_WIN32_RECUR__
    if ( (VOS_TRUE == g_ucMmaOmConnectFlg)
      && (VOS_TRUE == g_ucMmaOmPcRecurEnableFlg) )
    {
        NAS_MMA_SndOutsideFixedContextData();
    }
#endif

    TAF_MMA_ProcStartReq_PhoneMode();

    return VOS_TRUE;
}




VOS_UINT32 TAF_MMA_RcvTiWaitPihUsimStatusIndExpired_PhoneMode_WaitPihUsimStatusInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* warning ��ӡ */
    TAF_WARNING_LOG(WUEPS_PID_MMA, "TAF_MMA_RcvTiWaitPihUsimStatusIndExpired_PhoneMode_WaitPihUsimStatusInd:time expired!");

    /* ���¿�״̬Ϊ����Ч */
    TAF_SDC_SetSimStatus(TAF_SDC_USIM_STATUS_READY_TIMEROUT);

    /* �ϵ��һ���յ���״̬�仯,�Զ�����������ҪͶƱ��ֹ˯�� */
    TAF_DRVAPI_PWRCTRL_SLEEPVOTE_LOCK(PWRCTRL_SLEEP_NAS);

    if (VOS_TRUE == TAF_MMA_GetMmaLogInfoFlag())
    {
        vos_printf("\n*TAF_MMA_RcvTiWaitPihUsimStatusIndExpired_PhoneMode_WaitPihUsimStatusInd:PWRCTRL_SLEEP_NAS,TAF_DRVAPI_PWRCTRL_SLEEPVOTE_LOCK\n");
    }

    TAF_MMA_ProcStartReq_PhoneMode();

    return VOS_TRUE;
}

#if (FEATURE_IMS == FEATURE_ON)
VOS_UINT32 TAF_MMA_RcvImsaStartCnf_PhoneMode_WaitImsaStartCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֹͣ������ʱ�� */
    TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_IMSA_START_CNF);

    /* ����MMC���Ϳ������� */
    TAF_MMA_SndStartReq_PhoneMode(WUEPS_PID_MMC);

    return VOS_TRUE;
}



VOS_UINT32 TAF_MMA_RcvTiWaitImsaStartCnfExpired_PhoneMode_WaitImsaStartCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* warning��ӡ */
    TAF_WARNING_LOG(WUEPS_PID_MMA, "TAF_MMA_RcvTiWaitImsaStartCnfExpired_PhoneMode_WaitImsaStartCnf:time expired!");

    /* ����MMC���Ϳ������� */
    TAF_MMA_SndStartReq_PhoneMode(WUEPS_PID_MMC);

    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvImsaPowerOffCnf_PhoneMode_WaitImsaPowerOffCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ֹͣ������ʱ�� */
    TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_IMSA_POWER_OFF_CNF);

    /* ����MMC���͹ػ����� */
    TAF_MMA_SndPowerOffReq_PhoneMode(WUEPS_PID_MMC);

    return VOS_TRUE;
}



VOS_UINT32 TAF_MMA_RcvTiWaitImsaPowerOffCnfExpired_PhoneMode_WaitImsaPowerOffCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* warning��ӡ */
    TAF_WARNING_LOG(WUEPS_PID_MMA, "TAF_MMA_RcvTiWaitImsaPowerOffCnfExpired_PhoneMode_WaitImsaPowerOffCnf:time expired!");

    /* ����MMC���͹ػ����� */
    TAF_MMA_SndPowerOffReq_PhoneMode(WUEPS_PID_MMC);

    return VOS_TRUE;
}
#endif



VOS_VOID TAF_MMA_SndStartReq_PhoneMode(
    VOS_UINT32                          ulReceivePid
)
{
    MMA_MMC_CARD_STATUS_ENUM_UINT8      enCardStatus;
    MMA_MMC_PLMN_RAT_PRIO_STRU          stPlmnRatPrio;
    TAF_PH_RAT_ORDER_STRU              *pstRatPrioList = VOS_NULL_PTR;

    if (WUEPS_PID_MMC == ulReceivePid)
    {
        /* ��ȡ��״̬ */
        enCardStatus = TAF_MMA_ConvertSdcSimStatusToMmcFormat_PhoneMode(TAF_SDC_GetSimStatus());

        /* ��ȡ���뼼����Ϣ */
        pstRatPrioList = MN_MMA_GetRatPrioListAddr();

        PS_MEM_SET(&stPlmnRatPrio, 0, sizeof(MMA_MMC_PLMN_RAT_PRIO_STRU));

        stPlmnRatPrio.ucRatNum = pstRatPrioList->ucRatOrderNum;
        PS_MEM_CPY(stPlmnRatPrio.aucRatPrio,
                   pstRatPrioList->aenRatOrder,
                   MMA_MMC_MAX_RAT_NUM);

        TAF_MMA_GetAvailablePlmnRatPrio(&stPlmnRatPrio);

        /* ���Ϳ��������MMC */
        TAF_MMA_SndMmcStartReq(enCardStatus, &stPlmnRatPrio);

        /* Ǩ��״̬���ȴ�MMC�Ŀ����ظ� */
        TAF_MMA_FSM_SetCurrState(TAF_MMA_PHONE_MODE_STA_WAIT_MMC_START_CNF);

        /* ����������ʱ�� */
        TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_MMC_START_CNF, TI_TAF_MMA_WAIT_MMC_START_CNF_LEN);

        if (VOS_TRUE == TAF_MMA_GetMmaLogInfoFlag())
        {
            vos_printf("\n*TAF_MMA_SndStartReq_PhoneMode:ulReceivePid = %d\n", ulReceivePid);
        }

        return;
    }

#if (FEATURE_ON == FEATURE_IMS)
    if (PS_PID_IMSA == ulReceivePid)
    {
        /* ���Ϳ��������IMSA */
        TAF_MMA_SndImsaStartReq();

        /* Ǩ��״̬���ȴ�IMSA�Ŀ����ظ� */
        TAF_MMA_FSM_SetCurrState(TAF_MMA_PHONE_MODE_STA_WAIT_IMSA_START_CNF);

        /* ����������ʱ�� */
        TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_IMSA_START_CNF, TI_TAF_MMA_WAIT_IMSA_START_CNF_LEN);

        if (VOS_TRUE == TAF_MMA_GetMmaLogInfoFlag())
        {
            vos_printf("\n*TAF_MMA_SndStartReq_PhoneMode:ulReceivePid = %d\n", ulReceivePid);
        }
    }
#endif

    return;
}


VOS_VOID TAF_MMA_SndPowerOffReq_PhoneMode(
    VOS_UINT32                          ulReceivePid
)
{
    MMA_MMC_POWER_OFF_CAUSE_ENUM_UINT32 enCause;
    VOS_UINT32                          ulEntryMsgId;

    if (WUEPS_PID_MMC == ulReceivePid)
    {
        ulEntryMsgId = TAF_MMA_GetCurrFsmEntryMsgId_PhoneMode();

        /* ͨ��״̬�������Ϣ��ȡ�ػ�ԭ��ֵ */
        if ((MMA_MMA_INTER_USIM_STATUS_CHANGE_IND == ulEntryMsgId)
         || (MMA_MMA_SIM_LOCK_STATUS_CHANGE_IND   == ulEntryMsgId))
        {
            enCause  = MMA_MMC_POWER_OFF_CAUSE_SIM_REMOVE;
        }
        else
        {
            enCause  = MMA_MMC_POWER_OFF_CAUSE_NORMAL;
        }

        NAS_TRACE_HIGH("Recive from MMC");

        TAF_MMA_SndMmcPowerOffReq(enCause);

        /* Ǩ��״̬���ȴ�MMC�Ĺػ��ظ� */
        TAF_MMA_FSM_SetCurrState(TAF_MMA_PHONE_MODE_STA_WAIT_MMC_POWER_OFF_CNF);

        /* ����������ʱ�� */
        TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_MMC_POWER_OFF_CNF, TI_TAF_MMA_WAIT_MMC_POWER_OFF_CNF_LEN);

        return;
    }

#if (FEATURE_ON == FEATURE_IMS)
    if (PS_PID_IMSA == ulReceivePid)
    {
        /* ���͹ػ������IMSA */
        TAF_MMA_SndImsaStopReq();

        /* Ǩ��״̬���ȴ�IMSA�Ĺػ��ظ� */
        TAF_MMA_FSM_SetCurrState(TAF_MMA_PHONE_MODE_STA_WAIT_IMSA_POWER_OFF_CNF);

        /* ����������ʱ�� */
        TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_IMSA_POWER_OFF_CNF, TI_TAF_MMA_WAIT_IMSA_POWER_OFF_CNF_LEN);
    }
#endif

    return;
}



VOS_VOID TAF_MMA_ProcStartCnfSucc_NoPowerInit_PhoneMode(VOS_VOID)
{
    VOS_UINT8                                               ucNeedPinFlg;
    VOS_UINT8                                               ucSdcSimStatus;
    VOS_UINT16                                              usAppCfgSupportType;

    /* ���µ�ǰ��ģʽΪFULLģʽ */
    TAF_SDC_SetCurPhoneMode(TAF_PH_MODE_FULL);

    usAppCfgSupportType     = TAF_SDC_GetAppCfgSupportType();
    ucSdcSimStatus          = TAF_SDC_GetSimStatus();
    ucNeedPinFlg            = VOS_FALSE;

    /* ���µ�ǰ��ģʽΪFULLģʽ */
    TAF_SDC_SetCurPhoneMode(TAF_PH_MODE_FULL);

    if ((TAF_SDC_USIM_STATUS_SIM_PIN == ucSdcSimStatus)
     || (TAF_SDC_USIM_STATUS_SIM_PUK == ucSdcSimStatus))
    {
       ucNeedPinFlg = VOS_TRUE;
    }

    if (VOS_FALSE == ucNeedPinFlg)
    {
        MMA_GetMeLockStatus();
    }

    TTF_MemCcpuCheckPoolLeak();

    TAF_MMA_ReportPhoneModeCnf_PhoneMode();

    if (SYSTEM_APP_WEBUI == usAppCfgSupportType)
    {
        if ((VOS_FALSE == ucNeedPinFlg)
         && (MMA_SIM_IS_LOCK == g_stMmaMePersonalisationStatus.SimLockStatus))
        {
            MN_PH_SimLockedRpt();
        }
    }

#if ( FEATURE_MULTI_MODEM == FEATURE_ON )
    TAF_MMA_SndMtcPowerStateInd(MTC_MODEM_POWER_ON);
#endif


    TAF_MMA_SndMmcModeChangeReq((MMA_MMC_MS_MODE_ENUM_UINT32)g_StatusContext.ucModeService);


    /*��ȫ�ֱ�������ģʽ�жϣ������ΪC+L������ģʽ��������ע������*/
    if (VOS_TRUE != TAF_MMA_IsPowerOnCLInterWork())
    {
        /* SLEEP_GBBPIsReadyͬ���ӿڣ�ֻ�����Ѿ�readyʱ���Ż��NAS���� */
        NAS_TRACE_HIGH("Start call API:SLEEP_GBBPIsReady");
        
        SLEEP_GBBPIsReady();
        
        NAS_TRACE_HIGH("End call API:SLEEP_GBBPIsReady");

        TAF_MMA_SndMmcPlmnSearchReq();

        Sta_AttachByModeService(STA_OPID_ATTATCH);
    }

#if defined(__PS_WIN32_RECUR__) || defined(NAS_STUB)
    if (TAF_SDC_USIM_STATUS_VALID == TAF_SDC_GetSimStatus())
    {
        MN_PH_SndMsgUsimStatus(USIMM_CARD_SERVIC_AVAILABLE);
    }
    else
    {
        MN_PH_SndMsgUsimStatus(USIMM_CARD_SERVIC_UNAVAILABLE);
    }
#endif




    /* ����״̬���˳���Ϣ */
    TAF_MMA_SndPhoneModeRsltInd(TAF_MMA_PHONE_MODE_RESULT_SWITCH_ON_SUCC);

    /* �˳�״̬�� */
    TAF_MMA_FSM_QuitSubFsm();

    return;
}


VOS_VOID TAF_MMA_ProcStartCnfSucc_PowerInit_PhoneMode(VOS_VOID)
{
    VOS_UINT32                          ulAutoSwitchOnFlg;
    VOS_UINT8                           ucSdcSimStatus;

    ulAutoSwitchOnFlg       = TAF_MMA_GetAutoSwitchOnFlg();

    /* ��E5�µĳ��ģʽ���Զ��ϵ翪��NV�ر�ʱ���йػ����� */
    if ((VOS_FALSE == (VOS_UINT32)DRV_START_MODEGET())
     || (VOS_FALSE == ulAutoSwitchOnFlg))
    {
        NAS_TRACE_HIGH("Start mode is %d", (VOS_UINT32)DRV_START_MODEGET());

        /* �ϵ粻�Զ�������NAS��Ҫ�ر�RF */
        MMA_TurnOffForFlightMode();

        /* �߹ػ����� */
        TAF_MMA_ProcPowerOffReq_PhoneMode();

        return;
    }

    /* ���µ�ǰ��ģʽΪFULLģʽ */
    TAF_SDC_SetCurPhoneMode(TAF_PH_MODE_FULL);

    /* Ӳ���������ߵ���������ײ� */
    MMA_TurnOnForFlightMode();

    TTF_MemCcpuCheckPoolLeak();

#ifndef __PS_WIN32_RECUR__
    NAS_MMA_SndOutsideContextData();
#endif

    TAF_MMA_ReportPhoneModeInd_PhoneMode();

    ucSdcSimStatus          = TAF_SDC_GetSimStatus();

    if (SYSTEM_APP_WEBUI == TAF_SDC_GetAppCfgSupportType())
    {
        /* ����ҪPIN�뵫����ʱ�ϱ�SIM LOCKED�¼� */
        if (((TAF_SDC_USIM_STATUS_SIM_PIN != ucSdcSimStatus)
          && (TAF_SDC_USIM_STATUS_SIM_PUK != ucSdcSimStatus))
         && (MMA_SIM_IS_LOCK == g_stMmaMePersonalisationStatus.SimLockStatus))
        {
            MN_PH_SimLockedRpt();
        }
    }

#if ( FEATURE_MULTI_MODEM == FEATURE_ON )
    TAF_MMA_SndMtcPowerStateInd(MTC_MODEM_POWER_ON);
#endif

    TAF_MMA_SndMmcModeChangeReq((MMA_MMC_MS_MODE_ENUM_UINT32)g_StatusContext.ucModeService);


    /*��ȫ�ֱ�������ģʽ�жϣ������ΪC+L������ģʽ��������ע������,����ȴ�CMMCA��������ע������ */
    if (VOS_TRUE != TAF_MMA_IsPowerOnCLInterWork())
    {
        /* SLEEP_GBBP_IsReadyͬ���ӿڣ�ֻ�����Ѿ�readyʱ���Ż��NAS���� */
        NAS_TRACE_HIGH("Start call API:SLEEP_GBBPIsReady");
        
        SLEEP_GBBPIsReady();
        
        NAS_TRACE_HIGH("End call API:SLEEP_GBBPIsReady");

        TAF_MMA_SndMmcPlmnSearchReq();

        Sta_AttachByModeService(STA_OPID_ATTATCH);
    }

#if defined(__PS_WIN32_RECUR__) || defined(NAS_STUB)
    if (TAF_SDC_USIM_STATUS_VALID == ucSdcSimStatus)
    {
        MN_PH_SndMsgUsimStatus(USIMM_CARD_SERVIC_AVAILABLE);
    }
    else
    {
        MN_PH_SndMsgUsimStatus(USIMM_CARD_SERVIC_UNAVAILABLE);
    }
#endif

    /* �յ�rrmm-start-cnf��Ϣ��
     ��ǰ���Զ�������
        MMA��Ҫ�ͷ�ͶƱ,��AS����plmn_serach_reqʱ,��OAMͶ����˯��Ʊ,
     ��ǰ�Ƿ��Զ�������
        MMA��Ҫ�ͷ�ͶƱ,����˯��*/
    if (VOS_TRUE == TAF_MMA_GetMmaLogInfoFlag())
    {
        vos_printf("\n*TAF_MMA_ProcStartCnfSucc_PowerInit_PhoneMode:PWRCTRL_SLEEP_NAS,TAF_DRVAPI_PWRCTRL_SLEEPVOTE_UNLOCK\n");
    }

    TAF_DRVAPI_PWRCTRL_SLEEPVOTE_UNLOCK(PWRCTRL_SLEEP_NAS);


    /* ����״̬���˳���Ϣ */
    TAF_MMA_SndPhoneModeRsltInd(TAF_MMA_PHONE_MODE_RESULT_SWITCH_ON_SUCC);

    /* �˳�״̬�� */
    TAF_MMA_FSM_QuitSubFsm();

    return;
}


VOS_UINT32 TAF_MMA_RcvMmcStartCnf_PhoneMode_WaitMmcStartCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MMA_START_CNF_STRU             *pstStartCnf             = VOS_NULL_PTR;
    VOS_UINT8                           ucSimStatus;
    VOS_UINT8                           ucSimLockStatus;

    pstStartCnf             = (MMC_MMA_START_CNF_STRU*)pstMsg;
    ucSimStatus             = MMA_GetUsimStatus(VOS_FALSE);

    /* ��ȡ����״̬ */
    if ( MMA_SIM_IS_LOCK == MMA_GetMeLockStatus() )
    {
        ucSimLockStatus                 = VOS_TRUE;
    }
    else
    {
        ucSimLockStatus                 = VOS_FALSE;
    }

    /* ֹͣ������ʱ�� */
    TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_MMC_START_CNF);

    /* �յ�rrmm-start-cnf��Ϣ��
     ��ǰ���Զ�������
        MMA��Ҫ�ͷ�ͶƱ,��AS����plmn_serach_reqʱ,��OAMͶ����˯��Ʊ,
     ��ǰ�Ƿ��Զ�������
        MMA��Ҫ�ͷ�ͶƱ,����˯��*/

    if (VOS_TRUE == TAF_MMA_GetMmaLogInfoFlag())
    {
        vos_printf("\n*TAF_MMA_RcvMmcStartCnf_PhoneMode_WaitMmcStartCnf:result = %d\n", pstStartCnf->ulResult);
        vos_printf("\n*TAF_MMA_RcvMmcStartCnf_PhoneMode_WaitMmcStartCnf:PWRCTRL_SLEEP_NAS,TAF_DRVAPI_PWRCTRL_SLEEPVOTE_UNLOCK\n");
    }

    TAF_DRVAPI_PWRCTRL_SLEEPVOTE_UNLOCK(PWRCTRL_SLEEP_NAS);

    /* �ϵ翪��ʱ�ϱ�AT��ǰЭ��ջ�ĳ�ʼ����� */
    if (MMA_MMA_INTER_POWER_INIT == TAF_MMA_GetCurrFsmEntryMsgId_PhoneMode())
    {
        TAF_MMA_SndAtPsInitRsltInd_PhoneMode(pstStartCnf->ulResult);

        /* ����RIL�߼�,�����ϱ�һ�ο�״̬ */
        TAF_MMA_SndAtIccStatusInd(ucSimStatus, ucSimLockStatus);
    }

    /* ����ʧ�� */
    if (TAF_FAILURE == pstStartCnf->ulResult)
    {
        TAF_MMA_SetCurPhoneErrorCode_PhoneMode(TAF_ERR_UNSPECIFIED_ERROR);

        TAF_MMA_ReportPhoneModeCnf_PhoneMode();

        /* ����״̬���˳���Ϣ */
        TAF_MMA_SndPhoneModeRsltInd(TAF_MMA_PHONE_MODE_RESULT_SWITCH_ON_FAIL);

        /* �˳�״̬�� */
        TAF_MMA_FSM_QuitSubFsm();

        return VOS_TRUE;
    }

    /* �����ɹ�Ӧ�������ô�״̬,��Ϊ������attach�ȶ������жϴ�״̬ */
    g_StatusContext.ulFsmState = STA_FSM_ENABLE;

    /* ����SDC�е�SIM����ע��״̬ */
    TAF_MMA_UpdateSimRegStatus();

    /* ͨ��״̬�������Ϣ�ж��Ƿ�Ϊ�ϵ翪�� */
    if (MMA_MMA_INTER_POWER_INIT != TAF_MMA_GetCurrFsmEntryMsgId_PhoneMode())
    {
        TAF_MMA_ProcStartCnfSucc_NoPowerInit_PhoneMode();

        return VOS_TRUE;
    }

    /* �ϵ翪���ɹ��Ľ������ */
    TAF_MMA_ProcStartCnfSucc_PowerInit_PhoneMode();

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvTiWaitMmcStartCnfExpired_PhoneMode_WaitMmcStartCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (VOS_TRUE == TAF_MMA_GetMmaLogInfoFlag())
    {
        vos_printf("\n*TAF_MMA_RcvTiWaitMmcStartCnfExpired_PhoneMode_WaitMmcStartCnf\n");
    }

    /* warning��ӡ */
    TAF_WARNING_LOG(WUEPS_PID_MMA, "TAF_MMA_RcvTiWaitMmcStartCnfExpired_PhoneMode_WaitMmcStartCnf:time expired!");

    NAS_TRACE_HIGH("Enter!");

    TAF_MMA_SetCurPhoneErrorCode_PhoneMode(TAF_ERR_TIME_OUT);

    /* �ϱ�APP��ǰ״̬�¼� */
    TAF_MMA_ReportPhoneModeCnf_PhoneMode();

    /* ����״̬���˳������Ϣ */
    TAF_MMA_SndPhoneModeRsltInd(TAF_MMA_PHONE_MODE_RESULT_SWITCH_ON_FAIL);

    /* �˳�״̬�� */
    TAF_MMA_FSM_QuitSubFsm();

    return VOS_TRUE;
}


VOS_VOID TAF_MMA_ProcPowerOffCnf_UsimStatusChange_PhoneMode(VOS_VOID)
{
    /* ����ʱ��ɾ��EPLMN��Ϣ */
    if (TAF_SDC_USIM_STATUS_NO_PRESENT == TAF_SDC_GetSimStatus())
    {
        MMA_ClearEplmnFromNV();
        TAF_INFO_LOG(WUEPS_PID_MMA, "TAF_MMA_ProcPowerOffCnf_UsimStatusChange_PhoneMode:EPLMN is cleared:");
    }

    /* ��ʼ��ȫ�ֱ��� */
    TAF_MMA_InitCtx(TAF_MMA_INIT_CTX_POWEROFF, TAF_MMA_GetMmaCtxAddr());

    /* ���ڲ��������� */
    TAF_MMA_ProcStartReq_PhoneMode();

    return;
}
VOS_VOID TAF_MMA_ProcPowerOffCnfWithPowerOffMode_PhoneMode(VOS_VOID)
{
    VOS_UINT16                          usAppCfgSupportType;
    VOS_UINT32                          ulRslt;

    usAppCfgSupportType     = TAF_SDC_GetAppCfgSupportType();

    if (SYSTEM_APP_WEBUI == usAppCfgSupportType)
    {
        /* �ر���Ƶ */
        MMA_TurnOffForFlightMode();




        /* ��TASKDELAY�޸�Ϊ������ʱ�����б��� */
        ulRslt = NAS_StartRelTimer(&g_stPowerDownDelayTimer.MmaHTimer,
                              WUEPS_PID_MMA,
                              TI_TAF_MMA_DELAY_POWER_DOWN_TIMER_LEN,
                              TAF_MMA,
                              TI_TAF_MMA_DELAY_POWER_DOWN,
                              VOS_RELTIMER_NOLOOP );

        if( VOS_OK !=  ulRslt)
        {
            MMA_WARNINGLOG("Sta_PowerOffCnf():ERROR:Start TI_TAF_MMA_DELAY_POWER_DOWN Timer Error!");
            DRV_POWER_OFF();
        }



        return;
    }

    if (SYSTEM_APP_ANDROID   == usAppCfgSupportType)
    {
        /* ȥ����USIM�� */
        NAS_USIMMAPI_DeactiveCardReq(WUEPS_PID_MMA);

        /* �ر���Ƶ */
        MMA_TurnOffForFlightMode();

        /* ���ϱ��¼�ǰ�ȵ���COMM�ӿ�Flush NV */
#if (FEATURE_ON == FEATURE_LTE)
        NVM_Flush();
#endif
        NV_Flush();
    }

    return;
}


VOS_VOID TAF_MMA_ProcPowerOffCnfWithSpecPhoneMode_PhoneMode(
    VOS_UINT8                           ucPhoneMode
)
{
    switch (ucPhoneMode)
    {
        case TAF_PH_MODE_POWEROFF:

            TAF_MMA_ProcPowerOffCnfWithPowerOffMode_PhoneMode();
            break;

        case TAF_PH_MODE_MINI:

            MMA_TurnOffForFlightMode();

#if (FEATURE_ON == FEATURE_LTE)
            NVM_Flush();
#endif
            NV_Flush();

            /* ��"AT+CFUN=0������ػ���ȥ����(U)SIM������"������ʹ�ܣ���ȥ����SIM�� */
            if (MMA_DEACT_SIM_WHEN_POWEROFF_ENABLE == g_ucMmaDeactSimWhenPoweroff)
            {
                NAS_USIMMAPI_DeactiveCardReq(WUEPS_PID_MMA);
            }

            break;

        case TAF_PH_MODE_RFOFF:
        case TAF_PH_MODE_LOWPOWER:
        case TAF_PH_MODE_VDFMINI:

            MMA_TurnOffForFlightMode();
            break;

        default:

            TAF_WARNING_LOG(WUEPS_PID_MMA, "TAF_MMA_ProcPowerOffCnfWithSpecPhoneMode_PhoneMode:No need to deal with this mode!");
            break;
    }

    return;
}
VOS_UINT32 TAF_MMA_RcvMmcPowerOffCnf_PhoneMode_WaitMmcPowerOffCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucPhoneMode;
    VOS_UINT32                          ulEntryMsgId;
#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    MMA_MTC_CURR_CAMP_PLMN_INFO_IND_STRU    stSndMtcPlmnInfo;
#endif

    /* ֹͣ������ʱ�� */
    TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_MMC_POWER_OFF_CNF);

    MMA_InitCm();

    /* ������֪ͨMSGһ��CS���������ػ�ʱ����Ҫ��֪ͨMSG��STKҪ��ػ�ʱҲ֪ͨһ�� */
    if (VOS_TRUE == TAF_SDC_GetSvlteSupportFlag())
    {
        TAF_MMA_SndStkCsServiceChangeNotify(TAF_CS_OFF);
    }

    /* �ϱ�����״̬�仯 */
    if (TAF_SDC_REPORT_SRVSTA_NO_SERVICE != TAF_SDC_GetServiceStatus())
    {
        TAF_SDC_SetServiceStatus(TAF_SDC_REPORT_SRVSTA_NO_SERVICE);
        Sta_EventReport((VOS_UINT32)TAF_MMA_GetCurrFsmEntryMsgOpId_PhoneMode(), TAF_PH_EVT_SERVICE_STATUS_IND);
    }

    /* �ϱ�ϵͳģʽ��ģʽ�仯 */
    Sta_ModeChangeEventReport(TAF_PH_INFO_NONE_RAT, TAF_SYS_SUBMODE_NONE);

#if  ( FEATURE_MULTI_MODEM == FEATURE_ON )
    TAF_MMA_SndMtcPowerStateInd(MTC_MODEM_POWER_OFF);

    if (VOS_TRUE == TAF_SDC_GetUtranSkipWPlmnSearchFlag())
    {
        PS_MEM_SET(&stSndMtcPlmnInfo, 0, sizeof(stSndMtcPlmnInfo));

        stSndMtcPlmnInfo.enRatMode             = MTC_RATMODE_BUTT;
        stSndMtcPlmnInfo.ucIsForbiddenPlmnFlag = VOS_FALSE;
        stSndMtcPlmnInfo.ucPsAttachAllowFlag   = TAF_SDC_GetPsAttachAllowFlg();
        stSndMtcPlmnInfo.stPlmnId.ulMcc        = TAF_SDC_INVALID_MCC;
        stSndMtcPlmnInfo.stPlmnId.ulMnc        = TAF_SDC_INVALID_MCC;
        TAF_MMA_SndMtcCurrCampPlmnInfoInd(&stSndMtcPlmnInfo);
    }
#endif

    ulEntryMsgId = TAF_MMA_GetCurrFsmEntryMsgId_PhoneMode();

    /* ͨ�������Ϣ�ж��ǿ�״̬�ı�����Ĺػ� */
    if ((MMA_MMA_INTER_USIM_STATUS_CHANGE_IND == ulEntryMsgId)
     || (MMA_MMA_SIM_LOCK_STATUS_CHANGE_IND   == ulEntryMsgId))
    {
        TAF_MMA_ProcPowerOffCnf_UsimStatusChange_PhoneMode();

        /* ����ͶƱ:�յ�MMC�Ĺػ��ظ�֮������˯��ȥ */
        TAF_DRVAPI_PWRCTRL_SLEEPVOTE_UNLOCK(PWRCTRL_SLEEP_NAS);

        return VOS_TRUE;
    }

    /* ��ȡOM��AT���õ�ģʽ */
    ucPhoneMode             = TAF_MMA_GetCurrFsmEntryMsgPhoneMode_PhoneMode();

    /* ���µ�ǰ���ֻ�ģʽ */
    TAF_SDC_SetCurPhoneMode(ucPhoneMode);

    TAF_MMA_ProcPowerOffCnfWithSpecPhoneMode_PhoneMode(ucPhoneMode);

    /* ��APP�ϱ�Ӧ���¼�*/
    TAF_MMA_ReportPhoneModeCnf_PhoneMode();

    /* ����״̬���˳������Ϣ */
    TAF_MMA_SndPhoneModeRsltInd(TAF_MMA_PHONE_MODE_RESULT_POWER_OFF_SUCC);

    /* �˳�״̬�� */
    TAF_MMA_FSM_QuitSubFsm();

    /* ����ͶƱ:�յ�MMC�Ĺػ��ظ�֮������˯��ȥ */
    TAF_DRVAPI_PWRCTRL_SLEEPVOTE_UNLOCK(PWRCTRL_SLEEP_NAS);

    return VOS_TRUE;
}



VOS_UINT32 TAF_MMA_RcvTiWaitMmcPowerOffCnfExpired_PhoneMode_WaitMmcPowerOffCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* warning��ӡ */
    TAF_WARNING_LOG(WUEPS_PID_MMA, "TAF_MMA_RcvTiWaitMmcPowerOffCnfExpired_PhoneMode_WaitMmcPowerOffCnf:time expired!");

    NAS_TRACE_HIGH("Enter!");

    TAF_MMA_SetCurPhoneErrorCode_PhoneMode(TAF_ERR_TIME_OUT);

    /* �ϱ�APP��ǰ״̬�¼� */
    TAF_MMA_ReportPhoneModeCnf_PhoneMode();

    /* ����״̬���˳������Ϣ */
    TAF_MMA_SndPhoneModeRsltInd(TAF_MMA_PHONE_MODE_RESULT_POWER_OFF_FAIL);

    /* �˳�״̬�� */
    TAF_MMA_FSM_QuitSubFsm();

    /* ����ͶƱ:�յ�MMC�Ĺػ��ظ�֮������˯��ȥ */
    TAF_DRVAPI_PWRCTRL_SLEEPVOTE_UNLOCK(PWRCTRL_SLEEP_NAS);

    return VOS_TRUE;
}
VOS_VOID TAF_MMA_ProcPowerOffReq_PhoneMode(VOS_VOID)
{
    /* ����ͶƱ:�ػ�ʱ������˯��,�յ�MMC�Ĺػ��ظ�֮���� */
    TAF_DRVAPI_PWRCTRL_SLEEPVOTE_LOCK(PWRCTRL_SLEEP_NAS);

    /* ����STOP״̬��־��*/
    g_StatusContext.ulFsmState = STA_FSM_STOP;

    /* ���������ע��ɹ�ʱ�� */
    g_stRegTimeInfo.ulCostTime      = 0;

#if (FEATURE_ON == FEATURE_IMS)
    if (VOS_TRUE == TAF_SDC_GetImsSupportFlag())
    {
        /* ����IMSA���͹ػ����� */
        TAF_MMA_SndPowerOffReq_PhoneMode(PS_PID_IMSA);
    }
    else
#endif
    {
        /* ����MMC���͹ػ����� */
        TAF_MMA_SndPowerOffReq_PhoneMode(WUEPS_PID_MMC);
    }

    /* ���TI_TAF_MMA_PERIOD_TRYING_NETWORK_SELECTION_MENU��ʱ���������У���Ҫֹͣ */
    if (MMA_TIMER_RUN == TAF_MMA_GetTiTryingPeriodNetSelMenuStatus())
    {
       if (VOS_OK != NAS_StopRelTimer(WUEPS_PID_MMA,
                                      TI_TAF_MMA_PERIOD_TRYING_NETWORK_SELECTION_MENU,
                                      &(g_StatusContext.stNetSelMenuCtx.stTiPeriodTryingNetSelMenu.MmaHTimer)))
       {
           TAF_WARNING_LOG(WUEPS_PID_MMA, "TAF_MMA_ProcPowerOffReq_PhoneMode:stop TI_TAF_MMA_PERIOD_TRYING_NETWORK_SELECTION_MENU failed!");
       }

       TAF_MMA_SetTiTryingPeriodNetSelMenuStatus(MMA_TIMER_STOP);
    }

    /* ���TI_TAF_MMA_PERIOD_TRYING_RAT_BALANCING��ʱ���������У���Ҫֹͣ */
    if (MMA_TIMER_RUN == TAF_MMA_GetTiTryingPeriodRatBalancingStatus())
    {
       if (VOS_OK != NAS_StopRelTimer(WUEPS_PID_MMA,
                                      TI_TAF_MMA_PERIOD_TRYING_RAT_BALANCING,
                                      &(g_StatusContext.stRatBalancingCtx.stTiPeriodTryingRatBalancing.MmaHTimer)))
       {
           TAF_WARNING_LOG(WUEPS_PID_MMA, "TAF_MMA_ProcPowerOffReq_PhoneMode:stop TI_TAF_MMA_PERIOD_TRYING_RAT_BALANCING failed!");
       }

       TAF_MMA_SetTiTryingPeriodRatBalancingStatus(MMA_TIMER_STOP);
    }

    return;
}
VOS_VOID TAF_MMA_ProcStartReq_PhoneMode(VOS_VOID)
{
    TAF_CS_SERVICE_ENUM_UINT32          enCsServCapa;

    /* ����RESTART״̬��־�� */
    g_StatusContext.ulFsmState = STA_FSM_RESTART;

    if (MMA_MMA_INTER_POWER_INIT != TAF_MMA_GetCurrFsmEntryMsgId_PhoneMode())
    {
        /* ��RF��Դ */
        MMA_TurnOnForFlightMode();
    }

    TAF_MMA_UpdateUeBandNvim();

    /* ��¼�������ֵ */
    g_StatusContext.ucModeService = gstMmaValue.stSetMsClass.MsClassType;

#if (FEATURE_ON == FEATURE_IMS)
    if (VOS_TRUE == TAF_SDC_GetImsSupportFlag())
    {
        /* ����IMSA���Ϳ������� */
        TAF_MMA_SndStartReq_PhoneMode(PS_PID_IMSA);
    }
    else
#endif
    {
        /* ����MMC���Ϳ������� */
        TAF_MMA_SndStartReq_PhoneMode(WUEPS_PID_MMC);
    }

    if (VOS_TRUE == TAF_SDC_GetSvlteSupportFlag())
    {
        /* ��ȡCS������ */
        if (VOS_TRUE == TAF_MMA_IsCsServDomainAvail(gstMmaValue.stSetMsClass.MsClassType))
        {
            enCsServCapa = TAF_CS_ON;
        }
        else
        {
            enCsServCapa = TAF_CS_OFF;
        }

        TAF_MMA_SndStkCsServiceChangeNotify(enCsServCapa);
        TAF_MMA_SndMsgCsServiceChangeNotify(enCsServCapa);
    }

    MN_PH_UpdateBeginRegTime();

    return;
}


VOS_UINT32  TAF_MMA_GetCurrFsmEntryMsgId_PhoneMode(VOS_VOID)
{
    TAF_MMA_ENTRY_MSG_STRU             *pstEntryMsg    = VOS_NULL_PTR;
    VOS_UINT32                          ulMsgId;

   /* ����FSM ID��ȡ��FSM �����Ϣ */
    pstEntryMsg     = TAF_MMA_GetCurrFsmMsgAddr();

    ulMsgId         = TAF_MMA_ExtractMsgNameFromEvtType(pstEntryMsg->ulEventType);

    return ulMsgId;
}
VOS_UINT8  TAF_MMA_GetCurrFsmEntryMsgOpId_PhoneMode(VOS_VOID)
{
    TAF_MMA_ENTRY_MSG_STRU             *pstEntryMsg         = VOS_NULL_PTR;
    MN_APP_REQ_MSG_STRU                *pstAppMsg           = VOS_NULL_PTR;
    TAF_MMA_PHONE_MODE_SET_REQ_STRU    *pstPhoneModeSetReq  = VOS_NULL_PTR;
    VOS_UINT32                          ulEventType;

   /* ����FSM ID��ȡ��FSM �����Ϣ */
    pstEntryMsg         = TAF_MMA_GetCurrFsmMsgAddr();
    pstAppMsg           = (MN_APP_REQ_MSG_STRU *)(pstEntryMsg->aucEntryMsgBuffer);
    pstPhoneModeSetReq  = (TAF_MMA_PHONE_MODE_SET_REQ_STRU *)(pstEntryMsg->aucEntryMsgBuffer);
    ulEventType         = pstEntryMsg->ulEventType;

    if (TAF_BuildEventType(WUEPS_PID_MMA, MMA_MMA_INTER_POWER_INIT)== ulEventType)
    {
        return STA_OPID_INSIDE;
    }

    if ((TAF_BuildEventType(WUEPS_PID_MMA, MMA_MMA_INTER_USIM_STATUS_CHANGE_IND) == ulEventType)
     || (TAF_BuildEventType(WUEPS_PID_MMA, MMA_MMA_SIM_LOCK_STATUS_CHANGE_IND)   == ulEventType))
    {
        return MMA_OP_ID_INTERNAL;
    }

    if (TAF_BuildEventType(WUEPS_PID_TAF, ID_TAF_MMA_PHONE_MODE_SET_REQ) == ulEventType )
    {
        return pstPhoneModeSetReq->stCtrl.ucOpId;
    }

    return pstAppMsg->opId;

}
MMA_MMC_CARD_STATUS_ENUM_UINT8 TAF_MMA_ConvertSdcSimStatusToMmcFormat_PhoneMode(
    VOS_UINT8                           ucSdcSimStatus
)
{
    MMA_MMC_CARD_STATUS_ENUM_UINT8      enCardStatus;
    TAF_SDC_SIM_TYPE_ENUM_UINT8         enSimType;

    /* ��û���յ���״̬��Ϣʱ�򿨲���λ����PIN��ʱ,��Ϊ�޿����� */
    if ((TAF_SDC_USIM_STATUS_BUTT       == ucSdcSimStatus)
     || (TAF_SDC_USIM_STATUS_NO_PRESENT == ucSdcSimStatus)
     || (TAF_SDC_USIM_STATUS_SIM_PIN    == ucSdcSimStatus)
     || (TAF_SDC_USIM_STATUS_SIM_PUK    == ucSdcSimStatus)
     || (TAF_SDC_USIM_STATUS_UNVALID    == ucSdcSimStatus)
     || (TAF_SDC_USIM_STATUS_READY_TIMEROUT    == ucSdcSimStatus))
    {
        return MMA_MMC_CARD_STATUS_ABSENT;
    }

    if (TAF_PH_ME_SIM_LOCK == MMA_GetMeLockStatus())
    {
        return MMA_MMC_CARD_STATUS_ABSENT;
    }

    enSimType = TAF_SDC_GetSimType();

    if (TAF_SDC_SIM_TYPE_USIM == enSimType)
    {
        enCardStatus = MMA_MMC_CARD_STATUS_USIM_PRESENT;
    }
    else if (TAF_SDC_SIM_TYPE_ROM_SIM == enSimType)
    {
        enCardStatus = MMA_MMC_CARD_STATUS_USIM_PRESENT;
    }
    else
    {
        enCardStatus = MMA_MMC_CARD_STATUS_SIM_PRESENT;
    }

    return enCardStatus;
}


VOS_UINT8  TAF_MMA_GetCurrFsmEntryMsgPhoneMode_PhoneMode(VOS_VOID)
{
    TAF_MMA_ENTRY_MSG_STRU             *pstEntryMsg  = VOS_NULL_PTR;
    MN_APP_REQ_MSG_STRU                *pstAppMsg    = VOS_NULL_PTR;
    TAF_PH_OP_MODE_STRU                *pstPhModeSet    = VOS_NULL_PTR;


    TAF_MMA_PHONE_MODE_SET_REQ_STRU    *pstPhoneModeSet = VOS_NULL_PTR;

    VOS_UINT32                          ulEventType;

   /* ����FSM ID��ȡ��FSM �����Ϣ */
    pstEntryMsg     = TAF_MMA_GetCurrFsmMsgAddr();
    pstAppMsg       = (MN_APP_REQ_MSG_STRU *)(pstEntryMsg->aucEntryMsgBuffer);

    pstPhoneModeSet = (TAF_MMA_PHONE_MODE_SET_REQ_STRU *)(pstEntryMsg->aucEntryMsgBuffer);

    ulEventType = pstEntryMsg->ulEventType;

    if (TAF_BuildEventType(WUEPS_PID_OM, OAM_MMA_PHONE_MODE_SET_REQ) == ulEventType)
    {
        pstPhModeSet = (TAF_PH_OP_MODE_STRU*)pstAppMsg->aucContent;

        return pstPhModeSet->PhMode;
    }



    /* �ػ�������TAF���͵ģ�Ҳ����ʱCMMCA���͵� */
    if (TAF_BuildEventType(WUEPS_PID_TAF, ID_TAF_MMA_PHONE_MODE_SET_REQ) == ulEventType)
    {
        return pstPhoneModeSet->stPhoneModePara.PhMode;
    }
    return TAF_SDC_GetCurPhoneMode();
}




VOS_UINT16  TAF_MMA_GetCurrFsmEntryMsgClientId_PhoneMode(VOS_VOID)
{
    TAF_MMA_ENTRY_MSG_STRU             *pstEntryMsg         = VOS_NULL_PTR;
    MN_APP_REQ_MSG_STRU                *pstAppMsg           = VOS_NULL_PTR;
    TAF_MMA_PHONE_MODE_SET_REQ_STRU    *pstPhoneModeSetReq  = VOS_NULL_PTR;
    VOS_UINT32                          ulEventType;

    /* ����FSM ID��ȡ��FSM �����Ϣ */
    pstEntryMsg         = TAF_MMA_GetCurrFsmMsgAddr();
    pstAppMsg           = (MN_APP_REQ_MSG_STRU *)(pstEntryMsg->aucEntryMsgBuffer);
    pstPhoneModeSetReq  = (TAF_MMA_PHONE_MODE_SET_REQ_STRU *)(pstEntryMsg->aucEntryMsgBuffer);

    ulEventType = pstEntryMsg->ulEventType;

    if ((TAF_BuildEventType(WUEPS_PID_MMA, MMA_MMA_INTER_POWER_INIT)             == ulEventType)
     || (TAF_BuildEventType(WUEPS_PID_MMA, MMA_MMA_INTER_USIM_STATUS_CHANGE_IND) == ulEventType)
     || (TAF_BuildEventType(WUEPS_PID_MMA, MMA_MMA_SIM_LOCK_STATUS_CHANGE_IND)   == ulEventType))
    {
        return MMA_CLIENTID_NONE;
    }

    /* CMMCA����AT������ģʽ���� */
    if (TAF_BuildEventType(WUEPS_PID_TAF, ID_TAF_MMA_PHONE_MODE_SET_REQ) == ulEventType )
    {
        return pstPhoneModeSetReq->stCtrl.usClientId;
    }

    return pstAppMsg->clientId;

}


VOS_VOID TAF_MMA_ReportPhoneModeCnf_PhoneMode(VOS_VOID)
{
    TAF_MMA_ENTRY_MSG_STRU             *pstEntryMsg    = VOS_NULL_PTR;
    VOS_UINT32                          ulEventType;
    VOS_UINT8                           ucOpId;
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;
    VOS_UINT16                          usClientId;
    TAF_PH_ERR_CODE                     usErrorCode;

    /* ����FSM ID��ȡ��FSM �����Ϣ */
    pstEntryMsg     = TAF_MMA_GetCurrFsmMsgAddr();

    ulEventType = pstEntryMsg->ulEventType;
    ucOpId      = TAF_MMA_GetCurrFsmEntryMsgOpId_PhoneMode();
    usClientId  = TAF_MMA_GetCurrFsmEntryMsgClientId_PhoneMode();
    usErrorCode = TAF_MMA_GetCurPhoneErrorCode_PhoneMode();

    /* ֻ��ҪOM��AT��CMMCA�����Ŀ�����ػ�����ģʽ���ý���ϱ� */
    if ( (TAF_BuildEventType(WUEPS_PID_OM, OAM_MMA_PHONE_MODE_SET_REQ)      == ulEventType)
      || (TAF_BuildEventType(WUEPS_PID_TAF, ID_TAF_MMA_PHONE_MODE_SET_REQ)  == ulEventType))
    {
        stPhMode.CmdType = TAF_PH_CMD_SET;
        stPhMode.PhMode = TAF_SDC_GetCurPhoneMode();

        MMA_PhModeReport(usClientId, ucOpId, stPhMode, usErrorCode);

        return;
    }


    return;
}



VOS_VOID TAF_MMA_SndAtPsInitRsltInd_PhoneMode(
    VOS_UINT32                          ulRslt
)
{
    TAF_PHONE_EVENT_INFO_STRU          *pstPhoneEvent = VOS_NULL_PTR;

    pstPhoneEvent = (TAF_PHONE_EVENT_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMA, sizeof(TAF_PHONE_EVENT_INFO_STRU));

    if (VOS_NULL_PTR == pstPhoneEvent)
    {
        TAF_ERROR_LOG(WUEPS_PID_MMA, "TAF_MMA_SndAtPsInitRsltInd:ERROR:ALLOC MEMORY FAIL.");
        return;
    }

    PS_MEM_SET(pstPhoneEvent, 0, sizeof(TAF_PHONE_EVENT_INFO_STRU));

    /* ��ʼ���绰�ϱ��¼� */
    MMA_InitEventInfoOP(pstPhoneEvent);

    pstPhoneEvent->OP_PsInitRslt          = VOS_TRUE;
    pstPhoneEvent->PhoneEvent             = TAF_MMA_EVT_PS_INIT_RESULT_IND;

    /* ��д�¼��ͻ�������Ϊ�㲥�¼� */
    pstPhoneEvent->ClientId               = MMA_CLIENTID_BROADCAST;
    pstPhoneEvent->OpId                   = MMA_OP_ID_INTERNAL;

    if (TAF_FAILURE == ulRslt)
    {
        pstPhoneEvent->ulPsInitRslt           = VOS_FALSE;
    }
    else
    {
        pstPhoneEvent->ulPsInitRslt           = VOS_TRUE;
    }

    /* ���õ绰�����ϱ����� */
    MN_PH_SendMsg(pstPhoneEvent->ClientId,(VOS_UINT8*)pstPhoneEvent,sizeof(TAF_PHONE_EVENT_INFO_STRU));

    PS_MEM_FREE(WUEPS_PID_MMA, pstPhoneEvent);
    return;
}
VOS_VOID TAF_MMA_ReportPhoneModeInd_PhoneMode(VOS_VOID)
{
    TAF_PHONE_EVENT_INFO_STRU          *pstPhoneEvent           = VOS_NULL_PTR;

    /* �㲥�ϱ���ǰ��ϵͳģʽ */
    pstPhoneEvent = (TAF_PHONE_EVENT_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMA, sizeof(TAF_PHONE_EVENT_INFO_STRU));

    if (VOS_NULL_PTR == pstPhoneEvent)
    {
        TAF_ERROR_LOG(WUEPS_PID_MMA, "TAF_MMA_ReportPhoneModeInd_PhoneMode:ERROR:ALLOC MEMORY FAIL.");
        return;
    }

    PS_MEM_SET(pstPhoneEvent, 0, sizeof(TAF_PHONE_EVENT_INFO_STRU));
    pstPhoneEvent->OpId = MMA_OP_ID_INTERNAL;
    pstPhoneEvent->ClientId = MMA_CLIENTID_BROADCAST;
    pstPhoneEvent->PhoneEvent = TAF_PH_EVT_OPER_MODE_IND;
    pstPhoneEvent->PhoneError = TAF_ERR_NO_ERROR;
    pstPhoneEvent->OP_OperMode = VOS_TRUE;
    pstPhoneEvent->OperMode.CmdType = TAF_PH_CMD_SET;
    pstPhoneEvent->OperMode.PhMode = TAF_SDC_GetCurPhoneMode();

    /* ���õ绰�����ϱ����� */
    MN_PH_SendMsg(pstPhoneEvent->ClientId,(VOS_UINT8*)pstPhoneEvent,sizeof(TAF_PHONE_EVENT_INFO_STRU));
    PS_MEM_FREE(WUEPS_PID_MMA, pstPhoneEvent);

    return;
}




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif



