
/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/

#include "om.h"
#include "TafSdcCtx.h"
#include "MmaAppLocal.h"
#include "TafMmaCtx.h"
#include "Taf_Status.h"
#include "TafMmaSndInternalMsg.h"
#include "TafMmaMain.h"
#include "TafMmaSndApp.h"

#include "TafMmaMntn.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define     THIS_FILE_ID        PS_FILE_ID_TAF_MMA_FSM_MAIN_C
#define     STA_WARNING_PRINT   PS_PRINT_WARNING

/*****************************************************************************
  2 ȫ�ֱ���
*****************************************************************************/
extern STATUS_CONTEXT_STRU                     g_StatusContext;

/*****************************************************************************
  3 ���Ͷ���
*****************************************************************************/

/*****************************************************************************
  4 ��������
*****************************************************************************/

/*****************************************************************************
  5 ��������
*****************************************************************************/

/*****************************************************************************
  6 ����ʵ��
*****************************************************************************/


VOS_UINT32 TAF_MMA_RcvAppPhoneModeSetReq_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PH_OP_MODE_STRU                *pstPhModeSet    = VOS_NULL_PTR;
    MN_APP_REQ_MSG_STRU                *pstRcvMsg       = VOS_NULL_PTR;

    pstRcvMsg           = (MN_APP_REQ_MSG_STRU*)pstMsg;
    pstPhModeSet        = (TAF_PH_OP_MODE_STRU *)pstRcvMsg->aucContent;

    /* �ػ�ʱ��Ҫ�ж��Ƿ���Ҫ��ֹ��������,����ʱ����Ҫ�ж� */
    if ((STA_FSM_ENABLE   != g_StatusContext.ulFsmState)
     && (TAF_PH_MODE_FULL != pstPhModeSet->PhMode))
    {
        TAF_MMA_ProcPhoneStop_NotEnableStatus();
    }

    /* ����phone mode״̬�� */
    TAF_MMA_FSM_InitSubFsm(TAF_MMA_FSM_PHONE_MODE);

    return VOS_TRUE;
}



VOS_UINT32 TAF_MMA_RcvTafPhoneModeSetReq_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_PHONE_MODE_SET_REQ_STRU    *pstPhoneModeSetReq  = VOS_NULL_PTR;

    pstPhoneModeSetReq  = (TAF_MMA_PHONE_MODE_SET_REQ_STRU *)pstMsg;

    /* �ػ�ʱ��Ҫ�ж��Ƿ���Ҫ��ֹ��������,����ʱ����Ҫ�ж� */
    if ((STA_FSM_ENABLE   != g_StatusContext.ulFsmState)
     && (TAF_PH_MODE_FULL != pstPhoneModeSetReq->stPhoneModePara.PhMode))
    {
        TAF_MMA_ProcPhoneStop_NotEnableStatus();
    }

    /* ����phone mode״̬�� */
    TAF_MMA_FSM_InitSubFsm(TAF_MMA_FSM_PHONE_MODE);

    return VOS_TRUE;
}




VOS_UINT32 TAF_MMA_RcvSysCfgSetReq_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_SYS_CFG_REQ_STRU           *pstRcvMsg = VOS_NULL_PTR;
    VOS_UINT16                          usClientId;
    VOS_UINT8                           ucOpId;
    
    VOS_UINT8                           ucCtxIndex;

    pstRcvMsg = (TAF_MMA_SYS_CFG_REQ_STRU *)pstMsg;

    /*ClientId*/
    usClientId      = pstRcvMsg->stCtrl.usClientId;
    ucOpId          = pstRcvMsg->stCtrl.ucOpId;

    
    /* ���g_stTafMmaCtx.astMmaOperCtx��TAF_MMA_OPER_SYS_CFG_REQ���ͽṹ���ڱ�ʹ�� */
    if (VOS_TRUE == TAF_MMA_IsOperTypeUsed(TAF_MMA_OPER_SYS_CFG_REQ))
    {
        /* ֱ�ӻظ�SYSCFG����ʧ�� */
        TAF_MMA_SndSysCfgSetCnf(&(pstRcvMsg->stCtrl),
                                TAF_MMA_APP_OPER_RESULT_FAILURE,
                                TAF_ERR_ERROR);

        return VOS_TRUE;
    }

    /* �Ҳ���δ��ʹ�õģ��ظ�ʧ�� */
    if (VOS_FALSE == TAF_MMA_GetUnusedOperCtxIndex(&ucCtxIndex))
    {
        /* ֱ�ӻظ�SYSCFG����ʧ�� */
        TAF_MMA_SndSysCfgSetCnf(&(pstRcvMsg->stCtrl),
                                TAF_MMA_APP_OPER_RESULT_FAILURE,
                                TAF_ERR_ERROR);

        return VOS_TRUE;
    }

    /* ����һ��g_stTafMmaCtx.astMmaOperCtx����Ӧ���� */
    TAF_MMA_SetOperCtx(pstRcvMsg->stCtrl,
                       TAF_MMA_OPER_SYS_CFG_REQ, ucCtxIndex);
    

    /* �ڹػ�״̬��SYSCFG����ʱ�򲻼��CLģʽ��CLģʽֻ���ڿ�������Ч */
    if ( (STA_FSM_NULL == g_StatusContext.ulFsmState)
      || (STA_FSM_STOP == g_StatusContext.ulFsmState) )
    {
        /* ����ԭ�е�SYSCFG�������� */
        MMA_PhSysCfgSet(usClientId, ucOpId, &(pstRcvMsg->stSysCfgPara));

        return VOS_TRUE;
    }

    /* ����ǿ���״̬��C+Lģʽ����ʱ��AT������SYSCFG���ã���ظ�ʧ��,���б���  */
    if ((VOS_TRUE        == TAF_MMA_IsPowerOnCLInterWork())
     && (CMMCA_CLIENT_ID != usClientId))
    {
        TAF_MMA_ReportSysCfgSetCnf(TAF_ERR_ERROR);

        return VOS_TRUE;
    }

    /* �������״̬������C+Lģʽ����ʱ��CMMCA������SYSCFG���ã���ظ�ʧ��,���б���  */
    if ((VOS_FALSE       == TAF_MMA_IsPowerOnCLInterWork())
     && (CMMCA_CLIENT_ID == usClientId))
    {
        TAF_MMA_ReportSysCfgSetCnf(TAF_ERR_ERROR);

        return VOS_TRUE;
    }

    /* ����ԭ�е�SYSCFG�������� */
    MMA_PhSysCfgSet(usClientId, ucOpId, &(pstRcvMsg->stSysCfgPara));

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvMmaInterPowerInit_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ����phone mode״̬�� */
    TAF_MMA_FSM_InitSubFsm(TAF_MMA_FSM_PHONE_MODE);

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvMmaInterUsimStatusChangeInd_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (STA_FSM_ENABLE != g_StatusContext.ulFsmState)
    {
        TAF_MMA_ProcPhoneStop_NotEnableStatus();
    }


    /* �����ǰΪCLģʽ����ȴ�CDMA�����ػ� */
    if (VOS_TRUE == TAF_SDC_IsConfigCLInterWork())
    {
        return VOS_TRUE;
    }


    /* ����phone mode״̬�� */
    TAF_MMA_FSM_InitSubFsm(TAF_MMA_FSM_PHONE_MODE);

    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvMmaSimlockStatusChangeInd_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (STA_FSM_ENABLE != g_StatusContext.ulFsmState)
    {
        TAF_MMA_ProcPhoneStop_NotEnableStatus();
    }
        
    /* ����phone mode״̬�� */
    TAF_MMA_FSM_InitSubFsm(TAF_MMA_FSM_PHONE_MODE);

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvMmaPhoneModeRsltInd_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_PHONE_MODE_RSLT_IND_STRU   *pstPhoneModeRslt    = VOS_NULL_PTR;
    TAF_SDC_CTX_STRU                   *pstSdcCtx           = VOS_NULL_PTR;

    pstPhoneModeRslt = (TAF_MMA_PHONE_MODE_RSLT_IND_STRU*)pstMsg;
    pstSdcCtx = TAF_SDC_GetSdcCtx();

    if (TAF_MMA_PHONE_MODE_RESULT_SWITCH_ON_SUCC == pstPhoneModeRslt->enRslt)
    {
        /*����ʹ��״̬������״̬��־��*/
        g_StatusContext.ulFsmState = STA_FSM_ENABLE;
    }
    else
    {
        /* ��ʼ��ȫ�ֱ��� */
        TAF_MMA_InitCtx(TAF_MMA_INIT_CTX_POWEROFF, TAF_MMA_GetMmaCtxAddr());

        TAF_SDC_InitLcConfigPara(&(pstSdcCtx->stMsCfgInfo.stMsCapInfo.stLcConfigPara));

        g_StatusContext.ulFsmState = STA_FSM_NULL;
    }

    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvPihUsimStatusInd_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �����ڲ���״̬�ı���Ϣ */
    TAF_MMA_SndInterUsimChangeInd();

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvUsimRefreshIndMsg_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* �����ڲ���״̬�ı���Ϣ */
    TAF_MMA_SndInterUsimChangeInd();

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvDetachReq_Main(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_DETACH_REQ_STRU            *pstDetachReq = VOS_NULL_PTR;
    TAF_PH_DETACH_TYPE                  ucDetachType;
    VOS_UINT8                           ucIndex;

    pstDetachReq = (TAF_MMA_DETACH_REQ_STRU *)pstMsg;

    /* �����C+Lģʽ����ʱ��AT������detach����ظ�ʧ��,���б���  */
    if ((VOS_TRUE == TAF_MMA_IsPowerOnCLInterWork())
     && (CMMCA_CLIENT_ID != pstDetachReq->stCtrl.usClientId))
    {
        TAF_MMA_SndDetachCnf(&(pstDetachReq->stCtrl),
                             TAF_MMA_APP_OPER_RESULT_FAILURE,
                             TAF_ERR_ERROR);

        return VOS_TRUE;
    }

    /* �����ǰ����C+Lģʽ������CMMCA����DETACH,��ֱ�ӻظ�ʧ�� */
    if ((VOS_FALSE == TAF_MMA_IsPowerOnCLInterWork())
     && (CMMCA_CLIENT_ID == pstDetachReq->stCtrl.usClientId))
    {
        TAF_MMA_SndDetachCnf(&(pstDetachReq->stCtrl),
                             TAF_MMA_APP_OPER_RESULT_FAILURE,
                             TAF_ERR_ERROR);
        return VOS_TRUE;
    }

    /* ���g_stTafMmaCtx.astMmaOperCtx��TAF_MMA_OPER_ACQ_REQ���ͽṹ���ڱ�ʹ�� */
    if (VOS_TRUE == TAF_MMA_IsOperTypeUsed(TAF_MMA_OPER_DETACH_REQ))
    {
        TAF_MMA_SndDetachCnf(&(pstDetachReq->stCtrl),
                             TAF_MMA_APP_OPER_RESULT_FAILURE,
                             TAF_ERR_ERROR);
        return VOS_TRUE;
    }

    /* �Ҳ���δ��ʹ�õģ��ظ�ʧ�� */
    if (VOS_FALSE == TAF_MMA_GetUnusedOperCtxIndex(&ucIndex))
    {
        TAF_MMA_SndDetachCnf(&(pstDetachReq->stCtrl),
                             TAF_MMA_APP_OPER_RESULT_FAILURE,
                             TAF_ERR_ERROR);
        return VOS_TRUE;
    }

    /* ����һ��g_stTafMmaCtx.astMmaOperCtx����Ӧ���� */
    TAF_MMA_SetOperCtx(pstDetachReq->stCtrl,
                       TAF_MMA_OPER_DETACH_REQ, ucIndex);

    if (TAF_PH_SERVICE_CS == pstDetachReq->stDetachPara.ucDetachDomain)
    {
        ucDetachType = TAF_PH_CS_OPERATE;
    }
    else if (TAF_PH_SERVICE_PS == pstDetachReq->stDetachPara.ucDetachDomain)
    {
        ucDetachType = TAF_PH_PS_OPERATE;
    }
    else if (TAF_PH_SERVICE_CS_PS == pstDetachReq->stDetachPara.ucDetachDomain)
    {
        ucDetachType = TAF_PH_PS_CS_OPERATE;
    }
    else
    {
        ucDetachType = TAF_PH_CS_OPERATE;
    }

    MMA_PhoneDetach(pstDetachReq->stCtrl.usClientId, pstDetachReq->stCtrl.ucOpId,
                    ucDetachType, pstDetachReq->stDetachPara.enDetachCause);

    return VOS_TRUE;
}







#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


