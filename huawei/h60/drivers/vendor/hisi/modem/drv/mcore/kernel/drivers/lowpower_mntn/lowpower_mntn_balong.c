/*************************************************************************
*   ��Ȩ����(C) 1987-2011, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  lowpower_mntn_balong.c
*
*   ��    �� :  xujingcui
*
*   ��    �� : ���ļ����ڵ͹��Ŀ�ν�ɲ���Ϣ�ļ�¼���ϱ�
*
*   �޸ļ�¼ :  2013��6��10��  v1.00 xujingcui����
*************************************************************************/
#include <bsp_lowpower_mntn.h>
#include <soc_interrupts_m3.h> /*lint !e537*/
#include <m3_dump.h>
#ifdef CONFIG_V7R2_CLK_CRG
#define LP_REG_MNTN_NULL   0xFFFFFFFF

struct lp_dump_mem_info lowpower_dump_bus_error = {
	0,
	0,
	0,
};
/*��¼��Ҫ����ĵ͹��ļĴ���,�Ĵ���Ϊ����ֱ��readl�ļĴ���*/
#ifdef CONFIG_V7R2_CLK_CRG /* CONFIG_V7R2_CLK_CRG */
/*��¼��Ҫ����ķ������Ĵ�����ֵ*/
unsigned int lowpower_bus_error_register[]={
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT41_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT42_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT43_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT44_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT45_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT46_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT47_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT48_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT49_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT50_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT51_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT52_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT53_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT54_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT55_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT56_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT57_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT58_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT59_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT60_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT61_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT62_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT63_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT64_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT65_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT66_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT67_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT68_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_STAT69_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_CRG_CLKSTAT1_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_CRG_CLKSTAT2_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_CRG_CLKSTAT3_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_CRG_CLKSTAT4_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_CRG_CLKSTAT5_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_CRG_SRSTSTAT1_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_CRG_SRSTSTAT2_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_CRG_SRSTSTAT3_OFFSET,
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_CRG_CLK_SEL3_OFFSET,       /*BBP SEL*/
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_CRG_DFS2_CTRL3_OFFSET,   /*DSP PLL*/
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_CRG_DFS3_CTRL3_OFFSET,   /*HIFI PLL*/
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_CRG_DFS4_CTRL3_OFFSET,   /*BBP PLL*/
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_CRG_DFS5_CTRL3_OFFSET,   /*PERI PLL*/
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_PWR_CTRL2_OFFSET,            /*TCXO*/
	HI_SYSCTRL_BASE_ADDR_VIRT + HI_PWR_STAT4_OFFSET,            /*MTCOMS*/
	LP_REG_MNTN_NULL
};
#else
/*��¼��Ҫ����ķ������Ĵ�����ֵ*/
unsigned int lowpower_bus_error_register[]={
	LP_REG_MNTN_NULL
};
#endif /* CONFIG_V7R2_CLK_CRG */

/*��¼���һ�ι����Ĵ�����ֵ*/
#define LP_SAVE_BUS_ERROR_REG_NUM   80
unsigned int lowpower_last_bus_error_register[LP_SAVE_BUS_ERROR_REG_NUM] = {0};

void bsp_dump_bus_error_status(void)
{
	unsigned int i = 0;
	if(lowpower_bus_error_register[0] == LP_REG_MNTN_NULL){
		printk("Has nothing to dump!\n");
		return;
	}
	do{
		printk("d 0x%x = 0x%x\n", lowpower_bus_error_register[i], lowpower_last_bus_error_register[i]);
		i++;
	}while(lowpower_bus_error_register[i] != LP_REG_MNTN_NULL);

	printk("print bus error dump base Address: 0x%x,Total length: 0x%x\n\n", lowpower_dump_bus_error.lp_dump_base_addr, lowpower_dump_bus_error.lp_dump_base_addr_legth);
	return;
}

static irqreturn_t bsp_prevent_bus_error_reg_save(int irq, void* dev)
{
	unsigned int i = 0;
	unsigned int regvalue = 0;
#ifdef CONFIG_V7R2_CLK_CRG
	/*���ж�*/
	regvalue = readl(HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_CTRL69_OFFSET);
	regvalue |= (unsigned int)0x1 << 7;
	writel(regvalue, HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_CTRL69_OFFSET);
	regvalue &= ~((unsigned int)0x1 << 7);
	writel(regvalue, HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_CTRL69_OFFSET);
#endif
	if(lowpower_bus_error_register[0] == LP_REG_MNTN_NULL){
		return IRQ_HANDLED;
	}
	/*��Ǽ�¼�����*/
	if(lowpower_dump_bus_error.lp_current_legth > lowpower_dump_bus_error.lp_dump_base_addr_legth){
		lowpower_dump_bus_error.lp_current_legth = 0;
	}
	*((u32 *)lowpower_dump_bus_error.lp_dump_base_addr + lowpower_dump_bus_error.lp_current_legth) = 0x88888888;/* [false alarm]:�� */
	lowpower_dump_bus_error.lp_current_legth++;

	/*��Ǽ�¼��ʱ���*/
	if(lowpower_dump_bus_error.lp_current_legth > lowpower_dump_bus_error.lp_dump_base_addr_legth){
		lowpower_dump_bus_error.lp_current_legth = 0;
	}
	*((u32 *)lowpower_dump_bus_error.lp_dump_base_addr + lowpower_dump_bus_error.lp_current_legth) = bsp_get_slice_value();/* [false alarm]:�� */
	lowpower_dump_bus_error.lp_current_legth++;

	/*��ȡ�Ĵ�����ֵ����DUMP MEM*/
	do{

        /*��¼�Ĵ�����ַ���ڴ���*/
        if(lowpower_dump_bus_error.lp_current_legth > lowpower_dump_bus_error.lp_dump_base_addr_legth){
            lowpower_dump_bus_error.lp_current_legth = 0;
        }
        *((u32 *)lowpower_dump_bus_error.lp_dump_base_addr + lowpower_dump_bus_error.lp_current_legth) = lowpower_bus_error_register[i]; /* [false alarm]:�� */
        lowpower_dump_bus_error.lp_current_legth++;

        /*��¼�Ĵ���ֵ���ڴ���*/
		if(lowpower_dump_bus_error.lp_current_legth > lowpower_dump_bus_error.lp_dump_base_addr_legth){
			lowpower_dump_bus_error.lp_current_legth = 0;
		}
		regvalue = readl(lowpower_bus_error_register[i]);
               *((u32 *)lowpower_dump_bus_error.lp_dump_base_addr + lowpower_dump_bus_error.lp_current_legth) = regvalue;/* [false alarm]:�� */

	        /*����Ĵ���ֵ��������*/
		lowpower_last_bus_error_register[i] = regvalue;
	    lowpower_dump_bus_error.lp_current_legth++;
		i++;
	}while(lowpower_bus_error_register[i] != LP_REG_MNTN_NULL);
	return IRQ_HANDLED;
}
void bsp_lowpower_mntn_init(void)
{
	NV_DUMP_STRU g_dump_nv_lp;
	int ret = 0;

	/*����NV����оƬ�����������Ƿ��*/
	ret = bsp_nvm_read(NV_ID_DRV_DUMP, (u8 *)(&g_dump_nv_lp), sizeof(NV_DUMP_STRU));/*lint !e713*/
	if(ret!=0)
	{
		printk("NV_ID_DRV_DUMP:D111 read failure!\n");
		return;
	}else{
		printk("NV_ID_DRV_DUMP:D111 read success!\n");
	}
	if(g_dump_nv_lp.BusErrorFlagSet){
#ifdef CONFIG_V7R2_CLK_CRG

		/*��ȡDUMP���ڴ����ڼ�¼���߹�����ؼĴ���*/
		lowpower_dump_bus_error.lp_dump_base_addr = (unsigned int)DUMP_EXT_LOW_POWER_MCORE_BUSERROR;
		lowpower_dump_bus_error.lp_dump_base_addr_legth = DUMP_EXT_LOW_POWER_MCORE_BUSERROR_SIZE / 4;
		memset((void *)(lowpower_dump_bus_error.lp_dump_base_addr),0,(4 * lowpower_dump_bus_error.lp_dump_base_addr_legth));
		ret = request_irq(M3_X2H_X2P_ERR, (irq_handler_t)bsp_prevent_bus_error_reg_save,0,"mcore_bus_error129",NULL);
		if(ret)
		{
			printk("M3_X2H_X2P_ERR request_irq failed!\n");
			return;
		}
		ret = request_irq(M3_AXI_ERR, (irq_handler_t)bsp_prevent_bus_error_reg_save,0,"mcore_bus_error130",NULL);
		if(ret)
		{
			printk("M3_AXI_ERR request_irq failed!\n");
			return;
		}

		/*���÷�����������ؼĴ���*/
		writel(0x7F, HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_CTRL69_OFFSET);
		writel(0x4, HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_CTRL103_OFFSET);
		writel(0x0, HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_CTRL104_OFFSET);
		writel(0x0, HI_SYSCTRL_BASE_ADDR_VIRT + HI_SC_CTRL105_OFFSET);
#else
		printk("This version don't support prevent bus error identity!");
#endif
	}
	return;
}
#else
void bsp_lowpower_mntn_init(void)
{
    return;
}
void bsp_dump_bus_error_status(void)
{
    printk("This version M3 don't support prevent bus error identity!");
    return;
}
#endif

