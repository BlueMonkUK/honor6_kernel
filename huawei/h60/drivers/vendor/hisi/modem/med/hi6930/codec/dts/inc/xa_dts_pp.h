

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/

#ifndef __XA_DTS_PP_H__
#define __XA_DTS_PP_H__

#include "ucom_comm.h"
#include "srs_trmediahd_api.h"
#include "hifidrvinterface.h"
#include "ucom_nv.h"



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define XA_DTS_DEFAULT_ORDER_HPF                    (6)                         /* default order hpf */
#define XA_DTS_ORDER_HPF1                           (4)                         /* the order of the first HPF */
#define XA_DTS_ORDER_HPF2                           (4)                         /* the order of the second HPF */
#define XA_DTS_BANDS_PEQ                            (8)                         /* the number of PEQ bands */
#define XA_DTS_DEFAULT_BANDS_PEQ                    (8)                         /* the deault number of PEQ bands */
#define XA_DTS_CHANNEL_NUM                          (2)                         /* DTS��ǰ���������� */
#define XA_DTS_DEFAULT_GEQ_BAND_GAIN                (4096)                      /* GEQ Ĭ��ȱʡ���� */
#define XA_DTS_GEQ_BAND_NUM                         (10)                        /* GEQ�Ӵ����� */
#define XA_DTS_TBHD_PARA_LEN                        (24)
#define XA_DTS_HPF_PARA_LEN                         (20)
#define XA_DTS_PEQ_PARA_LEN                         (26)
#define XA_DTS_GetCurrentCfgPtr()                   (&g_stDtsCurtCfg)           /* ��ȡ��ǰDTSģ��������Ϣ */
#define XA_DTS_GetUinitDDRParaPtr()                 (&g_stDtsUintPara)          /* ��ȡ��ǰDTSʹ����������Ϣ */
#define XA_DTS_GetCurrentParaPtr()                  (&g_stDtsCurrentPara)       /* ��ȡ��ǰDTSʹ�õ����ò��� */
/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
 ʵ������  : XA_DTS_HPF_ENUM_UINT16
 ��������  : HPF����ö��
*****************************************************************************/
enum
{
    XA_DTS_HPF_DEFAULTCOEFS = 0,                                                /* ȱʡֵ */
    XA_DTS_HPF_TESTCOEFS,                                                       /* ���� */
    XA_DTS_HPF_BUTT
};
typedef VOS_UINT16 XA_DTS_HPF_ENUM_UINT16;

/*****************************************************************************
 ʵ������  : XA_DTS_FILTER_CFG_ENUM_UINT16
 ��������  : �˲�����������ö��
*****************************************************************************/
enum
{
    XA_DTS_FILTER_CFG_DEFAULT = 0,                                              /* ȱʡֵ */
    XA_DTS_FILTER_CFG_NON_FLAT_PEQ,                                             /* Any number of different filter configurations can be used  */
    XA_DTS_FILTER_CFG_BUTT
};
typedef VOS_UINT16 XA_DTS_FILTER_CFG_ENUM_UINT16;

/*****************************************************************************
 ʵ������  : XA_DTS_USE_PARA_ENUM_UINT16
 ��������  : DTSʹ�ò���������ö��
*****************************************************************************/
enum
{
    XA_DTS_USE_DEFAULT_PARA = 0,                                                /* ʹ��Ĭ�ϵĲ�������û������DTS��Ч */
    XA_DTS_USE_ENABLE_PARA,                                                     /* ʹ��DTSʹ�ܺ�Ĳ����� */
    XA_DTS_USE_PARA_BUTT
};
typedef VOS_UINT16 XA_DTS_USE_PARA_ENUM_UINT16;

/*****************************************************************************
 ʵ������  : XA_DTS_NEED_SET_PARA_ENUM_UINT16
 ��������  : DTS�Ƿ���Ҫ���ò�������ö��
*****************************************************************************/
enum
{
    XA_DTS_NEED_SET_PARA_NO = 0,                                                /* ��ǰ����Ҫ����DTS���� */
    XA_DTS_NEED_SET_PARA_YES,                                                   /* ��ǰ��Ҫ����DTS���� */
    XA_DTS_NEED_SET_PARA_BUTT
};
typedef VOS_UINT16 XA_DTS_NEED_SET_PARA_ENUM_UINT16;

/*****************************************************************************
 ʵ������  : XA_DTS_CURRENT_DEV_ENUM_UINT32
 ��������  : DTS��ǰ�����������ö��
*****************************************************************************/
enum
{
    XA_DTS_DEV_SPEAKRE = 0,                                                     /* ��ǰʹ��speaker(���)��Ϊ����豸 */
    XA_DTS_DEV_HEADSET,                                                         /* ��ǰʹ��headset(����)��Ϊ����豸 */
    XA_DTS_DEV_BUTT
};
typedef VOS_UINT32 XA_DTS_CURRENT_DEV_ENUM_UINT32;

/*****************************************************************************
 ʵ������  : XA_DTS_ENABLE_ENUM_UINT16
 ��������  : DTS�Ƿ�ʹ��ö��
*****************************************************************************/
enum
{
    XA_DTS_ENABLE_NO = 0,                                                       /* ��ʹ��DTS��Ч */
    XA_DTS_ENABLE_YES,                                                          /* ʹ��DTS��Ч */
    XA_DTS_ENABLE_BUTT
};
typedef VOS_UINT16 XA_DTS_ENABLE_ENUM_UINT16;

/*****************************************************************************
  4 ��Ϣͷ����
*****************************************************************************/


/*****************************************************************************
  5 ��Ϣ����
*****************************************************************************/
/*****************************************************************************
 ʵ������  : XA_DTS_USER_CFG_STRU
 ��������  : DTS�����ṩ��������Ϣ�ṹ��
*****************************************************************************/
typedef struct
{
    VOS_INT16          *pshwInputBuff;                                          /* ���������ݣ�Ĭ��Ϊ������������洢 */
    VOS_INT16          *pshwOutputBuff;                                         /* ������ɺ�������� */
    VOS_UINT32          uwBlockSize;                                            /* һ�δ���������� */
    VOS_UINT32          uwFilterCfg;                                            /* �˲������ã�0Ϊȱʡ���� */
    VOS_UINT32          uwSampleRate;                                           /* �������ݵĲ�����,�����ṩ����ʽ����480000 */
    VOS_INT32           swChannelNum;                                           /* �������ݵ������� */
} XA_DTS_USER_CFG_STRU;

/*****************************************************************************
 ʵ������  : XA_DTS_CURRENT_CFG_STRU
 ��������  : DTS��ǰʹ�õ�������Ϣ�ṹ��
*****************************************************************************/
typedef struct
{
    VOS_UINT32                       uwBlockSize;                               /* һ�δ���������� */
    VOS_UINT32                       uwFilterCfg;                               /* �˲������ã�0Ϊȱʡ���� */
    VOS_UINT32                       uwSampleRate;                              /* �������ݵĲ�����,DTSģ��ά����ʽ����48/K */
    VOS_INT32                        swChannelNum;                              /* �������ݵ������� */

    VOS_CHAR                        *pDtsObjBuff;                               /* DTS�����ڴ� */
    VOS_UINT32                       uwDtsObjSize;                              /* DTS�����ڴ��С */
    SRSTrMediaHDObj                  stDtsObj;                                  /* DTS���� */
    VOS_VOID                        *pvWorkSpaceBuff;                           /* WorkSpaceBuff */
    VOS_UINT32                       uwWorkSpaceSize;                           /* workspace�ڴ��С */
    VOS_VOID                        *pvFFTLibBuff;                              /* FFTlib���С */
    VOS_INT32                       *pswLeftBuff;                               /* �������洢�ռ� */
    VOS_UINT32                       uwLeftSize;                                /* �������ڴ��С */
    VOS_INT32                       *pswRightBuff;                              /* �������洢�ռ� */
    VOS_UINT32                       uwRightSize;                               /* �������ڴ��С */
} XA_DTS_CURRENT_CFG_STRU;

/*****************************************************************************
 ʵ������  : XA_DTS_ALL_PARA_STRU
 ��������  : DTS��ǰʹ�õ�����������Ϣ�ṹ��
*****************************************************************************/
typedef struct
{
    SRSTrMediaHDControls            stDtsCtrl;                                  /* DTS���Ʋ��� */
    VOS_INT16                       ashwGeqBandGain[XA_DTS_GEQ_BAND_NUM];       /* Geq�Ӵ����� */

} XA_DTS_ALL_PARA_STRU;

/*****************************************************************************
 ʵ������  : XA_DTS_SET_PARA_STRU
 ��������  : DTS�Ƿ�ʹ�ܡ�ʹ�õ�����豸���ɷ��͸�DTS����Ϣά��
*****************************************************************************/
typedef struct
{
    XA_DTS_NEED_SET_PARA_ENUM_UINT16            enIsNeedSetPara;                /* ��ǰDTS�Ƿ���Ҫ���ò��� */
    XA_DTS_ENABLE_ENUM_UINT16                   enIsEnable;                     /* �Ƿ�ʹ��DTS */
    XA_DTS_CURRENT_DEV_ENUM_UINT32              enCurrentDev;                   /* ��ǰʹ�õ�����豸 */
} XA_DTS_SET_PARA_STRU;

/*****************************************************************************
 ʵ������  : AUDIOPLAYER_DTS_SET_ENABLE_CMD_STRU
 ��������  : AUDIOPLAYER֪ͨDTS����DTS��Чʹ�����
*****************************************************************************/
typedef struct{
    VOS_MSG_HEADER
    unsigned short                      uhwMsgId;                               /* ����DTS��Чʹ�����ID */
    unsigned short                      uhwEnableFlag;                          /* DTS��Ч�Ƿ�ʹ�� */
}AUDIO_PLAYER_DTS_SET_ENABLE_CMD_STRU;

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
/* device���ò��� */
extern  SRSTrMediaHDControls DTS_speaker;
extern  SRSTrMediaHDControls DTS_headphone;
extern  SRSTrMediaHDHpfOrder HPF_Order;

/* device�˲����������� */
extern SRSInt32 Untitled_TBHD1[XA_DTS_TBHD_PARA_LEN];
extern SRSInt32 Untitled_TBHD2[XA_DTS_TBHD_PARA_LEN];

extern SRSInt16 Untitled_HPF1[XA_DTS_HPF_PARA_LEN];
extern SRSInt16 Untitled_HPF2[XA_DTS_HPF_PARA_LEN];

extern SRSInt32 Untitled_PEQ1[XA_DTS_PEQ_PARA_LEN];
extern SRSInt32 Untitled_PEQ2[XA_DTS_PEQ_PARA_LEN];

/*****************************************************************************
  10 ��������
*****************************************************************************/
extern VOS_INT32  XA_DTS_CreatObj(XA_DTS_USER_CFG_STRU * pstUserCfg);
extern VOS_UINT32 XA_DTS_DeleObj( VOS_VOID );
extern VOS_INT32  XA_DTS_ExeProcess(XA_DTS_USER_CFG_STRU * pstUserCfg);
extern VOS_INT32 XA_DTS_Hybrid2Stereo(
                       VOS_INT16 *pshwI2S,
                       VOS_INT32 *pswLeft,
                       VOS_INT32 *pswRight,
                       VOS_INT32  swBlockSize);
extern VOS_UINT32 XA_DTS_Init( XA_DTS_USER_CFG_STRU * pstUserCfg );
extern VOS_VOID  XA_DTS_InitFFT_Lib(VOS_VOID);
extern VOS_VOID XA_DTS_InitObj(SRSTrMediaHDObj stTmhdObj, XA_DTS_FILTER_CFG_ENUM_UINT16 uhwFset, VOS_UINT32 uwSampleRate);
extern VOS_UINT32  XA_DTS_MsgSetDtsDevCmd( VOS_VOID *pvOsaMsg );
extern VOS_UINT32 XA_DTS_MsgSetDtsEnableCmd (VOS_VOID *pvOsaMsg);
extern VOS_UINT32  XA_DTS_SetDev( XA_DTS_CURRENT_DEV_ENUM_UINT32 enDevice );
extern VOS_UINT32  XA_DTS_SetEnable(XA_DTS_ENABLE_ENUM_UINT16 enEnableFlag);
extern VOS_INT32 XA_DTS_SetGEQBandGains(SRSTrMediaHDObj stDtsObj, VOS_INT16 * pshwGeqGain);
extern VOS_UINT32 XA_DTS_Stereo2Hybrid(
                       VOS_INT32  *pswLeft,
                       VOS_INT32  *pswRight,
                       VOS_INT32   swBlockSize,
                       VOS_INT16  *shwHybrid);


/* DTS lib���ṩԴ�ļ��������� */
extern SRSTrMediaHDFilterConfig  GetFilterNonflatPeqCfg(int sampleRate, unsigned int device);

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of xa_dts_pp.h */

