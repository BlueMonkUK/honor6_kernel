
#ifndef _NAS_MMC_PROC_NVIM_H
#define _NAS_MMC_PROC_NVIM_H

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "NasMmlCtx.h"
#include  "NVIM_Interface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define NAS_MMC_NV_ITEM_ACTIVE          (1)                                     /* NV��� */
#define NAS_MMC_NV_ITEM_DEACTIVE        (0)                                     /* NV��δ���� */

/* GPRS GEA �㷨֧�ֺ궨�� */
#define NAS_MMC_GPRS_GEA1_SUPPORT       (0x01)                                  /* ֧��GPRS GEA1�㷨 */
#define NAS_MMC_GPRS_GEA1_VALUE         (0x80)                                  /* ֧��GEA1ʱ����������ֵ */
#define NAS_MMC_GPRS_GEA2_VALUE         (0x40)                                  /* ֧��GEA2ʱ����������ֵ */
#define NAS_MMC_GPRS_GEA3_VALUE         (0x20)                                  /* ֧��GEA3ʱ����������ֵ */
#define NAS_MMC_GPRS_GEA4_VALUE         (0x10)                                  /* ֧��GEA4ʱ����������ֵ */
#define NAS_MMC_GPRS_GEA5_VALUE         (0x08)                                  /* ֧��GEA5ʱ����������ֵ */
#define NAS_MMC_GPRS_GEA6_VALUE         (0x04)                                  /* ֧��GEA6ʱ����������ֵ */
#define NAS_MMC_GPRS_GEA7_VALUE         (0x02)                                  /* ֧��GEA7ʱ����������ֵ */

#define NAS_MMC_GPRS_GEA2_SUPPORT       (0x02)                                  /* ֧��GPRS GEA2�㷨 */
#define NAS_MMC_GPRS_GEA3_SUPPORT       (0x04)                                  /* ֧��GPRS GEA3�㷨 */
#define NAS_MMC_GPRS_GEA4_SUPPORT       (0x08)                                  /* ֧��GPRS GEA4�㷨 */
#define NAS_MMC_GPRS_GEA5_SUPPORT       (0x10)                                  /* ֧��GPRS GEA5�㷨 */
#define NAS_MMC_GPRS_GEA6_SUPPORT       (0x20)                                  /* ֧��GPRS GEA6�㷨 */
#define NAS_MMC_GPRS_GEA7_SUPPORT       (0x40)                                  /* ֧��GPRS GEA7�㷨 */
#define NAS_MMC_NVIM_MAX_EPLMN_NUM      (16)                                    /* en_NV_Item_EquivalentPlmn NV�е�Чplmn���� */
#define NAS_MMC_NVIM_MAX_MCC_SIZE       (3)                                     /* plmn��Mcc��󳤶� */
#define NAS_MMC_NVIM_MAX_MNC_SIZE       (3)                                     /* plmn��Mnc��󳤶� */
#define NAS_MMC_LOW_BYTE_MASK           (0x0f)

#define NAS_MMC_MAX_USER_CFG_IMSI_PLMN_NUM                  (6)                 /* �û����õ�����֧�ֵ�USIM/SIM���ĸ��� */
#define NAS_MMC_MAX_USER_CFG_EHPLMN_NUM                     (6)                 /* �û����õ�EHplmn�ĸ��� */
#define NAS_MMC_MAX_BLACK_LOCK_PLMN_WITH_RAT_NUM            (8)                 /* ��ֹ���뼼����PLMN ID�������� */

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
VOS_VOID NAS_MMC_ReadImeisvNvim( VOS_VOID );
VOS_VOID NAS_MMC_ReadChangeRegRejCauFlgNvim( VOS_VOID );

NAS_MML_CHANGE_REG_REJ_CAUSE_TYPE_ENUM_UINT8 NAS_MMC_ConvertNvimChangeRegRejTypeToMmlType( 
    NAS_NVIM_CHANGE_REG_REJ_CAUSE_TYPE_ENUM_UINT8           enNvimType
);
VOS_VOID NAS_MMC_ReadRoamingRejectNoRetryFlgNvim( VOS_VOID );
VOS_VOID NAS_MMC_ReadSupported3GppReleaseNvim(VOS_VOID);
VOS_VOID NAS_MMC_ReadClassMark1Nvim(VOS_VOID);
VOS_VOID NAS_MMC_ReadClassMark2Nvim(VOS_VOID);
VOS_VOID NAS_MMC_ReadNetworkCapabilityNvim(VOS_VOID);
VOS_VOID  NAS_MMC_ReadMsModeNvim( VOS_VOID );

VOS_VOID NAS_MMC_UpdateNetworkCapabilityGeaValue(VOS_VOID);

VOS_VOID NAS_MMC_ReadUseSingleRplmnFlagNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadRplmnWithRatNvim(VOS_VOID);

VOS_VOID NAS_MMC_WriteRplmnWithRatNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadEHPlmnSupportNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadUserSetBandNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadSearchHPlmnFlgAutoModeNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadSearchHPlmnFlgManualModeNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadSearchHplmnFlgPowerOnNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadEnhancedHPlmnSearchFlagNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadSpecialRoamFlgNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadNetworkNameNvim(VOS_VOID);

VOS_VOID NAS_MMC_WriteEplmnNvim(VOS_VOID);


VOS_VOID NAS_MMC_DeleteEPlmnList( VOS_VOID );


VOS_VOID NAS_MMC_ReadHplmnFirstSearchPeriodNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadDefaultHplmnSrchPeriodNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadSrchHplmnTtimerValueNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadWhiteOpeLockPlmnInfoNvim( VOS_VOID );

VOS_VOID NAS_MMC_ReadBlackOperLockPlmnInfoNvim( VOS_VOID );

VOS_VOID NAS_MMC_ReadUserCfgEhplmnInfoNvim( VOS_VOID );
VOS_VOID NAS_MMC_ReadDisabledRatPlmnInfoNvim( VOS_VOID );



VOS_UINT8 NAS_MMC_IsRatForbiddenListNvimValid( 
    NAS_MMC_NVIM_RAT_FORBIDDEN_LIST_STRU *pstParaValue
);

VOS_VOID NAS_MMC_ReadRatForbiddenListConfigNvim( VOS_VOID );

VOS_UINT32 NAS_MMC_ConvertNvimRatToMmlType( 
    NAS_MMC_NVIM_PLATFORM_SUPPORT_RAT_ENUM_UINT8            enNvRatType,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8                        *penMmlRatType
);



VOS_VOID NAS_MMC_ReadAvailableTimerInfoNvim( VOS_VOID  );

VOS_VOID NAS_MMC_ReadStandardSTKSteeringOfRoamingFlgNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadCsRejSearchSupportFlagNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadAutoAttachNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadPlmnSelectionModeNvim(VOS_VOID);

VOS_VOID NAS_MMC_WritePlmnSelectionModeNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadEquivalentPlmnNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadMaxForbRoamLaNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadGsmForbidAccessInfoNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadRoamBrokerRegisterFailCntNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadPsLociNvim(VOS_VOID);

VOS_VOID NAS_MMC_WritePsLociNvim(
    VOS_UINT8                          *pstPsLocInfo,
    VOS_UINT32                          ulSize
);
VOS_VOID NAS_MMC_ReadRoamCfgInfoNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadDtSingleDomainPlmnSearchNvim(VOS_VOID);

VOS_VOID  NAS_MMC_ReadLastImsiNvim( VOS_VOID );

VOS_VOID NAS_MMC_UpdateLastImsi( VOS_VOID );

#if   (FEATURE_ON == FEATURE_LTE)

VOS_VOID NAS_MMC_ReadTinInfoNvim(VOS_VOID);

VOS_VOID  NAS_MMC_WriteTinInfoNvim(
    NAS_MML_TIN_TYPE_ENUM_UINT8         enTinType,
    VOS_UINT8                          *pucImsi
);

VOS_VOID NAS_MMC_ReadLteCsServiceConfigNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadHoWaitSysinfoTimeLenNvim(VOS_VOID);

VOS_VOID  NAS_MMC_ReadLNasReleaseNvim(VOS_VOID);

#endif
VOS_VOID NAS_MMC_ReadCsfbEmgCallLaiChgLauFirstNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadLteRoamConfigNvim(VOS_VOID);
VOS_VOID NAS_MMC_ReadDailRejectConfigNvim(VOS_VOID);
VOS_VOID NAS_MMC_ReaducWcdmaPriorityGsmNvim(VOS_VOID);
VOS_VOID NAS_MMC_ReadPsOnlyCsServiceSupportNvim(VOS_VOID);
VOS_VOID NAS_MMC_ReadAPPConfigSupportNvim(VOS_VOID);


VOS_VOID NAS_MMC_ReadAisRoamingNvim( VOS_VOID  );



VOS_VOID NAS_MMC_ReadUserAutoReselCfgNvim(VOS_VOID);


VOS_VOID NAS_MMC_ReadScanCtrlNvim(VOS_VOID);


VOS_VOID NAS_MMC_ReadPrioHplmnActCfgNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadHPlmnSearchRegardLessMccNvim(VOS_VOID);
VOS_VOID NAS_MMC_ReadSingleDomainFailActionListNvim(VOS_VOID);
VOS_VOID NAS_MMC_ReadRegFailNetWorkFailureCustomFlagNvim(VOS_VOID);
VOS_VOID NAS_MMC_ReadActingHPlmnSupportNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadPlmnExactlyCompareNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadCustomizeServiceNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadHplmnRegisterCtrlNvim(VOS_VOID);
VOS_VOID NAS_MMC_ReadCellSignReportCfgNvim( VOS_VOID );

VOS_VOID NAS_MMC_WriteCellSignReportCfgNvim(VOS_UINT8 ucSignThreshold, VOS_UINT8 ucMinRptTimerInterval);


VOS_VOID NAS_MMC_ReadPlatformRatCapNvim( VOS_VOID );
VOS_UINT32 NAS_MMC_IsPlatformRatCapNvimValid(
    PLATAFORM_RAT_CAPABILITY_STRU              *pstNvPlatformRatCap
);
VOS_UINT32 NAS_MMC_ConvertNvimTypePlatformRatCapToNasType(
    PLATFORM_RAT_TYPE_ENUM_UINT16                       enNvPlatformRatCap,
    NAS_MML_PLATFORM_RAT_TYPE_ENUM_UINT8               *penNasPlatformRatCap
);

VOS_VOID NAS_MMC_ReadUcs2CustomizationNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadCloseSmsCapabilityConfigNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadH3gCtrlNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadSvlteSupportFlagNvim( VOS_VOID );

#if  (FEATURE_ON == FEATURE_LTE)
VOS_VOID NAS_MMC_ReadEnableLteTimerLenNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadCsfbRauFollowOnFlgNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadLteCustomMccInfoNvim(VOS_VOID);

#endif
VOS_VOID  NAS_MMC_ReadIsrSupportNvim( VOS_VOID );

#if (FEATURE_ON == FEATURE_PTM)
VOS_VOID NAS_MMC_ReadErrlogCtrlInfoNvim(VOS_VOID);
#endif


VOS_VOID NAS_MMC_ReadDsdaPlmnSearchEnhancedCfgNvim(VOS_VOID);


VOS_VOID NAS_MMC_ReadUserCfgOPlmnInfoNvim(VOS_VOID);
VOS_VOID NAS_MMC_ReadAccBarPlmnSearchNvim(VOS_VOID);

#if (FEATURE_ON == FEATURE_DSDS)
VOS_VOID NAS_MMC_ReadModemRfShareCfgNvim(VOS_VOID);

#endif


VOS_VOID NAS_MMC_ReadWgRfMainBandNvim( VOS_VOID );

VOS_VOID NAS_MMC_ReadImsVoiceMMEnableNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadLcCtrlParaNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadLteDisabledUseLteInfoFlagNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadCsOnlyDataServiceSupportNvim( VOS_VOID );

VOS_VOID NAS_MMC_ReadIgnoreAuthRejFlgNvim(VOS_VOID);
VOS_VOID NAS_MMC_ReadHighPrioRatHplmnTimerInfoNvim( VOS_VOID  );
VOS_VOID NAS_MMC_ReadSBMCustomDualIMSIConfigNvim(VOS_VOID);
VOS_VOID NAS_MMC_ReadRPLMNConfigNvim(VOS_VOID);
#if  (FEATURE_ON == FEATURE_LTE)
VOS_VOID NAS_MMC_ReadUltraFlashCsfbSupportFlgNvim(VOS_VOID);
VOS_VOID NAS_MMC_ReadSrvccSupportFlgNvim(VOS_VOID);
#endif

VOS_VOID NAS_MMC_Read3GPP2UplmnNotPrefFlgNvim(VOS_VOID);

VOS_VOID NAS_MMC_ReadSyscfgTriPlmnSrchCfgNvim(VOS_VOID);

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
