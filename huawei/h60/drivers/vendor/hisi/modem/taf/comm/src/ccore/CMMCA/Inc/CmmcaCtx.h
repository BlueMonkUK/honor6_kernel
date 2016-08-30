/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : CmmcaCtx.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2014��1��8��
  ����޸�   :
  ��������   : CmmcaCtx.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2014��1��8��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __CMMCACTX_H__
#define __CMMCACTX_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "PsCommonDef.h"
#include "pslog.h"
#include "TafAppMma.h"
#include "CmmcaCtxPkt.h"
#include "CmmcaTimerMgmt.h"


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
#define CMMCA_WARNING_LOG(string)                           vos_printf("%s \r\n", string)
#define CMMCA_WARNING1_LOG(string, para1)                   vos_printf("%s %d.\r\n", string, para1)
#define CMMCA_WARNING2_LOG(string, para1, para2)            vos_printf("%s %d, %d.\r\n", string, para1, para2)

#define CMMCA_ERROR_LOG(string)                             vos_printf("%s \r\n", string)
#define CMMCA_ERROR1_LOG(string, para1)                     vos_printf("%s %d.\r\n", string, para1)
#define CMMCA_ERROR2_LOG(string, para1, para2)              vos_printf("%s %d, %d.\r\n", string, para1, para2)
#else
#define CMMCA_WARNING_LOG(string)                           PS_LOG(WUEPS_PID_CMMCA, 0, PS_PRINT_WARNING,  string)
#define CMMCA_WARNING1_LOG(string, para1)                   PS_LOG1(WUEPS_PID_CMMCA, 0, PS_PRINT_WARNING,  string, (VOS_INT32)para1)
#define CMMCA_WARNING2_LOG(string, para1, para2)            PS_LOG2(WUEPS_PID_CMMCA, 0, PS_PRINT_WARNING,  string, (VOS_INT32)para1, (VOS_INT32)para2)

#define CMMCA_ERROR_LOG(string)                             PS_LOG(WUEPS_PID_CMMCA, 0, PS_PRINT_ERROR,  string)
#define CMMCA_ERROR1_LOG(string, para1)                     PS_LOG1(WUEPS_PID_CMMCA, 0, PS_PRINT_ERROR,  string, (VOS_INT32)para1)
#define CMMCA_ERROR2_LOG(string, para1, para2)              PS_LOG2(WUEPS_PID_CMMCA, 0, PS_PRINT_ERROR,  string, (VOS_INT32)para1, (VOS_INT32)para2)
#endif

#if (FEATURE_ON == FEATURE_CL_INTERWORK)

#define CMMCA_OPID_INVILID_VALUE                            (0xFF)
#define CMMCA_OPID_MIN_VALUE                                (1)
#define CMMCA_OPID_MAX_VALUE                                (200)

/* CMMCA �����ĵ�ַ */
#define CMMCA_GetCtxAddr()                                  (&g_stCmmcaCtx)

/* ��ȡ��ǰ��OPID */
#define CMMCA_GetCurMaxOpid()                               (CMMCA_GetCtxAddr()->ucCurMaxOpid)

/* ���õ�ǰ��OPID */
#define CMMCA_SetCurMaxOpid(ucCurMaxOpid)                   (CMMCA_GetCurMaxOpid() = (ucCurMaxOpid))

/* ��ȡ��ǰModem��״̬ */
#define CMMCA_GetModemStatus()                              (CMMCA_GetCtxAddr()->enModemStatus)

/* ���õ�ǰModem��״̬ */
#define CMMCA_SetModemStatus(enModemStatus)                 (CMMCA_GetModemStatus() = (enModemStatus))

/* �ȴ���Ϣ����Ķ��г��� */
#define CMMCA_MAX_CMD_BUFFER_QUEUE_SIZE                     (5)

/* CMMCA�շ�����Ϣ�ṹ�У�������ʱ���� */
#define CMMCA_MSG_DATA_RSV_LEN                              (4)

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

enum CMMCA_MODEM_STATUS_ENUM
{
    CMMCA_MODEM_STATUS_INACTIVE,                                                /* Modem״̬���ڹػ���Power Save״̬��L��ѡ��C */
    CMMCA_MODEM_STATUS_ACTIVE,                                                  /* Modem������������ע����Ѿ�ע��״̬ */
    CMMCA_MODEM_STATUS_BUTT
};
typedef VOS_UINT8 CMMCA_MODEM_STATUS_ENUM_UINT8;

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
    VOS_UINT8                           ucOpId;
    VOS_UINT8                           aucReserved[3];
    VOS_UINT8                          *pucMsgInfo;
    VOS_UINT32                          ulMsgInfoLen;
}CMMCA_CMD_BUFFER_STRU;


typedef struct
{
    VOS_UINT8                           ucIsRoamActive;
    VOS_UINT8                           aucReserved[3];
}CMMCA_USER_CFG_INFO_STRU;


typedef struct
{
    TAF_PHONE_SERVICE_STATUS                                ucCsSrvSta;         /*CS����״̬*/
    TAF_PHONE_SERVICE_STATUS                                ucPsSrvSta;         /*PS����״̬*/
    VOS_UINT8                                               ucCsSimValid;       /* CS���Ƿ���Ч��VOS_TRUE:��Ч��VOS_FALSE:��Ч */
    VOS_UINT8                                               ucPsSimValid;       /* PS���Ƿ���Ч��VOS_TRUE:��Ч��VOS_FALSE:��Ч */
    TAF_PLMN_ID_STRU                                        stPlmnId;
    VOS_UINT16                                              usArfcn;
    TAF_MMA_PLMN_PRIORITY_CLASS_ENUM_UINT8                  enPrioClass;
    TAF_PH_SERVICE_DOMAIN                                   ucSrvDomain;
    CMMCA_USER_CFG_INFO_STRU                                stUserCfgInfo;
}CMMCA_REG_CTX_STRU;


typedef struct
{
    VOS_UINT8                           ucCurMaxOpid;                           /* ��ǰ�����ȥ�����OPID */

    CMMCA_MODEM_STATUS_ENUM_UINT8       enModemStatus;

    VOS_UINT8                           aucReserved[2];

    /* CMMCA������Ϣ���� */
    CMMCA_CMD_BUFFER_STRU               astCmdBufferQueue[CMMCA_MAX_CMD_BUFFER_QUEUE_SIZE];

    /* Packet Data ��ص�ȫ�ֱ��� */
    CMMCA_PKT_CTX_STRU                  stPktCtx;

    CMMCA_REG_CTX_STRU                  stRegCtx;

    /* CMMCA��ʱ�������� */
    CMMCA_TIMER_CTX_STRU                astCmmcaTimer[CMMCA_MAX_TIMER_NUM];
}CMMCA_CTX_STRU;

/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/
extern CMMCA_CTX_STRU                          g_stCmmcaCtx;


/*****************************************************************************
  10 ��������
*****************************************************************************/
CMMCA_REG_CTX_STRU* CMMCA_REG_GetCtxAddr(VOS_VOID);

VOS_VOID CMMCA_REG_InitCtx(VOS_VOID);

VOS_VOID CMMCA_REG_ReadRoamCfgInfoNvim(VOS_VOID);

CMMCA_TIMER_CTX_STRU* CMMCA_GetTiCtx(VOS_VOID);

VOS_VOID CMMCA_InitCtx(VOS_VOID);

VOS_VOID CMMCA_AssignOpid(
    VOS_UINT8                          *pucOpid
);

CMMCA_CMD_BUFFER_STRU* CMMCA_GetCmdBufferQueueAddr(VOS_VOID);

VOS_VOID CMMCA_InitCmdBufferQueue(
    CMMCA_CMD_BUFFER_STRU              *pstCmdBufferQueue
);

VOS_VOID CMMCA_DelItemInCmdBufferQueue(
    VOS_UINT8                           ucOpId
);

VOS_UINT8 CMMCA_SaveItemInCmdBufferQueue(
    VOS_UINT8                           ucOpId,
    VOS_UINT8                          *pucMsgInfo,
    VOS_UINT32                          ulMsgInfoLen
);

CMMCA_CMD_BUFFER_STRU* CMMCA_GetItemFromCmdBufferQueue(
    VOS_UINT8                           ucOpId
);
#endif

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

#endif /* end of CmmcaCtx.h */
