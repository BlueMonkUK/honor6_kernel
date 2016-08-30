

#ifdef __cplusplus
extern "C"
{
#endif

#include <vxWorks.h>
#include <stdlib.h>
#include <logLib.h>
#include <stdio.h>
#include <errno.h>
#include <errnoLib.h>
#include <memLib.h>
#include <cacheLib.h>
#include <string.h>
#include <taskLib.h>
#include <intLib.h>
#include "osl_irq.h"
#include "hi_syscrg_interface.h"
#include "hi_syssc_interface.h"
#include "bsp_memmap.h"
#include "bsp_om_api.h"
#include "bsp_version.h"
#include "bsp_nvim.h"
#include "drv_nv_id.h"
#include "bsp_dump.h"
#include "bsp_dpm.h"
#include "amon_balong.h"

/* AXI״̬��飬�Ƿ��������У�������ʱ�����������û��߻�ȡͳ������ */
#define AXI_CHECK_STATE(config) \
do{\
    if(BSP_OK != amon_state_check(config))\
    {\
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_AMON, "%s: AXI %d is running\n", __FUNCTION__, config);\
        return BSP_ERROR;\
    }\
}while(0)  

/* axi monitor���� */
amon_config_stru g_amon_config;
/* ���ͳ�� */
amon_stat_t g_amon_stat;
/* log��¼ */
amon_buff_info_t g_amon_buff;

#ifdef BSP_CONFIG_HI3630  
#ifdef CONFIG_CCORE_PM
axi_reg_bak_t g_st_reg_bak;
#endif
#endif

void amon_int_handler(void);
axi_state_enum_uint32 amon_get_state(axi_config_enum_uint32 config, axi_get_state_req_enum_uint32 state_req);
s32 amon_state_check(axi_config_enum_uint32 config);
s32 amon_reset(axi_config_enum_uint32 config);
s32 amon_start(axi_config_enum_uint32 config);
s32 amon_stop(axi_config_enum_uint32 config);
void amon_config(axi_config_enum_uint32 config);
void amon_debug_reset(void);
void amon_debug_show(void);
void amon_save_log(axi_config_enum_uint32 mode, u32 id, u32 opt_type, amon_config_t * config);

void read_test(u32 addr);
void write_test(u32 addr);
void read_test(u32 addr)
{
    amon_debug("read 0x%x: 0x%x", addr, readl(addr));
}
void write_test(u32 addr)
{
    writel(0x9999aaaa, addr);
}

/* K3����modemʵ��dpm, V7R2��M3ʵ�� */
#ifdef BSP_CONFIG_HI3630  
#ifdef CONFIG_CCORE_PM
s32 amon_suspend(struct dpm_device *dev);
s32 amon_resume(struct dpm_device *dev);

struct dpm_device amon_dpm_device={
	.device_name = "amon_dpm",
	.prepare = NULL,
	.suspend_late = NULL,
	.suspend = amon_suspend,
	.resume = amon_resume,
	.resume_early = NULL,
	.complete = NULL,
};
#endif
#endif

/*****************************************************************************
 �� �� ��  : amon_save_log
 ��������  : ƥ�䵽������¼log
             �����ʽ: timestamp    soc/cpufast    id          port 
                       masterid     rd/wr          addr_start  addr_end
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��1��29��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
void amon_save_log(axi_config_enum_uint32 mode, u32 id, u32 opt_type, amon_config_t * config)
{
    u32 * data_wr = (u32 *)(g_amon_buff.buff + g_amon_buff.write_offset);

    *data_wr     = om_timer_get();
    *(data_wr+1) = mode;
    *(data_wr+2) = id;
    *(data_wr+3) = config->port;
    *(data_wr+4) = config->master_id;
    *(data_wr+5) = opt_type;
    *(data_wr+6) = config->start_addr;
    *(data_wr+7) = config->end_addr;

    g_amon_buff.write_offset = (g_amon_buff.write_offset + 0x20)%(g_amon_buff.buff_size);
}

/*****************************************************************************
 �� �� ��  : amon_int_handler
 ��������  : �жϴ������������д�ж�
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��1��29��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
void amon_int_handler(void)
{
    int i;
    u32 reg_value;
    u32 mask;

    /* SOC��д�жϴ��� */
    AXI_REG_READ(AXI_SOC_CONFIG, AXI_ID_INT_STAT, &reg_value);
    AXI_REG_READ(AXI_SOC_CONFIG, AXI_ID_INT_MASK, &mask);
    AXI_REG_WRITE(AXI_SOC_CONFIG, AXI_ID_INT_MASK, (reg_value | mask));
    AXI_REG_WRITE(AXI_SOC_CONFIG, AXI_MON_INT_CLR, reg_value<<16);
    for(i=0; i<AXI_MAX_CONFIG_ID; i++)
    {
        /* д�ж� */
        if(reg_value & (1<<(i*2)))
        {
            g_amon_stat.soc_wr_cnt[i]++;
            amon_save_log(AXI_SOC_CONFIG, i, AMON_OPT_WRITE, &g_amon_config.soc_config[i]);
            if(g_amon_config.soc_config[i].reset_flag == 1)
            {
                amon_error("soc id 0x%x wr hit, reboot\n", i);
                /* �������ж�д�ж� */
                AXI_REG_WRITE(AXI_SOC_CONFIG, AXI_ID_INT_MASK, 0xffff);
                system_error(DRV_ERRNO_AMON_SOC_WR, i, 0, 0, 0);
                return;
            }
            AXI_REG_SETBITS(AXI_SOC_CONFIG, AXI_ID_INT_MASK, i*2, 1, 0);
        }
        /* ���ж� */
        if(reg_value & (1<<(i*2+1)))
        {
            g_amon_stat.soc_rd_cnt[i]++;
            amon_save_log(AXI_SOC_CONFIG, i, AMON_OPT_READ, &g_amon_config.soc_config[i]);
            if(g_amon_config.soc_config[i].reset_flag == 1)
            {
                amon_error("soc id 0x%x rd hit, reboot\n", i);
                /* �������ж�д�ж� */
                AXI_REG_WRITE(AXI_SOC_CONFIG, AXI_ID_INT_MASK, 0xffff);
                system_error(DRV_ERRNO_AMON_SOC_RD, i, 1, 0, 0);
                return;
            }

            AXI_REG_SETBITS(AXI_SOC_CONFIG, AXI_ID_INT_MASK, i*2+1, 1, 0);
        }        
    }

#ifndef BSP_CONFIG_HI3630 
    /* CPUFAST��д�жϴ��� */
    AXI_REG_READ(AXI_CPUFAST_CONFIG, AXI_ID_INT_STAT, &reg_value);
    AXI_REG_READ(AXI_CPUFAST_CONFIG, AXI_ID_INT_MASK, &mask);
    AXI_REG_WRITE(AXI_CPUFAST_CONFIG, AXI_ID_INT_MASK, (mask | reg_value));
    AXI_REG_WRITE(AXI_CPUFAST_CONFIG, AXI_MON_INT_CLR, reg_value<<16);
    for(i=0; i<AXI_MAX_CONFIG_ID; i++)
    {
        /* д�ж� */
        if(reg_value & (1<<(i*2)))
        {
            g_amon_stat.cpufast_wr_cnt[i]++;
            amon_save_log(AXI_CPUFAST_CONFIG, i, AMON_OPT_WRITE, &g_amon_config.cpufast_config[i]);
            if(g_amon_config.cpufast_config[i].reset_flag == 1)
            {
                amon_error("cpufast id 0x%x wr hit, reboot\n", i);
                /* �������ж�д�ж� */
                AXI_REG_WRITE(AXI_CPUFAST_CONFIG, AXI_ID_INT_MASK, 0xffff);
                system_error(DRV_ERRNO_AMON_CPUFAST_WR, i, 2, 0, 0);
                return;
            }
            AXI_REG_SETBITS(AXI_CPUFAST_CONFIG, AXI_ID_INT_MASK, i*2, 1, 0);
        }
        /* ���ж� */
        if(reg_value & (1<<(i*2+1)))
        {
            g_amon_stat.cpufast_rd_cnt[i]++;
            amon_save_log(AXI_CPUFAST_CONFIG, i, AMON_OPT_READ, &g_amon_config.cpufast_config[i]);
            if(g_amon_config.cpufast_config[i].reset_flag == 1)
            {
                amon_error("cpufast id 0x%x rd hit, reboot\n", i);
                /* �������ж�д�ж� */
                AXI_REG_WRITE(AXI_CPUFAST_CONFIG, AXI_ID_INT_MASK, 0xffff);
                system_error(DRV_ERRNO_AMON_CPUFAST_RD, i, 3, 0, 0);
                return;
            }
            AXI_REG_SETBITS(AXI_CPUFAST_CONFIG, AXI_ID_INT_MASK, i*2+1, 1, 0);
        }        
    }
#endif    
}

/*****************************************************************************
 �� �� ��  : amon_get_state
 ��������  : ��ȡAXI monitor����/��λ״̬������ͬʱ��ȡCPUFAST/SOC״̬
 �������  : axi_config_enum_uint32 config
             axi_get_state_req_enum_uint32 state_req
 �������  : ��
 �� �� ֵ  : axi_state_enum_uint32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��1��29��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
axi_state_enum_uint32 amon_get_state(axi_config_enum_uint32 config, axi_get_state_req_enum_uint32 state_req)
{
    u32 reg_value = 0;

    /* ��״̬�Ĵ�����ȡ����״̬ */
    AXI_REG_READ(config, AXI_MON_CNT_STATE_INT, &reg_value);
    /* ��ȡ����״̬���� */
    if(AXI_GET_RUN_STATE_REQ == state_req)
    {
        reg_value = reg_value & AXI_RUN_STATE_MASK;
    }
    /* ��ȡ��λ״̬���� */
    else if(AXI_GET_RESET_STATE_REQ == state_req)
    {
        reg_value = reg_value & AXI_RESET_STATE_MASK;
    }
    /* ��Ч���� */
    else
    {
        return (axi_state_enum_uint32)AXI_STATE_BUTT;
    }

    return reg_value;
}

/*****************************************************************************
 �� �� ��  : amon_state_check
 ��������  : ״̬�ж��������Ƿ���������
 �������  : ��
 �� �� ֵ  : s32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��1��29��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
s32 amon_state_check(axi_config_enum_uint32 config)
{
    axi_state_enum_uint32 axi_state;

    axi_state = axi_get_state(config, AXI_GET_RUN_STATE_REQ);
    /* AXI monitor�������� */
    if(axi_state != AXI_IDLE && axi_state != AXI_STOP)
    {
        return BSP_ERROR;
    }

    return BSP_OK;
}

/*****************************************************************************
 �� �� ��  : amon_reset
 ��������  : monitor�ڲ���λ
 �������  : ��
 �� �� ֵ  : s32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��1��29��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
s32 amon_reset(axi_config_enum_uint32 config)
{
    axi_state_enum_uint32 axi_state;
    int i = 0;

    /* �������״̬ */
    AXI_CHECK_STATE(config);

    /* ��λ */
    AXI_REG_WRITE(config, AXI_MON_CNT_RESET, AXI_SOFT_RESET);
    /* ��λ�����ж���AXI monitor״̬�Ĵ���ָʾ��λ���� */
    do       
    {
        axi_state = axi_get_state(config, AXI_GET_RESET_STATE_REQ);
        if(AXI_RESET_FINISH == axi_state)
        {
            return BSP_OK;
        }
    }while(i++ < AXI_WAIT_CNT);
    
    /* ��λ��ʱ�쳣 */
    amon_error("reset 0x%x time out\n", config);
    return BSP_ERROR;
}

/*****************************************************************************
 �� �� ��  : amon_start
 ��������  : ����AXI monitor����ϵͳ����������
 �������  : axi_config_enum_uint32 config
 �������  : ��
 �� �� ֵ  : s32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��1��29��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
s32 amon_start(axi_config_enum_uint32 config)
{
    axi_state_enum_uint32 axi_state;
    amon_config_t * amon_config;
    int i = 0;
    u32 reg_value = 0xffff;

    /* �������״̬ */
    AXI_CHECK_STATE(config);

    /* ���ж� */
    AXI_REG_WRITE(config, AXI_MON_INT_CLR, 0xFFFFFFFF);

    if(config == AXI_SOC_CONFIG)
    {
        amon_config = (amon_config_t *)g_amon_config.soc_config;
    }
    else
    {
        amon_config = (amon_config_t *)g_amon_config.cpufast_config;
    }
    
    /* �����д�ж����Σ�����NV���ù��� */
    for(i=0; i<AXI_MAX_CONFIG_ID; i++)
    {
        /* ID i��ض����� */
        if(amon_config->opt_type & AMON_OPT_READ)
        {
            reg_value = reg_value & (~(1<<(i*2+1)));
        }
        /* ID i���д���� */
        if(amon_config->opt_type & AMON_OPT_WRITE)
        {
            reg_value = reg_value & (~(1<<(i*2)));
        }
        amon_config++;
    }

    AXI_REG_WRITE(config, AXI_ID_INT_MASK, reg_value);
    
    /* ����ϵͳ����������Monitor */
    axi_sc_mon_start(config);

    /* ���������ж����ȴ�������־��λ */
    i=0;
    do
    {
        axi_state = axi_get_state(config, AXI_GET_RUN_STATE_REQ);
        if(AXI_UNWIN_RUNNING == axi_state || AXI_WIN_RUNNING == axi_state)
        {
            return BSP_OK;
        }
    }while(i++ < AXI_WAIT_CNT);
    
    /* ������ʱ�쳣 */
    amon_error("start 0x%x time out\n", config);
    return BSP_ERROR;
}

/*****************************************************************************
 �� �� ��  : amon_stop
 ��������  : ֹͣAXI monitor����ϵͳ������ֹͣ
 �������  : axi_config_enum_uint32 config
 �������  : ��
 �� �� ֵ  : s32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��1��29��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
s32 amon_stop(axi_config_enum_uint32 config)
{
    axi_state_enum_uint32 axi_state;
    int i = 0;

    /* ���ζ�д�ж� */
    AXI_REG_WRITE(config, AXI_ID_INT_MASK, 0xFFFF);

    /* ����ϵͳ������ֹͣMonitor */
    axi_sc_mon_stop(config);

    /* ֹͣ�����ж� */
    do
    {
        axi_state = axi_get_state(config, AXI_GET_RUN_STATE_REQ);
        if(AXI_STOP == axi_state)
        {
            /* ֹ֮ͣ��ǿ�ƽ���IDLE̬ */
            AXI_REG_WRITE(config, AXI_MON_CNT_RESET, AXI_RESET_TO_IDLE);
            return BSP_OK;
        }
    }while(i++ < AXI_WAIT_CNT);
    
    /* ֹͣMonitor��ʱ�쳣 */
    amon_error("stop 0x%x time out\n", config);
    return BSP_ERROR;
}

/*****************************************************************************
 �� �� ��  : amon_config
 ��������  : ����monitor���ID
 �������  : axi_config_enum_uint32 config
 �������  : ��
 �� �� ֵ  : s32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��1��29��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
void amon_config(axi_config_enum_uint32 config)
{
    int i;
    
    if(AXI_SOC_CONFIG == config)
    {
        for(i=0; i<AXI_MAX_CONFIG_ID; i++)
        {
            /* ���ö˿� */
            AXI_REG_SETBITS(config, AXI_MON_PORT_SEL, i*3, 3, g_amon_config.soc_config[i].port&0x7);
            /* ����masterid */
            AXI_REG_WRITE(config, AXI_MON_CNT_ID(i), g_amon_config.soc_config[i].master_id);
            /* ������ʼ��ַ */
            AXI_REG_WRITE(config, AXI_MON_ID_ADDR_DES(i), g_amon_config.soc_config[i].start_addr);
            /* ���ý�����ַ */
            AXI_REG_WRITE(config, AXI_MON_ID_ADDR_DES_M(i), g_amon_config.soc_config[i].end_addr);
        }   
    }

    if(AXI_CPUFAST_CONFIG == config)
    {
        for(i=0; i<AXI_MAX_CONFIG_ID; i++)
        {
            /* ���ö˿� */
            AXI_REG_SETBITS(config, AXI_MON_PORT_SEL, i*3, 3, g_amon_config.cpufast_config[i].port&0x7);
            /* ����masterid */
            AXI_REG_WRITE(config, AXI_MON_CNT_ID(i), g_amon_config.cpufast_config[i].master_id);
            /* ������ʼ��ַ */
            AXI_REG_WRITE(config, AXI_MON_ID_ADDR_DES(i), g_amon_config.cpufast_config[i].start_addr);
            /* ���ý�����ַ */
            AXI_REG_WRITE(config, AXI_MON_ID_ADDR_DES_M(i), g_amon_config.cpufast_config[i].end_addr);
        }   
    }
}

/*****************************************************************************
 �� �� ��  : bsp_amon_init
 ��������  : axi monitor��ʼ������ȡNV���ü��ID
 �������  : ��
 �������  : ��
 �� �� ֵ  : s32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��1��29��
    ��    ��   : f
    �޸�����   : �����ɺ���

*****************************************************************************/
s32 bsp_amon_init(void)
{
#ifndef BSP_CONFIG_HI3630  
    struct clk * soc_clk;
    struct clk * cpufast_clk;
#endif

    /* ��ȡNV���� */
    if(BSP_OK != bsp_nvm_read(NV_ID_DRV_AMON, (u8*)&g_amon_config, sizeof(amon_config_stru)))
    {
        amon_error("read nv 0x%x fail\n", NV_ID_DRV_AMON);
        return BSP_ERROR;
    }

    /* δʹ�ܣ�����ʼ�� */
    if(g_amon_config.en_flag == 0)
    {
        return BSP_OK;
    }

    /* ʹ��monitor��ע��dpm */
#ifdef BSP_CONFIG_HI3630
#ifdef CONFIG_CCORE_PM
    if(BSP_OK != bsp_device_pm_add(&amon_dpm_device))
    {
        amon_error("add amon_dpm_device fail\n");
        return BSP_ERROR;
    }
    memset(&g_st_reg_bak, 0x0, sizeof(axi_reg_bak_t));
#endif
#endif

    /* SOC��ʼ�� */
    if(g_amon_config.en_flag & AMON_SOC_MASK)
    {
#ifndef BSP_CONFIG_HI3630        
        soc_clk = clk_get(NULL, "amon_soc_clk");
        clk_enable(soc_clk);
#else
        hi_syscrg_amon_soc_clk_enable();
#endif
        /* SOCϵͳ��������λ */
        hi_syscrg_amon_soc_soft_reset();
        hi_syscrg_amon_soc_rls_reset();
        /* monitor�ڲ���λ */
        if(BSP_OK != amon_reset(AXI_SOC_CONFIG))
        {
            amon_error("soc reset fail\n");
            return BSP_ERROR;
        }
        /* �ж����� */
        AXI_REG_WRITE(AXI_SOC_CONFIG, AXI_ID_INT_MASK, 0xFFFF);
        AXI_REG_SETBITS(AXI_SOC_CONFIG, AXI_MON_CNT_CTRL, 2, 2, 0x3);
        AXI_REG_WRITE(AXI_SOC_CONFIG, AXI_CAPT_INT_MASK, 0xF);        
    }

#ifndef BSP_CONFIG_HI3630
     /* CPUFAST��ʼ�� */
    if(g_amon_config.en_flag & AMON_CPUFAST_MASK)
    {   
        cpufast_clk = clk_get(NULL, "amon_cpufast_clk");
        clk_enable(cpufast_clk);
        /* CPUFASTϵͳ��������λ */
        hi_syscrg_amon_cpufast_soft_reset();
        hi_syscrg_amon_cpufast_rls_reset();
        if(BSP_OK != amon_reset(AXI_CPUFAST_CONFIG))
        {
            amon_error("cpufast reset fail\n");
            return BSP_ERROR;
        }
        AXI_REG_WRITE(AXI_CPUFAST_CONFIG, AXI_ID_INT_MASK, 0xFFFF);
        AXI_REG_WRITE(AXI_CPUFAST_CONFIG, AXI_CAPT_INT_MASK, 0xF);
        AXI_REG_SETBITS(AXI_CPUFAST_CONFIG, AXI_MON_CNT_CTRL, 2, 2, 0x3);
    }     
#endif

    if(BSP_OK != bsp_dump_get_buffer(DUMP_SAVE_MOD_AMON, &g_amon_buff.buff, &g_amon_buff.buff_size))
    {
        amon_error("get buffer fail\n");
        return BSP_ERROR;
    }
    g_amon_buff.write_offset = 0;
    memset(g_amon_buff.buff, 0, g_amon_buff.buff_size);

    /* ���ж� */
    if(BSP_OK != request_irq(AXI_INT_VECTOR, (irq_handler_t)amon_int_handler, 0, "AXI_MON_IRQ", NULL))
    {
        amon_error("INT connect fail\n");
        return BSP_ERROR;
    }
    
    /* ����SOC��� */
    if(g_amon_config.en_flag & AMON_SOC_MASK)
    {
        amon_config(AXI_SOC_CONFIG);
        if(BSP_OK != amon_start(AXI_SOC_CONFIG))
        {
            amon_error("soc start fail\n");
            return BSP_ERROR;
        }
    }
    
#ifndef BSP_CONFIG_HI3630
    /* ����CPUFAST��� */
    if(g_amon_config.en_flag & AMON_CPUFAST_MASK)
    {
        amon_config(AXI_CPUFAST_CONFIG);
        if(BSP_OK != amon_start(AXI_CPUFAST_CONFIG))
        {
            amon_error("cpufast start fail\n");
            return BSP_ERROR;
        }
    }
#endif

    amon_debug("%s init ok\n", __FUNCTION__);
    
    return BSP_OK;
}

void amon_debug_reset(void)
{
    int i;

    for(i=0; i<AXI_MAX_CONFIG_ID; i++)
    {
        g_amon_stat.soc_rd_cnt[i]     = 0;
        g_amon_stat.soc_wr_cnt[i]     = 0;
#ifndef BSP_CONFIG_HI3630
        g_amon_stat.cpufast_rd_cnt[i] = 0;
        g_amon_stat.cpufast_wr_cnt[i] = 0;
#endif
    }   
}

void amon_debug_show(void)
{
    int i;
    u32 reg_value_low;
    u32 reg_value_high;

    /* SOCͳ����Ϣ */
    amon_debug("************SOC STAT************\n");
    for(i=0; i<AXI_MAX_CONFIG_ID; i++)
    {
        AXI_REG_READ(AXI_SOC_CONFIG, AXI_MON_RD_BYTES_ID_LOW(i), &reg_value_low);
        AXI_REG_READ(AXI_SOC_CONFIG, AXI_MON_RD_BYTES_ID_HIGH(i), &reg_value_high);
        amon_debug("=======ID %d statistics=======\n", i);
        amon_debug("rd int cnt       : 0x%x\n", g_amon_stat.soc_rd_cnt[i]);
        amon_debug("wr int cnt       : 0x%x\n", g_amon_stat.soc_wr_cnt[i]);
        amon_debug("rd total bytes(h): 0x%x\n", reg_value_high);
        amon_debug("rd total bytes(l): 0x%x\n", reg_value_low);
        AXI_REG_READ(AXI_SOC_CONFIG, AXI_MON_WR_BYTES_ID_LOW(i), &reg_value_low);
        AXI_REG_READ(AXI_SOC_CONFIG, AXI_MON_WR_BYTES_ID_HIGH(i), &reg_value_high);
        amon_debug("wr total bytes(h): 0x%x\n", reg_value_high);
        amon_debug("wr total bytes(l): 0x%x\n", reg_value_low);        
    }

    amon_debug("\n");

#ifndef BSP_CONFIG_HI3630 
    /* CPUFASTͳ����Ϣ */
    amon_debug("************CPUFAST STAT************\n");
    for(i=0; i<AXI_MAX_CONFIG_ID; i++)
    {
        AXI_REG_READ(AXI_CPUFAST_CONFIG, AXI_MON_RD_BYTES_ID_LOW(i), &reg_value_low);
        AXI_REG_READ(AXI_CPUFAST_CONFIG, AXI_MON_RD_BYTES_ID_HIGH(i), &reg_value_high);        
        amon_debug("=======ID %d statistics=======\n", i);        
        amon_debug("rd int cnt: 0x%x\n", g_amon_stat.cpufast_rd_cnt[i]);
        amon_debug("wr int cnt: 0x%x\n", g_amon_stat.cpufast_wr_cnt[i]);
        amon_debug("rd total bytes(h): 0x%x\n", reg_value_high);
        amon_debug("rd total bytes(l): 0x%x\n", reg_value_low);
        AXI_REG_READ(AXI_CPUFAST_CONFIG, AXI_MON_WR_BYTES_ID_LOW(i), &reg_value_low);
        AXI_REG_READ(AXI_CPUFAST_CONFIG, AXI_MON_WR_BYTES_ID_HIGH(i), &reg_value_high);
        amon_debug("wr total bytes(h): 0x%x\n", reg_value_high);
        amon_debug("wr total bytes(l): 0x%x\n", reg_value_low);          
    }
#endif
}

#ifdef BSP_CONFIG_HI3630  
#ifdef CONFIG_CCORE_PM
s32 amon_suspend(struct dpm_device *dev)
{
    u32 j;
    u32 reg_value;
    /*lint -save -e958*/
    unsigned long long data_cnt;
    /*lint -restore*/
    u32 data_low;
    u32 data_high;
    axi_reg_bak_t * reg_bak;

    /* ����ǰ����ֹͣmonitor */
    axi_sc_mon_stop(AXI_SOC_CONFIG);

    reg_bak = &g_st_reg_bak;
    /* ���ƼĴ������� */
    axi_reg_read(AXI_SOC_CONFIG, AXI_MON_CNT_CTRL, &(reg_bak->ctrl_reg));
    axi_reg_read(AXI_SOC_CONFIG, AXI_MON_PORT_SEL, &(reg_bak->port_reg));
    axi_reg_read(AXI_SOC_CONFIG, AXI_ID_INT_MASK, &(reg_bak->int_mask));
    for(j=0; j<AXI_MAX_CONFIG_ID; j++)
    {
        axi_reg_read(AXI_SOC_CONFIG, AXI_MON_CNT_ID(j), &(reg_bak->id_reg[j]));
        axi_reg_read(AXI_SOC_CONFIG, AXI_MON_ID_ADDR_DES(j), &(reg_bak->addr_start_reg[j]));
        axi_reg_read(AXI_SOC_CONFIG, AXI_MON_ID_ADDR_DES_M(j), &(reg_bak->addr_end_reg[j]));
    }

    /* ͳ�ƼĴ����ۼ� */
    for(j=0; j<AXI_MAX_CONFIG_ID; j++)
    {
        axi_reg_read(AXI_SOC_CONFIG, AXI_MON_INCR1_ID(j), &reg_value);
        reg_bak->incr1_reg[j] += reg_value;
        axi_reg_read(AXI_SOC_CONFIG, AXI_MON_INCR2_ID(j), &reg_value);
        reg_bak->incr2_reg[j] += reg_value; 
        axi_reg_read(AXI_SOC_CONFIG, AXI_MON_INCR4_ID(j), &reg_value);
        reg_bak->incr4_reg[j] += reg_value; 
        axi_reg_read(AXI_SOC_CONFIG, AXI_MON_INCR8_ID(j), &reg_value);
        reg_bak->incr8_reg[j] += reg_value;  
        axi_reg_read(AXI_SOC_CONFIG, AXI_MON_INCR16_ID(j), &reg_value);
        reg_bak->incr16_reg[j] += reg_value; 
        axi_reg_read(AXI_SOC_CONFIG, AXI_MON_WRAP_ID(j), &reg_value);
        reg_bak->wrap_reg[j] += reg_value; 
        axi_reg_read(AXI_SOC_CONFIG, AXI_MON_BURST_ID(j), &reg_value);
        reg_bak->burst_send_reg[j] += reg_value; 
        axi_reg_read(AXI_SOC_CONFIG, AXI_MON_FINISH_ID(j), &reg_value);
        reg_bak->burst_fin_reg[j] += reg_value; 
        /* ��д��������40bit���ȣ���Ҫ�������ֵ */
        axi_reg_read(AXI_SOC_CONFIG, AXI_MON_RD_BYTES_ID_LOW(j), &data_low);
        axi_reg_read(AXI_SOC_CONFIG, AXI_MON_RD_BYTES_ID_HIGH(j), &data_high); 
        data_cnt = (unsigned long long)data_low + (unsigned long long)reg_bak->rd_cnt_low_reg[j];
        reg_bak->rd_cnt_high_reg[j] += data_high; 
        if(data_cnt >> 32)
        {
            reg_bak->rd_cnt_high_reg[j] += 1;
        }
        reg_bak->rd_cnt_low_reg[j] = (u32)data_cnt;
        axi_reg_read(AXI_SOC_CONFIG, AXI_MON_WR_BYTES_ID_LOW(j), &data_low);
        axi_reg_read(AXI_SOC_CONFIG, AXI_MON_WR_BYTES_ID_HIGH(j), &data_high); 
        data_cnt = (unsigned long long)data_low + (unsigned long long)reg_bak->wr_cnt_low_reg[j];
        reg_bak->wr_cnt_high_reg[j] += data_high; 
        if(data_cnt >> 32)
        {
            reg_bak->wr_cnt_high_reg[j] += 1;
        }
        reg_bak->wr_cnt_low_reg[j] = (u32)data_cnt;            
    }

    return BSP_OK;
}

s32 amon_resume(struct dpm_device *dev)
{
    u32 j;
    axi_reg_bak_t * reg_bak;

    reg_bak = &g_st_reg_bak;

    /* ���δ���ӣ��ȴ�ʱ�� */
    if(!hi_syscrg_amon_soc_get_clk_status())
    {
        hi_syscrg_amon_soc_clk_enable();
    }

    axi_reset(AXI_SOC_CONFIG);

    /* �ָ����üĴ��� */
    axi_reg_write(AXI_SOC_CONFIG, AXI_MON_CNT_CTRL, reg_bak->ctrl_reg);
    axi_reg_write(AXI_SOC_CONFIG, AXI_MON_PORT_SEL, reg_bak->port_reg);
    axi_reg_write(AXI_SOC_CONFIG, AXI_ID_INT_MASK, reg_bak->int_mask);
    for(j=0; j<AXI_MAX_CONFIG_ID; j++)
    {
        axi_reg_write(AXI_SOC_CONFIG, AXI_MON_CNT_ID(j), reg_bak->id_reg[j]);
        axi_reg_write(AXI_SOC_CONFIG, AXI_MON_ID_ADDR_DES(j), reg_bak->addr_start_reg[j]);
        axi_reg_write(AXI_SOC_CONFIG, AXI_MON_ID_ADDR_DES_M(j), reg_bak->addr_end_reg[j]);
    } 

    /* ����monitor */
    axi_sc_mon_start(AXI_SOC_CONFIG);

    return BSP_OK;
}
#endif
#endif

#ifdef __cplusplus
}
#endif

