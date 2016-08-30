

#ifndef __RRM_H__
#define __RRM_H__


/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "Ps.h"
#include "pslog.h"
#include "TtfNvInterface.h"
#include "RrmTimer.h"
#include "PsRrmInterface.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)

#if (FEATURE_ON == FEATURE_DSDS)
/*****************************************************************************
  2 �궨��
*****************************************************************************/
/* RRM RFIDĬ��ֵ */
#define RRM_RFID_DEFAULT_VALUE              (0xFFFF)

/* RRM PIDĬ��ֵ */
#define RRM_PID_DEFAULT_VALUE               (0xFFFFFFFF)

/* RRM ֧��ע��������� */
#define RRM_REG_MAX_NUMBER                  (6)

/* RFID�������� */
#define RRM_RFID_INDEX_MASK                 (0x0FFF)

/* MODEMID�������� */
#define RRM_MODEMID_INDEX_MASK              (0x0FFF)

/* REGID�������� */
#define RRM_REGID_INDEX_MASK                (0x0FFF)

/* ����RFID��ȡ�±������� */
#define RRM_GET_RFID_INDEX(enRFID)          ((VOS_UINT16)((enRFID) & (RRM_RFID_INDEX_MASK)))

/* ����MODEM ID��ȡ�±������� */
#define RRM_GET_MODEMID_INDEX(enModemId)    ((VOS_UINT16)((enModemId) & (RRM_MODEMID_INDEX_MASK)))

/* ����REGID��ȡ�±������� */
#define RRM_GET_REGID_INDEX(usRegId)        ((VOS_UINT16)((usRegId) & (RRM_REGID_INDEX_MASK)))




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
 �ṹ��    : RRM_REG_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : RRM ע����Ϣ�ṹ
*****************************************************************************/
typedef struct
{
    PS_BOOL_ENUM_UINT8                  enRegisteredFlg;    /* �Ƿ���ע��ı�ʶ */
    RRM_PS_RAT_TYPE_ENUM_UINT8          enRatType;          /* ע���RAT type */
    MODEM_ID_ENUM_UINT16                enModemId;          /* ע���MODEM ID */
    VOS_UINT32                          ulPid;              /* ע���PID */
}RRM_REG_INFO_STRU;




typedef struct
{
    PS_BOOL_ENUM_UINT8                  enRFIDUsedFlg;          /* �Ƿ���ʹ��RFID�ı�ʶ */
    VOS_UINT8                           aucReserved[3];
    VOS_UINT16                          usUsingRFIDValue;       /* ����ʹ�õ�RFIDֵ */
    MODEM_ID_ENUM_UINT16                enUsingModemId;         /* ����ʹ�õ�MODEMID��ֵ */
    RRM_RFID_TIMER_CTRL_STRU            stRFIDTimer;            /* RRM Timer����ṹ */
}RRM_RFID_INFO_STRU;



typedef struct
{
    NV_MODEM_RF_SHARE_CFG_STRU          astModemCfgInfo[MODEM_ID_BUTT];     /* RRMģ��MODEM��������Ϣ */

    RRM_RFID_INFO_STRU                  astRFIDInfo[RRM_RFID_BUTT];         /* RRMģ��RFID����Ϣ */

    RRM_REG_INFO_STRU                   astRrmRegInfo[RRM_REG_MAX_NUMBER];  /* RRMģ��ע�����Ϣ */

    /* �������ܻ���չ�������� */

}RRM_INFO_STRU;



/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/
extern   RRM_INFO_STRU                      g_stRrmInfo;


/* ��ȡRRM ģ����Ϣ��ַ */
#define RRM_GetInfoAddr()                   (&g_stRrmInfo)

/* ��ȡRRM RF��Ϣ�б��ַ */
#define RRM_GetRFIDInfoListAddr()           (((RRM_GetInfoAddr())->astRFIDInfo))

/* ��ȡRRM MODEM������Ϣ�б��ַ */
#define RRM_GetModemCfgInfoListAddr()       (((RRM_GetInfoAddr())->astModemCfgInfo))


/* ����MODEM ID�����Ż�ȡRRM MODEM������Ϣ��ַ */
#define RRM_GetModemSupportFlg(enModemId)  \
    ((enModemId < MODEM_ID_BUTT) ? (RRM_GetModemCfgInfoListAddr()[RRM_GET_MODEMID_INDEX(enModemId)].usSupportFlag) : PS_FALSE)


/* ����MODEM ID�����Ż�ȡRRM MODEM������Ϣ��ַ */
#define RRM_GetModemCfgInfoAddr(enModemId)  \
    ((enModemId < MODEM_ID_BUTT) ? (&(RRM_GetModemCfgInfoListAddr()[RRM_GET_MODEMID_INDEX(enModemId)])) : VOS_NULL_PTR)

/* ����RFID�����Ż�ȡRRM RF��Ϣ��ַ */
#define RRM_GetRFIDInfoAddr(enRFIDIndex)    \
    ((enRFIDIndex < RRM_RFID_BUTT) ? (&(RRM_GetRFIDInfoListAddr()[RRM_GET_RFID_INDEX(enRFIDIndex)])) : VOS_NULL_PTR)

/* ��ȡRRM RFID Timer��Ϣ��ַ */
#define RRM_GetRFIDTimerAddr(enRFIDIndex)   \
    ((enRFIDIndex < RRM_RFID_BUTT) ? (&(RRM_GetRFIDInfoListAddr()[RRM_GET_RFID_INDEX(enRFIDIndex)].stRFIDTimer)) : VOS_NULL_PTR)

/* ��ȡRRM ע����Ϣ�б��ַ */
#define RRM_GetRegInfoListddr()             \
    (((RRM_GetInfoAddr())->astRrmRegInfo))


/* ����RFID�����Ż�ȡRRM RF��Ϣ��ַ */
#define RRM_GetRegInfoAddr(usRegInfoIndex)    \
    ((usRegInfoIndex < RRM_REG_MAX_NUMBER) ? (&(RRM_GetRegInfoListddr()[RRM_GET_REGID_INDEX(usRegInfoIndex)])) : VOS_NULL_PTR)


/* ����MODEM ID�����Ż�ȡGSM RFIDֵ */
#define RRM_GET_GSM_RFID(enModemId)         \
    ((enModemId < MODEM_ID_BUTT) ? (RRM_GetModemCfgInfoListAddr()[RRM_GET_MODEMID_INDEX(enModemId)].usGSMRFID) : RRM_RFID_DEFAULT_VALUE)

/* ����MODEM ID�����Ż�ȡWCDMA RFIDֵ */
#define RRM_GET_WCDMA_RFID(enModemId)       \
    ((enModemId < MODEM_ID_BUTT) ? (RRM_GetModemCfgInfoListAddr()[RRM_GET_MODEMID_INDEX(enModemId)].usWCDMARFID) : RRM_RFID_DEFAULT_VALUE)

/* ����MODEM ID�����Ż�ȡTDS RFIDֵ */
#define RRM_GET_TDS_RFID(enModemId)         \
    ((enModemId < MODEM_ID_BUTT) ? (RRM_GetModemCfgInfoListAddr()[RRM_GET_MODEMID_INDEX(enModemId)].usTDSRFID) : RRM_RFID_DEFAULT_VALUE)

/* ����MODEM ID�����Ż�ȡLTE RFIDֵ */
#define RRM_GET_LTE_RFID(enModemId)         \
    ((enModemId < MODEM_ID_BUTT) ? (RRM_GetModemCfgInfoListAddr()[RRM_GET_MODEMID_INDEX(enModemId)].usLTERFID) : RRM_RFID_DEFAULT_VALUE)

/* ����MODEM ID�����Ż�ȡCDMA RFIDֵ */
#define RRM_GET_CDMA_RFID(enModemId)        \
    ((enModemId < MODEM_ID_BUTT) ? (RRM_GetModemCfgInfoListAddr()[RRM_GET_MODEMID_INDEX(enModemId)].usCDMARFID) : RRM_RFID_DEFAULT_VALUE)

/* ����MODEM ID�����Ż�ȡEVDO RFIDֵ */
#define RRM_GET_EVDO_RFID(enModemId)        \
    ((enModemId < MODEM_ID_BUTT) ? (RRM_GetModemCfgInfoListAddr()[RRM_GET_MODEMID_INDEX(enModemId)].usEVDORFID) : RRM_RFID_DEFAULT_VALUE)



/*****************************************************************************
  10 ��������
*****************************************************************************/

VOS_VOID  RRM_Init(VOS_VOID);
VOS_UINT32 RRM_JudgeRegInfoIsReged
(
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT32                          ulPid,
    PS_BOOL_ENUM_UINT8                 *enIsSameModem,
    VOS_UINT16                         *usRegInfoIndex,
    PS_BOOL_ENUM_UINT8                 *enRegInfoIsReged
);

VOS_UINT32 RRM_JudgeRFIDIsUsed
(
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT16                          usRFIDValue,
    PS_BOOL_ENUM_UINT8                 *enIsSameModem,
    RRM_RFID_ENUM_UINT16               *enRFIDIndex,
    PS_BOOL_ENUM_UINT8                 *enRFIDIsUsed
);


VOS_VOID RRM_SetRegInfo
(
    PS_BOOL_ENUM_UINT8                  enRegisteredFlg,
    RRM_PS_RAT_TYPE_ENUM_UINT8          enRatType,
    MODEM_ID_ENUM_UINT16                enModemId,
    VOS_UINT32                          ulPid
);

VOS_UINT32 RRM_SetRFIDUsedInfo
(
    PS_BOOL_ENUM_UINT8                  enRFIDUsedFlg,
    VOS_UINT16                          usUsingRFIDValue,
    MODEM_ID_ENUM_UINT16                enUsingModemId
);


VOS_VOID RRM_ClearRegInfo
(
    VOS_UINT16                          usRegInfoIndex
);

VOS_VOID RRM_ClearRFIDUsedInfo
(
    RRM_RFID_ENUM_UINT16                enRFIDIndex
);

VOS_UINT16 RRM_GetRFIDCfgValue
(
    MODEM_ID_ENUM_UINT16                enModemId,
    RRM_PS_RAT_TYPE_ENUM_UINT8          enRatType
);


#endif /* FEATURE_ON == FEATURE_DSDS */


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

#endif /* end of Rrm.h */

