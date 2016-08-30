
#ifndef _TAF_MMA_CTX_H_
#define _TAF_MMA_CTX_H_

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/

#include  "vos.h"
#include  "PsTypeDef.h"
#include  "TafAppMma.h"
#include  "TafFsm.h"
#include  "TafMmaTimerMgmt.h"
#include  "TafSdcCtx.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  1 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define TAF_MMA_MAX_MSG_BUFFER_LEN                      (500)                  /* �������󳤶� */
#define TAF_MMA_MAX_MSG_QUEUE_NUM                       (8)                     /* ��󻺴�ĸ��� */
#define TAF_MMA_MAX_STACK_DEPTH                         (4)                     /* ״̬��ջ����� */
#define TAF_MMA_ExtractMsgNameFromEvtType(ulEventType)     ((VOS_UINT16)((ulEventType) & 0x0000FFFF))          /* ��EventType�л�ȡMsgName     */
#define TAF_MMA_MAX_MSG_QUEUE_SIZE                      (11)                    /* �ڲ���Ϣ�������ĳ��� */

#define TAF_MMA_MAX_OPER_NUM                            (5)

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

enum TAF_MMA_MSG_CACHE_PRIO_ENUM
{
    TAF_MMA_MSG_CACHE_PRIO_HIGH              ,                           /* �����ȼ����� */
    TAF_MMA_MSG_CACHE_PRIO_MIDDLE            ,                           /* �����ȼ����� */
    TAF_MMA_MSG_CACHE_PRIO_LOW               ,                           /* �����ȼ����� */

    TAF_MMA_MSG_CACHE_PRIO_BUTT
};
typedef VOS_UINT32 TAF_MMA_MSG_CACHE_PRIO_ENUM_UINT8;
enum TAF_MMA_FSM_ID_ENUM
{
    TAF_MMA_FSM_MAIN                                            =0X00,

    TAF_MMA_FSM_PHONE_MODE                                      =0X01,


    TAF_MMA_FSM_BUTT

};
typedef VOS_UINT32 TAF_MMA_FSM_ID_ENUM_UINT32;


enum TAF_MMA_INIT_CTX_TYPE_ENUM
{
    TAF_MMA_INIT_CTX_STARTUP                    = 0,
    TAF_MMA_INIT_CTX_POWEROFF                   = 1,
    TAF_MMA_INIT_CTX_BUTT
};
typedef VOS_UINT32 TAF_MMA_INIT_CTX_TYPE_ENUM_UINT8;


enum TAF_MMA_OPER_TYPE_ENUM
{

    TAF_MMA_OPER_SYS_CFG_REQ                    = 0,
    TAF_MMA_OPER_ACQ_REQ                        = 1,
    TAF_MMA_OPER_REG_REQ                        = 2,
    TAF_MMA_OPER_POWER_SAVE_REQ                 = 3,
    TAF_MMA_OPER_DETACH_REQ                     = 4,
    TAF_MMA_OPER_BUTT

};
typedef VOS_UINT32 TAF_MMA_OPER_TYPE_ENUM_UINT32;

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

/*****************************************************************************
 �ṹ��    : TAF_MMA_ENTRY_MSG_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : ��Ҫ����������Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulEventType;
    VOS_UINT8                           aucEntryMsgBuffer[TAF_MMA_MAX_MSG_BUFFER_LEN];
} TAF_MMA_ENTRY_MSG_STRU;



typedef struct
{
    VOS_UINT16                          usPhoneError;       /* ���ػ��Ĵ���ԭ��ֵ */

    VOS_UINT8                           aucReserve[2];
}TAF_MMA_FSM_PHONE_MODE_CTX_STRU;
typedef union
{
    TAF_MMA_FSM_PHONE_MODE_CTX_STRU                         stPhoneModeCtx;

}TAF_MMA_FSM_EXTRA_CTX_UNION;


typedef struct
{
    /* ״̬������ָ��,ָ��ΪNULL��ʾ��ǰ״̬����Ч  */
    TAF_FSM_DESC_STRU                  *pstFsmDesc;

    /* ��ǰ״̬����ʶ */
    TAF_MMA_FSM_ID_ENUM_UINT32          enFsmId;

    /* ��ǰ״̬ */
    VOS_UINT32                          ulState;                                /* L1״̬ */

    /* �����ϢMSGID������  */
    TAF_MMA_ENTRY_MSG_STRU              stEntryMsg;

    /* ״̬������ */
    TAF_MMA_FSM_EXTRA_CTX_UNION         unFsmCtx;
}TAF_MMA_FSM_CTX_STRU;


typedef struct
{
    VOS_UINT16                          usStackDepth;                           /* ��ǰѹջ��״̬���� */
    VOS_UINT16                          usStackPopFlg;                          /* ��ǰջpop��־ */
    TAF_MMA_FSM_CTX_STRU                astFsmStack[TAF_MMA_MAX_STACK_DEPTH];   /* ״̬��ջ����� */
}TAF_MMA_FSM_STACK_STRU;
typedef struct
{
    TAF_MMA_ENTRY_MSG_STRU              stMsgEntry;     /* ����ľ�������   */
}TAF_MMA_CACH_MSG_INFO_STRU;
typedef struct
{
    VOS_UINT8                           ucCacheMsgNum;                          /* ������Ϣ�ĸ��� */
    VOS_UINT8                           aucReserve[3];
    TAF_MMA_CACH_MSG_INFO_STRU          astMsgQueue[TAF_MMA_MAX_MSG_QUEUE_NUM]; /* MMA����Ϣ�������飬�洢���Ǵ�ͨ����Ϣͷ����Ϣ */
}TAF_MMA_MSG_QUEUE_STRU;


typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT16                          usMsgID;                                /* ��ϢID */
    VOS_UINT8                           aucReserve[2];

    VOS_UINT8                           aucBuf[TAF_MMA_MAX_MSG_BUFFER_LEN];     /* ��Ϣ���� */
}TAF_MMA_INTERNAL_MSG_BUF_STRU;



typedef struct
{
    /* TAF MMA�ڲ���Ϣ���� */
    TAF_MMA_INTERNAL_MSG_BUF_STRU       astMmaMsgQueue[TAF_MMA_MAX_MSG_QUEUE_SIZE];

    /* ������Ϣ��ʱ����buff,����ֲ�����ʱֱ��ʹ�øÿռ�,Ϊ�˱��ⷢ���ڲ���Ϣʱ
    ռ�ýϴ��ջ�ռ� */
    TAF_MMA_INTERNAL_MSG_BUF_STRU       stSendMsgBuf;

    /* �ڲ���Ϣ������е�����βָ�� */
    VOS_UINT8                           ucTail;

    /* �ڲ���Ϣ������е�����ͷָ�� */
    VOS_UINT8                           ucHeader;

    VOS_UINT16                          usReserved;
} TAF_MMA_INTERNAL_MSG_QUEUE_STRU;


typedef struct
{
    VOS_UINT8                           aucRsv[4];
    VOS_UINT32                          ulAutoInitPsFlg;                        /* �ϵ��Ƿ��ʼ��Э��ջ */
    VOS_UINT32                          ulAutoSwitchOnFlg;                      /* �ϵ��Ƿ��Զ�����(�����ڲ����������MMC) */
}TAF_MMA_PHONE_MODE_CTRL_CTX_STRU;


typedef struct
{
    VOS_UINT8                           ucMmaLogInfoFlag;
    VOS_UINT8                           aucReserve[3];
}TAF_MMA_MAINTAIN_CTX_STRU;


typedef struct
{
    TAF_MMA_CTRL_STRU                   stCtrl;
    VOS_UINT8                           ucUsedFlag;
    VOS_UINT8                           aucReserved[3];
    TAF_MMA_OPER_TYPE_ENUM_UINT32       enOperType;
    VOS_VOID                           *pPara;
} TAF_MMA_OPER_CTX_STRU;





typedef struct
{
    /**************************************************************************
                        MMA״̬��״̬����״̬ջ
    **************************************************************************/
    TAF_MMA_FSM_CTX_STRU                stCurFsm;                               /* ״̬����ǰ��״̬������ */
    TAF_MMA_FSM_STACK_STRU              stFsmStack;                             /* ״̬����״̬��ջ����   */

    TAF_MMA_PHONE_MODE_CTRL_CTX_STRU    stPhoneModeCtrl;                        /* �ֻ�ģʽ���������� */

    TAF_MMA_TIMER_CTX_STRU              astMmaTimerCtx[TAF_MMA_MAX_TIMER_NUM];  /* MMA��ǰ�������еĶ�ʱ����Դ */

    TAF_MMA_OPER_CTX_STRU               astMmaOperCtx[TAF_MMA_MAX_OPER_NUM];  /* MMA L-C������������ */


    /**************************************************************************
                        TAF MMA������Ϣ����
    **************************************************************************/
    TAF_MMA_MSG_QUEUE_STRU              stBufferEntryMsgQueue;

    TAF_MMA_INTERNAL_MSG_QUEUE_STRU     stInternalMsgQueue;                     /* MMA�Ӳ���ڲ���Ϣ���� */

    TAF_MMA_MAINTAIN_CTX_STRU           stMaintainInfo;
}TAF_MMA_CONTEXT_STRU;

extern TAF_MMA_CONTEXT_STRU                    g_stTafMmaCtx;

/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/



/*****************************************************************************
  10 ��������
*****************************************************************************/
VOS_VOID TAF_MMA_SaveCurEntryMsg(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_VOID TAF_MMA_FSM_InitSubFsm(
    TAF_MMA_FSM_ID_ENUM_UINT32          enFsmId
);

VOS_VOID TAF_MMA_FSM_QuitSubFsm( VOS_VOID );

VOS_VOID  TAF_MMA_FSM_SetCurrState(
    VOS_UINT32                          ulCurrState
);

VOS_UINT32 TAF_MMA_FSM_GetFsmTopState( VOS_VOID );

VOS_UINT16  TAF_MMA_GetFsmStackDepth( VOS_VOID );

TAF_FSM_DESC_STRU* TAF_MMA_GetCurFsmDesc(VOS_VOID);
TAF_MMA_FSM_STACK_STRU* TAF_MMA_GetFsmStackAddr(VOS_VOID);

TAF_MMA_ENTRY_MSG_STRU* TAF_MMA_GetCurrFsmMsgAddr(VOS_VOID);

VOS_UINT32  TAF_MMA_GetNextCachedMsg(
    TAF_MMA_ENTRY_MSG_STRU             *pstEntryMsg
);

VOS_UINT32  TAF_MMA_GetNextPrioCachedMsg(
    TAF_MMA_ENTRY_MSG_STRU             *pstEntryMsg,
    TAF_MMA_MSG_CACHE_PRIO_ENUM_UINT8   enPrio
);


VOS_UINT32  TAF_MMA_ClearCacheMsg(
    VOS_UINT32                          ulEventType
);

VOS_VOID  TAF_MMA_InitFsmCtx_PhoneMode(VOS_VOID);


TAF_MMA_FSM_ID_ENUM_UINT32 TAF_MMA_GetCurrFsmId(VOS_VOID);

TAF_MMA_MSG_QUEUE_STRU* TAF_MMA_GetCachMsgBufferAddr(VOS_VOID);

VOS_VOID TAF_MMA_FSM_PushFsm(
    TAF_MMA_FSM_STACK_STRU              *pstFsmStack,
    TAF_MMA_FSM_CTX_STRU                *pstNewFsm
);

VOS_VOID TAF_MMA_FSM_PopFsm( VOS_VOID );

VOS_VOID  TAF_MMA_LoadSubFsm(
    TAF_MMA_FSM_ID_ENUM_UINT32          enFsmId,
    TAF_MMA_FSM_CTX_STRU               *pstCurFsm
);

VOS_VOID  TAF_MMA_SaveCacheMsgInMsgQueue(
    VOS_UINT32                          ulEventType,
    VOS_VOID                           *pstMsg
);

VOS_VOID  TAF_MMA_InitCurrFsmCtx(
    TAF_MMA_FSM_CTX_STRU                *pstCurrFsmCtx
);

VOS_VOID  TAF_MMA_InitFsmStackCtx(
    TAF_MMA_FSM_STACK_STRU              *pstFsmStack
);


VOS_VOID  TAF_MMA_InitInternalBuffer(
    TAF_MMA_MSG_QUEUE_STRU             *pstBufferEntryMsgQueue
);

VOS_VOID  TAF_MMA_InitInternalMsgQueue(
    TAF_MMA_INTERNAL_MSG_QUEUE_STRU     *pstInternalMsgQueue                   /* MMA���ڲ���Ϣ���� */
);
VOS_VOID  TAF_MMA_InitMaintainInfo(
    TAF_MMA_MAINTAIN_CTX_STRU          *pstMaintainInfo
);

TAF_MMA_PHONE_MODE_CTRL_CTX_STRU *TAF_MMA_GetPhoneModeCtrl(VOS_VOID);

VOS_UINT32  TAF_MMA_GetCacheNum( VOS_VOID );

VOS_VOID  TAF_MMA_SetFsmStackPopFlg(
    VOS_UINT16                          ucStachPopFlg
);

VOS_UINT16  TAF_MMA_GetFsmStackPopFlg( VOS_VOID );

TAF_MMA_FSM_CTX_STRU* TAF_MMA_GetCurFsmAddr(VOS_VOID);

TAF_MMA_CONTEXT_STRU* TAF_MMA_GetMmaCtxAddr(VOS_VOID);

VOS_VOID  TAF_MMA_SetAutoInitPsFlg(
    VOS_UINT32                          ulAutoInitPsFlg
);

VOS_UINT32  TAF_MMA_GetAutoInitPsFlg(VOS_VOID);

VOS_VOID  TAF_MMA_SetAutoSwitchOnFlg(
    VOS_UINT32                          ulAutoSwitchOnFlg
);

VOS_UINT32  TAF_MMA_GetAutoSwitchOnFlg(VOS_VOID);



TAF_MMA_TIMER_CTX_STRU*  TAF_MMA_GetTimerCtxAddr( VOS_VOID );

VOS_VOID  TAF_MMA_SetCurPhoneErrorCode_PhoneMode(
    VOS_UINT16                          usErrorCode
);

VOS_UINT16  TAF_MMA_GetCurPhoneErrorCode_PhoneMode(VOS_VOID);
VOS_UINT8  TAF_MMA_GetMmaLogInfoFlag( VOS_VOID );

VOS_VOID TAF_MMA_SetNetworkCapabilityInfo(
    TAF_SDC_SYS_MODE_ENUM_UINT8         enSysMode,
    TAF_SDC_NETWORK_CAP_INFO_STRU      *pstNwCapInfo
);

TAF_MMA_OPER_CTX_STRU*  TAF_MMA_GetOperCtxAddr( VOS_VOID );
VOS_UINT32 TAF_MMA_IsOperTypeUsed(
    TAF_MMA_OPER_TYPE_ENUM_UINT32       enOperType
);

VOS_UINT32 TAF_MMA_GetSpecOperTypeIndex(
    TAF_MMA_OPER_TYPE_ENUM_UINT32       enOperType,
    VOS_UINT8                          *pucCtxIndex
);

VOS_UINT32 TAF_MMA_GetUnusedOperCtxIndex(
    VOS_UINT8                          *pucCtxIndex
);

VOS_VOID TAF_MMA_SetOperCtx(
    TAF_MMA_CTRL_STRU                   stCtrl,
    TAF_MMA_OPER_TYPE_ENUM_UINT32       enOperType,
    VOS_UINT8                           ucCtxIndex
);

VOS_VOID TAF_MMA_ClearOperCtx(
    VOS_UINT8                           ucCtxIndex
);

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

#endif /* end of TafMmaCtx.h */

