


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/

#include "CDS.h"
#include "QosFcOm.h"
#include "QosFcStatus.h"
#include "QosFcRabStatus.h"
#include "QosFcChannelFlowCtrl.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_QOS_FC_OM_C
/*lint +e767*/




/*****************************************************************************
  2 �ⲿ��������
*****************************************************************************/

/******************************************************************************
   3 ˽�ж���
******************************************************************************/

/******************************************************************************
   4 ȫ�ֱ�������
******************************************************************************/
QOS_FC_STATS_INFO_STRU g_stQosFcStats = {0};

extern VOS_INT32 g_iRabEntityCnt;
extern QOS_FC_RAB_ENTITY_STRU g_stQosFcRabEntity[FC_MAX_RAB_NUM];
extern QOS_FC_RAB_STATE_ENUM_UINT32 g_enRabState[MODEM_ID_BUTT][FC_MAX_RAB_NUM];
extern QOS_FC_CHANNEL_ENTITY_STRU g_stQosFcChannelEntity[FC_MAX_RAB_NUM];



/******************************************************************************
   5 ����ʵ��
******************************************************************************/

VOS_VOID QosFc_ShowState(VOS_VOID)
{
    vos_printf("QoS����״̬     %d\r\n", g_stQosFcCtrl.enFlowCtrl);
    return;
}


VOS_VOID QosFc_ShowRabState(VOS_VOID)
{
    VOS_UINT32  i,k;
    VOS_UINT32  j=5;

    vos_printf("QoS����RAB״̬\r\n");
    for(k = 0; k < MODEM_ID_BUTT; k++)
    {
        for(i = 0; i < FC_MAX_RAB_NUM; )
        {
            vos_printf("RabId:   %2d  State:  %d",  i + 5, g_enRabState[k][i]);
            i++;
            while(--j)
            {
                if(i < FC_MAX_RAB_NUM)
                {
                	vos_printf(" :  %d", g_enRabState[k][i++]);
                }
            }
            j = 5;
            vos_printf("\r\n");
        }
        vos_printf("\r\n");
    }

    vos_printf("\r\n");
    return;
}


VOS_VOID QosFc_ShowEntity(VOS_VOID)
{
    VOS_UINT32  i;

    vos_printf("QoS����ʵ��״̬: %d\r\n", g_iRabEntityCnt);
    for(i = 0; i < FC_MAX_RAB_NUM; i++)
    {
        if(FC_INVALID_RABID == g_stQosFcRabEntity[i].ucRabId)
        {
            break;
        }
        vos_printf("Entity:    %2d  QCI:  %d  Priority: %d  LinkRabId:  %2d  State: %d\r\n",
                            g_stQosFcRabEntity[i].ucRabId,
                            g_stQosFcRabEntity[i].enQci,
                            g_stQosFcRabEntity[i].ucPriority,
                            g_stQosFcRabEntity[i].ucLinkRabId,
                            g_stQosFcRabEntity[i].enRabState);
    }
    vos_printf("\r\n");
    return;
}


VOS_VOID QosFc_ShowChannel(VOS_VOID)
{
    VOS_UINT32  i;

    vos_printf("QoS����ͨ��״̬\r\n");
    for(i = 0; i < FC_MAX_RAB_NUM; i++)
    {
        if(FC_INVALID_RABID == g_stQosFcChannelEntity[i].ucRabId)
        {
            break;
        }
        vos_printf("Channel:    %2d  QCI:  %d  State : %d\r\n",
                            g_stQosFcChannelEntity[i].ucRabId,
                            g_stQosFcChannelEntity[i].enQci,
                            g_stQosFcChannelEntity[i].enChannelState);
    }
    vos_printf("\r\n");
    return;
}



VOS_VOID QosFc_ShowPara(VOS_VOID)
{
    vos_printf("QoS���ز��� :\r\n");
    vos_printf("\r\n");
    vos_printf("QoS�����Ƿ�ʹ��             %s\r\n", g_stQosFcCtrl.bQosFcEnable?"Yes":"No");
    vos_printf("������ȼ������Ƿ�����      %s\r\n", g_stQosFcCtrl.bHighFcEnable?"Yes":"No");
    vos_printf("����QoS���ذ���             %d\r\n", g_stQosFcCtrl.ulPktCntLimit);
    vos_printf("����QoS����ʱ��             %d\r\n", g_stQosFcCtrl.ulTimerLen);
    vos_printf("���������                  %d\r\n", g_stQosFcCtrl.ulRandomDiscardRate);
    vos_printf("������                      %d\r\n", g_stQosFcCtrl.ulDiscardRate);
    vos_printf("������ֵ                    %d\r\n", g_stQosFcCtrl.ulWarningThres);
    vos_printf("������ֵ                    %d\r\n", g_stQosFcCtrl.ulDiscardThres);
    vos_printf("���������ֵ                %d\r\n", g_stQosFcCtrl.ulRandomDiscardThres);
    vos_printf("�ָ���ֵ                    %d\r\n", g_stQosFcCtrl.ulRestoreThres);

    vos_printf("\r\n");
    return;
}


VOS_VOID QosFc_OutPutRabStats(CONST VOS_UINT32 *aulData)
{
    VOS_UINT32 i = 0;
    VOS_UINT32 j = 5;

    for(i = 0; i < FC_MAX_RAB_NUM; )
    {
        vos_printf("%10d",  aulData[i++]);
        while(--j)
        {
            if(i < FC_MAX_RAB_NUM)
            {
                vos_printf("  %6d", aulData[i++]);
            }
        }
        j = 5;
        vos_printf("\r\n");
    }
    return;
}


VOS_VOID QosFc_ShowStats(VOS_VOID)
{
    VOS_UINT32 i = 0;
    vos_printf("QoS����ͳ�� :\r\n");
    vos_printf("\r\n");
    vos_printf("�յ�CREATE��Ϣ����      %d\r\n", g_stQosFcStats.ulCreateMsgNum);
    vos_printf("�յ�RELEASE��Ϣ����     %d\r\n", g_stQosFcStats.ulReleaseMsgNum);
    vos_printf("�������ش���            %d\r\n", g_stQosFcStats.ulCreateRabNum);
    vos_printf("���³��ش���            %d\r\n", g_stQosFcStats.ulUpdateRabNum);
    vos_printf("�ͷų��ش���            %d\r\n", g_stQosFcStats.ulReleaseRabNum);
    vos_printf("����LTE��������         %d\r\n", g_stQosFcStats.ulUlLteDiscardTotalNum);
    vos_printf("����GU��������          %d\r\n", g_stQosFcStats.ulUlGUDiscardTotalNum);
    vos_printf("����LTE��������         %d\r\n", g_stQosFcStats.ulDlLteDiscardTotalNum);
    vos_printf("����GU��������          %d\r\n", g_stQosFcStats.ulDlGUDiscardTotalNum);

    vos_printf("����ÿ�����ؽ����������״̬����     \r\n");
    for(i = 0; i < MODEM_ID_BUTT; i++)
    {
        QosFc_OutPutRabStats(g_stQosFcStats.aulToRandomDiscardNum[i]);
    }

    vos_printf("����ÿ�����ؽ��붪��״̬����        \r\n");
    for(i = 0; i < MODEM_ID_BUTT; i++)
    {
        QosFc_OutPutRabStats(g_stQosFcStats.aulToDiscardNum[i]);
    }

    vos_printf("����ÿ�����ػָ�����״̬����       \r\n");
    for(i = 0; i < MODEM_ID_BUTT; i++)
    {
        QosFc_OutPutRabStats(g_stQosFcStats.aulToNormalNum[i]);
    }

    vos_printf("����ÿ�������϶���״̬ʱ�հ�����       \r\n");
    for(i = 0; i < MODEM_ID_BUTT; i++)
    {
        QosFc_OutPutRabStats(g_stQosFcStats.aulRcvPktNumOnD[i]);
    }

    vos_printf("����ÿ�������϶���״̬ʱ��������       \r\n");
    for(i = 0; i < MODEM_ID_BUTT; i++)
    {
        QosFc_OutPutRabStats(g_stQosFcStats.aulDiscardPktNumOnD[i]);
    }

    vos_printf("����ÿ���������������״̬ʱ�հ�����       \r\n");
    for(i = 0; i < MODEM_ID_BUTT; i++)
    {
        QosFc_OutPutRabStats(g_stQosFcStats.aulRcvPktNumOnRD[i]);
    }

    vos_printf("����ÿ���������������״̬ʱ��������      \r\n");
    for(i = 0; i < MODEM_ID_BUTT; i++)
    {
        QosFc_OutPutRabStats(g_stQosFcStats.aulDiscardPktNumOnRD[i]);
    }

    vos_printf("\r\n");
    vos_printf("ͨ�����ش���       \r\n");
    for(i = 0; i < MODEM_ID_BUTT; i++)
    {
        QosFc_OutPutRabStats(g_stQosFcStats.aulChannelToFc[i]);
    }

    vos_printf("��ͨ�����ش���       \r\n");
    for(i = 0; i < MODEM_ID_BUTT; i++)
    {
        QosFc_OutPutRabStats(g_stQosFcStats.aulChannelResume[i]);
    }

    vos_printf("\r\n");

    return;
}


VOS_VOID QosFc_ResetStats(VOS_VOID)
{
    PS_MEM_SET(&g_stQosFcStats, 0, sizeof(QOS_FC_STATS_INFO_STRU));
    return;

}



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif



