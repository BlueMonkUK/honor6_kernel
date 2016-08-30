/*
 * cshell.c -- balong cshell driver
 *
 * Copyright (C) 2012 Huawei Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

/*******************************************************************/
/*                                              ͷ�ļ�                                              */
/*******************************************************************/
#include <vxWorks.h>
#include <stdio.h>
#include <taskLib.h>
#include <selectLib.h>
#include <ttyLib.h>
#include <shellLib.h>
#include <cacheLib.h>
#include "arm_pbxa9.h"
#include "private/shellLibP.h"
#include "soc_interrupts.h"    /* Added for intrrupt number for uart0 */
#include "osl_thread.h"
#include "bsp_om.h"
#include "bsp_icc.h"
#include "hi_uart.h"
#include <bsp_sram.h>
#include <bsp_shared_ddr.h>
#include <drv_nv_def.h>
#include <drv_nv_id.h>
#include <bsp_nvim.h>
#include "cshell.h"
/*lint --e{26, 30, 52, 63, 64, 101, 132, 160, 526, 527, 628, 550, 746 ,958} */
cprint_hook g_cprint_hook = NULL;
static SHELL_IO_CHAN shell_io_channel = {0};
struct cshell_debug g_cshell_dbg = {0};
static SIO_DRV_FUNCS shell_io_sio_drv_funcs;
#define WATER_LINE 128


int cshell_direction(cshell_mode_tpye mode);
/*
*-e30:Ҫ��case�ĺ�Ϊconst���ͣ����˴����õ��ú�Ϊvxworks�Դ���lint��
*-e63���ڴ���ֵ������ʵ�����Ѿ�����ֵ��ֵ����
*/

/*******************************************************************/
/*                                              ��������                                         */
/*******************************************************************/
//extern VOID usrShell(VOID);

/*lint --e{18,40,578,713} */

static int cshell_command_cb(u32 chan_id, u32 len, void* context)
{
	SHELL_IO_CHAN *ptr_shell = &shell_io_channel;
	s32 read_size = 0;
	u8 flag = 0;

	read_size = bsp_icc_read((ICC_CHN_IFC << 16)|IFC_RECV_FUNC_CSHELL, &flag, len);
	if ((read_size > (s32)len) && (read_size < 0))
    {
        return 1;
    }

	ptr_shell->cshell_send_permission = (s32)flag;

	return 0;
}

/*****************************************************************************
* �� �� ��     :  cshell_read_cb
*
* ��������  : ���ص�����
*
* �������  :   u32 channel_id :channel id
*                            int len   :read size
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
/* �˴��Ƿ���Ҫ������������������������жϴ������� */
static int cshell_read_cb(u32 channel_id , u32 len, void* context)
{
    int i = 0;
    int read_size = 0;
    SHELL_IO_CHAN *ptr_shell = &shell_io_channel;

    if (!g_cshell_dbg.state)
    {
		cshell_print_error("C:cshell_read_cb fail for icc channel is no opened\n");
        return 1;
    }

    if (!len)
    {
		cshell_print_error("C:cshell_read_cb fail for zero length data\n");
        return 1;
    }

    read_size = (int)bsp_icc_read((ptr_shell->icc_channel_id), ptr_shell->ptr_recv_buf, len);
    if ((read_size > (int)len) || (read_size < 0))
    {
		cshell_print_error("C:cshell_read_cb fail [0x%x]\n", read_size);
        return 1;
    }
	g_cshell_dbg.recv_data_cnt++;

    for (i = 0; i < read_size; i++)
    {
        (void)(ptr_shell->put_rx_char)(ptr_shell->put_rx_arg, *(ptr_shell->ptr_recv_buf + i));
    }
	
    return 0;
}


/*****************************************************************************
* �� �� ��     :  cshell_direction
*
* ��������  : �ı�cshell�ķ���
*
* �������  :  cshell_mode_tpye mode :
*                           CSHELL_MODE_UART : �л���UART
*                           CSHELL_MODE_USB   : �л���USB
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
int cshell_direction(cshell_mode_tpye mode)
{
    int key = 0;
    int fd = 0;
    SHELL_ID ShellID;
    char ttyFg[16] = {0x00};
    char ttyBg[16] = {0x00};
    SHELL_IO_CHAN *ptr_shell = &shell_io_channel;
	SRAM_SMALL_SECTIONS * sram = (SRAM_SMALL_SECTIONS * )SRAM_SMALL_SECTIONS_ADDR;

    if ((cshell_mode_tpye)(ptr_shell->shell_mode) == mode)
    {
		cshell_print_error("C:no need to redirect cshell!\n");
        return OK;
    }

    if (CSHELL_MODE_UART == mode)
    {
        /* coverity[secure_coding] */
        sprintf(ttyFg, "/tyCo/0");
        /* coverity[secure_coding] */
        sprintf(ttyBg, "/tyCo/3");
    }
    else if (CSHELL_MODE_USB == mode)
    {
        /* coverity[secure_coding] */
        sprintf(ttyFg, "/tyCo/3");
        /* coverity[secure_coding] */
        sprintf(ttyBg, "/tyCo/0");
    }
    else
    {
		cshell_print_error("C:redirect parm error[%d]!\n", mode);
        return ERROR;
    }
	if (CSHELL_MODE_UART == mode)
		intEnable((int)sram->UART_INFORMATION[1].interrupt_num);
    else
    {
        /* ��ֹ�����ж�,M���Ĳ���UART */
   	    intDisable((int)sram->UART_INFORMATION[1].interrupt_num);
    }

    key = intLock();
    //taskLock();

    /* �ر�Shell��ǰʹ�õĴ��� */
    fd = ioGlobalStdGet(STD_IN);
    (void)ioctl (fd, FIOSETOPTIONS, OPT_RAW);
    close(fd);

    ShellID = shellFromTaskGet(shellConsoleTaskId);
    shellTerminate(ShellID);

    if (0 != (fd = open(ttyFg, O_RDWR, 0)))/* [false alarm]:�� */
    {
        /* �л��ɹ� */
        ptr_shell->shell_mode = mode;
    }
    else
    {
        /* �л�ʧ�� */
        fd = open(ttyBg, O_RDWR, 0);
        if (0 == fd)
        {
            //taskUnlock();
            intUnlock(key);
            return ERROR;
        }
    }

    /* coverity[noescape] */
    (void)ioctl (fd, FIOSETOPTIONS, OPT_TERMINAL);

    /* coverity[noescape] */
    ioGlobalStdSet (STD_OUT, fd);
    /* coverity[noescape] */
    ioGlobalStdSet (STD_IN,  fd);
    /* coverity[noescape] */
    ioGlobalStdSet (STD_ERR, fd);

    //taskUnlock();
    intUnlock(key);

    usrShell();

    return OK;
}



/*****************************************************************************
* �� �� ��     :  cshell_io_uninit
*
* ��������  : cshellȥ��ʼ��
*
* �������  :  SHELL_IO_CHAN *ptr_shell :shellָ��
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static int cshell_io_uninit(SHELL_IO_CHAN *ptr_shell)
{
    if (ptr_shell)
    {

        if (ptr_shell->shell_send_tid)
        {
            kthread_stop(ptr_shell->shell_send_tid);
            ptr_shell->shell_send_tid = 0;
        }
        if (ptr_shell->cshell_recv_sem)
        {
            //semDelete(ptr_shell->cshell_recv_sem);
			osl_sema_delete(&(ptr_shell->cshell_recv_sem));
            ptr_shell->cshell_recv_sem = NULL;
        }

        ptr_shell->ptr_drv_funcs = NULL;
    }
	return OK;
}

/*****************************************************************************
* �� �� ��     :  cshell_io_ioctl
*
* ��������  : cshell���������·�
*
* �������  :  SHELL_IO_CHAN *ptr_shell :shellָ��
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static int cshell_io_ioctl(SHELL_IO_CHAN *ptr_chan, int request, int arg )
{
    int status = OK;

    switch (request)
    {
        case SIO_BAUD_SET: /* set baudrate */
        {
            break;
        }
        case SIO_BAUD_GET: /* get current baudrate */
        {
            *(int *)arg = ptr_chan->baud_rate;
            status = OK;
            break;
        }
        case SIO_MODE_SET: /* set mode ( POLL or INT ) */
        {
            ptr_chan->channel_mode=arg;
            status = OK;
            break;
        }
        case SIO_MODE_GET: /* get current mode */
        {
            *(int *)arg = ptr_chan->channel_mode;
            return (OK);
        }
        case SIO_AVAIL_MODES_GET: /* get the available mode */
        {
            *(int *)arg = SIO_MODE_INT;
            return (OK);
        }
        case SIO_HW_OPTS_SET: /* set options */
        {
            ptr_chan->options=arg;
            status = OK;
            break;
        }
        case SIO_HW_OPTS_GET: /* get current options */
        {
            *(int *)arg = (int)ptr_chan->options;
            break;
        }
        case SIO_HUP: /* hang up the modem control lines */
        case SIO_IRDA_MODE:
        case UART_DMA_MODE: /* set UART to DMA mode */
        {
            /* check if hupcl option is enabled */
            status=OK;
            break;
        }
        case SIO_OPEN: /* set the modem control lines 100a*/
        {
            if (ptr_chan->options & HUPCL)
            status = OK;
            break;
        }
        case FIOSELECT:
        {
            selNodeAdd (&ptr_chan->sel_wakeup_list, (SEL_WAKEUP_NODE *) arg);
            break;
        }
        case FIOUNSELECT:
        {
            selNodeDelete (&ptr_chan->sel_wakeup_list, (SEL_WAKEUP_NODE *) arg);
            break;
        }
        default:
        {
            status = ENOSYS;
            break;
        }
    }

    return (status);
}

/*****************************************************************************
* �� �� ��     :  cshell_io_startup
*
* ��������  :  cshell��ʼ���Ϳ�ʼ
*
* �������  :  SHELL_IO_CHAN *ptr_shell :shellָ��
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
/*�жϷ�ʽ�½��з�������*/
static int cshell_io_startup(SHELL_IO_CHAN *ptr_chan)
{
    //semGive(ptr_chan->cshell_recv_sem);
	osl_sem_up(&(ptr_chan->cshell_recv_sem));
    return (OK);
}

/*****************************************************************************
* �� �� ��     :  cshell_io_cb_install
*
* ��������  :  �ص�ע��
*
* �������  :  SHELL_IO_CHAN *ptr_shell :shellָ��
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static int cshell_io_cb_install(SIO_CHAN * ptr_sio_chan, int callback_type, int (*callback)(), void * callback_arg)
{
    SHELL_IO_CHAN * ptr_chan = (SHELL_IO_CHAN *)(void *)ptr_sio_chan;

	cshell_print_debug("C:cshell_io_cb_install register cb [0x%x].\n", callback_type);

    switch (callback_type)
    {
        case SIO_CALLBACK_GET_TX_CHAR:
            ptr_chan->get_tx_char = callback;
            ptr_chan->get_tx_arg  = callback_arg;
            return (OK);

        case SIO_CALLBACK_PUT_RCV_CHAR:
            ptr_chan->put_rx_char = callback;
            ptr_chan->put_rx_arg  = callback_arg;
            return (OK);

        default:
            return (ENOSYS);
    }
}
/*****************************************************************************
* �� �� ��     :  cshell_io_rx_char
*
* ��������  :  ��ѯ���մ��������˴������жϷ�ʽ���ú���ֱ�ӷ���
*
* �������  :  SHELL_IO_CHAN *ptr_shell :shellָ��
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static int cshell_io_rx_char(SHELL_IO_CHAN *ptr_chan, char *pChar)
{
    return (OK);
}

/*****************************************************************************
* �� �� ��     :  cshell_io_tx_char
*
* ��������  :  ��ѯ���ʹ��������˴������жϷ�ʽ���ú���ֱ�ӷ���
*
* �������  :  SHELL_IO_CHAN *ptr_shell :shellָ��
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static int cshell_io_tx_char( SHELL_IO_CHAN * ptr_chan, char outChar)
{
    return (OK);
}

/*****************************************************************************
* �� �� ��     :  cshell_io_send
*
* ��������  :  �������ݵ�A��
*
* �������  :  SHELL_IO_CHAN *ptr_shell :shellָ��
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
void cshell_io_send(void)
{
	SHELL_IO_CHAN *ptr_shell = &shell_io_channel;
	unsigned int data_size = 0;
	unsigned int len_send2icc = 0;
	int wr_len = 0;
	int ret = 0;

	cshell_print_debug("enter handler\n");
	/* coverity[no_escape] */
	for(;;)
	{
		//semTake(ptr_shell->cshell_recv_sem, WAIT_FOREVER);
		osl_sem_down(&(ptr_shell->cshell_recv_sem));

		/* ������, Ҫ���� */
		while (OK == (ret = (ptr_shell->get_tx_char)(ptr_shell->get_tx_arg, (ptr_shell->ptr_send_buf + ptr_shell->send_rd))))
		{
			(void)dmesg_write((const char *)(ptr_shell->ptr_send_buf + ptr_shell->send_rd), sizeof(char));
			/* buffer��, �׵������� */
			if(((ptr_shell->send_rd + 1) & (IO_BUFFER_MAX*2 - 1)) == ptr_shell->send_wt)
			{
				ptr_shell->send_wt = ptr_shell->send_wt + 1;
				ptr_shell->send_wt &= (IO_BUFFER_MAX*2 - 1);
				++g_cshell_dbg.lost_data_cnt;
			}
			++ptr_shell->send_rd;
			ptr_shell->send_rd &= (IO_BUFFER_MAX*2 - 1);
		}

		/* ����ˮλ��, ����icc���ͽӿڽ����ݷ���acore ACM */
		data_size = (ptr_shell->send_rd + IO_BUFFER_MAX*2 - ptr_shell->send_wt) & (IO_BUFFER_MAX*2 - 1);

		while(data_size && ptr_shell->cshell_send_permission)
		{
			len_send2icc = min(data_size, CSHELL_ICC_FIFO_DEPTH);
			if (ptr_shell->send_rd < ptr_shell->send_wt)
			{
				len_send2icc = min(len_send2icc, (unsigned int)(IO_BUFFER_MAX*2 - ptr_shell->send_wt));
			}
			wr_len = bsp_icc_send(ICC_CPU_APP, ptr_shell->icc_channel_id, 
					ptr_shell->ptr_send_buf + ptr_shell->send_wt, len_send2icc);

			if(wr_len < 0) /* ����ʧ�� */
			{
				++g_cshell_dbg.send_fail_cnt;
				cshell_print_error("icc send error\n");
				break;
			}
			else if(wr_len < (s32)len_send2icc) /* iccͨ����˳��,��ʱ�˳����� */
			{
				++g_cshell_dbg.send_busy_cnt;
				ptr_shell->send_wt = (ptr_shell->send_wt + wr_len) & (IO_BUFFER_MAX*2 - 1);
				data_size -= (unsigned int)wr_len;/* [false alarm]:�� */
				break;
			}
			else if(wr_len == (s32)len_send2icc) /* ˳�����������ݷ����� */
			{
				++g_cshell_dbg.send_succ_cnt;
				ptr_shell->send_wt = (ptr_shell->send_wt + wr_len) & (IO_BUFFER_MAX*2 - 1);
				data_size -= (unsigned int)wr_len;
			}
		}
	}
}

void cshell_send_data(char* data, int length)
{
    SHELL_IO_CHAN *ptr_shell = &shell_io_channel;
    int size = 1024;
    int send_size;
	int sended_len = 0;

    while(data && length)
    {
        send_size  = length < size ? length : size;
		sended_len = bsp_icc_send(ICC_CPU_APP, ptr_shell->icc_channel_id, (u8*)data, (u32)send_size);
    	if (sended_len < 0)
        {
			break;
        }

        data += sended_len;
        length -= sended_len;
        taskDelay(5);
    }
}

void cshell_register_hook(cprint_hook hook)
{
    g_cprint_hook = hook;
}


/*****************************************************************************
* �� �� ��     :  cshell_sio_init
*
* ��������  :  cshell tty�豸��ʼ������ʼ��������
*
* �������  :  void
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
static int cshell_sio_init(SHELL_IO_CHAN *ptr_shell)
{
    if (!ptr_shell)
    {
        cshell_print_error("NULL PTR!\n");
        return CSHELL_ERROR;
    }

    //ptr_shell->cshell_recv_sem = semBCreate(SEM_Q_FIFO, SEM_EMPTY);
	osl_sem_init(0, &(ptr_shell->cshell_recv_sem));
	if(!ptr_shell->cshell_recv_sem)
	{
		cshell_print_error("fail to cshell_recv_sem!\n");
		goto malloc_send_buf_fail;
	}

    /* ����Shell���ݷ��ͻ����� */
    if (NULL == (ptr_shell->ptr_send_buf = (u8*) cacheDmaMalloc(IO_BUFFER_MAX*2)))
    {
        cshell_print_error("send buf malloc fail!\n");
        goto malloc_send_buf_fail;
    }

    if (NULL == (ptr_shell->ptr_recv_buf = (u8*) cacheDmaMalloc(IO_BUFFER_MAX)))
    {
		cshell_print_error("recv buf malloc fail!\n");
        goto malloc_recv_buf_fail;
    }

    /* ��ʼ��tty�豸���ҽӻص�����ָ�� */
    if (shell_io_sio_drv_funcs.ioctl == NULL)
    {
        shell_io_sio_drv_funcs.ioctl           = (int (*)())cshell_io_ioctl;
        shell_io_sio_drv_funcs.txStartup       = (int (*)())cshell_io_startup;
        shell_io_sio_drv_funcs.callbackInstall = (int (*)())cshell_io_cb_install;
        shell_io_sio_drv_funcs.pollInput       = (int (*)())cshell_io_rx_char;
        shell_io_sio_drv_funcs.pollOutput      = (int (*)(SIO_CHAN *, char))cshell_io_tx_char;
    }
    ptr_shell->ptr_drv_funcs = &shell_io_sio_drv_funcs;

    if(CSHELL_OK != osl_task_init("tCShell", CSHELL_TASK_PRI, CSHELL_TASK_STACK_SIZE, (void *)cshell_io_send, NULL, 
								(u32 *)&ptr_shell->shell_send_tid))
    {
		cshell_print_error("create task fail!\n");
        goto create_task_fail;
    }

    selWakeupListInit (&ptr_shell->sel_wakeup_list);
    ttyDevCreate ("/tyCo/3", (SIO_CHAN *)ptr_shell, TTY_BUFFER_MAX, TTY_BUFFER_MAX); /*lint !e740 */

    return OK;

create_task_fail:
	cshell_safe_free(ptr_shell->ptr_recv_buf);
malloc_recv_buf_fail:
	cshell_safe_free(ptr_shell->ptr_send_buf);
malloc_send_buf_fail:
    cshell_io_uninit(ptr_shell);

    return CSHELL_ERROR;
}

/*****************************************************************************
* �� �� ��     :  cshell_init
*
* ��������  :  cshell��ʼ��
*
* �������  :  void
* �������  :  ��
*
* �� �� ֵ     :  ��
*
* �޸ļ�¼  :
*****************************************************************************/
int cshell_init(void)
{
    SHELL_IO_CHAN *ptr_shell = &shell_io_channel;
	int ret = 0;
	DRV_UART_SHELL_FLAG uartcshell_nv = {.extendedbits = 0};


	ptr_shell->cshell_send_permission = 0;
    if(bsp_nvm_read(NV_ID_DRV_UART_SHELL_FLAG, (u8 *)&uartcshell_nv, sizeof(DRV_UART_SHELL_FLAG)))
    {
        printk("[A:CSHELL]:bsp_nvm_read fails  line:%d uartcshell_nv.cshell_to_auart:%d\n", __LINE__, uartcshell_nv.extendedbits);
    }
    if(uartcshell_nv.extendedbits & 0x1U)
    {
        ptr_shell->cshell_send_permission = (s32)((unsigned int)(ptr_shell->cshell_send_permission) | (0x1U << 2));
    }

    if (CSHELL_OK != cshell_sio_init(ptr_shell))
    {
        cshell_print_error("io init fail!\n");
        return CSHELL_ERROR;
    }

    /* ע��ICC��д�ص� */
    ptr_shell->icc_channel_id = (ICC_CHN_CSHELL << 16 | 0);
    if(CSHELL_OK !=bsp_icc_event_register(ptr_shell->icc_channel_id, (read_cb_func)cshell_read_cb , NULL, NULL, NULL))
    {
        cshell_print_error("register icc callback fail!\n");
        return CSHELL_ERROR;
    }

	/* �ı�CSHELL ��USB  ���⴮��*/
	if(readl(SHM_MEM_CHSELL_FLAG_ADDR) == PRT_FLAG_EN_MAGIC_M)
		ret = cshell_direction(CSHELL_MODE_USB);
	else
		ret = cshell_direction(CSHELL_MODE_UART);

    if (OK != ret)
    {
        cshell_print_error("direct cshell to usb fail\n");
        return CSHELL_ERROR;
    }


    /* ��ֹ�����ж�,M���Ĳ���UART */
   	//intDisable(sram->UART_INFORMATION[1].interrupt_num);

	memset((void *)&g_cshell_dbg, 0, sizeof(g_cshell_dbg));
	if(CSHELL_OK != bsp_icc_event_register((ICC_CHN_IFC << 16)|IFC_RECV_FUNC_CSHELL, cshell_command_cb, NULL, NULL, NULL))
    {
        cshell_print_error("register icc callback fail!\n");
        return CSHELL_ERROR;
    }
	g_cshell_dbg.state = 1;

   	printf("ok\n");

    return CSHELL_OK;
}

void cshell_dbg_print_sw(int sw)
{
	g_cshell_dbg.print_sw = sw;
}

void cshell_debug_show(void)
{
	printf("*****************************************************\n");
	printf("g_cshell_dbg.state        : 0x%08x\n", g_cshell_dbg.state);
	printf("g_cshell_dbg.print_sw     : 0x%08x\n", g_cshell_dbg.print_sw);
	printf("g_cshell_dbg.lost_data_cnt: 0x%08x\n", g_cshell_dbg.lost_data_cnt);
	printf("g_cshell_dbg.recv_data_cnt: 0x%08x\n", g_cshell_dbg.recv_data_cnt);
	printf("g_cshell_dbg.send_succ_cnt: 0x%08x\n", g_cshell_dbg.send_succ_cnt);
	printf("g_cshell_dbg.send_busy_cnt: 0x%08x\n", g_cshell_dbg.send_busy_cnt);
	printf("g_cshell_dbg.send_fail_cnt: 0x%08x\n", g_cshell_dbg.send_fail_cnt);
}

#ifdef __cplusplus
}
#endif

