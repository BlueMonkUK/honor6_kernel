/*************************************************************************
*   ��Ȩ����(C) 1987-2020, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  efuse.c
*
*   ��    �� :  wuzechun
*
*   ��    �� :  Efuse��ȡ
*
*   �޸ļ�¼ :  2011��6��9��  v1.00  wuzechun  ����
*
*************************************************************************/
#include "osl_bio.h"
#include "soc_clk.h"
#include "bsp_memmap.h"
#include "hi_base.h"
#include "hi_syscrg_interface.h"
#include "hi_efuse.h"
#include "hi_syssc.h"
#include "bsp_pmu.h"
#include "time.h"
#include "boot/boot.h"
#include "efuse.h"

#define TIMEMAXDELAY        (0x1000000)       /*����ӳ�ʱ��*/
/*����efuseÿ����д��ɣ���������ܶ���efuse���ݣ�
* �޷�һ�������У���֤�ô���д�Ƿ���ȷ��Ŀǰ������
* console.c�������Ӧ���ƴ��룬����efuse���Ժ�����
* ��������֤��д�Ƿ���ȷ��
*/
#define EFUSE_TEST_ENABLE

typedef unsigned int (*punptr)(void);

/*********************************************************************************************
Function:           test_bit_value
Description:        ���ԼĴ����е�ĳλ���Ƿ����ָ����ֵ
Calls:              ��
Data Accessed:      ��
Data Updated:       ��
Input:              addr��  �Ĵ�����ַ
                    bitMask����Ҫ����λ��
                    bitValue��ָ����ֵ
Output:             ��
Return:             �ɹ��򷵻�TRUE�� ʧ���򷵻�FALSE
Others:             ��
*********************************************************************************************/
int test_bit_value(punptr pfun,  u32 bitValue)
{
    u32 time;                       /*��ʱʱ��*/

    for(time = 0; time < TIMEMAXDELAY; time++)
    {
        if(bitValue == (*pfun)())
        {
            return OK;
        }
    }

    return ERROR;
}

/*****************************************************************************
* �� �� ��  : bsp_efuse_read
*
* ��������  : �����ȡEFUSE�е�����
*
* �������  : group  ��ʼgroup
*                   num  ���鳤��(word��,������512/4=128)
* �������  : pBuf ��EFUSE�е�����
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
int bsp_efuse_read(u32* pBuf, u32 group, u32 num)
{
    u32* pSh;
    u32 cnt;


    if((group + num > EFUSE_MAX_SIZE) || (NULL == pBuf))
    {
        cprintf(" Line %d, FUNC %s fail .\n", (int)__LINE__, (int)__FUNCTION__);

        return ERROR;
    }

    /* efuse clk enable */
    hi_syscrg_efuse_clk_enble();

    /*�ж��Ƿ���power_down״̬*/
    if(get_hi_efusec_status_pd_status())
    {
       /*�˳�powerd_down״̬*/
        set_hi_efusec_cfg_pd_en(0);

        /* �ȴ�powerd_down�˳���־���óɹ�������ʱ���ش��� */
        if(OK != test_bit_value(get_hi_efusec_status_pd_status, 0))
        {
            cprintf(" Line %d, FUNC %s fail .\n", (int)__LINE__, (int)__FUNCTION__);
            goto efuse_err;
        }
    }

#ifdef HI_RF_STATUS_OFFSET
    /*����efuse�ӿ�ʱ��Ϊ�ڲ�����*/
    set_hi_efusec_cfg_signal_sel(1);
#else
    /*ѡ���ź�Ϊapb����efuse*/
    set_hi_efusec_cfg_aib_sel(1);
#endif

    pSh = pBuf;

    /* ѭ����ȡefuseֵ */
    for(cnt = 0; cnt < num; cnt++)
    {
        /* ���ö�ȡ��ַ */
        set_hi_efuse_group_efuse_group( group+cnt);

        /* ʹ�ܶ� */
        set_hi_efusec_cfg_rden(1);

        /* �ȴ������ */
        if(OK != test_bit_value(get_hi_efusec_status_rd_status, 1))
        {
            cprintf(" Line %d, FUNC %s fail .\n", (int)__LINE__, (int)__FUNCTION__);
            goto efuse_err;
        }
        /* ��ȡ���� */
        *pSh = get_hi_efusec_data_efusec_data();
        pSh++;

    }
    /* efuse clk disable */
    hi_syscrg_efuse_clk_disable();
    return OK;

efuse_err:
    /* efuse clk disable */
    hi_syscrg_efuse_clk_disable();
    return ERROR;
}

/*****************************************************************************
* �� �� ��  : bsp_efuse_Write
*
* ��������  : ��дEfsue
*
* �������  : pBuf:����д��EFUSEֵ
*                 group,Efuse��ַƫ��
*                 len,��д����
* �������  :
*
* �� �� ֵ  :
*
* ����˵��  :
*
*****************************************************************************/
int bsp_efuse_write( u32 *pBuf, u32 group, u32 len)
{

    u32 u32Grp;
    u32 *pu32Value;
    u32 i;
    int iRet = BSP_PMU_OK;
    int volt = 1800000;

    if((group + len > EFUSE_MAX_SIZE) || (NULL == pBuf))
    {
        cprintf(" Line %d, FUNC %s fail .\n", (int)__LINE__, (int)__FUNCTION__);
        return ERROR;
    }

    iRet = bsp_pmu_efuse_volt_set(volt);
    if(BSP_PMU_OK != iRet)
    {
        cprintf(" efuse power voltage error!\n", (int)__LINE__, (int)__FUNCTION__);
        return ERROR;
    }
    iRet = bsp_pmu_efuse_power_switch(POWER_ON);
    if(BSP_PMU_OK != iRet)
    {
        cprintf(" efuse power on failed!\n", (int)__LINE__, (int)__FUNCTION__);
        return ERROR;
    }

    /* efuse clk enable */
    hi_syscrg_efuse_clk_enble();

    mdelay(1);

    /*�ж��Ƿ���power_down״̬*/
    if(get_hi_efusec_status_pd_status())
    {
       /*�˳�powerd_down״̬*/
        set_hi_efusec_cfg_pd_en(0);
        /* �ȴ�powerd_down�˳���־���óɹ�������ʱ���ش��� */
        if(OK != test_bit_value(get_hi_efusec_status_pd_status, 0))
        {
            cprintf(" Line %d, FUNC %s fail .\n", (int)__LINE__, (int)__FUNCTION__);
            goto efusee_err;
        }
    }

    /* �ж��Ƿ�������д,Ϊ1��ֹ��д,ֱ���˳� */
    if(get_hi_hw_cfg_hw_cfg() & 0x1)
    {
        cprintf(" Line %d, FUNC %s fail .\n", (int)__LINE__, (int)__FUNCTION__);
        goto efusee_err;
    }

#ifdef HI_RF_STATUS_OFFSET
    /*����efuse�ӿ�ʱ��Ϊ�ڲ�����*/
    set_hi_efusec_cfg_signal_sel(1);
#else
    /*ѡ���ź�Ϊapb����efuse*/
    set_hi_efusec_cfg_aib_sel(1);
#endif

    /* ����ʱ��Ҫ�� */
    set_hi_efusec_count_efusec_count(EFUSE_COUNT_CFG);
    set_hi_pgm_count_pgm_count(PGM_COUNT_CFG);

    /* ʹ��Ԥ��д */
    set_hi_efusec_cfg_pre_pg(1);

    /*��ѯ�Ƿ�ʹ����λ */
    if(OK != test_bit_value(get_hi_efusec_status_pgenb_status, 1))
    {
        cprintf(" Line %d, FUNC %s fail .\n", (int)__LINE__, (int)__FUNCTION__);
        goto efusee_err;
    }

    /* ѭ����д */
    u32Grp = group;
    pu32Value = pBuf;
    for (i = 0; i < len; i++)
    {
        /* ����group */
        set_hi_efuse_group_efuse_group(u32Grp);

        /* ����value */
        set_hi_pg_value_pg_value(*pu32Value);

        /* ʹ����д */
        set_hi_efusec_cfg_pgen(1);

        /* ��ѯ��д��� */
        if(OK != test_bit_value(get_hi_efusec_status_pg_status, 1))
        {
            cprintf(" Line %d, FUNC %s fail .\n", (int)__LINE__, (int)__FUNCTION__);
            goto efusee_err;
        }

        /* ��д��һ�� */
        u32Grp++;
        pu32Value++;
    }

    /*ȥʹ��Ԥ��д*/
    set_hi_efusec_cfg_pre_pg(0);

    /* efuse clk disable */
    hi_syscrg_efuse_clk_disable();

    bsp_pmu_efuse_power_switch(POWER_OFF);

    return OK;
    
efusee_err:
    /* efuse clk disable */
    hi_syscrg_efuse_clk_disable();

    bsp_pmu_efuse_power_switch(POWER_OFF);

    return ERROR;
}

void efuse_info_show(void)
{
    u32 i = 0;
    u32 value = 0;

    for(i = 0;i < EFUSE_MAX_SIZE;i++)
    {
        if(OK == bsp_efuse_read(&value, i, 1))
        {
            cprintf("efuse group%d value = 0x%x.\n ", i,value);
        }
        else
        {
            cprintf("Line %d, FUNC %s , efuse group%d read fail.\n", (int)__LINE__, (int)__FUNCTION__, i);
            return;
        }
    }

}

/*efuse���Ժ�������дgroup0~15�����ݣ������ص���*/
#ifdef EFUSE_TEST_ENABLE

u32 efuseTestWriteData[] =
{
    /* random value */
    0x73c6200a, 0x41987331, 0x7b63350e, 0x25b8e5c5, 0x4aec85e2,
    0x73c6200a, 0x41987331, 0x7b63350e, 0x25b8e5c5, 0x4aec85e2,
    0x73c6200a, 0x41987331, 0x7b63350e, 0x25b8e5c5, 0x4aec85e2,
    0x0000FF00
};

u32 efuseTestReadData[EFUSE_MAX_SIZE] = {0};

int efuse_test_separate_write(void)
{
    int i =0;
    for(i = 0; i < EFUSE_MAX_SIZE; i++)
    {
        if(ERROR == bsp_efuse_write(&(efuseTestWriteData[i]), i, 1))
        {
            cprintf(" Line %d, FUNC %s, group %d, efuse data 0x%x fail .\n", (int)__LINE__, (int)__FUNCTION__, i, efuseTestWriteData[i]);
            return ERROR;
        }

    }

    return OK;
}

int efuse_test_separate_read(void)
{
    int i =0;

    for(i = 0; i < EFUSE_MAX_SIZE; i++)
    {
        efuseTestReadData[i] = 0;
        if(ERROR == bsp_efuse_read(&(efuseTestReadData[i]), i, 1))
        {
            cprintf(" Line %d, FUNC %s, group %d, efuse data 0x%x fail .\n", (int)__LINE__, (int)__FUNCTION__, i, efuseTestReadData[i]);
            return ERROR;
        }
    }

    for(i = 0; i < EFUSE_MAX_SIZE; i++)
    {
        if(efuseTestWriteData[i] != efuseTestReadData[i])
        {
            cprintf(" Line %d, FUNC %s , efuseTestWriteData == efuseTestReadData fail .\n", (int)__LINE__, (int)__FUNCTION__);
            return ERROR;
        }
    }

    return OK;

}

int efuse_test_continue_write(void)
{
    if(ERROR == bsp_efuse_write(efuseTestWriteData, 0, EFUSE_MAX_SIZE))
    {
        cprintf(" Line %d, FUNC %s fail .\n", (int)__LINE__, (int)__FUNCTION__);
        return ERROR;
    }

    return OK;
}

int efuse_test_continue_read(void)
{
    int i = 0;
    for(i = 0; i < EFUSE_MAX_SIZE; i++)
    {
        efuseTestReadData[i] = 0;
    }

    if(ERROR == bsp_efuse_read(efuseTestReadData, 0, EFUSE_MAX_SIZE))
    {
        cprintf(" Line %d, FUNC %s fail .\n", (int)__LINE__, (int)__FUNCTION__);
        return ERROR;
    }

    for(i = 0; i < EFUSE_MAX_SIZE; i++)
    {
        if(efuseTestWriteData[i] != efuseTestReadData[i])
        {
            cprintf(" Line %d, FUNC %s fail .\n", (int)__LINE__, (int)__FUNCTION__);
            return ERROR;
        }
    }

    return OK;

}

#ifndef HI_RF_STATUS_OFFSET
int efuse_test_auto_read()
{
    u32 hwCfgValue = 0;
    u32 tsensorTrimValue = 0;
    u32 efuseGroup14Value = 0;
    u32 efuseGroup15Value = 0;

    if(ERROR == bsp_efuse_read(&efuseGroup15Value, 15, 1))
    {
        cprintf(" Line %d, FUNC %s fail .\n", (int)__LINE__, (int)__FUNCTION__);
        return ERROR;
    }

    hwCfgValue = get_hi_hw_cfg_hw_cfg();

    if(hwCfgValue != efuseGroup15Value)
    {
        cprintf(" Line %d, FUNC %s fail .\n", (int)__LINE__, (int)__FUNCTION__);
        return ERROR;
    }

    if(ERROR == bsp_efuse_read(&efuseGroup14Value, 14, 1))
    {
        cprintf(" Line %d, FUNC %s fail .\n", (int)__LINE__, (int)__FUNCTION__);
        return ERROR;

    }

    tsensorTrimValue = readl(HI_SC_STAT86_OFFSET);

    if(efuseGroup14Value != tsensorTrimValue)
    {
        cprintf(" Line %d, FUNC %s fail .\n", (int)__LINE__, (int)__FUNCTION__);
        return ERROR;
    }

    return OK;
}
#endif

int efuse_test_repeat_write(void)
{
    int i =0;
    u32 efuseValue = 0;
    for(i = 0; i < EFUSE_MAX_SIZE; i++)
    {
        efuseValue = efuseTestWriteData[i];
        if(ERROR == bsp_efuse_write(&efuseValue, i, 1))
        {
            cprintf(" Line %d, FUNC %s, group %d, efuse data 0x%x fail .\n", (int)__LINE__, (int)__FUNCTION__, i, efuseValue);
            return ERROR;
        }

        efuseValue = ~(efuseTestWriteData[i]);

        if(ERROR == bsp_efuse_write(&efuseValue, i, 1))
        {
            cprintf(" Line %d, FUNC %s, group %d, efuse data 0x%x fail .\n", (int)__LINE__, (int)__FUNCTION__, i, efuseValue);
            return ERROR;
        }

        if(ERROR == bsp_efuse_read(&efuseValue, i, 1))
        {
            cprintf(" Line %d, FUNC %s, group %d, efuse data 0x%x fail .\n", (int)__LINE__, (int)__FUNCTION__, i, efuseValue);
            return ERROR;
        }

        if(0xFFFFFFFF != efuseValue)
        {
            cprintf(" Line %d, FUNC %s, group %d, efuse data 0x%x fail .\n", (int)__LINE__, (int)__FUNCTION__, i, efuseValue);
            return ERROR;
        }

    }

    return OK;
}

int efuse_write_test(void)
{
    u32 efuseValue[] =
    {
        /* ROOT Key Hash value */
        0x45003997, 0x68229d1a, 0xec67902c, 0xf4240b7a
    };

    if(ERROR == bsp_efuse_write(efuseValue, 0, sizeof(efuseValue)/sizeof(efuseValue[0])))
    {
        cprintf(" Line %d, FUNC %s fail .\n", (int)__LINE__, (int)__FUNCTION__);
        return ERROR;
    }

    return OK;
}

int efuse_read_test(void)
{
    u32 efuseValue[4] = {0};

    if(ERROR == bsp_efuse_read(0, efuseValue, 4))
    {
        cprintf(" Line %d, FUNC %s fail .\n", (int)__LINE__, (int)__FUNCTION__);
        return ERROR;
    }

    if(0x45003997 !=efuseValue[0] || (0x68229d1a !=efuseValue[1]) || (0xec67902c !=efuseValue[2])
      || (0xf4240b7a !=efuseValue[3]))
    {
        cprintf(" Line %d, FUNC %s fail .\n", (int)__LINE__, (int)__FUNCTION__);
        return ERROR;
    }

    return OK;
}

#endif

