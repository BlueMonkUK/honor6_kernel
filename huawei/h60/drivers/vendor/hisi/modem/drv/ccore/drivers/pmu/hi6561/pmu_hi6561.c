/******************************************************************************/
/*  Copyright (C), 2007-2013, Hisilicon Technologies Co., Ltd. */
/******************************************************************************/
/* File name     : pmu_hi6561.c */
/* Version       : 2.0 */
/* Created       : 2013-03-14*/
/* Last Modified : */
/* Description   :  The C union definition file for the module LTE_PMU*/
/* Function List : */
/* History       : */
/* 1 Date        : */
/* Modification  : Create file */
/******************************************************************************/

#include <vxWorks.h>
#include <string.h>
#include <bsp_om.h>
#include <bsp_regulator.h>
#include <bsp_hardtimer.h>
#include <bsp_nvim.h>
#include <bsp_mipi.h>
#include <bsp_pmu_hi6561.h>
#include "pmu_hi6561.h"

#define PMU_HI6561_OVER_FLOW	(1)
#define PMU_HI6561_UNOVER_FLOW	(0)
#define PMU_HI6561_MIPI_ADDR    (5)

/*buck0 voltage table*/
u16 buck0_voltage_tab[]={
	900 , 1100, 1300, 1500, 1700, 1900, 2100, 2300, 2500, 2700, 2900, 3100, 3300, 3500, 3700, 3900};

/*buck1 voltage table*/
u16 buck1_voltage_tab[]={
	1500, 1600, 1700, 1800, 1850, 1900, 2000, 2100, 2200, 2250, 2300, 2350, 2500, 2800, 2850, 3200};

/*buck2 voltage table*/
u16 buck2_voltage_tab[]={
	1000, 1100, 1150, 1200, 1250, 1300, 1350, 1400, 1450, 1500, 1550, 1600, 1700, 1750, 1800, 1850};

/*ldo0Voltage table*/
u16 ldo1_voltage_tab[]={
	1500, 1600, 1700, 1800, 2200, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 3300, 3400, 3500};

/*ldo1 voltage table*/
u16 ldo2_voltage_tab[]={
	1500, 1600, 1700, 1800, 2200, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 3300, 3400, 3500};

/*the volt order should be same with "HI6561_POWER_ID"*/
HI6561_VLTGS_ATTR hi6561_volt_attr[PMU_HI6561_POWER_ID_BUTT]={
	{ldo1_voltage_tab,HI6561_ONOFF2_OFFSET,HI6561_EN_LDO1_INIT_BIT  ,HI6561_SET_LDO1_OFFSET },
	{ldo2_voltage_tab,HI6561_ONOFF2_OFFSET,HI6561_EN_LDO2_INIT_BIT  ,HI6561_SET_LDO2_OFFSET },
	{buck0_voltage_tab,HI6561_ONOFF1_OFFSET,HI6561_EN_BUCK0_INIT_BIT ,HI6561_SET_BUCK0_OFFSET},
	{buck1_voltage_tab,HI6561_ONOFF1_OFFSET,HI6561_EN_BUCK1_INIT_BIT ,HI6561_SET_BUCK1_OFFSET},
	{buck2_voltage_tab,HI6561_ONOFF1_OFFSET,HI6561_EN_BUCK2_INIT_BIT ,HI6561_SET_BUCK2_OFFSET}
};
/*lock in C core */
spinlock_t  pmu_hi6561_lock[HI6561_BUTT];

/*limit current list */
LIMIT_CURRENT_TABLE_E limit_current_list[]= {
		[LIMIT_CURRENT_500MA] = {
			.current_id = LIMIT_CURRENT_500MA ,
			.current_ma = 500},	/*default*/

		[LIMIT_CURRENT_300MA] = {
			.current_id = LIMIT_CURRENT_300MA ,
			.current_ma = 300},

		[LIMIT_CURRENT_200MA] = {
			.current_id = LIMIT_CURRENT_200MA ,
			.current_ma = 200},

		[LIMIT_CURRENT_NO] 	= {
			.current_id = LIMIT_CURRENT_NO,
			.current_ma = 0},

		[LIMIT_CURRENT_1800MA] 	= {
			.current_id = LIMIT_CURRENT_1800MA,
			.current_ma = 1800},

		[LIMIT_CURRENT_1500MA] 	= {
			.current_id = LIMIT_CURRENT_1500MA,
			.current_ma = 1500},

		[LIMIT_CURRENT_1200MA] 	= {
			.current_id = LIMIT_CURRENT_1200MA,
			.current_ma = 1200},

		[LIMIT_CURRENT_900MA] 	= {
			.current_id = LIMIT_CURRENT_900MA ,
			.current_ma = 900}
};

/*exception list*/
char *err_list[]={
	"LDO1 overcurrent",
	"LDO2 overcurrent",
	"BUCKPA overcurrent",
	"BUCK1 overcurrent",
	"BUCK2 overcurrent",
	"Temperature over 150",
	"Temperature over 120",
	"UNKOWN",
};

/* EC  flag */
static u32 after_ec_flag[HI6561_BUTT]= {PASTAR_FALSE,PASTAR_FALSE};
/*������ʶ*/
static u32 over_flow_tag[HI6561_BUTT][PMU_HI6561_POWER_ID_BUTT]={{0,0,0,0,0},{0,0,0,0,0}};

/* ���ڱ�ʶÿ·LDO��BUCK�Ƿ�� */
NV_PASTAR_SWITCH_STRU pastar_switch;

/* ����ĳ·����ر�ʱ���Ը�·������µ�Ĵ�׮���� */
u8 pastar_status_value[PMU_HI6561_POWER_ID_BUTT];

/*���ڱ�ʶPA/RF���絥Ԫ��Pastar or ����*/
NV_PAPOWER_UNIT_STRU pa_power_unit;
NV_RFPOWER_UNIT_STRU rf_power_unit;



/*****************************************************************************

---------------------�ڲ��ӿ�,�ⲿ�������ɵ���-----------------

*****************************************************************************/


/*lint -save -e958*/
static int pmu_hi6561_reg_read(u8 reg_addr,u8 *data,HI6561_ID_ENUM chip_id)
{
	u8 reg_data=0xff;
	int ret = MIPI_ERROR;


	if(!data){
		mipi_print_error("Error:param pointer is null!\nthe func is:\nstatic int pmu_hi6561_reg_read(u8 reg_addr,u8 *data,HI6561_ID_ENUM chip_id)\n");
		return (int)MIPI_ERROR;
	}
	ret=bsp_mipi_data_rev(MIPI_EXTENDED_READ, PMU_HI6561_MIPI_ADDR, reg_addr, &reg_data,(MIPI_CTRL_ENUM)chip_id);
	if(ret<0)
	{
		mipi_print_error("Error: pmu_hi6561_reg_read fail!REG:0x%x,data:0x%0x\n",reg_addr,reg_data);
	}
	*data=reg_data;
	return ret;
}
static int pmu_hi6561_reg_write(u8 reg_addr,u8 reg_data,HI6561_ID_ENUM chip_id)
{
	int ret;
	ret=bsp_mipi_data_send(MIPI_EXTENDED_WRITE, PMU_HI6561_MIPI_ADDR, reg_addr, reg_data,(MIPI_CTRL_ENUM)chip_id);
	if(ret<0)
	{
		mipi_print_error("Error: pmu_hi6561_reg_read fail!reg_addr:0x%x,reg_data:0x%x,channel id:0x%x\n",reg_addr,reg_data,chip_id);
		return MIPI_ERROR;
	}
	return MIPI_OK;
}


static int pmu_hi6561_is_after_ec(HI6561_ID_ENUM chip_id)
{
	u8 reg_addr;
	u8 reg_val;
	int ret;

	reg_addr=HI6561_OTP_PDOB1_OFFSET;

	ret=pmu_hi6561_reg_read(reg_addr,&reg_val, chip_id);
	if((int)MIPI_OK!=ret)
	{
		mipi_print_error("Error: bsp mipi get data failed!channel id:%d\n",chip_id);
	}

	/*in OTP1 register:0x0 is befor EC��there is a bit not equal 0��the version is afer EC*/
	if(reg_val)
	{
		after_ec_flag[chip_id]=PASTAR_TRUE;
		mipi_print_info("hi6561 is after EC\n");
	}
	else{
		after_ec_flag[chip_id] =PASTAR_FALSE;
		mipi_print_info("hi6561 is befor EC\n");
	}
	return (int)MIPI_OK;
}
static void pmu_hi6561_om_log_save(EXCEPTION_TYPE_E hi6561_err_id)
{
    static int i = 0;

	//print2file(PMU_HI6561_OM_LOG,"error log[%d]:%s \n",i,err_list[hi6561_err_id]);
	mipi_print_info("error log[%d]:%s \n",i,err_list[hi6561_err_id]);
	return ;
}


int pmu_hi6561_buck1_2_real_exception_clear(HI6561_ID_ENUM chip_id)
{
    s32 ret = MIPI_ERROR;
    u8 reg_val_wr = 0;

    /* ����buck1�Ŀ���״̬ȷ���Ƿ�Ҫ�������� */
    if(1 == pastar_switch.cont.bits.buck1_switch)
    {
        reg_val_wr |= (1 << BUCK1);
    }

    /* ����buck2�Ŀ���״̬ȷ���Ƿ�Ҫ�������� */
    if(1 == pastar_switch.cont.bits.buck2_switch)
    {
        reg_val_wr |= (1 << BUCK2);
    }
    
    /* ���buck1��2��û�򿪣�ֱ�ӷ��أ�����д�Ĵ����Ĳ��� */
    if(0 == reg_val_wr)
    {
        return MIPI_OK;
    }    

    /* ����󱨵��ж� */
	ret = pmu_hi6561_reg_write(HI6561_STATUS1_OFFSET, reg_val_wr, chip_id);
	if(MIPI_OK != ret)
	{
		mipi_print_error("Error: write reg failed��reg addr 0x%x, ret %d\n", HI6561_STATUS1_OFFSET, ret);
        return MIPI_ERROR;
	}   	

    return MIPI_OK;
}



int pmu_hi6561_buck1_2_phony_exception_clear(HI6561_ID_ENUM chip_id)
{
    s32 ret = MIPI_ERROR;
    u8 status = 0;
    u8 reg_val_wr = 0;

    /* ����buck1�Ŀ���״̬��ʹ��״̬ȷ���Ƿ�Ҫ�������� */
    if(1 == pastar_switch.cont.bits.buck1_switch)
    {
    	ret = pmu_hi6561_power_status(PMU_HI6561_BUCK1, &status, chip_id);
    	if(MIPI_ERROR == ret)
        {
    		mipi_print_error("get buck1 power status error, pastar id:%d\n", chip_id);
            reg_val_wr |= (1 << BUCK1);
    	}
        else if(status == 0)    /* ��ǰ״̬Ϊpower off������������ */
        {
            reg_val_wr |= (1 << BUCK1);
        }
        else
        {
            /* nop */
        }
    }

    /* ����buck2�Ŀ���״̬��ʹ��״̬ȷ���Ƿ�Ҫ�������� */
    if(1 == pastar_switch.cont.bits.buck2_switch)
    {
    	ret = pmu_hi6561_power_status(PMU_HI6561_BUCK2, &status, chip_id);
    	if(MIPI_ERROR == ret)
        {
    		mipi_print_error("get buck2 power status error, pastar id:%d\n", chip_id);
            reg_val_wr |= (1 << BUCK2);
    	}
        else if(status == 0)    /* ��ǰ״̬Ϊpower off������������ */
        {
            reg_val_wr |= (1 << BUCK2);
        }
        else
        {
            /* nop */
        }
    }

    /* ���buck1��2������Ҫ�������ǣ�ֱ�ӷ��أ�����д�Ĵ����Ĳ��� */
    if(0 == reg_val_wr)
    {
        return MIPI_OK;
    }
  
    /* ����󱨵��ж� */
	ret |= pmu_hi6561_reg_write(HI6561_STATUS1_OFFSET, reg_val_wr, chip_id);
	if(MIPI_OK != ret)
	{
		mipi_print_error("Error: write reg failed��reg addr 0x%x, ret %d\n", HI6561_STATUS1_OFFSET, ret);
        return MIPI_ERROR;
	}   	

    return MIPI_OK;
}


static int pmu_hi6561_exc_clear(EXCEPTION_TYPE_E type,HI6561_ID_ENUM chip_id)
{
	/*���״̬�Ĵ���*/
	pmu_hi6561_reg_write(HI6561_STATUS1_OFFSET,1<<type,chip_id);
	return MIPI_OK;
}
static int pmu_hi6561_power_switch( HI6561_POWER_ID power_id,POWER_PROC_ENUM power_proc,HI6561_ID_ENUM chip_id )
{
	int ret=MIPI_OK;
	u8 reg_val=0;
	HI6561_VLTGS_ATTR hi6561_volt={0,0,0,0};

	/*judge the paramemt is invalid or not*/
	if(power_id>PMU_HI6561_BUCK2)
	{
		mipi_print_error("Error:power id is invalid!input power id is %d,but power id range is from 0 to 4\n",power_id);
		return MIPI_ERROR;
	}

	if(power_id<PMU_HI6561_POWER_ID_BUTT)hi6561_volt = hi6561_volt_attr[power_id];/*for pc-Lint*/

	ret=pmu_hi6561_reg_read(hi6561_volt.onoff_reg_addr, &reg_val, chip_id);
	if(MIPI_OK!=ret)
	{
		mipi_print_error("Error:can't get pastar register value !register is 0x%x\n",hi6561_volt.onoff_reg_addr);
		goto out;
	}

	/*if open*/
	if(PA_STAR_POWER_ON==power_proc){
		reg_val |= (u8)((unsigned)PA_STAR_POWER_ON<<hi6561_volt.onoff_bit_offset);
	}
	else{/*close*/
	    reg_val &= ~(u8)((unsigned)1<<hi6561_volt.onoff_bit_offset);
	}

	ret=pmu_hi6561_reg_write(hi6561_volt.onoff_reg_addr,reg_val,chip_id);
	if(MIPI_OK!=ret)
	{
		mipi_print_error("Error:can't write data to pastar!register:0x%x,data:0x%x,chip_id:0x:%x\n",hi6561_volt.onoff_reg_addr,reg_val,chip_id);
	}

out:
	return ret;
}


static int pmu_hi6561_power_off_local(HI6561_POWER_ID power_id,HI6561_ID_ENUM chip_id )
{
	int ret=MIPI_OK;
	/*judge the paramemt is invalid or not*/
	if(power_id>PMU_HI6561_BUCK2)
	{
		mipi_print_error("Error:power id is invalid!input power id is %d,but power id range is from 0 - 4\n",power_id);
		return MIPI_ERROR;
	}
	if(power_id>PMU_HI6561_LDO2&&power_id<PMU_HI6561_POWER_ID_BUTT){
		if(!strcmp(pmu_hi6561_exc_isr(chip_id),err_list[power_id])){
			over_flow_tag[chip_id][power_id]=PMU_HI6561_OVER_FLOW;/*��Ҫ��zhanglipingȷ�ϣ���־λ������ϲ�֪��*/
		}
	}
	ret=pmu_hi6561_power_switch(power_id, PA_STAR_POWER_OFF,chip_id);
	if(MIPI_OK!=ret){
		mipi_print_error("Error:close buck failed!power id:%d,channel id:%d\n",power_id,chip_id);
	}
    /* ���buck pa������ǣ�buck1/2�Ĺ��������adp�ӿ������ */
    if(PMU_HI6561_BUCK_PA == power_id)
    {
    	ret=pmu_hi6561_exc_clear((EXCEPTION_TYPE_E)power_id,chip_id);
    	if(MIPI_OK!=ret){
    		mipi_print_error("Error:close buck failed!power id:%d,channel id:%d\n",power_id,chip_id);
    	}
    }
	return ret;
}
static int pmu_hi6561_power_on_local(HI6561_POWER_ID power_id ,HI6561_ID_ENUM chip_id)
{
	int ret=MIPI_OK;

	/*judge the paramemt is invalid or not*/
	if(power_id>PMU_HI6561_BUCK2)
	{
		mipi_print_error("Error:power id is invalid!input power id is %d,but power id range is from 0 to 4\n",power_id);
		return MIPI_ERROR;
	}

	/*�鿴������־λ���Ƿ���Դ򿪵�Դ*/
	if(power_id>PMU_HI6561_LDO2&&power_id<PMU_HI6561_POWER_ID_BUTT){
		if(over_flow_tag[chip_id][power_id]==PMU_HI6561_OVER_FLOW){
			mipi_print_error("Error:pastar[%d] power id [%d]has over flow!\n",chip_id,power_id);
		}
	}

	/* ���buck pa������ǣ�buck1/2�Ĺ��������adp�ӿ������ */
    if(PMU_HI6561_BUCK_PA == power_id)
    {
    	ret=pmu_hi6561_exc_clear((EXCEPTION_TYPE_E)power_id,chip_id);
    	if(MIPI_OK!=ret){
    		mipi_print_error("Error:clear status register failed!power id:%d,channel id:%d\n",power_id,chip_id);
    	}
    }

	ret=pmu_hi6561_power_switch(power_id, PA_STAR_POWER_ON,chip_id);
	return ret;

}
static int pmu_hi6561_apt_enable_local(HI6561_ID_ENUM chip_id)
{
	u8 reg_addr = HI6561_SET_BUCK0_OFFSET;
	u8 reg_mask = HI6561_EN_BUCK0_APT_MASK;
	u8 reg_val;
	int ret = 0;

	/*ʹ��APT,���ñ��е�0x28д0x3c,zhanglipingȷ�Ͽɲ���ע��ѹ*/
	ret=pmu_hi6561_reg_read(reg_addr,&reg_val, chip_id);
	if(OK!=ret)
	{
		mipi_print_error("Error: bsp mipi get data failed!\n \
			addr:0x%x,chip_id:0x%x,read_data:0x%x\n",reg_addr,chip_id,reg_val);
		return BSP_ERROR;
	}

	reg_val=reg_val&reg_mask;
	reg_val=reg_val|(~reg_mask);
	ret=pmu_hi6561_reg_write(reg_addr,reg_val,chip_id);
	if(OK!=ret)
	{
		mipi_print_error("Error: bsp mipi write data failed!\n \
			addr:0x%x,chip_id:0x%x,write_data:0x%x\n",reg_addr,chip_id,reg_val);
		return BSP_ERROR;
	}

	/*����APT RC����*/
	ret|=pmu_hi6561_reg_write(0x2c,0x6a,chip_id);

	/*�Ż�buck 0*/
	ret|=pmu_hi6561_reg_write(0x29,0xbe,chip_id);
	ret|=pmu_hi6561_reg_write(0x2a,0xe9,chip_id);

	return ret;
}

static int pmu_hi6561_apt_disable_local(HI6561_ID_ENUM chip_id)
{
	u8 reg_mask = HI6561_EN_BUCK0_APT_MASK;
	u8 reg_addr = HI6561_SET_BUCK0_OFFSET;
	u8 reg_val;
	int ret;

	/*apt RC vlaue*/
	ret =pmu_hi6561_reg_write(0x2c,0x7a,chip_id);/* [false alarm]:����Fortify���� */
	if(OK != ret)
	{
		mipi_print_error("Error: pastar write data failed!register:0x2c,chip_id:0x%x\n", chip_id);
	}

	/*�ر�APT,���ñ��е�0x28д0x2c,zhanglipingȷ�Ͽɲ���ע��ѹ*/
	ret = pmu_hi6561_reg_read(reg_addr,&reg_val,chip_id);
	if(OK!=ret)
	{
		mipi_print_error("Error: pastar read data failed!register:0x%x,chip_id:0x%x\n",reg_addr,chip_id);
		return ret;
	}
	reg_val=reg_val&reg_mask;
	ret=pmu_hi6561_reg_write(reg_addr,reg_val,chip_id);
	if(OK!=ret)
	{
		mipi_print_error("Error: pastar write data failed!register:0x%x,chip_id:0x%x\n",reg_addr,chip_id);
		return ret;
	}
	/*�Ż�buck0*/
	ret|=pmu_hi6561_reg_write(0x29,0xbe,chip_id);
	ret|=pmu_hi6561_reg_write(0x2a,0xe9,chip_id);
	return ret;
}


static int pmu_hi6561_limit_current_set_local(u32 current_ma,HI6561_ID_ENUM chip_id)
{
	u8 reg_val=0;
	u8 reg_cur=0;
	u32 imax=0;
	u32 i=0;
	int ret=0;

	imax=sizeof(limit_current_list)/sizeof(struct LIMIT_CURRENT_TABLE);
	/*transfer current value to id*/
	for(i=0;i<imax;i++)
	{
		if(limit_current_list[i].current_ma==current_ma){
			reg_cur=limit_current_list[i].current_id;
			break;
		}
	}
	/*judge has got the current value or doesn't*/
	if(i==imax){
		mipi_print_error("Error:can't find the current %d in current list, \
			maybe the value is invalid!\n",current_ma);
		return MIPI_ERROR;
	}

	/*write the limit current value to register*/
	ret =pmu_hi6561_reg_read(HI6561_ILIM_CTRL1_OFFSET, &reg_val,chip_id);
	if(ret==MIPI_ERROR){
		mipi_print_error("Error: get the pastar reg data fail,the reg offset is %x!\n",HI6561_ILIM_CTRL1_OFFSET);
		goto out;
	}
	reg_val=(reg_val&HI6561_ILIM_CTRL1_MASK)|reg_cur;

	ret =pmu_hi6561_reg_write(HI6561_ILIM_CTRL1_OFFSET,reg_val,chip_id);
	if(MIPI_ERROR==ret)
	{
		mipi_print_error("Error:cant write the data to pastar register %x!\n",HI6561_ILIM_CTRL1_OFFSET);
	}
out:
	return ret;
}
static int pmu_hi6561_voltage_set_local(HI6561_POWER_ID power_id,u16 voltage_mv,HI6561_ID_ENUM chip_id)
{
	int ret=MIPI_OK;
	u8 vol_id=0;
	u8 reg_val=0;
	HI6561_VLTGS_ATTR hi6561_volt={0,0,0,0};

	/*judge the paramemt is invalid or not*/
	if(power_id>PMU_HI6561_BUCK2)
	{
		mipi_print_error("Error:power id is invalid!input power id is %d,but power id range is from 0 to 4\n",power_id);
		return  MIPI_ERROR;
	}

	if(power_id<PMU_HI6561_POWER_ID_BUTT){
		hi6561_volt=hi6561_volt_attr[power_id];
	}
	else
		return MIPI_ERROR;

	/*firstly,read voltage register value*/
	ret=pmu_hi6561_reg_read(hi6561_volt.voltage_reg_addr,&reg_val,chip_id);
	if(OK!=ret)
	{
		mipi_print_error("Error: bsp mipi get data failed!reg_addr:0x%x ; chip_id:%d\n",hi6561_volt.voltage_reg_addr,chip_id);
		goto out;
	}

	/*adapt the voltage value and register saved value*/
	for(vol_id=0;vol_id<VOLTAGEMAX;vol_id++)
	{
		if(voltage_mv==hi6561_volt.voltage_list[vol_id])
			break;
	}

	if(VOLTAGEMAX==vol_id){
		mipi_print_error("Error:voltage is not invalid!voltage:%d\n",voltage_mv);
		goto out;
	}

	reg_val=(reg_val&HI6561_SET_VOLTAGE_MASK)|vol_id;

	/*write the value*/
	ret=pmu_hi6561_reg_write(hi6561_volt.voltage_reg_addr,reg_val,chip_id);
	if(OK!=ret)
	{
		mipi_print_error("Error: bsp mipi get data failed!\n");
	}

out:
	return ret;

}

/*****************************************************************************

---------------------end �ڲ��ӿ�,�ⲿ�������ɵ���--------------

*****************************************************************************/


char *pmu_hi6561_exc_isr(HI6561_ID_ENUM chip_id )
{
	EXCEPTION_TYPE_E err_type=EXCEPTION_MAX;
	int ret=0;
	u32 bit=0;
	u8 reg_val[2]={0,0};

	/* get exception regitster,0x21 */
	ret=pmu_hi6561_reg_read((u8)HI6561_STATUS1_OFFSET,&reg_val[0], chip_id);
	if(OK!=ret)
	{
		mipi_print_error("Error: pastar exception regitster data failed!reg addr:0x%x\n", HI6561_STATUS1_OFFSET);
	}

    reg_val[0]=reg_val[0]&(~HI6561_NO_PWR_REG_RO_MASK);

	/*judge the type of exception*/
	for(bit=LDO1CUR;bit<EXCEPTION_MAX;bit++)
	{
		if(reg_val[0]&((unsigned)1<<bit)){
			err_type=(EXCEPTION_TYPE_E)bit;
			break;
		}
	}

	/*is there an exception*/
	if(EXCEPTION_MAX!=err_type)
	{
		pmu_hi6561_om_log_save(err_type);
        
#if defined(CONFIG_PASTAR_DSM)
        /* get 0x6a reg */
        ret=pmu_hi6561_reg_read((u8)HI6561_NO_PWR_REG_RO_OFFSET,&reg_val[1], chip_id);
    	if(OK != ret)
    	{
    		mipi_print_error("Error: pastar exception regitster data failed!reg addr:0x%x\n", HI6561_NO_PWR_REG_RO_OFFSET);
    	}

        /* �쳣��Ϣ���͸�A�ˣ�A�����ϱ� */
        ret = bsp_icc_send(ICC_CPU_APP, PASTAR_ICC_CHN_ID, (u8*)reg_val, sizeof(reg_val)); 
        if(ret != (int)sizeof(reg_val))
        {
    		mipi_print_error("Error: icc send failed! ret:0x%x\n", ret);
        }    
#endif	
    }

	return err_list[err_type];
}
int pmu_hi6561_apt_status_get(HI6561_ID_ENUM chip_id)
{
	u8 reg_addr = HI6561_SET_BUCK0_OFFSET;
	u8 reg_bit =HI6561_EN_BUCK0_APT_BIT;
	u8 reg_val = 0;
	int ret = MIPI_OK;
	unsigned int status;

	ret = pmu_hi6561_reg_read(reg_addr,&reg_val, chip_id);
	if(MIPI_OK!=ret)
	{
		mipi_print_error("Error: pastar get apt status data failed!\n");
	}

	reg_val = reg_val&(u8)(1<<HI6561_EN_BUCK0_APT_BIT);
	status = reg_val>>reg_bit;
	return (int)status;

}


int pmu_hi6561_apt_enable(HI6561_ID_ENUM chip_id)
{
	int ret = MIPI_OK;
	unsigned long flags;

	if(pmu_hi6561_apt_status_get(chip_id)){
		return MIPI_OK;
	}
	/*write after read,add lock */
	spin_lock_irqsave(&pmu_hi6561_lock[chip_id],flags);

	ret=pmu_hi6561_apt_enable_local(chip_id);
	if(OK!=ret)
	{
		mipi_print_error("Error: pastar enable apt failed!chip id:%d\n",chip_id);
	}
	spin_unlock_irqrestore(&pmu_hi6561_lock[chip_id],flags);

	return ret;
}

int pmu_hi6561_apt_disable(HI6561_ID_ENUM chip_id)
{
	int ret = 0;
	unsigned long flags;

	if(0==pmu_hi6561_apt_status_get(chip_id)){
		return MIPI_OK;
	}
	/*write after read,add lock */
	spin_lock_irqsave(&pmu_hi6561_lock[chip_id],flags);

	ret = pmu_hi6561_apt_disable_local(chip_id);
	if(OK!=ret)
	{
		mipi_print_error("Error: pastar disable apt failed!chip id:%d\n",chip_id);
	}

	spin_unlock_irqrestore(&pmu_hi6561_lock[chip_id],flags);

	return ret;
}
int pmu_hi6561_limit_current_get(u32 *current_ma,HI6561_ID_ENUM chip_id)
{
	u8 reg_val=0;
	u32 reg_cur=0;
	u32 imax=0;
	u32 i=0;
	int ret=0;

	/*read limit current id from register*/
	ret = pmu_hi6561_reg_read(HI6561_ILIM_CTRL1_OFFSET,&reg_val, chip_id);
	if(OK!=ret)
	{
		mipi_print_error("Error: pastar read current limited data failed!reg addr:0x%x,chip id:%x\n",HI6561_ILIM_CTRL1_OFFSET,chip_id);
		return MIPI_ERROR;
	}

	reg_cur=reg_val&(~HI6561_ILIM_CTRL1_MASK);

	/*get current value by current id��0MA is does't limit current*/
	imax=sizeof(limit_current_list)/sizeof(struct LIMIT_CURRENT_TABLE);
	for(i=0;i<imax;i++)
	{
		if(limit_current_list[i].current_id==reg_cur){
			*current_ma=limit_current_list[i].current_ma;
			break;
		}
	}
	/*judge getting limit current is success*/
	if(i==imax){
		mipi_print_error("Error:can't find the current %d in current list,maybe the value is invalid!\n",current_ma);
		return MIPI_ERROR;
	}

	return MIPI_OK;
}
int pmu_hi6561_limit_current_set(u32 current_ma,HI6561_ID_ENUM chip_id)
{
	int ret=0;
	unsigned long flags;

	/*write after read,add lock */
	spin_lock_irqsave(&pmu_hi6561_lock[chip_id],flags);

	/*write the limit current value to register*/
	ret =pmu_hi6561_limit_current_set_local(current_ma,chip_id);
	if(ret==MIPI_ERROR){
		mipi_print_error("Error: set pastar limit current fail, current:%dma,channel id:%d\n", current_ma,chip_id);
	}

	spin_unlock_irqrestore(&pmu_hi6561_lock[chip_id],flags);

	return ret;
}


int pmu_hi6561_voltage_get(HI6561_POWER_ID power_id,u32 *voltage_mv,HI6561_ID_ENUM chip_id)
{
	int ret=MIPI_OK;
	u8 reg_val=0;
	HI6561_VLTGS_ATTR hi6561_volt={0,0,0,0};

	/*judge the paramemt is invalid or not*/
	if(power_id>PMU_HI6561_BUCK2)
	{
		mipi_print_error("Error:power id is invalid!input power id is %d,but power id range is from 0 to 4\n",power_id);
		ret=MIPI_ERROR;
        goto out;
	}

	if(power_id<PMU_HI6561_POWER_ID_BUTT){/*for pc-Lint*/
		hi6561_volt=hi6561_volt_attr[power_id];
	}
	else
		return MIPI_ERROR;

	/*firstly,read voltage register value*/
	ret=pmu_hi6561_reg_read(hi6561_volt.voltage_reg_addr,&reg_val,chip_id);
	if(OK!=ret)
	{
		mipi_print_error("Error: pastar read data failed!reg addr:0x%x,channel id:%d\n",hi6561_volt.voltage_reg_addr,chip_id);
		goto out;
	}

	reg_val=(reg_val&(~HI6561_SET_VOLTAGE_MASK));

	if(VOLTAGEMAX<reg_val){
		mipi_print_error("Error:get voltage value is not invalid!reg_val:0x%x\n",reg_val);
		goto out;
	}

	*voltage_mv=(u32)hi6561_volt.voltage_list[reg_val];

out:
	return ret;
}
int pmu_hi6561_voltage_set(HI6561_POWER_ID power_id,u16 voltage_mv,HI6561_ID_ENUM chip_id)
{
	int ret=MIPI_OK;
	unsigned long flags;

	/*write after read,add lock */
	spin_lock_irqsave(&pmu_hi6561_lock[chip_id],flags);

	/*firstly,read voltage register value*/
	ret=pmu_hi6561_voltage_set_local(power_id,voltage_mv,chip_id);
	if(OK!=ret)
	{
		mipi_print_error("Error: bsp set pastar voltage failed!, vol:%dmv\n", voltage_mv);
	}

	spin_unlock_irqrestore(&pmu_hi6561_lock[chip_id],flags);

	return ret;

}

int pmu_hi6561_voltage_list_get(HI6561_POWER_ID power_id,u16 **list, u32 *size)
{
	int ret=MIPI_OK;
	HI6561_VLTGS_ATTR hi6561_volt={0,0,0,0};

	/*judge the paramemt is invalid or not*/
	if(power_id>PMU_HI6561_BUCK2)
	{
		mipi_print_error("Error:power id is invalid!input power id is %d,but power id range is from 0 to 4\n",power_id);
		return MIPI_ERROR;
	}

	if(power_id<PMU_HI6561_POWER_ID_BUTT)hi6561_volt=hi6561_volt_attr[power_id];/*for pc-Lint*/

	*list = hi6561_volt.voltage_list;
	*size = VOLTAGEMAX;
	ret = MIPI_OK;

	return ret;
}

int pmu_hi6561_power_status(HI6561_POWER_ID power_id,u8 *status,HI6561_ID_ENUM chip_id)
{
	int ret=MIPI_OK;
	u8 reg_val=0;
	HI6561_VLTGS_ATTR hi6561_volt={0,0,0,0};

	/*judge the paramemt is invalid or not*/
	if(power_id>PMU_HI6561_BUCK2)
    {
		mipi_print_error("Error:power id is invalid!input power id is %d,but power id range is from 0 to 4\n",power_id);
		return MIPI_ERROR;
	}

	if(!status)
	{
		mipi_print_error("Error:the param status is null pointer!\n");
		return MIPI_ERROR;
	}
	if(power_id<PMU_HI6561_POWER_ID_BUTT)hi6561_volt = hi6561_volt_attr[power_id];/*for pc-Lint*/

    /* ����nv�ж϶�Ӧ��LDO/BUCK�Ƿ�򿪣�û�д򿪣�ֱ�ӷ���OK������statusΪ�� */
    if(power_id >= PMU_HI6561_POWER_ID_BUTT)
    {
		mipi_print_error("Error:illigal param, power_id %d\n",power_id);
		return MIPI_ERROR;
    }
    
    if(!((1 << power_id) & pastar_switch.cont.u32))
    {
        *status = pastar_status_value[power_id]; /* ��·û�д򿪣���ѯpastar_stub�����ѯ�Ĵ��� */
        return MIPI_OK;
    }

	/*get the power status*/
	ret=pmu_hi6561_reg_read(hi6561_volt.onoff_reg_addr,&reg_val,chip_id);
	if(OK!=ret)
	{
		mipi_print_error("Error:pastar read reg data fail!,reg:0x%x,channel id:%d\n",hi6561_volt.onoff_reg_addr,chip_id);
		return MIPI_ERROR;
	}

	*status=(u8)(reg_val&((unsigned )1 << hi6561_volt.onoff_bit_offset));

	return ret;
}
int pmu_hi6561_power_off(HI6561_POWER_ID power_id,HI6561_ID_ENUM chip_id )
{
	int ret=MIPI_OK;
	unsigned long flags;
    
    if(power_id >= PMU_HI6561_POWER_ID_BUTT)
    {
		mipi_print_error("Error:illigal param, power_id %d\n",power_id);
		return MIPI_ERROR;
    }
    
    /* ����nv�ж϶�Ӧ��LDO/BUCK�Ƿ�򿪣�û�д򿪣�ֱ�ӷ���OK */
    if(!((1 << power_id) & pastar_switch.cont.u32))
    {
        pastar_status_value[power_id] = 0; /* ��·û�д򿪣�����pastar_stubΪoff,�������üĴ������Ա��ѯ״̬ */
        return MIPI_OK;
    }	

    /*write after read,add lock */
	spin_lock_irqsave(&pmu_hi6561_lock[chip_id],flags);

	ret=pmu_hi6561_power_off_local(power_id,chip_id);
	if(MIPI_OK!=ret){
		mipi_print_error("Error:close power failed!channel id %d,power id %d\n",chip_id,power_id);
		ret=MIPI_ERROR;
	}

	spin_unlock_irqrestore(&pmu_hi6561_lock[chip_id],flags);

	return ret;
}
int pmu_hi6561_power_on(HI6561_POWER_ID power_id ,HI6561_ID_ENUM chip_id)
{
	int ret=MIPI_OK;
	unsigned long flags;

    if(power_id >= PMU_HI6561_POWER_ID_BUTT)
    {
		mipi_print_error("Error:illigal param, power_id %d\n",power_id);
		return MIPI_ERROR;
    }    

    /* ����nv�ж϶�Ӧ��LDO/BUCK�Ƿ�򿪣�û�д򿪣�ֱ�ӷ���OK */
    if(!((1 << power_id) & pastar_switch.cont.u32))
    {
        pastar_status_value[power_id] = 1; /* ��·û�д򿪣�����pastar_stubΪon,�������üĴ������Ա��ѯ״̬ */
        return MIPI_OK;
    }

	/*write after read,add lock */
	spin_lock_irqsave(&pmu_hi6561_lock[chip_id],flags);

	ret=pmu_hi6561_power_on_local(power_id,chip_id);
	if(MIPI_OK!=ret){
		mipi_print_error("Error:close power failed!power id:%d,channel id:%d\n",power_id,chip_id);
		ret=MIPI_ERROR;
	}

	spin_unlock_irqrestore(&pmu_hi6561_lock[chip_id],flags);

	return ret;
}
static int pmu_hi6561_initial(HI6561_ID_ENUM chip_id)
{
	int ret;
	u8 version;

	/*get pastar version*/
	ret=pmu_hi6561_reg_read(HI6561_VERSION_OFFSET,&version,chip_id);
	if(MIPI_OK!=ret){
		mipi_print_info("Error:can't get pastar[%d] version,patar init fail!\n",chip_id);
		return MIPI_ERROR;
	}

	/*judge is it pastar v100(hi6561)*/
	if(HI6561_IS_V100!=version){
		mipi_print_info("Error: the pastar[%d] version is wrong!,version: 0x%x\n",chip_id,version);
		return MIPI_ERROR;
	}

	/*the judgment is befor EC or after EC */
	ret=pmu_hi6561_is_after_ec(chip_id);
	if(ret<0){
		mipi_print_info("Error:can't get pastar[%d] is after ec,pastar init fail!\n",chip_id);
		return MIPI_ERROR;
	}

	/*Befor EC or after EC optration*/
	if(PASTAR_FALSE==after_ec_flag[chip_id])
	{/*Befor EC optimized opration*/
		ret|=pmu_hi6561_reg_write(0x4e,0x04,chip_id);/*limit curren is 1800ma*/
		ret|=pmu_hi6561_reg_write(0x45,0x0c,chip_id);/*buck frequncy is 2.5MHZ*/
		ret|=pmu_hi6561_reg_write(0x29,0xbe,chip_id);/*for adjusting the EA transconductance and zero alignment resistance*/
		ret|=pmu_hi6561_reg_write(0x2a,0xe9,chip_id);/**/
		ret|=pmu_hi6561_reg_write(0x2c,0x7a,chip_id);
	}
	else{/*After EC optimized opration*/
		ret|=pmu_hi6561_reg_write(0x4c,0x06,chip_id);
		ret|=pmu_hi6561_reg_write(0x6a,0x00,chip_id);/*update 1.1*/
		ret|=pmu_hi6561_reg_write(0x2d,0x82,chip_id);/*update 1.2*/
		ret|=pmu_hi6561_reg_write(0x32,0x0c,chip_id);
		ret|=pmu_hi6561_reg_write(0x36,0x01,chip_id);
		ret|=pmu_hi6561_reg_write(0x37,0x09,chip_id);
		ret|=pmu_hi6561_reg_write(0x38,0x04,chip_id);
	}
	if(MIPI_OK!=ret)
	{
		mipi_print_error("Error: the optimized work can't oprate,hi6561 init has failed!\n");
		return MIPI_ERROR;
	}
	pmu_hi6561_voltage_set(PMU_HI6561_BUCK_PA,3300,chip_id);/*buck0 3.3v */
	pmu_hi6561_voltage_set(PMU_HI6561_BUCK1,1800,chip_id);/*buck1 1.8v */
	#if 0
	/*����nv�����ж��Ƿ��, ������Щ��Ʒ��̬��û�ж�Ӧ�ĵĹ�����������������*/
    pmu_hi6561_power_on(PMU_HI6561_BUCK1,chip_id); /*open buck1*/
	pmu_hi6561_power_on(PMU_HI6561_BUCK_PA,chip_id);/*open buck0*/
	pmu_hi6561_power_on(PMU_HI6561_LDO2,chip_id);/*open LDO2*/
	pmu_hi6561_power_on(PMU_HI6561_LDO1,chip_id);/*open LDO1*/
    
	pmu_hi6561_voltage_set(PMU_HI6561_BUCK2,1800,chip_id);/*buck2 1.8v */
	pmu_hi6561_power_on(PMU_HI6561_BUCK2,chip_id);	/*open buck2*/
	#endif


	pmu_hi6561_reg_write(0x4e,0x03,chip_id);/*set limit current vlaue*/
	pmu_hi6561_reg_write(0x2c,0x7a,chip_id);/*set apt RC*/

	return MIPI_OK;
}
int pmu_hi6561_init_phase2(void)
{
	int ret = MIPI_OK;

	/*���ø�pastar�ĳ�ʼ������*/
	ret |=pmu_hi6561_initial(HI6561_0);
	if(rf_power_unit.rfpower_m1 == RF_POWER_FROM_HI6561){
		ret |=pmu_hi6561_initial(HI6561_1);
	}

	if(ret){
		mipi_print_info("pmu hi6561 init phase2 config fail!\n");
	}

	return ret;
}
int pmu_hi6561_init_phase1(void)
{
	int ret=0;
	struct regulator *pastar_re = NULL;

	/*pastar �ϵ磬��smart star LVS5*/
    pastar_re = regulator_get(NULL, "PA_STAR_VDD-vcc");
    if(NULL == pastar_re)
    {
        mipi_print_error("pastar regulator cannot get. \n");/*[false alarm]:����fortify���� */
        return MIPI_ERROR;
    }

    if(0 != regulator_enable(pastar_re))
    {
        mipi_print_error("pastar regulator enable is fail. \n");
        return MIPI_ERROR;
    }

	/*��ȡnv ,buck2�Ƿ��*/
	ret = (int)bsp_nvm_read((u32)NV_ID_DRV_PASTAR_SWITCH,(u8 *)&pastar_switch,(u32)sizeof(NV_PASTAR_SWITCH_STRU));
	if(NV_OK != ret)
	{
		mipi_print_error("pastar buck2 read nv error,not set!\n");
	}

	ret |= (int)bsp_nvm_read((u32)NV_ID_DRV_PAPOWER_UNIT,(u8 *)&pa_power_unit,(u32)sizeof(NV_PAPOWER_UNIT_STRU));
	if(NV_OK != ret){
		mipi_print_error("get pa/rf power unit nv error,not set!\n");
	}

	ret |= (int)bsp_nvm_read((u32)NV_ID_DRV_RFPOWER_UNIT,(u8 *)&rf_power_unit,(u32)sizeof(NV_RFPOWER_UNIT_STRU));
	if(NV_OK != ret){
		mipi_print_error("get pa/rf power unit nv error,not set!\n");
	}
    
	/*init lock*/
	spin_lock_init(&pmu_hi6561_lock[0]);
	spin_lock_init(&pmu_hi6561_lock[1]);

	ret |=adp_pmu_hi6561_initial(HI6561_0);
	ret |=adp_pmu_hi6561_initial(HI6561_1);
	if(ret){
		mipi_print_error("pmu hi6561 lock init fail!\n");
	}

	return ret;
}

#define PMU_SAVE_REG_NUM_MAX 20
unsigned char hi6561_save_reg_list[]={
    HI6561_BUCK0_ADJ4_OFFSET,	/*0x2c*/
	HI6561_SET_BUCK0_OFFSET,	/*0x28*/
	HI6561_BUCK0_ADJ1_OFFSET,   /*0x29*/
	HI6561_BUCK0_ADJ2_OFFSET,	/*0x2a*/
};

unsigned char hi6561_0_save_reg_data[PMU_SAVE_REG_NUM_MAX];
unsigned char hi6561_1_save_reg_data[PMU_SAVE_REG_NUM_MAX];


unsigned int pmu_hi6561_reg_save(void)
{
	int i=0;
	int ret = 0;

	int save_num=sizeof(hi6561_save_reg_list)/sizeof(unsigned char);
	for(i=0;i<save_num;i++)
	{
		ret = pmu_hi6561_reg_read(hi6561_save_reg_list[i],&hi6561_0_save_reg_data[i],HI6561_0);
		if(ret){
			mipi_print_error("pmu hi6561 0 save fail!\n");
			return (unsigned int)MIPI_ERROR;
		}
		if(rf_power_unit.rfpower_m1 == RF_POWER_FROM_HI6561){

			ret = pmu_hi6561_reg_read(hi6561_save_reg_list[i],&hi6561_1_save_reg_data[i],HI6561_1);
			if(ret){
				mipi_print_error("pmu hi6561 1 save fail!\n");
				return (unsigned int)MIPI_ERROR;
			}
		}
	}
	
	/*��Ҫȷ���ڽ�����˯ʱ��0x6a�����Ĵ���Ϊ0����ֹ©����*/
	pmu_hi6561_reg_write(0x6a,0,HI6561_0);

	pmu_hi6561_reg_write(0x6a,0,HI6561_1);
	return MIPI_OK;
}
unsigned int pmu_hi6561_reg_resume(void)
{
	int index=0;
	int ret=0;

	int re_num=sizeof(hi6561_save_reg_list)/sizeof(unsigned char);

	/*��ʼ��*/
	pmu_hi6561_init_phase2();

	/*�ָ�*/
	for(index=0;index<re_num;index++)
	{
		ret = pmu_hi6561_reg_write(hi6561_save_reg_list[index],hi6561_0_save_reg_data[index],0);
		if(ret){
			mipi_print_error("pmu hi6561 resume fail!\n");
			return (unsigned int)MIPI_ERROR;
		}

	    if(rf_power_unit.rfpower_m1 == RF_POWER_FROM_HI6561)
        {
    		ret = pmu_hi6561_reg_write(hi6561_save_reg_list[index],hi6561_1_save_reg_data[index],1);
    		if(ret){
    			mipi_print_error("pmu hi6561 1 resume fail!\n");
    			return (unsigned int)MIPI_ERROR;
    		}
        }
    }
    
	adp_pmu_hi6561_resume();
	return MIPI_OK;

}

int pmu_hi6561_debug(HI6561_ID_ENUM chip_id)
{
	u8 onoff[PMU_HI6561_POWER_ID_BUTT]={0,0,0,0,0};
	u32 voltage_mv[PMU_HI6561_POWER_ID_BUTT]={0,0,0,0,0};
	u32 limit_current=0xff;
	int i=0;
	int ret=0;

	/*lint -save -e18 -e718 -e746*//*pclint for printf*/
	printf("\n******************pastar[%d] work status show******************\n",chip_id);
	printf("power id:\tLDO1--0\tLDO2--1\tBUCK0--2\tBUCK1--3\tBUCK2--4\n");
	printf("power status:\t0--power off\tnon 0--power on\n\n");
	for(i=0;i<PMU_HI6561_POWER_ID_BUTT;i++)
	{
		ret=pmu_hi6561_power_status((HI6561_POWER_ID)i,&onoff[i],chip_id);
		if(MIPI_ERROR==ret){
			mipi_print_error("get pmu_hi6561_power_status error,pastar id:%d,power id:%d\n",chip_id,i);
			return MIPI_ERROR;
		}

		ret=pmu_hi6561_voltage_get((HI6561_POWER_ID)i,&voltage_mv[i],chip_id);
		if(MIPI_ERROR==ret){
			mipi_print_error("get pmu_hi6561_voltage error,pastar id:%d,power id:%d\n",chip_id,i);
			return MIPI_ERROR;
		}
		printf("\npastar id:%d,power id:%d\npower status:%d\n",chip_id,i,onoff[i]);
		printf("voltage:%dmv\n",voltage_mv[i]);
	}

	ret = pmu_hi6561_limit_current_get(&limit_current, chip_id);
	if(MIPI_ERROR == ret){
		printf("Current is limited get failed\n");
		return MIPI_ERROR;
	}
	printf("\nCurrent is limited in : %d('0' don't limit current) \n",limit_current);

	ret = pmu_hi6561_apt_status_get(chip_id);
	if(MIPI_ERROR == ret){
		printf("pmu_hi6561_apt_status_get failed\n");
	}
	printf("\nAPT status is:%d (0 is disable ,1 is enable) \n",ret);
	printf("\n******************end pastar[%d] work status show ******************\n",chip_id);
	/*lint -restore*/

	return MIPI_OK;

}


int pmu_hi6561_read_debug(HI6561_ID_ENUM chip_id,u8 reg_addr)
{
	u8 data=0;
	int retval=0;

	retval = pmu_hi6561_reg_read(reg_addr,&data,chip_id);
	if(MIPI_OK!=retval){
		mipi_print_error("pmu_hi6561_ read data error!chip id:%d,reg addr %d\n",chip_id,reg_addr);
		return retval;
	}

	return data;
}


int pmu_hi6561_write_debug(u8 reg_addr,u8 reg_data,HI6561_ID_ENUM chip_id)
{
	int ret;
	ret=pmu_hi6561_reg_write(reg_addr, reg_data,chip_id);
	if(ret<0)
	{
		mipi_print_error("Error: pastar write data fail!reg:0x%x,data:0x%x,chip_id:%d\n",reg_addr,reg_data);
		return MIPI_ERROR;
	}
	return MIPI_OK;
}
/*lint -restore*/


