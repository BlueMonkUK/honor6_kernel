/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : om_comm.h
  �� �� ��   : ����
  ��    ��   : ��ׯ�� 59026
  ��������   : 2011��6��1��
  ����޸�   :
  ��������   : om_comm.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��6��1��
    ��    ��   : ��ׯ�� 59026
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "ucom_comm.h"

/* _H2ASN_Include OmCodecInterface_new.h */

#ifndef __OM_COMM_H__
#define __OM_COMM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  2 �궨��
*****************************************************************************/

#define OM_COMM_TRANS_BLK_MAX_NUM       (4)                 /* ���η���͸�����ݿ������Ŀ */
#define OM_COMM_TRACE_BLK_MAX_NUM       (2)                 /* ���η���TRACE���ݿ������Ŀ */

/* ȫ�ֱ������ʷ�װ */
#define OM_COMM_GetMsgStatusPtr()       (&g_stOmStatusDesc)
#define OM_COMM_GetMsgFuncTbl()         (g_astOmMsgFuncTable)

#define OM_COMM_SEC_DTCM                UCOM_SECTION(".om.debug.bss")
#define OM_COMM_SetMsgSeqNum(uwVar)     (g_uwOmMsgSeqNum = (uwVar))             /*����HIFI��ǰ��¼����ϢSN��*/

#define OM_COMM_GetAndIncMsgSeqNum()    (g_uwOmMsgSeqNum++)                     /*��ȡHIFI��ǰ��¼����ϢSN�ţ�������1*/
#define OM_COMM_GetOMConnStatus()       (g_OMConnectionStatus)
#define OM_COMM_SetOMConnStatus(uhwsta) (g_OMConnectionStatus = (uhwsta))

/*****************************************************************************
 �� �� ��  : OM_COMM_SendTranMsg
 ��������  : ��͸����Ϣ��ʽ����ָ����Ϣ
 �������  : pstMsg - �����͵���Ϣ
             uwLen  - ��������Ϣ����
 �������  : ��
*****************************************************************************/
#define OM_COMM_SendTranMsg(pstMsg, uwLen) /*lint -e{717} */        \
    do{                                                             \
        UCOM_DATA_BLK_STRU              stSendTransMsgDataBlk;      \
        stSendTransMsgDataBlk.pucData   = (VOS_UCHAR *)(pstMsg);    \
        stSendTransMsgDataBlk.uwSize    = (uwLen);                  \
        OM_COMM_SendTrans(&stSendTransMsgDataBlk, 1);               \
    }while(0)


/*****************************************************************************
 �� �� ��  : OM_COMM_CycAdd
 ��������  : ���Ǹ�����Χ��ѭ��, ��������������ĺ�
 �������  : Deta   - ����
             Pre    - ������
             CycLen - ���ζ������Կռ�ĳ���
 �������  : ��
 �� �� ֵ  : (Cur - Pre) ����(CycLen - Pre + Cur)
*****************************************************************************/
#define OM_COMM_CycAdd(Pre, Deta, CycLen)                    \
    (((Pre) + (Deta)) % (CycLen))

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


/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/
#if 0                                                       /* _H2ASN_Skip */
/*****************************************************************************
  H2ASN���ɵ�ASN�ļ���Ҫ���
*****************************************************************************/

/*****************************************************************************
  H2ASN������Ϣ�ṹ����
*****************************************************************************/
/* ��Ҫ������Ӧ��ASN����,ȥ��L��ʽ,������V��ʽ */
typedef struct
{
    CODEC_OM_MSG_ID_ENUM_UINT16           enMsgID;            /* _H2ASN_MsgChoice_Export CODEC_OM_MSG_ID_ENUM_UINT16 */

    VOS_UINT8                           aucMsgBlock[2];
    /***************************************************************************
        _H2ASN_MsgChoice_When_Comment          CODEC_OM_MSG_ID_ENUM_UINT16
    ****************************************************************************/
}CODEC_OM_TRACE_MSG_DATA;

/* Э����Ϣ�ṹ*/
typedef struct
{
    VOS_UINT32 uwSenderCpuId;
    VOS_UINT32 uwSenderPid;
    VOS_UINT32 uwReceiverCpuId;
    VOS_UINT32 uwReceiverPid;
    VOS_UINT32 uwLength;
    CODEC_OM_TRACE_MSG_DATA               stMsgData;
}CODECOmTrace_MSG;

#endif                                                     /* _H2ASN_Skip */

/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/

extern VOS_UINT16   g_OMConnectionStatus;

/*****************************************************************************
  10 ��������
*****************************************************************************/

extern VOS_VOID OM_COMM_Init(VOS_VOID);
extern VOS_VOID OM_COMM_InitFuncTable(VOS_VOID);
extern VOS_VOID OM_COMM_IsrNmi(VOS_VOID);
extern VOS_VOID OM_COMM_IsrSysException (VOS_UINT32 uwExceptionNo);
extern VOS_UINT32 OM_COMM_MsgExtDefReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 OM_COMM_PidInit(enum VOS_INIT_PHASE_DEFINE enInitPhrase);
extern VOS_VOID OM_COMM_PidProc(MsgBlock *pstOsaMsg);
extern VOS_VOID OM_COMM_SendTrace(VOS_UCHAR *pucTrace, VOS_UINT32 uwLength);
extern VOS_VOID OM_COMM_SendTrans(UCOM_DATA_BLK_STRU *pstDataBlk, VOS_UINT32 uwBlkCnt);
extern VOS_UINT32 OM_COMM_MsgOmConnectCmd(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 OM_COMM_MsgOmDisConnectCmd(VOS_VOID *pvOsaMsg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of om_comm.h */
