#ifndef  __STATUS_H__
#define  __STATUS_H__

/*===========================================================================*/

/*****************************************************************************/
/*  Copyright (C), 2004-2005, Huawei Tech. Co., Ltd.                         */
/*                                                                           */
/*  FileName: status.h                                                       */
/*                                                                           */
/*  Author:   Roger     Version :   1.00     Date:  2005.3.30                */
/*                                                                           */
/*  Description:      ģ������                                             */
/*    MAPS1000 STATUS ģ���ڲ��ӿ�ͷ�ļ�                                     */
/*                                                                           */
/*  Version:          �汾��Ϣ                                             */
/*    MAPS1000 V100R001                                                      */
/*                                                                           */
/*  Function List:    ��Ҫ�������书��                                     */
/*                                                                           */
/*  History:          ��ʷ�޸ļ�¼                                         */
/*      <author>  <time>   <version >   <desc>                               */
/*      Roger    05/03/30     1.0     build this file                        */
/*      liuyang  05/10/07     1.1     add attach & detach funcation                                                                    */
/*  jiangliping  06/08/09     1.1     A32D03479����PC����ʵ��ʱ��#pragma pack(0)��#pragma pack()�ӱ��뿪��                             */



/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/*=================================================================*/
/* ɾ��ExtAppMmcInterface.h*/

#include "MmaMmcInterface.h"
#include "TafMmaSndMmc.h"
#include "TafAppMma.h"
#include "TafSdcLib.h"



#include "NasStkInterface.h"



#pragma pack(4)
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*---------------  STATUS�ڲ�ʹ�ó������� ---------------*/
/*=======================================================*/
/*-----------------  ������־����  ------------*/
/* ���ʱ����*/
#define STA_TIMER_MAX                   2

/* ��Ч����״̬������ */
#define STA_FSMSUB_INDEX_NULL           2

/*---------- �ڲ�����������̵ı�ʶ  ----------*/
/* ATTACH����Ԥָ�����ڲ���ʶ */
#define STA_OPID_ATTATCH                255

/* RESTART����Ԥָ�����ڲ���ʶ */
#define STA_OPID_START                  255

/* �ڲ��������ʹ�õĹ�����ʶ */
#define STA_OPID_INSIDE                 255

/* TAFM�����߷���������̵���С��ʶ */
#define STA_OPID_MIN                    0

/* TAFM�����߷���������̵�����ʶ */
#define STA_OPID_MAX                    127

/*modified by liuyang id:48197 date:2005-10-5 for V100R001*/
/* TAFM�����߷���������̵���ЧOpId��ʾ */
#define STA_OPID_INVALID                200
/*modified by liuyang id:48197 date:2005-10-5 for V100R001*/

/*-------------  ��������ֵ����  --------------*/
#define STA_SUCCESS                     0
#define STA_ERROR                       1

#define STA_TRUE                        1
#define STA_FALSE                       0

/*-------- Sta_UpdateFsmFlag ���ͺ�ֵ ---------*/
#define STA_UPDATE_MONO                 0
#define STA_UPDATE_MONO_CLEAR           1
#define STA_UPDATE_DUAL                 2
#define STA_UPDATE_DUAL_CLEAR           3
#define STA_UPDATE_SINGL                4
#define STA_UPDATE_SINGL_CLEAR          5
#define STA_UPDATE_SETNEW               6

/*=======================================================*/


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*--------------- STATUS�ڲ�ʹ�ú��������---------------*/
/*=======================================================*/
/*----------       �ϲ�����32����    ----------*/
/* A��Bƴ��32λ���ͣ�Aռ��16λ��Bռ��16λ */
#ifndef MMA_MAKELPARAM
#define MMA_MAKELPARAM(A,B)     ((VOS_UINT32)((VOS_UINT32)((VOS_UINT32)(A)<<16) \
                                + (VOS_UINT32)(B)))
#endif
/*----------      ȡ�õ�16λ������   ----------*/
#ifndef MMA_LOWORD
#define MMA_LOWORD(A)           (VOS_UINT16)(A)
#endif
/*----------    ȡ�ø�16λ������     ----------*/
#ifndef MMA_HIWORD
#define MMA_HIWORD(A)           (VOS_UINT16)((VOS_UINT32)(A)>>16)
#endif
/*----------    ��ӡ���ٵ�����Ϣ     ----------*/
#define STA_TRACE( PrintLevel, Str )  MMA_LOG(PrintLevel, Str )
#define STA_TRACE1( PrintLevel, Str, Para) MMA_LOG1(PrintLevel, Str, Para)

/*=======================================================*/


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*------------- STATUS�ڲ�ȫ�����ݽṹ����  -------------*/
/*=======================================================*/

/*----------- CS��PS�����״̬��ֵ  -----------*/
#define STA_SRVC_NORMAL_SERVICE_D       0
#define STA_SRVC_LIMITED_SERVICE_D      1
#define STA_SRVC_ATTEMPTING_TO_UPDATE_D 2
#define STA_SRVC_NO_IMSI_D              3
#define STA_SRVC_NO_SERVICE_D           4
#define STA_SRVC_DETACH_D               5
#define STA_SRVC_DISABLE_D              6

#define STA_SAT_SERVICE_CHANGE          0
#define STA_SAT_LOCINFO_CHANGE          1

#define STA_SAT_LOCATION_NORMAL_SERVICE 0
#define STA_SAT_LOCATION_LIMIT_SERVICE  1
#define STA_SAT_LOCATION_NO_SERVICE     2

#define STA_SAT_LOCATION_INFORMATON_LEN 7

/*------------   �¼��������ͺ�    ------------*/
#define STA_OP_NULL                     0X000
#define STA_OP_REGPLMN                  0X001
#define STA_OP_CSSERVICESTATUS          0X002
#define STA_OP_PSSERVICESTATUS          0X004
#define STA_OP_PLMNLIST                 0X008
#define STA_OP_PLMNRESELMODE            0X010
#define STA_OP_PHONEERROR               0X020
#define STA_OP_PHONESERVICESTATUS       0X040    /* AT^SRVST */
#define STA_OP_SPARE                    0X100

/*----------- �ڲ�FSM��״̬��ʶ��  ------------*/
#define STA_FSM_INVALID                 0X0000
#define STA_FSM_NULL                    0X0001
#define STA_FSM_ENABLE                  0X0002
#define STA_FSM_RESTART                 0X0004
#define STA_FSM_STOP                    0X0008
#define STA_FSM_DE_ATTACH               0X0010
#define STA_FSM_PLMN_LIST               0X0040
#define STA_FSM_PLMN_RESEL              0X0080
#define STA_FSM_PLMN_SEL                0X0100
#define STA_FSM_SYS_CFG_SET             0X0200  /* ϵͳ����״̬ */

/* �����״̬�����Ķ�ʱ������ */
#define STA_FSM_TIMER_CLEAR             0X1001
/* ���ı䵱ǰ״̬��־ */
#define STA_FSM_NO_UPDATE               0X1002

/*------------ �ڲ�FSM��״̬��ʶ�� ------------*/
#define STA_FSMSUB_NULL                 0X00    /* ����״̬*/
#define STA_FSMSUB_OK                   0X10    /* ʹ��״̬*/
#define STA_FSMSUB_MONO                 0X20    /* ֻ�е�һ����״̬*/
#define STA_FSMSUB_ATTACH_CS            0X01    /* ATTACH ��״̬,CS�� */
#define STA_FSMSUB_ATTACH_PS            0X02    /* ATTACH ��״̬,PS�� */
#define STA_FSMSUB_DETACH_CS            0X03    /* DETACH��״̬,CS�� */
#define STA_FSMSUB_DETACH_PS            0X04    /* DETACH��״̬,PS�� */
#define STA_FSMSUB_PLMN_RESEL_AUTO      0X05    /* PLMN_RESEL��״̬,�Զ� */
#define STA_FSMSUB_PLMN_RESEL_MANUAL    0X06    /* PLMN_RESEL��״̬,�ֶ� */
#define STA_FSMSUB_ATTACH_CSPS          0X07    /* ATTACH��״̬��CS��PS�� */
#define STA_FSMSUB_DETACH_CSPS          0X08    /* DETACH��״̬��CS��PS�� */

#define STA_TIMER_NULL_LEN                  (100)     /* ��λms */
#define STA_TIMER_RESTART_LEN               (70000)   /* ��λms */
#define STA_TIMER_STOP_LEN                  (60000)   /* ��λms */
#define STA_TIMER_ATTACH_CS_LEN             (150000)  /* ��λms */
#define STA_TIMER_DETACH_CS_LEN             (100000)  /* ��λms */
#define STA_TIMER_ATTACH_PS_LEN             (150000)  /* ��λms */
#define STA_TIMER_DETACH_PS_LEN             (100000)  /* ��λms */
#define STA_TIMER_GUL_PLMN_LIST_LEN         (238000)  /* ��λms */
#define STA_TIMER_GU_PLMN_LIST_LEN          (118000)  /* ��λms */
#define STA_TIMER_PLMN_RESEL_LEN            (320000)  /* ��λms */
#define STA_TIMER_PLMN_SEL_LEN              (110000)  /* ��λms */
#define STA_TIMER_SYSCFG_SET_LEN            (20000)   /* ��λms */
#define STA_TIMER_SERVICE_IND_LEN           (10000)   /* ��λms */
#define STA_TIMER_SPEC_PLMN_ABORT_LEN       (5000)    /* ��λms */
#define STA_TIMER_GUL_PLMN_LIST_ABORT_LEN   (10000)   /* ��λms */
#define STA_TIMER_GU_PLMN_LIST_ABORT_LEN    (5000)    /* ��λms */

/*------------ FSM״̬����ʱ������ ------------*/
typedef enum
{
    STA_TIMER_NULL                      = 0,

    STA_TIMER_ATTACH_CS,
    STA_TIMER_DETACH_CS,
    STA_TIMER_ATTACH_PS,
    STA_TIMER_DETACH_PS,
    STA_TIMER_PLMN_LIST,
    STA_TIMER_PLMN_RESEL,
    STA_TIMER_PLMN_SEL,
    STA_TIMER_SYSCFG_SET,
    STA_TIMER_SERVICE_IND,/* ��ָ��ѡ��ʱ���ȴ�MMC�ķ���״̬�ϱ�������ʱ�� */
    /* ָ������STA_TIMER_PLMN_SEL��ʱ����ʱ��
       MMA����D_MMC_SPEC_PLMN_SEARCH_ABORT_REQ������������ʱ��10s �ղ���MMC��
       D_MMC_SPEC_PLMN_SEARCH_ABORT_CNF,ֱ�Ӹ�AT�ظ�ʧ�� */
    STA_TIMER_SPEC_PLMN_ABORT,

    /* list����STA_TIMER_PLMN_LIST��ʱ����ʱ��
       MMA����D_MMC_PLMN_LIST_ABORT_REQ������������ʱ��10s �ղ���MMC��
       D_MMC_PLMN_LIST_ABORT_CNF,ֱ�Ӹ�AT�ظ�ʧ�� */
    STA_TIMER_PLMN_LIST_ABORT,
    STA_TIMER_BUTT
} STA_TIMER_NAME_E;

/*--------- FSM״̬��״̬�����ṹ���� ---------*/
typedef struct
{
    VOS_UINT32  ulOpId;                     /* ��ǰ״̬����������̵ı�ʶ��*/
    VOS_UINT8   ucFsmStateSub;              /* ״̬����״ֵ̬*/

    /*��¼��ǰ STATUS��ʱ��״̬*/
    STA_TIMER_NAME_E  TimerName;        /* FSM״̬������ʱ������*/
} STA_FSM_SUB_S;

typedef enum
{
    STA_DATA_TRANSFER_EDGE,
    STA_DATA_TRANSFER_R99_ONLY,
    STA_DATA_TRANSFER_HSDPA,
    STA_DATA_TRANSFER_HSUPA,
    STA_DATA_TRANSFER_HSDPA_AND_HSUPA,
    STA_DATA_TRANSFER_HSPA_PLUS,
    STA_DATA_TRANSFER_LTE_DATA,
    STA_DATA_TRANSFER_LTE_IDLE,
    STA_DATA_TRANSFER_DC_HSPA_PLUS,
    STA_DATA_TRANSFER_DC_MIMO,
    STA_DATA_TRANSFER_MODE_BUTT
} STA_DATA_TRANSFER_MODE;

typedef VOS_UINT8 STA_DATA_TRANSFER_MODE_UINT8;

typedef enum
{
    STA_DATA_TRANSFER_PDP_DEACT,
    STA_DATA_TRANSFER_PDP_ACT,
    STA_DATA_TRANSFER_STATUS_BUTT
} STA_DATA_TRANSFER_STATUS;

typedef VOS_UINT8 STA_DATA_TRANSFER_STATUS_UINT8;


typedef VOS_UINT8 STA_USER_SPECIFY_PLMN_PROC_CTRL_ENUM_UINT8;
enum TAF_MMA_PLMN_MODE_ENUM
{
    TAF_MMA_PLMN_MODE_ENUM_DISABLE,
    TAF_MMA_PLMN_MODE_ENUM_ENABLE,
    TAF_MMA_PLMN_MODE_ENUM_BUTT
};
typedef VOS_UINT8 TAF_MMA_PLMN_MODE_ENUM_UINT8;


enum TAF_MMA_AUTO_PLMN_SEL_USER_ENUM
{
    TAF_MMA_AUTO_PLMN_SEL_USER_AT,
    TAF_MMA_AUTO_PLMN_SEL_USER_USIM_REFRESH,
    TAF_MMA_AUTO_PLMN_SEL_USER_BUTT
};
typedef VOS_UINT8 TAF_MMA_AUTO_PLMN_SEL_USER_ENUM_UINT8;


enum TAF_MMA_SYSCFG_USER_ENUM
{
    TAF_MMA_SYSCFG_USER_AT,
    TAF_MMA_SYSCFG_USER_USIM_REFRESH,
    TAF_MMA_SYSCFG_USER_BUTT
};
typedef VOS_UINT8 TAF_MMA_SYSCFG_USER_ENUM_UINT8;


enum TAF_MMA_RAT_MODE_ENUM
{
    TAF_MMA_RAT_MODE_ENUM_DAUL_MODE,
    TAF_MMA_RAT_MODE_ENUM_2G_ONLY,
    TAF_MMA_RAT_MODE_ENUM_3G_ONLY,
    TAF_MMA_RAT_MODE_ENUM_TRI_MODE,
    TAF_MMA_RAT_MODE_ENUM_LTE_ONLY,
    TAF_MMA_RAT_MODE_ENUM_BUTT          = 0xFF
};
typedef VOS_UINT8 TAF_MMA_RAT_MODE_ENUM_U8;



enum TAF_MMA_CS_DOMAIN_CAPA_CHANGE_TYPE_ENUM
{
    TAF_MMA_CS_DOMAIN_CAPA_NO_CHANGE                        = 0,                /* CS�������ޱ仯 */
    TAF_MMA_CS_DOMAIN_CAPA_UNAVAIL_TO_AVAIL                 = 1,                /* CS���ɲ����õ����� */
    TAF_MMA_CS_DOMAIN_CAPA_AVAIL_TO_UNAVAIL                 = 2,                /* CS���ɿ��õ������� */
    TAF_MMA_CS_DOMAIN_CAPA_CHANGE_TYPE_BUTT
};
typedef VOS_UINT8 TAF_MMA_CS_DOMAIN_CAPA_CHANGE_TYPE_ENUM_UINT8;



typedef struct MMA_TIMER
{
    HTIMER       MmaHTimer;
    VOS_UINT32   ulTimerId;
    VOS_UINT8    ucTimerStatus;
}MMA_TIMER_ST;

#define     MMA_TIMER_STOP  0
#define     MMA_TIMER_RUN   1
#define     MMA_TIMER_ERROR 2
typedef struct
{
    VOS_UINT8                                   ucNetSelMenuFlg;                /* ����ѡ��˵������ʶ��VOS_TRUE��ʾ���VOS_FALSE��ʾδ���� */
    TAF_MMA_PLMN_MODE_ENUM_UINT8                enPlmnMode;                     /* ��¼6F15�ļ���PLMN MODEλ */
    TAF_MMA_AUTO_PLMN_SEL_USER_ENUM_UINT8       enAutoPlmnSelUser;              /* ��¼����AT������Զ���������refresh�������Զ�������Ĭ��ΪAT */
    VOS_UINT8                                   ucRsv;                          /* ���� */
    MMA_TIMER_ST                                stTiPeriodTryingNetSelMenu;     /* ����ѡ��˵����������Գ����Զ�ѡ����ʱ�� */
}TAF_MMA_NETWORK_SELECTION_MENU_CTX_STRU;
typedef struct
{
    VOS_UINT8                                   ucRatBalancingFlg;              /* ���뼼��ƽ�⼤���ʶ��VOS_TRUE��ʾ���VOS_FALSE��ʾδ���� */
    TAF_MMA_RAT_MODE_ENUM_U8                    enRatMode;                      /* ��¼4F36�ļ���RAT MODE */
    TAF_MMA_SYSCFG_USER_ENUM_UINT8              enSyscfgUser;                   /* ��¼����AT�����SYSCFG����refresh������SYSCFG��Ĭ��ΪAT  */
    VOS_UINT8                                   ucRsv;                          /* ���� */
    MMA_TIMER_ST                                stTiPeriodTryingRatBalancing;   /* ���뼼��ƽ����������Գ����Զ�ѡ����ʱ�� */
}TAF_MMA_RAT_BALANCING_CTX_STRU;

/*--------- STATUS_CONTEXT_STRU�ṹ���� -------*/
typedef struct
{
    VOS_UINT32          ulFsmState;                     /* STATUS�ڲ�״̬����־λ */

    VOS_UINT32          ulPreFsmState;

    /* ״̬����״̬������ */
    STA_FSM_SUB_S       aFsmSub[STA_TIMER_MAX];         /* ״̬����ǰ״̬����Ϣ��¼*/

    VOS_UINT8           aucFsmSubFlag[STA_TIMER_MAX];   /* ��״̬��ʶ*/
    HTIMER              ahStaTimer[STA_TIMER_MAX];      /* ��ʱ������*/

    /* STATUSϵͳ״̬��¼*/
    VOS_UINT32          ulCurOpId;                      /* ��ǰЭ��ջ��Ӧ�Ĳ�����ʶ*/
    VOS_UINT32          ulCurClientId;                  /* ��ǰ�ͻ��˱�ʶ*/
    VOS_UINT32          ulNewOpId;                      /* ������Ĳ�����ʶ*/
    VOS_INT8            ucAttachReq;                    /* ATTACH�������*/
    VOS_INT8            ucDetachReq;                    /* DETACH�������*/

    VOS_UINT8           ucIsReportedLocStatus;          /* ��¼�������Ƿ񱨹�STK Location Status Event */
    VOS_UINT8           ucReserve1;

    /* STATUSҵ����Ϣ��¼*/
    TAF_PLMN_ID_STRU    StaPlmnId;                      /* ָ����PLMN*/
    TAF_PLMN_LIST_STRU  PlmnList;                       /* PLMN LIST���*/
    VOS_UINT8           ucPlmnListAbortProc;           /* Plmn List Abort������� */
    VOS_UINT8           aucReserv[3];

    VOS_UINT32          ulMmProc;                       /* MMC�������*/
    VOS_UINT32          ulCsCause;                      /* CS��ԭ��ֵ */
    VOS_UINT32          ulPsCause;                      /* PS��ԭ��ֵ */
    VOS_UINT32          ulTimMaxFlg;                    /* Availabe Plmn Search Timer�Ƿ�ﵽ���ֵ */
    VOS_UINT8           ucReselMode;                    /* PLMN ��ѡ��ʽ */
    VOS_UINT8           ucOperateType;                  /* �¼��������� */
    VOS_UINT8           ucDomainFlag;                   /* ����Ч�ı�־λ*/
    VOS_UINT8           ucModeService;                  /* ע�������ģʽ���� */

    TAF_PH_RAT_ORDER_STRU               stRatPrioList;   /* ����ģʽ���ȼ�*/

    TAF_PHONE_ERROR     PhoneError;                     /* �����¼�����*/

    VOS_UINT8                                   aucStaPhoneOperator[STA_TIMER_MAX];
    STA_DATA_TRANSFER_STATUS_UINT8              ucDataTranStatus;                           /* ��¼С������״̬*/
    STA_DATA_TRANSFER_MODE_UINT8                ucDataTranMode;                             /* ��¼С������״̬�µ�ϵͳ��ģʽ */
    VOS_UINT8                                   ucPlmnSelMode;                  /* ��¼����ģʽ:�ֶ����Զ� */
    VOS_UINT8                                   ucPreUtranMode;                 /* ��¼��ǰ��UtranMode��FDD��TDD */
    TAF_MMA_NETWORK_SELECTION_MENU_CTX_STRU     stNetSelMenuCtx;                /* ����ѡ��˵����������� */
    TAF_MMA_RAT_BALANCING_CTX_STRU              stRatBalancingCtx;              /* ���뼼��ƽ����������� */
} STATUS_CONTEXT_STRU;

/*=======================================================*/


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*------------ Sta_UpdateData�����ӿڲ����ṹ------------*/
/*=======================================================*/
/*----------   ��״̬���²�������    ----------*/
#define     STA_FSMSUB_NEW      0X01    /* �����µ���״̬*/
#define     STA_FSMSUB_UPDATE   0X02    /* ���µ�ǰ��״̬����*/
#define     STA_FSMSUB_FINISH   0X03    /* ������״̬ */

/*---------- STA_PHONE_DATA_S�ṹ����----------*/
typedef struct
{
    VOS_UINT32  ulOpId;                     /* ������ʶ��*/
    VOS_UINT32  ulFsmState;                 /* STATUS�ڲ�״̬����־λ */
    VOS_UINT8   ucFsmStateSub;              /* ״̬����״ֵ̬*/
    VOS_UINT8   ucFsmSubOperate;            /* ���²������� */
    VOS_UINT8   ucFsmUpdateFlag;            /* ��־���µ����� */
    /* STATUSҵ����Ϣ��¼*/
    TAF_PLMN_ID_STRU   PlmnId;          /* ָ����PLMN*/
    VOS_UINT32  ulMmProc;                   /* ��ӦMM�������*/
    VOS_UINT32  ulCsServiceStatus;          /* CS�����״̬*/
    VOS_UINT32  ulPsServiceStatus;          /* PS�����״̬*/
    VOS_UINT32  ulCsCause;                  /* CS��ԭ��ֵ */
    VOS_UINT32  ulPsCause;                  /* PS��ԭ��ֵ */
    VOS_UINT8   ucDomainFlag;               /* ����Ч�ı�־λ*/
} STA_PHONE_DATA_S;

/*=======================================================*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*----------  Э��ջ��Ӧ��Ϣ��������������  -----------*/
/*=======================================================*/
/*------------   ATTACH��������    ------------*/
#define STA_ATTACH_TYPE_GPRS            1
#define STA_ATTACH_TYPE_IMSI            2
#define STA_ATTACH_TYPE_GPRS_IMSI       3
#define STA_ATTACH_TYPE_NULL               0

/*-------------- DETACH��������  --------------*/
#define STA_PS_DETACH                   1
#define STA_CS_DETACH                   2
#define STA_PS_CS_DETACH                3
#define STA_DETACH_NULL                 0
/*=======================================================*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*---------- Э��ջ��Ӧ��Ϣ������(�ڲ�ʹ��) -----------*/
/*=======================================================*/
/*-------------  START������Ӧ����  -----------*/
VOS_UINT32 Sta_StartCnf ( MMC_MMA_START_CNF_STRU *pStartCnf );

/*-------------  ATTACH������Ӧ���� -----------*/
VOS_UINT32 Sta_AttachCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 Sta_DetachCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

/*------------  ����״̬�ϱ���Ӧ���� ----------*/
VOS_UINT32 Sta_ServiceStatusInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

/*-------------  DETACH������Ӧ���� -----------*/
VOS_UINT32 Sta_DetachCnf_SysCfgSet(MMC_MMA_DETACH_CNF_STRU *pDetachCnf );

/*-------------  DETACH�ϱ���Ӧ���� -----------*/
VOS_UINT32 Sta_DetachInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

/*------------  PLMN����������Ӧ���� ----------*/
VOS_UINT32 Sta_PlmnListInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

/*-----------  CoverageAreaInd��Ӧ���� --------*/
VOS_UINT32 Sta_CoverageAreaInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

/*----------- PLMN�����ܾ�������Ӧ���� --------*/
VOS_UINT32 Sta_PlmnListRej(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 Sta_DataTranAttri(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 Sta_SysCfgCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_VOID MN_PH_UpdateBeginRegTime( VOS_VOID  );



/* END:   Added by liuyang, 2005/10/21 */
/* BEGIN: Added by liuyang, 2005/10/25 */
VOS_UINT32   Sta_SyncMsClassType(VOS_UINT32 opId,
                                 TAF_PH_MS_CLASS_TYPE ucMsClass);
/* END:   Added by liuyang, 2005/10/25 */

VOS_VOID Sta_UpdateDataTranStatusAndMode(MMA_MMC_DATATRAN_ATTRI_ENUM_UINT8 enDataTranAttri);

/* VOS_VOID Sta_DataTranAttri(MMC_MMA_DATATRAN_ATTRI_IND_STRU *pstDataTranAttri); */
VOS_VOID Sta_ModeChangeEventReport(VOS_UINT8 ucNewSysMode, VOS_UINT8 ucNewSysSubMode);


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*---------- ��Э��ջ����������Ϣ������(�ڲ�)----------*/
/*=======================================================*/
/*------------- Э��ջ������Ϣ�ӿ�-------------*/
/*------------- Э��ջ������ֹ�ӿ�-------------*/
#define STA_MSGABORT_OK                 0    /* ���������¼� */
#define STA_MSGABORT_ERR                1    /* ���ش����¼� */
VOS_UINT32 Sta_MsgReqAbort ( VOS_UINT32 ulAbortType );

/*=======================================================*/


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*--------------- STATUS�ڲ����ݴ�����  ---------------*/
/*=======================================================*/

VOS_UINT32 Sta_ReOrderPlmnList (TAF_PLMN_LIST_STRU* pPlmnList);
VOS_UINT32 Sta_UpdateData ( STA_PHONE_DATA_S *pData );

VOS_VOID   Sta_EventReport ( VOS_UINT32 ulOpId, VOS_UINT32 ulEventType );

VOS_UINT32 Sta_UpdateFsmFlag ( VOS_UINT8 ucFsmSub,
                    VOS_UINT32 ulFsm, VOS_UINT32 ulType );

VOS_UINT8 Sta_FindFsmSubBySubSta ( VOS_UINT8 ucFsmSubSta );
VOS_UINT32 Sta_IsDualFsmSub ( VOS_VOID );

VOS_UINT32 Sta_FindFsmSub ( VOS_UINT8 ucFsmSub );

VOS_UINT32 Sta_FindIdelFsmSub ( VOS_UINT8 ucSubState );

VOS_BOOL Sta_IsFsmSubValid ( VOS_UINT8 ucFsmSub, VOS_UINT32 ulFsm );

VOS_BOOL Sta_IsInvalidOpId ( VOS_UINT32 ulOpId );

VOS_VOID Sta_ClearCurPlmnId(TAF_PLMN_ID_STRU *pstNewPlmnId);
VOS_VOID Sta_InitCurPlmnId(TAF_PLMN_ID_STRU    stSrcPlmnId,
                             TAF_PLMN_ID_STRU   *pstDstPlmnId);
VOS_VOID Sta_DetachTimeoutProc4DefMsClassType( STA_TIMER_NAME_E    TimerId,
                                                VOS_UINT16          TimerIndex);
VOS_VOID Sta_DetachTimeoutProc4App(STA_TIMER_NAME_E    TimerId,
                                   VOS_UINT16          TimerIndex,
                                   VOS_UINT8           ucOperate);

VOS_UINT32 Sta_ProcSetMsClassTypeforDetach(MMC_MMA_DETACH_CNF_STRU *pDetachCnf );

VOS_UINT32 Sta_SuccessDetach(MMC_MMA_DETACH_CNF_STRU *pDetachCnf );
VOS_UINT32 Sta_CompleteDefMsClassType(MMC_MMA_DETACH_CNF_STRU *pDetachCnf );


VOS_UINT32 Sta_ProcMsClassTypeNewToOld(TAF_PH_MS_CLASS_TYPE   CurMsClassType,
                                       TAF_PH_MS_CLASS_TYPE   NewMsClassType,
                                       VOS_UINT32            *pulAttachType,
                                       VOS_UINT32            *pulDetachType);


/* END:   Added by liuyang, 2005/10/25 */
/* BEGIN: Added by liuyang id:48197, 2006/1/19 for V200R001*/
extern VOS_UINT32 Sta_StopAllRunningTimer(VOS_VOID);
/* extern VOS_VOID Sta_ResetFsmFlg(VOS_VOID); */
/* END:   Added by liuyang id:48197, 2006/1/19 */
/* BEGIN: Added by liuyang id:48197, 2006/3/14   PN:A32D02500*/
VOS_UINT32 Sta_AttachByModeService(VOS_UINT32  ulOpId);


VOS_VOID Sta_GetLocInfoForSat(MMA_MMC_PLMN_ID_STRU stPlmnId, VOS_UINT16 usLac, VOS_UINT16 usCellId,
                              VOS_UINT8 *pucLocInfo);

VOS_UINT8 Sta_GetStatusForSat(VOS_UINT32 ulServiceStatus);
/* VOS_UINT32 Sta_SysCfgCnf( MMC_MMA_SYS_CFG_SET_CNF_STRU  *pstSysCfgCnf); */
/* ����״̬�ϱ���ͳһ�ŵ�Sta_ServiceStatusInd()����,attach cnf �� detach cnf���������� */

VOS_VOID Mma_ComGetServiceStatusForReport(VOS_UINT32 ulCsServiceStatus,
                                       VOS_UINT32 ulPsServiceStatus,
                                       VOS_UINT32 ulCsCause,
                                       VOS_UINT32 ulPsCause);
VOS_VOID Sta_UpdateServiceStatus(VOS_UINT32 ulServiceStatus,
                             VOS_UINT32 ulCnDomain);

TAF_SDC_REPORT_SRVSTA_ENUM_UINT8 TAF_MMA_GetSrvTypeForStk(VOS_VOID);

VOS_VOID TAF_MMA_ProcLociStatusEvent(VOS_VOID);


TAF_SDC_SERVICE_STATUS_ENUM_UINT8 TAF_MMA_ConvertSrvTypeForStk(
                                    TAF_SDC_SERVICE_STATUS_ENUM_UINT8  enTafSrvType
                                    );


VOS_VOID TAF_MMA_ProcCsServiceStatusChanged(VOS_UINT8 ucOldCsServiceStatus);

VOS_UINT8 TAF_MMA_IsCsServiceStatusChanged(VOS_UINT8 ucOldCsServiceStatus);

VOS_UINT8 TAF_MMA_IsPsServiceStatusChanged(VOS_UINT8 ucOldPsServiceStatus);

TAF_SDC_REPORT_SRVSTA_ENUM_UINT8 TAF_MMA_ConvertTafSrvStaToStkType(
                            TAF_SDC_REPORT_SRVSTA_ENUM_UINT8      enTafSrvType
                            );


VOS_VOID Mma_ModeChgReport(
    VOS_UINT8                           enCurNetWork,
    VOS_UINT8                           ucSysSubMode
);




TAF_PH_RAT_TYPE_ENUM_UINT8 TAF_MMA_ConvertMmaRatToStkRat(
                                         TAF_SDC_SYS_MODE_ENUM_UINT8 enMmaRatType
                                         );

TAF_PH_PLMN_SELECTION_RESULT_ENUM_UINT32 TAF_MMA_ConvertServiceTypeToAtType(
    MMA_MMC_PLMN_SELECTION_RESULT_ENUM_UINT32 enMmaServiceType
);

VOS_UINT32 TAF_MMA_ProcPlmnSelectStartInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

extern VOS_UINT32 Sta_MmInfo(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);


extern VOS_UINT32 MN_MMA_ProcPlmnSelectionRlstInd(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);





extern  VOS_UINT8 Sta_GetSysSubMode_W(STA_DATA_TRANSFER_STATUS_UINT8      ucDataTranStatus ,
                                       STA_DATA_TRANSFER_MODE_UINT8        ucDataTranMode
                                       );

/* END:   Added by liuyang id:48197, 2006/3/14 */

VOS_UINT8 Sta_GetSysSubMode_Td(
    STA_DATA_TRANSFER_STATUS_UINT8      ucDataTranStatus,
    STA_DATA_TRANSFER_MODE_UINT8        ucDataTranMode
);

extern VOS_VOID NAS_MMC_GetCsPsSimStatus(
    VOS_BOOL                           *pbCsSimStatus,
    VOS_BOOL                           *pbPsSimStatus);

extern VOS_VOID NAS_MN_ReportPlmnSrchResult(
    VOS_UINT32                  ulResult
);

extern VOS_UINT32  MN_PH_GetRegCostTime( VOS_VOID );

TAF_PH_MODE MMA_GetCurrPhMode(VOS_VOID);

VOS_VOID TAF_MMA_ReportPlmnModeEvent(VOS_UINT8 ucPlmnMode);
VOS_UINT32 TAF_MMA_IsEnablePlmnList(VOS_VOID);
VOS_UINT32 TAF_MMA_IsEnableDeAttach(VOS_VOID);
VOS_UINT32 TAF_MMA_SndAutoPlmnReselect_Refresh(VOS_VOID);

/* VOS_UINT32 TAF_MMA_RcvMmcPlmnReselCnf(MMC_MMA_PLMN_RESEL_CNF_STRU *pstMsg); */
VOS_UINT32 TAF_MMA_RcvMmcPlmnReselCnf_Refresh(MMC_MMA_PLMN_RESEL_CNF_STRU *pstMsg);

VOS_UINT32 TAF_MMA_RcvMmcPlmnSpecialSelCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvMmcPlmnReselCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);


VOS_UINT32 TAF_MMA_ProcPhoneStop_NotEnableStatus(VOS_VOID);
VOS_UINT32 TAF_MMA_IsEnablePlmnSelect(VOS_VOID);

VOS_VOID TAF_MMA_DeleteRatType(
    TAF_PH_RAT_TYPE_ENUM_UINT8           ucRatType,
    MMA_MMC_PLMN_RAT_PRIO_STRU          *pstPlmnRatPrioList
);

VOS_VOID TAF_MMA_GetAvailablePlmnRatPrio(MMA_MMC_PLMN_RAT_PRIO_STRU *pstPlmnRatPrioList);

VOS_VOID TAF_MMA_RcvSyscfgCnfFail(VOS_VOID);


VOS_UINT8 TAF_MMA_IsModeChange(
    TAF_SDC_SYS_MODE_ENUM_UINT8         enCurNetWork,
    VOS_UINT8                           ucSysSubMode
);




VOS_UINT32 TAF_MMA_PlmnListAbortCnfUserAbort (MMC_MMA_PLMN_LIST_ABORT_CNF_STRU *pPlmnListAbortCnf);
VOS_UINT32 TAF_MMA_RcvPlmnListAbortCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvSpecPlmnSearchAbortCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvMmcUserPlmnRej(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_VOID  TAF_MMA_RcvTimerPlmnListCnfExpired(VOS_UINT32 ulOpId);
VOS_VOID  TAF_MMA_RcvUserAbortPlmnList( VOS_UINT32 ulOpId );
VOS_VOID  TAF_MMA_RcvTimerPlmnListAbortCnfExpired(VOS_UINT16 usTimerIndex);


VOS_UINT32 TAF_MMA_IsGsmAccessForbidden(VOS_VOID);

VOS_UINT32 TAF_MMA_IsExistPlmnRatAllowAccess(VOS_VOID);

VOS_VOID TAF_MMA_ReportRegStatus(
    MMA_MMC_REG_STATE_ENUM_UINT8        enRegState,
    MMA_MMC_SRVDOMAIN_ENUM_UINT32        enCnDomainId
);

VOS_UINT32 TAF_MMA_IsCsServDomainAvail(
    TAF_PH_MS_CLASS_TYPE                enMsClassType
);
TAF_CS_SERVICE_ENUM_UINT32  TAF_MMA_GetCsStateFromSyscfgSetFlag(
    VOS_UINT16                          usSyscfgSetFlag
);
VOS_VOID  TAF_MMA_ClearSyscfgCsServiceChangeFlag(
    VOS_UINT16                          usSyscfgSetFlag
);

VOS_UINT32 TAF_MMA_SndStkCsServiceChangeNotify(
    TAF_CS_SERVICE_ENUM_UINT32          enCsState
);




VOS_UINT32 TAF_MMA_IsNeedSndStkEvt(VOS_VOID);

VOS_VOID TAF_MMA_SndMtaAreaLostInd(VOS_VOID);

VOS_UINT32  TAF_MMA_PlmnReselectAutoReq_PowerOff(VOS_UINT32 ulOpID);

VOS_UINT32  TAF_MMA_RcvPlmnSelectionReq_PowerOff(
    VOS_UINT32                          ulOpID,
    TAF_PLMN_ID_STRU                    stPlmnId,
    TAF_PH_RA_MODE                      ucAccessMode,
    VOS_UINT8                           ucReselMode
);

VOS_UINT32 TAF_MMA_IsPsServDomainAvail(
    TAF_PH_MS_CLASS_TYPE                enMsClassType
);


/*=======================================================*/

/*=================================================================*/
#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif
#ifdef __cplusplus
}
#endif
/*****************************************************************************/

/*===========================================================================*/
#endif      /* __STATUS_H__*/

/***** End of the file *****/
