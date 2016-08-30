

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "med_drv_interface.h"


#ifndef __DRV_IPC_H__
#define __DRV_IPC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define DRV_IPC_MAX_INT_NUM         (32)                                        /*  */
#define DRV_IPC_INT_NO_HIFI         (OS_INTR_CONNECT_02)                        /* IPC�����ж�����HIFI���жϺ�(�����ж���2���ź����ж���3��ֻ��Ҫʹ�ÿ����ж�) */

#define DRV_IPC_CPU_RAW_INT(i)      (DRV_IPC_BASE_ADDR+(0x400+(i*0x10)))       /* CPU_i�����ж�ԭʼ�жϼĴ���     */
#define DRV_IPC_CPU_INT_MASK(i)     (DRV_IPC_BASE_ADDR+(0x404+(i*0x10)))       /* CPU_i�����ж�����Ĵ���         */
#define DRV_IPC_CPU_INT_STAT(i)     (DRV_IPC_BASE_ADDR+(0x408+(i*0x10)))       /* CPU_i���κ�Ŀ����ж�״̬�Ĵ��� */
#define DRV_IPC_CPU_INT_CLR(i)      (DRV_IPC_BASE_ADDR+(0x40C+(i*0x10)))       /* CPU_i�����ж�����Ĵ���         */
#define DRV_IPC_SEM_RAW_INT(j)      (DRV_IPC_BASE_ADDR+(0x600+(j*0x10)))       /* CPU_j�ź����ͷ�ԭʼ�жϼĴ���   */
#define DRV_IPC_SEM_INT_MASK(j)     (DRV_IPC_BASE_ADDR+(0x604+(j*0x10)))       /* CPU_j�ź����ͷ��ж�����Ĵ���   */
#define DRV_IPC_SEM_INT_STAT(j)     (DRV_IPC_BASE_ADDR+(0x608+(j*0x10)))       /* CPU_j�ź����ͷ��ж�״̬�Ĵ���   */
#define DRV_IPC_SEM_INT_CLR(j)      (DRV_IPC_BASE_ADDR+(0x60C+(j*0x10)))       /* CPU_j�ź����ͷ��ж�����Ĵ���   */
#define DRV_IPC_HS_CTRL(j,k)        (DRV_IPC_BASE_ADDR+(0x800+(j*0x100)+(k*0x8))) /* CPU_j�ź���k����Ĵ���      */
#define DRV_IPC_HS_STAT(j,k)        (DRV_IPC_BASE_ADDR+(0x804+(j*0x100)+(k*0x8))) /* CPU_j�ź���k״̬�Ĵ���      */

#define DRV_IPC_CPU_RAW_INT_HIFI    (DRV_IPC_CPU_RAW_INT(IPC_CORE_HiFi))        /* HIFI�����ж�ԭʼ�жϼĴ���     */
#define DRV_IPC_CPU_INT_ENABLE_HIFI (DRV_IPC_CPU_INT_MASK(IPC_CORE_HiFi))       /* HIFI�����ж�����Ĵ���         */
#define DRV_IPC_CPU_INT_STAT_HIFI   (DRV_IPC_CPU_INT_STAT(IPC_CORE_HiFi))       /* HIFI���κ�Ŀ����ж�״̬�Ĵ��� */
#define DRV_IPC_CPU_INT_CLR_HIFI    (DRV_IPC_CPU_INT_CLR(IPC_CORE_HiFi))        /* HIFI�����ж�����Ĵ���         */

#define DRV_IPC_GetIntSrcIsrPtr()   (&g_astDrvIpcIntSrcIsr[0])                  /* ��ȡIPC�жϴ�����ȫ�ֱ����׵�ַ */

#define DRV_IPC_MCU_CORE            (IPC_CORE_MCORE)

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


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
 �� �� ��  : DRV_IPC_INT_FUNC
 ��������  : IPC�����жϴ���ص�����
 �������  : VOS_UINT32 uwPara - �ص�ʱ����ע��ʱ�ṩ�Ĳ���
 �������  : ��
 �� �� ֵ  : VOS_VOID
*****************************************************************************/
typedef VOS_VOID (*DRV_IPC_INT_FUNC)( VOS_UINT32 uwPara);

/*****************************************************************************
 ʵ �� ��  : DRV_IPC_INT_SRC_ISR_STRU
 ��������  : �����ж���Ӧ����ĺ���ָ��ͻص�����
*****************************************************************************/
typedef struct
{
    DRV_IPC_INT_FUNC    pfFunc;         /* �ص�����ָ�� */
    VOS_UINT32          uwPara;         /* �ص��������� */
} DRV_IPC_INT_SRC_ISR_STRU;

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  10 ��������
*****************************************************************************/

extern VOS_VOID DRV_IPC_Init( VOS_VOID );
extern VOS_VOID DRV_IPC_Isr( VOS_VOID );
extern VOS_VOID DRV_IPC_RegIntSrc(
                       IPC_INT_LEV_E                   enSrc,
                       DRV_IPC_INT_FUNC                pfFunc,
                       VOS_UINT32                      uwPara);
extern VOS_VOID DRV_IPC_TrigInt(
                       VOS_UINT16                  enTarget,
                       VOS_UINT16                  enIntSrc);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of med_drv_ipc.h */

