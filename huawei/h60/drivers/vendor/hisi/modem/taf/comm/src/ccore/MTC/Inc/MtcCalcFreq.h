

#ifndef __MTC_CALC_FREQ_H__
#define __MTC_CALC_FREQ_H__

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"
#include "PsTypeDef.h"

#if (FEATURE_ON == FEATURE_LTE)
#include "LNvCommon.h"
#endif

#include "MtcRrcInterface.h"
#include "MtcRfLcdIntrusion.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define DC_MIMO_REMOVEINTERFREQOFF                  ( 0xbfff )                  /* ȥ��ƫ��ֵ */
#define FREQ_BAND_NUM                               ( 10 )                               /* Ŀǰ֧�ֵ�Ƶ����Ŀ */

#if (FEATURE_ON == FEATURE_LTE)
/* ����Ƿ�ΪFDD��band ind */
#define MTC_COMM_CHECK_IS_FDD_BAND_IND(ucBandInd)\
    ((((MTC_FREQ_BAND1 <= ucBandInd) && (MTC_FREQ_BAND32 > ucBandInd)))? VOS_TRUE:VOS_FALSE)

/* ����Ƿ�ΪTDD��band ind */
#define MTC_COMM_CHECK_IS_PROCOL_TDD_BAND_IND(ucBandInd)\
    (((MTC_FREQ_BAND33 <= ucBandInd) && (MTC_FREQ_BAND43 >= ucBandInd))? VOS_TRUE:VOS_FALSE)

/* �Ǳ�׼Ƶ�� begin */
#define MTC_COMM_CHECK_IS_NONSTANDARD_BAND_IND(ucBandInd)\
    ((((LTE_COMM_NONSTANDARD_BAND_BEGIN  <= ucBandInd) && (LTE_COMM_NONSTANDARD_BAND_END >= ucBandInd)) )? VOS_TRUE:VOS_FALSE)
#endif

#define MTC_GAS_BAND_TYPE_UNKNOWN                      (0)                      /* δ֪Ƶ�� */

#define MTC_GSM_BAND_TYPE_450                          (0x0001)                /* ARFCN�������Ƶ��:GSM 450 */
#define MTC_GSM_BAND_TYPE_480                          (0x0002)                /* ARFCN�������Ƶ��:GSM 480 */
#define MTC_GSM_BAND_TYPE_850                          (0x0004)                /* ARFCN�������Ƶ��:GSM 850 */
#define MTC_GSM_BAND_TYPE_P900                         (0x0008)                /* ARFCN�������Ƶ��:P-GSM 900 */
#define MTC_GSM_BAND_TYPE_R900                         (0x0010)                /* ARFCN�������Ƶ��:R-GSM 900 */
#define MTC_GSM_BAND_TYPE_E900                         (0x0020)                /* ARFCN�������Ƶ��:E-GSM 900 */
#define MTC_GSM_BAND_TYPE_1800                         (0x0040)                /* ARFCN�������Ƶ��:DCS 1800 */
#define MTC_GSM_BAND_TYPE_1900                         (0x0080)                /* ARFCN�������Ƶ��:PCS 1900 */
#define MTC_GSM_BAND_TYPE_700                          (0x0100)                /* ARFCN�������Ƶ��:GSM 700 */



#define MTC_WAS_FREQ_BAND2_ADD_MAX_FREQ_NUM             (12)                    /*Ƶ��2����󸽼�Ƶ����*/
#define MTC_WAS_FREQ_BAND4_ADD_MAX_FREQ_NUM             (9)                     /*Ƶ��4����󸽼�Ƶ����*/
#define MTC_WAS_FREQ_BAND5_ADD_MAX_FREQ_NUM             (6)                     /*Ƶ��5����󸽼�Ƶ����*/
#define MTC_WAS_FREQ_BAND6_ADD_MAX_FREQ_NUM             (2)                     /*Ƶ��6����󸽼�Ƶ����*/
#define MTC_WAS_FREQ_BAND7_ADD_MAX_FREQ_NUM             (14)                    /*Ƶ��7����󸽼�Ƶ����*/
#define MTC_WAS_FREQ_BAND19_ADD_MAX_FREQ_NUM            (3)                     /*Ƶ��19����󸽼�Ƶ����*/

#define MTC_WAS_FREQ_BAND1_DL_LOW_FREQ                  (10562)
#define MTC_WAS_FREQ_BAND1_DL_HIGH_FREQ                 (10838)
#define MTC_WAS_FREQ_BAND1_TXRXSEPERATION               (950)

#define MTC_WAS_FREQ_BAND2_DL_LOW_FREQ                  (9662)
#define MTC_WAS_FREQ_BAND2_DL_HIGH_FREQ                 (9938)
#define MTC_WAS_FREQ_BAND2_TXRXSEPERATION               (400)

#define MTC_WAS_FREQ_BAND3_DL_LOW_FREQ                  (1162)
#define MTC_WAS_FREQ_BAND3_DL_HIGH_FREQ                 (1513)
#define MTC_WAS_FREQ_BAND3_TXRXSEPERATION               (225)

#define MTC_WAS_FREQ_BAND4_DL_LOW_FREQ                  (1537)
#define MTC_WAS_FREQ_BAND4_DL_HIGH_FREQ                 (1738)
#define MTC_WAS_FREQ_BAND4_TXRXSEPERATION               (225)

#define MTC_WAS_FREQ_BAND5_DL_LOW_FREQ                  (4357)
#define MTC_WAS_FREQ_BAND5_DL_HIGH_FREQ                 (4458)
#define MTC_WAS_FREQ_BAND5_TXRXSEPERATION               (225)

#define MTC_WAS_FREQ_BAND6_DL_LOW_FREQ                  (4387)
#define MTC_WAS_FREQ_BAND6_DL_HIGH_FREQ                 (4413)
#define MTC_WAS_FREQ_BAND6_TXRXSEPERATION               (225)

#define MTC_WAS_FREQ_BAND7_DL_LOW_FREQ                  (2237)
#define MTC_WAS_FREQ_BAND7_DL_HIGH_FREQ                 (2563)
#define MTC_WAS_FREQ_BAND7_TXRXSEPERATION               (225)

#define MTC_WAS_FREQ_BAND8_DL_LOW_FREQ                  (2937)
#define MTC_WAS_FREQ_BAND8_DL_HIGH_FREQ                 (3088)
#define MTC_WAS_FREQ_BAND8_TXRXSEPERATION               (225)

#define MTC_WAS_FREQ_BAND9_DL_LOW_FREQ                  (9237)
#define MTC_WAS_FREQ_BAND9_DL_HIGH_FREQ                 (9387)
#define MTC_WAS_FREQ_BAND9_TXRXSEPERATION               (475)

#define MTC_WAS_FREQ_BAND10_DL_LOW_FREQ                 (3112)
#define MTC_WAS_FREQ_BAND10_DL_HIGH_FREQ                (3388)
#define MTC_WAS_FREQ_BAND10_TXRXSEPERATION              (225)

#define MTC_WAS_FREQ_BAND11_DL_LOW_FREQ                 (3712)
#define MTC_WAS_FREQ_BAND11_DL_HIGH_FREQ                (3787)
#define MTC_WAS_FREQ_BAND11_TXRXSEPERATION              (225)

#define MTC_WAS_FREQ_BAND19_DL_LOW_FREQ                 (712)
#define MTC_WAS_FREQ_BAND19_DL_HIGH_FREQ                (763)
#define MTC_WAS_FREQ_BAND19_TXRXSEPERATION              (400)


#define MTC_TDS_FRQ_UNIN                                (2) /* ��λΪ100KHZ */

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
/*****************************************************************************
 ö����    : MTC_BAND_INFO_ENUM
 Э����  :
 ö��˵��  : Ƶ��ö��
*****************************************************************************/
enum MTC_BAND_INFO_ENUM
{
    MTC_FREQ_BAND_NONE = 0,                                                     /*  û��Band */
    MTC_FREQ_BAND1 ,                                                            /*  Band1 */
    MTC_FREQ_BAND2 ,
    MTC_FREQ_BAND3 ,
    MTC_FREQ_BAND4 ,
    MTC_FREQ_BAND5 ,
    MTC_FREQ_BAND6 ,
    MTC_FREQ_BAND7 ,
    MTC_FREQ_BAND8 ,
    MTC_FREQ_BAND9 ,
    MTC_FREQ_BAND10,
    MTC_FREQ_BAND11,
    MTC_FREQ_BAND12,
    MTC_FREQ_BAND13,
    MTC_FREQ_BAND14,
    MTC_FREQ_BAND15,
    MTC_FREQ_BAND16,
    MTC_FREQ_BAND17,
    MTC_FREQ_BAND18,
    MTC_FREQ_BAND19,
    MTC_FREQ_BAND20,

    MTC_FREQ_BAND21,
    MTC_FREQ_BAND22,
    MTC_FREQ_BAND23,
    MTC_FREQ_BAND24,
    MTC_FREQ_BAND25,
    MTC_FREQ_BAND26,
    MTC_FREQ_BAND27,
    MTC_FREQ_BAND28,
    MTC_FREQ_BAND29,
    MTC_FREQ_BAND30,
    MTC_FREQ_BAND31,
    MTC_FREQ_BAND32,
    MTC_FREQ_BAND33 = 33,
    MTC_FREQ_BAND34,
    MTC_FREQ_BAND35,
    MTC_FREQ_BAND36,
    MTC_FREQ_BAND37,
    MTC_FREQ_BAND38,
    MTC_FREQ_BAND39,
    MTC_FREQ_BAND40,
    MTC_FREQ_BAND41,
    MTC_FREQ_BAND42,
    MTC_FREQ_BAND43,
    MTC_FREQ_BAND64 = 64,

    /* �Ǳ�Ƶ��begin */
    MTC_FREQ_BAND101 = 101,
    MTC_FREQ_BAND102 = 102,
    MTC_FREQ_BAND103 = 103,
    MTC_FREQ_BAND104 = 104,
    MTC_FREQ_BAND105 = 105,
    MTC_FREQ_BAND106 = 106,
    MTC_FREQ_BAND107 = 107,
    MTC_FREQ_BAND108 = 108,
    MTC_FREQ_BAND109 = 109,
    MTC_FREQ_BAND110 = 110,
    MTC_FREQ_BAND111 = 111,
    MTC_FREQ_BAND112 = 112,
    MTC_FREQ_BAND113 = 113,
    MTC_FREQ_BAND114 = 114,
    MTC_FREQ_BAND115 = 115,
    MTC_FREQ_BAND116 = 116,

    /* �Ǳ�Ƶ��end */
    MTC_FREQ_BAND_BUTT = 0xFFFF
};
typedef VOS_UINT16 MTC_BAND_INFO_ENUM_UINT16;



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
/*****************************************************************************
 �ṹ��    : MTC_RF_WCDMA_FREQ_BAND_INFO_STRU
 Э����  : 25.101Э��
 �ṹ˵��  : Ƶ�ε������Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          uhwFreqBandId;                          /* BAND ID*/
    VOS_UINT16                          uhwTxRxSeparation;                      /* Э����͵��������շ�Ƶ��*/
    VOS_UINT16                          uhwDlGeneralFreqNum_High;               /* һ��Ƶ�������Ƶ������ */
    VOS_UINT16                          uhwDlGeneralFreqNum_Low;                /* һ��Ƶ�������Ƶ������ */
    VOS_UINT16                          uhwDlGeneralOffset;                     /* һ��Ƶ�������ƫ�� */
    VOS_UINT16                          uhwDlAddOffset;                         /* ��ɢƵ�������ƫ�� */
    VOS_UINT16                          uhwUlGeneralFreqNum_High;               /* һ��Ƶ�������Ƶ������ */
    VOS_UINT16                          uhwUlGeneralFreqNum_Low;                /* һ��Ƶ�������Ƶ������ */
    VOS_UINT16                          uhwUlGeneralOffset;                     /* һ��Ƶ�������ƫ�� */
    VOS_UINT16                          uhwUlAddOffset;                         /* ��ɢƵ�������ƫ�� */
    VOS_UINT16                          uhwBandWidth;                           /* Ƶ�δ��� */
    VOS_UINT16                          uhwStartFreq;                           /* Ƶ����ʼƵ�� 100kHZ */
}MTC_RF_WCDMA_FREQ_BAND_INFO_STRU;

/*****************************************************************************
  8 UNION����
*****************************************************************************/

/*****************************************************************************
  9 OTHERS����
*****************************************************************************/

/*****************************************************************************
  10 ��������
*****************************************************************************/
VOS_UINT16 MTC_CalcGsmDlFreq(
    VOS_UINT16                          ushwBand,
    VOS_UINT16                          ushwFreqIndex
);

VOS_UINT16  MTC_CalcWcdmaDlFreq(
    VOS_UINT16                          ushwFreqBandId,
    VOS_UINT16                          ushwDlFreqNum
);

VOS_UINT32 MTC_WCDMAFindFreqBandNoByDlFreq(VOS_UINT16 usDlFreq);


#if (FEATURE_ON == FEATURE_LTE)
VOS_UINT16  MTC_CalcLteDlFreq(
    VOS_UINT16                          ucFreqBandIndicator,
    VOS_UINT16                          usDlEarfcn
);
#endif

#if (FEATURE_ON == FEATURE_UE_MODE_TDS)
VOS_UINT16  MTC_CalcTdsDlFreq(
    VOS_UINT16                          usDlEarfcn
);

#endif

#if 0
VOS_VOID  MTC_FillGsmNcellFreq(
    MTC_RF_USING_FREQ_LIST_STRU        *pstRfUsingFreqList,
    MTC_RF_FREQ_LIST_STRU              *pstRfFreqList
);

VOS_VOID  MTC_FillWcdmaNcellFreq(
    MTC_RF_USING_FREQ_LIST_STRU        *pstRfUsingFreqList,
    MTC_RF_FREQ_LIST_STRU              *pstRfFreqList
);

#if (FEATURE_ON == FEATURE_LTE)
VOS_VOID  MTC_FillLteNcellFreq(
    MTC_RF_USING_FREQ_LIST_STRU        *pstRfUsingFreqList,
    MTC_RF_FREQ_LIST_STRU              *pstRfFreqList
);
#endif

#if (FEATURE_ON == FEATURE_UE_MODE_TDS)
VOS_VOID  MTC_FillTdsNcellFreq(
    MTC_RF_USING_FREQ_LIST_STRU        *pstRfUsingFreqList,
    MTC_RF_FREQ_LIST_STRU              *pstRfFreqList
);
#endif
#endif

VOS_VOID  MTC_CalcHoppingFreq(
    MTC_RF_FREQ_LIST_STRU              *pstGsmFreqInfo,
    MTC_RF_FREQ_LIST_STRU              *pstRfFreqList
);

VOS_VOID  MTC_ChangeRfArfcnToFreq(
    RRC_MTC_USING_FREQ_IND_STRU        *pstUsingFreq,
    MTC_RF_USING_FREQ_LIST_STRU        *pstRfFreqList
);

VOS_VOID MTC_CalcRfDlFreq(
    RRC_MTC_USING_FREQ_IND_STRU        *pstUsingFreq,
    MTC_MODEM_FREQ_LIST_STRU           *pstModemFreq
);

#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of MtcCalcFreq.h */
