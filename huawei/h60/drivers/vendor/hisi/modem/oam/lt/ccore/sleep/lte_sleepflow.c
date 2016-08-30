

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*lint -save -e537*/
#include <lte_sleepflow.h>
#include "vos.h"
#include <DrvInterface.h>
#include "msp_errno.h"

#include "product_config.h"
#include "msp_nvim.h"
#include <phyoaminterface.h>
#if (VOS_VXWORKS == VOS_OS_VER)
#include <cacheLib.h>
#endif

#include "FileSysInterface.h"
/*lint -restore*/

/*���ڿ��ƴ򿪹ر�MSP���ֵ͹���*/
#ifndef OPEN_MSP_SLEEP
#define OPEN_MSP_SLEEP
#endif

#ifndef MSP_IN_V9R1
#include <bsp_sram.h>
#include <TLPhyInterface.h>
#else
#include "NVIM_Interface.h"
#include <global_sram_map.h>
#include "msp_nvim.h"
#endif
#define    THIS_FILE_ID   (MSP_FILE_ID_LTE_SLEEPFLOW_C)

struct msp_power_control g_msp_pwrctrl = {0}; /*lint !e19*/
struct msp_delay_control g_msp_delay = {0, 5};
struct msp_hids_control g_msp_hidsctrl = {0, 0};
DRV_DRX_DELAY_STRU g_msp_drx_delay = {0};
VOS_INT dfs_bus_req_id = 0;
VOS_INT dfs_ddr_req_id = 0;
VOS_INT dfs_ccpu_req_id = 0;

#ifdef OPEN_MSP_SLEEP
/*lint -e409 -e52 -e550 -e701*/
/*****************************************************************************
 �� �� ��  : QueueInit/QueueIn/QueueLoopIn
 ��������  : ���в���,Ϊ��ά�ɲ�ʹ��
 �������  : void
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
static inline void QueueInit(drx_queue_t *Q, VOS_UINT32 elementNum)
{
    Q->maxNum = elementNum;
    Q->front = 0;
    Q->rear = 0;
    Q->num = 0;
}

static inline VOS_INT32 QueueIn(drx_queue_t *Q, VOS_UINT32 state)
{
    if (Q->num == Q->maxNum)
    {
        return -1;
    }

    Q->data[Q->rear].ulSlice = PWRCTRL_GetSleepSlice();
    Q->data[Q->rear].ulState = state;

    Q->rear = (Q->rear+1) % Q->maxNum;
    Q->num++;

    return 0;
}

static inline VOS_INT32 QueueLoopIn(drx_queue_t *Q, VOS_UINT32 state)
{
    if (Q->num < Q->maxNum)
    {
        return QueueIn(Q, state);
    }
    else
    {
        Q->data[Q->rear].ulSlice = PWRCTRL_GetSleepSlice();
        Q->data[Q->rear].ulState = state;

        Q->rear = (Q->rear+1) % Q->maxNum;
        Q->front = (Q->front+1) % Q->maxNum;
    }

    return 0;
}
/*****************************************************************************
 �� �� ��  : SLEEP_TimeRecord
 ��������  : ��¼������Ϣ
 �������  : void
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_VOID TLSLEEP_TimeRecord(char * cString)
{
    VOS_UINT32 ulRecordPosition = 0;
    VOS_UINT32 ulStringLenth = 0;
    g_msp_pwrctrl.SleepTimeRecord.ulCount++;
    ulRecordPosition = ((g_msp_pwrctrl.SleepTimeRecord.ulCount-1)%SLEEP_RECORD_NUM);
    TLSLEEP_GetTime(ulRecordPosition) = PWRCTRL_GetSleepSlice();
    ulStringLenth = strlen(cString);
    if(ulStringLenth > 16)
    {
        mspsleep_print_error("%s Len is more than 16", cString);
        return;
    }
    strncpy(TLSLEEP_GetType(ulRecordPosition),cString, strlen(cString));
    return;
}
/*****************************************************************************
 �� �� ��  : TLSLEEP_StateRecord
 ��������  : ��¼���������и����׶ε���Ϣ
 �������  : void
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_VOID TLSLEEP_StateRecord(DRX_STATE_SLICE state)
{
    if(VOS_NULL == g_msp_pwrctrl.DrxStateSlice)
    {
        return;
    }
    g_msp_pwrctrl.DrxStateSlice[state].ulCount++;
    g_msp_pwrctrl.DrxStateSlice[state].ulSlice = PWRCTRL_GetSleepSlice();
}
/*lint +e52 +e550*/
static inline VOS_VOID TLSLEEP_DbgTimeRecord(unsigned int recored)
{
    if(VOS_NULL == g_msp_pwrctrl.time_queue_record)
    {
        return;
    }
    /*Ϊ���������Ĳ����ڴ˼���ʱ����g_msp_delay.delay_control��ÿһ��bit ��־�Ƿ������ʱ*/
    if(g_msp_delay.delay_control&(1<<recored))
    {
        TLSLEEP_DelayMs(g_msp_delay.delay_ms, 1);
    }
    if(TLSLEEP_DSPHALTISR_0 == recored)
    {
        QueueLoopIn(g_msp_pwrctrl.time_queue_record,0x5a5a5a5a);
    }
    QueueLoopIn(g_msp_pwrctrl.time_queue_record, recored);
    g_msp_pwrctrl.dbg_time_record[recored] = PWRCTRL_GetSleepSlice();
}
/*****************************************************************************
 �� �� ��  : TLSLEEP_DelayMs
 ��������  : ���Թ��ĵĹ�������Ҫ����ʱ�ĵط�������,��λms
 �������  : void
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_VOID TLSLEEP_DelayMs(VOS_UINT delay_ms, VOS_UINT flag)
{
    VOS_UINT oldtime = 0;
    VOS_UINT newtime = 0;

    oldtime = PWRCTRL_GetSleepSlice();
    newtime = PWRCTRL_GetSleepSlice();
    if(1 == flag)
    {
        while((oldtime + delay_ms*32) >  newtime)
        {
            newtime = PWRCTRL_GetSleepSlice();
        }
    }
}

VOS_VOID TLSLEEP_RunDsp(VOS_VOID)
{
    VOS_INT32 power_lock = 0;

    power_lock = VOS_SplIMP();
    set_power_status_bit(MSP_PWR_WAIT_RESUMER);
    VOS_Splx(power_lock);
    DRV_PWRCTRL_DSP_RUN();
}


/*****************************************************************************
 �� �� ��  : TLSLEEP_PllEnable
 ��������  : ����NV�����ý���PLL��ʹ��
 �������  : ��
 �������  : ��,
 �� �� ֵ  :
*****************************************************************************/
VOS_VOID TLSLEEP_PllEnable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule)
{
    switch(enCommModule)
    {
#if (FEATURE_MULTI_CHANNEL == FEATURE_ON)
        case PWC_COMM_MODULE_ABB:
            if(g_msp_pwrctrl.tlmode_channel.abb_channel_id == 2)
            {
                DRV_PWRCTRL_PLLENABLE(enCommMode, enCommModule, PWC_COMM_MODEM_0, PWC_COMM_CHANNEL_0);
                DRV_PWRCTRL_PLLENABLE(enCommMode, enCommModule, PWC_COMM_MODEM_0, PWC_COMM_CHANNEL_1);
            }
            else
            {
                DRV_PWRCTRL_PLLENABLE(enCommMode, enCommModule, PWC_COMM_MODEM_0, (PWC_COMM_CHANNEL_E)g_msp_pwrctrl.tlmode_channel.abb_channel_id);
            }
            break;
#else
        case PWC_COMM_MODULE_ABB:
            if(g_msp_pwrctrl.tlmode_channel.abb_channel_id == 2)
            {
                DRV_PWRCTRL_PLLENABLE(enCommMode, enCommModule, PWC_COMM_MODEM_0);
                DRV_PWRCTRL_PLLENABLE(enCommMode, enCommModule, PWC_COMM_MODEM_1);
            }
            else
            {
                DRV_PWRCTRL_PLLENABLE(enCommMode, enCommModule, (PWC_COMM_MODEM_E)g_msp_pwrctrl.tlmode_channel.abb_channel_id);
            }
            break;
#endif
        default:
            mspsleep_print_error("check your input enCommMode:%d,  enCommModule: %d\n", enCommMode, enCommModule);
    }
}
/*****************************************************************************
 �� �� ��  : TLSLEEP_PwrctrlUp
 ��������  : ����NV�����ý����ϵ�
 �������  : ��
 �������  : ��,
 �� �� ֵ  :
*****************************************************************************/
VOS_VOID TLSLEEP_PwrctrlUp(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule)
{
    switch(enCommModule)
    {
#if (FEATURE_MULTI_CHANNEL == FEATURE_ON)
        case PWC_COMM_MODULE_RF:
            if(g_msp_pwrctrl.tlmode_channel.rf_channel_id== 2)
            {
                DRV_PWRCTRL_PWRUP(enCommMode, enCommModule,  PWC_COMM_MODEM_0, PWC_COMM_CHANNEL_0);
                DRV_PWRCTRL_PWRUP(enCommMode, enCommModule,  PWC_COMM_MODEM_0, PWC_COMM_CHANNEL_1);
            }
            else
            {
                DRV_PWRCTRL_PWRUP(enCommMode, enCommModule, PWC_COMM_MODEM_0, (PWC_COMM_CHANNEL_E)g_msp_pwrctrl.tlmode_channel.rf_channel_id);
            }
            break;
        case PWC_COMM_MODULE_ABB:
            if(g_msp_pwrctrl.tlmode_channel.abb_channel_id== 2)
            {
                DRV_PWRCTRL_PWRUP(enCommMode, enCommModule,  PWC_COMM_MODEM_0, PWC_COMM_CHANNEL_0);
                DRV_PWRCTRL_PWRUP(enCommMode, enCommModule,  PWC_COMM_MODEM_0, PWC_COMM_CHANNEL_1);
            }
            else
            {
                DRV_PWRCTRL_PWRUP(enCommMode, enCommModule, PWC_COMM_MODEM_0, (PWC_COMM_CHANNEL_E)g_msp_pwrctrl.tlmode_channel.abb_channel_id);
            }
            break;
#else
        case PWC_COMM_MODULE_RF:
            if(g_msp_pwrctrl.tlmode_channel.rf_channel_id== 2)
            {
                DRV_PWRCTRL_PWRUP(enCommMode, enCommModule,  PWC_COMM_MODEM_0);
                DRV_PWRCTRL_PWRUP(enCommMode, enCommModule,  PWC_COMM_MODEM_1);
            }
            else
            {
                DRV_PWRCTRL_PWRUP(enCommMode, enCommModule, (PWC_COMM_MODEM_E)g_msp_pwrctrl.tlmode_channel.rf_channel_id);
            }
            break;
        case PWC_COMM_MODULE_ABB:
            if(g_msp_pwrctrl.tlmode_channel.abb_channel_id== 2)
            {
                DRV_PWRCTRL_PWRUP(enCommMode, enCommModule,  PWC_COMM_MODEM_0);
                DRV_PWRCTRL_PWRUP(enCommMode, enCommModule,  PWC_COMM_MODEM_1);
            }
            else
            {
                DRV_PWRCTRL_PWRUP(enCommMode, enCommModule, (PWC_COMM_MODEM_E)g_msp_pwrctrl.tlmode_channel.abb_channel_id);
            }
            break;

#endif
        default:
            mspsleep_print_error("check your input enCommMode:%d,  enCommModule: %d\n", enCommMode, enCommModule);
    }
}
/*****************************************************************************
 �� �� ��  : TLSLEEP_PllDisable
 ��������  : ����NV�����ý���PLL��ȥʹ��
 �������  : ��
 �������  : ��,
 �� �� ֵ  :
*****************************************************************************/
VOS_VOID TLSLEEP_PllDisable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule)
{
    switch(enCommModule)
    {
#if (FEATURE_MULTI_CHANNEL == FEATURE_ON)
        case PWC_COMM_MODULE_ABB:
            if(g_msp_pwrctrl.tlmode_channel.abb_channel_id == 2)
            {
                DRV_PWRCTRL_PLLDISABLE(enCommMode, enCommModule, PWC_COMM_MODEM_0, PWC_COMM_CHANNEL_0);
                DRV_PWRCTRL_PLLDISABLE(enCommMode, enCommModule, PWC_COMM_MODEM_0, PWC_COMM_CHANNEL_1);
            }
            else
            {
                DRV_PWRCTRL_PLLDISABLE(enCommMode, enCommModule, PWC_COMM_MODEM_0, (PWC_COMM_CHANNEL_E)g_msp_pwrctrl.tlmode_channel.abb_channel_id);
            }
            break;
#else
        case PWC_COMM_MODULE_ABB:
            if(g_msp_pwrctrl.tlmode_channel.abb_channel_id == 2)
            {
                DRV_PWRCTRL_PLLDISABLE(enCommMode, enCommModule, PWC_COMM_MODEM_0);
                DRV_PWRCTRL_PLLDISABLE(enCommMode, enCommModule, PWC_COMM_MODEM_1);
            }
            else
            {
                DRV_PWRCTRL_PLLDISABLE(enCommMode, enCommModule, (PWC_COMM_MODEM_E)g_msp_pwrctrl.tlmode_channel.abb_channel_id);
            }
            break;
#endif
        default:
            mspsleep_print_error("check your input enCommMode:%d,  enCommModule: %d\n", enCommMode, enCommModule);
      }
}
/*****************************************************************************
 �� �� ��  : TLSLEEP_PwrctrlDown
 ��������  : ����NV�����ý����µ�
 �������  : ��
 �������  : ��,
 �� �� ֵ  :
*****************************************************************************/
VOS_VOID TLSLEEP_PwrctrlDown(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule)
{
    switch(enCommModule)
    {
#if (FEATURE_MULTI_CHANNEL == FEATURE_ON)
        case PWC_COMM_MODULE_RF:
            if(g_msp_pwrctrl.tlmode_channel.rf_channel_id== 2)
            {
                DRV_PWRCTRL_PWRDOWN(enCommMode, enCommModule,  PWC_COMM_MODEM_0, PWC_COMM_CHANNEL_0);
                DRV_PWRCTRL_PWRDOWN(enCommMode, enCommModule,  PWC_COMM_MODEM_0, PWC_COMM_CHANNEL_1);
            }
            else
            {
                DRV_PWRCTRL_PWRDOWN(enCommMode, enCommModule, PWC_COMM_MODEM_0, (PWC_COMM_CHANNEL_E)g_msp_pwrctrl.tlmode_channel.rf_channel_id);
            }
            break;
        case PWC_COMM_MODULE_ABB:
            if(g_msp_pwrctrl.tlmode_channel.abb_channel_id== 2)
            {
                DRV_PWRCTRL_PWRDOWN(enCommMode, enCommModule,  PWC_COMM_MODEM_0, PWC_COMM_CHANNEL_0);
                DRV_PWRCTRL_PWRDOWN(enCommMode, enCommModule,  PWC_COMM_MODEM_0, PWC_COMM_CHANNEL_1);
            }
            else
            {
                DRV_PWRCTRL_PWRDOWN(enCommMode, enCommModule, PWC_COMM_MODEM_0, (PWC_COMM_CHANNEL_E)g_msp_pwrctrl.tlmode_channel.abb_channel_id);
            }
            break;
#else
        case PWC_COMM_MODULE_RF:
            if(g_msp_pwrctrl.tlmode_channel.rf_channel_id== 2)
            {
                DRV_PWRCTRL_PWRDOWN(enCommMode, enCommModule,  PWC_COMM_MODEM_0);
                DRV_PWRCTRL_PWRDOWN(enCommMode, enCommModule,  PWC_COMM_MODEM_1);
            }
            else
            {
                DRV_PWRCTRL_PWRDOWN(enCommMode, enCommModule, (PWC_COMM_MODEM_E)g_msp_pwrctrl.tlmode_channel.rf_channel_id);
            }
            break;
        case PWC_COMM_MODULE_ABB:
            if(g_msp_pwrctrl.tlmode_channel.abb_channel_id== 2)
            {
                DRV_PWRCTRL_PWRDOWN(enCommMode, enCommModule,  PWC_COMM_MODEM_0);
                DRV_PWRCTRL_PWRDOWN(enCommMode, enCommModule,  PWC_COMM_MODEM_1);
            }
            else
            {
                DRV_PWRCTRL_PWRDOWN(enCommMode, enCommModule, (PWC_COMM_MODEM_E)g_msp_pwrctrl.tlmode_channel.abb_channel_id);
            }
            break;
#endif
        default:
            mspsleep_print_error("check your input enCommMode:%d,  enCommModule: %d\n", enCommMode, enCommModule);
    }
}

/*****************************************************************************
 �� �� ��  : LSLEEP_RttForceAwake
 ��������  : ǿ�ƻ��ѽӿڣ����ͷ�sleep�����ź���
 �������  : void
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_VOID LSLEEP_RttForceAwake(VOS_VOID)
{
    VOS_INT power_lock = 0;
    char *pcAwakeRecordType = "force awake";

    mspsleep_print_info("%s", pcAwakeRecordType);

    power_lock = VOS_SplIMP();

    TLSLEEP_StateRecord(DRX_FORCE_AWAKE_API);

    if((read_power_status_bit(MSP_PWR_FORCE_AWAKE))||(read_power_status_bit(MSP_PWR_WAKEUP))
        ||(read_power_status_bit(MSP_PWR_WAIT_RESUMER)))
    {
        VOS_Splx(power_lock);
        TLSLEEP_StateRecord(DRX_FORCE_AWAKE_NOT_DEAL);
        return;
    }
    else if(read_power_status_bit(MSP_PWR_SYSTEM_RUN))
    {
        VOS_Splx(power_lock);
        BSP_MailBox_DspAwakeProc();
        TLSLEEP_StateRecord(DRX_FORCE_AWAKE_RUN_RETURN);
        return;
    }

    TLSLEEP_TimeRecord(pcAwakeRecordType);
    set_power_status_bit(MSP_PWR_FORCE_AWAKE);
    VOS_Splx(power_lock);
    VOS_SmV(g_msp_pwrctrl.task_sem);
    return;
}
/*****************************************************************************
 �� �� ��  : TLSLEEP_TCXOIsrProc
 ��������  : ��TCXOʱע��Ļص�����
 �������  : ��
 �������  : ��,
 �� �� ֵ  :
*****************************************************************************/
VOS_VOID TLSLEEP_TCXO0_IsrProc( VOS_VOID )
{
    VOS_SmV(g_msp_pwrctrl.SleepTcxoSem[0]);
}
VOS_VOID TLSLEEP_TCXO1_IsrProc( VOS_VOID )
{
    VOS_SmV(g_msp_pwrctrl.SleepTcxoSem[1]);
}

/*****************************************************************************
 �� �� ��  : TLSLEEP_OpenTCXO
 ��������  : ��TCXO
 �������  : ��
 �������  : ��,
 �� �� ֵ  : ��
*****************************************************************************/
VOS_VOID TLSLEEP_OpenTCXO(VOS_VOID )
{
    PWC_COMM_MODEM_E                    enModem = PWC_COMM_MODEM_0;
    VOS_UINT32                          ulTcxoTime = 0;
    VOS_UINT32                          ulTcxoResult = VOS_OK;
    VOS_INT                             ulTcxoStatus[2] = {0};
    VOS_INT                             power_lock = 0;

    TLSLEEP_DbgTimeRecord(TLSLEEP_OPENTCXO_0);

    if(g_msp_pwrctrl.tlmode_channel.tcxo_id == 2)
    {
        power_lock = VOS_SplIMP();
        ulTcxoStatus[0] = DRV_TCXO_GETSTATUS(PWC_COMM_MODEM_0);
        ulTcxoStatus[1] = DRV_TCXO_GETSTATUS(PWC_COMM_MODEM_1);
		DRV_TCXO_ENABLE(PWC_COMM_MODEM_0, (PWC_COMM_MODE_E)PWC_COMM_MODE_LTE);
        DRV_TCXO_ENABLE(PWC_COMM_MODEM_1, (PWC_COMM_MODE_E)PWC_COMM_MODE_LTE);
        TLSLEEP_DbgTimeRecord(TLSLEEP_OPENTCXO_1);
        if ((PWRCTRL_COMM_OFF == ulTcxoStatus[0])||(PWRCTRL_COMM_OFF == ulTcxoStatus[1]))
        {
            ulTcxoTime  = g_msp_pwrctrl.SleepDrxResumeTime.ulTcxoRsmTime/1000;

            /* ����TCXO�ȶ��ȴ���ʱ�� */
            DRV_TCXO_TIMER_START(PWC_COMM_MODEM_0, (PWC_COMM_MODE_E)PWC_COMM_MODE_LTE, (PWC_TCXO_FUNCPTR)TLSLEEP_TCXO0_IsrProc, VOS_NULL, ulTcxoTime);
            DRV_TCXO_TIMER_START(PWC_COMM_MODEM_1, (PWC_COMM_MODE_E)PWC_COMM_MODE_LTE, (PWC_TCXO_FUNCPTR)TLSLEEP_TCXO1_IsrProc, VOS_NULL, ulTcxoTime);
            VOS_Splx(power_lock);
            /* coverity[lock] */
            ulTcxoResult    = VOS_SmP(g_msp_pwrctrl.SleepTcxoSem[0], WAIT_FOR_EVER);
            /* coverity[lock] */
            ulTcxoResult    += VOS_SmP(g_msp_pwrctrl.SleepTcxoSem[1], WAIT_FOR_EVER);
            TLSLEEP_DbgTimeRecord(TLSLEEP_OPENTCXO_2);
            if (VOS_OK != ulTcxoResult)
            {
                /* �쳣�������� */
                TLSLEEP_StateRecord(DRX_FAIL_WAIT_TCXO_SEM);
                mspsleep_print_error("tcxo get sem fail");
            }
        }
        else
        {
            VOS_Splx(power_lock);
        }
        TLSLEEP_DbgTimeRecord(TLSLEEP_OPENTCXO_3);
#if (FEATURE_MULTI_CHANNEL == FEATURE_ON)
        DRV_TCXO_RFCLKENABLE(PWC_COMM_MODEM_0, PWC_COMM_MODE_LTE, PWC_COMM_CHANNEL_0);
        DRV_TCXO_RFCLKENABLE(PWC_COMM_MODEM_1, PWC_COMM_MODE_LTE, PWC_COMM_CHANNEL_0);
#else
        DRV_TCXO_RFCLKENABLE(PWC_COMM_MODEM_0, PWC_COMM_MODE_LTE);
        DRV_TCXO_RFCLKENABLE(PWC_COMM_MODEM_1, PWC_COMM_MODE_LTE);
#endif
        TLSLEEP_DbgTimeRecord(TLSLEEP_OPENTCXO_MAX);
    }
    else
    {
        enModem = (PWC_COMM_MODEM_E)g_msp_pwrctrl.tlmode_channel.tcxo_id;
        power_lock = VOS_SplIMP();
        DRV_TCXO_ENABLE((PWC_COMM_MODEM_E)enModem, (PWC_COMM_MODE_E)PWC_COMM_MODE_LTE);
        TLSLEEP_DbgTimeRecord(TLSLEEP_OPENTCXO_1);
        if (PWRCTRL_COMM_OFF == DRV_TCXO_GETSTATUS((PWC_COMM_MODEM_E)enModem))
        {

            ulTcxoTime  = g_msp_pwrctrl.SleepDrxResumeTime.ulTcxoRsmTime/1000;

            /* ����TCXO�ȶ��ȴ���ʱ�� */
            DRV_TCXO_TIMER_START(enModem, (PWC_COMM_MODE_E)PWC_COMM_MODE_LTE, (PWC_TCXO_FUNCPTR)TLSLEEP_TCXO0_IsrProc, VOS_NULL, ulTcxoTime);
            VOS_Splx(power_lock);
            /* �ȴ�TCXO�ȶ�,Take�ź�����Сʱ�䳤�ȱ���Ϊ1 tick */
            ulTcxoResult    = VOS_SmP(g_msp_pwrctrl.SleepTcxoSem[0], WAIT_FOR_EVER);

            TLSLEEP_DbgTimeRecord(TLSLEEP_OPENTCXO_2);
            if (VOS_OK != ulTcxoResult)
            {
                /* �쳣�������� */
                TLSLEEP_StateRecord(DRX_FAIL_WAIT_TCXO_SEM);
                mspsleep_print_error("tcxo get sem fail");
            }
        }
        else
        {
            VOS_Splx(power_lock);
        }
        TLSLEEP_DbgTimeRecord(TLSLEEP_OPENTCXO_3);
#if (FEATURE_MULTI_CHANNEL == FEATURE_ON)
        DRV_TCXO_RFCLKENABLE(enModem, PWC_COMM_MODE_LTE, PWC_COMM_CHANNEL_0);
#else
        DRV_TCXO_RFCLKENABLE(enModem, PWC_COMM_MODE_LTE);
#endif
        TLSLEEP_DbgTimeRecord(TLSLEEP_OPENTCXO_MAX);
    }
    /* coverity[missing_unlock] */
}
/*****************************************************************************
 �� �� ��  : TLSLEEP_CloseTCXO
 ��������  : �ر�TCXO
 �������  : ��
 �������  : ��,
 �� �� ֵ  : ��
*****************************************************************************/
VOS_VOID TLSLEEP_CloseTCXO(VOS_VOID )
{
    PWC_COMM_MODEM_E           		enModem = PWC_COMM_MODEM_0;
    if(g_msp_pwrctrl.tlmode_channel.tcxo_id == 2)
    {
#if (FEATURE_MULTI_CHANNEL == FEATURE_ON)
        DRV_TCXO_RFCLKDISABLE(PWC_COMM_MODEM_0, PWC_COMM_MODE_LTE, PWC_COMM_CHANNEL_0);
        DRV_TCXO_RFCLKDISABLE(PWC_COMM_MODEM_0, PWC_COMM_MODE_LTE, PWC_COMM_CHANNEL_0);
#else
        DRV_TCXO_RFCLKDISABLE(PWC_COMM_MODEM_0, PWC_COMM_MODE_LTE);
        DRV_TCXO_RFCLKDISABLE(PWC_COMM_MODEM_1, PWC_COMM_MODE_LTE);
#endif
        DRV_TCXO_DISABLE(PWC_COMM_MODEM_0, PWC_COMM_MODE_LTE);
        DRV_TCXO_DISABLE(PWC_COMM_MODEM_1, PWC_COMM_MODE_LTE);
    }
    else
    {
    	enModem = (PWC_COMM_MODEM_E)g_msp_pwrctrl.tlmode_channel.tcxo_id;
#if (FEATURE_MULTI_CHANNEL == FEATURE_ON)
        DRV_TCXO_RFCLKDISABLE(enModem, PWC_COMM_MODE_LTE, PWC_COMM_CHANNEL_0);
#else
        DRV_TCXO_RFCLKDISABLE(enModem, PWC_COMM_MODE_LTE);
#endif
        DRV_TCXO_DISABLE(enModem, PWC_COMM_MODE_LTE);
    }
}



#if (FEATURE_MULTI_CHANNEL == FEATURE_ON)

VOS_VOID TLSLEEP_EnableRFTCXO(VOS_VOID )
{
    PWC_COMM_MODEM_E                    enModem = PWC_COMM_MODEM_0;

    TLSLEEP_DbgTimeRecord(TLSLEEP_OPENTCXO_0);

    if(g_msp_pwrctrl.tlmode_channel.tcxo_id == 2)
    {
#if defined(FEATURE_MULTI_CHANNEL)
        DRV_TCXO_RFCLKENABLE(PWC_COMM_MODEM_0, PWC_COMM_MODE_LTE, PWC_COMM_CHANNEL_0);
        DRV_TCXO_RFCLKENABLE(PWC_COMM_MODEM_1, PWC_COMM_MODE_LTE, PWC_COMM_CHANNEL_0);
#else
        DRV_TCXO_RFCLKENABLE(PWC_COMM_MODEM_0, PWC_COMM_MODE_LTE);
        DRV_TCXO_RFCLKENABLE(PWC_COMM_MODEM_1, PWC_COMM_MODE_LTE);
#endif
        TLSLEEP_DbgTimeRecord(TLSLEEP_OPENTCXO_MAX);
    }
    else
    {
#if defined(FEATURE_MULTI_CHANNEL)
        DRV_TCXO_RFCLKENABLE(enModem, PWC_COMM_MODE_LTE, PWC_COMM_CHANNEL_0);
#else
        DRV_TCXO_RFCLKENABLE(enModem, PWC_COMM_MODE_LTE);
#endif
        TLSLEEP_DbgTimeRecord(TLSLEEP_OPENTCXO_MAX);
    }
    /* coverity[missing_unlock] */
}

VOS_VOID TLSLEEP_DisableRFTCXO(VOS_VOID )
{
    PWC_COMM_MODEM_E           		enModem = PWC_COMM_MODEM_0;
    if(g_msp_pwrctrl.tlmode_channel.tcxo_id == 2)
    {
#if defined(FEATURE_MULTI_CHANNEL)
        DRV_TCXO_RFCLKDISABLE(PWC_COMM_MODEM_0, PWC_COMM_MODE_LTE, PWC_COMM_CHANNEL_0);
        DRV_TCXO_RFCLKDISABLE(PWC_COMM_MODEM_0, PWC_COMM_MODE_LTE, PWC_COMM_CHANNEL_0);
#else
        DRV_TCXO_RFCLKDISABLE(PWC_COMM_MODEM_0, PWC_COMM_MODE_LTE);
        DRV_TCXO_RFCLKDISABLE(PWC_COMM_MODEM_1, PWC_COMM_MODE_LTE);
#endif
    }
    else
    {
    	enModem = (PWC_COMM_MODEM_E)g_msp_pwrctrl.tlmode_channel.tcxo_id;
#if defined(FEATURE_MULTI_CHANNEL)
        DRV_TCXO_RFCLKDISABLE(enModem, PWC_COMM_MODE_LTE, PWC_COMM_CHANNEL_0);
#else
        DRV_TCXO_RFCLKDISABLE(enModem, PWC_COMM_MODE_LTE);
#endif
    }
}

/*****************************************************************************
 �� �� ��  : TLSLEEP_HwPowerUp
 ��������  : ΪRTT��������ϵ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : ERR_MSP_SUCCESS �ɹ�
*****************************************************************************/
VOS_INT32 TLSLEEP_HwPowerUp(PWC_COMM_MODE_E modeId)
{
    PWC_COMM_MODEM_E                enModem = PWC_COMM_MODEM_0;
    PWC_COMM_CHANNEL_E              enChannel = PWC_COMM_CHANNEL_0;
#if 0
    VOS_INT32                       ret = 0;
    VOS_INT                         power_lock = 0;
#endif
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_0);
    TLSLEEP_DelayMs(5, g_msp_drx_delay.msp_0);
    /*DDR������Ƶ����СֵΪ150M,V9R1ʹ��*/
#if 0
    ret += TLSLEEP_DfsQosUpdate((VOS_INT32)DFS_QOS_ID_BUS_MINFREQ_E, dfs_bus_req_id,TLSLEEP_DFS_BUS_150M);
    ret += TLSLEEP_DfsQosUpdate((VOS_INT32)DFS_QOS_ID_DDR_MINFREQ_E, dfs_ddr_req_id,TLSLEEP_DFS_DDR_150M);
    ret += TLSLEEP_DfsQosUpdate((VOS_INT32)DFS_QOS_ID_CCPU_MINFREQ_E, dfs_ccpu_req_id,TLSLEEP_DFS_CCPU_600M);
    if(ret)
    {
        power_lock = VOS_SplIMP();/* [false alarm]:����Fortify */
        set_power_status_bit(MSP_DFS_RUN_UPDATE_FAIL);
        VOS_Splx(power_lock);
    }
#endif
    TLSLEEP_DelayMs(5, g_msp_drx_delay.msp_1);
    if(POWER_SAVING_DEEP_SLEEP == g_msp_pwrctrl.dsp_sleep_flag)
    {
        /* V8R1��ʹ�õ�DCXO������ҪMSP����TCXO���ɵ������Σ�MSP���ִ���Ĺ�һ */
        DRV_PWRCTRL_PWRUP(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBE16, enModem, enChannel);

        TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_1);
        DRV_PWRCTRL_PLLENABLE(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBE16, enModem, enChannel);
        DRV_PWRCTRL_PLLENABLE(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBE16, enModem, enChannel);
        TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_2);
        if(DRV_EDMA_DSP_MemRestore())
        {
            TLSLEEP_StateRecord(RESTORE_DSP_TCM_FAIL);
            mspsleep_print_error("DSP TCM restore fail");
        }
    }
    else if(POWER_SAVING_SNOOZE == g_msp_pwrctrl.dsp_sleep_flag)
    {
        DRV_PWRCTRL_DSP_CLKENABLE();
        TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_1);
        DRV_PWRCTRL_DSP_UNRESET();
    }
    TLSLEEP_DelayMs(5, g_msp_drx_delay.msp_2);
    /* ABB PLLʹ��ǰ���ϵ� */
    TLSLEEP_PwrctrlUp(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB);

    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_2);

    /* BBP�ϵ����ǰҪ��ʹ��PLL(PLL����BBPǰ��) */
    TLSLEEP_PllEnable(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB);

    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_3);

    /* V8R1 BBP��ABB����PLL���˽ӿ��ڵ����д�׮ */
    DRV_PWRCTRL_PLLENABLE(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem, enChannel);

    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_4);

    /* TDS BBP�ϵ磬LTE BBP�ϵ���DSP���� */
    DRV_PWRCTRL_PWRUP(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBP_DRX, enModem, enChannel);
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_5);

    /* TDS BBPʱ��ʹ�ܣ�LTE BBPʱ��ʹ����DSP���� */
    DRV_PWRCTRL_BBPCLKENABLE(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBP_DRX, enModem, enChannel);

    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_6);
    TLSLEEP_DelayMs(5, g_msp_drx_delay.msp_3);
    /*RF�ϵ�*/
    TLSLEEP_PwrctrlUp(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_RF);

        TLSLEEP_EnableRFTCXO();

    GetRttSleepInfo(ulRfTime) = PWRCTRL_GetSleepSlice();
    VOS_FlushCpuWriteBuf();
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_7);
    if(POWER_SAVING_DEEP_SLEEP == g_msp_pwrctrl.dsp_sleep_flag)
    {
        DRV_EDMA_DSP_StateGet();
    }

    TLSLEEP_DelayMs(5, g_msp_drx_delay.msp_4);
    TLSLEEP_RunDsp();
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_MAX);

    return ERR_MSP_SUCCESS;
}

/*****************************************************************************
 �� �� ��  : TLSLEEP_HwPowerDown
 ��������  : ΪRTT��������µ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : ERR_MSP_SUCCESS �ɹ�
*****************************************************************************/
VOS_INT32 TLSLEEP_HwPowerDown(PWC_COMM_MODE_E modeId)
{
    PWC_COMM_MODEM_E                enModem = PWC_COMM_MODEM_0;
    PWC_COMM_CHANNEL_E              enChannel = PWC_COMM_CHANNEL_0;
#if 0
    VOS_UINT32                      ret = 0;
    VOS_INT32                       flag = 0;
#endif
    enModem = PWC_COMM_MODEM_0;

    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_0);
    DRV_PWRCTRL_DSP_HOLD();
    /*DDR������Ƶ����Сֵ����Ϊ0,V9R1ʹ��*/
#if 0
    ret += TLSLEEP_DfsQosUpdate((VOS_INT32)DFS_QOS_ID_BUS_MINFREQ_E, dfs_bus_req_id, TLSLEEP_DFS_BUS_75M);
    ret += TLSLEEP_DfsQosUpdate((VOS_INT32)DFS_QOS_ID_DDR_MINFREQ_E, dfs_ddr_req_id, TLSLEEP_DFS_DDR_75M);
    ret += TLSLEEP_DfsQosUpdate((VOS_INT32)DFS_QOS_ID_CCPU_MINFREQ_E, dfs_ccpu_req_id, TLSLEEP_DFS_CCPU_150M);
    if(ret)
    {
        flag = VOS_SplIMP();
        set_power_status_bit(MSP_DFS_HALT_UPDATE_FAIL);
        VOS_Splx(flag);
    }
#endif
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_1);

    if(POWER_SAVING_DEEP_SLEEP == g_msp_pwrctrl.dsp_sleep_flag)
    {
        /*������DSP�ϵ��ʱ����ܽ��б��棬�����ǲ�������������̵�*/
        if (DRV_EDMA_DSP_MemStore())
        {
           TLSLEEP_StateRecord(STORE_DSP_TCM_FAIL);
           mspsleep_print_error("DSP TCM store fail");
        }
    }
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_2);
    /*RF�µ�*/
        TLSLEEP_DisableRFTCXO();
    TLSLEEP_PwrctrlDown(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_RF);

    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_3);

    /* TDS BBPʱ��ȥʹ�ܣ�LTE BBPʱ��ȥʹ����DSP���� */
    DRV_PWRCTRL_BBPCLKDISABLE(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBP_DRX, enModem, enChannel);
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_4);

    /* TDS BBP�µ磬LTE BBP�µ���DSP���� */
    DRV_PWRCTRL_PWRDOWN(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBP_DRX, enModem, enChannel);
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_5);

    /* BBP PLL�ر�(V8R1�е�DCXO��BBP��PLLʹ��ABB�е�PLL���˽ӿڵ����׮) */
    DRV_PWRCTRL_PLLDISABLE(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem, enChannel);

    /* ABB PLLȥʹ�� */
    TLSLEEP_PllDisable(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB);

    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_6);

    /* ABB PLL�µ� */
    TLSLEEP_PwrctrlDown(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB);

    if(POWER_SAVING_DEEP_SLEEP == g_msp_pwrctrl.dsp_sleep_flag)
    {
        TLSLEEP_StateRecord(DSP_DEEP_SLEEP_COUNT);
        DRV_EDMA_DSP_StateGet();
        DRV_PWRCTRL_PLLDISABLE(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBE16, enModem, enChannel);
        DRV_PWRCTRL_PLLDISABLE(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBE16, enModem, enChannel);
        DRV_PWRCTRL_PWRDOWN(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBE16, enModem, enChannel);

        /* V8R1��ʹ�õ�DCXO������ҪMSP����TCXO���ɵ������Σ�MSP���ִ���Ĺ�һ */
    }
    else if(POWER_SAVING_SNOOZE == g_msp_pwrctrl.dsp_sleep_flag)
    {
        TLSLEEP_StateRecord(DSP_LIGHT_SLEEP_COUNT);
        DRV_PWRCTRL_DSP_RESET();
        /*BBE16 ����*/
        DRV_PWRCTRL_DSP_CLKDISABLE();
    }
    else
    {
        TLSLEEP_StateRecord(DSP_SLEEP_FLAG_ERROR);
    }
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_MAX);

    return ERR_MSP_SUCCESS;
}

#elif defined(MSP_IN_V9R1)
/*****************************************************************************
 �� �� ��  : TLSLEEP_HwPowerUp
 ��������  : ΪRTT��������ϵ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : ERR_MSP_SUCCESS �ɹ�
*****************************************************************************/
VOS_INT32 TLSLEEP_HwPowerUp(PWC_COMM_MODE_E modeId)
{
    VOS_UINT32                			enModem;
    VOS_INT32                           ret = 0;
    VOS_INT32                           power_lock = 0;
	enModem = PWC_COMM_MODEM_0;

	TLSLEEP_OpenTCXO();

    DRV_PWRCTRL_PWRUP(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBE16, enModem);
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_0);

    if(DRV_EDMA_DSP_MemRestore())
    {
        mspsleep_print_error("DSP TCM restore fail");
        TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_1);
        TLSLEEP_StateRecord(RESTORE_DSP_TCM_FAIL);
        return MSP_SLEEP_ERROR;
    }

    /*DDR������Ƶ����СֵΪ150M,V9R1ʹ��*/
    ret += TLSLEEP_DfsQosUpdate((VOS_INT32)DFS_QOS_ID_BUS_MINFREQ_E, dfs_bus_req_id,TLSLEEP_DFS_BUS_150M);
    ret += TLSLEEP_DfsQosUpdate((VOS_INT32)DFS_QOS_ID_DDR_MINFREQ_E, dfs_ddr_req_id,TLSLEEP_DFS_DDR_150M);
    ret += TLSLEEP_DfsQosUpdate((VOS_INT32)DFS_QOS_ID_CCPU_MINFREQ_E, dfs_ccpu_req_id,TLSLEEP_DFS_CCPU_600M);
    if(ret)
    {
        power_lock = VOS_SplIMP();
        set_power_status_bit(MSP_DFS_RUN_UPDATE_FAIL);
        VOS_Splx(power_lock);
    }
    TLSLEEP_PwrctrlUp(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB);
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_2);
    /*BBP�ϵ�*/
    DRV_PWRCTRL_PWRUP(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBP_DRX, enModem);
    DRV_PWRCTRL_PWRUP(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem);
    /*��TDS ʹ�õ�BBP 1A����ʱ��*/
    DRV_PWRCTRL_BBPCLKENABLE(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBP_DRX, enModem);
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_3);
    DRV_PWRCTRL_PLLENABLE(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem);
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_4);
    /*ͨ��NV���ƹر��ĸ�ͨ��*/
    TLSLEEP_PllEnable(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB);
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_5);
    /*�����ϵ�*/
    DRV_PWRCTRL_PWRUP(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_PA, enModem);
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_6);
    /*RF�ϵ�*/
    TLSLEEP_PwrctrlUp(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_RF);
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_7);

    DRV_EDMA_DSP_StateGet();
	if(g_msp_pwrctrl.LteIntTimer)
	{
		DRV_BBP_INT_TIMER_ENABLE();
	}
	(VOS_VOID)DRV_BBP_INT_TDS_TF_ENABLE();
    TLSLEEP_RunDsp();
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_MAX);

    return ERR_MSP_SUCCESS;
}
/*****************************************************************************
 �� �� ��  : TLSLEEP_HwPowerDown
 ��������  : ΪRTT��������µ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : ERR_MSP_SUCCESS �ɹ�
*****************************************************************************/
VOS_INT32 TLSLEEP_HwPowerDown(PWC_COMM_MODE_E modeId)
{
    VOS_UINT32                          enModem = 0;
    VOS_UINT32                          ret = 0;
    VOS_INT32                           flag = 0;

	enModem = PWC_COMM_MODEM_0;

    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_0);

    DRV_PWRCTRL_DSP_HOLD();
	DRV_BBP_INT_TDS_TF_DISABLE();
	/*LTE BBP timer int*/
	if(BSP_OK != DRV_BBP_INT_TIMER_SWITCH_STATUS(&g_msp_pwrctrl.LteIntTimer))
	{
		g_msp_pwrctrl.LteIntTimerFail++;
	}
	DRV_BBP_INT_TIMER_DISABLE();
    /*DDR������Ƶ����Сֵ����Ϊ0,V9R1ʹ��*/
    ret += TLSLEEP_DfsQosUpdate((VOS_INT32)DFS_QOS_ID_BUS_MINFREQ_E, dfs_bus_req_id, TLSLEEP_DFS_BUS_75M);
    ret += TLSLEEP_DfsQosUpdate((VOS_INT32)DFS_QOS_ID_DDR_MINFREQ_E, dfs_ddr_req_id, TLSLEEP_DFS_DDR_75M);
    ret += TLSLEEP_DfsQosUpdate((VOS_INT32)DFS_QOS_ID_CCPU_MINFREQ_E, dfs_ccpu_req_id, TLSLEEP_DFS_CCPU_150M);
    if(ret)
    {
        flag = VOS_SplIMP();
        set_power_status_bit(MSP_DFS_HALT_UPDATE_FAIL);
        VOS_Splx(flag);
    }

    /*������DSP�ϵ��ʱ����ܽ��б��棬�����ǲ�������������̵�*/
    if (DRV_EDMA_DSP_MemStore())
    {
        mspsleep_print_error("DSP TCM store fail");
        TLSLEEP_StateRecord(STORE_DSP_TCM_FAIL);
        return MSP_SLEEP_ERROR;
    }
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_1);
	DRV_EDMA_DSP_StateGet();
	/*BBE 16 �µ�*/
	DRV_PWRCTRL_PWRDOWN(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBE16, enModem);
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_2);
    /*RF�µ�*/
    TLSLEEP_PwrctrlDown(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_RF);
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_3);
    /*�����µ�*/
    DRV_PWRCTRL_PWRDOWN(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_PA, enModem);
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_4);
    TLSLEEP_PllDisable(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB);
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_5);

    /*BBPʱ�ӹر�*/
    DRV_PWRCTRL_PLLDISABLE(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem);
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_6);
    /*�ر�TDS ʹ�õ�BBP 1A����ʱ��*/
    DRV_PWRCTRL_BBPCLKDISABLE(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBP_DRX, enModem);
    /*BBP�µ�*/
    DRV_PWRCTRL_PWRDOWN(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem);
    DRV_PWRCTRL_PWRDOWN(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBP_DRX, enModem);
    TLSLEEP_PwrctrlDown(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB);
    TLSLEEP_CloseTCXO();
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_MAX);
    return ERR_MSP_SUCCESS;
}

#else
/*****************************************************************************
 �� �� ��  : TLSLEEP_CheckWakeTime
 ��������  : ��⵱ǰ����DSP�Ƿ�ʱ,�����ʱ���д�����
 �������  : ��
 �������  : ��
 �� �� ֵ  :
*****************************************************************************/
static inline VOS_VOID TLSLEEP_CheckWakeTime()
{
    VOS_UINT32 sleep_time = *(VOS_UINT32*)(HI_BBP_LTEDRX_BASE_ADDR + HI_BBP_LTEDRX_SLP_TIME_CUR_OFFSET);
    VOS_UINT32 wake_time = *(VOS_UINT32*)(HI_BBP_LTEDRX_BASE_ADDR + HI_BBP_LTEDRX_WAKEUP_32K_CNT_OFFSET);
    VOS_UINT32 clk_time = *(VOS_UINT32*)(HI_BBP_LTEDRX_BASE_ADDR + HI_BBP_LTEDRX_SWITCH_32K_CNT_OFFSET);
    VOS_UINT32 i = 0;
    /*���Ϊ��ģ,���һ��ѳ�ʱ�ͽ��м�¼*/
    if(((sleep_time + g_msp_pwrctrl.exc_overtime.timeOut) > (wake_time + clk_time))&&(clk_time > 163)\
        &&(0 == g_msp_pwrctrl.exc_overtime.ulReadMark))
    {
        for(i = TLSLEEP_DSPHALTISR_0; i < TLSLEEP_TIME_RECORD_MAX; i++)
        {
            g_msp_pwrctrl.exc_overtime.ulTime[i] = g_msp_pwrctrl.dbg_time_record[i];
        }
        g_msp_pwrctrl.exc_overtime.ulReadMark = 1;
        VOS_SmV(g_msp_pwrctrl.exc_overtime.time_sem);
    }
    return;
}

VOS_VOID TLSLEEP_DebugRecordTime(VOS_VOID)
{
    int i = 0;
    VOS_UINT32* axi_time = &g_msp_pwrctrl.dbg_time_record[TLSLEEP_AWAKEISR_0];
    VOS_UINT32* ddr_time = (VOS_UINT32*)DEBUG_TIMESTAMP_ADDR;

    TLSLEEP_CheckWakeTime();
    if((*(VOS_UINT32*)(SHM_TIMESTAMP_ADDR + 4)) == 1)
    {
        *(VOS_UINT32*)(SHM_TIMESTAMP_ADDR + 4) = 2;
        *(VOS_UINT32*)(SHM_TIMESTAMP_ADDR + 8) = 1;
    }
    else
    {
        return;
    }
    /*0x5A5A5A5A~0x5B5B5B5B,Ϊ�ϵ�����еĵ�*/
    *(ddr_time++) = 0x5A5A5A5A;
    for(i = TLSLEEP_AWAKEISR_0; i <= TLSEEP_HWPOWERUP_7; i++)
    {
        *(ddr_time++) = *(axi_time++);
    }
    /*0x5B5B5B5B~0x5C5C5C5C,Ϊ�µ�����еĵ�*/
    *(ddr_time++) = 0x5B5B5B5B;
    axi_time =  &g_msp_pwrctrl.dbg_time_record[TLSLEEP_DSPHALTISR_0];
    for(i = TLSLEEP_DSPHALTISR_0; i <= TLSLEEP_RTTSLEEP_INNER_MAX; i++)
    {
        *(ddr_time++) = *(axi_time++);
    }
    *(ddr_time++) = 0x5C5C5C5C;
}

VOS_VOID TLSLEEP_PWRCTRL_PWRUP(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
    DRV_PWRCTRL_DSP_CLKENABLE();
    DRV_PWRCTRL_DSP_RESET();
    DRV_PWRCTRL_DSP_UNRESET();
}
VOS_VOID TLSLEEP_PWRCTRL_PWRDOWN(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
    DRV_PWRCTRL_DSP_CLKDISABLE();
}

/*****************************************************************************
 �� �� ��  : TLSLEEP_HwPowerUp
 ��������  : ΪRTT��������ϵ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : ERR_MSP_SUCCESS �ɹ�
*****************************************************************************/
VOS_INT32 TLSLEEP_HwPowerUp(PWC_COMM_MODE_E modeId)
{
    PWC_COMM_MODEM_E       			enModem = PWC_COMM_MODEM_0;
    VOS_INT32                       ret = 0;
    VOS_INT                         power_lock = 0;
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_0);
    TLSLEEP_DelayMs(5, g_msp_drx_delay.msp_0);
    ret += TLSLEEP_DfsQosUpdate((VOS_INT32)DFS_QOS_ID_DDR_MINFREQ_E, dfs_ddr_req_id, TLSLEEP_DFS_DDR_75M);/* [false alarm]:����Fortify */
    if(ret)
    {
        power_lock = VOS_SplIMP();/* [false alarm]:����Fortify */
        set_power_status_bit(MSP_DFS_RUN_UPDATE_FAIL);
        VOS_Splx(power_lock);
    }
    TLSLEEP_DelayMs(5, g_msp_drx_delay.msp_1);
    if(POWER_SAVING_DEEP_SLEEP == g_msp_pwrctrl.dsp_sleep_flag)
    {
    	TLSLEEP_OpenTCXO();
        TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_1);
        DRV_PWRCTRL_PLLENABLE(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBE16, enModem);
        DRV_PWRCTRL_PLLENABLE(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBE16, enModem);
        TLSLEEP_PWRCTRL_PWRUP(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBE16, enModem);
        TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_2);
        if(DRV_EDMA_DSP_MemRestore())
        {
            TLSLEEP_StateRecord(RESTORE_DSP_TCM_FAIL);
            mspsleep_print_error("DSP TCM restore fail");
        }
    }
    else if(POWER_SAVING_SNOOZE == g_msp_pwrctrl.dsp_sleep_flag)
    {
        DRV_PWRCTRL_DSP_CLKENABLE();
        TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_1);
        /*V7R2�Ż�����,��λ�⸴λ����һ��*/
        DRV_PWRCTRL_DSP_RESET();
        DRV_PWRCTRL_DSP_UNRESET();
        TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_2);
    }
    TLSLEEP_DelayMs(5, g_msp_drx_delay.msp_2);
    TLSLEEP_PwrctrlUp(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB);
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_3);
    /*BBP�ϵ�*/
    DRV_PWRCTRL_PWRUP(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBP_DRX, enModem);
    DRV_PWRCTRL_PWRUP(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem);
    /*��TDS ʹ�õ�BBP 1A����ʱ��*/
    DRV_PWRCTRL_BBPCLKENABLE(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBP_DRX, enModem);
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_4);
    DRV_PWRCTRL_PLLENABLE(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem);
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_5);
    /*ͨ��NV���ƹر��ĸ�ͨ��*/
    TLSLEEP_PllEnable(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB);
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_6);
    TLSLEEP_DelayMs(5, g_msp_drx_delay.msp_3);
    /*RF�ϵ�*/
    TLSLEEP_PwrctrlUp(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_RF);
    GetRttSleepInfo(ulRfTime) = PWRCTRL_GetSleepSlice();
    VOS_FlushCpuWriteBuf();
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_7);
    if(POWER_SAVING_DEEP_SLEEP == g_msp_pwrctrl.dsp_sleep_flag)
    {
        DRV_EDMA_DSP_StateGet();
    }
    TLSLEEP_DebugRecordTime();
    TLSLEEP_DelayMs(5, g_msp_drx_delay.msp_4);
    TLSLEEP_RunDsp();
    TLSLEEP_DbgTimeRecord(TLSEEP_HWPOWERUP_MAX);

    return ERR_MSP_SUCCESS;
}
/*****************************************************************************
 �� �� ��  : TLSLEEP_HwPowerDown
 ��������  : ΪRTT��������µ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : ERR_MSP_SUCCESS �ɹ�
*****************************************************************************/
VOS_INT32 TLSLEEP_HwPowerDown(PWC_COMM_MODE_E modeId)
{
    PWC_COMM_MODEM_E       			enModem = PWC_COMM_MODEM_0;

	enModem = PWC_COMM_MODEM_0;

    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_0);
    DRV_PWRCTRL_DSP_HOLD();
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_1);
    if(POWER_SAVING_DEEP_SLEEP == g_msp_pwrctrl.dsp_sleep_flag)
    {
        /*������DSP�ϵ��ʱ����ܽ��б��棬�����ǲ�������������̵�*/
        if (DRV_EDMA_DSP_MemStore())
        {
           TLSLEEP_StateRecord(STORE_DSP_TCM_FAIL);
           mspsleep_print_error("DSP TCM store fail");
        }
    }
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_2);
    /*RF�µ�*/
    TLSLEEP_PwrctrlDown(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_RF);
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_3);
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_4);
    TLSLEEP_PllDisable(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB);
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_5);
    /*BBPʱ�ӹر�*/
    DRV_PWRCTRL_PLLDISABLE(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem);
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_6);
    /*�ر�TDS ʹ�õ�BBP 1A����ʱ��*/
    DRV_PWRCTRL_BBPCLKDISABLE(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBP_DRX, enModem);
    /*BBP�µ�*/
    DRV_PWRCTRL_PWRDOWN(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem);
    DRV_PWRCTRL_PWRDOWN(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBP_DRX, enModem);
    TLSLEEP_PwrctrlDown(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB);
    if(POWER_SAVING_DEEP_SLEEP == g_msp_pwrctrl.dsp_sleep_flag)
    {
        TLSLEEP_StateRecord(DSP_DEEP_SLEEP_COUNT);
        DRV_EDMA_DSP_StateGet();
        TLSLEEP_PWRCTRL_PWRDOWN(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBE16, enModem);
        DRV_PWRCTRL_PLLDISABLE(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBE16, enModem);
        DRV_PWRCTRL_PLLDISABLE(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBE16, enModem);
        TLSLEEP_CloseTCXO();
    }
    else if(POWER_SAVING_SNOOZE == g_msp_pwrctrl.dsp_sleep_flag)
    {
        TLSLEEP_StateRecord(DSP_LIGHT_SLEEP_COUNT);
        /*BBE16 ����*/
        DRV_PWRCTRL_DSP_CLKDISABLE();
    }
    else
    {
        TLSLEEP_StateRecord(DSP_SLEEP_FLAG_ERROR);
    }
    TLSLEEP_DbgTimeRecord(TLSLEEP_HWPOWERDOWN_MAX);
    return ERR_MSP_SUCCESS;
}

#endif
/*****************************************************************************
 �� �� ��  : TLSLEEP_RttForceAwake_Inner
 ��������  : ���ѹ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_INT32 TLSLEEP_RttForceAwake_Inner(VOS_VOID)
{
    VOS_INT power_lock = 0;
    char *pcAwakeRecordType = "start wake";
    char *pcCompleteRecordType = "Complete wake";

    /*ͶƱ���ܽ���˯��*/
    DRV_PWRCTRL_SLEEPVOTE_LOCK(PWRCTRL_SLEEP_TLPS);
    if(0 != ((g_msp_delay.delay_control)&DELAY_CTRL_BEFOR_WAKE))
    {
        TLSLEEP_DelayMs(g_msp_delay.delay_ms, 1);
    }
    mspsleep_print_info("%s", pcAwakeRecordType);
    TLSLEEP_DbgTimeRecord(TLSLEEP_FORCE_AWAKE_INNER_0);
    TLSLEEP_TimeRecord(pcAwakeRecordType);

    if(TLSLEEP_HwPowerUp((PWC_COMM_MODE_E)PWC_COMM_MODE_LTE))
    {
        mspsleep_print_error("sleep power up failed!");
        return MSP_SLEEP_ERROR;
    }
    if(0 != ((g_msp_delay.delay_control)&DELAY_CTRL_AFTER_SLEEP))
    {
        TLSLEEP_DelayMs(g_msp_delay.delay_ms, 1);
    }
    TLSLEEP_DbgTimeRecord(TLSLEEP_FORCE_AWAKE_INNER_1);

    power_lock = VOS_SplIMP();
    clear_power_status_bit(MSP_PWR_WAKEUP);
    clear_power_status_bit( MSP_PWR_FORCE_AWAKE);
    VOS_Splx(power_lock);
    TLSLEEP_TimeRecord(pcCompleteRecordType);
    mspsleep_print_info("%s", pcCompleteRecordType);
    return ERR_MSP_SUCCESS;
}
/*****************************************************************************
 �� �� ��  : TLSLEEP_RttSleep_Inner
 ��������  : ��������
 �������  :
 �������  :
 �� �� ֵ  :  VOS_VOID
*****************************************************************************/
VOS_VOID TLSLEEP_RttSleep_Inner(VOS_VOID)
{
    /*lint -save -e958*/
    BSP_BOOL bMeasFlag = VOS_FALSE;
    VOS_INT power_lock = 0;
    char *pcSleepRecordType = "start sleep";
    char *pcCompleteRecordType = "Complete sleep";
    char *pcResumeDspType = "Resme dsp";
    /*lint -restore*/
    if(0 != ((g_msp_delay.delay_control)&DELAY_CTRL_BEFOR_SLEEP))
    {
        TLSLEEP_DelayMs(g_msp_delay.delay_ms, 1);
    }
    TLSLEEP_TimeRecord(pcSleepRecordType);
    mspsleep_print_info("%s", pcSleepRecordType);
    TLSLEEP_DbgTimeRecord(TLSLEEP_RTTSLEEP_INNER_0);

    /*��������Ƿ�����˯�ߣ�������䲻����˯���򲻽���˯�ߺ��µ磬ֱ�ӽ⸴λDSP*/
    /*TRUE-allow;FALSE-forbid trueΪ1 false Ϊfalse*/
    power_lock = VOS_SplIMP();
    bMeasFlag = BSP_MailBox_IsAllowDspSleep();
    g_msp_pwrctrl.ForbidDspSleepValue= (read_power_status_bit(MSP_PWR_FORCE_AWAKE))|(read_power_status_bit(MSP_PWR_WAKEUP))| (!bMeasFlag); /*lint !e514*/
    TLSLEEP_DbgTimeRecord(TLSLEEP_RTTSLEEP_INNER_1);
    if(g_msp_pwrctrl.ForbidDspSleepValue)
    {
        clear_power_status_bit(MSP_PWR_WAKEUP);
        clear_power_status_bit(MSP_PWR_FORCE_AWAKE);
        clear_power_status_bit(MSP_PWR_HALT_INT);
        clear_power_status_bit(MSP_PWR_SYSTEM_RUN);
        clear_power_status_bit(MSP_PWR_WAIT_RESUMER);
        TLSLEEP_ResumeDsp();
        TLSLEEP_StateRecord(SLEEP_TASK_NOT_NEED_DEAL);
        VOS_Splx(power_lock);
        TLSLEEP_TimeRecord(pcResumeDspType);
        mspsleep_print_info("%s", pcResumeDspType);
        return;
    }
    else
    {
        VOS_Splx(power_lock);
    }
    TLSLEEP_HwPowerDown(PWC_COMM_MODE_LTE);
    TLSLEEP_DbgTimeRecord(TLSLEEP_RTTSLEEP_INNER_2);

    power_lock = VOS_SplIMP();
    clear_power_status_bit(MSP_PWR_HALT_INT);
    clear_power_status_bit(MSP_PWR_SYSTEM_RUN);
    clear_power_status_bit(MSP_PWR_WAIT_RESUMER);
    if((!read_power_status_bit(MSP_PWR_FORCE_AWAKE))&&(!read_power_status_bit(MSP_PWR_WAKEUP)))
    {
        VOS_Splx(power_lock);

        /*ͶƱ˯��*/
        if(0 != ((g_msp_delay.delay_control)&DELAY_CTRL_AFTER_SLEEP))
        {
            TLSLEEP_DelayMs(g_msp_delay.delay_ms, 1);
        }
#ifndef MSP_IN_V9R1
        if(((ENUM_YES_VOTE_SLEEP == (VOS_INT)g_msp_pwrctrl.SleepDrxResumeTime.ulVoteSwitch)
            &&(POWER_SAVING_SNOOZE == g_msp_pwrctrl.dsp_sleep_flag))
			||(POWER_SAVING_DEEP_SLEEP == g_msp_pwrctrl.dsp_sleep_flag))
#endif
        {
            DRV_PWRCTRL_SLEEPVOTE_UNLOCK(PWRCTRL_SLEEP_TLPS);
	        TLSLEEP_StateRecord(SLEEP_VOTE_UNLOCK);
        }
    }
    else
    {
        VOS_Splx(power_lock);
    }
    TLSLEEP_DbgTimeRecord(TLSLEEP_RTTSLEEP_INNER_MAX);
    TLSLEEP_TimeRecord(pcCompleteRecordType);
    mspsleep_print_info("%s", pcCompleteRecordType);
}
/*****************************************************************************
 �� �� ��  : TLSLEEP_SleepTask
 ��������  : msp˯���������ں���
 �������  : void
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_VOID TLSLEEP_SleepTask(VOS_UINT32 Para0, VOS_UINT32 Para1,
                                VOS_UINT32 Para2, VOS_UINT32 Para3 )
{
    VOS_INT power_lock = 0;

    for (;;)
    {
        /* coverity[lock] */
        if(VOS_OK != VOS_SmP(g_msp_pwrctrl.task_sem, WAIT_FOR_EVER))
        {
            mspsleep_print_error("take task sem error!");
            /* coverity[missing_unlock] */
            return;
        }

        TLSLEEP_StateRecord(DRX_SLEEP_TASK);
        power_lock = VOS_SplIMP();
        /*�����ǰ��halt �ж�*/
        if(read_power_status_bit(MSP_PWR_HALT_INT))
        {
            VOS_Splx(power_lock);
            TLSLEEP_RttSleep_Inner();
        }
        else if(read_power_status_bit(MSP_PWR_FORCE_AWAKE)||read_power_status_bit(MSP_PWR_WAKEUP))
        {
            VOS_Splx(power_lock);
            TLSLEEP_RttForceAwake_Inner();
        }
        else
        {
            VOS_Splx(power_lock);
            mspsleep_print_debug("the status is 0x%x", g_msp_pwrctrl.pwr_status);
        }
    }
}
/*****************************************************************************
 �� �� ��  : TLSLEEP_DspHaltIsr
 ��������  : ��ӦDSP halt�жϣ��ͷ��ź�����ʼsleep����ʹ����˯��
 �������  : void
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_VOID TLSLEEP_DspHaltIsr(VOS_UINT param)
{
    char *pcRecordType = "Halt Isr";
    VOS_INT flag = 0;
    /*�����Ǹ���ά�ɲ�,DSP�ڿ�ʼ���еĵ�һʱ����Ϊ1,*/
    GetRttSleepInfo(dspState) = ENUM_DSP_IS_SLEEP;
    VOS_FlushCpuWriteBuf();
    flag = VOS_SplIMP();
    if(FIRST_HALT_INT == g_msp_pwrctrl.DspHaltIntMark )
    {
        DRV_BBPWAKE_INT_CLEAR(PWC_COMM_MODE_LTE);
        DRV_BBPWAKE_INT_CLEAR(PWC_COMM_MODE_TDS);
        DRV_BBPWAKE_INT_ENABLE(PWC_COMM_MODE_LTE);
        DRV_BBPWAKE_INT_ENABLE(PWC_COMM_MODE_TDS);
        g_msp_pwrctrl.DspHaltIntMark = HAD_HALT_INT;
    }
    if(read_power_status_bit(MSP_PWR_HALT_INT))
    {
        set_power_status_bit(MSP_REPEAT_HALT_ERROR);
        VOS_Splx(flag);
        return;
    }
    TLSLEEP_DbgTimeRecord(TLSLEEP_DSPHALTISR_0);
    clear_power_status_bit(MSP_PWR_SYSTEM_RUN);
    set_power_status_bit(MSP_PWR_HALT_INT);
    g_msp_pwrctrl.dsp_sleep_flag = (LPHY_RTT_LPC_MODE_ENUM)GetRttSleepInfo(ulLightSleepFlag);
    TLSLEEP_StateRecord(DRX_HALT_INT);
    TLSLEEP_TimeRecord(pcRecordType);
    if(ENUM_NO_UP_DOWN_DSP == g_msp_pwrctrl.SleepDrxResumeTime.ulPowerSwich)
    {
        LSLEEP_RttForceAwake();
    }
    else
    {
        VOS_SmV(g_msp_pwrctrl.task_sem);
    }

    VOS_Splx(flag);
}
/*****************************************************************************
 �� �� ��  : TLSLEEP_DspResumeIsr
 ��������  : ��ӦDSP RESUMER�жϣ�֪ͨ����DSP������
 �������  : void
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_VOID TLSLEEP_DspResumeIsr(VOS_UINT param)
{
    VOS_INT status_lock = 0;
    char *pcRecordType = "Resume Isr";

    status_lock = VOS_SplIMP();
    TLSLEEP_StateRecord(DRX_RESUME_INT);
    TLSLEEP_DbgTimeRecord(TLSLEEP_DSPRESUMERISR_0);
    /*�жϵ�ǰ���յ�DSP Resume�ж��ڻ����������Ƿ�����*/
    if (read_power_status_bit(MSP_PWR_WAIT_RESUMER))
    {
        clear_power_status_bit(MSP_PWR_WAIT_RESUMER);
        if(read_power_status_bit(MSP_PWR_HALT_INT))
        {
            LSLEEP_RttForceAwake();
            TLSLEEP_StateRecord(DRX_HALT_RESUME_CONFLICT);
        }
        else
        {
            set_power_status_bit(MSP_PWR_SYSTEM_RUN);

            /*�������������ӿ�֪ͨ���份���Ѿ����*/
            BSP_MailBox_DspAwakeProc();
            TLSLEEP_StateRecord(RESUME_NOTIFY_MAILBOX);
        }
		VOS_Splx(status_lock);
        TLSLEEP_TimeRecord(pcRecordType);
        return;
    }
    else
    {
        mspsleep_print_error("dsp resumer int is error");
        mspsleep_print_error("pwr_status = %x", g_msp_pwrctrl.pwr_status);
    }
    VOS_Splx(status_lock);
    return;
}
/*****************************************************************************
 �� �� ��  : LSLEEP_RttIsSleep
 ��������  : ��ѯ��ǰTL�ǻ��ѻ���˯��״̬
 �������  : ��
 �������  : ��
 �� �� ֵ  : VOS_TRUE:��ǰΪ˯��״̬
             VOS_FALSE:��ǰ����˯��״̬
*****************************************************************************/
VOS_BOOL LSLEEP_RttIsSleep(VOS_VOID)
{
    VOS_INT power_lock = 0;
    power_lock = VOS_SplIMP();
    /*lint -e514*/
    g_msp_pwrctrl.RttIsSleepValue = (read_power_status_bit(MSP_PWR_HALT_INT))\
                    |(read_power_status_bit(MSP_PWR_WAIT_RESUMER))\
                    |(!read_power_status_bit(MSP_PWR_SYSTEM_RUN));
    /*lint +e514*/
    /*TLSLEEP_StateRecord(DRX_CHECK_DSP_STATUS);*/
    if(g_msp_pwrctrl.RttIsSleepValue)
    {
        VOS_Splx(power_lock);
        return VOS_TRUE;
    }
    VOS_Splx(power_lock);
    return VOS_FALSE;
}

/*****************************************************************************
 �� �� ��  : LSLEEP_AwakeIsr
 ��������  : ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : ERR_MSP_INIT_FAILURE:��ʼ��ʧ��
             ERR_MSP_SUCCESS: ��ʼ���ɹ�
*****************************************************************************/
VOS_INT TLSLEEP_AwakeIsr(VOS_INT enMode)
{
    char *pcRecordType = "Awake Isr";
    VOS_INT power_lock = 0;

    /*��BBP�����ж�*/
    DRV_BBPWAKE_INT_CLEAR((PWC_COMM_MODE_E)enMode);

    power_lock = VOS_SplIMP();

    if(read_power_status_bit(MSP_PWR_WAKEUP)||read_power_status_bit(MSP_PWR_FORCE_AWAKE)
      ||read_power_status_bit(MSP_PWR_SYSTEM_RUN)|| read_power_status_bit(MSP_PWR_WAIT_RESUMER))
    {
        VOS_Splx(power_lock);
        TLSLEEP_StateRecord(DRX_INVALID_WAKEUP_INT);
        return ERR_MSP_SUCCESS;
    }
    TLSLEEP_DbgTimeRecord(TLSLEEP_AWAKEISR_0);
    TLSLEEP_StateRecord(DRX_WAKEUP_INT);
    TLSLEEP_TimeRecord(pcRecordType);

    set_power_status_bit(MSP_PWR_WAKEUP);
    VOS_SmV(g_msp_pwrctrl.task_sem);

    TLSLEEP_DbgTimeRecord(TLSLEEP_AWAKEISR_1);
    VOS_Splx(power_lock);
    return ERR_MSP_SUCCESS;
}

VOS_VOID TLSLEEP_CheckValidChanel(VOS_VOID)
{
    /*���ABBʹ�õ�ͨ��*/
    if((g_msp_pwrctrl.nv_tlmode_chan_para[0].ucABBSwitch== g_msp_pwrctrl.nv_tlmode_chan_para[1].ucABBSwitch)
        &&(g_msp_pwrctrl.nv_tlmode_chan_para[0].ucABBSwitch != 2))
    {
        g_msp_pwrctrl.tlmode_channel.abb_channel_id= g_msp_pwrctrl.nv_tlmode_chan_para[0].ucABBSwitch;
    }
    else
    {
        g_msp_pwrctrl.tlmode_channel.abb_channel_id = 2;
    }
    /*���RFʹ�õ�ͨ��*/
    if((g_msp_pwrctrl.nv_tlmode_chan_para[0].ucRFSwitch== g_msp_pwrctrl.nv_tlmode_chan_para[1].ucRFSwitch)
        &&(g_msp_pwrctrl.nv_tlmode_chan_para[0].ucRFSwitch!= 2))
    {
        g_msp_pwrctrl.tlmode_channel.rf_channel_id= g_msp_pwrctrl.nv_tlmode_chan_para[0].ucRFSwitch;
    }
    else
    {
        g_msp_pwrctrl.tlmode_channel.rf_channel_id= 2;
    }
    /*���TCXOʹ�õ�ID*/
    if((g_msp_pwrctrl.nv_tlmode_chan_para[0].ucTCXOSwitch== g_msp_pwrctrl.nv_tlmode_chan_para[1].ucTCXOSwitch)
        &&(g_msp_pwrctrl.nv_tlmode_chan_para[0].ucTCXOSwitch!= 2))
    {
        g_msp_pwrctrl.tlmode_channel.tcxo_id= g_msp_pwrctrl.nv_tlmode_chan_para[0].ucTCXOSwitch;
    }
    else
    {
        g_msp_pwrctrl.tlmode_channel.tcxo_id= 2;
    }
}
VOS_VOID TLSLEEP_ExcTimeTask(VOS_UINT32 Para0, VOS_UINT32 Para1,
                                VOS_UINT32 Para2, VOS_UINT32 Para3 )
{
    VOS_CHAR                  acLogPath[] = "/modem_log/msp_sleep.txt";
    FILE                      *logfp = NULL;
    VOS_UINT                  count = 0;
    VOS_CHAR                  auLogBuf[40] = {0};

    logfp = DRV_FILE_OPEN(acLogPath, "w+");
    if (VOS_NULL_PTR == logfp)
    {
        return;
    }
    DRV_FILE_CLOSE(logfp);
    /*lint -save -e716*/
    while(1)
    {
    /*lint -restore*/
        if(VOS_OK != VOS_SmP(g_msp_pwrctrl.exc_overtime.time_sem, WAIT_FOR_EVER))
        {
            mspsleep_print_error("take task sem error!");
        }

        /*���ļ�ϵͳ��д������*/
        if((1 == g_msp_pwrctrl.exc_overtime.ulReadMark)&&(g_msp_pwrctrl.exc_overtime.current_count < 8))
        {
            logfp = DRV_FILE_OPEN(acLogPath, "a+");
            VOS_sprintf(auLogBuf, "< %d >\r\n", g_msp_pwrctrl.exc_overtime.current_count);
            DRV_FILE_WRITE(auLogBuf, sizeof(VOS_CHAR), sizeof(auLogBuf), logfp);
            (g_msp_pwrctrl.exc_overtime.current_count)++;
            for(count = TLSLEEP_DSPHALTISR_0; count <= TLSEEP_HWPOWERUP_MAX; count++)
            {
                VOS_MemSet((VOS_VOID*)auLogBuf, 0, 40);
                VOS_sprintf(auLogBuf, "0x%x  ", g_msp_pwrctrl.exc_overtime.ulTime[count]);
                DRV_FILE_WRITE(auLogBuf, sizeof(VOS_CHAR), 12, logfp);
                if(0 == (count + 1)%8)
                {
                    DRV_FILE_WRITE("\r\n", sizeof(VOS_CHAR), 2,logfp);
                }
            }
            DRV_FILE_WRITE("\r\n", sizeof(VOS_CHAR), 2 ,logfp);
            DRV_FILE_CLOSE(logfp);
            g_msp_pwrctrl.exc_overtime.ulReadMark= 0;
        }
        else
        {
            return;
        }
    }

}
/*****************************************************************************
 �� �� ��  : LSLEEP_Init
 ��������  : ��ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : ERR_MSP_INIT_FAILURE:��ʼ��ʧ��
             ERR_MSP_SUCCESS: ��ʼ���ɹ�
*****************************************************************************/
VOS_UINT32 TLSLEEP_Init(VOS_VOID)
{
    VOS_UINT32 Argument[VOS_TARG_NUM];
    VOS_UINT32 ExcArgument[VOS_TARG_NUM];
    int status_flag = 0;
	int lte_int_num = 0, tds_int_num = 0, ret = 0;
    unsigned int debug_len = 0;
    VOS_UINT_PTR realAddr = 0;
#ifndef MSP_IN_V9R1
    if(DRV_DUMP_GET_BUFFER(DUMP_SAVE_MOD_MSP_SLEEP, &(g_msp_pwrctrl.dump_base), &(g_msp_pwrctrl.dump_len)))
    {
        mspsleep_print_error("get exc buffer error,module id = %d", DUMP_SAVE_MOD_MSP_SLEEP);
        g_msp_pwrctrl.dump_len = 0x1000;
        g_msp_pwrctrl.dump_base = (VOS_VOID * )VOS_UnCacheMemAlloc(g_msp_pwrctrl.dump_len, &realAddr);
    }
#else
	if(1)
    {
        g_msp_pwrctrl.dump_len = 0x1000;
        g_msp_pwrctrl.dump_base = (VOS_VOID * )VOS_UnCacheMemAlloc(g_msp_pwrctrl.dump_len, &realAddr);
    }
#endif
    if(VOS_NULL != g_msp_pwrctrl.dump_base)
    {
        VOS_MemSet((void *)g_msp_pwrctrl.dump_base, 0, g_msp_pwrctrl.dump_len);
        /*��ʼ����ά�ɲ�*/
        debug_len = (VOS_UINT)MSP_EXC_RECORED_TIME_SIZE;
        g_msp_pwrctrl.time_queue_record = (drx_queue_t*)(MSP_EXC_RECORED_TIME_OFFSET);
        QueueInit(g_msp_pwrctrl.time_queue_record, (debug_len - 0x10)/sizeof(DRX_RECORD_STRU));
        g_msp_pwrctrl.DrxStateSlice = (SLEEP_RECORD_STRU*)(MSP_EXC_STATE_SLICE_OFFSET);
        g_msp_pwrctrl.DrxStateEnd = MSP_EXC_STATE_SLICE_OFFSET + MSP_EXC_STATE_SLICE_SIZE;
        g_msp_pwrctrl.dsp_state = (DspState *)MSP_EXC_DSP_STATE_OFFSET;
    }
    g_msp_pwrctrl.exc_overtime.timeOut = 45;

    g_msp_pwrctrl.pwr_status = 0;

    if(VOS_OK != VOS_SmBCreate("EXC", 0, VOS_SEMA4_FIFO, &g_msp_pwrctrl.exc_overtime.time_sem))
    {
        mspsleep_print_error("SLEEP_Init: Create EXC SEM Fail.");
        return ERR_MSP_INIT_FAILURE;
    }

    if(VOS_OK != VOS_SmBCreate("TC_0", 0, VOS_SEMA4_FIFO, &g_msp_pwrctrl.SleepTcxoSem[0]))
    {
        msp_set_error_bit(status_flag, MSP_CREATE_TCXO0_SEM_FAIL);
        mspsleep_print_error("SLEEP_Init: Create TCXO 0 Fail.");
        return ERR_MSP_INIT_FAILURE;
    }
    if(VOS_OK != VOS_SmBCreate("TC_1", 0, VOS_SEMA4_FIFO, &g_msp_pwrctrl.SleepTcxoSem[1]))
    {
        msp_set_error_bit(status_flag, MSP_CREATE_TCXO1_SEM_FAIL);
        mspsleep_print_error("SLEEP_Init: Create TCXO 1 Fail.");
        return ERR_MSP_INIT_FAILURE;
    }
#ifndef MSP_IN_V9R1
    /* ��ȡdelay nv */
    if(NVM_Read(NV_ID_DRV_DRX_DELAY, &g_msp_drx_delay, sizeof(DRV_DRX_DELAY_STRU)))
    {
        memset(&g_msp_drx_delay, 0x0, sizeof(DRV_DRX_DELAY_STRU));
    }
#endif
    /* ��ȡNV�����ȶ�ʱ�� */
    if(NVM_Read(NV_ID_MSP_TL_DRX_RESUME_TIME, &g_msp_pwrctrl.SleepDrxResumeTime, sizeof(NV_TL_DRX_RESUME_TIME_STRU)))
    {
        g_msp_pwrctrl.SleepDrxResumeTime.ulTcxoRsmTime = 3000;
        /* coverity[lock_acqire] */
        msp_set_error_bit(status_flag, MSP_READ_NV_FAIL);
        mspsleep_print_error("Read NV: 0x%x Data Fail.", NV_ID_MSP_TL_DRX_RESUME_TIME);
    }
    if(NVM_Read(NV_ID_MSP_TLMODE_CHAN_PARA_STRU, g_msp_pwrctrl.nv_tlmode_chan_para, 2*sizeof(NV_TLMODE_BASIC_PARA_STRU)))
    {
        /* coverity[lock_acqire] */
        msp_set_error_bit(status_flag, MSP_READ_NV_FAIL);
        mspsleep_print_error("Read NV: 0x%x Data Fail.", NV_ID_MSP_TLMODE_CHAN_PARA_STRU);
    }
    TLSLEEP_CheckValidChanel();
    /*�����ź�����ʼ��*/
    if(VOS_SmBCreate( "SLEEP", 0, VOS_SEMA4_FIFO , &g_msp_pwrctrl.task_sem))
    {
        msp_set_error_bit(status_flag, MSP_CREATE_TASK_SEM_FAIL);
        mspsleep_print_error("create task sem failed");
        return ERR_MSP_INIT_FAILURE;
    }
    /* ��ʼ��exc���������ڻ��ѳ�ʱ��ʱ�����ļ�ϵͳ��д�뻽�ѵ�ʱ���¼*/
    /* coverity[sleep] */
	if(VOS_CreateTask("mspExcTask", &g_msp_pwrctrl.exc_overtime.timeTaskId, TLSLEEP_ExcTimeTask, MSP_EXC_TASK_PRI, 2048, ExcArgument))
	{
		mspsleep_print_error("create Exc task failed");
	}
    /* ��ʼ��˯������*/
    /* coverity[sleep] */
	if(VOS_CreateTask("mspSleepTask", &g_msp_pwrctrl.task_id, TLSLEEP_SleepTask, MSP_SLEEP_TASK_PRI, 32768, Argument))
	{
        msp_set_error_bit(status_flag, MSP_CREATE_TASK_FAIL);
		mspsleep_print_error("create msp task failed");
	}

    /*��������ע��ӿ�ע���鵱ǰ������˯�ߺ�ע��ǿ�ƻ��ѽӿں���*/
    BSP_MailBox_GetDspStatusReg((BSP_MBX_GET_STATUS_FUNC)LSLEEP_RttIsSleep);
    BSP_MailBox_DspForceAwakeReg(LSLEEP_RttForceAwake);
    ret += TLSLEEP_DfsQosRequest((VOS_INT32)DFS_QOS_ID_BUS_MINFREQ_E, TLSLEEP_DFS_BUS_75M, &dfs_bus_req_id);
    ret += TLSLEEP_DfsQosRequest((VOS_INT32)DFS_QOS_ID_DDR_MINFREQ_E, TLSLEEP_DFS_DDR_75M, &dfs_ddr_req_id);
    ret += TLSLEEP_DfsQosRequest((VOS_INT32)DFS_QOS_ID_CCPU_MINFREQ_E, TLSLEEP_DFS_CCPU_150M, &dfs_ccpu_req_id);/* [false alarm]:����Fortify */
    if(ret)
    {
        set_power_status_bit(MSP_DFS_REQUEST_FAIL);/* [false alarm]:����Fortify */
        mspsleep_print_error("request dfs fai, ret = %d\n", ret);
    }

    DRV_BBPWAKE_INT_CLEAR(PWC_COMM_MODE_LTE);
    DRV_BBPWAKE_INT_CLEAR(PWC_COMM_MODE_TDS);
    DRV_BBPWAKE_INT_DISABLE(PWC_COMM_MODE_LTE);
    DRV_BBPWAKE_INT_DISABLE(PWC_COMM_MODE_TDS);
	lte_int_num = DRV_GET_INT_NO(BSP_INT_TYPE_LBBP_AWAKE);
	tds_int_num = DRV_GET_INT_NO(BSP_INT_TYPE_TBBP_AWAKE);
#ifdef MSP_IN_V9R1
    ret =  DRV_VICINT_CONNECT((VOIDFUNCPTR*)lte_int_num, (VOIDFUNCPTR)TLSLEEP_AwakeIsr, (VOS_INT)PWC_COMM_MODE_LTE);
    ret += DRV_VICINT_CONNECT((VOIDFUNCPTR*)tds_int_num, (VOIDFUNCPTR)TLSLEEP_AwakeIsr, (VOS_INT)PWC_COMM_MODE_TDS);
#else
    ret =  DRV_VICINT_CONNECT(lte_int_num, TLSLEEP_AwakeIsr, (VOS_INT)PWC_COMM_MODE_LTE);
    ret += DRV_VICINT_CONNECT(tds_int_num, TLSLEEP_AwakeIsr, (VOS_INT)PWC_COMM_MODE_TDS);
#endif
    ret += DRV_IPC_INTCONNECT((IPC_INT_LEV_E)IPC_INT_DSP_HALT, TLSLEEP_DspHaltIsr, 0);
    ret += DRV_IPC_INTCONNECT((IPC_INT_LEV_E)IPC_INT_DSP_RESUME, TLSLEEP_DspResumeIsr, 0);
    ret += DRV_IPC_INTCONNECT((IPC_INT_LEV_E)IPC_INT_WAKE_GSM, (VOIDFUNCPTR)SLEEP_MasterWakeSlave, (VOS_UINT)PWC_COMM_MODE_GSM);
    ret += DRV_IPC_INTCONNECT((IPC_INT_LEV_E)IPC_INT_WAKE_WCDMA, (VOIDFUNCPTR)SLEEP_MasterWakeSlave, (VOS_UINT)PWC_COMM_MODE_WCDMA);
    if(ret)
    {
        msp_set_error_bit(status_flag, MSP_INT_CONNECT_FAIL);
		mspsleep_print_error("int connect fail!");
		return ERR_MSP_INIT_FAILURE;
    }
    ret = DRV_VICINT_ENABLE(lte_int_num);
    ret += DRV_VICINT_ENABLE(tds_int_num);
    ret += DRV_IPC_INTENABLE(IPC_INT_DSP_HALT);
    ret += DRV_IPC_INTENABLE(IPC_INT_DSP_RESUME);
    ret += DRV_IPC_INTENABLE(IPC_INT_WAKE_GSM);
    ret += DRV_IPC_INTENABLE(IPC_INT_WAKE_WCDMA);
    if(ret)
    {
        msp_set_error_bit(status_flag, MSP_INT_ENABLE_FAIL);
		mspsleep_print_error("int connect fail!");
		return ERR_MSP_INIT_FAILURE;
    }

    VOS_MemSet((void *)(SRAM_RTT_SLEEP_FLAG_ADDR), 0, SRAM_RTT_SLEEP_FLAG_SIZE);
    /*��ʼ����DSP֮��ı�־��CCPU���µ�Ҫ�ж������־�����Ƿ��TCXO���µ�*/
    GetRttSleepInfo(ulLightSleepFlag) = POWER_SAVING_NULL;
    VOS_FlushCpuWriteBuf();
    mspsleep_print_error("msp sleep init OK!");

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : bsp_mailbox_init
 ��������  : ��ģ���Ѵ�ģ�Ľӿ�,LΪ��ģ����GU����,msp�ṩ,oam����
 �������  : None
 �������  : None
 �� �� ֵ  : None
*****************************************************************************/
VOS_VOID TLSLEEP_MasterAwakeSlave(VOS_VOID)
{
    TLSLEEP_StateRecord(DRX_SLAVE_WAKE_INT);
    LSLEEP_RttForceAwake();
}
/*****************************************************************************
 �� �� ��  : TLSLEEP_InitPowerUp
 ��������  : �ṩ��Э��ջ���ã�������DSP����֮ǰ���ã�������Щ������Ĭ�����ϵ��
             ��Ҫ�������Ϊ�ϵ�״̬
 �������  : None
 �������  : None
 �� �� ֵ  : None
*****************************************************************************/
VOS_VOID TLSLEEP_ActivateHw(PWC_COMM_MODE_E modeId)
{
    PWC_COMM_MODEM_E                    enModem = PWC_COMM_MODEM_0;
#if (FEATURE_MULTI_CHANNEL == FEATURE_ON)
    PWC_COMM_CHANNEL_E                  enChannel = PWC_COMM_CHANNEL_0;
#endif
    VOS_INT                          flag = 0;
    VOS_UINT32                          ret = 0;
	enModem = PWC_COMM_MODEM_0;

    TLSLEEP_OpenTCXO();
    /*DDR������Ƶ����СֵΪ150M,V9R1ʹ��*/
    ret += TLSLEEP_DfsQosUpdate((VOS_INT32)DFS_QOS_ID_BUS_MINFREQ_E, dfs_bus_req_id, TLSLEEP_DFS_BUS_150M);/* [false alarm]:����Fortify */
    ret += TLSLEEP_DfsQosUpdate((VOS_INT32)DFS_QOS_ID_DDR_MINFREQ_E, dfs_ddr_req_id, TLSLEEP_DFS_DDR_150M);/* [false alarm]:����Fortify */
    ret += TLSLEEP_DfsQosUpdate((VOS_INT32)DFS_QOS_ID_CCPU_MINFREQ_E, dfs_ccpu_req_id,TLSLEEP_DFS_CCPU_600M);/* [false alarm]:����Fortify */
    if(ret)
    {
        set_power_status_bit(MSP_DFS_HW_UPDATE_FAIL);/* [false alarm]:����Fortify */
    }
#if defined(FEATURE_MULTI_CHANNEL)&&defined(MSP_IN_V9R1)
    TLSLEEP_PwrctrlUp(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB);
    /* Э��ջ����ʱ����ʹ��PLL���ٸ�BBP�ϵ硢���� */
    DRV_PWRCTRL_PLLENABLE(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem, enChannel);
    TLSLEEP_PllEnable(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB);

    /* TDS��BBP�ϵ�ӿ���ͬʱ��CS/PS���������ϵ磬LTE�����ĵ���DSP���� */
    /* LTEģʽҲ����CS/PS����������CS/PS����LTE��ģҲ�����ϵ� */
    DRV_PWRCTRL_PWRUP(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBP_DRX, enModem, enChannel);

    /*��TDS ʹ�õ�BBP 1A����ʱ��*/
    DRV_PWRCTRL_BBPCLKENABLE(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBP_DRX, enModem, enChannel);

    /* LTE�漰���ʱ��Դ��DSPֻ����һ���֣���ҪMSP���õ���ӿڴ�LTE�������ʱ�� */
    DRV_PWRCTRL_BBPCLKENABLE(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem, enChannel);

    TLSLEEP_PwrctrlUp(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_RF);
#elif defined(MSP_IN_V9R1)
    TLSLEEP_PwrctrlUp(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB);
    DRV_PWRCTRL_PWRUP(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBP_DRX, enModem);
    DRV_PWRCTRL_PWRUP(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem);
    /*��TDS ʹ�õ�BBP 1A����ʱ��*/
    DRV_PWRCTRL_BBPCLKENABLE(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBP_DRX, enModem);
    DRV_PWRCTRL_PLLENABLE(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem);
    TLSLEEP_PllEnable(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB);
    DRV_PWRCTRL_PWRUP(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_PA, enModem);
    TLSLEEP_PwrctrlUp(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_RF);
#else
    DRV_PWRCTRL_PLLENABLE(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBE16, enModem);
    DRV_PWRCTRL_PLLENABLE(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBE16, enModem);
    TLSLEEP_PwrctrlUp(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB);
    DRV_PWRCTRL_PWRUP(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBP_DRX, enModem);
    DRV_PWRCTRL_PWRUP(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem);
    /*��TDS ʹ�õ�BBP 1A����ʱ��*/
    DRV_PWRCTRL_BBPCLKENABLE(PWC_COMM_MODE_TDS, PWC_COMM_MODULE_BBP_DRX, enModem);
    DRV_PWRCTRL_PLLENABLE(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem);
    TLSLEEP_PllEnable(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB);
    TLSLEEP_PwrctrlUp(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_RF);
#endif
    flag = VOS_SplIMP();
    set_power_status_bit(MSP_PWR_SYSTEM_RUN);
    VOS_Splx(flag);

    return;
}

VOS_VOID TLSLEEP_ResumeDsp(VOS_VOID)
{
    DRV_PWRCTRL_DSP_HOLD();
    DRV_PWRCTRL_DSP_RESET();
    DRV_PWRCTRL_DSP_UNRESET();
    TLSLEEP_RunDsp();
}

/*****************************************************************************
 �� �� ��  : TLSLEEP_GetTdsSubFrame
 ��������  : ��Ϣд��ռ�����
 �������  : V9R1�ṩ��DSP ������DSP��BBP 1A�µ������DSP���������ŷ��ظ�PS
 �������  : ��,
 �� �� ֵ  : PSӦ��ʹ�õ������
*****************************************************************************/
VOS_UINT16 TLSLEEP_GetTdsSubFrame(VOS_VOID)
{
    /*lint -save -e958*/
    VOS_UINT16 usSubFrame;
    VOS_UINT32 ulSleepTime;
    VOS_INT32 lIntLock;
    VOS_UINT32 ulStartSlice = 0;
    RTT_SLEEP_INFO_STRU *pstRttSleepInfo = (RTT_SLEEP_INFO_STRU *)(SRAM_RTT_SLEEP_FLAG_ADDR);
    /*lint -restore*/
    if(DRV_GET_TDS_CLK_STATUS())
    {
        ulStartSlice = DRV_GET_SLICE();
        while((!LSLEEP_RttIsSleep())&&(DRV_GET_SLICE() - ulStartSlice < TLSLEEP_TIMEOUT_CNT)){}
    }
    lIntLock = VOS_SplIMP();

    usSubFrame = BSP_GetTdsSubFrame();
    mspsleep_print_debug("usSubFrame = 0x%08x\n", usSubFrame);
    if ((VOS_UINT16)BSP_ERROR == usSubFrame)
    {
        ulSleepTime = BSP_GetTdsSleepTime();
        mspsleep_print_debug("ulSleepTime = 0x%08x\n", ulSleepTime);
        usSubFrame = (VOS_UINT16)(pstRttSleepInfo->ulTdsPreSfn + ((ulSleepTime * 25) >> 12) + 1);
        mspsleep_print_debug("usSubFrame = 0x%08x\n", usSubFrame);
    }
    VOS_Splx(lIntLock);

    return usSubFrame;
}
/*****************************************************************************
 �� �� ��  : LSLEEP_ShowStat
 ��������  :
 �������  : void
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :
*****************************************************************************/
VOS_VOID TLSLEEP_ShowStat(VOS_UINT32 ulIndex)
{
    VOS_UINT i = 0, bbp_pll = 0, bbp_ldo = 0;
    PWC_COMM_MODEM_E                    enModem = PWC_COMM_MODEM_0;
#if (FEATURE_MULTI_CHANNEL == FEATURE_ON)
    PWC_COMM_CHANNEL_E                  enChannel = PWC_COMM_CHANNEL_0;
#endif
    VOS_INT                             ulTcxoStatus[2] = {0};
    VOS_UINT32                          abb_pll[2] = {0};
    VOS_UINT32                          rf_ldo[2] = {0};
	enModem = PWC_COMM_MODEM_0;
#if (FEATURE_MULTI_CHANNEL == FEATURE_ON)
    bbp_pll = DRV_PWRCTRL_PLLSTATUSGET(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem, enChannel);
    abb_pll[0] = DRV_PWRCTRL_PLLSTATUSGET(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB, PWC_COMM_MODEM_0, enChannel);
    abb_pll[1] = DRV_PWRCTRL_PLLSTATUSGET(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB, PWC_COMM_MODEM_1, enChannel);

    rf_ldo[0] = DRV_PWRCTRL_PWRSTATUSGET(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_RF, PWC_COMM_MODEM_0, enChannel);
    rf_ldo[1] = DRV_PWRCTRL_PWRSTATUSGET(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_RF, PWC_COMM_MODEM_1, enChannel);
    bbp_ldo = DRV_PWRCTRL_PWRSTATUSGET(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem, enChannel);
#else
    bbp_pll = DRV_PWRCTRL_PLLSTATUSGET(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem);
    abb_pll[0] = DRV_PWRCTRL_PLLSTATUSGET(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB, PWC_COMM_MODEM_0);
    abb_pll[1] = DRV_PWRCTRL_PLLSTATUSGET(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_ABB, PWC_COMM_MODEM_1);

    rf_ldo[0] = DRV_PWRCTRL_PWRSTATUSGET(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_RF, PWC_COMM_MODEM_0);
    rf_ldo[1] = DRV_PWRCTRL_PWRSTATUSGET(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_RF, PWC_COMM_MODEM_1);
    bbp_ldo = DRV_PWRCTRL_PWRSTATUSGET(PWC_COMM_MODE_LTE, PWC_COMM_MODULE_BBP_DRX, enModem);
#endif
    ulTcxoStatus[0] = DRV_TCXO_GETSTATUS(PWC_COMM_MODEM_0);
    ulTcxoStatus[1] = DRV_TCXO_GETSTATUS(PWC_COMM_MODEM_1);
    /*lint -save -e713 -e574*/
    vos_printf("RTT SLEEP Show Statistics Info:\n");

    vos_printf("Sleep system state: 0x%x\n", read_power_status_bit(MSP_PWR_SYSTEM_RUN));
    vos_printf("BBP Pll state:      0x%x\n", bbp_pll);
    vos_printf("ABB CHN0 Pll state: 0x%x\n", abb_pll[0]);
    vos_printf("ABB CHN1 Pll state: 0x%x\n", abb_pll[1]);

    vos_printf("RF LDO0 state:       0x%x\n",rf_ldo[0]);
    vos_printf("RF LDO1 state:       0x%x\n",rf_ldo[1]);

    vos_printf("BBP LDO state:      0x%x\n",bbp_ldo);
    vos_printf("TCXO 0 state :      0x%x\n",ulTcxoStatus[0]);
    vos_printf("TCXO 1 state:      0x%x\n",ulTcxoStatus[1]);
    vos_printf("\n");

    vos_printf("Sleep Cmd mask:     0x%x\n",g_msp_pwrctrl.pwr_status);
    vos_printf("Deep Sleep Flag:     0x%x\n",g_msp_pwrctrl.dsp_sleep_flag);
    vos_printf("Last is sleep value: 0x%x\n", g_msp_pwrctrl.RttIsSleepValue);
    vos_printf("Forbid Dsp value: 0x%x\n", g_msp_pwrctrl.ForbidDspSleepValue);

    vos_printf("\n");
    if(VOS_NULL != g_msp_pwrctrl.DrxStateSlice)
    {
    vos_printf("DSP Halt Int:       Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[DRX_HALT_INT].ulCount,g_msp_pwrctrl.DrxStateSlice[DRX_HALT_INT].ulSlice);
    vos_printf("SLEEP task:         Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[DRX_SLEEP_TASK].ulCount,g_msp_pwrctrl.DrxStateSlice[DRX_SLEEP_TASK].ulSlice);
    vos_printf("Sleep NeedNot Deal: Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[SLEEP_TASK_NOT_NEED_DEAL].ulCount,g_msp_pwrctrl.DrxStateSlice[SLEEP_TASK_NOT_NEED_DEAL].ulSlice);
    vos_printf("DSP deep sleep:     Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[DSP_DEEP_SLEEP_COUNT].ulCount,g_msp_pwrctrl.DrxStateSlice[DSP_DEEP_SLEEP_COUNT].ulSlice);
    vos_printf("DSP liaght sleep:   Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[DSP_LIGHT_SLEEP_COUNT].ulCount,g_msp_pwrctrl.DrxStateSlice[DSP_LIGHT_SLEEP_COUNT].ulSlice);
	vos_printf("Store DspMem Fail:  Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[STORE_DSP_TCM_FAIL].ulCount,g_msp_pwrctrl.DrxStateSlice[STORE_DSP_TCM_FAIL].ulSlice);
    vos_printf("Vote sleep:         Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[SLEEP_VOTE_UNLOCK].ulCount,g_msp_pwrctrl.DrxStateSlice[SLEEP_VOTE_UNLOCK].ulSlice);
    vos_printf("Invalid Wake Int:   Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[DRX_INVALID_WAKEUP_INT].ulCount,g_msp_pwrctrl.DrxStateSlice[DRX_INVALID_WAKEUP_INT].ulSlice);
    vos_printf("Wake Int:           Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[DRX_WAKEUP_INT].ulCount,g_msp_pwrctrl.DrxStateSlice[DRX_WAKEUP_INT].ulSlice);
	vos_printf("Restore DspMem Fail Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[RESTORE_DSP_TCM_FAIL].ulCount,g_msp_pwrctrl.DrxStateSlice[RESTORE_DSP_TCM_FAIL].ulSlice);
    vos_printf("Force Awake API:    Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[DRX_FORCE_AWAKE_API].ulCount,g_msp_pwrctrl.DrxStateSlice[DRX_FORCE_AWAKE_API].ulSlice);
    vos_printf("Force Not Nedd Deal   Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[DRX_FORCE_AWAKE_NOT_DEAL].ulCount,g_msp_pwrctrl.DrxStateSlice[DRX_FORCE_AWAKE_NOT_DEAL].ulSlice);
    vos_printf("Force Awake Dsp Run:Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[DRX_FORCE_AWAKE_RUN_RETURN].ulCount,g_msp_pwrctrl.DrxStateSlice[DRX_FORCE_AWAKE_RUN_RETURN].ulSlice);
    vos_printf("Wait Tcxo Sem Fail: Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[DRX_FAIL_WAIT_TCXO_SEM].ulCount,g_msp_pwrctrl.DrxStateSlice[DRX_FAIL_WAIT_TCXO_SEM].ulSlice);
    vos_printf("Resume Notify Mail: Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[RESUME_NOTIFY_MAILBOX].ulCount,g_msp_pwrctrl.DrxStateSlice[RESUME_NOTIFY_MAILBOX].ulSlice);
    vos_printf("DSP Resume Int:     Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[DRX_RESUME_INT].ulCount,g_msp_pwrctrl.DrxStateSlice[DRX_RESUME_INT].ulSlice);
	vos_printf("SLAVE Sleep Int:    Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[DRX_SLAVE_DSP_INT].ulCount,g_msp_pwrctrl.DrxStateSlice[DRX_SLAVE_DSP_INT].ulSlice);
	vos_printf("Master Wake Int:    Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[DRX_MASTER_WAKE_INT].ulCount,g_msp_pwrctrl.DrxStateSlice[DRX_MASTER_WAKE_INT].ulSlice);
	vos_printf("SLAVE Wake Int:     Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[DRX_SLAVE_WAKE_INT].ulCount,g_msp_pwrctrl.DrxStateSlice[DRX_SLAVE_WAKE_INT].ulSlice);
	vos_printf("Check Dsp Status:   Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[DRX_CHECK_DSP_STATUS].ulCount,g_msp_pwrctrl.DrxStateSlice[DRX_CHECK_DSP_STATUS].ulSlice);
	vos_printf("Sleep Flag Error:   Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[DSP_SLEEP_FLAG_ERROR].ulCount,g_msp_pwrctrl.DrxStateSlice[DSP_SLEEP_FLAG_ERROR].ulSlice);
	vos_printf("Halt Resume Conflict:Count 0x%08x, Slice 0x%08x\n",g_msp_pwrctrl.DrxStateSlice[DRX_HALT_RESUME_CONFLICT].ulCount,g_msp_pwrctrl.DrxStateSlice[DRX_HALT_RESUME_CONFLICT].ulSlice);
    }
    if (0 != ulIndex)
    {
        vos_printf("\n");
        vos_printf("==========================================================\n");
        vos_printf("Time Record: count 0x%x position %d\n",g_msp_pwrctrl.SleepTimeRecord.ulCount,((g_msp_pwrctrl.SleepTimeRecord.ulCount-1)%SLEEP_RECORD_NUM));
        vos_printf("Num   Type               Time\n");
        for (i=0; i<(g_msp_pwrctrl.SleepTimeRecord.ulCount < SLEEP_RECORD_NUM ? g_msp_pwrctrl.SleepTimeRecord.ulCount : SLEEP_RECORD_NUM); i++)
        {
            vos_printf("%03d   %16s   0x%08x\n",i ,(VOS_INT)g_msp_pwrctrl.SleepTimeRecord.astRecord[i].acType, g_msp_pwrctrl.SleepTimeRecord.astRecord[i].ulTime);
        }
    }
    vos_printf("dbg time record addr = 0x%08x\r\n", (VOS_UINT)g_msp_pwrctrl.dbg_time_record);
    vos_printf("queue time record addr = 0x%08x\r\n", (VOS_UINT)g_msp_pwrctrl.time_queue_record->data);
    vos_printf("drx state slice addr = 0x%08x\r\n", (VOS_UINT)g_msp_pwrctrl.DrxStateSlice);
    vos_printf("timestamp shared memory address = 0x%08x\r\n", DEBUG_TIMESTAMP_ADDR);
    /*lint -restore*/

}

/*****************************************************************************
 �� �� ��  : TLSLEEP_ShowDebugTime
 ��������  :
 �������  : void
 �������  : ��
 �� �� ֵ  : ��ά�ɲ⺯�����ڴ�ӡ���µ�����е�ʱ���
 ���ú���  :
 ��������  :
*****************************************************************************/
VOS_VOID TLSLEEP_ShowDebugTime(VOS_INT mode)
{
    EN_SLEEP_TIME_RECORD i = TLSLEEP_RTTSLEEP_INNER_0;
    VOS_UINT32 count = 0;
	if(VOS_NULL == g_msp_pwrctrl.dbg_time_record)
	{
		return;
	}
    vos_printf("%s:\n", "sleep:", 0, 0, 0, 0, 0);
    for(i = TLSLEEP_DSPHALTISR_0; i <= TLSLEEP_RTTSLEEP_INNER_MAX; i++)
    {
        vos_printf("%08x  ", g_msp_pwrctrl.dbg_time_record[i], 0, 0, 0, 0, 0);
    }
    vos_printf("\n%s:\n", "wake", 0, 0, 0, 0, 0);/* [false alarm]:����Fortify */
    for(i = TLSLEEP_AWAKEISR_0, count = 0; i <= TLSLEEP_DSPRESUMERISR_MAX; i++)
    {
        vos_printf("%08x  ", g_msp_pwrctrl.dbg_time_record[i], 0, 0, 0, 0, 0);
        count++;
        if(count%8 == 0)
        {
            vos_printf("\n", 0, 0, 0, 0, 0, 0);
        }
    }
}
#else
VOS_UINT32 TLSLEEP_Init(VOS_VOID)
{
    return VOS_OK;
}
VOS_BOOL LSLEEP_RttIsSleep(VOS_VOID)
{
    return VOS_FALSE;
}
VOS_VOID LSLEEP_RttForceAwake(VOS_VOID)
{
}

VOS_VOID TLSLEEP_MasterAwakeSlave(VOS_VOID)
{
}

VOS_VOID TLSLEEP_ActivateHw(PWC_COMM_MODE_E modeId)
{
}

VOS_UINT16 TLSLEEP_GetTdsSubFrame(VOS_VOID)
{
    VOS_UINT16 usSubFrame = 0;

    usSubFrame = BSP_GetTdsSubFrame();

    return usSubFrame;
}

#endif
/*lint +e409*/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


