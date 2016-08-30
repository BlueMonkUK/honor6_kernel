/******************************************************************************

                  ��Ȩ���� (C), 2012-2013, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : audio_debug.h
  �� �� ��   : ����
  ��    ��   : ������ W164657
  ��������   : 2012��7��26��
  ����޸�   :
  ��������   : AUDIO_debug.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��7��26��
    ��    ��   : ������ W164657
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __AUDIO_DEBUG_H__
#define __AUDIO_DEBUG_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "codec_typedefine.h"
#include "OmCodecInterface.h"
#include "audio_pcm.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/
/* 2.2 ȫ�ֱ�����װ */
#define AUDIO_UtilGetHookPtr()       (&g_stAudioDbgHookObj)

/* 2.3 ���ݹ�ȡλ�����Ӧ���� */
enum AUDIO_HOOK_POS_ENUM
{
    AUDIO_HOOK_NONE                         = 0x0000,
    AUDIO_HOOK_APIN_PCM                     = 0x0001,   /* 0 bit */
    AUDIO_HOOK_CODECOUT_PCM                 = 0x0002,   /* 1 bit */
    AUDIO_HOOK_MICIN_PCM                    = 0x0004,   /* 2 bit */
    AUDIO_HOOK_ENHANCE_MICIN                = 0x0008,   /* 3 bit */
    AUDIO_HOOK_ENHANCE_SPKOUT               = 0x0010,   /* 4 bit,16 */
    AUDIO_HOOK_ENHANCE_MLIB_MICIN_16K_L     = 0x0020,   /* 5 bit,32 */
    AUDIO_HOOK_ENHANCE_MLIB_MICIN_16K_R     = 0x0040,   /* 6 bit,64 */
    AUDIO_HOOK_ENHANCE_MLIB_MICIN_48K_L     = 0x0080,   /* 7 bit,128 */
    AUDIO_HOOK_ENHANCE_MLIB_MICIN_48K_R     = 0x0100,   /* 8 bit,256 */
    AUDIO_HOOK_ENHANCE_MLIB_SPKOUT_16K_L    = 0x0200,   /* 9 bit,512 */
    AUDIO_HOOK_ENHANCE_MLIB_SPKOUT_16K_R    = 0x0400,   /* 10 bit,1024 */
    AUDIO_HOOK_ENHANCE_MLIB_SPKOUT_48K_L    = 0x0800,   /* 11 bit,2048 */
    AUDIO_HOOK_ENHANCE_MLIB_SPKOUT_48K_R    = 0x1000,   /* 12 bit,4096 */
    AUDIO_HOOK_ENHANCE_MLIB_SMARTPA_16K_L   = 0x2000,   /* 13 bit,8192 */
    AUDIO_HOOK_ENHANCE_MLIB_SMARTPA_16K_R   = 0x4000,   /* 14 bit,16384 */
    AUDIO_HOOK_BUTT
};
typedef VOS_UINT16 AUDIO_HOOK_POS_ENUM_UINT16;

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/* �����������ö�� */
enum AUDIO_DBG_CODEC_MODE_ENUM
{
    AUDIO_DBG_CODEC_MODE_ENCODE           = 0,
    AUDIO_DBG_CODEC_MODE_DECODE,
    AUDIO_DBG_CODEC_MODE_BUTT
};
typedef VOS_UINT16 AUDIO_DBG_CODEC_MODE_ENUM_UINT16;

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
/* ���ݹ�ȡ�ṹ�� */
typedef struct
{
    CODEC_SWITCH_ENUM_UINT16            enHookEnable;                           /* ��ȡ�����Ƿ�� */
    VOS_UINT16                          usHookTarget;                           /* ��ȡ����Ŀ�� */
}AUDIO_HOOK_STRU;

/*****************************************************************************
 ʵ������  : AUDIO_LOOP_CNF_STRU
 ��������  : ����Ϣ֪ͨAP��Ƶ���ش򿪻�ر��Ƿ�ɹ�
*****************************************************************************/
 typedef struct
{
    unsigned short                          uhwMsgId;           /*_H2ASN_Skip */
    AUDIO_PLAY_RESULT_CNF_ENUM_UINT16       enAudioLoopCnf;
} AUDIO_LOOP_CNF_STRU;

typedef struct
{
    unsigned short   usMsgId;           /*_H2ASN_Skip */
    unsigned short   usReserve;           /*_H2ASN_Skip */
    unsigned long    ulSenderPid;       /* SenderPid */
    unsigned long    ulReceiverPid;     /* ReceiverPid */
    unsigned long    ulChannel;         /* ͨ���� */
    unsigned long    ulMsgLen;          /* ��Ϣ���� */
}TEST_HIFI_MSG_STRU;

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/

extern AUDIO_HOOK_STRU g_stAudioDbgHookObj;

/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_VOID AUDIO_DEBUG_Init( CODEC_NV_PARA_AUDIO_TRACE_CFG_STRU *pstAudioTraceCfg );
extern VOS_UINT32 AUDIO_DEBUG_MsgAudioLoopCloseReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_DEBUG_MsgAudioLoopOpenReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_DEBUG_MsgQueryStatusReq(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 AUDIO_DEBUG_MsgSetHookReq(VOS_VOID *pstOsaMsg);
extern VOS_VOID AUDIO_DEBUG_SendHookInd(
                       VOS_UINT16              uhwPos,
                       VOS_VOID               *pvData,
                       VOS_UINT32              uwLen);
extern VOS_UINT32 AUDIO_DEBUG_MsgTestCmd(VOS_VOID *pvOsaMsg);
#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif /* end of audio_debug.h */

