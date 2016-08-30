
#ifdef __cplusplus
extern "C"
{
#endif
#include <drv_comm.h>
#include <bsp_shared_ddr.h>
#include <bsp_pa_rf.h>
#include <bsp_regulator.h>
#include <bsp_pmu_hi6561.h>
#include <bsp_pmu.h>
#include <bsp_icc.h>

#ifndef OK
#define OK      0
#endif

#ifndef ERROR
#define ERROR   -1
#endif

/* print func */
#define parf_print_error(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_PARF, "[pa_rf]: <%s> "fmt, __FUNCTION__, ##__VA_ARGS__))
#define parf_print_info(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_INFO, BSP_MODU_PARF, "[pa_rf]: <%s> "fmt, __FUNCTION__, ##__VA_ARGS__))
#define parf_print_always(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_PARF, "[pa_rf]: <%s> "fmt, __FUNCTION__, ##__VA_ARGS__))

#if defined(CONFIG_PASTAR)

#elif defined(CONFIG_PMIC_HI6559)

#define PA_RF_VOLT_ID_MASK  ((1 << PMIC_HI6559_BUCK0) | (1 << PMIC_HI6559_BUCK5) | (1 << PMIC_HI6559_LDO12) |\
                             (1 << PMIC_HI6559_LDO13) | (1 << PMIC_HI6559_LVS07))

typedef void (*pFUNC)(u32);
pFUNC g_pfunc_exc_handle = NULL;  
u32 g_pmu_exc_type = 0;    /* PA/RF�����쳣��¼��0��ʾû���쳣����0��ʾ���쳣 */


#define PA_RF_VOLT_NUM  MODEM_PA1
struct regulator *regulator_pmu[PA_RF_VOLT_NUM]; /* ��PA/RF���硣0/1��PA��,2/3/4��RF�� */

/*
 * ���������PMUоƬ������
 */
#if 0
/*****************************************************************************
 ���� 	: drv_pa_rf_exc_callback_register
 ����	: �ϲ��ṩ�Ļص�����ע��
 ����	: ��
 ���	: ��
 ����   : ��
*****************************************************************************/
void drv_pa_rf_exc_callback_register(pFUNC p)
{
    g_pfunc_exc_handle = p;
}
#endif
/*****************************************************************************
 ���� 	: parf_exc_handler
 ����	: pa/rf�쳣����
 ����	: 
 ���	: ��
 ����   : ��
*****************************************************************************/
s32 parf_exc_handler(u32 channel_id , u32 len, void* context)
{
    s32 ret = ERROR;

    /* ����ICC��Ϣ */
	ret = bsp_icc_read(PA_RF_ICC_CHN_ID, (u8 *)&g_pmu_exc_type, sizeof(u32));
	if(sizeof(u32) != ret)
	{
        parf_print_error("ERROR: bsp_icc_read failed %d\n", ret);
		return ret;
	}

    /* ������pa/rf�ĵ�Դ�쳣����ֱ�ӷ��� */
    g_pmu_exc_type = g_pmu_exc_type & PA_RF_VOLT_ID_MASK;
    if(!g_pmu_exc_type)
    {
        return OK;
    }

    parf_print_error("ERROR: PA/RF power error, volt id %d\n", g_pmu_exc_type);

#if 0
    /* �ϱ��쳣��Ϣ��DSP��δȷ�ϴ˴���ν�������ȷ�Ϻ��޸� */ /* �������HIDS��ӡ��Ϣ����Ҫ��һ��С���� */
    if(g_pfunc_exc_handle)
    {
        (*g_pfunc_exc_handle)(g_pmu_exc_type);
    }
#endif
    return OK;
}

/*****************************************************************************
 �� �� ��  : bsp_pa_rf_init
 ��������  : ��ȡregulator
 �������  : ��
 �������  : ��
 �� �� ֵ  : 0:�ɹ���else - ʧ�ܡ�ʧ��ʱ����ֵ��bit0~bit4��Ӧ��ȡʧ�ܵ�regulator_pmu id
*****************************************************************************/
s32 bsp_pa_rf_init(void)
{
    s32 i = 0;
    s32 ret = OK;
    
    /* ��ȡregulator */
    regulator_pmu[MODEM_PA0] = regulator_get(NULL, VOLT_NAME_PA_VCC);                        /* BUCK0 */
    regulator_pmu[MODEM_RFIC0_ANALOG0] = regulator_get(NULL, VOLT_NAME_RFIC_FRONT_VCC);      /* BUCK5 */
    regulator_pmu[MODEM_RFIC0_ANALOG1] = regulator_get(NULL, VOLT_NAME_RFIC_VCC);            /* LVS07 */
    regulator_pmu[MODEM_FEM0] = regulator_get(NULL, VOLT_NAME_RF_FEM_VCC);                   /* LDO13 */
    regulator_pmu[MODEM_PA_VBIAS0] = regulator_get(NULL, VOLT_NAME_PA_VBIA2_VCC);           /* LDO12 */

    for(i = 0; i < PA_RF_VOLT_NUM; i++)
    {
        if(NULL == regulator_pmu[i])
        {
            parf_print_error("ERROR: regulator_pmu%d get regulator failed %d\n", i);
            ret |= (1 << i);
        }
    }

    if(ret != OK)
    {
        return ret;
    }

    /* �쳣����ע��*/
    ret = bsp_icc_event_register(PA_RF_ICC_CHN_ID, parf_exc_handler, NULL, NULL, NULL);
	if(ret != OK)
	{
        parf_print_error("ERROR: register icc event failed, ret = 0x%x\n", ret);
        return ret;
	}
    
    parf_print_always("pa_rf init OK\n");
    
    return ret;
}


/*****************************************************************************
 �� �� ��  : bsp_pmu_get_pa_powerstatus
 ��������  : ��ȡpa���µ�״̬
 �������  : @modem_id - ����or����(6559ֻ֧�ֵ������˲�������)
 �������  : ��
 �� �� ֵ  : 0 - �ɹ�����0 - ʧ��
*****************************************************************************/
PWC_COMM_STATUS_E bsp_pmu_get_pa_powerstatus(PWC_COMM_MODEM_E modem_id)
{
    s32 status1 = 0, status2 = 0;
    s32 result = 0;

    /* bbstarֻ֧��modem0 */
    if(modem_id != PWC_COMM_MODEM_0)
    {
        parf_print_error("ERROR: invalid modem id %d\n", modem_id);
        return PWC_COMM_STATUS_BUTT;
    }

    status1 = regulator_is_enabled(regulator_pmu[MODEM_PA0]);
    status2 = regulator_is_enabled(regulator_pmu[MODEM_PA_VBIAS0]);

	if(status1 && status2)
    {
		result = PWC_COMM_MODEM_ON;
	}
	else if(0 == (status1 | status2))
    {
		result = PWC_COMM_MODEM_OFF;
	}
	else
    {
		result = PWC_COMM_STATUS_BUTT;
	}

	return result;
}

/*****************************************************************************
 �� �� ��  : bsp_pmu_pa_poweron
 ��������  : ��pa�ϵ�
 �������  : @modem_id - ����or����(6559ֻ֧�ֵ������˲�������)
 �������  : ��
 �� �� ֵ  : 0 - �ɹ�����0 - ʧ��
*****************************************************************************/
s32 bsp_pmu_pa_poweron(PWC_COMM_MODEM_E modem_id)
{
    s32 ret = OK;
    
    /* bbstarֻ֧��modem0 */
    if(modem_id != PWC_COMM_MODEM_0)
    {
        parf_print_error("ERROR: invalid modem id %d\n", modem_id);
        return BSP_PMU_ERROR;
    }
    
    /* ���Ѿ��򿪣����ظ��� */
    if(PWC_COMM_MODEM_ON == bsp_pmu_get_pa_powerstatus(modem_id))
    {
        return ret;
    }

    ret = regulator_enable(regulator_pmu[MODEM_PA0]);
    if(ret)
    {
        parf_print_error("ERROR: enable regulator_pmu%d failed, ret %d \n", MODEM_PA0, ret);
        return ret;
    }    

    ret = regulator_enable(regulator_pmu[MODEM_PA_VBIAS0]);
    if(ret)
    {
        parf_print_error("ERROR: enable regulator_pmu%d failed, ret %d \n", MODEM_PA_VBIAS0, ret);
        return ret;
    } 

    return ret;
}

/*****************************************************************************
 �� �� ��  : bsp_pmu_pa_poweroff
 ��������  : ��pa�µ�
 �������  : @modem_id - ����or����(6559ֻ֧�ֵ������˲�������)
 �������  : ��
 �� �� ֵ  : 0 - �ɹ�����0 - ʧ��
*****************************************************************************/
s32 bsp_pmu_pa_poweroff(PWC_COMM_MODEM_E modem_id)
{
    s32 ret = OK;

    /* bbstarֻ֧��modem0 */
    if(modem_id != PWC_COMM_MODEM_0)
    {
        parf_print_error("ERROR: invalid modem id %d\n", modem_id);
        return BSP_PMU_ERROR;
    }    

    if(PWC_COMM_MODEM_OFF == bsp_pmu_get_pa_powerstatus(modem_id))
    {
        return ret;
    }
    
    ret = regulator_disable(regulator_pmu[MODEM_PA0]);
    if(ret)
    {
        parf_print_error("ERROR: regulator_disable %d failed, ret %d \n", MODEM_PA0, ret);
        return ret;
    }
#if 0
    /* 711 LDO12��USB���ã��˴����ܹص� */
    ret = regulator_disable(regulator_pmu[MODEM_PA_VBIAS0]);
    if(ret)
    {
        parf_print_error("ERROR: regulator_disable %d failed, ret %d \n", MODEM_PA_VBIAS0, ret);
        return ret;
    }
#endif
    return ret;
}

/*****************************************************************************
 �� �� ��  : bsp_pmu_get_rf_powerstatus
 ��������  : ��ȡrf���µ�״̬
 �������  : @modem_id - ����or����(6559ֻ֧�ֵ������˲�������)
 �������  : ��
 �� �� ֵ  : 0 - �ɹ�����0 - ʧ��
*****************************************************************************/
PWC_COMM_STATUS_E bsp_pmu_get_rf_powerstatus(PWC_COMM_MODEM_E modem_id)
{    
    s32 status1 = 0, status2 = 0, status3 = 0;
    s32 result = 0;
    
    /* bbstarֻ֧��modem0 */
    if(modem_id != PWC_COMM_MODEM_0)
    {
        parf_print_error("ERROR: invalid modem id %d\n", modem_id);
        return PWC_COMM_STATUS_BUTT;
    }
    
    status1 = regulator_is_enabled(regulator_pmu[MODEM_RFIC0_ANALOG0]);
    status2 = regulator_is_enabled(regulator_pmu[MODEM_RFIC0_ANALOG1]);
    status3 = regulator_is_enabled(regulator_pmu[MODEM_FEM0]);

	if(status1 && status2 && status3)
    {
		result = PWC_COMM_MODEM_ON;
	}
	else if(0 == (status1 | status2 | status3))
    {
		result = PWC_COMM_MODEM_OFF;
	}
	else
    {
		result = PWC_COMM_STATUS_BUTT;
	}

	return result;
}

/*****************************************************************************
 �� �� ��  : bsp_pmu_rf_poweron
 ��������  : ��rf�ϵ�
 �������  : @modem_id - ����or����(6559ֻ֧�ֵ������˲�������)
 �������  : ��
 �� �� ֵ  : 0 - �ɹ�����0 - ʧ��
*****************************************************************************/
s32 bsp_pmu_rf_poweron(PWC_COMM_MODEM_E modem_id)
{
    s32 ret = OK;

    /* bbstarֻ֧��modem0 */
    if(modem_id != PWC_COMM_MODEM_0)
    {
        parf_print_error("ERROR: invalid modem id %d\n", modem_id);
        return BSP_PMU_ERROR;
    }
    
    if(PWC_COMM_MODEM_ON == bsp_pmu_get_rf_powerstatus(modem_id))
    {
        return ret;
    }
    
    ret = regulator_enable(regulator_pmu[MODEM_RFIC0_ANALOG0]);
    if(ret)
    {
        parf_print_error("ERROR: regulator_enable %d failed, ret %d \n", MODEM_RFIC0_ANALOG0, ret);
        return ret;
    }    

    ret = regulator_enable(regulator_pmu[MODEM_RFIC0_ANALOG1]);
    if(ret)
    {
        parf_print_error("ERROR: regulator_enable %d failed, ret %d \n", MODEM_RFIC0_ANALOG1, ret);
        return ret;
    }  
    
    ret = regulator_enable(regulator_pmu[MODEM_FEM0]);
    if(ret)
    {
        parf_print_error("ERROR: regulator_enable %d failed, ret %d \n", MODEM_FEM0, ret);
        return ret;
    }   

    return ret;
}

/*****************************************************************************
 �� �� ��  : bsp_pmu_rf_poweroff
 ��������  : ��rf�µ�
 �������  : @modem_id - ����or����(6559ֻ֧�ֵ������˲�������)
 �������  : ��
 �� �� ֵ  : 0 - �ɹ�����0 - ʧ��
*****************************************************************************/
s32 bsp_pmu_rf_poweroff(PWC_COMM_MODEM_E modem_id)
{
    s32 ret = OK;

    /* bbstarֻ֧��modem0 */
    if(modem_id != PWC_COMM_MODEM_0)
    {
        parf_print_error("ERROR: invalid modem id %d\n", modem_id);
        return BSP_PMU_ERROR;
    }
    
    if(PWC_COMM_MODEM_OFF == bsp_pmu_get_rf_powerstatus(modem_id))
    {
        return ret;
    }
    
    ret = regulator_disable(regulator_pmu[MODEM_RFIC0_ANALOG0]);
    if(ret)
    {
        parf_print_error("ERROR: regulator_disable %d failed, ret %d \n", MODEM_RFIC0_ANALOG0, ret);
        return ret;
    }
    
    ret = regulator_disable(regulator_pmu[MODEM_RFIC0_ANALOG1]);
    if(ret)
    {
        parf_print_error("ERROR: regulator_disable %d failed, ret %d \n", MODEM_RFIC0_ANALOG1, ret);
        return ret;
    }

    ret = regulator_disable(regulator_pmu[MODEM_FEM0]);
    if(ret)
    {
        parf_print_error("ERROR: regulator_disable %d failed, ret %d \n", MODEM_FEM0, ret);
        return ret;
    }    

    return ret;
}

/*****************************************************************************
 �� �� ��  : drv_pmu_parf_exc_check
 ��������  : ͨ��ģ����PASTAR�Ƿ����쳣�ӿ�
 �������  : modem_id       ����
 �������  : ��
 �� �� ֵ  : 0          û���쳣
             else       �����쳣
*****************************************************************************/
s32 drv_pmu_parf_exc_check(PWC_COMM_MODEM_E modem_id)
{
    /* ������� */
    if(modem_id > PWC_COMM_MODEM_0)
    {
        parf_print_error("ERROR: invalid modem_id %d\n", modem_id);
        return ERROR;
    }

    if(!(g_pmu_exc_type & PA_RF_VOLT_ID_MASK))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/*****************************************************************************
 �� �� ��  : drv_pmu_modem_voltage_set
 ��������  : ͨ��ģ�����õ�ѹ�ӿ�
 �������  : consumer_id     �û�id
             voltage_mv      ���õĵ�ѹֵ����
 �������  : ��
 �� �� ֵ  : 0          ���óɹ�
             else       ����ʧ��
*****************************************************************************/
s32 drv_pmu_modem_voltage_set(EM_MODEM_CONSUMER_ID consumer_id, u32 voltage_mv )
{
    s32 ret = ERROR;
    
    /* ������� */
    if(consumer_id > MODEM_PA_VBIAS0)
    {
        parf_print_error("ERROR: invalid consumer_id %d\n", consumer_id);
        return ERROR;
    }

    ret = regulator_set_voltage(regulator_pmu[consumer_id], voltage_mv * 1000, voltage_mv * 1000);
    if(ret)
    {
        parf_print_error("ERROR: regulator_set_voltage failed, ret = %d\n", ret);
        return ret;
    }

    return OK;
}

/*****************************************************************************
 �� �� ��  : drv_pmu_modem_voltage_get
 ��������  : ͨ��ģ���ȡ��ѹ�ӿ�
 �������  : consumer_id     �û�id
             voltage_mv      ��õĵ�ѹֵ����
 �������  : ��
 �� �� ֵ  : 0          ��ȡ�ɹ�
             else       ��ȡʧ��
*****************************************************************************/
s32 drv_pmu_modem_voltage_get( EM_MODEM_CONSUMER_ID consumer_id, u32 *voltage_mv )
{
    /* ������� */
    if(consumer_id > MODEM_PA_VBIAS0)
    {
        parf_print_error("ERROR: invalid consumer_id %d\n", consumer_id);
        return ERROR;
    }

    if(!voltage_mv)
    {
        parf_print_error("ERROR: null pointer of vlotage_mv\n");
        return ERROR;
    }

    /* ��ȡ��ѹֵ����ת��Ϊmv */
    *voltage_mv = (u32)regulator_get_voltage(regulator_pmu[consumer_id]) / 1000;

    return OK;
}

/*****************************************************************************
 �� �� ��  : drv_pmu_modem_voltage_list
 ��������  : ͨ��ģ���ȡ��ѹ���÷�Χ�ӿ�
 �������  : consumer_id     �û�id
             list            ��ѹ��Χ����
             size            �����С
 �������  : ��
 �� �� ֵ  : 0          ��ȡ�ɹ�
             else       ��ȡʧ��
*****************************************************************************/
s32 drv_pmu_modem_voltage_list(EM_MODEM_CONSUMER_ID consumer_id,u16 **list, u32 *size)
{
    /* �ϲ�û�е��ã��˴�ֱ�ӷ���ERROR */
    return ERROR;
}

/*****************************************************************************
 ����	: drv_pmu_modem_apt_enable
 ����	: ͨ��ģ��ʹ��APT״̬�ӿ�
 ����	: modem_id       ����
          mode_id        ģʽ
 ���	: ��
 ����	: 1  APTʹ��/   0    APTδʹ��/  -1    ��ȡʧ��
*****************************************************************************/
s32 drv_pmu_modem_apt_enable(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id)
{
    /* ������� */
    if(modem_id > PWC_COMM_MODEM_0)
    {
        parf_print_error("ERROR: invalid modem_id %d\n", modem_id);
        return ERROR;
    }
    
    if(mode_id >= PWC_COMM_MODE_BUTT)
    {
        parf_print_error("ERROR: invalid mode_id %d\n", mode_id);
        return ERROR;
    }

    return bsp_pmu_apt_enable();
}

/*****************************************************************************
 ����	: drv_pmu_modem_apt_disable
 ����	: ͨ��ģ��ȥʹ��APT״̬�ӿ�
 ����	: modem_id       ����
          mode_id        ģʽ
 ���	: ��
 ����	: 1  APTʹ��/   0    APTδʹ��/  -1    ��ȡʧ��
*****************************************************************************/
s32 drv_pmu_modem_apt_disable(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id)
{
    /* ������� */
    if(modem_id > PWC_COMM_MODEM_0)
    {
        parf_print_error("ERROR: invalid modem_id %d\n", modem_id);
        return ERROR;
    }
    
    if(mode_id >= PWC_COMM_MODE_BUTT)
    {
        parf_print_error("ERROR: invalid mode_id %d\n", mode_id);
        return ERROR;
    }
    
    return bsp_pmu_apt_disable();
}

/*****************************************************************************
 ����	: drv_pmu_modem_apt_status_get
 ����	: ͨ��ģ���ȡ��ǰAPT״̬�ӿ�
 ����	: modem_id       ����
          mode_id        ģʽ
 ���	: ��
 ����	: 1  APTʹ��/   0    APTδʹ��/  -1    ��ȡʧ��
*****************************************************************************/
s32 drv_pmu_modem_apt_status_get(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id)
{
    /* ������� */
    if(modem_id > PWC_COMM_MODEM_0)
    {
        parf_print_error("ERROR: invalid modem_id %d\n", modem_id);
        return ERROR;
    }
    
    if(mode_id >= PWC_COMM_MODE_BUTT)
    {
        parf_print_error("ERROR: invalid mode_id %d\n", mode_id);
        return ERROR;
    }
    
    return bsp_pmu_apt_status_get();
}

/*****************************************************************************
 ���� 	: drv_pmu_modem_mode_config
 ����	: ͨ��ģ������Gģ��Wģ�ӿ�
 ����	: modem_id       ����
          mode_id        ģʽ
 ���	: ��
 ����  : 0         ���óɹ�/  else      ����ʧ��
*****************************************************************************/
s32 drv_pmu_modem_mode_config(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id)
{
    s32 ret = ERROR;
    
    /* ������� */
    if(modem_id > PWC_COMM_MODEM_0)
    {
        parf_print_error("ERROR: invalid modem_id %d\n", modem_id);
        return ERROR;
    }
    
    if(mode_id >= PWC_COMM_MODE_BUTT)
    {
        parf_print_error("ERROR: invalid mode_id %d\n", mode_id);
        return ERROR;
    }
    
	switch(mode_id){
		case PWC_COMM_MODE_LTE :
    	case PWC_COMM_MODE_TDS :
		case PWC_COMM_MODE_WCDMA :
			/* ����buck pa�ĵ�ѹΪ3.3v */
			ret|= regulator_set_voltage(regulator_pmu[MODEM_PA0],3300000,3300000);

			/* �ر�APT */
			ret |= bsp_pmu_apt_disable();
			break;
		case PWC_COMM_MODE_GSM:

			/* ����buck pa�ĵ�ѹΪ3.7v */
			ret|= regulator_set_voltage(regulator_pmu[MODEM_PA0],3700000,3700000);

			/* �ر�APT */
			ret |= bsp_pmu_apt_disable();
			break;

		default:
			;

	}

	return ret;
}

#else

/*
 * �����Ǵ�׮�ӿڣ�ֻ�е�PASTAR��PMU��������ʱ���Ż��õ���Щ�ӿ�
 */

/*****************************************************************************
 �� �� ��  : dummy_modem_exc_check
 ��������  : ͨ��ģ����PASTAR�Ƿ����쳣�ӿڴ�׮
 �������  : modem_id       ����
 �������  : ��
 �� �� ֵ  : 0          û���쳣
             else       �����쳣
*****************************************************************************/
s32 dummy_modem_exc_check(PWC_COMM_MODEM_E modem_id)
{
	parf_print_error("ERROR: pa_rf didn't build all function!\n");
	return (s32)ERROR;
}

/*****************************************************************************
 �� �� ��  : dummy_modem_voltage_set
 ��������  : ͨ��ģ�����õ�ѹ�ӿڴ�׮
 �������  : consumer_id     �û�id
             voltage_mv      ���õĵ�ѹֵ����
 �������  : ��
 �� �� ֵ  : 0          ���óɹ�
             else       ����ʧ��
*****************************************************************************/
s32 dummy_modem_voltage_set( EM_MODEM_CONSUMER_ID consumer_id, u32 voltage_mv )
{
	parf_print_error("ERROR: pa_rf didn't build all function!\n");
    return (s32)ERROR;
}

/*****************************************************************************
 �� �� ��  : dummy_modem_voltage_get
 ��������  : ͨ��ģ���ȡ��ѹ�ӿڴ�׮
 �������  : consumer_id     �û�id
             voltage_mv      ��õĵ�ѹֵ����
 �������  : ��
 �� �� ֵ  : 0          ��ȡ�ɹ�
             else       ��ȡʧ��
*****************************************************************************/
s32 dummy_modem_voltage_get( EM_MODEM_CONSUMER_ID consumer_id, u32 *voltage_mv )
{
	parf_print_error("ERROR: pa_rf didn't build all function!\n");
    return (s32)ERROR;
}

/*****************************************************************************
 �� �� ��  : dummy_modem_voltage_list
 ��������  : ͨ��ģ���ȡ��ѹ���÷�Χ�ӿڴ�׮
 �������  : consumer_id     �û�id
             list            ��ѹ��Χ����
             size            �����С
 �������  : ��
 �� �� ֵ  : 0          ��ȡ�ɹ�
             else       ��ȡʧ��
*****************************************************************************/
s32 dummy_modem_voltage_list(EM_MODEM_CONSUMER_ID consumer_id,u16 **list, u32 *size)
{
	parf_print_error("ERROR: pa_rf didn't build all function!\n");
    return (s32)ERROR;
}

/*****************************************************************************
 �� �� ��  : dummy_modem_apt_enable
 ��������  : ͨ��ģ��ʹ��APT�ӿڴ�׮
 �������  : modem_id       ����
             mode_id        ͨ��ģʽ
 �������  : ��
 �� �� ֵ  : 0         ���óɹ�
             else      ����ʧ��
*****************************************************************************/
s32 dummy_modem_apt_enable(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id)
{
	parf_print_error("ERROR: pa_rf didn't build all function!\n");
    return (s32)ERROR;
}

/*****************************************************************************
 �� �� ��  : dummy_modem_apt_disable
 ��������  : ͨ��ģ��ȥʹ��APT�ӿڴ�׮
 �������  : modem_id       ����
             mode_id        ͨ��ģʽ
 �������  : ��
 �� �� ֵ  : 0         ���óɹ�
             else      ����ʧ��
*****************************************************************************/
s32 dummy_modem_apt_disable(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id)
{
	parf_print_error("ERROR: pa_rf didn't build all function!\n");
    return (s32)ERROR;
}

/*****************************************************************************
 �� �� ��  : dummy_modem_apt_status_get
 ��������  : ͨ��ģ���ȡ��ǰAPT״̬�ӿڴ�׮
 �������  : modem_id       ����
             mode_id        ͨ��ģʽ
 �������  : ��
 �� �� ֵ  : 1     APTʹ��
             		   0    APTδʹ��
             		  -1    ��ȡʧ��
*****************************************************************************/
s32 dummy_modem_apt_status_get(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id)
{
	parf_print_error("ERROR: pa_rf didn't build all function!\n");
    return (s32)ERROR;
}

/*****************************************************************************
 �� �� ��  : dummy_modem_mode_config
 ��������  : ͨ��ģ������Gģ��Wģ�ӿڴ�׮
 �������  : modem_id       ����
             mode_id        ͨ��ģʽ
 �������  : ��
 �� �� ֵ  : 0         ���óɹ�
             else      ����ʧ��
*****************************************************************************/
s32 dummy_modem_mode_config(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id)
{
	parf_print_error("ERROR: pa_rf didn't build all function!\n");
    return (s32)ERROR;
}

/*****************************************************************************
 �� �� ��  : dummy_pmu_pa_poweron
 ��������  : PA �ϵ��׮
 �������  : ��

 �������  : ��
 �� �� ֵ  : pa��Դ����״̬
*****************************************************************************/
int dummy_pmu_pa_poweron(PWC_COMM_MODEM_E modem_id)
{
	parf_print_error("ERROR: pa_rf didn't build all function!\n");
	return OK;
}

/*****************************************************************************
 �� �� ��  : dummy_pmu_lpa_poweroff
 ��������  : PA �µ��׮
 �������  : ��

 �������  : ��
 �� �� ֵ  : pa ��Դ����״̬
*****************************************************************************/
int dummy_pmu_pa_poweroff(PWC_COMM_MODEM_E modem_id)
{
	parf_print_error("ERROR: pa_rf didn't build all function!\n");
	return OK;
}

/*****************************************************************************
 �� �� ��  : dummy_pmu_rf_poweroff
 ��������  : RF �µ��׮
 �������  : ��

 �������  : ��
 �� �� ֵ  : rf��Դ����״̬
*****************************************************************************/
int dummy_pmu_rf_poweroff(PWC_COMM_MODEM_E modem_id)
{
	parf_print_error("ERROR: pa_rf didn't build all function!\n");
	return OK;
}

/*****************************************************************************
 ����	: dummy_pmu_rf_poweron
 ����	: RF �ϵ��׮
 ����	: ��
 ���	: ��
 ���� 	: rf��Դ����״̬
*****************************************************************************/
int dummy_pmu_rf_poweron(PWC_COMM_MODEM_E modem_id)
{
	parf_print_error("ERROR: pa_rf didn't build all function!\n");
	return OK;
}

/*****************************************************************************
 �� �� ��  : dummy_pmu_get_rf_powerstatus
 ��������  : RF��Դ״̬��ѯ��׮
 �������  : 
 �������  :
 �� �� ֵ  : rf��Դ����״̬
*****************************************************************************/
PWC_COMM_STATUS_E dummy_pmu_get_rf_powerstatus(PWC_COMM_MODEM_E modem_id)
{
	parf_print_error("ERROR: pa_rf didn't build all function!\n");
	return PWC_COMM_MODE_BUTT;
}

/*****************************************************************************
 �� �� ��  :dummy_pmu_get_rf_powerstatus
 ��������  :RF��Դ״̬��ѯ��׮
 �������  :
 �������  :
 �� �� ֵ  : pa��Դ����״̬
*****************************************************************************/
PWC_COMM_STATUS_E dummy_pmu_get_pa_powerstatus(PWC_COMM_MODEM_E modem_id)
{
	parf_print_error("ERROR: pa_rf didn't build all function!\n");
	return PWC_COMM_MODE_BUTT;
}

#endif

struct pmu_parf_ops pmu_parf_ops = { 
#if defined(CONFIG_PASTAR)  /* pastarоƬӦ���������֧ */
    .pmu_pa_power_on = bsp_pmu_hi6561_pa_poweron,
    .pmu_pa_power_off = bsp_pmu_hi6561_pa_poweroff,
    .pmu_pa_power_status = bsp_pmu_hi6561_get_pa_powerstatus,
    .pmu_rf_power_on = bsp_pmu_hi6561_rf_poweron,
    .pmu_rf_power_off = bsp_pmu_hi6561_rf_poweroff,
    .pmu_rf_power_status = bsp_pmu_hi6561_get_rf_powerstatus,
    .drv_pastar_exc_check = drv_pmu_hi6561_exc_check,
    .drv_modem_voltage_set = drv_pmu_hi6561_voltage_set,
    .drv_modem_voltage_get = drv_pmu_hi6561_voltage_get,
    .drv_modem_voltage_list = drv_pmu_hi6561_voltage_list,
    .drv_modem_apt_enable = drv_pmu_hi6561_apt_enable,
    .drv_modem_apt_disable = drv_pmu_hi6561_apt_disable,
    .drv_modem_apt_status_get = drv_pmu_hi6561_apt_status_get,
    .drv_modem_mode_config = drv_pmu_hi6561_mode_config,

#elif defined(CONFIG_PMIC_HI6559) /* PMUоƬӦ���������֧ */
    .pmu_pa_power_on = bsp_pmu_pa_poweron,
    .pmu_pa_power_off = bsp_pmu_pa_poweroff,
    .pmu_pa_power_status = bsp_pmu_get_pa_powerstatus,
    .pmu_rf_power_on = bsp_pmu_rf_poweron,
    .pmu_rf_power_off = bsp_pmu_rf_poweroff,
    .pmu_rf_power_status = bsp_pmu_get_rf_powerstatus,
    .drv_pastar_exc_check = drv_pmu_parf_exc_check,
    .drv_modem_voltage_set = drv_pmu_modem_voltage_set,
    .drv_modem_voltage_get = drv_pmu_modem_voltage_get,
    .drv_modem_voltage_list = drv_pmu_modem_voltage_list,
    .drv_modem_apt_enable = drv_pmu_modem_apt_enable,
    .drv_modem_apt_disable = drv_pmu_modem_apt_disable,
    .drv_modem_apt_status_get = drv_pmu_modem_apt_status_get,
    .drv_modem_mode_config = drv_pmu_modem_mode_config,

#else /* û��pastarҲû��pmu�����������֧ */
    .pmu_pa_power_on = dummy_pmu_pa_poweron,
    .pmu_pa_power_off = dummy_pmu_pa_poweroff,
    .pmu_pa_power_status = dummy_pmu_get_pa_powerstatus,
    .pmu_rf_power_on = dummy_pmu_rf_poweron,
    .pmu_rf_power_off = dummy_pmu_rf_poweroff,
    .pmu_rf_power_status = dummy_pmu_get_rf_powerstatus,
    .drv_pastar_exc_check = dummy_modem_exc_check,
    .drv_modem_voltage_set = dummy_modem_voltage_set,
    .drv_modem_voltage_get = dummy_modem_voltage_get,
    .drv_modem_voltage_list = dummy_modem_voltage_list,
    .drv_modem_apt_enable = dummy_modem_apt_enable,
    .drv_modem_apt_disable = dummy_modem_apt_disable,
    .drv_modem_apt_status_get = dummy_modem_apt_status_get,
    .drv_modem_mode_config = dummy_modem_mode_config,

#endif
};

/*
 * �����Ƕ����ṩ�Ľӿڣ�����DRV�ӿں�dpm���̽ӿ�
 */

/*****************************************************************************
 �� �� ��  : DRV_PASTAR_EXC_CHECK
 ��������  : ͨ��ģ����PASTAR�Ƿ����쳣�ӿ�
 �������  : modem_id       ����
 �������  : ��
 �� �� ֵ  : 0          û���쳣
             else       �����쳣
*****************************************************************************/
s32 DRV_PASTAR_EXC_CHECK(PWC_COMM_MODEM_E modem_id)
{
    return pmu_parf_ops.drv_pastar_exc_check(modem_id);
}

/*****************************************************************************
 �� �� ��  : DRV_MODEM_VOLTAGE_SET
 ��������  : ͨ��ģ�����õ�ѹ�ӿ�
 �������  : consumer_id     �û�id
             voltage_mv      ���õĵ�ѹֵ����
 �������  : ��
 �� �� ֵ  : 0          ���óɹ�
             else       ����ʧ��
*****************************************************************************/
s32 DRV_MODEM_VOLTAGE_SET( EM_MODEM_CONSUMER_ID consumer_id, u32 voltage_mv )
{
    return pmu_parf_ops.drv_modem_voltage_set(consumer_id,voltage_mv);
}

/*****************************************************************************
 �� �� ��  : DRV_MODEM_VOLTAGE_GET
 ��������  : ͨ��ģ���ȡ��ѹ�ӿ�
 �������  : consumer_id     �û�id
             voltage_mv      ��õĵ�ѹֵ����
 �������  : ��
 �� �� ֵ  : 0          ��ȡ�ɹ�
             else       ��ȡʧ��
*****************************************************************************/
s32 DRV_MODEM_VOLTAGE_GET( EM_MODEM_CONSUMER_ID consumer_id, u32 *voltage_mv )
{
    return pmu_parf_ops.drv_modem_voltage_get(consumer_id, voltage_mv);
}

/*****************************************************************************
 �� �� ��  : DRV_MODEM_VOLTAGE_LIST
 ��������  : ͨ��ģ���ȡ��ѹ���÷�Χ�ӿ�
 �������  : consumer_id     �û�id
             list            ��ѹ��Χ����
             size            �����С
 �������  : ��
 �� �� ֵ  : 0          ��ȡ�ɹ�
             else       ��ȡʧ��
*****************************************************************************/
s32 DRV_MODEM_VOLTAGE_LIST(EM_MODEM_CONSUMER_ID consumer_id,u16 **list, u32 *size)
{
    return pmu_parf_ops.drv_modem_voltage_list(consumer_id, list, size);
}

/*****************************************************************************
 ����	: DRV_MODEM_APT_STATUS_GET
 ����	: ͨ��ģ��ʹ��APT״̬�ӿ�
 ����	: modem_id       ����
 ���	: ��
 ����	: 1  APTʹ��/   0    APTδʹ��/  -1    ��ȡʧ��
*****************************************************************************/
s32 DRV_MODEM_APT_ENABLE(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id)
{
    return pmu_parf_ops.drv_modem_apt_enable(modem_id, mode_id);
}

/*****************************************************************************
 ����	: DRV_MODEM_APT_STATUS_GET
 ����	: ͨ��ģ��ȥʹ��APT״̬�ӿ�
 ����	: modem_id       ����
 ���	: ��
 ����	: 1  APTʹ��/   0    APTδʹ��/  -1    ��ȡʧ��
*****************************************************************************/
s32 DRV_MODEM_APT_DISABLE(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id)
{
    return pmu_parf_ops.drv_modem_apt_disable(modem_id, mode_id);
}

/*****************************************************************************
 ����	: DRV_MODEM_APT_STATUS_GET
 ����	: ͨ��ģ���ȡ��ǰAPT״̬�ӿ�
 ����	: modem_id       ����
 ���	: ��
 ����	: 1  APTʹ��/   0    APTδʹ��/  -1    ��ȡʧ��
*****************************************************************************/
s32 DRV_MODEM_APT_STATUS_GET(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id)
{
    return pmu_parf_ops.drv_modem_apt_status_get(modem_id, mode_id);
}

/*****************************************************************************
 ���� 	: bsp_pmu_hi6561_pa_poweron
 ����	: ͨ��ģ������Gģ��Wģ�ӿ�
 ����	: modem_id       ����
 ���	: ��
 ����  : 0         ���óɹ�/  else      ����ʧ��
*****************************************************************************/
s32 DRV_MODEM_MODE_CONFIG(PWC_COMM_MODEM_E modem_id, PWC_COMM_MODE_E mode_id)
{
    return pmu_parf_ops.drv_modem_mode_config(modem_id, mode_id);
}

/*****************************************************************************
 �� �� ��  : bsp_pa_poweron
 ��������  : ��pa�ϵ�
 �������  : @modem_id - 0:���� 1:����
 �������  : ��
 �� �� ֵ  : 0 - �ɹ�����0 - ʧ��
*****************************************************************************/
s32 bsp_pa_poweron(PWC_COMM_MODEM_E modem_id)
{
    return  pmu_parf_ops.pmu_pa_power_on(modem_id); 
}

/*****************************************************************************
 �� �� ��  : bsp_pa_poweroff
 ��������  : ��pa�µ�
 �������  : @modem_id - 0:���� 1:����
 �������  : ��
 �� �� ֵ  : 0 - �ɹ�����0 - ʧ��
*****************************************************************************/
s32 bsp_pa_poweroff(PWC_COMM_MODEM_E modem_id)
{
    return  pmu_parf_ops.pmu_pa_power_off(modem_id); 
}

/*****************************************************************************
 �� �� ��  : bsp_get_pa_powerstatus
 ��������  : ��ȡpa���µ�״̬
 �������  : @modem_id - 0:���� 1:����
 �������  : ��
 �� �� ֵ  : 0 - �ɹ�����0 - ʧ��
*****************************************************************************/
PWC_COMM_STATUS_E bsp_get_pa_powerstatus(PWC_COMM_MODEM_E modem_id)
{
    return  pmu_parf_ops.pmu_pa_power_status(modem_id); 
}

/*****************************************************************************
 �� �� ��  : bsp_rf_poweron
 ��������  : ��rf�ϵ�
 �������  : @modem_id - 0:���� 1:����
 �������  : ��
 �� �� ֵ  : 0 - �ɹ�����0 - ʧ��
*****************************************************************************/
s32 bsp_rf_poweron(PWC_COMM_MODEM_E modem_id)
{
    return  pmu_parf_ops.pmu_rf_power_on(modem_id); 
}

/*****************************************************************************
 �� �� ��  : bsp_rf_poweroff
 ��������  : ��rf�µ�
 �������  : @modem_id - 0:���� 1:����
 �������  : ��
 �� �� ֵ  : 0 - �ɹ�����0 - ʧ��
*****************************************************************************/
s32 bsp_rf_poweroff(PWC_COMM_MODEM_E modem_id)
{
    return  pmu_parf_ops.pmu_rf_power_off(modem_id); 
}

/*****************************************************************************
 �� �� ��  : bsp_get_rf_powerstatus
 ��������  : ��ȡrf���µ�״̬
 �������  : @modem_id - 0:���� 1:����
 �������  : ��
 �� �� ֵ  : 0 - �ɹ�����0 - ʧ��
*****************************************************************************/
PWC_COMM_STATUS_E bsp_get_rf_powerstatus(PWC_COMM_MODEM_E modem_id)
{
    return  pmu_parf_ops.pmu_rf_power_status(modem_id); 
}

#ifdef __cplusplus
}
#endif
