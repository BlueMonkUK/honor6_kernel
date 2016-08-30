

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "pslog.h"
#include "om.h"
#include "PsTypeDef.h"
#include "NasMmcFsmPoweroffTbl.h"
#include "NasMmcTimerMgmt.h"
#include "NasMmcMain.h"
#if (FEATURE_ON == FEATURE_LTE)
#include "MmcLmmInterface.h"
#endif
#include "NasMmcSndLmm.h"
#include "NasMmcSndInternalMsg.h"
#include "NasMmcCtx.h"
#include "NasMmlCtx.h"
#include "NasMmcSndMm.h"
#include "NasMmcSndMma.h"
#include "NasMmcSndGmm.h"
#include "NasMmcSndGuAs.h"
#include "NasMmcProcNvim.h"
#include "NasComm.h"
#include "NasMmcFsmPowerOff.h"
#include "NasMmlLib.h"
#include "NasMmcComFunc.h"

#include "PsRrmInterface.h"

#include "NasUtranCtrlInterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_NAS_MMCFSMPOWEROFF_C

/*****************************************************************************
  2 ȫ�ֱ���
*****************************************************************************/


/*****************************************************************************
  3 �궨��
*****************************************************************************/



VOS_UINT32 NAS_MMC_RcvTafPowerOffReq_PowerOff_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
#if   (FEATURE_ON == FEATURE_LTE)
    MMA_MMC_POWER_OFF_REQ_STRU          *pstPowerOffMsg = VOS_NULL_PTR;
#endif
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enMasterModeRat;

#if   (FEATURE_ON == FEATURE_LTE)
    pstPowerOffMsg = (MMA_MMC_POWER_OFF_REQ_STRU*)pstMsg;
#endif

    /* ���������Ϣ */
    NAS_MMC_SaveCurEntryMsg(ulEventType, pstMsg);

    /* ֹͣ���ж�ʱ�� */
    NAS_MMC_StopAllTimer();

#if (FEATURE_ON == FEATURE_DSDS)

    /* ��ػ�ʱ����Ҫ����ȥע�� */
    NAS_MMC_ProcRrmDeRegister_PowerOff();
#endif


    /* ���õ�ǰ����ģ������ */
    NAS_MMC_SetMasterModeRat_PowerOff( NAS_MML_GetCurrNetRatType() );
    /* ��ȡ��ǰ��ģ */
    enMasterModeRat = NAS_MMC_GetMasterModeRat_PowerOff();

    /* ��ģȷ���Ժ󣬹����ģ�б��Ա�������ģ���ػ�����ʱʹ�� */
    NAS_MMC_BuildSlaveModeRatList_PowerOff(enMasterModeRat);

#if   (FEATURE_ON == FEATURE_LTE)

    /* ��������Ĺػ����� */
    if ( ( NAS_MML_NET_RAT_TYPE_LTE         ==  enMasterModeRat)
      && ( MMA_MMC_POWER_OFF_CAUSE_SIM_REMOVE ==  pstPowerOffMsg->enCause) )
    {

        /* ֪ͨLMM��״̬�����仯 */
        NAS_MMC_SndLmmUsimStatusReq(MMC_LMM_USIM_INVALID);

        /* Ǩ�Ƶ� NAS_MMC_L2_STA_POWER_OFF:NAS_MMC_POWER_OFF_STA_WAIT_LMM_USIM_STATUS_CNF״̬,����������ʱ��*/
        NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_LMM_USIM_STATUS_CNF);

        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_USIM_STATUS_CNF,TI_NAS_MMC_WAIT_LMM_USIM_STATUS_CNF_LEN);

        return VOS_TRUE;
    }


    if ( NAS_MML_NET_RAT_TYPE_LTE == enMasterModeRat )
    {

        /* ��LTE���͹ػ���Ϣ */
        NAS_MMC_SndLmmStopReq();

        /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_MASTER_MODE_POWEROFF_CNF,������������ʱ�� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_MASTER_MODE_POWEROFF_CNF);
        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF_LEN);

        return VOS_TRUE;
    }

#endif

    /* ֪ͨMM�ػ��������õȴ�MM�ظ���־ */
    NAS_MMC_SndMmPowerOffReq();

    NAS_MMC_SetMmPowerOffCnfFlag_PowerOff( NAS_MMC_MM_POWER_OFF_FLG );

    /* ֪ͨGMM�ػ��������õȴ�GMM�ظ���־ */
    NAS_MMC_SndGmmPowerOffReq();

    NAS_MMC_SetMmPowerOffCnfFlag_PowerOff( NAS_MMC_GMM_POWER_OFF_FLG );

    /*****************************************************************
       ��ܳ���:
       PS only�ĳ����£�AT+CFUN=0�ػ�������PS detach
       GMM�����෢��detach���������δ�ظ�detach accept����rel indʱ��
       ��ʱͨ����һ��ͨ���·�AT+CGREG����ע��״̬��ѯ���ػ�ʱ��MMC��MM/
       GMM���͹ػ�����ʱ��MM��GMMǨ״̬��Ǩ��״̬���жϵ�ǰ��Ҫ��MMC
       ����Service Status Indָʾ��ǰCS/PS�����״̬��MMC�յ�MM/GMM����
       ״̬�ϱ��󣬻��MMA�ϱ���ǰ����״̬���÷���״̬��AT+COPS?ʱ��ʹ��
       ������MMC��MM/GMMģ�鷢��POWER OFF REQ֮��AT+COPS?��ѯ���Ϊ
       �޷��񡣵���ʱCS/PSע��״̬���Ǵ���ע�ᣬע��״̬�����յ�MM/GMM
       CNF֮����и��¡������಻�ظ�detach accept����rel ind��GMM�㲻��
       MMC�ϱ�POWER OFF CNF���������ʱ���ڲ�ѯAT+CGREG?��AT+COPS?������
       ����ά��״̬��һ�¡�

       ����:MMC��MMA����ע��״̬�����GMM��δ������״̬��MMC�ϱ�ʱ����
            �ö�ʱ���ڲ�ѯͬ������ֲ�һ�¡�
    *******************************************************************/
    NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);

    /* Ǩ�Ƶ� NAS_MMC_L2_STA_POWER_OFF:NAS_MMC_POWER_OFF_STA_WAIT_MM_POWEROFF_CNF״̬,������������ʱ��TI_NAS_MMC_WAIT_MM_POWEROFF_CNF */
    NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_MM_POWEROFF_CNF);

    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_MM_POWEROFF_CNF, TI_NAS_MMC_WAIT_MM_POWEROFF_CNF_LEN);

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvMmaPowerSaveReq_PowerOff_Init(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    /* ���������Ϣ */
    NAS_MMC_SaveCurEntryMsg(ulEventType, pstMsg);

    /* ��MM/GMM����Cover lostָʾ */
    NAS_MMC_SndMmCoverageLostInd();
    NAS_MMC_SndGmmCoverageLostInd();

    /* ��LMM���͹�������,����������ʱ��TI_NAS_MMC_WAIT_LMM_SUSPEND_CNF,״̬Ǩ�Ƶ�NAS_MMC_POWER_OFF_STA_WAIT_LMM_SUSPEND_CNF */
    NAS_MMC_SndSuspendReq_PowerOff(NAS_MML_GetCurrNetRatType());

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvMmPowerOffCnf_PowerOff_WaitMmPoweroffCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enMasterModeRat;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstSlaveModeRatList = VOS_NULL_PTR;

    /* ��ȡ��ģ�ػ��б� */
    pstSlaveModeRatList = NAS_MMC_GetSlaveModeRatList_PowerOff();
    enMasterModeRat     = NAS_MMC_GetMasterModeRat_PowerOff();

    /* ����MM�Ĺػ�ȷ�ϱ�־ */
    NAS_MMC_ClearMmPowerOffCnfFlag_PowerOff( NAS_MMC_MM_POWER_OFF_FLG );

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);

    /* δ����MM��GMM�Ĺػ�ȷ����Ϣ�����ؼ����ȴ� */
    if ( NAS_MMC_MMGMM_POWER_OFF_NULL_FLG != NAS_MMC_GetMmPowerOffCnfFlg_PowerOff())
    {
        return VOS_TRUE;
    }

    NAS_MMC_StopTimer ( TI_NAS_MMC_WAIT_MM_POWEROFF_CNF );

    switch ( enMasterModeRat )
    {

#if (FEATURE_ON == FEATURE_LTE)

        case NAS_MML_NET_RAT_TYPE_LTE:

            /* �����ģ����Ϊ0����ʾƽ̨֧�ֵ�ģ������Ҫ�ٸ�����ģ���͹ػ���Ϣ��ֱ����״̬�� */
            if (0 == pstSlaveModeRatList->ucRatNum)
            {
                /* ���GAS��WAS��LMM�Ĺػ�ȷ����Ϣ��¼��־ */
                NAS_MMC_ClearAllAsPowerOffCnfFlag_PowerOff();

                /* �ϱ���TAF���ػ��ɹ� */
                NAS_Mmc_SndMmaPowerOffCnf();

                /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
                NAS_MMC_SndPowerOffRslt(NAS_MMC_POWEROFF_RSLT_POWER_OFF);

                /* �ػ���ɣ��˳����״̬��*/
                NAS_MMC_FSM_QuitFsmL2();

                return VOS_TRUE;
            }
            else
            {
                /* ����ģ���͹ػ����� */
                NAS_MMC_SndSlaveModePowerOffReq_PowerOff();

                /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF,������������ʱ�� */
                NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF);
                NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF_LEN);
            }
            break;
#endif

        case NAS_MML_NET_RAT_TYPE_WCDMA:

            /* ��WAS��ģ���͹ػ���Ϣ,���õȴ�Was�ظ���־ */
            NAS_MMC_SndAsPowerOffReq(WUEPS_PID_WRR);

            /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_MASTER_MODE_POWEROFF_CNF,������������ʱ�� */
            NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_MASTER_MODE_POWEROFF_CNF);
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF_LEN);

            break;

        case NAS_MML_NET_RAT_TYPE_GSM:

            /* ��GAS��ģ���͹ػ���Ϣ,���õȴ�gas�ظ���־ */
            NAS_MMC_SndAsPowerOffReq(UEPS_PID_GAS);

            /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_MASTER_MODE_POWEROFF_CNF,������������ʱ�� */
            NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_MASTER_MODE_POWEROFF_CNF);
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF_LEN);

            break;

        /* ��ǰû����ģ */
        default:

            /* �����ģ����Ϊ0����û����ģ����λ */
            if (0 == pstSlaveModeRatList->ucRatNum)
            {
                /* ���ɻָ�����,���õ���ӿ�RESET */
                NAS_MML_SoftReBoot();

                return VOS_TRUE;
            }
            else
            {
                /* ����ģ���͹ػ����� */
                NAS_MMC_SndSlaveModePowerOffReq_PowerOff();

                /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF,������������ʱ�� */
                NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF);
                NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF_LEN);
            }
            break;
    }


    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvGmmPowerOffCnf_PowerOff_WaitMmPoweroffCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enMasterModeRat;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstSlaveModeRatList = VOS_NULL_PTR;

    /* ��ȡ��ģ�ػ��б� */
    pstSlaveModeRatList = NAS_MMC_GetSlaveModeRatList_PowerOff();
    enMasterModeRat = NAS_MMC_GetMasterModeRat_PowerOff();

    /* ����GMM�Ĺػ�ȷ�ϱ�־ */
    NAS_MMC_ClearMmPowerOffCnfFlag_PowerOff( NAS_MMC_GMM_POWER_OFF_FLG );

    NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);

    /*  δ����MM��GMM�Ĺػ�ȷ����Ϣ�����ؼ����ȴ� */
    if ( NAS_MMC_MMGMM_POWER_OFF_NULL_FLG != NAS_MMC_GetMmPowerOffCnfFlg_PowerOff())
    {
        return VOS_TRUE;
    }

    NAS_MMC_StopTimer ( TI_NAS_MMC_WAIT_MM_POWEROFF_CNF );

    switch ( enMasterModeRat )
    {

#if (FEATURE_ON == FEATURE_LTE)

        case NAS_MML_NET_RAT_TYPE_LTE:

            /* �����ģ����Ϊ0����ʾƽ̨֧�ֵ�ģ������Ҫ�ٸ�����ģ���͹ػ���Ϣ��ֱ����״̬�� */
            if (0 == pstSlaveModeRatList->ucRatNum)
            {
                /* ���GAS��WAS��LMM�Ĺػ�ȷ����Ϣ��¼��־ */
                NAS_MMC_ClearAllAsPowerOffCnfFlag_PowerOff();

                /* �ϱ���TAF���ػ��ɹ� */
                NAS_Mmc_SndMmaPowerOffCnf();

                /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
                NAS_MMC_SndPowerOffRslt(NAS_MMC_POWEROFF_RSLT_POWER_OFF);

                /* �ػ���ɣ��˳����״̬��*/
                NAS_MMC_FSM_QuitFsmL2();

                return VOS_TRUE;
            }
            else
            {
                /* ����ģ���͹ػ����� */
                NAS_MMC_SndSlaveModePowerOffReq_PowerOff();

                /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF,������������ʱ�� */
                NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF);
                NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF_LEN);
            }
            break;
#endif

        case NAS_MML_NET_RAT_TYPE_WCDMA:

            /* ��WAS���͹ػ���Ϣ,���õȴ�Was�ظ���־ */
            NAS_MMC_SndAsPowerOffReq(WUEPS_PID_WRR);

            /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_MASTER_MODE_POWEROFF_CNF,������������ʱ�� */
            NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_MASTER_MODE_POWEROFF_CNF);
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF_LEN);
            break;

        case NAS_MML_NET_RAT_TYPE_GSM:

            /* ��GAS���͹ػ���Ϣ,���õȴ�gas�ظ���־ */
            NAS_MMC_SndAsPowerOffReq(UEPS_PID_GAS);

            /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_MASTER_MODE_POWEROFF_CNF,������������ʱ�� */
            NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_MASTER_MODE_POWEROFF_CNF);
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF_LEN);

            break;

        /* ��ǰû����ģ */
        default:

            /* �����ģ����Ϊ0����û����ģ����λ */
            if (0 == pstSlaveModeRatList->ucRatNum)
            {
                /* ���ɻָ�����,���õ���ӿ�RESET */
                NAS_MML_SoftReBoot();

                return VOS_TRUE;
            }
            else
            {
                /* ����ģ���͹ػ����� */
                NAS_MMC_SndSlaveModePowerOffReq_PowerOff();

                /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF,������������ʱ�� */
                NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF);
                NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF_LEN);
            }
            break;
    }


    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvTiWaitMmPowerOffCnfExpired_PowerOff_WaitMmPowerOffCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enMasterModeRat;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstSlaveModeRatList = VOS_NULL_PTR;

    /* ��ȡ��ģ�ػ��б� */
    pstSlaveModeRatList = NAS_MMC_GetSlaveModeRatList_PowerOff();
    enMasterModeRat = NAS_MMC_GetMasterModeRat_PowerOff();

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitMmPowerOffCnfExpired_PowerOff_WaitMmPowerOffCnf ENTERED");

    /* ���MM�Ĺػ�ȷ�ϱ�־ */
    NAS_MMC_ClearAllMmPowerOffCnfFlag_PowerOff();

    NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);


    switch ( enMasterModeRat )
    {

#if (FEATURE_ON == FEATURE_LTE)

        case NAS_MML_NET_RAT_TYPE_LTE:

            /* �����ģ����Ϊ0����ʾƽ̨֧�ֵ�ģ������Ҫ�ٸ�����ģ���͹ػ���Ϣ��ֱ����״̬�� */
            if (0 == pstSlaveModeRatList->ucRatNum)
            {
                /* ���GAS��WAS��LMM�Ĺػ�ȷ����Ϣ��¼��־ */
                NAS_MMC_ClearAllAsPowerOffCnfFlag_PowerOff();

                /* �ϱ���TAF���ػ��ɹ� */
                NAS_Mmc_SndMmaPowerOffCnf();

                /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
                NAS_MMC_SndPowerOffRslt(NAS_MMC_POWEROFF_RSLT_POWER_OFF);

                /* �ػ���ɣ��˳����״̬��*/
                NAS_MMC_FSM_QuitFsmL2();

                return VOS_TRUE;
            }
            else
            {
                /* ����ģ���͹ػ����� */
                NAS_MMC_SndSlaveModePowerOffReq_PowerOff();

                /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF,������������ʱ�� */
                NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF);
                NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF_LEN);
            }
            break;
#endif

        case NAS_MML_NET_RAT_TYPE_WCDMA:

            /* ��WAS���͹ػ���Ϣ,���õȴ�Was�ظ���־ */
            NAS_MMC_SndAsPowerOffReq(WUEPS_PID_WRR);

            /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_MASTER_MODE_POWEROFF_CNF,������������ʱ�� */
            NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_MASTER_MODE_POWEROFF_CNF);
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF_LEN);

            break;

        case NAS_MML_NET_RAT_TYPE_GSM:

            /* ��GAS���͹ػ���Ϣ,���õȴ�gas�ظ���־ */
            NAS_MMC_SndAsPowerOffReq(UEPS_PID_GAS);

            /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_MASTER_MODE_POWEROFF_CNF,������������ʱ�� */
            NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_MASTER_MODE_POWEROFF_CNF);
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF_LEN);

            break;

        /* ��ǰû����ģ */
        default:

            /* �����ģ����Ϊ0����û����ģ����λ */
            if (0 == pstSlaveModeRatList->ucRatNum)
            {
                /* ���ɻָ�����,���õ���ӿ�RESET */
                NAS_MML_SoftReBoot();

                return VOS_TRUE;
            }
            else
            {
                /* ����ģ���͹ػ����� */
                NAS_MMC_SndSlaveModePowerOffReq_PowerOff();

                /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF,������������ʱ�� */
                NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF);
                NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF_LEN);
            }
            break;
    }


    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvGasPowerOffCnf_PowerOff_WaitMasterModePowerOffCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstSlaveModeRatList = VOS_NULL_PTR;

    NAS_TRACE_HIGH("Enter!");

    /* ��ȡ��ģ�ػ��б� */
    pstSlaveModeRatList = NAS_MMC_GetSlaveModeRatList_PowerOff();

    /* ֹͣ�յ���ģ�ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF);

    /* �����ģ����Ϊ0����ʾƽ̨֧�ֵ�ģ������Ҫ�ٸ�����ģ���͹ػ���Ϣ��ֱ����״̬�� */
    if (0 == pstSlaveModeRatList->ucRatNum)
    {
        /* ���GAS��WAS��LMM�Ĺػ�ȷ����Ϣ��¼��־ */
        NAS_MMC_ClearAllAsPowerOffCnfFlag_PowerOff();

        /* �ϱ���TAF���ػ��ɹ� */
        NAS_Mmc_SndMmaPowerOffCnf();

        /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
        NAS_MMC_SndPowerOffRslt(NAS_MMC_POWEROFF_RSLT_POWER_OFF);

        /* �ػ���ɣ��˳����״̬��*/
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }
    else
    {
        /* ����ģ���͹ػ����� */
        NAS_MMC_SndSlaveModePowerOffReq_PowerOff();

        /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF,������������ʱ�� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF);
        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF_LEN);
    }

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvWasPowerOffCnf_PowerOff_WaitMasterModePowerOffCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstSlaveModeRatList = VOS_NULL_PTR;

    NAS_TRACE_HIGH("%s", (NAS_UTRANCTRL_UTRAN_MODE_FDD == NAS_UTRANCTRL_GetCurrUtranMode()) ? "FDD" : "TDD");

    /* ��ȡ��ģ�ػ��б� */
    pstSlaveModeRatList = NAS_MMC_GetSlaveModeRatList_PowerOff();

    /* ֹͣ�յ���ģ�ظ���ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF);

    vos_printf("NAS_MMC_RcvWasPowerOffCnf_PowerOff_WaitMasterModePowerOffCnf, tick = %d\r\n ", OM_GetSlice());

    /* �����ģ����Ϊ0����ʾƽ̨֧�ֵ�ģ������Ҫ�ٸ�����ģ���͹ػ���Ϣ��ֱ����״̬�� */
    if (0 == pstSlaveModeRatList->ucRatNum)
    {
        /* ���GAS��WAS��LMM�Ĺػ�ȷ����Ϣ��¼��־ */
        NAS_MMC_ClearAllAsPowerOffCnfFlag_PowerOff();

        /* �ϱ���TAF���ػ��ɹ� */
        NAS_Mmc_SndMmaPowerOffCnf();

        /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
        NAS_MMC_SndPowerOffRslt(NAS_MMC_POWEROFF_RSLT_POWER_OFF);

        /* �ػ���ɣ��˳����״̬��*/
        NAS_MMC_FSM_QuitFsmL2();

        return VOS_TRUE;
    }
    else
    {
        /* ����ģ���͹ػ����� */
        NAS_MMC_SndSlaveModePowerOffReq_PowerOff();

        /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF,������������ʱ�� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF);
        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF_LEN);
    }

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvTiWaitMasterModePowerOffCnfExpired_PowerOff_WaitMasterModePowerOffCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enMasterModeRat;
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstSlaveModeRatList = VOS_NULL_PTR;

    /* ��ȡ��ģ�ػ��б� */
    pstSlaveModeRatList = NAS_MMC_GetSlaveModeRatList_PowerOff();
    enMasterModeRat = NAS_MMC_GetMasterModeRat_PowerOff();

    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitMasterModePowerOffCnfExpired_PowerOff_WaitMasterModePowerOffCnf:ENTERED");

    NAS_TRACE_HIGH("Enter!");

    switch ( enMasterModeRat )
    {

#if (FEATURE_ON == FEATURE_LTE)

        case NAS_MML_NET_RAT_TYPE_LTE:

            /* ֪ͨMM�ػ��������õȴ�MM�ظ���־ */
            NAS_MMC_SndMmPowerOffReq();
            NAS_MMC_SetMmPowerOffCnfFlag_PowerOff( NAS_MMC_MM_POWER_OFF_FLG );

            /* ֪ͨGMM�ػ��������õȴ�GMM�ظ���־ */
            NAS_MMC_SndGmmPowerOffReq();
            NAS_MMC_SetMmPowerOffCnfFlag_PowerOff( NAS_MMC_GMM_POWER_OFF_FLG );

            /* Ǩ�Ƶ� NAS_MMC_L2_STA_POWER_OFF:NAS_MMC_POWER_OFF_STA_WAIT_MM_POWEROFF_CNF״̬,������������ʱ��TI_NAS_MMC_WAIT_MM_POWEROFF_CNF*/
            NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_MM_POWEROFF_CNF);
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_MM_POWEROFF_CNF, TI_NAS_MMC_WAIT_MM_POWEROFF_CNF_LEN);

            break;
#endif

        case NAS_MML_NET_RAT_TYPE_WCDMA:

            /* �����ģ����Ϊ0����ʾƽ̨֧�ֵ�ģ������Ҫ�ٸ�����ģ���͹ػ���Ϣ��ֱ����״̬�� */
            if (0 == pstSlaveModeRatList->ucRatNum)
            {
                /* ���GAS��WAS��LMM�Ĺػ�ȷ����Ϣ��¼��־ */
                NAS_MMC_ClearAllAsPowerOffCnfFlag_PowerOff();

                /* �ϱ���TAF���ػ��ɹ� */
                NAS_Mmc_SndMmaPowerOffCnf();

                /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
                NAS_MMC_SndPowerOffRslt(NAS_MMC_POWEROFF_RSLT_POWER_OFF);

                /* �ػ���ɣ��˳����״̬��*/
                NAS_MMC_FSM_QuitFsmL2();

                return VOS_TRUE;
            }
            else
            {
                /* ����ģ���͹ػ����� */
                NAS_MMC_SndSlaveModePowerOffReq_PowerOff();

                /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF,������������ʱ�� */
                NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF);
                NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF_LEN);
            }
            break;

        case NAS_MML_NET_RAT_TYPE_GSM:

            /* �����ģ����Ϊ0����ʾƽ̨֧�ֵ�ģ������Ҫ�ٸ�����ģ���͹ػ���Ϣ��ֱ����״̬�� */
            if (0 == pstSlaveModeRatList->ucRatNum)
            {
                /* ���GAS��WAS��LMM�Ĺػ�ȷ����Ϣ��¼��־ */
                NAS_MMC_ClearAllAsPowerOffCnfFlag_PowerOff();

                /* �ϱ���TAF���ػ��ɹ� */
                NAS_Mmc_SndMmaPowerOffCnf();

                /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
                NAS_MMC_SndPowerOffRslt(NAS_MMC_POWEROFF_RSLT_POWER_OFF);

                /* �ػ���ɣ��˳����״̬��*/
                NAS_MMC_FSM_QuitFsmL2();

                return VOS_TRUE;
            }
            else
            {
                /* ����ģ���͹ػ����� */
                NAS_MMC_SndSlaveModePowerOffReq_PowerOff();

                /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF,������������ʱ�� */
                NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_SLAVE_MODE_POWEROFF_CNF);
                NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF_LEN);
            }
            break;

        /* ��ǰû����ģ */
        default:
            NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitMasterModePowerOffCnfExpired_PowerOff_WaitMasterModePowerOffCnf:NO MASTER MODE!");
            break;
    }

    return VOS_TRUE;

}



VOS_UINT32 NAS_MMC_RcvGasPowerOffCnf_PowerOff_WaitSlaveModePowerOffCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_TRACE_HIGH("Enter!");

    /* ����GAS�����Ĺػ�ȷ����Ϣ��¼��־ */
    NAS_MMC_ClearAsPowerOffCnfFlag_PowerOff( NAS_MMC_GAS_POWER_OFF_FLG );

    /* δ�������н����Ĺػ�ȷ����Ϣ�����ؼ����ȴ� */
    if ( NAS_MMC_AS_POWER_OFF_NULL_FLG != NAS_MMC_GetAsPowerOffCnfFlg_PowerOff() )
    {
        return VOS_TRUE;
    }


    /* ��ǰGģΪ��ģ���յ������н����Ĺػ��ظ� */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF);

    /* �ϱ���TAF���ػ��ɹ� */
    NAS_Mmc_SndMmaPowerOffCnf();

    /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
    NAS_MMC_SndPowerOffRslt(NAS_MMC_POWEROFF_RSLT_POWER_OFF);

    /* �ػ���ɣ��˳����״̬��*/
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvWasPowerOffCnf_PowerOff_WaitSlaveModePowerOffCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_TRACE_HIGH("%s", (NAS_UTRANCTRL_UTRAN_MODE_FDD == NAS_UTRANCTRL_GetCurrUtranMode()) ? "FDD" : "TDD");

    /* ���½����WAS�Ĺػ�ȷ����Ϣ��¼��־ */
    NAS_MMC_ClearAsPowerOffCnfFlag_PowerOff( NAS_MMC_WAS_POWER_OFF_FLG );

    /* δ�������н����Ĺػ�ȷ����Ϣ,���ؼ����ȴ� */
    if ( NAS_MMC_AS_POWER_OFF_NULL_FLG != NAS_MMC_GetAsPowerOffCnfFlg_PowerOff() )
    {
        return VOS_TRUE;
    }

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF);

    /* �ϱ���TAF���ػ��ɹ� */
    NAS_Mmc_SndMmaPowerOffCnf();

    /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
    NAS_MMC_SndPowerOffRslt(NAS_MMC_POWEROFF_RSLT_POWER_OFF);

    /* �ػ���ɣ��˳����״̬��*/
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvTiWaitSlaveModePowerOffCnfExpired_PowerOff_WaitSlaveModePowerOffCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �쳣��ӡ */
    NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitSlaveModePowerOffCnfExpired_PowerOff_WaitSlaveModePowerOffCnf:ENTERED");

    NAS_TRACE_HIGH("Enter!");

    /* ���GAS��WAS��LMM�Ĺػ�ȷ����Ϣ��¼��־ */
    NAS_MMC_ClearAllAsPowerOffCnfFlag_PowerOff();

    /* �ϱ���TAF���ػ��ɹ� */
    NAS_Mmc_SndMmaPowerOffCnf();

    /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
    NAS_MMC_SndPowerOffRslt(NAS_MMC_POWEROFF_RSLT_POWER_OFF);

    /* �ػ���ɣ��˳����״̬��*/
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;

}



#if (FEATURE_ON == FEATURE_LTE)
VOS_UINT32 NAS_MMC_RcvLmmStopCnf_PowerOff_WaitMasterModePowerOffCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_TRACE_HIGH("Enter!");

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF);

    /* ֪ͨMM�ػ��������õȴ�MM�ظ���־ */
    NAS_MMC_SndMmPowerOffReq();

    NAS_MMC_SetMmPowerOffCnfFlag_PowerOff( NAS_MMC_MM_POWER_OFF_FLG );

    /* ֪ͨGMM�ػ��������õȴ�GMM�ظ���־ */
    NAS_MMC_SndGmmPowerOffReq();

    NAS_MMC_SetMmPowerOffCnfFlag_PowerOff( NAS_MMC_GMM_POWER_OFF_FLG );

    /* Ǩ�Ƶ� NAS_MMC_L2_STA_POWER_OFF:NAS_MMC_POWER_OFF_STA_WAIT_MM_POWEROFF_CNF״̬,������������ʱ��TI_NAS_MMC_WAIT_MM_POWEROFF_CNF*/
    NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_MM_POWEROFF_CNF);

    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_MM_POWEROFF_CNF, TI_NAS_MMC_WAIT_MM_POWEROFF_CNF_LEN);

    return VOS_TRUE;

}
VOS_UINT32 NAS_MMC_RcvLmmSuspendCnf_PowerOff_WaitLmmSuspendCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_SUSPEND_CNF_STRU           *pstSuspendCnf = VOS_NULL_PTR;

    pstSuspendCnf = (LMM_MMC_SUSPEND_CNF_STRU*)pstMsg;

    /* ֹͣ������ʱ��TI_NAS_MMC_WAIT_LMM_SUSPEND_CNF */
    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_LMM_SUSPEND_CNF);

    /* �������ʧ��ֱ�Ӹ�λ */
    if (MMC_LMM_FAIL == pstSuspendCnf->ulRst)
    {
        /* �쳣��ӡ */
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmSuspendCnf_PowerOff_WaitLmmSuspendCnf: SUSPEND FAIL !");

        /* ���ɻָ�����,���õ���ӿ�RESET */
        NAS_MML_SoftReBoot();

        return VOS_TRUE;
    }

    /* ���õ�ǰ���뼼��ΪBUTT */
    NAS_MML_SetCurrNetRatType(NAS_MML_NET_RAT_TYPE_BUTT);

    /* �ϱ���MMA Power Save�ɹ� */
    NAS_MMC_SndMmaPowerSaveCnf();

    /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
    NAS_MMC_SndPowerOffRslt(NAS_MMC_POWEROFF_RSLT_POWER_SAVE);

    /* �ػ���ɣ��˳����״̬�� */
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvTiWaitLmmSuspendCnfExpired_PowerOff_WaitLmmSuspendCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �쳣��ӡ */
    NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTiWaitLmmSuspendCnfExpired_PowerOff_WaitLmmSuspendCnf: Enter !");

    /* ��λ */
    NAS_MML_SoftReBoot();

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvLmmStopCnf_PowerOff_WaitSlaveModePowerOffCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_TRACE_HIGH("Enter!");

    /* ���½����LTE�Ĺػ�ȷ����Ϣ��¼��־ */
    NAS_MMC_ClearAsPowerOffCnfFlag_PowerOff( NAS_MMC_LTE_POWER_OFF_FLG );

    /* δ�������н����Ĺػ�ȷ����Ϣ�����ؼ����ȴ� */
    if ( NAS_MMC_AS_POWER_OFF_NULL_FLG != NAS_MMC_GetAsPowerOffCnfFlg_PowerOff() )
    {
        return VOS_TRUE;
    }

    NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_SLAVE_MODE_POWEROFF_CNF);

    /* �ϱ���TAF���ػ��ɹ� */
    NAS_Mmc_SndMmaPowerOffCnf();

    /* ��������н��֪ͨ��һ����һ�յ�����Ϣ����в�һ״̬��Ǩ�� */
    NAS_MMC_SndPowerOffRslt(NAS_MMC_POWEROFF_RSLT_POWER_OFF);

    /* �ػ���ɣ��˳����״̬��*/
    NAS_MMC_FSM_QuitFsmL2();

    return VOS_TRUE;
}
VOS_UINT32  NAS_MMC_RcvLmmUsimStatusCnf_PowerOff_WaitLmmUsimStatusCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* MMCֹͣ��ʱ��: TI_NAS_MMC_WAIT_LMM_USIM_STATUS_CNF */
    NAS_MMC_StopTimer( TI_NAS_MMC_WAIT_LMM_USIM_STATUS_CNF );

#if   (FEATURE_ON == FEATURE_LTE)

    if ( NAS_MML_NET_RAT_TYPE_LTE == NAS_MMC_GetMasterModeRat_PowerOff() )
    {

        /* ��LTE��ģ���͹ػ���Ϣ */
        NAS_MMC_SndLmmStopReq();

        /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_MASTER_MODE_POWEROFF_CNF,������10s������ʱ�� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_MASTER_MODE_POWEROFF_CNF);

        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF_LEN);

        return VOS_TRUE;
    }

#endif

    /* ֪ͨMM�ػ��������õȴ�MM�ظ���־ */
    NAS_MMC_SndMmPowerOffReq();

    NAS_MMC_SetMmPowerOffCnfFlag_PowerOff( NAS_MMC_MM_POWER_OFF_FLG );

    /* ֪ͨGMM�ػ��������õȴ�GMM�ظ���־ */
    NAS_MMC_SndGmmPowerOffReq();

    NAS_MMC_SetMmPowerOffCnfFlag_PowerOff( NAS_MMC_GMM_POWER_OFF_FLG );

    /* Ǩ�Ƶ� NAS_MMC_L2_STA_POWER_OFF:NAS_MMC_POWER_OFF_STA_WAIT_MM_POWEROFF_CNF״̬,������������ʱ��TI_NAS_MMC_WAIT_MM_POWEROFF_CNF*/
    NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_MM_POWEROFF_CNF);

    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_MM_POWEROFF_CNF, TI_NAS_MMC_WAIT_MM_POWEROFF_CNF_LEN);

    return VOS_TRUE;

}


VOS_UINT32  NAS_MMC_RcvTiWaitLmmUsimStatusExpired_PowerOff_WaitLmmUsimStatusCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
#if   (FEATURE_ON == FEATURE_LTE)

    if ( NAS_MML_NET_RAT_TYPE_LTE == NAS_MMC_GetMasterModeRat_PowerOff() )
    {

        /* ��LTE���͹ػ���Ϣ */
        NAS_MMC_SndLmmStopReq();

        /* Ǩ��״̬��NAS_MMC_POWEROFF::NAS_MMC_POWER_OFF_STA_WAIT_AS_POWEROFF_CNF,������10s������ʱ�� */
        NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_MASTER_MODE_POWEROFF_CNF);

        NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF, TI_NAS_MMC_WAIT_MASTER_MODE_POWEROFF_CNF_LEN);

        return VOS_TRUE;
    }
#endif

    /* ֪ͨMM�ػ��������õȴ�MM�ظ���־ */
    NAS_MMC_SndMmPowerOffReq();

    NAS_MMC_SetMmPowerOffCnfFlag_PowerOff( NAS_MMC_MM_POWER_OFF_FLG );

    /* ֪ͨGMM�ػ��������õȴ�GMM�ظ���־ */
    NAS_MMC_SndGmmPowerOffReq();

    NAS_MMC_SetMmPowerOffCnfFlag_PowerOff( NAS_MMC_GMM_POWER_OFF_FLG );

    /* Ǩ�Ƶ� NAS_MMC_L2_STA_POWER_OFF:NAS_MMC_POWER_OFF_STA_WAIT_MM_POWEROFF_CNF״̬,������������ʱ��TI_NAS_MMC_WAIT_MM_POWEROFF_CNF*/
    NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_MM_POWEROFF_CNF);

    NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_MM_POWEROFF_CNF, TI_NAS_MMC_WAIT_MM_POWEROFF_CNF_LEN);

    return VOS_TRUE;
}
#endif


VOS_UINT32  NAS_MMC_SndSlaveModePowerOffReq_PowerOff(VOS_VOID)
{
    NAS_MML_PLMN_RAT_PRIO_STRU                 *pstSlaveModeRatList = VOS_NULL_PTR;
    VOS_UINT32                                  i;

    /* ��ȡ��ģ�ػ��б� */
    pstSlaveModeRatList = NAS_MMC_GetSlaveModeRatList_PowerOff();

    /* ƽ̨���ǵ�ģʱ������ģ���͹ػ���Ϣ */
    for (i = 0; i < pstSlaveModeRatList->ucRatNum; i++)
    {
        /* ��GAS���͹ػ���Ϣ,���õȴ�gas�ظ���־ */
        if (NAS_MML_NET_RAT_TYPE_GSM == pstSlaveModeRatList->aucRatPrio[i])
        {
            NAS_MMC_SndAsPowerOffReq(UEPS_PID_GAS);
            NAS_MMC_SetAsPowerOffCnfFlag_PowerOff( NAS_MMC_GAS_POWER_OFF_FLG );
        }

        /* ��WAS���͹ػ���Ϣ,���õȴ�Was�ظ���־ */
        else if (NAS_MML_NET_RAT_TYPE_WCDMA == pstSlaveModeRatList->aucRatPrio[i])
        {
            NAS_MMC_SndAsPowerOffReq(WUEPS_PID_WRR);
            NAS_MMC_SetAsPowerOffCnfFlag_PowerOff( NAS_MMC_WAS_POWER_OFF_FLG );
        }

        /* ��LMM���͹ػ���Ϣ,���õȴ�LMM�ظ���־ */
#if (FEATURE_ON == FEATURE_LTE)
        else if (NAS_MML_NET_RAT_TYPE_LTE == pstSlaveModeRatList->aucRatPrio[i])
        {
            NAS_MMC_SndLmmStopReq();
            NAS_MMC_SetAsPowerOffCnfFlag_PowerOff( NAS_MMC_LTE_POWER_OFF_FLG );
        }
#endif
        else
        {
        }
    }

    return VOS_TRUE;

}



#if (FEATURE_ON == FEATURE_DSDS)
VOS_VOID NAS_MMC_ProcRrmDeRegister_PowerOff(VOS_VOID)
{
    MODEM_ID_ENUM_UINT16                usModemId;

    /* ȡ�õ�ǰMODEM��ID��Ϣ  */
    usModemId = VOS_GetModemIDFromPid(WUEPS_PID_MMC);

    /* �����ǰNVδ����DSDS,����RRM���͹ػ�ȥע�� */
    if ( VOS_FALSE == NAS_MML_GetDsdsRfShareFlg() )
    {
        return;
    }

    /* ��MODEM0ʱ����RRM����W�µ�ȥע����Ϣ */
    if ( MODEM_ID_0 == usModemId )
    {
        NAS_MMC_SndRrmDeRegisterInd(MODEM_ID_0,
                                    RRM_PS_TASK_TYPE_NAS_SEARCH,
                                    RRM_PS_RAT_TYPE_WCDMA);
    }

    /* ��MODEM1ʱ����RRM����G�µ�ȥע����Ϣ */
    if ( MODEM_ID_1 == usModemId )
    {
        NAS_MMC_SndRrmDeRegisterInd(MODEM_ID_1,
                                    RRM_PS_TASK_TYPE_NAS_SEARCH,
                                    RRM_PS_RAT_TYPE_GSM);
    }

    return;
}

#endif


VOS_VOID NAS_MMC_SndSuspendReq_PowerOff(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
)
{
    /* ״̬����Ϊ��ǰδפ�� */
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
    NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_RUNNING);

    NAS_MMC_SetAsAnyCampOn(VOS_FALSE);

    /* ���ݲ�ͬ�Ľ��뼼�����õȴ������ظ���״̬ */
    switch (enRat)
    {
#if (FEATURE_ON == FEATURE_LTE)
        case NAS_MML_NET_RAT_TYPE_LTE:
            NAS_MMC_SndLmmSuspendReq();
            NAS_MMC_FSM_SetCurrState(NAS_MMC_POWER_OFF_STA_WAIT_LMM_SUSPEND_CNF);
            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_LMM_SUSPEND_CNF, TI_NAS_MMC_WAIT_LMM_SUSPEND_CNF_LEN);
            break;
#endif

        case NAS_MML_NET_RAT_TYPE_WCDMA:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SndSuspendReq_PowerOff: Not support WCDMA PowerSave.");
            break;

        case NAS_MML_NET_RAT_TYPE_GSM:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SndSuspendReq_PowerOff: Not support GSM PowerSave.");
            break;

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SndSuspendReq_PowerOff: Rat not support.");
            break;
    }

    return;
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

