/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : NasMmcFsmTbl.c
  �� �� ��   : ����
  ��    ��   : zhoujun /40661
  ��������   : 2010��11��12��
  ����޸�   :
  ��������   : NAS ��״̬��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2010��11��12��
    ��    ��   : zhoujun /40661
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "pslog.h"
#include "om.h"
#include "NasFsm.h"
#include "NasMmcTimerMgmt.h"
#include "MmaMmcInterface.h"

#include "NasMmcPreProcAct.h"
#include "UsimPsInterface.h"
#include "MmcGmmInterface.H"
#include "MmcMmInterface.h"
#if (FEATURE_ON == FEATURE_LTE)
#include "MmcLmmInterface.h"
#endif
#include "NasOmInterface.h"
#include "NasOmTrans.h"
#include "NasMmcCtx.h"
/* ɾ��ExtAppMmcInterface.h*/
#include "NasMmcSndOm.h"
#include "NasMmcSndInternalMsg.h"
#include "NasMmcFsmMainTbl.h"
#include  "siappstk.h"

#if (FEATURE_ON == FEATURE_PTM)
#include "NasErrorLog.h"
#endif


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_NAS_MMC_PREPROCTBL_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/* Ԥ����״̬�� */
NAS_FSM_DESC_STRU                       g_stNasMmcPreFsmDesc;


/*����״̬��������� */

/* ����״̬���������Ϣ ������ */
NAS_ACT_STRU        g_astNasMmcPreProcessActTbl[]   =
{
    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_ATTACH_REQ,
                      NAS_MMC_RcvMmaAttachReq_PreProc),

    NAS_ACT_TBL_ITEM( MAPS_PIH_PID,
                      PS_USIM_REFRESH_IND,
                      NAS_MMC_RcvUsimRefreshFileInd_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_UPDATE_UPLMN_NTF,
                      NAS_MMC_RcvMmaUpdateUplmnNtf_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_DETACH_REQ,
                      NAS_MMC_RcvMmaDetachReq_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_MODE_CHANGE_REQ,
                      NAS_MMC_RcvMmaModeChange_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_PLMN_SPECIAL_REQ,
                      NAS_MMC_RcvTafUserSpecPlmnSearch_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_PLMN_USER_RESEL_REQ,
                      NAS_MMC_RcvUserReselReq_PreProc),

    NAS_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_NAS_MMC_AVAILABLE_TIMER,
                      NAS_MMC_RcvTiAvailTimerExpired_PreProc),

    NAS_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_NAS_MMC_PERIOD_TRYING_USER_PLMN_LIST,
                      NAS_MMC_RcvTiPeriodTryingUserPlmnListExpired_PreProc),

    NAS_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_NAS_MMC_PERIOD_TRYING_INTER_PLMN_LIST,
                      NAS_MMC_RcvTiPeriodTryingInterPlmnListExpired_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_PLMN_LIST_REQ,
                      NAS_MMC_RcvTafPlmnListReq_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MMC,
                      MMCMMC_INTER_PLMN_LIST_REQ,
                      NAS_MMC_RcvMmcInterPlmnListReq_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_PLMN_LIST_ABORT_REQ,
                      NAS_MMC_RcvTafPlmnListAbortReq_PlmnList_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_SPEC_PLMN_SEARCH_ABORT_REQ,
                      NAS_MMC_RcvTafSpecPlmnSearchAbortReq_PreProc),


    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_EOPLMN_SET_REQ,
                      NAS_MMC_RcvTafEOPlmnSetReq_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MMC,
                      MMCMMC_INTER_NVIM_OPLMN_REFRESH_IND,
                      NAS_MMC_RcvMmcInterNvimOPlmnRefreshInd_PreProc),

    NAS_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_NAS_MMC_HPLMN_TIMER,
                      NAS_MMC_RcvTiHplmnTimerExpired_PreProc),

    NAS_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_NAS_MMC_PERIOD_TRYING_HIGH_PRIO_PLMN_SEARCH,
                      NAS_MMC_RcvTiTryingHighPrioPlmnSearchExpired_PreProc),

    NAS_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_NAS_MMC_HIGH_PRIO_RAT_HPLMN_TIMER,
                      NAS_MMC_RcvTiHighPrioRatHplmnSrchTimerExpired_PreProc),


    NAS_ACT_TBL_ITEM( MAPS_STK_PID,
                      STK_NAS_STEERING_OF_ROAMING_IND,
                      NAS_MMC_RcvStkSteerRoamingInd_PreProc),


    NAS_ACT_TBL_ITEM( WUEPS_PID_GMM,
                      MMCGMM_LOCAL_DETACH_IND,
                      NAS_MMC_RcvGmmLocalDetachInd_PreProc),

#if   (FEATURE_ON == FEATURE_LTE)

    NAS_ACT_TBL_ITEM( WUEPS_PID_GMM,
                      MMCGMM_TIN_TYPE_IND,
                      NAS_MMC_RcvGmmTinInd_PreProc ),
#endif

    NAS_ACT_TBL_ITEM( WUEPS_PID_GMM,
                      GMMMMC_NETWORK_CAPABILITY_INFO_IND,
                      NAS_MMC_RcvGmmNetworkCapabilityInfoInd_PreProc ),


    NAS_ACT_TBL_ITEM( WUEPS_PID_GMM,
                      MMCGMM_INFO_IND,
                      NAS_MMC_RcvGmmInfo_PreProc ),

    NAS_ACT_TBL_ITEM( WUEPS_PID_GMM,
                      MMCGMM_PDP_STATUS_IND,
                      NAS_MMC_RcvGmmPdpStatusInd_PreProc ),


    NAS_ACT_TBL_ITEM( WUEPS_PID_GMM,
                      MMCGMM_SERVICE_STATUS_IND,
                      NAS_MMC_RcvGmmServiceStatusInd_PreProc ),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MM,
                      MMCMM_INFO_IND,
                      NAS_MMC_RcvMmInfo_PreProc ),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MM,
                      MMCMM_SERVICE_STATUS_IND,
                      NAS_MMC_RcvMmServiceStatusInd_PreProc ),


    NAS_ACT_TBL_ITEM( WUEPS_PID_MM,
                      MMCMM_ATTACH_CNF,
                      NAS_MMC_RcvMmAttachCnf_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_GMM,
                      MMCGMM_ATTACH_CNF,
                      NAS_MMC_RcvGmmAttachCnf_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MM,
                      MMCMM_DETACH_CNF,
                      NAS_MMC_RcvMmDetachCnf_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_GMM,
                      MMCGMM_DETACH_CNF,
                      NAS_MMC_RcvGmmDetachCnf_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_GMM,
                      GMMMMC_PS_REG_RESULT_IND,
                      NAS_MMC_RcvGmmPsRegResultInd_PreProc),

    NAS_ACT_TBL_ITEM( UEPS_PID_GAS,
                      GRRMM_SCELL_MEAS_IND,
                      NAS_MMC_RcvGasScellRxInd_PreProc ),

    NAS_ACT_TBL_ITEM( WUEPS_PID_WRR,
                      RRMM_AT_MSG_IND,
                      NAS_MMC_RcvWasAtMsgInd_PreProc ),
    NAS_ACT_TBL_ITEM( WUEPS_PID_WRR,
                      RRMM_W_AC_INFO_CHANGE_IND,
                      NAS_MMC_RcvWasAcInfoChange_PreProc ),
    NAS_ACT_TBL_ITEM( WUEPS_PID_WRR,
                      RRMM_AT_MSG_CNF,
                      NAS_MMC_RcvWasAtMsgCnf_PreProc ),

    NAS_ACT_TBL_ITEM( WUEPS_PID_WRR,
                      RRMM_PLMN_QUERY_REQ,
                      NAS_MMC_RcvRrMmPlmnQryReqPreProc ),

    NAS_ACT_TBL_ITEM( UEPS_PID_GAS,
                      RRMM_NOT_CAMP_ON_IND,
                      NAS_MMC_RcvRrMmNotCampOn_PreProc ),

    NAS_ACT_TBL_ITEM( WUEPS_PID_WRR,
                      RRMM_NOT_CAMP_ON_IND,
                      NAS_MMC_RcvRrMmNotCampOn_PreProc ),

    NAS_ACT_TBL_ITEM( WUEPS_PID_WRR,
                      RRMM_EPLMN_QUERY_REQ,
                      NAS_MMC_RcvRrMmEquplmnQuery_PreProc ),

    NAS_ACT_TBL_ITEM( UEPS_PID_GAS,
                      RRMM_EPLMN_QUERY_REQ,
                      NAS_MMC_RcvRrMmEquplmnQuery_PreProc ),

    NAS_ACT_TBL_ITEM( WUEPS_PID_WRR,
                      RRMM_LIMIT_SERVICE_CAMP_IND,
                      NAS_MMC_RcvRrMmLimitServiceCamp_PreProc ),


    NAS_ACT_TBL_ITEM( WUEPS_PID_WRR,
                      RRMM_SUSPEND_CNF,
                      NAS_MMC_RcvRrMmSuspendCnf_PreProc ),

    NAS_ACT_TBL_ITEM( UEPS_PID_GAS,
                      RRMM_SUSPEND_CNF,
                      NAS_MMC_RcvRrMmSuspendCnf_PreProc ),



    NAS_ACT_TBL_ITEM( WUEPS_PID_WRR,
                      RRMM_TRANSACTION_INFO_ENQ,
                      NAS_MMC_RcvRrMmGetTransactionReq_PreProc ),

    NAS_ACT_TBL_ITEM( UEPS_PID_GAS,
                      RRMM_TRANSACTION_INFO_ENQ,
                      NAS_MMC_RcvRrMmGetTransactionReq_PreProc ),



    NAS_ACT_TBL_ITEM( WUEPS_PID_WRR,
                      RRMM_REL_IND,
                      NAS_MMC_RcvRrMmRelInd_PreProc ),



#if   (FEATURE_ON == FEATURE_LTE)

    NAS_ACT_TBL_ITEM( PS_PID_MM,
                      ID_LMM_MMC_TIN_TYPE_IND,
                      NAS_MMC_RcvLmmTinInd_PreProc ),

    NAS_ACT_TBL_ITEM( PS_PID_MM,
                      ID_LMM_MMC_DETACH_CNF,
                      NAS_MMC_RcvLmmDetachCnf_PreProc ),

    NAS_ACT_TBL_ITEM( PS_PID_MM,
                      ID_LMM_MMC_ATTACH_CNF,
                      NAS_MMC_RcvLmmAttachCnf_PreProc ),

    /* Lģ�ϱ�����Ĵ��� */
    NAS_ACT_TBL_ITEM( PS_PID_MM,
                      ID_LMM_MMC_ERR_IND,
                      NAS_MMC_RcvLmmErrInd_PreProc),

     NAS_ACT_TBL_ITEM( PS_PID_MM,
                       ID_LMM_MMC_REGISTER_STATUS_IND,
                       NAS_MMC_RcvLmmRegStatusChangeInd_PreProc),

    NAS_ACT_TBL_ITEM( PS_PID_MM,
                       ID_LMM_MMC_SERVICE_STATUS_IND,
                       NAS_MMC_RcvLmmServiceStatusInd_PreProc),



    /* LMM�ı���detach��Ԥ���� */
    NAS_ACT_TBL_ITEM( PS_PID_MM,
                      ID_LMM_MMC_DETACH_IND,
                      NAS_MMC_RcvLmmMmcDetachInd_PreProc),

    /* LMM�Ĺ���ظ���Ԥ���� */
    NAS_ACT_TBL_ITEM( PS_PID_MM,
                      ID_LMM_MMC_SUSPEND_CNF,
                      NAS_MMC_RcvLmmSuspendCnf_PreProc),

    NAS_ACT_TBL_ITEM( PS_PID_MM,
                      ID_LMM_MMC_STATUS_IND,
                      NAS_MMC_RcvLmmMmcStatusInd_PreProc),


    /* �յ�LMM T3412��T3423��ʱ������״̬��Ϣ��Ԥ���� */
    NAS_ACT_TBL_ITEM( PS_PID_MM,
                      ID_LMM_MMC_TIMER_STATE_NOTIFY,
                      NAS_MMC_RcvLmmTimerStateNotify_PreProc),

    /* ����LMM��ת���פ��̬����Ϣ���� */
    NAS_ACT_TBL_ITEM( PS_PID_MM,
                      ID_LMM_MMC_NOT_CAMP_ON_IND,
                      NAS_MMC_RcvLmmMmcNotCampOnInd_PreProc),

    NAS_ACT_TBL_ITEM( PS_PID_MM,
                      ID_LMM_MMC_SERVICE_RESULT_IND,
                      NAS_MMC_RcvLmmServiceRsltInd_PreProc),

    NAS_ACT_TBL_ITEM( PS_PID_MM,
                      ID_LMM_MMC_EMM_INFO_IND,
                      NAS_MMC_RcvLmmEmmInfoInd_PreProc),



    NAS_ACT_TBL_ITEM( PS_PID_MM,
                      ID_LMM_MMC_EMC_PDP_STATUS_NOTIFY,
                      NAS_MMC_RcvLmmEmcPdpStatusNotify_PreProc),

#endif

    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_SYS_CFG_SET_REQ,
                      NAS_MMC_RcvTafSysCfgReq_PreProc),


    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_ACQ_REQ,
                      NAS_MMC_RcvTafAcqReq_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_REG_REQ,
                      NAS_MMC_RcvTafRegReq_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_OM,
                      OM_NAS_OTA_REQ,
                      NAS_MMC_RcvOmOtaReq_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_OM,
                      ID_NAS_OM_MM_INQUIRE,
                      NAS_MMC_RcvOmInquireReq_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_OM,
                      ID_NAS_OM_CONFIG_TIMER_REPORT_REQ,
                      NAS_MMC_RcvOmConfigTimerReportReq_PreProc),

#ifdef __PS_WIN32_RECUR__
    NAS_ACT_TBL_ITEM( WUEPS_PID_MMC,
                      MMCOM_OUTSIDE_RUNNING_CONTEXT_FOR_PC_REPLAY,
                      NAS_MMC_RestoreContextData_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MMC,
                      MMCOM_FIXED_PART_CONTEXT,
                      NAS_MMC_RestoreFixedContextData_PreProc),

#endif

    NAS_ACT_TBL_ITEM( WUEPS_PID_GMM,
                      GMMMMC_PS_REG_RESULT_IND,
                      NAS_MMC_RcvGmmPsRegResultInd_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_WRR,
                      RRMM_CIPHER_INFO_IND,
                      NAS_MMC_RcvRrMmCipherInfoInd_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_GMM,
                      GMMMMC_CIPHER_INFO_IND,
                      NAS_MMC_RcvGmmCipherInfoInd_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_OM_MAINTAIN_INFO_IND,
                      NAS_MMC_RcvTafOmMaintainInfoInd_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_GMM,
                      MMCGMM_GPRS_SERVICE_IND,
                      NAS_MMC_RcvGmmGprsServiceInd_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_SIGN_REPORT_REQ,
                      NAS_MMC_RcvMmaSignReportReq_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MM,
                      MMCMM_SIM_AUTH_FAIL_IND,
                      NAS_MMC_RcvMmSimAuthFail_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_GMM,
                      MMCGMM_SIM_AUTH_FAIL_IND,
                      NAS_MMC_RcvGmmSimAuthFail_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MM,
                      MMCMM_CM_SERVICE_IND,
                      NAS_MMC_RcvMmCmServiceInd_PreProc),

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_OTHER_MODEM_INFO_NOTIFY,
                      NAS_MMC_RcvMmaOtherModemInfoNotify_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_NCELL_INFO_NOTIFY,
                      NAS_MMC_RcvMmaNcellInfoNotify_PreProc),
    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_PS_TRANSFER_IND,
                      NAS_MMC_RcvMmaPsTransferInd_PreProc),
#endif
#if   (FEATURE_ON == FEATURE_LTE)

    NAS_ACT_TBL_ITEM( WUEPS_PID_MM,
                      MMCMM_CM_SERVICE_REJECT_IND,
                      NAS_MMC_RcvCmServiceRejectInd_PreProc),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MM,
                      MMMMC_ABORT_IND,
                      NAS_MMC_RcvMmAbortInd_PreProc),

    NAS_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER,
                      NAS_MMC_RcvEnableLteExpired_PreProc ),

#endif

#if (FEATURE_ON == FEATURE_PTM)
    NAS_ACT_TBL_ITEM( ACPU_PID_OM,
                      ID_OM_ERR_LOG_REPORT_REQ,
                      NAS_MMC_RcvAcpuOmErrLogRptReq_PreProc),

    NAS_ACT_TBL_ITEM( ACPU_PID_OM,
                      ID_OM_ERR_LOG_CTRL_IND,
                      NAS_MMC_RcvAcpuOmErrLogCtrlInd_PreProc),

    NAS_ACT_TBL_ITEM( ACPU_PID_OM,
                      ID_OM_FTM_CTRL_IND,
                      NAS_MMC_RcvAcpuOmFtmCtrlInd_PreProc),

#endif

    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_NET_SCAN_REQ,
                      NAS_MMC_RcvMmaNetScanReq_PreProc ),

    NAS_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_NAS_MMC_PERIODIC_NET_SCAN_TIMER,
                      NAS_MMC_RcvPeriodicNetScanExpired_PreProc ),

    NAS_ACT_TBL_ITEM( WUEPS_PID_WRR,
                      RRMM_NET_SCAN_CNF,
                      NAS_MMC_RcvRrMmNetScanCnf_PreProc ),

    NAS_ACT_TBL_ITEM( UEPS_PID_GAS,
                      RRMM_NET_SCAN_CNF,
                      NAS_MMC_RcvRrMmNetScanCnf_PreProc ),

    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_ABORT_NET_SCAN_REQ,
                      NAS_MMC_RcvMmaAbortNetScanReq_PreProc ),

    NAS_ACT_TBL_ITEM( WUEPS_PID_WRR,
                      RRMM_NET_SCAN_STOP_CNF,
                      NAS_MMC_RcvRrMmNetScanStopCnf_PreProc ),

    NAS_ACT_TBL_ITEM( UEPS_PID_GAS,
                      RRMM_NET_SCAN_STOP_CNF,
                      NAS_MMC_RcvRrMmNetScanStopCnf_PreProc ),

    NAS_ACT_TBL_ITEM( UEPS_PID_GAS,
                      RRMM_NCELL_MONITOR_IND,
                      NAS_MMC_RcvGasNcellMonitorInd_PreProc),
    NAS_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      ID_MMA_MMC_IMS_VOICE_CAP_IND,
                      NAS_MMC_RcvMmaImsVoiceCapInd_PreProc),

     NAS_ACT_TBL_ITEM( WUEPS_PID_GMM,
                      GMMMMC_SERVICE_REQUEST_RESULT_IND,
                      NAS_MMC_RcvGmmServiceRequestResultInd_PreProc),


    NAS_ACT_TBL_ITEM( WUEPS_PID_WRR,
                      RRMM_SUSPEND_IND,
                      NAS_MMC_RcvRrmmSuspendInd_PreProc),

    NAS_ACT_TBL_ITEM( UEPS_PID_GAS,
                      RRMM_SUSPEND_IND,
                      NAS_MMC_RcvRrmmSuspendInd_PreProc),
                      
    NAS_ACT_TBL_ITEM( WUEPS_PID_WRR,
                      RRMM_RESUME_IND,
                      NAS_MMC_RcvRrmmResumeInd_PreProc),
    
    NAS_ACT_TBL_ITEM( UEPS_PID_GAS,
                      RRMM_RESUME_IND,
                      NAS_MMC_RcvRrmmResumeInd_PreProc),
                      
#if   (FEATURE_ON == FEATURE_LTE)
    
    NAS_ACT_TBL_ITEM( PS_PID_MM,
                      ID_LMM_MMC_SUSPEND_IND,
                      NAS_MMC_RcvLmmSuspendInd_PreProc ),
    
    NAS_ACT_TBL_ITEM( PS_PID_MM,
                      ID_LMM_MMC_RESUME_IND,
                      NAS_MMC_RcvLmmResumeInd_PreProc ),

    NAS_ACT_TBL_ITEM( PS_PID_MM,
                      ID_LMM_MMC_INFO_CHANGE_NOTIFY,
                      NAS_MMC_RcvLmmInfoChangeNotifyInd_PreProc ),

#endif

};

/* ����״̬���������Ϣ ״̬�� */
NAS_STA_STRU        g_astNasMmcPreProcessFsmTbl[]   =
{
    NAS_STA_TBL_ITEM( NAS_MMC_L1_STA_PREPROC,
                      g_astNasMmcPreProcessActTbl )
};

/*****************************************************************************
 �� �� ��  : NAS_MMC_GetPreProcessStaTblSize
 ��������  : ��ȡԤ����״̬���Ĵ�С
 �������  : ��
 �������  : ��
 �� �� ֵ  : VOS_UINT32:Ԥ����״̬���Ĵ�С
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��5��9��
    ��    ��   : zhoujun 40661
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 NAS_MMC_GetPreProcessStaTblSize( VOS_VOID  )
{
    return (sizeof(g_astNasMmcPreProcessFsmTbl)/sizeof(NAS_STA_STRU));
}

/*****************************************************************************
 �� �� ��  : NAS_MMC_GetPreFsmDescAddr
 ��������  : ��ȡԤ����״̬����������
 �������  : ��
 �������  : ��
 �� �� ֵ  : NAS_FSM_DESC_STRU:ָ��Ԥ����״̬����������
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2011��5��9��
    ��    ��   : zhoujun 40661
    �޸�����   : �����ɺ���

*****************************************************************************/
NAS_FSM_DESC_STRU * NAS_MMC_GetPreFsmDescAddr(VOS_VOID)
{
    return (&g_stNasMmcPreFsmDesc);
}




/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
