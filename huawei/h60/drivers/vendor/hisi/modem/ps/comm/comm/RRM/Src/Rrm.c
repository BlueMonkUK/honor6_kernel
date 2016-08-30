



/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "Rrm.h"
#include "RrmDebug.h"
#include "TtfNvInterface.h"
#include "NVIM_Interface.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_RRM_C

#if (FEATURE_ON == FEATURE_DSDS)

/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/
RRM_INFO_STRU                        g_stRrmInfo;


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/


VOS_UINT32 RRM_JudgeRegInfoIsReged
(
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT32                          ulPid,
    PS_BOOL_ENUM_UINT8                 *enIsSameModem,
    VOS_UINT16                         *usRegInfoIndex,
    PS_BOOL_ENUM_UINT8                 *enRegInfoIsReged
)
{

    VOS_UINT16                          usIndex;
    RRM_REG_INFO_STRU                  *pstRegInfoAddr = VOS_NULL_PTR;

    /* ��ʼ�� */
    *enIsSameModem      = PS_FALSE;
    *enRegInfoIsReged   = PS_FALSE;

    for (usIndex = 0; usIndex < RRM_REG_MAX_NUMBER; usIndex++)
    {
        pstRegInfoAddr = RRM_GetRegInfoAddr(usIndex);
        if (VOS_NULL_PTR != pstRegInfoAddr)
        {
            /* ��ǰREG INFO�Ƿ���ע���ж� */
            if (PS_TRUE == pstRegInfoAddr->enRegisteredFlg)
            {
                /* PID��ֵ�Ƿ���ͬ�ж� */
                if (ulPid == pstRegInfoAddr->ulPid)
                {
                      /* ��¼������ */
                    *usRegInfoIndex     = usIndex;

                    /* ����ʹ�ñ�ʶ */
                    *enRegInfoIsReged   = PS_TRUE;

                    /* ͬһ��MODEM�ж� */
                    if (enModemId == pstRegInfoAddr->enModemId)
                    {
                        *enIsSameModem = PS_TRUE;
                    }

                    return VOS_OK;
                }
            }
        }
    }


    return VOS_OK;
}




VOS_UINT32 RRM_JudgeRFIDIsUsed
(
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT16                          usRFIDValue,
    PS_BOOL_ENUM_UINT8                 *enIsSameModem,
    RRM_RFID_ENUM_UINT16               *enRFIDIndex,
    PS_BOOL_ENUM_UINT8                 *enRFIDIsUsed
)
{
    VOS_UINT16                          usIndex;
    RRM_RFID_INFO_STRU                 *pstRFIDInfoAddr = VOS_NULL_PTR;


    /* ��ʼ�� */
    *enIsSameModem                      = PS_FALSE;
    *enRFIDIsUsed                       = PS_FALSE;

    for (usIndex = 0; usIndex < RRM_RFID_BUTT; usIndex++)
    {
        pstRFIDInfoAddr = RRM_GetRFIDInfoAddr(usIndex);
        if (VOS_NULL_PTR != pstRFIDInfoAddr)
        {
            /* ��ǰRFID�Ƿ���ʹ���ж� */
            if (PS_TRUE == pstRFIDInfoAddr->enRFIDUsedFlg)
            {
                /* RFID��ֵ�Ƿ���ͬ�ж� */
                if (usRFIDValue == pstRFIDInfoAddr->usUsingRFIDValue)
                {
                    /* ��¼������ */
                    *enRFIDIndex  = usIndex;

                    /* ����ʹ�ñ�ʶ */
                    *enRFIDIsUsed = PS_TRUE;

                    /* ͬһ��MODEM */
                    if (enModemId == pstRFIDInfoAddr->enUsingModemId)
                    {
                        *enIsSameModem = PS_TRUE;
                    }

                    return VOS_OK;
                }
            }
        }
    }


    return VOS_OK;
}




VOS_UINT16 RRM_GetRFIDCfgValue
(
    MODEM_ID_ENUM_UINT16                enModemId,
    RRM_PS_RAT_TYPE_ENUM_UINT8          enRatType
)
{
    VOS_UINT16              usRFIDValue = RRM_RFID_DEFAULT_VALUE;


    switch (enRatType)
    {
        case RRM_PS_RAT_TYPE_GSM:
            usRFIDValue = RRM_GET_GSM_RFID(enModemId);
            break;

        case RRM_PS_RAT_TYPE_WCDMA:
            usRFIDValue = RRM_GET_WCDMA_RFID(enModemId);
            break;

        case RRM_PS_RAT_TYPE_TDS:
            usRFIDValue = RRM_GET_TDS_RFID(enModemId);
            break;

        case RRM_PS_RAT_TYPE_LTE:
            usRFIDValue = RRM_GET_LTE_RFID(enModemId);
            break;

        case RRM_PS_RAT_TYPE_1X:
            usRFIDValue = RRM_GET_CDMA_RFID(enModemId);
            break;

        case RRM_PS_RAT_TYPE_EVDO:
            usRFIDValue = RRM_GET_EVDO_RFID(enModemId);
            break;

        default:
            break;

    }


    return  usRFIDValue;
}




VOS_VOID RRM_SetRegInfo
(
    PS_BOOL_ENUM_UINT8                  enRegisteredFlg,
    RRM_PS_RAT_TYPE_ENUM_UINT8          enRatType,
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT32                          ulPid
)
{
    VOS_UINT16                          usIndex;
    RRM_REG_INFO_STRU                  *pstRegInfoAddr = VOS_NULL_PTR;


    for (usIndex = 0; usIndex < RRM_REG_MAX_NUMBER; usIndex++)
    {
        pstRegInfoAddr = RRM_GetRegInfoAddr(usIndex);
        if (VOS_NULL_PTR != pstRegInfoAddr)
        {
            if (PS_FALSE == pstRegInfoAddr->enRegisteredFlg)
            {
                /* ����REG��Ϣ */
                pstRegInfoAddr->enRegisteredFlg     = enRegisteredFlg;
                pstRegInfoAddr->enRatType           = enRatType;
                pstRegInfoAddr->enModemId           = enModemId;
                pstRegInfoAddr->ulPid               = ulPid;

                break;
            }
        }
    }

    if (RRM_REG_MAX_NUMBER == usIndex)
    {
        RRM_WARNING_LOG4(UEPS_PID_RRM, "Reg Info has been full.",
            enRegisteredFlg, enRatType, enModemId, ulPid);
    }


    return;
}
VOS_UINT32 RRM_SetRFIDUsedInfo
(
    PS_BOOL_ENUM_UINT8                  enRFIDUsedFlg,
    VOS_UINT16                          usUsingRFIDValue,
    MODEM_ID_ENUM_UINT16                enUsingModemId
)
{
    VOS_UINT16                          usIndex;
    RRM_RFID_INFO_STRU                 *pstRFIDInfoAddr = VOS_NULL_PTR;


    for (usIndex = 0; usIndex < RRM_RFID_BUTT; usIndex++)
    {
        pstRFIDInfoAddr = RRM_GetRFIDInfoAddr(usIndex);
        if (VOS_NULL_PTR != pstRFIDInfoAddr)
        {
            if (PS_FALSE == pstRFIDInfoAddr->enRFIDUsedFlg)
            {
                /* ����RFID��Ϣ */
                pstRFIDInfoAddr->enRFIDUsedFlg      = enRFIDUsedFlg;
                pstRFIDInfoAddr->usUsingRFIDValue   = usUsingRFIDValue;
                pstRFIDInfoAddr->enUsingModemId     = enUsingModemId;

                break;
            }
        }
    }

    if (RRM_RFID_BUTT == usIndex)
    {
        RRM_WARNING_LOG3(UEPS_PID_RRM, "RFID Used Info has been full.",
            enRFIDUsedFlg, usUsingRFIDValue, enUsingModemId);
        return VOS_ERR;
    }


    return VOS_OK;
}
VOS_VOID RRM_ClearRegInfo
(
    VOS_UINT16                          usRegInfoIndex
)
{
    RRM_REG_INFO_STRU                  *pstRegInfoAddr = VOS_NULL_PTR;


    pstRegInfoAddr = RRM_GetRegInfoAddr(usRegInfoIndex);
    if (VOS_NULL_PTR == pstRegInfoAddr)
    {
        RRM_WARNING_LOG1(UEPS_PID_RRM, "RRM_ClearRegInfo: pstRegInfoAddr is null.", usRegInfoIndex);
        return;
    }

    if (PS_TRUE == pstRegInfoAddr->enRegisteredFlg)
    {
        /* ���REG��Ϣ */
        pstRegInfoAddr->enRegisteredFlg     = PS_FALSE;
        pstRegInfoAddr->enRatType           = RRM_PS_RAT_TYPE_BUTT;
        pstRegInfoAddr->enModemId           = MODEM_ID_BUTT;
        pstRegInfoAddr->ulPid               = RRM_PID_DEFAULT_VALUE;
    }


    return;
}
VOS_VOID RRM_ClearRFIDUsedInfo
(
    RRM_RFID_ENUM_UINT16                enRFIDIndex
)
{
    RRM_RFID_INFO_STRU                 *pstRFIDInfoAddr = VOS_NULL_PTR;


    pstRFIDInfoAddr = RRM_GetRFIDInfoAddr(enRFIDIndex);
    if (VOS_NULL_PTR == pstRFIDInfoAddr)
    {
        RRM_ERROR_LOG1(UEPS_PID_RRM, "RRM_ClearRFIDUsedInfo: pstRFIDInfoAddr is null.", enRFIDIndex);
        return;
    }

    if (PS_TRUE == pstRFIDInfoAddr->enRFIDUsedFlg)
    {
        /* ���RFID��Ϣ */
        pstRFIDInfoAddr->enRFIDUsedFlg              = PS_FALSE;
        pstRFIDInfoAddr->enUsingModemId             = MODEM_ID_BUTT;
        pstRFIDInfoAddr->usUsingRFIDValue           = RRM_RFID_DEFAULT_VALUE;
        pstRFIDInfoAddr->stRFIDTimer.enRFIDTimerId  = RRM_RFID_BUTT;
        pstRFIDInfoAddr->stRFIDTimer.pstStaTHandle  = VOS_NULL_PTR;

    }


    return;
}
VOS_VOID RRM_InitModemCfgInfo(NV_MODEM_RF_SHARE_CFG_STRU *pstModemCfgInfo)
{
    /* �����Ϸ����ж� */
    if (VOS_NULL_PTR == pstModemCfgInfo)
    {
        RRM_ERROR_LOG(UEPS_PID_RRM, "RRM_InitModemInfo: pstModemCfgInfo is null.");
        return;
    }

    /* ��ʼ��MODEM������Ϣ */
    pstModemCfgInfo->usSupportFlag   = PS_FALSE;
    pstModemCfgInfo->usGSMRFID       = RRM_RFID_DEFAULT_VALUE;
    pstModemCfgInfo->usWCDMARFID     = RRM_RFID_DEFAULT_VALUE;
    pstModemCfgInfo->usTDSRFID       = RRM_RFID_DEFAULT_VALUE;
    pstModemCfgInfo->usLTERFID       = RRM_RFID_DEFAULT_VALUE;
    pstModemCfgInfo->usCDMARFID      = RRM_RFID_DEFAULT_VALUE;
    pstModemCfgInfo->usEVDORFID      = RRM_RFID_DEFAULT_VALUE;
    pstModemCfgInfo->usReserved      = RRM_RFID_DEFAULT_VALUE;


    return;
}



VOS_VOID RRM_InitRegInfo(VOS_VOID)
{
    VOS_UINT16                          usIndex;
    RRM_REG_INFO_STRU                  *pstRegInfoAddr = VOS_NULL_PTR;


    for (usIndex = 0; usIndex < RRM_REG_MAX_NUMBER; usIndex++)
    {
        pstRegInfoAddr = RRM_GetRegInfoAddr(usIndex);
        if (VOS_NULL_PTR != pstRegInfoAddr)
        {
            /* ��ʼ��REG��Ϣ */
            pstRegInfoAddr->enRegisteredFlg     = PS_FALSE;
            pstRegInfoAddr->enRatType           = RRM_PS_RAT_TYPE_BUTT;
            pstRegInfoAddr->enModemId           = MODEM_ID_BUTT;
            pstRegInfoAddr->ulPid               = RRM_PID_DEFAULT_VALUE;
        }
    }

    return;
}
VOS_VOID RRM_InitRFIDInfo(VOS_VOID)
{
    VOS_UINT32                          ulIndex;
    RRM_RFID_INFO_STRU                 *pstRFIDInfoAddr = VOS_NULL_PTR;


    for (ulIndex = 0; ulIndex < RRM_RFID_BUTT; ulIndex++)
    {
        pstRFIDInfoAddr = RRM_GetRFIDInfoAddr(ulIndex);
        if (VOS_NULL_PTR != pstRFIDInfoAddr)
        {
            /* ��ʼ��RFID��Ϣ */
            pstRFIDInfoAddr->enRFIDUsedFlg              = PS_FALSE;
            pstRFIDInfoAddr->enUsingModemId             = MODEM_ID_BUTT;
            pstRFIDInfoAddr->usUsingRFIDValue           = RRM_RFID_DEFAULT_VALUE;
            pstRFIDInfoAddr->stRFIDTimer.enRFIDTimerId  = RRM_RFID_BUTT;
            pstRFIDInfoAddr->stRFIDTimer.pstStaTHandle  = VOS_NULL_PTR;
        }
    }


    return;
}
VOS_VOID RRM_InitModemInfo(VOS_VOID)
{
    VOS_UINT32                          ulRet;
    VOS_UINT16                          usIndex;
    NV_MODEM_RF_SHARE_CFG_STRU          stModemRFShareCfg;
    NV_MODEM_RF_SHARE_CFG_STRU         *pstModemCfgInfoAddr = VOS_NULL_PTR;


    for (usIndex = 0; usIndex < MODEM_ID_BUTT; usIndex++)
    {
        PS_MEM_SET(&stModemRFShareCfg, 0, sizeof(stModemRFShareCfg));

        /* ��ȡModem������Ϣ��ַ */
        pstModemCfgInfoAddr = RRM_GetModemCfgInfoAddr(usIndex);
        if (VOS_NULL_PTR != pstModemCfgInfoAddr)
        {
            /* ��ʼ��Modem������Ϣ */
            RRM_InitModemCfgInfo(pstModemCfgInfoAddr);

            /* ��ȡModem��Ƶ����nv�� */
            ulRet = NV_ReadEx(usIndex,
                              en_NV_MODEM_RF_SHARE_CFG,
                              &stModemRFShareCfg,
                              sizeof(NV_MODEM_RF_SHARE_CFG_STRU));
            if (VOS_OK != ulRet)
            {
                RRM_ERROR_LOG1(UEPS_PID_RRM, "RRM_InitModemInfo: Read Nv Fail.", usIndex);
                continue;
            }

            /* ����MODEM��������Ϣ */
            PS_MEM_CPY(pstModemCfgInfoAddr, &stModemRFShareCfg, sizeof(NV_MODEM_RF_SHARE_CFG_STRU));
        }
    }

    return;
}



VOS_VOID  RRM_Init(VOS_VOID)
{
    /* ��ʼ��RRMģ��Modem��Ϣ */
    RRM_InitModemInfo();

    /* ��ʼ��RRMģ��RFID��Ϣ */
    RRM_InitRFIDInfo();

    /* ��ʼ��RRMģ��REG��Ϣ */
    RRM_InitRegInfo();

    /* ��ʼ��RRM������Ϣ */
    RRM_InitDebugInfo();

    return;
}



VOS_VOID RRM_ShowInfo(VOS_VOID)
{
    VOS_UINT16                          usIndex;
    RRM_RFID_INFO_STRU                 *pstRFIDInfoAddr         = VOS_NULL_PTR;
    RRM_REG_INFO_STRU                  *pstRegInfoAddr          = VOS_NULL_PTR;
    NV_MODEM_RF_SHARE_CFG_STRU         *pstModemCfgInfoAddr     = VOS_NULL_PTR;


    for (usIndex = 0; usIndex < RRM_RFID_BUTT; usIndex++)
    {
        pstRFIDInfoAddr = RRM_GetRFIDInfoAddr(usIndex);
        if (VOS_NULL_PTR != pstRFIDInfoAddr)
        {
            vos_printf("========RFID info for Index %d ========\n", usIndex);
            vos_printf("RFIDʹ��״̬:             %d \n", pstRFIDInfoAddr->enRFIDUsedFlg);
            vos_printf("RFIDʹ�õ�MODEM:          %d \n", pstRFIDInfoAddr->enUsingModemId);
            vos_printf("RFIDʹ�õ�ֵ:             %d \n", pstRFIDInfoAddr->usUsingRFIDValue);
        }
    }
    vos_printf("\r\n");

    for (usIndex = 0; usIndex < RRM_REG_MAX_NUMBER; usIndex++)
    {
        pstRegInfoAddr = RRM_GetRegInfoAddr(usIndex);
        if (VOS_NULL_PTR != pstRegInfoAddr)
        {
            vos_printf("========REG info for Index %d ========\n", usIndex);
            vos_printf("REG��Ϣ״̬:             %d \n", pstRegInfoAddr->enRegisteredFlg);
            vos_printf("REG��Ϣ��RAT TYPE:       %d \n", pstRegInfoAddr->enRatType);
            vos_printf("REG��Ϣ��MODEM:          %d \n", pstRegInfoAddr->enModemId);
            vos_printf("REG��Ϣ��PID:            %d \n", pstRegInfoAddr->ulPid);
        }
    }
    vos_printf("\r\n");

    for (usIndex = 0; usIndex < MODEM_ID_BUTT; usIndex++)
    {
        pstModemCfgInfoAddr = RRM_GetModemCfgInfoAddr(usIndex);
        if (VOS_NULL_PTR != pstModemCfgInfoAddr)
        {
            vos_printf("========MODEM info for Index %d ========\n", usIndex);
            vos_printf("MODEM�Ƿ�֧�ָ�����:      %d \n", pstModemCfgInfoAddr->usSupportFlag);
            vos_printf("MODEM��GSM RFIDֵ:        %d \n", pstModemCfgInfoAddr->usGSMRFID);
            vos_printf("MODEM��WCDMA RFIDֵ:      %d \n", pstModemCfgInfoAddr->usWCDMARFID);
            vos_printf("MODEM��TDS RFIDֵ:        %d \n", pstModemCfgInfoAddr->usTDSRFID);
            vos_printf("MODEM��LTE RFIDֵ:        %d \n", pstModemCfgInfoAddr->usLTERFID);
            vos_printf("MODEM��CDMA RFIDֵ:       %d \n", pstModemCfgInfoAddr->usCDMARFID);
            vos_printf("MODEM��EVDO RFIDֵ:       %d \n", pstModemCfgInfoAddr->usEVDORFID);
        }
    }
    vos_printf("\r\n");

    return;
}


#endif /* FEATURE_ON == FEATURE_DSDS */

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

