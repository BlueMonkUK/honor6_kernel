

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "vos.h"
#include  "PsTypeDef.h"
#include  "TafMmaCtx.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define TAF_MMA_UNIVERAL_MSG_ID                (0xFFFFFFFF)


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

enum TAF_MMA_ABORT_FSM_TYPE
{
    TAF_MMA_ABORT_FSM_IMMEDIATELY           = 0,
    TAF_MMA_ABORT_FSM_DELAY                 = 1,
    TAF_MMA_ABORT_BUTT
};
typedef VOS_UINT8 TAF_MMA_ABORT_FSM_TYPE_UINT8;


enum TAF_MMA_MSG_COMPARE_PRIO_RSLT_ENUM
{
    TAF_MMA_MSG_COMPARE_PRIO_RSLT_ABORT              ,                          /* �����ȼ���Ϣ�жϵ�ǰ���� */
    TAF_MMA_MSG_COMPARE_PRIO_RSLT_STORE              ,                          /* ��Ҫ�������Ϣ */
    TAF_MMA_MSG_COMPARE_PRIO_RSLT_INITFSM            ,                          /* ����״̬���д��� */
    TAF_MMA_MSG_COMPARE_PRIO_RSLT_DISCARD            ,                          /* ֱ�Ӷ�������Ϣ */

    TAF_MMA_MSG_COMPARE_PRIO_RSLT_BUTT
};
typedef VOS_UINT32 TAF_MMA_MSG_COMPARE_PRIO_RSLT_ENUM_UINT32;

/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  7 STRUCT����
*****************************************************************************/


typedef VOS_UINT32 (*TAF_MMA_MSG_COMPARE_FUNC)(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
);


typedef struct
{
    VOS_UINT32                          ulCurrEventType;        /* ��Ƚϵ���Ϣ */
    TAF_MMA_MSG_COMPARE_FUNC            pfCompareFun;           /* �ȽϺ��� */
}  TAF_MMA_MSG_COMPARE_STRU;


typedef struct
{
    TAF_MMA_FSM_ID_ENUM_UINT32          enFsmId;                /* ��ǰ���е�FSM ID */
    VOS_UINT32                          ulSize;                 /* ������Ĵ�С�ֽ�������ACT�ṹ�Ĵ�С */
    TAF_MMA_MSG_COMPARE_STRU           *pfCompareTbl;           /* �ȽϺ����� */
}  TAF_MMA_FSM_MSG_COMPARE_STRU;

/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
TAF_MMA_MSG_COMPARE_PRIO_RSLT_ENUM_UINT32 TAF_MMA_GetMsgComparePrioRslt(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
);


VOS_UINT32 TAF_MMA_ComparePhoneModeSetReqPrioWithPhoneMode(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
);

VOS_UINT32 TAF_MMA_CompareAtSyscfgReqPrioWithPhoneMode(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
);


VOS_UINT32 TAF_MMA_CompareAtPlmnListReqPrioWithPhoneMode(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
);

VOS_UINT32 TAF_MMA_CompareMmaInterUsimStatusChangeIndPrioWithPhoneMode(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
);


VOS_UINT32 TAF_MMA_CompareAtPlmnUserSelReqPrioWithPhoneMode(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
);

VOS_UINT32 TAF_MMA_CompareAtAttachPrioWithPhoneMode(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
);


VOS_UINT32 TAF_MMA_CompareAtDetachPrioWithPhoneMode(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
);


VOS_UINT32 TAF_MMA_CompareAtPlmnReselPrioWithPhoneMode(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg,
    TAF_MMA_ABORT_FSM_TYPE_UINT8       *penAbortType
);







    



#define TAF_MMA_COMPARE_TBL_ITEM(ulCurrMsgType, pCompareFun)\
{\
    (ulCurrMsgType),\
    ((pCompareFun))\
}


#define TAF_MMA_FSM_COMPARE_TBL_ITEM(enFsmId, astFsmCompareTbl)\
{\
    ( enFsmId),\
    ((sizeof(astFsmCompareTbl))/sizeof(TAF_MMA_MSG_COMPARE_STRU)),\
    (astFsmCompareTbl)\
}



#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


