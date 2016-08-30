
#include <boot/boot.h>
#include <soc_memmap.h>
#include <osl_types.h>

#include <hi_bbstar.h>
#include <bsp_shared_ddr.h>
#include <drv_nv_id.h>
#include <drv_nv_def.h>

//#include <bsp_hardtimer.h>
#include <nv_boot.h>
#include <pmu.h>
#include "pmic_hi6559.h"

/*PMU��ʼ������nvֵ��ֻ��fastboot����Ҫ*/
PMU_INIT_NV_STRU    pmu_init_config;
extern void hi6559_volttable_copytoddr(void);

/* BUCK��ѹ���� */
typedef struct
{
    s32 volt_base;
    s32 volt_step;

}PMIC_HI6559_BUCK_ADJ;

/* ��λ��uv */
static const PMIC_HI6559_BUCK_ADJ hi6559_buck_volt[PMIC_HI6559_BUCK_MAX + 1] =
{
    { 900000, 50000}, /* BUCK0 */
    { 700000,  8000}, /* BUCK3 */
    {1200000, 25000}, /* BUCK4 */
    {      0,     0}, /* BUCK5��Ϊ��λ���˴���׮ */
    {1800000, 25000}, /* BUCK6 */
};

#define HI6559_LVS_VOLTAGE 1800000  /* LVS��ѹΪ�̶�ֵ1800000uV */

/*****************************************************************************
* �� �� ��  : hi6559_volt_para_check
*
* ��������  : ����ѹԴ�Ƿ�Ϸ�
*
* �������  : @volt_id�������ĵ�ѹԴ
*
* �������  : ��
*
* �� �� ֵ  : BSP_PMU_OK:�Ϸ�;   BSP_PMU_ERROR:���Ϸ�
*****************************************************************************/
static __inline__ s32 hi6559_volt_para_check(s32 volt_id)
{
    /* ��Ч�Լ��*/
    if ((PMIC_HI6559_VOLT_MAX < volt_id) || (PMIC_HI6559_VOLT_MIN > volt_id))
    {
        pmic_print_error("PMIC doesn't have volt %d!\n",volt_id);
        pmic_print_error("the volt you can use in this PMIC is from %d to %d,please check!\n", PMIC_HI6559_VOLT_MIN, PMIC_HI6559_VOLT_MAX);
        return BSP_PMU_ERROR;
    }
    else
    {
        return BSP_PMU_OK;
    }
}

/*****************************************************************************
* �� �� ��  : hi6559_volt_table_check
*
* ��������  : ����ѹԴ���Ա��Ƿ����
*
* �������  : @hi6559_volt_table ��ѹԴ���Ա��ַ
*
* �������  : ��
*
* �� �� ֵ  : BSP_PMU_OK:����;   BSP_PMU_ERROR:������
*****************************************************************************/
static __inline__ s32 hi6559_volt_table_check(PMIC_HI6559_VLTGS_TABLE *hi6559_volt_table)
{
    if((SHM_PMU_VOLTTABLE_MAGIC_START_DATA == hi6559_volt_table->magic_start) && \
        (SHM_PMU_VOLTTABLE_MAGIC_END_DATA == hi6559_volt_table->magic_end))
    {
        return BSP_PMU_OK;
    }
    else
    {
        pmic_print_error("ERROR: volt_table error !\n");
        return BSP_PMU_ERROR;
    }
}

/*****************************************************************************
* �� �� ��  : hi6559_reg_write
*
* ��������  : ��pmuоƬ�Ĵ�����д����
*
* �������  : u16 addr����д��ļĴ�����ַ
*             u8 u8Data����д�������
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
void bsp_hi6559_reg_write( u16 addr, u8 value)
{
    writel((u32) value,(u32)(HI_PMUSSI0_REGBASE_ADDR_VIRT + (addr << 2)));
}

/*****************************************************************************
* �� �� ��  : hi6559_reg_read
*
* ��������  : ��pmuоƬ�Ĵ����Ķ�����
*
* �������  : u16 addr����д��ļĴ�����ַ
*             u8 u8Data������������
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
void  bsp_hi6559_reg_read( u16 addr, u8 *pValue)
{
      *pValue = (u8)readl((u32)(HI_PMUSSI0_REGBASE_ADDR_VIRT + (addr << 2)));
}

/*****************************************************************************
* �� �� ��  : hi6559_reg_write_mask
*
* ��������  : ��pmuоƬ�Ĵ�����ĳЩbitдֵ���磬��Ĵ���0x20��bit3д��1����bsp_hi6559_reg_write_mask(0x20, 1, 8)
*
* �������  : u16 addr����д��ļĴ�����ַ
*             u8 u8Data������������
*             u8 mask: ��д��bit��mask
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
void bsp_hi6559_reg_write_mask(u16 addr, u8 value, u8 mask)
{
    u8 reg_tmp = 0;

    bsp_hi6559_reg_read(addr, &reg_tmp);
    reg_tmp &= ~mask;
    reg_tmp |= value;
    bsp_hi6559_reg_write(addr, reg_tmp);
}

/*****************************************************************************
 �� �� ��  : bsp_hi6559_volt_enable
 ��������  : ʹ�ܵ�Դvolt_id
 �������  : @volt_id ��Դid
 �������  : ��
 �� �� ֵ  : BSP_PMU_OK: �ɹ��� else:ʧ��
*****************************************************************************/
s32 bsp_hi6559_volt_enable(s32 volt_id)
{
    PMIC_HI6559_VLTGS_TABLE *hi6559_volt_table = (PMIC_HI6559_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6559_VLTGS_ATTR *hi6559_volt = NULL;
    s32 iret = BSP_PMU_OK;
    u8 regval = 0;

    /* ������Ч�Լ�� */
    iret = hi6559_volt_para_check(volt_id);
    if(BSP_PMU_OK != iret)
    {
        return BSP_PMU_PARA_ERROR;
    }
    
    /* ��ѹ���Ա�ȫ�Լ�� */
    iret = hi6559_volt_table_check(hi6559_volt_table);
    if(BSP_PMU_OK != iret)
    {
        return  BSP_PMU_VOLTTABLE_ERROR;
    }
    
    hi6559_volt = &hi6559_volt_table->hi6559_volt_attr[volt_id];
    
    /* д1ʹ�ܣ�����LDO9��LDO11��ʹ����ͬһ���Ĵ����ϣ�Ҫ�ȶ���д�������ֱ��д */
    if(PMIC_HI6559_LDO09 == volt_id)
    {
        bsp_hi6559_reg_read(hi6559_volt->enable_reg_addr, &regval);
        regval |= (u8)((u32)0x3 << hi6559_volt->enable_bit_offset); /* ʹ��LDO9��ͬʱ��ʹ��SIM��ת����· */
        bsp_hi6559_reg_write(hi6559_volt->enable_reg_addr, regval);
        
        /* ����sim�������������ã����ڿ�����Դ�󣬷�����ë�� */
        bsp_hi6559_reg_write(HI6559_SIM_CFG_OFFSET, 0x10);
    }
    else if(PMIC_HI6559_LDO11 == volt_id)
    {
        bsp_hi6559_reg_read(hi6559_volt->enable_reg_addr, &regval);
        regval |= (u8)((u32)0x1 << hi6559_volt->enable_bit_offset);
        bsp_hi6559_reg_write(hi6559_volt->enable_reg_addr, regval);
    }
    else
    {
        regval = (u8)((u32)0x1 << hi6559_volt->enable_bit_offset);
        bsp_hi6559_reg_write(hi6559_volt->enable_reg_addr, regval);
    }    

    return BSP_PMU_OK;

}

/*****************************************************************************
 �� �� ��  : bsp_hi6559_volt_disable
 ��������  : ��ֹ��Դvolt_id
 �������  : @volt_id ��Դid
 �������  : ��
 �� �� ֵ  : BSP_PMU_OK: �ɹ��� else:ʧ��
*****************************************************************************/
s32 bsp_hi6559_volt_disable(s32 volt_id)
{
    PMIC_HI6559_VLTGS_TABLE *hi6559_volt_table = (PMIC_HI6559_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6559_VLTGS_ATTR *hi6559_volt = NULL;
    s32 iret = BSP_PMU_OK;
    u8 regval = 0;

    /* ������Ч�Լ�� */
    iret = hi6559_volt_para_check(volt_id);
    if(BSP_PMU_OK != iret)
    {
        return BSP_PMU_PARA_ERROR;
    }
    
    /* ��ѹ���Ա�ȫ�Լ�� */
    iret = hi6559_volt_table_check(hi6559_volt_table);
    if(BSP_PMU_OK != iret)
    {
        return  BSP_PMU_VOLTTABLE_ERROR;
    }
    
    hi6559_volt = &hi6559_volt_table->hi6559_volt_attr[volt_id];

    /* LDO9/11��д0�رգ�����ͬһ���Ĵ����ϣ�Ҫ�ȶ���д�������ֱ��д�Ҷ���д1�ر� */
    if(PMIC_HI6559_LDO09 == volt_id)
    {
        bsp_hi6559_reg_write_mask(HI6559_SIM_CFG_OFFSET,0x08,0x18);
        bsp_hi6559_reg_read(hi6559_volt->disable_reg_addr, &regval);
        regval &= ~(u8)((u32)0x3 << hi6559_volt->disable_bit_offset);
        bsp_hi6559_reg_write(hi6559_volt->disable_reg_addr, regval);
    }
    else if(PMIC_HI6559_LDO11 == volt_id)
    {
        bsp_hi6559_reg_read(hi6559_volt->disable_reg_addr, &regval);
        regval &= ~(u8)((u32)0x1 << hi6559_volt->disable_bit_offset);
        bsp_hi6559_reg_write(hi6559_volt->disable_reg_addr, regval);
    }
    else
    {
        regval = (u8)((u32)0x1 << hi6559_volt->disable_bit_offset);
        bsp_hi6559_reg_write(hi6559_volt->disable_reg_addr, regval);
    }

    return BSP_PMU_OK;

}

/*****************************************************************************
 �� �� ��  : bsp_hi6559_volt_is_enabled
 ��������  : ��ѯĳ·��ѹԴ�Ƿ�����
 �������  : volt_id:��ѹԴid��
 �������  : ��
 �� �� ֵ  : 0:δ������else:����
*****************************************************************************/
s32 bsp_hi6559_volt_is_enabled(s32 volt_id)
{
    PMIC_HI6559_VLTGS_TABLE *hi6559_volt_table = (PMIC_HI6559_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6559_VLTGS_ATTR *hi6559_volt;
    s32 iret = BSP_PMU_OK;
    u8 regval = 0;

    /* ������Ч�Լ�� */
    iret = hi6559_volt_para_check(volt_id);
    if(BSP_PMU_OK != iret)
    {
        return BSP_PMU_PARA_ERROR;
    }
    /* ��ѹ���Ա�ȫ�Լ�� */
    iret = hi6559_volt_table_check(hi6559_volt_table);
    if(BSP_PMU_OK != iret)
    {
        return  BSP_PMU_VOLTTABLE_ERROR;
    }
    
    hi6559_volt = &hi6559_volt_table->hi6559_volt_attr[volt_id];

    bsp_hi6559_reg_read(hi6559_volt->is_enabled_reg_addr, &regval);

    /* ����״̬bitΪ1ʱ����ʾ��ʹ�� */
    return (regval & ((u32)0x1 << hi6559_volt->is_enabled_bit_offset));
}

/*****************************************************************************
 �� �� ��  : bsp_hi6559_volt_get_voltage
 ��������  : ��ȡ��ѹԴvolt_id�ĵ�ѹֵ
 �������  : volt_id:��ѹԴid��
 �������  : ��
 �� �� ֵ  : ��ѹֵ
*****************************************************************************/
s32 bsp_hi6559_volt_get_voltage(s32 volt_id)
{
    PMIC_HI6559_VLTGS_TABLE *hi6559_volt_table = (PMIC_HI6559_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6559_VLTGS_ATTR *hi6559_volt;
    s32 voltage = 0;
    s32 iret  = BSP_PMU_OK;
    u8 regval = 0;
    u8 vltg = 0;

    /* ������Ч�Լ�� */
    iret = hi6559_volt_para_check(volt_id);
    if(BSP_PMU_OK != iret)
    {
        return BSP_PMU_PARA_ERROR;
    }

    /*DDR�е�ѹ���Ա�ȫ�Լ��*/
    iret = hi6559_volt_table_check(hi6559_volt_table);
    if(BSP_PMU_OK != iret)
    {
        return  BSP_PMU_VOLTTABLE_ERROR;
    }

    /*LVS�ĵ�ѹĬ����1.8v�����ܸı�*/
    if((PMIC_HI6559_LVS_MIN <= volt_id ) && (PMIC_HI6559_LVS_MAX >= volt_id))
    {
        pmic_print_info("volt_id %d's voltage is  %d uV!\n", volt_id, HI6559_LVS_VOLTAGE);
        return HI6559_LVS_VOLTAGE;
    }

    hi6559_volt = (PMIC_HI6559_VLTGS_ATTR *)&(hi6559_volt_table->hi6559_volt_attr[volt_id]);

    bsp_hi6559_reg_read(hi6559_volt->voltage_reg_addr, &regval);

    /* ��ȡ��ѹ��λֵ */
    vltg = (regval & hi6559_volt->voltage_bit_mask) >> hi6559_volt->voltage_bit_offset;

    /* LDO��buck5�ĵ�ѹֵΪ��λ������buck���� */
    if((PMIC_HI6559_BUCK_MAX >= volt_id) && (PMIC_HI6559_BUCK5 != volt_id))
    {
        voltage = (hi6559_buck_volt[volt_id].volt_base + vltg * hi6559_buck_volt[volt_id].volt_step);
    }
    else
    {
        voltage = hi6559_volt->voltage_list[vltg];
    }

    pmic_print_info("volt_id %d's voltage is  %d uV!\n", volt_id, voltage);

    return voltage;

}

/*****************************************************************************
 �� �� ��  : bsp_hi6559_volt_set_voltage
 ��������  : ���õ�ѹԴvolt_id�ĵ�ѹֵ������Ϊ[min_uV, max_uV]�����ֵ���ɣ����������û�кϷ�ֵ��ʧ�ܷ���
 �������  : volt_id: Ҫ���õĵ�Դ���
             min_uV: ��С�Ϸ���ѹֵ
             max_uV: ���Ϸ���ѹֵ
 �������  : @selector: ʵ�����õ�ѹֵ�ĵ�λ
 �� �� ֵ  : BSP_PMU_OK: �ɹ��� else:ʧ��
 ��ע˵��  : Acore��Ccore���ṩ�ú���ʵ�֣������˲����ĵ�Դ��ͬ(��regulator�㱣֤)���������ļĴ�����ͬ��
             �ʲ���Ҫ��Ӻ˼�����ֻʹ�ú�����
*****************************************************************************/
s32 bsp_hi6559_volt_set_voltage(s32 volt_id, s32 min_uV, s32 max_uV,unsigned *selector)
{
    PMIC_HI6559_VLTGS_TABLE *hi6559_volt_table = (PMIC_HI6559_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6559_VLTGS_ATTR *hi6559_volt;
    
    s32 iret = BSP_PMU_OK;
    s32 volt_base = 0;
    s32 volt_step = 0;
    s32 valid = 0;
    s32 voltage = 0; /* ʵ�����õĵ�ѹֵ */
    
    u8 volt_nums = 0;
    u8 i = 0;

    /* ������Ч�Լ�� */
    iret = hi6559_volt_para_check(volt_id);
    if(BSP_PMU_OK != iret)
    {
        return BSP_PMU_PARA_ERROR;
    }
    
    /* DDR�е�ѹ���Ա�ȫ�Լ�� */
    iret = hi6559_volt_table_check(hi6559_volt_table);
    if(BSP_PMU_OK != iret)
    {
        return  BSP_PMU_VOLTTABLE_ERROR;
    }

    /* LVS�ĵ�ѹĬ����1.8v�����ܸı� */
    if((PMIC_HI6559_LVS_MIN <= volt_id ) && (PMIC_HI6559_LVS_MAX >= volt_id))
    {
        pmic_print_error("volt_id %d's voltage can not be set!\n", volt_id);
        return BSP_PMU_ERROR;
    }

    hi6559_volt = (PMIC_HI6559_VLTGS_ATTR *)&(hi6559_volt_table->hi6559_volt_attr[volt_id]);

    /* buck��ѹֵ����(����buck5) */
    if((PMIC_HI6559_BUCK_MAX >= volt_id) && (PMIC_HI6559_BUCK5 != volt_id))
    {
        volt_base = hi6559_buck_volt[volt_id].volt_base;
        volt_step = hi6559_buck_volt[volt_id].volt_step;
        volt_nums = hi6559_volt->voltage_nums;

        for(i = 0; i <= volt_nums; i++ )
        {
            if((min_uV <= (volt_base + i * volt_step)) && (max_uV >= (volt_base + i * volt_step)))
            {
                valid = HI6559_VALUE_VALID;   /* [min, max]��һ���Ϸ����� */
                break;
            }
        }

        /* [min, max]����Ϸ���Ѱ�������ڵĵ�ѹֵ */
        if(valid)
        {
            voltage = (volt_base + i * volt_step);
            *selector = i;
        }
    }
    else /* LDO��buck5�ĵ�ѹֵΪ��λ */
    {
        /* ��� vltg �Ƿ�Ϊ����Ӧ�ĵ�ѹԴ����Чֵ����ת����ѹֵΪ�Ĵ�������ֵ */
        for (i = 0; i <= hi6559_volt->voltage_nums; i++)
        {
            if ((min_uV <= hi6559_volt->voltage_list[i]) && (max_uV >= hi6559_volt->voltage_list[i]))
            {
                valid = HI6559_VALUE_VALID;
                break;
            }
        }
        voltage = hi6559_volt->voltage_list[i];
        *selector = i;
    }

    /* �������ֵ��Ч�����üĴ��� */
    if (valid)
    {
        bsp_hi6559_reg_write_mask(hi6559_volt->voltage_reg_addr, (u8)((u32)i << hi6559_volt->voltage_bit_offset), hi6559_volt->voltage_bit_mask);
        pmic_print_info("voltage is set at %d uV!\n", voltage);
        return BSP_PMU_OK;
    }
    else
    {
        pmic_print_error("volt_id %d cann't support voltage between %d and %d uV!\n",volt_id,min_uV,max_uV);
        return BSP_PMU_ERROR;
    }

}

/*****************************************************************************
 �� �� ��  : bsp_hi6559_volt_get_mode
 ��������  : PMIC HI6559��Դģʽ��ѯ
            (֧��normal�͸���PMU����eco��ǿ�ƽ���eco����ģʽ,
             ��֧��ģʽ��·����PMU_MODE_NONE)
 �������  : volt_id:Ҫ��ѯ�ĵ�Դ���
 �������  : ��
 �� �� ֵ  : ��ѹԴ�����ģʽ
 ��ע˵��  : Acore��Ccore���ṩ�ú���ʵ�֣�ֻ��������Ҫ�����
*****************************************************************************/
pmu_mode_e bsp_hi6559_volt_get_mode(s32 volt_id)
{
    PMIC_HI6559_VLTGS_TABLE *hi6559_volt_table = (PMIC_HI6559_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6559_VLTGS_ATTR *hi6559_volt;
    s32 iret = BSP_PMU_OK;
    u8 regval = 0;

    /* ������Ч�Լ�� */
    iret = hi6559_volt_para_check(volt_id);
    if(BSP_PMU_OK != iret)
    {
        return PMU_MODE_BUTTOM;
    }

    /* ��ѹ���Ա�ȫ�Լ�� */
    iret = hi6559_volt_table_check(hi6559_volt_table);
    if(BSP_PMU_OK != iret)
    {
        return PMU_MODE_BUTTOM;
    }

    hi6559_volt = &hi6559_volt_table->hi6559_volt_attr[volt_id];

    /* �жϸ�·�Ƿ���ecoģʽ */
    if((hi6559_volt->eco_force_reg_addr == PMU_INVAILD_ADDR) && (hi6559_volt->eco_follow_reg_addr == PMU_INVAILD_ADDR))
    {
        pmic_print_info("volt_id[%d] have no eco mode\n", volt_id);
        return PMU_MODE_NONE;           /* û��ecoģʽ */
    }
    
    /* force �����ȼ���� */
    if(hi6559_volt->eco_force_reg_addr != PMU_INVAILD_ADDR)
    {
        bsp_hi6559_reg_read(hi6559_volt->eco_force_reg_addr, &regval);
        if(regval & ((u8)((u32)0x1 << hi6559_volt->eco_force_bit_offset)))
        {
            pmic_print_info("volt_id[%d] is in force eco mode\n", volt_id);
            return PMU_MODE_ECO_FORCE;  /* eco mode */
        }
    }

    if(hi6559_volt->eco_follow_reg_addr != PMU_INVAILD_ADDR)
    {
        bsp_hi6559_reg_read(hi6559_volt->eco_follow_reg_addr, &regval);
        if(regval & ((u8)((u32)0x1 << hi6559_volt->eco_follow_bit_offset)))
        {
            pmic_print_info("volt_id[%d] is in follow eco mode\n", volt_id);
            return PMU_MODE_ECO_FOLLOW; /* eco mode */
        }
    }

    pmic_print_info("volt_id[%d] is in normal mode\n", volt_id);
    return PMU_MODE_NORMAL;             /* ��·֧��ECOģʽ�����ǲ�����ECOģʽ */
}

/*****************************************************************************
 �� �� ��  : bsp_hi6559_volt_set_mode
 ��������  : PMIC HI6559��Դģʽ����(֧��normal/follow_eco/force_eco����ģʽ,ֻ���ض�·֧��)
 �������  : @volt_id: ��ѹԴid
             @mode:ģʽ
 �������  : ��
 �� �� ֵ  : BSP_PMU_OK: �ɹ��� else:ʧ��
 ��ע˵��  : ֻ��Acore�ṩ�ú���ʵ�֣�������Ӻ˼�����ֻʹ�ú�����
*****************************************************************************/
s32 bsp_hi6559_volt_set_mode(s32 volt_id, pmu_mode_e mode)
{
    PMIC_HI6559_VLTGS_TABLE *hi6559_volt_table = (PMIC_HI6559_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6559_VLTGS_ATTR *hi6559_volt;
    s32 iret = BSP_PMU_OK;
    u8 regval = 0;

    /* ������Ч�Լ�� */
    iret = hi6559_volt_para_check(volt_id);
    if(BSP_PMU_OK != iret)
    {
        return BSP_PMU_PARA_ERROR;
    }

    /* ��ѹ���Ա�ȫ�Լ�� */
    iret = hi6559_volt_table_check(hi6559_volt_table);
    if(BSP_PMU_OK != iret)
    {
        return BSP_PMU_VOLTTABLE_ERROR;
    }
    
    hi6559_volt = &hi6559_volt_table->hi6559_volt_attr[volt_id];

    /*�жϸ�·�Ƿ���ecoģʽ*/
    if(((PMU_INVAILD_ADDR == hi6559_volt->eco_force_reg_addr) && (PMU_MODE_ECO_FORCE== mode)) ||
       ((PMU_INVAILD_ADDR == hi6559_volt->eco_follow_reg_addr) && (PMU_MODE_ECO_FOLLOW== mode)))
    {
        pmic_print_error("PMIC HI6559 this volt doesn't have eco mode!\n");
        return BSP_PMU_ERROR;
    }

    /* ���ݲ�ͬ��ģʽ�������� */
    switch(mode)
    {
        case PMU_MODE_NORMAL:
            pmic_print_info("volt_id[%d] will be set normal mode\n", volt_id);

            if(PMU_INVAILD_ADDR != hi6559_volt->eco_force_reg_addr)
            {
                bsp_hi6559_reg_read(hi6559_volt->eco_force_reg_addr, &regval);
                regval &= ~(u8)((u32)0x1 << hi6559_volt->eco_force_bit_offset);
                bsp_hi6559_reg_write(hi6559_volt->eco_force_reg_addr, regval);
            }

            if(PMU_INVAILD_ADDR != hi6559_volt->eco_follow_reg_addr)
            {
                bsp_hi6559_reg_read(hi6559_volt->eco_follow_reg_addr, &regval);
                regval &= ~(u8)((u32)0x1 << hi6559_volt->eco_follow_bit_offset);
                bsp_hi6559_reg_write(hi6559_volt->eco_follow_reg_addr, regval);
            }
            break;

        case PMU_MODE_ECO_FOLLOW:
            pmic_print_info("volt_id[%d] will be set eco FOLLOW mode\n", volt_id);
            
            if(PMU_INVAILD_ADDR != hi6559_volt->eco_follow_reg_addr)
            {
                bsp_hi6559_reg_read(hi6559_volt->eco_follow_reg_addr, &regval);
                regval |= (u8)((u32)0x1 << hi6559_volt->eco_follow_bit_offset);
                bsp_hi6559_reg_write(hi6559_volt->eco_follow_reg_addr, regval);
            }
            break;

        case PMU_MODE_ECO_FORCE:
            pmic_print_info("volt_id[%d] will be set eco FORCE mode\n", volt_id);
            
            if(PMU_INVAILD_ADDR != hi6559_volt->eco_force_reg_addr)
            {
                bsp_hi6559_reg_read(hi6559_volt->eco_force_reg_addr, &regval);
                regval |= (u8)((u32)0x1 << hi6559_volt->eco_force_bit_offset);
                bsp_hi6559_reg_write(hi6559_volt->eco_force_reg_addr, regval);
            }            
            break;

        default:
            pmic_print_error("ERROR: unkonwn mode %d\n", mode);
            return BSP_PMU_ERROR;
    }

    return BSP_PMU_OK;
}

/*****************************************************************************
 �� �� ��  : hi6559_set_by_nv
 ��������  : ����nv�����ý���Ĭ������
 �������  : void
 �������  : ��
 �� �� ֵ  : pmuĬ������(��Ҫָ���Ʒ��̬��ص�)
*****************************************************************************/
void hi6559_set_by_nv(void)
{
    u32 selector=0;
    u32 iret = BSP_PMU_OK;
    u32 iret_temp = BSP_PMU_OK;
    s32 i = 0;

    /* ��ʼ��Ϊ0 */
    memset((void*)&pmu_init_config, 0, sizeof(PMU_INIT_NV_STRU));
    
    /* ��NV */
    iret = bsp_nvm_read(NV_ID_DRV_NV_PMU_INIT,(u8 *)&pmu_init_config,sizeof(PMU_INIT_NV_STRU));
    if(NV_OK != iret)
    {
        pmic_print_error("pmu init read nv error,not set,use the default config!\n");
        return;
    }

    for(i = PMIC_HI6559_VOLT_MIN;i <= PMIC_HI6559_VOLT_MAX;i++)
    {
        if(pmu_init_config.InitConfig[i].IsNeedSet)
        {
            if(pmu_init_config.InitConfig[i].IsVoltSet)
            {
                iret_temp = bsp_hi6559_volt_set_voltage(pmu_init_config.InitConfig[i].VoltId,(s32)pmu_init_config.InitConfig[i].Voltage,\
                                            (s32)pmu_init_config.InitConfig[i].Voltage, &selector);
                if(iret_temp)
                {
                    pmic_print_error("ERROR: set voltage failed, iret %d!\n", iret_temp);
                    iret |= iret_temp;
                }
            }
            
            if(pmu_init_config.InitConfig[i].IsOnSet)
            {
                iret_temp = bsp_hi6559_volt_enable(pmu_init_config.InitConfig[i].VoltId);
                if(iret_temp)
                {
                    pmic_print_error("ERROR: voltage enable failed, iret %d!\n", iret_temp);
                    iret |= iret_temp;
                }
            }
            
            if(pmu_init_config.InitConfig[i].IsOffSet)
            {
                iret_temp = bsp_hi6559_volt_disable(pmu_init_config.InitConfig[i].VoltId);
                if(iret_temp)
                {
                    pmic_print_error("ERROR: voltage disable failed, iret %d!\n", iret_temp);
                    iret |= iret_temp;
                }
            }
            
            if(pmu_init_config.InitConfig[i].IsEcoSet)
            {
                iret_temp = bsp_hi6559_volt_set_mode(pmu_init_config.InitConfig[i].VoltId,pmu_init_config.InitConfig[i].EcoMod);
                if(iret_temp)
                {
                    pmic_print_error("ERROR: set mode failed, iret %d!\n", iret_temp);
                    iret |= iret_temp;
                }
            }
        }
    }
}

void hi6559_optreg_config(void)
{
   /* LVS07����ģʽ��Ϊģʽ2 */ 
   bsp_hi6559_reg_write_mask(HI6559_OCP_MOD_CTRL8_OFFSET, 0x4, 0x4);

   /* buck0Ĭ�ϲ�ʹ��apt */
   bsp_hi6559_reg_write(HI6559_BUCK0_REG16_ADJ_OFFSET, 0x3A);
   bsp_hi6559_reg_write(HI6559_BUCK0_REG9_ADJ_OFFSET, 0x02);
   bsp_hi6559_reg_write(HI6559_BUCK0_REG12_ADJ_OFFSET, 0x0);
   bsp_hi6559_reg_write(HI6559_BUCK0_REG4_ADJ_OFFSET, 0xFE);
   bsp_hi6559_reg_write(HI6559_BUCK0_REG9_ADJ_OFFSET, 0x42);

   /* buck3 */
   bsp_hi6559_reg_write(HI6559_BUCK3_REG7_ADJ_OFFSET, 0x02);
   bsp_hi6559_reg_write(HI6559_BUCK3_REG1_ADJ_OFFSET, 0x0D);
   bsp_hi6559_reg_write(HI6559_BUCK3_REG9_ADJ_OFFSET, 0x03);

   /* buck4 */
   bsp_hi6559_reg_write(HI6559_BUCK4_REG1_ADJ_OFFSET, 0x5C);
   bsp_hi6559_reg_write(HI6559_BUCK4_REG2_ADJ_OFFSET, 0x06);

   /* buck5 */
   bsp_hi6559_reg_write(HI6559_BUCK5_REG5_ADJ_OFFSET, 0x03);
   bsp_hi6559_reg_write(HI6559_BUCK5_REG10_ADJ_OFFSET, 0x40);

   /* buck6 */
   bsp_hi6559_reg_write(HI6559_BUCK6_REG1_ADJ_OFFSET, 0x5C);
   bsp_hi6559_reg_write(HI6559_BUCK6_REG2_ADJ_OFFSET, 0x03);
   bsp_hi6559_reg_write(HI6559_BUCK6_REG9_ADJ_OFFSET, 0x03);
}

/*ͨ�����غ���������ҪĬ�ϴ򿪵ĵ�Դ*/
void hi6559_volt_config(void)
{
    unsigned selector = 0;

    /* ��hkadc���� */
    /* ����hi6559 LDO24 3.0v on */
    bsp_hi6559_volt_set_voltage(PMIC_HI6559_LDO24,3000000,3000000,&selector);
    bsp_hi6559_volt_enable(PMIC_HI6559_LDO24);
    udelay(240);
    
    /* ����hi6559 LDO1 2.5v on�� LDO1Ҫ��LDO24֮��� */
    bsp_hi6559_volt_set_voltage(PMIC_HI6559_LDO01,2500000,2500000,&selector);
    bsp_hi6559_volt_enable(PMIC_HI6559_LDO01);
    udelay(240);

    /* ��ƬĬ�ϴ�PA/RF��Դ������LDO12Ĭ��on������Ҫ����ٴ� */
    bsp_hi6559_volt_set_voltage(PMIC_HI6559_LDO13, 2850000, 2850000, &selector);
    bsp_hi6559_volt_enable(PMIC_HI6559_LDO13);
    udelay(240);
    bsp_hi6559_volt_enable(PMIC_HI6559_LVS07);    
    udelay(240);
    bsp_hi6559_volt_enable(PMIC_HI6559_BUCK5);    
    udelay(240);
    bsp_hi6559_volt_set_voltage(PMIC_HI6559_BUCK0, 3300000, 3300000, &selector);
    bsp_hi6559_volt_enable(PMIC_HI6559_BUCK0);

    /* ��ֹBUCK5������Ҫ�󣬸���LDO11����ѹӦ��Ϊ2.2V */
    /* bsp_hi6559_volt_set_voltage(PMIC_HI6559_LDO11, 2200000, 2200000, &selector); */
    /* bsp_hi6559_volt_enable(PMIC_HI6559_LDO11);    */
}

extern void hi6559_volttable_copytoddr(void);
/*****************************************************************************
 �� �� ��  : pmu_hi6559_init
 ��������  : pmu hi6559ģ���ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  : fastboot��pmuģ���ʼ��ʱ����
*****************************************************************************/
void pmu_hi6559_init(void)
{
    u32 *volt_need_off = (u64 *)SHM_PMU_OCP_INFO_ADDR;/*��������Ҫ�رյĹ���Դ*/
    u8 iregVal;

    pmic_print_info("\r\npmu_hi6559_init!");

	*volt_need_off = 0;

    /* ����ѹ���Ʊ�����DDR�� */
    hi6559_volttable_copytoddr();
    pmic_print_info("\r\nhi6559_volttable_copytoddr ok!");

    /* ����Ӳ���ṩ�ĵ�Դ�Ż����ò��� */
    hi6559_optreg_config();
    
    /* ������Ӳ���ӿ��ĵ�����Ҫ��Ĭ�ϵ�Դ���� */
    hi6559_volt_config();
    
    /* set dac */
    bsp_hi6559_reg_read(HI6559_DAC_CTRL_OFFSET,&iregVal);
    iregVal |=  0x01;
    bsp_hi6559_reg_write(HI6559_DAC_CTRL_OFFSET, iregVal);
    
    /* ����buck3/LDO8/LVS9/LDO12��Peri_EN�������µ� */
    bsp_hi6559_reg_read(HI6559_ENB3_ECO_MARK_OFFSET,&iregVal);
    iregVal |= 0x3c;
    bsp_hi6559_reg_write(HI6559_ENB3_ECO_MARK_OFFSET, iregVal);

    pmic_print_info("pmu_hi6559_init ok!\n");
}

/*���º������ṩ������ģ��ʹ�õĽӿڣ�����Ҫadp(pmu.c)��װ*/
/*****************************************************************************
 �� �� ��  : hi6559_power_key_state_get
 ��������  : ��ȡusb�Ƿ���״̬
 �������  : void
 �������  : ��
 �� �� ֵ  : usb�����γ�(1:���룻0:�γ�)
 ���ú���  :
 ��������  : ���ػ�ģ��
*****************************************************************************/
bool hi6559_power_key_state_get(void)
{
    u8 regval = 0;

    bsp_hi6559_reg_read(HI6559_STATUS1_OFFSET, &regval);
    return  (regval & PMU_HI6559_POWER_KEY_MASK) ? 1 : 0;
}
/*****************************************************************************
 �� �� ��  : hi6559_usb_state_get
 ��������  : ��ȡusb�Ƿ���״̬
 �������  : void
 �������  : ��
 �� �� ֵ  : usb�����γ�(1:���룻0:�γ�)
 ���ú���  :
 ��������  : ���ػ�ģ��
*****************************************************************************/
bool hi6559_usb_state_get(void)
{
    u8 regval = 0;

    bsp_hi6559_reg_read(HI6559_STATUS2_OFFSET, &regval);
    return  (regval & PMU_HI6559_USB_STATE_MASK) ? 1 : 0;
}

/*****************************************************************************
 �� �� ��  : hi6559_hreset_state_get
 ��������  : �ж�pmu�Ƿ�Ϊ�ȸ�λ
 �������  : void
 �������  : ��
 �� �� ֵ     : pmu����������������(1:��������0:������)
 ���ú���  :
 ��������  : ���ػ�ģ��
*****************************************************************************/
bool hi6559_hreset_state_get(void)
{
    u8 regval = 0;

    bsp_hi6559_reg_read(HI6559_NP_IRQ1_RECORD_OFFSET, &regval);
    return  (regval & PMU_HI6559_HRESET_STATE_MASK) ? 1 : 0;
}

/*****************************************************************************
 �� �� ��  : hi6559_hreset_state_get
 ��������  : ���pmu�ȸ�λ��־
 �������  : void
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  : ���ػ�ģ��
*****************************************************************************/
void hi6559_hreset_state_clear(void)
{
    bsp_hi6559_reg_write(HI6559_NP_IRQ1_RECORD_OFFSET, PMU_HI6559_HRESET_STATE_MASK);
}

/*****************************************************************************
 �� �� ��  : hi6559_version_get
 ��������  : ��ȡpmu�İ汾��
 �������  : void
 �������  : ��
 �� �� ֵ  : pmu�汾��
 ���ú���  :
 ��������  : pmu adp�ļ�����
*****************************************************************************/
u8 hi6559_version_get(void)
{
    u8 regval = 0;

    bsp_hi6559_reg_read(HI6559_VERSION_OFFSET, &regval);
    return  regval;
}

/*****************************************************************************
 �� �� ��  : hi6559_get_boot_state
 ��������  : ϵͳ����ʱ���pmu�Ĵ���״̬��ȷ���Ƿ�����pmu���������
 �������  : void
 �������  : reset.log
 �� �� ֵ  : pmu�����ok
 ���ú���  :
 ��������  : ϵͳ��ά�ɲ�
*****************************************************************************/
s32 hi6559_get_boot_state(void)
{
    s32 iret = PMU_STATE_OK;
    s32 flag = 0;
    u32 j = 0;
    u8 hi6559_om_boot[12] = {0};

    /* ������µ�Ĵ��� */
    for(j = 0; j < 11; j++)
    {
        bsp_hi6559_reg_read((HI6559_NP_IRQ1_RECORD_OFFSET + j), &hi6559_om_boot[j]);
    }

    /* Ƿѹ */
    if((hi6559_om_boot[0] & (0x01 << HI6559_VSYS_UNDER_2P5_OFFSET ))||
       (hi6559_om_boot[0] & (0x01 << HI6559_VSYS_UNDER_2P85_OFFSET )))
    {
        iret |= PMU_STATE_UNDER_VOL;
        pmic_print_error("hi6559 under voltage last time,check hardware!\n");
    }

    /* ��ѹ */
    if(hi6559_om_boot[0] & (0x01 << HI6559_VSYS_OVER_6P0_OFFSET ))
    {
        iret |=  PMU_STATE_OVER_VOL;
        pmic_print_error("hi6559 over voltage last time,check hardware!\n");
    }
    
    /* ���� */
    if(hi6559_om_boot[1] & (0x01 << HI6559_OTMP_150_OFFSET ) )
    {
        iret |=  PMU_STATE_OVER_TEMP;
        pmic_print_error("hi6559 over temperature last time,check hardware!\n");
    }
    
    /* ����,����buck��·����·��������ѹ����· */
    for(j = 0; j < 8; j++)
    {
        if(hi6559_om_boot[j + 3])
        {
            flag = 1;
        }
    }
    
    if(flag)
    {
        iret|=  PMU_STATE_OVER_CUR;
        pmic_print_error("hi6559 over current last time,check hardware!\n");
    }

    return iret;
}

/*****************************************************************************
 �� �� ��  : hi6559_ldo14_switch
 ��������  : ����ldo14��Դ
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  : lcdģ�����
*****************************************************************************/
s32 hi6559_ldo14_switch(power_switch_e sw)
{
    s32 iret = BSP_PMU_OK;

    switch(sw)
    {
        case POWER_OFF:
            iret = bsp_hi6559_volt_disable(PMIC_HI6559_LDO14);
            break;
        case POWER_ON:
            iret = bsp_hi6559_volt_enable(PMIC_HI6559_LDO14);
            break;
        default:
            break;
    }
    pmic_print_info("hi6559_ldo14_switch ok!\n");
    return iret;
}
/*****************************************************************************
 �� �� ��  : hi6559_ldo14_volt_set
 ��������  : ����ldo14��Դ��ѹ
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  : lcdģ�����
*****************************************************************************/
s32 hi6559_ldo14_volt_set(s32 voltage)
{
    unsigned selector = 0;
    s32 iret = BSP_PMU_OK;

     iret = bsp_hi6559_volt_set_voltage(PMIC_HI6559_LDO14,voltage,voltage,&selector);

     pmic_print_info("hi6559_ldo14_volt_set ok!\n");
     return iret;
}
/*****************************************************************************
 �� �� ��  : hi6559_ldo23_switch
 ��������  : ����ldo23��Դ
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  : efuseģ�����
*****************************************************************************/
s32 hi6559_ldo23_switch(power_switch_e sw)
{
    s32 iret = BSP_PMU_OK;

    switch(sw)
    {
        case POWER_OFF:
            iret = bsp_hi6559_volt_disable(PMIC_HI6559_LDO23);
            break;
        case POWER_ON:
            iret = bsp_hi6559_volt_enable(PMIC_HI6559_LDO23);
            break;
        default:
            break;
    }
    pmic_print_info("hi6559_ldo14_switch ok!\n");
    return iret;
}
/*****************************************************************************
 �� �� ��  : hi6559_ldo23_volt_set
 ��������  : ����ldo23��Դ��ѹ
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  : efuseģ�����
*****************************************************************************/
s32 hi6559_ldo23_volt_set(s32 voltage)
{
    unsigned selector = 0;
    s32 iret = BSP_PMU_OK;

     iret = bsp_hi6559_volt_set_voltage(PMIC_HI6559_LDO23,voltage,voltage,&selector);

     pmic_print_info("hi6559_ldo23_volt_set ok!\n");
     return iret;
}

