#ifndef __SOC_TIMER_ENUM__
#define __SOC_TIMER_ENUM__
/*������drv_timer.h�����ֲ�ͬ��Ʒ��̬*/
/***************1. acore timer************************/
/*timer 1/4/5/6/7Ϊk3�ָ�modem�ķǵ���timer*/
/*timer 13/14/15Ϊk3�ָ�modem�ĵ���timer*/
#define   ACPU_OSA_TIMER                               1
#define   ACPU_SOFTTIMER_NOWAKE                        13

#define   ACPU_VIEW_TIMER                              14
#define   ACPU_OM_TIMER                                15
#define   ACPU_WDT_TIMER                               1000
#define   ACPU_SYSTIMER                                1000
#define   ACPU_SOURCE_TIMER                            1000

/***************2. ccore timer************************/
#define   CCPU_DSP_DRX_TIMER          14/*ap timer20*/
#define   UDELAY_TIMER                1
#define   CCPU_VIEW_TIMER             2
#define   CCPU_DRX1                   3
#define   CCPU_SYSTIMER               4

#define   CCPU_SOFTTIMER_NOWAKE       5
#define   CCPU_DRX_TIMER              8/*ap timer30*/
#define   CCPU_OSA_TIMER              9/*ap timer31*/
#define   DSP_TIMER1                  7
#define   DSP_TIMER2                  0

#define   MCU_TIMER                   21/*ap timer21*/

/***************3. other*****************************/

#define   ACPU_SOFTTIMER_WAKE               1000
#define   CCPU_SOFTTIMER_WAKE               1000
#define   CCPU_DRX2                         1000
#define   CCPU_WDT_TIMER                    1000
//ʱ���timer��ϵͳ�������е�timer����
#define   STAMP_TIMER                       1000
//K3v3 usb balong�����ṩ�������ṩtimer
#define   USB_TIMER                         1000
#define   HIFI_TIMER1                       1000
#define   HIFI_TIMER2                       1000
#endif

