

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "med_drv_interface.h"
#include "med_drv_timer_hifi.h"


#ifndef __DRV_WATCHDOG_H__
#define __DRV_WATCHDOG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define DRV_WATCHDOG_LOAD               (DRV_WATCHDOG_BASE_ADDR + 0x000)        /* SP805 WATCHDOG������ֵ��д�üĴ���������¿�ʼ��������С��ЧֵΪ1 */
#define DRV_WATCHDOG_VALUE              (DRV_WATCHDOG_BASE_ADDR + 0x004)        /* SP805 WATCHDOG��������ǰֵ */
#define DRV_WATCHDOG_CONTROL            (DRV_WATCHDOG_BASE_ADDR + 0x008)        /* SP805 WATCHDOG���ƼĴ��� */
#define DRV_WATCHDOG_INTCLR             (DRV_WATCHDOG_BASE_ADDR + 0x00C)        /* SP805 WATCHDOG����жϼĴ�����д������ֵ������ж����������ֵ��ʼ���� */
#define DRV_WATCHDOG_RIS                (DRV_WATCHDOG_BASE_ADDR + 0x010)        /* SP805 WATCHDOGԭʼ�ж�״̬�Ĵ��� */
#define DRV_WATCHDOG_MIS                (DRV_WATCHDOG_BASE_ADDR + 0x014)        /* SP805 WATCHDOG���κ��ж�״̬�Ĵ��� */
#define DRV_WATCHDOG_LOCK               (DRV_WATCHDOG_BASE_ADDR + 0xC00)        /* SP805 WATCHDOG�����Ĵ��������ڽ�ֹ�����������Ĵ�����д���� */

#define DRV_WATCHDOG_CLOCK_ENABLE       (DRV_AO_SC_BASE_ADDR + 0x630)           /* SP805 WATCHDOG����ʱ��ʹ�ܼĴ��� */
#define DRV_WATCHDOG_CLOCK_DISABLE      (DRV_AO_SC_BASE_ADDR + 0x634)           /* SP805 WATCHDOG����ʱ�ӽ�ֹ�Ĵ��� */

#define DRV_WATCHDOG_LOCK_NUM           (0x0)                                   /* SP805 WATCHDOG������ */
#define DRV_WATCHDOG_UNLOCK_NUM         (0x1ACCE551)                            /* SP805 WATCHDOG������ */

#define DRV_WATCHDOG_CONTROL_DISABLE    (0x0)                                   /* SP805 WATCHDOG������:�����жϲ�ֹͣ���´δ�ʱ���Զ����¼��� */
#define DRV_WATCHDOG_CONTROL_ENABLE     (0x3)                                   /* SP805 WATCHDOG������:���ж����β����� */

#define DRV_WATCHDOG_INTCLR_NUM         (0x4455)                                /* SP805 WATCHDOG�ж�����֣�ʵ����д������ֵ������� */

#define DRV_WATCHDOG_TIMEOUT_LEN        (32)                                    /* ���Ź�ʱ��,��λ:���� */

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
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/
#ifdef _USING_WATCHDOG_
/*****************************************************************************
 �� �� ��  : DRV_WATCHDOG_Start
 ��������  : Watch Dog������, HiFiʹ��DW_APB_TIMER0�Ķ�ʱ��4��Ϊwatch dog
 �������  : uwTimeLen - ʱ��,��λms,32Kʱ��ʱ���㹫ʽΪuwTimerLen*32768/1000;
             ����ֵ���ó���0xfffff��������1-10000֮��
 �������  : ��
 �� �� ֵ  : VOS_VOID
*****************************************************************************/
#define DRV_WATCHDOG_Start(uwTimeLen)                           \
    DRV_WATCHDOG_StartWdg((VOS_UINT32)((uwTimeLen)*4096)/125)
/*****************************************************************************
 �� �� ��  : DRV_WATCHDOG_Feed
 ��������  : ι�����������ż໤
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
*****************************************************************************/
#define DRV_WATCHDOG_Feed()                                     \
    DRV_WATCHDOG_RestartWdg()
/*****************************************************************************
 �� �� ��  : DRV_WATCHDOG_Stop
 ��������  : Watch Dog��ֹͣ
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
*****************************************************************************/
#define DRV_WATCHDOG_Stop()                                     \
    DRV_WATCHDOG_StopWdg()
#else

#define DRV_WATCHDOG_Start(uwTimeLen)
#define DRV_WATCHDOG_Feed()
#define DRV_WATCHDOG_Stop()

#endif

/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/

extern VOS_VOID DRV_WATCHDOG_RestartWdg(VOS_VOID);
extern VOS_VOID DRV_WATCHDOG_StartWdg(VOS_UINT32 uwLoadCnt);
extern VOS_VOID DRV_WATCHDOG_StopWdg(VOS_VOID);










#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of med_drv_watchdog.h */
