
/*lint -save -e537*/
#include <osl_spinlock.h>
#include <osl_bio.h>
#include <drv_dpm.h>
#include <bsp_pmu_hi6561.h>
#include <bsp_abb.h>
#include <bsp_bbp.h>
#include <bsp_hardtimer.h>
#include <bsp_wakelock.h>
#include <bsp_lowpower_mntn.h>
#include <bsp_pmu.h>
#include <bsp_pmu_hi6561.h>
#include <bsp_regulator.h>
#include <bsp_hardtimer.h>
#include <bsp_pa_rf.h>
#include <bsp_dsp.h>
#include <tcxo_balong.h>

/*lint -restore +e537*/
#define MMU_RD_VALID_BIT  6

#define PWRCTRL_NV_BIT    7


/*ר��ģ���ӡ,JUST FOR adp_dpm.c*/
#define  adp_dpm_printf(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_ADP_DPM, "[adp_dpm]: <%s> "fmt"", __FUNCTION__, ##__VA_ARGS__))

struct pll_info{
    const char     *name;            /*PLL������*/
    unsigned int    reg;             /*ʹ�ܼĴ���*/
    unsigned int    bit;             /*ʹ��bit*/
    unsigned int    stable_reg;      /*���ȶ��Ĵ���*/
    unsigned int    stable_bit;      /*���ȶ�bit*/
};

/*����ABB BBP DSP PA RFIC�������ͶƱ����ʱ�Ӻ͵�Ŀ���*/
struct lock_map{
     const char   *name;                  /*��¼ͶƱ��Ŀ*/
     int               status;            /*��¼��ǰ״̬*/
     unsigned int   lock;                 /*��¼ͶƱ���*/
     unsigned int   enable_count[6];      /*��¼��ģ�Ŀ�����*/
     unsigned int   disable_count[6];     /*��¼��ģ�Ĺش���*/
     unsigned int   enable_refct;         /*��¼��ʵ���Ĵ���*/
     unsigned int   disable_refct;        /*��¼��ʵ�صĴ���*/
     spinlock_t  spinlock;
};

/*pastar dpm info*/
struct pastar_dpm_info{
	u32 *suspend_mask;
	u32 *en_timestamp;
	u32 *ps_switch;
};
/*��¼PLL����Ϣ�����Ĵ����͵��ȶ��Ĵ���*/
#ifdef CONFIG_V7R2_CLK_CRG /*CONFIG_V7R2_CLK_CRG*/
static struct pll_info hi6930_pll[] = {
    {"bbe16_pll", 0x214, 0, 0x214, 31},
    {"bbp_pll", 0x22c, 0, 0x22c, 31},
    {"abb_ch0_lwt_pll", 0xD0, 3, 0xDB, 7},
    {"abb_ch0_g_pll", 0xD0, 2, 0xDB, 6},
    {"abb_ch1_lwt_pll", 0x70, 3, 0x7B, 7},
    {"abb_ch1_g_pll", 0x70, 2, 0x7B, 6},
    {NULL, 0, 0, 0, 0}
};

#elif defined(CONFIG_K3V3_CLK_CRG) /* CONFIG_K3V3_CLK_CRG */
static struct pll_info hi6930_pll[] = {
	{"bbe16_pll", 0x20c, 0, 0x210, 0},
	{"bbp_pll", 0x224, 0, 0x228, 0},
	{"abb_ch0_lwt_pll", 0xD0, 3, 0xDB, 7},
	{"abb_ch0_g_pll", 0xD0, 2, 0xDB, 6},
	{"abb_ch1_lwt_pll", 0x70, 3, 0x7B, 7},
	{"abb_ch1_g_pll", 0x70, 2, 0x7B, 6},
	{NULL, 0, 0, 0, 0}
};
#else
static struct pll_info hi6930_pll[] = {};
#endif

/*��¼�������µ�Ϳ����ӵ�ģʽͶƱ��Ϣ*/
static struct lock_map vote_map[] = {
    {"PA0_pd", 0, 0,{0},{0},0,0,{0}},
    {"PA1_pd", 0, 0,{0},{0},0,0,{0}},
    {"RFIC0_pd", 0, 0,{0},{0},0,0,{0}},
    {"RFIC1_pd", 0, 0,{0},{0},0,0,{0}},
    {"BBP_IRM_pd", 0, 0,{0},{0},0,0,{0}},
    {"BBE16_pd", 0, 0,{0},{0},0,0,{0}},
    {"ABB_pd", 0, 0,{0},{0},0,0,{0}},
    {"G1BBP_pd", 0, 0,{0},{0},0,0,{0}},
    {"G2BBP_pd", 0, 0,{0},{0},0,0,{0}},
    {"WBBP_pd", 0, 0,{0},{0},0,0,{0}},
    {"BBP_COMMON_pd", 0, 0,{0},{0},0,0,{0}},
    {"TWBBP_pd", 0, 0,{0},{0},0,0,{0}},
    {"bbe16_pll", 0, 0,{0},{0},0,0,{0}},
    {"bbp_pll", 0, 0,{0},{0},0,0,{0}},
    {"abb_ch0_lwt_pll", 0, 0,{0},{0},0,0,{0}},
    {"abb_ch0_g_pll", 0, 0,{0},{0},0,0,{0}},
    {"abb_ch1_lwt_pll", 0, 0,{0},{0},0,0,{0}},
    {"abb_ch1_g_pll", 0, 0,{0},{0},0,0,{0}},
    {"pastar_pd", 0, 0,{0},{0},0,0,{0}},
    {"wcdma_clk", 0, 0,{0},{0},0,0,{0}},
    {"tds_clk", 0, 0,{0},{0},0,0,{0}},
    {NULL, 0, 0,{0},{0},0,0,{0}}
};

/*���µ�Ϳ����ӽӿ��е�����жϺ���*/
static int check_input_param(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enCommModem)
{
	/* coverity[unsigned_compare] */
        if((enCommMode < PWC_COMM_MODE_GSM) ||(enCommMode > PWC_COMM_MODE_BUTT))
        {
            adp_dpm_printf("The first input enCommMode param false. \n");
            return PWC_PARA_INVALID;
        }
	/* coverity[unsigned_compare] */
        if((enCommModule < PWC_COMM_MODULE_PA)||( enCommModule > PWC_COMM_MODULE_BUTT))
        {
            adp_dpm_printf("The secend input enCommModule param false. \n");
            return PWC_PARA_INVALID;
        }
	/* coverity[unsigned_compare] */
        if((enCommModem < PWC_COMM_MODEM_0)||( enCommModem > PWC_COMM_MODEM_BUTT))
        {
            adp_dpm_printf("The third input enCommModem param false. \n");
            return PWC_PARA_INVALID;
        }
        return PWC_SUCCESS;
}

/*�˱������ڼ�¼pastar ��dpm �ص������еĴ�����Ϣ��
*bit 0:��ʾ�Ƿ��߹�suspend
*bit 1:��ʾȥʹ��smart star LVS5ʧ��
*bit 2:��ʾʹ��smart star LVS5ʧ��
*/
//static unsigned int *debug_mask_pastar = (unsigned int *)SRAM_PASTAR_DPM_INFO;;

/*�˱������ڼ�¼pastar�ָ�ʱ��*/
//static unsigned int *time_stamp = (u32 *)(SRAM_PASTAR_DPM_INFO+4);

/*�˱������ڱ��pastar״ָ̬ʾ,1Ϊ�����Թرգ�0Ϊ����*/
//static unsigned int *pastar_switch = (u32 *)(SRAM_PASTAR_DPM_INFO+8);

/*�˱�����¼pastar�ĵ�Դ�ṹ��*/
static struct regulator *pastar_regulator = NULL;

/*�˱�����¼BBP PLL�����ĸ�BUFFER*/
static NV_TLMODE_BASIC_PARA_STRU tl_mode_nv[2];

static struct pastar_dpm_info dpm_info={
	.suspend_mask = (u32 *)(SRAM_PASTAR_DPM_INFO+PASTAR_DPM_SUSPEND_MASK_OFFSET),
	.en_timestamp = (u32 *)(SRAM_PASTAR_DPM_INFO+PASTAR_DPM_EN_TIMESTAMP_OFFSET),
	.ps_switch = (u32 *)(SRAM_PASTAR_DPM_INFO+PASTAR_DPM_SWITCH_OFFSET)
};
/*�ȴ�2ms*/
#define PASTAR_WAIT_TIME     (66)

#if defined(CONFIG_PASTAR)
/*�˺������ڵ���pastar ����smart star LVS5ʹ���źŵ�����2ms--[18]*/
static void check_pastar_stable_timestamp(void)
{
	unsigned int newslice = 0;

	/*�ж�pastar�Ƿ�رչ�smart star LVS5*/
	if(*dpm_info.suspend_mask & 0x1){
		newslice = bsp_get_slice_value();
		if(get_timer_slice_delta(*(dpm_info.en_timestamp), newslice) < PASTAR_WAIT_TIME){
			do{
				newslice = bsp_get_slice_value();

				/*��¼���ȶ�ʱ��δ��2ms����Ҫ�ٶ�ȡʱ����Ĵ���*/
				vote_map[18].enable_count[0] ++;
			}while(get_timer_slice_delta(*(dpm_info.en_timestamp), newslice) < PASTAR_WAIT_TIME);
		}
	}
	return;
}

/*�˺����������suspend�еı�Ǻ�����pastar*/
static void clear_pastar_suspend_mark_and_config(void)
{
    u32 temp = 0;
    
	/*��suspend ���*/
	if(*(dpm_info.suspend_mask) & 0x1){
        temp = *(dpm_info.suspend_mask);
        temp &= ~(0x1);
        writel(temp, (unsigned)(dpm_info.suspend_mask));
		pmu_hi6561_reg_resume();
	}
	return;
}

/*�˺������ڹر�pastar ����smart star LVS5 --[18]*/
static s32 pastar_suspend(struct dpm_device *dev)
{
	int ret = 0;
    u32 temp = 0;
    
	if(!(vote_map[0].lock || vote_map[1].lock || vote_map[2].lock || vote_map[3].lock)){
			check_pastar_stable_timestamp();
			clear_pastar_suspend_mark_and_config();
			
			ret = (int)pmu_hi6561_reg_save();
			if(ret)
				return -1;
#ifndef CONFIG_PASTAR_DPM_M3
			ret = regulator_disable(pastar_regulator);
			if(ret){
                temp = *(dpm_info.suspend_mask);
                temp |= (0x1 << 1);
                writel(temp, (unsigned)(dpm_info.suspend_mask));
				return -1;
			}
#else
            writel(0, (unsigned)(dpm_info.ps_switch));
#endif
			vote_map[18].disable_refct ++;

			/*���ʱ����Ĵ���*/
			vote_map[18].enable_count[0] = 0;
            temp = *(dpm_info.suspend_mask);
            temp |= 0x1;
            writel(temp, (unsigned)(dpm_info.suspend_mask));
	}
	return 0;
}

/*�˺�������ʹ��pastar ����smart star LVS5 --[18]*/
static s32 pastar_resume_early(struct dpm_device *dev)
{
    u32 temp = 0;
    
	/*�ж�pastar�Ƿ�رչ�smart star LVS5*/
	if(*(dpm_info.suspend_mask) & 0x1){
		if(0 != regulator_enable(pastar_regulator))
		{
		    /* д�����ڴ���õ���writel */
		    temp = *(dpm_info.suspend_mask);
            temp |= (0x1 << 2);
            writel(temp, (unsigned)(dpm_info.suspend_mask));

			return -1;
		}
		vote_map[18].enable_refct ++;

		/*��¼smart star LVS5ʹ�ܵ�ʱ�̵�*/
        writel(bsp_get_slice_value(), (unsigned)(dpm_info.en_timestamp));
	}
	return 0;
}

/*for�ػ�ǰ����modem �쳣����*/
void bsp_pastar_leakage_bugfix(void)
{
	check_pastar_stable_timestamp();
	clear_pastar_suspend_mark_and_config();
}
static struct dpm_device pastar_dpm =
{
	.device_name = "pastar dpm",
	.suspend_late = pastar_suspend,
#ifndef CONFIG_PASTAR_DPM_M3
	.resume_early  = pastar_resume_early,
#endif
};
#endif

void  bsp_adp_dpm_debug_init(void)
{
#if defined(CONFIG_PASTAR)
	s32 ret = 0;
#endif
	int i = 0;

	/*��ʼ��spinlock*/
	while( NULL != vote_map[i].name){
		spin_lock_init(&vote_map[i].spinlock);
		i++;
	}

#if defined(CONFIG_PASTAR)
	/*��ȡpastar_regulator*/
	pastar_regulator = regulator_get(NULL, "PA_STAR_VDD-vcc");
	if(NULL == pastar_regulator)
	{
	    adp_dpm_printf("pastar regulator_get is fail!\n");
	}else{
    	ret = bsp_device_pm_add(&pastar_dpm);
    	if(ret)
    	{
    		adp_dpm_printf("pastar dpm callback function register fail!\n");
    	}
    }
#endif

    if(NV_OK != bsp_nvm_read(NV_ID_DRV_TLMODE_CHAN_PARA_STRU, (u8 *)tl_mode_nv, 2 * sizeof(NV_TLMODE_BASIC_PARA_STRU)))
    {
        adp_dpm_printf("NV_ID_TLMODE_CHAN_PARA_STRU:0xD01B read failure!\n");
    }

    /* д�����ڴ治��ֱ�Ӹ�ֵ����õ���writel�����ֱ�Ӹ�ֵ����Ҫcache_sync */
    writel(0, (unsigned)(dpm_info.suspend_mask));
    writel(0, (unsigned)(dpm_info.en_timestamp));
    writel(0, (unsigned)(dpm_info.ps_switch));    

	return;
}

void debug_pastar_dpm(void)
{
	adp_dpm_printf("pastar suspend count: %d\n",vote_map[18].disable_refct);
	adp_dpm_printf("pastar resume count : %d\n",vote_map[18].enable_refct);
	adp_dpm_printf("read timestamp count: %d\n",vote_map[18].enable_count[0]);
	adp_dpm_printf("suspend_mask:     0x%x\n",*(dpm_info.suspend_mask));
	adp_dpm_printf("en_timestamp:     0x%x\n",*(dpm_info.en_timestamp));
	if(*(dpm_info.suspend_mask) & (0x1 << 1)){
		adp_dpm_printf("regulator_disable pastar failure!\n");
	}
	if(*(dpm_info.suspend_mask) & (0x1 << 2)){
		adp_dpm_printf("regulator_enable pastar failure!\n");
	}
	return;
}


/*�ײ����ʵ��PA���ϵ�PA0--[0]PA1--[1]*/
static int bsp_pa_power_up(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     
#if defined(CONFIG_PASTAR)
     check_pastar_stable_timestamp();
#endif
     if(enModemId == PWC_COMM_MODEM_0){
        spin_lock_irqsave(&vote_map[0].spinlock, flags);

        writel(1, (unsigned)(dpm_info.ps_switch));

#if defined(CONFIG_PASTAR)
        clear_pastar_suspend_mark_and_config();
#endif
        vote_map[0].enable_count[enCommMode] ++;
        vote_map[0].enable_refct ++;
        ret = bsp_pa_poweron(enModemId);
        if(!(vote_map[0].lock & ((unsigned int)1 << enCommMode))){
            vote_map[0].lock |= ((unsigned int)1 << enCommMode);
        }
        spin_unlock_irqrestore(&vote_map[0].spinlock, flags);
     }else if(enModemId == PWC_COMM_MODEM_1){
        spin_lock_irqsave(&vote_map[1].spinlock, flags);

        writel(1, (unsigned)(dpm_info.ps_switch));

#if defined(CONFIG_PASTAR)
        clear_pastar_suspend_mark_and_config();
#endif
        vote_map[1].enable_count[enCommMode] ++;
        vote_map[1].enable_refct ++;
        ret = bsp_pa_poweron(enModemId);
        if(!(vote_map[1].lock & ((unsigned int)1 << enCommMode))){

            vote_map[1].lock |= ((unsigned int)1 << enCommMode);
        }
        spin_unlock_irqrestore(&vote_map[1].spinlock, flags);
     }
     return ret;
}
static int bsp_pa_power_down(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     if(enModemId == PWC_COMM_MODEM_0){
        spin_lock_irqsave(&vote_map[0].spinlock, flags);

        vote_map[0].disable_count[enCommMode] ++;
        if(vote_map[0].lock & ((unsigned int)1 << enCommMode)){
            vote_map[0].lock &= ~((unsigned int)1 << enCommMode);
        }
        if(!vote_map[0].lock){

            /*����NV�����������Ƿ�ر�*/
            if (g_nv_pwc_switch.drx_pa0_pd == 0){
            vote_map[0].disable_refct ++;
                    ret = bsp_pa_poweroff(enModemId);
            }
        }
        spin_unlock_irqrestore(&vote_map[0].spinlock, flags);
     }else if(enModemId == PWC_COMM_MODEM_1){
        spin_lock_irqsave(&vote_map[1].spinlock, flags);

        vote_map[1].disable_count[enCommMode] ++;
         if(vote_map[1].lock & ((unsigned int)1 << enCommMode)){
                vote_map[1].lock &= ~((unsigned int)1 << enCommMode);
         }
          if(!vote_map[1].lock){

            /*����NV�����������Ƿ�ر�*/
            if (g_nv_pwc_switch.drx_pa1_pd == 0){
                        vote_map[1].disable_refct ++;
                        ret = bsp_pa_poweroff(enModemId);
            }
          }
        spin_unlock_irqrestore(&vote_map[1].spinlock, flags);
     }
     return ret;
}
static int bsp_pa_power_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = PWC_COMM_MODEM_OFF;

     if(enModemId == PWC_COMM_MODEM_0){
	     spin_lock_irqsave(&vote_map[0].spinlock, flags);

             ret = bsp_get_pa_powerstatus(enModemId);

	     spin_unlock_irqrestore(&vote_map[0].spinlock, flags);
     }else if(enModemId == PWC_COMM_MODEM_1){
	     spin_lock_irqsave(&vote_map[1].spinlock, flags);

             ret = bsp_get_pa_powerstatus(enModemId);

	    spin_unlock_irqrestore(&vote_map[1].spinlock, flags);
     }
     return ret;
}


/*�ײ����ʵ��RFIC���ϵ�RFIC0--[2] RFIC1--[3]*/
static int bsp_rfic_power_up(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
#if defined(CONFIG_PASTAR)
     check_pastar_stable_timestamp();
#endif
     if(enModemId == PWC_COMM_MODEM_0){
		spin_lock_irqsave(&vote_map[2].spinlock, flags);
        
        writel(1, (unsigned)(dpm_info.ps_switch));
        
#if defined(CONFIG_PASTAR)
		clear_pastar_suspend_mark_and_config();
#endif
        vote_map[2].enable_count[enCommMode] ++;
	    vote_map[2].enable_refct ++;
	    ret = bsp_rf_poweron(enModemId);
	    if(!(vote_map[2].lock & ((unsigned int)1 << enCommMode))){
		        vote_map[2].lock |= ((unsigned int)1 << enCommMode);
		 }
		spin_unlock_irqrestore(&vote_map[2].spinlock, flags);
     }else if(enModemId == PWC_COMM_MODEM_1){

		spin_lock_irqsave(&vote_map[3].spinlock, flags);
        
        writel(1, (unsigned)(dpm_info.ps_switch));
        
#if defined(CONFIG_PASTAR)
		clear_pastar_suspend_mark_and_config();
#endif
        vote_map[3].enable_count[enCommMode] ++;
	    vote_map[3].enable_refct ++;
		bsp_pmu_tcxo1_en_enable(RFIC1_ID_0);
	    ret = bsp_rf_poweron(enModemId);
		 if(!(vote_map[3].lock & ((unsigned int)1 << enCommMode))){
		        vote_map[3].lock |= ((unsigned int)1 << enCommMode);
		 }
		spin_unlock_irqrestore(&vote_map[3].spinlock, flags);
     }
     return ret;
}
static int bsp_rfic_power_down(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;

     if(enModemId == PWC_COMM_MODEM_0){
	    spin_lock_irqsave(&vote_map[2].spinlock, flags);
            vote_map[2].disable_count[enCommMode] ++;
             if(vote_map[2].lock & ((unsigned int)1 << enCommMode)){
                    vote_map[2].lock &= ~((unsigned int)1 << enCommMode);
             }
             if(!vote_map[2].lock){

			/*����NV�����������Ƿ�ر�*/
			if (g_nv_pwc_switch.drx_rfic0_pd== 0){
				vote_map[2].disable_refct ++;
			        ret = bsp_rf_poweroff(enModemId);
			}
		}

	     spin_unlock_irqrestore(&vote_map[2].spinlock, flags);
     }else if(enModemId == PWC_COMM_MODEM_1){

	     spin_lock_irqsave(&vote_map[3].spinlock, flags);
            vote_map[3].disable_count[enCommMode] ++;
             if(vote_map[3].lock & ((unsigned int)1 << enCommMode)){
                    vote_map[3].lock &= ~((unsigned int)1 << enCommMode);
             }
              if(!vote_map[3].lock){

			/*����NV�����������Ƿ�ر�*/
			if (g_nv_pwc_switch.drx_rfic1_pd== 0){
		                vote_map[3].disable_refct ++;
		                ret = bsp_rf_poweroff(enModemId);
			        bsp_pmu_tcxo1_en_disable(RFIC1_ID_0);
			}
              }
	      spin_unlock_irqrestore(&vote_map[3].spinlock, flags);
     }
     return ret;
}
static int bsp_rfic_power_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = PWC_COMM_MODEM_OFF;

     if(enModemId == PWC_COMM_MODEM_0){
             spin_lock_irqsave(&vote_map[2].spinlock, flags);

             ret = bsp_get_rf_powerstatus(enModemId);

	    spin_unlock_irqrestore(&vote_map[2].spinlock, flags);
     }else if(enModemId == PWC_COMM_MODEM_1){
	     spin_lock_irqsave(&vote_map[3].spinlock, flags);

             ret = bsp_get_rf_powerstatus(enModemId);

	     spin_unlock_irqrestore(&vote_map[3].spinlock, flags);
     }
     return ret;
}


/*�ײ����ʵ��BBP IRM���ϵ�--[4]*/
static int bsp_bbp_irm_power_up(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     spin_lock_irqsave(&vote_map[4].spinlock, flags);
     vote_map[4].enable_count[enCommMode] ++;
     vote_map[4].enable_refct ++;
     ret = bbp_pwrctrl_irm_poweron();
     if(!(vote_map[4].lock & ((unsigned int)1 << enCommMode))){
            vote_map[4].lock |= ((unsigned int)1 << enCommMode);
     }
     spin_unlock_irqrestore(&vote_map[4].spinlock, flags);
     return ret;
}

static int bsp_bbp_irm_power_down(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     spin_lock_irqsave(&vote_map[4].spinlock, flags);
    vote_map[4].disable_count[enCommMode] ++;
     if(vote_map[4].lock & ((unsigned int)1 << enCommMode)){
            vote_map[4].lock &= ~((unsigned int)1 << enCommMode);
     }
     if(!vote_map[4].lock){

		 /*����NV�����������Ƿ�ر�*/
		 if (g_nv_pwc_switch.drx_irm_pd == 0){
		         vote_map[4].disable_refct ++;
		         ret = bbp_pwrctrl_irm_poweroff();
		}
     }
     spin_unlock_irqrestore(&vote_map[4].spinlock, flags);
     return ret;
}
static int bsp_bbp_irm_power_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = PWC_COMM_MODEM_OFF;
     spin_lock_irqsave(&vote_map[4].spinlock, flags);

     ret = bbp_pwrctrl_irm_status();

     spin_unlock_irqrestore(&vote_map[4].spinlock, flags);
     return ret;
}


/*�ײ����ʵ��BBE16���ϵ�--[5]*/
static int bsp_bbe16_power_up(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     spin_lock_irqsave(&vote_map[5].spinlock, flags);
     vote_map[5].enable_count[enCommMode] ++;
     vote_map[5].enable_refct ++;
     ret = RET_OK;
     if(!(vote_map[5].lock & ((unsigned int)1 << enCommMode))){
            vote_map[5].lock |= ((unsigned int)1 << enCommMode);
     }
     spin_unlock_irqrestore(&vote_map[5].spinlock, flags);
     return ret;
}
static int bsp_bbe16_power_down(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     spin_lock_irqsave(&vote_map[5].spinlock, flags);
    vote_map[5].disable_count[enCommMode] ++;
     if(vote_map[5].lock & ((unsigned int)1 << enCommMode)){
            vote_map[5].lock &= ~((unsigned int)1 << enCommMode);
     }
     if(!vote_map[5].lock){

	 /*����NV�����������Ƿ�ر�*/
	 if (g_nv_pwc_switch.drx_bbe16_pd == 0){
	         vote_map[5].disable_refct ++;
	         ret = RET_OK;
	 }
     }
     spin_unlock_irqrestore(&vote_map[5].spinlock, flags);
     return ret;
}
static int bsp_bbe16_power_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = PWC_COMM_MODEM_ON;
     spin_lock_irqsave(&vote_map[5].spinlock, flags);

     ret = PWC_COMM_MODEM_ON;

     spin_unlock_irqrestore(&vote_map[5].spinlock, flags);
     return ret;
}

/*�ײ����ʵ��ABB���ϵ�--[6]*/
static int bsp_abb_power_up(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     spin_lock_irqsave(&vote_map[6].spinlock, flags);
     vote_map[6].enable_count[enCommMode] ++;
     //ret = bsp_abb_resume();
     vote_map[6].enable_refct ++;
     if(!(vote_map[6].lock & ((unsigned int)1 << enCommMode))){
            vote_map[6].lock |= ((unsigned int)1 << enCommMode);
     }
     spin_unlock_irqrestore(&vote_map[6].spinlock, flags);
     return ret;
}
static int bsp_abb_power_down(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     spin_lock_irqsave(&vote_map[6].spinlock, flags);
    vote_map[6].disable_count[enCommMode] ++;
     if(vote_map[6].lock & ((unsigned int)1 << enCommMode)){
            vote_map[6].lock &= ~((unsigned int)1 << enCommMode);
     }
     if(!vote_map[6].lock){

	 /*����NV�����������Ƿ�ر�*/
	 if (g_nv_pwc_switch.drx_abb_pd == 0){
	         vote_map[6].disable_refct ++;
	         //ret = bsp_abb_suspend();
	 }
     }
     spin_unlock_irqrestore(&vote_map[6].spinlock, flags);
     return ret;
}
static int bsp_abb_power_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = PWC_COMM_MODEM_ON;
     spin_lock_irqsave(&vote_map[6].spinlock, flags);
     //ret = xx;
     spin_unlock_irqrestore(&vote_map[6].spinlock, flags);
     return ret;
}

/*�ײ����ʵ��G1_BBP���ϵ�--[7]*/
static int bsp_g1bbp_power_up(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     spin_lock_irqsave(&vote_map[7].spinlock, flags);
     vote_map[7].enable_count[enCommMode] ++;
     vote_map[7].enable_refct ++;
     ret = bsp_bbp_pwrctrl_g1bbp_poweron();
     if(!(vote_map[7].lock & ((unsigned int)1 << enCommMode))){
            vote_map[7].lock |= ((unsigned int)1 << enCommMode);
     }
     spin_unlock_irqrestore(&vote_map[7].spinlock, flags);
     return ret;
}
static int bsp_g1bbp_power_down(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     spin_lock_irqsave(&vote_map[7].spinlock, flags);
    vote_map[7].disable_count[enCommMode] ++;
     if(vote_map[7].lock & ((unsigned int)1 << enCommMode)){
            vote_map[7].lock &= ~((unsigned int)1 << enCommMode);
     }
     if(!vote_map[7].lock){

	 /*����NV�����������Ƿ�ر�*/
	 if (g_nv_pwc_switch.drx_g1bbp_pd == 0){
	         vote_map[7].disable_refct ++;
	         ret = bsp_bbp_pwrctrl_g1bbp_poweroff();;
	 }
     }
     spin_unlock_irqrestore(&vote_map[7].spinlock, flags);
     return ret;
}
static int bsp_g1bbp_power_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = PWC_COMM_MODEM_OFF;
     spin_lock_irqsave(&vote_map[7].spinlock, flags);

     ret = bsp_bbp_pwrctrl_g1bbp_status();

     spin_unlock_irqrestore(&vote_map[7].spinlock, flags);
     return ret;
}

/*�ײ����ʵ��G2_BBP���ϵ�--[8]*/
static int bsp_g2bbp_power_up(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     spin_lock_irqsave(&vote_map[8].spinlock, flags);
     vote_map[8].enable_count[enCommMode] ++;
     vote_map[8].enable_refct ++;
     ret = bsp_bbp_pwrctrl_g2bbp_poweron();
     if(!(vote_map[8].lock & ((unsigned int)1 << enCommMode))){
            vote_map[8].lock |= ((unsigned int)1 << enCommMode);
     }
     spin_unlock_irqrestore(&vote_map[8].spinlock, flags);
     return ret;
}
static int bsp_g2bbp_power_down(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     spin_lock_irqsave(&vote_map[8].spinlock, flags);
    vote_map[8].disable_count[enCommMode] ++;
     if(vote_map[8].lock & ((unsigned int)1 << enCommMode)){
            vote_map[8].lock &= ~((unsigned int)1 << enCommMode);
     }
     if(!vote_map[8].lock){

	 /*����NV�����������Ƿ�ر�*/
	 if (g_nv_pwc_switch.drx_g2bbp_pd == 0){
	         vote_map[8].disable_refct ++;
	         ret = bsp_bbp_pwrctrl_g2bbp_poweroff();
	 }
     }
     spin_unlock_irqrestore(&vote_map[8].spinlock, flags);
     return ret;
}
static int bsp_g2bbp_power_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = PWC_COMM_MODEM_OFF;
     spin_lock_irqsave(&vote_map[8].spinlock, flags);

     ret = bsp_bbp_pwrctrl_g2bbp_status();

     spin_unlock_irqrestore(&vote_map[8].spinlock, flags);
     return ret;
}


/*�ײ����ʵ��W_BBP���ϵ�--[9]*/
static int bsp_wbbp_power_up(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     spin_lock_irqsave(&vote_map[9].spinlock, flags);
     vote_map[9].enable_count[enCommMode] ++;
     vote_map[9].enable_refct ++;
     ret = bsp_bbp_pwrctrl_wbbp_poweron();
     if(!(vote_map[9].lock & ((unsigned int)1 << enCommMode))){
            vote_map[9].lock |= ((unsigned int)1 << enCommMode);
     }
     spin_unlock_irqrestore(&vote_map[9].spinlock, flags);
     return ret;
}
static int bsp_wbbp_power_down(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     spin_lock_irqsave(&vote_map[9].spinlock, flags);
     vote_map[9].disable_count[enCommMode] ++;
     if(vote_map[9].lock & ((unsigned int)1 << enCommMode)){
            vote_map[9].lock &= ~((unsigned int)1 << enCommMode);
     }
     if(!vote_map[9].lock){

	/*����NV�����������Ƿ�ر�*/
	if (g_nv_pwc_switch.drx_wbbp_pd == 0){
	         vote_map[9].disable_refct ++;
	         ret = bsp_bbp_pwrctrl_wbbp_poweroff();
	}
     }
     spin_unlock_irqrestore(&vote_map[9].spinlock, flags);
     return ret;
}
static int bsp_wbbp_power_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = PWC_COMM_MODEM_OFF;
     spin_lock_irqsave(&vote_map[9].spinlock, flags);

     ret = bsp_bbp_pwrctrl_wbbp_status();

     spin_unlock_irqrestore(&vote_map[9].spinlock, flags);
     return ret;
}

/*�ײ����ʵ��BBP_COMMON���ϵ�--[10]*/
static int bsp_bbpcommon_power_up(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     spin_lock_irqsave(&vote_map[10].spinlock, flags);
     vote_map[10].enable_count[enCommMode] ++;
     vote_map[10].enable_refct ++;
     //ret = xxx;
     if(!(vote_map[10].lock & ((unsigned int)1 << enCommMode))){
            vote_map[10].lock |= ((unsigned int)1 << enCommMode);
     }
     spin_unlock_irqrestore(&vote_map[10].spinlock, flags);
     return ret;
}
static int bsp_bbpcommon_power_down(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     spin_lock_irqsave(&vote_map[10].spinlock, flags);
     vote_map[10].disable_count[enCommMode] ++;
     if(vote_map[10].lock & ((unsigned int)1 << enCommMode)){
            vote_map[10].lock &= ~((unsigned int)1 << enCommMode);
     }
     if(!vote_map[10].lock){

	 /*����NV�����������Ƿ�ر�*/
	 if (g_nv_pwc_switch.drx_bbpcommon_pd == 0){
	         vote_map[10].disable_refct ++;
	         //ret = XXX;
	 }
     }
     spin_unlock_irqrestore(&vote_map[10].spinlock, flags);
     return ret;
}
static int bsp_bbpcommon_power_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = PWC_COMM_MODEM_ON;
     spin_lock_irqsave(&vote_map[10].spinlock, flags);
     //ret = xxx;
     spin_unlock_irqrestore(&vote_map[10].spinlock, flags);
     return ret;
}

/*�ײ����ʵ��TWBBP���ϵ�--[11]*/
static int bsp_twbbp_power_up(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     spin_lock_irqsave(&vote_map[11].spinlock, flags);
     vote_map[11].enable_count[enCommMode] ++;
     vote_map[11].enable_refct ++;
     ret = bsp_bbp_pwrctrl_twbbp_poweron();
     if(!(vote_map[11].lock & ((unsigned int)1 << enCommMode))){
            vote_map[11].lock |= ((unsigned int)1 << enCommMode);
     }
     spin_unlock_irqrestore(&vote_map[11].spinlock, flags);
     return ret;
}
static int bsp_twbbp_power_down(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     spin_lock_irqsave(&vote_map[11].spinlock, flags);
     vote_map[11].disable_count[enCommMode] ++;
     if(vote_map[11].lock & ((unsigned int)1 << enCommMode)){
            vote_map[11].lock &= ~((unsigned int)1 << enCommMode);
     }
     if(!vote_map[11].lock){

	 /*����NV�����������Ƿ�ر�*/
	 if (g_nv_pwc_switch.drx_twbbp_pd == 0){
	         vote_map[11].disable_refct ++;
	         ret = bsp_bbp_pwrctrl_twbbp_poweroff();
	 }
     }
     spin_unlock_irqrestore(&vote_map[11].spinlock, flags);
     return ret;
}
static int bsp_twbbp_power_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = PWC_COMM_MODEM_OFF;
     spin_lock_irqsave(&vote_map[11].spinlock, flags);

     ret = bsp_bbp_pwrctrl_twbbp_status();

     spin_unlock_irqrestore(&vote_map[11].spinlock, flags);
     return ret;
}

/*�ײ����ʵ��GBBP���ϵ�*/
static int bsp_gbbp_power_up(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     int ret = RET_OK;
     switch(enModemId)
     {
            case PWC_COMM_MODEM_0:
                    ret = bsp_g1bbp_power_up(enCommMode, enCommModule, enModemId);
                    break;
            case PWC_COMM_MODEM_1:
                    ret = bsp_g2bbp_power_up(enCommMode, enCommModule, enModemId);
                    break;
            default:
                    adp_dpm_printf("check your input enModemId!\n");
                    break;
     }
     return ret;
}
static int bsp_gbbp_power_down(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     int ret = RET_OK;
     switch(enModemId)
     {
            case PWC_COMM_MODEM_0:
                    ret = bsp_g1bbp_power_down(enCommMode, enCommModule, enModemId);
                    break;
            case PWC_COMM_MODEM_1:
                    ret = bsp_g2bbp_power_down(enCommMode, enCommModule, enModemId);
                    break;
            default:
                    adp_dpm_printf("check your input enModemId!\n");
                    break;
     }
     return ret;
}
static int bsp_gbbp_power_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     int ret = PWC_COMM_MODEM_OFF;
     switch(enModemId)
     {
            case PWC_COMM_MODEM_0:
                    ret = bsp_g1bbp_power_status(enCommMode, enCommModule, enModemId);
                    break;
            case PWC_COMM_MODEM_1:
                    ret = bsp_g2bbp_power_status(enCommMode, enCommModule, enModemId);
                    break;
            default:
                    adp_dpm_printf("check your input enModemId!\n");
                    break;
     }
     return ret;
}


/*�ײ����ʵ��input BBP_DRX���ϵ�*/
static int bsp_bbp_drx_power_up(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     int ret = RET_OK;
     switch(enCommMode)
     {
            case PWC_COMM_MODE_WCDMA:
                    ret = bsp_twbbp_power_up(enCommMode, enCommModule, enModemId);
                    break;
            case PWC_COMM_MODE_GSM:
                    ret = bsp_gbbp_power_up(enCommMode, enCommModule, enModemId);
                    break;
            case PWC_COMM_MODE_LTE:
            case PWC_COMM_MODE_TDS:
                    ret = bsp_twbbp_power_up(enCommMode, enCommModule, enModemId);
                    ret |= bsp_bbp_irm_power_up(enCommMode, enCommModule, enModemId);
                    break;
            default:
                    adp_dpm_printf("check your input enCommMode!\n");
                    break;
     }
     return ret;
}
static int bsp_bbp_drx_power_down(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     int ret = RET_OK;
     switch(enCommMode)
     {
            case PWC_COMM_MODE_WCDMA:
                    ret = bsp_twbbp_power_down(enCommMode, enCommModule, enModemId);
                    break;
            case PWC_COMM_MODE_GSM:
                    ret = bsp_gbbp_power_down(enCommMode, enCommModule, enModemId);
                    break;
            case PWC_COMM_MODE_LTE:
            case PWC_COMM_MODE_TDS:
                    ret = bsp_twbbp_power_down(enCommMode, enCommModule, enModemId);
                    ret |= bsp_bbp_irm_power_down(enCommMode, enCommModule, enModemId);
                    break;
            default:
                    adp_dpm_printf("check your input enCommMode!\n");
                 break;
     }
     return ret;
}

static int bsp_bbp_drx_power_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     int ret = PWC_COMM_MODEM_OFF;
     switch(enCommMode)
     {
            case PWC_COMM_MODE_WCDMA:
                    ret = bsp_twbbp_power_status(enCommMode, enCommModule, enModemId);
                    break;
            case PWC_COMM_MODE_GSM:
                    ret = bsp_gbbp_power_status(enCommMode, enCommModule, enModemId);
                    break;
            case PWC_COMM_MODE_LTE:
            case PWC_COMM_MODE_TDS:
                    ret = bsp_twbbp_power_status(enCommMode, enCommModule, enModemId);
                    ret &= bsp_bbp_irm_power_status(enCommMode, enCommModule, enModemId);
                    break;
            default:
                    adp_dpm_printf("check your input enCommMode!\n");
                 break;
     }
     return ret;
}

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_PWRUP
 ��������  : Wģ��Gģ��Lģ��PA��RF��BBP��DSP��ABB��HIFI���ϵ���ơ�
 �������  : enCommMode: PWC_COMM_MODE_WCDMA/PWC_COMM_MODE_GSM/PWC_COMM_MODE_LTE,
             enCommModule:PWC_COMM_MODULE_PA/PWC_COMM_MODULE_RF/PWC_COMM_MODULE_BBP/PWC_COMM_MODULE_DSP/PWC_COMM_MODULE_ABB,
 �������  : None
 �� �� ֵ  : PWC_PARA_INVALID/PWC_COMM_MODEM_ON/PWC_COMM_MODEM_OFF
*****************************************************************************/
BSP_U32 DRV_PWRCTRL_PWRUP (PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
	int ret = RET_OK;
        if (PWC_PARA_INVALID == check_input_param(enCommMode, enCommModule, enModemId)){
                adp_dpm_printf("check your input int DRV_PWRCTRL_PWRUP!\n");
                return PWC_PARA_INVALID;
        }
        switch(enCommModule)
        {
            case PWC_COMM_MODULE_PA:
                 ret = bsp_pa_power_up(enCommMode, enCommModule, enModemId);
                 break;
            case PWC_COMM_MODULE_RF:
                 ret = bsp_rfic_power_up(enCommMode, enCommModule, enModemId);
                 break;
            case PWC_COMM_MODULE_BBP_DRX:
                 ret = bsp_bbp_drx_power_up(enCommMode, enCommModule, enModemId);
                 break;
            case PWC_COMM_MODULE_BBE16:
                 ret = bsp_bbe16_power_up(enCommMode, enCommModule, enModemId);
                 break;
            case PWC_COMM_MODULE_ABB:
                  ret = bsp_abb_power_up(enCommMode, enCommModule, enModemId);
                  break;
            case PWC_COMM_MODULE_BBP_DPA:
                  ret = bsp_wbbp_power_up(enCommMode, enCommModule, enModemId);
                  break;
            case PWC_COMM_MODULE_BBP_MEM:
                  ret = bsp_bbp_irm_power_up(enCommMode, enCommModule, enModemId);
                  break;
            default:
                  adp_dpm_printf("check your input enCommModule!\n");
                  break;
        }
        return (BSP_U32)ret;
}

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_PWRDOWN
 ��������  : Wģ��Gģ��Lģ��PA��RF��BBP��DSP��ABB��HIFI���µ���ơ�
 �������  : enCommMode: PWC_COMM_MODE_WCDMA/PWC_COMM_MODE_GSM/PWC_COMM_MODE_LTE,
             enCommModule:PWC_COMM_MODULE_PA/PWC_COMM_MODULE_RF/PWC_COMM_MODULE_BBP/PWC_COMM_MODULE_DSP/PWC_COMM_MODULE_ABB,
 �������  : None
 �� �� ֵ  : PWC_PARA_INVALID/PWC_COMM_MODEM_ON/PWC_COMM_MODEM_OFF
*****************************************************************************/
 BSP_U32 DRV_PWRCTRL_PWRDOWN(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
 {
	int ret = RET_OK;
        if (PWC_PARA_INVALID == check_input_param(enCommMode, enCommModule, enModemId)){
                adp_dpm_printf("check your input int DRV_PWRCTRL_PWRDOWN!\n");
                return PWC_PARA_INVALID;
        }
        switch(enCommModule)
        {
            case PWC_COMM_MODULE_PA:
                 ret = bsp_pa_power_down(enCommMode, enCommModule, enModemId);
                 break;
            case PWC_COMM_MODULE_RF:
                 ret = bsp_rfic_power_down(enCommMode, enCommModule, enModemId);
                 break;
            case PWC_COMM_MODULE_BBP_DRX:
                 ret = bsp_bbp_drx_power_down(enCommMode, enCommModule, enModemId);
                 break;
            case PWC_COMM_MODULE_BBE16:
                 ret = bsp_bbe16_power_down(enCommMode, enCommModule, enModemId);
                 break;
            case PWC_COMM_MODULE_ABB:
                  ret = bsp_abb_power_down(enCommMode, enCommModule, enModemId);
                  break;
            case PWC_COMM_MODULE_BBP_DPA:
                  ret = bsp_wbbp_power_down(enCommMode, enCommModule, enModemId);
                  break;
            case PWC_COMM_MODULE_BBP_MEM:
                  ret = bsp_bbp_irm_power_down(enCommMode, enCommModule, enModemId);
                  break;
            default:
                  adp_dpm_printf("check your input enCommModule!\n");
                  break;
        }
        return (BSP_U32)ret;
 }

/*****************************************************************************
 �� �� ��  : DRV_PWRCTRL_PWRSTATUSGET
 ��������  : Wģ��Gģ��Lģ��PA��DSP��ABB��PLL״̬��ѯ��
 �������  : enCommMode: PWC_COMM_MODE_WCDMA/PWC_COMM_MODE_GSM/PWC_COMM_MODE_LTE,
             enCommModule:PWC_COMM_MODULE_PA/PWC_COMM_MODULE_DSP/PWC_COMM_MODULE_ABB,
 �������  : None
 �� �� ֵ  : PWRCTRL_GET_SUCCESS/PWRCTRL_GET_PARAINVALID/PWRCTRL_ONORLOCK/PWRCTRL_OFFORUNLOCK
*****************************************************************************/
BSP_U32 DRV_PWRCTRL_PWRSTATUSGET(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
 {
	int ret = PWC_COMM_MODEM_OFF;
        if (PWC_PARA_INVALID == check_input_param(enCommMode, enCommModule, enModemId)){
                adp_dpm_printf("check your input int DRV_PWRCTRL_PWRSTATUSGET!\n");
                return PWC_PARA_INVALID;
        }
        switch(enCommModule)
        {
            case PWC_COMM_MODULE_PA:
                 ret = bsp_pa_power_status(enCommMode, enCommModule, enModemId);
                 break;
            case PWC_COMM_MODULE_RF:
                 ret = bsp_rfic_power_status(enCommMode, enCommModule, enModemId);
                 break;
            case PWC_COMM_MODULE_BBP_DRX:
                 ret = bsp_bbp_drx_power_status(enCommMode, enCommModule, enModemId);
                 break;
            case PWC_COMM_MODULE_BBE16:
                 ret = bsp_bbe16_power_status(enCommMode, enCommModule, enModemId);
                 break;
            case PWC_COMM_MODULE_ABB:
                  ret = bsp_abb_power_status(enCommMode, enCommModule, enModemId);
                  break;
            case PWC_COMM_MODULE_BBP_DPA:
                  ret = bsp_wbbp_power_status(enCommMode, enCommModule, enModemId);
                  break;
            case PWC_COMM_MODULE_BBP_MEM:
                  ret = bsp_bbp_irm_power_status(enCommMode, enCommModule, enModemId);
                  break;
            default:
                  adp_dpm_printf("check your input enCommModule!\n");
                  break;
        }
        return (BSP_U32)ret;
 }

/*********************************************************************************
*���½ӿڽ�Ϊ�ϲ�ʹ���ṩPLL�Ŀ��ع��ܣ�
*********************************************************************************/
#ifdef CONFIG_V7R2_CLK_CRG /*CONFIG_V7R2_CLK_CRG*/
static int dfs_mode_isenable(struct pll_info *pll)
{
        unsigned int clkcon = 0;
        clkcon = readl(HI_SYSCRG_BASE_ADDR_VIRT + pll->reg);
        if (clkcon &((unsigned int)(0x1) << pll->bit)) {
            return PWC_COMM_MODEM_OFF;
        }
        return PWC_COMM_MODEM_ON;
}
static int dfs_mode_enable (struct pll_info *pll, PWC_COMM_MODE_E enCommMode)
{
        unsigned int clkcon = 0;
        unsigned int newslice =0;
        unsigned int oldslice =0;

        /*check dfs status,if enabled ,return*/
        if(dfs_mode_isenable(pll) == PWC_COMM_MODEM_ON)
                return RET_OK;

        /*PLL enable reg*/
        clkcon = readl(HI_SYSCRG_BASE_ADDR_VIRT + pll->reg);
        clkcon &= ~((unsigned int)0x1 << pll->bit);
        writel(clkcon, HI_SYSCRG_BASE_ADDR_VIRT + pll->reg);

        /*waiting for PLL stable*/
        oldslice = bsp_get_slice_value();
        do
        {
            clkcon = readl(HI_SYSCRG_BASE_ADDR_VIRT + pll->stable_reg);
            clkcon &= ((unsigned int)0x1 << pll->stable_bit);
            newslice = bsp_get_slice_value();
            if(get_timer_slice_delta(oldslice, newslice) > 2){
                  adp_dpm_printf("%s can't get pll lock signal,ModeID: %d (0:GSM, 1:WCDMA, 2:LTE, 3:TDS)!\n", pll->name, enCommMode);
            }
        } while (clkcon != ((unsigned int)0x1 << pll->stable_bit));
        return RET_OK;
}

static int dfs_mode_disable (struct pll_info *pll)
{
        unsigned int clkcon = 0;

        /*disable PLL*/
        clkcon = readl(HI_SYSCRG_BASE_ADDR_VIRT + pll->reg);
        clkcon |= ((unsigned int)0x1 << pll->bit);
        writel(clkcon, HI_SYSCRG_BASE_ADDR_VIRT + pll->reg);
        return RET_OK;
}
#elif defined(CONFIG_K3V3_CLK_CRG) /* CONFIG_K3V3_CLK_CRG */
static int dfs_mode_isenable(struct pll_info *pll)
{
        unsigned int clkcon = 0;
        clkcon = readl(HI_SYSCRG_BASE_ADDR_VIRT + pll->reg);
        if (clkcon &((unsigned int)(0x1) << pll->bit)) {
            return PWC_COMM_MODEM_ON;
        }
        return PWC_COMM_MODEM_OFF;
}

static int dfs_mode_enable (struct pll_info *pll, PWC_COMM_MODE_E enCommMode)
{
        unsigned int clkcon = 0;
        unsigned int newslice =0;
        unsigned int oldslice =0;

        /*check dfs status,if enabled ,return*/
        if(dfs_mode_isenable(pll) == PWC_COMM_MODEM_ON)
                return RET_OK;

        /*PLL enable reg*/
        clkcon = readl(HI_SYSCRG_BASE_ADDR_VIRT + pll->reg);

	/*bit[0]=1*/
        clkcon |= (unsigned int)0x1;

	/*bit[1]=0*/
        clkcon &= ~((unsigned int)0x1 << 1);
        writel(clkcon, HI_SYSCRG_BASE_ADDR_VIRT + pll->reg);

        /*waiting for PLL stable*/
        oldslice = bsp_get_slice_value();
        do
        {
            clkcon = readl(HI_SYSCRG_BASE_ADDR_VIRT + pll->reg);
            clkcon &= ((unsigned int)0x1 << 26);
            newslice = bsp_get_slice_value();
            if(get_timer_slice_delta(oldslice, newslice) > 2){
                  adp_dpm_printf("%s can't get pll lock signal,ModeID: %d (0:GSM, 1:WCDMA, 2:LTE, 3:TDS)!\n", pll->name, enCommMode);
            }
        } while (clkcon != ((unsigned int)0x1 << 26));

	/*bit[26]=1*/
	clkcon = readl(HI_SYSCRG_BASE_ADDR_VIRT + pll->stable_reg);
	clkcon |= ((unsigned int)0x1 << 26);
	writel(clkcon, HI_SYSCRG_BASE_ADDR_VIRT + pll->stable_reg);

	/*if bbe16 pll bit[24]=1*/
	if(pll->reg == 0x20c){
		writel((unsigned int)0x1 << 24, HI_SYSCRG_BASE_ADDR_VIRT + 0x18);
	}
        return RET_OK;
}
static int dfs_mode_disable (struct pll_info *pll)
{
        unsigned int clkcon = 0;

        /*disable PLL*/
	/*bit[26]=0*/
	clkcon = readl(HI_SYSCRG_BASE_ADDR_VIRT + pll->stable_reg);
	clkcon &= ~((unsigned int)0x1 << 26);
	writel(clkcon, HI_SYSCRG_BASE_ADDR_VIRT + pll->stable_reg);

        clkcon = readl(HI_SYSCRG_BASE_ADDR_VIRT + pll->reg);

	/*bit[0]=0*/
        clkcon &= ~((unsigned int)0x1);

	/*bit[1]=1*/
        clkcon |= ((unsigned int)0x1 << 1);
        writel(clkcon, HI_SYSCRG_BASE_ADDR_VIRT + pll->reg);
        return RET_OK;
}
#else
static int dfs_mode_isenable(struct pll_info *pll)
{
        return PWC_COMM_MODEM_ON;
}
static int dfs_mode_enable (struct pll_info *pll, PWC_COMM_MODE_E enCommMode)
{
	return RET_OK;
}
static int dfs_mode_disable (struct pll_info *pll)
{
        return RET_OK;
}
#endif /*CONFIG_V7R2_CLK_CRG*/


static int abb_pll_isenable(struct pll_info *pll)
{
        unsigned int clkcon = 0;
        bsp_abb_read(pll->reg, &clkcon);
        if (clkcon &(((unsigned int)0x1) << pll->bit)) {
            return PWC_COMM_MODEM_OFF;
        }
        return PWC_COMM_MODEM_ON;
}
static int abb_pll_enable(struct pll_info *pll, PWC_COMM_MODE_E enCommMode)
{
        unsigned int clkcon = 0;
        unsigned int newslice =0;
        unsigned int oldslice =0;

        /*check pll status,if enabled ,return*/
        if(abb_pll_isenable(pll) == PWC_COMM_MODEM_ON)
                return RET_OK;

        /*PLL enable reg*/
        bsp_abb_read(pll->reg, &clkcon);
        clkcon &= ~((unsigned int)0x1 << pll->bit);
        bsp_abb_write(pll->reg, clkcon);

        /*waiting for PLL stable*/
        oldslice = bsp_get_slice_value();
        do
        {
            bsp_abb_read(pll->stable_reg, &clkcon);
            clkcon &= ((unsigned int)0x1 << pll->stable_bit);
            newslice = bsp_get_slice_value();
            if(get_timer_slice_delta(oldslice, newslice) > 2){
		  break;
            }
        } while (clkcon != ((unsigned int)0x1 << pll->stable_bit));
        return RET_OK;
}

static int abb_pll_disable(struct pll_info *pll)
{
        unsigned int clkcon = 0;

        /*PLL disable reg*/
        bsp_abb_read(pll->reg, &clkcon);
        clkcon |= ((unsigned int)0x1 << pll->bit);
        bsp_abb_write(pll->reg, clkcon);
        return RET_OK;
}


/*BBE16 DFS ��������ģʽ��--[12]--[0]*/
static int bbe16_dfs_workingmode_enable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;

     spin_lock_irqsave(&vote_map[12].spinlock, flags);
     vote_map[12].enable_count[enCommMode] ++;

     vote_map[12].enable_refct ++;

#if defined(CONFIG_K3V3_CLK_CRG)
     ret = bsp_dsp_pll_enable();
     bsp_bbe_chose_pll(0);
#endif

     if(!(vote_map[12].lock & ((unsigned int)1 << enCommMode))){
            vote_map[12].lock |= ((unsigned int)1 << enCommMode);
     }
     spin_unlock_irqrestore(&vote_map[12].spinlock, flags);
     return ret;
}
static int bbe16_dfs_workingmode_disable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;

     spin_lock_irqsave(&vote_map[12].spinlock, flags);
     vote_map[12].disable_count[enCommMode] ++;
     if(vote_map[12].lock & ((unsigned int)1 << enCommMode)){
            vote_map[12].lock &= ~((unsigned int)1 << enCommMode);
     }
     if(!vote_map[12].lock)
     {
    	/*����NV�����������Ƿ�ر�*/
    	if (g_nv_pwc_switch.drx_bbe16_pll == 0)
        {
	         vote_map[12].disable_refct ++;
#if defined(CONFIG_K3V3_CLK_CRG)
             bsp_bbe_chose_pll(1);
	         ret = bsp_dsp_pll_disable();
#endif
    	}
     }
     spin_unlock_irqrestore(&vote_map[12].spinlock, flags);
     return ret;
}
static int bbe16_dfs_workingmode_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = PWC_COMM_MODEM_OFF;

     spin_lock_irqsave(&vote_map[12].spinlock, flags);

     ret = bsp_dsp_pll_status();
     if(ret == 1)
     {
        ret = PWC_COMM_MODEM_ON;
     }
     else
     {
        ret = PWC_COMM_MODEM_OFF;
     }

     spin_unlock_irqrestore(&vote_map[12].spinlock, flags);
     return ret;
}

/*BBP DFS ��������ģʽ��--[13]--[1]*/
static int bbp_dfs_workingmode_enable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     struct pll_info pll = hi6930_pll[1];
     int ret = RET_OK;

     /*GSM BBP ��������BBP PLL*/
     if(PWC_COMM_MODE_GSM == enCommMode){
		return RET_ERR;
     }
     spin_lock_irqsave(&vote_map[13].spinlock, flags);

     vote_map[13].enable_count[enCommMode] ++;
     vote_map[13].enable_refct ++;
     ret = bsp_abb_buffer_enable((enum ABB_BUFFER_ID)tl_mode_nv[0].ucTCXOSwitch, ABB_BUFFER_VOTE_BBP_PLL);
     ret |= dfs_mode_enable(&pll, enCommMode);
     if(!(vote_map[13].lock & ((unsigned int)1 << enCommMode))){
            vote_map[13].lock |= ((unsigned int)1 << enCommMode);
     }
     spin_unlock_irqrestore(&vote_map[13].spinlock, flags);
     return ret;
}
static int bbp_dfs_workingmode_disable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     struct pll_info pll = hi6930_pll[1];

     /*GSM BBP ��������BBP PLL*/
     if(PWC_COMM_MODE_GSM == enCommMode){
		return RET_ERR;
     }
     spin_lock_irqsave(&vote_map[13].spinlock, flags);

     vote_map[13].disable_count[enCommMode] ++;
     if(vote_map[13].lock & ((unsigned int)1 << enCommMode)){
            vote_map[13].lock &= ~((unsigned int)1 << enCommMode);
     }
     if(!vote_map[13].lock){

		/*����NV�����������Ƿ�ر�*/
		if (g_nv_pwc_switch.drx_bbp_pll == 0){
		         vote_map[13].disable_refct ++;
		         ret = dfs_mode_disable(&pll);
                 ret |= bsp_abb_buffer_disable((enum ABB_BUFFER_ID)tl_mode_nv[0].ucTCXOSwitch, ABB_BUFFER_VOTE_BBP_PLL);
		}
     }
     spin_unlock_irqrestore(&vote_map[13].spinlock, flags);
     return ret;
}
static int bbp_dfs_workingmode_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = PWC_COMM_MODEM_OFF;
     struct pll_info pll = hi6930_pll[1];

     spin_lock_irqsave(&vote_map[13].spinlock, flags);

     ret = dfs_mode_isenable(&pll);

     spin_unlock_irqrestore(&vote_map[13].spinlock, flags);
     return ret;
}

/*ABB CH0 LWT PLL ��������ģʽ��---[14]--[2]*/
static int abb_ch0_lwt_pll_enable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     struct pll_info pll = hi6930_pll[2];
     int ret = RET_OK;

     spin_lock_irqsave(&vote_map[14].spinlock, flags);
     vote_map[14].enable_count[enCommMode] ++;
     vote_map[14].enable_refct ++;
     ret = bsp_abb_buffer_enable(ABB_BUFFER_ID_0, ABB_BUFFER_VOTE_W_PLL);
     ret |= abb_pll_enable(&pll, enCommMode);
     if(!(vote_map[14].lock & ((unsigned int)1 << enCommMode))){
            vote_map[14].lock |= ((unsigned int)1 << enCommMode);
     }
     spin_unlock_irqrestore(&vote_map[14].spinlock, flags);
     return ret;
}

static int abb_ch0_lwt_pll_disable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     struct pll_info pll = hi6930_pll[2];

     spin_lock_irqsave(&vote_map[14].spinlock, flags);

     vote_map[14].disable_count[enCommMode] ++;
     if(vote_map[14].lock & ((unsigned int)1 << enCommMode)){
            vote_map[14].lock &= ~((unsigned int)1 << enCommMode);
     }
     if(!vote_map[14].lock){

		/*����NV�����������Ƿ�ر�*/
		if (g_nv_pwc_switch.drx_abb0_wpll == 0){
		         vote_map[14].disable_refct ++;
		         ret = abb_pll_disable(&pll);
                 ret |= bsp_abb_buffer_disable(ABB_BUFFER_ID_0, ABB_BUFFER_VOTE_W_PLL);
		}
     }
     spin_unlock_irqrestore(&vote_map[14].spinlock, flags);
     return ret;
}

static int abb_ch0_lwt_pll_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = PWC_COMM_MODEM_OFF;
     struct pll_info pll = hi6930_pll[2];

     spin_lock_irqsave(&vote_map[14].spinlock, flags);

     ret = abb_pll_isenable(&pll);

     spin_unlock_irqrestore(&vote_map[14].spinlock, flags);
     return ret;
}



/*ABB CH0 GSM PLL ��������ģʽ��--[15]--[3]*/
static int abb_ch0_g_pll_enable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     struct pll_info pll = hi6930_pll[3];
     int ret = RET_OK;

     spin_lock_irqsave(&vote_map[15].spinlock, flags);

    vote_map[15].enable_count[enCommMode] ++;
    vote_map[15].enable_refct ++;

    /*open ABB 104M*/
    ret = bsp_abb_buffer_enable(ABB_BUFFER_ID_0, ABB_BUFFER_VOTE_G_PLL);

    bsp_abb_write(0xD1, 0x00);

    ret |= abb_pll_enable(&pll, enCommMode);
     if(!(vote_map[15].lock & ((unsigned int)1 << enCommMode))){
            vote_map[15].lock |= ((unsigned int)1 << enCommMode);
     }
     spin_unlock_irqrestore(&vote_map[15].spinlock, flags);
     return ret;
}

static int abb_ch0_g_pll_disable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     struct pll_info pll = hi6930_pll[3];

     spin_lock_irqsave(&vote_map[15].spinlock, flags);

     vote_map[15].disable_count[enCommMode] ++;
     if(vote_map[15].lock & ((unsigned int)1 << enCommMode)){
            vote_map[15].lock &= ~((unsigned int)1 << enCommMode);
     }
     if(!vote_map[15].lock){

		/*����NV�����������Ƿ�ر�*/
		if (g_nv_pwc_switch.drx_abb0_gpll == 0){
		     vote_map[15].disable_refct ++;
		     ret = abb_pll_disable(&pll);

		    /*close ABB 104M*/
		     bsp_abb_write(0xD1, 0x20);
            ret |= bsp_abb_buffer_disable(ABB_BUFFER_ID_0, ABB_BUFFER_VOTE_G_PLL);
		}
     }
     spin_unlock_irqrestore(&vote_map[15].spinlock, flags);
     return ret;
}

static int abb_ch0_g_pll_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = PWC_COMM_MODEM_OFF;
     struct pll_info pll = hi6930_pll[3];

     spin_lock_irqsave(&vote_map[15].spinlock, flags);

     ret = abb_pll_isenable(&pll);

     spin_unlock_irqrestore(&vote_map[15].spinlock, flags);
     return ret;
}


/*ABB CH1  LWT PLL ��������ģʽ��--[16]--[4]*/
static int abb_ch1_lwt_pll_enable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     struct pll_info pll = hi6930_pll[4];
     int ret = RET_OK;

     spin_lock_irqsave(&vote_map[16].spinlock, flags);

     vote_map[16].enable_count[enCommMode] ++;
     vote_map[16].enable_refct ++;
     ret = bsp_abb_buffer_enable(ABB_BUFFER_ID_1, ABB_BUFFER_VOTE_W_PLL);
     ret |= abb_pll_enable(&pll, enCommMode);
     if(!(vote_map[16].lock & ((unsigned int)1 << enCommMode))){
            vote_map[16].lock |= ((unsigned int)1 << enCommMode);
     }
     spin_unlock_irqrestore(&vote_map[16].spinlock, flags);
     return ret;
}
static int abb_ch1_lwt_pll_disable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     struct pll_info pll = hi6930_pll[4];

     spin_lock_irqsave(&vote_map[16].spinlock, flags);
     vote_map[16].disable_count[enCommMode] ++;
     if(vote_map[16].lock & ((unsigned int)1 << enCommMode)){
            vote_map[16].lock &= ~((unsigned int)1 << enCommMode);
     }
     if(!vote_map[16].lock){

		/*����NV�����������Ƿ�ر�*/
		if (g_nv_pwc_switch.drx_abb1_wpll == 0){
		         vote_map[16].disable_refct ++;
		         ret = abb_pll_disable(&pll);
                 ret |= bsp_abb_buffer_disable(ABB_BUFFER_ID_1, ABB_BUFFER_VOTE_W_PLL);
		}
     }
     spin_unlock_irqrestore(&vote_map[16].spinlock, flags);
     return ret;
}
static int abb_ch1_lwt_pll_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = PWC_COMM_MODEM_OFF;
     struct pll_info pll = hi6930_pll[4];

     spin_lock_irqsave(&vote_map[16].spinlock, flags);

     ret = abb_pll_isenable(&pll);

     spin_unlock_irqrestore(&vote_map[16].spinlock, flags);
     return ret;
}


/*ABB CH1 GSM PLL ��������ģʽ��--[17]--[5]*/
static int abb_ch1_g_pll_enable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     struct pll_info pll = hi6930_pll[5];
     int ret = RET_OK;

     spin_lock_irqsave(&vote_map[17].spinlock, flags);

     vote_map[17].enable_count[enCommMode] ++;
     vote_map[17].enable_refct ++;

    ret = bsp_abb_buffer_enable(ABB_BUFFER_ID_1, ABB_BUFFER_VOTE_G_PLL);
     /*open ABB 104M*/
     bsp_abb_write(0x71, 0x00);
     ret |= abb_pll_enable(&pll, enCommMode);
     if(!(vote_map[17].lock & ((unsigned int)1 << enCommMode))){
            vote_map[17].lock |= ((unsigned int)1 << enCommMode);
     }
     spin_unlock_irqrestore(&vote_map[17].spinlock, flags);
     return ret;
}

/*ABB LWT PLL  ��������ģʽ�ر�*/
static int abb_ch1_g_pll_disable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     struct pll_info pll = hi6930_pll[5];

     spin_lock_irqsave(&vote_map[17].spinlock, flags);

     vote_map[17].disable_count[enCommMode] ++;
     if(vote_map[17].lock & ((unsigned int)1 << enCommMode)){
            vote_map[17].lock &= ~((unsigned int)1 << enCommMode);
     }
     if(!vote_map[17].lock){

		/*����NV�����������Ƿ�ر�*/
		if (g_nv_pwc_switch.drx_abb1_gpll == 0){
		         vote_map[17].disable_refct ++;
		         ret = abb_pll_disable(&pll);

			 /*close  ABB 104M*/
		         bsp_abb_write(0x71, 0x20);
                 ret |= bsp_abb_buffer_disable(ABB_BUFFER_ID_1, ABB_BUFFER_VOTE_G_PLL);
		}
     }
     spin_unlock_irqrestore(&vote_map[17].spinlock, flags);
     return ret;
}

/*ABB LWT PLL  ��������ģʽ״̬��ѯ*/
static int abb_ch1_g_pll_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = PWC_COMM_MODEM_OFF;
     struct pll_info pll = hi6930_pll[5];

     spin_lock_irqsave(&vote_map[17].spinlock, flags);

     ret = abb_pll_isenable(&pll);

     spin_unlock_irqrestore(&vote_map[17].spinlock, flags);
     return ret;
}

/*ABB ch pll */
static int abb_ch_pll_enable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
        int ret = RET_OK;
        if(PWC_COMM_MODEM_0 == enModemId){
                switch (enCommMode)
                {
                        case PWC_COMM_MODE_LTE:
                        case PWC_COMM_MODE_WCDMA:
                        case PWC_COMM_MODE_TDS:
                                ret = abb_ch0_lwt_pll_enable(enCommMode, enCommModule, enModemId);
                                break;
                        case PWC_COMM_MODE_GSM:
                                ret = abb_ch0_g_pll_enable(enCommMode, enCommModule, enModemId);
                                break;
                        default:
                                adp_dpm_printf("check your input enCommMode!\n");
                                break;
                }

        }else if(PWC_COMM_MODEM_1 == enModemId){
                switch (enCommMode)
                {
                        case PWC_COMM_MODE_LTE:
                        case PWC_COMM_MODE_WCDMA:
                        case PWC_COMM_MODE_TDS:
                                ret = abb_ch1_lwt_pll_enable(enCommMode, enCommModule, enModemId);
                                break;
                        case PWC_COMM_MODE_GSM:
                                ret = abb_ch1_g_pll_enable(enCommMode, enCommModule, enModemId);
                                break;
                        default:
                                adp_dpm_printf("check your input enCommMode!\n");
                                break;
                }
         }
         return ret;
}

static int abb_ch_pll_disable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
        int ret = RET_OK;
        if(PWC_COMM_MODEM_0 == enModemId){
                switch (enCommMode)
                {
                        case PWC_COMM_MODE_LTE:
                        case PWC_COMM_MODE_WCDMA:
                        case PWC_COMM_MODE_TDS:
                                ret = abb_ch0_lwt_pll_disable(enCommMode, enCommModule, enModemId);
                                break;
                        case PWC_COMM_MODE_GSM:
                                ret = abb_ch0_g_pll_disable(enCommMode, enCommModule, enModemId);
                                break;
                        default:
                                adp_dpm_printf("check your input enCommMode!\n");
                                break;
                }

        }else if(PWC_COMM_MODEM_1 == enModemId){
                switch (enCommMode)
                {
                        case PWC_COMM_MODE_LTE:
                        case PWC_COMM_MODE_WCDMA:
                        case PWC_COMM_MODE_TDS:
                                ret = abb_ch1_lwt_pll_disable(enCommMode, enCommModule, enModemId);
                                break;
                        case PWC_COMM_MODE_GSM:
                                ret = abb_ch1_g_pll_disable(enCommMode, enCommModule, enModemId);
                                break;
                        default:
                                adp_dpm_printf("check your input enCommMode!\n");
                                break;
                }
         }
         return ret;
}


static int abb_ch_pll_status(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
        int ret = PWC_COMM_MODEM_OFF;
        if(PWC_COMM_MODEM_0 == enModemId){
                switch (enCommMode)
                {
                        case PWC_COMM_MODE_LTE:
                        case PWC_COMM_MODE_WCDMA:
                        case PWC_COMM_MODE_TDS:
                                ret = abb_ch0_lwt_pll_status(enCommMode, enCommModule, enModemId);
                                break;
                        case PWC_COMM_MODE_GSM:
                                ret = abb_ch0_g_pll_status(enCommMode, enCommModule, enModemId);
                                break;
                        default:
                                adp_dpm_printf("check your input enCommMode!\n");
                                break;
                }

        }else if(PWC_COMM_MODEM_1 == enModemId){
                switch (enCommMode)
                {
                        case PWC_COMM_MODE_LTE:
                        case PWC_COMM_MODE_WCDMA:
                        case PWC_COMM_MODE_TDS:
                                ret = abb_ch1_lwt_pll_status(enCommMode, enCommModule, enModemId);
                                break;
                        case PWC_COMM_MODE_GSM:
                                ret = abb_ch1_g_pll_status(enCommMode, enCommModule, enModemId);
                                break;
                        default:
                                adp_dpm_printf("check your input enCommMode!\n");
                                break;
                }
         }
         return ret;
}

static unsigned int bbp_pll_enable_count = 0;
static unsigned int bbp_pll_disable_count = 0;

#ifdef CONFIG_K3V3_CLK_CRG /*CONFIG_K3V3_CLK_CRG*/
int bsp_bbp_pll_enable(void)
{
	unsigned long flags = 0;
	struct pll_info pll = hi6930_pll[1];
	int ret = RET_OK;

	spin_lock_irqsave(&vote_map[13].spinlock, flags);
	ret = dfs_mode_enable(&pll, PWC_COMM_MODE_LTE);
	bbp_pll_enable_count++;
	spin_unlock_irqrestore(&vote_map[13].spinlock, flags);
	return ret;
}

int bsp_bbp_pll_disable(void)
{
	unsigned long flags = 0;
	int ret = RET_OK;
	struct pll_info pll = hi6930_pll[1];

	spin_lock_irqsave(&vote_map[13].spinlock, flags);
	if(!vote_map[13].lock){

	   /*����NV�����������Ƿ�ر�*/
	   if (g_nv_pwc_switch.drx_bbp_pll == 0){
		bbp_pll_disable_count++;
		ret = dfs_mode_disable(&pll);
	   }
	}
	spin_unlock_irqrestore(&vote_map[13].spinlock, flags);
	return ret;
}

#else
int bsp_bbp_pll_enable(void)
{
	return RET_OK;
}
int bsp_bbp_pll_disable(void)
{
	return RET_OK;
}
#endif

  BSP_U32 DRV_PWRCTRL_PLLSTATUSGET(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enCommModem)
 {
        int ret = PWC_COMM_MODEM_OFF;
        if (PWC_PARA_INVALID == check_input_param(enCommMode, enCommModule, enCommModem)){
                adp_dpm_printf("check your input int DRV_PWRCTRL_PLLSTATUSGET!\n");
                return PWC_PARA_INVALID;
        }
        switch (enCommModule)
        {
                case PWC_COMM_MODULE_BBE16:
                        ret = bbe16_dfs_workingmode_status(enCommMode, enCommModule, enCommModem);
                        break;
                case PWC_COMM_MODULE_BBP_DRX:
                        ret = bbp_dfs_workingmode_status(enCommMode, enCommModule, enCommModem);
                        break;
                case PWC_COMM_MODULE_ABB:
                        ret = abb_ch_pll_status(enCommMode, enCommModule, enCommModem);
                        break;
                default:
                        adp_dpm_printf("check your input enCommModule!\n");
                        break;
        }
	return (BSP_U32)ret;
 }


 BSP_U32 DRV_PWRCTRL_PLLENABLE (PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enCommModem)
 {
        int ret = RET_OK;
        if (PWC_PARA_INVALID == check_input_param(enCommMode, enCommModule, enCommModem)){
                adp_dpm_printf("check your input int DRV_PWRCTRL_PLLENABLE!\n");
                return PWC_PARA_INVALID;
        }
        switch (enCommModule)
        {
                case PWC_COMM_MODULE_BBE16:
                        ret = bbe16_dfs_workingmode_enable(enCommMode, enCommModule, enCommModem);
                        break;
                case PWC_COMM_MODULE_BBP_DRX:
                        ret = bbp_dfs_workingmode_enable(enCommMode, enCommModule, enCommModem);
                        break;
                case PWC_COMM_MODULE_ABB:
                        ret = abb_ch_pll_enable(enCommMode, enCommModule, enCommModem);
                        break;
                default:
                        adp_dpm_printf("check your input enCommModule!\n");
                        break;
        }
	return (BSP_U32)ret;
 }


 BSP_U32 DRV_PWRCTRL_PLLDISABLE (PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enCommModem)
 {

        int ret = RET_OK;
        if (PWC_PARA_INVALID == check_input_param(enCommMode, enCommModule, enCommModem)){
                adp_dpm_printf("check your input int DRV_PWRCTRL_PLLDISABLE!\n");
                return PWC_PARA_INVALID;
        }
        switch (enCommModule)
        {
                case PWC_COMM_MODULE_BBE16:
                        ret = bbe16_dfs_workingmode_disable(enCommMode, enCommModule, enCommModem);
                        break;
                case PWC_COMM_MODULE_BBP_DRX:
                        ret = bbp_dfs_workingmode_disable(enCommMode, enCommModule, enCommModem);
                        break;
                case PWC_COMM_MODULE_ABB:
                        ret = abb_ch_pll_disable(enCommMode, enCommModule, enCommModem);
                        break;
                default:
                        adp_dpm_printf("check your input enCommModule!\n");
                        break;
        }
	return (BSP_U32)ret;
 }

/*BBP WCDMA CLK ʹ�� ��������ģʽ��---[19]*/
static int bbp_wcdma_clk_enable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
	unsigned long flags = 0;
	int ret = RET_OK;
	spin_lock_irqsave(&vote_map[19].spinlock, flags);
	vote_map[19].enable_count[enCommMode] ++;
	if(!vote_map[19].lock){
		vote_map[19].enable_refct ++;
		ret =bsp_bbp_pwrctrl_wbbp_clk_enable();
	}
	if(!(vote_map[19].lock & ((unsigned int)1 << enCommMode))){
		   vote_map[19].lock |= ((unsigned int)1 << enCommMode);
	}
	spin_unlock_irqrestore(&vote_map[19].spinlock, flags);
	return ret;
}

/*BBP WCDMA CLK ʹ�� ��������ģʽ��---[19]*/
static int bbp_wcdma_clk_disable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     spin_lock_irqsave(&vote_map[19].spinlock, flags);
    vote_map[19].disable_count[enCommMode] ++;
     if(vote_map[19].lock & ((unsigned int)1 << enCommMode)){
            vote_map[19].lock &= ~((unsigned int)1 << enCommMode);
     }
     if(!vote_map[19].lock){
         vote_map[19].disable_refct ++;
         ret = bsp_bbp_pwrctrl_wbbp_clk_disable();
     }
     spin_unlock_irqrestore(&vote_map[19].spinlock, flags);
     return ret;
}
/*BBP TDS CLK ʹ�� ��������ģʽ��---[20]*/
static int bbp_tds_clk_enable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
	unsigned long flags = 0;
	int ret = RET_OK;
	spin_lock_irqsave(&vote_map[20].spinlock, flags);
	vote_map[20].enable_count[enCommMode] ++;
	if(!vote_map[20].lock){
		vote_map[20].enable_refct ++;
		ret = bsp_bbp_pwrctrl_tdsbbp_clk_enable();
	}
	if(!(vote_map[20].lock & ((unsigned int)1 << enCommMode))){
		   vote_map[20].lock |= ((unsigned int)1 << enCommMode);
	}
	spin_unlock_irqrestore(&vote_map[20].spinlock, flags);
	return ret;
}

/*BBP TDS CLK ʹ�� ��������ģʽ��---[20]*/
static int bbp_tds_clk_disable(PWC_COMM_MODE_E enCommMode, PWC_COMM_MODULE_E enCommModule, PWC_COMM_MODEM_E enModemId)
{
     unsigned long flags = 0;
     int ret = RET_OK;
     spin_lock_irqsave(&vote_map[20].spinlock, flags);
    vote_map[20].disable_count[enCommMode] ++;
     if(vote_map[20].lock & ((unsigned int)1 << enCommMode)){
            vote_map[20].lock &= ~((unsigned int)1 << enCommMode);
     }
     if(!vote_map[20].lock){
         vote_map[20].disable_refct ++;
         ret = bsp_bbp_pwrctrl_tdsbbp_clk_disable();
     }
     spin_unlock_irqrestore(&vote_map[20].spinlock, flags);
     return ret;
}

/***************************************************************************
 �� �� ��  : DRV_PWRCTRL_BBPCLKENABLE()
 ��������  : TCXO ��ʱ������
 �������  : enModeId      ͨ��ģʽ
    	                     enModuleId    ͨ��ģ��
    	                     enModemId    ����
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
                         RET_ERR        ʧ��
**************************************************************************/
BSP_S32	DRV_PWRCTRL_BBPCLKENABLE(PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId)
{
	int ret = RET_OK;
	if (PWC_PARA_INVALID == check_input_param(enModeId, enModuleId, enModemId)){
			adp_dpm_printf("check your input int DRV_PWRCTRL_BBPCLKENABLE!\n");
			return PWC_PARA_INVALID;
	}
	if(enModuleId == PWC_COMM_MODULE_BBP_DRX){
	        switch (enModeId)
	        {
	                case PWC_COMM_MODE_WCDMA:
	                        ret = bbp_wcdma_clk_enable(enModeId, enModuleId, enModemId);
	                        break;
	                case PWC_COMM_MODE_TDS:
	                        ret = bbp_tds_clk_enable(enModeId, enModuleId, enModemId);
	                        break;
	                default:
                            ret = RET_ERR;
	                        break;
	 	    }
    }
	return ret;
}

/***************************************************************************
 �� �� ��  : DRV_PWRCTRL_BBPCLKDISABLE()
 ��������  : TCXO ��ʱ������
 �������  : enModeId      ͨ��ģʽ
    	                     enModuleId    ͨ��ģ��
    	                     enModemId    ����
 �������  : ��
 �� �� ֵ  : RET_OK         �ɹ�
                         RET_ERR        ʧ��
**************************************************************************/
BSP_S32	DRV_PWRCTRL_BBPCLKDISABLE(PWC_COMM_MODE_E enModeId, PWC_COMM_MODULE_E enModuleId, PWC_COMM_MODEM_E enModemId)
{
	int ret = RET_OK;
	if (PWC_PARA_INVALID == check_input_param(enModeId, enModuleId, enModemId)){
			adp_dpm_printf("check your input int DRV_PWRCTRL_BBPCLKDISABLE!\n");
			return PWC_PARA_INVALID;
	}
	if(enModuleId == PWC_COMM_MODULE_BBP_DRX){
	        switch (enModeId)
	        {
	                case PWC_COMM_MODE_WCDMA:
	                        ret = bbp_wcdma_clk_disable(enModeId, enModuleId, enModemId);
	                        break;
	                case PWC_COMM_MODE_TDS:
	                        ret = bbp_tds_clk_disable(enModeId, enModuleId, enModemId);
	                        break;
	                default:
                            ret = RET_ERR;
	                        break;
	 	}
    }
	return ret;
}

/*���ڲ�ѯ����IP���µ�Ϳ����ӵ�ͶƱ���������PWC_COMM_MODE_BUTTֻ���PM����ʹ��*/
void debug_adp_dpm(void)
{
        int i = 0;
        unsigned int vote = 0;

        vote_map[0].status = bsp_get_pa_powerstatus(PWC_COMM_MODEM_0);
        vote_map[1].status = bsp_get_pa_powerstatus(PWC_COMM_MODEM_1);
        vote_map[2].status = bsp_get_rf_powerstatus(PWC_COMM_MODEM_0);
        vote_map[3].status = bsp_get_rf_powerstatus(PWC_COMM_MODEM_1);
        vote_map[4].status = bbp_pwrctrl_irm_status();
        vote_map[5].status = PWC_COMM_MODEM_ON;
        vote_map[6].status = PWC_COMM_MODEM_ON;
        vote_map[7].status = bsp_bbp_pwrctrl_g1bbp_status();
        vote_map[8].status = bsp_bbp_pwrctrl_g2bbp_status();
        vote_map[9].status = bsp_bbp_pwrctrl_wbbp_status();
        vote_map[10].status = PWC_COMM_MODEM_ON;
        vote_map[11].status = bsp_bbp_pwrctrl_twbbp_status();
        vote_map[12].status = bbe16_dfs_workingmode_status(PWC_COMM_MODE_WCDMA,PWC_COMM_MODULE_BBE16, PWC_COMM_MODEM_0);
        vote_map[13].status = bbp_dfs_workingmode_status(PWC_COMM_MODE_WCDMA,PWC_COMM_MODULE_BBP_DRX,PWC_COMM_MODEM_0);
        vote_map[14].status = abb_ch0_lwt_pll_status(PWC_COMM_MODE_WCDMA,PWC_COMM_MODULE_ABB,PWC_COMM_MODEM_0);
        vote_map[15].status = abb_ch0_g_pll_status(PWC_COMM_MODE_GSM,PWC_COMM_MODULE_ABB,PWC_COMM_MODEM_0);
        vote_map[16].status = abb_ch1_lwt_pll_status(PWC_COMM_MODE_WCDMA,PWC_COMM_MODULE_ABB,PWC_COMM_MODEM_1);
        vote_map[17].status = abb_ch1_g_pll_status(PWC_COMM_MODE_GSM,PWC_COMM_MODULE_ABB,PWC_COMM_MODEM_1);
	    vote_map[18].status = regulator_is_enabled(pastar_regulator);
	    vote_map[19].status = 0;
	    vote_map[20].status = 0;
        while( NULL != vote_map[i].name){
                vote = vote_map[i].lock;
                adp_dpm_printf("name:%-16s,vote map: WCDMA: %d,GSM: %d,LTE: %d,TDS: %d\n",vote_map[i].name, (vote >> PWC_COMM_MODE_WCDMA) & 0x1,\
                     (vote >> PWC_COMM_MODE_GSM)&0x1, (vote >> PWC_COMM_MODE_LTE)&0x1, (vote >> PWC_COMM_MODE_TDS)&0x1);

                adp_dpm_printf("name:%-16s,en_count: WCDMA: %d,GSM: %d,LTE: %d,TDS: %d\n", vote_map[i].name, vote_map[i].enable_count[PWC_COMM_MODE_WCDMA], vote_map[i].enable_count[PWC_COMM_MODE_GSM],\
                          vote_map[i].enable_count[PWC_COMM_MODE_LTE], vote_map[i].enable_count[PWC_COMM_MODE_TDS]);

                adp_dpm_printf("name:%-16s,discount: WCDMA: %d,GSM: %d,LTE: %d,TDS: %d\n", vote_map[i].name, vote_map[i].disable_count[PWC_COMM_MODE_WCDMA], vote_map[i].disable_count[PWC_COMM_MODE_GSM],\
                          vote_map[i].disable_count[PWC_COMM_MODE_LTE], vote_map[i].disable_count[PWC_COMM_MODE_TDS]);

                adp_dpm_printf("name:%-16s,PM  vote: %d,PM enable: %d,PM disable: %d,NV ctrol:%d\n", vote_map[i].name, (vote >> PWC_COMM_MODE_BUTT)&0x1, vote_map[i].enable_count[PWC_COMM_MODE_BUTT],\
                         vote_map[i].disable_count[PWC_COMM_MODE_BUTT], (vote >> PWRCTRL_NV_BIT)&0x1);

                adp_dpm_printf("name:%-16s,status  : 0x%x,valid en_count: %d,valid discount:%d\n\n", vote_map[i].name, vote_map[i].status, vote_map[i].enable_refct, vote_map[i].disable_refct);
                i++;
        }
	adp_dpm_printf("cipher enable BBP pll count : %d, cipher disable BBP pll count : %d(all only once is normal)\n", bbp_pll_enable_count, bbp_pll_disable_count);
        return;
}






