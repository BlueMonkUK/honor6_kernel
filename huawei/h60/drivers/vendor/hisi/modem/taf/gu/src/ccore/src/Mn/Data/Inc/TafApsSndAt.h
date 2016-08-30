

#ifndef __TAF_APS_SNDAT_H__
#define __TAF_APS_SNDAT_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "Ps.h"
#include "AtMnInterface.h"
#include "TafApsApi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/


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

VOS_UINT32  TAF_APS_SndClearDsFlowCnf(
    TAF_CTRL_STRU                    *pstCtrl,
    VOS_UINT32                          ulErrCode
);

VOS_UINT32  TAF_APS_SndDsFlowRptInd(
    TAF_CTRL_STRU                    *pstCtrl,
    VOS_UINT32                          ulErrCode,
    TAF_DSFLOW_REPORT_STRU             *pstDsFlowRptInfo
);

VOS_UINT32  TAF_APS_SndGetDsFlowInfoCnf(
    TAF_CTRL_STRU                    *pstCtrl,
    VOS_UINT32                          ulErrCode,
    TAF_DSFLOW_QUERY_INFO_STRU         *pstDsFlowQryInfo
);

VOS_UINT32  TAF_APS_SndConfigDsFlowRptCnf(
    TAF_CTRL_STRU                    *pstCtrl,
    VOS_UINT32                          ulErrCode
);
VOS_UINT32 TAF_APS_SndSetPdpCtxStateCnf(
    TAF_CTRL_STRU                    *pstCtrl,
    TAF_PS_CAUSE_ENUM_UINT32            enCause
);
VOS_UINT32  TAF_APS_SndSetPrimPdpCtxInfoCnf(
    TAF_CTRL_STRU                    *pstCtrl,
    VOS_UINT32                          ulErrCode
);

VOS_UINT32  TAF_APS_SndSetSecPdpCtxInfoCnf(
    TAF_CTRL_STRU                    *pstCtrl,
    VOS_UINT32                          ulErrCode
);

VOS_UINT32  TAF_APS_SndSetTftInfoCnf(
    TAF_CTRL_STRU                    *pstCtrl,
    VOS_UINT32                          ulErrCode
);

VOS_UINT32  TAF_APS_SndSetAnsModeInfoCnf(
    TAF_CTRL_STRU                    *pstCtrl,
    VOS_UINT32                          ulErrCode
);

VOS_UINT32  TAF_APS_SndSetCgeqosCnf(
    TAF_CTRL_STRU                    *pstCtrl,
    VOS_UINT32                          ulErrCode
);

VOS_UINT32  TAF_APS_SndSetAuthDataInfoCnf(
    TAF_CTRL_STRU                    *pstCtrl,
    VOS_UINT32                          ulErrCode
);

VOS_UINT32  TAF_APS_SndGetDynamicPrimPdpCtxInfoCnf(
    TAF_CTRL_STRU                    *pstCtrl,
    VOS_UINT32                          ulErrCode,
    VOS_UINT32                          ulCidNum
);

VOS_UINT32  TAF_APS_SndGetDynamicSecPdpCtxInfoCnf(
    TAF_CTRL_STRU                    *pstCtrl,
    VOS_UINT32                          ulErrCode,
    VOS_UINT32                          ulCidNum
);

VOS_UINT32  TAF_APS_SndGetDynamicTftInfoCnf(
    TAF_CTRL_STRU                    *pstCtrl,
    VOS_UINT32                          ulErrCode,
    VOS_UINT32                          ulCidNum
);

VOS_UINT32  TAF_APS_SndGetCgeqosrdpCnf(
    TAF_CTRL_STRU                    *pstCtrl,
    VOS_UINT32                          ulErrCode
);

VOS_UINT32  TAF_APS_SndGetPdpIpAddrInfoCnf(
    VOS_VOID                           *pData,
    VOS_UINT32                          ulLength
);

VOS_UINT32  TAF_APS_SndGetUmtsQosInfoCnf(
    VOS_VOID                           *pData,
    VOS_UINT32                          ulLength
);

VOS_UINT32  TAF_APS_SndGetUmtsQosMinInfoCnf(
    VOS_VOID                           *pData,
    VOS_UINT32                          ulLength
);

VOS_UINT32  TAF_APS_SndGetDynamicUmtsQosInfoCnf(
    VOS_VOID                           *pData,
    VOS_UINT32                          ulLength
);

VOS_UINT32  TAF_APS_SndGetPdpAuthInfoCnf(
    VOS_VOID                           *pData,
    VOS_UINT32                          ulLength
);

VOS_UINT32  TAF_APS_SndGetPdpDnsInfoCnf(
    VOS_VOID                           *pData,
    VOS_UINT32                          ulLength
);
#if(FEATURE_ON == FEATURE_LTE)
VOS_UINT32  TAF_APS_SndGetLtecsInfoCnf(
    TAF_CTRL_STRU                    *pstCtrl,
    VOS_UINT32                          ulErrCode
);
VOS_UINT32  TAF_APS_SndGetCemodeInfoCnf(
    TAF_CTRL_STRU                    *pstCtrl,
    VOS_UINT32                          ulErrCode
);
VOS_UINT32  TAF_APS_SndSetPdprofInfoCnf(
    TAF_CTRL_STRU                    *pstCtrl,
    VOS_UINT32                          ulErrCode
);
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

#endif /* end of TafApsSndAt.h */
