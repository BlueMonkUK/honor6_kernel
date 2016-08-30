/*
 * balongv7r2_uart.c -- balong uart driver
 *
 * Copyright (C) 2012 Huawei Corporation
 *
 *author:wangxiandong
 */

/*******************************************************************/
/*                                              ͷ�ļ�                                              */
/*******************************************************************/
#include <vxWorks.h>
#include <intLib.h>
#include <errnoLib.h>
#include <sioLib.h>
#include "osl_bio.h"                     /* �ṩ�Ĵ�����д�ӿ�*/
#include "hi_uart.h"                     /* �Ĵ���оƬ���ݶ���,A/M����*/
#include "balongv7r2_uart.h"
#include <bsp_shared_ddr.h>
#include "bsp_om.h"                    /* axi �ڴ��ʼ��*/

/*lint --e{30, 40, 49, 63, 64, 145, 512, 522, 528, 550, 571, 958, 734, 746 } */
int balongv7r2_uart_suspend(void);
int balongv7r2_uart_resume(void);

/*
*-e63����ֵ�Ϸ�
*-e30������ϵͳ��
*-e49 -e145������vxworks��ʽ����
*/
/*******************************************************************/
/*                                              �궨��                                              */
/*******************************************************************/


/*******************************************************************/
/*                                              ��������                                          */
/*******************************************************************/

/*****************************************************************************
* �� �� ��     :  balongv7r2_uart_rx_int_proc
*
* ��������  :  ���ڽ����жϴ�����
*
* �������  :  AMBA_UART_CHAN  :��Ӧͨ��
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static void balongv7r2_uart_rx_int_proc(AMBA_UART_CHAN *	ptr_chan)
{
    char in_char;
    unsigned flags;
    int i;

    /* FIFO�����16��byte */
    for(i = 0; i < UART_FIFO_SIZE; i++)
    {
        flags = readl((u32)(ptr_chan)->regs + UART_REGOFF_USR);
		if((flags & UART_USR_FIFO_NOT_EMP) != 0)
		{
			/* Read from data register. */
            in_char = readl((u32)(ptr_chan)->regs + UART_REGOFF_RBR);
            (void)(*ptr_chan->put_rcv_char) (ptr_chan->put_rcv_arg, in_char); /*!lint !e746 */
		}
		else
		{
			return;
		}
    }
}

/*****************************************************************************
* �� �� ��     :  balongv7r2_uart_get_data_len
*
* ��������  :  �������ݳ�������
*
* �������  :  s32  :data_len
* �������  :  ��Ӧ�ĳ��Ⱥ�
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static s32 balongv7r2_uart_get_data_len(s32 data_len)
{
    s32 ul_lcr = 0;

    switch (data_len)
	{
	    case CS5:
	        ul_lcr = UART_LCR_DLS_5BITS; /* 5 bit*/
	        break;
	    case CS6:
	        ul_lcr = UART_LCR_DLS_6BITS; /* 6 bit*/
	        break;
	    case CS7:
	        ul_lcr = UART_LCR_DLS_7BITS; /* 7 bit*/
	        break;
	    case CS8:
        default:
	        ul_lcr = UART_LCR_DLS_8BITS; /* 8 bit*/
	        break;
	}

	return ul_lcr;

}

/*****************************************************************************
* �� �� ��     :  balongv7r2_uart_par_set
*
* ��������  :  �������ݳ�������
*
* �������  :  s32  :para_options
* �������  :  ��Ӧ��У��ֵ
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static s32 balongv7r2_uart_par_set(s32 para_options)
{
    s32 para_set = 0;

    switch (para_options)
    {
        case PARENB | PARODD:   /* żУ��ʹ��*/
            para_set = (UART_LCR_PEN | UART_LCR_EPS);
            break;
        case PARENB:            /* ��У��ʹ�� */
            para_set = UART_LCR_PEN;
            break;
        case PARODD:            /* invalid mode, not normally used. */
        case 0:                 /* ����żУ�� */
        default:
            para_set = UART_LCR_DEFAULTVALUE;
            break;
    }

	return para_set;

}

/*****************************************************************************
* �� �� ��     :  balongv7r2_uart_is_busy
*
* ��������  :  ���ڶ˿��Ƿ�æ״̬
*
* �������  :  AMBA_UART_CHAN * channel
* �������  :  1:æ
*                           0:��æ
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static s32 balongv7r2_uart_is_busy(AMBA_UART_CHAN * channel)
{
    u32 i = 0;
    u32 ul_discard = 0;

    /* ��ȡUART ״̬�Ĵ�������UART�Ƿ���idle״̬���������busy״̬��ȴ�*/
    for(i = 0; i < 10000; i++)
    {
        ul_discard = readl((u32)(channel)->regs + UART_REGOFF_USR);
        /* ���FIFO�������ݣ�����*/
        if(ul_discard & UART_USR_UART_BUSY)
        {
            balongv7r2_uart_rx_int_proc(channel);
        }
        else
        {
            break;
        }

    }
    /* ���10000�γ�ʱ*/
    if (i == 10000)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

/*****************************************************************************
* �� �� ��     :  balongv7r2_uart_is_busy
*
* ��������  :  ���ڶ˿��Ƿ�æ״̬
*
* �������  :  AMBA_UART_CHAN * channel
* �������  :  1:æ
*                           0:��æ
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static s32 balongv7r2_uart_mode_set(AMBA_UART_CHAN * ptr_chan, s32 arg)
{
    unsigned  value_ier = 0;

    if (arg == SIO_MODE_INT)
    {
        if(*(u32*)SHM_MEM_CHSELL_FLAG_ADDR == PRT_FLAG_EN_MAGIC_A)
        {
            intDisable ((int)ptr_chan->level_rx);
        }
        else
        {
            value_ier = readl((u32)(ptr_chan)->regs + UART_REGOFF_IER);
            value_ier &= ~( UART_IER_TX_IRQ_ENABLE | UART_IER_RX_IRQ_ENABLE | UART_IER_LS_IRQ_ENABLE);
            writel(value_ier, (u32)(ptr_chan)->regs + UART_REGOFF_IER);

            value_ier = readl((u32)(ptr_chan)->regs + UART_REGOFF_IER);
            value_ier |= UART_IER_RX_IRQ_ENABLE;
            writel(value_ier, (u32)(ptr_chan)->regs + UART_REGOFF_IER);
            /* Enable appropriate interrupts. */
            intEnable ((int)ptr_chan->level_rx);
        }
    }
    else
    {
        /* Disable all interrupts for this UART. */
        intDisable ((int)ptr_chan->level_rx);
        if (ptr_chan->level_tx != ptr_chan->level_rx)
        {
            intDisable ((int)ptr_chan->level_tx);
        }
        value_ier = readl((u32)(ptr_chan)->regs + UART_REGOFF_IER);
        value_ier &= ~( UART_IER_TX_IRQ_ENABLE | UART_IER_RX_IRQ_ENABLE | UART_IER_LS_IRQ_ENABLE);
        writel(value_ier, (u32)(ptr_chan)->regs + UART_REGOFF_IER);
    }
	return 0;
}


/*****************************************************************************
* �� �� ��     :  balongv7r2_uart_opts_set
*
* ��������  :  ��������
* �������  :  AMBA_UART_CHAN  :��Ӧͨ��
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static s32 balongv7r2_uart_opts_set(SIO_CHAN *  ptr_sio_chan, s32 options)
{
	FAST s32    int_old_level;		/* current interrupt level mask */
	u32         ul_lcr = 0;
	AMBA_UART_CHAN * ptr_chan = (AMBA_UART_CHAN *)ptr_sio_chan;
    s32 temp = 0;
	if (ptr_chan == NULL)
    {
		return ERROR;
    }

	/* ���ݳ���ѡ�� */
    ul_lcr = (u32)balongv7r2_uart_get_data_len(options & CSIZE);

	/* ֹͣλ����*/
    if (options & STOPB)
    {
        ul_lcr |= UART_LCR_STOP_2BITS; /* 2 stop bit */
    }
    else
    {
        ul_lcr |= UART_LCR_STOP_1BITS;	/* 1 stop bit */
    }


    /* У��λ���� */
    temp = options & (PARENB | PARODD);
    ul_lcr |= (u32)balongv7r2_uart_par_set(temp);


	/* ��ͣ����λ */
    if (options & CREAD)
	{
		ul_lcr |= UART_LCR_BC_UNNORMAL;/* ������豸������ͣ*/
	}
	else
	{
		ul_lcr |= UART_LCR_BC_NORMAL;  /* �����豸����*/
	}

    if(balongv7r2_uart_is_busy(ptr_chan))
    {
        return ERROR;
    }

	int_old_level = intLock ();

	/* д�봫���߿��ƼĴ���*/
    writel(ul_lcr, (u32)(ptr_chan)->regs + UART_REGOFF_LCR);
	intUnlock (int_old_level);

	ptr_chan->options = options;
	return OK;
}


/*****************************************************************************
* �� �� ��     :  balongv7r2_uart_ioctl
*
* ��������  :  ���ô��ڵĲ���
*
* �������  :   SIO_CHAN *	ptr_sio_chan:ptr to SIO_CHAN describing this channel
*                             int		    request         :request code
*                             int		    arg	            :some argument
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static s32 balongv7r2_uart_ioctl(SIO_CHAN * ptr_sio_chan, int request, int arg)
{
    int		oldlevel;	        /* current interrupt level mask */
    s32	status;		        /* status to return */
	u32  ulDivisor;
	u32  ulDivisorHigh;      /* �����ʸ�λ*/
	u32  ulDivisorLow;       /* �����ʵ�λ*/
	u32  ulLCR;              /* �����߿��ƼĴ���ֵ*/

    AMBA_UART_CHAN * ptr_chan = (AMBA_UART_CHAN *)ptr_sio_chan;

    status = OK;	            /* preset to return OK */

    switch (request)
    {
        case SIO_BAUD_SET:
        {
            if ((arg < AMBA_UART_MIN_BAUT_RATE) || (arg > AMBA_UART_MAX_BAUT_RATE))
			{
			   status = EIO;       /* baud rate out of range */
			   break;
			}

            /*begin set baudrate */
            /* ������=�ο�ʱ��/��16*divisor��*/
            ulDivisor = ptr_chan->xtal / (u32)(16 * arg);
            ulDivisorHigh = (ulDivisor & 0xFF00) >> 8;
            ulDivisorLow = ulDivisor & 0xFF;

            if(balongv7r2_uart_is_busy(ptr_chan))
            {
                return EIO;
            }

            /* disable interrupts during chip access */
            oldlevel = intLock ();
            ulLCR = readl((u32)(ptr_chan)->regs + UART_REGOFF_LCR);
            /*enable DLL&DLH*/
            writel(UART_LCR_DLAB, (u32)(ptr_chan)->regs + UART_REGOFF_LCR);

            /* ����DLH ��DLL �Ĵ��������ô���ͨѶ�Ĳ�����*/
            writel(ulDivisorLow, (u32)(ptr_chan)->regs + UART_REGOFF_DLL);
            writel(ulDivisorHigh, (u32)(ptr_chan)->regs + UART_REGOFF_DLH);
            /*end set baudrate */

            /* USR bit[0]=0 ��UART ����idle ̬ʱ�����ܶ�LCR bit[7]��λ����д����*/
            /* bit[7] 0����ֹDLL ��DLH��1��ʹ��DLL ��DLH*/
            /*��DLL��DLH������ɺ��λ�������㣬������DLL��DLH���õ�ַ�ļĴ������޷�����*/
            writel((0x7f & ulLCR), (u32)(ptr_chan)->regs + UART_REGOFF_LCR);
            /*========================= Modify end=================================*/

            ptr_chan->baud_rate = arg;

            intUnlock (oldlevel);

            break;
        }

#ifdef	INCLUDE_TTY_DEV
        case SIO_BAUD_GET:
        {
            /* Get the baud rate and return OK */
            *(int *)arg = ptr_chan->baud_rate;
            break;
        }
        case SIO_MODE_SET:
        {
            if ((arg != SIO_MODE_POLL) && (arg != SIO_MODE_INT))
            {
                status = EIO;
                break;
            }
            oldlevel = intLock();

            balongv7r2_uart_mode_set(ptr_chan, arg);

            ptr_chan->channel_mode = (u32)arg;

            intUnlock (oldlevel);
            break;
        }
        case SIO_MODE_GET:
        {
            /* Get the current mode and return OK */
            *(int *)arg = ptr_chan->channel_mode;/*lint !e713*/
            break;
        }
        case SIO_AVAIL_MODES_GET:
        {
            /* Get the available modes and return OK */
            *(int *)arg = SIO_MODE_INT | SIO_MODE_POLL;
            break;
        }
        case SIO_HW_OPTS_SET:
        {
             status = (balongv7r2_uart_opts_set(ptr_sio_chan, arg) == OK) ? OK : EIO;
			 break;
        }
        case SIO_HW_OPTS_GET:
        {
        	*(int *)arg = ptr_chan->options;
        	break;
        }
#endif	/* INCLUDE_TTY_DEV */

        default:
        {
            status = ENOSYS;
        }
    }

    return status;

}


/*****************************************************************************
* �� �� ��     :  balongv7r2_uart_tx_startup
*
* ��������  :  ���ô��ڵĲ���
*
* �������  :  SIO_CHAN *	ptr_sio_chan:ptr to SIO_CHAN describing this channel
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static int balongv7r2_uart_tx_startup(SIO_CHAN * ptr_sio_chan)
{
    AMBA_UART_CHAN * ptr_chan = (AMBA_UART_CHAN *)ptr_sio_chan;
    u32 value_ier;

    if (SIO_MODE_INT == ptr_chan->channel_mode)
    {
        /* ������ӡ */
        value_ier = readl((u32)(ptr_chan)->regs + UART_REGOFF_IER);
        value_ier |= UART_IER_TX_IRQ_ENABLE;
        writel(value_ier, (u32)(ptr_chan)->regs + UART_REGOFF_IER);
        return OK;
    }
    else
    {
        return ENOSYS;
    }
}



/*****************************************************************************
* �� �� ��     :  balongv7r2_uart_cb_install
*
* ��������  :  ���ô��ڻص�
*
* �������  :  SIO_CHAN *	ptr_sio_chan:ptr to SIO_CHAN describing this channel
*                           int		    callback_type :type of callback
*                           s32	    (*callback)()   :callback
*                           void *	    callbackArg     :parameter to callback
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static int balongv7r2_uart_cb_install(SIO_CHAN * ptr_sio_chan,int callback_type,s32 (*callback)(),void * callback_arg)
{
    AMBA_UART_CHAN * ptr_chan = (AMBA_UART_CHAN *)ptr_sio_chan;

    switch (callback_type)
    {
        /* ���ͻص�ע��*/
        case SIO_CALLBACK_GET_TX_CHAR:
        {
            ptr_chan->get_tx_char    = callback;
            ptr_chan->get_tx_arg     = callback_arg;
            return OK;
        }

        /* ���ջص�ע��*/
        case SIO_CALLBACK_PUT_RCV_CHAR:
        {
            ptr_chan->put_rcv_char   = callback;
            ptr_chan->put_rcv_arg    = callback_arg;
            return OK;
        }

        default:
        {
            return ENOSYS;
        }
    }

}


/*****************************************************************************
* �� �� ��     :  balongv7r2_uart_poll_input
*
* ��������  :  ��ѯ����
*
* �������  :  SIO_CHAN *	ptr_sio_chan:ptr to SIO_CHAN describing this channel
*                           char *	    this_char       : pointer to where to return character
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static int balongv7r2_uart_poll_input(SIO_CHAN * ptr_sio_chan, char * this_char)
{
    AMBA_UART_CHAN * ptr_chan = (AMBA_UART_CHAN *)ptr_sio_chan;
    FAST u32     poll_status;

    poll_status = readl((u32)(ptr_chan)->regs + UART_REGOFF_LSR);

    if ((poll_status & UART_LSR_DR) != UART_LSR_DR)
    {
        return EAGAIN;
    }

    /* got a character */
    *this_char = readl((u32)(ptr_chan)->regs + UART_REGOFF_RBR);

    return OK;
}



/*****************************************************************************
* �� �� ��     :  balongv7r2_uart_poll_output
*
* ��������  : ��ѯ���
*
* �������  :  SIO_CHAN *	ptr_sio_chan:ptr to SIO_CHAN describing this channel
*                           char	    out_char              :char to output
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static int balongv7r2_uart_poll_output(SIO_CHAN * ptr_sio_chan,char out_char)
{
    AMBA_UART_CHAN * ptr_chan = (AMBA_UART_CHAN *)ptr_sio_chan;
    FAST u32 pollStatus;

    pollStatus = readl((u32)(ptr_chan)->regs + UART_REGOFF_LSR);

    /* is the transmitter ready to accept a character? */
    if ((pollStatus & UART_LSR_TEMT) != UART_LSR_TEMT)
    {
        return EAGAIN;
    }

    /* write out the character */
    writel((unsigned)out_char, (u32)(ptr_chan)->regs + UART_REGOFF_RBR);

    return OK;
}

static SIO_DRV_FUNCS uart_sio_drv_funcs =
{
    (int (*)())                 balongv7r2_uart_ioctl,
#ifdef	INCLUDE_TTY_DEV
    (int (*)())                 balongv7r2_uart_tx_startup,
    (int (*)())                 balongv7r2_uart_cb_install,
#else	/* INCLUDE_TTY_DEV */
    (int (*)())                 NULL,
    (int (*)())                 NULL,
#endif	/* INCLUDE_TTY_DEV */
    (int (*)())                 balongv7r2_uart_poll_input,
    (int (*)(SIO_CHAN *,char))  balongv7r2_uart_poll_output
};

static s32 balongv7r2_uart_dummy_cb (void)
{
    return -1;
}

print_hook g_print_hook = NULL;

/*****************************************************************************
* �� �� ��     :  balongv7r2_uart_init_channel
*
* ��������  :  ���ô��ڵĲ���
*
* �������  :  AMBA_UART_CHAN *	ptr_chan:ptr to AMBA_UART_CHAN describing this channel
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static void balongv7r2_uart_init_channel(AMBA_UART_CHAN *	ptr_chan)
{
    u32  ul_divisor;
    u32  ul_divisor_high;
    u32  ul_divisor_low;

    /* FCR�Ĵ�����ʼ��*/
    writel(UART_FCR_DEF_SET, (u32)(ptr_chan)->regs + UART_REGOFF_FCR);

    /*ȥʹ�������ж�*/
    writel(UART_IER_IRQ_DISABLE, (u32)(ptr_chan)->regs + UART_REGOFF_IER);

    /*clear line status interrupt*/
    readl((u32)(ptr_chan)->regs + UART_REGOFF_LSR);

    /*clear rcv interrupt*/
    readl((u32)(ptr_chan)->regs + UART_REGOFF_RBR);

    /*clear send empty interrupt*/
    readl((u32)(ptr_chan)->regs + UART_REGOFF_IIR);

    /*clear line busy interrupt*/
    readl((u32)(ptr_chan)->regs + UART_REGOFF_USR);

    /*begin set baudrate */
    ul_divisor = ptr_chan->xtal / (u32)(16 * ptr_chan->baud_rate);/* [false alarm]:�� */
    ul_divisor_high = (ul_divisor & 0xFF00) >> 8;
    ul_divisor_low = ul_divisor & 0xFF;

    /* Enable DLL/DHL */
    writel(UART_LCR_DLAB, (u32)(ptr_chan)->regs + UART_REGOFF_LCR);


    /* ����DLH ��DLL �Ĵ��������ô���ͨѶ�Ĳ�����*/
    writel(ul_divisor_low, (u32)(ptr_chan)->regs + UART_REGOFF_DLL);
    writel(ul_divisor_high, (u32)(ptr_chan)->regs + UART_REGOFF_DLH);
    /*end set baudrate */

    /* ������Χ���ͺͽ������ݵ���ĿΪ8bit,1ֹͣλ,��У��λ,disable DLL&DLH */
    writel((UART_LCR_PEN_NONE | UART_LCR_STOP_1BITS | UART_LCR_DLS_8BITS), (u32)(ptr_chan)->regs + UART_REGOFF_LCR);
    /* ʹ�ܽ����ж�*/
    /*writel(UART_IER_RX_IRQ_ENABLE, (u32)(ptr_chan)->regs + UART_REGOFF_IER);*/
	writel(UART_IER_RX_IRQ_ENABLE, (u32)(ptr_chan)->regs + UART_REGOFF_IER);

}/*lint !e550*/


/*****************************************************************************
* �� �� ��     :  balongv7r2_uart_init
*
* ��������  :  ��ʼ������
*
* �������  :  AMBA_UART_CHAN *	ptr_chan:ptr to AMBA_UART_CHAN describing this channel
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
void balongv7r2_uart_init(AMBA_UART_CHAN * ptr_chan)
{
    int old_level;

    old_level = intLock();
    /* initialise the driver function pointers in the SIO_CHAN */
    ptr_chan->sio.pDrvFuncs = &uart_sio_drv_funcs;

    /* set the non BSP-specific constants */
    ptr_chan->get_tx_char  = balongv7r2_uart_dummy_cb;
    ptr_chan->put_rcv_char = balongv7r2_uart_dummy_cb;

    ptr_chan->channel_mode = 0;

    /* initialise the chip channel */
    balongv7r2_uart_init_channel (ptr_chan);

    intUnlock (old_level);
}

/*****************************************************************************
* �� �� ��     :  balongv7r2_uart_tx_int_proc
*
* ��������  :  ���ڷ����жϴ�����
*
* �������  :  AMBA_UART_CHAN *	ptr_chan:ptr to AMBA_UART_CHAN describing this channel
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static void balongv7r2_uart_tx_int_proc (AMBA_UART_CHAN *	ptr_chan)
{
    char    out_char;
    int     i;
    u32  reg_value;
    u32  value_ier;

    /* FIFO�����16��byte */
    for(i = 0; i < UART_FIFO_SIZE; i++)
    {
        reg_value = readl((u32)(ptr_chan)->regs + UART_REGOFF_USR);

		if (0 != (reg_value & UART_USR_FIFO_NOT_FULL))
		{
			if ((*ptr_chan->get_tx_char)(ptr_chan->get_tx_arg, &out_char) != ERROR)
			{
				/* ���������е����ݷŵ����ݼĴ�����*/
                writel((unsigned)out_char, (u32)(ptr_chan)->regs + UART_REGOFF_RBR);
                (void)dmesg_write(&out_char, sizeof(char));
			}
            else if((g_print_hook) && (g_print_hook(&out_char) != ERROR))
            {
				/* ���������е����ݷŵ����ݼĴ�����*/
                writel((unsigned)out_char, (u32)(ptr_chan)->regs + UART_REGOFF_RBR);
            }
			else
			{
				/* ���ж� */
                value_ier = readl((u32)(ptr_chan)->regs + UART_REGOFF_IER);
                value_ier &= ~(UART_IER_TX_IRQ_ENABLE);
                writel(value_ier, (u32)(ptr_chan)->regs + UART_REGOFF_IER);
                readl((u32)(ptr_chan)->regs + UART_REGOFF_IIR);
				return;
			}
		}
		else
		{
			/*close tx interrupts*/
			value_ier = readl((u32)(ptr_chan)->regs + UART_REGOFF_IER);
			value_ier &= ~(UART_IER_TX_IRQ_ENABLE);
			writel(value_ier, (u32)(ptr_chan)->regs + UART_REGOFF_IER);
			/* ���ж� */
            readl((u32)(ptr_chan)->regs + UART_REGOFF_IIR);
			return;
		}
    }
}



/*****************************************************************************
* �� �� ��     :  balongv7r2_uart_irq_handler
*
* ��������  :  �����ܵ��ж���Ӧ����
*
* �������  :  AMBA_UART_CHAN * ptr_chan:ptr to AMBA_UART_CHAN describing this channel
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
void balongv7r2_uart_irq_handler(AMBA_UART_CHAN * ptr_chan)
{
    char int_id;

    int_id = readl((u32)(ptr_chan)->regs + UART_REGOFF_IIR);

    int_id &= UART_FIFO_MASK;

    /* �����жϴ��� */
    if (UART_IIR_THR_EMPTY & int_id)
    {
        balongv7r2_uart_tx_int_proc(ptr_chan);
    }
    /* �����жϴ��� */
    if (int_id & (UART_IIR_REV_VALID | UART_IIR_REV_TIMEOUT))
    {
        balongv7r2_uart_rx_int_proc(ptr_chan);
    }

    return;
}

void balongv7r2_uart_register_hook(print_hook hook)
{
    g_print_hook = hook;
}


#ifdef CONFIG_CCORE_PM
extern AMBA_UART_CHAN * get_amba_chan();


int balongv7r2_uart_suspend(void)
{
	(void)intDisable((int)(get_amba_chan())->level_rx);
	return 0;
}

/*****************************************************************************
* �� �� ��  :  balongv7r2_uart_resume
*
* ��������  :  ����1�ָ�
*
* �������  :  ��

* �������  :  ��
*
* �� �� ֵ  :  0 : �ɹ�[״̬��־λȡ��
*              1 : ʧ��[״̬��־λȡ��
*
* �޸ļ�¼  :
*****************************************************************************/
int balongv7r2_uart_resume(void)
{
	balongv7r2_uart_init_channel(get_amba_chan());
	(void)intEnable ((int)(get_amba_chan())->level_rx);
    balongv7r2_uart_tx_startup(&((get_amba_chan())->sio));
	return 0;
}
#endif

