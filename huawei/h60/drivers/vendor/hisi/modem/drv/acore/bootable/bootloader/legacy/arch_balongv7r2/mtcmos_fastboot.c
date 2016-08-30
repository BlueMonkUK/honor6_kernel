/*************************************************************************
*   ��Ȩ����(C) 1987-2011, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� : mtcmos_fastboot.c
*
*   ��    �� :  yangqiang
*
*   ��    �� : ���ļ����ڹر�ϵͳʱ�Ӳ����еķ��ü���IPʱ��
*
*   �޸ļ�¼ :  2013��8��10��  v1.00 yangqiang����
*************************************************************************/

#include <hi_pwrctrl_interface.h>
#include <drv_nv_id.h>
#include <drv_nv_def.h>
#include <bsp_nvim.h>

#ifdef REGULATOR_V7R2_MTCMOS

void bsp_disable_mtcmos(void)
{
	unsigned int cmos_dis_flag =0xd,ret=0;
	DRV_NV_PM_CLKINIT_STRU  g_clk_init;
    ret = bsp_nvm_read(NV_ID_DRV_PM_CLKINIT, (u8 *)&g_clk_init, sizeof(g_clk_init));
	if(NV_OK!=ret)
	  	return;
       cmos_dis_flag = g_clk_init.mtcmosdis;

	//sd mtcmos
	if(cmos_dis_flag&0x1)
	{
		hi_pwrctrl_regu_sdmtcmos_dis();
	}

	//usb mtcmos
	if(cmos_dis_flag&0x2)
	{
	 	hi_pwrctrl_regu_usbmtcmos_dis();
	}
	//hsic mtcmos
	if(cmos_dis_flag&0x4)
	{
		hi_pwrctrl_regu_hiscmtcmos_dis();
	}
	//hifi mtcmos
	if(cmos_dis_flag&0x8)
	{
		hi_pwrctrl_regu_hifimtcmos_dis();
	}
	//irm mtcmos
	if(cmos_dis_flag&0x10)
	{
		hi_pwrctrl_regu_irmmtcmos_dis();
	}
	//g2bbp mtcmos
	if(cmos_dis_flag&0x20)
	{
		hi_pwrctrl_regu_g2bbpmtcmos_dis();
	}
	//g1bbp mtcmos
	if(cmos_dis_flag&0x40)
	{
		hi_pwrctrl_regu_g1bbpmtcmos_dis();
	}
	//wbbp mtcmos
	if(cmos_dis_flag&0x80)
	{
		hi_pwrctrl_regu_wbbpmtcmos_dis();
	}
	//twbbp mtcmos
	if(cmos_dis_flag&0x100)
	{
		hi_pwrctrl_regu_twbbpmtcmos_dis();
	}

	//lbbp mtcmos
	if(cmos_dis_flag&0x200)
	{
		hi_pwrctrl_regu_lbbpmtcmos_dis();
	}

	//dsp0 mtcmos
	if(cmos_dis_flag&0x400)
	{
		hi_pwrctrl_regu_dsp0mtcmos_dis();
	}
	return;
}

#else

void bsp_disable_mtcmos(void)
{
	return;
}

#endif