

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "med_drv_interface.h"

#ifndef __DRV_UART_H__
#define __DRV_UART_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/

#define DRV_UART_RBR_THR_DLL            (DRV_UART_BASE_ADDR + 0x000)            /* UART���շ������ݺͲ����ʵ�8λ */
#define DRV_UART_DLH_IER                (DRV_UART_BASE_ADDR + 0x004)            /* UART�ж�ʹ�ܺͲ����ʸ�8λ     */
#define DRV_UART_IIR_FCR                (DRV_UART_BASE_ADDR + 0x008)            /* UART�ж�ID��fifo���ƼĴ���    */
#define DRV_UART_LCR                    (DRV_UART_BASE_ADDR + 0x00C)            /* UART���Կ��ƼĴ���            */
#define DRV_UART_MCR                    (DRV_UART_BASE_ADDR + 0x010)            /* UART Modem���ƼĴ���          */
#define DRV_UART_LSR                    (DRV_UART_BASE_ADDR + 0x014)            /* UART����״̬�Ĵ���            */
#define DRV_UART_MSR                    (DRV_UART_BASE_ADDR + 0x018)            /* Modem״̬�Ĵ���               */
#define DRV_UART_SCR                    (DRV_UART_BASE_ADDR + 0x01C)            /* �м����ݴ�Ĵ���            */
#define DRV_UART_FAR                    (DRV_UART_BASE_ADDR + 0x070)            /* FIFO���ʿ��ƼĴ���            */
#define DRV_UART_TFR                    (DRV_UART_BASE_ADDR + 0x074)            /* ����FIFO���Ĵ���              */
#define DRV_UART_RFW                    (DRV_UART_BASE_ADDR + 0x078)            /* ����FIFOд�Ĵ���              */
#define DRV_UART_USR                    (DRV_UART_BASE_ADDR + 0x07C)            /* UART״̬�Ĵ���                */
#define DRV_UART_TFL                    (DRV_UART_BASE_ADDR + 0x080)            /* ����FIFO���ݸ����Ĵ���        */
#define DRV_UART_RFL                    (DRV_UART_BASE_ADDR + 0x084)            /* ����FIFO���ݸ����Ĵ���        */
#define DRV_UART_HTX                    (DRV_UART_BASE_ADDR + 0x0A4)            /* ��ͣ���ͼĴ���                */
#define DRV_UART_DMASA                  (DRV_UART_BASE_ADDR + 0x0A8)            /* DMA�����Ӧ�Ĵ���             */
#define DRV_UART_CPR                    (DRV_UART_BASE_ADDR + 0x0F4)            /* ���ò����Ĵ���                */
#define DRV_UART_UCV                    (DRV_UART_BASE_ADDR + 0x0F8)            /* UART�汾�Ĵ���                */
#define DRV_UART_CTR                    (DRV_UART_BASE_ADDR + 0x0FC)            /* ����ID�Ĵ���                  */

#define DRV_UART_WAIT_CNT               (0x100)                                 /* UART��ͻʱ�ȴ����� */

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

extern VOS_VOID DRV_UART_Init(VOS_VOID);
extern VOS_VOID DRV_UART_SendData(VOS_UCHAR *pucData, VOS_UINT32 uwDataLen);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of med_drv_uart.h */
