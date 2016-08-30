/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : New0001.h
  �� �� ��   : ����
  ��    ��   : ��ӭ�� 61362
  ��������   : 2013��1��11��
  ����޸�   :
  ��������   : �ṩDSDA SCI��Ӳ������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��1��11��
    ��    ��   : ��ӭ�� 61362
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
/* lint --e{729,830,569,737,537,525}*/
#include <stdlib.h>
#include <string.h>
#include "cacheLib.h"
#include "product_config.h"
#include "drv_nv_id.h"
#include "regulator_balong.h"
#include "bsp_edma.h"
#include "bsp_nvim.h"
#include "bsp_sci.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/*��¼NV_SCI��ȫ�ֱ���*/
#if (FEATURE_OFF==FEATURE_MULTI_MODEM)
unsigned int g_SciNVValue = 0;   		/* default */
#else
unsigned int g_SciNVValue = 0x802;      /* default */
#endif

/*��¼�쳣�ļ�·����ȫ�ֱ���*/
const char * g_pcCardRecordFileName[] = {SCI0_RECORD_LOG_PATH_LOST_CARD,
                                         SCI1_RECORD_LOG_PATH_LOST_CARD};

/*��¼��������ȫ�ֱ���*/
const char * g_pcCardTaskName[] = {SCI_CARD0_SYNC_TASK_NAME,
                                   SCI_CARD1_SYNC_TASK_NAME};

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
/*****************************************************************************
 �� �� ��  : bsp_sci_get_nv
 ��������  : ��ȡSCI��NV��ֵ
 �������  : 
 �������  : ��
 �� �� ֵ  : int
 ���ú���  :
 ��������  :

 �޸���ʷ      :

*****************************************************************************/
int bsp_sci_get_nv(unsigned int *value)
{
    /*����NV���ӿ�*/
    if(BSP_OK == bsp_nvm_read(NV_ID_DRV_SCI_DSDA_SELECT, (u8*)&g_SciNVValue, NV_SCI_LEN)) 
    {
#if (FEATURE_ON==FEATURE_MULTI_MODEM)
        if(0x802 !=g_SciNVValue && 0x102 !=g_SciNVValue)
        {
            g_SciNVValue = 0x802;
        }
#endif
        /*��ȡ�ɹ�*/
        *value = g_SciNVValue;
        
        sci_print_debug("nv value:0x%x\r\n",g_SciNVValue);
        return BSP_OK;
    }
    else 
	{
#if (FEATURE_ON==FEATURE_MULTI_MODEM)
        *value = 0x802;
#else 
        /*��ȡʧ��,Ĭ���ǵ���0*/
        *value = 0x0;
#endif
        sci_print_error("default value is 0x%x\r\n", 0);
        return BSP_ERROR;
    }
    
}

/*****************************************************************************
 �� �� ��  : bsp_sci_cfg
 ��������  : ����Ӳ����ص�����
 �������  : unsigned int card_id
             unsigned int sci_id
 �������  : ��
 �� �� ֵ  : int
 ���ú���  :
 ��������  : BSP_SCI_ParseNV

 �޸���ʷ      :

*****************************************************************************/
int bsp_sci_cfg(unsigned int card_id, unsigned int sci_id)
{
    SCI_CFG_STRU * p_sci_cfg;

    if (card_id >= (unsigned int)(&_sci_init_data_end - &_sci_init_data_start)) 
	{
		sci_print_error("card_id is too large!\r\n");
        return BSP_ERROR;
    }

    p_sci_cfg = (SCI_CFG_STRU *)((int)&_sci_cfg_data_start + \
                                (int)(sizeof(SCI_CFG_STRU)*card_id));

    p_sci_cfg->card_id = card_id;
    if (CARD_CONNECT_SCI0 == sci_id) 
    {  
        /* regs base\ int \ clk */
        p_sci_cfg->base_addr  = HI_SCI0_REGBASE_ADDR;
        p_sci_cfg->int_number = INT_LVL_SCI0;
        p_sci_cfg->sci_id     = sci_id;
        p_sci_cfg->dma_chan   = EDMA_SCI0_RX;     
        p_sci_cfg->clk_en_reg  = HI_CRG_CLKEN4_OFFSET;
        p_sci_cfg->clk_en_bit  = SCI0_CLK_SET_BIT_SHIFT;
        p_sci_cfg->clk_dis_reg = HI_CRG_CLKDIS4_OFFSET;        
        p_sci_cfg->clk_dis_bit = SCI0_CLK_SET_BIT_SHIFT; 

        /* PMU */
        p_sci_cfg->pwr_type = CARD_PM_TYPE_SIM0;

        /*�쳣��¼��ص����ã�������¼���Ļ���ַ�Լ���¼�ļ�·��*/
        p_sci_cfg->record_enum= CARD0_EXCP_RECORD_ADDR_NUM;
#ifdef BSP_CONFIG_HI3630
        p_sci_cfg->icc_chnl     = ((ICC_CHN_MCORE_CCORE << 16) | MCORE_CCORE_FUNC_SIM0);
#else
        p_sci_cfg->icc_chnl     = ((ICC_CHN_IFC<< 16) | IFC_RECV_FUNC_SIM0);
#endif
        p_sci_cfg->record_file= (char *)g_pcCardRecordFileName[card_id];

        /* task */
        p_sci_cfg->syncTaskName   = (char *)g_pcCardTaskName[card_id];

        /*��0ͶƱ*/
        p_sci_cfg->sleep_vote = PWRCTRL_SLEEP_SCI0;
        
    }
    else if (CARD_CONNECT_SCI1 == sci_id) {
        p_sci_cfg->base_addr  = HI_SCI1_REGBASE_ADDR;
        p_sci_cfg->sci_id     = sci_id;
        p_sci_cfg->int_number = INT_LVL_SCI1;
        p_sci_cfg->dma_chan    = EDMA_SCI1_RX;  
        p_sci_cfg->clk_en_reg  = HI_CRG_CLKEN4_OFFSET;
        p_sci_cfg->clk_en_bit  = SCI1_CLK_SET_BIT_SHIFT;
        p_sci_cfg->clk_dis_reg = HI_CRG_CLKDIS4_OFFSET;        
        p_sci_cfg->clk_dis_bit = SCI1_CLK_SET_BIT_SHIFT;

        /* PMU */
        p_sci_cfg->pwr_type = CARD_PM_TYPE_SIM1;

        /*�쳣��¼��ص����ã�������¼���Ļ���ַ�Լ���¼�ļ�·��*/
        p_sci_cfg->record_enum = CARD1_EXCP_RECORD_ADDR_NUM;
#ifdef BSP_CONFIG_HI3630
        p_sci_cfg->icc_chnl = ((ICC_CHN_MCORE_CCORE << 16) | MCORE_CCORE_FUNC_SIM1);
#else
        p_sci_cfg->icc_chnl = ((ICC_CHN_IFC<< 16) | IFC_RECV_FUNC_SIM1);
#endif

        p_sci_cfg->record_file = (char *)g_pcCardRecordFileName[card_id];

        /* task */
        p_sci_cfg->syncTaskName   = (char *)g_pcCardTaskName[card_id];

        /*��0ͶƱ*/
        p_sci_cfg->sleep_vote = PWRCTRL_SLEEP_SCI1;
    }
    else 
    {
    	sci_print_error("card_id error!\r\n");
        return BSP_ERROR;
    }
    

    return BSP_OK;
}

/*****************************************************************************
 �� �� ��  : BSP_SCI_ParseNV
 ��������  : ����NV
 �������  : NV_SCI_CFG_UNION * nv
 �������  : ��
 �� �� ֵ  : int
 ���ú���  :
 ��������  : BSP_SCI_CfgInit

 �޸���ʷ      :

*****************************************************************************/
int bsp_sci_parse_nv(NV_SCI_CFG_UNION * nv)
{
    NV_SCI_CFG_UNION *p_nv_sci = nv;

    if (SCI_SINGLE == p_nv_sci->cfg.card_num) 
	{
        /*����0����*/
        if (p_nv_sci->cfg.card0 <= CARD_CONNECT_SCI1) 
        {
            sci_print_debug("card0 connect sci%d\r\n",p_nv_sci->cfg.card0);
            if(BSP_OK != bsp_sci_cfg(CARD0, p_nv_sci->cfg.card0))
            {
            	sci_print_error("cfg sci%d failed! \r\n",p_nv_sci->cfg.card0);
				return BSP_ERROR;
			}
        }
        else 
		{
            /*�ݲ�֧�ֶ࿨*/
			sci_print_error("do not support multi card! case 1\n");
            return ERROR;
        }
    }
    else if(SCI_SINGLE_MODLEM1 == p_nv_sci->cfg.card_num)
    { /*����1����*/
        if (p_nv_sci->cfg.card1 <= CARD_CONNECT_SCI1) 
        {
            sci_print_debug("SCI_SINGLE_MODLEM1  connect sci%d\r\n",p_nv_sci->cfg.card1);
			if(BSP_OK != bsp_sci_cfg(CARD1, p_nv_sci->cfg.card1))
            {
            	sci_print_error("cfg sci%d failed! \r\n",p_nv_sci->cfg.card1);
				return BSP_ERROR;
			}
        }
        else 
		{
            /*�ݲ�֧�ֶ࿨*/
			sci_print_error("do not support multi card! case 2\n");
            return ERROR;
        }
    }
    else if (SCI_DUAL == p_nv_sci->cfg.card_num) 
    {
        /*˫������*/
        if ((p_nv_sci->cfg.card0 <= CARD_CONNECT_SCI1) &&
            (p_nv_sci->cfg.card1 <= CARD_CONNECT_SCI1) &&
            (p_nv_sci->cfg.card0 != p_nv_sci->cfg.card1)) 
        {
            sci_print_debug("dual cards, card0 connect sci%d,card1 connect sci%d\r\n",p_nv_sci->cfg.card0, p_nv_sci->cfg.card1);
			if(BSP_OK != bsp_sci_cfg(CARD0, p_nv_sci->cfg.card0))
            {
            	sci_print_error("bsp_sci_parse_nv, total cfg sci%d failed! \r\n",p_nv_sci->cfg.card0);
				return BSP_ERROR;
			}
			if(BSP_OK != bsp_sci_cfg(CARD1, p_nv_sci->cfg.card1))
            {
            	sci_print_error(" total cfg sci%d failed! \r\n",p_nv_sci->cfg.card1);
				return BSP_ERROR;
			}
        }
        else 
		{
            /*�ݲ�֧�ֶ࿨*/
			sci_print_error("do not support multi card! case 3\n");
            return BSP_ERROR;
        }
    }
    else 
	{
        /*�ݲ�֧�ֶ࿨*/
		sci_print_error(" do not support multi card! case 4\n");
        return BSP_ERROR;
    }

    return BSP_OK;
}


/*****************************************************************************
 �� �� ��  : BSP_SCI_CfgInit
 ��������  : SCI�ܵĳ�ʼ����������ʼ���׶ν���Ӳ�����ã���������Ҫ������ص�
             SCI������ʼ��
 �������  : void
 �������  : ��
 �� �� ֵ  : int
 ���ú���  :
 ��������  : BSP_DRV_Init

 �޸���ʷ      :
  1.��    ��   : 2013��1��11��
    ��    ��   : ��ӭ�� 61362
    �޸�����   : �����ɺ���

*****************************************************************************/
int bsp_sci_cfg_init(void)
{
    unsigned int i = 0;
    unsigned int init_func_num;
    /* coverity[assign] */
    int *p_init_start = (int *)&_sci_init_data_start;
    int call_addr;
    NV_SCI_CFG_UNION nv_sci;
    SCI_INIT_FUNCP p_func_sci_init;
    int card_type = 0;

    /*��ȡNV_SCI��ֵ*/
    (void)bsp_sci_get_nv(&nv_sci.value);

    /*����NV_SCI*/
    if (BSP_ERROR == bsp_sci_parse_nv(&nv_sci)) 
	{
		sci_print_error("parse nv failed!\n");
        return BSP_ERROR;
    }

    init_func_num = &_sci_init_data_end - &_sci_init_data_start;

    card_type = ((NV_SCI_CFG_UNION *)&g_SciNVValue)->cfg.card_num; 
    if(SCI_SINGLE == card_type)
    {
        init_func_num= 1;
    }
    else if (SCI_SINGLE_MODLEM1 == card_type)
    {
         init_func_num= 1;
         /* coverity[ptr_arith] */
         p_init_start++;
    }
    
    sci_print_debug("init_func_num %d  start_add_opp 0x%0x\r\n", init_func_num,(p_init_start-(int*)&_sci_init_data_start));
        
    /*����SCI�ĳ�ʼ��*/
    for (i = 0; i < init_func_num; i++) 
	{
        call_addr = *(volatile int *)p_init_start;
        p_func_sci_init = (SCI_INIT_FUNCP)call_addr;
        sci_print_debug("call init function of card%d, address is 0x%x\r\n",i, call_addr);
        p_func_sci_init();
        /* coverity[ptr_arith] */
        p_init_start++;
    }

    return BSP_OK;
}



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

