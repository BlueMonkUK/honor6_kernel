
#ifndef  TAF_MMA_TIMER_MGMT_PROC_H
#define  TAF_MMA_TIMER_MGMT_PROC_H

#pragma pack(4)

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
  2 �궨��
*****************************************************************************/
#define TAF_MMA_MAX_TIMER_NUM                       (30)

/* �ȴ�PB���ļ�ˢ��ָʾ��ʱ�� */
#define TI_TAF_MMA_WAIT_PIH_USIM_STATUS_IND_LEN                  (10*1000)

/* �ȴ�IMSA�Ŀ����ظ���ʱ�� */
#define TI_TAF_MMA_WAIT_IMSA_START_CNF_LEN                       (3*1000)

/* �ȴ�IMSA�Ĺػ��ظ���ʱ�� */
#define TI_TAF_MMA_WAIT_IMSA_POWER_OFF_CNF_LEN                   (3*1000)

/* �ȴ�MMC�Ŀ����ظ���ʱ�� */
#define TI_TAF_MMA_WAIT_MMC_START_CNF_LEN                        (70*1000)

/* �ȴ�MMC�Ĺػ��ظ���ʱ�� */
#define TI_TAF_MMA_WAIT_MMC_POWER_OFF_CNF_LEN                    (60*1000)

#define TI_TAF_MMA_WAIT_MMC_ACQ_CNF_LEN                          (180*1000)

/* �ȴ�MMC��REG_CNF��ʱ�� */
#define TI_TAF_MMA_WAIT_MMC_REG_CNF_LEN                          (1200*1000)

/* �ȴ�MMC��POWER_SAVE_CNF��ʱ�� */
#define TI_TAF_MMA_WAIT_MMC_POWER_SAVE_CNF_LEN                   (10*1000)

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

enum TAF_MMA_TIMER_ID_ENUM
{
    /* �ȴ�PB���ļ�ˢ��ָʾ��ʱ�� */
    TI_TAF_MMA_WAIT_PIH_USIM_STATUS_IND                  = MN_TIMER_CLASS_MMA ,

    /* �ȴ�IMSA�Ŀ����ظ���ʱ�� */
    TI_TAF_MMA_WAIT_IMSA_START_CNF                       ,

    /* �ȴ�IMSA�Ĺػ��ظ���ʱ�� */
    TI_TAF_MMA_WAIT_IMSA_POWER_OFF_CNF                   ,

    /* �ȴ�MMC�Ŀ����ظ���ʱ�� */
    TI_TAF_MMA_WAIT_MMC_START_CNF                        ,

    /* �ȴ�MMC�Ĺػ��ظ���ʱ�� */
    TI_TAF_MMA_WAIT_MMC_POWER_OFF_CNF                    ,

    /* �ȴ�IMSA��IMS VOICE CAP��Ϣ�ı�����ʱ�� */
    TI_TAF_MMA_WAIT_IMSA_IMS_VOICE_CAP_NOTIFY,

    /* �ȴ�MMC��MMC_ACQ_CNF��ʱ�� */
    TI_TAF_MMA_WAIT_MMC_ACQ_CNF                         ,

    /* �ȴ�MMC��REG_CNF��ʱ�� */
    TI_TAF_MMA_WAIT_MMC_REG_CNF                         ,

    /* �ȴ�MMC��POWER_SAVE_CNF��ʱ�� */
    TI_TAF_MMA_WAIT_MMC_POWER_SAVE_CNF                  ,


    TI_TAF_MMA_TIMER_BUTT
};
typedef VOS_UINT32  TAF_MMA_TIMER_ID_ENUM_UINT32;
enum TAF_MMA_TIMER_STATUS_ENUM
{
    TAF_MMA_TIMER_STATUS_STOP,              /* ��ʱ��ֹͣ״̬ */
    TAF_MMA_TIMER_STATUS_RUNING,            /* ��ʱ������״̬ */
    TAF_MMA_TIMER_STATUS_BUTT
};
typedef VOS_UINT8 TAF_MMA_TIMER_STATUS_ENUM_UINT8;

/*****************************************************************************
  3 �ṹ����
*****************************************************************************/


typedef struct
{
    HTIMER                              hTimer;                                 /* ��ʱ��������ָ�� */
    TAF_MMA_TIMER_ID_ENUM_UINT32        enTimerId;                              /* ��ʱ����ID */
    TAF_MMA_TIMER_STATUS_ENUM_UINT8     enTimerStatus;                          /* ��ʱ��������״̬,������ֹͣ */
    VOS_UINT8                           aucReserve[3];
} TAF_MMA_TIMER_CTX_STRU;
typedef struct
{
    MSG_HEADER_STRU                     stMsgHeader;/* ��Ϣͷ                                   */ /*_H2ASN_Skip*/
    TAF_MMA_TIMER_STATUS_ENUM_UINT8     enTimerStatus;
    VOS_UINT8                           aucReserve[3];
    VOS_UINT32                          ulLen;              /* ��ʱ��ʱ�� */
}TAF_MMA_TIMER_INFO_STRU;

/*****************************************************************************
  4 ��������
*****************************************************************************/
VOS_VOID  TAF_MMA_StartTimer(
    TAF_MMA_TIMER_ID_ENUM_UINT32        enTimerId,
    VOS_UINT32                          ulLen
);

VOS_VOID  TAF_MMA_StopTimer(
    TAF_MMA_TIMER_ID_ENUM_UINT32        enTimerId
);

TAF_MMA_TIMER_STATUS_ENUM_UINT8  TAF_MMA_GetTimerStatus(
    TAF_MMA_TIMER_ID_ENUM_UINT32        enTimerId
);

VOS_VOID  TAF_MMA_SndOmTimerStatus(
    TAF_MMA_TIMER_STATUS_ENUM_UINT8     enTimerStatus,
    TAF_MMA_TIMER_ID_ENUM_UINT32        enTimerId,
    VOS_UINT32                          ulLen
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

#endif /* TAF_MMA_TIMER_MGMT_H */


