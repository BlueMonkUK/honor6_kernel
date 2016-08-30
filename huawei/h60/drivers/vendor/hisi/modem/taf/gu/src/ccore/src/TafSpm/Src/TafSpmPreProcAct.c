

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "pslog.h"
#include "om.h"
#include "PsTypeDef.h"
#include "PsCommonDef.h"
#include "MnCallApi.h"
#include "MnCallReqProc.h"
#include "TafLog.h"
#include "MnCallMgmt.h"
#include "MnCallMnccProc.h"
#include "TafSpmServiceDomainSelProc.h"
#include "TafMmaCtx.h"
#include "TafSdcLib.h"
#include "TafSpmSndInternalMsg.h"
#include "SpmImsaInterface.h"
#include "TafSpmRedial.h"
#include "TafSpmRedial.h"
#include "TafSpmPreProcTbl.h"
#include "TafSpmPreProcAct.h"
#include "TafAgentInterface.h"
#include "TafSpmComFunc.h"
#if (FEATURE_ON == FEATURE_IMS)
#include "CallImsaInterface.h"
#endif

#include "TafSpmRedial.h"
#include "TafSpmCtx.h"
#include "NasSms.h"
#include "TafSpmMntn.h"
#include "MnMsgApi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_TAF_SPM_PREPROC_ACT_C


/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_UINT32 TAF_SPM_RcvAppOrigReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MN_CALL_APP_REQ_MSG_STRU           *pstAppMsg  = VOS_NULL_PTR;
    MN_CALL_EMERGENCY_CAT_STRU          stEmergencyCat;

    pstAppMsg                 = (MN_CALL_APP_REQ_MSG_STRU *)pstMsg;
    
    PS_MEM_SET(&stEmergencyCat, 0, sizeof(MN_CALL_EMERGENCY_CAT_STRU));    
    
    /* call is allowed if phone mode is power on */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
        TAF_SPM_SendCcServiceRequetFail(ulEventType, pstMsg, TAF_CS_CAUSE_POWER_OFF);
        
        return VOS_TRUE;
    }

    /* VIDEO call������ͨ���д������������к����� */
    if (MN_CALL_TYPE_VIDEO != pstAppMsg->unParm.stOrig.enCallType)
    {
        /* �ж��ǽ��������½������е�CAT��Ϣ */
        if (VOS_TRUE  == TAF_SPM_IsEmergencyNum(&pstAppMsg->unParm.stOrig.stDialNumber, 
                                                VOS_TRUE,
                                                &stEmergencyCat))
        {
            pstAppMsg->unParm.stOrig.enCallType  = MN_CALL_TYPE_EMERGENCY;

            PS_MEM_CPY(&(pstAppMsg->unParm.stOrig.stEmergencyCat),
                        &stEmergencyCat,
                        sizeof(MN_CALL_EMERGENCY_CAT_STRU));

            return VOS_FALSE;
        }
    }  

    /* forbid normal call when USIM service is not available */
    if (VOS_FALSE == TAF_SPM_IsUsimServiceAvailable())
    {
        TAF_SPM_SendCcServiceRequetFail(ulEventType, pstMsg, TAF_CS_CAUSE_SIM_NOT_EXIST);
        return VOS_TRUE;
    }
    
    /* ��Ҫ�ж���������Ч,��Ϊ������Ч������ܻ�disable LTE��GU��,��Ҫ��GU�¼������� */
    if (VOS_FALSE == TAF_SDC_IsUsimStausValid())
    {
        TAF_SPM_SendCcServiceRequetFail(ulEventType, pstMsg, TAF_CS_CAUSE_SIM_INVALID);
        return VOS_TRUE;
    }

    return VOS_FALSE;
}
VOS_UINT32 TAF_SPM_RcvAppSupsCmdReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRet;
#if (FEATURE_ON == FEATURE_IMS)    
    MN_CALL_APP_REQ_MSG_STRU           *pstAppMsg = VOS_NULL_PTR;
#endif
    
    ulRet           = VOS_FALSE;
    
#if (FEATURE_ON == FEATURE_IMS)
    /* ����Ѿ�IMS����д��ڣ�ֱ��ѡ��IMS�������������VOS_FALSE����CALLģ�鴦�� */
    if (VOS_TRUE == TAF_SDC_GetImsCallExistFlg())
    {
        ulRet = TAF_SPM_ProcReqMsgBasedOnCsOverIp(ulEventType, pstMsg);
    }
    else
    {
        /* �ó����������SRVCC���л��ɹ��������ʱ�û�����Ҷ���Ӧ��callʱ����Ҫ
         * ����ز������б����call������Ϣ��������GU���ַ����ز�
         */
        pstAppMsg = (MN_CALL_APP_REQ_MSG_STRU *)pstMsg;
        
        /* �û��һ�������ж�Ӧ���ز�������ڣ������ */
        switch (pstAppMsg->unParm.stCallMgmtCmd.enCallSupsCmd)
        {
            case MN_CALL_SUPS_CMD_REL_CALL_X:
                TAF_SPM_FreeCallRedialBufferWithCallId(pstAppMsg->callId);
                break;
                
            case MN_CALL_SUPS_CMD_REL_ALL_CALL:
            case MN_CALL_SUPS_CMD_REL_ALL_EXCEPT_WAITING_CALL:
                TAF_SPM_FreeCallRedialBufferWithClientId(pstAppMsg->clientId);
                break;

            default:
                break;
        }    
    }
#endif
    
    return ulRet;
}
VOS_UINT32 TAF_SPM_RcvAppGetInfoReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                                              ulRet;
    ulRet           = VOS_FALSE;
    
#if (FEATURE_ON == FEATURE_IMS)
    /* ����Ѿ�IMS����д��ڣ�ֱ��ѡ��IMS�������������VOS_FALSE����CALLģ�鴦�� */
    if (VOS_TRUE == TAF_SDC_GetImsCallExistFlg())
    {
        ulRet = TAF_SPM_ProcReqMsgBasedOnCsOverIp(ulEventType, pstMsg);
    }    
#endif
    
    return ulRet;
}
VOS_UINT32 TAF_SPM_RcvAppStartDtmfReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                                              ulRet;
    ulRet           = VOS_FALSE;
    
#if (FEATURE_ON == FEATURE_IMS)
    /* ����Ѿ�IMS����д��ڣ�ֱ��ѡ��IMS�������������VOS_FALSE����CALLģ�鴦�� */
    if (VOS_TRUE == TAF_SDC_GetImsCallExistFlg())
    {
        ulRet = TAF_SPM_ProcReqMsgBasedOnCsOverIp(ulEventType, pstMsg);
    }    
#endif
    
    return ulRet;
}
VOS_UINT32 TAF_SPM_RcvAppStopDtmfReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                                              ulRet;
    ulRet           = VOS_FALSE;
    
#if (FEATURE_ON == FEATURE_IMS)
    /* ����Ѿ�IMS����д��ڣ�ֱ��ѡ��IMS�������������VOS_FALSE����CALLģ�鴦�� */
    if (VOS_TRUE == TAF_SDC_GetImsCallExistFlg())
    {
        ulRet = TAF_SPM_ProcReqMsgBasedOnCsOverIp(ulEventType, pstMsg);
    }    
#endif
    
    return ulRet;
}
VOS_UINT32 TAF_SPM_RcvAppGetCdurReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                                              ulRet;
    ulRet           = VOS_FALSE;
    
#if (FEATURE_ON == FEATURE_IMS)
    /* ����Ѿ�IMS����д��ڣ�ֱ��ѡ��IMS�������������VOS_FALSE����CALLģ�鴦�� */
    if (VOS_TRUE == TAF_SDC_GetImsCallExistFlg())
    {
        ulRet = TAF_SPM_ProcReqMsgBasedOnCsOverIp(ulEventType, pstMsg);
    }    
#endif
    
    return ulRet;
}
VOS_UINT32 TAF_SPM_RcvAppGetCallInfoReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                                              ulRet;
    ulRet           = VOS_FALSE;
    
#if (FEATURE_ON == FEATURE_IMS)
    /* ����Ѿ�IMS����д��ڣ�ֱ��ѡ��IMS�������������VOS_FALSE����CALLģ�鴦�� */
    if (VOS_TRUE == TAF_SDC_GetImsCallExistFlg())
    {
        ulRet = TAF_SPM_ProcReqMsgBasedOnCsOverIp(ulEventType, pstMsg);
    }    
#endif
    
    return ulRet;
}
VOS_UINT32 TAF_SPM_RcvAppGetClprReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                                              ulRet;
    ulRet           = VOS_FALSE;
    
#if (FEATURE_ON == FEATURE_IMS)
    /* ����Ѿ�IMS����д��ڣ�ֱ��ѡ��IMS�������������VOS_FALSE����CALLģ�鴦�� */
    if (VOS_TRUE == TAF_SDC_GetImsCallExistFlg())
    {
        ulRet = TAF_SPM_ProcReqMsgBasedOnCsOverIp(ulEventType, pstMsg);
    }    
#endif
    
    return ulRet;
}
VOS_UINT32 TAF_SPM_RcvAppSendRpdataDirect_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulCause;

    /* ��鵱ǰ��״̬�����ػ�״̬�Ƿ�������SMSҵ�� */
    if (VOS_FALSE == TAF_SPM_IsSmsServiceReqAllowed_PreProc(ulEventType, pstMsg, &ulCause))
    {
        /* �����������SMSҵ�񣬸�AT�ظ�ʧ�ܣ�����Ӧ��ԭ��ֵ */
        TAF_SPM_SendSmsServiceRequetFail(ulEventType, pstMsg, ulCause);
        
        return VOS_TRUE;
    }
    
    /* ������SMSҵ��ʱ��Ҫ����FDN&CALL CONTROL��� */
    return VOS_FALSE;

}
VOS_UINT32 TAF_SPM_RcvProcUssSsReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MN_APP_REQ_MSG_STRU                *pstRcvMsg   = VOS_NULL_PTR;
    TAF_SS_PROCESS_USS_REQ_STRU        *pstSsReqMsg = VOS_NULL_PTR;
    TAF_PH_ERR_CODE                     enCause;

    pstRcvMsg   = (MN_APP_REQ_MSG_STRU *)pstMsg;
    pstSsReqMsg = (TAF_SS_PROCESS_USS_REQ_STRU *)&(pstRcvMsg->aucContent[0]);

    /* configure CUSD setting */
    if (TAF_SS_CUSD_TYPE_ENABLE == pstSsReqMsg->enCusdType)
    {
        TAF_SDC_UpdateRptCmdStatus(TAF_SDC_RPT_CMD_CUSD, VOS_TRUE);
    }
    else
    {
        TAF_SDC_UpdateRptCmdStatus(TAF_SDC_RPT_CMD_CUSD, VOS_FALSE);
    }

    /* check whether or not USSD string exists */
    if (0 == pstSsReqMsg->UssdStr.usCnt)
    {
        /* ���ֻ��CUSD�Ƿ������ϱ�������AT���Ѿ��ͷţ�����Ҫ�ȴ�C�˻ظ� */
        return VOS_TRUE;
    }

    /* ��鵱ǰ��״̬�����ػ�״̬�Ƿ�������SSҵ�� */
    if (VOS_FALSE == TAF_SPM_IsSsServiceReqAllowed_PreProc(ulEventType, pstMsg, &enCause))
    {
        /* �����������SSҵ�񣬸�AT�ظ�ʧ�ܣ�����Ӧ��ԭ��ֵ */
        TAF_SPM_SendSsServiceRequetFail(ulEventType, pstMsg, enCause);
        
        return VOS_TRUE;
    }

    /* ������SSҵ��ʱ��Ҫ����FDN&CALL CONTROL��� */
    return VOS_FALSE;
}


VOS_UINT32 TAF_SPM_RcvRegisterSsReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    
    TAF_PH_ERR_CODE                     enCause;

    /* ��鵱ǰ��״̬�����ػ�״̬�Ƿ�������SSҵ�� */
    if (VOS_FALSE == TAF_SPM_IsSsServiceReqAllowed_PreProc(ulEventType, pstMsg, &enCause))
    {
        /* �����������SSҵ�񣬸�AT�ظ�ʧ�ܣ�����Ӧ��ԭ��ֵ */
        TAF_SPM_SendSsServiceRequetFail(ulEventType, pstMsg, enCause);
        
        return VOS_TRUE;
    }

    /* ������SSҵ��ʱ��Ҫ����FDN&CALL CONTROL��� */
    return VOS_FALSE;
}
VOS_UINT32 TAF_SPM_RcvEraseSsReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    
    TAF_PH_ERR_CODE                     enCause;

    /* ��鵱ǰ��״̬�����ػ�״̬�Ƿ�������SSҵ�� */
    if (VOS_FALSE == TAF_SPM_IsSsServiceReqAllowed_PreProc(ulEventType, pstMsg, &enCause))
    {
        /* �����������SSҵ�񣬸�AT�ظ�ʧ�ܣ�����Ӧ��ԭ��ֵ */
        TAF_SPM_SendSsServiceRequetFail(ulEventType, pstMsg, enCause);
        
        return VOS_TRUE;
    }

    /* ������SSҵ��ʱ��Ҫ����FDN&CALL CONTROL��� */
    return VOS_FALSE;    
}
VOS_UINT32 TAF_SPM_RcvActivateSsReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    
    TAF_PH_ERR_CODE                     enCause;

    /* ��鵱ǰ��״̬�����ػ�״̬�Ƿ�������SSҵ�� */
    if (VOS_FALSE == TAF_SPM_IsSsServiceReqAllowed_PreProc(ulEventType, pstMsg, &enCause))
    {
        /* �����������SSҵ�񣬸�AT�ظ�ʧ�ܣ�����Ӧ��ԭ��ֵ */
        TAF_SPM_SendSsServiceRequetFail(ulEventType, pstMsg, enCause);
        
        return VOS_TRUE;
    }

    /* ������SSҵ��ʱ��Ҫ����FDN&CALL CONTROL��� */
    return VOS_FALSE;    
}
VOS_UINT32 TAF_SPM_RcvDeactivateSsReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    
    TAF_PH_ERR_CODE                     enCause;

    /* ��鵱ǰ��״̬�����ػ�״̬�Ƿ�������SSҵ�� */
    if (VOS_FALSE == TAF_SPM_IsSsServiceReqAllowed_PreProc(ulEventType, pstMsg, &enCause))
    {
        /* �����������SSҵ�񣬸�AT�ظ�ʧ�ܣ�����Ӧ��ԭ��ֵ */
        TAF_SPM_SendSsServiceRequetFail(ulEventType, pstMsg, enCause);
        
        return VOS_TRUE;
    }

    /* ������SSҵ��ʱ��Ҫ����FDN&CALL CONTROL��� */
    return VOS_FALSE;    
}
VOS_UINT32 TAF_SPM_RcvInterrogateSsReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    
    TAF_PH_ERR_CODE                     enCause;

    /* ��鵱ǰ��״̬�����ػ�״̬�Ƿ�������SSҵ�� */
    if (VOS_FALSE == TAF_SPM_IsSsServiceReqAllowed_PreProc(ulEventType, pstMsg, &enCause))
    {
        /* �����������SSҵ�񣬸�AT�ظ�ʧ�ܣ�����Ӧ��ԭ��ֵ */
        TAF_SPM_SendSsServiceRequetFail(ulEventType, pstMsg, enCause);
        
        return VOS_TRUE;
    }

    /* ������SSҵ��ʱ��Ҫ����FDN&CALL CONTROL��� */
    return VOS_FALSE;    
}
VOS_UINT32 TAF_SPM_RcvRegPwdSsReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    
    TAF_PH_ERR_CODE                     enCause;

    /* ��鵱ǰ��״̬�����ػ�״̬�Ƿ�������SSҵ�� */
    if (VOS_FALSE == TAF_SPM_IsSsServiceReqAllowed_PreProc(ulEventType, pstMsg, &enCause))
    {
        /* �����������SSҵ�񣬸�AT�ظ�ʧ�ܣ�����Ӧ��ԭ��ֵ */
        TAF_SPM_SendSsServiceRequetFail(ulEventType, pstMsg, enCause);
        
        return VOS_TRUE;
    }

    /* ������SSҵ��ʱ��Ҫ����FDN&CALL CONTROL��� */
    return VOS_FALSE;    
}
VOS_UINT32 TAF_SPM_RcvEraseCCentrySsReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    
    TAF_PH_ERR_CODE                     enCause;

    /* ��鵱ǰ��״̬�����ػ�״̬�Ƿ�������SSҵ�� */
    if (VOS_FALSE == TAF_SPM_IsSsServiceReqAllowed_PreProc(ulEventType, pstMsg, &enCause))
    {
        /* �����������SSҵ�񣬸�AT�ظ�ʧ�ܣ�����Ӧ��ԭ��ֵ */
        TAF_SPM_SendSsServiceRequetFail(ulEventType, pstMsg, enCause);
        
        return VOS_TRUE;
    }

    /* ������SSҵ��ʱ��Ҫ����FDN&CALL CONTROL��� */
    return VOS_FALSE;    
}
VOS_UINT32 TAF_SPM_RcvRleaseSsReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRet;
    
    ulRet               = VOS_FALSE;
    
#if (FEATURE_ON == FEATURE_IMS)
    /* ����Ѿ�IMS��USSDҵ����ڣ�ֱ��ѡ��IMS�������������VOS_FALSE����NAS�������̴��� */
    if (VOS_TRUE == TAF_SDC_GetImsSsSrvExistFlg())
    {
        ulRet = TAF_SPM_ProcReqMsgBasedOnCsOverIp(ulEventType, pstMsg);
    }
#endif
    
    return ulRet;
}
VOS_UINT32 TAF_SPM_RcvStkOrigReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRet;
    MN_CALL_EMERGENCY_CAT_STRU          stEmergencyCat;
    MN_CALL_TYPE_ENUM_U8                enCallType; 
    VOS_UINT32                          ulExistBc;
    MN_APP_CALL_CALLORIG_REQ_STRU      *pstOrigParam = VOS_NULL_PTR;

    pstOrigParam = (MN_APP_CALL_CALLORIG_REQ_STRU *)pstMsg;

    PS_MEM_SET(&stEmergencyCat, 0, sizeof(MN_CALL_EMERGENCY_CAT_STRU));
    enCallType  = MN_CALL_TYPE_VOICE;
    ulExistBc   = VOS_TRUE;
    
    /* call is allowed if phone mode is power on */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
        TAF_SPM_SendCcServiceRequetFail(ulEventType, pstMsg, TAF_CS_CAUSE_POWER_OFF);
        return VOS_TRUE;
    }

    if (0 == pstOrigParam->stBc.ucLen)
    {
        ulExistBc    = VOS_FALSE;
    }

    /* ����BC������ȡ�������� */
    ulRet = TAF_SPM_GetBcCallType(ulExistBc,
                                  pstOrigParam->stBc.ucLen,
                                  pstOrigParam->stBc.aucBc,
                                  &enCallType);
    if (VOS_TRUE != ulRet)
    {
        MN_WARN_LOG("MN_CALL_StkCallOrigReqProc: Fail to TAF_SPM_GetBcCallType.");
        
        TAF_CALL_SendCallOrigCnf(pstOrigParam->usClientId,
                                 pstOrigParam->opID,
                                 pstOrigParam->callID,
                                 TAF_CS_CAUSE_CALL_CTRL_BEYOND_CAPABILITY);
                                 
        MN_CALL_ReportErrIndEvent(MN_CLIENT_ALL,
                                  0,
                                  TAF_CS_CAUSE_CALL_CTRL_BEYOND_CAPABILITY,
                                  pstOrigParam->callID);
        return VOS_TRUE;
    }
    
    /* VIDEO call������ͨ���д������������к����� */
    pstOrigParam->enCallType = enCallType;
    if (MN_CALL_TYPE_VIDEO != pstOrigParam->enCallType)
    {
        /* �ж��ǽ��������½������е�CAT��Ϣ */
        if (VOS_TRUE  == TAF_SPM_IsEmergencyNum((MN_CALL_CALLED_NUM_STRU *)&pstOrigParam->stCalledAddr,
                                                  VOS_FALSE,
                                                  &stEmergencyCat))
        {
            pstOrigParam->enCallType        = MN_CALL_TYPE_EMERGENCY;
            
            PS_MEM_CPY(&(pstOrigParam->stEmergencyCat),
                        &stEmergencyCat,
                        sizeof(MN_CALL_EMERGENCY_CAT_STRU));

            return VOS_FALSE;
        }
        else
        {
            PS_MEM_SET(&(pstOrigParam->stEmergencyCat),
                       0,
                       sizeof(MN_CALL_EMERGENCY_CAT_STRU));

        }
    }
    
    /* forbid normal call when USIM service is not available */
    if (VOS_FALSE == TAF_SPM_IsUsimServiceAvailable())
    {
        TAF_SPM_SendCcServiceRequetFail(ulEventType, pstMsg, TAF_CS_CAUSE_SIM_NOT_EXIST);
        return VOS_TRUE;
    }
    
    /* ��Ҫ�ж���������Ч,��Ϊ������Ч������ܻ�disable LTE��GU��,��Ҫ��GU�¼������� */
    if (VOS_FALSE == TAF_SDC_IsUsimStausValid())
    {
        TAF_SPM_SendCcServiceRequetFail(ulEventType, pstMsg, TAF_CS_CAUSE_SIM_INVALID);
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


#if (FEATURE_ON == FEATURE_IMS)

VOS_UINT32 TAF_SPM_RcvMsgSmmaInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_SPM_DOMAIN_SEL_RESULT_ENUM_UINT8                    enDomainSelRslt;

    /* ������ѡ�� */
    enDomainSelRslt = TAF_SPM_ProcSmsDomainSelection();

    TAF_SPM_ProcSmmaIndDomainSelectionResult(ulEventType, pstMsg, enDomainSelRslt);

    return VOS_TRUE;
}
VOS_UINT32 TAF_SPM_RcvMsgReportInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_SPM_MSG_REPORT_IND_STRU                            *pstMsgReportInd = VOS_NULL_PTR;
    TAF_SPM_ENTRY_MSG_STRU                                 *pstCacheMsg     = VOS_NULL_PTR;
    VOS_UINT8                                               ucIndex;
    TAF_SPM_DOMAIN_SEL_RESULT_ENUM_UINT8                    enDomainSelRslt;
    VOS_UINT32                                              ulRst;
    VOS_UINT8                                               ucImsSmsSndFlg;

    pstMsgReportInd = (TAF_SPM_MSG_REPORT_IND_STRU *)pstMsg;

    /* ���IMS�������ڷ��ͱ�־ */
    TAF_SPM_SetImsSmsSendingFlg(VOS_FALSE);

    if (VOS_TRUE == TAF_SDC_GetSmsRedailFromImsToCsSupportFlag())
    {
        /* �Ӷ����ز�������ȡ��������Ϣ */
        pstCacheMsg = TAF_SPM_GetSmsRedialBufferWithClientId(&ucIndex, pstMsgReportInd->usClientId);
        
        if (VOS_NULL_PTR != pstCacheMsg)
        {
            if (VOS_TRUE == TAF_MSG_IsSmsRedialCauseValueFromImsDomain(pstMsgReportInd->stRptEvtInfo.enErrorCode))
            {
                if (TAF_BuildEventType(WUEPS_PID_TAF, ID_TAF_SPM_SMMA_IND) == pstCacheMsg->ulEventType)
                {
                    /* SMMA��Ϣ����Ҫ�����ⲿ��Ϣ��MSGģ�� */
                    TAF_SPM_SendMsgSmmaRsp(TAF_MSG_SIGNALLING_TYPE_NAS_SIGNALLING);
                }
                else
                {
                    /* �޸���Ϣ�е��������� */
                    TAF_SPM_ProcReqMsgBasedOnNasSignalling(pstCacheMsg->ulEventType, 
                                                           (struct MsgCB*)&(pstCacheMsg->aucEntryMsgBuffer[0]));
                    
                    /* ���������Ϣ */
                    TAF_SPM_UpdateServiceCtrlEntryMsg(pstCacheMsg->ulEventType,
                                                      (struct MsgCB*)&(pstCacheMsg->aucEntryMsgBuffer[0]));
                    
                    TAF_SPM_SetUpdateEntryMsgFlg(VOS_TRUE);
                }
                
                /* ����ز����� */
                TAF_SPM_FreeSpecificedIndexSmsRedialBuffer(ucIndex);
        
                /* �����ʱ��ѡ�񻺴�����Ϣ���ڣ���Ҫ����Ϣת����MSGģ�� */
                if (0 != TAF_SPM_GetSmsMsgQueueNum())
                {
                    /* �����ڲ���ѡ���SPM */
                    TAF_SPM_SndInternalDomainSelectionInd();
                }

                TAF_SPM_LogSrvDomainSelRedialInfo();
                
                return VOS_FALSE;
            }
        
            /* ����ز����� */
            TAF_SPM_FreeSpecificedIndexSmsRedialBuffer(ucIndex);

            TAF_SPM_LogSrvDomainSelRedialInfo();
        }
    }

    /* ��֧�ֻ����ز�������ϱ����ŷ��ͱ��� */
    TAF_SPM_ProcSmsRptEvent(pstMsgReportInd);

    /* �������������ѡ��queue */
    if (0 != TAF_SPM_GetSmsMsgQueueNum())
    {
        enDomainSelRslt = TAF_SPM_DOMAIN_SEL_RESULT_BUTT;

        ulRst = TAF_SPM_ProcSmsMsgQueue(&enDomainSelRslt);

        /* check if domain selection type is buffer message */
        if (TAF_SPM_DOMAIN_SEL_RESULT_BUFFER_MESSAGE != enDomainSelRslt)
        {
            /* ���������ѡ�񻺴治Ϊ�գ��ҵ�ǰ��IMS�����ڷ��ͣ��������������ѡ�񻺴� */
            ucImsSmsSndFlg = TAF_SPM_GetImsSmsSendingFlg();
            if ((0         != TAF_SPM_GetSmsMsgQueueNum())
             && (VOS_FALSE == ucImsSmsSndFlg))
            {
                TAF_SPM_SndInternalDomainSelectionInd();
            }

            /* log service domain selection infomation */
            TAF_SPM_LogSrvDomainSelQueueInfo();

            return ulRst;
        }
    }
    return VOS_TRUE;
}


VOS_UINT32 TAF_SPM_RcvImsaCallOrigCnf_PreProc(
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_MSG_STRU             *pstImsaCallMsg           = VOS_NULL_PTR;
    TAF_SDC_CURC_RPT_CTRL_STRU         *pstCurcRptCtrl           = VOS_NULL_PTR;
    TAF_SDC_UNSOLICITED_RPT_CTRL_STRU  *pstUnsolicitedRptCtrl    = VOS_NULL_PTR;
    TAF_SPM_ENTRY_MSG_STRU             *pstCacheInfo             = VOS_NULL_PTR;
    MN_CALL_INFO_STRU                   stCallInfo;

    pstImsaCallMsg          = (IMSA_SPM_CALL_MSG_STRU *)pstMsg;

    pstCurcRptCtrl          = TAF_SDC_GetCurcRptCtrl();
    pstUnsolicitedRptCtrl   = TAF_SDC_GetUnsolicitedRptCtrl();

    PS_MEM_SET(&stCallInfo, 0, sizeof(MN_CALL_INFO_STRU));

    PS_MEM_CPY(&stCallInfo, &(pstImsaCallMsg->stCallInfo), sizeof(MN_CALL_INFO_STRU));
    
    /* ֻҪMO�ɹ�ʱ��������IMS����д��ڱ�ʶ */
    if (TAF_CS_CAUSE_SUCCESS == stCallInfo.enCause)
    {
        TAF_SDC_SetImsCallExistFlg(VOS_TRUE);
    }
    else
    {
        /* IMSA����д���з��򣬱����޸�һ�¸��Ϊ�����ز��ж�ʹ�� */
        stCallInfo.enCallDir = MN_CALL_DIR_MO;
        
        if (VOS_TRUE == TAF_SPM_IsCsCallRedialAllowed(&stCallInfo))
        {
            /* ��ȡ�����ز�������Ϣ */
            pstCacheInfo = TAF_SPM_GetSpecificedIndexFromCallRedialBuffer(0);
        
            /* ���������Ϣ */
            TAF_SPM_UpdateServiceCtrlEntryMsg(pstCacheInfo->ulEventType,
                                              (struct MsgCB*)&(pstCacheInfo->aucEntryMsgBuffer[0]));
        
            TAF_SPM_SetUpdateEntryMsgFlg(VOS_TRUE);
        
            /* ����ز����� */
            TAF_SPM_FreeSpecificedIndexCallRedialBuffer(0);
        
            return VOS_FALSE;
        }

        /* ����ز����� */
        TAF_SPM_FreeSpecificedIndexCallRedialBuffer(0);
    }

    /* ���������ϱ����ȫ�ֱ�����ֵ */
    PS_MEM_CPY(stCallInfo.aucCurcRptCfg,
                pstCurcRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    PS_MEM_CPY(stCallInfo.aucUnsolicitedRptCfg,
                pstUnsolicitedRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    /* ����һ��MN_CALL_EVT_CALL_ORIG_CNF��Ϣ������Ӧ��cliendId�ظ� */
    MN_SendClientEvent(stCallInfo.clientId,
                        MN_CALLBACK_CS_CALL,
                        MN_CALL_EVT_CALL_ORIG_CNF,
                        &stCallInfo);

    /* ����Err_Ind��Ϣ��cliendIdΪ�㲥���� */
    MN_CALL_ReportErrIndEvent(MN_CLIENT_ALL, 0, stCallInfo.enCause, 0);

    return VOS_TRUE;
}
VOS_UINT32 TAF_SPM_RcvImsaCallOrig_PreProc(
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_MSG_STRU             *pstImsaCallMsg           = VOS_NULL_PTR;
    TAF_SDC_CURC_RPT_CTRL_STRU         *pstCurcRptCtrl           = VOS_NULL_PTR;
    TAF_SDC_UNSOLICITED_RPT_CTRL_STRU  *pstUnsolicitedRptCtrl    = VOS_NULL_PTR;
    MN_CALL_INFO_STRU                   stCallInfo;

    pstImsaCallMsg          = (IMSA_SPM_CALL_MSG_STRU *)pstMsg;

    pstCurcRptCtrl          = TAF_SDC_GetCurcRptCtrl();
    pstUnsolicitedRptCtrl   = TAF_SDC_GetUnsolicitedRptCtrl();

    PS_MEM_SET(&stCallInfo, 0, sizeof(MN_CALL_INFO_STRU));

    PS_MEM_CPY(&stCallInfo, &(pstImsaCallMsg->stCallInfo), sizeof(MN_CALL_INFO_STRU));

    /* ���������ϱ����ȫ�ֱ�����ֵ */
    PS_MEM_CPY(stCallInfo.aucCurcRptCfg,
                pstCurcRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    PS_MEM_CPY(stCallInfo.aucUnsolicitedRptCfg,
                pstUnsolicitedRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    /* ����һ��MN_CALL_EVT_ORIG��Ϣ������Ӧ��cliendId�ظ� */
    MN_SendClientEvent(pstImsaCallMsg->usClientId,
                       MN_CALLBACK_CS_CALL,
                       MN_CALL_EVT_ORIG,
                       &stCallInfo);

    /* �����ز�������Ϣ��call ID��������Ϣ���ͷ�ʱ���� */
    TAF_SPM_UpdateCallRedialBufferMsgWithCallId(stCallInfo.clientId, stCallInfo.callId);
    
    return VOS_TRUE;
}
VOS_UINT32 TAF_SPM_RcvImsaCallProc_PreProc(
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_MSG_STRU             *pstImsaCallMsg           = VOS_NULL_PTR;
    TAF_SDC_CURC_RPT_CTRL_STRU         *pstCurcRptCtrl           = VOS_NULL_PTR;
    TAF_SDC_UNSOLICITED_RPT_CTRL_STRU  *pstUnsolicitedRptCtrl    = VOS_NULL_PTR;
    MN_CALL_INFO_STRU                   stCallInfo;

    pstImsaCallMsg          = (IMSA_SPM_CALL_MSG_STRU *)pstMsg;

    pstCurcRptCtrl          = TAF_SDC_GetCurcRptCtrl();
    pstUnsolicitedRptCtrl   = TAF_SDC_GetUnsolicitedRptCtrl();

    PS_MEM_SET(&stCallInfo, 0, sizeof(MN_CALL_INFO_STRU));

    PS_MEM_CPY(&stCallInfo, &(pstImsaCallMsg->stCallInfo), sizeof(MN_CALL_INFO_STRU));

    /* ���������ϱ����ȫ�ֱ�����ֵ */
    PS_MEM_CPY(stCallInfo.aucCurcRptCfg,
                pstCurcRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    PS_MEM_CPY(stCallInfo.aucUnsolicitedRptCfg,
                pstUnsolicitedRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    /* ����һ��MN_CALL_EVT_CALL_PROC��Ϣ������Ӧ��cliendId�ظ� */
    MN_SendClientEvent(pstImsaCallMsg->usClientId,
                       MN_CALLBACK_CS_CALL,
                       MN_CALL_EVT_CALL_PROC,
                       &stCallInfo);

    return VOS_TRUE;
}


VOS_UINT32 TAF_SPM_RcvImsaCallAlerting_PreProc(
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_MSG_STRU             *pstImsaCallMsg           = VOS_NULL_PTR;
    TAF_SDC_CURC_RPT_CTRL_STRU         *pstCurcRptCtrl           = VOS_NULL_PTR;
    TAF_SDC_UNSOLICITED_RPT_CTRL_STRU  *pstUnsolicitedRptCtrl    = VOS_NULL_PTR;
    MN_CALL_INFO_STRU                   stCallInfo;

    pstImsaCallMsg          = (IMSA_SPM_CALL_MSG_STRU *)pstMsg;

    pstCurcRptCtrl          = TAF_SDC_GetCurcRptCtrl();
    pstUnsolicitedRptCtrl   = TAF_SDC_GetUnsolicitedRptCtrl();

    PS_MEM_SET(&stCallInfo, 0, sizeof(MN_CALL_INFO_STRU));

    PS_MEM_CPY(&stCallInfo, &(pstImsaCallMsg->stCallInfo), sizeof(MN_CALL_INFO_STRU));

    /* ���������ϱ����ȫ�ֱ�����ֵ */
    PS_MEM_CPY(stCallInfo.aucCurcRptCfg,
                pstCurcRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    PS_MEM_CPY(stCallInfo.aucUnsolicitedRptCfg,
                pstUnsolicitedRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    /* ����һ��MN_CALL_EVT_ALERTING��Ϣ������Ӧ��cliendId�ظ� */
    MN_SendClientEvent(pstImsaCallMsg->usClientId,
                       MN_CALLBACK_CS_CALL,
                       MN_CALL_EVT_ALERTING,
                       &stCallInfo);

    /* ����ز����� */
    TAF_SPM_FreeCallRedialBufferWithCallId(pstImsaCallMsg->stCallInfo.callId);
    
    return VOS_TRUE;
}
VOS_UINT32 TAF_SPM_RcvImsaCallConnect_PreProc(
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_MSG_STRU             *pstImsaCallMsg           = VOS_NULL_PTR;
    TAF_SDC_CURC_RPT_CTRL_STRU         *pstCurcRptCtrl           = VOS_NULL_PTR;
    TAF_SDC_UNSOLICITED_RPT_CTRL_STRU  *pstUnsolicitedRptCtrl    = VOS_NULL_PTR;
    MN_CALL_INFO_STRU                   stCallInfo;

    pstImsaCallMsg          = (IMSA_SPM_CALL_MSG_STRU *)pstMsg;

    pstCurcRptCtrl          = TAF_SDC_GetCurcRptCtrl();
    pstUnsolicitedRptCtrl   = TAF_SDC_GetUnsolicitedRptCtrl();

    PS_MEM_SET(&stCallInfo, 0, sizeof(MN_CALL_INFO_STRU));

    PS_MEM_CPY(&stCallInfo, &(pstImsaCallMsg->stCallInfo), sizeof(MN_CALL_INFO_STRU));

    /* ���������ϱ����ȫ�ֱ�����ֵ */
    PS_MEM_CPY(stCallInfo.aucCurcRptCfg,
                pstCurcRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    PS_MEM_CPY(stCallInfo.aucUnsolicitedRptCfg,
                pstUnsolicitedRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    /* ����һ��MN_CALL_EVT_CONNECT��Ϣ������Ӧ��cliendId�ظ� */
    MN_SendClientEvent(pstImsaCallMsg->usClientId,
                       MN_CALLBACK_CS_CALL,
                       MN_CALL_EVT_CONNECT,
                       &stCallInfo);

    /* ����ز����� */
    TAF_SPM_FreeCallRedialBufferWithCallId(pstImsaCallMsg->stCallInfo.callId);

    return VOS_TRUE;
}
VOS_UINT32 TAF_SPM_RcvImsaCallSupsCmdCnf_PreProc(
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_MSG_STRU             *pstImsaCallMsg           = VOS_NULL_PTR;
    TAF_SDC_CURC_RPT_CTRL_STRU         *pstCurcRptCtrl           = VOS_NULL_PTR;
    TAF_SDC_UNSOLICITED_RPT_CTRL_STRU  *pstUnsolicitedRptCtrl    = VOS_NULL_PTR;
    MN_CALL_INFO_STRU                   stCallInfo;

    pstImsaCallMsg          = (IMSA_SPM_CALL_MSG_STRU *)pstMsg;

    pstCurcRptCtrl          = TAF_SDC_GetCurcRptCtrl();
    pstUnsolicitedRptCtrl   = TAF_SDC_GetUnsolicitedRptCtrl();

    PS_MEM_SET(&stCallInfo, 0, sizeof(MN_CALL_INFO_STRU));

    PS_MEM_CPY(&stCallInfo, &(pstImsaCallMsg->stCallInfo), sizeof(MN_CALL_INFO_STRU));

    /* ���������ϱ����ȫ�ֱ�����ֵ */
    PS_MEM_CPY(stCallInfo.aucCurcRptCfg,
                pstCurcRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    PS_MEM_CPY(stCallInfo.aucUnsolicitedRptCfg,
                pstUnsolicitedRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    /* ����һ��MN_CALL_EVT_SUPS_CMD_CNF��Ϣ������Ӧ��cliendId�ظ� */
    MN_SendClientEvent(stCallInfo.clientId,
                       MN_CALLBACK_CS_CALL,
                       MN_CALL_EVT_SUPS_CMD_CNF,
                       &stCallInfo);


    return VOS_TRUE;
}


VOS_UINT32 TAF_SPM_RcvImsaCallRelease_PreProc(
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_MSG_STRU             *pstImsaCallMsg           = VOS_NULL_PTR;
    TAF_SDC_CURC_RPT_CTRL_STRU         *pstCurcRptCtrl           = VOS_NULL_PTR;
    TAF_SDC_UNSOLICITED_RPT_CTRL_STRU  *pstUnsolicitedRptCtrl    = VOS_NULL_PTR;
    TAF_SPM_ENTRY_MSG_STRU             *pstCacheInfo             = VOS_NULL_PTR;
    MN_CALL_INFO_STRU                   stCallInfo;

    pstImsaCallMsg          = (IMSA_SPM_CALL_MSG_STRU *)pstMsg;

    pstCurcRptCtrl          = TAF_SDC_GetCurcRptCtrl();
    pstUnsolicitedRptCtrl   = TAF_SDC_GetUnsolicitedRptCtrl();

    PS_MEM_SET(&stCallInfo, 0, sizeof(MN_CALL_INFO_STRU));

    PS_MEM_CPY(&stCallInfo, &(pstImsaCallMsg->stCallInfo), sizeof(MN_CALL_INFO_STRU));

    if (VOS_TRUE == TAF_SPM_IsCsCallRedialAllowed(&stCallInfo))
    {
        /* ��ȡ�����ز�������Ϣ */
        pstCacheInfo = TAF_SPM_GetCallRedialBufferWithCallId(stCallInfo.callId);
        if (VOS_NULL_PTR != pstCacheInfo)
        {
            /* ���������Ϣ */
            TAF_SPM_UpdateServiceCtrlEntryMsg(pstCacheInfo->ulEventType,
                                              (struct MsgCB*)&(pstCacheInfo->aucEntryMsgBuffer[0]));
            
            TAF_SPM_SetUpdateEntryMsgFlg(VOS_TRUE);
            
            /* ����ز����� */
            TAF_SPM_FreeCallRedialBufferWithCallId(stCallInfo.callId);
            
            return VOS_FALSE;
        }
    }

    /* ����ز����� */
    TAF_SPM_FreeCallRedialBufferWithCallId(stCallInfo.callId);

    /* ���������ϱ����ȫ�ֱ�����ֵ */
    PS_MEM_CPY(stCallInfo.aucCurcRptCfg,
                pstCurcRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    PS_MEM_CPY(stCallInfo.aucUnsolicitedRptCfg,
                pstUnsolicitedRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    /* ����һ��MN_CALL_EVT_RELEASED��Ϣ������Ӧ��cliendId�ظ� */
    MN_SendClientEvent(pstImsaCallMsg->usClientId,
                       MN_CALLBACK_CS_CALL,
                       MN_CALL_EVT_RELEASED,
                       &stCallInfo);

    /* ����Err_Ind��Ϣ,����ԭ��CS��NV��Ŀ��� */
    MN_CALL_ReportErrIndEvent(MN_CLIENT_ALL, 0, stCallInfo.enCause, 0);

    return VOS_TRUE;
}
VOS_UINT32 TAF_SPM_RcvImsaCallIncoming_PreProc(
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_MSG_STRU             *pstImsaCallMsg           = VOS_NULL_PTR;
    TAF_SDC_CURC_RPT_CTRL_STRU         *pstCurcRptCtrl           = VOS_NULL_PTR;
    TAF_SDC_UNSOLICITED_RPT_CTRL_STRU  *pstUnsolicitedRptCtrl    = VOS_NULL_PTR;
    MN_CALL_INFO_STRU                   stCallInfo;

    pstImsaCallMsg          = (IMSA_SPM_CALL_MSG_STRU *)pstMsg;

    pstCurcRptCtrl          = TAF_SDC_GetCurcRptCtrl();
    pstUnsolicitedRptCtrl   = TAF_SDC_GetUnsolicitedRptCtrl();

    PS_MEM_SET(&stCallInfo, 0, sizeof(MN_CALL_INFO_STRU));

    PS_MEM_CPY(&stCallInfo, &(pstImsaCallMsg->stCallInfo), sizeof(MN_CALL_INFO_STRU));

    /* ����IMS����д��ڱ�ʶ */
    TAF_SDC_SetImsCallExistFlg(VOS_TRUE);

    /* ���������ϱ����ȫ�ֱ�����ֵ */
    PS_MEM_CPY(stCallInfo.aucCurcRptCfg,
                pstCurcRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    PS_MEM_CPY(stCallInfo.aucUnsolicitedRptCfg,
                pstUnsolicitedRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    /* ����һ��MN_CALL_EVT_INCOMING��Ϣ������Ӧ��cliendId�ظ� */
    MN_SendClientEvent(pstImsaCallMsg->usClientId,
                       MN_CALLBACK_CS_CALL,
                       MN_CALL_EVT_INCOMING,
                       &stCallInfo);

    return VOS_TRUE;
}
VOS_UINT32 TAF_SPM_RcvImsaCallStartDtmfCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_START_DTMF_CNF_STRU  *pstStartDtmfCnfMsg       = VOS_NULL_PTR;
    TAF_CALL_EVT_DTMF_CNF_STRU          stDtmfCnf;    

    pstStartDtmfCnfMsg      = (IMSA_SPM_CALL_START_DTMF_CNF_STRU *)pstMsg;
    
    PS_MEM_SET(&stDtmfCnf, 0, sizeof(TAF_CALL_EVT_DTMF_CNF_STRU));
    
    stDtmfCnf.callId        = pstStartDtmfCnfMsg->ucCallId;
    stDtmfCnf.usClientId    = pstStartDtmfCnfMsg->usClientId;
    stDtmfCnf.opId          = pstStartDtmfCnfMsg->ucOpId;
    stDtmfCnf.enCause       = pstStartDtmfCnfMsg->enCause;
    stDtmfCnf.enDtmfState   = pstStartDtmfCnfMsg->enDtmfState;
    stDtmfCnf.ucDtmfCnt     = pstStartDtmfCnfMsg->ucDtmfCnt;

    
    TAF_CALL_SendMsg(pstStartDtmfCnfMsg->usClientId,
                     MN_CALL_EVT_START_DTMF_CNF,
                     &stDtmfCnf,
                     sizeof(TAF_CALL_EVT_DTMF_CNF_STRU));

    return VOS_TRUE;
}


VOS_UINT32 TAF_SPM_RcvImsaCallStartDtmfRsltInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_START_DTMF_RSLT_IND_STRU                 *pstDtmfRsltIndMsg       = VOS_NULL_PTR;
    TAF_CALL_EVT_DTMF_CNF_STRU                              stDtmfCnf;

    pstDtmfRsltIndMsg      = (IMSA_SPM_CALL_START_DTMF_RSLT_IND_STRU *)pstMsg;
    
    PS_MEM_SET(&stDtmfCnf, 0, sizeof(TAF_CALL_EVT_DTMF_CNF_STRU));
    
    stDtmfCnf.callId        = pstDtmfRsltIndMsg->ucCallId;
    stDtmfCnf.usClientId    = pstDtmfRsltIndMsg->usClientId;
    stDtmfCnf.opId          = pstDtmfRsltIndMsg->ucOpId;
    stDtmfCnf.enCause       = pstDtmfRsltIndMsg->enCause;
    stDtmfCnf.enDtmfState   = pstDtmfRsltIndMsg->enDtmfState;
    stDtmfCnf.ucDtmfCnt     = pstDtmfRsltIndMsg->ucDtmfCnt;

    
    TAF_CALL_SendMsg(pstDtmfRsltIndMsg->usClientId,
                     MN_CALL_EVT_START_DTMF_RSLT,
                     &stDtmfCnf,
                     sizeof(TAF_CALL_EVT_DTMF_CNF_STRU));

    return VOS_TRUE;
}



VOS_UINT32 TAF_SPM_RcvImsaCallStopDtmfCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_STOP_DTMF_CNF_STRU   *pstStopDtmfCnfMsg       = VOS_NULL_PTR;
    TAF_CALL_EVT_DTMF_CNF_STRU          stDtmfCnf;    

    pstStopDtmfCnfMsg      = (IMSA_SPM_CALL_STOP_DTMF_CNF_STRU *)pstMsg;
    
    PS_MEM_SET(&stDtmfCnf, 0, sizeof(TAF_CALL_EVT_DTMF_CNF_STRU));
    
    stDtmfCnf.callId       = pstStopDtmfCnfMsg->ucCallId;
    stDtmfCnf.usClientId   = pstStopDtmfCnfMsg->usClientId;
    stDtmfCnf.opId         = pstStopDtmfCnfMsg->ucOpId;
    stDtmfCnf.enCause      = pstStopDtmfCnfMsg->enCause;
    stDtmfCnf.enDtmfState  = pstStopDtmfCnfMsg->enDtmfState;
    stDtmfCnf.ucDtmfCnt    = pstStopDtmfCnfMsg->ucDtmfCnt;

    
    TAF_CALL_SendMsg(pstStopDtmfCnfMsg->usClientId,
                     MN_CALL_EVT_STOP_DTMF_CNF,
                     &stDtmfCnf,
                     sizeof(TAF_CALL_EVT_DTMF_CNF_STRU));

    return VOS_TRUE;
}


VOS_UINT32 TAF_SPM_RcvImsaCallStopDtmfRsltInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_STOP_DTMF_RSLT_IND_STRU                  *pstDtmfRsltIndMsg       = VOS_NULL_PTR;
    TAF_CALL_EVT_DTMF_CNF_STRU                              stDtmfCnf;

    pstDtmfRsltIndMsg      = (IMSA_SPM_CALL_STOP_DTMF_RSLT_IND_STRU *)pstMsg;
    
    PS_MEM_SET(&stDtmfCnf, 0, sizeof(TAF_CALL_EVT_DTMF_CNF_STRU));
    
    stDtmfCnf.callId        = pstDtmfRsltIndMsg->ucCallId;
    stDtmfCnf.usClientId    = pstDtmfRsltIndMsg->usClientId;
    stDtmfCnf.opId          = pstDtmfRsltIndMsg->ucOpId;
    stDtmfCnf.enCause       = pstDtmfRsltIndMsg->enCause;
    stDtmfCnf.enDtmfState   = pstDtmfRsltIndMsg->enDtmfState;
    stDtmfCnf.ucDtmfCnt     = pstDtmfRsltIndMsg->ucDtmfCnt;

    
    TAF_CALL_SendMsg(pstDtmfRsltIndMsg->usClientId,
                     MN_CALL_EVT_STOP_DTMF_RSLT,
                     &stDtmfCnf,
                     sizeof(TAF_CALL_EVT_DTMF_CNF_STRU));

    return VOS_TRUE;
}



VOS_UINT32 TAF_SPM_RcvImsaCallSsCmdRslt_PreProc(
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_MSG_STRU             *pstImsaCallMsg           = VOS_NULL_PTR;
    TAF_SDC_CURC_RPT_CTRL_STRU         *pstCurcRptCtrl           = VOS_NULL_PTR;
    TAF_SDC_UNSOLICITED_RPT_CTRL_STRU  *pstUnsolicitedRptCtrl    = VOS_NULL_PTR;
    MN_CALL_INFO_STRU                   stCallInfo;

    pstImsaCallMsg      = (IMSA_SPM_CALL_MSG_STRU *)pstMsg;

    pstCurcRptCtrl          = TAF_SDC_GetCurcRptCtrl();
    pstUnsolicitedRptCtrl   = TAF_SDC_GetUnsolicitedRptCtrl();

    PS_MEM_SET(&stCallInfo, 0, sizeof(MN_CALL_INFO_STRU));

    PS_MEM_CPY(&stCallInfo, &(pstImsaCallMsg->stCallInfo), sizeof(MN_CALL_INFO_STRU));

    /* ���������ϱ����ȫ�ֱ�����ֵ */
    PS_MEM_CPY(stCallInfo.aucCurcRptCfg,
                pstCurcRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    PS_MEM_CPY(stCallInfo.aucUnsolicitedRptCfg,
                pstUnsolicitedRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    /* ����һ��MN_CALL_EVT_SS_CMD_RSLT��Ϣ������Ӧ��cliendId�ظ� */
    MN_SendClientEvent(pstImsaCallMsg->usClientId,
                       MN_CALLBACK_CS_CALL,
                       MN_CALL_EVT_SS_CMD_RSLT,
                       &stCallInfo);

    return VOS_TRUE;
}


VOS_UINT32 TAF_SPM_RcvImsaCallSsNotify_PreProc(
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_MSG_STRU             *pstImsaCallMsg           = VOS_NULL_PTR;
    TAF_SDC_CURC_RPT_CTRL_STRU         *pstCurcRptCtrl           = VOS_NULL_PTR;
    TAF_SDC_UNSOLICITED_RPT_CTRL_STRU  *pstUnsolicitedRptCtrl    = VOS_NULL_PTR;
    MN_CALL_INFO_STRU                   stCallInfo;

    pstImsaCallMsg          = (IMSA_SPM_CALL_MSG_STRU *)pstMsg;

    pstCurcRptCtrl          = TAF_SDC_GetCurcRptCtrl();
    pstUnsolicitedRptCtrl   = TAF_SDC_GetUnsolicitedRptCtrl();

    PS_MEM_SET(&stCallInfo, 0, sizeof(MN_CALL_INFO_STRU));

    PS_MEM_CPY(&stCallInfo, &(pstImsaCallMsg->stCallInfo), sizeof(MN_CALL_INFO_STRU));

    /* ���������ϱ����ȫ�ֱ�����ֵ */
    PS_MEM_CPY(stCallInfo.aucCurcRptCfg,
                pstCurcRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    PS_MEM_CPY(stCallInfo.aucUnsolicitedRptCfg,
                pstUnsolicitedRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    /* ����һ��MN_CALL_EVT_SS_NOTIFY��Ϣ������Ӧ��cliendId�ظ� */
    MN_SendClientEvent(pstImsaCallMsg->usClientId,
                       MN_CALLBACK_CS_CALL,
                       MN_CALL_EVT_SS_NOTIFY,
                       &stCallInfo);

    return VOS_TRUE;
}


VOS_UINT32 TAF_SPM_RcvImsaCallAllRelease_PreProc(
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_MSG_STRU             *pstImsaCallMsg           = VOS_NULL_PTR;
    TAF_SDC_CURC_RPT_CTRL_STRU         *pstCurcRptCtrl           = VOS_NULL_PTR;
    TAF_SDC_UNSOLICITED_RPT_CTRL_STRU  *pstUnsolicitedRptCtrl    = VOS_NULL_PTR;
    MN_CALL_INFO_STRU                   stCallInfo;

    /* ����IMS����д��ڱ�ʶ */
    TAF_SDC_SetImsCallExistFlg(VOS_FALSE);

    /* ������ز���GU�·�����Ҫ�ϱ�call all release�¼���Ӧ�� */
    if (VOS_FALSE == TAF_SDC_GetCsCallExistFlg())
    {
        pstImsaCallMsg          = (IMSA_SPM_CALL_MSG_STRU *)pstMsg;

        pstCurcRptCtrl          = TAF_SDC_GetCurcRptCtrl();
        pstUnsolicitedRptCtrl   = TAF_SDC_GetUnsolicitedRptCtrl();

        PS_MEM_SET(&stCallInfo, 0, sizeof(MN_CALL_INFO_STRU));

        PS_MEM_CPY(&stCallInfo, &(pstImsaCallMsg->stCallInfo), sizeof(MN_CALL_INFO_STRU));
    
        /* ���������ϱ����ȫ�ֱ�����ֵ */
        PS_MEM_CPY(stCallInfo.aucCurcRptCfg,
                    pstCurcRptCtrl->aucRptCfg,
                    MN_CALL_RPT_CFG_MAX_SIZE);
        
        PS_MEM_CPY(stCallInfo.aucUnsolicitedRptCfg,
                    pstUnsolicitedRptCtrl->aucRptCfg,
                    MN_CALL_RPT_CFG_MAX_SIZE);
        
        /* ����һ��MN_CALL_EVT_ALL_RELEASED��Ϣ������Ӧ��cliendId�ظ� */
        MN_SendClientEvent(pstImsaCallMsg->usClientId,
                           MN_CALLBACK_CS_CALL,
                           MN_CALL_EVT_ALL_RELEASED,
                           &stCallInfo);
    }

    return VOS_TRUE;
}


VOS_UINT32 TAF_SPM_RcvImsaCallMsg_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          ulRetFlg;
    TAF_SPM_PROC_IMSA_CALL_MSG_STRU    *pstTabHeader    = VOS_NULL_PTR;
    IMSA_SPM_CALL_MSG_STRU             *pstImsaCallMsg  = VOS_NULL_PTR;
    VOS_UINT32                          ulTabSize;

    pstImsaCallMsg      = (IMSA_SPM_CALL_MSG_STRU *)pstMsg;

    /* ��ȡ�������ı���׵�ַ */
    pstTabHeader = TAF_SPM_GetImsaCallMsgTabAddr();

    ulRetFlg     = VOS_TRUE;
    ulTabSize    = TAF_SPM_GetImsaCallMsgTabSize();

    /* ����EventType��� */
    for (i = 0; i < ulTabSize; i++)
    {
        if (pstImsaCallMsg->enEventType == pstTabHeader->ulMsgEvtType)
        {
            ulRetFlg = pstTabHeader->pfMsgFun(pstMsg);

            break;
        }

        pstTabHeader++;
    }

    return ulRetFlg;
}



VOS_UINT32 TAF_SPM_RcvImsaGetCallInfoCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_GET_CALL_INFO_CNF_STRU                   *pstImsaMsg  = VOS_NULL_PTR;

    pstImsaMsg = (IMSA_SPM_CALL_GET_CALL_INFO_CNF_STRU *)pstMsg;

    if (MN_CALL_MAX_NUM < pstImsaMsg->ucNumOfCalls)
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_SPM_RcvImsaGetCallInfoCnf_PreProc: call numbers is abnormal!");

        return VOS_TRUE;
    }

    if (CALL_IMSA_GET_CALL_INFO_REQ_TYPE_ATA == pstImsaMsg->enReqType)
    {
        /* ����������ATAʱ����TAFAGENT API��ѯ������Ϣ,��TAFAGENT�ظ� */
        TAF_SPM_SendTafAgentGetCallInfoCnf(pstImsaMsg);
    }
    else
    {
        /* ����������CLCC��ѯ������Ϣ,��AT�ظ� */
        TAF_SPM_SendAtGetCallInfoCnf(pstImsaMsg);
    }

    return VOS_TRUE;
}


VOS_UINT32 TAF_SPM_RcvImsaChannelInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CHANNEL_INFO_IND_STRU     *pstImsaMsg  = VOS_NULL_PTR;
    MN_CALL_EVT_CHANNEL_INFO_STRU       stChannelEvt;

    pstImsaMsg  = (IMSA_SPM_CHANNEL_INFO_IND_STRU *)pstMsg;

    /* ����һ����Ϣ����AT�ظ� */
    PS_MEM_SET(&stChannelEvt, 0x00, sizeof(MN_CALL_EVT_CHANNEL_INFO_STRU));

    stChannelEvt.enEvent                = MN_CALL_EVT_CHANNEL_INFO_IND;
    stChannelEvt.usClientId             = MN_GetRealClientId(pstImsaMsg->usClientId, WUEPS_PID_TAF);
    stChannelEvt.ucIsLocalAlertingFlag  = pstImsaMsg->ucIsLocalAlertingFlag;
    stChannelEvt.enVoiceDomain          = TAF_CALL_VOICE_DOMAIN_IMS;
    stChannelEvt.enCodecType            = pstImsaMsg->enCodecType;

    MN_SendReportMsg(MN_CALLBACK_CS_CALL, (VOS_UINT8 *)&stChannelEvt, sizeof(MN_CALL_EVT_CHANNEL_INFO_STRU));

    return VOS_TRUE;
}
VOS_UINT32 TAF_SPM_RcvImsaGetClprCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_GET_CLPR_CNF_STRU    *pstImsaMsg  = VOS_NULL_PTR;
    MN_CALL_CLPR_GET_CNF_STRU           stClprCnf;

    pstImsaMsg  = (IMSA_SPM_CALL_GET_CLPR_CNF_STRU *)pstMsg;

    /* ����һ����Ϣ����AT�ظ� */
    PS_MEM_SET(&stClprCnf, 0x00, sizeof(MN_CALL_CLPR_GET_CNF_STRU));

    stClprCnf.enEvent                   = MN_CALL_EVT_CLPR_SET_CNF;
    stClprCnf.stAppCtrl.usClientId      = pstImsaMsg->usClientId;
    stClprCnf.ulRet                     = pstImsaMsg->ulRet;
    stClprCnf.stRedirectInfo            = pstImsaMsg->stRedirectInfo;

    MN_SendReportMsg(MN_CALLBACK_CS_CALL, (VOS_UINT8 *)&stClprCnf, sizeof(MN_CALL_CLPR_GET_CNF_STRU));

    return VOS_TRUE;
}
VOS_UINT32 TAF_SPM_RcvImsaCallHold_PreProc(
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_MSG_STRU             *pstImsaMsg              = VOS_NULL_PTR;
    TAF_SDC_CURC_RPT_CTRL_STRU         *pstCurcRptCtrl          = VOS_NULL_PTR;
    TAF_SDC_UNSOLICITED_RPT_CTRL_STRU  *pstUnsolicitedRptCtrl   = VOS_NULL_PTR;
    MN_CALL_EVT_HOLD_STRU               stEvent;
    MN_CALL_INFO_STRU                   stCallInfo;

    pstCurcRptCtrl          = TAF_SDC_GetCurcRptCtrl();
    pstUnsolicitedRptCtrl   = TAF_SDC_GetUnsolicitedRptCtrl();


    pstImsaMsg  = (IMSA_SPM_CALL_MSG_STRU *)pstMsg;

    PS_MEM_SET(&stEvent, 0, sizeof(MN_CALL_EVT_HOLD_STRU));
    PS_MEM_SET(&stCallInfo, 0, sizeof(MN_CALL_INFO_STRU));

    stEvent.enEvent         = MN_CALL_EVT_HOLD;
    stEvent.usClientId      = MN_GetRealClientId(pstImsaMsg->usClientId, WUEPS_PID_TAF);
    stEvent.ucCallNum       = 1;
    stEvent.aucCallId[0]    = pstImsaMsg->stCallInfo.callId;
    stEvent.enVoiceDomain   = pstImsaMsg->stCallInfo.enVoiceDomain;

    PS_MEM_CPY(stEvent.aucCurcRptCfg,
                pstCurcRptCtrl->aucRptCfg,
                sizeof(stEvent.aucCurcRptCfg));
    PS_MEM_CPY(stEvent.aucUnsolicitedRptCfg,
            pstUnsolicitedRptCtrl->aucRptCfg,
            sizeof(stEvent.aucUnsolicitedRptCfg));

    /* ��HOLD�¼����͵�AT */
    if (VOS_TRUE == TAF_SDC_CheckRptCmdStatus(pstCurcRptCtrl->aucRptCfg,
                                                TAF_SDC_CMD_RPT_CTRL_BY_CURC,
                                                TAF_SDC_RPT_CMD_CALLSTATE))
    {
        MN_SendReportMsg(MN_CALLBACK_CS_CALL, (VOS_UINT8 *)&stEvent, sizeof(MN_CALL_EVT_HOLD_STRU));
    }

    /* ��HOLD�¼����͵�OAM */
    PS_MEM_CPY(&stCallInfo, &(pstImsaMsg->stCallInfo), sizeof(MN_CALL_INFO_STRU));

    MN_CS_SendMsgToOam(MN_CLIENT_ALL,
                       MN_CALL_EVT_HOLD,
                       MAPS_STK_PID,
                       &stCallInfo);

    return VOS_TRUE;
}


VOS_UINT32 TAF_SPM_RcvImsaCallRetrieve_PreProc(
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_MSG_STRU             *pstImsaMsg              = VOS_NULL_PTR;
    TAF_SDC_CURC_RPT_CTRL_STRU         *pstCurcRptCtrl          = VOS_NULL_PTR;
    TAF_SDC_UNSOLICITED_RPT_CTRL_STRU  *pstUnsolicitedRptCtrl   = VOS_NULL_PTR;
    MN_CALL_EVT_RETRIEVE_STRU           stEvent;
    MN_CALL_INFO_STRU                   stCallInfo;

    pstCurcRptCtrl          = TAF_SDC_GetCurcRptCtrl();
    pstUnsolicitedRptCtrl   = TAF_SDC_GetUnsolicitedRptCtrl();

    pstImsaMsg  = (IMSA_SPM_CALL_MSG_STRU *)pstMsg;

    PS_MEM_SET(&stEvent, 0, sizeof(MN_CALL_EVT_RETRIEVE_STRU));
    PS_MEM_SET(&stCallInfo, 0, sizeof(MN_CALL_INFO_STRU));

    stEvent.enEvent         = MN_CALL_EVT_RETRIEVE;
    stEvent.usClientId      = MN_GetRealClientId(pstImsaMsg->usClientId, WUEPS_PID_TAF);
    stEvent.ucCallNum       = 1;
    stEvent.aucCallId[0]    = pstImsaMsg->stCallInfo.callId;
    stEvent.enVoiceDomain   = pstImsaMsg->stCallInfo.enVoiceDomain;

    PS_MEM_CPY(stEvent.aucCurcRptCfg,
                pstCurcRptCtrl->aucRptCfg,
                sizeof(stEvent.aucCurcRptCfg));
    PS_MEM_CPY(stEvent.aucUnsolicitedRptCfg,
                pstUnsolicitedRptCtrl->aucRptCfg,
                sizeof(stEvent.aucUnsolicitedRptCfg));

    /* ��RETRIEVE�¼����͵�AT */
    if (VOS_TRUE == TAF_SDC_CheckRptCmdStatus(pstCurcRptCtrl->aucRptCfg,
                                                TAF_SDC_CMD_RPT_CTRL_BY_CURC,
                                                TAF_SDC_RPT_CMD_CALLSTATE))
    {
        MN_SendReportMsg(MN_CALLBACK_CS_CALL, (VOS_UINT8 *)&stEvent, sizeof(MN_CALL_EVT_RETRIEVE_STRU));
    }

    /* ��RETRIEVE�¼����͵�OAM */
    PS_MEM_CPY(&stCallInfo, &(pstImsaMsg->stCallInfo), sizeof(MN_CALL_INFO_STRU));

    MN_CS_SendMsgToOam(MN_CLIENT_ALL,
                       MN_CALL_EVT_RETRIEVE,
                       MAPS_STK_PID,
                       &stCallInfo);

    return VOS_TRUE;
}


VOS_UINT32 TAF_SPM_RcvImsaSsMsg_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstRcvMsg
)
{
    IMSA_SPM_SS_MSG_STRU               *pstImsaMsg = VOS_NULL_PTR;


    pstImsaMsg = (IMSA_SPM_SS_MSG_STRU *)pstRcvMsg;

    MN_SendClientEvent(pstImsaMsg->usClientId,
                       MN_CALLBACK_SS,
                       (VOS_UINT32)(pstImsaMsg->stSsEventInfo.SsEvent),
                       (VOS_VOID *)&(pstImsaMsg->stSsEventInfo));

    switch(pstImsaMsg->stSsEventInfo.SsEvent)
    {
        /* �������������SSҵ����Ҫ���ϱ�־ */
        case TAF_SS_EVT_REGISTERSS_CNF:
        case TAF_SS_EVT_USS_NOTIFY_IND:
        
            TAF_SDC_SetImsSsSrvExistFlg(VOS_TRUE);

            break;
            
        /* �����¼���Ҫ�����־ */
        case TAF_SS_EVT_ERROR:
        case TAF_SS_EVT_ERASESS_CNF:
        case TAF_SS_EVT_ACTIVATESS_CNF:
        case TAF_SS_EVT_DEACTIVATESS_CNF:
        case TAF_SS_EVT_INTERROGATESS_CNF:
        case TAF_SS_EVT_PROCESS_USS_REQ_CNF:
        case TAF_SS_EVT_USS_RELEASE_COMPLETE_IND:
        case TAF_SS_EVT_ERASE_CC_ENTRY_CNF:
        case TAF_SS_EVT_PROBLEM:
        
            TAF_SDC_SetImsSsSrvExistFlg(VOS_FALSE);
        
            break;
            
        /* �����¼�ʱ����Ҫ����/�����־ */
        default:

            break;
    }

    return VOS_TRUE;
}
VOS_UINT32 TAF_SPM_RcvImsaNormalRegStatusNotify_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRst;

    /* ���������Ϣ */
    ulRst = TAF_SPM_ProcImsaNormalRegStatusNotify(pstMsg);

    return ulRst;
}
VOS_UINT32 TAF_SPM_RcvInternalDomainSelInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRst;

    /* ���������Ϣ */
    ulRst = TAF_SPM_ProcBufferedMsgInQueue();

    return ulRst;
}
VOS_UINT32 TAF_SPM_RcvMmaServiceStatusChangeNotify_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRlst;
    TAF_SPM_SERVICE_STATUS_ENUM_UINT8   enLastPsStatus;
    TAF_SPM_SERVICE_STATUS_ENUM_UINT8   enCurrPsStatus;
    
    /* ��ȡ�ϴ�PS����״̬ */
    enLastPsStatus = TAF_SPM_GetLastPsServiceStatus();

    /* ���µ�ǰ��PS����״̬��SPM CONTEXT�� */
    enCurrPsStatus = (TAF_SPM_SERVICE_STATUS_ENUM_UINT8)TAF_SDC_GetPsServiceStatus();   
    if (TAF_SPM_SERVICE_STATUS_BUTT < enCurrPsStatus)
    {
        return  VOS_FALSE;
    }
    TAF_SPM_SetLastPsServiceStatus(enCurrPsStatus);

    if (VOS_TRUE == TAF_SPM_IsNeedtoWaitImsRegStatus(enLastPsStatus))
    {
        /* �ȴ���IMSע���� */
        return VOS_TRUE;
    }  

    /* ���������Ϣ */
    ulRlst = TAF_SPM_ProcBufferedMsgInQueue();

    return ulRlst;

}


VOS_UINT32 TAF_SPM_RcvMmaNetworkCapabilityChangeNotify_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRlst;
    
    /* ���������Ϣ */
    ulRlst = TAF_SPM_ProcBufferedMsgInQueue();

    return ulRlst;

}
VOS_UINT32 TAF_SPM_RcvMmaRatChangeNotify_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRlst;

    /* ���������Ϣ�������������仯ʱ�Ĵ���һ�� */
    ulRlst = TAF_SPM_ProcBufferedMsgInQueue();

    return ulRlst;
}
VOS_UINT32 TAF_SPM_RcvMmaImsVoiceCapInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRlst;

    /* ���������Ϣ�������������仯ʱ�Ĵ���һ�� */
    ulRlst = TAF_SPM_ProcBufferedMsgInQueue();

    return ulRlst;
}
VOS_UINT32 TAF_SPM_RcvCcSrvReqProtectTimerExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRst;

    /* ���������Ϣ */
    ulRst = TAF_SPM_ProcCcSrvReqProtectTimerExpired();

    return ulRst;
}
VOS_UINT32 TAF_SPM_RcvSmsSrvReqProtectTimerExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRst;

    /* ���������Ϣ */
    ulRst = TAF_SPM_ProcSmsSrvReqProtectTimerExpired(pstMsg);

    return ulRst;
}
VOS_UINT32 TAF_SPM_RcvSsSrvReqProtectTimerExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRst;

    /* ���������Ϣ */
    ulRst = TAF_SPM_ProcSsSrvReqProtectTimerExpired();

    return ulRst;
}
VOS_UINT32 TAF_SPM_RcvImsaCallMsgSyncInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucNum;
    VOS_UINT8                           i;    

    /* �����ز�δ�򿪣�ֱ�ӷ��� */
    if (VOS_TRUE == TAF_SDC_GetCallRedailFromImsToCsSupportFlag())
    {
        /* ����SRVCC����ǰ�ĺ��������߻����ز����̣�IMSA��֤�ڸ���Ϣǰ�ȷ���SPM,
         * ��ˣ�����յ�����Ϣʱ���ز���������Ȼ����Ϣ����һ������SRVCC�������յ���
         * ������������ز����棬����������Ϣ��callģ�鸺����
         */    
        ucNum = TAF_SPM_GetNumberOfCallRedialBuffer();

        for (i = 0; i < ucNum; i++)
        {
            TAF_SPM_FreeSpecificedIndexCallRedialBuffer(i);
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 TAF_SPM_RcvImsaCallInviteNewPtptCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    IMSA_SPM_CALL_INVITE_NEW_PTPT_CNF_STRU                 *pstNewPtptCnf         = VOS_NULL_PTR;
    TAF_SDC_CURC_RPT_CTRL_STRU                             *pstCurcRptCtrl        = VOS_NULL_PTR;
    TAF_SDC_UNSOLICITED_RPT_CTRL_STRU                      *pstUnsolicitedRptCtrl = VOS_NULL_PTR;    
    MN_CALL_INFO_STRU                                       stCallInfo;    

    pstNewPtptCnf = (IMSA_SPM_CALL_INVITE_NEW_PTPT_CNF_STRU *)pstMsg;

    pstCurcRptCtrl          = TAF_SDC_GetCurcRptCtrl();
    pstUnsolicitedRptCtrl   = TAF_SDC_GetUnsolicitedRptCtrl();

    PS_MEM_SET(&stCallInfo, 0, sizeof(MN_CALL_INFO_STRU));    

    stCallInfo.clientId = pstNewPtptCnf->usClientId;
    stCallInfo.opId     = pstNewPtptCnf->ucOpId;
    stCallInfo.enCause  = pstNewPtptCnf->enCause;
    
    /* ���������ϱ����ȫ�ֱ�����ֵ */
    PS_MEM_CPY(stCallInfo.aucCurcRptCfg,
                pstCurcRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    PS_MEM_CPY(stCallInfo.aucUnsolicitedRptCfg,
                pstUnsolicitedRptCtrl->aucRptCfg,
                MN_CALL_RPT_CFG_MAX_SIZE);

    /* ����һ��MN_CALL_EVT_CALL_ORIG_CNF��Ϣ������Ӧ��cliendId�ظ� */
    MN_SendClientEvent(stCallInfo.clientId,
                       MN_CALLBACK_CS_CALL,
                       MN_CALL_EVT_CALL_ORIG_CNF,
                       &stCallInfo);

    /* ����Err_Ind��Ϣ��cliendIdΪ�㲥���� */
    MN_CALL_ReportErrIndEvent(MN_CLIENT_ALL, 0, stCallInfo.enCause, 0);

    return VOS_TRUE;    
}
#endif
VOS_UINT32 TAF_SPM_RcvMmaPowerOffInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

#if (FEATURE_ON == FEATURE_IMS)
    TAF_SPM_ProcMmaPowerOffInd();
#endif    

    /* return VOS_FASLE�� APS also need this message */
    return VOS_FALSE;
}
VOS_UINT32 TAF_SPM_IsSsServiceReqAllowed_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_PH_ERR_CODE                    *penCause
)
{
    VOS_UINT32                          ulIsUsimValid;


    *penCause   = TAF_ERR_NO_ERROR;
    
    ulIsUsimValid   = TAF_SDC_IsUsimStausValid();
    
    /* SS is allowed if phone mode is power on */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
        *penCause   = TAF_ERR_SS_POWER_OFF;
        return VOS_FALSE;
    }

    /* forbid SS when USIM service is not available */
    /* ��Ҫ�ж���������Ч,��Ϊ������Ч������ܻ�disable LTE��GU��,��Ҫ��GU�¼������� */
    if ( (VOS_FALSE == TAF_SPM_IsUsimServiceAvailable())
      || (VOS_FALSE == ulIsUsimValid) )
    {
        *penCause   = TAF_ERR_USIM_SIM_INVALIDATION;
        return VOS_FALSE;
    }

    return VOS_TRUE;
}
VOS_UINT32 TAF_SPM_IsSmsServiceReqAllowed_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    VOS_UINT32                         *pulCause
)
{
    *pulCause   = MN_ERR_NO_ERROR;
        
    /* SMS is allowed if phone mode is power on */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
        *pulCause    = MN_ERR_CLASS_SMS_POWER_OFF;
        return VOS_FALSE;
    }

    /* forbid SMS when USIM service is not available */
    if (VOS_FALSE == TAF_SPM_IsUsimServiceAvailable())
    {
        *pulCause    = MN_ERR_CLASS_SMS_NOUSIM;
        return VOS_FALSE;
    }

    /* �ж���������Чʱ��ֱ�Ӹ�AT�ظ�ʧ�� */
    if (VOS_FALSE == TAF_SDC_IsUsimStausValid())
    {
        *pulCause    = MN_ERR_CLASS_SMS_UNAVAILABLE;
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


