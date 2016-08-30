

#ifndef __TAFAPS_COMFUNC_H__
#define __TAFAPS_COMFUNC_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "NasStkInterface.h"

#include "TafApsGetPdpIdList.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/

#define TAF_APS_GET_SM_ERR_CODE_MAP_TBL_PTR()   (g_astTafApsSmErrCodeMapTbl)
#define TAF_APS_GET_SM_ERR_CODE_MAP_TBL_SIZE()  (sizeof(g_astTafApsSmErrCodeMapTbl)/sizeof(TAF_APS_SM_ERR_CODE_MAP_STRU))

#define TAF_APS_GET_L4A_ERR_CODE_MAP_TBL_PTR()  (g_astTafApsL4aErrCodeMapTbl)
#define TAF_APS_GET_L4A_ERR_CODE_MAP_TBL_SIZE() (sizeof(g_astTafApsL4aErrCodeMapTbl)/sizeof(TAF_APS_L4A_ERR_CODE_MAP_STRU))


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


/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_UINT8 NAS_MML_GetPsAttachAllowFlg(VOS_VOID);

VOS_UINT32   TAF_APS_AllocPdpId(
    TAF_CTRL_STRU                      *pstCtrl,
    VOS_UINT8                           ucCid,
    VOS_UINT8                          *pucPdpId
);
VOS_UINT32   TAF_APS_IsPdpIdValid(
    VOS_UINT8                           ucPdpId
);

VOS_UINT8 TAF_APS_GetCidValue(
    VOS_UINT32                          ulBearerActivateFlg,
    VOS_UINT8                          *pucCidList
);
VOS_UINT32 TAF_APS_CheckCidDefinedCtx(
    VOS_UINT8                           ucCid
);
VOS_UINT8   TAF_APS_GetPdpIdByNsapi(
    VOS_UINT8                           ucNsapi
);
VOS_UINT8 TAF_APS_GetPdpIdByLlcSapi(
    VOS_UINT8                           ucLlcSapi
);

VOS_UINT32 TAF_APS_CheckBearerPdpTypeSuitable(
    VOS_UINT8                           ucDefaultPdpType,
    TAF_PDP_TYPE_ENUM_UINT8             enUserPdpType
);

VOS_UINT32 TAF_APS_String2Hex(
    VOS_UINT8                          *pucStr,
    VOS_UINT16                          usStrLen,
    VOS_UINT32                         *pulResult
);
VOS_UINT8   TAF_APS_GetPdpIdByCid(
    VOS_UINT8                           ucCid
);

VOS_UINT8   TAF_APS_GetPdpEntOpId(
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid
);

VOS_UINT32   TAF_APS_GetPdpEntModuleId(
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid
);

VOS_UINT8  TAF_APS_GetPdpIdByDefaultCid (VOS_VOID);

VOS_VOID TAF_APS_SetPdpEntClientId (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid,
    VOS_UINT16                          usClientId
);
VOS_VOID TAF_APS_SetPdpEntOpId (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid,
    VOS_UINT8                           ucOpId
);
VOS_UINT32  TAF_APS_GetTiFromPdpId(
    VOS_UINT8                           ucPdpId,
    SM_TAF_TRANS_ID_STRU               *pstTi
);
VOS_UINT32  TAF_APS_GetPdpIdFromTi(
    SM_TAF_TRANS_ID_STRU               *pstTi,
    VOS_UINT8                          *pucPdpId
);
VOS_VOID  TAF_APS_SetPdpEntityTi(
    VOS_UINT8                           ucPdpId,
    SM_TAF_TRANS_ID_STRU               *pstTis
);
VOS_UINT32 TAF_APS_GetCidFromLteBitCid(
    VOS_UINT32                          ulBitCid
);

VOS_UINT32  TAF_APS_GetActivedCid(
    VOS_UINT8                           *pucActivedCid
);

VOS_UINT32 TAF_APS_Ipv4AddrAtoi(
    VOS_CHAR                           *pcString,
    VOS_UINT8                          *pucNumber
);

VOS_UINT32 TAF_APS_ValidatePdpType(
    TAF_PDP_TYPE_ENUM_UINT8             enPdpTypeReq,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpTypeCnf
);

VOS_UINT32 TAF_APS_Ipv4AddrItoa(
    VOS_CHAR                           *pcString,
    VOS_UINT8                          *pucNumber
);

VOS_UINT8 TAF_APS_ConverUserDefPdpTypeToApsEntityPdpType (
    TAF_PDP_TYPE_ENUM_UINT8             enUserPdpType,
    VOS_UINT8                           ucDefaultPdpType
);

VOS_VOID TAF_APS_GetSpecPdpLinkedPdpInfo (
    TAF_APS_PDPID_LIST_STRU            *pstPdpIdList,
    VOS_UINT8                           ucNsapi
);

extern TAF_APS_USER_CONN_STATUS_ENUM_UINT8 TAF_APS_GetUserConnStatus(VOS_VOID);

/*****************************************************************************
 �� �� ��  : TAF_APS_MapSmCause
 ��������  : ��SM������ӳ���PS����д�����
 �������  : enSmCause - SM������
 �������  : ��
 �� �� ֵ  : TAF_PS_CAUSE_ENUM_UINT32
 ���ú���  :
 ��������  :
*****************************************************************************/
TAF_PS_CAUSE_ENUM_UINT32 TAF_APS_MapSmCause(
    SM_TAF_CAUSE_ENUM_UINT16            enSmCause
);


#if (FEATURE_ON == FEATURE_LTE)
/*****************************************************************************
 �� �� ��  : TAF_APS_MapL4aCause
 ��������  : ��L4A������ӳ���PS����д�����
 �������  : enL4aCause - L4A������
 �������  : ��
 �� �� ֵ  : TAF_PS_CAUSE_ENUM_UINT32
 ���ú���  :
 ��������  :
*****************************************************************************/
TAF_PS_CAUSE_ENUM_UINT32 TAF_APS_MapL4aCause(
    VOS_UINT32                          ulL4aCause
);
#endif

/*****************************************************************************
 �� �� ��  : TAF_APS_ConvertPdpType
 ��������  : ת��PDP����(APS--->TAF)
 �������  : ucPdpTypeNum - �ڲ�PDP����
 �������  : ��
 �� �� ֵ  : TAF_PDP_TYPE_ENUM_UINT8
 ���ú���  :
 ��������  :
*****************************************************************************/
TAF_PDP_TYPE_ENUM_UINT8 TAF_APS_ConvertPdpType(VOS_UINT8 ucPdpTypeNum);

/*****************************************************************************
 �� �� ��  : TAF_APS_GetCtrlHdrFromCmdBufferQueue
 ��������  : �ӻ�����Ϣ�����л�ȡ��Ϣͷ
 �������  : AF_APS_TIMER_ID_ENUM_UINT32        enTimer
 �������  : TAF_CTRL_STRU                      *pstCtrl
 �� �� ֵ  : VOS_UINT32

*****************************************************************************/
VOS_UINT32 TAF_APS_GetCtrlHdrFromCmdBufferQueue(
    TAF_CTRL_STRU                      *pstCtrl,
    TAF_APS_TIMER_ID_ENUM_UINT32        enTimer,
    VOS_UINT32                          ulPara
);

#if (FEATURE_ON == FEATURE_IMS)
/*****************************************************************************
 �� �� ��  : TAF_APS_IsImsBearer
 ��������  : �ж��Ƿ�ΪIMSר�г���
 �������  : ulLinkedEpsbId - ������IMSר�г���ID
 �������  : ��
 �� �� ֵ  : VOS_VOID

*****************************************************************************/
VOS_UINT32  TAF_APS_IsImsBearer(
    VOS_UINT8                           ucLinkedCid,
    VOS_UINT32                          ulLinkedEpsbId
);
#endif

#if (FEATURE_ON == FEATURE_PTM)
VOS_VOID TAF_APS_PsCallFailErrRecord(TAF_PS_CAUSE_ENUM_UINT32 enCause);
#endif


/*****************************************************************************
 �� �� ��  : TAF_APS_SetPdpEntModuleId
 ��������  : ����PDP IDʵ������Ӧ��ulModuleIdֵ
 �������  : ucPdpId        :PDP ID��ֵ
             ulModuleId     :ulModuleId��ֵ
 �������  : ��
 �� �� ֵ  : ��
*****************************************************************************/
VOS_VOID TAF_APS_SetPdpEntModuleId (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid,
    VOS_UINT32                          ulModuleId
);

VOS_VOID TAF_APS_SetPdpEntCidValue (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid
);

VOS_VOID TAF_APS_RmvPdpEntClientInfo (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid,
    TAF_APS_BITCID_INFO_STRU           *pstBitCid
);

VOS_VOID   TAF_APS_SetPdpEntClientInfo(
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid,
    VOS_UINT32                          ulModuleId,
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId
);

VOS_VOID   TAF_APS_AddPdpEntClientInfo(
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid,
    CONST TAF_CTRL_STRU                *pstCtrl
);

VOS_VOID TAF_APS_SetPdpEntBitCidMask (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid
);
VOS_VOID TAF_APS_RmvPdpEntBitCidMask (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid
);

VOS_UINT32 TAF_APS_IsPdpEntBitCidMaskExit (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid
);

#if (FEATURE_ON == FEATURE_IMS)
VOS_UINT32 TAF_APS_CheckImsBearerByCid (
    VOS_UINT8                           ucCid
);

VOS_UINT32 TAF_APS_CheckImsBearerByRabId (
    VOS_UINT32                          ulRabId
);

VOS_VOID TAF_APS_GetImsBearerCid (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                          *pucCid
);

#endif

VOS_VOID TAF_APS_GetPdpEntBitCidInfo(
    VOS_UINT8                           ucPdpId,
    TAF_APS_BITCID_INFO_STRU                    *pstCid
);

VOS_VOID TAF_APS_SetPdpEntAttachBearerFlag (
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucFlag
);

VOS_UINT8 TAF_APS_GetPdpEntAttachBearerFlag (
    VOS_UINT8                           ucPdpId
);

VOS_VOID   TAF_APS_SetPsCallEvtCtrl(
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid,
    TAF_CTRL_STRU                      *pstCtrl
);

VOS_INT32 TAF_APS_CompareApn(
    const VOS_UINT8                 aucBearerApn[],
    VOS_UINT8                       ucBearerApnLen,
    const VOS_UINT8                 aucUserApn[],
    VOS_UINT8                       ucUserApnLen
);

VOS_VOID TAF_APS_SetPdpEntDialPdpType(
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
);

VOS_UINT8 TAF_APS_GetPdpEntDialPdpType(
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid
);

/*****************************************************************************
 �� �� ��  : TAF_APS_MatchCallWithAllBearer
 ��������  : ��鵱ǰ�Ƿ��г���ƥ���û����Ų����ĳ���
 �������  : pstCtrl      --- ������Ϣ
             pstDialParam --- ���Ų���
 �������  : pucPdpId     --- ʵ������
 �� �� ֵ  : VOS_TRUE/VOS_FALSE
*****************************************************************************/
VOS_UINT32 TAF_APS_MatchCallWithAllBearer(
    CONST TAF_CTRL_STRU                *pstCtrl,
    CONST TAF_APS_MATCH_PARAM_STRU     *pstMatchParam,
    VOS_UINT8                          *pucPdpId
);

VOS_UINT16   TAF_APS_GetPdpEntClientId(
    VOS_UINT8                           ucPdpId,
    VOS_UINT8                           ucCid
);

VOS_VOID TAF_APS_SetPdpEntModDialInfo (
    VOS_UINT8                           ucPdpId,
    TAF_APS_PDP_MOD_DIAL_STRU          *pstModDial
);

VOS_VOID TAF_APS_GetPdpEntModDialInfo (
    VOS_UINT8                           ucPdpId,
    TAF_APS_PDP_MOD_DIAL_STRU          *pstModDial
);


/*****************************************************************************
 �� �� ��  : TAF_APS_SetPdpEntAddrType
 ��������  : ����ʵ�������ĵĵ�ַ����
 �������  : ucPdpId   --- ʵ������(��Ч���ɵ����߱�֤)
             enPdpType --- PDP����
 �������  : ��
 �� �� ֵ  : VOS_VOID
*****************************************************************************/
VOS_VOID TAF_APS_SetPdpEntAddrType(
    VOS_UINT8                           ucPdpId,
    TAF_PDP_TYPE_ENUM_UINT8             enPdpType
);

#if (FEATURE_ON == FEATURE_LTE)
/*****************************************************************************
 �� �� ��  : TAF_APS_IsAttachBearerExist
 ��������  : ���ע������Ƿ����
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_TRUE/VOS_FALSE
*****************************************************************************/
VOS_UINT32 TAF_APS_IsAttachBearerExist(VOS_VOID);
#endif

/*****************************************************************************
 �� �� ��  : TAF_APS_IsAnyBearerExist
 ��������  : ����Ƿ��г��ش���
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_TRUE/VOS_FALSE
*****************************************************************************/
VOS_UINT32 TAF_APS_IsAnyBearerExist(VOS_VOID);

/*****************************************************************************
 �� �� ��  : TAF_APS_GetAttachAllowFlg
 ��������  : ��ȡע�������ʶ
 �������  : VOS_VOID
 �������  : ��
 �� �� ֵ  : VOS_TRUE/VOS_FALSE
*****************************************************************************/
VOS_UINT8 TAF_APS_GetAttachAllowFlg(VOS_VOID);

/*****************************************************************************
 �� �� ��  : TAF_APS_GenMatchParamByCidInfo
 ��������  : ���ɳ���ƥ�����
 �������  : pstPdpTblInfo --- CID����Ϣ
 �������  : pstMatchParam --- ƥ�����
 �� �� ֵ  : VOS_VOID
*****************************************************************************/
VOS_VOID TAF_APS_GenMatchParamByCidInfo(
    CONST TAF_PDP_TABLE_STRU           *pstPdpTblInfo,
    TAF_APS_MATCH_PARAM_STRU           *pstMatchParam
);

/*****************************************************************************
 �� �� ��  : TAF_APS_GenMatchParamByDailInfo
 ��������  : ���ɳ���ƥ�����
 �������  : pstDialParaInfo --- ���Ų���
 �������  : pstMatchParam   --- ƥ�����
 �� �� ֵ  : VOS_VOID
*****************************************************************************/
VOS_VOID TAF_APS_GenMatchParamByDailInfo(
    CONST TAF_PS_DIAL_PARA_STRU        *pstDialParaInfo,
    TAF_APS_MATCH_PARAM_STRU           *pstMatchParam
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

#endif /* end of TafApsCtx.h */
