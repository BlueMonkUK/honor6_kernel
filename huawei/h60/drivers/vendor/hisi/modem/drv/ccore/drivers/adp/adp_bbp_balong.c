
/*lint --e{537}*/
#include "drv_comm.h"
#include "bsp_bbp.h"
#include "bsp_version.h"
#include "bsp_memmap.h"
#include "osl_bio.h"
#include "product_config.h"
#define BBP_CTU_MEAS_FLAG_L_RD  0x640
#define BBP_CTU_MEAS_FLAG_W_RD  0x644
#define BBP_CTU_MEAS_FLAG_T_RD  0x648
#define BBP_CTU_MEAS_FLAG_GM_RD 0x64c
#define BBP_CTU_MEAS_FLAG_GS_RD 0x650

#ifdef CONFIG_BBP_INT
/*****************************************************************************
* ����  : BSP_BBPIntTimerRegCb
* ����  : ��PS���ã����������ע��1ms��ʱ�жϵĻص�
* ����  : pfunc
* ���  : void
* ����  : void
*****************************************************************************/
BSP_VOID BSP_BBPIntTimerRegCb(BSPBBPIntTimerFunc pFunc)
{
    bsp_bbp_timerintregcb(pFunc);
}

/*****************************************************************************
* ����  : bsp_bbp_dltbintregcb
* ����  : ��PS���ã����������ע������������������жϵĻص�
* ����  : pfunc
* ���  : void
* ����  : void
*****************************************************************************/
BSP_VOID BSP_BBPIntDlTbRegCb(BSPBBPIntDlTbFunc pFunc)
{
    bsp_bbp_dltbintregcb(pFunc);
}

/*****************************************************************************
* �� ��: BSP_BBPIntTimerClear
* �� ��: ��PS���ã��������1ms��ʱ�ж�
* ���� : void
* ��� : void
* �� ��: void
*****************************************************************************/
BSP_VOID BSP_BBPIntTimerClear(void)
{
    bsp_bbp_timerintclear();
}

/*****************************************************************************
* �� ��: BSP_BBPIntTimerEnable
* �� ��: ��PS���ã�������1ms��ʱ�ж�
* ���� : void
* ��� : void
* ���� : int
*****************************************************************************/
BSP_S32 BSP_BBPIntTimerEnable(void)
{
    return bsp_bbp_timerintenable();
}

/*****************************************************************************
* �� ��: BSP_BBPIntTimerDisable
* �� ��: ��PS���ã������ر�1ms��ʱ�ж�
* ���� : void
* ��� : void
* �� ��: void
*****************************************************************************/
BSP_VOID BSP_BBPIntTimerDisable(void)
{
    bsp_bbp_timerintdisable();
}

/*****************************************************************************
* ����  : BSP_BBPGetCurTime
* ����  : ��PS���ã�������ȡϵͳ��ȷʱ��
* ����  : void
* ���  : u64 *pcurtime
* ����  : u32
*****************************************************************************/
BSP_U32 BSP_BBPGetCurTime(BSP_U64 *pCurTime)
{
    return bsp_bbp_getcurtime(pCurTime);
}

/*****************************************************************************
* ����  : BSP_BBPGetCurTime
* ����  : ��ȡBBP��ʱ����ֵ������OAM ʱ��
* ����  : void
* ���  :
			pulLow32bitValueָ���������Ϊ�գ�����᷵��ʧ�ܡ�
			pulHigh32bitValue���Ϊ�գ� ��ֻ���ص�32bit��ֵ��
* ����  : int
*****************************************************************************/
int DRV_GET_BBP_TIMER_VALUE(unsigned int  *pulHigh32bitValue,  unsigned int  *pulLow32bitValue)
{
	BSP_U64 CurTime;

	if(BSP_NULL == pulLow32bitValue)
	{
		return -1;
	}

	bsp_bbp_getcurtime(&CurTime);

	if(BSP_NULL != pulHigh32bitValue)
	{
		*pulHigh32bitValue = (CurTime>>32);
	}

	*pulLow32bitValue = CurTime & 0xffffffff;

	return 0;
}
/*****************************************************************************
* ����  : BSP_GetSysFrame
* ����  : get system frame num
* ����  : void
* ���  : void
* ����  : u32
*****************************************************************************/
BSP_U16 BSP_GetSysFrame(BSP_VOID)
{
    return bsp_bbp_getsysframe();
}

/*****************************************************************************
* ����  : BSP_GetSysSubFrame
* ����  : get system sub frame num
* ����  : void
* ���  : void
* ����  : u32
*****************************************************************************/
BSP_U16 BSP_GetSysSubFrame(BSP_VOID)
{
    return bsp_bbp_getsyssubframe();
}

/*****************************************************************************
* ����  : BSP_GetTdsSubFrame
* ����  : get tds system sub frame num
* ����  : void
* ���  : void
* ����  : u32
*****************************************************************************/
BSP_U16 BSP_GetTdsSubFrame(BSP_VOID)
{
	return bsp_bbp_get_tds_subframe();
}
/*****************************************************************************
* ����  : BSP_SetTdsSubFrameOffset
* ����  : set tds system sub frame offset
* ����  : void
* ���  : void
* ����  : u32
*****************************************************************************/
BSP_U16 BSP_SetTdsSubFrameOffset(BSP_U16 usOffset)
{
	return bsp_bbp_set_tds_subframoffset(usOffset);
}
/*****************************************************************************
* ����  : BSP_SetTdsSubFrameOffset
* ����  : set tds system sub frame offset
* ����  : void
* ���  : void
* ����  : u32
*****************************************************************************/
BSP_U32 BSP_GetTdsSleepTime(BSP_VOID)
{
	return bsp_bbp_get_tds_sleep_time();
}

/*****************************************************************************
* ����  : BSP_BBPIntTdsTFRegCb
* ����  : regist callback
* ����  : void
* ���  : void
* ����  : void
*****************************************************************************/

BSP_VOID BSP_BBPIntTdsTFRegCb(BSPBBPIntTdsFunc pFunc)
{
    bsp_bbp_tdstf_regcb(pFunc);
}

/*****************************************************************************
* ����  : BSP_BBPIntTdsTFEnable
* ����  : enable tdstf
* ����  : void
* ���  : viod
* ����  : u32
*****************************************************************************/

BSP_S32 BSP_BBPIntTdsTFEnable(BSP_VOID)
{
    return bsp_bbp_tdstf_enable();
}

/*****************************************************************************
* ����  : BSP_BBPIntTdsTFDisable
* ����  : disable tdstf
* ����  : void
* ���  : void
* ����  : void
*****************************************************************************/

BSP_VOID BSP_BBPIntTdsTFDisable(BSP_VOID)
{
    bsp_bbp_tdstf_disable();
}
/*****************************************************************************
* ����  : BSP_GetTdsClkSwitchStatus
* ����  : get tdsclk switch
* ����  : void
* ���  : 0��ʾ�л���ϵͳʱ�ӣ�1��ʾ�л���32kʱ��
* ����  : void
*****************************************************************************/
BSP_S32 BSP_GetTdsClkSwitchStatus(BSP_VOID)
{
	return bsp_bbp_get_tdsclk_switch();
}

/*****************************************************************************
* ����  : DRV_BBPWAKE_INT_ENABLE
* ����  : enable bbp wakeup int
* ����  : void
* ���  : void
* ����  : void
*****************************************************************************/
BSP_VOID DRV_BBPWAKE_INT_ENABLE(PWC_COMM_MODE_E mode)
{
    bbp_wakeup_int_enable(mode);
    return;
}

/*****************************************************************************
* ����  : DRV_BBPWAKE_INT_DISABLE
* ����  : disable bbp wakeup int
* ����  : void
* ���  : void
* ����  : void
*****************************************************************************/
BSP_VOID DRV_BBPWAKE_INT_DISABLE(PWC_COMM_MODE_E mode)
{
    bbp_wakeup_int_disable(mode);
    return;
}

/*****************************************************************************
* ����  : DRV_BBPWAKE_INT_CLEAR
* ����  : clear bbp wakeup int
* ����  : void
* ���  : void
* ����  : void
*****************************************************************************/
BSP_VOID DRV_BBPWAKE_INT_CLEAR(PWC_COMM_MODE_E mode)
{
    bbp_wakeup_int_clear(mode);
    return;
}

/*****************************************************************************
Function:   BSP_PWC_GetMeansFlag
Description:
Input:
Output:     the means flag value;
Return:
Others:
*****************************************************************************/
 BSP_S32 BSP_PWC_GetMeansFlag(PWC_COMM_MODE_E enCommMode)
 {

	u32 pwc_meas_offset=0;
	u32 meas_value=0xffffffff;

	switch(enCommMode){

	case PWC_COMM_MODE_WCDMA:
		pwc_meas_offset = BBP_CTU_MEAS_FLAG_W_RD;
		break;
	case PWC_COMM_MODE_GSM:
		pwc_meas_offset = BBP_CTU_MEAS_FLAG_GM_RD;
		break;
	case PWC_COMM_MODE_LTE:
		pwc_meas_offset = BBP_CTU_MEAS_FLAG_L_RD;
		break;
	case PWC_COMM_MODE_TDS:
		pwc_meas_offset = BBP_CTU_MEAS_FLAG_T_RD;
		break;
	default:
		return -1;
	}

	meas_value = readl(HI_CTU_BASE_ADDR+pwc_meas_offset);
	return (BSP_S32)meas_value;
 }
/*****************************************************************************
* �� �� : BSP_LPS_Get_Ta
* �� �� : ��lps����,�鿴������֡ͷ���ϵͳ��֡ͷ����ǰ��
* �� �� : void
* �� �� : void
* �� �� :
* ˵ �� :
*****************************************************************************/
BSP_U16 BSP_LPS_GetTa(void)
{
	return bsp_bbp_lps_get_ta();
}

#else
BSP_VOID BSP_BBPIntTimerRegCb(BSPBBPIntTimerFunc pFunc)
{
    return ;
}

/*****************************************************************************
* ����  : bsp_bbp_dltbintregcb
* ����  : ��PS���ã����������ע������������������жϵĻص�
* ����  : pfunc
* ���  : void
* ����  : void
*****************************************************************************/
BSP_VOID BSP_BBPIntDlTbRegCb(BSPBBPIntDlTbFunc pFunc)
{
    return ;
}

/*****************************************************************************
* �� ��: BSP_BBPIntTimerClear
* �� ��: ��PS���ã��������1ms��ʱ�ж�
* ���� : void
* ��� : void
* �� ��: void
*****************************************************************************/
BSP_VOID BSP_BBPIntTimerClear(void)
{
    return ;
}

/*****************************************************************************
* �� ��: BSP_BBPIntTimerEnable
* �� ��: ��PS���ã�������1ms��ʱ�ж�
* ���� : void
* ��� : void
* ���� : int
*****************************************************************************/
BSP_S32 BSP_BBPIntTimerEnable(void)
{
    return (BSP_S32)-1;
}

/*****************************************************************************
* �� ��: BSP_BBPIntTimerDisable
* �� ��: ��PS���ã������ر�1ms��ʱ�ж�
* �� ��: void
* �� ��: void
* �� ��: void
*****************************************************************************/
BSP_VOID BSP_BBPIntTimerDisable(void)
{
    return ;
}

/*****************************************************************************
* ����  : BSP_BBPGetCurTime
* ����  : ��PS���ã�������ȡϵͳ��ȷʱ��
* ����  : void
* ���  : u64 *pcurtime
* ����  : u32
*****************************************************************************/
BSP_U32 BSP_BBPGetCurTime(BSP_U64 *pCurTime)
{
    return (BSP_U32)0;
}

/*****************************************************************************
* ����  : BSP_BBPGetCurTime
* ����  : ��ȡBBP��ʱ����ֵ������OAM ʱ��
* ����  : void
* ���  :
			pulLow32bitValueָ���������Ϊ�գ�����᷵��ʧ�ܡ�
			pulHigh32bitValue���Ϊ�գ� ��ֻ���ص�32bit��ֵ��
* ����  : int
*****************************************************************************/
int DRV_GET_BBP_TIMER_VALUE(unsigned int  *pulHigh32bitValue,  unsigned int  *pulLow32bitValue)
{
	return (int)0;
}
/*****************************************************************************
* ����  : BSP_GetSysFrame
* ����  : get system frame num
* ����  : void
* ���  : void
* ����  : u32
*****************************************************************************/
BSP_U16 BSP_GetSysFrame(BSP_VOID)
{
    return (BSP_U32)0;
}

/*****************************************************************************
* ����  : BSP_GetSysSubFrame
* ����  : get system sub frame num
* ����  : void
* ���  : void
* ����  : u32
*****************************************************************************/
BSP_U16 BSP_GetSysSubFrame(BSP_VOID)
{
    return (BSP_U32)0;
}

/*****************************************************************************
* ����  : BSP_GetTdsSubFrame
* ����  : get tds system sub frame num
* ����  : void
* ���  : void
* ����  : u32
*****************************************************************************/
BSP_U16 BSP_GetTdsSubFrame(BSP_VOID)
{
	return 0;
}
/*****************************************************************************
* ����  : BSP_SetTdsSubFrameOffset
* ����  : set tds system sub frame offset
* ����  : void
* ���  : void
* ����  : u32
*****************************************************************************/
BSP_U16 BSP_SetTdsSubFrameOffset(BSP_U16 usOffset)
{
	return 0;
}
/*****************************************************************************
* ����  : BSP_SetTdsSubFrameOffset
* ����  : set tds system sub frame offset
* ����  : void
* ���  : void
* ����  : u32
*****************************************************************************/
BSP_U32 BSP_GetTdsSleepTime(BSP_VOID)
{
	return 0;
}

/*****************************************************************************
* ����  : BSP_BBPIntTdsTFRegCb
* ����  : regist callback
* ����  : void
* ���  : void
* ����  : void
*****************************************************************************/

BSP_VOID BSP_BBPIntTdsTFRegCb(BSPBBPIntTdsFunc pFunc)
{
    return ;
}

/*****************************************************************************
* ����  : BSP_BBPIntTdsTFEnable
* ����  : enable tdstf
* ����  : void
* ���  : viod
* ����  : u32
*****************************************************************************/

BSP_S32 BSP_BBPIntTdsTFEnable(BSP_VOID)
{
    return (BSP_S32)0;
}

/*****************************************************************************
* ����  : BSP_BBPIntTdsTFDisable
* ����  : disable tdstf
* ����  : void
* ���  : void
* ����  : void
*****************************************************************************/

BSP_VOID BSP_BBPIntTdsTFDisable(BSP_VOID)
{
    return ;
}
/*****************************************************************************
* ����  : DRV_BBPWAKE_INT_ENABLE
* ����  : enable bbp wakeup int
* ����  : void
* ���  : void
* ����  : void
*****************************************************************************/
BSP_VOID DRV_BBPWAKE_INT_ENABLE(PWC_COMM_MODE_E mode)
{

    return ;
}

/*****************************************************************************
* ����  : DRV_BBPWAKE_INT_DISABLE
* ����  : disable bbp wakeup int
* ����  : void
* ���  : void
* ����  : void
*****************************************************************************/
BSP_VOID DRV_BBPWAKE_INT_DISABLE(PWC_COMM_MODE_E mode)
{
    return ;
}

/*****************************************************************************
* ����  : DRV_BBPWAKE_INT_CLEAR
* ����  : clear bbp wakeup int
* ����  : void
* ���  : void
* ����  : void
*****************************************************************************/
BSP_VOID DRV_BBPWAKE_INT_CLEAR(PWC_COMM_MODE_E mode)
{
    return ;
}

/*****************************************************************************
Function:   BSP_PWC_GetMeansFlag
Description:
Input:
Output:     the means flag value;
Return:
Others:
*****************************************************************************/
 BSP_S32 BSP_PWC_GetMeansFlag(PWC_COMM_MODE_E enCommMode)
 {
	return (BSP_S32)0;
 }

/*****************************************************************************
* �� �� : bsp_bbp_get_wakeup_time
* �� �� : ���͹��ĵ��ã�������ѯ˯�߳�ʱʱ��
* �� �� : void
* �� �� : void
* �� �� : bbp˯��ʣ��ʱ�䣬��λΪ32.768KHzʱ�Ӽ���
* ˵ �� : �͹��Ĳ������ĸ�ģ���ж೤ʱ�䱻�滽�ѣ�
		    ֻ��Ҫ������С��ʱ�伴��
*****************************************************************************/
u32 bsp_bbp_get_wakeup_time(void)
{
	return 0xffffffff;
}

/*****************************************************************************
* �� �� : bbp_pwrctrl_irm_poweron
* �� �� : ���͹��ĵ��ã�������bbp���������ϵ�
* �� �� : void
* �� �� : void
* �� �� : ģ��ü�ʱ����׮
* ˵ �� :
*****************************************************************************/
int bbp_pwrctrl_irm_poweron(void)
{
	return 0;
}

/*****************************************************************************
* �� �� : bbp_pwrctrl_irm_poweroff
* �� �� : ���͹��ĵ��ã�������bbp���������µ�
* �� �� : void
* �� �� : void
* �� �� : ģ��ü�ʱ����׮
* ˵ �� :
*****************************************************************************/
int bbp_pwrctrl_irm_poweroff()
{
	return 0;
}
PWC_COMM_STATUS_E bbp_pwrctrl_irm_status(void)
{
    return 0x10;
}

/*****************************************************************************
* �� �� : bbp_int_init
* �� �� : bbp ��ʼ��
* �� �� : void
* �� �� : void
* �� �� : ģ��ü�ʱ����׮
* ˵ �� :
*****************************************************************************/
s32 bbp_int_init(void)
{
	return 0;
}

/*****************************************************************************
* �� �� : bsp_bbp_timerintregcb
* �� �� : bbp ��ʼ��
* �� �� : void
* �� �� : void
* �� �� : ģ��ü�ʱ����׮
* ˵ �� :
*****************************************************************************/
void bsp_bbp_timerintregcb(BSPBBPIntTimerFunc pfunc)
{
	return ;
}

/*****************************************************************************
* �� �� : bsp_bbp_dltbintregcb
* �� �� : bbp ��ʼ��
* �� �� : void
* �� �� : void
* �� �� : ģ��ü�ʱ����׮
* ˵ �� :
*****************************************************************************/
void bsp_bbp_dltbintregcb(BSPBBPIntDlTbFunc pfunc)
{
	return ;
}

/*****************************************************************************
* �� �� : bsp_bbp_timerintclear
* �� �� : bbp ��ʼ��
* �� �� : void
* �� �� : void
* �� �� : ģ��ü�ʱ����׮
* ˵ �� :
*****************************************************************************/
void bsp_bbp_timerintclear(void)
{
	return ;
}
/*****************************************************************************
* �� �� : bsp_bbp_timerintenable
* �� �� : bbp ��ʼ��
* �� �� : void
* �� �� : void
* �� �� : ģ��ü�ʱ����׮
* ˵ �� :
*****************************************************************************/
int  bsp_bbp_timerintenable(void)
{
	return 0;
}
/*****************************************************************************
* �� �� : bsp_bbp_timerintdisable
* �� �� : bbp ��ʼ��
* �� �� : void
* �� �� : void
* �� �� : ģ��ü�ʱ����׮
* ˵ �� :
*****************************************************************************/
void bsp_bbp_timerintdisable(void)
{
	return ;
}

/*****************************************************************************
* �� �� : bsp_bbp_getcurtime
* �� �� : bbp ��ʼ��
* �� �� : void
* �� �� : void
* �� �� : ģ��ü�ʱ����׮
* ˵ �� :
*****************************************************************************/
u32  bsp_bbp_getcurtime(u64 *pcurtime)
{
	return 0;
}

/*****************************************************************************
* �� �� : bsp_bbp_getsysframe
* �� �� : bbp ��ʼ��
* �� �� : void
* �� �� : void
* �� �� : ģ��ü�ʱ����׮
* ˵ �� :
*****************************************************************************/
u16  bsp_bbp_getsysframe(void)
{
	return 0;
}
/*****************************************************************************
* �� �� : bsp_bbp_timerintdisable
* �� �� : bbp ��ʼ��
* �� �� : void
* �� �� : void
* �� �� : ģ��ü�ʱ����׮
* ˵ �� :
*****************************************************************************/
u16  bsp_bbp_getsyssubframe(void)
{
	return 0;
}

/*****************************************************************************
* �� �� : bsp_bbp_tdstf_regcb
* �� �� : bbp ��ʼ��
* �� �� : void
* �� �� : void
* �� �� : ģ��ü�ʱ����׮
* ˵ �� :
*****************************************************************************/
void bsp_bbp_tdstf_regcb(BSPBBPIntTdsFunc pFunc)
{
	return ;
}
/*****************************************************************************
* �� �� : bsp_bbp_tdstf_enable
* �� �� : bbp ��ʼ��
* �� �� : void
* �� �� : void
* �� �� : ģ��ü�ʱ����׮
* ˵ �� :
*****************************************************************************/
int  bsp_bbp_tdstf_enable()
{
	return ;
}

/*****************************************************************************
* �� �� : bsp_bbp_tdstf_disable
* �� �� : bbp ��ʼ��
* �� �� : void
* �� �� : void
* �� �� : ģ��ü�ʱ����׮
* ˵ �� :
*****************************************************************************/
void bsp_bbp_tdstf_disable()
{
	return ;
}
/*****************************************************************************
* ����  : BSP_GetTdsClkSwitchStatus
* ����  : get tdsclk switch
* ����  : void
* ���  : 0��ʾ�л���ϵͳʱ�ӣ�1��ʾ�л���32kʱ��
* ����  : void
*****************************************************************************/
BSP_S32 BSP_GetTdsClkSwitchStatus(BSP_VOID)
{
	return -1;
}

/*****************************************************************************
* �� �� : bsp_bbp_tdstf_disable
* �� �� : bbp ��ʼ��
* �� �� : void
* �� �� : void
* �� �� : ģ��ü�ʱ����׮
* ˵ �� :
*****************************************************************************/
void bbp_wakeup_int_enable(PWC_COMM_MODE_E mode)
{
	return ;
}
/*****************************************************************************
* �� �� : bbp_wakeup_int_disable
* �� �� : bbp ��ʼ��
* �� �� : void
* �� �� : void
* �� �� : ģ��ü�ʱ����׮
* ˵ �� :
*****************************************************************************/
void bbp_wakeup_int_disable(PWC_COMM_MODE_E mode)
{
	return ;
}
/*****************************************************************************
* �� �� : bbp_wakeup_int_clear
* �� �� : bbp ��ʼ��
* �� �� : void
* �� �� : void
* �� �� : ģ��ü�ʱ����׮
* ˵ �� :
*****************************************************************************/
void bbp_wakeup_int_clear(PWC_COMM_MODE_E mode)
{
	return ;
}
/*****************************************************************************
* �� �� : bsp_bbp_g1_poweron
* �� �� : ���͹��ĵ��ã�������G1 BBP�ϵ�
* �� �� : void
* �� �� : void
* �� �� : 0Ϊ�ɹ���-1ʧ��
* ˵ �� :
*****************************************************************************/
int bsp_bbp_pwrctrl_g1bbp_poweron(void)
{

	return BSP_OK;
}

/*****************************************************************************
* �� �� : bsp_bbp_g1_poweroff
* �� �� : ���͹��ĵ��ã�������G1 BBP�µ�
* �� �� : ģʽ
* �� �� : void
* �� �� : �µ��Ƿ�ɹ�
* ˵ �� :
*****************************************************************************/
int bsp_bbp_pwrctrl_g1bbp_poweroff(void)
{
	return BSP_OK;
}

/*****************************************************************************
* �� �� : bsp_bbp_pwrctrl_g1_status
* �� �� : ���͹��ĵ��ã�������G1 BBP�ϵ�
* �� �� : void
* �� �� : void
* �� �� : 0x10Ϊopen, 0x20Ϊclose
* ˵ �� :
*****************************************************************************/
PWC_COMM_STATUS_E bsp_bbp_pwrctrl_g1bbp_status(void)
{
    return PWC_COMM_STATUS_BUTT;
}

/*****************************************************************************
* �� �� : bsp_bbp_g2_poweron
* �� �� : ���͹��ĵ��ã�������G2 BBP�ϵ�
* �� �� : void
* �� �� : void
* �� �� : 0Ϊ�ɹ���-1ʧ��
* ˵ �� :
*****************************************************************************/
int bsp_bbp_pwrctrl_g2bbp_poweron(void)
{
	return BSP_OK;
}

/*****************************************************************************
* �� �� : bsp_bbp_g2_poweroff
* �� �� : ���͹��ĵ��ã�������G1 BBP�µ�
* �� �� : ģʽ
* �� �� : void
* �� �� : �µ��Ƿ�ɹ�
* ˵ �� :
*****************************************************************************/
int bsp_bbp_pwrctrl_g2bbp_poweroff(void)
{
	return BSP_OK;
}

/*****************************************************************************
* �� �� : bsp_bbp_pwrctrl_g2_status
* �� �� : ���͹��ĵ��ã�������G1 BBP�ϵ�
* �� �� : void
* �� �� : void
* �� �� : 0x10Ϊopen, 0x20Ϊclose
* ˵ �� :
*****************************************************************************/
PWC_COMM_STATUS_E bsp_bbp_pwrctrl_g2bbp_status(void)
{
    return PWC_COMM_STATUS_BUTT;
}

/*****************************************************************************
* �� �� : bsp_bbp_wbbp_poweron
* �� �� : ���͹��ĵ��ã�������W BBP�ϵ�
* �� �� : void
* �� �� : void
* �� �� : 0Ϊ�ɹ���-1ʧ��
* ˵ �� :
*****************************************************************************/
int bsp_bbp_pwrctrl_wbbp_poweron(void)
{
	return BSP_OK;
}

/*****************************************************************************
* �� �� : bsp_bbp_wbbp_poweroff
* �� �� : ���͹��ĵ��ã�������W BBP�µ�
* �� �� : ģʽ
* �� �� : void
* �� �� : �µ��Ƿ�ɹ�
* ˵ �� :
*****************************************************************************/
int bsp_bbp_pwrctrl_wbbp_poweroff(void)
{

	return BSP_OK;

}

/*****************************************************************************
* �� �� : bsp_bbp_pwrctrl_wbbp_status
* �� �� : ���͹��ĵ��ã�������W BBP�ϵ�
* �� �� : void
* �� �� : void
* �� �� : 0x10Ϊopen, 0x20Ϊclose
* ˵ �� :
*****************************************************************************/
PWC_COMM_STATUS_E bsp_bbp_pwrctrl_wbbp_status(void)
{
    return PWC_COMM_STATUS_BUTT;
}

/*****************************************************************************
* �� �� : bsp_bbp_twbbp_poweron
* �� �� : ���͹��ĵ��ã�������TW BBP�ϵ�
* �� �� : void
* �� �� : void
* �� �� : 0Ϊ�ɹ���-1ʧ��
* ˵ �� :
*****************************************************************************/
int bsp_bbp_pwrctrl_twbbp_poweron(void)
{

	return BSP_OK;
}

/*****************************************************************************
* �� �� : bsp_bbp_wbbp_poweroff
* �� �� : ���͹��ĵ��ã�������W BBP�µ�
* �� �� : ģʽ
* �� �� : void
* �� �� : �µ��Ƿ�ɹ�
* ˵ �� :
*****************************************************************************/
int bsp_bbp_pwrctrl_twbbp_poweroff(void)
{

	return BSP_OK;

}

/*****************************************************************************
* �� �� : bsp_bbp_pwrctrl_wbbp_status
* �� �� : ���͹��ĵ��ã�������W BBP�ϵ�
* �� �� : void
* �� �� : void
* �� �� : 0x10Ϊopen, 0x20Ϊclose
* ˵ �� :
*****************************************************************************/
PWC_COMM_STATUS_E bsp_bbp_pwrctrl_twbbp_status(void)
{
    return PWC_COMM_STATUS_BUTT;
}
/*****************************************************************************
* �� �� : BSP_LPS_Get_Ta
* �� �� : ��lps����,�鿴������֡ͷ���ϵͳ��֡ͷ����ǰ��
* �� �� : void
* �� �� : void
* �� �� :
* ˵ �� :
*****************************************************************************/
BSP_U16 BSP_LPS_Get_Ta(void)
{
	return 0xffff;
}

#endif
/*****************************************************************************
* �� �� : BSP_BBPIntTimerSwitchStatus
* �� �� : ���ڻ�ȡLBBP TIMER�ж�ʹ��״̬,��׮��������ֵ��Ϊ-1������V9R1
* �� �� : BSP_BOOL
* �� �� : void
* �� �� :
* ˵ �� :
*****************************************************************************/
BSP_S32 BSP_BBPIntTimerSwitchStatus(BSP_BOOL *pbStat)
{
	*pbStat = 0;
	return -1;
}

