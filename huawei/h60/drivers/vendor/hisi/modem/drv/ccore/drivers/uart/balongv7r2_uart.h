/* balongv7r2_uart.h - balong  AMBA UART header file */

/* Copyright (C) 2012 Huawei Corporation */

/*
modification history
*/

#ifndef __BALONGV7R2_UART_M_H 
#define __BALONGV7R2_UART_M_H 

#ifdef __cplusplus
extern "C" {
#endif

#include <sioLib.h>
#include "product_config.h"
#include "osl_types.h"

/* Register description OF ARM AMBA UART */

#ifndef _ASMLANGUAGE
/* �����ʶ�Ӧ��*/
#define AMBA_UART_MAX_BAUT_RATE 921600
#define AMBA_UART_MIN_BAUT_RATE 1

typedef struct
{
    /* must be first */
    SIO_CHAN	sio;		    /* standard SIO_CHAN element */

    /* callbacks */
    STATUS	(*get_tx_char) ();  /* installed Tx callback routine */
    STATUS	(*put_rcv_char) (); /* installed Rx callback routine */
    void *	get_tx_arg;	        /* argument to Tx callback routine */
    void *	put_rcv_arg;	    /* argument to Rx callback routine */

    u32 *	regs;		        /* AMBA registers */
    u32 	level_rx;	        /* Rx Interrupt level for this device */
    u32 	level_tx;	        /* Tx Interrupt level for this device */

    u32	    channel_mode;	    /* such as INT, POLL modes */
    s32		baud_rate;	        /* the current baud rate */
    u32	    xtal;		        /* UART clock frequency */
    
    s32     options;            /* hardware options*/
    s32     fifo_options;        /* FIFO options*/
} AMBA_UART_CHAN;

/*****************************************************************************
* �� �� ��     :  balongv7r2_uart_irq_handler
*
* ��������  :  �����ܵ��ж���Ӧ����
*
* �������  :  AMBA_UART_CHAN  :��Ӧͨ��

* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
void balongv7r2_uart_irq_handler (AMBA_UART_CHAN *ptr_chan);


/*****************************************************************************
* �� �� ��     :  bsp_uart_init
*
* ��������  :  ��ʼ������
*
* �������  :  AMBA_UART_CHAN  :��Ӧͨ��

* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
void balongv7r2_uart_init(AMBA_UART_CHAN *	ptr_chan);

#endif	/* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif
 
#endif /* __INCprimeCellSioh */

