

#ifndef _CC_MM_INTERFACE_H_
#define _CC_MM_INTERFACE_H_

#include "MmCmInterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif                                                                          /* __cpluscplus                             */
#endif                                                                          /* __cpluscplus                             */
#pragma pack(4)
/* CC����MM����Ϣ */
#define MMCC_EST_REQ                                        0
#define MMCC_REL_REQ                                        2
#define MMCC_ABORT_REQ                                      4
#define MMCC_DATA_REQ                                       6
#define MMCC_UNIT_DATA_REQ                                  8
#define MMCC_REEST_REQ                                      10
#define MMCC_PROMPT_REJ                                     12
#define MMCC_START_CC                                       14
#define MMCC_SRVCC_CALL_INFO_NOTIFY                         16

/* CC����MM��ԭ�� */
#define MMCC_EST_CNF                                        1
#define MMCC_EST_IND                                        3
#define MMCC_REL_IND                                        5
#define MMCC_DATA_IND                                       7
#define MMCC_UNIT_DATA_IND                                  9
#define MMCC_SYNC_IND                                       13
#define MMCC_REEST_CNF                                      11
#define MMCC_ERR_IND                                        15
#define MMCC_PROMPT_IND                                     17
#define MMCC_EMC_NUM_LST_IND                                19
#define MMCC_SRVCC_STATUS_IND                               21

/* ԭ��MMCC_EST_REQ�Ľṹ�� */
#define                     MMCC_MO_NORMAL_CALL             1                   /* Mobile originating call establishment    */
#define                     MMCC_EMERGENCY_CALL             2                   /* Emergency call establishment             */

#define                     MMCC_NO_CALL_PRIORITY           0
#define                     MMCC_CALL_PRIORITY_4            1
#define                     MMCC_CALL_PRIORITY_3            2
#define                     MMCC_CALL_PRIORITY_2            3
#define                     MMCC_CALL_PRIORITY_1            4
#define                     MMCC_CALL_PRIORITY_0            5
#define                     MMCC_CALL_PRIORITY_B            6
#define                     MMCC_CALL_PRIORITY_A            7

#define                     MMCC_CALL_MODE_SPEECH           (0)
#define                     MMCC_CALL_MODE_VIDEO            (1)
#define                     MMCC_CALL_MODE_OTHER            (2)

typedef struct
{
    MSG_HEADER_STRU                     MsgHeader;                              /* ��Ϣͷ*/
    VOS_UINT32                          ulTransactionId;                        /* TI */
    VOS_UINT32                          ulCallType;                             /* �������� */
    VOS_UINT32                          ulCallMode;                             /* ����ģʽ,speech��video�������� */
    VOS_UINT32                          ulCallPri ;                             /* �������ȼ� */
}MMCC_EST_REQ_STRU;

/* ԭ��MMCC_EST_CNF�Ľṹ�� */
#define                 MMCC_EST_ESTING                     0                   /* ��MM�������ڽ���                          */
#define                 MMCC_EST_SUCCESS                    1                   /* ��MM���ӽ����ɹ�                          */
typedef struct
{
    MSG_HEADER_STRU                     MsgHeader;                              /* ��Ϣͷ                                   */
    VOS_UINT32                          ulTransactionId;                        /* TI                                       */
    VOS_UINT32                          ulResult;                               /* ��MM���ӽ������                          */
}MMCC_EST_CNF_STRU;

typedef struct
{
    VOS_UINT32                   ulCcMsgSize;                                        /* [1,MMCC_MAX_SIZE_CC_MSG],Byte            */
    VOS_UINT8                   aucCcMsg[4];                                        /* ��Ϣ��ǰ�ĸ��ֽ�����                        */
}CC_MSG_STRU;

typedef struct
{
    MSG_HEADER_STRU         MsgHeader;                                          /* ��Ϣͷ                                   */
    VOS_UINT32                   ulTransactionId;                                    /* TI���˲������Բ���Ҫ                       */
    CC_MSG_STRU             FisrtCcMsg;                                         /* ��һ��CC��Ϣ                              */
}MMCC_EST_IND_STRU;

/* ԭ��MMCC_REL_REQ�Ľṹ�� */
#define     MMCC_RELEASE_ALL        0                                       /* �ͷ����е�MM����                           */
#define     MMCC_RELEASE_SPECIFIC   1                                       /* �ͷ�ָ����MM����                           */
typedef struct
{

    MSG_HEADER_STRU         MsgHeader;                                          /* ��Ϣͷ                                    */
    VOS_UINT32                   ulReleaseType;                                      /* �ͷ�����                                  */
    VOS_UINT32                   ulTransactionId;                                    /* TI                                       */
}MMCC_REL_REQ_STRU;

/* ԭ��MMCC_REL_IND�Ľṹ�� */
#define             MMCC_REL_CAUSE_EST_OR_REEST_FAIL    0
#define             MMCC_REL_CAUSE_REL_OR_ABSENT        1




typedef struct
{
    MSG_HEADER_STRU         MsgHeader;                                          /* ��Ϣͷ                                    */
    VOS_UINT32                   ulTransactionId;                                    /* TI                                       */
    VOS_UINT32                   ulRelCause;                                         /* �ͷ�ԭ��                                  */
}MMCC_REL_IND_STRU;

/* ԭ��MMCC_ABORT_REQ�Ľṹ�� */
typedef struct
{
    MSG_HEADER_STRU         MsgHeader;                                          /* ��Ϣͷ                                    */
    VOS_UINT32                   ulTransactionId;                                    /* TI                                       */
}MMCC_ABORT_REQ_STRU;

/* ԭ��MMCC_DATA_REQ�Ľṹ�� */
typedef struct
{
    MSG_HEADER_STRU         MsgHeader;                                          /* ��Ϣͷ                                   */
    VOS_UINT32                   ulTransactionId;                                    /* TI                                       */
    CC_MSG_STRU             SendCcMsg;                                          /* ��Ҫ���͵�CC��Ϣ                           */
}MMCC_DATA_REQ_STRU;


/* ԭ��MMCC_DATA_IND�Ľṹ�� */
typedef struct
{
    MSG_HEADER_STRU         MsgHeader;                                          /* ��Ϣͷ                                    */
    VOS_UINT32                   ulTransactionId;                                    /* TI                                       */
    CC_MSG_STRU             RcvCcMsg;                                           /* MM�յ���CC��Ϣ                            */
}MMCC_DATA_IND_STRU;

/* ԭ��MMCC_SYNC_IND�Ľṹ�� */
/*****************************************************************************
 ö����    : MMCC_CHANNEL_MODE_ENUM_U32
 ö��˵��  : MMCC�ӿڵĶ��壬��Ҫ��RRMM�ӿڶ���һ��
 1.��    ��   : 2012��2��9��
   ��    ��   : zhoujun 40661
   �޸�����   : modify
*****************************************************************************/
typedef enum
{
    MMCC_CHAN_MOD_SIG_ONLY,
    MMCC_CHAN_MOD_FR,
    MMCC_CHAN_MOD_HR,
    MMCC_CHAN_MOD_EFR,
    MMCC_CHAN_MOD_AHR,
    MMCC_CHAN_MOD_AMR,
    MMCC_CHAN_MOD_AMR2,
    MMCC_CHAN_MOD_AMRWB = 0x08,
    MMCC_CHAN_MOD_DATA_9600,
    MMCC_CHAN_MOD_DATA_4800,
    MMCC_CHAN_MOD_DATA_2400,
    MMCC_CHAN_MOD_DATA_14400,
    MMCC_CHAN_MOD_WFS,


    MMCC_CHAN_MOD_BUTT
} MMCC_SYNC_CHANNAL_MODE_ENUM;
typedef VOS_UINT32 MMCC_CHANNEL_MODE_ENUM_U32;

/*MMCC�ӿڵĶ��壬��Ҫ��RRMM�ӿڶ���һ��*/
typedef enum
{
    MM_CC_AMR_RATE_NULL               = 0x00,             /* ����ASN.1���룬���벻ʹ�� */
    MM_CC_AMR_RATE_4P75               = 0x01,             /* 4.75k */
    MM_CC_AMR_RATE_5P15               = 0x02,             /* 5.15k */
    MM_CC_AMR_RATE_5P9                = 0x04,             /* 5.90k */
    MM_CC_AMR_RATE_6P7                = 0x08,             /* 6.70k */
    MM_CC_AMR_RATE_7P4                = 0x10,             /* 7.40k */
    MM_CC_AMR_RATE_7P95               = 0x20,             /* 7.95k */
    MM_CC_AMR_RATE_10P2               = 0x40,             /* 10.2k */
    MM_CC_AMR_RATE_12P2               = 0x80,             /* 12.2k */
    MM_CC_AMR_RATE_BUTT
} MMCC_TCH_RATE_ENUM;
typedef VOS_UINT32 MMCC_TCH_RATE_ENUM_U32;


/*MMCC�ӿڵĶ��壬��Ҫ��RRMM�ӿڶ���һ��*/
typedef enum
{
    /* reasons for GSM */
    MMCC_GSM_RES_ASSIGNED,
    MMCC_GSM_CHANNEL_MODE_MODIFIED,
    MMCC_GSM_GSM_HANDOVER,
    MMCC_WCDMA_GSM_HANDOVER,

    /* reasons for WCDMA */
    MMCC_WCDMA_RAB_ESTABLISHED,
    MMCC_WCDMA_RAB_RECONFIGURED,
    MMCC_GSM_WCDMA_INTER_HANDOVER,
    MMCC_WCDMA_NO_CHANGE,
    MMCC_WCDMA_RAB_RELEASED
} MMCC_SYNC_REASON_ENUM;
typedef VOS_UINT32 MMCC_SYNC_REASON_ENUM_U32;

#define NAS_MM_CC_MAX_RAB_NUM           16                                      /* RAB�������� */

typedef struct
{
    MSG_HEADER_STRU                     MsgHeader;                              /* ��Ϣͷ */
    MMCC_CHANNEL_MODE_ENUM_U32          enChannelMode;                          /* �ŵ�ģʽ����GSM */

    VOS_UINT32                          ulRabNum;                               /* ��Ҫͬ����RAB���� */
    MMCC_SYNC_REASON_ENUM_U32           enSyncReason;

    struct
    {
        VOS_UINT32                      ulRabId;                                /* RAB ID */
        VOS_UINT32                      ulRabSyncInfo;                          /* ��RAB��ͬ����Ϣ */
    }syncInfo[NAS_MM_CC_MAX_RAB_NUM];

} MMCC_SYNC_IND_STRU;

/* ԭ��MMCC_REEST_REQ�Ľṹ�� */
typedef struct
{
    MSG_HEADER_STRU         MsgHeader;                                          /* ��Ϣͷ                                    */
    VOS_UINT32                   ulTransactionId;                                    /* TI                                       */
}MMCC_REEST_REQ_STRU;

/* ԭ��MMCC_REEST_CNF�Ľṹ�� */
#define     MMCC_REEST_ESTING       0                                       /* ��MM���������ؽ�                          */
#define     MMCC_REEST_SUCCESS      1                                       /* ��MM�����ؽ��ɹ�                          */
typedef struct
{
    MSG_HEADER_STRU         MsgHeader;                                          /* ��Ϣͷ                                    */
    VOS_UINT32                   ulTransactionId;                                    /* TI                                       */
    VOS_UINT32                   ulResult;                                           /* ��MM�����ؽ����                          */
}MMCC_REEST_CNF_STRU;

/* ԭ��MMCC_ERR_IND�Ľṹ�� */
typedef struct
{
    MSG_HEADER_STRU         MsgHeader;                                          /* ��Ϣͷ                                   */
    VOS_UINT32                   ulTransactionId;                                    /* TI                                       */
}MMCC_ERR_IND_STRU;

/* ԭ��MMCC_PROMPT_IND�Ľṹ�� */
typedef struct
{
    MSG_HEADER_STRU         MsgHeader;                                          /* ��Ϣͷ                                   */
}MMCC_PROMPT_IND_STRU;

/* ԭ��MMCC_PROMPT_REJ�Ľṹ�� */
#define     MMCC_CAUSE_SERV_OPT_TEMP_OUT_OF_ORDER      0                    /* Service option temporarily out of order  */
#define     MMCC_CAUSE_SERV_OPT_NOT_SUPP               1                    /* Service option not supported             */
typedef struct
{
    MSG_HEADER_STRU         MsgHeader;                                          /* ��Ϣͷ                                    */
    VOS_UINT32                   ulRefuseCause;                                      /* �ܾ�ԭ��                                  */
}MMCC_PROMPT_REJ_STRU;

/* ԭ��MMCC_EMC_NUM_LST_IND�Ľṹ�� */
#define MMCC_EMERGENCY_NUMBER_MAX_LENGTH              46                        /* Emergency Number information������ֽ��� */
#define MMCC_EMERGENCY_NUMBER_LISTS_MAX_NUMBER        16                        /* Emergency Number Lists��������         */
typedef struct
{
    VOS_UINT8                           ucCategoryValue;                        /* Emergency Service Category Value         */

    VOS_UINT8                           ucEmcNumLen;                            /*���������볤�ȣ���aucEmcNum����Ч����*/
    VOS_UINT8                           aucEmergencyList[MMCC_EMERGENCY_NUMBER_MAX_LENGTH];
}MMCC_EMERGENCY_CONTENT_STRU;

typedef struct
{
    MSG_HEADER_STRU                     MsgHeader;
    VOS_UINT32                          ulMcc;
    VOS_UINT8                           ucEmergencyNumber;
    VOS_UINT8                           aucReserved[3];
    MMCC_EMERGENCY_CONTENT_STRU         astEmergencyLists[MMCC_EMERGENCY_NUMBER_LISTS_MAX_NUMBER];
}MMCC_EMERGENCY_LIST_STRU;
enum NAS_MMCC_SRVCC_STATUS_ENUM
{
    NAS_MMCC_SRVCC_STATUS_START        = 0,     /* SRVCC��ʼ */                          
    NAS_MMCC_SRVCC_STATUS_SUCCESS      = 1,     /* SRVCC�ɹ� */                          
    NAS_MMCC_SRVCC_STATUS_FAIL         = 2,     /* SRVCCʧ�� */                          
    NAS_MMCC_SRVCC_STATUS_BUTT
};
typedef VOS_UINT32  NAS_MMCC_SRVCC_STATUS_ENUM_UINT32;
typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;          /* _H2ASN_Skip */     
    NAS_MMCC_SRVCC_STATUS_ENUM_UINT32   enSrvccStatus; 
    VOS_UINT8                           aucReserve1[4];    /*Ԥ��*/        
}MMCC_SRVCC_STATUS_IND_STRU;


#define   NAS_MM_MAX_CC_CONNECTION_NUM      7


typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;          /* _H2ASN_Skip */
    VOS_UINT8                           ucTiNum;
    VOS_UINT8                           aucTransactionId[NAS_MM_MAX_CC_CONNECTION_NUM];     
}MMCC_SRVCC_CALL_INFO_NOTIFY_STRU;


#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif                                                                          /* __cpluscplus                             */

#endif
