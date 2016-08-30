

#ifndef __QOS_FC_RAB_STATUS_H__
#define __QOS_FC_RAB_STATUS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "QosFcCommon.h"

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/

/*****************************************************************************
  3 Massage Declare
*****************************************************************************/

/*****************************************************************************
  4 ö�ٶ���
*****************************************************************************/
/*****************************************************************************
 �ṹ��    : QOS_FC_RAB_STATE_ENUM
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �������߳��ص�״̬����״̬��GU��L�����岻ͬ:��GU��˵�������RAB
             ��״̬����LTE��˵�������DRB��״̬
*****************************************************************************/
typedef enum QOS_FC_RAB_STATE
{
    QOS_FC_RAB_STATE_NORMAL,           /* ��������״̬ */
    QOS_FC_RAB_STATE_RANDOM_DISCARD,   /* �������״̬ */
    QOS_FC_RAB_STATE_DISCARD,          /* ����״̬ */
    QOS_FC_RAB_STATE_NOT_DISCARD,      /* ���ز�����״̬��STICK */
    QOS_FC_RAB_STATE_BUTT
} QOS_FC_RAB_STATE_ENUM;

typedef VOS_UINT32 QOS_FC_RAB_STATE_ENUM_UINT32;


/*****************************************************************************
   5 STRUCT����
*****************************************************************************/
/*****************************************************************************
 �ṹ��    : QOS_FC_RAB_ENTITY_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : ����CDS����ʵ��ṹ
*****************************************************************************/
typedef struct QOS_FC_RAB_ENTITY
{
    VOS_UINT8               ucRabId;                      /* ���ر�ʶ  */
    QCI_TYPE_ENUM_UINT8     enQci;
    VOS_UINT8               ucPriority;                   /* �������ȼ� */
    VOS_UINT8               ucLinkRabId;                  /* �����ر�ʶ  */
    QOS_FC_RAB_STATE_ENUM_UINT32       enRabState;
} QOS_FC_RAB_ENTITY_STRU;



/*****************************************************************************
  6 UNION����
*****************************************************************************/


/*****************************************************************************
  7 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  8 ��������
*****************************************************************************/
extern VOS_VOID QosFcRabInit(VOS_VOID);
extern VOS_UINT32 QosFc_RestoreHighPriRab(VOS_VOID);
extern VOS_UINT32 QosFc_DiscardAllDataFlow(VOS_VOID);
extern VOS_UINT32 QosFc_RestoreDataFlow(VOS_VOID);
extern VOS_UINT32 QosFc_DiscardDataFlow(VOS_VOID);
extern VOS_UINT32 QosFc_RandomDiscardDataFlow(VOS_VOID);

extern VOS_VOID QosFc_RabCreate(CONST MsgBlock  *pstMsg);
extern VOS_VOID QosFc_RabRelease(CONST MsgBlock  *pstMsg);

/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif

