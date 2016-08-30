/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : fsm.h
  �� �� ��   : ����
  ��    ��   : �ۺ��� 45755
  ��������   : 2009��7��10��
  ����޸�   :
  ��������   : fsm.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2009��7��10��
    ��    ��   : �ۺ��� 45755
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __FSM_H__
#define __FSM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"


/*****************************************************************************
  2 �궨��
*****************************************************************************/

/* ״̬������� */
#define FSM_STA_TBL_ITEM(enMainState, enSubState, astActTbl)\
        {\
            ((((VOS_UINT32)(enMainState)) << 16) | ((VOS_UINT16)(enSubState))),\
            ((sizeof(astActTbl))/sizeof(ACT_STRU)),\
            (astActTbl)\
        }

#define FSM_ACT_TBL_ITEM(PID, enMsgType, pActFun)\
        {\
            ((((VOS_UINT32)(PID)) << 16) | ((VOS_UINT16)(enMsgType))),\
            ((pActFun))\
        }

#define DSP_MAX_STACK_DEPTH                             (8)                     /* ״̬��ջ����� */
/*FSM �ṹ�����ռ�,����STA_STRU/ACT_STRU*/
#define FSM_COMP_LENGTH_MAX                             (12)


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/

/* �����쳣����ԭ�� */
typedef VOS_UINT32 ( * EXCEP_FUN )
(
    VOS_UINT32, /* EVENT ID,����PID��MSG ID */
    VOS_VOID *  /* ��Ϣָ�� */
);

/* ���������������Ͷ��� */
typedef VOS_UINT32 ( * ACTION_FUN )
(
    VOS_VOID *  /* ��Ϣָ�� */
);

/* ������ṹ */
typedef struct ACT
{
    VOS_UINT32      ulEventType;   /* �¼����� */
    ACTION_FUN      pfActionFun;   /* _H2ASN_Replace VOS_UINT32 pfActionFun; */ /* �������� */
}  ACT_STRU;


/* ״̬ת�Ʊ�ṹ */
typedef struct STA
{
    VOS_UINT32       ulState;       /* ״̬                                */
    VOS_UINT32       ulSize;        /* ������Ĵ�С�ֽ�������ACT�ṹ�Ĵ�С */
    ACT_STRU        *pActTable;     /* _H2ASN_Replace VOS_UINT32 pActTable; */ /* ������Ļ���ַ                      */
}  STA_STRU;

/* ����״̬���������ṹ */
typedef struct FSM_DESC
{
    STA_STRU*       pStaTable;                /* ״̬ת�Ʊ�Ļ���ַ     */
    VOS_UINT32      ulSize;                   /* ״̬ת�Ʊ�Ĵ�С       */
    EXCEP_FUN       pfExceptHandle;           /* _H2ASN_Replace VOS_UINT32 pfExceptHandle; */ /* ���ڴ����쳣�Ļص����� */
}  FSM_DESC_STRU;


/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/
typedef VOS_INT16 (* FSM_COMP_FUNC)(const VOS_VOID*, const VOS_VOID*);


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
VOS_VOID FSM_Swap(
                        VOS_INT16           *pshwTarget,
                        VOS_INT16           *pshwSource,
                        VOS_UINT32           uwLength );
extern VOS_VOID * FSM_Bsearch(
                                    VOS_INT16      *pKey,
                                    VOS_UINT8      *pBase,
                                    VOS_UINT32      uwNum,
                        VOS_UINT32 uwWidth,
                        FSM_COMP_FUNC fCompareFunc);

extern VOS_INT16 FSM_ActCompare( const VOS_VOID *arg1, const VOS_VOID *arg2 );
extern ACTION_FUN FSM_FindAct( FSM_DESC_STRU  *pFsmDesc,
                                    VOS_UINT32      uwState,
                                    VOS_UINT32      uwEventType);
extern VOS_UINT32 FSM_ProcessEvent(
                           FSM_DESC_STRU                   *pFsmDesc,
                           VOS_UINT32                       uwCurState,
                           VOS_UINT32                       uwEventType,
                           VOS_UINT16                       uhwMsgID,
                           VOS_VOID                        *pRcvMsg );
extern VOS_UINT32 FSM_RegisterFsm( FSM_DESC_STRU   *pstFsmDesc,
                                         VOS_UINT32       uwSize,
                                         STA_STRU        *pstStaTable,
                                         EXCEP_FUN        pfExceptHandle );
extern VOS_VOID FSM_Sort(
                             VOS_UINT8            *puhwTable,
                             VOS_UINT32            uwNum,
                             VOS_UINT32            uwWidth,
                             FSM_COMP_FUNC         fCompareFunc);
extern VOS_INT16 FSM_StaCompare( const VOS_VOID *arg1, const VOS_VOID *arg2 );
extern VOS_VOID FSM_StaSort(STA_STRU* pstTable, VOS_UINT32 uwSize);







#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of fsm.h */
