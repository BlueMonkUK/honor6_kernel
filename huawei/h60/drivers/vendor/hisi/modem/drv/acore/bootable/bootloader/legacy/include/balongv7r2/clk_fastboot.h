/*************************************************************************
*   ��Ȩ����(C) 1987-2011, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� : clk_fastboot.h
*
*   ��    �� :  xujingcui
*
*   ��    �� : ���ļ����ڹر�ϵͳʱ�Ӳ����еķ��ü���IPʱ��
*
*   �޸ļ�¼ :  2013��8��10��  v1.00 xujingcui����
*************************************************************************/
#ifndef __CLK_FASTBOOT_H__
#define __CLK_FASTBOOT_H__
#include <hi_syscrg_interface.h>
#include <hi_pwrctrl_interface.h>

#ifdef CONFIG_V7R2_CLK_CRG
#define  HI_CRG_DISABLE_OFFSET_01    HI_CRG_CLKDIS1_OFFSET      /*0X4*/
#define  HI_CRG_DISABLE_OFFSET_02    HI_CRG_CLKDIS2_OFFSET      /*0X10*/
#define  HI_CRG_DISABLE_OFFSET_03    HI_CRG_CLKDIS3_OFFSET      /*0X1C*/
#define  HI_CRG_DISABLE_OFFSET_04    HI_CRG_CLKDIS4_OFFSET      /*0X28*/
#define  HI_CRG_DISABLE_OFFSET_05    HI_CRG_CLKDIS5_OFFSET      /*0X34*/

#define  HI_CRG_RESETEN_OFFSET_02   HI_CRG_SRSTEN2_OFFSET      /*0x6C*/

#define HI_PWRCTR_ISO_CELL_OFFSET_01   HI_PWR_CTRL4_OFFSET   /*0XC10*/
#define HI_PWRCTR_MTCOMS_OFFSET            HI_PWR_CTRL7_OFFSET   /*0XC1C*/

#endif

/****************************************************************************
*�˺������ڹر�ʱ�ӷ��ü��ز����е�IPʱ��
**/
void bsp_disable_non_on_clk(void);

#endif
