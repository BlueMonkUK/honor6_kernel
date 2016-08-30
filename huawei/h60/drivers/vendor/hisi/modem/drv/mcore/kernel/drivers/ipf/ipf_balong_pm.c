/*************************************************************************
*   ��Ȩ����(C) 1987-2013, ���ڻ�Ϊ�������޹�˾.
*
*   �� �� �� :  ipf_balong_pm.c
*
*   ��    �� :  chendongyue
*
*   ��    �� :  IPF�ĵ͹��Ľӿڣ�������m3
*
*   �޸ļ�¼ :  2013��4��23��v1.00 chendongyue ����
*
*************************************************************************/

/*lint -save -e18 -e156 -e438 -e740 -e830*/

#include "bsp_ipf.h"


/**************************************************************************
ipf �Ĵ����ָ�˳��:
1.���ƣ�����
2.���������ж�дָ��(��������������֮��û��˳��)
	AD:��д��rptr(�����)��д��wptr(�������)
	BD��RD:���ر��ݡ��ָ�д�룬��ֱ�ӹ�0
3.ͨ��ʹ��
***************************************************************************/
#if (defined(BSP_CONFIG_HI3630))
__ao_data u32 g_u32_ipf_setting_bak_space[20];
__ao_data u32 g_u32_ipf_adq_rptr_bak_space[8];
__ao_data u32 g_u32_ipf_adq_wptr_chctrl_bak_space[9];

__ao_data u32* g_u32_ipf_setting_bak_addr = g_u32_ipf_setting_bak_space;
__ao_data u32* g_u32_ipf_adq_rptr_bak_addr = g_u32_ipf_adq_rptr_bak_space;
__ao_data u32* g_u32_ipf_adq_wptr_chctrl_bak_addr = g_u32_ipf_adq_wptr_chctrl_bak_space;
#else

__ao_data u32* g_u32_ipf_setting_bak_addr = (u32*)IPF_PWRCTL_INFO_ADDR;
__ao_data u32* g_u32_ipf_adq_rptr_bak_addr = (u32*)IPF_PWRCTL_ADQ_WPTR_INFO_ADDR;
__ao_data u32* g_u32_ipf_adq_wptr_chctrl_bak_addr = (u32*)IPF_PWRCTL_ADQ_RPTR_CHCTRL_ADDR;

#endif

#ifdef CONFIG_BALONG_MODEM_RESET
__ao_data u32 g_u32_ipf_ccore_reset_flag = IPF_FORRESET_CONTROL_ALLOW;
#endif

__ao_data u32 g_ipf_setting_table[] =
    {
        HI_IPF_CTRL_OFFSET,
        HI_IPF_GATE_OFFSET,
        HI_IPF_DMA_CTRL1_OFFSET,
        HI_IPF_EF_BADDR_OFFSET,

        HI_IPF_CH0_CTRL_OFFSET,
        HI_IPF_CH1_CTRL_OFFSET,		
			
        HI_IPF_CH0_BDQ_SIZE_OFFSET,
        HI_IPF_CH1_BDQ_SIZE_OFFSET,
        
        HI_IPF_CH0_RDQ_SIZE_OFFSET,
        HI_IPF_CH1_RDQ_SIZE_OFFSET,
        
        HI_IPF_CH0_BDQ_BADDR_OFFSET,
        HI_IPF_CH1_BDQ_BADDR_OFFSET,
        
        HI_IPF_CH0_RDQ_BADDR_OFFSET,
        HI_IPF_CH1_RDQ_BADDR_OFFSET,
        
        HI_IPF_CH0_ADQ0_BASE_OFFSET, 
        HI_IPF_CH0_ADQ1_BASE_OFFSET, 
        
        HI_IPF_CH1_ADQ0_BASE_OFFSET,  
        HI_IPF_CH1_ADQ1_BASE_OFFSET, 
        
        HI_IPF_TIME_OUT_OFFSET,
        HI_IPF_PKT_LEN_OFFSET
    };
__ao_data u32 g_ipf_adq_rptr_table[] =
    {
        HI_IPF_CH0_ADQ0_RPTR_OFFSET, 
        HI_IPF_CH0_ADQ1_RPTR_OFFSET, 
        HI_IPF_CH1_ADQ0_RPTR_OFFSET, 
        HI_IPF_CH1_ADQ1_RPTR_OFFSET, 
        
        HI_IPF_CH0_ADQ0_STAT_OFFSET, 
        HI_IPF_CH0_ADQ1_STAT_OFFSET, 
        HI_IPF_CH1_ADQ0_STAT_OFFSET, 
        HI_IPF_CH1_ADQ1_STAT_OFFSET
    };
__ao_data u32 g_ipf_adq_wptr_chctrl_table[] =
    {
        HI_IPF_CH0_ADQ0_WPTR_OFFSET, 
        HI_IPF_CH0_ADQ1_WPTR_OFFSET, 
        HI_IPF_CH1_ADQ0_WPTR_OFFSET, 
        HI_IPF_CH1_ADQ1_WPTR_OFFSET, 

        HI_IPF_CH0_ADQ_CTRL_OFFSET,
        HI_IPF_CH1_ADQ_CTRL_OFFSET,
        HI_IPF_INT_MASK0_OFFSET,
        HI_IPF_INT_MASK1_OFFSET,		
        HI_IPF_CH_EN_OFFSET
    };

#ifdef CONFIG_BALONG_MODEM_RESET

int bsp_ipf_reset_ccore_lpm3_cb(DRV_RESET_CALLCBFUN_MOMENT eparam, int userdata)
{
	if(DRV_RESET_CALLCBFUN_RESET_BEFORE == eparam)
	{
		g_u32_ipf_ccore_reset_flag = IPF_FORRESET_CONTROL_FORBID;
	}
	else if(DRV_RESET_CALLCBFUN_RESET_AFTER == eparam)
	{
		g_u32_ipf_ccore_reset_flag = IPF_FORRESET_CONTROL_ALLOW;
	}
	return IPF_SUCCESS;
}
#endif

static void ipf_drx_restore_adq_rptr(void)
{
#ifndef CONFIG_IPF_AD_RPRT_FIX
    u32 u32AdStateValue = 0;
#endif	
    u32 u32Index = 0;
    u32 u32RptrValue = 0;
    u32 u32RptrOffsetValue = 0; 
    u32 u32AdqRptrMaxNum[4]  = {IPF_ULAD0_DESC_SIZE,
                                                  IPF_ULAD1_DESC_SIZE,
                                                  IPF_DLAD0_DESC_SIZE,
                                                  IPF_DLAD1_DESC_SIZE};
    u32 u32ADCtrl[IPF_CHANNEL_MAX] = {IPF_ADQ_OFF,IPF_ADQ_OFF};

    /*д��ADQ���ȵ���ʹ��ADQͨ�������ڷ�ֹ����ADQ�ж�*/
    ipf_writel(u32ADCtrl[IPF_CHANNEL_UP], HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ_CTRL_OFFSET);
    ipf_writel(u32ADCtrl[IPF_CHANNEL_DOWN], HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ_CTRL_OFFSET);
    for(u32Index = 0; u32Index < 4; u32Index++)
    {

        u32RptrValue = g_u32_ipf_adq_rptr_bak_addr[u32Index];

#ifndef CONFIG_IPF_AD_RPRT_FIX
        u32AdStateValue = g_u32_ipf_adq_rptr_bak_addr[u32Index+4];
		
        /* ���ݶ�ȡ��״̬�Ĵ���/��ָ��Ĵ���ֵ���¼���Rptr*/        
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
#endif
        if(u32RptrValue >= u32RptrOffsetValue)
        {
            u32RptrValue = u32RptrValue - u32RptrOffsetValue;
        }
        else
        {
            u32RptrValue = u32AdqRptrMaxNum[u32Index] + u32RptrValue - u32RptrOffsetValue;
        }
        /* ����ADQ״̬�Ĵ���/��ָ��Ĵ��� */
        ipf_writel(u32RptrValue, HI_IPF_REGBASE_ADDR_VIRT + g_ipf_adq_rptr_table[u32Index]); 
    }
}




/*****************************************************************************
* �� �� ��       : ipf_drx_bak_reg
*
* ��������  : �ṩ�ĵ͹��Ľӿڣ�����IPF�����Ϣ
*
* �������  : ��
*             
* �������  : ��
* �� �� ֵ     : IPF_SUCCESS     IPF�����µ磬�����Ϣ�Ѿ�����
*                           IPF_ERROR        IPF�������µ�
* ע��:�ú�����Ҫ�����жϵ�����µ���
* �޸ļ�¼  : 2013��4��23��v1.00 chendongyue ����
*****************************************************************************/
s32 ipf_drx_bak_reg(void)
{
    u32* u32_addr = NULL;
    u32 i = 0;
    u32 u32_total = 0;
    u32 u32_ul_state = 0;
    u32 u32_dl_state = 0;
    u32 u32ADCtrl[IPF_CHANNEL_MAX] = {0,0};

#ifdef CONFIG_BALONG_MODEM_RESET
    if(IPF_FORRESET_CONTROL_ALLOW != g_u32_ipf_ccore_reset_flag)
    {
        return IPF_SUCCESS;
    }
#endif
	
    /* �ж�����IPF�Ƿ���� */    
    u32_ul_state = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_STATE_OFFSET);

    if(u32_ul_state != IPF_CHANNEL_STATE_IDLE)
    {
        return IPF_ERROR;
    }

    /* �ж�����IPF�Ƿ���� */  	    
    u32_dl_state = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_STATE_OFFSET);

    if(u32_dl_state != IPF_CHANNEL_STATE_IDLE)
    {
        return IPF_ERROR;
    }
    u32ADCtrl[IPF_CHANNEL_UP] = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ_CTRL_OFFSET);
    u32ADCtrl[IPF_CHANNEL_DOWN] = ipf_readl(HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ_CTRL_OFFSET);
    u32ADCtrl[IPF_CHANNEL_UP] &= IPF_ADQ_EN_MASK;
    u32ADCtrl[IPF_CHANNEL_DOWN] &= IPF_ADQ_EN_MASK;
    u32ADCtrl[IPF_CHANNEL_UP] |= (IPF_NO_ADQ);
    u32ADCtrl[IPF_CHANNEL_DOWN] |= (IPF_NO_ADQ);

    /*�ر�ADQͨ�������ڷ�ֹ����ADQԤȡ*/
    ipf_writel(u32ADCtrl[IPF_CHANNEL_UP], HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH0_ADQ_CTRL_OFFSET);
    ipf_writel(u32ADCtrl[IPF_CHANNEL_DOWN], HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_CH1_ADQ_CTRL_OFFSET);

	
    /* �����Ҫ�ļĴ��� */
    u32_total = sizeof(g_ipf_setting_table)/sizeof(u32);
    u32_addr = g_u32_ipf_setting_bak_addr;
    
    for(i=0; i<u32_total; i++)
    {
        *u32_addr = ipf_readl(g_ipf_setting_table[i] + HI_IPF_REGBASE_ADDR_VIRT);
        u32_addr ++;
    }

    u32_total = sizeof(g_ipf_adq_wptr_chctrl_table)/sizeof(u32);
    u32_addr = g_u32_ipf_adq_wptr_chctrl_bak_addr;
	
    /* ��������ADQ_Wptr�Ĵ�����״̬�Ĵ���������Ĵ�����Ҫ���⴦�� */
    for(i=0; i<u32_total; i++)
    {
        *u32_addr = ipf_readl(g_ipf_adq_wptr_chctrl_table[i] + HI_IPF_REGBASE_ADDR_VIRT);
        u32_addr ++;
    }
	
    /*�����ϵ�����˳�����⣬����ʣ��ļĴ���*/
    u32_total = sizeof(g_ipf_adq_rptr_table)/sizeof(u32);
    u32_addr = g_u32_ipf_adq_rptr_bak_addr;


    /* ��������ADQ_Rptr�Ĵ�����״̬�Ĵ������üĴ�����Ҫ���⴦�� */
    for(i=0; i<u32_total; i++)
    {
        *u32_addr = ipf_readl(g_ipf_adq_rptr_table[i] + HI_IPF_REGBASE_ADDR_VIRT);
        u32_addr ++;
    }

    return IPF_SUCCESS;
}

#if !(defined(BSP_CONFIG_HI3630))
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
#endif

/*****************************************************************************
* �� �� ��       : ipf_drx_restore_reg
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
*****************************************************************************/
void ipf_drx_restore_reg(void)
{
    u32* u32_addr;
    u32 i = 0;
    u32 u32_total = 0;
    u32_addr = (u32*)IPF_PWRCTL_INFO_ADDR;

#ifdef CONFIG_BALONG_MODEM_RESET
    if(IPF_FORRESET_CONTROL_ALLOW != g_u32_ipf_ccore_reset_flag)
    {
        return;
    }
#endif

     /* ��λIPF */
    ipf_writel(1, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_SRST_OFFSET); 

    /* �����λ */
    ipf_writel(0, HI_IPF_REGBASE_ADDR_VIRT + HI_IPF_SRST_OFFSET); 

    u32_total = sizeof(g_ipf_setting_table)/sizeof(u32);
    u32_addr = g_u32_ipf_setting_bak_addr;

    for(i=0; i<u32_total; i++)
    {
        ipf_writel(*u32_addr, (g_ipf_setting_table[i]+HI_IPF_REGBASE_ADDR_VIRT));        
        u32_addr ++;
    }
#if !(defined(BSP_CONFIG_HI3630))
    ipf_drx_restore_filter();
#endif
    ipf_drx_restore_adq_rptr();

    u32_total = sizeof(g_ipf_adq_wptr_chctrl_table)/sizeof(u32);
    u32_addr = g_u32_ipf_adq_wptr_chctrl_bak_addr;

    for(i=0; i<4; i++)
    {
        ipf_writel(*u32_addr, (g_ipf_adq_wptr_chctrl_table[i]+HI_IPF_REGBASE_ADDR_VIRT));        
        u32_addr ++;
    }

    ipf_writel(((*u32_addr)|=IPF_BOTH_ADQ_EN), (HI_IPF_CH0_ADQ_CTRL_OFFSET + HI_IPF_REGBASE_ADDR_VIRT));        
    u32_addr ++;
    
    ipf_writel(((*u32_addr)|=IPF_BOTH_ADQ_EN), (HI_IPF_CH1_ADQ_CTRL_OFFSET + HI_IPF_REGBASE_ADDR_VIRT));        
    u32_addr ++;
    for(i=6; i<9; i++)
    {
        ipf_writel(*u32_addr, (g_ipf_adq_wptr_chctrl_table[i]+HI_IPF_REGBASE_ADDR_VIRT));        
        u32_addr ++;

    }
}

/*lint -restore*/
