


#include "product_config.h"
#include "osl_types.h"
#include "osl_bio.h"
#include "osl_module.h"
#include "drv_version.h"
#include "bsp_memmap.h"

void p531_fpga_test_init()
{
	u32 reg;
	u32 *mem = (u32 *)0x20680000;
	u32 size = 6145;
	memset(mem, 0xaa, size);
	while(size -->0)
	{
		reg = readl((unsigned)(mem++));
		if(0xaaaaaaaa != reg)
			{
				logMsg("write failed!!!\n", 0, 0, 0, 0, 0, 0);
				break;
			}
	}
	logMsg("fpgs init test successed\n", 0, 0, 0, 0, 0, 0);
}

void p531_fpga_multi_mode_init()
{
	/* GUģ��Ҫ�������� */
	if(bsp_board_type_is_gutl())
	{
		#ifdef BSP_CONFIG_P531_FPGA
		/* GU ģ�⸴λ*/	
		writel(0xffffffff, 0x27050008);    
		writel(0xffffffff, 0x27050018);  
		#endif
		/* LBUS���� */
		writel(0x10, 0x220a2024);
	}
}

void k3v3_fpga_init()
{
    u8 iregVal;

	if(bsp_board_type_is_gutl())
	{
		/* LBUS���� */
		writel(0x10, 0xe1fa0024);

        /*hi6551����*/
        /*����ldo24Ϊ3v*/
        writel(0x04, (0xe1f96000+(0xb4<<2)));
        /*��ldo24*/
        writel(0x20, (0xe1f96000+(0x36<<2)));
        /*auxdac �����ܼĴ�������*/
        iregVal = (u8)readl(0xe1f96000+(0x11c<<2));
        iregVal |=  0x01;
        writel((u32)iregVal, 0xe1f96000+(0x11c<<2));
	}
}

void board_fpga_init(void)
{
#if defined (BSP_CONFIG_P531_FPGA) || defined(BSP_CONFIG_V7R2_SFT)
    p531_fpga_multi_mode_init();
#endif

#ifdef BSP_CONFIG_K3V3_SFT
    k3v3_fpga_init();
#endif

    return;
}

