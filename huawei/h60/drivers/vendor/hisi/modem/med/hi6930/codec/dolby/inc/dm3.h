/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : xa_dm3.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2012��12��20��
  ����޸�   :
  ��������   : xa_dm3.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   :
    ��    ��   : twx144307
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __XA_DM3_H__
#define __XA_DM3_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "vos.h"
#include  "xa_apicmd_standards.h"
#include  "xa_error_handler.h"
#include  "xa_error_standards.h"
#include  "xa_memory_standards.h"
#include  "xa_type_def.h"
#include  "xa_dm3plus_ak_pp_api.h"

#include  "xa_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/
/* ˫���� */
#define AUDIO_PCM_CHANNEL_NUM           (2)
#define AUDIO_PCM_BYTES_PER_SAMPLE      (2)

//#define AUDIO_EFFECT_MAX_CHANNLE_NUM    (8)
#define AUDIO_EFFECT_MAX_SAMPLE_RATE    (48000)

/* Number of samples to process per exec call (must be multiple of 32). */
#define DM3_N_SAMPLES                   (512)

/* ͼ�ξ���(Graphic EQ)������漶�� */
#define AUDIO_EFFECT_GEQ_MAX_GAIN_SCALE (48)

/* ͼ�ξ���(Graphic EQ)������漶�� */
#define AUDIO_EFFECT_GEQ_MIN_GAIN_SCALE (-48)

/* λ����Ϣ */
#define AUDIO_EFFECT_PCM_BIT_WIDTH      (16)

#define AUDIO_EFFECT_GetDm3HeadphoneCfgPtr() (&g_stDm3HeadphoneParams)
#define AUDIO_EFFECT_GetDm3SpkCfgPtr()       (&g_stDm3SpkParams)
#define AUDIO_EFFECT_GetDm3MemCfgPtr()       (&g_stDm3MemoryCfg)
#define AUDIO_EFFECT_GetDtsMemCfgPtr()       (&g_stDtsMemoryCfg)
#define AUDIO_EFFECT_GetDm3ApiPtr()          (g_pstDm3ApiObj)

/* ������������ʱ��ֲ��� */
#define AUDIO_EFFECT_DM3_IO_SAMP_EQU    (1)

/* DM3��������������ʱ任���ܹر� */
#define AUDIO_EFFECT_DM3_UPSAMP_DISABLE (0)

/* �����Ѿ���ʼ�� */
#define AUDIO_EFFECT_PARA_INITIALIZED   (2)

/* ֻ�в�����Ҫ��ʼ�����ڴ治��Ҫ���·��� */
#define AUDIO_EFFECT_PARA_RESET_ONLY    (1)

/* ����û�г�ʼ������Ҫ���·����ڴ�ռ䲢��ʼ�� */
#define AUDIO_EFFECT_PARA_UNINITIALIZED (0)

/* ��ǰDM3��Ч����Ҫ��ʼ�� */
#define AUDIO_EFFECT_GetDm3InitStatus()       (g_uwDm3InitStatus)
#define AUDIO_EFFECT_SetDm3InitStatus(uwPara) (g_uwDm3InitStatus = (uwPara))

#define AUDIO_EFFECT_GetUserCfgPtr()          (&g_stAudioEffectUserCfg)

#define AUDIO_EFFECT_Dm3ApiValid        (0)
#define AUDIO_EFFECT_Dm3ApiInvalid      (1)


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/* ������Ч���� */
enum AUDIO_EFFECT_DM3_CHAN_ORDER_ENUM
{
    AUDIO_EFFECT_DM3_CHANS_ORDER_C   = 1,      /* ������ */
    AUDIO_EFFECT_DM3_CHANS_ORDER_L_R = 2,      /* ˫���� */
    AUDIO_EFFECT_DM3_CHANS_ORDER_BUT
};
typedef VOS_UINT16 AUDIO_EFFECT_DM3_CHAN_ORDER_ENUM_UINT16;


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
 ʵ������  : AUDIO_EFFECT_VALUE_STRU
 ��������  : �������ݽṹ����
*****************************************************************************/
typedef struct
{
    VOS_UINT32  uwIsSetted;
    VOS_INT32   swValue;
} AUDIO_EFFECT_VALUE_STRU;

/*****************************************************************************
 ʵ������  : AUDIO_EFFECT_DOBLY_PARAMS
 ��������  : DOBLY��Ч���ò���
*****************************************************************************/
typedef struct
{
    VOS_INT32 swInMatrix;
    VOS_INT32 swLfeMixLevel;

    VOS_INT32 swByPass; /* 0 disable, 1 soft mode, 2 hard mode*/

    /* ������2˫���� */
    VOS_INT32 swM2sEnable;
    VOS_INT32 swM2sDetecotr;/* tensilca Ĭ�Ͽ��� */

    /* ������ǿ */
    VOS_INT32 swSSEEnable;
    VOS_INT32 swSSEWidth;
    VOS_INT32 swSSESpkMode;

    /* �����������ģ�� */
    VOS_INT32 swSlcEnable;
    VOS_INT32 swSlcLevel;
    VOS_INT32 swSlcDepth;

    /* ����ģ�� */
    VOS_INT32 swNBEnable;
    VOS_INT32 swNBCutoff;
    VOS_INT32 swNBBoost;
    VOS_INT32 swNBLevel;

    /* ����ģ�� */
    VOS_INT32 swGeqEnable;
    VOS_INT32 swGeqNbands;
    VOS_INT32 swGeqPreamp;
    VOS_INT32 swGeqMaxBoost;
    VOS_INT32 aswGeqBands[GEQ_N_BANDS_MAX];

    /* �ƶ����� */
    VOS_INT32 swMsrMaxProfile;
    VOS_INT32 swMsrEnable;
    VOS_INT32 swMsrRoot;
    VOS_INT32 swMsrBright;

    /* ��Ƶ��ǿģ�� */
    VOS_INT32 swHfeEnable;
    VOS_INT32 swHfeDepth;

    /* Speaker Equalizer Module */
    VOS_INT32 swSpkEQEnable;
    VOS_INT32 swSpkEQChgainDB1;
    VOS_INT32 swSpkEQChgainDB2;

    /* XA_DM3PLUS_AK_PP_CONFIG_PARAM_TDAS_PORTABLEMODE */
    VOS_INT32 swPortableMode;
    VOS_INT32 swPortableModeGain;
    VOS_INT32 swPortableModeOrl;

} AUDIO_EFFECT_DOBLY_PARAMS_STRU;
/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern xa_codec_handle_t                g_pstDm3ApiObj;
extern AUDIO_EFFECT_DOBLY_PARAMS_STRU   g_stDm3HeadphoneParams; /* dolby headphone��Ч����ȫ������,���ܳ�ʼ�� */
extern AUDIO_EFFECT_DOBLY_PARAMS_STRU   g_stDm3SpkParams;
extern XA_COMM_MEMORY_CFG_STRU          g_stDm3MemoryCfg;       /* �ڴ���� */
extern XA_COMM_MEMORY_CFG_STRU          g_stDtsMemoryCfg;       /* �ڴ���� */
extern VOS_UINT32                       g_uwDm3InitStatus;
extern XA_AUDIO_EFFECT_USER_CFG_STRU    g_stAudioEffectUserCfg;

/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_INT32 AUDIO_EFFECT_CharArrayToInt16(VOS_CHAR cLb, VOS_CHAR cHb);
extern VOS_INT32 AUDIO_EFFECT_CharArrayToInt32(VOS_CHAR* cBa);
extern VOS_VOID AUDIO_EFFECT_Dm3Close(VOS_VOID);
extern VOS_UINT32 AUDIO_EFFECT_Dm3Execute(XA_AUDIO_EFFECT_USER_CFG_STRU* pstUserCfg);
extern VOS_UINT32  AUDIO_EFFECT_Dm3Init(XA_AUDIO_EFFECT_USER_CFG_STRU* pstUserCfg);
extern VOS_VOID AUDIO_EFFECT_Dm3ParseParams(VOS_INT32 swParamId, VOS_INT32 swValue, AUDIO_EFFECT_DOBLY_PARAMS_STRU* pstDoblyParams);
extern VOS_UINT32 AUDIO_EFFECT_Dm3SetPara(AUDIO_EFFECT_DOBLY_PARAMS_STRU* pstDoblyParams);
extern VOS_UINT32 AUDIO_EFFECT_Execute(XA_AUDIO_EFFECT_USER_CFG_STRU* pstUserCfg);
extern VOS_UINT32 AUDIO_EFFECT_MsgSetEnableReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_EFFECT_MsgSetParamReq(VOS_VOID *pvOsaMsg);
extern VOS_VOID AUDIO_EFFECT_Dm3SetParaAsDefault(AUDIO_EFFECT_DOBLY_PARAMS_STRU *pstPara);



#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of audio_pcm.h */




