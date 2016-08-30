
#ifndef  VC_PHY_INTERFACE_H
#define  VC_PHY_INTERFACE_H


/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "VcCallInterface.h"
#include "AppVcApi.h"
#include "VcCodecInterface.h"
#include "NasNvInterface.h"
#if (FEATURE_ON == FEATURE_IMS)
#include "VcImsaInterface.h"
#endif
#include "VcComm.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 ��������
*****************************************************************************/


/*****************************************************************************
  3���Ͷ���
*****************************************************************************/
enum VC_GPHY_MSG_ID_ENUM
{
    /* VC->GDSP */
    ID_VC_GPHY_TEST_MODE_NOTIFY         = 0xE705,

    /* GDSP->VC */

    ID_VC_GPHY_MSG_BUTT
};
typedef VOS_UINT16 VC_GPHY_MSG_ID_ENUM_U16;

typedef struct
{
    VOS_INT16                           sTxVol[20];                             /* ����������dBֵ, -80~12 */
    VOS_INT16                           sRxVol[20];                             /* ����������dBֵ, -80~12 */
    VOS_UINT16                          usSlopeEnable;                          /* Ԥ�����˲�ʹ�� */
    VOS_INT16                           asSlopeCoeff[15];                       /* Ԥ�����˲�ϵ�� */
    VOS_UINT16                          usTxHpfEnable;                          /* ���и�ͨ�˲�ʹ�� */
    VOS_INT16                           asTxHpfCoeff[8];                        /* ���и�ͨ�˲�ϵ�� */
    VOS_UINT16                          usRxHpfEnable;                          /* ���и�ͨ�˲�ʹ�� */
    VOS_INT16                           asRxHpfCoeff[8];                        /* ���и�ͨ�˲�ϵ�� */
    VOS_INT16                           sDevTxGain;                             /* �豸������棨Codec�������棩 */
    VOS_INT16                           sDevRxGain;                             /* �豸�������棨Codec�������棩 */
    VOS_INT16                           sSideGain;                              /* �������� */
    VOS_UINT16                          usTxCompEnable;                         /* ���в���ʹ�� */
    VOS_INT16                           asTxCompCoeff[31];                      /* ���в����˲�ϵ�� */
    VOS_UINT16                          usRxCompEnable;                         /* ���в���ʹ�� */
    VOS_INT16                           asRxCompCoeff[31];                      /* ���в����˲�ϵ�� */
    VOS_UINT16                          usEcEnable;                             /* ��������ʹ�ܣ�0 ��ֹ��1 ʹ�� */
    VOS_INT16                           asEcParam[20];                          /* EC���� */
    VOS_UINT16                          usNsEnable;                             /* ��������ʹ�ܣ�0 ��ֹ��1 ʹ�� */
    VOS_INT16                           asNsParam[10];                          /* NS���� */
    VOS_UINT16                          usAgcEnable;                            /* �Զ��������ʹ�ܣ�0 ��ֹ��1 ʹ�� */
    VOS_INT16                           asAgcParam[6];                          /* AGC���� */
}VC_DEVICE_PARA_NV_STRU;


typedef struct
{
    VC_PHY_DEVICE_MODE_ENUM_U16         enDeviceMode;                           /* �����豸��0 �ֳ֣�1 �ֳ����᣻2 �������᣻3 ������4 ���� */
    VOS_INT16                           sTxVol;                                 /* ����������dBֵ, -80~12 */
    VOS_INT16                           sRxVol;                                 /* ����������dBֵ, -80~12 */
    VOS_UINT16                          usSlopeEnable;                          /* Ԥ�����˲�ʹ�� */
    VOS_INT16                           asSlopeCoeff[15];                       /* Ԥ�����˲�ϵ�� */
    VOS_UINT16                          usTxHpfEnable;                          /* ���и�ͨ�˲�ʹ�� */
    VOS_INT16                           asTxHpfCoeff[8];                        /* ���и�ͨ�˲�ϵ�� */
    VOS_UINT16                          usRxHpfEnable;                          /* ���и�ͨ�˲�ʹ�� */
    VOS_INT16                           asRxHpfCoeff[8];                        /* ���и�ͨ�˲�ϵ�� */
    VOS_INT16                           sDevTxGain;                             /* �豸������棨Codec�������棩 */
    VOS_INT16                           sDevRxGain;                             /* �豸�������棨Codec�������棩 */
    VOS_INT16                           sSideGain;                              /* �������� */
    VOS_UINT16                          usTxCompEnable;                         /* ���в���ʹ�� */
    VOS_INT16                           asTxCompCoeff[31];                      /* ���в����˲�ϵ�� */
    VOS_UINT16                          usRxCompEnable;                         /* ���в���ʹ�� */
    VOS_INT16                           asRxCompCoeff[31];                      /* ���в����˲�ϵ�� */
    VOS_UINT16                          usEcEnable;                             /* ��������ʹ�ܣ�0 ��ֹ��1 ʹ�� */
    VOS_INT16                           asEcParam[20];                          /* EC���� */
    VOS_UINT16                          usNsEnable;                             /* ��������ʹ�ܣ�0 ��ֹ��1 ʹ�� */
    VOS_INT16                           asNsParam[10];                          /* NS���� */
    VOS_UINT16                          usAgcEnable;                            /* �Զ��������ʹ�ܣ�0 ��ֹ��1 ʹ�� */
    VOS_INT16                           asAgcParam[6];                          /* AGC���� */
    VOS_UINT16                          usReserve2;                             /* ���� */
}VC_DEVICE_PARA_STRU;

/*****************************************************************************
 �ṹ��    : VC_PHY_SET_DEVICE_REQ_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : ֹͣ������������ṹ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT16                          usMsgName;                              /* ԭ������ */
    VOS_UINT16                          usReserve;                              /* ���� */
    VC_DEVICE_PARA_STRU                 stDevPara;                              /* ���� */
}VC_PHY_SET_DEVICE_REQ_STRU;
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT16                          usMsgName;                              /* ԭ������ */
    VOS_UINT8                           aucReserved[2];
}VC_PHY_TEST_MODE_NOTIFY_STRU;

/*****************************************************************************
  4 �궨��
*****************************************************************************/





/*****************************************************************************
  5 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  6 �ӿں�������
*****************************************************************************/
extern VOS_UINT32   OM_TraceMsgHook(VOS_VOID *pMsg);


VOS_UINT32  APP_VC_SendStartReq(
    CALL_VC_CHANNEL_INFO_STRU           *pstChanInfo,
    CALL_VC_RADIO_MODE_ENUM_U8          enRadioMode
);


VOS_UINT32  APP_VC_SendStopReq(CALL_VC_RADIO_MODE_ENUM_U8          enRadioMode);


VOS_UINT32  APP_VC_SendSetDeviceReq(
    VC_DEVICE_PARA_STRU                 *pstDevPara
);


VOS_UINT32  APP_VC_SendSetVolumeReq(
    VOS_UINT16                          usVolTarget,
    VOS_INT16                           sVolValue
);


VOS_UINT32  APP_VC_SendSetCodecReq(CALL_VC_CHANNEL_INFO_STRU *pstChanInfo);


VOS_UINT32  APP_VC_SendEndCallReq(VOS_VOID);


VOS_UINT32  APP_VC_SendPhyTestModeNotify(
    CALL_VC_RADIO_MODE_ENUM_U8              enMode
);



VOS_UINT32 APP_VC_SendSetForeGroundReq(VOS_VOID);


VOS_UINT32 APP_VC_SendSetBackGroundReq(VOS_VOID);


VOS_UINT32 APP_VC_SendGroundQry(VOS_VOID);


VOS_UINT32  APP_VC_SendSetForeGroundCnf(
    MN_CLIENT_ID_T                      clientId,
    VCVOICE_EXECUTE_RSLT_ENUM_UINT16    enExeRslt
);


VOS_UINT32  APP_VC_SendSetBackGroundCnf(
    MN_CLIENT_ID_T                      clientId,
    VCVOICE_EXECUTE_RSLT_ENUM_UINT16    enExeRslt
);


VOS_UINT32  APP_VC_SendGroundRsp(
    MN_CLIENT_ID_T                      clientId,
    VCVOICE_GROUND_ENUM_UINT16          enState ,
    VOS_UINT8                           ucQryRslt
);


VOS_UINT32 APP_VC_SendSetModemLoopReq(VCVOICE_LOOP_ENUM_UINT16 enVoiceLoop);


#if (FEATURE_ON == FEATURE_IMS)
VOS_UINT32  APP_VC_SendImsaExceptionNtf(
    VC_IMSA_EXCEPTION_CAUSE_ENUM_UINT32                     enCause
);
#endif

VOS_VOID  APP_VC_LogEndCallReq(
    APP_VC_OPEN_CHANNEL_FAIL_CAUSE_ENUM_UINT32              enVcCause
);


#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* VC_PHY_INTERFACE_H */

