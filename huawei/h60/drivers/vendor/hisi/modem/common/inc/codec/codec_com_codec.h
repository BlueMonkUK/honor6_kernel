
#ifndef __CODEC_COM_CODEC_H__
#define __CODEC_COM_CODEC_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "codec_op_lib.h"
#include "CodecInterface.h"
#include "ImsCodecInterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 �궨��
*****************************************************************************/
extern VOS_INT16                        g_shwCodecPcmFrameLength;              /* PCM����֡���� */

#define CODEC_WB_PCM_MASK           ((VOS_INT16)0xfffC)                     /*���ڿ����16����PCM���������ȡ��14����������Ч */
#define CODEC_PCM_MASK              (0xfff8)                                /*16����PCM���������ȡ��13����������Ч */
#define CODEC_EHF_MASK              (0x0008)                                /*Homing֡����ǰ/�����̶�ֵ*/
#define CODEC_FRAME_LENGTH_NB       (160)                                   /*8K��������ÿ֡PCM������������*/
#define CODEC_FRAME_LENGTH_WB       (320)                                   /*16K��������ÿ֡PCM������������*/
#define CODEC_FRAME_LENGTH_MU       (960)                                   /*48K��������ÿ֡PCM������������*/

#define CODEC_DWORD_BITS_NUM              (32)                                    /* 4�ֽڶ�Ӧ������ */
#define CODEC_WORD_BITS_NUM               (16)                                    /* 2�ֽڶ�Ӧ������ */
#define CODEC_BYTE_BITS_NUM               (8)                                     /* 1�ֽڶ�Ӧ������ */
#define BIT0_MASK                       (0x0001)
#define BIT15_MASK                      (0x8000)
#define BIT31_MASK                      (0x80000000)
#define MIN(var1,var2)                  ((var1)>(var2)?(var2):(var1))

#define CODEC_PCM_WB_FRAME_LENGTH             ((VOS_UINT16)320)              /* �������һ֡PCM���ݵ�֡��,��λ������ */
#define CODEC_PCM_MAX_FRAME_LENGTH            (CODEC_FRAME_LENGTH_WB)    /* һ֡PCM���ݵ����֡��,���ڶ���PCM����������λ������ */

#define CODEC_PcmGetPcmFrameLength()          (g_shwCodecPcmFrameLength)
#define CODEC_PcmSetPcmFrameLength(shwPcmFrameLen) (g_shwCodecPcmFrameLength = shwPcmFrameLen)
#define CODEC_PCM_FRAME_LENGTH                 CODEC_PcmGetPcmFrameLength()
#define CODEC_PCM_FRAME_LENGTH_BYTES          (CODEC_PCM_FRAME_LENGTH * (VOS_UINT16)sizeof(VOS_INT16))

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
/*****************************************************************************
 ʵ������  : CODEC_SAMPLE_RATE_MODE_ENUM
 ��������  : �����ʶ���
*****************************************************************************/
enum CODEC_SAMPLE_RATE_MODE_ENUM
{
    CODEC_SAMPLE_RATE_MODE_8000             = 8000,
    CODEC_SAMPLE_RATE_MODE_16000            = 16000,
    CODEC_SAMPLE_RATE_MODE_BUTT
};
typedef VOS_INT32 CODEC_SAMPLE_RATE_MODE_ENUM_INT32;

/*****************************************************************************
 ʵ������  : CODEC_VAD_FLAG_ENUM
 ��������  : VAD����
*****************************************************************************/
enum CODEC_VAD_FLAG_ENUM
{
    CODEC_VAD_FLAG_NOISE              = 0,
    CODEC_VAD_FLAG_SPEECH             = 1,
    CODEC_USED_BUTT
};
typedef VOS_INT16 CODEC_VAD_FLAG_ENUM_INT16;

/* ģ���Ƿ���ʹ��ö�ٶ��� */
/*****************************************************************************
 ʵ������  : CODEC_OBJ_USED_STATUS_ENUM
 ��������  : ģ���Ƿ���ʹ�ö���
*****************************************************************************/
enum CODEC_OBJ_USED_STATUS_ENUM
{
    CODEC_OBJ_UNUSED              = 0,
    CODEC_OBJ_USED,
    CODEC_OBJ_BUTT
};
typedef VOS_UINT16  CODEC_OBJ_USED_STATUS_ENUM_UINT16;

/*****************************************************************************
 ʵ������  : CODEC_RET_ENUM
 ��������  : �ӿڷ���ֵ����
*****************************************************************************/
enum CODEC_RET_ENUM
{
    CODEC_RET_OK                    = 0,
    CODEC_RET_ERR                   = 1,
    CODEC_RET_BUTT
};
typedef VOS_UINT32 CODEC_RET_ENUM_UINT32;

/*****************************************************************************
 ʵ������  : CODEC_AMR_MODE_ENUM
 ��������  : AMR����ģʽ����
*****************************************************************************/
enum CODEC_AMR_MODE_ENUM
{
    CODEC_AMR_MODE_475              = 0,
    CODEC_AMR_MODE_515,
    CODEC_AMR_MODE_59,
    CODEC_AMR_MODE_67,
    CODEC_AMR_MODE_74,
    CODEC_AMR_MODE_795,
    CODEC_AMR_MODE_102,
    CODEC_AMR_MODE_122,
    CODEC_AMR_MODE_MRDTX,
    CODEC_AMR_MODE_BUTT
};
typedef VOS_UINT16 CODEC_AMR_MODE_ENUM_UINT16;

/*****************************************************************************
 ʵ������  : CODEC_AMRWB_MODE_ENUM
 ��������  : AMR����ģʽ����
*****************************************************************************/
enum CODEC_AMRWB_MODE_ENUM
{
    CODEC_AMRWB_MODE_660            = 0,
    CODEC_AMRWB_MODE_885,
    CODEC_AMRWB_MODE_1265,
    CODEC_AMRWB_MODE_1425,
    CODEC_AMRWB_MODE_1585,
    CODEC_AMRWB_MODE_1825,
    CODEC_AMRWB_MODE_1985,
    CODEC_AMRWB_MODE_2305,
    CODEC_AMRWB_MODE_2385,
    CODEC_AMRWB_MODE_MRDTX,
    CODEC_AMRWB_MODE_BUTT
};
typedef VOS_UINT16 CODEC_AMRWB_MODE_ENUM_UINT16;

/*****************************************************************************
 ʵ������  : CODEC_OBJ_ENUM
 ��������  : ������������/���������Ͷ���
*****************************************************************************/
enum CODEC_OBJ_ENUM
{
    CODEC_OBJ_ENCODER              = 0,
    CODEC_OBJ_DECODER,
    CODEC_OBJ_BUT
};
typedef VOS_UINT16  CODEC_OBJ_ENUM_UINT16;

/*****************************************************************************
 ʵ������  : CODEC_PREFRAME_TYPE_ENUM
 ��������  : ֮ǰ֡״̬
*****************************************************************************/
enum CODEC_PREFRAME_TYPE_ENUM
{
    CODEC_PREFRAME_NONE            = 0,                                         /* ����һ֡ */
    CODEC_PREFRAME_SPEECH,                                                      /* ����֡ */
    CODEC_PREFRAME_SID,                                                         /* SID֡ */
    CODEC_PREFRAME_BUTT
};
typedef VOS_UINT16  CODEC_PREFRAME_TYPE_ENUM_UINT16;

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
 ʵ������  : CODEC_ENC_IN_PARA_STRU
 ��������  : ������������������ṹ��
*****************************************************************************/
typedef struct
{
    CODEC_DTX_ENUM_UINT16               enDtxMode;                              /* ʹ��DTX����ģʽ */
    CODEC_AMR_FORMAT_ENUM_UINT16        enAmrFormat;                            /* ����AMR��ָʾ��ǰ��UMTS��GSM���� */
    VOS_UINT16                          enAmrMode;                              /* AMR����ʱʹ�õ�����ģʽ */
    CODEC_AMR_NSYNC_ENUM_UINT16         enAmrNsync;                             /* ���룬AMR DTXʹ��ʱָʾ�л�, ����δʵ��, ��δʹ�� */
    VOS_INT16                          *pshwEncSpeech;                          /* ָ�������������PCM����ָ�� */
}CODEC_ENC_IN_PARA_STRU;

/*****************************************************************************
 ʵ������  : CODEC_ENC_OUT_PARA_STRU
 ��������  : ������������������ṹ��
*****************************************************************************/
typedef struct
{
    CODEC_AMR_TYPE_TX_ENUM_UINT16       enAmrFrameType;                         /* GSM��AMR����������֡���� */
    CODEC_VAD_ENUM_UINT16               enVadFlag;                              /* HR/FR/EFRʹ��, Voice Active Detection flag */
    CODEC_SP_ENUM_UINT16                enSpFlag;                               /* HR/FR/EFRʹ��, Speech flag */
    VOS_UINT16                          enAmrMode;                              /* AMR/AMR WB����ʱʹ�õ�����ģʽ */
    VOS_INT16                          *pshwEncSerial;                          /* ָ����������������ָ�� */
}CODEC_ENC_OUT_PARA_STRU;

/*****************************************************************************
 ʵ������  : CODEC_DEC_IN_PARA_STRU
 ��������  : ������������������ṹ��
*****************************************************************************/
typedef struct
{
    CODEC_AMR_FORMAT_ENUM_UINT16        enAmrFormat;                            /* ָʾ��ǰ��UMTS��GSM���� */
    VOS_UINT16                          enAmrMode;                              /* AMR����ʱʹ�õ�����ģʽ */
    CODEC_AMR_TYPE_RX_ENUM_UINT16       enAmrFrameType;                         /* GSM��AMR����֡���� */
    CODEC_BFI_ENUM_UINT16               enBfiFlag;                              /* HR/FR/EFRʹ��, Bad Frame Indication */
    CODEC_SID_ENUM_UINT16               enSidFlag;                              /* HR/FR/EFRʹ��, Silence Descriptor flag */
    CODEC_TAF_ENUM_UINT16               enTafFlag;                              /* HR/FR/EFRʹ��, Time Alignment flag  */
    CODEC_BFI_ENUM_UINT16               enHrUfiFlag;                            /* HRʹ��, Unreliable Frame Indication */
    VOS_UINT16                          uhwFrameFacchCnt;                       /* �����յ���FA֡���� */
    VOS_INT16                           shwFrameRangeMax;                       /* ʱ��������ֵ */
    VOS_INT16                           shwReserved;
    VOS_INT16                          *pshwDecSerial;                          /* ָ���ŵ������������������������������ָ��*/
    CODEC_PREFRAME_TYPE_ENUM_UINT16     enGoodFrameType;                        /* ��֡����:0-����һ֡ 1-����֡ 2-SID֡ */
    VOS_UINT16                          uhwFrameBfiCnt;                         /* �����յ���BFI֡���� */
    VOS_UINT16                          uhwRxTypeForAjb;                        /* ֡���ͣ�����AJB����AJBʱ���ù��� */
    VOS_UINT16                          uhwReserve;
}CODEC_DEC_IN_PARA_STRU;

/*****************************************************************************
 ʵ������  : CODEC_DEC_PREIN_PARA_STRU
 ��������  : ����������ǰһ֡��������ṹ��
*****************************************************************************/
typedef struct
{
    CODEC_AMR_FORMAT_ENUM_UINT16        enAmrFormat;                            /* ָʾ��ǰ��UMTS��GSM���� */
    VOS_UINT16                          enAmrMode;                              /* AMR����ʱʹ�õ�����ģʽ */
    CODEC_AMR_TYPE_RX_ENUM_UINT16       enAmrFrameType;                         /* GSM��AMR����֡���� */
    CODEC_BFI_ENUM_UINT16               enBfiFlag;                              /* HR/FR/EFRʹ��, Bad Frame Indication */
    CODEC_SID_ENUM_UINT16               enSidFlag;                              /* HR/FR/EFRʹ��, Silence Descriptor flag */
    CODEC_TAF_ENUM_UINT16               enTafFlag;                              /* HR/FR/EFRʹ��, Time Alignment flag  */
    CODEC_BFI_ENUM_UINT16               enHrUfiFlag;                            /* HRʹ��, Unreliable Frame Indication */
    VOS_UINT16                          uhwFrameFacchCnt;                       /* �����յ���FA֡���� */
}CODEC_DEC_PREIN_PARA_STRU;
/*****************************************************************************
 ʵ������  : IMS_DATA_OBJ_STRU
 ��������  : Decode Buffer��ŵ�IMS�������ݸ�ʽ
*****************************************************************************/
typedef struct
{
    unsigned short                      usRateMode;                             /* ���б����RateMode */
    unsigned short                      usErrorFlag;                            /* Bufferû����ʱ��ErrorFlag */
    unsigned short                      usFrameType;                            /* ���б�֡��FrameType */
    unsigned short                      usQualityIdx;                           /* Quality indification */
    unsigned short                      ausData[IMSA_CODEC_MAX_DATA_LEN];       /* IF1Э���е�A��B��C������ABC�������У�Byte���� */
}IMS_DL_DATA_OBJ_STRU;

/*****************************************************************************
 ʵ������  : IMS_DATA_OBJ_STRU
 ��������  : Encode Buffer��ŵ�IMS�������ݸ�ʽ,��ImsCodecInterface��Ӧ
*****************************************************************************/
typedef struct
{
    CODEC_AMR_TYPE_TX_ENUM_UINT16       usStatus;
    VOS_UINT16                          usFrameType;                            /* ���б�֡��FrameType */
    VOS_UINT16                          ausData[IMSA_CODEC_MAX_DATA_LEN];       /* IF1Э���е�A��B��C������ABC�������У�Byte���� */
}IMS_UL_DATA_OBJ_STRU;
/*****************************************************************************
  7 UNION����
*****************************************************************************/


/*****************************************************************************
  8 OTHERS����
*****************************************************************************/
/* ֡���ͻص����� */
typedef VOS_INT16 (*CODEC_FRAMETYPE_CALLBACK)(VOS_UINT16);

/*****************************************************************************
  9 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/

/*****************************************************************************
 �� �� ��  : AMR_Dec_Init
 ��������  : AMR��ʼ������
 �������  : ��
 �������  : ��
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 AMR_Dec_Init(VOS_VOID);

/*****************************************************************************
 �� �� ��  : AMR_Enc_Init
 ��������  : AMR�����ʼ������
 �������  : enDtxMode         --ʹ��DTX����ģʽ, 1Ϊʹ��, 0Ϊ����
             pfCodecCallBack   --֡���ͻص�����
 �������  : ��
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 AMR_Enc_Init(
                       CODEC_DTX_ENUM_UINT16 enDtxMode,
                       VOS_VOID                 *pfCodecCallBack);

/*****************************************************************************
 �� �� ��  : AMR_Encode
 ��������  : AMR����
 �������  : pstEncInPara      --  ָ��������������������ṹ���ָ��
 �������  : pstEncOutPara     --  ָ��������������ṹ���ָ��
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 AMR_Encode(
                CODEC_ENC_IN_PARA_STRU  *pstEncInPara,
                CODEC_ENC_OUT_PARA_STRU *pstEncOutPara);

/*****************************************************************************
 �� �� ��  : AMR_Decode
 ��������  : AMR����
 �������  : pstDecPara     --  ָ������������ṹ���ָ��
 �������  : pshwDecSpeech  --  ��������,����Ϊ160���ֽڵ�PCM����
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 AMR_Decode(
                CODEC_DEC_IN_PARA_STRU *pstDecPara,
                VOS_INT16                  *pshwDecSpeech);

/*****************************************************************************
 �� �� ��  : AMRWB_Dec_Init
 ��������  : AMRWB�����ʼ������
 �������  : ��
 �������  : ��
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 AMRWB_Dec_Init(VOS_VOID);

/*****************************************************************************
 �� �� ��  : AMRWB_Enc_Init
 ��������  : AMRWB�����ʼ������
 �������  : enDtxMode         --ʹ��DTX����ģʽ, 1Ϊʹ��, 0Ϊ����
             pfCodecCallBack   --֡���ͻص�����
 �������  : ��
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 AMRWB_Enc_Init(
                CODEC_DTX_ENUM_UINT16  enDtxMode,
                VOS_VOID                  *pfCodecCallBack);

/*****************************************************************************
 �� �� ��  : AMRWB_Encode
 ��������  : AMRWB����
 �������  : pstEncInPara      --  ָ��������������������ṹ���ָ��
 �������  : pstEncOutPara     --  ָ��������������ṹ���ָ��
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 AMRWB_Encode(
                CODEC_ENC_IN_PARA_STRU  *pstEncInPara,
                CODEC_ENC_OUT_PARA_STRU *pstEncOutPara);

/*****************************************************************************
 �� �� ��  : AMRv_Decode
 ��������  : AMRWB����
 �������  : pstDecPara     --  ָ������������ṹ���ָ��
 �������  : pshwDecSpeech  --  ��������,����Ϊ160���ֽڵ�PCM����
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 AMRWB_Decode(
                CODEC_DEC_IN_PARA_STRU *pstDecPara,
                VOS_INT16                  *pshwDecSpeech);

/*****************************************************************************
 �� �� ��  : EFR_Dec_Init
 ��������  : EFR�����ʼ������
 �������  : ��
 �������  : ��
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 EFR_Dec_Init(VOS_VOID);

/*****************************************************************************
 �� �� ��  : EFR_Enc_Init
 ��������  : EFR��ʼ������
 �������  : enDtxMode  --��ʾʹ��DTX����ģʽ��1Ϊʹ�ܣ�0Ϊ����
 �������  : ��
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 EFR_Enc_Init(CODEC_DTX_ENUM_UINT16 enDtxMode);

/*****************************************************************************
 �� �� ��  : EFR_Encode
 ��������  : EFR���뺯��
 �������  : pstEncInPara      --ָ��������������������ṹ���ָ��
 �������  : pstEncOutPara     --ָ��������������ṹ���ָ��
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 EFR_Encode(
                CODEC_ENC_IN_PARA_STRU  *pstEncInPara,
                CODEC_ENC_OUT_PARA_STRU *pstEncOutPara);

/*****************************************************************************
 �� �� ��  : EFR_Decode
 ��������  : EFR���뺯��
 �������  : pstDecPara     --ָ������������ṹ���ָ��
 �������  : pshwDecSpeech  --�������ݣ�����Ϊ160���ֽڵ�PCM����
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 EFR_Decode(
                CODEC_DEC_IN_PARA_STRU *pstDecPara,
                VOS_INT16                  *pshwDecSpeech);

/*****************************************************************************
 �� �� ��  : FR_Dec_Init
 ��������  : FR�����ʼ������
 �������  : ��
 �������  : ��
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 FR_Dec_Init(VOS_VOID);

/*****************************************************************************
 �� �� ��  : FR_Enc_Init
 ��������  : FR�����ʼ������
 �������  : enDtxMode ��ʾʹ��DTX����ģʽ��1Ϊʹ�ܣ�0Ϊ����
 �������  : ��
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 FR_Enc_Init(CODEC_DTX_ENUM_UINT16 enDtxMode);

/*****************************************************************************
 �� �� ��  : FR_Encode
 ��������  : ���һ֡160���������ݵ�FR�������
 �������  : pstEncInPara      --  ָ��������������������ṹ���ָ��
 �������  : pstEncOutPara     --  ָ��������������ṹ���ָ��
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 FR_Encode(
                CODEC_ENC_IN_PARA_STRU  *pstEncInPara,
                CODEC_ENC_OUT_PARA_STRU *pstEncOutPara);

/*****************************************************************************
 �� �� ��  : FR_Decode
 ��������  : FR���뺯��
 �������  : pstVocodecDecPara --ָ������������ṹ���ָ��
 �������  : pshwDecSpeech     --��������ݣ�����Ϊ160���ֽڵ�PCM����
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 FR_Decode(
                CODEC_DEC_IN_PARA_STRU  *pstVocodecDecPara,
                VOS_INT16 *pshwDecSpeech);

/*****************************************************************************
 �� �� ��  : HR_Dec_Init
 ��������  : HR�����ʼ������
 �������  : ��
 �������  : ��
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 HR_Dec_Init(VOS_VOID);

/*****************************************************************************
 �� �� ��  : HR_Enc_Init
 ��������  : HR�����ʼ������
 �������  : enDtxMode ��ʾʹ��DTX����ģʽ��1Ϊʹ�ܣ�0Ϊ����
 �������  : ��
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/

extern VOS_UINT32 HR_Enc_Init(CODEC_DTX_ENUM_UINT16 enDtxMode);

/*****************************************************************************
 �� �� ��  : HR_Encode
 ��������  : ���һ֡160���������ݵ�HR�������
 �������  : pstEncInPara      --  ָ��������������������ṹ���ָ��
 �������  : pstEncOutPara     --  ָ��������������ṹ���ָ��
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 HR_Encode(
                CODEC_ENC_IN_PARA_STRU  *pstEncInPara,
                CODEC_ENC_OUT_PARA_STRU  *pstEncOutPara);

/*****************************************************************************
 �� �� ��  : HR_Decode
 ��������  : HR���뺯��
 �������  : pstVocodecDecPara -- ָ������������ṹ���ָ��
 �������  : pshwDecSpeech     -- ��������ݣ�����Ϊ160���ֽڵ�PCM����
 �� �� ֵ  : �ɹ�����CODEC_RET_OK
             ʧ�ܷ���CODEC_RET_ERR
*****************************************************************************/
extern VOS_UINT32 HR_Decode(
                CODEC_DEC_IN_PARA_STRU  *pstVocodecDecPara,
                VOS_INT16                   *pshwDecSpeech);

/*****************************************************************************
 �� �� ��  : CODEC_ComCodecBits2Prm
 ��������  : ����Ӧ������ת��Ϊ����
 �������  : VOS_INT16 *pshwBitStream      --������ָ��
             VOS_INT16 shwNumOfBits        --��ת�����ظ���
 �������  : VOS_INT16 *pshwPrmValue       --ת����õĲ���ָ��
 �� �� ֵ  : VOS_VOID
*****************************************************************************/
extern VOS_VOID CODEC_ComCodecBits2Prm(
                VOS_INT16 *pshwBitStream,
                VOS_INT16 shwNumOfBits,
                VOS_INT16 *pshwPrmValue);

/*****************************************************************************
 �� �� ��  : CODEC_ComCodecPrm2Bits
 ��������  : ������ת��Ϊ��Ӧ���أ�ÿ������ռһ��word
 �������  : VOS_INT16 shwValue         --��Ҫת���Ĳ���ֵ
             VOS_INT16 shwNumOfBits     --���������ı�����
 �������  : VOS_INT16 *pshwBitStream   --ת����ı�����
 �� �� ֵ  : VOS_VOID
*****************************************************************************/
extern VOS_VOID CODEC_ComCodecPrm2Bits(
                VOS_INT16 shwValue,
                VOS_INT16 shwNumOfBits,
                VOS_INT16 *pshwBitStream);

/*****************************************************************************
 �� �� ��  : CODEC_ComCodecBits2Bytes
 ��������  : ���ǽ�����������еı�������ʽת��Ϊ�������е�������ʽ
 �������  : VOS_INT16 *pshwBits       --����ǽ�����������еı�����ָ��
             VOS_INT16  shwBitsNum     --��Ҫת���ı�����
 �������  : VOS_INT8  *pscBytes       --����������е�����ָ��
 �� �� ֵ  : VOS_VOID
*****************************************************************************/
VOS_VOID CODEC_ComCodecBits2Bytes(
                VOS_INT16 *pshwBits,
                VOS_INT16  shwBitsNum,
                VOS_INT8  *pscBytes);


/*****************************************************************************
 �� �� ��  : CODEC_ComCodecBytes2Bits
 ��������  : ���������е�������ʽת��Ϊ�ǽ�����������еı�������ʽ
 �������  : VOS_INT8  *pscBytes      --����������е�����ָ��
             VOS_INT16  shwBitsNum    --��Ҫת���ı�����
 �������  : VOS_INT16 *pshwBits      --ת����ǽ��ձ������еı�����ָ��
 �� �� ֵ  : VOS_VOID
*****************************************************************************/
VOS_VOID CODEC_ComCodecBytes2Bits(
                VOS_INT8  *pscBytes,
                VOS_INT16  shwBitsNum,
                VOS_INT16 *pshwBits);




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* end of codec_com_codec.h */

