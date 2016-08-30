


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include  "NasLmmPubMTimer.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASMMPUBMTMER_C
/*lint +e767*/

/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/


/*****************************************************************************
  3 Function
*****************************************************************************/
/*****************************************************************************
  3.1 ״̬��ʱ�� Function
*****************************************************************************/


/*lint -e960*/
/*lint -e961*/
VOS_VOID    NAS_LMM_StartStateTimer(
                    NAS_LMM_STATE_TI_ENUM_UINT16             enStateTimerId )
{
    NAS_LMM_TIMER_CTRL_STRU              *pstStateTimer;
    VOS_UINT32                          ulRslt;
    VOS_UINT32                          ulParam;            /* ��ʱ�����ͱ�ʶ */

    /*��ȡ��״̬��ʱ�����ƿ��ַ*/
    pstStateTimer = NAS_LMM_GetStateTimerAddr(enStateTimerId);
    /*�޷���ȡ�ö�ʱ�����ƿ��ַ��˵����ʱ��ID�Ƿ�*/
    if ( VOS_NULL_PTR == pstStateTimer )
    {
        NAS_LMM_PUBM_LOG_ERR("NAS_LMM_StartStateTimer, pstStateTimer = 0");
        return;
    }

    /*����״̬��ʱ���Ƿ������У�����������У���ֹͣ�ö�ʱ����
      ֹͣ��ʱ��ʱ��VOS��ֱ�ӽ��ö�ʱ��������ΪVOS_NULL_PTR*/
    if ( VOS_NULL_PTR != pstStateTimer->psthTimer )
    {
        ulRslt = PS_STOP_REL_TIMER(&(pstStateTimer->psthTimer));
        /*������õ���VOS�Ľӿں����������Ҫʹ��VOS�ķ���ֵ���м��*/
        if ( VOS_OK != ulRslt )
        {
            NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_StartStateTimer, Start State Timer Failure!",
                                 enStateTimerId);
            return;
        }
    }

    /*���ʱ��Ϊ�㣬ֱ�ӷ��أ���������ʱ��*/
    if(0 == pstStateTimer->ulTimerLen)
    {
        NAS_LMM_PUBM_LOG1_WARN("NAS_LMM_StartStateTimer:TimerLen = 0; enStaTimerId = ",
                               enStateTimerId);
        return;
    }

    pstStateTimer->ucTimerType          = NAS_LMM_STATE_TIMER;
    ulParam                             = pstStateTimer->ucTimerType;

    /*
    */
    /*NAS-MM״̬��ʱ���޲�����Ϣ��������ʱ��ʱ����ʱ����������ΪulParam
      ״̬��ʱ�����Ƿ�ѭ����ʱ������ʱ��ģʽΪVOS_RELTIMER_NOLOOP*/
    ulRslt = PS_START_REL_TIMER(&(pstStateTimer->psthTimer),
                                        PS_PID_MM,
                                        pstStateTimer->ulTimerLen,
                                        enStateTimerId,
                                        ulParam,
                                        VOS_RELTIMER_NOLOOP);
    /*������õ���VOS�Ľӿں����������Ҫʹ��VOS�ķ���ֵ���м��*/
    if ( VOS_OK != ulRslt )
    {
        NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_StartStateTimer, Start State Timer Failure!",
                             enStateTimerId);
        return;
    }

    pstStateTimer->ucTimerRs            = NAS_LMM_TIMER_RS_RUNNING;
    pstStateTimer->ucTimerSs            = NAS_LMM_TIMER_SS_NOT_SUSPENDING;

    /*
    NAS_LMM_PUBM_LOG2_NORM("NAS_LMM_StartStateTimer OK. StateTimerId: Timer Len:",
                           enStateTimerId,pstStateTimer->ulTimerLen);
    */


    /* ��ʱ��״̬�������� */
    NAS_LMM_SndOmEmmTimerStatus(NAS_LMM_TIMER_RUNNING, (VOS_UINT16)enStateTimerId
                                , pstStateTimer->ulTimerLen);

    return;
}
VOS_VOID    NAS_LMM_StopStateTimer(
                    NAS_LMM_STATE_TI_ENUM_UINT16             enStateTimerId )
{
    NAS_LMM_TIMER_CTRL_STRU              *pstStateTimer;
    VOS_UINT32                          ulRslt;
    VOS_UINT32                          ulTimerRemainLen;

    /*��ȡ��״̬��ʱ�����ƿ��ַ*/
    pstStateTimer = NAS_LMM_GetStateTimerAddr(enStateTimerId);
    /*�޷���ȡ�ö�ʱ�����ƿ飬˵����ʱ��ID�Ƿ�*/
    if ( VOS_NULL_PTR == pstStateTimer )
    {
        NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_StopStateTimer, pstStateTimer = 0.enStateTimerId: ",
                             enStateTimerId);
        return;
    }

    /*����״̬��ʱ���Ƿ������У�����������У�ֹͣ�ö�ʱ����
      ֹͣ��ʱ��ʱ��VOS��ֱ�ӽ��ö�ʱ��������ΪVOS_NULL_PTR*/
    if ( VOS_NULL_PTR != pstStateTimer->psthTimer )
    {
        /* ��ȡ��ǰ��ʱ����ʣ��ʱ�� */
        if (VOS_OK != VOS_GetRelTmRemainTime(&(pstStateTimer->psthTimer), &ulTimerRemainLen ))
        {
            ulTimerRemainLen = 0;
        }
        ulRslt = PS_STOP_REL_TIMER(&(pstStateTimer->psthTimer));
        /*������õ���VOS�Ľӿں����������Ҫʹ��VOS�ķ���ֵ���м��*/
        if ( VOS_OK != ulRslt )
        {
            NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_StopStateTimer, Stop State Timer Failure!",
                                 enStateTimerId);
            return;
        }

        pstStateTimer->ucTimerRs        = NAS_LMM_TIMER_RS_NOT_RUNNING;
        pstStateTimer->ucTimerSs        = NAS_LMM_TIMER_SS_NOT_SUSPENDING;

        /* ��ʱ��״̬�������� */
        NAS_LMM_SndOmEmmTimerStatus(NAS_LMM_TIMER_STOPED, (VOS_UINT16)enStateTimerId
                                    , ulTimerRemainLen);

    }
    return;
}
VOS_VOID    NAS_LMM_SuspendStateTimer(
                    NAS_LMM_STATE_TI_ENUM_UINT16             enStateTimerId )
{
    NAS_LMM_TIMER_CTRL_STRU              *pstStateTimer;
    VOS_UINT32                          ulTick;
    VOS_UINT32                          ulRslt;

    /*��ȡ��״̬��ʱ�����ƿ��ַ*/
    pstStateTimer = NAS_LMM_GetStateTimerAddr(enStateTimerId);
    /*�޷���ȡ�ö�ʱ�����ƿ��ַ��˵����ʱ��ID�Ƿ�*/
    if ((VOS_NULL_PTR == pstStateTimer))
    {
        NAS_LMM_PUBM_LOG_ERR("NAS_LMM_SuspendStateTimer, pstStateTimer = 0");
        return;
    }

    /*����״̬��ʱ���Ƿ������У�����������У��Ȼ�ȡ��ʱ����ʣ��ʱ����
      ֹͣ��ʱ��ʱ��VOS��ֱ�ӽ��ö�ʱ��������ΪVOS_NULL_PTR*/
    if ( VOS_NULL_PTR != pstStateTimer->psthTimer )
    {
        ulRslt = VOS_GetRelTmRemainTime(&(pstStateTimer->psthTimer),
                                        &ulTick);
        /*������õ���VOS�Ľӿں����������Ҫʹ��VOS�ķ���ֵ���м��*/
        if ( VOS_OK != ulRslt )
        {
            NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_SuspendStateTimer,VOS_GetRelTmRemainTime.enStateTimerId: ",
                                 enStateTimerId);
            return;
        }

        ulRslt = PS_STOP_REL_TIMER(&(pstStateTimer->psthTimer));
        /*������õ���VOS�Ľӿں����������Ҫʹ��VOS�ķ���ֵ���м��*/
        if ( VOS_OK != ulRslt )
        {
            NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_SuspendStateTimer,VOS_StopRelTimer.enStateTimerId: ",
                                 enStateTimerId);
            return;
        }

        pstStateTimer->ucTimerType      = NAS_LMM_STATE_TIMER;
        pstStateTimer->ucTimerSs        = NAS_LMM_TIMER_SS_SUSPENDING;
        pstStateTimer->ulTimerRemainLen = VOS_TmTickToMillSec(ulTick);
    }

    return;
}


VOS_VOID    NAS_LMM_ResumeStateTimer(
                    NAS_LMM_STATE_TI_ENUM_UINT16             enStateTimerId )
{
    NAS_LMM_TIMER_CTRL_STRU              *pstStateTimer;
    VOS_UINT32                          ulRslt;
    VOS_UINT32                          ulParam;            /* ��ʱ�����ͱ�ʶ */

    /*��ȡ��״̬��ʱ�����ƿ��ַ*/
    pstStateTimer = NAS_LMM_GetStateTimerAddr(enStateTimerId);
    /*�޷���ȡ�ö�ʱ�����ƿ��ַ��˵����ʱ��ID�Ƿ�*/
    if ( VOS_NULL_PTR == pstStateTimer )
    {
        NAS_LMM_PUBM_LOG_ERR("NAS_LMM_ResumeStateTimer, pstStateTimer = 0");
        return;
    }

    /*����״̬��ʱ���Ƿ񱻹���*/
    if (    (VOS_NULL_PTR               == pstStateTimer->psthTimer)
         && (NAS_LMM_TIMER_RS_NOT_RUNNING == pstStateTimer->ucTimerRs)
         && (NAS_LMM_TIMER_SS_SUSPENDING == pstStateTimer->ucTimerSs) )
    {
        pstStateTimer->ucTimerType      = NAS_LMM_STATE_TIMER;
        ulParam                         = pstStateTimer->ucTimerType;

        /*NAS-MM״̬��ʱ���޲�����Ϣ��������ʱ��ʱ����ʱ����������ΪulParam
          ״̬��ʱ�����Ƿ�ѭ����ʱ������ʱ��ģʽΪVOS_RELTIMER_NOLOOP*/
        ulRslt = PS_START_REL_TIMER(&(pstStateTimer->psthTimer),
                                        PS_PID_MM,
                                        pstStateTimer->ulTimerRemainLen,
                                        enStateTimerId,
                                        ulParam,
                                        VOS_RELTIMER_NOLOOP );
        /*������õ���VOS�Ľӿں����������Ҫʹ��VOS�ķ���ֵ���м��*/
        if ( VOS_OK != ulRslt )
        {
            NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_ResumeStateTimer, Start State Timer Failure!",
                                 enStateTimerId);
            return;
        }

         /* ͬʱ�޸�g_stMmMainContext�ж�Ӧ��ʱ�������б�ʶ,��
        g_stMmMainContext.astMmTimerSta */
        pstStateTimer->ucTimerRs              = NAS_LMM_TIMER_RS_RUNNING;
    }
    else
    {
        NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_ResumeStateTimer, State Timer is not suspended!",
                             enStateTimerId);
        return;
    }

    pstStateTimer->ucTimerSs            = NAS_LMM_TIMER_SS_NOT_SUSPENDING;
    pstStateTimer->ulTimerRemainLen     = NAS_LMM_TIMER_ZERO_VALUE;

    return;
}


VOS_VOID    NAS_LMM_ModifyStateTimer(
                    NAS_LMM_STATE_TI_ENUM_UINT16             enStateTimerId,
                    VOS_UINT32                              ulTimerLen )
{
    NAS_LMM_TIMER_CTRL_STRU              *pstStateTimer;

    pstStateTimer = NAS_LMM_GetStateTimerAddr(enStateTimerId);
    /*�޷���ȡ�ö�ʱ�����ƿ飬˵����ʱ��ID�Ƿ�*/
    if ( VOS_NULL_PTR == pstStateTimer )
    {
        NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_ModifyStateTimer, pstStateTimer = 0.enStateTimerId: ",
                             enStateTimerId);
        return;
    }

    pstStateTimer->ulTimerLen           = ulTimerLen;
    pstStateTimer->ucTimerType          = NAS_LMM_STATE_TIMER;

    NAS_LMM_PUBM_LOG2_NORM("NAS_LMM_ModifyStateTimer OK. StateTimerId, StateTimerLength",
                          enStateTimerId,
                          pstStateTimer->ulTimerLen );

    return;
}
VOS_VOID    NAS_LMM_StopAllStateTimer( VOS_VOID )
{
    NAS_LMM_TIMER_CTRL_STRU              *pstStateTimerList;
    VOS_UINT32                          ulTcbIdxLoop;
    VOS_UINT32                          ulRslt;

    /*��ȡ״̬��ʱ���б��׵�ַ*/
    pstStateTimerList = NAS_LMM_GetStateTimerListAddr();

    /*ֹͣ���д�������״̬��Э�鶨ʱ��*/
    for ( ulTcbIdxLoop = TI_NAS_EMM_STATE_NO_TIMER + 1; ulTcbIdxLoop < NAS_LMM_STATE_TI_BUTT; ulTcbIdxLoop++ )
    {
        /*����״̬��ʱ���Ƿ������У�����������У�ֹͣ�ö�ʱ����
          ֹͣ��ʱ��ʱ��VOS��ֱ�ӽ��ö�ʱ��������ΪVOS_NULL_PTR*/
        if ( VOS_NULL_PTR != pstStateTimerList[ulTcbIdxLoop].psthTimer )
        {
            ulRslt = PS_STOP_REL_TIMER(&(pstStateTimerList[ulTcbIdxLoop].psthTimer));
            /*������õ���VOS�Ľӿں����������Ҫʹ��VOS�ķ���ֵ���м��*/
            if ( VOS_OK != ulRslt )
            {
                /* ����ʧ�ܣ���ӡʧ����Ϣ */
                NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_StopAllStateTimer, Stop pstStateTimerList[%d] Failure!",
                                     ulTcbIdxLoop);
            }

            pstStateTimerList[ulTcbIdxLoop].ucTimerRs = NAS_LMM_TIMER_RS_NOT_RUNNING;
            pstStateTimerList[ulTcbIdxLoop].ucTimerSs = NAS_LMM_TIMER_SS_NOT_SUSPENDING;
        }
    }

    return;
}
/*****************************************************************************
 Function Name   : NAS_LMM_SuspendInitStopPtlTimer
 Description     : �ڹ���ʱֹͣ��3412,3402,3423�������Э�鶨ʱ��
 Input           : VOS_VOID
 Output          : None
 Return          : None

 History         :
    1.FTY          2012-02-15  Draft Enact

*****************************************************************************/
VOS_VOID    NAS_LMM_SuspendInitStopPtlTimer (VOS_VOID )
{
    NAS_LMM_TIMER_CTRL_STRU              *pstPtlTimerList;
    VOS_UINT32                          ulTcbIdxLoop;
    VOS_UINT32                          ulRslt;

    /*��ȡ״̬��ʱ���б��׵�ַ*/
    pstPtlTimerList = NAS_LMM_GetPtlTimerListAddr();


    NAS_LMM_PUBM_LOG1_INFO("NAS_LMM_SuspendInitStopPtlTimer. 3412",
                                      TI_NAS_EMM_PTL_T3412);
   NAS_LMM_PUBM_LOG1_INFO("NAS_LMM_SuspendInitStopPtlTimer. 3402",
                                      TI_NAS_EMM_PTL_T3402);

    /*ֹͣ���д�������״̬��Э�鶨ʱ��*/
    for (   ulTcbIdxLoop = TI_NAS_LMM_PTL_TI_PUB_BOUNDARY_START +1;
            ulTcbIdxLoop < NAS_LMM_PTL_TI_BUTT;
            ulTcbIdxLoop++ )
    {
        /*����Э�鶨ʱ���Ƿ������У�����������У�ֹͣ�ö�ʱ����
         ֹͣ��ʱ��ʱ��VOS��ֱ�ӽ��ö�ʱ��������ΪVOS_NULL_PTR*/
        if ( VOS_NULL_PTR != pstPtlTimerList[ulTcbIdxLoop].psthTimer )
        {
            /* ��ֹͣT3411,T3412,T3423,T3402, T3416*/
            if(  (TI_NAS_EMM_PTL_T3411 == (pstPtlTimerList[ulTcbIdxLoop].ulName))
               ||(TI_NAS_EMM_PTL_T3412 == (pstPtlTimerList[ulTcbIdxLoop].ulName))
               ||(TI_NAS_EMM_PTL_T3402 == (pstPtlTimerList[ulTcbIdxLoop].ulName))
               ||(TI_NAS_EMM_PTL_T3423 == (pstPtlTimerList[ulTcbIdxLoop].ulName))
               ||(TI_NAS_EMM_PTL_T3416 == (pstPtlTimerList[ulTcbIdxLoop].ulName)))
            {
                NAS_LMM_PUBM_LOG1_INFO("NAS_LMM_SuspendInitStopPtlTimer. ",
                                      pstPtlTimerList[ulTcbIdxLoop].ulName);

                continue;
            }

            ulRslt = PS_STOP_REL_TIMER(&(pstPtlTimerList[ulTcbIdxLoop].psthTimer));

            /*������õ���VOS�Ľӿں����������Ҫʹ��VOS�ķ���ֵ���м��*/
            if ( VOS_OK != ulRslt )
            {
                NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_StopAllPtlTimer, Stop pstPtlTimerList[%d] Failure!",
                                     ulTcbIdxLoop);
            }

            pstPtlTimerList[ulTcbIdxLoop].ucTimerRs = NAS_LMM_TIMER_RS_NOT_RUNNING;
            pstPtlTimerList[ulTcbIdxLoop].ucTimerSs = NAS_LMM_TIMER_SS_NOT_SUSPENDING;
        }
    }

    return;
}

/*****************************************************************************
 Function Name   : NAS_LMM_StopAllStateTimer
 Description     : �ڹ���ʱֹͣ��3411�������״̬��ʱ��
 Input           : VOS_VOID
 Output          : None
 Return          : None

 History         :
    1.FTY          2012-02-15  Draft Enact

*****************************************************************************/
VOS_VOID    NAS_LMM_SuspendInitStopStateTimer (VOS_VOID )
{
    NAS_LMM_TIMER_CTRL_STRU            *pstStateTimerList;
    VOS_UINT32                          ulTcbIdxLoop;
    VOS_UINT32                          ulRslt;

    /*��ȡ״̬��ʱ���б��׵�ַ*/
    pstStateTimerList = NAS_LMM_GetStateTimerListAddr();

    /*ֹͣ��STATE_SERVICE_T3442�Լ�STATE_DEL_FORB_TA_PROID�����д�������״̬��Э�鶨ʱ��*/
    for ( ulTcbIdxLoop = TI_NAS_EMM_STATE_NO_TIMER + 1; ulTcbIdxLoop < NAS_LMM_STATE_TI_BUTT; ulTcbIdxLoop++ )
    {
        /*����״̬��ʱ���Ƿ������У�����������У�ֹͣ�ö�ʱ����
          ֹͣ��ʱ��ʱ��VOS��ֱ�ӽ��ö�ʱ��������ΪVOS_NULL_PTR*/
        if ( VOS_NULL_PTR != pstStateTimerList[ulTcbIdxLoop].psthTimer )
        {
            /* ����STATE_SERVICE_T3442�Լ�STATE_DEL_FORB_TA_PROID����ͣ��ʱ���������ж�ʣ�ඨʱ�� */
            if( (TI_NAS_EMM_STATE_SERVICE_T3442 == (pstStateTimerList[ulTcbIdxLoop].ulName))
                || (TI_NAS_EMM_STATE_DEL_FORB_TA_PROID == (pstStateTimerList[ulTcbIdxLoop].ulName)))
            {
                NAS_LMM_PUBM_LOG1_INFO("NAS_LMM_StopAllStateTimerExcept3411. ",
                                      pstStateTimerList[ulTcbIdxLoop].ulName);
                continue;
            }

            ulRslt = PS_STOP_REL_TIMER(&(pstStateTimerList[ulTcbIdxLoop].psthTimer));
            /*������õ���VOS�Ľӿں����������Ҫʹ��VOS�ķ���ֵ���м��*/
            if ( VOS_OK != ulRslt )
            {
                /* ����ʧ�ܣ���ӡʧ����Ϣ */
                NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_StopAllStateTimer, Stop pstStateTimerList[%d] Failure!",
                                     ulTcbIdxLoop);
            }

            pstStateTimerList[ulTcbIdxLoop].ucTimerRs = NAS_LMM_TIMER_RS_NOT_RUNNING;
            pstStateTimerList[ulTcbIdxLoop].ucTimerSs = NAS_LMM_TIMER_SS_NOT_SUSPENDING;
        }
    }

    return;
}
/*****************************************************************************
  3.2 Э�鶨ʱ�� Function
*****************************************************************************/

VOS_VOID    NAS_LMM_StartPtlTimer(
                    NAS_LMM_PTL_TI_ENUM_UINT16               enPtlTimerId )
{
    NAS_LMM_TIMER_CTRL_STRU              *pstPtlTimer;
    VOS_UINT32                          ulRslt;
    VOS_UINT32                          ulParam;            /* ��ʱ�����ͱ�ʶ */

    /*��ȡ��Э�鶨ʱ�����ƿ��ַ*/
    pstPtlTimer = NAS_LMM_GetPtlTimerAddr(enPtlTimerId);
    /*�޷���ȡ��Э�鶨ʱ�����ƿ��ַ��˵����Э�鶨ʱ��ID�Ƿ�*/
    if ( VOS_NULL_PTR == pstPtlTimer )
    {
        NAS_LMM_PUBM_LOG_ERR("NAS_LMM_StartPtlTimer, pstPtlTimer = 0");
        return;
    }

    /*����Э�鶨ʱ���Ƿ������У�����������У���ֹͣ��Э�鶨ʱ����
      ֹͣ��ʱ��ʱ��VOS��ֱ�ӽ��ö�ʱ��������ΪVOS_NULL_PTR*/
    if ( VOS_NULL_PTR != pstPtlTimer->psthTimer )
    {
        ulRslt = PS_STOP_REL_TIMER(&(pstPtlTimer->psthTimer));
        /*������õ���VOS�Ľӿں����������Ҫʹ��VOS�ķ���ֵ���м��*/
        if ( VOS_OK != ulRslt )
        {
            NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_StartPtlTimer, Start Ptl Timer Failure!",
                                 enPtlTimerId);
            return;
        }
    }

    /*��ʱ��ʱ�����ܳ���18Сʱ����Ϊ��*/
    if((TI_NAS_LMM_TIMER_MAX_LEN < pstPtlTimer->ulTimerLen)
    ||(0 == pstPtlTimer->ulTimerLen))
    {
        NAS_LMM_PUBM_LOG2_WARN("NAS_LMM_StartPtlTimer:TimerLen ERR; enPtlTimerId, TimerLength",
                              enPtlTimerId,
                              pstPtlTimer->ulTimerLen);
        return;
    }

    pstPtlTimer->ucTimerType            = NAS_LMM_PTL_TIMER;
    ulParam                             = pstPtlTimer->ucTimerType;

    /*NAS-MMЭ�鶨ʱ���޲�����Ϣ��������ʱ��ʱ����ʱ����������Ϊ0*/
    ulRslt = PS_START_REL_TIMER(&(pstPtlTimer->psthTimer),
                                        PS_PID_MM,
                                        pstPtlTimer->ulTimerLen,
                                        enPtlTimerId,
                                        ulParam,
                                        pstPtlTimer->ucMode);
    /*������õ���VOS�Ľӿں����������Ҫʹ��VOS�ķ���ֵ���м��*/
    if ( VOS_OK != ulRslt )
    {
        NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_StartPtlTimer, Start State Timer Failure!",
                             enPtlTimerId);
        return;
    }

    pstPtlTimer->ucTimerRs            = NAS_LMM_TIMER_RS_RUNNING;
    pstPtlTimer->ucTimerSs            = NAS_LMM_TIMER_SS_NOT_SUSPENDING;

    NAS_LMM_PUBM_LOG2_NORM("NAS_LMM_StartPtlTimer OK. PtlTimerId: Timer Len:",
                           enPtlTimerId,pstPtlTimer->ulTimerLen);

    /* ��ʱ��״̬�������� */
    NAS_LMM_SndOmEmmTimerStatus(NAS_LMM_TIMER_RUNNING, (VOS_UINT16)enPtlTimerId
                                , pstPtlTimer->ulTimerLen);



    return;
}
VOS_VOID    NAS_LMM_StopPtlTimer(
                    NAS_LMM_PTL_TI_ENUM_UINT16               enPtlTimerId )
{
    NAS_LMM_TIMER_CTRL_STRU              *pstPtlTimer;
    VOS_UINT32                          ulRslt;
    VOS_UINT32                          ulTimerRemainLen;

    /*��ȡ��Э�鶨ʱ�����ƿ��ַ*/
    pstPtlTimer = NAS_LMM_GetPtlTimerAddr(enPtlTimerId);
    /*�޷���ȡ��Э�鶨ʱ�����ƿ飬˵����Э�鶨ʱ��ID�Ƿ�*/
    if ( VOS_NULL_PTR == pstPtlTimer )
    {
        NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_StopPtlTimer, pstPtlTimer = 0.enPtlTimerId: ",
                             enPtlTimerId);
        return;
    }

    /*����Э�鶨ʱ���Ƿ������У�����������У�ֹͣ�ö�ʱ����
      ֹͣ��ʱ��ʱ��VOS��ֱ�ӽ��ö�ʱ��������ΪVOS_NULL_PTR*/
    if ( VOS_NULL_PTR != pstPtlTimer->psthTimer )
    {
        /* ��ȡ��ǰ��ʱ����ʣ��ʱ�� */
        if (VOS_OK != VOS_GetRelTmRemainTime(&(pstPtlTimer->psthTimer), &ulTimerRemainLen ))
        {
            ulTimerRemainLen = 0;
        }

        ulRslt = PS_STOP_REL_TIMER(&(pstPtlTimer->psthTimer));
        /*������õ���VOS�Ľӿں����������Ҫʹ��VOS�ķ���ֵ���м��*/
        if ( VOS_OK != ulRslt )
        {
            NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_StopPtlTimer, Stop Ptl Timer Failure!",
                                 enPtlTimerId);
            return;
        }

        pstPtlTimer->ucTimerRs = NAS_LMM_TIMER_RS_NOT_RUNNING;
        pstPtlTimer->ucTimerSs = NAS_LMM_TIMER_SS_NOT_SUSPENDING;

        /* ��ʱ��״̬�������� */
        NAS_LMM_SndOmEmmTimerStatus(NAS_LMM_TIMER_STOPED, (VOS_UINT16)enPtlTimerId
                                    , ulTimerRemainLen);


    }

    return;
}
VOS_VOID  NAS_LMM_StartInactivePtlTimer( NAS_LMM_PTL_TI_ENUM_UINT16           enPtlTimerId)
{
    NAS_LMM_TIMER_CTRL_STRU              *pstPtlTimer;

    /*��ȡ��Э�鶨ʱ�����ƿ��ַ*/
    pstPtlTimer = NAS_LMM_GetPtlTimerAddr(enPtlTimerId);

    /*�޷���ȡ��Э�鶨ʱ�����ƿ飬˵����Э�鶨ʱ��ID�Ƿ�*/
    if ( VOS_NULL_PTR == pstPtlTimer )
    {
        NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_StartInactivePtlTimer, pstPtlTimer = 0.enPtlTimerId: ",
                             enPtlTimerId);
        return;
    }

    /*����Э�鶨ʱ���Ƿ����У����������������*/
    if ( VOS_NULL_PTR == pstPtlTimer->psthTimer )
    {
        NAS_LMM_StartPtlTimer(enPtlTimerId);
    }

    return;
}
VOS_VOID  NAS_LMM_StartInactiveStateTimer( NAS_LMM_STATE_TI_ENUM_UINT16 enStateTimerId )
{
    NAS_LMM_TIMER_CTRL_STRU              *pstStateTimer;

    /*��ȡ��״̬��ʱ�����ƿ��ַ*/
    pstStateTimer = NAS_LMM_GetStateTimerAddr(enStateTimerId);

    /*�޷���ȡ�ö�ʱ�����ƿ��ַ��˵����ʱ��ID�Ƿ�*/
    if ( VOS_NULL_PTR == pstStateTimer )
    {
        NAS_LMM_PUBM_LOG_ERR("NAS_LMM_StartInactiveStateTimer, pstStateTimer = 0");
        return;
    }

    /*���״̬��ʱ���Ƿ����У����������������*/
    if ( VOS_NULL_PTR == pstStateTimer->psthTimer )
    {
        NAS_LMM_PUBM_LOG_NORM("NAS_LMM_StartInactiveStateTimer: timer");
        NAS_LMM_StartStateTimer(enStateTimerId);
    }

    return;
}
VOS_VOID    NAS_LMM_ModifyPtlTimer(
                    NAS_LMM_PTL_TI_ENUM_UINT16               enPtlTimerId,
                    VOS_UINT32                              ulTimerLen )
{
    NAS_LMM_TIMER_CTRL_STRU             *pstPtlTimer;

    pstPtlTimer = NAS_LMM_GetPtlTimerAddr(enPtlTimerId);

    /*�޷���ȡ��Э�鶨ʱ�����ƿ飬˵����Э�鶨ʱ��ID�Ƿ�*/
    if ( VOS_NULL_PTR == pstPtlTimer )
    {
        NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_ModifyPtlTimer Error: pstPtlTimer = 0.enPtlTimerId: ",
                             enPtlTimerId);
        return;
    }

    /*�޸� NVIM ʱ���ɹ����޸�ȫ�ֱ���*/
    pstPtlTimer->ulTimerLen           = ulTimerLen;
    pstPtlTimer->ucTimerType          = NAS_LMM_PTL_TIMER;

    /*
    NAS_LMM_PUBM_LOG2_NORM("NAS_LMM_ModifyPtlTimer OK. PtlTimerId, PtlTimerLength",
                          enPtlTimerId,
                          pstPtlTimer->ulTimerLen );
    */

    return;
}

VOS_VOID    NAS_LMM_StopAllPtlTimer( VOS_VOID )
{
    NAS_LMM_TIMER_CTRL_STRU              *pstPtlTimerList;
    VOS_UINT32                          ulTcbIdxLoop;
    VOS_UINT32                          ulRslt;

    /*��ȡ״̬��ʱ���б��׵�ַ*/
    pstPtlTimerList = NAS_LMM_GetPtlTimerListAddr();

    /*ֹͣ���д�������״̬��Э�鶨ʱ��*/
    for ( ulTcbIdxLoop = TI_NAS_LMM_PTL_TI_PUB_BOUNDARY_START +1; ulTcbIdxLoop < NAS_LMM_PTL_TI_BUTT; ulTcbIdxLoop++ )
    {
        /*����Э�鶨ʱ���Ƿ������У�����������У�ֹͣ�ö�ʱ����
          ֹͣ��ʱ��ʱ��VOS��ֱ�ӽ��ö�ʱ��������ΪVOS_NULL_PTR*/
        if ( VOS_NULL_PTR != pstPtlTimerList[ulTcbIdxLoop].psthTimer )
        {
            ulRslt = PS_STOP_REL_TIMER(&(pstPtlTimerList[ulTcbIdxLoop].psthTimer));

            /*������õ���VOS�Ľӿں����������Ҫʹ��VOS�ķ���ֵ���м��*/
            if ( VOS_OK != ulRslt )
            {
                NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_StopAllPtlTimer, Stop pstPtlTimerList[%d] Failure!",
                                     ulTcbIdxLoop);
            }

            pstPtlTimerList[ulTcbIdxLoop].ucTimerRs = NAS_LMM_TIMER_RS_NOT_RUNNING;
            pstPtlTimerList[ulTcbIdxLoop].ucTimerSs = NAS_LMM_TIMER_SS_NOT_SUSPENDING;
        }
    }

   return;
}



/*****************************************************************************
  3.3 ��ʱ�� Function
*****************************************************************************/

VOS_VOID    NAS_LMM_InitAllStateTimer( VOS_VOID )
{

    NAS_LMM_PUBM_LOG_NORM("NAS_LMM_InitAllStateTimer          START INIT...");

    /*T3440*/
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_STATE_T3440,
                                        NAS_LMM_TIMER_T3440_LEN);
    /*PUBM*/
    /*USIM*/
    NAS_LMM_InitStateTimer(              TI_NAS_LMM_TIMER_WAIT_USIM_CNF,
                                        NAS_LMM_TIMER_WAIT_USIM_CNF_LEN);
    NAS_LMM_InitStateTimer(              TI_NAS_LMM_TIMER_WAIT_USIM_READY_START,
                                        NAS_LMM_TIMER_WAIT_USIM_READY_START_LEN);
    /*MRRC*/
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_MRRC_WAIT_RRC_CONN_CNF,
                                        NAS_LMM_TIMER_MRRC_WAIT_RRC_CONN_CNF_LEN);
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_MRRC_WAIT_RRC_REL_CNF,
                                        NAS_LMM_TIMER_MRRC_WAIT_RRC_REL_CNF_LEN);
    /*ATTACH*/
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_T3410,
                                        NAS_LMM_TIMER_ATTACH_T3410_LEN);
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_WAIT_ESM_PDN_RSP,
                                        NAS_LMM_TIMER_ATTACH_WAIT_ESM_PDN_RSP_LEN);
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_WAIT_ESM_BEARER_CNF,
                                        NAS_LMM_TIMER_ATTACH_WAIT_ESM_BEARER_CNF_LEN);
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_WAIT_RRC_DATA_CNF,
                                        NAS_LMM_TIMER_WAIT_RRC_DATA_CNF_LEN);
    /*DETACH*/
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_T3421,
                                        NAS_LMM_TIMER_DETACH_T3421_LEN);

    /*TAU*/
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_STATE_TAU_T3430,
                                        NAS_LMM_TIMER_TAU_T3430);

    /*SER*/
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_STATE_SERVICE_T3417,
                                        NAS_LMM_TIMER_SER_T3417_LEN);

    NAS_LMM_InitStateTimer(              TI_NAS_EMM_STATE_SERVICE_T3442,
                                        NAS_EMM_NULL);

    NAS_LMM_InitStateTimer(              TI_NAS_EMM_STATE_SERVICE_T3417_EXT,
                                        NAS_LMM_TIMER_SER_T3417_EXT_LEN);

    /*PLMN*/
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_WAIT_MMC_START_CNF_TIMER,
                                        NAS_LMM_TIMER_PLMN_WAIT_MMC_START_CNF_LEN);
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_WAIT_RRC_START_CNF_TIMER,
                                        NAS_LMM_TIMER_PLMN_WAIT_RRC_START_CNF_LEN);
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_WAIT_MMC_STOP_CNF_TIMER,
                                        NAS_LMM_TIMER_PLMN_WAIT_MMC_STOP_CNF_LEN);
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_WAIT_RRC_STOP_CNF_TIMER,
                                        NAS_LMM_TIMER_PLMN_WAIT_RRC_STOP_CNF_LEN);

    /*AUTH*/
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_T3418,
                                        NAS_LMM_TIMER_AUTH_T3418_LEN);
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_T3420,
                                        NAS_LMM_TIMER_AUTH_T3420_LEN);
    /*MMC*/
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_STATE_DEL_FORB_TA_PROID,
                                        NAS_LMM_TIMER_EMM_DEL_FORB_TA_PRIOD_LEN);
    /*SUSPEND RESUME */
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_RRCORI_WAIT_OTHER_SUSPEND_RSP_TIMER,
                                        NAS_LMM_TIMER_SUSPEND_RESUME_WAIT_LEN);

    NAS_LMM_InitStateTimer(              TI_NAS_EMM_WAIT_SUSPEND_END_TIMER,
                                        NAS_LMM_TIMER_SUSPEND_WAIT_END_LEN);

    NAS_LMM_InitStateTimer(              TI_NAS_EMM_MMCORI_WAIT_OTHER_SUSPEND_RSP_TIMER,
                                        NAS_LMM_TIMER_SUSPEND_RESUME_WAIT_LEN);

    NAS_LMM_InitStateTimer(              TI_NAS_EMM_SYSCFGORI_WAIT_OTHER_SUSPEND_RSP_TIMER,
                                        NAS_LMM_TIMER_SUSPEND_RESUME_WAIT_LEN);

    NAS_LMM_InitStateTimer(              TI_NAS_EMM_RRCRSM_WAIT_OTHER_RESUME_RSP_TIMER,
                                        NAS_LMM_TIMER_SUSPEND_RESUME_WAIT_LEN);

    NAS_LMM_InitStateTimer(              TI_NAS_EMM_SYSCFGRSM_WAIT_OTHER_RESUME_RSP_TIMER,
                                        NAS_LMM_TIMER_SUSPEND_RESUME_WAIT_LEN);

    /*��ϵͳ�任��ʱ��L->GU->L�����ĳ������ص�L��ʱ����Ҫ��������ʱ�᳤��15s,
      ����LNAS�ȴ�ϵͳ��ʱ�䳬ʱ�Ķ�ʱ����ǰֻ��5s�������ӳ��ȴ�ʱ�䵽20s*/
    NAS_LMM_InitStateTimer(              TI_NAS_EMM_WAIT_SYS_INFO_IND_TIMER,
                                        NAS_LMM_TIMER_WAIT_SYS_INFO_IND_LEN);

    NAS_LMM_InitStateTimer(              TI_NAS_EMM_STATE_WAIT_SYSCFG_CNF_TIMER,
                                        NAS_LMM_TIMER_SYSCFG_CNF_WAIT_LEN);

    NAS_LMM_InitStateTimer(              TI_NAS_EMMC_STATE_WAIT_PLMN_SRCH_CNF_TIMER,
                                        NAS_LMM_TIMER_PLMN_SRCH_CNF_WAIT_LEN);

   #if 0
    /*�� SIM��ȡ HPLMN ��������*/
    usDataLen = sizeof(LNAS_LMM_NV_HPLMN_PERI_STRU);
    ulRslt = NAS_LMM_NvimRead(EN_NV_ID_HPLMN_PERI_FILE, &stNvHplmnPeri, &usDataLen);
    if((EN_NV_OK == ulRslt) && (NAS_EMM_BIT_SLCT == stNvHplmnPeri.bitOpHplmnPeri))
    {
        /*NAS_LMM_InitStateTimer(          TI_NAS_EMMC_STATE_HPLMN_SRCH,stNvHplmnPeri.ulHplmnPeriTimerLen);*/
        NAS_EMMC_GetMmcHplmnPeriod()     = stNvHplmnPeri.ulHplmnPeriTimerLen;
    }
    else
    {
       /* NAS_LMM_InitStateTimer(          TI_NAS_EMMC_STATE_HPLMN_SRCH,
                                        NAS_LMM_TIMER_MMC_DEFAULT_HPLMN_SRCH_PRIOD_LEN);*/
        NAS_EMMC_GetMmcHplmnPeriod()     = NAS_LMM_TIMER_MMC_DEFAULT_HPLMN_SRCH_PRIOD_LEN;
    }
    #endif
    return;
}


VOS_VOID    NAS_LMM_InitStateTimer(
                    NAS_LMM_STATE_TI_ENUM_UINT16             enStateTimerId,
                    VOS_UINT32                              ulTimerLen )
{
    NAS_LMM_TIMER_CTRL_STRU              *pstStateTimer;

    pstStateTimer = NAS_LMM_GetStateTimerAddr(enStateTimerId);

    /*�޷���ȡ�ö�ʱ�����ƿ飬˵����ʱ��ID�Ƿ�*/
    if ( VOS_NULL_PTR == pstStateTimer )
    {
        NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_InitStateTimer, pstStateTimer = 0.enStateTimerId: ",
                             enStateTimerId);
        return;
    }

    pstStateTimer->ulName               = enStateTimerId;
    pstStateTimer->ulTimerLen           = ulTimerLen;
    pstStateTimer->psthTimer            = VOS_NULL_PTR;
    pstStateTimer->ulParam              = NAS_LMM_TIMER_NOT_CARE;
    pstStateTimer->ulTimerRemainLen     = NAS_LMM_TIMER_ZERO_VALUE;
    pstStateTimer->ucMaxNum             = NAS_LMM_TIMER_NOT_CARE;
    pstStateTimer->ucExpedNum           = NAS_LMM_TIMER_ZERO_VALUE;
    pstStateTimer->ucMode               = VOS_RELTIMER_NOLOOP;
    pstStateTimer->ucTimerType          = NAS_LMM_STATE_TIMER;
    pstStateTimer->ucTimerRs            = NAS_LMM_TIMER_RS_NOT_RUNNING;
    pstStateTimer->ucTimerSs            = NAS_LMM_TIMER_SS_NOT_SUSPENDING;
    pstStateTimer->ucTimerVs            = NAS_LMM_TIMER_VS_NOT_NEED_TO_UPDATE;

    return;

}
VOS_VOID    NAS_LMM_InitAllPtlTimer( VOS_VOID )
{

    NAS_LMM_PUBM_LOG_NORM("NAS_LMM_InitAllPtlTimer            START INIT...");

    NAS_LMM_InitPtlTimer(            TI_NAS_EMM_PTL_T3412,
                                    NAS_LMM_TIMER_T3412_LEN);
    NAS_LMM_InitPtlTimer(            TI_NAS_EMM_PTL_T3402,
                                    NAS_LMM_TIMER_T3402_LEN);
    NAS_LMM_InitPtlTimer(            TI_NAS_EMM_PTL_T3423,
                                    NAS_LMM_TIMER_T3423_LEN);
    NAS_LMM_InitPtlTimer(            TI_NAS_EMM_PTL_T3416,
                                    NAS_LMM_TIMER_AUTH_T3416_LEN);
    NAS_LMM_InitPtlTimer(            TI_NAS_EMM_PTL_SWITCH_OFF_TIMER,
                                    NAS_LMM_TIMER_DETACH_SWITCH_OFF_LEN);
    NAS_LMM_InitPtlTimer(            TI_NAS_EMM_PTL_REATTACH_DELAY,
                                     NAS_LMM_TIMER_REATTACH_DELAY_LEN);
    NAS_LMM_InitPtlTimer(            TI_NAS_EMM_PTL_T3411,
                                     NAS_LMM_TIMER_T3411_LEN);
	
    NAS_LMM_InitPtlTimer(            TI_NAS_EMM_PTL_CSFB_DELAY,
                                     NAS_LMM_TIMER_CSFB_DELAY_LEN);

    return;
}


VOS_VOID    NAS_LMM_InitPtlTimer(
                    NAS_LMM_PTL_TI_ENUM_UINT16             enPtlTimerId,
                    VOS_UINT32                            ulTimerLen )
{
    NAS_LMM_TIMER_CTRL_STRU              *pstPtlTimer;

    pstPtlTimer = NAS_LMM_GetPtlTimerAddr(enPtlTimerId);

    /*�޷���ȡ�ö�ʱ�����ƿ飬˵����ʱ��ID�Ƿ�*/
    if ( VOS_NULL_PTR == pstPtlTimer )
    {
        NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_InitPtlimer, pstPtlTimer = 0.enPtlTimerId: ",
                             enPtlTimerId);
        return;
    }

    pstPtlTimer->ulName               = enPtlTimerId;
    pstPtlTimer->ulTimerLen           = ulTimerLen;
    pstPtlTimer->psthTimer            = VOS_NULL_PTR;
    pstPtlTimer->ulParam              = NAS_LMM_TIMER_NOT_CARE;
    pstPtlTimer->ulTimerRemainLen     = NAS_LMM_TIMER_ZERO_VALUE;
    pstPtlTimer->ucMaxNum             = NAS_LMM_TIMER_NOT_CARE;
    pstPtlTimer->ucExpedNum           = NAS_LMM_TIMER_ZERO_VALUE;

    pstPtlTimer->ucTimerType          = NAS_LMM_PTL_TIMER;
    pstPtlTimer->ucTimerRs            = NAS_LMM_TIMER_RS_NOT_RUNNING;
    pstPtlTimer->ucTimerSs            = NAS_LMM_TIMER_SS_NOT_SUSPENDING;
    pstPtlTimer->ucTimerVs            = NAS_LMM_TIMER_VS_NOT_NEED_TO_UPDATE;
    pstPtlTimer->ucMode               = VOS_RELTIMER_NOLOOP;
    return;

}
VOS_VOID  NAS_LMM_StopAllEmmStateTimer(VOS_VOID)
{
    NAS_LMM_TIMER_CTRL_STRU              *pstStateTimerList;
    VOS_UINT32                          ulTcbIdxLoop;
    VOS_UINT32                          ulRslt;
    VOS_BOOL                            bFlag               = VOS_TRUE;

    NAS_LMM_PUBM_LOG_INFO("NAS_LMM_StopAllEmmStateTimer");

    /*��ȡ״̬��ʱ���б��׵�ַ*/
    pstStateTimerList = NAS_LMM_GetStateTimerListAddr();

    /*ֹͣ���д�������״̬��Э�鶨ʱ��*/
    for ( ulTcbIdxLoop = TI_NAS_EMM_STATE_NO_TIMER + 1; ulTcbIdxLoop < TI_NAS_EMM_STATE_TI_BUTT; ulTcbIdxLoop++ )
    {
        /*����״̬��ʱ���Ƿ������У�����������У�ֹͣ�ö�ʱ����
          ֹͣ��ʱ��ʱ��VOS��ֱ�ӽ��ö�ʱ��������ΪVOS_NULL_PTR*/
        if ( VOS_NULL_PTR != pstStateTimerList[ulTcbIdxLoop].psthTimer )
        {
            ulRslt = PS_STOP_REL_TIMER(&(pstStateTimerList[ulTcbIdxLoop].psthTimer));
            /*������õ���VOS�Ľӿں����������Ҫʹ��VOS�ķ���ֵ���м��*/
            if ( VOS_OK != ulRslt )
            {
                bFlag = VOS_FALSE;
                /* ����ʧ�ܣ���ӡʧ����Ϣ */
                NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_StopAllEmmStateTimer, Stop pstStateTimerList[%d] Failure!",
                                     ulTcbIdxLoop);
            }

            pstStateTimerList[ulTcbIdxLoop].ucTimerRs = NAS_LMM_TIMER_RS_NOT_RUNNING;
            pstStateTimerList[ulTcbIdxLoop].ucTimerSs = NAS_LMM_TIMER_SS_NOT_SUSPENDING;
        }
    }

    NAS_LMM_PUBM_LOG1_INFO("NAS_LMM_StopAllEmmStateTimer: bFlag =", bFlag);

    return;

}



VOS_VOID    NAS_LMM_StopAllEmmPtlTimer( VOS_VOID )
{
    NAS_LMM_TIMER_CTRL_STRU              *pstPtlTimerList;
    VOS_UINT32                          ulTcbIdxLoop;
    VOS_UINT32                          ulRslt;

    /*��ȡ״̬��ʱ���б��׵�ַ*/
    pstPtlTimerList = NAS_LMM_GetPtlTimerListAddr();

    /*ֹͣ���д�������״̬��Э�鶨ʱ��*/
    for (   ulTcbIdxLoop = TI_NAS_LMM_PTL_TI_PUB_BOUNDARY_START +1;
            ulTcbIdxLoop < TI_NAS_EMM_PTL_TI_BUTT;
            ulTcbIdxLoop++ )
    {
        /*����Э�鶨ʱ���Ƿ������У�����������У�ֹͣ�ö�ʱ����
          ֹͣ��ʱ��ʱ��VOS��ֱ�ӽ��ö�ʱ��������ΪVOS_NULL_PTR*/
        if ( VOS_NULL_PTR != pstPtlTimerList[ulTcbIdxLoop].psthTimer )
        {
            ulRslt = PS_STOP_REL_TIMER(&(pstPtlTimerList[ulTcbIdxLoop].psthTimer));

            /*������õ���VOS�Ľӿں����������Ҫʹ��VOS�ķ���ֵ���м��*/
            if ( VOS_OK != ulRslt )
            {
                NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_StopAllEmmPtlTimer, Stop pstPtlTimerList[%d] Failure!",
                                     ulTcbIdxLoop);
            }

            pstPtlTimerList[ulTcbIdxLoop].ucTimerRs = NAS_LMM_TIMER_RS_NOT_RUNNING;
            pstPtlTimerList[ulTcbIdxLoop].ucTimerSs = NAS_LMM_TIMER_SS_NOT_SUSPENDING;
        }
    }

   return;
}


/*****************************************************************************
 Function Name   : NAS_LMM_StopAllEmmPtlTimer
 Description     : ֹͣEMM���е�Э�鶨ʱ��
 Input           : VOS_VOID
 Output          : None
 Return          : NAS_LMM_SUCC   -- �ɹ�
                   NAS_LMM_FAIL   -- ʧ��

 History         :
    1.Hanlufeng 41410      2011-04-28  Draft Enact

*****************************************************************************/
VOS_VOID    NAS_LMM_StopAllLmmTimerExcept3412_3423( VOS_VOID )
{
    NAS_LMM_TIMER_CTRL_STRU              *pstPtlTimerList;
    VOS_UINT32                          ulTcbIdxLoop;
    VOS_UINT32                          ulRslt;

    /* ֹͣ����״̬��ʱ�� */
    NAS_LMM_StopAllStateTimer();


    /* ֹͣ����Э�鶨ʱ�� */

    /*��ȡ״̬��ʱ���б��׵�ַ*/
    pstPtlTimerList = NAS_LMM_GetPtlTimerListAddr();

    /*ֹͣ���д�������״̬��Э�鶨ʱ��*/
    for (   ulTcbIdxLoop = TI_NAS_LMM_PTL_TI_PUB_BOUNDARY_START +1;
            ulTcbIdxLoop < NAS_LMM_PTL_TI_BUTT;
            ulTcbIdxLoop++ )
    {
        /*����Э�鶨ʱ���Ƿ������У�����������У�ֹͣ�ö�ʱ����
          ֹͣ��ʱ��ʱ��VOS��ֱ�ӽ��ö�ʱ��������ΪVOS_NULL_PTR*/
        if ( VOS_NULL_PTR != pstPtlTimerList[ulTcbIdxLoop].psthTimer )
        {

            /* ����T3412��3423����ͣ��2��ʱ���������ж�ʣ�ඨʱ�� */
            if(   (TI_NAS_EMM_PTL_T3412 == (pstPtlTimerList[ulTcbIdxLoop].ulName))
                ||(TI_NAS_EMM_PTL_T3423 == (pstPtlTimerList[ulTcbIdxLoop].ulName)))
            {
                NAS_LMM_PUBM_LOG1_INFO("NAS_LMM_StopAllEmmPtlTimerExcept3412_3423. ",
                                      pstPtlTimerList[ulTcbIdxLoop].ulName);
                continue;
            }

            ulRslt = PS_STOP_REL_TIMER(&(pstPtlTimerList[ulTcbIdxLoop].psthTimer));

            /*������õ���VOS�Ľӿں����������Ҫʹ��VOS�ķ���ֵ���м��*/
            if ( VOS_OK != ulRslt )
            {
                NAS_LMM_PUBM_LOG1_ERR("NAS_LMM_StopAllPtlTimer, Stop pstPtlTimerList[%d] Failure!",
                                     ulTcbIdxLoop);
            }

            pstPtlTimerList[ulTcbIdxLoop].ucTimerRs = NAS_LMM_TIMER_RS_NOT_RUNNING;
            pstPtlTimerList[ulTcbIdxLoop].ucTimerSs = NAS_LMM_TIMER_SS_NOT_SUSPENDING;

        }
    }




   return;
}
/*****************************************************************************
 Function Name   : NAS_LMM_StopAllEmmPtlTimer
 Description     : �ڹ���ʱֹͣEMM��ʱ��
 Input           : VOS_VOID
 Output          : None
 Return          : None

 History         :
    1.FTY         2012-02-21 Draft Enact
*****************************************************************************/
VOS_VOID    NAS_LMM_SuspendInitStopLmmTimer( VOS_VOID )
{
    /* ֹͣ״̬��ʱ�� */
    NAS_LMM_SuspendInitStopStateTimer();

    /* ֹͣЭ�鶨ʱ�� */
    NAS_LMM_SuspendInitStopPtlTimer();
}


VOS_VOID  NAS_LMM_GetStateTimerLen( NAS_LMM_STATE_TI_ENUM_UINT16   enStateTimerId,
                                   VOS_UINT32   *pulTimerLen)
{
    NAS_LMM_TIMER_CTRL_STRU             *pstStateTimer = VOS_NULL_PTR;


    /*��ȡ��״̬��ʱ�����ƿ��ַ*/
    pstStateTimer = NAS_LMM_GetStateTimerAddr(enStateTimerId);
    /*�޷���ȡ�ö�ʱ�����ƿ��ַ��˵����ʱ��ID�Ƿ�*/
    if ( VOS_NULL_PTR == pstStateTimer )
    {
        NAS_LMM_PUBM_LOG_ERR("NAS_LMM_GetStateTimerLen, pstStateTimer��Ч");
        *pulTimerLen = 0;
        return;
    }

    /*��ȡ��ʱ��ʱ��*/
    *pulTimerLen = pstStateTimer->ulTimerLen;
}
VOS_VOID  NAS_LMM_GetPtlTimerLen( NAS_LMM_PTL_TI_ENUM_UINT16   enPtlTimerId,
                                   VOS_UINT32   *pulTimerLen)
{
    NAS_LMM_TIMER_CTRL_STRU             *pstPtlTimer = VOS_NULL_PTR;


    /*��ȡ��Э�鶨ʱ�����ƿ��ַ*/
    pstPtlTimer = NAS_LMM_GetPtlTimerAddr(enPtlTimerId);
    /*�޷���ȡ�ö�ʱ�����ƿ��ַ��˵����ʱ��ID�Ƿ�*/
    if ( VOS_NULL_PTR == pstPtlTimer )
    {
        NAS_LMM_PUBM_LOG_ERR("NAS_LMM_GetPtlTimerLen, pstPtlTimer��Ч");
        *pulTimerLen = 0;
        return;
    }

    /*��ȡ��ʱ��ʱ��*/
    *pulTimerLen = pstPtlTimer->ulTimerLen;
}
NAS_LMM_TIMER_RUN_STA_ENUM_UINT32  NAS_LMM_IsStaTimerRunning(
                                    NAS_LMM_STATE_TI_ENUM_UINT16 enStateTimerId )
{
    NAS_LMM_TIMER_CTRL_STRU              *pstStateTimer = VOS_NULL_PTR;

    /*��ȡ��״̬��ʱ�����ƿ��ַ*/
    pstStateTimer = NAS_LMM_GetStateTimerAddr(enStateTimerId);
    if ( VOS_NULL_PTR == pstStateTimer )
    {
        NAS_LMM_PUBM_LOG_ERR("NAS_LMM_IsStaTimerRunning: pstStateTimer NULL.");
        return NAS_LMM_TIMER_INVALID;
    }

    /*����״̬��ʱ���Ƿ�������*/
    if ( VOS_NULL_PTR != pstStateTimer->psthTimer )
    {
        return NAS_LMM_TIMER_RUNNING;
    }
    else
    {
        return NAS_LMM_TIMER_STOPED;
    }
}


NAS_LMM_TIMER_RUN_STA_ENUM_UINT32  NAS_LMM_IsPtlTimerRunning(
                                    NAS_LMM_PTL_TI_ENUM_UINT16 enPtlTimerId )
{
    NAS_LMM_TIMER_CTRL_STRU              *pstPtlTimer = VOS_NULL_PTR;

    /*��ȡ��Э�鶨ʱ�����ƿ��ַ*/
    pstPtlTimer = NAS_LMM_GetPtlTimerAddr(enPtlTimerId);
    if ( VOS_NULL_PTR == pstPtlTimer )
    {
        NAS_LMM_PUBM_LOG_ERR("NAS_LMM_IsPtlTimerRunning: pstPtlTimer NULL.");
        return NAS_LMM_TIMER_INVALID;
    }

    /*����״̬��ʱ���Ƿ�������*/
    if ( VOS_NULL_PTR != pstPtlTimer->psthTimer )
    {
        return NAS_LMM_TIMER_RUNNING;
    }
    else
    {
        return NAS_LMM_TIMER_STOPED;
    }
}

VOS_VOID    NAS_LMM_SuspendAllPtlTimer( VOS_VOID )
{
    VOS_UINT32                          ulTcbIdxLoop;

    /*ֹͣ���д�������״̬��Э�鶨ʱ��*/
    for ( ulTcbIdxLoop = TI_NAS_EMM_STATE_NO_TIMER + 1; ulTcbIdxLoop < TI_NAS_EMM_STATE_TI_BUTT; ulTcbIdxLoop++ )
    {
        /*����Э�鶨ʱ���Ƿ������У�����������У�����ö�ʱ��*/
        NAS_LMM_SuspendStateTimer((NAS_LMM_STATE_TI_ENUM_UINT16) ulTcbIdxLoop);
    }
    for (   ulTcbIdxLoop = TI_NAS_LMM_PTL_TI_PUB_BOUNDARY_START +1;
            ulTcbIdxLoop < TI_NAS_EMM_PTL_TI_BUTT;
            ulTcbIdxLoop++ )
    {
        /*����Э�鶨ʱ���Ƿ������У�����������У�����ö�ʱ��*/
        NAS_LMM_SuspendStateTimer((NAS_LMM_STATE_TI_ENUM_UINT16) ulTcbIdxLoop);
    }

   return;
}
VOS_VOID    NAS_LMM_SuspendAllPtlTimerExpT3412( VOS_VOID )
{
    VOS_UINT32                          ulTcbIdxLoop;

    /*ֹͣ���д�������״̬��Э�鶨ʱ��*/
    for ( ulTcbIdxLoop = TI_NAS_EMM_STATE_NO_TIMER + 1; ulTcbIdxLoop < TI_NAS_EMM_STATE_TI_BUTT; ulTcbIdxLoop++ )
    {
        /*����Э�鶨ʱ���Ƿ������У�����������У�����ö�ʱ��*/
        NAS_LMM_SuspendStateTimer((NAS_LMM_STATE_TI_ENUM_UINT16) ulTcbIdxLoop);
    }
    for (   ulTcbIdxLoop = TI_NAS_LMM_PTL_TI_PUB_BOUNDARY_START +1;
            ulTcbIdxLoop < TI_NAS_EMM_PTL_TI_BUTT;
            ulTcbIdxLoop++ )
    {
        if(ulTcbIdxLoop == TI_NAS_EMM_PTL_T3412)
        {
            continue;
        }
        /*����Э�鶨ʱ���Ƿ������У�����������У�����ö�ʱ��*/
        NAS_LMM_SuspendStateTimer( (NAS_LMM_STATE_TI_ENUM_UINT16)ulTcbIdxLoop);
    }

   return;
}
VOS_VOID    NAS_LMM_ResumeAllPtlTimer( VOS_VOID )
{
    VOS_UINT32                          ulTcbIdxLoop;

    /*ֹͣ���д�������״̬��Э�鶨ʱ��*/
    for ( ulTcbIdxLoop = TI_NAS_EMM_STATE_NO_TIMER + 1; ulTcbIdxLoop < TI_NAS_EMM_STATE_TI_BUTT; ulTcbIdxLoop++ )
    {
        /*����Э�鶨ʱ���Ƿ����������ڹ��𣬻ָ��ö�ʱ��*/
        NAS_LMM_ResumeStateTimer((NAS_LMM_STATE_TI_ENUM_UINT16) ulTcbIdxLoop);
    }
    for (   ulTcbIdxLoop = TI_NAS_LMM_PTL_TI_PUB_BOUNDARY_START +1;
            ulTcbIdxLoop < TI_NAS_EMM_PTL_TI_BUTT;
            ulTcbIdxLoop++ )
    {
        /*����Э�鶨ʱ���Ƿ����������ڹ��𣬻ָ��ö�ʱ��*/
        NAS_LMM_ResumeStateTimer((NAS_LMM_STATE_TI_ENUM_UINT16)ulTcbIdxLoop);
    }

   return;
}
VOS_VOID    NAS_LMM_StopAllAttachTimer( VOS_VOID )
{
    NAS_LMM_StopStateTimer( TI_NAS_EMM_WAIT_ESM_PDN_RSP);

    NAS_LMM_StopStateTimer( TI_NAS_EMM_T3410);

    NAS_LMM_StopStateTimer( TI_NAS_EMM_WAIT_ESM_BEARER_CNF);

    NAS_LMM_StopStateTimer( TI_NAS_EMM_WAIT_RRC_DATA_CNF);
}


VOS_VOID  NAS_LMM_SndOmEmmTimerStatus(
    NAS_LMM_TIMER_RUN_STA_ENUM_UINT32          enTimerStatus,
    VOS_UINT16                          enTimerId,
    VOS_UINT32                          ulTimerRemainLen
)
{
    NAS_EMM_TIMER_INFO_STRU            *pstMsg = VOS_NULL_PTR;

    pstMsg = (NAS_EMM_TIMER_INFO_STRU*)NAS_LMM_MEM_ALLOC(sizeof(NAS_EMM_TIMER_INFO_STRU));
    if (VOS_NULL_PTR == pstMsg)
    {
        NAS_LMM_PUBM_LOG_ERR("NAS_LMM_SndOmEmmTimerStatus: mem alloc fail!.");
        return;
    }

    pstMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulSenderPid     = PS_PID_MM;
    pstMsg->stMsgHeader.ulReceiverPid   = PS_PID_MM;
    pstMsg->stMsgHeader.ulLength        = sizeof(NAS_EMM_TIMER_INFO_STRU) - NAS_EMM_LEN_VOS_MSG_HEADER;

    pstMsg->stMsgHeader.ulMsgName       = enTimerId + PS_MSG_ID_EMM_TO_EMM_OM_BASE;
    pstMsg->enTimerStatus               = enTimerStatus;
    pstMsg->usTimerId                   = enTimerId;
    pstMsg->usRsv                       = 0;

    pstMsg->ulTimerRemainLen            = ulTimerRemainLen;

    (VOS_VOID)LTE_MsgHook((VOS_VOID*)pstMsg);

    NAS_LMM_MEM_FREE(pstMsg);

}
/*lint +e961*/
/*lint +e960*/


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

