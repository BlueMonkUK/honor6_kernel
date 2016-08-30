/*************************************************************************
*   ��Ȩ����(C) 1987-2004, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  BSP_GPIO.c
*
*   ��    �� :  liuwenhui
*
*   ��    �� :  GPIO�����ļ�
*
*   �޸ļ�¼ :  2012��11��27��
*
*************************************************************************/
/* includes */
/*lint -save -e537*/
#include <vxWorks.h>
#include <iv.h>
#include <stdio.h>
#include <string.h>
#include <intLib.h>
#include <logLib.h>
#include <semLib.h>
#include <taskLib.h>
#include "osl_bio.h"
#include "osl_list.h"
#include "soc_clk.h"

#include <hi_gpio.h>
#include "hi_gpio.h"
#include "bsp_memmap.h"
#include <bsp_ipc.h>
#include "bsp_clk.h"
#include "gpio_balong.h"
#include "bsp_dpm.h"
#include "bsp_om.h"
/*lint -restore */

#ifdef __cplusplus
extern "C" {
#endif

#define  gpio_print_error(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_GPIO, "[gpio]: <%s> <%d> "fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__))
#define  gpio_print_info(fmt, ...)     (bsp_trace(BSP_LOG_LEVEL_ERROR,  BSP_MODU_GPIO, "[gpio]: "fmt, ##__VA_ARGS__))

#ifndef HI_K3_GPIO

/*�Ƿ��ʼ����־*/
static u32 g_u32GpioRunning = GPIO_DEF_NOT_RUNNING; /* GPIO�Ƿ��ʼ����־*/

/*ÿ��GPIO��ַ*/
static u32 s_u32GpioBaseAddr[GPIO_MAX_BANK_NUM]=
{
    HI_GPIO0_REGBASE_ADDR,
    HI_GPIO1_REGBASE_ADDR,
    HI_GPIO2_REGBASE_ADDR,
    HI_GPIO3_REGBASE_ADDR,

#if (GPIO_MAX_BANK_NUM == 6)
    HI_GPIO4_REGBASE_ADDR,
    HI_GPIO5_REGBASE_ADDR
#endif
};

#ifdef CONFIG_CCORE_PM
static int bsp_gpio_suspend(struct dpm_device *dev)
{
	return OK;
}

static int bsp_gpio_resume(struct dpm_device *dev)
{
	return OK;
}

struct dpm_device gpio_device={
	.device_name = "balong gpio driver",
	.suspend_late = NULL,
	.prepare = NULL,
	.suspend = bsp_gpio_suspend,
	.resume = bsp_gpio_resume,
	.resume_early = NULL,
	.complete = NULL,
};

#endif

/*****************************************************************************
* �� �� ��  : bsp_gpio_init
*
* ��������  : GPIO��ʼ���ӿ�
*
* �������  : ��
*
* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2012��11��27��
*****************************************************************************/
s32 bsp_gpio_init(void)
{
    u32 i = 0;
    char gpio_clk_name[40] = "";
    struct clk *gpio_clk = NULL;

    if (GPIO_DEF_RUNNING == g_u32GpioRunning)
    {
        return GPIO_OK;
    }

    for(i = 1; i < GPIO_MAX_BANK_NUM; i++)
    {
        memset(gpio_clk_name, 0 , 40);
        snprintf(gpio_clk_name, 40, "gpio%d_clk", i); /*lint !e119*/
        gpio_clk = (struct clk *)clk_get(NULL, gpio_clk_name);
    	if(IS_ERR(gpio_clk))
        {
    		gpio_print_error("gpio%d clk cannot get, 0x%x.\n", i, gpio_clk);
    		goto clk_get_error;
    	}

        if(0 != clk_enable(gpio_clk))
        {
    		gpio_print_error("gpio%d clk enable is fail, 0x%x.\n", i, gpio_clk);
    		goto clk_enable_error;
        }

    }

    gpio_print_info("gpio init ok.\n");

    g_u32GpioRunning = GPIO_DEF_RUNNING;

    return GPIO_OK;

clk_enable_error:
    //clk_put(gpio_clk);

clk_get_error:
    while(--i > 0)
    {
        //clk_disable(gpio_clk);
        //clk_put(gpio_clk);
    }

    return GPIO_ERROR;

}


/*****************************************************************************
* �� �� ��  : bsp_gpio_direction_input
*
* ��������  : ����GPIO���ŷ���Ϊ����
*
* �������  : UINT32 gpio    GPIO���ű��
*
* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2012��11��27��
*****************************************************************************/
void bsp_gpio_direction_input(u32 gpio)
{
    u32 chip_num;
    u32 pin;
    u32 u32GpioAddr = 0;
    u32 gpio_value = 0;
    int old_level;

    /* ���GPIO û�г�ʼ�������ش��� */
    if (GPIO_DEF_NOT_RUNNING == g_u32GpioRunning)
    {
        gpio_print_error(" GPIO is not inited.\n");
        return ;
    }

    if(gpio >= GPIO_TOTAL_PINS_NUM)
    {
        gpio_print_error(" GPIO parameter error.\n");
        return ;
    }
    chip_num   = gpio / GPIO_MAX_PINS;
    pin        = gpio % GPIO_MAX_PINS;

    u32GpioAddr = s_u32GpioBaseAddr[chip_num];

    old_level = intLock();
    bsp_ipc_spin_lock(IPC_SEM_GPIO);

    gpio_value = readl(u32GpioAddr + HI_GPIO_SWPORT_DDR_OFFSET);

    writel(gpio_value & ~((u32)0x1 << pin), u32GpioAddr + HI_GPIO_SWPORT_DDR_OFFSET);

    bsp_ipc_spin_unlock(IPC_SEM_GPIO);
    intUnlock (old_level);

}

/*****************************************************************************
* �� �� ��  : bsp_gpio_direction_output
*
* ��������  : ����GPIO���ŵķ���Ϊ����������ø�����Ĭ�ϵ�ƽֵ
*
* �������  : UINT32 gpio       GPIO���ű��
*             UINT32 value      �����õ�ƽֵ,��0Ϊ�͵�ƽ��1Ϊ�ߵ�ƽ
*
* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2012��11��27��
*****************************************************************************/
void bsp_gpio_direction_output(u32 gpio, u32 value)
{
    u32 chip_num;
    u32 pin;
    u32 u32GpioAddr = 0;
    u32 gpio_value = 0;
    int old_level;

    /* ���GPIO û�г�ʼ�������ش��� */
    if (GPIO_DEF_NOT_RUNNING == g_u32GpioRunning)
    {
        gpio_print_error(" GPIO is not inited.\n");
        return ;
    }

    if(gpio >= GPIO_TOTAL_PINS_NUM)
    {
         gpio_print_error(" GPIO parameter error.\n");
        return ;
    }

    chip_num   = gpio / GPIO_MAX_PINS;
    pin        = gpio % GPIO_MAX_PINS;

    u32GpioAddr = s_u32GpioBaseAddr[chip_num];

    old_level = intLock();
    bsp_ipc_spin_lock(IPC_SEM_GPIO);

    gpio_value = readl(u32GpioAddr + HI_GPIO_SWPORT_DDR_OFFSET);
    writel(gpio_value | ((u32)0x1 << pin), u32GpioAddr + HI_GPIO_SWPORT_DDR_OFFSET);

    bsp_ipc_spin_unlock(IPC_SEM_GPIO);
    intUnlock (old_level);

    bsp_gpio_set_value(gpio, value);

}

/*****************************************************************************
* �� �� ��  : bsp_gpio_direction_get
*
* ��������  : ��ѯGPIO ���ŵķ���
*
* �������  : UINT32 gpio        GPIO���ű��
*
* �� �� ֵ  : ����gpio���ŷ���,��0Ϊ���룬1Ϊ���
*
* �޸ļ�¼  : 2012��11��27��
*****************************************************************************/
s32 bsp_gpio_direction_get(u32 gpio)
{
    u32 chip_num;
    u32 pin;
    u32 u32GpioAddr = 0;
    u32 gpio_value = 0;

    /* ���GPIO û�г�ʼ�������ش��� */
    if (GPIO_DEF_NOT_RUNNING == g_u32GpioRunning)
    {
        gpio_print_error(" GPIO is not inited.\n");
        return GPIO_ERROR;
    }

    if(gpio >= GPIO_TOTAL_PINS_NUM)
    {
        gpio_print_error(" GPIO parameter error.\n");
        return GPIO_ERROR;
    }
    chip_num   = gpio / GPIO_MAX_PINS;
    pin        = gpio % GPIO_MAX_PINS;

    u32GpioAddr = s_u32GpioBaseAddr[chip_num];

    gpio_value = readl(u32GpioAddr + HI_GPIO_SWPORT_DDR_OFFSET);

    return (s32)((gpio_value & ((u32)0x1 << pin)) >> pin);
}

/*****************************************************************************
* �� �� ��  : bsp_gpio_set_value
*
* ��������  : ����GPIO ���ŵĵ�ƽֵ
*
* �������  : u32 gpio        GPIO���ű��
*              u32 value      �ܽŸ�/�͵�ƽ
*
* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2012��11��27��
*****************************************************************************/
void bsp_gpio_set_value(u32 gpio,u32 value)
{
    u32 chip_num;
    u32 pin;
    u32 u32GpioAddr = 0;
    u32 gpio_value = 0;
    int old_level;

    /* ���GPIO û�г�ʼ�������ش��� */
    if (GPIO_DEF_NOT_RUNNING == g_u32GpioRunning)
    {
        gpio_print_error(" GPIO is not inited.\n");
        return ;
    }

    if(gpio >= GPIO_TOTAL_PINS_NUM)
    {
        gpio_print_error(" GPIO parameter error.\n");
        return;
    }

    /*���GPIO�����Ƿ���������Ϊ���룬�򷵻ش���*/
    if(0 == bsp_gpio_direction_get(gpio))
    {
        gpio_print_error(" GPIO direction error.\n");
        return;
    }

    chip_num   = gpio / GPIO_MAX_PINS;
    pin        = gpio % GPIO_MAX_PINS;

    u32GpioAddr = s_u32GpioBaseAddr[chip_num];


    old_level = intLock();
    bsp_ipc_spin_lock(IPC_SEM_GPIO);

    gpio_value = readl(u32GpioAddr + HI_GPIO_SWPORT_DR_OFFSET);
    if(!!value)
    {
        writel(gpio_value | ((u32)0x1 << pin), u32GpioAddr + HI_GPIO_SWPORT_DR_OFFSET);
    }
    else
    {
        writel(gpio_value & ~((u32)0x1 << pin), u32GpioAddr + HI_GPIO_SWPORT_DR_OFFSET);
    }

    bsp_ipc_spin_unlock(IPC_SEM_GPIO);
    intUnlock (old_level);

}

/*****************************************************************************
* �� �� ��  : bsp_gpio_get_value
*
* ��������  : ��ѯGPIO���ŵĵ�ƽֵ
*
* �������  : UINT32 gpio        GPIO���ű��
*
* �� �� ֵ  : ����GPIO���ŵĵ�ƽֵ
*
* �޸ļ�¼  : 2012��11��27��
*****************************************************************************/
s32 bsp_gpio_get_value(u32 gpio)
{
    u32 chip_num;
    u32 pin;
    u32 u32GpioAddr = 0;
    u32 gpio_value = 0;

    /* ���GPIO û�г�ʼ�������ش��� */
    if (GPIO_DEF_NOT_RUNNING == g_u32GpioRunning)
    {
        gpio_print_error(" GPIO is not inited.\n");
        return GPIO_ERROR;
    }

    if(gpio >= GPIO_TOTAL_PINS_NUM)
    {
        gpio_print_error(" GPIO parameter error.\n");
        return GPIO_ERROR;
    }

    /*���GPIO�����Ƿ����룬���Ϊ������򷵻ش���*/
    if(1 == bsp_gpio_direction_get(gpio))
    {
        gpio_print_error(" GPIO direction error.\n");
        return GPIO_ERROR;
    }

    chip_num   = gpio / GPIO_MAX_PINS;
    pin        = gpio % GPIO_MAX_PINS;

    u32GpioAddr = s_u32GpioBaseAddr[chip_num];

    /* ��ȡ���ŵĵ�ƽֵ */
    gpio_value = readl(u32GpioAddr + HI_GPIO_EXT_PORT_OFFSET);

    return (s32)((gpio_value & ((u32)0x1 << pin)) >> pin);

}

#else

// static u32 gpio_total[] = {GPIO_123, GPIO_124, GPIO_158, GPIO_159, GPIO_178, GPIO_179, 0xffffffff};

s32 bsp_gpio_init(void)
{
    /* ����ѡ��NV��ʼ�� */
    if(0 != bsp_rf_switch_init())
    {
        gpio_print_error("bsp_rf_switch_init is fail.\n");
    }

    /* RF LDO NV��ʼ�� */
    if(0 != bsp_ldo_gpio_init())
    {
        gpio_print_error("bsp_rf_switch_init is fail.\n");
        return -1;
    }
    
    return 0;
}

/*
 * Function name:get_gpio_group.
 * Discription:get GPIO's group. There are 26(0~25) groups GPIO in K3V2 solution,
 * use this function, you can know which group is your GPIO is belonged to.
 * Parameters:
 *      @ gpio_id:GPIO pin's number which is defined in gpio.h
 * return value:
 *      @ gpio group that your GPIO is belonged to.
 */
static unsigned int get_gpio_group(unsigned int gpio_id)
{
	unsigned int gpio_group = (gpio_id >> GPIO_GROUP_BIT);
	//lint --e{716}
	//lint --e{722}

	return gpio_group;
}

/*
 * Function name:get_gpio_group.
 * Discription:Get GPIO number in a group. There are 8(0~7) GPIO pins in one group
 * in K3V2 solution, use this function, you can know your GPIO' number.
 * Parameters:
 *      @ gpio_id:GPIO pin's number which is defined in gpio.h
 * return value:
 *      @ gpio number that your GPIO in a group.
 */
static unsigned int get_gpio_number(unsigned int gpio_id)
{
	unsigned int gpio_group = get_gpio_group(gpio_id);
	unsigned int gpio_number = gpio_id - (gpio_group << GPIO_GROUP_BIT);
	//lint --e{716}
	//lint --e{722}

	return gpio_number;
}


/*
 * Function name:get_gpio_base_addr.
 * Discription:Get GPIO base address. There are 26(0~25) GPIO groups
 * in K3V2 solution, each group corresponde to each different base address,
 * use this function, you can know your GPIO' base address.
 * Parameters:
 *      @ gpio_id:GPIO pin's number which is defined in gpio.h
 * return value:
 *      @ gpio address of your GPIO.
 */
static unsigned int get_gpio_base_addr(unsigned int gpio_id)
{
	unsigned int delta_addr = HI_GPIO0_REG_SIZE;
	unsigned int gpio_group = get_gpio_group(gpio_id);
	unsigned int gpio_base_addr;

	if (gpio_group < 22)
		gpio_base_addr = (HI_GPIO0_REGBASE_ADDR + gpio_group * delta_addr);
	else
		gpio_base_addr = (HI_GPIO22_REGBASE_ADDR + (gpio_group - 22) * delta_addr);

	return gpio_base_addr;
}

void bsp_gpio_direction_input(u32 gpio)
{
	unsigned int gpio_base_addr, gpio_number, reg_value_old, reg_value_new;

	gpio_number = get_gpio_number(gpio);
	if (gpio_number >= GPIO_MAX_NUMBER) {
		return;
	}

	gpio_base_addr = get_gpio_base_addr(gpio);

	reg_value_old  = readl(gpio_base_addr + GPIODIR);
	reg_value_new  = (~(0x1UL << gpio_number)) & reg_value_old;

	writel(reg_value_new, gpio_base_addr + GPIODIR);
}


void bsp_gpio_set_value(u32 gpio,u32 value)
{
    u32 gpio_base_addr = 0;
    u32 gpio_number = 0;
    u32 gpio_data_reg = 0;

    if(value > 1 || (gpio > GPIO_TOTAL))
    {
        gpio_print_error("para is error, gpio = %d, value = %d.\n", gpio, value);
        return ;
    }

    gpio_base_addr = get_gpio_base_addr(gpio);
	gpio_number = get_gpio_number(gpio);
	gpio_data_reg = ((unsigned int)0x1 << (gpio_number + 2));

	writel(value << gpio_number, gpio_base_addr + gpio_data_reg);

}

s32 bsp_gpio_direction_get(u32 gpio)
{
	unsigned int gpio_base_addr, gpio_number;

    if(gpio > GPIO_TOTAL)
    {
        gpio_print_error("para is error, gpio = %d.\n", gpio);
        return -1;
    }

	gpio_number = get_gpio_number(gpio);
	if (gpio_number >= GPIO_MAX_NUMBER)
    {
        gpio_print_error("para is error, gpio = %d.\n", gpio);
		return -1;
	}

	gpio_base_addr = get_gpio_base_addr(gpio);
	unsigned int reg_value  = readl(gpio_base_addr + GPIODIR);
	return (reg_value >> gpio_number) & 0x01;

}


s32 bsp_gpio_get_value(u32 gpio)
{
    s32 value = 0;

    u32 gpio_base_addr = 0;
    u32 gpio_number = 0;
    u32 gpio_data_reg = 0;

    if(gpio > GPIO_TOTAL)
    {
        gpio_print_error("para is error, gpio = %d.\n", gpio);
        return -1;
    }

	gpio_base_addr = get_gpio_base_addr(gpio);
	gpio_number = get_gpio_number(gpio);
	gpio_data_reg = ((unsigned int)0x1 << (gpio_number + 2));

	value = (s32)(readl(gpio_base_addr + gpio_data_reg) >> gpio_number);
	return value;

}

void bsp_gpio_direction_output(u32 gpio, u32 value)
{
	unsigned int gpio_base_addr = 0;
	unsigned int gpio_number = 0;
	unsigned int reg_value_old = 0;
	unsigned int reg_value_new = 0;

    if(value > 1 || gpio > GPIO_TOTAL)
    {
        gpio_print_error("para is error, gpio = %d, value = %d.\n", gpio, value);
        return ;
    }

	gpio_base_addr = get_gpio_base_addr(gpio);
	gpio_number = get_gpio_number(gpio);

	reg_value_old  = readl(gpio_base_addr + GPIODIR);

	reg_value_new  = (((unsigned int)0x1 << gpio_number)) | (reg_value_old);

	writel(reg_value_new, gpio_base_addr + GPIODIR);

    bsp_gpio_set_value( gpio, value);

}



#endif

#ifdef __cplusplus
}
#endif

