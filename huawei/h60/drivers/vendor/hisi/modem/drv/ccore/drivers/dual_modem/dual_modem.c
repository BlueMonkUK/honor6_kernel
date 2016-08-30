
#include "bsp_dump.h"
#include "bsp_hardtimer.h"
#include "dual_modem.h"
#include "gpio_balong.h"

#ifdef CONFIG_CCORE_PM
#include "bsp_dpm.h"
#endif

#ifdef CONFIG_CCORE_PM
struct dpm_device dual_modem_device =
{
	.device_name = "balong dual modem driver",
	.suspend = dual_uart_suspend,
	.resume = dual_uart_resume,
	.prepare = NULL,
	.suspend_late = NULL,
	.complete = NULL,
	.resume_early = NULL,
};
#endif

UART_HW_OPS muart2_ops =
{
    .send = dual_modem_send_bytes,
};

struct dual_modem_control g_dual_modem_ctrl={{0},{0},{0},0};
struct dual_modem_info g_dual_modem_init_info=
{
	{
	    HI_UART5_REGBASE_ADDR, 	//���ڼĴ�������ַ
	   	LPM3_UART5_IPC_INTID,	//���ڶ�Ӧ���жϺ�
		PBXA9_UART_CLK,			//����ʱ��Ƶ��60Mhz
	    UART_DEFAULT_BAUDRATE	//������Ϊ57600
	},
	{
	    {CBP_UART_PORT_ID,  MUART1_ID},
	    {CBP_UART_PORT_ID, MUART1_ID},
	    {UART_CONSUMER_BUTT,UART_PHY_BUTT}
	}
	
};
struct dm_dump_info	*g_dm_dump_info = NULL;
/*****************************************************************************
 �� �� ��  : recv_lpm3_msg_icc_cb
 ��������  : ��lpm3��ת���Ļ�����Ϣ or ����v3 modemʱ�õ���ȷ����Ϣ 
 �������  : UART_HW_DESC* uart_hw_desc
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
static s32 recv_lpm3_msg_icc_cb(u32 chan_id, u32 len, void* context)
{
	u8 flag = 0;
	s32 read_size = 0;
	
 	read_size = bsp_icc_read((ICC_CHN_MCORE_CCORE << 16)|MCORE_CCORE_FUNC_UART, &flag, len); 
	if ((read_size > (s32)len) && (read_size <= 0))
	{
		return ERROR;
	}

	if(g_dual_modem_ctrl.dual_modem_init_flag != UART_SWITCH_ENABLE)
	{
		dm_print_err(" recv_lpm3_msg_icc_cb g_dual_modem_ctrl.dual_modem_init_flag != UART_SWITCH_ENABLE return OK\n");
		return OK;
	}
	
	/* via modem����balong mode	*/
	if(flag == VIA_WAKEUP_BALONG)
	{
		/*����300ms��ʱ����Ͷ����˯��Ʊ*/
		wake_lock(&g_dual_modem_ctrl.wakelock);	
		dual_modem_restart_timer(&g_dual_modem_ctrl.sleep_timer);

		g_dm_dump_info->via_wakeup_balong_cnt++;
		if(g_dual_modem_ctrl.dual_modem_log_flag == 1)
		{
			dm_debug_print("WakeupBalongcnt %d\n",g_dm_dump_info->via_wakeup_balong_cnt);
		}
	}
	/* lpm3 ��������uart5 ��ʼ����Ϣ */
	if(flag == LPm3_UART5_IQR_ENABLE)
	{
		dm_print_err("lpm3 revc uart init icc\n");
	}
	return OK;
}

/*****************************************************************************
 �� �� ��  : uart_port_hw_init
 ��������  : ���ò�����
 �������  : UART_HW_DESC* uart_hw_desc
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
void dual_modem_uart_channel_init(UART_HW_DESC* uart_hw_desc)
{
	int  key = 0;
	u32  ul_divisor;
	
    if(NULL == uart_hw_desc)
    {
        dm_print_err("uart_hw_desc is null\n");
        return;
    }
    key = intLock();	

	/*uart����*/
    writel(UART_DISABLE, (u32)(uart_hw_desc)->base_addr + UART_REGOFFSET_CR);	
    /*����FIFOˮ�����1/8-FIFO*/
    writel(UART_IFLS_RX_DEF_ONETWO, (u32)(uart_hw_desc)->base_addr + UART_REGOFFSET_IFLS);
    /*��������ж�*/
    writel(UART_IER_IRQ_CLR,(u32)(uart_hw_desc)->base_addr+ UART_REGOFFSET_ICR);
    /* ���ô���ͨѶ�Ĳ�����*/
    ul_divisor = uart_hw_desc->clk/(u32)(16 * uart_hw_desc->baudrate);/* [false alarm]:�� */
    writel(ul_divisor, (u32)(uart_hw_desc)->base_addr + UART_REGOFFSET_IBRD);
    /* �������ݳ���Ϊ8bit,1ֹͣλ,��У��λ,ʹ��FIFO*/
    writel((UART_LCR_PEN_NONE | UART_LCR_STOP_1BITS | UART_ARM_LCR_DLS_8BITS | UART_LCR_FIFO_ENABLE),(u32)(uart_hw_desc)->base_addr + UART_REGOFFSET_LCR_H);
	/* ʹ��rx���ա���ʱ�ж�*/	
	writel(UART_RX_IRQ_ENABLE | UART_TIMEOUT_IRQ_ENABLE, (u32)(uart_hw_desc)->base_addr+ UART_REGOFFSET_IMSC);	
	/* ʹ��uart���ܡ����ͣ�uartʹ��*/
	writel(UART_TX_ENABLE |UART_RX_ENABLE |UART_ENABLE,(u32)(uart_hw_desc)->base_addr + UART_REGOFFSET_CR);
	
    intUnlock(key);
}
/*****************************************************************************
 �� �� ��  : uart_core_recv_handler_register
 ��������  : �û��������ݻص�ע��ӿ�
 �������  : UART_CONSUMER_PORT uPortNo
             pUARTRecv pCallback
 �������  : ��
 �� �� ֵ  : int
*****************************************************************************/
int uart_core_recv_handler_register(UART_CONSUMER_ID uPortNo, pUARTRecv pCallback)
{
	UART_PORT * cur_port = &g_dual_modem_ctrl.uart_port[MUART1_ID];
	if(NULL == pCallback)
    {
        dm_print_err("uPortNo %d pCallback IS NULL\n",uPortNo);
        return ERROR;
    }

    if(CBP_UART_PORT_ID != uPortNo)
    {
        dm_print_err("uPortNo %d port IS NULL\n",uPortNo);
        return ERROR;
    }
    cur_port->recv_callback = pCallback;
	
    g_dm_dump_info->recv_register_cnt++;
    return OK;
}

/*****************************************************************************
 �� �� ��  : uart_send
 ��������  : uart send
 �������  : UART_HW_PORT* uart_port
             BSP_U8 *pbuf
             BSP_U32 size
 �������  : ��
 �� �� ֵ  : BSP_S32
*****************************************************************************/
int dual_modem_send_bytes(UART_PORT* uart_port,BSP_U8* pbuf,BSP_U32 size)
{
	u8 * pu8Buffer = NULL;
	u32  regval = 0;
	UART_HW_DESC* uart_hw_desc = NULL;
	
    pu8Buffer = pbuf;
	
    if(NULL == uart_port || NULL == pbuf || 0 == size)
    {
        dm_print_err("parm is null\n");
        return ERROR;
    }

    if(NULL == uart_port->hw_desc)
    {
        dm_print_err("hw_desc is null\n");
        return ERROR;
    }

	uart_hw_desc = uart_port->hw_desc;
    osl_sem_down(&uart_port->send_mutex);	
	
	g_dm_dump_info->send_mutex_cnt++;
	g_dm_dump_info->tx_cur_size = size;
	g_dm_dump_info->tx_total_size += size;

	g_dm_dump_info->send_time_stamp = BSP_GetSliceValue();
	if(g_dm_dump_info->tx_cur_offset + 4 > DUAL_DUMP_TX_BUFF_SIZE)
	{
		g_dm_dump_info->tx_cur_offset = 0;
	}
	*(g_dm_dump_info->tx_dump_addr + g_dm_dump_info->tx_cur_offset) = (u8)((g_dm_dump_info->send_time_stamp & 0xff000000) >> 24);
	g_dm_dump_info->tx_cur_offset ++;

	*(g_dm_dump_info->tx_dump_addr + g_dm_dump_info->tx_cur_offset) = (u8)((g_dm_dump_info->send_time_stamp & 0x00ff0000) >> 16);
	g_dm_dump_info->tx_cur_offset ++;

	*(g_dm_dump_info->tx_dump_addr + g_dm_dump_info->tx_cur_offset) = (u8)((g_dm_dump_info->send_time_stamp & 0x0000ff00) >> 8);
	g_dm_dump_info->tx_cur_offset ++;

	*(g_dm_dump_info->tx_dump_addr + g_dm_dump_info->tx_cur_offset) = (u8)(g_dm_dump_info->send_time_stamp  & 0x000000ff);
	g_dm_dump_info->tx_cur_offset ++;

	if(g_dual_modem_ctrl.dual_modem_log_flag == 1)
	{	
		dm_debug_print("TxCurSize %d,TxTotalSize %d\n",g_dm_dump_info->tx_cur_size,g_dm_dump_info->tx_total_size);
		dm_print_info(pu8Buffer ,size);
	}
	while(size)
	{
		regval = readl(uart_hw_desc->base_addr+ UART_REGOFFSET_FR);
		if (0 == (regval & UART_FR_TX_FIFO_FULL))
    	{
			writel(*pu8Buffer, uart_hw_desc->base_addr + UART_REGOFF_THR);
			*(g_dm_dump_info->tx_dump_addr + g_dm_dump_info->tx_cur_offset) = *pu8Buffer;
			g_dm_dump_info->tx_cur_offset++;
			if(g_dm_dump_info->tx_cur_offset > DUAL_DUMP_TX_BUFF_SIZE)
			{
				g_dm_dump_info->tx_cur_offset = 0;
			}
			pu8Buffer++;
			size--;
      	}
	}
	
	osl_sem_up(&uart_port->send_mutex);
	return OK;
}

/*****************************************************************************
 �� �� ��  : uart_core_send
 ��������  : ��������
 �������  : UART_CONSUMER_PORT uPortNo
             unsigned char * pDataBuffer
             unsigned int uslength
 �������  : ��
 �� �� ֵ  : int
*****************************************************************************/
int uart_core_send(UART_CONSUMER_ID uPortNo, unsigned char * pDataBuffer, unsigned int uslength)
{
    int ret = ERROR;
	UART_PORT* cur_port = &g_dual_modem_ctrl.uart_port[uPortNo];
	

	if(UART_SWITCH_ENABLE != g_dual_modem_ctrl.dual_modem_init_flag)
	{
		dm_print_err("dual modem not init\n");
		return ERROR;
	}
	
	g_dm_dump_info->cbpa_send_cnt++;
	if(g_dual_modem_ctrl.dual_modem_log_flag == 1)
	{
    	dm_debug_print("CbpaSendCnt %d\n",g_dm_dump_info->cbpa_send_cnt);
	}
	
	if(CBP_UART_PORT_ID != uPortNo)
    {
        dm_print_err("uPortNo %d  port not find\n", uPortNo);
        return ERROR;
    }
	
	if(NULL == pDataBuffer)
    {
        dm_print_err("pDataBuffer is null\n");
        return ERROR;
    }

    if(0 == uslength)
    {
        dm_print_err("uslength is 0\n");
        return ERROR;
    }
	
	if(NULL == cur_port->ops->send)
  	{
  		 dm_print_err("send function is null\n");
        return ERROR;
  	}
	wake_lock(&g_dual_modem_ctrl.wakelock);
	/*���ö�ʱ����300ms��˯��*/
	bsp_softtimer_delete(&g_dual_modem_ctrl.sleep_timer);
	/*��һ�η��ͻ򳬹�240ms��ͨ��ǰ�Ȼ���*/
	if(g_dual_modem_ctrl.wakeup_3rdmodem_flag == DO_WAKEUP_3RD)
	{
		if(OK != wakeup_via_modem())
		{
            dm_print_err("wakeup err\n");
			dual_modem_restart_timer(&g_dual_modem_ctrl.sleep_timer);
		    return ERROR;
		}
		osl_sem_downtimeout(&g_dual_modem_ctrl.wait_reply_mutex ,CBP_WAKEUP_DELAY_TIME_MS);
	}
    bsp_softtimer_delete(&g_dual_modem_ctrl.hold_wake_timer);

	ret = cur_port->ops->send(cur_port,pDataBuffer,uslength);
    if(OK != ret)
    {
        dm_print_err("uart tx layer err\n");
		dual_modem_restart_timer(&g_dual_modem_ctrl.sleep_timer);
		g_dual_modem_ctrl.wakeup_3rdmodem_flag = DO_WAKEUP_3RD;
		return ERROR;
	}
    /*���ö�ʱ����300ms��˯��*/
	dual_modem_restart_timer(&g_dual_modem_ctrl.sleep_timer);
	
	/*���ö�ʱ����240ms�ڲ��û��ѶԷ�*/
	g_dual_modem_ctrl.wakeup_3rdmodem_flag = NOT_WAKEUP_V3;
	dual_modem_restart_timer(&g_dual_modem_ctrl.hold_wake_timer);
    return OK;
}

/*****************************************************************************
 �� �� ��  : dual_modem_uart_irq_handler
 ��������  : uart�жϴ�����
 �������  : u32 uart_port_addr
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
static irqreturn_t dual_modem_uart_irq_handler(int uart_port_addr,void *dev)
{
	u8  ulData = 0;
    u32 regval = ERROR;
	u32 ret = ERROR;
	u32 maxrecvcnt = 0;
	UART_PORT *uart_port = NULL;
    UART_HW_DESC* uart_hw_desc = NULL;

    if(0 == uart_port_addr)
    {
        dm_print_err("uart_port_addr is null\n", uart_port_addr);
        return IRQ_NONE;
    }
	maxrecvcnt = UART_RECV_BUF_SIZE;
	uart_port = (UART_PORT *)uart_port_addr;
	uart_hw_desc = uart_port->hw_desc;
	
	DUAL_MODEM_RECV_STR *Hsuart_RecvStr = NULL;
	
	Hsuart_RecvStr = &g_dual_modem_ctrl.HSUART_RECV;

	g_dm_dump_info->recv_time_stamp = BSP_GetSliceValue();

	g_dm_dump_info->irq_cnt++;
	if(g_dual_modem_ctrl.dual_modem_log_flag == 1)
	{
		dm_debug_print("IrqCnt %d\n",g_dm_dump_info->irq_cnt);
	}
	regval = readl(uart_hw_desc->base_addr+ UART_REGOFFSET_MIS);
	regval &= UART_IRQ_MARK;
	while((0 != maxrecvcnt--) && (0 != (regval & (UART_ARMIP_REV_VALID | UART_ARMIP_REV_TIMEOUT))))
	{
		writel(UART_RX_IRQ_CLEAR | UART_TIMEOUT_IRQ_CLEAR,uart_hw_desc->base_addr + UART_REGOFFSET_ICR);
		regval = readl(uart_hw_desc->base_addr + UART_REGOFFSET_FR);		
		while((regval & UART_FR_RX_FIFO_EMP) == 0)
		{				
			ulData = readl(uart_hw_desc->base_addr + UART_REGOFF_RBR);
			ret = DualModem_InQue(Hsuart_RecvStr, ulData);
		    if(ret != OK)
		    {
		         dm_print_err("pstQue is null\n");
		         return IRQ_NONE;
		    }		
			regval = readl(uart_hw_desc->base_addr + UART_REGOFFSET_FR);	 				
		 }		
		regval = readl(uart_hw_desc->base_addr + UART_REGOFFSET_MIS);
		regval &= UART_IRQ_MARK;
	}
	if(0 == maxrecvcnt)
    {
        dm_print_err("write buffer full\n"); 
        return IRQ_NONE;
    }
	if(ret == OK)
	{
		/*ֹͣ300ms��˯�߶�ʱ��*/
		bsp_softtimer_delete(&g_dual_modem_ctrl.sleep_timer);
		wake_lock(&g_dual_modem_ctrl.wakelock);

		osl_sem_up(&Hsuart_RecvStr->recv_mutex);
		g_dm_dump_info->irq_SemGive_cnt++;
		if(g_dual_modem_ctrl.dual_modem_log_flag == 1)
		{
			dm_debug_print("IrqSemGiveCnt %d\n",g_dm_dump_info->irq_SemGive_cnt);
		}
	}
	writel(IPC_MBX19_INT_CLEAR , HI_IPC_S_REGBASE_ADDR + IPC_MBX_ICLR_REG_SETTOFF);
	return IRQ_HANDLED;
}
	
/*****************************************************************************
 �� �� ��  : dual_modem_uart_recv_task
 ��������  : ���ûص����������ϲ㷢������
 �������  : UART_PORT * uart_port_add �˿ڵ�ַ
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
void dual_modem_uart_recv_task(UART_PORT *uart_port_addr)
{
	s32 ret = 0;
	volatile u32 ulwrite = 0;
	volatile u32 this_size = 0;
    UART_PORT *uart_port = NULL;
	DUAL_MODEM_RECV_STR *Hsuart_RecvStr = NULL;

	Hsuart_RecvStr = &g_dual_modem_ctrl.HSUART_RECV;

    if(NULL == uart_port_addr)
    {
        dm_print_err("uart_port_addr is null\n");
        return ;
    }
    uart_port = uart_port_addr;

   	while(1)
    {
		osl_sem_down(&Hsuart_RecvStr->recv_mutex);
		g_dm_dump_info->rtask_SemTake_cnt++;
		if(g_dual_modem_ctrl.dual_modem_log_flag == 1)
		{
			dm_debug_print("RtaskSemTakecnt %d\n",g_dm_dump_info->rtask_SemTake_cnt);
		}
		ulwrite = Hsuart_RecvStr->ulWrite;
		bsp_softtimer_delete(&g_dual_modem_ctrl.sleep_timer);

        /*���ö�ʱ����240ms�ڲ��û��ѶԷ�*/
		dual_modem_restart_timer(&g_dual_modem_ctrl.hold_wake_timer);
		g_dual_modem_ctrl.wakeup_3rdmodem_flag = NOT_WAKEUP_V3;
        if(NULL != uart_port->recv_callback)
		{
			if(Hsuart_RecvStr->ulRead  == ulwrite)
        	{
                 /*���ö�ʱ����300ms��˯��*/
                dual_modem_restart_timer(&g_dual_modem_ctrl.sleep_timer);
		  //dm_print_err("buf is null \n");
                continue;
        	}
			if(Hsuart_RecvStr->ulRead < ulwrite)
			{
				this_size = ulwrite - Hsuart_RecvStr->ulRead;
        		ret = uart_port->recv_callback(CBP_UART_PORT_ID,(Hsuart_RecvStr->ucData + Hsuart_RecvStr->ulRead),this_size);			
				if((g_dm_dump_info->rx_cur_offset + this_size + 4) > DUAL_DUMP_RX_BUFF_SIZE)
				{
					g_dm_dump_info->rx_cur_offset = 0;
				}

				*(g_dm_dump_info->rx_dump_addr + g_dm_dump_info->rx_cur_offset) = (u8)((g_dm_dump_info->recv_time_stamp & 0xff000000) >> 24);
				g_dm_dump_info->rx_cur_offset ++;

				*(g_dm_dump_info->rx_dump_addr + g_dm_dump_info->rx_cur_offset) = (u8)((g_dm_dump_info->recv_time_stamp & 0x00ff0000) >> 16);
				g_dm_dump_info->rx_cur_offset ++;

				*(g_dm_dump_info->rx_dump_addr + g_dm_dump_info->rx_cur_offset) = (u8)((g_dm_dump_info->recv_time_stamp & 0x0000ff00) >> 8);
				g_dm_dump_info->rx_cur_offset ++;

				*(g_dm_dump_info->rx_dump_addr + g_dm_dump_info->rx_cur_offset) = (u8)(g_dm_dump_info->recv_time_stamp  & 0x000000ff);
				g_dm_dump_info->rx_cur_offset ++;

				memcpy((g_dm_dump_info->rx_dump_addr + g_dm_dump_info->rx_cur_offset) ,(Hsuart_RecvStr->ucData + Hsuart_RecvStr->ulRead), this_size);
				g_dm_dump_info->rx_cur_offset += this_size;
				
				g_dm_dump_info->rx_cur_size = this_size;
				g_dm_dump_info->rx_total_size += this_size;
				if(g_dual_modem_ctrl.dual_modem_log_flag == 1)
				{
					dm_debug_print("RecvCurSize %d\n",this_size);				
					dm_print_info((Hsuart_RecvStr->ucData + Hsuart_RecvStr->ulRead),this_size);
				}
			}
			else 
			{
				this_size = UART_RECV_BUF_SIZE - Hsuart_RecvStr->ulRead;
				ret = uart_port->recv_callback(CBP_UART_PORT_ID,(Hsuart_RecvStr->ucData + Hsuart_RecvStr->ulRead),this_size);			

				if((g_dm_dump_info->rx_cur_offset + this_size + 4) > DUAL_DUMP_RX_BUFF_SIZE)
				{
					g_dm_dump_info->rx_cur_offset = 0;
				}

				*(g_dm_dump_info->rx_dump_addr + g_dm_dump_info->rx_cur_offset) = (u8)((g_dm_dump_info->recv_time_stamp & 0xff000000) >> 24);
				g_dm_dump_info->rx_cur_offset ++;

				*(g_dm_dump_info->rx_dump_addr + g_dm_dump_info->rx_cur_offset) = (u8)((g_dm_dump_info->recv_time_stamp & 0x00ff0000) >> 16);
				g_dm_dump_info->rx_cur_offset ++;

				*(g_dm_dump_info->rx_dump_addr + g_dm_dump_info->rx_cur_offset) = (u8)((g_dm_dump_info->recv_time_stamp & 0x0000ff00) >> 8);
				g_dm_dump_info->rx_cur_offset ++;

				*(g_dm_dump_info->rx_dump_addr + g_dm_dump_info->rx_cur_offset) = (u8)(g_dm_dump_info->recv_time_stamp  & 0x000000ff);
				g_dm_dump_info->rx_cur_offset ++;

				memcpy((g_dm_dump_info->rx_dump_addr + g_dm_dump_info->rx_cur_offset) ,(Hsuart_RecvStr->ucData + Hsuart_RecvStr->ulRead), this_size);
				g_dm_dump_info->rx_cur_offset += this_size;
				
				g_dm_dump_info->rx_cur_size = this_size + ulwrite;
				g_dm_dump_info->rx_total_size += this_size;	

				if(g_dual_modem_ctrl.dual_modem_log_flag == 1)
				{
					dm_debug_print("RecvCurSize %d\n",g_dm_dump_info->rx_cur_size);
					dm_print_info((Hsuart_RecvStr->ucData + Hsuart_RecvStr->ulRead),this_size);
				}
				
				this_size = ulwrite;
				ret = uart_port->recv_callback(CBP_UART_PORT_ID,Hsuart_RecvStr->ucData,this_size);	

				if((g_dm_dump_info->rx_cur_offset + this_size + 4) > DUAL_DUMP_RX_BUFF_SIZE)
				{
					g_dm_dump_info->rx_cur_offset = 0;
				}

				*(g_dm_dump_info->rx_dump_addr + g_dm_dump_info->rx_cur_offset) = (u8)((g_dm_dump_info->recv_time_stamp & 0xff000000) >> 24);
				g_dm_dump_info->rx_cur_offset ++;

				*(g_dm_dump_info->rx_dump_addr + g_dm_dump_info->rx_cur_offset) = (u8)((g_dm_dump_info->recv_time_stamp & 0x00ff0000) >> 16);
				g_dm_dump_info->rx_cur_offset ++;

				*(g_dm_dump_info->rx_dump_addr + g_dm_dump_info->rx_cur_offset) = (u8)((g_dm_dump_info->recv_time_stamp & 0x0000ff00) >> 8);
				g_dm_dump_info->rx_cur_offset ++;

				*(g_dm_dump_info->rx_dump_addr + g_dm_dump_info->rx_cur_offset) = (u8)(g_dm_dump_info->recv_time_stamp  & 0x000000ff);
				g_dm_dump_info->rx_cur_offset ++;

				memcpy((g_dm_dump_info->rx_dump_addr + g_dm_dump_info->rx_cur_offset) ,Hsuart_RecvStr->ucData , this_size);
				g_dm_dump_info->rx_cur_offset += this_size;
				
				g_dm_dump_info->rx_total_size += this_size;

				if(g_dual_modem_ctrl.dual_modem_log_flag == 1)
				{
					dm_print_info(Hsuart_RecvStr->ucData,this_size);
				}
			}
			Hsuart_RecvStr->ulRead = ulwrite;
		}
		else 
		{
			dm_print_err("callback is null\n");
			g_dm_dump_info->callback_fail_cnt++;
		}
        /*���ö�ʱ����300ms��˯��*/
		dual_modem_restart_timer(&g_dual_modem_ctrl.sleep_timer);
    }
}
/*****************************************************************************
* �� �� ��  :  wakeup_via_modem
* ��������  :  gpio��ʼ��
* �������  :  ��
* �������  :  ��
* �� �� ֵ  :  0
* �޸ļ�¼  :  
*****************************************************************************/
s32 wakeup_via_modem(void)
{
	s32 ret = ERROR;
	u32 i = 0;
	u8 cbp_wakeup_string[] = {0x00,0x7e,0x7e,0x7e};//����CPB�����ַ���0x7e7e7e
	for(i = 0;i < 3;i++)
	{
		ret = dual_modem_send_bytes(&g_dual_modem_ctrl.uart_port[MUART1_ID],cbp_wakeup_string,sizeof(cbp_wakeup_string));
    	if(ERROR == ret)
    	{
        	dm_print_err("send wakeup string fail\n");
        	return ERROR;
    	}
	    osl_sem_downtimeout(&g_dual_modem_ctrl.wait_reply_mutex, CBP_WAKEUP_DELAY_TIME_MS);
		g_dm_dump_info->balong_wakeup_via_cnt++;
		if(g_dual_modem_ctrl.dual_modem_log_flag == 1)
		{
    		dm_debug_print("WakeupViaCnt %d\n",g_dm_dump_info->balong_wakeup_via_cnt);
    	}
    }
	return OK;
}
/*****************************************************************************
 �� �� ��  : DualModem_InQue
 ��������  : ���ٴ��ڽ��ջ�������Ӻ���
 �������  : pstQue ����������ָ��
             sucData ���յ����ַ�
 �������  : ��
 �� �� ֵ  : 
*****************************************************************************/
int DualModem_InQue(DUAL_MODEM_RECV_STR *pstQue, UINT8 ucData)
{
    u32 ulTail = 0;

    if(NULL == pstQue)
    {
		dm_print_err("pstQue is null\n");
		return ERROR;
    }
	
    ulTail = pstQue->ulWrite;
    pstQue->ucData[ulTail] = ucData;
    ulTail = ((ulTail+1) ==  UART_RECV_BUF_SIZE) ? 0 : (ulTail+1);
    pstQue->ulWrite = ulTail;
    return OK;
}
/*****************************************************************************
 �� �� ��  : modem_wakeup_delay_overhandler
 ��������  : 300ms��ʱ��Ͷ˯��Ʊ
 �������  : BSP_U32 uart_port_addr
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
void dual_modem_restart_timer(struct softtimer_list * softtimer)
{
	bsp_softtimer_delete(softtimer);
	bsp_softtimer_add(softtimer);
}

/*****************************************************************************
 �� �� ��  : modem_sleeptimer_overhandler
 ��������  : 300ms��ʱ��Ͷ˯��Ʊ
 �������  : BSP_U32 uart_port_addr
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
void modem_sleeptimer_overhandler(u32 temp)
{
	temp = temp;
	g_dm_dump_info->modem_sleeptimer_cnt++;
	g_dual_modem_ctrl.wakeup_3rdmodem_flag = DO_WAKEUP_3RD;
	bsp_softtimer_delete(&g_dual_modem_ctrl.hold_wake_timer);
	wake_unlock(&g_dual_modem_ctrl.wakelock);
}
/*****************************************************************************
 �� �� ��  : modem_awaketimer_overhandler
 ��������  : ����240ms��˫modemͨ��ǰҪ���ѶԷ�
 �������  : u32 temp
 �������  : ��
 �� �� ֵ  : void
*****************************************************************************/
void modem_awaketimer_overhandler(u32 temp)
{
	temp = temp;
	g_dual_modem_ctrl.wakeup_3rdmodem_flag = DO_WAKEUP_3RD;
}

/*****************************************************************************
 �� �� �� : dual_modem_wakeup_init
 �������� : ��ʼ��˯��ͶƱ����ʼ����ʱ����ע��iccͨ��
 ������� : ��
 ������� : 0:�ɹ���-1:ʧ��
 �� �� ֵ : int
*****************************************************************************/
int dual_modem_wakeup_init(DRV_DUAL_MODEM_STR DualModemNv)
{
	struct softtimer_list *T1_timer =&g_dual_modem_ctrl.hold_wake_timer;
	struct softtimer_list *T2_timer =&g_dual_modem_ctrl.sleep_timer;
	T1_timer->func = modem_awaketimer_overhandler;
	T1_timer->para = 0;	
	T1_timer->timeout = DualModemNv.AwakeTmer;
	T1_timer->wake_type = SOFTTIMER_NOWAKE;
	
	T2_timer->func = modem_sleeptimer_overhandler;
	T2_timer->para = 0;	
	T2_timer->timeout= DualModemNv.DoSleepTimer;
	T2_timer->wake_type = SOFTTIMER_NOWAKE;

	/*��ʼ��˯��ͶƱ�ӿ�*/	
	wake_lock_init(&g_dual_modem_ctrl.wakelock,PWRCTRL_SLEEP_UART0,"PWRCTRL_SLEEP_UART0");

 	g_dual_modem_ctrl.wakeup_3rdmodem_flag = DO_WAKEUP_3RD;
	/*��ʼ����ʱ����240ms�ڲ��û��ѶԷ�*/
	if(OK !=bsp_softtimer_create(T1_timer))
	{
		dm_print_err("T1_timer create fail!\n");
		goto T1timerfail;
	}
	/*��ʼ����ʱ����300ms��˯��*/
	if(OK !=bsp_softtimer_create(T2_timer))
	{
		dm_print_err("T2_timer create fail!\n");
		goto T2timerfail;
	}
	return OK;
	
T2timerfail:
	bsp_softtimer_free(T1_timer);
T1timerfail:
	return ERROR;	
}

/*****************************************************************************
 �� �� �� : dual_modem_dump_hook
 �������� : ϵͳ��λǰ���ú�����������Ϣ
 ������� : ��
 ������� : 
 �� �� ֵ : 
*****************************************************************************/
void dual_modem_dump_hook(void)
{
	if(NULL != g_dm_dump_info)
	{
		g_dm_dump_info->uart_reg_val[0]= readl(HI_UART5_REGBASE_ADDR + UART_REGOFFSET_FR);
		g_dm_dump_info->uart_reg_val[1] = readl(HI_UART5_REGBASE_ADDR + UART_REGOFFSET_IBRD);
		g_dm_dump_info->uart_reg_val[2] = readl(HI_UART5_REGBASE_ADDR + UART_REGOFFSET_LCR_H);
		g_dm_dump_info->uart_reg_val[3] = readl(HI_UART5_REGBASE_ADDR + UART_REGOFFSET_CR);
		g_dm_dump_info->uart_reg_val[4] = readl(HI_UART5_REGBASE_ADDR + UART_REGOFFSET_IFLS);
		g_dm_dump_info->uart_reg_val[5] = readl(HI_UART5_REGBASE_ADDR + UART_REGOFFSET_IMSC);
		g_dm_dump_info->uart_reg_val[6] = readl(HI_UART5_REGBASE_ADDR + UART_REGOFFSET_MIS);
	}
	return;
}

/*****************************************************************************
 �� �� �� : dual_modem_dump_init
 �������� : dump ��ʼ��
 ������� : ��
 ������� : 
 �� �� ֵ : 
*****************************************************************************/
s32 dual_modem_dump_init(void)
{
	u8 *dump_addr = NULL;

	if(OK != bsp_dump_register_hook(DUMP_EXT_DUAL_MODEM_SIZE , (dump_save_hook)dual_modem_dump_hook))
	{
		dm_print_err("dump hook fail");
		return ERROR;
	}
	
	dump_addr = bsp_dump_register_field((u32)DUMP_SAVE_MOD_DUAL_MODEM , DUMP_EXT_DUAL_MODEM_SIZE);
	if(dump_addr == NULL)
	{
		dm_print_err("dual modem dump no buffer\n");
		return ERROR;
	}
	dm_print_err("dual modem dump addr 0x%x\n",(u32)dump_addr);
    memset(dump_addr ,0 ,DUMP_EXT_DUAL_MODEM_SIZE);
	g_dm_dump_info = (struct dm_dump_info *)dump_addr;
	g_dm_dump_info->rx_dump_addr = (u8 *)(dump_addr + DUAL_DUMP_COUNT_SIZE);
	g_dm_dump_info->tx_dump_addr = (u8 *)(g_dm_dump_info->rx_dump_addr + DUAL_DUMP_RX_BUFF_SIZE);
	return OK;
}
/*****************************************************************************
 �� �� ��  : dual_modem_uart_port_init
 ��������  : ������������ʹ�ܴ����жϣ����ô��ڲ�����
 �������  : UART_PORT *uart_port
 �������  : 0 �ɹ���-1ʧ��
 �� �� ֵ  : s32
*****************************************************************************/
s32 dual_modem_uart_port_init(UART_PORT *uart_port)
{
	u8 flag = 0;
	DUAL_MODEM_RECV_STR *Hsuart_RecvStr = NULL;
		
	Hsuart_RecvStr = &g_dual_modem_ctrl.HSUART_RECV;
	flag = LPm3_UART5_IQR_ENABLE;	
	if(NULL == uart_port)
    {
        dm_print_err("uart_port is null\n");
        return ERROR;
    }

    if(NULL == uart_port->hw_desc)
    {
        dm_print_err("uart_hw_desc is null\n");
        return ERROR;
    }
	
	memset((void*)&g_dual_modem_ctrl.HSUART_RECV,0,sizeof(g_dual_modem_ctrl.HSUART_RECV)); 
	osl_sem_init(0,&g_dual_modem_ctrl.wait_reply_mutex);
	osl_sem_init(0,&Hsuart_RecvStr->recv_mutex);
   	uart_port->send_mutex= semMCreate(SEM_Q_PRIORITY |SEM_DELETE_SAFE| SEM_INVERSION_SAFE);

	/* ���ò����� */
	dual_modem_uart_channel_init(uart_port->hw_desc);
	
	if(OK != osl_task_init("utlrecv",DUAL_MODEM_TASK_PRO,DUAL_MODEM_TASK_STK,(void *)dual_modem_uart_recv_task,(UART_PORT *)uart_port,&Hsuart_RecvStr->recv_task_id))
    {
        dm_print_err("OSAL_TaskCreate utlrecv err\n");
        return  ERROR;
    }

	request_irq(uart_port->hw_desc->irq_num, (irq_handler_t)dual_modem_uart_irq_handler, 0 , "dual modem irq", uart_port);
	/* ֪ͨlpm3 ��ʼ��uart5	*/	
	bsp_icc_send((u32)ICC_CPU_MCU,(ICC_CHN_MCORE_CCORE << 16)|MCORE_CCORE_FUNC_UART,&flag,sizeof(flag));	

    return OK;
}
/*****************************************************************************
 �� �� ��  : bsp_dual_modem_init
 ��������  : K3 modem uart��س�ʼ��
 �������  : void
 �������  : -1:ʧ�ܣ�0:�ɹ�
 �� �� ֵ  : int
****************************************************************************/
s32 dual_modem_init(void)
{
	int ret = ERROR;

	DRV_DUAL_MODEM_STR dual_modem_nv;
 	unsigned int  retVal = 0;
	DRV_DM_UART5_STR dm_init_nv;
	
	memset((void*)&g_dual_modem_ctrl.uart_port,0,sizeof(g_dual_modem_ctrl.uart_port)); 			//��ʼ����������
	memset((void*)&dual_modem_nv,0,sizeof(DRV_DUAL_MODEM_STR));

	retVal =bsp_nvm_read(NV_ID_DRV_DUAL_MODEM,(u8 *)&dual_modem_nv,sizeof(DRV_DUAL_MODEM_STR));
    if (retVal != OK)
    {
        dm_print_err("read NV_ID_DRV_UART_FLAG NV ERROR: %d \n",NV_ID_DRV_DUAL_MODEM);
        return ERROR;
    }
	
	if(UART_SWITCH_ENABLE == dual_modem_nv.enUartEnableCfg)
    {
		memset((void *)&dm_init_nv ,0 ,sizeof(DRV_DM_UART5_STR));
		ret = bsp_nvm_read(NV_ID_DRV_DM_UART5_CFG, (u8 *)&dm_init_nv, sizeof(DRV_DM_UART5_STR));
		if(ret != OK)
		{
			dm_print_err("read dm_uart5_irq_nv NV ERROR: %d \n",NV_ID_DRV_DM_UART5_CFG);
			dm_init_nv.enUart5IrqCfg = 0;
			dm_init_nv.dmInitCfg = 0;
		}
		
		ret = dual_modem_wakeup_init(dual_modem_nv);
		if(ret !=OK)
		{
			dm_print_err("dual modem wakeup init failed!\n");
			return ERROR;
		}
		if(OK != dual_modem_dump_init())
		{
			dm_print_err("dual_modem_dump_init fail!\n");
			return ERROR;
    	}
		
		/* ע��ICC��д�ص� */
		if(OK != bsp_icc_event_register((ICC_CHN_MCORE_CCORE << 16)|MCORE_CCORE_FUNC_UART,recv_lpm3_msg_icc_cb , NULL, NULL, NULL))
    	{
			dm_print_err("register icc callback fail!\n");
			return ERROR;
    	}
		
		if(UART_SWITCH_DISABLE == dm_init_nv.dmInitCfg)
		{
			if(OK != bsp_dual_modem_init())
			{
				dm_print_err("uart init fail!\n");
				return ERROR;	
			}
		}
    }
    return OK;
}
s32 bsp_dual_modem_init(void)
{	
	int ret = ERROR;

	DRV_DUAL_MODEM_STR dual_modem_nv;
 	unsigned int  retVal = 0;
	memset((void*)&dual_modem_nv,0,sizeof(DRV_DUAL_MODEM_STR));

	retVal =bsp_nvm_read(NV_ID_DRV_DUAL_MODEM,(u8 *)&dual_modem_nv,sizeof(DRV_DUAL_MODEM_STR));
    if (retVal != OK)
    {
        dm_print_err("read NV_ID_DRV_UART_FLAG NV ERROR: %d \n",NV_ID_DRV_DUAL_MODEM);
        return ERROR;
    }
	
	if(UART_SWITCH_ENABLE == dual_modem_nv.enUartEnableCfg)
    {
		g_dual_modem_ctrl.uart_port[MUART1_ID].hw_desc = &g_dual_modem_init_info.uart_port_hw_desc;
        g_dual_modem_ctrl.uart_port[MUART1_ID].port_id = MUART1_ID;
        g_dual_modem_ctrl.uart_port[MUART1_ID].ops = &muart2_ops;						      //���ͻص�����
		
		if(UART_SWITCH_ENABLE == dual_modem_nv.enUartlogEnableCfg)
		{
			g_dual_modem_ctrl.dual_modem_log_flag = 1;	
			bsp_mod_level_set(BSP_MODU_DUAL_MODEM ,BSP_LOG_LEVEL_DEBUG);
		}
		/* uart5ʱ�ӷ�Ƶ�ȿ��ƼĴ��� */
	   	writel(GT_CLK_UARTL_OPEN , HI_LP_PERI_CRG_REG_ADDR + PERI_CRG_CLKDIV19_REG);
		/* �� uart5 ʱ�� */
		writel(GT_CLK_UART5_ENABLE , HI_LP_PERI_CRG_REG_ADDR + PERI_CRG_PEREN2_REG);
		/* ����������λ����Ĵ���*/
		writel(GT_RST_UART5_ENABLE , HI_LP_PERI_CRG_REG_ADDR + PERI_CRG_PERRSTDIS2_REG);
	
		ret = dual_modem_uart_port_init(&g_dual_modem_ctrl.uart_port[MUART1_ID]);
		if(ret !=OK)
		{
			dm_print_err("dual modem uart port init failed!\n");
			return ERROR;;			
		}     
#ifdef CONFIG_CCORE_PM
	   if(bsp_device_pm_add(&dual_modem_device))
	   {
		   dm_print_err("dual_modem_device add erro\n");
		   return ERROR;
	   }
#endif			
		g_dual_modem_ctrl.dual_modem_init_flag = UART_SWITCH_ENABLE;		
    }
    return OK;
}

/*****************************************************************************
* �� �� ��  :  dual_uart_suspend
* ��������  :  ����5����
* �������  :  ��
* �������  :  ��
* �� �� ֵ  :  0
* �޸ļ�¼  :  
*****************************************************************************/
s32 dual_uart_suspend(struct dpm_device *dev)
{
	UART_PORT *uart_port = NULL;
	uart_port = &g_dual_modem_ctrl.uart_port[MUART1_ID];
	writel(GT_CLK_UART5_DISABLE , HI_LP_PERI_CRG_REG_ADDR + PERI_CRG_PEREN2_REG);
	writel(GT_CLK_UARTL_CLOSS , HI_LP_PERI_CRG_REG_ADDR + PERI_CRG_CLKDIV19_REG);
	if(NULL != g_dm_dump_info)
		g_dm_dump_info->suspend_cnt++;
	return 0;
}

/*****************************************************************************
* �� �� ��  :  dual_uart_resume
* ��������  :  ����5�ָ�
* �������  :  ��
* �������  :  ��
* �� �� ֵ  :  0 : �ɹ�[״̬��־λȡ��
*              1 : ʧ��[״̬��־λȡ��
* �޸ļ�¼  :
*****************************************************************************/
s32 dual_uart_resume(struct dpm_device *dev)
{
	UART_PORT *uart_port = NULL;
	uart_port = &g_dual_modem_ctrl.uart_port[MUART1_ID];
	
	/* uartlʱ�ӷ�Ƶ�ȿ��ƼĴ��� */
	writel(GT_CLK_UARTL_OPEN, HI_LP_PERI_CRG_REG_ADDR + PERI_CRG_CLKDIV19_REG);
	/* ��uart5ʱ�� */
	writel(GT_CLK_UART5_ENABLE , HI_LP_PERI_CRG_REG_ADDR + PERI_CRG_PEREN2_REG);
	/* ����������λ����Ĵ���*/
	writel(GT_RST_UART5_ENABLE , HI_LP_PERI_CRG_REG_ADDR + PERI_CRG_PERRSTDIS2_REG);
	dual_modem_uart_channel_init(uart_port->hw_desc);
	if(NULL != g_dm_dump_info)
		g_dm_dump_info->resume_cnt++;
	return 0;
}

/*****************************************************************************
* �� �� ��  : uart_driver_info
*
* ��������  : UART ��ά�ɲ�ӿ�
*
* �������  : NA
* �������  : NA
*
* �� �� ֵ  : NA
*
*****************************************************************************/
void uart_driver_info(void)
{	
	if(g_dm_dump_info != NULL)
	{
		dm_print_err("recv_register_cnt    %d\n",g_dm_dump_info->recv_register_cnt);
		dm_print_err("irq_cnt    %d\n",g_dm_dump_info->irq_cnt);
		dm_print_err("irq_SemGive_cnt    %d\n",g_dm_dump_info->irq_SemGive_cnt);
	    dm_print_err("rtask_SemTake_cnt    %d\n",g_dm_dump_info->rtask_SemTake_cnt);
	
		dm_print_err("cbpa_send_count    %d\n",g_dm_dump_info->cbpa_send_cnt);
		dm_print_err("tx_cur_size    %d\n",g_dm_dump_info->tx_cur_size);
		dm_print_err("tx_total_size    %d\n",g_dm_dump_info->tx_total_size);
		dm_print_err("send_mutex    %d\n",g_dm_dump_info->send_mutex_cnt);

		dm_print_err("rx_total_size    %d\n",g_dm_dump_info->rx_total_size);
		dm_print_err("suspend_cnt    %d\n",g_dm_dump_info->suspend_cnt);
	    dm_print_err("resume_cnt    %d\n",g_dm_dump_info->resume_cnt);

		dm_print_err("via_wakeup_balong_cnt    %d\n",g_dm_dump_info->via_wakeup_balong_cnt);
		dm_print_err("balong_wakeup_via_cnt	   %d\n",g_dm_dump_info->balong_wakeup_via_cnt);
		dm_print_err("modem_sleeptimer_cnt	  %d\n",g_dm_dump_info->modem_sleeptimer_cnt);
	
		dm_print_err("callback_fail_cnt    %d\n",g_dm_dump_info->callback_fail_cnt);
	t_read_reg();
	}
	else 
	{
		dm_print_err("dump addr is null");
	}
}

int dm_print_info(unsigned char *pData, unsigned int ulLength)
{
	char dm_debug_buff[256];
	char *p_buff = dm_debug_buff;
	u8 *pdatabuff = pData;
	s32 ret = 0;
	u32 i = 0;
	u32 datalen = ulLength;
	dm_debug_buff[0] = 0;
	for(i = 0;i < ulLength; i++)
    {
		ret = snprintf(p_buff, sizeof(dm_debug_buff)-(p_buff - dm_debug_buff + 1), "%x ", *pdatabuff);
		if (ret <= 0)
		{
			dm_debug_print("dm_print_info error, code:%d \n",ret);
			return ERROR;
		}
		p_buff += ret;
		pdatabuff++;
		datalen--;
	    if(((sizeof(dm_debug_buff)-(p_buff - dm_debug_buff + 1)) < 3) || (datalen == 0))
		{
			*(p_buff) = '\0';
			dm_debug_print("%s\n" ,dm_debug_buff);
			p_buff = dm_debug_buff;
		}
	}
    return OK;
}
void t_read_reg(void)
{
	u32 regval[7] = {0};

	regval[0] = readl(HI_UART5_REGBASE_ADDR + 0x18);
	regval[1] = readl(HI_UART5_REGBASE_ADDR + 0x24);
	regval[2] = readl(HI_UART5_REGBASE_ADDR + 0x2c);
	regval[3] = readl(HI_UART5_REGBASE_ADDR + 0x30);
	regval[4] = readl(HI_UART5_REGBASE_ADDR + 0x34);
	regval[5] = readl(HI_UART5_REGBASE_ADDR + 0x38);
	regval[6] = readl(HI_UART5_REGBASE_ADDR + 0x40);
	dm_print_err("UARTFR VALUE 0x%x\n", regval[0]);
	dm_print_err("UARTIBRD  VALUE 0x%x\n", regval[1]);
	dm_print_err("UARTLCR_H  VALUE 0x%x\n", regval[2]);
	dm_print_err("UARTCR  VALUE 0x%x\n", regval[3]);
	dm_print_err("UARTIFLS  VALUE 0x%x\n", regval[4]);
	dm_print_err("UARTIMSC  VALUE 0x%x\n", regval[5]);
	dm_print_err("UARTMIC  VALUE 0x%x\n", regval[6]);
}
/******************test***************/

#if 1
u32 send_task;
void ut_send(u32 lenth)
{
    u32 i = 0;
    int ret = ERROR;
    unsigned char dual_modem_buf[1024];
	
	for(i = 0; i < lenth; i++)
    {
        dual_modem_buf[i]=i;
    }

    ret = uart_core_send(CBP_UART_PORT_ID,dual_modem_buf,lenth);
}

void t_send(void)
{
    if(OK != osl_task_init("utlsend",53,8192,(void *)ut_send,0,&send_task))
    {
         dm_print_err("OSAL_TaskCreate utlsend err\n");
    }
}

int tt_recv_handler(UART_CONSUMER_ID uPortNo,unsigned char *pData, unsigned int ulLength)
{
	return 0;
}

void t_recv_reg(void)
{
 	
	(int)uart_core_recv_handler_register(CBP_UART_PORT_ID,tt_recv_handler);
}

void t_write_reg(void)
{
	u8 temp = 0xaa;
	writel(temp, HI_UART5_REGBASE_ADDR);
}
void t_resq_via(void)
{
	u8 hi_via[14] ={0x7e,0x00,0x00,0x06,0x00,0x00,0x00,0xee,0x00,0x00,0x00,0x01,0x64,0x7e};
	uart_core_send(CBP_UART_PORT_ID,hi_via,14);
}

void t_send_to_via(void)
{
	u32 ret = ERROR;
	u8 hi_char[11] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x32,0xdf,0x7e};
	
	ret = uart_core_send(CBP_UART_PORT_ID,hi_char,11);
	if(ret == OK)
		dm_print_err("SEND SUCCESS!\n");
}

void t_send_via(void)
{
	u8 hi_via[64] ={0x7e,0x00,0x00,0x43,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,
					0x00,0x01,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00,
					0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
					0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
					0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
					0x00,0x00,0x00,0x00};
		u32 ret = ERROR;
		
		ret = uart_core_send(CBP_UART_PORT_ID,hi_via,64);
	if(ret == OK)
		dm_print_err("SEND SUCCESS!\n");

}


void clear_recv_num(void)
{
	g_dual_modem_ctrl.HSUART_RECV.ulTotalNum =0;
}
void t_dbg(void)
{
   system_error(0, 0, 0, NULL, 0);
}

#endif

