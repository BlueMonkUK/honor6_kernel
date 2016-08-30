

#ifndef __VOICE_SJB_H__
#define __VOICE_SJB_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "ImsCodecInterface.h"
#include "voice_jb_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define VOICE_SJB_RX_DEPTH               (15)                         /* ����JB����� */
#define VOICE_SJB_TX_DEPTH               (3)                          /* ����JB����� */
#define VOICE_SJB_INIT_DEPTH             (4)                          /* ��ʼ������� */
#define VOICE_SJB_UINT32_MAX             (0xFFFFFFFF)                 /* UINT32�����ֵ */
#define VOICE_SJB_20MSTS_WB              (320)                        /* 16K������20ms��Ӧ��TS */
#define VOICE_SJB_20MSTS_NB              (160)                        /* 8K������20ms��Ӧ��TS */
#define VOICE_SJB_MaxTSDiff              (48*20*5)                    /* 48K�����ʣ�20ms,5֡��TS */
#define VOICE_SJB_GetRxSjbCtrl()         (&g_stRxSjbManager)          /* ����JB���ƿ� */
#define VOICE_SJB_GetTxSjbCtrl()         (&g_stTxSjbManager)          /* ����JB���ƿ� */

#define VOICE_SJB_TSErrCntMax            (5)                          /* ������TS���ɶ���������
                                                                        ��ֵ���ͳ�ʼ��JB */

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
enum VOICE_SJB_ITEM_STATUS_ENUM
{
    VOICE_SJB_ITEM_STATUS_EMPTY            = 0,                       /* ������Ԫ��Ϊ�� */
    VOICE_SJB_ITEM_STATUS_NOTEMPTY,                                   /* ������Ԫ�ز��� */
    VOICE_SJB_ITEM_STATUS_BUTT
};
typedef VOS_UINT32 VOICE_SJB_ITEM_STATUS_ENUM_UINT32;

/* JB�Ƿ��Ѿ���ʼ����ȫ */
enum VOICE_SJB_INIT_STATUS_ENUM
{
    VOICE_SJB_INIT_STATUS_NOT              = 0,                       /* ��ʼ��δ��� */
    VOICE_SJB_INIT_STATUS_DONE,                                       /* ��ʼ����� */
    VOICE_SJB_INIT_STATUS_BUTT
};
typedef VOS_UINT32 VOICE_SJB_INIT_STATUS_ENUM_UINT32;
/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/

/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/

/*****************************************************************************
  6 STRUCT����
*****************************************************************************/

/*****************************************************************************
 ʵ������  : VOICE_SJB_PKT_RX_STRU
 ��������  : �������ݰ�
*****************************************************************************/
typedef struct
{
    IMSA_VOICE_RX_DATA_IND_STRU       stPktData;                   /* ��Ϣ���� */

    VOS_VOID                         *pstPrev;                     /* ��һ֡��ַ */
    VOS_VOID                         *pstNext;                     /* ��һ֡��ַ */
    VOICE_SJB_ITEM_STATUS_ENUM_UINT32  enIsNotEmpty;               /* ���Ԫ����û������ */
} VOICE_SJB_PKT_RX_STRU;

/*****************************************************************************
 ʵ������  : VOICE_SJB_PKT_TX_STRU
 ��������  : �������ݰ�
*****************************************************************************/
typedef struct
{
    VOICE_IMSA_TX_DATA_IND_STRU       stPktData;                   /* ��Ϣ���� */

    VOS_VOID                         *pstPrev;                     /* ��һ֡��ַ */
    VOS_VOID                         *pstNext;                     /* ��һ֡��ַ */
    VOICE_SJB_ITEM_STATUS_ENUM_UINT32  enIsNotEmpty;               /* ���Ԫ����û������ */
} VOICE_SJB_PKT_TX_STRU;

/*****************************************************************************
 ʵ������  : VOICE_SJB_RX_BUF_CTRL_STRU
 ��������  : ����SJB������
*****************************************************************************/
typedef struct
{
    VOICE_SJB_PKT_RX_STRU              stPkts[VOICE_SJB_RX_DEPTH]; /* ���ݰ����� */
    VOICE_SJB_PKT_RX_STRU             *pstHead;                    /* ���׵�ַ */
    VOICE_SJB_PKT_RX_STRU             *pstTail;                    /* ��β��ַ */
} VOICE_SJB_RX_BUF_CTRL_STRU;

/*****************************************************************************
 ʵ������  : VOICE_SJB_TX_BUF_CTRL_STRU
 ��������  : ����SJB������
*****************************************************************************/
typedef struct
{
    VOICE_SJB_PKT_TX_STRU              stPkts[VOICE_SJB_TX_DEPTH]; /* ���ݰ����� */
    VOICE_SJB_PKT_TX_STRU             *pstHead;                    /* ���׵�ַ */
    VOICE_SJB_PKT_TX_STRU             *pstTail;                    /* ��β��ַ */
} VOICE_SJB_TX_BUF_CTRL_STRU;

/*****************************************************************************
 ʵ������  : VOICE_SJB_STATE
 ��������  : SJB��״̬������
*****************************************************************************/
typedef struct
{
    VOS_UINT32                        uwTSof20ms;                  /* 20ms��Ӧ��TSֵ */
    VOICE_SJB_INIT_STATUS_ENUM_UINT32  enAlreadyInit;               /* ��ʼ����ɱ�־ */
} VOICE_SJB_STATE;

/*****************************************************************************
 ʵ������  : VOICE_SJB_PARA
 ��������  : SJB��ͳ����
*****************************************************************************/
typedef struct
{
    VOS_UINT32                        uwLastTS;                    /* �����TS */
    VOS_UINT32                        uwCurPktNum;                 /* ��ǰ�����а��� */
    VOS_UINT32                        uwRcvPktNum;                 /* �յ��İ��� */
    VOS_UINT32                        uwLostByStore;               /* ���ʱ���ֵĶ����� */
    VOS_UINT32                        uwLostByGet;                 /* ȡ��ʱ���ֵĶ��� */
    VOS_UINT32                        uwTSErrCnt;                  /* ��TS���ɶ����������İ��� */
    VOS_UINT32                        uwGetErrCnt;                 /* ����ȡ��ʧ�ܴ��� */
} VOICE_SJB_PARA;

/*****************************************************************************
 ʵ������  : VOICE_SJB_RX_MANAGER_STRU
 ��������  : ����JitterBuffer���ƿ�
*****************************************************************************/
typedef struct
{
    VOICE_SJB_RX_BUF_CTRL_STRU         stBuff;                      /* buff���ƿ� */
    VOICE_SJB_STATE                    stState;                     /* ״̬�� */
    VOICE_SJB_PARA                     stPara;                      /* ͳ���� */
} VOICE_SJB_RX_MANAGER_STRU;

/*****************************************************************************
 ʵ������  : VOICE_SJB_TX_MANAGER_STRU
 ��������  : ����JitterBuffer���ƿ�
*****************************************************************************/
typedef struct
{
    VOICE_SJB_TX_BUF_CTRL_STRU         stBuff;                       /* buff���ƿ� */
    VOICE_SJB_STATE                    stState;                      /* ״̬�� */
    VOICE_SJB_PARA                     stPara;                       /* ͳ���� */
    VOICE_JB_PKT_TIME_ENUM_UINT32      enPktTime;                    /* ����ʱ�� */
} VOICE_SJB_TX_MANAGER_STRU;


/*****************************************************************************
  7 UNION����
*****************************************************************************/

/*****************************************************************************
  8 OTHERS����
*****************************************************************************/

/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern VOICE_SJB_RX_MANAGER_STRU       g_stRxSjbManager;
extern VOICE_SJB_TX_MANAGER_STRU       g_stTxSjbManager;


/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_VOID VOICE_SJB_InitTx( VOICE_SJB_TX_MANAGER_STRU *pstSjbManager);
extern VOS_UINT32 VOICE_SJB_StorePacketTx( VOICE_SJB_TX_MANAGER_STRU *pstSjbManager, VOICE_IMSA_TX_DATA_IND_STRU *pstPkt);
extern VOS_UINT32 VOICE_SJB_RemoveTailTx( VOICE_SJB_TX_MANAGER_STRU  *pstSjbManager);
extern VOS_UINT32 VOICE_SJB_InsertPktTx(
                VOICE_SJB_TX_MANAGER_STRU               *pstSjbManager,
                VOICE_SJB_PKT_TX_STRU                   *pstPktToStore,
                VOICE_SJB_PKT_TX_STRU                   *pstPktPrev,
                VOICE_SJB_PKT_TX_STRU                   *pstPktNext );
extern VOICE_SJB_PKT_TX_STRU* VOICE_SJB_FindEmptyPtrTx( VOICE_SJB_TX_MANAGER_STRU *pstSjbManager );
extern VOS_UINT32 VOICE_SJB_GetPacketTx( VOICE_SJB_TX_MANAGER_STRU *pstSjbManager, VOICE_IMSA_TX_DATA_IND_STRU *pstPkt);

extern VOS_VOID VOICE_SJB_InitRx( VOICE_SJB_RX_MANAGER_STRU *pstSjbManager);
extern VOS_UINT32 VOICE_SJB_StorePacketRx( VOICE_SJB_RX_MANAGER_STRU *pstSjbManager, IMSA_VOICE_RX_DATA_IND_STRU *pstPkt);
extern VOS_UINT32 VOICE_SJB_RemoveTailRx( VOICE_SJB_RX_MANAGER_STRU *pstSjbManager);
extern VOS_UINT32 VOICE_SJB_InsertPktRx(
                VOICE_SJB_RX_MANAGER_STRU               *pstSjbManager,
                VOICE_SJB_PKT_RX_STRU                   *pstPktToStore,
                VOICE_SJB_PKT_RX_STRU                   *pstPktPrev,
                VOICE_SJB_PKT_RX_STRU                   *pstPktNext );
extern VOICE_SJB_PKT_RX_STRU* VOICE_SJB_FindEmptyPtrRx( VOICE_SJB_RX_MANAGER_STRU *pstSjbManager );
extern VOS_UINT32 VOICE_SJB_GetPacketRx( VOICE_SJB_RX_MANAGER_STRU *pstSjbManager, IMSA_VOICE_RX_DATA_IND_STRU *pstPkt);
extern VOS_UINT32 VOICE_SJB_GetPktNumTx( VOICE_SJB_TX_MANAGER_STRU* pstSjbManager );
extern VOS_UINT32 VOICE_SJB_TSCompare( VOS_UINT32 uwSrc, VOS_UINT32 uwDest );
extern VOS_UINT32 VOICE_SJB_SetPktTimeTx(
                VOICE_SJB_TX_MANAGER_STRU               *pstSjbManager,
                VOICE_JB_PKT_TIME_ENUM_UINT32            enPktTime);


#ifdef __cplusplus
}
#endif

#endif


