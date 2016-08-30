



/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "RrmDebug.h"



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_RRM_DEBUG_C

#if (FEATURE_ON == FEATURE_DSDS)

RRM_STATS_INFO_STRU                     g_stRrmStats;



VOS_VOID  RRM_InitDebugInfo(VOS_VOID)
{

    PS_MEM_SET(&g_stRrmStats, 0, sizeof(RRM_STATS_INFO_STRU));

    return;
}



VOS_VOID  RRM_ShowStatsInfo(VOS_VOID)
{
    VOS_UINT16                          usIndex;


    for (usIndex = 0; usIndex < MODEM_ID_BUTT; usIndex++)
    {
        vos_printf("========MODEM Msg info for Index %d ========\n", usIndex);
        vos_printf("����RFID�Ĵ���:          %d \n", g_stRrmStats.astRrmModemStats[usIndex].ulRrmRcvResApplyNum);
        vos_printf("�ظ�RFID����Ĵ���:      %d \n", g_stRrmStats.astRrmModemStats[usIndex].ulRrmSndApplyCnfNum);
        vos_printf("�ͷ�RFID�Ĵ���:          %d \n", g_stRrmStats.astRrmModemStats[usIndex].ulRrmRcvResReleaseNum);
        vos_printf("ע��Ĵ���:              %d \n", g_stRrmStats.astRrmModemStats[usIndex].ulRrmRcvRegNum);
        vos_printf("ȥע��Ĵ���:            %d \n", g_stRrmStats.astRrmModemStats[usIndex].ulRrmRcvDeregNum);
    }
    vos_printf("\r\n");
    vos_printf("����STATUS IND�Ĵ���:        %d \n", g_stRrmStats.ulRrmSndStatusIndNum);
    vos_printf("\r\n");

    return;

}



VOS_VOID RRM_Help(VOS_VOID)
{

    vos_printf("********************RRM�����Ϣ************************\n");
    vos_printf("RRM_ShowInfo          ��ʾRRMģ�鵱ǰ��Ϣ\n");
    vos_printf("RRM_ShowStatsInfo     ��ʾRRMģ��ͳ����Ϣ\n");

    return;
}


#endif /* FEATURE_ON == FEATURE_DSDS */


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

