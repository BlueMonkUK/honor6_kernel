

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "audio.h"
#include  "audio_pcm.h"

#ifndef __AUDIO_RECORDER_H__
#define __AUDIO_RECORDER_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/

/* ��ȡ¼�����ƽṹ�� */
#define AUDIO_RecorderGetObjPtr()                   (&g_stAudioRecorderObj)

/* ��ȡ��ǰ¼������ */
#define AUDIO_RecorderGetCaptureType()              (g_stAudioRecorderObj.enRecordType)
/* ���õ�ǰ¼������ */
#define AUDIO_RecorderSetCaptureType(enTpye)        (g_stAudioRecorderObj.enRecordType = enTpye)

#define AUDIO_RECORDER_BUFF_SIZE_320                (320)
#define AUDIO_RECORDER_BUFF_SIZE_640                (640)
#define AUDIO_RECORDER_BUFF_SIZE_1920               (1920)
#define AUDIO_RECORDER_BUFF_SIZE_3840               (3840)
#define ID_VOICE_AUDIO_RECORDER_UPDATE_BUFF_IND     (0xDDE6)

/****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
 ʵ������  : AUDIO_RECORDER_SAMPLERATE_ENUM
 ��������  : ¼��������ö��
*****************************************************************************/
enum AUDIO_RECORDER_SAMPLERATE_ENUM
{
    AUDIO_RECORDER_SAMPLERATE_8K              = 8000,
    AUDIO_RECORDER_SAMPLERATE_16K             = 16000,
    AUDIO_RECORDER_SAMPLERATE_48K             = 48000,
    AUDIO_RECORDER_SAMPLERATE_BUTT
};
typedef VOS_UINT32 AUDIO_RECORDER_SAMPLERATE_ENUM_UINT32;

/*****************************************************************************
 ʵ������  : AUDIO_RECORDER_VOICE_TXRX_ENUM
 ��������  : ͨ��¼������������ö��
*****************************************************************************/
enum AUDIO_RECORDER_VOICE_TXRX_ENUM
{
    AUDIO_RECORDER_VOICE_TX             = 0,
    AUDIO_RECORDER_VOICE_RX             = 1,
    AUDIO_RECORDER_VOICE_TXRX_BUT
};
typedef VOS_UINT32 AUDIO_RECORDER_VOICE_TXRX_ENUM_UINT32;


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
 ʵ������  : AUDIO_RECORDER_VOICE_BUFF_STRU
 ��������  : Audio recorder ͨ��¼������ṹ��
*****************************************************************************/
 typedef struct
{
    VOS_UINT32                                  uwProtectWord1;
    VOS_INT16                                   ashwTxDataIn[AUDIO_RECORDER_BUFF_SIZE_1920];
    VOS_UINT32                                  uwProtectWord2;
    VOS_INT16                                   ashwTxDataOut[AUDIO_RECORDER_BUFF_SIZE_1920];

    VOS_UINT32                                  uwProtectWord3;
    VOS_INT16                                   ashwRxDataIn[AUDIO_RECORDER_BUFF_SIZE_1920];
    VOS_UINT32                                  uwProtectWord4;
    VOS_INT16                                   ashwRxDataOut[AUDIO_RECORDER_BUFF_SIZE_1920];
    VOS_UINT32                                  uwProtectWord5;

}AUDIO_RECORDER_VOICE_BUFF_STRU;

/*****************************************************************************
 ʵ������  : AUDIO_RECORDER_VOICE_STRU
 ��������  : Audio recorder ͨ��¼���ṹ��
*****************************************************************************/
 typedef struct
{
    VOS_UINT16                                  uwRefCnt;                       /* ͨ��¼�����ü��� */
    VOS_UINT16                                  uhwReserved;
    AUDIO_RECORDER_SAMPLERATE_ENUM_UINT32       uwOrignalSmpRate;               /* ԭʼ�������ݲ����� */
    AUDIO_RECORDER_SAMPLERATE_ENUM_UINT32       uwTargetSmpRate;                /* Ŀ���������ݲ����� */
    AUDIO_RECORDER_VOICE_BUFF_STRU              stRecordBuff;                   /* ¼������ */

}AUDIO_RECORDER_VOICE_STRU;

/*****************************************************************************
 ʵ������  : AUDIO_RECORDER_3RD_VOICE_STRU
 ��������  : Audio recorder ������modemͨ��¼���ṹ�壬������
*****************************************************************************/
 typedef struct
{
    VOS_UINT16                                  uwRefCnt;                       /* ͨ��¼�����ü��� */
    VOS_UINT16                                  uhwReserved;
    AUDIO_RECORDER_SAMPLERATE_ENUM_UINT32       uwOrignalSmpRate;               /* ԭʼ�������ݲ����� */
    AUDIO_RECORDER_SAMPLERATE_ENUM_UINT32       uwTargetSmpRate;                /* Ŀ���������ݲ����� */

}AUDIO_RECORDER_3RD_VOICE_STRU;

/*****************************************************************************
 ʵ������  : AUDIO_RECORDER_AUDIO_STRU
 ��������  : Audio recorder ��Ƶ����¼���ṹ�壬������
*****************************************************************************/
 typedef struct
{
    VOS_UINT16                                  uwRefCnt;                       /* ��Ƶ����¼�����ü��� */
    VOS_UINT16                                  uhwReserved;
    AUDIO_RECORDER_SAMPLERATE_ENUM_UINT32       uwOrignalSmpRate;               /* ԭʼ��Ƶ���ݲ����� */
    AUDIO_RECORDER_SAMPLERATE_ENUM_UINT32       uwTargetSmpRate;                /* Ŀ����Ƶ���ݲ����� */

}AUDIO_RECORDER_AUDIO_STRU;

/*****************************************************************************
 ʵ������  : AUDIO_RECORDER_FM_STRU
 ��������  : Audio recorder FM����¼���ṹ�壬������
*****************************************************************************/
 typedef struct
{
    VOS_UINT16                                  uwRefCnt;                       /* FM����¼�����ü��� */
    VOS_UINT16                                  uhwReserved;
    AUDIO_RECORDER_SAMPLERATE_ENUM_UINT32       uwOrignalSmpRate;               /* ԭʼ��Ƶ���ݲ����� */
    AUDIO_RECORDER_SAMPLERATE_ENUM_UINT32       uwTargetSmpRate;                /* Ŀ����Ƶ���ݲ����� */

}AUDIO_RECORDER_FM_STRU;

/*****************************************************************************
 ʵ������  : AUDIO_RECORDER_OBJ_STRU
 ��������  : Audio recorder ¼�����ƽṹ��
*****************************************************************************/
 typedef struct
{
    VOS_UINT32                                  uwProtectWord1;
    VOS_UINT32                                  uwProtectWord2;

    AUDIO_RECORD_TYPE_ENUM_UINT16               enRecordType;                   /* ��ǰ¼������ */
    VOS_UINT16                                  uhwReserved;

    AUDIO_RECORDER_VOICE_STRU                   stVoiceRecord;                  /* CSͨ��¼�� */
    AUDIO_RECORDER_3RD_VOICE_STRU               st3RDVoiceRecord;               /* ������modemͨ��¼�� */
    AUDIO_RECORDER_AUDIO_STRU                   stAudioRecord;                  /* ��Ƶ����¼�� */
    AUDIO_RECORDER_FM_STRU                      stFMRecord;                     /* FM����¼�� */

    VOS_UINT32                                  uwProtectWord3;
    VOS_UINT32                                  uwProtectWord4;
}AUDIO_RECORDER_OBJ_STRU;

/*****************************************************************************
 ʵ������  : AUDIO_RECORDER_UPDATE_BUFF_STRU
 ��������  : Audio recorder ����¼��������Ϣ�ṹ��
*****************************************************************************/
 typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT16                                  uhwMsgId;
    VOS_UINT16                                  uhwReserved;
    VOS_INT16                                  *pshwBuff;
    VOS_UINT32                                  uwBuffSize;
    AUDIO_RECORDER_SAMPLERATE_ENUM_UINT32       uwVoiceSmpRate;
    AUDIO_RECORDER_VOICE_TXRX_ENUM_UINT32       enTxRxChn;
}AUDIO_RECORDER_UPDATE_BUFF_STRU;

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern AUDIO_RECORDER_OBJ_STRU                 g_stAudioRecorderObj;

/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_UINT32  AUDIO_RECOEDER_Init(VOS_VOID);
extern VOS_VOID AUDIO_RECORDER_AudioCallBack(
                       VOS_VOID  *pshwBufOut,
                       VOS_VOID  *pshwBufIn,
                       VOS_UINT32 uwSampleRate,
                       VOS_UINT32 uwChannelNum,
                       VOS_UINT32 uwBufSize);
extern VOS_VOID AUDIO_RECORDER_FMCallBack(
                       VOS_VOID  *pshwBufOut,
                       VOS_VOID  *pshwBufIn,
                       VOS_UINT32 uwSampleRate,
                       VOS_UINT32 uwChannelNum,
                       VOS_UINT32 uwBufSize);
extern VOS_UINT32 AUDIO_RECORDER_MsgRecordStartCmd(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_RECORDER_MsgRecordStopCmd(VOS_VOID *pvOsaMsg);
extern VOS_VOID AUDIO_RECORDER_SecondVoiceCallBack(
                       VOS_VOID  *pshwBufOut,
                       VOS_VOID  *pshwBufIn,
                       VOS_UINT32 uwSampleRate,
                       VOS_UINT32 uwChannelNum,
                       VOS_UINT32 uwBufSize);
extern VOS_VOID AUDIO_RECORDER_VoiceAudioPcmCB(
                       VOS_VOID  *pshwBufOut,
                       VOS_VOID  *pshwBufIn,
                       VOS_UINT32 uwTarSmpRate,
                       VOS_UINT32 uwChannelNum,
                       VOS_UINT32 uwTarBufSize);
extern VOS_UINT32  AUDIO_RECORDER_VoiceRcdStart(AUDIO_RECORD_START_STRU *pstRecStartMsg);
extern VOS_UINT32  AUDIO_RECORDER_VoiceRcdStop(VOS_VOID);
extern VOS_UINT32  AUDIO_RECORDER_VoiceVcMcCB(
                       VOS_INT16 *pshwTx,
                       VOS_INT16 *pshwRx,
                       VOS_UINT32 uwOrignalSmpRate);






#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of audio_recorder.h */
