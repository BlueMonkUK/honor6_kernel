

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "pslog.h"
#include "om.h"
#include "PsTypeDef.h"
#include "PsCommonDef.h"
#include "NasComm.h"
#include "NasCommDef.h"
#include "TafMmaCtx.h"
#include "TafSdcCtx.h"
#include "MmaAppLocal.h"
#include "Taf_Status.h"
#include "TafMmaSndImsa.h"
#include "NasUsimmApi.h"
#include "msp_nvim.h"
#include "TafApsProcNvim.h"
#include "TafMmaFsmPhoneModeTbl.h"
#include "TafMmaPreProcAct.h"
#include "TafMmaSndImsa.h"
#include "TafLog.h"
#include "TafSdcLib.h"
#include "TafMmaSndInternalMsg.h"
#include "TafMmaSndTaf.h"
#if (FEATURE_ON == FEATURE_IMS)
#include "ImsaMmaInterface.h"
#endif

#include "TafMmaSndApp.h"
#include "TafMmaProcNvim.h"
#include "TafMmaMntn.h"
#include "TafMmaFsmPhoneMode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_TAF_MMA_PREPROC_ACT_C


/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
extern MMA_ME_PERSONALISATION_STATUS_STRU       g_stMmaMePersonalisationStatus;
extern VOS_UINT8                                g_ucMmaOmConnectFlg ;
extern MMA_TIMER_ST                             g_stPhPhResetTimer;
extern STATUS_CONTEXT_STRU                      g_StatusContext;
extern VOS_UINT32                               g_ucUsimHotOutFlag;
extern NAS_MMA_NEW_ADD_GLABAL_VARIABLE_FOR_PC_REPLAY_ST g_MmaNewValue;
extern VOS_UINT8                               g_aucMmaImei[TAF_PH_IMEI_LEN-1];
extern TAF_MMA_GLOBAL_VALUE_ST                 gstMmaValue;
extern MMA_TI_TABLE                            gastMmaTiTab[MMA_MAX_TI];
extern MMA_INTERNAL_TIMER_ST                   gstMmaInternalTimer;
extern MMA_TIMER_ST                            g_MmaSimTimer[TAF_SIM_TIMER_NUM];
extern MMA_UE_BAND_CAPA_ST                     gstMmaBandCapa;
extern MMA_TIMER_ST                                     g_stNetScanProtectTimer;
extern MMA_TIMER_ST                                     g_stAbortNetScanProtectTimer;

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
#ifdef __PS_WIN32_RECUR__

VOS_UINT32 NAS_MMA_RestoreContextData(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMA_SDT_MSG_ST                      *pRcvMsgCB;
    NAS_MMA_OUTSIDE_RUNNING_CONTEXT_ST      *pstOutsideCtx;

    pRcvMsgCB     = (NAS_MMA_SDT_MSG_ST *)pstMsg;

    if (EVT_NAS_MMA_OUTSIDE_RUNNING_CONTEXT_FOR_PC_REPLAY == pRcvMsgCB->usMsgID)
    {
        pstOutsideCtx = &pRcvMsgCB->stOutsideCtx;


        g_MmaNewValue.ucSimType = pstOutsideCtx->ucSimType;

        TAF_SDC_SetSimImsi(pstOutsideCtx->pc_g_aucMmaImsi);

        PS_MEM_CPY(g_aucMmaImei, pstOutsideCtx->pc_g_aucMmaImei, TAF_PH_IMEI_LEN-1);


        PS_MEM_CPY(&g_StatusContext, &pstOutsideCtx->pc_g_StatusContext, sizeof(STATUS_CONTEXT_STRU));
        PS_MEM_CPY(&gstMmaValue, &pstOutsideCtx->pc_gstMmaValue, sizeof(TAF_MMA_GLOBAL_VALUE_ST));
        gstMmaValue.pg_StatusContext = &g_StatusContext;

        PS_MEM_CPY(&g_stMmaMePersonalisationStatus, &pstOutsideCtx->pc_g_stMmaMePersonalisationStatus, sizeof(MMA_ME_PERSONALISATION_STATUS_STRU));
        PS_MEM_CPY(gastMmaTiTab, pstOutsideCtx->pc_gastMmaTiTab, MMA_MAX_TI*sizeof(MMA_TI_TABLE));

        PS_MEM_CPY(&gstMmaInternalTimer, &pstOutsideCtx->pc_gstMmaInternalTimer, sizeof(MMA_INTERNAL_TIMER_ST));

        PS_MEM_CPY(g_MmaSimTimer, pstOutsideCtx->pc_g_MmaSimTimer, TAF_SIM_TIMER_NUM*sizeof(MMA_TIMER_ST));

        MMA_INFOLOG("MMA: NAS_MMA_RestoreContextData - data is restored.");
    }
    return VOS_TRUE;
}


VOS_UINT32 NAS_MMA_RestoreFixedContextData(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMA_FIXED_CONTEXT_MSG_ST       *pRcvMsgCB;
    NAS_MMA_FIXED_PART_CONTEXT_ST      *pstOutsideCtx;
    VOS_UINT32                          ulVersion;
    NAS_NVIM_ROAM_CFG_INFO_STRU         stRoamFeature;
    MN_MMA_LAST_SETTED_SYSCFG_SET_STRU *pstLastSyscfgSet = VOS_NULL_PTR;

    NAS_NVIM_FOLLOWON_OPENSPEED_FLAG_STRU               stQuickStartFlg;

    stQuickStartFlg.ulQuickStartSta = MMA_QUICK_START_DISABLE;

    pstLastSyscfgSet = MN_MMA_GetLastSyscfgSetAddr();

    pRcvMsgCB        = (NAS_MMA_FIXED_CONTEXT_MSG_ST *)pstMsg;

    if (EVT_NAS_MMA_FIXED_PART_CONTEXT == pRcvMsgCB->usMsgID)
    {
        pstOutsideCtx = &pRcvMsgCB->stFixedOutsideCtx;

        ulVersion = pstOutsideCtx->ulVersion;
        switch (ulVersion)
        {
            case 1:      /* restore new para here, no break. */
                gstMmaBandCapa = pstOutsideCtx->gstMmaBandCapa;
            case 0:
                /* ȫ�ֱ����ָ� */

                TAF_SDC_SetSimImsi(pstOutsideCtx->aucMmaImsi);

                g_MmaNewValue.ucSimType = pstOutsideCtx->aucSimType;

                pstLastSyscfgSet->stUserSetBand.uUserSetGuBand.ulPrefBand = pstOutsideCtx->ulPrefBand;
                pstLastSyscfgSet->stUserSetBand.uUserSetUeFormatGuBand = pstOutsideCtx->Band;
                gstMmaValue.ulQuickStartFlg = pstOutsideCtx->ulQuickStartFlg;

                gstMmaValue.stEfustServiceCfg = pstOutsideCtx->stEfustServiceCfg;

                PS_MEM_CPY(g_MmaNewValue.aucImeiBcd, pstOutsideCtx->aucImeiBcd, TAF_PH_IMEI_LEN+1);

                gstMmaValue.stSetMsClass.MsClassType = pstOutsideCtx->MsClassType;
                pstLastSyscfgSet->ucRoam = pstOutsideCtx->ucRoam;
                gstMmaValue.ucRoamFeatureStatus = pstOutsideCtx->ucRoamFeatureStatus;

                gstMmaValue.stMeInfo = pstOutsideCtx->stMeInfo;

                g_MmaNewValue.stSimLockPlmnInfo = pstOutsideCtx->stSimLockPlmnInfo;
                g_MmaNewValue.stCardLockStatus = pstOutsideCtx->stCardLockStatus;
                g_MmaNewValue.stSimLockMaxTimes = pstOutsideCtx->stSimLockMaxTimes;

                PS_MEM_CPY(g_MmaNewValue.aucPwd, pstOutsideCtx->aucPwd, TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX+1);
                g_MmaNewValue.usSimMePersonalisationActFlag = pstOutsideCtx->usSimMePersonalisationActFlag;
                PS_MEM_CPY(g_MmaNewValue.aucSimPersonalisationStr, pstOutsideCtx->aucSimPersonalisationStr, TAF_MAX_IMSI_LEN+1);


                /* NV��Ļָ� */
                MMA_WriteValue2Nvim(en_NV_Item_User_Set_Freqbands, &pstOutsideCtx->ulPrefBand, sizeof(VOS_UINT32));
                MMA_WriteValue2Nvim(en_NV_Item_Support_Freqbands, &pstOutsideCtx->Band, sizeof(MMA_UE_SUPPORT_FREQ_BAND_STRU));

                stQuickStartFlg.ulQuickStartSta = pstOutsideCtx->ulQuickStartFlg;
                MMA_WriteValue2Nvim(en_NV_Item_FollowOn_OpenSpeed_Flag, &(stQuickStartFlg.ulQuickStartSta), sizeof(NAS_NVIM_FOLLOWON_OPENSPEED_FLAG_STRU));

                MMA_WriteValue2Nvim(en_NV_Item_RF_Auto_Test_Flag, &pstOutsideCtx->usRfAutoTestFlg, sizeof(VOS_UINT16));
                MMA_WriteValue2Nvim(en_NV_Item_EFust_Service_Cfg, &pstOutsideCtx->stEfustServiceCfg, sizeof(MMA_EFUST_SERVICE_CFG_STRU));

                MMA_WriteValue2Nvim(en_NV_Item_IMEI, pstOutsideCtx->aucImeiBcd, TAF_PH_IMEI_LEN);


                MMA_WriteValue2Nvim(en_NV_Item_MMA_MsClass, &pstOutsideCtx->MsClassType, sizeof(TAF_PH_MS_CLASS_TYPE));
                stRoamFeature.ucRoamFeatureFlg = pstOutsideCtx->ucRoamFeatureStatus;
                stRoamFeature.ucRoamCapability = pstOutsideCtx->ucRoam;
                MMA_WriteValue2Nvim(en_NV_Item_Roam_Capa, &stRoamFeature, 2);

                MMA_WriteValue2Nvim(en_NV_Item_FMRID, &pstOutsideCtx->stMeInfo.FmrId, TAF_MAX_MFR_ID_LEN);
                MMA_WriteValue2Nvim(en_NV_Item_ProductName, &pstOutsideCtx->stMeInfo.ProductName, TAF_PH_PRODUCT_NAME_LEN);
                /* MMA_WriteValue2Nvim(en_NV_Item_ModelId, &pstOutsideCtx->stMeInfo.ModelId, TAF_MAX_MODEL_ID_LEN); */

                MMA_WriteValue2Nvim(en_NV_Item_CustomizeSimLockPlmnInfo, &pstOutsideCtx->stSimLockPlmnInfo, sizeof(TAF_CUSTOM_SIM_LOCK_PLMN_INFO_STRU));
                MMA_WriteValue2Nvim(en_NV_Item_CardlockStatus, &pstOutsideCtx->stCardLockStatus, sizeof(TAF_CUSTOM_CARDLOCK_STATUS_STRU));
                MMA_WriteValue2Nvim(en_NV_Item_CustomizeSimLockMaxTimes, &pstOutsideCtx->stSimLockMaxTimes, sizeof(TAF_CUSTOM_SIM_LOCK_MAX_TIMES_STRU));

                MMA_WriteValue2Nvim(en_NV_Item_Sim_Personalisation_Pwd, pstOutsideCtx->aucPwd, TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX);
                MMA_WriteValue2Nvim(en_NV_Item_Me_Personalisation_Active_Flag, &pstOutsideCtx->usSimMePersonalisationActFlag, 2);
                MMA_WriteValue2Nvim(en_NV_Item_Sim_Personalisation_Imsi_Str, pstOutsideCtx->aucSimPersonalisationStr, pstOutsideCtx->aucSimPersonalisationStr[0]+1);

                break;
            default:
                break;
        }

        MMA_INFOLOG("MMA: NAS_MMA_RestoreFixedContextData - data is restored.");
    }
    return VOS_TRUE;
}
#endif


VOS_UINT32 TAF_MMA_SetMiniMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
)
{
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;

    stPhMode.CmdType    = TAF_PH_CMD_SET;
    stPhMode.PhMode     = ucSetMode;

    /* ��ǰ��ΪFULLģʽ,������ֱ�ӻظ��ɹ� */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
#if (FEATURE_ON == FEATURE_LTE)
        NVM_Flush();
#endif
        NV_Flush();

        TAF_SDC_SetCurPhoneMode(ucSetMode);

        /* �ϱ�TAF_PH_EVT_OPER_MODE_CNF�¼� */
        MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);

        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 TAF_MMA_SetRfOffMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
)
{
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;

    stPhMode.CmdType    = TAF_PH_CMD_SET;
    stPhMode.PhMode     = ucSetMode;

    /* ��ǰģʽ��ΪFULLģʽ��ֱ���ϱ��ɹ� */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
        TAF_SDC_SetCurPhoneMode(ucSetMode);

        /* �ϱ�TAF_PH_EVT_OPER_MODE_CNF�¼� */
        MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);

        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_UINT32 TAF_MMA_SetFtMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
)
{
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;

    stPhMode.CmdType    = TAF_PH_CMD_SET;
    stPhMode.PhMode     = ucSetMode;

    /* ��ǰģʽ��ΪFULLģʽ��ֱ���ϱ��ɹ� */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
        TAF_SDC_SetCurPhoneMode(ucSetMode);

        /* �ϱ�TAF_PH_EVT_OPER_MODE_CNF�¼� */
        MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);

        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_UINT32 TAF_MMA_SetVdfMiniMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
)
{
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;

    stPhMode.CmdType    = TAF_PH_CMD_SET;
    stPhMode.PhMode     = ucSetMode;

    /* ��ǰģʽ��ΪFULLģʽ��ֱ���ϱ��ɹ� */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
        TAF_SDC_SetCurPhoneMode(ucSetMode);

        /* �ϱ�TAF_PH_EVT_OPER_MODE_CNF�¼� */
        MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);

        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_UINT32 TAF_MMA_SetLowPowerMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
)
{
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;

    stPhMode.CmdType    = TAF_PH_CMD_SET;
    stPhMode.PhMode     = ucSetMode;

    /* ��ǰģʽ��ΪFULLģʽ��ֱ���ϱ��ɹ� */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
        TAF_SDC_SetCurPhoneMode(ucSetMode);

        /* �ϱ�TAF_PH_EVT_OPER_MODE_CNF�¼� */
        MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);

        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_UINT32 TAF_MMA_SetResetMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
)
{
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;
    VOS_UINT32                          ulRet;

    stPhMode.CmdType    = TAF_PH_CMD_SET;
    stPhMode.PhMode     = ucSetMode;

    /* �ϱ�TAF_PH_EVT_OPER_MODE_CNF�¼� */
    MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);

    /* �����ӳٶ�ʱ����AT�ָ�OK����������*/
    g_stPhPhResetTimer.ulTimerId = TI_MN_PH_RESET;

    ulRet = NAS_StartRelTimer( &g_stPhPhResetTimer.MmaHTimer,
                               WUEPS_PID_MMA,
                               TI_MN_PH_RESET_LENGTH,
                               TAF_MMA,
                               TI_MN_PH_RESET,
                               VOS_RELTIMER_NOLOOP );

    if (VOS_OK !=  ulRet)
    {
        TAF_ERROR_LOG(WUEPS_PID_MMA, "TAF_MMA_SetRestMode:start TI_MN_PH_RESET failed!");
    }

    return VOS_TRUE;
}



VOS_UINT32 TAF_MMA_SetFullMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
)
{
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;

    /* ��ǰ�Ѿ���������ģʽ��ֱ���ϱ��ɹ� */
    if (ucSetMode == TAF_SDC_GetCurPhoneMode())
    {
        stPhMode.CmdType    = TAF_PH_CMD_SET;
        stPhMode.PhMode     = ucSetMode;

        /* �ϱ�TAF_PH_EVT_OPER_MODE_CNF�¼� */
        MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);

        return VOS_TRUE;
    }

#if (VOS_WIN32 == VOS_OS_VER)

    if (VOS_TRUE == g_ucMmaOmConnectFlg)
    {
        NAS_MMA_SndNVData();
    }
#endif

    return VOS_FALSE;
}


VOS_UINT32 TAF_MMA_SetPowerOffMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
)
{
    TAF_PH_OP_MODE_CNF_STRU                                 stPhMode;
    VOS_UINT16                                              usAppCfgSupportType;
#if (FEATURE_ON == FEATURE_POWER_ON_OFF)
    VOS_UINT32                          ulRslt;
#endif

    usAppCfgSupportType     = TAF_SDC_GetAppCfgSupportType();
    stPhMode.CmdType        = TAF_PH_CMD_SET;
    stPhMode.PhMode         = ucSetMode;

    TAF_APS_SaveDsFlowInfoToNv();

    if (SYSTEM_APP_WEBUI == usAppCfgSupportType)
    {
#if (FEATURE_ON == FEATURE_POWER_ON_OFF)
        if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
        {
            /* �ϱ�TAF_PH_EVT_OPER_MODE_CNF�¼� */
            MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);





            /* ��TASKDELAY�޸�Ϊ������ʱ�����б��� */
            ulRslt = NAS_StartRelTimer(&g_stPowerDownDelayTimer.MmaHTimer,
                                  WUEPS_PID_MMA,
                                  TI_TAF_MMA_DELAY_POWER_DOWN_TIMER_LEN,
                                  TAF_MMA,
                                  TI_TAF_MMA_DELAY_POWER_DOWN,
                                  VOS_RELTIMER_NOLOOP );

            if( VOS_OK !=  ulRslt)
            {
                MMA_WARNINGLOG("MN_PH_Switch2OffFunc():ERROR:Start TI_TAF_MMA_DELAY_POWER_DOWN Timer Error!");
                DRV_POWER_OFF();
            }



            return VOS_TRUE;
        }

        return VOS_FALSE;
#endif
    }

    if (SYSTEM_APP_ANDROID == usAppCfgSupportType)
    {
        if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
        {
            /* ���ϱ��¼�ǰ�ȵ���COMM�ӿ�Flush NV */
#if (FEATURE_ON == FEATURE_LTE)
            NVM_Flush();
#endif
            NV_Flush();

            /* ����ģʽ��ʹ��AT+CFUN=8Ҳ��Ҫȥ����USIM�� */
            NAS_USIMMAPI_DeactiveCardReq(WUEPS_PID_MMA);

            MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);

            return VOS_TRUE;
        }

        return VOS_FALSE;
    }

    /* Ŀǰ����WEBUI��ANDROID��������̨�����·�AT+CFUN=8 */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
        MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);

        return VOS_TRUE;
    }

    return VOS_FALSE;

}

VOS_UINT32 TAF_MMA_IsPhoneModeSetAllowed_PreProc(
    VOS_UINT16                          usClientId,
    TAF_PH_MODE                         ucPhMode
)
{
    TAF_SDC_RAT_PRIO_STRU              *pstRatPrio      = VOS_NULL_PTR;
    VOS_UINT8                           ucCurPhmode;

    /* ģʽ�����Ƿ�����ı� VOS_TRUE :��������, VOS_FALSE:�������� */
    VOS_UINT32                          aulOperModeSetTable[TAF_PH_MODE_NUM_MAX][TAF_PH_MODE_NUM_MAX] =
    {
        /* operate mode: MINI--0,FULL--1,TXOFF--2,RXOFF--3,RFOFF--4,FT--5,RESET--6,LOW--7
                         POWEROFF--8,LOWPOWER--9 */
        /*->0*/     /*->1*/     /*->2*/     /*->3*/     /*->4*/     /*->5*/     /*->6*/     /*->7*/     /* ->8 */   /* ->9 */
/*0->*/ {VOS_TRUE,  VOS_TRUE,   VOS_FALSE,  VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_TRUE},
/*1->*/ {VOS_TRUE,  VOS_TRUE,   VOS_FALSE,  VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_TRUE},
/*2->*/ {VOS_FALSE, VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE},
/*3->*/ {VOS_FALSE, VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE},
/*4->*/ {VOS_FALSE, VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_TRUE,   VOS_FALSE,  VOS_TRUE,   VOS_FALSE,  VOS_TRUE,   VOS_FALSE},
/*5->*/ {VOS_TRUE,  VOS_TRUE,   VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_TRUE,   VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_TRUE},
/*6->*/ {VOS_FALSE, VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE},
/*7->*/ {VOS_TRUE,  VOS_TRUE,   VOS_FALSE,  VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_TRUE},
/*8->*/ {VOS_TRUE,  VOS_TRUE,   VOS_FALSE,  VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_TRUE},
/*9->*/ {VOS_TRUE,  VOS_TRUE,   VOS_FALSE,  VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_TRUE}
    };

    pstRatPrio                          = TAF_SDC_GetMsPrioRatList();
    ucCurPhmode                         = TAF_SDC_GetCurPhoneMode();

    /* ���в�׼�����ã���ֱ�ӻظ�ʧ�� */
    if (VOS_FALSE                       == aulOperModeSetTable[ucCurPhmode][ucPhMode] )
    {
        return VOS_FALSE;
    }

    /* �쳣����:���CLģʽ���ҵ�ǰ��AT����ģʽ�����ϱ�����ʧ�� */
    if ((VOS_TRUE                       == TAF_SDC_IsConfigCLInterWork())
     && (usClientId                     < AT_CLIENT_ID_BUTT))
    {
        return VOS_FALSE;
    }

    /* �쳣����:�������CLģʽ���ҵ�ǰ��CMMCA����ģʽ�����ϱ�CMMCAʧ�� */
    if ((VOS_FALSE                      == TAF_SDC_IsConfigCLInterWork())
     && (CMMCA_CLIENT_ID                == usClientId))
    {
        return VOS_FALSE;
    }

    /* ��CMMCA���п���ʱ�������ǰ����L ONLY MODE,��ֱ�ӻظ�CMMCAʧ�� */
    if ( (VOS_TRUE                      != TAF_SDC_IsLteOnlyMode(pstRatPrio))
      && (CMMCA_CLIENT_ID               == usClientId) )
    {
        return VOS_FALSE;
    }

    /* ���Խ���ģʽ���� */
    return VOS_TRUE;

}


VOS_UINT32 TAF_MMA_ProcOmPhoneModeSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT16                          usClientId;
    VOS_UINT8                           ucOpId;
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;
    VOS_UINT32                          ulRet;
    VOS_UINT8                           ucOldMode;
    MN_APP_REQ_MSG_STRU                *pstRcvMsg       = VOS_NULL_PTR;
    TAF_PH_OP_MODE_STRU                *pstPhModeSet    = VOS_NULL_PTR;

    pstRcvMsg       = (MN_APP_REQ_MSG_STRU *)pstMsg;
    pstPhModeSet    = (TAF_PH_OP_MODE_STRU*)pstRcvMsg->aucContent;

    usClientId      = pstRcvMsg->clientId;
    ucOpId          = pstRcvMsg->opId;
    ulRet           = VOS_TRUE;
    ucOldMode       = TAF_SDC_GetCurPhoneMode();

    /* ģʽ����״̬����������ֱ�ӽ�������� */
    if (TAF_MMA_FSM_PHONE_MODE == TAF_MMA_GetCurrFsmId())
    {
        return VOS_FALSE;
    }


    /* �жϵ�ǰ�Ƿ��������ģʽ���� */
    if (VOS_FALSE == TAF_MMA_IsPhoneModeSetAllowed_PreProc(usClientId, pstPhModeSet->PhMode))
    {
        stPhMode.CmdType    = TAF_PH_CMD_SET;
        stPhMode.PhMode     = TAF_SDC_GetCurPhoneMode();

        MMA_PhModeReport(usClientId,  ucOpId, stPhMode, TAF_ERR_ERROR);

        return VOS_TRUE;
    }


    /* ���ݲ�ͬ��ģʽ�������ô��� */
    ulRet =TAF_MMA_ProcPhoneModeSet_PreProc(pstPhModeSet->PhMode, usClientId, ucOpId);

    if (ucOldMode != pstPhModeSet->PhMode)
    {
        /* �뿪FTģʽ��Ҫ����˯�� */
        if (TAF_PH_MODE_FT == ucOldMode)
        {
            TAF_DRVAPI_PWRCTRL_SLEEPVOTE_UNLOCK(PWRCTRL_SLEEP_FTM);
            if (VOS_TRUE == TAF_MMA_GetMmaLogInfoFlag())
            {
                vos_printf("\n*TAF_MMA_ProcOmPhoneModeSetReq_PreProc:PWRCTRL_SLEEP_FTM,TAF_DRVAPI_PWRCTRL_SLEEPVOTE_UNLOCK\n");
            }
        }

        /* ����FTģʽ��Ҫ��ֹ˯�� */
        if (TAF_PH_MODE_FT == pstPhModeSet->PhMode)
        {
            TAF_DRVAPI_PWRCTRL_SLEEPVOTE_LOCK(PWRCTRL_SLEEP_FTM);
            if (VOS_TRUE == TAF_MMA_GetMmaLogInfoFlag())
            {
                vos_printf("\n*TAF_MMA_ProcOmPhoneModeSetReq_PreProc:PWRCTRL_SLEEP_FTM,TAF_DRVAPI_PWRCTRL_SLEEPVOTE_LOCK\n");
            }
        }
    }

    return ulRet;
}
VOS_UINT32 TAF_MMA_ProcPhoneModeSet_PreProc(
    TAF_PH_MODE                         ucCurrPhMode,
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId
)
{
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;
    VOS_UINT32                          ulRet;

    ulRet = VOS_TRUE;

    /* ���ݲ�ͬ��ģʽ�������ô��� */
    switch (ucCurrPhMode)
    {
        case TAF_PH_MODE_MINI:

            ulRet = TAF_MMA_SetMiniMode_PreProc(usClientId, ucOpId, ucCurrPhMode);
            break;

        case TAF_PH_MODE_RFOFF:

            ulRet = TAF_MMA_SetRfOffMode_PreProc(usClientId, ucOpId, ucCurrPhMode);
            break;

        case TAF_PH_MODE_FT:

            ulRet = TAF_MMA_SetFtMode_PreProc(usClientId, ucOpId, ucCurrPhMode);
            break;

        case TAF_PH_MODE_VDFMINI:

            ulRet = TAF_MMA_SetVdfMiniMode_PreProc(usClientId, ucOpId, ucCurrPhMode);
            break;

        case TAF_PH_MODE_LOWPOWER:

            ulRet = TAF_MMA_SetLowPowerMode_PreProc(usClientId, ucOpId, ucCurrPhMode);
            break;

        case TAF_PH_MODE_FULL:

            ulRet = TAF_MMA_SetFullMode_PreProc(usClientId, ucOpId, ucCurrPhMode);
            break;

        case TAF_PH_MODE_RESET:

            ulRet = TAF_MMA_SetResetMode_PreProc(usClientId, ucOpId, ucCurrPhMode);
            break;

        case TAF_PH_MODE_POWEROFF:

            ulRet = TAF_MMA_SetPowerOffMode_PreProc(usClientId, ucOpId, ucCurrPhMode);
            break;

        default:

            /* warning ��ӡ */
            TAF_WARNING_LOG(WUEPS_PID_MMA, "TAF_MMA_ProcPhoneModeSet_PreProc:invalid mode!");

            stPhMode.CmdType = TAF_PH_CMD_SET;
            stPhMode.PhMode = TAF_SDC_GetCurPhoneMode();
            MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_ERROR);

            break;
    }

    return ulRet;
}



VOS_UINT32 TAF_MMA_ProcTafPhoneModeSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT16                          usClientId;
    VOS_UINT8                           ucOpId;
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;
    VOS_UINT32                          ulRet;
    VOS_UINT8                           ucOldMode;
    TAF_PH_MODE                         ucCurrPhMode;
    TAF_MMA_PHONE_MODE_SET_REQ_STRU    *pstRcvMsg       = VOS_NULL_PTR;

    pstRcvMsg       = (TAF_MMA_PHONE_MODE_SET_REQ_STRU *)pstMsg;

    usClientId      = pstRcvMsg->stCtrl.usClientId;
    ucOpId          = pstRcvMsg->stCtrl.ucOpId;
    ulRet           = VOS_TRUE;
    ucOldMode       = TAF_SDC_GetCurPhoneMode();
    ucCurrPhMode    = pstRcvMsg->stPhoneModePara.PhMode;

    /* ������ȡNV */
    if (TAF_PH_MODE_FULL == pstRcvMsg->stPhoneModePara.PhMode)
    {
        TAF_MMA_ReadLcCtrlNvim();

        /* ɾ����ӡ����ֹ�ڿ���ʱ���ͷ�CPU����˯�ߣ����������쳣 */
    }

    /* ģʽ����״̬����������ֱ�ӽ�������� */
    if (TAF_MMA_FSM_PHONE_MODE == TAF_MMA_GetCurrFsmId())
    {
        return VOS_FALSE;
    }

    /* �жϵ�ǰ�Ƿ��������ģʽ���� */
    if (VOS_FALSE == TAF_MMA_IsPhoneModeSetAllowed_PreProc(usClientId, ucCurrPhMode))
    {
        stPhMode.CmdType = TAF_PH_CMD_SET;
        stPhMode.PhMode = TAF_SDC_GetCurPhoneMode();
        MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_ERROR);

        return VOS_TRUE;
    }

    /* ��CMMCA���п���ʱ�������ǰ���ֶ�ģʽ�����޸�Ϊ�Զ�����ģʽ */
    if ( (MMA_MMC_PLMN_SEL_MODE_AUTO    != TAF_MMA_GetPlmnSelectionMode())
      && (CMMCA_CLIENT_ID               == usClientId) )
    {
        /* CLģʽʱ������ֶ�ģʽ��������Ϊ�Զ�����ģʽ�������쳣��������ֹAPPû�����õ�ǰΪ�Զ�����ģʽ */
        TAF_MMA_WritePlmnSelectionModeNvim(MMA_MMC_PLMN_SEL_MODE_AUTO);
        TAF_MMA_SetPlmnSelectionMode(TAF_PH_PLMN_SEL_MODE_AUTO);
    }

    /* ���ݲ�ͬ��ģʽ�������ô��� */
    ulRet = TAF_MMA_ProcPhoneModeSet_PreProc(ucCurrPhMode, usClientId, ucOpId);

    if (ucOldMode != ucCurrPhMode)
    {
        /* �뿪FTģʽ��Ҫ����˯�� */
        if (TAF_PH_MODE_FT == ucOldMode)
        {

            if (VOS_TRUE == TAF_MMA_GetMmaLogInfoFlag())
            {
                vos_printf("\n*TAF_MMA_ProcTafPhoneModeSetReq_PreProc:PWRCTRL_SLEEP_FTM,TAF_DRVAPI_PWRCTRL_SLEEPVOTE_UNLOCK\n");
            }

            TAF_DRVAPI_PWRCTRL_SLEEPVOTE_UNLOCK(PWRCTRL_SLEEP_FTM);
        }

        /* ����FTģʽ��Ҫ��ֹ˯�� */
        if (TAF_PH_MODE_FT == ucCurrPhMode)
        {
            TAF_DRVAPI_PWRCTRL_SLEEPVOTE_LOCK(PWRCTRL_SLEEP_FTM);
            if (VOS_TRUE == TAF_MMA_GetMmaLogInfoFlag())
            {
                vos_printf("\n*TAF_MMA_ProcTafPhoneModeSetReq_PreProc:PWRCTRL_SLEEP_FTM,TAF_DRVAPI_PWRCTRL_SLEEPVOTE_LOCK\n");
            }
        }
    }

    return ulRet;
}
VOS_UINT32 TAF_MMA_RcvOmPhoneModeSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MN_APP_REQ_MSG_STRU                *pstRcvMsg       = VOS_NULL_PTR;
    TAF_PH_OP_MODE_STRU                *pstPhModeSet    = VOS_NULL_PTR;
    TAF_PH_OP_MODE_CNF_STRU             stPhModeCnf;
    VOS_UINT32                          ulRet;

    pstRcvMsg       = (MN_APP_REQ_MSG_STRU *)pstMsg;
    pstPhModeSet    = (TAF_PH_OP_MODE_STRU *)pstRcvMsg->aucContent;

    /* �����������ͷַ����� */
    switch (pstPhModeSet->CmdType)
    {
        case TAF_PH_CMD_SET:            /* ������ַ����� */

            ulRet = TAF_MMA_ProcOmPhoneModeSetReq_PreProc(ulEventType, pstMsg);

            break;

        case TAF_PH_CMD_QUERY:          /* ��ѯ��ַ����� */

            /* �ϱ��¼�����������д */
            stPhModeCnf.CmdType = TAF_PH_CMD_QUERY;

            /* �ϱ��¼����ǰģʽ������д */
            stPhModeCnf.PhMode = TAF_SDC_GetCurPhoneMode();

            /* �ϱ�TAF_PH_EVT_OPER_MODE_CNF�¼� */
            MMA_PhModeReport(pstRcvMsg->clientId, pstRcvMsg->opId, stPhModeCnf, TAF_ERR_NO_ERROR);

            ulRet = VOS_TRUE;
            break;

        default:

            /* ������ַ����� */
            TAF_WARNING_LOG(WUEPS_PID_MMA, "TAF_MMA_RcvOmPhoneModeSetReq_PreProc:invalid para!");

            stPhModeCnf.CmdType    = pstPhModeSet->CmdType;
            stPhModeCnf.PhMode     = TAF_SDC_GetCurPhoneMode();
            MMA_PhModeReport(pstRcvMsg->clientId, pstRcvMsg->opId, stPhModeCnf, TAF_ERR_PARA_ERROR);
            ulRet = VOS_TRUE;
            break;
    }

    return ulRet;
}
VOS_UINT32 TAF_MMA_RcvPhoneModeQuery_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MN_APP_REQ_MSG_STRU                *pstRcvMsg       = VOS_NULL_PTR;
    TAF_PH_OP_MODE_CNF_STRU             stPhModeCnf;

    pstRcvMsg       = (MN_APP_REQ_MSG_STRU *)pstMsg;

    /* �ϱ��¼�����������д */
    stPhModeCnf.CmdType = TAF_PH_CMD_QUERY;

    /* �ϱ��¼����ǰģʽ������д */
    stPhModeCnf.PhMode = TAF_SDC_GetCurPhoneMode();

    /* �ϱ�TAF_PH_EVT_OPER_MODE_CNF�¼� */
    MMA_PhModeReport(pstRcvMsg->clientId, pstRcvMsg->opId, stPhModeCnf, TAF_ERR_NO_ERROR);

    return VOS_TRUE;
}
VOS_VOID TAF_MMA_SaveInterUsimStatusChangeIndMsgInCacheQueue_PreProc(VOS_VOID)
{
    TAF_MMA_INTER_USIM_STATUS_CHANGE_IND_STRU               stInternalMsg;
    VOS_UINT32                                              ulEventType;

    PS_MEM_SET(&stInternalMsg, 0, sizeof(TAF_MMA_INTER_USIM_STATUS_CHANGE_IND_STRU));

    stInternalMsg.MsgHeader.ulSenderCpuId   = VOS_LOCAL_CPUID;
    stInternalMsg.MsgHeader.ulSenderPid     = WUEPS_PID_MMA;
    stInternalMsg.MsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    stInternalMsg.MsgHeader.ulReceiverPid   = WUEPS_PID_MMA;
    stInternalMsg.MsgHeader.ulMsgName       = MMA_MMA_INTER_USIM_STATUS_CHANGE_IND;
    stInternalMsg.MsgHeader.ulLength        = sizeof(TAF_MMA_INTER_USIM_STATUS_CHANGE_IND_STRU) - VOS_MSG_HEAD_LENGTH;

    /* �����ڲ���״ָ̬ʾ��Ϣ */
    ulEventType = TAF_BuildEventType(WUEPS_PID_MMA, MMA_MMA_INTER_USIM_STATUS_CHANGE_IND);
    TAF_MMA_SaveCacheMsgInMsgQueue(ulEventType, &stInternalMsg);
}
VOS_UINT32 TAF_MMA_RcvPihUsimStatusInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    PS_USIM_STATUS_IND_STRU            *pstUsimMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulUsimStaChg;
    VOS_UINT32                          ulImsiChg;
    TAF_PH_FPLMN_OPERATE_STRU           stFplmnOperate;
    VOS_UINT8                           ucMmaLogInfoFlag;

#if (FEATURE_ON == FEATURE_CL_INTERWORK)
    TAF_MMA_USIMM_CARD_TYPE_ENUM_UINT32           enCardType;                   /* ������:SIM��USIM��ROM-SIM  */
    TAF_MMA_CARD_STATUS_ENUM_UINT8                enCardStatus;
#endif

    pstUsimMsg      = (PS_USIM_STATUS_IND_STRU*)pstMsg;

    ucMmaLogInfoFlag = TAF_MMA_GetMmaLogInfoFlag();

    if (VOS_TRUE == ucMmaLogInfoFlag)
    {
        vos_printf("\n*TAF_MMA_RcvPihUsimStatusInd_PrePro:CurFsm State = %d\n", g_StatusContext.ulFsmState);
        vos_printf("\n*TAF_MMA_RcvPihUsimStatusInd_PrePro:enCardStatus = %d\n", (pstUsimMsg->enCardStatus));
        vos_printf("\n*TAF_MMA_RcvPihUsimStatusInd_PrePro:enCardType = %d\n", (pstUsimMsg->enCardType));
    }

    /* ������Ϣ AT_MMA_USIM_STATUS_IND ��AT */
    MMA_PhUsimStatusInd(pstUsimMsg);

#if (FEATURE_ON == FEATURE_CL_INTERWORK)
    TAF_MMA_ConvertCardType(pstUsimMsg->enCardType, &enCardType);
    TAF_MMA_ConvertCardStatus(pstUsimMsg->enCardStatus, &enCardStatus);
    TAF_MMA_SndSimStatusInd(enCardType, enCardStatus);
#endif

    /* ���¿����� */
    TAF_MMA_ChangeUsimType(pstUsimMsg->enCardType);

    /* ���յ�SIM�ϱ��Ŀ�״̬ʱ��֪ͨSMSģ�鵱ǰ�Ŀ�״̬ */
    MN_PH_SndMsgUsimStatus(pstUsimMsg->enCardStatus);

    ulUsimStaChg    = TAF_MMA_IsUsimStatusChanged(pstUsimMsg->enCardStatus);
    ulImsiChg       = TAF_MMA_IsImsiChanged(pstUsimMsg);

    if (VOS_TRUE == ulImsiChg)
    {
        TAF_SDC_SetSimImsi(pstUsimMsg->aucIMSI);

        TAF_SDC_SetLastSimImsi(pstUsimMsg->aucIMSI);
    }

    if (USIMM_CARD_SERVIC_AVAILABLE == pstUsimMsg->enCardStatus)
    {
#if (VOS_WIN32 == VOS_OS_VER)
        /* PC���̳�������������룬��״̬�ı����½�����������У�� */
        g_stMmaMePersonalisationStatus.SimLockStatus = MMA_SIM_IS_UNLOCK;
#endif
        /* ͻ��˹�����޸�:��SIM��״̬�����ı�,����IMSI�����ı�ʱ,��Ҫ����У������״̬.
           ��ΪУ�麯��ֻ����MMA_SIM_IS_UNLOCK״̬�Ż�����У��,�������³�ʼ���˱��� */
        if ((VOS_TRUE == ulUsimStaChg)
         || (VOS_TRUE == ulImsiChg))
        {
            g_stMmaMePersonalisationStatus.SimLockStatus = MMA_SIM_IS_UNLOCK;
        }

        /* ����Ƿ����� */
        MMA_CheckMePersonalisationStatus();
    }

    MMA_ChangeUsimStatus(pstUsimMsg->enCardStatus);

    /* ���¿���IMSI��SDC�е�IMSI״̬�Լ��������У���,���ϱ���״ָ̬ʾ��AT */
    TAF_MMA_SndAtIccStatusInd_PreProc(ulUsimStaChg);

    /* �����������յ���״ָ̬ʾ�Ĵ��� */
    if (TAF_MMA_FSM_PHONE_MODE == TAF_MMA_GetCurrFsmId())
    {
        if (VOS_TRUE == ucMmaLogInfoFlag)
        {
            vos_printf("*TAF_MMA_RcvPihUsimStatusInd_PrePro:Cur Phone Mode Fsm is running\n");
        }
        return TAF_MMA_ProcUsimStatusIndInPhoneModeProc_PreProc(ulUsimStaChg, ulImsiChg);
    }

    /* ��״̬�Լ�IMSI��δ�ı��򷵻� */
    if ((VOS_FALSE == ulUsimStaChg)
     && (VOS_FALSE == ulImsiChg))
    {
        if (VOS_TRUE == ucMmaLogInfoFlag)
        {
            vos_printf("*TAF_MMA_RcvPihUsimStatusInd_PrePro:UsimStaChg = VOS_FALSE, ulImsiChg = VOS_FALSE\n");
        }
        return VOS_TRUE;
    }

    if (VOS_TRUE == MN_MMA_GetRoamingBrokerFlg())
    {
        stFplmnOperate.ulCmdType = TAF_PH_FPLMN_DEL_ALL;
        Api_MmaFplmnHandleFromMmc(0xffff, 0, stFplmnOperate);
    }

    /* ����п����Ͷ��ļ����� */
    if (TAF_SDC_USIM_STATUS_VALID == TAF_SDC_GetSimStatus())
    {
        TAF_MMA_ReadUsimInfo_PreProc();
    }

    /* �ǿ����������յ���״̬�������ػ����� */
    if (STA_FSM_NULL == g_StatusContext.ulFsmState)
    {
        return VOS_TRUE;
    }

    /* �Ѿ����������main�����ػ����� */
    return VOS_FALSE;
}
VOS_UINT32 TAF_MMA_RcvMmaInterUsimStatusChangeInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �����ǰΪNULL״̬����Ҫ���� */
    if (STA_FSM_NULL == g_StatusContext.ulFsmState)
    {
        return VOS_TRUE;
    }

    /* ����phone mode״̬�� */
    return VOS_FALSE;
}


VOS_UINT32 TAF_MMA_RcvMmaSimLockStatusChangeInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �����ǰ��available״̬��������Ϣ */
    if (TAF_SDC_USIM_STATUS_VALID != TAF_SDC_GetSimStatus())
    {
        return VOS_TRUE;
    }

    /* �ϱ�һ�ο�״̬ */
    TAF_MMA_SndAtIccStatusInd_PreProc(VOS_TRUE);

    /* ����������,�����ڲ���״̬�ı�ָʾ��Ϣ���� */
    if (TAF_MMA_FSM_PHONE_MODE == TAF_MMA_GetCurrFsmId())
    {
        TAF_MMA_SaveInterUsimStatusChangeIndMsgInCacheQueue_PreProc();

        return VOS_TRUE;
    }

    /* �����ǰΪNULL״̬����Ҫ���� */
    if (STA_FSM_NULL == g_StatusContext.ulFsmState)
    {
        return VOS_TRUE;
    }

    /* ����phone mode״̬�� */
    return VOS_FALSE;
}

VOS_UINT32 TAF_MMA_RcvAtSyscfgHandle_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MN_APP_REQ_MSG_STRU                *pstRcvMsg = VOS_NULL_PTR;

    pstRcvMsg   = (MN_APP_REQ_MSG_STRU *)pstMsg;

    /*��ѯ�����*/
    MMA_PhSysCfgQuery(pstRcvMsg->clientId, pstRcvMsg->opId);

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvAtParaReadReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MN_APP_REQ_MSG_STRU                *pstRcvMsg = VOS_NULL_PTR;
    VOS_UINT16                          usClientId;
    VOS_UINT8                           ucOpId;
    TAF_PARA_TYPE                       ucParaType;

    pstRcvMsg = (MN_APP_REQ_MSG_STRU *)pstMsg;

    ucParaType  = pstRcvMsg->aucContent[0];
    usClientId  = pstRcvMsg->clientId;
    ucOpId      = pstRcvMsg->opId;

    /* MMA������ѯ���� */
    if ((ucParaType >= TAF_PH_MFR_ID_PARA )
     && (ucParaType <= (TAF_PH_PRODUCT_NAME_PARA + 1)))
    {
        MMA_QueryProc(usClientId, ucOpId, &ucParaType);

        return VOS_TRUE;
    }

    switch (ucParaType)
    {
        case TAF_PH_ACCESS_MODE_PARA:
        case TAF_PH_MS_CLASS_PARA:
        case TAF_PH_REG_PLMN_PARA:

            MMA_DbQuery(usClientId, ucOpId, ucParaType);
            return VOS_TRUE;

        default:

            TAF_WARNING_LOG(WUEPS_PID_MMA, "TAF_MMA_RcvAtParaReadReq_PreProc:invalid query para!");
            break;
    }

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvMmcNetworkCapabilityInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MMA_NETWORK_CAPABILITY_INFO_IND_STRU               *pstRcvMsg       = VOS_NULL_PTR;
    TAF_SDC_NETWORK_CAP_INFO_STRU                           stNewNwCapInfo;
    VOS_UINT32                                              ulNwCapInfoChanged;

    pstRcvMsg = (MMC_MMA_NETWORK_CAPABILITY_INFO_IND_STRU *)pstMsg;

    stNewNwCapInfo.enNwEmcBsCap = (TAF_SDC_NW_EMC_BS_CAP_ENUM_UINT8)pstRcvMsg->enNwEmcBsCap;
    stNewNwCapInfo.enNwImsVoCap = (TAF_SDC_NW_IMS_VOICE_CAP_ENUM_UINT8)pstRcvMsg->enNwImsVoCap;

    switch (pstRcvMsg->enLteCsCap)
    {
        case MMA_MMC_LTE_CS_CAPBILITY_NO_ADDITION_INFO:
            stNewNwCapInfo.enLteCsCap = TAF_SDC_LTE_CS_CAPBILITY_NO_ADDITION_INFO;
            break;

        case MMA_MMC_LTE_CS_CAPBILITY_CSFB_NOT_PREFER:
            stNewNwCapInfo.enLteCsCap = TAF_SDC_LTE_CS_CAPBILITY_CSFB_NOT_PREFER;
            break;

        case MMA_MMC_LTE_CS_CAPBILITY_SMS_ONLY:
            stNewNwCapInfo.enLteCsCap = TAF_SDC_LTE_CS_CAPBILITY_SMS_ONLY;
            break;

        case MMA_MMC_LTE_CS_CAPBILITY_NOT_SUPPORTED:
            stNewNwCapInfo.enLteCsCap = TAF_SDC_LTE_CS_CAPBILITY_NOT_SUPPORTED;
            break;

        default:
            stNewNwCapInfo.enLteCsCap = TAF_SDC_LTE_CS_CAPBILITY_BUTT;
            break;
    }

    /* �жϵ�ǰ������������GU��L����������Ƿ����˸ı� */
    ulNwCapInfoChanged = TAF_MMA_IsNetworkCapInfoChanged(&stNewNwCapInfo);

    if (VOS_TRUE == ulNwCapInfoChanged)
    {
        /* update infomaton to the corresponding global variable according to RAT */
        TAF_MMA_SetNetworkCapabilityInfo(TAF_SDC_GetSysMode(), &stNewNwCapInfo);

#if (FEATURE_IMS == FEATURE_ON)
        if (VOS_TRUE == TAF_SDC_GetImsSupportFlag())
        {
            /* ֪ͨSPM�����������������SPM���������Ϣ����Ϣ�в�����ǰ��������������SPMȡSDCȫ�ֱ����е� */
            TAF_MMA_SndSpmNetworkCapabilityChangeNotify();

            /* send service infomation change indcation */
            TAF_MMA_SndImsaSrvInfoNotify((MMA_MMC_SERVICE_STATUS_ENUM_UINT32)TAF_SDC_GetPsServiceStatus());
        }
#endif
    }


    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvPihUsimRefreshIndMsgProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    PS_USIM_REFRESH_IND_STRU           *pUsimRefreshIndMsg = VOS_NULL_PTR;

    pUsimRefreshIndMsg = (PS_USIM_REFRESH_IND_STRU*)pstMsg;

    /* ĿǰPIH�ϱ��ļ�ˢ��ָʾ��Ϣ��, USIM�����ϱ���״ָ̬ʾ��MMA
       �����˴��߼���USIMɾ����,MMA��Ҫ���¶�ȡIMSI�Ƚ��Ƿ����ı�
       ,�����йػ��Ϳ������� */
    if (USIMM_REFRESH_ALL_FILE == pUsimRefreshIndMsg->enRefreshType)
    {
        /* refresh��Ϣָʾ�����ļ���������� */
        TAF_MMA_UsimRefreshIndAllFileChangedMsgProc(pUsimRefreshIndMsg);

        return VOS_TRUE;
    }

    if (USIMM_REFRESH_FILE_LIST == pUsimRefreshIndMsg->enRefreshType)
    {
        /* refresh ��Ϣָʾ�ض��ļ�������� */
        TAF_MMA_UsimRefreshIndFileListChangedMsgProc(pUsimRefreshIndMsg);

        return VOS_TRUE;
    }

    if (USIMM_REFRESH_3G_SESSION_RESET == pUsimRefreshIndMsg->enRefreshType)
    {
       return TAF_MMA_ProcPihUsimRefreshInd3gSessionReset_PreProc();
    }

    return VOS_TRUE;
}




VOS_UINT32 TAF_MMA_ProcPihUsimRefreshInd3gSessionReset_PreProc(VOS_VOID)
{
    TAF_MMA_ReadUsimInfo_PreProc();

    /* ����������,�����ڲ���״̬�ı�ָʾ��Ϣ���� */
    if (TAF_MMA_FSM_PHONE_MODE == TAF_MMA_GetCurrFsmId())
    {
        TAF_MMA_SaveInterUsimStatusChangeIndMsgInCacheQueue_PreProc();

        return VOS_TRUE;
    }

    /* �ǿ����������յ���״̬�������ػ����� */
    if (STA_FSM_NULL == g_StatusContext.ulFsmState)
    {
       return VOS_TRUE;
    }

    /* �Ѿ����������main�����ػ����� */
    return VOS_FALSE;
}

VOS_VOID TAF_MMA_ReadUsimInfo_PreProc(VOS_VOID)
{
    /* ��ȡSPN�ļ� */
    TAF_MMA_ReadSpnFile();

    MMA_ReadSimCPHSOperNameStr();

    /* ����ѡ��˵�����ʹ�ܣ�������Ҫ��6F15�ļ� */
    TAF_MMA_ReadCustomerServiceProfileFile_SwitchOn();

    /* ���뼼��ƽ�����ʹ�ܣ�������4F36�ļ� */
    TAF_MMA_ReadRatModeFile();
}

VOS_VOID TAF_MMA_SndAtIccStatusInd_PreProc(
    VOS_UINT32                          ulUsimStaChg
)
{
    VOS_UINT8                           ucSimStatus;
    VOS_UINT8                           ucSimLockStatus;

    ucSimStatus                         = MMA_GetUsimStatus(ulUsimStaChg);

    /* ��ȡ����״̬ */
    if ( MMA_SIM_IS_LOCK == MMA_GetMeLockStatus() )
    {
        ucSimLockStatus                 = VOS_TRUE;
    }
    else
    {
        ucSimLockStatus                 = VOS_FALSE;
    }

    /* �Ȳ���ڼ䲻�ϱ�SIMST��AP */
   if ((TAF_PH_MODE_FULL == TAF_SDC_GetCurPhoneMode())
    && (VOS_TRUE         == g_ucUsimHotOutFlag))
   {
       g_ucUsimHotOutFlag = VOS_FALSE;

       return;
   }

   /* ����RIL�߼�,�ϵ��һ�������յ�����Э��ջ������ɵ�����(PSINIT)
      �ϵ�ʱ�ϱ��Ŀ�״̬�ӳٵ�Э��ջ��ʼ�����֮���ϱ���ȥ */
   if (TAF_MMA_PHONE_MODE_STA_WAIT_PIH_USIM_STATUS_IND == TAF_MMA_FSM_GetFsmTopState())
   {
       return;
   }

   TAF_MMA_SndAtIccStatusInd(ucSimStatus, ucSimLockStatus);

   return;
}




VOS_UINT32 TAF_MMA_ProcUsimStatusIndInPhoneModeProc_PreProc(
    VOS_UINT32                          ulUsimStaChg,
    VOS_UINT32                          ulImsiChg
)
{
    /* ����������,�ȴ���״ָ̬ʾ״̬ʱֱ�ӽ�״̬���������Ϣ */
    if (TAF_MMA_PHONE_MODE_STA_WAIT_PIH_USIM_STATUS_IND == TAF_MMA_FSM_GetFsmTopState())
    {
        /* ����п����Ͷ��ļ����� */
        if (TAF_SDC_USIM_STATUS_VALID == TAF_SDC_GetSimStatus())
        {
            TAF_MMA_ReadUsimInfo_PreProc();
        }

        return VOS_FALSE;
    }

    /* ��״̬��IMSI�����ı������¶�ȡ���ļ������濨״̬�ı���Ϣ */
    if ((VOS_TRUE == ulUsimStaChg)
     || (VOS_TRUE == ulImsiChg))
    {
        if (TAF_SDC_USIM_STATUS_VALID == TAF_SDC_GetSimStatus())
        {
            TAF_MMA_ReadUsimInfo_PreProc();
        }

        TAF_MMA_SaveInterUsimStatusChangeIndMsgInCacheQueue_PreProc();
    }

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvMmcCampOnInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MMA_CAMP_ON_IND_STRU_STRU      *pstCampOnInd = VOS_NULL_PTR;

    pstCampOnInd = (MMC_MMA_CAMP_ON_IND_STRU_STRU*)pstMsg;

    if (VOS_TRUE == pstCampOnInd->ucCampOnFlg)
    {
        TAF_SDC_SetCampOnFlag(VOS_TRUE);
    }
    else
    {
        TAF_SDC_SetCampOnFlag(VOS_FALSE);
    }

    return VOS_TRUE;
}



TAF_PH_RAT_TYPE_ENUM_UINT8 TAF_MMA_ConvertSdcRatToAppRatType(
                        TAF_SDC_SYS_MODE_ENUM_UINT8 enSdcRatType
                        )
{
    switch (enSdcRatType)
    {
        case TAF_SDC_SYS_MODE_GSM:
             return TAF_PH_RAT_GSM;

        case TAF_SDC_SYS_MODE_WCDMA:
             return TAF_PH_RAT_WCDMA;

        case TAF_SDC_SYS_MODE_LTE:
             return TAF_PH_RAT_LTE;

        default:
            MMA_WARNINGLOG("TAF_MMA_ConvertSdcRatToAppRatType():WORNING:The input is invalid.");
            return TAF_PH_RAT_BUTT;
    }
}


TAF_PH_SERVICE_DOMAIN TAF_MMA_ConvertMmcDomainToMmaDomainType(
                        MMA_MMC_SRVDOMAIN_ENUM_UINT32 enMmcDomainType
                        )
{
    switch (enMmcDomainType)
    {
        case MMA_MMC_SRVDOMAIN_NO_DOMAIN:
        case MMA_MMC_SRVDOMAIN_NOT_REG_PLMN_SEARCHING:
             return TAF_PH_SERVICE_NULL;

        case MMA_MMC_SRVDOMAIN_CS:
             return TAF_PH_SERVICE_CS;

        case MMA_MMC_SRVDOMAIN_PS:
             return TAF_PH_SERVICE_PS;

        case MMA_MMC_SRVDOMAIN_CS_PS:
             return TAF_PH_SERVICE_CS_PS;

        case MMA_MMC_SRVDOMAIN_NO_CHANGE:
             return TAF_PH_SERVICE_NOCHANGE;

        default:
            MMA_WARNINGLOG("TAF_MMA_ConvertMmcDomainToMmaDomainType():WORNING:The input is invalid.");
            return TAF_PH_SERVICE_NULL;
    }
}

VOS_VOID TAF_MMA_SndRegRejInfoInd(
    MMC_MMA_REG_RESULT_IND_STRU        *pstRejInfoInd
)
{
    /* ��MMC����������ת��ΪTAF���������ͣ������ϱ� */
    TAF_PH_REG_REJ_INFO_STRU            stTafRejInfo;
    TAF_SDC_CAMP_PLMN_INFO_STRU        *pstCampPlmnInfo = VOS_NULL_PTR;

    /* ����ϱ��¼� */
    pstCampPlmnInfo           = TAF_SDC_GetCampPlmnInfo();
    stTafRejInfo.enRat        = TAF_MMA_ConvertSdcRatToAppRatType(pstCampPlmnInfo->enSysMode);
    stTafRejInfo.stPlmnId.Mcc = pstCampPlmnInfo->stPlmnId.ulMcc;
    stTafRejInfo.stPlmnId.Mnc = pstCampPlmnInfo->stPlmnId.ulMnc;
    stTafRejInfo.ucSrvDomain  = TAF_MMA_ConvertMmcDomainToMmaDomainType(pstRejInfoInd->enCnDomainId);
    stTafRejInfo.ulRejCause   = pstRejInfoInd->enRejCause;

    /* �ܾ�����Ϊע�ᱻ�� */
    stTafRejInfo.ucRejType    = MMA_MMC_REJ_TYPE_REG;

    /* ��PLMN IDתΪBCD��ʽ */
    MMA_PlmnId2Bcd(&stTafRejInfo.stPlmnId);

    /* �����Ϣͷ */
    stTafRejInfo.PhoneEvent = TAF_MMA_EVT_REG_REJ_INFO_IND;
    stTafRejInfo.OpId       = MMA_OP_ID_INTERNAL;
    stTafRejInfo.ClientId   = MMA_CLIENTID_BROADCAST;

    /* ���õ绰�����ϱ����� */
    MN_PH_SendMsg(stTafRejInfo.ClientId,(VOS_UINT8*)&stTafRejInfo,sizeof(TAF_PH_REG_REJ_INFO_STRU));

    return;
}


VOS_UINT32 TAF_MMA_RcvMmcRegResultInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MMA_REG_RESULT_IND_STRU        *pstRegResultInd   = VOS_NULL_PTR;
#if (FEATURE_ON == FEATURE_IMS)
    VOS_UINT32                          ulUeSupportIms;
#endif
    pstRegResultInd     = (MMC_MMA_REG_RESULT_IND_STRU *)pstMsg;

#if (FEATURE_ON == FEATURE_IMS)
    /* IMS֧��ʱ��PSע����ɺ�֪ͨһ��IMSA,��֤��ϵͳ������,RAT���ʱ,IMSA��֪ */
    if ((VOS_TRUE                 == TAF_SDC_GetImsSupportFlag())
     && ((MMA_MMC_SRVDOMAIN_PS    == pstRegResultInd->enCnDomainId)
      || (MMA_MMC_SRVDOMAIN_CS_PS == pstRegResultInd->enCnDomainId)))
    {
        TAF_MMA_SndImsaSrvInfoNotify((MMA_MMC_SERVICE_STATUS_ENUM_UINT32)TAF_SDC_GetPsServiceStatus());
    }
#endif

    /* ע��ʧ��ʱ����Ҫ�ϱ��ܾ�ԭ��ֵ */
    if ((VOS_FALSE == pstRegResultInd->ucRegRslt)
     && (VOS_TRUE  == pstRegResultInd->ucReportCauseCtrl))
    {
        TAF_MMA_SndRegRejInfoInd(pstRegResultInd);
    }

#if (FEATURE_ON == FEATURE_IMS)

    if ((MMA_MMC_SRVDOMAIN_PS    == pstRegResultInd->enCnDomainId)
     || (MMA_MMC_SRVDOMAIN_CS_PS == pstRegResultInd->enCnDomainId))
    {
        /* PSע��ʧ��ֱ��֪ͨIMS������ */
        if (VOS_FALSE == pstRegResultInd->ucRegRslt)
        {
            TAF_SDC_SetImsVoiceAvailFlg(VOS_FALSE);

            /* ֹͣ�ȴ�IMSA��IMS voice�Ƿ������Ϣ��ʱ�� */
            TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_IMSA_IMS_VOICE_CAP_NOTIFY);

            /* ֪ͨMMC��ǰIMS voice�Ƿ���� */
            TAF_MMA_SndMmcImsVoiceCapInd(VOS_FALSE);

            return VOS_TRUE;
        }

        ulUeSupportIms = TAF_SDC_IsImsVoiceSupportedInUE();

        /* ��ǰIMS��RAT NV��֧�ֻ�������ѡ��ΪCS ONLY�����粻֧��IMSʱ ��ΪIMS VOICE������*/
        if ((VOS_FALSE == TAF_SDC_IsImsVoiceSupportedInNW())
         || (VOS_FALSE == ulUeSupportIms))
        {
            TAF_SDC_SetImsVoiceAvailFlg(VOS_FALSE);

            /* ֹͣ�ȴ�IMSA��IMS voice�Ƿ������Ϣ��ʱ�� */
            TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_IMSA_IMS_VOICE_CAP_NOTIFY);

            /* ֪ͨMMC��ǰIMS voice�Ƿ���� */
            TAF_MMA_SndMmcImsVoiceCapInd(VOS_FALSE);

            return VOS_TRUE;
        }

        /* ��PSע��ɹ�ʱ����������ʱ���ȴ�IMSע����ɺ��ϱ�IMS voice�Ƿ���� */
        TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_IMSA_IMS_VOICE_CAP_NOTIFY, TAF_SDC_GetWaitImsVoiceAvailTimerLen());
    }
#else

    /* ֪ͨMMC��ǰIMS voice�Ƿ���� */
    TAF_MMA_SndMmcImsVoiceCapInd(VOS_FALSE);
#endif

    return VOS_TRUE;
}


#if (FEATURE_ON == FEATURE_IMS)

VOS_UINT32 TAF_MMA_RcvImsaImsVoiceCapNtf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    IMSA_MMA_IMS_VOICE_CAP_NOTIFY_STRU *pstImsVoiceCap = VOS_NULL_PTR;

    pstImsVoiceCap = (IMSA_MMA_IMS_VOICE_CAP_NOTIFY_STRU *)pstMsg;

    /* ��ǰ����ȴ�IMS�Ƿ���ö�ʱ���������������Ϣ;����ֱ�Ӻ��Ը���Ϣ */
    if (TAF_MMA_TIMER_STATUS_RUNING != TAF_MMA_GetTimerStatus(TI_TAF_MMA_WAIT_IMSA_IMS_VOICE_CAP_NOTIFY))
    {
        return VOS_TRUE;
    }

    /* ֹͣ�ȴ�IMSA��IMS voice�Ƿ������Ϣ��ʱ�� */
    TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_IMSA_IMS_VOICE_CAP_NOTIFY);

    /* ����IMS voice�Ƿ������Ϣ��SDC�� */
    if (MMA_IMSA_IMS_VOICE_CAP_AVAILABLE == pstImsVoiceCap->enImsVoiceCap)
    {
        TAF_SDC_SetImsVoiceAvailFlg(VOS_TRUE);
    }
    else
    {
        TAF_SDC_SetImsVoiceAvailFlg(VOS_FALSE);
    }

    /* ֪ͨMMC��ǰIMS voice�Ƿ���� */
    TAF_MMA_SndMmcImsVoiceCapInd(TAF_SDC_GetImsVoiceAvailFlg());

    /* ֪ͨSPM��ǰIMS VOICE�Ƿ���� */
    TAF_MMA_SndSpmImsVoiceCapInd(TAF_SDC_GetImsVoiceAvailFlg());

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvTiWaitImsaImsVoiceCapNtfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ����IMS voice��������Ϣ��SDC�� */
    TAF_SDC_SetImsVoiceAvailFlg(VOS_FALSE);

    /* ֪ͨMMC��ǰIMS voice������ */
    TAF_MMA_SndMmcImsVoiceCapInd(VOS_FALSE);

    /* ֪ͨSPM��ǰIMS VOICE�Ƿ���� */
    TAF_MMA_SndSpmImsVoiceCapInd(TAF_SDC_GetImsVoiceAvailFlg());

    return VOS_TRUE;
}

#endif


VOS_UINT32 TAF_MMA_RcvMmcUsimAuthFailInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��MMC����������ת��ΪTAF���������ͣ������ϱ� */
    TAF_PH_REG_REJ_INFO_STRU            stTafRejInfo;
    TAF_SDC_CAMP_PLMN_INFO_STRU        *pstCampPlmnInfo = VOS_NULL_PTR;
    MMC_MMA_USIM_AUTH_FAIL_IND_STRU    *pstAuthFailInd  = VOS_NULL_PTR;

    pstAuthFailInd            = (MMC_MMA_USIM_AUTH_FAIL_IND_STRU *)pstMsg;

    /* ����ϱ��¼� */
    pstCampPlmnInfo           = TAF_SDC_GetCampPlmnInfo();
    stTafRejInfo.enRat        = TAF_MMA_ConvertSdcRatToAppRatType(pstCampPlmnInfo->enSysMode);
    stTafRejInfo.stPlmnId.Mcc = pstCampPlmnInfo->stPlmnId.ulMcc;
    stTafRejInfo.stPlmnId.Mnc = pstCampPlmnInfo->stPlmnId.ulMnc;
    stTafRejInfo.ucSrvDomain  = TAF_MMA_ConvertMmcDomainToMmaDomainType(pstAuthFailInd->enCnDomainId);

    /* Ϊ�˺�����ܾ�ԭ��ֵ���֣�usim��Ȩʧ��ԭ��ֵ����һ��ƫ��λ */
    stTafRejInfo.ulRejCause   = pstAuthFailInd->enRejCause + (1 << 16);

    stTafRejInfo.ucReserved  = 0;
    stTafRejInfo.ucRejType   = MMA_MMC_REJ_TYPE_USIM_AUTH;

    /* ��PLMN IDתΪBCD��ʽ */
    MMA_PlmnId2Bcd(&stTafRejInfo.stPlmnId);

    /* �����Ϣͷ */
    stTafRejInfo.PhoneEvent = TAF_MMA_EVT_REG_REJ_INFO_IND;
    stTafRejInfo.OpId       = MMA_OP_ID_INTERNAL;
    stTafRejInfo.ClientId   = MMA_CLIENTID_BROADCAST;

    /* ���õ绰�����ϱ����� */
    MN_PH_SendMsg(stTafRejInfo.ClientId,(VOS_UINT8*)&stTafRejInfo,sizeof(TAF_PH_REG_REJ_INFO_STRU));

    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvMmcCsServiceConnStatusInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MMA_CS_SERVICE_CONN_STATUS_IND_STRU                *pstCsServiceConnStatusInd = VOS_NULL_PTR;

    pstCsServiceConnStatusInd = (MMC_MMA_CS_SERVICE_CONN_STATUS_IND_STRU *)pstMsg;

    TAF_SDC_SetCsServiceConnStatusFlag(pstCsServiceConnStatusInd->ucCsServiceConnStatusFlag);

    return VOS_TRUE;
}



VOS_UINT32 TAF_MMA_RcvMmcServRejInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MMA_SERV_REJ_IND_STRU          *pstServRejInd = VOS_NULL_PTR;
    TAF_PH_REG_REJ_INFO_STRU            stTafServRejInfo;
    TAF_SDC_CAMP_PLMN_INFO_STRU        *pstCampPlmnInfo = VOS_NULL_PTR;

    pstServRejInd = (MMC_MMA_SERV_REJ_IND_STRU *) pstMsg;
    PS_MEM_SET(&stTafServRejInfo, 0, sizeof(stTafServRejInfo));

    /* ����ϱ��¼� */
    pstCampPlmnInfo               = TAF_SDC_GetCampPlmnInfo();
    stTafServRejInfo.enRat        = TAF_MMA_ConvertSdcRatToAppRatType(pstCampPlmnInfo->enSysMode);
    stTafServRejInfo.stPlmnId.Mcc = pstCampPlmnInfo->stPlmnId.ulMcc;
    stTafServRejInfo.stPlmnId.Mnc = pstCampPlmnInfo->stPlmnId.ulMnc;
    stTafServRejInfo.ucSrvDomain  = TAF_MMA_ConvertMmcDomainToMmaDomainType(pstServRejInd->enCnDomainId);

    stTafServRejInfo.ulRejCause   = pstServRejInd->usRejCause;
    stTafServRejInfo.ucRejType    = MMA_MMC_REJ_TYPE_SERV_REQ;

    /* ��PLMN IDתΪBCD��ʽ */
    MMA_PlmnId2Bcd(&stTafServRejInfo.stPlmnId);

    /* �����Ϣͷ */
    stTafServRejInfo.PhoneEvent = TAF_MMA_EVT_REG_REJ_INFO_IND;
    stTafServRejInfo.OpId       = MMA_OP_ID_INTERNAL;
    stTafServRejInfo.ClientId   = MMA_CLIENTID_BROADCAST;

    /* ���õ绰�����ϱ����� */
    MN_PH_SendMsg(stTafServRejInfo.ClientId,(VOS_UINT8*)&stTafServRejInfo,sizeof(TAF_PH_REG_REJ_INFO_STRU));
    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvMmcDetachInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MMA_DETACH_IND_STRU            *pstDetachInd = VOS_NULL_PTR;
    TAF_PH_REG_REJ_INFO_STRU            stTafServRejInfo;
    TAF_SDC_CAMP_PLMN_INFO_STRU        *pstCampPlmnInfo = VOS_NULL_PTR;

    pstDetachInd = (MMC_MMA_DETACH_IND_STRU *) pstMsg;
    PS_MEM_SET(&stTafServRejInfo, 0, sizeof(stTafServRejInfo));

    if (MMA_MMC_NETWORK_DETACH_TYPE_NOATTACH == pstDetachInd->enNetworkDetachType)
    {
        /* ����ϱ��¼� */
        pstCampPlmnInfo               = TAF_SDC_GetCampPlmnInfo();
        stTafServRejInfo.enRat        = TAF_MMA_ConvertSdcRatToAppRatType(pstCampPlmnInfo->enSysMode);
        stTafServRejInfo.stPlmnId.Mcc = pstCampPlmnInfo->stPlmnId.ulMcc;
        stTafServRejInfo.stPlmnId.Mnc = pstCampPlmnInfo->stPlmnId.ulMnc;
        stTafServRejInfo.ucSrvDomain  = TAF_MMA_ConvertMmcDomainToMmaDomainType(pstDetachInd->enCnDomainId);

        stTafServRejInfo.ulRejCause   = pstDetachInd->ulDetachCause;
        stTafServRejInfo.ucRejType    = MMA_MMC_REJ_TYPE_NETWORK_DETACH_IND;

        /* ��PLMN IDתΪBCD��ʽ */
        MMA_PlmnId2Bcd(&stTafServRejInfo.stPlmnId);

        /* �����Ϣͷ */
        stTafServRejInfo.PhoneEvent = TAF_MMA_EVT_REG_REJ_INFO_IND;
        stTafServRejInfo.OpId       = MMA_OP_ID_INTERNAL;
        stTafServRejInfo.ClientId   = MMA_CLIENTID_BROADCAST;

        /* ���õ绰�����ϱ����� */
        MN_PH_SendMsg(stTafServRejInfo.ClientId, (VOS_UINT8*)&stTafServRejInfo, sizeof(TAF_PH_REG_REJ_INFO_STRU));
    }

    /* ��״̬������ */
    return VOS_FALSE;
}



VOS_VOID TAF_MMA_ConvertMmcCauseToTafFormat(
    MMC_MMA_NET_SCAN_CAUSE_ENUM_UINT8   enMmcCause,
    TAF_MMA_NET_SCAN_CAUSE_ENUM_UINT8  *pstMmaCause
)
{
    switch ( enMmcCause )
    {
        case MMC_MMA_NET_SCAN_CAUSE_SIGNAL_EXIST :
            *pstMmaCause = TAF_MMA_NET_SCAN_CAUSE_SIGNAL_EXIST;
            break;

        case MMC_MMA_NET_SCAN_CAUSE_STATE_NOT_ALLOWED :
            *pstMmaCause = TAF_MMA_NET_SCAN_CAUSE_STATE_NOT_ALLOWED;
            break;

        case MMC_MMA_NET_SCAN_CAUSE_FREQ_LOCK :
           *pstMmaCause = TAF_MMA_NET_SCAN_CAUSE_FREQ_LOCK;
            break;

        case MMC_MMA_NET_SCAN_CAUSE_PARA_ERROR :
            *pstMmaCause = TAF_MMA_NET_SCAN_CAUSE_PARA_ERROR;
            break;

        case MMC_MMA_NET_SCAN_CAUSE_CONFLICT :
            *pstMmaCause = TAF_MMA_NET_SCAN_CAUSE_CONFLICT;
            break;

        case MMC_MMA_NET_SCAN_CAUSE_SERVICE_EXIST :
            *pstMmaCause = TAF_MMA_NET_SCAN_CAUSE_SERVICE_EXIST;
            break;

        case MMC_MMA_NET_SCAN_CAUSE_NOT_CAMPED :
            *pstMmaCause = TAF_MMA_NET_SCAN_CAUSE_NOT_CAMPED;
            break;

        case MMC_MMA_NET_SCAN_CAUSE_RAT_TYPE_ERROR :
            *pstMmaCause = TAF_MMA_NET_SCAN_CAUSE_RAT_TYPE_ERROR;
            break;

        default:
            *pstMmaCause = TAF_MMA_NET_SCAN_CAUSE_BUTT;
            break;
    }

    return;
}


VOS_VOID TAF_MMA_ConvertMmcBandToTafFormat(
    NAS_MML_MS_BAND_INFO_STRU          *pstMmcBand,
    TAF_USER_SET_PREF_BAND64           *pstUserBand
)
{
    MMA_USER_PREF_BAND_STRU            *pstUserBitBand      = VOS_NULL_PTR;
    GSM_BAND_STRU                      *pstGsmBitBand       = VOS_NULL_PTR;
    WCDMA_BAND_STRU                    *pstWcdmaBitBand     = VOS_NULL_PTR;
    MMA_USER_BAND_SET_UN                uUserSetBand;

    PS_MEM_SET(&uUserSetBand, 0x00, sizeof(MMA_USER_BAND_SET_UN));

    /* uUserSetBand���յ���MMC��ʽƵ�α����32λ */
    pstUserBitBand                        = &(uUserSetBand.BitBand);
    pstGsmBitBand                         = &(pstMmcBand->unGsmBand.stBitBand);
    pstWcdmaBitBand                       = &(pstMmcBand->unWcdmaBand.stBitBand);

    pstUserBitBand->BandGsm850            = pstGsmBitBand->BandGsm850;
    pstUserBitBand->BandGsmDcs1800        = pstGsmBitBand->BandGsm1800;
    pstUserBitBand->BandGsmEgsm900        = pstGsmBitBand->BandGsmE900;
    pstUserBitBand->BandGsmPcs1900        = pstGsmBitBand->BandGsm1900;
    pstUserBitBand->BandGsmPgsm900        = pstGsmBitBand->BandGsmP900;
    pstUserBitBand->BandGsmRgsm900        = pstGsmBitBand->BandGsmR900;

    pstUserBitBand->BandWcdma_I_Imt2k1    = pstWcdmaBitBand->BandWCDMA_I_2100;
    pstUserBitBand->BandWcdma_II_PCS_1900 = pstWcdmaBitBand->BandWCDMA_II_1900;
    pstUserBitBand->BandWcdma_III_1800    = pstWcdmaBitBand->BandWCDMA_III_1800;
    pstUserBitBand->BandWcdma_IV_1700     = pstWcdmaBitBand->BandWCDMA_IV_1700;
    pstUserBitBand->BandWcdma_V_850       = pstWcdmaBitBand->BandWCDMA_V_850;
    pstUserBitBand->BandWcdma_VI_800      = pstWcdmaBitBand->BandWCDMA_VI_800;
    pstUserBitBand->BandWcdma_VII_2600    = pstWcdmaBitBand->BandWCDMA_VII_2600;
    pstUserBitBand->BandWcdma_VIII_900    = pstWcdmaBitBand->BandWCDMA_VIII_900;
    pstUserBitBand->BandWcdma_IX_1700     = pstWcdmaBitBand->BandWCDMA_IX_J1700;
    pstUserBitBand->BandWCDMA_XIX_850     = pstWcdmaBitBand->BandWCDMA_XIX_850;
    pstUserBitBand->BandWcdma_XI_1500     = pstWcdmaBitBand->BandWCDMA_XI_1500;

    /* ��uUserSetBandת���ɵ�32λ�͸�32λ�ṹ */
    pstUserBand->ulBandHigh = uUserSetBand.ulPrefBand & TAF_PH_BAND_NEED_CHANGE_TO_64BIT;
    pstUserBand->ulBandLow  = uUserSetBand.ulPrefBand & (~TAF_PH_BAND_NEED_CHANGE_TO_64BIT);

    return;
}




VOS_VOID TAF_MMA_ConvertMmcNetScanCnfToMmaFormat(
    MMC_MMA_NET_SCAN_CNF_STRU          *pstMsg,
    TAF_MMA_NET_SCAN_CNF_STRU          *pstNetScanCnf
)
{
    VOS_UINT32                          i;

    /* �����Ϣͷ */
    pstNetScanCnf->PhoneEvent  = TAF_MMA_EVT_NET_SCAN_CNF;
    pstNetScanCnf->usClientId  = gstMmaValue.stNetScan.usClientId;
    pstNetScanCnf->ucOpId      = gstMmaValue.stNetScan.ucOpId;

    pstNetScanCnf->enResult    = pstMsg->enResult;
    pstNetScanCnf->ucFreqNum   = pstMsg->ucFreqNum;

    TAF_MMA_ConvertMmcCauseToTafFormat(pstMsg->enCause, &pstNetScanCnf->enCause);

    if ( TAF_MMA_NET_SCAN_MAX_FREQ_NUM < pstNetScanCnf->ucFreqNum )
    {
        pstNetScanCnf->ucFreqNum = TAF_MMA_NET_SCAN_MAX_FREQ_NUM;
    }

    for ( i = 0; i < pstNetScanCnf->ucFreqNum; i++ )
    {
        pstNetScanCnf->astNetScanInfo[i].usArfcn        = pstMsg->astNetScanInfo[i].usArfcn;
        pstNetScanCnf->astNetScanInfo[i].usC1           = pstMsg->astNetScanInfo[i].usC1;
        pstNetScanCnf->astNetScanInfo[i].usC2           = pstMsg->astNetScanInfo[i].usC2;
        pstNetScanCnf->astNetScanInfo[i].usLac          = pstMsg->astNetScanInfo[i].usLac;
        pstNetScanCnf->astNetScanInfo[i].ulMcc          = pstMsg->astNetScanInfo[i].ulMcc;
        pstNetScanCnf->astNetScanInfo[i].ulMnc          = pstMsg->astNetScanInfo[i].ulMnc;
        pstNetScanCnf->astNetScanInfo[i].usBsic         = pstMsg->astNetScanInfo[i].usBsic;
        pstNetScanCnf->astNetScanInfo[i].sRxlev         = pstMsg->astNetScanInfo[i].sRxlev;
        pstNetScanCnf->astNetScanInfo[i].sRssi          = pstMsg->astNetScanInfo[i].sRssi;
        pstNetScanCnf->astNetScanInfo[i].ulCellId       = pstMsg->astNetScanInfo[i].ulCellId;

        TAF_MMA_ConvertMmcBandToTafFormat(&pstMsg->astNetScanInfo[i].stBand,
                                          &pstNetScanCnf->astNetScanInfo[i].stBand);
    }


    return;
}
VOS_UINT32 TAF_MMA_RcvMmcNetScanCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_NET_SCAN_CNF_STRU           stNetScanCnf;
    MMC_MMA_NET_SCAN_CNF_STRU          *pstMmcNetScanCnf = VOS_NULL_PTR;

    pstMmcNetScanCnf = (MMC_MMA_NET_SCAN_CNF_STRU *)pstMsg;

    PS_MEM_SET(&stNetScanCnf, 0x00, sizeof(TAF_MMA_NET_SCAN_CNF_STRU));

    TAF_MMA_ConvertMmcNetScanCnfToMmaFormat(pstMmcNetScanCnf, &stNetScanCnf);

    if ( MMA_TIMER_RUN == g_stNetScanProtectTimer.ucTimerStatus )
    {
        TAF_MMA_StopNetScanProtectTimer();

        /* ���õ绰�����ϱ����� */
        MN_PH_SendMsg( stNetScanCnf.usClientId,
                       (VOS_UINT8*)&stNetScanCnf,
                       sizeof(TAF_MMA_NET_SCAN_CNF_STRU) );
    }
    else if ( MMA_TIMER_RUN == g_stAbortNetScanProtectTimer.ucTimerStatus )
    {
        TAF_MMA_StopAbortNetScanProtectTimer();

        /* ���õ绰�����ϱ����� */
        MN_PH_SendMsg( stNetScanCnf.usClientId,
                       (VOS_UINT8*)&stNetScanCnf,
                       sizeof(TAF_MMA_NET_SCAN_CNF_STRU) );
    }
    else
    {
        /* NETSCAN�ı�����ʱ���������У�����Ϊ���쳣�ظ� */
        MMA_WARNINGLOG("TAF_MMA_RcvMmcNetScanCnf: RCV UNEXPECTED NETSCAN CNF MSG!\r\n");
    }

    return VOS_TRUE;
}
VOS_VOID TAF_MMA_ConvertMmcAbortNetScanCnfToMmaFormat(
    MMC_MMA_ABORT_NET_SCAN_CNF_STRU    *pstMsg,
    TAF_MMA_NET_SCAN_CNF_STRU          *pstNetScanCnf
)
{
    VOS_UINT32                          i;

    pstNetScanCnf->PhoneEvent           = TAF_MMA_EVT_NET_SCAN_CNF;
    pstNetScanCnf->ucOpId               = gstMmaValue.stNetScan.ucOpId;
    pstNetScanCnf->usClientId           = gstMmaValue.stNetScan.usClientId;

    pstNetScanCnf->enResult             = TAF_MMA_NET_SCAN_RESULT_SUCCESS;
    pstNetScanCnf->enCause              = TAF_MMA_NET_SCAN_CAUSE_BUTT;
    pstNetScanCnf->ucFreqNum            = pstMsg->ucFreqNum;

    if ( TAF_MMA_NET_SCAN_MAX_FREQ_NUM < pstNetScanCnf->ucFreqNum )
    {
        pstNetScanCnf->ucFreqNum = TAF_MMA_NET_SCAN_MAX_FREQ_NUM;
    }

    for ( i = 0; i < pstNetScanCnf->ucFreqNum; i++ )
    {
        pstNetScanCnf->astNetScanInfo[i].usArfcn        = pstMsg->astNetScanInfo[i].usArfcn;
        pstNetScanCnf->astNetScanInfo[i].usC1           = pstMsg->astNetScanInfo[i].usC1;
        pstNetScanCnf->astNetScanInfo[i].usC2           = pstMsg->astNetScanInfo[i].usC2;
        pstNetScanCnf->astNetScanInfo[i].usLac          = pstMsg->astNetScanInfo[i].usLac;
        pstNetScanCnf->astNetScanInfo[i].ulMcc          = pstMsg->astNetScanInfo[i].ulMcc;
        pstNetScanCnf->astNetScanInfo[i].ulMnc          = pstMsg->astNetScanInfo[i].ulMnc;
        pstNetScanCnf->astNetScanInfo[i].usBsic         = pstMsg->astNetScanInfo[i].usBsic;
        pstNetScanCnf->astNetScanInfo[i].sRxlev         = pstMsg->astNetScanInfo[i].sRxlev;
        pstNetScanCnf->astNetScanInfo[i].sRssi          = pstMsg->astNetScanInfo[i].sRssi;
        pstNetScanCnf->astNetScanInfo[i].ulCellId       = pstMsg->astNetScanInfo[i].ulCellId;

        TAF_MMA_ConvertMmcBandToTafFormat(&pstMsg->astNetScanInfo[i].stBand,
                                          &pstNetScanCnf->astNetScanInfo[i].stBand);
    }

    return;
}



VOS_UINT32 TAF_MMA_RcvMmcAbortNetScanCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_NET_SCAN_CNF_STRU           stNetScanCnf;
    MMC_MMA_ABORT_NET_SCAN_CNF_STRU    *pstMmcAbortNetScanCnf = VOS_NULL_PTR;

    pstMmcAbortNetScanCnf = (MMC_MMA_ABORT_NET_SCAN_CNF_STRU *)pstMsg;

    if ( MMA_TIMER_RUN != g_stAbortNetScanProtectTimer.ucTimerStatus )
    {
        MMA_WARNINGLOG("TAF_MMA_RcvMmcAbortNetScanCnf:abort timer not run!!!\r\n");

        return VOS_TRUE;
    }

    PS_MEM_SET(&stNetScanCnf, 0x00, sizeof(stNetScanCnf));

    /* ��װ��ATģ��ظ���CNF�ṹ */
    TAF_MMA_ConvertMmcAbortNetScanCnfToMmaFormat(pstMmcAbortNetScanCnf, &stNetScanCnf);

    /* ֹͣNETSCAN�ı�����ʱ�� */
    TAF_MMA_StopAbortNetScanProtectTimer();

    /* ���õ绰�����ϱ����� */
    MN_PH_SendMsg( stNetScanCnf.usClientId,
                   (VOS_UINT8*)&stNetScanCnf,
                   sizeof(TAF_MMA_NET_SCAN_CNF_STRU) );

    return VOS_TRUE;
}


#if (FEATURE_MULTI_MODEM == FEATURE_ON)
VOS_UINT32 TAF_MMA_RcvMtcNcellInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ת����mmc */
    TAF_MMA_SndMmcNcellInfoInd(pstMsg);
    return VOS_TRUE;
}




VOS_UINT32 TAF_MMA_RcvMtcPsTransferInd_PreProc(
     VOS_UINT32                         ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ת����mmc */
    TAF_MMA_SndMmcPsTransferInd(pstMsg);
    return VOS_TRUE;
}





VOS_UINT32 TAF_MMA_RcvMtcOtherModemInfoNotify_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ת����mmc */
    TAF_MMA_SndMmcOtherModemInfoNotify(pstMsg);

    return VOS_TRUE;
}
#endif
VOS_UINT32 TAF_MMA_RcvMmcEplmnInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    MMC_MMA_EPLMN_INFO_IND_STRU        *pstEplmnInfoIndMsg  = VOS_NULL_PTR;

    pstEplmnInfoIndMsg   = (MMC_MMA_EPLMN_INFO_IND_STRU *)pstMsg;

    TAF_MMA_SndMtcEplmnInfoInd(pstEplmnInfoIndMsg);

#endif
    return VOS_TRUE;
}





VOS_UINT32 TAF_MMA_RcvMmaAcqReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_ACQ_REQ_STRU               *pstMmaAcqReqMsg    = VOS_NULL_PTR;
    MMC_MMA_ACQ_CNF_STRU                stSndMsg;
    VOS_UINT8                           ucCtxIndex;
    VOS_UINT32                          ulRet;

    pstMmaAcqReqMsg = (TAF_MMA_ACQ_REQ_STRU *)pstMsg;

    PS_MEM_SET(&stSndMsg, 0, sizeof(MMC_MMA_ACQ_CNF_STRU));

    /* ����ǹػ�״̬�����������ڿ��ػ�����ظ��ܾ� */
    if ((TAF_MMA_FSM_PHONE_MODE == TAF_MMA_GetCurrFsmId())
     || (STA_FSM_NULL           == g_StatusContext.ulFsmState)
     || (STA_FSM_STOP           == g_StatusContext.ulFsmState))
    {
        TAF_MMA_SndAcqBestNetworkCnf(&(pstMmaAcqReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_REJECT,
                          &stSndMsg);
        return VOS_TRUE;
    }

    /* �������CLģʽ������CMMCA��Ϣ�����ϱ�CMMCA�ܾ� */
    if ((VOS_FALSE              == TAF_SDC_IsConfigCLInterWork())
     && (CMMCA_CLIENT_ID        == pstMmaAcqReqMsg->stCtrl.usClientId))
    {
        TAF_MMA_SndAcqBestNetworkCnf(&(pstMmaAcqReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_REJECT,
                          &stSndMsg);
        return VOS_TRUE;
    }

    /* �������Ч��ֱ�ӻ�ʧ�� */
    if (VOS_FALSE == TAF_SDC_IsUsimStausValid())
    {
        TAF_MMA_SndAcqBestNetworkCnf(&(pstMmaAcqReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE,
                          &stSndMsg);
        return VOS_TRUE;
    }

    /* ���g_stTafMmaCtx.astMmaOperCtx��TAF_MMA_OPER_ACQ_REQ���ͽṹ���ڱ�ʹ�� */
    if (VOS_TRUE == TAF_MMA_IsOperTypeUsed(TAF_MMA_OPER_ACQ_REQ))
    {
        TAF_MMA_SndAcqBestNetworkCnf(&(pstMmaAcqReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE,
                          &stSndMsg);
        return VOS_TRUE;
    }

    /* �Ҳ���δ��ʹ�õģ��ظ�ʧ�� */
    if (VOS_FALSE == TAF_MMA_GetUnusedOperCtxIndex(&ucCtxIndex))
    {
        TAF_MMA_SndAcqBestNetworkCnf(&(pstMmaAcqReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE,
                          &stSndMsg);
        return VOS_TRUE;
    }

    /* ����һ��g_stTafMmaCtx.astMmaOperCtx����Ӧ���� */
    TAF_MMA_SetOperCtx(pstMmaAcqReqMsg->stCtrl,
                       TAF_MMA_OPER_ACQ_REQ, ucCtxIndex);

    /* ��MMC��acq req��Ϣ */
    ulRet = TAF_MMA_SndMmcAcqReq(&(pstMmaAcqReqMsg->stAcqPara));
    if (VOS_OK != ulRet)
    {
        return VOS_TRUE;
    }

    /* ����������ʱ��TI_TAF_MMA_WAIT_MMC_ACQ_CNF */
    TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_MMC_ACQ_CNF, TI_TAF_MMA_WAIT_MMC_ACQ_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_ConvertAttachDomainToAttachType(
    TAF_PH_SERVICE_DOMAIN               ucAttachDomain
)
{
    VOS_UINT32                          ulAttachType;

    switch ( ucAttachDomain )
    {
        case TAF_PH_SERVICE_CS:
            ulAttachType = STA_ATTACH_TYPE_IMSI;
            break;

        case TAF_PH_SERVICE_PS:
            ulAttachType = STA_ATTACH_TYPE_GPRS;
            break;

        case TAF_PH_SERVICE_CS_PS:
        case TAF_PH_SERVICE_ANY:
            ulAttachType = STA_ATTACH_TYPE_GPRS_IMSI;
            break;

        case TAF_PH_SERVICE_NULL:
            ulAttachType = STA_ATTACH_TYPE_NULL;
            break;

        case TAF_PH_SERVICE_NOCHANGE:
        default:
            ulAttachType = STA_ATTACH_TYPE_GPRS;
            break;

    }

    return ulAttachType;

}


VOS_UINT32 TAF_MMA_RcvMmaRegReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_REG_REQ_STRU               *pstMmaRegReqMsg = VOS_NULL_PTR;
    MMC_MMA_REG_CNF_STRU                stSndMsg;
    VOS_UINT8                           ucCtxIndex;
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulAttachType;

    pstMmaRegReqMsg = (TAF_MMA_REG_REQ_STRU*)pstMsg;
    PS_MEM_SET(&stSndMsg, 0, sizeof(MMC_MMA_REG_CNF_STRU));

    /* ����ǹػ�״̬�����������ڿ��ػ�����ظ��ܾ� */
    if ((TAF_MMA_FSM_PHONE_MODE == TAF_MMA_GetCurrFsmId())
     || (STA_FSM_NULL           == g_StatusContext.ulFsmState)
     || (STA_FSM_STOP           == g_StatusContext.ulFsmState))
    {
        TAF_MMA_SndRegCnf(&(pstMmaRegReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_REJECT,
                          &stSndMsg);
        return VOS_TRUE;
    }

    /* �������CLģʽ������CMMCA��Ϣ�����ϱ�CMMCA�ܾ� */
    if ((VOS_FALSE              == TAF_SDC_IsConfigCLInterWork())
     && (CMMCA_CLIENT_ID        == pstMmaRegReqMsg->stCtrl.usClientId))
    {
        TAF_MMA_SndRegCnf(&(pstMmaRegReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_REJECT,
                          &stSndMsg);
        return VOS_TRUE;
    }

    /* �������Ч��ֱ�ӻ�ʧ�� */
    if (VOS_FALSE == TAF_SDC_IsUsimStausValid())
    {
        TAF_MMA_SndRegCnf(&(pstMmaRegReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE,
                          &stSndMsg);
        return VOS_TRUE;
    }

    /* ���g_stTafMmaCtx.astMmaOperCtx��TAF_MMA_OPER_REG_REQ���ͽṹ���ڱ�ʹ�� */
    if (VOS_TRUE == TAF_MMA_IsOperTypeUsed(TAF_MMA_OPER_REG_REQ))
    {
        TAF_MMA_SndRegCnf(&(pstMmaRegReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE,
                          &stSndMsg);
        return VOS_TRUE;
    }

    /* �Ҳ���δ��ʹ�õģ��ظ�ʧ��  */
    if (VOS_FALSE == TAF_MMA_GetUnusedOperCtxIndex(&ucCtxIndex))
    {
        TAF_MMA_SndRegCnf(&(pstMmaRegReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE,
                          &stSndMsg);
        return VOS_TRUE;
    }

    /* ����һ��g_stTafMmaCtx.astMmaOperCtx����Ӧ���� */
    TAF_MMA_SetOperCtx(pstMmaRegReqMsg->stCtrl,
                       TAF_MMA_OPER_REG_REQ, ucCtxIndex);

    /* ��MMC��reg req��Ϣ */
    ulRet = TAF_MMA_SndMmcRegReq(&(pstMmaRegReqMsg->stRegPara));
    if (VOS_OK != ulRet)
    {
        TAF_MMA_ClearOperCtx(ucCtxIndex);
        return VOS_TRUE;
    }

    ulAttachType = TAF_MMA_ConvertAttachDomainToAttachType(pstMmaRegReqMsg->stRegPara.ucAttachDomain);

    /* ����attach */
    ulRet = Sta_Attach(STA_OPID_ATTATCH,
                       ulAttachType,
                       pstMmaRegReqMsg->stRegPara.enEpsAttachReason);

    if (STA_ERROR == ulRet)
    {
        MMA_WARNINGLOG("TAF_MMA_RcvMmaRegReq_PreProc:Sta Attach fail!");
    }

    /* ����������ʱ��TI_TAF_MMA_WAIT_MMC_REG_CNF */
    TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_MMC_REG_CNF, TI_TAF_MMA_WAIT_MMC_REG_CNF_LEN);

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvMmaPowerSaveReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_POWER_SAVE_REQ_STRU        *pstMmaPowerSaveReqMsg  = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;
    VOS_UINT32                          ulRet;

    pstMmaPowerSaveReqMsg  = (TAF_MMA_POWER_SAVE_REQ_STRU*)pstMsg;

    /* ����ǿ��ػ������У�ֱ�ӻظ��ܾ� */
    if (TAF_MMA_FSM_PHONE_MODE == TAF_MMA_GetCurrFsmId())
    {
        TAF_MMA_SndPowerSaveCnf(&(pstMmaPowerSaveReqMsg->stCtrl),
                                TAF_MMA_APP_OPER_RESULT_REJECT);
        return VOS_TRUE;
    }

    /* �������CLģʽ������CMMCA��Ϣ�����ϱ�CMMCA�ܾ� */
    if ((VOS_FALSE             == TAF_SDC_IsConfigCLInterWork())
     && (CMMCA_CLIENT_ID       == pstMmaPowerSaveReqMsg->stCtrl.usClientId))
    {
        TAF_MMA_SndPowerSaveCnf(&(pstMmaPowerSaveReqMsg->stCtrl),
                                TAF_MMA_APP_OPER_RESULT_REJECT);
        return VOS_TRUE;
    }

    /* ����Ѿ��ػ���ֱ�ӻظ��ɹ� */
    if ((STA_FSM_NULL    == g_StatusContext.ulFsmState)
     || (STA_FSM_STOP    == g_StatusContext.ulFsmState))
    {
        TAF_MMA_SndPowerSaveCnf(&(pstMmaPowerSaveReqMsg->stCtrl),
                                TAF_MMA_APP_OPER_RESULT_SUCCESS);
        return VOS_TRUE;
    }

    /* ���g_stTafMmaCtx.astMmaOperCtx��TAF_MMA_OPER_POWER_SAVE_REQ���ͽṹ���ڱ�ʹ�� */
    if (VOS_TRUE == TAF_MMA_IsOperTypeUsed(TAF_MMA_OPER_POWER_SAVE_REQ))
    {
        TAF_MMA_SndPowerSaveCnf(&(pstMmaPowerSaveReqMsg->stCtrl),
                                TAF_MMA_APP_OPER_RESULT_FAILURE);
        return VOS_TRUE;
    }

    /* �Ҳ���δ��ʹ�õģ��ظ�ʧ��  */
    if (VOS_FALSE == TAF_MMA_GetUnusedOperCtxIndex(&ucCtxIndex))
    {
        TAF_MMA_SndPowerSaveCnf(&(pstMmaPowerSaveReqMsg->stCtrl),
                                TAF_MMA_APP_OPER_RESULT_FAILURE);
        return VOS_TRUE;
    }

    /* ����һ��g_stTafMmaCtx.astMmaOperCtx����Ӧ���� */
    TAF_MMA_SetOperCtx(pstMmaPowerSaveReqMsg->stCtrl,
                       TAF_MMA_OPER_POWER_SAVE_REQ, ucCtxIndex);

    /* ��MMC��power save req��Ϣ */
    ulRet = TAF_MMA_SndMmcPowerSaveReq(&(pstMmaPowerSaveReqMsg->stPowerSavePara));
    if (VOS_OK != ulRet)
    {
        return VOS_TRUE;
    }

    /* ����������ʱ��TI_TAF_MMA_WAIT_MMC_POWER_SAVE_CNF */
    TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_MMC_POWER_SAVE_CNF,
                       TI_TAF_MMA_WAIT_MMC_POWER_SAVE_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvMmcAcqCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MMA_ACQ_CNF_STRU               *pstMmcMmaAcqCnf = VOS_NULL_PTR;
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;
    TAF_MMA_APP_OPER_RESULT_ENUM_UINT32 enRslt;

    pstMmcMmaAcqCnf = (MMC_MMA_ACQ_CNF_STRU *)pstMsg;
    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();
    enRslt          = TAF_MMA_APP_OPER_RESULT_SUCCESS;

    if (MMC_MMA_ACQ_RESULT_SUCCESS != pstMmcMmaAcqCnf->enAcqRslt)
    {
        enRslt = TAF_MMA_APP_OPER_RESULT_FAILURE;
    }

    /* ��ȫ�ֱ���g_stTafMmaCtx.astMmaOperCtx����TAF_MMA_OPER_ACQ_REQ��Ӧ�ļ�¼�� */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_ACQ_REQ, &ucCtxIndex))
    {
        /*  �ظ�MMA_ACQ_CNF */
        TAF_MMA_SndAcqBestNetworkCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                          enRslt,
                          pstMmcMmaAcqCnf);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    /* ֹͣ������ʱ�� */
    TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_MMC_ACQ_CNF);

    return VOS_TRUE;
}

#if (FEATURE_ON == FEATURE_CL_INTERWORK)
VOS_UINT32 TAF_MMA_RcvMmcAcqInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MMA_ACQ_IND_STRU               *pstMmcMmaAcqInd = VOS_NULL_PTR;
    TAF_MMA_APP_OPER_RESULT_ENUM_UINT32 enRslt;

    pstMmcMmaAcqInd = (MMC_MMA_ACQ_IND_STRU *)pstMsg;
    enRslt          = TAF_MMA_APP_OPER_RESULT_SUCCESS;

    if (MMC_MMA_ACQ_RESULT_SUCCESS != pstMmcMmaAcqInd->enAcqRslt)
    {
        enRslt = TAF_MMA_APP_OPER_RESULT_FAILURE;
    }

    /* �ظ�MMA_ACQ_IND */
    TAF_MMA_SndAcqInd(enRslt, pstMmcMmaAcqInd);

    return VOS_TRUE;
}
#endif
VOS_UINT32 TAF_MMA_RcvMmcRegCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MMA_REG_CNF_STRU               *pstMmcMmaRegCnf = VOS_NULL_PTR;
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;
    TAF_MMA_APP_OPER_RESULT_ENUM_UINT32 enRslt;

    pstMmcMmaRegCnf = (MMC_MMA_REG_CNF_STRU *)pstMsg;
    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

    switch (pstMmcMmaRegCnf->enRegRslt)
    {
        case MMC_MMA_REG_RESULT_SUCCESS:
            enRslt = TAF_MMA_APP_OPER_RESULT_SUCCESS;
            break;

        case MMC_MMA_REG_RESULT_ACQ_FAILURE:
            enRslt = TAF_MMA_APP_OPER_RESULT_ACQUIRED_FAIL;
            break;

        case MMC_MMA_REG_RESULT_REG_FAILURE:
            enRslt = TAF_MMA_APP_OPER_RESULT_FAILURE;
            break;

        default:
            enRslt = TAF_MMA_APP_OPER_RESULT_ACQUIRED_FAIL;
            break;
    }

    /* ��ȫ�ֱ���g_stTafMmaCtx.astMmaOperCtx����TAF_MMA_OPER_REG_REQ��Ӧ�ļ�¼�� */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_REG_REQ, &ucCtxIndex))
    {

        /*  �ظ�MMA_REG_CNF */
        TAF_MMA_SndRegCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                          enRslt,
                          pstMmcMmaRegCnf);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    /* ֹͣ������ʱ�� */
    TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_MMC_REG_CNF);

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvMmcPowerSaveCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    TAF_MMA_CTRL_STRU                   stCtrlInfo;
    VOS_UINT8                           ucCtxIndex;

    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

    /* ��ȫ�ֱ���g_stTafMmaCtx.astMmaOperCtx����TAF_MMA_OPER_ACQ_REQ��Ӧ�ļ�¼�� */
    if (VOS_TRUE != TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_POWER_SAVE_REQ, &ucCtxIndex))
    {
        MMA_WARNINGLOG("TAF_MMA_RcvMmcPowerSaveCnf_PreProc:NOT FIND TAF_MMA_OPER_POWER_SAVE_REQ!");

        TAF_MMA_BuildMmaCtrlInfo(WUEPS_PID_CMMCA, CMMCA_CLIENT_ID, 0, &stCtrlInfo);
        TAF_MMA_SndPowerSaveCnf(&stCtrlInfo,
                                TAF_MMA_APP_OPER_RESULT_SUCCESS);

        return VOS_TRUE;
    }

    /* �ظ�MMA_POWER_SAVE_CNF */
    TAF_MMA_SndPowerSaveCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                            TAF_MMA_APP_OPER_RESULT_SUCCESS);

    TAF_MMA_ClearOperCtx(ucCtxIndex);

    /* ֹͣ������ʱ�� */
    TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_MMC_POWER_SAVE_CNF);

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvTiWaitMmcAcqCnfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    MMC_MMA_ACQ_CNF_STRU                stSndMsg;
    VOS_UINT8                           ucCtxIndex;

    PS_MEM_SET(&stSndMsg, 0, sizeof(MMC_MMA_ACQ_CNF_STRU));
    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

    stSndMsg.stPlmnId.ulMcc = MMA_INVALID_MCC;
    stSndMsg.stPlmnId.ulMnc = MMA_INVALID_MNC;

    /* ��ȫ�ֱ���g_stTafMmaCtx.astMmaOperCtx����TAF_MMA_OPER_ACQ_REQ��Ӧ�ļ�¼�� */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_ACQ_REQ, &ucCtxIndex))
    {
        /*  �ظ�MMA_ACQ_CNF */
        TAF_MMA_SndAcqBestNetworkCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE,
                          &stSndMsg);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    return VOS_TRUE;

}


VOS_UINT32 TAF_MMA_RcvTiWaitMmcRegCnfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    MMC_MMA_REG_CNF_STRU                stSndMsg;
    VOS_UINT8                           ucCtxIndex;

    PS_MEM_SET(&stSndMsg, 0, sizeof(MMC_MMA_REG_CNF_STRU));
    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

    stSndMsg.stPlmnId.ulMcc = MMA_INVALID_MCC;
    stSndMsg.stPlmnId.ulMnc = MMA_INVALID_MNC;

    /* ��ȫ�ֱ���g_stTafMmaCtx.astMmaOperCtx����TAF_MMA_OPER_REG_REQ��Ӧ�ļ�¼�� */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_REG_REQ, &ucCtxIndex))
    {
        /*  �ظ�MMA_REG_CNF */
        TAF_MMA_SndRegCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                          TAF_MMA_APP_OPER_RESULT_ACQUIRED_FAIL,
                          &stSndMsg);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    return VOS_TRUE;

}


VOS_UINT32 TAF_MMA_RcvTiWaitMmcPowerSaveExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

    /* ��ȫ�ֱ���g_stTafMmaCtx.astMmaOperCtx����TAF_MMA_OPER_POWER_SAVE_REQ��Ӧ�ļ�¼�� */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_POWER_SAVE_REQ, &ucCtxIndex))
    {
        /*  �ظ�MMA_POWER_SAVE_CNF */
        TAF_MMA_SndPowerSaveCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    return VOS_TRUE;

}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


