

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "med_drv_timer_hifi.h"
#include "ucom_comm.h"
#if (VOS_CPU_TYPE == VOS_HIFI)
#include "hal.h"
#endif


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
/* ��ʱ������������ݶ���ʵ��, ���涨ʱ��״̬��ע����Ϣ�� */
DRV_TIMER_CTRL_OBJ_STRU     g_stDrvTimerCtrlObj;


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_VOID DRV_TIMER_Init(VOS_VOID)
{
    VOS_UINT32                          i;
    /* ��V7R2��HIFIʹ�õ�timer���л�Ϊ32k������Ҫ�ϲ���� */
    //VOS_UINT32                          uwScTimerEn1Addr;
    DRV_TIMER_CTRL_OBJ_STRU            *pstTimers  = DRV_TIMER_GetTimerCtrlObjPtr();

    /* ��V7R2��HIFIʹ�õ�timer���л�Ϊ32k������Ҫ�ϲ���� */
    /* ��ȡtimerʹ�ܼĴ���1�ĵ�ַ */
    //uwScTimerEn1Addr = SOC_AO_SCTRL_SC_TIMER_EN1_ADDR(DRV_AO_SC_BASE_ADDR);

    /*��ʼ��Timer����ȫ�ֽṹȫ0*/
    UCOM_MemSet(pstTimers, 0, sizeof(DRV_TIMER_CTRL_OBJ_STRU));

    /*��ʼ��Timer����״̬*/
    pstTimers->uwIdleNum                        = DRV_TIMER_LINKED_MAX_NUM;
    pstTimers->pstLinkedBlk                     = VOS_NULL_PTR;
    pstTimers->pstIdleBlk                       = pstTimers->astLinkedBlk;

    /*��ʼ����ʽTimer˫�������¼��ʹ��ǰָ������*/
    for(i = 0; i < (DRV_TIMER_LINKED_MAX_NUM - 1); i++)
    {
        pstTimers->astLinkedBlk[i].uhwUsedFlag  = VOS_NO;
        pstTimers->astLinkedBlk[i].pstNext      = &(pstTimers->astLinkedBlk[i+1]);
    }

    /*����ʽTimer�߽�����������⴦��*/
    pstTimers->astLinkedBlk[DRV_TIMER_LINKED_MAX_NUM - 1].uhwUsedFlag  = VOS_NO;
    pstTimers->astLinkedBlk[DRV_TIMER_LINKED_MAX_NUM - 1].pstNext      = VOS_NULL_PTR;

    /*��ʼ������ʽ��ʱ����Ŀǰϵͳ��֧��1����֧�ָ������ڴ���ӳ�ʼ������*/
    pstTimers->astUnlinkBlk[0].uwDevice         = DRV_TIMER_DEVICE_DUALTIMER5_1;
    pstTimers->astUnlinkBlk[0].uhwUsedFlag      = VOS_NO;

    /*ע�ᶨʱ���豸�е�timer�жϴ������, Timer8,AP Dual_Timer5_1�ж�*/
    VOS_ConnectInterrupt(OS_INTR_CONNECT_30, DRV_TIMER_IsrUnLinkedTimer);

    /*ʹ�ܶ�ʱ���豸�е�Timer11�ж�*/
    VOS_EnableInterrupt(OS_INTR_CONNECT_30);

    /* ע��Timer0����жϻص����� */
    VOS_ConnectInterrupt(OS_INTR_CONNECT_28, DRV_TIMER_IsrLinkedTimer);

    /*ʹ�ܶ�ʱ���豸�е�Timer10�ж�*/
    VOS_EnableInterrupt(OS_INTR_CONNECT_28);

    /*��ֹ����Hifi�ڲ�Timer�жϣ���Щ�жϿ��Բ鿴V7R2���ж�������*/
    VOS_DisableInterrupt(OS_INTR_CONNECT_05);
    VOS_DisableInterrupt(OS_INTR_CONNECT_06);
    VOS_DisableInterrupt(OS_INTR_CONNECT_19);

    /* ��V7R2��HIFIʹ�õ�timer���л�Ϊ32k������Ҫ�ϲ���� */
}


VOS_UINT32 DRV_TIMER_Start(
                VOS_UINT32                                 *puwTimer,
                VOS_UINT32                                  uwTimeOutLen,
                DRV_TIMER_MODE_ENUM_UINT16                  enMode,
                DRV_TIMER_INT_FUNC                          pfFunc,
                VOS_UINT32                                  uwPara)
{
    DRV_TIMER_CTRL_BLOCK_STRU                              *pstTimer= VOS_NULL_PTR;
    DRV_TIMER_CTRL_OBJ_STRU                                *pstTimers;
    VOS_CPU_SR                                              uwCpuSr;
    VOS_UINT32                                              uwLinkedMaxTimeout;

    /*���жϻ���*/
    uwCpuSr = VOS_SplIMP();

    /*��ȡtimer���ƿ�ṹ��ָ��*/
    pstTimers   = DRV_TIMER_GetTimerCtrlObjPtr();

    uwLinkedMaxTimeout = DRV_TIMER_LINKED_MAX_TIMEOUT;

    /*����ʹ�õ����Ӷ�ʱ�������Ѵ����޻�ʱʱ������Լ��ֵ��ֱ�ӷ��ش���*/
    if (uwTimeOutLen > uwLinkedMaxTimeout)
    {
        VOS_Splx(uwCpuSr);
        return VOS_ERR;
    }

    /*�����µĶ�ʱ��*/
    pstTimer    = DRV_TIMER_GetLinkedBlk(pstTimers);

    /*���޷����뵽�µĶ�ʱ��*/
    if (VOS_NULL_PTR == pstTimer)
    {
        VOS_Splx(uwCpuSr);
        return VOS_ERR;
    }
    else
    {
        /*���ն�ʱ��ز���*/
        pstTimer->uwLength  = uwTimeOutLen;
        pstTimer->enMode    = enMode;
        pstTimer->pfFunc    = pfFunc;
        pstTimer->uwPara    = uwPara;

        /* ��ʱ���豸���� */
        pstTimer->uwDevice  = DRV_TIMER_DEVICE_DUALTIMER5_0;

        /* �������Ӷ�ʱ�� */
        DRV_TIMER_AddToLink(pstTimer, pstTimers);

        /* ���˶�ʱ�����ƿ��ַ���� */
        *puwTimer           = (VOS_UINT32)pstTimer;
    }

    /*�ͷŻ���*/
    VOS_Splx(uwCpuSr);

    return VOS_OK;
}


VOS_UINT32 DRV_TIMER_Stop(VOS_UINT32 *puwTimer)
{
    VOS_UINT32                          uwRet       = VOS_OK;
    DRV_TIMER_CTRL_OBJ_STRU            *pstTimers   = DRV_TIMER_GetTimerCtrlObjPtr();
    DRV_TIMER_CTRL_BLOCK_STRU          *pstTimer    = (DRV_TIMER_CTRL_BLOCK_STRU *)(*puwTimer);
    VOS_CPU_SR                          uwCpuSr;

    /*���жϻ���*/
    uwCpuSr = VOS_SplIMP();

    /*�ж�puwTimer�Ϸ���*/
    uwRet   = DRV_TIMER_CheckLinkedBlk((DRV_TIMER_CTRL_BLOCK_STRU *)(*puwTimer),
                                        pstTimers->pstLinkedBlk);

    /*���Ƿ����򷵻ش���*/
    if (VOS_ERR == uwRet)
    {
        VOS_Splx(uwCpuSr);
        return VOS_ERR;
    }

    /* ����ʱ���ó���Чģʽ,��ʱ�ж���Ӧ�н�ɾ�����ͷ������ʱ�� */
    pstTimer->enMode = DRV_TIMER_MODE_ONESHOT;
    pstTimer->pfFunc = (DRV_TIMER_INT_FUNC)VOS_NULL_PTR;

    VOS_Splx(uwCpuSr);

    return uwRet;
}


VOS_UINT32 DRV_TIMER_StartPrecise(
                VOS_UINT32                                 *puwTimer,
                VOS_UINT32                                  uwTimeOutLen,
                DRV_TIMER_MODE_ENUM_UINT16                  enMode,
                DRV_TIMER_INT_FUNC                          pfFunc,
                VOS_UINT32                                  uwPara)
{
    DRV_TIMER_CTRL_BLOCK_STRU                              *pstTimer;
    DRV_TIMER_CTRL_OBJ_STRU                                *pstTimers;
    VOS_CPU_SR                                              uwCpuSr;

    /*���жϻ���*/
    uwCpuSr = VOS_SplIMP();

    /*��ȡtimer���ƿ�ṹ��ָ��*/
    pstTimers   = DRV_TIMER_GetTimerCtrlObjPtr();

    /*����ʱʱ���Ƿ���ǰ������ʱ���Ѿ��ﵽ֧�ֵ����ޣ���ֱ�ӷ��ش���*/
    if (uwTimeOutLen > DRV_TIMER_UNLINK_MAX_TIMEOUT)
    {
        /*�ͷŻ���*/
        VOS_Splx(uwCpuSr);
        return VOS_ERR;
    }

    /*�����µĶ�ʱ��*/
    pstTimer = DRV_TIMER_GetUnlinkBlk(pstTimers);

    /*���޷����뵽�µĶ�ʱ��*/
    if (VOS_NULL_PTR == pstTimer)
    {
        /*�ͷŻ���*/
        VOS_Splx(uwCpuSr);
        return VOS_ERR;
    }
    else
    {
        /*���ն�ʱ��ز���*/
        pstTimer->uwLength  = uwTimeOutLen;
        pstTimer->enMode    = enMode;
        pstTimer->pfFunc    = pfFunc;
        pstTimer->uwPara    = uwPara;

        /*���ݶ�ʱ��ʱ�Ӽ���LoadCntֵ��������pstTimer*/
        pstTimer->uwLoadCnt = (pstTimer->uwLength * (DRV_TIMER_UNLINK_TIMER_FREQ))/10000;

        /*���������Ӷ�ʱ��*/
        if (DRV_TIMER_DEVICE_DUALTIMER5_1 == pstTimer->uwDevice)
        {
            /*���ò�����������ʱ��*/
            DRV_TIMER_StartDwApbTimer(DRV_TIMER_UNLINK_TIMER_ADDR,
                                      DRV_TIMER_UNLINK_TIMER_IDX,
                                      pstTimer->uwLoadCnt,
                                      pstTimer->enMode);
        }
        else
        {
            /*��HIFI֧�ָ����ⲿ������ʱ�����ڴ�������Ʋ���*/
        }

        /*���˶�ʱ�����ƿ��ַ���棬���ظ�������*/
        *puwTimer           = (VOS_UINT32)pstTimer;
    }

    /*�ͷŻ���*/
    VOS_Splx(uwCpuSr);

    return VOS_OK;
}


VOS_UINT32 DRV_TIMER_StopPrecise(VOS_UINT32 *puwTimer)
{
    VOS_UINT32                                              uwTimerNum;
    DRV_TIMER_CTRL_OBJ_STRU                                *pstTimers;
    DRV_TIMER_CTRL_BLOCK_STRU                              *pstTimer= VOS_NULL_PTR;
    VOS_CPU_SR                                              uwCpuSr;

    /*���жϻ���*/
    uwCpuSr = VOS_SplIMP();

    pstTimer    = (DRV_TIMER_CTRL_BLOCK_STRU *)(*puwTimer);
    /*��ȡtimer���ƿ�ṹ��ָ��*/
    pstTimers   = DRV_TIMER_GetTimerCtrlObjPtr();

    /*��֤��ʱ������Ƿ��ڷ����Ӷ�ʱ�����ƿ������*/
    for (uwTimerNum = 0; uwTimerNum < DRV_TIMER_UNLINK_MAX_NUM; uwTimerNum++)
    {
        if (pstTimer == &(pstTimers->astUnlinkBlk[uwTimerNum]))
        {
            pstTimer->uhwUsedFlag = VOS_NO;

            /*��ɾ���Ľڵ�ΪӲ����ʱ��������ҪֹͣӲ���豸*/
            if (DRV_TIMER_DEVICE_DUALTIMER5_1 == pstTimer->uwDevice)
            {
                DRV_TIMER_StopDwApbTimer(DRV_TIMER_UNLINK_TIMER_ADDR,
                                         DRV_TIMER_UNLINK_TIMER_IDX);
                pstTimers->uhwUnlinkNum--;
            }
            else
            {
                /*��HIFI֧�ָ����ⲿ������ʱ�����ڴ�������Ʋ���*/
            }

            /*�ͷŻ���*/
            VOS_Splx(uwCpuSr);
            return VOS_OK;
        }
    }

    /*�ͷŻ���*/
    VOS_Splx(uwCpuSr);

    /*���˶�ʱ�����ڶ����ڣ�ֱ�ӷ��ش���*/
    return VOS_ERR;
}


VOS_VOID DRV_TIMER_IsrUnLinkedTimer(VOS_VOID)
{
    VOS_UINT32                          uwTimerNum;
    DRV_TIMER_CTRL_BLOCK_STRU          *pstTimer;
    DRV_TIMER_CTRL_OBJ_STRU            *pstTimers;

    /*��ȡtimer���ƿ�ṹ��ָ��*/
    pstTimers       = DRV_TIMER_GetTimerCtrlObjPtr();

    /*V7R2�����ж�Ϊ����*/
    (VOS_VOID)UCOM_RegRd(DRV_TIMER_EOI(DRV_TIMER_UNLINK_TIMER_ADDR, DRV_TIMER_UNLINK_TIMER_IDX));

    for (uwTimerNum = 0; uwTimerNum < DRV_TIMER_UNLINK_MAX_NUM; uwTimerNum++)
    {
        pstTimer    = &(pstTimers->astUnlinkBlk[uwTimerNum]);

        if (DRV_TIMER_DEVICE_DUALTIMER5_1 ==  pstTimer->uwDevice)
        {
            /*���ö�Ӧ��ʱ�ص�����*/
            pstTimer->pfFunc((VOS_UINT32)pstTimer, pstTimer->uwPara);
            return;
        }
    }
}
VOS_VOID DRV_TIMER_IsrLinkedTimer(VOS_VOID)
{
    VOS_UINT32                     uwLoadCnt       = 0;
    DRV_TIMER_CTRL_OBJ_STRU       *pstTimers;
    DRV_TIMER_CTRL_BLOCK_STRU     *pstTimer;
    DRV_TIMER_CTRL_BLOCK_STRU     *pstNextTimer;
    DRV_TIMER_CTRL_BLOCK_STRU     *pstExpiredTimers[DRV_TIMER_LINKED_MAX_NUM] = {VOS_NULL_PTR};
    VOS_UINT32                     uwExpiredCnt = 0;
    VOS_UINT32                     i;
    VOS_UINT32                     uwLinkedPrecisionNum;

    /*��ȡtimer���ƿ�ṹ��ָ��*/
    pstTimers                       = DRV_TIMER_GetTimerCtrlObjPtr();

    pstTimer                        = pstTimers->pstLinkedBlk;

    /*�����Ӷ�ʱͷ�ڵ�Ϊ��(�����нڵ����ͷ�)���رն�ʱ��*/
    if (VOS_NULL_PTR == pstTimer)
    {
        DRV_TIMER_StopLinkedTimer();
        return;
    }

    /* ͷ�ڵ��ѵ���, ��ͷ�ڵ�LoadCnt��0 */
    pstTimer->uwLoadCnt  = 0;

    uwLinkedPrecisionNum = DRV_TIMER_LINKED_TIMER_FREQ / 10000;

    /*���ҽڵ������е����е��ڽڵ�(��������Ϊ0.1ms�������Χ�ڿ����ж�����ڽڵ�)*/
    while ( uwLoadCnt <= uwLinkedPrecisionNum )
    {
        /*����ýڵ��Nextָ��*/
        pstNextTimer = pstTimer->pstNext;

        /*���浽�ڽڵ�*/
        pstExpiredTimers[uwExpiredCnt] = pstTimer;
        uwExpiredCnt++;

        /*���˵��ڽڵ��������ɾ��*/
        DRV_TIMER_DelFromLink(pstTimer, pstTimers);

        /*��һ���ڵ�*/
        pstTimer = pstNextTimer;

        /*�ۼӵ��ڵĽڵ�LoadCntֵ*/
        if(VOS_NULL_PTR != pstTimer)
        {
            uwLoadCnt += pstTimer->uwLoadCnt;
        }
        else
        {
            break;
        }

    }

    /*���ڵ�һ���ǵ��ڽڵ㣬���ǿգ���ɾ����ǰ�ڵ��ϵ����������ʱ��*/
    if (VOS_NULL_PTR != (pstTimers->pstLinkedBlk))
    {
        /*����ͷָ����Ϣ*/
        (pstTimers->pstLinkedBlk)->pstPrevious  = VOS_NULL_PTR;

        /*������ʱ��*/
        DRV_TIMER_StartLinkedTimer((pstTimers->pstLinkedBlk)->uwLoadCnt);
    }
    /*��ͷ�ڵ�Ϊ�գ�ֹͣ��ʱ��, ��ֹhifi��ʱ����תƥ����Ŀ��ֵ�����ж�*/
    else
    {
        DRV_TIMER_StopLinkedTimer();
    }

    /* �����ڽڵ� */
    for(i = 0; i < uwExpiredCnt; i++)
    {
        pstTimer = pstExpiredTimers[i];

        /*����ѭ����ʱ�������¼ӻ�����*/
        if (DRV_TIMER_MODE_PERIODIC == pstTimer->enMode)
        {
            DRV_TIMER_AddToLink(pstTimer, pstTimers);
        }
        /*���ڷ�ѭ����ʱ�����ͷ�*/
        else
        {
            DRV_TIMER_FreeLinkedBlk(pstTimer, pstTimers);
        }
    }
    /* ���ûص�����,ע��:���������������DRV_TIMER_FreeLinkedBlkû�������ʱ���ص� */
    for(i = 0; i < uwExpiredCnt; i++)
    {
        pstTimer = pstExpiredTimers[i];

         /* ִ�лص����� */
        if( pstTimer->pfFunc != VOS_NULL_PTR )
        {
            pstTimer->pfFunc((VOS_UINT32)pstTimer, pstTimer->uwPara);
        }
    }

}


VOS_UINT32 DRV_TIMER_CheckLinkedBlk(
                DRV_TIMER_CTRL_BLOCK_STRU                  *pstTimer,
                DRV_TIMER_CTRL_BLOCK_STRU                  *pstTimerLst)
{
    DRV_TIMER_CTRL_BLOCK_STRU                              *pstListTimer;

    /*������ͷ�ڵ㸳����ʱ����*/
    pstListTimer = pstTimerLst;

    /*������Ӧ����,���ҿ��ƿ���ƥ��Ľڵ�*/
    while (VOS_NULL_PTR != pstListTimer)
    {
        /*����������ƥ��ڵ��ַ��ÿ��ƿ��ַ���ȣ�ֱ�ӷ�������*/
        if (pstTimer == pstListTimer)
        {
            return VOS_OK;
        }
        pstListTimer = pstListTimer->pstNext;
    }

    /*��δ��������ƥ��ڵ����ƥ��Ľڵ��ַ��ÿ��ƿ��ַ���ȣ����ش���*/
    return VOS_ERR;
}


DRV_TIMER_CTRL_BLOCK_STRU *DRV_TIMER_GetLinkedBlk(DRV_TIMER_CTRL_OBJ_STRU *pstTimers)
{
    DRV_TIMER_CTRL_BLOCK_STRU                              *pstTempTimer;

    /*���޿��ж�ʱ�����ʹ�ã���ֱ�ӷ��ؿ�ָ��*/
    if (0 == pstTimers->uwIdleNum)
    {
        return VOS_NULL_PTR;
    }
    else
    {
        pstTimers->uwIdleNum--;
    }


    /*ȡ����timer���ƿ��ͷ�ڵ�ʹ��*/
    pstTempTimer                = pstTimers->pstIdleBlk;

    /*���¿��ƿ�ͷ�ڵ�*/
    pstTimers->pstIdleBlk       = pstTimers->pstIdleBlk->pstNext;

    /*��ʼ����timer���ƿ�ڵ���Ϣ*/
    pstTempTimer->uhwUsedFlag   = VOS_YES;
    pstTempTimer->pstNext       = VOS_NULL_PTR;
    pstTempTimer->pstPrevious   = VOS_NULL_PTR;

    /*���ظ�timer���ƿ�*/
    return pstTempTimer;
}
DRV_TIMER_CTRL_BLOCK_STRU *DRV_TIMER_GetUnlinkBlk(DRV_TIMER_CTRL_OBJ_STRU *pstTimers)
{
    VOS_UINT32                          uwTimerNum;
    DRV_TIMER_CTRL_BLOCK_STRU          *pstTimer;

    /*�����з���ʽ���ƿ鶼��ʹ�ã���ֱ�ӷ��ؿ�ָ��*/
    if (DRV_TIMER_UNLINK_MAX_NUM == pstTimers->uhwUnlinkNum)
    {
        return VOS_NULL_PTR;
    }

    /*���·����Ӷ�ʱ��״̬��Ϣ*/
    pstTimers->uhwUnlinkNum++;

    /*���ҷ����Ӷ�ʱ�����ƿ����ڵ�һ��δ��ʹ�õĶ�ʱ����*/
    for (uwTimerNum = 0; uwTimerNum < DRV_TIMER_UNLINK_MAX_NUM; uwTimerNum++)
    {
        /*��ȡ�����ķ����Ӷ�ʱ����ָ��*/
        pstTimer    = &(pstTimers->astUnlinkBlk[uwTimerNum]);

        /*����ǰ�����Ӷ�ʱ����δ��ʹ��*/
        if (VOS_NO == pstTimer->uhwUsedFlag)
        {
            /*�޸ĸ�timer���ƿ�״̬��Ϣ*/
            pstTimer->uhwUsedFlag   = VOS_YES;
            break;
        }
    }

    /*���ظ�timer���ƿ�*/
    return pstTimer;
}
VOS_VOID DRV_TIMER_FreeLinkedBlk(
                DRV_TIMER_CTRL_BLOCK_STRU                  *pstTimer,
                DRV_TIMER_CTRL_OBJ_STRU                    *pstTimers)
{
    /*���ͷŵ�timer���ƿ�ǰ�������timer���ƿ����*/
    pstTimer->uhwUsedFlag   = VOS_NO;
    pstTimer->pstNext       = pstTimers->pstIdleBlk;
    pstTimers->pstIdleBlk   = pstTimer;

    /*���¿���timer���ƿ����*/
    pstTimers->uwIdleNum++;
}


VOS_VOID DRV_TIMER_AddToLink(
                DRV_TIMER_CTRL_BLOCK_STRU                  *pstTimer,
                DRV_TIMER_CTRL_OBJ_STRU                    *pstTimers)
{
    VOS_UINT32                                              uwCntTmp;
    DRV_TIMER_CTRL_BLOCK_STRU                              *pstTmpTimer;
    DRV_TIMER_CTRL_BLOCK_STRU                              *pstTmpPreTimer;

    /*���½ڵ�״̬��Ϣ*/
    pstTimers->uhwLinkedNum++;

    /*���ݶ�ʱ��ʱ�Ӽ���LoadCntֵ��������pstTimer*/
    pstTimer->uwLoadCnt = (pstTimer->uwLength * (DRV_TIMER_LINKED_TIMER_FREQ))/10000;

    /*��ʼ����ʱ����*/
    pstTmpTimer     = pstTimers->pstLinkedBlk;
    pstTmpPreTimer  = pstTimers->pstLinkedBlk;

    if (VOS_NULL_PTR != pstTmpTimer)
    {
        /*���㱾�ڵ������õ�LoadCntֵ*/
        uwCntTmp        = pstTimer->uwLoadCnt
                        + (pstTmpTimer->uwLoadCnt - DRV_TIMER_ReadLinkedTimer());
    }
    else
    {
        /*��ǰΪ�׽ڵ�*/
        uwCntTmp        = pstTimer->uwLoadCnt;
    }

    /*����ǰͷ�ڵ�Ϊ��,�򱾽ڵ�Ϊͷ�ڵ�*/
    if (VOS_NULL_PTR == pstTimers->pstLinkedBlk)
    {
        pstTimers->pstLinkedBlk                = pstTimer;
    }
    else
    {
        /*�������ҵ�ǰ�����и��ڵ�ʱ��,���ҵ����ʵĲ���λ��*/
        while (VOS_NULL_PTR != pstTmpTimer)
        {
            /*�������Ľڵ�ʱ���ȵ�ǰ�ڵ㳤�������������Ѱ�Ҳ���λ��*/
            if (uwCntTmp >= (pstTmpTimer->uwLoadCnt))
            {
                /*��������ڵ�ʱ��*/
                uwCntTmp -= pstTmpTimer->uwLoadCnt;
            }
            else
            {
                /*�������Ľڵ�ʱ���ȵ�ǰ�ڵ�̣������λ���ڵ�ǰ�ڵ�ǰ�����µ�ǰ�ڵ�ʱ��*/
                pstTmpTimer->uwLoadCnt -= uwCntTmp;
                break;
            }
            pstTmpPreTimer          = pstTmpTimer;
            pstTmpTimer             = pstTmpTimer->pstNext;
        }

        /*����Ҫ����ͷ�ڵ�֮ǰ*/
        if ((pstTimers->pstLinkedBlk == pstTmpTimer)
            && (VOS_NULL_PTR != pstTmpTimer))
        {
            pstTimers->pstLinkedBlk = pstTimer;
            pstTimer->pstNext       = pstTmpTimer;
            pstTmpTimer->pstPrevious= pstTimer;
        }
        else
        {
            /*���²���ڵ��ǰ���ϵ*/
            pstTmpPreTimer->pstNext = pstTimer;
            pstTimer->pstPrevious   = pstTmpPreTimer;
            pstTimer->pstNext       = pstTmpTimer;
            if(pstTmpTimer != VOS_NULL_PTR)
            {
                pstTmpTimer->pstPrevious= pstTimer;
            }
        }
    }
    /* ��������λ����ͷ�ڵ�ǰ,�򽫴˽ڵ����Ϊͷ�ڵ㲢����������ʱ��*/
    if ( pstTimer == pstTimers->pstLinkedBlk )
    {
        /*����������ʱ��*/
        DRV_TIMER_StartLinkedTimer(pstTimer->uwLoadCnt);
    }
    /*���򣬸��´�����ڵ��ʱ��*/
    else
    {
        pstTimer->uwLoadCnt = uwCntTmp;
    }
}
VOS_VOID DRV_TIMER_DelFromLink(
                DRV_TIMER_CTRL_BLOCK_STRU                  *pstTimer,
                DRV_TIMER_CTRL_OBJ_STRU                    *pstTimers)
{

    /*����ɾ���Ķ�Ӧ�ڵ�Ϊͷ�ڵ�*/
    if (pstTimer == pstTimers->pstLinkedBlk)
    {
        /*����ͷ�ڵ���Ϣ*/
        pstTimers->pstLinkedBlk                    = pstTimer->pstNext;
        if (VOS_NULL_PTR != pstTimers->pstLinkedBlk)
        {
            pstTimers->pstLinkedBlk->pstPrevious   = VOS_NULL_PTR;
        }
    }
    else
    {
        /*����ɾ���Ľڵ��ͷ�ڵ㣬�����ǰ��ڵ���Ϣ*/
        (pstTimer->pstPrevious)->pstNext            = pstTimer->pstNext;
        if ( VOS_NULL_PTR != pstTimer->pstNext )
        {
            (pstTimer->pstNext)->pstPrevious        = pstTimer->pstPrevious;
        }
    }

    pstTimers->uhwLinkedNum -- ;

    /*��ɾ���Ľڵ��к����ڵ�,����¸ýڵ�ļ���ֵ*/
    if (VOS_NULL_PTR != pstTimer->pstNext)
    {
        pstTimer->pstNext->uwLoadCnt += pstTimer->uwLoadCnt;
    }

    /*����ǰ���޽ڵ㣬��ֹͣ��ʱ��*/
    if (0 == pstTimers->uhwLinkedNum)
    {
        /*ֹͣ��ʱ��*/
        DRV_TIMER_StopLinkedTimer();
    }

    /* ɾ����ǰ�ڵ��ǰ���ϵ */
    pstTimer->pstNext       = VOS_NULL_PTR;
    pstTimer->pstPrevious   = VOS_NULL_PTR;
}
VOS_VOID DRV_TIMER_StartDwApbTimer(
                VOS_UINT32              uwBaseAddr,
                VOS_UINT32              uwTimerIndx,
                VOS_UINT32              uwLoadCnt,
                VOS_UINT16              uhwMode)
{
    VOS_UINT32                          uwTimerCtrlAddr;
    VOS_UINT32                          uwTimerCountAddr;
    VOS_UINT32                          uwTimerBitWr;
    /* ������ʱ�� */
    UCOM_RegBitWr(DRV_TIMER_PERIPH_CLK_EN_ADDR,UCOM_BIT0,UCOM_BIT0,0x1);

    /*����APB TIMER�豸��������ΪuwTimerIndx�Ķ�ʱ���Ŀ��ƼĴ�����ַ*/
    uwTimerCtrlAddr  = DRV_TIMER_CONTROL(uwBaseAddr, uwTimerIndx);

    /*����APB TIMER�豸��������ΪuwTimerIndx�Ķ�ʱ���ĳ�ʼֵ�Ĵ�����ַ*/
    uwTimerCountAddr = DRV_TIMER_LOADCOUNT(uwBaseAddr, uwTimerIndx);

    /*��V7R2��TIMER����֤���ֲ�����BIT4Ϊ1ʱд����λ���������������쳣��
      ��ctrl�Ĵ�������ֵ���Ļ���loadcnt��ʹ�ܡ���˲���ʹ��BITλ��һд��
      ���ݼĴ����ֲᣬ�жϲ����Σ�BIT2д0��TIMER��ʹ�ܣ�BIT0д0��BIT1���������*/
    uwTimerBitWr     = 0;
    uwTimerBitWr     = uwTimerBitWr | (uhwMode << UCOM_BIT1);

    /*�趨��ʱ��: ��ʱ���жϲ�����, ��ʱ��ģʽ������ȷ��
      ���⣬V7R2Ĭ��Ϊ�ݼ���DRV�����úô�СΪ32bit����, �����㲻��Ҫר������*/
    UCOM_RegWr(uwTimerCtrlAddr, uwTimerBitWr);

    /*���ö�ʱ����ֵ*/
    UCOM_RegWr(uwTimerCountAddr, uwLoadCnt);

    /*�����Ӧ���жϼĴ���,�����Ӧ�ж�*/
    (VOS_VOID)UCOM_RegRd(DRV_TIMER_EOI(uwBaseAddr, uwTimerIndx));

    /*������ʱ��*/
    uwTimerBitWr     = uwTimerBitWr | (1 << UCOM_BIT0);
    UCOM_RegWr(uwTimerCtrlAddr, uwTimerBitWr);

}
VOS_VOID DRV_TIMER_RestartDwApbTimer(
                VOS_UINT32              uwBaseAddr,
                VOS_UINT32              uwTimerIndx)
{
    VOS_UINT32                          uwTimerAddr;
    VOS_UINT32                          uwTimerWr;

    /*����APB TIMER�豸��������ΪuwTimerIndx�Ķ�ʱ���Ŀ��ƼĴ�����ַ*/
    uwTimerAddr = DRV_TIMER_CONTROL(uwBaseAddr, uwTimerIndx);

    uwTimerWr   = UCOM_RegRd(uwTimerAddr);

    /*��ֹ��ʱ��,���ڵ�0λд0����4λΪtimer���λ����ͬʱд0*/
    uwTimerWr   = uwTimerWr & (0xffffffee);
    UCOM_RegWr(uwTimerAddr, uwTimerWr);

    /*д���Ӧ���жϼĴ���,�����Ӧ�ж�*/
    (VOS_VOID)UCOM_RegRd(DRV_TIMER_EOI(uwBaseAddr, uwTimerIndx));

    /*������ʱ��, ���ڵ�0λд1*/
    uwTimerWr   = uwTimerWr | (1 << UCOM_BIT0);
    UCOM_RegWr(uwTimerAddr, uwTimerWr);
}


VOS_UINT32 DRV_TIMER_ReadDwApbTimer(
                VOS_UINT32              uwBaseAddr,
                VOS_UINT32              uwTimerIndx,
                VOS_UINT32             *puwCntLow,
                VOS_UINT32             *puwCntHigh)
{
    VOS_UINT32                          uwCntLow;

    /*��ȡ����ֵ��32bit*/
    uwCntLow = UCOM_RegRd(DRV_TIMER_CURRENTVALUE(uwBaseAddr, uwTimerIndx));

    /*������ı������ֵָ��ǿգ��򱣴����ֵ��32bit*/
    if (VOS_NULL != puwCntLow)
    {
        *puwCntLow  = uwCntLow;
    }

    /*Ĭ�Ϸ��ؼ���ֵ��32bit*/
    return uwCntLow;
}
VOS_VOID DRV_TIMER_StopDwApbTimer(
                VOS_UINT32              uwBaseAddr,
                VOS_UINT32              uwTimerIndx)
{
    VOS_UINT32                          uwTimerAddr;
    VOS_UINT32                          uwTimerWr;

    /*����APB TIMER�豸��������ΪuwTimerIndx�Ķ�ʱ���Ŀ��ƼĴ�����ַ*/
    uwTimerAddr = DRV_TIMER_CONTROL(uwBaseAddr, uwTimerIndx);

    uwTimerWr   = UCOM_RegRd(uwTimerAddr);

    /*��ֹ��ʱ��,�������жϣ����ڵ�0λд0����2λд1, ��4λΪtimer���λ����ͬʱд0 */
    uwTimerWr   = uwTimerWr & 0xffffffee;
    uwTimerWr   = uwTimerWr | (1 << UCOM_BIT2);

    /*�����ж�*/
    UCOM_RegWr(uwTimerAddr, uwTimerWr);
}
VOS_UINT32 DRV_TIMER_ReadSysTimeStamp(VOS_VOID)
{
    return UCOM_RegRd(DRV_TIMER_OM_ADDR);
}


VOS_UINT32 DRV_TIMER_GetOmFreq(VOS_VOID)
{
    return DRV_TIMER_OM_FREQ;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

