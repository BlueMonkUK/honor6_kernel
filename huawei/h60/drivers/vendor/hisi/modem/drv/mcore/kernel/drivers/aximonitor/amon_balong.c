

#ifdef __cplusplus
extern "C"
{
#endif

#include <osl_bio.h>
#include "amon_balong.h"
#include "hi_syscrg_interface.h"
#include "hi_syssc_interface.h"
#include "printk.h"
#include "bsp_version.h"

/* ���ݻָ����� */
axi_reg_bak_t * g_st_reg_bak = (axi_reg_bak_t *)AXI_MON_BACK_ADDR;
/* ����ǰ���б�־ */
u32 g_ul_running[2] = {false, false};
u32 g_ul_bak_flag[2] = {false, false};
/*****************************************************************************
 �� �� ��  : axi_reg_read
 ��������  : AXI�Ĵ���������, һ��ֻ�ܶ�һ��Ĵ�����CPUFAST/SOC��
 �������  : axi_config_enum_uint32 config  
             u32 reg                                              
 �������  : u32 * value
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 �޸���ʷ      :
  1.��    ��   : 2013��6��6��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
static void axi_reg_read(axi_config_enum_uint32 config, u32 reg, u32 * value)
{   
    /* ��������Ŀ���ȡ��Ӧ�Ĵ��� */
    switch(config)
    {
        /* ��ȡCPUFAST�Ĵ��� */
        case AXI_CPUFAST_CONFIG:
            *value = readl(AXI_MON_CPUFAST_BASE_ADDR + reg); 
            break;
            
        /* ��ȡSOC�Ĵ��� */    
        case AXI_SOC_CONFIG:
            *value = readl(AXI_MON_SOC_BASE_ADDR + reg); 
            break; 

        default:
            break;
    }    
}

/*****************************************************************************
 �� �� ��  : axi_reg_write
 ��������  : AXI�Ĵ���д������һ��ֻ��дһ��Ĵ�����CPUFAST/SOC��
 �������  : axi_config_enum_uint32 config  
             u32 reg                   
             u32 value                 
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��6��6��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
static void axi_reg_write(axi_config_enum_uint32 config, u32 reg, u32 value)
{
    /* ��������Ŀ��д����Ӧ�Ĵ��� */
    switch(config)
    {
        /* дCPUFAST�Ĵ��� */
        case AXI_CPUFAST_CONFIG:
            writel(value, AXI_MON_CPUFAST_BASE_ADDR + reg); 
            break;

        /* дSOC�Ĵ��� */     
        case AXI_SOC_CONFIG:
            writel(value, AXI_MON_SOC_BASE_ADDR + reg); 
            break; 

        default:
            break;
    } 
}

/*****************************************************************************
 �� �� ��  : axi_is_clk_enable
 ��������  : ��ȡAXI monitorʱ��״̬
 �������  : axi_config_enum_uint32 config            
 �������  : ��
 �� �� ֵ  : u32 1 --- ʱ��ʹ��, 0 --- ʱ��δʹ��
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��6��6��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
static u32 axi_is_clk_enable(axi_config_enum_uint32 config)
{
    if(AXI_CPUFAST_CONFIG == config)
    {
        return hi_syscrg_amon_cpufast_get_clk_status();
    }

    if(AXI_SOC_CONFIG == config)
    {
        return hi_syscrg_amon_soc_get_clk_status();
    }

    return 1;
}

/*****************************************************************************
 �� �� ��  : axi_clk_enable
 ��������  : AXI monitorʱ��ʹ��
 �������  : axi_config_enum_uint32 config            
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��6��6��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
static void axi_clk_enable(axi_config_enum_uint32 config)
{
    if(AXI_CPUFAST_CONFIG == config)
    {
        hi_syscrg_amon_cpufast_clk_enable();
    }

    if(AXI_SOC_CONFIG == config)
    {
        hi_syscrg_amon_soc_clk_enable();
    }
}

/*****************************************************************************
 �� �� ��  : axi_clk_disable
 ��������  : AXI monitorȥʱ��
 �������  : axi_config_enum_uint32 config            
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��6��6��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
static void axi_clk_disable(axi_config_enum_uint32 config)
{
    if(AXI_CPUFAST_CONFIG == config)
    {
        hi_syscrg_amon_cpufast_clk_disable();
    }

    if(AXI_SOC_CONFIG == config)
    {
        hi_syscrg_amon_soc_clk_disable();
    }
}

/*****************************************************************************
 �� �� ��  : axi_get_state
 ��������  : ��ȡAXI monitor����״̬
 �������  : axi_config_enum_uint32 config            
 �������  : ��
 �� �� ֵ  : axi_state_enum_uint32
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��6��6��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
static axi_state_enum_uint32 axi_get_state(axi_config_enum_uint32 config)
{
    u32 reg_value;
    
    /* ��״̬�Ĵ�����ȡ����״̬ */
    axi_reg_read(config, AXI_MON_CNT_STATE_INT, &reg_value);
    
    return reg_value & 0xC;
}

/*****************************************************************************
 �� �� ��  : axi_reset
 ��������  : AXI monitor�ڲ���λ
 �������  : axi_config_enum_uint32 config            
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��6��6��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
static void axi_reset(axi_config_enum_uint32 config)
{
    /* ��λ */
    axi_reg_write(config, AXI_MON_CNT_RESET, 0x2);
}

/*****************************************************************************
 �� �� ��  : axi_sc_mon_start
 ��������  : AXI Monitor����ͨ��ϵͳ���������� 
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��6��6��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
static void axi_sc_mon_start(axi_config_enum_uint32 config)
{
    /* ����CPUFAST */
    if(AXI_CPUFAST_CONFIG == config)
    {
        hi_syssc_amon_cpufast_start();
    }
    /* ����SOC */
    else if(AXI_SOC_CONFIG == config)
    {
        hi_syssc_amon_soc_start();
    }
    else
    {
        return;
    }
}

/*****************************************************************************
 �� �� ��  : axi_sc_mon_stop
 ��������  : AXI Monitorֹͣͨ��ϵͳ���������� 
 �������  : ��
 �� �� ֵ  : void
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��6��6��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
static void axi_sc_mon_stop(axi_config_enum_uint32 config)
{
    /* ֹͣCPUFAST */
    if(AXI_CPUFAST_CONFIG == config)
    {
        hi_syssc_amon_cpufast_stop();
    }
    /* ֹͣSOC */   
    else if(AXI_SOC_CONFIG == config)
    {
        hi_syssc_amon_soc_stop();
    }
    else
    {
        return;
    }
}

/*****************************************************************************
 �� �� ��  : bsp_amon_suspend
 ��������  : ����ǰ���ݱ���
 �������  : �� 
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��6��6��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
void bsp_amon_suspend()
{
    u32 i;
    u32 j;
    u32 reg_value;
    /*lint -save -e958*/
    unsigned long long data_cnt;
    /*lint -restore*/
    u32 data_low;
    u32 data_high;
    axi_state_enum_uint32 state;
    u32 clk_status;
    axi_reg_bak_t * reg_bak;

    for(i=0; i<AXI_CONFIG_BUTT; i++)
    {
        reg_bak = &g_st_reg_bak[i];
        clk_status = axi_is_clk_enable(i);
        if(!clk_status)
        {
            axi_clk_enable(i);
        }
        
        state = axi_get_state(i);
        if(AXI_UNWIN_RUNNING != state)
        {
            g_ul_running[i] = false;

            if(!clk_status)
            {
                axi_clk_disable(i);
            } 
            
            continue;
        }

        /* ����ǰ����ֹͣmonitor */
        axi_sc_mon_stop(i);
        g_ul_running[i] = true;
        
        /* ���ƼĴ������� */
        axi_reg_read(i, AXI_MON_CNT_CTRL, &(reg_bak->ctrl_reg));
        axi_reg_read(i, AXI_MON_PORT_SEL, &(reg_bak->port_reg));
        axi_reg_read(i, AXI_ID_INT_MASK, &(reg_bak->int_mask));
        for(j=0; j<AXI_MAX_CONFIG_ID; j++)
        {
            axi_reg_read(i, AXI_MON_CNT_ID(j), &(reg_bak->id_reg[j]));
            axi_reg_read(i, AXI_MON_ID_ADDR_DES(j), &(reg_bak->addr_start_reg[j]));
            axi_reg_read(i, AXI_MON_ID_ADDR_DES_M(j), &(reg_bak->addr_end_reg[j]));
        }
        
        /* ��һ���µ磬��ձ����� */
        if(g_ul_bak_flag[i] == false)
        {
            g_ul_bak_flag[i] = true;
            for(j=0; j<AXI_MAX_CONFIG_ID; j++)
            {
                reg_bak->incr1_reg[j] = 0;
                reg_bak->incr2_reg[j] = 0;
                reg_bak->incr4_reg[j] = 0;
                reg_bak->incr16_reg[j] = 0;
                reg_bak->wrap_reg[j] = 0;
                reg_bak->burst_send_reg[j] = 0;
                reg_bak->burst_fin_reg[j] = 0;
                reg_bak->rd_cnt_high_reg[j] = 0;
                reg_bak->rd_cnt_low_reg[j] = 0;
                reg_bak->wr_cnt_high_reg[j] = 0;
                reg_bak->wr_cnt_low_reg[j] = 0;
            }
        }
        
        /* ͳ�ƼĴ����ۼ� */
        for(j=0; j<AXI_MAX_CONFIG_ID; j++)
        {
            axi_reg_read(i, AXI_MON_INCR1_ID(j), &reg_value);
            reg_bak->incr1_reg[j] += reg_value;
            axi_reg_read(i, AXI_MON_INCR2_ID(j), &reg_value);
            reg_bak->incr2_reg[j] += reg_value; 
            axi_reg_read(i, AXI_MON_INCR4_ID(j), &reg_value);
            reg_bak->incr4_reg[j] += reg_value; 
            axi_reg_read(i, AXI_MON_INCR8_ID(j), &reg_value);
            reg_bak->incr8_reg[j] += reg_value;  
            axi_reg_read(i, AXI_MON_INCR16_ID(j), &reg_value);
            reg_bak->incr16_reg[j] += reg_value; 
            axi_reg_read(i, AXI_MON_WRAP_ID(j), &reg_value);
            reg_bak->wrap_reg[j] += reg_value; 
            axi_reg_read(i, AXI_MON_BURST_ID(j), &reg_value);
            reg_bak->burst_send_reg[j] += reg_value; 
            axi_reg_read(i, AXI_MON_FINISH_ID(j), &reg_value);
            reg_bak->burst_fin_reg[j] += reg_value; 
            /* ��д��������40bit���ȣ���Ҫ�������ֵ */
            axi_reg_read(i, AXI_MON_RD_BYTES_ID_LOW(j), &data_low);
            axi_reg_read(i, AXI_MON_RD_BYTES_ID_HIGH(j), &data_high); 
            data_cnt = (unsigned long long)data_low + (unsigned long long)reg_bak->rd_cnt_low_reg[j];
            reg_bak->rd_cnt_high_reg[j] += data_high; 
            if(data_cnt >> 32)
            {
                reg_bak->rd_cnt_high_reg[j] += 1;
            }
            reg_bak->rd_cnt_low_reg[j] = (u32)data_cnt;
            axi_reg_read(i, AXI_MON_WR_BYTES_ID_LOW(j), &data_low);
            axi_reg_read(i, AXI_MON_WR_BYTES_ID_HIGH(j), &data_high); 
            data_cnt = (unsigned long long)data_low + (unsigned long long)reg_bak->wr_cnt_low_reg[j];
            reg_bak->wr_cnt_high_reg[j] += data_high; 
            if(data_cnt >> 32)
            {
                reg_bak->wr_cnt_high_reg[j] += 1;
            }
            reg_bak->wr_cnt_low_reg[j] = (u32)data_cnt;            
        }

        if(!clk_status)
        {
            axi_clk_disable(i);
        }        
    }
}

/*****************************************************************************
 �� �� ��  : bsp_amon_resume
 ��������  : �ϵ�ָ�
 �������  : �� 
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��6��6��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
void bsp_amon_resume()
{
    u32 i;
    u32 j;
    u32 clk_status;
    axi_reg_bak_t * reg_bak;

    for(i=0; i<AXI_CONFIG_BUTT; i++)
    {
        /* ����ǰmonitorδ���У���ִ�лָ����� */
        if(false == g_ul_running[i])
        {			
            continue;
        }  

        reg_bak = &g_st_reg_bak[i];
        clk_status = axi_is_clk_enable(i);
        if(!clk_status)
        {
            axi_clk_enable(i);
        }

        axi_reset(i);
        
        axi_reg_write(i, AXI_MON_CNT_CTRL, reg_bak->ctrl_reg);
        axi_reg_write(i, AXI_MON_PORT_SEL, reg_bak->port_reg);
        axi_reg_write(i, AXI_ID_INT_MASK, reg_bak->int_mask);
        for(j=0; j<AXI_MAX_CONFIG_ID; j++)
        {
            axi_reg_write(i, AXI_MON_CNT_ID(j), reg_bak->id_reg[j]);
            axi_reg_write(i, AXI_MON_ID_ADDR_DES(j), reg_bak->addr_start_reg[j]);
            axi_reg_write(i, AXI_MON_ID_ADDR_DES_M(j), reg_bak->addr_end_reg[j]);
        } 

        /* ����monitor */
        axi_sc_mon_start(i);

        if(!clk_status)
        {
            axi_clk_disable(i);
        }         
    }
}


#ifdef __cplusplus
}
#endif


