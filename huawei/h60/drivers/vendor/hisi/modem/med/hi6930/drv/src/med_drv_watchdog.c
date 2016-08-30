

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "med_drv_watchdog.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

VOS_VOID DRV_WATCHDOG_StartWdg(VOS_UINT32 uwLoadCnt)
{
#if 0
    /* ʹ������ʱ��,25��26bit��1 */
    UCOM_RegBitWr(DRV_WATCHDOG_CLOCK_ENABLE, UCOM_BIT25, UCOM_BIT26, 0x3);

    /* д�����ֵ0x1ACCE551������ */
    UCOM_RegWr(DRV_WATCHDOG_LOCK, DRV_WATCHDOG_UNLOCK_NUM);

    /* ����ֹͣ */
    UCOM_RegWr(DRV_WATCHDOG_CONTROL, DRV_WATCHDOG_CONTROL_DISABLE);

    /* ��ʼ��ʱ������λ 1/32768 s(ʱ��Ƶ��Ϊ32K) */
    UCOM_RegWr(DRV_WATCHDOG_LOAD, uwLoadCnt);

    /* ���ж�����,��ʼ���� */
    UCOM_RegWr(DRV_WATCHDOG_CONTROL, DRV_WATCHDOG_CONTROL_ENABLE);

    /* ������д����ǽ���ֵ���˴�д0x0 */
    UCOM_RegWr(DRV_WATCHDOG_LOCK, DRV_WATCHDOG_LOCK_NUM);
#endif
    return;

}


VOS_VOID DRV_WATCHDOG_RestartWdg(VOS_VOID)
{
#if 0
    /* д�����ֵ0x1ACCE551������ */
    UCOM_RegWr(DRV_WATCHDOG_LOCK,    DRV_WATCHDOG_UNLOCK_NUM);

    /* ���жϣ�д����ֵ���ɣ��˴�д0x4455��ҲʹWatchdog�Զ����������ֵ���¿�ʼ���� */
    UCOM_RegWr(DRV_WATCHDOG_INTCLR,  DRV_WATCHDOG_INTCLR_NUM);

    /* ������д����ǽ���ֵ���˴�д0x0 */
    UCOM_RegWr(DRV_WATCHDOG_LOCK,    DRV_WATCHDOG_LOCK_NUM);
#endif
    return;

}
VOS_VOID DRV_WATCHDOG_StopWdg(VOS_VOID)
{
#if 0
    /* ���� */
    UCOM_RegWr(DRV_WATCHDOG_LOCK,    DRV_WATCHDOG_UNLOCK_NUM);

    /* ����ֹͣ */
    UCOM_RegWr(DRV_WATCHDOG_CONTROL, DRV_WATCHDOG_CONTROL_DISABLE);

    /* ���� */
    UCOM_RegWr(DRV_WATCHDOG_LOCK,    DRV_WATCHDOG_LOCK_NUM);

    /* ��ֹ����ʱ�ӣ�watchdog1 */
    UCOM_RegBitWr(DRV_WATCHDOG_CLOCK_DISABLE, UCOM_BIT25, UCOM_BIT26, 0x0);
#endif
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


