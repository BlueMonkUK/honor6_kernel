

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "PsLib.h"
#include "pslog.h"
#include "PsCommonDef.h"
#include "TafSdcCtx.h"
#include "TafLog.h"

#include "TafSdcLib.h"

#include "TafNvInterface.h"
#include  "NasNvInterface.h"
#if (FEATURE_IMS == FEATURE_ON)
#include  "ImsaNvInterface.h"
#endif

#include  "LNvCommon.h"


#include "NasErrorLog.h"
#include  "LNasNvInterface.h"
#ifdef  __cplusplus
  #if  __cplusplus
  extern "C"{
  #endif
#endif


/*****************************************************************************
  2 ��������
*****************************************************************************/

#define THIS_FILE_ID PS_FILE_ID_TAFSDC_CTX_C

/*****************************************************************************
  3 ���Ͷ���
*****************************************************************************/

/*****************************************************************************
  4 ��������
*****************************************************************************/

/*****************************************************************************
  5 ��������
*****************************************************************************/
/* ����TAF�㹲�õ�CTX */
TAF_SDC_CTX_STRU                        g_stTafSdcCtx;

/*Ĭ�������ʹ�õ�GSM 7bit to ASCII��ת����*/
LOCAL VOS_UINT8 g_aucTafSdcDef7BitToAsciiTbl[] =
{
 '@', 0xA3, '$', 0xA5, 0xE8, 0xE9, 0xF9, 0xEC, 0xF2, 0xC7,0x0A, 0xD8, 0xF8,0x0D, 0xC5, 0xE5,
 '?', '_', '?', '?', '?', '?', '?', '?', '?', '?', '?',' ', 0xC6, 0xE6, 0xDF, 0xC9,
 ' ', '!',0x22, '#', 0x2A, '%', '&',0x27, '(', ')', '*', '+', ',', '-', '.', '/',
 '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
 0xA1, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 0xC4, 0xD6, 0xD1, 0xDC, 0xA7,
 0xBF, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 0xE4, 0xF6, 0xF1, 0xFC, 0xE0
};


/*TME����ʹ�õ�GSM 7bit to ASCII��ת����*/
LOCAL VOS_UINT8 g_aucTafSdcTme7BitToAsciiTbl[] =
{
 '@', 0x9C, '$', 0x9D, 0x8A, 0x82, 0x97, 0x8D, 0x95, 0x80,0x0A, '?', '?',0x0D, 0x8F, 0x86,
 '?', '_', 0xE8, 0xE2, '?', 0xEA, 0xE3, '?',0xE4, 0xE9, '?','?', 0x92, 0x91, 0xE1, 0x90,
 ' ', '!',0x22, '#', '?', '%', '&',0x27, '(', ')', '*', '+', ',', '-', '.', '/',
 '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
 0xAD, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 0x8E, 0x99, 0xA5, 0x9A, '?',
 0xA8, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 0x84, 0x94, 0xA4, 0x81, 0x85
};


LOCAL VOS_CHAR *g_aucTafSdcCallEmerNumsNoUsim[TAF_SDC_NUMOF_EMER_NUMS_NO_USIM] =
{"112", "911", "000", "08", "110", "118", "119", "999"};

LOCAL VOS_CHAR *g_aucTafSdcCallEmerNumsWithUsim[TAF_SDC_NUMOF_EMER_NUMS_WITH_USIM] =
{"112", "911"};

/*****************************************************************************
  6 ��������
*****************************************************************************/

TAF_SDC_CTX_STRU*  TAF_SDC_GetSdcCtx( VOS_VOID )
{
    return &(g_stTafSdcCtx);
}


TAF_SDC_MS_CAP_INFO_STRU*  TAF_SDC_GetMsCapInfo( VOS_VOID )
{
    return &(TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo);
}


VOS_UINT8 TAF_SDC_GetRoamFlag( VOS_VOID )
{
    return (TAF_SDC_GetSdcCtx()->stNetworkInfo.stCampPlmnInfo.ucRoamFlag);
}


VOS_VOID TAF_SDC_SetRoamFlag(VOS_UINT8 ucRoamFlag)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stCampPlmnInfo.ucRoamFlag = ucRoamFlag;
    return;
}


VOS_UINT8 TAF_SDC_GetCampOnFlag( VOS_VOID )
{
    return (TAF_SDC_GetSdcCtx()->stNetworkInfo.stCampPlmnInfo.ucCampOnFlg);
}


VOS_VOID TAF_SDC_SetCampOnFlag(VOS_UINT8 ucCampOnFlg)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stCampPlmnInfo.ucCampOnFlg = ucCampOnFlg;
    return;
}




TAF_SDC_SYS_MODE_ENUM_UINT8 TAF_SDC_GetSysMode( VOS_VOID )
{
    return (TAF_SDC_GetSdcCtx()->stNetworkInfo.stCampPlmnInfo.enSysMode);
}


VOS_VOID TAF_SDC_SetSysMode(TAF_SDC_SYS_MODE_ENUM_UINT8 enSysMode)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stCampPlmnInfo.enSysMode = enSysMode;
    return;
}


TAF_SDC_SYS_SUBMODE_ENUM_UINT8 TAF_SDC_GetSysSubMode( VOS_VOID )
{
    return (TAF_SDC_GetSdcCtx()->stNetworkInfo.stCampPlmnInfo.enSysSubMode);
}

VOS_VOID TAF_SDC_SetSysSubMode(TAF_SDC_SYS_SUBMODE_ENUM_UINT8 enSysSubMode)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stCampPlmnInfo.enSysSubMode = enSysSubMode;
    return;
}


TAF_SDC_CAMP_PLMN_INFO_STRU* TAF_SDC_GetCampPlmnInfo( VOS_VOID )
{
    return &(TAF_SDC_GetSdcCtx()->stNetworkInfo.stCampPlmnInfo);
}


VOS_VOID TAF_SDC_SetCurrCampPlmnId(TAF_SDC_PLMN_ID_STRU *pstPlmnId)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stCampPlmnInfo.stPlmnId = *pstPlmnId;
    return;
}


TAF_SDC_PLMN_ID_STRU *TAF_SDC_GetCurrCampPlmnId(VOS_VOID)
{
    return &(TAF_SDC_GetSdcCtx()->stNetworkInfo.stCampPlmnInfo.stPlmnId);
}


VOS_UINT16 TAF_SDC_GetCurrCampLac(VOS_VOID)
{
    return (TAF_SDC_GetSdcCtx()->stNetworkInfo.stCampPlmnInfo.usLac);
}


VOS_VOID TAF_SDC_SetCurrCampLac(VOS_UINT16 usLac)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stCampPlmnInfo.usLac = usLac;
    return;
}


VOS_UINT8 TAF_SDC_GetCurrCampRac(VOS_VOID)
{
    return (TAF_SDC_GetSdcCtx()->stNetworkInfo.stCampPlmnInfo.ucRac);
}


VOS_VOID TAF_SDC_SetCurrCampRac(VOS_UINT8 ucRac)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stCampPlmnInfo.ucRac = ucRac;
    return;
}


VOS_UINT32 TAF_SDC_GetCurrCampCellId(VOS_VOID)
{
    return (TAF_SDC_GetSdcCtx()->stNetworkInfo.stCampPlmnInfo.ulCellId);
}


VOS_VOID TAF_SDC_SetCurrCampCellId(VOS_UINT32 ulCellId)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stCampPlmnInfo.ulCellId = ulCellId;
    return;
}


TAF_SDC_SERVICE_STATUS_ENUM_UINT8 TAF_SDC_GetCsServiceStatus( VOS_VOID )
{
    return (TAF_SDC_GetSdcCtx()->stNetworkInfo.stCsDomainInfo.enCsServiceStatus);
}


VOS_VOID TAF_SDC_SetCsServiceStatus(TAF_SDC_SERVICE_STATUS_ENUM_UINT8 enCsServiceStatus)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stCsDomainInfo.enCsServiceStatus = enCsServiceStatus;
    return;
}


TAF_SDC_SERVICE_STATUS_ENUM_UINT8 TAF_SDC_GetPsServiceStatus( VOS_VOID )
{
    return (TAF_SDC_GetSdcCtx()->stNetworkInfo.stPsDomainInfo.enPsServiceStatus);
}


VOS_VOID TAF_SDC_SetPsServiceStatus(TAF_SDC_SERVICE_STATUS_ENUM_UINT8 enPsServiceStatus)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stPsDomainInfo.enPsServiceStatus = enPsServiceStatus;
    return;
}



VOS_VOID TAF_SDC_SetPsAttachAllowFlg(VOS_UINT8 ucPsAttachAllowFlg)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stPsDomainInfo.ucPsAttachAllowFlg = ucPsAttachAllowFlg;
    return;
}


VOS_UINT8 TAF_SDC_GetPsAttachAllowFlg(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stNetworkInfo.stPsDomainInfo.ucPsAttachAllowFlg;
}




VOS_VOID TAF_SDC_SetServiceStatus(TAF_SDC_REPORT_SRVSTA_ENUM_UINT8 enReportSrvsta)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.enReportSrvsta = enReportSrvsta;
    return;
}


TAF_SDC_REPORT_SRVSTA_ENUM_UINT8 TAF_SDC_GetServiceStatus(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stNetworkInfo.enReportSrvsta;
}


TAF_SDC_SERVICE_DOMAIN_ENUM_UINT8 TAF_SDC_GetServiceDomain( VOS_VOID )
{
    return (TAF_SDC_GetSdcCtx()->stNetworkInfo.enServiceDomain);
}


VOS_VOID TAF_SDC_SetServiceDomain(TAF_SDC_SERVICE_DOMAIN_ENUM_UINT8 enServiceDomain)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.enServiceDomain = enServiceDomain;
    return;
}



TAF_SDC_REG_STATUS_ENUM_UINT8 TAF_SDC_GetCsRegStatus( VOS_VOID )
{
    return (TAF_SDC_GetSdcCtx()->stNetworkInfo.stCsDomainInfo.enCsRegStatus);
}


VOS_VOID TAF_SDC_SetCsRegStatus(TAF_SDC_REG_STATUS_ENUM_UINT8 enCsRegStatus)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stCsDomainInfo.enCsRegStatus = enCsRegStatus;
    return;
}



TAF_SDC_REG_STATUS_ENUM_UINT8 TAF_SDC_GetPsRegStatus( VOS_VOID )
{
    return (TAF_SDC_GetSdcCtx()->stNetworkInfo.stPsDomainInfo.enPsRegStatus);
}



VOS_VOID TAF_SDC_SetPsRegStatus(TAF_SDC_REG_STATUS_ENUM_UINT8 enPsRegStatus)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stPsDomainInfo.enPsRegStatus = enPsRegStatus;
    return;
}


VOS_UINT8 TAF_SDC_GetSimCsRegStatus( VOS_VOID )
{
    return (TAF_SDC_GetSdcCtx()->stNetworkInfo.stCsDomainInfo.ucSimCsRegStatus);
}


VOS_VOID TAF_SDC_SetSimCsRegStatus(
    VOS_UINT8       ucSimCsRegStatus
)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stCsDomainInfo.ucSimCsRegStatus = ucSimCsRegStatus;
    return;
}



VOS_UINT8 TAF_SDC_GetSimPsRegStatus( VOS_VOID )
{
    return (TAF_SDC_GetSdcCtx()->stNetworkInfo.stPsDomainInfo.ucSimPsRegStatus);
}



VOS_VOID TAF_SDC_SetSimPsRegStatus(
    VOS_UINT8       ucSimPsRegStatus
)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stPsDomainInfo.ucSimPsRegStatus= ucSimPsRegStatus;
    return;
}



VOS_VOID TAF_SDC_SetCsAcRestriction(TAF_SDC_ACCESS_RESTRICTION_STRU *pstCsAcRetriction)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stCsDomainInfo.stCsAcRestriction = *pstCsAcRetriction;
    return;
}


TAF_SDC_ACCESS_RESTRICTION_STRU *TAF_SDC_GetCsAcRestriction(VOS_VOID)
{
    return (&(TAF_SDC_GetSdcCtx()->stNetworkInfo.stCsDomainInfo.stCsAcRestriction));
}


TAF_SDC_ACCESS_RESTRICTION_STRU *TAF_SDC_GetPsAcRestriction(VOS_VOID)
{
    return (&(TAF_SDC_GetSdcCtx()->stNetworkInfo.stPsDomainInfo.stPsAcRestriction));
}



VOS_VOID TAF_SDC_SetPsAcRestriction(TAF_SDC_ACCESS_RESTRICTION_STRU *pstPsAcRetriction)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stPsDomainInfo.stPsAcRestriction = *pstPsAcRetriction;
    return;
}


VOS_UINT8* TAF_SDC_GetSimImsi( VOS_VOID )
{
    return (TAF_SDC_GetSdcCtx()->stSimInfo.stMsIdentity.aucImsi);
}


VOS_VOID TAF_SDC_SetSimImsi( VOS_UINT8 *pstImsi )
{
    PS_MEM_CPY(TAF_SDC_GetSdcCtx()->stSimInfo.stMsIdentity.aucImsi, pstImsi, sizeof(g_stTafSdcCtx.stSimInfo.stMsIdentity.aucImsi));
    return;
}


VOS_UINT8* TAF_SDC_GetLastSimImsi( VOS_VOID )
{
    return (TAF_SDC_GetSdcCtx()->stSimInfo.stMsIdentity.aucLastImsi);
}


VOS_VOID TAF_SDC_SetLastSimImsi( VOS_UINT8 *pstImsi )
{
    PS_MEM_CPY(TAF_SDC_GetSdcCtx()->stSimInfo.stMsIdentity.aucLastImsi, pstImsi, sizeof(g_stTafSdcCtx.stSimInfo.stMsIdentity.aucLastImsi));
    return;
}


TAF_SDC_USIM_STATUS_ENUM_UINT8 TAF_SDC_GetSimStatus( VOS_VOID )
{
    return (TAF_SDC_GetSdcCtx()->stSimInfo.stSimStatus.enSimStatus);
}


VOS_VOID TAF_SDC_SetSimStatus(TAF_SDC_USIM_STATUS_ENUM_UINT8 enSimStatus)
{
    TAF_SDC_GetSdcCtx()->stSimInfo.stSimStatus.enSimStatus = enSimStatus;
    return;
}


TAF_SDC_SIM_TYPE_ENUM_UINT8 TAF_SDC_GetSimType( VOS_VOID )
{
    return (TAF_SDC_GetSdcCtx()->stSimInfo.enSimType);
}


VOS_VOID TAF_SDC_SetSimType(TAF_SDC_SIM_TYPE_ENUM_UINT8 enSimType)
{
    TAF_SDC_GetSdcCtx()->stSimInfo.enSimType = enSimType;
    return;
}



TAF_SDC_PLATFORM_RAT_CAP_STRU*  TAF_SDC_GetPlatformRatCap( VOS_VOID )
{
    return &(TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stPlatformRatCap);
}

TAF_SDC_CURC_RPT_CTRL_STRU*  TAF_SDC_GetCurcRptCtrl( VOS_VOID )
{
    return &(TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsSysCfgInfo.stRptCtrl.stCurcRptCtrl);
}


TAF_SDC_UNSOLICITED_RPT_CTRL_STRU*  TAF_SDC_GetUnsolicitedRptCtrl( VOS_VOID )
{
    return &(TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsSysCfgInfo.stRptCtrl.stUnsolicitedRptCtrl);
}


TAF_SDC_NETWORK_CAP_INFO_STRU *TAF_SDC_GetGuNwCapInfo(VOS_VOID)
{
    return &(TAF_SDC_GetSdcCtx()->stNetworkInfo.stGuNwCapInfo);
}



TAF_SDC_NETWORK_CAP_INFO_STRU *TAF_SDC_GetLteNwCapInfo(VOS_VOID)
{
    return &(TAF_SDC_GetSdcCtx()->stNetworkInfo.stLteNwCapInfo);
}


VOS_VOID TAF_SDC_SetGuNwCapInfo(TAF_SDC_NETWORK_CAP_INFO_STRU *pstNwCapInfo)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stGuNwCapInfo.enLteCsCap   = pstNwCapInfo->enLteCsCap;
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stGuNwCapInfo.enNwEmcBsCap = pstNwCapInfo->enNwEmcBsCap;
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stGuNwCapInfo.enNwImsVoCap = pstNwCapInfo->enNwImsVoCap;
}


VOS_VOID TAF_SDC_SetLteNwCapInfo(TAF_SDC_NETWORK_CAP_INFO_STRU *pstNwCapInfo)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stLteNwCapInfo.enLteCsCap   = pstNwCapInfo->enLteCsCap;
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stLteNwCapInfo.enNwEmcBsCap = pstNwCapInfo->enNwEmcBsCap;
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stLteNwCapInfo.enNwImsVoCap = pstNwCapInfo->enNwImsVoCap;
}


VOS_VOID TAF_SDC_SetCsAttachAllowFlg(VOS_UINT8 ucCsAttachAllowFlg)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stCsDomainInfo.ucCsAttachAllowFlg = ucCsAttachAllowFlg;
    return;
}


VOS_UINT8 TAF_SDC_GetCsAttachAllowFlg(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stNetworkInfo.stCsDomainInfo.ucCsAttachAllowFlg;
}



VOS_UINT32 TAF_SDC_IsPlatformRatCapNvimValid(
    PLATAFORM_RAT_CAPABILITY_STRU      *pstNvPlatformRatCap
)
{
    VOS_UINT32                          i;
    VOS_UINT32                          j;

    /* ���ƽ̨֧�ֵĽ��뼼������Ϊ0��������Ч */
    if (0 == pstNvPlatformRatCap->usRatNum)
    {
        return VOS_FALSE;
    }

    for (i = 0; i < pstNvPlatformRatCap->usRatNum; i++)
    {
        /* ���ƽ̨֧�ֵĽ��뼼�����ظ���������Ч  */
        for (j = i + 1 ; j < pstNvPlatformRatCap->usRatNum; j++)
        {
            if (pstNvPlatformRatCap->aenRatList[i] == pstNvPlatformRatCap->aenRatList[j])
            {
                return VOS_FALSE;
            }
        }

        /* ���ƽ̨֧�ֵĽ��뼼�����ڻ����BUTT��������Ч */
        if (pstNvPlatformRatCap->aenRatList[i] >= PLATFORM_RAT_BUTT)
        {
            return VOS_FALSE;
        }
    }

    return VOS_TRUE;
}
TAF_SDC_PLATFORM_RAT_TYPE_ENUM_UINT8 TAF_SDC_ConvertNvPlatformRatToTafType(
    PLATFORM_RAT_TYPE_ENUM_UINT16       enNvPlatformRatCap
)
{
    TAF_SDC_PLATFORM_RAT_TYPE_ENUM_UINT8                    enTafSdcPlatformRat;

    switch (enNvPlatformRatCap)
    {
        case PLATFORM_RAT_GSM:
            enTafSdcPlatformRat = TAF_SDC_PLATFORM_RAT_GSM;
            break;

        case PLATFORM_RAT_WCDMA:
            enTafSdcPlatformRat = TAF_SDC_PLATFORM_RAT_WCDMA;
            break;

#if (FEATURE_ON == FEATURE_UE_MODE_TDS)
        case PLATFORM_RAT_TDS:
            enTafSdcPlatformRat = TAF_SDC_PLATFORM_RAT_TDS;
            break;
#endif

#if   (FEATURE_ON == FEATURE_LTE)
        case PLATFORM_RAT_LTE:
            enTafSdcPlatformRat = TAF_SDC_PLATFORM_RAT_LTE;
            break;
#endif

        default:
            enTafSdcPlatformRat = TAF_SDC_PLATFORM_RAT_BUTT;
            break;
    }

    return enTafSdcPlatformRat;
}


VOS_VOID TAF_SDC_InitPlatformRatCap(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_PLATFORM_RAT_CAP_STRU      *pstPlatformRatCap
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    PLATAFORM_RAT_CAPABILITY_STRU                           stNvPlatformRatCap;
    TAF_SDC_PLATFORM_RAT_TYPE_ENUM_UINT8                    enTafSdcPlatformRat;

    /* �����ϵ��ʼ��ʱ���г�ʼ���������������޸� */
    if (TAF_SDC_INIT_CTX_STARTUP != enInitType)
    {
        return;
    }

    /* ��ʼ��ΪGSM ONLY */
    pstPlatformRatCap->ucRatNum      = 1;
    pstPlatformRatCap->aenRatList[0] = TAF_SDC_PLATFORM_RAT_GSM;
    for (i = 1; i < TAF_SDC_PLATFORM_MAX_RAT_NUM; i++)
    {
        pstPlatformRatCap->aenRatList[i] = TAF_SDC_PLATFORM_RAT_BUTT;
    }

    /* ��ȡNV, �����ȡʧ�ܣ�ֱ�ӷ��أ�Ĭ��ֵ����Ϊ��֧��GSM */
    PS_MEM_SET(&stNvPlatformRatCap, 0x0, sizeof(stNvPlatformRatCap));
    if (NV_OK != NV_Read(en_NV_Item_Platform_RAT_CAP,
                         &stNvPlatformRatCap, sizeof(PLATAFORM_RAT_CAPABILITY_STRU)))
    {
        TAF_ERROR_LOG(WUEPS_PID_MMC, "TAF_SDC_InitPlatformRatCap():ERROR: read en_NV_Item_PLATFORM_RAT_CAP Error");

        return;
    }

    if (stNvPlatformRatCap.usRatNum >= PLATFORM_MAX_RAT_NUM)
    {
        stNvPlatformRatCap.usRatNum = PLATFORM_MAX_RAT_NUM;
    }

    /* ��Ч�Լ�飺��Ч��ֱ�ӷ��أ�Ĭ��ֵ����Ϊ��֧��GSM */
    if (VOS_FALSE == TAF_SDC_IsPlatformRatCapNvimValid(&stNvPlatformRatCap))
    {
        TAF_ERROR_LOG(WUEPS_PID_MMC, "TAF_SDC_InitPlatformRatCap():ERROR: en_NV_Item_PLATFORM_RAT_CAP Invalid");

        return;
    }

    j = 0;

    /* ��ȡ�ɹ�����NV�ж�ȡֵ��ֵ��pstPlatformRatCap */
    for (i = 0; i < stNvPlatformRatCap.usRatNum; i++)
    {
        enTafSdcPlatformRat = TAF_SDC_ConvertNvPlatformRatToTafType(stNvPlatformRatCap.aenRatList[i]);
        if (TAF_SDC_PLATFORM_RAT_BUTT != enTafSdcPlatformRat)
        {
            pstPlatformRatCap->aenRatList[j] = enTafSdcPlatformRat;
            j ++;
        }
    }

    /* ����д�NV�л�ȡƽ̨֧�ֽ��뼼������Ҫ����֧�ֵĸ��������򲻸��£���Ȼ���ճ�ʼֵ1 */
    if (j > 0)
    {
        pstPlatformRatCap->ucRatNum      = (VOS_UINT8)j;
    }

    return;
}


VOS_VOID TAF_SDC_InitSvlteSupportFlag(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
)
{
#if (FEATURE_ON == FEATURE_MULTI_MODEM)
    VOS_UINT32                                  ulLength;
    SVLTE_SUPPORT_FLAG_STRU                     stSvlteFlag;
#endif

    /* �����ϵ��ʼ��ʱ���г�ʼ���������������޸� */
    if (TAF_SDC_INIT_CTX_STARTUP != enInitType)
    {
        return;
    }

    /* ��ʼ��Ϊ��֧��SVLTE */
    pstMsCapInfo->ucSvlteSupportFlag = VOS_FALSE;

    /* SVLTEֻ����FEATURE_MULTI_MODEM��ʱ�����п�������Ϊ���� */
#if (FEATURE_ON == FEATURE_MULTI_MODEM)

    /* �Ȼ�ȡNV�ĳ��� */
    ulLength = 0;
    NV_GetLength(en_NV_Item_SVLTE_FLAG, &ulLength);

    if (ulLength > sizeof(SVLTE_SUPPORT_FLAG_STRU))
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_SDC_InitSvlteSupportFlag():WARNING: en_NV_Item_SVLTE length Error");

        return;
    }

    /* ��NV��en_NV_Item_SVLTE_FLAG��ʧ�ܣ�ֱ�ӷ��� */
    if (NV_OK != NV_Read(en_NV_Item_SVLTE_FLAG,
                         &stSvlteFlag, ulLength))
    {

        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_SDC_InitSvlteSupportFlag():WARNING: read en_NV_Item_SVLTE Error");

        return;
    }

    /* ��ֵ��ȫ�ֱ����� */
    pstMsCapInfo->ucSvlteSupportFlag = stSvlteFlag.ucSvlteSupportFlag;
#endif

    return;
}



VOS_VOID TAF_SDC_InitImsRatSupport(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
)
{
#if (FEATURE_ON == FEATURE_IMS)
    VOS_UINT32                                  ulLength;
    IMSA_NV_IMS_RAT_SUPPORT_STRU                stImsSupport;
#endif

    /* �����ϵ��ʼ��ʱ���г�ʼ���������������޸� */
    if (TAF_SDC_INIT_CTX_STARTUP != enInitType)
    {
        return;
    }

    /* IMS��ر�ʱ��Ĭ�ϳ�ʼ��Ϊ��֧��IMS */
    PS_MEM_SET(&(pstMsCapInfo->stImsConfigPara.stImsRatSupport), 0, sizeof(TAF_SDC_IMS_RAT_SUPPORT_STRU));
    pstMsCapInfo->stImsConfigPara.stImsRatSupport.ucGsmImsSupportFlag   = VOS_FALSE;
    pstMsCapInfo->stImsConfigPara.stImsRatSupport.ucUtranImsSupportFlag = VOS_FALSE;
    pstMsCapInfo->stImsConfigPara.stImsRatSupport.ucLteImsSupportFlag   = VOS_FALSE;
    pstMsCapInfo->stImsConfigPara.stImsRatSupport.ucGsmEmsSupportFlag   = VOS_FALSE;
    pstMsCapInfo->stImsConfigPara.stImsRatSupport.ucUtranEmsSupportFlag = VOS_FALSE;
    pstMsCapInfo->stImsConfigPara.stImsRatSupport.ucLteEmsSupportFlag   = VOS_FALSE;

    /* IMS����ֻ����FEATURE_IMS��ʱ�����п�������Ϊ���� */
#if (FEATURE_ON == FEATURE_IMS)

    /* �Ȼ�ȡNV�ĳ��� */
    ulLength = 0;
    NV_GetLength(EN_NV_ID_IMS_RAT_SUPPORT, &ulLength);

    if (ulLength > sizeof(IMSA_NV_IMS_RAT_SUPPORT_STRU))
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_SDC_InitImsRatSupport():WARNING: EN_NV_ID_IMS_RAT_SUPPORT length Error");

        return;
    }

    /* ��NV��EN_NV_ID_IMS_RAT_SUPPORT��ʧ�ܣ�ֱ�ӷ��� */
    if (NV_OK != NV_Read(EN_NV_ID_IMS_RAT_SUPPORT,
                         &stImsSupport, ulLength))
    {

        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_SDC_InitImsRatSupport():WARNING: read EN_NV_ID_IMS_RAT_SUPPORT Error");

        return;
    }

    /* ��ֵ��ȫ�ֱ����� */
    pstMsCapInfo->stImsConfigPara.stImsRatSupport.ucGsmImsSupportFlag   = stImsSupport.ucGsmEmsSupportFlag;
    pstMsCapInfo->stImsConfigPara.stImsRatSupport.ucUtranImsSupportFlag = stImsSupport.ucUtranImsSupportFlag;
    pstMsCapInfo->stImsConfigPara.stImsRatSupport.ucLteImsSupportFlag   = stImsSupport.ucLteImsSupportFlag;
    pstMsCapInfo->stImsConfigPara.stImsRatSupport.ucGsmEmsSupportFlag   = stImsSupport.ucGsmEmsSupportFlag;
    pstMsCapInfo->stImsConfigPara.stImsRatSupport.ucUtranEmsSupportFlag = stImsSupport.ucUtranEmsSupportFlag;
    pstMsCapInfo->stImsConfigPara.stImsRatSupport.ucLteEmsSupportFlag   = stImsSupport.ucLteEmsSupportFlag;
#endif

    return;
}



VOS_VOID TAF_SDC_InitImsCapability(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
)
{
#if (FEATURE_ON == FEATURE_IMS)
    VOS_UINT32                                  ulLength;
    IMS_NV_IMS_CAP_STRU                         stImsCapa;
#endif

    /* �����ϵ��ʼ��ʱ���г�ʼ���������������޸� */
    if (TAF_SDC_INIT_CTX_STARTUP != enInitType)
    {
        return;
    }

    /* IMS��ر�ʱ��Ĭ�ϳ�ʼ��Ϊ��֧��IMS VOICE��IMS SMS */
    PS_MEM_SET(&(pstMsCapInfo->stImsConfigPara.stImsCapability), 0, sizeof(TAF_SDC_IMS_CAPABILITY_STRU));
    pstMsCapInfo->stImsConfigPara.stImsCapability.ucVoiceCallOnImsSupportFlag   = VOS_FALSE;
    pstMsCapInfo->stImsConfigPara.stImsCapability.ucVideoCallOnImsSupportFlag   = VOS_FALSE;
    pstMsCapInfo->stImsConfigPara.stImsCapability.ucSmsOnImsSupportFlag         = VOS_FALSE;

    /* IMS����ֻ����FEATURE_IMS��ʱ�����п�������Ϊ���� */
#if (FEATURE_ON == FEATURE_IMS)

    /* �Ȼ�ȡNV�ĳ��� */
    ulLength = 0;
    NV_GetLength(EN_NV_ID_IMS_CAPABILITY, &ulLength);

    if (ulLength > sizeof(IMS_NV_IMS_CAP_STRU))
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_SDC_InitImsCapability():WARNING: EN_NV_ID_IMS_CAPABILITY length Error");

        return;
    }

    /* ��NV��EN_NV_ID_IMS_CAPABILITY��ʧ�ܣ�ֱ�ӷ��� */
    if (NV_OK != NV_Read(EN_NV_ID_IMS_CAPABILITY,
                         &stImsCapa, ulLength))
    {

        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_SDC_InitImsCapability():WARNING: read EN_NV_ID_IMS_CAPABILITY Error");

        return;
    }

    /* ��ֵ��ȫ�ֱ����� */
    pstMsCapInfo->stImsConfigPara.stImsCapability.ucVoiceCallOnImsSupportFlag   = stImsCapa.ucVoiceCallOnImsSupportFlag;
    pstMsCapInfo->stImsConfigPara.stImsCapability.ucVideoCallOnImsSupportFlag   = stImsCapa.ucVideoCallOnImsSupportFlag;
    pstMsCapInfo->stImsConfigPara.stImsCapability.ucSmsOnImsSupportFlag         = stImsCapa.ucSmsOnImsSupportFlag;
#endif

    return;
}


VOS_VOID TAF_SDC_InitVoiceDomain(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
)
{
#if (FEATURE_ON == FEATURE_IMS)
    VOS_UINT32                                  ulLength;
    LNAS_LMM_NV_VOICE_DOMAIN_STRU               stVoiceDomain;
#endif

    /* �����ϵ��ʼ��ʱ���г�ʼ���������������޸� */
    if (TAF_SDC_INIT_CTX_STARTUP != enInitType)
    {
        return;
    }

    /* Ĭ�ϳ�ʼ��ΪCS ONLY */
    pstMsCapInfo->stImsConfigPara.enVoiceDomain = TAF_SDC_VOICE_DOMAIN_CS_ONLY;

#if (FEATURE_ON == FEATURE_IMS)
    /* �Ȼ�ȡNV�ĳ��� */
    ulLength = 0;
    NV_GetLength(EN_NV_ID_UE_VOICE_DOMAIN, &ulLength);

    if (ulLength > sizeof(LNAS_LMM_NV_VOICE_DOMAIN_STRU))
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_SDC_InitVoiceDomain():WARNING: EN_NV_ID_UE_VOICE_DOMAIN length Error");

        return;
    }

    /* ��NV��EN_NV_ID_UE_VOICE_DOMAIN��ʧ�ܣ�ֱ�ӷ��� */
    if (NV_OK != NV_Read(EN_NV_ID_UE_VOICE_DOMAIN,
                         &stVoiceDomain, ulLength))
    {

        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_SDC_InitVoiceDomain():WARNING: read EN_NV_ID_UE_VOICE_DOMAIN Error");

        return;
    }

    /* NV������ȡֵ�Ϸ�ʱ����ֵ��ȫ�ֱ����� */
    if ( (NV_ITEM_ACTIVE == stVoiceDomain.bitOpVoicDomain)
      && (NAS_LMM_VOICE_DOMAIN_IMS_PS_PREFERRED >= stVoiceDomain.enVoicDomain) )
    {
        pstMsCapInfo->stImsConfigPara.enVoiceDomain = stVoiceDomain.enVoicDomain;
    }
#endif

    return;
}
VOS_VOID TAF_SDC_InitSmsDomain(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
)
{
#if (FEATURE_ON == FEATURE_IMS)
    VOS_UINT32                                  ulLength;
    TAF_NVIM_SMS_DOMAIN_STRU                    stSmsDomain;
#endif

    /* �����ϵ��ʼ��ʱ���г�ʼ���������������޸� */
    if (TAF_SDC_INIT_CTX_STARTUP != enInitType)
    {
        return;
    }

    /* Ĭ�ϳ�ʼ��Ϊnot use sms over ip */
    pstMsCapInfo->stImsConfigPara.enSmsDomain   = TAF_SDC_SMS_DOMAIN_NOT_USE_SMS_OVER_IP;

#if (FEATURE_ON == FEATURE_IMS)
    /* �Ȼ�ȡNV�ĳ��� */
    ulLength = 0;
    NV_GetLength(en_NV_Item_SMS_DOMAIN, &ulLength);

    if (ulLength > sizeof(TAF_NVIM_SMS_DOMAIN_STRU))
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_SDC_InitSmsDomain():WARNING: en_NV_Item_SMS_DOMAIN length Error");

        return;
    }

    /* ��NV��en_NV_Item_SMS_DOMAIN��ʧ�ܣ�ֱ�ӷ��� */
    if (NV_OK != NV_Read(en_NV_Item_SMS_DOMAIN,
                         &stSmsDomain, ulLength))
    {

        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_SDC_InitSmsDomain():WARNING: read en_NV_Item_SMS_DOMAIN Error");

        return;
    }

    /* NV������ȡֵ�Ϸ�ʱ����ֵ��ȫ�ֱ����� */
    if (TAF_SDC_SMS_DOMAIN_PREFER_TO_USE_SMS_OVER_IP >= stSmsDomain.ucSmsDomain)
    {
        pstMsCapInfo->stImsConfigPara.enSmsDomain = stSmsDomain.ucSmsDomain;
    }
#endif

    return;
}
VOS_VOID TAF_SDC_InitLteCsServiceCfg(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
)
{
    NAS_NVIM_LTE_CS_SERVICE_CFG_STRU                        stLteCsServiceCfg;
    VOS_UINT32                                              ulLength;

    /* �����ϵ��ʼ��ʱ���г�ʼ���������������޸� */
    if (TAF_SDC_INIT_CTX_STARTUP != enInitType)
    {
        return;
    }

    ulLength        = 0;
    PS_MEM_SET(&stLteCsServiceCfg, 0x00, sizeof(NAS_NVIM_LTE_CS_SERVICE_CFG_STRU));

    /* Ĭ�ϳ�ʼ��ΪTAF_SDC_LTE_SUPPORT_BUTT����MMC��ʼֵ����һ�� */
    pstMsCapInfo->stImsConfigPara.enLteCsServiceCfg = TAF_SDC_LTE_SUPPORT_BUTT;

    NV_GetLength(en_NV_Item_Lte_Cs_Service_Config, &ulLength);

    if (ulLength > sizeof(stLteCsServiceCfg))
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_SDC_InitLteCsServiceCfg():WARNING: en_NV_Item_Lte_Cs_Service_Config length Error");
        return;
    }
    if ( NV_OK != NV_Read(en_NV_Item_Lte_Cs_Service_Config,
                          &stLteCsServiceCfg,
                          ulLength ))
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_SDC_InitLteCsServiceCfg(): en_NV_Item_Lte_Cs_Service_Config Error");
        return;
    }

    /* ���NV����ֵ��Ч�������ȫ�ֱ��� */
    if ((NV_ITEM_ACTIVE == stLteCsServiceCfg.ucNvimActiveFlg)
     && (stLteCsServiceCfg.ucLteCsServiceCfg < TAF_SDC_LTE_SUPPORT_BUTT))
    {
        pstMsCapInfo->stImsConfigPara.enLteCsServiceCfg = stLteCsServiceCfg.ucLteCsServiceCfg;

    }

    return;
}
VOS_VOID TAF_SDC_InitRedialCfgInfo(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
)
{
#if (FEATURE_ON == FEATURE_IMS)
    VOS_UINT32                                  ulLength;
    TAF_NV_SWITCH_DOMAIN_REDIAL_CONFIG_STRU     stRedialImsToCs;
#endif

    /* �����ϵ��ʼ��ʱ���г�ʼ���������������޸� */
    if (TAF_SDC_INIT_CTX_STARTUP != enInitType)
    {
        return;
    }

    /* Ĭ�ϳ�ʼ��ΪFALSE */
    /* IMS to CS */
    PS_MEM_SET(&(pstMsCapInfo->stImsConfigPara.stRedial.stRedialImsToCs),
               0,
               sizeof(TAF_SDC_SWITCH_DOMAIN_REDIAL_CONFIG_STRU));

    /* CS to IMS */
    PS_MEM_SET(&(pstMsCapInfo->stImsConfigPara.stRedial.stRedialCsToIms),
               0,
               sizeof(TAF_SDC_SWITCH_DOMAIN_REDIAL_CONFIG_STRU));

#if (FEATURE_ON == FEATURE_IMS)
    /* �Ȼ�ȡNV�ĳ��� */
    ulLength = 0;
    NV_GetLength(en_NV_Item_REDIAL_IMS_TO_CS_DOMAIN, &ulLength);

    if (ulLength > sizeof(TAF_NV_SWITCH_DOMAIN_REDIAL_CONFIG_STRU))
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_SDC_InitRedialConfig():WARNING: en_NV_Item_REDIAL_IMS_TO_CS_DOMAIN length Error");

        return;
    }

    /* ��NV��en_NV_Item_REDIAL_IMS_TO_CS_DOMAIN��ʧ�ܣ�ֱ�ӷ��� */
    if (NV_OK != NV_Read(en_NV_Item_REDIAL_IMS_TO_CS_DOMAIN,
                         &stRedialImsToCs, ulLength))
    {

        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_SDC_InitRedialConfig():WARNING: read en_NV_Item_REDIAL_IMS_TO_CS_DOMAIN Error");

        return;
    }

    /* NV�������ز����� */
    pstMsCapInfo->stImsConfigPara.stRedial.stRedialImsToCs.ucCallRedial = stRedialImsToCs.ucCallRedial;
    pstMsCapInfo->stImsConfigPara.stRedial.stRedialImsToCs.ucSmsRedial  = stRedialImsToCs.ucSmsRedial;
    pstMsCapInfo->stImsConfigPara.stRedial.stRedialImsToCs.ucSsRedial   = stRedialImsToCs.ucSsRedial;

    /* CS to IMS����Ļ����ز�δ���������� */
#endif

    return;
}


VOS_VOID TAF_SDC_InitImsRoamingCfgInfo(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
)
{
#if (FEATURE_ON == FEATURE_IMS)
    VOS_UINT32                                  ulLength;
    TAF_NV_IMS_ROAMING_SUPPORT_STRU             stRoamingFlg;
#endif

    /* �����ϵ��ʼ��ʱ���г�ʼ���������������޸� */
    if (TAF_SDC_INIT_CTX_STARTUP != enInitType)
    {
        return;
    }

    /* Ĭ�ϳ�ʼ��ΪFALSE */
    pstMsCapInfo->stImsConfigPara.ucRoamingSupport = VOS_FALSE;

#if (FEATURE_ON == FEATURE_IMS)
    /* �Ȼ�ȡNV�ĳ��� */
    ulLength = 0;
    NV_GetLength(en_NV_Item_IMS_ROAMING_SUPPORT_FLG, &ulLength);

    if (ulLength > sizeof(TAF_NV_IMS_ROAMING_SUPPORT_STRU))
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, 
                      "TAF_SDC_InitImsRoamingCfgInfo():WARNING: en_NV_Item_IMS_ROAMING_SUPPORT_FLG length Error");

        return;
    }

    /* ��NV��en_NV_Item_IMS_ROAMING_SUPPORT_FLG��ʧ�ܣ�ֱ�ӷ��� */
    if (NV_OK != NV_Read(en_NV_Item_IMS_ROAMING_SUPPORT_FLG,
                         &stRoamingFlg, ulLength))
    {

        TAF_ERROR_LOG(WUEPS_PID_TAF, 
                      "TAF_SDC_InitImsRoamingCfgInfo():WARNING: read en_NV_Item_IMS_ROAMING_SUPPORT_FLG Error");

        return;
    }

    /* NV������ims����֧��������Ϣ */
    pstMsCapInfo->stImsConfigPara.ucRoamingSupport = stRoamingFlg.ucImsRoamingFlg;
#endif

    return;
}


VOS_VOID TAF_SDC_InitImsUssdCfgInfo(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
)
{
#if (FEATURE_ON == FEATURE_IMS)
    VOS_UINT32                                  ulLength;
    TAF_NV_IMS_USSD_SUPPORT_STRU                stUssdFlg;
#endif

    /* �����ϵ��ʼ��ʱ���г�ʼ���������������޸� */
    if (TAF_SDC_INIT_CTX_STARTUP != enInitType)
    {
        return;
    }

    /* Ĭ�ϳ�ʼ��ΪFALSE */
    pstMsCapInfo->stImsConfigPara.stImsCapability.ucUssdOnImsSupportFlag = VOS_FALSE;

#if (FEATURE_ON == FEATURE_IMS)
    /* �Ȼ�ȡNV�ĳ��� */
    ulLength = 0;
    NV_GetLength(en_NV_Item_IMS_USSD_SUPPORT_FLG, &ulLength);

    if (ulLength > sizeof(TAF_NV_IMS_USSD_SUPPORT_STRU))
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, 
                      "TAF_SDC_InitImsUssdCfgInfo():WARNING: en_NV_Item_IMS_USSD_SUPPORT_FLG length Error");

        return;
    }

    /* ��NV��en_NV_Item_IMS_USSD_SUPPORT_FLG��ʧ�ܣ�ֱ�ӷ��� */
    if (NV_OK != NV_Read(en_NV_Item_IMS_USSD_SUPPORT_FLG,
                         &stUssdFlg, ulLength))
    {

        TAF_ERROR_LOG(WUEPS_PID_TAF, 
                      "TAF_SDC_InitImsUssdCfgInfo():WARNING: read en_NV_Item_IMS_USSD_SUPPORT_FLG Error");

        return;
    }

    /* NV������USSD֧��������Ϣ */
    pstMsCapInfo->stImsConfigPara.stImsCapability.ucUssdOnImsSupportFlag = stUssdFlg.ucUssdOnImsSupportFlag;
#endif

    return;
}
VOS_VOID TAF_SDC_InitImsConfigPara(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
)
{
    /* EN_NV_ID_IMS_RAT_SUPPORT */
    TAF_SDC_InitImsRatSupport(enInitType, pstMsCapInfo);

    /* EN_NV_ID_IMS_CAPABILITY */
    TAF_SDC_InitImsCapability(enInitType, pstMsCapInfo);

    /* EN_NV_ID_UE_VOICE_DOMAIN */
    TAF_SDC_InitVoiceDomain(enInitType, pstMsCapInfo);

    /* en_NV_Item_SMS_DOMAIN */
    TAF_SDC_InitSmsDomain(enInitType, pstMsCapInfo);

    /* en_NV_Item_Lte_Cs_Service_Config */
    TAF_SDC_InitLteCsServiceCfg(enInitType, pstMsCapInfo);

    TAF_SDC_InitRedialCfgInfo(enInitType, pstMsCapInfo);

    TAF_SDC_InitImsRoamingCfgInfo(enInitType, pstMsCapInfo);

    TAF_SDC_SetWaitImsVoiceAvailTimerLen(TI_TAF_SDC_WAIT_IMSA_IMS_VOICE_AVAIL_IND_LEN);

    TAF_SDC_InitImsUssdCfgInfo(enInitType, pstMsCapInfo);
}
VOS_VOID TAF_SDC_InitLcConfigPara(
    TAF_SDC_LC_CONFIG_PARA_STRU        *pstLcConfigPara
)
{
    pstLcConfigPara->ucLCEnableFlg  = VOS_FALSE;
    pstLcConfigPara->enRatCombined  = TAF_NVIM_LC_RAT_COMBINED_BUTT;
    pstLcConfigPara->enLCWorkCfg    = TAF_NVIM_LC_WORK_CFG_BUTT;
    pstLcConfigPara->aucReserved[0] = 0;

    return;
}


VOS_VOID TAF_SDC_InitMsCapInfo(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8     enInitType,
    TAF_SDC_MS_CAP_INFO_STRU            *pstMsCapInfo
)
{
    TAF_SDC_InitPlatformRatCap(enInitType, &(pstMsCapInfo->stPlatformRatCap));

    TAF_SDC_InitSvlteSupportFlag(enInitType, pstMsCapInfo);

    TAF_SDC_InitImsConfigPara(enInitType, pstMsCapInfo);

    TAF_SDC_InitLcConfigPara(&(pstMsCapInfo->stLcConfigPara));
}


VOS_VOID TAF_SDC_InitRptCtrlInfo(VOS_VOID)
{
    /* ��ʼ��ȫ�ֱ��� */
    TAF_SDC_InitCurcRptCtrlInfo();

    /* ��ʼ���������������ϱ���ʶ */
    TAF_SDC_InitUnsolicitedRptCtrlInfo();
    return;
}


VOS_VOID TAF_SDC_InitUssdCfgInfo(VOS_VOID)
{
    TAF_SDC_ReadUssdTransModeNvim();

    TAF_SDC_ReadUssdTransTableNvim();

    return;
}



VOS_VOID TAF_SDC_ReadUssdTransModeNvim(VOS_VOID)
{
    TAF_USSD_NVIM_TRANS_MODE_STRU       stUssdTranMode;
    VOS_UINT32                          ulLength;

    ulLength = 0;

    PS_MEM_SET(&stUssdTranMode, 0x00, sizeof(TAF_USSD_NVIM_TRANS_MODE_STRU));

    NV_GetLength(en_NV_Item_CUST_USSD_MODE, &ulLength);

    /* ��ȡUSSD�Ƿ�͸������ */
    if (NV_OK == NV_Read(en_NV_Item_CUST_USSD_MODE,
                          &stUssdTranMode,
                          ulLength))
    {
        if (VOS_TRUE != stUssdTranMode.ucStatus)
        {
            stUssdTranMode.ucUssdTransMode = TAF_SDC_USSD_TRAN_MODE;
        }
    }
    else
    {
        stUssdTranMode.ucUssdTransMode = TAF_SDC_USSD_TRAN_MODE;
    }

    TAF_SDC_SetUssdTransMode(stUssdTranMode.ucUssdTransMode);

    return;
}
VOS_VOID TAF_SDC_ReadUssdTransTableNvim(VOS_VOID)
{
    SSA_NV_ITEM_ALPHA_to_ASCII_STRU               stUSSDApha;
    VOS_UINT32                                    ulResult;
    VOS_UINT32                                    ulLength;

    ulLength = 0;
    stUSSDApha.ucStatus         = VOS_FALSE;
    stUSSDApha.ucAlphaTransMode = TAF_SDC_USSD_TRAN_MODE_BUTT;

    NV_GetLength(en_NV_Item_USSD_Apha_To_Ascii, &ulLength);

    ulResult = NV_Read(en_NV_Item_USSD_Apha_To_Ascii,
                       &stUSSDApha,
                       ulLength);

    if ((ulResult == NV_OK)
     && (VOS_TRUE == stUSSDApha.ucStatus)
     && (VOS_TRUE == (stUSSDApha.ucAlphaTransMode & 0x01)))
    {
        TAF_SDC_Set7BitToAssicTableType(TAF_SDC_ALPHA_TO_ASCII_TRAN_TABLE_TME);
    }
    else
    {
        TAF_SDC_Set7BitToAssicTableType(TAF_SDC_ALPHA_TO_ASCII_TRAN_TABLE_DEF);
    }

    return;
}



VOS_VOID TAF_SDC_InitAppCfgSupportType(VOS_VOID)
{
    VOS_UINT32                          ulLength;
    NAS_NVIM_SYSTEM_APP_CONFIG_STRU     stSysAppConfig;
    VOS_UINT16                          usAppCfgType;

    stSysAppConfig.usSysAppConfigType = SYSTEM_APP_BUTT;

    NV_GetLength(en_NV_Item_System_APP_Config, &ulLength);

    if (ulLength > sizeof(stSysAppConfig))
    {
        TAF_SDC_SetAppCfgSupportType(SYSTEM_APP_MP);
        return;
    }

    if (NV_OK != NV_Read(en_NV_Item_System_APP_Config,
                         &stSysAppConfig, ulLength))
    {
        TAF_SDC_SetAppCfgSupportType(SYSTEM_APP_MP);

        return;
    }

    if ( SYSTEM_APP_ANDROID < stSysAppConfig.usSysAppConfigType )
    {
        usAppCfgType    = SYSTEM_APP_MP;
    }
    else
    {
        usAppCfgType    = stSysAppConfig.usSysAppConfigType ;
    }

    TAF_SDC_SetAppCfgSupportType(usAppCfgType);

    return;
}


VOS_VOID TAF_SDC_InitUeUsageSetting(VOS_VOID)
{
    VOS_UINT32                          ulLength;
    LNAS_LMM_NV_UE_CENTER_STRU          stUeCentric;

    ulLength = 0;
    
    NV_GetLength(EN_NV_ID_UE_CENTER, &ulLength);

    if (ulLength > sizeof(LNAS_LMM_NV_UE_CENTER_STRU))
    {        
        /* set data centric same as LNAS operation */
        TAF_SDC_SetUeUsageSetting(EMM_SETTING_DATA_CENTRIC);
        
        return;
    }

    stUeCentric.enUeCenter = EMM_USAGE_SETTING_BUTT;
    
    if (NV_OK != NV_Read(EN_NV_ID_UE_CENTER,
                         &stUeCentric, ulLength))
    {
        /* set data centric same as LNAS operation */
        TAF_SDC_SetUeUsageSetting(TAF_SDC_UE_USAGE_DATA_CENTRIC);

        return;
    }

    TAF_SDC_SetUeUsageSetting((TAF_SDC_UE_USAGE_SETTING_ENUM_UINT8)(stUeCentric.enUeCenter));

    return;    
}

VOS_VOID TAF_SDC_InitMsSysCfgInfo(TAF_SDC_MS_SYS_CFG_INFO_STRU *pstMsSysCfgInfo)
{
    TAF_SDC_InitRptCtrlInfo();

    TAF_SDC_InitUssdCfgInfo();

    TAF_SDC_InitAppCfgSupportType();

    TAF_SDC_InitDsdaPlmnSearchEnhancedCfg();
	
    TAF_SDC_InitUeUsageSetting();

#if (FEATURE_ON == FEATURE_LTE)
    pstMsSysCfgInfo->stPrioRatList.ucRatNum     = TAF_SDC_MAX_RAT_NUM;
    pstMsSysCfgInfo->stPrioRatList.aenRatPrio[0]= TAF_SDC_SYS_MODE_LTE;
    pstMsSysCfgInfo->stPrioRatList.aenRatPrio[1]= TAF_SDC_SYS_MODE_WCDMA;
    pstMsSysCfgInfo->stPrioRatList.aenRatPrio[2]= TAF_SDC_SYS_MODE_GSM;
#else
    pstMsSysCfgInfo->stPrioRatList.ucRatNum     = TAF_SDC_MAX_RAT_NUM;
    pstMsSysCfgInfo->stPrioRatList.aenRatPrio[0]= TAF_SDC_SYS_MODE_WCDMA;
    pstMsSysCfgInfo->stPrioRatList.aenRatPrio[1]= TAF_SDC_SYS_MODE_GSM;
#endif
    
    return;
}



VOS_VOID TAF_SDC_InitMsCfgInfo(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8     enInitType,
    TAF_SDC_MS_CFG_INFO_STRU            *pstMsCfgInfo
)
{
    TAF_SDC_InitMsCapInfo(enInitType, &(pstMsCfgInfo->stMsCapInfo));

    TAF_SDC_InitMsSysCfgInfo(&pstMsCfgInfo->stMsSysCfgInfo);
    return;
}


VOS_VOID TAF_SDC_InitCampPlmnInfo(TAF_SDC_CAMP_PLMN_INFO_STRU *pstCampPlmnInfo)
{
    pstCampPlmnInfo->stPlmnId.ulMcc = TAF_SDC_INVALID_MCC;
    pstCampPlmnInfo->stPlmnId.ulMnc = TAF_SDC_INVALID_MNC;
    pstCampPlmnInfo->enSysMode      = TAF_SDC_SYS_MODE_BUTT;
    pstCampPlmnInfo->enSysSubMode   = TAF_SDC_SYS_SUBMODE_BUTT;
    pstCampPlmnInfo->ucRac          = TAF_SDC_RAC_INVALID;
    pstCampPlmnInfo->usLac          = TAF_SDC_LAC_INVALID;
    pstCampPlmnInfo->ulCellId       = TAF_SDC_CELLID_INVALID;
    pstCampPlmnInfo->ucRoamFlag     = VOS_FALSE;
    pstCampPlmnInfo->ucCampOnFlg    = VOS_FALSE;

    PS_MEM_SET(pstCampPlmnInfo->aucReserved, 0, sizeof(pstCampPlmnInfo->aucReserved));
}


VOS_VOID TAF_SDC_InitCsDomainInfo(TAF_SDC_CS_DOMAIN_INFO_STRU *pstCsDomainInfo)
{
    pstCsDomainInfo->enCsRegStatus                         = TAF_SDC_REG_NOT_REGISTERED_NOT_SEARCH;
    pstCsDomainInfo->enCsServiceStatus                     = TAF_SDC_SERVICE_STATUS_NO_SERVICE;

    /* С����������Ĭ����Ҫ����Ϊ������,��������ΪBUTTʱRIL����Ϊ���� */
    pstCsDomainInfo->stCsAcRestriction.enCellAcType        = TAF_SDC_CELL_RESTRICTION_TYPE_NONE;

    pstCsDomainInfo->stCsAcRestriction.ucRestrictPagingRsp = VOS_FALSE;
    pstCsDomainInfo->stCsAcRestriction.ucRestrictRegister  = VOS_FALSE;

    pstCsDomainInfo->ucSimCsRegStatus                      = VOS_FALSE;

    TAF_SDC_SetCsAttachAllowFlg(VOS_FALSE);
    return;
}
VOS_VOID TAF_SDC_InitPsDomainInfo(TAF_SDC_PS_DOMAIN_INFO_STRU *pstPsDomainInfo)
{
    pstPsDomainInfo->enPsRegStatus                         = TAF_SDC_REG_NOT_REGISTERED_NOT_SEARCH;
    pstPsDomainInfo->enPsServiceStatus                     = TAF_SDC_SERVICE_STATUS_NO_SERVICE;

    /* С����������Ĭ����Ҫ����Ϊ������,��������ΪBUTTʱRIL����Ϊ���� */
    pstPsDomainInfo->stPsAcRestriction.enCellAcType        = TAF_SDC_CELL_RESTRICTION_TYPE_NONE;
    
    pstPsDomainInfo->stPsAcRestriction.ucRestrictPagingRsp = VOS_FALSE;
    pstPsDomainInfo->stPsAcRestriction.ucRestrictRegister  = VOS_FALSE;
    pstPsDomainInfo->ucSimPsRegStatus                      = VOS_FALSE;
	
    TAF_SDC_SetPsAttachAllowFlg(VOS_FALSE);
	
    return;
}
VOS_VOID TAF_SDC_InitGuNwCapInfo(
    TAF_SDC_NETWORK_CAP_INFO_STRU *pstGuNwCapInfo
)
{
    /* ��ʼ�������������Ϣ */
    PS_MEM_SET(pstGuNwCapInfo, 0, sizeof(TAF_SDC_NETWORK_CAP_INFO_STRU));
    pstGuNwCapInfo->enNwImsVoCap = TAF_SDC_NW_IMS_VOICE_NOT_SUPPORTED;
    pstGuNwCapInfo->enNwEmcBsCap = TAF_SDC_NW_EMC_BS_NOT_SUPPORTED;
    pstGuNwCapInfo->enLteCsCap   = TAF_SDC_LTE_CS_CAPBILITY_NOT_SUPPORTED;

    return;
}
VOS_VOID TAF_SDC_InitLteNwCapInfo(
    TAF_SDC_NETWORK_CAP_INFO_STRU *pstLteNwCapInfo
)
{
    /* ��ʼ�������������Ϣ */
    PS_MEM_SET(pstLteNwCapInfo, 0, sizeof(TAF_SDC_NETWORK_CAP_INFO_STRU));
    pstLteNwCapInfo->enNwImsVoCap = TAF_SDC_NW_IMS_VOICE_NOT_SUPPORTED;
    pstLteNwCapInfo->enNwEmcBsCap = TAF_SDC_NW_EMC_BS_NOT_SUPPORTED;
    pstLteNwCapInfo->enLteCsCap   = TAF_SDC_LTE_CS_CAPBILITY_NOT_SUPPORTED;

    return;
}
VOS_VOID TAF_SDC_InitNetWorkInfo(TAF_SDC_NETWORK_INFO_STRU *pstNetworkInfo)
{
    TAF_SDC_InitCampPlmnInfo(&pstNetworkInfo->stCampPlmnInfo);

    TAF_SDC_InitCsDomainInfo(&pstNetworkInfo->stCsDomainInfo);

    TAF_SDC_InitPsDomainInfo(&pstNetworkInfo->stPsDomainInfo);

    pstNetworkInfo->enServiceDomain = TAF_SDC_SERVICE_DOMAIN_BUTT;

    pstNetworkInfo->enReportSrvsta  = TAF_SDC_REPORT_SRVSTA_BUTT;

    TAF_SDC_InitGuNwCapInfo(&pstNetworkInfo->stGuNwCapInfo);
    TAF_SDC_InitLteNwCapInfo(&pstNetworkInfo->stLteNwCapInfo);


    PS_MEM_SET(pstNetworkInfo->aucReserved, 0, sizeof(pstNetworkInfo->aucReserved));

    /* ���IMS��������Ϣ */
    TAF_SDC_SetImsVoiceAvailFlg(VOS_FALSE);

    TAF_SDC_SetImsNormalRegStatus(TAF_SDC_IMS_NORMAL_REG_STATUS_DEREG);

    return;
}
VOS_VOID TAF_SDC_InitServiceInfo(
    TAF_SDC_SERVICE_INFO_STRU          *pstServiceInfo
)
{
    pstServiceInfo->stSmsInfo.ucCsSmsSrvExistFlg    = VOS_FALSE;
    pstServiceInfo->stSmsInfo.ucPsSmsSrvExistFlg    = VOS_FALSE;
    pstServiceInfo->stSmsInfo.ucImsSmsSrvExistFlg   = VOS_FALSE;
    pstServiceInfo->stSsInfo.ucCsSsSrvExistFlg      = VOS_FALSE;
    pstServiceInfo->stSsInfo.ucImsSsSrvExistFlg     = VOS_FALSE;
    PS_MEM_SET(pstServiceInfo->stSmsInfo.aucReserved, 0, sizeof(pstServiceInfo->stSmsInfo.aucReserved));
    PS_MEM_SET(pstServiceInfo->stSsInfo.aucReserved, 0, sizeof(pstServiceInfo->stSsInfo.aucReserved));

    TAF_SDC_InitCallInfo(&(pstServiceInfo->stCallInfo));

    return;
}



VOS_VOID TAF_SDC_InitCallInfo(
    TAF_SDC_CALL_INFO_STRU             *pstCallInfo
)
{
    PS_MEM_SET(pstCallInfo, 0, sizeof(TAF_SDC_CALL_INFO_STRU));

    /* ��ʼ��CALL��ر�־ */
    pstCallInfo->ucCsCallExistFlg                       = VOS_FALSE;
    pstCallInfo->ucImsCallExistFlg                      = VOS_FALSE;
    
    /* SRVCC��־�����Ƶ���CALLģ��*/

    /* ��ʼ��MM��ؽ����������б� */
    pstCallInfo->stMmCallEmerNumList.ulMcc              = TAF_SDC_INVALID_MCC;
    pstCallInfo->stMmCallEmerNumList.ucEmergencyNumber  = 0;

    /* ����NV��en_NV_Item_CUSTOM_ECC_NUM_LIST����ʼ��APP���ƽ����������б� */
    TAF_SDC_ReadCustomEccNumListNvim();

    TAF_SDC_SetCsServiceConnStatusFlag(VOS_FALSE);

    return;
}


VOS_VOID TAF_SDC_InitPhoneInfo(
    TAF_SDC_PHONE_INFO_STRU            *pstPhInfo
)
{
    PS_MEM_SET(pstPhInfo, 0, sizeof(TAF_SDC_PHONE_INFO_STRU));

    pstPhInfo->enPhMode = TAF_SDC_PHONE_MODE_MINI;
}



VOS_VOID TAF_SDC_InitRegReportStatus(
    TAF_SDC_REG_REPORT_STATUS_STRU     *pstRegReportStatus
)
{
    PS_MEM_SET(pstRegReportStatus, 0, sizeof(TAF_SDC_REG_REPORT_STATUS_STRU));

    pstRegReportStatus->enCregType  = TAF_SDC_CREG_TYPE_BUTT;
    pstRegReportStatus->enCgregType = TAF_SDC_CGREG_TYPE_BUTT;
    pstRegReportStatus->enCeregType = TAF_SDC_CEREG_TYPE_BUTT;
}


VOS_VOID TAF_SDC_ReadCustomEccNumListNvim(VOS_VOID)
{
    MN_CALL_NVIM_CUSTOM_ECC_NUM_LIST_STRU   stCustomEccNumList;
    VOS_UINT8                               i;
    TAF_SDC_CALL_INFO_STRU                 *pstCallInfo = VOS_NULL_PTR;
    
    pstCallInfo = TAF_SDC_GetCallInfo();


    PS_MEM_SET(&stCustomEccNumList, 0x00, sizeof(stCustomEccNumList));

    /* ��ȡAPP���ƽ������б�NV */
    if (NV_OK != NV_Read(en_NV_Item_CUSTOM_ECC_NUM_LIST,
                         &stCustomEccNumList,
                         sizeof(MN_CALL_NVIM_CUSTOM_ECC_NUM_LIST_STRU)))
    {
        return;
    }

    /* ����б��б���ĸ���Ϊ��Чֵ */
    if ( stCustomEccNumList.ucEccNumCount > TAF_SDC_MAX_CUSTOM_ECC_NUM)
    {
        return;
    }


    /* ����NV���ֵ��ֵ��ȫ�ֱ����� */    
    pstCallInfo->stCustomCallEccNumCtx.stCustomEccNumList.ucEccNumCount
        = stCustomEccNumList.ucEccNumCount;

    /* ��NV��ȡ�����ݱ�����ȫ�ֱ����� */
    for (i = 0; i < stCustomEccNumList.ucEccNumCount; i++)
    {
        pstCallInfo->stCustomCallEccNumCtx.stCustomEccNumList.astCustomEccNumList[i].ucCategory
            = stCustomEccNumList.astCustomEccNumList[i].ucCategory;

        pstCallInfo->stCustomCallEccNumCtx.stCustomEccNumList.astCustomEccNumList[i].ucValidSimPresent
            = stCustomEccNumList.astCustomEccNumList[i].ucValidSimPresent;

        pstCallInfo->stCustomCallEccNumCtx.stCustomEccNumList.astCustomEccNumList[i].ulMcc
            = stCustomEccNumList.astCustomEccNumList[i].ulMcc;

        pstCallInfo->stCustomCallEccNumCtx.stCustomEccNumList.astCustomEccNumList[i].ucEccNumLen
            = stCustomEccNumList.astCustomEccNumList[i].ucEccNumLen;

        VOS_MemCpy(pstCallInfo->stCustomCallEccNumCtx.stCustomEccNumList.astCustomEccNumList[i].aucEccNum,
                   stCustomEccNumList.astCustomEccNumList[i].aucEccNum,
                   stCustomEccNumList.astCustomEccNumList[i].ucEccNumLen);
    }

    return;
}




VOS_VOID TAF_SDC_InitMsIdentity(TAF_SDC_SIM_MS_IDENTITY_STRU *pstMsIdentity)
{
    VOS_UINT32                          i;

    PS_MEM_SET(pstMsIdentity, 0, sizeof(TAF_SDC_SIM_MS_IDENTITY_STRU));

    for ( i = 0 ; i < TAF_SDC_MAX_IMSI_LEN; i++ )
    {
        pstMsIdentity->aucImsi[i]      = TAF_SDC_IMSI_INVALID;

		pstMsIdentity->aucLastImsi[i]  = TAF_SDC_IMSI_INVALID;
	}

    return;
}


VOS_VOID TAF_SDC_InitSimStatus(TAF_SDC_SIM_STATUS_STRU *pstSimStatus)
{
    PS_MEM_SET(pstSimStatus, 0, sizeof(TAF_SDC_SIM_STATUS_STRU));

    pstSimStatus->enSimStatus = TAF_SDC_USIM_STATUS_BUTT;
    return;
}



VOS_VOID TAF_SDC_InitSimInfo(TAF_SDC_SIM_INFO_STRU *pstSimInfo)
{
    TAF_SDC_InitSimStatus(&pstSimInfo->stSimStatus);

    TAF_SDC_InitMsIdentity(&pstSimInfo->stMsIdentity);

    pstSimInfo->enSimType = TAF_SDC_SIM_TYPE_BUTT;

    return;
}


VOS_VOID TAF_SDC_InitCtx(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8     enInitType,
    TAF_SDC_CTX_STRU                    *pstSdcCtx
)
{
    TAF_SDC_InitMsCfgInfo(enInitType, &pstSdcCtx->stMsCfgInfo);

    TAF_SDC_InitNetWorkInfo(&pstSdcCtx->stNetworkInfo);

    TAF_SDC_InitSimInfo(&pstSdcCtx->stSimInfo);

    TAF_SDC_InitServiceInfo(&pstSdcCtx->stServiceInfo);

#if (FEATURE_ON == FEATURE_PTM)
    TAF_SDC_InitErrLogInfo(&pstSdcCtx->stErrlogInfo);
#endif

    TAF_SDC_InitPhoneInfo(&pstSdcCtx->stPhoneInfo);

    TAF_SDC_InitRegReportStatus(&pstSdcCtx->stRegReportStatus);
}


VOS_VOID TAF_SDC_InitCurcRptCtrlInfo(VOS_VOID)
{
    TAF_SDC_CURC_RPT_CTRL_STRU             *pstCurcRptCtrl = VOS_NULL_PTR;
    NVIM_PRIVATE_CMD_STATUS_RPT_STRU        stNvimCmdRptCtrl;
    VOS_UINT32                              ulLength;

    ulLength = 0;

    /* ��ȡ^CURC�����Ӧ��NV�ȫ�ֱ��� */
    stNvimCmdRptCtrl.ucStatus                     = VOS_FALSE;
    stNvimCmdRptCtrl.ucStatusRptGeneralControl    = TAF_SDC_STATUS_RPT_GENERAL_CONTROL_TYPE_REPORT;

    pstCurcRptCtrl = TAF_SDC_GetCurcRptCtrl();
    NV_GetLength(en_NV_Item_PRIVATE_CMD_STATUS_RPT, &ulLength);

    if(NV_OK != NV_Read(en_NV_Item_PRIVATE_CMD_STATUS_RPT,
                          &stNvimCmdRptCtrl,
                          ulLength))
    {
        pstCurcRptCtrl->enStatusRptGeneralControl = TAF_SDC_STATUS_RPT_GENERAL_CONTROL_TYPE_REPORT;
        PS_MEM_SET(pstCurcRptCtrl->aucRptCfg, 0xFF, TAF_SDC_RPT_CFG_MAX_SIZE);
    }
    else
    {
        if( (VOS_TRUE == stNvimCmdRptCtrl.ucStatus)
         && (VOS_FALSE == stNvimCmdRptCtrl.ucStatusRptGeneralControl) )
        {
            pstCurcRptCtrl->enStatusRptGeneralControl = TAF_SDC_STATUS_RPT_GENERAL_CONTROL_TYPE_NO_REPORT;
            PS_MEM_SET(pstCurcRptCtrl->aucRptCfg, 0x00, TAF_SDC_RPT_CFG_MAX_SIZE);
        }
        else
        {
            pstCurcRptCtrl->enStatusRptGeneralControl = TAF_SDC_STATUS_RPT_GENERAL_CONTROL_TYPE_REPORT;
            PS_MEM_SET(pstCurcRptCtrl->aucRptCfg, 0xFF, TAF_SDC_RPT_CFG_MAX_SIZE);
        }
    }
}
VOS_VOID TAF_SDC_InitUnsolicitedRptCtrlInfo(VOS_VOID)
{
    TAF_SDC_UNSOLICITED_RPT_CTRL_STRU      *pstUnsolicitedRptCtrl   = VOS_NULL_PTR;

    pstUnsolicitedRptCtrl   = TAF_SDC_GetUnsolicitedRptCtrl();

    /* Ĭ��Ϊ�����ϱ�ȫ�� */
    PS_MEM_SET(pstUnsolicitedRptCtrl->aucRptCfg, 0xFF, TAF_SDC_RPT_CFG_MAX_SIZE);

    /* CREG/CGREG/CEREG�ر� */
    TAF_SDC_UpdateRptCmdStatus(TAF_SDC_RPT_CMD_CREG, VOS_FALSE);
    TAF_SDC_UpdateRptCmdStatus(TAF_SDC_RPT_CMD_CGREG, VOS_FALSE);
    TAF_SDC_UpdateRptCmdStatus(TAF_SDC_RPT_CMD_CEREG, VOS_FALSE);

    /* CTZV�ر� */
    TAF_SDC_UpdateRptCmdStatus(TAF_SDC_RPT_CMD_CTZV, VOS_FALSE);

    /* CUSD�ر� */
    TAF_SDC_UpdateRptCmdStatus(TAF_SDC_RPT_CMD_CUSD, VOS_FALSE);

    /* CUUS1�ر� */
    TAF_SDC_UpdateRptCmdStatus(TAF_SDC_RPT_CMD_CUUS1I, VOS_FALSE);
    TAF_SDC_UpdateRptCmdStatus(TAF_SDC_RPT_CMD_CUUS1U, VOS_FALSE);

    /* CSSN�ر� */
    TAF_SDC_UpdateRptCmdStatus(TAF_SDC_RPT_CMD_CSSI, VOS_FALSE);
    TAF_SDC_UpdateRptCmdStatus(TAF_SDC_RPT_CMD_CSSU, VOS_FALSE);

    /* �����ϱ���ʼֵͨ����NV en_NV_Item_DSFLOW_REPORT��ȡ */

    /* CERSSI�ϱ���ʼֵͨ����en_NV_Item_CELL_SIGN_REPORT_CFG��ȡ */
    TAF_SDC_ReadCellSignReportCfgNV();
    return;
}


VOS_VOID TAF_SDC_ReadCellSignReportCfgNV( VOS_VOID )
{
    TAF_SDC_CELL_SIGN_REPORT_CFG_STRU   stCellSignReportCfg;
    VOS_UINT32                          ulCerssiRptFlg;
    VOS_UINT32                          ulLength;

    ulLength = 0;

    PS_MEM_SET(&stCellSignReportCfg, 0x0, sizeof(stCellSignReportCfg));
    NV_GetLength(en_NV_Item_CELL_SIGN_REPORT_CFG, &ulLength);
    if (ulLength > sizeof(stCellSignReportCfg))
    {
        return;
    }
    if(NV_OK != NV_Read(en_NV_Item_CELL_SIGN_REPORT_CFG,
                          &stCellSignReportCfg,
                          ulLength))
    {
         TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_SDC_ReadCellSignReportCfgNV:read en_NV_Item_CELL_SIGN_REPORT_CFG failed");
         return;
    }

    ulCerssiRptFlg  = VOS_TRUE;

    /* ����ֵΪ��Чֵ�����ϱ� */
    if (TAF_SDC_MAX_SIGN_THRESHOLD < stCellSignReportCfg.ucSignThreshold)
    {
        ulCerssiRptFlg  = VOS_FALSE;
    }

    /* ����ֵΪ0�����ϱ� */
    if (0 == stCellSignReportCfg.ucSignThreshold)
    {
        ulCerssiRptFlg  = VOS_FALSE;
    }

    TAF_SDC_UpdateRptCmdStatus(TAF_SDC_RPT_CMD_CERSSI, ulCerssiRptFlg);

    return;
}



VOS_VOID    TAF_SDC_SetCsCallExistFlg(
    VOS_UINT8                           ucCsCallExistFlg
)
{
    TAF_SDC_GetSdcCtx()->stServiceInfo.stCallInfo.ucCsCallExistFlg = ucCsCallExistFlg;

    return;
}


VOS_VOID TAF_SDC_SetCsServiceConnStatusFlag(
    VOS_UINT8                           ucCsServiceConnStatusFlag
)
{
    TAF_SDC_GetSdcCtx()->stServiceInfo.stCsInfo.ucCsServiceConnStatusFlag = ucCsServiceConnStatusFlag;

    return;
}


VOS_UINT8 TAF_SDC_GetCsServiceConnStatusFlag(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stServiceInfo.stCsInfo.ucCsServiceConnStatusFlag;
}




VOS_UINT8   TAF_SDC_GetCsCallExistFlg(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stServiceInfo.stCallInfo.ucCsCallExistFlg;
}


VOS_VOID    TAF_SDC_SetImsCallExistFlg(
    VOS_UINT8                           ucImsCallExistFlg
)
{
    TAF_SDC_GetSdcCtx()->stServiceInfo.stCallInfo.ucImsCallExistFlg = ucImsCallExistFlg;

    return;
}


VOS_UINT8   TAF_SDC_GetImsCallExistFlg(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stServiceInfo.stCallInfo.ucImsCallExistFlg;
}

/* SRVCC�Ƶ�CALLģ���Լ�ά������*/

TAF_SDC_CALL_INFO_STRU* TAF_SDC_GetCallInfo(VOS_VOID)
{
    return &(TAF_SDC_GetSdcCtx()->stServiceInfo.stCallInfo);
}



VOS_VOID TAF_SDC_SetMmCallEmerNumList(
    TAF_SDC_MM_EMERGENCY_LIST_STRU     *pstMmCallEmerNumList
)
{
    TAF_SDC_CALL_INFO_STRU             *pstCallInfo = VOS_NULL_PTR;

    pstCallInfo = TAF_SDC_GetCallInfo();

    PS_MEM_CPY(&(pstCallInfo->stMmCallEmerNumList), pstMmCallEmerNumList, sizeof(TAF_SDC_MM_EMERGENCY_LIST_STRU));

    return;
}


TAF_SDC_MM_EMERGENCY_LIST_STRU* TAF_SDC_GetMmCallEmerNumList(VOS_VOID)
{
    return &(TAF_SDC_GetSdcCtx()->stServiceInfo.stCallInfo.stMmCallEmerNumList);
}


VOS_VOID TAF_SDC_SetCustomCallEccNumCtx(
    TAF_SDC_CUSTOM_ECC_CTX_STRU        *pstCustomCallEccNumCtx
)
{
    TAF_SDC_CALL_INFO_STRU             *pstCallInfo = VOS_NULL_PTR;

    pstCallInfo = TAF_SDC_GetCallInfo();

    PS_MEM_CPY(&(pstCallInfo->stCustomCallEccNumCtx), pstCustomCallEccNumCtx, sizeof(TAF_SDC_CUSTOM_ECC_CTX_STRU));

    return;
}


TAF_SDC_CUSTOM_ECC_CTX_STRU* TAF_SDC_GetCustomCallEccNumCtx(VOS_VOID)
{
    return &(TAF_SDC_GetSdcCtx()->stServiceInfo.stCallInfo.stCustomCallEccNumCtx);
}



VOS_CHAR** TAF_SDC_GetCallEmerNumsNoUsimTblAddr(VOS_VOID)
{
    return g_aucTafSdcCallEmerNumsNoUsim;
}


VOS_CHAR** TAF_SDC_GetCallEmerNumsWithUsimTblAddr(VOS_VOID)
{
    return g_aucTafSdcCallEmerNumsWithUsim;
}



VOS_VOID  TAF_SDC_SetUtranSkipWPlmnSearchFlag(VOS_UINT8  ucUtranSkipWPlmnSearchFlag)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsSysCfgInfo.stDsdaPlmnSearchEnhancedCfg.ucUtranSkipWPlmnSearchFlag = ucUtranSkipWPlmnSearchFlag;
    return;
}



VOS_UINT8  TAF_SDC_GetUtranSkipWPlmnSearchFlag(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsSysCfgInfo.stDsdaPlmnSearchEnhancedCfg.ucUtranSkipWPlmnSearchFlag;
}



VOS_VOID TAF_SDC_InitDsdaPlmnSearchEnhancedCfg(VOS_VOID)
{
#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    NV_DSDA_PLMN_SEARCH_ENHANCED_CFG_STRU                   stDsdaPlmnSearchEnhancedCfg;
    VOS_UINT32                                              ulLength;

    ulLength = 0;
    PS_MEM_SET(&stDsdaPlmnSearchEnhancedCfg, 0x00, sizeof(stDsdaPlmnSearchEnhancedCfg));

    TAF_SDC_SetUtranSkipWPlmnSearchFlag(VOS_FALSE);

    /* �Ȼ�ȡNV�ĳ��� */
    NV_GetLength(en_NV_Item_DSDA_PLMN_SEARCH_ENHANCED_CFG, &ulLength);

    if (ulLength > sizeof(NV_DSDA_PLMN_SEARCH_ENHANCED_CFG_STRU))
    {
        return;
    }

    /* ��NVʧ�� */
    if (NV_OK != NV_Read(en_NV_Item_DSDA_PLMN_SEARCH_ENHANCED_CFG,
                         &stDsdaPlmnSearchEnhancedCfg, ulLength))
    {
        return;
    }

    if (VOS_TRUE == (stDsdaPlmnSearchEnhancedCfg.usSolutionMask & 0x0001))
    {
         TAF_SDC_SetUtranSkipWPlmnSearchFlag(VOS_TRUE);
    }
#endif

    return;
}




VOS_VOID    TAF_SDC_SetUssdTransMode(
    VOS_UINT8                           ucTransMode
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsSysCfgInfo.stUssdCfg.ucUssdTransMode = ucTransMode;

    return;
}


VOS_UINT8   TAF_SDC_GetUssdTransMode(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsSysCfgInfo.stUssdCfg.ucUssdTransMode;
}


VOS_VOID    TAF_SDC_Set7BitToAssicTableType(
    VOS_UINT8                           ucType
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsSysCfgInfo.stUssdCfg.ucAlphaTransMode = ucType;

    return;
}


VOS_UINT8   TAF_SDC_Get7BitToAssicTableType(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsSysCfgInfo.stUssdCfg.ucAlphaTransMode;
}


VOS_UINT8*   TAF_SDC_GetCurr7BitToAsciiTableAddr(VOS_VOID)
{
    if (TAF_SDC_ALPHA_TO_ASCII_TRAN_TABLE_TME == TAF_SDC_Get7BitToAssicTableType())
    {
        return g_aucTafSdcTme7BitToAsciiTbl;
    }
    else
    {
        return g_aucTafSdcDef7BitToAsciiTbl;
    }
}



VOS_VOID TAF_SDC_GetCurrentLai(
    VOS_UINT8                          *aucLai,
    VOS_UINT32                         *pulLaiLen
)
{
    VOS_UINT32                          ulPos;
    TAF_SDC_REG_STATUS_ENUM_UINT8       enCsRegStatus;
    TAF_SDC_CAMP_PLMN_INFO_STRU        *pstCampplmnInfo = VOS_NULL_PTR;

    pstCampplmnInfo = TAF_SDC_GetCampPlmnInfo();

    ulPos = 0;

    aucLai[ulPos]  = (VOS_UINT8)(0x0000000F & pstCampplmnInfo->stPlmnId.ulMcc);
    aucLai[ulPos] |= (VOS_UINT8)((0x00000F00 & pstCampplmnInfo->stPlmnId.ulMcc) >> 4);
    ulPos++;

    aucLai[ulPos]  = (VOS_UINT8)((0x000F0000 & pstCampplmnInfo->stPlmnId.ulMcc) >> 16);
    aucLai[ulPos] |= (VOS_UINT8)((0x000F0000 & pstCampplmnInfo->stPlmnId.ulMnc) >> 12);
    ulPos++;

    aucLai[ulPos]  = (VOS_UINT8)(0x0000000F & pstCampplmnInfo->stPlmnId.ulMnc);
    aucLai[ulPos] |= (VOS_UINT8)((0x00000F00 & pstCampplmnInfo->stPlmnId.ulMnc) >> 4);
    ulPos++;

    aucLai[ulPos]  = (VOS_UINT8)((pstCampplmnInfo->usLac & 0xff00) >> 8);
    ulPos++;
    aucLai[ulPos]  = (VOS_UINT8)(pstCampplmnInfo->usLac & 0x00ff);
    ulPos++;

    aucLai[ulPos] = (VOS_UINT8)((pstCampplmnInfo->ulCellId >> 8) & 0xff);
    ulPos++;
    aucLai[ulPos] = (VOS_UINT8)(pstCampplmnInfo->ulCellId & 0xff);
    ulPos++;

    if (TAF_SDC_SYS_MODE_GSM != pstCampplmnInfo->enSysMode)
    {
        aucLai[ulPos] = (VOS_UINT8)((pstCampplmnInfo->ulCellId >> 24) & 0xff);
        ulPos++;
        aucLai[ulPos] = (VOS_UINT8)((pstCampplmnInfo->ulCellId >> 16) & 0xff);
        ulPos++;
    }

    *pulLaiLen = ulPos;

    /*��ȡMM��ע��״̬*/
    enCsRegStatus = TAF_SDC_GetCsRegStatus();
    if ((TAF_SDC_REG_REGISTERED_HOME_NETWORK != enCsRegStatus)
     && (TAF_SDC_REG_REGISTERED_ROAM != enCsRegStatus))
    {
        PS_MEM_SET(aucLai, (VOS_CHAR)0xff, ulPos);
    }

    return;
}
VOS_VOID    TAF_SDC_SetSvlteSupportFlag(
    VOS_UINT8                          ucSvlteSupportFlag
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.ucSvlteSupportFlag = ucSvlteSupportFlag;

    return;
}


VOS_UINT8   TAF_SDC_GetSvlteSupportFlag(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.ucSvlteSupportFlag;
}


VOS_VOID    TAF_SDC_SetCsSmsSrvExistFlg(
    VOS_UINT8                           ucCsSmsSrvExistFlg
)
{
    TAF_SDC_GetSdcCtx()->stServiceInfo.stSmsInfo.ucCsSmsSrvExistFlg = ucCsSmsSrvExistFlg;

    return;
}


VOS_UINT8   TAF_SDC_GetCsSmsSrvExistFlg(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stServiceInfo.stSmsInfo.ucCsSmsSrvExistFlg;
}


VOS_VOID TAF_SDC_SetPsSmsSrvExistFlg(
    VOS_UINT8                           ucPsSmsSrvExistFlg
)
{
    TAF_SDC_GetSdcCtx()->stServiceInfo.stSmsInfo.ucPsSmsSrvExistFlg = ucPsSmsSrvExistFlg;

    return;
}


VOS_UINT8 TAF_SDC_GetPsSmsSrvExistFlg(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stServiceInfo.stSmsInfo.ucPsSmsSrvExistFlg;
}


VOS_VOID TAF_SDC_SetImsSmsSrvExistFlg(
    VOS_UINT8                           ucImsSmsSrvExistFlg
)
{
    TAF_SDC_GetSdcCtx()->stServiceInfo.stSmsInfo.ucImsSmsSrvExistFlg = ucImsSmsSrvExistFlg;

    return;
}


VOS_UINT8 TAF_SDC_GetImsSmsSrvExistFlg(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stServiceInfo.stSmsInfo.ucImsSmsSrvExistFlg;
}



VOS_VOID    TAF_SDC_SetCsSsSrvExistFlg(
    VOS_UINT8                           ucCsSsSrvExistFlg
)
{
    TAF_SDC_GetSdcCtx()->stServiceInfo.stSsInfo.ucCsSsSrvExistFlg = ucCsSsSrvExistFlg;

    return;
}


VOS_UINT8   TAF_SDC_GetCsSsSrvExistFlg(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stServiceInfo.stSsInfo.ucCsSsSrvExistFlg;
}


VOS_VOID    TAF_SDC_SetImsSsSrvExistFlg(
    VOS_UINT8                           ucImsSsSrvExistFlg
)
{
    TAF_SDC_GetSdcCtx()->stServiceInfo.stSsInfo.ucImsSsSrvExistFlg = ucImsSsSrvExistFlg;

    return;
}


VOS_UINT8   TAF_SDC_GetImsSsSrvExistFlg(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stServiceInfo.stSsInfo.ucImsSsSrvExistFlg;
}

VOS_VOID TAF_SDC_SetGsmImsSupportFlag(
    VOS_UINT8                          ucGsmImsSupportFlag
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stImsRatSupport.ucGsmImsSupportFlag = ucGsmImsSupportFlag;

    return;
}


VOS_UINT8 TAF_SDC_GetGsmImsSupportFlag(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stImsRatSupport.ucGsmImsSupportFlag;
}


VOS_VOID TAF_SDC_SetUtranImsSupportFlag(
    VOS_UINT8                          ucUtranImsSupportFlag
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stImsRatSupport.ucUtranImsSupportFlag = ucUtranImsSupportFlag;

    return;
}


VOS_UINT8 TAF_SDC_GetUtranImsSupportFlag(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stImsRatSupport.ucUtranImsSupportFlag;
}



VOS_VOID TAF_SDC_SetLteImsSupportFlag(
    VOS_UINT8                          ucLteImsSupportFlag
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stImsRatSupport.ucLteImsSupportFlag = ucLteImsSupportFlag;

    return;
}


VOS_UINT8 TAF_SDC_GetLteImsSupportFlag(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stImsRatSupport.ucLteImsSupportFlag;
}


VOS_VOID TAF_SDC_SetAppCfgSupportType(
    VOS_UINT16                          usAppCfgSupportType
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsSysCfgInfo.usAppCfgSupportType = usAppCfgSupportType;

    return;
}


VOS_UINT16 TAF_SDC_GetAppCfgSupportType(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsSysCfgInfo.usAppCfgSupportType;
}


#if (FEATURE_ON == FEATURE_PTM)

OM_RING_ID TAF_SDC_GetErrLogRingBufAddr(VOS_VOID)
{
    return g_stTafSdcCtx.stErrlogInfo.pstRingBuffer;
}


VOS_VOID TAF_SDC_SetErrLogRingBufAddr(OM_RING_ID pRingBuffer)
{
    g_stTafSdcCtx.stErrlogInfo.pstRingBuffer = pRingBuffer;

    return;
}


VOS_UINT8 TAF_SDC_GetErrlogCtrlFlag(VOS_VOID)
{
    return g_stTafSdcCtx.stErrlogInfo.ucErrLogCtrlFlag;
}


VOS_VOID TAF_SDC_SetErrlogCtrlFlag(VOS_UINT8 ucFlag)
{
    g_stTafSdcCtx.stErrlogInfo.ucErrLogCtrlFlag = ucFlag;

    return;
}


VOS_UINT16 TAF_SDC_GetErrlogAlmLevel(VOS_VOID)
{
    return g_stTafSdcCtx.stErrlogInfo.usAlmLevel;
}


VOS_VOID TAF_SDC_SetErrlogAlmLevel(VOS_UINT16 usAlmLevel)
{
    g_stTafSdcCtx.stErrlogInfo.usAlmLevel = usAlmLevel;

    return;
}


VOS_VOID TAF_SDC_ReadErrlogCtrlInfoNvim(VOS_VOID)
{
    VOS_UINT32                          ulResult;
    VOS_UINT32                          ulLength;
    NV_ID_ERR_LOG_CTRL_INFO_STRU        stErrorLogCtrlInfo;

    ulLength = 0;
    PS_MEM_SET(&stErrorLogCtrlInfo, 0x00, sizeof(NV_ID_ERR_LOG_CTRL_INFO_STRU));
    NV_GetLength(en_NV_Item_ErrLogCtrlInfo, &ulLength);

    ulResult = NV_Read(en_NV_Item_ErrLogCtrlInfo,
                       &stErrorLogCtrlInfo,
                       ulLength);

    if (ulResult == NV_OK)
    {
        TAF_SDC_SetErrlogCtrlFlag(stErrorLogCtrlInfo.ucAlmStatus);
        TAF_SDC_SetErrlogAlmLevel(stErrorLogCtrlInfo.ucAlmLevel);
    }
    else
    {
        TAF_SDC_SetErrlogCtrlFlag(VOS_FALSE);
        TAF_SDC_SetErrlogAlmLevel(NAS_ERR_LOG_CTRL_LEVEL_CRITICAL);
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_SDC_ReadErrlogCtrlInfoNvim:read en_NV_Item_ErrLogCtrlInfo failed");
    }

    return;
}


VOS_VOID TAF_SDC_InitErrLogInfo(
    TAF_SDC_ERRLOG_INFO_STRU           *pstErrLogInfo
)
{
    VOS_CHAR                           *pbuffer;
    OM_RING_ID                          pRingbuffer;

    TAF_SDC_ReadErrlogCtrlInfoNvim();

    /* ����cache�Ķ�̬�ڴ� , ���ȼ�1����Ϊ����дָ��֮����д��ʱ�����һ���ֽ� */
    pbuffer = (char *)PS_MEM_ALLOC(WUEPS_PID_TAF, TAF_SDC_RING_BUFFER_SIZE + 1);
    if (VOS_NULL_PTR == pbuffer)
    {
        pstErrLogInfo->pstRingBuffer = VOS_NULL_PTR;
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_SDC_InitErrLogInfo: alloc fail");
        return;
    }

    /* ����OM�Ľӿڣ�������Ķ�̬�ڴ洴��ΪRING BUFFER */
    pRingbuffer = OM_RingBufferCreateEx(pbuffer, TAF_SDC_RING_BUFFER_SIZE + 1);
    if (VOS_NULL_PTR == pRingbuffer)
    {
        PS_MEM_FREE(WUEPS_PID_TAF, pbuffer);
    }

    /* ����ringbufferָ�� */
    pstErrLogInfo->pstRingBuffer = pRingbuffer;

    return;

}
VOS_UINT32 TAF_SDC_PutErrLogRingBuf(
    VOS_CHAR                           *pbuffer,
    VOS_UINT32                          ulbytes
)
{
    VOS_UINT32                          ulFreeSize;
    OM_RING_ID                          pTafRingBuffer;

    pTafRingBuffer = TAF_SDC_GetErrLogRingBufAddr();
    if (VOS_NULL_PTR == pTafRingBuffer)
    {
        return 0;
    }

    /* ���д���RING BUFFER������д�� */
    if (ulbytes > TAF_SDC_RING_BUFFER_SIZE)
    {
        return 0;
    }

    /* ��ȡRING BUFFERʣ��ռ��С */
    ulFreeSize = (VOS_UINT32)OM_RingBufferFreeBytes(pTafRingBuffer);

    /* ���ʣ��ռ䲻��д��Ĵ�С�������RING BUFFER */
    if (ulFreeSize < ulbytes)
    {
        OM_RingBufferFlush(pTafRingBuffer);
    }

    /* д��RING BUFFER */
    return (VOS_UINT32)OM_RingBufferPut(pTafRingBuffer, pbuffer, (VOS_INT)ulbytes);
}


VOS_UINT32 TAF_SDC_GetErrLogRingBufContent(
    VOS_CHAR                           *pbuffer,
    VOS_UINT32                          ulbytes
)
{
    return (VOS_UINT32)OM_RingBufferGet(TAF_SDC_GetErrLogRingBufAddr(), pbuffer, (VOS_INT)ulbytes);
}


VOS_UINT32 TAF_SDC_GetErrLogRingBufferUseBytes(VOS_VOID)
{
    return (VOS_UINT32)OM_RingBufferNBytes(TAF_SDC_GetErrLogRingBufAddr());
}


VOS_VOID TAF_SDC_CleanErrLogRingBuf(VOS_VOID)
{
    OM_RingBufferFlush(TAF_SDC_GetErrLogRingBufAddr());

    return;
}

#endif


VOS_VOID TAF_SDC_SetVoiceCallOnImsSupportFlag(
    VOS_UINT8                          ucVoiceCallOnImsSupportFlag
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stImsCapability.ucVoiceCallOnImsSupportFlag = ucVoiceCallOnImsSupportFlag;

    return;
}


VOS_UINT8 TAF_SDC_GetVoiceCallOnImsSupportFlag(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stImsCapability.ucVoiceCallOnImsSupportFlag;
}


VOS_VOID TAF_SDC_SetSmsOnImsSupportFlag(
    VOS_UINT8                          ucSmsOnImsSupportFlag
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stImsCapability.ucSmsOnImsSupportFlag = ucSmsOnImsSupportFlag;

    return;
}


VOS_UINT8 TAF_SDC_GetSmsOnImsSupportFlag(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stImsCapability.ucSmsOnImsSupportFlag;
}


VOS_VOID TAF_SDC_SetVideoCallOnImsSupportFlag(
    VOS_UINT8                          ucVideoCallOnImsSupportFlag
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stImsCapability.ucVideoCallOnImsSupportFlag = ucVideoCallOnImsSupportFlag;

    return;
}


VOS_UINT8 TAF_SDC_GetVideoCallOnImsSupportFlag(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stImsCapability.ucVideoCallOnImsSupportFlag;
}



VOS_VOID TAF_SDC_SetVoiceDomain(
    TAF_SDC_VOICE_DOMAIN_ENUM_UINT32    enVoiceDomain
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.enVoiceDomain = enVoiceDomain;

    return;
}


TAF_SDC_VOICE_DOMAIN_ENUM_UINT32 TAF_SDC_GetVoiceDomain(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.enVoiceDomain;
}


VOS_VOID TAF_SDC_SetWaitImsVoiceAvailTimerLen(
    VOS_UINT32                          ulTimerLen
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.ulWaitImsVoiceAvailTimerLen = ulTimerLen;

    return;
}


VOS_UINT32 TAF_SDC_GetWaitImsVoiceAvailTimerLen(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.ulWaitImsVoiceAvailTimerLen;
}



VOS_VOID TAF_SDC_SetSmsDomain(
    TAF_SDC_SMS_DOMAIN_ENUM_UINT8      enSmsDomain
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.enSmsDomain = enSmsDomain;

    return;
}


TAF_SDC_SMS_DOMAIN_ENUM_UINT8 TAF_SDC_GetSmsDomain(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.enSmsDomain;
}


VOS_VOID TAF_SDC_SetLteEmsSupportFlag(
    VOS_UINT8                           ucLteEmsSupportFlag
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stImsRatSupport.ucLteEmsSupportFlag = ucLteEmsSupportFlag;

    return;
}


VOS_UINT8 TAF_SDC_GetLteEmsSupportFlag(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stImsRatSupport.ucLteEmsSupportFlag;
}


VOS_VOID TAF_SDC_SetLteCsSeviceCap(
    TAF_SDC_LTE_CS_SERVICE_ENUM_UINT8   enLteCsServiceCap
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.enLteCsServiceCfg = enLteCsServiceCap;

    return;
}


TAF_SDC_LTE_CS_SERVICE_ENUM_UINT8 TAF_SDC_GetLteCsSeviceCap(VOS_VOID)
{
    return (TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.enLteCsServiceCfg);
}


TAF_SDC_UE_USAGE_SETTING_ENUM_UINT8 TAF_SDC_GetUeUsageSetting(VOS_VOID)
{
    return (TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsSysCfgInfo.enUeUsageSetting);
}


VOS_VOID TAF_SDC_SetUeUsageSetting(
    TAF_SDC_UE_USAGE_SETTING_ENUM_UINT8   enUeUsageSetting
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsSysCfgInfo.enUeUsageSetting = enUeUsageSetting;
}


TAF_SDC_SYS_MODE_ENUM_UINT8 TAF_SDC_GetCampSysMode( VOS_VOID )
{
    if (VOS_TRUE == TAF_SDC_GetCampOnFlag())
    {
        return TAF_SDC_GetSysMode();
    }

    return TAF_SDC_SYS_MODE_BUTT;
}


TAF_SDC_RAT_PRIO_STRU* TAF_SDC_GetMsPrioRatList( VOS_VOID )
{
    return &(TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsSysCfgInfo.stPrioRatList);
}




VOS_VOID TAF_SDC_SetMsPrioRatList(
    TAF_SDC_RAT_PRIO_STRU              *pstPrioRatList
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsSysCfgInfo.stPrioRatList = *pstPrioRatList;
}



VOS_UINT8 TAF_SDC_GetCallRedailFromImsToCsSupportFlag(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stRedial                    \
        .stRedialImsToCs.ucCallRedial;
}


VOS_VOID TAF_SDC_SetCallRedailFromImsToCsSupportFlag(VOS_UINT8 ucCallRedialFromImsToCs)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stRedial.stRedialImsToCs           \
        .ucCallRedial = ucCallRedialFromImsToCs;
}


VOS_UINT8 TAF_SDC_GetSmsRedailFromImsToCsSupportFlag(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stRedial.stRedialImsToCs    \
        .ucSmsRedial;
}


VOS_VOID TAF_SDC_SetSmsRedailFromImsToCsSupportFlag(VOS_UINT8 ucSmsRedialFromImsToCs)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stRedial.stRedialImsToCs           \
        .ucSmsRedial = ucSmsRedialFromImsToCs;
}


VOS_UINT8 TAF_SDC_GetRoamingSupportFlag(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.ucRoamingSupport;
}


VOS_VOID TAF_SDC_SetRoamingSupportFlag(VOS_UINT8 ucRoamingSupport)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.ucRoamingSupport = ucRoamingSupport;
}


VOS_VOID    TAF_SDC_SetImsNormalRegStatus(
    TAF_SDC_IMS_NORMAL_REG_STATUS_ENUM_UINT8    enRegStatus
)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stImsDomainInfo.enImsNormalRegSta = enRegStatus;

    return;
}


TAF_SDC_IMS_NORMAL_REG_STATUS_ENUM_UINT8   TAF_SDC_GetImsNormalRegStatus(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stNetworkInfo.stImsDomainInfo.enImsNormalRegSta;
}


VOS_VOID    TAF_SDC_SetImsVoiceAvailFlg(
    VOS_UINT8       ucAvail
)
{
    TAF_SDC_GetSdcCtx()->stNetworkInfo.stImsDomainInfo.ucImsAvail = ucAvail;

    return;
}


VOS_UINT8   TAF_SDC_GetImsVoiceAvailFlg(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stNetworkInfo.stImsDomainInfo.ucImsAvail;
}


VOS_VOID  TAF_SDC_SetCurPhoneMode(
    TAF_SDC_PHONE_MODE_ENUM_UINT8       enPhoneMode
)
{
    (TAF_SDC_GetSdcCtx()->stPhoneInfo.enPhMode) = enPhoneMode;
}



TAF_SDC_PHONE_MODE_ENUM_UINT8  TAF_SDC_GetCurPhoneMode(VOS_VOID)
{
    return (TAF_SDC_GetSdcCtx()->stPhoneInfo.enPhMode);
}



TAF_SDC_CREG_TYPE_ENUM_UINT8  TAF_SDC_GetCregType(VOS_VOID)
{
    return (TAF_SDC_GetSdcCtx()->stRegReportStatus.enCregType);
}


VOS_VOID  TAF_SDC_SetCregType(TAF_SDC_CREG_TYPE_ENUM_UINT8 enCregType)
{
    TAF_SDC_GetSdcCtx()->stRegReportStatus.enCregType = enCregType;
}


TAF_SDC_CGREG_TYPE_ENUM_UINT8  TAF_SDC_GetCgregType(VOS_VOID)
{
    return (TAF_SDC_GetSdcCtx()->stRegReportStatus.enCgregType);
}


VOS_VOID  TAF_SDC_SetCgregType(TAF_SDC_CGREG_TYPE_ENUM_UINT8 enCgregType)
{
    TAF_SDC_GetSdcCtx()->stRegReportStatus.enCgregType = enCgregType;
}


TAF_SDC_CEREG_TYPE_ENUM_UINT8  TAF_SDC_GetCeregType(VOS_VOID)
{
    return (TAF_SDC_GetSdcCtx()->stRegReportStatus.enCeregType);
}


VOS_VOID  TAF_SDC_SetCeregType(TAF_SDC_CEREG_TYPE_ENUM_UINT8 enCeregType)
{
    TAF_SDC_GetSdcCtx()->stRegReportStatus.enCeregType = enCeregType;
}


VOS_UINT8 TAF_SDC_GetUssdOnImsSupportFlag(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stImsCapability.ucUssdOnImsSupportFlag;
}


VOS_VOID TAF_SDC_SetUssdOnImsSupportFlag(VOS_UINT8 ucUssdOnImsSupportFlag)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stImsConfigPara.stImsCapability.ucUssdOnImsSupportFlag = ucUssdOnImsSupportFlag;
}


VOS_VOID TAF_SDC_SetLCEnableFlg(
    VOS_UINT8                           ucLCEnableFlg
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stLcConfigPara.ucLCEnableFlg = ucLCEnableFlg;
}



VOS_UINT8 TAF_SDC_GetLCEnableFlg(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stLcConfigPara.ucLCEnableFlg;
}



VOS_VOID TAF_SDC_SetLCWorkCfg(
    TAF_NVIM_LC_WORK_CFG_ENUM_UINT8     enLCWorkCfg
)
{
    TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stLcConfigPara.enLCWorkCfg = enLCWorkCfg;
}



VOS_UINT8 TAF_SDC_GetLCWorkCfg(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stLcConfigPara.enLCWorkCfg;
}


VOS_UINT8 TAF_SDC_GetLCRatCombined(VOS_VOID)
{
    return TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stLcConfigPara.enRatCombined;
}


TAF_SDC_LC_CONFIG_PARA_STRU* TAF_SDC_GetLcConfigPara(VOS_VOID)
{
    return &(TAF_SDC_GetSdcCtx()->stMsCfgInfo.stMsCapInfo.stLcConfigPara);
}


#ifdef  __cplusplus
  #if  __cplusplus
  }
  #endif
#endif

