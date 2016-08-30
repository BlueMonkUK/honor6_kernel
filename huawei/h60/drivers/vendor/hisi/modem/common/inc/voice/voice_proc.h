/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : med_vs_proc.h
  �� �� ��   : ����
  ��    ��   : л���� 58441
  ��������   : 2008��5��28��
  ����޸�   :
  ��������   : med_vs_proc.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2010��4��8��
    ��    ��   : л���� 58441
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __VOICE_PROC_H__
#define __VOICE_PROC_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "codec_com_codec.h"
#include "voice_pcm.h"
#include "PhyCodecInterface.h"
#include "TdphyCodecInterface.h"
#include "VcCodecInterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
������˵��:

COMP    compensate filter   �����˲���
TXRX    Tx and Rx item      ������ѡ��
PROC    Pre & post process  ����ǰ����
HPF     High pass filter    ��ͨ�˲���
SND     sound generator     sound���кϳ���

*****************************************************************************/

/*****************************************************************************
  2 �궨��
*****************************************************************************/
/* �ص��������� */
typedef void (*VOICE_PROC_SONDIND_CALLBACK)(void);

/* ��鲢���� */
#define VOICE_ProcCheckAndRet(usRet);\
    if ((usRet) != UCOM_RET_SUCC)   \
    {                               \
        return (usRet);             \
    }

#define VOICE_PROC_HOMING_MASK      (0x0008)
#define VOICE_RX_LOST_FRAME_THD     (3)                                     /*����������ж�֡���֡�������ֵ*/
#define VOICE_PROC_GSM_RX_FACCH_THD (2)                                     /*GSM������FA֡������������ֵ*/
#define VOICE_PROC_GAIN_MAX         (12)                                    /* ���������� */
#define VOICE_PROC_GAIN_MIN         (-80)
#define VOICE_PROC_BFI_MAX          (2)                                     /* ��ദ������2֡BFI */

/* ��ȡ��������������� */
#define VOICE_ProcGetObjsPtr()           (&g_stVoiceProcObjs)

#define VOICE_ProcGetCodecPtr()      (g_stVoiceProcObjs.pstCodec)
#define VOICE_ProcGetEncBufPtr()     (&g_stVoiceCodecData.asEncSerial[0])
#define VOICE_ProcGetDecBufPtr()     (&g_stVoiceCodecData.astDecSerial[0])
#define VOICE_ProcGetLineInBufPtr()  (&g_stVoiceCodecData.asLineInBuff[0])
#define VOICE_ProcGetLineOutBufPtr() (&g_stVoiceCodecData.asLineOutBuff[0])
#define VOICE_ProcGetPreDecInObj()   (&g_stVoiceProcObjs.pstCodec->stPreDecInObj)
#define VOICE_ProcGetPrevDecDataPtr() (&g_stVoiceCodecData.asDecPrevData[0])

#define VOICE_GetRxLostFrameCnt()    (g_stVoiceProcObjs.pstCodec->uhwRxLostFrmCnt)              /* ��ȡ��֡����ֵ */
#define VOICE_SetRxLostFrameCnt(uhwVar) \
                                     (g_stVoiceProcObjs.pstCodec->uhwRxLostFrmCnt = (uhwVar))   /* ���ö�֡����ֵ */
#define VOICE_AddRxLostFrameCnt(uhwVar) \
                                     (g_stVoiceProcObjs.pstCodec->uhwRxLostFrmCnt += (uhwVar))  /* ��֡������ֵ�ۼӼ��� */
#define VOICE_ProcFacchFrameCntAdd() (g_stVoiceProcObjs.pstCodec->stDecInObj.uhwFrameFacchCnt++)   /* GSM����FA֡������1 */
#define VOICE_ProcFacchFrameCntReset()  \
                                     (g_stVoiceProcObjs.pstCodec->stDecInObj.uhwFrameFacchCnt = 0) /* GSM����FA֡������0 */
#define VOICE_ProcGetFacchFrameCnt() (g_stVoiceProcObjs.pstCodec->stDecInObj.uhwFrameFacchCnt)     /* ��ȡGSM��������FA֡���� */
#define VOICE_ProcSetMaxFrameRange(uhwRange)  \
                                     (g_stVoiceProcObjs.pstCodec->stDecInObj.shwFrameRangeMax = uhwRange) /* �洢���Ⱦ���ֵ���ֵ */
#define VOICE_ProcGetMaxFrameRange() (g_stVoiceProcObjs.pstCodec->stDecInObj.shwFrameRangeMax)     /* ��ȡ��һ֡�ķ��Ⱦ���ֵ���ֵ */



/* ����洢�������ݵ�дָ�� */
#define VOICE_ProcGetDecSerialWritePtr()  (g_pstDecSerialWrite)                    /* ��ȡ�洢�������ݵĵ�ַ*/
#define VOICE_ProcInitDecSerialWritePtr() (g_pstDecSerialWrite = &g_stVoiceCodecData.astDecSerial[0])  /* ��ʼ����������дָ��*/
#define VOICE_ProcAddDecSerialWritePtr()  (g_pstDecSerialWrite += 1)            /* ָ����һ���������ݴ洢��Ԫ */
#define VOICE_ProcSubDecSerialWritePtr()  (g_pstDecSerialWrite -= 1)            /* ָ����һ���������ݴ洢��Ԫ*/

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/* �Ƿ� */
enum VOICE_PROC_IS_ENUM
{
    VOICE_PROC_IS_NO                 = 0,
    VOICE_PROC_IS_YES                = 1,
    VOICE_PROC_IS_BUTT
};
typedef VOS_UINT16 VOICE_PROC_IS_ENUM_UINT16;

/*****************************************************************************
  4 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  5 ��Ϣͷ����
*****************************************************************************/

/*****************************************************************************
  6 ��Ϣ����
*****************************************************************************/

/*****************************************************************************
  7 STRUCT����
*****************************************************************************/

/* 7.1 �������ݽṹ */
/* 7.1.1 SOUND�ϳ������ò��� */
typedef struct
{
    VOS_INT16                           sTxVolume;                              /*����Snd��������:-80~12dB*/
    VOS_INT16                           sRxVolume;                              /*����Snd��������:-80~12dB*/
    VOS_UINT16                          enTxRx;                                 /*SOUND����Ŀ��ͨ��, VOICE_PROC_TXRX_ENUM*/
    VOS_UINT16                          usSndId;                                /*Snd ID*/
    VOS_UINT16                          usRptCnt;                               /*�ظ����Ŵ���:0 ��ʾ����ѭ��*/
    VOS_UINT16                          usReserve;
    VOICE_PROC_SONDIND_CALLBACK         funcSndInd;                             /*SOUND���Ž����ϱ��ص�����*/
} VOICE_PROC_SND_CFG_STRU;


/* 7.2.10 Codec���� */
typedef struct
{
    VOS_UINT16                          enCodecType;                            /*���������*/
    VOS_UINT16                          usIsEncInited;                          /*�������Ƿ��ѳ�ʼ��*/
    VOS_UINT16                          usIsDecInited;                          /*�������Ƿ��ѳ�ʼ��*/
    VOS_UINT16                          uhwRxLostFrmCnt;                        /*��������֡����*/
    CODEC_ENC_IN_PARA_STRU              stEncInObj;                             /*��������ṹ*/
    CODEC_ENC_OUT_PARA_STRU             stEncOutObj;                            /*��������ṹ*/
    CODEC_DEC_IN_PARA_STRU              stDecInObj;                             /*��������ṹ(���������ΪPCM����,��˲��ڴ�����)*/
    CODEC_DEC_PREIN_PARA_STRU           stPreDecInObj;                          /*ǰһ֡��������ṹ(���������ΪPCM����,��˲��ڴ�����)*/
} VOICE_PROC_CODEC_STRU;

/* 7.2.11 SOUND�ϳ��� */
typedef struct
{
    VOS_UINT16                          enTxEnable;                             /*snd����ʹ��*/
    VOS_UINT16                          enRxEnable;                             /*snd����ʹ��*/
    VOS_INT16                           sSndTxVol;                              /*Snd��������:-80~12*/
    VOS_INT16                           sSndRxVol;                              /*Snd��������:-80~12*/
    VOS_UINT16                          usPreDisable;
    VOS_UINT16                          usReserve;
    VOICE_PROC_SONDIND_CALLBACK         funcSndInd;                             /* _H2ASN_Replace VOS_UINT32 funcSndInd; *///Snd���Ž����ϱ��ص�����
} VOICE_PROC_SOUND_STRU;

/* 7.2.12 ����������� */
typedef struct
{
    VOS_UINT16                          enPpMicNum;                             /* PP������˸�������:��˫��� */
    VOS_UINT16                          usReserve;
    VOICE_PROC_CODEC_STRU              *pstCodec;                               /* MC�����codecָ�� */
} VOICE_PROC_OBJS_STRU;

typedef struct
{
    VOS_INT16                           asDecSerial[VOICE_CODED_FRAME_WITH_OBJ_LEN ];  /* ���������� */
    VOS_UINT32                          uwTimeStamp;                                   /* ����������ʱ��� */
} VOICE_DEC_SERIAL_STRU;

typedef struct
{
    VOS_INT16                           asEncSerial[VOICE_CODED_FRAME_WITH_OBJ_LEN ]; /* ��������� */
    VOICE_DEC_SERIAL_STRU               astDecSerial[VOICE_CODED_DECODE_BUF_FRAME_NUM]; /* ���������� */
    VOS_UINT32                          uwReserve;
    VOS_INT16                           asDecPrevData[GPHY_VOICE_DATA_LEN];           /* ��һ֡���������� */
    VOS_INT16                           asReserved;
    VOS_INT16                           asLineInBuff[CODEC_PCM_MAX_FRAME_LENGTH];     /* ����󻺳��� */
    VOS_INT16                           asLineOutBuff[CODEC_PCM_MAX_FRAME_LENGTH];    /* ����ǰ������ */
} VOICE_PROC_CODEC_DATA_STRU;


/*****************************************************************************
  8 UNION����
*****************************************************************************/

/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern VOICE_PROC_OBJS_STRU          g_stVoiceProcObjs;
extern VOICE_PROC_CODEC_DATA_STRU    g_stVoiceCodecData;                   /*���������ṹ��ȫ�ֱ���*/
extern VOICE_DEC_SERIAL_STRU        *g_pstDecSerialWrite;


/*****************************************************************************
  11 FUNCTION����
*****************************************************************************/
extern VOS_UINT32 VOICE_ProcCfgGsmDlCodec(GPHY_VOICE_RX_DATA_IND_STRU *pstRxDataMsg);
extern VOS_UINT32 VOICE_ProcCfgGsmUlCodec(GPHY_VOICE_UPDATE_PARA_CMD_STRU *pstUpdateMsg, VOS_UINT16 uhwModemNo);
extern VOS_UINT32 VOICE_ProcCfgUmtsDlCodec( VOS_VOID );
extern VOS_UINT32 VOICE_ProcCfgUmtsUlCodec( VOS_VOID );
extern VOS_UINT32 VOICE_ProcCfgDlCodec(
                        CODEC_NET_MODE_ENUM_UINT16         enNetMode,
                        VOS_INT16                         *pshwDecBuf,
                        VOS_UINT16                         uhwRxFrmLost,
                        VOS_INT16                        **pshwDecData );
extern VOS_VOID   VOICE_ProcInitDecSerialWrite( VOS_VOID );
extern VOS_VOID   VOICE_ProcAddDecSerialWrite(VOS_VOID *pvMsg, VOS_UINT32 uwLenBytes);
extern VOS_VOID   VOICE_ProcSubDecSerialWrite( VOS_UINT16 uhwRxFrmLost);

extern VOS_UINT32 VOICE_ProcDecode(VOS_INT16 *psStrm, VOS_INT16 *psPcm);
extern VOS_UINT32 VOICE_ProcDestroy(VOICE_PROC_OBJS_STRU *pstProcObj);
extern VOS_UINT32 VOICE_ProcEncode(VOS_INT16 *psPcm, VOS_INT16 *psStrm);
extern VOS_UINT32 VOICE_ProcFrameTypeToRlc(VOS_UINT16 uhwFrameTypeIdx);
extern VOS_INT16* VOICE_ProcGetEcRef( VOS_VOID );
extern VOS_UINT32 VOICE_ProcGetSampleRate(VOS_VOID);
extern VOS_UINT32 VOICE_ProcInit(VOS_VOID);
extern VOS_UINT32 VOICE_ProcInitDecoder(VOS_UINT16 usType);
extern VOS_UINT32 VOICE_ProcInitEncoder(VOS_UINT16 usType, VOS_UINT16 usDtxMode);
extern VOS_UINT32 VOICE_ProcOutputFrameType(VOS_UINT16 uhwFrameTypeIdx);
extern VOS_UINT32 VOICE_ProcRxPp(
                       VOS_INT16   *pshwLineIn,
                       VOS_INT16   *pshwSpkOut);
extern VOS_UINT32 VOICE_ProcTxPp(
                       VOS_INT16   *pshwMcIn,
                       VOS_INT16   *pshwRcIn,
                       VOS_INT16   *pshwLineOut);
extern VOS_UINT32 VOICE_ProcUpdate(VOS_UINT16 uhwDevMode,VOS_UINT16 uhwNetMode);
extern VOS_UINT32 VOICE_ProcVolume(
                       VOS_INT16               shwVolumeDb,
                       VOS_INT16              *pshwIn,
                       VOS_INT16              *pshwOut);
extern VOS_UINT32 VOICE_ProcFacch(VOS_INT16   *psPcm);
extern VOS_UINT32 VOICE_ProcBFI(VOICE_PROC_CODEC_STRU  *pstCodec);
extern VOS_UINT32 VOICE_ProcCfgImsCodec(
                VOS_UINT16 uhwCodecType,
                VOS_UINT16 uhwDtxEnable,
                VOS_UINT16 uhwRateMode);
extern MED_UINT16 VOICE_ProcCheckUpdate( VOS_UINT16 uhwCodecType  );

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /*_VOICE_PROC_H*/

