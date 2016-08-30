

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#if (VOS_CPU_TYPE ==  VOS_HIFI)
#include <xtensa/config/specreg.h>
#include <xtensa/config/core.h>
#include <xtensa/simcall.h>
#include "arch_hifi330.h"
#endif
#include "vos.h"
#include "cpu_c.h"

/*for test*/
#include "venus_stub.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/*���Ӻ����б�*/
VOS_VOID *g_pfVosHookFuncTable[OS_HOOK_TYPE_BUTT];

/*�жϴ����Ӻ����б�*/
VOS_VOID *g_pfVosIntrFuncTable[OS_INTR_CONNECT_BUTT];

/*���ڴ������жϵ���Ϣ���������жϵ�ʱ��д���������жϵ�ʱ���*/
VOS_UINT32 g_uwSoftIntInfo = 0;

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

#if (VOS_CPU_TYPE ==  VOS_HIFI)


VOS_STK *VOS_TaskStkInit(VOS_VOID (*task)(VOS_UINT32 d), VOS_UINT32 data,
                        VOS_STK *ptos, VOS_UINT16 opt)
{
    /*
    1) ����ջ��λ��,��Ҫ16�ֽڶ���
    sp = (OS_STK*) (((INT32U)(ptos+1) - XT_CP_SIZE - XT_STK_FRMSZ) & ~0xf);

    2) ��ջ����ջ��ȫ����0;

    3) ��ʼ��PC,A0,A1,EXIT,A6,PS��ջ�ж�Ӧλ�õ�ֵ����:
    PC=task;
    A0=0;
    A1=(VOS_UINT32)sp+XT_STK_FRMSZ;
    EXIT=_xt_user_exit;
    A6=data;
    PS=PS_UM | PS_EXCM | PS_WOE | PS_CALLINC(1);

    4) ����ջ��sp;
    */
    VOS_STK *sp, *tp;

    /* Create interrupt stack frame aligned to 16 byte boundary */
    sp = (VOS_STK*) (((VOS_UINT32)(ptos + 1) - XT_CP_SIZE - XT_STK_FRMSZ) & (~0xf) );

    /* Clear the entire frame (do not use memset() because we don't depend on C library) */
    for (tp = sp; tp <= ptos; ++tp)
    {
        *tp = 0;
    }

    /* Explicitly initialize certain saved registers */
    SET_STKREG( XT_STK_PC,      task                        );  /* task entrypoint                  */
    SET_STKREG( XT_STK_A0,      0                           );  /* to terminate GDB backtrace       */
    SET_STKREG( XT_STK_A1,      (VOS_UINT32)sp + XT_STK_FRMSZ   );  /* physical top of stack frame      */
    SET_STKREG( XT_STK_EXIT,    OS_UserExit               );  /* user exception exit dispatcher   */
    SET_STKREG( XT_STK_A6,      (VOS_UINT32)data   );  /* physical top of stack frame      */
    /* Set initial PS to int level 0, EXCM disabled ('rfe' will enable), user mode. */
    /* + for windowed ABI also set WOE and CALLINC (pretend task was 'call4'd). */
    SET_STKREG( XT_STK_PS,      PS_UM | PS_EXCM | PS_WOE | PS_CALLINC(1) );

    return sp;
}

#endif


VOS_VOID OS_TaskSwHook(VOS_VOID)
{
    VOS_TASK_SWITCH_HOOK pfTaskSwHook = (VOS_TASK_SWITCH_HOOK)g_pfVosHookFuncTable[OS_HOOK_TYPE_TASK_SWITCH];

    if (VOS_NULL != pfTaskSwHook)
    {
        pfTaskSwHook(g_pstVosTCBCur,g_pstVosTCBHighRdy);
    }
}


VOS_VOID OS_EnterIntHook(VOS_UINT32 uhwIntrNo)
{
    INTR_HOOK_FUN_TYPE pfIntrHook = (INTR_HOOK_FUN_TYPE)g_pfVosHookFuncTable[OS_HOOK_TYPE_ENTER_INTR];

    if (VOS_NULL != pfIntrHook)
    {
        pfIntrHook(uhwIntrNo);
    }
}



VOS_VOID OS_ExitIntHook(VOS_UINT32 uhwIntrNo)
{
    INTR_HOOK_FUN_TYPE pfIntrHook = (INTR_HOOK_FUN_TYPE)g_pfVosHookFuncTable[OS_HOOK_TYPE_EXIT_INTR];

    if (VOS_NULL != pfIntrHook)
    {
        pfIntrHook(uhwIntrNo);
    }
}


VOS_VOID OS_UserexecHandler(VOS_UINT32 uwExecCauseNo)
{
    VOS_EXCEPTION_HOOK_FUNC pfIntrHook = (VOS_EXCEPTION_HOOK_FUNC)g_pfVosHookFuncTable[OS_HOOK_TYPE_EXCEPTION];
    VOS_DEBUG_LOG_EXCEPTION(uwExecCauseNo);
    if (VOS_NULL != pfIntrHook)
    {
        pfIntrHook(uwExecCauseNo);
    }
    else
    {
        extern VOS_VOID OS_Panic(VOS_VOID);
        OS_Panic();
    }
}




VOS_VOID OS_InterruptHandler(VOS_UINT32 uhwIntrNo)
{
    HOOK_FUN_TYPE pfIntrHook = (HOOK_FUN_TYPE)g_pfVosIntrFuncTable[uhwIntrNo];

    VOS_DEBUG_LOG_CPU_INFO(0xf0000000 + uhwIntrNo);

    if (VOS_NULL != pfIntrHook)
    {
        pfIntrHook();
    }
}




VOS_VOID OS_NmiHook(VOS_VOID)
{
    HOOK_FUN_TYPE pfNmiHook = (HOOK_FUN_TYPE)g_pfVosHookFuncTable[OS_HOOK_TYPE_NMI];

    if (VOS_NULL != pfNmiHook)
    {
        pfNmiHook();
    }
}

VOS_VOID VOS_RegisterEnterIntrHook(INTR_HOOK_FUN_TYPE  pfnEnterIntrHook)
{
    g_pfVosHookFuncTable[OS_HOOK_TYPE_ENTER_INTR] = (VOS_VOID *)pfnEnterIntrHook;
}



VOS_VOID VOS_RegisterExitIntrHook(INTR_HOOK_FUN_TYPE  pfnExitIntrHook)
{
    g_pfVosHookFuncTable[OS_HOOK_TYPE_EXIT_INTR] = (VOS_VOID *)pfnExitIntrHook;
}




VOS_VOID VOS_RegTaskSwitchHook(VOS_TASK_SWITCH_HOOK pfnTaskSwitchHook)
{
    g_pfVosHookFuncTable[OS_HOOK_TYPE_TASK_SWITCH] = (VOS_VOID *)pfnTaskSwitchHook;
}



VOS_VOID VOS_RegisterMsgGetHook(VOS_MSG_HOOK_FUNC pfnMsgGetHook)
{
    g_pfVosHookFuncTable[OS_HOOK_TYPE_MSG_GET] = (VOS_VOID *)pfnMsgGetHook;
}



VOS_VOID VOS_ConnectInterrupt(VOS_UINT32 uwIntrNo, HOOK_FUN_TYPE pfnInterruptHook)
{
    g_pfVosIntrFuncTable[uwIntrNo] = (VOS_VOID *)pfnInterruptHook;
}



VOS_VOID VOS_RegisterExceptionHandler(VOS_EXCEPTION_HOOK_FUNC pfnExceptionHook)
{
     g_pfVosHookFuncTable[OS_HOOK_TYPE_EXCEPTION] = (VOS_VOID *)pfnExceptionHook;
}


VOS_VOID VOS_RegisterNMIHook(HOOK_FUN_TYPE  pfnNmiHook)
{
    g_pfVosHookFuncTable[OS_HOOK_TYPE_NMI] = (VOS_VOID *)pfnNmiHook;
}




VOS_VOID VOS_EnableInterrupt(VOS_UINT32 uwIntNo)
{
    VOS_UINT32 uwBitEnable;
    /*
    ��uwIntNo��Ӧ��intenableλΪ1
    */
    uwBitEnable = xthal_get_intenable();
    xthal_set_intenable(uwBitEnable | (((VOS_UINT32)1) << uwIntNo ));

}



VOS_VOID VOS_DisableInterrupt(VOS_UINT32 uwIntNo)
{
    VOS_UINT32 uwBitEnable;
    /*
    ��uwIntNo��Ӧ��intenableλΪ0
    */

    uwBitEnable = xthal_get_intenable();
    xthal_set_intenable(uwBitEnable & (~(((VOS_UINT32)1) << uwIntNo )));
}


VOS_VOID VOS_SetInterruptMask(VOS_UINT32 uwBitEnable)
{
    xthal_set_intenable(uwBitEnable);
}


VOS_UINT32 VOS_GetInterruptMask(VOS_VOID)
{
    VOS_UINT32 uwBitEnable;

    uwBitEnable = xthal_get_intenable();

    return uwBitEnable;
}


VOS_UINT32 VOS_GetInterrupt( VOS_VOID )
{
    VOS_UINT32 uwIntStatus;

    uwIntStatus = xthal_get_interrupt();

    return uwIntStatus;
}



VOS_VOID VOS_RegAppInitFuncHook(HOOK_FUN_TYPE  pfnAppInitFuncHook)
{
    g_pfVosHookFuncTable[OS_HOOK_TYPE_APPINIT] = (VOS_VOID *)pfnAppInitFuncHook;
}




VOS_VOID VOS_SoftIntTriger(VOS_UINT32 uwInfo)
{
    /*
    ����uwInfo���ض���ַ
    �������ж�(�жϺ�1)
    */
    g_uwSoftIntInfo = uwInfo;
    xthal_set_intset((((VOS_UINT32)1) << OS_INTR_CONNECT_01 ));
}


VOS_VOID VOS_RegIdleHook(VOS_IDLE_HOOK_FUN_TYPE pfnIdleHook)
{
    g_pfVosHookFuncTable[OS_HOOK_TYPE_IDLE] = (VOS_VOID *)pfnIdleHook;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

