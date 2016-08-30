



/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "RrmInit.h"
#include "Rrm.h"
#include "RrmCtrl.h"
#include "RrmDebug.h"



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_RRM_INIT_C

#if (FEATURE_ON == FEATURE_MULTI_MODEM)

#if (FEATURE_ON == FEATURE_DSDS)

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_UINT32 RRM_PidInit(enum VOS_INIT_PHASE_DEFINE enPhase)
{
    switch (enPhase)
    {
        case VOS_IP_LOAD_CONFIG:
            /* ��ʼ��RRM��Ϣ */
            RRM_Init();
            break;

        default:
            break;
    }

    return VOS_OK;
}
VOS_UINT32 RRM_FidInit(enum VOS_INIT_PHASE_DEFINE enPhase)
{
    VOS_UINT32                          ulResult = VOS_ERR;


    switch (enPhase)
    {
        case VOS_IP_LOAD_CONFIG:
            /* ������ά�ɲ��ź��� */
            ulResult =  RRM_MNTN_CreateMutexSem(UEPS_PID_RRM);
            if (VOS_OK != ulResult)
            {
                RRM_ERROR_LOG(UEPS_PID_RRM, "RRM_FidInit, VOS_RegisterPIDInfo Fail.");
                return VOS_ERR;
            }

            /* LOG ��ʼ�� */
            RRM_INIT_LOG_ENT();

            /* ע��PID */
            ulResult = VOS_RegisterPIDInfo(UEPS_PID_RRM,
                                           (Init_Fun_Type)RRM_PidInit,
                                           (Msg_Fun_Type)RRM_MsgProc);
            if (VOS_OK != ulResult)
            {
                RRM_ERROR_LOG(UEPS_PID_RRM, "RRM_FidInit, VOS_RegisterPIDInfo Fail.");
                return VOS_ERR;
            }

            /* ע���������ȼ� */
            ulResult = VOS_RegisterTaskPrio(UEPS_FID_RRM, RRM_TASK_PRIO);
            if (VOS_OK != ulResult)
            {
                RRM_ERROR_LOG(UEPS_PID_RRM, "RRM_FidInit, VOS_RegisterTaskPrio Fail.");
                return VOS_ERR;
            }

            break;

        default:
            break;
    }

    return VOS_OK;
}

#else

VOS_UINT32 RRM_FidInit(enum VOS_INIT_PHASE_DEFINE enPhase)
{
    switch (enPhase)
    {
        case VOS_IP_LOAD_CONFIG:
            break;

        default:
            break;
    }

    return VOS_OK;
}

#endif /* FEATURE_ON == FEATURE_DSDS */

#endif /* FEATURE_ON == FEATURE_MULTI_MODEM */

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

