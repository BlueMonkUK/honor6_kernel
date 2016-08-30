

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "codec_typedefine.h"
#include "codec_com_codec.h"
#include "ucom_stub.h"
#include "med_drv_ipc.h"
#include "med_drv_dma.h"
#include "med_drv_sio.h"


#ifndef __UCOM_PCM_H__
#define __UCOM_PCM_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
/* ͨ�� */
/* ���֧�ֲ�����Ϊ48k��һ֡�Ĵ�СΪ48000*0.02 */
#define UCOM_PCM_MAX_FRAME_LENGTH          (960)

/* ����ΪI2S��ʽ�������ӵ������2·PCM���� */
#define UCOM_PCM_I2S_CHANNEL_NUM           (2)

/* ƹ��BUFF��󳤶� */
#define UCOM_PCM_PINGPONG_LEN              (UCOM_PCM_MAX_FRAME_LENGTH * UCOM_PCM_I2S_CHANNEL_NUM)

/* ����BUFF����2·���� */
#define UCOM_PCM_PINGPONG_BUFF_NUM         (2)

/* ����ΪSMART PAʹ�� */
/* V7R2��û�з����ͨ�� */
#define UCOM_PCM_DMAC_CHN_SPA              (VOS_NULL)
#define UCOM_PCM_GetSpaCfgPtr()            (&g_stSmartPaCfg)


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/
/* ����PCM���ݿ��ƽṹ�� */
typedef struct
{
    VOS_UINT16            enIsInit;             /* �Ƿ��ʼ�� */
    VOS_UINT16            uhwDataLenBytes;      /* ���ݳ��� */
    VOS_INT16            *pshwDestVirtualAddr;  /* ��ǰ���ݵ�ַ */
}UCOM_PCM_CFG_STRU;


typedef struct
{
    VOS_INT16             ashwSmartPaBuffA[UCOM_PCM_PINGPONG_LEN];
    VOS_INT16             ashwSmartPaBuffB[UCOM_PCM_PINGPONG_LEN];
}UCOM_PCM_BUFFER_STRU;

/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/


/*****************************************************************************
  6 STRUCT����
*****************************************************************************/


/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern DRV_DMA_CXCFG_STRU                      g_astSmartPaInDmaPara[2];
extern UCOM_PCM_CFG_STRU                       g_stSmartPaCfg;
extern UCOM_PCM_BUFFER_STRU                    g_stUcomPcmBuff;
/*****************************************************************************
  10 ��������
*****************************************************************************/

extern VOS_VOID UCOM_PCM_SmartPaPcmInit(VOS_VOID);
extern VOS_UINT32 UCOM_PCM_SmartPaStartLoopDma(
                      VOS_UINT16              uhwFrameLenBytes,
                      VOS_UINT32              uwSampleRate,
                      VOS_UINT16              uhwSmartPaMaster,
                      VOS_UINT16              usChNum,
                      DRV_DMA_INT_FUNC        pfuncIsr);
extern VOS_VOID UCOM_PCM_SmartPaStop(VOS_VOID);
extern VOS_UINT32 UCOM_PCM_SmartPaGetAvailVirtAddr(VOS_VOID);
extern VOS_UINT16 UCOM_PCM_GetSmartPaDmacChn(VOS_VOID);




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of ucom_pcm.h */
