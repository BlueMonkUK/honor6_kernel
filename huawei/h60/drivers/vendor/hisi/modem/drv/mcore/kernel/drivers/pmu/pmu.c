/*lint --e{537}*/
#include <osl_bio.h>
#include <soc_memmap.h>
#include <bsp_shared_ddr.h>
#include <product_config.h>
#ifdef CONFIG_PMIC_HI6551
#include <hi_smartstar.h>
#endif
#ifdef CONFIG_PMIC_HI6559
#include <hi_bbstar.h>
#endif
#include <bsp_hardtimer.h>
#include <bsp_pmu.h>

#define PMU_VOLT_ENABLED   0
#define PMU_VOLT_DISABLED  1

typedef struct
{
    u16 flag;
    u8 volt_id;
    u8 reserved;
}PMU_VOLT_ATTR;

#ifdef CONFIG_PMIC_HI6551

PMU_VOLT_ATTR pmu_volt[]={
    {
        .volt_id = PMIC_HI6551_LDO01,
        .flag = 0,
    },
    {
        .volt_id = PMIC_HI6551_LDO24,
        .flag = 0,
    }
};

static __inline__ int hi6551_volt_table_check(PMIC_HI6551_VLTGS_TABLE *hi6551_volt_table)
{
    if((SHM_PMU_VOLTTABLE_MAGIC_START_DATA == hi6551_volt_table->magic_start) && \
        (SHM_PMU_VOLTTABLE_MAGIC_END_DATA == hi6551_volt_table->magic_end))
        return BSP_PMU_OK;
    else
        return BSP_PMU_ERROR;
}

void bsp_hi6551_reg_write( u16 addr, u8 value)
{
    writel((u32) value,(u32)(HI_PMUSSI0_REGBASE_ADDR + (addr << 2)));
}

void  bsp_hi6551_reg_read( u16 addr, u8 *pValue)
{/*lint !e958*/
      *pValue = (u8)readl((u32)(HI_PMUSSI0_REGBASE_ADDR + (addr << 2)));
}

PMU_VOLT_ATTR pmu_volt_sdio[]={
    {
        .volt_id = PMIC_HI6551_LDO07,
        .flag = 0,
    },
    {
        .volt_id = PMIC_HI6551_LDO22,
        .flag = 0,
    }
};
/*suspendʱ��Ҫ�رյ�Դ*/
int bsp_pmu_suspend(void)
{

    int iret = BSP_PMU_OK;
    PMIC_HI6551_VLTGS_TABLE *hi6551_volt_table = (PMIC_HI6551_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6551_VLTGS_ATTR *hi6551_volt;
    unsigned int i = 0 ;
    u8 volt_id = 0;
    u8 regval = 0;


    /*DDR�е�ѹ���Ա�ȫ�Լ��*/
    iret = hi6551_volt_table_check(hi6551_volt_table);
    if(BSP_PMU_OK != iret)
        return  BSP_PMU_VOLTTABLE_ERROR;

    for(i = 0;i < sizeof(pmu_volt)/(sizeof((pmu_volt)[0]));i++)
    {
        volt_id = pmu_volt[i].volt_id;
        hi6551_volt = &hi6551_volt_table->hi6551_volt_attr[volt_id];

        bsp_hi6551_reg_read(hi6551_volt->is_enabled_reg_addr, &regval);
        if(regval & ((u32)0x1 << hi6551_volt->is_enabled_bit_offset))
        {
            regval = (u8)((u32)0x1 << hi6551_volt->disable_bit_offset);
            bsp_hi6551_reg_write(hi6551_volt->disable_reg_addr, regval);
            pmu_volt[i].flag = 1;
        }
    }

     return  BSP_PMU_OK;
}
/*resumeʱ��Ҫ������Դ*/
int bsp_pmu_resume(void)
{
    int iret = BSP_PMU_OK;
    PMIC_HI6551_VLTGS_TABLE *hi6551_volt_table = (PMIC_HI6551_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6551_VLTGS_ATTR *hi6551_volt;
    int i = 0 ;
    u8 volt_id = 0;
    u8 regval = 0;

    /*DDR�е�ѹ���Ա�ȫ�Լ��*/
    iret = hi6551_volt_table_check(hi6551_volt_table);
    if(BSP_PMU_OK != iret)
        return  BSP_PMU_VOLTTABLE_ERROR;

    //for(i = 0;i < sizeof(pmu_volt)/(sizeof((pmu_volt)[0]));i++)
    for(i = (signed)(sizeof(pmu_volt)/(sizeof((pmu_volt)[0])) - 1);i >= 0;i--)
    {
        volt_id = pmu_volt[i].volt_id;
        hi6551_volt = &hi6551_volt_table->hi6551_volt_attr[volt_id];

        if(pmu_volt[i].flag)
        {
            regval = (u8)((u32)0x1 << hi6551_volt->enable_bit_offset);
            bsp_hi6551_reg_write(hi6551_volt->enable_reg_addr, regval);
            pmu_volt[i].flag = 0;
        }
    }

    return  BSP_PMU_OK;
}

/*suspendʱ��Ҫ�رյ�Դ*/
int bsp_pmu_sdio_suspend(void)
{
    int iret = BSP_PMU_OK;
    PMIC_HI6551_VLTGS_TABLE *hi6551_volt_table = (PMIC_HI6551_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6551_VLTGS_ATTR *hi6551_volt;
    unsigned int i = 0 ;
    u8 volt_id = 0;
    u8 regval = 0;

    /*DDR�е�ѹ���Ա�ȫ�Լ��*/
    iret = hi6551_volt_table_check(hi6551_volt_table);
    if(BSP_PMU_OK != iret)
        return  BSP_PMU_VOLTTABLE_ERROR;

    for(i = 0;i < sizeof(pmu_volt_sdio)/(sizeof((pmu_volt_sdio)[0]));i++)
    {
        volt_id = pmu_volt_sdio[i].volt_id;
        hi6551_volt = &hi6551_volt_table->hi6551_volt_attr[volt_id];

        bsp_hi6551_reg_read(hi6551_volt->is_enabled_reg_addr, &regval);
        if(regval & ((u32)0x1 << hi6551_volt->is_enabled_bit_offset))
        {
            if(PMIC_HI6551_LDO22 == volt_id)
                udelay(20);

            regval = (u8)((u32)0x1 << hi6551_volt->disable_bit_offset);
            bsp_hi6551_reg_write(hi6551_volt->disable_reg_addr, regval);
            pmu_volt_sdio[i].flag = 1;
        }
    }

     return  BSP_PMU_OK;
}
/*resumeʱ��Ҫ������Դ*/
int bsp_pmu_sdio_resume(void)
{
    int iret = BSP_PMU_OK;
    PMIC_HI6551_VLTGS_TABLE *hi6551_volt_table = (PMIC_HI6551_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6551_VLTGS_ATTR *hi6551_volt;
    int i = 0 ;
    u8 volt_id = 0;
    u8 regval = 0;


    /*DDR�е�ѹ���Ա�ȫ�Լ��*/
    iret = hi6551_volt_table_check(hi6551_volt_table);
    if(BSP_PMU_OK != iret)
        return  BSP_PMU_VOLTTABLE_ERROR;

    //for(i = 0;i < sizeof(pmu_volt)/(sizeof((pmu_volt)[0]));i++)
    for(i = (signed)(sizeof(pmu_volt_sdio)/(sizeof((pmu_volt_sdio)[0])) - 1);i >= 0;i--)
    {
        volt_id = pmu_volt_sdio[i].volt_id;
        hi6551_volt = &hi6551_volt_table->hi6551_volt_attr[volt_id];

        if(pmu_volt_sdio[i].flag)
        {
            regval = (u8)((u32)0x1 << hi6551_volt->enable_bit_offset);
            bsp_hi6551_reg_write(hi6551_volt->enable_reg_addr, regval);
            pmu_volt_sdio[i].flag = 0;

            if(PMIC_HI6551_LDO22 == volt_id)
                udelay(20);
        }
    }

    return  BSP_PMU_OK;
}

u8 bsp_pmu_version_get(void)
{
    u8 regval = 0;

    bsp_hi6551_reg_read(HI6551_VERSION_OFFSET, &regval);
    return  regval;
}

#elif defined(CONFIG_PMIC_HI6559)

PMU_VOLT_ATTR pmu_volt[]={
    {
        .volt_id = PMIC_HI6559_LDO01,
        .flag = 0,
    },
    {
        .volt_id = PMIC_HI6559_LDO24,
        .flag = 0,
    }
};
PMU_VOLT_ATTR pmu_volt_sdio[]={     /* v711 sdio����ֻ��ҪLDO07��������ҪLDO22 */
    {
        .volt_id = PMIC_HI6559_LDO07,   
        .flag = 0,
    },
};

/*****************************************************************************
* �� �� ��  : hi6559_volt_table_check
* ��������  : ����ѹԴ���Ա��Ƿ����
* �������  : @hi6559_volt_table ��ѹԴ���Ա��ַ
* �������  : ��
* �� �� ֵ  : BSP_PMU_OK:����;   BSP_PMU_ERROR:������
*****************************************************************************/
static __inline__ s32 hi6559_volt_table_check(PMIC_HI6559_VLTGS_TABLE *hi6559_volt_table)
{
    if((SHM_PMU_VOLTTABLE_MAGIC_START_DATA == hi6559_volt_table->magic_start) &&
        (SHM_PMU_VOLTTABLE_MAGIC_END_DATA == hi6559_volt_table->magic_end))
    {
        return BSP_PMU_OK;
    }
    else
    {
        return BSP_PMU_ERROR;
    }
}

/*****************************************************************************
 �� �� ��  : bsp_hi6559_reg_write
 ��������  : д�Ĵ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
void bsp_hi6559_reg_write( u16 addr, u8 value)
{
    writel((u32) value,(u32)(HI_PMUSSI0_REGBASE_ADDR + (addr << 2)));
}

/*****************************************************************************
 �� �� ��  : bsp_hi6559_reg_read
 ��������  : ���Ĵ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
void  bsp_hi6559_reg_read( u16 addr, u8 *pValue)
{   
    /*lint !e958*/
    *pValue = (u8)readl((u32)(HI_PMUSSI0_REGBASE_ADDR + (addr << 2)));
}

/*****************************************************************************
 �� �� ��  : bsp_pmu_suspend
 ��������  : ��˯
 �������  : ��
 �������  : ��
 �� �� ֵ  : BSP_PMU_OK: �ɹ��� else:ʧ��
*****************************************************************************/
s32 bsp_pmu_suspend(void)
{
    PMIC_HI6559_VLTGS_TABLE *hi6559_volt_table = (PMIC_HI6559_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6559_VLTGS_ATTR *hi6559_volt;
    u32 i = 0 ;
    u8 volt_id = 0;
    u8 regval = 0;

    /* ��ѹ���Ա�ȫ�Լ�� */
    if(BSP_PMU_OK != hi6559_volt_table_check(hi6559_volt_table))
    {
        return BSP_PMU_VOLTTABLE_ERROR;
    }

    /* �ص���Ҫ�رյĵ�Դ */
    for(i = 0;i < sizeof(pmu_volt)/(sizeof((pmu_volt)[0]));i++)
    {
        volt_id = pmu_volt[i].volt_id;
        hi6559_volt = &hi6559_volt_table->hi6559_volt_attr[volt_id];

        bsp_hi6559_reg_read(hi6559_volt->is_enabled_reg_addr, &regval);
        if(regval & ((u32)0x1 << hi6559_volt->is_enabled_bit_offset))
        {
            regval = (u8)((u32)0x1 << hi6559_volt->disable_bit_offset);
            bsp_hi6559_reg_write(hi6559_volt->disable_reg_addr, regval);
            pmu_volt[i].flag = PMU_VOLT_DISABLED;
        }
    }

    return  BSP_PMU_OK;
}

/*****************************************************************************
 �� �� ��  : bsp_pmu_resume
 ��������  : ����
 �������  : ��
 �������  : ��
 �� �� ֵ  : BSP_PMU_OK: �ɹ��� else:ʧ��
*****************************************************************************/
s32 bsp_pmu_resume(void)
{
    PMIC_HI6559_VLTGS_TABLE *hi6559_volt_table = (PMIC_HI6559_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6559_VLTGS_ATTR *hi6559_volt;
    s32 iret = BSP_PMU_OK;
    s32 i = 0 ;
    u8 volt_id = 0;
    u8 regval = 0;

    /* ��ѹ���Ա�ȫ�Լ�� */
    iret = hi6559_volt_table_check(hi6559_volt_table);
    if(BSP_PMU_OK != iret)
    {
        return  BSP_PMU_VOLTTABLE_ERROR;
    }

    for(i = (signed)(sizeof(pmu_volt)/(sizeof((pmu_volt)[0])) - 1); i >= 0; i--)
    {
        volt_id = pmu_volt[i].volt_id;
        hi6559_volt = &hi6559_volt_table->hi6559_volt_attr[volt_id];

        if(pmu_volt[i].flag)
        {
            regval = (u8)((u32)0x1 << hi6559_volt->enable_bit_offset);
            bsp_hi6559_reg_write(hi6559_volt->enable_reg_addr, regval);
            pmu_volt[i].flag = PMU_VOLT_ENABLED;
        }
    }

    return  BSP_PMU_OK;
}

/*****************************************************************************
 �� �� ��  : bsp_pmu_sdio_suspend
 ��������  : SDIO��˯
 �������  : ��
 �������  : ��
 �� �� ֵ  : BSP_PMU_OK: �ɹ��� else:ʧ��
*****************************************************************************/
s32 bsp_pmu_sdio_suspend(void)
{
    PMIC_HI6559_VLTGS_TABLE *hi6559_volt_table = (PMIC_HI6559_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6559_VLTGS_ATTR *hi6559_volt;
    s32 iret = BSP_PMU_OK;
    u32 i = 0 ;
    u8 volt_id = 0;
    u8 regval = 0;

    /* ��ѹ���Ա�ȫ�Լ�� */
    iret = hi6559_volt_table_check(hi6559_volt_table);
    if(BSP_PMU_OK != iret)
    {
        return  BSP_PMU_VOLTTABLE_ERROR;
    }

    for(i = 0;i < sizeof(pmu_volt_sdio)/(sizeof((pmu_volt_sdio)[0]));i++)
    {
        volt_id = pmu_volt_sdio[i].volt_id;
        hi6559_volt = &hi6559_volt_table->hi6559_volt_attr[volt_id];

        bsp_hi6559_reg_read(hi6559_volt->is_enabled_reg_addr, &regval);
        if(regval & ((u32)0x1 << hi6559_volt->is_enabled_bit_offset))
        {
            regval = (u8)((u32)0x1 << hi6559_volt->disable_bit_offset);
            bsp_hi6559_reg_write(hi6559_volt->disable_reg_addr, regval);
            pmu_volt_sdio[i].flag = 1;
        }
    }

     return  BSP_PMU_OK;
}

/*****************************************************************************
 �� �� ��  : bsp_pmu_sdio_resume
 ��������  : SDIO����
 �������  : ��
 �������  : ��
 �� �� ֵ  : BSP_PMU_OK: �ɹ��� else:ʧ��
*****************************************************************************/
s32 bsp_pmu_sdio_resume(void)
{
    s32 iret = BSP_PMU_OK;
    PMIC_HI6559_VLTGS_TABLE *hi6559_volt_table = (PMIC_HI6559_VLTGS_TABLE *)SHM_PMU_VOLTTABLE_ADDR;
    PMIC_HI6559_VLTGS_ATTR *hi6559_volt;
    s32 i = 0 ;
    u8 volt_id = 0;
    u8 regval = 0;


    /* ��ѹ���Ա�ȫ�Լ�� */
    iret = hi6559_volt_table_check(hi6559_volt_table);
    if(BSP_PMU_OK != iret)
    {
        return  BSP_PMU_VOLTTABLE_ERROR;
    }

    for(i = (signed)(sizeof(pmu_volt_sdio)/(sizeof((pmu_volt_sdio)[0])) - 1);i >= 0;i--)
    {
        volt_id = pmu_volt_sdio[i].volt_id;
        hi6559_volt = &hi6559_volt_table->hi6559_volt_attr[volt_id];

        if(pmu_volt_sdio[i].flag)
        {
            regval = (u8)((u32)0x1 << hi6559_volt->enable_bit_offset);
            bsp_hi6559_reg_write(hi6559_volt->enable_reg_addr, regval);
            pmu_volt_sdio[i].flag = 0;
        }
    }

    return  BSP_PMU_OK;
}

/*****************************************************************************
 �� �� ��  : bsp_pmu_version_get
 ��������  : ��ȡ�汾��
 �������  : ��
 �������  : ��
 �� �� ֵ  : BSP_PMU_OK: �ɹ��� else:ʧ��
*****************************************************************************/
u8 bsp_pmu_version_get(void)
{
    u8 regval = 0;

    bsp_hi6559_reg_read(HI6559_VERSION_OFFSET, &regval);
    return regval;
}
#else
s32 bsp_pmu_suspend(void)
{
    return 0;
}
s32 bsp_pmu_resume(void)
{
    return 0;
}
s32 bsp_pmu_sdio_suspend(void)
{
    return 0;
}
s32 bsp_pmu_sdio_resume(void)
{
    return 0;
}
u8 bsp_pmu_version_get(void)
{
    return 0;
}
#endif