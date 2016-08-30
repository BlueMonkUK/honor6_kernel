

#ifndef __RRMDEBUG_H__
#define __RRMDEBUG_H__


/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "PsCommonDef.h"
#include "PsRrmInterface.h"
#include "RrmLog.h"



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
 �ṹ��    : RRM_MODEM_STATS_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : ��ͬModem��ͳ����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT32              ulRrmRcvResApplyNum;            /* RRM�յ�RFID��Դ����Ĵ��� */
    VOS_UINT32              ulRrmSndApplyCnfNum;            /* RRM�ظ�RFID��Դ����Ĵ��� */
    VOS_UINT32              ulRrmRcvResReleaseNum;          /* RRM�յ��ͷ�RFID��Դ��Ϣ�Ĵ��� */
    VOS_UINT32              ulRrmRcvRegNum;                 /* RRM�յ�REG��Ϣ�Ĵ��� */
    VOS_UINT32              ulRrmRcvDeregNum;               /* RRM�յ�DEREG��Ϣ�Ĵ��� */
}RRM_MODEM_STATS_INFO_STRU;


/*****************************************************************************
 �ṹ��    : RRM_STATS_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : RRM��ͳ����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                  ulRrmSndStatusIndNum;              /* RRM����״̬֪ͨ�Ĵ��� */
    RRM_MODEM_STATS_INFO_STRU   astRrmModemStats[MODEM_ID_BUTT];   /* RRM MODEM��ͳ����Ϣ */
}RRM_STATS_INFO_STRU;


extern   RRM_STATS_INFO_STRU    g_stRrmStats;


/* RRM�յ�RFID��Դ����ͳ���� */
#define  RRM_DBG_MODEM_RX_RES_APPLY_REQ_NUM(ModemId,n) \
    ((ModemId < MODEM_ID_BUTT) ? (g_stRrmStats.astRrmModemStats[ModemId].ulRrmRcvResApplyNum  += (n)) : PS_FALSE)

/* RRM�ظ�RFID��Դ����ͳ���� */
#define  RRM_DBG_MODEM_SND_RES_APPLY_CNF_NUM(ModemId,n) \
    ((ModemId < MODEM_ID_BUTT) ? (g_stRrmStats.astRrmModemStats[ModemId].ulRrmSndApplyCnfNum  += (n)) : PS_FALSE)


/* RRM�յ�RFID��Դ�ͷ�ͳ���� */
#define  RRM_DBG_MODEM_RX_RES_RELEASE_NUM(ModemId,n) \
    ((ModemId < MODEM_ID_BUTT) ? (g_stRrmStats.astRrmModemStats[ModemId].ulRrmRcvResReleaseNum  += (n)) : PS_FALSE)

/* RRM�յ�REG��Ϣͳ���� */
#define  RRM_DBG_MODEM_RX_REG_NUM(ModemId,n) \
    ((ModemId < MODEM_ID_BUTT) ? (g_stRrmStats.astRrmModemStats[ModemId].ulRrmRcvRegNum  += (n)) : PS_FALSE)

/* RRM�յ�DEREG��Ϣͳ���� */
#define  RRM_DBG_MODEM_RX_DEREG_NUM(ModemId,n) \
    ((ModemId < MODEM_ID_BUTT) ? (g_stRrmStats.astRrmModemStats[ModemId].ulRrmRcvDeregNum  += (n)) : PS_FALSE)

/* RRM����STATUS IND��Ϣͳ���� */
#define  RRM_DBG_SND_STATUS_IND_NUM(n) \
    (g_stRrmStats.ulRrmSndStatusIndNum  += (n))

/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/

VOS_VOID  RRM_InitDebugInfo(VOS_VOID);

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

#endif /* end of RrmDebug.h */

