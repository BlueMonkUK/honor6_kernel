/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : NasMmcCtx.h
  �� �� ��   : ����
  ��    ��   : zhoujun /40661
  ��������   : 2011��04��22��
  ����޸�   :
  ��������   : NasMmcCtx.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2011��04��22��
    ��    ��   : zhoujun /40661
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef _NAS_MMC_CTX_H_
#define _NAS_MMC_CTX_H_

/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/

#include  "vos.h"
#include  "PsTypeDef.h"
#include  "Nasrrcinterface.h"
#include  "NasOmInterface.h"
#include  "NasOmTrans.h"
#include  "asnasapi.h"
#include  "product_config.h"
#include  "GmmDef.h"
#include  "MM_Def.h"
#include  "GmmStru.h"
#include  "MM_Global.h"
#include  "GmmCasDef.h"
#include  "Nas_Mem.h"
#include  "NasFsm.h"
#if (FEATURE_ON == FEATURE_LTE)
#include "MmcLmmInterface.h"
#endif
#include  "NasMmcTimerMgmt.h"
#include  "NasMmlCtx.h"
#include  "NasCommDef.h"

#include "MmaMmcInterface.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  1 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
  2 �궨��
*****************************************************************************/

#define NAS_MMC_MAX_STACK_DEPTH                         (4)                     /* ״̬��ջ����� */
#define NAS_MMC_MAX_HIGH_MSG_NUM                        (2)                     /* ����ĸ����ȼ���Ϣ���� */
#define NAS_MMC_MAX_MID_MSG_NUM                         (4)                     /* ����������ȼ���Ϣ���� */
#define NAS_MMC_MAX_LOW_MSG_NUM                         (8)                     /* ����ĵ����ȼ���Ϣ���� */
#define NAS_MMC_INVALID_MSG_TYPE                        (0xFFFF)                /* ��Ч����Ϣ���� */
#define NAS_MMC_MAX_MSG_QUEUE_NUM                       (8)                     /* ��󻺴�ĸ��� */
#define NAS_MMC_MAX_MSG_BUFFER_LEN                      (1600)                  /* �������󳤶� */


#define NAS_MMC_MAX_REG_PLMN_INFO_NUM                   (80)

#define NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST       (410)    /* �����б�PLMN������չ160����250+160 */

#define NAS_MMC_INTRA_MAX_HIGH_QUALITY_PLMN_NUM        (RRC_MAX_HIGH_PLMN_NUM/3)  /* �ڲ�������Ϣ��Я���ĸ�����PLMN�������� */
#define NAS_MMC_INTRA_MAX_LOW_QUALITY_PLMN_NUM         (RRC_MAX_LOW_PLMN_NUM/3)   /* �ڲ�������Ϣ��Я���ĵ�����PLMN�������� */

#define NAS_MMC_NUM_ZERO                               (0)                      /*���ֳ�ʼ��Ϊ0 */

#define NAS_MMC_MAX_SIGN_THRESHOLD                     (5)                      /* �źű仯�������ֵ5db*/

#define NAS_MMC_MAX_SIGN_TIMERINTERVAL                 (20)                      /* �źű仯���ʱ����(s)*/

#define NAS_MMC_UTRAN_SKIP_W_PLMNSEARCH_MASK           (0x0001)                 /* 9234 nv��usSolutionMask�ֶε�0 bit������˫Modem��ͨ����һModem��PLMN��Ϣ����FDD�����Ƿ������Ĳ����Ƿ�����*/
#define NAS_MMC_SUPPORT_NCELL_SEARCH_MASK              (0x0002)                 /* 9234 nv��usSolutionMask�ֶε�1 bit��ͨ��Modem1��GSM�ϱ���L��TDS������Ϣ,
                                                                               Modem0��֧��GSM������£�T/L�������ܹ����ݴ��ݵ�����Ƶ�����������TDS/LTE*/
#define  NAS_MMC_TDD_ARFCN_MAX_NUM                      (9)                     /* TDDƵ���б�������������GAS���������ṹ��MTC�ϱ�Ƶ����Ϣ */
#define  NAS_MMC_LTE_ARFCN_MAX_NUM                      (8)                     /* LTEƵ���б�������������GAS���������ṹ��MTC�ϱ�Ƶ����Ϣ */

#define  NAS_MMC_LTE_CELL_MAX_NUM                       (8)                     /* LTEС���б������� */


#define NAS_MMC_WCDMA_BAND_NULL                 0x0000
#define NAS_MMC_WCDMA_BAND_I                    0x0001
#define NAS_MMC_WCDMA_BAND_II                   0x0002
#define NAS_MMC_WCDMA_BAND_III                  0x0004
#define NAS_MMC_WCDMA_BAND_IV                   0x0008
#define NAS_MMC_WCDMA_BAND_V                    0x0010
#define NAS_MMC_WCDMA_BAND_VI                   0x0020
#define NAS_MMC_WCDMA_BAND_VII                  0x0040
#define NAS_MMC_WCDMA_BAND_VIII                 0x0080
#define NAS_MMC_WCDMA_BAND_IX                   0x0100

/*****************************************************************************
  3 ö�ٶ���
*****************************************************************************/

/*****************************************************************************
 ö����    : NAS_MMC_FSM_ID_ENUM_UINT32
 ö��˵��  : ״̬��IDö�ٶ���
 1.��    ��   : 2011��4��28��
   ��    ��   : zhoujun 40661
   �޸�����   : �½�
*****************************************************************************/
enum NAS_MMC_FSM_ID_ENUM
{
    /* NAS MMC L1״̬�������� */
    NAS_MMC_FSM_L1_MAIN                                     =0X00,

    /* NAS MMC L2״̬��������,���ڶ��L2״̬�� */

    /* Э��ջ��ʼ��L2״̬�� */
    NAS_MMC_FSM_SWITCH_ON                                   =0X01,

    /* Э��ջ�ػ�L2״̬�� */
    NAS_MMC_FSM_POWER_OFF                                   =0X02,

    /* PLMN SELECTION ״̬�� */
    NAS_MMC_FSM_PLMN_SELECTION                              =0X03,

    /* ANYCELL����״̬�� */
    NAS_MMC_FSM_ANYCELL_SEARCH                              =0X04,

    /* ��ϵͳ��ѡ״̬�� */
    NAS_MMC_FSM_INTER_SYS_CELLRESEL                         =0X05,

    /* ��ϵͳ������������״̬�� */
    NAS_MMC_FSM_INTER_SYS_OOS                               =0X06,

    /* �б�����״̬�� */
    NAS_MMC_FSM_PLMN_LIST                                   =0X07,

    /* SYSCFG����L2״̬�� */
    NAS_MMC_FSM_SYSCFG                                      =0X08,

    /* BG����L2״̬�� */
    NAS_MMC_FSM_BG_PLMN_SEARCH                              =0X09,

    /* ��ϵͳHandover״̬�� */
    NAS_MMC_FSM_INTER_SYS_HO                                =0X0A,

    /* ��ϵͳCellChange״̬�� */
    NAS_MMC_FSM_INTER_SYS_CCO                               =0X0B,

    NAS_FSM_BUTT

};
typedef VOS_UINT32 NAS_MMC_FSM_ID_ENUM_UINT32;

/*****************************************************************************
 ö����    : NAS_MMC_PLMN_SELECTION_MODE_ENUM_UINT8
 �ṹ˵��  : MMC PLMN��������
 1.��    ��   : 2011��6��30��
   ��    ��   : zhoujun 40661
   �޸�����   : ����
*****************************************************************************/
enum NAS_MMC_PLMN_SELECTION_MODE_ENUM
{
    NAS_MMC_PLMN_SELECTION_MODE_AUTO,                                   /* �Զ�����ģʽ */
    NAS_MMC_PLMN_SELECTION_MODE_MANUAL,                                 /* �ֶ�ģʽ */
    NAS_MMC_PLMN_SELECTION_MODE_BUTT
};
typedef VOS_UINT8 NAS_MMC_PLMN_SELECTION_MODE_ENUM_UINT8;


enum NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM
{
    NAS_MMC_SPEC_PLMN_SEARCH_RUNNING,                             /* ����ָ������,���������ͷš�ָ���������������� */
    NAS_MMC_SPEC_PLMN_SEARCH_STOP,                                /* û��ָ������ */
    NAS_MMC_SPEC_PLMN_SEARCH_STATE_BUTT
};
typedef VOS_UINT8 NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8;


enum NAS_MMC_AS_CELL_CAMP_ON_ENUM
{
    NAS_MMC_AS_CELL_CAMP_ON,                             /* ����㵱ǰפ��״̬ */
    NAS_MMC_AS_CELL_NOT_CAMP_ON,                         /* ����㵱ǰ��פ��״̬,�����������������Not camp on��NAS�������� */
    NAS_MMC_AS_CELL_CAMP_ON_BUTT
};
typedef VOS_UINT8 NAS_MMC_AS_CELL_CAMP_ON_ENUM_UINT8;

/*****************************************************************************
 ö����    : NAS_MMC_WAIT_MM_START_CNF_ENUM
 �ṹ˵��  : ���ڱ���յ���MM/GMM�Ŀ����ظ���Ϣ
*****************************************************************************/
enum NAS_MMC_WAIT_MM_START_CNF_ENUM
{
    NAS_MMC_WAIT_MMGMM_START_CNF_NULL   = 0x00,
    NAS_MMC_WAIT_MMCGMM_START_CNF       = 0x01,
    NAS_MMC_WAIT_MMCMM_START_CNF        = 0x02
};
typedef VOS_UINT8 NAS_MMC_WAIT_MM_START_CNF_ENUM_UINT8;

/*****************************************************************************
 ö����    : NAS_MMC_WAIT_AS_START_CNF_ENUM
 �ṹ˵��  : ���ڱ���յ���GU�����Ŀ����ظ���Ϣ
*****************************************************************************/
enum NAS_MMC_WAIT_AS_START_CNF_ENUM
{
    NAS_MMC_WAIT_GASWAS_START_CNF_NULL  = 0x00,
    NAS_MMC_WAIT_GAS_START_CNF          = 0x01,
    NAS_MMC_WAIT_WAS_START_CNF          = 0x02
};
typedef VOS_UINT8 NAS_MMC_WAIT_AS_START_CNF_ENUM_UINT8;

/*****************************************************************************
 ö����    : NAS_MMC_MM_POWEROFF_FLAG_ENUM
 Э����  :
 ASN.1���� :
 ö��˵��  : MMC�ػ��ȴ�MM,GMM�Ļظ���־
*****************************************************************************/
enum    NAS_MMC_MM_POWEROFF_FLAG_ENUM
{
    NAS_MMC_MMGMM_POWER_OFF_NULL_FLG    = 0x00,
    NAS_MMC_MM_POWER_OFF_FLG            = 0x01,
    NAS_MMC_GMM_POWER_OFF_FLG           = 0x02
};

typedef VOS_UINT8 NAS_MMC_MM_POWEROFF_FLAG_ENUM_UINT8;

/*****************************************************************************
 ö����    : NAS_MMC_AS_POWEROFF_FLAG_ENUM
 Э����  :
 ASN.1���� :
 ö��˵��  : MMC�ػ��ȴ�GAS,WAS,LMM�Ļظ���־
*****************************************************************************/
enum    NAS_MMC_AS_POWEROFF_FLAG_ENUM
{
    NAS_MMC_AS_POWER_OFF_NULL_FLG       = 0x00,
    NAS_MMC_GAS_POWER_OFF_FLG           = 0x01,
    NAS_MMC_WAS_POWER_OFF_FLG           = 0x02,
    NAS_MMC_LTE_POWER_OFF_FLG           = 0x04
};

typedef VOS_UINT8 NAS_MMC_AS_POWEROFF_FLAG_ENUM_UINT8;

/*****************************************************************************
 ö����    : NAS_MMC_SUSPEND_RSP_FLAG_ENUM
 Э����  :
 ASN.1���� :
 ö��˵��  : MMC�յ�������Ϣ�󣬵ȴ�MM,GMM�Ļظ���־
*****************************************************************************/
enum    NAS_MMC_SUSPEND_RSP_FLAG_ENUM
{
    NAS_MMC_SUSPEND_RSP_NULL_FLG        = 0x00,
    NAS_MMC_MM_SUSPEND_RSP_FLG          = 0x01,
    NAS_MMC_GMM_SUSPEND_RSP_FLG         = 0x02,
    NAS_MMC_MMGMM_SUSPEND_RSP_FLG       = 0x03,
};
typedef VOS_UINT8 NAS_MMC_SUSPEND_RSP_FLAG_ENUM_UINT8;


/*****************************************************************************
 ö����    : NAS_MMC_RESUME_RSP_FLAG_ENUM
 Э����  :
 ASN.1���� :
 ö��˵��  : MMC�յ�GUL��resume ind��Ϣ�󣬵ȴ�MM,GMM��resume�ظ���־
*****************************************************************************/
enum    NAS_MMC_RESUME_RSP_FLAG_ENUM
{
    NAS_MMC_RESUME_RSP_NULL_FLG         = 0x00,
    NAS_MMC_MM_RESUME_RSP_FLG           = 0x01,
    NAS_MMC_GMM_RESUME_RSP_FLG          = 0x02,
    NAS_MMC_MMGMM_RESUME_RSP_FLG        = 0x03,
};
typedef VOS_UINT8 NAS_MMC_RESUME_RSP_FLAG_ENUM_UINT8;

/*RRC signaling status*/
enum NAS_MMC_SIGNALING_EXIST_ENUM
{
    NAS_MMC_SIGNALING_NOT_EXIST                 = (0x00),
    NAS_MMC_PS_SIGNALING_PRESENT                = (0x01),
    NAS_MMC_CS_SIGNALING_PRESENT                = (0x02),
    NAS_MMC_CS_PS_SIGNALING_PRESENT             = (0x03),
    NAS_MMC_RRC_CONNECTION_PRESENT              = (0x04),
    NAS_MMC_EPS_CONNECTION_SIGNALING_PRESENT    = (0x08),
    NAS_MMC_EPS_CONNECTION_DATA_PRESENT         = (0x10),
    NAS_MMC_SIGNALING_EXIST_BUTT
};

/*****************************************************************************
 ö����    : NAS_MMC_REG_DOMAIN_ENUM_UINT8
 ö��˵��  : ע�������Ϣ
 1.��    ��   : 2011��5��28��
   ��    ��   : zhoujun 40661
   �޸�����   : �½�
*****************************************************************************/
enum NAS_MMC_REG_DOMAIN_ENUM
{
    NAS_MMC_REG_DOMAIN_NONE  = 0x00,                                            /* ע������Ϊ���� */
    NAS_MMC_REG_DOMAIN_CS    = 0x01,                                            /* ע������ΪCS�� */
    NAS_MMC_REG_DOMAIN_PS    = 0x02,                                            /* ע������ΪPS�� */
    NAS_MMC_REG_DOMAIN_PS_CS = 0x03,                                            /* ע������ΪCS+PS�� */
    NAS_MMC_REG_DOMAIN_EPS   = 0x04,                                            /* ע������ΪEPS�� */
    NAS_MMC_REG_DOMAIN_BUTT
};
typedef VOS_UINT8 NAS_MMC_REG_DOMAIN_ENUM_UINT8;


enum NAS_MMC_PLMN_SEARCH_SCENE_ENUM
{
    NAS_MMC_PLMN_SEARCH_SCENE_SWITCH_ON,                        /* ���� */
    NAS_MMC_PLMN_SEARCH_SCENE_AREA_LOST,                        /* �������� */
    NAS_MMC_PLMN_SEARCH_SCENE_SYSCFG_SET,                       /* Syscfg���ô������� */
    NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED,          /* Available Timer��ʱ�������� */
    NAS_MMC_PLMN_SEARCH_SCENE_USER_LIST,                        /* �û��б����������󴥷����� */
    NAS_MMC_PLMN_SEARCH_SCENE_REG_REJ_PLMN_SEARCH,              /* ע�ᱻ�ܴ����������� */
    NAS_MMC_PLMN_SEARCH_SCENE_REG_REJ_CURRENT_PLMN_SEARCH,      /* ע�ᱻ�ܴ����ѵ�ǰ���� */
    NAS_MMC_PLMN_SEARCH_SCENE_USER_SPEC_PLMN_SEARCH,            /* �û�ָ������ */
    NAS_MMC_PLMN_SEARCH_SCENE_MANUAL_CHANGE_AUTO_MODE,          /* �ֶ�����ģʽ�޸�Ϊ�Զ�ģʽ�������� */
    NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_PLMN_SEARCH,            /* �����ȼ����� */
    NAS_MMC_PLMN_SEARCH_SCENE_DISABLE_LTE,                      /* disable LTE�������� */
    NAS_MMC_PLMN_SEARCH_SCENE_INTER_SYS_OOS,
    NAS_MMC_PLMN_SEARCH_SCENE_CSFB_SERVIEC_REJ,                 /* CSFB������SERVICE REJECTʱ���µ���������*/
    NAS_MMC_PLMN_SEARCH_SCENE_USER_AUTO_RESEL,                  /*  �û��Զ���ѡ  */

    NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED_NCELL_SEARCH, /* Available Timer��ʱ������ncell���� */

    NAS_MMC_PLMN_SEARCH_SCENE_RF_ENABLE,                        /* ����NO RFʧ�ܺ�RF���ô���������������������ʱ����ʱ�������� */               

    NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_RAT_HPLMN_SEARCH,       /* �����ȼ�RAT��HPLMN���� */
    NAS_MMC_PLMN_SEARCH_SCENE_REG_NCELL,                        /* ����NCELL��Ϣ����������ע�� */
    NAS_MMC_PLMN_SEARCH_SCENE_REG_HPLMN,                        /* ֻ������ע��(E)HPLMN */
    NAS_MMC_PLMN_SEARCH_SCENE_REG_PREF_PLMN,                    /* ֻ������ע��(E)HPLMN+UPLMN+OPLMN */
    NAS_MMC_PLMN_SEARCH_SCENE_REG_ANY_PLMN,                     /* ����������ע���κηǽ�ֹ���� */


    NAS_MMC_PLMN_SEARCH_SCENE_AREA_LOST_ROAMING_CFG,           /*������������״̬�¶�������������*/
    NAS_MMC_PLMN_SEARCH_SCENE_BUTT
};
typedef VOS_UINT32 NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32;

/*****************************************************************************
 ö����    : NAS_MMC_SERVICE_ENUM_UINT8
 ö��˵��  : MMC��ǰ�ķ���״̬
 1.��    ��   : 2011��7��11��
   ��    ��   : zhoujun 40661
   �޸�����   : �½�
*****************************************************************************/
enum NAS_MMC_SERVICE_ENUM
{
    NAS_MMC_NORMAL_SERVICE,
    NAS_MMC_LIMITED_SERVICE,
    NAS_MMC_LIMITED_SERVICE_REGION,
    NAS_MMC_NO_SERVICE,
    NAS_MMC_SERVICE_BUTT
};
typedef VOS_UINT8  NAS_MMC_SERVICE_ENUM_UINT8;



/*****************************************************************************
 ö����    : NAS_MMC_INIT_CTX_TYPE_ENUM_UINT8
 �ṹ˵��  : ��ʼ��MMC CTX����
 1.��    ��   : 2011��7��14��
   ��    ��   : zhoujun 40661
   �޸�����   : ����
*****************************************************************************/
enum NAS_MMC_INIT_CTX_TYPE_ENUM
{
    NAS_MMC_INIT_CTX_STARTUP                    = 0,
    NAS_MMC_INIT_CTX_POWEROFF                   = 1,
    NAS_MMC_INIT_CTX_BUTT
};
typedef VOS_UINT32 NAS_MMC_INIT_CTX_TYPE_ENUM_UINT8;





enum NAS_MMC_ADDITIONAL_ACTION_ENUM
{
    NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL                      = 0,            /* ����Ч������С��פ��,PS:#3,#6,#8,CS/PS:Auth Fail */
    NAS_MMC_ADDITIONAL_ACTION_PLMN_SELECTION                    = 1,            /* #11,#13��Ҫ��������; attempt counter>=Max������������*/
    NAS_MMC_ADDITIONAL_ACTION_SEARCH_SUITABLE_CELL_IN_SAME_PLMN = 2,            /* #15 ����������ǰ���� */
    NAS_MMC_ADDITIONAL_ACTION_CELL_SELECTION                    = 3,            /* #12����������Ҳ����Ҫ�����ȴ�ע�ᣬ�ɽ���������ѡ */
    NAS_MMC_ADDITIONAL_ACTION_WAIT_REG_ATTEMPT                  = 4,            /* #other cause, attempt counter<Maxֵ,�ȴ���һ��ע�᳢�� */
    NAS_MMC_ADDITIONAL_ACTION_NORMAL_CAMP_ON                    = 5,            /* CS��PSפ���ɹ�������Ҫ���� */
    NAS_MMC_ADDITIONAL_ACTION_ACCESS_BARRED                     = 6,            /* access barred������Ҫ���� */
    NAS_MMC_ADDITIONAL_ACTION_OPTIONAL_PLMN_SELECTION           = 7,            /* PS #14 ��ѡ����,����CS��ע����;attempt counter>=Max������������*/
    NAS_MMC_ADDITIONAL_ACTION_LIMITED_CAMP_ON                   = 8,            /* ֻ��CS/PS����Ч����Gprs not support,
                                                                                   CS only��PS Onlyʱ������פ��������Ҫ���� */
    NAS_MMC_ADDITIONAL_ACTION_SINGLE_DOMAIN_ANYCELL             = 9,            /* CS��PS����ע�ᵼ�µĿ���Ч:CSע�ᱻ����2,3,6 ��PS:7*/
    NAS_MMC_ADDITIONAL_ACTION_BUTT
};
typedef VOS_UINT8 NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8;
enum NAS_MMC_NET_STATUS_ENUM
{
    NAS_MMC_NET_STATUS_NO_SEARCHED,                     /* δ��������ǰ�������Ƿ���ڸ����绹��ȷ�� */
    NAS_MMC_NET_STATUS_SEARCHED_NO_EXIST,               /* ����������ǰ�����в����ڶ�Ӧ���뼼�������� */
    NAS_MMC_NET_STATUS_SEARCHED_EXIST,                  /* ����������ǰ�����д��ڶ�Ӧ���뼼�������磬����δ����פ�� */
    NAS_MMC_NET_STATUS_SEARCHED_REGISTERED,             /* ����������Ӧ���뼼����������ڣ������Ѿ����Թ�ע�� */
    NAS_MMC_NET_STATUS_BUTT
};
typedef VOS_UINT8 NAS_MMC_NET_STATUS_ENUM_UINT8;


enum NAS_MMC_NET_QUALITY_ENUM
{
    NAS_MMC_NET_QUALITY_UNKNOWN,                     /* �����ź�δ֪ */
    NAS_MMC_NET_HIGH_QUALITY,                        /* �����ź�Ϊ������ */
    NAS_MMC_NET_LOW_QUALITY,                         /* �����ź�Ϊ������ */
    NAS_MMC_NET_QUALITY_BUTT
};
typedef VOS_UINT8 NAS_MMC_NET_QUALITY_ENUM_UINT8;
enum NAS_MMC_PLMN_TYPE_ENUM
{
    NAS_MMC_PLMN_TYPE_RPLMN,                     /* ������ΪRPLMN */
    NAS_MMC_PLMN_TYPE_USER_SEPC_PLMN,            /* ������Ϊ�û�ָ������ */
    NAS_MMC_PLMN_TYPE_HPLMN,                     /* ������ΪHome����EHome PLMN */
    NAS_MMC_PLMN_TYPE_UPLMN,                     /* ������ΪUser Controlled PLMN */
    NAS_MMC_PLMN_TYPE_OPLMN,                     /* ������ΪOperator Controlled PLMN */
    NAS_MMC_PLMN_TYPE_EPLMN,                     /* ������Ϊ��Ч���� */
    NAS_MMC_PLMN_TYPE_AVAIL_PLMN,                /* ������Ϊ������������ */
    NAS_MMC_PLMN_TYPE_BUTT
};
typedef VOS_UINT8 NAS_MMC_PLMN_TYPE_ENUM_UINT8;
enum NAS_MMC_WAIT_REG_RESULT_IND_ENUM
{
    NAS_MMC_WAIT_REG_RESULT_IND_NULL   = 0x00,                                  /*��ǰ����Ҫ�ȴ�CS/PS��ע����*/
    NAS_MMC_WAIT_REG_RESULT_IND_CS     = 0x01,                                  /*��ǰ��Ҫ�ȴ�CS��ע����*/
    NAS_MMC_WAIT_REG_RESULT_IND_PS     = 0x02,                                  /*��ǰ��Ҫ�ȴ�PS��ע����*/
    NAS_MMC_WAIT_REG_RESULT_IND_IMS    = 0x04,                                  /*��ǰ��Ҫ�ȴ�IMS��ע����*/
    NAS_MMC_WAIT_REG_RESULT_IND_BUTT
};
typedef VOS_UINT8 NAS_MMC_WAIT_REG_RSLT_IND_ENUM_UINT8;


enum NAS_MMC_AT_ACTION_TYPE_ENUM
{
    NAS_MMC_AT_ACTION_TYPE_START_FOREVER                        = 0,            /* ָʾWRRC��GRRC�������� */
    NAS_MMC_AT_ACTION_TYPE_START_ONCE                           = 1,            /* ָʾWRRC��GRRC��������һ�� */
    NAS_MMC_AT_ACTION_TYPE_STOP                                 = 2,            /* ָʾWRRC��GRRCֹͣ */
    NAS_MMC_AT_ACTION_TYPE_BUTT
};
typedef VOS_UINT8 NAS_MMC_AT_ACTION_TYPE_ENUM_UINT8;
enum NAS_MMC_AT_MSG_TYPE_ENUM
{
    NAS_MMC_AT_MSG_TYPE_CELL_NONE                                   = 0,        /* ��ʾ����ȡС���κ���Ϣ */
    NAS_MMC_AT_MSG_TYPE_CELL_SIGN                                   = 1,        /* ��ʾ��ȡС����Ϣǿ�� */
    NAS_MMC_AT_MSG_TYPE_CELL_ID                                     = 2,        /* ��ʾ��ȡС��ID */
    NAS_MMC_AT_MSG_TYPE_CELL_BLER                                   = 4,        /* ��ʾ��ȡ�ŵ������� */
    NAS_MMC_AT_MSG_TYPE_BUTT
};
typedef VOS_UINT8 NAS_MMC_AT_MSG_TYPE_ENUM_UINT8;

/*****************************************************************************
 ö����    : NAS_MMC_ABORT_FSM_TYPE_UINT8
 �ṹ˵��  : Abort����
 1.��    ��   : 2011��7��14��
   ��    ��   : zhoujun 40661
   �޸�����   : ����
*****************************************************************************/
enum NAS_MMC_ABORT_FSM_TYPE
{
    NAS_MMC_ABORT_FSM_IMMEDIATELY           = 0,
    NAS_MMC_ABORT_FSM_DELAY                 = 1,
    NAS_MMC_ABORT_BUTT
};
typedef VOS_UINT8 NAS_MMC_ABORT_FSM_TYPE_UINT8;



enum NAS_MMC_PLMN_LIST_SEARCH_SCENE_ENUM
{
    NAS_MMC_PLMN_LIST_SEARCH_SCENE_USER_LIST,                                     /* �û��б������������������ */
    NAS_MMC_PLMN_LIST_SEARCH_SCENE_DISABLE_LTE,                                   /* disable LTE�������� */
    NAS_MMC_PLMN_LIST_SEARCH_SCENE_BUTT
};
typedef VOS_UINT8  NAS_MMC_PLMN_LIST_SEARCH_SCENE_ENUM_UINT8;


enum NAS_MMC_BG_PLMN_SEARCH_SCENE_ENUM
{
    NAS_MMC_BG_PLMN_SEARCH_SCENE_AREA_LOST,                                     /* �������� */
    NAS_MMC_BG_PLMN_SEARCH_SCENE_DISABLE_LTE,                                   /* disable LTE�������� */
    NAS_MMC_BG_PLMN_SEARCH_SCENE_BUTT
};
typedef VOS_UINT8 NAS_MMC_BG_PLMN_SEARCH_SCENE_ENUM_UINT8;



enum NAS_MMC_COVERAGE_TYPE_ENUM
{
    NAS_MMC_COVERAGE_TYPE_NONE              = 0,                                /* �����縲�� */
    NAS_MMC_COVERAGE_TYPE_LOW               = 1,                                /* �е��������縲��*/
    NAS_MMC_COVERAGE_TYPE_HIGH              = 2,                                /* �и��������縲�� */
    NAS_MMC_COVERAGE_TYPE_BUTT
};
typedef VOS_UINT8  NAS_MMC_COVERAGE_TYPE_ENUM_UINT8;
enum NAS_MMC_CS_DOMAIN_CAPA_CHANGE_TYPE_ENUM
{
    NAS_MMC_CS_DOMAIN_CAPA_NO_CHANGE                        = 0,                /* CS�������ޱ仯 */
    NAS_MMC_CS_DOMAIN_CAPA_UNAVAIL_TO_AVAIL                 = 1,                /* CS���ɲ����õ����� */
    NAS_MMC_CS_DOMAIN_CAPA_AVAIL_TO_UNAVAIL                 = 2,                /* CS���ɿ��õ������� */
    NAS_MMC_CS_DOMAIN_CAPA_CHANGE_TYPE_BUTT
};
typedef VOS_UINT8 NAS_MMC_CS_DOMAIN_CAPA_CHANGE_TYPE_ENUM_UINT8;
enum NAS_MMC_PLMN_SELECTION_LIST_TYPE_ENUM
{
    NAS_MMC_STORE_HIGH_PRIO_PLMN_SELECTION_LIST             = 0,                /* �洢���Ǹ����ȼ������б���Ϣ */
    NAS_MMC_BACKUP_PLMN_SELECTION_LIST                      = 1,                /* �洢����ѡ��״̬�����б���Ϣ�ı��� */
    NAS_MMC_PLMN_SELECTION_LIST_TYPE_BUTT                                       /* ��ǰ��Ϣ������ */
};
typedef VOS_UINT8 NAS_MMC_PLMN_SELECTION_LIST_TYPE_ENUM_UINT8;
enum NAS_MMC_AVAILABLE_TIMER_TYPE_ENUM
{
    NAS_MMC_AVAILABLE_TIMER_TYPE_NCELL                      = 0,                /* available��ʱ������ʱ����Ϊncell���� */
    NAS_MMC_AVAILABLE_TIMER_TYPE_NORMAL                     = 1,                /* available��ʱ������ʱ����Ϊ��ͨ���� */
    NAS_MMC_AVAILABLE_TIMER_TYPE_BUTT
};
typedef VOS_UINT8 NAS_MMC_AVAILABLE_TIMER_TYPE_ENUM_UINT8;


enum NAS_MMC_REG_CONTROL_ENUM
{
    NAS_MMC_REG_CONTROL_BY_3GPP_MMC = 0,                 /* ������������Ƿ�ע�ᰴĿǰ3GPPЭ��ģ��MMC���� */
    NAS_MMC_REG_CONTROL_BY_3GPP2_CBP,                    /*������������Ƿ�ע����3GPP2Э��CDMA Baseband Protocol���� */
    NAS_MMC_REG_CONTROL_BUTT
};
typedef VOS_UINT8 NAS_MMC_REG_CONTROL_ENUM_UINT8;
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
 �ṹ��    : NAS_MMC_ENTRY_MSG_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : ��Ҫ����������Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulEventType;
    VOS_UINT8                           aucEntryMsgBuffer[NAS_MMC_MAX_MSG_BUFFER_LEN];
} NAS_MMC_ENTRY_MSG_STRU;



typedef struct
{
    VOS_UINT32                              ulPlmnSearchFlg;                    /* ������������ڱ�־ */
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32   enPlmnSearchScene;                  /* �������� */
}NAS_MMC_BUFFERED_PLMN_SEARCH_INFO_STRU;



typedef struct{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8    enRatType;         /* ����Ľ��뼼�� */
    NAS_MMC_NET_STATUS_ENUM_UINT8      enNetStatus;       /* ����Ĵ���״̬ */
    NAS_MMC_NET_QUALITY_ENUM_UINT8     enQuality;         /* ������ź����� */
    VOS_UINT8                          ucReserve;
    VOS_INT32                          lRscp;             /* RSCP����ֵ */
}NAS_MMC_PLMN_RAT_INFO_STRU;


typedef struct{
    NAS_MML_PLMN_ID_STRU                stPlmnId;                                   /* �������PLMN ID */
    NAS_MMC_PLMN_TYPE_ENUM_UINT8        enPlmnType;                                 /* ��������        */
    VOS_UINT8                           aucReserve[2];
    VOS_UINT8                           ucRatNum;                                   /* ���뼼����Ϣ����Ч�Ľ��뼼������        */
    NAS_MMC_PLMN_RAT_INFO_STRU          astPlmnRatInfo[NAS_MML_MAX_RAT_NUM];        /* ����������뼼������Ϣ */
}NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU;


typedef struct
{
    VOS_UINT8                           ucSearchAllBand;   /* �Ƿ��ڵ�ǰ���뼼��ִ�й�ȫƵ����, VOS_TRUE:ִ�й�ȫƵ��, VOS_FALSE:δִ�й� */
    NAS_MMC_COVERAGE_TYPE_ENUM_UINT8    enCoverageType;    /* ��ǰ���뼼���µ����縲������, 0:�����ڸ��ǣ�1:���ڵ��������縲��,2:���ڸ��������縲�� */
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRatType;         /* ����Ľ��뼼�� */
    VOS_UINT8                           ucSearchRplmnAndHplmnFlg;
}NAS_MMC_RAT_SEARCH_INFO_STRU;
typedef struct
{
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsRegCause;                           /* CSע����causeֵ */
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsRegCause;                           /* PSע����causeֵ */
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enCsRegAdditionalAction;                /* ��ǰ����CS��ע�����ĸ��Ӵ��� */
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enPsRegAdditionalAction;                /* ��ǰ����PS��ע�����ĸ��Ӵ��� */
    VOS_UINT8                           ucWaitRegRsltFlag;                      /* �ȴ� CS/PS��ע���־ */
    VOS_UINT8                           ucReserve;
}NAS_MMC_PLMN_SELECTION_REG_RSLT_INFO_STRU;
typedef struct
{
    VOS_UINT16                             usSearchPlmnNum;                     /* �б��б����PLMN���� */
    VOS_UINT8                              aucReserve[2];
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU  astPlmnSelectionList[NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST]; /* �����б� */
}NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU;






typedef struct
{
    VOS_UINT32                          ulCsAttemptCount;                       /* CSע��ĳ��Դ��� */
    VOS_UINT32                          ulPsAttemptCount;                       /* PSע��ĳ��Դ��� */
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enCsRegAdditionalAction;               /* ��ǰ����CS��ע�����ĸ��Ӵ��� */
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enPsRegAdditionalAction;               /* ��ǰ����PS��ע�����ĸ��Ӵ��� */
    VOS_UINT8                           ucReserve[2];
}NAS_MMC_L1_MAIN_REG_RSLT_INFO_STRU;


typedef struct
{
    NAS_MMC_L1_MAIN_REG_RSLT_INFO_STRU                      stRegRsltInfo;                    /* ��������״̬��ע������Ϣ */
    VOS_UINT32                                              ulCurTimerCount;                  /* ��ǰAvailable Timer�������� */
    
    VOS_UINT32                                              ulCurHighRatHplmnTimerCount;      /* ��ǰhigh prio rat hplmn Timer�������� */

    VOS_UINT32                                              ulCurNcellSearchTimerCount;  /* ��ǰncell���������������� */

    NAS_MMC_AVAILABLE_TIMER_TYPE_ENUM_UINT8                 enAvailableTimerType;
    VOS_UINT8                                               aucReserved[3];

}NAS_MMC_FSM_L1_MAIN_CTX_STRU;


typedef struct
{
    VOS_UINT8                                 ucAbortFlg;                       /* ��ǰ״̬����־�Ƿ��յ���ֹҪ��,VOS_TRUE:�յ�, VOS_FALSE:δ�յ� */
    VOS_UINT8                                 ucRelRequestFlg;                  /* �Ƿ��������������Ƿ�,VOS_TRUE:����������, VOS_FALSE:�����ȴ��ͷ� */

    NAS_MML_NET_RAT_TYPE_ENUM_UINT8           ucInterSysSuspendRat;             /* ��¼��ϵͳ״̬������ʱ�Ľ��뼼�������ע����ʱ��G�л���W */  
    VOS_UINT8                                 aucReserve[1];
    
    NAS_MMC_PLMN_SELECTION_REG_RSLT_INFO_STRU stRegRlstInfo;                    /* ����ѡ��״̬����ע������Ϣ */
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU     stPlmnSelectionListInfo;          /* ����ѡ��״̬����ѡ���б���Ϣ */
    NAS_MMC_RAT_SEARCH_INFO_STRU              astSearchRatInfo[NAS_MML_MAX_RAT_NUM];  /* ���治ͬ���뼼����������Ϣ */
    NAS_MML_FORBIDPLMN_ROAMING_LAS_INFO_STRU  stForbRoamLaInfo;
    NAS_MML_PLMN_ID_STRU                      stForbGprsPlmn;                   /* "forbidden PLMNs for GPRS service"  */

    NAS_MML_PLMN_ID_STRU                      stCsPsMode1ReCampLtePlmn;         /* ���浱ǰL�����PLMNID */

    NAS_MML_PLMN_WITH_RAT_STRU                stCurrSearchingPlmn;                /* ��ǰ���ڳ��Ե����缰����뼼��,����at+cops=0 9074 nv������ʱ�жϵ�ǰ���������������Ƿ�Ϊhplmn */
}NAS_MMC_FSM_PLMN_SELECTION_CTX_STRU;
typedef struct
{
    VOS_UINT8                       ucTotalSwitchOnRatNum;                         /* �����㷢�Ϳ���������ܸ��� */
    VOS_UINT8                       ucSwitchOnIndex;                            /* ָ����Ҫ������Ӧģ�Ľ��뼼�����±� */
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8 aucSwitchOnRatPrio[NAS_MML_MAX_RAT_NUM];    /* ������ÿ�����Ҫ�����������Ⱥ�˳�� */
    VOS_UINT8                       aucResevel[3];
}NAS_MMC_SENDING_AS_SWITCH_ON_SEQUENCE_CTRL_STRU;
typedef struct
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8 enSwitchOnAsRat;                            /* ������ÿ�����Ҫ���������Ľ��뼼�� */
    VOS_UINT8                       ucNeedWaitCnfFlag;                          /* ��Ҫ�ȴ��ý���ģ�Ļظ���ʶ */
    VOS_UINT8                       aucReserve[2];
}NAS_MMC_AS_SWITCH_ON_CNF_INFO_STRU;

typedef struct
{
    VOS_UINT8                           ucMmcMmStartCnfFlg;
    
    VOS_UINT8                           aucReserve[3];
    VOS_UINT32                          ulMmcReadSimFileFlg;
    NAS_MMC_SENDING_AS_SWITCH_ON_SEQUENCE_CTRL_STRU         stSwitchOnRecord; 

    /* ��Ž���㿪���ظ���Ϣ�� */
    NAS_MMC_AS_SWITCH_ON_CNF_INFO_STRU  astSwitchOnCnfInfoList[NAS_MML_MAX_RAT_NUM];           

    /* ����ǰNV�б����IMSI������ */
    VOS_UINT8                           aucLastImsi[NAS_MML_MAX_IMSI_LEN];
    VOS_UINT8                           aucReserved[3];
}NAS_MMC_FSM_SWITCH_ON_CTX_STRU;


typedef struct
{
    VOS_UINT8                           ucMmcMmPowerOffCnfFlg;                  /* ���ڼ�¼�Ƿ���Ҫ�ȴ�MM��GMM�Ļظ���Ϣ��־ */
    VOS_UINT8                           ucMmcAsPowerOffCnfFlg;                  /* ���ڼ�¼�Ƿ���Ҫ�ȴ������:WAS/GAS/LMM�Ļظ���Ϣ��־ */

    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enMasterModeRat;                        /* �ػ�ʱ��¼�ĵ�ǰ����ģ */    
    VOS_UINT8                           aucReserve[1];
    NAS_MML_PLMN_RAT_PRIO_STRU          stSlaveModeList;
}NAS_MMC_FSM_POWER_OFF_CTX_STRU;

/*****************************************************************************
 �ṹ��    : NAS_MMC_FSM_ANYCELL_SEARCH_CTX_STRU
 �ṹ˵��  : Anycell�������ƽṹ
*****************************************************************************/
typedef struct
{
    NAS_MML_PLMN_RAT_PRIO_STRU          stMmcPlmnRatPrio;
    VOS_UINT8                           aucSearchedFlag[NAS_MML_MAX_RAT_NUM];   /* ��ʶ�Ƿ��������Ľ��뼼��:VOS_TRUE ���ѹ���VOS_FALSE ��δ�ѹ� */
    VOS_UINT8                           ucAbortFlag;                            /* Abort��Ϣ���ڱ�־:VOS_TRUE��ʾ����,VOS_FALSE��ʾ������ */
}NAS_MMC_FSM_ANYCELL_SEARCH_CTX_STRU;


typedef struct
{
    VOS_UINT8                          ucMmcSuspendRspFlg;                      /* MMC����ظ��ı�־ */
    VOS_UINT8                          ucMmcResumeRspFlg;                       /* MMC�ָ��ظ��ı�־ */
    VOS_UINT8                          ucAbortFlg;                              /* �Ƿ���ֹ״̬����־,VOS_TRUE:��ʾ�յ�״̬���Ĵ����Ϣ;VOS_FALSE:��ʾδ�յ�״̬���Ĵ����Ϣ */
    VOS_UINT8                          ucSndSuspendRelReqFlg;

    MMC_RESUME_ORIGEN_ENUM_UINT8       enResumeOrign;
    VOS_UINT8                          aucReserve[3];

}NAS_MMC_FSM_INTER_SYS_CELLRESEL_CTX_STRU;
typedef struct
{
    VOS_UINT8                          ucMmcSuspendRspFlg;                      /* MMC����ظ��ı�־ */
    VOS_UINT8                          ucMmcResumeRspFlg;                       /* MMC�ָ��ظ��ı�־ */
    VOS_UINT8                          aucReserve[2];
}NAS_MMC_FSM_INTER_SYS_OOS_CTX_STRU;


typedef struct
{
    VOS_UINT8                           ucMmcSuspendRspFlg;                     /* MMC����ظ��ı�־ */
    VOS_UINT8                           ucMmcResumeRspFlg;                      /* MMC�ָ��ظ��ı�־ */
    VOS_UINT8                           ucCsSigExistFlg;                        /* �л�ʱ����CS�������� */    
    VOS_UINT8                           ucPsSigExistFlg;                        /* �л�ʱ����PS�������� */
}NAS_MMC_FSM_INTER_SYS_HO_CTX_STRU;



typedef struct
{
    VOS_UINT8                           ucMmcSuspendRspFlg;                     /* MMC����ظ��ı�־ */
    VOS_UINT8                           ucMmcResumeRspFlg;                      /* MMC�ָ��ظ��ı�־ */
    VOS_UINT8                           aucReserve[2];
}NAS_MMC_FSM_INTER_SYS_CCO_CTX_STRU;


/*****************************************************************************
 �ṹ����: NAS_MMC_LOW_PLMN_INFO_STRU
 �ṹ˵��: ������PLMN��Ϣ�ṹ��
 1.��    ��   : 2011��7��11��
   ��    ��   : sunxibo 46746
   �޸�����   : ����
*****************************************************************************/
typedef struct
{
    NAS_MML_PLMN_ID_STRU                stPlmnId;           /* PLMN ID    */
    VOS_INT32                           lRscp;              /* RSCP����ֵ */
}NAS_MMC_LOW_PLMN_INFO_STRU;


typedef struct
{
    VOS_UINT8                           ucSearchedFlag;                         /* ��ʶ�Ƿ��������Ľ��뼼�� */
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat;                                  /* ��Ҫ�����Ľ��뼼�� */
    VOS_UINT8                           aucResevel[2];
}NAS_MMC_PLMN_LIST_SEARCH_CTRL_STRU;


typedef struct
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRatType;                                                  /* ����Ľ��뼼�� */
    NAS_MMC_COVERAGE_TYPE_ENUM_UINT8    enCoverType;                                                /* ��ǰ���뼼���µĸ������� */
    VOS_UINT8                           aucReserve[2];
    VOS_UINT32                          ulHighPlmnNum;                                              /* ������PLMN�ĸ��� */
    NAS_MML_PLMN_ID_STRU                astHighPlmnList[NAS_MMC_INTRA_MAX_HIGH_QUALITY_PLMN_NUM];   /* ������PLMN�б�   */
    VOS_UINT32                          ulLowPlmnNum;                                               /* ������PLMN�ĸ��� */
    NAS_MMC_LOW_PLMN_INFO_STRU          astLowPlmnList[NAS_MMC_INTRA_MAX_LOW_QUALITY_PLMN_NUM];     /* ������PLMN�б�   */
}NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU;


typedef struct
{
    VOS_UINT8                                               ucWaitRegRsltFlag;                       /* �ȴ� CS/PS��ע���־ */
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enCsRegAdditionalAction;                 /* ��ǰ����CS��ע�����ĸ��Ӵ��� */
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8                    enPsRegAdditionalAction;                 /* ��ǰ����PS��ע�����ĸ��Ӵ��� */
    VOS_UINT8                                               aucReserve[1];
}NAS_MMC_PLMN_LIST_REG_RSLT_INFO_STRU;
typedef struct
{
    NAS_MMC_PLMN_LIST_SEARCH_CTRL_STRU                      astPlmnListCtrlInfo[NAS_MML_MAX_RAT_NUM];/* LIST����������������Ϣ*/
    NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU                    astPlmnSearchInfo[NAS_MML_MAX_RAT_NUM];  /* LIST�����������ѵ��������б���Ϣ */
    NAS_MMC_PLMN_LIST_REG_RSLT_INFO_STRU                    stRegRsltInfo;                           /* LIST�����ȴ�ע�����Լ��ȴ���·�ͷ�ʱ�յ���ע������Ϣ */
    VOS_UINT8                                               ucAbortFlg;                              /* �Ƿ���ֹ״̬����־,VOS_TRUE:��ʾ�յ�״̬���Ĵ����Ϣ;VOS_FALSE:��ʾδ�յ�״̬���Ĵ����Ϣ */
    VOS_UINT8                                               ucNotifyMmaFlg;                          /* �Ƿ���MMA֪ͨ���,VOT_TRUE:��ʾMMA��ʱ����ʱ��ֹLIST����;VOT_FALSE:��ʾδ�յ�MMA��ʱ����ʱ��ABORT��Ϣ*/

    VOS_UINT8                                               ucPreLteDisableFlg;               /* ����BG����ʱL�Ƿ��Ѿ�disable̬��������List�ѹ����д���disable LTE�ĳ�����
                                                                                                List�ѹ���disable lte�˳�״̬��ʱ��Ҫ��disable����������,VOS_TRUE:����List
                                                                                                ��ʱL�Ѿ�disable��VOS_FALSE:����List��ʱLδdisable*/

    VOS_UINT8                                               ucAnyCellSrchFlg;               /* LIST����֮ǰ����Ч,�������Ƿ���Ҫ����anycell����,����Ѿ�����anycell����,
                                                                                               ��������Ҫ�ظ�����anycell����.VOS_TRUE:LIST����������Ҫ����anycell����
                                                                                               VOS_FALSE:LIST������������Ҫanycell���� */
}NAS_MMC_FSM_PLMN_LIST_CTX_STRU;



typedef struct
{
    VOS_UINT8                       ucSetCurrSyscfgIndex;                       /* ָ����Ҫ����SYSCFG����Ӧģ�Ľ��뼼�����±� */
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8 aucSyscfgRatPrio[NAS_MML_MAX_RAT_NUM];      /* ���SYSCFG������Ҫ�����������Ⱥ�˳�� */
    VOS_UINT8                       ucTotalSyscfgNum;                           /* ����������SYSCFG���ܸ��� */
    VOS_UINT8                       aucResevel[3];

}NAS_MMC_SYSCFG_SETTING_CTRL_STRU;
typedef struct
{
    VOS_UINT8                           ucRelRequestFlg;                  /* �Ƿ��������������Ƿ�,VOS_TRUE:����������, VOS_FALSE:�����ȴ��ͷ� */
    VOS_UINT8                           aucReserve[3];

    VOS_UINT32                          ulNeedSearchPlmnFlag;                   /* syscfg�����Ƿ���Ҫ������ʶ */
    NAS_MMC_SYSCFG_SETTING_CTRL_STRU    stSyscfgSettingRecord;                  /* ��¼��������·�SYSCFG���õ��Ⱥ�˳���Լ���ǰ�������õĽ���� */

    NAS_MML_PLMN_RAT_PRIO_STRU          stPrePrioRatList;                       /* ��syscfg״̬��ǰ�Ľ��뼼�������ȼ� */
}NAS_MMC_FSM_SYSCFG_CTX_STRU;


typedef struct
{
    VOS_UINT8                                 ucRelRequestFlg;                  /* �Ƿ������������ӱ�־,VOS_TRUE:����������, VOS_FALSE:�����ȴ��ͷ� */
    VOS_UINT8                                 ucWaitRegRsltFlag;                /* �ȴ� CS/PS��ע���־ */
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8      enCsRegAdditionalAction;          /* ��ǰ����CS��ע�����ĸ��Ӵ��� */
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8      enPsRegAdditionalAction;          /* ��ǰ����PS��ע�����ĸ��Ӵ��� */
}NAS_MMC_BG_PLMN_SEARCH_REG_INFO_STRU;


typedef struct
{
    VOS_UINT32                                ulAbortEventType;                 /* ����¼�ID */

    NAS_MMC_ABORT_FSM_TYPE_UINT8              enAbortType;                      /* ������յ���ֹ����,��¼��ֹ������ */

    VOS_UINT8                                 ucNeedSndSysInfo;                 /* �Ƿ���Ҫ������MMA�ϱ�ϵͳ��Ϣ */

#if (FEATURE_ON == FEATURE_LTE)
    VOS_UINT8                                 ucPreLteDisableFlg;               /* ����BG����ʱL�Ƿ��Ѿ�disable̬��������BG�ѹ����д���disable LTE�ĳ�����
                                                                                   BG�ѹ���disable lte�˳�״̬��ʱ��Ҫ��disable����������,VOS_TRUE:����BG
                                                                                   ��ʱL�Ѿ�disable��VOS_FALSE:����BG��ʱLδdisable*/
    VOS_UINT8                                 ucNeedEnableLteFlg;               /* �����ж��ڿ���ָ����ʱ�Ƿ���Ҫenable LTE */
#else
    VOS_UINT8                                 aucReserve[2];
#endif
    NAS_MMC_BG_PLMN_SEARCH_REG_INFO_STRU      stCurHighPrioPlmnRegInfo;         /* ����״̬����ע����������Ϣ */
    NAS_MML_PLMN_WITH_RAT_STRU                stCurHighPrioPlmn;                /* ��ǰ���ڳ��Եĸ����ȼ����缰����뼼��,���ڿ���ָ������ʧ�ܺ󣬸��¸�����״̬ */
    NAS_MML_PLMN_WITH_RAT_STRU                stPreCampPlmn;                    /* ����BG����ʱפ����PLMN��BG״̬����ע����ɺ��PLMN������뼼�� */
    NAS_MML_EQUPLMN_INFO_STRU                 stPreEquPlmnInfo;                 /* ����BG����ʱ��BG״̬����ע����ɺ������Ӧ��EQUPLMN��Ϣ */
}NAS_MMC_FSM_BG_PLMN_SEARCH_CTX_STRU;
typedef union
{
    NAS_MMC_FSM_SWITCH_ON_CTX_STRU                      stSwitchOnCtx;
    NAS_MMC_FSM_POWER_OFF_CTX_STRU                      stPowerOffCtx;
    NAS_MMC_FSM_ANYCELL_SEARCH_CTX_STRU                 stAnyCellSearchCtx;
    NAS_MMC_FSM_PLMN_SELECTION_CTX_STRU                 stPlmnSelectionCtx;

    NAS_MMC_FSM_L1_MAIN_CTX_STRU                        stL1MainCtx;
    NAS_MMC_FSM_INTER_SYS_OOS_CTX_STRU                  stInterSysOosCtx;
    NAS_MMC_FSM_INTER_SYS_CELLRESEL_CTX_STRU            stInterSysCellReselCtx;
    NAS_MMC_FSM_INTER_SYS_HO_CTX_STRU                   stInterSysHoCtx;
    NAS_MMC_FSM_INTER_SYS_CCO_CTX_STRU                  stInterSysCcoCtx;

    NAS_MMC_FSM_SYSCFG_CTX_STRU                         stSyscfgCtx;

    NAS_MMC_FSM_BG_PLMN_SEARCH_CTX_STRU                 stBgPlmnSearchCtx;

    NAS_MMC_FSM_PLMN_LIST_CTX_STRU                      stPlmnListCtx;
}NAS_MMC_FSM_EXTRA_CTX_UNION;


typedef struct
{
    /* ״̬������ָ��,ָ��ΪNULL��ʾ��ǰ״̬����Ч  */
    NAS_FSM_DESC_STRU                  *pstFsmDesc;

    /* ��ǰ״̬����ʶ */
    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;

    /* �ϲ�״̬����ʶ */
    NAS_MMC_FSM_ID_ENUM_UINT32          enParentFsmId;

    /* �ϲ�״̬�������Ϣ���� */
    VOS_UINT32                          ulParentEventType;

    
    /* �ϲ�״̬����������ջ��ַ */
    NAS_MMC_FSM_EXTRA_CTX_UNION        *punParentFsmCtx;

    /* ��ǰ״̬ */
    VOS_UINT32                          ulState;                                /* L1״̬ */

    /* �����ϢMSGID������  */
    NAS_MMC_ENTRY_MSG_STRU              stEntryMsg;

    /* ״̬������ */
    NAS_MMC_FSM_EXTRA_CTX_UNION         unFsmCtx;
}NAS_MMC_FSM_CTX_STRU;

/*****************************************************************************
 �ṹ��    : NAS_MMC_FSM_STACK_STRU
 �ṹ˵��  : ״̬��ջ�����ṹ
 1.��    ��   : 2011��5��28��
   ��    ��   : zhoujun 40661
   �޸�����   : �½�

 2.��    ��   : 2011��10��9��
   ��    ��   : zhoujun /40661
   �޸�����   : ����ջPOP�ı�־λ
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          usStackDepth;                           /* ��ǰѹջ��״̬���� */
    VOS_UINT16                          usStackPopFlg;                          /* ��ǰջpop��־ */
    NAS_MMC_FSM_CTX_STRU                astFsmStack[NAS_MMC_MAX_STACK_DEPTH];   /* ״̬��ջ����� */
}NAS_MMC_FSM_STACK_STRU;


/*****************************************************************************
 �ṹ��    : NAS_MMC_CACH_MSG_INFO_STRU
 �ṹ˵��  : �������Ϣ����
 1.��    ��   : 2011��5��28��
   ��    ��   : zhoujun 40661
   �޸�����   : �½�
*****************************************************************************/
typedef struct
{
    NAS_MMC_ENTRY_MSG_STRU              stMsgEntry;     /* ����ľ������� */
}NAS_MMC_CACH_MSG_INFO_STRU;


/*****************************************************************************
 �ṹ��    : NAS_MMC_CACH_MSG_INFO_STRU
 �ṹ˵��  : �������Ϣ����
 1.��    ��   : 2011��5��28��
   ��    ��   : zhoujun 40661
   �޸�����   : �½�
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ucCurrIndex;                            /* ��¼��ǰ�������index */
    VOS_UINT8                           ucCacheMsgNum;                          /* ������Ϣ�ĸ��� */
    VOS_UINT8                           aucReserve[3];
    NAS_MMC_CACH_MSG_INFO_STRU          astMsgQueue[NAS_MMC_MAX_MSG_QUEUE_NUM]; /* MMC����Ϣ�������飬�洢���Ǵ�ͨ����Ϣͷ����Ϣ */
}NAS_MMC_MSG_QUEUE_STRU;
typedef struct
{
    NAS_MML_PROC_TYPE_ENUM_U32          enProcType;         /* ��ǰ��ҵ����� */
    NAS_MML_REG_RESULT_ENUM_UINT8       enRegRst;           /* �ù��̵Ĵ����� */
    NAS_MMC_REG_DOMAIN_ENUM_UINT8       enReqDomain;        /* �ù��̵ķ����� */
    NAS_MMC_REG_DOMAIN_ENUM_UINT8       enRstDomain;        /* �ù��̵Ľ���� */
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCnCause;          /* ���̵�causeֵ��ʧ��ʱΪ��Ӧԭ��ֵ���ɹ�ʱ��ΪNULL */
    VOS_UINT32                          ulAttemptCount;     /* ��ǰ���̵ĳ��Դ��� */

}NAS_MMC_GU_ACTION_RSLT_INFO_STRU;


typedef struct
{
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8             enRatType;                      /* ����ģʽ */
    VOS_UINT8                                   ucSimPsRegStatus;               /* ��״̬��VOS_TRUE��ʾ����Ч��VOS_FALSE��ʾ����Ч */
    VOS_UINT8                                   aucReserved[2];
}NAS_MMC_SRV_STA_CHNG_INFO_STRU;



typedef struct
{    
    VOS_UINT8                           ucPlmnSrchNoRfFlg;                      /* ��ǰ�Ƿ���NO RF���µ�����ʧ�ܣ�������ϱ�NO RF������ΪVOS_TRUE,MMC��������ʱ������ΪVOS_FALSE */
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRatType;                              /* �ϱ�NO RFʱ��Ľ��뼼�� */
    VOS_UINT8                           aucReserved[2]; 
}NAS_MMC_PLMN_SEARCH_NO_RF_INFO_STRU;



typedef struct
{
    VOS_UINT8                                   ucUtranSkipWPlmnSearchFlag;

    VOS_UINT8                                   ucNcellSearchFlag;              /* �Ƿ�֧��ncell������ʶ��1:֧�֣�0:��֧�֣�
                                                                                 ͨ��Modem1��GSM�ϱ���L��TDS������Ϣ, Modem0��֧��GSM������£�
                                                                                 T/L�������ܹ����ݴ��ݵ�����Ƶ�����������TDS/LTE;
                                                                                 ���Modem1���ݹ�����������Ϣ�����ڵ�����£�Ҳ��ͨ����ʷƵ��֧��NCELL����*/
    VOS_UINT8                                   ucNcellSearchFirstTimerLen;     /* ��һ�׶�����Ƶ������������Ե�ʱ���� ����λ�룩*/
    VOS_UINT8                                   ucNcellSearchSecondTimerLen;    /* �ڶ��׶�����Ƶ������������Ե�ʱ���� ����λ�룩*/
}NAS_MMC_DSDA_PLMN_SEARCH_ENHANCED_CFG_STRU;


typedef struct
{
    VOS_UINT8                           ucTdsArfcnNum;                              /* 0��ʾTDDƵ�㲻���� */
    VOS_UINT8                           ucReserved;
    VOS_UINT16                          ausTdsArfcnList[NAS_MMC_TDD_ARFCN_MAX_NUM];
}NAS_MMC_TDS_NCELL_INFO_STRU;
typedef struct
{
    VOS_UINT8                           ucLteArfcnNum;                              /* 0��ʾLTEƵ�㲻���� */
    VOS_UINT8                           aucReserved[3];
    VOS_UINT16                          ausLteArfcnList[NAS_MMC_LTE_ARFCN_MAX_NUM];

    VOS_UINT8                           ucLteCellNum;                              /* 0��ʾLTE��ϢID������ */
    VOS_UINT8                           aucReserved1[3];
    VOS_UINT16                          ausLteCellList[NAS_MMC_LTE_CELL_MAX_NUM];

}NAS_MMC_LTE_NCELL_INFO_STRU;



typedef struct
{
    NAS_MMC_TDS_NCELL_INFO_STRU         stTdsNcellInfo;
    NAS_MMC_LTE_NCELL_INFO_STRU         stLteNcellInfo;
    NAS_MML_EQUPLMN_INFO_STRU           stOtherModemEplmnInfo;
}NAS_MMC_NCELL_SEARCH_INFO_STRU;



typedef struct
{
    NAS_MMC_PLMN_SELECTION_MODE_ENUM_UINT8        enSelectionMode;              /* MMC��ǰ����ģʽ,�Զ�ģʽ���ֶ�ģʽ*/
    VOS_UINT8                                     ucWrrLimitServiceInd;         /* �յ�WRR�Ľ������Ʒ���ָʾ,VOS_FALSE:δ�յ�,VOS_TRUE:�յ� */

    NAS_MMC_AS_CELL_CAMP_ON_ENUM_UINT8            enAsCellCampOn;               /* ������Ƿ�Camp On */
    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8     enSpecPlmnSearchState;        /* NAS�Ƿ�Ϊ��פ������״̬ */

    /*  �û��ֶ�ָ�������ע��״̬,VOS_FALSE:δע��ɹ�,VOS_TRUE:��ע��ɹ�
        �û��ֶ�ָ�������ע��״̬��ά��:
     1. ���յ��û�ָ���ѵĴ�����ڴ� ����ñ�־;
     2. ���û��ϱ�ָ�������ɹ��ĵط� ���øñ�־, ��������ʱ����RPLMN����EPLMN;
     3. PSע��ɹ�, CSע��ʧ��, ԭ��Ϊ11 12 13 15�����, ��Ҫ����ñ�־.
        ��Ϊ��ʱCS��RPLMNû�и���Ϊ�û�ָ����PLMN, �˺����ȴ���������������
        RPLMN����EPLMN, ����Ҫ����ñ�־.
        CSע��ɹ�, PSע��ʧ��ʱ, ��ȻRPLMN���û�ָ��PLMN��ͬ, ��Ϊ�˴���Գ�,
        Ҳ�������־�Ĵ���. */
    VOS_UINT8                                     ucUserSpecPlmnRegStatus;

    NAS_MMC_REG_CONTROL_ENUM_UINT8                enRegCtrl;
    VOS_UINT8                                     ucAsAnyCampOn;
    VOS_UINT8                                     aucReserve[1];

    NAS_MMC_BUFFERED_PLMN_SEARCH_INFO_STRU        stBufferedPlmnSearchInfo;     /* ��¼�����������־�Լ�����������Ϣ */

    NAS_MMC_DSDA_PLMN_SEARCH_ENHANCED_CFG_STRU    stDsdaPlmnSearchEnhancedCfg;

    NAS_MML_PLMN_ID_STRU                          stOtherModemPlmnId;           /* ����פ��plmn id��Ϣ��ȫfΪ��Чֵ */

    NAS_MMC_NCELL_SEARCH_INFO_STRU                stNcellSearchInfo;

    NAS_MML_PLMN_WITH_RAT_STRU                    stUserSpecPlmnId;             /* �û��ֶ�����ʱ�����PLMNID�ͽ��뼼�� */
    NAS_MML_PLMN_WITH_RAT_STRU                    stUserReselPlmnId;            /* ָ������ǰפ����PLMNID�ͽ��뼼��,��ͬʱָ������ע��ɹ���Ҳ����¸�ֵ */

    NAS_MMC_SRV_STA_CHNG_INFO_STRU                stLastSrvStaInfo;             /* �ϴ�֪ͨ����ģ�鵱ǰ����ģʽ�Ϳ�״̬�ļ�¼ */

    NAS_MMC_PLMN_SEARCH_NO_RF_INFO_STRU           stPlmnSrchNoRfInfo;
    NAS_MML_PLMN_ID_STRU                                    stLastCampedPlmnId;                      /*�洢����ʱ�ϴ�פ����PLMN*/

}NAS_MMC_PLMN_SEARCH_CTRL_CTX_STRU;


typedef struct
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                   stHighPrioPlmnSearchListInfo; /* ��¼�����ȼ������б���Ϣ(HPLMN+UPLMN+OPLMN֧�ֵĽ��뼼��������״̬)*/
    VOS_UINT8                                               ucFirstStartHPlmnTimerFlg;    /* ��¼�Ƿ�Ϊ�״�����HPLMN��ʱ��:VOS_TRUE:�״�����HPLMN��ʱ����VOS_FALSE:���״�����HPLMN��ʱ�� */
    NAS_MMC_PLMN_SELECTION_LIST_TYPE_ENUM_UINT8             enPlmnSelectionListType; 
    VOS_UINT8                                               aucReserve[2];
}NAS_MMC_HIGH_PRIO_PLMN_SEARCH_CTRL_CTX_STRU;



typedef struct
{
    MMA_MMC_NET_SCAN_REQ_STRU          stNetScanReq;
}NAS_MMC_NET_SCAN_REQ_CTX_STRU;

/*****************************************************************************
 �ṹ��    : NAS_MMC_HPLMN_REG_REJ_STRU
 �ṹ˵��  : HPLMN���ܵ�λ����,·�����Լ�������,��ҪΪPlmn not allow��ԭ��ֵhplmn��Ҫ���⴦��
 1.��    ��   : 2011��6��30��
   ��    ��   : zhoujun 40661
   �޸�����   : ����
*****************************************************************************/
typedef struct
{
    VOS_UINT16                          aucLac[NAS_MML_MAX_LAC_LEN];/* λ���� */
    VOS_UINT8                           ucRac;                      /* ·���� */
    VOS_UINT8                           ucRegRejCnDomain;           /* ���ܾ��ķ����� NAS_MMC_REG_DOMAIN_ENUM_UINT8*/
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enRejCause;                 /* ���ܾ���ԭ��ֵ */
    NAS_MML_PLMN_ID_STRU                stHplmnId;                  /* ���ܾ���PLMN ID */
}NAS_MMC_HPLMN_REG_REJ_STRU;
typedef struct{
    NAS_MML_PLMN_ID_STRU                   stPlmnId;
    VOS_UINT16                             aucLac[NAS_MML_MAX_LAC_LEN];         /* λ���� */
    VOS_UINT8                              ucRac;                               /* ·���� */
    VOS_UINT8                              ucReserved;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16     enWPsRegStatus;                      /* W�����ע��״̬����ʼ����ע��ɹ�ʱΪNULL */
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16     enWCsRegStatus;                      /* W�����ע��״̬����ʼ����ע��ɹ�ʱΪNULL */
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16     enGPsRegStatus;                      /* G�����ע��״̬����ʼ����ע��ɹ�ʱΪNULL */
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16     enGCsRegStatus;                      /* G�����ע��״̬����ʼ����ע��ɹ�ʱΪNULL */
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16     enLCsRegStatus;                         /* L�����ע��״̬����ʼ��ΪNULL */
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16     enLPsRegStatus;                         /* L�����ע��״̬����ʼ��ΪNULL */
#endif
}NAS_MMC_PLMN_REG_INFO_STRU;                     /*  ��Ӧ�����ע����Ϣ           */

typedef struct
{
    VOS_UINT8                               ucPlmnRegInfoNum;                   /* ��ǰ��ע��ԭ���б��� ���ڵ� Plmn���� */
    VOS_UINT8                               aucReserve[3];
    NAS_MMC_PLMN_REG_INFO_STRU              astPlmnRegInfoList[NAS_MMC_MAX_REG_PLMN_INFO_NUM]; /*���ο�����, Plmn��ע����Ϣ */
}NAS_MMC_PLMN_REG_REJ_CTX_STRU;

/*****************************************************************************
 �ṹ��    : NAS_MMC_SERVICE_INFO_CTX_STRU
 �ṹ˵��  : NAS MMC ��ǰ�ķ���״̬
 1.��    ��   : 2011��6��30��
   ��    ��   : zhoujun 40661
   �޸�����   : ����
*****************************************************************************/
typedef struct
{
    NAS_MMC_SERVICE_ENUM_UINT8          enCsCurrService;                        /* ��ǰCS��ķ���״̬ */
    NAS_MMC_SERVICE_ENUM_UINT8          enPsCurrService;                        /* ��ǰPS��ķ���״̬ */
    VOS_UINT8                           aucReserve[2];
}NAS_MMC_SERVICE_INFO_CTX_STRU;


typedef struct
{
    VOS_UINT8                                               ucGcfCh9_4_3_AND_Ch26_7_4_3;            /* GCF��׮�����Ƿ���Ч,VOS_FALSE:����Ч,VOS_TRUE:��Ч */
    NAS_MMC_AT_ACTION_TYPE_ENUM_UINT8                       ucActionType;       /* GU����������ϱ���Ϣ�� enActionType��ȡֵ */
    NAS_MMC_AT_MSG_TYPE_ENUM_UINT8                          ucMsgType;          /* GU����������ϱ���Ϣ�� enMsgType��ȡֵ */

    VOS_UINT8                                               ucSignThreshold;
    VOS_UINT8                                               ucMinRptTimerInterval;     /* ����ϱ���ʱ��   */
    VOS_UINT8                                               aucReserve[3];
}NAS_MMC_MAINTAIN_CTX_STRU;





typedef struct
{
    /**************************************************************************
                        NAS MMC״̬��״̬����״̬ջ
    **************************************************************************/
    NAS_MMC_FSM_CTX_STRU                stCurFsm;                               /* ״̬����ǰ��״̬������ */
    NAS_MMC_FSM_STACK_STRU              stFsmStack;                             /* ״̬����״̬��ջ����   */

    NAS_MMC_PLMN_SEARCH_CTRL_CTX_STRU   stPlmnSearchCtrl;                       /* PLMN�������������� */
    NAS_MMC_PLMN_REG_REJ_CTX_STRU       stPlmnRegInfo;                          /* PLMNע��������ʧ��ԭ��ֵ������ */
    NAS_MMC_SERVICE_INFO_CTX_STRU       stServiceInfo;                          /* MMC��ǰ�ķ���״̬ */
    NAS_MMC_MAINTAIN_CTX_STRU           stMaintainInfo;                         /* ��ά�ɲ������� */
    NAS_MMC_TIMER_CTX_STRU              astMmcTimerCtx[NAS_MMC_MAX_TIMER_NUM];  /* MMC��ǰ�������еĶ�ʱ����Դ */

    NAS_MMC_HIGH_PRIO_PLMN_SEARCH_CTRL_CTX_STRU   stHighPrioPlmnSearchCtrl;     /* �����ȼ��������������� */

    NAS_MMC_NET_SCAN_REQ_CTX_STRU       stNetScanReqCtx;

    /**************************************************************************
                        NAS MMC������Ϣ����
    **************************************************************************/
    NAS_MMC_MSG_QUEUE_STRU              stBufferEntryMsgQueue;

}NAS_MMC_CONTEXT_STRU;

extern NAS_MMC_CONTEXT_STRU                    g_stNasMmcCtx;

#if (FEATURE_ON == FEATURE_LTE)
#ifdef WIN32
extern TAF_PLMN_ID_STRU                        g_stSetRplmn;
#endif
#endif
/*****************************************************************************
  8 UNION����
*****************************************************************************/


/*****************************************************************************
  9 OTHERS����
*****************************************************************************/



/*****************************************************************************
  10 ��������
*****************************************************************************/
VOS_VOID NAS_MMC_SaveCurEntryMsg(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
);

VOS_VOID NAS_MMC_FSM_InitFsmL2(
    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId
);
VOS_VOID NAS_MMC_FSM_QuitFsmL2( VOS_VOID );

VOS_VOID  NAS_MMC_FSM_SetCurrState(
    VOS_UINT32                          ulCurrState
);


VOS_UINT32 NAS_MMC_GetFsmTopState( VOS_VOID );

VOS_UINT16  NAS_MMC_GetFsmStackDepth( VOS_VOID );

NAS_MMC_TIMER_CTX_STRU*  NAS_MMC_GetTimerAddr( VOS_VOID );

VOS_UINT32  NAS_MMC_SaveCacheMsg(
    VOS_UINT32                          ulEventType,
    VOS_VOID                           *pMsg
);


VOS_UINT32  NAS_MMC_GetNextCachedMsg(
    NAS_MMC_ENTRY_MSG_STRU             *pstEntryMsg
);

VOS_UINT32  NAS_MMC_ClearCacheMsg(
    VOS_UINT32                          ulEventType
);

VOS_UINT32  NAS_MMC_ClearAllCacheMsg( VOS_VOID );

VOS_UINT32  NAS_MMC_IsExistCacheMsg(
    VOS_UINT32                          ulEventType,
    VOS_UINT32                         *pulIndex
);


VOS_VOID  NAS_MMC_InitFsmCtx_L1Main(VOS_VOID);

VOS_VOID  NAS_MMC_InitFsmCtx_SwitchOn(VOS_VOID);

NAS_MMC_SENDING_AS_SWITCH_ON_SEQUENCE_CTRL_STRU* NAS_MMC_GetSwitchOnSequenceRecord_SwitchOn(VOS_VOID);
NAS_MML_NET_RAT_TYPE_ENUM_UINT8 NAS_MMC_GetNextSendingRat_SwitchOn(VOS_VOID);
VOS_VOID NAS_MMC_AddSwitchOnIndex_SwitchOn(VOS_VOID);
VOS_VOID NAS_MMC_SetSwitchOnSendingAsOrder_SwitchOn(VOS_VOID);

VOS_UINT8* NAS_MMC_GetLastImsi_SwitchOn(VOS_VOID);

VOS_VOID  NAS_MMC_InitFsmCtx_PowerOff(VOS_VOID);


VOS_VOID  NAS_MMC_InitFsmCtx_PlmnSelection(VOS_VOID);

VOS_VOID  NAS_MMC_InitFsmCtx_BgPlmnSearch(VOS_VOID);

VOS_VOID  NAS_MMC_InitFsmCtx_AnyCellSearch(VOS_VOID);

VOS_VOID  NAS_MMC_InitFsmCtx_InterSysCellResel(VOS_VOID);

VOS_VOID  NAS_MMC_InitFsmCtx_InterSysHo(VOS_VOID);

VOS_VOID  NAS_MMC_InitFsmCtx_InterSysCco(VOS_VOID);

VOS_VOID  NAS_MMC_InitFsmCtx_InterSysOos(VOS_VOID);

VOS_VOID  NAS_MMC_InitFsmCtx_PlmnList(VOS_VOID);


VOS_VOID  NAS_MMC_InitFsmStackCtx(
    NAS_MMC_FSM_STACK_STRU              *pstFsmStack
);

VOS_VOID  NAS_MMC_InitInternalBuffer(
    NAS_MMC_MSG_QUEUE_STRU             *pstBufferEntryMsgQueue
);

VOS_VOID NAS_MMC_InitNetScanReqCtx(
    NAS_MMC_NET_SCAN_REQ_CTX_STRU      *pstNetScanReqCtx
);

VOS_VOID  NAS_MMC_InitCurrFsmCtx(
    NAS_MMC_FSM_CTX_STRU                *pstCurrFsmCtx
);

VOS_VOID  NAS_MMC_InitPlmnSearchCtrlCtx(
    NAS_MMC_INIT_CTX_TYPE_ENUM_UINT8     enInitType,
    NAS_MMC_PLMN_SEARCH_CTRL_CTX_STRU   *pstPlmnSearchCtrl
);

VOS_VOID  NAS_MMC_InitServiceInfo(
    NAS_MMC_SERVICE_INFO_CTX_STRU       *pstServiceInfo
);


VOS_VOID  NAS_MMC_InitPlmnRegRejCtx(
    NAS_MMC_PLMN_REG_REJ_CTX_STRU           *pstPlmnRegRejInfo
);

VOS_VOID  NAS_MMC_InitMaintainCtx(
    NAS_MMC_MAINTAIN_CTX_STRU           *pstMaintainInfo
);
NAS_MML_PLMN_WITH_RAT_STRU *NAS_MMC_GetUserSpecPlmnId(VOS_VOID);

VOS_UINT32 NAS_MMC_GetUserSpecPlmnRegisterStatus(VOS_VOID);

NAS_MML_PLMN_WITH_RAT_STRU *NAS_MMC_GetUserReselPlmnId(VOS_VOID);

VOS_VOID NAS_MMC_SetUserReselPlmnId(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
);

VOS_VOID NAS_MMC_SetUserSpecPlmnRegisterStatus(
    VOS_UINT8                           ucRegisterStatus
);

VOS_VOID NAS_MMC_SetUserSpecPlmnId(
    NAS_MML_PLMN_WITH_RAT_STRU         *pstPlmnInfo
);

NAS_MML_PLMN_ID_STRU* NAS_MMC_GetLastCampedPlmnid( VOS_VOID );
VOS_UINT32 NAS_MMC_SaveLastCampedPlmnid( 
    VOS_UINT32                   ulMcc,
    VOS_UINT32                   ulMnc
);

NAS_MMC_PLMN_SELECTION_MODE_ENUM_UINT8 NAS_MMC_GetPlmnSelectionMode(VOS_VOID);

VOS_VOID NAS_MMC_SetPlmnSelectionMode(
    NAS_MMC_PLMN_SELECTION_MODE_ENUM_UINT8      enSelectionMode
);

NAS_MMC_AS_CELL_CAMP_ON_ENUM_UINT8 NAS_MMC_GetAsCellCampOn(VOS_VOID);

VOS_VOID NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON_ENUM_UINT8   enAsCellCampOn);

NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8 NAS_MMC_GetSpecPlmnSearchState(VOS_VOID);

VOS_VOID NAS_MMC_SetSpecPlmnSearchState(NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8 enSpecPlmnSearchState);

NAS_MMC_ENTRY_MSG_STRU* NAS_MMC_GetCurrEntryMsg(VOS_VOID);

NAS_MMC_PLMN_SEARCH_CTRL_CTX_STRU *NAS_MMC_GetPlmnSearchCtrl(VOS_VOID);

NAS_MMC_PLMN_REG_REJ_CTX_STRU *NAS_MMC_GetPlmnRegRejInfo(VOS_VOID);

NAS_MMC_SERVICE_INFO_CTX_STRU *NAS_MMC_GetServiceInfo(VOS_VOID);

NAS_MMC_MAINTAIN_CTX_STRU *NAS_MMC_GetMaintainInfo(VOS_VOID);

VOS_UINT32 NAS_MMC_IsCsNormalService( VOS_VOID );

VOS_UINT32 NAS_MMC_IsPsNormalService( VOS_VOID );

VOS_UINT32 NAS_MMC_IsNormalServiceStatus(VOS_VOID);

NAS_MMC_SERVICE_ENUM_UINT8  NAS_MMC_ConverMmStatusToMmc(
    NAS_MMC_REG_DOMAIN_ENUM_UINT8           enRegDomain,
    NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8    enServiceStatus
);


NAS_MMC_SERVICE_ENUM_UINT8 NAS_MMC_GetCsServiceStatus(VOS_VOID);

NAS_MMC_SERVICE_ENUM_UINT8 NAS_MMC_GetPsServiceStatus(VOS_VOID);

VOS_VOID NAS_MMC_UpdateDomainServiceStatus(
    VOS_UINT8                           enCurrDomain,
    NAS_MMC_SERVICE_ENUM_UINT8          enCurrService
);


VOS_VOID    NAS_MMC_SetCsServiceStatus(NAS_MMC_SERVICE_ENUM_UINT8   enCsCurrService);


VOS_VOID    NAS_MMC_SetPsServiceStatus(NAS_MMC_SERVICE_ENUM_UINT8   enPsCurrService);

VOS_UINT32  NAS_MMC_GetNextAvailableTimerValue(VOS_VOID);

NAS_MMC_SYSCFG_SETTING_CTRL_STRU* NAS_MMC_GetSysCfgSettingRecord_SysCfg(VOS_VOID);
VOS_VOID NAS_MMC_AddSysCfgIndex_SysCfg(VOS_VOID);

NAS_MML_NET_RAT_TYPE_ENUM_UINT8 NAS_MMC_GetNextSettingRat_SysCfg(VOS_VOID);
VOS_VOID NAS_MMC_InitFsmCtx_SysCfg(VOS_VOID);
VOS_UINT32 NAS_MMC_GetPlmnSearchFlag_SysCfg(VOS_VOID);
VOS_VOID NAS_MMC_SetPlmnSearchFlag_SysCfg(VOS_UINT32 ulNeedSearchPlmnFlag);

VOS_VOID NAS_MMC_SetPrePrioRatList_SysCfg(
    NAS_MML_PLMN_RAT_PRIO_STRU          *pstPrePrioRatList
);

NAS_MML_PLMN_RAT_PRIO_STRU* NAS_MMC_GetPrePrioRatList_SysCfg(VOS_VOID);

NAS_MMC_SYSCFG_SETTING_CTRL_STRU* NAS_MMC_GetSysCfgSettingRecord_SysCfg(VOS_VOID);



/* L1 MAIN״̬�������Ĳ������� */

VOS_UINT32  NAS_MMC_GetCurNormalAvailableTimerCount_L1Main(VOS_VOID);

VOS_VOID    NAS_MMC_ResetCurNormalAvailableTimerCount_L1Main(VOS_VOID);


VOS_VOID    NAS_MMC_AddCurNormalAvailableTimerCount_L1Main(VOS_VOID);

VOS_VOID   NAS_MMC_SetCurNormalAvailableTimerCount(
    VOS_UINT32                          ulCurTimerCount
);

VOS_VOID  NAS_MMC_AddAvailableTimerCount_OnPlmn(VOS_VOID);



VOS_UINT32  NAS_MMC_GetCurNcellSearchTimerCount_OnPlmn(VOS_VOID);
VOS_VOID  NAS_MMC_ResetCurNcellSearchTimerCount_OnPlmn(VOS_VOID);
VOS_VOID  NAS_MMC_AddCurNcellSearchTimerCount_OnPlmn(VOS_VOID);
VOS_UINT8  NAS_MMC_IsNeedStartNcellAvailableTimer(VOS_VOID);

VOS_VOID  NAS_MMC_SetAvailableTimerType_OnPlmn(
    NAS_MMC_AVAILABLE_TIMER_TYPE_ENUM_UINT8                 enAvailableTimerType
);
NAS_MMC_AVAILABLE_TIMER_TYPE_ENUM_UINT8  NAS_MMC_GetAvailableTimerType_OnPlmn(VOS_VOID);


NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_GetCsRegAdditionalAction_L1Main(VOS_VOID);
NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_GetPsRegAdditionalAction_L1Main(VOS_VOID);

VOS_VOID NAS_MMC_SetCsRegAdditionalAction_L1Main(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enCsAddtionalAction
);

VOS_VOID NAS_MMC_SetPsRegAdditionalAction_L1Main(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8  enPsAddtionalAction
);

VOS_VOID NAS_MMC_ClearAllRegAdditionalAction_L1Main(VOS_VOID);

VOS_VOID NAS_MMC_SetCsRegAttemptCount_L1Main(
    VOS_UINT32      ulCsCount
);

VOS_VOID NAS_MMC_SetPsRegAttemptCount_L1Main(
    VOS_UINT32      ulPsCount
);

VOS_UINT32 NAS_MMC_GetCsRegAttemptCount_L1Main(VOS_VOID);

VOS_UINT32 NAS_MMC_GetPsRegAttemptCount_L1Main(VOS_VOID);

VOS_VOID NAS_MMC_ClearAllRegAttemptCount_L1Main(VOS_VOID);

/* �������״̬�������Ĳ������� */
NAS_MMC_WAIT_MM_START_CNF_ENUM_UINT8    NAS_MMC_GetWaitMmStartCnfFlg_SwitchOn(VOS_VOID);
NAS_MML_READ_SIM_FILES_CNF_ENUM_UINT32   NAS_MMC_GetWaitSimFilesCnfFlg_SwitchOn(VOS_VOID);

VOS_VOID    NAS_MMC_ClearMmStartCnfFlg_SwitchOn(NAS_MMC_WAIT_MM_START_CNF_ENUM_UINT8    enMmStartCnfFlg);
VOS_VOID    NAS_MMC_ClearAllWaitMmStartCnfFlg_SwitchOn(VOS_VOID);
VOS_VOID    NAS_MMC_ClearWaitSimFilesCnfFlg_SwitchOn(NAS_MML_READ_SIM_FILES_CNF_ENUM_UINT32  enSimFileCnfFlg);
VOS_VOID    NAS_MMC_ClearAllWaitSimFilesCnfFlg_SwitchOn(VOS_VOID);

VOS_VOID    NAS_MMC_SetWaitMmStartCnfFlg_SwitchOn(NAS_MMC_WAIT_MM_START_CNF_ENUM_UINT8    enMmStartCnfFlg);
VOS_VOID    NAS_MMC_SetWaitSimFilesCnfFlg_SwitchOn(NAS_MML_READ_SIM_FILES_CNF_ENUM_UINT32 enSimFileCnfFlg);

/* �ػ����״̬�������Ĳ������� */
NAS_MMC_MM_POWEROFF_FLAG_ENUM_UINT8     NAS_MMC_GetMmPowerOffCnfFlg_PowerOff(VOS_VOID);
NAS_MMC_AS_POWEROFF_FLAG_ENUM_UINT8     NAS_MMC_GetAsPowerOffCnfFlg_PowerOff(VOS_VOID);

VOS_VOID    NAS_MMC_SetMmPowerOffCnfFlag_PowerOff(NAS_MMC_MM_POWEROFF_FLAG_ENUM_UINT8   enMmPowerOffFlg);
VOS_VOID    NAS_MMC_SetAsPowerOffCnfFlag_PowerOff(NAS_MMC_AS_POWEROFF_FLAG_ENUM_UINT8   enAsPowerOffFlg);

VOS_VOID    NAS_MMC_ClearMmPowerOffCnfFlag_PowerOff(NAS_MMC_MM_POWEROFF_FLAG_ENUM_UINT8   enMmPowerOffFlg);
NAS_MML_NET_RAT_TYPE_ENUM_UINT8 NAS_MMC_GetMasterModeRat_PowerOff(VOS_VOID);

VOS_VOID NAS_MMC_SetMasterModeRat_PowerOff(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enMasterModeRat
);

NAS_MML_PLMN_RAT_PRIO_STRU* NAS_MMC_GetSlaveModeRatList_PowerOff(VOS_VOID);
VOS_VOID NAS_MMC_BuildSlaveModeRatList_PowerOff(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enMasterModeRat
);
VOS_VOID    NAS_MMC_ClearAsPowerOffCnfFlag_PowerOff(NAS_MMC_AS_POWEROFF_FLAG_ENUM_UINT8   enAsPowerOffFlg);
VOS_VOID    NAS_MMC_ClearAllMmPowerOffCnfFlag_PowerOff(VOS_VOID);
VOS_VOID    NAS_MMC_ClearAllAsPowerOffCnfFlag_PowerOff(VOS_VOID);

/* ��ϵͳ��ѡ���״̬�������Ĳ������� */
NAS_MMC_SUSPEND_RSP_FLAG_ENUM_UINT8  NAS_MMC_GetSuspendRspFlg_InterSysCellResel(VOS_VOID);
NAS_MMC_RESUME_RSP_FLAG_ENUM_UINT8   NAS_MMC_GetResumeRspFlg_InterSysCellResel(VOS_VOID);

MMC_RESUME_ORIGEN_ENUM_UINT8 NAS_MMC_GetResumeOrign_InterSysCellResel(VOS_VOID);
VOS_VOID NAS_MMC_SetResumeOrign_InterSysCellResel(
    MMC_RESUME_ORIGEN_ENUM_UINT8        enResumeOrign
);

VOS_VOID NAS_MMC_ClearSuspendRspFlag_InterSysCellResel(NAS_MMC_SUSPEND_RSP_FLAG_ENUM_UINT8 enMmSuspendRsp);
VOS_VOID NAS_MMC_ClearResumeRspFlag_InterSysCellResel(NAS_MMC_RESUME_RSP_FLAG_ENUM_UINT8 enMmResumeRsp);

VOS_VOID NAS_MMC_SetSuspendRspFlag_InterSysCellResel(NAS_MMC_SUSPEND_RSP_FLAG_ENUM_UINT8 enMmSuspendRsp);
VOS_VOID NAS_MMC_SetResumeRspFlag_InterSysCellResel(NAS_MMC_RESUME_RSP_FLAG_ENUM_UINT8 enMmResumeRsp);
VOS_VOID NAS_MMC_ClearAllSuspendRspFlag_InterSysCellResel(VOS_VOID);
VOS_VOID NAS_MMC_ClearAllResumeRspFlag_InterSysCellResel(VOS_VOID);
VOS_VOID NAS_MMC_SetAbortFlag_InterSysCellResel(VOS_UINT8   ucAbortFlg);
VOS_UINT8 NAS_MMC_GetAbortFlag_InterSysCellResel(VOS_VOID);
VOS_VOID NAS_MMC_SetSndSuspendRelReqFlg_InterSysCellResel(VOS_UINT8    ucSndSuspendRelReqFlg);
VOS_UINT8 NAS_MMC_GetSndSuspendRelReqFlg_InterSysCellResel(VOS_VOID);



/* ��ϵͳOOS���״̬�������Ĳ������� */
NAS_MMC_SUSPEND_RSP_FLAG_ENUM_UINT8  NAS_MMC_GetSuspendRspFlg_InterSysOos(VOS_VOID);
NAS_MMC_RESUME_RSP_FLAG_ENUM_UINT8   NAS_MMC_GetResumeRspFlg_InterSysOos(VOS_VOID);

VOS_VOID NAS_MMC_ClearSuspendRspFlag_InterSysOos(NAS_MMC_SUSPEND_RSP_FLAG_ENUM_UINT8 enMmSuspendRsp);
VOS_VOID NAS_MMC_ClearResumeRspFlag_InterSysOos(NAS_MMC_RESUME_RSP_FLAG_ENUM_UINT8 enMmResumeRsp);

VOS_VOID NAS_MMC_SetSuspendRspFlag_InterSysOos(NAS_MMC_SUSPEND_RSP_FLAG_ENUM_UINT8 enMmSuspendRsp);
VOS_VOID NAS_MMC_SetResumeRspFlag_InterSysOos(NAS_MMC_RESUME_RSP_FLAG_ENUM_UINT8 enMmResumeRsp);
VOS_VOID NAS_MMC_ClearAllSuspendRspFlag_InterSysOos(VOS_VOID);
VOS_VOID NAS_MMC_ClearAllResumeRspFlag_InterSysOos(VOS_VOID);

/* ��ϵͳHO���״̬�������Ĳ������� */
NAS_MMC_SUSPEND_RSP_FLAG_ENUM_UINT8  NAS_MMC_GetSuspendRspFlg_InterSysHo(VOS_VOID);
NAS_MMC_RESUME_RSP_FLAG_ENUM_UINT8   NAS_MMC_GetResumeRspFlg_InterSysHo(VOS_VOID);

VOS_VOID NAS_MMC_ClearSuspendRspFlag_InterSysHo(NAS_MMC_SUSPEND_RSP_FLAG_ENUM_UINT8 enMmSuspendRsp);
VOS_VOID NAS_MMC_ClearResumeRspFlag_InterSysHo(NAS_MMC_RESUME_RSP_FLAG_ENUM_UINT8 enMmResumeRsp);

VOS_VOID NAS_MMC_SetSuspendRspFlag_InterSysHo(NAS_MMC_SUSPEND_RSP_FLAG_ENUM_UINT8 enMmSuspendRsp);
VOS_VOID NAS_MMC_SetResumeRspFlag_InterSysHo(NAS_MMC_RESUME_RSP_FLAG_ENUM_UINT8 enMmResumeRsp);
VOS_VOID NAS_MMC_ClearAllSuspendRspFlag_InterSysHo(VOS_VOID);
VOS_VOID NAS_MMC_ClearAllResumeRspFlag_InterSysHo(VOS_VOID);

VOS_VOID NAS_MMC_SetCsSigExistFlag_InterSysHo(
    VOS_UINT8       ucSigExistFlg
);
VOS_VOID NAS_MMC_SetPsSigExistFlag_InterSysHo(
    VOS_UINT8       ucSigExistFlg
);
VOS_UINT8  NAS_MMC_GetCsSigExistFlag_InterSysHo(VOS_VOID);
VOS_UINT8  NAS_MMC_GetPsSigExistFlag_InterSysHo(VOS_VOID);


/* ��ϵͳCCO���״̬�������Ĳ������� */
NAS_MMC_SUSPEND_RSP_FLAG_ENUM_UINT8  NAS_MMC_GetSuspendRspFlg_InterSysCco(VOS_VOID);
NAS_MMC_RESUME_RSP_FLAG_ENUM_UINT8   NAS_MMC_GetResumeRspFlg_InterSysCco(VOS_VOID);

VOS_VOID NAS_MMC_ClearSuspendRspFlag_InterSysCco(NAS_MMC_SUSPEND_RSP_FLAG_ENUM_UINT8 enMmSuspendRsp);
VOS_VOID NAS_MMC_ClearResumeRspFlag_InterSysCco(NAS_MMC_RESUME_RSP_FLAG_ENUM_UINT8 enMmResumeRsp);

VOS_VOID NAS_MMC_SetSuspendRspFlag_InterSysCco(NAS_MMC_SUSPEND_RSP_FLAG_ENUM_UINT8 enMmSuspendRsp);
VOS_VOID NAS_MMC_SetResumeRspFlag_InterSysCco(NAS_MMC_RESUME_RSP_FLAG_ENUM_UINT8 enMmResumeRsp);

VOS_VOID NAS_MMC_ClearAllSuspendRspFlag_InterSysCco(VOS_VOID);
VOS_VOID NAS_MMC_ClearAllResumeRspFlag_InterSysCco(VOS_VOID);

RRMM_RAT_PRIO_ENUM_UINT8 NAS_MMC_GetSpecRatPrio(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat,
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstRatList
);

VOS_UINT32 NAS_MMC_GetAbortFlag_PlmnSelection(VOS_VOID);

VOS_VOID NAS_MMC_SetAbortFlag_PlmnSelection(
    VOS_UINT8                          ucAbortFlg
);

VOS_UINT32 NAS_MMC_GetRelRequestFlag_PlmnSelection(VOS_VOID);

VOS_VOID NAS_MMC_SetRelRequestFlag_PlmnSelection(
    VOS_UINT8                           ucRelRequestFlg
);

VOS_VOID NAS_MMC_SetInterSysSuspendRat_PlmnSelection(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
);

NAS_MML_NET_RAT_TYPE_ENUM_UINT8 NAS_MMC_GetInterSysSuspendRat_PlmnSelection(VOS_VOID);

NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU * NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection(VOS_VOID);

VOS_UINT8 NAS_MMC_GetAllBandSearch_PlmnSelection(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
);

VOS_VOID NAS_MMC_SetAllBandSearch_PlmnSelection(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat,
    VOS_UINT8                           ucAllBandSearch
);

VOS_VOID NAS_MMC_SetRatCoverage_PlmnSelection(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat,
    NAS_MMC_COVERAGE_TYPE_ENUM_UINT8    enCoverType
);

NAS_MMC_RAT_SEARCH_INFO_STRU* NAS_MMC_GetRatCoverage_PlmnSelection(VOS_VOID);

VOS_UINT8 NAS_MMC_GetSearchRplmnAndHplmnFlg_PlmnSelection(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
);

VOS_VOID NAS_MMC_SetSearchRplmnAndHplmnFlg_PlmnSelection(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat,
    VOS_UINT8                           ucSearchRplmnAndHplmnFlg
);


VOS_VOID NAS_MMC_InitSearchRatInfo_PlmnSelection(VOS_VOID);

NAS_MML_FORBIDPLMN_ROAMING_LAS_INFO_STRU* NAS_MMC_GetForbRoamLaInfo_PlmnSelection(VOS_VOID);


NAS_MML_PLMN_ID_STRU* NAS_MMC_GetForbPlmnForGrpsInfo_PlmnSelection(VOS_VOID);

NAS_MML_PLMN_WITH_RAT_STRU* NAS_MMC_GetCurrSearchingPlmn_PlmnSelection(VOS_VOID);
VOS_VOID NAS_MMC_SetCurrSearchingPlmn_PlmnSelection(
    NAS_MML_PLMN_WITH_RAT_STRU         *pstPlmnId
);


#if (FEATURE_ON == FEATURE_LTE)
VOS_VOID NAS_MMC_SetCsPsMode1ReCampLtePlmn_PlmnSelection(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId
);

NAS_MML_PLMN_ID_STRU* NAS_MMC_GetCsPsMode1ReCampLtePlmn_PlmnSelection(VOS_VOID);
#endif

VOS_UINT32 NAS_MMC_GetAbortFlag_AnyCellSearch(VOS_VOID);
VOS_VOID NAS_MMC_SetAbortFlag_AnyCellSearch(
    VOS_UINT8                           ucAbortFlg
);
NAS_MMC_FSM_ANYCELL_SEARCH_CTX_STRU* NAS_MMC_GetFsmCtxAddr_AnyCellSearch(VOS_VOID);

VOS_UINT8 NAS_MMC_GetRatNum_AnyCellSearch(VOS_VOID);

NAS_MML_NET_RAT_TYPE_ENUM_UINT8 NAS_MMC_GetNextSearchRat_AnyCellSearch(VOS_VOID);

VOS_VOID NAS_MMC_SetSearchedFlag_AnyCellSearch(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enSpecRat
);

VOS_VOID NAS_MMC_SetRatPrioList_AnyCellSearch(
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstRatPrioList
);




NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16 NAS_MMC_GetCsRegCause_PlmnSelection(VOS_VOID);
NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16 NAS_MMC_GetPsRegCause_PlmnSelection(VOS_VOID);
VOS_VOID NAS_MMC_SetCsRegCause_PlmnSelection(
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCsCause
);
VOS_VOID NAS_MMC_SetPsRegCause_PlmnSelection(
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enPsCause
);

VOS_VOID NAS_MMC_ClearAllWaitRegRsltFlag_PlmnSelection(VOS_VOID);
VOS_UINT8 NAS_MMC_GetWaitRegRsltFlag_PlmnSelection(VOS_VOID);
VOS_VOID NAS_MMC_SetWaitRegRsltFlag_PlmnSelection(VOS_UINT8 usWaitFlag);
VOS_VOID NAS_MMC_ClearSingleDomainWaitRegRsltFlag_PlmnSelection(
    VOS_UINT8                           ucRegDomain
);

NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_GetCsRegAdditionalAction_PlmnSelection(VOS_VOID);

NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_GetPsRegAdditionalAction_PlmnSelection(VOS_VOID);
VOS_VOID NAS_MMC_SetCsRegAdditionalAction_PlmnSelection(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enCsAddtionalAction
);
VOS_VOID NAS_MMC_SetPsRegAdditionalAction_PlmnSelection(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8  enPsAddtionalAction
);



VOS_VOID NAS_MMC_SetSysCfgSendingOrder_SysCfg(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enDestRat
);

VOS_UINT32 NAS_MMC_IsCurrentPlmnIdRoamingAllowed_SysCfg(
    MMA_MMC_SYS_CFG_SET_REQ_STRU        *pstSysCfgSetParm
);


VOS_UINT8 NAS_MMC_GetRelRequestFlag_SysCfg(VOS_VOID);
VOS_VOID NAS_MMC_SetRelRequestFlag_SysCfg(
    VOS_UINT8                           ucRelRequestFlg
);

/******************LIST ����״̬�������Ĳ������� **********************/
VOS_VOID    NAS_MMC_InitSearchRatInfo_PlmnList(VOS_VOID);

NAS_MMC_PLMN_LIST_SEARCH_CTRL_STRU *NAS_MMC_GetSearchCtrlInfo_PlmnList(VOS_VOID);

VOS_UINT32  NAS_MMC_GetNextSearchingRat_PlmnList(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8    *penRat
);

VOS_VOID    NAS_MMC_GetOrigenSearchRat_PlmnList(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8    *penRat
);

VOS_VOID    NAS_MMC_SetSearchedFlag_PlmnList(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enSpecRat
);

VOS_UINT8   NAS_MMC_GetAbortFlag_PlmnList(VOS_VOID);
VOS_VOID    NAS_MMC_SetAbortFlag_PlmnList(
    VOS_UINT8                           ucAbortFlg
);

VOS_UINT8   NAS_MMC_GetNotifyMmaFlag_PlmnList(VOS_VOID);
VOS_VOID    NAS_MMC_SetNotifyMmaFlag_PlmnList(
    VOS_UINT8                           ucNotifyFlg
);

VOS_UINT8   NAS_MMC_GetWaitRegRsltFlag_PlmnList(VOS_VOID);
VOS_VOID    NAS_MMC_SetWaitRegRsltFlag_PlmnList(
    VOS_UINT8       ucWaitFlag
);

VOS_VOID    NAS_MMC_ClearSingleDomainWaitRegRsltFlag_PlmnList(
    VOS_UINT8       ucRegDomain
);
VOS_VOID    NAS_MMC_ClearAllWaitRegRsltFlag_PlmnList(VOS_VOID);

NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_GetCsRegAdditionalAction_PlmnList(VOS_VOID);

NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_GetPsRegAdditionalAction_PlmnList(VOS_VOID);

VOS_VOID NAS_MMC_SetCsRegAdditionalAction_PlmnList(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enCsAddtionalAction
);
VOS_VOID NAS_MMC_SetPsRegAdditionalAction_PlmnList(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8  enPsAddtionalAction
);

NAS_MMC_PLMN_LIST_REG_RSLT_INFO_STRU *NAS_MMC_GetRegRsltInfo_PlmnList(VOS_VOID);

VOS_VOID    NAS_MMC_InitSearchedPlmnListInfo_PlmnList(VOS_VOID);

NAS_MMC_SEARCHED_PLMN_LIST_INFO_STRU *NAS_MMC_GetSearchedPlmnListInfo_PlmnList(VOS_VOID);

VOS_VOID    NAS_MMC_UpdateSearchedPlmnListInfo_PlmnList(
    RRC_PLMN_ID_LIST_STRU              *pstRrcList
);


#if (FEATURE_ON == FEATURE_LTE)
VOS_UINT8 NAS_MMC_GetPreLteDisableFlg_PlmnList(VOS_VOID);
VOS_VOID NAS_MMC_SetPreLteDisableFlg_PlmnList(
    VOS_UINT8                           ucPreLteDisableFlg
);
#endif


VOS_UINT8   NAS_MMC_GetAnyCellSrchFlg_PlmnList(VOS_VOID);
VOS_VOID    NAS_MMC_SetAnyCellSrchFlg_PlmnList(
    VOS_UINT8                           ucPreLteDisableFlg
);


VOS_VOID NAS_MMC_InitHighPrioPlmnSearchCtx(NAS_MMC_HIGH_PRIO_PLMN_SEARCH_CTRL_CTX_STRU *pstHighPrioPlmnSearchCtrl);


MMA_MMC_NET_SCAN_REQ_STRU *NAS_MMC_GetNetScanReq(VOS_VOID);

VOS_VOID NAS_MMC_SetNetScanReq(
    MMA_MMC_NET_SCAN_REQ_STRU          *pstNetScanReq
);

NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU *NAS_MMC_GetHighPrioPlmnList(VOS_VOID);
VOS_UINT16 NAS_MMC_GetHighPrioPlmnNum(VOS_VOID);

NAS_MMC_PLMN_SELECTION_LIST_TYPE_ENUM_UINT8 NAS_MMC_GetPlmnSelectionListType(VOS_VOID);
VOS_VOID NAS_MMC_SetPlmnSelectionListType(
    NAS_MMC_PLMN_SELECTION_LIST_TYPE_ENUM_UINT8             enPlmnSelectionListType       
);

VOS_UINT32 NAS_MMC_GetEHPlmn(
    RRC_PLMN_ID_STRU                   *pstEHPlmn,
    VOS_UINT8                          *pucEHPlmnNum
);


VOS_VOID NAS_MMC_SetPlmnSelectionMode(
    NAS_MMC_PLMN_SELECTION_MODE_ENUM_UINT8  enSelectionMode
);

VOS_VOID NAS_MMC_SaveUserReselPlmnInfo(VOS_VOID);

NAS_MMC_CONTEXT_STRU* NAS_MMC_GetMmcCtxAddr(VOS_VOID);
NAS_MMC_FSM_CTX_STRU* NAS_MMC_GetCurFsmAddr(VOS_VOID);
NAS_MMC_FSM_STACK_STRU* NAS_MMC_GetFsmStackAddr(VOS_VOID);
NAS_FSM_DESC_STRU* NAS_MMC_GetCurFsmDesc(VOS_VOID);
NAS_MMC_ENTRY_MSG_STRU* NAS_MMC_GetCurrFsmMsgAddr(VOS_VOID);
NAS_MMC_FSM_ID_ENUM_UINT32 NAS_MMC_GetCurrFsmId(VOS_VOID);
VOS_UINT32 NAS_MMC_GetCurrFsmEventType(VOS_VOID);
NAS_MMC_FSM_ID_ENUM_UINT32 NAS_MMC_GetParentFsmId(VOS_VOID);
VOS_UINT32 NAS_MMC_GetParentFsmEventType(VOS_VOID);

NAS_MMC_FSM_EXTRA_CTX_UNION* NAS_MMC_GetParentFsmCtx(VOS_VOID);
    
NAS_MMC_MSG_QUEUE_STRU* NAS_MMC_GetCachMsgBufferAddr(VOS_VOID);

VOS_VOID NAS_MMC_UpdatePlmnRegInfoList(
    NAS_MML_PLMN_ID_STRU               *pstPlmn,
    NAS_MMC_REG_DOMAIN_ENUM_UINT8       enDomain,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCause
);


VOS_VOID NAS_MMC_GetHplmnRejDomainInfo(
    VOS_UINT8                          *pucRejDomainRlst
);

VOS_VOID NAS_MMC_ClearHplmnRejDomainInfo(
    NAS_MML_PLMN_ID_STRU               *pstPlmn,
    VOS_UINT8                           ucAcceptCnDomain
);

VOS_VOID  NAS_MMC_SetPlmnRegRejInfo(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    NAS_MMC_REG_DOMAIN_ENUM_UINT8       enDomain,
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16  enCause,
    NAS_MMC_PLMN_REG_INFO_STRU         *pstPlmnRegInfo
);

VOS_UINT32 NAS_MMC_GetAbortEventType_BgPlmnSearch(VOS_VOID);
VOS_VOID NAS_MMC_SetAbortEventType_BgPlmnSearch(
    VOS_UINT32                          ulAbortEventType
);


NAS_MMC_ABORT_FSM_TYPE_UINT8 NAS_MMC_GetAbortType_BgPlmnSearch(VOS_VOID);

VOS_VOID NAS_MMC_SetAbortType_BgPlmnSearch(
    NAS_MMC_ABORT_FSM_TYPE_UINT8        enAbortType
);

VOS_UINT8 NAS_MMC_GetRelRequestFlag_BgPlmnSearch(VOS_VOID);

VOS_VOID NAS_MMC_SetRelRequestFlag_BgPlmnSearch(
    VOS_UINT8                           ucRelRequestFlg
);

VOS_UINT8 NAS_MMC_GetWaitRegRsltFlag_BgPlmnSearch(VOS_VOID);

VOS_VOID NAS_MMC_SetWaitRegRsltFlag_BgPlmnSearch(
    VOS_UINT8                           ucWaitFlag
);

VOS_VOID NAS_MMC_ClearSingleDomainWaitRegRsltFlag_BgPlmnSearch(
    VOS_UINT8                           ucRegDomain
);

VOS_VOID NAS_MMC_ClearAllWaitRegRsltFlag_BgPlmnSearch(VOS_VOID);

NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_GetCsRegAdditionalAction_BgPlmnSearch(VOS_VOID);

VOS_VOID NAS_MMC_SetCsRegAdditionalAction_BgPlmnSearch(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enCsAddtionalAction
);

NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 NAS_MMC_GetPsRegAdditionalAction_BgPlmnSearch(VOS_VOID);

VOS_VOID NAS_MMC_SetPsRegAdditionalAction_BgPlmnSearch(
    NAS_MMC_ADDITIONAL_ACTION_ENUM_UINT8 enPsAddtionalAction
);

NAS_MML_PLMN_WITH_RAT_STRU* NAS_MMC_GetCurHighPrioPlmn_BgPlmnSearch(VOS_VOID);

VOS_VOID NAS_MMC_SetCurHighPrioPlmn_BgPlmnSearch(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
);

NAS_MML_PLMN_WITH_RAT_STRU* NAS_MMC_GetPreCampPlmn_BgPlmnSearch(VOS_VOID);

VOS_VOID NAS_MMC_SetPreCampPlmn_BgPlmnSearch(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
);
VOS_VOID NAS_MMC_SetPreCampPlmnRat_BgPlmnSearch(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
);
VOS_VOID NAS_MMC_SetPreEquPlmnInfo_BgPlmnSearch(
    NAS_MML_PLMN_ID_STRU               *pstCurrCampPlmnId,
    NAS_MML_EQUPLMN_INFO_STRU          *pstEquPlmnInfo
);

NAS_MML_EQUPLMN_INFO_STRU* NAS_MMC_GetPreEquPlmnInfo_BgPlmnSearch(VOS_VOID);
VOS_VOID NAS_MMC_SetPreLteDisableFlg_BgPlmnSearch(
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapStatus
);

VOS_UINT32 NAS_MMC_GetPreLteDisableFlg_BgPlmnSearch(VOS_VOID);


VOS_VOID NAS_MMC_SetNeedEnableLteFlg_BgPlmnSearch(
    VOS_UINT8                                               ucNeedEnableLteFlg);
VOS_UINT32 NAS_MMC_GetNeedEnableLteFlg_BgPlmnSearch(VOS_VOID);

NAS_MML_EQUPLMN_INFO_STRU* NAS_MMC_GetPreEquPlmnInfo_BgPlmnSearch(VOS_VOID);
VOS_VOID NAS_MMC_SetFirstStartHPlmnTimerFlg(
    VOS_UINT8                           ucFirstStartHPlmnTimer
);

VOS_UINT8 NAS_MMC_GetFirstStartHPlmnTimerFlg(VOS_VOID);

VOS_UINT32  NAS_MMC_GetHplmnTimerLen( VOS_VOID );

VOS_UINT32  NAS_MMC_GetCacheNum( VOS_VOID );

VOS_UINT16  NAS_MMC_GetFsmStackPopFlg( VOS_VOID ) ;

VOS_VOID  NAS_MMC_SetFsmStackPopFlg(
    VOS_UINT16                          ucStachPopFlg
);

VOS_UINT32 NAS_MMC_IsFsmEntryEventType(
    VOS_UINT32                          ulEventType
);

VOS_VOID NAS_MMC_SetBufferedPlmnSearchInfo(
    VOS_UINT32                                              ulPlmnSearchFlg,
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene
);


VOS_UINT32  NAS_MMC_GetBufferedPlmnSearchFlg( VOS_VOID );

NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32  NAS_MMC_GetBufferedPlmnSearchScene( VOS_VOID );

VOS_VOID NAS_MMC_UpdateGURegRlstRPlmnIdInNV(VOS_VOID);

NAS_MMC_SRV_STA_CHNG_INFO_STRU* NAS_MMC_GetSrvStaChngInfo( VOS_VOID );

VOS_UINT32  NAS_MMC_IsFsmIdInFsmStack(
    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId
);

VOS_UINT8 NAS_MMC_GetNeedSndSysInfo_BgPlmnSearch(VOS_VOID);
VOS_VOID NAS_MMC_SetNeedSndSysInfo_BgPlmnSearch(
    VOS_UINT8                           ucNeedSndSysInfo
);

VOS_UINT32 NAS_MMC_NeedTrigPlmnSrch_UserSpecCurrentPlmn(VOS_VOID);


VOS_VOID NAS_MMC_DelMsUnsupportRat(
    NAS_MML_PLMN_RAT_PRIO_STRU         *pstRatPrioList
);

VOS_VOID NAS_MMC_SetUtranSkipWPlmnSearchFlag(VOS_UINT8 ucUtranSkipWPlmnSearchFlag);
VOS_UINT8 NAS_MMC_GetUtranSkipWPlmnSearchFlag(VOS_VOID);
VOS_VOID NAS_MMC_SetOtherModemPlmnId(NAS_MML_PLMN_ID_STRU *pstPlmnId);
NAS_MML_PLMN_ID_STRU* NAS_MMC_GetOtherModemPlmnId(VOS_VOID);


VOS_UINT8 NAS_MMC_GetNcellSearchFlag(VOS_VOID);
VOS_VOID NAS_MMC_SetNcellSearchFlag(VOS_UINT8 ucNcellSearchFlag);


VOS_UINT8 NAS_MMC_GetNcellSearchFirstTimerLen(VOS_VOID);
VOS_VOID NAS_MMC_SetNcellSearchFirstTimerLen(VOS_UINT8 ucNcellSearchTimerLen);

VOS_VOID NAS_MMC_SetNcellSearchSecondTimerLen(VOS_UINT8 ucNcellSearchTimerLen);
VOS_UINT8 NAS_MMC_GetNcellSearchSecondTimerLen(VOS_VOID);


NAS_MMC_NCELL_SEARCH_INFO_STRU* NAS_MMC_GetNcellSearchInfo(VOS_VOID);
VOS_VOID NAS_MMC_SetNcellSearchInfo(NAS_MMC_NCELL_SEARCH_INFO_STRU *pstNcellSearchInfo);


VOS_UINT32 NAS_MMC_GetWaitWasPlmnSrchCnfTimerLen(VOS_VOID);

VOS_UINT8  NAS_MMC_GetPlmnSrchNoRfFlg( VOS_VOID );
NAS_MML_NET_RAT_TYPE_ENUM_UINT8 NAS_MMC_GetPlmnSrchNoRfRatType( VOS_VOID );
VOS_VOID  NAS_MMC_SetPlmnSrchNoRfFlg( 
    VOS_UINT8                           ucPlmnSrchNoRfFlg
);
VOS_VOID  NAS_MMC_SetCurrRatPlmnSrchNoRfFlg(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRatType,
    VOS_UINT8                           ucPlmnSrchNoRfFlg
);

VOS_UINT32  NAS_MMC_GetHighPrioRatHplmnTimerLen( VOS_VOID );

VOS_UINT32  NAS_MMC_GetHighPrioRatHplmnTimerRetryLen( VOS_VOID );

VOS_UINT8  NAS_MMC_GetHighPrioRatHplmnTimerActiveFlg( VOS_VOID );

VOS_UINT32  NAS_MMC_GetCurHighPrioRatHplmnTimerFirstSearchCount_L1Main(VOS_VOID);

VOS_VOID    NAS_MMC_ResetCurHighPrioRatHplmnTimerFirstSearchCount_L1Main(VOS_VOID);

VOS_VOID    NAS_MMC_AddCurHighPrioRatHplmnTimerFirstSearchCount_L1Main(VOS_VOID);

NAS_MMC_REG_CONTROL_ENUM_UINT8 NAS_MMC_GetRegCtrl( VOS_VOID );

VOS_VOID NAS_MMC_SetRegCtrl(
    NAS_MMC_REG_CONTROL_ENUM_UINT8      enRegCtrl
);

VOS_UINT8 NAS_MMC_GetAsAnyCampOn( VOS_VOID );

VOS_VOID NAS_MMC_SetAsAnyCampOn(
    VOS_UINT8                          ucAsAnyCampOn
);

VOS_VOID NAS_MMC_SaveRegReqNCellInfo(
    MMA_MMC_REG_CELL_INFO_STRU         *pstCellInfo
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

#endif /* end of NasMmcCtx.h */
