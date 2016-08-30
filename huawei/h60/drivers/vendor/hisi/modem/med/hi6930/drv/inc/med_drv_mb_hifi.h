

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/


#ifndef __DRV_MB_HIFI_H__
#define __DRV_MB_HIFI_H__

#include "vos.h"
#include "drv_mailbox.h"
#include "drv_mailbox_cfg.h"
#include "HifiOmInterface.h"
#include "ucom_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define DRV_SOCP_SRC_CHAN_ID_HIFI           (9)
#define DRV_SOCP_DES_CHAN_ID_HIFI           (3)                                 /*��ΪMSP��̬���룬������Ϊ��������*/
#define DRV_SOCP_CHAN_LEN_HIFI              (0x2000)
#define DRV_SOCP_GET_ENCSRC_WPTR(m)         (DRV_SOCP_BASE_ADDR+(0x0100+((m)*0x40)))/* SOCP����Դ����дָ��Ĵ��� */
#define DRV_SOCP_GET_ENCSRC_RPTR(m)         (DRV_SOCP_BASE_ADDR+(0x0104+((m)*0x40)))/* SOCP����Դ�����ָ��Ĵ��� */
#define DRV_SOCP_GET_ENCSRC_BUFADDR(m)      (DRV_SOCP_BASE_ADDR+(0x0108+((m)*0x40)))/* SOCP����Դ�����׵�ַ�Ĵ��� */
#define DRV_SOCP_GET_ENCSRC_BUFCFG0(m)      (DRV_SOCP_BASE_ADDR+(0x010C+((m)*0x40)))/* SOCP����Դ���泤�ȼĴ��� */
#define DRV_SOCP_GET_ENCSRC_BUFCFG1(m)      (DRV_SOCP_BASE_ADDR+(0x0110+((m)*0x40)))

#define DRV_SOCP_GET_ENCSRC_RDQWPTR(m)      (DRV_SOCP_BASE_ADDR+(0x0114+((m)*0x40)))
#define DRV_SOCP_GET_ENCSRC_RDQRPTR(m)      (DRV_SOCP_BASE_ADDR+(0x0118+((m)*0x40)))
#define DRV_SOCP_GET_ENCSRC_RDQADDR(m)      (DRV_SOCP_BASE_ADDR+(0x011C+((m)*0x40)))
#define DRV_SOCP_GET_ENCSRC_RDQCFG(m)       (DRV_SOCP_BASE_ADDR+(0x0120+((m)*0x40)))

#define DRV_SOCP_GET_ENCDEST_WPTR(n)        (DRV_SOCP_BASE_ADDR+(0x0900+((n)*0x20)))
#define DRV_SOCP_GET_ENCDEST_RPTR(n)        (DRV_SOCP_BASE_ADDR+(0x0904+((n)*0x20)))
#define DRV_SOCP_GET_ENCDEST_BUFADDR(n)     (DRV_SOCP_BASE_ADDR+(0x0908+((n)*0x20)))
#define DRV_SOCP_GET_ENCDEST_BUFCFG(n)      (DRV_SOCP_BASE_ADDR+(0x090C+((n)*0x20)))

#define DRV_SOCP_ENC_SRST_REG               (DRV_SOCP_BASE_ADDR + (0x0004))     /* SOCP����ͨ����λ�Ĵ��� */
#define DRV_SOCP_REG_ENC_RAWINT1            (DRV_SOCP_BASE_ADDR + (0x0068))
#define DRV_SOCP_REG_APP_MASK1              (DRV_SOCP_BASE_ADDR + (0x0060))
#define DRV_SOCP_RESET_TIMECNT              (1000)                              /* SOCPͨ����λ���� */
#define DRV_SOCP_ENCSRC_BYPASS_DISABLE      (0U)                                /* ͨ����·û��ʹ�� */
#define DRV_SOCP_ENCSRC_CHNMODE_CTSPACKET   (0U)                                /* ��ʾ�������ݰ��ṹ */
#define DRV_SOCP_CHAN_PRIORITY_0            (0U)                                /* ͨ�����ȼ���ͣ�Ϊ0 */
#define DRV_SOCP_DATA_TYPE_1                (1U)                                /* SOCP��������ΪGU��ά�ɲ�����,��drvinterface.h�е�SOCP_DATA_TYPE_Eö�ٺ�����ͬ */

#define DRV_SOCP_MAGIC_NUMBER               (0x48495349)                        /* ASCII "HISI"  */
#define DRV_SOCP_USBHEAD_NUMBER             (0xaaaa5555)                        /* GU��USB���ݰ�ͷ */
#define DRV_SOCP_USBTAIL_NUMBER             (0x5555aaaa)                        /* GU��USB���ݰ�β */
#define DRV_SOCP_HIFI_SERVICE_ID            (0x7)                               /* ��������,�˴���0x7 */
#define DRV_SOCP_HIFI_SESSION_ID            (0x6)                               /* CPU��Χ���˴���0x6 */
#define DRV_SOCP_HEADER_LEN                 (sizeof(DRV_SOCP_EMPTY_MSG_STRU)-8) /* ��״buffer����ʱ��usMsgLen�������ֶκ󣨲���������Ϣ��ͷ�ĳ��ȣ���λbyte */
#define DRV_SOCP_SEG_NUM_ONE                (0x1)                               /* ��ʾ���ֶΣ��̶���1 */
#define DRV_SOCP_SEG_SN_ONE                 (0x1)                               /* 1��ʾ��ǰ�����Ϊ1 */

#define DRV_MAILBOX_PROTECT1                (MAILBOX_PROTECT1)                        /* ���䱣����1����������ͷ������ */
#define DRV_MAILBOX_PROTECT2                (MAILBOX_PROTECT2)                        /* ���䱣����2����������ͷ������ */
#define DRV_MAILBOX_PROTECT_LEN             (MAILBOX_PROTECT_LEN)                                 /* ���䱣���ֳ��ȣ���λbyte������ͷ������ͷ��β�������������ı����� */
#define DRV_MAILBOX_MSGHEAD_NUMBER          (MAILBOX_MSGHEAD_NUMBER)                        /* �����Ϣ��Ϣ�ָ��� */
#define DRV_MAILBOX_INFO_NO                 (3)                                 /* ������Ϣ�������е����������Ϣ���� */
#define DRV_MAILBOX_IPC_INFO_NUM            (2)                                 /* �����жϵ���Ϣ����Ŀ */

#define DRV_MAILBOX_MSG_DELAY_THD           ((VOS_UINT32)(DRV_TIMER_OM_FREQ/25))/* ���������Ϣ�����ʱ��(40ms)����Ӧ��Sliceֵ */

#define DRV_MAILBOX_GetMailBoxHeadAddr(uhwId)(g_uwDrvMailboxAddrTbl[(uhwId)][0])/* ��ȡ����uhwId������ͷ�׵�ַ */
#define DRV_MAILBOX_GetMailBoxAddr(uhwId)   (g_uwDrvMailboxAddrTbl[(uhwId)][1]) /* ��ȡ����uhwId���׵�ַ */
#define DRV_MAILBOX_GetMailBoxLen(uhwId)    (g_uwDrvMailboxAddrTbl[(uhwId)][2]) /* ��ȡ����uhwId�ĳ��� */

#define DRV_MAILBOX_InitMBHeadAddr(uhwId, uwAddr)      \
                                            (g_uwDrvMailboxAddrTbl[(uhwId)][0] = (uwAddr))/* ��ʼ������uhwId������ͷ�׵�ַ */
#define DRV_MAILBOX_InitMBAddr(uhwId, uwAddr)          \
                                            (g_uwDrvMailboxAddrTbl[(uhwId)][1] = (uwAddr)) /* ��ʼ������uhwId���׵�ַ */
#define DRV_MAILBOX_InitMBLen(uhwId, uwLen)          \
                                            (g_uwDrvMailboxAddrTbl[(uhwId)][2] = (uwLen)) /* ��ʼ������uhwId�ĳ��� */

#define DRV_MAILBOX_GetMailCode(uwCpuId, uwChannel) \
                                            (g_uwDrvMailCodeTbl[uwCpuId][uwChannel])

#define DRV_MAILBOX_GetMailBoxTx(uhwCpuId)  (g_uhwDrvMailboxIdTbl[uhwCpuId][0]) /* ��ȡHIFI��ӦuhwCpuId������������ */
#define DRV_MAILBOX_GetMailBoxRx(uhwCpuId)  (g_uhwDrvMailboxIdTbl[uhwCpuId][1]) /* ��ȡHIFI��ӦuhwCpuId������������ */

#define DRV_MAILBOX_GetTargetCpu(uhwCpuId)  (g_uhwDrvMailboxIpcTbl[uhwCpuId][0])/* ͨ��ȫϵͳCPU ID��ȡ���Ӧ��IPC Target Cpu��� */
#define DRV_MAILBOX_GetIntrSrc(uhwCpuId)    (g_uhwDrvMailboxIpcTbl[uhwCpuId][1])/* ��ȡָ��CPU��HiFi�����Ϣ֪ͨ�ж�bit��� */

#define DRV_MAILBOX_GetMailBoxSeqAddr()     (&g_auwDrvMailboxSeqNum[0])         /* ��ȡ���������Ϣ���м�¼ȫ�ֱ����׵�ַ */
#define DRV_MAILBOX_GetMailBoxSeqNum(uhwId) (g_auwDrvMailboxSeqNum[(uhwId)])    /* ��ȡ�ÿ�����䵱ǰ��¼����Ϣ���к�(SN��) */
#define DRV_MAILBOX_SetMailBoxSeqNum(uhwId, uwNum)          \
                                            (g_auwDrvMailboxSeqNum[(uhwId)] = (uwNum))/* ���øÿ�����䵱ǰ��¼����Ϣ���к�(SN��) */
/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
 ʵ������  : DRV_MAILBOX_ID_ENUM
 ��������  : ������ö�ٶ���
*****************************************************************************/
enum DRV_MAILBOX_ID_ENUM
{
    DRV_MAILBOX_ID_HIFI2CCPU = 0,       /* HIFI��CCPU������ */
    DRV_MAILBOX_ID_CCPU2HIFI,           /* CCPU��HIFI������ */
    DRV_MAILBOX_ID_HIFI2ACPU,           /* HIFI��ACPU������ */
    DRV_MAILBOX_ID_ACPU2HIFI,           /* ACPU��HIFI������ */
    DRV_MAILBOX_ID_HIFI2BBE16,          /* HIFI��BBE16������ */
    DRV_MAILBOX_ID_BBE162HIFI,          /* BBE16��HIFI������ */
    DRV_MAILBOX_ID_BUTT
};
typedef VOS_UINT16 DRV_MAILBOX_ID_ENUM_UINT16;


/*****************************************************************************
 ʵ������  : DRV_MAILBOX_TXRX_ENUM
 ��������  : ���͡����ձ�־
*****************************************************************************/
enum DRV_MAILBOX_TXRX_ENUM
{
    DRV_MAILBOX_TXRX_TX = 0,            /* ���� */
    DRV_MAILBOX_TXRX_RX,                /* ���� */
    DRV_MAILBOX_TXRX_BUTT
};
typedef VOS_UINT16 DRV_MAILBOX_TXRX_ENUM_UINT16;

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
 ʵ������  : DRV_MAILBOX_QUEUE_STRU
 ��������  : MAILBOX/SOCP���л�����ƽṹ
*****************************************************************************/
typedef struct
{
    VOS_UINT32      uwBase;            /* ���л���ַ       */
    VOS_UINT32      uwLength;          /* ���г��ȣ���λbyte */
    VOS_UINT32      uwFront;           /* ����дָ��, ���Ե�ַ��in   */
    VOS_UINT32      uwRear;            /* ���ж�ָ��, ���Ե�ַ��out  */
}DRV_MAILBOX_QUEUE_STRU;


/************************************************************************
 �ṹ��    : IPC_MSG_BLK_STRU
 �ṹ˵��  : �˼���Ϣ��
 ************************************************************************/
typedef struct
{
    MAILBOX_MSG_HEADER stMsgHead;      /* ��Ϣͷ */
    unsigned long      ulSenderCpuId;      /* ��Ϣ����CPU */
    unsigned long      ulSenderPid;        /* ��Ϣ����PID */
    unsigned long      ulReceiverCpuId;    /* ��Ϣ����CPU */
    unsigned long      ulReceiverPid;      /* ��Ϣ����PID */
    unsigned long      ulLength;           /* ��Ϣ���ĳ���,bytes */
    unsigned char      aucValue[4];        /* ��Ϣ���� */
} IPC_MSG_BLK_STRU;

/*****************************************************************************
 ʵ������  : DRV_SOCP_EMPTY_MSG_STRU
 ��������  : SOCPͨ������Ϣ�ṹ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          uwMagicNum;     /* 0x48495349 */
    VOS_UINT16                          uhwMsgLen;      /* ��Ϣ����   */
    VOS_UINT16                          uhwReserve;     /* ����       */
    VOS_UINT32                          uwReserve1;     /* ����OM������SOCP����ͨ��ͳһ��������32bit��Ϊ������ */
    OM_SOCP_FRAME_HEAD_STRU             stFrmHead;      /* OM֡ͷ     */
    VOS_UINT32                          uwUsbHead;      /* 0xaaaa5555 */
    OM_SOCP_FRAME_SEG_STRU              stFrmSegHead;   /* OM֡�ֶ�ͷ */
    VOS_UINT32                          uwUsbTail;      /* 0x5555aaaa */
}DRV_SOCP_EMPTY_MSG_STRU;

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_VOID DRV_MAILBOX_CheckRecMsg(
                       VOS_UINT32              uwMailboxId,
                       VOS_UINT32              uwTimeStamp,
                       MAILBOX_MSG_HEADER    *pstMsgHead);
extern VOS_VOID DRV_MAILBOX_Init( VOS_VOID );
extern VOS_UINT32 DRV_MAILBOX_Read(
                       IPC_MSG_BLK_STRU       *pstMsgBlk,
                       DRV_MAILBOX_QUEUE_STRU *pstQueue);
extern VOS_VOID DRV_MAILBOX_ReadQueue(
                       DRV_MAILBOX_QUEUE_STRU *pstQueue,
                       VOS_UCHAR              *pucData,
                       VOS_UINT32              uwSize);
extern VOS_VOID DRV_MAILBOX_ReceiveMsg(VOS_UINT32 uwMailboxId);
extern VOS_UINT32 DRV_MAILBOX_SendMsg(
                       VOS_UINT32                  uwCpuId,
                       VOS_UINT32                  uwChannel,
                       VOS_UINT32                  uwPriority,
                       VOS_UCHAR                  *pucMsg,
                       VOS_UINT32                  uwMsgSize);
extern VOS_UINT32 DRV_MAILBOX_Write(
                       DRV_MAILBOX_ID_ENUM_UINT16  enMailboxId,
                       VOS_UINT32                  uwCpuId,
                       VOS_UINT32                  uwPriority,
                       VOS_UCHAR                  *pucwMsg,
                       VOS_UINT32                  uwSize,
                       VOS_UINT32                  uwChannel);
extern VOS_VOID DRV_MAILBOX_WriteQueue(
                       DRV_MAILBOX_QUEUE_STRU *pstQueue,
                       VOS_UCHAR              *pucData,
                       VOS_UINT32              uwSize);
extern VOS_VOID DRV_SOCP_Init(VOS_VOID);
extern VOS_VOID DRV_SOCP_Stop(VOS_VOID);
extern VOS_UINT32 DRV_SOCP_Write(UCOM_DATA_BLK_STRU *pstBlk, VOS_UINT32 uwBlkNum);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of med_drv_mb_hifi.h */

