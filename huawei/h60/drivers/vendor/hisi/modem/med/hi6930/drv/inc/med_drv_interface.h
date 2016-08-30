

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "ucom_config.h"
#include "soc_baseaddr_interface.h"
#include "soc_sctrl_interface.h"
#include "drv_ipc_enum.h"
#include "drv_mailbox.h"
#include "product_config.h"

#ifndef __DRVINTERFACE_H__
#define __DRVINTERFACE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define DRV_SC_CRG_BASE_ADDR            (HI_SYSCTRL_BASE_ADDR)                  /* SC����ַ */
#define DRV_SOCP_BASE_ADDR              (HI_SOCP_REGBASE_ADDR)                  /* SOCP����ַ */
#define DRV_IPC_BASE_ADDR               (SOC_IPC_S_BASE_ADDR)                   /* IPC����ַ */
#define DRV_DMA_BASE_ADDR               (SOC_AP_DMAC_BASE_ADDR)                 /* AP��DMA����ַ������ */
#define DRV_UART_BASE_ADDR              (SOC_UART3_BASE_ADDR)                   /* HIFIʹ�õ�UART3����ַ */
#define DRV_WATCHDOG_BASE_ADDR          (SOC_Watchdog1_BASE_ADDR)               /* HIFIʹ�õ�WDG3����ַ */

#define DRV_TIMER_DWAPB_WATCHDOG_ADDR   (0)                                     /* HIFI Watch dog ��ǰ������ */
#define DRV_TIMER_DWAPB_WATCHDOGR_IDX   (0)                                     /* HIFI Watch dog ��ʱ���ڶ�ʱ���豸��������Ϊ4 */
#define DRV_TIMER_DWAPB_WATCHDOG_FREQ   (32767U)                                /* HIFI Watch dog ʱ��Ƶ��Ϊ32K */

#define DRV_TIMER_SC_SLICE_ADDR         (SOC_AO_SCTRL_SC_SLICER_COUNT0_ADDR(VOS_NULL))
#define DRV_TIMER_SC_SLICE_FREQ         (32767U)

#define DRV_TIMER_DWAPB_HIFI_ADDR       (SOC_HIFI_Timer00_BASE_ADDR)            /* V7R2��HIFI������ʱ��ʹ��TIMER0 */
#define DRV_TIMER_DWAPB_HIFI_FREQ       (32767U)                                /* V7R2��HIFI������ʱ��ʱ��Ƶ�ʣ���λ(Hz) */
#define DRV_TIMER_UNLINK_HIFI_ADDR      (SOC_HIFI_Timer08_BASE_ADDR)            /* V7R2��PC VOICE������ʱ��Ԥ��ʹ��TIMER8 */
#define DRV_TIMER_UNLINK_HIFI_FREQ      (32767U)                                /* V7R2��HIFI������ʱ��ʱ��Ƶ�ʣ���λ(Hz) */

#define DRV_SOCP_CHAN_START_ADDR_HIFI   ((DDR_HIFI_ADDR + 0x200000) - 0x2000) /* hifi ǰ2M�����������8K�ڴ���ΪHifi��ά�ɲ�ԴBuff */

#define DRV_IPC_CORE_ACPU               (MAILBOX_CPUID_ACPU)

#define DRV_TDSCDMA_CTRL_REG_BASE_ADDR   (SOC_BBP_TDS_BASE_ADDR)                           /* HIFI��ȡTDS-CDMA��ؼĴ����Ļ���ַ */
#define DRV_TDSCDMA_CTRL_EN_ADDR         (DRV_TDSCDMA_CTRL_REG_BASE_ADDR +(0xa0)) /* TDS-CDMA֡�����Ĵ����ж�ʹ�ܵ�ַ����0λ���� */
#define DRV_TDSCDMA_CTRL_INT_CLEAR_ADDR  (DRV_TDSCDMA_CTRL_REG_BASE_ADDR +(0xa8)) /* HIFI��TDS-CDMA���жϼĴ�����ַ����0λ���� */
#define DRV_TDSCDMA_FRM_CNT_REG_ADDR     (DRV_TDSCDMA_CTRL_REG_BASE_ADDR +(0x204)) /* HIFI��ȡTDS-CDMA֡�����Ĵ�����ӳ���ַ����16λ��Ч */
#define DRV_TDSCDMA_CTRL_SYNC_INTR       (OS_INTR_CONNECT_07)

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


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/












#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of med_drv_interface.h */
