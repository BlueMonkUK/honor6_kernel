/*************************************************************************
*   ��Ȩ����(C) 1987-2004, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  CLK.h
*
*   ��    �� :  liumengcun
*
*   ��    �� :  �û�ʱ��ͷ�ļ�
*
*   �޸ļ�¼ :  2009��3��5��  v1.00  liumengcun  ����
*
*************************************************************************/

#ifndef _CLK_H_
#define _CLK_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "bsp_memmap.h"
#include "soc_clk.h"
#include "soc_interrupts.h"
#include <soc_timer.h>

#ifndef _ASMLANGUAGE
typedef  void (*SYSCLKFUNCPTR)(int);
#endif

/* frequency of counter/timers */
#define SYS_TIMER_CLK           CCORE_SYS_TIMER_CLK
#define SYSTIMER_BASE_ADDR      CCORE_SYS_TIMER_BASE_ADDR
#define INT_LVL_SYSTIMER        CCORE_SYS_TIMER_INT_LVL

//#define CLK_DEFULT_TICKS_PERSECOND 100

#define AMBA_RELOAD_TICKS    0

#define CLK_REGOFF_LOAD	     0x000	/* Load (R/W) */
#define CLK_REGOFF_VALUE     0x004	/* Value (R/O) */
#define CLK_REGOFF_CTRL      0x008	/* Control (R/W) */
#define CLK_REGOFF_CLEAR     0x00C	/* Clear (W/O) */
#define CLK_REGOFF_INTSTATUS 0x010	/* INT STATUS (R/O) */


/* ��ʱ��1 ���������ƼĴ���,����ɶ�ʱ�������������󣬸üĴ����Զ�����*/
#define CLK_DEF_TC_UNLOCK	0       /* 0��������*/
#define CLK_DEF_TC_LOCK		(1<<3)  /* 1������ʱ���ĵ�ǰֵ������TIMER1_CURRENTVALUE��*/

/* TimerXRIS�ж�����λ*/
#define CLK_DEF_TC_INTENABLE	0       /* 0�������θ��ж�*/
#define CLK_DEF_TC_INTDISABLE	(1<<2)  /* 1�����θ��ж� */

/* ��ʱ���ļ���ģʽλ*/
#define CLK_DEF_TC_PERIODIC	(1<<1)          /* 1��user-defined count mode*/
#define CLK_DEF_TC_FREERUN	0               /* 0��free-running mode*/

/* ��ʱ��ʹ��λ*/
#define CLK_DEF_TC_ENABLE	1     /* 1��Timerʹ��*/
#define	CLK_DEF_TC_DISABLE	0     /* 0��Timer��ֹ*/

/* �û�ʱ��ʹ��*/
#define CLK_DEF_ENABLE   (CLK_DEF_TC_PERIODIC | CLK_DEF_TC_INTENABLE | CLK_DEF_TC_ENABLE)
/* �û�ʱ��ȥʹ��*/
#define CLK_DEF_DISABLE  (CLK_DEF_TC_PERIODIC | CLK_DEF_TC_INTDISABLE | CLK_DEF_TC_DISABLE)

/*ʱ���ж�ʹ��*/
#ifndef TIMER_INT_ENABLE
#define TIMER_INT_ENABLE(level) intEnable (level)
#endif
/*ʱ���ж�ȥʹ��*/
#ifndef TIMER_INT_DISABLE
#define TIMER_INT_DISABLE(level) intDisable (level)
#endif


#ifdef __cplusplus
}
#endif

#endif /*end #define _CLK_H_*/
