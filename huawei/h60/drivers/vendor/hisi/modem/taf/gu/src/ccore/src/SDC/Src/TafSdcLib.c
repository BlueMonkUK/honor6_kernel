

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "PsLib.h"
#include "pslog.h"
#include "PsCommonDef.h"
#include "UsimPsInterface.h"
#include "TafSdcLib.h"
#include "zlib.h"
#include "NVIM_Interface.h"

#include "NasErrorLog.h"

#ifdef  __cplusplus
  #if  __cplusplus
  extern "C"{
  #endif
#endif

/*****************************************************************************
  2 ��������
*****************************************************************************/

#define THIS_FILE_ID PS_FILE_ID_TAF_SDC_LIB_C

/*****************************************************************************
  3 ���Ͷ���
*****************************************************************************/

/*****************************************************************************
  4 ��������
*****************************************************************************/

/*****************************************************************************
  5 ��������
*****************************************************************************/
/* ����AT^REJINFO�������ϱ����ƣ�����һ���ֽڵĵ���������BIT */
/* ����AT^PLMNSELERLST�������ϱ����ƣ�����һ���ֽڵĵ��������BIT */
/* ����^MIPICLK�������ϱ����ƣ����ڵ�һ���ֽڵ����һ��Bit */
TAF_SDC_RPT_CMD_INDEX_ENUM_UINT8            g_aenTafSdcCurcRptCmdTable[] =
{
    TAF_SDC_RPT_CMD_MODE,        TAF_SDC_RPT_CMD_RSSI,        TAF_SDC_RPT_CMD_REJINFO,     TAF_SDC_RPT_CMD_SRVST,
    TAF_SDC_RPT_CMD_PLMNSELEINFO,TAF_SDC_RPT_CMD_SIMST,       TAF_SDC_RPT_CMD_TIME,        TAF_SDC_RPT_CMD_MIPICLK,
    TAF_SDC_RPT_CMD_ANLEVEL,     TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_SMMEMFULL,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_CTZV,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_DSFLOWRPT,   TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_ORIG,        TAF_SDC_RPT_CMD_CONF,        TAF_SDC_RPT_CMD_CONN,        TAF_SDC_RPT_CMD_CEND,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_STIN,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_CERSSI,      TAF_SDC_RPT_CMD_LWCLASH,     TAF_SDC_RPT_CMD_XLEMA,       TAF_SDC_RPT_CMD_ACINFO,
    TAF_SDC_RPT_CMD_PLMN,        TAF_SDC_RPT_CMD_CALLSTATE,   TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT
};

TAF_SDC_RPT_CMD_INDEX_ENUM_UINT8            g_aenTafSdcUnsolicitedRptCmdTable[] =
{
    TAF_SDC_RPT_CMD_MODE,        TAF_SDC_RPT_CMD_RSSI,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_SRVST,
    TAF_SDC_RPT_CMD_CREG,        TAF_SDC_RPT_CMD_SIMST,       TAF_SDC_RPT_CMD_TIME,        TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_ANLEVEL,     TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_CTZV,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_DSFLOWRPT,   TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_CUSD,        TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_CSSI,
    TAF_SDC_RPT_CMD_CSSU,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_CERSSI,      TAF_SDC_RPT_CMD_LWURC,       TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_CUUS1U,
    TAF_SDC_RPT_CMD_CUUS1I,      TAF_SDC_RPT_CMD_CGREG,       TAF_SDC_RPT_CMD_CEREG,       TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,
    TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT,        TAF_SDC_RPT_CMD_BUTT
};


/*****************************************************************************
  6 ��������
*****************************************************************************/

#if (FEATURE_ON == FEATURE_LTE)

VOS_UINT32  TAF_SDC_IsPlatformSupportLte( VOS_VOID )
{
    VOS_UINT32                          i;
    TAF_SDC_PLATFORM_RAT_CAP_STRU      *pstPlatformRatCap = VOS_NULL_PTR;

    pstPlatformRatCap = TAF_SDC_GetPlatformRatCap();

    /* LTE�Ƿ���ƽ̨���뼼�������б��� */
    for (i = 0 ; i < pstPlatformRatCap->ucRatNum; i++)
    {
        if (TAF_SDC_PLATFORM_RAT_LTE == pstPlatformRatCap->aenRatList[i])
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}
#endif
VOS_UINT32  TAF_SDC_IsPlatformSupportUtran( VOS_VOID )
{
    VOS_UINT32                          i;
    TAF_SDC_PLATFORM_RAT_CAP_STRU      *pstPlatformRatCap = VOS_NULL_PTR;

    pstPlatformRatCap = TAF_SDC_GetPlatformRatCap();

    /* UTRAN�Ƿ���ƽ̨���뼼�������б��� */
    for (i = 0 ; i < pstPlatformRatCap->ucRatNum; i++)
    {
        if ((TAF_SDC_PLATFORM_RAT_WCDMA == pstPlatformRatCap->aenRatList[i])
         || (TAF_SDC_PLATFORM_RAT_TDS == pstPlatformRatCap->aenRatList[i]))
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}
VOS_UINT32  TAF_SDC_IsPlatformSupportUtranFdd( VOS_VOID )
{
    VOS_UINT32                          i;
    TAF_SDC_PLATFORM_RAT_CAP_STRU      *pstPlatformRatCap = VOS_NULL_PTR;

    pstPlatformRatCap = TAF_SDC_GetPlatformRatCap();

    /* UTRAN�Ƿ���ƽ̨���뼼�������б��� */
    for (i = 0 ; i < pstPlatformRatCap->ucRatNum; i++)
    {
        if (TAF_SDC_PLATFORM_RAT_WCDMA == pstPlatformRatCap->aenRatList[i])
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}

#if (FEATURE_ON == FEATURE_UE_MODE_TDS)
VOS_UINT32  TAF_SDC_IsPlatformSupportUtranTdd( VOS_VOID )
{
    VOS_UINT32                          i;
    TAF_SDC_PLATFORM_RAT_CAP_STRU      *pstPlatformRatCap = VOS_NULL_PTR;

    pstPlatformRatCap = TAF_SDC_GetPlatformRatCap();

    /* UTRAN�Ƿ���ƽ̨���뼼�������б��� */
    for (i = 0 ; i < pstPlatformRatCap->ucRatNum; i++)
    {
        if (TAF_SDC_PLATFORM_RAT_TDS == pstPlatformRatCap->aenRatList[i])
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}
#endif
VOS_UINT32  TAF_SDC_IsPlatformSupportGsm( VOS_VOID )
{
    VOS_UINT32                          i;
    TAF_SDC_PLATFORM_RAT_CAP_STRU      *pstPlatformRatCap = VOS_NULL_PTR;

    pstPlatformRatCap = TAF_SDC_GetPlatformRatCap();

    /* GSM�Ƿ���ƽ̨���뼼�������б��� */
    for (i = 0 ; i < pstPlatformRatCap->ucRatNum; i++)
    {
        if (TAF_SDC_PLATFORM_RAT_GSM == pstPlatformRatCap->aenRatList[i])
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}
VOS_VOID  TAF_SDC_LogUnsolicitedRptInfo(
    TAF_SDC_RPT_CMD_INDEX_ENUM_UINT8            enRptCmdIndex,
    TAF_SDC_CURC_RPT_CTRL_STRU                 *pstCurcRptCtrl,
    TAF_SDC_UNSOLICITED_RPT_CTRL_STRU          *pstUnsolicitedRptCtrl
)
{
    TAF_OAM_UNSOLICITED_RPT_INFO_STRU  *pstMsg = VOS_NULL_PTR;

    pstMsg = (TAF_OAM_UNSOLICITED_RPT_INFO_STRU*)PS_MEM_ALLOC(WUEPS_PID_TAF,
                                         sizeof(TAF_OAM_UNSOLICITED_RPT_INFO_STRU));

    if (VOS_NULL_PTR == pstMsg)
    {
        return;
    }

    PS_MEM_SET(pstMsg, 0x00, sizeof(TAF_OAM_UNSOLICITED_RPT_INFO_STRU));

    pstMsg->ulSenderCpuId   = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->ulSenderPid     = WUEPS_PID_TAF;
    pstMsg->ulReceiverPid   = WUEPS_PID_TAF;
    pstMsg->ulLength        = sizeof(TAF_OAM_UNSOLICITED_RPT_INFO_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMsg->ulMsgId         = TAF_OAM_UNSOLICITED_RPT_INFO_IND;
    pstMsg->ulRptCmdIndex   = enRptCmdIndex;
    PS_MEM_CPY(pstMsg->aucCurcRptCfg, pstCurcRptCtrl->aucRptCfg, sizeof(pstMsg->aucCurcRptCfg));
    PS_MEM_CPY(pstMsg->aucUnsolicitedRptCfg, pstUnsolicitedRptCtrl->aucRptCfg, sizeof(pstMsg->aucUnsolicitedRptCfg));

    OM_TraceMsgHook(pstMsg);

    PS_MEM_FREE(WUEPS_PID_TAF, pstMsg);

    return;
}



VOS_UINT32 TAF_SDC_CheckRptCmdStatus(
    VOS_UINT8                                  *pucRptCfg,
    TAF_SDC_CMD_RPT_CTRL_TYPE_ENUM_UINT8        enRptCtrlType,
    TAF_SDC_RPT_CMD_INDEX_ENUM_UINT8            enRptCmdIndex
)
{
    TAF_SDC_RPT_CMD_INDEX_ENUM_UINT8   *pulRptCmdTblPtr       = VOS_NULL_PTR;
    VOS_UINT32                          ulRptCmdTblSize;
    VOS_UINT8                           ucTableIndex;
    VOS_UINT32                          ulOffset;
    VOS_UINT8                           ucBit;

    /* �����ϱ�������������Ĭ�������ϱ� */
    if (enRptCmdIndex >= TAF_SDC_RPT_CMD_BUTT)
    {
        return VOS_TRUE;
    }

    TAF_SDC_LogUnsolicitedRptInfo(enRptCmdIndex, TAF_SDC_GetCurcRptCtrl(), TAF_SDC_GetUnsolicitedRptCtrl());

    /* �����ϱ��ܿ�������д����Ĭ�������ϱ� */
    if (TAF_SDC_CMD_RPT_CTRL_BUTT == enRptCtrlType)
    {
        return VOS_TRUE;
    }

    if (TAF_SDC_CMD_RPT_CTRL_BY_CURC == enRptCtrlType)
    {
        pulRptCmdTblPtr = TAF_SDC_GET_CURC_RPT_CTRL_STATUS_MAP_TBL_PTR();
        ulRptCmdTblSize = TAF_SDC_GET_CURC_RPT_CTRL_STATUS_MAP_TBL_SIZE();
    }
    else
    {
        pulRptCmdTblPtr = TAF_SDC_GET_UNSOLICITED_RPT_CTRL_STATUS_MAP_TBL_PTR();
        ulRptCmdTblSize = TAF_SDC_GET_UNSOLICITED_RPT_CTRL_STATUS_MAP_TBL_SIZE();
    }

    for (ucTableIndex = 0; ucTableIndex < ulRptCmdTblSize; ucTableIndex++)
    {
        if (enRptCmdIndex == pulRptCmdTblPtr[ucTableIndex])
        {
            break;
        }
    }

    /* ��ȫ�ֱ����е�Bitλ�Ա� */
    if (ulRptCmdTblSize != ucTableIndex)
    {
        /* �����û����õ��ֽ�����Bitӳ������෴, ���ȷ�תBitλ */
        ulOffset        = TAF_SDC_RPT_CFG_MAX_SIZE - (ucTableIndex / 8) - 1;
        ucBit           = (VOS_UINT8)(ucTableIndex % 8);

        return (VOS_UINT32)((pucRptCfg[ulOffset] >> ucBit) & 0x1);
    }

    return VOS_TRUE;
}


VOS_UINT32 TAF_SDC_UpdateRptCmdStatus(
    TAF_SDC_RPT_CMD_INDEX_ENUM_UINT8            enRptCmdIndex,
    VOS_UINT32                                  ulRptCmdStatus
)
{
    TAF_SDC_RPT_CMD_INDEX_ENUM_UINT8   *pulRptCmdTblPtr     = VOS_NULL_PTR;
    TAF_SDC_UNSOLICITED_RPT_CTRL_STRU  *pstUnsolicitedRpt   = VOS_NULL_PTR;
    VOS_UINT32                          ulRptCmdTblSize;
    VOS_UINT8                           ucTableIndex;
    VOS_UINT32                          ulOffset;
    VOS_UINT8                           ucBit;
    VOS_UINT8                           ucRptFlg;

    ucRptFlg    = 0x01;

    /* �����ϱ�������������Ĭ�������ϱ� */
    if (enRptCmdIndex >= TAF_SDC_RPT_CMD_BUTT)
    {
        return VOS_FALSE;
    }

    pulRptCmdTblPtr     = TAF_SDC_GET_UNSOLICITED_RPT_CTRL_STATUS_MAP_TBL_PTR();
    ulRptCmdTblSize     = TAF_SDC_GET_UNSOLICITED_RPT_CTRL_STATUS_MAP_TBL_SIZE();
    pstUnsolicitedRpt   = TAF_SDC_GetUnsolicitedRptCtrl();

    for (ucTableIndex = 0; ucTableIndex < ulRptCmdTblSize; ucTableIndex++)
    {
        if (enRptCmdIndex == pulRptCmdTblPtr[ucTableIndex])
        {
            break;
        }
    }

    /* ��ȫ�ֱ����е�Bitλ�Ա� */
    if (ulRptCmdTblSize != ucTableIndex)
    {
        /* �����û����õ��ֽ�����Bitӳ������෴, ���ȷ�תBitλ */
        ulOffset        = TAF_SDC_RPT_CFG_MAX_SIZE - (ucTableIndex / 8) - 1;
        ucBit           = (VOS_UINT8)(ucTableIndex % 8);

        ucRptFlg = (VOS_UINT8)(ucRptFlg << ucBit);

        /* �ر������ϱ�ʱ */
        if (VOS_FALSE == ulRptCmdStatus)
        {
            (pstUnsolicitedRpt->aucRptCfg[ulOffset]) &= (~ucRptFlg);
        }
        else
        {
            (pstUnsolicitedRpt->aucRptCfg[ulOffset]) |= ucRptFlg;
        }

    }

    return VOS_TRUE;
}



VOS_UINT32 TAF_SDC_ConvertOamSendPid(
    VOS_UINT32                          ulConvertedSendPid
)
{

    if ((I0_WUEPS_PID_USIM == ulConvertedSendPid)
     || (I1_WUEPS_PID_USIM == ulConvertedSendPid))
    {
        return WUEPS_PID_USIM;
    }

    if ((I0_MAPS_STK_PID == ulConvertedSendPid)
     || (I1_MAPS_STK_PID == ulConvertedSendPid))
    {
        return MAPS_STK_PID;
    }

    if ((I0_MAPS_PB_PID == ulConvertedSendPid)
     || (I1_MAPS_PB_PID == ulConvertedSendPid))
    {
        return MAPS_PB_PID;
    }


    if ((I0_MAPS_PIH_PID == ulConvertedSendPid)
     || (I1_MAPS_PIH_PID == ulConvertedSendPid))
    {
        return MAPS_PIH_PID;
    }

    return ulConvertedSendPid;
}



VOS_UINT32 TAF_SDC_GetCsSrvExistFlg(VOS_VOID)
{
    /* ����е绰ҵ����ڣ���Ϊ��ǰ��CSҵ�� */
    if (VOS_TRUE == TAF_SDC_GetCsCallExistFlg())
    {
        return VOS_TRUE;
    }

    /* �����CS�����ҵ����ڣ���Ϊ��ǰ��CSҵ�� */
    if (VOS_TRUE == TAF_SDC_GetCsSmsSrvExistFlg())
    {
        return VOS_TRUE;
    }

    /* �����CS�򲹳�ҵ����ڣ���Ϊ��ǰ��CSҵ�� */
    if (VOS_TRUE == TAF_SDC_GetCsSsSrvExistFlg())
    {
        return VOS_TRUE;
    }

    /* �����������Ϊû��CSҵ�� */
    return VOS_FALSE;

}



VOS_UINT32 TAF_SDC_GetImsSupportFlag(VOS_VOID)
{
    /* ���֧��GSM IMS����Ϊ��ǰ֧��IMS */
    if (VOS_TRUE == TAF_SDC_GetGsmImsSupportFlag())
    {
        return VOS_TRUE;
    }

    /* ���֧��UTRAN IMS����Ϊ��ǰ֧��IMS */
    if (VOS_TRUE == TAF_SDC_GetUtranImsSupportFlag())
    {
        return VOS_TRUE;
    }

    /* ���֧��LTE IMS����Ϊ��ǰ֧��IMS */
    if (VOS_TRUE == TAF_SDC_GetLteImsSupportFlag())
    {
        return VOS_TRUE;
    }

    /* �����������Ϊ��ǰ��֧��IMS */
    return VOS_FALSE;

}




#if (FEATURE_ON == FEATURE_PTM)

VOS_UINT32 TAF_SDC_IsErrLogNeedRecord(VOS_UINT16 usLevel)
{
    /* Log���عرգ�����Ҫ�ϱ� */
    if (0 == TAF_SDC_GetErrlogCtrlFlag())
    {
        return VOS_FALSE;
    }

    /* ģ��log����usLevel�����û����õ�log�ϱ������usLevel��Ч������Ҫ�ϱ� */
    if ((TAF_SDC_GetErrlogAlmLevel() < usLevel)
     || (NAS_ERR_LOG_CTRL_LEVEL_NULL == usLevel))
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}
#endif


VOS_UINT32  TAF_CalcTime(
                VOS_UINT32  ulFirTime,
                VOS_UINT32  ulSecTime,
                VOS_UINT32 *pulTotalTime
)
{
    if (ulFirTime > ulSecTime)
    {
        *pulTotalTime = 0;
        return VOS_ERR;
    }

    *pulTotalTime = ulSecTime - ulFirTime;

    return VOS_OK;
}


VOS_UINT32 TAF_SDC_IsLteOnlyMode(
    TAF_SDC_RAT_PRIO_STRU              *pstRatOrder
)
{
    if ( (1                        == pstRatOrder->ucRatNum)
      && (TAF_SDC_SYS_MODE_LTE     == pstRatOrder->aenRatPrio[0]) )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 TAF_SDC_IsUsimStausValid(VOS_VOID)
{
    VOS_UINT8                           ucSimCsRegStatus;
    VOS_UINT8                           ucSimPsRegStatus;
#if (FEATURE_ON == FEATURE_LTE)
    TAF_SDC_RAT_PRIO_STRU              *pstRatPrio = VOS_NULL_PTR;

    pstRatPrio       = TAF_SDC_GetMsPrioRatList();
#endif

    ucSimCsRegStatus = TAF_SDC_GetSimCsRegStatus();
    ucSimPsRegStatus = TAF_SDC_GetSimPsRegStatus();

    if ((VOS_FALSE == ucSimPsRegStatus)
     && (VOS_FALSE == ucSimCsRegStatus))
    {
        return VOS_FALSE;
    }

    /* �޿�ʱ����VOS_FALSE����Ч */
    if (TAF_SDC_USIM_STATUS_NO_PRESENT == TAF_SDC_GetSimStatus())
    {
        return VOS_FALSE;
    }

    if (( VOS_FALSE == TAF_SDC_GetPsAttachAllowFlg())
     && ( VOS_FALSE == ucSimCsRegStatus))
    {
        return VOS_FALSE;
    }

    if (( VOS_FALSE == TAF_SDC_GetCsAttachAllowFlg())
     && ( VOS_FALSE == ucSimPsRegStatus))
    {
        return VOS_FALSE;
    }

#if (FEATURE_ON == FEATURE_LTE)

    /* LTE��ģ�����PS����Ч��ʱ��Ҳ��Ϊ��ʱ����Ч */
    if ((VOS_TRUE == TAF_SDC_IsLteOnlyMode(pstRatPrio))
     && (VOS_FALSE == ucSimPsRegStatus))
    {
        return VOS_FALSE;
    }
#endif
    return VOS_TRUE;

}


#if (FEATURE_ON == FEATURE_LTE)

VOS_UINT32 TAF_SDC_IsImsVoiceSupportedInUE(VOS_VOID)
{
    VOS_UINT32                          ulImsSupportFlag;

    ulImsSupportFlag    = TAF_SDC_GetImsSupportFlag();

    /* ��ǰIMS��RAT NV��֧�ֻ�������ѡ��ΪCS ONLY�����粻֧��IMSʱ ��ΪIMS VOICE������*/
    if ((TAF_SDC_VOICE_DOMAIN_CS_ONLY       == TAF_SDC_GetVoiceDomain())
     || (VOS_FALSE                          == ulImsSupportFlag))
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}
VOS_UINT32 TAF_SDC_IsImsVoiceSupportedInNW(VOS_VOID)
{
    TAF_SDC_NETWORK_CAP_INFO_STRU      *pstCurNwCapInfo   = VOS_NULL_PTR;

    if (TAF_SDC_SYS_MODE_LTE == TAF_SDC_GetSysMode())
    {
        pstCurNwCapInfo = TAF_SDC_GetLteNwCapInfo();
    }
    else
    {
        pstCurNwCapInfo = TAF_SDC_GetGuNwCapInfo();
    }

    if (TAF_SDC_NW_IMS_VOICE_NOT_SUPPORTED == pstCurNwCapInfo->enNwImsVoCap)
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}



#endif


VOS_VOID TAF_SDC_PlmnId2Bcd(
    TAF_SDC_PLMN_ID_STRU               *pstPlmnId
)
{
    VOS_UINT8                           ucI;
    VOS_UINT32                          ulTmp;
    VOS_UINT8                           aucTmp[4];

    ulTmp            = pstPlmnId->ulMcc;
    pstPlmnId->ulMcc = 0;
    for ( ucI = 0 ; ucI < 3 ; ucI++ )
    {
        aucTmp[ucI]     = ulTmp & 0x0f;
        ulTmp         >>=  8;
    }
    pstPlmnId->ulMcc = ((VOS_UINT32)aucTmp[0] << 8)
                      |((VOS_UINT32)aucTmp[1] << 4)
                      | aucTmp[2];

    ulTmp            = pstPlmnId->ulMnc;
    pstPlmnId->ulMnc = 0;
    for ( ucI = 0 ; ucI < 3 ; ucI++ )
    {
        aucTmp[ucI]     = ulTmp & 0x0f;
        ulTmp         >>=  8;
    }

    if(0xf == aucTmp[2])
    {
        pstPlmnId->ulMnc = ((VOS_UINT32)aucTmp[0] << 4)
                          |((VOS_UINT32)aucTmp[1] )
                          |0xf00;
    }
    else
    {
        pstPlmnId->ulMnc = ((VOS_UINT32)aucTmp[0] << 8)
                          |((VOS_UINT32)aucTmp[1] << 4)
                          | aucTmp[2];
    }
    return;
}


VOS_UINT32 TAF_SDC_IsConfigCLInterWork(VOS_VOID)
{
    if (VOS_FALSE == TAF_SDC_GetLCEnableFlg())
    {
        return VOS_FALSE;
    }

    if ( TAF_NVIM_LC_RAT_COMBINED_CL != TAF_SDC_GetLCRatCombined() )
    {
        return VOS_FALSE;
    }

    if (TAF_NVIM_LC_INTER_WORK == TAF_SDC_GetLCWorkCfg())
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}

#ifdef  __cplusplus
  #if  __cplusplus
  }
  #endif
#endif

