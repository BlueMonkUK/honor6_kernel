

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#ifndef __CPU_C_H__
#define __CPU_C_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define MAX_PID_USED 3
#define OS_PID_MASK  (0xf00)


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
/*****************************************************************************
 ʵ������  : OS_HOOK_TYPE_ENUM
 ��������  : ���Ӻ�������ö�ٶ���
*****************************************************************************/
enum OS_HOOK_TYPE_ENUM
{
    OS_HOOK_TYPE_ENTER_INTR=0,
    OS_HOOK_TYPE_EXIT_INTR,
    OS_HOOK_TYPE_TASK_SWITCH,
    OS_HOOK_TYPE_MSG_GET,
    OS_HOOK_TYPE_NMI,
    OS_HOOK_TYPE_EXCEPTION,
    OS_HOOK_TYPE_APPINIT,
    OS_HOOK_TYPE_IDLE,
    OS_HOOK_TYPE_BUTT
};
typedef VOS_UINT16 OS_HOOK_TYPE_ENUM_UINT16;

/*****************************************************************************
 ʵ������  : OS_INTR_CONNECT_ENUM
 ��������  : �жϴ����Ӻ�������ö�ٶ���
*****************************************************************************/
enum OS_INTR_CONNECT_TYPE_ENUM
{
    OS_INTR_CONNECT_00=0,        /*Int 0   type / priority level NMI / 6          */
    OS_INTR_CONNECT_01,          /*Int 1   type / priority level Software / 3  */
    OS_INTR_CONNECT_02,          /*Int 2   type / priority level ExtLevel / 2  */
    OS_INTR_CONNECT_03,          /*Int 3   type / priority level ExtLevel / 2  */
    OS_INTR_CONNECT_04,          /*Int 4   type / priority level ExtLevel / 2  */
    OS_INTR_CONNECT_05,          /*Int 5   type / priority level Timer / 3     */
    OS_INTR_CONNECT_06,          /*Int 6   type / priority level Timer / 4     */
    OS_INTR_CONNECT_07,          /*Int 7   type / priority level ExtLevel / 3  */
    OS_INTR_CONNECT_08,          /*Int 8   type / priority level ExtLevel / 3  */
    OS_INTR_CONNECT_09,          /*Int 9   type / priority level ExtLevel / 3  */
    OS_INTR_CONNECT_10,          /*Int 10   type / priority level ExtLevel / 2 */
    OS_INTR_CONNECT_11,          /*Int 11   type / priority level ExtLevel / 2 */
    OS_INTR_CONNECT_12,          /*Int 12   type / priority level ExtLevel / 2 */
    OS_INTR_CONNECT_13,          /*Int 13   type / priority level ExtLevel / 2 */
    OS_INTR_CONNECT_14,          /*Int 14   type / priority level ExtLevel / 2 */
    OS_INTR_CONNECT_15,          /*Int 15   type / priority level ExtLevel / 1 */
    OS_INTR_CONNECT_16,          /*Int 16   type / priority level ExtLevel / 1 */
    OS_INTR_CONNECT_17,          /*Int 17   type / priority level ExtLevel / 1 */
    OS_INTR_CONNECT_18,          /*Int 18   type / priority level ExtLevel / 1 */
    OS_INTR_CONNECT_19,          /*Int 19   type / priority level Timer / 2    */
    OS_INTR_CONNECT_20,          /*Int 20   type / priority level ExtLevel / 1 */
    OS_INTR_CONNECT_21,          /*Int 21   type / priority level ExtLevel / 1 */
    OS_INTR_CONNECT_22,          /*Int 22   type / priority level ExtLevel / 1 */
    OS_INTR_CONNECT_23,          /*Int 23   type / priority level ExtLevel / 1 */
    OS_INTR_CONNECT_24,          /*Int 24   type / priority level ExtLevel / 1 */
    OS_INTR_CONNECT_25,          /*Int 25   type / priority level ExtLevel / 1 */
    OS_INTR_CONNECT_26,          /*Int 26   type / priority level ExtLevel / 1 */
    OS_INTR_CONNECT_27,          /*Int 27   type / priority level ExtLevel / 1 */
    OS_INTR_CONNECT_28,          /*Int 28   type / priority level ExtLevel / 1 */
    OS_INTR_CONNECT_29,          /*Int 29   type / priority level ExtLevel / 1 */
    OS_INTR_CONNECT_30,          /*Int 30   type / priority level ExtLevel / 1 */
    OS_INTR_CONNECT_31,          /*Int 31   type / priority level ExtEdge / 1  */
    OS_INTR_CONNECT_BUTT
};
typedef VOS_UINT16 OS_INTR_CONNECT_TYPE_ENUM_UINT16;


#define SET_STKREG(r,v) sp[(r) >> 2] = (VOS_STK)(v)

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/


/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/
/*****************************************************************************
 ʵ������  : HOOK_FUN_TYPE
 ��������  : ���Ӻ������Ͷ���
*****************************************************************************/
typedef VOS_VOID (*HOOK_FUN_TYPE)(VOS_VOID);

/*****************************************************************************
 ʵ������  : VOS_IDLE_HOOK_FUN_TYPE
 ��������  : IDLE�����Ӻ������Ͷ���
*****************************************************************************/
typedef VOS_VOID (*VOS_IDLE_HOOK_FUN_TYPE)(VOS_UINT32 ulVosCtxSw);

/*****************************************************************************
 ʵ������  : INTR_HOOK_FUN_TYPE
 ��������  : �жϽ���/�˳����Ӻ������Ͷ���
*****************************************************************************/
typedef VOS_VOID  (*INTR_HOOK_FUN_TYPE )(VOS_UINT32 uwIntNo);

/*****************************************************************************
 ʵ������  : VOS_TASK_SWITCH_HOOK
 ��������  : �����л����Ӻ������Ͷ���
*****************************************************************************/
typedef VOS_VOID (*VOS_TASK_SWITCH_HOOK)( VOS_VOID *tcb_old, VOS_VOID *tcb_new);

/*****************************************************************************
 ʵ������  : VOS_MSG_HOOK_FUNC
 ��������  : ��Ϣ��ȡ���Ӻ������Ͷ���
*****************************************************************************/
typedef VOS_VOID (*VOS_MSG_HOOK_FUNC)( VOS_VOID *pMsg);

/*****************************************************************************
 ʵ������  : VOS_EXCEPTION_HOOK_FUNC
 ��������  : �쳣�����Ӻ������Ͷ���
*****************************************************************************/
typedef VOS_VOID (*VOS_EXCEPTION_HOOK_FUNC)( VOS_UINT32 uwExceptionNo);




/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern VOS_VOID *g_pfVosHookFuncTable[OS_HOOK_TYPE_BUTT];

/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_VOID VOS_RegisterEnterIntrHook(INTR_HOOK_FUN_TYPE  pfnEnterIntrHook);
extern VOS_VOID VOS_RegisterExitIntrHook(INTR_HOOK_FUN_TYPE  pfnExitIntrHook);
extern VOS_VOID VOS_RegTaskSwitchHook(VOS_TASK_SWITCH_HOOK pfnTaskSwitchHook);
extern VOS_VOID VOS_RegisterMsgGetHook(VOS_MSG_HOOK_FUNC pfnMsgGetHook);
extern VOS_VOID VOS_ConnectInterrupt(VOS_UINT32 uwIntrNo, HOOK_FUN_TYPE pfnInterruptHook);
extern VOS_VOID VOS_RegisterExceptionHandler(VOS_EXCEPTION_HOOK_FUNC pfnExceptionHook);
extern VOS_VOID VOS_RegisterNMIHook(HOOK_FUN_TYPE  pfnNmiHook);
extern VOS_VOID VOS_EnableInterrupt(VOS_UINT32 uwIntNo);
extern VOS_VOID VOS_DisableInterrupt(VOS_UINT32 uwIntNo);
extern VOS_VOID VOS_RegAppInitFuncHook(HOOK_FUN_TYPE  pfnAppInitFuncHook);
extern VOS_VOID VOS_SoftIntTriger(VOS_UINT32 uwInfo);
extern VOS_VOID VOS_RegIdleHook(VOS_IDLE_HOOK_FUN_TYPE pfnIdleHook);

extern VOS_VOID  OS_UserExit(VOS_VOID);
extern VOS_VOID VOS_SetInterruptMask(VOS_UINT32 uwBitEnable);
extern VOS_UINT32 VOS_GetInterruptMask(VOS_VOID);
extern VOS_UINT32 VOS_GetInterrupt( VOS_VOID );


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of cpu_c.h */
