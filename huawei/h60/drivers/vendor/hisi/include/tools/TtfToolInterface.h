

#ifndef _TTF_TOOL_INTERFACE_H_
#define _TTF_TOOL_INTERFACE_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define ID_GRM_MNTN_OM_GRM_THROUGHPUT_CFG_REQ   (0xD007)  /*OM��GRM��������GRM������ͳ����Ϣ�۲⹦����Ϣ��*/
#define ID_GRM_MNTN_GRM_OM_THROUGHPUT_CFG_CNF   (0xD008)  /*GRM��OM�ظ�����GRM������ͳ����Ϣ�۲⹦����Ϣ��*/
#define ID_GRM_MNTN_GRM_OM_THROUGHPUT_INFO      (0xD009)  /*GRM��OM�ϱ�GRM������ͳ����Ϣ��Ϣ��*/

/*******************************************************************************
  3 ö�ٶ���
*******************************************************************************/
enum GRM_MNTN_OM_REPORT_ACTION_ENUM
{
    GRM_MNTN_OM_REPORT_STOP     = 0,
    GRM_MNTN_OM_REPORT_START    = 1,

    GRM_MNTN_OM_REPORT_BUTT
};

typedef VOS_UINT16 GRM_MNTN_OM_REPORT_ACTION_ENUM_UINT16;

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
#pragma pack(4)

/*================*/  /*GRMͳ�������ʿ�ά�ɲ���Ϣ������Ϣ*/
typedef struct
{
    GRM_MNTN_OM_REPORT_ACTION_ENUM_UINT16   enStartOrStop;
    VOS_UINT8                               aucRsv[2];
}GRM_MNTN_OM_GRM_THROUGHPUT_CFG_REQ_STRU;

typedef struct
{
    GRM_MNTN_OM_REPORT_ACTION_ENUM_UINT16   enStartOrStop;
    GRM_RESULT_TYPE_ENUM_UINT16             enRslt;
}GRM_MNTN_GRM_OM_THROUGHPUT_CFG_CNF_STRU;

/*����������ʵ��*/
typedef struct
{
    VOS_UINT32                              ulUlThroughput;          /*����������B/s����Ϣ�ϱ���ʱ����Ϊ1s����������ÿ���ϱ�֮������ͳ��*/
    VOS_UINT32                              ulUlNewDataBlk;          /*�����¿���������1sͳ��*/
    VOS_UINT32                              ulUlNackDataBlk;         /*����NACK�ش�����������1sͳ��*/
    VOS_UINT32                              ulUlPendDataBlk;         /*����PENDING�ش�����������1sͳ��*/
    VOS_UINT32                              ulUlDummyBlk;            /*����Dummy����������1sͳ��*/
}GRM_MNTN_GRLC_UL_THROUGHPUT_ENTITY;

/*����������ʵ��*/
typedef struct
{
    VOS_UINT32                              ulDlThroughput;          /*���������� B/s*/
    VOS_UINT32                              ulDlDataAmount;          /*�������ݿ�����, ��1sͳ��*/
    VOS_UINT32                              ulDlCorrectData;         /*������ȷ�����ݿ�, ��1sͳ��*/
    VOS_UINT32                              ulDlDataOkBeforeHarq;    /*HARQǰ������ȷ�����ݿ�, ��GPRSģʽ��û��HARQ���룬������GPRSģʽ��ʹ�ø����������ͳ�ƣ���1sͳ��*/
    VOS_UINT32                              ulDlPendDataBlk;         /*����PENGDING�ش�����������1sͳ��*/
    VOS_UINT32                              ulDlDataErrBeforeHarq;   /* HARQǰ�����������ݿ飬��1sͳ��*/
}GRM_MNTN_GRLC_DL_THROUGHPUT_ENTITY;

/*GRLC��ά�ɲ���Ϣʵ��*/
typedef struct
{
    GRM_MNTN_GRLC_UL_THROUGHPUT_ENTITY      stUlThroughputEntity;   /*������������Ϣ*/
    GRM_MNTN_GRLC_DL_THROUGHPUT_ENTITY      stDlThroughputEntity;   /*������������Ϣ*/
}GRM_MNTN_GRLC_THROUGHPUT_INFO;
/****************************************************/


/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/

#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of TtfToolInterface.h*/


