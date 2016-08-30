
#ifndef __MODULE_BALONG_PP_H__
#define __MODULE_BALONG_PP_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "mlib_interface.h"




#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/

/* PP����NV�������� */
#define MED_PP_NV_ARRAY_TOTAL_LEN           (600)                               /*������ѧ����NV�����ܳ���*/
#define MED_CTRL_McPpNvArrayPtr()           (&g_ashwPpNv[0])                    /* ��ȡPP����NV����ȫ������ָ�� */

#define MODULE_BALONG_GetVoiceParaSet()     (&g_stModuleBalongVoiceCtrl.stPara)
#define MODULE_BALONG_GetAudioParaSet()     (&g_stModuleBalongAudioCtrl.stPara)

#define MODULE_BALONG_GetVoiceCtrl()        (&g_stModuleBalongVoiceCtrl)
#define MODULE_BALONG_GetAudioCtrl()        (&g_stModuleBalongAudioCtrl)

#define MED_CTRL_SAMPLE_RATE_NUM            (2)                                 /* ֧�ֵĲ����ʸ��� 8k��16k */
#define MED_CTRL_DEVICE_MODE_NUM            (7)                                 /* ֧�ֵ�ģʽ���� handset handfree ... */
#define MED_CTRL_MODE_SUB_ID_NUM            (3)                                 /* ͬһ��ģʽ�µ�ID���� */
#define MED_CTRL_MODE_SUB_ID_LEN            (200)                               /* ����ID�����ݳ��� ��λ:int16 */
#define MED_CTRL_MC_SET_NV_MSG_MASK         (7)                                 /* SETNV��Ϣ����NV���������� */


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/
/* �����豸�����ṹ�� */
typedef struct
{
    MLIB_UINT16                          usDeviceMode;                          /* �����豸��0 �ֳ֣�1 �ֳ����᣻2 �������᣻3 ������4 ������5 PCVOICE */
    MLIB_UINT16                          usNetMode;                             /* ����ģʽ��0 GSM�� 1 WCDMA��   3 TD */
    MLIB_UINT16                          usSampleRate;                          /* ������  : 0 8k;   1 16k */
    MLIB_UINT16                          usIndex;                               /* �����Ϣ�� ��ID���� */
    MLIB_INT16                           ashwNv[MED_CTRL_MODE_SUB_ID_LEN];      /* ����ID��NV�������� */
}OM_BALONG_VOICE_NV_DATA_STRU;

/* ��Ƶ�豸�����ṹ�� */
typedef struct
{
    MLIB_UINT16                          usDeviceMode;                          /* �����豸��0 �ֳ֣�1 �ֳ����᣻2 �������᣻3 ������4 ������5 PCVOICE */
    MLIB_UINT16                          usSampleRate;                          /* ������  : 0 8k;   1 16k */
    MLIB_UINT32                          usIndex;                               /* �����Ϣ�� ��ID���� */
    MLIB_INT16                           ashwNv[MED_CTRL_MODE_SUB_ID_LEN];      /* ����ID��NV�������� */
}OM_BALONG_AUDIO_NV_DATA_STRU;

/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/

/* ���ƽṹ�� */
typedef struct
{
    MLIB_INT32                          swRef;                                  /* �ṹ�����ü��� */
    MLIB_PARA_STRU                      stPara;                                 /* �������� */
} MODULE_BALONG_CTRL_STRU;

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern MODULE_BALONG_CTRL_STRU g_stModuleBalongVoiceCtrl;
extern MODULE_BALONG_CTRL_STRU g_stModuleBalongAudioCtrl;

/*****************************************************************************
  10 ��������
*****************************************************************************/

extern MLIB_ERROR_ENUM_UINT32  BA_AUDIO_PP_Clear( MLIB_VOID );
extern MLIB_ERROR_ENUM_UINT32  BA_AUDIO_PP_Get(
                       MLIB_VOID              *pvData,
                       MLIB_UINT32             uwSize);
extern MLIB_ERROR_ENUM_UINT32  BA_AUDIO_PP_Init( MLIB_VOID );
extern MLIB_ERROR_ENUM_UINT32  BA_AUDIO_PP_ProcMicIn(
                       MLIB_SOURCE_STRU       *pstSourceOut,
                       MLIB_SOURCE_STRU       *pstSourceIn);
extern MLIB_ERROR_ENUM_UINT32  BA_AUDIO_PP_ProcSpkOut(
                       MLIB_SOURCE_STRU       *pstSourceOut,
                       MLIB_SOURCE_STRU       *pstSourceIn);
extern MLIB_ERROR_ENUM_UINT32  BA_AUDIO_PP_Set(
                       MLIB_PARA_STRU         *pstPara,
                       MLIB_UINT32             uwSize);
extern MLIB_ERROR_ENUM_UINT32  BA_VOICE_PP_Clear( MLIB_VOID );
extern MLIB_ERROR_ENUM_UINT32  BA_VOICE_PP_Get(
                       MLIB_VOID              *pvData,
                       MLIB_UINT32             uwSize);
extern MLIB_ERROR_ENUM_UINT32  BA_VOICE_PP_Init( MLIB_VOID );
extern MLIB_ERROR_ENUM_UINT32  BA_VOICE_PP_ProcMicIn(
                       MLIB_SOURCE_STRU       *pstSourceOut,
                       MLIB_SOURCE_STRU       *pstSourceIn);
extern MLIB_ERROR_ENUM_UINT32  BA_VOICE_PP_ProcSpkOut(
                       MLIB_SOURCE_STRU       *pstSourceOut,
                       MLIB_SOURCE_STRU       *pstSourceIn);
extern MLIB_ERROR_ENUM_UINT32  BA_VOICE_PP_Set(
                       MLIB_PARA_STRU         *pstPara,
                       MLIB_UINT32             uwSize);


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of module_balong_pp.h */
