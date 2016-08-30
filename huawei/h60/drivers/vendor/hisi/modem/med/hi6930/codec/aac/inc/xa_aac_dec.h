/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : xa_aac_dec.h
  �� �� ��   : ����
  ��    ��   :
  ��������   : 2012��12��20��
  ����޸�   :
  ��������   : xa_aac_dec.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   :
    ��    ��   :
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "ucom_comm.h"
#include  "xa_aac_dec_api.h"
#include  "xa_comm.h"
#ifndef __XA_AAC_DEC_H__
#define __XA_AAC_DEC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define XA_AACDEC_SET_MONO2STEREO_ENABLE  (1)
#define XA_AACDEC_SET_PCMWIDTH_16         (16)
#define XA_AACDEC_SET_MAXOUTCHAN_2        (2)

#define XA_AACDEC_MAXCHAN_NUMBER          (8)
#define XA_AACDEC_CHANNELMAP_SHIFT_SCALE  (4)

#define XA_AACDEC_SBR_TYPE_1              (1)
#define XA_AACDEC_SBR_TYPE_2              (2)

#define XA_AACDEC_GetMemCfgPtr()          (&g_stAacDecMemCfg)
#define XA_AACDEC_GetParaCfgPtr()         (&g_stAacDecParamCfg)
#define XA_AACDEC_GetParamsPtr()          (&g_stAacParams)

#define XA_AACDEC_GetApiObjPtr()          (g_pstAacApiObj)


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
/*****************************************************************************
 ʵ������  : AAC�������������ʽ����������ADIF��ADTS
 ��������  : AAC�������������ʽ��Tensilica Lib��ֻ֧�ֲ��ָ�ʽ
*****************************************************************************/
enum
{
    XA_AACDEC_EBITSTREAM_TYPE_AAC_UNKNOW = 0,
    XA_AACDEC_EBITSTREAM_TYPE_AAC_ADIF,
    XA_AACDEC_EBITSTREAM_TYPE_AAC_ADTS,
    XA_AACDEC_EBITSTREAM_TYPE_AAC_LATM,                                         /* Tensilica Lib Not supported */
    XA_AACDEC_EBITSTREAM_TYPE_AAC_LATM_OUTOFBAND_CONFIG,                        /* Tensilica Lib Not supported */
    XA_AACDEC_EBITSTREAM_TYPE_AAC_LOAS,                                         /* Supported if library is built with LOAS_SUPPORT=1 */
    XA_AACDEC_EBITSTREAM_TYPE_AAC_RAW,
    XA_AACDEC_EBITSTREAM_TYPE_DABPLUS_RAW_SIDEINFO,                             /* Supported only if short side-info
                                                                                (format: Bit15 = badFramIndicator & Bit 0-14 = payloadSizeInBytes)
                                                                                is provided as config parameter for every raw-input frame */
    XA_AACDEC_EBITSTREAM_TYPE_DABPLUS,
    XA_AACDEC_EBITSTREAM_TYPE_BUTT
};
typedef VOS_INT16 XA_AACDEC_EBITSTREAM_TYPE_INT16;

/*****************************************************************************
 ʵ������  : AAC��ʽ
 ��������  : AAC LIB�İ汾��ʽ��Ϣ
*****************************************************************************/
enum
{
    XA_AACDEC_EBITSTREAM_FORMAT_PLAIN_AAC = 0,
    XA_AACDEC_EBITSTREAM_FORMAT_AAC_PLUS,
    XA_AACDEC_EBITSTREAM_FORMAT_AAC_PLUS_V2,
    XA_AACDEC_EBITSTREAM_FORMAT_BUTT
};
typedef VOS_INT16 XA_AACDEC_FORMAT_TYPE_INT16;

/*****************************************************************************
 ʵ������  : ����������������ʽ
 ��������  : ȷ��������������������ʽ
*****************************************************************************/
enum
{
    XA_AACDEC_CHANNELMODE_MONO = 0,
    XA_AACDEC_CHANNELMODE_STEREO,
    XA_AACDEC_CHANNELMODE_DUAL_CHANNEL,
    XA_AACDEC_CHANNELMODE_PARAMETRIC_STEREO,
    XA_AACDEC_CHANNELMODE_3_CHANNEL_FRONT,
    XA_AACDEC_CHANNELMODE_3_CHANNEL_SURR,
    XA_AACDEC_CHANNELMODE_4_CHANNEL_1SURR,
    XA_AACDEC_CHANNELMODE_4_CHANNEL_2SURR,
    XA_AACDEC_CHANNELMODE_6_CHANNEL,
    XA_AACDEC_CHANNELMODE_7_CHANNEL,
    XA_AACDEC_CHANNELMODE_5_CHANNEL,
    XA_AACDEC_CHANNELMODE_2_1_STEREO,
    XA_AACDEC_CHANNELMODE_3_1_CHANNEL_FRONT,
    XA_AACDEC_CHANNELMODE_3_1_CHANNEL_SURR,
    XA_AACDEC_CHANNELMODE_4_1_CHANNEL_1SURR,
    XA_AACDEC_CHANNELMODE_4_1_CHANNEL_2SURR,
    XA_AACDEC_CHANNELMODE_6_1_CHANNEL,
    XA_AACDEC_CHANNELMODE_5_1_CHANNEL,
    XA_AACDEC_CHANNELMODE_7_1_CHANNEL,
    XA_AACDEC_CHANNELMODE_BUTT
};
typedef VOS_INT16 XA_AACDEC_CHANNELMODE_INT16;

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
 ʵ������  : AAC_DEC_CONFIG_PARAMS_STRU
 ��������  : AAC��������������ò����ṹ��
*****************************************************************************/
typedef struct
{
    VOS_INT32 swPcmWidthSize;      /* ����λ�� */
    VOS_INT32 swMaxOutChanNum;     /* ������������ */
    VOS_INT32 swForceOutputStereo; /* ������ǿ�����˫���� */
}AAC_DEC_CONFIG_PARAMS_STRU;

/*****************************************************************************
 ʵ������  : AAC_DEC_PARAMS_STRU
 ��������  : AAC�����������������ṹ��
*****************************************************************************/
typedef struct
{
    XA_AACDEC_FORMAT_TYPE_INT16       enAacLibFormat;/* AAC���ʽ */
    XA_AACDEC_EBITSTREAM_TYPE_INT16   enAacFormat;   /* AAC�����ʽ */
    VOS_INT32                         swNumChannels; /* ������ */
    XA_AACDEC_CHANNELMODE_INT16       enAcmod;       /* ������ģʽ */
    VOS_INT16                         shwReserved;
    VOS_INT32                         swSampFreq;    /* ������ */
    VOS_INT32                         swSbrType;     /* SBR���� */
    VOS_INT32                         swAacRate;     /* AAC�������� */
    VOS_INT32                         swChanMap;     /* ͨ��ӳ��� */
    VOS_INT32                         swOutNChans;   /* ������������������ */
}AAC_DEC_PARAMS_STRU;


/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/


/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/
extern XA_COMM_MEMORY_CFG_STRU                   g_stAacDecMemCfg;       /* AAC�����ڴ����ù��� */
extern AAC_DEC_CONFIG_PARAMS_STRU                g_stAacDecParamCfg;     /* AAC����������ù��� */
extern AAC_DEC_PARAMS_STRU                       g_stAacParams;          /* AAC�������� */

/* AAC�����APIָ�� */
extern xa_codec_handle_t                         g_pstAacApiObj;


/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_VOID AAC_DECODER_Close(VOS_VOID);
extern VOS_UINT32 AAC_DECODER_Decode(XA_AUDIO_DEC_USER_CFG_STRU *pstUserCfg);
extern VOS_INT32 AAC_DECODER_GetCurrDecPos(VOS_UINT32 *puwPosInBytes);
extern VOS_UINT32  AAC_DECODER_GetParams(AAC_DEC_PARAMS_STRU *pstConfig);
extern VOS_UINT32  AAC_DECODER_Init(XA_AUDIO_DEC_USER_CFG_STRU *pstUserCfg);
extern VOS_UINT32 AAC_DECODER_PcmDataConvert
                       (VOS_INT32       *pswTimeCh,
                        VOS_INT32        swPcmSampleSize,
                        VOS_INT32        swOutputSize,       /* AUDIO_DEC_MEMORY_CFG_STRU�е���ʱ���Buff�ĳ��� */
                        VOS_INT16       *pshwOutBuff,
                        VOS_INT32       *pswTotalOutSize);
extern VOS_UINT32  AAC_DECODER_Seek(
                       VOS_UINT32                    uwSkipSize,
                       CODEC_SEEK_DERECT_ENUM_UINT16 enSeekDirect);
extern VOS_UINT32  AAC_DECODER_SetParams(AAC_DEC_CONFIG_PARAMS_STRU *pstParamCfg);
//extern VOS_VOID AAC_DECODER_SetOutChanFixedTo2(VOS_INT32 *pswChanMap);


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of xa_aac_dec.h */


