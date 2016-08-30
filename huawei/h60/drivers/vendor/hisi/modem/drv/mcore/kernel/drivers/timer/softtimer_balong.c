
/*lint --e{438,537,732}*/
#include <osl_common.h>
#include <bsp_softtimer.h>
#include <soc_clk.h>
#include <bsp_memmap.h>
#include <bsp_om.h>
#include <bsp_hardtimer.h>
#include <soc_timer.h>
#include "softtimer_balong.h"
#ifdef __cplusplus
extern "C" 
{
#endif
#ifdef K3V3_LPM3_HAS_MODEM_FEATURE
#define  softtimer_print(fmt, ...)    (printk("[softtimer]: <%s> "fmt"\n", __FUNCTION__, ##__VA_ARGS__))
#else
#define  softtimer_print(fmt, ...)    (bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_SOFTTIMER, "[softtimer]: <%s> "fmt"\n", __FUNCTION__, ##__VA_ARGS__))
#endif
/*lint --e{133 } */
osSemaphoreDef(soft_timer_sem);

static void thread_softtimer_fun(void const *obj);
osThreadDef(thread_softtimer_fun, osPriorityAboveNormal, 1, 1024);
#ifdef K3V3_LPM3_HAS_MODEM_FEATURE
void IntTimerHandler(void);
#else
irqreturn_t IntTimerHandler(void);
#endif

struct softtimer_ctrl
{
    struct list_head timer_list_head;
    u32 softtimer_start_value;                         /*��¼ÿ������timer������ʼֵ                     */
    u32 hard_timer_id_addr;                            /*��timerʹ�õ�����timer��ַ                     */
    osThreadId thread_softtimer_id;                /*��¼����������                                 */
    osSemaphoreId      soft_timer_sem;    
    u32 clk;                                           /*��ʹ������timer��ʱ��Ƶ��                       */
};

static struct softtimer_ctrl timer_control;       
#ifdef K3V3_LPM3_HAS_MODEM_FEATURE
static u32 __inline__ timer_get_value(void)
{
	u32 ret = 0;
	ret=readl(PM_MCU_TIMER_BASE + 0x28);
	if(ret&0x80)
	{
		ret=readl(PM_MCU_TIMER_BASE + 0x24);
		if(ret >timer_control.softtimer_start_value)
			return timer_control.softtimer_start_value;
		else
			return ret;
	}
	else
		return 0;
}
static void start_hard_timer(u32 ulvalue )
{
    u32 ret = 0;
    timer_control.softtimer_start_value = ulvalue;
    ret = readl(PM_MCU_TIMER_BASE+0x28);
    writel(ret&(~0x80),PM_MCU_TIMER_BASE+0x28);
    writel(ulvalue, PM_MCU_TIMER_BASE + 0x20);
    ret = readl(PM_MCU_TIMER_BASE+0x28);
    writel(ret |0xA2, PM_MCU_TIMER_BASE +0x28);
}
static void stop_hard_timer()
{
    u32 ret = 0;
    ret = readl(PM_MCU_TIMER_BASE+0x28);
    writel(ret&(~0x80),PM_MCU_TIMER_BASE+0x28);
    timer_control.softtimer_start_value = ELAPESD_TIME_INVAILD;
}
#else
static u32 __inline__ timer_get_value(void)
{
	u32 ret = 0;
	ret = readl(PM_MCU_TIMER_BASE + HI_TIMER_CONTROLREG_OFFSET); 
	if(ret&0x1)
	{
		ret=readl(PM_MCU_TIMER_BASE + HI_TIMER_CURRENTVALUE_OFFSET);
		if(ret >timer_control.softtimer_start_value)
			return timer_control.softtimer_start_value;
		else
			return ret;
	}
	else
		return 0;
	
}

static void start_hard_timer(u32 ulvalue )
{
    u32 ret = 0;
    timer_control.softtimer_start_value = ulvalue;
    writel(0x0, PM_MCU_TIMER_BASE + HI_TIMER_CONTROLREG_OFFSET);
    while(readl(PM_MCU_TIMER_BASE +HI_TIMER_CURRENTVALUE_OFFSET)!=(u32)0xffffffff)
    {}
    writel(ulvalue, PM_MCU_TIMER_BASE + HI_TIMER_LOADCOUNT_OFFSET);
    writel(0x1, PM_MCU_TIMER_BASE + HI_TIMER_CONTROLREG_OFFSET);
    do{
		ret = readl(PM_MCU_TIMER_BASE + HI_TIMER_CONTROLREG_OFFSET);
	}while((ret&0x1)!=0x1);
    timer_control.softtimer_start_value = ulvalue;
}

static void stop_hard_timer()
{
    u32 ret = 0;
    writel(0x0, PM_MCU_TIMER_BASE + HI_TIMER_CONTROLREG_OFFSET);
    do{
		ret = readl(PM_MCU_TIMER_BASE + HI_TIMER_CONTROLREG_OFFSET);
	}while(ret&0x1);
    timer_control.softtimer_start_value = ELAPESD_TIME_INVAILD;
}
#endif
static u32 hard_timer_elapsed_time()
{
    u32 ulTempValue = 0;
    if ( ELAPESD_TIME_INVAILD == timer_control.softtimer_start_value)
    {
        return 0;
    }
    ulTempValue = timer_get_value();
    return ((timer_control.softtimer_start_value < ulTempValue) ? 0 : (timer_control.softtimer_start_value - ulTempValue+1));
}

void bsp_softtimer_add(struct softtimer_list * timer)
{
    u32 elapsed_time = 0;
    struct softtimer_list *p = NULL;
    unsigned long flags = 0;
    if (NULL == timer)
    {
        softtimer_print("timer to be added is NULL\n");
        return;
    }
    local_irq_save(flags);
    /*����Ѿ��ڳ�ʱ�����У��򲻲���*/
    if(!list_empty(&timer->entry))
    {
    	 local_irq_restore(flags);
    	 return;
    }
    timer->timeout = timer->count_num;
    elapsed_time = hard_timer_elapsed_time();
    timer->timeout += elapsed_time;
    /*���timer����Ϊ�գ������ͷ���*/
    if (list_empty(&(timer_control.timer_list_head)))
    {
        list_add_tail(&(timer->entry),&(timer_control.timer_list_head));
    }
    /*�������Ϊ�գ����ճ�ʱʱ���С�������*/
    else
    {
        p=list_first_entry(&(timer_control.timer_list_head),struct softtimer_list,entry);
        if((TIMER_TRUE==p->is_running)&&(0==elapsed_time))
        {
            timer->timeout += p->timeout;
        }
	    if(elapsed_time!=0)
    	{
    	     timer->timeout -= 1;
	         elapsed_time     -= 1;
    	}
        list_for_each_entry(p,&(timer_control.timer_list_head),entry)
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
	    if(&(p->entry)!=&(timer_control.timer_list_head))
	    {
	        p->timeout -= timer->timeout;
	    }
	    list_add_tail(&(timer->entry),&(p->entry));
    }
    if (timer_control.timer_list_head.next == &(timer->entry))
    {
        timer->timeout -= elapsed_time;
	 if ((timer->entry.next)!=(&(timer_control.timer_list_head)))
	 {
	     p = list_entry(timer->entry.next,struct softtimer_list,entry);
	     if(TIMER_TRUE==p->is_running)
	     {
	         p->is_running = TIMER_FALSE;
	     }
	 }
	 timer->is_running = TIMER_TRUE;
	 start_hard_timer(timer->timeout);
    }
    local_irq_restore(flags);
}
s32 bsp_softtimer_delete(struct softtimer_list * timer)
{
    struct softtimer_list * p=NULL;
    unsigned long flags;
    if (NULL == timer)
    {
        softtimer_print("NULL pointer \n");
        return ERROR;
    }
    local_irq_save(flags);
    if (list_empty(&timer->entry))
    {
        local_irq_restore(flags);
   	 return NOT_ACTIVE;
    }
    else
    {
        if(timer->entry.next == &(timer_control.timer_list_head))/*���ɾ���������һ���ڵ㣬ֱ��ɾ��*/
        {
            timer->is_running = TIMER_FALSE;
            list_del_init(&(timer->entry));
        }
	 /*���ɾ�����������е�һ�����,����������������*/
        else if((timer->entry.prev == &(timer_control.timer_list_head))
			&&(timer->entry.next != &(timer_control.timer_list_head)))
        {
            timer->is_running = TIMER_FALSE;
            list_del_init(&(timer->entry));
			p=list_first_entry(&(timer_control.timer_list_head),struct softtimer_list,entry);/**/
			p->timeout += timer->timeout -hard_timer_elapsed_time();
			start_hard_timer(p->timeout);
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
    if (list_empty(&(timer_control.timer_list_head)))/*���ɾ�����º�����Ϊ�գ���ֹͣ����*/
    {
        stop_hard_timer();
    }
    local_irq_restore(flags);
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
    if((new_expire_time) < (SOFTTIMER_MAX_LENGTH)) 
    {
        timer->timeout= (HI_TCXO_CLK* new_expire_time)/1000;
    }
    else /* ��ֹ�˷���� */ 
    {
        timer->timeout= HI_TCXO_CLK * (new_expire_time/1000);
    }
    timer->count_num = timer->timeout;
    return OK;
}

s32 bsp_softtimer_create(struct softtimer_list *sft_info)
{
    INIT_LIST_HEAD(&(sft_info->entry));
    sft_info->is_running = TIMER_FALSE;
    sft_info->init_flags = TIMER_INIT_FLAG;
    	/*����������ֵ���򷵻�ʧ��,��λΪms*/
	if(sft_info->timeout>SOFTTIMER_MAX_LENGTH*1000)
	{
		softtimer_print("time too long ,not support\n");
		return ERROR;
    	}
	    if((sft_info->timeout) < SOFTTIMER_MAX_LENGTH)  /* ��С��SOFTTIMER_MAX_LENGTH�ļ�ʱ�����Ա��־��� */
	    {
		  sft_info->timeout= (HI_TCXO_CLK*(sft_info->timeout))/1000;
	    }
	    else /* ��ֹ�˷���� */ 
	    {
		 sft_info->timeout= HI_TCXO_CLK* ((sft_info->timeout)/1000);
	    }	
	    sft_info->count_num = sft_info->timeout;
     return OK;
}
s32 bsp_softtimer_free(struct softtimer_list *p)
{
    if ((NULL == p)||(!list_empty(&p->entry)))
	{
	    return ERROR;
    }
	p->init_flags = 0;
	 return OK;   
}
static void thread_softtimer_fun(void const *obj)
{
	struct softtimer_list     *p = NULL;
	unsigned long flags;
	softtimer_func func =NULL;
	u32 para = 0;
	/* coverity[no_escape] */
	/*lint --e{569 } */
	for( ; ; )
	{
	        /* coverity[sleep] */
		osSemaphoreWait(timer_control.soft_timer_sem, osWaitForever);
		 /* coverity[lock_acquire] */
		local_irq_save(flags);
		timer_control.softtimer_start_value = ELAPESD_TIME_INVAILD;
		if (!list_empty(&(timer_control.timer_list_head)))
		{
			p = list_first_entry(&(timer_control.timer_list_head),struct softtimer_list,entry);
			if(p->is_running == TIMER_TRUE)
			{
				list_del_init(&p->entry);
				p->is_running = TIMER_FALSE;
				func = p->func;
				para = p->para;
				local_irq_restore(flags); 
				func(para);			
				local_irq_save(flags);
				while(!list_empty(&(timer_control.timer_list_head)))
				{
					p=list_first_entry(&(timer_control.timer_list_head),struct softtimer_list,entry);
					if(0==p->timeout)
					{
						list_del_init(&p->entry);
						p->is_running = TIMER_FALSE;
						func = p->func;
						para = p->para;
						local_irq_restore(flags); 
						func(para);
						local_irq_save(flags);
					}
					else
						break;
				}
				if (!list_empty(&(timer_control.timer_list_head)))/*�������δ��ʱ��ʱ��*/
				{
					p=list_first_entry(&(timer_control.timer_list_head),struct softtimer_list,entry);
					p->is_running = TIMER_TRUE;
					start_hard_timer(p->timeout);
				}
				else 
				{  
					stop_hard_timer();
				}	
			}
			else  if (p->is_running == TIMER_FALSE)
			{
				p->is_running = TIMER_TRUE;
				start_hard_timer(p->timeout);
			}
		}
		else
		{
			stop_hard_timer();
		}
		local_irq_restore(flags); 
	} 
}
#ifdef K3V3_LPM3_HAS_MODEM_FEATURE
void IntTimerHandler(void)
{
	 u32 readValue = 0;    
    /*1����ȡӲ����ʱ�����ж�״̬
	 2��������жϣ������жϣ�ͬʱ�ͷ��ź���
   */
    readValue = readl(PM_MCU_TIMER_BASE+0x34);
    if (0 != readValue)
    {
        writel(0x1,PM_MCU_TIMER_BASE+0x2c);
        osSemaphoreRelease(timer_control.soft_timer_sem);
    }
}
#else
irqreturn_t IntTimerHandler(void)
{
	 u32 readValue = 0;    
    /*1����ȡӲ����ʱ�����ж�״̬
	 2��������жϣ������жϣ�ͬʱ�ͷ��ź���
   */
    readValue = readl(PM_MCU_TIMER_BASE+HI_TIMER_INTSTATUS_OFFSET);
    if (0 != readValue)
    {
        readValue = readl(PM_MCU_TIMER_BASE+HI_TIMER_EOI_OFFSET);
        osSemaphoreRelease(timer_control.soft_timer_sem);
    }
    return IRQ_HANDLED;
}
#endif
int  bsp_softtimer_init(void)
{
    INIT_LIST_HEAD(&(timer_control.timer_list_head));
    timer_control.clk                    = HI_TCXO_CLK;
    timer_control.softtimer_start_value  = ELAPESD_TIME_INVAILD;
	timer_control.soft_timer_sem=osSemaphoreCreate(osSemaphore(soft_timer_sem),SEM_FULL);
	if(NULL == timer_control.soft_timer_sem)
	{
		softtimer_print("osSemaphoreCreate create error\n");
		return -1;
	}
	/*lint --e{569 } */
	osSemaphoreWait(timer_control.soft_timer_sem, osWaitForever);
	 timer_control.thread_softtimer_id = osThreadCreate(osThread(thread_softtimer_fun), (void*)0);
	if (timer_control.thread_softtimer_id == NULL)
	{
		softtimer_print("thread_softtimer_id create error\n");
		return -1;
	}
	stop_hard_timer();
	#ifdef K3V3_LPM3_HAS_MODEM_FEATURE	
	(void)request_irq(PM_MCU_TIMER_INT_LVL, IntTimerHandler, 0, 0, 0);
	#else
    (void)request_irq(PM_MCU_TIMER_INT_LVL, (irq_handler_t)IntTimerHandler, 0, "timer10 irq", 0);
	#endif
	enable_irq(PM_MCU_TIMER_INT_LVL);
     softtimer_print("softtimer init success\n");
    return 0;
}
#ifdef __cplusplus
}
#endif


