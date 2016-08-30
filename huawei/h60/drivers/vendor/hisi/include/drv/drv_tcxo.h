/*************************************************************************
*   ��Ȩ����(C) 1987-2011, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  drv_clk.h
*
*   ��    �� :  xujingcui
*
*   ��    �� :  ���ļ�����Ϊ"drv_clk", ����V7R2�����Э��ջ֮���CLK_API�ӿ�
*
*   �޸ļ�¼ :  2013��1��18��  v1.00 xujingcui����
*************************************************************************/
#ifndef __DRV_TCXO_H__
#define __DRV_TCXO_H__

#include <drv_dpm.h>
typedef BSP_S32 (*PWC_TCXO_FUNCPTR)(int);


 int BSP_TCXO_RfclkStatus(PWC_COMM_MODEM_E enModemId);
#define DRV_TCXO_RFCLKSTATUS(enModemId) BSP_TCXO_RfclkStatus(enModemId)

 int BSP_TCXO_RfclkEnable(PWC_COMM_MODEM_E  enModemId, PWC_COMM_MODE_E enModeType);
#define DRV_TCXO_RFCLKENABLE(enModemId, enModeType) BSP_TCXO_RfclkEnable(enModemId, enModeType)

 int BSP_TCXO_RfclkDisable(PWC_COMM_MODEM_E  enModemId, PWC_COMM_MODE_E enModeType);
#define DRV_TCXO_RFCLKDISABLE(enModemId, enModeType) BSP_TCXO_RfclkDisable(enModemId, enModeType)

 int bsp_tcxo_getstatus(PWC_COMM_MODEM_E enModemId);
#define DRV_TCXO_GETSTATUS(enModemId)   bsp_tcxo_getstatus(enModemId)

 int bsp_tcxo_enable(PWC_COMM_MODEM_E  enModemId, PWC_COMM_MODE_E enModeType);
#define DRV_TCXO_ENABLE(enModemId, enModeType)  bsp_tcxo_enable(enModemId, enModeType)

 int bsp_tcxo_disable(PWC_COMM_MODEM_E  enModemId, PWC_COMM_MODE_E enModeType);
#define DRV_TCXO_DISABLE(enModemId, enModeType) bsp_tcxo_disable(enModemId, enModeType)

 int bsp_tcxo_timer_start(PWC_COMM_MODEM_E  enModemId, PWC_COMM_MODE_E enModeType, PWC_TCXO_FUNCPTR routine, int arg, unsigned int timerValue);
#define DRV_TCXO_TIMER_START(enModemId, enModeType, routine, arg, timerValue)   bsp_tcxo_timer_start(enModemId, enModeType, routine, arg, timerValue)





#endif
