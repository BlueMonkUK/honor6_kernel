/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : phytoolinterface.h
  �� �� ��   : ����
  ��    ��   : luoqingquan
  ��������   : 2012��8��14��
  ����޸�   :
  ��������   : phytoolinterface.h ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��8��14��
    ��    ��   : luoqingquan
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include "vos.h"


#ifndef __PHYTOOLINTERFACE_H__
#define __PHYTOOLINTERFACE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#pragma pack(4)


/*****************************************************************************
  2 �궨��
*****************************************************************************/


/*****************************************************************************
  2.2 �궨��,W/G�궨��
*****************************************************************************/

/* ��ӡЯ���������� */
#define UPHY_TOOL_MNTN_ERROR_RPT_VAR_NUM        ( 5 )
#define UPHY_TOOL_INTER_REG_MAX_NUM             ( 300 )

/* ��ά�ɲ��func typeΪ4,intra msg������Ҫ��TOOLȷ�� */
#define UPHY_TOOL_MODEM1_FUNC_TYPE_OM_MSG       ( 0x44 )
#define UPHY_TOOL_MODEM1_FUNC_TYPE_INTRA_MSG    ( 0x54 )

#define UPHY_TOOL_MODEM0_FUNC_TYPE_OM_MSG       ( 0x04 )
#define UPHY_TOOL_MODEM0_FUNC_TYPE_INTRA_MSG    ( 0x1A )

/* ��С���뱣�ֺ�phyoaminterface.h��UPHY_OAM_SLEEP_INFO_NUMһ�� */
#define UPHY_TOOL_SLEEP_INFO_NUM                ( 16 )

/*****************************************************************************
  2.2 �궨��,GPHY�궨��
*****************************************************************************/

#ifdef INSTANCE_1
#define GPHY_RF_CAL_NO_SIG_FUNC_TYPE            ( 0x48 )
#define WPHY_RF_CAL_NO_SIG_FUNC_TYPE            ( 0x07 )
#else
#define GPHY_RF_CAL_NO_SIG_FUNC_TYPE            ( 0x08 )
#define WPHY_RF_CAL_NO_SIG_FUNC_TYPE            ( 0x07 )
#endif


/* PS����ʱ��������ʱ϶���� */
#define GPHY_PS_MAX_RX_TSN_NUM                  ( 5 )

/* PS����ʱ��������ʱ϶���� */
#define GPHY_PS_MAX_TX_TSN_NUM                  ( 4 )

/*****************************************************************************
 ԭ��GPHY_TOOL_FUNC_BIT_CTRL_REQ_STRU��ulFuncBitMaskÿ��bitλ�ĺ���;
 �����Ӧbitλ��1:���Ӧ����ʧЧ;
 �����Ӧbitλ��0:��ָ���Ӧ����;
*****************************************************************************/

#define GPHY_FUNC_DRX_USE_DISABLE_FLG           ( BIT0 )
#define GPHY_FUNC_DRX_SIMPLIFY_DISABLE_FLG      ( BIT1 )
#define GPHY_FUNC_BA_MEA_DISABLE_FLG            ( BIT2 )
#define GPHY_FUNC_INTERFERE_MEA_DISABLE_FLG     ( BIT3 )
#define GPHY_FUNC_C1_RESEL_DISABLE_FLG          ( BIT4 )
#define GPHY_FUNC_C2_RESEL_DISABLE_FLG          ( BIT5 )
#define GPHY_FUNC_NCELL_FBSB_DISABLE_FLG        ( BIT6 )
#define GPHY_FUNC_NCELL_SB_DISABLE_FLG          ( BIT7 )
#define GPHY_FUNC_NCELL_TASK_DISABLE_FLG        ( BIT8 )
#define GPHY_FUNC_RF_FRONT_DISPLAY_MNTN_FLG     ( BIT9 )



/* ��ά�ɲ����ݿ�����ϱ����� */
#define GPHY_TOOL_RPT_BLK_MAX_NUM               ( 10 )

/* ��ӡЯ���������� */
#define GPHY_TOOL_MNTN_ERROR_RPT_VAR_NUM        ( 5 )                           /* �����ϱ�ʱ���綨���ϱ��������� */


/*****************************************************************************
  2.3 �궨��,WPHY�궨��
*****************************************************************************/

/* ��ӡЯ���������� */
#define WPHY_TOOL_MNTN_ERROR_RPT_VAR_NUM        ( 5 )                           /* �����ϱ�ʱ���綨���ϱ��������� */

/* ����С�������� */
#define WPHY_MNTN_MAX_ACT_CELL_NUM              ( 7 )                           /* ���С�������Ŀ */


/* ��ά�ɲ����ݿ�����ϱ����� */
#define WPHY_TOOL_RPT_BLK_MAX_NUM               ( 10 )

#define WPHY_MNTN_TRCH_NUM                      ( 8 )

#define GPHY_TOOL_MA_FREQ_NUM                   ( 64 )                          /* ��Ƶ���еĸ��� */

#define GPHY_TOOL_BA_LIST_MAX_NUM               ( 32 )                          /* GSM BA LIST���ĸ��� */

#define GPHY_TOOL_AMR_MAX_ACS_NUM               ( 4 )                           /* ��󼤻���� */

#define GPHY_TOOL_RPT_MAX_LEN                   ( 100 )

#define GPHY_TOOL_AMR_CODEC_MAX_NUM             ( 8 )                           /* AMR��������� */

/* �����ÿ֡���ʱ϶���� */
#define GPHY_TOOL_TN_NUM_IN_FRAME               ( 8 )

#define GPHY_TOOL_MAX_UL_SLOT_NUM               ( 4 )                           /* Gsm��������ʱ϶���� */

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


/*****************************************************************************
  3.1 ö�ٶ���,GPHYö�ٶ���
*****************************************************************************/

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/
/*****************************************************************************
 UPHY�ظ�CNFʱ,Я���Ĵ�����ö��
*****************************************************************************/
enum UPHY_TOOL_RTC_DEBUG_ENUM
{
    UPHY_MNTN_RTC_DEBUG_CLOSE                   = 0,
    UPHY_MNTN_RTC_DEBUG_OPEN                    = 1,

    UPHY_MNTN_RTC_DEBUG_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_RTC_DEBUG_ENUM_UINT16;



/*****************************************************************************
 UPHY<-->TOOLԭ���б�
*****************************************************************************/
enum UPHY_TOOL_MNTN_MSG_ID_ENUM
{
    ID_TOOL_UPHY_REQ                            = 0x0,                          /* _H2ASN_MsgChoice  TOOL_UPHY_REQ_STRU */

    ID_TOOL_UPHY_QUERY_SHARE_ADDR_REQ           = 0xFE01,                       /* _H2ASN_MsgChoice  UPHY_TOOL_QUERY_SHARE_ADDR_REQ_STRU */
    ID_UPHY_TOOL_QUERY_SHARE_ADDR_IND           = 0xFE02,                       /* _H2ASN_MsgChoice  UPHY_TOOL_QUERY_SHARE_ADDR_IND_STRU */

    ID_TOOL_UPHY_SDT_LINK_STATUS_REQ            = 0xFE03,                       /* _H2ASN_MsgChoice  UPHY_TOOL_SDT_LINK_STATUS_REQ_STRU */
    ID_UPHY_TOOL_SDT_LINK_STATUS_CNF            = 0xFE04,                       /* _H2ASN_MsgChoice  UPHY_TOOL_COMMON_CNF_STRU */

    ID_TOOL_UPHY_ERROR_REPORT_REQ               = 0xFE05,                       /* _H2ASN_MsgChoice  UPHY_TOOL_ERROR_REPORT_REQ_STRU */
    ID_UPHY_TOOL_ERROR_REPORT_CNF               = 0xFE06,                       /* _H2ASN_MsgChoice  UPHY_TOOL_COMMON_CNF_STRU */

    ID_TOOL_UPHY_WPHY_BLER_RPT_REQ              = 0xFE07,
    ID_TOOL_UPHY_WPHY_INTRA_MSG_RPT_CTRL_REQ    = 0xFE08,
    ID_TOOL_UPHY_GPHY_INTRA_MSG_RPT_CTRL_REQ    = 0xFE09,


    ID_TOOL_UPHY_WPHY_INTER_GREG_REPORT_REQ     = 0xFE0A,                       /* _H2ASN_MsgChoice  UPHY_TOOL_WPHY_INTER_GREG_REPORT_REQ_STRU */
    ID_UPHY_TOOL_WPHY_INTER_GREG_REPORT_CNF     = 0xFE0B,                       /* _H2ASN_MsgChoice  UPHY_TOOL_WPHY_INTER_GREG_REPORT_CNF_STRU */
    ID_UPHY_TOOL_WPHY_INTER_GREG_REPORT_IND     = 0xFE0C,                       /* _H2ASN_MsgChoice  UPHY_TOOL_WPHY_INTER_GREG_REPORT_IND_STRU */
    ID_TOOL_UPHY_WPHY_INTER_GREG_STOP_REQ       = 0xFE0D,                       /* _H2ASN_MsgChoice  UPHY_TOOL_WPHY_INTER_GREG_STOP_REQ_STRU */
    ID_UPHY_TOOL_WPHY_INTER_GREG_STOP_CNF       = 0xFE0E,                       /* _H2ASN_MsgChoice  UPHY_TOOL_COMMON_CNF_STRU */

    ID_TOOL_UPHY_GPHY_INTER_WREG_REPORT_REQ     = 0xFE0F,                       /* _H2ASN_MsgChoice  UPHY_TOOL_GPHY_INTER_WREG_REPORT_REQ_STRU */
    ID_UPHY_TOOL_GPHY_INTER_WREG_REPORT_CNF     = 0xFE10,                       /* _H2ASN_MsgChoice  UPHY_TOOL_GPHY_INTER_WREG_REPORT_CNF_STRU */
    ID_UPHY_TOOL_GPHY_INTER_WREG_REPORT_IND     = 0xFE11,                       /* _H2ASN_MsgChoice  UPHY_TOOL_GPHY_INTER_WREG_REPORT_IND_STRU */
    ID_TOOL_UPHY_GPHY_INTER_WREG_STOP_REQ       = 0xFE12,                       /* _H2ASN_MsgChoice  UPHY_TOOL_GPHY_INTER_WREG_STOP_REQ_STRU */
    ID_UPHY_TOOL_GPHY_INTER_WREG_STOP_CNF       = 0xFE13,                       /* _H2ASN_MsgChoice  UPHY_TOOL_COMMON_CNF_STRU */

    ID_UPHY_TOOL_DRX_LATE_INFO_IND              = 0xFE14,                       /* _H2ASN_MsgChoice  UPHY_TOOL_DRX_LATE_INFO_IND_STRU */

    ID_UPHY_TOOL_INFO_REPORT_IND                = 0xFEF7,                       /* _H2ASN_MsgChoice  UPHY_TOOL_ERROR_REPORT_IND_STRU */
    ID_UPHY_TOOL_WARNING_REPORT_IND             = 0xFEF8,                       /* _H2ASN_MsgChoice  UPHY_TOOL_ERROR_REPORT_IND_STRU */
    ID_UPHY_TOOL_ERROR_REPORT_IND               = 0xFEF9,                       /* _H2ASN_MsgChoice  UPHY_TOOL_ERROR_REPORT_IND_STRU */


    ID_UPHY_TOOL_MSG_ID_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16;


/*****************************************************************************
 UPHY�ظ�CNFʱ,Я���Ĵ�����ö��
*****************************************************************************/
enum UPHY_TOOL_MNTN_RSLT_ENUM
{
    UPHY_MNTN_RSLT_SUCC                         = 0,
    UPHY_MNTN_RSLT_FAIL                         = 1,

    UPHY_MNTN_RSLT_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_MNTN_RSLT_ENUM_UINT16;



/*****************************************************************************
 ö����    : UPHY_TOOL_MNTN_RPT_LEVEL_ENUM
 Э����  :
 ö��˵��  : UPHY�����ϱ�����
*****************************************************************************/
enum UPHY_TOOL_MNTN_RPT_LEVEL_ENUM
{
    UPHY_MNTN_RPT_LEVEL_INFO                    = 1,
    UPHY_MNTN_RPT_LEVEL_WARNING                 = 2,
    UPHY_MNTN_RPT_LEVEL_ERROR                   = 3,

    UPHY_MNTN_RPT_LEVEL_BUTT
};
typedef VOS_UINT16 UPHY_TOOL_MNTN_RPT_LEVEL_ENUM_UINT16;





/*****************************************************************************
 GPHY<-->TOOLԭ���б�
*****************************************************************************/
enum GPHY_TOOL_MNTN_MSG_ID_ENUM
{
    /* �·�REQ��Ϣ��Ϊ��VOS��Ϣ���ϱ���IND��CNF��Ϣ����VOS��Ϣͷ��Ӧ��REQ�������������ָʾΪ��ASN���ɷ��� */
    ID_TOOL_GPHY_REQ                            = 0x0,                          /* _H2ASN_MsgChoice  TOOL_GPHY_REQ_STRU */

    ID_TOOL_GPHY_BLER_REPORT_REQ                = 0xE001,                       /* _H2ASN_MsgChoice  GPHY_TOOL_BLER_REPORT_REQ_STRU */
    ID_GPHY_TOOL_BLER_REPORT_CNF                = 0xE010,                       /* _H2ASN_MsgChoice  GPHY_TOOL_COMMON_CNF_STRU */
    ID_GPHY_TOOL_BLER_REPORT_IND                = 0xE011,                       /* _H2ASN_MsgChoice  GPHY_TOOL_BLER_REPORT_IND_STRU */

    ID_TOOL_GPHY_SNR_REPORT_REQ                 = 0xE002,                       /* _H2ASN_MsgChoice  GPHY_TOOL_SNR_REPORT_REQ_STRU */
    ID_GPHY_TOOL_SNR_REPORT_CNF                 = 0xE020,                       /* _H2ASN_MsgChoice  GPHY_TOOL_COMMON_CNF_STRU */
    ID_GPHY_TOOL_SNR_REPORT_IND                 = 0xE021,                       /* _H2ASN_MsgChoice  GPHY_TOOL_SNR_REPORT_IND_STRU */

    ID_TOOL_GPHY_AFC_REPORT_REQ                 = 0xE003,                       /* _H2ASN_MsgChoice  GPHY_TOOL_AFC_REPORT_REQ_STRU */
    ID_GPHY_TOOL_AFC_REPORT_CNF                 = 0xE030,                       /* _H2ASN_MsgChoice  GPHY_TOOL_COMMON_CNF_STRU */
    ID_GPHY_TOOL_AFC_REPORT_IND                 = 0xE031,                       /* _H2ASN_MsgChoice  GPHY_TOOL_AFC_REPORT_IND_STRU */

    ID_TOOL_GPHY_POWER_CTRL_REPORT_REQ          = 0xE004,                       /* _H2ASN_MsgChoice  GPHY_TOOL_POWER_CTRL_REPORT_REQ_STRU */
    ID_GPHY_TOOL_POWER_CTRL_REPORT_CNF          = 0xE040,                       /* _H2ASN_MsgChoice  GPHY_TOOL_COMMON_CNF_STRU */
    ID_GPHY_TOOL_POWER_CTRL_REPORT_IND          = 0xE041,                       /* _H2ASN_MsgChoice  GPHY_TOOL_POWER_CTRL_REPORT_IND_STRU */

    ID_TOOL_GPHY_POWER_CTRL_SET_REQ             = 0xE005,                       /* _H2ASN_MsgChoice  GPHY_TOOL_POWER_CTRL_SET_REQ_STRU */
    ID_GPHY_TOOL_POWER_CTRL_SET_CNF             = 0xE050,                       /* _H2ASN_MsgChoice  GPHY_TOOL_COMMON_CNF_STRU */

    ID_GPHY_TOOL_CHANNEL_QUALITY_IND            = 0xE080,                       /* _H2ASN_MsgChoice  GPHY_TOOL_CHANNEL_QUALITY_IND_STRU */

    ID_TOOL_GPHY_FUNC_BIT_CONTROL_REQ           = 0xE00A,                       /* _H2ASN_MsgChoice  GPHY_TOOL_FUNC_BIT_CTRL_REQ_STRU */
    ID_GPHY_TOOL_FUNC_BIT_CONTROL_CNF           = 0xE0A0,                       /* _H2ASN_MsgChoice  GPHY_TOOL_COMMON_CNF_STRU */

    ID_TOOL_GPHY_PARA_CONTROL_REQ               = 0xE00B,                       /* _H2ASN_MsgChoice  GPHY_TOOL_PARA_CONTROL_REQ_STRU */
    ID_GPHY_TOOL_PARA_CONTROL_CNF               = 0xE0B0,                       /* _H2ASN_MsgChoice  GPHY_TOOL_COMMON_CNF_STRU */

    ID_TOOL_GPHY_RF_FRONT_STATIC_REQ            = 0xE052,                       /* _H2ASN_MsgChoice  GPHY_TOOL_RF_FRONT_STATIC_REQ_STRU */
    ID_GPHY_TOOL_RF_FRONT_STATIC_IND            = 0xE053,                       /* _H2ASN_MsgChoice  GPHY_TOOL_RF_FRONT_STATIC_IND_STRU */

    ID_TOOL_GPHY_INTRA_MSG_RPT_CTRL_REQ         = 0xE054,                       /* _H2ASN_MsgChoice  GPHY_TOOL_INTRA_MSG_RPT_CTRL_REQ_STRU */
    ID_GPHY_TOOL_INTRA_MSG_RPT_CTRL_CNF         = 0xE055,                       /* _H2ASN_MsgChoice  GPHY_TOOL_COMMON_CNF_STRU */

    ID_TOOL_GPHY_AMR_REPORT_REQ                 = 0xE057,                       /* _H2ASN_MsgChoice  GPHY_TOOL_AMR_REPORT_REQ_STRU */
    ID_GPHY_TOOL_AMR_REPORT_CNF                 = 0xE058,                       /* _H2ASN_MsgChoice  GPHY_TOOL_COMMON_CNF_STRU */
    ID_GPHY_TOOL_AMR_REPORT_IND                 = 0xE059,                       /* _H2ASN_MsgChoice  GPHY_TOOL_AMR_REPORT_IND_STRU */

    ID_TOOL_GPHY_BLK_REPORT_LIST_REQ            = 0xE05A,                       /* _H2ASN_MsgChoice  GPHY_TOOL_BLK_REPORT_LIST_REQ_STRU */
    ID_GPHY_TOOL_BLK_REPORT_LIST_CNF            = 0xE05B,                       /* _H2ASN_MsgChoice  GPHY_TOOL_COMMON_CNF_STRU */
    ID_GPHY_TOOL_BLK_REPORT_LIST_IND            = 0xE05C,                       /* _H2ASN_MsgChoice  GPHY_TOOL_BLK_REPORT_LIST_IND_STRU */

    ID_GPHY_TOOL_GTC_TASK_INFO_IND              = 0xE061,                       /* _H2ASN_MsgChoice GPHY_GTC_CONTEXT_STRU */
    ID_GPHY_TOOL_RCV_SINGAL_INFO_IND            = 0xE062,                       /* _H2ASN_MsgChoice GPHY_TOOL_RCV_SINGAL_INFO_REQ_STRU */
    ID_GPHY_TOOL_CS_UL_INFO_IND                 = 0xE064,                       /* _H2ASN_MsgChoice GSM_CHN_UL_INFO_RPT_STRU */
    ID_GPHY_TOOL_CS_DL_INFO_IND                 = 0xE065,                       /* _H2ASN_MsgChoice COM_CS_DECODE_CTRL_INFO_STRU */

    ID_UPHY_TOOL_DCXO_TEMP_COMP_INFO_IND        = 0xE066,                       /* _H2ASN_MsgChoice UPHY_TOOL_DCXO_TEMP_COMP_INFO_IND_STRU */

    ID_GPHY_TOOL_REPORT_INFO_IND                = 0xE0F7,                       /* _H2ASN_MsgChoice  GPHY_TOOL_REPORT_ERROR_IND_STRU */
    ID_GPHY_TOOL_REPORT_WARNING_IND             = 0xE0F8,                       /* _H2ASN_MsgChoice  GPHY_TOOL_REPORT_ERROR_IND_STRU */
    ID_GPHY_TOOL_REPORT_ERROR_IND               = 0xE0F9,                       /* _H2ASN_MsgChoice  GPHY_TOOL_REPORT_ERROR_IND_STRU */

    ID_TOOL_GPHY_BBP_RTC_REQ                    = 0xE0FA,                       /* _H2ASN_MsgChoice GPHY_TOOL_BBP_RTC_REQ_STRU  */
    ID_GPHY_TOOL_BBP_RTC_CNF                    = 0xE0FB,                       /* _H2ASN_MsgChoice GPHY_TOOL_COMMON_CNF_STRU  */

    /* DSP-PROBE */
    ID_TOOL_GPHY_GSM_CELL_SNR_REQ               = 0xE600,                       /* _H2ASN_MsgChoice  GPHY_TOOL_GSM_CELL_SNR_REQ_STRU *//* ���������������ϱ�����С����6ǿ����������ȣ�����ֵ */
    ID_GPHY_TOOL_GSM_CELL_SNR_CNF               = 0xE601,                       /* _H2ASN_MsgChoice  GPHY_TOOL_COMMON_CNF_STRU *//* �ظ�ȷ����Ϣ */
    ID_GPHY_TOOL_GSM_CELL_SNR_IND               = 0xE602,                       /* _H2ASN_MsgChoice  GPHY_TOOL_GSM_CELL_SNR_IND_STRU *//* �ϱ�����С����6ǿ����������ȣ�����ֵ */

    ID_TOOL_GPHY_BA_LIST_MEAS_REQ               = 0xE603,                       /* _H2ASN_MsgChoice  GPHY_TOOL_BA_LIST_MEAS_REQ_STRU *//* ���������������ϱ���������32���������ź�ǿ�ȣ�bsicֵ */
    ID_GPHY_TOOL_BA_LIST_MEAS_CNF               = 0xE604,                       /* _H2ASN_MsgChoice  GPHY_TOOL_COMMON_CNF_STRU *//* �ظ�ȷ����Ϣ */
    ID_GPHY_TOOL_BA_LIST_MEAS_IND               = 0xE605,                       /* _H2ASN_MsgChoice  GPHY_TOOL_BA_LIST_MEAS_IND_STRU *//* �ϱ���������32���������ź�ǿ�ȣ�bsicֵ */

    ID_TOOL_GPHY_AMR_SETTING_REQ                = 0xE606,                       /* _H2ASN_MsgChoice  GPHY_TOOL_AMR_SETTING_REQ_STRU *//* ���������������ϱ�AMR������Ϣ */
    ID_GPHY_TOOL_AMR_SETTING_CNF                = 0xE607,                       /* _H2ASN_MsgChoice  GPHY_TOOL_COMMON_CNF_STRU *//* �ظ�ȷ����Ϣ */
    ID_GPHY_TOOL_AMR_SETTING_IND                = 0xE608,                       /* _H2ASN_MsgChoice  GPHY_TOOL_AMR_SETTING_IND_STRU *//* �ϱ�AMR������Ϣ */

    ID_TOOL_GPHY_AMR_CODEC_USAGE_REQ            = 0XE609,                       /* _H2ASN_MsgChoice  GPHY_TOOL_AMR_CODEC_USAGE_REQ_STRU *//* ���������������ϱ������ÿ���������ʵ�ʹ���� */
    ID_GPHY_TOOL_AMR_CODEC_USAGE_CNF            = 0XE60A,                       /* _H2ASN_MsgChoice  GPHY_TOOL_COMMON_CNF_STRU *//* �ظ�ȷ����Ϣ */
    ID_GPHY_TOOL_AMR_CODEC_USAGE_IND            = 0XE60B,                       /* _H2ASN_MsgChoice  GPHY_TOOL_AMR_CODEC_USAGE_IND_STRU *//* �ϱ������ÿ���������ʵ�ʹ������ */

    ID_TOOL_GPHY_AMR_CODEC_STAT_REQ             = 0xE60C,                       /* _H2ASN_MsgChoice  GPHY_TOOL_AMR_CODEC_STAT_REQ_STRU *//* ������������������������ʵ�ʹ���� */
    ID_GPHY_TOOL_AMR_CODEC_STAT_CNF             = 0xE60D,                       /* _H2ASN_MsgChoice  GPHY_TOOL_COMMON_CNF_STRU *//* �ظ�ȷ����Ϣ */
    ID_GPHY_TOOL_AMR_CODEC_STAT_IND             = 0xE60E,                       /* _H2ASN_MsgChoice  GPHY_TOOL_AMR_CODEC_STAT_IND_STRU *//* �ϱ������������ʵ�ʹ���� */

    ID_TOOL_GPHY_CS_PS_TX_INFO_REQ              = 0xE60F,                       /* _H2ASN_MsgChoice  GPHY_TOOL_CS_TX_INFO_REQ_STRU *//* ���������������ϱ�CS��������Ϣ */
    ID_GPHY_TOOL_CS_PS_TX_INFO_CNF              = 0xE610,                       /* _H2ASN_MsgChoice  GPHY_TOOL_COMMON_CNF_STRU *//* �ظ�ȷ����Ϣ */
    ID_GPHY_TOOL_CS_PS_TX_INFO_IND              = 0xE611,                       /* _H2ASN_MsgChoice  GPHY_TOOL_CS_TX_INFO_IND_STRU *//* �ϱ�CS��������Ϣ */

    ID_TOOL_GPHY_CS_FER_REQ                     = 0xE612,                       /* _H2ASN_MsgChoice  GPHY_TOOL_CS_FER_REQ_STRU *//* �����������ϱ�CS���FER */
    ID_GPHY_TOOL_CS_FER_CNF                     = 0xE613,                       /* _H2ASN_MsgChoice  GPHY_TOOL_COMMON_CNF_STRU *//* �ظ�ȷ����Ϣ */
    ID_GPHY_TOOL_CS_FER_IND                     = 0xE614,                       /* _H2ASN_MsgChoice  GPHY_TOOL_CS_FER_IND_STRU *//* �ϱ�CS���FER */

    ID_TOOL_GPHY_GET_CURRENT_DSC_REQ            = 0xE615,                       /* _H2ASN_MsgChoice  GPHY_TOOL_GET_CURRENT_DSC_REQ_STRU *//* �����������ϱ�DSC����ֵ */
    ID_GPHY_TOOL_GET_CURRENT_DSC_CNF            = 0xE616,                       /* _H2ASN_MsgChoice  GPHY_TOOL_COMMON_CNF_STRU *//* �ظ�ȷ����Ϣ */
    ID_GPHY_TOOL_GET_CURRENT_DSC_IND            = 0xE617,                       /* _H2ASN_MsgChoice  GPHY_TOOL_GET_CURRENT_DSC_IND_STRU *//* �ϱ�DSC����ֵ */

    ID_GPHY_TOOL_MASTER_WAKE_LTE_IND            = 0xE620,
    ID_GPHY_TOOL_MASTER_START_LTE_MEAS_IND      = 0xE621,
    ID_GPHY_TOOL_MASTER_REPORT_LTE_MEAS_IND     = 0xE622,
    ID_GPHY_TOOL_MASTER_STOP_LTE_MEAS_IND       = 0xE623,
    ID_GPHY_TOOL_SET_LTE_MEAS_FLAG_IND          = 0xE624,
    ID_GPHY_TOOL_CLEAR_LTE_MEAS_FLAG_IND        = 0xE625,

    ID_GPHY_TOOL_MASTER_WAKE_TDS_IND            = 0xE630,
    ID_GPHY_TOOL_MASTER_START_TDS_MEAS_IND      = 0xE631,
    ID_GPHY_TOOL_MASTER_REPORT_TDS_MEAS_IND     = 0xE632,
    ID_GPHY_TOOL_MASTER_STOP_TDS_MEAS_IND       = 0xE633,
    ID_GPHY_TOOL_SET_TDS_MEAS_FLAG_IND          = 0xE634,
    ID_GPHY_TOOL_CLEAR_TDS_MEAS_FLAG_IND        = 0xE635,

    /* ˫�����о������Ϣ�ϱ� */
    ID_GPHY_TOOL_DUAL_ANT_ESTIMATE_IND          = 0xE640,                       /* _H2ASN_MsgChoice  GPHY_TOOL_DUAL_ANT_ESTIMATE_IND_STRU *//* �ϱ����������������׶��о���Ϣ */
    ID_GPHY_TOOL_DUAL_ANT_COMPARE_IND           = 0xE641,                       /* _H2ASN_MsgChoice  GPHY_TOOL_DUAL_ANT_COMPARE_IND_STRU *//* �ϱ��������������ȽϽ׶��о���Ϣ */
    ID_GPHY_TOOL_DUAL_ANT_DELAY_IND             = 0xE642,                       /* _H2ASN_MsgChoice  GPHY_TOOL_DUAL_ANT_DELAY_IND_STRU *//* �ϱ������л�������Ϣ */

    ID_GPHY_TOOL_MSG_ID_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16;


/*****************************************************************************
 GPHY�ظ�CNFʱ,Я���Ĵ�����ö��
*****************************************************************************/
enum GPHY_TOOL_MNTN_RSLT_ENUM
{
    GPHY_MNTN_RSLT_SUCC                         = 0,
    GPHY_MNTN_RSLT_FAIL                         = 1,

    GPHY_MNTN_RSLT_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_MNTN_RSLT_ENUM_UINT16;



/*****************************************************************************
 TOOL�·��������ƣ���Ҫ�ǹ��ʿ��ƺ�BEP����
*****************************************************************************/
enum GPHY_TOOL_PARA_CTRL_ENUM
{
    GPHY_TOOL_PARA_PWC_CTRL                     = 0x01,                         /* �·����ƹ��ʲ��� */
    GPHY_TOOL_PARA_BEP_CTRL                     = 0x02,                         /* �·�����BEP���� */

    GPHY_TOOL_PARA_CTRL_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_PARA_CTRL_ENUM_UINT16;



/*****************************************************************************
 ö����    : GPHY_TOOL_MNTN_ERROR_RPT_LEVEL_ENUM_UINT16
 Э����  :
 ö��˵��  : GPHY�����ϱ�����
*****************************************************************************/
enum GPHY_TOOL_ERROR_RPT_LEVEL_ENUM
{
    GPHY_TOOL_ERROR_LEVEL_INFO                  = 1,
    GPHY_TOOL_ERROR_LEVEL_WARNING               = 2,
    GPHY_TOOL_ERROR_LEVEL_ERROR                 = 3,

    GPHY_TOOL_ERROR_LEVEL_BUTT
};
typedef VOS_UINT16 GPHY_TOOL_ERROR_RPT_LEVEL_ENUM_UINT16;






/*****************************************************************************
 GPHYģ�鶨��
*****************************************************************************/

enum GPHY_TOOL_MODULE_ENUM
{
    GPHY_MODULE_DRV                             = 0x5501,                       /* DRIVERģ������� */
    GPHY_MODULE_MEA                             = 0x5502,                       /* ����ģ������� */
    GPHY_MODULE_NCE_TSK                         = 0x5503,                       /* ��������ģ������� */
    GPHY_MODULE_NCE_RES                         = 0x5504,                       /* ������ѡģ������� */
    GPHY_MODULE_CHN_CCH                         = 0x5505,                       /* �����ŵ�ģ������� */
    GPHY_MODULE_CHN_CSDCH                       = 0x5506,                       /* CSר���ŵ�ģ������� */
    GPHY_MODULE_CHN_PSDCH                       = 0x5507,                       /* PSר���ŵ�ģ������� */
    GPHY_MODULE_CHN_AMRDTX                      = 0x5508,                       /* �ŵ�AMRDTXģ������� */
    GPHY_MODULE_COM                             = 0x5509,                       /* ����ģ������� */
    GPHY_MODULE_COM_MAIL                        = 0x550A,                       /* ��������ģ������� */
    GPHY_MODULE_COM_TOA                         = 0x550B,                       /* ����TOAģ������� */
    GPHY_MODULE_DRX                             = 0x550C,                       /* DRXģ������� */
    GPHY_MODULE_OM                              = 0x550D,                       /* OMģ������� */
    GPHY_MODULE_WMEAS                           = 0x550E,                       /* WMEASģ������� */
    GPHY_MODULE_CAL                             = 0x550F,                       /* CALģ������� */
    GPHY_MODULE_LMEAS                           = 0x5510,                       /* LTE����ģ������� */
    GPHY_MODULE_PD_INTERFACE                    = 0x5511,                       /* �µ粿�ִ���ӿں���ģ�� */
    GPHY_MODULE_TMEAS                           = 0x5512,                       /* TD����ģ������� */
    GPHY_MODULE_BUTT

};
typedef VOS_UINT16 GPHY_TOOL_MODULE_ENUM_UINT16;


/*****************************************************************************
  3.2 ö�ٶ���,WPHYö�ٶ���
*****************************************************************************/


/*****************************************************************************
 WPHY<-->TOOLԭ���б�
*****************************************************************************/

enum WPHY_TOOL_MNTN_MSG_ID_ENUM
{
    /* �·�REQ��Ϣ��Ϊ��VOS��Ϣ���ϱ���IND��CNF��Ϣ����VOS��Ϣͷ��Ӧ��REQ�������������ָʾΪ��ASN���ɷ��� */
    ID_TOOL_WPHY_REQ                            = 0x0,                          /* _H2ASN_MsgChoice TOOL_WPHY_REQ_STRU */

    /* ���С���ϱ�  */
    ID_TOOL_WPHY_ACT_CELL_REQ                   = 0xF001,                       /* _H2ASN_MsgChoice WPHY_TOOL_ACT_CELL_REQ_STRU */
    ID_WPHY_TOOL_ACT_CELL_CNF                   = 0xF002,                       /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_CNF_STRU */
    ID_WPHY_TOOL_ACT_CELL_IND                   = 0xF003,                       /* _H2ASN_MsgChoice WPHY_TOOL_ACT_CELL_INFO_IND_STRU */

    /* С�������б��ϱ� */
    ID_TOOL_WPHY_CELL_SEARCH_LIST_REQ           = 0xF004,                       /* _H2ASN_MsgChoice WPHY_TOOL_CELL_SEARCH_LIST_REQ_STRU  */
    ID_WPHY_TOOL_CELL_SEARCH_LIST_CNF           = 0xF005,                       /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_CNF_STRU */
    ID_WPHY_TOOL_CELL_SEARCH_LIST_IND           = 0xF006,                       /* _H2ASN_MsgChoice WPHY_TOOL_CELL_SEARCH_LIST_IND_STRU */


    /* ���ؿ��ؿ��� */
    ID_TOOL_WPHY_PWR_SWITCH_CTRL_REQ            = 0xF00A,                       /* _H2ASN_MsgChoice WPHY_TOOL_PWR_SWITCH_CTRL_REQ_STRU */
    ID_WPHY_TOOL_PWR_SWITCH_CTRL_CNF            = 0xF00B,                       /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_CNF_STRU */

    /* ���ز������� */
    ID_TOOL_WPHY_PWR_PARA_SET_REQ               = 0xF010,                       /* _H2ASN_MsgChoice WPHY_TOOL_PWR_PARA_SET_REQ_STRU */
    ID_WPHY_TOOL_PWR_PARA_SET_CNF               = 0xF011,                       /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_CNF_STRU */


    /* ������ϱ� */
    ID_TOOL_WPHY_BLER_REPORT_REQ                = 0xF020,                       /* _H2ASN_MsgChoice WPHY_TOOL_BLER_REPORT_REQ_STRU */
    ID_WPHY_TOOL_BLER_REPORT_CNF                = 0xF021,                       /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_CNF_STRU */
    ID_WPHY_TOOL_BLER_REPORT_IND                = 0xF022,                       /* _H2ASN_MsgChoice WPHY_TOOL_BLER_REPORT_IND_STRU */


    /* �¼��ϱ� */
    ID_TOOL_WPHY_EVENT_REPORT_REQ               = 0xF02A,                       /* _H2ASN_MsgChoice WPHY_TOOL_EVENT_REPORT_REQ_STRU */
    ID_WPHY_TOOL_EVENT_REPORT_CNF               = 0xF02B,                       /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_CNF_STRU */

    /* �¼��ϱ�:ͬʧ���ϱ� */
    ID_WPHY_TOOL_SYNC_STATUS_IND                = 0xF02C,                       /* _H2ASN_MsgChoice WPHY_TOOL_SYNC_STATUS_IND_STRU */

    /* �¼��ϱ�:AFC״̬ */
    ID_WPHY_TOOL_VCTCXO_STATUS_IND              = 0xF02E,                       /* _H2ASN_MsgChoice WPHY_TOOL_AFC_STATUS_IND_STRU */

    /* �¼��ϱ�:�ж������쳣���� */
    ID_WPHY_TOOL_INTERRUPT_EXCEPTIION_IND       = 0xF030,                       /* _H2ASN_MsgChoice WPHY_TOOL_INTERRUPT_EXCEPTIION_IND_STRU */

    /* �¼��ϱ�:L1״̬�ϱ� */
    ID_WPHY_TOOL_WCDMA_L1_STATUS_IND            = 0xF031,                       /* _H2ASN_MsgChoice WPHY_TOOL_WCDMA_L1_STATUS_IND_STRU */

    /* DRX���� */
    ID_TOOL_WPHY_DRX_SWITCH_REQ                 = 0xF032,                       /* _H2ASN_MsgChoice WPHY_TOOL_DRX_SWITCH_REQ_STRU */
    ID_WPHY_TOOL_DRX_SWITCH_CNF                 = 0xF033,                       /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_CNF_STRU */

    /* �������� */
    ID_TOOL_WPHY_THRESHOLD_SET_REQ              = 0xF034,                       /* _H2ASN_MsgChoice WPHY_TOOL_THRESHOLD_SET_REQ_STRU */
    ID_WPHY_TOOL_THRESHOLD_SET_CNF              = 0xF035,                       /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_CNF_STRU */

    /* ���޲�ѯ */
    ID_TOOL_WPHY_THRESHOLD_QUERY_REQ            = 0xF036,                       /* _H2ASN_MsgChoice WPHY_TOOL_THRESHOLD_QUERY_REQ_STRU */
    ID_WPHY_TOOL_THRESHOLD_QUERY_IND            = 0xF037,                       /* _H2ASN_MsgChoice WPHY_TOOL_THRESHOLD_QUERY_IND_STRU */

    /* �����ϱ� */
    ID_TOOL_WPHY_ERROR_REPORT_REQ               = 0xF038,                       /* _H2ASN_MsgChoice WPHY_TOOL_ERROR_REPORT_REQ_STRU */
    ID_WPHY_TOOL_ERROR_REPORT_CNF               = 0xF039,                       /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_CNF_STRU */
    ID_WPHY_TOOL_ERROR_REPORT_IND               = 0xF03A,                       /* _H2ASN_MsgChoice WPHY_TOOL_ERROR_REPORT_IND_STRU  */
    ID_WPHY_TOOL_WARNING_REPORT_IND             = 0xF03B,                       /* _H2ASN_MsgChoice WPHY_TOOL_ERROR_REPORT_IND_STRU  */
    ID_WPHY_TOOL_INFO_REPORT_IND                = 0xF03C,                       /* _H2ASN_MsgChoice WPHY_TOOL_ERROR_REPORT_IND_STRU  */


    ID_TOOL_WPHY_RF_FRONT_END_PARA_REQ          = 0xF03D,                       /* _H2ASN_MsgChoice WPHY_TOOL_RF_FRONT_END_PARA_REQ_STRU  */
    ID_WPHY_TOOL_RF_FRONT_END_PARA_CNF          = 0xF03E,                       /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_CNF_STRU  */
    ID_WPHY_TOOL_RF_FRONT_END_PARA_IND          = 0xF03F,                       /* _H2ASN_MsgChoice WPHY_TOOL_RF_FRONT_END_PARA_IND_STRU  */

    ID_WPHY_TOOL_PI_INT_RESULT_IND              = 0xF040,                       /* _H2ASN_MsgChoice WPHY_TOOL_PI_INT_RESULT_IND_STRU  */
    ID_WPHY_TOOL_AI_INT_RESULT_IND              = 0xF041,                       /* _H2ASN_MsgChoice WPHY_TOOL_AI_INT_RESULT_IND_STRU  */

    /* T313 ״̬�ϱ� */
    ID_WPHY_TOOL_T313_STATUS_IND                = 0xF042,                       /* _H2ASN_MsgChoice WPHY_TOOL_T313_STATUS_IND_STRU  */
    /* T312 ״̬�ϱ� */
    ID_WPHY_TOOL_T312_STATUS_IND                = 0xF043,                       /* _H2ASN_MsgChoice WPHY_TOOL_T312_STATUS_IND_STRU */

    /* PA״̬�ϱ� */
    ID_TOOL_WPHY_PA_STATUS_REPORT_REQ           = 0xF046,                       /* _H2ASN_MsgChoice WPHY_TOOL_PA_STATUS_REPORT_REQ_STRU */
    ID_WPHY_TOOL_PA_STATUS_REPORT_CNF           = 0xF047,                       /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_CNF_STRU */
    ID_WPHY_TOOL_PA_STATUS_REPORT_IND           = 0xF048,                       /* _H2ASN_MsgChoice WPHY_TOOL_PA_STATUS_REPORT_IND_STRU */

    ID_TOOL_WPHY_INTRA_MSG_RPT_CTRL_REQ         = 0xF049,                       /* _H2ASN_MsgChoice WPHY_TOOL_INTRA_MSG_RPT_CTRL_REQ_STRU */
    ID_WPHY_TOOL_INTRA_MSG_RPT_CTRL_CNF         = 0xF04A,                       /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_CNF_STRU */

    /* ǰ�˿�ά�ɲ� */
    ID_TOOL_WPHY_RF_FRONT_STATIC_REQUIRE_REQ    = 0xF070,                       /* _H2ASN_MsgChoice WPHY_TOOL_RF_FRONT_END_STATIC_REQUIRE_REQ_STRU  */
    ID_WPHY_TOOL_RF_FRONT_STATIC_REQUIRE_IND    = 0xF071,                       /* _H2ASN_MsgChoice WPHY_TOOL_RF_FRONT_END_STATIC_REQUIRE_IND_STRU  */

    ID_TOOL_WPHY_BBP_RTC_REQ                    = 0xF072,                       /* _H2ASN_MsgChoice WPHY_TOOL_BBP_RTC_REQ_STRU  */
    ID_WPHY_TOOL_BBP_RTC_CNF                    = 0xF073,                       /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_CNF_STRU  */

    ID_TOOL_WPHY_RF_FRONT_DISPLAY_REQ           = 0xF075,                       /* _H2ASN_MsgChoice WPHY_TOOL_RF_FRONT_END_DISPLAY_REQ_STRU  */
    ID_WPHY_TOOL_RF_FRONT_DISPLAY_CNF           = 0xF077,                       /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_CNF_STRU */


    ID_TOOL_WPHY_RAM_REPORT_REQ                 = 0xF078,                       /* _H2ASN_MsgChoice WPHY_TOOL_RAM_REPORT_REQ_STRU  */
    ID_WPHY_TOOL_RAM_REPORT_CNF                 = 0xF079,                       /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_CNF_STRU  */
    ID_WPHY_TOOL_RAM_REPORT_IND                 = 0xF07A,                       /* _H2ASN_MsgChoice WPHY_TOOL_RAM_REPORT_IND_STRU  */

    ID_TOOL_WPHY_BLK_REPORT_LIST_REQ            = 0xF07B,                       /* _H2ASN_MsgChoice WPHY_TOOL_BLK_REPORT_LIST_REQ_STRU  */
    ID_WPHY_TOOL_BLK_REPORT_LIST_CNF            = 0xF07C,                       /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_CNF_STRU  */
    ID_WPHY_TOOL_BLK_REPORT_LIST_IND            = 0xF07D,                       /* _H2ASN_MsgChoice WPHY_TOOL_BLK_REPORT_LIST_IND_STRU  */

    ID_TOOL_WPHY_RA_MNTN_CTRL_REQ               = 0xF07E,                       /* _H2ASN_MsgChoice WPHY_TOOL_RA_MNTN_CTRL_REQ_STRU */
    ID_WPHY_TOOL_RA_MNTN_CTRL_CNF               = 0xF07F,                       /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_CNF_STRU  */

    ID_TOOL_WPHY_ARM_0MS_MNTN_CTRL_REQ          = 0xF080,                       /* _H2ASN_MsgChoice WPHY_TOOL_ARM_0MS_MNTN_CTRL_REQ_STRU */
    ID_WPHY_TOOL_ARM_0MS_MNTN_CTRL_CNF          = 0xF081,                       /* _H2ASN_MsgChoice WPHY_TOOL_COMMON_CNF_STRU  */

    ID_WPHY_TOOL_SLAVE_CDRX_START_IND           = 0xF082,                       /* _H2ASN_MsgChoice WPHY_TOOL_SLAVE_CDRX_START_IND_STRU  */
    ID_WPHY_TOOL_SLAVE_CDRX_STOP_IND            = 0xF083,                       /* _H2ASN_MsgChoice WPHY_TOOL_SLAVE_CDRX_STOP_IND_STRU  */
    ID_WPHY_TOOL_SLAVE_W_FINISH_IND             = 0xF084,                       /* _H2ASN_MsgChoice WPHY_TOOL_SLAVE_W_FINISH_IND_STRU */
    ID_WPHY_TOOL_SLAVE_GRSSI_FINISH_IND         = 0xF085,                       /* _H2ASN_MsgChoice WPHY_TOOL_SLAVE_GRSSI_FINISH_IND_STRU */
    ID_WPHY_TOOL_SLAVE_GBSIC_FINISH_IND         = 0xF086,                       /* _H2ASN_MsgChoice WPHY_TOOL_SLAVE_GBSIC_FINISH_IND_STRU */
    ID_WPHY_TOOL_SLAVE_SCHEDULE_TIMEOUT_IND     = 0xF087,                       /* _H2ASN_MsgChoice WPHY_TOOL_SLAVE_SCHEDULE_TIMEOUT_IND_STRU */

    ID_WPHY_TOOL_MASTER_WAKE_LTE_IND            = 0xF090,                       /* _H2ASN_MsgChoice WPHY_TOOL_MASTER_WAKE_LTE_IND_STRU */
    ID_WPHY_TOOL_MASTER_START_LTE_MEAS_IND      = 0xF091,                       /* _H2ASN_MsgChoice WPHY_TOOL_MASTER_START_LTE_MEAS_IND_STRU */
    ID_WPHY_TOOL_MASTER_REPORT_LTE_MEAS_IND     = 0xF092,                       /* _H2ASN_MsgChoice WPHY_TOOL_MASTER_REPORT_LTE_MEAS_IND_STRU */
    ID_WPHY_TOOL_MASTER_STOP_LTE_MEAS_IND       = 0xF093,                       /* _H2ASN_MsgChoice WPHY_TOOL_MASTER_STOP_LTE_MEAS_IND_STRU */
    ID_WPHY_TOOL_SET_LTE_MEAS_FLAG_IND          = 0xF094,                       /* _H2ASN_MsgChoice WPHY_TOOL_SET_LTE_MEAS_FLAG_IND_STRU */

    ID_WPHY_TOOL_QPC_OPEN_UL_IND                = 0xF0A0,                       /* _H2ASN_MsgChoice WPHY_TOOL_QPC_OPEN_UL_IND_STRU */
    ID_WPHY_TOOL_QPC_CLOSE_UL_IND               = 0xF0A1,                       /* _H2ASN_MsgChoice WPHY_TOOL_QPC_CLOSE_UL_IND_STRU */
    ID_WPHY_TOOL_QPC_RRC_STATE_IND              = 0xF0A2,                       /* _H2ASN_MsgChoice WPHY_TOOL_QPC_RRC_STATE_IND_STRU */
    ID_WPHY_TOOL_QPC_RB_TYPE_IND                = 0xF0A3,                       /* _H2ASN_MsgChoice WPHY_TOOL_QPC_RB_TYPE_IND_STRU */

    ID_WPHY_TOOL_DPDT_ESTIMATE_STATE_IND        = 0xF0B0,                       /* _H2ASN_MsgChoice WPHY_TOOL_DPDT_ESTIMATE_STATE_IND_STRU */
    ID_WPHY_TOOL_DPDT_COMPARE_STATE_IND         = 0xF0B1,                       /* _H2ASN_MsgChoice WPHY_TOOL_DPDT_COMPARE_STATE_IND_STRU */
    ID_WPHY_TOOL_DPDT_PROTECT_STATE_IND         = 0xF0B2,                       /* _H2ASN_MsgChoice WPHY_TOOL_DPDT_PROTECT_STATE_IND_STRU */

    ID_WPHY_TOOL_MSG_ID_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16;


/*****************************************************************************
 ö����    : WPHY_TOOL_MNTN_RSLT_ENUM_UINT16
 Э����  :
 ö��˵��  : ���ֻظ���Ϣ�Ľ��ֵ:�ɹ�����ʧ�ܵ�
*****************************************************************************/
enum WPHY_TOOL_MNTN_RSLT_ENUM
{
    WPHY_MNTN_RSLT_SUCC                         = 0,                            /* �ɹ� */
    WPHY_MNTN_RSLT_FAIL                         = 1,                            /* ʧ�� */
    WPHY_MNTN_RSLT_PERIOD_NOT_SUPPORT           = 2,                            /* �������ڲ�֧�� */
    WPHY_MNTN_RSLT_PARA_INVALID                 = 3,                            /* ���ò�����Ч */
    WPHY_MNTN_RSLT_PARA_TYPE_OVERFLOW           = 4,                            /* ���ò������Ͳ���ȷ */

    WPHY_MNTN_RSLT_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MNTN_RSLT_ENUM_UINT16;





/*****************************************************************************
 ö����    : WPHY_TOOL_MNTN_REPORT_TYPE_ENUM_UINT16
 Э����  :
 ö��˵��  : ���ֲ��������ϱ�����
*****************************************************************************/
enum WPHY_TOOL_MNTN_REPORT_TYPE_ENUM
{
    WPHY_MNTN_REPORT_TYPE_NO_RPT                = 0,                            /* ���ϱ�: */
    WPHY_MNTN_REPORT_TYPE_ONE_TIME              = 1,                            /* �ϱ�1��: */
    WPHY_MNTN_REPORT_TYPE_PER_FRAME             = 2,                            /* ÿ֡�ϱ�: */
    WPHY_MNTN_REPORT_TYPE_ASSIGNED_PERIOD       = 3,                            /* ָ������:��֡Ϊ��λ�����ѡ�����������д��һ��������ָ�����ڡ� */
    WPHY_MNTN_REPORT_TYPE_EVENT_TRIGER          = 4,                            /* �¼�����: */

    WPHY_MNTN_REPORT_TYPE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MNTN_REPORT_TYPE_ENUM_UINT16;


/*****************************************************************************
 ö����    : WPHY_MNTN_PWR_SWITCH_TYPE_ENUM_UINT16
 Э����  :
 ö��˵��  : ���ؿ��Ƶ�ö��ֵ
*****************************************************************************/
enum WPHY_TOOL_MNTN_PWR_SWITCH_TYPE_ENUM
{
    WPHY_MNTN_PWR_SWITCH_TYPE_UL_PC             = 0,                            /* ���й��ʿ���:�̶����з��䶫�� */
    WPHY_MNTN_PWR_SWITCH_TYPE_OLPC              = 1,                            /* �⻷���� */
    WPHY_MNTN_PWR_SWITCH_TYPE_WINUP             = 2,                            /* winup���� */
    WPHY_MNTN_PWR_SWITCH_TYPE_WINDOWN           = 3,                            /* windown���� */
    WPHY_MNTN_PWR_SWITCH_TYPE_FIX_DL_PWR        = 4,                            /* �̶����з��书�� */

    WPHY_MNTN_PWR_SWITCH_TYPE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MNTN_PWR_SWITCH_TYPE_ENUM_UINT16;


/*****************************************************************************
 ö����    : WPHY_TOOL_MNTN_SWITCH_CTRL_ENUM_UINT16
 Э����  :
 ö��˵��  : ���ؿ��Ƶ�ö��ֵ
*****************************************************************************/
enum WPHY_TOOL_MNTN_SWITCH_CTRL_ENUM
{
    WPHY_MNTN_SWITCH_CTRL_CLOSE                 = 0,                            /* �ر� */
    WPHY_MNTN_SWITCH_CTRL_OPEN                  = 1,                            /* �� */

    WPHY_MNTN_SWITCH_CTRL_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MNTN_SWITCH_CTRL_ENUM_UINT16;


/*****************************************************************************
 ö����    : WPHY_TOOL_SYNC_STATUS_ENUM_UINT16
 Э����  :
 ö��˵��  : ���ؿ��Ƶ�ö��ֵ
*****************************************************************************/
enum WPHY_TOOL_SYNC_STATUS_ENUM
{
    WPHY_MNTN_SYNC_STATUS_FIX                   = 0,                            /* ���� */
    WPHY_MNTN_SYNC_STATUS_LOST                  = 1,                            /* ʧ�� */

    WPHY_MNTN_SYNC_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_SYNC_STATUS_ENUM_UINT16;


/*****************************************************************************
 ö����    : WPHY_TOOL_TCXO_STATUS_ENUM_UINT16
 Э����  :
 ö��˵��  : ���ؿ��Ƶ�ö��ֵ
*****************************************************************************/
enum WPHY_TOOL_TCXO_STATUS_ENUM
{
    WPHY_MNTN_TCXO_STATUS_FREEZE                = 0,                            /* ���� */
    WPHY_MNTN_TCXO_STATUS_UNFREEZE              = 1,                            /* ʧ�� */

    WPHY_MNTN_TCXO_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_TCXO_STATUS_ENUM_UINT16;



/*****************************************************************************
 ö����    : WPHY_TOOL_MNTN_PWR_PARA_ENUM_UINT32
 Э����  :
 ö��˵��  : ���ʲ������ͣ���ѯ�����ù���
*****************************************************************************/
enum WPHY_TOOL_MNTN_PWR_PARA_ENUM
{
    WPHY_MNTN_PWR_PARA_SIR_TARGET               = 0x00000001,                   /* SirTarget */
    WPHY_MNTN_PWR_PARA_CURR_TRCH_ID_SELECTED    = 0x00000002,                   /* ��ǰѡ���TrchId */
    WPHY_MNTN_PWR_PARA_DL_TPC                   = 0x00000004,                   /* DL TPC */
    WPHY_MNTN_PWR_PARA_CURR_SIR                 = 0x00000008,                   /* ��ǰ��SIR */
    WPHY_MNTN_PWR_PARA_CURR_BLER                = 0x00000010,                   /* ��ǰ��BLER */
    WPHY_MNTN_PWR_PARA_UL_TPC                   = 0x00000020,                   /* UL TPC */
    WPHY_MNTN_PWR_PARA_BD                       = 0x00000040,                   /* Bd */
    WPHY_MNTN_PWR_PARA_BEC                      = 0x00000080,                   /* Bec */
    WPHY_MNTN_PWR_PARA_BED                      = 0x00000100,                   /* Bed */
    WPHY_MNTN_PWR_PARA_BHS_NACK                 = 0x00000200,                   /* BhsNack */
    WPHY_MNTN_PWR_PARA_BHS_ACK                  = 0x00000400,                   /* BhsAck */
    WPHY_MNTN_PWR_PARA_BHS_CQI                  = 0x00000800,                   /* BhsCQI */
    WPHY_MNTN_PWR_PARA_BHS_TFCI                 = 0x00001000,                   /* TFCI */
    WPHY_MNTN_PWR_PARA_SLOT_FORMAT              = 0x00002000,                   /* SlotFormat */
    WPHY_MNTN_PWR_PARA_MAX_PWR                  = 0x00004000,                   /* ����书�� */
    WPHY_MNTN_PWR_PARA_MIN_PWR                  = 0x00008000,                   /* ��С���书�� */
    WPHY_MNTN_PWR_PARA_INIT_PWR                 = 0x00010000,                   /* lint !e575 ��ʼ���书�� */

    WPHY_MNTN_PWR_PARA_BUTT
};
typedef VOS_UINT32 WPHY_TOOL_MNTN_PWR_PARA_ENUM_UINT32;



/*****************************************************************************
 ö����    : WPHY_TOOL_MNTN_EVENT_TYPE_ENUM_UINT32
 Э����  :
 ö��˵��  : �ϱ��¼�����
*****************************************************************************/
enum WPHY_TOOL_MNTN_EVENT_TYPE_ENUM
{
    WPHY_MNTN_EVENT_TYPE_AFC_STATUS             = 0x00000001,                   /* 6ms ����ʱ */
    WPHY_MNTN_EVENT_TYPE_SYNC_STATUS            = 0x00000002,                   /* 9ms ����ʱ */
    WPHY_MNTN_EVENT_TYPE_L1_STATUS              = 0x00000004,                   /* L1״̬ */
    WPHY_MNTN_EVENT_TYPE_INT_EXCEPT             = 0x00000008,                   /* �ж��쳣 */

    WPHY_MNTN_EVENT_TYPE_BUTT
};
typedef VOS_UINT32 WPHY_TOOL_MNTN_EVENT_TYPE_ENUM_UINT32;




/*****************************************************************************
 ö����    : WPHY_TOOL_MNTN_INT_EXCEPT_ENUM_UINT16
 Э����  :
 ö��˵��  : �ж��쳣����
*****************************************************************************/
enum WPHY_TOOL_MNTN_INT_EXCEPT_ENUM
{
    WPHY_MNTN_INT_EXCEPT_TYPE_6MS_TASK_EXPIRED  = 0,                            /* 6ms ����ʱ */
    WPHY_MNTN_INT_EXCEPT_TYPE_9MS_TASK_EXPIRED  = 1,                            /* 9ms ����ʱ */

    WPHY_MNTN_INT_EXCEPT_TYPE_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MNTN_INT_EXCEPT_ENUM_UINT16;



/*****************************************************************************
 ö����    : WPHY_TOOL_MNTN_SYS_STATUS_ENUM_UINT16
 Э����  :
 ö��˵��  : ϵͳ״̬
*****************************************************************************/
enum WPHY_TOOL_MNTN_SYS_STATUS_ENUM
{
    WPHY_MNTN_SYS_STATUS_FREE                   = 0,
    WPHY_MNTN_SYS_STATUS_CPICH                  = 1,
    WPHY_MNTN_SYS_STATUS_FACH                   = 2,
    WPHY_MNTN_SYS_STATUS_DPCH                   = 3,

    WPHY_MNTN_SYS_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MNTN_SYS_STATUS_ENUM_UINT16;



/*****************************************************************************
 ö����    : WPHY_TOOL_MNTN_THRESHOLD_TYPE_ENUM_UINT32
 Э����  :
 ö��˵��  : ��������������
*****************************************************************************/
enum WPHY_TOOL_MNTN_THRESHOLD_TYPE_ENUM
{
    /* С���������� */
    WPHY_MNTN_THRESHOLD_TYPE_STEP1_NOISE                    = 0,
    WPHY_MNTN_THRESHOLD_TYPE_STEP2_NOISE                       ,
    WPHY_MNTN_THRESHOLD_TYPE_STEP3_NOISE_AFC_UNLOCK            ,
    WPHY_MNTN_THRESHOLD_TYPE_STEP3_NOISE_AFC_LOCK              ,
    WPHY_MNTN_THRESHOLD_TYPE_STEPB_NOISE                       ,

    WPHY_MNTN_THRESHOLD_TYPE_STEP1_FAIL_FOR_SPEC_FREQ          ,
    WPHY_MNTN_THRESHOLD_TYPE_STEP1_FAIL_FOR_BAND               ,

    WPHY_MNTN_THRESHOLD_TYPE_STEP1_CORRELEN_FOR_INIT_CS        ,

    WPHY_MNTN_THRESHOLD_TYPE_STEP2_RSDEC_LEN                   ,
    WPHY_MNTN_THRESHOLD_TYPE_STEP3_CORRELEN_AFC_UNLOCK         ,
    WPHY_MNTN_THRESHOLD_TYPE_STEP3_CORRELEN_AFC_LOCK           ,
    WPHY_MNTN_THRESHOLD_TYPE_STEP3_NCORRELEN_AFC_UNLOCK        ,
    WPHY_MNTN_THRESHOLD_TYPE_STEP3_NCORRELEN_AFC_LOCK          ,

    WPHY_MNTN_THRESHOLD_TYPE_STEPB_CORRELEN                    ,

    /*  �ྶ�������� */
    WPHY_MNTN_THRESHOLD_TYPE_CORRE_LENTH_LOCK                  ,
    WPHY_MNTN_THRESHOLD_TYPE_INCORRE_LENTH_LOCK                ,
    WPHY_MNTN_THRESHOLD_TYPE_CORRE_LENTH_UNLOCK                ,
    WPHY_MNTN_THRESHOLD_TYPE_INCORRE_LENTH_UNLOCK              ,

    /* �������� */
    WPHY_MNTN_THRESHOLD_TYPE_MEAS_AVERAGE_NOISE                ,
    WPHY_MNTN_THRESHOLD_TYPE_MP_RELATIVE_FACTOR                ,

    WPHY_MNTN_THRESHOLD_TYPE_MEAS_TIMING_ECNO_THRESHOLD        ,
    WPHY_MNTN_THRESHOLD_TYPE_MEAS_TIMING_RSCP_THRESHOLD        ,

    WPHY_MNTN_THRESHOLD_TYPE_MAX_TIMING_MEAS_FRAME_NUMBER      ,

    /* �������� */
    WPHY_MNTN_THRESHOLD_TYPE_SIR_THRESHOLD_PILOT1              ,
    WPHY_MNTN_THRESHOLD_TYPE_SIR_THRESHOLD_PILOT2              ,

    WPHY_MNTN_THRESHOLD_TYPE_TPC_THRESHOLD_TTI1OR2             ,
    WPHY_MNTN_THRESHOLD_TYPE_TPC_THRESHOLD_TTI4                ,
    WPHY_MNTN_THRESHOLD_TYPE_TPC_THRESHOLD_TTI8                ,

    WPHY_MNTN_THRESHOLD_TYPE_INCORRE_CPU_QIN                   ,
    WPHY_MNTN_THRESHOLD_TYPE_INCORRE_CPU_QOUT                  ,
    WPHY_MNTN_THRESHOLD_TYPE_INCORRE_SS_THRESHOLD              ,
    WPHY_MNTN_THRESHOLD_TYPE_INCORRE_MRC_THRESHOLD             ,
    WPHY_MNTN_THRESHOLD_TYPE_INCORRE_INT_THRESHOLD             ,
    WPHY_MNTN_THRESHOLD_TYPE_INCORRE_ML_THRESHOLD              ,

    /* DRX�������� */
    WPHY_MNTN_THRESHOLD_TYPE_32K_CLOCK_MSR_WAIT_SFN_NUM        ,
    WPHY_MNTN_THRESHOLD_TYPE_SLEEP_LENTH                       ,
    WPHY_MNTN_THRESHOLD_TYPE_TIME_ADJUST                       ,

    WPHY_MNTN_THRESHOLD_TYPE_BUTT
};
typedef VOS_UINT32 WPHY_TOOL_MNTN_THRESHOLD_TYPE_ENUM_UINT32;


/*****************************************************************************
 ö����    : WPHY_TOOL_MNTN_PA_STATUS_ENUM
 Э����  :
 ö��˵��  : ���ؿ��Ƶ�ö��ֵ
*****************************************************************************/
enum WPHY_TOOL_MNTN_PA_STATUS_ENUM
{
    WPHY_MNTN_PA_STATUS_CLOSE                       = 0,                        /* �ر� */
    WPHY_MNTN_PA_STATUS_OPEN                        = 1,                        /* �� */

    WPHY_MNTN_PA_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MNTN_PA_STATUS_ENUM_UINT16;


/*****************************************************************************
 ö����    : WPHY_TOOL_MNTN_RPT_ADDR_ENUM_UINT16
 Э����  :
 ö��˵��  : WPHY��ά�ɲ�̶�ץȡ�ļ�������ö��
*****************************************************************************/
enum WPHY_TOOL_MNTN_RAM_RPT_ENUM
{
    WPHY_MNTN_RPT_HSUPA_SHARE_CTRL                  = 0,                        /* UPA���� */
    WPHY_MNTN_RPT_DSP_MODE                          = 1,                        /* DSPģʽ */
    WPHY_MNTN_RPT_HSUPA_RPT                         = 2,
    WPHY_MNTN_RPT_HSUPA_TTI                         = 3,
    WPHY_MNTN_RPT_CPC_DRX_RPT                       = 4,
    WPHY_MNTN_RPT_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MNTN_RAM_RPT_ENUM_UINT16;


/*****************************************************************************
 ö����    : WPHY_TOOL_MNTN_ERROR_RPT_LEVEL_ENUM_UINT16
 Э����  :
 ö��˵��  : WPHY�����ϱ�����
*****************************************************************************/
enum WPHY_TOOL_MNTN_ERROR_RPT_LEVEL_ENUM
{
    WPHY_MNTN_ERROR_LEVEL_INFO                      = 1,
    WPHY_MNTN_ERROR_LEVEL_WARNING                   = 2,
    WPHY_MNTN_ERROR_LEVEL_ERROR                     = 3,

    WPHY_MNTN_ERROR_LEVEL_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MNTN_ERROR_RPT_LEVEL_ENUM_UINT16;




/*****************************************************************************
 ö����    : WPHY_TOOL_MNTN_PA_STATUS_ENUM
 Э����  :
 ö��˵��  : ���ؿ��Ƶ�ö��ֵ
*****************************************************************************/
enum WPHY_TOOL_MNTN_RPT_STATUS_ENUM
{
    WPHY_MNTN_RPT_STATUS_CLOSE                      = 0,                        /* �ر� */
    WPHY_MNTN_RPT_STATUS_OPEN                       = 1,                        /* �� */

    WPHY_MNTN_RPT_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MNTN_RPT_STATUS_ENUM_UINT16;

/*****************************************************************************
 ö����    : WPHY_TOOL_MNTN_RSLT_ENUM_UINT16
 Э����  :
 ö��˵��  : T313 Status
ASN.1���� :
 ö��˵��  : T313��ʱ�� ״̬
*****************************************************************************/
enum WPHY_TOOL_MNTN_T313_STATUS_ENUM
{
    WPHY_MNTN_T313_START                        = 0,
    WPHY_MNTN_T313_STOP                         = 1,
    WPHY_MNTN_T313_TIMEOUT                      = 2,

    WPHY_MNTN_T313_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MNTN_T313_STATUS_ENUM_UINT16;

/*****************************************************************************
 ö����    : WPHY_TOOL_MNTN_RSLT_ENUM_UINT16
 Э����  :
 ö��˵��  : T312 Status
ASN.1���� :
 ö��˵��  : T312��ʱ�� ״̬
*****************************************************************************/
enum WPHY_TOOL_MNTN_T312_STATUS_ENUM
{
    WPHY_MNTN_T312_START                        = 0,
    WPHY_MNTN_T312_STOP                         = 1,
    WPHY_MNTN_T312_TIMEOUT                      = 2,

    WPHY_MNTN_T312_STATUS_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MNTN_T312_STATUS_ENUM_UINT16;

/*****************************************************************************
 ö����    : WPHY_TOOL_MNTN_MASTER_STOP_LTE_REASION_ENUM
 Э����  :
 ö��˵��  : ǿͣLTEԭ��
ASN.1���� :
 ö��˵��  : ǿͣLTEԭ��

*****************************************************************************/
enum WPHY_TOOL_MNTN_MASTER_STOP_LTE_REASION_ENUM
{
    WPHY_TOOL_MNTN_MASTER_STOP_LTE_REASION_BG               = 0,
    WPHY_TOOL_MNTN_MASTER_STOP_LTE_REASION_MEAS_TIMEOUT     = 1,
    WPHY_TOOL_MNTN_MASTER_STOP_LTE_REASION_BG_TIMEOUT       = 2,
    WPHY_TOOL_MNTN_MASTER_STOP_LTE_REASION_HIGH_TASK        = 3,
    WPHY_TOOL_MNTN_MASTER_STOP_LTE_REASION_INIT             = 4,

    WPHY_TOOL_MNTN_MASTER_STOP_LTE_REASION_BUTT
};
typedef VOS_UINT16 WPHY_TOOL_MNTN_MASTER_STOP_LTE_REASION_ENUM_UINT16;

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
  6.0 STRUCT����,WG��������
*****************************************************************************/

/*****************************************************************************
 �ṹ��    : UPHY_TOOL_TYPE_HEADER_STRU
 ö��˵��  : ���߽����ṹͷ
*****************************************************************************/

typedef struct
{
    VOS_UINT16                                  usFuncType;                     /* ��ά�ɲ���0x4,�ڲ���Ϣץȡ��0x */
    VOS_UINT16                                  usLength;                       /* ֮��ĳ���,��λΪbyte */
    VOS_UINT32                                  ulSn;
    VOS_UINT32                                  ulTimeSlice;
}UPHY_TOOL_TYPE_HEADER_STRU;



/*****************************************************************************
 �ṹ��    : UPHY_TOOL_OM_MSG_HEADER_STRU
 ö��˵��  : ��Ϣͷ�ṹ����
*****************************************************************************/
typedef struct
{
    VOS_UINT16                                  usMsgId;
    VOS_UINT16                                  usReserved;
}UPHY_TOOL_OM_MSG_HEADER_STRU;




/*****************************************************************************
 �ṹ��    : UPHY_TOOL_INTRA_MSG_HEADER_STRU
 ö��˵��  : ��Ϣͷ�ṹ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT16                                  usMsgId;
    VOS_UINT16                                  usReserved;
}UPHY_TOOL_INTRA_MSG_HEADER_STRU;



/*****************************************************************************
  6.1 STRUCT����,UPHY����
*****************************************************************************/



/*****************************************************************************
 �ṹ��    : GPHY_TOOL_SDT_LINK_STATUS_REQ
 �ṹ˵��  : TOOL����GPHY,SDT����״̬
*****************************************************************************/


/*****************************************************************************
 �ṹ��    : GPHY_TOOL_COMMON_CNF_STRU
 �ṹ˵��  : GPHY��ά�ɲ⹫���ظ���Ϣ�ṹ
*****************************************************************************/
typedef struct
{
    UPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip *//* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    UPHY_TOOL_MNTN_RSLT_ENUM_UINT16             enResult;                       /* ����ԭ��Ĵ�����ID */
    VOS_UINT16                                  usRsv;                          /* �����ֶ� */
}UPHY_TOOL_COMMON_CNF_STRU;


/*****************************************************************************
 �ṹ��    : UPHY_TOOL_QUERY_SHARE_ADDR_REQ_STRU
 �ṹ˵��  : �����ַ��ѯ����ԭ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    UPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT32                                  ulParaValue;                    /* �Ƿ���Ƹò��� */
}UPHY_TOOL_QUERY_SHARE_ADDR_REQ_STRU;



/*****************************************************************************
 �ṹ��    : UPHY_TOOL_QUERY_SHARE_ADDR_IND_STRU
 �ṹ˵��  : UPHY�����ַ��ѯ����ظ�
*****************************************************************************/
typedef struct
{
    UPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT32                                  ulShareAddr;                    /* �����׵�ַ */

}UPHY_TOOL_QUERY_SHARE_ADDR_IND_STRU;


/*****************************************************************************
 �ṹ��    : UPHY_TOOL_SDT_LINK_STATUS_REQ_STRU
 �ṹ˵��  : TOOL����UPHY,SDT����״̬
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    UPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSdtStatus;
    VOS_UINT16                                  usRsv;
}UPHY_TOOL_SDT_LINK_STATUS_REQ_STRU;

/*****************************************************************************
 �ṹ��    : UPHY_TOOL_ERROR_REPORT_REQ_STRU
 �ṹ˵��  : �����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    UPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    UPHY_TOOL_MNTN_RPT_LEVEL_ENUM_UINT16        enErrorRptLevel;                /* �����ϱ����󼶱� */
    VOS_UINT16                                  usRsv;
}UPHY_TOOL_ERROR_REPORT_REQ_STRU;



/*****************************************************************************
 �ṹ��    : UPHY_TOOL_ERROR_REPORT_IND_STRU
 �ṹ˵��  : �����ϱ�
*****************************************************************************/
typedef struct
{
    UPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usFileNo;                       /* �ļ������� */
    VOS_UINT16                                  usLineNo;                       /* ������к� */
    VOS_UINT32                                  ulTimeSlice;
    VOS_UINT32                                  ulLogId;                        /* PHY�����ͳһ���LOG ID */
    VOS_UINT16                                  usReserved;                     /* �����ֶ� */
    VOS_UINT16                                  usVarCnt;                       /* �ϱ��������� */
    VOS_UINT32                                  aulVarValue[UPHY_TOOL_MNTN_ERROR_RPT_VAR_NUM]; /* �ϱ�����ֵ */

}UPHY_TOOL_ERROR_REPORT_IND_STRU;



/*****************************************************************************
 �ṹ��    : UPHY_TOOL_WPHY_INTER_GREG_REPORT_REQ_STRU
 �ṹ˵��  : WPHY����ϵͳG�Ĵ����б��·�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    UPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usRsv;                         /* �����ϱ����󼶱� */
    VOS_UINT16                                  usRegNum;                       /* �����ϱ����󼶱� */
    VOS_UINT32                                  aulRegAddr[UPHY_TOOL_INTER_REG_MAX_NUM];
}UPHY_TOOL_WPHY_INTER_GREG_REPORT_REQ_STRU;



/*****************************************************************************
 �ṹ��    : UPHY_TOOL_WPHY_INTER_GREG_REPORT_CNF_STRU
 �ṹ˵��  : WPHY����ϵͳG�Ĵ����յ��ظ�
*****************************************************************************/
typedef struct
{
    UPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    UPHY_TOOL_MNTN_RSLT_ENUM_UINT16             enResult;                       /* ����ԭ��Ĵ�����ID */
    VOS_UINT16                                  usRegNum;                       /* �����ϱ����󼶱� */
    VOS_UINT32                                  aulRegAddr[UPHY_TOOL_INTER_REG_MAX_NUM];
}UPHY_TOOL_WPHY_INTER_GREG_REPORT_CNF_STRU;


/*****************************************************************************
 �ṹ��    : UPHY_TOOL_WPHY_INTER_GREG_REPORT_IND_STRU
 �ṹ˵��  : WPHY����ϵͳG�Ĵ���ֵ�ϱ�
*****************************************************************************/
typedef struct
{
    UPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usRsv;                         /* �����ϱ����󼶱� */
    VOS_UINT16                                  usRegNum;                       /* �����ϱ����󼶱� */
    VOS_UINT32                                  aulRegData[UPHY_TOOL_INTER_REG_MAX_NUM];
}UPHY_TOOL_WPHY_INTER_GREG_REPORT_IND_STRU;



/*****************************************************************************
 �ṹ��    : UPHY_TOOL_WPHY_INTER_GREG_STOP_REQ_STRU
 �ṹ˵��  : ֹͣWPHY����ϵͳG�Ĵ����ϱ�����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    UPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT32                                  ulRsv;                          /* �����ϱ����󼶱� */
}UPHY_TOOL_WPHY_INTER_GREG_STOP_REQ_STRU;




/*****************************************************************************
 �ṹ��    : UPHY_TOOL_GPHY_INTER_WREG_REPORT_REQ_STRU
 �ṹ˵��  : GPHY����ϵͳW�Ĵ����б��·�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    UPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usRsv;                         /* �����ϱ����󼶱� */
    VOS_UINT16                                  usRegNum;                       /* �����ϱ����󼶱� */
    VOS_UINT32                                  aulRegAddr[UPHY_TOOL_INTER_REG_MAX_NUM];
}UPHY_TOOL_GPHY_INTER_WREG_REPORT_REQ_STRU;



/*****************************************************************************
 �ṹ��    : UPHY_TOOL_WPHY_INTER_GREG_REPORT_CNF_STRU
 �ṹ˵��  : GPHY����ϵͳW�Ĵ����յ��ظ�
*****************************************************************************/
typedef struct
{
    UPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    UPHY_TOOL_MNTN_RSLT_ENUM_UINT16             enResult;                       /* ����ԭ��Ĵ�����ID */
    VOS_UINT16                                  usRegNum;                       /* �����ϱ����󼶱� */
    VOS_UINT32                                  aulRegAddr[UPHY_TOOL_INTER_REG_MAX_NUM];
}UPHY_TOOL_GPHY_INTER_WREG_REPORT_CNF_STRU;


/*****************************************************************************
 �ṹ��    : UPHY_TOOL_WPHY_INTER_GREG_REPORT_IND_STRU
 �ṹ˵��  : GPHY����ϵͳW�Ĵ���ֵ�ϱ�
*****************************************************************************/
typedef struct
{
    UPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usRsv;                         /* �����ϱ����󼶱� */
    VOS_UINT16                                  usRegNum;                       /* �����ϱ����󼶱� */
    VOS_UINT32                                  aulRegData[UPHY_TOOL_INTER_REG_MAX_NUM];
}UPHY_TOOL_GPHY_INTER_WREG_REPORT_IND_STRU;



/*****************************************************************************
 �ṹ��    : UPHY_TOOL_GPHY_INTER_WREG_STOP_REQ_STRU
 �ṹ˵��  : ֹͣGPHY����ϵͳW�Ĵ����ϱ�����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    UPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT32                                  ulRsv;                          /* �����ϱ����󼶱� */
}UPHY_TOOL_GPHY_INTER_WREG_STOP_REQ_STRU;


/*****************************************************************************
 �ṹ��    : UPHY_TOOL_DRX_LATE_INFO_IND_STRU
 �ṹ˵��  : ������ʱ�Ĵ����Ϣ
*****************************************************************************/
typedef struct
{
    UPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usRsv;                                          /* �����ֶ� */
    VOS_UINT32                                  ulAwakeTime;                                    /* ��Ҫ���ѵ�ʱ�䣬W��ΪSFN��G��ΪSlice */
    VOS_UINT32                                  ulCurTime;                                      /* ʵ�ʻ��ѵ�ʱ�䣬W��ΪSFN��G��ΪSlice */
    VOS_UINT32                                  aulAwakeHWInfo[UPHY_TOOL_SLEEP_INFO_NUM];       /* OAM��ά�ɲ���Ϣ */
}UPHY_TOOL_DRX_LATE_INFO_IND_STRU;


/*****************************************************************************
  6.2 STRUCT����,GPHY����
*****************************************************************************/

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_COMMON_CNF_STRU
 �ṹ˵��  : GPHY��ά�ɲ⹫���ظ���Ϣ�ṹ
*****************************************************************************/
typedef struct
{
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip *//* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    GPHY_TOOL_MNTN_RSLT_ENUM_UINT16             enResult;                       /* ����ԭ��Ĵ�����ID */
    VOS_UINT16                                  usRsv;                          /* �����ֶ� */
}GPHY_TOOL_COMMON_CNF_STRU;


/*****************************************************************************
 �ṹ��    : GPHY_TOOL_BLER_REPORT_REQ_STRU
 �ṹ˵��  : GPHY GPRS�ŵ����������ϱ�����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usBlerFlag;                     /* �Ƿ��ϱ�BLER */
    VOS_UINT16                                  usRptPeriod;                    /* BLER�ϱ�������,��λ:���� */
}GPHY_TOOL_BLER_REPORT_REQ_STRU;


/*****************************************************************************
 �ṹ��    : GPHY_TOOL_BLER_REPROT_IND_STRU
 �ṹ˵��  : GPHY�ϱ�BLER�Ľṹ���ӽṹ
*****************************************************************************/

typedef struct
{
    VOS_UINT32                                  ulBlkCnt;
    VOS_UINT32                                  ulErrBlkCnt;
}GPHY_BLER_PARA_STRU;

typedef struct
{
    VOS_UINT32                                  ulRcvBlkCnt;
    VOS_UINT32                                  ulDummyCnt;
    GPHY_BLER_PARA_STRU                         stBlerPara[GPHY_PS_MAX_RX_TSN_NUM];
}GPHY_GPRS_BLER_RECORD_STRU;



typedef struct
{
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;
    VOS_UINT16                                  usToolsId;
    GPHY_GPRS_BLER_RECORD_STRU                  stGprsBlerRecord;
}GPHY_TOOL_BLER_REPORT_IND_STRU;



/*****************************************************************************
 �ṹ��    : GPHY_TOOL_SNR_REPORT_REQ_STRU
 �ṹ˵��  : GPHY GPRS�ŵ���SNR�ϱ�����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSNRFlag;                      /* �Ƿ��ϱ�SNR */
    VOS_UINT16                                  usRsv;                          /* �����ֶ� */
}GPHY_TOOL_SNR_REPORT_REQ_STRU;


/*****************************************************************************
 �ṹ��    : GPHY_TOOL_SNR_REPROT_IND_STRU
 �ṹ˵��  : GPHY�ϱ�SNR�Ľṹ���ӽṹ
*****************************************************************************/

typedef struct
{
    VOS_UINT16                                  usRptValidNum;
    VOS_UINT16                                  usRsv;
    VOS_INT16                                   sSNR[GPHY_PS_MAX_RX_TSN_NUM];
    VOS_UINT16                                  usRsv2;
}GPHY_GPRS_SNR_RECORD_STRU;

/* ��ά�ɲ�:SNR�ϱ��ṹ */
typedef struct
{
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;
    VOS_UINT16                                  usToolsId;
    GPHY_GPRS_SNR_RECORD_STRU                   stGprsSNRRecord;
}GPHY_TOOL_SNR_REPORT_IND_STRU;



/*****************************************************************************
 �ṹ��    : GPHY_TOOL_AFC_REPORT_REQ_STRU
 �ṹ˵��  : GPHY AFC����ֵ��Ϣ�ϱ�����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usAFCFlag;                      /* �Ƿ��ϱ�AFCֵ */
    VOS_UINT16                                  usRsv;                          /* �����ֶ� */
}GPHY_TOOL_AFC_REPORT_REQ_STRU;



/*****************************************************************************
 �ṹ��    : GPHY_TOOL_AFC_REPROT_IND_STRU
 �ṹ˵��  : GPHY�ϱ�AFC�Ľṹ
*****************************************************************************/
typedef struct
{
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;
    VOS_UINT16                                  usToolsId;
    VOS_INT16                                   sAFCOffset;
    VOS_UINT16                                  usRsv;
}GPHY_TOOL_AFC_REPORT_IND_STRU;




/*****************************************************************************
 �ṹ��    : GPHY_TOOL_POWER_CTRL_REPORT_REQ_STRU
 �ṹ˵��  : GPHY �����¹�����Ϣ�ϱ�����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usPwrCtrlFlag;                  /* �Ƿ��ϱ����ʿ��� */
    VOS_UINT16                                  usRsv;                          /* �����ֶ� */
}GPHY_TOOL_POWER_CTRL_REPORT_REQ_STRU;



/*****************************************************************************
 �ṹ��    : GPHY_GPRS_POWER_CTRL_REPROT_IND_STRU
 �ṹ˵��  : GPHY�ϱ����ʿ��Ʋ����Ľṹ���ӽṹ
*****************************************************************************/

typedef struct
{
    VOS_UINT16                                  usTxChannel;
    VOS_UINT16                                  usTxTn;
    VOS_INT16                                   sTxPwr;
    VOS_UINT16                                  usGammaTn;
    VOS_UINT16                                  usGammaBand;
    VOS_UINT16                                  usAlpha;
    VOS_UINT16                                  usPMax;
    VOS_UINT16                                  usRsv;
}GPHY_PWR_CTRL_PARA_STRU;

typedef struct
{
    VOS_UINT16                                  usTxCnt;
    VOS_UINT16                                  usRsv;
    GPHY_PWR_CTRL_PARA_STRU                     stPwrCtrlPara[GPHY_PS_MAX_TX_TSN_NUM];
}GPHY_POWER_CONTROL_RECORD_STRU;


typedef struct
{
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;
    VOS_UINT16                                  usToolsId;
    GPHY_POWER_CONTROL_RECORD_STRU              stPwrCtrlRecord;
}GPHY_TOOL_POWER_CTRL_REPORT_IND_STRU;




/*****************************************************************************
 �ṹ��    : GPHY_TOOL_POWER_CTRL_SET_REQ_STRU
 �ṹ˵��  : GPHY ������ָ�����ʷ�����������
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usPwrCtrlFlag;                  /* �Ƿ���ָ�����ʷ��� */
    VOS_UINT16                                  usTxPwr;                        /* ָ������,��λ:0.1dB */
}GPHY_TOOL_POWER_CTRL_SET_REQ_STRU;



/*****************************************************************************
 �ṹ��    : GPHY_GPRS_POWER_CTRL_REPROT_IND_STRU
 �ṹ˵��  : GPHY�ϱ�EGPRS�ŵ������ϱ��ṹ
*****************************************************************************/
typedef struct
{
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usGmskMeanBep;                  /* 0-31,0xffΪ��Чֵ */
    VOS_UINT16                                  us8PSKMeanBep;                  /* 0-31,0xffΪ��Чֵ */
    VOS_UINT16                                  usGmskCvBep;                    /* 0-7,0xffΪ��Чֵ */
    VOS_UINT16                                  us8PSKCvBep;                    /* 0-7,0xffΪ��Чֵ */
}GPHY_TOOL_CHANNEL_QUALITY_IND_STRU;


/*****************************************************************************
 �ṹ��    : GPHY_TOOL_FUNC_BIT_CTRL_REQ_STRU
 �ṹ˵��  : GPHY ��ά�ɲ⹦�ܿ����·��Ľṹԭ��:�������ƹ�����������
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT32                                  ulFuncBitMask;                  /* ��ӦBitλ��λ,����Ƹù�����Ч */
}GPHY_TOOL_FUNC_BIT_CTRL_REQ_STRU;



/*****************************************************************************
 �ṹ��    : GPHY_TOOL_PARA_CONTROL_REQ_STRU
 �ṹ˵��  : GPHY��ά�ɲ���������·��Ľṹԭ��:�������Ʋ�����������
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    GPHY_TOOL_PARA_CTRL_ENUM_UINT16             enParaId;                       /* ������ID,���Ʋ������б� */
    VOS_UINT16                                  usParaCtrlFlg;                  /* �Ƿ���Ƹò��� */
    VOS_UINT32                                  ulParaValue1;                   /* ���Ʋ�����ֵ */
    VOS_UINT32                                  ulParaValue2;                   /* Ŀǰ����,������չ */
}GPHY_TOOL_PARA_CONTROL_REQ_STRU;




/*****************************************************************************
 �ṹ��    : GPHY_TOOL_RF_FRONT_STATIC_REQ_STRU
 �ṹ˵��  : GPHY �ϱ�ǰ�˶�̬��Ϣ�ϱ�����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usMsgRptFlag;                   /* ָʾ��Ϣ��ص��ϱ������Ƿ��б�� */
    VOS_UINT16                                  usRsv;                          /* �����ֶ� */
}GPHY_TOOL_RF_FRONT_STATIC_REQ_STRU;


/*****************************************************************************
 �ṹ��    : GPHY_TOOL_BBP_RTC_REQ_STRU
 �ṹ˵��  : RTC���ܴ򿪺͹ر�����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    UPHY_TOOL_RTC_DEBUG_ENUM_UINT16             enRtcCtrl;                      /* RTC�򿪻��߹ر� */
    VOS_UINT16                                  usRsv;

}GPHY_TOOL_BBP_RTC_REQ_STRU;



/*****************************************************************************
 �ṹ��    : GPHY_TOOL_RF_FRONT_STATIC_IND_STRU
 �ṹ˵��  : GPHY�ϱ�ǰ�˶�̬��Ϣ�ϱ�
*****************************************************************************/
typedef struct
{
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */

    VOS_UINT16                                  usRfTrcvOnState;                /* RF�ܿ��أ�1Ϊ�򿪣�0Ϊ�ر� */
    VOS_UINT16                                  usRfRxOnOrOff;                  /* 04h[1:0] = 3Ϊ�� */
    VOS_UINT16                                  usRxPathselect;                 /* 04h[4:3] = 3Ϊ���ּ�������2������ */
    VOS_UINT16                                  usAbbRxConfig;                  /* 7Ϊ���ּ�������3������ */

    VOS_UINT16                                  usRfTxOnOrOff;                  /* 02h[1:0] = 3Ϊ�� */
    VOS_UINT16                                  usRfTxSHDN;                     /* RF���д򿪿��ƣ�0��BBP���� */

    VOS_UINT16                                  usGpioFuncOn;                   /* Ӳ��֧��GPIO */
    VOS_UINT16                                  usGpioPaMode;                   /* GPIO PA mode */

    VOS_UINT16                                  usMipiFuncOn;                   /* Ӳ��֧��MIPI */
    VOS_UINT16                                  usMipiPaMode;                   /* MIPI PA mode */

    VOS_UINT16                                  usAptFuncOn;                    /* Ӳ��֧��APT */
    VOS_UINT16                                  usAptPaMode;                    /* APT��ѹmode */
}GPHY_TOOL_RF_FRONT_STATIC_IND_STRU;





/*****************************************************************************
 �ṹ��    : GPHY_TOOL_INTRA_MSG_RPT_CTRL_REQ_STRU
 �ṹ˵��  : PHY �ڲ���Ϣ�ϱ���������
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usMsgRptFlag;                   /* ָʾ�ڲ���Ϣ�Ƿ��ϱ� */
    VOS_UINT16                                  usRsv;                          /* �����ֶ� */
}GPHY_TOOL_INTRA_MSG_RPT_CTRL_REQ_STRU;




/*****************************************************************************
 �ṹ��    : GPHY_TOOL_AMR_REPORT_REQ_STRU
 �ṹ˵��  : GPHY ������AMR�ŵ������������ϱ�����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usAmrRptFlag;                   /* �Ƿ��ϱ�AMR��Ϣ */
    VOS_UINT16                                  usRsv;                          /* �����ֶ� */
}GPHY_TOOL_AMR_REPORT_REQ_STRU;


/*****************************************************************************
 �ṹ��    : GPHY_TOOL_AMR_REPORT_IND_STRU
 �ṹ˵��  : GPHY�ϱ�AMR��Ϣ����
*****************************************************************************/
typedef struct
{
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usCrcErrFlg;                    /* ������0-��ȷ;1-���� */
    VOS_UINT16                                  usDecodeMode;                   /* ���б��뷽ʽ */
    VOS_UINT16                                  usDecodeRate;                   /* ���б������� */
    VOS_UINT16                                  usEncodeMode;                   /* ���б��뷽ʽ */
    VOS_UINT16                                  usEncodeRate;                   /* ���б������� */
    VOS_UINT16                                  usRsv;
}GPHY_TOOL_AMR_REPORT_IND_STRU;




/*****************************************************************************
 �ṹ��    : GPHY_TOOL_BLK_REPORT_LIST_REQ_STRU
 �ṹ˵��  : GPHYץȡ�ǹ̶���ַ������
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                  ulBlkAddr;                      /* ץȡ���ݿ���׵�ַ */
    VOS_UINT32                                  ulBlkLen;                       /* ץȡ���ݿ�ĳ���,��λ:32bit */
}GPHY_TOOL_BLK_REPORT_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usRptFlag;
    VOS_UINT16                                  usRsv;
    GPHY_TOOL_BLK_REPORT_STRU                   astRptBlk[GPHY_TOOL_RPT_BLK_MAX_NUM];
}GPHY_TOOL_BLK_REPORT_LIST_REQ_STRU;



/*****************************************************************************
 �ṹ��    : GPHY_TOOL_BLK_REPORT_LIST_IND_STRU
 �ṹ˵��  : GPHYץȡ�ǹ̶���ַ������
*****************************************************************************/

typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    GPHY_TOOL_BLK_REPORT_STRU                   astRptBlk[GPHY_TOOL_RPT_BLK_MAX_NUM];
    VOS_UINT32                                  aulData[2];
}GPHY_TOOL_BLK_REPORT_LIST_IND_STRU;


/*****************************************************************************
 �ṹ��    : GPHY_TOOL_RCV_BURST_INFO_STRU
 �ṹ˵��  : ��ʱ϶������Ϣ�Ľṹ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                              ulMeaFn;                            /* �ϱ������ж�ʱ��֡�� */
    VOS_UINT16                              usMeaArfcn;                         /* �ϱ�������Ƶ�� */
    VOS_UINT16                              usTsMeasType;                       /* �������� */
    VOS_UINT16                              usNextAgcGain;                      /* AGC��λ */
    VOS_INT16                               sAgcRssi;                           /* ����ֵ:0.125dB */
    VOS_UINT32                              ulDrssi;                            /* DRSSI���������ǰ��ֵ�ϱ� */
    VOS_UINT32                              ulFastDrssi;                        /* ���ٲ�����һ���Լ��ڶ���RSSI�ϱ� */
    VOS_UINT32                              ulLsDcValue;
    VOS_UINT32                              ulDemoduFn;                         /* �ϱ������ж�ʱ��֡�� */
    VOS_UINT16                              usDemArfcn;                         /* �ϱ�������Ƶ�� */
    VOS_UINT16                              usNbPos;
    VOS_UINT32                              ulGrossCarrierEn;
    VOS_UINT32                              ulGrossInterfereEn;
    VOS_UINT32                              ulValppCarrierEn;
    VOS_UINT32                              ulValppInterfereEn;
    VOS_UINT32                              ulDemTypeToa;
}GPHY_TOOL_RCV_BURST_INFO_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_RCV_SINGAL_INFO_REQ_STRU
 �ṹ˵��  : ��֡������Ϣ�Ľṹ
*****************************************************************************/
typedef struct
{
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT32                                  ulCurFn;
    VOS_UINT16                                  usAfcValue;
    VOS_INT16                                   sAfcTempComp;
    VOS_UINT32                                  ulReserved[2];
    GPHY_TOOL_RCV_BURST_INFO_STRU               astBurstInfo[GPHY_TOOL_TN_NUM_IN_FRAME];
}GPHY_TOOL_RCV_SINGAL_INFO_REQ_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_REPORT_ERROR_IND_STRU
 �ṹ˵��  : GPHY��Ϣ,�澯,error�ϱ��ṹ
*****************************************************************************/

typedef struct
{
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usFileNo;                       /* �ļ������� */
    VOS_UINT16                                  usLineNo;                       /* ������к� */
    VOS_UINT32                                  ulTimeSlice;
    VOS_UINT32                                  ulLogId;                        /* PHY�����ͳһ���LOG ID */
    VOS_UINT16                                  usReserved;                     /* �����ֶ� */
    VOS_UINT16                                  usVarCnt;                       /* �ϱ��������� */
    VOS_UINT32                                  aulVarValue[GPHY_TOOL_MNTN_ERROR_RPT_VAR_NUM]; /* �ϱ�����ֵ */
}GPHY_TOOL_REPORT_ERROR_IND_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_RCV_SINGAL_INFO_REQ_STRU
 �ṹ˵��  : ��֡������Ϣ�Ľṹ
*****************************************************************************/
typedef struct
{
    GPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_INT16                                   shwCurrTemp;                    /* ��ǰ�¶� */
    VOS_INT16                                   shwXoInitTemp;                  /* У׼�¶� */
    VOS_INT32                                   swTempCompValue;                /* ���μ�����¶Ȳ������ */
    VOS_UINT32                                  auwMantissa[4];  /* DCXO����ʽϵ�� */
    VOS_UINT16                                  auhwExponent[4]; /* DCXO����ʽϵ�� */
}UPHY_TOOL_DCXO_TEMP_COMP_INFO_IND_STRU;


/*****************************************************************************
  6.2 STRUCT����,WPHY����
*****************************************************************************/



/*****************************************************************************
 �ṹ��    : WPHY_TOOL_COMMON_CNF_STRU
 �ṹ˵��  : WPHY��ά�ɲ⹫���ظ���Ϣ�ṹ
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    WPHY_TOOL_MNTN_RSLT_ENUM_UINT16             enResult;                       /* ����ԭ��Ĵ�����ID */
    VOS_UINT16                                  usRsv;                          /* �����ֶ� */
}WPHY_TOOL_COMMON_CNF_STRU;





/*****************************************************************************
 �ṹ��    : WPHY_TOOL_ACT_CELL_REQ_STRU
 �ṹ˵��  : ���С����Ϣ
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_REPORT_TYPE_ENUM_UINT16      enReportType;
    VOS_UINT16                                  usReportPeriod;                /* Ĭ��0����ʶ��Ч�����ϱ�����Ϊָ������ʱ����IE��Ч��*/
}WPHY_MNTN_REPORT_TYPE_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    WPHY_MNTN_REPORT_TYPE_STRU                  stReportType;                   /* Ĭ��ÿ֡�ϱ�  */
}WPHY_TOOL_ACT_CELL_REQ_STRU;



/*****************************************************************************
 �ṹ��    : WPHY_TOOL_ACT_CELL_INFO_IND_STRU
 �ṹ˵��  : ���С����Ϣ
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;
    VOS_UINT16                                  usCfn;
    VOS_UINT16                                  ausScramble[WPHY_MNTN_MAX_ACT_CELL_NUM]; /* ���С�������� */
    VOS_UINT16                                  usServingCellScramble;                   /* ��С������ */
}WPHY_TOOL_ACT_CELL_INFO_IND_STRU;



/*****************************************************************************
 �ṹ��    : WPHY_TOOL_CELL_SEARCH_LIST_REQ_STRU
 �ṹ˵��  : С�������б���Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    WPHY_MNTN_REPORT_TYPE_STRU                  stReportType;                   /* Ĭ���¼������ϱ���ÿ��С�������ϱ�  */
}WPHY_TOOL_CELL_SEARCH_LIST_REQ_STRU;



/*****************************************************************************
 �ṹ��    : WPHY_TOOL_CELL_SEARCH_LIST_IND_STRU
 �ṹ˵��  : С�������б���Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                                  usScramble;
    VOS_UINT16                                  usEnergy;
    VOS_UINT32                                  ulPhase;                        /* ��Ҫת��ΪChip���� */
    VOS_UINT16                                  usLifeCycle;                    /* �������� */
    VOS_UINT16                                  usRsv;
}WPHY_MNTN_CS_LIST_CELL_INFO_STRU;

typedef struct
{
    VOS_UINT16                                  usFreq;
    VOS_UINT16                                  usCnt;
    WPHY_MNTN_CS_LIST_CELL_INFO_STRU            astCellInfo[32];

}WPHY_MNTN_CS_LIST_INFO_STRU;


typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;
    VOS_UINT16                                  usCfn;
    VOS_UINT16                                  usFreqCnt;
    VOS_UINT16                                  usRsv;
    WPHY_MNTN_CS_LIST_INFO_STRU                 astCsList[3];
}WPHY_TOOL_CELL_SEARCH_LIST_IND_STRU;





/*****************************************************************************
 �ṹ��    : WPHY_TOOL_PWR_SWITCH_CTRL_REQ_STRU
 �ṹ˵��  : ���ع��ܿ��ƣ�ָ���򿪻�ر����µĹ��ع���:
               1.���й��ؿ���(����˿��عرգ����ʶ���еĹ��ع���ȫ��ʧЧ,
                  �����ڻ����أ��⻷���أ�Winup,Windown�ȣ�
                  ��ʱ������ͨ���������ò�������������TPC,SirTarget�ȡ�)
               2.�⻷����(����˹��ܹرգ�����ʶ�⻷���ܹرգ���ʱ����ͨ��
                  ���ʲ������ù���������SirTarget��)
               3.Windown
               4.Winup
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    WPHY_TOOL_MNTN_PWR_SWITCH_TYPE_ENUM_UINT16  enPCType;
    WPHY_TOOL_MNTN_SWITCH_CTRL_ENUM_UINT16      enStatus;

}WPHY_TOOL_PWR_SWITCH_CTRL_REQ_STRU;



/*****************************************************************************
 �ṹ��    : WPHY_TOOL_PWR_PARA_SET_REQ_STRU
 �ṹ˵��  : ���ʲ������ã���Mask��ʶ��Ч���
               1.SirTarget
               2.��ʼ���书��
               3.Hold���з��书��(���������С���书��)
               4.����ָ�����͵�����TPC.
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    WPHY_TOOL_MNTN_PWR_PARA_ENUM_UINT32         enPwrParaMask;
    VOS_UINT32                                  ulSirTarget;                    /* SirTarget */
    VOS_INT16                                   sInitTxPwr;                     /* ��ʼ���� */
    VOS_INT16                                   sUlTpc;                         /* ����TPC */
    VOS_INT16                                   sMaxTxPwr;                      /* ����书�� */
    VOS_INT16                                   sMinTxPwr;                      /* ��С���书�� */

}WPHY_TOOL_PWR_PARA_SET_REQ_STRU;





/*****************************************************************************
 �ṹ��    : WPHY_TOOL_MNTN_BLER_REQ_STRU
 �ṹ˵��  : ������ϱ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    WPHY_MNTN_REPORT_TYPE_STRU                  stReportType;                   /* Ĭ��ÿ֡�ϱ�  */

}WPHY_TOOL_BLER_REPORT_REQ_STRU;


/*****************************************************************************
 �ṹ��    : WPHY_MNTN_TRCH_BLER_STRU
 �ṹ˵��  : ������ϱ�����
             ����� = ulErrorBlocks/ulTotalBlocks
*****************************************************************************/
typedef struct
{
    VOS_UINT16                                  usTrchId;
    VOS_UINT16                                  usTti;                          /* ÿ�������ŵ���TTI */
    VOS_UINT32                                  ulErrorBlocks;                  /* �����  */
    VOS_UINT32                                  ulTotalBlocks;                  /* �ܿ��� */

}WPHY_MNTN_TRCH_BLER_STRU;

/*****************************************************************************
 �ṹ��    : WPHY_TOOL_MNTN_BLER_IND_STRU
 �ṹ˵��  : ������ϱ�����
             ÿ���ϱ��Ĵ����ŵ��������̶����������ٸ���CNTȷ����
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;
    VOS_UINT16                                  usCfn;
    VOS_UINT16                                  usCnt;
    VOS_UINT16                                  usRsv;
    WPHY_MNTN_TRCH_BLER_STRU                    astTrchBler[WPHY_MNTN_TRCH_NUM];

}WPHY_TOOL_BLER_REPORT_IND_STRU;







/*****************************************************************************
 �ṹ��    : WPHY_MNTN_EVENT_REPORT_REQ_STRU
 �ṹ˵��  : �¼��ϱ�����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    WPHY_TOOL_MNTN_EVENT_TYPE_ENUM_UINT32       enEventTypeMask;

}WPHY_TOOL_EVENT_REPORT_REQ_STRU;



/*****************************************************************************
 �ṹ��    : WPHY_TOOL_SYNC_STATUS_IND_STRU
 �ṹ˵��  : W��ͬ��ʧ��״̬�ϱ�
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;
    VOS_UINT16                                  usCfn;
    WPHY_TOOL_SYNC_STATUS_ENUM_UINT16           enSyncStatus;
    VOS_UINT16                                  usRsv;
}WPHY_TOOL_SYNC_STATUS_IND_STRU;




/*****************************************************************************
 �ṹ��    : WPHY_TOOL_AFC_STATUS_IND_STRU
 �ṹ˵��  : AFC����
   EVENT_VCTCXO_FREEZE ,EVENT_VCTCXO_UNFREEZE
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;
    VOS_UINT16                                  usCfn;
    WPHY_TOOL_TCXO_STATUS_ENUM_UINT16           enTcxoStatus;
    VOS_UINT16                                  usRsv;
    VOS_UINT32                                  ulAfcStatusValue;

}WPHY_TOOL_AFC_STATUS_IND_STRU;



/*****************************************************************************
 �ṹ��    : WPHY_TOOL_MNTN_INTERRUPT_EXCEPTIION_IND_STRU
 �ṹ˵��  : �ж��쳣�¼��ϱ���
               1.��6ms�������ʱ����ȡ9ms�����Ѿ����
               2.��9ms�������ʱ����ȡSFN�жϸ������Ƿ��Ѿ���֡��
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;
    VOS_UINT16                                  usCfn;

    WPHY_TOOL_MNTN_INT_EXCEPT_ENUM_UINT16       enIntExceptType;                /* �ж��쳣���� */
    VOS_UINT16                                  usRsv;
}WPHY_TOOL_INTERRUPT_EXCEPTIION_IND_STRU;



/*****************************************************************************
 �ṹ��    : WPHY_TOOL_WCDMA_L1_STATUS_IND_STRU
 �ṹ˵��  : �ϱ�L1״̬
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;
    VOS_UINT16                                  usCfn;

    VOS_UINT32                                  ulChannelStatus;                /* �ŵ�״̬ */
    WPHY_TOOL_MNTN_SYS_STATUS_ENUM_UINT16       enSysStatus;                    /* ϵͳ״̬ */
    VOS_UINT16                                  usRsv;
}WPHY_TOOL_WCDMA_L1_STATUS_IND_STRU;





/*****************************************************************************
 �ṹ��    : WPHY_TOOL_DRX_SWITCH_REQ_STRU
 �ṹ˵��  : DRX���ܴ򿪺͹ر�����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    WPHY_TOOL_MNTN_SWITCH_CTRL_ENUM_UINT16      enDrxCtrl;
    VOS_UINT16                                  usRsv;

}WPHY_TOOL_DRX_SWITCH_REQ_STRU;



/*****************************************************************************
 �ṹ��    : WPHY_TOOL_THRESHOLD_SET_REQ_STRU
 �ṹ˵��  : ��������
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    WPHY_TOOL_MNTN_THRESHOLD_TYPE_ENUM_UINT32   enThresholdType;
    VOS_INT32                                   lValue;

}WPHY_TOOL_THRESHOLD_SET_REQ_STRU;


/*****************************************************************************
 �ṹ��    : WPHY_TOOL_THRESHOLD_QUERY_REQ_STRU
 �ṹ˵��  : ��ѯ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    WPHY_TOOL_MNTN_THRESHOLD_TYPE_ENUM_UINT32   enThresholdType;

}WPHY_TOOL_THRESHOLD_QUERY_REQ_STRU;


/*****************************************************************************
 �ṹ��    : WPHY_TOOL_THRESHOLD_QUERY_CNF_STRU
 �ṹ˵��  : ��ѯ
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;
    VOS_UINT16                                  usCfn;
    WPHY_TOOL_MNTN_THRESHOLD_TYPE_ENUM_UINT32   enThresholdType;
    VOS_INT32                                   lValue;

}WPHY_TOOL_THRESHOLD_QUERY_IND_STRU;



/*****************************************************************************
 �ṹ��    : WPHY_TOOL_ERROR_REPORT_REQ_STRU
 �ṹ˵��  : �����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    WPHY_TOOL_MNTN_ERROR_RPT_LEVEL_ENUM_UINT16  enErrorRptLevel;                /* �����ϱ����󼶱� */
    VOS_UINT16                                  usRsv;
}WPHY_TOOL_ERROR_REPORT_REQ_STRU;




/*****************************************************************************
 �ṹ��    : WPHY_TOOL_ERROR_REPORT_IND_STRU
 �ṹ˵��  : �����ϱ�
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usFileNo;                       /* �ļ������� */
    VOS_UINT16                                  usLineNo;                       /* ������к� */
    VOS_UINT32                                  ulTimeSlice;
    VOS_UINT32                                  ulLogId;                        /* PHY�����ͳһ���LOG ID */
    VOS_UINT16                                  usReserved;                     /* �����ֶ� */
    VOS_UINT16                                  usVarCnt;                       /* �ϱ��������� */
    VOS_UINT32                                  aulVarValue[WPHY_TOOL_MNTN_ERROR_RPT_VAR_NUM]; /* �ϱ�����ֵ */

}WPHY_TOOL_ERROR_REPORT_IND_STRU;




/*****************************************************************************
 �ṹ��    : WPHY_TOOL_RF_FRONT_END_PARA_REQ_STRU
 �ṹ˵��  : RFǰ�˲����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */

    WPHY_MNTN_REPORT_TYPE_STRU                  stReportType;                   /* Ĭ��ÿ֡�ϱ�  */

}WPHY_TOOL_RF_FRONT_END_PARA_REQ_STRU;


/*****************************************************************************
 �ṹ��    : WPHY_TOOL_RF_FRONT_END_PARA_IND_STRU
 �ṹ˵��  : �ϱ�TX Temp Comp,Freq Comp,AFC,LNA,HDET
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;
    VOS_UINT16                                  usCfn;
    VOS_INT16                                   sPaHighTempComp;                /* PA������ʱ�¶Ȳ��� */
    VOS_INT16                                   sPaMidTempComp;                 /* PA������ʱ�¶Ȳ��� */
    VOS_INT16                                   sPaLowTempComp;                 /* PA������ʱ�¶Ȳ��� */

    VOS_INT16                                   sPaHighFreqComp;                /* PA������ʱƵ�ʲ��� */
    VOS_INT16                                   sPaMidFreqComp;                 /* PA������ʱƵ�ʲ��� */
    VOS_INT16                                   sPaLowFreqComp;                 /* PA������ʱƵ�ʲ��� */

    VOS_UINT16                                  usAfcStatus;                    /* AFCֵ */
    VOS_UINT16                                  usAnt1AgcStatus;                /* ����1��AGC����״̬ */
    VOS_UINT16                                  usAnt2AgcStatus;                /* ����2��AGC����״̬ */
    VOS_UINT16                                  usHdet;                         /* HDETֵ */
    VOS_UINT16                                  usPaStatus;                     /* PA״̬:�ϱ�����PA��ģʽ������ */
    VOS_UINT16                                  usDbbAtten;                     /* DBB˥�� */
    VOS_UINT16                                  usRfAtten;                      /* RF˥�� */
    VOS_UINT16                                  usReserved;

}WPHY_TOOL_RF_FRONT_END_PARA_IND_STRU;


/*****************************************************************************
 �ṹ��    : WPHY_TOOL_PI_INT_RESULT_IND_STRU
 �ṹ˵��  : WPHY��PI�жϽ���ϱ�
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_INT16                                   sPiResult;
    VOS_UINT16                                  usRsv;
}WPHY_TOOL_PI_INT_RESULT_IND_STRU;

/*****************************************************************************
 �ṹ��    : WPHY_TOOL_AI_INT_RESULT_IND_STRU
 �ṹ˵��  : WPHY��AI�жϽ���ϱ�
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_INT16                                   sAiResult;                      /* AI���,+1��-1��0 */
    VOS_UINT16                                  usReTranNum;                    /* �ش����� */
}WPHY_TOOL_AI_INT_RESULT_IND_STRU;

/*****************************************************************************
 �ṹ��    : T313 Timer Status
 �ṹ˵��  : T313��ʱ��״̬�ϱ�
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    WPHY_TOOL_MNTN_T313_STATUS_ENUM_UINT16      enT313Status;                   /* T313״̬*/
    VOS_UINT16                                  usRsv;
}WPHY_TOOL_T313_STATUS_IND_STRU;

/*****************************************************************************
 �ṹ��    : T312 Timer Status
 �ṹ˵��  : T312��ʱ��״̬�ϱ�
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    WPHY_TOOL_MNTN_T312_STATUS_ENUM_UINT16      enT312Status;                   /* T313״̬*/
    VOS_UINT16                                  usRsv;
}WPHY_TOOL_T312_STATUS_IND_STRU;

/*****************************************************************************
 �ṹ��    : ��ģCDRX��������
 �ṹ˵��  :
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
}WPHY_TOOL_SLAVE_CDRX_START_IND_STRU;

/*****************************************************************************
 �ṹ��    : ��ģCDRX����ֹͣ
 �ṹ˵��  :
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
}WPHY_TOOL_SLAVE_CDRX_STOP_IND_STRU;


/*****************************************************************************
 �ṹ��    : WPHY_TOOL_PA_STATUS_REPORT_REQ_STRU
 �ṹ˵��  : PA״̬�ϱ�����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */

    WPHY_MNTN_REPORT_TYPE_STRU                  stReportType;                   /* Ĭ��ÿ֡�ϱ�  */

}WPHY_TOOL_PA_STATUS_REPORT_REQ_STRU;


/*****************************************************************************
 �ṹ��    : WPHY_TOOL_MASTER_WAKE_LTE_IND_STRU
 �ṹ˵��  : ����LTE��ָʾ
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;                          /* ����ʱ�̵�sfn */
    VOS_UINT16                                  usSlot;                         /* ����ʱ�̵�slot */
    VOS_UINT32                                  ulWakeReason;                   /* ����Ŀ�� */
}WPHY_TOOL_MASTER_WAKE_LTE_IND_STRU;

/*****************************************************************************
 �ṹ��    : WPHY_TOOL_MASTER_START_LTE_MEAS_IND_STRU
 �ṹ˵��  : ����LTE������ָʾ
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;                          /* ����ʱ�̵�sfn */
    VOS_UINT16                                  usSlot;                         /* ����ʱ�̵�slot */
    VOS_UINT32                                  ulStartReason;                  /* ����ԭ�� */
    VOS_UINT32                                  ulMeasTime;                     /* ����ʱ�� us */
    VOS_UINT32                                  ulInt0Int1Time;                 /* int0 int1��� us */
    VOS_UINT32                                  ulAfcLockFlag;
    VOS_UINT32                                  ulAfcValue;
}WPHY_TOOL_MASTER_START_LTE_MEAS_IND_STRU;

/*****************************************************************************
 �ṹ��    : WPHY_TOOL_MASTER_REPORT_LTE_MEAS_IND_STRU
 �ṹ˵��  : ����LTE������ָʾ
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;                          /* �ϱ�ʱ�̵�sfn */
    VOS_UINT16                                  usSlot;                         /* �ϱ�ʱ�̵�slot */
}WPHY_TOOL_MASTER_REPORT_LTE_MEAS_IND_STRU;

/*****************************************************************************
 �ṹ��    : WPHY_TOOL_MASTER_STOP_LTE_MEAS_IND_STRU
 �ṹ˵��  : ǿͣLTE������ָʾ
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16                   enMsgId;                /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                          usToolsId;              /* �����ֶ� */
    WPHY_TOOL_MNTN_MASTER_STOP_LTE_REASION_ENUM_UINT16  enReason;               /* ǿͣԭ�� */
    VOS_UINT16                                          usSfn;                  /* ǿͣʱ�̵�sfn */
    VOS_UINT16                                          usSlot;                 /* ǿͣʱ�̵�slot */
    VOS_UINT16                                          usRsv;
}WPHY_TOOL_MASTER_STOP_LTE_MEAS_IND_STRU;

/*****************************************************************************
 �ṹ��    : WPHY_TOOL_SET_LTE_MEAS_FLAG_IND_STRU
 �ṹ˵��  : ����LTE������ǵ�ָʾ
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usValue;
    VOS_UINT16                                  usRsv;
}WPHY_TOOL_SET_LTE_MEAS_FLAG_IND_STRU;

/*****************************************************************************
 �ṹ��    : WPHY_TOOL_QPC_OPEN_UL_IND_STRU
 �ṹ˵��  : QPC��������·��ָʾ
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;                          /* ����ʱ�̵�sfn */
    VOS_UINT16                                  usSlot;                         /* ����ʱ�̵�slot */
    VOS_UINT32                                  ulOpenReason;                   /* ��ԭ�� */
}WPHY_TOOL_QPC_OPEN_UL_IND_STRU;

/*****************************************************************************
 �ṹ��    : WPHY_TOOL_QPC_CLOSE_UL_IND_STRU
 �ṹ˵��  : QPC�ر�������·��ָʾ
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;                          /* ����ʱ�̵�sfn */
    VOS_UINT16                                  usSlot;                         /* ����ʱ�̵�slot */
    VOS_UINT32                                  ulCloseReason;                  /* �ر�ԭ�� */
}WPHY_TOOL_QPC_CLOSE_UL_IND_STRU;

/*****************************************************************************
 �ṹ��    : WPHY_TOOL_QPC_RRC_STATE_IND_STRU
 �ṹ˵��  : QPC��ǰRRC״̬��ָʾ
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;                          /* ����ʱ�̵�sfn */
    VOS_UINT16                                  usSlot;                         /* ����ʱ�̵�slot */
    VOS_UINT32                                  ulRrcState;                     /* RRC״̬ */
}WPHY_TOOL_QPC_RRC_STATE_IND_STRU;

/*****************************************************************************
 �ṹ��    : WPHY_TOOL_QPC_RB_TYPE_IND_STRU
 �ṹ˵��  : QPC��ǰҵ�����͵�ָʾ CS/PS
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;                          /* ����ʱ�̵�sfn */
    VOS_UINT16                                  usSlot;                         /* ����ʱ�̵�slot */
    VOS_UINT32                                  ulRbType;                       /* RB TYPE */
}WPHY_TOOL_QPC_RB_TYPE_IND_STRU;

/*****************************************************************************
 �ṹ��    : WPHY_TOOL_DPDT_ESTIMATE_STATE_IND_STRU
 �ṹ˵��  : DPDT�����׶ο�ά�ɲ��ϱ�
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usNextState;                    /* ��һ˫����״̬ */
    VOS_INT16                                   sRssi;                          /* �����ߵ�RSSI,�˲���Ľ��,��λΪ0.125dbm */
    VOS_INT16                                   sRscp;                          /* �����ߵ�RSCP,�˲���Ľ��,��λΪ0.125dbm */
    VOS_UINT16                                  usAntStatus;                    /* ��ǰ����״̬,0Ϊ������1Ϊ�ּ� */
}WPHY_TOOL_DPDT_ESTIMATE_STATE_IND_STRU;

/*****************************************************************************
 �ṹ��    : WPHY_TOOL_DPDT_COMPARE_STATE_IND_STRU
 �ṹ˵��  : DPDT�ȽϽ׶ο�ά�ɲ��ϱ�
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usNextState;                    /* ��һ˫����״̬ */
    VOS_UINT16                                  usPhase;                        /* �ȽϽ׶���һ�׶� */
    VOS_INT16                                   sRscp;                          /* ���ּ������˲����RSCP��ֵ,��λΪ0.125dbm */
    VOS_UINT16                                  usAntStatus;                    /* ��ǰ����״̬,0Ϊ������1Ϊ�ּ� */
}WPHY_TOOL_DPDT_COMPARE_STATE_IND_STRU;

/*****************************************************************************
 �ṹ��    : WPHY_TOOL_DPDT_PROTECT_STATE_IND_STRU
 �ṹ˵��  : DPDT���ͽ׶ο�ά�ɲ��ϱ�
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usNextState;                    /* ��һ˫����״̬ */
    VOS_UINT16                                  usAntStatus;                    /* ��ǰ����״̬,0Ϊ������1Ϊ�ּ� */
}WPHY_TOOL_DPDT_PROTECT_STATE_IND_STRU;


/*****************************************************************************
 �ṹ��    : WPHY_TOOL_SLAVE_W_FINISH_IND_STRU
 �ṹ˵��  : ��ģW�������ָʾ
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT32                                  ulRsv;
}WPHY_TOOL_SLAVE_W_FINISH_IND_STRU;

/*****************************************************************************
 �ṹ��    : WPHY_TOOL_SLAVE_GRSSI_FINISH_IND_STRU
 �ṹ˵��  : ��ģW�������ָʾ
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT32                                  ulRsv;
}WPHY_TOOL_SLAVE_GRSSI_FINISH_IND_STRU;

/*****************************************************************************
 �ṹ��    : WPHY_TOOL_SLAVE_GBSIC_FINISH_IND_STRU
 �ṹ˵��  : ��ģG BSIC�������ָʾ
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT32                                  ulRsv;
}WPHY_TOOL_SLAVE_GBSIC_FINISH_IND_STRU;

/*****************************************************************************
 �ṹ��    : WPHY_TOOL_SLAVE_SCHEDULE_TIMEOUT_IND_STRU
 �ṹ˵��  : ��ģ���ȳ�ʱָʾ
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usStartChip;
    VOS_UINT16                                  usStartSlot;
    VOS_UINT16                                  usEndChip;
    VOS_UINT16                                  usEndSlot;
    VOS_UINT16                                  usScheduleTime;                 /* ����ʱ�䣬��λus */
    VOS_UINT16                                  usRsv;
}WPHY_TOOL_SLAVE_SCHEDULE_TIMEOUT_IND_STRU;
/*****************************************************************************
 �ṹ��    : WPHY_TOOL_PA_STATUS_REPORT_IND_STRU
 �ṹ˵��  : PA״̬�ϱ�
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;
    VOS_UINT16                                  usCfn;
    WPHY_TOOL_MNTN_PA_STATUS_ENUM_UINT16        enPaStatus;
    VOS_UINT16                                  usRsv;
}WPHY_TOOL_PA_STATUS_REPORT_IND_STRU;





/*****************************************************************************
 �ṹ��    : WPHY_TOOL_INTRA_MSG_RPT_CTRL_REQ_STRU
 �ṹ˵��  : PHY �ڲ���Ϣ�ϱ���������
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usMsgRptFlag;                   /* ָʾ�ڲ���Ϣ�Ƿ��ϱ� */
    VOS_UINT16                                  usRsv;                          /* �����ֶ� */
}WPHY_TOOL_INTRA_MSG_RPT_CTRL_REQ_STRU;



/*****************************************************************************
 �ṹ��    : WPHY_TOOL_RF_FRONT_END_STATIC_REQUIRE_REQ_STRU
 �ṹ˵��  : ��̬��ѯǰ��״̬����ԭ��
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
}WPHY_TOOL_RF_FRONT_END_STATIC_REQUIRE_REQ_STRU;




/*****************************************************************************
 �ṹ��    : WPHY_TOOL_RF_FRONT_END_STATIC_REQUIRE_IND_STRU
 �ṹ˵��  : ��̬��ѯǰ��״̬�ظ�ԭ��
*****************************************************************************/
typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usRfTrcvOnState;                /* RF�ܿ��أ�1Ϊ�򿪣�0Ϊ�ر� */
    VOS_UINT16                                  usRfRxOnOrOff;                  /* 04h[1:0] = 3Ϊ�� */
    VOS_UINT16                                  usRxPathselect;                 /* 04h[4:3] = 3Ϊ���ּ�������2������ */
    VOS_UINT16                                  usWagcSwitch;                   /* WBBP ���ּ����� */

    VOS_UINT16                                  usAbbRxConfig;                  /* 3Ϊ���ּ�������1������ */
    VOS_UINT16                                  usAbbLineCtrlMode;              /* CH1:[15:8],CH0:[7:0] = 0:IDLE 1:Gģ 2:Wģ 3:LTEģ 4:TDSģ 5:ETģʽ 6:APTģʽ 7:CAģʽ */
    VOS_UINT16                                  usAbbRxMode;                    /* CH1:[15:8],CH0:[7:0] = 0:2G 1:3G_SC 2:4G 3: 3G_DC 4:TDS */
    VOS_UINT16                                  usAbbVoiceData;                 /* CH1:[15:8],CH0:[7:0] = 1:voice mode 2: data mode */

    VOS_UINT16                                  usRfTxOnOrOff;                  /* 02h[1:0] = 3Ϊ�� */
    VOS_UINT16                                  usRfTxSHDN;                     /* RF���д򿪿��ƣ�0��BBP���� */
    VOS_UINT16                                  usTxPathselect;                 /* RFIC TX Patch */
    VOS_UINT16                                  usRfTxAgc;                      /* RFIC TXAGC */

    VOS_UINT16                                  usCpcDrxFuncOn;                 /* ��ǰ������CPC DRX�ı�־ */
    VOS_UINT16                                  usCpcDrxStartCfn;               /* CPC DRX��ʼ������CFN */
    VOS_UINT16                                  usCpcSleepBitMap;               /* ��16bit��1Ϊ����ʱ϶��0Ϊ˯��ʱ϶ */

    VOS_UINT16                                  usGpioFuncOn;                   /* Ӳ��֧��GPIO */
    VOS_UINT16                                  usGpioPaMode;                   /* GPIO PA mode */

    VOS_UINT16                                  usMipiFuncOn;                   /* Ӳ��֧��MIPI */
    VOS_UINT16                                  usMipiPaMode;                   /* MIPI PA mode */

    VOS_UINT16                                  usAptFuncOn;                    /* Ӳ��֧��APT */
    VOS_UINT16                                  usAptPaMode;                    /* APT��ѹmode */
    VOS_UINT16                                  uhwAptVcc;                          /* ���� */
}WPHY_TOOL_RF_FRONT_END_STATIC_REQUIRE_IND_STRU;




/*****************************************************************************
 �ṹ��    : WPHY_TOOL_RF_FRONT_END_DISPLAY_REQ_STRU
 �ṹ˵��  : �Ƿ�ʵʱ���ǰ����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    WPHY_TOOL_MNTN_SWITCH_CTRL_ENUM_UINT16      enDisplayCtrl;
    VOS_UINT16                                  usReserved;
}WPHY_TOOL_RF_FRONT_END_DISPLAY_REQ_STRU;


/*****************************************************************************
 �ṹ��    : WPHY_TOOL_BBP_RTC_REQ_STRU
 �ṹ˵��  : RTC���ܴ򿪺͹ر�����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    UPHY_TOOL_RTC_DEBUG_ENUM_UINT16             enRtcCtrl;                      /* RTC�򿪻��߹ر� */
    VOS_UINT16                                  usRsv;

}WPHY_TOOL_BBP_RTC_REQ_STRU;



/*****************************************************************************
 �ṹ��    : WPHY_TOOL_RAM_REPORT_REQ_STRU
 �ṹ˵��  : WPHYץȡ�̶��黭ͼ��RAM
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    WPHY_TOOL_MNTN_RAM_RPT_ENUM_UINT16          enRptRam;
    WPHY_TOOL_MNTN_RPT_STATUS_ENUM_UINT16       enRptStatus;
}WPHY_TOOL_RAM_REPORT_REQ_STRU;





/*****************************************************************************
 �ṹ��    : WPHY_TOOL_RAM_REPORT_IND_STRU
 �ṹ˵��  : WPHYץȡ�̶��黭ͼ��RAM
*****************************************************************************/

typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;
    VOS_UINT16                                  usCfn;
    WPHY_TOOL_MNTN_RAM_RPT_ENUM_UINT16          enRptRam;
    VOS_UINT16                                  usRsv;
    VOS_UINT32                                  aulData[2];
}WPHY_TOOL_RAM_REPORT_IND_STRU;



/*****************************************************************************
 �ṹ��    : WPHY_TOOL_BLK_REPORT_LIST_REQ_STRU
 �ṹ˵��  : WPHYץȡ�ǹ̶���ַ������
*****************************************************************************/
typedef struct
{
    VOS_UINT32                                  ulBlkAddr;                      /* ץȡ���ݿ���׵�ַ */
    VOS_UINT32                                  ulBlkLen;                       /* ץȡ���ݿ�ĳ���,��λ:32bit */
}WPHY_TOOL_BLK_REPORT_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    WPHY_TOOL_MNTN_RPT_STATUS_ENUM_UINT16       enRptFlag;
    VOS_UINT16                                  usRsv;
    WPHY_TOOL_BLK_REPORT_STRU                   astRptBlk[WPHY_TOOL_RPT_BLK_MAX_NUM];
}WPHY_TOOL_BLK_REPORT_LIST_REQ_STRU;




/*****************************************************************************
 �ṹ��    : WPHY_TOOL_BLK_REPORT_LIST_IND_STRU
 �ṹ˵��  : WPHYץȡ�ǹ̶���ַ������
*****************************************************************************/

typedef struct
{
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    VOS_UINT16                                  usSfn;
    VOS_UINT16                                  usCfn;
    WPHY_TOOL_BLK_REPORT_STRU                   astRptBlk[WPHY_TOOL_RPT_BLK_MAX_NUM];
    VOS_UINT32                                  aulData[2];
}WPHY_TOOL_BLK_REPORT_LIST_IND_STRU;


/*****************************************************************************
 �ṹ��    : WPHY_TOOL_RA_MNTN_CTRL_REQ_STRU
 �ṹ˵��  : ��������ά�ɲ��ϱ�������Ϣ�ṹ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    WPHY_TOOL_MNTN_SWITCH_CTRL_ENUM_UINT16      enRaMntnCtrl;                   /* ��������ά�ɲ�򿪻�ر� */
    VOS_UINT16                                  usReserve;                      /* �����ֶ� */
}WPHY_TOOL_RA_MNTN_CTRL_REQ_STRU;

/*****************************************************************************
 �ṹ��    : WPHY_TOOL_ARM_0MS_MNTN_CTRL_REQ_STRU
 �ṹ˵��  : ARM 0MS��ά�ɲ��ϱ�������Ϣ�ṹ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip */
    WPHY_TOOL_MNTN_MSG_ID_ENUM_UINT16           enMsgId;                        /*_H2ASN_Skip */ /* ԭ������ */
    VOS_UINT16                                  usToolsId;                      /* �����ֶ� */
    WPHY_TOOL_MNTN_SWITCH_CTRL_ENUM_UINT16      enArm0msMntnCtrl;               /* ARM 0MS�򿪻�ر� */
    VOS_UINT16                                  usReserve;                      /* �����ֶ� */
}WPHY_TOOL_ARM_0MS_MNTN_CTRL_REQ_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_GSM_CELL_SNR_REQ_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : ���������������ϱ�����С���������������,����ֵ����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT16                              uhwMsgId;                           /* _H2ASN_Skip */    /* ԭ��ID */
    VOS_UINT16                              uhwToolId;                              /* �����ֶ� */
    VOS_UINT16                              uhwCommand;                             /* ���0:ֹͣ��1������ */
    VOS_UINT16                              uhwResvered;                            /* �����ֶ� */
}GPHY_TOOL_GSM_CELL_SNR_REQ_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_GSM_CELL_SNR_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : ÿ��С���������,����ֵ����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          uhwFreqInfo;                            /* Ƶ����Ϣ�ϱ�����12bit��ʾƵ�Σ�0:850,1:900,2:1800,3:1900����12bit��ʾƵ��ţ�0-1023��*/
    VOS_INT16                           shwRxLev;                               /* �ź�ǿ��ֵ��[-110,-15],��λΪdbm */
    VOS_UINT16                          uhwCellSnr;                             /* ����ȣ�[0,99],��λΪdb */
    VOS_UINT16                          uhwResvered;                            /* �����ֶ� */
}GPHY_TOOL_GSM_CELL_SNR_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_GSM_CELL_SNR_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �����ϱ�����С���������������,����ֵ����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          uhwMsgId;                               /* _H2ASN_Skip *//* ԭ��ID */
    VOS_UINT16                          uhwToolId;                              /* �����ֶ� */
    VOS_UINT16                          uhwCellNum;                             /* Ƶ���� */
    GPHY_TOOL_GSM_CELL_SNR_STRU         astCellInfo[GPHY_TOOL_MA_FREQ_NUM];       /* С����Ϣ */
    VOS_UINT16                          uhwResvered;                            /* �����ֶ� */
}GPHY_TOOL_GSM_CELL_SNR_IND_STRU;


/*****************************************************************************
 �ṹ��    : GPHY_TOOL_BA_LIST_MEAS_REQ_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �����ϱ�BA LIST���ź�ǿ�ȣ�bsic��Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT16                          uhwMsgId;                               /* _H2ASN_Skip *//* ԭ��ID */
    VOS_UINT16                          uhwToolId;                              /* �����ֶ� */
    VOS_UINT16                          uhwCommand;                             /* ���0:ֹͣ��1������ */
    VOS_UINT16                          uhwResvered;                            /* �����ֶ� */
}GPHY_TOOL_BA_LIST_MEAS_REQ_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_CELL_MEAS_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : ÿ��С������Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          uhwFreqInfo;                            /* Ƶ����Ϣ�ϱ�����12bit��ʾƵ�Σ�0:850,1:900,2:1800,3:1900����12bit��ʾƵ��ţ�0-1023��*/
    VOS_INT16                           shwRxLev;                               /* �ź�ǿ��ֵ��[-110,-15],��λΪdbm */
    VOS_UINT16                          uhwBsic;                                /* ��վ��ʶ�룬0-77����Ч0xffff */
    VOS_UINT16                          uhwResvered;                            /* �����ֶ� */
}GPHY_TOOL_CELL_MEAS_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_BA_LIST_MEAS_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �ϱ�BA LIST���ź�ǿ�ȣ�bsic��Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          uhwMsgId;                               /* _H2ASN_Skip *//* ԭ��ID */
    VOS_UINT16                          uhwToolId;                              /* �����ֶ� */
    VOS_UINT16                          uhwScellFreqInfo;                       /* Ƶ����Ϣ�ϱ�����12bit��ʾƵ�Σ�0:850,1:900,2:1800,3:1900����12bit��ʾƵ��ţ�0-1023��*/
    VOS_INT16                           shwScellRxLev;                          /* �ź�ǿ��ֵ��[-110,-15],��λΪdbm */
    VOS_UINT16                          uhwHoppingInd;                          /* ��Ƶָʾ,1:��ʾ��Ƶ,0��ʾ����Ƶ */
    VOS_UINT16                          uhwResvered1;                           /* �����ֶ� */
    VOS_UINT16                          uhwCellNum;                             /* �����ĸ��� */
    VOS_UINT16                          uhwResvered2;                           /* �����ֶ� */
    GPHY_TOOL_CELL_MEAS_STRU            astNeighbourCell[GPHY_TOOL_BA_LIST_MAX_NUM]; /* �����Ĳ�����Ϣ */
}GPHY_TOOL_BA_LIST_MEAS_IND_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_AMR_SETTING_REQ_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �����ϱ�AMR������Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT16                          uhwMsgId;                               /* _H2ASN_Skip *//* ԭ��ID */
    VOS_UINT16                          uhwToolId;                              /* �����ֶ� */
    VOS_UINT16                          uhwCommand;                             /* ���0:ֹͣ��1������ */
    VOS_UINT16                          uhwResvered;                            /* �����ֶ� */
}GPHY_TOOL_AMR_SETTING_REQ_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_AMR_SETTING_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �ϱ�AMR������Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          uhwMsgId;                               /* _H2ASN_Skip *//* ԭ��ID */
    VOS_UINT16                          uhwToolId;                              /* �����ֶ� */
    VOS_UINT16                          uhwAcsNum;                              /* �������,��ΧΪ0-4 */
    VOS_UINT16                          uhwResvered;                            /* �����ֶ� */
    VOS_UINT16                          auhwAcs[GPHY_TOOL_AMR_MAX_ACS_NUM];       /* ������ϣ���Χ0-7 */
    VOS_UINT16                          auhwHyst[GPHY_TOOL_AMR_MAX_ACS_NUM-1];    /* �л����ʹ�С����Χ0-15����λΪ0.5db */
    VOS_UINT16                          auhwThd[GPHY_TOOL_AMR_MAX_ACS_NUM-1];     /* �л����޴�С����ΧΪ0-63����λΪ0.5db */
}GPHY_TOOL_AMR_SETTING_IND_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_AMR_CODEC_USAGE_REQ_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �����ϱ������ÿ���������ʵ�ʹ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT16                          uhwMsgId;                               /* _H2ASN_Skip *//* ԭ��ID */
    VOS_UINT16                          uhwToolId;                              /* �����ֶ� */
    VOS_UINT16                          uhwCommand;                             /* ���0:ֹͣ��1������ */
    VOS_UINT16                          uhwResvered;                            /* �����ֶ� */
}GPHY_TOOL_AMR_CODEC_USAGE_REQ_STRU;


/*****************************************************************************
 �ṹ��    : GPHY_TOOL_AMR_CODEC_USAGE_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �ϱ������ÿ���������ʵ�ʹ����
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          uhwMsgId;                               /* _H2ASN_Skip *//* ԭ��ID */
    VOS_UINT16                          uhwToolId;                              /* �����ֶ� */
    VOS_UINT16                          uhwAcsNum;                              /* �������,��ΧΪ0-4 */
    VOS_UINT16                          uhwResvered;                            /* �����ֶ� */
    VOS_UINT16                          auhwAcs[GPHY_TOOL_AMR_MAX_ACS_NUM];       /* ������ϣ���Χ0-7 */
    VOS_UINT32                          auwUplink[GPHY_TOOL_AMR_MAX_ACS_NUM];     /* ������·�����ÿ�����ʵĿ�������Χ0-2^32-1����λΪ�� */
    VOS_UINT32                          auwDownlink[GPHY_TOOL_AMR_MAX_ACS_NUM];   /* ������·�����ÿ�����ʵĿ�������Χ0-2^32-1����λΪ�� */
}GPHY_TOOL_AMR_CODEC_USAGE_IND_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_AMR_CODEC_STAT_REQ_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : ��������������ʵ�ʹ����
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT16                          uhwMsgId;                               /* _H2ASN_Skip *//* ԭ��ID */
    VOS_UINT16                          uhwToolId;                              /* �����ֶ� */
    VOS_UINT16                          uhwCommand;                             /* ���0:ֹͣ��1������ */
    VOS_UINT16                          uhwResvered;                            /* �����ֶ� */
}GPHY_TOOL_AMR_CODEC_STAT_REQ_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_AMR_CODEC_AVG_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : ÿ���������ʵ�ʹ����
*****************************************************************************/

typedef struct
{
    VOS_UINT32                          auhwUplink[GPHY_TOOL_AMR_CODEC_MAX_NUM];  /* ������·�����ÿ�����ʵĿ�������Χ0-2^32-1����λΪ�� */
    VOS_UINT32                          auhwDownlink[GPHY_TOOL_AMR_CODEC_MAX_NUM];/* ������·�����ÿ�����ʵĿ�������Χ0-2^32-1����λΪ�� */
}GPHY_AMR_CODEC_AVG_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_AMR_CODEC_STAT_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : С�������е�ͳ����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          uhwMsgId;                               /* _H2ASN_Skip *//* ԭ��ID */
    VOS_UINT16                          uhwToolId;                              /* �����ֶ� */
    GPHY_AMR_CODEC_AVG_STRU             stCell;                                 /* С����ͳ����Ϣ */
    GPHY_AMR_CODEC_AVG_STRU             stCall;                                 /* ÿ�����е�ͳ����Ϣ */
}GPHY_TOOL_AMR_CODEC_STAT_IND_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_CS_PS_TX_INFO_REQ_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �����ϱ�CS/PS��������Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT16                          uhwMsgId;                               /* _H2ASN_Skip *//* ԭ��ID */
    VOS_UINT16                          uhwToolId;                              /* �����ֶ� */
    VOS_UINT16                          uhwCommand;                             /* ���0:ֹͣ��1������ */
    VOS_UINT16                          uhwResvered;                            /* �����ֶ� */
}GPHY_TOOL_CS_PS_TX_INFO_REQ_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_CS_PS_TX_INFO_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �ϱ�CS/PS��������Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          uhwMsgId;                               /* _H2ASN_Skip *//* ԭ��ID */
    VOS_UINT16                          uhwToolId;                              /* �����ֶ� */
    VOS_UINT16                          uhwTa;                                  /* ʱ����ǰ������ΧΪ0-63����λΪ���� */
    VOS_UINT16                          uhwSlotNumber;                          /* ʱ϶���� */
    VOS_UINT16                          uhwTxPower[4];                          /* ���з��书�ʣ���ΧΪ0-99����λΪdbm */
    VOS_UINT16                          uhwPcl;                                 /* ��վָ���Ĺ��ʵȼ�����ΧΪ0-31 */
    VOS_UINT16                          uhwResvered;
}GPHY_TOOL_CS_PS_TX_INFO_IND_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_CS_FER_REQ_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �����ϱ�CS FER��Ϣ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT16                          uhwMsgId;                               /* _H2ASN_Skip *//* ԭ��ID */
    VOS_UINT16                          uhwToolId;                              /* �����ֶ� */
    VOS_UINT16                          uhwCommand;                             /* ���0:ֹͣ��1������ */
    VOS_UINT16                          uhwResvered;                            /* �����ֶ� */
}GPHY_TOOL_CS_FER_REQ_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_CS_FER_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �ϱ�CS FER��Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          uhwMsgId;                               /* _H2ASN_Skip *//* ԭ��ID */
    VOS_UINT16                          uhwToolId;                              /* �����ֶ� */
    VOS_UINT32                          uwErrorFrameFull;                       /* full��Ĵ���֡�� */
    VOS_UINT32                          uwTotalFrameFull;                       /* full��֡�� */
    VOS_UINT32                          uwErrorFrameSub;                        /* sub��Ĵ���֡�� */
    VOS_UINT32                          uwTotalFrameSub;                        /* sub��֡�� */
}GPHY_TOOL_CS_FER_IND_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_CS_GET_CURRENT_DSC_REQ_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �����ϱ�������·����ֵ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /* _H2ASN_Skip */
    VOS_UINT16                          uhwMsgId;                               /* _H2ASN_Skip *//* ԭ��ID */
    VOS_UINT16                          uhwToolId;                              /* �����ֶ� */
    VOS_UINT16                          uhwCommand;                             /* ���0:ֹͣ��1������ */
    VOS_UINT16                          uhwResvered;                            /* �����ֶ� */
}GPHY_TOOL_GET_CURRENT_DSC_REQ_STRU;

/*****************************************************************************
 �ṹ��    : GPHY_TOOL_GET_CURRENT_DSC_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �ϱ�������·����ֵ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          uhwMsgId;                               /* _H2ASN_Skip *//* ԭ��ID */
    VOS_UINT16                          uhwToolId;                              /* �����ֶ� */
    VOS_UINT16                          uhwDsc;                                 /* ������·����ֵ */
    VOS_UINT16                          uhwResvered;
}GPHY_TOOL_GET_CURRENT_DSC_IND_STRU;


/*****************************************************************************
 �ṹ��    : GPHY_TOOL_DUAL_ANT_ESTIMATE_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : ���������������׶��о�����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          uhwMsgID;                               /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16                          uhwDualAntCurId;                        /* ����id */
    VOS_UINT16                          uhwDualAntNextState;                    /* ��һ����ת�׶� */
    VOS_UINT16                          uhwRxlevAvgSub;                         /* ������ƽ��ÿ��ͳ������SUB�������ֵ */
}GPHY_TOOL_DUAL_ANT_ESTIMATE_IND_STRU;


/*****************************************************************************
 �ṹ��    : GPHY_TOOL_DUAL_ANT_COMPARE_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �������������ȽϽ׶��о�����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_UINT16                      uhwMsgID;                                   /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16                      uhwDualAntCurId;                            /* �л�ǰ������id */
    VOS_UINT16                      uhwDualAntNextId;                           /* �л��������id */
    VOS_UINT16                      uhwDualAntNextState;                        /* ��һ����ת�׶� */
    VOS_UINT16                      uhwAntCmpNextSubState;                      /* ��һ����ת��״̬ */
    VOS_INT16                       shwRxlevFilterPriAnt;                       /* ������ƽ��ÿ��ͳ�����ڵ������˲�ֵ */
    VOS_INT16                       shwRxlevFilterSecAnt;                       /* ������ƽ��ÿ��ͳ�����ڵ������˲�ֵ */
    VOS_INT16                       shwDeltaRxlev;                              /* ��������������ƽ��ÿ��ͳ�����ڵ������˲���ֵ */
}GPHY_TOOL_DUAL_ANT_COMPARE_IND_STRU;


/*****************************************************************************
 �ṹ��    : GPHY_TOOL_DUAL_ANT_DELAY_IND_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : �����л�ͣ�ͽ׶��о�����ϱ�
*****************************************************************************/
typedef struct
{
    VOS_UINT16                      uhwMsgID;                                   /* _H2ASN_Skip */ /* ԭ��ID */
    VOS_UINT16                      uhwDualAntCurId;                            /* ����id */
    VOS_UINT16                      uhwDualAntNextState;                        /* ��һ����ת�׶� */
    VOS_UINT16                      uhwHandAntEn;                               /* ͣ�ͽ׶������������־ */
}GPHY_TOOL_DUAL_ANT_DELAY_IND_STRU;


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

#endif /* end of phytoolinterface.h */
