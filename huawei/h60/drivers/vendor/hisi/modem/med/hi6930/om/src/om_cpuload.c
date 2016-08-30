

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "om_cpuload.h"
#include "med_drv_timer_hifi.h"
#include "ucom_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
OM_CPULOAD_COUNT_STRU g_stCpuLoadCount = {0};

#define OM_CPULOAD_GetLoadCntAddr()     ((VOS_UINT32)&g_stCpuLoadCount)

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


void OM_CPULOAD_Init()
{
    OM_CPULOAD_COUNT_STRU *pstLoadCnt;
    pstLoadCnt = (OM_CPULOAD_COUNT_STRU *)UCOM_GetUncachedAddr(OM_CPULOAD_GetLoadCntAddr());

    UCOM_MemSet(pstLoadCnt, 0, sizeof(OM_CPULOAD_COUNT_STRU));
}


void OM_CPULOAD_FuncEnter(VOS_UINT32 uwFuncId)
{
    OM_CPULOAD_COUNT_STRU *pstLoadCnt;
    pstLoadCnt = (OM_CPULOAD_COUNT_STRU *)UCOM_GetUncachedAddr(OM_CPULOAD_GetLoadCntAddr());

    pstLoadCnt->auwEnterTS[uwFuncId] = DRV_TIMER_ReadSysTimeStamp();

}


void OM_CPULOAD_FuncExit(VOS_UINT32 uwFuncId)
{
    OM_CPULOAD_COUNT_STRU *pstLoadCnt;
    VOS_UINT32     uwTimeUsed;

    pstLoadCnt = (OM_CPULOAD_COUNT_STRU *)UCOM_GetUncachedAddr(OM_CPULOAD_GetLoadCntAddr());
    uwTimeUsed = DRV_TIMER_ReadSysTimeStamp() - pstLoadCnt->auwEnterTS[uwFuncId];

    /* ���µ��ô��� */
    pstLoadCnt->auwEnterTimes[uwFuncId] = pstLoadCnt->auwEnterTimes[uwFuncId] + 1;

    /* ��������ʱ */
    if(uwTimeUsed > pstLoadCnt->auwMaxTime[uwFuncId])
    {
        pstLoadCnt->auwMaxTime[uwFuncId] = uwTimeUsed;
    }

    /* ������̺�ʱ */
    if((uwTimeUsed < pstLoadCnt->auwMinTime[uwFuncId]) || (0 == pstLoadCnt->auwMinTime[uwFuncId]))
    {
        pstLoadCnt->auwMinTime[uwFuncId] = uwTimeUsed;
    }
}












#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

