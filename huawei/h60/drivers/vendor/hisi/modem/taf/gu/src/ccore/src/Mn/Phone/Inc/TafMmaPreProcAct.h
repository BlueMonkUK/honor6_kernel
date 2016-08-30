
#ifndef _TAF_MMA_PREPROCACT_H_
#define _TAF_MMA_PREPROCACT_H_

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "vos.h"
#include "UsimPsInterface.h"
#include "TafAppMma.h"
#include "TafSdcCtx.h"
#include "MmaMmcInterface.h"

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
#ifdef __PS_WIN32_RECUR__
VOS_UINT32 NAS_MMA_RestoreContextData(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 NAS_MMA_RestoreFixedContextData(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);
#endif

VOS_UINT32 TAF_MMA_RcvOmPhoneModeSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvPhoneModeQuery_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvPihUsimStatusInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvAtSyscfgHandle_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvMmaInterUsimStatusChangeInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_ProcOmPhoneModeSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_ProcPhoneModeSet_PreProc(
    TAF_PH_MODE                         ucCurrPhMode,
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId
);
VOS_UINT32 TAF_MMA_ProcTafPhoneModeSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_SetMiniMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
);

VOS_UINT32 TAF_MMA_SetResetMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
);

VOS_UINT32 TAF_MMA_SetPowerOffMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
);

VOS_UINT32 TAF_MMA_SetFullMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
);

VOS_UINT32 TAF_MMA_RcvAtParaReadReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);


VOS_UINT32 TAF_MMA_RcvMmcNetworkCapabilityInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvPihUsimRefreshIndMsgProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvMmaSimLockStatusChangeInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

/*************************************Ԥ�����й���������*****************************/
VOS_VOID TAF_MMA_ReadUsimInfo_PreProc(VOS_VOID);
VOS_VOID TAF_MMA_SndAtIccStatusInd_PreProc(
    VOS_UINT32                          ulUsimStaChg
);
VOS_VOID TAF_MMA_SaveInterUsimStatusChangeIndMsgInCacheQueue_PreProc(VOS_VOID);
VOS_UINT32 TAF_MMA_ProcPihUsimRefreshInd3gSessionReset_PreProc(VOS_VOID);
VOS_UINT32 TAF_MMA_ProcUsimStatusIndInPhoneModeProc_PreProc(
    VOS_UINT32                          ulUsimStaChg,
    VOS_UINT32                          ulImsiChg
);

VOS_UINT32 TAF_MMA_RcvMmcCampOnInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvMmcRegResultInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);


TAF_PH_RAT_TYPE_ENUM_UINT8 TAF_MMA_ConvertSdcRatToAppRatType( 
                        TAF_SDC_SYS_MODE_ENUM_UINT8 enSdcRatType
                        );

TAF_PH_SERVICE_DOMAIN TAF_MMA_ConvertMmcDomainToMmaDomainType( 
                        MMA_MMC_SRVDOMAIN_ENUM_UINT32 enMmcDomainType
                        );

#if (FEATURE_ON == FEATURE_IMS)
VOS_UINT32 TAF_MMA_RcvImsaImsVoiceCapNtf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvTiWaitImsaImsVoiceCapNtfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

#endif

VOS_UINT32 TAF_MMA_RcvTiWaitMmcAcqCnfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvTiWaitMmcRegCnfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvTiWaitMmcPowerSaveExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);



VOS_UINT32 TAF_MMA_RcvMmcUsimAuthFailInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvMmcCsServiceConnStatusInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);


VOS_UINT32 TAF_MMA_RcvMmcServRejInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);
VOS_UINT32 TAF_MMA_RcvMmcDetachInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);


VOS_UINT32 TAF_MMA_RcvMmcNetScanCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvMmcAbortNetScanCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);


#if (FEATURE_MULTI_MODEM == FEATURE_ON)

VOS_UINT32 TAF_MMA_RcvMtcOtherModemInfoNotify_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);


VOS_UINT32 TAF_MMA_RcvMtcNcellInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvMtcPsTransferInd_PreProc(
     VOS_UINT32                         ulEventType,
    struct MsgCB                       *pstMsg
);


#endif

VOS_UINT32 TAF_MMA_RcvMmcEplmnInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsgs
);




VOS_UINT32 TAF_MMA_RcvMmaAcqReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvMmaRegReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_ConvertAttachDomainToAttachType(
    TAF_PH_SERVICE_DOMAIN               ucAttachDomain
);

VOS_UINT32 TAF_MMA_RcvMmaPowerSaveReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvMmcAcqCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

#if (FEATURE_ON == FEATURE_CL_INTERWORK)
VOS_UINT32 TAF_MMA_RcvMmcAcqInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);
#endif

VOS_UINT32 TAF_MMA_RcvMmcRegCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_UINT32 TAF_MMA_RcvMmcPowerSaveCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
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

#endif /* end of TafMmaPreProcAct */

