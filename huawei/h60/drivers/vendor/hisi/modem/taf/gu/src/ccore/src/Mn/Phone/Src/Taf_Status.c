
#include "om.h"
#include "MmaAppLocal.h"
#include "rfa.h"
#include "NasOmInterface.h"
#include "TTFMemInterface.h"
#include "NVIM_Interface.h"
#include "DrvInterface.h"
/* ɾ��ExtAppMmcInterface.h*/

#include "TafAppMma.h"
#include "TafMmcInterface.h"
#if ( FEATURE_ON == FEATURE_LTE )
#include "msp_nvim.h"
#endif
#include "NasUtranCtrlInterface.h"

#include "TafSdcLib.h"
#include "Taf_Status.h"

#include "NasUsimmApi.h"
#include "TafLog.h"
#include "TafMmaCtx.h"
#include "TafMmaSndImsa.h"

#include "NasMtaInterface.h"

#include "NasStkInterface.h"

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
#include "MtcMmaInterface.h"
#endif

#include "TafMmaSndTaf.h"
#include "TafMmaProcNvim.h"

#include "TafMmaSndApp.h"

#ifdef  __cplusplus
  #if  __cplusplus
  extern "C"{
  #endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_TAF_STATUS_C

/* STATUS ȫ�ֹ������ */
STATUS_CONTEXT_STRU                     g_StatusContext;
extern TAF_MMA_GLOBAL_VALUE_ST          gstMmaValue;



extern MMA_ME_PERSONALISATION_STATUS_STRU  g_stMmaMePersonalisationStatus;

MN_PH_REG_TIME_INFO_STRU            g_stRegTimeInfo = {0, 0, 0, 0};

extern MMA_TIMER_ST                            g_stSyscfgWaitDetachCnfTimer;

extern MMA_TIMER_ST                            g_stPowerDownDelayTimer;



extern VOS_UINT32                       g_ucUsimHotOutFlag;




VOS_UINT32                              aStaTimerLen[STA_TIMER_BUTT] = {0};

extern TAF_MMA_LOCATION_STATUS_EVENT_INFO_STRU g_stMmsStkLocStaSysInfo;

VOS_UINT8 g_MMA_IccStatus = 0xFE;

extern MMA_DEACT_SIM_WHEN_POWEROFF_ENUM_UINT8  g_ucMmaDeactSimWhenPoweroff;

#define STA_INFO_PRINT     PS_PRINT_INFO
#define STA_NORMAL_PRINT   PS_PRINT_NORMAL
#define STA_WARNING_PRINT  PS_PRINT_WARNING
#define STA_ERROR_PRINT    PS_PRINT_ERROR


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*----------- STATUS ��TAFM�ṩ��API�ӿں��� ------------*/
/*=======================================================*/




VOS_UINT32 Sta_MsgReqAbort ( VOS_UINT32 ulAbortType )
{
    VOS_UINT32      ulEvent;        /* �¼����� */

    /* δ�������������� */
    switch ( g_StatusContext.ulFsmState )
    {
        case STA_FSM_RESTART:
            ulEvent = TAF_PH_EVT_OPER_MODE_CNF;
            break;
        case STA_FSM_STOP:
            ulEvent = TAF_PH_EVT_OPER_MODE_CNF;
            break;
        case STA_FSM_PLMN_LIST:
            ulEvent = TAF_PH_EVT_PLMN_LIST_CNF;
            break;
        case STA_FSM_PLMN_SEL:
            ulEvent = TAF_PH_EVT_PLMN_SEL_CNF;
            break;
        case STA_FSM_PLMN_RESEL:
            ulEvent = TAF_PH_EVT_PLMN_RESEL_CNF;
            break;
        case STA_FSM_SYS_CFG_SET:
            ulEvent = TAF_PH_EVT_SYS_CFG_CNF;
            break;

        default:
            /* δָ����������̣����ش��� */
            return STA_ERROR;
    }

    /* ȷ�������¼��������� */
    if ( STA_MSGABORT_OK != ulAbortType )
    {
        g_StatusContext.ucOperateType = STA_OP_PHONEERROR;
    }

    Sta_EventReport ( g_StatusContext.ulCurOpId, ulEvent );

    return STA_SUCCESS;
}


VOS_UINT32 Sta_Attach (VOS_UINT32 ulOpID, VOS_UINT32 ulAttachType, TAF_MMA_EPS_ATTACH_REASON_ENUM_UINT8 enAttachReason)
{
    VOS_UINT8               ucSubNum;   /* ���õ���״̬���*/
    VOS_UINT32               ulRet;

    ucSubNum    = 0;

    /* ��ڲ����Ϸ��Լ�� */
    if ( Sta_IsInvalidOpId (ulOpID) )
    {
        STA_TRACE (STA_WARNING_PRINT,"Sta_Attach():WARNING:parameter invalid ");
        return STA_ERROR;
    }

    /* ���ATTACH�������ͺϷ��� */
    if ( ulAttachType > STA_ATTACH_TYPE_GPRS_IMSI )
    {
        g_StatusContext.PhoneError    = TAF_ERR_PARA_ERROR;
        g_StatusContext.ucOperateType = STA_OP_PHONEERROR;

        /* �ϱ�APP��ǰ״̬�¼� */
        Sta_EventReport (ulOpID, TAF_PH_EVT_ATTACH_CNF);

        /* ��ӡ���Ը�����Ϣ */
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_Attach():WARNING:parameter is wrong");
        return STA_ERROR;
    }

    /* ATTACH״̬������������쳣*/
    if (VOS_TRUE != TAF_MMA_IsEnableDeAttach())
    {/* �����NULL״̬�����ش��� */
        g_StatusContext.PhoneError    = TAF_ERR_PHONE_MSG_UNMATCH;

        /* �¼��ϱ���Ϣ��������� */
        g_StatusContext.ucOperateType = STA_OP_PHONEERROR;

        /* �ϱ�APP��ǰ״̬�¼� */
        Sta_EventReport(ulOpID, TAF_PH_EVT_ATTACH_CNF);

        /* ��ӡ���Ը�����Ϣ */
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_Attach():WARNING:system FSM state is wrong");
        return STA_ERROR;     /* ���󷵻�*/
    }/* ATTACH״̬������������쳣*/

    /* ��鵱ǰϵͳ������״̬ */
    /* �ж��Ƿ�����������*/
    /* ͬ�������������Ƿ��Ѿ��õ���Ӧ��һ������������Ӧ�Ļ�������*/
    /* �������������ͬ��������PS��CS���ϵ�ATTACH�㲻ͬ������*/
    switch (ulAttachType)
    {
        case STA_ATTACH_TYPE_GPRS: /* PS*/
            ucSubNum = (VOS_UINT8)Sta_FindIdelFsmSub (STA_FSMSUB_ATTACH_PS);
            break;

        case STA_ATTACH_TYPE_IMSI: /* CS*/
            ucSubNum = (VOS_UINT8)Sta_FindIdelFsmSub (STA_FSMSUB_ATTACH_CS);
            break;

        case STA_ATTACH_TYPE_GPRS_IMSI: /* PS&CS*/
            ucSubNum = (VOS_UINT8)Sta_FindIdelFsmSub (STA_FSMSUB_ATTACH_CSPS);
            break;

        default:    /* �������� */
            g_StatusContext.PhoneError    = TAF_ERR_PARA_ERROR;
            g_StatusContext.ucOperateType = STA_OP_PHONEERROR;
            /* �ϱ�APP��ǰ״̬�¼� */
            Sta_EventReport (ulOpID, TAF_PH_EVT_ATTACH_CNF);

            /* ������ٵ�����Ϣ */
            STA_TRACE (STA_WARNING_PRINT,
                "Sta_Attach():WARNING:parameter is wrong");
            return STA_ERROR;
    }

    /* ��ЧFSM״̬���� */
    if ( STA_FSMSUB_INDEX_NULL == ucSubNum )
    {
        g_StatusContext.PhoneError    = TAF_ERR_PHONE_MSG_UNMATCH;
        g_StatusContext.ucOperateType = STA_OP_PHONEERROR;

        /* �ϱ�APP��ǰ״̬�¼� */
        Sta_EventReport (ulOpID, TAF_PH_EVT_ATTACH_CNF);

        /* ������ٵ�����Ϣ */
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_Attach():WARNING:system FSM state is wrong");
        return STA_ERROR;
    }

    ulRet = TAF_MMA_SndMmcAttachReq(ulOpID, ulAttachType, enAttachReason);

    if ( VOS_ERR == ulRet )
    {
        STA_TRACE (STA_ERROR_PRINT,
            "Sta_Attach():  failed  ");
        return STA_ERROR;       /* ���󷵻� */
    }

    /* ��¼��ǰ�Ĳ������̱�ʶ�� */
    g_StatusContext.ulCurOpId = ulOpID;
    /*modified by liuyang id:48197 date:2005-10-9 for V100R001 Begin*/
    /*������״̬*/
    g_StatusContext.ulFsmState = STA_FSM_DE_ATTACH;
    /*modified by liuyang id:48197 date:2005-10-9 for V100R001 End*/

    /* �ϱ�APP��ǰ״̬�¼� */
    /* Ԥ���ĺ����ӿڣ��ݲ�ʹ�� */
    /* ulRet = Sta_EventReport (ulOpID, TAF_PH_EVT_ATTACH);*/

    /* ������״̬��־��������������ʱ�� */
    if ( STA_ATTACH_TYPE_GPRS == ulAttachType )
    {
        TAF_SDC_SetPsAttachAllowFlg(VOS_TRUE);

        /* PS��ATTACH */
        g_StatusContext.aucFsmSubFlag[ucSubNum]
            = STA_FSMSUB_ATTACH_PS;
        /*����PS��ATTACH��־;*/
        g_StatusContext.aFsmSub[ucSubNum].ucFsmStateSub
            = STA_FSMSUB_ATTACH_PS;
        g_StatusContext.aFsmSub[ucSubNum].TimerName
            = STA_TIMER_ATTACH_PS;

        /*����PS��ATTACH��ʱ��;*/
        ulRet = NAS_StartRelTimer(&g_StatusContext.ahStaTimer[ucSubNum],
                                  WUEPS_PID_MMA,
                                  aStaTimerLen[STA_TIMER_ATTACH_PS],
                                  TAF_MMA,
                                  MMA_MAKELPARAM(ucSubNum, STA_TIMER_ATTACH_PS),
                                  VOS_RELTIMER_NOLOOP
                                     );
        if ( VOS_OK != ulRet )
        {
            STA_TRACE (STA_ERROR_PRINT,
                "Sta_Attach():ERROR: timer start failed");

            /* �ָ�ԭ������״̬��־ */
            if ( STA_ERROR
                == Sta_UpdateFsmFlag(STA_FSMSUB_ATTACH_PS,
                        0, STA_UPDATE_SINGL_CLEAR) )
            {
                STA_TRACE (STA_WARNING_PRINT,
                    "Sta_Attach():WARNING:timer clear failed");
            }

            return STA_ERROR;       /* ���󷵻� */
        }
        /* ������״̬���������ʶ�� */
        g_StatusContext.aFsmSub[ucSubNum].ulOpId = ulOpID;
        g_StatusContext.ucAttachReq++;
        /* BEGIN: Added by liuyang, id:48197, 2005/10/12 */
        g_StatusContext.aucStaPhoneOperator[ucSubNum] = TAF_PH_EVT_PS_ATTACH_CNF;
        /* END:   Added by liuyang, id:48197, 2005/10/12 */
    }
    else if ( STA_ATTACH_TYPE_IMSI == ulAttachType )
    {
        TAF_SDC_SetCsAttachAllowFlg(VOS_TRUE);

        /* CS��ATTACH */
        g_StatusContext.aucFsmSubFlag[ucSubNum]
            = STA_FSMSUB_ATTACH_CS;
        /* ����CS��ATTACH��־;*/
        g_StatusContext.aFsmSub[ucSubNum].ucFsmStateSub
            = STA_FSMSUB_ATTACH_CS;
        g_StatusContext.aFsmSub[ucSubNum].TimerName
            = STA_TIMER_ATTACH_CS;

        /*����CS��ATTACH��ʱ��;*/
        ulRet = NAS_StartRelTimer (
            &g_StatusContext.ahStaTimer[ucSubNum],
            WUEPS_PID_MMA,
            aStaTimerLen[STA_TIMER_ATTACH_CS],
            TAF_MMA,
            MMA_MAKELPARAM(ucSubNum, STA_TIMER_ATTACH_CS),
            VOS_RELTIMER_NOLOOP
             );
        if ( VOS_OK != ulRet )
        {
            STA_TRACE (STA_ERROR_PRINT,
                "Sta_Attach():ERROR:timer start failed");

            /* �ָ�ԭ������״̬��־ */
            if ( STA_ERROR
                == Sta_UpdateFsmFlag(STA_FSMSUB_ATTACH_CS,
                        0, STA_UPDATE_SINGL_CLEAR) )
            {
                STA_TRACE (STA_WARNING_PRINT,
                    "Sta_Attach():WARNING:timer clear failed");
            }

            return STA_ERROR;       /* ���󷵻� */
        }
        /* ������״̬���������ʶ�� */
        g_StatusContext.aFsmSub[ucSubNum].ulOpId = ulOpID;
        g_StatusContext.ucAttachReq++;
        /* BEGIN: Added by liuyang, id:48197, 2005/10/12 */
        g_StatusContext.aucStaPhoneOperator[ucSubNum] = TAF_PH_EVT_CS_ATTACH_CNF;
        /* END:   Added by liuyang, id:48197, 2005/10/12 */
    }
    else
    {
        /* CS&PS ATTATCH */
        TAF_SDC_SetPsAttachAllowFlg(VOS_TRUE);

        TAF_SDC_SetCsAttachAllowFlg(VOS_TRUE);

        g_StatusContext.aucFsmSubFlag[0]
            = STA_FSMSUB_ATTACH_CSPS;
        /*����PS��ATTACH��־;*/
        g_StatusContext.aFsmSub[0].ucFsmStateSub
            = STA_FSMSUB_ATTACH_PS;
        g_StatusContext.aFsmSub[0].TimerName
            = STA_TIMER_ATTACH_PS;

        /*����PS��ATTACH��ʱ��;*/
        ulRet = NAS_StartRelTimer (
            &g_StatusContext.ahStaTimer[0],
            WUEPS_PID_MMA,
            aStaTimerLen[STA_TIMER_ATTACH_PS],
            TAF_MMA,
            MMA_MAKELPARAM(0, STA_TIMER_ATTACH_PS),
            VOS_RELTIMER_NOLOOP
            );
        if ( VOS_OK != ulRet )
        {
            STA_TRACE (STA_ERROR_PRINT,
                "Sta_Attach():ERROR:timer start failed");

            /* �ָ�ԭ������״̬��־ */
            if ( STA_ERROR
                == Sta_UpdateFsmFlag(STA_FSMSUB_ATTACH_PS,
                        0, STA_UPDATE_SINGL_CLEAR) )
            {
                STA_TRACE (STA_WARNING_PRINT,
                    "Sta_Attach():WARNING:timer clear failed");
            }
            return STA_ERROR;       /* ���󷵻� */
        }
        /* ������״̬���������ʶ�� */
        g_StatusContext.aFsmSub[0].ulOpId = ulOpID;
        g_StatusContext.ucAttachReq++;
        /* BEGIN: Added by liuyang, id:48197, 2005/10/12 */
        g_StatusContext.aucStaPhoneOperator[0] = TAF_PH_EVT_ATTACH_CNF;
        /* END:   Added by liuyang, id:48197, 2005/10/12 */

        /* CS��ATTACH */
        g_StatusContext.aucFsmSubFlag[1]
            = STA_FSMSUB_ATTACH_CSPS;
        /* ����CS��ATTACH��־;*/
        g_StatusContext.aFsmSub[1].ucFsmStateSub
            = STA_FSMSUB_ATTACH_CS;
        g_StatusContext.aFsmSub[1].TimerName
            = STA_TIMER_ATTACH_CS;

        /*����CS��ATTACH��ʱ��;*/
        ulRet = NAS_StartRelTimer (
            &g_StatusContext.ahStaTimer[1],
            WUEPS_PID_MMA,
            aStaTimerLen[STA_TIMER_ATTACH_CS],
            TAF_MMA,
            MMA_MAKELPARAM(1, STA_TIMER_ATTACH_CS),
            VOS_RELTIMER_NOLOOP
            );
        if ( VOS_OK != ulRet )
        {
            STA_TRACE (STA_ERROR_PRINT,
                "Sta_Attach():ERROR:timer start failed");

            /* �ָ�ԭ������״̬��־ */
            if ( STA_ERROR
                == Sta_UpdateFsmFlag(STA_FSMSUB_ATTACH_CS,
                        0, STA_UPDATE_SINGL_CLEAR) )
            {
                STA_TRACE (STA_WARNING_PRINT,
                    "Sta_Attach():WARNING:timer clear failed");
            }
            return STA_ERROR;       /* ���󷵻� */
        }
        /* ������״̬���������ʶ�� */
        g_StatusContext.aFsmSub[1].ulOpId = ulOpID;
        g_StatusContext.ucAttachReq++;
        /* BEGIN: Added by liuyang, id:48197, 2005/10/12 */
        g_StatusContext.aucStaPhoneOperator[1] = TAF_PH_EVT_ATTACH_CNF;
        /* END:   Added by liuyang, id:48197, 2005/10/12 */
    }
     /*
    ��յ�ǰ��PLMNid
    modified by liuyang id:48197 date:2005-9-5 for V200R001
    */
    /* Sta_ClearCurPlmnId( &(g_StatusContext.CurPlmnId) ); */
    return STA_SUCCESS;             /* �������� */
}




VOS_UINT32 TAF_MMA_GetDetachFsmSubNum(
    VOS_UINT32                          ulDetachType,
    VOS_UINT8                          *pucFsmSubNum

)
{
    /* ����DETACH TYPE�ҵ���Ӧ����״̬�� */
    switch (ulDetachType)
    {
        case STA_PS_DETACH:

            *pucFsmSubNum = (VOS_UINT8)Sta_FindIdelFsmSub (STA_FSMSUB_DETACH_PS);
            break;

        case STA_CS_DETACH:

            *pucFsmSubNum = (VOS_UINT8)Sta_FindIdelFsmSub (STA_FSMSUB_DETACH_CS);
            break;

        case STA_PS_CS_DETACH:

            *pucFsmSubNum = (VOS_UINT8)Sta_FindIdelFsmSub (STA_FSMSUB_DETACH_CSPS);
            break;

        default:

            /* ���󷵻� */
            return VOS_FALSE;
    }

    return VOS_TRUE;
}



VOS_UINT32 TAF_MMA_IsDetachCnfEnable(VOS_VOID)
{    
    /* DETACHCNF״̬������������쳣*/
    if ((g_StatusContext.ulFsmState != STA_FSM_DE_ATTACH)
     && (g_StatusContext.ulFsmState != STA_FSM_ENABLE)
     && (g_StatusContext.ulFsmState != STA_FSM_SYS_CFG_SET))
    {
        NAS_ERROR_LOG(WUEPS_PID_MMA, "TAF_MMA_IsDetachCnfEnable ERROR! FSM STATE NOT OK!!!");

        /* �������DE_ATTACH״̬��ENABLE״̬�����ش��� */
        return VOS_FALSE;
    }

    return VOS_TRUE;
}



VOS_UINT32 Sta_Detach (
    VOS_UINT32                          ulOpID,
    VOS_UINT32                          ulDetachType,
    TAF_MMA_DETACH_CAUSE_ENUM_UINT8     enDetachCause)
{

    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

    VOS_UINT8       ucSubNum;   /* ���õ���״̬���*/
    VOS_UINT32      ulRet;
    /* void        *pRet; */
    VOS_UINT32      ulTimerIndex;


    ucSubNum    = 0;

    if (VOS_TRUE != TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_DETACH_REQ, &ucCtxIndex))
    {
        NAS_ERROR_LOG(WUEPS_PID_MMA, "Sta_Detach ERROR! NOT FIND DETACH REQ");

        return STA_ERROR;
    }

    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

    /* ��ڲ����Ϸ��Լ�� */
    if ( Sta_IsInvalidOpId (ulOpID) )
    {
        STA_TRACE(STA_WARNING_PRINT,"Sta_Detach():WARNING:parameter invalid");
        return STA_ERROR;
    }

    /* ���DETACH�������ͺϷ��� */
    if ( ulDetachType > STA_PS_CS_DETACH )
    {

        /* ��APP����DETACH CNF */
        TAF_MMA_SndDetachCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                             TAF_MMA_APP_OPER_RESULT_FAILURE,
                             TAF_ERR_PARA_ERROR);

        TAF_MMA_ClearOperCtx(ucCtxIndex);

        /* ��ӡ���Ը�����Ϣ */
        STA_TRACE (STA_WARNING_PRINT, "Sta_Detach():WARNING:parameter wrong");

        return STA_ERROR;
    }

    /* DETACH״̬������������쳣*/
    if (VOS_TRUE != TAF_MMA_IsEnableDeAttach())

    {

        /* ��APP����DETACH CNF */
        TAF_MMA_SndDetachCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                             TAF_MMA_APP_OPER_RESULT_FAILURE,
                             TAF_ERR_PHONE_MSG_UNMATCH);

        TAF_MMA_ClearOperCtx(ucCtxIndex);

        /* ������ٵ�����Ϣ */
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_Detach():WARNING:system FSM state is wrong");

        return STA_ERROR;       /* ���󷵻�*/
    }

    /* ��鵱ǰϵͳ������״̬ */
    /* �ж��Ƿ�����������*/
    /* ͬ�������������Ƿ��Ѿ��õ���Ӧ��һ������������Ӧ�Ļ�������*/
    /* �������������ͬ��������PS��CS���ϵ�ATTACH�㲻ͬ������*/

    if ( VOS_FALSE == TAF_MMA_GetDetachFsmSubNum(ulDetachType, &ucSubNum) )
    {
        /* ������ٵ�����Ϣ */
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_Detach():WARNING:parameters is wrong");


        /* ��APP����DETACH CNF */
        TAF_MMA_SndDetachCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                             TAF_MMA_APP_OPER_RESULT_FAILURE,
                             TAF_ERR_PARA_ERROR);

        TAF_MMA_ClearOperCtx(ucCtxIndex);

        return STA_ERROR;   /* ���󷵻� */
    }

    /* ��ЧFSM״̬���� */
    if ( STA_FSMSUB_INDEX_NULL == ucSubNum )
    {

        /* ��APP����DETACH CNF */
        TAF_MMA_SndDetachCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                             TAF_MMA_APP_OPER_RESULT_FAILURE,
                             TAF_ERR_PHONE_MSG_UNMATCH);

        TAF_MMA_ClearOperCtx(ucCtxIndex);

        /* ������ٵ�����Ϣ */
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_Detach():WARNING:system FSM state is wrong");

        return STA_ERROR;
    }

    ulRet = TAF_MMA_SndMmcDetachReq(ulOpID, ulDetachType, enDetachCause);

    if ( VOS_ERR == ulRet )
    {
        STA_TRACE (STA_ERROR_PRINT,
            "Sta_Detach():ERROR: failed");
        return STA_ERROR;       /* ���󷵻� */
    }

    /* ��¼��ǰ�Ĳ������̱�ʶ�� */
    g_StatusContext.ulCurOpId = ulOpID;

    /*modified by liuyang id:48197 date:2005-10-9 for V100R001 Begin*/
    /*������״̬*/
    g_StatusContext.ulFsmState = STA_FSM_DE_ATTACH;
    /*modified by liuyang id:48197 date:2005-10-9 for V100R001 End*/

    /* �ϱ�APP��ǰ״̬�¼� */
    /* Ԥ���ĺ����ӿڣ��ݲ�ʹ�� */
    /*ulRet = Sta_EventReport (ulOpID, TAF_PH_EVT_DETACH);*/

    /* ����DETACH��״̬��־��������״̬������ʱ�� */
    if ( ulDetachType == STA_PS_DETACH )
    {/* PS��DETACH */
        g_StatusContext.aucFsmSubFlag[ucSubNum]
            = STA_FSMSUB_DETACH_PS;

        /*����PS��DETACH��־;*/
        g_StatusContext.aFsmSub[ucSubNum].ucFsmStateSub
            = STA_FSMSUB_DETACH_PS;
        g_StatusContext.aFsmSub[ucSubNum].TimerName
            = STA_TIMER_DETACH_PS;

        /*����PS��DETACH��ʱ��;*/
        ulRet = NAS_StartRelTimer (
            &g_StatusContext.ahStaTimer[ucSubNum],
            WUEPS_PID_MMA,
            aStaTimerLen[STA_TIMER_DETACH_PS],
            TAF_MMA,
            MMA_MAKELPARAM(ucSubNum, STA_TIMER_DETACH_PS),
            VOS_RELTIMER_NOLOOP
             );
        if ( VOS_OK != ulRet )
        {
            STA_TRACE (STA_ERROR_PRINT,
                "Sta_Detach():ERROR:timer start failed");

            /* �ָ�ԭ������״̬��־ */
            if ( STA_ERROR
                == Sta_UpdateFsmFlag(STA_FSMSUB_DETACH_PS,
                        0, STA_UPDATE_SINGL_CLEAR) )
            {
                STA_TRACE (STA_WARNING_PRINT,
                    "Sta_Detach():WARNING:timer clear failed");
            }
            return STA_ERROR;
        }
        /* ��¼��ǰ�Ĳ������̱�ʶ�� */
        g_StatusContext.aFsmSub[ucSubNum].ulOpId = ulOpID;
        g_StatusContext.ucDetachReq++;

        /* BEGIN: Added by liuyang, id:48197, 2005/10/12 */
        g_StatusContext.aucStaPhoneOperator[ucSubNum] = TAF_PH_EVT_PS_DETACH_CNF;
        /* END:   Added by liuyang, id:48197, 2005/10/12 */

    }
    else if ( ulDetachType == STA_CS_DETACH )
    {/* CS��DETACH */
        g_StatusContext.aucFsmSubFlag[ucSubNum]
            = STA_FSMSUB_DETACH_CS;

        /* ����CS��DETACH��־;*/
        g_StatusContext.aFsmSub[ucSubNum].ucFsmStateSub
            = STA_FSMSUB_DETACH_CS;
        g_StatusContext.aFsmSub[ucSubNum].TimerName
            = STA_TIMER_DETACH_CS;

        /*����CS��ATTACH��ʱ��;*/
        ulRet = NAS_StartRelTimer (
                    &g_StatusContext.ahStaTimer[ucSubNum],
                    WUEPS_PID_MMA,
                    aStaTimerLen[STA_TIMER_DETACH_CS],
                    TAF_MMA,
                    MMA_MAKELPARAM(ucSubNum, STA_TIMER_DETACH_CS),
                    VOS_RELTIMER_NOLOOP
                    );
        if ( VOS_OK != ulRet )
        {
            STA_TRACE (STA_ERROR_PRINT,
                "Sta_Detach():ERROR:timer start failed");

            /* �ָ�ԭ������״̬��־ */
            if ( STA_ERROR
                == Sta_UpdateFsmFlag(STA_FSMSUB_DETACH_CS,
                        0, STA_UPDATE_SINGL_CLEAR) )
            {
                STA_TRACE (STA_WARNING_PRINT,
                    "Sta_Detach():WARNING:timer clear failed");
            }
            return STA_ERROR;
        }
        /* ��¼��ǰ�Ĳ������̱�ʶ�� */
        g_StatusContext.aFsmSub[ucSubNum].ulOpId = ulOpID;
        g_StatusContext.ucDetachReq++;
        /* BEGIN: Added by liuyang, id:48197, 2005/10/12 */
        g_StatusContext.aucStaPhoneOperator[ucSubNum] = TAF_PH_EVT_CS_DETACH_CNF;
        /* END:   Added by liuyang, id:48197, 2005/10/12 */
    }
    else
    {/* CS&PS DETACH */
        ulTimerIndex = 0;
        /* PS��DETACH */
        g_StatusContext.aucFsmSubFlag[0]
            = STA_FSMSUB_DETACH_CSPS;

        /*����PS��DETACH��־*/
        g_StatusContext.aFsmSub[0].ucFsmStateSub
            = STA_FSMSUB_DETACH_PS;
        g_StatusContext.aFsmSub[0].TimerName
            = STA_TIMER_DETACH_PS;

        /*����PS��ATTACH��ʱ��*/
        ulRet = NAS_StartRelTimer (
                    &g_StatusContext.ahStaTimer[0],
                    WUEPS_PID_MMA,
                    aStaTimerLen[STA_TIMER_DETACH_PS],
                    TAF_MMA,
                    MMA_MAKELPARAM(ulTimerIndex, STA_TIMER_DETACH_PS),
                    VOS_RELTIMER_NOLOOP
                    );
        if ( VOS_OK != ulRet )
        {
            STA_TRACE (STA_ERROR_PRINT,
                "Sta_Detach():ERROR:timer start failed");

            /* �ָ�ԭ������״̬��־ */
            if ( STA_ERROR
                == Sta_UpdateFsmFlag(STA_FSMSUB_DETACH_PS,
                        0, STA_UPDATE_SINGL_CLEAR) )
            {
                STA_TRACE (STA_WARNING_PRINT,
                    "Sta_Detach():WARNING:timer clear failed");
            }
            return STA_ERROR;
        }
        /* ��¼��ǰ�Ĳ������̱�ʶ�� */
        g_StatusContext.aFsmSub[0].ulOpId = ulOpID;
        g_StatusContext.ucDetachReq++;
        /* BEGIN: Added by liuyang, id:48197, 2005/10/12 */
        g_StatusContext.aucStaPhoneOperator[0] = TAF_PH_EVT_DETACH_CNF;
        /* END:   Added by liuyang, id:48197, 2005/10/12 */

        /* CS��DETACH */
    /*modified by liuyang id:48197 date:2005-9-29 for V100R001*/
    /*��ʱ��״̬Ӧ����PS,CS��DETACH����*/
       /* g_StatusContext.aucFsmSubFlag[1]
            = STA_FSMSUB_DETACH_CS;*/
        g_StatusContext.aucFsmSubFlag[1]
            = STA_FSMSUB_DETACH_CSPS;
        ulTimerIndex = 1;
        /* ����CS��DETACH��־;*/
        g_StatusContext.aFsmSub[1].ucFsmStateSub
            = STA_FSMSUB_DETACH_CS;
        g_StatusContext.aFsmSub[1].TimerName
            = STA_TIMER_DETACH_CS;
    /*modified by liuyang id:48197 date:2005-9-29 for V100R001*/
        /*����CS��ATTACH��ʱ��;*/
        ulRet = NAS_StartRelTimer (
            &g_StatusContext.ahStaTimer[1],
            WUEPS_PID_MMA,
            aStaTimerLen[STA_TIMER_DETACH_CS],
            TAF_MMA,
            MMA_MAKELPARAM(ulTimerIndex, STA_TIMER_DETACH_CS),
            VOS_RELTIMER_NOLOOP
            );
        if ( VOS_OK != ulRet )
        {
            STA_TRACE (STA_ERROR_PRINT,
                "Sta_Detach():ERROR:timer start failed");

            /* �ָ�ԭ������״̬��־ */
            /*modified by liuyang id:48197 date:2005-9-29 for V100R001*/
            if ( STA_ERROR
                == Sta_UpdateFsmFlag(STA_FSMSUB_DETACH_CS,0, STA_UPDATE_SINGL_CLEAR) )
            /*modified by liuyang id:48197 date:2005-9-29 for V100R001*/
            {
                STA_TRACE (STA_WARNING_PRINT,
                    "Sta_Detach():WARNING:timer clear failed");
            }
            return STA_ERROR;
        }
        /* ��¼��ǰ�Ĳ������̱�ʶ�� */
        g_StatusContext.aFsmSub[1].ulOpId = ulOpID;
        g_StatusContext.ucDetachReq++;
        /* BEGIN: Added by liuyang, id:48197, 2005/10/12 */
        g_StatusContext.aucStaPhoneOperator[1] = TAF_PH_EVT_DETACH_CNF;
        /* END:   Added by liuyang, id:48197, 2005/10/12 */
    }

    /* �û��·�AT^CGCATT=1,3,at����ERROR,��ʱ����ᱻ���棬��detachʱ����ո���Ϣ */
    g_StatusContext.ucOperateType = STA_OP_NULL;

    /*
    ��յ�ǰ��PLMNid
    modified by liuyang id:48197 date:2005-9-5 for V200R001
    */
    /* Sta_ClearCurPlmnId( &(g_StatusContext.CurPlmnId) ); */
    return STA_SUCCESS;    /* �������� */
}


VOS_VOID MN_PH_UpdateBeginRegTime( VOS_VOID  )
{
    VOS_UINT8                           ucSdcSimStatus;
    VOS_UINT8                           ucIsNeedPin;

    ucSdcSimStatus          = TAF_SDC_GetSimStatus();

    /* ��״̬Ϊ��ҪPIN��PUKʱ����Ϊ��ҪPIN�� */
    if ( (TAF_SDC_USIM_STATUS_SIM_PIN   == ucSdcSimStatus)
      || (TAF_SDC_USIM_STATUS_SIM_PUK   == ucSdcSimStatus) )
    {
        ucIsNeedPin = VOS_TRUE;
    }
    else
    {
        ucIsNeedPin = VOS_FALSE;
    }
    
    if ( ((TAF_MMA_FSM_PHONE_MODE        == TAF_MMA_GetCurrFsmId())
       || (MN_PH_REG_TIME_STATE_PINVALID == g_stRegTimeInfo.enRegTimeState))
      && (VOS_FALSE == ucIsNeedPin) )    
    {
        g_stRegTimeInfo.ulBeginTick = OM_GetSlice();
        MN_INFO_LOG1("MN_PH_UpdateBeginRegTime:first time",(VOS_INT16)g_stRegTimeInfo.ulBeginTick);
    }
    
    if (VOS_FALSE == ucIsNeedPin)
    {
        MN_INFO_LOG1("MN_PH_UpdateBeginRegTime:second time,no need pin",(VOS_INT16)g_stRegTimeInfo.ulBeginTick);
        g_stRegTimeInfo.enRegTimeState = MN_PH_REG_TIME_STATE_BEGIN;
        g_stRegTimeInfo.ulCostTime     = 0;
    }
    else
    {
        MN_INFO_LOG("MN_PH_UpdateBeginRegTime:need pin");
        g_stRegTimeInfo.enRegTimeState = MN_PH_REG_TIME_STATE_PINVALID;
    }
}
VOS_VOID  MN_PH_UpdateEndRegTime( VOS_VOID )
{
    if ( MN_PH_REG_TIME_STATE_BEGIN != g_stRegTimeInfo.enRegTimeState )
    {
        return;
    }

    g_stRegTimeInfo.ulEndTick = OM_GetSlice();
    MN_INFO_LOG2("MN_PH_UpdateEndRegTime beginTick = %0x,endtick = %0x\r\n",
                 (VOS_INT16)g_stRegTimeInfo.ulBeginTick,(VOS_INT16)g_stRegTimeInfo.ulEndTick);

    /* ʱ�������0��ʼ���������Ե�ǰ��ʱ���Ӧ�ô����ϴΡ�����ʱ���
       ��0xFFFFFFFFʱ�����õ�0������else��֧����������� */
    /* MMA����ATʱ����slice�ϱ���(32 * 1024)��slice��1S
       ���sliceΪ0����ʾû��ע��ɹ������sliceС��1S,AT��1S�ϱ� */
    if (g_stRegTimeInfo.ulEndTick >= g_stRegTimeInfo.ulBeginTick)
    {
        g_stRegTimeInfo.ulCostTime
            = (g_stRegTimeInfo.ulEndTick - g_stRegTimeInfo.ulBeginTick);
    }
    else
    {
        g_stRegTimeInfo.ulCostTime
            = (0xFFFFFFFF - g_stRegTimeInfo.ulBeginTick + g_stRegTimeInfo.ulEndTick);
    }

    MN_INFO_LOG1("Into MN_PH_UpdateEndRegTime ulCostTime = %d\r\n",(VOS_INT16)g_stRegTimeInfo.ulCostTime);
    g_stRegTimeInfo.enRegTimeState  = MN_PH_REG_TIME_STATE_END;
    g_stRegTimeInfo.ulEndTick       = 0;

}





VOS_UINT32 Sta_PlmnList ( VOS_UINT32 ulOpID )
{
    VOS_UINT32          ulRet;
    VOS_UINT16          ClientId;
    VOS_UINT8           AppOpId;
    VOS_UINT8           ucPhoneEvent;

    /* ��ڲ����Ϸ��Լ�� */
    if ( Sta_IsInvalidOpId (ulOpID) )
    {
        STA_TRACE(STA_WARNING_PRINT, "Sta_PlmnList():WARNING:parameter invalid  ");
        return STA_ERROR;
    }

    /* PLMN_LIST״̬������������쳣*/
    /* ֻ����ENABLE״̬������REFRESH�������Զ������������·�list���󣬷��򷵻ش���*/
    if (VOS_TRUE != TAF_MMA_IsEnablePlmnList())
    {
        g_StatusContext.PhoneError    = TAF_ERR_PHONE_MSG_UNMATCH;
        g_StatusContext.ucOperateType = STA_OP_PHONEERROR;

        if (ulOpID <= TAF_MAX_STATUS_TI)
        {
            /*ͨ��Ti��ȡClientId,CallId*/
            if (TAF_SUCCESS != MMA_GetIdByTi(TAF_MMA, (VOS_UINT8)ulOpID, &ClientId, &AppOpId, &ucPhoneEvent))
            {
                MMA_WARNINGLOG("Taf_PhoneEvent():WARNING:Invoke Taf_GetIdByTi failed");
                return STA_ERROR;
            }
        }
        else
        {   /*��STATUS����������¼�*/
            AppOpId = MMA_OP_ID_INTERNAL;
            /*ClientId��Ϊ�㲥ֵ:���ֽڵĵ�4bit������MUX_PORT_BUTT*/
            ClientId = MMA_CLIENTID_BROADCAST;
        }

        /* �ϱ�APP��ǰ״̬�¼� */
        MMA_HandleEventError(ClientId, AppOpId, TAF_ERR_PHONE_MSG_UNMATCH, TAF_PH_EVT_ERR);

        /* ������ٵ�����Ϣ */
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_PlmnList():WARNING:system FSM state is wrong\n");

        return STA_ERROR;       /* ���󷵻� */
    }


    ulRet = TAF_MMA_SndMmcPlmnListReq();

    if ( VOS_ERR == ulRet )
    {
        STA_TRACE (STA_ERROR_PRINT,
            "Sta_PlmnList():ERROR: failed");
        return STA_ERROR;       /* ���󷵻� */
    }
    /* ��¼��ǰ�Ĳ������̱�ʶ�� */
    g_StatusContext.ulCurOpId = ulOpID;

    /* ����PLMN_LIST״̬��־�� */
    g_StatusContext.ulFsmState = STA_FSM_PLMN_LIST;

    /* ����PLMN_LIST״̬��ʱ�� */
    g_StatusContext.aucFsmSubFlag[0]         = STA_FSMSUB_MONO;
    g_StatusContext.aFsmSub[0].ucFsmStateSub = STA_FSMSUB_MONO;
    g_StatusContext.aFsmSub[0].TimerName     = STA_TIMER_PLMN_LIST;
    ulRet = NAS_StartRelTimer (
            &g_StatusContext.ahStaTimer[0],
            WUEPS_PID_MMA,
            aStaTimerLen[STA_TIMER_PLMN_LIST],
            TAF_MMA,
            MMA_MAKELPARAM(0, STA_TIMER_PLMN_LIST),
            VOS_RELTIMER_NOLOOP
            );
    if ( VOS_OK != ulRet )
    {
        /* �ָ�ԭ������״̬��־ */
        if ( STA_ERROR
            == Sta_UpdateFsmFlag(STA_FSMSUB_NULL,
                STA_FSM_NULL, STA_UPDATE_MONO_CLEAR) )
        {
            STA_TRACE (STA_ERROR_PRINT,
                "Sta_PlmnList():ERROR:timer clear failed\n");
        }
        return STA_ERROR;
    }

    /* ������״̬�Ĳ������̱�ʶ�� */
    g_StatusContext.aFsmSub[0].ulOpId = ulOpID;

    return STA_SUCCESS;         /* �������� */
}


VOS_UINT32 Sta_PlmnReselectAuto ( VOS_UINT32 ulOpID )
{
    VOS_UINT32                            ulRet;
    TAF_MMA_AUTO_PLMN_SEL_USER_ENUM_UINT8 enAutoPlmnSelUser;

    enAutoPlmnSelUser = TAF_MMA_GetAutoPlmnSelUser();

    /* ��ڲ����Ϸ��Լ�� */
    if ( Sta_IsInvalidOpId (ulOpID) )
    {
        STA_TRACE(STA_WARNING_PRINT,
            "Sta_PlmnReselectAuto():WARNING:parameter invalid");
        return STA_ERROR;
    }

    /* svlte nv��������ػ�״̬����cops */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
        return TAF_MMA_PlmnReselectAutoReq_PowerOff(ulOpID);
    }

    /* PLMN_RESEL״̬������������쳣 */
    /* ��ǰ���ڽ���6F15�ļ��������Զ����� */
    if ((STA_FSM_PLMN_RESEL == g_StatusContext.ulFsmState)
     && (TAF_MMA_AUTO_PLMN_SEL_USER_USIM_REFRESH == enAutoPlmnSelUser))
    {
        /* ��¼ΪAT������Զ��������յ�CNF֮����Ҫ���û��ظ���� */
        TAF_MMA_SetAutoPlmnSelUser(TAF_MMA_AUTO_PLMN_SEL_USER_AT);

        /* ��¼��ǰ�Ĳ������̱�ʶ�� */
        g_StatusContext.ulCurOpId = ulOpID;

        return STA_SUCCESS;
    }

    if ( !(g_StatusContext.ulFsmState & STA_FSM_ENABLE) )
    {/* �������ENABLE״̬�����ش��� */
        g_StatusContext.PhoneError    = TAF_ERR_PHONE_MSG_UNMATCH;
        g_StatusContext.ucOperateType = STA_OP_PHONEERROR;
        /* �ϱ�APP��ǰ״̬�¼� */
        Sta_EventReport (ulOpID, TAF_PH_EVT_PLMN_RESEL_CNF);

        /* ������ٵ�����Ϣ */
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_PlmnReselectAuto():WARNING:system FSM state  is wrong ");

        return STA_ERROR;           /* ���󷵻� */
    }

    ulRet = TAF_MMA_SndMmcPlmnUserReselReq(MMA_MMC_PLMN_SEL_MODE_AUTO);

    if ( VOS_ERR == ulRet )
    {
        STA_TRACE (STA_ERROR_PRINT,
            "Sta_PlmnReselectAuto():ERROR: failed");
        return STA_ERROR;           /* ���󷵻� */
    }

    /* ��¼AT������Զ��������� */
    TAF_MMA_SetAutoPlmnSelUser(TAF_MMA_AUTO_PLMN_SEL_USER_AT);

    /* ��¼��ǰ�Ĳ������̱�ʶ�� */
    g_StatusContext.ulCurOpId = ulOpID;

    /* ��¼PLMN�������� */
    g_StatusContext.ucReselMode = MMA_MMC_PLMN_SEL_MODE_AUTO;


    /* ����PLMN_RESEL״̬��־�� */
    g_StatusContext.ulFsmState = STA_FSM_PLMN_RESEL;

    STA_TRACE1(STA_NORMAL_PRINT, "@@@@Sta_PlmnReselectAuto:FsmState=",STA_FSM_PLMN_RESEL);
    /* ����PLMN_RESEL״̬��ʱ�� */
    g_StatusContext.aucFsmSubFlag[0]         = STA_FSMSUB_PLMN_RESEL_AUTO;
    g_StatusContext.aFsmSub[0].ucFsmStateSub = STA_FSMSUB_PLMN_RESEL_AUTO;
    g_StatusContext.aFsmSub[0].TimerName     = STA_TIMER_PLMN_RESEL;
    ulRet = NAS_StartRelTimer (
            &g_StatusContext.ahStaTimer[0],
            WUEPS_PID_MMA,
            aStaTimerLen[STA_TIMER_PLMN_RESEL],
            TAF_MMA,
            MMA_MAKELPARAM(0, STA_TIMER_PLMN_RESEL),
            VOS_RELTIMER_NOLOOP
            );
    if ( VOS_OK != ulRet )
    {
        /* �ָ�ԭ������״̬��־ */
        if ( STA_ERROR
            == Sta_UpdateFsmFlag(STA_FSMSUB_NULL,
                STA_FSM_NULL, STA_UPDATE_MONO_CLEAR) )
        {
            STA_TRACE (STA_ERROR_PRINT,
                "Sta_PlmnReselectAuto():ERROR:timer clear failed");
        }
        return STA_ERROR;
    }
    /* ������״̬�Ĳ������̱�ʶ�� */
    g_StatusContext.aFsmSub[0].ulOpId = ulOpID;

    return STA_SUCCESS;         /* �������� */
}
VOS_UINT32 Sta_PlmnReselectManual ( VOS_UINT32 ulOpID )
{
    VOS_UINT32                      ulRet;
    /* void                        *pRet; */

    /* ��ڲ����Ϸ��Լ�� */
    if ( Sta_IsInvalidOpId (ulOpID) )
    {
        STA_TRACE(STA_WARNING_PRINT,
            "Sta_PlmnReselectManual():WARNING:parameter invalid");
        return STA_ERROR;
    }

    /* PLMN_RESEL״̬������������쳣*/
    if ( !(g_StatusContext.ulFsmState & STA_FSM_ENABLE) )
    {/* �������ENABLE״̬�����ش��� */
        g_StatusContext.PhoneError    = TAF_ERR_PHONE_MSG_UNMATCH;
        g_StatusContext.ucOperateType = STA_OP_PHONEERROR;
        /* �ϱ�APP��ǰ״̬�¼� */
        Sta_EventReport (ulOpID, TAF_PH_EVT_PLMN_RESEL_CNF);

        /* ������ٵ�����Ϣ */
       STA_TRACE (STA_WARNING_PRINT,
            "Sta_PlmnReselectManual():WARNING:system FSM state is wrong\n ");

        return STA_ERROR;    /* ���󷵻�*/
    }

    ulRet = TAF_MMA_SndMmcPlmnUserReselReq(MMA_MMC_PLMN_SEL_MODE_MANUAL);

    if ( VOS_ERR == ulRet )
    {
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_PlmnReselectManual():WARNING: failed");
        return STA_ERROR;       /* ���󷵻� */
    }
    /* ��¼��ǰ�Ĳ������̱�ʶ�� */
    g_StatusContext.ulCurOpId = ulOpID;

    /* ��¼PLMN�������� */
    g_StatusContext.ucReselMode = MMA_MMC_PLMN_SEL_MODE_MANUAL;

    /* �ϱ�APP��ǰ״̬�¼����ֶ�PLMN���� */
    /*Sta_EventReport (ulOpID, TAF_PH_EVT_PLMN_RESEL);*/

    /* ����PLMN_RESEL״̬��־�� */
    g_StatusContext.ulFsmState = STA_FSM_PLMN_RESEL;

    STA_TRACE1(STA_NORMAL_PRINT, "@@@@Sta_PlmnReselectManual:FsmState=",STA_FSM_PLMN_RESEL);
    /* ����PLMN_RESEL״̬��ʱ�� */
    g_StatusContext.aucFsmSubFlag[0]
        = STA_FSMSUB_PLMN_RESEL_MANUAL;
    g_StatusContext.aFsmSub[0].ucFsmStateSub
        = STA_FSMSUB_PLMN_RESEL_MANUAL;
    g_StatusContext.aFsmSub[0].TimerName
        = STA_TIMER_PLMN_RESEL;
    ulRet = NAS_StartRelTimer (
            &g_StatusContext.ahStaTimer[0],
            WUEPS_PID_MMA,
            aStaTimerLen[STA_TIMER_PLMN_RESEL],
            TAF_MMA,
            MMA_MAKELPARAM(0, STA_TIMER_PLMN_RESEL),
            VOS_RELTIMER_NOLOOP
            );
    if ( VOS_OK != ulRet )
    {
        /* �ָ�ԭ������״̬��־ */
        if ( STA_ERROR
            == Sta_UpdateFsmFlag(STA_FSMSUB_NULL,
                STA_FSM_NULL, STA_UPDATE_MONO_CLEAR) )
        {
            STA_TRACE (STA_WARNING_PRINT,
                "Sta_PlmnReselectManual():WARNING:timer clear failed");
        }
        return STA_ERROR;       /* ���󷵻� */
    }
    /* ������״̬�Ĳ������̱�ʶ�� */
    g_StatusContext.aFsmSub[0].ulOpId = ulOpID;
    /*
    ��յ�ǰ��PLMNid
    modified by liuyang id:48197 date:2005-9-5 for V200R001
    */
    /* Sta_ClearCurPlmnId( &(g_StatusContext.CurPlmnId) ); */
    return STA_SUCCESS;         /* �������� */
}


VOS_UINT32 Sta_PlmnSelect ( VOS_UINT32 ulOpID, TAF_PLMN_ID_STRU PlmnId,
                        TAF_PH_RA_MODE AccessMode, VOS_UINT8 ucReselMode)
{
    VOS_UINT32                      ulRet;

    /* ��ڲ����Ϸ��Լ�� */
    if ( Sta_IsInvalidOpId (ulOpID) )
    {
        STA_TRACE(STA_WARNING_PRINT,
            "Sta_PlmnSelect():WARNING:parameter invalid");
        return STA_ERROR;
    }
    
    /* svlte nv��������ػ�״̬����cops */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
        return TAF_MMA_RcvPlmnSelectionReq_PowerOff(ulOpID, PlmnId, AccessMode, ucReselMode);
    }


    /* PLMN_SEL״̬������������쳣*/
    if (VOS_TRUE != TAF_MMA_IsEnablePlmnSelect())
    {
        /* �������ENABLE״̬�����ش��� */
        if (TAF_MMA_PLMN_MODE_ENUM_DISABLE == TAF_MMA_GetPlmnMode_Refresh())
        {
            g_StatusContext.PhoneError = TAF_ERR_NET_SEL_MENU_DISABLE;
        }
        else
        {
            g_StatusContext.PhoneError = TAF_ERR_PHONE_MSG_UNMATCH;
        }

        g_StatusContext.ucOperateType = STA_OP_PHONEERROR;
        /* �ϱ�APP��ǰ״̬�¼� */
        Sta_EventReport (ulOpID, TAF_PH_EVT_PLMN_SEL_CNF);

        /* ������ٵ�����Ϣ */
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_PlmnSelect():WARNING:system FSM state is wrong");

        return STA_ERROR;    /* ���󷵻� */
    }

    /* USIM����ֹGSM���벢����G��ģʱ���������û�ָ����
       SIM��������L��ģʱ��Ҳ�������û�ָ���� */
    if (VOS_FALSE == TAF_MMA_IsExistPlmnRatAllowAccess())
    {
        g_StatusContext.PhoneError    = TAF_ERR_NO_NETWORK_SERVICE;
        g_StatusContext.ucOperateType = STA_OP_PHONEERROR;

        /* �ϱ�APP��ǰ״̬�¼� */
        Sta_EventReport (ulOpID, TAF_PH_EVT_PLMN_SEL_CNF);

        /* ������ٵ�����Ϣ */
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_PlmnSelect():WARNING:system FSM state is wrong");

        return STA_ERROR;
    }

    /* ��¼������� */
    g_StatusContext.ulCurOpId     = ulOpID;
    g_StatusContext.StaPlmnId.Mcc = PlmnId.Mcc;
    g_StatusContext.StaPlmnId.Mnc = PlmnId.Mnc;

    ulRet = TAF_MMA_SndMmcPlmnSpecialReq((MMA_MMC_PLMN_ID_STRU *)&PlmnId, AccessMode);

    if ( VOS_ERR == ulRet )
    {
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_PlmnSelect():WARNING: failed");
        return STA_ERROR;           /* ���󷵻� */
    }
    /* ��¼��ǰ�Ĳ������̱�ʶ�� */
    g_StatusContext.ulCurOpId = ulOpID;

    /* �ϱ�APP��ǰ״̬�¼� */
    /*Sta_EventReport (ulOpID, TAF_PH_EVT_PLMN_SEL);*/

    /* ����PLMN_SEL״̬��־�� */
    g_StatusContext.ulFsmState = STA_FSM_PLMN_SEL;

    STA_TRACE1(STA_NORMAL_PRINT, "@@@@Sta_PlmnSelect:FsmState=",STA_FSM_PLMN_SEL);
    /* ����PLMN_SEL״̬��ʱ�� */
    g_StatusContext.aucFsmSubFlag[0]         = STA_FSMSUB_MONO;
    g_StatusContext.aFsmSub[0].ucFsmStateSub = STA_FSMSUB_MONO;
    g_StatusContext.aFsmSub[0].TimerName     = STA_TIMER_PLMN_SEL;

    ulRet = NAS_StartRelTimer (
            &g_StatusContext.ahStaTimer[0],
            WUEPS_PID_MMA,
            aStaTimerLen[STA_TIMER_PLMN_SEL],
            TAF_MMA,
            MMA_MAKELPARAM(0, STA_TIMER_PLMN_SEL),
            VOS_RELTIMER_NOLOOP
            );

    if ( VOS_OK != ulRet )
    {
        /* �ָ�ԭ������״̬��־ */
        if (STA_ERROR
            == Sta_UpdateFsmFlag(STA_FSMSUB_NULL,
                STA_FSM_NULL, STA_UPDATE_MONO_CLEAR) )
        {
            STA_TRACE (STA_WARNING_PRINT,
                "Sta_PlmnSelect():WARNING:timer clear failed\n");
        }
        return STA_ERROR;       /* ���󷵻� */
    }
    /* ��¼��ǰ�Ĳ������̱�ʶ�� */
    g_StatusContext.aFsmSub[0].ulOpId = ulOpID;
    /*
    ��յ�ǰ��PLMNid
    modified by liuyang id:48197 date:2005-9-5 for V200R001
    */
    /* Sta_ClearCurPlmnId( &(g_StatusContext.CurPlmnId) ); */
    return STA_SUCCESS;         /* �������� */
}


VOS_VOID  Sta_AttachTimeoutProc(REL_TIMER_MSG * pMsg)
{
    STA_TIMER_NAME_E    TimerId;    /* ��ʱ��״̬����*/
    VOS_UINT16              TimerIndex; /* ��ʱ����� */
    /* HTIMER              hTimer; */    /* ��ʱ������*/
    VOS_UINT32          ulRet;
    VOS_UINT8           ucOperate;


    /*��ȡϵͳ��ʱ����Ϣ,�õ���ʱ��ID�Ͷ�ʱ������*/
    TimerId     = (STA_TIMER_NAME_E)MMA_LOWORD(pMsg->ulPara);
    TimerIndex  = MMA_HIWORD(pMsg->ulPara);
    /* hTimer      = pMsg->hTm; */
    g_StatusContext.PhoneError    = TAF_ERR_TIME_OUT;
    g_StatusContext.ucOperateType = STA_OP_PHONEERROR;

    switch ( TimerId )
    {
        case STA_TIMER_ATTACH_CS:
            ucOperate = TAF_PH_EVT_CS_ATTACH_CNF;
            g_StatusContext.ucOperateType |= STA_OP_CSSERVICESTATUS;
            break;
        case STA_TIMER_ATTACH_PS:
            ucOperate = TAF_PH_EVT_PS_ATTACH_CNF;
            g_StatusContext.ucOperateType |= STA_OP_PSSERVICESTATUS;
            break;
        default:
            MMA_WARNINGLOG("Sta_AttachTimeoutProc():WARNING:UNKNOW TIMERID!");
            return;
    }
    /*����Ƿ�������attach���̣��ж��Ƿ���ͬʱ�������attach���̣������ǣ�
      ��������*/
    if (  STA_FSMSUB_ATTACH_CSPS == g_StatusContext.aucFsmSubFlag[TimerIndex])
    {
        ucOperate = TAF_PH_EVT_ATTACH_CNF;
        g_StatusContext.ucOperateType |= (STA_OP_CSSERVICESTATUS
                                         |STA_OP_PSSERVICESTATUS);
    }

   g_StatusContext.ucAttachReq--;

   /* �ϱ�APP��ǰ״̬�¼� */

   /*�ж����ATTACH�Ƿ�����������*/
   if (( STA_OPID_ATTATCH != g_StatusContext.aFsmSub[TimerIndex].ulOpId )
       && (0 == g_StatusContext.ucAttachReq))
   {
        if ( MMA_SET_MSCLASSTYPE_NO ==
                gstMmaValue.stSetMsClass.enSetMsClassTypeflag)
        {
              Sta_EventReport (g_StatusContext.ulCurOpId, ucOperate);
        }
        else if(MMA_SET_MSCLASSTYPE_YES ==
                    gstMmaValue.stSetMsClass.enSetMsClassTypeflag)
        {
          /*�������ò������̣��ж��Ƿ�������е�ATTACH�������ǣ�ͬ��ȫ��
            �������������ò����ɹ�*/
            Sta_SyncMsClassType(g_StatusContext.aFsmSub[TimerIndex].ulOpId ,
                                gstMmaValue.stSetMsClass.NewMsClassType);
        }
        else
        {
            STA_TRACE (STA_WARNING_PRINT,
                       "Sta_AttachTimeoutProc():WARNING:SET MSClass Type Flag Error failed");
        }
   }
   else
   {
       STA_TRACE(STA_NORMAL_PRINT, "Sta_AttachTimeoutProc():NORMAL:Internal Attach TimeOut");
   }
   /*�����ǰ״̬��־���ص��¼�����״̬*/

   /*  �����״̬������Ϣ */
   /* BEGIN: Modified by liuyang, 2005/10/22 */
   /*��ʱӦ��OpId��Ϊ��Чֵ*/
   /* g_StatusContext.aFsmSub[TimerIndex].ulOpId = 0; */
   g_StatusContext.aFsmSub[TimerIndex].ulOpId = STA_OPID_INVALID;
   /* END:   Modified by liuyang, 2005/10/22 */
   g_StatusContext.aFsmSub[TimerIndex].ucFsmStateSub
                          = STA_FSMSUB_NULL;
   g_StatusContext.aFsmSub[TimerIndex].TimerName
                          = STA_TIMER_NULL;
   g_StatusContext.aucFsmSubFlag[TimerIndex]
                          = STA_FSMSUB_NULL;
   /* ����������������DE_ATTACH���ص�ENABLE̬*/
   if (g_StatusContext.ucAttachReq <= 0)
   {
        ulRet = Sta_UpdateFsmFlag ( STA_FSMSUB_NULL,
                                    STA_FSM_ENABLE,
                                    STA_UPDATE_DUAL_CLEAR );
        if (STA_ERROR == ulRet)
        {
            STA_TRACE(STA_WARNING_PRINT, "Sta_AttachTimeoutProc():WARNING:Sta_UpdateFsmFlag failed\n");
        }
        g_StatusContext.ucAttachReq = 0;
        Sta_EventReport (g_StatusContext.ulCurOpId, ucOperate);
   }
   else
   {
       g_StatusContext.ulFsmState = STA_FSM_DE_ATTACH;
   }
   return;
}
VOS_VOID  Sta_DetachTimeoutProc(REL_TIMER_MSG * pMsg)
{
    STA_TIMER_NAME_E    TimerId;    /* ��ʱ��״̬����*/
    VOS_UINT16              TimerIndex; /* ��ʱ����� */
    VOS_UINT8           ucOperate;
    VOS_UINT32          ulRet;

    /*��ȡϵͳ��ʱ����Ϣ,�õ���ʱ��ID�Ͷ�ʱ������*/
    TimerId     = (STA_TIMER_NAME_E)MMA_LOWORD(pMsg->ulPara);
    TimerIndex  = MMA_HIWORD(pMsg->ulPara);
    /* hTimer      = pMsg->hTm; */


    g_StatusContext.PhoneError    = TAF_ERR_TIME_OUT;
    g_StatusContext.ucOperateType = STA_OP_PHONEERROR;

    switch ( TimerId )
    {
        case STA_TIMER_DETACH_CS:
            ucOperate = TAF_PH_EVT_CS_DETACH_CNF;
            g_StatusContext.ucOperateType |= STA_OP_CSSERVICESTATUS;
            break;
        case STA_TIMER_DETACH_PS:
            ucOperate = TAF_PH_EVT_PS_DETACH_CNF;
            g_StatusContext.ucOperateType |= STA_OP_PSSERVICESTATUS;
            break;
        default:
            MMA_WARNINGLOG("Sta_DetachTimeoutProc():WARNING:UNKNOW TIMERID!");
            return;
    }
    /*�ж��Ƿ�˴�����������ͬʱDETACH*/
    if (  STA_FSMSUB_DETACH_CSPS == g_StatusContext.aucFsmSubFlag[TimerIndex])
    {
        ucOperate = TAF_PH_EVT_DETACH_CNF;
        g_StatusContext.ucOperateType |= (STA_OP_CSSERVICESTATUS
                                         |STA_OP_PSSERVICESTATUS);
    }

    g_StatusContext.ucDetachReq--;


    /* �ϱ�APP��ǰ״̬�¼� */
    /* Ԥ���ĺ����ӿڣ��ݲ�ʹ�� */
    /*ulRet = Sta_EventReport (g_StatusContext.ulCurOpId,
        TAF_PH_EVT_DETACH_CNF);
    if ( STA_ERROR == ulRet )
    {
        return STA_ERROR;
    }*/
    /*�жϴ˴�DETACH�������Դ*/
    if (STA_OPID_INSIDE == g_StatusContext.aFsmSub[TimerIndex].ulOpId)
    {
       /*�����ǰ״̬��־���ص��¼�����״̬*/
       /*  �����״̬������Ϣ */
       /* BEGIN: Modified by liuyang, 2005/10/22 */
       /*��ʱӦ��OpId��Ϊ��Чֵ*/
       /* g_StatusContext.aFsmSub[TimerIndex].ulOpId = 0; */
       g_StatusContext.aFsmSub[TimerIndex].ulOpId = STA_OPID_INVALID;
       /* END:   Modified by liuyang, 2005/10/22 */
       g_StatusContext.aFsmSub[TimerIndex].ucFsmStateSub
           = STA_FSMSUB_NULL;
       g_StatusContext.aFsmSub[TimerIndex].TimerName
           = STA_TIMER_NULL;
       g_StatusContext.aucFsmSubFlag[TimerIndex]
           = STA_FSMSUB_NULL;
       /* ����������������DE_ATTACH���ص�ENABLE̬*/
       if (g_StatusContext.ucDetachReq <= 0)
       {
           ulRet = Sta_UpdateFsmFlag(STA_FSMSUB_NULL,
                                     STA_FSM_ENABLE,
                                     STA_UPDATE_DUAL_CLEAR);
           if (STA_ERROR == ulRet)
           {
               STA_TRACE(STA_WARNING_PRINT, "Sta_DetachTimeoutProc():WARNING:Sta_UpdateFsmFlag failed\n");
           }
           g_StatusContext.ucDetachReq = 0;
       }
       else
       {
           g_StatusContext.ulFsmState = STA_FSM_DE_ATTACH;
       }
       return;
    }
    /*��APP��AT�����ֱ��DETACH����*/
    if( MMA_SET_MSCLASSTYPE_NO ==
         gstMmaValue.stSetMsClass.enSetMsClassTypeflag )
    {
        Sta_DetachTimeoutProc4App(TimerId,TimerIndex, ucOperate);
    }
    else if(MMA_SET_MSCLASSTYPE_YES ==
               gstMmaValue.stSetMsClass.enSetMsClassTypeflag )
    {
        Sta_DetachTimeoutProc4DefMsClassType(TimerId, TimerIndex);
    }
    else
    {
        ;
    }
    return;
}
VOS_VOID Sta_DetachTimeoutProc4App(STA_TIMER_NAME_E    TimerId,
                                   VOS_UINT16          TimerIndex,
                                   VOS_UINT8           ucOperate)
{
    VOS_UINT32      ulRet;

    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

     /*DETACH���*/
    if( 0 ==  g_StatusContext.ucDetachReq )
    {
        if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_DETACH_REQ, &ucCtxIndex))
        {
            pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

            TAF_MMA_SndDetachCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                                 TAF_MMA_APP_OPER_RESULT_FAILURE,
                                 TAF_ERR_ERROR);

            TAF_MMA_ClearOperCtx(ucCtxIndex);
        }

        ulRet = Sta_UpdateFsmFlag(STA_FSMSUB_NULL,
                                 STA_FSM_ENABLE,
                                 STA_UPDATE_DUAL_CLEAR);
       if (STA_ERROR == ulRet)
       {
           STA_TRACE(STA_WARNING_PRINT, "Sta_DetachTimeoutProc4App():WARNING:Sta_UpdateFsmFlag failed\n");
       }
       g_StatusContext.ucDetachReq = 0;
       return;
    }
    /*�����ǰ״̬��־���ص��¼�����״̬*/
    /*  �����״̬������Ϣ */
    /* BEGIN: Modified by liuyang, 2005/10/22 */
    /*��ʱӦ��OpId��Ϊ��Чֵ*/
    /* g_StatusContext.aFsmSub[TimerIndex].ulOpId = 0; */
    g_StatusContext.aFsmSub[TimerIndex].ulOpId          = STA_OPID_INVALID;
    /* END:   Modified by liuyang, 2005/10/22 */
    g_StatusContext.aFsmSub[TimerIndex].ucFsmStateSub   = STA_FSMSUB_NULL;
    g_StatusContext.aFsmSub[TimerIndex].TimerName       = STA_TIMER_NULL;
    g_StatusContext.aucFsmSubFlag[TimerIndex]           = STA_FSMSUB_NULL;

    g_StatusContext.ulFsmState = STA_FSM_DE_ATTACH;
    return;
}
VOS_VOID  Sta_DetachTimeoutProc4DefMsClassType( STA_TIMER_NAME_E    TimerId,
                                                VOS_UINT16          TimerIndex)
{
    VOS_UINT32      ulRet;

    if (TimerIndex > STA_TIMER_MAX -1)
    {
        return ;
    }

    /*����ǰ����������DETACH���̣���Ҫ�ر���һ��DETACH��ʱ����
    ͬʱ��λ״̬*/
    if ( 0 < g_StatusContext.ucDetachReq )
    {
         /*�ͷ���һ����ʱ������Դ*/

        ulRet = NAS_StopRelTimer(WUEPS_PID_MMA, g_StatusContext.aFsmSub[1 - TimerIndex].TimerName,
                                 &g_StatusContext.ahStaTimer[1 - TimerIndex]);

        if ( VOS_OK != ulRet )
        {
            STA_TRACE (STA_WARNING_PRINT,
                    "Sta_DetachTimeoutProc4DefMsClassType(): WARNING:\
                    NAS_StopRelTimer failed");
        }
        g_StatusContext.ahStaTimer[1 - TimerIndex] = 0;
    }

    /*����MODE_CHANGE_REQ��MMC��������״̬�ع�*/
    /* gstMmaValue.stSetMsClass.enSetMsClassTypeflag����û���ó�YES�ĵط���
        ����Sta_DetachTimeoutProc4DefMsClassType�����Ѿ��߲�����,����Ҫ֪ͨSTK/MSG CS��������� */
    TAF_MMA_SndMmcModeChangeReq((MMA_MMC_MS_MODE_ENUM_UINT32)gstMmaValue.stSetMsClass.MsClassType);

    /*ͬ���ֻ�״̬ΪΪ����ǰ״̬*/
    Sta_SyncMsClassType(g_StatusContext.ulCurOpId, gstMmaValue.stSetMsClass.MsClassType);

    /*��յ�ǰSTATUSģ����״̬*/
    ulRet = Sta_UpdateFsmFlag(STA_FSMSUB_NULL,
                              STA_FSM_ENABLE,
                              STA_UPDATE_DUAL_CLEAR);
    if (STA_ERROR == ulRet)
    {
        STA_TRACE(STA_WARNING_PRINT, "Sta_DetachTimeoutProc4DefMsClassType():WARNING:Sta_UpdateFsmFlag failed\n");
    }

    /* ���DETACH���� */
    g_StatusContext.ucDetachReq = 0;
    return;
}



VOS_UINT32 Sta_TimeoutProc (REL_TIMER_MSG * pMsg)
{
    STA_TIMER_NAME_E                    TimerId;    /* ��ʱ��״̬����*/
    VOS_UINT16                          TimerIndex; /* ��ʱ����� */
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulOpId;

    ulOpId  = STA_OPID_INVALID;

    /* ������� */
    if ( VOS_NULL_PTR == pMsg )
    {
        STA_TRACE (STA_ERROR_PRINT,
            "Sta_TimeoutProc():ERROR:parameter invaild null PTR");
        return STA_ERROR;
    }

    /*��ȡϵͳ��ʱ����Ϣ,�õ���ʱ��ID�Ͷ�ʱ������*/
    TimerId     = (STA_TIMER_NAME_E)MMA_LOWORD(pMsg->ulPara);
    TimerIndex  = MMA_HIWORD(pMsg->ulPara);
    STA_TRACE1 (STA_WARNING_PRINT,
            "Sta_TimeoutProc():WARNING:Status Time Out, TimerId is ",TimerId);

    if (TimerIndex >= STA_TIMER_MAX)
    {
        STA_TRACE (STA_ERROR_PRINT,
                "Sta_TimeoutProc():TimerIndex over run.");
        return STA_ERROR;
    }

    /* ϵͳ��ʱ����Ϣ�ַ�����*/
    switch ( TimerId )
    {


        case STA_TIMER_ATTACH_CS :
        case STA_TIMER_ATTACH_PS :
            Sta_AttachTimeoutProc(pMsg);
            break;

        case STA_TIMER_DETACH_CS:
        case STA_TIMER_DETACH_PS:
            Sta_DetachTimeoutProc(pMsg);
            break;

        case STA_TIMER_PLMN_LIST :


            /* ��ʱ����AT�ظ�������յ�ID_MMC_MMA_PLMN_LIST_ABORT_CNF ����
               AT�ظ�*/

            /*�����ǰ״̬��־���ص�ENABLE״̬*/
            /*  �����״̬������Ϣ */
            /* BEGIN: Modified by liuyang, 2005/10/22 */
            /*��ʱӦ��OpId��Ϊ��Чֵ*/
            ulOpId                                     = g_StatusContext.aFsmSub[TimerIndex].ulOpId;
            g_StatusContext.aFsmSub[TimerIndex].ulOpId = STA_OPID_INVALID;
            /* END:   Modified by liuyang, 2005/10/22 */

            g_StatusContext.aFsmSub[TimerIndex].ucFsmStateSub
                = STA_FSMSUB_NULL;
            g_StatusContext.aFsmSub[TimerIndex].TimerName
                = STA_TIMER_NULL;
            g_StatusContext.aucFsmSubFlag[TimerIndex]
                = STA_FSMSUB_NULL;

            /* ���ô����� */
            TAF_MMA_RcvTimerPlmnListCnfExpired(ulOpId);

            break;

        case STA_TIMER_PLMN_SEL :
            ulOpId                                            = g_StatusContext.aFsmSub[TimerIndex].ulOpId;
            g_StatusContext.aFsmSub[TimerIndex].ulOpId        = STA_OPID_INVALID;
            g_StatusContext.aFsmSub[TimerIndex].ucFsmStateSub = STA_FSMSUB_NULL;
            g_StatusContext.aFsmSub[TimerIndex].TimerName     = STA_TIMER_NULL;
            g_StatusContext.aucFsmSubFlag[TimerIndex]         = STA_FSMSUB_NULL;

            /* MMA��ָ������������ʱ����ʱ��������MMC����ָֹͣ����������
               �յ�MMC�Ļظ�������AT�ظ� */
            ulRet = TAF_MMA_SndMmcSpecPlmnSearchAbortReq();

            if (VOS_ERR == ulRet)
            {
                STA_TRACE(STA_ERROR_PRINT, "Sta_TimeoutProc():ERROR: failed");
            }

            /* ����STA_TIMER_SPEC_PLMN_ABORT 10s��ʱ���������ʱδ�յ�MMC�Ļظ����ϱ�ATʧ�� */
            g_StatusContext.aucFsmSubFlag[0]         = STA_FSMSUB_MONO;
            g_StatusContext.aFsmSub[0].ucFsmStateSub = STA_FSMSUB_MONO;
            g_StatusContext.aFsmSub[0].TimerName     = STA_TIMER_SPEC_PLMN_ABORT;
            g_StatusContext.aFsmSub[0].ulOpId        = ulOpId;

            if (VOS_OK != NAS_StartRelTimer(&g_StatusContext.ahStaTimer[0],
                                            WUEPS_PID_MMA,
                                            aStaTimerLen[STA_TIMER_SPEC_PLMN_ABORT],
                                            TAF_MMA,
                                            MMA_MAKELPARAM(0, STA_TIMER_SPEC_PLMN_ABORT),
                                            VOS_RELTIMER_NOLOOP))
            {
                STA_TRACE(STA_ERROR_PRINT, "Sta_TimeoutProc():ERROR: VOS_StartRelTimer runs failed");
            }

            break;

        case STA_TIMER_PLMN_RESEL:
            g_StatusContext.PhoneError    = TAF_ERR_TIME_OUT;
            g_StatusContext.ucOperateType = STA_OP_PHONEERROR;

            /* 6F15�ļ�refresh�������Զ�����������Ҫ�ϱ�APP */
            if (TAF_MMA_AUTO_PLMN_SEL_USER_AT == TAF_MMA_GetAutoPlmnSelUser())
            {
                /* �ϱ�APP��ǰ״̬�¼� */
                Sta_EventReport (g_StatusContext.ulCurOpId,
                    TAF_PH_EVT_PLMN_RESEL_CNF);
            }

            /* �����ǰ״̬��־���ص�ENABLE״̬ */
            /*  �����״̬������Ϣ */
            /*��ʱӦ��OpId��Ϊ��Чֵ*/
            g_StatusContext.aFsmSub[TimerIndex].ulOpId = STA_OPID_INVALID;
            g_StatusContext.aFsmSub[TimerIndex].ucFsmStateSub
                = STA_FSMSUB_NULL;
            g_StatusContext.aFsmSub[TimerIndex].TimerName
                = STA_TIMER_NULL;
            g_StatusContext.aucFsmSubFlag[TimerIndex]
                = STA_FSMSUB_NULL;
            g_StatusContext.ulFsmState = STA_FSM_ENABLE;
            break;

        case STA_TIMER_SPEC_PLMN_ABORT:
            g_StatusContext.aFsmSub[TimerIndex].ulOpId        = STA_OPID_INVALID;
            g_StatusContext.aFsmSub[TimerIndex].ucFsmStateSub = STA_FSMSUB_NULL;
            g_StatusContext.aFsmSub[TimerIndex].TimerName     = STA_TIMER_NULL;
            g_StatusContext.aucFsmSubFlag[TimerIndex]         = STA_FSMSUB_NULL;

            /* ��AT�ظ�ָ���ѳ�ʱ��� */
            g_StatusContext.PhoneError    = TAF_ERR_TIME_OUT;
            g_StatusContext.ucOperateType = STA_OP_PHONEERROR;

            /* �ϱ�APP��ǰ״̬�¼� */
            Sta_EventReport (g_StatusContext.ulCurOpId, TAF_PH_EVT_PLMN_SEL_CNF);

            /* Ǩ��״̬��ENABLE״̬ */
            g_StatusContext.ulFsmState = STA_FSM_ENABLE;
            break;

        case STA_TIMER_PLMN_LIST_ABORT:

            TAF_MMA_RcvTimerPlmnListAbortCnfExpired(TimerIndex);

            break;

        default :
            /* do nothing */
            /* ������ٵ�����Ϣ */
            STA_TRACE (STA_NORMAL_PRINT,
                "Sta_TimeoutProc():NORMAL:do nothing");
            return STA_ERROR;
    }

    return STA_SUCCESS;     /* �������� */
}

/* ɾ�� Sta_SoftReset ( VOS_UINT8 ucResetMode )*/



VOS_UINT32 Sta_PlmnListAbort ( VOS_UINT32 ulOpID )
{
    VOS_UINT32                      ulRet;
    /* void                        *pRet; */
    /* ST_MMC_PLMN_LIST_ABORT_REQ  *pReq; */    /* ������̲���ָ�� */

    /* ��ڲ����Ϸ��Լ�� */
    if ( Sta_IsInvalidOpId (ulOpID) )
    {
        STA_TRACE(STA_WARNING_PRINT, "Sta_PlmnListAbort():WARNING: parameter invalid");
        return STA_ERROR;
    }

    /* ����״̬PLMN_LIST�µ���Ӧ */
    if ( g_StatusContext.ulFsmState & STA_FSM_PLMN_LIST )
    {
        ulRet = TAF_MMA_SndMmcPlmnListAbortReq();

        if ( VOS_ERR == ulRet )
        {
            STA_TRACE (STA_ERROR_PRINT,
                "Sta_PlmnListAbort():ERROR: failed");
            return STA_ERROR;     /* ���󷵻� */
        }

        /*�����״̬*/
        ulRet = Sta_UpdateFsmFlag (STA_FSMSUB_NULL, STA_FSM_ENABLE, STA_UPDATE_MONO_CLEAR);
        if (STA_ERROR == ulRet)
        {
            STA_TRACE(STA_WARNING_PRINT, "Sta_PlmnListAbort():WARNING:Sta_UpdateFsmFlag failed\n");
        }

        /* �ϱ�APP��ǰ״̬�¼� */
        Sta_EventReport (ulOpID, TAF_PH_EVT_PLMN_LIST_ABORT_CNF);

        return STA_SUCCESS;       /* �������� */
    }

    /* PLMN_LIST_ABORT ���������쳣 */
    g_StatusContext.PhoneError    = TAF_ERR_PHONE_MSG_UNMATCH;
    g_StatusContext.ucOperateType = STA_OP_PHONEERROR;

    /* ��APP��������¼� */
    Sta_EventReport (ulOpID, TAF_PH_EVT_PLMN_LIST_ABORT_CNF);

    /* ������ٵ�����Ϣ */
    STA_TRACE (STA_WARNING_PRINT,
        "Sta_PlmnListAbort():WARNING:system FSM state is wrong");

    return STA_ERROR;            /* ���󷵻� */
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*---------- Э��ջ��Ӧ��Ϣ������(�ڲ�ʹ��) -----------*/
/*=======================================================*/



/* Sta_AttachCnf_SysCfgSet*/




VOS_UINT32 Sta_AttachCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    STA_PHONE_DATA_S                    Update; /* ��Ҫ���µ�ȫ������ */
    VOS_UINT8                           ucSub;          /* ��״̬������ */
    VOS_UINT32                          ulOpId;         /* �����������ʶ */
    VOS_UINT32                          ulRet;
    VOS_UINT8                           ucFsmSubSta;    /*��ʾ��ǰ���в�������*/
    VOS_UINT8                           ucOperate;      /*���ڼ�¼����ǰ-cnf��REQ*/

    MMC_MMA_ATTACH_CNF_STRU            *pAttachCnf;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enCsServiceStatus;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enPsServiceStatus;

    enCsServiceStatus = TAF_SDC_GetCsServiceStatus();
    enPsServiceStatus = TAF_SDC_GetPsServiceStatus();

    pAttachCnf = (MMC_MMA_ATTACH_CNF_STRU*)pstMsg;

    ulOpId  =   pAttachCnf->ulOpID;

    /* ATTACHCNF״̬������������쳣*/
    if ((g_StatusContext.ulFsmState != STA_FSM_DE_ATTACH)
        && (g_StatusContext.ulFsmState != STA_FSM_ENABLE)
        && (g_StatusContext.ulFsmState != STA_FSM_PLMN_SEL)
        && (g_StatusContext.ulFsmState != STA_FSM_SYS_CFG_SET)
       )
    {
        /* �������DE_ATTACH״̬��ENABLE״̬�����ش��� */
        /* ��ӡ���Ը�����Ϣ */
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_AttachCnf ():WARNING: system FSM state is wrong");
        return VOS_TRUE;     /* ���󷵻�*/
    }/* ATTACHCNF״̬������������쳣*/

    

    /*������ENABLE״̬��ATTACH_CNF�Ľ���*/
    if ( STA_FSM_DE_ATTACH == g_StatusContext.ulFsmState)
    {
        /*ʹ�õ�ǰ����������ȷ���ϴβ������е�����ֵ*/
        if (  MMA_MMC_SRVDOMAIN_CS == pAttachCnf->enCnDomainId )
        {
            ucFsmSubSta = STA_FSMSUB_ATTACH_CS;
        }
        else if (MMA_MMC_SRVDOMAIN_PS == pAttachCnf->enCnDomainId )
        {
            ucFsmSubSta = STA_FSMSUB_ATTACH_PS;
        }
        else
        {
            ucFsmSubSta = STA_FSMSUB_NULL;
        }
        ucSub   = (VOS_UINT8)Sta_FindFsmSubBySubSta (ucFsmSubSta);

        if ( STA_FSMSUB_INDEX_NULL == ucSub )
        {
            /* ������ٵ�����Ϣ */
            STA_TRACE (STA_WARNING_PRINT,
                "Sta_AttachCnf():WARNING:wrong message parameter");

            return VOS_TRUE;   /* ���󷵻� */
        }

        /* �ر�ATTACH��ʱ�� */
        ulRet = NAS_StopRelTimer(WUEPS_PID_MMA,
                                 g_StatusContext.aFsmSub[ucSub].TimerName,
                                 &g_StatusContext.ahStaTimer[ucSub]);

        if ( VOS_OK != ulRet )
        {
            STA_TRACE (STA_WARNING_PRINT,
                "Sta_AttachCnf():WARNING:NAS_StopRelTimer failed");
        }
        g_StatusContext.ahStaTimer[ucSub] = 0;


        /* ���������̸��� */
        if ((MMA_MMC_SRVDOMAIN_CS == pAttachCnf->enCnDomainId)
         || (MMA_MMC_SRVDOMAIN_PS == pAttachCnf->enCnDomainId))
        {
            g_StatusContext.ucAttachReq--;
        }
        else
        {
            g_StatusContext.ucAttachReq = 0;
        }



        /* ����״̬�ϱ���ͳһ�ŵ�Sta_ServiceStatusInd()����,attach cnf �� detach cnf���������� */
        
        /* �޸ģ�Attach CS+PSʱ��CS��PS����һ�����������񣬾���ΪATTACH�ɹ���
           ֻ�������򶼲�����������ʱ���Ÿ�AT�ϱ�ERROR */
        if (MMA_MMC_SRVDOMAIN_CS == pAttachCnf->enCnDomainId)
        {
            /* ����״̬�ϱ���ͳһ�ŵ�Sta_ServiceStatusInd()����,attach cnf �� detach cnf���������� */
            if (MMA_MMC_SERVICE_STATUS_NO_CHANGE != pAttachCnf->enServiceStatus)
            {
                enCsServiceStatus = (VOS_UINT8)pAttachCnf->enServiceStatus;
            }
            g_StatusContext.ucOperateType     = STA_OP_CSSERVICESTATUS;
        }
        if (MMA_MMC_SRVDOMAIN_PS == pAttachCnf->enCnDomainId)
        {
            /* ����״̬�ϱ���ͳһ�ŵ�Sta_ServiceStatusInd()����,attach cnf �� detach cnf���������� */

            if (MMA_MMC_SERVICE_STATUS_NO_CHANGE != pAttachCnf->enServiceStatus)
            {
                enPsServiceStatus = (VOS_UINT8)pAttachCnf->enServiceStatus;
            }

            g_StatusContext.ucOperateType     = STA_OP_PSSERVICESTATUS;
        }

        /*�ϱ�ATTACH���*/
        if ( STA_FSMSUB_ATTACH_CS == g_StatusContext.aucFsmSubFlag[ucSub] )
        {
            ucOperate = TAF_PH_EVT_CS_ATTACH_CNF;

            TAF_SDC_SetCsAttachAllowFlg(VOS_TRUE);

            /* �����ж�CS ATTACH�����ԭ��ֵ,����������������ϱ�error */
            if (MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE != enCsServiceStatus)
            {
                g_StatusContext.ucOperateType |= STA_OP_PHONEERROR;
                g_StatusContext.PhoneError = TAF_ERR_PHONE_ATTACH_FORBIDDEN;
            }

        }
        else if ( STA_FSMSUB_ATTACH_PS == g_StatusContext.aucFsmSubFlag[ucSub] )
        {
            ucOperate = TAF_PH_EVT_PS_ATTACH_CNF;

            TAF_SDC_SetPsAttachAllowFlg(VOS_TRUE);
            
            /* �����ж�PS ATTACH�����ԭ��ֵ,����������������ϱ�error */
            if (MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE != enPsServiceStatus)
            {
                g_StatusContext.ucOperateType |= STA_OP_PHONEERROR;
                g_StatusContext.PhoneError     = TAF_ERR_PHONE_ATTACH_FORBIDDEN;
            }
        }

        else if ( STA_FSMSUB_ATTACH_CSPS == g_StatusContext.aucFsmSubFlag[ucSub])
        {
            ucOperate = TAF_PH_EVT_ATTACH_CNF;

            TAF_SDC_SetCsAttachAllowFlg(VOS_TRUE);
            TAF_SDC_SetPsAttachAllowFlg(VOS_TRUE);

            /*�����ж�CS+PS ATTACH�����ԭ��ֵ��ֻ�������򶼲�����������ʱ���Ÿ�AT�ϱ�ERROR */
          if ((TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE != enCsServiceStatus )
           && (TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE != enPsServiceStatus) )
            {
                g_StatusContext.ucOperateType |= STA_OP_PHONEERROR;
                g_StatusContext.PhoneError = TAF_ERR_PHONE_ATTACH_FORBIDDEN;
            }
        }
        else
        {
            ucOperate = TAF_PH_EVT_ERR;
        }

        /* ����ϵͳ״̬���� */
        Update.ucDomainFlag      = (VOS_UINT8)(pAttachCnf->enCnDomainId);
        Update.ulCsServiceStatus = TAF_SDC_GetCsServiceStatus();
        Update.ulPsServiceStatus = TAF_SDC_GetPsServiceStatus();
        Update.ulCsCause         = 0;
        Update.ulPsCause         = 0;
        Update.PlmnId.Mcc        = pAttachCnf->stPlmnId.ulMcc;
        Update.PlmnId.Mnc        = pAttachCnf->stPlmnId.ulMnc;
        /*
          ϵͳ���ɲ����з�����������:
          1��ȱ����״̬����ֵ��
          2������Sta_UpdateFsmFlag()��Update.ucFsmSubOperate
             ��Ϊ��״̬����ֵ��
        */

        /* ����ϵͳ��״̬������ֵ��*/

        Update.ucFsmSubOperate   = STA_FSMSUB_FINISH;
        Update.ucFsmUpdateFlag   = STA_UPDATE_SINGL_CLEAR;
        Update.ucFsmStateSub     = g_StatusContext.aFsmSub[ucSub].ucFsmStateSub;

        /* �޸�ϵͳ״̬����״̬��ֵ */
        if (g_StatusContext.ucAttachReq <= 0)
        {
            /* ATTACH����ȫ����ɣ�״̬��ֵ��λ */
            if (0 == g_StatusContext.ucDetachReq)
            {
                Update.ulFsmState           = STA_FSM_ENABLE;
            }
            else
            {
                Update.ulFsmState           = STA_FSM_NO_UPDATE;
            }
            g_StatusContext.ucAttachReq = 0;
            /*
            V200R001�汾��Ҫ�ڿͻ�����Ŀͻ�
            ATTACH�Ľ���ϱ��������¸Ķ���
            */
            if (STA_OPID_INSIDE != ulOpId)
            {
                /*����������ò��������ATTACH����*/
                if( MMA_SET_MSCLASSTYPE_NO ==
                    gstMmaValue.stSetMsClass.enSetMsClassTypeflag)
                {
                     Sta_EventReport (ulOpId, ucOperate);
                }
                else
                {
                    /*
                    ����������ֻ�ģʽ���̵�ATTACH��Ӧ
                    ��˵�����ò��������Ѿ���ɣ����
                    ��ȫ�ֱ��������ϱ�����ɡ�
                    */
                    Sta_SyncMsClassType(ulOpId,
                              gstMmaValue.stSetMsClass.NewMsClassType);
                }
            }
        }
        else
        {/* ATTACH����δȫ����ɣ�״̬��ֵ���䣬
            �����ȴ�ATTACH�Ľ��
         */
            Update.ulFsmState    = STA_FSM_NO_UPDATE;
        }
        Update.ucFsmSubOperate   = STA_FSMSUB_FINISH;

        ulRet                    = Sta_UpdateData (&Update);
        if ( STA_ERROR == ulRet )
        {
            STA_TRACE (STA_WARNING_PRINT,
                "Sta_AttachCnf():WARNING:Sta_UpdateData failed");
            return STA_ERROR;       /* ���󷵻� */
        }
    }
    /* ����״̬�ϱ���ͳһ�ŵ�Sta_ServiceStatusInd()����,attach cnf �� detach cnf���������� */

    return VOS_TRUE;         /* �������� */
}


VOS_UINT32   Sta_SyncMsClassType(VOS_UINT32 ulOpId,
                                 TAF_PH_MS_CLASS_TYPE ucMsClass)
{
    VOS_UINT16   ClientId;
    VOS_UINT8          Id;
    VOS_UINT32      ulRet;
    VOS_UINT8       ucPhoneEvent;


    Id       = 0;
    ClientId = 0;

    ulRet = MMA_GetIdByTi(TAF_MMA, (VOS_UINT8)ulOpId, &ClientId, &Id, &ucPhoneEvent);
    if ( MMA_SUCCESS != ulRet )
    {
        MMA_WARNINGLOG("Sta_SyncMsClassType():WARING:MMA_GetIdByTi failed");
    }

    /*
    ����ȡTIʧ�ܣ���Ӧ�ü���ͬ��ȫ�ֱ�����
    ���޷��ϱ��������֤���̿��Լ�������ȥ
    */
    MMA_SyncMsClassType(ClientId, Id, ucMsClass);
    return MMA_SUCCESS;
}


VOS_UINT32 Sta_DetachCnf_SysCfgSet(MMC_MMA_DETACH_CNF_STRU *pDetachCnf )
{
    STA_PHONE_DATA_S                    Update;
    VOS_UINT32                          ulRst;
    MMA_SYS_CFG_SET_STRU               *pstSysCfgSet;
    TAF_PH_MS_CLASS_TYPE                ucClassType;

    MMA_MMC_ATTACH_TYPE_ENUM_UINT32     enAttachType;

    TAF_CS_SERVICE_ENUM_UINT32          enCsState;

    pstSysCfgSet = &gstMmaValue.stSysCfgSet;


    /*CS DETACH����ϱ�*/
    if (MMA_MMC_SRVDOMAIN_CS == pDetachCnf->enCnDomainId)
    {
        /*���CS DETACH��־*/
        pstSysCfgSet->usSetFlag &= ~MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_CS_DETACH;
    }

    /*PS DETACH����ϱ�*/
    if (MMA_MMC_SRVDOMAIN_PS == pDetachCnf->enCnDomainId)
    {
        /*���PS DETACH��־*/
        pstSysCfgSet->usSetFlag &= ~MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_PS_DETACH;
    }


    if ((!MMA_BIT_ISVALID(pstSysCfgSet->usSetFlag, MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_CS_DETACH))
        && (!MMA_BIT_ISVALID(pstSysCfgSet->usSetFlag, MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_PS_DETACH)))
    {
        /* ֹͣ������ʱ�� */
        if ( MMA_TIMER_RUN == g_stSyscfgWaitDetachCnfTimer.ucTimerStatus )
        {
            ulRst = NAS_StopRelTimer(WUEPS_PID_MMA,
                                     TI_MN_MMA_SYSCFG_WAIT_DETACH_CNF,
                                     &(g_stSyscfgWaitDetachCnfTimer.MmaHTimer));
            if (VOS_OK != ulRst)
            {
                STA_TRACE (STA_WARNING_PRINT, "Sta_DetachCnf_SysCfgSet():WARNING:NAS_StopRelTimer failed");
            }

            /*����ֹͣ��ʱ����־*/
            g_stSyscfgWaitDetachCnfTimer.ucTimerStatus = MMA_TIMER_STOP;
        }
    }
    else
    {
        /* ��Detach��Ϣ��־ʱ�����ȴ�  */
        return STA_SUCCESS;
    }

    /*ATTACH,DETACH��־�Ѿ����,��������������ñ�־*/
    if (MMA_SYS_CFG_NONE_SET == (pstSysCfgSet->usSetFlag & MMA_SYS_CFG_SRV_DOMAIN_NEED_ATTACH_DETACH))
    {
        pstSysCfgSet->usSetFlag &= ~MMA_SYS_CFG_SRV_DOMAIN_SET;
    }

    STA_TRACE1 (STA_INFO_PRINT,
                "Sta_DetachCnf_SysCfgSet:INFO:SYSCFG setflag:",
                pstSysCfgSet->usSetFlag);

    /*DETACH��־�Ѿ����,�����г������������������������*/
    if ((!MMA_BIT_ISVALID(pstSysCfgSet->usSetFlag, MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_CS_DETACH))
        && (!MMA_BIT_ISVALID(pstSysCfgSet->usSetFlag, MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_PS_DETACH))
        && (MMA_SYS_CFG_NONE_SET != (pstSysCfgSet->usSetFlag&MMA_SYS_CFG_SND_TO_MMC)))
    {
        if (!((MMA_BIT_ISVALID(gstMmaValue.stSysCfgSet.usSetFlag, MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_CS_ATTACH))
            || (MMA_BIT_ISVALID(gstMmaValue.stSysCfgSet.usSetFlag, MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_PS_ATTACH))))
        {
            MMA_ChangeSrv2Class(gstMmaValue.stSysCfgSet.stSysCfgSetPara.ucSrvDomain, &ucClassType);

            /* �˴�����Ҫ֪ͨSTK��MSGģ��CS����������Ϊ�÷�֧��ҪSYSCFG���õģ���MMA�յ�SYSCFG���ûظ�ʱ��֪ͨ */
            TAF_MMA_SndMmcModeChangeReq((MMA_MMC_MS_MODE_ENUM_UINT32)ucClassType);

            enCsState = TAF_MMA_GetCsStateFromSyscfgSetFlag(pstSysCfgSet->usSetFlag);
            if (TAF_CS_SERVICE_BUTT != enCsState)
            {
                TAF_MMA_SndStkCsServiceChangeNotify(enCsState);
                TAF_MMA_SndMsgCsServiceChangeNotify(enCsState);
            }

            /* �����ʶ:�ñ�ʶʱû���ж��ǲ���֧��SVLTE�����Բ�֧��SVLTEʱ��Ҳ��Ҫͳһ��� */
            TAF_MMA_ClearSyscfgCsServiceChangeFlag(pstSysCfgSet->usSetFlag);
        }

        TAF_MMA_SndMmcSysCfgReq(&(pstSysCfgSet->stSysCfgSetPara), pstSysCfgSet->usSetFlag);
    }
    /*DETACH��־�Ѿ����,û�г������������������������,��ATTACH��־����*/
    else if ((!MMA_BIT_ISVALID(pstSysCfgSet->usSetFlag, MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_CS_DETACH))
             && (!MMA_BIT_ISVALID(pstSysCfgSet->usSetFlag, MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_PS_DETACH))
             && (MMA_BIT_ISVALID(pstSysCfgSet->usSetFlag, MMA_SYS_CFG_SRV_DOMAIN_SET)))
    {
        if (VOS_OK == TAF_MMA_ConvertAttachTypeToMmc(gstMmaValue.stSysCfgSet.ucAttachType, &enAttachType))
        {
            /*ATTACH��Ϣ��Ҫ�ȷ���MODE CHANGE��Ϣ*/
            MMA_ChangeSrv2Class(gstMmaValue.stSysCfgSet.stSysCfgSetPara.ucSrvDomain, &ucClassType);
            TAF_MMA_SndMmcModeChangeReq((MMA_MMC_MS_MODE_ENUM_UINT32)ucClassType);

            enCsState = TAF_MMA_GetCsStateFromSyscfgSetFlag(pstSysCfgSet->usSetFlag);
            if (TAF_CS_SERVICE_BUTT != enCsState)
            {
                TAF_MMA_SndStkCsServiceChangeNotify(enCsState);
                TAF_MMA_SndMsgCsServiceChangeNotify(enCsState);
            }

            /* �����ʶ:�ñ�ʶʱû���ж��ǲ���֧��SVLTE�����Բ�֧��SVLTEʱ��Ҳ��Ҫͳһ��� */
            TAF_MMA_ClearSyscfgCsServiceChangeFlag(pstSysCfgSet->usSetFlag);

            TAF_MMA_SndMmcAttachReq(STA_OPID_INSIDE, enAttachType, TAF_MMA_ATTACH_REASON_INITIAL);
        }

        pstSysCfgSet->usSetFlag = MMA_SYS_CFG_NONE_SET;

        TAF_MMA_ReportSysCfgSetCnf(TAF_ERR_NO_ERROR);
    }
    else
    {

        /*�ϱ����ý�� */
        TAF_MMA_ReportSysCfgSetCnf(TAF_ERR_NO_ERROR);

        MMA_ChangeSrv2Class(gstMmaValue.stSysCfgSet.stSysCfgSetPara.ucSrvDomain, &ucClassType);
        TAF_MMA_SndMmcModeChangeReq((MMA_MMC_MS_MODE_ENUM_UINT32)ucClassType);

        enCsState = TAF_MMA_GetCsStateFromSyscfgSetFlag(pstSysCfgSet->usSetFlag);
        if (TAF_CS_SERVICE_BUTT != enCsState)
        {
            TAF_MMA_SndStkCsServiceChangeNotify(enCsState);
            TAF_MMA_SndMsgCsServiceChangeNotify(enCsState);
        }

        /* �����ʶ:�ñ�ʶʱû���ж��ǲ���֧��SVLTE�����Բ�֧��SVLTEʱ��Ҳ��Ҫͳһ��� */
        TAF_MMA_ClearSyscfgCsServiceChangeFlag(pstSysCfgSet->usSetFlag);

        gstMmaValue.stSysCfgSet.usSetFlag = MMA_SYS_CFG_NONE_SET;

    }


    /* ����ϵͳ״̬���� */
    Update.ucDomainFlag = (VOS_UINT8)pDetachCnf->enCnDomainId;

    Update.ulCsServiceStatus = TAF_SDC_GetCsServiceStatus();
    Update.ulPsServiceStatus = TAF_SDC_GetPsServiceStatus();


    Update.ulCsCause = 0;
    Update.ulPsCause = 0;
    Update.ucFsmStateSub = STA_FSMSUB_NULL;

    if (MMA_SYS_CFG_NONE_SET == pstSysCfgSet->usSetFlag)
    {
        Update.ulFsmState = STA_FSM_ENABLE;
    }
    else
    {
        Update.ulFsmState = STA_FSM_NO_UPDATE;
    }

    Update.ucFsmSubOperate = STA_FSMSUB_FINISH;
    Update.ucFsmUpdateFlag = STA_UPDATE_DUAL_CLEAR;

    ulRst = Sta_UpdateData (&Update);
    if (STA_ERROR == ulRst)
    {
        STA_TRACE (STA_WARNING_PRINT,
                   "Sta_DetachCnf():WARNING:Sta_UpdateData failed  ");
    }
    return ulRst;
}
VOS_UINT32 Sta_DetachCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    STA_PHONE_DATA_S                    Update;                                 /* ��Ҫ���µ�ȫ������ */
    VOS_UINT32                          opId;                                   /* �����������ʶ */
    VOS_UINT8                           ucSub;                                  /* ��״̬������ */
    VOS_UINT32                          ulRet;
    VOS_UINT8                           ucFsmSubSta;                            /*��ʾ��ǰ���в�������*/

    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

    /* ����״̬�ϱ���ͳһ�ŵ�Sta_ServiceStatusInd()����,attach cnf �� detach cnf���������� */

    MMC_MMA_DETACH_CNF_STRU            *pDetachCnf;

    pDetachCnf = (MMC_MMA_DETACH_CNF_STRU*)pstMsg;

    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();


    /* DETACH CNF�Ƿ�׼����������׼����ֱ�ӷ��� */
    if ( VOS_FALSE == TAF_MMA_IsDetachCnfEnable())
    {
        return VOS_TRUE;
    }

    /* ����״̬�ϱ���ͳһ�ŵ�Sta_ServiceStatusInd()����,attach cnf �� detach cnf���������� */

    /*������ENABLE״̬��ATTACH_CNF�Ľ���*/
    if ( STA_FSM_DE_ATTACH == g_StatusContext.ulFsmState)
    {
        /* ����״̬�ϱ���ͳһ�ŵ�Sta_ServiceStatusInd()����,attach cnf �� detach cnf���������� */


        /* û��DETACH REQ,�����쳣���� */
        if (VOS_TRUE != TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_DETACH_REQ, &ucCtxIndex))
        {
            return VOS_TRUE;
        } 

        /* ���Ҷ�Ӧ����״̬*/
        opId  = pDetachCnf->ulOpID;
        /*ʹ�õ�ǰ����������ȷ���ϴβ������е�����ֵ*/
        if (  MMA_MMC_SRVDOMAIN_CS == pDetachCnf->enCnDomainId )
        {
            /* ����״̬�ϱ���ͳһ�ŵ�Sta_ServiceStatusInd()����,attach cnf �� detach cnf���������� */
            ucFsmSubSta = STA_FSMSUB_DETACH_CS;
        }
        else if (MMA_MMC_SRVDOMAIN_PS == pDetachCnf->enCnDomainId )
        {
            /* ����״̬�ϱ���ͳһ�ŵ�Sta_ServiceStatusInd()����,attach cnf �� detach cnf���������� */
            ucFsmSubSta = STA_FSMSUB_DETACH_PS;
        }
        else
        {
            ucFsmSubSta = STA_FSMSUB_NULL;
        }


        ucSub   = (VOS_UINT8)Sta_FindFsmSubBySubSta (ucFsmSubSta);
        if ( STA_FSMSUB_INDEX_NULL == ucSub )
        {/* û��ƥ���������� */


            /* ��APP����DETACH CNF */
            TAF_MMA_SndDetachCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                                 TAF_MMA_APP_OPER_RESULT_FAILURE,
                                 TAF_ERR_PHONE_MSG_UNMATCH);

            TAF_MMA_ClearOperCtx(ucCtxIndex);

            /* ������ٵ�����Ϣ */
            STA_TRACE (STA_WARNING_PRINT,
                "Sta_DetachCnf():WARNING:wrong message parameter");

            return VOS_TRUE;
        }

        /*�ر�DETACH��ʱ��*/
        ulRet = NAS_StopRelTimer(WUEPS_PID_MMA,
                                 g_StatusContext.aFsmSub[ucSub].TimerName,
                                 &g_StatusContext.ahStaTimer[ucSub]);


        if ( VOS_OK != ulRet )
        {
            STA_TRACE (STA_WARNING_PRINT,
                "Sta_DetachCnf():WARNING:NAS_StopRelTimer failed  ");
        }
        g_StatusContext.ahStaTimer[ucSub] = 0;

        /* ���������̸��� */
        if ((MMA_MMC_SRVDOMAIN_CS == pDetachCnf->enCnDomainId)
         || (MMA_MMC_SRVDOMAIN_PS == pDetachCnf->enCnDomainId))
        {
            g_StatusContext.ucDetachReq--;
        }

        /*�ϱ�detach���*/
        /*ֻ�ϱ���ǰ��Ч����״̬*/
        if (MMA_MMC_SRVDOMAIN_CS == pDetachCnf->enCnDomainId)
        {
            g_StatusContext.ucOperateType |= STA_OP_CSSERVICESTATUS;

            /* ����״̬�ϱ���ͳһ�ŵ�Sta_ServiceStatusInd()����,attach cnf �� detach cnf���������� */
            
            /*�����ж�DETACH�����ԭ��ֵ*/
            if (MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE == pDetachCnf->enServiceStatus)
            {
                g_StatusContext.ucOperateType |= STA_OP_PHONEERROR;
                g_StatusContext.PhoneError     = TAF_ERR_PHONE_DETACH_FORBIDDEN;
            }
        }
        if (MMA_MMC_SRVDOMAIN_PS == pDetachCnf->enCnDomainId)
        {
            g_StatusContext.ucOperateType |= STA_OP_PSSERVICESTATUS;
            
            /* ����״̬�ϱ���ͳһ�ŵ�Sta_ServiceStatusInd()����,attach cnf �� detach cnf���������� */
            
            /*�����ж�DETACH�����ԭ��ֵ*/
            if (MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE == pDetachCnf->enServiceStatus)
            {
                g_StatusContext.ucOperateType |= STA_OP_PHONEERROR;
                g_StatusContext.PhoneError     = TAF_ERR_PHONE_DETACH_FORBIDDEN;
            }
        }


        /*�ϱ�detach���*/
        if ( STA_FSMSUB_DETACH_CS == g_StatusContext.aucFsmSubFlag[ucSub] )
        {
            TAF_SDC_SetCsAttachAllowFlg(VOS_FALSE);
        }
        else if ( STA_FSMSUB_DETACH_PS == g_StatusContext.aucFsmSubFlag[ucSub] )
        {
            TAF_SDC_SetPsAttachAllowFlg(VOS_FALSE);
        }
        else if ( STA_FSMSUB_DETACH_CSPS == g_StatusContext.aucFsmSubFlag[ucSub] )

        {
           /*g_StatusContext.ucOperateType = STA_OP_CSSERVICESTATUS|STA_OP_PSSERVICESTATUS;*/

           TAF_SDC_SetCsAttachAllowFlg(VOS_FALSE);
           TAF_SDC_SetPsAttachAllowFlg(VOS_FALSE);
        }
        else
        {
        }

        /* ����ϵͳ״̬���� */
        Update.ucDomainFlag      = (VOS_UINT8)pDetachCnf->enCnDomainId;
        Update.ulCsServiceStatus = TAF_SDC_GetCsServiceStatus();
        Update.ulPsServiceStatus = TAF_SDC_GetPsServiceStatus();
        Update.ulCsCause         = 0;
        Update.ulPsCause         = 0;

        /*��ȷ����״̬������ʱ��Ӧʹ�õ�ǰ����״ֵ̬��������ʹ����״̬��־����״̬��־
        ��ͬʱ����������Ĳ���ʱ������״ֵ̬����ȣ����������ȡ�*/
        Update.ucFsmStateSub     = g_StatusContext.aFsmSub[ucSub].ucFsmStateSub;

        if ( g_StatusContext.ucDetachReq == 0 )
        {
            if ( 0 == g_StatusContext.ucAttachReq )
            {
                if (STA_FSM_RESTART == g_StatusContext.ulPreFsmState)
                {
                    Update.ulFsmState             = g_StatusContext.ulPreFsmState;

                    g_StatusContext.ulPreFsmState = STA_FSM_NULL;
                }
                else
                {
                    Update.ulFsmState           = STA_FSM_ENABLE;

                }
            }
            else
            {
                Update.ulFsmState           = STA_FSM_NO_UPDATE;
            }
            g_StatusContext.ucDetachReq = 0;
        }
        else
        {
            Update.ulFsmState        = STA_FSM_NO_UPDATE;
        }
        Update.ucFsmSubOperate   = STA_FSMSUB_FINISH;
        Update.ucFsmUpdateFlag   = STA_UPDATE_SINGL_CLEAR;

        ulRet = Sta_UpdateData (&Update);
        if ( STA_ERROR == ulRet )
        {
            STA_TRACE (STA_WARNING_PRINT,
                "Sta_DetachCnf():WARNING:Sta_UpdateData failed  ");
        }
        /*
        ��DETACH�����Ĺ��̲����������̣�
        ���Ѿ����DETACH���������
        */
        if ( (STA_OPID_INSIDE != opId) && (0 == g_StatusContext.ucDetachReq))
        {
            /*�������ò���������DETACH����*/
            if (MMA_SET_MSCLASSTYPE_NO == gstMmaValue.stSetMsClass.enSetMsClassTypeflag )
            {
                if (STA_OP_PHONEERROR == (g_StatusContext.ucOperateType & STA_OP_PHONEERROR))
                {
                    TAF_MMA_SndDetachCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                                         TAF_MMA_APP_OPER_RESULT_FAILURE,
                                         g_StatusContext.PhoneError);

                    TAF_MMA_ClearOperCtx(ucCtxIndex);
                }
                else
                {
                    TAF_MMA_SndDetachCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                                         TAF_MMA_APP_OPER_RESULT_SUCCESS,
                                         TAF_ERR_NO_ERROR);

                    TAF_MMA_ClearOperCtx(ucCtxIndex);
                }

            }
            /*�ֻ����͸��ķ�����DETACH����*/
            else if (MMA_SET_MSCLASSTYPE_YES == gstMmaValue.stSetMsClass.enSetMsClassTypeflag )
            {
               ulRet = Sta_ProcSetMsClassTypeforDetach(pDetachCnf);
            }
            else
            {
                /*�쳣��ӡ*/
                MMA_WARNINGLOG("Sta_DetachCnf():WARNING:ERROR Ms Class Type Flag!");
            }
        }
    }
    else if (STA_FSM_SYS_CFG_SET == g_StatusContext.ulFsmState)
    {
        /* ����״̬�ϱ���ͳһ�ŵ�Sta_ServiceStatusInd()����,attach cnf �� detach cnf���������� */
        ulRet = Sta_DetachCnf_SysCfgSet(pDetachCnf);

        if ( STA_ERROR == ulRet )
        {
            STA_TRACE (STA_WARNING_PRINT,
                "Sta_DetachCnf():WARNING:Sta_DetachCnf_SysCfgSet failed  ");
            return VOS_TRUE;
        }
    }
    else
    {   
          /* ����״̬�ϱ���ͳһ�ŵ�Sta_ServiceStatusInd()����,attach cnf �� detach cnf���������� */
    }

    return VOS_TRUE;
}
VOS_UINT32 Sta_SysCfgCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PH_ERR_CODE                     usErrorCode;
    MN_MMA_LAST_SETTED_SYSCFG_SET_STRU *pstLastSyscfgSet = VOS_NULL_PTR;

    TAF_SDC_SYS_MODE_ENUM_UINT8         enSysMode;
    MMA_MMC_ATTACH_TYPE_ENUM_UINT32     enAttachType;
    TAF_PH_MS_CLASS_TYPE                ucClassType;
    TAF_CS_SERVICE_ENUM_UINT32          enCsState;
    MMC_MMA_SYS_CFG_SET_CNF_STRU       *pstSysCfgCnf;

    pstSysCfgCnf = (MMC_MMA_SYS_CFG_SET_CNF_STRU*)pstMsg;

    enSysMode = TAF_SDC_GetSysMode();

    pstLastSyscfgSet     = MN_MMA_GetLastSyscfgSetAddr();

    if (STA_FSM_SYS_CFG_SET == g_StatusContext.ulFsmState)
    {
        if ((MMA_BIT_ISVALID(gstMmaValue.stSysCfgSet.usSetFlag, MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_CS_ATTACH))
         || (MMA_BIT_ISVALID(gstMmaValue.stSysCfgSet.usSetFlag, MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_PS_ATTACH)))
        {
            if (VOS_OK == TAF_MMA_ConvertAttachTypeToMmc(gstMmaValue.stSysCfgSet.ucAttachType, &enAttachType))
            {
                /*ATTACH��Ϣ��Ҫ�ȷ���MODE CHANGE��Ϣ*/
                MMA_ChangeSrv2Class(gstMmaValue.stSysCfgSet.stSysCfgSetPara.ucSrvDomain, &ucClassType);
                TAF_MMA_SndMmcModeChangeReq((MMA_MMC_MS_MODE_ENUM_UINT32)ucClassType);

                enCsState = TAF_MMA_GetCsStateFromSyscfgSetFlag(gstMmaValue.stSysCfgSet.usSetFlag);
                if (TAF_CS_SERVICE_BUTT != enCsState)
                {
                    TAF_MMA_SndStkCsServiceChangeNotify(enCsState);
                    TAF_MMA_SndMsgCsServiceChangeNotify(enCsState);
                }

                /* �����ʶ:�ñ�ʶʱû���ж��ǲ���֧��SVLTE�����Բ�֧��SVLTEʱ��Ҳ��Ҫͳһ��� */
                TAF_MMA_ClearSyscfgCsServiceChangeFlag(gstMmaValue.stSysCfgSet.usSetFlag);

                TAF_MMA_SndMmcAttachReq(STA_OPID_INSIDE, enAttachType, TAF_MMA_ATTACH_REASON_INITIAL);
            }
        }

        g_StatusContext.ulFsmState = g_StatusContext.ulPreFsmState;

        if (MMA_MMC_SYS_CFG_SET_SUCCESS == pstSysCfgCnf->enRst)
        {
            usErrorCode = TAF_ERR_NO_ERROR;


            if (TAF_MMA_SYSCFG_USER_AT == TAF_MMA_GetSyscfgUser())
            {
                 /* ���µ�ǰ���óɹ��Ľ��뼼�� */
                 MMA_PhSysCfgWriteNvim();
            }
            else
            {
                /* ֻ����ȫ�ֱ��� */
                pstLastSyscfgSet->stRatPrioList = gstMmaValue.stSysCfgSet.stSysCfgSetPara.stRatOrder;
            }


            /* ��ǰ�Ľ��뼼�����Ƿ����µĽ��뼼���б���, ����RAT�б���ʱ���ϱ� Mode:0,0*/
            if ((VOS_FALSE == MN_MMA_IsSpecRatSupported(enSysMode))
             && (TAF_SDC_SYS_MODE_BUTT != enSysMode))
            {
                TAF_SDC_SetSysMode(TAF_SDC_SYS_MODE_BUTT);
                TAF_SDC_SetSysSubMode(TAF_SDC_SYS_SUBMODE_NONE);

                Sta_ModeChangeEventReport(TAF_PH_INFO_NONE_RAT, TAF_SYS_SUBMODE_NONE);

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
                /* ��MTCģ���ϱ���ǰ����ģʽ */
                TAF_MMA_SndMtcRatModeInd(TAF_SDC_SYS_MODE_BUTT, NAS_UTRANCTRL_GetCurrUtranMode());
#endif

            }
        }
        else if (MMA_MMC_SYS_CFG_SET_FAILURE_CS_SERV_EXIST == pstSysCfgCnf->enRst)
        {
            usErrorCode = TAF_ERR_SYSCFG_CS_SERV_EXIST;
            /* �����4F36refresh������SYSCFGʧ�ܣ���Ҫ����TI_TAF_MMA_PERIOD_TRYING_RAT_BALANCING��ʱ�� */
            TAF_MMA_RcvSyscfgCnfFail();
        }
        else
        {
            usErrorCode = TAF_ERR_ERROR;

            /* �����4F36refresh������SYSCFGʧ�ܣ���Ҫ����TI_TAF_MMA_PERIOD_TRYING_RAT_BALANCING��ʱ�� */
            TAF_MMA_RcvSyscfgCnfFail();
        }

        /* ���SYSCFG���ñ�־ */
        gstMmaValue.stSysCfgSet.usSetFlag = MMA_SYS_CFG_NONE_SET;

        if (TAF_MMA_SYSCFG_USER_AT == TAF_MMA_GetSyscfgUser())
        {
            TAF_MMA_ReportSysCfgSetCnf(usErrorCode);
        }
    }

    return VOS_TRUE;
}


VOS_UINT32 Sta_ProcSetMsClassTypeforDetach(MMC_MMA_DETACH_CNF_STRU *pDetachCnf )
{
    VOS_UINT32          opId;   /* �����������ʶ */
    VOS_UINT32          ulRet = STA_SUCCESS;
    VOS_UINT16       ClientId;
    VOS_UINT8              Id;
    TAF_PHONE_EVENT     PhoneEvt;

     /* ���Ҷ�Ӧ����״̬*/
    opId  = pDetachCnf->ulOpID;

    /*�Ƚ�������attach,detach������λ��Ч*/
    gstMmaValue.stSetMsClass.enSetMsClassTypeDeAttFlg = MMA_SET_CLASSTYPE_DE_ATTACH_DISABLE;

    /*�ж�Detach�����Ƿ�ɹ�*/
    if(STA_SUCCESS == Sta_SuccessDetach(pDetachCnf) )
    {
        /* BEGIN: Added by liuyang id:48197, 2006/3/2   PN:A32D02310*/
        /*��MMC�㷢��ģʽ�л�����*/
        /* gstMmaValue.stSetMsClass.enSetMsClassTypeflag����û���ó�YES�ĵط���
            ����Sta_ProcSetMsClassTypeforDetach�����Ѿ��߲����ˣ�����Ҫ֪ͨSTK/MSG CS��������� */
        TAF_MMA_SndMmcModeChangeReq((MMA_MMC_MS_MODE_ENUM_UINT32)gstMmaValue.stSetMsClass.NewMsClassType);
        /* END:   Added by liuyang id:48197, 2006/3/2 */

        /*���������жϹ����Ƿ����*/
        if ( STA_SUCCESS == Sta_CompleteDefMsClassType(pDetachCnf) )
        {
            /*����Sta_SyncMsClassType()���ͬ�������ϱ�*/
            Sta_SyncMsClassType(opId,
                           gstMmaValue.stSetMsClass.NewMsClassType);
           ulRet = STA_SUCCESS;
        }
        /*������ΪCG->CC��CC->CGʱ����������ATTACH����*/
        else
        {
            if (TAF_SUCCESS !=  MMA_GetIdByTi(TAF_MMA, (VOS_UINT8)opId, &ClientId, &Id, &PhoneEvt))
            {
                MMA_WARNINGLOG("Sta_ProcSetMsClassTypeforDetach():WARNING:Invoke Taf_GetIdByTi failed");
            }

            /* BEGIN: Added by liuyang id:48197, 2006/3/2   PN:A32D02310*/
            ulRet = MMA_StartInternalTimer(1, MMA_INTERNAL_OP_CLASS_CHANGE, ClientId, Id);
            /* END:   Added by liuyang id:48197, 2006/3/2 */
            /* BEGIN: Deleted by liuyang id:48197, 2006/3/2   PN:A32D02310*/
               /*�ж�����Ҫ�趨���ֻ�����*/
            /*if ( TAF_PH_MS_CLASS_CC ==   */
            /*     gstMmaValue.stSetMsClass.NewMsClassType)  */
            /*{  */
                   /*NULL -> CC*/
                   /* BEGIN: Added by liuyang, 2005/10/26 */
               /*gstMmaValue.stSetMsClass.enSetMsClassTypeDeAttFlg = MMA_SET_CLASSTYPE_ATTACH_ENABLE;  */
               /*MMA_PhoneAttach(ClientId, Id, TAF_PH_CS_OPERATE);  */
               /*Sta_Attach(opId, TAF_CS_ATTACH);  */
                   /* END:   Added by liuyang, 2005/10/26 */
            /*   ulRet = STA_SUCCESS;  */
            /*}  */
            /*else if(TAF_PH_MS_CLASS_CG ==  */
            /*        gstMmaValue.stSetMsClass.NewMsClassType)  */
            /*{  */
                    /*NULL -> CG*/
                   /* BEGIN: Added by liuyang, 2005/10/26 */
               /*gstMmaValue.stSetMsClass.enSetMsClassTypeDeAttFlg = MMA_SET_CLASSTYPE_ATTACH_ENABLE;  */
               /*MMA_PhoneAttach(ClientId, Id, TAF_PH_PS_OPERATE);  */
               /*Sta_Attach(opId, TAF_PS_ATTACH);  */
                   /* END:   Added by liuyang, 2005/10/26 */
                /*ulRet = STA_SUCCESS;  */
            /*}  */
            /*else  */
            /*{  */
                   /*��ӡ������Ϣ*/
               /*STA_TRACE (STA_WARNING_PRINT,  */
               /*             "Sta_ProcSetMsClassTypeforDetach():WARNING: Error MS class attach type  input");  */
               /*ulRet  =  STA_ERROR;  */
            /*}  */
            /* END:   Deleted by liuyang id:48197, 2006/3/2 */
          }
    }
    else
    {
        /*
        ��MMC����MODE_CHANGE_REQ��״̬�ع���δ����״̬
        �ϱ���������ʧ���¼�
        */
        /* gstMmaValue.stSetMsClass.enSetMsClassTypeflag����û���ó�YES�ĵط���
            ����Sta_ProcSetMsClassTypeforDetach�����Ѿ��߲����ˣ�����Ҫ֪ͨSTK/MSG CS��������� */
        TAF_MMA_SndMmcModeChangeReq((MMA_MMC_MS_MODE_ENUM_UINT32)gstMmaValue.stSetMsClass.MsClassType);

        ulRet = Sta_SyncMsClassType(opId, gstMmaValue.stSetMsClass.MsClassType);
    }
    return ulRet;
}


VOS_UINT32 Sta_SuccessDetach(MMC_MMA_DETACH_CNF_STRU *pDetachCnf )
{
    VOS_UINT32                          ulAttachType;
    VOS_UINT32                          ulDetachType;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enCsServiceStatus;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enPsServiceStatus;

    enCsServiceStatus = TAF_SDC_GetCsServiceStatus();
    enPsServiceStatus = TAF_SDC_GetPsServiceStatus();

    Sta_ProcMsClassTypeNewToOld(gstMmaValue.stSetMsClass.MsClassType,
                                gstMmaValue.stSetMsClass.NewMsClassType,
                                &ulAttachType,
                                &ulDetachType);

    if ((STA_PS_DETACH   == ulDetachType)
     && (TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE != enPsServiceStatus))
    {
        return STA_SUCCESS;
    }

    if (( STA_CS_DETACH  == ulDetachType )
     && (TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE != enCsServiceStatus))
    {
        return STA_SUCCESS;
    }

    if ((STA_PS_CS_DETACH == ulDetachType)
     && ((TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE  != enPsServiceStatus)
      && (TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE != enCsServiceStatus)))
    {
        return STA_SUCCESS;
    }

    return STA_ERROR;
}

/*****************************************************************************
 �� �� ��  : Sta_ProcMsClassTypeNewToOld
 ��������  : ͨ����ǰ���ֻ�������Ŀ�����ͣ�����Ҫ���ŵĺ�ȥ���ŵ�����
 �������  : TAF_PH_MS_CLASS_TYPE   CurMsClassType
             TAF_PH_MS_CLASS_TYPE   NewMsClassType
             VOS_UINT32            *ulAttachType
             VOS_UINT32            *ulDetachtype
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2005��9��8��
    ��    ��   : liuyang
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 Sta_ProcMsClassTypeNewToOld(TAF_PH_MS_CLASS_TYPE   CurMsClassType,
                                       TAF_PH_MS_CLASS_TYPE   NewMsClassType,
                                       VOS_UINT32            *pulAttachType,
                                       VOS_UINT32            *pulDetachType)
{
   MMA_CLASS_CHANGE_INDEX_ENUM      enMsClassIndex;

  *pulAttachType =  STA_ATTACH_TYPE_NULL;
  *pulDetachType =  STA_DETACH_NULL;

   if (CurMsClassType == NewMsClassType)
   {
        return STA_SUCCESS;
   }

   enMsClassIndex  = (MMA_CLASS_CHANGE_INDEX_ENUM)(((CurMsClassType << 4) & 0xf0)
                                                    |(NewMsClassType & 0x0f));
   switch ( enMsClassIndex )
   {
        /*NULL -> A*/
        case MMA_NULL2A :
            *pulAttachType = STA_ATTACH_TYPE_GPRS_IMSI;
            break;
         /*A -> NULL*/
        case MMA_A2NULL :
            *pulDetachType = STA_PS_CS_DETACH;
            break;

        /*NULL -> CG*/
        case MMA_NULL2CG :
        /*CC -> A*/
        case MMA_CC2A:
            *pulAttachType = STA_ATTACH_TYPE_GPRS;
            break;
        /*NULL -> CC*/
        case MMA_NULL2CC :
        /*CG -> A*/
        case MMA_CG2A :
            *pulAttachType = STA_ATTACH_TYPE_IMSI;
            break;

        /*A -> CG*/
        case MMA_A2CG:
        /*CC ->NULL*/
        case MMA_CC2NULL :
            *pulDetachType = STA_CS_DETACH;
            break;

        /*A -> CC*/
        case MMA_A2CC:
        /*CG -> NULL*/
        case MMA_CG2NULL :
            *pulDetachType = STA_PS_DETACH;
            break;

       /*CG-> CC*/
        case MMA_CG2CC :
            *pulAttachType = STA_ATTACH_TYPE_IMSI;
            *pulDetachType = STA_PS_DETACH;
            break;

        /*CC -> CG*/
        case MMA_CC2CG :
            *pulAttachType = STA_ATTACH_TYPE_GPRS;
            *pulDetachType = STA_CS_DETACH;
            break;

        default:
             STA_TRACE (STA_WARNING_PRINT,
            "Sta_ProcMsClassTypeNewToOld():WARNING:Sta_ProcMsClassType New To Old failed");
        return STA_ERROR;   /* ���󷵻� */
   }
   return STA_SUCCESS;
}
VOS_UINT32 Sta_CompleteDefMsClassType(MMC_MMA_DETACH_CNF_STRU *pDetachCnf )
{
    VOS_UINT32 ulAttachType, ulDetachType;

    /*��ȡ��ǰ��ҪATTACH,DETACH������*/
    Sta_ProcMsClassTypeNewToOld(gstMmaValue.stSetMsClass.MsClassType,
                                gstMmaValue.stSetMsClass.NewMsClassType,
                                &ulAttachType,
                                &ulDetachType);

    /*��ֻ��DETACH���̣���˵���˹��̵����Ѿ����*/
    if (STA_ATTACH_TYPE_NULL == ulAttachType)
    {
        return STA_SUCCESS;
    }
    return STA_ERROR;
}





VOS_UINT32 Sta_SelPlmnSpecializeCnf( VOS_UINT32 ulResult )
{
    VOS_UINT32 ulRet;

    STA_TRACE1(STA_NORMAL_PRINT, "@@@@Sta_SelPlmnSpecializeCnf:FsmState=",(VOS_INT32)g_StatusContext.ulFsmState);

    if ((STA_FSM_PLMN_RESEL == g_StatusContext.ulFsmState)
     && (MMA_MMC_USER_PLMN_SEARCH_RESULT_REJ == ulResult))
    {
        /*�ر�״̬��ʱ��*/
        ulRet = NAS_StopRelTimer(WUEPS_PID_MMA,
                                 g_StatusContext.aFsmSub[0].TimerName,
                                 &g_StatusContext.ahStaTimer[0]);


        if ( VOS_OK != ulRet )
        {
            STA_TRACE (STA_WARNING_PRINT,
                "Sta_ServiceStatusInd():WARNING:NAS_StopRelTimer failed");
        }

        /*������״̬*/
        ulRet = Sta_UpdateFsmFlag(STA_FSMSUB_NULL, STA_FSM_ENABLE, STA_UPDATE_MONO_CLEAR);
        if (STA_ERROR == ulRet)
        {
            STA_TRACE(STA_WARNING_PRINT, "Sta_SelPlmnSpecializeCnf():WARNING:Sta_UpdateFsmFlag failed\n");
        }

         /* �ϱ���ǰָ������ʧ�� */
        g_StatusContext.PhoneError    = TAF_ERR_ERROR;
        g_StatusContext.ucOperateType = STA_OP_PHONEERROR;
        MMA_WARNINGLOG("Sta_SelPlmnSpecializeCnf():WARING:REJECT PLMN USER SEL");
        /* �ϱ�APP��ǰ״̬�¼� */
        Sta_EventReport (g_StatusContext.ulCurOpId, TAF_PH_EVT_PLMN_RESEL_CNF);

        return MMA_SUCCESS;
    }


    /*�ر�״̬��ʱ��*/

    ulRet = NAS_StopRelTimer(WUEPS_PID_MMA,
                             g_StatusContext.aFsmSub[0].TimerName,
                             &g_StatusContext.ahStaTimer[0]);


    if ( VOS_OK != ulRet )
    {
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_ServiceStatusInd():WARNING:NAS_StopRelTimer failed");
    }

    /* BEGIN: Modified by liuyang id:48197, 2006/3/27   PN:A32D02372*/
    /*�ڲ������ָ����������,ֻ�����ֶ���ѡ�Ĺ�����,���PLMNList��ᷢ��*/

    STA_TRACE1(STA_NORMAL_PRINT, "Sta_SelPlmnSpecializeCnf:g_StatusContext.ulCurOpId=", (VOS_INT32)g_StatusContext.ulCurOpId);
    if ( STA_OPID_INSIDE == g_StatusContext.ulCurOpId )
    {
        g_StatusContext.ulCurOpId = 0;
        Sta_EventReport (g_StatusContext.ulCurOpId,
                         TAF_PH_EVT_PLMN_RESEL_CNF);
        STA_TRACE(STA_NORMAL_PRINT,"Sta_SelPlmnSpecializeCnf: 8 reported to AT!");
        return STA_SUCCESS;
    }
    /* END:   Modified by liuyang id:48197, 2006/3/27 */

    NAS_MN_ReportPlmnSrchResult(ulResult);

    ulRet = Sta_UpdateFsmFlag(STA_FSMSUB_NULL, STA_FSM_ENABLE, STA_UPDATE_MONO_CLEAR);
    if (STA_ERROR == ulRet)
    {
        STA_TRACE(STA_WARNING_PRINT, "Sta_SelPlmnSpecializeCnf():WARNING:Sta_UpdateFsmFlag failed\n");
    }
    return MMA_SUCCESS;
}


VOS_UINT8 Sta_GetSysSubMode_W(STA_DATA_TRANSFER_STATUS_UINT8      ucDataTranStatus ,
                                       STA_DATA_TRANSFER_MODE_UINT8        ucDataTranMode
                                       )
{
    VOS_UINT8                          ucSysSubMode;

    if (STA_DATA_TRANSFER_PDP_ACT == ucDataTranStatus)
    {
        switch  ( ucDataTranMode )
        {
            case STA_DATA_TRANSFER_HSDPA :
                ucSysSubMode = TAF_SYS_SUBMODE_HSDPA;
                break;
            case STA_DATA_TRANSFER_HSUPA :
                ucSysSubMode = TAF_SYS_SUBMODE_HSUPA;
                break;
            case STA_DATA_TRANSFER_HSDPA_AND_HSUPA :
                ucSysSubMode = TAF_SYS_SUBMODE_HSDPA_HSUPA;
                break;
            case STA_DATA_TRANSFER_HSPA_PLUS :
                ucSysSubMode = TAF_SYS_SUBMODE_HSPA_PLUS;
                break;

            case STA_DATA_TRANSFER_DC_HSPA_PLUS:
                ucSysSubMode = TAF_SYS_SUBMODE_DC_HSPA_PLUS;
                break;

            case STA_DATA_TRANSFER_DC_MIMO:
                ucSysSubMode = TAF_SYS_SUBMODE_DC_MIMO;
                break;



            default:
                ucSysSubMode = TAF_SYS_SUBMODE_WCDMA;
                break;
        }
    }
    else
    {
        ucSysSubMode = TAF_SYS_SUBMODE_WCDMA;
    }

    return ucSysSubMode;
}


VOS_UINT8 Sta_GetSysSubMode_Td(
    STA_DATA_TRANSFER_STATUS_UINT8      ucDataTranStatus,
    STA_DATA_TRANSFER_MODE_UINT8        ucDataTranMode
)
{
    VOS_UINT8                          ucSysSubMode;

    if (STA_DATA_TRANSFER_PDP_ACT == ucDataTranStatus)
    {
        switch  ( ucDataTranMode )
        {
            case STA_DATA_TRANSFER_HSDPA :
                ucSysSubMode = TAF_SYS_SUBMODE_HSDPA;
                break;

            case STA_DATA_TRANSFER_HSUPA :
                ucSysSubMode = TAF_SYS_SUBMODE_HSUPA;
                break;

            case STA_DATA_TRANSFER_HSDPA_AND_HSUPA :
                ucSysSubMode = TAF_SYS_SUBMODE_HSDPA_HSUPA;
                break;

            case STA_DATA_TRANSFER_HSPA_PLUS :
                ucSysSubMode = TAF_SYS_SUBMODE_HSPA_PLUS;
                break;

            default:
                ucSysSubMode = TAF_SYS_SUBMODE_TD_SCDMA;
                break;
        }
    }
    else
    {
        ucSysSubMode = TAF_SYS_SUBMODE_TD_SCDMA;
    }

    return ucSysSubMode;
}

VOS_VOID Sta_ModeChangeEventReport(VOS_UINT8 ucNewSysMode, VOS_UINT8 ucNewSysSubMode)
{
    TAF_PHONE_EVENT_INFO_STRU *pstPhoneEvent;

    pstPhoneEvent = (TAF_PHONE_EVENT_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMA, sizeof(TAF_PHONE_EVENT_INFO_STRU));

    if (VOS_NULL_PTR == pstPhoneEvent)
    {
        MMA_ERRORLOG("Sta_ModeChangeEventReport:ERROR:ALLOC MEMORY FAIL.");
        return;
    }

    PS_MEM_SET(pstPhoneEvent, 0, sizeof(TAF_PHONE_EVENT_INFO_STRU));

    pstPhoneEvent->OP_Mode      = MMA_TRUE;
    pstPhoneEvent->PhoneEvent   = TAF_PH_EVT_MODE_CHANGE_IND;
    pstPhoneEvent->RatType      = ucNewSysMode;
    pstPhoneEvent->ucSysSubMode = ucNewSysSubMode;

    Taf_PhoneEvent(pstPhoneEvent);

    PS_MEM_FREE(WUEPS_PID_MMA, pstPhoneEvent);

    return;
}



VOS_VOID Sta_UpdateDataTranStatusAndMode(MMA_MMC_DATATRAN_ATTRI_ENUM_UINT8 enDataTranAttri)
{

    switch (enDataTranAttri)
    {
        case MMA_MMC_DATATRAN_ATTRI_PDP_ACT:
            g_StatusContext.ucDataTranStatus = STA_DATA_TRANSFER_PDP_ACT;

            if ((TAF_SDC_SYS_MODE_WCDMA == TAF_SDC_GetSysMode())
             && (STA_DATA_TRANSFER_MODE_BUTT == g_StatusContext.ucDataTranMode))
            {
                g_StatusContext.ucDataTranMode = STA_DATA_TRANSFER_R99_ONLY;
            }
            break;

        case MMA_MMC_DATATRAN_ATTRI_PDP_DEACT:
            g_StatusContext.ucDataTranStatus = STA_DATA_TRANSFER_PDP_DEACT;
            break;

        case MMA_MMC_DATATRAN_ATTRI_R99_ONLY:
            g_StatusContext.ucDataTranMode   = STA_DATA_TRANSFER_R99_ONLY;
            break;

        case MMA_MMC_DATATRAN_ATTRI_HSDPA:
            g_StatusContext.ucDataTranMode   = STA_DATA_TRANSFER_HSDPA;
            break;

        case MMA_MMC_DATATRAN_ATTRI_HSUPA:
            g_StatusContext.ucDataTranMode   = STA_DATA_TRANSFER_HSUPA;
            break;
        case MMA_MMC_DATATRAN_ATTRI_HSDPA_AND_HSUPA:
            g_StatusContext.ucDataTranMode   = STA_DATA_TRANSFER_HSDPA_AND_HSUPA;
            break;

        case MMA_MMC_DATATRAN_ATTRI_HSPA_PLUS:
            g_StatusContext.ucDataTranMode = STA_DATA_TRANSFER_HSPA_PLUS;
            break;

        case MMA_MMC_DATATRAN_ATTRI_LTE_DATA:
            g_StatusContext.ucDataTranStatus = STA_DATA_TRANSFER_PDP_ACT;
            g_StatusContext.ucDataTranMode = STA_DATA_TRANSFER_LTE_DATA;
            break;

        case MMA_MMC_DATATRAN_ATTRI_LTE_IDLE:
            g_StatusContext.ucDataTranMode = STA_DATA_TRANSFER_LTE_IDLE;
            break;

        case MMA_MMC_DATATRAN_ATTRI_DC_HSPA_PLUS:
            g_StatusContext.ucDataTranMode = STA_DATA_TRANSFER_DC_HSPA_PLUS;
            break;

        case MMA_MMC_DATATRAN_ATTRI_DC_MIMO:
            g_StatusContext.ucDataTranMode = STA_DATA_TRANSFER_DC_MIMO;
            break;


        default:
            STA_TRACE(STA_WARNING_PRINT, "Sta_DataTranAttri:Attri incorrect.");
            break;
    }

}


VOS_UINT32 Sta_DataTranAttri(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8   ucOldWSysSubMode = TAF_SYS_SUBMODE_WCDMA;
    VOS_UINT8   ucNewWSysSubMode = TAF_SYS_SUBMODE_WCDMA;

    VOS_UINT8   ucOldTdSysSubMode = TAF_SYS_SUBMODE_TD_SCDMA;
    VOS_UINT8   ucNewTdSysSubMode = TAF_SYS_SUBMODE_TD_SCDMA;

    TAF_SDC_SYS_MODE_ENUM_UINT8         enSysMode;
    MMC_MMA_DATATRAN_ATTRI_IND_STRU    *pstDataTranAttri;

    pstDataTranAttri = (MMC_MMA_DATATRAN_ATTRI_IND_STRU*)pstMsg;


    enSysMode = TAF_SDC_GetSysMode();

    STA_TRACE1(STA_NORMAL_PRINT, "Sta_DataTranAttri:ucDataTranAttri=",(VOS_INT32)pstDataTranAttri->enDataTranAttri);

    if (TAF_SDC_SYS_MODE_WCDMA == enSysMode)
    {
        if (NAS_UTRANCTRL_UTRAN_MODE_FDD == NAS_UTRANCTRL_GetCurrUtranMode())
        {
            ucOldWSysSubMode = Sta_GetSysSubMode_W(g_StatusContext.ucDataTranStatus,
                                                   g_StatusContext.ucDataTranMode);
        }
        else
        {
            ucOldTdSysSubMode = Sta_GetSysSubMode_Td(g_StatusContext.ucDataTranStatus,
                                                     g_StatusContext.ucDataTranMode);
        }
    }

    /* ����ȫ�ֱ����е�DataTranStatus �� DataTranStatus */
    Sta_UpdateDataTranStatusAndMode(pstDataTranAttri->enDataTranAttri);


    /* �ϱ�^MODE�¼������ϵͳ��ģʽ�иı� */
    if (TAF_SDC_SYS_MODE_WCDMA == enSysMode)
    {
        if (NAS_UTRANCTRL_UTRAN_MODE_FDD == NAS_UTRANCTRL_GetCurrUtranMode())
        {
            /* ��ǰUTRAN_MODEΪFDDʱ���ϱ�W��ϵͳ��ģʽ */
            ucNewWSysSubMode = Sta_GetSysSubMode_W(g_StatusContext.ucDataTranStatus,
                                                   g_StatusContext.ucDataTranMode);

            if(ucOldWSysSubMode != ucNewWSysSubMode)
            {

                Sta_ModeChangeEventReport(TAF_PH_INFO_WCDMA_RAT, ucNewWSysSubMode);
            }
        }
        else
        {
            /* ��ǰUTRAN_MODEΪTDDʱ���ϱ�TD��ϵͳ��ģʽ */
            ucNewTdSysSubMode = Sta_GetSysSubMode_Td(g_StatusContext.ucDataTranStatus,
                                                     g_StatusContext.ucDataTranMode);

            if(ucOldTdSysSubMode != ucNewTdSysSubMode)
            {

                Sta_ModeChangeEventReport(TAF_PH_INFO_TD_SCDMA_RAT, ucNewTdSysSubMode);
            }
        }
    }

    return VOS_TRUE;
}


VOS_UINT32 Sta_GetServiceStatus()
{
    return TAF_SDC_GetServiceStatus();
}


VOS_VOID Sta_IccStatusReport(VOS_UINT32 ulCsServiceStatus, VOS_UINT32 ulPsServiceStatus)
{
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enCsServiceStatus;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enPsServiceStatus;
    VOS_UINT8                           ucSimStatus;
    VOS_UINT8                           ucSimLockStatus;
    
    ucSimStatus                         = MMA_GetUsimStatus(VOS_FALSE);
     
    /* ��ȡ����״̬ */
    if ( MMA_SIM_IS_LOCK == MMA_GetMeLockStatus() )
    {
        ucSimLockStatus                 = VOS_TRUE;
    }
    else
    {
        ucSimLockStatus                 = VOS_FALSE;
    }
    

    enCsServiceStatus = TAF_SDC_GetCsServiceStatus();
    enPsServiceStatus = TAF_SDC_GetPsServiceStatus();

    if (((enCsServiceStatus != ulCsServiceStatus)
      && (TAF_SDC_SERVICE_STATUS_NO_IMSI == enCsServiceStatus))
     || ((enPsServiceStatus != ulPsServiceStatus)
      && (TAF_SDC_SERVICE_STATUS_NO_IMSI == enPsServiceStatus)) )
    {
        if (TAF_PH_ME_SIM_UNLOCK == MMA_GetMeLockStatus())
        {
            TAF_MMA_SndAtIccStatusInd(ucSimStatus, ucSimLockStatus);
        }
    }

    return;
}
VOS_VOID Sta_ReportServiceEvent(VOS_UINT32 ulCsServiceStatus, VOS_UINT32 ulPsServiceStatus)
{
    static VOS_UINT8   ucLimitServiceSentFlg = VOS_FALSE;/* ��־limit service�Ƿ��Ѿ��ϱ� */

    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enCsServiceStatus;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enPsServiceStatus;

    enCsServiceStatus = TAF_SDC_GetCsServiceStatus();
    enPsServiceStatus = TAF_SDC_GetPsServiceStatus();

    if ((enCsServiceStatus != ulCsServiceStatus)
     || (enPsServiceStatus != ulPsServiceStatus))
    {
        if ((TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE == enCsServiceStatus)
         && (TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE == enPsServiceStatus))
        {
            NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_CS_PS_SERVICE, VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);
            ucLimitServiceSentFlg = VOS_FALSE;
        }
        else if (TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE == enCsServiceStatus)
        {
            /*��ǰ��CS����normal service����֮ǰCS��Ϊnormal service��PS֮ǰΪnormal service����Ҫ�ϱ�CS_SERVICE*/
            if ((TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE != ulCsServiceStatus)
             || (TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE == ulPsServiceStatus))
            {
                NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_CS_SERVICE, VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);
                ucLimitServiceSentFlg = VOS_FALSE;
            }
        }
        else if (TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE == enPsServiceStatus)
        {
            if ((TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE != ulPsServiceStatus)
             || (TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE == ulCsServiceStatus))
            {
                NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_PS_SERVICE, VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);
                ucLimitServiceSentFlg = VOS_FALSE;
            }
        }
        else
        {
            /*��ǰ����limit service����֮ǰCSΪnormal service��PS֮ǰΪnormal service����Ҫ�ϱ�LIMIT_SERVICE*/
            if (((TAF_SDC_SERVICE_STATUS_NO_SERVICE != enPsServiceStatus)
             || (TAF_SDC_SERVICE_STATUS_NO_SERVICE != enCsServiceStatus))
             && (VOS_FALSE == ucLimitServiceSentFlg))
            {
                NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_LIMITED_SERVICE, VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);
                ucLimitServiceSentFlg = VOS_TRUE;
            }
        }
    }

}

VOS_VOID TAF_MMA_ProcCsServiceStatusChanged(VOS_UINT8 ucOldCsServiceStatus)
{
    if(VOS_TRUE == TAF_MMA_IsCsServiceStatusChanged(ucOldCsServiceStatus))
    {
        TAF_MMA_SndStkLocationInfoInd();
    }
}



VOS_UINT8 TAF_MMA_IsCsServiceStatusChanged(VOS_UINT8 ucOldCsServiceStatus)
{
    if(ucOldCsServiceStatus != TAF_SDC_GetCsServiceStatus())
    {
        return VOS_TRUE;
    }
    else
    {
        return VOS_FALSE;
    }
}



VOS_UINT8 TAF_MMA_IsPsServiceStatusChanged(VOS_UINT8 ucOldPsServiceStatus)
{
    if(ucOldPsServiceStatus != TAF_SDC_GetPsServiceStatus())
    {
        return VOS_TRUE;
    }
    else
    {
        return VOS_FALSE;
    }
}


VOS_VOID Sta_UpdateServiceStatus(VOS_UINT32 ulServiceStatus, VOS_UINT32 ulCnDomainId)
{
    VOS_UINT32 ulPsServiceStatus;
    VOS_UINT32 ulCsServiceStatus;

    STA_TRACE1 (STA_INFO_PRINT,
        "Sta_UpdateServiceStatus():INFO:ulCnDomainId is: ", (VOS_INT32)ulCnDomainId);
    STA_TRACE1 (STA_INFO_PRINT,
        "Sta_UpdateServiceStatus():INFO:ulServiceStatus is: ", (VOS_INT32)ulServiceStatus);

    ulCsServiceStatus = TAF_SDC_GetCsServiceStatus();
    ulPsServiceStatus = TAF_SDC_GetPsServiceStatus();

    /* ����CS/PS����״̬ */
    switch (ulCnDomainId)
    {
        case MMA_MMC_SRVDOMAIN_CS:
            if(MMA_MMC_SERVICE_STATUS_NO_CHANGE != ulServiceStatus)
            {
                TAF_SDC_SetCsServiceStatus((VOS_UINT8)ulServiceStatus);
            }
            break;
        case MMA_MMC_SRVDOMAIN_PS:
            if(MMA_MMC_SERVICE_STATUS_NO_CHANGE != ulServiceStatus)
            {
                TAF_SDC_SetPsServiceStatus((VOS_UINT8)ulServiceStatus);
            }
            break;
        case MMA_MMC_SRVDOMAIN_NO_CHANGE:
        case MMA_MMC_SRVDOMAIN_CS_PS:
        case MMA_MMC_SRVDOMAIN_NOT_REG_PLMN_SEARCHING:
            /* �����ַ�����ֻ��MMC�Ż��ϱ������PS/CS����Ҫ���� */
            if(MMA_MMC_SERVICE_STATUS_NO_CHANGE != ulServiceStatus)
            {
                TAF_SDC_SetCsServiceStatus((VOS_UINT8)ulServiceStatus);
                TAF_SDC_SetPsServiceStatus((VOS_UINT8)ulServiceStatus);
            }
            break;
        default:
            break;
    }

    Sta_IccStatusReport(ulCsServiceStatus, ulPsServiceStatus);
    Sta_ReportServiceEvent(ulCsServiceStatus, ulPsServiceStatus);

    ulCsServiceStatus = TAF_SDC_GetCsServiceStatus();
    ulPsServiceStatus = TAF_SDC_GetPsServiceStatus();

    /* �����ܵķ���״̬ */
    if (  (MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE == ulCsServiceStatus)
       && (MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE == ulPsServiceStatus) )
    {
        TAF_SDC_SetServiceStatus(TAF_SDC_REPORT_SRVSTA_NORMAL_SERVICE);
        TAF_SDC_SetServiceDomain(TAF_SDC_SERVICE_DOMAIN_CS_PS);
    }
    else if (MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE == ulCsServiceStatus)
    {
        TAF_SDC_SetServiceStatus(TAF_SDC_REPORT_SRVSTA_NORMAL_SERVICE);
        TAF_SDC_SetServiceDomain(TAF_SDC_SERVICE_DOMAIN_CS);
    }
    else if (MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE == ulPsServiceStatus)
    {
        TAF_SDC_SetServiceStatus(TAF_SDC_REPORT_SRVSTA_NORMAL_SERVICE);
        TAF_SDC_SetServiceDomain(TAF_SDC_SERVICE_DOMAIN_PS);
    }
    else if ((MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE_REGION == ulCsServiceStatus)
          || (MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE_REGION == ulPsServiceStatus) )
    {
        TAF_SDC_SetServiceStatus(TAF_SDC_REPORT_SRVSTA_REGIONAL_LIMITED_SERVICE);
    }
    else if ((MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE == ulCsServiceStatus)
          || (MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE == ulPsServiceStatus) )
    {
        TAF_SDC_SetServiceStatus(TAF_SDC_REPORT_SRVSTA_LIMITED_SERVICE);
    }
    else if ((MMA_MMC_SERVICE_STATUS_NO_IMSI == ulCsServiceStatus)
          || (MMA_MMC_SERVICE_STATUS_NO_IMSI == ulPsServiceStatus) )
    {
        TAF_SDC_SetServiceStatus(TAF_SDC_REPORT_SRVSTA_LIMITED_SERVICE);
    }
    else
    {
        TAF_SDC_SetServiceStatus(TAF_SDC_REPORT_SRVSTA_NO_SERVICE);


        if (VOS_TRUE == gstMmaValue.pg_StatusContext->ulTimMaxFlg )
        {
            TAF_SDC_SetServiceStatus(TAF_SDC_REPORT_SRVSTA_DEEP_SLEEP);
        }

    }

    /* �����ܵķ����� */
    if (TAF_SDC_REPORT_SRVSTA_NORMAL_SERVICE != TAF_SDC_GetServiceStatus() )
    {
        if(MMA_MMC_SRVDOMAIN_NOT_REG_PLMN_SEARCHING == ulCnDomainId)
        {
            TAF_SDC_SetServiceDomain(TAF_SDC_SERVICE_DOMAIN_SEARCHING);
        }
        else
        {
            switch(gstMmaValue.stSetMsClass.NewMsClassType)
            {
                case TAF_PH_MS_CLASS_A:
                case TAF_PH_MS_CLASS_B:
                    TAF_SDC_SetServiceDomain(TAF_SDC_SERVICE_DOMAIN_CS_PS);
                    break;
                case TAF_PH_MS_CLASS_CC:
                    TAF_SDC_SetServiceDomain(TAF_SDC_SERVICE_DOMAIN_CS);
                    break;
                case TAF_PH_MS_CLASS_CG:
                    TAF_SDC_SetServiceDomain(TAF_SDC_SERVICE_DOMAIN_PS);
                    break;
                case TAF_PH_MS_CLASS_NULL:
                    TAF_SDC_SetServiceDomain(TAF_SDC_SERVICE_DOMAIN_NO_DOMAIN);
                    break;
                default:
                    TAF_SDC_SetServiceDomain(TAF_SDC_SERVICE_DOMAIN_NO_DOMAIN);
                    break;
            }
        }
    }
    else
    {
        g_StatusContext.ulTimMaxFlg = VOS_FALSE;
    }

    if ( TAF_SDC_REPORT_SRVSTA_NORMAL_SERVICE == TAF_SDC_GetServiceStatus() )
    {
        MN_PH_UpdateEndRegTime();
    }

}



VOS_VOID Sta_UpdateLocalState(
    MMC_MMA_SERVICE_STATUS_IND_STRU    *pServiceStatus,
    VOS_UINT8                           ucFsmSubOperate
)
{
    STA_PHONE_DATA_S                    Update;

    TAF_SDC_PLMN_ID_STRU               *pstPlmnId = VOS_NULL_PTR;

    pstPlmnId = TAF_SDC_GetCurrCampPlmnId();
    Update.ulCsServiceStatus = TAF_SDC_GetCsServiceStatus();
    Update.ulPsServiceStatus = TAF_SDC_GetPsServiceStatus();
    Update.ucDomainFlag      = MMA_MMC_SRVDOMAIN_CS_PS;
    Update.ulCsCause         = 0;
    Update.ulPsCause         = 0;

    Update.PlmnId.Mcc        = pstPlmnId->ulMcc;
    Update.PlmnId.Mnc        = pstPlmnId->ulMnc;
    Update.ucFsmSubOperate   = ucFsmSubOperate;
    Update.ulFsmState        = STA_FSM_ENABLE;
    Update.ucFsmStateSub     = STA_FSMSUB_NULL;
    Update.ucFsmUpdateFlag   = STA_UPDATE_MONO_CLEAR;

    if ( STA_ERROR == Sta_UpdateData (&Update) )
    {
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_UpdateLocalState():WARNING:Sta_UpdateData failed");
    }
}


VOS_UINT32 TAF_MMA_IsPsSrvStatusChanged(
    MMA_MMC_SRVDOMAIN_ENUM_UINT32       enCnDomainId,
    MMA_MMC_SERVICE_STATUS_ENUM_UINT32  enServiceStatus
)
{
    if ((MMA_MMC_SRVDOMAIN_PS    == enCnDomainId)
     || (MMA_MMC_SRVDOMAIN_CS_PS == enCnDomainId))
    {
        if (TAF_SDC_GetPsServiceStatus() != enServiceStatus)
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}



VOS_VOID TAF_MMA_ProcMmcServiceStatusInd(
    MMC_MMA_SERVICE_STATUS_IND_STRU    *pstServiceStatus
)
{
    VOS_UINT8                           ucOldCsSrvSta;
    VOS_UINT8                           ulOldSrvSta;
    
#if (FEATURE_IMS == FEATURE_ON)    
    if (VOS_TRUE  == TAF_SDC_GetImsSupportFlag())
    {
        /* PS����״̬�����仯֪ͨIMSA��SPMģ�� */       
        if (VOS_TRUE == TAF_MMA_IsPsSrvStatusChanged(pstServiceStatus->enCnDomainId, pstServiceStatus->enServiceStatus))
        {
            /* ֪ͨSPM����״̬���������SPM���������Ϣ����Ϣ�в���������ͷ���״̬����SPMȡSDCȫ�ֱ����е� */
            TAF_MMA_SndSpmServiceStatusChangeNotify();

            /* �޸�֪ͨIMSA��SERVICE CHANGE IND�߼�,�յ�����״̬�ط��仯��֪ͨ,�����յ�PSע����ɺ�֪ͨһ�� */
            if ((MMA_MMC_SRVDOMAIN_PS    == pstServiceStatus->enCnDomainId)
             || (MMA_MMC_SRVDOMAIN_CS_PS == pstServiceStatus->enCnDomainId))
            { 
                TAF_MMA_SndImsaSrvInfoNotify(pstServiceStatus->enServiceStatus);             
            }
        }
    }
#endif

    ucOldCsSrvSta   = TAF_SDC_GetCsServiceStatus();
    ulOldSrvSta     = TAF_SDC_GetServiceStatus();

    Sta_UpdateServiceStatus(pstServiceStatus->enServiceStatus, pstServiceStatus->enCnDomainId);

    TAF_MMA_ProcCsServiceStatusChanged(ucOldCsSrvSta);
    TAF_MMA_ProcLociStatusEvent();

    if (ulOldSrvSta != TAF_SDC_GetServiceStatus())
    {
        g_StatusContext.ucOperateType = STA_OP_PHONESERVICESTATUS;
    
        Sta_EventReport(g_StatusContext.ulCurOpId, TAF_PH_EVT_SERVICE_STATUS_IND);
    }

    return;
}



VOS_UINT32 Sta_ServiceStatusInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    STA_PHONE_DATA_S                    Update;         /* ��Ҫ���µ�ȫ������ */
    VOS_UINT32                          ulRet;
    TAF_SDC_PLMN_ID_STRU               *pstPlmnId = VOS_NULL_PTR;

#if ((FEATURE_MULTI_MODEM == FEATURE_ON) || (FEATURE_CL_INTERWORK == FEATURE_ON))
    VOS_UINT32                          ulOldUsimValidStatus;
    VOS_UINT32                          ulNewUsimValidStatus;
#endif

#if (FEATURE_CL_INTERWORK == FEATURE_ON)
    VOS_UINT8                           ulOldSrvSta;
#endif

    MMC_MMA_SERVICE_STATUS_IND_STRU    *pServiceStatus;

    pServiceStatus = (MMC_MMA_SERVICE_STATUS_IND_STRU*)pstMsg;

    pstPlmnId = TAF_SDC_GetCurrCampPlmnId();

#if (FEATURE_CL_INTERWORK == FEATURE_ON)
    ulOldSrvSta     = TAF_SDC_GetServiceStatus();
#endif

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    ulOldUsimValidStatus = TAF_SDC_IsUsimStausValid();
#endif

    /* ����SDC�е�SIM����CS/PS��ע��״̬ */
    TAF_SDC_SetSimCsRegStatus(pServiceStatus->ucSimCsRegStatus);
    TAF_SDC_SetSimPsRegStatus(pServiceStatus->ucSimPsRegStatus);

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    ulNewUsimValidStatus = TAF_SDC_IsUsimStausValid();

    /* ���cs ps��״̬�����ı���Ҫ֪ͨmtc */
    if (ulOldUsimValidStatus != ulNewUsimValidStatus)
    {
        TAF_MMA_SndMtcRegStatusInd((VOS_UINT8)ulNewUsimValidStatus);
    }
#endif

    /* ���±��صķ���״̬ */
    if (VOS_TRUE == pServiceStatus->bitOpSrvSta)
    {
        TAF_MMA_ProcMmcServiceStatusInd(pServiceStatus); 
    }

    if (VOS_TRUE == pServiceStatus->bitOpRegSta)
    {
        TAF_MMA_ReportRegStatus(pServiceStatus->enRegState, pServiceStatus->enCnDomainId);
    }

#if (FEATURE_CL_INTERWORK == FEATURE_ON)
    if ((ulOldSrvSta != TAF_SDC_GetServiceStatus())
     || (ulOldUsimValidStatus != ulNewUsimValidStatus))
    {
        if (VOS_TRUE == TAF_MMA_IsPowerOnCLInterWork())
        {
            TAF_MMA_SndCmmcaServiceInd();
        }
    }
#endif

    switch ( g_StatusContext.ulFsmState )
    {/* ϵͳ״̬ */
        case  STA_FSM_PLMN_SEL:

            Sta_UpdateLocalState(pServiceStatus, STA_FSMSUB_UPDATE);

            break;

        case  STA_FSM_ENABLE:

            /*ȡ��PS��CS��ķ�����Ϣ*/
            Update.ulCsServiceStatus = TAF_SDC_GetCsServiceStatus();
            Update.ulPsServiceStatus = TAF_SDC_GetPsServiceStatus();
            Update.ucDomainFlag      = (VOS_UINT8)pServiceStatus->enCnDomainId;
            Update.ulCsCause         = 0;
            Update.ulPsCause         = 0;

            Update.PlmnId.Mcc        = pstPlmnId->ulMcc;
            Update.PlmnId.Mnc        = pstPlmnId->ulMnc;


            Update.ucFsmSubOperate   = STA_FSMSUB_FINISH;
            Update.ulFsmState        = STA_FSM_ENABLE;
            Update.ucFsmStateSub     = STA_FSMSUB_NULL;
            Update.ucFsmUpdateFlag   = STA_UPDATE_DUAL_CLEAR;

            g_StatusContext.ucAttachReq = 0;
            g_StatusContext.ucDetachReq = 0;

            ulRet = Sta_UpdateData (&Update);
            if ( STA_ERROR == ulRet )
            {
                STA_TRACE (STA_WARNING_PRINT,
                    "Sta_ServiceStatusInd():WARNING:Sta_UpdateData failed");
            }

            break;

        default:
            
            break;
    }

    return VOS_TRUE;         /* �������� */
}


VOS_UINT32 Sta_DetachInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    STA_PHONE_DATA_S                    Update; /* ��Ҫ���µ�ȫ������ */
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulOldSrvSta;
    VOS_UINT8                           ucOldCsSrvSta; 

    MMC_MMA_DETACH_IND_STRU            *pDetachInd;

    pDetachInd = (MMC_MMA_DETACH_IND_STRU*)pstMsg;


    /* ENABLE״̬*/
    if ( g_StatusContext.ulFsmState & STA_FSM_ENABLE )
    {
        ulOldSrvSta = TAF_SDC_GetServiceStatus();

        ucOldCsSrvSta = TAF_SDC_GetCsServiceStatus();

        Sta_UpdateServiceStatus(pDetachInd->enServiceStatus, pDetachInd->enCnDomainId);

        TAF_MMA_ProcCsServiceStatusChanged(ucOldCsSrvSta);

        /*ȡ��PS��CS��ķ�����Ϣ*/
        Update.ulCsServiceStatus = TAF_SDC_GetCsServiceStatus();
        Update.ulPsServiceStatus = TAF_SDC_GetPsServiceStatus();
        Update.ulCsCause         = 0;
        Update.ulPsCause         = 0;
        Update.ucDomainFlag      = (VOS_UINT8)pDetachInd->enCnDomainId;

        Update.ucFsmSubOperate   = STA_FSMSUB_FINISH;
        Update.ulFsmState        = STA_FSM_ENABLE;
        Update.ucFsmStateSub     = STA_FSMSUB_NULL;
        Update.ucFsmUpdateFlag   = STA_UPDATE_DUAL_CLEAR;
        ulRet = Sta_UpdateData (&Update);
        if ( STA_ERROR == ulRet )
        {
            STA_TRACE (STA_WARNING_PRINT,
                "Sta_DetachInd():WARNING:Sta_UpdateData failed");
        }

        /* ��APP�ϱ��¼�*/
        g_StatusContext.ucOperateType = STA_OP_CSSERVICESTATUS
                                       |STA_OP_PSSERVICESTATUS;
        if (ulOldSrvSta != TAF_SDC_GetServiceStatus())
        {
            g_StatusContext.ucOperateType |= STA_OP_PHONESERVICESTATUS;
        }
        Sta_EventReport (g_StatusContext.ulCurOpId,
            TAF_PH_EVT_SERVICE_STATUS_IND);

        return VOS_TRUE;
    }


    /* û�������� */
    STA_TRACE (STA_WARNING_PRINT,
        "Sta_DetachInd():WARNING:invalid state, do nothing");

    return VOS_TRUE;
}
VOS_UINT32 Sta_ReOrderPlmnList (TAF_PLMN_LIST_STRU* pPlmnList)
{
    VOS_UINT32   ulRegPlmnIndx = TAF_MAX_PLMN_NUM;
    VOS_UINT32   ulSamePlmnIndx = TAF_MAX_PLMN_NUM;
    TAF_PH_RAT_ORDER_STRU              *pstRatOrder = VOS_NULL_PTR;
    VOS_UINT32   ulIndx = 0;

    TAF_PLMN_ID_STRU    stPlmnId;
    TAF_PLMN_STATUS_STRU    stStatus;
    PS_MEM_SET(&stPlmnId, 0, sizeof(stPlmnId));
    PS_MEM_SET(&stStatus, 0, sizeof(stStatus));

    pstRatOrder     = MN_MMA_GetRatPrioListAddr();

    /* �ϱ��б���С��2��ǰģʽ��˫ģ,��ֱ�ӷ��� */
    if( (pPlmnList->ucPlmnNum < 2 )
     || (pstRatOrder->ucRatOrderNum <= 1) )
    {
        return STA_SUCCESS;
    }
    /* ȷ����ǰ�Ƿ���ע���PLMN */
    for( ulIndx = 0; ulIndx < pPlmnList->ucPlmnNum; ++ulIndx)
    {
        if( TAF_PH_NETWORK_STATUS_CURRENT == pPlmnList->PlmnInfo[ulIndx].PlmnStatus)
        {
            ulRegPlmnIndx = ulIndx;
        }
    }
    if(TAF_MAX_PLMN_NUM == ulRegPlmnIndx)
    {
        return STA_ERROR; /* �ϱ��б���û��ע���PLMN ���ش���*/
    }
    stPlmnId.Mcc = pPlmnList->Plmn[ulRegPlmnIndx].Mcc;
    stPlmnId.Mnc = pPlmnList->Plmn[ulRegPlmnIndx].Mnc;
    /* ��ǰ��˫ģ����ע���PLMN,ȷ���б����Ƿ��������б����뵱ǰע���PLMN��ͬ */
    for( ulIndx = 0; ulIndx < pPlmnList->ucPlmnNum; ++ulIndx)
    {
        if( (stPlmnId.Mnc == pPlmnList->Plmn[ulIndx].Mnc)
            && (stPlmnId.Mcc == pPlmnList->Plmn[ulIndx].Mcc)
            && (ulIndx != ulRegPlmnIndx) )
        {
            ulSamePlmnIndx = ulIndx;
        }
    }
    if(TAF_MAX_PLMN_NUM == ulSamePlmnIndx)
    {
       return STA_SUCCESS; /* �ϱ��б���û����ע��PLMN��ͬ���б��� */
    }
    if(ulRegPlmnIndx < ulSamePlmnIndx)
    {
       return STA_SUCCESS; /* �ϱ��б��е�ע��PLMN��˳�������� ,����*/
    }
    /* ����ע��PLMN�ǰ�� ����*/
    PS_MEM_CPY(&stStatus, &(pPlmnList->PlmnInfo[ulSamePlmnIndx]), sizeof(TAF_PLMN_STATUS_STRU));
    PS_MEM_CPY(&(pPlmnList->PlmnInfo[ulSamePlmnIndx]), &(pPlmnList->PlmnInfo[ulRegPlmnIndx]), sizeof(TAF_PLMN_STATUS_STRU));
    PS_MEM_CPY(&(pPlmnList->PlmnInfo[ulRegPlmnIndx]), &stStatus, sizeof(TAF_PLMN_STATUS_STRU));
    return STA_SUCCESS; /* �ϱ��б��е�ע��PLMN��˳��������,����*/
}


VOS_VOID Sta_PlmnListEventReport (VOS_UINT32 ulOpID)
{
    TAF_PHONE_EVENT_PLMN_LIST_STRU         *pstPhoneEvent;
    VOS_UINT8                               n;          /* ѭ������ */
    VOS_UINT16                              ClientId;
    VOS_UINT8                               AppOpId;
    TAF_PHONE_EVENT                         ucPhoneEvent;
    TAF_PH_OPERATOR_NAME_FORMAT_OP_STRU     AtOperName;

    if ( STA_OP_PLMNLIST & g_StatusContext.ucOperateType )
    {
        pstPhoneEvent = (TAF_PHONE_EVENT_PLMN_LIST_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMA, sizeof(TAF_PHONE_EVENT_PLMN_LIST_STRU));

        if (VOS_NULL_PTR == pstPhoneEvent)
        {
            STA_TRACE(PS_PRINT_ERROR,"Sta_PlmnListEventReport:ERROR:ALLOC MEMORY FAIL.");
            return;
        }

        PS_MEM_SET(pstPhoneEvent, 0, sizeof(TAF_PHONE_EVENT_PLMN_LIST_STRU));

        pstPhoneEvent->PhoneEvent   = TAF_PH_EVT_PLMN_LIST_CNF;
        pstPhoneEvent->ulPlmnNum    = 0;
        for ( n = 0; n < g_StatusContext.PlmnList.ucPlmnNum; n++ )
        {
            if (n >= MAX_PLMN_NAME_LIST )
            {
                break;
            }

            pstPhoneEvent->astPlmnName[n].PlmnId.Mcc = g_StatusContext.PlmnList.Plmn[n].Mcc;
            pstPhoneEvent->astPlmnName[n].PlmnId.Mnc = g_StatusContext.PlmnList.Plmn[n].Mnc;
            MMA_PlmnId2Bcd(&(pstPhoneEvent->astPlmnName[n].PlmnId));
            pstPhoneEvent->astPlmnInfo[n].PlmnStatus = g_StatusContext.PlmnList.PlmnInfo[n].PlmnStatus;
            pstPhoneEvent->astPlmnInfo[n].RaMode     = g_StatusContext.PlmnList.PlmnInfo[n].RaMode;

            PS_MEM_SET(&AtOperName,0,sizeof(AtOperName));
            AtOperName.OperName.PlmnId.Mcc  = pstPhoneEvent->astPlmnName[n].PlmnId.Mcc;
            AtOperName.OperName.PlmnId.Mnc  = pstPhoneEvent->astPlmnName[n].PlmnId.Mnc;
            AtOperName.ListOp               = TAF_PH_OPER_NAME_PLMN2ALPHANUMERICNAME;

            /* ��ȡ��������ĳ����� */
            if (MMA_SUCCESS == Taf_PhoneGetNetworkNameForListPlmn(&AtOperName))
            {
                PS_MEM_CPY(pstPhoneEvent->astPlmnName[n].aucOperatorNameLong,AtOperName.OperName.aucOperatorNameLong,TAF_PH_OPER_NAME_LONG);
                PS_MEM_CPY(pstPhoneEvent->astPlmnName[n].aucOperatorNameShort,AtOperName.OperName.aucOperatorNameShort,TAF_PH_OPER_NAME_SHORT);
            }
            else
            {
                pstPhoneEvent->astPlmnName[n].aucOperatorNameLong[0]  = '\0';
                pstPhoneEvent->astPlmnName[n].aucOperatorNameShort[0] = '\0';
            }

            pstPhoneEvent->ulPlmnNum++;
        }

        /*APP����Ĺ������Ti��1*/
        if (ulOpID <= TAF_MAX_STATUS_TI)
        {
            /*ͨ��Ti��ȡClientId,CallId*/
            if (TAF_SUCCESS != MMA_GetIdByTi(TAF_MMA,(VOS_UINT8)ulOpID, &ClientId, &AppOpId, &ucPhoneEvent))
            {
                PS_MEM_FREE(WUEPS_PID_MMA, pstPhoneEvent);
                MMA_WARNINGLOG("Taf_PhoneEvent():WARNING:Invoke Taf_GetIdByTi failed");
                return;
            }
        }
        else
        {   /*��STATUS����������¼�*/
            AppOpId = MMA_OP_ID_INTERNAL;
            /*ClientId��Ϊ�㲥ֵ:���ֽڵĵ�4bit������MUX_PORT_BUTT*/
            ClientId = MMA_CLIENTID_BROADCAST;
        }

        pstPhoneEvent->ClientId  = ClientId;
        pstPhoneEvent->OpId      = AppOpId;

        /* ���õ绰�����ϱ����� */
        MN_PH_SendMsg(pstPhoneEvent->ClientId,(VOS_UINT8*)pstPhoneEvent,sizeof(TAF_PHONE_EVENT_PLMN_LIST_STRU));

        PS_MEM_FREE(WUEPS_PID_MMA, pstPhoneEvent);

    }

    return ;    /* �������� */
}


VOS_UINT32 Sta_PlmnListInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MMA_PLMN_LIST_CNF_STRU         *pPlmnListInd = VOS_NULL_PTR;
    VOS_UINT32                          ulRet;
    VOS_UINT8                           n;          /* ѭ������ */

    pPlmnListInd = (MMC_MMA_PLMN_LIST_CNF_STRU*)pstMsg;

    /* ϵͳ״̬ */
    switch ( g_StatusContext.ulFsmState )
    {
        case  STA_FSM_PLMN_LIST:
            /* �ر�״̬��ʱ�� */

            ulRet = NAS_StopRelTimer(WUEPS_PID_MMA,
                                     g_StatusContext.aFsmSub[0].TimerName,
                                     &g_StatusContext.ahStaTimer[0]);

            if ( VOS_OK != ulRet )
            {
                STA_TRACE (STA_WARNING_PRINT,
                    "Sta_PlmnListInd():WARNING:NAS_StopRelTimer failed");
            }
            g_StatusContext.ahStaTimer[0] = 0;

            /* ���������� */
            for ( n = 0; n < pPlmnListInd->ulCnt; n++ )
            {
                g_StatusContext.PlmnList.Plmn[n].Mcc
                    = pPlmnListInd->aPlmnList[n].ulMcc;
                g_StatusContext.PlmnList.Plmn[n].Mnc
                    = pPlmnListInd->aPlmnList[n].ulMnc;
                /* BEGIN: Added by liuyang, id:48197, 2005/10/16 */
                /*����ÿ��PLMN��״̬�ͽ���ģʽ*/
                g_StatusContext.PlmnList.PlmnInfo[n].PlmnStatus
                    = pPlmnListInd->aucPlmnStatus[n];
                g_StatusContext.PlmnList.PlmnInfo[n].RaMode
                    = pPlmnListInd->aucRaMode[n];
                /* END:   Added by liuyang, id:48197, 2005/10/16 */
            }
            g_StatusContext.PlmnList.ucPlmnNum = (VOS_UINT8)pPlmnListInd->ulCnt;

            /*���ϱ���PLMN�б���е��� */
            Sta_ReOrderPlmnList(&g_StatusContext.PlmnList);
            /* ��APP�ϱ��¼�*/
            g_StatusContext.ucOperateType = STA_OP_PLMNLIST;

            /* �б��ѻظ����ͨ��ר�е��¼��ظ� */
            Sta_PlmnListEventReport(g_StatusContext.ulCurOpId);
            break;

        case  STA_FSM_PLMN_RESEL:
            /* �����ֶ���ѡ��״̬ */
            if ( STA_FSMSUB_PLMN_RESEL_MANUAL
                != g_StatusContext.aFsmSub[0].ucFsmStateSub )
            {
                /* ������ٵ�����Ϣ */
                STA_TRACE (STA_WARNING_PRINT,
                    "Sta_PlmnListInd():WARNING:is not manual PLMN_RESEL");

                return VOS_TRUE;
            }
            /* �ر�״̬��ʱ�� */
            ulRet = NAS_StopRelTimer(WUEPS_PID_MMA,
                                     g_StatusContext.aFsmSub[0].TimerName,
                                     &g_StatusContext.ahStaTimer[0]);


            if ( VOS_OK != ulRet )
            {
                STA_TRACE (STA_WARNING_PRINT,
                    "Sta_PlmnListInd():WARNING:NAS_StopRelTimer failed");
            }
            g_StatusContext.ahStaTimer[0] = 0;
            /* BEGIN: Added by liuyang, 2005/10/26 */
            /* ���������� */
            for ( n = 0; n < pPlmnListInd->ulCnt; n++ )
            {
                g_StatusContext.PlmnList.Plmn[n].Mcc
                    = pPlmnListInd->aPlmnList[n].ulMcc;
                g_StatusContext.PlmnList.Plmn[n].Mnc
                    = pPlmnListInd->aPlmnList[n].ulMnc;
                /* BEGIN: Added by liuyang, id:48197, 2005/10/16 */
                /*����ÿ��PLMN��״̬�ͽ���ģʽ*/
                g_StatusContext.PlmnList.PlmnInfo[n].PlmnStatus
                    = pPlmnListInd->aucPlmnStatus[n];
                g_StatusContext.PlmnList.PlmnInfo[n].RaMode
                    = pPlmnListInd->aucRaMode[n];
                /* END:   Added by liuyang, id:48197, 2005/10/16 */
            }
            g_StatusContext.PlmnList.ucPlmnNum = (VOS_UINT8)pPlmnListInd->ulCnt;
            /* END:   Added by liuyang, 2005/10/26 */
            /* BEGIN: Modified by liuyang, 2005/10/26 */
            /*Ӧ���ϱ�plmn�б�*/
            /* ��APP�ϱ��¼�*/
            /*g_StatusContext.ucOperateType = STA_OP_CSSERVICESTATUS
                                           |STA_OP_PSSERVICESTATUS
                                           |STA_OP_PLMNRESELMODE;*/
            g_StatusContext.ucOperateType = STA_OP_PLMNLIST
                                           |STA_OP_PLMNRESELMODE;
            /* END:   Modified by liuyang, 2005/10/26 */

            /* BEGIN: Added by liuyang id:48197, 2006/3/27   PN:A32D02372*/


            Sta_EventReport (g_StatusContext.ulCurOpId,
                             TAF_PH_EVT_PLMN_RESEL_CNF);    /*<==A32D12559*/

            if ( (MMA_PLMN_MCC_NONE != g_StatusContext.StaPlmnId.Mcc)
               &&(MMA_PLMN_MNC_NONE != g_StatusContext.StaPlmnId.Mnc)
               )
            {
                /*����ע��״̬,�ȴ��û�����ָ��PLMN����*/
                ulRet = Sta_UpdateFsmFlag(STA_FSMSUB_NULL, STA_FSM_ENABLE, STA_UPDATE_MONO_CLEAR);
                if (STA_ERROR == ulRet)
                {
                    STA_TRACE(STA_WARNING_PRINT, "Sta_PlmnListInd():WARNING:Sta_UpdateFsmFlag failed\n");
                }
                /*����ע���ڲ�Rplmn����*/
                Sta_PlmnSelect(STA_OPID_INSIDE,
                               g_StatusContext.StaPlmnId,
                               TAF_PH_RA_MODE_DEFAULT,  /*<==A32D12536*/
                               TAF_PLMN_RESELETION_MANUAL);
            }
            else
            {
                /*
                Sta_EventReport (g_StatusContext.ulCurOpId,
                                 TAF_PH_EVT_PLMN_RESEL_CNF);
                */
                /*����ע��״̬,�ȴ��û�����ָ��PLMN����*/
                ulRet = Sta_UpdateFsmFlag(STA_FSMSUB_NULL, STA_FSM_ENABLE, STA_UPDATE_MONO_CLEAR);
                if (STA_ERROR == ulRet)
                {
                    STA_TRACE(STA_WARNING_PRINT, "Sta_PlmnListInd():WARNING:Sta_UpdateFsmFlag failed\n");
                }
            }

            return VOS_TRUE;
        case STA_FSM_ENABLE:
            /*ֱ���ϱ��б�*/
            for ( n = 0; n < pPlmnListInd->ulCnt; n++ )
            {
                g_StatusContext.PlmnList.Plmn[n].Mcc
                    = pPlmnListInd->aPlmnList[n].ulMcc;
                g_StatusContext.PlmnList.Plmn[n].Mnc
                    = pPlmnListInd->aPlmnList[n].ulMnc;
                /* BEGIN: Added by liuyang, id:48197, 2005/10/16 */
                /*����ÿ��PLMN��״̬�ͽ���ģʽ*/
                g_StatusContext.PlmnList.PlmnInfo[n].PlmnStatus
                    = pPlmnListInd->aucPlmnStatus[n];
                g_StatusContext.PlmnList.PlmnInfo[n].RaMode
                    = pPlmnListInd->aucRaMode[n];
                /* END:   Added by liuyang, id:48197, 2005/10/16 */
            }
            g_StatusContext.PlmnList.ucPlmnNum = (VOS_UINT8)pPlmnListInd->ulCnt;
            /* END:   Added by liuyang, 2005/10/26 */
            /* BEGIN: Modified by liuyang, 2005/10/26 */
            /*Ӧ���ϱ�plmn�б�*/
            g_StatusContext.ucOperateType = STA_OP_PLMNLIST
                                           |STA_OP_PLMNRESELMODE;
            Sta_EventReport (g_StatusContext.ulCurOpId,
                             TAF_PH_EVT_PLMN_LIST_IND);

            return VOS_TRUE;
        /* END:   Added by liuyang id:48197, 2005/12/14 */
        default :
            /* do nothing */
            STA_TRACE (STA_WARNING_PRINT,
                "Sta_PlmnListInd():WARNING:invalid state, do nothing");

            return VOS_TRUE;
    }

    /*�����״̬*/
    ulRet = Sta_UpdateFsmFlag(STA_FSMSUB_NULL, STA_FSM_ENABLE, STA_UPDATE_MONO_CLEAR);
    if (STA_ERROR == ulRet)
    {
        STA_TRACE(STA_WARNING_PRINT, "Sta_PlmnListInd():WARNING:Sta_UpdateFsmFlag failed\n");
    }
    return VOS_TRUE;
}
VOS_UINT32 Sta_CoverageAreaInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    STA_PHONE_DATA_S                                        Update;         /* ��Ҫ���µ�ȫ������ */
    VOS_UINT32                                              ulRet;
    MMC_MMA_COVERAGE_AREA_IND_STRU                         *pCoverageAreaInd;

    TAF_SDC_SYS_MODE_ENUM_UINT8                             ucSysMode;
    TAF_SDC_SYS_SUBMODE_ENUM_UINT8                          ucSysSubmode;

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    MMA_MTC_CURR_CAMP_PLMN_INFO_IND_STRU                    stSndMtcPlmnInfo;
#endif
    
    pCoverageAreaInd = (MMC_MMA_COVERAGE_AREA_IND_STRU*)pstMsg;

    g_StatusContext.ulTimMaxFlg = pCoverageAreaInd->ulTimMaxFlg;

    if (MMA_MMC_COVERAGE_AREA_CHANGE_MODE_LOST == pCoverageAreaInd->enCoverageChgMode)
    {
        TAF_SDC_SetCampOnFlag(VOS_FALSE);
        
        TAF_MMA_SndMtaAreaLostInd();
        
#if (FEATURE_MULTI_MODEM == FEATURE_ON)
        if (VOS_TRUE == TAF_SDC_GetUtranSkipWPlmnSearchFlag())
        {
            PS_MEM_SET(&stSndMtcPlmnInfo, 0, sizeof(stSndMtcPlmnInfo));
            stSndMtcPlmnInfo.enRatMode             = MTC_RATMODE_BUTT;
            stSndMtcPlmnInfo.ucIsForbiddenPlmnFlag = VOS_FALSE;
            stSndMtcPlmnInfo.ucPsAttachAllowFlag   = TAF_SDC_GetPsAttachAllowFlg();
            stSndMtcPlmnInfo.stPlmnId.ulMcc        = TAF_SDC_INVALID_MCC;
            stSndMtcPlmnInfo.stPlmnId.ulMnc        = TAF_SDC_INVALID_MCC;
            TAF_MMA_SndMtcCurrCampPlmnInfoInd(&stSndMtcPlmnInfo);
        }
#endif
        
        ucSysMode = TAF_SDC_GetSysMode();
        ucSysSubmode = TAF_SDC_GetSysSubMode();
        if ((TAF_SDC_SYS_MODE_BUTT    != ucSysMode)
         || (TAF_SDC_SYS_SUBMODE_NONE != ucSysSubmode)){
            Mma_ModeChgReport(TAF_SDC_SYS_MODE_BUTT, TAF_SYS_SUBMODE_NONE);
            TAF_SDC_SetSysMode(TAF_SDC_SYS_MODE_BUTT);
            TAF_SDC_SetSysSubMode(TAF_SYS_SUBMODE_NONE); 
        }
    }

    switch ( g_StatusContext.ulFsmState )
    {
        case  STA_FSM_ENABLE :
            /*����������ʱ��MMC������MMA����service status ind��ָʾCS/PS����״̬
              �Լ�PLMNID�����ɾ���������*/
            return VOS_TRUE;

        case  STA_FSM_DE_ATTACH:
            /* ����Ϊ��������*/
            /* Detach�����У�����������ʱ��MMC������MMA����detach cnf��service status ind��ָʾCS/PS����״̬
              �Լ�PLMNID�����ɾ��������� */
            return VOS_TRUE;

        case  STA_FSM_PLMN_RESEL: /* PLMN_RESEL״̬ */
            /* ��״̬Ϊ�Զ���ѡģʽ*/
            if ( STA_FSMSUB_PLMN_RESEL_AUTO
                == g_StatusContext.aFsmSub[0].ucFsmStateSub )
            {
                /* ����Ϊ���븲����*/
                if ( MMA_MMC_COVERAGE_AREA_CHANGE_MODE_ENTER == pCoverageAreaInd->enCoverageChgMode )
                { /* PLMNפ���� */
                    /* do nothing */
                    STA_TRACE (STA_NORMAL_PRINT,
                                "Sta_CoverageAreaInd():NORMAL:resident PLMN indicator,\
                                do nothing");
                    return VOS_TRUE;
                }
                else /* ��������*/
                {
                    /* �ر�PLMN_RESEL״̬��ʱ��*/
                    if ( 0 != g_StatusContext.ahStaTimer[0] )
                    {
                        ulRet = NAS_StopRelTimer(WUEPS_PID_MMA,
                                                 g_StatusContext.aFsmSub[0].TimerName,
                                                 &g_StatusContext.ahStaTimer[0]);


                        if ( VOS_OK != ulRet )
                        {
                            STA_TRACE (STA_WARNING_PRINT,
                                        "Sta_CoverageAreaInd():WARNING:\
                                        NAS_StopRelTimer failed");
                        }
                        g_StatusContext.ahStaTimer[0] = 0;
                    }

                    /* ���������� */
                    Update.ulCsServiceStatus = pCoverageAreaInd->enCsSvcSta;
                    Update.ulPsServiceStatus = pCoverageAreaInd->enPsSvcSta;

                    Update.ulCsCause         = MMA_MMC_CAUSE_NULL;
                    Update.ulPsCause         = MMA_MMC_CAUSE_NULL;

                    Update.ucFsmSubOperate   = STA_FSMSUB_FINISH;
                    Update.ulFsmState        = STA_FSM_ENABLE;
                    Update.ucFsmStateSub     = STA_FSMSUB_NULL;
                    Update.ucFsmUpdateFlag   = STA_UPDATE_MONO_CLEAR;
                    ulRet = Sta_UpdateData (&Update);
                    if ( STA_ERROR == ulRet )
                    {
                        STA_TRACE (STA_WARNING_PRINT,
                                    "Sta_CoverageAreaInd():WARNING:\
                                    Sta_UpdateData failed");
                    }

                    /* ��APP�ϱ��¼�*/
                    g_StatusContext.ucOperateType = STA_OP_CSSERVICESTATUS
                        |STA_OP_PSSERVICESTATUS|STA_OP_PLMNRESELMODE;
                    Sta_EventReport (g_StatusContext.ulCurOpId,
                        TAF_PH_EVT_PLMN_RESEL_CNF);
                }
                return VOS_TRUE;
            }/* ���� ��״̬Ϊ�Զ���ѡģʽ */

            /* ��״̬Ϊ�ֶ���ѡģʽ */
            if ( STA_FSMSUB_PLMN_RESEL_MANUAL
                == g_StatusContext.aFsmSub[0].ucFsmStateSub )
            {
                /* ����Ϊ��������*/
                if ( MMA_MMC_COVERAGE_AREA_CHANGE_MODE_LOST == pCoverageAreaInd->enCoverageChgMode )
                {
                    /* �ر�PLMN_RESEL״̬��ʱ��*/
                    if ( 0 != g_StatusContext.ahStaTimer[0] )
                    {
                        ulRet = NAS_StopRelTimer(WUEPS_PID_MMA,
                                                 g_StatusContext.aFsmSub[0].TimerName,
                                                 &g_StatusContext.ahStaTimer[0]);


                        if ( VOS_OK != ulRet )
                        {
                            STA_TRACE (STA_WARNING_PRINT,
                                "Sta_CoverageAreaInd():WARNING:\
                                NAS_StopRelTimer failed  ");
                        }
                        g_StatusContext.ahStaTimer[0] = 0;
                    }

                    /* ���������� */
                    Update.ulCsServiceStatus = pCoverageAreaInd->enCsSvcSta;
                    Update.ulPsServiceStatus = pCoverageAreaInd->enPsSvcSta;

                    Update.ulCsCause         = MMA_MMC_CAUSE_NULL;
                    Update.ulPsCause         = MMA_MMC_CAUSE_NULL;

                    Update.ucFsmSubOperate   = STA_FSMSUB_FINISH;
                    Update.ulFsmState        = STA_FSM_ENABLE;
                    Update.ucFsmStateSub     = STA_FSMSUB_NULL;
                    Update.ucFsmUpdateFlag   = STA_UPDATE_MONO_CLEAR;
                    ulRet = Sta_UpdateData (&Update);
                    if ( STA_ERROR == ulRet )
                    {
                        STA_TRACE (STA_WARNING_PRINT,
                            "Sta_CoverageAreaInd():WARNING:Sta_UpdateData failed  ");
                    }

                    /* ��APP�ϱ��¼�*/
                    g_StatusContext.ucOperateType = STA_OP_CSSERVICESTATUS
                        |STA_OP_PSSERVICESTATUS|STA_OP_PLMNRESELMODE;
                    Sta_EventReport (g_StatusContext.ulCurOpId,
                        TAF_PH_EVT_PLMN_RESEL_CNF);
                }
                return VOS_TRUE;
            }
            return VOS_TRUE;

        case  STA_FSM_PLMN_SEL:  /* PLMN_SEL״̬ */
            /* ����Ϊ���븲����*/
            /* ɾ��֮ǰ������ı�������,Ŀǰ�û�ָ������ʱ,MMC�����ϱ�ָ���������
               ����˴����б��� */
            return VOS_TRUE;

        default:
            STA_TRACE (STA_WARNING_PRINT,
                "Sta_CoverageAreaInd():WARNING:invalid state, do nothing  ");
            return VOS_TRUE;
    }
}


VOS_VOID NAS_MN_ReportPlmnSrchResult(
    VOS_UINT32                  ulResult
)
{
    if ( MMA_MMC_USER_PLMN_SEARCH_RESULT_SUCCESS == ulResult )
    {
        /* �ϱ���ǰָ�������ɹ� */
        g_StatusContext.PhoneError    = TAF_ERR_NO_ERROR;
        if (STA_FSM_PLMN_RESEL == g_StatusContext.ulFsmState)
        {
            g_StatusContext.ucOperateType = STA_OP_PLMNRESELMODE;
            /* �ϱ�APP��ǰ״̬�¼� */
            Sta_EventReport (g_StatusContext.ulCurOpId, TAF_PH_EVT_PLMN_RESEL_CNF);
            STA_TRACE(STA_NORMAL_PRINT, "NAS_MN_ReportPlmnSrchResult: TAF_PH_EVT_PLMN_RESEL_CNF reported to AT!");
        }
        else if(STA_FSM_PLMN_SEL == g_StatusContext.ulFsmState)
        {
            /* �����ϱ��¼����ͣ����ϱ�APP�����ɹ� */
            g_StatusContext.ucOperateType = STA_OP_REGPLMN;
            Sta_EventReport (g_StatusContext.ulCurOpId, TAF_PH_EVT_PLMN_SEL_CNF);
        }
        else
        {
        }
    }
    else
    {
        /* �ϱ���ǰָ������ʧ�� */
        g_StatusContext.PhoneError    = TAF_ERR_NO_NETWORK_SERVICE;

        g_StatusContext.ucOperateType = STA_OP_PHONEERROR;
        MMA_NORMAILLOG("NAS_MN_ReportPlmnSrchResult():NORMAIL:SPECIALIZE THE PLMN FAILURE!");
        /* �ϱ�APP��ǰ״̬�¼� */
        if (STA_FSM_PLMN_RESEL == g_StatusContext.ulFsmState)
        {
            /* �ϱ�APP��ǰ״̬�¼� */
            Sta_EventReport (g_StatusContext.ulCurOpId, TAF_PH_EVT_PLMN_RESEL_CNF);
            STA_TRACE(STA_NORMAL_PRINT, "NAS_MN_ReportPlmnSrchResult: TAF_PH_EVT_PLMN_RESEL_CNF reported to AT!");
        }
        else if(STA_FSM_PLMN_SEL == g_StatusContext.ulFsmState)
        {
            /* �ϱ�APP��ǰ״̬�¼� */
            Sta_EventReport (g_StatusContext.ulCurOpId, TAF_PH_EVT_PLMN_SEL_CNF);
            STA_TRACE(STA_NORMAL_PRINT, "NAS_MN_ReportPlmnSrchResult: TAF_PH_EVT_PLMN_SEL_CNF reported to AT!");
        }
        else
        {
        }
    }
}
VOS_UINT32 Sta_PlmnListRej(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32          ulRet;

    /* ����PLMN_LIST״̬ */
    if ( !(g_StatusContext.ulFsmState & STA_FSM_PLMN_LIST) )
    {
        g_StatusContext.PhoneError = TAF_ERR_PHONE_MSG_UNMATCH;

        /* ������ٵ�����Ϣ */
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_PlmnListInd():WARNING:unmatched system FSM state\n" );

        return VOS_TRUE;
    }

    /* �ر�״̬��ʱ��*/

    ulRet = NAS_StopRelTimer(WUEPS_PID_MMA,
                             g_StatusContext.aFsmSub[0].TimerName,
                             &g_StatusContext.ahStaTimer[0]);


    if ( VOS_OK != ulRet )
    {
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_PlmnListRej():WARNING:NAS_StopRelTimer failed  ");
    }
    g_StatusContext.ahStaTimer[0]    = 0;
    /* ���б�־ */
    g_StatusContext.aucFsmSubFlag[0] = STA_FSMSUB_NULL;

    /*�����״̬*/
    ulRet = Sta_UpdateFsmFlag(STA_FSMSUB_NULL, STA_FSM_ENABLE, STA_UPDATE_MONO_CLEAR);
    if (STA_ERROR == ulRet)
    {
        STA_TRACE(STA_WARNING_PRINT, "Sta_PlmnListRej():WARNING:Sta_UpdateFsmFlag failed\n");
    }

    /* Э��ջ�ܾ�PLMN����*/
    /* ��APP�ϱ��¼�*/
    Sta_EventReport (g_StatusContext.ulCurOpId,
        TAF_PH_EVT_PLMN_LIST_REJ);

    return VOS_TRUE;
}

VOS_UINT32 Sta_PlmnListAortCnf (MMC_MMA_PLMN_LIST_ABORT_CNF_STRU *pPlmnListAbortCnf)
{
    VOS_UINT16                          ClientId;
    VOS_UINT8                           AppOpId;
    TAF_PHONE_EVENT                     PhoneEvent;

    VOS_UINT32                          ulIndex;

    VOS_UINT32                          ulRet;

    /* stop cnf��search cnf�Գ�ʱ�п��ܴ��������б�Ϊ�գ���Ҫ�ϱ�*/
    if (pPlmnListAbortCnf->ulCnt > 0)
    {
        for ( ulIndex = 0; ulIndex < pPlmnListAbortCnf->ulCnt; ulIndex++ )
        {
            g_StatusContext.PlmnList.Plmn[ulIndex].Mcc
                = pPlmnListAbortCnf->aPlmnList[ulIndex].ulMcc;
            g_StatusContext.PlmnList.Plmn[ulIndex].Mnc
                = pPlmnListAbortCnf->aPlmnList[ulIndex].ulMnc;

            /*����ÿ��PLMN��״̬�ͽ���ģʽ*/
            g_StatusContext.PlmnList.PlmnInfo[ulIndex].PlmnStatus
                = pPlmnListAbortCnf->aucPlmnStatus[ulIndex];
            g_StatusContext.PlmnList.PlmnInfo[ulIndex].RaMode
                = pPlmnListAbortCnf->aucRaMode[ulIndex];

        }
        g_StatusContext.PlmnList.ucPlmnNum = (VOS_UINT8)pPlmnListAbortCnf->ulCnt;

        /*���ϱ���PLMN�б���е��� */
        Sta_ReOrderPlmnList(&g_StatusContext.PlmnList);

        /* ��APP�ϱ��¼�*/
        g_StatusContext.ucOperateType = STA_OP_PLMNLIST;

        /* �б��ѻظ����ͨ��ר�е��¼��ظ� */
        Sta_PlmnListEventReport(g_StatusContext.ulCurOpId);
    }
    else
    {
        /* ��AT�ظ��б��ѳ�ʱ��� */
        g_StatusContext.PhoneError    = TAF_ERR_TIME_OUT;
        g_StatusContext.ucOperateType = STA_OP_PHONEERROR;

        if (g_StatusContext.ulCurOpId <= TAF_MAX_STATUS_TI)
        {
            /*ͨ��Ti��ȡClientId,CallId*/
            if (TAF_SUCCESS != MMA_GetIdByTi(TAF_MMA,(VOS_UINT8)g_StatusContext.ulCurOpId, &ClientId, &AppOpId, &PhoneEvent))
            {
                MMA_WARNINGLOG("Taf_PhoneEvent():WARNING:Invoke Taf_GetIdByTi failed");
                return STA_ERROR;
            }
        }
        else
        {   /*��STATUS����������¼�*/
            AppOpId = MMA_OP_ID_INTERNAL;
            /*ClientId��Ϊ�㲥ֵ:���ֽڵĵ�4bit������MUX_PORT_BUTT*/
            ClientId = MMA_CLIENTID_BROADCAST;
        }

        /* �ϱ�APP��ǰ״̬�¼� */
        MMA_HandleEventError(ClientId, AppOpId, TAF_ERR_TIME_OUT, TAF_PH_EVT_ERR);
    }


    /* ����״̬��־ */
    ulRet = Sta_UpdateFsmFlag (STA_FSMSUB_NULL, STA_FSM_ENABLE, STA_UPDATE_MONO_CLEAR);

    if (STA_ERROR == ulRet)
    {
        STA_TRACE(STA_WARNING_PRINT, "Sta_PlmnListAortCnf():WARNING:Sta_UpdateFsmFlag failed\n");
    }

    /* ���õ�ǰû�д����û��б��Ѵ�Ϲ��� */
    g_StatusContext.ucPlmnListAbortProc = TAF_MMA_PLMN_LIST_ABORT_BUTT;

    return STA_SUCCESS;
}



VOS_UINT32 TAF_MMA_PlmnListAbortCnfUserAbort (MMC_MMA_PLMN_LIST_ABORT_CNF_STRU *pPlmnListAbortCnf)
{

    VOS_UINT32                      ulRet;

    g_StatusContext.ucOperateType = STA_OP_NULL;

    /* ��AT�ظ��б��Ѵ�� */
    Sta_EventReport (g_StatusContext.ulCurOpId,
            TAF_PH_EVT_PLMN_LIST_ABORT_CNF);

    /* ����״̬��־ */
    ulRet = Sta_UpdateFsmFlag (STA_FSMSUB_NULL, STA_FSM_ENABLE, STA_UPDATE_MONO_CLEAR);

    if (STA_ERROR == ulRet)
    {
        STA_TRACE(STA_WARNING_PRINT, "TAF_MMA_PlmnListAbortCnfUserAbort():WARNING:Sta_UpdateFsmFlag failed\n");
    }

    /* ���õ�ǰû�д����û��б��Ѵ�Ϲ��� */
    g_StatusContext.ucPlmnListAbortProc = TAF_MMA_PLMN_LIST_ABORT_BUTT;

    return STA_SUCCESS;
}
VOS_UINT32 TAF_MMA_RcvPlmnListAbortCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MMA_PLMN_LIST_ABORT_CNF_STRU   *pPlmnListAbortCnf;

    pPlmnListAbortCnf = (MMC_MMA_PLMN_LIST_ABORT_CNF_STRU*)pstMsg;

    /* ��ǰ����PLMN LIST״̬��ֱ�ӷ��� */
    if (STA_FSM_PLMN_LIST != g_StatusContext.ulFsmState)
    {
        STA_TRACE(STA_WARNING_PRINT, "TAF_MMA_RcvPlmnListAbortCnf():WARNING:Sta_UpdateFsmFlag failed\n");
        return VOS_TRUE;
    }

    /* �쳣��������ǰû�����ڴ����û��б��Ѵ�ϣ�ֱ�ӷ��� */
    if (TAF_MMA_PLMN_LIST_ABORT_BUTT == g_StatusContext.ucPlmnListAbortProc)
    {
        STA_TRACE(STA_WARNING_PRINT, "TAF_MMA_RcvPlmnListAbortCnf():WARNING:Sta_UpdateFsmFlag failed\n");
        return VOS_TRUE;
    }

    /* ͣ��ʱ�� */
    if (VOS_OK != NAS_StopRelTimer(WUEPS_PID_MMA,
                     STA_TIMER_PLMN_LIST_ABORT,
                     &g_StatusContext.ahStaTimer[0]))
    {
        STA_TRACE(STA_WARNING_PRINT, "TAF_MMA_RcvPlmnListAbortCnf():WARNING:NAS_StopRelTimer failed!");
    }

    g_StatusContext.ahStaTimer[0] = VOS_NULL_PTR;

    /* ��ʱ����ʱ���µ�PLMN LIST ABORT�Ĵ��� */
    if ( TAF_MMA_PLMN_LIST_ABORT_PROCESSING_TIMEOUT == g_StatusContext.ucPlmnListAbortProc)
    {
        Sta_PlmnListAortCnf(pPlmnListAbortCnf);
    }
    else
    {
        TAF_MMA_PlmnListAbortCnfUserAbort(pPlmnListAbortCnf);
    }

    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvSpecPlmnSearchAbortCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (VOS_OK != NAS_StopRelTimer(WUEPS_PID_MMA,
                     STA_TIMER_SPEC_PLMN_ABORT,
                     &g_StatusContext.ahStaTimer[0]))
    {
        STA_TRACE(STA_WARNING_PRINT, "TAF_MMA_RcvSpecPlmnSearchAortCnf():WARNING:NAS_StopRelTimer failed!");
    }
    /* ��AT�ظ�ָ���ѳ�ʱ��� */
    g_StatusContext.PhoneError    = TAF_ERR_TIME_OUT;
    g_StatusContext.ucOperateType = STA_OP_PHONEERROR;

    /* �ϱ�APP��ǰ״̬�¼� */
    Sta_EventReport (g_StatusContext.ulCurOpId,
        TAF_PH_EVT_PLMN_SEL_CNF);

    /* Ǩ��״̬��ENABLE״̬ */
    g_StatusContext.ulFsmState = STA_FSM_ENABLE;

    return VOS_TRUE;
}

/* ����Sta_MsgIndɾ�� */



TAF_SDC_REPORT_SRVSTA_ENUM_UINT8 TAF_MMA_ConvertTafSrvStaToStkType(
                                    TAF_SDC_REPORT_SRVSTA_ENUM_UINT8  enTafSrvType
                                    )
{
    TAF_SDC_REPORT_SRVSTA_ENUM_UINT8 enRtType;

    enRtType = enTafSrvType;

    if(TAF_SDC_REPORT_SRVSTA_REGIONAL_LIMITED_SERVICE == enTafSrvType)
    {
        enRtType = TAF_SDC_REPORT_SRVSTA_LIMITED_SERVICE;
    }

    if(TAF_SDC_REPORT_SRVSTA_DEEP_SLEEP == enTafSrvType)
    {
        enRtType = TAF_SDC_REPORT_SRVSTA_NO_SERVICE;
    }

    return enRtType;
}



TAF_SDC_SERVICE_STATUS_ENUM_UINT8 TAF_MMA_ConvertSrvTypeForStk(
                                    TAF_SDC_SERVICE_STATUS_ENUM_UINT8  enTafSrvType
                                    )
{
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8 enRtType;

    enRtType = enTafSrvType;

    if (TAF_SDC_SERVICE_STATUS_LIMITED_SERVICE_REGION == enTafSrvType
     || TAF_SDC_SERVICE_STATUS_NO_IMSI == enTafSrvType)
    {
        enRtType = TAF_SDC_SERVICE_STATUS_LIMITED_SERVICE;
    }

    if (TAF_SDC_SERVICE_STATUS_DEEP_SLEEP == enTafSrvType)
    {
        enRtType = TAF_SDC_SERVICE_STATUS_NO_SERVICE;
    }

    return enRtType;
}




TAF_SDC_REPORT_SRVSTA_ENUM_UINT8 TAF_MMA_GetSrvTypeForStk(VOS_VOID)
{
    TAF_SDC_REPORT_SRVSTA_ENUM_UINT8    enRtType;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enCsServiceStatus;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enPsServiceStatus;
    MN_MMA_LAST_SETTED_SYSCFG_SET_STRU *pstSysCfg;

    pstSysCfg         = MN_MMA_GetLastSyscfgSetAddr();

    enCsServiceStatus = TAF_MMA_ConvertSrvTypeForStk(TAF_SDC_GetCsServiceStatus());
    enPsServiceStatus = TAF_MMA_ConvertSrvTypeForStk(TAF_SDC_GetPsServiceStatus());

    if ((TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE == enCsServiceStatus)
     || (TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE == enPsServiceStatus))
    {
        enRtType = TAF_SDC_REPORT_SRVSTA_NORMAL_SERVICE;
    }
    else if ((TAF_SDC_SERVICE_STATUS_NO_SERVICE == enCsServiceStatus)
          || (TAF_SDC_SERVICE_STATUS_NO_SERVICE == enPsServiceStatus))
    {
        if ((TAF_PH_SERVICE_CS == pstSysCfg->ucSrvDomain)
         && (TAF_SDC_SERVICE_STATUS_LIMITED_SERVICE == enCsServiceStatus))
        {
            enRtType = TAF_SDC_REPORT_SRVSTA_LIMITED_SERVICE;
        }
        else
        {
            enRtType = TAF_SDC_REPORT_SRVSTA_NO_SERVICE;
        }
    }
    else
    {
        enRtType = TAF_SDC_REPORT_SRVSTA_LIMITED_SERVICE;
    }

    return enRtType;
}



VOS_VOID TAF_MMA_ProcLociStatusEvent(VOS_VOID)
{
   TAF_SDC_REPORT_SRVSTA_ENUM_UINT8    enStkServiceType;

   enStkServiceType = TAF_MMA_GetSrvTypeForStk();

   if (enStkServiceType != g_stMmsStkLocStaSysInfo.enServiceStatus)
   {
       if (VOS_FALSE == MN_MMA_GetRoamingBrokerFlg())
       {
          NAS_MMA_ReportLociStatus();
       }
       else
       {
           if (TAF_SDC_REPORT_SRVSTA_NORMAL_SERVICE == enStkServiceType)
           {
               NAS_MMA_ReportLociStatus();
           }
       }
   }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*--------------- STATUS�ڲ����ݴ�����  ---------------*/
/*=======================================================*/


VOS_VOID Sta_EventReport ( VOS_UINT32 ulOpID, VOS_UINT32 ulEventType )
{
    TAF_PHONE_EVENT_INFO_STRU      *pstPhoneEvent;
    VOS_UINT8                       n;          /* ѭ������ */
    VOS_UINT8                       ucNum;      /* PLMN��Ч��Ŀ*/
    TAF_PHONE_EVENT_PLMN_LIST_STRU         *pstPlmnListPhoneEvent;
    TAF_PHONE_EVENT                         ucPhoneEvent;
    VOS_UINT16                              ClientId;
    VOS_UINT8                               AppOpId;
    
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enCsServiceStatus;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enPsServiceStatus;
    TAF_SDC_REPORT_SRVSTA_ENUM_UINT8    enServiceStatus;

    enCsServiceStatus = TAF_SDC_GetCsServiceStatus();
    enPsServiceStatus = TAF_SDC_GetPsServiceStatus();
    enServiceStatus   = TAF_SDC_GetServiceStatus();


    pstPhoneEvent = (TAF_PHONE_EVENT_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMA, sizeof(TAF_PHONE_EVENT_INFO_STRU));

    if (VOS_NULL_PTR == pstPhoneEvent)
    {
        STA_TRACE(PS_PRINT_ERROR,"Sta_EventReport:ERROR:ALLOC MEMORY FAIL.");
        return;
    }

    PS_MEM_SET(pstPhoneEvent, 0, sizeof(TAF_PHONE_EVENT_INFO_STRU));
    /* ����ṹ������־ */
    pstPhoneEvent->OP_CsServiceStatus    = 0;
    pstPhoneEvent->OP_PhoneError         = 0;
    pstPhoneEvent->OP_PlmnList           = 0;
    pstPhoneEvent->OP_PlmnReselMode      = 0;
    pstPhoneEvent->OP_PsServiceStatus    = 0;
    pstPhoneEvent->OP_RegPlmn            = 0;
    pstPhoneEvent->OP_Spare              = 0;
    /*�����ϱ��¼�ѡ���ʼ��*/
    pstPhoneEvent->OP_CurRegSta          = 0;
    pstPhoneEvent->OP_Rssi               = 0;
    pstPhoneEvent->OP_BatteryPower       = 0;
    pstPhoneEvent->OP_Pin                = 0;
    pstPhoneEvent->OP_Srvst              = 0;

    /* ��д�ϱ��¼����� */
    /* ������� */
    pstPhoneEvent->OpId = (VOS_UINT8)ulOpID;         /*д��ulOpId*/
    pstPhoneEvent->PhoneEvent = (TAF_PHONE_EVENT)ulEventType;
    pstPhoneEvent->PhoneError = TAF_ERR_NO_ERROR;
    /* ��ѡ�������� */
    if ( STA_OP_REGPLMN & g_StatusContext.ucOperateType )
    {
        pstPhoneEvent->OP_RegPlmn = 1;
        pstPhoneEvent->RegPlmn.Mcc= g_StatusContext.StaPlmnId.Mcc;
        pstPhoneEvent->RegPlmn.Mnc= g_StatusContext.StaPlmnId.Mnc;
        MMA_PlmnId2Bcd(&(pstPhoneEvent->RegPlmn));
    }

    if ( STA_OP_PLMNLIST & g_StatusContext.ucOperateType )
    {
        ucNum = g_StatusContext.PlmnList.ucPlmnNum;
        if (ucNum <= TAF_MAX_PLMN_NUM)
        {
            pstPhoneEvent->OP_PlmnList = 1;
            for ( n = 0; n < ucNum; n++ )
            {
                pstPhoneEvent->PlmnList.Plmn[n].Mcc
                    = g_StatusContext.PlmnList.Plmn[n].Mcc;
                pstPhoneEvent->PlmnList.Plmn[n].Mnc
                    = g_StatusContext.PlmnList.Plmn[n].Mnc;
                MMA_PlmnId2Bcd(&(pstPhoneEvent->PlmnList.Plmn[n]));
                pstPhoneEvent->PlmnList.PlmnInfo[n].PlmnStatus
                    = g_StatusContext.PlmnList.PlmnInfo[n].PlmnStatus;
                pstPhoneEvent->PlmnList.PlmnInfo[n].RaMode
                    = g_StatusContext.PlmnList.PlmnInfo[n].RaMode;
            }
            pstPhoneEvent->PlmnList.ucPlmnNum = g_StatusContext.PlmnList.ucPlmnNum;
        }
        else
        {
            pstPhoneEvent->OP_PhoneError = 1;
            pstPhoneEvent->PhoneError    = TAF_ERR_UNSPECIFIED_ERROR;
        }
    }

    if ( STA_OP_PLMNRESELMODE & g_StatusContext.ucOperateType )
    {
        pstPhoneEvent->OP_PlmnReselMode = 1;
        pstPhoneEvent->PlmnReselMode
            = (TAF_PLMN_RESEL_MODE_TYPE)g_StatusContext.ucReselMode;
    }

    if ( STA_OP_CSSERVICESTATUS & g_StatusContext.ucOperateType )
    {
        pstPhoneEvent->OP_CsServiceStatus = 1;

        /* CS�����״ֵ̬ */
        pstPhoneEvent->CsServiceStatus
            = (TAF_PHONE_SERVICE_STATUS)enCsServiceStatus;
    }

    if ( STA_OP_PSSERVICESTATUS & g_StatusContext.ucOperateType )
    {
        pstPhoneEvent->OP_PsServiceStatus = 1;

        /* PS�����״ֵ̬ */
        pstPhoneEvent->PsServiceStatus
            = (TAF_PHONE_SERVICE_STATUS)enPsServiceStatus;
    }

    if ( STA_OP_PHONEERROR & g_StatusContext.ucOperateType )
    {
        pstPhoneEvent->OP_PhoneError = 1;
        pstPhoneEvent->PhoneError    = g_StatusContext.PhoneError;
    }


    if ( STA_OP_PHONESERVICESTATUS & g_StatusContext.ucOperateType )
    {
        pstPhoneEvent->OP_Srvst      = 1;
        pstPhoneEvent->ServiceStatus = enServiceStatus;


    }

    /* LIST�����¼�����ϵ�������п��ظ����б�,�޿��ظ�ERROR */
    if ((STA_OP_PHONEERROR == g_StatusContext.ucOperateType)
     && (TAF_PH_EVT_PLMN_LIST_CNF == ulEventType))
    {
        pstPlmnListPhoneEvent = (TAF_PHONE_EVENT_PLMN_LIST_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMA, sizeof(TAF_PHONE_EVENT_PLMN_LIST_STRU));

        if (VOS_NULL_PTR == pstPlmnListPhoneEvent)
        {
            PS_MEM_FREE(WUEPS_PID_MMA, pstPhoneEvent);
            STA_TRACE(PS_PRINT_ERROR,"Sta_EventReport:ERROR:ALLOC MEMORY FAIL.");
            return;
        }

        PS_MEM_SET(pstPlmnListPhoneEvent, 0, sizeof(TAF_PHONE_EVENT_PLMN_LIST_STRU));

        /* ���SIM��״̬ */
        if (TAF_ERR_NO_ERROR == MMA_CheckUsimStatusForPlmnSel())
        {
            /*APP����Ĺ������Ti��1*/
            if (ulOpID <= TAF_MAX_STATUS_TI)
            {
                /*ͨ��Ti��ȡClientId,CallId*/
                if (TAF_SUCCESS != MMA_GetIdByTi(TAF_MMA, (VOS_UINT8)ulOpID, &ClientId, &AppOpId, &ucPhoneEvent))
                {
                    PS_MEM_FREE(WUEPS_PID_MMA, pstPhoneEvent);
                    PS_MEM_FREE(WUEPS_PID_MMA, pstPlmnListPhoneEvent);

                    MMA_WARNINGLOG("Sta_EventReport():WARNING:Invoke Taf_GetIdByTi failed");
                    return;
                }
            }
            else
            {   /*��STATUS����������¼�*/
                AppOpId = MMA_OP_ID_INTERNAL;
                /*ClientId��Ϊ�㲥ֵ:���ֽڵĵ�4bit������MUX_PORT_BUTT*/
                ClientId = MMA_CLIENTID_BROADCAST;
            }

            /* ����LIST�����ظ��¼� */
            pstPlmnListPhoneEvent->PhoneEvent   = (TAF_PHONE_EVENT)ulEventType;
            pstPlmnListPhoneEvent->ulPlmnNum    = 0;
            pstPlmnListPhoneEvent->OpId         = (VOS_UINT8)ulOpID;         /*д��ulOpId*/
            pstPlmnListPhoneEvent->ClientId     = ClientId;
            pstPlmnListPhoneEvent->OpId         = AppOpId;

            /* ���õ绰�����ϱ����� */
            MN_PH_SendMsg(pstPlmnListPhoneEvent->ClientId,(VOS_UINT8*)pstPlmnListPhoneEvent,sizeof(TAF_PHONE_EVENT_PLMN_LIST_STRU));

            PS_MEM_FREE(WUEPS_PID_MMA, pstPlmnListPhoneEvent);
            PS_MEM_FREE(WUEPS_PID_MMA, pstPhoneEvent);

            return;
        }

        pstPhoneEvent->OP_PhoneError = MMA_TRUE;

        pstPhoneEvent->PhoneError = TAF_ERR_USIM_SIM_CARD_NOTEXIST;

        /* ���õ绰�����¼��ϱ����� ��APP����Ӧ���¼� */
        Taf_PhoneEvent (pstPhoneEvent);

        PS_MEM_FREE(WUEPS_PID_MMA, pstPhoneEvent);

        PS_MEM_FREE(WUEPS_PID_MMA, pstPlmnListPhoneEvent);

        return;
    }

    /* ���õ绰�����¼��ϱ����� ��APP����Ӧ���¼�*/
    Taf_PhoneEvent (pstPhoneEvent);

    PS_MEM_FREE(WUEPS_PID_MMA, pstPhoneEvent);

    
    
    return ;    /* �������� */
}

/*************************************************
  Function:
    Sta_IsFsmSubValid
  Description:
     �ж���״̬�Ƿ�ƥ��FSM��״̬
  Calls:                 �����������õĺ����嵥
  Called By:             ���ñ������ĺ����嵥
  Input:
    VOS_UINT8   ucFsmSub;   ��״ֵ̬
    VOS_UINT32  ulFsm;      FSM״ֵ̬
  Output:
    ��
  Return:
    STA_TRUE,  ��Ч����״ֵ̬��ƥ�䣻
    STA_FALSE, ��Ч����״ֵ̬����ƥ�䣻
  Others:
  1.��    ��   : 2007��07��24��
    ��    ��   : l60022475
    �޸�����   : A32D12535
*************************************************/
VOS_BOOL Sta_IsFsmSubValid ( VOS_UINT8 ucFsmSub, VOS_UINT32 ulFsm )
{
    switch ( ulFsm )
    {
        case STA_FSM_RESTART:
        case STA_FSM_STOP:
        case STA_FSM_PLMN_LIST:
        case STA_FSM_PLMN_SEL:
            /* ==>A32D12535 */
            if ( (STA_FSMSUB_MONO == ucFsmSub)
               ||(STA_FSM_RESTART == ucFsmSub)
                )
            /* <==A32D12535 */
            {
                return STA_TRUE;
            }
            return STA_FALSE;

        /*
          ����CS&PSͬʱDETACH��ATTACH״̬�ĺϷ��ԣ�
          modified by roger in 2005-07-11
        */
        case STA_FSM_DE_ATTACH:
            if ( (STA_FSMSUB_ATTACH_CS == ucFsmSub)
              || (STA_FSMSUB_ATTACH_PS == ucFsmSub)
              || (STA_FSMSUB_DETACH_CS == ucFsmSub)
              || (STA_FSMSUB_DETACH_PS == ucFsmSub)
              || (STA_FSMSUB_ATTACH_CSPS == ucFsmSub)
              || (STA_FSMSUB_DETACH_CSPS == ucFsmSub) )
            {
                return STA_TRUE;
            }
            return STA_FALSE;

        case STA_FSM_PLMN_RESEL:
            if ( (STA_FSMSUB_PLMN_RESEL_AUTO == ucFsmSub)
              || (STA_FSMSUB_PLMN_RESEL_MANUAL == ucFsmSub) )
            {
                return STA_TRUE;
            }
            return STA_FALSE;

        /* �ڲ���������ʱʹ�õ��м�״̬������Ϊ��Ч״̬ */
        case STA_FSM_NO_UPDATE:
        /* FSM����̬������״ֵ̬û��ƥ��Ҫ�� */
        case STA_FSM_ENABLE:
        case STA_FSM_NULL:
            return STA_TRUE;

        default :
            return STA_FALSE;
    }
}

/*************************************************
  Function:
    Sta_IsInvalidOpId
  Description:
    �Ƿ�����Ч��OPID
  Calls:                  �����������õĺ����嵥
  Called By:              ���ñ������ĺ����嵥
  Input:
    VOS_UINT32  ulOpId;      �����ʶ
  Output:
    ��
  Return:
    STA_TRUE,   ָ����ulOpId��Ч��
    STA_FALSE,  ָ����ulOpId��Ч��
  Others:
*************************************************/
VOS_BOOL Sta_IsInvalidOpId ( VOS_UINT32 ulOpId )
{
    if ( STA_OPID_INSIDE == ulOpId )
    {
        return STA_FALSE;
    }

    if ( ulOpId <= STA_OPID_MAX )
    {
        return STA_FALSE;
    }

    return STA_TRUE;
}


/* BEGIN: Deleted by liuyang, 2005/10/26 */
/*�˺����Ѿ�����*/
/*************************************************
  Function:
    Sta_IsDualFsmSub
  Description:
    �Ƿ����������״̬ͬʱ��Ч�����ͬһ��OPID
  Calls:                  �����������õĺ����嵥
  Called By:              ���ñ������ĺ����嵥
  Input:
    VOS_UINT32  ulOpId;      �����ʶ
  Output:
    ��
  Return:
    STA_TRUE,   ����˫����Ч��״̬��
    STA_FALSE,  ������˫����Ч��״̬��
  Others:
*************************************************/
VOS_UINT32 Sta_IsDualFsmSub ( VOS_VOID )
{
    /*modified by liuyang id:48197 date:2005-10-5 for V100R001*/
    /*��ǰ���е�˫����״̬��OpId����һ���ģ�
    ����жϵ���״̬��־λ��Ϊ��ͬʱ,��״̬Ϊ˫����״̬*/
    if  ( g_StatusContext.aucFsmSubFlag[0]
          != g_StatusContext.aucFsmSubFlag[1])
    {
        return STA_FALSE;
    }
    /* ��״̬OPID����ͬ������˫����״̬ */
    /*if ( g_StatusContext.aFsmSub[0].ulOpId
        != g_StatusContext.aFsmSub[1].ulOpId )
    {
        return STA_FALSE;
    }
    */
    /*modified by liuyang id:48197 date:2005-10-5 for V100R001*/
    return STA_TRUE;
}
/* END:   Deleted by liuyang, 2005/10/26 */

/*************************************************
  Function:
    Sta_FindFsmSubBySubSta
  Description:
     ����ָ�����������ʶ�Ķ�Ӧ��״̬������
  Calls:                  �����������õĺ����嵥
  Called By:              ���ñ������ĺ����嵥
  Input:
    VOS_UINT32  ulOpId;       �����ʶ
  Output:
    ��
  Return:
    STA_FSMSUB_INDEX_NULL, ��Ч����״̬����ֵ��
    0< ret <STA_TIMER_MAX, ���������ֵret��
  Others:
*************************************************/
/*
VOS_UINT32 Sta_FindFsmSubBySubSta ( VOS_UINT32 ulOpId )
{
    VOS_UINT8       ucIndex;
    VOS_UINT8       n;

    ucIndex = STA_FSMSUB_INDEX_NULL;
    for ( n = 0; n < STA_TIMER_MAX; n++ )
    {
        if ( ulOpId == g_StatusContext.aFsmSub[n].ulOpId )
        {
            ucIndex = n;
            break;
        }
    }

    return ucIndex;
}
*/
/*************************************************
  Function:
    Sta_FindFsmSubBySubSta
  Description:
     ����ָ��ָ����״̬�Ķ�Ӧ��״̬������
  Calls:                  �����������õĺ����嵥
  Called By:              ���ñ������ĺ����嵥
  Input:
    VOS_UINT8  ucFsmSubSta;       �����ʶ
  Output:
    ��
  Return:
    STA_FSMSUB_INDEX_NULL, ��Ч����״̬����ֵ��
    0< ret <STA_TIMER_MAX, ���������ֵret��
  Others:
*************************************************/
VOS_UINT8 Sta_FindFsmSubBySubSta ( VOS_UINT8 ucFsmSubSta )
{
    VOS_UINT8       ucIndex;
    VOS_UINT8       n;

    ucIndex = STA_FSMSUB_INDEX_NULL;

    for ( n = 0; n < STA_TIMER_MAX; n++ )
    {
        if (ucFsmSubSta == g_StatusContext.aFsmSub[n].ucFsmStateSub)
        {
            ucIndex = n;
            break;
        }
    }

    return ucIndex;
}

/*************************************************
  Function:
    Sta_FindFsmSub
  Description:
     ���Ҷ�Ӧ��״̬��������
  Calls:                  �����������õĺ����嵥
  Called By:              ���ñ������ĺ����嵥
  Input:
    VOS_UINT8  ucFsmSub;      �����ʶ
  Output:
    ��
  Return:
    STA_FSMSUB_INDEX_NULL, ��Ч����״̬����ֵ��
    0< ret <STA_TIMER_MAX, ���������ֵret��
  Others:
*************************************************/
VOS_UINT32 Sta_FindFsmSub ( VOS_UINT8 ucFsmSub )
{
    VOS_UINT8       ucIndex;
    VOS_UINT8       n;

    ucIndex = STA_FSMSUB_INDEX_NULL;    /* ��Ч������ */
    for ( n = 0; n < STA_TIMER_MAX; n++ )
    {
        /* ������״̬���Բ��� */
        if ( ucFsmSub <= STA_FSMSUB_DETACH_PS )
        {
            if ( ucFsmSub == g_StatusContext.aFsmSub[n].ucFsmStateSub )
            {
                ucIndex = n;
                break;
            }
        }

        /* CS&PS ATTACH��״̬���Բ��� */
        if ( STA_FSMSUB_ATTACH_CSPS == ucFsmSub )
        {
            if ( (STA_FSMSUB_ATTACH_CS
                == g_StatusContext.aFsmSub[n].ucFsmStateSub)
              || (STA_FSMSUB_ATTACH_PS
                == g_StatusContext.aFsmSub[n].ucFsmStateSub) )
            {
                ucIndex = n;
                break;
            }
        }

        /* CS&PS DETACH��״̬���Բ��� */
        if ( STA_FSMSUB_DETACH_CSPS == ucFsmSub )
        {
            if ( (STA_FSMSUB_DETACH_CS
                == g_StatusContext.aFsmSub[n].ucFsmStateSub)
              || (STA_FSMSUB_DETACH_PS
                == g_StatusContext.aFsmSub[n].ucFsmStateSub) )
            {
                ucIndex = n;
                break;
            }
        }
    }

    return ucIndex;
}

/*************************************************
  Function:
    Sta_FindIdleFsmSub
  Description:
     ����ָ����״ֵ̬�Ķ�Ӧ�Ŀ�����Դ������
  Calls:                  �����������õĺ����嵥
  Called By:              ���ñ������ĺ����嵥
  Input:
    VOS_UINT32  ulOpId;      �����ʶ
  Output:
    ��
  Return:
    STA_FSMSUB_INDEX_NULL, ��Ч����״̬����ֵ��
    0< ret <STA_TIMER_MAX, ���������ֵret��
  Others:
*************************************************/
VOS_UINT32 Sta_FindIdelFsmSub ( VOS_UINT8 ucSubState )
{
    VOS_UINT8       ucRetIndex;
    VOS_UINT8       n;                  /* ѭ������ */

    ucRetIndex = STA_FSMSUB_INDEX_NULL;

    /* ���ҿ�����״̬ */
    for ( n = 0; n < STA_TIMER_MAX; n++ )
    {
        if ( STA_FSMSUB_NULL == g_StatusContext.aucFsmSubFlag[n] )
        {
            ucRetIndex = n;
            break;
        }
    }

    /* �޿��õ���״̬��Դ��ֱ�ӷ�����Ч����ֵ */
    if ( STA_FSMSUB_INDEX_NULL == ucRetIndex )
    {
        return STA_FSMSUB_INDEX_NULL;      /* ������Чֵ */
    }

    /* ���õ���״̬��Դ���㣬ֱ�ӷ�����Ч����ֵ */
    if ( (STA_FSMSUB_ATTACH_CSPS == ucSubState)
      || (STA_FSMSUB_DETACH_CSPS == ucSubState) )
    {
        /*modified by liuyang id:48197 date:2005-10-5 for V100R001*/
        /*if ( 0 != ucRetIndex )*/
        if ((STA_FSMSUB_NULL != g_StatusContext.aucFsmSubFlag[0])
          ||(STA_FSMSUB_NULL != g_StatusContext.aucFsmSubFlag[1]))
        /*modified by liuyang id:48197 date:2005-10-5 for V100R001*/
        {
            return STA_FSMSUB_INDEX_NULL;   /* ������Чֵ */
        }
        else
        {
            return ucRetIndex;              /* ����һ����Ч������ֵ */
        }
    }

    /* ����״ֱ̬�ӷ��ؿ�����״̬���� */
    if ( (STA_FSMSUB_NULL == ucSubState)
        || (STA_FSMSUB_OK == ucSubState)
        || (STA_FSMSUB_MONO == ucSubState) )
    {
        return ucRetIndex;
    }

    /* �ǿ��е���״ֻ̬�������һ�� */
    for ( n = 0; n < STA_TIMER_MAX; n++ )
    {
        if ( ucSubState == g_StatusContext.aucFsmSubFlag[n] )
        {
            return STA_FSMSUB_INDEX_NULL;   /* ������Чֵ */
        }
    }

    return ucRetIndex;
}
VOS_UINT32 Sta_UpdateFsmFlag ( VOS_UINT8 ucFsmSub,
                           VOS_UINT32 ulFsm,
                           VOS_UINT32 ulType )
{
    VOS_UINT8                           n;                  /* ѭ������*/
    VOS_UINT8                           ucIndex;            /* ��״̬����*/

    ucIndex = STA_FSMSUB_INDEX_NULL;

    /* ���ո������ͽ��в��� */
    switch ( ulType )
    {
        case STA_UPDATE_SETNEW:
            if (  STA_FSM_NO_UPDATE == ulFsm )
            {
               return STA_SUCCESS;
            }
            /* ˫����״̬ͬʱ���� */
            if ((STA_FSMSUB_ATTACH_CSPS == ucFsmSub)
              ||(STA_FSMSUB_DETACH_CSPS == ucFsmSub))
            {
                for ( n = 0; n < STA_TIMER_MAX; n++ )
                {
                    g_StatusContext.aFsmSub[n].ucFsmStateSub
                        = STA_FSMSUB_NULL;
                    g_StatusContext.aucFsmSubFlag[n]
                        = STA_FSMSUB_NULL;
                    /*����ǰ���е�OpId����Ϊ��Ч*/
                    g_StatusContext.aFsmSub[n].ulOpId = STA_OPID_INVALID;
                    /*�����ǰ������־*/
                    g_StatusContext.aucStaPhoneOperator[n] = STA_FSMSUB_NULL;
                }
                g_StatusContext.ulFsmState = ulFsm;
                return STA_SUCCESS;
            }
            /* ˫����״̬�ĵ�һ���� */
            if ( STA_FSM_DE_ATTACH == ulFsm )
            {
                /*�滻������״̬��������*/
                for ( n = 0; n < STA_TIMER_MAX; n++ )
                {
                    if ( g_StatusContext.aFsmSub[n].ulOpId != STA_OPID_INVALID )
                    {
                        ucIndex = n;
                        break;
                    }
                    else
                    {
                        ucIndex = STA_FSMSUB_INDEX_NULL;
                    }
                }

                if ( STA_FSMSUB_INDEX_NULL == ucIndex )
                {/* ��Ч����״̬ */
                    STA_TRACE (STA_WARNING_PRINT,
                        "Sta_UpdateFsmFlag():WARNING:invalid FSM substate  ");
                    return STA_ERROR;
                }
                g_StatusContext.aFsmSub[ucIndex].ucFsmStateSub = ucFsmSub;
                g_StatusContext.aucFsmSubFlag[ucIndex]         = ucFsmSub;
                /*����OpID��Чֵ*/
                g_StatusContext.aFsmSub[ucIndex].ulOpId        = STA_OPID_INVALID;
                /*�����ǰ������־*/
                g_StatusContext.aucStaPhoneOperator[ucIndex]   = STA_FSMSUB_NULL;
                return STA_SUCCESS;
            }
            /* ������״̬�ĸ��� */
            g_StatusContext.aFsmSub[0].ucFsmStateSub  = ucFsmSub;
            g_StatusContext.aucFsmSubFlag[0]          = ucFsmSub;
            break;

        /* ������״̬�ĸ���,�������ʱ������Դ */
        case STA_UPDATE_MONO_CLEAR:

            if (VOS_NULL_PTR != g_StatusContext.ahStaTimer[0])
            {
                if (VOS_OK != NAS_StopRelTimer(WUEPS_PID_MMA,
                                 g_StatusContext.aFsmSub[0].TimerName,
                                &g_StatusContext.ahStaTimer[0]))
                {
                    STA_TRACE(STA_WARNING_PRINT, "Sta_UpdateFsmFlag():WARNING:NAS_StopRelTimer failed!");
                }
            }
            g_StatusContext.aFsmSub[0].ucFsmStateSub  = ucFsmSub;
            g_StatusContext.aucFsmSubFlag[0]          = ucFsmSub;
            g_StatusContext.aFsmSub[0].TimerName      = STA_TIMER_NULL;
            g_StatusContext.ahStaTimer[0]             = VOS_NULL_PTR;



            if ( STA_FSM_INVALID != ulFsm )
            {
                g_StatusContext.ulFsmState = ulFsm;
            }

            /*����OpID��Чֵ*/
            g_StatusContext.aFsmSub[0].ulOpId        = STA_OPID_INVALID;
            /*�����ǰ������־*/
            g_StatusContext.aucStaPhoneOperator[0]   = STA_FSMSUB_NULL;
            break;

        /* ˫����״̬ͬʱ����, ����������Դ���綨ʱ�� */
        case STA_UPDATE_DUAL_CLEAR:

            for ( n = 0; n < STA_TIMER_MAX; n++ )
            {
                if (VOS_NULL_PTR != g_StatusContext.ahStaTimer[n])
                {
                    if (VOS_OK != NAS_StopRelTimer(WUEPS_PID_MMA,
                                     g_StatusContext.aFsmSub[n].TimerName,
                                    &g_StatusContext.ahStaTimer[n]))
                    {
                        STA_TRACE(STA_WARNING_PRINT, "Sta_UpdateFsmFlag():WARNING:NAS_StopRelTimer failed!");
                    }
                }
                g_StatusContext.aFsmSub[n].ucFsmStateSub  = ucFsmSub;
                g_StatusContext.aucFsmSubFlag[n]          = ucFsmSub;
                g_StatusContext.aFsmSub[n].TimerName      = STA_TIMER_NULL;
                g_StatusContext.ahStaTimer[n]             = VOS_NULL_PTR;


                /*����OpID��Чֵ*/
                g_StatusContext.aFsmSub[n].ulOpId        = STA_OPID_INVALID;

                /*�����ǰ������־*/
                g_StatusContext.aucStaPhoneOperator[n]   = STA_FSMSUB_NULL;
            }
            if ( (STA_FSM_INVALID != ulFsm)
                && (STA_FSM_NO_UPDATE != ulFsm) )
            {
                g_StatusContext.ulFsmState = ulFsm;
            }
            break;

       /* ˫����״̬�ĵ�һ���£��������ʱ������Դ */
        case STA_UPDATE_SINGL_CLEAR:
            ucIndex = (VOS_UINT8)Sta_FindFsmSub (ucFsmSub);

            if ( STA_FSMSUB_INDEX_NULL == ucIndex )
            {/* ��Ч����״̬ */
                STA_TRACE (STA_WARNING_PRINT,
                    "Sta_UpdateFsmFlag():WARNING:invalid FSM substate");
                return STA_ERROR;
            }

            if (VOS_NULL_PTR != g_StatusContext.ahStaTimer[ucIndex])
            {
                if (VOS_OK != NAS_StopRelTimer(WUEPS_PID_MMA,
                                 g_StatusContext.aFsmSub[ucIndex].TimerName,
                                &g_StatusContext.ahStaTimer[ucIndex]))
                {
                    STA_TRACE(STA_WARNING_PRINT, "Sta_UpdateFsmFlag():WARNING:NAS_StopRelTimer failed!");
                }
            }

            g_StatusContext.aFsmSub[ucIndex].ucFsmStateSub  = STA_FSMSUB_NULL;
            g_StatusContext.aucFsmSubFlag[ucIndex]          = STA_FSMSUB_NULL;

            g_StatusContext.aFsmSub[ucIndex].TimerName      = STA_TIMER_NULL;
            g_StatusContext.ahStaTimer[ucIndex]             = VOS_NULL_PTR;


            /*�����ǰ������־*/
            g_StatusContext.aucStaPhoneOperator[ucIndex]   = STA_FSMSUB_NULL;

            /*
              ������STA_FSM_NO_UPDATEʱ������״̬����״ֵ̬��
            */
            if ( (STA_FSM_INVALID != ulFsm)
                && (STA_FSM_NO_UPDATE != ulFsm) )
            {
                g_StatusContext.ulFsmState = ulFsm;
            }

            /*����OpID��Чֵ*/
            g_StatusContext.aFsmSub[ucIndex].ulOpId        = STA_OPID_INVALID;
            break;

        /* δʹ�õ�״̬�����ش��� */
        default:
            STA_TRACE (STA_WARNING_PRINT,
                "Sta_UpdateFsmFlag():WARNING:unused state, do nothing  ");
            return STA_ERROR;
    }

    return STA_SUCCESS;
}


VOS_UINT32 Sta_UpdateData ( STA_PHONE_DATA_S *pData )
{
    VOS_UINT32  ulRet;
    VOS_UINT8   ucFsmSub;
    VOS_UINT32  ulFsm;
    VOS_UINT8   ucFsmFlag;

    /* ����ָ����Ч���ɵ����߱�֤��Ĭ����Ч */
    ucFsmSub  = pData->ucFsmStateSub;
    ulFsm     = pData->ulFsmState;
    ucFsmFlag = pData->ucFsmUpdateFlag;

    /* �ж���״ֵ̬�Ƿ���Ч */
    if ( !Sta_IsFsmSubValid (ucFsmSub,ulFsm) )
    {
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_UpdateData():WARNING:unmatched FSM state  ");

        return STA_ERROR;     /* ���󷵻� */
    }

    /* ������״̬�ĸ���ģʽ������ */
    switch ( pData->ucFsmSubOperate )
    {
        /* ����FSM״̬��־������ʼ�µ���״̬ */
        case STA_FSMSUB_NEW:
            g_StatusContext.ulCurOpId = pData->ulOpId;  /*��¼�����ʶ��*/
            ulRet = Sta_UpdateFsmFlag (ucFsmSub, ulFsm, STA_UPDATE_SETNEW);
            if ( STA_ERROR == ulRet )
            {
                STA_TRACE (STA_WARNING_PRINT,
                    "Sta_UpdateData():WARNING:Sta_UpdateFsmFlag failed  ");
            }
            break;

        /* ˢ����״̬������Ϣ */
        case STA_FSMSUB_UPDATE:
            g_StatusContext.ulNewOpId = pData->ulOpId;  /* ���θ��µı�ʶ��*/
            break;

        /* ��״̬������ɣ������״̬��־ */
        case STA_FSMSUB_FINISH:
            ulRet = Sta_UpdateFsmFlag (ucFsmSub, ulFsm, ucFsmFlag);
            if ( STA_ERROR == ulRet )
            {
                STA_TRACE (STA_WARNING_PRINT,
                    "Sta_UpdateData():WARNING:Sta_UpdateFsmFlag failed  ");
            }
            break;

        /* ����ģʽ��֧�֣�ֱ�ӷ��� */
        default:
            STA_TRACE (STA_WARNING_PRINT,
                "Sta_UpdateData():WARNING:invalid operate ");
            return STA_ERROR;
    }

    /* ����STATUSҵ����Ϣ��¼ */
    g_StatusContext.StaPlmnId.Mcc       = pData->PlmnId.Mcc;
    g_StatusContext.StaPlmnId.Mnc       = pData->PlmnId.Mnc;

    g_StatusContext.ulMmProc            = pData->ulMmProc ;
    g_StatusContext.ucDomainFlag        = pData->ucDomainFlag;
    /*��֤ÿ��ֻ����Ч���򷵻صķ���״̬����ȫ�ֱ���*/
    switch ( pData->ucDomainFlag )
    {
        case MMA_MMC_SRVDOMAIN_CS :
            TAF_SDC_SetCsServiceStatus((VOS_UINT8)pData->ulCsServiceStatus);
            g_StatusContext.ulCsCause = pData->ulCsCause;
            break;
        case MMA_MMC_SRVDOMAIN_PS:
            TAF_SDC_SetPsServiceStatus((VOS_UINT8)pData->ulPsServiceStatus);
            g_StatusContext.ulPsCause = pData->ulPsCause;
            break;
        case MMA_MMC_SRVDOMAIN_CS_PS:
            TAF_SDC_SetCsServiceStatus((VOS_UINT8)pData->ulCsServiceStatus);
            TAF_SDC_SetPsServiceStatus((VOS_UINT8)pData->ulPsServiceStatus);
            g_StatusContext.ulCsCause = pData->ulCsCause;
            g_StatusContext.ulPsCause = pData->ulPsCause;
            break;
        default :
            break;
    }

    return STA_SUCCESS;       /* �������� */
}




/*
    �ڽ���PLMN����̬֮��ͨ��MMC���ϱ���Ϣ����
    ������������
    modified by liuyang id:48197 date:2005-9-5 for V200R001
*/
/*****************************************************************************
 �� �� ��  : ClearCurPlmnId
 ��������  : ��PLMNID���㣬��Ϊ��ЧPLMN
 �������  : ��
 �� �� ֵ  : VOS_UINT8
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2005��9��5��
    ��    ��   : liuyang
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID   Sta_ClearCurPlmnId(TAF_PLMN_ID_STRU *pstNewPlmnId)
{
    pstNewPlmnId->Mcc  = 0;
    pstNewPlmnId->Mnc  = 0;
}

/*****************************************************************************
 �� �� ��  : Sta_InitCurPlmnId
 ��������  : ����ǰPLMNidװ�뵱ǰȫ�ֱ���
 �������  : TAF_PLMN_ID_STRU NewPlmnId
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2005��9��5��
    ��    ��   : liuyang
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID   Sta_InitCurPlmnId(TAF_PLMN_ID_STRU    stSrcPlmnId,
                             TAF_PLMN_ID_STRU   *pstDstPlmnId)
{
    *pstDstPlmnId = stSrcPlmnId;
}



/*****************************************************************************
 �� �� ��  : Sta_StopAllRunningTimer
 ��������  : ֹͣ�������еĶ�ʱ�����������ʱ��״̬��������صı���
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2006��1��19��
    ��    ��   : liuyang id:48197
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 Sta_StopAllRunningTimer()
{
    VOS_UINT32      ulI;
    VOS_UINT32      ulRet = VOS_OK;

    for ( ulI = 0; ulI < STA_TIMER_MAX; ulI++ )
    {
        if (STA_TIMER_NULL != g_StatusContext.aFsmSub[ulI].TimerName )
        {
            ulRet = VOS_StopRelTimer (&g_StatusContext.ahStaTimer[ulI]);
            if ( VOS_OK != ulRet )
            {
                STA_TRACE (STA_WARNING_PRINT,
                    "Sta_StopAllRunningTimer():WARNING:VOS_StopRelTimer failed ");
            }
            g_StatusContext.aFsmSub[ulI].TimerName = STA_TIMER_NULL;
        }
    }
    return ulRet;
}

/* Sta_ResetFsmFlg */


/* BEGIN: Added by liuyang id:48197, 2006/3/13   PN:A32D02500*/

VOS_UINT32 Sta_AttachByModeService(VOS_UINT32  ulOpId)
{
    VOS_UINT32                          ulAttachType;
    VOS_UINT32                          ulRet;
    VOS_UINT8                           ucAutoAttachFlg;
    NAS_NVIM_AUTOATTACH_STRU            stAutoattachFlag;
    VOS_UINT32                          ulLength;

    ulLength = 0;
    PS_MEM_SET(&stAutoattachFlag, 0x00, sizeof(NAS_NVIM_AUTOATTACH_STRU));
    NV_GetLength(en_NV_Item_Autoattach, &ulLength);
    if (NV_OK == NV_Read(en_NV_Item_Autoattach,
                         (VOS_VOID*)(&stAutoattachFlag),
                         ulLength))
    {
        STA_TRACE1(STA_INFO_PRINT,
            "Sta_AttachByModeService:INFO:Read Auto Attach",(VOS_INT32)stAutoattachFlag.usAutoattachFlag);

        if (1 == stAutoattachFlag.usAutoattachFlag)
        {
            ucAutoAttachFlg = MMA_AUTO_ATTACH_ENABLE;
        }
        else
        {
            ucAutoAttachFlg = MMA_AUTO_ATTACH_DISABLE;
        }
    }
    else
    {
        ucAutoAttachFlg = MMA_AUTO_ATTACH_ENABLE;
    }
    switch (g_StatusContext.ucModeService)
    {
        case TAF_PH_MS_CLASS_A:
            ulAttachType = STA_ATTACH_TYPE_GPRS_IMSI;
            if (MMA_AUTO_ATTACH_DISABLE == ucAutoAttachFlg)
            {
                 ulAttachType = STA_ATTACH_TYPE_IMSI;
            }
            break;

        case TAF_PH_MS_CLASS_CG:
            ulAttachType = STA_ATTACH_TYPE_GPRS;
            if (MMA_AUTO_ATTACH_DISABLE == ucAutoAttachFlg)
            {
                 STA_TRACE(STA_NORMAL_PRINT,
                           "Sta_AttachByModeService:WARNING:Auto Attach Not Allowed!");
                 return STA_ERROR;
            }
            break;

        case TAF_PH_MS_CLASS_CC:
            ulAttachType = STA_ATTACH_TYPE_IMSI;
            break;

        case TAF_PH_MS_CLASS_NULL:
            g_StatusContext.ulFsmState = STA_FSM_ENABLE;
            return STA_SUCCESS;

        default:
            STA_TRACE (STA_WARNING_PRINT,
            "Sta_AttachByModeService():WARNING:g_StatusContext.ucModeService error");
            return STA_ERROR;
    }
    ulRet      = Sta_Attach (ulOpId, ulAttachType, TAF_MMA_ATTACH_REASON_INITIAL);
    if ( STA_ERROR == ulRet )
    {
        STA_TRACE (STA_WARNING_PRINT,
            "Sta_AttachByModeService():WARNING:Sta_Attach failed");
    }
    return ulRet;
}




VOS_UINT8 Sta_GetStatusForSat(VOS_UINT32 ulServiceStatus)
{
    VOS_UINT8 ucSatServiceStatus;

    switch (ulServiceStatus)
    {
        case MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE:
            ucSatServiceStatus = STA_SAT_LOCATION_NORMAL_SERVICE;
            break;

        case MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE :
        case MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE_REGION :
        case MMA_MMC_SERVICE_STATUS_NO_IMSI :
            ucSatServiceStatus = STA_SAT_LOCATION_LIMIT_SERVICE;
            break;

        case MMA_MMC_SERVICE_STATUS_NO_SERVICE:
            ucSatServiceStatus = STA_SAT_LOCATION_NO_SERVICE;
            break;

        default :
            ucSatServiceStatus = STA_SAT_LOCATION_NO_SERVICE;
            break;
    }

    return ucSatServiceStatus;
}


VOS_VOID Sta_GetLocInfoForSat(
    MMA_MMC_PLMN_ID_STRU                stPlmnId,
    VOS_UINT16                          usLac,
    VOS_UINT16                          usCellId,
    VOS_UINT8                          *pucLocInfo
)
{
    VOS_UINT8 ucSn = 0;

    pucLocInfo[ucSn]    = (VOS_UINT8)(stPlmnId.ulMcc & 0x0000000f);
    pucLocInfo[ucSn++] |= (VOS_UINT8)((stPlmnId.ulMcc & 0x00000f00) >> 4);
    pucLocInfo[ucSn]    = (VOS_UINT8)((stPlmnId.ulMcc & 0x000f0000) >> 16);
    pucLocInfo[ucSn++] |= (VOS_UINT8)((stPlmnId.ulMnc & 0x000f0000) >> 12);
    pucLocInfo[ucSn]    = (VOS_UINT8)(stPlmnId.ulMnc & 0x0000000f);
    pucLocInfo[ucSn++] |= (VOS_UINT8)((stPlmnId.ulMnc & 0x00000f00) >> 4);

    pucLocInfo[ucSn++]  = (VOS_UINT8)(usLac >> 8);
    pucLocInfo[ucSn++]  = (VOS_UINT8)(usLac & 0x000000ff);

    pucLocInfo[ucSn++]  = (VOS_UINT8)(usCellId >> 8);
    pucLocInfo[ucSn++]  = (VOS_UINT8)(usCellId & 0x000000ff);

    for (ucSn = 0; ucSn < STA_SAT_LOCATION_INFORMATON_LEN; ucSn ++)
    {
        STA_TRACE1(STA_INFO_PRINT,
                   "Sta_GetLocInfoForSat():INFO:Locinfo:",pucLocInfo[ucSn]);
    }

    return;
}




VOS_VOID Mma_ModeChgReport(
    TAF_SDC_SYS_MODE_ENUM_UINT8         enCurNetWork,
    VOS_UINT8                           ucSysSubMode
)
{

    VOS_UINT8                            ucModeChangeFlg;

    ucModeChangeFlg = TAF_MMA_IsModeChange(enCurNetWork, ucSysSubMode);

    if (VOS_TRUE == ucModeChangeFlg)
    {
        if (TAF_SDC_SYS_MODE_WCDMA == enCurNetWork)
        {
            if (NAS_UTRANCTRL_UTRAN_MODE_FDD == NAS_UTRANCTRL_GetCurrUtranMode())
            {
                /* G->W�£����ϱ�ȱʡģʽ: ^MODE:5,4,�����ϱ�ʱ����RRMM_AT_MSG_IND���� */
                Sta_ModeChangeEventReport(TAF_PH_INFO_WCDMA_RAT, TAF_SYS_SUBMODE_WCDMA);
            }
            else
            {
                /* G->TD�£����ϱ�ȱʡģʽ: ^MODE:15,8,�����ϱ�ʱ����RRMM_AT_MSG_IND���� */
                Sta_ModeChangeEventReport(TAF_PH_INFO_TD_SCDMA_RAT, TAF_SYS_SUBMODE_TD_SCDMA);
            }

            /* ����ȫ�ֱ����б����UtranMode */
            g_StatusContext.ucPreUtranMode = NAS_UTRANCTRL_GetCurrUtranMode();
        }
        else if (TAF_SDC_SYS_MODE_GSM == enCurNetWork)
        {
            Sta_ModeChangeEventReport(TAF_PH_INFO_GSM_RAT, ucSysSubMode);
        }
        else if (TAF_SDC_SYS_MODE_LTE == enCurNetWork)
        {
            Sta_ModeChangeEventReport(TAF_PH_INFO_LTE_RAT, ucSysSubMode);
        }
        else
        {
            Sta_ModeChangeEventReport(TAF_PH_INFO_NONE_RAT, TAF_SYS_SUBMODE_NONE);
        }
    }

    return;
}

TAF_PH_RAT_TYPE_ENUM_UINT8 TAF_MMA_ConvertMmaRatToStkRat(
                                         TAF_SDC_SYS_MODE_ENUM_UINT8 enMmaRatType
                                         )
{
    switch (enMmaRatType)
    {
        case TAF_SDC_SYS_MODE_GSM:
             return TAF_PH_RAT_GSM;

        case TAF_SDC_SYS_MODE_WCDMA:
             return TAF_PH_RAT_WCDMA;

        case TAF_SDC_SYS_MODE_LTE:
             return TAF_PH_RAT_LTE;

        default:
            MN_ERR_LOG("TAF_MMA_ConvertMmaRatToStkRat: The input is invalid");
            return TAF_PH_RAT_BUTT;
    }
}


/* ɾ��TAF_MMA_SndStkAccTechChgEvt */

VOS_UINT32 Sta_MmInfo(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PHONE_EVENT_INFO_STRU          *pstPhoneEvent;
    MMC_MMA_MM_INFO_IND_STRU           *pstMmInfo;

    pstMmInfo = (MMC_MMA_MM_INFO_IND_STRU*)pstMsg;

    pstPhoneEvent = (TAF_PHONE_EVENT_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMA, sizeof(TAF_PHONE_EVENT_INFO_STRU));

    if (VOS_NULL_PTR == pstPhoneEvent)
    {
        MMA_ERRORLOG("Sta_MmInfo:ERROR:ALLOC MEMORY FAIL.");
        return VOS_TRUE;
    }

    PS_MEM_SET(pstPhoneEvent, 0, sizeof(TAF_PHONE_EVENT_INFO_STRU));

    pstPhoneEvent->OP_MmInfo    = MMA_TRUE;
    pstPhoneEvent->PhoneEvent   = TAF_PH_EVT_MM_INFO_IND;

    PS_MEM_CPY(&gstMmaValue.stOperatorNameInfo, &pstMmInfo->stName, sizeof(gstMmaValue.stOperatorNameInfo));

    PS_MEM_CPY(pstPhoneEvent->stMmInfo.ucLSAID, pstMmInfo->aucLSAID, sizeof(pstPhoneEvent->stMmInfo.ucLSAID));
    pstPhoneEvent->stMmInfo.stUniversalTimeandLocalTimeZone  = pstMmInfo->stUniversalTimeandLocalTimeZone;
    pstPhoneEvent->stMmInfo.cLocalTimeZone                   = pstMmInfo->cLocalTimeZone;
    pstPhoneEvent->stMmInfo.ucDST                            = pstMmInfo->ucDST;
    pstPhoneEvent->stMmInfo.ucIeFlg                          = pstMmInfo->ucIeFlg;


    Taf_PhoneEvent(pstPhoneEvent);

    PS_MEM_FREE(WUEPS_PID_MMA, pstPhoneEvent);

    return VOS_TRUE;
}



TAF_PH_PLMN_SELECTION_RESULT_ENUM_UINT32 TAF_MMA_ConvertServiceTypeToAtType(
    MMA_MMC_PLMN_SELECTION_RESULT_ENUM_UINT32 enMmaServiceType
)
{
    switch (enMmaServiceType)
    {
        case MMA_MMC_PLMN_SELECTION_RESULT_NORMAL_SERVICE:
             return TAF_PH_PLMN_SELECTION_RESULT_NORMAL_SERVICE;

        case MMA_MMC_PLMN_SELECTION_RESULT_LIMITED_SERVICE:
             return TAF_PH_PLMN_SELECTION_RESULT_LIMITED_SERVICE;

        case MMA_MMC_PLMN_SELECTION_RESULT_NO_SERVICE:
             return TAF_PH_PLMN_SELECTION_RESULT_NO_SERVICE;
             
        case MMA_MMC_PLMN_SELECTION_RESULT_INTERRUPT:
             return TAF_PH_PLMN_SELECTION_RESULT_INTERRUPT;

        default:
            MN_ERR_LOG("TAF_MMA_ConvertServiceTypeToAtType: The input is invalid");
            return TAF_PH_PLMN_SELECTION_RESULT_BUTT;
    }
}


VOS_UINT32 TAF_MMA_ProcPlmnSelectStartInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MMA_PLMN_SELECTION_START_IND_STRU                  *pstPlmnStartInd    = VOS_NULL_PTR;  
    TAF_PH_PLMN_SELECTION_INFO_STRU                         stTafPlmnSelectInfo;   
    
    pstPlmnStartInd = (MMC_MMA_PLMN_SELECTION_START_IND_STRU *)pstMsg;
    PS_MEM_SET(&stTafPlmnSelectInfo, 0, sizeof(TAF_PH_PLMN_SELECTION_INFO_STRU));

    if (MMA_MMC_PLMN_SELECTION_START != pstPlmnStartInd->enPlmnSeleStartFlag)
    {
        MN_ERR_LOG("TAF_MMA_ProcPlmnSelectStartInd: The input is invalid");
        return VOS_TRUE;
    }
  
    /* ����ϱ��¼� */
    stTafPlmnSelectInfo.ulPlmnSelectRlst = TAF_PH_PLMN_SELECTION_RESULT_BUTT;
    stTafPlmnSelectInfo.ulPlmnSelectFlag = TAF_PH_PLMN_SELECTION_START;

    /* �����Ϣͷ */
    stTafPlmnSelectInfo.PhoneEvent       = TAF_MMA_EVT_PLMN_SELECTION_INFO_IND;
    stTafPlmnSelectInfo.OpId             = MMA_OP_ID_INTERNAL;
    stTafPlmnSelectInfo.ClientId         = MMA_CLIENTID_BROADCAST;

    /* ���õ绰�����ϱ����� */
    MN_PH_SendMsg(stTafPlmnSelectInfo.ClientId,(VOS_UINT8*)&stTafPlmnSelectInfo,sizeof(TAF_PH_PLMN_SELECTION_INFO_STRU));
    
    return VOS_TRUE;
}




VOS_UINT32 MN_MMA_ProcPlmnSelectionRlstInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MMA_PLMN_SELECTION_RLST_IND_STRU                    *pstPlmnSelRslt;

    TAF_PH_PLMN_SELECTION_INFO_STRU     stTafPlmnSelectInfo;   

    pstPlmnSelRslt = (MMC_MMA_PLMN_SELECTION_RLST_IND_STRU*)pstMsg; 

    PS_MEM_SET(&stTafPlmnSelectInfo, 0, sizeof(TAF_PH_PLMN_SELECTION_INFO_STRU));


    /* RoamingBroker����ʹ������SIM������Location�¼� */
    if (VOS_TRUE == MN_MMA_GetRoamingBrokerFlg())
    {
        if ((MMA_MMC_PLMN_SELECTION_RESULT_LIMITED_SERVICE == pstPlmnSelRslt->enResult)
         || (MMA_MMC_PLMN_SELECTION_RESULT_NO_SERVICE == pstPlmnSelRslt->enResult))
        {
            NAS_MMA_ReportLociStatus();
        }
    }
    /* ����������һ�����綼û����������UE�����޷��񣬶����ڿ�����ʼ��ʱ
       ����״̬Ҳ���޷������ͨ������״̬�ı��޷���location status�¼���
       ��Ҫ�ڴ˴���һ��,��AT&T��֤������֤�˳��� */
    else
    {
        if ((VOS_FALSE == g_StatusContext.ucIsReportedLocStatus)
         && (MMA_MMC_PLMN_SELECTION_RESULT_NO_SERVICE == pstPlmnSelRslt->enResult))
        {
            NAS_MMA_ReportLociStatus();
        }
    }

    /* ����ϱ��¼� */
    stTafPlmnSelectInfo.ulPlmnSelectRlst = TAF_MMA_ConvertServiceTypeToAtType(pstPlmnSelRslt->enResult);

    if (TAF_PH_PLMN_SELECTION_RESULT_INTERRUPT == stTafPlmnSelectInfo.ulPlmnSelectRlst)
    {
        stTafPlmnSelectInfo.ulPlmnSelectFlag = TAF_PH_PLMN_SELECTION_INTERRUPT;
    }
    else
    {
        stTafPlmnSelectInfo.ulPlmnSelectFlag = TAF_PH_PLMN_SELECTION_END;
    }

    /* �����Ϣͷ */
    stTafPlmnSelectInfo.PhoneEvent       = TAF_MMA_EVT_PLMN_SELECTION_INFO_IND;
    stTafPlmnSelectInfo.OpId             = MMA_OP_ID_INTERNAL;
    stTafPlmnSelectInfo.ClientId         = MMA_CLIENTID_BROADCAST;

    /* ���õ绰�����ϱ����� */
    MN_PH_SendMsg(stTafPlmnSelectInfo.ClientId,(VOS_UINT8*)&stTafPlmnSelectInfo,sizeof(TAF_PH_PLMN_SELECTION_INFO_STRU));
    
    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : MN_PH_GetRegCostTime
 ��������  : ���ص�ǰ���������ɹ�ע�Ứ�ѵ�ʱ��
 �������  : ��
 �������  : VOS_UINT32:��������ע�Ứ�ѵ�ʱ��,��λΪs
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2010��9��8��
    ��    ��   : zhoujun /40661
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32  MN_PH_GetRegCostTime( VOS_VOID )
{
    return g_stRegTimeInfo.ulCostTime;
}



TAF_PH_MODE MMA_GetCurrPhMode(VOS_VOID)
{
    return TAF_SDC_GetCurPhoneMode();
}




VOS_VOID TAF_MMA_ReportPlmnModeEvent(VOS_UINT8 ucPlmnMode)
{
    TAF_PHONE_EVENT_INFO_STRU          *pstPhoneEvent = VOS_NULL_PTR;

    pstPhoneEvent = (TAF_PHONE_EVENT_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMA, sizeof(TAF_PHONE_EVENT_INFO_STRU));

    if (VOS_NULL_PTR == pstPhoneEvent)
    {
        MMA_ERRORLOG("TAF_MMA_ReportPlmnModeEvent:ERROR:ALLOC MEMORY FAIL.");
        return;
    }

    PS_MEM_SET(pstPhoneEvent, 0, sizeof(TAF_PHONE_EVENT_INFO_STRU));

    pstPhoneEvent->OP_PlmnMode  = VOS_TRUE;
    pstPhoneEvent->PhoneEvent   = TAF_PH_EVT_NSM_STATUS_IND;
    pstPhoneEvent->ucPlmnMode   = ucPlmnMode;

    Taf_PhoneEvent(pstPhoneEvent);

    PS_MEM_FREE(WUEPS_PID_MMA, pstPhoneEvent);

    return;
}


VOS_UINT32 TAF_MMA_IsEnablePlmnList(VOS_VOID)
{
    TAF_MMA_AUTO_PLMN_SEL_USER_ENUM_UINT8 enAutoPlmnSelUser;

    enAutoPlmnSelUser = TAF_MMA_GetAutoPlmnSelUser();

    if (STA_FSM_ENABLE == g_StatusContext.ulFsmState)
    {
        return VOS_TRUE;
    }

    /* refresh�������Զ����� */
    if ((STA_FSM_PLMN_RESEL == g_StatusContext.ulFsmState)
     && (TAF_MMA_AUTO_PLMN_SEL_USER_USIM_REFRESH == enAutoPlmnSelUser))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}
VOS_UINT32 TAF_MMA_IsEnablePlmnSelect(VOS_VOID)
{
    TAF_MMA_PLMN_MODE_ENUM_UINT8        enPlmnMode;

    enPlmnMode = TAF_MMA_GetPlmnMode_Refresh();

    /* ֻ����ENABLE״̬����������û��disable������ѡ��˵������ܽ���ָ������ */
    if ((STA_FSM_ENABLE == g_StatusContext.ulFsmState)
     && (TAF_MMA_PLMN_MODE_ENUM_ENABLE == enPlmnMode))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}
VOS_UINT32 TAF_MMA_IsEnableDeAttach(VOS_VOID)
{
    TAF_MMA_AUTO_PLMN_SEL_USER_ENUM_UINT8 enAutoPlmnSelUser;

    enAutoPlmnSelUser = TAF_MMA_GetAutoPlmnSelUser();

    if (STA_FSM_ENABLE == g_StatusContext.ulFsmState)
    {
        return VOS_TRUE;
    }

    if (STA_FSM_DE_ATTACH == g_StatusContext.ulFsmState)
    {
        return VOS_TRUE;
    }

    /* refresh�������Զ����� */
    if ((STA_FSM_PLMN_RESEL == g_StatusContext.ulFsmState)
     && (TAF_MMA_AUTO_PLMN_SEL_USER_USIM_REFRESH == enAutoPlmnSelUser))
    {
        return VOS_TRUE;
    }

    /* refresh������SYSCFG�����յ�CNF֮����ܻᴥ��attach��
       ��ʱ�û��·�attach����detach�����ܴ��SYSCFG��
       ����MMAĿǰû�л�����ƣ�����ֱ�Ӹ��û�����ERROR��
    */

    return VOS_FALSE;
}


VOS_UINT32 TAF_MMA_SndAutoPlmnReselect_Refresh(VOS_VOID)
{
    VOS_UINT32                          ulRet;

    ulRet = TAF_MMA_SndMmcPlmnUserReselReq(MMA_MMC_PLMN_SEL_MODE_AUTO);

    if (VOS_ERR == ulRet)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMA, "TAF_MMA_SndAutoPlmnReselect_Refresh(): send msg fail");
        return VOS_ERR;
    }

    /* ��¼PLMN�������� */
    g_StatusContext.ucReselMode   = MMA_MMC_PLMN_SEL_MODE_AUTO;

    /* ��¼��ǰ����״̬ */
    g_StatusContext.ulPreFsmState = g_StatusContext.ulFsmState;

    /* ������״̬ */
    g_StatusContext.ulFsmState    = STA_FSM_PLMN_RESEL;

    /* ������״̬ */
    g_StatusContext.aucFsmSubFlag[0]         = STA_FSMSUB_PLMN_RESEL_AUTO;
    g_StatusContext.aFsmSub[0].ucFsmStateSub = STA_FSMSUB_PLMN_RESEL_AUTO;
    g_StatusContext.aFsmSub[0].TimerName     = STA_TIMER_PLMN_RESEL;

    /* ����PLMN_RESEL״̬��ʱ�� */
    ulRet = NAS_StartRelTimer(&g_StatusContext.ahStaTimer[0],
                              WUEPS_PID_MMA,
                              aStaTimerLen[STA_TIMER_PLMN_RESEL],
                              TAF_MMA,
                              MMA_MAKELPARAM(0, STA_TIMER_PLMN_RESEL),
                              VOS_RELTIMER_NOLOOP
                              );
    if (VOS_OK != ulRet)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMA, "TAF_MMA_SndAutoPlmnReselect_Refresh(): start timer fail");
        return VOS_ERR;
    }

    return VOS_OK;
}
VOS_UINT32 TAF_MMA_RcvMmcPlmnReselCnf_Refresh(MMC_MMA_PLMN_RESEL_CNF_STRU *pstMsg)
{
    VOS_UINT32                          ulRet;

    /*�ر�״̬��ʱ��*/
    ulRet = NAS_StopRelTimer(WUEPS_PID_MMA,
                             g_StatusContext.aFsmSub[0].TimerName,
                             &g_StatusContext.ahStaTimer[0]);

    if ( VOS_OK != ulRet )
    {
        STA_TRACE (STA_WARNING_PRINT,
            "TAF_MMA_RcvMmcPlmnReselCnf_Refresh():WARNING:NAS_StopRelTimer failed");
    }

    ulRet = Sta_UpdateFsmFlag(STA_FSMSUB_NULL, STA_FSM_ENABLE, STA_UPDATE_MONO_CLEAR);
    if (STA_ERROR == ulRet)
    {
        STA_TRACE(STA_WARNING_PRINT, "TAF_MMA_RcvMmcPlmnReselCnf_Refresh():WARNING:Sta_UpdateFsmFlag failed\n");
    }

    return VOS_OK;
}


VOS_UINT32 TAF_MMA_RcvMmcPlmnReselCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MMA_PLMN_RESEL_CNF_STRU        *pstPlmnReselCnfMsg;

    pstPlmnReselCnfMsg = (MMC_MMA_PLMN_RESEL_CNF_STRU*)pstMsg;

    TAF_MMA_SetPlmnSelectionMode(TAF_PH_PLMN_SEL_MODE_AUTO);

    /* �����6F15�ļ�refresh�������Զ����� */
    if (TAF_MMA_AUTO_PLMN_SEL_USER_USIM_REFRESH == TAF_MMA_GetAutoPlmnSelUser())
    {
        /* MMC��MMA�ظ�ָ���Ѻ��Զ��Ѷ���D_MMC_PLMN_SEL_CNF��Ϣ��refreshֻ�ᴥ���Զ����� */
        if (STA_FSM_PLMN_RESEL != g_StatusContext.ulFsmState)
        {
            return VOS_TRUE;
        }

        TAF_MMA_RcvMmcPlmnReselCnf_Refresh(pstPlmnReselCnfMsg);
    }
    /* AT������Զ�������ָ����������ԭ������ */
    else
    {
        Sta_SelPlmnSpecializeCnf(pstPlmnReselCnfMsg->enResult);
    }


    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvMmcPlmnSpecialSelCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MMA_PLMN_SPECIAL_SEL_CNF_STRU  *pstPlmnSpecSelCnfMsg;

    pstPlmnSpecSelCnfMsg = (MMC_MMA_PLMN_SPECIAL_SEL_CNF_STRU*)pstMsg;

    /* svlte nv��������ػ�״̬����cops */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
        TAF_MMA_SetPlmnSelectionMode(MMA_MMC_PLMN_SEL_MODE_MANUAL);
        
        g_StatusContext.ucOperateType = STA_OP_REGPLMN;
        Sta_EventReport(g_StatusContext.ulCurOpId, TAF_PH_EVT_PLMN_SEL_CNF);

        return VOS_TRUE;
    }

    TAF_MMA_SetPlmnSelectionMode(TAF_PH_PLMN_SEL_MODE_MANUAL);

    Sta_SelPlmnSpecializeCnf(pstPlmnSpecSelCnfMsg->enResult);


    return VOS_TRUE;
}


VOS_VOID TAF_MMA_StopStatusCtxTimer(VOS_VOID)
{
    VOS_UINT32                            ulRet;

    /* �رյ�ǰ״̬�Ķ�ʱ������*/
    if (VOS_NULL != g_StatusContext.ahStaTimer[0])
    {
        ulRet = NAS_StopRelTimer(WUEPS_PID_MMA,
                                 g_StatusContext.aFsmSub[0].TimerName,
                                 &g_StatusContext.ahStaTimer[0]);
        if ( VOS_OK != ulRet )
        {
            STA_TRACE (STA_WARNING_PRINT,
                "TAT_MMA_ProcPhoneStop_NotEnableStatus():WARNING:NAS_StopRelTimer failed  ");
        }
        g_StatusContext.ahStaTimer[0] = VOS_NULL;
    }

    if ( VOS_NULL != g_StatusContext.ahStaTimer[1] )
    {
        ulRet = NAS_StopRelTimer(WUEPS_PID_MMA,
                                 g_StatusContext.aFsmSub[1].TimerName,
                                 &g_StatusContext.ahStaTimer[1]);


        if ( VOS_OK != ulRet )
        {
            STA_TRACE (STA_WARNING_PRINT,
                "TAT_MMA_ProcPhoneStop_NotEnableStatus():WARNING:NAS_StopRelTimer failed  ");
        }
        g_StatusContext.ahStaTimer[1] = VOS_NULL;
    }
}
VOS_UINT32 TAF_MMA_ProcPhoneStop_NotEnableStatus(VOS_VOID)
{
    VOS_UINT32                            ulRet;
    VOS_UINT32                            ulIsRefreshAutoPlmnSel;
    VOS_UINT32                            ulIsRefreshSyscfg;
    TAF_MMA_AUTO_PLMN_SEL_USER_ENUM_UINT8 enAutoPlmnSelUser;
    TAF_MMA_SYSCFG_USER_ENUM_UINT8        enSyscfgUser;

    enAutoPlmnSelUser = TAF_MMA_GetAutoPlmnSelUser();
    enSyscfgUser      = TAF_MMA_GetSyscfgUser();

    ulIsRefreshAutoPlmnSel = VOS_FALSE;
    ulIsRefreshSyscfg      = VOS_FALSE;

    /* refresh�������Զ����� */
    if ((STA_FSM_PLMN_RESEL == g_StatusContext.ulFsmState)
     && (TAF_MMA_AUTO_PLMN_SEL_USER_USIM_REFRESH == enAutoPlmnSelUser))
    {
        ulIsRefreshAutoPlmnSel = VOS_TRUE;
    }

    /* refresh������SYSCFG*/
    if ((STA_FSM_SYS_CFG_SET == g_StatusContext.ulFsmState)
     && (TAF_MMA_SYSCFG_USER_USIM_REFRESH == enSyscfgUser))
    {
        ulIsRefreshSyscfg = VOS_TRUE;
    }

    /* �����Refresh�������Զ�������SYSCFG������Ҫ�ϱ���Ӧ�¼�֪ͨAPP*/
    if ((VOS_TRUE != ulIsRefreshAutoPlmnSel)
     && (VOS_TRUE != ulIsRefreshSyscfg))
    {
        /* ��ֹ��ǰ��������̣��ϱ���Ӧ�¼�֪ͨAPP */
        ulRet = Sta_MsgReqAbort (STA_MSGABORT_ERR);
        if (STA_ERROR == ulRet)
        {
            NAS_WARNING_LOG(WUEPS_PID_MMA,
                "TAF_MMA_ProcPhoneStop_NotEnableStatus(): different request abort failed  ");
        }
    }

    TAF_MMA_StopStatusCtxTimer();

    return VOS_OK;
}


VOS_VOID TAF_MMA_DeleteRatType(
    TAF_PH_RAT_TYPE_ENUM_UINT8           ucRatType,
    MMA_MMC_PLMN_RAT_PRIO_STRU          *pstPlmnRatPrioList
)
{
    VOS_UINT32                          i;

    MMA_MMC_PLMN_RAT_PRIO_STRU          stOldPlmnRatPrioList;

    /* ��¼����Ľ��뼼���б� */
    PS_MEM_CPY(&stOldPlmnRatPrioList, pstPlmnRatPrioList, sizeof(MMA_MMC_PLMN_RAT_PRIO_STRU));


    pstPlmnRatPrioList->ucRatNum = 0;

    /* ���뼼����ʼ��ΪBUTT */
    for (i = 0 ; i < MMA_MMC_MAX_RAT_NUM; i++)
    {
        pstPlmnRatPrioList->aucRatPrio[i] = TAF_SDC_SYS_MODE_BUTT;
    }


    if (stOldPlmnRatPrioList.ucRatNum > MMA_MMC_MAX_RAT_NUM)
    {
        stOldPlmnRatPrioList.ucRatNum = MMA_MMC_MAX_RAT_NUM;
    }

    for (i = 0 ; i < stOldPlmnRatPrioList.ucRatNum; i++)
    {
        if (ucRatType == stOldPlmnRatPrioList.aucRatPrio[i])
        {
            continue;
        }
        pstPlmnRatPrioList->aucRatPrio[pstPlmnRatPrioList->ucRatNum] = stOldPlmnRatPrioList.aucRatPrio[i];
        pstPlmnRatPrioList->ucRatNum++;
    }

    return;
}

VOS_VOID TAF_MMA_GetAvailablePlmnRatPrio(MMA_MMC_PLMN_RAT_PRIO_STRU *pstPlmnRatPrioList)
{

    VOS_UINT32                          ulGsmAccessForbidden;
#if (FEATURE_ON == FEATURE_LTE)
    VOS_UINT8                           ucCardType;
    VOS_UINT32                          ulGsmForbidden;
    VOS_UINT8                           i;

    VOS_UINT8                           ucRatBalancingEnableFlg;

    ucRatBalancingEnableFlg = TAF_MMA_GetRatBalancingEnableFlg();
#endif


    ulGsmAccessForbidden = TAF_MMA_IsGsmAccessForbidden();

    /* ���UE��֧��GSM���룬�����÷�G��ģȥ��Gģ�������G��ģ����MMC�����ṩ���Ʒ��� */
    if ((VOS_FALSE == MN_MMA_IsGOnlyMode((TAF_PH_RAT_ORDER_STRU *)pstPlmnRatPrioList))
     && (VOS_TRUE == ulGsmAccessForbidden))
    {
        TAF_MMA_DeleteRatType(TAF_PH_RAT_GSM, pstPlmnRatPrioList);
    }

#if (FEATURE_ON == FEATURE_LTE)
    /* SIM��У��ȥ��Lģ */
    NAS_USIMMAPI_GetCardType(VOS_NULL_PTR, &ucCardType) ;

    /* SIM��ʱ�������LTE only����ɾ��Lģ */
    if ( (USIMM_CARD_SIM == ucCardType)
      && (VOS_TRUE == ucRatBalancingEnableFlg) )
    {
        TAF_MMA_DeleteRatType(TAF_PH_RAT_LTE, pstPlmnRatPrioList);
    }

    /* �������Ϊ0,����ǰ������L��ģҲ�����ֹʱ,Ĭ��ΪGWģ */
    if (0 == pstPlmnRatPrioList->ucRatNum)
    {
        i = 0;

        if (VOS_TRUE == TAF_SDC_IsPlatformSupportUtran())
        {
            pstPlmnRatPrioList->aucRatPrio[i] = TAF_PH_RAT_WCDMA;
            i ++;
        }

        ulGsmForbidden = MMA_IsGsmForbidden();
        if ((VOS_TRUE == TAF_SDC_IsPlatformSupportGsm())
         && ((MMA_FALSE == ulGsmForbidden)))
        {
            pstPlmnRatPrioList->aucRatPrio[i] = TAF_PH_RAT_GSM;
            i ++;
        }
        pstPlmnRatPrioList->ucRatNum      = i;
    }
#endif

    return;

}


VOS_VOID TAF_MMA_RcvSyscfgCnfFail(VOS_VOID)
{
    VOS_UINT32                          ulResult;

    if (TAF_MMA_SYSCFG_USER_USIM_REFRESH != TAF_MMA_GetSyscfgUser())
    {
        return;
    }

    /* TI_TAF_MMA_PERIOD_TRYING_RAT_BALANCING��ʱ���������У����� */
    if (MMA_TIMER_RUN == TAF_MMA_GetTiTryingPeriodRatBalancingStatus())
    {
        return;
    }

    /* �����4F36refresh������SYSCFGʧ�ܣ���Ҫ����TI_TAF_MMA_PERIOD_TRYING_RAT_BALANCING��ʱ�� */
    ulResult = NAS_StartRelTimer(&(g_StatusContext.stRatBalancingCtx.stTiPeriodTryingRatBalancing.MmaHTimer),
                                 WUEPS_PID_MMA,
                                 TI_TAF_MMA_PERIOD_TRYING_RAT_BALANCING_LEN,
                                 TAF_MMA,
                                 TI_TAF_MMA_PERIOD_TRYING_RAT_BALANCING,
                                 VOS_RELTIMER_NOLOOP );

    if (VOS_OK != ulResult)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMA, "TAF_MMA_RcvSyscfgCnfFail(): Start Timer Fail");
    }
    else
    {
        TAF_MMA_SetTiTryingPeriodRatBalancingStatus(MMA_TIMER_RUN);
    }

    return;
}



VOS_UINT8 TAF_MMA_IsModeChange(
    TAF_SDC_SYS_MODE_ENUM_UINT8         enCurNetWork,
    VOS_UINT8                           ucSysSubMode
)
{
    VOS_UINT8                           ucModeChangeFlg;
    NAS_UTRANCTRL_UTRAN_MODE_ENUM_UINT8 enCurrUtranMode;

    enCurrUtranMode = NAS_UTRANCTRL_GetCurrUtranMode();

    ucModeChangeFlg = VOS_FALSE;

    /* ������뼼������ͬ����Ҫ�ϱ�Mode */
    if (enCurNetWork != TAF_SDC_GetSysMode())
    {
        ucModeChangeFlg = VOS_TRUE;
#ifndef __PS_WIN32_RECUR__
#endif
    }
    /* ���뼼��ΪGʱ��ϵͳ��ģʽ��ͬʱ����Ҫ�ϱ�Mode */
    else if (TAF_SDC_SYS_MODE_GSM == enCurNetWork)
    {
        if (ucSysSubMode != TAF_SDC_GetSysSubMode())
        {
            ucModeChangeFlg = VOS_TRUE;
        }
    }

    /* ��ǰ����ΪWCDMAʱ���п���TD->W��W->TD,��Ҫ��һ���ж�UtranMode
       ���UtranMode�����仯����Ҫ�ϱ�Mode */
    else if (TAF_SDC_SYS_MODE_WCDMA == enCurNetWork)
    {
        if (enCurrUtranMode != g_StatusContext.ucPreUtranMode)
        {
            ucModeChangeFlg             = VOS_TRUE;
        }
    }
    else
    {
    }


    return ucModeChangeFlg;

}


VOS_UINT32 TAF_MMA_RcvMmcUserPlmnRej(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRet;

    /*�ر�״̬��ʱ��*/
    ulRet = NAS_StopRelTimer(WUEPS_PID_MMA,
                             g_StatusContext.aFsmSub[0].TimerName,
                             &g_StatusContext.ahStaTimer[0]);


    if ( VOS_OK != ulRet )
    {
        STA_TRACE (STA_WARNING_PRINT,
            "TAF_MMA_RcvMmcUserPlmnRej():WARNING:NAS_StopRelTimer failed");
    }


    /* �ϱ���ǰָ������ʧ�� */
    g_StatusContext.PhoneError    = TAF_ERR_CMD_TYPE_ERROR;

    g_StatusContext.ucOperateType = STA_OP_PHONEERROR;
    MMA_NORMAILLOG("NAS_MN_ReportPlmnSrchResult():NORMAIL:SPECIALIZE THE PLMN FAILURE!");

    /* �ϱ�APP��ǰ״̬�¼� */
    Sta_EventReport (g_StatusContext.ulCurOpId, TAF_PH_EVT_PLMN_SEL_CNF);
    STA_TRACE(STA_NORMAL_PRINT, "NAS_MN_ReportPlmnSrchResult: TAF_PH_EVT_PLMN_SEL_CNF reported to AT!");

    ulRet = Sta_UpdateFsmFlag(STA_FSMSUB_NULL, STA_FSM_ENABLE, STA_UPDATE_MONO_CLEAR);
    if (STA_ERROR == ulRet)
    {
        STA_TRACE(STA_WARNING_PRINT, "Sta_SelPlmnSpecializeCnf():WARNING:Sta_UpdateFsmFlag failed\n");
    }

    return VOS_TRUE;

}
VOS_VOID  TAF_MMA_RcvTimerPlmnListCnfExpired( VOS_UINT32 ulOpId )
{
    VOS_UINT32                          ulRet;

    /* ��ǰ״̬����PLMN LIST״̬��ֱ�ӷ��� */
    if ( STA_FSM_PLMN_LIST != g_StatusContext.ulFsmState )
    {
        STA_TRACE(STA_WARNING_PRINT, "TAF_MMA_RcvUserAbortPlmnList():WARNING: no plmn list searching");
        return;
    }

    /* MMA��LIST����������ʱ����ʱ��������MMC����ֹͣLIST��������
       �յ�MMC�Ļظ�������AT�ظ� */
    ulRet = TAF_MMA_SndMmcPlmnListAbortReq();

    if (VOS_ERR == ulRet)
    {
        STA_TRACE(STA_ERROR_PRINT, "TAF_MMA_AbortPlmnList():ERROR: failed");
    }


    /* ����STA_TIMER_PLMN_LIST_ABORT 10s��ʱ���������ʱδ�յ�MMC�Ļظ����ϱ�ATʧ�� */
    g_StatusContext.aucFsmSubFlag[0]         = STA_FSMSUB_MONO;
    g_StatusContext.aFsmSub[0].ucFsmStateSub = STA_FSMSUB_MONO;
    g_StatusContext.aFsmSub[0].TimerName     = STA_TIMER_PLMN_LIST_ABORT;
    g_StatusContext.aFsmSub[0].ulOpId        = ulOpId;
    if (VOS_OK != NAS_StartRelTimer(&g_StatusContext.ahStaTimer[0],
                                     WUEPS_PID_MMA,
                                     aStaTimerLen[STA_TIMER_PLMN_LIST_ABORT],
                                     TAF_MMA,
                                     MMA_MAKELPARAM(0, STA_TIMER_PLMN_LIST_ABORT),
                                     VOS_RELTIMER_NOLOOP))
    {
        STA_TRACE(STA_ERROR_PRINT, "TAF_MMA_AbortPlmnList():ERROR: VOS_StartRelTimer runs failed");
    }


    /* ���õ�ǰ���ڴ���ʱ����ʱ���б��Ѵ�Ϲ��� */
    g_StatusContext.ucPlmnListAbortProc = TAF_MMA_PLMN_LIST_ABORT_PROCESSING_TIMEOUT;

    return;
}



VOS_VOID  TAF_MMA_RcvUserAbortPlmnList( VOS_UINT32 ulOpId )
{
    VOS_UINT32                          ulRet;

    /* MMA�ϱ�PLMN LIST CNF���б��Ѵ�϶Գ�ʱ����ǰ״̬�Ѳ���PLMN LIST״̬��
       ֱ�Ӹ�AT�ظ�TAF_PH_EVT_PLMN_LIST_ABORT_CNF */
    if ( STA_FSM_PLMN_LIST != g_StatusContext.ulFsmState )
    {
        STA_TRACE(STA_WARNING_PRINT, "TAF_MMA_RcvUserAbortPlmnList():WARNING: no plmn list searching");

        g_StatusContext.ucOperateType = STA_OP_NULL;

        /* �ϱ�AT��ǰ״̬�¼� */
        Sta_EventReport (g_StatusContext.ulCurOpId, TAF_PH_EVT_PLMN_LIST_ABORT_CNF);
        return;
    }

    /* ��ʱ����ʱ����б��Ѻ��û�����б��ѶԳ�ʱ�����û����Ϊ׼ */
    if ( TAF_MMA_PLMN_LIST_ABORT_PROCESSING_TIMEOUT == g_StatusContext.ucPlmnListAbortProc )
    {
        STA_TRACE(STA_WARNING_PRINT, "TAF_MMA_RcvUserAbortPlmnList():WARNING: plmn list abort processing");

        /* ���õ�ǰ���ڴ����û�����б��ѹ��� */
        g_StatusContext.ucPlmnListAbortProc = TAF_MMA_PLMN_LIST_ABORT_PROCESSING_USER_ABORT;

        return;
    }

    /* ͣ�б��Ѷ�ʱ�� */
    if (VOS_OK != NAS_StopRelTimer(WUEPS_PID_MMA,
             STA_TIMER_PLMN_LIST,
             &g_StatusContext.ahStaTimer[0]))
    {
        STA_TRACE(STA_ERROR_PRINT, "TAF_MMA_RcvUserAbortPlmnList():WARNING: NAS_StopRelTimer failed");
    }

    /* MMA��LIST����������ʱ����ʱ��������MMC����ֹͣLIST��������
       �յ�MMC�Ļظ�������AT�ظ� */

    ulRet = TAF_MMA_SndMmcPlmnListAbortReq();
    if (VOS_ERR == ulRet)
    {
        STA_TRACE(STA_ERROR_PRINT, "TAF_MMA_RcvUserAbortPlmnList():ERROR: failed");
    }


    /* ����STA_TIMER_PLMN_LIST_ABORT 10s��ʱ���������ʱδ�յ�MMC�Ļظ����ϱ�ATʧ�� */
    g_StatusContext.aucFsmSubFlag[0]         = STA_FSMSUB_MONO;
    g_StatusContext.aFsmSub[0].ucFsmStateSub = STA_FSMSUB_MONO;
    g_StatusContext.aFsmSub[0].TimerName     = STA_TIMER_PLMN_LIST_ABORT;
    g_StatusContext.aFsmSub[0].ulOpId        = ulOpId;
    if (VOS_OK != NAS_StartRelTimer(&g_StatusContext.ahStaTimer[0],
                                     WUEPS_PID_MMA,
                                     aStaTimerLen[STA_TIMER_PLMN_LIST_ABORT],
                                     TAF_MMA,
                                     MMA_MAKELPARAM(0, STA_TIMER_PLMN_LIST_ABORT),
                                     VOS_RELTIMER_NOLOOP))
    {
        STA_TRACE(STA_ERROR_PRINT, "TAF_MMA_RcvUserAbortPlmnList():ERROR: VOS_StartRelTimer runs failed");
    }


    /* ���õ�ǰ���ڽ����û�����б��ѹ��� */
    g_StatusContext.ucPlmnListAbortProc = TAF_MMA_PLMN_LIST_ABORT_PROCESSING_USER_ABORT;

    return;
}



VOS_VOID  TAF_MMA_RcvTimerPlmnListAbortCnfExpired(
    VOS_UINT16                          usTimerIndex
)
{
    if (TAF_MMA_PLMN_LIST_ABORT_BUTT == g_StatusContext.ucPlmnListAbortProc)
    {
        STA_TRACE(STA_WARNING_PRINT, "TAF_MMA_RcvTimerPlmnListAbortCnfExpired():WARNING: no plmn list abort");
        return;
    }

    g_StatusContext.aFsmSub[usTimerIndex].ulOpId        = STA_OPID_INVALID;
    g_StatusContext.aFsmSub[usTimerIndex].ucFsmStateSub = STA_FSMSUB_NULL;
    g_StatusContext.aFsmSub[usTimerIndex].TimerName     = STA_TIMER_NULL;
    g_StatusContext.aucFsmSubFlag[usTimerIndex]         = STA_FSMSUB_NULL;

    /* PLMN LIST ABORTԭ��ֵΪ��ʱ����ʱ */
    if ( TAF_MMA_PLMN_LIST_ABORT_PROCESSING_TIMEOUT == g_StatusContext.ucPlmnListAbortProc )
    {
        /* ��AT�ظ��б��ѳ�ʱ��� */
        g_StatusContext.PhoneError    = TAF_ERR_TIME_OUT;
        g_StatusContext.ucOperateType = STA_OP_PHONEERROR;

        /* �ϱ�AT��ǰ״̬�¼� */
        Sta_EventReport (g_StatusContext.ulCurOpId, TAF_PH_EVT_PLMN_LIST_CNF);
    }
    else
    {
        g_StatusContext.ucOperateType = STA_OP_NULL;

        /* �ϱ�AT��ǰ״̬�¼� */
        Sta_EventReport (g_StatusContext.ulCurOpId, TAF_PH_EVT_PLMN_LIST_ABORT_CNF);
    }

    /* Ǩ��״̬��ENABLE״̬ */
    g_StatusContext.ulFsmState = STA_FSM_ENABLE;

    /* ���õ�ǰû�д����û��б��Ѵ�Ϲ��� */
    g_StatusContext.ucPlmnListAbortProc = TAF_MMA_PLMN_LIST_ABORT_BUTT;

    return;
}



VOS_UINT32 TAF_MMA_IsGsmAccessForbidden(VOS_VOID)
{
    VOS_UINT32   ulEfustGsmAccess;
    VOS_UINT32   ulEfustGsmSecContext;
    VOS_UINT8    ucSimStatus;
    VOS_UINT8    ucSimType;

    if (MMA_QUICK_START_ENABLE == gstMmaValue.ulQuickStartFlg)
    {
        return VOS_FALSE;
    }

    if (VOS_TRUE == gstMmaValue.stEfustServiceCfg.ucForbidReg2GNetWork)
    {
        /*��ȡ��״̬�Ϳ�����*/
        MMA_PhoneGetUsimStatus(&ucSimStatus);
        MMA_PhoneGetUsimType(&ucSimType);

        if ( ( MMA_USIM_PRESENT == ucSimStatus )
            && (MMA_USIM_TYPE_USIM == ucSimType) )
        {
            /* ����USIM���ļ���EFust����27��38���ж��Ƿ�����GSM���� */
            ulEfustGsmAccess     = NAS_USIMMAPI_IsServiceAvailable(NAS_USIM_SVR_GSM_ACCESS_IN_USIM);
            ulEfustGsmSecContext = NAS_USIMMAPI_IsServiceAvailable(NAS_USIM_SVR_GSM_SECURITY_CONTEXT);            

            if ((0 == ulEfustGsmAccess) && (0 == ulEfustGsmSecContext))
            {
                return VOS_TRUE;
            }
        }

    }

    return VOS_FALSE;
}

/* TAF_MMA_InitTimerLen�ƶ�λ�� */

VOS_UINT32 TAF_MMA_IsExistPlmnRatAllowAccess(VOS_VOID)
{
    TAF_PH_RAT_ORDER_STRU              *pstRatPrioList = VOS_NULL_PTR;
    VOS_UINT32                          ulGOnlyModeFlg;
#if (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                          ulLOnlyModeFlg;
    VOS_UINT8                           ucCardType;
#endif

    ulGOnlyModeFlg = VOS_FALSE;
    pstRatPrioList = MN_MMA_GetRatPrioListAddr();
    ulGOnlyModeFlg = MN_MMA_IsGOnlyMode(pstRatPrioList);

#if (FEATURE_ON == FEATURE_LTE)
    ulLOnlyModeFlg = VOS_FALSE;
    NAS_USIMMAPI_GetCardType(VOS_NULL_PTR, &ucCardType);
    ulLOnlyModeFlg = MN_MMA_IsLOnlyMode(pstRatPrioList);
#endif

    /* USIM����ֹGSM���벢����G��ģʱ���������û�ָ���� */
    if ((VOS_TRUE == TAF_MMA_IsGsmAccessForbidden())
     && (VOS_TRUE == ulGOnlyModeFlg))
    {
        return VOS_FALSE;
    }

#if (FEATURE_ON == FEATURE_LTE)
    /* SIM��������L��ģʱ��Ҳ�������û�ָ���� */
    if ((USIMM_CARD_SIM == ucCardType)
     && (VOS_TRUE == ulLOnlyModeFlg))
    {
        return VOS_FALSE;
    }
#endif

    return VOS_TRUE;

}


VOS_VOID TAF_MMA_ReportRegStatus(
    MMA_MMC_REG_STATE_ENUM_UINT8        enRegState,
    MMA_MMC_SRVDOMAIN_ENUM_UINT32       enCnDomainId
)
{
    TAF_PHONE_EVENT_INFO_STRU          *pstPhoneEvent = VOS_NULL_PTR;
    TAF_SDC_PLMN_ID_STRU               *pstPlmnId     = VOS_NULL_PTR;

    pstPlmnId     = TAF_SDC_GetCurrCampPlmnId();
    pstPhoneEvent = (TAF_PHONE_EVENT_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMA,
                                        sizeof(TAF_PHONE_EVENT_INFO_STRU));

    if (VOS_NULL_PTR == pstPhoneEvent)
    {
        MMA_ERRORLOG("TAF_MMA_ReportRegStatus:ERROR:ALLOC MEMORY FAIL.");
        return;
    }

    PS_MEM_SET(pstPhoneEvent, 0, sizeof(TAF_PHONE_EVENT_INFO_STRU));
    MMA_InitEventInfoOP(pstPhoneEvent);
    pstPhoneEvent->OP_CurRegSta            = MMA_FALSE;

    if (MMA_MMC_SRVDOMAIN_CS == enCnDomainId)
    {
        pstPhoneEvent->CurRegSta.OP_CsRegState = MMA_TRUE;
        pstPhoneEvent->CurRegSta.OP_Lac        = MMA_TRUE;
        pstPhoneEvent->CurRegSta.OP_CellId     = MMA_TRUE;
        pstPhoneEvent->CurRegSta.OP_ucAct      = MMA_TRUE;

        TAF_SDC_SetCsRegStatus(enRegState);
        pstPhoneEvent->CurRegSta.RegState = enRegState;
    }
    else if (MMA_MMC_SRVDOMAIN_PS == enCnDomainId)
    {
        pstPhoneEvent->CurRegSta.OP_PsRegState = MMA_TRUE;
        pstPhoneEvent->CurRegSta.OP_Lac        = MMA_TRUE;
        pstPhoneEvent->CurRegSta.OP_Rac        = MMA_TRUE;
        pstPhoneEvent->CurRegSta.OP_CellId     = MMA_TRUE;
        pstPhoneEvent->CurRegSta.OP_ucAct      = MMA_TRUE;

        TAF_SDC_SetPsRegStatus(enRegState);
        pstPhoneEvent->CurRegSta.ucPsRegState = enRegState;
    }
    else
    {
    }


    pstPhoneEvent->CurRegSta.ucRac               = TAF_SDC_GetCurrCampRac();
    pstPhoneEvent->CurRegSta.CellId.ucCellNum    = 1;
    pstPhoneEvent->CurRegSta.CellId.aulCellId[0] = TAF_SDC_GetCurrCampCellId();
    pstPhoneEvent->CurRegSta.usLac               = TAF_SDC_GetCurrCampLac();

    PS_MEM_CPY(&pstPhoneEvent->CurRegSta.Plmn, pstPlmnId, sizeof(pstPhoneEvent->CurRegSta.Plmn));

    pstPhoneEvent->PhoneEvent              = TAF_PH_EVT_SYSTEM_INFO_IND;
    pstPhoneEvent->OP_CurRegSta            = MMA_TRUE;

    if (TAF_PH_USIM_SPN == gstMmaValue.stUsimInfo.UsimSpnInfo.SpnType)
    {
        pstPhoneEvent->OP_SpnInfo      = MMA_TRUE;
        pstPhoneEvent->PhSpnInfo.DispRplmnMode   = gstMmaValue.stUsimInfo.UsimSpnInfo.DispRplmnMode;
        pstPhoneEvent->PhSpnInfo.PhRoam          = MMA_IsRoam(pstPlmnId);
        PS_MEM_CPY(pstPhoneEvent->PhSpnInfo.aucSpnName, gstMmaValue.stUsimInfo.UsimSpnInfo.aucSpnName,
                   TAF_PH_SPN_NAME_MAXLEN);
    }
    else if (TAF_PH_GSM_SPN == gstMmaValue.stUsimInfo.SimSpnInfo.SpnType)
    {
        pstPhoneEvent->OP_SpnInfo      = MMA_TRUE;
        pstPhoneEvent->PhSpnInfo.DispRplmnMode   = gstMmaValue.stUsimInfo.SimSpnInfo.DispRplmnMode;
        pstPhoneEvent->PhSpnInfo.PhRoam          = MMA_IsRoam(pstPlmnId);
        PS_MEM_CPY(pstPhoneEvent->PhSpnInfo.aucSpnName, gstMmaValue.stUsimInfo.SimSpnInfo.aucSpnName,
                   TAF_PH_SPN_NAME_MAXLEN);
    }
    else
    {
    }

    /* ��MMC�ϱ���ϵͳ��ģʽתΪ+CREG/+CGREG�������[ACT]��Э��涨�Ľ��뼼��ֵ */
    NAS_MMA_TransferSysSubMode2ActForAtCmdCreg(TAF_SDC_GetSysSubMode(), &(pstPhoneEvent->CurRegSta.ucAct));

    switch (TAF_SDC_GetCampSysMode())
    {
#if (FEATURE_ON == FEATURE_LTE)
        case TAF_SDC_SYS_MODE_LTE:
            pstPhoneEvent->RatType = TAF_PH_INFO_LTE_RAT;
            break;
#endif

        case TAF_SDC_SYS_MODE_WCDMA:
            pstPhoneEvent->RatType = TAF_PH_INFO_WCDMA_RAT;
            break;
            
        case TAF_SDC_SYS_MODE_GSM:
            pstPhoneEvent->RatType = TAF_PH_INFO_GSM_RAT;
            break;

        default:
            pstPhoneEvent->RatType = TAF_PH_INFO_NONE_RAT;
            break;
    }

    Taf_PhoneEvent(pstPhoneEvent);

    PS_MEM_FREE(WUEPS_PID_MMA, pstPhoneEvent);
    return;
}
VOS_UINT32 TAF_MMA_IsCsServDomainAvail(
    TAF_PH_MS_CLASS_TYPE                enMsClassType
)
{
    /* �ֻ�ģʽ��Aģʽ����CCģʽ������ΪCS����ã����������ΪCS�򲻿��� */
    if ( (TAF_PH_MS_CLASS_A  == enMsClassType)
      || (TAF_PH_MS_CLASS_CC == enMsClassType) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}
VOS_UINT32 TAF_MMA_IsPsServDomainAvail(
    TAF_PH_MS_CLASS_TYPE                enMsClassType
)
{
    /* �ֻ�ģʽ��Aģʽ����CGģʽ������ΪPS����ã����������ΪPS�򲻿��� */
    if ( (TAF_PH_MS_CLASS_A  == enMsClassType)
      || (TAF_PH_MS_CLASS_CG == enMsClassType) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}
TAF_CS_SERVICE_ENUM_UINT32  TAF_MMA_GetCsStateFromSyscfgSetFlag(
    VOS_UINT16                          usSyscfgSetFlag
)
{
    /* ��֧��SVLTE������Ҫ֪ͨMSG��STK,��Ϊ�ޱ仯 */
    if (VOS_FALSE == TAF_SDC_GetSvlteSupportFlag())
    {
        return TAF_CS_SERVICE_BUTT;
    }

    /* syscfg���ñ�־�µ�CS������������ */
    if (MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_SEND_CS_OFF ==
        (usSyscfgSetFlag & MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_SEND_CS_OFF))
    {
        return TAF_CS_OFF;
    }

    /* syscfg���ñ�־�µ�CS���������� */
    if (MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_SEND_CS_ON ==
        (usSyscfgSetFlag & MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_SEND_CS_ON))
    {
        return TAF_CS_ON;
    }

    /* �������Ĭ�ϲ���Ҫ���� */
    return TAF_CS_SERVICE_BUTT;

}


VOS_VOID  TAF_MMA_ClearSyscfgCsServiceChangeFlag(
    VOS_UINT16                          usSyscfgSetFlag
)
{
    if (MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_SEND_CS_OFF ==
        (usSyscfgSetFlag & MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_SEND_CS_OFF))
    {
        /* ֮ǰ�ù�����CS_OFF�ı�־����CS_OFF�ı�־��� */
        gstMmaValue.stSysCfgSet.usSetFlag &= ~MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_SEND_CS_OFF;
    }
    else if (MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_SEND_CS_ON ==
        (usSyscfgSetFlag & MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_SEND_CS_ON))
    {
        /* ֮ǰ�ù�����CS_ON�ı�־����CS_ON�ı�־��� */
        gstMmaValue.stSysCfgSet.usSetFlag &= ~MMA_SYS_CFG_SRV_DOMAIN_SET_NEED_SEND_CS_ON;
    }
    else
    {
    }

    return;

}


VOS_UINT32 TAF_MMA_SndStkCsServiceChangeNotify(
    TAF_CS_SERVICE_ENUM_UINT32          enCsState
)
{
    VOS_UINT32                          ulRet;
    TAF_STK_CS_SERVICE_IND             *pstMsg = VOS_NULL_PTR;

    pstMsg = (TAF_STK_CS_SERVICE_IND *)PS_ALLOC_MSG(WUEPS_PID_TAF,
                           sizeof(TAF_STK_CS_SERVICE_IND) - VOS_MSG_HEAD_LENGTH);

    if ( VOS_NULL_PTR == pstMsg )
    {
        NAS_ERROR_LOG(WUEPS_PID_TAF, "TAF_MMA_SndStkCsServiceChangeNotify:ERROR: Memory Alloc Error for pMsg");
        return VOS_ERR;
    }

    PS_MEM_SET((VOS_INT8*)pstMsg + VOS_MSG_HEAD_LENGTH, 0,
                     sizeof(TAF_STK_CS_SERVICE_IND) - VOS_MSG_HEAD_LENGTH);

    pstMsg->ulReceiverCpuId  = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverPid    = NAS_GetOamReceivePid(MAPS_STK_PID);
    pstMsg->ulSenderCpuId    = VOS_LOCAL_CPUID;
    pstMsg->ulSenderPid      = WUEPS_PID_TAF;
    pstMsg->ulLength         = sizeof(TAF_STK_CS_SERVICE_IND) - VOS_MSG_HEAD_LENGTH;
    pstMsg->ulMsgName        = TAF_STK_CS_STATE_IND;
    pstMsg->enCSState        = enCsState;

    /* ����VOS����ԭ�� */
    ulRet = PS_SEND_MSG( WUEPS_PID_TAF, pstMsg );

    if ( VOS_OK != ulRet )
    {
        NAS_ERROR_LOG(WUEPS_PID_TAF, "TAF_MMA_SndStkCsServiceChangeNotify:ERROR:PS_SEND_MSG FAILURE");
        return VOS_ERR;
    }

    return VOS_OK;

}
VOS_UINT32 TAF_MMA_IsNeedSndStkEvt(VOS_VOID)
{
    if (VOS_TRUE == TAF_SDC_GetSvlteSupportFlag())
    {
       if (VOS_TRUE == TAF_MMA_IsCsServDomainAvail(gstMmaValue.stSetMsClass.MsClassType))
       {
           return VOS_TRUE;
       }
       return VOS_FALSE;
    }
    else
    {
        return VOS_TRUE;
    }
}



VOS_VOID TAF_MMA_SndMtaAreaLostInd(VOS_VOID)
{
    VOS_UINT32                          ulRet;
    NAS_MTA_AREA_LOST_IND_STRU         *pstMsg = VOS_NULL_PTR;

    /* �����ڴ�  */
    pstMsg = (NAS_MTA_AREA_LOST_IND_STRU *)PS_ALLOC_MSG(WUEPS_PID_MMA,
                           sizeof(NAS_MTA_AREA_LOST_IND_STRU) - VOS_MSG_HEAD_LENGTH);

    if ( VOS_NULL_PTR == pstMsg )
    {
        NAS_ERROR_LOG(WUEPS_PID_MMA, "TAF_MMA_SndMtaAreaLostInd:ERROR: Memory Alloc Error for pMsg");
        return;
    }

    PS_MEM_SET((VOS_INT8*)pstMsg + VOS_MSG_HEAD_LENGTH, 0,
                     sizeof(NAS_MTA_AREA_LOST_IND_STRU) - VOS_MSG_HEAD_LENGTH);

    pstMsg->stMsgHeader.ulReceiverCpuId  = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulReceiverPid    = UEPS_PID_MTA;
    pstMsg->stMsgHeader.ulSenderCpuId    = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulSenderPid      = WUEPS_PID_MMA;
    pstMsg->stMsgHeader.ulLength         = sizeof(NAS_MTA_AREA_LOST_IND_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMsg->stMsgHeader.ulMsgName        = ID_NAS_MTA_AREA_LOST_IND;

    /* ����VOS����ԭ�� */
    ulRet = PS_SEND_MSG( WUEPS_PID_MMA, pstMsg );

    if ( VOS_OK != ulRet )
    {
        NAS_ERROR_LOG(WUEPS_PID_MMA, "TAF_MMA_SndMsgCsServiceChangeNotify:ERROR:PS_SEND_MSG FAILURE");
        return;
    }

    return;
}


VOS_UINT32  TAF_MMA_PlmnReselectAutoReq_PowerOff(VOS_UINT32 ulOpID)
{
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo = VOS_NULL_PTR;

    pstMsCapInfo = TAF_SDC_GetMsCapInfo();
    TAF_SDC_InitSvlteSupportFlag(TAF_SDC_INIT_CTX_STARTUP, pstMsCapInfo);

    if (VOS_TRUE == TAF_SDC_GetSvlteSupportFlag())
    {
        /* ֱ��д8214 nv��*/
        TAF_MMA_WritePlmnSelectionModeNvim(MMA_MMC_PLMN_SEL_MODE_AUTO);

        TAF_MMA_SetPlmnSelectionMode(MMA_MMC_PLMN_SEL_MODE_AUTO);
            
        /* ֱ���ϱ���� */
        g_StatusContext.PhoneError    = TAF_ERR_NO_ERROR;
        g_StatusContext.ucOperateType = STA_OP_PLMNRESELMODE;
        Sta_EventReport(ulOpID, TAF_PH_EVT_PLMN_RESEL_CNF);
        return STA_SUCCESS;
    }

    /* svlte nv�ر��ϱ�error */
    g_StatusContext.PhoneError    = TAF_ERR_PHONE_MSG_UNMATCH;
    g_StatusContext.ucOperateType = STA_OP_PHONEERROR;

    Sta_EventReport (ulOpID, TAF_PH_EVT_PLMN_RESEL_CNF);
    return STA_ERROR;
}
VOS_UINT32  TAF_MMA_RcvPlmnSelectionReq_PowerOff(
    VOS_UINT32                          ulOpID,
    TAF_PLMN_ID_STRU                    stPlmnId,
    TAF_PH_RA_MODE                      ucAccessMode,
    VOS_UINT8                           ucReselMode
)
{
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo = VOS_NULL_PTR;

    pstMsCapInfo = TAF_SDC_GetMsCapInfo();
    TAF_SDC_InitSvlteSupportFlag(TAF_SDC_INIT_CTX_STARTUP, pstMsCapInfo);

    if (VOS_TRUE == TAF_SDC_GetSvlteSupportFlag())
    {
        TAF_MMA_SndMmcPlmnSpecialReq((MMA_MMC_PLMN_ID_STRU *)&stPlmnId, ucAccessMode);
        g_StatusContext.ulCurOpId = ulOpID;
        g_StatusContext.StaPlmnId.Mcc = stPlmnId.Mcc;
        g_StatusContext.StaPlmnId.Mnc = stPlmnId.Mnc;

        return STA_SUCCESS;
    }

    /* svlte nv�ر��ϱ�error */
    g_StatusContext.ucOperateType = STA_OP_PHONEERROR;

    Sta_EventReport (ulOpID, TAF_PH_EVT_PLMN_SEL_CNF);
    return STA_ERROR;

}

#ifdef  __cplusplus
  #if  __cplusplus
  }
  #endif
#endif

