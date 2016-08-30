

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "ucom_comm.h"
#include "med_drv_interface.h"
#include "ucom_share.h"

#ifndef __UCOM_LOW_POWER_H__
#define __UCOM_LOW_POWER_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/

/* �µ�������ʱ������HIFI���г�����ʱ������Ӧ�����µ磬��λ((16384*10000)/DRV_TIMER_OM_FREQ)ms = 500ms */
#define UCOM_HIFI_PD_TIME_THD           (16384)

/* �͹���ģʽ������ʱ������HIFI���г�����ʱ������Ӧ����͹���ģʽ����λ((33*10000)/DRV_TIMER_OM_FREQ)ms = 1ms */
#define UCOM_HIFI_WI_TIME_THD           (33)

#define UCOM_IS_FIRST_IDLE()            (g_uwUcomPowerCtrl.uwIsFirstIdle != 0x5A5A5A5A)
#define UCOM_GET_LAST_IDLE_TIME()       (g_uwUcomPowerCtrl.uwLastIdleTime)
#define UCOM_GET_LAST_SW_CNT()          (g_uwUcomPowerCtrl.uwLastSwCnt)
#define UCOM_SET_FIRST_IDLE()           (g_uwUcomPowerCtrl.uwIsFirstIdle = 0x5A5A5A5A)
#define UCOM_SET_LAST_IDLE_TIME(uwVar)  (g_uwUcomPowerCtrl.uwLastIdleTime = (uwVar))
#define UCOM_SET_LAST_SW_CNT(uwVar)     (g_uwUcomPowerCtrl.uwLastSwCnt = (uwVar))

#define UCOM_GET_POWEROFF_ENABLE()       (g_uwUcomPowerCtrl.uhwPowerOffEnable)
#define UCOM_SET_POWEROFF_ENABLE(uwVar)  (g_uwUcomPowerCtrl.uhwPowerOffEnable = uwVar)
#define UCOM_GET_WI_TIME_THD()           (g_uwUcomPowerCtrl.uwWITimeThd)
#define UCOM_SET_WI_TIME_THD(uwVar)      (g_uwUcomPowerCtrl.uwWITimeThd = uwVar)
#define UCOM_GET_PD_TIME_THD()           (g_uwUcomPowerCtrl.uwPDTimeThd)
#define UCOM_SET_PD_TIME_THD(uwVar)      (g_uwUcomPowerCtrl.uwPDTimeThd = uwVar)
#define UCOM_GET_WATCHDOG_ENABLE()       (g_uwUcomPowerCtrl.uhwWatchDogEnable)
#define UCOM_SET_WATCHDOG_ENABLE(uwVar)  (g_uwUcomPowerCtrl.uhwWatchDogEnable = uwVar)
#define UCOM_GET_WD_TIMEOUT_LEN()        (g_uwUcomPowerCtrl.uwWDTimeoutLen)
#define UCOM_SET_WD_TIMEOUT_LEN(uwVar)   (g_uwUcomPowerCtrl.uwWDTimeoutLen = uwVar)

#if (VOS_CPU_TYPE == VOS_HIFI)
#define UCOM_SET_WFI(var1)              asm ("waiti 0": :)                      /* ����Wait for interruptģʽ */
#define UCOM_SET_WFI_NMI(var1)          asm ("waiti 5": :)                      /* ����Wait for interruptģʽ��ֻ��ӦNMI�ж� */
#else
#define UCOM_SET_WFI(var1)
#define UCOM_SET_WFI_NMI(var1)
#endif
/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
/*****************************************************************************
 ʵ������  : UCOM_POWER_CTRL_STRU
 ��������  : HIFI���µ���ƽṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT32      uwIsFirstIdle;      /* �����Ƿ�ϵͳ��һ�γ�ʼ����� */
    VOS_UINT32      uwLastIdleTime;     /* ��һ�ν�����й��ӵ�ϵͳʱ��  */
    VOS_UINT32      uwLastSwCnt;        /* ��һ�ν�����й��ӵ��������л�����  */
    VOS_UINT16      uhwPowerOffEnable;  /* HIFI�µ��Ƿ�ʹ�� */
    VOS_UINT16      uhwWatchDogEnable;  /* HIFI watchdog�Ƿ�ʹ�� */
    VOS_UINT32      uwWITimeThd;        /* ����͹���ģʽ������ʱ�� */
    VOS_UINT32      uwPDTimeThd;        /* �µ�������ʱ�� */
    VOS_UINT32      uwWDTimeoutLen;     /* watchdog timeoutʱ�� */
}UCOM_POWER_CTRL_STRU;
/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern UCOM_POWER_CTRL_STRU                    g_uwUcomPowerCtrl;


/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_VOID UCOM_LOW_PowerInit(VOS_VOID);
extern VOS_VOID UCOM_LOW_PowerDRF(VOS_VOID);
extern VOS_VOID UCOM_LOW_PowerNmiHook(   );
extern VOS_VOID UCOM_LOW_PowerTask(VOS_UINT32 ulVosCtxSw);



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of ucom_low_power.h */
