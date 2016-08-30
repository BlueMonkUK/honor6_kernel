

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "ucom_low_power.h"
#include "med_drv_ipc.h"
#include "med_drv_timer_hifi.h"
#include "med_drv_watchdog.h"
#include "HifiOmInterface.h"
#include "med_drv_mb_hifi.h"
#include "CodecNvInterface.h"
#include "ucom_nv.h"
#include "om_cpuview.h"
#include "soc_baseaddr_interface.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
UCOM_POWER_CTRL_STRU                    g_uwUcomPowerCtrl;

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
extern VOS_UINT32                       VOICE_ApiIsIdle(VOS_VOID);
//extern VOS_UINT32                       AUDIO_COMM_IsIdle(VOS_VOID);


VOS_VOID UCOM_LOW_PowerTask(VOS_UINT32 ulVosCtxSw)
{
    VOS_UINT32                              uwTimeStamps  = DRV_TIMER_ReadSysTimeStamp();
    VOS_UINT32                              uwIdleTime    = 0;
    VOS_UINT32                              uwVoiceIsIdle = VOS_OK;
    VOS_UINT32                              uwAudioIsIdle = VOS_OK;
    VOS_CPU_SR                              srSave;

    /* ���ж� */
    srSave = VOS_SplIMP();

    /* ����һ���ϵ�����Idle�����򴥷�IPC�ж�֪ͨC-ARM��HIFI������� */
    if (UCOM_IS_FIRST_IDLE())
    {
        #if 0
        /* �����ж�,�����˴��ж�ΪHIFI��������жϣ��˹���MCU��δʵ�֣�����ʵ�ֺ�� */
        DRV_IPC_TrigInt(DRV_IPC_MCU_CORE,IPC_MCU_INT_SRC_HIFI_PU);
        #endif

        /*ʹ��IPC�ж�*/
        VOS_EnableInterrupt(DRV_IPC_INT_NO_HIFI);

        /* �����ѵ�һ�ν���IDLE���� */
        UCOM_SET_FIRST_IDLE();

        /* ���±��ν���IDLE�����ʱ�估�����л�����*/
        UCOM_SET_LAST_IDLE_TIME(uwTimeStamps);

        UCOM_SET_LAST_SW_CNT(ulVosCtxSw);

        /* �ͷ��ж� */
        VOS_Splx(srSave);

        return;
    }

    /* ��������δ�л��������ϵͳһֱ����IDLE���У�����е͹��Ķ��� */
    if (ulVosCtxSw == UCOM_GET_LAST_SW_CNT())
    {
        /* ���㱾�ο���ʱ�� */
        uwIdleTime = UCOM_COMM_CycSub(uwTimeStamps,
                                      UCOM_GET_LAST_IDLE_TIME(),
                                      0xffffffff);

        /*������ʱ�������ֵ������Ϊ�������͹��ĵ�����*/
        if (uwIdleTime > UCOM_GET_WI_TIME_THD())
        {
            uwVoiceIsIdle = VOICE_ApiIsIdle();
            uwAudioIsIdle = VOS_OK;//AUDIO_COMM_IsIdle();

            /*Hifi�ϸ�ҵ��ģ�����ͶƱ��������ҵ����׼���µ磬�������WI�͹���ģʽ*/
            if ((VOS_OK != uwVoiceIsIdle) || (VOS_OK != uwAudioIsIdle))
            {
                /* ���±��ν���IDLE�����ʱ�估�����л����� */
                UCOM_SET_LAST_IDLE_TIME(uwTimeStamps);

                UCOM_SET_LAST_SW_CNT(ulVosCtxSw);

                if(UCOM_GET_WATCHDOG_ENABLE())
                {
                    /*��WFI֮ǰ�رտ��Ź�*/
                    DRV_WATCHDOG_Stop();
                }

                OM_CPUVIEW_EnterArea((VOS_UCHAR)OM_CPUVIEW_AREA_UCOM_WFI);

                /*����ǰHIFI����ҵ�����У������Wait For Interruptģʽ*/
                UCOM_SET_WFI(0);

                OM_CPUVIEW_ExitArea((VOS_UCHAR)OM_CPUVIEW_AREA_UCOM_WFI);

                /*��WFI�������Ź�*/
                if(UCOM_GET_WATCHDOG_ENABLE())
                {
                    DRV_WATCHDOG_Start(UCOM_GET_WD_TIMEOUT_LEN());
                }
            }
            /*������ʱ�������ֵ������Ϊ�����µ���������������ֿ�*/
            else if (uwIdleTime >= UCOM_GET_PD_TIME_THD())
            {
                /* ���±��ν���IDLE�����ʱ�估�����л�����*/
                UCOM_SET_LAST_IDLE_TIME(uwTimeStamps);

                if(UCOM_GET_WATCHDOG_ENABLE())
                {
                    /* ֹͣHifi��WatchDog */
                    DRV_WATCHDOG_Stop();
                }

                /* ֹͣHifi��Socpͨ�� */
                DRV_SOCP_Stop();

                OM_CPUVIEW_EnterArea((VOS_UCHAR)OM_CPUVIEW_AREA_UCOM_PD);

                if(UCOM_GET_POWEROFF_ENABLE())
                {
                    /* �����жϣ�֪ͨMCU���Խ���HIFI�µ� */
                    DRV_IPC_TrigInt(DRV_IPC_MCU_CORE,IPC_MCU_INT_SRC_HIFI_PD);
                }

                /* ����WFI �ȴ��µ� */
                UCOM_SET_WFI(0);

                /* ���³�ʼ��Hifi��socpͨ�� */
                DRV_SOCP_Init();

                OM_CPUVIEW_ExitArea((VOS_UCHAR)OM_CPUVIEW_AREA_UCOM_PD);

            }
            else
            {
                /* for pc lint */
            }
        }
    }
    else
    {
        /* ���±��ν���IDLE�����ʱ�估�����л�����*/
        UCOM_SET_LAST_IDLE_TIME(uwTimeStamps);

        UCOM_SET_LAST_SW_CNT(ulVosCtxSw);
    }

    /* �ͷ��ж� */
    VOS_Splx(srSave);

}
VOS_VOID UCOM_LOW_PowerInit(VOS_VOID)
{
    VOS_UINT32                              uwRet;
    CODEC_NV_PARA_HIFI_POWEROFF_CFG_STRU    stHifiPowerOffCfg;
    CODEC_NV_PARA_HIFI_WATCHDOG_CFG_STRU    stHifiWatchDogCfg;

    UCOM_MemSet(&stHifiPowerOffCfg, 0, sizeof(CODEC_NV_PARA_HIFI_POWEROFF_CFG_STRU));
    UCOM_MemSet(&stHifiWatchDogCfg, 0, sizeof(CODEC_NV_PARA_HIFI_WATCHDOG_CFG_STRU));

    /* ��ȡHIFI�µ�ʹ������NVֵ */
    uwRet = UCOM_NV_Read(en_NV_HifiPowerOffCfg, &stHifiPowerOffCfg, sizeof(CODEC_NV_PARA_HIFI_POWEROFF_CFG_STRU));

    /* ���NV��ȡʧ�ܣ���ȡĬ��ֵ */
    if( NV_OK != uwRet )
    {
        UCOM_SET_POWEROFF_ENABLE(VOS_TRUE);
        UCOM_SET_WI_TIME_THD(UCOM_HIFI_WI_TIME_THD);
        UCOM_SET_PD_TIME_THD(UCOM_HIFI_PD_TIME_THD);
    }
    else
    {
        /* ���NV��ȡ�ɹ�������ȫ�ֱ��� */
        UCOM_SET_POWEROFF_ENABLE(stHifiPowerOffCfg.uhwPowerOffEnable);
        UCOM_SET_WI_TIME_THD(stHifiPowerOffCfg.uwWITimeThd);
        UCOM_SET_PD_TIME_THD(stHifiPowerOffCfg.uwPDTimeThd);
    }

    /* ��ȡHIFI watchdog ����NVֵ */
    uwRet = UCOM_NV_Read(en_NV_HifiWatchDogCfg, &stHifiWatchDogCfg, sizeof(CODEC_NV_PARA_HIFI_WATCHDOG_CFG_STRU));
    /* ���NV��ȡʧ�ܣ���ȡĬ��ֵ */
    if( NV_OK != uwRet )
    {
        UCOM_SET_WATCHDOG_ENABLE(VOS_TRUE);
        UCOM_SET_WD_TIMEOUT_LEN(DRV_WATCHDOG_TIMEOUT_LEN);
    }
    else
    {
        /* ���NV��ȡ�ɹ�������ȫ�ֱ��� */
        UCOM_SET_WATCHDOG_ENABLE(stHifiWatchDogCfg.uhwWDEnable);
        UCOM_SET_WD_TIMEOUT_LEN(stHifiWatchDogCfg.uwWDTimeoutLen);
    }

    return;
}


VOS_VOID UCOM_LOW_PowerDRF(VOS_VOID)
{
    VOS_UINT32          uwBitEnable;

    /* ��ѯ��ǰhifi״̬��hifi��������ͨ��״̬ʱ��ֱ�ӷ���  */
    if (VOS_ERR == VOICE_ApiIsIdle())
    {
        return;
    }

    OM_CPUVIEW_EnterArea((VOS_UCHAR)OM_CPUVIEW_AREA_UCOM_DRF);

    uwBitEnable = VOS_GetInterruptMask();

    /* ���������ж� */
    VOS_SetInterruptMask(0);

    /* hifi������Ƶ����״̬ʱ����MCU����IPC�жϣ�Ҫ��DDR������ˢ��ģʽ */
    DRV_IPC_TrigInt(DRV_IPC_MCU_CORE,IPC_MCU_INT_SRC_HIFI_DDR_VOTE);

    /*��WFI֮ǰ�رտ��Ź�*/
    DRV_WATCHDOG_Stop();

    /* ����hifi����WFI״̬������֮ǰ�����������жϣ���ֻ��NMI�жϿɴ�ϸ�״̬ */
    UCOM_SET_WFI_NMI(0);

    /*��WFI�������Ź�*/
    DRV_WATCHDOG_Start(DRV_WATCHDOG_TIMEOUT_LEN);

    VOS_SetInterruptMask(uwBitEnable);

    OM_CPUVIEW_ExitArea((VOS_UCHAR)OM_CPUVIEW_AREA_UCOM_DRF);

    return;

}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

