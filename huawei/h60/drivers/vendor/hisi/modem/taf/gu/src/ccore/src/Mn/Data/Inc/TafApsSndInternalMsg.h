
#ifndef _TAF_APS_SND_INTERNAL_MSG_H_
#define _TAF_APS_SND_INTERNAL_MSG_H_

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "Taf_Aps.h"

#include "TafSmInterface.h"

#include "TafApsCtx.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/

/* ��װ�ڲ���ϢOSA��Ϣͷ */
#define TAF_APS_CFG_INTER_MSG_HDR(pstMsg, ulLen)\
            (pstMsg)->ulSenderCpuId   = VOS_LOCAL_CPUID;\
            (pstMsg)->ulSenderPid     = WUEPS_PID_TAF;\
            (pstMsg)->ulReceiverCpuId = VOS_LOCAL_CPUID;\
            (pstMsg)->ulReceiverPid   = WUEPS_PID_TAF;\
            (pstMsg)->ulLength        = (ulLen)


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/



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
    MSG_HEADER_STRU                     stMsgHeader;
    VOS_UINT8                           ucPdpId;
    VOS_UINT8                           aucReserved[1];
    SM_TAF_CAUSE_ENUM_UINT16            enCause;

} TAF_APS_INTER_PDP_DEACTIVATE_REQ_STRU;


typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;
    VOS_UINT8                           ucPdpId;
    VOS_UINT8                           aucReserved[3];
    APS_PDP_ACT_REQ_ST                  stApsPdpActReq;

} TAF_APS_INTER_PDP_ACTIVATE_REQ_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT32                          enMsgId;            /* ��Ϣ����     */  /* _H2ASN_Skip */
    TAF_APS_SWITCH_DDR_RATE_INFO_STRU   stDfsInfo;
}TAF_APS_MNTN_DFS_INFO_STRU;

#if (FEATURE_ON == FEATURE_LTE)

typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT32                          enMsgId;            /* ��Ϣ����     */  /* _H2ASN_Skip */
} TAF_APS_ATTACH_BEARER_ACTIVATE_IND_STRU;
#endif


/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/

/*****************************************************************************
 �� �� ��  : TAF_APS_SndInterPdpDeactivateReq
 ��������  : �����ڲ�ȥ����PDP������Ϣ
 �������  : enCause                    - ȥ����ԭ��
 �������  : ��
 �� �� ֵ  : VOS_OK                     - �����ڲ���Ϣ�ɹ�
             VOS_OERR                   - �����ڲ���Ϣʧ��
*****************************************************************************/
VOS_UINT32 TAF_APS_SndInterPdpDeactivateReq(
    VOS_UINT8                           ucPdpId,
    SM_TAF_CAUSE_ENUM_UINT16            enCause
);

/*****************************************************************************
 �� �� ��  : TAF_APS_SndLocalPdpDeactivateInd
 ��������  : ���ͱ���ȥ����PDP������Ϣ
 �������  : enCause                  - ȥ����ԭ��
 �������  : ��
 �� �� ֵ  : VOS_OK                     - �����ڲ���Ϣ�ɹ�
             VOS_OERR                   - �����ڲ���Ϣʧ��
*****************************************************************************/
VOS_UINT32 TAF_APS_SndLocalPdpDeactivateInd(
    VOS_UINT8                           ucPdpId,
    SM_TAF_CAUSE_ENUM_UINT16            enCause
);

/*****************************************************************************
 �� �� ��  : TAF_APS_SndLocalAllSecPdpDeactivateInd
 ��������  : ���ͱ������д�PDPȥ����ָʾ
 �������  : VOS_UINT8 ucPdpId
             VOS_UINT8 ucSmCause
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_VOID TAF_APS_SndLocalAllSecPdpDeactivateInd(
    VOS_UINT8                           ucPdpId,
    SM_TAF_CAUSE_ENUM_UINT16            enCause
);

/*****************************************************************************
 �� �� ��  : TAF_APS_SndInterPdpActivateReq
 ��������  : �����ڲ�����PDP������Ϣ
 �������  : ucPdpId
             stApsPdpActReq
 �������  : ��
 �� �� ֵ  : VOS_OK                     - �����ڲ���Ϣ�ɹ�
             VOS_OERR                   - �����ڲ���Ϣʧ��
*****************************************************************************/
VOS_UINT32 TAF_APS_SndInterPdpActivateReq(
    VOS_UINT8                           ucPdpId,
    APS_PDP_ACT_REQ_ST                  stApsPdpActReq
);

VOS_VOID  TAF_APS_SndOmDfsInfo(TAF_APS_SWITCH_DDR_RATE_INFO_STRU *pstSwitchDdrInfo);

#if (FEATURE_ON == FEATURE_LTE)
/*****************************************************************************
 �� �� ��  : TAF_APS_SndInterAttachBearerActivateInd
 ��������  : ����ע����ؼ���ָʾ��Ϣ
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_VOID
*****************************************************************************/
VOS_VOID TAF_APS_SndInterAttachBearerActivateInd(VOS_VOID);

/*****************************************************************************
 �� �� ��  : TAF_APS_SndInterFsmEntryMsg
 ��������  : ����״̬�������Ϣ���ڲ���Ϣ����
 �������  : pstEntryMsg --- ״̬�������Ϣ
 �������  : ��
 �� �� ֵ  : VOS_VOID
*****************************************************************************/
VOS_VOID TAF_APS_SndInterFsmEntryMsg(TAF_APS_ENTRY_MSG_STRU *pstEntryMsg);
#endif


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

#endif

