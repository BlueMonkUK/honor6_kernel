
/*lint -save -e537 -e451*/
#include <vxWorks.h>
#include <vxWorksCommon.h>
#include <types/vxTypesOld.h>
#include <stdlib.h>
#include <logLib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <errnoLib.h>
#include <memLib.h>
#include <string.h>
#include <taskLib.h>
#include <intLib.h>
#include <vxWind.h>
#include <vxCpuLib.h>
#include <dbgLib.h>
#include <trcLib.h>
#include <sysSymTbl.h>
#include <taskHookLib.h>
#include <private/cplusLibP.h>
#include <private/dbgLibP.h>
#include <private/vxdbgLibP.h>
#include <private/vxdbgBpMsgQLibP.h>
#include <private/excLibP.h>
#include <private/rtpLibP.h>
#include <private/taskLibP.h>
#include <private/trcLibP.h>
#include <private/funcBindP.h>
#include <private/shellInternalLibP.h>
#include <private/eventLibP.h>
#include <symLib.h>
#include <hashLib.h>
#include <private/hashLibP.h>
#include <a_out.h>
#include <vmLib.h>
#include <arch/arm/esfArm.h>
#include <arch/arm/archArm.h>
#include <arch/arm/excArmLib.h>
#include <arch/arm/regsArm.h>
#include <arch/arm/archArmCommon.h>
#include "cacheLib.h"
#include "product_config.h"
#include "osl_types.h"
#include <osl_list.h>
#include "hi_dsp.h"
#include "drv_comm.h"
#include "bsp_om_api.h"
#include "bsp_ipc.h"
#include "bsp_memmap.h"
#include "bsp_wdt.h"
#include "bsp_nvim.h"
#include "bsp_utrace.h"
#include "drv_onoff.h"
#include "bsp_rfile.h"
#include "bsp_nvim.h"
#include <bsp_uart.h>
#include "bsp_dsp.h"
#include "bsp_dump.h"
#include "bsp_dump_def.h"
#include "bsp_dump_drv.h"
/*lint -restore*/
/*lint --e{18 } */
#define DUMP_TASK_NUM_MAX 128

typedef struct _dump_field_hook_t
{
    struct list_head hook_list;
    s32              field_id;
    dump_save_hook   pfunc;
}dump_field_hook_t;

static LIST_HEAD(g_dump_hook_list);

dump_top_head_t      *g_dump_top_head = BSP_NULL;
dump_core_internal_t *g_dump_core_internal = BSP_NULL;
dump_global_base_info_t *g_dump_global_base_info = BSP_NULL;
dump_base_info_t     *g_dump_base_info = BSP_NULL;
dump_task_info_t     *g_dump_task_info = BSP_NULL;
dump_regs_map_t      *g_dump_reg_map = BSP_NULL;

dump_ctrl_s           g_dump_ctrl;
dump_nv_s             g_dump_cfg = {{0}};
dump_save_hook        g_dump_trace_read_cb[DUMP_SYSVIEW_BUTT] = {BSP_NULL};
dump_debug_info_t g_dump_ptr_info = {0};
func_void g_intlock_in_hook = NULL;
func_void g_intlock_out_hook = NULL;
bool g_exc_flag = false;
/* ARM�쳣�����־ */
bool g_arm_exc_save_flag = false;

extern s32  intCnt;
extern char * vxIntStackBase;
extern char * vxIntStackEnd;
/*lint -save -e762*/
extern s32  taskSwitchHookAdd (FUNCPTR switchHook);
/*lint -restore*/
extern void vicIntLvlChgHookAdd(FUNCPTR p_Func);
extern void exc_hook_add(FUNC_VOID pHookFunc);
extern void dump_stack_add_hook(FUNC_VOID *fn);
extern void vicIntLvlChgHookDel(void);
extern void regcpy(void* dst, void* src, int size);
void QueueInit(dump_queue_t *Q, u32 elementNum);
s32  QueueIn(dump_queue_t *Q, u32 element);
s32  QueueOut(dump_queue_t *Q, u32 *element);
s32  QueueReadLast(dump_queue_t *Q, u32 *element);
s32  QueueLoopIn(dump_queue_t *Q, u32 element);
int  QueueNumGet(dump_queue_t *Q);
s32  dump_vsnprintf(char *s, s32 n, const char *fmt, ...);
s32  dump_print_buffer(UserTraceCtx* traceCtx, char* fmt, ...);
s32  dump_task_regs(s32 tid, REG_SET *pRegs);
s32  dump_trc_print(u32 *callAdrs, u32 *funcAdrs, s32 nArgs, s32 *args, s32	taskId, bool isKernelAdrs, void *usrCtx);
s32  dump_stack_trace(s32 taskId, UserTraceCtx* traceCtx);
void dump_task_show_stack(s32 tid, void* buffer, s32 bufferlen);
int  dump_sym_find_symbol(SYMTAB_ID symTblId, char * name, void * value, SYM_TYPE type, SYM_TYPE mask, SYMBOL_ID * pSymbolId);
s32  dump_int_trc_print(INSTR *callAdrs, INSTR *funcAdrs, s32 nArgs, s32 *args, s32	taskId, bool isKernelAdrs, void *usrCtx);
void dump_int_stack_trace(REG_SET * pRegSet);
int  dump_int_exc_hook(int vec, ESF * pEsf, REG_SET * pRegSet, EXC_INFO * pExcInfo);
void dump_trc_arm_exc_init(void);
s32  dump_in_interrrupt(void);
void dump_task_switch_hook(void *old_tcb, void *new_tcb);
void dump_int_switch_hook(u32 dir, u32 newVec);
void dump_exc_hook(void * currentTaskId, s32 vec, void * pReg);
void dump_stack_hook(s32 where);
bool dump_check_reboot(void);
void dump_register_hook(void);
void dump_save_global_info(void);
void dump_save_global_base_info(void);
void dump_save_base_info(u32 mod_id, u32 arg1, u32 arg2, char *data, u32 length);
void dump_save_log(void);
void dump_save_exc_task(u32 addr);
void dump_save_regs(void);
void dump_save_usr_data(char *data, u32 length);
void dump_save_ext(void);
void dump_wait_for_reboot(void);
void dump_wait_and_reboot(void);
void dump_save_all_task_tcb(s32 tid);
void dump_save_all_task_job(s32 tid, s32 agent_dump);
void dump_save_all_task(bool agent_dump);
void dump_trace_stop(void);
void dump_system_error_enter(void);
void dump_notify_target(void);
void dump_int_handle(s32 param);
s32  dump_config_init(void);
void dump_map_init(void);
void dump_global_info_init(void);
void dump_global_base_info_init(void);
void dump_base_info_init(void);
void dump_queue_t_init(void);
void dump_wait_save(void);
s32  dump_init_phase1(void);
s32  dump_task_init(void);
void dump_init_done(void);
s32  dump_init(void);
s32  dump_show_map(void);
int QueueFrontDel(dump_queue_t *Q, u32 del_num);
int dump_trc_pc_validate(u32 * pc, TRC_OS_CTX * pOsCtx);
int dump_trc_reg_count (int offset);
int dump_trc_func_info_get(TRC_OS_CTX * pCtx, u32 * pc, char * sp, u32 * lr, char ** pFp, u32 ** pReturnPc, u32 ** pFuncAddr);
int dump_trc_lvl_info_get(char * fp, u32 * pc, TRC_OS_CTX * pCtx, char ** pPrevFp, u32 ** pPrevPc, u32 **	pFuncAddr);
int dump_trc_stack_lvl_user(char * fp, u32 * pc, u32 * funcAddr, TRC_CTX * pCtx);
int dump_trc_stack_lvl(char * fp,	u32 *	pc,	TRC_CTX * pCtx);
int dump_trc_top_info_get(TRC_OS_CTX * pCtx, char ** pFp, char ** pPrevFp, u32 ** pReturnPc, u32 ** pFuncAddr);
int dump_trc_stack_ctx_trace(TRC_CTX * pCtx);

u32 g_cp_field_num = 0;
dump_area_field_t* dump_get_field(u32 field_id);
u8* bsp_dump_register_field(u32 field_id, u32 length);

extern u32 BSP_MailBox_IsDspSleep();
void dump_save_dtcm(void);
extern int rotate_N_bits (int val, int rot);
int dump_trc_stack_trace(REG_SET * pTaskRegs, TRC_CTX * pCtx);

#define DUMP_T_TASK_ERROR(mod_id)      (mod_id & (1<<24))
#define DUMP_LINUX_TASK_NUM_MAX         128
#define DUMP_REG_SET_MAX                4096
#define DBG_DEMANGLE_PRINT_LEN 256  /* Num chars of demangled names to print */

#define	rtpFromTaskGet(taskId)	(taskTcb(taskId)->rtpId)

#define MAKE_FIXED_FIELD(p, id, addr, size) \
{ \
    ((dump_area_field_t*)p)->field_id    = id; \
    ((dump_area_field_t*)p)->length      = size; \
    ((dump_area_field_t*)p)->offset_addr = addr - DUMP_COMM_SECTION_ADDR; \
    ((dump_area_field_t*)p)->status      = DUMP_FIELD_USED; \
}

void QueueInit(dump_queue_t *Q, u32 elementNum)
{
    Q->maxNum = elementNum;
    Q->front = 0;
    Q->rear = 0;
    Q->num = 0;

    /*lint --e{419 } */
    memset((void *)Q->data, 0, (size_t)(elementNum*sizeof(u32)));
}

__inline__ s32 QueueIn(dump_queue_t *Q, u32 element)
{
    if (Q->num == Q->maxNum)
    {
        return -1;
    }

    Q->data[Q->rear] = element;
    Q->rear = (Q->rear+1) % Q->maxNum;
    Q->num++;

    return 0;
}

s32 QueueOut(dump_queue_t *Q, u32 *element)
{
    if (0 == Q->num)
    {
        return -1;
    }

    *element = Q->data[Q->front];
    Q->front = (Q->front+1) % Q->maxNum;
    Q->num--;

    return 0;
}

int QueueFrontDel(dump_queue_t *Q, u32 del_num)
{
    if (del_num > Q->num)
    {
        return -1;
    }
    Q->front = (Q->front+del_num) % Q->maxNum;
    Q->num   = Q->num - del_num;

    return 0;
}


s32 QueueReadLast(dump_queue_t *Q, u32 *element)
{
    if (0 == Q->num)
    {
        return -1;
    }

    *element = Q->data[((Q->rear+Q->maxNum)-1) % Q->maxNum];

    return 0;
}


__inline__ s32 QueueLoopIn(dump_queue_t *Q, u32 element)
{
    if (Q->num < Q->maxNum)
    {
        return QueueIn(Q, element);
    }
    else
    {
        Q->data[Q->rear] = element;
        Q->rear = (Q->rear+1) % Q->maxNum;
        Q->front = (Q->front+1) % Q->maxNum;
    }

    return 0;
}

s32 dump_vsnprintf(char *s, s32 n, const char *fmt, ...)
{
    va_list vaList = (va_list)NULL;
    s32 len =0;

    va_start(vaList, fmt);
    len = vsnprintf(s, (u32)n, fmt, vaList); /* [false alarm]:����Fortify���� */
    va_end(vaList);
    return len;
}

/*lint -save -e737 -e775*/
s32 dump_print_buffer(UserTraceCtx* traceCtx, char* fmt, ...)
{
    va_list vaList = (va_list)NULL;
    s32 len =0;

	if (traceCtx->bufferLen <= 0)
		return 0;

    va_start(vaList, fmt);
    len = vsnprintf(traceCtx->buffer, traceCtx->bufferLen, fmt, vaList); /* [false alarm]:����Fortify���� */
    va_end(vaList);
    traceCtx->buffer += len;
    traceCtx->bufferLen -= len;
    return len;
}
/*lint -restore*/
/*lint -save -e40 -e119 -e522 -e63 -e64 -e438 -e826 -e553 -e409 -e613 -e550 -e702 -e713 -e737 -e744 -e762 -e771 -e746 -e958 */
s32 dump_task_regs
    (
    s32         tid,    /* task ID */
    REG_SET *   pRegs   /* put register contents here */
    )
{
    FAST WIND_TCB * pTcb = taskTcb(tid);   /* ptr to task's TCB */

    if (pTcb == NULL)
        return (ERROR);

    if (pTcb->pExcRegSet != NULL)
        bcopy ((char *)pTcb->pExcRegSet, (char *) pRegs, sizeof (REG_SET)); /* [false alarm]:����Fortify���� */
    else
    	bcopy ((char *)&pTcb->regs, (char *)pRegs, sizeof(REG_SET)); /* [false alarm]:����Fortify���� */

    return (OK);
}

u32 g_ul_print_func_flag = false;

s32 dump_trc_print
    (
    u32 *	callAdrs,	/* address from which function was called */
    u32 *	funcAdrs,	/* address of function called */
    s32		nArgs,		/* number of arguments in function call */
    s32 *	args, 		/* pointer to function args */
    s32		taskId,		/* Task's ID */
    bool	isKernelAdrs,	/* TRUE if Kernel addresses */
    void *	usrCtx		/* user context */
    )
{
    UserTraceCtx* traceCtx = (UserTraceCtx*)usrCtx;
    SYM_TYPE	symType;
    char *	nameToPrint;
    char *	name;
    s32		val;
    char	demangled [DBG_DEMANGLE_PRINT_LEN + 1];
    s32		ix;
    bool	doingDefault = (bool)FALSE;
    SYMTAB_ID	symTabId = (SYMTAB_ID)0;
    static s32 trcDefaultArgs = 0;	/* default # of args to print */

    /* Print call address and name of calling function plus offset */
    (void)dump_print_buffer(traceCtx, "0x%08x ", callAdrs);	/* print address from which called */
    /* print exc task stack */
    if((u32)taskId == g_dump_base_info->reboot_task)
    {
        sprintf(demangled, "0x%08x ", (u32)callAdrs);
        printksync("%s", demangled);
    }

    if((u32)taskId != g_dump_base_info->reboot_task)
    {
        (void)dump_print_buffer(traceCtx, "\n");
        return OK;
    }

    if (!isKernelAdrs)
    	symTabId = (SYMTAB_ID)shellInternalSymTblFromTaskGet ((u32)taskId, (INSTR *)callAdrs);
    else
    	symTabId = sysSymTbl;	/* kernel symbol table for a kernel task */

    name = NULL;

    if (symTabId != (SYMTAB_ID)0
    	&& symByValueAndTypeFind (symTabId, (u32)callAdrs,
				  &name, &val, &symType,
				  SYM_MASK_NONE, SYM_MASK_NONE) == OK)
	{
    	nameToPrint = (char*)cplusDemangle (name, demangled, sizeof (demangled));
    	(void)dump_print_buffer(traceCtx, "%-13s+0x%-3x\n", nameToPrint, (s32)callAdrs - val);
        /* print exc task stack */
        if((u32)taskId == g_dump_base_info->reboot_task)
        {
            /* coverity[secure_coding] */
            sprintf(demangled, "%-13s+0x%-3x", nameToPrint, (s32)callAdrs - val); /* [false alarm]:����Fortify���� */
            printksync("%s\n", demangled);
        }
	}
    else
    {
    	(void)dump_print_buffer(traceCtx, _WRS_FRAME_PRINT_STRING);
        /* print exc task stack */
        if((u32)taskId == g_dump_base_info->reboot_task)
        {
            printksync(_WRS_FRAME_PRINT_STRING);
        }
    }
    if(name)
        free((void *)name);
    name = NULL;

    /* Print function address/name once */
    if(g_ul_print_func_flag == false)
    {
        return OK;
    }

    if (!isKernelAdrs)
    	symTabId = (SYMTAB_ID)shellInternalSymTblFromTaskGet ((u32)taskId, (INSTR *)funcAdrs);
    else
    	symTabId = sysSymTbl;	/* kernel symbol table for a kernel task */

    if (symTabId != (SYMTAB_ID)0
        && symByValueAndTypeFind (symTabId, (UINT)funcAdrs,
                  				  &name, &val, &symType,
                  				  SYM_MASK_NONE, SYM_MASK_NONE) == OK)
	{
    	nameToPrint = (char*)cplusDemangle (name, demangled, sizeof (demangled));
    	(void)dump_print_buffer(traceCtx, "%s(", nameToPrint);		/* print function name */
        if((u32)taskId == g_dump_base_info->reboot_task)
        {
            printksync("%s(", nameToPrint);
        }
	}
    else
    {
    	(void)dump_print_buffer(traceCtx, "0x%08x(", funcAdrs);	/* print function address */
        if((u32)taskId == g_dump_base_info->reboot_task)
        {
            printksync("0x%x(", (u32)funcAdrs);
        }
    }
    free((void *)name);
    name = NULL;

    /* If no args are specified, print out default number (see doc at top) */
    if (nArgs == 0 && trcDefaultArgs != 0)
	{
    	doingDefault = (bool)TRUE;
    	nArgs = trcDefaultArgs;
    	(void)dump_print_buffer(traceCtx, "[");
        if((u32)taskId == g_dump_base_info->reboot_task)
        {
            printksync("[");
        }
	}

    if (!isKernelAdrs)
    	symTabId = (SYMTAB_ID)shellInternalSymTblFromTaskGet (taskId, 0);
    else
    	symTabId = sysSymTbl;	/* kernel symbol table for a kernel task */

    /* Print args */

    for (ix = 0; ix < nArgs; ix++)
	{
    	if (ix > 0)
    	    (void)dump_print_buffer(traceCtx, ", ");

    	if (args[ix] == 0)
    	    (void)dump_print_buffer(traceCtx, "0");
    	else if (symTabId != (SYMTAB_ID)0
    		 && symByValueAndTypeFind (symTabId, (UINT)args[ix],
    					   &name, &val, &symType,
    					   SYM_MASK_NONE, SYM_MASK_NONE) == OK
    		 && val == args[ix])
    	    {
        	    nameToPrint = (char*)cplusDemangle (name, demangled,
        					                        sizeof (DBG_DEMANGLE_PRINT_LEN));
        	    (void)dump_print_buffer(traceCtx, "&%s", nameToPrint);	/* print argument name */
    	    }
    	else
    	    (void)dump_print_buffer(traceCtx, "0x%x", args[ix]);

        if(name)
            free((void *)name);
    	name = NULL;
	}

    if (doingDefault)
    {
    	(void)dump_print_buffer(traceCtx, "]");
        if((u32)taskId == g_dump_base_info->reboot_task)
        {
            printksync("]");
        }
    }
    (void)dump_print_buffer(traceCtx, ")\n");

    /* print exc task stack */
    if((u32)taskId == g_dump_base_info->reboot_task)
    {
        printksync(")\n");
    }

    return (OK);
}

extern char wrs_kernel_text_start[];
extern char wrs_kernel_text_end[];

int dump_trc_pc_validate(u32 * pc, TRC_OS_CTX * pOsCtx)
{
    u32  instr;

    pOsCtx = pOsCtx;

    /* first try kernel image text start/end address */
    if (((char *)pc >= wrs_kernel_text_start) && ((char *)pc < wrs_kernel_text_end))
	    return (ALIGNED (pc, sizeof (u32)) ? OK : ERROR);

    /* fall back to vxMemProbe() */
    return (vxMemProbe ((char *)pc, VX_READ, sizeof (u32), (char *)&instr));
}

int dump_trc_reg_count (int offset)
{
    int i, x;
    int count = 0;

    for (i=0; i<3; i++)
    {
        x = (0x00f & (offset >> (i*4)));

        switch (x)
        {
            case 0xf:
	            count += 4;
                break;
            case 0xe:
            case 0xd:
            case 0xb:
            case 0x7:
	            count += 3;
                break;
            case 0xc:
            case 0xa:
            case 0x9:
            case 0x6:
            case 0x5:
            case 0x3:
	            count += 2;
                break;
            case 0x8:
            case 0x4:
            case 0x2:
            case 0x1:
	            count += 1;
                break;
        }
    }

    return (count);
}

int dump_trc_func_info_get(TRC_OS_CTX * pCtx, u32 * pc, char * sp, u32 * lr, char ** pFp, u32 ** pReturnPc, u32 ** pFuncAddr)
{
    u32 *   pInstr = pc;
    u32 *   funcAddr = (u32 *)NULL;
    UINT32  memAddr  = 0;
    char *  fp = sp;
    u32	    instr;
    int     scanCount = 0;
    int     numSaved, offset = 0;
    int     value, rotation;
    u32     push_instr = 0;  /* 0xE92DD*** */

    /* Disable stack tracing for SMP */

#if (defined(_WRS_KERNEL) && defined (_WRS_CONFIG_SMP))

    *pFp = NULL;
    *pReturnPc = NULL;
    *pFuncAddr = NULL;
    return (ERROR);

#else

    /* �ж������ģ��ж��Ƿ񵽴��ж�ջ�� */
    if(0 != dump_in_interrrupt())
    {
        if((u32)sp + 0x20 > (u32)vxIntStackBase)
        {
            return (ERROR);
        }
    }

    if (vxCpuConfiguredGet() > 1)
    {
        *pFp = NULL;
        *pReturnPc = NULL;
        *pFuncAddr = NULL;
        return (ERROR);
    }

    /* if invalid stack pointer, return error */

    if ((BOGUS_SP (sp, pCtx->stackEnd, pCtx->stackBase)) ||
        (PC_VALIDATE (pc, pCtx) != OK))
    {
        *pFp = NULL;
        *pReturnPc = NULL;
        *pFuncAddr = NULL;
        return (ERROR);

    }

    /* begin the backward scan */

    while (scanCount++ < MAX_SCAN_DEPTH)
    {
    	/* Read instruction upcode */

    	TRC_MEM_INST_READ (pInstr, &instr, u32);

        instr = SWAP32_BE8(instr);

        /* stmdb   sp! {...} */
        if (((instr & STMDB_SP_MASK) == STDDB_SP_INSTR) || ((instr & PUSH_SP_MASK) == PUSH_SP_INSTR))
	    {
            /* return address */
            funcAddr = pInstr;

    	    /* get lr by finding out how many return how many regs
    	     * were saved to the stack via store multiple instruction.
    	     */
    	    offset = instr & OFFSET_MASK;
    	    numSaved = dump_trc_reg_count(offset);

            /* push */
            if((instr & PUSH_SP_MASK) == PUSH_SP_INSTR)
            {
                push_instr = instr;

                /* r12 push into stack */
                if(instr & 0x1000)
                {
                    numSaved++;
                }
                /* r13 push into stack */
                if(instr & 0x2000)
                {
                    numSaved++;
                }
            }

    	    lr = (u32 *) (sp + (numSaved * sizeof(UINT32)));

    	    /*
    	     * get fp, if stored via store multiple instruction
    	     *
    	     * NOTE: if not, fp dummy'd up to point to previous stack
    	     */
    	    if (instr & FP_MASK)
                  fp = (char *) (lr + 1);
    	    else
                  fp = sp;

    	    if (PC_VALIDATE (--pInstr, pCtx) == OK)
            {
        		/* Read instruction upcode */

        		TRC_MEM_INST_READ (pInstr, &instr, u32);

                instr = SWAP32_BE8(instr);

                /* sub     sp, sp, # */
                if ((instr & INSTR_MASK) == SUB_SP_SP_IMM_INSTR)
                {
                    offset = instr & OFFSET_MASK;
                    sp += offset;
                    fp += offset;

                    /* return address */
                    funcAddr = pInstr;
                }
            }

            /* r15(pc) push into stack, update fp */
            if(push_instr & 0x8000)
            {
                fp = (char *)(lr + 2);
            }

            break;
        }

        /* sub     sp, sp, # */
        if ((instr & INSTR_MASK) == SUB_SP_SP_IMM_INSTR)
        {
    	    offset = instr & OFFSET_MASK;
    	    sp += offset;
        }

        /* str     lr, [sp, #-4] */
        if (instr == STR_LR_SP_INSTR)
        {
            /* return address */
            funcAddr = pInstr;

    	    /* get lr. */
    	    lr = (u32 *)(sp);

    	    /*
    	     * NOTE: fp dummy'd up to point to previous stack
    	     */
    	    offset = instr & OFFSET_MASK;
    	    sp += offset;
            fp = sp;

            break;
        }

        /* sub     sp, sp, ip */
        if (instr == SUB_SP_SP_IP_INSTR)
        {
            /* now find value in ip */
    	    if (PC_VALIDATE (--pInstr, pCtx) == OK)
                    {
    		TRC_MEM_INST_READ (pInstr, &instr, u32);

            instr = SWAP32_BE8(instr);

                /* mov     ip, # */
                if ((instr & INSTR_MASK) == MOV_IP_IMM_INSTR)
                    {
                    value = instr & VALUE_MASK;
                    rotation = (((instr & ROTATION_MASK)>>8)*2);

                    /*
                     * convert shifter operand to offset
                     */
                    offset = rotate_N_bits (value, rotation);

                    /* adjust stack pointer */
                    sp += offset;
                    }

                /* ldr     ip, [pc, #] */
                if ((instr & INSTR_MASK) == LDR_IP_PC_IMM_INSTR)
                    {
                    /*
                     * convert shifter operand to offset,
                     * assume word-aligned
                     */

                    offset = instr & OFFSET_MASK;

                    /* relative to PC */
                    memAddr = (UINT32) pInstr;
                    memAddr += offset;
                    memAddr += 8;

                   /* adjust stack pointer */
                   TRC_MEM_INST_READ (memAddr, &offset, UINT32);
                   sp += offset;
                   }
               }
           }

        /* muleq   sp, rX, rY */
        if ((instr & MULEQ_SP_MASK) == MULEQ_SP_INSTR)
            {
	    /*
	     * No way to properly retrieve values of rX and rY,
	     * so we must terminate trace at this point...
	     */

            /* return address */
            funcAddr = pInstr;

	    /* current stack pointer */
            fp = sp;

	    break;
            }

        /* check to see if the next instruction is accessible */
        if (PC_VALIDATE (--pInstr, pCtx) != OK)
            break;
	}

    /*
     * Validate the return values.
     *
     * We have a problem if we haven't actually located a stack offsetting
     * instruction before we ran out of instructions to scan, we assume
     * that the pc we were handed was the entry point of the function.
     * Probably not right, but better than nothing.
     */
    *pFuncAddr = ((funcAddr) ? funcAddr : pc);

    *pFp = fp;

    if (BOGUS_SP ((char *)lr, pCtx->stackEnd, pCtx->stackBase))
    {

        *pReturnPc = NULL;
    }
    else
    {
        /* coverity[deref_parm] */
        *pReturnPc = (PC_VALIDATE ((u32 *) *lr, pCtx) != OK) ? NULL : (u32 *) *lr;
    }

    return (OK);

#endif /* _WRS_KERNEL && _WRS_CONFIG_SMP */
}

int dump_trc_lvl_info_get(char * fp, u32 * pc, TRC_OS_CTX * pCtx, char ** pPrevFp, u32 ** pPrevPc, u32 **	pFuncAddr)
{
    /* coverity[var_deref_model] */
    return (dump_trc_func_info_get(pCtx, pc, fp, (u32 *)0, pPrevFp, pPrevPc, pFuncAddr));
}

int dump_trc_stack_lvl_user(char * fp, u32 * pc, u32 * funcAddr, TRC_CTX * pCtx)
{
    int		nArgs = 0;
    int *	pArgs = NULL;

    fp = fp;
    /* protection from possible incorrect top frame */

    if ((pc == NULL) || (funcAddr == NULL))
	    return (OK);

    /* get the function arguments */

    if (pCtx->nArgs > 0)
	    nArgs = -1;

    /* call user routine; return status ERROR means stop printing */

    if (pCtx->usrRtn (pc, funcAddr, nArgs, pArgs, pCtx->taskId,
		      pCtx->isKernelAdrs, pCtx->usrCtx) == ERROR)
	    return (ERROR);

    return (OK);
}

int dump_trc_stack_lvl(char * fp,	u32 *	pc,	TRC_CTX * pCtx)
{
    char *	prevFp;
    u32 *	prevPc;
    u32 *	funcAddr;
    TRC_LVL_INFO trcInfo[MAX_TRACE_DEPTH];
    int		depth = 0;

    while (depth < MAX_TRACE_DEPTH)
	{
    	if (dump_trc_lvl_info_get(fp, pc, pCtx->pOsCtx, &prevFp, &prevPc,
    				    &funcAddr) == ERROR)
    	    break;

    	if ((prevPc == NULL) || (prevFp == NULL))
    	    break;

    	/* save the trace info for this level */
    	trcInfo[depth].fp       = fp;
    	trcInfo[depth].prevPc   = (INSTR *)prevPc;
    	trcInfo[depth].funcAddr = (INSTR *)funcAddr;

    	depth++;

    	/* update for next iteration */

    	fp = prevFp;
    	pc = prevPc;
	}

    /* print in reverse order */

    for (depth--; depth >= 0; depth--)
	{
    	if (dump_trc_stack_lvl_user (trcInfo[depth].fp, (u32 *)trcInfo[depth].prevPc,
    			     (u32 *)trcInfo[depth].funcAddr, pCtx) == ERROR)
    	    return (ERROR);
	}

    return (OK);
}

int dump_trc_top_info_get(TRC_OS_CTX * pCtx, char ** pFp, char ** pPrevFp, u32 ** pReturnPc, u32 ** pFuncAddr)
{
    /* vxworks define spReg to r[13] for arm */
    *pFp = (char *)pCtx->pRegs->spReg;

    return (dump_trc_func_info_get(pCtx,
             (u32 *)pCtx->pRegs->pc,
             (char *)pCtx->pRegs->spReg,
             (u32 *)pCtx->pRegs->lrReg,
             pPrevFp,
             pReturnPc,
             pFuncAddr));
}

int dump_trc_stack_ctx_trace(TRC_CTX * pCtx)
{
    char *	fp;			    /* current frame pointer */
    char *	prevFp;			/* previous frame pointer */
    u32 *	prevPc;			/* previous PC */
    u32 *	funcAddr;		/* function start address */

    if (pCtx->pOsCtx->pcValidateRtn == NULL)
	    pCtx->pOsCtx->pcValidateRtn = dump_trc_pc_validate;

    /* get the stack frame, return address etc */
    if (dump_trc_top_info_get(pCtx->pOsCtx, &fp, &prevFp, &prevPc, &funcAddr) == ERROR)
	    return (ERROR);

    if (dump_trc_stack_lvl (prevFp, prevPc, pCtx) == OK)
	{
        if((u32)(pCtx->taskId) == g_dump_base_info->reboot_task)
        {
            g_ul_print_func_flag = true;
        }
	    /* print top frame if posssible */
    	(void)dump_trc_stack_lvl_user (fp, prevPc, funcAddr, pCtx);
	}

    return (OK);
}


int dump_trc_stack_trace(REG_SET * pTaskRegs, TRC_CTX * pCtx)
{
    BOOL 	isKernelTask;		/* TRUE if kernel task */
#ifdef _WRS_CONFIG_RTP
    REG_SET	scRegs;
    SYSCALL_ENTRY_STATE * pSyscallEntryState = NULL;
    RTP_ID	oldCtx = NULL;
#endif
    WIND_TCB *  pTcb = taskTcb(pCtx->taskId);
    TRC_OS_CTX	osCtx;

    if(pTcb == NULL)
	    return (ERROR);

    /* Check if it is a kernel task or a RTP task */

    isKernelTask = ((pTcb->options & VX_SUPERVISOR_MODE) == VX_SUPERVISOR_MODE);

    /* Get info on the regular stack's top level stack frame */

    /*
     * XXX - the stackBase is not always exactly the same as the initial
     * stack pointer:
     *  - RTP init task save the initial stack frame (env, arg, auxv),
     *    as documented in SPR#93024.
     */

    osCtx.stackBase = pTcb->pStackBase;
    osCtx.stackEnd  = pTcb->pStackEnd;

    /* set the PC validate routine */

    osCtx.pcValidateRtn = (FUNCPTR)dump_trc_pc_validate;

    /* initially assume it's not syscall */

    osCtx.isSyscall = (bool)false;

    /* do memory context switch for user tasks */

#ifdef _WRS_CONFIG_RTP
    if (!isKernelTask)
	{
    	if (_func_taskMemCtxSwitch == NULL)
    	{
    	    return (ERROR);
    	}
    	else
    	{
    	    if ((oldCtx = (RTP_ID) _func_taskMemCtxSwitch (pTcb->rtpId, 0)) == NULL)
    		    return (ERROR);
        }
    	/* check if system call */

    	if (pTcb->excCnt != 0)
    	{
	    /* may be a system call */

#if (CPU_FAMILY==ARM)
    	    pSyscallEntryState = (SYSCALL_ENTRY_STATE *)(pTcb->pExcStackBase - (sizeof (SYSCALL_ENTRY_STATE) + ARM_RTP_STACK_OFFSET));
#else
    	    pSyscallEntryState = (SYSCALL_ENTRY_STATE *)(pTcb->pExcStackBase - sizeof (SYSCALL_ENTRY_STATE));
#endif /* CPU_FAMILY==ARM */

    	    osCtx.pRegs = &scRegs;

    	    trcLibFuncs.syscallCheck (pSyscallEntryState, &osCtx);
	    }
	}
#endif /* _WRS_CONFIG_RTP */

    /*
     * if it's system call, use the register values were obtained from the
     * system call entry state, and filled in by syscallCheck. Otherwise use
     * the regset pointed by pRegs;
     */

    if (!osCtx.isSyscall)
	    osCtx.pRegs = pTaskRegs;

    /* Do the stack trace on the regular stack */

    pCtx->isKernelAdrs = isKernelTask;
    pCtx->pOsCtx       = &osCtx;

    if (dump_trc_stack_ctx_trace(pCtx) == ERROR)
	{
#ifdef _WRS_CONFIG_RTP
    	if (!isKernelTask)
    	    _func_taskMemCtxSwitch (oldCtx, 0);
#endif /* _WRS_CONFIG_RTP */

    	if (pCtx->verbose && (_func_printErr != NULL))
    		_func_printErr ("trcStack aborted: error in top frame\n");

    	return (ERROR);
	}

    /* switch back memory context if needed */

#ifdef _WRS_CONFIG_RTP
    if (!isKernelTask)
	    _func_taskMemCtxSwitch (oldCtx, 0);
#endif /* _WRS_CONFIG_RTP */

    /*
     * Now do the same thing on the exception stack if the task is currently
     * using it.
     *
     * XXX - we can only do the trace for system calls. For non-system call
     * exceptions the the regset is not stored permanently anywhere; the
     * regset saved in the TCB is overwritten by taskRegsGet() with the
     * exception regset.
     */

    if (osCtx.isSyscall)
	{
	    if (pCtx->verbose && (_func_printErr != NULL))
	        _func_printErr ("---- system call boundary ----\n");

    	/* Get info on the exception stack's top level stack frame */

    	osCtx.stackBase = pTcb->pExcStackBase;
    	osCtx.stackEnd  = pTcb->pExcStackEnd;
    	osCtx.pRegs     = pTaskRegs;
    	osCtx.isSyscall = false;

    	/*
    	 * Do the stack trace on the exception stack;
    	 * these are always kernel addresses.
    	 */

	    pCtx->isKernelAdrs = true;

	    if (dump_trc_stack_ctx_trace(pCtx) == ERROR)
	    {
    	    if (pCtx->verbose && (_func_printErr != NULL))
    		    _func_printErr ("trcStack aborted: error in top frame on "
    				"exception stack\n");
    	        return (ERROR);
	    }
	}

    return (OK);
}

/*�����ǰ�����ջ��Ϣ��ָ��������*/
s32 dump_stack_trace(s32 taskId, UserTraceCtx* traceCtx)
{
    REG_SET regSet;
    bool    continueIt = false;	/* flag to remember if resuming is necessary */
    RTP_ID  oldCtx = NULL;
    TRC_CTX	trcCtx;
    s32		argBuf[MAX_TRC_ARGS];

//    dump_print_buffer(traceCtx, "start %s(%#x) stack trace.\n", taskName(taskId), taskId);

    /* Make sure the task don't try to trace itself */
    if (taskId == 0 || taskId == taskIdSelf ())
	{
    	(void)dump_print_buffer(traceCtx, "unable to self trace.%#x\n", taskId);
    	return ERROR;
	}

    /* Make sure the task exists */
    if (taskIdVerify (taskId) != OK)
	{
    	(void)dump_print_buffer(traceCtx, "invalid task id %#x.\n", taskId);
    	return ERROR;
	}

    /* If the task is not already stopped or suspended, stop it while we trace it */
    if ((!taskIsStopped (taskId)) && (!taskIsSuspended (taskId)))
	{
    	if (taskStop (taskId) != OK)
	    {
    	    if ((((WIND_TCB *) taskId)->options &
    	    		VX_UNBREAKABLE) == VX_UNBREAKABLE)
        		(void)dump_print_buffer(traceCtx, "unable to trace unbreakable task.\n");
    	    else
        		(void)dump_print_buffer(traceCtx, "cannot stop the task %#x.\n", taskId);
    	    return ERROR;
	    }

    	continueIt = true;              /* we want to resume it later */
	}

    /* Trace the stack */
    (void)dump_task_regs (taskId, &regSet);

    /*
     * switch to the memory context of the task to be traced
     * since we will probe the RTP memory. If the task's
     * memory context may not be accessed, either because
     * the RTP is being deleted or the context is invalid,
     * the stack trace will fail with an ERROR.
     */

    if (_func_taskMemCtxSwitch != NULL)
    {
        /* coverity[returned_null] */
        if ((oldCtx = (RTP_ID) _func_taskMemCtxSwitch (rtpFromTaskGet (taskId), 0)) == NULL)
	    {
     	    (void)dump_print_buffer(traceCtx, "unable to access task's (%#x) memory.\n", taskId);
    	    return ERROR;
	    }
	}

    //trcStack (&regSet, (FUNCPTR) dbgCallPrint, taskId);
    {
        trcCtx.usrRtn  = dump_trc_print;
        trcCtx.taskId  = taskId;
        trcCtx.usrCtx  = traceCtx;
        trcCtx.nArgs   = MAX_TRC_ARGS;
        trcCtx.pArgs   = argBuf;
        trcCtx.verbose = TRUE;

        /* print exc task stack */
        if((u32)taskId == g_dump_base_info->reboot_task)
        {
            printksync("0x%x (%s) stack:\n", taskId, g_dump_base_info->taskName);
        }

        //if (trcStackTrace (&regSet, &trcCtx) != OK)
        if(dump_trc_stack_trace(&regSet, &trcCtx) != OK)
        {
            (void)dump_print_buffer(traceCtx, "trcStackTrace error.\n");
        }
    }

    /* restore the memory context */
    if (_func_taskMemCtxSwitch != NULL)
    {
	    _func_taskMemCtxSwitch (oldCtx, 0);
	}

    if (continueIt)
	    (void)taskCont (taskId);		/* resume task if we suspended it */

//    dump_print_buffer(traceCtx, "end trace stack\n");

    return OK;
}

/*����쳣ջ������ϵͳJOB*/
void dump_task_show_stack(s32 tid, void* buffer, s32 bufferlen)
{
    UserTraceCtx usrCtx;

    /* Init user trace context */
    usrCtx.buffer = buffer;
    usrCtx.bufferLen = (u32)bufferlen;
    usrCtx.callPrintFunc =  (FUNCPTR)dump_trc_print;

	memset(buffer, 0, (u32)bufferlen);
	(void)dump_stack_trace(tid, &usrCtx);

    return;
}

int QueueNumGet(dump_queue_t *Q)
{
    return Q->num;
}

s32 dump_in_interrrupt(void)
{
    return intCnt;
}

void dump_task_switch_hook(void *old_tcb, void *new_tcb)
{
    /* ��Ҫ�л�������ID��ʱ�����*/
    u32 addr = DUMP_TASK_SWITCH_ADDR;
    u32 tcb_addr = DUMP_TASK_TCB_ADDR;
    u32 old_tcb_pc;
    WIND_TCB * ptcb;
    REG_SET * tcb_regs;
    s32 lock_key;
    
    if(BSP_FALSE == g_dump_core_internal->taskSwitchStart)
    {
        //old_tcb = old_tcb;
        return;
    }

    if (0 == taskIdVerify((s32)new_tcb))
    {
        ptcb = (WIND_TCB *)old_tcb;
        tcb_regs = (REG_SET *)(&(ptcb->regs));
        old_tcb_pc = (u32)(tcb_regs->pc);

        local_irq_save(lock_key);
        (void)QueueLoopIn((dump_queue_t *)tcb_addr, (u32)old_tcb_pc);
        (void)QueueLoopIn((dump_queue_t *)tcb_addr, om_timer_get());

        (void)QueueLoopIn((dump_queue_t *)addr, (u32)new_tcb);
        (void)QueueLoopIn((dump_queue_t *)addr, om_timer_get());
        local_irq_restore(lock_key);

        g_dump_base_info->current_task = (u32)new_tcb;

        g_dump_ptr_info.task_swt_ptr = DUMP_TASK_SWITCH_ADDR +0x10 + (((dump_queue_t *)addr)->rear )*4;
    }

    if(BSP_NULL != g_dump_trace_read_cb[DUMP_SYSVIEW_TASKSWITCH])
    {
        if( (QueueNumGet((dump_queue_t*)addr)*sizeof(u32)) >= DUMP_TASK_SWT_THRESHOLD_SIZE)
        {
            g_dump_trace_read_cb[DUMP_SYSVIEW_TASKSWITCH]();
        }
    }

    return;
}

void dump_int_switch_hook(u32 dir, u32 newVec)
{
    u32 addr = DUMP_TASK_SWITCH_ADDR;
    s32 lock_key;
    
    if(BSP_FALSE == g_dump_core_internal->taskSwitchStart)
    {
        return;
    }

    local_irq_save(lock_key);
    /* ���������ж�ID��ʱ�����*/
    if (0 == dir)/*�жϽ���*/
    {
        (void)QueueLoopIn((dump_queue_t *)addr, (((u32)DUMP_INT_IN_FLAG<<16)|newVec));
    }
    else/*�ж��˳�*/
    {
        (void)QueueLoopIn((dump_queue_t *)addr, (((u32)DUMP_INT_EXIT_FLAG<<16)|newVec));
    }
    (void)QueueLoopIn((dump_queue_t *)addr, om_timer_get());
    local_irq_restore(lock_key);

    g_dump_ptr_info.task_swt_ptr = DUMP_TASK_SWITCH_ADDR +0x10
                                                            + (((dump_queue_t *)addr)->rear )*4;

    if(BSP_NULL != g_dump_trace_read_cb[DUMP_SYSVIEW_TASKSWITCH])
    {
        if( (QueueNumGet((dump_queue_t *)addr)*sizeof(u32)) >= DUMP_TASK_SWT_THRESHOLD_SIZE)
        {
            g_dump_trace_read_cb[DUMP_SYSVIEW_TASKSWITCH]();
        }
    }

    g_dump_base_info->current_int = newVec;

    return;
}

typedef int (* FUNCPTR_INT)(int, ...);
extern FUNCPTR _func_excBaseHook;
FUNCPTR_INT g_sl_excBaseHook;

int dump_sym_find_symbol(
    SYMTAB_ID   symTblId,       /* symbol table ID */
    char *      name,           /* name to search for */
    void * 	value,		        /* value of symbol to search for */
    SYM_TYPE    type,           /* symbol type */
    SYM_TYPE    mask,           /* type bits that matter */
    SYMBOL_ID * pSymbolId       /* where to return id of matching symbol */
    )
{
    int	                index;      /* counter for search by value */
    SYMBOL *	        pSymbol;    /* current symbol, search by value */
    SYMBOL *	        pBestSymbol = NULL;
                                   /* symbol with lower value, matching type */
    char *		pUnder;    /* string for _text, etc., check */
    void *		bestValue = NULL;
                                    /* current value of symbol with matching type */

    name = name;
    if (pSymbolId == NULL)
    {
    	return ERROR;
	}

	/* Search by value or by value and type: */
	for (index = 0; index < (int)(((HASH_ID)(symTblId->nameHashId))->elements); index++)
	{
	    pSymbol = (SYMBOL *) SLL_FIRST(&(symTblId->nameHashId->pHashTbl[index]));

	    while (pSymbol != NULL)			/* list empty */
	    {
            if (((pSymbol->type & mask) == (type & mask)) &&
                (pSymbol->value == value) &&
                (((pUnder = rindex (pSymbol->name, '_')) == NULL) ||
                 ((strcmp (pUnder, "_text") != 0) &&
                  (strcmp (pUnder, "_data") != 0) &&
                  (strcmp (pUnder, "_bss") != 0) &&
                  (strcmp (pUnder, "_compiled.") != 0))) &&
                (((pUnder = rindex (pSymbol->name, '.')) == NULL) ||
                 ((strcmp (pUnder, ".o") != 0))) && (!(pSymbol->type & SYM_ABS)))
            {
                /* We've found the entry.  Return it. */
                *pSymbolId = pSymbol;
                return OK;
            }
            else if (((pSymbol->type & mask) == (type & mask)) && ((pSymbol->value <= value) && \
                (pSymbol->value > bestValue)) && (!(pSymbol->type & SYM_ABS)))
    		{
    		    /*
    		     * This symbol is of correct type and closer than the last
    		     * one
    		     */

    		    bestValue   = pSymbol->value;
    		    pBestSymbol = pSymbol;
    		}

    		pSymbol = (SYMBOL *)SLL_NEXT (&pSymbol->nameHNode);
		}
	}

    if (bestValue == NULL || pBestSymbol == NULL) /* any closer symbol? */
    {
        return ERROR;
    }

	*pSymbolId = pBestSymbol;
    
    return OK;
}

void dump_trc_int_print
    (
    INSTR *	callAdrs,	/* address from which function was called */
    INSTR *	funcAdrs,	/* address of function called */
    s32		nArgs,		/* number of arguments in function call */
    s32 *	args, 		/* pointer to function args */
    s32		taskId,		/* Task's ID */
    bool	isKernelAdrs,	/* TRUE if Kernel addresses */
    void *	usrCtx		/* user context */
    )
{
    s32		    val;
    char	    demangled[DBG_DEMANGLE_PRINT_LEN + 1];
    SYMTAB_ID	symTabId = sysSymTbl; /* [false alarm]:����Fortify���� */
    SYMBOL *    pSymbol = NULL;
    int         ret;

    taskId  = taskId;   /* [false alarm]:����Fortify���� */
    nArgs   = nArgs;    /* [false alarm]:����Fortify���� */
    funcAdrs= funcAdrs; /* [false alarm]:����Fortify���� */
    isKernelAdrs = isKernelAdrs; /* [false alarm]:����Fortify���� */
    args    = args;     /* [false alarm]:����Fortify���� */
    usrCtx  = usrCtx;   /* [false alarm]:����Fortify���� */
    
    ret = dump_sym_find_symbol(symTabId, NULL, (char *)(callAdrs), SYM_MASK_NONE, SYM_MASK_NONE, &pSymbol);
    if(ret == 0)
    {
        val = (int)pSymbol->value;
        /* coverity[secure_coding] */
        sprintf(demangled, "0x%08x %-13s+0x%-3x", (s32)callAdrs, pSymbol->name, (s32)callAdrs - val); /* [false alarm]:����Fortify���� */
        printksync("%s\n", demangled);
    }
    else
    {
        printksync("symbol 0x%x not found\n", (s32)callAdrs);
    }
}

void dump_int_stack_trace(REG_SET * pRegSet)
{
    TRC_CTX trcCtx;
    TRC_OS_CTX  osCtx;
    char *	prevFp;
    u32 *	prevPc;

    osCtx.stackBase = vxIntStackBase;
    osCtx.stackEnd  = vxIntStackEnd;
    osCtx.pcValidateRtn = dump_trc_pc_validate;
    osCtx.isSyscall = FALSE;
    osCtx.pRegs     = pRegSet;

    trcCtx.usrRtn  = (FUNCPTR)dump_trc_int_print;
    trcCtx.taskId  = (s32)0xffffffff;
    trcCtx.usrCtx  = 0;
    trcCtx.nArgs   = MAX_TRC_ARGS;
    trcCtx.pArgs   = 0;
    trcCtx.verbose = TRUE;
    trcCtx.isKernelAdrs = TRUE;
    trcCtx.pOsCtx  = &osCtx;

    prevFp = (char *)osCtx.pRegs->spReg;
    prevPc = (u32 *)osCtx.pRegs->pc;

    /* �Ƶ��ж�ջ */
    (void)dump_trc_stack_lvl(prevFp, prevPc, &trcCtx);
}

void dump_trc_int_stack(REG_SET * reg_set)
{
    s32		    val; /* [false alarm]:����Fortify���� */
    char	    demangled[DBG_DEMANGLE_PRINT_LEN + 1]; /* [false alarm]:����Fortify���� */
    SYMTAB_ID	symTabId = sysSymTbl; /* [false alarm]:����Fortify���� */
    SYMBOL *    pSymbol = NULL;

    printksync("[interrupt]Exception address: 0x%x\n", (int)reg_set->pc);
    printksync("interrupt %d stack:\n", g_dump_base_info->current_int);

    dump_int_stack_trace((REG_SET *)reg_set);
    if(0 == dump_sym_find_symbol(symTabId, NULL, (char *)(reg_set->pc), SYM_MASK_NONE, SYM_MASK_NONE, &pSymbol))
    {
        val = (int)pSymbol->value;
        /* coverity[secure_coding] */
        sprintf(demangled, "0x%08x %-13s+0x%-3x", (s32)reg_set->pc, pSymbol->name, (s32)reg_set->pc - val); /* [false alarm]:����Fortify���� */
        printksync("%s\n", demangled);
    }
    else
    {
        printksync("symbol 0x%x not found\n", (s32)reg_set->pc);
    }
}

int dump_int_exc_hook(int vec, ESF * pEsf, REG_SET * pRegSet, EXC_INFO * pExcInfo)
{
    s32         lock_key;
    int         ret;
    REG_SET     reg_set;

    ret = g_sl_excBaseHook(vec, pEsf, pRegSet, pExcInfo);

    /* ��ǰ�������� */
    if(0 == dump_in_interrrupt())
    {
        dump_trace_stop();
        return ret;
    }

    if(g_arm_exc_save_flag == true || g_exc_flag == true)
    {
        return BSP_OK;
    }

    dump_trace_stop();

    /* coverity[lock_acquire] */
    local_irq_save(lock_key);
    if(lock_key)
    {}

    g_dump_base_info->vec = (u32)vec;
    g_dump_base_info->cpsr = (u32)pEsf->cpsr;
    g_dump_base_info->current_task = (u32)(-1);
    g_dump_top_head->except_reason = DUMP_REASON_ARM;

    /*�����Ĵ�����Ϣ*/
    memcpy((void * )(g_dump_base_info->regSet), (const void * )(pRegSet), (size_t)(ARM_REGS_NUM*sizeof(u32)));
    g_dump_base_info->regSet[15] = (u32)pEsf->pc;

    /* �Ĵ�����Ϣ������ϣ���ֹ����ջ�����д���dataabort�ظ���¼ */
    g_arm_exc_save_flag = true;

    memcpy((void *)&reg_set, (void * )(g_dump_base_info->regSet), (size_t)(ARM_REGS_NUM*sizeof(u32)));
    dump_trc_int_stack(&reg_set);

    /*����ȫ�ָ�λ*/
    /* coverity[sleep] */
    system_error(BSP_MODU_DUMP, DUMP_REASON_ARM, 0, 0, 0);;

    /* �ж���������ARM�쳣���˳�excExcHandle */
    return 1;
}

void dump_exc_hook(void * currentTaskId, s32 vec, void * pReg)
{
    s32 lock_key;
    ESF *pEsf = (ESF*)pReg;

    if(g_exc_flag == true)
    {
        return;
    }

    /* coverity[lock_acquire] */
    local_irq_save(lock_key);
    if(lock_key)
    {}

    g_dump_base_info->vec = (u32)vec;
    g_dump_base_info->cpsr = (u32)pEsf->cpsr;
    g_dump_base_info->current_task = (u32)currentTaskId;
    g_dump_top_head->except_reason = DUMP_REASON_ARM;
    
    /*�����Ĵ�����Ϣ*/
    memcpy((void * )(g_dump_base_info->regSet), (const void * )(((WIND_TCB *)currentTaskId)->pExcRegSet), (size_t)(ARM_REGS_NUM*sizeof(u32)));
    g_dump_base_info->regSet[15] = (u32)pEsf->pc;

    g_arm_exc_save_flag = true;

    /*����ȫ�ָ�λ*/
    /* coverity[sleep] */
    system_error(BSP_MODU_DUMP, DUMP_REASON_ARM, 0, 0, 0);;

    return;
}

void dump_trc_arm_exc_init(void)
{
    g_sl_excBaseHook = (FUNCPTR_INT)_func_excBaseHook;
    _func_excBaseHook = (FUNCPTR)dump_int_exc_hook;
}
/*lint -restore */

void dump_stack_hook(s32 where)
{
    dump_task_info_t *task = (dump_task_info_t *)g_dump_ctrl.current_task_info;

    /* coverity[printf_arg_mismatch] */
	task->offset += sprintf((char *)(task->dump_stack+task->offset), "[%08lx] %pS\n", where, (char *)where);
    
    return;
}

void dump_register_hook(void)
{
    s32 ret;
    /*�����ϵͳע�ṳ�Ӻ���*/
    if(1 == g_dump_cfg.dump_cfg.Bits.taskSwitch)
    {
        ret = taskSwitchHookAdd((FUNCPTR)dump_task_switch_hook);
        if(OK != ret)
        {
            dump_error("dump_register_hook: reg task hook failed!\n");
        }

    }
    if(1 == g_dump_cfg.dump_cfg.Bits.intSwitch)
    {
        vicIntLvlChgHookAdd((FUNCPTR)dump_int_switch_hook);
    }
    if(1 == g_dump_cfg.dump_cfg.Bits.ARMexc)
    {
        excHookAdd((FUNCPTR)dump_exc_hook);
    }

    if(1 == g_dump_cfg.dump_cfg.Bits.intLock)
    {
        int_lock_hook_add((func_void)bsp_dump_int_lock,(func_void)bsp_dump_int_unlock);
    }

    /*ȱ���Ź��ж�*/

    g_dump_core_internal->taskSwitchStart = BSP_TRUE;
    g_dump_core_internal->IntSwitchStart  = BSP_TRUE;

    return;
}

__inline__ void dump_save_global_info(void)
{
    /* ������ϲ㸴λ����־��λ״̬ */
    if(DUMP_REASON_UNDEF == g_dump_top_head->except_reason)
    {
        g_dump_top_head->except_reason = DUMP_REASON_NORMAL;
    }

    g_dump_top_head->except_core = DUMP_CPU_COMM;
    g_dump_core_internal->start_flag = DUMP_START_EXCH;

    return;
}

__inline__ void dump_save_global_base_info(void)
{
    g_dump_global_base_info->reboot_context = g_dump_base_info->reboot_context;
    g_dump_global_base_info->reboot_task = g_dump_base_info->reboot_task;
    g_dump_global_base_info->reboot_int = g_dump_base_info->reboot_int;
    g_dump_global_base_info->modId = g_dump_base_info->modId;
    g_dump_global_base_info->arg1 = g_dump_base_info->arg1;
    g_dump_global_base_info->arg2 = g_dump_base_info->arg2;
    g_dump_global_base_info->arg3 = g_dump_base_info->arg3;
    g_dump_global_base_info->arg3_length = g_dump_base_info->arg3_length;
    g_dump_global_base_info->vec = g_dump_base_info->vec;
    g_dump_global_base_info->cpsr = g_dump_base_info->cpsr;
    g_dump_global_base_info->current_task = g_dump_base_info->current_task;
    g_dump_global_base_info->current_int = g_dump_base_info->current_int;
    g_dump_global_base_info->cpu_usage = g_dump_base_info->cpu_usage;
    g_dump_global_base_info->mem_free = g_dump_base_info->mem_free;
    memcpy(g_dump_global_base_info->taskName, g_dump_base_info->taskName, sizeof(g_dump_global_base_info->taskName));
    memcpy(g_dump_global_base_info->regSet, g_dump_base_info->regSet, sizeof(g_dump_global_base_info->regSet));
    memcpy(g_dump_global_base_info->axi_dlock_reg, g_dump_base_info->axi_dlock_reg, sizeof(g_dump_global_base_info->axi_dlock_reg));
    return;    
}

void dump_save_base_info(u32 mod_id, u32 arg1, u32 arg2, char *data, u32 length)
{
    if(BSP_MODU_OTHER_CORE == mod_id)
    {
        g_dump_base_info->reboot_task = (u32)(-1);
        g_dump_base_info->reboot_int = (u32)(-1);
    }
    else
    {
        /* ��ǰ�������� */
        if (0 == dump_in_interrrupt())
        {
            //if(!DUMP_T_TASK_ERROR(mod_id))
            if(mod_id == 0x11000025 || mod_id == 0x1100002A)
            {
                g_dump_base_info->reboot_task = arg1;
            }
            else
            {

                g_dump_base_info->reboot_task = g_dump_base_info->current_task;
            }
            if(NULL != (void*)g_dump_base_info->reboot_task)
            {
                memcpy(g_dump_base_info->taskName, ((WIND_TCB *)(g_dump_base_info->reboot_task))->objCore.name, 16);
            }
            g_dump_base_info->reboot_int = (u32)(-1);
        }
        /* ��ǰ�����ж� */
        else
        {
            g_dump_base_info->reboot_task = (u32)(-1);
            memset(g_dump_base_info->taskName, 0, sizeof(g_dump_base_info->taskName));
            g_dump_base_info->reboot_int = g_dump_base_info->current_int;
        }
    }

    /*��¼��ǰ״̬������̬�����ж�̬*/
    if (!dump_in_interrrupt())
    {
        g_dump_base_info->reboot_context = DUMP_CTX_TASK;
    }
    else
    {
        g_dump_base_info->reboot_context = DUMP_CTX_INT;
    }

    g_dump_base_info->modId = mod_id;
    g_dump_base_info->arg1 = arg1;
    g_dump_base_info->arg2 = arg2;
    g_dump_base_info->arg3 = (u32)data;
    g_dump_base_info->arg3_length = length;

    /*save blobal_base_info*/
    dump_save_global_base_info();

    return;
}


void dump_save_log(void)
{
    memset((void*)DUMP_PRINT_ADDR, 0, DUMP_PRINT_SIZE);/* [false alarm]:����Fortify���� */
    (void)dmesg_save((char*)DUMP_PRINT_ADDR, DUMP_PRINT_SIZE);
}

/*lint -save -e123 -e715 */
__inline__ void dump_save_arm_regs(u32 addr)
{
    asm volatile(
        "str r0, [r0,#0x00]\n"
        "str r1, [r0,#0x04]\n"
        "str r2, [r0,#0x08]\n"
        "str r3, [r0,#0x0C]\n"
        "str r4, [r0,#0x10]\n"
        "str r5, [r0,#0x14]\n"
        "str r6, [r0,#0x18]\n"
        "str r7, [r0,#0x1C]\n"
        "str r8, [r0,#0x20]\n"
        "str r9, [r0,#0x24]\n"
        "str r10, [r0,#0x28]\n"
        "str r11, [r0,#0x2C]\n"
        "str r12, [r0,#0x30]\n"
        "str r13, [r0,#0x34]\n"
        "str r14, [r0,#0x38]\n"
        "push {r1}\n"
        "mov r1, pc\n"
        "str r1, [r0,#0x3C]\n"
        "mrs r1, cpsr\n"
        "str r1, [r0,#0x40]\n"
        "pop {r1}\n"
    );
}
/*lint -restore */

void dump_save_exc_task(u32 addr)
{
#if 0
    WIND_TCB *ptcb = (WIND_TCB*)g_dump_base_info->current_task;
    s32 len;
#endif

    WIND_TCB *ptcb = _WRS_KERNEL_GLOBAL_ACCESS(taskIdCurrent);
    s32 len;    
    dump_area_field_t* p_field; 

    /* ARM�쳣���Ĵ�����Ϣ�Ѿ����� */
    if(g_arm_exc_save_flag != true)
    {
        dump_save_arm_regs(addr);
    }
    g_dump_base_info->current_task = (u32)ptcb;

    /*�����ж�ջ����*/
    memcpy((void * )DUMP_INT_STACK_ADDR, (const void * )vxIntStackEnd, (size_t )(DUMP_INT_STACK_SIZE)); /* [false alarm]:����Fortify���� */

    /* ��������ջ */
    len = ((ptcb->pStackBase - ptcb->pStackEnd)
           > DUMP_TASK_STACK_SIZE) ? DUMP_TASK_STACK_SIZE : (ptcb->pStackBase - ptcb->pStackEnd);
    memcpy((void * )DUMP_TASK_STACK_ADDR, (const void * )(ptcb->pStackBase - len), (size_t )len); /* [false alarm]:����Fortify���� */
    p_field = dump_get_field(DUMP_CP_TASK_STACK);
    /* coverity[dereference] */
    p_field->length = len;
  
    return;
}

/*lint -save -e713 -e732 -e737 -e746 -e115 -e40 -e550 -e420 -e438*/
void dump_save_regs(void)
{
    s32 size;

    if(g_dump_cfg.dump_cfg.Bits.commRegSave1)
    {
        size = (g_dump_cfg.commRegSize1< DUMP_REG_SET_MAX) ? g_dump_cfg.commRegSize1 : DUMP_REG_SET_MAX;
        regcpy((void*)(DUMP_REGS_ADDR + sizeof(dump_regs_map_t)), (void*)g_dump_cfg.commRegAddr1, size);
        g_dump_reg_map->reg_addr1 = g_dump_cfg.commRegAddr1;
        g_dump_reg_map->reg_size1 = size;
    }
    if(g_dump_cfg.dump_cfg.Bits.commRegSave2)
    {
        size = (g_dump_cfg.commRegSize2 < DUMP_REG_SET_MAX) ? g_dump_cfg.commRegSize2 : DUMP_REG_SET_MAX;
        regcpy((void*)(DUMP_REGS_ADDR+ DUMP_REG_SET_MAX), (void*)g_dump_cfg.commRegAddr2, size);
        g_dump_reg_map->reg_addr2 = g_dump_cfg.commRegAddr2;
        g_dump_reg_map->reg_size2 = size;
    }
    if(g_dump_cfg.dump_cfg.Bits.commRegSave3)
    {
        size = (g_dump_cfg.commRegSize3 < (DUMP_REG_SET_MAX-sizeof(dump_regs_map_t)))
            ? g_dump_cfg.commRegSize3
            : (DUMP_REG_SET_MAX-sizeof(dump_regs_map_t));
        regcpy((void*)(DUMP_REGS_ADDR+ DUMP_REG_SET_MAX*2), (void*)g_dump_cfg.commRegAddr3, size);
        g_dump_reg_map->reg_addr3 = g_dump_cfg.commRegAddr3;
        g_dump_reg_map->reg_size3 = size;
    }

    return;
}


void dump_save_usr_data(char *data, u32 length)
{
    s32 len;

    /* coverity[returned_null] */
    dump_area_field_t* p_field = dump_get_field(DUMP_CP_USER_DATA);

    if(p_field == BSP_NULL)
    {
        return;
    }

    if((NULL==data)||(0== length))
    {
        p_field->length = 0;
        return;
    }

    len = (length > DUMP_USER_DATA_SIZE) ? DUMP_USER_DATA_SIZE : length;
    memcpy((void *)DUMP_USER_DATA_ADDR, (const void *)data, (size_t)len); /* [false alarm]:����Fortify���� */
	p_field->length = len;
    return;
}

#define dump_save_comm_done()\
    do{\
        g_dump_core_internal->save_flag = DUMP_SAVE_SUCCESS;\
    }while(0)

#define dump_save_ext_done()\
    do{\
        g_dump_core_internal->ext_save_flag = DUMP_SAVE_SUCCESS;\
    }while(0)


void dump_save_ext(void)
{
    struct list_head* cur = NULL;
    dump_field_hook_t* pfieldhook = NULL;
    int nlock; 
    
    list_for_each(cur, &g_dump_hook_list)
    {
        nlock = intLock();
        pfieldhook = list_entry(cur, dump_field_hook_t, hook_list);
        intUnlock(nlock);  
        
        if(pfieldhook->pfunc)
        {
            pfieldhook->pfunc();
        }
    }

    dump_save_ext_done();

    return;
}

__inline__ void dump_wait_for_reboot(void)
{
    /* coverity[no_escape] */
    do{
    }while(1);
}

void dump_wait_and_reboot(void)
{
#define DUMP_WAIT_MAX   3000   /* 30S ����Ҫ����modem�������������ʱ��ӳ� */
    s32 time_start, time_cur;
    int flag = 1;
    
    time_start = om_timer_tick_get();

    do{
        time_cur = om_timer_tick_get();
        if(DUMP_WAIT_MAX < (time_cur - time_start))
        {
            dump_debug("long time: %d %d %d %d\n", time_start, time_cur);
            break;
        }
    }while(flag);

    /* ����DDR������ˢ�� */

    /* ����ϵͳ */
    dump_fetal("system dead loop in dump_wait_and_reboot\n");
    /* g_dump_core_internal->save_file_flag = DUMP_SAVE_FILE_NEED; */
    ((dump_global_internal_t*)DUMP_GLOBAL_INTERNAL_ADDR)->app_internal.save_file_flag = DUMP_SAVE_FILE_NEED;
    if(1 == g_dump_cfg.dump_cfg.Bits.sysErrReboot)
    {
        /* K3 DUMP��λ��AP��ɣ��˺���ֻ�ܵ�����λCP�����AP���ܸ�λϵͳ CP������λ������, ���ٵ��ô˺��� */
        /* bsp_drv_power_reboot_direct(); */
    }

    /* ��ֹϵͳ�������� */
    do
    {
    }while(1);
}

/*lint -save -e115 -e40 -e550 -e420 */
void dump_save_all_task_tcb(s32 tid)
{
    s32 i;
    s32 task_num = 0;
    dump_task_info_t *task_info;
    u32 buffer = (u32)DUMP_ALLTASK_ADDR;
    s32 taskIdList[DUMP_TASK_NUM_MAX] = {0};
    u8 task_name[12] = {0};
    WIND_TCB *ptcb = NULL;
    TASK_DESC td = {0};                /* task descriptor */

    tid = tid; /* [false alarm]:����Fortify���� */

    /*��õ�ǰ��������ID*/
    task_num = (UINT32)taskIdListGet(taskIdList, DUMP_TASK_NUM_MAX);
    if (task_num > DUMP_TASK_NUM_MAX)
    {
        task_num = DUMP_TASK_NUM_MAX;
    }

    /*��������ID���NAME�����ŵ�ָ���ڴ���*/
    for (i = 0; i < task_num; i++)
    {
        strncpy((char *)task_name, (const char *)taskName(taskIdList[i]), 11);
        (void)QueueLoopIn((dump_queue_t *)buffer, (u32)taskIdList[i]);
        (void)QueueLoopIn((dump_queue_t *)buffer, *((u32 *)(task_name)));
        (void)QueueLoopIn((dump_queue_t *)buffer, *((u32 *)(task_name+4)));
        (void)QueueLoopIn((dump_queue_t *)buffer, *((u32 *)(task_name+8)));
    }

    for (i = 0; i < (s32)task_num; i++)
    {
        /* ���ȱ���������Ϣ*/
        ptcb = (WIND_TCB *)taskIdList[i];
    	if (taskInfoGet (taskIdList[i], &td) != OK)
        {
            continue;
    		//return;  /*continue*/
    	}
        task_info = &g_dump_task_info[i];
        task_info->pid = taskIdList[i];
        task_info->entry = (u32)td.td_entry;
        task_info->status = td.td_status;
        task_info->policy = TASK_SCHED_RR;
        task_info->priority = td.td_priority;
        task_info->stack_base = (u32)td.td_pStackBase;
        task_info->stack_end = (u32)td.td_pStackEnd;
        task_info->stack_high = td.td_stackHigh;
        task_info->stack_current = td.td_stackCurrent;
        /* coverity[buffer_size_warning] */
        strncpy((void*)task_info->name, (void*)td.td_name, 16);
        if(ptcb->pExcRegSet != NULL)
        {
            memcpy((void *)(task_info->regs), (void *)(ptcb->pExcRegSet), 17*sizeof(u32));
        }
        else
        {
            memcpy((void *)(task_info->regs), (void *)(&ptcb->regs), 17*sizeof(u32));
        }

        dump_task_show_stack(task_info->pid, task_info->dump_stack, DUMP_TASK_INFO_STACK_SIZE);
    }

    return;
}

void dump_save_all_task_job(s32 tid, s32 agent_dump)
{
    dump_save_all_task_tcb(tid);

    /*�����ӡ��Ϣ*/
    dump_save_log();

    (void)cacheFlush(DATA_CACHE, (u8 *)DDR_MCORE_ADDR, ENTIRE_CACHE);

    /* ����DSP DTCM */
    dump_save_dtcm();

    /*COMM���ݱ������ */
    dump_save_comm_done();

    if(agent_dump)
    {
        //dump_wait_for_reboot();
    }
    else
    {
        dump_wait_and_reboot();
    }

    return;
}


void dump_save_all_task(bool agent_dump)
{
	if( OK != excJobAdd((VOIDFUNCPTR)dump_save_all_task_job,
                        (s32)g_dump_base_info->reboot_task, agent_dump, 0, 0, 0, 0))
	{
		dump_save_all_task_job((s32)g_dump_base_info->reboot_task, agent_dump);
	}

    return; /*clean lint e713*/
}


void dump_trace_stop(void)
{
    bsp_utrace_stop();
    vicIntLvlChgHookDel();

    g_dump_core_internal->taskSwitchStart = BSP_FALSE;
    g_dump_core_internal->IntSwitchStart = BSP_FALSE;

    return;
}

void dump_system_error_enter(void)
{
    /* ֹͣ��¼�����л�/�жϵ� */
    dump_trace_stop();

    /* ��ֹ��ռ */
//    local_irq_save();

    return;
}

void dump_notify_target(void)
{
    (void)bsp_ipc_int_send(IPC_CORE_ACORE, IPC_ACPU_SRC_CCPU_DUMP);
}

void dump_int_handle(s32 param)
{
    param = param; /* [false alarm]:����Fortify���� */
    /* �Ѿ������쳣�����ټ�¼ */
    if(g_exc_flag == true)
    {
        return;
    }
    else
    {
        g_exc_flag = true;
    }
    /*�����쳣������Ϣ*/
    dump_save_exc_task((u32)(&(g_dump_base_info->regSet[0])));

    dump_system_error_enter();

    printksync("[0x%x][dump]: <%s> ================ ccore enter system error agent! ================\n",
    om_timer_get(), __FUNCTION__);

    if (DUMP_INIT_FLAG != g_dump_core_internal->init_flag)
    {
        /*�ȴ�����*/
        dump_wait_for_reboot();

        return;
    }

    /* ���ȱ���ϵͳ�쳣������Ϣ */
    dump_save_base_info(BSP_MODU_OTHER_CORE, 0, 0, BSP_NULL, 0);

    /*����Ĵ�����Ϣ*/
    dump_save_regs();

    /* ������չ������ */
    dump_save_ext();

    /*��������������Ϣ*/
    dump_save_all_task((bool)BSP_TRUE);

    return;
}

s32 dump_config_init(void)
{
    s32 ret = BSP_ERROR;

    ret = bsp_nvm_read(NVID_DUMP, (u8*)&g_dump_cfg, sizeof(dump_nv_s));
    if (BSP_OK != ret)
    {
        /*ʹ��Ĭ��ֵ*/
        g_dump_cfg.dump_cfg.Bits.dump_switch     = 0x1;
        g_dump_cfg.dump_cfg.Bits.ARMexc          = 0x1;
        g_dump_cfg.dump_cfg.Bits.stackFlow       = 0x1;
        g_dump_cfg.dump_cfg.Bits.taskSwitch      = 0x1;
        g_dump_cfg.dump_cfg.Bits.intSwitch       = 0x1;
        g_dump_cfg.dump_cfg.Bits.intLock         = 0x1;
        g_dump_cfg.dump_cfg.Bits.appRegSave1     = 0x0;
        g_dump_cfg.dump_cfg.Bits.appRegSave2     = 0x0;
        g_dump_cfg.dump_cfg.Bits.appRegSave3     = 0x0;
        g_dump_cfg.dump_cfg.Bits.commRegSave1    = 0x0;
        g_dump_cfg.dump_cfg.Bits.commRegSave2    = 0x0;
        g_dump_cfg.dump_cfg.Bits.commRegSave3    = 0x0;
        g_dump_cfg.dump_cfg.Bits.sysErrReboot    = 0x1;
        g_dump_cfg.dump_cfg.Bits.reset_log       = 0x1;
        g_dump_cfg.dump_cfg.Bits.fetal_err       = 0x1;
        dump_error("[dump]: read NV failed, use the default value!.ret = %d nv id = 0x%x\n", ret, NVID_DUMP);
    }

    return BSP_OK;
}

void dump_map_init(void)
{
    int nlock = 0;
    dump_area_head_t*  p_head  = (dump_area_head_t*)DUMP_HEAD_ADDR;
    dump_area_field_t* p_field = (dump_area_field_t*)DUMP_MAP_ADDR;

    /* ��ʼ��AREA HEAD���� */
    p_head->magic_num = DUMP_CP_MAGIC_NUM;
    strcpy((char*)p_head->name, "CP");          /* [false alarm]:����Fortify���� */
    /* coverity[secure_coding] */
    strcpy((char*)p_head->version, "K300V300"); /* [false alarm]:����Fortify���� */

    nlock = intLock();

    /* ע��DUMP�ڲ����� */   
    MAKE_FIXED_FIELD(&p_field[g_cp_field_num], DUMP_CP_BASE_INFO,       DUMP_BASE_INFO_ADDR,        DUMP_BASE_INFO_SIZE)
    g_cp_field_num ++;
    MAKE_FIXED_FIELD(&p_field[g_cp_field_num], DUMP_CP_TASK_SWITCH,     DUMP_TASK_SWITCH_ADDR,      DUMP_TASK_SWITCH_SIZE)
    g_cp_field_num ++;
    MAKE_FIXED_FIELD(&p_field[g_cp_field_num], DUMP_CP_INTLOCK,         DUMP_INTLOCK_ADDR,          DUMP_INTLOCK_SIZE)
    g_cp_field_num ++;
    MAKE_FIXED_FIELD(&p_field[g_cp_field_num], DUMP_CP_TASK_STACK,      DUMP_TASK_STACK_ADDR,       DUMP_TASK_STACK_SIZE)
    g_cp_field_num ++;
    MAKE_FIXED_FIELD(&p_field[g_cp_field_num], DUMP_CP_INT_STACK,       DUMP_INT_STACK_ADDR,        DUMP_INT_STACK_SIZE)
    g_cp_field_num ++;
    MAKE_FIXED_FIELD(&p_field[g_cp_field_num], DUMP_CP_ALLTASK,         DUMP_ALLTASK_ADDR,          DUMP_ALLTASK_SIZE)
    g_cp_field_num ++;
    MAKE_FIXED_FIELD(&p_field[g_cp_field_num], DUMP_CP_ALLTASK_TCB,     DUMP_ALLTASK_TCB_ADDR,      DUMP_ALLTASK_TCB_SIZE)
    g_cp_field_num ++;
    MAKE_FIXED_FIELD(&p_field[g_cp_field_num], DUMP_CP_PRINT,           DUMP_PRINT_ADDR,            DUMP_PRINT_SIZE)
    g_cp_field_num ++;
    MAKE_FIXED_FIELD(&p_field[g_cp_field_num], DUMP_CP_DMESG,           DUMP_DMESG_ADDR,            DUMP_DMESG_SIZE)
    g_cp_field_num ++;
    MAKE_FIXED_FIELD(&p_field[g_cp_field_num], DUMP_CP_REGS,            DUMP_REGS_ADDR,             DUMP_REGS_SIZE)
    g_cp_field_num ++;
    MAKE_FIXED_FIELD(&p_field[g_cp_field_num], DUMP_CP_CPUVIEW,         DUMP_CPUVIEW_ADDR,          DUMP_CPUVIEW_SIZE)
    g_cp_field_num ++;
    MAKE_FIXED_FIELD(&p_field[g_cp_field_num], DUMP_CP_MEMINFO,         DUMP_MEMINFO_ADDR,          DUMP_MEMINFO_SIZE)
    g_cp_field_num ++;
    MAKE_FIXED_FIELD(&p_field[g_cp_field_num], DUMP_CP_USER_DATA,       DUMP_USER_DATA_ADDR,        DUMP_USER_DATA_SIZE)
    g_cp_field_num ++;
    MAKE_FIXED_FIELD(&p_field[g_cp_field_num], DUMP_CP_BBE16_TCM,       DUMP_BBE16_TCM_ADDR,        DUMP_BBE16_TCM_SIZE)
    g_cp_field_num ++;
    MAKE_FIXED_FIELD(&p_field[g_cp_field_num], DUMP_SAVE_MOD_DRX_CCORE, DUMP_EXT_OM_DRX_CCORE_ADDR, DUMP_EXT_OM_DRX_CCORE_SIZE)
    g_cp_field_num ++;
    MAKE_FIXED_FIELD(&p_field[g_cp_field_num], DUMP_CP_TASK_TCB,        DUMP_TASK_TCB_ADDR,         DUMP_TASK_TCB_SIZE);
    g_cp_field_num ++;
    MAKE_FIXED_FIELD(&p_field[g_cp_field_num], DUMP_CP_RSV,             DUMP_RSV_ADDR,              DUMP_RSV_SIZE);
    g_cp_field_num ++;
    
    p_head->field_num = g_cp_field_num;
    
    intUnlock(nlock);

    g_dump_top_head = (dump_top_head_t*)DUMP_TOP_HEAD_ADDR;
    g_dump_core_internal = &((dump_global_internal_t*)DUMP_GLOBAL_INTERNAL_ADDR)->comm_internal;
    g_dump_global_base_info = (dump_global_base_info_t*)DUMP_GLOBAL_BASE_INFO_ADDR;
    g_dump_base_info = (dump_base_info_t*)DUMP_BASE_INFO_ADDR;
    g_dump_task_info = (dump_task_info_t*)DUMP_ALLTASK_TCB_ADDR;
    g_dump_reg_map   = (dump_regs_map_t*)DUMP_REGS_ADDR;

    g_dump_reg_map->reg_addr1 = 0;
    g_dump_reg_map->reg_size1 = 0;
    g_dump_reg_map->reg_addr2 = 0;
    g_dump_reg_map->reg_size2 = 0;
    g_dump_reg_map->reg_addr3 = 0;
    g_dump_reg_map->reg_size3 = 0;

    return;
}

void dump_global_info_init(void)
{
    g_dump_core_internal->start_flag = DUMP_START_CRASH;
    g_dump_core_internal->save_flag = BSP_FALSE;
    g_dump_core_internal->ext_save_flag = BSP_FALSE;
    g_dump_core_internal->inInterrupt = 0;
    g_dump_core_internal->taskSwitchStart = BSP_FALSE;
    g_dump_core_internal->IntSwitchStart = BSP_FALSE;
}

void dump_global_base_info_init(void)
{
    memset(g_dump_global_base_info, 0, sizeof(dump_global_base_info_t));
    g_dump_global_base_info->vec = DUMP_ARM_VEC_UNKNOW;   
}

void dump_base_info_init(void)
{
    int i;

    memset(g_dump_base_info, 0, sizeof(dump_base_info_t));
    g_dump_base_info->vec = DUMP_ARM_VEC_UNKNOW;

    memset(g_dump_reg_map, 0, sizeof(dump_regs_map_t));

    memset((void*)DUMP_ALLTASK_TCB_ADDR, 0, DUMP_ALLTASK_TCB_SIZE); /* [false alarm]:����Fortify���� */
    for(i=0; i<DUMP_LINUX_TASK_NUM_MAX; i++)
    {
        g_dump_task_info[i].pid = 0xffffffff;
    }

    g_dump_base_info->int_stack_base = (u32)vxIntStackBase;
    g_dump_base_info->int_stack_end  = (u32)vxIntStackEnd;

    /*���modem�汾�źͰ汾����ʱ�� */
    memset(g_dump_base_info->version, 0, sizeof(g_dump_base_info->version));
    memset(g_dump_base_info->compile_time, 0, sizeof(g_dump_base_info->compile_time));
    strcpy((char*)g_dump_base_info->version, (char*)bsp_version_get_firmware());
    strcpy((char*)g_dump_base_info->compile_time, (char*)bsp_version_get_build_date_time());

    /*init global base info*/
    dump_global_base_info_init();
}

void dump_queue_t_init(void)
{
    /* ��ʼ�������л����� */
    QueueInit((dump_queue_t *)(DUMP_TASK_SWITCH_ADDR), (DUMP_TASK_SWITCH_SIZE - 0x10) / 0x4);
    g_dump_ptr_info.task_swt_ptr = DUMP_TASK_SWITCH_ADDR +0x10;

    /* ��ʼ���ж϶��� */
    QueueInit((dump_queue_t *)(DUMP_INTLOCK_ADDR), (DUMP_INTLOCK_SIZE - 0x10) / 0x4);
    g_dump_ptr_info.int_lock_ptr= DUMP_INTLOCK_ADDR +0x10;

    /* ��ʼ������������ */
    QueueInit((dump_queue_t *)(DUMP_ALLTASK_ADDR), (DUMP_ALLTASK_SIZE - 0x10) / 0x4);

    QueueInit((dump_queue_t *)(DUMP_TASK_TCB_ADDR), (DUMP_TASK_TCB_SIZE - 0x10) / 0x4);

    return;
}

/* ͨ��ID��ȡ�������� */
dump_area_field_t* dump_get_field(u32 field_id)
{
    u32 i =0;
    dump_area_t* p_area = (dump_area_t*)(DUMP_COMM_SECTION_ADDR);
    
    for(i=0; i<g_cp_field_num; i++)
    {
        if(p_area->fields[i].field_id == field_id)
        {
            return &p_area->fields[i];
        }
    }

    return BSP_NULL;
}

void dump_wait_save(void)
{
    int flag = 1;
    g_dump_core_internal = &((dump_global_internal_t*)DUMP_GLOBAL_INTERNAL_ADDR)->comm_internal;
    
    do{
        if(DUMP_INIT_FLAG_WAIT == g_dump_core_internal->start_wait_flag)
            break;
        (void)taskDelay(1);
    }while(flag);

    g_dump_core_internal->start_wait_flag = 0;
}

s32 dump_init_phase1(void)
{
    s32 ret;

    /*��ȡ����*/
    ret = dump_config_init();
    if(BSP_OK != ret)
    {
        /* ��ȡ����ʧ�ܣ��쳣����*/
    }

    return BSP_OK;
}

s32 dump_task_init(void)
{
    s32 ret;
    /* ע���ж� */
    ret = bsp_ipc_int_connect(IPC_CCPU_SRC_ACPU_DUMP, (voidfuncptr)dump_int_handle, 0);
    if(BSP_OK != ret)
    {
        dump_error("bsp_ipc_int_connect failed! ret = %d\n", ret);
        return BSP_ERR_DUMP_INIT_FAILED;
    }

    ret = bsp_ipc_int_enable(IPC_CCPU_SRC_ACPU_DUMP);
    if(OK != ret)
    {
        dump_error("bsp_ipc_int_enable failed! ret = %d\n", ret);
        return BSP_ERR_DUMP_INIT_FAILED;
    }

    return BSP_OK;
}

__inline__ void dump_init_done(void)
{
    g_dump_core_internal->init_flag = DUMP_INIT_FLAG;
    return;
}

/* dump��ʼ��ʱ������ϵͳ����������ֹ��������£��������ò������� */
void bsp_dump_init_task_name(void)
{
    s32 i;
    s32 task_num = 0;
    u32 buffer = (u32)DUMP_ALLTASK_ADDR;
    s32 taskIdList[DUMP_TASK_NUM_MAX] = {0};
    u8 task_name[12] = {0};

    /*��õ�ǰ��������ID*/
    task_num = (UINT32)taskIdListGet(taskIdList, DUMP_TASK_NUM_MAX);
    if (task_num > DUMP_TASK_NUM_MAX)
    {
        task_num = DUMP_TASK_NUM_MAX;
    }

    /*��������ID���NAME�����ŵ�ָ���ڴ���*/
    for (i = 0; i < task_num; i++)
    {
        strncpy((char *)task_name, (const char *)taskName(taskIdList[i]), 11);
        (void)QueueLoopIn((dump_queue_t *)buffer, taskIdList[i]);
        (void)QueueLoopIn((dump_queue_t *)buffer, *((int *)(task_name)));
        (void)QueueLoopIn((dump_queue_t *)buffer, *((int *)(task_name+4)));
        (void)QueueLoopIn((dump_queue_t *)buffer, *((int *)(task_name+8)));
    }
}

s32 dump_init(void)
{
    s32 ret;

    dump_trc_arm_exc_init();

    /*��ʼ�����ֽṹ��*/
    ret  = dump_init_phase1();
    if(BSP_OK != ret)
    {
        dump_error("dump_init_phase1 fail 0x%x\n",ret);
        return BSP_ERROR;
    }

    dump_wait_save();

    /*��ʼ��������ж�*/
    ret = dump_task_init();
    if(BSP_OK != ret)
    {
        dump_error("dump_task_init fail 0x%x\n",ret);
        return BSP_ERROR;
    }

    /*�ڴ沼��*/
    dump_map_init();

    /*ȫ����Ϣ��ʼ��*/
    dump_global_info_init();

    dump_base_info_init();

    /*��ʼ������*/
    dump_queue_t_init();

    /*ϵͳ���лص�����ע��*/
    dump_register_hook();

    /*ϵͳ���лص�����ע��*/
    dump_init_done();

    dump_fetal("DUMP INIT SUCCESS!\n");

    /* �ӳ�һ���Ӻ󣬱���һ��ϵͳ���������� */
    (void)taskDelay(6000);
    bsp_dump_init_task_name();

    return BSP_OK;
}

s32 bsp_dump_init(void)
{
    g_exc_flag = false;
    g_arm_exc_save_flag = false;

    /* ��ʼ������������ */
    QueueInit((dump_queue_t *)(DUMP_ALLTASK_ADDR), (DUMP_ALLTASK_SIZE - 0x10) / 0x4);

    (void)taskSpawn("tDumpInit", 100, 0, 0x1000, (FUNCPTR)dump_init, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    return BSP_OK;
}

void system_error (u32 mod_id, u32 arg1, u32 arg2, char *data, u32 length)
{
    /* �Ѿ������쳣�����ټ�¼ */
    if(g_exc_flag == true)
    {
        return;
    }
    else
    {
        g_exc_flag = true;
    }

    /*�����쳣������Ϣ*/
    dump_save_exc_task((u32)(&(g_dump_base_info->regSet[0])));
   
    if (DUMP_INIT_FLAG != g_dump_core_internal->init_flag)
    {
        return;
    }

    dump_system_error_enter();

    /* ʹ��printksync����֤���ж���Ҳ�ܹ���� */
    printksync("[0x%x][dump]: <%s> ccore enter system error!\n", om_timer_get(), __FUNCTION__);

    /* ֪ͨcomm CPU�����쳣���� */
    dump_notify_target();

    /* ���ȱ���ϵͳ�쳣������Ϣ */
    dump_save_global_info();
    dump_save_base_info(mod_id, arg1, arg2, data, length);

    /*����Ĵ�����Ϣ*/
    dump_save_regs();

    /*�����ϲ��û�����*/
    dump_save_usr_data(data, length);

    /* ������չ������ */
    dump_save_ext();

    /* ���������� */
    if(0 == dump_in_interrrupt())
    {
        /*��������������Ϣ*/
    	dump_save_all_task((bool)BSP_FALSE);
    }
    /* �ж������� */
    else
    {
        dump_save_all_task_job((s32)g_dump_base_info->reboot_task, BSP_FALSE);
    }

    /*APP���ݱ����ϵͳ��������excTask���� */

    return;
}

s32 bsp_dump_register_hook(dump_save_modid_t mod_id, dump_save_hook func)
{
    int nlock = 0;
    
    dump_field_hook_t* pfieldhook = (dump_field_hook_t*)bsp_malloc(sizeof(dump_field_hook_t));
    if(pfieldhook == NULL)
    {
        return BSP_ERROR;
    }
    
    pfieldhook->field_id  = mod_id;
    pfieldhook->pfunc     = func;
    
    nlock = intLock();
    list_add(&pfieldhook->hook_list, &g_dump_hook_list);
    intUnlock(nlock);
    /* coverity[leaked_storage] */
    return BSP_OK;
}

/* ����mod_id��ȡ��Ӧ��buffer��ַ��ʧ�ܷ���0 */
u32 bsp_dump_get_buffer_addr(dump_save_modid_t mod_id)
{
    u32 addr = 0;
    dump_save_t * top_head = (dump_save_t *)DUMP_TOP_HEAD_ADDR;
    dump_area_t * area_head;
    u32 i;

    /* ��ȡAP buffer */
    if((mod_id & 0x0F000000) == 0x01000000)
    {
        /* AP��Ӧarea 0 */
        area_head = (dump_area_t *)((u32)DUMP_TOP_HEAD_ADDR + top_head->area_info[0].offset);
    }
    /* ��ȡCP buffer */
    else if((mod_id & 0x0F000000) == 0x02000000)
    {
        /* CP��Ӧarea 2 */
        area_head = (dump_area_t *)((u32)DUMP_TOP_HEAD_ADDR + top_head->area_info[2].offset);
    }
    /* ��ȡLPM3 buffer */
    else if((mod_id & 0x0F000000) == 0x04000000)
    {
        /* LPM3��Ӧarea 5 */
        area_head = (dump_area_t *)((u32)DUMP_TOP_HEAD_ADDR + top_head->area_info[5].offset);
        if(area_head->head.magic_num != 0x88118811)
        {
            dump_fetal("LPM3 rdr filed is not inited\n");
            return 0;            
        }
    }
    else
    {
        dump_fetal("invalid mod id 0x%x\n", (u32)mod_id);
        return 0;
    }

    /* ����filed id */
    for(i=0; i<area_head->head.field_num; i++)
    {
        if(mod_id == area_head->fields[i].field_id)
        {
            addr = (u32)area_head + area_head->fields[i].offset_addr;
            dump_fetal("field found: offset 0x%x, addr 0x%x\n", area_head->fields[i].offset_addr, addr);
            break;
        }
    }

    return addr;
}

/* ע������ */
u8* bsp_dump_register_field(u32 field_id, u32 length)
{   
    //static u32 next_field_index = DUMP_CP_BUTT-1;
    static u32 next_field_addr  = DUMP_DYNAMIC_ADDR;

    u32 i = 0;
    u32 p_addr = 0;
    dump_area_t* p_area = (dump_area_t*)(DUMP_COMM_SECTION_ADDR);

    int nlock = intLock();

    /* �Ѿ�ע�����������ע��,ֱ�ӷ��ص�ַ */
    for(i=0; i<g_cp_field_num; i++)
    {
        if(p_area->fields[i].field_id == field_id)
        {
            if(p_area->fields[i].length != length)
            {
                logMsg("[dump]: bsp_dump_register_field, field was registered, length is not equal, old: 0x%x, new: 0x%x\n", 
                    p_area->fields[i].length, length, 0, 0, 0, 0);
            }

            intUnlock(nlock);            
            return (u8*)(p_area->fields[i].offset_addr + DUMP_COMM_SECTION_ADDR);
        }
    }

    /* �ж��Ƿ���������� */
    if(((g_cp_field_num+1) >= CP_AREA_FIELD_MAX_NUM)
        || ((next_field_addr+length) > (DUMP_DYNAMIC_ADDR+DUMP_DYNAMIC_SIZE)))
    {
        intUnlock(nlock);
        return BSP_NULL;
    }
    
    p_addr = next_field_addr;
    p_area->fields[g_cp_field_num].field_id    = field_id;
    p_area->fields[g_cp_field_num].offset_addr = p_addr - DUMP_COMM_SECTION_ADDR;
    p_area->fields[g_cp_field_num].length      = length;
    p_area->fields[g_cp_field_num].status      = DUMP_FIELD_USED;
        
    next_field_addr += length;

    g_cp_field_num ++;    
    p_area->head.field_num = g_cp_field_num;

    intUnlock(nlock);

    return (u8*)p_addr;
}

s32 bsp_dump_get_buffer(dump_save_modid_t mod_id, char** buffer, u32* length)
{
    if((!buffer) || (!length))
    {
        dump_error("bsp_dump_get_buffer, invalid param!.buffer=0x%x length=0x%x\n", buffer, length);
		return BSP_ERR_DUMP_INVALID_PARAM;
    }

    switch(mod_id)
    {
        case DUMP_SAVE_MOD_USB:
            *length = (u32)DUMP_EXT_USB_DATA_SIZE;
            break;
        case DUMP_SAVE_MOD_SIM0:
            *length = (u32)DUMP_EXT_SCI0_DATA_SIZE;
            break;
        case DUMP_SAVE_MOD_SIM1:
            *length = (u32)DUMP_EXT_SCI1_DATA_SIZE;
            break;
        case DUMP_SAVE_MOD_OM_MSG:
            *length = (u32)DUMP_EXT_OM_MSG_SIZE;
            break;
        case DUMP_SAVE_MOD_OM_LOG:
            *length = (u32)DUMP_EXT_OM_LOG_SIZE;
            break;
        case DUMP_SAVE_MOD_OSA_APP:
            *length = (u32)DUMP_INTER_OSA_APP_SIZE;
            break;
        case DUMP_SAVE_MOD_OSA_COMM:
            *length = (u32)DUMP_INTER_OSA_COMM_SIZE;
            break;
        case DUMP_SAVE_MOD_UTRACE:
            *length = (u32)DUMP_EXT_OM_UTRACE_DATA_SIZE;
            break;
         case DUMP_SAVE_MOD_DRX_ACORE:
            *length = (u32)DUMP_EXT_OM_DRX_ACORE_SIZE;
            break;
        case DUMP_SAVE_MOD_DRX_CCORE:
            /* �����⴦��һ�£����ܶ�̬���� */
            *buffer = (char*)DUMP_EXT_OM_DRX_CCORE_ADDR;
            *length = (u32)DUMP_EXT_OM_DRX_CCORE_SIZE;
            return BSP_OK;
         case DUMP_SAVE_MOD_OSA_MEM:
            *length = (u32)DUMP_MEMINFO_SIZE;
            break;
        case DUMP_SAVE_MOD_LP_ACORE:
            *length = (u32)DUMP_EXT_LOW_POWER_ACORE_SIZE;
            break;
        case DUMP_SAVE_MOD_LP_CCORE:
            *length = (u32)DUMP_EXT_LOW_POWER_CCORE_SIZE;
            break;	
	    case DUMP_SAVE_MOD_LP_ACORE_BUSERROR:
		   *length = (u32)DUMP_EXT_LOW_POWER_ACORE_BUSERROR_SIZE;
			break;			
	   case DUMP_SAVE_MOD_LP_CCORE_BUSERROR:
		   *length = (u32)DUMP_EXT_LOW_POWER_CCORE_BUSERROR_SIZE;
        	break;
	   case DUMP_SAVE_MOD_LP_MCORE_BUSERROR:
		   *length = (u32)DUMP_EXT_LOW_POWER_MCORE_BUSERROR_SIZE;
	        break;

		/* icc */
		case DUMP_SAVE_MOD_ICC_ACORE:
			*length = (u32)DUMP_EXT_ICC_ACORE_SIZE;
			break;			
		case DUMP_SAVE_MOD_ICC_CCORE:
			*length = (u32)DUMP_EXT_ICC_CCORE_SIZE;
			break;
		case DUMP_SAVE_MOD_ICC_MCORE:
			*length = (u32)DUMP_EXT_ICC_MCORE_SIZE;
			break;
	    case DUMP_SAVE_MOD_BOOST:
	         *length = (u32)DUMP_EXT_BOOST_MCORE_SIZE;
	         break;
        case DUMP_SAVE_MOD_MSP_SLEEP:
            *length = (u32)DUMP_EXT_MSP_SLEEP_SIZE;
             break;
        case DUMP_SAVE_MOD_AMON:
            *length = (u32)DUMP_EXT_AMON_SIZE;
             break;
        case DUMP_SAVE_MOD_TLPS:
             *length = (u32)DUMP_EXT_TLPS_SIZE;
             break;
        default:
            dump_error("bsp_dump_get_buffer, invalid mod id!.mod_id=0x%x\n", mod_id);
            return BSP_ERR_DUMP_INVALID_MODULE;
    }

    *buffer = (char*)bsp_dump_register_field((u32)mod_id, *length);

    if(NULL == (*buffer))
    {
        dump_error("bsp_dump_get_buffer, dump no buffer.");
        return BSP_ERR_DUMP_NO_BUF;
    }

    return BSP_OK;
}

s32 bsp_dump_save_file(dump_save_file_t file_id, char* buffer, u32 length, dump_save_file_mode_t mode)
{
    file_id = file_id;  /* [false alarm]:����Fortify���� */
    buffer = buffer;    /* [false alarm]:����Fortify���� */
    length = length;    /* [false alarm]:����Fortify���� */
    mode = mode;        /* [false alarm]:����Fortify���� */

    return BSP_OK;
}

void bsp_dump_int_lock(bsp_module_e mod_id)
{
    u32 addr = DUMP_INTLOCK_ADDR;

    if(1 == g_dump_cfg.dump_cfg.Bits.intLock)
    {
        (void)QueueLoopIn((dump_queue_t *)addr, (u32)(mod_id<<16));
        (void)QueueLoopIn((dump_queue_t *)addr, om_timer_get());

        g_dump_ptr_info.int_lock_ptr= DUMP_INTLOCK_ADDR +0x10 + (((dump_queue_t *)addr)->rear )*4;


        if(BSP_NULL != g_dump_trace_read_cb[DUMP_SYSVIEW_INTLOCK])
        {
            if( (QueueNumGet((dump_queue_t*)addr)*sizeof(u32)) >= DUMP_INT_LOCK_THRESHOLD_SIZE)
            {
                g_dump_trace_read_cb[DUMP_SYSVIEW_INTLOCK]();
            }
        }
    }
}

void bsp_dump_int_unlock(bsp_module_e mod_id)
{
    u32 addr = DUMP_INTLOCK_ADDR;

    if(1 == g_dump_cfg.dump_cfg.Bits.intLock)
    {
        (void)QueueLoopIn((dump_queue_t *)addr, (((u32)DUMP_INT_UNLOCK_FLAG<<16) |mod_id));
        (void)QueueLoopIn((dump_queue_t *)addr, om_timer_get());

        g_dump_ptr_info.int_lock_ptr= DUMP_INTLOCK_ADDR +0x10 + (((dump_queue_t *)addr)->rear )*4;

        if(BSP_NULL != g_dump_trace_read_cb[DUMP_SYSVIEW_INTLOCK])
        {
            if( (QueueNumGet((dump_queue_t *)addr)*sizeof(u32)) >= DUMP_INT_LOCK_THRESHOLD_SIZE)
            {
                g_dump_trace_read_cb[DUMP_SYSVIEW_INTLOCK]();
            }
        }

    }
}

s32 bsp_dump_get_sysview_addr(dump_sysview_t mod_id, void *addr)
{
    mod_id = mod_id; /* [false alarm]:����Fortify���� */
    addr = addr;     /* [false alarm]:����Fortify���� */

    return BSP_ERROR;
}

s32 bsp_dump_register_sysview_hook(dump_sysview_t mod_id, dump_save_hook func)
{
    if(mod_id >= DUMP_SYSVIEW_BUTT )
    {
        return BSP_ERROR;
    }
    else
    {
        g_dump_trace_read_cb[mod_id] = func;
    }

    return BSP_OK;
}

void bsp_dump_trace_stop(void)
{
    dump_trace_stop();
}

s32 dump_show_map(void)
{
    logMsg("================================CP DUMP AREA MEMORY MAP=========================\n", 0,0,0,0,0,0);
    logMsg("DUMP_HEAD_ADDR:          addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_HEAD_ADDR,             DUMP_HEAD_ADDR-DUMP_COMM_SECTION_ADDR,             DUMP_HEAD_SIZE, 0, 0, 0);
    logMsg("DUMP_FIELD_ADDR:         addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_MAP_ADDR,              DUMP_MAP_ADDR-DUMP_COMM_SECTION_ADDR,              DUMP_MAP_SIZE, 0, 0, 0);
    logMsg("DUMP_BASE_INFO_ADDR:     addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_BASE_INFO_ADDR,        DUMP_BASE_INFO_ADDR-DUMP_COMM_SECTION_ADDR,        DUMP_BASE_INFO_SIZE, 0, 0, 0);
    logMsg("DUMP_TASK_SWITCH_ADDR:   addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_TASK_SWITCH_ADDR,      DUMP_TASK_SWITCH_ADDR-DUMP_COMM_SECTION_ADDR,      DUMP_TASK_SWITCH_SIZE, 0, 0, 0);
    logMsg("DUMP_INTLOCK_ADDR:       addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_INTLOCK_ADDR,          DUMP_INTLOCK_ADDR-DUMP_COMM_SECTION_ADDR,          DUMP_INTLOCK_SIZE, 0, 0, 0);
    logMsg("DUMP_TASK_STACK_ADDR:    addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_TASK_STACK_ADDR,       DUMP_TASK_STACK_ADDR-DUMP_COMM_SECTION_ADDR,       DUMP_TASK_STACK_SIZE, 0, 0, 0);
    logMsg("DUMP_INT_STACK_ADDR:     addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_INT_STACK_ADDR,        DUMP_INT_STACK_ADDR-DUMP_COMM_SECTION_ADDR,        DUMP_INT_STACK_SIZE, 0, 0, 0);
    logMsg("DUMP_ALLTASK_ADDR:       addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_ALLTASK_ADDR,          DUMP_ALLTASK_ADDR-DUMP_COMM_SECTION_ADDR,          DUMP_ALLTASK_SIZE, 0, 0, 0);
    logMsg("DUMP_ALLTASK_TCB_ADDR:   addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_ALLTASK_TCB_ADDR,      DUMP_ALLTASK_TCB_ADDR-DUMP_COMM_SECTION_ADDR,      DUMP_ALLTASK_TCB_SIZE, 0, 0, 0);
    logMsg("DUMP_PRINT_ADDR:         addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_PRINT_ADDR,            DUMP_PRINT_ADDR-DUMP_COMM_SECTION_ADDR,            DUMP_PRINT_SIZE, 0, 0, 0);
    logMsg("DUMP_DMESG_ADDR:         addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_DMESG_ADDR,            DUMP_DMESG_ADDR-DUMP_COMM_SECTION_ADDR,            DUMP_DMESG_SIZE, 0, 0, 0); 
    logMsg("DUMP_REGS_ADDR:          addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_REGS_ADDR,             DUMP_REGS_ADDR-DUMP_COMM_SECTION_ADDR,             DUMP_REGS_SIZE, 0, 0, 0);
    logMsg("DUMP_CPUVIEW_ADDR:       addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_CPUVIEW_ADDR,          DUMP_CPUVIEW_ADDR-DUMP_COMM_SECTION_ADDR,          DUMP_CPUVIEW_SIZE, 0, 0, 0);
    logMsg("DUMP_MEMINFO_ADDR:       addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_MEMINFO_ADDR,          DUMP_MEMINFO_ADDR-DUMP_COMM_SECTION_ADDR,          DUMP_MEMINFO_SIZE, 0, 0, 0);
    logMsg("DUMP_USER_DATA_ADDR:     addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_USER_DATA_ADDR,        DUMP_USER_DATA_ADDR-DUMP_COMM_SECTION_ADDR,        DUMP_USER_DATA_SIZE, 0, 0, 0);   
    logMsg("DUMP_BBE16_TCM_ADDR:     addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_BBE16_TCM_ADDR,        DUMP_BBE16_TCM_ADDR-DUMP_COMM_SECTION_ADDR,        DUMP_BBE16_TCM_SIZE, 0, 0, 0);
    logMsg("DUMP_EXT_OM_DRX_ADDR:    addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_EXT_OM_DRX_CCORE_ADDR, DUMP_EXT_OM_DRX_CCORE_ADDR-DUMP_COMM_SECTION_ADDR, DUMP_EXT_OM_DRX_CCORE_SIZE, 0, 0, 0);
	logMsg("DUMP_TASK_TCB_ADDR:      addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_TASK_TCB_ADDR,         DUMP_TASK_TCB_ADDR-DUMP_COMM_SECTION_ADDR,         DUMP_TASK_TCB_SIZE, 0, 0, 0);
    logMsg("DUMP_RSV_ADDR:           addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_RSV_ADDR,              DUMP_RSV_ADDR-DUMP_COMM_SECTION_ADDR,              DUMP_RSV_SIZE, 0, 0, 0);
    logMsg("DUMP_DYNAMIC_ADDR:       addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_DYNAMIC_ADDR,          DUMP_DYNAMIC_ADDR-DUMP_COMM_SECTION_ADDR,          DUMP_DYNAMIC_SIZE, 0, 0, 0);    
    logMsg("DUMP_CORE_INTERNAL_ADDR: addr=0x%x, offset=0x%x, size=0x%x\n", DUMP_GLOBAL_INTERNAL_ADDR,  0,                                                 DUMP_GLOBAL_INTERNAL_SIZE, 0, 0, 0);
    return 0;
}

/* print cp dump area fields infomation */
s32 dump_show_area()
{
    u32 i = 0;
    u8*  pcontent = NULL;
    dump_area_t* parea = (dump_area_t*)DUMP_COMM_SECTION_ADDR;

    printf("=============================CP DUMP AREA INFO================================\n");

    printf("==Dump area head info:========================================================\n");
    printf("magic number: 0x%X\n", parea->head.magic_num);
    printf("field number: 0x%X\n", parea->head.field_num);
    printf("name        : %s\n",   parea->head.name);
    printf("version     : %s\n",   parea->head.version);
    
    printf("==Dump area fields info:======================================================\n");
    for(i=0; i<parea->head.field_num; i++)
    {
        pcontent = (u8*)parea->data + parea->fields[i].offset_addr;
        
        printf("Index:%3d, field id: 0x%8X, offset addr: 0x%8X, length: 0x%8X, status: 0x%X, ", 
            i, parea->fields[i].field_id, parea->fields[i].offset_addr, parea->fields[i].length, parea->fields[i].status);
        printf("content: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X ...\n", pcontent[0], pcontent[1],
            pcontent[2], pcontent[3], pcontent[4], pcontent[5], pcontent[6], pcontent[7],pcontent[8], pcontent[9],
            pcontent[10], pcontent[11], pcontent[12], pcontent[13], pcontent[14], pcontent[15]);
    }

    return 0;    
}

/* print extension hook list */
s32 dump_show_exthook()
{
    struct list_head* cur = NULL;
    dump_field_hook_t* pfieldhook = NULL;

    logMsg("==============CP Ext Area Hook Function List=============\n",0,0,0,0,0,0);

    list_for_each(cur, &g_dump_hook_list)
    {
        pfieldhook = list_entry(cur, dump_field_hook_t, hook_list);
        
        if(pfieldhook->pfunc)
        {
            logMsg("Field ID=0x%x, Function Addr=0x%x\n", pfieldhook->field_id, (u32)pfieldhook->pfunc,0,0,0,0);
        }
    }

    return 0;
}

void int_lock_hook_add(func_void in_func,func_void out_func)
{
    g_intlock_in_hook = in_func;
    g_intlock_out_hook = out_func;
}

void int_lock_hook(void)
{
    if(g_intlock_in_hook)
    {
        (g_intlock_in_hook)();
    }
}

void int_unlock_hook(void)
{
    if(g_intlock_out_hook)
    {
        (g_intlock_out_hook)();
    }
}

u32 dump_is_deadlock()
{
   u32 regval1;
   //u32 regval2;
   u32 regval3;

   regval1 = readl(HI_SYSCTRL_BASE_ADDR_VIRT+0x624);
   //regval2 = readl(HI_SYSCTRL_BASE_ADDR_VIRT+0x628);
   regval3 = readl(HI_SYSCTRL_BASE_ADDR_VIRT+0x658);

   if(regval1 || regval3)
   {
        return 1;
   }

   return 0;    
}

/* save dsp dtcm to cp dump area in ddr */
void dump_save_dtcm(void)
{ 
    u32 dst = 0;
    u32 src = 0;

    /* ��ʼ��BBE16����Ϊ0xFE */
    memset((void*)DUMP_BBE16_TCM_ADDR, 0xFE, DUMP_BBE16_TCM_SIZE);                                 /* [false alarm]:����Fortify���� */
  
    /* if(!BSP_MailBox_IsDspSleep()) */
	if((1 == bsp_bbe_is_tcm_accessible()) && (0 == dump_is_deadlock()))
    {
        if(HI_BBE16DMEM_SIZE > DUMP_BBE16_TCM_SIZE)
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_DUMP, "%s dump save dtcm fail!\n", __FUNCTION__, 1);
            return;
        }

        memcpy((void*)DUMP_BBE16_TCM_ADDR, (void*)HI_BBE16DMEM_BASE_ADDR, HI_BBE16DMEM_SIZE);        /* [false alarm]:����Fortify���� */

        /* ����ITCMǰ256K, ����ǰ4�ֽ���TCM��־ */
        memcpy((void*)DUMP_BBE16_TCM_ADDR+HI_BBE16DMEM_SIZE, (void*)HI_BBE16IMEM_BASE_ADDR,  SZ_256K),/* [false alarm]:����Fortify���� */
        memcpy((void*)DUMP_BBE16_TCM_ADDR, "_MCT", 4);                                                /* [false alarm]:����Fortify���� */
    }
    else
    {
        /* �͹���ʱ������dsp���� */
        if((LPHY_BBE16_PUB_DTCM_LOAD_SIZE+LPHY_BBE16_MOD_DTCM_LOAD_SIZE+LPHY_BBE16_MOD_DTCM_LOAD_SIZE) > DUMP_BBE16_TCM_SIZE)
        {
            bsp_trace(BSP_LOG_LEVEL_ERROR, BSP_MODU_DUMP, "%s dump save dtcm fail!\n", __FUNCTION__, 2);
            return;
        }        
        
        /* DTCM PUB */
        dst = DUMP_BBE16_TCM_ADDR;
        src = DDR_TLPHY_IMAGE_ADDR;
        memcpy((void*)dst, (void*)src, LPHY_BBE16_PUB_DTCM_LOAD_SIZE);/* [false alarm]:����Fortify���� */

        /* DTCM LTE */
        dst = DUMP_BBE16_TCM_ADDR  + LPHY_BBE16_PUB_DTCM_LOAD_SIZE;
        src = DDR_TLPHY_IMAGE_ADDR + LPHY_BBE16_PUB_DTCM_LOAD_SIZE + LPHY_BBE16_PUB_ITCM_LOAD_SIZE;
        memcpy((void*)dst, (void*)src,  LPHY_BBE16_MOD_DTCM_LOAD_SIZE);/* [false alarm]:����Fortify���� */
        
        /* DTCM TDS */
        dst = DUMP_BBE16_TCM_ADDR  + LPHY_BBE16_PUB_DTCM_LOAD_SIZE + LPHY_BBE16_MOD_DTCM_LOAD_SIZE;
        src = DDR_TLPHY_IMAGE_ADDR + LPHY_BBE16_PUB_DTCM_LOAD_SIZE + LPHY_BBE16_PUB_ITCM_LOAD_SIZE + LPHY_BBE16_MOD_DTCM_LOAD_SIZE + LPHY_BBE16_MOD_ITCM_LOAD_SIZE;
        memcpy((void*)dst, (void*)src, LPHY_BBE16_MOD_DTCM_LOAD_SIZE);/* [false alarm]:����Fortify���� */
    }
}

/*lint -restore */

