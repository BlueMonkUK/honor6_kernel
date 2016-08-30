/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : VOICE_amr_mode.h
  �� �� ��   : ����
  ��    ��   : л���� 58441
  ��������   : 2010��4��3��
  ����޸�   :
  ��������   : VOICE_amr_mode.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2010��4��3��
    ��    ��   : л���� 58441
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __VOICE_AMR_MODE_H__
#define __VOICE_AMR_MODE_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "codec_typedefine.h"
#include "WttfCodecInterface.h"
#include "CodecInterface.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/
/* ȫ�ֱ������װ */
#define VOICE_AMR_MODE_CTRL_GetObjPtr()        (&g_stVoiceAmrModeCtrlObj)
#define VOICE_AMR_MODE_CTRL_SetAcsSet(enAcs)   (g_stVoiceAmrModeCtrlObj.enAcsSet = enAcs)
#define VOICE_AMR_MODE_CTRL_GetAcsSet()        (g_stVoiceAmrModeCtrlObj.enAcsSet)

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
enum VOICE_AMR_DTX_ENUM
{
    VOICE_AMR_DTX_DISABLE                 = 0,                                    /* DTX OFF */
    VOICE_AMR_DTX_ENABLE                  = 1,                                    /* DTX ON  */
    VOICE_AMR_DTX_BUTT
};
typedef VOS_UINT16 VOICE_AMR_DTX_ENUM_UINT16;


enum VOICE_AMR_ACS_SET_ENUM
{
    VOICE_AMR_ACS_SET_NO                  = 0,                                    /* ACSδ���� */
    VOICE_AMR_ACS_SET_YES                 = 1,                                    /* ACS������ */
    VOICE_AMR_ACS_SET_BUTT
};
typedef VOS_UINT16 VOICE_AMR_ACS_SET_ENUM_UINT16;
/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/
typedef struct
{
    WTTFVOICE_AMR_TYPE_ENUM_UINT16                enAmrType;                    /* AMR����: UMTS-AMR|UMTS-AMR2|UMTS-AMRWB */
    VOS_UINT16                                    enCurrentMode;                /* ��ǰAMR����ģʽ */
    VOS_UINT16                                    enTargetMode;                 /* Ŀ��AMR����ģʽ */
    VOS_UINT16                                    uhwNumInAcs;                  /* ACS������ģʽ��Ŀ */
    VOS_UINT16                                    aenAcs[WTTFVOICE_AMR_CODEC_MODE_NUM];
                                                                                /* AMR ACS */
    VOICE_AMR_DTX_ENUM_UINT16                     enDtxEnable;                  /* DTX ��־ */
    VOS_UINT16                                    uhwFrmCnt;                    /* ����֡���� */
    VOICE_AMR_ACS_SET_ENUM_UINT16                 enAcsSet;                     /* ��־ACS�Ƿ������� */
    VOS_UINT16                                    uhwReserve;
} VOICE_AMR_MODE_CTRL_OBJ;

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern VOICE_AMR_MODE_CTRL_OBJ                   g_stVoiceAmrModeCtrlObj;                  /* AMRģʽ��������ʵ��, ��ϸ�μ�VOICE_AMR_MODE_CTRL_OBJ���� */


/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_UINT32 VOICE_MsgUmtsMacModeSetIndIRP(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgUmtsMacModeChangeIndR(VOS_VOID *pstOsaMsg );
extern VOICE_AMR_DTX_ENUM_UINT16 VOICE_AmrModeCtrlGetDtx(VOS_VOID);
extern VOS_UINT16 VOICE_AmrModeCtrlChangeMode(VOS_VOID);
extern VOS_VOID VOICE_AmrModeCtrlInit(VOS_VOID);
extern VOS_VOID VOICE_UmtsAmrModeCtrlSetType(CODEC_ENUM_UINT16 enCodecType,VOS_UINT32 uwReceiverPid);
extern VOS_VOID VOICE_TDAmrModeCtrlSetType(CODEC_ENUM_UINT16 enCodecType);
extern VOS_UINT32 VOICE_MsgTDmacModeSetIndIRP(VOS_VOID *pstOsaMsg);
extern VOS_UINT32 VOICE_MsgTDMacModeChangeIndR(VOS_VOID *pstOsaMsg );


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of med_vs_amr_mode_ctrl.h */
