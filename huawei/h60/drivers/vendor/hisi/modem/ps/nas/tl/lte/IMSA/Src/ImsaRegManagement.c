/******************************************************************************

   Copyright(C)2013,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : ImsaRegManagement.c
  Description     : ��C�ļ�ʵ��ע�����ģ��ĳ�ʼ�����ڲ���Ϣ�Ĵ���ͷ���
  History           :
     1.sunbing 49683      2013-06-19  Draft Enact

******************************************************************************/

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include "ImsaEntity.h"
#include "ImsaRegManagement.h"
#include "ImsaPublic.h"
#include "ImsaImsAdaption.h"
#include "ImsaImsInterface.h"
#include "math.h"

/*lint -e767*/
#define    THIS_FILE_ID         PS_FILE_ID_IMSAREGMANAGEMENT_C
/*lint +e767*/


/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if (FEATURE_ON == FEATURE_IMS)
/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/

VOS_UINT32 IMSA_RegFsmProcIdleReqReq(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID *pData);

VOS_UINT32 IMSA_RegFsmProcRegisteringImsRegSuccess(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID *pData);
VOS_UINT32 IMSA_RegFsmProcRegisteringImsRegFailure(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID *pData);
VOS_UINT32 IMSA_RegFsmProcRegisteringProtectTimeout(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID *pData);
VOS_UINT32 IMSA_RegFsmProcRegisteringUserDereg(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData);

VOS_UINT32 IMSA_RegFsmProcRegisteredImsRegFailure(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData);
VOS_UINT32 IMSA_RegFsmProcRegisteredUserDereg(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData);

VOS_UINT32 IMSA_RegFsmProcDeregingImsRegFailure(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData);
VOS_UINT32 IMSA_RegFsmProcDeregingProtectTimeout(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData);
VOS_UINT32 IMSA_RegFsmProcDeregingUserReg(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData);

VOS_UINT32 IMSA_RegFsmProcWaitRetryTimeout(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData);
VOS_UINT32 IMSA_RegFsmProcWaitRetryUserDereg(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData);

VOS_UINT32 IMSA_RegFsmProcPendingProtectTimeout(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData);
VOS_UINT32 IMSA_RegFsmProcPendingUserReg(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData);
VOS_UINT32 IMSA_RegFsmProcPendingUserDereg(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData);
VOS_UINT32 IMSA_RegFsmProcPendingImsRegFailure(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData);
VOS_UINT32 IMSA_RegFsmProcPendingImsRegSuccess(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData);

VOS_UINT32 IMSA_RegFsmProcRollingBackProtectTimeout(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData);
VOS_UINT32 IMSA_RegFsmProcRollingBackImsRegFailure(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData);
VOS_UINT32 IMSA_RegFsmProcRollingBackUserReg(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData);
VOS_UINT32 IMSA_RegFsmProcRollingBackUserDereg(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData);

IMSA_FSM_PROC IMSA_FsmGetProcItem(VOS_UINT32 ulState, VOS_UINT32 ulEvt);
VOS_UINT32 IMSA_FsmRun(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID *pData);
VOS_VOID IMSA_RegResetRetryTimes
(
    IMSA_REG_TYPE_ENUM_UINT8            enRegType
);
VOS_VOID IMSA_RegConverterReg2Ims
(
    IMSA_REG_TYPE_ENUM_UINT8 enRegType,
    const VOS_CHAR *pucIpAddr,
    const VOS_CHAR *pacPcscfAddr,
    IMSA_IMS_INPUT_EVENT_STRU *pstImsaImsInputEvt
);
VOS_VOID IMSA_RegConfigIpAddress2Ims
(
    IMSA_REG_TYPE_ENUM_UINT8            enRegType,
    const VOS_CHAR                     *pcUeAddr,
    const VOS_CHAR                     *pacPcscfAddr
);
VOS_VOID IMSA_RegSendImsMsgDereg
(
    IMSA_REG_TYPE_ENUM_UINT8            enRegType,
    VOS_UINT32                          ulIsLocal
);
VOS_UINT32 IMSA_RegSendImsMsgReg
(
    IMSA_REG_TYPE_ENUM_UINT8 enRegType,
    const VOS_CHAR *pcUeAddr,
    const VOS_CHAR *pcPcscfAddr
);
VOS_UINT32 IMSA_RegSendIntraMsgRegInd
(
    IMSA_REG_TYPE_ENUM_UINT8 enRegType,
    IMSA_REG_RESULT_ENUM_UINT32 enResult,
    IMSA_RESULT_ACTION_ENUM_UINT32 enAction
);
VOS_UINT32 IMSA_RegSendIntraMsgDeregInd
(
    IMSA_REG_TYPE_ENUM_UINT8 enRegType,
    IMSA_REG_RESULT_ENUM_UINT32 enResult,
    IMSA_RESULT_ACTION_ENUM_UINT32 enAction
);
VOS_UINT32 IMSA_RegGetAddrByParamType
(
    IMSA_REG_TYPE_ENUM_UINT8 enRegType,
    IMSA_REG_ADDR_PARAM_ENUM_UINT32 enParamType,
    VOS_CHAR *pucUeAddr,
    VOS_CHAR *pucPcscfAddr
);
VOS_UINT32 IMSA_RegParseImsError
(
    const IMSA_IMS_OUTPUT_SERVICE_EVENT_STRU  *pstOutputService,
    IMSA_IMS_OUTPUT_ERROR_STRU          *pstImsErrorInfo
);
VOS_UINT32 IMSA_RegCalculateRetryDelayTime
(
    VOS_UINT32                          ulMaxTime,
    VOS_UINT32                          ulBaseTime,
    VOS_UINT32                          ulConsecutiveFailures
);
VOS_UINT32 IMSA_RegCalculateRegFailureRetryIntervelTime
(
    VOS_UINT32                          ulRetryTimes,
    VOS_UINT32                          ulRetryAfter,
    VOS_UINT32                          ulIsReRegisteration
);
VOS_VOID IMSA_RegFsmProcRegisteringImsRegFailure305
(
    IMSA_REG_ENTITY_STRU               *pstEntity
);
VOS_VOID IMSA_RegWaitForRetryCommonProc
(
    IMSA_REG_ENTITY_STRU               *pstEntity,
    VOS_UINT32                          ulRetryAfter,
    VOS_UINT32                          ulIsReRegisteration
);
VOS_UINT32 IMSA_RegProcImsMsgState(const IMSA_IMS_OUTPUT_SERVICE_EVENT_STRU  *pstOutputService);
IMSA_REG_ADDR_PAIR_STRU* IMSA_RegAddrPairCreate
(
    IMSA_IP_TYPE_ENUM_UINT8 enIpType,
    const VOS_CHAR *pacUeAddr,
    const VOS_CHAR *pacPcscfAddr
);
VOS_VOID IMSA_RegAddrPairDestroy(IMSA_REG_ADDR_PAIR_STRU *pstAddrPair);
IMSA_REG_ADDR_PAIR_STRU* IMSA_RegAddrPairListAdd
(
    IMSA_REG_ADDR_PAIR_STRU **ppstListHead,
    IMSA_REG_ADDR_PAIR_STRU *pstNode
);
VOS_VOID IMSA_RegAddrPairListDestroy(IMSA_REG_ADDR_PAIR_STRU *pstListHead);
VOS_UINT32 IMSA_RegAddrPairMgrUpdateWithNewPcscfAddr
(
    IMSA_REG_ADDR_PAIR_MGR_STRU *pstAddrPairMgr,
    IMSA_IP_TYPE_ENUM_UINT8 enIpType,
    const VOS_CHAR *pacPcscfAddr
);
VOS_UINT32 IMSA_RegAddrPairMgrUpdateWithNewUeAddr
(
    IMSA_REG_ADDR_PAIR_MGR_STRU *pstAddrPairMgr,
    IMSA_IP_TYPE_ENUM_UINT8 enIpType,
    const VOS_CHAR *pacUeAddr
);
VOS_VOID IMSA_RegGetAddrPairListHead
(
    IMSA_REG_TYPE_ENUM_UINT8            ulRegType,
    IMSA_IP_TYPE_ENUM_UINT8             enIpType,
    IMSA_REG_ADDR_PAIR_STRU          ***pppstListHead
);
VOS_VOID IMSA_RegAddrPairListClearAllUsedFlag(IMSA_REG_ADDR_PAIR_STRU *pstList);


/**
 *  ע��ģ��״̬����
 *
 */
IMSA_REG_FSM_ITEM_STRU g_stImsaRegFsmTable[] =
{
    {IMSA_REG_STATUS_NOT_REGISTER, IMSA_REG_EVT_USER_REG_REQ,     IMSA_RegFsmProcIdleReqReq},

    {IMSA_REG_STATUS_REGISTERING,  IMSA_REG_EVT_IMS_REG_SUCCESS,  IMSA_RegFsmProcRegisteringImsRegSuccess},
    {IMSA_REG_STATUS_REGISTERING,  IMSA_REG_EVT_IMS_REG_FAILURE,  IMSA_RegFsmProcRegisteringImsRegFailure},
    {IMSA_REG_STATUS_REGISTERING,  IMSA_REG_EVT_TIMEOUT_PROTECT,  IMSA_RegFsmProcRegisteringProtectTimeout},
    {IMSA_REG_STATUS_REGISTERING,  IMSA_REG_EVT_USER_DEREG_REQ,   IMSA_RegFsmProcRegisteringUserDereg},

    {IMSA_REG_STATUS_REGISTERED,   IMSA_REG_EVT_IMS_REG_FAILURE,  IMSA_RegFsmProcRegisteredImsRegFailure},
    {IMSA_REG_STATUS_REGISTERED,   IMSA_REG_EVT_USER_DEREG_REQ,   IMSA_RegFsmProcRegisteredUserDereg},

    {IMSA_REG_STATUS_DEREGING,     IMSA_REG_EVT_IMS_REG_FAILURE,  IMSA_RegFsmProcDeregingImsRegFailure},
    {IMSA_REG_STATUS_DEREGING,     IMSA_REG_EVT_TIMEOUT_PROTECT,  IMSA_RegFsmProcDeregingProtectTimeout},
    {IMSA_REG_STATUS_DEREGING,     IMSA_REG_EVT_USER_REG_REQ,     IMSA_RegFsmProcDeregingUserReg},

    {IMSA_REG_STATUS_WAIT_RETRY,   IMSA_REG_EVT_TIMEOUT_RETRY,    IMSA_RegFsmProcWaitRetryTimeout},
    {IMSA_REG_STATUS_WAIT_RETRY,   IMSA_REG_EVT_USER_DEREG_REQ,   IMSA_RegFsmProcWaitRetryUserDereg},

    {IMSA_REG_STATUS_PENDING,      IMSA_REG_EVT_TIMEOUT_PROTECT,  IMSA_RegFsmProcPendingProtectTimeout},
    {IMSA_REG_STATUS_PENDING,      IMSA_REG_EVT_USER_REG_REQ,     IMSA_RegFsmProcPendingUserReg},
    {IMSA_REG_STATUS_PENDING,      IMSA_REG_EVT_USER_DEREG_REQ,   IMSA_RegFsmProcPendingUserDereg},
    {IMSA_REG_STATUS_PENDING,      IMSA_REG_EVT_IMS_REG_FAILURE,  IMSA_RegFsmProcPendingImsRegFailure},
    /*{IMSA_REG_STATUS_PENDING,      IMSA_REG_EVT_IMS_REG_SUCCESS,  IMSA_RegFsmProcPendingImsRegSuccess},*/

    {IMSA_REG_STATUS_ROLLING_BACK, IMSA_REG_EVT_TIMEOUT_PROTECT,  IMSA_RegFsmProcRollingBackProtectTimeout},
    {IMSA_REG_STATUS_ROLLING_BACK, IMSA_REG_EVT_IMS_REG_FAILURE,  IMSA_RegFsmProcRollingBackImsRegFailure},
    {IMSA_REG_STATUS_ROLLING_BACK, IMSA_REG_EVT_USER_REG_REQ,     IMSA_RegFsmProcRollingBackUserReg},
    {IMSA_REG_STATUS_ROLLING_BACK, IMSA_REG_EVT_USER_DEREG_REQ,   IMSA_RegFsmProcRollingBackUserDereg},
};


VOS_VOID IMSA_RegAddrPairMgrInit(IMSA_REG_ADDR_PAIR_MGR_STRU *pstPairMgr);
VOS_VOID IMSA_RegAddrPairMgrDeinit(const IMSA_REG_ADDR_PAIR_MGR_STRU *pstPairMgr);
VOS_UINT32 IMSA_RegAddrPairMgrGetNextUnused(IMSA_REG_TYPE_ENUM_UINT8 ulRegType,
                                            VOS_CHAR *pacUeIp,
                                            VOS_CHAR *pacPcscfIp);
VOS_UINT32 IMSA_RegAddrPairMgrGetCurrent(IMSA_REG_TYPE_ENUM_UINT8 ulRegType,
                                         VOS_CHAR  *pacUeAddr,
                                         VOS_CHAR  *pacPcscfAddr);
IMSA_REG_ADDR_PAIR_STRU* IMSA_RegAddrPairListGetNextUnused(IMSA_REG_ADDR_PAIR_STRU *pstList);
IMSA_REG_ADDR_PAIR_STRU* IMSA_RegAddrPairMgrGetFirst(IMSA_REG_TYPE_ENUM_UINT8 ulRegType);

VOS_VOID IMSA_RegAddrPairMgrClearAllUsedFlag(IMSA_REG_TYPE_ENUM_UINT8 enRegType);


/*****************************************************************************
  3 Function
*****************************************************************************/
/*lint -e960*/
/*lint -e961*/

IMSA_FSM_PROC IMSA_FsmGetProcItem(VOS_UINT32 ulState, VOS_UINT32 ulEvt)
{
    VOS_UINT32 i = 0;

    for (i = 0; i < (sizeof(g_stImsaRegFsmTable)/sizeof(IMSA_REG_FSM_ITEM_STRU)); i++)
    {
        if ((g_stImsaRegFsmTable[i].ulState == ulState) &&
            (g_stImsaRegFsmTable[i].ulEvent == ulEvt))
        {
            return g_stImsaRegFsmTable[i].ProcFun;
        }
    }

    IMSA_INFO_LOG2("IMSA_FsmGetProcItem: not found fsm procedure", ulState, ulEvt);

    return VOS_NULL_PTR;
}


VOS_UINT32 IMSA_FsmRun(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID *pData)
{
    IMSA_FSM_PROC FsmProcFun;

    IMSA_INFO_LOG2("IMSA_FsmRun:", pstEntity->enStatus, ulEvt);

    FsmProcFun = IMSA_FsmGetProcItem(pstEntity->enStatus, ulEvt);
    if (VOS_NULL_PTR != FsmProcFun)
    {
        return FsmProcFun(pstEntity, ulEvt, pData);
    }

    IMSA_ERR_LOG("IMSA_FsmRun: Not registered state and event");

    return VOS_FALSE;
}


VOS_VOID IMSA_RegResetRetryTimes
(
    IMSA_REG_TYPE_ENUM_UINT8            enRegType
)
{
    IMSA_REG_ENTITY_STRU               *pstRegEntity = VOS_NULL_PTR;

    pstRegEntity = IMSA_RegEntityGetByType(enRegType);

    pstRegEntity->ulRetryTimes = 0;
}


VOS_VOID IMSA_RegConverterReg2Ims(IMSA_REG_TYPE_ENUM_UINT8 enRegType,
                                    const VOS_CHAR *pucIpAddr,
                                    const VOS_CHAR *pacPcscfAddr,
                                    IMSA_IMS_INPUT_EVENT_STRU *pstImsaImsInputEvt)
{
    IMSA_INFO_LOG("IMSA_RegConverterReg2Ims is entered!");

    /* ���������¼����� */
    pstImsaImsInputEvt->enEventType = IMSA_IMS_EVENT_TYPE_PARA;

    /* ��������Reason��opid */
    pstImsaImsInputEvt->evt.stInputParaEvent.ulOpId = IMSA_AllocImsOpId();
    if (IMSA_REG_TYPE_NORMAL == enRegType)
    {
        pstImsaImsInputEvt->evt.stInputParaEvent.enInputParaReason = IMAS_IMS_INPUT_PARA_REASON_SET_NORMAL_IP;

        /* ����IP��ַ */
        IMSA_UtilStrNCpy(pstImsaImsInputEvt->evt.stInputParaEvent.u.stNormalIp.acUeAddress, pucIpAddr, IMSA_IMS_IP_ADDRESS_STRING_SZ);

        /* ����PCSCF��ַ */
        IMSA_UtilStrNCpy(pstImsaImsInputEvt->evt.stInputParaEvent.u.stNormalIp.acPcscf, pacPcscfAddr, IMSA_IMS_EVENT_STRING_SZ);
    }
    else
    {
        pstImsaImsInputEvt->evt.stInputParaEvent.enInputParaReason = IMAS_IMS_INPUT_PARA_REASON_SET_EMC_IP;

        /* ����IP��ַ */
        IMSA_UtilStrNCpy(pstImsaImsInputEvt->evt.stInputParaEvent.u.stEmcIp.acUeAddress, pucIpAddr, IMSA_IMS_IP_ADDRESS_STRING_SZ);

        /* ����PCSCF��ַ */
        IMSA_UtilStrNCpy(pstImsaImsInputEvt->evt.stInputParaEvent.u.stEmcIp.acPcscf, pacPcscfAddr, IMSA_IMS_EVENT_STRING_SZ);
    }

}
#if 0

VOS_VOID IMSA_RegConverterEmcFlag2Ims(HICSM_InputEvent *pstInputEvent)
{
    IMSA_INFO_LOG("IMSA_RegConverterEmcFlag2Ims is entered!");

    /* ���������¼����� */
    pstInputEvent->type = CSM_EVENT_TYPE_RADIO;

    /* ��������opid�ͽ���ע���ʶ */
    pstInputEvent->evt.radio.csmId = IMSA_AllocImsOpId();
    pstInputEvent->evt.radio.isEmergencyRegRequired = 1;
}


VOS_VOID IMSA_RegConverterDereg2Ims(IMSA_REG_TYPE_ENUM_UINT8 enRegType,
                                      const VOS_CHAR *pacPcscfAddr,
                                      IMSA_IMS_INPUT_EVENT_STRU *pstInputEvt)
{
    IMSA_INFO_LOG("IMSA_RegConverterDereg2Ims is entered!");

    /* ���������¼����� */
    pstInputEvt->enEventType = IMSA_IMS_EVENT_TYPE_SERVICE;

    /* ����opid */
    pstInputEvt->evt.stInputServiceEvent.ulOpId = IMSA_AllocImsOpId();

    /* ����ע��ʵ�����ͼ��Ƿ񱾵�ȥע������reason */
    if (IMSA_REG_TYPE_NORMAL == enRegType)
    {
        pstInputEvt->evt.stInputParaEvent.enInputParaReason = IMAS_IMS_INPUT_PARA_REASON_SET_NORMAL_IP;

        /* ����IP��ַ */
        IMSA_UtilStrNCpy(pstInputEvt->evt.stInputParaEvent.u.stNormalIp.acUeAddress,"", IMSA_IMS_IP_ADDRESS_STRING_SZ);

        /* ����PCSCF��ַ */
        IMSA_UtilStrNCpy(pstInputEvt->evt.stInputParaEvent.u.stNormalIp.acPcscf, pacPcscfAddr, IMSA_IMS_EVENT_STRING_SZ);

    }
    else
    {
        pstInputEvt->evt.stInputParaEvent.enInputParaReason = IMAS_IMS_INPUT_PARA_REASON_SET_NORMAL_IP;

        /* ����IP��ַ */
        IMSA_UtilStrNCpy(pstInputEvt->evt.stInputParaEvent.u.stNormalIp.acUeAddress, "", IMSA_IMS_IP_ADDRESS_STRING_SZ);

        /* ����PCSCF��ַ */
        IMSA_UtilStrNCpy(pstInputEvt->evt.stInputParaEvent.u.stNormalIp.acPcscf, pacPcscfAddr, IMSA_IMS_EVENT_STRING_SZ);
    }
    #if 0
    /* ���IP��ַ */
    pstInputEvt->evt.stInputServiceEvent.address[0] = 0;

    /* ������������ */
    IMSA_UtilStrNCpy(pstInputEvt->evt.radio.infcName, pIfName, CSM_EVENT_STRING_SZ);
    #endif
}
#endif
VOS_VOID IMSA_RegConfigIpAddress2Ims
(
    IMSA_REG_TYPE_ENUM_UINT8            enRegType,
    const VOS_CHAR                     *pcUeAddr,
    const VOS_CHAR                     *pacPcscfAddr
)
{
    IMSA_IMS_INPUT_EVENT_STRU                    *pstImsaImsInputEvent = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegConfigIpAddress2Ims is entered!");

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstImsaImsInputEvent = IMSA_MEM_ALLOC(sizeof(IMSA_IMS_INPUT_EVENT_STRU));

    if ( VOS_NULL_PTR == pstImsaImsInputEvent )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_RegConfigIpAddress2Ims:ERROR:Alloc Mem fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(pstImsaImsInputEvent, 0, \
                 sizeof(IMSA_IMS_INPUT_EVENT_STRU));

    IMSA_RegConverterReg2Ims(enRegType, pcUeAddr, pacPcscfAddr, pstImsaImsInputEvent);

    IMSA_SndImsMsgParaEvent(pstImsaImsInputEvent);

    /*�ͷ���Ϣ�ռ�*/
    IMSA_MEM_FREE(pstImsaImsInputEvent);
}

#if 0

VOS_VOID IMSA_DeregConfigIpAddress2Ims
(
    IMSA_REG_TYPE_ENUM_UINT8            enRegType,
    const VOS_CHAR                     *pacPcscfAddr
)
{
    IMSA_IMS_INPUT_EVENT_STRU                    *pstImsaImsInputEvent = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegConfigIpAddress2Ims is entered!");

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstImsaImsInputEvent = IMSA_MEM_ALLOC(sizeof(IMSA_IMS_INPUT_EVENT_STRU));

    if ( VOS_NULL_PTR == pstImsaImsInputEvent )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_RegConfigIpAddress2Ims:ERROR:Alloc Mem fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(pstImsaImsInputEvent, 0, \
                 sizeof(IMSA_IMS_INPUT_EVENT_STRU));

    IMSA_RegConverterDereg2Ims(enRegType, pacPcscfAddr, pstImsaImsInputEvent);

    IMSA_SndImsMsgParaEvent(pstImsaImsInputEvent);

    /*�ͷ���Ϣ�ռ�*/
    IMSA_MEM_FREE(pstImsaImsInputEvent);
}



VOS_VOID IMSA_RegConfigEmcFlag2Ims( VOS_VOID )
{
    HICSM_InputEvent                    *pstHiInputEvent = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegConfigEmcFlag2Ims is entered!");

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstHiInputEvent = IMSA_MEM_ALLOC(sizeof(HICSM_InputEvent));

    if ( VOS_NULL_PTR == pstHiInputEvent )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_RegConfigEmcFlag2Ims:ERROR:Alloc Mem fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(pstHiInputEvent, 0, \
                 sizeof(HICSM_InputEvent));

    IMSA_RegConverterEmcFlag2Ims(pstHiInputEvent);

    IMSA_SndD2ImsMsgRadioEvent(pstHiInputEvent);

    /*�ͷ���Ϣ�ռ�*/
    IMSA_MEM_FREE(pstHiInputEvent);
}
#endif


VOS_VOID IMSA_RegSendImsMsgDereg
(
    IMSA_REG_TYPE_ENUM_UINT8            enRegType,
    VOS_UINT32                          ulIsLocal
)
{

    /*IMSA_DeregConfigIpAddress2Ims(enRegType,"");*/

    if (VOS_TRUE == ulIsLocal)
    {
        if (IMSA_REG_TYPE_NORMAL == enRegType)
        {
            IMSA_SndD2MsgServiceRegInfo(IMSA_IMS_INPUT_SERVICE_REASON_LOCAL_DEREGISTER);
        }
        else
        {
            IMSA_SndD2MsgServiceRegInfo(IMSA_IMS_INPUT_SERVICE_REASON_LOCAL_DEREGISTER_EMERGENCY);
        }

    }
    else
    {
        IMSA_SndD2MsgServiceRegInfo(IMSA_IMS_INPUT_SERVICE_REASON_DEREGISTER);
    }


    #if 0
    IMSA_IMS_INPUT_EVENT_STRU           *pstInputEvt = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegSendImsMsgDereg is entered!");

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstInputEvt = IMSA_MEM_ALLOC(sizeof(IMSA_IMS_INPUT_EVENT_STRU));

    if ( VOS_NULL_PTR == pstInputEvt )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_RegConfigEmcFlag2Ims:ERROR:Alloc Mem fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(pstInputEvt, 0, \
                 sizeof(IMSA_IMS_INPUT_EVENT_STRU));

    /*IMSA_SndD2ImsMsgRadioEvent(pstInputEvt);*/
    IMSA_DeregConfigIpAddress2Ims(enRegType,"");

    IMSA_SndD2MsgServiceRegInfo();

    /*�ͷ���Ϣ�ռ�*/
    IMSA_MEM_FREE(pstInputEvt);
    #endif
}


VOS_VOID IMSA_SndD2MsgServiceRegInfo(IMSA_IMS_INPUT_SERVICE_REASON_ENUM_UINT32   enInputServeReason)
{
    IMSA_IMS_INPUT_EVENT_STRU   *pstHiInputEvent;

    /*����ռ䲢��������Ƿ�ɹ�*/
    pstHiInputEvent = IMSA_MEM_ALLOC(sizeof(IMSA_IMS_INPUT_EVENT_STRU));

    if ( VOS_NULL_PTR == pstHiInputEvent )
    {
        /*��ӡ�쳣��Ϣ*/
        IMSA_ERR_LOG("IMSA_SndD2MsgServiceRegInfo:ERROR:Alloc Mem fail!");
        return ;
    }

    /*���*/
    IMSA_MEM_SET(pstHiInputEvent, 0, \
                 sizeof(IMSA_IMS_INPUT_EVENT_STRU));

    pstHiInputEvent->enEventType = IMSA_IMS_EVENT_TYPE_SERVICE;

    pstHiInputEvent->evt.stInputServiceEvent.enInputServeReason = enInputServeReason;

    pstHiInputEvent->evt.stInputServiceEvent.ulOpId = IMSA_AllocImsOpId();

    IMSA_SndImsMsgServiceEvent(pstHiInputEvent);

    /*�ͷ���Ϣ�ռ�*/
    IMSA_MEM_FREE(pstHiInputEvent);

}




VOS_UINT32 IMSA_RegSendImsMsgReg(IMSA_REG_TYPE_ENUM_UINT8 enRegType,
                                 const VOS_CHAR *pcUeAddr,
                                 const VOS_CHAR *pcPcscfAddr)
{
    IMSA_INFO_LOG("IMSA_RegSendImsMsgReg is entered!");

    if (IMSA_REG_TYPE_NORMAL == enRegType)
    {
        /* set UE IP address to trigger register */
        IMSA_RegConfigIpAddress2Ims(enRegType, pcUeAddr, pcPcscfAddr);

        IMSA_SndD2MsgServiceRegInfo(IMSA_IMS_INPUT_SERVICE_REASON_REGISTER);
    }
    else if (IMSA_REG_TYPE_EMC == enRegType)
    {
        /* set UE IP address to trigger register */
        IMSA_RegConfigIpAddress2Ims(enRegType, pcUeAddr, pcPcscfAddr);

        IMSA_SndD2MsgServiceRegInfo(IMSA_IMS_INPUT_SERVICE_REASON_REGISTER_EMERGENCY);
    }
    else
    {
    }

    return VOS_TRUE;
}


VOS_UINT32 IMSA_RegSendIntraMsgRegInd(IMSA_REG_TYPE_ENUM_UINT8 enRegType,
                                      IMSA_REG_RESULT_ENUM_UINT32 enResult,
                                      IMSA_RESULT_ACTION_ENUM_UINT32 enAction)
{
    IMSA_REG_REG_IND_STRU stRegInd = {0};

    IMSA_INFO_LOG("IMSA_RegSendIntraMsgRegInd is entered!");

    stRegInd.enRegType      = enRegType;
    stRegInd.enResult       = enResult;
    stRegInd.enResultAction = enAction;
    /**
     * ��ʱ���ϱ�SIP Server���صĲ�����Ϣ��
     * ���ϲ��û�������ʱ�����
     */

    return IMSA_RegSendIntraMsg(ID_IMSA_REG_REG_IND, (VOS_UINT8*)&stRegInd, sizeof(IMSA_REG_REG_IND_STRU));
}


VOS_UINT32 IMSA_RegSendIntraMsgDeregInd(IMSA_REG_TYPE_ENUM_UINT8 enRegType,
                                        IMSA_REG_RESULT_ENUM_UINT32 enResult,
                                        IMSA_RESULT_ACTION_ENUM_UINT32 enAction)
{
    IMSA_REG_DEREG_IND_STRU stDeregInd = {0};

    IMSA_INFO_LOG("IMSA_RegSendIntraMsgDeregInd is entered!");

    stDeregInd.enRegType      = enRegType;
    stDeregInd.enResult       = enResult;
    stDeregInd.enResultAction = enAction;
    /**
     * ��ʱ���ϱ�SIP Server���صĲ�����Ϣ��
     * ���ϲ��û�������ʱ�����
     */

    return IMSA_RegSendIntraMsg(ID_IMSA_REG_DEREG_IND, (VOS_UINT8*)&stDeregInd, sizeof(IMSA_REG_DEREG_IND_STRU));
}


VOS_UINT32 IMSA_RegSendIntraMsg(VOS_UINT32 ulMsgId, const VOS_UINT8 *pucMsg, VOS_UINT32 ulLen)
{
    /* ������Ϣ�ṹͷ���ֵĶ��� */
    IMSA_REG_REG_IND_STRU *pstIntraMsg = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegSendIntraMsg is entered!");

    /* ����ռ䲢��������Ƿ�ɹ� */
    pstIntraMsg = (VOS_VOID*)IMSA_GetIntraMsgBuffAddr(ulLen);
    if (VOS_NULL_PTR == pstIntraMsg)
    {
        IMSA_ERR_LOG("IMSA_RegSendIntraMsg: alloc fail");
        return VOS_FALSE;
    }

    /* �����ض���Ϣ������ */
    IMSA_MEM_CPY(pstIntraMsg, pucMsg, ulLen);

    /* ��д��Ϣͷ */
    IMSA_WRITE_INTRA_MSG_HEAD(pstIntraMsg, ulMsgId, ulLen);

    /* ������Ϣ */
    IMSA_SEND_INTRA_MSG((VOS_VOID*)pstIntraMsg);

    return VOS_TRUE;
}

VOS_UINT32 IMSA_RegGetAddrByParamType(IMSA_REG_TYPE_ENUM_UINT8 enRegType,
                                      IMSA_REG_ADDR_PARAM_ENUM_UINT32 enParamType,
                                      VOS_CHAR *pucUeAddr,
                                      VOS_CHAR *pucPcscfAddr)
{
    VOS_UINT32 ulResult = VOS_TRUE;
    IMSA_REG_ADDR_PAIR_STRU *pstAddrPair = VOS_NULL_PTR;

    IMSA_INFO_LOG1("IMSA_RegGetAddrByParamType is entered!", enParamType);

    switch (enParamType)
    {
    case IMSA_REG_ADDR_PARAM_NEW:
        IMSA_RegAddrPairMgrClearAllUsedFlag(enRegType);

        IMSA_RegResetRetryTimes(enRegType);

        pstAddrPair = IMSA_RegAddrPairMgrGetFirst(enRegType);
        if (VOS_NULL_PTR != pstAddrPair)
        {
            IMSA_UtilStrNCpy(pucUeAddr, pstAddrPair->acUeAddr, IMSA_IPV6_ADDR_STRING_LEN);
            IMSA_UtilStrNCpy(pucPcscfAddr, pstAddrPair->acPcscfAddr, IMSA_IPV6_ADDR_STRING_LEN);

            IMSA_INFO_LOG(pucUeAddr);
            IMSA_INFO_LOG(pucPcscfAddr);
        }
        else
        {
            ulResult = VOS_FALSE;
        }
        break;
    case IMSA_REG_ADDR_PARAM_NEXT:
        IMSA_RegResetRetryTimes(enRegType);

        ulResult = IMSA_RegAddrPairMgrGetNextUnused(enRegType, pucUeAddr, pucPcscfAddr);
        break;
    case IMSA_REG_ADDR_PARAM_SAME:
        ulResult = IMSA_RegAddrPairMgrGetCurrent(enRegType, pucUeAddr, pucPcscfAddr);
        break;
    case IMSA_REG_ADDR_PARAM_RESTORATION:
        IMSA_RegResetRetryTimes(enRegType);

        if ((VOS_FALSE == IMSA_RegAddrPairMgrGetNextUnused(enRegType, pucUeAddr, pucPcscfAddr)) &&
            (VOS_FALSE == IMSA_RegAddrPairMgrGetCurrent(enRegType, pucUeAddr, pucPcscfAddr)))
        {
            ulResult = VOS_FALSE;
        }
        break;
    default:
        ulResult = VOS_FALSE;
        IMSA_ERR_LOG1("IMSA_RegGetAddrByParamType: invalid param type", enParamType);
    }

    return ulResult;
}
VOS_UINT32 IMSA_RegFsmProcIdleReqReq(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    VOS_UINT32 ulResult = VOS_FALSE;
    VOS_CHAR aucUeAddr[IMSA_IPV6_ADDR_STRING_LEN+1] = {0};
    VOS_CHAR aucPcscfAddr[IMSA_IPV6_ADDR_STRING_LEN+1] = {0};
    IMSA_REG_REG_REQ_STRU *pstRegReq = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegFsmProcIdleReqReq is entered!");

    (VOS_VOID)ulEvt;

    pstRegReq = (IMSA_REG_REG_REQ_STRU *)pData;

    /* ����ע�����������ȡһ����ַ�ԣ����ʧ����ظ��û�ע��ʧ�� */
    if (VOS_FALSE == IMSA_RegGetAddrByParamType(pstEntity->enRegType, pstRegReq->enAddrType, aucUeAddr, aucPcscfAddr))
    {
        (VOS_VOID)IMSA_RegSendIntraMsgRegInd(pstEntity->enRegType, IMSA_REG_RESULT_FAIL_NO_ADDR_PAIR, IMSA_RESULT_ACTION_NULL);

        return VOS_FALSE;
    }

    IMSA_CONN_UpdateNicPdpInfo();

    /* ����ע���������D2 IMS Stack��ʼ����ע�� */
    ulResult = IMSA_RegSendImsMsgReg(pstEntity->enRegType, aucUeAddr, aucPcscfAddr);
    if (VOS_FALSE == ulResult)
    {
        (VOS_VOID)IMSA_RegSendIntraMsgRegInd(pstEntity->enRegType, IMSA_REG_RESULT_FAIL, IMSA_RESULT_ACTION_NULL);
        return VOS_FALSE;
    }

    /* ����������ʱ�� */
    IMSA_RegTimerStart(&pstEntity->stProtectTimer, pstEntity->enRegType);

    /* ״̬�л� */
    IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_REGISTERING);

    return VOS_TRUE;
}


VOS_UINT32 IMSA_RegFsmProcRegisteringImsRegSuccess(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    IMSA_INFO_LOG("IMSA_RegFsmProcRegisteringImsRegSuccess is entered!");

    (VOS_VOID)ulEvt;
    (VOS_VOID)pData;

    /* ֹͣ������ʱ�� */
    IMSA_RegTimerStop(&pstEntity->stProtectTimer);

    /* ֪ͨ�û�ע��ɹ� */
    (VOS_VOID)IMSA_RegSendIntraMsgRegInd(pstEntity->enRegType, IMSA_REG_RESULT_SUCCESS, IMSA_RESULT_ACTION_NULL);

    /* ״̬�л� */
    IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_REGISTERED);

    /* ��0���Դ��� */
    IMSA_RegResetRetryTimes(pstEntity->enRegType);

    return VOS_TRUE;
}


VOS_UINT32 IMSA_RegParseImsError
(
    const IMSA_IMS_OUTPUT_SERVICE_EVENT_STRU  *pstOutputService,
    IMSA_IMS_OUTPUT_ERROR_STRU          *pstImsErrorInfo
)
{
    if (VOS_TRUE == pstOutputService->bitOpErrorCode)
    {
        IMSA_MEM_CPY(   pstImsErrorInfo,
                        &pstOutputService->stErrorCode,
                        sizeof(IMSA_IMS_OUTPUT_ERROR_STRU));

        return IMSA_SUCC;
    }
    else
    {
        IMSA_WARN_LOG("IMSA_RegParseImsError: HICSM_parseServiceError failed!");
        return IMSA_FAIL;
    }
}


VOS_UINT32 IMSA_RegCalculateRetryDelayTime
(
    VOS_UINT32                          ulMaxTime,
    VOS_UINT32                          ulBaseTime,
    VOS_UINT32                          ulConsecutiveFailures
)
{
    VOS_UINT32                          W                   = IMSA_NULL;
    VOS_UINT32                          ulTmp               = IMSA_NULL;
    VOS_UINT32                          ulRetryDelayTime    = IMSA_NULL;

    /*  RFC 5626�涨�ļ��ʱ����㹫ʽ���£�
    W = min (max-time, (base-time * (2 ^ consecutive-failures)))
    retry delay time = rand * W
    rand�Ǹ����ֵ��ȡֵ��Χ��50%~100%*/
    ulTmp = ulBaseTime * (VOS_UINT32)pow(2.0, (double)ulConsecutiveFailures);

    if (ulMaxTime >= ulTmp)
    {
        W = ulTmp;
    }
    else
    {
        W = ulMaxTime;
    }

    ulRetryDelayTime = (VOS_UINT32)(((VOS_Rand(51) + 50) / 100.0) * W);

    return ulRetryDelayTime;
}



VOS_UINT32 IMSA_RegCalculateRegFailureRetryIntervelTime
(
    VOS_UINT32                          ulRetryTimes,
    VOS_UINT32                          ulRetryAfter,
    VOS_UINT32                          ulIsReRegisteration
)
{
    IMSA_REG_MANAGER_STRU              *pstImsaRegManager   = IMSA_RegCtxGet();
    VOS_UINT32                          ulRetryDelayTime    = IMSA_NULL;

    IMSA_INFO_LOG("IMSA_RegCalculateRegFailureRetryIntervelTime is entered!");

    /* ���NV�����ü���̶�ʱ�����򷵻�NV�����õĹ̶�ʱ���� */
    if (pstImsaRegManager->ulRetryPeriod != 0)
    {
        if (ulRetryAfter == 0)
        {
            return (1000 * pstImsaRegManager->ulRetryPeriod);
        }
        else
        {
            return (1000 * ulRetryAfter);
        }
    }

    /* ���NV�����ü���ǹ̶�ʱ��������24.229 5.1.1.2.1�½��Լ�RFC5626
       ������ע��ʧ�����³��Լ��ʱ�䣬������Բμ�IMS�����ĵ�2.3.9�½� */
    ulRetryDelayTime = IMSA_RegCalculateRetryDelayTime( pstImsaRegManager->ulMaxTime,
                                                        pstImsaRegManager->ulBaseTime,
                                                        ulRetryTimes);

    /* ����ǵ�һ�γ�ʼע�ᣬ�Ҳ�Я��retry after����ȡֵ5���Ӻ�retry delay time
       ����Сֵ��Э������:
       After a first unsuccessful initial registration attempt,  if the Retry-After
       header field was not present and the initial registration was not performed
       as a consequence of a failed reregistration, the UE shall not wait more
       than 5 minutes before attempting a new registration.*/
    if ((1 == ulRetryTimes) && (IMSA_FALSE == ulIsReRegisteration)
       && (0 == ulRetryAfter))
    {
        return 1000 * IMSA_Min(IMSA_REG_FAILURE_RETRY_INTERVEL_5_MIN, ulRetryDelayTime);
    }

    if (0 != ulRetryAfter)
    {
        return 1000 * IMSA_Max(ulRetryAfter, ulRetryDelayTime);
    }
    else
    {
        return (1000 * ulRetryDelayTime);
    }
}


VOS_VOID IMSA_RegFsmProcRegisteringImsRegFailure305
(
    IMSA_REG_ENTITY_STRU               *pstEntity
)
{
    VOS_UINT32                          ulResult                                = VOS_FALSE;
    VOS_CHAR                            aucUeAddr[IMSA_IPV6_ADDR_STRING_LEN+1]    = {0};
    VOS_CHAR                            aucPcscfAddr[IMSA_IPV6_ADDR_STRING_LEN+1] = {0};

    IMSA_INFO_LOG("IMSA_RegFsmProcRegisteringImsRegFailure305 is entered!");

    /* ֹͣ������ʱ�� */
    IMSA_RegTimerStop(&pstEntity->stProtectTimer);

    /* ״̬�л� */
    IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_NOT_REGISTER);

    /* ��ȡ��һ��ע���ַ�ԣ����ʧ����ظ��û�ע��ʧ�� */
    if (VOS_FALSE == IMSA_RegGetAddrByParamType(    pstEntity->enRegType,
                                                    IMSA_REG_ADDR_PARAM_NEXT,
                                                    aucUeAddr,
                                                    aucPcscfAddr))
    {
        (VOS_VOID)IMSA_RegSendIntraMsgRegInd(   pstEntity->enRegType,
                                                IMSA_REG_RESULT_FAIL_REMOTE,
                                                IMSA_RESULT_ACTION_NULL);

        return ;
    }

    IMSA_CONN_UpdateNicPdpInfo();

    /* ����ע���������IMS Stack��ʼ����ע�� */
    ulResult = IMSA_RegSendImsMsgReg(pstEntity->enRegType, aucUeAddr, aucPcscfAddr);
    if (VOS_FALSE == ulResult)
    {
        (VOS_VOID)IMSA_RegSendIntraMsgRegInd(   pstEntity->enRegType,
                                                IMSA_REG_RESULT_FAIL_REMOTE,
                                                IMSA_RESULT_ACTION_NULL);
        return ;
    }

    /* ����������ʱ�� */
    IMSA_RegTimerStart(&pstEntity->stProtectTimer, pstEntity->enRegType);

    /* ״̬�л� */
    IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_REGISTERING);
}




VOS_VOID IMSA_RegWaitForRetryCommonProc
(
    IMSA_REG_ENTITY_STRU               *pstEntity,
    VOS_UINT32                          ulRetryAfter,
    VOS_UINT32                          ulIsReRegisteration
)
{
    VOS_UINT32                          ulRetryTimerLen = IMSA_NULL;

    /* ֹͣ������ʱ�� */
    IMSA_RegTimerStop(&pstEntity->stProtectTimer);

    /* ����ע��ʧ���ٳ��Լ��ʱ�� */
    ulRetryTimerLen = IMSA_RegCalculateRegFailureRetryIntervelTime( pstEntity->ulRetryTimes,
                                                                    ulRetryAfter,
                                                                    ulIsReRegisteration);
    if (VOS_FALSE == ulIsReRegisteration)
    {
        if (0 != ulRetryTimerLen)
        {
            /* ����retry��ʱ�� */
            pstEntity->stRetryTimer.ulTimerLen = ulRetryTimerLen;
            IMSA_RegTimerStart(&pstEntity->stRetryTimer, pstEntity->enRegType);

            /* ״̬�л� */
            IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_WAIT_RETRY);
        }
        else
        {
            IMSA_RegFsmProcRegisteringImsRegFailure305(pstEntity);
        }
    }
    else
    {
        if (0 != ulRetryTimerLen)
        {
            /* ����retry��ʱ�� */
            pstEntity->stRetryTimer.ulTimerLen = ulRetryTimerLen;
            IMSA_RegTimerStart(&pstEntity->stRetryTimer, pstEntity->enRegType);

            /* ״̬�л� */
            IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_WAIT_RETRY);

            /* ֪ͨSERVICEʹ�õ�ǰ������������ע�� */
            (VOS_VOID)IMSA_RegSendIntraMsgDeregInd( pstEntity->enRegType,
                                                 IMSA_REG_RESULT_FAIL,
                                                 IMSA_RESULT_ACTION_REG_WITH_CURRENT_ADDR_PAIR);
        }
        else
        {
            /* ֪ͨSERVICEʹ����һ�������������ע�� */
            (VOS_VOID)IMSA_RegSendIntraMsgDeregInd( pstEntity->enRegType,
                                                 IMSA_REG_RESULT_FAIL,
                                                 IMSA_RESULT_ACTION_REG_WITH_NEXT_ADDR_PAIR);
        }

    }
}



VOS_UINT32 IMSA_RegFsmProcRegisteringImsRegFailure
(
    IMSA_REG_ENTITY_STRU               *pstEntity,
    VOS_UINT32                          ulEvt,
    VOS_VOID                           *pData
)
{
    IMSA_IMS_OUTPUT_ERROR_STRU         *pstImsErrorInfo = VOS_NULL_PTR;
    VOS_CHAR                            aucUeAddr[IMSA_IPV6_ADDR_STRING_LEN+1] = {0};
    VOS_CHAR                            aucPcscfAddr[IMSA_IPV6_ADDR_STRING_LEN+1] = {0};

    IMSA_REG_MANAGER_STRU              *pstImsaRegManager   = IMSA_RegCtxGet();

    IMSA_INFO_LOG("IMSA_RegFsmProcRegisteringImsRegFailure is entered!");

    (VOS_VOID)ulEvt;

    /* ���Դ�����1 */
    pstEntity->ulRetryTimes++;

    if (VOS_NULL_PTR == pData)
    {
        IMSA_RegFsmProcRegisteringImsRegFailure305(pstEntity);

        return VOS_TRUE;
    }

    pstImsErrorInfo = (IMSA_IMS_OUTPUT_ERROR_STRU*)pData;

    switch (pstImsErrorInfo->usSipStatusCode)
    {
        case IMSA_SIP_NW_ERROR_CAUSE_USE_PROXY:
        case IMSA_IMS_INT_ERROR_CODE_TIMERF_OUT:
        case IMSA_SIP_NW_ERROR_CAUSE_UNAUTHORIZED:

            IMSA_RegFsmProcRegisteringImsRegFailure305(pstEntity);
            break;
        case IMSA_SIP_NW_ERROR_CAUSE_REQUEST_TIMEOUT:
        case IMSA_SIP_NW_ERROR_CAUSE_SERVER_INTERNAL_ERROR:
        case IMSA_SIP_NW_ERROR_CAUSE_SERVER_TIMEOUT:
        case IMSA_SIP_NW_ERROR_CAUSE_BUSY_EVERYWHERE:
        case IMSA_SIP_NW_ERROR_CAUSE_SERVICE_UNAVAILABLE:

            /* ���ڽ���������Ҫ���ǽ�ͨ�ٶȵģ����ԾͲ���retry��ʱ���ȴ��ˣ�
               ֱ�ӻظ�����ע��ʧ�ܣ�����SPMȥCS���Խ����� */
            if (IMSA_REG_TYPE_EMC == pstEntity->enRegType)
            {
                /* ֹͣ������ʱ�� */
                IMSA_RegTimerStop(&pstEntity->stProtectTimer);

                /* ״̬�л� */
                IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_NOT_REGISTER);

                /* ֪ͨSERVICE����ע��ʧ�� */
                (VOS_VOID)IMSA_RegSendIntraMsgRegInd(   pstEntity->enRegType,
                                                        IMSA_REG_RESULT_FAIL_REMOTE,
                                                        IMSA_RESULT_ACTION_NULL);

                return VOS_TRUE;
            }

            IMSA_RegWaitForRetryCommonProc( pstEntity,
                                            pstImsErrorInfo->ulRetryAfter,
                                            IMSA_FALSE);

            break;

        case IMSA_IMS_INT_ERROR_CODE_TCP_ERROR:
        case IMSA_IMS_INT_ERROR_CODE_IPSEC_ERROR:

            /* �����ȡ��ǰ����ʹ�õĵ�ַ�Գɹ�����ʹ�øõ�ַ�����·���ע�� */
            if (VOS_TRUE == IMSA_RegGetAddrByParamType( pstEntity->enRegType,
                                                        IMSA_REG_ADDR_PARAM_SAME,
                                                        aucUeAddr,
                                                        aucPcscfAddr))
            {
                /* ֹͣ������ʱ�� */
                IMSA_RegTimerStop(&pstEntity->stProtectTimer);

                IMSA_CONN_UpdateNicPdpInfo();

                /* ��D2 IMS����ע����Ϣ */
                (VOS_VOID)IMSA_RegSendImsMsgReg(pstEntity->enRegType, aucUeAddr, aucPcscfAddr);

                /* ����������ʱ�� */
                IMSA_RegTimerStart(&pstEntity->stProtectTimer, pstEntity->enRegType);

                /* ״̬�л� */
                IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_REGISTERING);
            }
            else
            {
                IMSA_WARN_LOG("IMSA_RegFsmProcRegisteringImsRegFailure:get current para failed!");

                /* ֹͣ������ʱ�� */
                IMSA_RegTimerStop(&pstEntity->stProtectTimer);

                /* ״̬�л� */
                IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_NOT_REGISTER);

                (VOS_VOID)IMSA_RegSendIntraMsgRegInd(   pstEntity->enRegType,
                                                        IMSA_REG_RESULT_FAIL_TRANSPORT,
                                                        IMSA_RESULT_ACTION_NULL);
            }

            break;

        default:
            /* ����NV���õ��ٳ��Դ�����ʹ�õ�ǰע�������ȥ����ע�ᣬ
               ���NV���õĳ��Դ���Ϊ0����������޴γ��ԣ�
               ���NV���õĳ��Դ�����Ϊ0���������Ч�γ��ԣ������Գ�����Ч��ʱ��
               ��任�������ٴη���ע�� */
            if (0 == pstImsaRegManager->ulSaveRetryTimes)
            {
                IMSA_INFO_LOG("IMSA_RegFsmProcRegisteringImsRegFailure: retry limitless times!");
                IMSA_RegWaitForRetryCommonProc( pstEntity,
                                                pstImsErrorInfo->ulRetryAfter,
                                                IMSA_FALSE);
            }
            else
            {
                if (pstEntity->ulRetryTimes < pstImsaRegManager->ulSaveRetryTimes)
                {
                    IMSA_INFO_LOG("IMSA_RegFsmProcRegisteringImsRegFailure: retry limitary times!");
                    IMSA_RegWaitForRetryCommonProc( pstEntity,
                                                pstImsErrorInfo->ulRetryAfter,
                                                IMSA_FALSE);
                }
                else
                {
                    IMSA_INFO_LOG("IMSA_RegFsmProcRegisteringImsRegFailure: retry with next addr!");
                    IMSA_RegFsmProcRegisteringImsRegFailure305(pstEntity);
                }
            }
            break;

    }

    return VOS_TRUE;
}


VOS_UINT32 IMSA_RegFsmProcRegisteredImsRegFailure
(
    IMSA_REG_ENTITY_STRU               *pstEntity,
    VOS_UINT32                          ulEvt,
    VOS_VOID                           *pData
)
{
    IMSA_IMS_OUTPUT_ERROR_STRU         *pstImsErrorInfo = VOS_NULL_PTR;

    IMSA_REG_MANAGER_STRU              *pstImsaRegManager   = IMSA_RegCtxGet();

    IMSA_INFO_LOG("IMSA_RegFsmProcRegisteredImsRegFailure is entered!");

    (VOS_VOID)ulEvt;

    /* ���Դ�����1 */
    pstEntity->ulRetryTimes++;

    /* �л�״̬ */
    IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_NOT_REGISTER);

    /* IMS��������ע���쳣�������ٷ���IMS������ʼע�ᣬֱ�ӽ���IMS��������
       �����ͷţ���Ϊ�������Ѿ����ͷ��� */
    if (IMSA_REG_TYPE_EMC == pstEntity->enRegType)
    {
        /* ֪ͨSERVICE���ٳ���ע�� */
        (VOS_VOID)IMSA_RegSendIntraMsgDeregInd( pstEntity->enRegType,
                                                IMSA_REG_RESULT_FAIL,
                                                IMSA_RESULT_ACTION_NULL);

        return VOS_TRUE;
    }

    if (VOS_NULL_PTR == pData)
    {
        /* ֪ͨSERVICEʹ����һ�������������ע�� */
        (VOS_VOID)IMSA_RegSendIntraMsgDeregInd( pstEntity->enRegType,
                                                IMSA_REG_RESULT_FAIL,
                                                IMSA_RESULT_ACTION_REG_WITH_NEXT_ADDR_PAIR);

        return VOS_TRUE;
    }

    pstImsErrorInfo = (IMSA_IMS_OUTPUT_ERROR_STRU*)pData;

    switch (pstImsErrorInfo->usSipStatusCode)
    {
        case IMSA_SIP_NW_ERROR_CAUSE_USE_PROXY:
        case IMSA_IMS_INT_ERROR_CODE_TIMERF_OUT:

            /* ֪ͨSERVICEʹ����һ�������������ע�� */
            (VOS_VOID)IMSA_RegSendIntraMsgDeregInd( pstEntity->enRegType,
                                                    IMSA_REG_RESULT_FAIL,
                                                    IMSA_RESULT_ACTION_REG_WITH_NEXT_ADDR_PAIR);
            break;

        case IMSA_SIP_NW_ERROR_CAUSE_REQUEST_TIMEOUT:
        case IMSA_SIP_NW_ERROR_CAUSE_SERVER_INTERNAL_ERROR:
        case IMSA_SIP_NW_ERROR_CAUSE_SERVER_TIMEOUT:
        case IMSA_SIP_NW_ERROR_CAUSE_BUSY_EVERYWHERE:
        case IMSA_SIP_NW_ERROR_CAUSE_SERVICE_UNAVAILABLE:

            IMSA_RegWaitForRetryCommonProc( pstEntity,
                                            pstImsErrorInfo->ulRetryAfter,
                                            IMSA_TRUE);
            break;

        case IMSA_IMS_INT_ERROR_CODE_TCP_ERROR:
        case IMSA_IMS_INT_ERROR_CODE_IPSEC_ERROR:

            /* ֪ͨSERVICEʹ�õ�ǰ������������ע�� */
            (VOS_VOID)IMSA_RegSendIntraMsgDeregInd( pstEntity->enRegType,
                                                    IMSA_REG_RESULT_FAIL,
                                                    IMSA_RESULT_ACTION_REG_WITH_CURRENT_ADDR_PAIR);
            break;

        default:
            /* ����NV���õ��ٳ��Դ�����ʹ�õ�ǰע�������ȥ����ע�ᣬ
               ���NV���õĳ��Դ���Ϊ0����������޴γ��ԣ�
               ���NV���õĳ��Դ�����Ϊ0���������Ч�������ԣ������Գ�����Ч����ʱ��
               ��ʹ����һ���������ע�� */
            if (0 == pstImsaRegManager->ulSaveRetryTimes)
            {
                IMSA_INFO_LOG("IMSA_RegFsmProcRegisteredImsRegFailure: retry limitless times!");
                IMSA_RegWaitForRetryCommonProc( pstEntity,
                                                pstImsErrorInfo->ulRetryAfter,
                                                IMSA_TRUE);
            }
            else
            {
                if (pstEntity->ulRetryTimes < pstImsaRegManager->ulSaveRetryTimes)
                {
                    IMSA_INFO_LOG("IMSA_RegFsmProcRegisteredImsRegFailure: retry limitary times!");
                    IMSA_RegWaitForRetryCommonProc( pstEntity,
                                                    pstImsErrorInfo->ulRetryAfter,
                                                    IMSA_TRUE);
                }
                else
                {
                    /* ֪ͨSERVICEʹ����һ�������������ע�� */
                    IMSA_INFO_LOG("IMSA_RegFsmProcRegisteredImsRegFailure: retry with next addr!");
                    (VOS_VOID)IMSA_RegSendIntraMsgDeregInd( pstEntity->enRegType,
                                                            IMSA_REG_RESULT_FAIL,
                                                            IMSA_RESULT_ACTION_REG_WITH_NEXT_ADDR_PAIR);
                }
            }
            break;
    }

    return VOS_TRUE;
}
VOS_UINT32 IMSA_RegFsmProcRegisteringProtectTimeout(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    IMSA_INFO_LOG("IMSA_RegFsmProcRegisteringProtectTimeout is entered!");

    (VOS_VOID)ulEvt;
    (VOS_VOID)pData;

    /* ����ȥע�� */
    IMSA_RegSendImsMsgDereg(pstEntity->enRegType, VOS_TRUE);

    /* ����������ʱ�� */
    IMSA_RegTimerStart(&pstEntity->stProtectTimer, pstEntity->enRegType);

    /* ��������״̬ */
    pstEntity->enExceptedStatus = IMSA_REG_STATUS_REGISTERED;

    /*����ע�����*/
    pstEntity->enPendingRegParam = IMSA_REG_ADDR_PARAM_NEXT;

    /* ״̬�л� */
    IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_ROLLING_BACK);

    return VOS_TRUE;
}
VOS_UINT32 IMSA_RegFsmProcRegisteringUserDereg(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    IMSA_REG_DEREG_REQ_STRU *pstDeregReq = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegFsmProcRegisteringUserDereg is entered!");

    (VOS_VOID)ulEvt;

    pstDeregReq = (IMSA_REG_DEREG_REQ_STRU *)pData;

    /* ֹͣ������ʱ�� */
    IMSA_RegTimerStop(&pstEntity->stProtectTimer);

    /* ����ȥע�� */
    IMSA_RegSendImsMsgDereg(pstDeregReq->enDeRegType, pstDeregReq->ulLocalFlag);

    /* ����������ʱ�� */
    IMSA_RegTimerStart(&pstEntity->stProtectTimer, pstEntity->enRegType);

    /* ״̬�л� */
    IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_DEREGING);

    return VOS_TRUE;
}


VOS_UINT32 IMSA_RegFsmProcRegisteredUserDereg(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    IMSA_REG_DEREG_REQ_STRU *pstDeregReq = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegFsmProcRegisteredUserDereg is entered!");

    (VOS_VOID)ulEvt;

    pstDeregReq = (IMSA_REG_DEREG_REQ_STRU *)pData;

    /* ȥע��D2 IMS */
    IMSA_RegSendImsMsgDereg(pstEntity->enRegType,pstDeregReq->ulLocalFlag);

    /* ����������ʱ�� */
    IMSA_RegTimerStart(&pstEntity->stProtectTimer, pstEntity->enRegType);

    /* �л�״̬ */
    IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_DEREGING);

    return VOS_TRUE;
}
VOS_UINT32 IMSA_RegFsmProcDeregingImsRegFailure(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    IMSA_INFO_LOG("IMSA_RegFsmProcDeregingImsRegFailure is entered!");

    (VOS_VOID)ulEvt;
    (VOS_VOID)pData;

    /* ֹͣ������ʱ�� */
    IMSA_RegTimerStop(&pstEntity->stProtectTimer);

    /* ֪ͨ�û� */
    (VOS_VOID)IMSA_RegSendIntraMsgDeregInd(pstEntity->enRegType, IMSA_REG_RESULT_SUCCESS, IMSA_RESULT_ACTION_NULL);

    /* �л�״̬ */
    IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_NOT_REGISTER);

    return VOS_TRUE;
}
VOS_UINT32 IMSA_RegFsmProcDeregingProtectTimeout(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    IMSA_INFO_LOG("IMSA_RegFsmProcDeregingProtectTimeout is entered!");

    (VOS_VOID)ulEvt;
    (VOS_VOID)pData;

    /**
     * �������ִ�е��Ǳ���ȥע�ᣬ�������ǲ����г�ʱ�¼�;
     * �������ִ�е�����ͨȥע�ᣬ��ʱ�󱾵�ȥע����ͬ��״̬����ȡ���Ѻõ���Ϊ
     */

    /* ����ȥע�� */
    IMSA_RegSendImsMsgDereg(pstEntity->enRegType, VOS_TRUE);

    /* ����������ʱ�� */
    IMSA_RegTimerStart(&pstEntity->stProtectTimer, pstEntity->enRegType);

    /* ��������״̬ */
    pstEntity->enExceptedStatus = IMSA_REG_STATUS_NOT_REGISTER;

    /* ״̬�л� */
    IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_ROLLING_BACK);

    return VOS_TRUE;
}


VOS_UINT32 IMSA_RegFsmProcDeregingUserReg(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    IMSA_REG_REG_REQ_STRU *pstRegReq = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegFsmProcDeregingUserReg is entered!");

    (VOS_VOID)ulEvt;

    pstRegReq = (IMSA_REG_REG_REQ_STRU *)pData;

    /* ��������״̬ */
    pstEntity->enExceptedStatus = IMSA_REG_STATUS_REGISTERED;

    /* ����ע����� */
    pstEntity->enPendingRegParam = pstRegReq->enAddrType;

    /* ״̬�л� */
    IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_PENDING);

    return VOS_TRUE;
}
VOS_UINT32 IMSA_RegFsmProcWaitRetryTimeout(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    VOS_CHAR aucUeAddr[IMSA_IPV6_ADDR_STRING_LEN+1];
    VOS_CHAR aucPcscfAddr[IMSA_IPV6_ADDR_STRING_LEN+1];

    IMSA_INFO_LOG("IMSA_RegFsmProcWaitRetryTimeout is entered!");

    (VOS_VOID)ulEvt;
    (VOS_VOID)pData;

    if (VOS_TRUE == IMSA_RegGetAddrByParamType( pstEntity->enRegType,
                                                IMSA_REG_ADDR_PARAM_SAME,
                                                aucUeAddr,
                                                aucPcscfAddr))
    {
        /* �����ȡ��ǰ����ʹ�õĵ�ַ�Գɹ�����ʹ�øõ�ַ�����·���ע�� */

        IMSA_CONN_UpdateNicPdpInfo();

        /* ��D2 IMS����ע����Ϣ */
        (VOS_VOID)IMSA_RegSendImsMsgReg(pstEntity->enRegType, aucUeAddr, aucPcscfAddr);

        /* ����������ʱ�� */
        IMSA_RegTimerStart(&pstEntity->stProtectTimer, pstEntity->enRegType);

        /* ״̬�л� */
        IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_REGISTERING);
    }
    else
    {
        /**
         * �����ߵ������֧��ԭ������ڵȴ����������ӶϿ���CONN��֪ͨSERVICE��
         * SERVICE��֪ͨREG����ȥע�ᣬ����WAIT RETRY״̬�»��ܵ�����ȥע������
         * ��WAIT RETRY״̬ʱ�ϲ㴦��REGING�������ٸ�ע��ģ�鷢ע������
         * ������֣��Ǿ���SERVICEģ���������
         */
    }

    return VOS_TRUE;
}


VOS_UINT32 IMSA_RegFsmProcWaitRetryUserDereg(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    IMSA_INFO_LOG("IMSA_RegFsmProcWaitRetryUserDereg is entered!");

    (VOS_VOID)ulEvt;
    (VOS_VOID)pData;

    /* ֹͣ���Զ�ʱ�� */
    IMSA_RegTimerStop(&pstEntity->stRetryTimer);

    /* ֪ͨ�û� */
    (VOS_VOID)IMSA_RegSendIntraMsgDeregInd(pstEntity->enRegType, IMSA_REG_RESULT_SUCCESS, IMSA_RESULT_ACTION_NULL);

    /* ״̬�л� */
    IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_NOT_REGISTER);

    return VOS_TRUE;
}
VOS_UINT32 IMSA_RegFsmProcPendingProtectTimeout(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    IMSA_INFO_LOG("IMSA_RegFsmProcPendingProtectTimeout is entered!");

    (VOS_VOID)ulEvt;
    (VOS_VOID)pData;

    /* ����ȥע�� */
    IMSA_RegSendImsMsgDereg(pstEntity->enRegType, VOS_TRUE);

    /* ����������ʱ�� */
    IMSA_RegTimerStart(&pstEntity->stProtectTimer, pstEntity->enRegType);

    /* ״̬�л� */
    IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_ROLLING_BACK);

    return VOS_TRUE;
}
VOS_UINT32 IMSA_RegFsmProcPendingUserReg(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    IMSA_REG_REG_REQ_STRU *pstRegReq = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegFsmProcPendingUserReg is entered!");

    (VOS_VOID)ulEvt;

    pstRegReq = (IMSA_REG_REG_REQ_STRU *)pData;

    /* ����ע����� */
    pstEntity->enPendingRegParam = pstRegReq->enAddrType;

    /* ��������״̬ */
    pstEntity->enExceptedStatus = IMSA_REG_STATUS_REGISTERED;

    return VOS_TRUE;
}


VOS_UINT32 IMSA_RegFsmProcPendingUserDereg(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    IMSA_REG_DEREG_REQ_STRU *pstDeregReq = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegFsmProcPendingUserDereg is entered!");

    (VOS_VOID)ulEvt;

    pstDeregReq = (IMSA_REG_DEREG_REQ_STRU *)pData;

    /* ����ע����� */
    pstEntity->ulPendingDeregParam = pstDeregReq->ulLocalFlag;

    /* ��������״̬ */
    pstEntity->enExceptedStatus = IMSA_REG_STATUS_NOT_REGISTER;

    return VOS_TRUE;
}


VOS_UINT32 IMSA_RegFsmProcPendingImsRegFailure(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    VOS_CHAR aucUeAddr[IMSA_IPV6_ADDR_STRING_LEN+1] = {0};
    VOS_CHAR aucPcscfAddr[IMSA_IPV6_ADDR_STRING_LEN+1] = {0};

    IMSA_INFO_LOG("IMSA_RegFsmProcPendingImsRegFailure is entered!");

    (VOS_VOID)ulEvt;
    (VOS_VOID)pData;

    /* ֹͣ������ʱ�� */
    IMSA_RegTimerStop(&pstEntity->stProtectTimer);

    /* �����ǰ״̬������״̬һ�£���֪ͨ�û� */
    if (IMSA_REG_STATUS_NOT_REGISTER == pstEntity->enExceptedStatus)
    {
        (VOS_VOID)IMSA_RegSendIntraMsgDeregInd(pstEntity->enRegType, IMSA_REG_RESULT_SUCCESS, IMSA_RESULT_ACTION_NULL);
    }
    /* �������״̬��ע�ᣬ����ע�� */
    else if (IMSA_REG_STATUS_REGISTERED == pstEntity->enExceptedStatus)
    {
        /* ���ݱ����ע�����ѡȡһ���ַ�Խ���ע�� */
        if (VOS_TRUE == IMSA_RegGetAddrByParamType(pstEntity->enRegType, pstEntity->enPendingRegParam, aucUeAddr, aucPcscfAddr))
        {
            IMSA_CONN_UpdateNicPdpInfo();

            (VOS_VOID)IMSA_RegSendImsMsgReg(pstEntity->enRegType, aucUeAddr, aucPcscfAddr);

            /* ״̬�л� */
            IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_REGISTERING);
        }
        else
        {
            IMSA_ERR_LOG("IMSA_RegFsmProcPendingImsRegFailure: select address pair failure");

            (VOS_VOID)IMSA_RegSendIntraMsgRegInd(pstEntity->enRegType, IMSA_REG_RESULT_FAIL_NO_ADDR_PAIR, IMSA_RESULT_ACTION_NULL);

            /* ״̬�л� */
            IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_NOT_REGISTER);
        }
    }
    else
    {
        IMSA_ERR_LOG("IMSA_RegFsmProcPendingImsRegFailure: invalid excepted status");

        /* ״̬�л� */
        IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_NOT_REGISTER);
    }

    return VOS_TRUE;
}
#if 0
VOS_UINT32 IMSA_RegFsmProcPendingImsRegSuccess(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    IMSA_INFO_LOG("IMSA_RegFsmProcPendingImsRegSuccess is entered!");

    (VOS_VOID)ulEvt;
    (VOS_VOID)pData;

    /* ֹͣ������ʱ�� */
    IMSA_RegTimerStop(&pstEntity->stProtectTimer);

    /* �����ǰ״̬������״̬һ�£���֪ͨ�û� */
    if (IMSA_REG_STATUS_REGISTERED == pstEntity->enExceptedStatus)
    {
        (VOS_VOID)IMSA_RegSendIntraMsgRegInd(pstEntity->enRegType, IMSA_REG_RESULT_SUCCESS, IMSA_RESULT_ACTION_NULL);
    }
    /* �������״̬��ȥע�ᣬ����ȥע�� */
    else if (IMSA_REG_STATUS_NOT_REGISTER == pstEntity->enExceptedStatus)
    {
        /* ����ȥע�� */
        IMSA_RegSendImsMsgDereg(pstEntity->enRegType, pstEntity->ulPendingDeregParam);

        /* ����������ʱ�� */
        IMSA_RegTimerStart(&pstEntity->stProtectTimer, pstEntity->enRegType);

        /* ״̬�л� */
        IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_DEREGING);
    }
    else
    {
        IMSA_ERR_LOG("IMSA_RegFsmProcPendingImsRegSuccess: invalid excepted status");
    }

    return VOS_TRUE;
}
#endif
/**
 * ״̬��������
 *
 * Roll-backʱ������ʱ�Ĵ����������Ӧ�þ������ⷢ���Ŷԣ�
 * ��ΪĿǰ��Roll-back��ͨ������ȥע����ʵ�ֵģ�������ע��
 * �����ϲ�Ӧ���г�ʱ�Ŷ�
 *
 * @param[in]   pstEntity   ע��ʵ��
 * @param[in]   ulEvt       �յ����¼�, �ο� #IMSA_REG_EVT_ENUM
 * @param[in]   pData       �¼���Ӧ��������������
 */
VOS_UINT32 IMSA_RegFsmProcRollingBackProtectTimeout(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    IMSA_INFO_LOG("IMSA_RegFsmProcRollingBackProtectTimeout is entered!");

    (VOS_VOID)ulEvt;
    (VOS_VOID)pData;

    /* ��������״̬֪ͨ�û� */
    switch (pstEntity->enExceptedStatus)
    {
    case IMSA_REG_STATUS_NOT_REGISTER:
        {
            /* ֹͣ������ʱ�� */
            IMSA_RegTimerStop(&pstEntity->stProtectTimer);

            /* ֪ͨ�û� */
            (VOS_VOID)IMSA_RegSendIntraMsgDeregInd(pstEntity->enRegType, IMSA_REG_RESULT_FAIL_TIMEOUT, IMSA_RESULT_ACTION_NULL);

            /* ״̬�л� */
            IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_NOT_REGISTER);
            break;
        }
    case IMSA_REG_STATUS_REGISTERED:
        {
            /* ֹͣ������ʱ�� */
            IMSA_RegTimerStop(&pstEntity->stProtectTimer);

            /* ֪ͨ�û� */
            (VOS_VOID)IMSA_RegSendIntraMsgRegInd(pstEntity->enRegType, IMSA_REG_RESULT_FAIL_TIMEOUT, IMSA_RESULT_ACTION_NULL);

            /* ״̬�л� */
            IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_NOT_REGISTER);
            break;
        }
    default:
        IMSA_ERR_LOG("IMSA_RegFsmProcRollingBackProtectTimeout: invalid excepted status");
    }

    return VOS_TRUE;
}
VOS_UINT32 IMSA_RegFsmProcRollingBackImsRegFailure(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    VOS_CHAR aucUeAddr[IMSA_IPV6_ADDR_STRING_LEN+1] = {0};
    VOS_CHAR aucPcscfAddr[IMSA_IPV6_ADDR_STRING_LEN+1] = {0};

    IMSA_INFO_LOG("IMSA_RegFsmProcRollingBackImsRegFailure is entered!");

    (VOS_VOID)ulEvt;
    (VOS_VOID)pData;

    /* ��������״̬֪ͨ�û� */
    switch (pstEntity->enExceptedStatus)
    {
    case IMSA_REG_STATUS_NOT_REGISTER:
        {
            /* ֹͣ������ʱ�� */
            IMSA_RegTimerStop(&pstEntity->stProtectTimer);

            /* ֪ͨ�û� */
            (VOS_VOID)IMSA_RegSendIntraMsgDeregInd(pstEntity->enRegType, IMSA_REG_RESULT_SUCCESS, IMSA_RESULT_ACTION_NULL);

            /* ״̬�л� */
            IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_NOT_REGISTER);
            break;
        }
    case IMSA_REG_STATUS_REGISTERED:
        /* �������״̬��ע�ᣬ����ݱ����ע�����ѡȡһ���ַ�Խ���ע�� */
        if (VOS_TRUE == IMSA_RegGetAddrByParamType(pstEntity->enRegType, pstEntity->enPendingRegParam, aucUeAddr, aucPcscfAddr))
        {
            IMSA_CONN_UpdateNicPdpInfo();

            (VOS_VOID)IMSA_RegSendImsMsgReg(pstEntity->enRegType, aucUeAddr, aucPcscfAddr);

            /* ״̬�л� */
            IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_REGISTERING);
        }
        else
        {
            IMSA_ERR_LOG("IMSA_RegFsmProcRollingBackImsRegFailure: select address pair failure");

            (VOS_VOID)IMSA_RegSendIntraMsgRegInd(pstEntity->enRegType, IMSA_REG_RESULT_FAIL_NO_ADDR_PAIR, IMSA_RESULT_ACTION_NULL);

            /* ״̬�л� */
            IMSA_REG_FSM_STATE_TRANS(pstEntity, IMSA_REG_STATUS_NOT_REGISTER);
        }
        break;

    default:
        IMSA_ERR_LOG("IMSA_RegFsmProcRollingBackImsRegFailure: invalid excepted status");
    }

    return VOS_TRUE;
}
VOS_UINT32 IMSA_RegFsmProcRollingBackUserReg(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    IMSA_REG_REG_REQ_STRU *pstRegReq = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegFsmProcRollingBackUserReg is entered!");

    (VOS_VOID)ulEvt;

    pstRegReq = (IMSA_REG_REG_REQ_STRU *)pData;

    /* ����ע����� */
    pstEntity->enPendingRegParam = pstRegReq->enAddrType;

    /* ��������״̬ */
    pstEntity->enExceptedStatus = IMSA_REG_STATUS_REGISTERED;

    return VOS_TRUE;
}


VOS_UINT32 IMSA_RegFsmProcRollingBackUserDereg(IMSA_REG_ENTITY_STRU *pstEntity, VOS_UINT32 ulEvt, VOS_VOID* pData)
{
    IMSA_REG_DEREG_REQ_STRU *pstDeregReq = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegFsmProcRollingBackUserDereg is entered!");

    (VOS_VOID)ulEvt;

    pstDeregReq = (IMSA_REG_DEREG_REQ_STRU *)pData;

    /* ����ע����� */
    pstEntity->ulPendingDeregParam = pstDeregReq->ulLocalFlag;

    /* ��������״̬ */
    pstEntity->enExceptedStatus = IMSA_REG_STATUS_NOT_REGISTER;

    return VOS_TRUE;
}



IMSA_REG_ENTITY_STRU* IMSA_RegEntityGetByType(IMSA_REG_TYPE_ENUM_UINT8 enRegType)
{
    IMSA_REG_MANAGER_STRU *pstRegCtx = IMSA_RegCtxGet();

    if (IMSA_REG_TYPE_NORMAL == enRegType)
    {
        return &pstRegCtx->stNormalRegEntity;
    }
    else
    {
        return &pstRegCtx->stEmcRegEntity;
    }
}


VOS_UINT32 IMSA_RegProcUserRegReqMsg(const VOS_VOID *pRcvMsg)
{
    IMSA_REG_ENTITY_STRU *pstRegEntity = VOS_NULL_PTR;
    IMSA_REG_REG_REQ_STRU *pstRegReqData = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegProcUserRegReqMsg is entered!");

    pstRegReqData = (IMSA_REG_REG_REQ_STRU *)pRcvMsg;
    pstRegEntity = IMSA_RegEntityGetByType(pstRegReqData->enRegType);

    return IMSA_FsmRun(pstRegEntity, IMSA_REG_EVT_USER_REG_REQ, (VOS_VOID *)pstRegReqData);
}


VOS_UINT32 IMSA_RegProcUserDeregReqMsg(const VOS_VOID *pRcvMsg)
{
    IMSA_REG_ENTITY_STRU *pstRegEntity = VOS_NULL_PTR;
    IMSA_REG_DEREG_REQ_STRU *pstDeregReqData = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegProcUserDeregReqMsg is entered!");

    pstDeregReqData = (IMSA_REG_DEREG_REQ_STRU *)pRcvMsg;
    pstRegEntity = IMSA_RegEntityGetByType(pstDeregReqData->enDeRegType);

    return IMSA_FsmRun(pstRegEntity, IMSA_REG_EVT_USER_DEREG_REQ, (VOS_VOID *)pstDeregReqData);
}



VOS_UINT32 IMSA_RegProcImsMsgState(const IMSA_IMS_OUTPUT_SERVICE_EVENT_STRU  *pstOutputService)
{
    IMSA_REG_ENTITY_STRU *pstRegEntity = VOS_NULL_PTR;
    VOS_UINT32 ulResult = VOS_FALSE;
    IMSA_IMS_OUTPUT_ERROR_STRU                      stImsErrorInfo = {0};

    IMSA_INFO_LOG1("IMSA_RegProcImsMsgState: ", pstOutputService->enServeState);

    if (pstOutputService->ulIsEmergency)
    {
        pstRegEntity = IMSA_RegEntityGetByType(IMSA_REG_TYPE_EMC);
    }
    else
    {
        pstRegEntity = IMSA_RegEntityGetByType(IMSA_REG_TYPE_NORMAL);
    }

    switch (pstOutputService->enServeState)
    {
    case IMSA_IMS_SERVICE_STATE_INACTIVE:
        ulResult = IMSA_FsmRun(pstRegEntity, IMSA_REG_EVT_IMS_REG_FAILURE, VOS_NULL_PTR);
        break;

    case IMSA_IMS_SERVICE_STATE_FAILED:
    case IMSA_IMS_SERVICE_STATE_UNKNOWN:

        /* Convert IMS extra parameter to REG data structure */
        ulResult = IMSA_RegParseImsError(pstOutputService, &stImsErrorInfo);

        if (IMSA_SUCC == ulResult)
        {
            ulResult = IMSA_FsmRun(pstRegEntity, IMSA_REG_EVT_IMS_REG_FAILURE, &stImsErrorInfo);
        }
        else
        {
            ulResult = IMSA_FsmRun(pstRegEntity, IMSA_REG_EVT_IMS_REG_FAILURE, VOS_NULL_PTR);
        }

        break;
    case IMSA_IMS_SERVICE_STATE_ACTIVE:
        ulResult = IMSA_FsmRun(pstRegEntity, IMSA_REG_EVT_IMS_REG_SUCCESS, VOS_NULL_PTR);
        break;
    case IMSA_IMS_SERVICE_STATE_REGISTERING:
    case IMSA_IMS_SERVICE_STATE_DEREGISTERING:
    case IMSA_IMS_SERVICE_STATE_ROAMING:
    default:
        IMSA_INFO_LOG1("IMSA_RegProcImsMsgState: state need not handle", pstOutputService->enServeState);
        break;
    }

    return ulResult;
}
VOS_VOID IMSA_RegProcTimeoutProtect(const VOS_VOID *pTimerMsg)
{
    IMSA_REG_TYPE_ENUM_UINT8 enRegType;
    IMSA_REG_ENTITY_STRU *pstRegEntity = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegProcTimeoutProtect is entered!");

    enRegType = (IMSA_REG_TYPE_ENUM_UINT8)PS_GET_REL_TIMER_PARA(pTimerMsg);

    pstRegEntity = IMSA_RegEntityGetByType(enRegType);

    (VOS_VOID)IMSA_FsmRun(pstRegEntity, IMSA_REG_EVT_TIMEOUT_PROTECT, VOS_NULL_PTR);
}


VOS_VOID IMSA_RegProcTimeoutRetry(const VOS_VOID *pTimerMsg)
{
    IMSA_REG_TYPE_ENUM_UINT8 enRegType;
    IMSA_REG_ENTITY_STRU *pstRegEntity = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegProcTimeoutRetry is entered!");

    enRegType = (IMSA_REG_TYPE_ENUM_UINT8)PS_GET_REL_TIMER_PARA(pTimerMsg);

    pstRegEntity = IMSA_RegEntityGetByType(enRegType);

    (VOS_VOID)IMSA_FsmRun(pstRegEntity, IMSA_REG_EVT_TIMEOUT_RETRY, VOS_NULL_PTR);
}




IMSA_REG_ADDR_PAIR_STRU* IMSA_RegAddrPairCreate(IMSA_IP_TYPE_ENUM_UINT8 enIpType,
                                                const VOS_CHAR *pacUeAddr,
                                                const VOS_CHAR *pacPcscfAddr)
{
    IMSA_REG_ADDR_PAIR_STRU *pstAddrPair = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegAddrPairCreate is entered!");

    pstAddrPair = (IMSA_REG_ADDR_PAIR_STRU*)IMSA_MEM_ALLOC(sizeof(IMSA_REG_ADDR_PAIR_STRU));
    if (pstAddrPair != VOS_NULL_PTR)
    {
        IMSA_MEM_SET(pstAddrPair, 0, sizeof(IMSA_REG_ADDR_PAIR_STRU));

        pstAddrPair->ucHasTryed = VOS_FALSE;
        pstAddrPair->enIpType = enIpType;
        IMSA_UtilStrNCpy(pstAddrPair->acUeAddr, pacUeAddr, IMSA_IPV6_ADDR_STRING_LEN);
        IMSA_UtilStrNCpy(pstAddrPair->acPcscfAddr, pacPcscfAddr, IMSA_IPV6_ADDR_STRING_LEN);
    }

    return pstAddrPair;
}



VOS_VOID IMSA_RegAddrPairDestroy(IMSA_REG_ADDR_PAIR_STRU *pstAddrPair)
{
    IMSA_INFO_LOG("IMSA_RegAddrPairDestroy is entered!");

    if (pstAddrPair)
    {
        IMSA_MEM_FREE(pstAddrPair);
    }
}


IMSA_REG_ADDR_PAIR_STRU* IMSA_RegAddrPairListAdd(IMSA_REG_ADDR_PAIR_STRU **ppstListHead, IMSA_REG_ADDR_PAIR_STRU *pstNode)
{
    IMSA_REG_ADDR_PAIR_STRU *pstTmp = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegAddrPairListAdd is entered!");

    if (VOS_NULL_PTR == *ppstListHead)
    {
        *ppstListHead = pstNode;

        pstNode->pstNext = VOS_NULL_PTR;
    }
    else
    {
        pstTmp = *ppstListHead;
        /* modify by jiqiang 2014.03.25 pclint fix error 960 begin */
        while (VOS_NULL_PTR != pstTmp->pstNext)
        {
            pstTmp = pstTmp->pstNext;
        }
        /* modify by jiqiang 2014.03.25 pclint fix error 960 end */
        pstTmp->pstNext = pstNode;

        pstNode->pstNext = VOS_NULL_PTR;
    }

    return pstNode;
}


VOS_VOID IMSA_RegAddrPairListDestroy(IMSA_REG_ADDR_PAIR_STRU *pstListHead)
{
    IMSA_REG_ADDR_PAIR_STRU *pstTmpAddrPair = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegAddrPairListDestroy is entered!");

    while (pstListHead)
    {
        pstTmpAddrPair = pstListHead;
        pstListHead = pstListHead->pstNext;

        IMSA_RegAddrPairDestroy(pstTmpAddrPair);
    }
}


VOS_VOID IMSA_RegAddrPairMgrInit(IMSA_REG_ADDR_PAIR_MGR_STRU *pstPairMgr)
{
    IMSA_INFO_LOG("IMSA_RegAddrPairMgrInit is entered!");

    IMSA_MEM_SET(pstPairMgr, 0, sizeof(IMSA_REG_ADDR_PAIR_MGR_STRU));
}



VOS_VOID IMSA_RegAddrPairMgrDeinit(const IMSA_REG_ADDR_PAIR_MGR_STRU *pstPairMgr)
{
    IMSA_INFO_LOG("IMSA_RegAddrPairMgrDeinit is entered!");

    IMSA_RegAddrPairListDestroy(pstPairMgr->pstIpv6List);
    IMSA_RegAddrPairListDestroy(pstPairMgr->pstIpv4List);
}


VOS_UINT32 IMSA_RegAddrPairMgrUpdateWithNewPcscfAddr(IMSA_REG_ADDR_PAIR_MGR_STRU *pstAddrPairMgr,
                                                     IMSA_IP_TYPE_ENUM_UINT8 enIpType,
                                                     const VOS_CHAR *pacPcscfAddr)
{
    VOS_CHAR *pacDstAddr = VOS_NULL_PTR;
    IMSA_REG_ADDR_PAIR_STRU *pstNewPair = VOS_NULL_PTR;
    IMSA_REG_ADDR_PAIR_STRU **ppstDstPairList = VOS_NULL_PTR;
    VOS_UINT8 ulResult = VOS_FALSE;

    IMSA_INFO_LOG("IMSA_RegAddrPairMgrUpdateWithNewPcscfAddr is entered!");

    /* ����PCSCF��ַ���;���Ҫ�����ĸ��б��Լ�ʹ���ĸ�UE��ַ */
    if (enIpType == IMSA_IP_TYPE_IPV4)
    {
        pacDstAddr = pstAddrPairMgr->acUeAddrIpv4;
        ppstDstPairList = &pstAddrPairMgr->pstIpv4List;
    }
    else if (enIpType == IMSA_IP_TYPE_IPV6)
    {
        pacDstAddr = pstAddrPairMgr->acUeAddrIpv6;
        ppstDstPairList = &pstAddrPairMgr->pstIpv6List;
    }
    else
    {
        IMSA_ERR_LOG("_RegAddrPairMgrUpdateWithNewPcscfAddr: invalid ip type");
        return VOS_FALSE;
    }

    if (0 != VOS_StrLen(pacDstAddr))
    {
        pstNewPair = IMSA_RegAddrPairCreate(enIpType, pacDstAddr, pacPcscfAddr);
        if (pstNewPair)
        {
            (VOS_VOID)IMSA_RegAddrPairListAdd(ppstDstPairList, pstNewPair);

            ulResult = VOS_TRUE;
        }
        else
        {
            IMSA_ERR_LOG("_RegAddrPairMgrUpdateWithNewPcscfAddr: alloc memory fail");
        }
    }

    return ulResult;
}
VOS_UINT32 IMSA_RegAddrPairMgrUpdateWithNewUeAddr(IMSA_REG_ADDR_PAIR_MGR_STRU *pstAddrPairMgr,
                                                  IMSA_IP_TYPE_ENUM_UINT8 enIpType,
                                                  const VOS_CHAR *pacUeAddr)
{
    IMSA_REG_ADDR_PAIR_STRU *pstNewPair = VOS_NULL_PTR;
    IMSA_REG_ADDR_PAIR_STRU **ppstDstPairList = VOS_NULL_PTR;
    VOS_UINT8 ulResult = VOS_FALSE;
    VOS_UINT8 i = 0;

    IMSA_INFO_LOG("IMSA_RegAddrPairMgrUpdateWithNewUeAddr is entered!");

    /* ���ݵ�ַ����ѡ��Ҫ���µ��б� */
    if (enIpType == IMSA_IP_TYPE_IPV4)
    {
        ppstDstPairList = &pstAddrPairMgr->pstIpv4List;
    }
    else if (enIpType == IMSA_IP_TYPE_IPV6)
    {
        ppstDstPairList = &pstAddrPairMgr->pstIpv6List;
    }
    else
    {
        IMSA_ERR_LOG("_RegAddrPairMgrUpdateWithNewUeAddr: invalid ip type");
        return VOS_FALSE;
    }

    for (i = 0; i < IMSA_PCSCF_MAX_NUM; i++)
    {
        /* ѡȡ��ַ����ƥ���PCSCF�������µĵ�ַ�� */
        if ((0 != VOS_StrLen(pstAddrPairMgr->astPcscfs[i].acIpAddr)) &&
            (pstAddrPairMgr->astPcscfs[i].enIpType == enIpType))
        {
            pstNewPair = IMSA_RegAddrPairCreate(enIpType, pacUeAddr, pstAddrPairMgr->astPcscfs[i].acIpAddr);
            if (pstNewPair)
            {
                (VOS_VOID)IMSA_RegAddrPairListAdd(ppstDstPairList, pstNewPair);

                ulResult = VOS_TRUE;
            }
            else
            {
                IMSA_ERR_LOG("_RegAddrPairMgrUpdateWithNewUeAddr: alloc memory fail");
            }
        }
    }

    return ulResult;
}

VOS_UINT32 IMSA_RegAddrPairMgrAddPcscfAddr(IMSA_REG_TYPE_ENUM_UINT8 ulRegType,
                                           IMSA_IP_TYPE_ENUM_UINT8 enIpType,
                                           IMSA_PCSCF_SRC_TYPE_UINT8 enSrcType,
                                           const VOS_CHAR *pacIpAddr)
{
    IMSA_REG_ENTITY_STRU *pstRegEntity = IMSA_RegEntityGetByType(ulRegType);
    VOS_UINT8 i = 0;

    IMSA_INFO_LOG("IMSA_RegAddrPairMgrAddPcscfAddr is entered!");

    for (i = 0; i < IMSA_PCSCF_MAX_NUM; i++)
    {
        if (0 == VOS_StrLen(pstRegEntity->stPairMgrCtx.astPcscfs[i].acIpAddr))
        {
            /* Ѱ��δʹ�õĿռ�洢�¼����PCSCF��ַ */
            pstRegEntity->stPairMgrCtx.astPcscfs[i].enIpType = enIpType;
            pstRegEntity->stPairMgrCtx.astPcscfs[i].enSrcType = enSrcType;

            IMSA_UtilStrNCpy(pstRegEntity->stPairMgrCtx.astPcscfs[i].acIpAddr, pacIpAddr, IMSA_IPV6_ADDR_STRING_LEN);

            /* ʹ���µ�PCSCF��ַ���µ�ַ���б� */
            (VOS_VOID)IMSA_RegAddrPairMgrUpdateWithNewPcscfAddr(&pstRegEntity->stPairMgrCtx, enIpType, pacIpAddr);

            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_VOID IMSA_RegGetAddrPairListHead
(
    IMSA_REG_TYPE_ENUM_UINT8            ulRegType,
    IMSA_IP_TYPE_ENUM_UINT8             enIpType,
    IMSA_REG_ADDR_PAIR_STRU          ***pppstListHead
)
{
    IMSA_REG_ADDR_PAIR_MGR_STRU        *pstAddrPairMgr      = VOS_NULL_PTR;
    IMSA_REG_ENTITY_STRU               *pstRegEntity        = IMSA_RegEntityGetByType(ulRegType);

    IMSA_INFO_LOG("IMSA_RegGetAddrPairListHead is entered!");

    pstAddrPairMgr = &pstRegEntity->stPairMgrCtx;

    if (enIpType == IMSA_IP_TYPE_IPV4)
    {
        if (VOS_NULL_PTR == pstAddrPairMgr->pstIpv4List)
        {
            return ;
        }

        *pppstListHead = &pstAddrPairMgr->pstIpv4List;
    }
    else
    {
        if (VOS_NULL_PTR == pstAddrPairMgr->pstIpv6List)
        {
            return ;
        }

        *pppstListHead = &pstAddrPairMgr->pstIpv6List;
    }
}


VOS_UINT32 IMSA_RegAddrPairMgrRmvPcscfAddr
(
    IMSA_REG_TYPE_ENUM_UINT8            ulRegType,
    IMSA_IP_TYPE_ENUM_UINT8             enIpType,
    VOS_CHAR                           *pacIpAddr
)
{
    VOS_UINT32                          i                   = IMSA_NULL;
    IMSA_REG_ENTITY_STRU               *pstRegEntity        = IMSA_RegEntityGetByType(ulRegType);
    IMSA_REG_ADDR_PAIR_MGR_STRU        *pstAddrPairMgr      = VOS_NULL_PTR;
    IMSA_REG_ADDR_PAIR_STRU            *pstTmpAddrPairPrior = VOS_NULL_PTR;
    IMSA_REG_ADDR_PAIR_STRU            *pstTmpAddrPair      = VOS_NULL_PTR;
    IMSA_REG_ADDR_PAIR_STRU           **ppstListHead        = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegAddrPairMgrRmvPcscfAddr is entered!");

    pstAddrPairMgr = &pstRegEntity->stPairMgrCtx;

    /* ���ȥ���������漰����ǰ����ʹ�õĵ�ַ�ԣ�CONNģ��Ḻ��֪ͨSERVICEģ��
       �����ͷŻ��ߵ�ǰע�����ʧЧ��SERVICEģ����֪ͨע��ģ�鱾��ȥע�᣻��ַ��
       �Ĺ���ֻ��Ҫɾ��P-CSCF��ַ��Ϣ�͹����ĵ�ַ�Լ��� */

    /* �����ǰע������е�P-CSCF��ַ�������ͬ�������currentָ�� */
    if (pstAddrPairMgr->pstCurrent != VOS_NULL_PTR)
    {
        if ((0 == VOS_StrCmp(pstAddrPairMgr->pstCurrent->acPcscfAddr, pacIpAddr))
            && (pstAddrPairMgr->pstCurrent->enIpType == enIpType))
        {
            pstAddrPairMgr->pstCurrent = VOS_NULL_PTR;
        }
    }

    /* ����洢��P-CSCF��ַ��Ϣ */
    for (i = 0; i < IMSA_PCSCF_MAX_NUM; i++)
    {
        if (0 == VOS_StrLen(pstAddrPairMgr->astPcscfs[i].acIpAddr))
        {
            continue ;
        }

        if ((0 == VOS_StrCmp(pstAddrPairMgr->astPcscfs[i].acIpAddr, pacIpAddr))
            && (pstAddrPairMgr->astPcscfs[i].enIpType == enIpType))
        {
            IMSA_MEM_SET(   &pstAddrPairMgr->astPcscfs[i],
                            0,
                            sizeof(IMSA_REG_PCSCF_STRU));
        }
    }

    IMSA_RegGetAddrPairListHead(ulRegType, enIpType, &ppstListHead);

    if (VOS_NULL_PTR == ppstListHead)
    {
        return IMSA_SUCC;
    }

    /* ������P-CSCF��صĵ�ַ�� */

    /* ���Ҫɾ���ĵ�ַ�����б�ĵ�һ����¼ */
    pstTmpAddrPair = *ppstListHead;
    if ((0 == VOS_StrCmp(pstTmpAddrPair->acPcscfAddr, pacIpAddr))
            && (pstTmpAddrPair->enIpType == enIpType))
    {
        /* �޸ı�ͷ */
        *ppstListHead = pstTmpAddrPair->pstNext;

         IMSA_RegAddrPairDestroy(pstTmpAddrPair);

         return IMSA_SUCC;
    }

    pstTmpAddrPairPrior = pstTmpAddrPair;
    pstTmpAddrPair      = pstTmpAddrPair->pstNext;
    while (pstTmpAddrPair != VOS_NULL_PTR)
    {
        if ((0 == VOS_StrCmp(pstTmpAddrPair->acPcscfAddr, pacIpAddr))
            && (pstTmpAddrPair->enIpType == enIpType))
        {
            /* ����ǰ��ַ�Դ��б���ɾ�� */
            pstTmpAddrPairPrior->pstNext = pstTmpAddrPair->pstNext;

            IMSA_RegAddrPairDestroy(pstTmpAddrPair);

            return IMSA_SUCC;
        }

        pstTmpAddrPairPrior = pstTmpAddrPair;
        pstTmpAddrPair      = pstTmpAddrPair->pstNext;
    }

    return IMSA_SUCC;
}

VOS_UINT32 IMSA_RegAddrPairMgrAddUeAddr(IMSA_REG_TYPE_ENUM_UINT8 ulRegType,
                                        IMSA_IP_TYPE_ENUM_UINT8 enIpType,
                                        const VOS_CHAR *pacIpAddr)
{
    IMSA_REG_ENTITY_STRU *pstRegEntity = IMSA_RegEntityGetByType(ulRegType);
    VOS_CHAR *pacDstAddr = VOS_NULL_PTR;
    VOS_UINT32 ulResult = VOS_FALSE;

    IMSA_INFO_LOG("IMSA_RegAddrPairMgrAddUeAddr is entered!");

    /* ���ݵ�ַ����ѡȡҪ�洢�ĵ�ַ */
    if (enIpType == IMSA_IP_TYPE_IPV6)
    {
        pacDstAddr = (VOS_CHAR *)pstRegEntity->stPairMgrCtx.acUeAddrIpv6;
    }
    else if (enIpType == IMSA_IP_TYPE_IPV4)
    {
        pacDstAddr = (VOS_CHAR *)pstRegEntity->stPairMgrCtx.acUeAddrIpv4;
    }
    else
    {
        IMSA_ERR_LOG("IMSA_RegAddrPairMgrAddUeAddr: invalid ip type");
        return VOS_FALSE;
    }

    /* ���Ŀ���ַδʹ�ã���洢�µ�UE��ַ */
    if (0 == VOS_StrLen(pacDstAddr))
    {
        IMSA_UtilStrNCpy(pacDstAddr, pacIpAddr, IMSA_IPV6_ADDR_STRING_LEN);

        /* ʹ���¼����UE��ַ���µ�ַ���б� */
        (VOS_VOID)IMSA_RegAddrPairMgrUpdateWithNewUeAddr(&pstRegEntity->stPairMgrCtx, enIpType, pacIpAddr);

        ulResult = VOS_TRUE;
    }

    return ulResult;
}
VOS_UINT32 IMSA_RegAddrPairMgrRmvUeAddr
(
    IMSA_REG_TYPE_ENUM_UINT8            ulRegType,
    IMSA_IP_TYPE_ENUM_UINT8             enIpType,
    VOS_CHAR                           *pacIpAddr
)
{
    IMSA_REG_ENTITY_STRU               *pstRegEntity        = IMSA_RegEntityGetByType(ulRegType);
    IMSA_REG_ADDR_PAIR_MGR_STRU        *pstAddrPairMgr      = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegAddrPairMgrRmvUeAddr is entered!");

    (VOS_VOID)pacIpAddr;

    pstAddrPairMgr = &pstRegEntity->stPairMgrCtx;

    /* ���ȥ���������漰����ǰ����ʹ�õĵ�ַ�ԣ�CONNģ��Ḻ��֪ͨSERVICEģ��
       �����ͷŻ��ߵ�ǰע�����ʧЧ��SERVICEģ����֪ͨע��ģ�鱾��ȥע�᣻��ַ��
       �Ĺ���ֻ��Ҫɾ��IP��ַ��Ϣ�͹����ĵ�ַ�Լ��� */

    /* �����ǰע������е�IP��ַ�������ͬ�������currentָ�� */
    if (pstAddrPairMgr->pstCurrent != VOS_NULL_PTR)
    {
        if (pstAddrPairMgr->pstCurrent->enIpType == enIpType)
        {
            pstAddrPairMgr->pstCurrent = VOS_NULL_PTR;
        }
    }

    /* ����洢��IP��ַ��Ϣ */
    if (enIpType == IMSA_IP_TYPE_IPV4)
    {
        IMSA_MEM_SET(   pstAddrPairMgr->acUeAddrIpv4,
                        0x0,
                        IMSA_IPV4_ADDR_STRING_LEN + 1);

        IMSA_RegAddrPairListDestroy(pstAddrPairMgr->pstIpv4List);

        pstAddrPairMgr->pstIpv4List = VOS_NULL_PTR;
    }
    else if (enIpType == IMSA_IP_TYPE_IPV6)
    {
        IMSA_MEM_SET(   pstAddrPairMgr->acUeAddrIpv6,
                        0x0,
                        IMSA_IPV6_ADDR_STRING_LEN + 1);

        IMSA_RegAddrPairListDestroy(pstAddrPairMgr->pstIpv6List);
        pstAddrPairMgr->pstIpv6List = VOS_NULL_PTR;
    }
    else
    {
        IMSA_ERR_LOG("IMSA_RegAddrPairListRmvPcscfAddr: invalid ip type");
    }

    return IMSA_SUCC;
}

VOS_UINT32 IMSA_RegAddrPairMgrGetCurrent(IMSA_REG_TYPE_ENUM_UINT8 ulRegType,
                                         VOS_CHAR  *pacUeAddr,
                                         VOS_CHAR  *pacPcscfAddr)
{
    IMSA_REG_ENTITY_STRU *pstRegEntity = IMSA_RegEntityGetByType(ulRegType);
    IMSA_REG_ADDR_PAIR_MGR_STRU *pstAddrMgr = VOS_NULL_PTR;
    IMSA_REG_ADDR_PAIR_STRU *pstDstAddrPair = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegAddrPairMgrGetCurrent is entered!");

    pstAddrMgr = &pstRegEntity->stPairMgrCtx;

    pstDstAddrPair = pstAddrMgr->pstCurrent;

    /* ���ؽ�� */
    if (VOS_NULL_PTR == pstDstAddrPair)
    {
        return VOS_FALSE;
    }
    else
    {
        IMSA_UtilStrNCpy(pacUeAddr, pstDstAddrPair->acUeAddr, IMSA_IPV6_ADDR_STRING_LEN + 1);
        IMSA_UtilStrNCpy(pacPcscfAddr, pstDstAddrPair->acPcscfAddr, IMSA_IPV6_ADDR_STRING_LEN + 1);

        IMSA_INFO_LOG(pacUeAddr);
        IMSA_INFO_LOG(pacPcscfAddr);

        return VOS_TRUE;
    }
}

VOS_VOID IMSA_RegAddrPairListClearAllUsedFlag(IMSA_REG_ADDR_PAIR_STRU *pstList)
{
    IMSA_INFO_LOG("IMSA_RegAddrPairListClearAllUsedFlag is entered!");

    while (pstList)
    {
        pstList->ucHasTryed = VOS_FALSE;

        pstList = pstList->pstNext;
    }
}


VOS_VOID IMSA_RegAddrPairMgrClearAllUsedFlag(IMSA_REG_TYPE_ENUM_UINT8 enRegType)
{
    IMSA_REG_ENTITY_STRU *pstRegEntity = IMSA_RegEntityGetByType(enRegType);

    IMSA_INFO_LOG("IMSA_RegAddrPairMgrClearAllUsedFlag is entered!");

    IMSA_RegAddrPairListClearAllUsedFlag(pstRegEntity->stPairMgrCtx.pstIpv4List);
    IMSA_RegAddrPairListClearAllUsedFlag(pstRegEntity->stPairMgrCtx.pstIpv6List);
}

IMSA_REG_ADDR_PAIR_STRU* IMSA_RegAddrPairListGetNextUnused(IMSA_REG_ADDR_PAIR_STRU *pstList)
{
    IMSA_INFO_LOG("IMSA_RegAddrPairListGetNextUnused is entered!");

    while (pstList)
    {
        if (VOS_FALSE == pstList->ucHasTryed)
        {
            return pstList;
        }

        pstList = pstList->pstNext;
    }

    return VOS_NULL_PTR;
}

IMSA_REG_ADDR_PAIR_STRU* IMSA_RegAddrPairMgrGetFirst(IMSA_REG_TYPE_ENUM_UINT8 ulRegType)
{
    IMSA_REG_ENTITY_STRU *pstRegEntity = IMSA_RegEntityGetByType(ulRegType);

    IMSA_INFO_LOG("IMSA_RegAddrPairMgrGetFirst is entered!");

    if (VOS_NULL_PTR != pstRegEntity->stPairMgrCtx.pstIpv6List)
    {
        pstRegEntity->stPairMgrCtx.pstCurrent = pstRegEntity->stPairMgrCtx.pstIpv6List;

        return pstRegEntity->stPairMgrCtx.pstIpv6List;
    }
    else if (VOS_NULL_PTR != pstRegEntity->stPairMgrCtx.pstIpv4List)
    {
        pstRegEntity->stPairMgrCtx.pstCurrent = pstRegEntity->stPairMgrCtx.pstIpv4List;

        return pstRegEntity->stPairMgrCtx.pstIpv4List;
    }
    else
    {
        return VOS_NULL_PTR;
    }
}

VOS_UINT32 IMSA_RegAddrPairMgrGetNextUnused(IMSA_REG_TYPE_ENUM_UINT8 ulRegType,
                                            VOS_CHAR *pacUeIp,
                                            VOS_CHAR *pacPcscfIp)
{
    IMSA_REG_ENTITY_STRU *pstRegEntity = IMSA_RegEntityGetByType(ulRegType);
    IMSA_REG_ADDR_PAIR_MGR_STRU *pstAddrMgr = VOS_NULL_PTR;
    IMSA_REG_ADDR_PAIR_STRU *pstDstAddrPair = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegAddrPairMgrGetNextUnused is entered!");

    pstAddrMgr = &pstRegEntity->stPairMgrCtx;

    /* �����û�е�ǰ��ַ�ԣ���ӵ�ַ�б��е�һ����ʼ */
    if (VOS_NULL_PTR == pstAddrMgr->pstCurrent)
    {
        pstDstAddrPair = IMSA_RegAddrPairMgrGetFirst(ulRegType);
    }
    /* ����Ѿ����ڵ�ַ�б� */
    else
    {
        /* ����Ѿ����ڵ�ǰ��ַ�ԣ������Ƚ���ǰ��ַ������Ϊ�Ѿ����Թ� */
        pstAddrMgr->pstCurrent->ucHasTryed = VOS_TRUE;

        /**
         * TODO: �����´ӵ�һ����ַ�Կ�ʼ����ʱ��
         *       ����ζ��Ҫ��֮ǰ�Ѿ���ǳ��Թ��ĵ�ַ��ȫ����������
         */

        /**
         * ����ӵ�ǰλ�ÿ�ʼ����ң�Ŀǰ�ݲ�������ǰ�ҡ�
         * ��Ҫ�ǳ������ȼ����һ�δ���Թ��ĵ�ַ���ڵ�ǰ��ַ��֮ǰ���ܵ�
         * �����磺
         *     �տ�ʼʹ��IPv4��ַע�ᣬ�ڼ�IPv6�����ӽ����ɹ�����ʱ����
         * ��IPv6���ɵĵ�ַ�Կ��ܻ��ŵ���ǰʹ��IPv4��ǰ�ߡ�
         * Ŀǰ�ݲ�֧�����ֳ���
         */

        /**
         * �����ǰ��IPv6����IPv6���ȣ����Դӵ�ǰλ��(��IPv6�б�)�����ң�
         * ���IPv6�б�δ�ҵ������ٳ��Դ�IPv4���б��в���
         */
        if (IMSA_IP_TYPE_IPV6 == pstAddrMgr->pstCurrent->enIpType)
        {
            pstDstAddrPair = IMSA_RegAddrPairListGetNextUnused(pstAddrMgr->pstCurrent);
            if (VOS_NULL_PTR == pstDstAddrPair)
            {
                pstDstAddrPair = IMSA_RegAddrPairListGetNextUnused(pstAddrMgr->pstIpv4List);
            }
        }
        else
        /**
         * �����ǰ��IPv4����IPv6���ȣ������ȴ�IPv6�б��������ң�
         * ���IPv6�б�δ�ҵ������ٳ��Դ�IPv4�б��м�������
         */
        {
            pstDstAddrPair = IMSA_RegAddrPairListGetNextUnused(pstAddrMgr->pstIpv6List);
            if (VOS_NULL_PTR == pstDstAddrPair)
            {
                pstDstAddrPair = IMSA_RegAddrPairListGetNextUnused(pstAddrMgr->pstCurrent);
            }
        }
    }

    /* ���ؽ�� */
    if (VOS_NULL_PTR == pstDstAddrPair)
    {
        return VOS_FALSE;
    }
    else
    {
        /* ��currentָ��ָ���ȡ���ĵ�ַ�Ե�ַ */
        pstAddrMgr->pstCurrent = pstDstAddrPair;

        IMSA_UtilStrNCpy(pacUeIp, pstDstAddrPair->acUeAddr, IMSA_IPV6_ADDR_STRING_LEN);
        IMSA_UtilStrNCpy(pacPcscfIp, pstDstAddrPair->acPcscfAddr, IMSA_IPV6_ADDR_STRING_LEN);

        IMSA_INFO_LOG(pacUeIp);
        IMSA_INFO_LOG(pacPcscfIp);

        return VOS_TRUE;
    }
}


VOS_UINT32 IMSA_RegGetRegedPara
(
    IMSA_REG_TYPE_ENUM_UINT8            ulRegType,
    VOS_CHAR                           *pacUeAddr,
    VOS_CHAR                           *pacPcscfAddr
)
{
    IMSA_REG_ENTITY_STRU               *pstRegEntity = VOS_NULL_PTR;

    IMSA_INFO_LOG("IMSA_RegGetRegedPara is entered!");

    if ((VOS_NULL_PTR == pacUeAddr) || (VOS_NULL_PTR == pacPcscfAddr)
        || (ulRegType >= IMSA_REG_TYPE_BUTT))
    {
        IMSA_ERR_LOG("IMSA_RegGetRegedPara:Input Para Illegal");

        return IMSA_FAIL;
    }

    pstRegEntity = IMSA_RegEntityGetByType(ulRegType);

    if (IMSA_REG_STATUS_REGISTERED != pstRegEntity->enStatus)
    {
        return IMSA_FAIL;
    }

    if (VOS_FALSE == IMSA_RegGetAddrByParamType(    ulRegType,
                                                    IMSA_REG_ADDR_PARAM_SAME,
                                                    pacUeAddr,
                                                    pacPcscfAddr))
    {
        return IMSA_FAIL;
    }
    else
    {
        return IMSA_SUCC;
    }
}

VOS_VOID IMSA_RegSaveRegedPara
(
    IMSA_REG_TYPE_ENUM_UINT8            ulRegType,
    VOS_UINT8                           ucCid,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
)
{
    IMSA_CONTROL_MANAGER_STRU              *pstImsaControlManager   = VOS_NULL_PTR;
    /* IMSA_REG_ENTITY_STRU                   *pstRegEntity        = VOS_NULL_PTR; */
    VOS_UINT32                              ulRslt          = IMSA_FAIL;
    VOS_CHAR                                acRegUeAddr[IMSA_IPV6_ADDR_STRING_LEN + 1] = {0};
    VOS_CHAR                                acRegPcscfAddr[IMSA_IPV6_ADDR_STRING_LEN + 1] = {0};

    IMSA_INFO_LOG("IMSA_RegSaveRegedPara is entered!");

    pstImsaControlManager = IMSA_GetControlManagerAddress();

    if (IMSA_REG_TYPE_NORMAL == ulRegType)
    {
        ulRslt = IMSA_RegGetRegedPara(  ulRegType,
                                        acRegUeAddr,
                                        acRegPcscfAddr);

        /* ����ܻ�ȡ��ע��Ĳ���������ɾ��������Ϣǰ���б���  */
        if (IMSA_FAIL != ulRslt)
        {
            /* ɾ��������Ϣǰ�������ͷų��ص����ͺ�CID */
            pstImsaControlManager->stNrmRegParaInfo.ucCid = ucCid;
            pstImsaControlManager->stNrmRegParaInfo.enIpType = enPdpType;

            /* ����IP��ַ */
            IMSA_MEM_CPY( pstImsaControlManager->stNrmRegParaInfo.acUeAddr,
                          acRegUeAddr,
                          IMSA_IPV6_ADDR_STRING_LEN + 1);

            /* ����P-CSCF��ַ */
            IMSA_MEM_CPY( pstImsaControlManager->stNrmRegParaInfo.acPcscfAddr,
                          acRegPcscfAddr,
                          IMSA_IPV6_ADDR_STRING_LEN + 1);
        }
    }
    else
    {
        ulRslt = IMSA_RegGetRegedPara(  ulRegType,
                                        acRegUeAddr,
                                        acRegPcscfAddr);

        /* ����ܻ�ȡ��ע��Ĳ���������ɾ��������Ϣǰ���б���  */
        if (IMSA_FAIL != ulRslt)
        {
            /* ɾ��������Ϣǰ�������ͷų��ص����ͺ�CID */
            pstImsaControlManager->stEmcRegParaInfo.ucCid = ucCid;
            pstImsaControlManager->stEmcRegParaInfo.enIpType = enPdpType;

            /* ����IP��ַ */
            IMSA_MEM_CPY( pstImsaControlManager->stEmcRegParaInfo.acUeAddr,
                          acRegUeAddr,
                          IMSA_IPV6_ADDR_STRING_LEN + 1);

            /* ����P-CSCF��ַ */
            IMSA_MEM_CPY( pstImsaControlManager->stEmcRegParaInfo.acPcscfAddr,
                          acRegPcscfAddr,
                          IMSA_IPV6_ADDR_STRING_LEN + 1);
        }
    }
}
VOS_VOID IMSA_RegEntityInit(IMSA_REG_ENTITY_STRU *pstRegEntity)
{
    IMSA_INFO_LOG("IMSA_RegEntityInit IMSA_RegEntityInitis entered!");

    IMSA_MEM_SET(pstRegEntity, 0, sizeof(IMSA_REG_ENTITY_STRU));

    pstRegEntity->stProtectTimer.usName = TI_IMSA_REG_PROTECT;
    pstRegEntity->stProtectTimer.ucMode = VOS_RELTIMER_NOLOOP;
    pstRegEntity->stProtectTimer.ulTimerLen = IMSA_REG_TIMER_LEN_PROTECT;

    pstRegEntity->stRetryTimer.usName = TI_IMSA_REG_RETRY;
    pstRegEntity->stRetryTimer.ucMode = VOS_RELTIMER_NOLOOP;
    pstRegEntity->stRetryTimer.ulTimerLen = IMSA_REG_TIMER_LEN_RETRY;

    IMSA_RegAddrPairMgrInit(&pstRegEntity->stPairMgrCtx);
}


VOS_VOID IMSA_RegEntityDeinit(IMSA_REG_ENTITY_STRU *pstRegEntity)
{
    IMSA_INFO_LOG("IMSA_RegEntityDeinit is entered!");

    if (IMSA_IsTimerRunning(&pstRegEntity->stProtectTimer))
    {
        IMSA_RegTimerStop(&pstRegEntity->stProtectTimer);
    }

    if (IMSA_IsTimerRunning(&pstRegEntity->stRetryTimer))
    {
        IMSA_RegTimerStop(&pstRegEntity->stRetryTimer);
    }

    IMSA_RegAddrPairMgrDeinit(&pstRegEntity->stPairMgrCtx);

    IMSA_RegEntityInit(pstRegEntity);
}


VOS_VOID IMSA_RegMgrInit(VOS_VOID)
{
    IMSA_REG_MANAGER_STRU *pstRegCtx = IMSA_RegCtxGet();

    IMSA_INFO_LOG("IMSA_RegMgrInit is entered!");

    IMSA_MEM_SET(pstRegCtx, 0, sizeof(IMSA_REG_MANAGER_STRU));

    IMSA_RegEntityInit(&pstRegCtx->stNormalRegEntity);
    pstRegCtx->stNormalRegEntity.enRegType = IMSA_REG_TYPE_NORMAL;
    IMSA_RegEntityInit(&pstRegCtx->stEmcRegEntity);
    pstRegCtx->stEmcRegEntity.enRegType = IMSA_REG_TYPE_EMC;
}


VOS_VOID IMSA_RegMgrDeinit( VOS_VOID )
{
    IMSA_REG_MANAGER_STRU *pstRegCtx = IMSA_RegCtxGet();

    IMSA_INFO_LOG("IMSA_RegMgrDeinit is entered!");

    IMSA_RegEntityDeinit(&pstRegCtx->stNormalRegEntity);
    pstRegCtx->stNormalRegEntity.enRegType = IMSA_REG_TYPE_NORMAL;
    IMSA_RegEntityDeinit(&pstRegCtx->stEmcRegEntity);
    pstRegCtx->stEmcRegEntity.enRegType = IMSA_REG_TYPE_EMC;

    pstRegCtx->stImsMsg.bitOpImsMsg = VOS_FALSE;
}

/*****************************************************************************
 Function Name   : IMSA_RegPrintNrmRegState
 Description     :
 Input           : None
 Output          : None
 Return          : VOS_INT32

 History         :
    1.lihong00150010    2013-08-29  Draft Enact

*****************************************************************************/
VOS_VOID IMSA_RegPrintNrmRegState( VOS_VOID )
{
    IMSA_REG_ENTITY_STRU *pstRegEntity = VOS_NULL_PTR;

    pstRegEntity = IMSA_RegEntityGetByType(IMSA_REG_TYPE_NORMAL);

    switch (pstRegEntity->enStatus)
    {
        case IMSA_REG_STATUS_NOT_REGISTER:
            IMSA_INFO_LOG("Current Normal Reg State: NOT REGISTER");
            break;
        case IMSA_REG_STATUS_REGISTERING:
            IMSA_INFO_LOG("Current Normal Reg State: REGISTERING");
            break;
        case IMSA_REG_STATUS_REGISTERED:
            IMSA_INFO_LOG("Current Normal Reg State: REGISTERED");
            break;
        case IMSA_REG_STATUS_DEREGING:
            IMSA_INFO_LOG("Current Normal Reg State: DEREGISTERING");
            break;
        case IMSA_REG_STATUS_WAIT_RETRY:
            IMSA_INFO_LOG("Current Normal Reg State: WATI RETRY");
            break;
        case IMSA_REG_STATUS_PENDING:
            IMSA_INFO_LOG("Current Normal Reg State: PENDING");
            break;
        case IMSA_REG_STATUS_ROLLING_BACK:
            IMSA_INFO_LOG("Current Normal Reg State: ROLLING BACK");
            break;
        default:
            IMSA_INFO_LOG("Current Normal Reg State: UNKNOWN");
            break;
    }
}

/*****************************************************************************
 Function Name   : IMSA_RegPrintEmcRegState
 Description     :
 Input           : None
 Output          : None
 Return          : VOS_INT32

 History         :
    1.lihong00150010    2013-08-29  Draft Enact

*****************************************************************************/
VOS_VOID IMSA_RegPrintEmcRegState( VOS_VOID )
{
    IMSA_REG_ENTITY_STRU *pstRegEntity = VOS_NULL_PTR;

    pstRegEntity = IMSA_RegEntityGetByType(IMSA_REG_TYPE_EMC);

    switch (pstRegEntity->enStatus)
    {
        case IMSA_REG_STATUS_NOT_REGISTER:
            IMSA_INFO_LOG("Current Emc Reg State: NOT REGISTER");
            break;
        case IMSA_REG_STATUS_REGISTERING:
            IMSA_INFO_LOG("Current Emc Reg State: REGISTERING");
            break;
        case IMSA_REG_STATUS_REGISTERED:
            IMSA_INFO_LOG("Current Emc Reg State: REGISTERED");
            break;
        case IMSA_REG_STATUS_DEREGING:
            IMSA_INFO_LOG("Current Emc Reg State: DEREGISTERING");
            break;
        case IMSA_REG_STATUS_WAIT_RETRY:
            IMSA_INFO_LOG("Current Emc Reg State: WAIT RETRY");
            break;
        case IMSA_REG_STATUS_PENDING:
            IMSA_INFO_LOG("Current Emc Reg State: PENDING");
            break;
        case IMSA_REG_STATUS_ROLLING_BACK:
            IMSA_INFO_LOG("Current Emc Reg State: ROLLING BACK");
            break;
        default:
            IMSA_INFO_LOG("Current Emc Reg State: UNKNOWN");
            break;
    }
}
/*lint +e961*/
/*lint +e960*/

#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
/* end of ImsaRegManagement.c */
