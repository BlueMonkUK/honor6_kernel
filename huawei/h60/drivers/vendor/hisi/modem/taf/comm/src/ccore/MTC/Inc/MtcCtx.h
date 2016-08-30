

#ifndef __MTCCTX_H__
#define __MTCCTX_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "Ps.h"
#include "pslog.h"
#include "MtcRrcInterface.h"
#include "MtcMmaInterface.h"
#include "TafMtcInterface.h"
#include "MtaMtcInterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#if (VOS_WIN32 == VOS_OS_VER)
#define MTC_WARNING_LOG(string)                 vos_printf("%s \r\n", string)
#define MTC_WARNING1_LOG(string, para1)         vos_printf("%s %d.\r\n", string, para1)
#define MTC_WARNING2_LOG(string, para1, para2)  vos_printf("%s %d, %d.\r\n", string, para1, para2)

#define MTC_ERROR_LOG(string)                   vos_printf("%s \r\n", string)
#define MTC_ERROR1_LOG(string, para1)           vos_printf("%s %d.\r\n", string, para1)
#define MTC_ERROR2_LOG(string, para1, para2)    vos_printf("%s %d, %d.\r\n", string, para1, para2)
#else
#define MTC_WARNING_LOG(string)                 PS_LOG(UEPS_PID_MTC, 0, PS_PRINT_WARNING,  string)
#define MTC_WARNING1_LOG(string, para1)         PS_LOG1(UEPS_PID_MTC, 0, PS_PRINT_WARNING,  string, (VOS_INT32)para1)
#define MTC_WARNING2_LOG(string, para1, para2)  PS_LOG2(UEPS_PID_MTC, 0, PS_PRINT_WARNING,  string, (VOS_INT32)para1, (VOS_INT32)para2)

#define MTC_ERROR_LOG(string)                   PS_LOG(UEPS_PID_MTC, 0, PS_PRINT_ERROR,  string)
#define MTC_ERROR1_LOG(string, para1)           PS_LOG1(UEPS_PID_MTC, 0, PS_PRINT_ERROR,  string, (VOS_INT32)para1)
#define MTC_ERROR2_LOG(string, para1, para2)    PS_LOG2(UEPS_PID_MTC, 0, PS_PRINT_ERROR,  string, (VOS_INT32)para1, (VOS_INT32)para2)
#endif

/* ��BIT n��λ */
#define MTC_SET_BIT(n)                  (0x01 << n)
#define MTC_SET_BIT64(n)                ((VOS_UINT64)1 << (n))

#define MTC_INTRUSION_CFG_BIT           MTC_SET_BIT(0)                          /* NV�б�ʶ���ſ��Ƶ�bitλ */

#define MTC_NOTCH_CFG_BIT               MTC_SET_BIT(1)                          /* NV�б�ʶnotch���Ƶ�bitλ */

#define MTC_RSE_CFG_BIT                 MTC_SET_BIT(4)                          /* NV�б�ʶRSE���Ƶ�bitλ */


#define MTC_PS_AUTOMATIC_START              (0)                                 /* ֵ��NAS_MMA_AUTOMATIC_START����һ�� */
#define MTC_PS_MMA_MANUAL_START             (1)                                 /* ֵ��NAS_MMA_MANUAL_START����һ��  */




#define MTC_NARROW_BAND_DCS_CFG_BIT     MTC_SET_BIT(2)                          /* NV�б�ʶnarrow band dcs���Ƶ�bitλ */

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

enum MTC_CFG_ENUM
{
    MTC_CFG_DISABLE                     = 0x00,                                 /* ���ò�ʹ�� */
    MTC_CFG_ENABLE                      = 0x01,                                 /* ����ʹ�� */
    MTC_CFG_BUTT
};
typedef VOS_UINT8  MTC_CFG_ENUM_UINT8;


enum MTC_CS_SERVICE_STATE_ENUM
{
    MTC_CS_NO_SERVICE         = 0x00,                                           /* CS����ҵ�� */
    MTC_CS_IN_SERVICE,                                                          /* CS�����ҵ�� */

    MTC_CS_SERVICE_STATE_BUTT
};
typedef VOS_UINT8 MTC_CS_SERVICE_STATE_ENUM_UINT8;


enum MTC_OPERATOR_CUST_SOLUTION_ENUM
{
    MTC_OPERATOR_CUST_NONE          = 0x00,                                     /* ����Ӫ�̶��Ʒ��� */
    MTC_OPERATOR_CUST_CMCC_SVLTE    = 0x01,                                     /* ���ƶ�����SVLTE���� */
    MTC_OPERATOR_CUST_CT_LC         = 0x02,                                     /* �й�����C+L���� */

    MTC_OPERATOR_CUST_BUTT
};
typedef VOS_UINT8 MTC_OPERATOR_CUST_SOLUTION_ENUM_UINT8;
enum MTC_PS_TRANSFER_CAUSE_ENUM
{
    MTC_PS_TRANSFER_CAUSE_AREA_LOST                 = 0,

    MTC_PS_TRANSFER_CAUSE_AREA_AVALIABLE            = 1,

    MTC_PS_TRANSFER_CAUSE_BUTT
};
typedef VOS_UINT8 MTC_PS_TRANSFER_CAUSE_ENUM_UINT8;

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
    VOS_UINT8                           ucIntrusionCfg      : 1;
    VOS_UINT8                           ucNotchBypass       : 1;
    VOS_UINT8                           ucNarrowBandDcsCfg  : 1;
    VOS_UINT8                           ucTlRfCtrlCfg       : 1;
    VOS_UINT8                           ucReserved          : 4;

    VOS_UINT8                           aucAdditonCfg[3];

} MTC_RF_INTRUSION_CFG_STRU;


typedef struct
{
    MTC_NV_RF_INTRUSION_CFG_STRU        stIntrusionCfg;                         /* ����RF INTRUSION NV������Ϣ */
    PS_BOOL_ENUM_UINT8                  enNotchEnableFlag;

    PS_BOOL_ENUM_UINT8                  enNarrowDcsEnableFlag;                  /* ��¼NarrowDcs״̬ */

    PS_BOOL_ENUM_UINT8                  enTlRfCtrlEnableFlag;

    PS_BOOL_ENUM_UINT8                  enRseCfg;                               /* ��¼�Ѿ��·�����RSE���� */
}MTC_INTRUSION_CTX_STRU;
typedef struct
{
    VOS_UINT8                           ucCallSrvExistFlg;                      /* Modem�Ƿ���ں��з��� */
    VOS_UINT8                           ucSmsSrvExistFlg;                       /* Modem�Ƿ���ڶ���ҵ�� */
    VOS_UINT8                           ucSsSrvExistFlg;                        /* Modem�Ƿ���ڲ���ҵ�� */
    VOS_UINT8                           aucReserved1[1];
}MTC_MODEM_SERVICE_INFO_STRU;
typedef struct
{
    VOS_UINT8                           ucPsSupportFlg;                         /* ��ǰPS���Ƿ�֧��GPRS���� */
    VOS_UINT8                           ucPsAttachAllowFlg;                     /* PS���Ƿ�����ע��,VOS_TRUE:����PS��ע��,VOS_FALSE:������PS��ע�� */

    VOS_UINT8                           aucReserved1[2];
}MTC_MODEM_PS_DOMAIN_INFO_STRU;


typedef struct
{
    MTC_MMA_PLMN_ID_STRU                stPlmnId;                               /* Modem��ǰפ������ */
    MTC_MODEM_PS_DOMAIN_INFO_STRU       stPsDomainInfo;                         /* Modem��ǰPS����Ϣ */

    VOS_UINT8                           ucIsForbiddenPlmnFlag;                  /* Modem��ǰפ�������Ƿ�Ϊ��ֹ�����ʶ */
    VOS_UINT8                           aucReserved1[3];
}MTC_MODEM_NETWORK_INFO_STRU;
typedef struct
{
    PS_BOOL_ENUM_UINT8                  enCsExistFlag;
    PS_BOOL_ENUM_UINT8                  enPsExistFlag;

    VOS_UINT16                          usScellBitMap;
    VOS_UINT16                          usHoppingBitMap;
    VOS_UINT16                          usNcellBitMap;
}MTC_MODEM_MIPICLK_BITMAP_STRU;

typedef struct
{
    VOS_UINT8                           ucRatCap;                               /* Modem�Ľ��뼼������ֵ */
    MTC_MODEM_POWER_STATE_ENUM_UINT8    enPowerState;                           /* Modem�Ŀ��ػ�״̬ */
    MTC_RATMODE_ENUM_UINT8              enRatMode;                              /* Modem��ǰ��ģʽ���� */
    VOS_UINT8                           ucUsimValidFlag;                        /* modem��ǰ���Ƿ���Ч��1:����Ч��0:����Ч */

    MTC_MODEM_SERVICE_INFO_STRU         stSrvInfo;                              /* Modem��ǰ������Ϣ */
    RRC_MTC_MS_BAND_INFO_STRU           stCurBandInfo;                          /* Modem��ǰ��Ƶ����Ϣ */
    RRC_MTC_MS_BAND_INFO_STRU           stSerCellBandInfo;                      /* Modem��ǰפ������Ƶ����Ϣ */

    MTC_MODEM_NETWORK_INFO_STRU         stNetworkInfo;                          /* Modem��ǰפ��������Ϣ */

    MTC_MODEM_MIPICLK_BITMAP_STRU       stRfCellInfo;                           /* Modem��ǰ��BITMAP״̬ */

}MTC_MODEM_INFO_STRU;
typedef struct
{
    MTC_NV_PS_TRANSFER_CFG_STRU         stPsTransferCfg;                        /* ����PS TRANSFER NV������Ϣ */

    MTC_PS_TRANSFER_CAUSE_ENUM_UINT8    enRrcCause;
    MTC_PS_TRANSFER_CAUSE_ENUM_UINT8    enReportCause;
    VOS_UINT8                           aucRsve[2];
}MTC_PS_TRANSFER_CTX_STRU;
typedef struct
{
    VOS_UINT32                          ulAvailNum;                             /* ��ЧƵ�ʸ��� */
    VOS_UINT32                          ulMipiClk;                              /* MipiClkֵ */
    VOS_UINT32                          aulFreq[MTC_RF_LCD_MIPICLK_FREQ_MAX_NUM];/* ulMipiClkֵӰ���Ƶ�ʱ� */
}MTC_RF_LCD_MIPICLK_FREQ_STRU;
typedef struct
{
    VOS_UINT8                           ucAvailFreqListNum;                     /* ��Ч��astRfLcdFreqList���� */
    VOS_UINT8                           aucReserved[1];
    VOS_UINT16                          usMipiClkBitMap;                        /* MIPICLKλͼ */
    VOS_UINT32                          ulMipiClkReportModemId;                 /* ����MipiClk�����ϱ����Ǹ�modem��MTA */
    VOS_UINT16                          usFreqWidth;                            /* ���� ��λ100KHZ */
    VOS_UINT16                          usEnableBitMap;
    MTC_RF_LCD_MIPICLK_FREQ_STRU        astRfLcdFreqList[MTC_RF_LCD_MIPICLK_MAX_NUM];
}MTC_RF_LCD_INTRUSION_CTX_STRU;


typedef struct
{
    MTC_MODEM_INFO_STRU                 astModemInfo[MODEM_ID_BUTT];            /* MTCģ�鱣��Modem״̬��Ϣ */

    MTC_INTRUSION_CTX_STRU              stIntrusionCtx;                         /* ���ſ��������� */

    MTC_PS_TRANSFER_CTX_STRU            stPsTransferCtx;                        /* PS��Ǩ�������� */

    MTC_OPERATOR_CUST_SOLUTION_ENUM_UINT8   enOperatorCustSolution;             /* ��Ӫ�̶��Ʒ��� */

    VOS_UINT8                           aucReserved1[3];

    MTC_RF_LCD_INTRUSION_CTX_STRU       stRfLcdIntrusionCtx;

    /* �������ܻ���չ�������� */

}MTC_CONTEXT_STRU;

/*****************************************************************************
  8 UNION����
*****************************************************************************/

/*****************************************************************************
  9 OTHERS����
*****************************************************************************/

/*****************************************************************************
  10 ��������
*****************************************************************************/
MTC_CONTEXT_STRU* MTC_GetCtxAddr(VOS_VOID);

MTC_INTRUSION_CTX_STRU* MTC_GetIntrusionCtxAddr(VOS_VOID);

MTC_MODEM_INFO_STRU* MTC_GetModemCtxAddr(MODEM_ID_ENUM_UINT16 enModemId);

VOS_VOID MTC_InitIntrusionCtx(VOS_VOID);

VOS_UINT32 MTC_IsSupportRatType(
    MODEM_ID_ENUM_UINT16                enModemId,
    MTC_RATMODE_ENUM_UINT8              enRatMode
);

VOS_VOID MTC_InitModemCtx(VOS_VOID);

VOS_VOID MTC_InitCtx(VOS_VOID);

MTC_CFG_ENUM_UINT8 MTC_GetIntrusionCfg(VOS_VOID);

VOS_VOID MTC_SetModemPowerState(MODEM_ID_ENUM_UINT16 enModemId, MTC_MODEM_POWER_STATE_ENUM_UINT8 enState);

MTC_MODEM_POWER_STATE_ENUM_UINT8 MTC_GetModemPowerState(MODEM_ID_ENUM_UINT16 enModemId);

VOS_VOID MTC_SetModemRatMode(
    MODEM_ID_ENUM_UINT16                enModemId,
    MTC_RATMODE_ENUM_UINT8              enRatMode
);

MTC_RATMODE_ENUM_UINT8 MTC_GetModemRatMode(MODEM_ID_ENUM_UINT16 enModemId);

VOS_VOID MTC_SetModemCallSrvExistFlg(
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT8                           ucFlg
);

VOS_VOID MTC_SetModemSmsSrvExistFlg(
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT8                           ucFlg
);

VOS_VOID MTC_SetModemSsSrvExistFlg(
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT8                           ucFlg
);

MTC_CS_SERVICE_STATE_ENUM_UINT8 MTC_GetModemCsSrvExistFlg(
    MODEM_ID_ENUM_UINT16                enModemId
);

RRC_MTC_MS_BAND_INFO_STRU* MTC_GetModemCurBandInfo(
    MODEM_ID_ENUM_UINT16                enModemId
);

VOS_UINT8 MTC_GetModemUsimValidFlag(MODEM_ID_ENUM_UINT16 enModemId);
VOS_VOID MTC_SetModemUsimValidFlag(
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT8                           ucUsimValidStatus
);

VOS_VOID MTC_ResetModemInfo(MODEM_ID_ENUM_UINT16 enModemId);

VOS_VOID MTC_SetNotchEnalbeFlag(PS_BOOL_ENUM_UINT8 enFlag);
PS_BOOL_ENUM_UINT8 MTC_GetNotchEnalbeFlag(VOS_VOID);
MTC_CFG_ENUM_UINT8 MTC_GetNotchCfg(VOS_VOID);

RRC_MTC_MS_BAND_INFO_STRU* MTC_GetModemSerCellBandInfo(
    MODEM_ID_ENUM_UINT16                enModemId
);
MTC_CFG_ENUM_UINT8 MTC_GetTlRfCtrlCfg(VOS_VOID);
VOS_VOID MTC_SetTlRfCtrlEnalbeFlag(PS_BOOL_ENUM_UINT8 enFlag);
PS_BOOL_ENUM_UINT8 MTC_GetTlRfCtrlEnalbeFlag(VOS_VOID);

VOS_VOID MTC_InitPsTransferCtx(VOS_VOID);

VOS_VOID MTC_InitOperatorCustSolution(VOS_VOID);

MTC_PS_TRANSFER_ENUM_UINT8 MTC_GetPsTransferCfg(VOS_VOID);

MTC_PS_TRANSFER_CTX_STRU* MTC_GetPsTransferCtxAddr(VOS_VOID);

MTC_MODEM_NETWORK_INFO_STRU* MTC_GetModemNetworkInfoAddr(
    MODEM_ID_ENUM_UINT16                enModemId);

MTC_OPERATOR_CUST_SOLUTION_ENUM_UINT8 MTC_GetOperatorCustSolution(VOS_VOID);

VOS_VOID MTC_SetOperatorCustSolution(
    MTC_OPERATOR_CUST_SOLUTION_ENUM_UINT8   enOperatorCustSolution);


VOS_VOID MTC_SetNarrowDcsEnalbeFlag(PS_BOOL_ENUM_UINT8 enFlag);
PS_BOOL_ENUM_UINT8 MTC_GetNarrowDcsEnalbeFlag(VOS_VOID);
MTC_CFG_ENUM_UINT8 MTC_GetNarrowBandDcsCfg(VOS_VOID);

MTC_RF_LCD_INTRUSION_CTX_STRU* MTC_GetRfLcdIntrusionCtxAddr(VOS_VOID);
MTC_CFG_ENUM_UINT8 MTC_GetRfLcdIntrusionCfg(VOS_VOID);
VOS_VOID MTC_InitRfLcdIntrusionCtx(VOS_VOID);
VOS_VOID MTC_ReadRfLcdCfgNv(VOS_VOID);
VOS_UINT32 MTC_GetMipiClkRcvPid(VOS_VOID);
MTC_MODEM_MIPICLK_BITMAP_STRU* MTC_GetRfCellInfoAddr(
    MODEM_ID_ENUM_UINT16                enModemId
);
VOS_UINT16 MTC_GetMipiClkBitMap(VOS_VOID);
VOS_VOID MTC_InitRfCellInfo(MODEM_ID_ENUM_UINT16 enModemId);
MTC_RF_LCD_INTRUSION_CTX_STRU* MTC_GetRfLcdIntrusionCtxAddr(VOS_VOID);

MTC_CFG_ENUM_UINT8 MTC_GetRseSupportCfg(VOS_VOID);

PS_BOOL_ENUM_UINT8 MTC_GetRseCfgValue(VOS_VOID);

VOS_VOID MTC_SetRseCfgValue(PS_BOOL_ENUM_UINT8 enValue);





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

#endif /* end of MtcCtx.h */
