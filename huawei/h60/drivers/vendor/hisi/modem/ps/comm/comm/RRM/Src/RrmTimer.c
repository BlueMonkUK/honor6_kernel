



/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "RrmTimer.h"
#include "Rrm.h"
#include "RrmDebug.h"



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_RRM_TIMER_C


#if (FEATURE_ON == FEATURE_DSDS)

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_UINT32 RRM_StartTimer
(
    RRM_RFID_ENUM_UINT16                enRFIDTimerId
)
{
    RRM_RFID_TIMER_CTRL_STRU           *pstRFIDTimerAddr;
    VOS_UINT32                          ulRslt;


    /*��ȡ��״̬��ʱ�����ƿ��ַ*/
    pstRFIDTimerAddr       = RRM_GetRFIDTimerAddr(enRFIDTimerId);

    /*�޷���ȡ�ö�ʱ�����ƿ��ַ��˵����ʱ��ID�Ƿ�*/
    if( VOS_NULL_PTR == pstRFIDTimerAddr)
    {
        /* ��ӡ������Ϣ */
        RRM_WARNING_LOG1(UEPS_PID_RRM, "TimerId is Error!", enRFIDTimerId);
        return VOS_ERR;
    }

    /*����״̬��ʱ���Ƿ������У�����������У���ֹͣ�ö�ʱ����
      ֹͣ��ʱ��ʱ��VOS��ֱ�ӽ��ö�ʱ��������ΪVOS_NULL_PTR*/
    if(VOS_NULL_PTR != pstRFIDTimerAddr->pstStaTHandle)
    {
        VOS_StopRelTimer(&(pstRFIDTimerAddr->pstStaTHandle));
    }

    /* ��ʱ��ID���� */
    pstRFIDTimerAddr->enRFIDTimerId = enRFIDTimerId;

    /* ��ʱ���޲�����Ϣ��������ʱ��ʱ����ʱ����������Ϊ0
      ��ѭ����ʱ������ʱ��ģʽΪVOS_RELTIMER_NOLOOP*/
    ulRslt        = VOS_StartRelTimer( &(pstRFIDTimerAddr->pstStaTHandle),
                                        UEPS_PID_RRM,
                                        RRM_TMR_DEF_LEN,
                                        pstRFIDTimerAddr->enRFIDTimerId,
                                        0,
                                        VOS_RELTIMER_NOLOOP,
                                        VOS_TIMER_PRECISION_0);

    /*������õ���VOS�Ľӿں����������Ҫʹ��VOS�ķ���ֵ���м��*/
    if( VOS_OK != ulRslt)
    {
        /* ����ʧ�ܣ���ӡʧ����Ϣ */
        RRM_ERROR_LOG1(UEPS_PID_RRM, "Start TimerId Failure!", enRFIDTimerId);
        return VOS_ERR;
    }

    return VOS_OK;
}



VOS_UINT32 RRM_StopTimer
(
    RRM_RFID_ENUM_UINT16                enRFIDTimerId
)
{
    RRM_RFID_TIMER_CTRL_STRU           *pstRFIDTimerAddr;


    /*��ȡ��״̬��ʱ�����ƿ��ַ*/
    pstRFIDTimerAddr       = RRM_GetRFIDTimerAddr(enRFIDTimerId);

    /*�޷���ȡ�ö�ʱ�����ƿ飬˵����ʱ��ID�Ƿ�*/
    if(VOS_NULL_PTR == pstRFIDTimerAddr)
    {
        /* ��ӡ������Ϣ */
        RRM_WARNING_LOG1(UEPS_PID_RRM, "TimerId is Error!", enRFIDTimerId);
        return VOS_ERR;
    }

    /* ��ʱ����ID��ʶ�Ƿ�һ�£������һ�£�ֱ�ӷ��� */
    if (enRFIDTimerId != pstRFIDTimerAddr->enRFIDTimerId)
    {
        /* ��ӡ������Ϣ */
        RRM_WARNING_LOG2(UEPS_PID_RRM,
                       "enRFIDTimerId is <1>, RFIDTimerId is <2>!",
                       enRFIDTimerId, pstRFIDTimerAddr->enRFIDTimerId);

        return VOS_ERR;
    }

    /*����״̬��ʱ���Ƿ������У�����������У�ֹͣ�ö�ʱ����
      ֹͣ��ʱ��ʱ��VOS��ֱ�ӽ��ö�ʱ��������ΪVOS_NULL_PTR*/
    if(VOS_NULL_PTR != pstRFIDTimerAddr->pstStaTHandle)
    {
        VOS_StopRelTimer(&(pstRFIDTimerAddr->pstStaTHandle));

        /* Timer ID��� */
        pstRFIDTimerAddr->enRFIDTimerId = RRM_RFID_BUTT;
    }

    return VOS_OK;
}

#endif /* FEATURE_ON == FEATURE_DSDS */

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

