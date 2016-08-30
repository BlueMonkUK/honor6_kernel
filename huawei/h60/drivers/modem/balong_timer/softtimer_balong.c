
/*lint --e{537,718,746,752,958}*/
#ifdef __KERNEL__
//#include <linux/init.h>
#include <linux/irq.h>
#elif defined(__VXWORKS__)
#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <taskLib.h>
#include <sysLib.h>
#endif
#include <osl_module.h>
#include <osl_sem.h>
#include <osl_spinlock.h>
#include <osl_malloc.h>
#include <soc_clk.h>
#include <bsp_softtimer.h>
#include <bsp_memmap.h>
#include <bsp_om.h>
#include <osl_thread.h>
#include "softtimer_balong.h"
#include "hardtimer_balong.h"
#ifdef __cplusplus
extern "C" 
{
#endif
/*lint --e{129, 63, 64, 409, 49, 52, 502, 574, 569 ,613 , 550} */
int  softtimer_task_func_wake(void *data);
int  softtimer_task_func_normal(void *data);
s32 show_list(u32 wake);

struct softtimer_ctrl
{
    unsigned char timer_id_alloc[SOFTTIMER_MAX_NUM];              /*���֧��40��softtimer��������,���ڷ�����timerId  */
    struct list_head timer_list_head;
    u32 softtimer_start_value;                         /*��¼ÿ������timer������ʼֵ                     */
    u32 hard_timer_id_addr;                            /*��timerʹ�õ�����timer��ַ                     */
    u32 hard_timer_id;                                 /*��timerʹ�õ�����timer id                      */
     /*lint -save -e43*/
    spinlock_t  timer_list_lock;                       /*���������timer����                            */
    osl_sem_id soft_timer_sem;                         /*Ӳtimer�ж��ź���                              */
    SOFTTIMER_TASK_ID softtimer_task;                  /*��¼����������                                 */
    u32 clk;                                           /*��ʹ������timer��ʱ��Ƶ��                       */
};

static struct softtimer_ctrl timer_control[2];         /*timer_control[0] wake, timer_control[1] normal*/

static void start_hard_timer(u32 wake_type,u32 ulvalue )
{
    timer_control[wake_type].softtimer_start_value = ulvalue;
    bsp_hardtimer_disable(timer_control[wake_type].hard_timer_id);
    bsp_hardtimer_load_value(timer_control[wake_type].hard_timer_id,ulvalue);
    bsp_hardtimer_enable(timer_control[wake_type].hard_timer_id);
}

static void stop_hard_timer(u32 wake_type,u32 timer_id)
{
    bsp_hardtimer_disable(timer_id);
    timer_control[wake_type].softtimer_start_value = ELAPESD_TIME_INVAILD;
}
static u32 hard_timer_elapsed_time(u32 wake_type)
{
    u32 ulTempValue = 0;
    if ((u32)ELAPESD_TIME_INVAILD == timer_control[wake_type].softtimer_start_value)
    {
        return 0;
    }
    ulTempValue = bsp_get_timer_current_value(timer_control[wake_type].hard_timer_id);
    return ((timer_control[wake_type].softtimer_start_value < ulTempValue) ? 0 : (timer_control[wake_type].softtimer_start_value - ulTempValue+1));
}
/*lint --e{438,564}*/
void bsp_softtimer_add(struct softtimer_list * timer)
{
    u32 elapsed_time = 0;
    struct softtimer_list *p = NULL;
    unsigned long flags = 0;
    if (NULL == timer)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"timer to be added is NULL\n");
        return;
    }
    spin_lock_irqsave(&(timer_control[timer->wake_type].timer_list_lock),flags);
    /*����Ѿ��ڳ�ʱ�����У��򲻲���*/
    if(!list_empty(&timer->entry))
    {
    	 spin_unlock_irqrestore(&(timer_control[timer->wake_type].timer_list_lock),flags);
    	 return;
    }
    timer->timeout = timer->count_num;
    elapsed_time = hard_timer_elapsed_time(timer->wake_type);
    timer->timeout += elapsed_time;
    /*���timer����Ϊ�գ������ͷ���*/
    if (list_empty(&(timer_control[timer->wake_type].timer_list_head)))
    {
        list_add_tail(&(timer->entry),&(timer_control[timer->wake_type].timer_list_head));
    }
    /*�������Ϊ�գ����ճ�ʱʱ���С�������*/
    else
    {
    	  /*lint -save -e413 *//*lint -save -e613*/
        p=list_first_entry(&(timer_control[timer->wake_type].timer_list_head),struct softtimer_list,entry);
        if((TIMER_TRUE==p->is_running)&&(0==elapsed_time))
        {
            timer->timeout += p->timeout;
        }
	    if(elapsed_time!=0)
    	{
    	     timer->timeout -= 1;
	         elapsed_time     -= 1;
    	}
        list_for_each_entry(p,&(timer_control[timer->wake_type].timer_list_head),entry)
        {
            if(p->timeout <= timer->timeout)
	        {
	            timer->timeout -= p->timeout;
	        }
	        else
	        {
	            break;
	        }
	    }
	    if(&(p->entry)!=&(timer_control[timer->wake_type].timer_list_head))
	    {
	        p->timeout -= timer->timeout;
	    }
	    list_add_tail(&(timer->entry),&(p->entry));
    }
    if (timer_control[timer->wake_type].timer_list_head.next == &(timer->entry))
    {
        timer->timeout -= elapsed_time;
	 if ((timer->entry.next)!=(&(timer_control[timer->wake_type].timer_list_head)))
	 {
	     p = list_entry(timer->entry.next,struct softtimer_list,entry);
	     if(TIMER_TRUE==p->is_running)
	     {
	         p->is_running = TIMER_FALSE;
	     }
	 }
	 timer->is_running = TIMER_TRUE;
	 start_hard_timer(timer->wake_type,timer->timeout);
    }
    spin_unlock_irqrestore(&(timer_control[timer->wake_type].timer_list_lock),flags);
}
s32 bsp_softtimer_delete(struct softtimer_list * timer)
{
    struct softtimer_list * p=NULL;
    unsigned long flags;
    if (NULL == timer)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"NULL pointer \n");
        return ERROR;
    }
    spin_lock_irqsave(&(timer_control[timer->wake_type].timer_list_lock),flags);
    if (list_empty(&timer->entry))
    {
        spin_unlock_irqrestore(&(timer_control[timer->wake_type].timer_list_lock),flags);
        return NOT_ACTIVE;
    }
    else
    {
        if(timer->entry.next == &(timer_control[timer->wake_type].timer_list_head))/*���ɾ���������һ���ڵ㣬ֱ��ɾ��*/
        {
            timer->is_running = TIMER_FALSE;
            list_del_init(&(timer->entry));
        }
	 /*���ɾ�����������е�һ�����,����������������*/
        else if((timer->entry.prev == &(timer_control[timer->wake_type].timer_list_head))
			&&(timer->entry.next != &(timer_control[timer->wake_type].timer_list_head)))
        {
            timer->is_running = TIMER_FALSE;
            list_del_init(&(timer->entry));
			p=list_first_entry(&(timer_control[timer->wake_type].timer_list_head),struct softtimer_list,entry);
			p->timeout += timer->timeout - hard_timer_elapsed_time((u32)timer->wake_type);
			start_hard_timer(p->wake_type,p->timeout);
			p->is_running = TIMER_TRUE;
        }
	 /*���ɾ�������м�ڵ�*/
        else
        {
            p = list_entry(timer->entry.next,struct softtimer_list,entry);
	        p->timeout += timer->timeout;
	        timer->is_running = TIMER_FALSE;
	        list_del_init(&(timer->entry));
        }
    }
    if (list_empty(&(timer_control[timer->wake_type].timer_list_head)))/*���ɾ�����º�����Ϊ�գ���ֹͣ����*/
    {
        stop_hard_timer(timer->wake_type,timer_control[timer->wake_type].hard_timer_id);
    }
    spin_unlock_irqrestore(&(timer_control[timer->wake_type].timer_list_lock),flags);
    return OK;
}

s32 bsp_softtimer_delete_sync(struct softtimer_list * timer)
{
     return bsp_softtimer_delete(timer);
}

s32 bsp_softtimer_modify(struct softtimer_list *timer,u32 new_expire_time)
{
    if((NULL == timer)||(!list_empty(&timer->entry)) )
    {
        return ERROR;
    }
    if(WAKE_SOURCE_CLK==timer_control[timer->wake_type].clk)
    {
	    if((new_expire_time) < (SOFTTIMER_MAX_LENGTH)) 
	    {
	        timer->timeout= (WAKE_SOURCE_CLK* new_expire_time)/1000;
	    }
	    else /* ��ֹ�˷���� */ 
	    {
	        timer->timeout= WAKE_SOURCE_CLK * (new_expire_time/1000);
	    }
	    timer->count_num = timer->timeout;
    }
     else if(NOWAKE_SOURCE_CLK==timer_control[timer->wake_type].clk)
    {
	    timer->timeout= (NOWAKE_SOURCE_CLK/1000)* new_expire_time;
	    timer->count_num = timer->timeout;
    }
    return OK;
}

s32 bsp_softtimer_create(struct softtimer_list *sft_info)
{
    u32 i = 0;
      /*lint -save -e685*/    
    if (check_timer_type((u32)sft_info->wake_type))
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"wake type is error\n");
		return ERROR;
	}
	if(sft_info->init_flags==TIMER_INIT_FLAG)
		return ERROR;
      /*lint -restore +e685*/  
    INIT_LIST_HEAD(&(sft_info->entry));
    sft_info->is_running = TIMER_FALSE;
    sft_info->init_flags=TIMER_INIT_FLAG;
    if(WAKE_SOURCE_CLK==timer_control[sft_info->wake_type].clk)
    {
    	/*����������ֵ���򷵻�ʧ��,��λΪms*/
		if(sft_info->timeout>SOFTTIMER_MAX_LENGTH*1000)
		{
			bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"time too long ,not support\n");
			return ERROR;
		}
		if((sft_info->timeout) < SOFTTIMER_MAX_LENGTH)  /* ��С��SOFTTIMER_MAX_LENGTH�ļ�ʱ�����Ա��־��� */
		{
			sft_info->timeout= (WAKE_SOURCE_CLK*(sft_info->timeout))/1000;
		}
		else /* ��ֹ�˷���� */ 
		{
			sft_info->timeout= WAKE_SOURCE_CLK* ((sft_info->timeout)/1000);
		}	
		sft_info->count_num = sft_info->timeout;
	}
	else if(NOWAKE_SOURCE_CLK==timer_control[sft_info->wake_type].clk)
	{
		/*����������ֵ���򷵻�ʧ��,��λΪms*/
		if(sft_info->timeout>SOFTTIMER_MAX_LENGTH_NORMAL*1000)
		{
			bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"time too long ,not support\n");
			return ERROR;
		}
		sft_info->timeout= (NOWAKE_SOURCE_CLK/1000)* (sft_info->timeout);
		sft_info->count_num = sft_info->timeout;
	}
	for (i=0 ;i < SOFTTIMER_MAX_NUM; i++)
	{
		if (timer_control[sft_info->wake_type].timer_id_alloc[i] == 0)
		{
			sft_info->timer_id = i;
		timer_control[sft_info->wake_type].timer_id_alloc[i] = 1;
		break;
		}
	}
	if (SOFTTIMER_MAX_NUM == i)
	{
		bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"error,not enough timerid for alloc, already 40 exists\n");
		return ERROR;
	}
	return OK;
}
/*lint -restore +e685*/ 

s32 bsp_softtimer_free(struct softtimer_list *p)
{
	 if ((NULL == p)||(!list_empty(&p->entry)))
	{
		return ERROR;
	}
	p->init_flags = 0;
	 timer_control[p->wake_type].timer_id_alloc[p->timer_id] = 0;
	 return OK;   
}

int  softtimer_task_func(void* data)
{
	u32  timer_id = (u32) data;
	struct softtimer_list     *p = NULL;
	unsigned long flags;
	softtimer_func func =NULL;
	u32 para = 0;

	/* coverity[no_escape] */
	for( ; ; )
	{
		/* coverity[sleep] */
		osl_sem_down(&(timer_control[timer_id].soft_timer_sem));
		 /* coverity[lock_acquire] */
		spin_lock_irqsave(&(timer_control[timer_id].timer_list_lock),flags);
		timer_control[timer_id].softtimer_start_value = ELAPESD_TIME_INVAILD;
		if (!list_empty(&(timer_control[timer_id].timer_list_head)))
		{
			p = list_first_entry(&(timer_control[timer_id].timer_list_head),struct softtimer_list,entry);
			if(p->is_running == TIMER_TRUE)
			{
				list_del_init(&p->entry);
				p->is_running = TIMER_FALSE;
				func = p->func;
				para = p->para;
				spin_unlock_irqrestore(&(timer_control[timer_id].timer_list_lock),flags); 
				func(para);
				spin_lock_irqsave(&(timer_control[timer_id].timer_list_lock),flags);
				while(!list_empty(&(timer_control[timer_id].timer_list_head)))
				{
					p=list_first_entry(&(timer_control[timer_id].timer_list_head),struct softtimer_list,entry);
					if(0==p->timeout)
					{
						list_del_init(&p->entry);
						p->is_running = TIMER_FALSE;
						func = p->func;
						para = p->para;
						spin_unlock_irqrestore(&(timer_control[timer_id].timer_list_lock),flags); 
						func(para);
						spin_lock_irqsave(&(timer_control[timer_id].timer_list_lock),flags);
					}
					else
						break;
				}
				if (!list_empty(&(timer_control[timer_id].timer_list_head)))/*�������δ��ʱ��ʱ��*/
				{
					p=list_first_entry(&(timer_control[timer_id].timer_list_head),struct softtimer_list,entry);
					p->is_running = TIMER_TRUE;
					start_hard_timer(timer_id,p->timeout);
				}
				else 
				{  
					stop_hard_timer(timer_id,timer_control[timer_id].hard_timer_id);
				}
			}
			else  if (p->is_running == TIMER_FALSE)
			{
				p->is_running = TIMER_TRUE;
				start_hard_timer(timer_id,p->timeout);
			}
		}
		else
		{
			stop_hard_timer(timer_id,timer_control[timer_id].hard_timer_id);
		}
		spin_unlock_irqrestore(&(timer_control[timer_id].timer_list_lock),flags); 
	} 
	/*lint -save -e527*/ 
	return 0;
	/*lint -restore +e527*/ 
}


OSL_IRQ_FUNC(static irqreturn_t,softtimer_interrupt_call_back,irq,dev)
{    
	u32  timer_id = (u32) dev;
    u32 readValue = 0;    
    /*1����ȡӲ����ʱ�����ж�״̬
	 2��������жϣ������жϣ�ͬʱ�ͷ��ź���
   */
    readValue = bsp_hardtimer_int_status(timer_control[timer_id].hard_timer_id);
    if (0 != readValue)
    {
        bsp_hardtimer_int_clear(timer_control[timer_id].hard_timer_id);
        osl_sem_up(&(timer_control[timer_id].soft_timer_sem));
    }
    return IRQ_HANDLED;
}


int  bsp_softtimer_init(void)
{
    s32 ret = 0;
    u32 array_size=0 ;
    /* coverity[var_decl] */
    struct bsp_hardtimer_control timer_ctrl ;
    INIT_LIST_HEAD(&(timer_control[SOFTTIMER_WAKE].timer_list_head));
    INIT_LIST_HEAD(&(timer_control[SOFTTIMER_NOWAKE].timer_list_head));
    timer_control[SOFTTIMER_NOWAKE].hard_timer_id_addr = ST_NORMAL_HARDTIMER_ADDR;
    timer_control[SOFTTIMER_WAKE].hard_timer_id_addr     = ST_WAKE_HARDTIMER_ADDR;
    timer_control[SOFTTIMER_NOWAKE].hard_timer_id      = ST_NORMAL_HARDTIMER_ID;
    timer_control[SOFTTIMER_WAKE].hard_timer_id          = ST_WAKE_HARDTIMER_ID;
    timer_control[SOFTTIMER_NOWAKE].clk                = NOWAKE_TIMER_CLK;
    timer_control[SOFTTIMER_WAKE].clk                    = WAKE_TIMER_CLK;
    array_size = sizeof(u8)*SOFTTIMER_MAX_NUM;/*lint !e419 */
    memset(timer_control[SOFTTIMER_WAKE].timer_id_alloc,0,array_size);/*lint !e419 */
    memset(timer_control[SOFTTIMER_NOWAKE].timer_id_alloc,0,array_size);/*lint !e419 */
    timer_control[SOFTTIMER_WAKE].softtimer_start_value  = ELAPESD_TIME_INVAILD;
    timer_control[SOFTTIMER_NOWAKE].softtimer_start_value = ELAPESD_TIME_INVAILD;
    osl_sem_init(SEM_EMPTY,&(timer_control[SOFTTIMER_NOWAKE].soft_timer_sem));
    osl_sem_init(SEM_EMPTY,&(timer_control[SOFTTIMER_WAKE].soft_timer_sem));
    spin_lock_init(&(timer_control[SOFTTIMER_WAKE].timer_list_lock));
    spin_lock_init(&(timer_control[SOFTTIMER_NOWAKE].timer_list_lock));
    timer_ctrl.func=(timer_func)softtimer_interrupt_call_back;
    timer_ctrl.mode=TIMER_ONCE_COUNT;
    if (OK==check_timer_type((u32)SOFTTIMER_WAKE))
    {
        if(ERROR == osl_task_init("softtimer_wake", TIMER_TASK_WAKE_PRI, TIMER_TASK_STK_SIZE ,(void *)softtimer_task_func, (void*)SOFTTIMER_WAKE,
			&timer_control[SOFTTIMER_WAKE].softtimer_task))
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"softtimer_wake task create failed\n");
            return ERROR;
        }
        timer_ctrl.para=SOFTTIMER_WAKE;
        timer_ctrl.timerId =ST_WAKE_HARDTIMER_ID;
        ret =bsp_hardtimer_alloc(&timer_ctrl);
        if (ret)
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"bsp_hardtimer_alloc error,softtimer init failed 2\n");
            return ERROR;
        }
    }
     if (OK==check_timer_type((u32)SOFTTIMER_NOWAKE))
     {
         if(ERROR == osl_task_init("softtimer_nowake", TIMER_TASK_NOWAKE_PRI, TIMER_TASK_STK_SIZE ,(void *)softtimer_task_func, (void*)SOFTTIMER_NOWAKE,
			&timer_control[SOFTTIMER_NOWAKE].softtimer_task))
            {
                bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"softtimer_normal task create failed\n");
                return ERROR;
            }
        timer_ctrl.para=SOFTTIMER_NOWAKE;
        timer_ctrl.timerId =ST_NORMAL_HARDTIMER_ID;
	/* coverity[uninit_use_in_call] */
         ret =bsp_hardtimer_alloc(&timer_ctrl);
        if (ret)
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"bsp_hardtimer_alloc error,softtimer init failed 2\n");
            return ERROR;
        }
     }
     bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"softtimer init success\n");
    return OK;
}
/*lint -restore +e550*/

/*lint -save -e413*/

s32 show_list(u32 wake_type)
{
    struct softtimer_list * timer = NULL;
    unsigned long flags = 0;
    bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"timer_id\t timeout \t time_count_num \t is_running\n");
    bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"----------------------------------------------------------------------------------\n");
    spin_lock_irqsave(&(timer_control[wake_type].timer_list_lock),flags); 
    list_for_each_entry(timer,&(timer_control[wake_type].timer_list_head),entry)
    {
        bsp_trace(BSP_LOG_LEVEL_ERROR,BSP_MODU_SOFTTIMER,"%d \t %d \t\t %d \t\t %d\n",timer->timer_id,timer->timeout,timer->count_num,timer->is_running);
    }
     spin_unlock_irqrestore(&(timer_control[wake_type].timer_list_lock),flags); 
    return OK;
}
/*lint -restore +e413*/

/*lint -save -e19*/

EXPORT_SYMBOL(bsp_softtimer_create);
EXPORT_SYMBOL(bsp_softtimer_delete);
EXPORT_SYMBOL(bsp_softtimer_delete_sync);
EXPORT_SYMBOL(bsp_softtimer_modify);
EXPORT_SYMBOL(bsp_softtimer_add);
EXPORT_SYMBOL(bsp_softtimer_free);
arch_initcall(bsp_softtimer_init);

/*lint -restore +e19*/

/*lint -restore +e43*/
/*lint -restore +e527*/
/*lint -restore +e413*//*lint -restore +e613*/


#ifdef __cplusplus
}
#endif


