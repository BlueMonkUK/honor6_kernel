

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "med_drv_interface.h"
#include "soc_timer_interface.h"

#ifndef __DRV_TIMER_HIFI_H__
#define __DRV_TIMER_HIFI_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define DRV_TIMER_OM_ADDR               DRV_TIMER_SC_SLICE_ADDR                 /* ϵͳ Slice���������ڵĵ�ַ */
#define DRV_TIMER_OM_FREQ               DRV_TIMER_SC_SLICE_FREQ

#define DRV_TIMER_UNLINK_TIMER_ADDR     DRV_TIMER_UNLINK_HIFI_ADDR              /* PC VOICEʹ��TIMER8 */
#define DRV_TIMER_UNLINK_TIMER_IDX      (VOS_NULL)                              /* �ò���V7R2�в�ʹ�� */
#define DRV_TIMER_UNLINK_TIMER_FREQ     DRV_TIMER_UNLINK_HIFI_FREQ              /* ���㷽��:��ʱ��Ƶ��(Hz)/10000 */
#define DRV_TIMER_UNLINK_MAX_NUM        (1)                                     /* ������ʱ����Ŀ       */
#define DRV_TIMER_UNLINK_MAX_TIMEOUT    (1000000)                               /* ������ʱ��֧�ֵ����ʱʱ������λ0.1ms, =(10000*2P32)/(DRV_TIMER_UNLINK_TIMER_FREQ*2) */

#define DRV_TIMER_LINKED_TIMER_ADDR     DRV_TIMER_DWAPB_HIFI_ADDR               /* ��ʽ��ʱ��ʹ��TIMER0 */
#define DRV_TIMER_LINKED_TIMER_IDX      (VOS_NULL)                              /* �ò���V7R2�в�ʹ�� */
#define DRV_TIMER_LINKED_TIMER_FREQ     DRV_TIMER_DWAPB_HIFI_FREQ               /* HIFI����ʱ�ӣ����㷽��:��ʱ��Ƶ��(Hz)/10000 */
#define DRV_TIMER_LINKED_MAX_NUM        (9)                                     /* ֧�ֵ����Ӷ�ʱ����Ŀ */
#define DRV_TIMER_LINKED_MAX_TIMEOUT    (1000000)                               /* ���Ӷ�ʱ��֧�ֵ����ʱʱ������λ0.1ms, =(10000*2P32)/(266000000*2) */

#define DRV_TIMER_MAX_NUM               (DRV_TIMER_LINKED_MAX_NUM       \
                                         + DRV_TIMER_UNLINK_MAX_NUM)            /* ���֧�ֶ�ʱ����Ŀ */

#define DRV_TIMER_PERIPH_CLK_EN_ADDR SOC_AO_SCTRL_SC_PERIPH_CLKEN4_ADDR(VOS_NULL)

#define DRV_TIMER_CONTROL(base, i)       \
                SOC_TIMER_TIMERN_CONTROL_ADDR(base, i)                          /* ��ʱ��i���ƼĴ���         */
#define DRV_TIMER_EOI(base, i)           \
                SOC_TIMER_TIMERN_INTCLR_ADDR(base, i)                           /* ��ʱ��i���жϼĴ���       */
#define DRV_TIMER_INTSTATUS(base, i)     \
                SOC_TIMER_TIMERN_RIS_ADDR(base, i)                              /* ��ʱ��i�ж�״̬�Ĵ���     */
#define DRV_TIMER_LOADCOUNT(base, i)     \
                SOC_TIMER_TIMERN_LOAD_ADDR(base, i)                             /* ��ʱ��i��ʼֵ��32λ�Ĵ��� */
#define DRV_TIMER_CURRENTVALUE(base, i)  \
                SOC_TIMER_TIMERN_VALUE_ADDR(base, i)                            /* ��ʱ��i��ǰֵ��32λ�Ĵ��� */

#define DRV_TIMER_GetTimerCtrlObjPtr()  (&g_stDrvTimerCtrlObj)                  /* ��ȡTimer����ȫ�ֽṹָ�� */

#define DRV_TIMER_StartLinkedTimer(uwLoadCnt)                         \
        DRV_TIMER_StartDwApbTimer(DRV_TIMER_LINKED_TIMER_ADDR,    \
                                  DRV_TIMER_LINKED_TIMER_IDX,     \
                                  uwLoadCnt,                          \
                                  DRV_TIMER_MODE_ONESHOT)
#define DRV_TIMER_StopLinkedTimer()                                   \
        DRV_TIMER_StopDwApbTimer(DRV_TIMER_LINKED_TIMER_ADDR,     \
                                 DRV_TIMER_LINKED_TIMER_IDX)
#define DRV_TIMER_ReadLinkedTimer()                                   \
        DRV_TIMER_ReadDwApbTimer(DRV_TIMER_LINKED_TIMER_ADDR,     \
                                 DRV_TIMER_LINKED_TIMER_IDX,      \
                                 VOS_NULL,                            \
                                 VOS_NULL)


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
 ʵ������  : DRV_TIMER_MODE_ENUM
 ��������  : ��ʱ��ģʽö�ٶ���
*****************************************************************************/
enum DRV_TIMER_MODE_ENUM
{
    DRV_TIMER_MODE_ONESHOT              = 0,                                    /* һ���Զ�ʱ�� */
    DRV_TIMER_MODE_PERIODIC             = 1,                                    /* �����Զ�ʱ�� */
    DRV_TIMER_MODE_BUTT
};
typedef VOS_UINT16 DRV_TIMER_MODE_ENUM_UINT16;

/*****************************************************************************
 ʵ������  : DRV_TIMER_DEVICE_ENUM
 ��������  : HIFI���ö�ʱ���豸��־ö�ٶ���
*****************************************************************************/
enum DRV_TIMER_DEVICE_ENUM
{
    DRV_TIMER_DEVICE_DUALTIMER5_0       = 0,                                    /* DUALTimer��ʱ��5_0 */
    DRV_TIMER_DEVICE_DUALTIMER5_1,                                              /* DUALTimer��ʱ��5_1 */
    DRV_TIMER_DEVICE_BUTT
};
typedef VOS_UINT32 DRV_TIMER_DEVICE_ENUM_UINT32;

/*****************************************************************************
 ʵ������  : DRV_TIMER_DEVICE_STATUS_ENUM
 ��������  : ��ʱ���豸״̬ö�ٶ���
*****************************************************************************/
enum DRV_TIMER_DEVICE_STATUS_ENUM
{
    DRV_TIMER_DEVICE_STATUS_IDLE          = 0,                                  /* ��ʱ��ֹͣ */
    DRV_TIMER_DEVICE_STATUS_RUNNING,                                            /* ��ʱ������ */
    DRV_TIMER_DEVICE_STATUS_BUTT
};
typedef VOS_UINT32 DRV_TIMER_DEVICE_STATUS_ENUM_UINT32;

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
 �� �� ��  : DRV_TIMER_INT_FUNC
 ��������  : ��ʱ���жϴ���ص�����
 �������  : uwTimer   - ��ʱ�����
             uwPara    - �û�����
 �������  : ��
 �� �� ֵ  : VOS_VOID
*****************************************************************************/
typedef VOS_VOID (*DRV_TIMER_INT_FUNC)(VOS_UINT32 uwTimer, VOS_UINT32 uwPara);

/*****************************************************************************
 ʵ������  : DRV_TIMER_CTRL_BLOCK_STRU
 ��������  : ��ʱ�����ƿ�
*****************************************************************************/
typedef struct _DRV_TIMER_CTRL_BLOCK_TAG
{
    VOS_UINT16                          uhwUsedFlag;                            /* 0-δʹ�á�1-ʹ�� */
    DRV_TIMER_MODE_ENUM_UINT16          enMode;                                 /* ��ʱ��ģʽ       */
    VOS_UINT32                          uwLength;                               /* ��ʱ��ʱ��,0.1ms */
    VOS_UINT32                          uwLoadCnt;                              /* ��ʱ������ֵ     */
    VOS_UINT32                          uwPara;                                 /* ��ʱ�ص�����     */
    DRV_TIMER_INT_FUNC                  pfFunc;                                 /* ��ʱ�ص�����     */
    DRV_TIMER_DEVICE_ENUM_UINT32        uwDevice;                               /* ����ʱ����־   */
    struct _DRV_TIMER_CTRL_BLOCK_TAG   *pstNext;
    struct _DRV_TIMER_CTRL_BLOCK_TAG   *pstPrevious;
}DRV_TIMER_CTRL_BLOCK_STRU;

/*****************************************************************************
 ʵ������  : DRV_TIMER_CTRL_BLOCK_STRU
 ��������  : ��ʱ������ʵ��
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          uwIdleNum;                              /* δʹ�õĿ��ƿ��� */
    VOS_UINT16                          uhwLinkedNum;                           /* ����ʱ������   */
    VOS_UINT16                          uhwUnlinkNum;                           /* ������ʱ������   */
    DRV_TIMER_CTRL_BLOCK_STRU          *pstIdleBlk;                             /* ���п��ƿ�,������ʽ��ʱ��ʹ�� */
    DRV_TIMER_CTRL_BLOCK_STRU          *pstLinkedBlk;                           /* ����ʱ�����ƿ� */
    DRV_TIMER_CTRL_BLOCK_STRU           astUnlinkBlk[DRV_TIMER_UNLINK_MAX_NUM]; /* ������ʱ�����ƿ� */
    DRV_TIMER_CTRL_BLOCK_STRU           astLinkedBlk[DRV_TIMER_LINKED_MAX_NUM]; /* ��ʱ�����ƿ黺�� */
}DRV_TIMER_CTRL_OBJ_STRU;

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
extern VOS_VOID DRV_TIMER_AddToLink(
                       DRV_TIMER_CTRL_BLOCK_STRU                  *pstTimer,
                       DRV_TIMER_CTRL_OBJ_STRU                    *pstTimers);
extern VOS_UINT32 DRV_TIMER_CheckLinkedBlk(
                       DRV_TIMER_CTRL_BLOCK_STRU                  *pstTimer,
                       DRV_TIMER_CTRL_BLOCK_STRU                  *pstTimerLst);
extern VOS_VOID DRV_TIMER_DelFromLink(
                       DRV_TIMER_CTRL_BLOCK_STRU                  *pstTimer,
                       DRV_TIMER_CTRL_OBJ_STRU                    *pstTimers);
extern VOS_VOID DRV_TIMER_FreeLinkedBlk(
                       DRV_TIMER_CTRL_BLOCK_STRU                  *pstTimer,
                       DRV_TIMER_CTRL_OBJ_STRU                    *pstTimers);
extern DRV_TIMER_CTRL_BLOCK_STRU *DRV_TIMER_GetLinkedBlk(DRV_TIMER_CTRL_OBJ_STRU *pstTimers);
extern VOS_UINT32 DRV_TIMER_GetOmFreq(VOS_VOID);
extern DRV_TIMER_CTRL_BLOCK_STRU *DRV_TIMER_GetUnlinkBlk(DRV_TIMER_CTRL_OBJ_STRU *pstTimers);
extern VOS_VOID DRV_TIMER_Init(VOS_VOID);
extern VOS_VOID DRV_TIMER_IsrUnLinkedTimer(VOS_VOID);
extern VOS_VOID DRV_TIMER_IsrLinkedTimer(VOS_VOID);
extern VOS_UINT32 DRV_TIMER_ReadDwApbTimer(
                       VOS_UINT32              uwBaseAddr,
                       VOS_UINT32              uwTimerIndx,
                       VOS_UINT32             *puwCntLow,
                       VOS_UINT32             *puwCntHigh);
extern VOS_VOID DRV_TIMER_RestartDwApbTimer(
                       VOS_UINT32              uwBaseAddr,
                       VOS_UINT32              uwTimerIndx);
extern VOS_UINT32 DRV_TIMER_ReadSysTimeStamp(VOS_VOID);
extern VOS_UINT32 DRV_TIMER_Start(
                       VOS_UINT32                                 *puwTimer,
                       VOS_UINT32                                  uwTimeOutLen,
                       DRV_TIMER_MODE_ENUM_UINT16                  enMode,
                       DRV_TIMER_INT_FUNC                          pfFunc,
                       VOS_UINT32                                  uwPara);
extern VOS_VOID DRV_TIMER_StartDwApbTimer(
                       VOS_UINT32              uwBaseAddr,
                       VOS_UINT32              uwTimerIndx,
                       VOS_UINT32              uwLoadCnt,
                       VOS_UINT16              uhwMode);
extern VOS_UINT32 DRV_TIMER_StartPrecise(
                       VOS_UINT32                                 *puwTimer,
                       VOS_UINT32                                  uwTimeOutLen,
                       DRV_TIMER_MODE_ENUM_UINT16                  enMode,
                       DRV_TIMER_INT_FUNC                          pfFunc,
                       VOS_UINT32                                  uwPara);
extern VOS_UINT32 DRV_TIMER_Stop(VOS_UINT32 *puwTimer);
extern VOS_VOID DRV_TIMER_StopDwApbTimer(
                       VOS_UINT32              uwBaseAddr,
                       VOS_UINT32              uwTimerIndx);
extern VOS_UINT32 DRV_TIMER_StopPrecise(VOS_UINT32 *puwTimer);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of med_drv_timer_hifi.h */
