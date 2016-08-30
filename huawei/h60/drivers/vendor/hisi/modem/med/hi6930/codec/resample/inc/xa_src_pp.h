

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "audio.h"
#include "xa_comm.h"
#include "xa_src_pp_api.h"

#ifndef __XA_SRC_MY_H__
#define __XA_SRC_MY_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define XA_SRC_MAX_PROCESS_NUM              (10)                                /* ���֧�ֶ������������ĸ��� */
#define XA_SRC_MAX_CHANLE_NUM               (8)                                 /* ���֧��8���� */
#define XA_SRC_CURRENT_CHANLE_NUM           (2)                                 /* ��ǰ���Ϊ˫�������� */
#define XA_SRC_MAX_INPUT_CHUNK_LEN          (512)                               /* SRCģ�������������������4~512 Ϊ4�ı��� */
#define XA_SRC_INPUT_CHUNKSIZE_128          (128)                               /* ÿ������128������(N����) */
#define XA_SRC_INPUT_CHUNKSIZE_160          (160)                               /* ÿ������160������(N����) */
#define XA_SRC_INPUT_CHUNKSIZE_240          (240)                               /* ÿ������240������(N����) */
#define XA_SRC_REQUIRED_ALIGNMENT           (8)                                 /* SRCģ�����õ����ڴ��׵�ַҪ��8�ֽڶ��� */
#define XA_SRC_FINAL_OUTPUT_BUFF_SIZE       (0x4E00)                            /* SRC��DOLBY����ء�192K��Ϊ26ms˫�������ݳ��ȣ���λ:byte */

#define XA_SRC_GetApiPtr()                          (g_pfuncAudioSrcApi)                /* ��ȡSRC ������ָ�� */
#define XA_SRC_SetApiPtr(apiPtr)                    (g_pfuncAudioSrcApi = apiPtr)       /* ����SRC ������ָ�� */
#define XA_SRC_GetApiObjPtr(enProcId)               (g_pastAudioSrcApiObj[enProcId])    /* ��ȡSRC ���ýṹ��ָ�� */
#define XA_SRC_SetApiObjPtr(enProcId, objPtr)       (g_pastAudioSrcApiObj[enProcId] = objPtr)
                                                                                        /* ��ȡSRC ���ýṹ��ָ�� */
#define XA_SRC_GetMemCfgPtr(enProcId)               (&g_astSrcMemCfg[enProcId])         /* ��ȡSRCģ�����漰�����ڴ����ýṹ�� */
#define XA_SRC_GetInputBuff(enProcId)               (g_astSrcCurrentCfg[enProcId].pswSrcInput)
                                                                                        /* ��ȡSRCģ������Buffָ�� */
#define XA_SRC_GetOutputBuff(enProcId)              (g_astSrcCurrentCfg[enProcId].pswSrcOutput)
                                                                                        /* ��ȡSRCģ�����Buffָ�� */
#define XA_SRC_GetCurrentCfgPtr(enProcId)           (&g_astSrcCurrentCfg[enProcId])     /* ��ȡ��ǰ����״̬������Ϣ */
#define XA_SRC_SetInputBuff(enProcId, pswInput)     (g_astSrcCurrentCfg[enProcId].pswSrcInput = pswInput)
                                                                                        /* ��ȡSRCģ������Buffָ�� */
#define XA_SRC_SetOutputBuff(enProcId, pswOutput)   (g_astSrcCurrentCfg[enProcId].pswSrcOutput = pswOutput)
                                                                                        /* ��ȡSRCģ�����Buffָ�� */

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
 ʵ������  : XA_SRC_PROC_ID_ENUM_UINT32
 ��������  : SRC�����ö��
*****************************************************************************/
enum
{
    XA_SRC_PlAYBACK = 0,                                                        /* ��Ƶ����ʱ�ı���� */

    XA_SRC_VOIP_MICIN_48K_16K_L,                                                /* VOIP MICIN������48K��������16K */
    XA_SRC_VOIP_MICIN_48K_16K_R,                                                /* VOIP MICIN������48K��������16K */
    XA_SRC_VOIP_MICIN_16K_48K_L,                                                /* VOIP MICIN������16K��������48K */
    XA_SRC_VOIP_MICIN_16K_48K_R,                                                /* VOIP MICIN������16K��������48K */

    XA_SRC_VOIP_SPKOUT_48K_16K_L,                                               /* VOIP SPKOUT������48K��������16K */
    XA_SRC_VOIP_SPKOUT_48K_16K_R,                                               /* VOIP SPKOUT������48K��������16K */
    XA_SRC_VOIP_SPKOUT_16K_48K_L,                                               /* VOIP SPKOUT������16K��������48K */
    XA_SRC_VOIP_SPKOUT_16K_48K_R,                                               /* VOIP SPKOUT������16K��������48K */

    XA_SRC_VOIP_SMARTPA_48K_16K_L,                                              /* VOIP SMARTPA������48K��������16K */
    XA_SRC_VOIP_SMARTPA_48K_16K_R,                                              /* VOIP SMARTPA������48K��������16K */

    XA_SRC_VOICE_RECORD_TX,                                                     /* ͨ��¼�����б���� */
    XA_SRC_VOICE_RECORD_RX,                                                     /* ͨ��¼�����б���� */

    XA_SRC_PROC_ID_BUTT
};
typedef VOS_UINT32 XA_SRC_PROC_ID_ENUM_UINT32;

/*****************************************************************************
 ʵ������  : XA_SRC_INIT_ENUM_UINT32
 ��������  : SRC��ʼ����ö��
*****************************************************************************/
enum
{
    XA_SRC_INIT_NO = 0,                                                         /* �����δ��ʼ�� */
    XA_SRC_INIT_OK = 1,                                                         /* ������Ѿ���ʼ�� */
    XA_SRC_INIT_BUT
};
typedef VOS_UINT32 XA_SRC_INIT_ENUM_UINT32;

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
 ʵ������  : XA_SRC_USER_CFG_STRU
 ��������  : SRC�����ṩ��������Ϣ�ṹ��
*****************************************************************************/
typedef struct
{
    VOS_INT32           swChannels;                                             /* �ⲿ���������� */
    VOS_INT32           swInputSmpRat;                                          /* �ⲿ������������� */
    VOS_INT32           swInputChunkSize;                                       /* ��������������32~512������Ϊ4�ı��� */
    VOS_INT32           swOutputSmpRat;                                         /* �ⲿ������������� */
} XA_SRC_USER_CFG_STRU;

/*****************************************************************************
 ʵ������  : XA_SRC_CURRENT_CFG_STRU
 ��������  : SRC��ǰʹ�õ�������Ϣ�ṹ��
*****************************************************************************/
typedef struct
{
    VOS_INT32                       *pswSrcInput;                               /* ��ģ���ڲ�ʹ�õ����뻺�壬���ö�̬�����ڴ�,��Ƶ������ɺ�ͳһ�ͷ� */
    VOS_INT32                       *pswSrcOutput;                              /* ��ģ���ڲ�ʹ�õ�������壬�����������Ҫ��8�ֽڶ��� */
    XA_SRC_INIT_ENUM_UINT32          enInitFlag;                                /* �Ƿ��ʼ����־λ */
    VOS_INT32                        swChannels;                                /* �ⲿ���������� */
    VOS_INT32                        swInputSmpRat;                             /* �ⲿ������������� */
    VOS_INT32                        swInputChunkSize;                          /* ������������ */
    VOS_INT32                        swOutputSmpRat;                            /* �ⲿ������������� */
    VOS_INT32                        swOutputChunkSize;                         /* ������������ */
} XA_SRC_CURRENT_CFG_STRU;

/*****************************************************************************
 ʵ������  : XA_SRC_IO_CFG_STRU
 ��������  : SRC���������buff���ڴ��Ƕ�̬�����
*****************************************************************************/
typedef struct
{
    VOS_INT32          *paswInputBuff[8];                                       /* �������� */
    VOS_INT32          *paswOutputBuff[8];                                      /* ������� */
} XA_SRC_IO_CFG_STRU;

/*****************************************************************************
 ʵ������  : XA_SRC_SOURCE_STRU
 ��������  : SRC����������ݶ���
*****************************************************************************/
typedef struct
{
    VOS_UINT8          *pucData;                                                /* ���������׵�ַ(4�ֽڶ���)*/
    VOS_INT32           swSize;                                                 /* �������ݴ�С(��λByte) */
    VOS_INT32           swSmpRat;                                               /* ������(8000 - 8KHz, 16000 - 16KHz, ...) */
    VOS_INT32           swReso;                                                 /* ÿ�������Byte�� */
    VOS_INT32           swChnNum;                                               /* ͨ���� */
} XA_SRC_SOURCE_STRU;

/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_UINT32 XA_SRC_Close(XA_SRC_PROC_ID_ENUM_UINT32 enProcId);
extern VOS_VOID XA_SRC_Convert16To24bit(XA_SRC_PROC_ID_ENUM_UINT32 enProcId,
                                                VOS_VOID *pSrcBuff,
                                                VOS_INT32 *pswDestBuff);
extern VOS_VOID XA_SRC_Convert24To16bit( XA_SRC_PROC_ID_ENUM_UINT32 enProcId,
                                                 VOS_INT32 **ppuwSrcBuff,
                                                 VOS_VOID  *pDestBuff);
extern VOS_VOID  XA_SRC_ConvertChn24To16bit( VOS_INT16* pshwDst, VOS_INT32 *pswSrc, VOS_UINT32 uwLen);
extern VOS_UINT32 XA_SRC_ExeProcess( XA_SRC_PROC_ID_ENUM_UINT32 enProcId,
                                            VOS_VOID   *pInputBuff,
                                            VOS_VOID   *pOutputBuff,
                                            VOS_UINT32 *puwOutputBuffSize);
extern VOS_INT32 XA_SRC_Init( XA_SRC_PROC_ID_ENUM_UINT32 enProcId,
                                  XA_SRC_USER_CFG_STRU * pstSrcUserCfg);
extern VOS_INT32  XA_SRC_MemoryAlloc (xa_codec_func_t                  *pfuncProcess,
                                              xa_codec_handle_t                 pstApiObj,
                                              XA_COMM_MEMORY_CFG_STRU          *pstMemoryCfg,
                                              XA_SRC_USER_CFG_STRU             *pstSrcUserCfg,
                                              XA_SRC_PROC_ID_ENUM_UINT32        enProcId);
extern VOS_UINT32  XA_SRC_Proc8x(XA_SRC_PROC_ID_ENUM_UINT32   enProcId,
                                       VOS_INT16                   *pshwInputBuff,
                                       VOS_INT16                   *pshwOutputBuff,
                                       VOS_UINT32                  *puwOutPutSize,
                                       VOS_UINT32                   uwProcTimes);
extern VOS_INT32 XA_SRC_SetBlksMemory(xa_codec_func_t        *pfuncProcess,
                                       xa_codec_handle_t               pstApiObj,
                                       XA_COMM_MEMORY_CFG_STRU        *pstMemoryCfg,
                                       XA_SRC_USER_CFG_STRU           *pstSrcUserCfg,
                                       XA_SRC_PROC_ID_ENUM_UINT32      enProcId);
extern VOS_INT32 XA_SRC_SetPara(XA_SRC_PROC_ID_ENUM_UINT32 enProcId,
                                     XA_SRC_USER_CFG_STRU *pstSrcUserCfg);






#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of Xa_src_my.h */
