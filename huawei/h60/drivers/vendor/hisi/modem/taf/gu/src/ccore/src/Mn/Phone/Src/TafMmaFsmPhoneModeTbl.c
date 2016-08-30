

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "pslog.h"
#include "om.h"
#include "UsimPsInterface.h"
#include "PsTypeDef.h"
#include "MnComm.h"
#include "TafFsm.h"
#include "TafMmaFsmPhoneModeTbl.h"
#include "NasComm.h"
#include "TafMmaFsmPhoneMode.h"
#include "MmaMmcInterface.h"
#include "Taf_Tafm_Remote.h"
#include "TafMmaSndInternalMsg.h"
#if (FEATURE_IMS == FEATURE_ON)
#include "ImsaMmaInterface.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_TAF_MMA_FSM_PHONE_MODE_TBL_C


/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
TAF_FSM_DESC_STRU                       g_stTafMmaPhoneModeFsmDesc;

/**************************************************************************/
/*����״̬��������� */
/**************************************************************************/

/* TAF_MMA_PHONE_MODE_STA_INIT ������ */
TAF_ACT_STRU        g_astTafMmaPhoneModeInitActTbl[]   =
{
    TAF_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      MMA_MMA_INTER_POWER_INIT,
                      TAF_MMA_RcvMmaInterPowerInit_PhoneMode_Init ),

    TAF_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      MMA_MMA_INTER_USIM_STATUS_CHANGE_IND,
                      TAF_MMA_RcvMmaInterUsimStatusChangeInd_PhoneMode_Init ),

    TAF_ACT_TBL_ITEM( WUEPS_PID_MMA,
                      MMA_MMA_SIM_LOCK_STATUS_CHANGE_IND,
                      TAF_MMA_RcvMmaInterUsimStatusChangeInd_PhoneMode_Init ),

    TAF_ACT_TBL_ITEM( WUEPS_PID_OM,
                      OAM_MMA_PHONE_MODE_SET_REQ,
                      TAF_MMA_RcvOMPhoneModeSet_PhoneMode_Init ),

    TAF_ACT_TBL_ITEM( WUEPS_PID_TAF,
                      ID_TAF_MMA_PHONE_MODE_SET_REQ,
                      TAF_MMA_RcvTafPhoneModeSet_PhoneMode_Init ),

};

/* TAF_MMA_PHONE_MODE_STA_WAIT_PIH_USIM_STATUS_IND ������ */
TAF_ACT_STRU        g_astTafMmaPhoneModeWaitPihUsimStatusIndActTbl[]   =
{
    TAF_ACT_TBL_ITEM( MAPS_PIH_PID,
                      PS_USIM_GET_STATUS_IND,
                      TAF_MMA_RcvPihUsimStatusInd_PhoneMode_WaitPihUsimStatusInd ),

    TAF_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_TAF_MMA_WAIT_PIH_USIM_STATUS_IND,
                      TAF_MMA_RcvTiWaitPihUsimStatusIndExpired_PhoneMode_WaitPihUsimStatusInd ),
};

#if (FEATURE_IMS == FEATURE_ON)
/* TAF_MMA_PHONE_MODE_STA_WAIT_IMSA_START_CNF ������ */
TAF_ACT_STRU        g_astTafMmaPhoneModeWaitImsaStartCnfActTbl[]   =
{
    TAF_ACT_TBL_ITEM( PS_PID_IMSA,
                      ID_IMSA_MMA_START_CNF,
                      TAF_MMA_RcvImsaStartCnf_PhoneMode_WaitImsaStartCnf ),

    TAF_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_TAF_MMA_WAIT_IMSA_START_CNF,
                      TAF_MMA_RcvTiWaitImsaStartCnfExpired_PhoneMode_WaitImsaStartCnf ),

};

/* TAF_MMA_PHONE_MODE_STA_WAIT_IMSA_POWER_OFF_CNF ������ */
TAF_ACT_STRU        g_astTafMmaPhoneModeWaitImsaPowerOffCnfActTbl[]   =
{
    TAF_ACT_TBL_ITEM( PS_PID_IMSA,
                      ID_IMSA_MMA_STOP_CNF,
                      TAF_MMA_RcvImsaPowerOffCnf_PhoneMode_WaitImsaPowerOffCnf ),

    TAF_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_TAF_MMA_WAIT_IMSA_POWER_OFF_CNF,
                      TAF_MMA_RcvTiWaitImsaPowerOffCnfExpired_PhoneMode_WaitImsaPowerOffCnf ),

};
#endif

/* TAF_MMA_PHONE_MODE_STA_WAIT_MMC_START_CNF ������ */
TAF_ACT_STRU        g_astTafMmaPhoneModeWaitMmcStartCnfActTbl[]   =
{
    TAF_ACT_TBL_ITEM( WUEPS_PID_MMC,
                      ID_MMC_MMA_START_CNF,
                      TAF_MMA_RcvMmcStartCnf_PhoneMode_WaitMmcStartCnf ),

    TAF_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_TAF_MMA_WAIT_MMC_START_CNF,
                      TAF_MMA_RcvTiWaitMmcStartCnfExpired_PhoneMode_WaitMmcStartCnf ),

};

/* TAF_MMA_PHONE_MODE_STA_WAIT_MMC_POWER_OFF_CNF ������ */
TAF_ACT_STRU        g_astTafMmaPhoneModeWaitMmcPowerOffCnfActTbl[]   =
{
    TAF_ACT_TBL_ITEM( WUEPS_PID_MMC,
                      ID_MMC_MMA_POWER_OFF_CNF,
                      TAF_MMA_RcvMmcPowerOffCnf_PhoneMode_WaitMmcPowerOffCnf ),

    TAF_ACT_TBL_ITEM( VOS_PID_TIMER,
                      TI_TAF_MMA_WAIT_MMC_POWER_OFF_CNF,
                      TAF_MMA_RcvTiWaitMmcPowerOffCnfExpired_PhoneMode_WaitMmcPowerOffCnf ),

};



/* �ֻ�ģʽ״̬���������Ϣ ״̬�� */
TAF_STA_STRU        g_astTafMmaPhoneModeStaTbl[]   =
{
    /* �ֻ�ģʽ״̬���ĳ�ʼ��״̬ */
    TAF_STA_TBL_ITEM( TAF_MMA_PHONE_MODE_STA_INIT,
                       g_astTafMmaPhoneModeInitActTbl ),

    /* �ȴ�USIM��״ָ̬ʾ��״̬ */
    TAF_STA_TBL_ITEM( TAF_MMA_PHONE_MODE_STA_WAIT_PIH_USIM_STATUS_IND,
                      g_astTafMmaPhoneModeWaitPihUsimStatusIndActTbl ),

#if (FEATURE_LTE == FEATURE_ON) && (FEATURE_IMS == FEATURE_ON)

    /* �ȴ�IMSA�Ŀ����ظ�״̬ */
    TAF_STA_TBL_ITEM( TAF_MMA_PHONE_MODE_STA_WAIT_IMSA_START_CNF,
                      g_astTafMmaPhoneModeWaitImsaStartCnfActTbl ),

    /* �ȴ�IMSA�Ĺػ��ظ�״̬ */
    TAF_STA_TBL_ITEM( TAF_MMA_PHONE_MODE_STA_WAIT_IMSA_POWER_OFF_CNF,
                      g_astTafMmaPhoneModeWaitImsaPowerOffCnfActTbl ),

#endif

    /* �ȴ�MMC�Ŀ����ظ�״̬ */
    TAF_STA_TBL_ITEM( TAF_MMA_PHONE_MODE_STA_WAIT_MMC_START_CNF,
                      g_astTafMmaPhoneModeWaitMmcStartCnfActTbl ),


    /* �ȴ�MMC�Ĺػ��ظ�״̬ */
    TAF_STA_TBL_ITEM( TAF_MMA_PHONE_MODE_STA_WAIT_MMC_POWER_OFF_CNF,
                      g_astTafMmaPhoneModeWaitMmcPowerOffCnfActTbl ),
};


VOS_UINT32 TAF_MMA_GetPhoneModeStaTblSize(VOS_VOID)
{
    return (sizeof(g_astTafMmaPhoneModeStaTbl)/sizeof(TAF_STA_STRU));
}


TAF_FSM_DESC_STRU * TAF_MMA_GetPhoneModeFsmDescAddr(VOS_VOID)
{
    return (&g_stTafMmaPhoneModeFsmDesc);
}






#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif



