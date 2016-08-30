
#include <boot/boot.h>
#include <soc_memmap.h>
#include <hi_smartstar.h>
#include <bsp_shared_ddr.h>
#include <drv_nv_id.h>
#include <drv_nv_def.h>
#include <nv_boot.h>
#include <pmu.h>
#include "pmic_hi6551.h"

/*PMU��ʼ������nvֵ��ֻ��fastboot����Ҫ*/
/*PMU_INIT_NV_STRU    pmu_init_config={{0,0,0,0,0,0,0,0,0}};*//*��ֵ��warning��init��ʹ��memset*/
PMU_INIT_NV_STRU    pmu_init_config;
extern void hi6551_volttable_copytoddr(void);

/*BUCK��ѹ����*/
typedef struct
{
    int volt_base;
    int step;

}PMIC_HI6551_BUCK_ADJ;
/*��λ��0.1uv*/
static const PMIC_HI6551_BUCK_ADJ hi6551_buck_volt[PMIC_HI6551_BUCK_MAX + 1] =
{
    { 7020000, 79365},
    { 7020000, 79365},
    { 7000000, 80000},
    { 7500000, 250000},
    {12000000, 250000},
    { 7000000, 80000},
    {18000000, 250000},
};

#define HI6551_LVS_VOLTAGE 1800000  /*LVS��ѹΪ�̶�ֵ*/
#define HI6551_BUCK2_5_VOLTAGE_1 1604000 /*BUCK2/5�̶���ѹ��1*/
#define HI6551_BUCK2_5_STEP_1 400000 /*BUCK2/5�̶���ѹ��1*/
#define HI6551_BUCK2_5_VOLTAGE_2 1804000 /*BUCK2/5�̶���ѹ��2*/
#define HI6551_BUCK2_5_STEP_2 200000 /*BUCK2/5�̶���ѹ��1*/
/*----------------------------ȫ�ֱ���\������---------------------------------------------*/

/*para check func*/
static __inline__ int hi6551_volt_para_check(int volt_id)
{
    /* ��Ч�Լ��*/
    if ((PMIC_HI6551_VOLT_MAX < volt_id) || (PMIC_HI6551_VOLT_MIN > volt_id))
    {
        pmic_print_error("PMIC doesn't have volt %d!\n",volt_id);
        pmic_print_error("the volt you can use in this PMIC is from %d to %d,please check!\n",PMIC_HI6551_VOLT_MIN,PMIC_HI6551_VOLT_MAX);

        return BSP_PMU_ERROR;
    }
    else
        return BSP_PMU_OK;
}
static __inline__ int hi6551_volt_table_check(PMIC_HI6551_VLTGS_TABLE *hi6551_volt_table)
{
    if((SHM_PMU_VOLTTABLE_MAGIC_START_DATA == hi6551_volt_table->magic_start) && \
        (SHM_PMU_VOLTTABLE_MAGIC_END_DATA == hi6551_volt_table->magic_end))
        return BSP_PMU_OK;
    else
        return BSP_PMU_ERROR;
}

/*****************************************************************************
* �� �� ��  : hi6551_reg_write
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
void bsp_hi6551_reg_write( u16 addr, u8 value)
{
    writel((u32) value,(u32)(HI_PMUSSI0_REGBASE_ADDR_VIRT + (addr << 2)));
}
/*****************************************************************************
* �� �� ��  : hi6551_reg_read
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
void  bsp_hi6551_reg_read( u16 addr, u8 *pValue)
{
      *pValue = (u8)readl((u32)(HI_PMUSSI0_REGBASE_ADDR_VIRT + (addr << 2)));
}
/*****************************************************************************
* �� �� ��  : hi6551_reg_write_mask
*
* ��������  : ��pmuоƬ�Ĵ�����ĳЩbit��λ
*
* �������  : u16 addr����д��ļĴ�����ַ
*             u8 u8Data������������
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
void bsp_hi6551_reg_write_mask(u16 addr, u8 value, u8 mask)
{
    u8 reg_tmp = 0;

    bsp_hi6551_reg_read(addr, &reg_tmp);
    reg_tmp &= ~mask;
    reg_tmp |= value;
    bsp_hi6551_reg_write(addr, reg_tmp);
}

/*****************************************************************************
 �� �� ��  : bsp_hi6551_volt_enable
 ��������  : pmu hi6551ģ���ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  : fastboot��pmuģ���ʼ��ʱ����
*****************************************************************************/
int bsp_hi6551_volt_enable(int volt_id)
{
    int iret = BSP_PMU_OK;
    PMIC_HI6551_VLTGS_TABLE *hi6551_volt_table = (PMIC_HI6551_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6551_VLTGS_ATTR *hi6551_volt;
    u8 regval = 0;

    /*������Ч�Լ��*/
    iret = hi6551_volt_para_check(volt_id);
    if(BSP_PMU_OK != iret)
        return BSP_PMU_PARA_ERROR;
    /*DDR�е�ѹ���Ա�ȫ�Լ��*/
    iret = hi6551_volt_table_check(hi6551_volt_table);
    if(BSP_PMU_OK != iret)
        return  BSP_PMU_VOLTTABLE_ERROR;

    hi6551_volt = &hi6551_volt_table->hi6551_volt_attr[volt_id];
     /*ʹ��reg������д1��Ч����pmussi���߻��Ᵽ֤���⣬����ϲ����������⡣boost��LDO9��LDO11����*/
    if(PMIC_HI6551_LDO09 == volt_id)
    {
        bsp_hi6551_reg_read(hi6551_volt->enable_reg_addr, &regval);
        /*������Դ��ת����·����Ӳ��ȷ���Ƿ���˳��Ҫ��*/
        regval |= (u8)((u32)0x3 << hi6551_volt->enable_bit_offset);
        bsp_hi6551_reg_write(hi6551_volt->enable_reg_addr, regval);
        /*����sim���������������ã����ڿ�����Դ�󣬷�����ë��*/
        bsp_hi6551_reg_write(HI6551_SIM0_CFG_OFFSET, 0x10);
    }
    else if(PMIC_HI6551_LDO11 == volt_id)
    {
        bsp_hi6551_reg_read(hi6551_volt->enable_reg_addr, &regval);
        /*������Դ��ת����·����Ӳ��ȷ���Ƿ���˳��Ҫ��*/
        regval |= (u8)((u32)0x3 << hi6551_volt->enable_bit_offset);
        bsp_hi6551_reg_write(hi6551_volt->enable_reg_addr, regval);
        /*����sim����������������*/
        bsp_hi6551_reg_write(HI6551_SIM1_CFG_OFFSET, 0x10);
    }
    else if(PMIC_HI6551_BOOST == volt_id)/*boost��д1����������bitλ�����޸�*/
    {
        bsp_hi6551_reg_read(hi6551_volt->enable_reg_addr, &regval);
        regval |= (u8)((u32)0x1 << hi6551_volt->enable_bit_offset);
        bsp_hi6551_reg_write(hi6551_volt->enable_reg_addr, regval);
    }
    else
    {
        regval = (u8)((u32)0x1 << hi6551_volt->enable_bit_offset);
        bsp_hi6551_reg_write(hi6551_volt->enable_reg_addr, regval);
    }

    return BSP_PMU_OK;

}
int bsp_hi6551_volt_disable(int volt_id)
{

    int iret = BSP_PMU_OK;
    PMIC_HI6551_VLTGS_TABLE *hi6551_volt_table = (PMIC_HI6551_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6551_VLTGS_ATTR *hi6551_volt;
    u8 regval = 0;

    /*������Ч�Լ��*/
    iret = hi6551_volt_para_check(volt_id);
    if(BSP_PMU_OK != iret)
        return BSP_PMU_PARA_ERROR;
    /*DDR�е�ѹ���Ա�ȫ�Լ��*/
    iret = hi6551_volt_table_check(hi6551_volt_table);
    if(BSP_PMU_OK != iret)
        return  BSP_PMU_VOLTTABLE_ERROR;

    hi6551_volt = &hi6551_volt_table->hi6551_volt_attr[volt_id];

    /*BOOST��LDO9/11��д0�رգ���������д1�ر�*/
    /*����reg������д1��Ч����pmussi���߻��Ᵽ֤���⣬����ϲ����������⡣boost��LDO9��LDO11����*/
    if(PMIC_HI6551_LDO09 == volt_id)
    {
        /*�ָ�sim����������������*/
        bsp_hi6551_reg_write(HI6551_SIM0_CFG_OFFSET, 0x0c);
        bsp_hi6551_reg_read(hi6551_volt->disable_reg_addr, &regval);
        /*�رյ�Դ��ת����·����Ӳ��ȷ���Ƿ���˳��Ҫ��*/
        regval &= ~(u8)((u32)0x3 << hi6551_volt->disable_bit_offset);
        bsp_hi6551_reg_write(hi6551_volt->disable_reg_addr, regval);
    }
    else if(PMIC_HI6551_LDO11 == volt_id)
    {
        /*�ָ�sim����������������*/
        bsp_hi6551_reg_write(HI6551_SIM1_CFG_OFFSET, 0x0c);
        bsp_hi6551_reg_read(hi6551_volt->disable_reg_addr, &regval);
        /*�رյ�Դ��ת����·����Ӳ��ȷ���Ƿ���˳��Ҫ��*/
        regval &= ~(u8)((u32)0x3 << hi6551_volt->disable_bit_offset);
        bsp_hi6551_reg_write(hi6551_volt->disable_reg_addr, regval);
    }
    else if(PMIC_HI6551_BOOST == volt_id)/*boost��д0�رգ�����bitλ�����޸�*/
    {
        bsp_hi6551_reg_read(hi6551_volt->disable_reg_addr, &regval);
        regval &= ~(u8)((u32)0x1<<hi6551_volt->disable_bit_offset);
        bsp_hi6551_reg_write(hi6551_volt->disable_reg_addr, regval);
    }
    else
    {
        regval = (u8)((u32)0x1 << hi6551_volt->disable_bit_offset);
        bsp_hi6551_reg_write(hi6551_volt->disable_reg_addr, regval);
    }

    return BSP_PMU_OK;

}
/*****************************************************************************
 �� �� ��  : bsp_hi6551_volt_is_enabled
 ��������  : ��ѯĳ·��ѹԴ�Ƿ�����
 �������  : volt_id:��ѹԴid��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��ע˵��  : ֻ��Acore��Ccore�ṩ�ú���ʵ�֣�
*****************************************************************************/
int bsp_hi6551_volt_is_enabled(int volt_id)
{
    u8 regval = 0;
    int iret = BSP_PMU_OK;
    PMIC_HI6551_VLTGS_TABLE *hi6551_volt_table = (PMIC_HI6551_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6551_VLTGS_ATTR *hi6551_volt;

    /*������Ч�Լ��*/
    iret = hi6551_volt_para_check(volt_id);
    if(BSP_PMU_OK != iret)
        return BSP_PMU_PARA_ERROR;
    /*DDR�е�ѹ���Ա�ȫ�Լ��*/
    iret = hi6551_volt_table_check(hi6551_volt_table);
    if(BSP_PMU_OK != iret)
        return  BSP_PMU_VOLTTABLE_ERROR;

    hi6551_volt = &hi6551_volt_table->hi6551_volt_attr[volt_id];

    bsp_hi6551_reg_read(hi6551_volt->is_enabled_reg_addr, &regval);

    return (regval & ((u32)0x1 << hi6551_volt->is_enabled_bit_offset));
}
int bsp_hi6551_volt_get_voltage(int volt_id)
{
    int iret = BSP_PMU_OK;
    PMIC_HI6551_VLTGS_TABLE *hi6551_volt_table = (PMIC_HI6551_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6551_VLTGS_ATTR *hi6551_volt;
    int voltage = 0;
    u8 vltg = 0;
    u8 regval = 0;

    /*������Ч�Լ��*/
    iret = hi6551_volt_para_check(volt_id);
    if(BSP_PMU_OK != iret)
        return BSP_PMU_PARA_ERROR;
    /*DDR�е�ѹ���Ա�ȫ�Լ��*/
    iret = hi6551_volt_table_check(hi6551_volt_table);
    if(BSP_PMU_OK != iret)
        return  BSP_PMU_VOLTTABLE_ERROR;

    /*LVS�ĵ�ѹĬ����1.8v�����ܸı�*/
    if ((PMIC_HI6551_LVS_MIN <= volt_id ) && (PMIC_HI6551_LVS_MAX >= volt_id))
    {
        pmic_print_info("volt_id %d's voltage is  %d uV!\n", volt_id,1800000);
        return HI6551_LVS_VOLTAGE;
    }

    hi6551_volt = (PMIC_HI6551_VLTGS_ATTR *)&(hi6551_volt_table->hi6551_volt_attr[volt_id]);

    bsp_hi6551_reg_read(hi6551_volt->voltage_reg_addr, &regval);

    /* ��ȡ��ѹ��λֵ */
    vltg = (regval & hi6551_volt->voltage_bit_mask) >> hi6551_volt->voltage_bit_offset;

    /* buck�ĵ�ѹԴ������BOOST��LDO��Ϊ��λ*/
    if(volt_id <= PMIC_HI6551_BUCK_MAX)
    {
        if((vltg & 0xc0) && ((volt_id == PMIC_HI6551_BUCK2) || (volt_id == PMIC_HI6551_BUCK5)))
        {
            if(vltg & (0x1 << 6))
                voltage += HI6551_BUCK2_5_STEP_1;
            if(vltg & (0x1 << 7))
                voltage += HI6551_BUCK2_5_STEP_2;
            vltg = vltg & (~0xc0);
        }
        voltage += (hi6551_buck_volt[volt_id].volt_base + vltg * hi6551_buck_volt[volt_id].step)/10;
    }
    else
    {
        voltage = hi6551_volt->voltage_list[vltg];
    }

    pmic_print_info("volt_id %d's voltage is  %d uV!\n", volt_id,voltage);

    return voltage;

}
/*****************************************************************************
 �� �� ��  : bsp_hi6551_volt_get_mode
 ��������  : PMIC HI6551��Դ��ѹ
 �������  : volt_id:Ҫ��ѯ�ĵ�Դ���
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��ע˵��  : Acore��Ccore���ṩ�ú���ʵ�֣������˲����ĵ�Դ��ͬ(��regulator�㱣֤)��
             �ʲ���Ҫ��Ӻ˼�����ֻʹ�ú�����
*****************************************************************************/
int bsp_hi6551_volt_set_voltage(int volt_id, int min_uV, int max_uV,unsigned *selector)
{
    PMIC_HI6551_VLTGS_TABLE *hi6551_volt_table = (PMIC_HI6551_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6551_VLTGS_ATTR *hi6551_volt;
    int iret = BSP_PMU_OK;

    int volt_base = 0;
    int step = 0;

    int valid = 0;
    int voltage = 0; /*actual voltage be setted*/
    u8 voltage_nums = 0;
    u8 i = 0;

    /*������Ч�Լ��*/
    iret = hi6551_volt_para_check(volt_id);
    if(BSP_PMU_OK != iret)
        return BSP_PMU_PARA_ERROR;
    /*DDR�е�ѹ���Ա�ȫ�Լ��*/
    iret = hi6551_volt_table_check(hi6551_volt_table);
    if(BSP_PMU_OK != iret)
        return  BSP_PMU_VOLTTABLE_ERROR;

    /*LVS�ĵ�ѹĬ����1.8v�����ܸı�*/
    if ((PMIC_HI6551_LVS_MIN <= volt_id ) && (PMIC_HI6551_LVS_MAX >= volt_id))
    {
        pmic_print_error("volt_id %d's voltage can not be set!\n", volt_id);
        return BSP_PMU_ERROR;
    }

    hi6551_volt = (PMIC_HI6551_VLTGS_ATTR *)&(hi6551_volt_table->hi6551_volt_attr[volt_id]);

    /* buck�ĵ�ѹԴ������BOOST��LDO��Ϊ��λ*/
    if(PMIC_HI6551_BUCK_MAX >= volt_id)
    {
        volt_base = hi6551_buck_volt[volt_id].volt_base;
        step = hi6551_buck_volt[volt_id].step;
        voltage_nums = hi6551_volt->voltage_nums;

        if((PMIC_HI6551_BUCK2 == volt_id) || (PMIC_HI6551_BUCK5 == volt_id))
            voltage_nums -= 2;/*buck2/5 0~63��������64��65�̶�*/
        for(i = 0; i <= voltage_nums; i++ )
        {
            if ((min_uV * 10 <= (volt_base + i * step))&&(max_uV * 10 >= (volt_base + i * step)))
            {
                valid = 1;
                break;
            }
        }
        if(valid)
        {
            voltage = (volt_base + i * step)/10;
            *selector = i;
        }
        else
        {
            /*buck2��buck5��6bit��������2bit����������Ҫ���⴦��*/
            if((PMIC_HI6551_BUCK2 == volt_id) || (PMIC_HI6551_BUCK5 == volt_id))
            {
                if((min_uV <= HI6551_BUCK2_5_VOLTAGE_1) && (max_uV >= HI6551_BUCK2_5_VOLTAGE_1))
                {
                    valid = 1;
                    voltage = HI6551_BUCK2_5_VOLTAGE_1;
                    i = 127;
                    *selector = 64;
                }
                else if((min_uV <= HI6551_BUCK2_5_VOLTAGE_2) && (max_uV >= HI6551_BUCK2_5_VOLTAGE_2))
                {
                    valid = 1;
                    voltage = HI6551_BUCK2_5_VOLTAGE_2;
                    i = 255;
                    *selector = 65;
                }
            }
        }
    }
    else
    {
        /* �������2: ��� vltg �Ƿ�Ϊ����Ӧ�ĵ�ѹԴ����Чֵ����ת����ѹֵΪ�Ĵ�������ֵ */
        for (i = 0; i <= hi6551_volt->voltage_nums; i++)
        {
            if ((min_uV <= hi6551_volt->voltage_list[i])&&(max_uV >= hi6551_volt->voltage_list[i]))
            {
                valid = 1;
                break;
            }
        }
        voltage = hi6551_volt->voltage_list[i];
        *selector = i;
    }
    /* �������ֵ��Ч�����üĴ��� */
    if (valid)
    {
        bsp_hi6551_reg_write_mask(hi6551_volt->voltage_reg_addr, (u8)((u32)i << hi6551_volt->voltage_bit_offset), hi6551_volt->voltage_bit_mask);

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
 �� �� ��  : bsp_hi6551_volt_get_mode
 ��������  : PMIC HI6551��Դģʽ��ѯ
            (֧��normal�͸���PMU����eco��ǿ�ƽ���eco����ģʽ,
             ��֧��ģʽ��·����PMU_MODE_NONE)
 �������  : volt_id:Ҫ��ѯ�ĵ�Դ���
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��ע˵��  : Acore��Ccore���ṩ�ú���ʵ�֣�ֻ��������Ҫ�����
*****************************************************************************/
pmu_mode_e bsp_hi6551_volt_get_mode(int volt_id)
{
    PMIC_HI6551_VLTGS_TABLE *hi6551_volt_table = (PMIC_HI6551_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6551_VLTGS_ATTR *hi6551_volt;
    int iret = BSP_PMU_OK;
    u8 regval = 0;

    /*������Ч�Լ��*/
    iret = hi6551_volt_para_check(volt_id);
    if(BSP_PMU_OK != iret)
        return PMU_MODE_BUTTOM;
    /*DDR�е�ѹ���Ա�ȫ�Լ��*/
    iret = hi6551_volt_table_check(hi6551_volt_table);
    if(BSP_PMU_OK != iret)
        return PMU_MODE_BUTTOM;

    hi6551_volt = &hi6551_volt_table->hi6551_volt_attr[volt_id];

    /*�жϸ�·�Ƿ���ecoģʽ*/
    if((hi6551_volt->eco_force_reg_addr == PMU_INVAILD_ADDR)&&(hi6551_volt->eco_follow_reg_addr == PMU_INVAILD_ADDR))
    {
        pmic_print_info("volt_id[%d] have no eco mode\n", volt_id);
        return PMU_MODE_NONE;/*û��ecoģʽ*/
    }
    /*force �����ȼ����*/
    bsp_hi6551_reg_read(hi6551_volt->eco_force_reg_addr, &regval);
    if(regval & ((u8)((u32)0x1 << hi6551_volt->eco_force_bit_offset)))
    {
        pmic_print_info("volt_id[%d] is in force eco mode\n", volt_id);
        return PMU_MODE_ECO_FORCE;/*eco mode*/
    }

    bsp_hi6551_reg_read(hi6551_volt->eco_follow_reg_addr, &regval);
    if(regval & ((u8)((u32)0x1 << hi6551_volt->eco_follow_bit_offset)))
    {
        pmic_print_info("volt_id[%d] is in follow eco mode\n", volt_id);
        return PMU_MODE_ECO_FOLLOW;/*eco mode*/
    }
    else
    {
        pmic_print_info("volt_id[%d] is in normal mode\n", volt_id);
        return PMU_MODE_NORMAL;
    }

}
/*****************************************************************************
 �� �� ��  : bsp_hi6551_volt_set_mode
 ��������  : PMIC HI6551��Դģʽ����
            (֧��normal�͸���PMU����eco��ǿ�ƽ���eco����ģʽ,ֻ���ض�·֧��)
 �������  : irq
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��ע˵��  : ֻ��Acore�ṩ�ú���ʵ�֣�������Ӻ˼�����ֻʹ�ú�����
*****************************************************************************/
int bsp_hi6551_volt_set_mode(int volt_id, pmu_mode_e mode)
{
    PMIC_HI6551_VLTGS_TABLE *hi6551_volt_table = (PMIC_HI6551_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6551_VLTGS_ATTR *hi6551_volt;
    int iret = BSP_PMU_OK;
    u8 regval = 0;

    /*������Ч�Լ��*/
    iret = hi6551_volt_para_check(volt_id);
    if(BSP_PMU_OK != iret)
        return BSP_PMU_PARA_ERROR;
    /*DDR�е�ѹ���Ա�ȫ�Լ��*/
    iret = hi6551_volt_table_check(hi6551_volt_table);
    if(BSP_PMU_OK != iret)
        return  BSP_PMU_VOLTTABLE_ERROR;

    hi6551_volt = &hi6551_volt_table->hi6551_volt_attr[volt_id];

    /*�жϸ�·�Ƿ���ecoģʽ*/
    if((hi6551_volt->eco_force_reg_addr == PMU_INVAILD_ADDR)&&(hi6551_volt->eco_follow_reg_addr == PMU_INVAILD_ADDR))
    {
        pmic_print_error("PMIC HI6551 this volt doesn't have eco mode!\n");
        return PMU_MODE_NONE;
    }

    switch(mode){
    case PMU_MODE_NORMAL:
        pmic_print_info("volt_id[%d] will be set normal mode\n", volt_id);
        bsp_hi6551_reg_read(hi6551_volt->eco_force_reg_addr, &regval);
        regval &= ~(u8)((u32)0x1 << hi6551_volt->eco_force_bit_offset);
        bsp_hi6551_reg_write(hi6551_volt->eco_force_reg_addr, regval);

        bsp_hi6551_reg_read(hi6551_volt->eco_follow_reg_addr, &regval);
        regval &= ~(u8)((u32)0x1 << hi6551_volt->eco_follow_bit_offset);
        bsp_hi6551_reg_write(hi6551_volt->eco_follow_reg_addr, regval);

        break;

    case PMU_MODE_ECO_FOLLOW:
        pmic_print_info("volt_id[%d] will be set eco FOLLOW mode\n", volt_id);
        bsp_hi6551_reg_read(hi6551_volt->eco_follow_reg_addr, &regval);
        regval |= (u8)((u32)0x1 << hi6551_volt->eco_follow_bit_offset);
        bsp_hi6551_reg_write(hi6551_volt->eco_follow_reg_addr, regval);
        break;

    case PMU_MODE_ECO_FORCE:
        pmic_print_info("volt_id[%d] will be set eco FORCE mode\n", volt_id);
        bsp_hi6551_reg_read(hi6551_volt->eco_force_reg_addr, &regval);
        regval |= (u8)((u32)0x1 << hi6551_volt->eco_force_bit_offset);
        bsp_hi6551_reg_write(hi6551_volt->eco_force_reg_addr, regval);
        break;

    default:
        pmic_print_error("cann't support the mode\n");
        iret = BSP_PMU_ERROR;
        break;
    }

    return iret;
}

/*Ӳ����ʾboostû��ʹ��ʱ��Ҫ�رգ���������Ƭ���ܣ���init�н��õ�Դ�رգ�����Ҫʹ�ã�����ͨ������nv��*/
void hi6551_set_by_nv(void)
{
    int i = 0;
    u32 iret;
    unsigned selector=0;

    /*init default nv config,all is 0*/
    memset((void*)&pmu_init_config, 0, sizeof(PMU_INIT_NV_STRU));
    /*read nv,get the exc protect setting*/
    iret = bsp_nvm_read(NV_ID_DRV_NV_PMU_INIT,(u8 *)&pmu_init_config,sizeof(PMU_INIT_NV_STRU));

    if(NV_OK != iret)
    {
        pmic_print_error("pmu init read nv error,not set,use the default config!\n");
        return;
    }

    for(i = PMIC_HI6551_VOLT_MIN;i <= PMIC_HI6551_VOLT_MAX;i++)
    {
        if(pmu_init_config.InitConfig[i].IsNeedSet)
        {
            if(pmu_init_config.InitConfig[i].IsVoltSet)
                bsp_hi6551_volt_set_voltage(pmu_init_config.InitConfig[i].VoltId,(int)pmu_init_config.InitConfig[i].Voltage,\
                                            (int)pmu_init_config.InitConfig[i].Voltage,&selector);
            if(pmu_init_config.InitConfig[i].IsOnSet)
                bsp_hi6551_volt_enable(pmu_init_config.InitConfig[i].VoltId);
            if(pmu_init_config.InitConfig[i].IsOffSet)
                bsp_hi6551_volt_disable(pmu_init_config.InitConfig[i].VoltId);
            if(pmu_init_config.InitConfig[i].IsEcoSet)
                bsp_hi6551_volt_set_mode(pmu_init_config.InitConfig[i].VoltId,pmu_init_config.InitConfig[i].EcoMod);
        }
    }
}

void hi6551_optreg_config(void)
{
    /*BUCK2*/
    bsp_hi6551_reg_write(HI6551_BUCK2_REG1_ADJ_OFFSET,0x3c);/*����LX����������ݣ�R�ڶ���*/
    bsp_hi6551_reg_write(HI6551_BUCK2_REG8_ADJ_OFFSET,0x00);/*�Ƚ���������С*/
    bsp_hi6551_reg_write(HI6551_BUCK2_REG6_ADJ_OFFSET,0x12);/*PG_drver��ǿ1��             */
    bsp_hi6551_reg_write(HI6551_BUCK2_REG7_ADJ_OFFSET,0x03);/*NG_drver��ǿ                */
    bsp_hi6551_reg_write(HI6551_BUCK2_REG4_ADJ_OFFSET,0x20);/*DMD��С1����OCP��С1��      */
    bsp_hi6551_reg_write(HI6551_BUCK2_REG5_ADJ_OFFSET,0x01);/*N2P ������                  */
    /*BUCK3*/
    bsp_hi6551_reg_write(HI6551_BUCK3_REG7_ADJ_OFFSET,0x00);/*�Ƚ���������С*/
    bsp_hi6551_reg_write(HI6551_BUCK3_REG6_ADJ_OFFSET,0x03);/*NG_drver��ǿ*/
    bsp_hi6551_reg_write(HI6551_BUCK3_REG5_ADJ_OFFSET,0x12);/*PG_drver��ǿ1��*/
    bsp_hi6551_reg_write(HI6551_BUCK3_REG4_ADJ_OFFSET,0x11);/*�������Ʒ�ʽ����*/
    /*BUCK4*/
    bsp_hi6551_reg_write(HI6551_BUCK4_REG5_ADJ_OFFSET,0x12);/*PG_drver��ǿ1��*/
    bsp_hi6551_reg_write(HI6551_BUCK4_REG7_ADJ_OFFSET,0x01);/*�Ƚ�����������Ĭ��*/
    bsp_hi6551_reg_write(HI6551_BUCK4_REG6_ADJ_OFFSET,0x03);/*NG_drver��ǿ*/
    bsp_hi6551_reg_write(HI6551_BUCK4_REG3_ADJ_OFFSET,0x00);/*DMD��С2��*/
    bsp_hi6551_reg_write(HI6551_BUCK4_REG4_ADJ_OFFSET,0x01);/*�������Ʒ�ʽ����*/
    /*BUCK5*/
    bsp_hi6551_reg_write(HI6551_BUCK5_REG5_ADJ_OFFSET,0x12);/*PG_drver��ǿ1��*/
    bsp_hi6551_reg_write(HI6551_BUCK5_REG4_ADJ_OFFSET,0x03);/*�Ƚ�����������Ĭ��*/
    bsp_hi6551_reg_write(HI6551_BUCK5_REG7_ADJ_OFFSET,0x00);/*�Ƚ���������С*/
    bsp_hi6551_reg_write(HI6551_BUCK5_REG6_ADJ_OFFSET,0x03);/*NG_drver��ǿ*/
    bsp_hi6551_reg_write(HI6551_BUCK5_REG3_ADJ_OFFSET,0x01);/*DMD��С1��*/

    /*BUCK6*/
    bsp_hi6551_reg_write(HI6551_BUCK6_REG7_ADJ_OFFSET,0x00);/*�Ƚ���������С*/
    bsp_hi6551_reg_write(HI6551_BUCK6_REG5_ADJ_OFFSET,0x12);/*PG_drver��ǿ1��*/
    bsp_hi6551_reg_write(HI6551_BUCK6_REG4_ADJ_OFFSET,0x01);/*�������Ʒ�ʽ����*/
    bsp_hi6551_reg_write(HI6551_BUCK6_REG6_ADJ_OFFSET,0x03);/*NG_drver��ǿ*/
    bsp_hi6551_reg_write(HI6551_BUCK6_REG9_ADJ_OFFSET,0x13);/*regulator ��̬��Χ����*/

    /*BOOST*/
    bsp_hi6551_reg_write(HI6551_BOOST_ADJ1_OFFSET,0x0e);/*�ر�OVP��OCP*/
    bsp_hi6551_reg_write(HI6551_BOOST_ADJ2_OFFSET,0x32);/*�ر�DMD*/
    bsp_hi6551_reg_write(HI6551_BOOST_ADJ3_OFFSET,0x3b);/*����������к�·����*/
    /*LDO*/
    if((BSP_PMU_V230 == bsp_pmu_version_get()) ||
        (BSP_PMU_V240 == bsp_pmu_version_get()))
    {
        //bsp_hi6551_reg_write(HI6551_LDO22_REG_ADJ_OFFSET,0x01);/*LDO22������������Ϊ1.5K*/
        bsp_hi6551_reg_write(HI6551_BUCK4_REG9_ADJ_OFFSET,0x13);/*BUCK4 regulator ��̬��Χ����*/
        bsp_hi6551_reg_write(HI6551_OCP_DEB_OFFSET,0x45);/*BUCK0-6 SCP�˲�*/
    }
    else if((BSP_PMU_V210 == bsp_pmu_version_get())||(BSP_PMU_V220 == bsp_pmu_version_get()))
    {
        bsp_hi6551_reg_write(HI6551_RESERVED1_OFFSET,0x02);     /*bandgap:����׼��buffer��ƫ�õ�������*/

        /*BUCK2*/
        bsp_hi6551_reg_write(HI6551_BUCK2_REG12_ADJ_OFFSET,0x01);/*����DMDʱ������tonʱ��*/
        /*BUCK3*/
        bsp_hi6551_reg_write(HI6551_BUCK3_REG12_ADJ_OFFSET,0x01);/*����DMDʱ������tonʱ��*/
        /*BUCK4*/
        bsp_hi6551_reg_write(HI6551_BUCK4_REG9_ADJ_OFFSET,0x15);/*BUCK4 regulator ��̬��Χ����*/
        bsp_hi6551_reg_write(HI6551_BUCK4_REG12_ADJ_OFFSET,0x01);/*����DMDʱ������tonʱ��*/
        /*BUCK5*/
        bsp_hi6551_reg_write(HI6551_BUCK5_REG12_ADJ_OFFSET,0x01);/*����DMDʱ������tonʱ��*/
        /*BUCK6*/
        bsp_hi6551_reg_write(HI6551_BUCK6_REG12_ADJ_OFFSET,0x01);/*����DMDʱ������tonʱ��*/
        //bsp_hi6551_reg_write(HI6551_LDO22_REG_ADJ_OFFSET,0x31); /*LDO22������������Ϊ150ŷķ*/
    }
    /*����ocp*/
    bsp_hi6551_reg_write_mask(HI6551_OCP_MOD_CTRL4_OFFSET,0x0,(0x1<<0));/*LDO5 OCPģʽ����Ϊģʽ��*/
    bsp_hi6551_reg_write_mask(HI6551_OCP_SCP_MOD_CTRL1_OFFSET,0x0,(0x1<<0));/*BUCK01 OCPģʽ����Ϊģʽ��*/
    bsp_hi6551_reg_write_mask(HI6551_OCP_MOD_CTRL7_OFFSET,0x0,(0x1<<4));/*LDO19 OCPģʽ����Ϊģʽ��*/
    bsp_hi6551_reg_write_mask(HI6551_OCP_MOD_CTRL10_OFFSET,0x0,(0x1<<2));/*LVS6 OCPģʽ����Ϊģʽ��*/
}
/*ͨ��reg������ҪĬ�ϴ򿪵ĵ�Դ*/
void hi6551_volt_config_by_reg(void)
{
    /*����hi6551 LDO24 3.0v on��LDO1 2.5v on*/
    bsp_hi6551_reg_write_mask(HI6551_LDO24_REG_ADJ_OFFSET, 0x04,0x07);
    bsp_hi6551_reg_write(HI6551_ENABLE4_OFFSET, (0x01 << 5));

    bsp_hi6551_reg_write_mask(HI6551_LDO1_REG_ADJ_OFFSET, 0x01,0x07);
    bsp_hi6551_reg_write(HI6551_ENABLE2_OFFSET, 0x01);

    /*��LVS05,��pastar����*/
    bsp_hi6551_reg_write(HI6551_ENABLE5_OFFSET, (0x01 << 3));

    /*��lcd����*/
    /*��LVS04*/
    bsp_hi6551_reg_write(HI6551_ENABLE5_OFFSET, (0x01 << 2));
    /*����hi6551 LDO14 2.85v on*/
    bsp_hi6551_reg_write_mask(HI6551_LDO14_REG_ADJ_OFFSET, 0x05,0x07);
    bsp_hi6551_reg_write(HI6551_ENABLE3_OFFSET, (0x01 << 3));
}
/*ͨ�����غ���������ҪĬ�ϴ򿪵ĵ�Դ*/
void hi6551_volt_config(void)
{
    unsigned selector = 0;

    /*Ӳ����ʾboostû��ʹ��ʱ��Ҫ�رգ���������Ƭ����,��Ҫʹ�øõ�Դ��ƽ̨����ͨ��nv���ô򿪣�����ע���ô���*/
    bsp_hi6551_volt_disable(PMIC_HI6551_BOOST);
    /*��hkadc����*/
    /*����hi6551 LDO24 3.0v on*/
    bsp_hi6551_volt_set_voltage(PMIC_HI6551_LDO24,3000000,3000000,&selector);
    bsp_hi6551_volt_enable(PMIC_HI6551_LDO24);
    /*����hi6551 LDO1 2.5v on*/
    bsp_hi6551_volt_set_voltage(PMIC_HI6551_LDO01,2500000,2500000,&selector);
    bsp_hi6551_volt_enable(PMIC_HI6551_LDO01);

#if 0
    /*��lcd���磬����ģ�����Լ�ʵ��*/
    /*��LVS04*/
    bsp_hi6551_volt_enable(PMIC_HI6551_LVS04);
    /*����hi6551 LDO14 2.85v on*/
    bsp_hi6551_volt_set_voltage(PMIC_HI6551_LDO14,2850000,2850000,&selector);
    bsp_hi6551_volt_enable(PMIC_HI6551_LDO14);
    /*��LVS05,��pastar����*/
    bsp_hi6551_volt_enable(PMIC_HI6551_LVS05);
#endif
}
extern void hi6551_volttable_copytoddr(void);
/*****************************************************************************
 �� �� ��  : pmu_hi6551_init
 ��������  : pmu hi6551ģ���ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  : fastboot��pmuģ���ʼ��ʱ����
*****************************************************************************/
void pmu_hi6551_init(void)
{
    u64 *volt_need_off = (u64 *)SHM_PMU_OCP_INFO_ADDR;/*��������Ҫ�رյĹ���Դ*/
    u8 iregVal;

    pmic_print_info("\r\npmu_hi6551_init!");

	*volt_need_off = 0;

    /*����ѹ���Ʊ�����DDR��*/
    hi6551_volttable_copytoddr();
    pmic_print_info("\r\nhi6551_volttable_copytoddr ok!");

    /*��ʼ��Ĭ������*/

    /*����Ӳ���ṩ�ĵ�Դ�Ż����ò���*/
    hi6551_optreg_config();
    /*������Ӳ���ӿ��ĵ�����Ҫ��Ĭ�ϵ�Դ����*/
    hi6551_volt_config();

    /* set dac */
    bsp_hi6551_reg_read(HI6551_DAC_CTRL_OFFSET,&iregVal);
    iregVal |=  0x01;
    bsp_hi6551_reg_write(HI6551_DAC_CTRL_OFFSET, iregVal);
    /*���� buck3��Peri_EN�������µ�,bit5*/
    bsp_hi6551_reg_read(HI6551_ENB3_ECO_MARK_OFFSET,&iregVal);
    iregVal |= 0x3c;
    bsp_hi6551_reg_write(HI6551_ENB3_ECO_MARK_OFFSET, iregVal);

    /*û�п��ؼƵĲ�Ʒ��̬���رտ��ؼƣ��п��ؼƵ���ʽ��һ��*/
#ifdef CONFIG_COUL
    bsp_hi6551_reg_read(HI6551_CLJ_CTRL_REG_OFFSET,&iregVal);
    iregVal |= (0x01 << 7);
    bsp_hi6551_reg_write(HI6551_CLJ_CTRL_REG_OFFSET, iregVal);
#else
    bsp_hi6551_reg_read(HI6551_CLJ_CTRL_REG_OFFSET,&iregVal);
    iregVal &=  ~(0x01 << 7);
    bsp_hi6551_reg_write(HI6551_CLJ_CTRL_REG_OFFSET, iregVal);
#endif

    pmic_print_info("pmu_hi6551_init ok!\n");
}

/*���º������ṩ������ģ��ʹ�õĽӿڣ�����Ҫadp(pmu.c)��װ*/
/*****************************************************************************
 �� �� ��  : hi6551_power_key_state_get
 ��������  : ��ȡusb�Ƿ���״̬
 �������  : void
 �������  : ��
 �� �� ֵ  : usb�����γ�(1:���룻0:�γ�)
 ���ú���  :
 ��������  : ���ػ�ģ��
*****************************************************************************/
bool hi6551_power_key_state_get(void)
{
    u8 regval = 0;

    bsp_hi6551_reg_read(HI6551_STATUS1_OFFSET, &regval);
    return  (regval & PMU_HI6551_POWER_KEY_MASK) ? 1 : 0;
}
/*****************************************************************************
 �� �� ��  : hi6551_usb_state_get
 ��������  : ��ȡusb�Ƿ���״̬
 �������  : void
 �������  : ��
 �� �� ֵ  : usb�����γ�(1:���룻0:�γ�)
 ���ú���  :
 ��������  : ���ػ�ģ��
*****************************************************************************/
bool hi6551_usb_state_get(void)
{
    u8 regval = 0;

    bsp_hi6551_reg_read(HI6551_STATUS2_OFFSET, &regval);
    return  (regval & PMU_HI6551_USB_STATE_MASK) ? 1 : 0;
}
/*****************************************************************************
 �� �� ��  : hi6551_hreset_state_get
 ��������  : �ж�pmu�Ƿ�Ϊ�ȸ�λ
 �������  : void
 �������  : ��
 �� �� ֵ     : pmu����������������(1:��������0:������)
 ���ú���  :
 ��������  : ���ػ�ģ��
*****************************************************************************/
bool hi6551_hreset_state_get(void)
{
    u8 regval = 0;

    bsp_hi6551_reg_read(HI6551_NP_IRQ1_RECORD_OFFSET, &regval);
    return  (regval & PMU_HI6551_HRESET_STATE_MASK) ? 1 : 0;
}
/*****************************************************************************
 �� �� ��  : hi6551_hreset_state_get
 ��������  : ���pmu�ȸ�λ��־
 �������  : void
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  : ���ػ�ģ��
*****************************************************************************/
void hi6551_hreset_state_clear(void)
{
    bsp_hi6551_reg_write(HI6551_NP_IRQ1_RECORD_OFFSET, PMU_HI6551_HRESET_STATE_MASK);
}
/*****************************************************************************
 �� �� ��  : hi6551_version_get
 ��������  : ��ȡpmu�İ汾��
 �������  : void
 �������  : ��
 �� �� ֵ  : pmu�汾��
 ���ú���  :
 ��������  : pmu adp�ļ�����
*****************************************************************************/
u8 hi6551_version_get(void)
{
    u8 regval = 0;

    bsp_hi6551_reg_read(HI6551_VERSION_OFFSET, &regval);
    return  regval;
}
/*****************************************************************************
 �� �� ��  : hi6551_get_boot_state
 ��������  : ϵͳ����ʱ���pmu�Ĵ���״̬��
                ȷ���Ƿ�����pmu���������
 �������  : void
 �������  : reset.log
 �� �� ֵ  : pmu�����ok
 ���ú���  :
 ��������  : ϵͳ��ά�ɲ�
*****************************************************************************/
int hi6551_get_boot_state(void)
{

    u8 hi6551_om_boot[12] = {0};
    u32 j = 0;
    int iret = PMU_STATE_OK;
    int flag = 0;

    for(j = 0; j < 11; j++)
    {
        bsp_hi6551_reg_read((HI6551_NP_IRQ1_RECORD_OFFSET+j), &hi6551_om_boot[j]);
    }

    /*Ƿѹ����ѹ*/
    if((hi6551_om_boot[0] & (0x01 << HI6551_VSYS_UNDER_2P5_OFFSET ))||(hi6551_om_boot[0] & (0x01 << HI6551_VSYS_UNDER_2P7_OFFSET )))
    {
        iret|= PMU_STATE_UNDER_VOL;
        pmic_print_error("hi6551 under voltage last time,check hardware!\n");
    }
    if(hi6551_om_boot[0] & (0x01 << HI6551_VSYS_OVER_6P0_OFFSET ))
    {
        iret|=  PMU_STATE_OVER_VOL;
        pmic_print_error("hi6551 over voltage last time,check hardware!\n");
    }
    /*����*/
    if(hi6551_om_boot[1] & (0x01 << HI6551_OTMP_150_OFFSET ) )
    {
        iret|=  PMU_STATE_OVER_TEMP;
        pmic_print_error("hi6551 over temperature last time,check hardware!\n");
    }
    /*����,����buck��·����·������boostǷѹ����ѹ����·*/
    for(j= 0; j < 8; j++)
    {
        if(hi6551_om_boot[j+3])
            flag = 1;
    }
    if(flag)
    {
        iret|=  PMU_STATE_OVER_CUR;
        pmic_print_error("hi6551 over current last time,check hardware!\n");
    }

    return iret;
}
/*****************************************************************************
 �� �� ��  : hi6551_lvs4_switch
 ��������  : ����lvs04��Դ
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  : lcdģ�����
*****************************************************************************/
int hi6551_lvs4_switch(power_switch_e sw)
{
    int iret = BSP_PMU_OK;
    switch(sw)
    {
        case POWER_OFF:
            iret = bsp_hi6551_volt_disable(PMIC_HI6551_LVS04);
            break;
        case POWER_ON:
            iret = bsp_hi6551_volt_enable(PMIC_HI6551_LVS04);
            break;
        default:
            break;
    }
    pmic_print_info("hi6551_lvs4_switch ok!\n");
    return iret;
}
/*****************************************************************************
 �� �� ��  : hi6551_ldo14_switch
 ��������  : ����ldo14��Դ
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  : lcdģ�����
*****************************************************************************/
int hi6551_ldo14_switch(power_switch_e sw)
{
    int iret = BSP_PMU_OK;

    switch(sw)
    {
        case POWER_OFF:
            iret = bsp_hi6551_volt_disable(PMIC_HI6551_LDO14);
            break;
        case POWER_ON:
            iret = bsp_hi6551_volt_enable(PMIC_HI6551_LDO14);
            break;
        default:
            break;
    }
    pmic_print_info("hi6551_ldo14_switch ok!\n");
    return iret;
}
/*****************************************************************************
 �� �� ��  : hi6551_ldo14_volt_set
 ��������  : ����ldo14��Դ��ѹ
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  : lcdģ�����
*****************************************************************************/
int hi6551_ldo14_volt_set(int voltage)
{
    unsigned selector = 0;
    int iret = BSP_PMU_OK;

     iret = bsp_hi6551_volt_set_voltage(PMIC_HI6551_LDO14,voltage,voltage,&selector);

     pmic_print_info("hi6551_ldo14_volt_set ok!\n");
     return iret;
}
/*****************************************************************************
 �� �� ��  : hi6551_ldo23_switch
 ��������  : ����ldo23��Դ
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  : efuseģ�����
*****************************************************************************/
int hi6551_ldo23_switch(power_switch_e sw)
{
    int iret = BSP_PMU_OK;

    switch(sw)
    {
        case POWER_OFF:
            iret = bsp_hi6551_volt_disable(PMIC_HI6551_LDO23);
            break;
        case POWER_ON:
            iret = bsp_hi6551_volt_enable(PMIC_HI6551_LDO23);
            break;
        default:
            break;
    }
    pmic_print_info("hi6551_ldo14_switch ok!\n");
    return iret;
}
/*****************************************************************************
 �� �� ��  : hi6551_ldo23_volt_set
 ��������  : ����ldo23��Դ��ѹ
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  : efuseģ�����
*****************************************************************************/
int hi6551_ldo23_volt_set(int voltage)
{
    unsigned selector = 0;
    int iret = BSP_PMU_OK;

     iret = bsp_hi6551_volt_set_voltage(PMIC_HI6551_LDO23,voltage,voltage,&selector);

     pmic_print_info("hi6551_ldo23_volt_set ok!\n");
     return iret;
}
