
#ifndef __AUDIO_PCM_H__
#define __AUDIO_PCM_H__
/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "audio.h"
#include  "vos.h"
#include  "med_drv_dma.h"
#include  "med_drv_sio.h"
#include  "hifidrvinterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/

typedef VOS_VOID (*AUDIO_PCM_CALLBACK)(VOS_VOID *pshwBufOut,    \
                                         VOS_VOID *pshwBufIn,   \
                                         VOS_UINT32 uwSampleRate,\
                                         VOS_UINT32 uwChannelNum,\
                                         VOS_UINT32 uwBufSize);

#define AUDIO_PCM_DMAC_CHN_CAPTURE      (2)                                    /* ��Ƶ����DMAͨ����ʹ��AP��EDMA��ͨ���Ŵ�ȷ�� */
#define AUDIO_PCM_DMAC_CHN_PLAY         (1)                                    /* ��Ƶ���DMAͨ����ʹ��AP��EDMA��ͨ���Ŵ�ȷ�� */

#define AUDIO_PCM_PINGPONG_BUF_NUM      (2)                                     /* ��Ƶƹ�һ���ĸ���������/¼��������ͬ */
#define AUDIO_PCM_MAX_CHANNEL_NUM       (2)
#define AUDIO_PCM_MAX_SAMPLE_RATE       (192000)
#define AUDIO_PCM_MAX_FRAME_LENGTH      (0x780)                                /* ������Ϊ192Kʱ20ms��˫ͨ�����ݣ���λ16bit */
#define AUDIO_PCM_FORMAT_16_SIZE        (2)                                     /* 16bit��������ռ�õĴ�С����λByte */
#define AUDIO_PCM_FRAME_PER_SEC         (50)                                    /* ÿ�����������Frame������50��ʾÿ֡20ms */

#define AUDIO_PcmGetPcmObjPtr()         (&g_stAudioPcmObj)
#define AUDIO_PcmGetPcmPbBufPtr()       (&g_stAudioPcmPbBuff)
#define AUDIO_PcmGetPlayTmpBufPtr()     (g_stAudioPcmTmpBuff.shwPlayTmpBuff)
#define AUDIO_PcmGetCaptureTmpBufPtr()  (g_stAudioPcmTmpBuff.shwCaptureTmpBuff)
#define AUDIO_PcmGetPlayBufAPtr()       (g_stAudioPcmPbBuff.shwBuffA)
#define AUDIO_PcmGetPlayBufBPtr()       (g_stAudioPcmPbBuff.shwBuffB)

#define AUDIO_PcmGetPcmCpBufPtr()       (&g_stAudioPcmCpBuff)
#define AUDIO_PcmGetCaptureBufAPtr()    (g_stAudioPcmCpBuff.shwBuffA)
#define AUDIO_PcmGetCaptureBufBPtr()    (g_stAudioPcmCpBuff.shwBuffB)

#define AUDIO_PcmGetPlayDmaCfgPtr()     (&g_astAudioPlayDmaPara[0])
#define AUDIO_PcmGetCaptureDmaCfgPtr()  (&g_astAudioCaptureDmaPara[0])
#define AUDIO_PcmGetPlayDmaCfgAPtr()    (&g_astAudioPlayDmaPara[0])
#define AUDIO_PcmGetPlayDmaCfgBPtr()    (&g_astAudioPlayDmaPara[1])
#define AUDIO_PcmGetCaptureDmaCfgAPtr() (&g_astAudioCaptureDmaPara[0])
#define AUDIO_PcmGetCaptureDmaCfgBPtr() (&g_astAudioCaptureDmaPara[1])

#define AUDIO_PcmGetState()             (g_stAudioPcmObj.enPcmStatus)
#define AUDIO_PcmGetSpeState(uhwMode)   (g_stAudioPcmObj.astPcmCfg[uhwMode].enPcmStatus)
#define AUDIO_PcmGetPcmCfgPtr(uhwMode)  (&g_stAudioPcmObj.astPcmCfg[uhwMode])

#define AUDIO_PcmSetState(uhwStatus)    (g_stAudioPcmObj.enPcmStatus = (uhwStatus))        /* ���������������״̬ */
#define AUDIO_PcmSetSpeState(uhwMode, uhwStatus)  \
                                        (g_stAudioPcmObj.astPcmCfg[uhwMode].enPcmStatus = (uhwStatus))

#ifndef AUDIO_IO_SLIMBUS
#define AUDIO_PCM_IO_Open(enIntMask,uwSampleRate,uhwIsAudioMaster,pfIntHandle,uwPara) \
            DRV_SIO_Audio_Open(enIntMask,uwSampleRate,uhwIsAudioMaster,pfIntHandle,uwPara)
#define AUDIO_PCM_IO_Close()            DRV_SIO_Audio_Close()
#else
#define AUDIO_PCM_IO_Open(enIntMask,uwSampleRate,pfIntHandle,uwPara)
#define AUDIO_PCM_IO_Close()            DRV_SLIMBUS_Audio_Close()
#endif

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
/*****************************************************************************
 ʵ������  : AUDIO_PCM_BUFF_ISREADY_ENUM
 ��������  : ap��hifi���͵�buff�Ƿ�ready��־λ
*****************************************************************************/
enum AUDIO_PCM_BUFF_ISREADY_ENUM
{
    AUDIO_PCM_BUFF_ISREADY_NO = 0,
    AUDIO_PCM_BUFF_ISREADY_YES,
    AUDIO_PCM_BUFF_ISREADY_BUT
};
typedef VOS_UINT16  AUDIO_PCM_BUFF_ISREADY_ENUM_UINT16;

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/

/*****************************************************************************
 ʵ������  : AUDIO_PCM_UPDATE_PLAY_BUFF_CMD_STRU
 ��������  : ����Ϣ����Hifi�Ĳ���Buff��Ϣ����ʾһ֡�����Ѿ��������
*****************************************************************************/
 typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT16                          uhwMsgId;           /*_H2ASN_Skip */
    VOS_UINT16                          uhwReserve;
} AUDIO_PCM_UPDATE_PLAY_BUFF_CMD_STRU;

/*****************************************************************************
 ʵ������  : AUDIO_PCM_UPDATE_CAPTURE_BUFF_CMD_STRU
 ��������  : ����Ϣ����Hifi��¼��Buff��Ϣ����ʾһ֡�����Ѿ�¼�����
*****************************************************************************/
 typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT16                          uhwMsgId;           /*_H2ASN_Skip */
    VOS_UINT16                          uhwReserve;
} AUDIO_PCM_UPDATE_CAPTURE_BUFF_CMD_STRU;

/*****************************************************************************
  6 STRUCT����
*****************************************************************************/

/*****************************************************************************
 ʵ������  : AUDIO_PCM_PINGPONG_BUF_STRU
 ��������  : ��Ƶ�������ƹ��BUFF�ṹ��
*****************************************************************************/
typedef struct
{
    VOS_INT16                           shwBuffA[AUDIO_PCM_MAX_FRAME_LENGTH];
    VOS_INT16                           shwBuffB[AUDIO_PCM_MAX_FRAME_LENGTH];
}AUDIO_PCM_PINGPONG_BUF_STRU;

typedef struct
{
    VOS_INT16                           shwPlayTmpBuff[AUDIO_PCM_MAX_FRAME_LENGTH];
    VOS_INT16                           shwCaptureTmpBuff[AUDIO_PCM_MAX_FRAME_LENGTH];
}AUDIO_PCM_TMP_BUF_STRU;
/*****************************************************************************
 ʵ������  : AUDIO_PCM_BUF_STRU
 ��������  : ��Ƶ�������ͨ��״̬�ṹ��
*****************************************************************************/
 typedef struct
{
    AUDIO_PCM_STATUS_ENUM_UINT16        enBufStatus;        /* ����״̬����Ҫ���Ƿ�ʹ�ø�Buf����������� */
    AUDIO_PCM_BUFF_ISREADY_ENUM_UINT16  enBufReadyFlag;     /* ap��hifi���͵�buff�Ƿ�ready��־λ */
    VOS_UINT32                          uwBufAddr;          /* ������׵�ַ */
    AUDIO_PCM_CALLBACK                  pfunCallBack;       /* ͨ������Ļص����� */
    AUDIO_PCM_CALLBACK                  pfunVcRecordCB;     /* ͨ��¼���Ļص���������ͨ��¼��ʱ��Ч */
    VOS_VOID                           *pSubStream;         /* ��ǰBuff��Ӧ����Ƶ�������,��AP����Ч */
}AUDIO_PCM_BUF_STRU;

/*****************************************************************************
 ʵ������  : AUDIO_PCM_CFG_STRU
 ��������  : ��Ƶ�������ͨ��״̬�ṹ��
*****************************************************************************/
 typedef struct
{
    AUDIO_PCM_STATUS_ENUM_UINT16        enPcmStatus;        /* ��Ƶͨ·״̬ */
    AUDIO_CAPTURE_MODE_ENUM_UINT16      enCaptureMode;      /* ¼��ģʽ������¼��ͨ����Ч */
    VOS_UINT32                          uwSampleRate;       /* ��ǰͨ�������� */
    VOS_UINT32                          uwChannelNum;       /* ͨ���������� */
    VOS_UINT32                          uwBufSize;          /* �������Ĵ�С����λbyte,Ĭ��Ϊ20ms�������� */
    AUDIO_PCM_BUF_STRU                  astPcmBuf[AUDIO_PCM_OBJ_BUT];
                                                            /* AP��Hifi�ṩ��BUF��Ϣ����AUDIO_PCM_OBJ_ENUM˳������ */
}AUDIO_PCM_CFG_STRU;

/*****************************************************************************
 ʵ������  : AUDIO_PCM_CTRL_OBJ_STRU
 ��������  : ��Ƶͨ·������ƽṹ��
*****************************************************************************/
 typedef struct
{
    AUDIO_PCM_STATUS_ENUM_UINT16        enPcmStatus;        /* ��Ƶͨ·����״̬ */
    VOS_UINT16                          uhwReserve;
    AUDIO_PCM_CFG_STRU                  astPcmCfg[AUDIO_PCM_MODE_BUT];
                                                            /* ��ǰ���š�¼���Ĳ�������AUDIO_PCM_MODE_ENUM˳������ */
}AUDIO_PCM_CTRL_OBJ_STRU;

/*****************************************************************************
 �ṹ��    : AUDIO_PCM_ENHANCE_START_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : ������Ƶ��ǿ������Ϣ�ṹ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT16                          uhwMsgId;           /*_H2ASN_Skip */
    VOS_UINT16                          uhwReserve;
    VOS_UINT32                          uwSampleRate;
    AUDIO_ENHANCE_MODULE_ENUM_UINT32    enModule;           /* ��Ҫ������ģ�� */
}AUDIO_PCM_ENHANCE_START_IND_STRU;

/*****************************************************************************
 �ṹ��    : AUDIO_PCM_ENHANCE_STOP_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : ֹͣ��Ƶ��ǿ������Ϣ�ṹ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT16                          uhwMsgId;           /*_H2ASN_Skip */
    VOS_UINT16                          uhwReserve;
    AUDIO_ENHANCE_MODULE_ENUM_UINT32    enModule;           /* ��Ҫ������ģ�� */
}AUDIO_PCM_ENHANCE_STOP_IND_STRU;

/*****************************************************************************
 �ṹ��    : AUDIO_PCM_ENHANCE_SET_DEVICE_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �����豸ģʽ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT16                          uhwMsgId;           /*_H2ASN_Skip */
    VOS_UINT16                          uhwReserve;
    AUDIO_VOIP_DEVICE_ENUM_UINT32       uwDevice;
}AUDIO_PCM_ENHANCE_SET_DEVICE_IND_STRU;

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern AUDIO_PCM_CTRL_OBJ_STRU                 g_stAudioPcmObj;

/*****************************************************************************
  10 ��������
*****************************************************************************/

extern VOS_VOID AUDIO_PCM_CaptureDmaIsr(
                       DRV_DMA_INT_TYPE_ENUM_UINT16 enIntType,
                       VOS_UINT32 uwPara);
extern VOS_INT16* AUDIO_PCM_GetCurCaptureBuff(VOS_VOID);
extern VOS_INT16* AUDIO_PCM_GetCurPlayBuff(VOS_VOID);
extern VOS_VOID AUDIO_PCM_Init(VOS_VOID);
extern VOS_UINT32 AUDIO_PCM_MsgPcmCloseReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_PCM_MsgPcmHwFreeReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_PCM_MsgPcmHwParaReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_PCM_MsgPcmOpenReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_PCM_MsgPcmPointerReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_PCM_MsgPcmPrepareReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_PCM_MsgPcmSetBufCmd(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_PCM_MsgPcmTriggerReq(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_PCM_MsgUpdateCaptureBuffCmd(VOS_VOID *pvOsaMsg);
extern VOS_UINT32 AUDIO_PCM_MsgUpdatePlayBuffCmd(VOS_VOID *pvOsaMsg);
extern VOS_VOID AUDIO_PCM_PlayDmaIsr(
                       DRV_DMA_INT_TYPE_ENUM_UINT16 enIntType,
                       VOS_UINT32 uwPara);
extern VOS_UINT32 AUDIO_PCM_RegisterPcmCbFunc( AUDIO_PCM_MODE_ENUM_UINT16 enPcmMode,
                                                           AUDIO_PCM_OBJ_ENUM_UINT16 enPcmObj,
                                                           AUDIO_CAPTURE_MODE_ENUM_UINT16  enCaptureMode,
                                                           AUDIO_PCM_CALLBACK pfunCallBack);
extern VOS_UINT32 AUDIO_PCM_StartCaptureLoopDMA(VOS_UINT16 uhwLen, VOS_UINT16 usChNum);
extern VOS_UINT32 AUDIO_PCM_StartPlayLoopDMA(VOS_UINT16 uhwLen, VOS_UINT16 usChNum);
extern VOS_UINT32 AUDIO_PCM_UnRegisterPcmCbFunc(AUDIO_PCM_MODE_ENUM_UINT16 enPcmMode,
                                                             AUDIO_PCM_OBJ_ENUM_UINT16 enPcmObj);








#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of audio_pcm.h */

