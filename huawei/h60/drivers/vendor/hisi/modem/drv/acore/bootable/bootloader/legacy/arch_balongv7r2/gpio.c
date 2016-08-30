/*************************************************************************
*   ��Ȩ����(C) 1987-2004, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  gpio.c
*
*   ��    �� :  liuwenhui
*
*   ��    �� :  GPIO�����ļ�
*
*   �޸ļ�¼ :  2012��11��27��
*
*************************************************************************/
/* includes */
#include <hi_gpio.h>
#include "soc_memmap.h"
#include "bsp_memmap.h"

#include "gpio.h"
#include "boot/boot.h"
#include "types.h"

/* GPIO���ܽ������͹ܽ���*/
/*lint -save -e760*/
#ifdef HI_GPIO4_REGBASE_ADDR
#define GPIO_MAX_BANK_NUM      (6)
#else
#define GPIO_MAX_BANK_NUM      (4)
#endif

#define GPIO_MAX_PINS          (32)
/*lint -restore */

/*ÿ��GPIO��ַ*/
unsigned int s_u32GpioBaseAddr[GPIO_MAX_BANK_NUM]=
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

#ifndef HI_GPIO4_REGBASE_ADDR
typedef int (*gpio_clk_opt)(void);

typedef struct 
{
    gpio_clk_opt get_status;
    gpio_clk_opt enable_clk;
    gpio_clk_opt disable_clk;
}gpio_clk_stru;

int gpio1_clk_get_status(void)
{
    unsigned int tmp = 0;
    tmp = readl(HI_SYSCRG_BASE_ADDR_VIRT + 0x8);
    return tmp & (0x1 << 28);
}

int gpio1_clk_enable(void)
{
    unsigned int tmp = 0;
    tmp = readl(HI_SYSCRG_BASE_ADDR_VIRT + 0x0);
    tmp |= 0x1 << 28;
    writel(tmp, HI_SYSCRG_BASE_ADDR_VIRT + 0x0);

    return GPIO_OK;
}

int gpio1_clk_disable(void)
{
    unsigned int tmp = 0;
    tmp = readl(HI_SYSCRG_BASE_ADDR_VIRT + 0x4);
    tmp |= 0x1 << 28;
    writel(tmp, HI_SYSCRG_BASE_ADDR_VIRT + 0x4);

    return GPIO_OK;
}

int gpio2_clk_get_status(void)
{
    unsigned int tmp = 0;
    tmp = readl(HI_SYSCRG_BASE_ADDR_VIRT + 0x14);
    return tmp & (0x1 << 2);
}

int gpio2_clk_enable(void)
{
    unsigned int tmp = 0;
    tmp = readl(HI_SYSCRG_BASE_ADDR_VIRT + 0xc);
    tmp |= 0x1 << 2;
    writel(tmp, HI_SYSCRG_BASE_ADDR_VIRT + 0xc);

    return GPIO_OK;
}

int gpio2_clk_disable(void)
{
    unsigned int tmp = 0;
    tmp = readl(HI_SYSCRG_BASE_ADDR_VIRT + 0x10);
    tmp |= 0x1 << 2;
    writel(tmp, HI_SYSCRG_BASE_ADDR_VIRT + 0x10);

    return GPIO_OK;
}

int gpio3_clk_get_status(void)
{
    unsigned int tmp = 0;
    tmp = readl(HI_SYSCRG_BASE_ADDR_VIRT + 0x8);
    return tmp & (0x1 << 30);
}

int gpio3_clk_enable(void)
{
    unsigned int tmp = 0;
    tmp = readl(HI_SYSCRG_BASE_ADDR_VIRT + 0x0);
    tmp |= 0x1 << 30;
    writel(tmp, HI_SYSCRG_BASE_ADDR_VIRT + 0x0);

    return GPIO_OK;
}

int gpio3_clk_disable(void)
{
    unsigned int tmp = 0;
    tmp = readl(HI_SYSCRG_BASE_ADDR_VIRT + 0x4);
    tmp |= 0x1 << 30;
    writel(tmp, HI_SYSCRG_BASE_ADDR_VIRT + 0x4);

    return GPIO_OK;

}

static gpio_clk_stru clk_opt[GPIO_MAX_BANK_NUM] =
{
    [0] = {
        .get_status  = NULL,
        .enable_clk  = NULL,
        .disable_clk = NULL,
    },
    [1] = {
        .get_status  = gpio1_clk_get_status,
        .enable_clk  = gpio1_clk_enable,
        .disable_clk = gpio1_clk_disable,
    },
    [2] = {
        .get_status  = gpio2_clk_get_status,
        .enable_clk  = gpio2_clk_enable,
        .disable_clk = gpio2_clk_disable,
    },
    [3] = {
        .get_status  = gpio3_clk_get_status,
        .enable_clk  = gpio3_clk_enable,
        .disable_clk = gpio3_clk_disable,
    }
};

int gpio_clk_enable(unsigned num)
{
    int ret = 0;

    if(num >= GPIO_MAX_BANK_NUM)
    {
        cprintf("gpio%d num is error.\n", num);
        return GPIO_ERROR;
    }
    
    if(0 == num)
    {
        return GPIO_OK;
    }
    
    /*���ʱ���Ѵ򿪣���ֱ�ӷ���*/
    ret = (*(clk_opt[num].get_status))();
    if(1 == ret)
    {
        cprintf("gpio%d clk has been opened.\n", num);
        return GPIO_OK;
    }

    /*��ʱ��*/
    ret = (*(clk_opt[num].enable_clk))();
    if(GPIO_ERROR == ret)
    {
        cprintf("gpio%d clk enable fail.\n", num);
        return GPIO_ERROR;
    }
    
    /*����Ƿ�򿪳ɹ�*/
    ret = (*(clk_opt[num].get_status))();
    if(0 == ret)
    {
        cprintf("gpio%d clk has not been opened.\n", num);
        return GPIO_ERROR;
    }

    return GPIO_OK;

}


int gpio_clk_disable(int num)
{
    int ret = 0;

    if(num >= GPIO_MAX_BANK_NUM)
    {
        cprintf("gpio%d num is error.\n", num);
        return GPIO_ERROR;
    }
    
    if(0 == num)
    {
        return GPIO_OK;
    }
    
    /*���ʱ���ѹرգ���ֱ�ӷ���*/
    ret = (*(clk_opt[num].get_status))();
    if(0 == ret)
    {
        cprintf("gpio%d clk has been closed.\n", num);
        return GPIO_OK;
    }

    /*��ʱ��*/
    ret = (*(clk_opt[num].disable_clk))();
    if(GPIO_ERROR == ret)
    {
        cprintf("gpio%d clk disable fail.\n", num);
        return GPIO_ERROR;
    }
    
    /*����Ƿ�رճɹ�*/
    ret = (*(clk_opt[num].get_status))();
    if(1 == ret)
    {
        cprintf("gpio%d clk has not been closed.\n", num);
        return GPIO_ERROR;
    }

    return GPIO_OK;

}

#else

int gpio_clk_enable(int num)
{
    return GPIO_OK;
}


int gpio_clk_disable(int num)
{
    return GPIO_OK;
}

#endif

/*lint -save -e550*/
/*****************************************************************************
* �� �� ��  : gpio_direction_input
*
* ��������  : ����GPIO���ŷ���Ϊ����
*
* �������  : unsigned gpio    GPIO���ű��
*
* �� �� ֵ  : void
*
* �޸ļ�¼  : 2012��11��27��   
*****************************************************************************/
int gpio_direction_input(unsigned gpio)
{
	unsigned chip_num = 0;
	unsigned pin = 0;

    unsigned addr = 0;
    unsigned value = 0;
	
    if(gpio > (GPIO_MAX_BANK_NUM * GPIO_MAX_PINS - 1))
    {
    	cprintf("gpio_direction_input: gpio parameter error, lines:%d\r\n ",__LINE__);
        return GPIO_ERROR;
    }
    
    chip_num   = gpio / GPIO_MAX_PINS;
    pin        = gpio % GPIO_MAX_PINS;	

    addr = s_u32GpioBaseAddr[chip_num] + HI_GPIO_SWPORT_DDR_OFFSET;

    if(GPIO_ERROR == gpio_clk_enable(chip_num))
    {
        cprintf("gpio_clk_enable is fail.\n");
        return GPIO_ERROR;
    }
    
    value = readl(addr) & ~((unsigned)0x1 << pin);
    writel(value, addr);
    
    gpio_clk_disable(chip_num);
    
    return GPIO_OK;
    
}


/*****************************************************************************
* �� �� ��  : gpio_direction_output
*
* ��������  : ����GPIO���ŵķ���Ϊ����������ø�����Ĭ�ϵ�ƽֵ
*
* �������  : unsigned gpio       GPIO���ű��
*             int      value      �����õ�ƽֵ,��0Ϊ�͵�ƽ��1Ϊ�ߵ�ƽ
*
* �� �� ֵ  : void
*
* �޸ļ�¼  : 2012��11��27��   
*****************************************************************************/
int gpio_direction_output(unsigned gpio, int value)
{
	unsigned chip_num;
	unsigned pin;
	unsigned u32GpioAddr = 0;
	unsigned gpio_value = 0;
    if(gpio > (GPIO_MAX_BANK_NUM*GPIO_MAX_PINS - 1))
    {
     	cprintf("gpio_direction_output: gpio parameter error, lines:%d",__LINE__);
        return GPIO_ERROR;
    }
    chip_num   = gpio / GPIO_MAX_PINS;
    pin        = gpio % GPIO_MAX_PINS;

	u32GpioAddr = s_u32GpioBaseAddr[chip_num];

    /* ����GPIO����Ϊ��� */    
    if(GPIO_ERROR == gpio_clk_enable(chip_num))
    {
        cprintf("gpio_clk_enable is fail.\n");
        return GPIO_ERROR;
    }
    
	gpio_value = readl(u32GpioAddr + HI_GPIO_SWPORT_DDR_OFFSET);
    
	writel(gpio_value | (0x1 << pin), u32GpioAddr + HI_GPIO_SWPORT_DDR_OFFSET);
    
    gpio_clk_disable(chip_num);
    
    /* ����GPIOĬ��ֵ�ܽŵ�ƽ */
	gpio_set_value(gpio, value); 
    
    return GPIO_OK;
}


/*****************************************************************************
* �� �� ��  : gpio_direction_get
*
* ��������  : ��ѯGPIO ���ŵķ���
*
* �������  : unsigned gpio        GPIO���ű��
*
* �� �� ֵ  : ����gpio���ŷ���,��0Ϊ���룬1Ϊ���
*
* �޸ļ�¼  : 2012��11��27��   
*****************************************************************************/
int gpio_direction_get(unsigned gpio)
{/*lint !e14*/
	unsigned chip_num;
	unsigned pin;
	unsigned u32GpioAddr = 0;
	unsigned gpio_value = 0;
	
    if(gpio > (GPIO_MAX_BANK_NUM*GPIO_MAX_PINS - 1))
    {
    	cprintf("gpio_direction_get: gpio parameter error, lines:%d\r\n ",__LINE__);
        return GPIO_ERROR;
    }
    chip_num   = gpio / GPIO_MAX_PINS;
    pin        = gpio % GPIO_MAX_PINS;

	u32GpioAddr = s_u32GpioBaseAddr[chip_num];
    
    if(GPIO_ERROR == gpio_clk_enable(chip_num))
    {
        cprintf("gpio_clk_enable is fail.\n");
        return GPIO_ERROR;
    }

	gpio_value = readl(u32GpioAddr + HI_GPIO_SWPORT_DDR_OFFSET);
    
    gpio_clk_disable(chip_num);
    
	return (int)((gpio_value & ((unsigned)0x1 << pin)) >> pin);

}

/*****************************************************************************
* �� �� ��  : gpio_set_value
*
* ��������  : ����GPIO ���ŵĵ�ƽֵ
*
* �������  : unsigned gpio        GPIO���ű��
*             int      value       ���ŵ�ƽ
*
* �������  : ��
*
* �� �� ֵ  : void
*
* �޸ļ�¼  : 2012��11��27��   
*****************************************************************************/
void gpio_set_value(unsigned gpio, int value)
{/*lint !e18*/
	unsigned chip_num;
	unsigned pin;
	unsigned u32GpioAddr = 0;
	unsigned gpio_value = 0;
	
    if(gpio > (GPIO_MAX_BANK_NUM*GPIO_MAX_PINS - 1))
    {
	    cprintf("gpio_set_value: GPIO parameter error, lines:%d\r\n ",__LINE__);
        return;
    }

	/*���GPIO�����Ƿ���������Ϊ���룬�򷵻ش���*/
	if(0 == gpio_direction_get(gpio))
	{
		return;
	}
	
    chip_num   = gpio / GPIO_MAX_PINS;
    pin        = gpio % GPIO_MAX_PINS;

	u32GpioAddr = s_u32GpioBaseAddr[chip_num];

    /*����Ĭ��ֵΪ�͵�ƽ*/
    if(GPIO_ERROR == gpio_clk_enable(chip_num))
    {
        cprintf("gpio_clk_enable is fail.\n");
        return ;
    }

    gpio_value = readl(u32GpioAddr + HI_GPIO_SWPORT_DR_OFFSET);
	if(!!value)
	{
		writel(gpio_value | (0x1 << pin), u32GpioAddr + HI_GPIO_SWPORT_DR_OFFSET);	
	}
	else
	{
		writel(gpio_value & ~(0x1 << pin), u32GpioAddr + HI_GPIO_SWPORT_DR_OFFSET);
	}
    
    gpio_clk_disable(chip_num);
}


/*****************************************************************************
* �� �� ��  : gpio_get_value
*
* ��������  : ��ѯGPIO���ŵĵ�ƽֵ
*
* �������  : unsigned gpio        GPIO���ű��
*
* �� �� ֵ  : ����GPIO���ŵĵ�ƽֵ
*
* �޸ļ�¼  : 2012��11��27��
*****************************************************************************/
int gpio_get_value(unsigned gpio)
{
	unsigned chip_num;
	unsigned pin;
	unsigned u32GpioAddr = 0;
	unsigned gpio_value = 0;
	
    if(gpio > (GPIO_MAX_BANK_NUM*GPIO_MAX_PINS - 1))
    {
	    cprintf("gpio_get_value: GPIO parameter error, lines:%d",__LINE__);
        return GPIO_ERROR;
    }

	/*���GPIO�����Ƿ����룬���Ϊ������򷵻ش���*/
	if(1 == gpio_direction_get(gpio))
	{
		return GPIO_ERROR;
	}
	
    chip_num   = gpio / GPIO_MAX_PINS;
    pin        = gpio % GPIO_MAX_PINS;

	u32GpioAddr = s_u32GpioBaseAddr[chip_num];
	
    /* ��ȡ���ŵĵ�ƽֵ */
    if(GPIO_ERROR == gpio_clk_enable(chip_num))
    {
        cprintf("gpio_clk_enable is fail.\n");
        return GPIO_ERROR;
    }

    gpio_value = readl(u32GpioAddr + HI_GPIO_EXT_PORT_OFFSET);
    
    gpio_clk_disable(chip_num);
    
	return (int)((gpio_value & ((unsigned)0x1 << pin)) >> pin);

}
/*lint -restore */

