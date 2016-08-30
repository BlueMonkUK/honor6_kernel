

#ifndef __TAFMTACTX_H__
#define __TAFMTACTX_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "TafMtaTimerMgmt.h"
#include "AtMtaInterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/
/* �ȴ���Ϣ����Ķ��г��ȡ�������ʱ��ʱ����ͬʱ������Ϣ���ʸ�ֵ��TAF_MTA_CTX_MAX_TIMER_NUMһ�� */
#define TAF_MTA_MAX_CMD_BUFFER_QUEUE_SIZE                   (5)

#define TAF_MTA_REFCLOCK_DEFAULT_FREQ                       (19200000)
#define TAF_MTA_REFCLOCK_DEFAULT_PRECISION                  (100)

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

typedef struct
{
    VOS_CHAR                           *pcBufHead;                              /* ���XML�����ڴ���׵�ַ */
    VOS_CHAR                           *pcBufCur;                               /* ��ǰ�����ڴ���׵�ַ */
}TAF_MTA_XML_TEXT_STRU;


typedef struct
{
    AT_MTA_CMD_RPT_FLG_ENUM_UINT8       enRptFlg;                               /* �����ϱ����ر�־ */
    VOS_UINT8                           aucReserved1[3];                        /* ����λ */
    VOS_UINT32                          ulFreq;                                 /* GPS�ο�ʱ�ӵ�Ƶ��ֵ����λHz */
    VOS_UINT32                          ulPrecision;                            /* ��ǰGPS�ο�ʱ�ӵľ��ȣ���λppb */
    PHY_MTA_REFCLOCK_STATUS_ENUM_UINT16 enStatus;                               /* ʱ��Ƶ������״̬ */
    VOS_UINT8                           aucReserved2[2];                        /* ����λ */
} TAF_MTA_REFCLOCK_INFO_STRU;


typedef struct
{
    TAF_MTA_XML_TEXT_STRU               stXmlText;                              /* ���XML�������ڴ� */
    TAF_MTA_REFCLOCK_INFO_STRU          stRefClockInfo;                         /* GPS�ο�ʱ��״̬��Ϣ */
}TAF_MTA_AGPS_CTX_STRU;


typedef struct
{
    TAF_MTA_TIMER_ID_ENUM_UINT32        enTimerId;
    VOS_UINT8                          *pucMsgInfo;
    VOS_UINT32                          ulMsgInfoLen;
}TAF_MTA_CMD_BUFFER_STRU;


typedef struct
{
    VOS_UINT16                          usNewMipiClk;                           /* �������µ�MIPICLKֵ */
    VOS_UINT16                          usReportMipiClk;                        /* �����Ѿ��ϱ����ĵ�MIPICLKֵ */
    VOS_UINT32                          ulTimerInterVal;                        /* ����MIPICLK�ϱ���ʱ���� */
    VOS_UINT16                          usEnableBitMap;                         /* ʹ�ܿ��أ�ÿ��bit����ͬ���㷨����0����ʹ�� */
    VOS_UINT8                           aucReserved2[2];                        /* ����λ */
}TAF_MTA_RF_LCD_CTX_STRU;
typedef struct
{
    /* ��¼AGPS XML��������������Դ */
    TAF_MTA_AGPS_CTX_STRU           stAgpsCtx;

    /* MTA��ǰ�������еĶ�ʱ����Դ */
    TAF_MTA_TIMER_CTX_STRU          astTimerCtx[TAF_MTA_CTX_MAX_TIMER_NUM];

    /* MTA������Ϣ���� */
    TAF_MTA_CMD_BUFFER_STRU         astCmdBufferQueue[TAF_MTA_MAX_CMD_BUFFER_QUEUE_SIZE];

    TAF_MTA_RF_LCD_CTX_STRU         stMtaRfLcdCtx;

}TAF_MTA_CONTEXT_STRU;

/*****************************************************************************
  8 UNION����
*****************************************************************************/
/*****************************************************************************
  9 OTHERS����
*****************************************************************************/
/*****************************************************************************
  10 ��������
*****************************************************************************/
extern TAF_MTA_AGPS_CTX_STRU*  TAF_MTA_GetMtaAgpsCtxAddr( VOS_VOID );

extern TAF_MTA_CONTEXT_STRU*  TAF_MTA_GetMtaCtxAddr( VOS_VOID );

extern TAF_MTA_TIMER_CTX_STRU*  TAF_MTA_GetTimerCtxAddr( VOS_VOID );

extern VOS_VOID  TAF_MTA_InitCmdBufferQueue(
            TAF_MTA_CMD_BUFFER_STRU            *pstCmdBufferQueue);

extern TAF_MTA_CMD_BUFFER_STRU  *TAF_MTA_GetCmdBufferQueueAddr( VOS_VOID );

extern VOS_VOID  TAF_MTA_DelItemInCmdBufferQueue(
            TAF_MTA_TIMER_ID_ENUM_UINT32        enTimerId);

extern VOS_VOID  TAF_MTA_SaveItemInCmdBufferQueue(
            TAF_MTA_TIMER_ID_ENUM_UINT32        enTimerId,
            VOS_UINT8                          *pucMsgInfo,
            VOS_UINT32                          ulMsgInfoLen);

extern TAF_MTA_CMD_BUFFER_STRU  *TAF_MTA_GetItemFromCmdBufferQueue(
            TAF_MTA_TIMER_ID_ENUM_UINT32                enTimerId);


extern VOS_VOID TAF_MTA_InitRefClockInfo(
    TAF_MTA_REFCLOCK_INFO_STRU         *pstRefClockInfo
);

extern TAF_MTA_REFCLOCK_INFO_STRU* TAF_MTA_GetRefClockInfo(VOS_VOID);

VOS_VOID  TAF_MTA_InitRfLcdIntrusionCtx(
    TAF_MTA_RF_LCD_CTX_STRU            *pstRfLcdCtx
);
TAF_MTA_RF_LCD_CTX_STRU* TAF_MTA_GetRfLcdCtxAddr( VOS_VOID );




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

#endif


