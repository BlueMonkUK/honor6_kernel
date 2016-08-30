

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
/*lint -save -e537 -e451*/

#include <semLib.h>
#include <vxWorks.h>
#include <logLib.h>
#include <errnoLib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "osl_sem.h"
#include "drv_om.h"
#include <bsp_uart.h>
#include "bsp_om_api.h"
#include "bsp_om.h"
#include "bsp_socp.h"
#include "bsp_om_server.h"
#include "bsp_bbp.h"
#include "bsp_om_log.h"
/* add time sample yangzhi 8-16 B:*/
#include "bsp_hardtimer.h"
/* add time sample yangzhi 8-16 E!*/
/*lint -restore*/
#define    THIS_MODU_ID        BSP_MODU_LOG

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

bsp_log_swt_cfg_s  g_mod_peint_level_info[BSP_MODU_MAX]    =
{
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR},
    {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}, {BSP_LOG_LEVEL_ERROR}
};
u32 g_get_buf_fail = 0;

log_bin_ind_cb        g_log_ind_cb = NULL;

u32 bsp_log_level_set(bsp_log_level_e log_level);
void bsp_log_header_packet(bsp_module_e mod_id,bsp_log_level_e log_level,u8 *print_buf,u32 buf_size);
void bsp_log_show(void);

/* add time sample yangzhi 8-16 B:*/
char log_buf_temp[BSP_LOG_BUF_LEN] = {0};

bsp_log_buf_ctrl_s g_log_buf_ctrl;
bool log_init_flag = FALSE;
/* add time sample yangzhi 8-16 E!*/
/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/
/*****************************************************************************
* �� �� ��  : bsp_log_module_cfg_set
*
* ��������  : HSO���õ����ӡ��������
*
* �������  : log_swt_stru:����ģ��Ĵ�ӡ����ֵ
*                         data_len:      ����log_swt_stru�ĳ���
* �������  : ��
*
* �� �� ֵ  : BSP_OK �ɹ�;���� ʧ��
*****************************************************************************/

u32 bsp_log_module_cfg_set(bsp_log_swt_cfg_s *log_swt_stru ,u32 data_len)
{
    u32 mod_num = 0;
    u32 i;

    if((NULL == log_swt_stru )||( 0 == data_len)||((data_len % sizeof(bsp_log_swt_cfg_s) != 0)))
    {
        bsp_om_debug(BSP_LOG_LEVEL_ERROR," bsp_log_module_cfg_set error!!  data_len = %d\n",data_len);
       return BSP_ERR_LOG_INVALID_PARAM;
    }

    mod_num = data_len / sizeof(bsp_log_swt_cfg_s);

    if(mod_num > BSP_MODU_MAX )
    {
        bsp_om_debug(BSP_LOG_LEVEL_ERROR," bsp_log_module_cfg_set error!!  mod_num = %d\n",mod_num);
        return BSP_ERR_LOG_INVALID_MODULE;
    }

    for(i = 0; i < mod_num; i++)
    {
        if(log_swt_stru[i].print_level <= BSP_LOG_LEVEL_MAX)
        {
            g_mod_peint_level_info[i].print_level = log_swt_stru[i].print_level;

        }
        else
        {
            bsp_om_debug(BSP_LOG_LEVEL_ERROR," bsp_log_module_cfg_set   log_swt_stru[i].print_level = %d\n",log_swt_stru[i].print_level);
            return BSP_ERR_LOG_INVALID_LEVEL ;
        }

    }

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : bsp_log_module_cfg_get
*
* ��������  : ��ѯģ�鵱ǰ���õĴ�ӡ����
*
* �������  : mod_id:����ѯģ��ID
*
* �������  : ��
*
* �� �� ֵ  : ��ӡ����
*****************************************************************************/

u32 bsp_log_module_cfg_get(bsp_module_e mod_id)
{
    if(mod_id >= BSP_MODU_MAX )
    {
        return BSP_ERR_LOG_INVALID_MODULE;
    }

    return g_mod_peint_level_info[mod_id].print_level;
}

/*****************************************************************************
* �� �� ��  : bsp_mod_level_set
*
* ��������  : ���õ���ģ��Ĵ�ӡ����
*
* �������  : mod_id:ģ��ID
*                         print_level: ��ӡ����
* �������  : ��
*
* �� �� ֵ  : BSP_OK ���óɹ�������  ����ʧ��
*****************************************************************************/

u32 bsp_mod_level_set(bsp_module_e  mod_id ,u32 print_level)
{
    if(mod_id >= BSP_MODU_MAX )
    {
        return BSP_ERR_LOG_INVALID_MODULE;
    }

    if(print_level > BSP_LOG_LEVEL_MAX)
    {
        return BSP_ERR_LOG_INVALID_LEVEL;
    }

    g_mod_peint_level_info[mod_id].print_level = print_level;

    return BSP_OK;
}

/*****************************************************************************
* �� �� ��  : bsp_log_level_set
*
* ��������  : ��������ģ��Ĵ�ӡ����
*
* �������  : print_level: ��ӡ����
*
* �������  : ��
*
* �� �� ֵ  : BSP_OK ���óɹ�������  ����ʧ��
*****************************************************************************/
u32 bsp_log_level_set(bsp_log_level_e log_level)
{
    u32 mod_id = 0;

    if(log_level >BSP_LOG_LEVEL_MAX)
    {
        return BSP_ERR_LOG_INVALID_LEVEL;
    }

    for(mod_id = 0; mod_id < BSP_MODU_MAX; mod_id++)
    {
        g_mod_peint_level_info[mod_id].print_level = log_level;
    }

    return BSP_OK;
}


/*****************************************************************************
* �� �� ��  : bsp_log_level_reset
*
* ��������  : ������ģ��Ĵ�ӡ��������ΪĬ��ֵ
*
* �������  : ��
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/

void bsp_log_level_reset(void)
{
    u32 i;

    for(i = 0; i < BSP_MODU_MAX;i++)
    {
        g_mod_peint_level_info[i].print_level= BSP_LOG_LEVEL_ERROR;
    }

}

/*****************************************************************************
* �� �� ��  : bsp_log_header_packet
*
* ��������  : ��ӡ���ݰ�ͷ��Ϣ���
*
* �������  :  mod_id: ���ģ��
*                           print_level: ��ӡ����
*                           print_buf :����bufָ��
*                           buf_size: ���������buf����
* �������  : ��
*
* �� �� ֵ  : BSP_OK ����ɹ�
*****************************************************************************/

void bsp_log_header_packet(bsp_module_e mod_id,bsp_log_level_e log_level,u8 *print_buf,u32 buf_size)
{

    bsp_om_head_s        *bsp_om_header  = NULL;
    bsp_trace_txt_s  *print_header   = NULL;

   /*���SOCPͷ*/
    PACKET_BSP_SOCP_HEAD(print_buf);

    bsp_om_header = (bsp_om_head_s *)(print_buf + sizeof(bsp_socp_head_s));

    /*lint -save -e648*/
    bsp_om_header->om_id            = ((BSP_STRU_ID_28_31_GROUP_BSP << 28) | (BSP_STRU_ID_16_23_BSP_PRINT << 16));
    /*lint -restore +e648*/
    bsp_om_header->data_size        = buf_size - sizeof(bsp_socp_head_s)-sizeof(bsp_om_head_s);

    print_header = ( bsp_trace_txt_s  *)(print_buf + sizeof(bsp_socp_head_s) + sizeof(bsp_om_head_s));
    print_header->mod_id            = mod_id;
    print_header->level             = log_level;
    print_header->log_sn            = g_om_global_info.print_sn++;

    return ;

}


/*****************************************************************************
* �� �� ��  : bsp_trace
*
* ��������  : �����ӡ�������ӿ�
*
* �������  :  mod_id: ���ģ��
*                           print_level: ��ӡ����
*                           fmt :��ӡ�������
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
void bsp_trace(bsp_log_level_e log_level, bsp_module_e mod_id,char *fmt,...)
{
    va_list arglist;
    s32 _args[6];
    s32 i;
    int flag;
	/* add time sample yangzhi 8-16 B:*/
    u32 time_cur =0;
    int sprint_len = 0;
    int fmt_len = 0;
    int buf_back_len = 0;
    char  buf_trans_temp[BSP_TRACE_BUF_LEN_MAX+1] = {0};
    char* ptr_log_msg = NULL;
	/* add time sample yangzhi 8-16 E!*/
	
    if(mod_id >= BSP_MODU_MAX )
    {
        return ;
    }

    if(g_mod_peint_level_info[mod_id].print_level > log_level )
    {
        return ;
    }
	
	/* add time sample yangzhi 8-16 B:*/
	/*lint -save -e530*/
    if(FALSE == log_init_flag)
    {
        g_log_buf_ctrl.buf_offset= 0;
        memset(log_buf_temp,0,BSP_LOG_BUF_LEN);
        g_log_buf_ctrl.write_ptr = log_buf_temp;
        log_init_flag = TRUE;
    }
    time_cur = bsp_get_slice_value();
    sprint_len = snprintf(buf_trans_temp,BSP_TRACE_BUF_LEN_MAX,"[%08x(dec:%d)]",time_cur,time_cur);
    if(sprint_len > BSP_TRACE_BUF_LEN_MAX)
    {
        return ;
    }
    va_start(arglist, fmt);
    /*lint -restore +e530*/
    for(i = 0; i < 6; i++)
    {
        _args[i] = va_arg(arglist, s32);
    }
    va_end(arglist);

    fmt_len = snprintf(buf_trans_temp+sprint_len,BSP_TRACE_BUF_LEN_MAX - sprint_len,fmt,_args[0],_args[1],\
        _args[2],_args[3],_args[4],_args[5]);
    if(sprint_len > BSP_TRACE_BUF_LEN_MAX)
    {
        return ;
    }
    fmt_len = fmt_len +sprint_len;
	buf_trans_temp[fmt_len +1] = '\0';
    fmt_len +=1;
    flag = intLock();
     /* �����ǰ�������жϻ������ж�������*/
    if(VXWORKS_INT_LOCK_FLAG&flag)
    {
        printksync((char*)buf_trans_temp,0,0,0,0,0,0);
    }
    else
    {
        if(NULL != g_log_buf_ctrl.write_ptr)
        {
            ptr_log_msg = g_log_buf_ctrl.write_ptr;
            
			/* if tail of the buffer*/
            if((g_log_buf_ctrl.buf_offset + fmt_len)>= BSP_LOG_BUF_LEN)
            {
                buf_back_len = BSP_LOG_BUF_LEN-g_log_buf_ctrl.buf_offset;
                memcpy(g_log_buf_ctrl.write_ptr, buf_trans_temp, buf_back_len);
                
                /* set to head of buffer */
                g_log_buf_ctrl.write_ptr = log_buf_temp;
                memcpy(g_log_buf_ctrl.write_ptr, buf_trans_temp+buf_back_len, (fmt_len - buf_back_len));
                buf_back_len = fmt_len - buf_back_len;
                g_log_buf_ctrl.buf_offset= buf_back_len;
                g_log_buf_ctrl.write_ptr +=buf_back_len;
            }
            else
            {
                memcpy(g_log_buf_ctrl.write_ptr,buf_trans_temp,fmt_len);
                g_log_buf_ctrl.write_ptr +=fmt_len;
                g_log_buf_ctrl.buf_offset += fmt_len; 
            }
            logMsg(ptr_log_msg, 0,0,0,0,0,0); 
        }
        else
        {
            logMsg((char*)buf_trans_temp, 0,0,0,0,0,0);
        }
    }
    /* add time sample yangzhi 8-16 E!*/
	
    intUnlock(flag);

    return ;
}

/*****************************************************************************
* �� �� ��  : bsp_log_bin_ind
*
* ��������  : ���������ϱ��ӿ�
*
* �������  :  str_id: ��HSO�����Ľṹ��ID
*                           ind_data_size: �ϱ��Ŀ�ά�ɲ���Ϣ����
*                           ind_data :�ϱ��Ŀ�ά�ɲ���Ϣ
*
* �������  : ��
*
* �� �� ֵ  : ��
*****************************************************************************/
u32 log_ind_enter_cnt=0;
u32 log_ind_exit_cnt=0;
void bsp_log_bin_ind(s32 str_id, void* ind_data, u32 ind_data_size)
{
    u32 pbuf = 0;
    u32 buflen = 0;
    bsp_om_head_s        *bsp_om_header  = NULL;
    log_ind_enter_cnt ++;

    if((NULL == ind_data) ||(0 == ind_data_size) || (ind_data_size > BSP_DIAG_IND_DATA_MAX_LEN))
    {
        return ;
    }

    if( TRUE != bsp_om_get_hso_conn_flag())
    {
        return ;
    }

    buflen = sizeof(bsp_socp_head_s)+sizeof(bsp_om_head_s)+ind_data_size ;

    /*��ȡbuf*/
    if(bsp_om_buf_sem_take())
    {
        return;
    }
    pbuf = bsp_om_get_buf(BSP_OM_SOCP_BUF_TYPE,buflen);

    if(0 == pbuf)
    {
        bsp_om_buf_sem_give();
        return ;
    }

    /*���SOCPͷ*/
    PACKET_BSP_SOCP_HEAD(pbuf);

    bsp_om_header = (bsp_om_head_s*)(pbuf + sizeof(bsp_socp_head_s));

    bsp_om_header->om_id            = (BSP_STRU_ID_28_31_GROUP_MSP << 28) | (BSP_STRU_ID_16_23_BSP_CMD_IND << 16) |(u16)str_id;

    memcpy((u8*)bsp_om_header +sizeof(bsp_om_head_s),ind_data,ind_data_size);

    if(BSP_OK != bsp_om_into_send_list(pbuf,buflen))
    {
        bsp_om_free_buf(pbuf,buflen);
    }

    bsp_om_buf_sem_give();
    log_ind_exit_cnt ++;
}


/*debug �ӿ�*/
void bsp_log_show(void)
{
    printk("trace level              = %d\n",g_mod_peint_level_info[0].print_level);
    printk("hso_connect_flag   = %d\n",g_om_global_info.hso_connect_flag);
    printk("print_sn                  = %d\n",  g_om_global_info.print_sn);
    printk("git buf fail               = %d\n",  g_get_buf_fail);

    printk("log_ind_enter_cnt = %d\n",log_ind_enter_cnt);
    printk("log_ind_exit_cnt = %d\n",log_ind_exit_cnt);

}


