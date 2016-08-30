
#ifndef _TAF_SDC_CTX_H_
#define _TAF_SDC_CTX_H_

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include  "PsTypeDef.h"
#include  "ComInterface.h"

#include  "product_config.h"
#include  "NVIM_Interface.h"

#include  "NasNvInterface.h"

#include "omringbuffer.h"

#include "TafNvInterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 �궨��
*****************************************************************************/
#define TAF_SDC_PLATFORM_MAX_RAT_NUM                        (7)                 /* ƽ̨֧�ֵ������뼼����Ŀ */

#if (FEATURE_ON == FEATURE_LTE)
#define TAF_SDC_MAX_RAT_NUM                             (3)                     /* ��ǰ֧�ֵĽ��뼼������ */
#else
#define TAF_SDC_MAX_RAT_NUM                             (2)                     /* ��ǰ֧�ֵ�GU���뼼������ */
#endif


#define TAF_SDC_MAX_IMSI_LEN                                (9)                 /* IMSI����󳤶� */
#define TAF_SDC_IMSI_INVALID                                (0xFF)              /* IMSI����Чֵ�ĺ궨��     */
#define TAF_SDC_INVALID_MCC                                 (0xFFFFFFFF)        /* MCC����Чֵ */
#define TAF_SDC_INVALID_MNC                                 (0xFFFFFFFF)        /* MNC����Чֵ */
#define TAF_SDC_RAC_INVALID                                 (0xFF)              /* RAC��Ϊ��Чֵ */
#define TAF_SDC_CELLID_INVALID                              (0xFFFFFFFF)
#define TAF_SDC_LAC_INVALID                                 (0xFFFF)
#define TAF_SDC_IMSI_INVALID                                (0xFF)                  /* IMSI����Чֵ�ĺ궨��     */

#define TAF_SDC_RPT_CFG_MAX_SIZE                            (8)                 /* �����ϱ���ʶ���ĳ��� */
#define TAF_SDC_MAX_SIGN_THRESHOLD                          (5)                 /* RSSI/CERSSI����ϱ�����ֵ:5DB */


#define TAF_SDC_ASCII2BCDLENGTH(ulAsciiLength)              ((ulAsciiLength + 1) / 2)
#define TAF_SDC_BITS_PER_OCTET                              (8)
#define TAF_SDC_BITS_PER_SEPTET                             (7)
#define TAF_SDC_USSD_7BIT_PAD                               (0x1a)

#define TAF_SDC_EMER_NUM_MAX_LENGTH                         (46)                             /* Emergency Number information������ֽ��� */
#define TAF_SDC_EMER_NUM_LISTS_MAX_NUMBER                   (16)
#define TAF_SDC_NUMOF_EMER_NUMS_NO_USIM                     (8)
#define TAF_SDC_NUMOF_EMER_NUMS_WITH_USIM                   (2)
#define TAF_SDC_MAX_CUSTOM_ECC_NUM                          (20)                             /* �û����ƵĽ���������������� */
#define TAF_SDC_MAX_BCD_NUM_LEN                             (20)                             /* �ú궨���ȡֵ��MN_CALL_MAX_BCD_NUM_LEN��ȡֵ����һ�� */

#define TI_TAF_SDC_WAIT_IMSA_IMS_VOICE_AVAIL_IND_LEN         (35*1000)
#define TAF_SDC_ONE_THOUSAND_MILLISECOND                     (1000)                         /* 1000MS */


/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/


enum TAF_SDC_USSD_TRAN_MODE_ENUM
{
    TAF_SDC_USSD_NON_TRAN_MODE,                                     /* ��͸��:����֧�ֱ���� */
    TAF_SDC_USSD_TRAN_MODE,                                         /* ͸��:  ���岻����룬ֻ��͸�����ɺ�̨������� */
    TAF_SDC_USSD_TRAN_MODE_BUTT
};
typedef VOS_UINT8 TAF_SDC_USSD_TRAN_MODE_ENUM_UINT8;


enum TAF_SDC_ALPHA_TO_ASCII_TRAN_TABLE_ENUM
{
    TAF_SDC_ALPHA_TO_ASCII_TRAN_TABLE_DEF   = 0X0,                          /* Ĭ�ϵ��ַ���ת����*/
    TAF_SDC_ALPHA_TO_ASCII_TRAN_TABLE_TME,                                  /* TEM�ַ���ת��*/
    TAF_SDC_ALPHA_TO_ASCII_TRAN_TABLE_BUTT
};
typedef VOS_UINT8 TAF_SDC_ALPHA_TO_ASCII_TRAN_TABLE_ENUM_UINT8;


enum TAF_SDC_PLATFORM_RAT_TYPE_ENUM
{
    TAF_SDC_PLATFORM_RAT_GSM   = 0X0,
    TAF_SDC_PLATFORM_RAT_WCDMA,
    TAF_SDC_PLATFORM_RAT_LTE,
    TAF_SDC_PLATFORM_RAT_TDS,
    TAF_SDC_PLATFORM_RAT_BUTT
};
typedef VOS_UINT8 TAF_SDC_PLATFORM_RAT_TYPE_ENUM_UINT8;


enum TAF_SDC_INIT_CTX_TYPE_ENUM
{
    TAF_SDC_INIT_CTX_STARTUP                    = 0,
    TAF_SDC_INIT_CTX_POWEROFF                   = 1,
    TAF_SDC_INIT_CTX_BUTT
};
typedef VOS_UINT8 TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8;



enum TAF_SDC_SYS_SUBMODE_ENUM
{
    TAF_SDC_SYS_SUBMODE_NONE                = 0,                                /* �޷��� */
    TAF_SDC_SYS_SUBMODE_GSM                 = 1,                                /* GSMģʽ */
    TAF_SDC_SYS_SUBMODE_GPRS                = 2,                                /* GPRSģʽ */
    TAF_SDC_SYS_SUBMODE_EDGE                = 3,                                /* EDGEģʽ */
    TAF_SDC_SYS_SUBMODE_WCDMA               = 4,                                /* WCDMAģʽ */
    TAF_SDC_SYS_SUBMODE_HSDPA               = 5,                                /* HSDPAģʽ */
    TAF_SDC_SYS_SUBMODE_HSUPA               = 6,                                /* HSUPAģʽ */
    TAF_SDC_SYS_SUBMODE_HSDPA_HSUPA         = 7,                                /* HSDPA+HSUPAģʽ */
    TAF_SDC_SYS_SUBMODE_TD_SCDMA            = 8,                                /* TD_SCDMAģʽ */
    TAF_SDC_SYS_SUBMODE_HSPA_PLUS           = 9,                                /* HSPA+ģʽ */
    TAF_SDC_SYS_SUBMODE_LTE                 = 10,                               /* LTEģʽ */
    TAF_SDC_SYS_SUBMODE_DC_HSPA_PLUS        = 17,                               /* DC-HSPA+ģʽ */
    TAF_SDC_SYS_SUBMODE_DC_MIMO             = 18,                               /* MIMO-HSPA+ģʽ */
    TAF_SDC_SYS_SUBMODE_BUTT
};
typedef VOS_UINT8  TAF_SDC_SYS_SUBMODE_ENUM_UINT8;
enum TAF_SDC_SYS_MODE_ENUM
{
    TAF_SDC_SYS_MODE_GSM,
    TAF_SDC_SYS_MODE_WCDMA,
    TAF_SDC_SYS_MODE_LTE,
    TAF_SDC_SYS_MODE_BUTT
};
typedef VOS_UINT8  TAF_SDC_SYS_MODE_ENUM_UINT8;


enum TAF_SDC_SERVICE_STATUS_ENUM
{
    TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE           = 0,                        /* �������� */
    TAF_SDC_SERVICE_STATUS_LIMITED_SERVICE          = 1,                        /* ���Ʒ��� */
    TAF_SDC_SERVICE_STATUS_LIMITED_SERVICE_REGION   = 2,                        /* ������������� */
    TAF_SDC_SERVICE_STATUS_NO_IMSI                  = 3,                        /* ��imsi */
    TAF_SDC_SERVICE_STATUS_NO_SERVICE               = 4,                        /* �޷��� */
    TAF_SDC_SERVICE_STATUS_NO_CHANGE                = 5,
    TAF_SDC_SERVICE_STATUS_DEEP_SLEEP               = 6,
    TAF_SDC_SERVICE_STATUS_BUTT
};
typedef VOS_UINT8 TAF_SDC_SERVICE_STATUS_ENUM_UINT8;

/*****************************************************************************
    ö����    : TAF_SDC_IMS_NORMAL_REG_STATUS_ENUM
    ö��˵��  : ims����ͨע��״̬
*****************************************************************************/
enum TAF_SDC_IMS_NORMAL_REG_STATUS_ENUM
{
    TAF_SDC_IMS_NORMAL_REG_STATUS_DEREG        = 0x00,
    TAF_SDC_IMS_NORMAL_REG_STATUS_DEREGING     = 0x01,
    TAF_SDC_IMS_NORMAL_REG_STATUS_REGING       = 0x02,
    TAF_SDC_IMS_NORMAL_REG_STATUS_REG          = 0x03,
    
    TAF_SDC_IMS_NORMAL_REG_STATUS_BUTT         = 0xFF
};
typedef VOS_UINT8 TAF_SDC_IMS_NORMAL_REG_STATUS_ENUM_UINT8;



enum TAF_SDC_RPT_SRVSTA_ENUM
{
    TAF_SDC_REPORT_SRVSTA_NO_SERVICE               = 0,                         /* �޷��� */
    TAF_SDC_REPORT_SRVSTA_LIMITED_SERVICE          = 1,                         /* ���Ʒ��� */
    TAF_SDC_REPORT_SRVSTA_NORMAL_SERVICE           = 2,                         /* �������� */
    TAF_SDC_REPORT_SRVSTA_REGIONAL_LIMITED_SERVICE = 3,                         /* ���޵�������� */
    TAF_SDC_REPORT_SRVSTA_DEEP_SLEEP               = 4,                         /* ʡ�����˯��״̬ */
    TAF_SDC_REPORT_SRVSTA_BUTT
};
typedef VOS_UINT8  TAF_SDC_REPORT_SRVSTA_ENUM_UINT8;
enum  TAF_SDC_CELL_RESTRICTION_TYPE_ENUM
{
    TAF_SDC_CELL_RESTRICTION_TYPE_NONE                   = 0,                   /* ��ǰС���������κ�ҵ�� */
    TAF_SDC_CELL_RESTRICTION_TYPE_NORMAL_SERVICE         = 1,                   /* ��ǰС����������ҵ�� */
    TAF_SDC_CELL_RESTRICTION_TYPE_EMERGENCY_SERVICE      = 2,                   /* ��ǰС�����ƽ���ҵ�� */
    TAF_SDC_CELL_RESTRICTION_TYPE_ALL_SERVICE            = 3,                   /* ��ǰС����������ҵ��ͽ���ҵ�� */
    TAF_SDC_CELL_RESTRICTION_TYPE_BUTT
};
typedef VOS_UINT8  TAF_SDC_CELL_RESTRICTION_TYPE_ENUM_UINT8;


enum  TAF_SDC_USIM_STATUS_ENUM
{
    TAF_SDC_USIM_STATUS_UNVALID         = 0,                                    /* ��ʼ����״̬ */
    TAF_SDC_USIM_STATUS_VALID           = 1,                                    /* ��������CS/PS������Ч */
    TAF_SDC_USIM_STATUS_CS_UNVALID      = 2,
    TAF_SDC_USIM_STATUS_PS_UNVALID      = 3,
    TAF_SDC_USIM_STATUS_CS_PS_UNVALID   = 4,
    TAF_SDC_USIM_STATUS_SIM_PIN         = 5,                                    /* SIM����������PIN��ԭ�򲻿��� */
    TAF_SDC_USIM_STATUS_SIM_PUK         = 6,                                    /* SIM����������PUK��ԭ�򲻿��� */
    TAF_SDC_USIM_STATUS_NET_LCOK        = 7,                                    /* SIM������������������ԭ�򲻿��� */
    TAF_SDC_USIM_STATUS_IMSI_LCOK       = 8,                                    /* SIM����������IMSI����ԭ�򲻿��� */

    TAF_SDC_USIM_STATUS_READY_TIMEROUT  = 9,                                    /* �ȴ�usim��ʱ����ʱԭ�򲻿��� */

    TAF_SDC_USIM_STATUS_ROMSIM          = 240,
    TAF_SDC_USIM_STATUS_NO_PRESENT      = 255,
    TAF_SDC_USIM_STATUS_BUTT            = 254
};

typedef VOS_UINT8  TAF_SDC_USIM_STATUS_ENUM_UINT8;
enum TAF_SDC_SIM_TYPE_ENUM
{
    TAF_SDC_SIM_TYPE_SIM                , /* SIM�� */
    TAF_SDC_SIM_TYPE_USIM               , /* USIM�� */
    TAF_SDC_SIM_TYPE_ROM_SIM,
    TAF_SDC_SIM_TYPE_BUTT
};
typedef VOS_UINT8 TAF_SDC_SIM_TYPE_ENUM_UINT8;




enum TAF_SDC_SERVICE_DOMAIN_ENUM
{
    TAF_SDC_SERVICE_DOMAIN_NO_DOMAIN = 0,                                       /* �޷��� */
    TAF_SDC_SERVICE_DOMAIN_CS        = 1,                                       /* ��CS���� */
    TAF_SDC_SERVICE_DOMAIN_PS        = 2,                                       /* ��PS���� */
    TAF_SDC_SERVICE_DOMAIN_CS_PS     = 3,                                       /* PS+CS���� */
    TAF_SDC_SERVICE_DOMAIN_SEARCHING = 4,                                       /* CS��PS��δע�ᣬ����������״̬ */
    TAF_SDC_SERVICE_DOMAIN_BUTT
};
typedef VOS_UINT8  TAF_SDC_SERVICE_DOMAIN_ENUM_UINT8;
enum TAF_SDC_REG_STATUS_ENUM
{
    TAF_SDC_REG_NOT_REGISTERED_NOT_SEARCH,                                      /* δע��δ���� */
    TAF_SDC_REG_REGISTERED_HOME_NETWORK,                                        /* ע�ᣬע���� HOME PLMN */
    TAF_SDC_REG_NOT_REGISTERED_SEARCHING,                                       /* δע�ᣬ��������PLMN�������ڸ��Ź��� */
    TAF_SDC_REG_REGISTERED_DENIED,                                              /* ע�ᱻ�ܾ�����ǰ�û�����������ע�ᣬ�������񱻽�ֹ */
    TAF_SDC_REG_UNKNOWN,                                                        /* ������� */
    TAF_SDC_REG_REGISTERED_ROAM,                                                /* ע�ᣬע���ڷ� HOME PLMN */
    TAF_SDC_REG_STATUS_BUTT
};
typedef VOS_UINT8 TAF_SDC_REG_STATUS_ENUM_UINT8;



enum TAF_SDC_RPT_CMD_INDEX_ENUM
{
    TAF_SDC_RPT_CMD_MODE                     = 0,
    TAF_SDC_RPT_CMD_RSSI,
    TAF_SDC_RPT_CMD_REJINFO,
    TAF_SDC_RPT_CMD_SRVST,
    TAF_SDC_RPT_CMD_PLMNSELEINFO,
    TAF_SDC_RPT_CMD_SIMST,
    TAF_SDC_RPT_CMD_TIME,
    TAF_SDC_RPT_CMD_SMMEMFULL,
    TAF_SDC_RPT_CMD_CTZV,
    TAF_SDC_RPT_CMD_DSFLOWRPT,
    TAF_SDC_RPT_CMD_ORIG,
    TAF_SDC_RPT_CMD_CONF,
    TAF_SDC_RPT_CMD_CONN,
    TAF_SDC_RPT_CMD_CEND,
    TAF_SDC_RPT_CMD_STIN,
    TAF_SDC_RPT_CMD_CERSSI,
    TAF_SDC_RPT_CMD_ANLEVEL,
    TAF_SDC_RPT_CMD_LWCLASH,
    TAF_SDC_RPT_CMD_XLEMA,
    TAF_SDC_RPT_CMD_ACINFO,
    TAF_SDC_RPT_CMD_PLMN,
    TAF_SDC_RPT_CMD_CALLSTATE,

    /* ���䵥�����������ϱ���ö�� */
    TAF_SDC_RPT_CMD_CREG,
    TAF_SDC_RPT_CMD_CGREG,
    TAF_SDC_RPT_CMD_CEREG,
    TAF_SDC_RPT_CMD_CTZR,
    TAF_SDC_RPT_CMD_CUSD,
    TAF_SDC_RPT_CMD_CSSI,
    TAF_SDC_RPT_CMD_CSSU,
    TAF_SDC_RPT_CMD_LWURC,
    TAF_SDC_RPT_CMD_CUUS1I,
    TAF_SDC_RPT_CMD_CUUS1U,
    TAF_SDC_RPT_CMD_NCELLMONITOR,

    TAF_SDC_RPT_CMD_MIPICLK,

    TAF_SDC_RPT_CMD_BUTT
};
typedef VOS_UINT8 TAF_SDC_RPT_CMD_INDEX_ENUM_UINT8;
enum TAF_SDC_CALL_STATE_RPT_STATUS_ENUM
{
    TAF_SDC_CALL_STATE_RPT_STATUS_DISABLE,                 /* �����������ϱ� */
    TAF_SDC_CALL_STATE_RPT_STATUS_ENABLE,                  /* ���������ϱ� */
    TAF_SDC_CALL_STATE_RPT_STATUS_BUTT
};
typedef VOS_UINT8 TAF_SDC_CALL_STATE_RPT_STATUS_ENUM_UINT8;


enum TAF_SDC_CMD_RPT_CTRL_TYPE_ENUM
{
    TAF_SDC_CMD_RPT_CTRL_BY_CURC,               /* �����ϱ���CURC���� */
    TAF_SDC_CMD_RPT_CTRL_BY_UNSOLICITED,        /* �����ϱ��ܵ������������ */
    TAF_SDC_CMD_RPT_CTRL_BUTT
};
typedef VOS_UINT8 TAF_SDC_CMD_RPT_CTRL_TYPE_ENUM_UINT8;


enum TAF_SDC_STATUS_RPT_GENERAL_CTRL_TYPE_ENUM
{
    TAF_SDC_STATUS_RPT_GENERAL_CONTROL_TYPE_NO_REPORT = 0,
    TAF_SDC_STATUS_RPT_GENERAL_CONTROL_TYPE_REPORT    = 1,
    TAF_SDC_STATUS_RPT_GENERAL_CONTROL_TYPE_CUSTOM    = 2,
    TAF_SDC_STATUS_RPT_GENERAL_CONTROL_TYPE_BUTT
};
typedef VOS_UINT8 TAF_SDC_STATUS_RPT_GENERAL_CTRL_TYPE_ENUM_UINT8;


/*****************************************************************************
 ö����    : TAF_SDC_NW_IMS_VOICE_CAP_ENUM_UINT8
 �ṹ˵��  : IMS voice capability
 1.��    ��   : 2013��7��13��
   ��    ��   : y0024524
   �޸�����   : �½�
*****************************************************************************/
enum TAF_SDC_NW_IMS_VOICE_CAP_ENUM
{
    TAF_SDC_NW_IMS_VOICE_NOT_SUPPORTED    = 0,
    TAF_SDC_NW_IMS_VOICE_SUPPORTED        = 1,

    TAF_SDC_NW_IMS_VOICE_BUTT
};
typedef VOS_UINT8 TAF_SDC_NW_IMS_VOICE_CAP_ENUM_UINT8;


enum TAF_SDC_NW_EMC_BS_CAP_ENUM
{
    TAF_SDC_NW_EMC_BS_NOT_SUPPORTED       = 0,
    TAF_SDC_NW_EMC_BS_SUPPORTED           = 1,

    TAF_SDC_NW_EMC_BS_BUTT
};
typedef VOS_UINT8 TAF_SDC_NW_EMC_BS_CAP_ENUM_UINT8;


enum TAF_SDC_LTE_CS_CAPBILITY_ENUM
{
    TAF_SDC_LTE_CS_CAPBILITY_NO_ADDITION_INFO  = 0,
    TAF_SDC_LTE_CS_CAPBILITY_CSFB_NOT_PREFER   = 1,
    TAF_SDC_LTE_CS_CAPBILITY_SMS_ONLY          = 2,
    TAF_SDC_LTE_CS_CAPBILITY_NOT_SUPPORTED     = 3,

    TAF_SDC_LTE_CS_CAPBILITY_BUTT
};
typedef VOS_UINT8 TAF_SDC_LTE_CS_CAPBILITY_ENUM_UINT8;


enum TAF_SDC_VOICE_DOMAIN_ENUM
{
    TAF_SDC_VOICE_DOMAIN_CS_ONLY            = 0,                                /**< CS voice only */
    TAF_SDC_VOICE_DOMAIN_IMS_PS_ONLY        = 1,                                /**< IMS PS voice only */
    TAF_SDC_VOICE_DOMAIN_CS_PREFERRED       = 2,                                /**< CS voice preferred, IMS PS Voice as secondary */
    TAF_SDC_VOICE_DOMAIN_IMS_PS_PREFERRED   = 3,                                /**< IMS PS voice preferred, CS Voice as secondary */

    TAF_SDC_VOICE_DOMAIN_BUTT
};
typedef VOS_UINT32 TAF_SDC_VOICE_DOMAIN_ENUM_UINT32;


enum TAF_SDC_SMS_DOMAIN_ENUM
{
    TAF_SDC_SMS_DOMAIN_NOT_USE_SMS_OVER_IP                  = 0,                /**< not to use SMS over ip */
    TAF_SDC_SMS_DOMAIN_PREFER_TO_USE_SMS_OVER_IP            = 1,                /**< prefer to use SMS over ip, CS/PS SMS as secondary */

    TAF_SDC_SMS_DOMAIN_BUTT
};
typedef VOS_UINT8 TAF_SDC_SMS_DOMAIN_ENUM_UINT8;


enum TAF_SDC_LTE_CS_SERVICE_ENUM
{
    TAF_SDC_LTE_SUPPORT_CSFB_AND_SMS_OVER_SGS = 1,                              /* ֧��cs fallback��sms over sgs*/
    TAF_SDC_LTE_SUPPORT_SMS_OVER_SGS_ONLY,                                      /* ֧��sms over sgs only*/
    TAF_SDC_LTE_SUPPORT_1XCSFB,                                                 /* ֧��1XCSFB */
    TAF_SDC_LTE_SUPPORT_BUTT
};
typedef VOS_UINT8 TAF_SDC_LTE_CS_SERVICE_ENUM_UINT8;
enum TAF_SDC_UE_USAGE_SETTING_ENUM
{
    TAF_SDC_UE_USAGE_VOICE_CENTRIC      = 0,                                    /* �������� */
    TAF_SDC_UE_USAGE_DATA_CENTRIC,                                              /* �������� */

    TAF_SDC_UE_USAGE_BUTT
};
typedef VOS_UINT8 TAF_SDC_UE_USAGE_SETTING_ENUM_UINT8;


enum TAF_SDC_PHONE_MODE_ENUM
{
    TAF_SDC_PHONE_MODE_MINI                = 0,   /* minimum functionality*/
    TAF_SDC_PHONE_MODE_FULL                = 1,   /* full functionality */
    TAF_SDC_PHONE_MODE_TXOFF               = 2,   /* disable phone transmit RF circuits only */
    TAF_SDC_PHONE_MODE_RXOFF               = 3,   /* disable phone receive RF circuits only */
    TAF_SDC_PHONE_MODE_RFOFF               = 4,   /* disable phone both transmit and receive RF circuits */
    TAF_SDC_PHONE_MODE_FT                  = 5,   /* factory-test functionality */
    TAF_SDC_PHONE_MODE_RESET               = 6,   /* reset */
    TAF_SDC_PHONE_MODE_VDFMINI             = 7,   /* mini mode required by VDF*/
    TAF_SDC_PHONE_MODE_POWEROFF            = 8,   /* �ػ��µ�ģʽ */
    TAF_SDC_PHONE_MODE_LOWPOWER            = 9,
    TAF_SDC_PHONE_MODE_BUTT 
};
typedef VOS_UINT8 TAF_SDC_PHONE_MODE_ENUM_UINT8;
enum TAF_SDC_CREG_TYPE_ENUM
{
    TAF_SDC_CREG_TYPE_NOT_REPORT        = 0,
    TAF_SDC_CREG_TYPE_BREVITE           = 1,
    TAF_SDC_CREG_TYPE_ENTIRE            = 2,
    TAF_SDC_CREG_TYPE_BUTT
};
typedef VOS_UINT8 TAF_SDC_CREG_TYPE_ENUM_UINT8;


enum TAF_SDC_CGREG_TYPE_ENUM
{
    TAF_SDC_CGREG_TYPE_NOT_REPORT       = 0,
    TAF_SDC_CGREG_TYPE_BREVITE          = 1,
    TAF_SDC_CGREG_TYPE_ENTIRE           = 2,
    TAF_SDC_CGREG_TYPE_BUTT
};
typedef VOS_UINT8 TAF_SDC_CGREG_TYPE_ENUM_UINT8;


enum TAF_SDC_CEREG_TYPE_ENUM
{
    TAF_SDC_CEREG_TYPE_NOT_REPORT       = 0,
    TAF_SDC_CEREG_TYPE_BREVITE          = 1,
    TAF_SDC_CEREG_TYPE_ENTIRE           = 2,
    TAF_SDC_CEREG_TYPE_BUTT
};
typedef VOS_UINT8 TAF_SDC_CEREG_TYPE_ENUM_UINT8;


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

typedef struct
{
    VOS_UINT8                                     ucRatNum;
    TAF_SDC_PLATFORM_RAT_TYPE_ENUM_UINT8          aenRatList[TAF_SDC_PLATFORM_MAX_RAT_NUM];
}TAF_SDC_PLATFORM_RAT_CAP_STRU;


typedef struct
{
    VOS_UINT8                           ucRatNum;                          
    TAF_SDC_SYS_MODE_ENUM_UINT8         aenRatPrio[TAF_SDC_MAX_RAT_NUM];    
}TAF_SDC_RAT_PRIO_STRU;



typedef struct
{
    VOS_UINT8                           ucGsmImsSupportFlag;    /**< GSM IMSʹ����,VOS_TRUE :֧�֣�VOS_FALSE :��֧�� */
    VOS_UINT8                           ucUtranImsSupportFlag;  /**< UNTRAN IMSʹ����,VOS_TRUE :֧�֣�VOS_FALSE :��֧�� */
    VOS_UINT8                           ucLteImsSupportFlag;    /**< LTE IMSʹ����,VOS_TRUE :֧�֣�VOS_FALSE :��֧�� */
    VOS_UINT8                           ucGsmEmsSupportFlag;    /**< GSM EMSʹ����,VOS_TRUE :֧�֣�VOS_FALSE :��֧�� */
    VOS_UINT8                           ucUtranEmsSupportFlag;  /**< UNTRAN EMSʹ����,VOS_TRUE :֧�֣�VOS_FALSE :��֧�� */
    VOS_UINT8                           ucLteEmsSupportFlag;    /**< LTE EMSʹ����,VOS_TRUE :֧�֣�VOS_FALSE :��֧�� */
    VOS_UINT8                           aucReserved[2];
}TAF_SDC_IMS_RAT_SUPPORT_STRU;


typedef struct
{
    VOS_UINT8                           ucVoiceCallOnImsSupportFlag;            /* IMS����ʹ����,VOS_TRUE :֧�֣�VOS_FALSE :��֧�� */
    VOS_UINT8                           ucSmsOnImsSupportFlag;                  /* IMS����ʹ����,VOS_TRUE :֧�֣�VOS_FALSE :��֧�� */
    VOS_UINT8                           ucVideoCallOnImsSupportFlag;            /* IMS��VIDEO CALLʹ����,VOS_TRUE :֧�֣�VOS_FALSE :��֧�� */
    VOS_UINT8                           ucUssdOnImsSupportFlag;                 /* IMS USSDҵ��ʹ���VOS_TRUE :֧�֣�VOS_FALSE :��֧�� */
}TAF_SDC_IMS_CAPABILITY_STRU;


typedef struct
{
    VOS_UINT8                           ucCallRedial;                           /* call�ز�flag */
    VOS_UINT8                           ucSmsRedial;                            /* SMS �ز�flag */
    VOS_UINT8                           ucSsRedial;                             /* SS  �ز�flag */
    VOS_UINT8                           aucReserve[1];
}TAF_SDC_SWITCH_DOMAIN_REDIAL_CONFIG_STRU;
typedef struct
{
    TAF_SDC_SWITCH_DOMAIN_REDIAL_CONFIG_STRU                stRedialImsToCs;                        /* IMS to CS�ز����ݽṹ  */
    TAF_SDC_SWITCH_DOMAIN_REDIAL_CONFIG_STRU                stRedialCsToIms;                        /* CS to IMS �ز����ݽṹ */
}TAF_SDC_SWITCH_DOMAIN_REDIAL_STRU;


typedef struct
{
    TAF_SDC_IMS_RAT_SUPPORT_STRU        stImsRatSupport;
    TAF_SDC_IMS_CAPABILITY_STRU         stImsCapability;
    TAF_SDC_LTE_CS_SERVICE_ENUM_UINT8   enLteCsServiceCfg;                      /* Lģ֧�ֵ�cs��ҵ������������ */
    TAF_SDC_SMS_DOMAIN_ENUM_UINT8       enSmsDomain;                            /* sms domain preferrece */
    VOS_UINT8                           ucRoamingSupport;
    VOS_UINT8                           aucReserved[1];
    TAF_SDC_SWITCH_DOMAIN_REDIAL_STRU   stRedial;                               /* IMS<-->CS�����ز���־ */
    TAF_SDC_VOICE_DOMAIN_ENUM_UINT32    enVoiceDomain;                          /* voice domain preferrece */

    VOS_UINT32                          ulWaitImsVoiceAvailTimerLen;            /* �ȴ�IMS VOICE ���õĶ�ʱ��ʱ�� */    
}TAF_SDC_IMS_CONFIG_PARA_STRU;
typedef struct
{
    VOS_UINT8                      ucCategoryValue;                                 /* Emergency Service Category Value         */
    VOS_UINT8                      ucEmcNumLen;                                     /* ���������볤�ȣ���aucEmergencyList����Ч����*/
    VOS_UINT8                      aucEmergencyList[TAF_SDC_EMER_NUM_MAX_LENGTH];   /* �������б� */
}TAF_SDC_EMERGENCY_CONTENT_STRU;
typedef struct
{
    VOS_UINT32                         ulMcc;                                               /* MCC */
    VOS_UINT8                          ucEmergencyNumber;                                   /* ���������� */
    VOS_UINT8                          aucReserved[3];
    TAF_SDC_EMERGENCY_CONTENT_STRU     astEmergencyLists[TAF_SDC_EMER_NUM_LISTS_MAX_NUMBER];/* �������б� */
}TAF_SDC_MM_EMERGENCY_LIST_STRU;
typedef struct
{
    VOS_UINT8                           ucCategory;                             /* �������������� */
    VOS_UINT8                           ucValidSimPresent;                      /* �������������п�ʱ��Ч�����޿�ʱ��Ч��1�п�ʱ��Ч��0�޿�ʱ��Ч */
    VOS_UINT8                           ucReserved;
    VOS_UINT8                           ucEccNumLen;                            /* ���������볤�� */
    VOS_UINT8                           aucEccNum[TAF_SDC_MAX_BCD_NUM_LEN];     /* ���������� */
    VOS_UINT32                          ulMcc;                                  /* MCC,3 bytes */
} TAF_SDC_CUSTOM_ECC_NUM_STRU;
typedef struct
{
    VOS_UINT8                           ucEccNumCount;                                  /* �����Ÿ��� */
    VOS_UINT8                           aucReseve[3];                                   
    TAF_SDC_CUSTOM_ECC_NUM_STRU         astCustomEccNumList[TAF_SDC_MAX_CUSTOM_ECC_NUM];/* �������б� */
} TAF_SDC_CUSTOM_ECC_NUM_LIST_STRU;


typedef struct
{
    VOS_UINT8                               ucCustomSetFlg;                     /* VOS_TRUE,����APP�������ö��ƽ��������룬VOS_FALSE,��ʾû�п�ʼ���� */
    VOS_UINT8                               aucReserve[3];
    TAF_SDC_CUSTOM_ECC_NUM_LIST_STRU        stCustomEccNumList;                 /* APP���ƵĽ������б� */
} TAF_SDC_CUSTOM_ECC_CTX_STRU;


typedef struct
{
    VOS_UINT8                                     ucLCEnableFlg;
    TAF_NVIM_LC_RAT_COMBINED_ENUM_UINT8           enRatCombined;
    TAF_NVIM_LC_WORK_CFG_ENUM_UINT8               enLCWorkCfg;
    VOS_UINT8                                     aucReserved[1];
}TAF_SDC_LC_CONFIG_PARA_STRU;


typedef struct
{
    TAF_SDC_PLATFORM_RAT_CAP_STRU       stPlatformRatCap;                       /* ƽ̨���뼼��������Ϣ */

    VOS_UINT8                           ucSvlteSupportFlag;                     /* �Ƿ�֧��SVLTE��־ */
    VOS_UINT8                           aucReserved[3];

    TAF_SDC_IMS_CONFIG_PARA_STRU       stImsConfigPara;                       /* IMS���������Ϣ */

    TAF_SDC_LC_CONFIG_PARA_STRU        stLcConfigPara;

}TAF_SDC_MS_CAP_INFO_STRU;
typedef struct
{
    VOS_UINT32                          ulMcc;
    VOS_UINT32                          ulMnc;
} TAF_SDC_PLMN_ID_STRU;


typedef struct
{
    TAF_SDC_SYS_MODE_ENUM_UINT8         enSysMode;
    TAF_SDC_SYS_SUBMODE_ENUM_UINT8      enSysSubMode;
    VOS_UINT16                          usLac;
    TAF_SDC_PLMN_ID_STRU                stPlmnId;
    VOS_UINT8                           ucRac;
    VOS_UINT8                           ucRoamFlag;                     /* ��ǰפ�������Ƿ����� VOS_TRUE:�������� VOS_FALSE:���������� */
    VOS_UINT8                           ucCampOnFlg;                    /* ��ǰ�Ƿ�פ��,�յ�ϵͳ��Ϣ��Ϊפ��,�յ���������ָʾ��Ϊδפ�� */
    VOS_UINT8                           aucReserved[1];
    VOS_UINT32                          ulCellId;
} TAF_SDC_CAMP_PLMN_INFO_STRU;


typedef struct
{
    TAF_SDC_CELL_RESTRICTION_TYPE_ENUM_UINT8    enCellAcType;                   /* С���������� */
    VOS_UINT8                                   ucRestrictRegister;             /* �Ƿ�����ע��VOS_TRUE:���ƣ�VOS_FALSE:������ */
    VOS_UINT8                                   ucRestrictPagingRsp;            /* �Ƿ�������ӦѰ��VOS_TRUE:���ƣ�VOS_FALSE:������ */
    VOS_UINT8                                   ucReserved;
}TAF_SDC_ACCESS_RESTRICTION_STRU;
typedef struct
{
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8      enCsServiceStatus;                   /* cs����״̬ */
    TAF_SDC_REG_STATUS_ENUM_UINT8          enCsRegStatus;                       /* csע��״̬ */
    VOS_UINT8                              ucSimCsRegStatus;    /* SIM��CS���ע�������µĿ��Ƿ���ЧVOS_TRUE:CS��Ŀ���Ч,VOS_FALSE:CS��Ŀ���Ч*/

    VOS_UINT8                              ucCsAttachAllowFlg;                  /* cs�Ƿ�����ע���ʶ */

    TAF_SDC_ACCESS_RESTRICTION_STRU        stCsAcRestriction;                   /* ��ǰCS������������ */
}TAF_SDC_CS_DOMAIN_INFO_STRU;


typedef struct
{
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8      enPsServiceStatus;                                                            /* ps����״̬ */
    TAF_SDC_REG_STATUS_ENUM_UINT8          enPsRegStatus;                       /* psע��״̬ */
    VOS_UINT8                              ucSimPsRegStatus;                    /* SIM��PS���ע�������µĿ��Ƿ���ЧVOS_TRUE:CS��Ŀ���Ч,VOS_FALSE:CS��Ŀ���Ч*/
    VOS_UINT8                              ucPsAttachAllowFlg;                  /* ps�Ƿ�����ע���ʶ */
	TAF_SDC_ACCESS_RESTRICTION_STRU        stPsAcRestriction;                   /* ��ǰPS������������ */

}TAF_SDC_PS_DOMAIN_INFO_STRU;
typedef struct
{
    TAF_SDC_IMS_NORMAL_REG_STATUS_ENUM_UINT8   enImsNormalRegSta;
    VOS_UINT8                                  ucImsAvail;
    VOS_UINT8                                  aucReserved[2];
}TAF_SDC_IMS_DOMAIN_INFO_STRU;




typedef struct
{
    TAF_SDC_USIM_STATUS_ENUM_UINT8      enSimStatus;
    VOS_UINT8                           aucReserved[3];
}TAF_SDC_SIM_STATUS_STRU;


typedef struct
{
    VOS_UINT8                           aucImsi[TAF_SDC_MAX_IMSI_LEN];
	
	VOS_UINT8                           aucLastImsi[TAF_SDC_MAX_IMSI_LEN];
    VOS_UINT8                           aucReserved[2];
}TAF_SDC_SIM_MS_IDENTITY_STRU;



typedef struct
{
    TAF_SDC_SIM_STATUS_STRU             stSimStatus;                            /* SIM��״̬ */
    TAF_SDC_SIM_MS_IDENTITY_STRU        stMsIdentity;                           /* MS Identity��Ϣ */
    TAF_SDC_SIM_TYPE_ENUM_UINT8         enSimType;                              /* SIM������ */
    VOS_UINT8                           aucRsv[3];
}TAF_SDC_SIM_INFO_STRU;
typedef struct
{
    TAF_SDC_STATUS_RPT_GENERAL_CTRL_TYPE_ENUM_UINT8   enStatusRptGeneralControl; /* �����Ƿ���������˽������������ϱ� */
    VOS_UINT8                                         aucReserved[3];
    VOS_UINT8                                         aucRptCfg[TAF_SDC_RPT_CFG_MAX_SIZE]; /* 64bit�����ϱ���ʶ */
}TAF_SDC_CURC_RPT_CTRL_STRU;


typedef struct
{
    VOS_UINT8                                         aucRptCfg[TAF_SDC_RPT_CFG_MAX_SIZE]; /* 64bit�����ϱ���ʶ */
}TAF_SDC_UNSOLICITED_RPT_CTRL_STRU;
typedef struct
{
    TAF_SDC_CURC_RPT_CTRL_STRU              stCurcRptCtrl;                      /* CURC���������ϱ� */
    TAF_SDC_UNSOLICITED_RPT_CTRL_STRU       stUnsolicitedRptCtrl;               /* ����������������ϱ� */
}TAF_SDC_RPT_CTRL_STRU;



typedef struct
{
    VOS_UINT8                           ucUssdTransMode;
    VOS_UINT8                           ucAlphaTransMode;
    VOS_UINT8                           aucRsv[2];
}TAF_SDC_USSD_CFG_STRU;



typedef struct
{
    VOS_UINT8                                   ucUtranSkipWPlmnSearchFlag;
    VOS_UINT8                                   aucReserved[3];
}TAF_SDC_DSDA_PLMN_SEARCH_ENHANCED_CFG_STRU;

typedef struct
{
    TAF_SDC_RPT_CTRL_STRU               stRptCtrl;                              /* ���������ϱ���Ϣ */
    TAF_SDC_USSD_CFG_STRU               stUssdCfg;                              /* USSD������Ϣ */
    VOS_UINT16                          usAppCfgSupportType;                    /*����Ӧ�ð汾*/
    TAF_SDC_UE_USAGE_SETTING_ENUM_UINT8 enUeUsageSetting;                       /* UE's usage setting */
    VOS_UINT8                           aucReserve[1];
    
    TAF_SDC_RAT_PRIO_STRU               stPrioRatList;                          /* ���뼼���Լ����ȼ� */
	
    TAF_SDC_DSDA_PLMN_SEARCH_ENHANCED_CFG_STRU  stDsdaPlmnSearchEnhancedCfg;
}TAF_SDC_MS_SYS_CFG_INFO_STRU;
typedef struct
{
    TAF_SDC_MS_CAP_INFO_STRU            stMsCapInfo;                            /* MS��������Ϣ */
    TAF_SDC_MS_SYS_CFG_INFO_STRU        stMsSysCfgInfo;                         /* ϵͳ������Ϣ */
}TAF_SDC_MS_CFG_INFO_STRU;


typedef struct
{
    TAF_SDC_NW_IMS_VOICE_CAP_ENUM_UINT8   enNwImsVoCap;
    TAF_SDC_NW_EMC_BS_CAP_ENUM_UINT8      enNwEmcBsCap;
    TAF_SDC_LTE_CS_CAPBILITY_ENUM_UINT8   enLteCsCap;
    VOS_UINT8                             aucReserved[1];
}
TAF_SDC_NETWORK_CAP_INFO_STRU;


typedef struct
{
    TAF_SDC_CAMP_PLMN_INFO_STRU         stCampPlmnInfo;                         /* ��ǰפ��PLMN��Ϣ */
    TAF_SDC_CS_DOMAIN_INFO_STRU         stCsDomainInfo;                         /* ��ǰCS����Ϣ */
    TAF_SDC_PS_DOMAIN_INFO_STRU         stPsDomainInfo;                         /* ��ǰPS����Ϣ */
    TAF_SDC_REPORT_SRVSTA_ENUM_UINT8    enReportSrvsta;                         /* UE����״̬^srvst�ϱ��ķ���״̬ */
    TAF_SDC_SERVICE_DOMAIN_ENUM_UINT8   enServiceDomain;                        /* UE������sysinfo��ѯʱ�ϱ��ķ����� */
    VOS_UINT8                           aucReserved[2];
    TAF_SDC_NETWORK_CAP_INFO_STRU       stGuNwCapInfo;                          /* GU������ҵ��������Ϣ */
    TAF_SDC_NETWORK_CAP_INFO_STRU       stLteNwCapInfo;                         /* LTE������ҵ��������Ϣ */

    TAF_SDC_IMS_DOMAIN_INFO_STRU        stImsDomainInfo;                        /* IMS���������Ϣ */    

}TAF_SDC_NETWORK_INFO_STRU;



typedef struct
{
    VOS_UINT8                           ucCsCallExistFlg;                       /* ��ǰ�Ƿ��ں��й�����:VOS_TRUE  ��ǰ���ں��й�����
                                                                                                  VOS_FALSE ��ǰ�����ں��й�����  */
    VOS_UINT8                           ucImsCallExistFlg;                      /* ��ǰ�Ƿ��ں��й�����:VOS_TRUE  ��ǰ���ں��й�����
                                                                                                  VOS_FALSE ��ǰ�����ں��й�����  */
    /* ɾ��SRVCC��־���Ƶ�CALLģ�� */
    VOS_UINT8                           aucReserved[2];

    TAF_SDC_MM_EMERGENCY_LIST_STRU      stMmCallEmerNumList;                    /* MM�������Ľ������б� */
    TAF_SDC_CUSTOM_ECC_CTX_STRU         stCustomCallEccNumCtx;                  /* ��Ӫ�̶��ƵĽ������б� */

}TAF_SDC_CALL_INFO_STRU;
typedef struct
{
    VOS_UINT8                           ucCsSmsSrvExistFlg;                     /* CS��Ķ���ҵ����ڱ�ʶ:VOS_TRUE  ��ǰ��CS��Ķ���ҵ��
                                                                                   VOS_FALSE ��ǰû��CS��Ķ���ҵ��  */
    VOS_UINT8                           ucPsSmsSrvExistFlg;                     /* PS��Ķ���ҵ����ڱ�ʶ:VOS_TRUE  ��ǰ��PS��Ķ���ҵ��
                                                                                   VOS_FALSE ��ǰû��PS��Ķ���ҵ��  */
    VOS_UINT8                           ucImsSmsSrvExistFlg;                    /* IMS��Ķ���ҵ����ڱ�ʶ:VOS_TRUE  ��ǰ��IMS��Ķ���ҵ��
                                                                                   VOS_FALSE ��ǰû��IMS��Ķ���ҵ��  */
    VOS_UINT8                           aucReserved[1];
}TAF_SDC_SMS_INFO_STRU;


typedef struct
{
    VOS_UINT8                           ucCsSsSrvExistFlg;                      /* CS��Ĳ���ҵ����ڱ�ʶ:VOS_TRUE  ��ǰ��CS��Ĳ���ҵ��
                                                                                   VOS_FALSE ��ǰû��CS��Ĳ���ҵ��  */
    VOS_UINT8                           ucImsSsSrvExistFlg;                     /* IMS��Ĳ���ҵ����ڱ�ʶ:VOS_TRUE  ��ǰ��IMS��Ĳ���ҵ��
                                                                                   VOS_FALSE ��ǰû��IMS��Ĳ���ҵ��  */
    VOS_UINT8                           aucReserved[2];
}TAF_SDC_SS_INFO_STRU;



typedef struct
{
    VOS_UINT8                           ucCsServiceConnStatusFlag;              /* csҵ�����������Ƿ���ڱ�ʶ */
    VOS_UINT8                           ucReserved[3];                                                                                             VOS_UINT8                           aucReserved[2];
}TAF_SDC_CS_INFO_STRU;
typedef struct
{
    TAF_SDC_CALL_INFO_STRU              stCallInfo;                               /* CC�����Ϣ */
    TAF_SDC_SMS_INFO_STRU               stSmsInfo;                              /* SMS�����Ϣ */
    TAF_SDC_SS_INFO_STRU                stSsInfo;                               /* SS�����Ϣ */

    TAF_SDC_CS_INFO_STRU                stCsInfo;                                                                                      VOS_UINT8                           aucReserved[2];
}TAF_SDC_SERVICE_INFO_STRU;



typedef struct
{
    /* TAF��Ĺ����� */
    OM_RING_ID                          pstRingBuffer;
    VOS_UINT8                           ucErrLogCtrlFlag;                       /* ERRLOG�򿪱�ʶ */
    VOS_UINT8                           ucReserved;
    VOS_UINT16                          usAlmLevel;                             /* ���ϸ澯���� */
}TAF_SDC_ERRLOG_INFO_STRU;
typedef struct
{
    TAF_SDC_PHONE_MODE_ENUM_UINT8       enPhMode;
    VOS_UINT8                           aucRsv[3];
}TAF_SDC_PHONE_INFO_STRU;


typedef struct
{
    TAF_SDC_CREG_TYPE_ENUM_UINT8        enCregType;
    TAF_SDC_CGREG_TYPE_ENUM_UINT8       enCgregType;
    TAF_SDC_CEREG_TYPE_ENUM_UINT8       enCeregType;
    VOS_UINT8                           aucRsv;
}TAF_SDC_REG_REPORT_STATUS_STRU;




typedef struct
{
    TAF_SDC_SIM_INFO_STRU               stSimInfo;                              /* �������Ϣ */
    TAF_SDC_MS_CFG_INFO_STRU            stMsCfgInfo;                            /* MS��������Ϣ */
    TAF_SDC_NETWORK_INFO_STRU           stNetworkInfo;                          /* ��ǰפ����������Ϣ */
    TAF_SDC_SERVICE_INFO_STRU           stServiceInfo;                          /* ��ǰҵ�������Ϣ */

    TAF_SDC_PHONE_INFO_STRU             stPhoneInfo;

    TAF_SDC_ERRLOG_INFO_STRU            stErrlogInfo;

    TAF_SDC_REG_REPORT_STATUS_STRU      stRegReportStatus;

}TAF_SDC_CTX_STRU;


typedef struct
{
    VOS_UINT8                           ucSignThreshold;                        /* �źű仯����,��RSSI�仯������ֵ���������Ҫ�����ϱ��ź�������ȡֵ0��ʾ����㰴Ĭ��ֵ���� */
    VOS_UINT8                           ucMinRptTimerInterval;                  /* ����ϱ���ʱ��   */
} TAF_SDC_CELL_SIGN_REPORT_CFG_STRU;

/* call state NVɾ����call state���ϱ���curcͳһ���� */


/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/


/*****************************************************************************
  10 ��������
*****************************************************************************/
TAF_SDC_CTX_STRU*  TAF_SDC_GetSdcCtx( VOS_VOID );

TAF_SDC_MS_CAP_INFO_STRU*  TAF_SDC_GetMsCapInfo( VOS_VOID );

TAF_SDC_PLATFORM_RAT_CAP_STRU*  TAF_SDC_GetPlatformRatCap( VOS_VOID );

TAF_SDC_CURC_RPT_CTRL_STRU*  TAF_SDC_GetCurcRptCtrl( VOS_VOID );
TAF_SDC_UNSOLICITED_RPT_CTRL_STRU*  TAF_SDC_GetUnsolicitedRptCtrl( VOS_VOID );


TAF_SDC_NETWORK_CAP_INFO_STRU *TAF_SDC_GetGuNwCapInfo(VOS_VOID);


TAF_SDC_NETWORK_CAP_INFO_STRU *TAF_SDC_GetLteNwCapInfo(VOS_VOID);


VOS_VOID TAF_SDC_SetGuNwCapInfo(TAF_SDC_NETWORK_CAP_INFO_STRU *pstNwCapInfo);


VOS_VOID TAF_SDC_SetLteNwCapInfo(TAF_SDC_NETWORK_CAP_INFO_STRU *pstNwCapInfo);

VOS_UINT32 TAF_SDC_IsPlatformRatCapNvimValid(
    PLATAFORM_RAT_CAPABILITY_STRU      *pstNvPlatformRatCap
);

TAF_SDC_PLATFORM_RAT_TYPE_ENUM_UINT8 TAF_SDC_ConvertNvPlatformRatToTafType(
    PLATFORM_RAT_TYPE_ENUM_UINT16       enNvPlatformRatCap
);

VOS_VOID TAF_SDC_InitPlatformRatCap(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8     enInitType,
    TAF_SDC_PLATFORM_RAT_CAP_STRU       *pstPlatformRatCap
);

VOS_VOID TAF_SDC_InitMsCapInfo(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8     enInitType,
    TAF_SDC_MS_CAP_INFO_STRU            *pstMsCapInfo
);

VOS_VOID TAF_SDC_InitCtx(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8     enInitType,
    TAF_SDC_CTX_STRU                    *pstSdcCtx
);

TAF_SDC_SYS_MODE_ENUM_UINT8 TAF_SDC_GetSysMode( VOS_VOID );
VOS_VOID TAF_SDC_SetSysMode(TAF_SDC_SYS_MODE_ENUM_UINT8 enSysMode);
TAF_SDC_SYS_SUBMODE_ENUM_UINT8 TAF_SDC_GetSysSubMode( VOS_VOID );
VOS_VOID TAF_SDC_SetSysSubMode(TAF_SDC_SYS_SUBMODE_ENUM_UINT8 enSysSubMode);
TAF_SDC_CAMP_PLMN_INFO_STRU* TAF_SDC_GetCampPlmnInfo( VOS_VOID );
TAF_SDC_SERVICE_STATUS_ENUM_UINT8 TAF_SDC_GetCsServiceStatus( VOS_VOID );
VOS_VOID TAF_SDC_SetCsServiceStatus(TAF_SDC_SERVICE_STATUS_ENUM_UINT8 enCsServiceStatus);
TAF_SDC_SERVICE_STATUS_ENUM_UINT8 TAF_SDC_GetPsServiceStatus( VOS_VOID );
VOS_VOID TAF_SDC_SetPsServiceStatus(TAF_SDC_SERVICE_STATUS_ENUM_UINT8 enPsServiceStatus);
TAF_SDC_SERVICE_DOMAIN_ENUM_UINT8 TAF_SDC_GetServiceDomain( VOS_VOID );
VOS_VOID TAF_SDC_SetServiceDomain(TAF_SDC_SERVICE_DOMAIN_ENUM_UINT8 enServiceDomain);
TAF_SDC_REG_STATUS_ENUM_UINT8 TAF_SDC_GetCsRegStatus( VOS_VOID );
VOS_VOID TAF_SDC_SetCsRegStatus(TAF_SDC_REG_STATUS_ENUM_UINT8 enCsRegStatus);
TAF_SDC_REG_STATUS_ENUM_UINT8 TAF_SDC_GetPsRegStatus( VOS_VOID );
VOS_VOID TAF_SDC_SetPsRegStatus(TAF_SDC_REG_STATUS_ENUM_UINT8 enPsRegStatus);
VOS_UINT8* TAF_SDC_GetSimImsi( VOS_VOID );
VOS_VOID TAF_SDC_SetSimImsi( VOS_UINT8 *pstImsi );

VOS_UINT8* TAF_SDC_GetLastSimImsi( VOS_VOID );
VOS_VOID TAF_SDC_SetLastSimImsi( VOS_UINT8 *pstImsi );

TAF_SDC_USIM_STATUS_ENUM_UINT8 TAF_SDC_GetSimStatus( VOS_VOID );
VOS_VOID TAF_SDC_SetSimStatus(TAF_SDC_USIM_STATUS_ENUM_UINT8 enSimStatus);
VOS_VOID TAF_SDC_SetSimType(TAF_SDC_SIM_TYPE_ENUM_UINT8 enSimType);
TAF_SDC_SIM_TYPE_ENUM_UINT8 TAF_SDC_GetSimType( VOS_VOID );
VOS_VOID TAF_SDC_SetCurrCampPlmnId(TAF_SDC_PLMN_ID_STRU *pstPlmnId);
TAF_SDC_PLMN_ID_STRU *TAF_SDC_GetCurrCampPlmnId(VOS_VOID);


VOS_UINT16 TAF_SDC_GetCurrCampLac(VOS_VOID);

VOS_UINT8 TAF_SDC_GetCurrCampRac(VOS_VOID);
VOS_UINT32 TAF_SDC_GetCurrCampCellId(VOS_VOID);

TAF_SDC_REPORT_SRVSTA_ENUM_UINT8 TAF_SDC_GetServiceStatus(VOS_VOID);

VOS_VOID TAF_SDC_SetServiceStatus(TAF_SDC_REPORT_SRVSTA_ENUM_UINT8 enReportSrvsta);
VOS_VOID TAF_SDC_SetCurrCampLac(VOS_UINT16 usLac);
VOS_VOID TAF_SDC_SetCurrCampRac(VOS_UINT8 ucRac);
VOS_VOID TAF_SDC_SetCurrCampCellId(VOS_UINT32 ulCellId);
VOS_VOID TAF_SDC_SetCsAcRestriction(TAF_SDC_ACCESS_RESTRICTION_STRU *pstCsAcRetriction);
VOS_VOID TAF_SDC_SetPsAcRestriction(TAF_SDC_ACCESS_RESTRICTION_STRU *pstPsAcRetriction);
TAF_SDC_ACCESS_RESTRICTION_STRU *TAF_SDC_GetCsAcRestriction(VOS_VOID);

TAF_SDC_ACCESS_RESTRICTION_STRU *TAF_SDC_GetPsAcRestriction(VOS_VOID);


VOS_VOID TAF_SDC_InitCurcRptCtrlInfo(VOS_VOID);
VOS_VOID TAF_SDC_InitUnsolicitedRptCtrlInfo(VOS_VOID);
VOS_VOID TAF_SDC_ReadCellSignReportCfgNV( VOS_VOID );

VOS_UINT8   TAF_SDC_GetCsCallExistFlg(VOS_VOID);
VOS_VOID    TAF_SDC_SetCsCallExistFlg(
    VOS_UINT8                           ucInCall
);
VOS_VOID    TAF_SDC_SetImsCallExistFlg(
    VOS_UINT8                           ucImsCallExistFlg
);
VOS_UINT8   TAF_SDC_GetImsCallExistFlg(VOS_VOID);

/* ɾ��SRVCC��־���Ƶ�CALLģ�� */

TAF_SDC_CALL_INFO_STRU* TAF_SDC_GetCallInfo(VOS_VOID);
VOS_VOID TAF_SDC_SetMmCallEmerNumList(
    TAF_SDC_MM_EMERGENCY_LIST_STRU     *pstMmCallEmerNumList
);
TAF_SDC_MM_EMERGENCY_LIST_STRU* TAF_SDC_GetMmCallEmerNumList(VOS_VOID);
VOS_VOID TAF_SDC_SetCustomCallEccNumCtx(
    TAF_SDC_CUSTOM_ECC_CTX_STRU        *pstCustomCallEccNumCtx
);
TAF_SDC_CUSTOM_ECC_CTX_STRU* TAF_SDC_GetCustomCallEccNumCtx(VOS_VOID);
VOS_CHAR** TAF_SDC_GetCallEmerNumsNoUsimTblAddr(VOS_VOID);
VOS_CHAR** TAF_SDC_GetCallEmerNumsWithUsimTblAddr(VOS_VOID);



VOS_VOID   TAF_SDC_SetUssdTransMode(
    VOS_UINT8                           ucTransMode
);
VOS_UINT8   TAF_SDC_GetUssdTransMode(VOS_VOID);

VOS_VOID TAF_SDC_InitUssdCfgInfo(VOS_VOID);

VOS_VOID TAF_SDC_ReadUssdTransModeNvim(VOS_VOID);
VOS_VOID TAF_SDC_ReadUssdTransTableNvim(VOS_VOID);

VOS_VOID TAF_SDC_InitServiceInfo(
    TAF_SDC_SERVICE_INFO_STRU          *pstServiceInfo
);

VOS_UINT8 TAF_SDC_GetCsServiceConnStatusFlag(VOS_VOID);
VOS_VOID TAF_SDC_SetCsServiceConnStatusFlag(
    VOS_UINT8                           ucCsServiceConnStatusFlag
);


VOS_VOID TAF_SDC_InitCallInfo(
    TAF_SDC_CALL_INFO_STRU             *pstCallInfo
);
VOS_VOID TAF_SDC_ReadCustomEccNumListNvim(VOS_VOID);



VOS_VOID TAF_SDC_InitAppCfgSupportType(VOS_VOID);

VOS_VOID TAF_SDC_InitGuNwCapInfo(
    TAF_SDC_NETWORK_CAP_INFO_STRU *pstGuNwCapInfo
);
VOS_VOID TAF_SDC_InitLteNwCapInfo(
    TAF_SDC_NETWORK_CAP_INFO_STRU *pstLteNwCapInfo
);
VOS_VOID TAF_SDC_InitNetWorkInfo(
    TAF_SDC_NETWORK_INFO_STRU *pstNetworkInfo
);


VOS_VOID TAF_SDC_InitUeUsageSetting(VOS_VOID);

VOS_UINT8   TAF_SDC_Get7BitToAssicTableType(VOS_VOID);
VOS_VOID    TAF_SDC_Set7BitToAssicTableType(
    VOS_UINT8                           ucType
);

VOS_UINT8*   TAF_SDC_GetCurr7BitToAsciiTableAddr(VOS_VOID);

VOS_VOID TAF_SDC_GetCurrentLai(
    VOS_UINT8                          *aucLai,
    VOS_UINT32                         *pulLaiLen
);



VOS_VOID    TAF_SDC_SetSvlteSupportFlag(
    VOS_UINT8                           ucSvlteSupportFlag
);
VOS_UINT8   TAF_SDC_GetSvlteSupportFlag(VOS_VOID);
VOS_VOID    TAF_SDC_SetCsSmsSrvExistFlg(
    VOS_UINT8                           ucCsSmsSrvExistFlg
);
VOS_UINT8   TAF_SDC_GetCsSmsSrvExistFlg(VOS_VOID);
VOS_VOID TAF_SDC_SetPsSmsSrvExistFlg(
    VOS_UINT8                           ucPsSmsSrvExistFlg
);
VOS_UINT8 TAF_SDC_GetPsSmsSrvExistFlg(VOS_VOID);
VOS_VOID TAF_SDC_SetImsSmsSrvExistFlg(
    VOS_UINT8                           ucImsSmsSrvExistFlg
);
VOS_UINT8 TAF_SDC_GetImsSmsSrvExistFlg(VOS_VOID);
VOS_VOID    TAF_SDC_SetImsSsSrvExistFlg(
    VOS_UINT8                           ucImsSsSrvExistFlg
);
VOS_UINT8   TAF_SDC_GetImsSsSrvExistFlg(VOS_VOID);
VOS_VOID    TAF_SDC_SetCsSsSrvExistFlg(
    VOS_UINT8                           ucCsSsSrvExistFlg
);
VOS_UINT8   TAF_SDC_GetCsSsSrvExistFlg(VOS_VOID);
VOS_VOID TAF_SDC_InitSvlteSupportFlag(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
);


VOS_VOID TAF_SDC_InitImsRatSupport(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
);
VOS_VOID TAF_SDC_InitImsCapability(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
);
VOS_VOID TAF_SDC_InitVoiceDomain(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
);
VOS_VOID TAF_SDC_InitSmsDomain(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
);

VOS_VOID TAF_SDC_InitImsConfigPara(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
);
VOS_VOID TAF_SDC_InitLteCsServiceCfg(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
);

VOS_VOID TAF_SDC_InitRedialCfgInfo(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
);

VOS_VOID TAF_SDC_InitImsRoamingCfgInfo(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
);

VOS_VOID TAF_SDC_InitImsUssdCfgInfo(
    TAF_SDC_INIT_CTX_TYPE_ENUM_UINT8    enInitType,
    TAF_SDC_MS_CAP_INFO_STRU           *pstMsCapInfo
);

VOS_VOID TAF_SDC_InitLcConfigPara(
    TAF_SDC_LC_CONFIG_PARA_STRU        *pstLcConfigPara
);



VOS_VOID TAF_SDC_SetGsmImsSupportFlag(
    VOS_UINT8                          ucGsmImsSupportFlag
);
VOS_UINT8 TAF_SDC_GetGsmImsSupportFlag(VOS_VOID);
VOS_VOID TAF_SDC_SetUtranImsSupportFlag(
    VOS_UINT8                          ucUtranImsSupportFlag
);
VOS_UINT8 TAF_SDC_GetUtranImsSupportFlag(VOS_VOID);

VOS_VOID TAF_SDC_SetLteImsSupportFlag(
    VOS_UINT8                           ucImsSupportFlag
);
VOS_UINT8 TAF_SDC_GetLteImsSupportFlag(VOS_VOID);
VOS_VOID TAF_SDC_SetAppCfgSupportType(
    VOS_UINT16                          usAppCfgSupportType
);
VOS_UINT16 TAF_SDC_GetAppCfgSupportType(VOS_VOID);


VOS_VOID TAF_SDC_SetCsAttachAllowFlg(VOS_UINT8 ucCsAttachAllowFlg);
VOS_UINT8 TAF_SDC_GetCsAttachAllowFlg(VOS_VOID);


#if (FEATURE_ON == FEATURE_PTM)
OM_RING_ID TAF_SDC_GetErrLogRingBufAddr(VOS_VOID);
VOS_VOID TAF_SDC_SetErrLogRingBufAddr(OM_RING_ID pRingBuffer);
VOS_UINT8 TAF_SDC_GetErrlogCtrlFlag(VOS_VOID);
VOS_VOID TAF_SDC_SetErrlogCtrlFlag(VOS_UINT8 ucFlag);
VOS_UINT16 TAF_SDC_GetErrlogAlmLevel(VOS_VOID);
VOS_VOID TAF_SDC_SetErrlogAlmLevel(VOS_UINT16 usAlmLevel);
VOS_VOID TAF_SDC_ReadErrlogCtrlInfoNvim(VOS_VOID);
VOS_VOID TAF_SDC_InitErrLogInfo(
    TAF_SDC_ERRLOG_INFO_STRU           *pstErrLogInfo
);
extern VOS_UINT32 TAF_SDC_PutErrLogRingBuf(
    VOS_CHAR                           *pbuffer,
    VOS_UINT32                          ulbytes
);
extern VOS_UINT32 TAF_SDC_GetErrLogRingBufContent(
    VOS_CHAR                           *pbuffer,
    VOS_UINT32                          ulbytes
);
extern VOS_UINT32 TAF_SDC_GetErrLogRingBufferUseBytes(VOS_VOID);

extern VOS_VOID TAF_SDC_CleanErrLogRingBuf(VOS_VOID);
#endif

VOS_VOID TAF_SDC_SetVoiceCallOnImsSupportFlag(
    VOS_UINT8                          ucVoiceCallOnImsSupportFlag
);
VOS_UINT8 TAF_SDC_GetVoiceCallOnImsSupportFlag(VOS_VOID);
VOS_VOID TAF_SDC_SetSmsOnImsSupportFlag(
    VOS_UINT8                          ucSmsOnImsSupportFlag
);
VOS_UINT8 TAF_SDC_GetSmsOnImsSupportFlag(VOS_VOID);
VOS_VOID TAF_SDC_SetVideoCallOnImsSupportFlag(
    VOS_UINT8                          ucVideoCallOnImsSupportFlag
);
VOS_UINT8 TAF_SDC_GetVideoCallOnImsSupportFlag(VOS_VOID);
VOS_VOID TAF_SDC_SetVoiceDomain(
    TAF_SDC_VOICE_DOMAIN_ENUM_UINT32    enVoiceDomain
);
TAF_SDC_VOICE_DOMAIN_ENUM_UINT32 TAF_SDC_GetVoiceDomain(VOS_VOID);
VOS_VOID TAF_SDC_SetSmsDomain(
    TAF_SDC_SMS_DOMAIN_ENUM_UINT8      enSmsDomain
);
TAF_SDC_SMS_DOMAIN_ENUM_UINT8 TAF_SDC_GetSmsDomain(VOS_VOID);
VOS_VOID TAF_SDC_SetLteEmsSupportFlag(
    VOS_UINT8                           ucLteEmsSupportFlag
);
VOS_UINT8 TAF_SDC_GetLteEmsSupportFlag(VOS_VOID);
VOS_VOID TAF_SDC_SetLteCsSeviceCap(
    TAF_SDC_LTE_CS_SERVICE_ENUM_UINT8   enLteCsServiceCap
);
TAF_SDC_LTE_CS_SERVICE_ENUM_UINT8 TAF_SDC_GetLteCsSeviceCap(VOS_VOID);
TAF_SDC_UE_USAGE_SETTING_ENUM_UINT8 TAF_SDC_GetUeUsageSetting(VOS_VOID);
VOS_VOID TAF_SDC_SetUeUsageSetting(
    TAF_SDC_UE_USAGE_SETTING_ENUM_UINT8   enUeUsageSetting
);

VOS_UINT8 TAF_SDC_GetRoamFlag(VOS_VOID);
VOS_VOID  TAF_SDC_SetRoamFlag(VOS_UINT8 ucRoamFlag);

VOS_UINT8 TAF_SDC_GetCampOnFlag( VOS_VOID );
VOS_VOID  TAF_SDC_SetCampOnFlag(VOS_UINT8 ucCampOnFlg);

TAF_SDC_SYS_MODE_ENUM_UINT8 TAF_SDC_GetCampSysMode( VOS_VOID );

VOS_UINT8 TAF_SDC_GetSimCsRegStatus( VOS_VOID );
VOS_VOID TAF_SDC_SetSimCsRegStatus(
    VOS_UINT8       ucSimCsRegStatus
);


VOS_UINT8 TAF_SDC_GetSimPsRegStatus( VOS_VOID );
VOS_VOID TAF_SDC_SetSimPsRegStatus(
    VOS_UINT8       ucSimPsRegStatus
);

TAF_SDC_RAT_PRIO_STRU* TAF_SDC_GetMsPrioRatList( VOS_VOID );
VOS_VOID TAF_SDC_SetMsPrioRatList(
    TAF_SDC_RAT_PRIO_STRU              *pstPrioRatList
);


VOS_UINT8 TAF_SDC_GetCallRedailFromImsToCsSupportFlag(VOS_VOID);

VOS_VOID TAF_SDC_SetCallRedailFromImsToCsSupportFlag(VOS_UINT8 ucCallRedialFromImsToCs);

VOS_UINT8 TAF_SDC_GetSmsRedailFromImsToCsSupportFlag(VOS_VOID);

VOS_VOID TAF_SDC_SetSmsRedailFromImsToCsSupportFlag(VOS_UINT8 ucSmsRedialFromImsToCs);

VOS_UINT8 TAF_SDC_GetRoamingSupportFlag(VOS_VOID);

VOS_VOID TAF_SDC_SetRoamingSupportFlag(VOS_UINT8 ucRoamingSupport);

VOS_VOID    TAF_SDC_SetImsNormalRegStatus(
    TAF_SDC_IMS_NORMAL_REG_STATUS_ENUM_UINT8    enRegStatus
);

TAF_SDC_IMS_NORMAL_REG_STATUS_ENUM_UINT8   TAF_SDC_GetImsNormalRegStatus(VOS_VOID);

VOS_VOID    TAF_SDC_SetImsVoiceAvailFlg(
    VOS_UINT8       ucAvail
);

VOS_UINT8   TAF_SDC_GetImsVoiceAvailFlg(VOS_VOID);

VOS_VOID  TAF_SDC_SetCurPhoneMode(
    TAF_SDC_PHONE_MODE_ENUM_UINT8       enPhoneMode
);

TAF_SDC_PHONE_MODE_ENUM_UINT8  TAF_SDC_GetCurPhoneMode(VOS_VOID);

TAF_SDC_CREG_TYPE_ENUM_UINT8  TAF_SDC_GetCregType(VOS_VOID);

TAF_SDC_CGREG_TYPE_ENUM_UINT8  TAF_SDC_GetCgregType(VOS_VOID);

TAF_SDC_CEREG_TYPE_ENUM_UINT8  TAF_SDC_GetCeregType(VOS_VOID);

VOS_VOID  TAF_SDC_SetCregType(TAF_SDC_CREG_TYPE_ENUM_UINT8 enCregType);

VOS_VOID  TAF_SDC_SetCgregType(TAF_SDC_CGREG_TYPE_ENUM_UINT8 enCgregType);

VOS_VOID  TAF_SDC_SetCeregType(TAF_SDC_CEREG_TYPE_ENUM_UINT8 enCeregType);
VOS_VOID TAF_SDC_InitPhoneInfo(
    TAF_SDC_PHONE_INFO_STRU            *pstPhInfo
);
VOS_VOID TAF_SDC_InitRegReportStatus(
    TAF_SDC_REG_REPORT_STATUS_STRU     *pstRegReportStatus
);

VOS_VOID TAF_SDC_SetWaitImsVoiceAvailTimerLen(
    VOS_UINT32                          ulTimerLen
);

VOS_UINT32 TAF_SDC_GetWaitImsVoiceAvailTimerLen(VOS_VOID);

VOS_VOID TAF_SDC_InitDsdaPlmnSearchEnhancedCfg(VOS_VOID);

VOS_UINT8 TAF_SDC_GetPsAttachAllowFlg(VOS_VOID);
VOS_VOID TAF_SDC_SetPsAttachAllowFlg(VOS_UINT8 ucPsAttachAllowFlg);


VOS_VOID  TAF_SDC_SetUtranSkipWPlmnSearchFlag(VOS_UINT8  ucUtranSkipWPlmnSearchFlag);
VOS_UINT8  TAF_SDC_GetUtranSkipWPlmnSearchFlag(VOS_VOID);


VOS_UINT8 TAF_SDC_GetUssdOnImsSupportFlag(VOS_VOID);

VOS_VOID TAF_SDC_SetUssdOnImsSupportFlag(VOS_UINT8 ucUssdOnImsSupportFlag);
VOS_VOID TAF_SDC_SetLCEnableFlg(
    VOS_UINT8                           ucLCEnableFlg
);

VOS_UINT8 TAF_SDC_GetLCEnableFlg(VOS_VOID);

VOS_UINT8 TAF_SDC_GetLCWorkCfg(VOS_VOID);
VOS_VOID   TAF_SDC_SetLCWorkCfg(
    TAF_NVIM_LC_WORK_CFG_ENUM_UINT8     enLCWorkCfg
);

VOS_UINT8 TAF_SDC_GetLCRatCombined(VOS_VOID);

TAF_SDC_LC_CONFIG_PARA_STRU* TAF_SDC_GetLcConfigPara(VOS_VOID);

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

#endif /* end of TafSdcCtx.h */
