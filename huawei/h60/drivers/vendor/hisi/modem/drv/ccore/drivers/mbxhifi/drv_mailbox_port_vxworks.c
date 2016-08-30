

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include <vxWorks.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <logLib.h>
#include <semLib.h>
#include <intLib.h>
#include <memLib.h>
#include <sysLib.h>
#include <taskLib.h>

#include "drv_mailbox.h"
#include "drv_mailbox_cfg.h"
#include "drv_mailbox_debug.h"
#include "drv_mailbox_gut.h"
#include "osl_sem.h"
#include "drv_ipc.h"
#include "drv_timer.h"
#include "osl_bio.h"   /*lint !e537*/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  1 ��ά�ɲ���Ϣ�а�����C�ļ���ź궨��
*****************************************************************************/
#undef  _MAILBOX_FILE_
#define _MAILBOX_FILE_   "vxworks"

#define MAILBOX_MILLISEC_PER_SECOND                         1000

#define  MAILBOX_VXWORKS_SEND_FULL_DELAY_MS                 10          /*�������ӳٺ���*/

#ifdef _DRV_LLT_
#define  MAILBOX_VXWORKS_SEND_FULL_DELAY_TIMES              500         /*�������ӳٳ��Դ���*/
#else
#define  MAILBOX_VXWORKS_SEND_FULL_DELAY_TIMES              0          /*�������ӳٳ��Դ���*/
#endif

#ifndef WAIT_FOREVER
#define WAIT_FOREVER	(-1)
#endif

/*****************************************************************************
  ����C��vxworks����ϵͳ���������ݵĴ���ӿڵ��÷�ʽ
*****************************************************************************/
enum MAILBOX_VXWORKS_PROC_STYLE_E
{
    MAILBOX_VXWORKS_PROC_STYLE_SEND_RESERVED   = 0,  /*0��ʾ����ͨ��*/

    /*���մ���ʽ���忪ʼ*/
    MAILBOX_VXWORKS_PROC_STYLE_RECEV_START,

    /*�ҽ��������ϵ��ʼ�������п�ʼ*/
    MAILBOX_VXWORKS_PROC_STYLE_TASK_RESERVED,

    MAILBOX_VXWORKS_PROC_STYLE_TASK_MSG,
    MAILBOX_VXWORKS_PROC_STYLE_TASK_IFC ,

    /*�ҽ��������ϵ��ʼ�������н���*/
    MAILBOX_VXWORKS_PROC_STYLE_TASK_BUTT,

    /*���ж��д�����ʼ��������*/
    MAILBOX_VXWORKS_PROC_STYLE_INT_NORMAL,


    /*���մ���ʽ�������*/
    MAILBOX_VXWORKS_PROC_STYLE_RECEV_END

};

/*****************************************************************************
    ����C�����䵥������ҽӵ����乤�����м�������
*****************************************************************************/
struct mb_vx_work
{
    unsigned long                               channel_id;      /*����ID�ţ������Ǻ˼�����ID,Ҳ����������ͨ��ID*/
    unsigned long                               data_flag;       /*�������Ƿ������ݵı�־λ*/
    long								(*cb)(unsigned long channel_id);
    struct mb_vx_work                           *next;          /*ָ����һ��*/
#ifdef MAILBOX_OPEN_MNTN
    struct mb_buff                              *mb_priv;
#endif

};

/*****************************************************************************
  ����C������������ص�����
*****************************************************************************/
struct mb_vx_proc
{
    signed char                                 name[16];   /*����ʽ��*/
    unsigned long                               id;         /*����ʽID��*/
    signed long                                 priority;   /*����ʽ���ȼ�*/
    signed long                                 stack;      /*����ջ��С*/
    osl_sem_id                                  sema;       /*����ȴ�����Ϣ*/
    struct mb_vx_work                          *vxqueue;    /*������ҽӵ����乤������*/
};
/*****************************************************************************
  ������������ͨ�������񣬺˼��жϼ����ջص�����֮��Ķ�Ӧ��ϵ
*****************************************************************************/
struct mb_vx_cfg
{
    unsigned long                               channel_id;      /*����ͨ��ID��*/
    unsigned long                               proc_style;      /*�������ͨ�������崦�����乤�����е�����ID��*/
    unsigned long                               Int_src;         /*����ͨ����ʹ�õĺ˼��ж���Դ�š�*/
    unsigned long                               dst_cpu;          /*����ͨ����ʹ�õĺ˼��ж�Ŀ��CPU��*/
};

//extern int read_efuse(unsigned int group, unsigned int *buf, unsigned int len);
void mailbox_usr_init(void);

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/*�������������б�*/
MAILBOX_LOCAL struct mb_vx_proc g_mailbox_vx_proc_tbl[] =
{
    /*����ʽ��*/    /*����ʽID��*/                         /*���ȼ�*/   /*ջ��С*/

    /*����ʽ���ʼ����ݴ���*/
    {"mailboxMsg"   ,MAILBOX_VXWORKS_PROC_STYLE_TASK_MSG,     4,          4096        },
    {"mailboxIfc"   ,MAILBOX_VXWORKS_PROC_STYLE_TASK_IFC,     5,          4096        },

    /*�жϷ�ʽ���ʼ����ݴ���*/
    {"mailboxInt"   ,MAILBOX_VXWORKS_PROC_STYLE_INT_NORMAL,     0,          0         },

    /*���ڴ˺���������ʽ��Ŀ�������Ӱ��UT����*/

    /*����*/
};

/*C�˵�����ͨ����Դ��ƽ̨ϵͳ��Դ��Ӧ��ϵ���ñ�*/
MAILBOX_LOCAL struct mb_vx_cfg g_mailbox_vx_cfg_tbl[] =
{
#if 0
    /*����ͨ��������*/
    /*ChannelID*/                                       /*ͨ������ʽID��*/
    {MAILBOX_MAILCODE_RESERVED(MCU,  CCPU, MSG),    MAILBOX_VXWORKS_PROC_STYLE_TASK_MSG      },
    {MAILBOX_MAILCODE_RESERVED(HIFI, CCPU, MSG),    MAILBOX_VXWORKS_PROC_STYLE_TASK_MSG      },
    {MAILBOX_MAILCODE_RESERVED(ACPU, CCPU, MSG),    MAILBOX_VXWORKS_PROC_STYLE_TASK_MSG      },
    {MAILBOX_MAILCODE_RESERVED(ACPU, CCPU, IFC),    MAILBOX_VXWORKS_PROC_STYLE_TASK_IFC      },
    {MAILBOX_MAILCODE_RESERVED(MCU,  CCPU, IFC),    MAILBOX_VXWORKS_PROC_STYLE_TASK_IFC      },
    /*����ͨ������*/
    /*ChannelID*/                                       /*ͨ������ʽID��*/
    {MAILBOX_MAILCODE_RESERVED( CCPU, MCU,  MSG),   MAILBOX_VXWORKS_PROC_STYLE_SEND_RESERVED    },
    {MAILBOX_MAILCODE_RESERVED( CCPU, HIFI, MSG),   MAILBOX_VXWORKS_PROC_STYLE_SEND_RESERVED    },
    {MAILBOX_MAILCODE_RESERVED( CCPU, ACPU, MSG),   MAILBOX_VXWORKS_PROC_STYLE_SEND_RESERVED    },
    {MAILBOX_MAILCODE_RESERVED( CCPU, ACPU, IFC),   MAILBOX_VXWORKS_PROC_STYLE_SEND_RESERVED    },
    {MAILBOX_MAILCODE_RESERVED( CCPU, MCU, IFC),    MAILBOX_VXWORKS_PROC_STYLE_SEND_RESERVED    },
#else
    /*����ͨ��������*/
    /*ChannelID*/                                       /*ͨ������ʽID��*/
    {MAILBOX_MAILCODE_RESERVED(HIFI, CCPU, MSG),    MAILBOX_VXWORKS_PROC_STYLE_TASK_MSG      },
    /*����ͨ������*/
    /*ChannelID*/                                       /*ͨ������ʽID��*/
    {MAILBOX_MAILCODE_RESERVED( CCPU, HIFI, MSG),   MAILBOX_VXWORKS_PROC_STYLE_SEND_RESERVED    },
#endif
    /*������־*/
    {MAILBOX_MAILCODE_INVALID, 0,0}
};

MAILBOX_LOCAL void *mailbox_mutex_create(void);
/*****************************************************************************
  3 ��������
*****************************************************************************/

MAILBOX_LOCAL void mailbox_receive_task(
                osl_sem_id         *sema,
                struct mb_vx_work  **work_list )
{
    struct mb_vx_work    *work   =   MAILBOX_NULL;

    /* coverity[no_escape] */
    while (1) { /*lint !e716*/
        if (MAILBOX_OK == osl_sem_downtimeout(sema, WAIT_FOREVER)) {
            work = *work_list;
            while (MAILBOX_NULL != work) {
                /*������־λ���������λ�����ö�Ӧ������ID�ŵĻص�����*/
                if (MAILBOX_TRUE == work->data_flag) {
                    work->data_flag = MAILBOX_FALSE;
 #ifdef MAILBOX_OPEN_MNTN
                mailbox_record_sche_recv(work->mb_priv);
 #endif
                    if (work->cb) {
                        if (MAILBOX_OK !=  work->cb(work->channel_id)) {
                             (void)mailbox_logerro_p1(MAILBOX_ERR_VXWORKS_CALLBACK_ERRO, work->channel_id);
                        }
                    } else {
                        (void)mailbox_logerro_p1(MAILBOX_ERR_VXWORKS_CALLBACK_NOT_FIND, work->channel_id);
                        #ifdef _DRV_LLT_
                        /*PC UT ������֤�ܹ�����*/
                        return MAILBOX_ERR_VXWORKS_CALLBACK_NOT_FIND;
                        #endif
                    }
                }
                work = work->next;
            }
        }
    }
}

/*������˵Ķ����ṩ�ӿ�*/

/*lint -save -e64*/
MAILBOX_EXTERN long mailbox_init_platform(void)
{
    struct mb_vx_proc   *process    = &g_mailbox_vx_proc_tbl[0];
    unsigned long        count      =  sizeof(g_mailbox_vx_proc_tbl) /
                                       sizeof(struct mb_vx_proc);
    unsigned long        proc_id;

    /*����ƽ̨�����ж��ź�������*/
    while (count) {
        /*Ϊ������ʽ������ͨ����������*/
        proc_id = process->id;
        if ((proc_id > MAILBOX_VXWORKS_PROC_STYLE_TASK_RESERVED) && (proc_id < MAILBOX_VXWORKS_PROC_STYLE_TASK_BUTT)) {
            /* ���������������ȴ��ź���*/
            osl_sem_init(0, &process->sema);

            /* �������������ݴ�������*/
            if (ERROR == (int)taskSpawn((char *)process->name, (int)process->priority, (int)0,
                            (int)process->stack,
                            (void*)mailbox_receive_task,
                            (int)(&process->sema),
                            (int)(&process->vxqueue),
                            (int)0, (int)0, (int)0, (int)0, (int)0, (int)0, (int)0, (int)0)) /*lint !e119 !e64*/
            {
                return mailbox_logerro_p1(MAILBOX_ERR_VXWORKS_TASK_CREATE, proc_id);
            }
        }
        count--;
        process++;
    }

    mailbox_usr_init();

    return MAILBOX_OK;
}
/*lint -restore*/

MAILBOX_LOCAL long mailbox_ipc_process(
                struct mb_vx_work  *work,
                struct mb_vx_proc  *proc,
                unsigned long channel_id,
                unsigned long proc_id)
{
    unsigned long is_find = MAILBOX_FALSE;

    while (work) {
        /*�����乤���������ҵ���Ӧ�����䣬���ñ�־λ���ͷ��ź���֪ͨ��������*/
        if (channel_id  == work->channel_id) {
            if ((proc_id > MAILBOX_VXWORKS_PROC_STYLE_TASK_RESERVED) && (proc_id < MAILBOX_VXWORKS_PROC_STYLE_TASK_BUTT)) {
                is_find = MAILBOX_TRUE;

                /*�����������乤�����������д���������ݱ�־λ*/
                work->data_flag = MAILBOX_TRUE;
 #ifdef MAILBOX_OPEN_MNTN
                mailbox_record_sche_send(work->mb_priv);
 #endif
                /*�ͷ��ź�����֪ͨ����*/
                osl_sem_up(&proc->sema);
            } else if (MAILBOX_VXWORKS_PROC_STYLE_INT_NORMAL == proc_id) {
                 /*�жϴ���ʽ�����ж���ֱ�Ӵ�����������*/
                if (work->cb) {
                    is_find = MAILBOX_TRUE;

                    if (MAILBOX_OK !=  work->cb(work->channel_id)) {
                         (void)mailbox_logerro_p1(MAILBOX_ERR_VXWORKS_CALLBACK_ERRO, work->channel_id);
                    }
                } else {
                     (void)mailbox_logerro_p1(MAILBOX_ERR_VXWORKS_CALLBACK_NOT_FIND, work->channel_id);
                }
            } else {
                is_find = MAILBOX_FALSE;
            }

        }
        work = work->next;
    }

    return (long)is_find;
}

int g_hifimailbox = 0;


MAILBOX_LOCAL long mailbox_ipc_int_handle(unsigned long int_src)
{
    struct mb_vx_cfg        *cfg    =  &g_mailbox_vx_cfg_tbl[0];
    struct mb_vx_proc       *proc   =   MAILBOX_NULL;
    struct mb_vx_work       *work   =   MAILBOX_NULL;
    unsigned long count = sizeof(g_mailbox_vx_proc_tbl)/sizeof(struct mb_vx_proc);
    unsigned long proc_id = 0;
    unsigned long channel_id = 0;
    unsigned long is_find = MAILBOX_FALSE;
    unsigned long ret_val = MAILBOX_OK;

    if(MAILBOX_INIT_MAGIC != g_mailbox_handle.init_flag)
    {
        return (long)ret_val;
    }

    g_hifimailbox++;

    /*�ҵ�����ID��Ӧ����������*/
    while (MAILBOX_MAILCODE_INVALID != cfg->channel_id) {
        /*�������йҽӵ�����жϺŵĽ�������ͨ��*/
        proc_id = cfg->proc_style;
        if ((int_src == cfg->Int_src) && (MAILBOX_VXWORKS_PROC_STYLE_SEND_RESERVED != cfg->proc_style)) {
            channel_id = cfg->channel_id;

            proc   =  &g_mailbox_vx_proc_tbl[0];
            count          =   sizeof(g_mailbox_vx_proc_tbl)/sizeof(struct mb_vx_proc);
            while (count) {
                /*�ҵ�������ͨ����Ӧ��������Ϣ*/
                if (proc_id == proc->id) {
                    work = proc->vxqueue;
                    is_find = (unsigned long)mailbox_ipc_process( work,
                                                 proc,
                                                 channel_id,
                                                 proc_id);
                    break;
                }
                count--;
                proc++;
            }

            if (0 == count) {
                 ret_val = (unsigned long)mailbox_logerro_p1(MAILBOX_ERR_VXWORKS_MAIL_TASK_NOT_FIND, channel_id);
            }
        }
        cfg++;
    }

    if (MAILBOX_TRUE != is_find) {
         ret_val = (unsigned long)mailbox_logerro_p1(MAILBOX_ERR_VXWORKS_MAIL_INT_NOT_FIND, channel_id);
    }

    return (long)ret_val;
}
MAILBOX_EXTERN long mailbox_process_register(
                unsigned long channel_id,
                 long (*cb)(unsigned long channel_id),
                 void *priv)
{
    struct mb_vx_work       *work    =   MAILBOX_NULL;
    struct mb_vx_cfg        *cfg  =  &g_mailbox_vx_cfg_tbl[0];
    struct mb_vx_proc       *proc =  &g_mailbox_vx_proc_tbl[0];
    struct mb_vx_cfg*        cfg_find     =   MAILBOX_NULL;
    unsigned long count = sizeof(g_mailbox_vx_proc_tbl)/sizeof(struct mb_vx_proc);

    while (MAILBOX_MAILCODE_INVALID != cfg->channel_id) {
        /*�ҵ��봫������ID�������ϵͳ��������*/
        if (cfg->channel_id ==  channel_id) {
            cfg_find = cfg;
            break;
        }
        cfg++;
    }

    if (cfg_find) {
        /*�������������Ӧ�����乤������*/
        while (count) {
            if (cfg_find->proc_style == proc->id) {
                if (proc->vxqueue) {
                    work = proc->vxqueue;
                    while (MAILBOX_NULL != work->next) {
                        work = work->next;
                    }
                    work->next            = (struct mb_vx_work *)calloc(
                                                    sizeof(struct mb_vx_work), 1);
                    if (MAILBOX_NULL == work->next) {
                        return mailbox_logerro_p0(MAILBOX_ERR_VXWORKS_ALLOC_MEMORY);

                    }
                    work->next->channel_id = cfg_find->channel_id;
                    work->next->cb  = cb;
                    work->next->mb_priv = priv;
                } else {
                    proc->vxqueue     = (struct mb_vx_work *)calloc(
                                                   sizeof(struct mb_vx_work), 1);
                    if (MAILBOX_NULL == proc->vxqueue) {
                        return mailbox_logerro_p0(MAILBOX_ERR_VXWORKS_ALLOC_MEMORY);
                    }
                    proc->vxqueue->channel_id = cfg_find->channel_id;
                    proc->vxqueue->cb  = cb;
                    proc->vxqueue->mb_priv = priv;
                }
            }
            count--;
            proc++;
        }

        return MAILBOX_OK;
    }

    return  mailbox_logerro_p1(MAILBOX_ERR_VXWORKS_CHANNEL_NOT_FIND, channel_id);

}

extern void tm_mailbox_msg_reg(unsigned long channel_id);


MAILBOX_EXTERN long mailbox_channel_register(
                unsigned long channel_id,
                unsigned long int_src,
                unsigned long dst_id,
                unsigned long direct,
                void   **mutex)
{
    struct mb_vx_cfg    *cfg  =  &g_mailbox_vx_cfg_tbl[0];

    while (MAILBOX_MAILCODE_INVALID != cfg->channel_id) {
        if (channel_id == cfg->channel_id) {
            *mutex = mailbox_mutex_create();

            /*ͨ����Դ�������ҵ���ע��IPC�ж�*/
            cfg->Int_src = int_src;
            cfg->dst_cpu  = dst_id;
            if(MIALBOX_DIRECTION_RECEIVE == direct) {
                (void)BSP_IPC_IntConnect((IPC_INT_LEV_E)int_src , (VOIDFUNCPTR)mailbox_ipc_int_handle, int_src);
                (void)BSP_IPC_IntEnable ((IPC_INT_LEV_E)int_src);

                /*���ST����ͨ��ע��*/
                tm_mailbox_msg_reg(channel_id);

                /*TODO: ������������µ��������ԣ�������Ҫ��������һ��ͨ�����ص�:  */
                /*mailbox_ipc_int_handle(int_src);*/
            }

            return MAILBOX_OK;
        }
        cfg++;
    }

    /*���������Ҳ�����Ӧ�����ã�����*/
    return mailbox_logerro_p1(MAILBOX_ERR_VXWORKS_CHANNEL_NOT_FIND, channel_id);

}
MAILBOX_EXTERN long mailbox_delivery(unsigned long channel_id)
{
    struct mb_vx_cfg     *cfg       =  &g_mailbox_vx_cfg_tbl[0];
    struct mb_vx_cfg     *cfg_find  =   MAILBOX_NULL;

    while (MAILBOX_MAILCODE_INVALID != cfg->channel_id) {
        /*�ҵ��봫������ID�������ϵͳ��������*/
        if (cfg->channel_id == channel_id) {
            cfg_find = cfg;
            break;
        }
        cfg++;
    }

    cache_sync();

    if (MAILBOX_NULL != cfg_find) {
        return (long)BSP_IPC_IntSend((IPC_INT_CORE_E)cfg_find->dst_cpu, (IPC_INT_LEV_E)cfg_find->Int_src);
    }

    return mailbox_logerro_p1(MAILBOX_ERR_VXWORKS_CHANNEL_NOT_FIND, channel_id);
}
MAILBOX_LOCAL void *mailbox_mutex_create(void)
{
    return  (void *)semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
}


MAILBOX_EXTERN long mailbox_mutex_lock(void **mutexId)
{

    return semTake(*mutexId, WAIT_FOREVER);
}



MAILBOX_EXTERN void mailbox_mutex_unlock(void **mutex_id)
{
    (void)semGive((SEM_ID)*mutex_id);
}
/*lint -save -e64*/
MAILBOX_EXTERN void *mailbox_init_completion(void)
{
    return  (void *)semBCreate(SEM_Q_PRIORITY, 0);
}
/*lint -restore*/

MAILBOX_EXTERN long  mailbox_wait_completion(void **sema_id, unsigned long timeout)
{
    long ret =  (long)semTake(*sema_id, (int)(timeout/10)); /*vxworks's tick is 10 millisecond*/
    return ret;
}

MAILBOX_EXTERN void mailbox_complete(void **sema_id)
{
    (void)semGive((SEM_ID)*sema_id);
}

MAILBOX_EXTERN void mailbox_del_completion(void **wait)
{
    (void)semDelete(*wait);
}
MAILBOX_EXTERN void *mailbox_memcpy(void *dst, const void *src, long size)
{
    return (void *)memcpy(dst, src, (unsigned int)size);  /*lint !e516*/
}
MAILBOX_EXTERN void *mailbox_memset(void * m, long c, unsigned long size)
{
    return memset(m, c, (u32)size);  /*lint !e516*/
}
/*lint -save -e715*/
MAILBOX_EXTERN void mailbox_assert(unsigned long ErroNo)
{
#ifndef _DRV_LLT_
    unsigned int ticks = 0;

    ticks = (unsigned int)sysClkRateGet();
    ticks++;

    /* coverity[no_escape] */
    while (1) /*lint !e716*/
    {
        printf("mb Assert!!");

        (void)taskDelay((int)ticks);

    }
#endif
}
/*lint -restore*/


/*lint -save -e18 -e64*/
MAILBOX_EXTERN long mailbox_int_context(void)
{
    return  INT_CONTEXT();
}
/*lint -restore*/


/*lint -save -e685 -e568*/
MAILBOX_EXTERN long mailbox_scene_delay(unsigned long scene_id, unsigned long *try_times)
{
    unsigned long go_on = MAILBOX_FALSE;
    unsigned long delay_ms = 0;
    unsigned int ticks = 0;
    
    switch (scene_id) {
        case MAILBOX_DELAY_SCENE_MSG_FULL: 
        case MAILBOX_DELAY_SCENE_IFC_FULL:
            delay_ms = MAILBOX_VXWORKS_SEND_FULL_DELAY_MS; 
            /* coverity[unsigned_compare] */
            go_on = (*try_times >= MAILBOX_VXWORKS_SEND_FULL_DELAY_TIMES) ? 
                     MAILBOX_FALSE : MAILBOX_TRUE;
            break;
        default:
            break;
    }

    if (MAILBOX_TRUE == go_on) {
        ticks = (delay_ms * sysClkRateGet()) / MAILBOX_MILLISEC_PER_SECOND; /*lint !e737*/
        ticks++;

    #ifndef _DRV_LLT_ /*taskDelay()Ӱ��UT�����ʺͲ���Ч��*/
        (void)taskDelay((int)ticks);
    #endif
    }

    *try_times = *try_times + 1;
    return (long)go_on;
}
/*lint -restore*/

MAILBOX_EXTERN long mailbox_get_timestamp(void)
{

#ifndef _DRV_LLT_
    return (long)BSP_GetSliceValue();
#else
    return 0;
#endif
}

#if 0
#ifndef _DRV_LLT_
IFC_GEN_EXEC3(read_efuse,
			  IFC_INCNT, unsigned int, group, 0,
			  IFC_OUTVAR, unsigned int*, buf, 0,
			  IFC_INCNT, unsigned int, len, 0)

#else
extern int ife_read_efuse(unsigned int group, unsigned int *buf, unsigned int len);
#endif
#endif

void mailbox_usr_init(void)
{
//    MAILBOX_IFC_REG(MAILBOX_IFC_ACPU_TO_CCPU_READ_EFUSE, read_efuse);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

