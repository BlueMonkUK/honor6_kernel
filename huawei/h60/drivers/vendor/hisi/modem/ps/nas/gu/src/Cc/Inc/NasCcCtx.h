
#ifndef _NAS_CC_CTX_H_
#define _NAS_CC_CTX_H_

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/

#include "vos.h"
#include "PsTypeDef.h"
#include "NasCcCommon.h"
#include "NasCcIe.h"
#include "NasCcAirMsg.h"


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
#define NAS_CC_ONE_THOUSAND_MILLISECOND                    (1000)              /* 1000MS */

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

enum NAS_CC_SS_SWITCH_STATE_ENUM
{
    NAS_CC_SS_SWITCH_IDLE               = 0,                                    /* ��ǰû�в���ҵ��״̬�л� */
    NAS_CC_SS_SWITCH_WAIT_CNF,                                                  /* �����л����󣬵ȴ�����Ļظ� */
    NAS_CC_SS_SWITCH_SUCCESS,                                                   /* �����ѻظ�ACK��Ϣ */
    NAS_CC_SS_SWITCH_FAILED,                                                    /* ����ظ�REJ��ERR */
    NAS_CC_SS_SWITCH_TIME_OUT,                                                  /* ����δ�ظ����³�ʱ */

    NAS_CC_SS_SWITCH_LAST_CALL_RELEASED,

    NAS_CC_SS_SWITCH_STATE_BUTT
};
typedef VOS_UINT8 NAS_CC_SS_SWITCH_STATE_ENUM_UINT8;



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


typedef struct
{
    VOS_UINT8                           ucCcbsSupportFlg;                       /*CCBS(��æ�������)ҵ��*/
    VOS_UINT8                           aucReserved[3];
}NAS_CC_CUSTOM_CFG_INFO_STRU;
typedef struct
{
    NAS_CC_CUSTOM_CFG_INFO_STRU        stCustomCfg;                             /* NVIM�еĶ�����Ϣ */
}NAS_CC_MS_CFG_INFO_STRU;
typedef struct
{
    NAS_CC_SS_SWITCH_STATE_ENUM_UINT8   enHoldState;                            /* ���б���ҵ���л�״̬ */
    NAS_CC_CAUSE_VALUE_ENUM_U8          enHoldCause;                            /* ���б���ҵ���л�״̬ԭ��ֵ */
    NAS_CC_SS_SWITCH_STATE_ENUM_UINT8   enRetrieveState;                        /* ���лָ�ҵ���л�״̬ */
    NAS_CC_CAUSE_VALUE_ENUM_U8          enRetrieveCause;                        /* ���лָ�ҵ���л�״̬ԭ��ֵ */
    NAS_CC_ENTITY_ID_T                  ulHoldEntityID;                         /* ���б���ҵ��ʵ��ID */
    NAS_CC_ENTITY_ID_T                  ulRetrieveEntityID;                     /* ���лָ�ҵ��ʵ��ID */
    VOS_UINT8                           ucOpFacility;                           /* �Ƿ񱣴��������·���FACILITY��Ϣ */
    VOS_UINT8                           ucOpInvokeId;                           /* �Ƿ񱣴���InvokeId */
    VOS_UINT8                           ucInvokeId;                             /* ��¼InvokeId */
    VOS_UINT8                           aucReserved1[1];
    NAS_CC_MSG_FACILITY_MT_STRU         stFacility;                             /* ��¼�����·���FACILITY��Ϣ */
}NAS_CC_SS_SWITCH_INFO_STRU;


typedef struct
{
    NAS_CC_SS_SWITCH_INFO_STRU          stSwitchInfo;                           /* ͨ������ҵ��״̬�л���Ϣ */
} NAS_CC_SS_CONTROL_STRU;
typedef struct
{
    NAS_CC_MS_CFG_INFO_STRU             stMsCfgInfo;                            /* MS��������Ϣ */
    NAS_CC_SS_CONTROL_STRU              stSsControl;                            /* CC����ҵ�������Ϣ */
} NAS_CC_CONTEXT_STRU;

/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/



/*****************************************************************************
  10 ��������
*****************************************************************************/
NAS_CC_CUSTOM_CFG_INFO_STRU* NAS_CC_GetCustomCfgInfo( VOS_VOID );

NAS_CC_SS_SWITCH_STATE_ENUM_UINT8 NAS_CC_GetSsSwitchHoldState(VOS_VOID);

NAS_CC_SS_SWITCH_STATE_ENUM_UINT8 NAS_CC_GetSsSwitchRetrieveState(VOS_VOID);

NAS_CC_ENTITY_ID_T NAS_CC_GetSsSwitchHoldEntityID(VOS_VOID);

NAS_CC_ENTITY_ID_T NAS_CC_GetSsSwitchRetrieveEntityID(VOS_VOID);

NAS_CC_CAUSE_VALUE_ENUM_U8 NAS_CC_GetSsSwitchHoldCause(VOS_VOID);

NAS_CC_CAUSE_VALUE_ENUM_U8 NAS_CC_GetSsSwitchRetrieveCause(VOS_VOID);

VOS_VOID NAS_CC_SetSsSwitchHoldInfo(
    NAS_CC_ENTITY_ID_T                  ulEntityID,
    NAS_CC_SS_SWITCH_STATE_ENUM_UINT8   enState,
    NAS_CC_CAUSE_VALUE_ENUM_U8          enCause
);

VOS_VOID NAS_CC_SetSsSwitchRetrieveInfo(
    NAS_CC_ENTITY_ID_T                  ulEntityID,
    NAS_CC_SS_SWITCH_STATE_ENUM_UINT8   enState,
    NAS_CC_CAUSE_VALUE_ENUM_U8          enCause
);

VOS_UINT8 NAS_CC_GetSsSwitchOpFacility(VOS_VOID);

VOS_VOID NAS_CC_SetSsSwitchOpFacility(
    VOS_UINT32                          ulOpFlg,
    NAS_CC_MSG_FACILITY_MT_STRU        *pstFacilityMsg
);

NAS_CC_MSG_FACILITY_MT_STRU* NAS_CC_GetSsSwitchFacility(VOS_VOID);

VOS_VOID NAS_CC_SetSsSwitchInvokeId(
    VOS_UINT8                           ucFlg,
    VOS_UINT8                           ucInvokeId);

VOS_UINT8 NAS_CC_GetSsSwitchOpInvokeId(VOS_VOID);

VOS_UINT8 NAS_CC_GetSsSwitchInvokeId(VOS_VOID);

VOS_VOID NAS_CC_InitSsSwitchInfo(VOS_VOID);

VOS_VOID NAS_CC_SetSsSwitchHoldEntityID(NAS_CC_ENTITY_ID_T ulEntityID);

VOS_VOID NAS_CC_SetSsSwitchRetrieveEntityID(NAS_CC_ENTITY_ID_T ulEntityID);


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

#endif /* end of MnCallCtx.h */

