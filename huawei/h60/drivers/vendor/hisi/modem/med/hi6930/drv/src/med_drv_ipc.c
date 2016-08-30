

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "med_drv_ipc.h"
#include "ucom_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/*lint -e413*/

/* IPC�����ж���Ӧ����ĺ���ָ������ */
DRV_IPC_INT_SRC_ISR_STRU    g_astDrvIpcIntSrcIsr[DRV_IPC_MAX_INT_NUM];

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/



VOS_VOID DRV_IPC_Init( VOS_VOID )
{
    DRV_IPC_INT_SRC_ISR_STRU           *pstIntIsr   = DRV_IPC_GetIntSrcIsrPtr();

    /*ע��IPC ISR*/
    VOS_ConnectInterrupt(DRV_IPC_INT_NO_HIFI, DRV_IPC_Isr);

    /*дȫ0����HiFi���л���Դ*/
    UCOM_RegWr(DRV_IPC_CPU_INT_ENABLE_HIFI, 0x0);

    /*��ʼ��g_astDrvIpcIntSrcIsrΪȫNULL*/
    UCOM_MemSet(&pstIntIsr[0], 0, DRV_IPC_MAX_INT_NUM*sizeof(DRV_IPC_INT_SRC_ISR_STRU));

}
VOS_VOID DRV_IPC_Isr( VOS_VOID )
{
    VOS_UINT32                              i;
    VOS_UINT32                              uwIntState;
    DRV_IPC_INT_SRC_ISR_STRU               *pstIntIsr = DRV_IPC_GetIntSrcIsrPtr();

    /*��ȡDRV_IPC_CPU_INT_STAT_REG(IPC_TARGET_CPU_HIFI)*/
    uwIntState  = UCOM_RegRd(DRV_IPC_CPU_INT_STAT_HIFI);

    /* ���ж�Դ���������˳� */
    if (0 == uwIntState)
    {
        return;
    }

    /*дuwState��DRV_IPC_CPU_INT_CLR_REG(IPC_TARGET_CPU_HIFI)����Ѿ������ж�*/
    UCOM_RegWr(DRV_IPC_CPU_INT_CLR_HIFI, uwIntState);

    /*��������32��BITλ��IPC�ж�����*/
    for (i = 0; i < DRV_IPC_MAX_INT_NUM; i++)
    {
        /*����BITλ���ж�����*/
        if (uwIntState & (0x1L << i))
        {
            /*����Ӧ�жϴ������ǿ�*/
            if (VOS_NULL != pstIntIsr[i].pfFunc)
            {
                /*���ö�Ӧ���жϴ�����*/
                pstIntIsr[i].pfFunc(pstIntIsr[i].uwPara);
            }
        }
    }

}
VOS_VOID DRV_IPC_RegIntSrc(
                IPC_INT_LEV_E                   enSrc,
                DRV_IPC_INT_FUNC                pfFunc,
                VOS_UINT32                      uwPara)
{
    DRV_IPC_INT_SRC_ISR_STRU               *pstIntIsr = DRV_IPC_GetIntSrcIsrPtr();

    /*�����Ӧ�ж�Դ�ж�*/
    /* UCOM_RegWr(DRV_IPC_CPU_INT_CLR_HIFI, (0x1<<enSrc)); */

    /*����ע����жϷ��������Ϣ*/
    pstIntIsr[enSrc].pfFunc = pfFunc;
    pstIntIsr[enSrc].uwPara = uwPara;

    /*������λ��ʹ�ܸ�Դ�ж�*/
    UCOM_RegBitWr(DRV_IPC_CPU_INT_ENABLE_HIFI, enSrc, enSrc, 1);
}


VOS_VOID DRV_IPC_TrigInt(
                VOS_UINT16                  enTarget,
                VOS_UINT16                  enIntSrc)
{
    /*д��ӦCPU��Ӧ�ж�Դ���أ�����IPC�ж�*/
    UCOM_RegBitWr(DRV_IPC_CPU_RAW_INT((VOS_UINT32)enTarget), enIntSrc, enIntSrc, 1);
}

/*lint +e413*/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

