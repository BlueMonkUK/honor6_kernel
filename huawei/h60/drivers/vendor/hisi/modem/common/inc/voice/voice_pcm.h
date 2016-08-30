/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : VOICE_pcm.h
  �� �� ��   : ����
  ��    ��   : л���� 58441
  ��������   : 2011��6��2��
  ����޸�   :
  ��������   : VOICE_pcm.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��6��2��
    ��    ��   : л���� 58441
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __VOICE_PCM_H__
#define __VOICE_PCM_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "codec_typedefine.h"
#include "codec_com_codec.h"
#include "OmCodecInterface.h"
#include "VcCodecInterface.h"
#include "ucom_stub.h"
#include "med_drv_ipc.h"
#include "med_drv_dma.h"
#include "med_drv_sio.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define VOICE_PCM_FRAME_INSERT_THD   (5)                                     /* ��֡����ֵ */
#define VOICE_PCM_FRAME_WIN_LEN      (1000)                                  /* ����������ͳ�ƴ���*/

#define VOICE_PCM_SHIFT_RIGHT_16     (16)                                    /* ����16λ*/
#define VOICE_PCM_AHB_ADDR_INC       (4)                                     /* AHB�����ַ����*/
#define VOICE_PCM_TX_BUF_SIZE_INIT   (5)                                     /* ���л���buf��ʼֵ*/
#define VOICE_PCM_RX_BUF_SIZE_INIT   (3)                                     /* ���л���buf��ʼֵ*/
#define VOICE_PCM_SIGNAL_RAND_RANGE_32  (32)                                    /* С�ź���������� */

#define VOICE_PcmSaveNvMemAddr(uwAddr)\
                (g_stVoicePcVoiceObj.uwRingBuffBaseAddr = (uwAddr))
/*****************************************************************************
 ��    ��  : VOICE_PcmTransferRxDataInd
 ��������  : ֪ͨOM�����������ݰ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��

 ****************************************************************************/
#define VOICE_PcmTransferRxDataInd() \
{ \
    DRV_IPC_TrigInt(DRV_IPC_CORE_ACPU, PC_VOICE_RX_DATA_ACPU_IPC_BIT); \
}

#define VOICE_CODED_FRAME_LENGTH                   ((VOS_UINT16)164)               /* һ֡�������������ݵ����֡������λ˫�ֽڣ�����ΪAMR_WB��72���ֽ�*/
#define VOICE_CODED_FRAME_WITH_OBJ_LEN             ((VOS_UINT16)256)               /* һ֡�������������ݵ����֡������λ˫�ֽڣ�����־*/
#define VOICE_CODED_DECODE_BUF_FRAME_NUM           ((VOS_UINT16)2)               /* �������л�������֡��Ϊ2������GSM��20ms�������յ�2֡���������� */
#define VOICE_CODED_DECODE_SERIAL_LEN  \
        ((VOICE_CODED_DECODE_BUF_FRAME_NUM * VOICE_CODED_FRAME_WITH_OBJ_LEN))   /* �������л���ĳ��ȣ�����2֡���������� */


/* ȫ�ֱ�����װ */
/* ��ʱ�ȸ�Ϊֻ������MIC����,�����ٽ�������ͬʱ���� */
#define VOICE_PcmGetMcInBufPtr()        (g_stVoicePcmBuff.asMicInBuffLeft)
#define VOICE_PcmGetRcInBufPtr()        (g_stVoicePcmBuff.asMicInBuffRight)
#define VOICE_PcmGetSpkOutBufPtr()      (g_psVoicePcmSpkOut)
#define VOICE_PcmGetSpkOutBufTempPtr()  (g_stVoicePcmBuff.asSpkOutBuffTemp)
#define VOICE_PcmGetRxBufStatePtr()     (&g_stVoicePcVoiceObj.stTxRingBufferState)
#define VOICE_PcmGetEcRefLeftChanPtr()  (g_stVoicePcmBuff.asEcRefBuffLeft)
#define VOICE_PcmGetEcRefRightChanPtr() (g_stVoicePcmBuff.asEcRefBuffRight)

#if (VOS_CPU_TYPE == VOS_HIFI)
/*PC VOICE ���л���buffer������Ϣ ��HIFIͨ����ַӳ��ʵ��uncache�ķ���*/
#define CODEC_PC_VOICE_OM_CODEC_ADDR        \
                (UCOM_GetUncachedAddr(g_stVoicePcVoiceObj.uwRingBuffBaseAddr))

/*PC VOICE ���л���buffer������Ϣ ��HIFIͨ����ַӳ��ʵ��uncache�ķ���*/
#define CODEC_PC_VOICE_CODEC_OM_ADDR        \
                (UCOM_GetUncachedAddr(g_stVoicePcVoiceObj.uwRingBuffBaseAddr))
#endif

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/* 3.1 ƹ�һ���ö�� */
typedef enum
{
    VOICE_PCM_BUFF_A = 0,
    VOICE_PCM_BUFF_B,
    VOICE_PCM_BUFF_BUTT
} VOICE_PCM_BUFF_ENUM;

/* 3.2 ���˵�������ö�� */
typedef enum
{
    VOICE_PCM_PLAY_TX = 0,
    VOICE_PCM_PLAY_RX,
    VOICE_PCM_PLAY_TXRX,
    VOICE_PCM_PLAY_BUTT
} VOICE_PCM_PLAY_ENUM;

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

/* 7.1 PCM����buffer�ṹ�� */
typedef struct
{
    VOS_INT16                           asMicInBuffA[CODEC_PCM_MAX_FRAME_LENGTH * 2];
    VOS_INT16                           asMicInBuffB[CODEC_PCM_MAX_FRAME_LENGTH * 2];
    VOS_INT16                           asMicInBuffLeft[CODEC_PCM_MAX_FRAME_LENGTH];
    VOS_INT16                           asMicInBuffRight[CODEC_PCM_MAX_FRAME_LENGTH];
    VOS_INT16                           asSpkOutBuffA[CODEC_PCM_MAX_FRAME_LENGTH * 2];
    VOS_INT16                           asSpkOutBuffB[CODEC_PCM_MAX_FRAME_LENGTH * 2];
    VOS_INT16                           asSpkOutBuffTemp[CODEC_PCM_MAX_FRAME_LENGTH];
    VOS_INT16                           asEcRefBuffA[CODEC_PCM_MAX_FRAME_LENGTH * 2];
    VOS_INT16                           asEcRefBuffB[CODEC_PCM_MAX_FRAME_LENGTH * 2];
    VOS_INT16                           asEcRefBuffLeft[CODEC_PCM_MAX_FRAME_LENGTH];
    VOS_INT16                           asEcRefBuffRight[CODEC_PCM_MAX_FRAME_LENGTH];
} VOICE_PCM_BUFFER_STRU;

/* ring buffer״̬�ṹ��*/
typedef struct
{
    VOS_UINT16                          uhwAdpBufferSize;                       /*����Ӧ�����С����λ��֡*/
    VOS_UINT16                          uhwCntDataSizeIsOne;                    /*���������ݴ�С����1�Ĵ���*/
    VOS_UINT16                          uhwCurrDataSize;                        /*��ǰ���������ݴ�С����λ��֡*/
    VOS_INT16                           shwInsertFrameCnt;                      /*��֡��*/
    VOS_UINT16                          uhwFrameCnt;                            /*֡��������*/
    VOS_UINT16                          uhwReserved;
}VOICE_PCM_RING_BUFFER_STATE_STRU;

/* PC Voice����ṹ��*/
typedef struct
{
    VOS_UINT32                          uwRingBuffBaseAddr;
    VOICE_PCM_RING_BUFFER_STATE_STRU    stTxRingBufferState;                    /*����ring buffer״̬�ṹ��*/
    VOICE_PCM_RING_BUFFER_STATE_STRU    stRxRingBufferState;                    /*����ring buffer״̬�ṹ��*/
}VOICE_PCM_PC_VOICE_OBJ_STRU;
/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern VOS_INT16                       *g_psVoicePcmMicIn;                    /*ָ��ǰMic�����PCM����Buffer��ָ��*/
extern VOS_INT16                       *g_psVoicePcmSpkOut;                   /*ָ��ǰ�����Speaker��PCM����Buffer��ָ��*/
extern VOS_INT16                       *g_psVoiceSmartPaPtr;
extern VOS_INT16                       *g_psVoiceEcRef;
extern VOS_INT16                       *g_psVoiceSpaDes;

/* DMAͨ�����ò����ṹ��ȫ�ֱ��� */
extern DRV_DMA_CXCFG_STRU               g_astVoicePcmMicInDmaPara[2];
extern DRV_DMA_CXCFG_STRU               g_astVoicePcmSpkOutDmaPara[2];
extern DRV_DMA_CXCFG_STRU               g_astVoiceEcRefInDmaPara[2];
extern VOICE_PCM_BUFFER_STRU            g_stVoicePcmBuff;                     /*PCM�����ṹ��ȫ�ֱ���*/

extern VOICE_PCM_PC_VOICE_OBJ_STRU      g_stVoicePcVoiceObj;                  /*PC Voiceʵ��ȫ�ֱ��� */

/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_VOID VOICE_EcRefDmaIsr(
                       DRV_DMA_INT_TYPE_ENUM_UINT16 enIntType,
                       VOS_UINT32 uwPara);
extern VOS_VOID VOICE_MicDmaIsr(
                       DRV_DMA_INT_TYPE_ENUM_UINT16 enIntType,
                       VOS_UINT32 uwPara);
extern VOS_VOID VOICE_PcmAdjustRingBufSize(
                       OMMED_RING_BUFFER_CONTROL_STRU      *pstTxRingBufferControl,
                       VOICE_PCM_RING_BUFFER_STATE_STRU    *pstTxRingBufferState);
extern VOS_VOID VOICE_PcmClrLastSpkBuff(VOS_VOID);
extern VOS_INT16* VOICE_PcmGetWritableSpkBuffPtr(VOS_VOID);
extern VOS_UINT32 VOICE_PcmHybrid2Stereo(VOS_INT16 *pshwI2S, VOS_INT16 *pshwLeft, VOS_INT16 *pshwRight, VOS_INT16 shwPcmLen);
extern VOS_VOID VOICE_PcmInit(VOS_VOID);
extern VOS_VOID VOICE_PcmInsertCn(
                       VOS_INT16   *pshwIn,
                       VOS_INT16    shwLen,
                       VOS_INT16    shwPcmRandRange);
extern VOS_VOID VOICE_PcmManageRingBuf(
                       VOS_UINT16  uhwAdpBufferSize,
                       VOS_UINT16  uhwCurrDataSize,
                       VOS_INT16  *pshwBufProcess);
extern VOS_VOID VOICE_PcmMicInNoData(VOS_VOID);
extern VOS_UINT32 VOICE_PcmMicInStartLoopDMA(VOS_UINT16 usChNum, DRV_DMA_INT_FUNC pfuncIsr);
extern VOS_UINT32 VOICE_PcmSpkOutStartLoopDMA(VOS_UINT16 usChNum, DRV_DMA_INT_FUNC pfuncIsr);
extern VOS_UINT32 VOICE_PcmStartDMAFromDataIn(VOS_UINT16 usChNum);
extern VOS_UINT32 VOICE_PcmStartDMAToDataOut(VOS_UINT16 usChNum);
extern VOS_UINT32 VOICE_PcmStereo2Hybrid(
                       VOS_INT16  *pshwSpkOutTemp,
                       VOS_INT16  *pshwSpkOut,
                       VOS_INT16   shwPcmLen);
extern VOS_VOID VOICE_PcmSwitchMicBuff(VOS_VOID);
extern VOS_VOID VOICE_PcmSwitchSpkBuff(VOS_VOID);
extern VOS_UINT32 VOICE_PcmUpdate(
                       VOS_UINT32                      swSampleRate,
                       VCVOICE_DEV_MODE_ENUM_UINT16    enDevMode);
extern VOS_VOID VOICE_PcmUpdateRxRingBuf(VOS_VOID);
extern VOS_VOID VOICE_PcmUpdateTxRingBuf(VOS_VOID);
extern VOS_VOID VOICE_PcmVcLoopMicIsr(DRV_DMA_INT_TYPE_ENUM_UINT16 uhwPara, VOS_UINT32 uwIntType);
extern VOS_VOID VOICE_PcmVcLoopSpkIsr(DRV_DMA_INT_TYPE_ENUM_UINT16 uhwPara, VOS_UINT32 uwIntType);
extern VOS_VOID VOICE_SioIsr(DRV_SIO_INT_MASK_ENUM_UINT32 enIntType, VOS_UINT32 uwPara);
extern VOS_VOID VOICE_SpkDmaIsr(
                       DRV_DMA_INT_TYPE_ENUM_UINT16 enIntType,
                       VOS_UINT32 uwPara);





#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif

#endif /* end of med_pcm.h */
