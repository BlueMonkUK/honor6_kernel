/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : voice_jb_interface.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2014��5��5��
  ����޸�   :
  ��������   : JB�Ľӿ�ͷ�ļ�,����SJB��AJB�Ľӿڷ�װ
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2014��5��5��
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef _VOICE_JB_INTERFACE_H_
#define _VOICE_JB_INTERFACE_H_


/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "hme_jb_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define VOICE_JB_GetJbObjsPtr()         (&g_stVoiceJbObjs)
#define VOICE_JB_GetAjbModual()         (g_stVoiceJbObjs.pvAjbModule)                   /* ��ȡAJBģ���ָ�� */
#define VOICE_JB_GetAjbBuff()           (g_stVoiceJbObjs.pvAjbBuff)                     /* ��ȡAJB��Buff��ַ */
#define VOICE_JB_SetAjbBuff(pAddr)      (g_stVoiceJbObjs.pvAjbBuff = pAddr)             /* ��¼AJB��Buff��ַ */
#define VOICE_JB_GetImsaSsrc()          (g_stVoiceJbObjs.uwSsrc)                        /* ȡ��ǰIMSAͨ���е�ssrcֵ */
#define VOICE_JB_SetImsaSsrc(uwVar)     (g_stVoiceJbObjs.uwSsrc = uwVar)                /* ����ssrcֵ */
#define VOICE_JB_IsAjbEnable()          (g_stVoiceJbObjs.uhwAjbEnabled)                 /* ��ȡAJB�Ƿ�ʹ�� */
#define VOICE_JB_SetAjbEnable(uhwVar)   (g_stVoiceJbObjs.uhwAjbEnabled = uhwVar)        /* ����AJB�Ƿ�ʹ�� */
#define VOICE_JB_GetLtePktTime()        (g_stVoiceJbObjs.uwLtePktTime)                  /* ȡ��ǰ�Ĵ��ʱ��*/
#define VOICE_JB_SetLtePktTime(uwVar)   (g_stVoiceJbObjs.uwLtePktTime = uwVar)          /* ���ô��ʱ�� */
#define VOICE_JB_ImsTxPktExist()        (g_stVoiceJbObjs.uhwTxPktExist)                 /* ��ȡIMS��������а��Ƿ���� */
#define VOICE_JB_SetImsTxPktExist(uwVar)(g_stVoiceJbObjs.uhwTxPktExist = uwVar)         /* ����IMS��������а��Ƿ���� */
#define VOICE_JB_GetImsTxPktAddr()      (g_stVoiceJbObjs.pvTxPkt)                       /* ��ȡIMS��������а���ַ */
#define VOICE_JB_SetImsTxPktAddr(pstPkt)(g_stVoiceJbObjs.pvTxPkt = pstPkt)              /* ����IMS��������а���ַ */

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
/* ��Ӳ��log�ȼ� */
enum VOICE_JB_HME_LOG_LEVEL_ENUM
{
    VOICE_JB_HME_LOG_LEVEL_ERR        = 1,                 /* ERROR */
    VOICE_JB_HME_LOG_LEVEL_WARNING    = 4,                 /* WARNING */
    VOICE_JB_HME_LOG_LEVEL_BUTT
};

/* ������������� */
enum VOICE_FRAME_TYPE_ENUM
{
    VOICE_FRAME_TYPE_OTHER            = 0,                 /* ���� */
    VOICE_FRAME_TYPE_NO_DATA,                              /* NoData֡ */
    VOICE_FRAME_TYPE_SID,                                  /* SID֡ */
    VOICE_FRAME_TYPE_VOICE_GOOD,                           /* ����֡ */
    VOICE_FRAME_TYPE_BUTT
};
typedef VOS_UINT16 VOICE_FRAME_TYPE_ENUM_UINT16;

/* LTE��������ģʽ����÷�һ�����ݸ�IMSA */
enum VOICE_JB_PKT_TIME_ENUM
{
    VOICE_JB_PKT_TIME_20MS            = 20,                /* 20ms */
    VOICE_JB_PKT_TIME_40MS            = 40,                /* 40ms */
    VOICE_JB_PKT_TIME_BUTT
};
typedef VOS_UINT32 VOICE_JB_PKT_TIME_ENUM_UINT32;

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
 ʵ������  : VOICE_MC_IMS_CTRL_STRU
 ��������  : ����IMS������һЩ����
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          uwSsrc;                    /* VoLTEͨ��ʱ��SSRC���仯ʱ��ҪInitJB */
    VOS_UINT32                          uwLtePktTime;              /* VoLTEʱ�Ĵ��ʱ�������೤ʱ�䷢һ�� */
    VOS_UINT16                          uhwAjbEnabled;             /* AJB�Ƿ�ʹ��,0/1:��/�� */
    VOS_UINT16                          uhwTxPktExist;             /* ���ṹ���е����а��Ƿ����,0/1:��/�� */
    VOS_VOID                           *pvTxPkt;                   /* ����IMS֡����֡��ָ�룬�����ʱ��Ϊ40msʱ��
                                                                      ������ָ֡����ڴˣ�����һ��ż��֡һ�� */
    VOS_VOID                           *pvAjbBuff;                 /* AJB������ڴ��ַ */
    VOS_VOID                           *pvAjbModule;               /* AJB�Ŀ��ƿ�ָ�� */
} VOICE_JB_OBJS_STRU;

/*****************************************************************************
 ʵ������  : VOICE_MC_IMS_CTRL_STRU
 ��������  : ����IMS������һЩ����
*****************************************************************************/
typedef struct
{
    HME_JB_HIFI_STATICS                 stData;                    /* SJB��ȡ��������AJB����һ�� */
} VOICE_JB_HOOK_STRU;

/*****************************************************************************
  7 UNION����
*****************************************************************************/

/*****************************************************************************
  8 OTHERS����
*****************************************************************************/

/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern VOICE_JB_OBJS_STRU              g_stVoiceJbObjs;

/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_VOID VOICE_JB_Init(VOS_VOID);
extern VOS_UINT32 VOICE_JB_Start(VOS_UINT16  uhwCodecType);
extern VOS_VOID VOICE_JB_Free(VOS_VOID);
extern VOS_UINT32 VOICE_JB_AddPkt( VOS_VOID *pstPkt );
extern VOS_UINT32 VOICE_JB_GetSjbPkt(VOS_VOID);
extern VOS_UINT32 VOICE_JB_GetAjbPcmData(VOS_UINT16 usLength, VOS_VOID *pPcmOut, VOS_UINT32 *puiOutLen);
extern VOS_VOID VOICE_JB_SendImsaPkt(VOS_VOID *pvEncRslt);
extern VOS_UINT32 VOICE_JB_SetSjbPktTimeTx(VOS_UINT32 uwPktTime);

extern VOS_INT32 VOICE_JB_AjbGetTsInMs(unsigned int *uwTimeStamp);
extern VOS_INT32 VOICE_JB_AjbWriteLog(VOS_VOID *pLogMessage);
extern VOS_INT32 VOICE_JB_AjbWriteHookTrace(VOS_VOID *pTraceData);
extern VOS_INT32 VOICE_JB_AjbDecode(void *pstJBmodule, VOS_VOID *pstDecPrm);

#ifdef __cplusplus
}
#endif

#endif /* end of _VOICE_JB_INTERFACE_H_ */

