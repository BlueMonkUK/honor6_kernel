

#ifndef __TAFAPSCTX_H__
#define __TAFAPSCTX_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "TafFsm.h"
#include "TafApsTimerMgmt.h"
#include "TafApsDsFlowStats.h"
#include "AtMnInterface.h"
#include "SmEsmInterface.h"
#include "MmcApsInterface.h"
#include "Taf_Aps.h"
#include "NasNvInterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define TAF_APS_MAX_MSG_BUFFER_LEN                      (1500)                  /* �������󳤶� */
#define TAF_APS_MAX_MSG_QUEUE_NUM                       (22)                    /* ��󻺴�ĸ��� */

#define TAF_APS_RAB_ID_OFFSET                           (5)                     /* ��ǰ������Ķ�����5��15�������һ��5��ƫ���� */

#define TAF_APS_RAB_ID_MIN                              (5)                     /* RAB ID��ȡֵ��Χ����С��5 */
#define TAF_APS_RAB_ID_MAX                              (15)                    /* RAB ID��ȡֵ��Χ�������15 */

/* ���RAB���ظ��� */
#define TAF_APS_MAX_RAB_NUM                             (15)                    /* RAB ID��ȡֵ��Χ�������15 */


#define TAF_APS_MAX_CMD_BUFFER_QUEUE_SIZE               (11)                    /* �ȴ���Ϣ����Ķ��г��ȣ�Ŀǰ�ٶ�Ϊ10�� */

#define TAF_APS_INVALID_PDPID                           (0xFF)                  /* ��Ч��PDP ID */
#define TAF_APS_INVALID_CID                             (0xFF)                  /* ��Ч��CID */
#define TAF_APS_INVALID_CLIENTID                        (0xFFFE)                /* ��Ч��CLIENT ID */
#define TAF_APS_INVALID_OPID                            (0xFF)                  /* ��Ч��OPID ID */

#define TAF_APS_MAX_MSG_QUEUE_SIZE                      (11)                    /* �ڲ���Ϣ�������ĳ��ȣ�
                                                                                   ��ǰ״̬���ڲ�������Ӧ�����ʮ��PDPʵ�壬Ŀǰʹ�õĳ�����12 */
#define TAF_APS_INVALID_INTERNAL_MSG_ID                 (0xFFFF)                /* ��ϢID����Чֵ */

#define TAF_APS_32BIT_DATA_LEN                          (32)                    /* 32λ���ݵ�bit���� */
#define TAF_APS_DEFAULT_CID                             (0)                     /* Ĭ�ϳ�����ʹ�õ�CID��CID0 */
#define TAF_APS_TI_MASK                                 (7)                     /* ȥ��Ti��flagֵʱʹ�� */
#define TAF_APS_OCTET_MOVE_THREE_BITS                   (0x03)                  /* ��һ���ֽ��ƶ�3λ */

/* PDP�����10��֮�ڣ��������û������෢��PDP�������� */
#define TAF_APS_PDP_ACT_LIMIT_NO_DELAY_COUNT_THRESHOLD   (10)

/* PDP�����11-19��֮�ڣ�����10s��������������ʱ�� */
#define TAF_APS_PDP_ACT_LIMIT_STEP_DELAY_COUNT_THRESHOLD (19)
#define TAF_APS_PDP_ACT_LIMIT_STEP_TIME                  (10)

/* PDP�����19��(����19��)���ϣ�����90sʱ��������ʱ�� */
#define TAF_APS_PDP_ACT_LIMIT_AVERAGE_DELAY_TIME         (90)

/* PDP��ͣ���ʱ��ʱ�����������ֵ */
#define TAF_APS_PDP_ACT_LIMIT_RANDOM_TIME_THRESHOLD      (45)

/* ��ʧ�ܴ�����10��������ʱ��PDP��ͣ���ʱ��ʹ�����ʱ�� */
#define TAF_APS_PDP_ACT_LIMIT_RANDOM_TIME_FAILCOUNT      (10)

/* TI_TAF_APS_LIMIT_PDP_ACT��ʱ�������κ�PDP ID������Զ���һ����Чֵ */
#define TAF_APS_PDP_ACT_LIMIT_PDP_ID                    (0xFF)

#define TAF_APS_DSFLOW_AT_REPORT_PERIOD                 (2)

#define TAF_APS_GET_MULTI_DFS_SUPPORT_FLG()     (g_stTafApsCtx.stSwitchDdrRateInfo.ulMultiDfsFlg)

#define TAF_APS_GET_MULTI_DFS_PROFILE_NUM(dfsType)             (g_stTafApsCtx.stSwitchDdrRateInfo.astDfsConfig[dfsType].ulProfileNum)
#define TAF_APS_GET_MULTI_DFS_PROFILE_DL_RATE(dfsType, i)      (g_stTafApsCtx.stSwitchDdrRateInfo.astDfsConfig[dfsType].astProfile[i].ulDlRate)
#define TAF_APS_GET_MULTI_DFS_PROFILE_UL_RATE(dfsType, i)      (g_stTafApsCtx.stSwitchDdrRateInfo.astDfsConfig[dfsType].astProfile[i].ulUlRate)
#define TAF_APS_GET_MULTI_DFS_PROFILE_DDR_BAND(dfsType, i)     (g_stTafApsCtx.stSwitchDdrRateInfo.astDfsConfig[dfsType].astProfile[i].ulDdrBand)

#define TAF_APS_GET_DFS_PROFILE_NUM()           (g_stTafApsCtx.stSwitchDdrRateInfo.astDfsConfig[0].ulProfileNum)
#define TAF_APS_GET_DFS_PROFILE_DL_RATE(i)      (g_stTafApsCtx.stSwitchDdrRateInfo.astDfsConfig[0].astProfile[i].ulDlRate)
#define TAF_APS_GET_DFS_PROFILE_UL_RATE(i)      (g_stTafApsCtx.stSwitchDdrRateInfo.astDfsConfig[0].astProfile[i].ulUlRate)
#define TAF_APS_GET_DFS_PROFILE_DDR_BAND(i)     (g_stTafApsCtx.stSwitchDdrRateInfo.astDfsConfig[0].astProfile[i].ulDdrBand)

#define TAF_APS_DFS_REQUEST_SUCC_CNT(i)         (g_stTafApsCtx.stSwitchDdrRateInfo.stDfsDebugInfo.ulRequestSucCnt  += i)
#define TAF_APS_DFS_REQUEST_FAIL_CNT(i)         (g_stTafApsCtx.stSwitchDdrRateInfo.stDfsDebugInfo.ulRequestFailCnt  += i)
#define TAF_APS_DFS_UPDATE_SUCC_CNT(i)          (g_stTafApsCtx.stSwitchDdrRateInfo.stDfsDebugInfo.ulUpdateSucCnt  += i)
#define TAF_APS_DFS_UPDATE_FAIL_CNT(i)          (g_stTafApsCtx.stSwitchDdrRateInfo.stDfsDebugInfo.ulUpdateFailCnt += i)

#define TAF_APS_GET_DSFLOW_RELEASE_DFS()        (g_stTafApsCtx.stSwitchDdrRateInfo.ulMinBand)
#define TAF_APS_GET_DSFLOW_MIN_DDR_BAND()       (g_stTafApsCtx.stSwitchDdrRateInfo.ulMinBand)
#define TAF_APS_GET_DSFLOW_MAX_DDR_BAND()       (g_stTafApsCtx.stSwitchDdrRateInfo.ulMaxBand)


#define TAF_APS_GET_RAT_TYPE_IN_SUSPEND()           (g_stTafApsCtx.enCurrRatType)
#define TAF_APS_SET_RAT_TYPE_IN_SUSPEND(ratType)    (g_stTafApsCtx.enCurrRatType = (ratType))

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

enum TAF_APS_FSM_ID_ENUM
{
    /* TAF APS MAIN ״̬������ǰ������̬ */
    TAF_APS_FSM_MAIN                                    =0X00,

    /* TAF APS MS ACTIVATING״̬������ǰ��UE���𼤻���������� */
    TAF_APS_FSM_MS_ACTIVATING                           =0X01,

    /* TAF APS NW ACTIVATING״̬������ǰ��NW���𼤻���������� */
    TAF_APS_FSM_NW_ACTIVATING                           =0X02,

    /* TAF APS MS DEACTIVATING״̬������ǰ��UE����ȥ������������� */
    TAF_APS_FSM_MS_DEACTIVATING                         =0X03,

    /* TAF APS MS MODIFYING״̬������ǰ��UE����MODIFY���������� */
    TAF_APS_FSM_MS_MODIFYING                            =0X04,

    TAF_APS_FSM_BUTT

};
typedef VOS_UINT32 TAF_APS_FSM_ID_ENUM_UINT32;
enum TAF_APS_DFS_TYPE_ENUM
{
    TAF_APS_DFS_TYPE_BASE               = 0x00,                                 /*��ǰδ���κ�������פ��*/
    TAF_APS_DFS_TYPE_GSM                = 0x01,                                 /* GSM��DFS */
    TAF_APS_DFS_TYPE_WCDMA              = 0x02,                                 /* WCDMA��DFS */
    TAF_APS_DFS_TYPE_LTE                = 0x03,                                 /* LTE��DFS */
    TAF_APS_DFS_TYPE_TD_WCDMA           = 0x04,                                 /* TD-WCDMA��DFS */
    TAF_APS_DFS_TYPE_CDMA               = 0x05,                                 /* CDMA��DFS */
    TAF_APS_DFS_TYPE_BUTT                                                       /* ��Ч�Ľ��뼼�� */
};
typedef VOS_UINT32 TAF_APS_DFS_TYPE_ENUM_UINT32;

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

typedef SM_ESM_EPS_BEARER_INFO_IND_STRU TAF_APS_EPS_BEARER_INFO_IND_STRU;




typedef struct
{
    VOS_UINT8                           ucRabActiveFlag;                        /*��ʶ��RAB�Ƿ񼤻�*/
    VOS_UINT8                           aucReserve[3];                          /*����λ*/

    VOS_UINT32                          ulStartLinkTime;                        /*��ǰ���ӿ�ʼʱ��*/
    TAF_DSFLOW_INFO_STRU                stCurrentFlowInfo;  /*��ǰ����������Ϣ�������һ��PDP���ӵ�������Ϣ */
    TAF_DSFLOW_INFO_STRU                stTotalFlowInfo;    /*�ۻ�������Ϣ����RAB��ʷ���������ۼӵ�������Ϣ */

}TAF_APS_DSFLOW_STATS_STRU;
typedef struct
{
    VOS_UINT32                          ulRequestSucCnt;
    VOS_UINT32                          ulRequestFailCnt;
    VOS_UINT32                          ulUpdateSucCnt;                         /* ͶƱ�ɹ����� */
    VOS_UINT32                          ulUpdateFailCnt;                        /* ͶƱʧ�ܴ��� */
}TAF_APS_DFS_DEBUG_INFO_STRU;


typedef struct
{
    VOS_INT32                           iDfsReqId;                              /* DDR��ƵͶƱ����ID */
    VOS_UINT32                          ulReqValue;                             /* DDR��ƵͶƱ����ֵ */
    VOS_UINT32                          ulSetMaxFlag;                           /* �������Ƶ�ʵı�ʶ */

    VOS_UINT32                          ulMultiDfsFlg;
    TAF_NV_DFS_DSFLOW_RATE_CONFIG_STRU  astDfsConfig[TAF_NVIM_DIFF_DFS_NUM];
    TAF_APS_DFS_DEBUG_INFO_STRU         stDfsDebugInfo;

    VOS_UINT32                          ulMinBand;                              /* ��СDDRƵ�� */
    VOS_UINT32                          ulMaxBand;                              /* ���DDRƵ�� */
}TAF_APS_SWITCH_DDR_RATE_INFO_STRU;
typedef struct
{
    VOS_UINT8                           ucApsDsFlowSave2NvFlg;                  /* ��ʶ�Ƿ���Ҫ������ʷ������Ϣ��NV�� */

    VOS_UINT8                           ucDsFlowOMReportFlg;                    /* ��ʶ�Ƿ����OM�����ϱ� */
    VOS_UINT8                           ucDsFlowATRptPeriod;                    /* AT�˿������ϱ������ڵ�λs */
    VOS_UINT8                           ucDsFlowOMRptPeriod;                    /* OM�����ϱ������ڵ�λs */
    VOS_UINT8                           ucDsFlowATRptTmrExpCnt;                 /* AT�˿������ϱ�ʱ�������ڿ����ϱ����� */
    VOS_UINT8                           ucDsFlowOMRptTmrExpCnt;                 /* OM�����ϱ�ʱ�������ڿ����ϱ����� */

    VOS_UINT8                           ucApsDsFlowSavePeriod;                  /* ����дNV������ */
    VOS_UINT8                           ucReserve;
    TAF_APS_DSFLOW_STATS_STRU           astApsDsFlowStats[TAF_APS_MAX_RAB_NUM]; /* ������RABIDΪ��λ���������� */

}TAF_APS_DSFLOW_STATS_CTX_STRU;



typedef struct
{
    TAF_APS_TIMER_ID_ENUM_UINT32        enTimerId;
    VOS_UINT32                         *pulMsgInfo;
    VOS_UINT32                          ulMsgInfoLen;
    VOS_UINT32                          ulPara;
}TAF_APS_CMD_BUFFER_STRU;


typedef struct
{
    VOS_UINT32                          ulEventType;
    VOS_UINT8                           aucEntryMsgBuffer[TAF_APS_MAX_MSG_BUFFER_LEN];
} TAF_APS_ENTRY_MSG_STRU;


typedef struct
{
    TAF_APS_ENTRY_MSG_STRU              stMsgEntry;                             /* ����ľ������� */
}TAF_APS_CACHE_MSG_INFO_STRU;
typedef struct
{
    VOS_UINT32                          ucCurrIndex;                            /* ��¼��ǰ�������index */
    VOS_UINT8                           ucCacheMsgNum;                          /* ������Ϣ�ĸ��� */
    VOS_UINT8                           aucReserve[3];
    TAF_APS_CACHE_MSG_INFO_STRU         astMsgQueue[TAF_APS_MAX_MSG_QUEUE_NUM]; /* APS����Ϣ�������飬�洢���Ǵ�ͨ����Ϣͷ����Ϣ */
}TAF_APS_MSG_QUEUE_STRU;
typedef struct
{
    /* TAF APS��Ϣ����*/
    AT_MN_MSGTYPE_ENUM_UINT16           enMsgType;
    VOS_UINT8                           ucPdpid;
    VOS_UINT8                           ucReserved;
} TAF_APS_PS_CALL_ANSWER_MSG_STRU;
typedef struct
{
    TAF_APS_PS_CALL_ANSWER_MSG_STRU     stAnswerMsg;

} TAF_APS_FSM_NW_ACTIVATING_CTX_STRU;


typedef union
{
    TAF_APS_FSM_NW_ACTIVATING_CTX_STRU  stNwActivatingCtx;

}TAF_APS_FSM_EXTRA_CTX_UNION;


typedef struct
{
    /* ״̬������ָ��,ָ��ΪNULL��ʾ��ǰ״̬����Ч  */
    TAF_FSM_DESC_STRU                  *pstFsmDesc;

    /* ��ǰ״̬����ʶ */
    TAF_APS_FSM_ID_ENUM_UINT32          enFsmId;

    /* ��ǰ״̬ */
    VOS_UINT32                          ulState;                            /*  */

    /* ��״̬�������ϢMSGID������  */
    TAF_APS_ENTRY_MSG_STRU              stEntryMsg;

    /* ״̬������ */
    TAF_APS_FSM_EXTRA_CTX_UNION         unFsmCtx;
}TAF_APS_SUB_FSM_CTX_STRU;


typedef struct
{
    /* ״̬������ָ��,ָ��ΪNULL��ʾ��ǰ״̬����Ч  */
    TAF_FSM_DESC_STRU                  *pstFsmDesc;

    /* ��ǰ״̬����ʶ */
    TAF_APS_FSM_ID_ENUM_UINT32          enFsmId;

    /* ��ǰ״̬ */
    VOS_UINT32                          ulState;                            /*  */
}TAF_APS_MAIN_FSM_CTX_STRU;


typedef struct
{
    TAF_APS_MAIN_FSM_CTX_STRU           stMainFsm;                              /* ��״̬�� */
    TAF_APS_SUB_FSM_CTX_STRU            stSubFsm;                               /* ��״̬�� */

    /**************************************************************************
                        TAF APS ��ǰ������״̬��PS��Ŀ�״̬������״̬��ʵ���������ά��
    **************************************************************************/
    MMC_APS_RAT_TYPE_ENUM_UINT32        enRatType;                              /* ��ѯ���صĵ�ǰ���� */
    VOS_UINT32                          ulPsSimRegStatus;                       /* PS��SIM��״̬��Ϣ,VOS_TRUE:����Ч,VOS_FALSE:����Ч */
    VOS_UINT8                           ucPdpId;                                /* ��ǰ״̬������Ӧ��PDP ID */
    VOS_UINT8                           aucReserve[3];
}TAF_APS_PDP_ENTITY_FSM_CTX_STRU;
typedef struct
{
    VOS_UINT32                          ulDefaultBearerInfoFlg;                 /* ��ǰĬ�ϳ�����Ϣ�Ƿ�ʹ�õı�ǣ�VOS_TRUE:��ǰ�Ѿ�ʹ�ã�
                                                                                   VOS_FALSE:��ǰ��û��ʹ�� */
    TAF_APS_EPS_BEARER_INFO_IND_STRU    stDefaultBearerInfo;                    /* �洢��Ĭ�ϳ�����Ϣ */
}TAF_APS_DEFAULT_BEARER_INFO_STRU;
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT16                          usMsgID;                                /* ��ϢID */
    VOS_UINT8                           aucReserve[2];

    VOS_UINT8                           aucBuf[TAF_APS_MAX_MSG_BUFFER_LEN];     /* ��Ϣ���� */
}TAF_APS_INTERNAL_MSG_BUF_STRU;


typedef struct
{
    /* TAF APS�ڲ���Ϣ���� */
    TAF_APS_INTERNAL_MSG_BUF_STRU       astApsMsgQueue[TAF_APS_MAX_MSG_QUEUE_SIZE];

    /* ������Ϣ��ʱ����buff,����ֲ�����ʱֱ��ʹ�øÿռ�,Ϊ�˱��ⷢ���ڲ���Ϣʱ
    ռ�ýϴ��ջ�ռ� */
    TAF_APS_INTERNAL_MSG_BUF_STRU       stSendMsgBuf;

    /*�ڲ���Ϣ������е�����βָ��*/
    VOS_UINT8                           ucTail;

    /*�ڲ���Ϣ������е�����ͷָ��*/
    VOS_UINT8                           ucHeader;

    VOS_UINT16                          usReserved;
} TAF_APS_INTERNAL_MSG_QUEUE_STRU;



typedef struct
{
    VOS_UINT8                           ucPdpActLimitFlg;                       /* ����NV��򿪹رձ�־ */
    VOS_UINT8                           ucPdpActFailCount;                      /* ��¼PDP����ʧ�ܴ��� */
    VOS_UINT8                           aucReserved[2];                         /* ���� ���ֽڶ��� */
}TAF_APS_PDP_ACT_LIMIT_INFO_STRU;


typedef struct
{
    /**************************************************************************
                        �洢Lģ��Ĭ�ϳ���
    **************************************************************************/
    TAF_APS_DEFAULT_BEARER_INFO_STRU        stLteDefaultBearerInfo;             /* ���Ĭ�ϳ�����Ϣ */

    /**************************************************************************
                        TAF APS״̬��״̬
    **************************************************************************/
    TAF_APS_PDP_ENTITY_FSM_CTX_STRU         astPdpEntityFsmCtx[TAF_APS_MAX_PDPID];   /* ��ǰ���11��PDP�����ģ������11��������״̬�� */
    TAF_APS_PDP_ENTITY_FSM_CTX_STRU        *pstCurrPdpEntityFsmCtx;                    /* ��ǰ��ʹ�õ�ʵ��״̬�� */

    TAF_APS_TIMER_CTX_STRU                  astApsTimerCtx[TAF_APS_MAX_TIMER_NUM];  /* APS��ǰ�������еĶ�ʱ����Դ */
    TAF_APS_DSFLOW_STATS_CTX_STRU           stApsDsFlowCtx;                         /* APS����ͳ�������� */

    /**************************************************************************
                        TAF APS������Ϣ����
    **************************************************************************/
    TAF_APS_MSG_QUEUE_STRU                  stBufferEntryMsgQueue;              /* APS�Ļ�����Ϣ���� */

    /**************************************************************************
                        TAF APS�����������
    **************************************************************************/
    TAF_APS_CMD_BUFFER_STRU                 astCmdBufferQueue[TAF_APS_MAX_CMD_BUFFER_QUEUE_SIZE]; /* �ȴ�L4A���ò�ѯ����Ķ��� */

    /**************************************************************************
                        APS �ڲ���Ϣ�������
    **************************************************************************/
    TAF_APS_INTERNAL_MSG_QUEUE_STRU         stInternalMsgQueue;                     /* MM�Ӳ���ڲ���Ϣ���� */

    TAF_APS_PDP_ACT_LIMIT_INFO_STRU         stPdpActLimitInfo;

    TAF_APS_SWITCH_DDR_RATE_INFO_STRU       stSwitchDdrRateInfo;                    /* DDRͶƱ��Ϣ */

    VOS_UINT32                              aulCallRemainTmrLen[TAF_MAX_CID + 1];
    MMC_APS_RAT_TYPE_ENUM_UINT32            enCurrRatType;                      /* ��¼��������й���ʱ�Ľ��뼼��ģʽ */
}TAF_APS_CONTEXT_STRU;

extern TAF_APS_CONTEXT_STRU             g_stTafApsCtx;


/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/

TAF_APS_PDP_ENTITY_FSM_CTX_STRU* TAF_APS_GetPdpEntityFsm(
    VOS_UINT32                          ulMainFsmState,
    TAF_APS_FSM_ID_ENUM_UINT32          enSubFsmId,
    VOS_UINT32                          ulSubFsmState
);

VOS_VOID  TAF_APS_InitDsFlowCtx(
    TAF_APS_DSFLOW_STATS_CTX_STRU      *pstApsTimerCtx
);

TAF_APS_DSFLOW_STATS_CTX_STRU*  TAF_APS_GetDsFlowCtxAddr( VOS_VOID );

TAF_APS_TIMER_CTX_STRU*  TAF_APS_GetTimerCtxAddr( VOS_VOID );




TAF_APS_MSG_QUEUE_STRU* TAF_APS_GetCachMsgBufferAddr(VOS_VOID);


VOS_UINT8  TAF_APS_GetCacheNum( VOS_VOID );

VOS_VOID  TAF_APS_SaveCacheMsgInMsgQueue(
    VOS_UINT32                          ulEventType,
    VOS_VOID                           *pstMsg
);

VOS_UINT32  TAF_APS_SaveCacheMsg(
    VOS_UINT32                          ulEventType,
    VOS_VOID                           *pstMsg
);

VOS_UINT32  TAF_APS_ClearCacheMsg(
    VOS_UINT32                          ulEventType
);

VOS_UINT32  TAF_APS_GetNextCachedMsg(
    TAF_APS_ENTRY_MSG_STRU             *pstEntryMsg
);

VOS_VOID  TAF_APS_SetCurrPdpEntityMainFsmState(
    VOS_UINT32                          ulState
);


VOS_VOID  TAF_APS_SetCurrPdpEntitySubFsmState(
    VOS_UINT32                          ulState
);


VOS_VOID  TAF_APS_DelWaitInfoInL4aMsgRsltQueue(
    AT_MN_MSGTYPE_ENUM_UINT16           enMsgType
);

VOS_UINT32  TAF_APS_SetPdpIdMainFsmState(
    VOS_UINT8                          ucPdpId,
    VOS_UINT32                         ulState
);

VOS_UINT32  TAF_APS_GetPdpIdMainFsmState(
    VOS_UINT8                          ucPdpId
);

VOS_UINT32  TAF_APS_CheckPdpServiceActivated(
    VOS_UINT8                          ucPdpId
);

VOS_UINT32  TAF_APS_SetPdpIdSubFsmState(
    VOS_UINT8                          ucPdpId,
    VOS_UINT32                         ulState
);

VOS_UINT32  TAF_APS_GetPdpIdSubFsmState(
    VOS_UINT8                          ucPdpId
);

VOS_VOID  TAF_APS_SaveWaitInfoInWaitL4aMsgRsltQueue(
    TAF_APS_TIMER_ID_ENUM_UINT32        enTimerId,
    VOS_UINT16                          usClientId,
    AT_MN_MSGTYPE_ENUM_UINT16           enMsgType,
    VOS_UINT8                           ucOpId
);

TAF_APS_PDP_ENTITY_FSM_CTX_STRU* TAF_APS_GetCurrPdpEntityFsmAddr(VOS_VOID);


VOS_VOID  TAF_APS_InitCmdBufferQueue(
    TAF_APS_CMD_BUFFER_STRU            *pstCmdBufferQueue
);

TAF_APS_CMD_BUFFER_STRU  *TAF_APS_GetCmdBufferQueueAddr( VOS_VOID );

VOS_VOID  TAF_APS_DelItemInCmdBufferQueue(
    TAF_APS_TIMER_ID_ENUM_UINT32        enTimerId,
    VOS_UINT32                          ulPara
);

VOS_VOID  TAF_APS_SaveItemInCmdBufferQueue(
    TAF_APS_TIMER_ID_ENUM_UINT32        enTimerId,
    VOS_UINT32                         *pulMsgInfo,
    VOS_UINT32                          ulMsgInfoLen,
    VOS_UINT32                          ulPara
);

TAF_APS_CMD_BUFFER_STRU  *TAF_APS_GetItemFromCmdBufferQueue(
    TAF_APS_TIMER_ID_ENUM_UINT32        enTimerId,
    VOS_UINT32                          ulPara
);


VOS_VOID TAF_APS_SetCurrPdpEntityFsmAddr(
    VOS_UINT8                           ucPdpId
);

VOS_VOID TAF_APS_SaveCurrSubFsmEntryMsg(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_VOID  TAF_APS_InitMainFsmCtx(
    TAF_APS_MAIN_FSM_CTX_STRU                *pstCurrFsmCtx
);

VOS_VOID  TAF_APS_InitSubFsmCtx(
    TAF_APS_SUB_FSM_CTX_STRU                *pstCurrFsmCtx
);

VOS_VOID  TAF_APS_InitInternalBuffer(
    TAF_APS_MSG_QUEUE_STRU             *pstBufferEntryMsgQueue
);

VOS_VOID TAF_APS_QuitCurrSubFsm( VOS_VOID );

VOS_VOID TAF_APS_InitSubFsm(
    TAF_APS_FSM_ID_ENUM_UINT32          enFsmId,
    TAF_FSM_DESC_STRU                  *pstFsmDesc,
    VOS_UINT32                          ulState
);
VOS_VOID  TAF_APS_SetCurrFsmEntityPdpId(
    VOS_UINT8                           ucPdpId
);

VOS_UINT8  TAF_APS_GetCurrFsmEntityPdpId(VOS_VOID);

VOS_VOID  TAF_APS_SetCurrPdpEntitySimRegStatus(
    VOS_UINT32                          ulPsSimRegStatus
);

VOS_UINT32  TAF_APS_GetCurrPdpEntitySimRegStatus( VOS_VOID );

VOS_VOID  TAF_APS_SetCurrPdpEntityRatType(
    MMC_APS_RAT_TYPE_ENUM_UINT32                enRatType
);

MMC_APS_RAT_TYPE_ENUM_UINT32  TAF_APS_GetCurrPdpEntityRatType( VOS_VOID );

MMC_APS_RAT_TYPE_ENUM_UINT32 TAF_APS_GetPdpEntityRatType(
    VOS_UINT8                          ucPdpId
);

VOS_VOID  TAF_APS_SaveDefaultBearInfo(
    TAF_APS_EPS_BEARER_INFO_IND_STRU       *pstDefaultBearerInfo
);

TAF_APS_EPS_BEARER_INFO_IND_STRU*  TAF_APS_GetDefaultBearInfo( VOS_VOID );

VOS_VOID  TAF_APS_SetDefaultBearInfoStatus(
    VOS_UINT32                          ulDefaultBearerInfoFlg
);

TAF_APS_ENTRY_MSG_STRU* TAF_APS_GetCurrSubFsmMsgAddr(VOS_VOID);

VOS_VOID  TAF_APS_GetPsSimRegStatus(
    VOS_UINT32                          ulPsSimRegStatus
);

VOS_VOID  TAF_APS_SetPsSimRegStatus(
    VOS_UINT32                          ulPsSimRegStatus
);

TAF_APS_INTERNAL_MSG_BUF_STRU* TAF_APS_GetNextInternalMsg( VOS_VOID );
VOS_UINT32  TAF_APS_SndInternalMsg(
    VOS_VOID                           *pSndMsg
);
TAF_APS_INTERNAL_MSG_BUF_STRU *TAF_APS_GetIntMsgSendBuf(
    VOS_UINT32                          ulLen
);
VOS_VOID  TAF_APS_InitInternalMsgQueue(
    TAF_APS_INTERNAL_MSG_QUEUE_STRU     *pstInternalMsgQueue                   /* MM�Ӳ���ڲ���Ϣ���� */
);
TAF_APS_CONTEXT_STRU*  TAF_APS_GetApsCtxAddr( VOS_VOID );

VOS_VOID  TAF_APS_SaveCachePsCallAnswerMsg(
    AT_MN_MSGTYPE_ENUM_UINT16           enMsgType,
    VOS_UINT8                           ucPdpid
);



TAF_APS_PDP_ACT_LIMIT_INFO_STRU* TAF_APS_GetPdpActLimitInfoAddr(VOS_VOID);
VOS_VOID TAF_APS_InitPdpActLimitInfo(VOS_VOID);



VOS_VOID TAF_APS_InitSwitchDdrInfo(VOS_VOID);
TAF_APS_SWITCH_DDR_RATE_INFO_STRU*  TAF_APS_GetSwitchDdrRateInfoAddr(VOS_VOID);

VOS_VOID TAF_APS_InitCallRemainTmrLen(VOS_VOID);
VOS_VOID TAF_APS_SetCallRemainTmrLen(
    VOS_UINT8                           ucCid,
    VOS_UINT32                          ulTmrLen
);
VOS_UINT32 TAF_APS_GetCallRemainTmrLen(VOS_UINT8 ucCid);

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

#endif /* end of TafApsCtx.h */
