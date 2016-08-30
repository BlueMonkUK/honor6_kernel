

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include  "TafMmaMsgPrioCompare.h"
#include  "TafFsm.h"
#include  "Taf_Tafm_Remote.h"
#include  "TafOamInterface.h"
#include  "TafLog.h"
#include  "TafMmaSndInternalMsg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_TAF_MMA_MSG_PRIO_COMPARE_C

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/

TAF_MMA_MSG_COMPARE_STRU g_astMmaMsgPrioCompareWithPhoneModeTbl[] =
{
    /* phone mode�������յ�OM��ģʽ�����������Ϣ���ȼ��Ƚ� */
    TAF_MMA_COMPARE_TBL_ITEM(TAF_BuildEventType(WUEPS_PID_OM, OAM_MMA_PHONE_MODE_SET_REQ),
                             TAF_MMA_ComparePhoneModeSetReqPrioWithPhoneMode),

    /* ģʽ��ѯ����Ҫ�Ƚ� */



    /* phone mode�������յ�ģʽ�����������Ϣ���ȼ��Ƚ� */
    TAF_MMA_COMPARE_TBL_ITEM(TAF_BuildEventType(WUEPS_PID_TAF, ID_TAF_MMA_PHONE_MODE_SET_REQ),
                             TAF_MMA_ComparePhoneModeSetReqPrioWithPhoneMode),

    /* phone mode�������յ�MMA���ڲ���״ָ̬ʾ����Ϣ���ȼ��Ƚ� */
    TAF_MMA_COMPARE_TBL_ITEM(TAF_BuildEventType(WUEPS_PID_MMA, MMA_MMA_INTER_USIM_STATUS_CHANGE_IND),
                             TAF_MMA_CompareMmaInterUsimStatusChangeIndPrioWithPhoneMode),


    /* phone mode�������յ�TAF��SYSCFG�����������Ϣ���ȼ��Ƚ� */
    TAF_MMA_COMPARE_TBL_ITEM(TAF_BuildEventType(WUEPS_PID_TAF, ID_TAF_MMA_SYS_CFG_SET_REQ),
                             TAF_MMA_CompareAtSyscfgReqPrioWithPhoneMode),

    /* phone mode�������յ�AT��list�����������Ϣ���ȼ��Ƚ� */
    TAF_MMA_COMPARE_TBL_ITEM(TAF_BuildEventType(WUEPS_PID_AT, TAF_MSG_MMA_PLMN_LIST),
                             TAF_MMA_CompareAtPlmnListReqPrioWithPhoneMode),

    /* phone mode�������յ�AT���û�ָ�������������Ϣ���ȼ��Ƚ� */
    TAF_MMA_COMPARE_TBL_ITEM(TAF_BuildEventType(WUEPS_PID_AT, TAF_MSG_MMA_PLMN_USER_SEL),
                             TAF_MMA_CompareAtPlmnUserSelReqPrioWithPhoneMode),

    /* phone mode�������յ�AT��attach�������Ϣ���ȼ��Ƚ� */
    TAF_MMA_COMPARE_TBL_ITEM(TAF_BuildEventType(WUEPS_PID_AT, TAF_MSG_MMA_ATTACH),
                             TAF_MMA_CompareAtAttachPrioWithPhoneMode),


    /* phone mode�������յ�AT��detach�������Ϣ���ȼ��Ƚ� */
    TAF_MMA_COMPARE_TBL_ITEM(TAF_BuildEventType(WUEPS_PID_TAF, ID_TAF_MMA_DETACH_REQ),
                             TAF_MMA_CompareAtDetachPrioWithPhoneMode),

    /* phone mode�������յ�AT���Զ���ѡ�������Ϣ���ȼ��Ƚ� */
    TAF_MMA_COMPARE_TBL_ITEM(TAF_BuildEventType(WUEPS_PID_AT, TAF_MSG_MMA_PLMN_RESEL),
                             TAF_MMA_CompareAtPlmnReselPrioWithPhoneMode),                         
};



TAF_MMA_FSM_MSG_COMPARE_STRU g_astMmaMsgCompareTbl[] =
{
     /* phone mode ״̬���ȽϺ�����*/
    TAF_MMA_FSM_COMPARE_TBL_ITEM(TAF_MMA_FSM_PHONE_MODE,
                                 g_astMmaMsgPrioCompareWithPhoneModeTbl),

};

/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

VOS_UINT32 TAF_MMA_ComparePhoneModeSetReqPrioWithPhoneMode(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
)
{
    *penAbortType   = TAF_MMA_ABORT_BUTT;
    
    return TAF_MMA_MSG_COMPARE_PRIO_RSLT_STORE;
}



VOS_UINT32 TAF_MMA_CompareAtSyscfgReqPrioWithPhoneMode(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
)
{
    *penAbortType   = TAF_MMA_ABORT_BUTT;
    
    return TAF_MMA_MSG_COMPARE_PRIO_RSLT_STORE;
}



VOS_UINT32 TAF_MMA_CompareMmaInterUsimStatusChangeIndPrioWithPhoneMode(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
)
{
    *penAbortType   = TAF_MMA_ABORT_BUTT;
    
    return TAF_MMA_MSG_COMPARE_PRIO_RSLT_STORE;
}


VOS_UINT32 TAF_MMA_CompareAtPlmnListReqPrioWithPhoneMode(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
)
{
    *penAbortType   = TAF_MMA_ABORT_BUTT;
    
    return TAF_MMA_MSG_COMPARE_PRIO_RSLT_STORE;
}




VOS_UINT32 TAF_MMA_CompareAtPlmnUserSelReqPrioWithPhoneMode(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
)
{
    *penAbortType   = TAF_MMA_ABORT_BUTT;
    
    return TAF_MMA_MSG_COMPARE_PRIO_RSLT_STORE;
}



VOS_UINT32 TAF_MMA_CompareAtAttachPrioWithPhoneMode(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
)
{
    *penAbortType   = TAF_MMA_ABORT_BUTT;
    
    return TAF_MMA_MSG_COMPARE_PRIO_RSLT_STORE;
}



VOS_UINT32 TAF_MMA_CompareAtDetachPrioWithPhoneMode(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
)
{
    *penAbortType   = TAF_MMA_ABORT_BUTT;
    
    return TAF_MMA_MSG_COMPARE_PRIO_RSLT_STORE;
}


VOS_UINT32 TAF_MMA_CompareAtPlmnReselPrioWithPhoneMode(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
)
{
    *penAbortType   = TAF_MMA_ABORT_BUTT;
    
    return TAF_MMA_MSG_COMPARE_PRIO_RSLT_STORE;
}





TAF_MMA_MSG_COMPARE_FUNC  TAF_MMA_FindMsgPrioCompareFun(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_FSM_ID_ENUM_UINT32          enFsmId
)
{
    TAF_MMA_FSM_MSG_COMPARE_STRU       *pstFsmCompareTbl = VOS_NULL_PTR;
    VOS_UINT32                          i;
    VOS_UINT32                          ulTblSize;

    ulTblSize = sizeof(g_astMmaMsgCompareTbl)/sizeof(TAF_MMA_FSM_MSG_COMPARE_STRU);

    for ( i = 0 ; i < ulTblSize ; i++ )
    {
        if ( enFsmId == g_astMmaMsgCompareTbl[i].enFsmId)
        {
            pstFsmCompareTbl = &g_astMmaMsgCompareTbl[i];
            break;
        }
    }

    /* δ���ҵ���״̬���ıȽϺ�����,ֱ�ӷ��ؿ�ָ�� */
    if ( VOS_NULL_PTR == pstFsmCompareTbl )
    {
        return VOS_NULL_PTR;
    }

    for ( i = 0 ; i < pstFsmCompareTbl->ulSize; i++ )
    {
        /* ���Ҳ�ͬ�¼��ıȽϺ��� */
        if ( pstFsmCompareTbl->pfCompareTbl[i].ulCurrEventType == ulEventType)
        {
            return pstFsmCompareTbl->pfCompareTbl[i].pfCompareFun;
        }
    }

    /* δ�ҵ�������ֱ�ӷ��ؿ�ָ�� */
    return VOS_NULL_PTR;
}
TAF_MMA_MSG_COMPARE_PRIO_RSLT_ENUM_UINT32 TAF_MMA_GetMsgComparePrioRslt(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
)
{
    TAF_MMA_MSG_COMPARE_FUNC                    pfCompareFunc;
    TAF_MMA_FSM_ID_ENUM_UINT32                  enCurrFsmId;
    TAF_MMA_MSG_COMPARE_PRIO_RSLT_ENUM_UINT32   enRslt;

    /*  ��ȡ��ǰ״̬����ID */
    enCurrFsmId         = TAF_MMA_GetCurrFsmId();

    if (TAF_MMA_FSM_BUTT <= enCurrFsmId)
    {
        TAF_ERROR_LOG(WUEPS_PID_MMA, "ERROR:FsmId Error");
    }

    /*  ��ȡ�ȽϺ��� TAF_MMA_FindMsgPrioCompareFun() */
    pfCompareFunc       = TAF_MMA_FindMsgPrioCompareFun(ulEventType, pstMsg, enCurrFsmId);

    /* δ�ҵ�������,��Ĭ����Ϣ���� */
    if ( VOS_NULL_PTR == pfCompareFunc )
    {
        *penAbortType = TAF_MMA_ABORT_BUTT;
        return TAF_MMA_MSG_COMPARE_PRIO_RSLT_DISCARD;
    }

    /* ���ñȽϺ���,�������ȼ��ʹ������ */
    enRslt = pfCompareFunc(ulEventType, pstMsg, penAbortType);

    return enRslt;

}




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

