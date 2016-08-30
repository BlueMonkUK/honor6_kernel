
#ifndef  VC_COMM_H
#define  VC_COMM_H


/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "MnComm.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 ��������
*****************************************************************************/

#define NV_COPY_DATA_OFFSET                     (80)
#define VC_VOLUME_DEFAULT_VALUE                 (-6)

/*�ط�����ϢID*/
#define EVT_NAS_VC_OUTSIDE_RUNNING_CONTEXT_FOR_PC_REPLAY   0xaaaa         /* ΪPC���̻طŶ������Ϣ */

#define APP_VC_VOICE_TEST_FLAG_DEFALUT          (0x0)                     /* Ƶ�ײ���ģʽ��־Ĭ��ֵ��Ĭ�ϲ�����Ƶ�ײ���ģʽ */
#define APP_VC_VOICE_TEST_FLAG_ENABLE           (0x55aa55aa)              /* ��0x55aa55aaֵƵ�ײ���ģʽ������ */

/*****************************************************************************
  3���Ͷ���
*****************************************************************************/

/* VC��ʱ��ID���� */
enum APP_VC_TIMER_ID_ENUM
{
    APP_VC_TIMER_START,
    APP_VC_TIMER_STOP,
    APP_VC_TIMER_SET_DEV,
    APP_VC_TIMER_SET_VOLUME,
    APP_VC_TIMER_SET_CODEC,
    APP_VC_TIMER_SET_FOREGROUND,
    APP_VC_TIMER_SET_BACKGROUND,
    APP_VC_TIMER_QRY_GROUND,

    APP_VC_TIMER_BUTT
};
typedef VOS_UINT16  APP_VC_TIMER_ID_ENUM_U16;

enum APP_VC_AUDIO_SAMPLE_RATE_ENUM
{
    APP_VC_SAMPLE_RATE_8K       =0,
    APP_VC_SAMPLE_RATE_11K025,
    APP_VC_SAMPLE_RATE_12K,
    APP_VC_SAMPLE_RATE_16K,
    APP_VC_SAMPLE_RATE_22K05,
    APP_VC_SAMPLE_RATE_24K,
    APP_VC_SAMPLE_RATE_32K,
    APP_VC_SAMPLE_RATE_44K1,
    APP_VC_SAMPLE_RATE_48K,
    APP_VC_SAMPLE_RATE_BUTT
};
typedef VOS_UINT16  APP_VC_AUDIO_SAMPLE_RATE_ENUM_U16;

enum APP_VC_AUDIO_IN_DEV_SELECT_ENUM
{
    APP_VC_AUDIO_IN_HEADSET_OUT_MIC         = 1,
    APP_VC_AUDIO_IN_HEADSET_INTERNAL_MIC    = 2,
    APP_VC_AUDIO_IN_HEADFREE_INTERNAL_MIC   = 4,
    APP_VC_AUDIO_IN_BUTT
};
typedef VOS_UINT16  APP_VC_AUDIO_IN_DEV_SELECT_ENUM_U16;

enum APP_VC_AUDIO_OUT_DEV_SELECT_ENUM
{
    APP_VC_AUDIO_OUT_HEADSET_EARPHONE   = 1,
    APP_VC_AUDIO_OUT_HEADSET_RECEIVER   = 2,
    APP_VC_AUDIO_OUT_HEADFREE_SPEAKER   = 4,
    APP_VC_AUDIO_OUT_BUTT
};
typedef VOS_UINT16  APP_VC_AUDIO_OUT_DEV_SELECT_ENUM_U16;

enum VC_NVIM_WORK_MODE_ENUM
{
    en_NV_Item_HANDSET_W_PARAM_ID           = 0X3000,                           /*Wģ�ֳֹ�������ID*/
    en_NV_Item_HANDHELD_HANDFREE_W_PARAM_ID = 0X3001,                           /*Wģ�ֳ����Ṥ������ID*/
    en_NV_Item_VECHILE_HANDFREE_W_PARAM_ID  = 0X3002,                           /*Wģ�������Ṥ������ID*/
    en_NV_Item_HEADSET_W_PARAM_ID           = 0X3003,                           /*Wģ������������ID*/
    en_NV_Item_BT_W_PARAM_ID                = 0X3004,                           /*Wģ������������ID*/
    en_NV_Item_HANDSET_G_PARAM_ID           = 0X3005,                           /*Gģ�ֳֹ�������ID*/
    en_NV_Item_HANDHELD_HANDFREE_G_PARAM_ID = 0X3006,                           /*Gģ�ֳ����Ṥ������ID*/
    en_NV_Item_VECHILE_HANDFREE_G_PARAM_ID  = 0X3007,                           /*Gģ�������Ṥ������ID*/
    en_NV_Item_HEADSET_G_PARAM_ID           = 0X3008,                           /*Gģ������������ID*/
    en_NV_Item_BT_G_PARAM_ID                = 0X3009,                           /*Gģ������������ID*/
    en_NV_Item_PCVOICE_PARAM_ID_W           = 0X300F,                           /*W PC VOICE��������ID*/

    en_NV_Item_ID_BUTT                      = 0X3100
};
typedef VOS_UINT16 VC_NVIM_WORK_MODE_ENUM_U16;

enum APP_VC_IOCTL_AUDIO_IN_DEV_ENUM
{
    APP_VC_IN_DEV_HEADSET_EXTERNAL_MIC = 1,
    APP_VC_IN_DEV_HANDSET_INTERNAL_MIC  = 2,
    APP_VC_IN_DEV_HANDFREE_INTERNAL_MIC = 4,

    APP_VC_IN_DEV_BUTT
};
typedef VOS_UINT16 APP_VC_IOCTL_AUDIO_IN_DEV_ENUM_U16;

enum APP_VC_IOCTL_AUDIO_OUT_DEV_ENUM
{
    APP_VC_OUT_DEV_HEADSET_EXTERNAL_MIC = 1,
    APP_VC_OUT_DEV_HANDSET_INTERNAL_MIC  = 2,
    APP_VC_OUT_DEV_HANDFREE_INTERNAL_MIC = 4,

    APP_VC_OUT_DEV_BUTT
};
typedef VOS_UINT16 APP_VC_IOCTL_AUDIO_OUT_DEV_ENUM_U16;


enum APP_VC_OPEN_CHANNEL_FAIL_CAUSE_ENUM
{
    APP_VC_OPEN_CHANNEL_CAUSE_SUCC                = 0,                          /* �ɹ� */
    APP_VC_OPEN_CHANNEL_FAIL_CAUSE_STARTED,                                     /* HIFI�Ѿ����� */
    APP_VC_OPEN_CHANNEL_FAIL_CAUSE_PORT_CFG_FAIL,                               /* �˿�����ʧ�� */
    APP_VC_OPEN_CHANNEL_FAIL_CAUSE_SET_DEVICE_FAIL,                             /* ����Deviceʧ�� */
    APP_VC_OPEN_CHANNEL_FAIL_CAUSE_SET_START_FAIL,                              /* startʧ�� */
    APP_VC_OPEN_CHANNEL_FAIL_CAUSE_SET_VOLUME_FAIL,                             /* ��������ʧ�� */
    APP_VC_OPEN_CHANNEL_FAIL_CAUSE_SAMPLE_RATE_FAIL,                            /* ���ʲ���ʧ�� */
    APP_VC_OPEN_CHANNEL_FAIL_CAUSE_TI_START_EXPIRED,                            /* start������ʱ����ʱ */

    APP_VC_OPEN_CHANNEL_FAIL_CAUSE_BUTT
};
typedef VOS_UINT32  APP_VC_OPEN_CHANNEL_FAIL_CAUSE_ENUM_UINT32;


/*****************************************************************************
 �ṹ��    : VC_OM_PCVOICE_TRANS_STATUS_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  :����ϢVC_OM_PCVOICE_TRANS_STATUS_STRU�Ľṹ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                        /* ��Ϣͷ */ /*_H2ASN_Skip*/
    VOS_UINT16                     usPcVoiceChannelOper;  /* ��PC VOICE����ͨ�����еĲ���: �򿪡��޸ġ��ر�*/
    VOS_UINT16                     usPort;                /* ����ͨ���˿� */
    VOS_UINT32                     usOmOperResult;        /* OMִ�н�� */
} VC_OM_PCVOICE_TRANS_STATUS_STRU;


/*****************************************************************************
  4 �궨��
*****************************************************************************/
#define APP_VC_INVALID_DEV_HANDLE                   -1

#define APP_VC_AUDIO_IOCTL_BASE                     0x30000000
#define APP_VC_IOCTL_AUDIO_PCM_MODE_SET             0x30000000 + 7
#define APP_VC_IOCTL_AUDIO_SAMPLE_RATE_SET          0x30000000 + 1
#define APP_VC_IOCTL_AUDIO_IN_DEV_SELECT            0x30000000 + 11
#define APP_VC_IOCTL_AUDIO_OUT_DEV_SELECT           0x30000000 + 12
#define APP_VC_IOCTL_AUDIO_IN_DEV_UNSELECT          0x30000000 + 14
#define APP_VC_IOCTL_AUDIO_OUT_DEV_UNSELECT         0x30000000 + 13
#define APP_VC_IOCTL_AUDIO_VOICE_OPEN               0x30000000 + 23
#define APP_VC_IOCTL_AUDIO_VOICE_CLOSE              0x30000000 + 24


/* ������� */
#define VC_INFO_LOG(str)      PS_LOG(WUEPS_PID_VC, 0, PS_PRINT_INFO, str)
#define VC_NORM_LOG(str)      PS_LOG(WUEPS_PID_VC, 0, PS_PRINT_NORMAL, str)
#define VC_WARN_LOG(str)      PS_LOG(WUEPS_PID_VC, 0, PS_PRINT_WARNING, str)
#define VC_ERR_LOG(str)       PS_LOG(WUEPS_PID_VC, 0, PS_PRINT_ERROR, str)

#define VC_INFO_LOG1(str, x)  PS_LOG1(WUEPS_PID_VC, 0, PS_PRINT_INFO, str, x)
#define VC_NORM_LOG1(str, x)  PS_LOG1(WUEPS_PID_VC, 0, PS_PRINT_NORMAL, str, x)
#define VC_WARN_LOG1(str, x)  PS_LOG1(WUEPS_PID_VC, 0, PS_PRINT_WARNING, str, x)
#define VC_ERR_LOG1(str, x)   PS_LOG1(WUEPS_PID_VC, 0, PS_PRINT_ERROR, str, x)

#define VC_WARN_LOG2(str, x1, x2)  PS_LOG2(WUEPS_PID_VC, 0, PS_PRINT_WARNING, str, x1, x2)

/*****************************************************************************
  5 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  6 �ӿں�������
*****************************************************************************/
extern VOS_UINT32 WuepsVCPidInit(enum VOS_INIT_PHASE_DEFINE InitPhrase);
extern VOS_VOID   APP_VC_MsgProc(MsgBlock* pMsg);

VOS_UINT32 NAS_VC_SndOutsideContextData(VOS_VOID);


VOS_UINT32  APP_VC_SendVcReqToOm(VOS_UINT32 ulStatus, VOS_UINT32 ulPort);

#ifdef __PS_WIN32_RECUR__

VOS_UINT32 NAS_VC_RestoreContextData(struct MsgCB * pMsg);
#endif


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

#endif /* VC_COMM_H */

