/*************************************************************************
*   ��Ȩ����(C) 1987-2012, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  ipf_balong.c
*
*   ��    �� :  luting chendongyue
*
*   ��    �� :  ������Ҫ���IP���˽ӿں�����ʵ�����к����е�IP���˹���
*
*   �޸ļ�¼ :  2011��1��11��  v1.00  luting  ����
*                            2012��11��23��v2.00 chendongyue �޸�
*                            2013��3��29��v2.10 chendongyue �޸�
*
*************************************************************************/
/*lint -save -e429 -e529 -e534 -e550 -e650 -e661 -e715 -e537  -e737 -e539 -e574 -e239 -e438 -e701 -e740 -e958 -e451
-e64 -e732 -e740
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __VXWORKS__
#include <vxWorks.h>
#include <cacheLib.h>
#include <intLib.h>
#include <logLib.h>
#include <string.h>
#include <taskLib.h>
#include <memLib.h>
#include <config.h>
#include <stdio.h>
#include <stdlib.h>

#include "arm_pbxa9.h"
#include "product_config.h"

#include "TtfDrvInterface.h"
#include "bsp_dpm.h"
#include "bsp_hardtimer.h"
#endif

#ifdef __KERNEL__ 
#include <linux/amba/bus.h>
#include <linux/io.h> 
#include <linux/gfp.h>
#include <linux/clkdev.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/mm.h>
#include <linux/mman.h>

#include <asm/system.h>
#include <asm/irq.h>
#include <asm/mach-types.h>
#include <asm/pgtable.h>
#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <asm/mach/map.h>
#include <asm/mach/time.h>
#include <asm/io.h>
#include <asm/system.h>

#endif
#include <bsp_clk.h>
#include <bsp_hardtimer.h>

#if (defined(BSP_CONFIG_HI3630))
#include <bsp_ipc.h>
#endif
#include <bsp_ipf.h>

IPF_UL_S g_stIpfUl = {0};
IPF_DL_S g_stIpfDl = {0};
/* ������Ϣ�ṹ�� */
IPF_DEBUG_INFO_S* g_stIPFDebugInfo = NULL;
BSP_U32 g_IPFInit;

#if (defined(BSP_CONFIG_HI3630))
/*k3�͹��Ļָ����*/
u32* ipf_acore_init_status = NULL;
u32* ipf_ccore_init_status = NULL;
#endif

#ifdef CONFIG_BALONG_MODEM_RESET
/*C������(��������)��ʾ*/
IPF_FORREST_CONTROL_E* modem_reset_flag = NULL;
#endif

#ifdef __VXWORKS__

IPF_MATCH_INFO_S* g_pstExFilterAddr;
IPF_ID_S* g_stIPFExtFreeList = NULL;
IPF_ID_S* g_stIPFBasicFreeList = NULL;
IPF_ID_S* g_stIPFFreeList = NULL;
IPF_ID_S* g_stIPFHeadList = NULL;
IPF_FILTER_INFO_S g_stIPFFilterInfo[IPF_FILTER_CHAIN_MAX_NUM];
IPF_PWRCTL_FILTER_INFO_S g_stIPFPwrCtlFilterInfo[IPF_FILTER_CHAIN_MAX_NUM];
BSP_U32 g_u32IPFPwrCtlStart = 0;
BSP_U32 g_u32IpfSwitchInt = 0;
spinlock_t ipf_filter_spinlock;
u32 g_filter_delay_time = 10;

BSP_S32 BSP_IPF_BDInfo(IPF_CHANNEL_TYPE_E eChnType, BSP_U32 u32BdqPtr);
BSP_S32 BSP_IPF_RDInfo(IPF_CHANNEL_TYPE_E eChnType, BSP_U32 u32RdqPtr);
BSP_S32 BSP_IPF_ADInfo(IPF_CHANNEL_TYPE_E eChnType, BSP_U32 u32AdqPtr, BSP_U32 u32AdType);
BSP_VOID IPF_AddToBasicFreeList(IPF_ID_S* stNode);
BSP_VOID IPF_AddToExtFreeList(IPF_ID_S* stNode);
IPF_ID_S* IPF_MallocOneBasicFilter(BSP_VOID);
IPF_ID_S* IPF_MallocOneExtFilter(BSP_VOID);
BSP_S32 IPF_ConfigCD(TTF_MEM_ST *pstTtf, BSP_U16* pu16TotalDataLen, BSP_U32* pu32BdInPtr);
BSP_U32 BSP_IPF_GetDlRdNum(BSP_VOID);
BSP_U32 BSP_IPF_GetUlRdNum(BSP_VOID);
BSP_VOID BSP_IPF_UseFilterInfo(IPF_FILTER_CHAIN_TYPE_E eFilterChainhead);
BSP_VOID BSP_IPF_BasicFreeFilterInfo(BSP_VOID);
BSP_VOID BSP_IPF_ExtFreeFilterInfo(BSP_VOID);
BSP_VOID BSP_IPF_FilterInfoHWID(BSP_U32 u32Num);
BSP_VOID BSP_IPF_FilterInfoPSID(IPF_FILTER_CHAIN_TYPE_E eFilterChainhead, BSP_U32 u32Num);
BSP_VOID BSP_IPF_BackupFilter(BSP_U32 u32FilterID);
BSP_S32 BSP_IPF_Shared_DDR_Info(BSP_VOID);


#ifdef CONFIG_CCORE_PM
static int ipf_ul_dpm_prepare(struct dpm_device *dev);
static int ipf_ul_suspend(struct dpm_device *dev);
static int ipf_ul_resume(struct dpm_device *dev);
static int ipf_ul_dpm_complete(struct dpm_device *dev);

struct dpm_device ipf_dpm_device={
	.device_name = "ipf_dpm",
	.prepare = ipf_ul_dpm_prepare,
	.suspend_late = NULL,
	.suspend = ipf_ul_suspend,
	.resume = ipf_ul_resume,
	.resume_early = NULL,
	.complete = ipf_ul_dpm_complete,
};
#endif

#endif

	
#ifdef __KERNEL__
static int ipf_probe(struct platform_device *pdev);
static int ipf_remove(struct platform_device *pdev);

#ifdef CONFIG_BALONG_MODEM_RESET
int bsp_ipf_reset_ccore_cb(DRV_RESET_CALLCBFUN_MOMENT eparam, int userdata);
#endif

#ifdef CONFIG_PM
static int ipf_dl_dpm_prepare(struct device *pdev);
static void ipf_dl_dpm_complete(struct device *pdev);
static int ipf_dl_suspend(struct device *dev);
static int ipf_dl_resume(struct device *dev);


static const struct dev_pm_ops balong_ipf_dev_pm_ops ={
	.prepare = ipf_dl_dpm_prepare,
	.suspend_noirq = ipf_dl_suspend,
	.resume_noirq = ipf_dl_resume,
	.complete = ipf_dl_dpm_complete,
};
#define BALONG_DEV_PM_OPS (&balong_ipf_dev_pm_ops)

#if (defined(BSP_CONFIG_HI3630))
spinlock_t ipf_filter_spinlock;
#endif

#endif

/*C������(��������)��ʾ���ñ�־����Acore�����Ƿ������������*/
IPF_FORREST_CONTROL_E g_eCcoreResetFlag = IPF_FORRESET_CONTROL_ALLOW;

static struct platform_driver balong_driver_ipf = {
	.probe		= ipf_probe,
	.remove		= ipf_remove,

	.driver		= {
		.name	= "balong_ipf_v700r200",
		.owner	= THIS_MODULE,
#ifdef CONFIG_PM
		.pm		= BALONG_DEV_PM_OPS
#endif
	},
};

struct platform_device balong_device_ipf = {
	.name		  = "balong_ipf_v700r200",
	.id                	  =0,
};



#endif


/* IPF transfer time recorder start */
#define IPF_MAX_STAMP_ORDER             32
#define IPF_MAX_TIME_LIMIT              (19200000*10)
#define IPF_FLS_MASK                    (31)


typedef struct tagIPF_TIMESTAMP_INFO_S
{
    unsigned int diff_order_cnt[IPF_MAX_STAMP_ORDER];
    unsigned int cnt_sum;
    unsigned int diff_sum;
    unsigned int diff_max;
    unsigned int overflow;
}IPF_TIMESTAMP_INFO_S;

#ifdef __KERNEL__
IPF_TIMESTAMP_INFO_S g_ipf_dl_timestamp_info;
unsigned int g_ipf_ul_start_enable;
#define IPF_GET_TIMESTAMP_INFO() (&g_ipf_dl_timestamp_info)
#define IPF_START_STAMP_ENABLE (g_ipf_ul_start_enable)
#endif

#ifdef __VXWORKS__
IPF_TIMESTAMP_INFO_S g_ipf_ul_timestamp_info;
unsigned int g_ipf_dl_start_enable;
#define IPF_GET_TIMESTAMP_INFO() (&g_ipf_ul_timestamp_info)
#define IPF_START_STAMP_ENABLE (g_ipf_dl_start_enable)
#endif

/*
 * On ARMv5 and above those functions can be implemented around
 * the clz instruction.
 * refer to kernel/arch/arm/include/asm/bitops.h
 * put the code here for both vxWorks and linux version.
 */
static inline int ipf_fls(int x)
{
    int ret;

    asm("clz\t%0, %1" : "=r" (ret) : "r" (x));
    ret = 32 - ret;
    return ret;
}

static inline void ipf_record_start_time_stamp(BSP_U32 en, BSP_U32* rec_point)
{
    if (!en) {
        return;
    }
    *rec_point = bsp_get_slice_value_hrt();
    return;
}

static inline void
ipf_record_end_time_stamp(BSP_U32 en, BSP_U32 beg_time)
{
    unsigned int diff_time;
    IPF_TIMESTAMP_INFO_S* stamp_info = IPF_GET_TIMESTAMP_INFO();
    int idx;

    if (!en) {
        return;
    }

    diff_time = bsp_get_slice_value_hrt() - beg_time;

    /* avoid to record the overflowed value */
    if (diff_time > IPF_MAX_TIME_LIMIT) {
        stamp_info->overflow++;
    }
    else {
        if (diff_time > stamp_info->diff_max)
            stamp_info->diff_max = diff_time;

        stamp_info->diff_sum += diff_time;
        stamp_info->cnt_sum++;

        /* find the first bit not zero */
        idx = ((ipf_fls(diff_time)-1) & IPF_FLS_MASK);
        stamp_info->diff_order_cnt[idx]++;
    }
}

static inline
unsigned int ipf_calc_percent(unsigned int value, unsigned int sum)
{
    if (0 == sum) {
        return 0;
    }
    return (value * 100 / sum);
}

void ipf_enable_ul_time_stamp(int en)
{
    g_stIPFDebugInfo->ipf_timestamp_ul_en = en;
    return;
}

void ipf_enable_dl_time_stamp(int en)
{
    g_stIPFDebugInfo->ipf_timestamp_dl_en = en;
    return;
}

void ipf_clear_time_stamp(void)
{
    IPF_TIMESTAMP_INFO_S* stamp_info = IPF_GET_TIMESTAMP_INFO();

    memset(stamp_info, 0, sizeof(IPF_TIMESTAMP_INFO_S));
    return;
}

void ipf_dump_time_stamp(void)
{
    IPF_TIMESTAMP_INFO_S* stamp_info = IPF_GET_TIMESTAMP_INFO();
    unsigned int tmp = 0;
    int i;

    IPF_PRINT(" max diff:%u(%uus)\n",
              stamp_info->diff_max, stamp_info->diff_max*10/192);
    IPF_PRINT(" sum diff:%u(%uus)\n",
              stamp_info->diff_sum, stamp_info->diff_max/19);

    if (stamp_info->cnt_sum) {
        tmp = stamp_info->diff_sum / stamp_info->cnt_sum;
    }

    IPF_PRINT(" avg diff:%u(%uus)\n", tmp, tmp*10/192);

    for (i = 0; i < IPF_MAX_STAMP_ORDER; i++) {
        tmp = ipf_calc_percent(stamp_info->diff_order_cnt[i], stamp_info->cnt_sum);
        IPF_PRINT(" diff time (%u~%u) (%uus~%uus) count:%u (%u %%)\n",
            (0x80000000 >> (31-i)),
            (0xFFFFFFFF >> (31-i)),
            (0x80000000 >> (31-i))/19,
            (0xFFFFFFFF >> (31-i))/19,
            stamp_info->diff_order_cnt[i], tmp);
    }
    return;
}


/* IPF transfer time recorder end */


void ipf_write_basic_filter(u32 filter_hw_id, IPF_MATCH_INFO_S* match_infos)
{
    u32 j;
    u32 match_info;
    u32* match_info_addr = (u32*)match_infos;
    ipf_writel(filter_hw_id, HI_IPF_REGBASE_ADDR_VIRT + HI_BFLT_INDEX_OFFSET);
    for(j=0; j<(sizeof(IPF_MATCH_INFO_S)/4); j++)
    {
        match_info = *(match_info_addr+j);
        ipf_writel((match_info), (HI_IPF_REGBASE_ADDR_VIRT+HI_FLT_LOCAL_ADDR0_OFFSET+j*4)); 
    }
}

/*****************************************************************************
* �� �� ��      : ipf_init
*
* ��������  : IPF��ʼ��     �ڲ�ʹ�ã�����Ϊ�ӿں���
*
* �������  : BSP_VOID
* �������  : ��
* �� �� ֵ     : IPF_SUCCESS    ��ʼ���ɹ�
*                           IPF_ERROR      ��ʼ��ʧ��
*
* �޸ļ�¼  :2011��1��21��   ³��  ����
				 2013��4��30��    �¶����޸ģ����Ĵ������÷��䵽���� 
*****************************************************************************/
#ifdef __KERNEL__
BSP_S32 __init ipf_init(BSP_VOID)
#else
BSP_S32 ipf_init(BSP_VOID)
#endif
{
    BSP_U32 u32BDSize[IPF_CHANNEL_MAX] = {IPF_ULBD_DESC_SIZE, IPF_DLBD_DESC_SIZE};
    BSP_U32 u32RDSize[IPF_CHANNEL_MAX] = {IPF_ULRD_DESC_SIZE, IPF_DLRD_DESC_SIZE};
    BSP_U32 u32ADCtrl[IPF_CHANNEL_MAX] = {IPF_ADQ_DEFAULT_SEETING,IPF_ADQ_DEFAULT_SEETING};

#ifdef __VXWORKS__
    BSP_U32 u32IntMask0 = 0;
    BSP_U32 u32IntMask1 = 0;
    BSP_U32 u32ChanCtrl[IPF_CHANNEL_MAX] = {0,0};
    BSP_U32 u32IntStatus = 0;
    BSP_U32 u32Timeout = 0;
    BSP_U32 u32IntGate = 0;
    BSP_U32 u32DMAOutstanding = 3;	
#ifdef CONFIG_CCORE_PM
    BSP_U32 u32Result = 0;
#endif

#else
    BSP_S32 s32Ret = 0;
#endif

#if (defined(BSP_CONFIG_HI3630))
    unsigned long ipf_flags = 0;
#endif

#if !(defined(BSP_CONFIG_HI3630) && defined(__KERNEL__))
    /*ipf enable clock*/
    struct clk *c_IpfClk; 

    c_IpfClk = clk_get(NULL, "ipf_clk");
    if(IS_ERR(c_IpfClk))
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"ipf_init: ipf clk get failed.\n");
        return BSP_ERROR;
    }

    if(BSP_OK !=clk_enable(c_IpfClk))
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"ipf_init: ipf clk open failed.\n");
        return BSP_ERROR;
    }
#endif

#ifdef __VXWORKS__
    /* ��ʼ��ȫ�ֽṹ�� */
    memset((BSP_VOID*)IPF_ULBD_MEM_ADDR, 0x0, SHM_MEM_IPF_SIZE - IPF_DLDESC_SIZE);/* [false alarm]:fortify disable */
    
    g_stIpfUl.pstIpfBDQ = (IPF_BD_DESC_S*)IPF_ULBD_MEM_ADDR;
    g_stIpfUl.pstIpfRDQ = (IPF_RD_DESC_S*)IPF_ULRD_MEM_ADDR;
    g_stIpfUl.pstIpfADQ0 = (IPF_AD_DESC_S*)IPF_ULAD0_MEM_ADDR;
    g_stIpfUl.pstIpfADQ1 = (IPF_AD_DESC_S*)IPF_ULAD1_MEM_ADDR;
    g_stIpfUl.pu32IdleBd = (BSP_U32*)IPF_ULBD_IDLENUM_ADDR;
    *(BSP_U32*)IPF_ULBD_IDLENUM_ADDR = IPF_ULBD_DESC_SIZE;
    
    g_stIpfDl.pstIpfBDQ = (IPF_BD_DESC_S*)IPF_DLBD_MEM_ADDR;
    g_stIpfDl.pstIpfRDQ = (IPF_RD_DESC_S*)IPF_DLRD_MEM_ADDR;
    g_stIpfDl.pstIpfADQ0 = (IPF_AD_DESC_S*)IPF_DLAD0_MEM_ADDR;
    g_stIpfDl.pstIpfADQ1 = (IPF_AD_DESC_S*)IPF_DLAD1_MEM_ADDR;
    g_stIpfDl.pstIpfCDQ = (IPF_CD_DESC_S*)IPF_DLCD_MEM_ADDR;
    g_stIpfDl.u32IpfCdRptr = (BSP_U32*) IPF_DLCDRPTR_MEM_ADDR;
    *(g_stIpfDl.u32IpfCdRptr) = 0;
    #ifdef __BSP_IPF_DEBUG__
    g_stIpfDl.pstIpfDebugCDQ = (IPF_CD_DESC_S*)IPF_DEBUG_DLCD_ADDR;
    #endif
    g_stIpfDl.u32IdleBd = IPF_DLBD_DESC_SIZE;
	
    g_stIPFDebugInfo = (IPF_DEBUG_INFO_S*)IPF_DEBUG_INFO_ADDR;

    /* ������չ���������ڴ�, ������չ�������Ļ�ַ */
    g_pstExFilterAddr = (IPF_MATCH_INFO_S*)IPF_EXT_FILTER_ADDR;
    memset((BSP_VOID*)g_pstExFilterAddr, 0x0, EXFLITER_NUM*sizeof(IPF_MATCH_INFO_S));
    
    /* ������չ����������ʼ��ַ */	
    ipf_writel((BSP_U32)g_pstExFilterAddr, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_EF_BADDR_OFFSET);

    /* ��ʼ������������ */
    if(IPF_FilterList_Init() != IPF_SUCCESS)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF, "\r ipf_init malloc list ERROR! \n");
        goto FREE_ERROR;
    }
    
    /* ���ó�ʱ���üĴ�����ʹ�ܳ�ʱ�жϣ����ó�ʱʱ�� */
    u32Timeout = TIME_OUT_CFG | (TIME_OUT_ENABLE);
    ipf_writel(u32Timeout, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_TIME_OUT_OFFSET);

    /* ����ʱ���ſ����üĴ���������Ϊ�Զ��ſ�*/
    u32IntGate = GATE_CFG;
#if (defined(BSP_CONFIG_HI3630))
    ipf_writel(u32IntGate, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_GATE_OFFSET);/* [false alarm]:fortify disable */
#endif
    /* ��������ж��� */
    u32ChanCtrl[IPF_CHANNEL_UP] = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_CTRL_OFFSET);
    u32ChanCtrl[IPF_CHANNEL_UP] |= 0x30;
    ipf_writel(u32ChanCtrl[IPF_CHANNEL_UP], HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_CTRL_OFFSET);

    u32ChanCtrl[IPF_CHANNEL_DOWN] = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_CTRL_OFFSET);
    u32ChanCtrl[IPF_CHANNEL_DOWN] |= 0x30;
    ipf_writel(u32ChanCtrl[IPF_CHANNEL_DOWN], HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_CTRL_OFFSET);

    /* ��������ͨ����AD��BD��RD��� */
    ipf_writel(u32BDSize[IPF_CHANNEL_UP]-1, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_BDQ_SIZE_OFFSET);
    ipf_writel(u32RDSize[IPF_CHANNEL_UP]-1, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_RDQ_SIZE_OFFSET);
    ipf_writel(u32ADCtrl[IPF_CHANNEL_UP], HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ_CTRL_OFFSET);

    /* ����ͨ����BD��RD��ʼ��ַ*/    
    ipf_writel((BSP_U32)g_stIpfUl.pstIpfBDQ, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_BDQ_BADDR_OFFSET);
    ipf_writel((BSP_U32)g_stIpfUl.pstIpfRDQ, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_RDQ_BADDR_OFFSET);
    ipf_writel((BSP_U32)g_stIpfUl.pstIpfADQ0, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ0_BASE_OFFSET);
    ipf_writel((BSP_U32)g_stIpfUl.pstIpfADQ1, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ1_BASE_OFFSET);
	
    /* �����ж�����,ֻ��5���жϣ�����ϱ��������ʱ��RD���磬AD0��AD1�� */
    u32IntMask0 = IPF_INT_OPEN0;  /* 0��ΪModem CPU */
    u32IntMask1 = IPF_INT_OPEN1;
    ipf_writel(u32IntMask0, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_INT_MASK0_OFFSET);
    ipf_writel(u32IntMask1, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_INT_MASK1_OFFSET);

    /* ���ж� */
    u32IntStatus = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_INT_STATE_OFFSET);
    ipf_writel(u32IntStatus, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_INT_STATE_OFFSET);
    /*��DMAͨ����Outstanding��д*/
    ipf_writel(u32DMAOutstanding, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_DMA_CTRL1_OFFSET);

    /*��ʼ��ipf����������������*/
    spin_lock_init(&ipf_filter_spinlock);
	
    IPF_Int_Connect();
	
#ifdef CONFIG_CCORE_PM

    u32Result = bsp_device_pm_add(&ipf_dpm_device);
    if(OK!= u32Result)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r ipf_dpm_device add error\n");
        return IPF_ERROR;
    }
#if (defined(BSP_CONFIG_HI3630))
    ipf_acore_init_status = (u32*)IPF_INIT_ADDR;
    ipf_ccore_init_status = (u32*)(IPF_INIT_ADDR+4);
	
    spin_lock_irqsave(&ipf_filter_spinlock, ipf_flags);
    bsp_ipc_spin_lock(IPC_SEM_IPF_PWCTRL);
    *ipf_ccore_init_status = IPF_MCORE_INIT_SUCCESS;
    bsp_ipc_spin_unlock(IPC_SEM_IPF_PWCTRL);
    spin_unlock_irqrestore(&ipf_filter_spinlock, ipf_flags);
#endif

#ifdef CONFIG_BALONG_MODEM_RESET
    modem_reset_flag = (IPF_FORREST_CONTROL_E*)(IPF_INIT_ADDR+8);
#endif

#endif

    g_IPFInit = IPF_MCORE_INIT_SUCCESS;

    bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"[IPF] ipf init success\n");
	
    return IPF_SUCCESS;
    
    FREE_ERROR:
    bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r ipf_init malloc ERROR! \n");
    return IPF_ERROR;
	
#else

    memset((BSP_VOID*)IPF_DLBD_MEM_ADDR, 0x0, IPF_DLDESC_SIZE);
	
    g_stIpfUl.pstIpfBDQ = (IPF_BD_DESC_S*)IPF_ULBD_MEM_ADDR;
    g_stIpfUl.pstIpfRDQ = (IPF_RD_DESC_S*)IPF_ULRD_MEM_ADDR;
    g_stIpfUl.pstIpfADQ0 = (IPF_AD_DESC_S*)IPF_ULAD0_MEM_ADDR;
    g_stIpfUl.pstIpfADQ1 = (IPF_AD_DESC_S*)IPF_ULAD1_MEM_ADDR;
    g_stIpfUl.pu32IdleBd = (BSP_U32*)IPF_ULBD_IDLENUM_ADDR;

    g_stIpfDl.pstIpfBDQ = (IPF_BD_DESC_S*)IPF_DLBD_MEM_ADDR;
    g_stIpfDl.pstIpfRDQ = (IPF_RD_DESC_S*)IPF_DLRD_MEM_ADDR;
    g_stIpfDl.pstIpfADQ0 = (IPF_AD_DESC_S*)IPF_DLAD0_MEM_ADDR;
    g_stIpfDl.pstIpfADQ1 = (IPF_AD_DESC_S*)IPF_DLAD1_MEM_ADDR;
    g_stIpfDl.pstIpfCDQ = (IPF_CD_DESC_S*)IPF_DLCD_MEM_ADDR;

    g_stIpfDl.pstIpfPhyBDQ = (IPF_BD_DESC_S*)(IPF_IO_ADDRESS_PHY(IPF_DLBD_MEM_ADDR));
    g_stIpfDl.pstIpfPhyRDQ = (IPF_RD_DESC_S*)(IPF_IO_ADDRESS_PHY(IPF_DLRD_MEM_ADDR));
    g_stIpfDl.pstIpfPhyADQ0 = (IPF_AD_DESC_S*)(IPF_IO_ADDRESS_PHY(IPF_DLAD0_MEM_ADDR));
    g_stIpfDl.pstIpfPhyADQ1 = (IPF_AD_DESC_S*)(IPF_IO_ADDRESS_PHY(IPF_DLAD1_MEM_ADDR));
	
    g_stIpfDl.u32IpfCdRptr = (BSP_U32*) IPF_DLCDRPTR_MEM_ADDR;
    *(g_stIpfDl.u32IpfCdRptr) = 0;

    #if (defined(BSP_CONFIG_HI3630))
    spin_lock_init(&ipf_filter_spinlock);
    #endif
	
    #ifdef __BSP_IPF_DEBUG__
    g_stIpfDl.pstIpfDebugCDQ = (IPF_CD_DESC_S*)IPF_DEBUG_DLCD_ADDR;
    #endif

    g_stIPFDebugInfo = (IPF_DEBUG_INFO_S*)IPF_DEBUG_INFO_ADDR;
	
    /* ��������ͨ����AD��BD��RD��� */
    ipf_writel(u32BDSize[IPF_CHANNEL_DOWN]-1, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_BDQ_SIZE_OFFSET);
    ipf_writel(u32RDSize[IPF_CHANNEL_DOWN]-1, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_RDQ_SIZE_OFFSET);
    ipf_writel(u32ADCtrl[IPF_CHANNEL_DOWN], HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ_CTRL_OFFSET);

    /*����ͨ����BD��RD��ʼ��ַ*/    
    ipf_writel((BSP_U32)g_stIpfDl.pstIpfPhyBDQ, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_BDQ_BADDR_OFFSET);
    ipf_writel((BSP_U32)g_stIpfDl.pstIpfPhyRDQ, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_RDQ_BADDR_OFFSET);
    ipf_writel((BSP_U32)g_stIpfDl.pstIpfPhyADQ0, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ0_BASE_OFFSET);
    ipf_writel((BSP_U32)g_stIpfDl.pstIpfPhyADQ1, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ1_BASE_OFFSET);

    IPF_Int_Connect();

    s32Ret = platform_device_register(&balong_device_ipf);
    if(s32Ret)
    {

        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,
                        "Platform device register failed! \n");

        return s32Ret;
    }
    s32Ret = platform_driver_register(&balong_driver_ipf);
    if(s32Ret)
    {

        platform_device_unregister(&balong_device_ipf);

        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,
                        "Platform driver register failed! \n");
        return s32Ret;
    }
#if (defined(BSP_CONFIG_HI3630))
    ipf_acore_init_status = (u32*)IPF_INIT_ADDR;
    ipf_ccore_init_status = (u32*)(IPF_INIT_ADDR+4);

    spin_lock_irqsave(&ipf_filter_spinlock, ipf_flags);
    bsp_ipc_spin_lock(IPC_SEM_IPF_PWCTRL);
    *ipf_acore_init_status = IPF_ACORE_INIT_SUCCESS;
    cache_sync();
    bsp_ipc_spin_unlock(IPC_SEM_IPF_PWCTRL);
    spin_unlock_irqrestore(&ipf_filter_spinlock, ipf_flags);
#endif

#ifdef CONFIG_BALONG_MODEM_RESET
    modem_reset_flag = (IPF_FORREST_CONTROL_E*)(IPF_INIT_ADDR+8);
    *modem_reset_flag = IPF_FORRESET_CONTROL_ALLOW;

    if(0 != DRV_CCORERESET_REGCBFUNC("IPF_BALONG",bsp_ipf_reset_ccore_cb, 0, DRV_RESET_CB_PIOR_IPF))
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"ipf set modem reset call back func fail!\n");
    }
#endif

    g_IPFInit = IPF_ACORE_INIT_SUCCESS;

    bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"[IPF]  ipf init success\n");

    return s32Ret;
#endif
}

/*****************************************************************************
* �� �� ��     : IPF_Int_Connect
*
* ��������  : ��IPF�жϴ�����(���˶��ṩ)
*
* �������  : BSP_VOID
* �������  : ��
* �� �� ֵ      : ��
*
* �޸ļ�¼  :2011��12��2��   ³��  ����
*****************************************************************************/
BSP_VOID IPF_Int_Connect(BSP_VOID)
{
	BSP_S32 s32Result;
#ifdef CONFIG_MODULE_VIC
	bsp_vic_connect((int)INT_LVL_IPF, (vicfuncptr)IPF_IntHandler, (s32)0);    
	bsp_vic_enable(INT_LVL_IPF);

#else 
	s32Result = request_irq(INT_LVL_IPF, (irq_handler_t)IPF_IntHandler, IRQF_NO_SUSPEND, "balong_ipf_v700r200", NULL);
	if(0 != s32Result)
	{
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF_Int_Connect Failed! \n",0,0,0,0,0,0);
	}
#endif
}

/*****************************************************************************
* �� �� ��  : IPF_IntHandler
*
* ��������  : IPF�жϴ�����
*
* �������  : ��
* �������  : ��
* �� �� ֵ  : ��
*
* �޸ļ�¼  :2011��1��24��   ³��  ����


��Ҫ�Ķ�������ADQ���жεĴ�������δ��
*****************************************************************************/
#ifdef __KERNEL__
irqreturn_t  IPF_IntHandler (int irq, void* dev)
#else
BSP_VOID IPF_IntHandler(BSP_VOID)
#endif
{
    BSP_U32 u32IpfInt = 0;

    /* ��ȡ�ж�״̬ */
#ifdef __VXWORKS__ 

    u32IpfInt = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_INT0_OFFSET); 

    /* ���н���ϱ��ͽ����ʱ�ϱ� */
    if(u32IpfInt & (IPF_UL_RPT_INT0|IPF_UL_TIMEOUT_INT0))
    {
        ipf_writel((IPF_UL_RPT_INT0|IPF_UL_TIMEOUT_INT0), HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_INT_STATE_OFFSET);
	
        g_stIPFDebugInfo->ipf_ulbd_done_count++;

        /* ����ps�������� */
        if(g_stIpfUl.pFnUlIntCb != NULL)
        {
            (BSP_VOID)g_stIpfUl.pFnUlIntCb();
        }
        else
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,
                        "\r IPF_IntHandler ULTASK NULL! \n",0,0,0,0,0,0);
        }
    }

    /* ����ADQ0��ADQ1����ָʾ */	
    if((IPF_UL_ADQ0_EPTY_INT0 | IPF_UL_ADQ1_EPTY_INT0) == (u32IpfInt & (IPF_UL_ADQ0_EPTY_INT0 | IPF_UL_ADQ1_EPTY_INT0)))
    {
        g_stIPFDebugInfo->u32UlAdq0Overflow++;
        g_stIPFDebugInfo->u32UlAdq1Overflow++;

        ipf_writel((IPF_UL_ADQ0_EPTY_INT0 | IPF_UL_ADQ1_EPTY_INT0), HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_INT_STATE_OFFSET);
        if(g_stIpfUl.pAdqEmptyUlCb != NULL)
        {
            (BSP_VOID)g_stIpfUl.pAdqEmptyUlCb(IPF_EMPTY_ADQ);  
            return;
        }
        else
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF_IntHandler ULADQEMPTY NULL! \n",0,0,0,0,0,0);
        }
    }

	
    /* ����ADQ0��ָʾ */
    if(u32IpfInt & IPF_UL_ADQ0_EPTY_INT0)
    {
        g_stIPFDebugInfo->u32UlAdq0Overflow++;
        ipf_writel(IPF_UL_ADQ0_EPTY_INT0, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_INT_STATE_OFFSET);

        if(g_stIpfUl.pAdqEmptyUlCb != NULL)
        {
            (BSP_VOID)g_stIpfUl.pAdqEmptyUlCb(IPF_EMPTY_ADQ0);  
        }
        else
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF_IntHandler ULADQEMPTY NULL! \n",0,0,0,0,0,0);
        }
    }
    /* ����ADQ1��ָʾ */	
    if(u32IpfInt & IPF_UL_ADQ1_EPTY_INT0)
    {
       g_stIPFDebugInfo->u32UlAdq1Overflow++;
       ipf_writel(IPF_UL_ADQ1_EPTY_INT0, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_INT_STATE_OFFSET);
       if(g_stIpfUl.pAdqEmptyUlCb != NULL)
       {
           (BSP_VOID)g_stIpfUl.pAdqEmptyUlCb(IPF_EMPTY_ADQ1);  
       }
       else
       {
           bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF_IntHandler ULADQEMPTY NULL! \n",0,0,0,0,0,0);
       }
    }

#else

    u32IpfInt = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_INT1_OFFSET); 

    /* ���н���ϱ��ͽ����ʱ�ϱ� */
    if(u32IpfInt&(IPF_DL_RPT_INT1|IPF_DL_TIMEOUT_INT1))
    {
        ipf_writel((IPF_DL_RPT_INT1|IPF_DL_TIMEOUT_INT1), HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_INT_STATE_OFFSET);

        g_stIPFDebugInfo->ipf_dlbd_done_count++;

        /* ����ps�������� */
        if(g_stIpfDl.pFnDlIntCb != NULL)
        {
            (BSP_VOID)g_stIpfDl.pFnDlIntCb();  
        }
        else
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF_IntHandler DLTASK NULL! \n",0,0,0,0,0,0);
	 }
    } 
     /* ����ADQ0��ADQ1����ָʾ */	
    if((IPF_DL_ADQ0_EPTY_INT1 | IPF_DL_ADQ1_EPTY_INT1) == (u32IpfInt & (IPF_DL_ADQ0_EPTY_INT1 | IPF_DL_ADQ1_EPTY_INT1)))
    {
        g_stIPFDebugInfo->u32DlAdq0Overflow++;
        g_stIPFDebugInfo->u32DlAdq1Overflow++;
        ipf_writel((IPF_DL_ADQ0_EPTY_INT1 | IPF_DL_ADQ1_EPTY_INT1), HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_INT_STATE_OFFSET);
        if(g_stIpfDl.pAdqEmptyDlCb != NULL)
        {
            (BSP_VOID)g_stIpfDl.pAdqEmptyDlCb(IPF_EMPTY_ADQ);  
        }
        else
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF_IntHandler DLADQEMPTY NULL! \n",0,0,0,0,0,0);
        }
        return IRQ_HANDLED;
    }

    /* ����ADQ0��ָʾ */
    if(u32IpfInt & IPF_DL_ADQ0_EPTY_INT1)
    {
        g_stIPFDebugInfo->u32DlAdq0Overflow++;
        ipf_writel(IPF_DL_ADQ0_EPTY_INT1, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_INT_STATE_OFFSET);
        if(g_stIpfDl.pAdqEmptyDlCb != NULL)
        {
            (BSP_VOID)g_stIpfDl.pAdqEmptyDlCb(IPF_EMPTY_ADQ0);  
        }
        else
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF_IntHandler DLADQEMPTY NULL! \n",0,0,0,0,0,0);
        }
    }
	
    /* ����ADQ1��ָʾ */	
    if(u32IpfInt & IPF_DL_ADQ1_EPTY_INT1)
    {
        g_stIPFDebugInfo->u32DlAdq1Overflow++;
        ipf_writel(IPF_DL_ADQ1_EPTY_INT1, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_INT_STATE_OFFSET);
        if(g_stIpfDl.pAdqEmptyDlCb != NULL)
        {
            (BSP_VOID)g_stIpfDl.pAdqEmptyDlCb(IPF_EMPTY_ADQ1);  
        }
        else
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF_IntHandler DLADQEMPTY NULL! \n",0,0,0,0,0,0);
        }
    }
    return IRQ_HANDLED;
#endif
}/*lint !e550*/


#if (defined(BSP_CONFIG_HI3630))
/*����K3V3����������⣬ipf�޷���M3�ϻָ�ipf�Ĵ��������ƶ���acore��ccore*/

/*****************************************************************************
* �� �� ��       : ipf_drx_restore_filter
*
* ��������  : �ṩ�ĵ͹��Ľӿڣ�����IPF�ϵ�ָ�
*
* �������  : ��
*             
* �������  : ��
* �� �� ֵ     : ��
*
* ע��:�ú�����Ҫ�����жϵ�����µ���
* �޸ļ�¼  : 2013��4��23��v1.00 chendongyue ����
			2014��1��23��v1.01 chendongyue ��ֲ��A9core
*****************************************************************************/
s32 ipf_drx_restore_filter(void)
{
    u32 i,j,k,filter_serial;
    IPF_MATCH_INFO_S* pstMatchInfo;
    u32 match_info;
    u32* match_info_addr = (u32 *)(IPF_PWRCTL_BASIC_FILTER_ADDR);

    for(i=0 ; i < IPF_MODEM_MAX; i++)
    {
        match_info_addr = (u32 *)(IPF_PWRCTL_BASIC_FILTER_ADDR + i*sizeof(IPF_MATCH_INFO_S));
        pstMatchInfo = (IPF_MATCH_INFO_S*)match_info_addr;
        filter_serial = i;
        k = 0;
        do
        {
            if(filter_serial < IPF_BF_NUM)
            {
                ipf_writel(filter_serial, HI_IPF_REGBASE_ADDR_VIRT + HI_BFLT_INDEX_OFFSET); 
                for(j=0; j<(sizeof(IPF_MATCH_INFO_S)/4); j++)
                {
                        match_info = *(match_info_addr+j);
                        ipf_writel((match_info), (HI_IPF_REGBASE_ADDR_VIRT+HI_FLT_LOCAL_ADDR0_OFFSET+j*4)); 
                }
                k++;
            }
            filter_serial = pstMatchInfo->unFltChain.Bits.u16NextIndex;
            pstMatchInfo = (IPF_MATCH_INFO_S*)(IPF_PWRCTL_BASIC_FILTER_ADDR + filter_serial*sizeof(IPF_MATCH_INFO_S));
            match_info_addr = (u32 *)pstMatchInfo;
            if(k >= IPF_BF_NUM)
            {
                break;
            }
        }while(filter_serial != IPF_TAIL_INDEX);
    }
    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��       : ipf_check_filter_restore
*
* ��������  : �ṩ�ĵ͹��Ľӿڣ�����IPF�ϵ�ָ�
*
* �������  : ��
*             
* �������  : ��
* �� �� ֵ     : ��
*
* 
* �޸ļ�¼  : 2014��1��23��v1.00 chendongyue����
*****************************************************************************/
void ipf_check_filter_restore(void)
{
	unsigned long ipf_flags = 0;
	u32 ipf_init_status = 0;
	spin_lock_irqsave(&ipf_filter_spinlock, ipf_flags);
	bsp_ipc_spin_lock(IPC_SEM_IPF_PWCTRL);

	ipf_init_status = (*ipf_acore_init_status)|(*ipf_ccore_init_status);
#ifdef __VXWORKS__
	*ipf_ccore_init_status = IPF_MCORE_INIT_SUCCESS;
#endif

#ifdef __KERNEL__
	*ipf_acore_init_status = IPF_ACORE_INIT_SUCCESS;
#endif
	cache_sync();

	if(IPF_PWC_DOWN != ipf_init_status)
	{
		bsp_ipc_spin_unlock(IPC_SEM_IPF_PWCTRL);
		spin_unlock_irqrestore(&ipf_filter_spinlock, ipf_flags);
		return;
	}
	else
	{
		ipf_drx_restore_filter();
		bsp_ipc_spin_unlock(IPC_SEM_IPF_PWCTRL);
		spin_unlock_irqrestore(&ipf_filter_spinlock, ipf_flags);
		return;
	}
}

#endif

/*****************************************************************************
* �� �� ��      : BSP_IPF_ConfigTimeout
*
* ��������  : ����ʹ�ã����ó�ʱʱ��ӿ�
*
* �������  : BSP_U32 u32Timeout ���õĳ�ʱʱ��
* �������  : ��
* �� �� ֵ     : IPF_SUCCESS    �ɹ�
*                           BSP_ERR_IPF_INVALID_PARA      ������Ч
*
* ˵��              : 1����256��ʱ������
*
* �޸ļ�¼   : 2011��11��30��   ³��  ����
*****************************************************************************/
BSP_S32 BSP_IPF_ConfigTimeout(BSP_U32 u32Timeout)
{
    if((u32Timeout == 0) || (u32Timeout > 0xFFFF))
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigTimeout u32Timeout ERROR! \n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    u32Timeout |= TIME_OUT_ENABLE;
    ipf_writel(u32Timeout, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_TIME_OUT_OFFSET);
    
    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��      : BSP_IPF_PktLen
*
* ��������  : �ýӿ��������ù�������������С����
*
* �������  : BSP_U32 MaxLen   ������
*                           BSP_U32 MinLen   ��С����
*
* �������   : ��
* �� �� ֵ      : IPF_SUCCESS                �ɹ�
*                           BSP_ERR_IPF_INVALID_PARA   ��������(����������С����С)
*
* �޸ļ�¼  :2011��2��17��   ³��  ����
*****************************************************************************/
BSP_S32 BSP_IPF_SetPktLen(BSP_U32 u32MaxLen, BSP_U32 u32MinLen)
{
    BSP_U32 u32PktLen = 0;

    /* ������� */
    if(u32MaxLen < u32MinLen)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF, "\r BSP_IPF_PktLen input error! \n");
        return BSP_ERR_IPF_INVALID_PARA;
    }
    
    u32PktLen = ((u32MaxLen&0x3FFF)<<16) | (u32MinLen&0x3FFF);
    
    ipf_writel(u32PktLen, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_PKT_LEN_OFFSET);
    return IPF_SUCCESS;
}/*lint !e550*/

/**********************************��ά�ɲ�ӿں���************************************/
BSP_VOID BSP_IPF_Help(BSP_VOID)
{
    IPF_PRINT("===============================================\n");
    IPF_PRINT("BSP_IPF_Info    ����1:ͨ������  0Ϊ���У�1Ϊ����\n");
    IPF_PRINT("BSP_IPF_Shared_DDR_Info \n");
    IPF_PRINT("BSP_IPF_BDInfo  ����1:ͨ������  ����2:BDָ��\n");
    IPF_PRINT("BSP_IPF_RDInfo  ����1:ͨ������  ����2:RDָ��\n"); 
    IPF_PRINT("BSP_IPF_ADInfo  ����1:ͨ������  ����2:ADָ�����3:AD ��������0Ϊ��,1Ϊ��\n"); 
    IPF_PRINT("===============================================\n");
    IPF_PRINT("BSP_IPF_Dump_BDInfo  ����1:ͨ������\n");
    IPF_PRINT("BSP_IPF_Dump_RDInfo  ����1:ͨ������\n"); 
    IPF_PRINT("BSP_IPF_Dump_ADInfo  ����1:ͨ������\n"); 
    IPF_PRINT("===============================================\n");
    IPF_PRINT("ipf_enable_ul_time_stamp  ����1:0-disable, 1-enable\n");
    IPF_PRINT("ipf_enable_dl_time_stamp  ����1:0-disable, 1-enable\n");
    IPF_PRINT("ipf_clear_time_stamp  ���ʵ�ʴ���¼\n");
    IPF_PRINT("ipf_dump_time_stamp  Linux:����ʱ���, vxWorks:����ʱ���\n");

#ifdef __VXWORKS__    
    IPF_PRINT("===============================================\n");
    IPF_PRINT("BSP_IPF_UseFilterInfo  ����1:ͨ������\n"); 
    IPF_PRINT("BSP_IPF_FreeFilterInfo\n"); 
    IPF_PRINT("BSP_IPF_FilterInfoHWID  ����1:Ӳ��Filter ID\n");
    IPF_PRINT("BSP_IPF_FilterInfoPSID  ����1 :PS Filter ID\n");
#endif
    IPF_PRINT("===============================================\n");
    IPF_PRINT("��������BD,BD�����ô���(��������GetDescʱ��Ч):            %d\n",g_stIPFDebugInfo->u32UlBdNotEnough);
    IPF_PRINT("��������BD,AD0�����ô���(��������GetDescʱ��Ч):            %d\n",g_stIPFDebugInfo->u32UlAd0NotEnough);
    IPF_PRINT("��������BD,AD1�����ô���(��������GetDescʱ��Ч):            %d\n",g_stIPFDebugInfo->u32UlAd1NotEnough);
    IPF_PRINT("��������BD,BD�����ô���(��������GetDescʱ��Ч):            %d\n",g_stIPFDebugInfo->u32DlBdNotEnough);
    IPF_PRINT("��������BD,AD0�����ô���(��������GetDescʱ��Ч):            %d\n",g_stIPFDebugInfo->u32DlAd0NotEnough);
    IPF_PRINT("��������BD,AD1�����ô���(��������GetDescʱ��Ч):            %d\n",g_stIPFDebugInfo->u32DlAd1NotEnough);
    IPF_PRINT("��������CD,CD�����ô���:            %d\n",g_stIPFDebugInfo->u32DlCdNotEnough);
    IPF_PRINT("�ж��ϱ�����BD�����������:         %d\n",g_stIPFDebugInfo->u32UlBdqOverflow);
    IPF_PRINT("�ж��ϱ�����BD�����������:         %d\n",g_stIPFDebugInfo->u32DlBdqOverflow);
    IPF_PRINT("===============================================\n");
    IPF_PRINT("���з����BD��������Ϊ0����:            %u\n",g_stIPFDebugInfo->ipf_ulbd_len_zero_count);
    IPF_PRINT("���з����AD0���ô������:            %u\n",g_stIPFDebugInfo->ipf_ulbd_len_zero_count);
    IPF_PRINT("���з����AD1���ô������:            %u\n",g_stIPFDebugInfo->ipf_ulbd_len_zero_count);
    IPF_PRINT("���з����BD��������Ϊ0����:            %u\n",g_stIPFDebugInfo->ipf_dlbd_len_zero_count);
    IPF_PRINT("���з����AD0���ô������:            %u\n",g_stIPFDebugInfo->ipf_dlbd_len_zero_count);
    IPF_PRINT("���з����AD1���ô������:            %u\n",g_stIPFDebugInfo->ipf_dlbd_len_zero_count);
    IPF_PRINT("acore������ccore��λʱ��������:            %u\n",g_stIPFDebugInfo->ipf_acore_not_init_count);
    IPF_PRINT("ccore������ccore��λʱ��������:            %u\n",g_stIPFDebugInfo->ipf_ccore_not_init_count);
    IPF_PRINT("===============================================\n");
    IPF_PRINT("����ʱ�������ʹ��:                  %u\n",g_stIPFDebugInfo->ipf_timestamp_ul_en);
    IPF_PRINT("����BD���ô���:                      %u\n",g_stIPFDebugInfo->ipf_cfg_ulbd_count);
    IPF_PRINT("����BD����жϴ���:                  %u\n",g_stIPFDebugInfo->ipf_ulbd_done_count);
    IPF_PRINT("����RD��ȡ����:                      %u\n",g_stIPFDebugInfo->ipf_get_ulrd_count);
    IPF_PRINT("����AD0���ô���:                     %u\n",g_stIPFDebugInfo->ipf_cfg_ulad0_count);
    IPF_PRINT("����AD1���ô���:                     %u\n",g_stIPFDebugInfo->ipf_cfg_ulad1_count);
    IPF_PRINT("CCore suspend����:                   %u\n",g_stIPFDebugInfo->ipf_ccore_suspend_count);
    IPF_PRINT("CCore resume����:                    %u\n",g_stIPFDebugInfo->ipf_ccore_resume_count);
    IPF_PRINT("===============================================\n");
    IPF_PRINT("����ʱ�������ʹ��:                  %u\n",g_stIPFDebugInfo->ipf_timestamp_dl_en);
    IPF_PRINT("����BD���ô���:                      %u\n",g_stIPFDebugInfo->ipf_cfg_dlbd_count);
    IPF_PRINT("����BD����жϴ���:                  %u\n",g_stIPFDebugInfo->ipf_dlbd_done_count);
    IPF_PRINT("����RD��ȡ����:                      %u\n",g_stIPFDebugInfo->ipf_get_dlrd_count);
    IPF_PRINT("����AD0���ô���:                     %u\n",g_stIPFDebugInfo->ipf_cfg_dlad0_count);
    IPF_PRINT("����AD1���ô���:                     %u\n",g_stIPFDebugInfo->ipf_cfg_dlad1_count);
    IPF_PRINT("ACore suspend����:                   %u\n",g_stIPFDebugInfo->ipf_acore_suspend_count);
    IPF_PRINT("ACore resume����:                    %u\n",g_stIPFDebugInfo->ipf_acore_resume_count);
}

BSP_S32 BSP_IPF_Shared_DDR_Info(BSP_VOID)
{
/*	BSP_U32* pIPFInit = (BSP_U32*)IPF_INIT_ADDR;*/
	BSP_U32 ipf_Shared_ddr_start = SHM_MEM_IPF_ADDR;
	
	BSP_U32 ipf_Shared_ddr_ul_start = IPF_ULBD_MEM_ADDR;

	BSP_U32 ipf_Shared_ddr_filter_pwc_start = IPF_PWRCTL_BASIC_FILTER_ADDR;

	BSP_U32 ipf_Shared_ddr_pwc_info_start = IPF_PWRCTL_INFO_ADDR;

	BSP_U32 ipf_Shared_ddr_dlcdrptr = IPF_DLCDRPTR_MEM_ADDR;

	BSP_U32 ipf_Shared_ddr_debug_dlcd_start = IPF_DEBUG_DLCD_ADDR;
	BSP_U32 ipf_Shared_ddr_debug_dlcd_size = IPF_DEBUG_DLCD_SIZE;
	BSP_U32 ipf_Shared_ddr_end = IPF_DEBUG_INFO_END_ADDR;
	BSP_U32 ipf_Shared_ddr_len = IPF_DEBUG_INFO_END_ADDR - SHM_MEM_IPF_ADDR;
	
	IPF_PRINT("ipf_Shared_ddr_start                    value is 0x%x \n", ipf_Shared_ddr_start);
	IPF_PRINT("ipf_Shared_ddr_ul_start                value is 0x%x \n", ipf_Shared_ddr_ul_start);
	IPF_PRINT("ipf_Shared_ddr_filter_pwc_start     value is 0x%x \n", ipf_Shared_ddr_filter_pwc_start);
	IPF_PRINT("ipf_Shared_ddr_pwc_info_start      value is 0x%x \n", ipf_Shared_ddr_pwc_info_start);
	IPF_PRINT("ipf_Shared_ddr_dlcdrptr                value is 0x%x \n", ipf_Shared_ddr_dlcdrptr);
	IPF_PRINT("ipf_Shared_ddr_debug_dlcd_start   value is 0x%x \n", ipf_Shared_ddr_debug_dlcd_start);
	IPF_PRINT("ipf_Shared_ddr_debug_dlcd_size    value is 0x%x \n", ipf_Shared_ddr_debug_dlcd_size);
	IPF_PRINT("ipf_Shared_ddr_end                     value is 0x%x \n", ipf_Shared_ddr_end);
	IPF_PRINT("ipf_Shared_ddr_len                     value is 0x%x (Max len is 0x10000)\n", ipf_Shared_ddr_len);
/*	if(IPF_INIT_SUCCESS != (*pIPFInit))
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF Not Init! \n");
		IPF_PRINT("g_pIPFInit value is 0x%x \n", (*pIPFInit));
		return BSP_ERR_IPF_NOT_INIT;
	}
*/
	return IPF_SUCCESS;
}

BSP_S32 BSP_IPF_BDInfo(IPF_CHANNEL_TYPE_E eChnType, BSP_U32 u32BdqPtr)
{
#ifdef __BSP_IPF_CD_DEBUG__
    BSP_U32 u32CdPtr;
#endif
      
    switch(eChnType)
    {
        case IPF_CHANNEL_UP:
                if(u32BdqPtr >= IPF_ULBD_DESC_SIZE)
                {
                    return IPF_ERROR;
                }
                IPF_PRINT("==========BD Info=========\n");
                IPF_PRINT("BDλ��:         %d\n",u32BdqPtr);
                IPF_PRINT("u16Attribute:   %d\n",g_stIpfUl.pstIpfBDQ[u32BdqPtr].u16Attribute);
                IPF_PRINT("u16PktLen:      %d\n",g_stIpfUl.pstIpfBDQ[u32BdqPtr].u16PktLen);
                IPF_PRINT("u32InPtr:       0x%x\n",g_stIpfUl.pstIpfBDQ[u32BdqPtr].u32InPtr);
                IPF_PRINT("u32OutPtr:      0x%x\n",g_stIpfUl.pstIpfBDQ[u32BdqPtr].u32OutPtr);
                IPF_PRINT("u16Resv:        %d\n",g_stIpfUl.pstIpfBDQ[u32BdqPtr].u16Resv);
                IPF_PRINT("u16UsrField1:   %d\n",g_stIpfUl.pstIpfBDQ[u32BdqPtr].u16UsrField1);
                IPF_PRINT("u32UsrField2:   0x%x\n",g_stIpfUl.pstIpfBDQ[u32BdqPtr].u32UsrField2);
                IPF_PRINT("u32UsrField3:   0x%x\n",g_stIpfUl.pstIpfBDQ[u32BdqPtr].u32UsrField3);
            break;
       case IPF_CHANNEL_DOWN:
                if(u32BdqPtr >= IPF_DLBD_DESC_SIZE)
                {
                    return IPF_ERROR;
                }
                IPF_PRINT("==========BD Info=========\n");
                IPF_PRINT("BDλ��:         %d\n",u32BdqPtr);
                IPF_PRINT("u16Attribute:   %d\n",g_stIpfDl.pstIpfBDQ[u32BdqPtr].u16Attribute);
                IPF_PRINT("u16PktLen:      %d\n",g_stIpfDl.pstIpfBDQ[u32BdqPtr].u16PktLen);
                IPF_PRINT("u32InPtr:       0x%x\n",g_stIpfDl.pstIpfBDQ[u32BdqPtr].u32InPtr);
                IPF_PRINT("u32OutPtr:      0x%x\n",g_stIpfDl.pstIpfBDQ[u32BdqPtr].u32OutPtr);
                IPF_PRINT("u16Resv:        %d\n",g_stIpfDl.pstIpfBDQ[u32BdqPtr].u16Resv);
                IPF_PRINT("u16UsrField1:   %d\n",g_stIpfDl.pstIpfBDQ[u32BdqPtr].u16UsrField1);
                IPF_PRINT("u32UsrField2:   0x%x\n",g_stIpfDl.pstIpfBDQ[u32BdqPtr].u32UsrField2);
                IPF_PRINT("u32UsrField3:   0x%x\n",g_stIpfDl.pstIpfBDQ[u32BdqPtr].u32UsrField3);
                #ifdef __BSP_IPF_CD_DEBUG__
                #ifdef __VXWORKS__
                u32CdPtr = g_stIpfDl.pstIpfBDQ[u32BdqPtr].u32InPtr-(BSP_U32)g_stIpfDl.pstIpfCDQ;
                #else
                u32CdPtr = IO_ADDRESS(g_stIpfDl.pstIpfBDQ[u32BdqPtr].u32InPtr)-(BSP_U32)g_stIpfDl.pstIpfCDQ;
                #endif
                u32CdPtr = u32CdPtr/sizeof(IPF_CD_DESC_S);
                while(g_stIpfDl.pstIpfDebugCDQ[u32CdPtr].u16Attribute != 1)
                {
                    IPF_PRINT("==========CD Info=========\n");
                    IPF_PRINT("CDλ��:             %d\n",u32CdPtr);
                    IPF_PRINT("u16Attribute:       %d\n",g_stIpfDl.pstIpfDebugCDQ[u32CdPtr].u16Attribute);
                    IPF_PRINT("u16PktLen:          %d\n",g_stIpfDl.pstIpfDebugCDQ[u32CdPtr].u16PktLen);
                    IPF_PRINT("u32Ptr:             0x%x\n",g_stIpfDl.pstIpfDebugCDQ[u32CdPtr].u32Ptr);                
                    u32CdPtr = ((u32CdPtr+1) < IPF_DLCD_DESC_SIZE)?(u32CdPtr+1):0;
                };
                IPF_PRINT("==========CD Info=========\n");
                IPF_PRINT("CDλ��:             %d\n",u32CdPtr);
                IPF_PRINT("u16Attribute:       %d\n",g_stIpfDl.pstIpfDebugCDQ[u32CdPtr].u16Attribute);
                IPF_PRINT("u16PktLen:          %d\n",g_stIpfDl.pstIpfDebugCDQ[u32CdPtr].u16PktLen);
                IPF_PRINT("u32Ptr:             0x%x\n",g_stIpfDl.pstIpfDebugCDQ[u32CdPtr].u32Ptr);  
                #endif
            break; 
        default:
            break;
    }
    IPF_PRINT("************************\n");
    return 0;
}

BSP_S32 BSP_IPF_Dump_BDInfo(IPF_CHANNEL_TYPE_E eChnType)
{
    BSP_U32 i;
    switch(eChnType)
    {
        case IPF_CHANNEL_UP:
			
            for(i = 0;i < IPF_ULBD_DESC_SIZE;i++)
            {
                BSP_IPF_BDInfo(IPF_CHANNEL_UP,i);
            }
        break;
		
        case IPF_CHANNEL_DOWN:

            for(i = 0;i < IPF_DLBD_DESC_SIZE;i++)
            {
                BSP_IPF_BDInfo(IPF_CHANNEL_DOWN,i);
            }
        break;
				
        default:
        IPF_PRINT("Input param invalid ! \n");
        break;

    }
    return 0;
}

BSP_S32 BSP_IPF_RDInfo(IPF_CHANNEL_TYPE_E eChnType, BSP_U32 u32RdqPtr)
{     
    switch(eChnType)
    {
        case IPF_CHANNEL_UP:
            if(u32RdqPtr >= IPF_ULRD_DESC_SIZE)
            {
                return IPF_ERROR;
            }
            IPF_PRINT("===========RD Info==========\n");
            IPF_PRINT("RDλ��:             %d\n",u32RdqPtr);
            IPF_PRINT("u16Attribute:       %d\n",g_stIpfUl.pstIpfRDQ[u32RdqPtr].u16Attribute);
            IPF_PRINT("u16PktLen:          %d\n",g_stIpfUl.pstIpfRDQ[u32RdqPtr].u16PktLen);
            IPF_PRINT("u32InPtr:           0x%x\n",g_stIpfUl.pstIpfRDQ[u32RdqPtr].u32InPtr);
            IPF_PRINT("u32OutPtr:          0x%x\n",g_stIpfUl.pstIpfRDQ[u32RdqPtr].u32OutPtr);
            IPF_PRINT("u16Result:          0x%x\n",g_stIpfUl.pstIpfRDQ[u32RdqPtr].u16Result);
            IPF_PRINT("u16UsrField1:       0x%x\n",g_stIpfUl.pstIpfRDQ[u32RdqPtr].u16UsrField1);
            IPF_PRINT("u32UsrField2:       0x%x\n",g_stIpfUl.pstIpfRDQ[u32RdqPtr].u32UsrField2);
            IPF_PRINT("u32UsrField3:       0x%x\n",g_stIpfUl.pstIpfRDQ[u32RdqPtr].u32UsrField3);
            break;
       case IPF_CHANNEL_DOWN:
            if(u32RdqPtr >= IPF_DLRD_DESC_SIZE)
            {
                return IPF_ERROR;
            }
            IPF_PRINT("============RD Info===========\n");
            IPF_PRINT("RDλ��:             %d\n",u32RdqPtr);
            IPF_PRINT("u16Attribute:       %d\n",g_stIpfDl.pstIpfRDQ[u32RdqPtr].u16Attribute);
            IPF_PRINT("u16PktLen:          %d\n",g_stIpfDl.pstIpfRDQ[u32RdqPtr].u16PktLen);
            IPF_PRINT("u32InPtr:           0x%x\n",g_stIpfDl.pstIpfRDQ[u32RdqPtr].u32InPtr);
            IPF_PRINT("u32OutPtr:          0x%x\n",g_stIpfDl.pstIpfRDQ[u32RdqPtr].u32OutPtr);
            IPF_PRINT("u16Result:          0x%x\n",g_stIpfDl.pstIpfRDQ[u32RdqPtr].u16Result);
            IPF_PRINT("u16UsrField1:       0x%x\n",g_stIpfDl.pstIpfRDQ[u32RdqPtr].u16UsrField1);
            IPF_PRINT("u32UsrField2:       0x%x\n",g_stIpfDl.pstIpfRDQ[u32RdqPtr].u32UsrField2);
            IPF_PRINT("u32UsrField3:       0x%x\n",g_stIpfDl.pstIpfRDQ[u32RdqPtr].u32UsrField3);
            break; 
        default:
            break;
    }
    IPF_PRINT("************************\n");
    return 0;
}


BSP_S32 BSP_IPF_Dump_RDInfo(IPF_CHANNEL_TYPE_E eChnType)
{
    int i;

    switch(eChnType)
    {
        case IPF_CHANNEL_UP:

            for(i = 0;i < IPF_ULBD_DESC_SIZE;i++)
            {
                BSP_IPF_RDInfo(IPF_CHANNEL_UP,i);
            }
        break;
		
        case IPF_CHANNEL_DOWN:

            for(i = 0;i < IPF_DLBD_DESC_SIZE;i++)
            {
                BSP_IPF_RDInfo(IPF_CHANNEL_DOWN,i);
            }
        break;
        default:
        IPF_PRINT("Input param invalid ! \n");
        break;
    }
    return 0;
}
BSP_S32 BSP_IPF_ADInfo(IPF_CHANNEL_TYPE_E eChnType, BSP_U32 u32AdqPtr, BSP_U32 u32AdType)
{

    switch(eChnType)
    {
        case IPF_CHANNEL_UP:
            if(u32AdqPtr >= IPF_ULAD0_DESC_SIZE)
            {
                return IPF_ERROR;
            }
            if(0 == u32AdType)
            {
                 IPF_PRINT("===========UL AD0 Info==========\n");
                 IPF_PRINT("ADλ��:             %d\n",u32AdqPtr);
                 IPF_PRINT("u32OutPtr0(phy_addr, use by hardware):       0x%x\n",g_stIpfUl.pstIpfADQ0[u32AdqPtr].u32OutPtr0);
                 IPF_PRINT("u32OutPtr1(usrfield skb_addr default):          0x%x\n",g_stIpfUl.pstIpfADQ0[u32AdqPtr].u32OutPtr1);
            }
            else
            {
                 IPF_PRINT("===========UL AD1 Info==========\n");
                 IPF_PRINT("ADλ��:             %d\n",u32AdqPtr);
                 IPF_PRINT("u32OutPtr0(phy_addr, use by hardware):       0x%x\n",g_stIpfUl.pstIpfADQ1[u32AdqPtr].u32OutPtr0);
                 IPF_PRINT("u32OutPtr1(usrfield skb_addr default):          0x%x\n",g_stIpfUl.pstIpfADQ1[u32AdqPtr].u32OutPtr1);
            }
            break;
       case IPF_CHANNEL_DOWN:
            if(u32AdqPtr >= IPF_ULAD1_DESC_SIZE)
            {
                return IPF_ERROR;
            }
            if(0 == u32AdType)
	      	{
                 IPF_PRINT("===========DL AD0 Info==========\n");
                 IPF_PRINT("ADλ��:             %d\n",u32AdqPtr);
                 IPF_PRINT("u32OutPtr0(phy_addr, use by hardware):       0x%x\n",g_stIpfDl.pstIpfADQ0[u32AdqPtr].u32OutPtr0);
                 IPF_PRINT("u32OutPtr1(usrfield skb_addr default):          0x%x\n",g_stIpfDl.pstIpfADQ0[u32AdqPtr].u32OutPtr1);
            }
            else
            {
                 IPF_PRINT("===========DL AD1 Info==========\n");
                 IPF_PRINT("ADλ��:             %d\n",u32AdqPtr);
                 IPF_PRINT("u32OutPtr0(phy_addr, use by hardware):       0x%x\n",g_stIpfDl.pstIpfADQ1[u32AdqPtr].u32OutPtr0);
                 IPF_PRINT("u32OutPtr1(usrfield skb_addr default):          0x%x\n",g_stIpfDl.pstIpfADQ1[u32AdqPtr].u32OutPtr1);
            }
            break;
        default:
            break;
    }
    IPF_PRINT("************************\n");
    return 0;
}


BSP_S32 BSP_IPF_Dump_ADInfo(IPF_CHANNEL_TYPE_E eChnType, BSP_U32 u32AdType)
{
    int i;

    switch(eChnType)
    {
        case IPF_CHANNEL_UP:
            for(i = 0;i < IPF_ULAD0_DESC_SIZE;i++)
            {
                BSP_IPF_ADInfo(IPF_CHANNEL_UP, i, u32AdType);
            }
        break;
		
        case IPF_CHANNEL_DOWN:

            for(i = 0;i < IPF_DLAD0_DESC_SIZE;i++)
            {
                BSP_IPF_ADInfo(IPF_CHANNEL_DOWN, i, u32AdType);
            }
        break;
		
        default:
        IPF_PRINT("Input param invalid ! \n");
        break;
    }
    return 0;
}

BSP_S32 BSP_IPF_Info(IPF_CHANNEL_TYPE_E eChnType)
{
    BSP_U32 u32BdqDepth = 0;
    BSP_U32 u32BdqWptr = 0;
    BSP_U32 u32BdqRptr = 0;
    BSP_U32 u32BdqWaddr = 0;
    BSP_U32 u32BdqRaddr = 0;
    BSP_U32 u32RdqDepth = 0;
    BSP_U32 u32RdqRptr = 0;
    BSP_U32 u32RdqWptr = 0;
    BSP_U32 u32RdqWaddr = 0;
    BSP_U32 u32RdqRaddr = 0;
    BSP_U32 u32Depth = 0;
    BSP_U32 u32status = 0;
	
    BSP_U32 u32Adq0Rptr = 0;
    BSP_U32 u32Adq0Wptr = 0;

    BSP_U32 u32Adq1Rptr = 0;
    BSP_U32 u32Adq1Wptr = 0;

    if(IPF_CHANNEL_UP == eChnType)
    {
        u32Depth = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_DQ_DEPTH_OFFSET);
        u32RdqDepth = (u32Depth>>16)&IPF_DQ_DEPTH_MASK;
        u32BdqDepth = u32Depth&IPF_DQ_DEPTH_MASK;

        u32status = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_STATE_OFFSET);

        u32BdqWptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_BDQ_WPTR_OFFSET);
        u32BdqRptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_BDQ_RPTR_OFFSET); 
        u32BdqWaddr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_BDQ_WADDR_OFFSET); 
        u32BdqRaddr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_BDQ_RADDR_OFFSET); 
    
        u32RdqWptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_RDQ_WPTR_OFFSET);
        u32RdqRptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_RDQ_RPTR_OFFSET);
        u32RdqWaddr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_RDQ_WADDR_OFFSET); 
        u32RdqRaddr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_RDQ_RADDR_OFFSET); 
		
        u32Adq0Rptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ0_RPTR_OFFSET);
        u32Adq0Wptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ0_WPTR_OFFSET);

        u32Adq1Rptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ1_RPTR_OFFSET);
        u32Adq1Wptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ1_WPTR_OFFSET);

    }
    else if(IPF_CHANNEL_DOWN == eChnType)
    {
        u32Depth = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_DQ_DEPTH_OFFSET);
        u32RdqDepth = (u32Depth>>16)&IPF_DQ_DEPTH_MASK;
        u32BdqDepth = u32Depth&IPF_DQ_DEPTH_MASK;

        u32status = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_STATE_OFFSET);
		
        u32BdqWptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_BDQ_WPTR_OFFSET);
        u32BdqRptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_BDQ_RPTR_OFFSET); 
        u32BdqWaddr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_BDQ_WADDR_OFFSET); 
        u32BdqRaddr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_BDQ_RADDR_OFFSET); 
    
        u32RdqWptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_RDQ_WPTR_OFFSET);
        u32RdqRptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_RDQ_RPTR_OFFSET);
        u32RdqWaddr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_RDQ_WADDR_OFFSET); 
        u32RdqRaddr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_RDQ_RADDR_OFFSET); 

        u32Adq0Rptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ0_RPTR_OFFSET);
        u32Adq0Wptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ0_WPTR_OFFSET);

        u32Adq1Rptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ1_RPTR_OFFSET);
        u32Adq1Wptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ1_WPTR_OFFSET);

    }
    else
    {
        return 1;
    }
    IPF_PRINT("============================\n");
    IPF_PRINT("ͨ�� ״̬:            0x%x\n", u32status);
    IPF_PRINT("BD ���:            %d\n", u32BdqDepth);
    IPF_PRINT("BD дָ��:          %d\n", u32BdqWptr);
    IPF_PRINT("BD ��ָ��:          %d\n", u32BdqRptr);
    IPF_PRINT("BD д��ַ:          0x%x\n", u32BdqWaddr);
    IPF_PRINT("BD ����ַ:          0x%x\n", u32BdqRaddr);
    IPF_PRINT("RD ���:            %d\n", u32RdqDepth);
    IPF_PRINT("RD ��ָ��:          %d\n", u32RdqRptr);
    IPF_PRINT("RD дָ��:          %d\n", u32RdqWptr);
    IPF_PRINT("RD ����ַ:          0x%x\n", u32RdqRaddr);
    IPF_PRINT("RD д��ַ:          0x%x\n", u32RdqWaddr);

    IPF_PRINT("AD0 ��ָ��:          %d\n", u32Adq0Rptr);
    IPF_PRINT("AD0 дָ��:          %d\n", u32Adq0Wptr);
	
    IPF_PRINT("AD1 ��ָ��:          %d\n", u32Adq1Rptr);
    IPF_PRINT("AD1 дָ��:          %d\n", u32Adq1Wptr);
    IPF_PRINT("============================\n");   
    return 0;
}

BSP_VOID BSP_IPF_MEM(BSP_VOID)
{
	BSP_U32 ipf_Shared_ddr_start = SHM_MEM_IPF_ADDR;
	
	BSP_U32 ipf_Shared_ddr_ul_start = IPF_ULBD_MEM_ADDR;

	BSP_U32 ipf_Shared_ddr_filter_pwc_start = IPF_PWRCTL_BASIC_FILTER_ADDR;

	BSP_U32 ipf_Shared_ddr_pwc_info_start = IPF_PWRCTL_INFO_ADDR;

	BSP_U32 ipf_Shared_ddr_dlcdrptr = IPF_DLCDRPTR_MEM_ADDR;

	BSP_U32 ipf_Shared_ddr_debug_dlcd_start = IPF_DEBUG_DLCD_ADDR;
	BSP_U32 ipf_Shared_ddr_debug_dlcd_size = IPF_DEBUG_DLCD_SIZE;
	BSP_U32 ipf_Shared_ddr_end = IPF_DEBUG_INFO_END_ADDR;

	
	IPF_PRINT("ipf_Shared_ddr_start                    value is 0x%x \n", ipf_Shared_ddr_start);
	IPF_PRINT("ipf_Shared_ddr_ul_start                value is 0x%x \n", ipf_Shared_ddr_ul_start);
	IPF_PRINT("ipf_Shared_ddr_filter_pwc_start     value is 0x%x \n", ipf_Shared_ddr_filter_pwc_start);
	IPF_PRINT("ipf_Shared_ddr_pwc_info_start      value is 0x%x \n", ipf_Shared_ddr_pwc_info_start);
	IPF_PRINT("ipf_Shared_ddr_dlcdrptr                value is 0x%x \n", ipf_Shared_ddr_dlcdrptr);
	IPF_PRINT("ipf_Shared_ddr_debug_dlcd_start   value is 0x%x \n", ipf_Shared_ddr_debug_dlcd_start);
	IPF_PRINT("ipf_Shared_ddr_debug_dlcd_size    value is 0x%x \n", ipf_Shared_ddr_debug_dlcd_size);
	IPF_PRINT("ipf_Shared_ddr_end                     value is 0x%x \n", ipf_Shared_ddr_end);
	IPF_PRINT("ipf_Shared_ddr_total_size             value is 0x%x \n", (ipf_Shared_ddr_end-ipf_Shared_ddr_start));


    IPF_PRINT("=======================================\n");
    IPF_PRINT("   BSP_IPF_MEM          ADDR            SIZE\n");
    IPF_PRINT("%s%#x\t\t%#x\n", "IPF_ULBD_MEM_ADDR    ", IPF_ULBD_MEM_ADDR, IPF_ULBD_MEM_SIZE);
    IPF_PRINT("%s%#x\t\t%#x\n", "IPF_ULRD_MEM_ADDR    ", IPF_ULRD_MEM_ADDR, IPF_ULRD_MEM_SIZE);
    IPF_PRINT("%s%#x\t\t%#x\n", "IPF_DLBD_MEM_ADDR    ", IPF_DLBD_MEM_ADDR, IPF_DLBD_MEM_SIZE);
    IPF_PRINT("%s%#x\t\t%#x\n", "IPF_DLRD_MEM_ADDR    ", IPF_DLRD_MEM_ADDR, IPF_DLRD_MEM_SIZE);
    IPF_PRINT("%s%#x\t\t%#x\n", "IPF_DLCD_MEM_ADDR    ", IPF_DLCD_MEM_ADDR, IPF_DLCD_MEM_SIZE);
    IPF_PRINT("%s%#x\t\t%#x\n", "IPF_INIT_ADDR        ", IPF_INIT_ADDR, IPF_INIT_SIZE);
    IPF_PRINT("%20s%#x\t\t%#x\n", "IPF_DEBUG_INFO_ADDR  ", IPF_DEBUG_INFO_ADDR, IPF_DEBUG_INFO_SIZE);
}

#ifdef __VXWORKS__
#ifdef CONFIG_BALONG_MODEM_RESET
/*****************************************************************************
* �� �� ��     : BSP_IPF_GetControlFLagForCcoreReset
*
* ��������  : modem������λipf���亯���������ڸ�λʱ��ֹ��������
*
* �������  : ��
*   
* �������  : ��
*
* �� �� ֵ     : �ɹ�
*
* �޸ļ�¼  :2014��2��14��   �¶���  ����
*****************************************************************************/
static IPF_FORREST_CONTROL_E BSP_IPF_GetControlFLagForCcoreReset(BSP_VOID)
{
    return *modem_reset_flag;
}
#endif


/*****************************************************************************
* �� �� ��  : IPF_FilterList_Init
*
* ��������  : IPF�����������ʼ��     �ڲ�ʹ�ã�����Ϊ�ӿں���
*
* �������  : BSP_VOID
* �������  : ��
* �� �� ֵ  : IPF_SUCCESS    �����������ʼ���ɹ�
*                       IPF_ERROR      �����������ʼ��ʧ��
*
* �޸ļ�¼  :2011��11��17��   ³��  ����
*****************************************************************************/
BSP_S32 IPF_FilterList_Init(BSP_VOID)
{
    BSP_U32 i = 0;
    IPF_ID_S* current = NULL;
    IPF_ID_S* prev = NULL;
    IPF_ID_S* tmp = NULL;
	
    /* ��ʼ������ͷ�ڵĵĹ�������uselist����0-7*/	
    for(i =  0; i < IPF_FILTER_CHAIN_MAX_NUM; i++)
    {
        g_stIPFFilterInfo[i].u32FilterNum = 0;
        g_stIPFFilterInfo[i].pstUseList = NULL;
        IPF_DeleteAll(i);
    }

    /* ��ʼ��basiclist  ��8�ſ�ʼ��0-7������ͷ������*/
    for(i =  IPF_FILTER_CHAIN_MAX_NUM; i < IPF_BF_NUM; i++)
    {
        /* coverity[alloc_fn] */
        current = (IPF_ID_S*)malloc(sizeof(IPF_ID_S));
        if(NULL == current)
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF, "\r IPF_FilterList_Init malloc ERROR! \n");
            goto FREE_ALL;
        }
        if(g_stIPFBasicFreeList != NULL)
        {
            if(NULL != prev)
            {
                prev->pstNext = current;/*lint !e613*/
            }
            else
            {
                bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF, "\r IPF_FilterList_Init prev == null! \n");
                goto FREE_ALL;
            }
        }
        else /* basicfreelistͷ��� */
        {
            g_stIPFBasicFreeList = current;
        }
        current->u32FilterID = i;
        current->u32PsID = 0;
        current->pstNext = NULL;

        prev = current;
    }
	
    prev = NULL;
    /* ��ʼ��extfreelist  ��64�ſ�ʼ*/
    for(i =  IPF_BF_NUM; i < IPF_TOTAL_FILTER_NUM; i++)
    {
        current = (IPF_ID_S*)malloc(sizeof(IPF_ID_S));
        if(NULL == current)
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF, "\r IPF_FilterList_Init malloc ERROR! \n");
            goto FREE_ALL;
        }
        if(g_stIPFExtFreeList != NULL)
        {
            if(NULL != prev)
            {
                prev->pstNext = current;/*lint !e613*/
            }
            else
            {
                bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF, "\r IPF_FilterList_Init prev == null! \n");
                goto FREE_ALL;
            }
        }
        else /* freelistͷ��� */
        {
            g_stIPFExtFreeList = current;
        }
        current->u32FilterID = i;
        current->u32PsID = 0;
        current->pstNext = NULL;
        prev = current;
    }
    	
    return IPF_SUCCESS;
    
FREE_ALL:
    i=0;
	
    /* coverity[overrun-local] */
    while((g_stIPFFilterInfo[i].pstUseList != NULL)&&(i <IPF_FILTER_CHAIN_MAX_NUM))
    {
        free(g_stIPFFilterInfo[i].pstUseList );
        /* coverity[incr] */
        i++;
    }

    while(g_stIPFBasicFreeList!= NULL)
    {
    	tmp = g_stIPFBasicFreeList->pstNext;
        free(g_stIPFBasicFreeList);
        g_stIPFBasicFreeList = tmp;
    }

    while(g_stIPFExtFreeList!= NULL)
    {
    	tmp = g_stIPFExtFreeList->pstNext;
        free(g_stIPFExtFreeList);
        g_stIPFExtFreeList = tmp;
    }

    return IPF_ERROR;
}

/*****************************************************************************
* �� �� ��  : IPF_AddToFreeList
*
* ��������  : �����ӵ�freelist�ĺ���λ��
*
* �������  : IPF_ID_S* stNode             ������Ľ��ָ��
*             
* �������  : ��

* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2011��3��30��   ³��  ����
*****************************************************************************/
BSP_VOID IPF_AddToBasicFreeList(IPF_ID_S* stNode)
{
    IPF_ID_S* current = NULL;    
    IPF_ID_S* prev = NULL;

    current = g_stIPFBasicFreeList;

    /* ���freelistΪ�� */
    if(NULL == current)
    {
        g_stIPFBasicFreeList = stNode;
        g_stIPFBasicFreeList->pstNext = NULL;
        return;
    }

    /* ����ȵ�һ������ID��С */
    if(stNode->u32FilterID < current->u32FilterID)
    {
        stNode->pstNext = current;
        g_stIPFBasicFreeList = stNode;       
        return;
    }

    prev = g_stIPFBasicFreeList;
    current = g_stIPFBasicFreeList->pstNext;

    while(current != NULL)
    {
        if(stNode->u32FilterID < current->u32FilterID)
        {
            prev->pstNext = stNode;
            stNode->pstNext = current;
            return;
        }
        prev = current;
        current = current->pstNext;
    }
    /* ������� */
    prev->pstNext = stNode;
    stNode->pstNext = NULL;

}

BSP_VOID IPF_AddToExtFreeList(IPF_ID_S* stNode)
{
    IPF_ID_S* current = NULL;    
    IPF_ID_S* prev = NULL;

    current = g_stIPFExtFreeList;

    /* ���freelistΪ�� */
    if(NULL == current)
    {
        g_stIPFExtFreeList = stNode;
        g_stIPFExtFreeList->pstNext = NULL;
        return;
    }

    /* ����ȵ�һ������ID��С */
    if(stNode->u32FilterID < current->u32FilterID)
    {
        stNode->pstNext = current;
        g_stIPFExtFreeList = stNode;       
        return;
    }

    prev = g_stIPFExtFreeList;
    current = g_stIPFExtFreeList->pstNext;

    while(current != NULL)
    {
        if(stNode->u32FilterID < current->u32FilterID)
        {
            prev->pstNext = stNode;
            stNode->pstNext = current;
            return;
        }
        prev = current;
        current = current->pstNext;
    }
    /* ������� */
    prev->pstNext = stNode;
    stNode->pstNext = NULL;

}

/*****************************************************************************
* �� �� ��  : IPF_DeleteAll
*
* ��������  : ɾ�������е����н��
*
* �������  : IPF_FILTER_CHAIN_TYPE_E eFilterChainhead���������׵�ַ           
*             
* �������  : ��

* �� �� ֵ  : ��
*
* �޸ļ�¼  :2011��3��30��   ³��  ����
*****************************************************************************/
BSP_VOID IPF_DeleteAll(IPF_FILTER_CHAIN_TYPE_E eFilterChainhead)
{
    IPF_ID_S* pstMove = NULL;
    /* coverity[var_decl] */
    while(IPF_FILTERHEAD < g_stIPFFilterInfo[eFilterChainhead].u32FilterNum)
    {
        pstMove = g_stIPFFilterInfo[eFilterChainhead].pstUseList;
        /* ������uselistɾ�� */
        g_stIPFFilterInfo[eFilterChainhead].pstUseList = pstMove->pstNext;
	
        /* ��ɾ���Ľ����ӵ�freelist�� */ 
        if(IPF_BF_NUM > (pstMove->u32FilterID))
        {
            IPF_AddToBasicFreeList(pstMove); 
        }
        else
        {	
            IPF_AddToExtFreeList(pstMove); 
        }
        g_stIPFFilterInfo[eFilterChainhead].u32FilterNum--;
    }

    /* ���׹�������������Ϊ511��ʹ������Ϊ0 */
    ipf_writel(eFilterChainhead, HI_IPF_REGBASE_ADDR_VIRT + HI_BFLT_INDEX_OFFSET);
    ipf_writel(IPF_TAIL_INDEX, HI_IPF_REGBASE_ADDR_VIRT+HI_FLT_CHAIN_OFFSET); 
    ipf_writel(IPF_DISABLE_FILTER, HI_IPF_REGBASE_ADDR_VIRT+HI_FLT_RULE_CTRL_OFFSET); 
    BSP_IPF_BackupFilter(eFilterChainhead);
}

/*****************************************************************************
* �� �� ��  : IPF_MallocOneBasicFilter
*
* ��������  : ��freelist�з���һ�����(ȡ����һ�����)
*
* �������  : ��       
*             
* �������  : ��

* �� �� ֵ  : ���ָ��
*
* �޸ļ�¼  : 2011��3��30��   ³��  ����
*
* ˵��      : �ɵ��ú�������֤һ���ܷ��䵽���
*****************************************************************************/
IPF_ID_S* IPF_MallocOneBasicFilter(BSP_VOID)
{
    IPF_ID_S* current = NULL;
    	current = g_stIPFBasicFreeList ;

    if(NULL == current)
    {
        bsp_trace(BSP_LOG_LEVEL_WARNING, BSP_MODU_IPF, "IPF_MallocOneBasicFilter Basic run out\n");
        return NULL;
    }
    g_stIPFBasicFreeList  = g_stIPFBasicFreeList ->pstNext;  

    return current;
}
/*****************************************************************************
* �� �� ��  : IPF_MallocOneExtFilter
*
* ��������  : ��freelist�з���һ�����(ȡ����һ�����)
*
* �������  : ��       
*             
* �������  : ��

* �� �� ֵ  : ���ָ��
*
* �޸ļ�¼  : 2011��3��30��   ³��  ����
*
* ˵��      : �ɵ��ú�������֤һ���ܷ��䵽���
*****************************************************************************/
IPF_ID_S* IPF_MallocOneExtFilter(BSP_VOID)
{
    IPF_ID_S* current = NULL;
    	current = g_stIPFExtFreeList ;

    if(NULL == current)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF, "IPF_MallocOneExtFilter Ext run out\n");
        return NULL;
    }
    g_stIPFExtFreeList  = g_stIPFExtFreeList ->pstNext;  

    return current;
}

/*****************************************************************************
* �� �� ��  : IPF_AddTailUsedFilter
*
* ��������  : �����ӵ�uselist�Ľ�β
*
* �������  : BSP_U8 eFilterChainhead ͨ������   
*             IPF_ID_S* stNode             ������Ľ��ָ��
*             
* �������  : ��

* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2011��3��30��   ³��  ����
                             2012��11��25�ճ¶����޸�
*****************************************************************************/
BSP_VOID IPF_AddTailUsedFilter(IPF_FILTER_CHAIN_TYPE_E eFilterChainhead,IPF_ID_S* stNode)
{
    IPF_ID_S* current = NULL;

    current = g_stIPFFilterInfo[eFilterChainhead].pstUseList;
    if(NULL == current)
    {
        g_stIPFFilterInfo[eFilterChainhead].pstUseList = stNode;
        g_stIPFFilterInfo[eFilterChainhead].pstUseList->pstNext = NULL;
        g_stIPFFilterInfo[eFilterChainhead].u32FilterNum = 1;
        return;
    }
	
    /* �����ӵ������β */
    while(current->pstNext != NULL)
    {
        current = current->pstNext;
    }
    current->pstNext = stNode;
    stNode->pstNext = NULL;
    
    g_stIPFFilterInfo[eFilterChainhead].u32FilterNum++;

}

/*****************************************************************************
* �� �� ��  : IPF_AddTailFilterChain
*
* ��������  :�����������õ�Ӳ����
*
* �������  : BSP_U32 u32LastFilterID, BSP_U32 u32FilterID, IPF_MATCH_INFO_S* pstMatchInfo
*             
* �������  : ��

* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2011��3��30��   ³��  ����
*****************************************************************************/
BSP_VOID IPF_AddTailFilterChain(BSP_U32 u32LastFilterID, BSP_U32 u32FilterID, IPF_MATCH_INFO_S* pstMatchInfo)
{
    BSP_U32 u32Value = 0;
    IPF_MATCH_INFO_S* pstLastMatchInfo = NULL;
    
    /* ����������--���µĹ������������õ������� */
    if(u32FilterID < IPF_BF_NUM)
    {
        ipf_write_basic_filter(u32FilterID, pstMatchInfo);
		
        /*�Ի������������б���*/
        BSP_IPF_BackupFilter(u32FilterID);
    }
    /* ��չ������ */
    else
    {
        memcpy((BSP_VOID*)(g_pstExFilterAddr + u32FilterID - IPF_BF_NUM), pstMatchInfo, sizeof(IPF_MATCH_INFO_S));
    }

    /* ���ǵ�һ��filter����Ҫ������һ��filter��nextindex�� */
    if(u32LastFilterID != IPF_TAIL_INDEX)
    {
        /* ���ϴ��������һ����������nextIndex���������� */
        if(u32LastFilterID < IPF_BF_NUM)
        {
            ipf_writel(u32LastFilterID, HI_IPF_REGBASE_ADDR_VIRT + HI_BFLT_INDEX_OFFSET); 
            u32Value = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_FLT_CHAIN_OFFSET);
            u32Value = (u32Value&0x1FF0000) + u32FilterID;
            ipf_writel(u32Value, HI_IPF_REGBASE_ADDR_VIRT + HI_FLT_CHAIN_OFFSET);
			
            /*���޸�next index�Ļ����������������±���*/
            BSP_IPF_BackupFilter(u32LastFilterID);
        }
        else
        {
            pstLastMatchInfo = g_pstExFilterAddr + u32LastFilterID - IPF_BF_NUM;
            pstLastMatchInfo->unFltChain.Bits.u16NextIndex = u32FilterID;        
        }
    }    
}

/*****************************************************************************
* �� �� ��     : IPF_FindFilterID
*
* ��������  : ��������������Ѱ����PS ID ƥ���Filter ID
*
* �������  : IPF_FILTER_CHAIN_TYPE_E eFilterChainhead    ͨ������
*                           BSP_U32 u32PsID                PS ID            
*             
* �������  : BSP_U32* u32FilterID   ��ѯ����Filter ID

* �� �� ֵ     : IPF_SUCCESS                ��ѯ�ɹ�
*                          IPF_ERROR                  ��ѯʧ��
*
* �޸ļ�¼  :2011��1��11��   ³��  ����
*****************************************************************************/
BSP_S32 IPF_FindFilterID(IPF_FILTER_CHAIN_TYPE_E eFilterChainhead, BSP_U32 u32PsID, BSP_U32* u32FilterID)
{
    IPF_ID_S* current = NULL;

    current = g_stIPFFilterInfo[eFilterChainhead].pstUseList;
    while(current != NULL)
    {
        if(current->u32PsID != u32PsID)
        {
            current = current->pstNext;
        }
        else
        {
            *u32FilterID = current->u32FilterID;
            return IPF_SUCCESS;
        }
    }
    return IPF_ERROR;        
}

/*****************************************************************************
* �� �� ��      : BSP_IPF_Init
*
* ��������  : IPF����������ʼ��
*
* �������  : IPF_COMMON_PARA_S *pstCommPara
* �������  : ��
* �� �� ֵ      : IPF_SUCCESS    ���óɹ�
*                           BSP_ERR_IPF_NOT_INIT     δ��ʼ��
*                           BSP_ERR_IPF_INVALID_PARA  ��������
*
* ˵��             : ����IPFȫ�ֿ������üĴ��� ����ѡ�����ȼ���������
*
* �޸ļ�¼  : 2011��11��29��   ³��  ����
*****************************************************************************/
BSP_S32 BSP_IPF_Init(IPF_COMMON_PARA_S *pstCommPara)
{
    BSP_U32 u32IPFCtrl = 0;
    BSP_BOOL chn_schedule_strategy = SCH_BD_ONLY;

#ifndef IPF_SCHEDULER_PATCH
    chn_schedule_strategy = SCH_ALL_DESC;
#else
    chn_schedule_strategy = SCH_BD_ONLY;
#endif
    /* ������� */
    if(NULL == pstCommPara)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_Init pstCommPara NULL! \n");
        return BSP_ERR_IPF_INVALID_PARA;
    }
    /*��ʼ�����*/
    if(IPF_MCORE_INIT_SUCCESS != g_IPFInit)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF, "\r IPF NOT Init! \n");
        return BSP_ERR_IPF_NOT_INIT;
    }

    /* ����IPFȫ�ֿ������üĴ��� */
    u32IPFCtrl = pstCommPara->bFilterSeq | (pstCommPara->bFltAddrReverse<<1) | (pstCommPara->bSpPriSel<<2)/*lint !e701*/ 
                 | (pstCommPara->bSpWrrModeSel<<3) | (pstCommPara->eMaxBurst<<4)/*lint !e701*/
                 | (pstCommPara->bIpv6NextHdSel<<6) | (pstCommPara->bEspAhSel<<7)/*lint !e701*/
                 | (pstCommPara->bAhSpiDisable<<8) | (pstCommPara->bEspSpiDisable<<9)/*lint !e701*/
                 | (pstCommPara->bMultiFilterChainEn<<10) | (pstCommPara->bMultiModeEn<<11)
                 | (pstCommPara->bAdReport<<12) | (chn_schedule_strategy<<13);
	
    bsp_trace(BSP_LOG_LEVEL_DEBUG, BSP_MODU_IPF, "\r u32IPFCtrl = 0x%x \n", u32IPFCtrl);
    ipf_writel(u32IPFCtrl, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CTRL_OFFSET); 

    return  IPF_SUCCESS; 
}

/*****************************************************************************
* �� �� ��      : BSP_IPF_ConfigUlChannel
*
* ��������  : ��������ͨ�����Ʋ���
*
* �������  : IPF_CHL_CTRL_S *pstCtrl 
* �������  : ��
* �� �� ֵ      : ��
*
* ˵��             : ��������IPFͨ�����ƼĴ���
*
* �޸ļ�¼  : 2011��11��29��   ³��  ����
			 2014��1��23��v1.01 �¶��� �޸� ����K3V3����������⣬
			 �޷���m3�Ͻ��е͹��Ļָ����ƶ���A9�Ͻ��С�
*****************************************************************************/
BSP_S32 BSP_IPF_ConfigUlChannel(IPF_CHL_CTRL_S *pstCtrl)
{
    BSP_U32 u32ChanCtrl = 0; 
	
#if (defined(BSP_CONFIG_HI3630))
    /*����K3V3����������⣬ʹ��ipf ǰ��Ҫ����Ƿ���Ҫ�ָ�������*/
    ipf_check_filter_restore();
#endif

    /* ������� */
    if(NULL == pstCtrl)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigUlChannel pstCtrl NULL! \n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    /*��������ͨ�����ƼĴ��� */
    u32ChanCtrl = pstCtrl->eIpfMode | (pstCtrl->bEndian<<2) | 
                            (pstCtrl->bDataChain<<3) | (pstCtrl->u32WrrValue<<16);
    bsp_trace(BSP_LOG_LEVEL_DEBUG, BSP_MODU_IPF,"\r u32ChanCtrl = 0x%x \n", u32ChanCtrl);
    ipf_writel(u32ChanCtrl, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_CTRL_OFFSET);  

    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��      : BSP_IPF_ConfigDlChannel
*
* ��������  : ��������ͨ�����Ʋ���
*
* �������  : IPF_CHL_CTRL_S *pstCtrl 
* �������  : ��
* �� �� ֵ      : ��
*
* ˵��             : ��������IPFͨ�����ƼĴ���
*
* �޸ļ�¼  : 2011��11��29��   ³��  ����
			 2014��1��23��v1.01 �¶��� �޸� ����K3V3����������⣬
			 �޷���m3�Ͻ��е͹��Ļָ����ƶ���A9�Ͻ��С�
*****************************************************************************/
BSP_S32 BSP_IPF_ConfigDlChannel(IPF_CHL_CTRL_S *pstCtrl)
{
    BSP_U32 u32ChanCtrl = 0; 

#if (defined(BSP_CONFIG_HI3630))
    /*����K3V3����������⣬ʹ��ipf ǰ��Ҫ����Ƿ���Ҫ�ָ�������*/
    ipf_check_filter_restore();
#endif

    /* ������� */
    if(NULL == pstCtrl)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigDlChannel pstCtrl NULL! \n");
        return BSP_ERR_IPF_INVALID_PARA;
    }
    
    /*��������ͨ�����ƼĴ��� */
    u32ChanCtrl = pstCtrl->eIpfMode | (pstCtrl->bEndian<<2) | 
                            (pstCtrl->bDataChain<<3) | (pstCtrl->u32WrrValue<<16);
    bsp_trace(BSP_LOG_LEVEL_DEBUG, BSP_MODU_IPF,"\r u32ChanCtrl = 0x%x \n", u32ChanCtrl);
    ipf_writel(u32ChanCtrl, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_CTRL_OFFSET);

    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��      : BSP_IPF_ConfigADThr
*
* ��������  : ����������ͨ��ADq���̰���ֵ
*
* �������  : BSP_U32 u32UlADThr,BSP_U32 u32DlADThr
* �������  : ��
* �� �� ֵ      : ��
*
* ˵��             : ����������ͨ��ADq���̰���ֵ
*
* �޸ļ�¼  : 2011��12��13��   �¶�������
			 2014��1��23��v1.01 �¶��� �޸� ����K3V3����������⣬
			 �޷���m3�Ͻ��е͹��Ļָ����ƶ���A9�Ͻ��С�
*****************************************************************************/
BSP_S32 BSP_IPF_ConfigADThr(BSP_U32 u32UlADThr,BSP_U32 u32DlADThr)
{
    BSP_U32 u32ADCtrl[IPF_CHANNEL_MAX] = {0,0};

    /* ���ģ���Ƿ��ʼ�� */
    if(IPF_MCORE_INIT_SUCCESS != g_IPFInit)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF Not Init! \n");
        return BSP_ERR_IPF_NOT_INIT;
    }
	
#if (defined(BSP_CONFIG_HI3630))
    /*����K3V3����������⣬ʹ��ipf ǰ��Ҫ����Ƿ���Ҫ�ָ�������*/
    ipf_check_filter_restore();
#endif

    if((u32UlADThr == 0)  || (u32UlADThr > 0xFFFF) || (u32DlADThr == 0)  || (u32DlADThr > 0xFFFF))
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigADThr INPUT ERROR! \n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    u32ADCtrl[IPF_CHANNEL_UP] = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ_CTRL_OFFSET);
    u32ADCtrl[IPF_CHANNEL_DOWN] = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ_CTRL_OFFSET);
    u32ADCtrl[IPF_CHANNEL_UP] &= IPF_ADQ_THR_MASK;
    u32ADCtrl[IPF_CHANNEL_DOWN] &= IPF_ADQ_THR_MASK;
    u32ADCtrl[IPF_CHANNEL_UP] |= (u32UlADThr<<16);
    u32ADCtrl[IPF_CHANNEL_DOWN] |= (u32DlADThr<<16);
    /* ����������ͨ����AD��ֵ*/
    ipf_writel(u32ADCtrl[IPF_CHANNEL_UP], HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ_CTRL_OFFSET);
    ipf_writel(u32ADCtrl[IPF_CHANNEL_DOWN], HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ_CTRL_OFFSET);
    return IPF_SUCCESS;
}
BSP_S32 BSP_IPF_SetFilter(IPF_FILTER_CHAIN_TYPE_E eFilterChainhead, IPF_FILTER_CONFIG_S *pstFilterInfo, BSP_U32 u32FilterNum)
{
	BSP_U32 i = 0;
	IPF_ID_S* current = NULL;
	BSP_U32 u32LastFilterID = IPF_TAIL_INDEX;
	BSP_U32 u32FirstFilterID = IPF_TAIL_INDEX;
	unsigned long flags = 0;

#if (defined(BSP_CONFIG_HI3630))
    /*����K3V3����������⣬ʹ��ipf ǰ��Ҫ����Ƿ���Ҫ�ָ�������*/
    ipf_check_filter_restore();
#endif

	/* ������� */
	if((eFilterChainhead >= IPF_FILTER_CHAIN_MAX_NUM) || (NULL == pstFilterInfo))
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_SetFilter input para ERROR! \n");
		return BSP_ERR_IPF_INVALID_PARA;
	}

	/* ���ģ���Ƿ��ʼ�� */
	if(IPF_MCORE_INIT_SUCCESS != g_IPFInit)
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF Not Init! \n");
		return BSP_ERR_IPF_NOT_INIT;
	}

	spin_lock_irqsave(&ipf_filter_spinlock, flags);
#if (defined(BSP_CONFIG_HI3630))
	bsp_ipc_spin_lock(IPC_SEM_IPF_PWCTRL);
#endif
	/* ����üĴ������׵�ַ��������н�� */
	IPF_DeleteAll(eFilterChainhead);
	udelay(g_filter_delay_time);
		
	/*��������*/
	for(i = 0; i < u32FilterNum; i++)
	{
        /* ��freelist��ȡ��δ���õĹ����� */
		if(!(IPF_SET_EXT_FILTER & (pstFilterInfo->u32FilterID)))
		{    
			current = IPF_MallocOneBasicFilter();
			if(NULL == current)
			{		
				current = IPF_MallocOneExtFilter();
				if(NULL == current)
				{		
				#if (defined(BSP_CONFIG_HI3630))
					bsp_ipc_spin_unlock(IPC_SEM_IPF_PWCTRL);
				#endif
					spin_unlock_irqrestore(&ipf_filter_spinlock, flags);
					bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF_MallocOneFilter error! \n");
					return BSP_ERR_IPF_FILTER_NOT_ENOUGH;
				}
			}		
		}
		else
		{
			current = IPF_MallocOneExtFilter();
			if(NULL == current)
			{
			#if (defined(BSP_CONFIG_HI3630))
				bsp_ipc_spin_unlock(IPC_SEM_IPF_PWCTRL);
			#endif
				spin_unlock_irqrestore(&ipf_filter_spinlock, flags);
				bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF_MallocOneFilter error! \n");
				return BSP_ERR_IPF_FILTER_NOT_ENOUGH;
			}
		}
		if(0 == i)
		{
			u32FirstFilterID = current->u32FilterID;
		}
		current->u32PsID = (pstFilterInfo->u32FilterID)&IPF_FILTERTYPE_MASK;
		/* �Ƚ�NextIndex����Ϊ511 */
		pstFilterInfo->stMatchInfo.unFltChain.Bits.u16NextIndex = IPF_TAIL_INDEX;
		IPF_AddTailFilterChain(u32LastFilterID, current->u32FilterID, &pstFilterInfo->stMatchInfo);
		IPF_AddTailUsedFilter(eFilterChainhead, current);
		u32LastFilterID = current->u32FilterID;
		pstFilterInfo++; 
	}
	
	/*�����úõĹ��������ҵ�������*/
	ipf_writel(eFilterChainhead, HI_IPF_REGBASE_ADDR_VIRT + HI_BFLT_INDEX_OFFSET);
	ipf_writel(u32FirstFilterID, HI_IPF_REGBASE_ADDR_VIRT+HI_FLT_CHAIN_OFFSET); 
	
	/*���ݸ��ĺ���׹�����*/
	BSP_IPF_BackupFilter(eFilterChainhead);
	cache_sync();

	#if (defined(BSP_CONFIG_HI3630))
		bsp_ipc_spin_unlock(IPC_SEM_IPF_PWCTRL);
	#endif
	spin_unlock_irqrestore(&ipf_filter_spinlock, flags);

	return IPF_SUCCESS;
}
/*****************************************************************************
* �� �� ��      : BSP_IPF_GetFilter
*
* ��������  : �ṩ��PS��ѯĳ��Filter������Ϣ
*
* �������  : BSP_U32 u32FilterID   Filter ID��ע�������psid
*             
* �������  : IPF_FILTER_CONFIG_S * pFilterInfo  ��ѯ����Filter��Ϣ

* �� �� ֵ     : IPF_SUCCESS                ��ѯ�ɹ�
*                           IPF_ERROR                  ��ѯʧ��
*                           BSP_ERR_IPF_NOT_INIT       ģ��δ��ʼ��
*                           BSP_ERR_IPF_INVALID_PARA   ��������
*
* �޸ļ�¼  : 2011��1��11��   ³��  ����
                             2011��3��30��   ³��  �޸�
                             2011    11��30��   ³��  �޸�
                             2012    11��26��   �¶����޸�
				 			 2014��1��23��v1.01 �¶��� �޸� ����K3V3����������⣬
							 �޷���m3�Ͻ��е͹��Ļָ����ƶ���A9�Ͻ��С�
*****************************************************************************/
BSP_S32 BSP_IPF_GetFilter(IPF_FILTER_CHAIN_TYPE_E eFilterChainhead, BSP_U32 u32FilterID, IPF_FILTER_CONFIG_S *pstFilterInfo)
{
    BSP_U32 u32FindID = 0;
    
    
    /* ���ģ���Ƿ��ʼ�� */
    if(IPF_MCORE_INIT_SUCCESS != g_IPFInit)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF Not Init! \n");
        return BSP_ERR_IPF_NOT_INIT;
    }
	
#if (defined(BSP_CONFIG_HI3630))
    /*����K3V3����������⣬ʹ��ipf ǰ��Ҫ����Ƿ���Ҫ�ָ�������*/
    ipf_check_filter_restore();
#endif

    /* ������� */
    if(NULL == pstFilterInfo)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF, "\r BSP_IPF_GetFilter input para ERROR! \n");
        return BSP_ERR_IPF_INVALID_PARA;
    }

    /* ����PS ID ��Ӧ��FilterID*/
    if(IPF_FindFilterID(eFilterChainhead, u32FilterID, &u32FindID) != IPF_SUCCESS)
    {
        return IPF_ERROR;
    }
    pstFilterInfo->u32FilterID = u32FilterID;    
    
    /* ����ǻ��������� */
    if(u32FindID < IPF_BF_NUM)
    {
        /* д���˱������ַ */    
        ipf_writel(u32FindID, HI_IPF_REGBASE_ADDR_VIRT + HI_BFLT_INDEX_OFFSET); 
         
        /* ���������������� */
        memcpy(&pstFilterInfo->stMatchInfo, (BSP_VOID*)(HI_IPF_REGBASE_ADDR_VIRT+HI_FLT_LOCAL_ADDR0_OFFSET), sizeof(IPF_MATCH_INFO_S));/* [false alarm]:fortify disable */
    }
    /* ��չ������ */
    else
    {
        /* ���������������� */
        memcpy(&pstFilterInfo->stMatchInfo, (BSP_VOID*)(g_pstExFilterAddr + u32FindID - IPF_BF_NUM), sizeof(IPF_MATCH_INFO_S));
    }
    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��      : IPF_ConfigCD
*
* ��������  : ����CD
*
* �������  : IPF_TTF_MEM_ST *pstTtf, BSP_U16* pu16TotalDataLen, BSP_U32* pu32BdInPtr
* �������  : ��
* �� �� ֵ      :  ��
*
* �޸ļ�¼  :2011��1��24��   ³��  ����
*****************************************************************************/
BSP_S32 IPF_ConfigCD(TTF_MEM_ST *pstTtf, BSP_U16* pu16TotalDataLen, BSP_U32* pu32BdInPtr)
{
    BSP_U32 u32DlCDWptr = 0;
    BSP_U32 u32DlCDRptr = 0;
    BSP_U16 u16TotalDataLen = 0;
    TTF_MEM_ST* p = NULL;

    /* Ϊ��ֹBD �����ߺ�������Ŀ�ĵ�ַ���ɹ���������ʱcd��λ�� */
    u32DlCDWptr = g_stIpfDl.u32IpfCdWptr;
    u32DlCDRptr = *(g_stIpfDl.u32IpfCdRptr);
    *pu32BdInPtr = (BSP_U32)(g_stIpfDl.pstIpfCDQ + (u32DlCDWptr));

    /* ����CD */
    p = pstTtf;
    do
    {
        /* �ж��Ƿ��п���CD */
        if(u32DlCDRptr != (u32DlCDWptr+1)%IPF_DLCD_DESC_SIZE)
        {
            g_stIpfDl.pstIpfCDQ[u32DlCDWptr].u32Ptr = (BSP_U32)p->pData;
            g_stIpfDl.pstIpfCDQ[u32DlCDWptr].u16PktLen = p->usUsed;
            #ifdef __BSP_IPF_CD_DEBUG__
            g_stIpfDl.pstIpfDebugCDQ[u32DlCDWptr].u32Ptr = (BSP_U32)p->pData;
            g_stIpfDl.pstIpfDebugCDQ[u32DlCDWptr].u16PktLen = p->usUsed;
            if(g_stIPFDebugInfo->u32IpfDebug)
            {
                IPF_PRINT("usUsed = %d  pData = 0x%x\n", p->usUsed, p->pData);
            }
            #endif
            u16TotalDataLen += p->usUsed;
            
            /* ��ʶ�Ƿ���� */
            if(p->pNext != NULL)
            {
                g_stIpfDl.pstIpfCDQ[u32DlCDWptr].u16Attribute = 0;
                #ifdef __BSP_IPF_CD_DEBUG__
                g_stIpfDl.pstIpfDebugCDQ[u32DlCDWptr].u16Attribute = 0;
                #endif
                if(u32DlCDWptr+1 < IPF_DLCD_DESC_SIZE)
                {
                    u32DlCDWptr += 1;
                    p = p->pNext;
                }
                /* δ����ʱ��CD���ܷ�ת�����Ǽ�¼CD��β����ͬʱ����0��ʼ����CD */
                else
                {
                    u32DlCDWptr = 0;
                    u16TotalDataLen = 0;
                    p = pstTtf;
                    *pu32BdInPtr = (BSP_U32)g_stIpfDl.pstIpfCDQ;
                    #ifdef __BSP_IPF_CD_DEBUG__
                    if(g_stIPFDebugInfo->u32IpfDebug)
                    {
                        IPF_PRINT("CD ��ת \n");
                    }
                    #endif
                }
            }
            else
            {
                g_stIpfDl.pstIpfCDQ[u32DlCDWptr].u16Attribute = 1;
                #ifdef __BSP_IPF_CD_DEBUG__
                g_stIpfDl.pstIpfDebugCDQ[u32DlCDWptr].u16Attribute = 1;
                #endif
                
                /* ����ʱCD���Է�ת */
                u32DlCDWptr = (u32DlCDWptr+1 < IPF_DLCD_DESC_SIZE)?(u32DlCDWptr+1):0;
                #ifdef __BSP_IPF_CD_DEBUG__
                if(g_stIPFDebugInfo->u32IpfDebug)
                {
                    IPF_PRINT("u32DlCDWptr  = %d\n", u32DlCDWptr);
                }
                #endif
                break;
            }
        }
        else
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,
                    "\r DownFilter CD FULL ! u32DlCDRptr=%d u32DlCDWptr=%d\n",u32DlCDRptr,u32DlCDWptr);
            return BSP_ERR_IPF_CDQ_NOT_ENOUGH;
        }        
    }while(p != NULL);
    
    g_stIpfDl.u32IpfCdWptr = u32DlCDWptr;
    *pu16TotalDataLen = u16TotalDataLen;    
    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��      : BSP_IPF_ConfigDownFilter
*
* ��������  : IPF����BD���ú��� 
*
* �������  : BSP_U32 u32Num, IPF_CONFIG_ULPARAM_S* pstUlPara
* �������  : ��
* �� �� ֵ      : IPF_SUCCESS    ���óɹ�
*                           IPF_ERROR      ����ʧ��
*                           BSP_ERR_IPF_NOT_INIT         ģ��δ��ʼ��
*                           BSP_ERR_IPF_INVALID_PARA     ��������
*
* �޸ļ�¼  :2011��11��30��   ³��  ����
			2012��11��30�� �¶����޸���Ӷ����������
									��̬ҵ��ģʽ���õ�֧��
			2014��1��23��v1.01 �¶��� �޸� ����K3V3����������⣬
			�޷���m3�Ͻ��е͹��Ļָ����ƶ���A9�Ͻ��С�

*****************************************************************************/
BSP_S32 BSP_IPF_ConfigDownFilter(BSP_U32 u32Num, IPF_CONFIG_DLPARAM_S* pstDlPara)
{
    BSP_U32 u32BdqWptr = 0;
    BSP_U32 i;
    BSP_U32 u32BdInPtr = 0;
    BSP_U32 u32BD = 0;
    BSP_U16 u16TotalDataLen = 0;
    BSP_U32 u32TimeStampEn;

#ifdef CONFIG_BALONG_MODEM_RESET	
    /* ���Ccore�Ƿ��ϵ�*/
    if(IPF_FORRESET_CONTROL_FORBID <= BSP_IPF_GetControlFLagForCcoreReset())
    {
        g_stIPFDebugInfo->ipf_ccore_not_init_count++;
        return BSP_ERR_IPF_CCORE_RESETTING;
    }
#endif

#if (defined(BSP_CONFIG_HI3630))
    /*����K3V3����������⣬ʹ��ipf ǰ��Ҫ����Ƿ���Ҫ�ָ�������*/
    ipf_check_filter_restore();
#endif

    /*��μ��*/
    if((NULL == pstDlPara)||(0 == u32Num))
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigDownFilter input para ERROR! \n");
        return BSP_ERR_IPF_INVALID_PARA;
    }
	
#ifdef __BSP_IPF_DEBUG__
    if(u32Num > g_stIpfDl.u32IdleBd)
    {
        g_stIPFDebugInfo->u32DlBdNotEnough++;
	 IPF_PRINT("u32Num %u  <= g_stIpfDl.u32IdleBd %u \n",u32Num,g_stIpfDl.u32IdleBd);
        return IPF_ERROR;
    }
#endif
    for(i = 0; i < u32Num; i++)
    {
        if(0 == pstDlPara[i].u16Len)
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r INPUT BD_Len =0 !Drop pkt \n");
            g_stIPFDebugInfo->ipf_dlbd_len_zero_count++;
            return BSP_ERR_IPF_INVALID_PARA;
        }
    }

	
    /* ����BDдָ�� */
    u32BdqWptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_BDQ_WPTR_OFFSET);

    u32BD = u32BdqWptr&IPF_DQ_PTR_MASK;
    u32TimeStampEn = g_stIPFDebugInfo->ipf_timestamp_dl_en;

    for(i = 0; i < u32Num; i++)
    {
        /* ����CD */
        if(IPF_ConfigCD((void*)pstDlPara[i].u32Data, &u16TotalDataLen, &u32BdInPtr) != IPF_SUCCESS)
        {
            g_stIPFDebugInfo->u32DlCdNotEnough++;
            return BSP_ERR_IPF_CDQ_NOT_ENOUGH;
        }
        g_stIpfDl.pstIpfBDQ[u32BD].u16Attribute = pstDlPara[i].u16Attribute; 
        g_stIpfDl.pstIpfBDQ[u32BD].u32InPtr =  u32BdInPtr;
        g_stIpfDl.pstIpfBDQ[u32BD].u16PktLen = u16TotalDataLen;
        g_stIpfDl.pstIpfBDQ[u32BD].u16UsrField1 = pstDlPara[i].u16UsrField1;
        g_stIpfDl.pstIpfBDQ[u32BD].u32UsrField2 = pstDlPara[i].u32UsrField2;
        g_stIpfDl.pstIpfBDQ[u32BD].u32UsrField3 = pstDlPara[i].u32UsrField3;
        ipf_record_start_time_stamp(u32TimeStampEn, &g_stIpfDl.pstIpfBDQ[u32BD].u32UsrField3);
        u32BD = ((u32BD + 1) < IPF_DLBD_DESC_SIZE)? (u32BD + 1) : 0;
    }

#ifdef CONFIG_BALONG_MODEM_RESET	
    /* ���Ccore�Ƿ��ϵ�*/
    if(IPF_FORRESET_CONTROL_FORBID <= BSP_IPF_GetControlFLagForCcoreReset())
    {
        g_stIPFDebugInfo->ipf_ccore_not_init_count++;
        return BSP_ERR_IPF_CCORE_RESETTING;
    }
#endif

    g_stIPFDebugInfo->ipf_cfg_dlbd_count += u32Num;

    /* ����BDдָ�� */
    ipf_writel(u32BD, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_BDQ_WPTR_OFFSET);  

    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��     : BSP_IPF_ConfigUlAd
*
* ��������  : �ýӿڽ���C���ṩ�������ƶ�ADQдָ�룬
				�����е�AD�����µ��ڴ滺������һ�ο��Դ�����AD��
				����ǰҪ��������������仺������
*                           
* �������  : BSP_U32 u32Num0; 
				 BSP_U32 u32Num1; 
				 BSP_VOID* psk0; 
				 BSP_VOID* psk1
*
* �������  : ��
* �� �� ֵ      : ��
* �޸ļ�¼  :2012��11��24��   �¶���  ����
			2014��1��23��v1.01 �¶��� �޸� ����K3V3����������⣬
			�޷���m3�Ͻ��е͹��Ļָ����ƶ���A9�Ͻ��С�
*****************************************************************************/
BSP_S32 BSP_IPF_ConfigUlAd(BSP_U32 u32AdType, BSP_U32  u32AdNum, IPF_AD_DESC_S * pstAdDesc)
{
	BSP_U32 u32ADQwptr = 0;
	struct tagIPF_AD_DESC_S * pstADDesc = pstAdDesc;
	BSP_U32 i;

	/* ���ģ���Ƿ��ʼ�� */
	if(IPF_MCORE_INIT_SUCCESS != g_IPFInit)
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF Not Init! \n");
		return BSP_ERR_IPF_NOT_INIT;
	}
	
#if (defined(BSP_CONFIG_HI3630))
	/*����K3V3����������⣬ʹ��ipf ǰ��Ҫ����Ƿ���Ҫ�ָ�������*/
	ipf_check_filter_restore();
#endif


	if(NULL == pstAdDesc)
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigUlAd input para ERROR! NULL == pstAdDesc\n");
		return BSP_ERR_IPF_INVALID_PARA;
	}	

	if(u32AdType >= IPF_AD_MAX)
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigUlAd input para ERROR! u32AdType >= IPF_AD_MAX\n");
		return BSP_ERR_IPF_INVALID_PARA;
	}
	
	if(IPF_AD_0 == u32AdType)
	{
		if(u32AdNum >= IPF_ULAD0_DESC_SIZE)
		{
			bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigUlAd input para ERROR! u32AdNum >=IPF_ULAD0_DESC_SIZE\n");
			g_stIPFDebugInfo->ipf_ulad0_error_count++;
			return BSP_ERR_IPF_INVALID_PARA;
		}
		
		/*����дָ��*/
		u32ADQwptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ0_WPTR_OFFSET);
		for(i=0; i < u32AdNum; i++)
		{
			if(NULL == (BSP_VOID*)(pstADDesc->u32OutPtr1))
			{
				bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigDlAd input u32OutPtr1 NULL! \n");
				return BSP_ERR_IPF_INVALID_PARA;
			}
			g_stIpfUl.pstIpfADQ0[u32ADQwptr].u32OutPtr1 = pstADDesc->u32OutPtr1;
			g_stIpfUl.pstIpfADQ0[u32ADQwptr].u32OutPtr0 = pstADDesc->u32OutPtr0;
			u32ADQwptr = ((u32ADQwptr + 1) < IPF_ULAD0_DESC_SIZE)? (u32ADQwptr + 1) : 0;	
			pstADDesc++;
		}
		g_stIPFDebugInfo->ipf_cfg_ulad0_count += u32AdNum;

		/* ����AD0дָ��*/
		ipf_writel(u32ADQwptr, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ0_WPTR_OFFSET);  
	}
	
	else if(IPF_AD_1 == u32AdType)
	{
		if(u32AdNum >= IPF_ULAD1_DESC_SIZE)
		{
			bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigUlAd input para ERROR! u32AdNum >=IPF_ULAD1_DESC_SIZE\n");
			g_stIPFDebugInfo->ipf_ulad1_error_count++;
			return BSP_ERR_IPF_INVALID_PARA;
		}
		
		/*����дָ��*/
		u32ADQwptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ1_WPTR_OFFSET);
		for(i=0; i < u32AdNum; i++)
		{
			if(NULL == (BSP_VOID*)(pstADDesc->u32OutPtr1))
			{
				bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigDlAd input u32OutPtr1 NULL! \n");
				return BSP_ERR_IPF_INVALID_PARA;
			}
			g_stIpfUl.pstIpfADQ1[u32ADQwptr].u32OutPtr1 = pstADDesc->u32OutPtr1;
			g_stIpfUl.pstIpfADQ1[u32ADQwptr].u32OutPtr0 = pstADDesc->u32OutPtr0;
			u32ADQwptr = ((u32ADQwptr + 1) < IPF_ULAD1_DESC_SIZE)? (u32ADQwptr + 1) : 0;		
			pstADDesc++;
		}
		g_stIPFDebugInfo->ipf_cfg_ulad1_count += u32AdNum;

		/* ����AD0дָ��*/
		ipf_writel(u32ADQwptr, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ1_WPTR_OFFSET);  

	}
	return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��     : BSP_IPF_ChannelEnable
*
* ��������  : �ýӿ�����ʹ�ܻ�λIP������������ͨ��
*
* �������  : IPF_CHANNEL_TYPE_E eChanType      ������ͨ����ʶ
*                           BSP_BOOL bFlag   ʹ�ܸ�λ��ʶ 
*
* �������  : ��
* �� �� ֵ      : IPF_SUCCESS                ʹ�ܸ�λ�ɹ�
*                            IPF_ERROR                  ʹ�ܸ�λʧ��
*                            BSP_ERR_IPF_INVALID_PARA   ��������
*
* �޸ļ�¼  :2011��1��11��   ³��  ����
			2014��1��23��v1.01 �¶��� �޸� ����K3V3����������⣬
			�޷���m3�Ͻ��е͹��Ļָ����ƶ���A9�Ͻ��С�
*****************************************************************************/
BSP_S32 BSP_IPF_ChannelEnable(IPF_CHANNEL_TYPE_E eChanType, BSP_BOOL bFlag)
{
    BSP_U32 u32ChanEnable = 0;
    BSP_U32 u32ChanState = 0;
    BSP_U32 u32Times = 0;
    BSP_U32 u32ChCtrl = 0;

#if (defined(BSP_CONFIG_HI3630))
    /*����K3V3����������⣬ʹ��ipf ǰ��Ҫ����Ƿ���Ҫ�ָ�������*/
    ipf_check_filter_restore();
#endif

    /* ������� */
    if(eChanType >= IPF_CHANNEL_MAX)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,
                     "\r BSP_IPF_ChannelEnable eChanType = %d ! \n",eChanType,0,0,0,0,0);
        return BSP_ERR_IPF_INVALID_PARA;
    }

    /* ͨ��ʹ�� */
    if(bFlag)
    {
        u32ChanEnable = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH_EN_OFFSET);
        u32ChanEnable |= 0x1<<eChanType;
        ipf_writel(u32ChanEnable, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH_EN_OFFSET);                        
    }
    /* ͨ��ȥʹ�� */
    else
    {
        u32ChanEnable = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH_EN_OFFSET);
        u32ChanEnable &= 0xFFFFFFFF ^ (0x1<<eChanType);
        ipf_writel(u32ChanEnable, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH_EN_OFFSET); 
        
        /* ����ֱ��ͨ��״̬Ϊ0 */      
        do
        {
            (BSP_VOID)taskDelay(1);
            u32ChanState = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_EN_STATE_OFFSET);
            u32ChanState &= 0x1<<eChanType;
            u32Times++;
        }while((u32ChanState) && (u32Times < 100)); 
        
        if(100 == u32Times)
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,
                       "\r BSP_IPF_ChannelEnable disable error u32Times = %d ! \n",u32Times,0,0,0,0,0);
            return IPF_ERROR;
        }
		
        /* ��λ������к�������еĶ�дָ�� */
        if(IPF_CHANNEL_UP == eChanType)
        {
            u32ChCtrl = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_CTRL_OFFSET);
            u32ChCtrl |= 0x30; /* ��4��5bit����Ϊ1 */
            ipf_writel(u32ChCtrl, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_CTRL_OFFSET); 
        }
        else if(IPF_CHANNEL_DOWN == eChanType)
        {
            u32ChCtrl = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_CTRL_OFFSET);
            u32ChCtrl |= 0x30; /* ��4��5bit����Ϊ1 */
            ipf_writel(u32ChCtrl, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_CTRL_OFFSET); 
        }
        else
        {
            return BSP_ERR_IPF_INVALID_PARA;
        }
    }
    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��     : BSP_IPF_GetDlBDNum
*
* ��������  : �ýӿ����ڻ�ȡ���п���BD��CD ��Ŀ
*                           BD ��Χ: 0~63      CD��Χ: 0~1023
* �������  : ��
*
* �������  : ����CD��Ŀ
* �� �� ֵ      : ����BD��Ŀ
*  
* �޸ļ�¼  :2011��11��30��   ³��  ����
			2014��1��23��v1.01 �¶��� �޸� ����K3V3����������⣬
			�޷���m3�Ͻ��е͹��Ļָ����ƶ���A9�Ͻ��С�

*****************************************************************************/
BSP_U32 BSP_IPF_GetDlBDNum(BSP_U32* pu32CDNum)
{
    BSP_U32 u32BdqDepth = 0;
    BSP_U32 u32IdleBd = 0;
    BSP_U32 u32IdleCd = 0;
    BSP_U32 u32IdleCdDown = 0;
    BSP_U32 u32IdleCdUp = 0;
	
#if (defined(BSP_CONFIG_HI3630))
    /*����K3V3����������⣬ʹ��ipf ǰ��Ҫ����Ƿ���Ҫ�ָ�������*/
    ipf_check_filter_restore();
#endif

    /* �������BD���� */
    u32BdqDepth = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_DQ_DEPTH_OFFSET);
    u32BdqDepth = u32BdqDepth & IPF_DQ_DEPTH_MASK;
    u32IdleBd = IPF_DLBD_DESC_SIZE - u32BdqDepth;
    /* �������CD ���� */
    if(*(g_stIpfDl.u32IpfCdRptr) > g_stIpfDl.u32IpfCdWptr)
    {
        u32IdleCd = *(g_stIpfDl.u32IpfCdRptr) - g_stIpfDl.u32IpfCdWptr - 1;
    }
    else
    {
        u32IdleCdUp = (*(g_stIpfDl.u32IpfCdRptr) > 1)?(*(g_stIpfDl.u32IpfCdRptr) - 1):0;
        u32IdleCdDown = IPF_DLCD_DESC_SIZE -  g_stIpfDl.u32IpfCdWptr - 1;
        u32IdleCd = (u32IdleCdUp > u32IdleCdDown)? u32IdleCdUp:u32IdleCdDown;
    }
#if 0
    /*����BDQ_RESERVE_NUM��BD���ڼ���AD���п��ж�*/	
    u32IdleBd = (u32IdleBd > BDQ_RESERVE_NUM)? (u32IdleBd - BDQ_RESERVE_NUM):0;
#endif
    g_stIpfDl.u32IdleBd = u32IdleBd;
    *pu32CDNum = u32IdleCd;

    if(0 == u32IdleBd)
	{
		g_stIPFDebugInfo->u32DlBdNotEnough++;
	}
    return u32IdleBd;
}

/*****************************************************************************
* �� �� ��     : BSP_IPF_GetDlRdNum
*
* ��������  : �ýӿ����ڶ�ȡ����RD��Ŀ
*
* �������  : ��
*   
* �������  : ��
*
* �� �� ֵ     : ����RD��Ŀ
*
* �޸ļ�¼  :2013��8��1��   chendongyue  ����
*****************************************************************************/
BSP_U32 BSP_IPF_GetDlRdNum(BSP_VOID)
{
    BSP_U32 u32RdqDepth = 0;
  
    /* ��ȡRD��� */
    u32RdqDepth = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_DQ_DEPTH_OFFSET);
    u32RdqDepth = (u32RdqDepth>>16)&IPF_DQ_DEPTH_MASK;
    return u32RdqDepth;
}


/*****************************************************************************
* �� �� ��     : BSP_IPF_GetDlDescNum
*
* ��������  : �ýӿڿɶ�ȡ���пɴ������
			���ڹ��ipfӲ����ͷ��������
*
* �������  : ��
*   
* �������  : ��
*
* �� �� ֵ     : ���пɴ������
*
* �޸ļ�¼  :2013��8��1��   chendongyue  ����
			2014��1��23��v1.01 �¶��� �޸� ����K3V3����������⣬
			�޷���m3�Ͻ��е͹��Ļָ����ƶ���A9�Ͻ��С�

*****************************************************************************/

BSP_U32 BSP_IPF_GetDlDescNum(BSP_VOID)
{
	BSP_U32 u32DlAd0Num = 0;
	BSP_U32 u32DlAd1Num = 0;
	BSP_U32 u32DlBdNum = 0;
/*	BSP_U32 u32DlRdNum = 0;*/
	BSP_U32 u32DlAdwptr = 0;
	BSP_U32 u32DlAdrptr = 0;
	BSP_U32 u32DlBdDepth = 0;
	BSP_U32 u32DlCdNum = 0;
	
#if (defined(BSP_CONFIG_HI3630))
	/*����K3V3����������⣬ʹ��ipf ǰ��Ҫ����Ƿ���Ҫ�ָ�������*/
	ipf_check_filter_restore();
#endif
				
	/* �������AD0���� */
	u32DlBdDepth = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_DQ_DEPTH_OFFSET);
	u32DlAdwptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ0_WPTR_OFFSET);
	u32DlAdrptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ0_RPTR_OFFSET);
	u32DlBdDepth = u32DlBdDepth&IPF_DQ_DEPTH_MASK;
	
	if (u32DlAdwptr >= u32DlAdrptr)/*дָ����ǰ������˳��*/
	{
		u32DlAd0Num = u32DlAdwptr - u32DlAdrptr;
	}
	else
	{
		u32DlAd0Num = IPF_ULAD0_DESC_SIZE - (u32DlAdrptr -u32DlAdwptr);
	}
	if(u32DlAd0Num > u32DlBdDepth)
	{
		u32DlAd0Num -= u32DlBdDepth;
	}
	else
	{
		u32DlAd0Num = 0;
		g_stIPFDebugInfo->u32DlAd0NotEnough++;
	}


	/* �������AD1���� */
	u32DlBdDepth = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_DQ_DEPTH_OFFSET);
	u32DlAdwptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ1_WPTR_OFFSET);
	u32DlAdrptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ1_RPTR_OFFSET);
	u32DlBdDepth = u32DlBdDepth&IPF_DQ_DEPTH_MASK;

	if (u32DlAdwptr >= u32DlAdrptr)/*дָ����ǰ������˳��*/
	{
		u32DlAd1Num = u32DlAdwptr - u32DlAdrptr;
	}
	else
	{
		u32DlAd1Num =  IPF_ULAD1_DESC_SIZE - (u32DlAdrptr -u32DlAdwptr);
	}
	
	if(u32DlAd1Num > u32DlBdDepth)
	{
		u32DlAd1Num -= u32DlBdDepth;
	}
	else
	{
		u32DlAd1Num = 0;
		g_stIPFDebugInfo->u32DlAd1NotEnough++;

	}

	u32DlBdNum = BSP_IPF_GetDlBDNum(&u32DlCdNum);
#if 0
	if(0 == u32DlBdNum)
	{
		g_stIPFDebugInfo->u32DlBdNotEnough++;
	}
#endif
	if(u32DlBdNum > u32DlAd0Num)
	{
		u32DlBdNum = u32DlAd0Num;
	}
	if(u32DlBdNum > u32DlAd1Num)
	{
		u32DlBdNum = u32DlAd1Num;
	}
	/*
	u32DlRdNum = IPF_ULRD_DESC_SIZE - BSP_IPF_GetDlRdNum();
	if(u32DlRdNum > 1)
	{
		u32DlRdNum -= 1;
	}
	else
	{
		u32DlRdNum = 0;
	}



	
	if(u32DlBdNum > u32DlRdNum)
	{
		u32DlBdNum = u32DlRdNum;
	}
	*/
       return u32DlBdNum;

}


/*****************************************************************************
* �� �� ��     : BSP_IPF_DlStateIdle
*
* ��������  : �ýӿ����ڻ�ȡ����ͨ���Ƿ�Ϊ����
*                            
* �������  : ��
*
* �������  : ��
* �� �� ֵ      : IPF_SUCCESS  ��ʾ���п��У�
*                            IPF_ERROR      ��ʾ���зǿ��У�
*  
* �޸ļ�¼  :2011��12��9��   ³��  ����
*****************************************************************************/
BSP_S32 BSP_IPF_DlStateIdle(BSP_VOID)
{
    BSP_U32 u32DlState = 0;
    BSP_U32 u32BdqWptr = 0;
    
    u32DlState = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_STATE_OFFSET);

    if(u32DlState != IPF_CHANNEL_STATE_IDLE)
    {
        return IPF_ERROR;
    }

    u32BdqWptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_BDQ_WPTR_OFFSET);/* [false alarm]:fortify disable */

    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��     : BSP_IPF_RegisterWakeupUlCb
*
* ��������  : �ýӿ�����ע������PS����ص�����
*                           
* �������  : BSP_IPF_WakeupUlCb *pFnWakeupUl
*
* �������  : ��
* �� �� ֵ      : ��
*  
* �޸ļ�¼  :2011��11��30��   ³��  ����
*****************************************************************************/
BSP_S32 BSP_IPF_RegisterWakeupUlCb(BSP_IPF_WakeupUlCb pFnWakeupUl)
{
    /* ������� */
    if(NULL == pFnWakeupUl)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_RegisterWakeupUlCb inputPara ERROR! \n");
        return BSP_ERR_IPF_INVALID_PARA;
    }
    g_stIpfUl.pFnUlIntCb = pFnWakeupUl;
    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��     : BSP_IPF_RegisterAdqEmptyUlCb
*
* ��������  : �˽ӿ�ֻ��C���ṩ������ע�ỽ������PS��
                             ADQ���пջص�����
*                           
* �������  : BSP_IPF_AdqEmptyCb pFnWakeupUl
*
* �������  : ��
* �� �� ֵ      : IPF_SUCCESS ע��ɹ�
*                            IPF_ERROR	ע��ʧ��
* �޸ļ�¼  :2012��11��24��   �¶���  ����
*****************************************************************************/
BSP_S32 BSP_IPF_RegisterAdqEmptyUlCb(BSP_IPF_AdqEmptyUlCb pAdqEmptyUl)
{
    /* ������� */
    if(NULL == pAdqEmptyUl)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_RegisterAdqEmptyUlCb inputPara ERROR! \n");
        return IPF_ERROR;
    }
    g_stIpfUl.pAdqEmptyUlCb = pAdqEmptyUl;
    return IPF_SUCCESS;
}


/*****************************************************************************
* �� �� ��     : BSP_IPF_GetUlRd
*
* ��������  : �ýӿ����ڶ�ȡ����BD, ֧��һ�ζ�ȡ���BD
*
* �������  : BSP_U32* pu32Num    
*                           IPF_RD_DESC_S *pstRd
*   
* �������  : BSP_U32* pu32Num    ʵ�ʶ�ȡ��RD��Ŀ
*
* �� �� ֵ     : IPF_SUCCESS               �����ɹ�
*                           IPF_ERROR                   ����ʧ��
*
* �޸ļ�¼  :2011��11��30��   ³��  ����
*****************************************************************************/
BSP_VOID BSP_IPF_GetUlRd(BSP_U32* pu32Num, IPF_RD_DESC_S *pstRd)
{
    BSP_U32 u32RdqRptr = 0;
    BSP_U32 u32RdqDepth = 0;
    BSP_U32 u32Num = 0;
    BSP_U32 i = 0;
    BSP_U32 u32TimeStampEn;
	
    /* ��ȡRD��� */
    u32RdqDepth = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_DQ_DEPTH_OFFSET);
    u32RdqDepth = (u32RdqDepth>>16)&IPF_DQ_DEPTH_MASK;
    u32Num = (u32RdqDepth < *pu32Num)?u32RdqDepth:*pu32Num;

    if(0 == u32Num)
    {
        *pu32Num = 0;
        return;
    }

    u32TimeStampEn = g_stIPFDebugInfo->ipf_timestamp_ul_en;

    /* ��ȡRD��ָ�� */
    u32RdqRptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_RDQ_RPTR_OFFSET);
    for(i = 0; i < u32Num; i++)
    {        
        /* ��ȡRD */
        pstRd[i].u16Attribute = g_stIpfUl.pstIpfRDQ[u32RdqRptr].u16Attribute;
        pstRd[i].u16PktLen = g_stIpfUl.pstIpfRDQ[u32RdqRptr].u16PktLen;
        pstRd[i].u16Result = g_stIpfUl.pstIpfRDQ[u32RdqRptr].u16Result;
        pstRd[i].u32InPtr = g_stIpfUl.pstIpfRDQ[u32RdqRptr].u32InPtr;
        pstRd[i].u32OutPtr = g_stIpfUl.pstIpfRDQ[u32RdqRptr].u32OutPtr;
        pstRd[i].u16UsrField1 = g_stIpfUl.pstIpfRDQ[u32RdqRptr].u16UsrField1;
        pstRd[i].u32UsrField2 = g_stIpfUl.pstIpfRDQ[u32RdqRptr].u32UsrField2;
        pstRd[i].u32UsrField3 = g_stIpfUl.pstIpfRDQ[u32RdqRptr].u32UsrField3;
        ipf_record_end_time_stamp(u32TimeStampEn, g_stIpfUl.pstIpfRDQ[u32RdqRptr].u32UsrField3);
				
        /* ����RD��ָ�� */
        u32RdqRptr = ((u32RdqRptr+1) < IPF_ULRD_DESC_SIZE)?(u32RdqRptr+1):0;        
    }
    ipf_writel(u32RdqRptr, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_RDQ_RPTR_OFFSET);
    #ifdef __BSP_IPF_DEBUG__
    if(g_stIPFDebugInfo->u32IpfDebug)
    {
        IPF_PRINT("out u32RdqRptr = %d\n", u32RdqRptr);
    }
    #endif
    *pu32Num = u32Num;
    g_stIPFDebugInfo->ipf_get_ulrd_count += u32Num;
}


/*****************************************************************************
* �� �� ��       : BSP_IPF_GetUlAdNum
*
* ��������  : �ýӿ�ֻ��C���ṩ����ȡ���У�C�ˣ�����AD��Ŀ
*
* �������  :BSP_OK����������
                            BSP_ERROR������
*             
* �������  : ��
* �� �� ֵ     : ��
* �޸ļ�¼  : 2011��11��24��   �¶���  ����
			2014��1��23��v1.01 �¶��� �޸� ����K3V3����������⣬
			�޷���m3�Ͻ��е͹��Ļָ����ƶ���A9�Ͻ��С�
*****************************************************************************/
BSP_S32 BSP_IPF_GetUlAdNum(BSP_U32* pu32AD0Num,BSP_U32* pu32AD1Num)
{
	BSP_U32 u32UlAdDepth = 0;
	BSP_U32 u32UlAdwptr = 0;
	BSP_U32 u32UlAdrptr = 0;

#if (defined(BSP_CONFIG_HI3630))
    /*����K3V3����������⣬ʹ��ipf ǰ��Ҫ����Ƿ���Ҫ�ָ�������*/
    ipf_check_filter_restore();
#endif

	/*��μ��*/
	if((NULL == pu32AD0Num)||(NULL == pu32AD1Num))
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_GetDlAdNum pstCtrl NULL! \n");
		return BSP_ERR_IPF_INVALID_PARA;
	}

	/* �������AD���� */
	u32UlAdwptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ0_WPTR_OFFSET);
	u32UlAdrptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ0_RPTR_OFFSET);
	if (u32UlAdwptr >= u32UlAdrptr)/*дָ����ǰ������˳��*/
	{
		u32UlAdDepth = IPF_ULAD0_DESC_SIZE - (u32UlAdwptr - u32UlAdrptr);
	}
	else
	{
		u32UlAdDepth =  u32UlAdrptr -u32UlAdwptr;
	}
	*pu32AD0Num = u32UlAdDepth;

	/*�۳�reserve ad�����ڷ�ֹӲ����ad������ʶ��ɿպ͵͹��������ڴ�й¶*/	
	if(u32UlAdDepth > IPF_ADQ_RESERVE_NUM)
	{
		*pu32AD0Num = u32UlAdDepth - IPF_ADQ_RESERVE_NUM;
	}
	else
	{
		*pu32AD0Num = 0;
	}

	u32UlAdwptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ1_WPTR_OFFSET);
	u32UlAdrptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ1_RPTR_OFFSET);
	if (u32UlAdwptr >= u32UlAdrptr)/*дָ����ǰ������˳��*/
	{
		u32UlAdDepth = IPF_ULAD1_DESC_SIZE - (u32UlAdwptr - u32UlAdrptr);
	}
	else
	{
		u32UlAdDepth =  u32UlAdrptr - u32UlAdwptr;
	}
	
	/*�۳�reserve ad�����ڷ�ֹӲ����ad������ʶ��ɿպ͵͹��������ڴ�й¶*/
	if(u32UlAdDepth > IPF_ADQ_RESERVE_NUM)
	{
		*pu32AD1Num = u32UlAdDepth - IPF_ADQ_RESERVE_NUM;
	}
	else
	{
		*pu32AD1Num = 0;
	}

	return IPF_SUCCESS;
}


/*****************************************************************************
* �� �� ��     : BSP_IPF_GetUlRdNum
*
* ��������  : �ýӿ����ڶ�ȡ����RD��Ŀ
*
* �������  : ��
*   
* �������  : ��
*
* �� �� ֵ     : ����RD��Ŀ
*
* �޸ļ�¼  :2012��2��16��   ³��  ����
*****************************************************************************/
BSP_U32 BSP_IPF_GetUlRdNum(BSP_VOID)
{
    BSP_U32 u32RdqDepth = 0;
  
    /* ��ȡRD��� */
    u32RdqDepth = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_DQ_DEPTH_OFFSET);
    u32RdqDepth = (u32RdqDepth>>16)&IPF_DQ_DEPTH_MASK;
    return u32RdqDepth;
}

/*****************************************************************************
* �� �� ��       : BSP_IPF_SwitchInt
*
* ��������  : �ṩ�ĵ͹��Ľӿڣ� ���ϱ���A�˵��ж�ͬʱ�ϱ���C ��
*
* �������  : BSP_TRUE   �ϱ���C��
*                           BSP_FALSE  ���ϱ���C��
*             
* �������  : ��
* �� �� ֵ     : ��
* �޸ļ�¼  : 2011��2��14��   ³��  ����
*****************************************************************************/
BSP_VOID BSP_IPF_SwitchInt(BSP_BOOL bFlag)
{
	BSP_U32 u32IntMask0 = 0;

	if(bFlag == BSP_TRUE)
	{
    	u32IntMask0 = IPF_INT_OPEN0 | IPF_INT_OPEN1;
		g_u32IpfSwitchInt = 1;
	}
	else
	{
		u32IntMask0 = IPF_INT_OPEN0;
		g_u32IpfSwitchInt = 0;
	}
    ipf_writel(u32IntMask0, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_INT_MASK0_OFFSET);
}

/*****************************************************************************
* �� �� ��  : BSP_IPF_UseFilterInfo
*
* ��������  : �ṩ��debug�ӿڣ���ȡ�Ѿ�ʹ�õ�filter��
*
* �������  : IPF_CHANNEL_TYPE_E eChnType  ͨ������
*             
* �������  : ��
* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2011��1��11��   ³��  ����
                             2011��3��30��   ³��  �޸�
*****************************************************************************/
BSP_VOID BSP_IPF_UseFilterInfo(IPF_FILTER_CHAIN_TYPE_E eFilterChainhead)
{
    IPF_ID_S* current = NULL;
    BSP_U32 u32FilterNum = 0;
/*    BSP_S32 s32GetChar = 0;*/
    
    u32FilterNum = g_stIPFFilterInfo[eFilterChainhead].u32FilterNum;
    current = g_stIPFFilterInfo[eFilterChainhead].pstUseList;
	
    IPF_PRINT("Total Filter Num = %u\n",u32FilterNum);            
    IPF_PRINT("=============================\n");

    while(u32FilterNum)
    { 
        while(u32FilterNum)
        {
            if(1 == u32FilterNum)
            {
                IPF_PRINT("Filter ID = %d,    PS ID = %d\n    ,Next HWID = %d\n",current->u32FilterID, current->u32PsID, current->pstNext->u32FilterID);            
                u32FilterNum--;
            }
            else
            {
                IPF_PRINT("Filter ID = %d,    PS ID = %d\n    ,Next HWID = %d\n",current->u32FilterID, current->u32PsID, current->pstNext->u32FilterID);            
                current = current->pstNext;
                u32FilterNum--;
            }
        }
    }
    IPF_PRINT("*****************************\n");

}

/*****************************************************************************
* �� �� ��  : BSP_IPF_FreeFilterInfo
*
* ��������  : �ṩ��debug�ӿڣ���ȡδʹ�õ�filter��
*
* �������  : IPF_CHANNEL_TYPE_E eChnType  ͨ������
*             
* �������  : ��
* �� �� ֵ  : ��
*
* �޸ļ�¼  : 2011��1��11��   ³��  ����
*
*****************************************************************************/
BSP_VOID BSP_IPF_BasicFreeFilterInfo(BSP_VOID)
{
    IPF_ID_S* current = NULL;
/*    BSP_S32 s32GetChar = 0;*/

    current = g_stIPFBasicFreeList;

    IPF_PRINT("============================\n");

    while(current != NULL)
    {
        while(current != NULL)
        {
            IPF_PRINT("Filter ID:      %d\n",current->u32FilterID);
            current = current->pstNext; 
        }
    }
    IPF_PRINT("*****************************\n");    
}
BSP_VOID BSP_IPF_ExtFreeFilterInfo(BSP_VOID)
{
    IPF_ID_S* current = NULL;
/*    BSP_S32 s32GetChar = 0;*/

    current = g_stIPFExtFreeList;

    IPF_PRINT("============================\n");

    while(current != NULL)
    {
        while(current != NULL)
        {
            IPF_PRINT("Filter ID:      %d\n",current->u32FilterID);
            current = current->pstNext; 
        }
    }
    IPF_PRINT("*****************************\n");    
}

BSP_VOID BSP_IPF_FilterInfoHWID(BSP_U32 u32Num)
{
    IPF_MATCH_INFO_S stMatchInfo; 
    
    if(u32Num < IPF_BF_NUM)
    {
         /* д���˱������ַ */    
        ipf_writel(u32Num, HI_IPF_REGBASE_ADDR_VIRT + HI_BFLT_INDEX_OFFSET); 
         
        /* ���������������� */
        memcpy(&stMatchInfo, (BSP_VOID*)(HI_IPF_REGBASE_ADDR_VIRT+HI_FLT_LOCAL_ADDR0_OFFSET), sizeof(IPF_MATCH_INFO_S));/* [false alarm]:fortify disable */
    }
    else
    {
        memcpy(&stMatchInfo, (BSP_VOID*)(g_pstExFilterAddr + u32Num - IPF_BF_NUM), sizeof(IPF_MATCH_INFO_S));
    }

    IPF_PRINT("============================\n");
    IPF_PRINT("src ADDR0 : %x.%x.%x.%x\n", stMatchInfo.u8SrcAddr[0], stMatchInfo.u8SrcAddr[1], stMatchInfo.u8SrcAddr[2], stMatchInfo.u8SrcAddr[3]);
    IPF_PRINT("src ADDR1 : %x.%x.%x.%x\n", stMatchInfo.u8SrcAddr[4], stMatchInfo.u8SrcAddr[5], stMatchInfo.u8SrcAddr[6], stMatchInfo.u8SrcAddr[7]);
    IPF_PRINT("src ADDR2 : %x.%x.%x.%x\n", stMatchInfo.u8SrcAddr[8], stMatchInfo.u8SrcAddr[9], stMatchInfo.u8SrcAddr[10], stMatchInfo.u8SrcAddr[11]);
    IPF_PRINT("src ADDR3 : %x.%x.%x.%x\n", stMatchInfo.u8SrcAddr[12], stMatchInfo.u8SrcAddr[13], stMatchInfo.u8SrcAddr[14], stMatchInfo.u8SrcAddr[15]);
    IPF_PRINT("dst ADDR0 : %x.%x.%x.%x\n", stMatchInfo.u8DstAddr[0], stMatchInfo.u8DstAddr[1], stMatchInfo.u8DstAddr[2], stMatchInfo.u8DstAddr[3]);
    IPF_PRINT("dst ADDR1 : %x.%x.%x.%x\n", stMatchInfo.u8DstAddr[4], stMatchInfo.u8DstAddr[5], stMatchInfo.u8DstAddr[6], stMatchInfo.u8DstAddr[7]);
    IPF_PRINT("dst ADDR2 : %x.%x.%x.%x\n", stMatchInfo.u8DstAddr[8], stMatchInfo.u8DstAddr[9], stMatchInfo.u8DstAddr[10], stMatchInfo.u8DstAddr[11]);
    IPF_PRINT("dst ADDR3 : %x.%x.%x.%x\n", stMatchInfo.u8DstAddr[12], stMatchInfo.u8DstAddr[13], stMatchInfo.u8DstAddr[14], stMatchInfo.u8DstAddr[15]);
    IPF_PRINT("SrcPortLo : %d  SrcPortHi: %d\n", stMatchInfo.unSrcPort.Bits.u16SrcPortLo, stMatchInfo.unSrcPort.Bits.u16SrcPortHi);
    IPF_PRINT("DstPortLo : %d  DstPortHi: %d\n", stMatchInfo.unDstPort.Bits.u16DstPortLo, stMatchInfo.unDstPort.Bits.u16DstPortHi);
    IPF_PRINT("TrafficClass :      %d\n", stMatchInfo.unTrafficClass.u32TrafficClass);
    IPF_PRINT("TrafficClassMsk :  %d\n", stMatchInfo.u32LocalAddressMsk);
    IPF_PRINT("Protocol :          %d\n", stMatchInfo.unNextHeader.u32Protocol);
    IPF_PRINT("FlowLable :         %d\n", stMatchInfo.u32FlowLable);
    IPF_PRINT("Type :        %d  Code: %d\n", stMatchInfo.unFltCodeType.Bits.u16Type, stMatchInfo.unFltCodeType.Bits.u16Code);
    IPF_PRINT("NextIndex : %d  FltPri: %d\n", stMatchInfo.unFltChain.Bits.u16NextIndex, stMatchInfo.unFltChain.Bits.u16FltPri);
    IPF_PRINT("FltSpi :             %d\n", stMatchInfo.u32FltSpi);
    IPF_PRINT("FltRuleCtrl :        %x\n", stMatchInfo.unFltRuleCtrl.u32FltRuleCtrl);    
    IPF_PRINT("============================\n");
}

BSP_VOID BSP_IPF_FilterInfoPSID(IPF_FILTER_CHAIN_TYPE_E eFilterChainhead, BSP_U32 u32Num)
{
    BSP_U32 u32FindID = 0;
    
    /* ����PS ID ��Ӧ��FilterID*/
    if(IPF_FindFilterID(eFilterChainhead, u32Num, &u32FindID) != IPF_SUCCESS)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF, "\r cannot find filter ID! \n");
        return ;
    }
    IPF_PRINT("HWID is %d",u32FindID);
    BSP_IPF_FilterInfoHWID(u32FindID);
}

/*****************************************************************************
* �� �� ��     : BSP_IPF_BackupFilter
*
* ��������  : ���ݻ�����������DDR
*
* �������  : ������ID
*   
* �������  : ��
*
* �� �� ֵ     : ��
*
* �޸ļ�¼  :2013��5��16��   �¶���  ����
*****************************************************************************/
BSP_VOID BSP_IPF_BackupFilter(BSP_U32 u32FilterID)
{
#if 0
    IPF_MATCH_INFO_S * pstFilterMatchInfo = (IPF_MATCH_INFO_S *)IPF_PWRCTL_BASIC_FILTER_ADDR;
    pstFilterMatchInfo += u32FilterID;
    memcpy((BSP_VOID*)pstFilterMatchInfo, (BSP_VOID*)(HI_IPF_REGBASE_ADDR_VIRT+HI_FLT_LOCAL_ADDR0_OFFSET), sizeof(IPF_MATCH_INFO_S));
#else
    BSP_U32 j;	
    BSP_U32* match_info_addr = (u32 *)(IPF_PWRCTL_BASIC_FILTER_ADDR + u32FilterID*sizeof(IPF_MATCH_INFO_S));

    for(j=0; j<(sizeof(IPF_MATCH_INFO_S)/4); j++)
    {
        *match_info_addr = ipf_readl((HI_IPF_REGBASE_ADDR_VIRT+HI_FLT_LOCAL_ADDR0_OFFSET+j*4)); 
        match_info_addr++;

    }
#endif
}

#ifdef CONFIG_CCORE_PM

/*****************************************************************************
* �� �� ��     : ipf_ul_dpm_prepare
*
* ��������  : dpm����׼������
*
* �������  : �豸ָ��
*   
* �������  : ��
*
* �� �� ֵ     : IPF_ERROR ʧ��
                            IPF_SUCCESS �ɹ�
*
* �޸ļ�¼  :2013��6��16��   �¶���  ����
*****************************************************************************/
static int ipf_ul_dpm_prepare(struct dpm_device *dev)
{
    u32 u32_ul_state;
#if 0
    u32 u32_adq0_wptr;
    u32 u32_adq0_rptr;
    u32 u32_adq1_wptr;
    u32 u32_adq1_rptr;
#endif
    /* �ж�����IPF�Ƿ���� */
    u32_ul_state = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_STATE_OFFSET);

    if(u32_ul_state != IPF_CHANNEL_STATE_IDLE)
    {
        /*bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF CHANNEL NOT IDLE! \n");*/
        return IPF_ERROR;
    }
    #if 0
    /*�ȴ�ad���жϴ������*/
    u32_adq0_wptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ0_WPTR_OFFSET);
    u32_adq0_rptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ0_RPTR_OFFSET);
    u32_adq1_wptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ1_WPTR_OFFSET);
    u32_adq1_rptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ1_RPTR_OFFSET);
    if((u32_adq0_rptr == u32_adq0_wptr)||(u32_adq1_rptr == u32_adq1_wptr))
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF UL AD EMPTY INTRRUPT PROCESSING! \n");
        return IPF_ERROR;
    }
    #endif

    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��     : ipf_ul_suspend
*
* ��������  : dpm׮����
*
* �������  : �豸ָ��
*   
* �������  : ��
*
* �� �� ֵ     : �ɹ�
*
* �޸ļ�¼  :2013��6��16��   �¶���  ����
			2014��1��23��v1.01 �¶��� �޸� ����K3V3����������⣬
			�޷���m3�Ͻ��е͹��Ļָ����ƶ���A9�Ͻ��С�
*****************************************************************************/
static int ipf_ul_suspend(struct dpm_device *dev)
{
#if (defined(BSP_CONFIG_HI3630))

    unsigned long ipf_flags = 0;
	
    spin_lock_irqsave(&ipf_filter_spinlock, ipf_flags);
    bsp_ipc_spin_lock(IPC_SEM_IPF_PWCTRL);
    *ipf_ccore_init_status = IPF_PWC_DOWN;
    cache_sync();		
    bsp_ipc_spin_unlock(IPC_SEM_IPF_PWCTRL);
    spin_unlock_irqrestore(&ipf_filter_spinlock, ipf_flags);

#endif
    g_stIPFDebugInfo->ipf_ccore_suspend_count++;
	return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��     : ipf_ul_resume
*
* ��������  : dpm׮����
*
* �������  : �豸ָ��
*   
* �������  : ��
*
* �� �� ֵ     : �ɹ�
*
* �޸ļ�¼  :2013��6��16��   �¶���  ����
*****************************************************************************/
static int ipf_ul_resume(struct dpm_device *dev)
{
    g_stIPFDebugInfo->ipf_ccore_resume_count++;
	return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��     : ipf_ul_dpm_complete
*
* ��������  : dpm׮����
*
* �������  : �豸ָ��
*   
* �������  : ��
*
* �� �� ֵ     : �ɹ�
*
* �޸ļ�¼  :2013��6��16��   �¶���  ����
*****************************************************************************/
static int ipf_ul_dpm_complete(struct dpm_device *dev)
{
	return IPF_SUCCESS;
}
#endif
#endif

#ifdef __KERNEL__


/*****************************************************************************
* �� �� ��     : BSP_IPF_DlRegReInit
*
* ��������  : Ccore��λʱ,IPF����֮��λ,��Ĵ�����Ϣ��ȫ����ʧ��
				  �ú�����ccore��λ���⸴λ�������ɹ���,
				  ��ADS�ص����������ȵ���,������������IPF����ͨ����ؼĴ���

*
* �������  : ��
*   
* �������  : ��
*
* �� �� ֵ     : ��
*
* �޸ļ�¼  :2013��9��1��   �¶�������
*****************************************************************************/
BSP_VOID BSP_IPF_DlRegReInit(BSP_VOID)
{
	/*����IPF����ͨ���Ĵ���*/
    BSP_U32 u32BDSize[IPF_CHANNEL_MAX] = {IPF_ULBD_DESC_SIZE, IPF_DLBD_DESC_SIZE};
    BSP_U32 u32RDSize[IPF_CHANNEL_MAX] = {IPF_ULRD_DESC_SIZE, IPF_DLRD_DESC_SIZE};
    BSP_U32 u32ADCtrl[IPF_CHANNEL_MAX] = {IPF_ADQ_DEFAULT_SEETING,IPF_ADQ_DEFAULT_SEETING};
    memset((BSP_VOID*)IPF_DLBD_MEM_ADDR, 0x0, IPF_DLDESC_SIZE);
	
    g_stIpfUl.pstIpfBDQ = (IPF_BD_DESC_S*)IPF_ULBD_MEM_ADDR;
    g_stIpfUl.pstIpfRDQ = (IPF_RD_DESC_S*)IPF_ULRD_MEM_ADDR;
    g_stIpfUl.pstIpfADQ0 = (IPF_AD_DESC_S*)IPF_ULAD0_MEM_ADDR;
    g_stIpfUl.pstIpfADQ1 = (IPF_AD_DESC_S*)IPF_ULAD1_MEM_ADDR;
    g_stIpfUl.pu32IdleBd = (BSP_U32*)IPF_ULBD_IDLENUM_ADDR;

    g_stIpfDl.pstIpfBDQ = (IPF_BD_DESC_S*)IPF_DLBD_MEM_ADDR;
    g_stIpfDl.pstIpfRDQ = (IPF_RD_DESC_S*)IPF_DLRD_MEM_ADDR;
    g_stIpfDl.pstIpfADQ0 = (IPF_AD_DESC_S*)IPF_DLAD0_MEM_ADDR;
    g_stIpfDl.pstIpfADQ1 = (IPF_AD_DESC_S*)IPF_DLAD1_MEM_ADDR;
    g_stIpfDl.pstIpfCDQ = (IPF_CD_DESC_S*)IPF_DLCD_MEM_ADDR;

    g_stIpfDl.pstIpfPhyBDQ = (IPF_BD_DESC_S*)(IPF_DLBD_MEM_ADDR - DDR_SHARED_MEM_VIRT_ADDR + DDR_SHARED_MEM_ADDR);
    g_stIpfDl.pstIpfPhyRDQ = (IPF_RD_DESC_S*)(IPF_DLRD_MEM_ADDR - DDR_SHARED_MEM_VIRT_ADDR + DDR_SHARED_MEM_ADDR);
    g_stIpfDl.pstIpfPhyADQ0 = (IPF_AD_DESC_S*)(IPF_DLAD0_MEM_ADDR - DDR_SHARED_MEM_VIRT_ADDR + DDR_SHARED_MEM_ADDR);
    g_stIpfDl.pstIpfPhyADQ1 = (IPF_AD_DESC_S*)(IPF_DLAD1_MEM_ADDR - DDR_SHARED_MEM_VIRT_ADDR + DDR_SHARED_MEM_ADDR);
	
    g_stIpfDl.u32IpfCdRptr = (BSP_U32*) IPF_DLCDRPTR_MEM_ADDR;
    *(g_stIpfDl.u32IpfCdRptr) = 0;

    g_stIPFDebugInfo = (IPF_DEBUG_INFO_S*)IPF_DEBUG_INFO_ADDR;
	
    /* ��������ͨ����AD��BD��RD��� */
    ipf_writel(u32BDSize[IPF_CHANNEL_DOWN]-1, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_BDQ_SIZE_OFFSET);
    ipf_writel(u32RDSize[IPF_CHANNEL_DOWN]-1, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_RDQ_SIZE_OFFSET);
    ipf_writel(u32ADCtrl[IPF_CHANNEL_DOWN], HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ_CTRL_OFFSET);

    /*����ͨ����BD��RD��ʼ��ַ*/    
    ipf_writel((BSP_U32)g_stIpfDl.pstIpfPhyBDQ, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_BDQ_BADDR_OFFSET);
    ipf_writel((BSP_U32)g_stIpfDl.pstIpfPhyRDQ, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_RDQ_BADDR_OFFSET);
    ipf_writel((BSP_U32)g_stIpfDl.pstIpfPhyADQ0, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ0_BASE_OFFSET);
    ipf_writel((BSP_U32)g_stIpfDl.pstIpfPhyADQ1, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ1_BASE_OFFSET);

    g_IPFInit = IPF_ACORE_INIT_SUCCESS;

    bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"[IPF]  ipf dl register reinit success\n");

    return;

}

/*****************************************************************************
* �� �� ��     : BSP_IPF_GetUsedDlAd
*
* ��������  : ����ͬV9R1��ͬ�����������ڻ�ȡ���ø�AD���еģ�
				  ����δ��Ӳ��ʹ�õ�AD��Ϣ���������ͷŻ�ȡ��AD
				  �ж�Ӧ��skb(IMM_Zc)���Է�ֹCcore resetʱ�ڴ�й¶
				  ���øú����ڼ䣬Ӧ��ֹ����BD��
				  �ú�������ʱ��ر�ͨ��

* �������  : eAdType: AD��������
*   
* �������  : pu32AdNum: ��Ҫ�ͷŵ�AD��Ŀ
				  pstAdDesc: ��Ҫ�ͷŵ�AD����ͷָ��
*
* �� �� ֵ     : BSP_ERR_IPF_INVALID_PARA ��ηǷ�
				  IPF_ERROR ʧ��
*				  IPF_SUCCESS �ɹ�
* �޸ļ�¼  :2013��9��1��   �¶�������
*****************************************************************************/
BSP_S32 BSP_IPF_GetUsedDlAd(IPF_AD_TYPE_E eAdType, BSP_U32 * pu32AdNum, IPF_AD_DESC_S * pstAdDesc)
{
	BSP_U32 u32Timeout = 10;
	BSP_U32 u32DlStateValue;
	BSP_U32 u32ChanEnable;
	BSP_U32 u32AdStateValue;
	BSP_U32 u32FreeAdNum = 0;
	BSP_U32 u32ADQwptr;
	BSP_U32 u32ADQrptr;
	BSP_U32 u32ADCtrl;
#ifndef CONFIG_IPF_AD_RPRT_FIX		
	BSP_U32 u32RptrOffsetValue = 0;
#endif
	/*�ر�����AD���ýӿ�*/
	g_IPFInit = 0;

	/*��μ��*/
	if((NULL == pu32AdNum)||(NULL == pstAdDesc))
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigDlAd input para ERROR!NULL == pu32AdNum or NULL == pstAdDesc\n");
		return BSP_ERR_IPF_INVALID_PARA;
	}	
		
	/*�ȴ�ͨ��idle ,200ms��ʱ*/
	do
	{
		msleep(20);
		u32DlStateValue = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_STATE_OFFSET);

		if(u32DlStateValue == IPF_CHANNEL_STATE_IDLE)
		{
			break;
		}
		
	}while(--u32Timeout);
	if (!u32Timeout) 
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r After 20ms IPF dl channel still on, unable to free AD \n");
		return IPF_ERROR;
	}
	/*���Թر�����ͨ��*/
	u32ChanEnable = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH_EN_OFFSET);
	u32ChanEnable &= 0xFFFFFFFF ^ (0x1<<IPF_CHANNEL_DOWN);
	ipf_writel(u32ChanEnable, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH_EN_OFFSET); 

	u32ADCtrl = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ_CTRL_OFFSET);
	u32ADCtrl &= IPF_ADQ_EN_MASK;
	u32ADCtrl |= (IPF_NO_ADQ);

	/*�ر�AD�����ڷ�ֹ����ADQԤȡ*/
	ipf_writel(u32ADCtrl, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ_CTRL_OFFSET);
	if(IPF_AD_0 == eAdType)
	{
		u32AdStateValue = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ0_STAT_OFFSET);
		/*����AD��ָ��*/
		u32ADQwptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ0_WPTR_OFFSET);
		u32ADQrptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ0_RPTR_OFFSET);
#ifndef CONFIG_IPF_AD_RPRT_FIX		
		if(u32AdStateValue & IPF_ADQ_BUF_EPT_MASK)
		{
			u32RptrOffsetValue = 0; 
		}
		else if(u32AdStateValue & IPF_ADQ_BUF_FULL_MASK)
		{
			u32RptrOffsetValue = 2; 
		}
		else
		{
			u32RptrOffsetValue = 1;    
		}
		if(u32ADQrptr >= u32RptrOffsetValue)
		{
			u32ADQrptr = u32ADQrptr - u32RptrOffsetValue;
		}
		else
		{
			u32ADQrptr = IPF_DLAD0_DESC_SIZE + u32ADQrptr - u32RptrOffsetValue;
		}
#endif
		while(u32ADQrptr != u32ADQwptr)
		{
			pstAdDesc->u32OutPtr1 = g_stIpfDl.pstIpfADQ0[u32ADQrptr].u32OutPtr1;
			pstAdDesc->u32OutPtr0 = g_stIpfDl.pstIpfADQ0[u32ADQrptr].u32OutPtr0;
//			printk("AD0[%u]OutPtr1 = 0x%x \n", u32ADQrptr, pstAdDesc->u32OutPtr1);
			u32ADQrptr = ((u32ADQrptr + 1) < IPF_DLAD0_DESC_SIZE)? (u32ADQrptr + 1) : 0;	
			pstAdDesc++;
			u32FreeAdNum++;
		}
	}
	else if(IPF_AD_1 == eAdType)
	{
		u32AdStateValue = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ1_STAT_OFFSET);
		/*����AD��ָ��*/
		u32ADQwptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ1_WPTR_OFFSET);
		u32ADQrptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ1_RPTR_OFFSET);
#ifndef CONFIG_IPF_AD_RPRT_FIX		
		if(u32AdStateValue & IPF_ADQ_BUF_EPT_MASK)
		{
			u32RptrOffsetValue = 0; 
		}
		else if(u32AdStateValue & IPF_ADQ_BUF_FULL_MASK)
		{
			u32RptrOffsetValue = 2; 
		}
		else
		{
			u32RptrOffsetValue = 1;    
		}
		if(u32ADQrptr >= u32RptrOffsetValue)
		{
			u32ADQrptr = u32ADQrptr - u32RptrOffsetValue;
		}
		else
		{
			u32ADQrptr = IPF_DLAD1_DESC_SIZE + u32ADQrptr - u32RptrOffsetValue;
		}
#endif

		while(u32ADQrptr != u32ADQwptr)
		{
			pstAdDesc->u32OutPtr1 = g_stIpfDl.pstIpfADQ1[u32ADQrptr].u32OutPtr1;
			pstAdDesc->u32OutPtr0 = g_stIpfDl.pstIpfADQ1[u32ADQrptr].u32OutPtr0;
//			printk("AD1[%u]OutPtr1 = 0x%x", u32ADQrptr, pstAdDesc->u32OutPtr1);
			u32ADQrptr = ((u32ADQrptr + 1) < IPF_DLAD1_DESC_SIZE)? (u32ADQrptr + 1) : 0;	
			pstAdDesc++;
			u32FreeAdNum++;
		}
	}
	else
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigDlAd input para ERROR! u32AdType >= IPF_AD_MAX\n");
		return BSP_ERR_IPF_INVALID_PARA;
	}
	/*����AD*/
	*pu32AdNum = u32FreeAdNum;
	return IPF_SUCCESS;
	
}



/*****************************************************************************
* �� �� ��     : BSP_IPF_SetControlFLagForCcoreReset
*
* ��������  : modem������λipf���亯���������ڸ�λʱ��ֹ��������
*
* �������  : ��
*   
* �������  : ��
*
* �� �� ֵ     : �ɹ�
*
* �޸ļ�¼  :	2013��4��19��   ¬��ʤ����
					2013��6��16��   �¶������䵽V7R2
*****************************************************************************/
BSP_VOID BSP_IPF_SetControlFLagForCcoreReset(IPF_FORREST_CONTROL_E eResetFlag)
{
    if(eResetFlag >= IPF_FORRESET_CONTROL_MAX)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_SetControlFLagForCcoreReset eResetFlag overflow! \n");
        return;
    }
    /*���ñ�־����ֹ��������*/
    g_eCcoreResetFlag = eResetFlag;
#if (defined(BSP_CONFIG_HI3630))
#ifdef CONFIG_BALONG_MODEM_RESET	
    /*���ñ�־����ֹ��������*/
    *modem_reset_flag = eResetFlag;
#endif
#endif
    cache_sync();			
}

/*****************************************************************************
* �� �� ��     : BSP_IPF_GetControlFLagForCcoreReset
*
* ��������  : modem������λipf���亯���������ڸ�λʱ��ֹ��������
*
* �������  : ��
*   
* �������  : ��
*
* �� �� ֵ     : �ɹ�
*
* �޸ļ�¼  :2013��6��16��   �¶���  ����
*****************************************************************************/
static IPF_FORREST_CONTROL_E BSP_IPF_GetControlFLagForCcoreReset(BSP_VOID)
{
    return g_eCcoreResetFlag;
}

#ifdef CONFIG_BALONG_MODEM_RESET	
int bsp_ipf_reset_ccore_cb(DRV_RESET_CALLCBFUN_MOMENT eparam, int userdata)
{
	BSP_U32 u32Timeout = 2;
	BSP_U32 u32DlStateValue;
	if(DRV_RESET_CALLCBFUN_RESET_BEFORE == eparam)
	{
		BSP_IPF_SetControlFLagForCcoreReset(IPF_FORRESET_CONTROL_FORBID);

		do
		{
			msleep(150);
			u32DlStateValue = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_STATE_OFFSET);

			if(u32DlStateValue == IPF_CHANNEL_STATE_IDLE)
			{
				break;
			}
			
		}while(--u32Timeout);
		if (!u32Timeout) 
		{
			bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,
				"\r IPF dl channel on after bsp_ipf_reset_ccore_cb called \n");
		}
	}
	else if(DRV_RESET_CALLCBFUN_RESET_AFTER == eparam)
	{
		BSP_IPF_SetControlFLagForCcoreReset(IPF_FORRESET_CONTROL_ALLOW);
		return IPF_SUCCESS;
	}
	else
	{
		/*under the requeset of yaoguocai*/
		return IPF_SUCCESS;
	}
	return IPF_SUCCESS;
}
#endif

/******************************************************************************
* �� �� ��     : BSP_IPF_GetUlBDNum
*
* ��������  : �ýӿ����ڻ�ȡ���п���BD ��Ŀ
*                            ��Χ: 0~64
* �������  : ��
*
* �������  : ��
* �� �� ֵ      : ����BD��Ŀ
*  
* �޸ļ�¼  :2011��11��30��   ³��  ����
*****************************************************************************/
BSP_U32 BSP_IPF_GetUlBDNum(BSP_VOID)
{
    BSP_U32 u32UlBdDepth = 0;
    BSP_U32 u32IdleBd = 0;

    /* �������BD���� */
    u32UlBdDepth = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_DQ_DEPTH_OFFSET);

    u32IdleBd = IPF_ULBD_DESC_SIZE - (u32UlBdDepth & IPF_DQ_DEPTH_MASK);
#if 0
    /*����BDQ_RESERVE_NUM��BD���ڼ���AD���п��ж�*/
    u32IdleBd = (u32IdleBd > BDQ_RESERVE_NUM)? (u32IdleBd - BDQ_RESERVE_NUM):0;
#endif
    *(g_stIpfUl.pu32IdleBd) = u32IdleBd;
	
    if(0 == u32IdleBd)
	{
		g_stIPFDebugInfo->u32UlBdNotEnough++;
	}
    return u32IdleBd;
}

/*****************************************************************************
* �� �� ��     : BSP_IPF_GetUlRdNum
*
* ��������  : �ýӿ����ڶ�ȡ����RD��Ŀ
*
* �������  : ��
*   
* �������  : ��
*
* �� �� ֵ     : ����RD��Ŀ
*
* �޸ļ�¼  :2013��8��1��   chendongyue  ����
*****************************************************************************/
BSP_U32 BSP_IPF_GetUlRdNum(BSP_VOID)
{
    BSP_U32 u32RdqDepth = 0;
  
    /* ��ȡRD��� */
    u32RdqDepth = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_DQ_DEPTH_OFFSET);
    u32RdqDepth = (u32RdqDepth>>16)&IPF_DQ_DEPTH_MASK;
    return u32RdqDepth;
}


/*****************************************************************************
* �� �� ��     : BSP_IPF_GetUlDescNum
*
* ��������  : �ýӿڿɶ�ȡ���пɴ������
			���ڹ��ipfӲ����ͷ��������
*
* �������  : ��
*   
* �������  : ��
*
* �� �� ֵ     : ���пɷ��Ͱ���
*
* �޸ļ�¼  :2013��8��1��   chendongyue  ����
*****************************************************************************/

BSP_U32 BSP_IPF_GetUlDescNum(BSP_VOID)
{
	BSP_U32 u32UlAd0Num = 0;
	BSP_U32 u32UlAd1Num = 0;
	BSP_U32 u32UlBdNum = 0;
/*	BSP_U32 u32UlRdNum = 0;*/
	BSP_U32 u32UlAdwptr = 0;
	BSP_U32 u32UlAdrptr = 0;
	BSP_U32 u32UlBdDepth = 0;

	/* �������AD0���� */
	u32UlBdDepth = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_DQ_DEPTH_OFFSET);
	u32UlAdwptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ0_WPTR_OFFSET);
	u32UlAdrptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ0_RPTR_OFFSET);
	u32UlBdDepth = u32UlBdDepth&IPF_DQ_DEPTH_MASK;
	
	if (u32UlAdwptr >= u32UlAdrptr)/*дָ����ǰ������˳��*/
	{
		u32UlAd0Num = u32UlAdwptr - u32UlAdrptr;
	}
	else
	{
		u32UlAd0Num = IPF_ULAD0_DESC_SIZE - (u32UlAdrptr -u32UlAdwptr);
	}
	if(u32UlAd0Num > u32UlBdDepth)
	{
		u32UlAd0Num -= u32UlBdDepth;
	}
	else
	{
		u32UlAd0Num = 0;
		g_stIPFDebugInfo->u32UlAd0NotEnough++;
	}


	/* �������AD1���� */
	u32UlBdDepth = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_DQ_DEPTH_OFFSET);
	u32UlAdwptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ1_WPTR_OFFSET);
	u32UlAdrptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ1_RPTR_OFFSET);
	u32UlBdDepth = u32UlBdDepth&IPF_DQ_DEPTH_MASK;
	
	if (u32UlAdwptr >= u32UlAdrptr)/*дָ����ǰ������˳��*/
	{
		u32UlAd1Num = u32UlAdwptr - u32UlAdrptr;
	}
	else
	{
		u32UlAd1Num =  IPF_ULAD1_DESC_SIZE - (u32UlAdrptr -u32UlAdwptr);
	}
	
	if(u32UlAd1Num > u32UlBdDepth)
	{
		u32UlAd1Num -= u32UlBdDepth;
	}
	else
	{
		u32UlAd1Num = 0;
		g_stIPFDebugInfo->u32UlAd1NotEnough++;
	}


	u32UlBdNum = BSP_IPF_GetUlBDNum();
#if 0
	if(0 == u32UlBdNum)
	{
		g_stIPFDebugInfo->u32UlBdNotEnough++;
	}
#endif

	if(u32UlBdNum > u32UlAd0Num)
	{
		u32UlBdNum = u32UlAd0Num;
	}

	if(u32UlBdNum > u32UlAd1Num)
	{
		u32UlBdNum = u32UlAd1Num;
	}
	/*
	u32UlRdNum = IPF_ULRD_DESC_SIZE - BSP_IPF_GetUlRdNum();
	if(u32UlRdNum > 1)
	{
		u32UlRdNum -= 1;
	}
	else
	{
		u32UlRdNum = 0;
	}


	if(u32UlBdNum > u32UlRdNum)
	{
		u32UlBdNum = u32UlRdNum;
	*/
       return u32UlBdNum;

}


/*****************************************************************************
* �� �� ��      : BSP_IPF_ConfigUpFilter
*
* ��������  : IPF����BD���ú��� 
*
* �������  : BSP_U32 u32Num, IPF_CONFIG_ULPARAM_S* pstUlPara
* �������  : ��
* �� �� ֵ      : IPF_SUCCESS    ���óɹ�
*                           IPF_ERROR      ����ʧ��
*                           BSP_ERR_IPF_NOT_INIT         ģ��δ��ʼ��
*                           BSP_ERR_IPF_INVALID_PARA     ��������
*
* �޸ļ�¼  :2011��11��30��   ³��  ����
				2012��11��30��	�¶����޸���Ӷ����������
									��̬ҵ��ģʽ���õ�֧��
*****************************************************************************/
BSP_S32 BSP_IPF_ConfigUpFilter(BSP_U32 u32Num, IPF_CONFIG_ULPARAM_S* pstUlPara)
{
    BSP_U32 u32BdqWptr = 0;
    IPF_CONFIG_ULPARAM_S* pstUlParam = pstUlPara;
    BSP_U32 u32BD = 0;
    BSP_U32 i = 0;
    BSP_U32 u32TimeStampEn;

    /* ������� */
    if((NULL == pstUlPara)||(0 == u32Num))
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,
                         "\r BSP_IPF_ConfigUpFilter pInfoNode NULL! \n",0,0,0,0,0,0);
        return BSP_ERR_IPF_INVALID_PARA;
    }

    /* ���ģ���Ƿ��ʼ�� */
    if(IPF_ACORE_INIT_SUCCESS != g_IPFInit)
    {
        g_stIPFDebugInfo->ipf_acore_not_init_count++;
        return BSP_ERR_IPF_NOT_INIT;
    }

    /* ���Ccore�Ƿ��ϵ�*/
    if(IPF_FORRESET_CONTROL_FORBID <= BSP_IPF_GetControlFLagForCcoreReset())
    {
        g_stIPFDebugInfo->ipf_acore_not_init_count++;
        return BSP_ERR_IPF_CCORE_RESETTING;
    }

    #ifdef __BSP_IPF_DEBUG__
    if(u32Num > *(g_stIpfUl.pu32IdleBd))
    {
        g_stIPFDebugInfo->u32UlBdNotEnough++;
        return IPF_ERROR;
    }
    #endif
    for(i = 0; i < u32Num; i++)
    {
        if(0 == pstUlParam[i].u16Len)
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r INPUT BD_Len =0 !Drop pkt \n");
            g_stIPFDebugInfo->ipf_ulbd_len_zero_count++;
            return BSP_ERR_IPF_INVALID_PARA;
        }
    }
	
    u32TimeStampEn = g_stIPFDebugInfo->ipf_timestamp_ul_en;

    /* ����BDдָ��,��u32BdqWptr��Ϊ��ʱдָ��ʹ�� */
    u32BdqWptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_BDQ_WPTR_OFFSET); 
    u32BD = (u32BdqWptr&IPF_DQ_PTR_MASK);
    for(i = 0; i < u32Num; i++)
    {
        g_stIpfUl.pstIpfBDQ[u32BD].u16Attribute = pstUlParam[i].u16Attribute;
        g_stIpfUl.pstIpfBDQ[u32BD].u32InPtr = pstUlParam[i].u32Data;
        g_stIpfUl.pstIpfBDQ[u32BD].u16PktLen = pstUlParam[i].u16Len;
        g_stIpfUl.pstIpfBDQ[u32BD].u16UsrField1 = pstUlParam[i].u16UsrField1;
        g_stIpfUl.pstIpfBDQ[u32BD].u32UsrField2= pstUlParam[i].u32UsrField2;
        g_stIpfUl.pstIpfBDQ[u32BD].u32UsrField3 = pstUlParam[i].u32UsrField3;
        ipf_record_start_time_stamp(u32TimeStampEn, &g_stIpfUl.pstIpfBDQ[u32BD].u32UsrField3);

        #ifdef __BSP_IPF_DEBUG__
        if(g_stIPFDebugInfo->u32IpfDebug)
        {
            IPF_PRINT(" func: %d  %x, %d\n", u32BD, g_stIpfUl.pstIpfBDQ[u32BD].u32UsrField2,g_stIpfUl.pstIpfBDQ[u32BD].u32UsrField3);
        }
        #endif
        u32BD = ((u32BD + 1) < IPF_ULBD_DESC_SIZE)? (u32BD + 1) : 0;
    }
    
    /* ���Ccore�Ƿ��ϵ�*/
    if(IPF_FORRESET_CONTROL_FORBID <= BSP_IPF_GetControlFLagForCcoreReset())
    {
        g_stIPFDebugInfo->ipf_acore_not_init_count++;
        return BSP_ERR_IPF_CCORE_RESETTING;
    }
	
    g_stIPFDebugInfo->ipf_cfg_ulbd_count += u32Num;

    /* ����BDдָ��*/
    ipf_writel(u32BD, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_BDQ_WPTR_OFFSET);  
    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��     : BSP_IPF_ConfigDlAd
*
* ��������  : �ýӿڽ���A���ṩ�������ƶ�ADQдָ�룬
				�����е�AD�����µ��ڴ滺������һ�ο��Դ�����AD��
				����ǰҪ��������������仺������
*                           
* �������  : BSP_U32 u32ADNum0; 
				 BSP_U32 u32ADNum1; 
				 BSP_VOID* psk0; 
				 BSP_VOID* psk1
*
* �������  : ��
* �� �� ֵ      : ��
* �޸ļ�¼  :2012��11��24��   �¶���  ����
*****************************************************************************/
BSP_S32 BSP_IPF_ConfigDlAd(BSP_U32 u32AdType, BSP_U32  u32AdNum, IPF_AD_DESC_S * pstAdDesc)
{
	BSP_U32 u32ADQwptr = 0;
	struct tagIPF_AD_DESC_S * pstADDesc = pstAdDesc;
	BSP_U32 i;
	if(NULL == pstAdDesc)
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigDlAd input para ERROR! NULL == pstAdDesc\n");
		return BSP_ERR_IPF_INVALID_PARA;
	}	

	if(u32AdType >= IPF_AD_MAX)
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigDlAd input para ERROR! u32AdType >= IPF_AD_MAX\n");
		return BSP_ERR_IPF_INVALID_PARA;
	}

	/* ���ģ���Ƿ��ʼ�� */
	if(IPF_ACORE_INIT_SUCCESS != g_IPFInit)
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigDlAd IPF Not Init! \n");
		return BSP_ERR_IPF_NOT_INIT;
	}


	if(IPF_AD_0 == u32AdType)
	{
		if(u32AdNum >= IPF_DLAD0_DESC_SIZE)
		{
			bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigDlAd input para ERROR! u32AdNum >=IPF_ULAD0_DESC_SIZE\n");
			return BSP_ERR_IPF_INVALID_PARA;
		}

		/*����дָ��*/
		u32ADQwptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ0_WPTR_OFFSET);
		for(i=0; i < u32AdNum; i++)
		{
			if(NULL == (BSP_VOID*)(pstADDesc->u32OutPtr1))
			{
				bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigDlAd input u32OutPtr1 NULL! \n");
				g_stIPFDebugInfo->ipf_dlad0_error_count++;
				return BSP_ERR_IPF_INVALID_PARA;
			}
			g_stIpfDl.pstIpfADQ0[u32ADQwptr].u32OutPtr1 = pstADDesc->u32OutPtr1;
			g_stIpfDl.pstIpfADQ0[u32ADQwptr].u32OutPtr0 = pstADDesc->u32OutPtr0;
			u32ADQwptr = ((u32ADQwptr + 1) < IPF_DLAD0_DESC_SIZE)? (u32ADQwptr + 1) : 0;		
			pstADDesc++;
		}
		g_stIPFDebugInfo->ipf_cfg_dlad0_count += u32AdNum;

		/* ����AD0дָ��*/
		ipf_writel(u32ADQwptr, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ0_WPTR_OFFSET);  
	}
	else if(IPF_AD_1 == u32AdType)
	{
		if(u32AdNum >= IPF_DLAD1_DESC_SIZE)
		{
			bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigDlAd input para ERROR! u32AdNum >=IPF_ULAD1_DESC_SIZE \n");
			return BSP_ERR_IPF_INVALID_PARA;
		}

		/*����дָ��*/
		u32ADQwptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ1_WPTR_OFFSET);
		for(i=0; i < u32AdNum; i++)
		{
			if(NULL == (BSP_VOID*)(pstADDesc->u32OutPtr1))
			{
				bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_ConfigDlAd input u32OutPtr1 NULL! \n");
				g_stIPFDebugInfo->ipf_dlad1_error_count++;
				return BSP_ERR_IPF_INVALID_PARA;
			}

			g_stIpfDl.pstIpfADQ1[u32ADQwptr].u32OutPtr1 = pstADDesc->u32OutPtr1;
			g_stIpfDl.pstIpfADQ1[u32ADQwptr].u32OutPtr0 = pstADDesc->u32OutPtr0;
			u32ADQwptr = ((u32ADQwptr + 1) < IPF_DLAD1_DESC_SIZE)? (u32ADQwptr + 1) : 0;		
			pstADDesc++;
		}
		g_stIPFDebugInfo->ipf_cfg_dlad1_count += u32AdNum;

		/* ����AD1дָ��*/
		ipf_writel(u32ADQwptr, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ1_WPTR_OFFSET);  
	}
	return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��     : BSP_IPF_RegisterWakeupDlCb
*
* ��������  : �ýӿ�����ע������PS����ص�����
*                           
* �������  : BSP_IPF_WakeupDlkCb *pFnWakeupDl
*
* �������  : ��
* �� �� ֵ      : ��
*  
* �޸ļ�¼  :2011��11��30��   ³��  ����
*****************************************************************************/
BSP_S32 BSP_IPF_RegisterWakeupDlCb(BSP_IPF_WakeupDlCb pFnWakeupDl)
{
    /* ������� */
    if(NULL == pFnWakeupDl)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_RegisterWakeupDlCb inputPara ERROR! \n");
        return BSP_ERR_IPF_INVALID_PARA;
    }
    g_stIpfDl.pFnDlIntCb = pFnWakeupDl;
    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��     : BSP_IPF_RegisterAdqEmptyDlCb
*
* ��������  : �˽ӿ�ֻ��A���ṩ������ע�ỽ������PS��
                             ADQ���пջص�����
*                           
* �������  : BSP_IPF_AdqEmptyCb pAdqEmptyDl
*
* �������  : ��
* �� �� ֵ      : IPF_SUCCESS ע��ɹ�
*                            IPF_ERROR	ע��ʧ��
* �޸ļ�¼  :2012��11��24��   �¶���  ����
*****************************************************************************/
BSP_S32 BSP_IPF_RegisterAdqEmptyDlCb(BSP_IPF_AdqEmptyDlCb pAdqEmptyDl)
{
    /* ������� */
    if(NULL == pAdqEmptyDl)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_RegisterAdqEmptyDlCb inputPara ERROR! \n");
        return IPF_ERROR;
    }
    g_stIpfDl.pAdqEmptyDlCb = pAdqEmptyDl;
    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��     : BSP_IPF_GetDlRd
*
* ��������  : �ýӿ����ڶ�ȡ����BD, ֧��һ�ζ�ȡ���BD
*
* �������  : BSP_U32* pu32Num    
*                           IPF_RD_DESC_S *pstRd
*   
* �������  : BSP_U32* pu32Num    ʵ�ʶ�ȡ��RD��Ŀ
*
* �� �� ֵ     : IPF_SUCCESS               �����ɹ�
*                           IPF_ERROR                   ����ʧ��
*
* �޸ļ�¼  :2011��11��30��   ³��  ����
*****************************************************************************/
BSP_VOID BSP_IPF_GetDlRd(BSP_U32* pu32Num, IPF_RD_DESC_S *pstRd)
{
    BSP_U32 u32RdqRptr = 0;
    BSP_U32 u32RdqDepth = 0;
    BSP_U32 u32Num = 0;
    BSP_U32 i = 0;
    BSP_U32 u32CdqRptr;
    BSP_U32 u32TimeStampEn;
	
    /* ��ȡRD��� */
    u32RdqDepth = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_DQ_DEPTH_OFFSET);
    u32RdqDepth = (u32RdqDepth>>16)&IPF_DQ_DEPTH_MASK;
	
    u32Num = (u32RdqDepth < *pu32Num)?u32RdqDepth:*pu32Num;
    if(0 == u32Num)
    {
        *pu32Num = 0;
        return;
    }
    u32TimeStampEn = g_stIPFDebugInfo->ipf_timestamp_dl_en;

    /* ��ȡRD��ָ�� */
    u32RdqRptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_RDQ_RPTR_OFFSET);
    for(i = 0; i < u32Num; i++)
    {
        
        /* ��ȡRD */
        pstRd[i].u16Attribute = g_stIpfDl.pstIpfRDQ[u32RdqRptr].u16Attribute;
        pstRd[i].u16PktLen = g_stIpfDl.pstIpfRDQ[u32RdqRptr].u16PktLen;
        pstRd[i].u16Result = g_stIpfDl.pstIpfRDQ[u32RdqRptr].u16Result;
        pstRd[i].u32InPtr = g_stIpfDl.pstIpfRDQ[u32RdqRptr].u32InPtr;
        pstRd[i].u32OutPtr = g_stIpfDl.pstIpfRDQ[u32RdqRptr].u32OutPtr;
        pstRd[i].u16UsrField1 = g_stIpfDl.pstIpfRDQ[u32RdqRptr].u16UsrField1;
        pstRd[i].u32UsrField2 = g_stIpfDl.pstIpfRDQ[u32RdqRptr].u32UsrField2;
        pstRd[i].u32UsrField3 = g_stIpfDl.pstIpfRDQ[u32RdqRptr].u32UsrField3;
        ipf_record_end_time_stamp(u32TimeStampEn, g_stIpfDl.pstIpfRDQ[u32RdqRptr].u32UsrField3);

       /* ����CD��ָ�� */
        u32CdqRptr = (IPF_IO_ADDRESS(pstRd[i].u32InPtr) - (BSP_U32)g_stIpfDl.pstIpfCDQ)/sizeof(IPF_CD_DESC_S);

        while(g_stIpfDl.pstIpfCDQ[u32CdqRptr].u16Attribute != 1)
        {
            /* ���ͷŵ�CD  ��0 */
            g_stIpfDl.pstIpfCDQ[u32CdqRptr].u16Attribute = 0;
            g_stIpfDl.pstIpfCDQ[u32CdqRptr].u16PktLen = 0;
            g_stIpfDl.pstIpfCDQ[u32CdqRptr].u32Ptr = 0;
            u32CdqRptr = ((u32CdqRptr+1) < IPF_DLCD_DESC_SIZE)?(u32CdqRptr+1):0;
        }
        g_stIpfDl.pstIpfCDQ[u32CdqRptr].u16Attribute = 0;
        g_stIpfDl.pstIpfCDQ[u32CdqRptr].u16PktLen = 0;
        g_stIpfDl.pstIpfCDQ[u32CdqRptr].u32Ptr = 0;
		
        u32CdqRptr = ((u32CdqRptr+1) < IPF_DLCD_DESC_SIZE)?(u32CdqRptr+1):0;
        *(g_stIpfDl.u32IpfCdRptr) = u32CdqRptr;
        /* ����RD��ָ�� */
        u32RdqRptr = ((u32RdqRptr+1) < IPF_DLRD_DESC_SIZE)?(u32RdqRptr+1):0;        
    }
    ipf_writel(u32RdqRptr, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_RDQ_RPTR_OFFSET);         
    *pu32Num = u32Num;
    g_stIPFDebugInfo->ipf_get_dlrd_count += u32Num;
}
/*****************************************************************************
* �� �� ��       : BSP_IPF_GetDlAdNum
*
* ��������  : �ýӿ�ֻ��A���ṩ����ȡ���У�A�ˣ�
					����(�������AD�Ļ������Ѿ���ռ��)AD��Ŀ
*
* �������  :BSP_OK����������
                            BSP_ERROR������
*             
* �������  : ��
* �� �� ֵ     : ��
* �޸ļ�¼  : 2011��11��24��   �¶���  ����
*****************************************************************************/
BSP_S32 BSP_IPF_GetDlAdNum(BSP_U32* pu32AD0Num,BSP_U32* pu32AD1Num)
{
	BSP_U32 u32DlAdDepth = 0;
	BSP_U32 u32DlAdwptr = 0;
	BSP_U32 u32DlAdrptr = 0;

	/* ���ģ���Ƿ��ʼ�� */
	if(IPF_ACORE_INIT_SUCCESS != g_IPFInit)
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_GetDlAdNum IPF Not Init! \n");
		return BSP_ERR_IPF_NOT_INIT;
	}

	/*��μ��*/
	if((NULL == pu32AD0Num)||(NULL == pu32AD1Num))
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r BSP_IPF_GetDlAdNum pstCtrl NULL! \n");
		return BSP_ERR_IPF_INVALID_PARA;
	}
	
	/* �������AD0���� */
	u32DlAdwptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ0_WPTR_OFFSET);
	u32DlAdrptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ0_RPTR_OFFSET);
	if (u32DlAdwptr >= u32DlAdrptr)/*дָ����ǰ������˳����ָ����ȵ�������Ϊ0*/
	{
		u32DlAdDepth = IPF_DLAD0_DESC_SIZE - (u32DlAdwptr - u32DlAdrptr);
	}
	else
	{
		u32DlAdDepth = u32DlAdrptr -u32DlAdwptr;
	}

	/*�۳�reserve ad�����ڷ�ֹӲ����ad������ʶ��ɿպ͵͹��������ڴ�й¶*/
	if(u32DlAdDepth > IPF_ADQ_RESERVE_NUM)
	{
		*pu32AD0Num = u32DlAdDepth - IPF_ADQ_RESERVE_NUM;
	}
	else
	{
		*pu32AD0Num = 0;
	}
	
	/* �������AD1���� */
	u32DlAdwptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ1_WPTR_OFFSET);
	u32DlAdrptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ1_RPTR_OFFSET);
	if (u32DlAdwptr >= u32DlAdrptr)/*дָ����ǰ������˳��*/
	{
		u32DlAdDepth = IPF_DLAD1_DESC_SIZE - (u32DlAdwptr - u32DlAdrptr);
	}
	else
	{
		u32DlAdDepth =  u32DlAdrptr - u32DlAdwptr;
	}
	*pu32AD1Num = u32DlAdDepth;

	/*�۳�reserve ad�����ڷ�ֹӲ����ad������ʶ��ɿպ͵͹��������ڴ�й¶*/	
	if(u32DlAdDepth > IPF_ADQ_RESERVE_NUM)
	{
		*pu32AD1Num = u32DlAdDepth - IPF_ADQ_RESERVE_NUM;
	}
	else
	{
		*pu32AD1Num = 0;
	}

	return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��     : BSP_IPF_GetDlRdNum
*
* ��������  : �ýӿ����ڶ�ȡ����RD��Ŀ
*
* �������  : ��
*   
* �������  : ��
*
* �� �� ֵ     : ����RD��Ŀ
*
* �޸ļ�¼  :2012��7��16��   ³��  ����
*****************************************************************************/
BSP_U32 BSP_IPF_GetDlRdNum(BSP_VOID)
{
    BSP_U32 u32RdqDepth = 0;
  
    /* ��ȡRD��� */
    u32RdqDepth = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_DQ_DEPTH_OFFSET);
    u32RdqDepth = (u32RdqDepth>>16)&IPF_DQ_DEPTH_MASK;
    return u32RdqDepth;
}

/*****************************************************************************
* �� �� ��     : ipf_probe
*
* ��������  : ƽ̨�豸׮����
*
* �������  : ��
*   
* �������  : ��
*
* �� �� ֵ     : �ɹ�
*
* �޸ļ�¼  :2013��6��16��   �¶���  ����
*****************************************************************************/
static int ipf_probe(struct platform_device *pdev)
{

    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��     : ipf_remove
*
* ��������  : ƽ̨�豸׮����
*
* �������  : ��
*   
* �������  : ��
*
* �� �� ֵ     : �ɹ�
*
* �޸ļ�¼  :2013��6��16��   �¶���  ����
*****************************************************************************/
static int ipf_remove(struct platform_device *pdev)
{
    return IPF_SUCCESS;
}

#ifdef CONFIG_PM

/*****************************************************************************
* �� �� ��     : ipf_dl_dpm_prepare
*
* ��������  : dpm����׼������
*
* �������  : �豸ָ��
*   
* �������  : ��
*
* �� �� ֵ     : IPF_ERROR ʧ��
                            IPF_SUCCESS �ɹ�
*
* �޸ļ�¼  :2013��6��16��   �¶���  ����
*****************************************************************************/
static int ipf_dl_dpm_prepare(struct device *pdev)
{
    u32 u32_dl_state;
	
    #if 0
    u32 u32_adq0_wptr;
    u32 u32_adq0_rptr;
    u32 u32_adq1_wptr;
    u32 u32_adq1_rptr;
    #endif

#ifdef CONFIG_BALONG_MODEM_RESET	
    /* ���Ccore�Ƿ��ϵ磬����Ѿ��µ磬�����͹��ı���ָ�����*/
    if(IPF_FORRESET_CONTROL_FORBID <= BSP_IPF_GetControlFLagForCcoreReset())
    {
        bsp_trace(BSP_LOG_LEVEL_WARNING, BSP_MODU_IPF,"\r CCore power down!IPF  Skip dpm process\n");
        return IPF_SUCCESS;
    }
#endif

    /* �ж�����IPF�Ƿ���� */
    u32_dl_state = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_STATE_OFFSET);

    if(u32_dl_state != IPF_CHANNEL_STATE_IDLE)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF CHANNEL NOT IDLE! \n");
        return IPF_ERROR;
    }
    #if 0
	/*�ȴ�ad���жϴ������*/
    u32_adq0_wptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ0_WPTR_OFFSET);
    u32_adq0_rptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ0_RPTR_OFFSET);
    u32_adq1_wptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ1_WPTR_OFFSET);
    u32_adq1_rptr = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ1_RPTR_OFFSET);
    if((u32_adq0_rptr == u32_adq0_wptr)||(u32_adq1_rptr == u32_adq1_wptr))
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_IPF,"\r IPF DL AD EMPTY INTRRUPT PROCESSING! \n");
        return IPF_ERROR;
    }
    #endif
    return IPF_SUCCESS;
}

/*****************************************************************************
* �� �� ��     : ipf_dl_dpm_complete
*
* ��������  : dpm׮����
*
* �������  : �豸ָ��
*   
* �������  : ��
*
* �� �� ֵ     : ��
*
* �޸ļ�¼  :2013��6��16��   �¶���  ����
*****************************************************************************/
static void ipf_dl_dpm_complete(struct device *pdev)
{
    return ;
}

/*****************************************************************************
* �� �� ��     : ipf_dl_suspend
*
* ��������  : dpm׮����
*
* �������  : �豸ָ��
*   
* �������  : ��
*
* �� �� ֵ     : �ɹ�
*
* �޸ļ�¼  :2013��6��16��   �¶���  ����
			2014��1��23��v1.01 �¶��� �޸� ����K3V3����������⣬
			�޷���m3�Ͻ��е͹��Ļָ����ƶ���A9�Ͻ��С�
*****************************************************************************/
static int ipf_dl_suspend(struct device *dev)
{
#if (defined(BSP_CONFIG_HI3630))

    unsigned long ipf_flags = 0;

#ifdef CONFIG_BALONG_MODEM_RESET	
    /* ���Ccore�Ƿ��ϵ�����Ѿ��µ磬�����͹��ı���ָ�����*/
    if(IPF_FORRESET_CONTROL_FORBID <= BSP_IPF_GetControlFLagForCcoreReset())
    {
        bsp_trace(BSP_LOG_LEVEL_WARNING, BSP_MODU_IPF,"\r CCore power down!IPF  Skip dpm process\n");
        return IPF_SUCCESS;
    }
#endif

    spin_lock_irqsave(&ipf_filter_spinlock, ipf_flags);
    bsp_ipc_spin_lock(IPC_SEM_IPF_PWCTRL);
    *ipf_acore_init_status = IPF_PWC_DOWN;
    cache_sync();		
    bsp_ipc_spin_unlock(IPC_SEM_IPF_PWCTRL);
    spin_unlock_irqrestore(&ipf_filter_spinlock, ipf_flags);

#endif
    g_stIPFDebugInfo->ipf_acore_suspend_count++;
    return IPF_SUCCESS;
}


/*****************************************************************************
* �� �� ��     : ipf_dl_resume
*
* ��������  : dpm׮����
*
* �������  : �豸ָ��
*   
* �������  : ��
*
* �� �� ֵ     : �ɹ�
*
* �޸ļ�¼  :2013��6��16��   �¶���  ����
			2014��1��23��v1.01 �¶��� �޸� ����K3V3����������⣬
			�޷���m3�Ͻ��е͹��Ļָ����ƶ���A9�Ͻ��С�
*****************************************************************************/
static int ipf_dl_resume(struct device *dev)
{
#ifdef CONFIG_BALONG_MODEM_RESET	
    /* ���Ccore�Ƿ��ϵ�����Ѿ��µ磬�����͹��ı���ָ�����*/
    if(IPF_FORRESET_CONTROL_FORBID <= BSP_IPF_GetControlFLagForCcoreReset())
    {
        bsp_trace(BSP_LOG_LEVEL_WARNING, BSP_MODU_IPF,"\r CCore power down!IPF Skip dpm process\n");
        return IPF_SUCCESS;
    }
#endif

#if (defined(BSP_CONFIG_HI3630))
    /*����K3V3����������⣬�ָ��������ƶ����˴�*/
    ipf_check_filter_restore();
#endif
    g_stIPFDebugInfo->ipf_acore_resume_count++;
    return IPF_SUCCESS;
}
#endif

#endif

#ifdef __KERNEL__
module_init(ipf_init); /*lint !e528*/
EXPORT_SYMBOL(ipf_init);
EXPORT_SYMBOL(BSP_IPF_ConfigTimeout);
EXPORT_SYMBOL(BSP_IPF_ConfigUpFilter);
EXPORT_SYMBOL(BSP_IPF_GetUlBDNum);
EXPORT_SYMBOL(BSP_IPF_SetPktLen);
EXPORT_SYMBOL(BSP_IPF_RegisterWakeupDlCb);
EXPORT_SYMBOL(BSP_IPF_GetDlRd);
EXPORT_SYMBOL(BSP_IPF_GetDlRdNum);
EXPORT_SYMBOL(BSP_IPF_Help);
EXPORT_SYMBOL(BSP_IPF_BDInfo);
EXPORT_SYMBOL(BSP_IPF_RDInfo);
EXPORT_SYMBOL(BSP_IPF_Info);
EXPORT_SYMBOL(BSP_IPF_MEM);
EXPORT_SYMBOL(BSP_IPF_ConfigDlAd);
EXPORT_SYMBOL(BSP_IPF_RegisterAdqEmptyDlCb);
EXPORT_SYMBOL(BSP_IPF_GetDlAdNum);
EXPORT_SYMBOL(BSP_IPF_GetControlFLagForCcoreReset);
MODULE_LICENSE("GPL");

#endif

#ifdef __cplusplus
}
#endif

/*lint -restore*/

