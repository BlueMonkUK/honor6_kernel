/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : VOICE_debug.h
  �� �� ��   : ����
  ��    ��   : л���� 58441
  ��������   : 2011��7��4��
  ����޸�   :
  ��������   : VOICE_debug.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��7��4��
    ��    ��   : л���� 58441
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __VOICE_DEBUG_H__
#define __VOICE_DEBUG_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "codec_typedefine.h"
#include "OmCodecInterface.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/
/* 2.2 ȫ�ֱ�����װ */
#define VOICE_DbgGetLoopFLagPtr()       (&g_stVoiceDbgLoop)
#define VOICE_DbgGetHookPtr()           (&g_stVoiceDbgHookObj)

/* 2.3 ���ݹ�ȡλ�����Ӧ����*/
#define VOICE_HOOK_NONE                 (0x0000)
#define VOICE_HOOK_TX_STRM              (0x0001)                                  /*bit0:��������*/
#define VOICE_HOOK_RX_STRM              (0x0002)                                  /*bit1:��������*/
#define VOICE_HOOK_MICIN_PCM            (0x0004)                                  /*bit2:��Mic�����PCM*/
#define VOICE_HOOK_SPKOUT_PCM           (0x0008)                                  /*bit3:�����Spk��PCM*/
#define VOICE_HOOK_CODECIN_PCM          (0x0010)                                  /*bit4:�����������PCM*/
#define VOICE_HOOK_CODECOUT_PCM         (0x0020)                                  /*bit5:�����������PCM*/
#define VOICE_HOOK_AECIN_PCM            (0x0040)                                  /*bit6:AEC�����PCM*/
#define VOICE_HOOK_AECREF_PCM           (0x0080)                                  /*bit7:AEC�ο�PCM*/
#define VOICE_HOOK_JB_PARA              (0x0100)                                  /*bit8:JitterBuffer�ϱ�����*/
#define VOICE_HOOK_TSM_DATA             (0x0400)                                  /*bit10:AJB TSM�������*/
#define VOICE_HOOK_BUTT                 (0x0800)                                  /*��������*/

/* 2.4 ���ݹ�ȡ������ */
#define VOICE_DBG_CODEC_TITLE_MAGIC_NUM                  (0x55AA55AA)
#define VOICE_DBG_CODEC_ENCODE_MAGIC_NUM                 (0x5A5A5A5A)
#define VOICE_DBG_CODEC_DECODE_MAGIC_NUM                 (0xA5A5A5A5)


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/* �����������ö�� */
enum VOICE_DBG_CODEC_MODE_ENUM
{
    VOICE_DBG_CODEC_MODE_ENCODE           = 0,
    VOICE_DBG_CODEC_MODE_DECODE,
    VOICE_DBG_CODEC_MODE_BUTT
};
typedef VOS_UINT16 VOICE_DBG_CODEC_MODE_ENUM_UINT16;

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
/* ���ݹ�ȡ�ṹ��*/
typedef struct
{
    CODEC_SWITCH_ENUM_UINT16            enHookEnable;                           /* ��ȡ�����Ƿ�� */
    VOS_UINT16                          usHookTarget;                           /* ��ȡ����Ŀ�� */
    VOS_UINT32                          ulFrameTick;                            /*��ǰ����֡֡��*/
} VOICE_HOOK_STRU;

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern CODEC_LOOP_STRU g_stVoiceDbgLoop;
extern VOICE_HOOK_STRU g_stVoiceDbgHookObj;

/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_VOID VOICE_DbgCheckAndLoop(CODEC_LOOP_ENUM_UINT16 uhwPos);
extern VOS_VOID VOICE_DbgInfoHookCfg(VOS_VOID);
extern VOS_VOID VOICE_DbgInit( CODEC_NV_PARA_VOICE_TRACE_CFG_STRU *pstVoiceTraceCfg );
extern VOS_VOID VOICE_DbgSendHook(
                       VOS_UINT16              uhwPos,
                       VOS_VOID               *pvData,
                       VOS_UINT16              uhwLen);
extern VOS_UINT32 VOICE_MsgOmSetHookReqIRP(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgOmSetLoopReqIRP(VOS_VOID *pstOsaMsg);
extern VOS_VOID VOICE_DbgSaveCodecObj(
                       VOS_INT16                           *pshwCodedBuf,
                       VOICE_DBG_CODEC_MODE_ENUM_UINT16  uhwCodecMode,
                       VOS_VOID                            *pstCodec);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif /* end of VOICE_debug.h */
