

#ifndef _NASLMMPUBMOM_H
#define _NASLMMPUBMOM_H


/*****************************************************************************
  1 ����ͷ�ļ�����
*****************************************************************************/
#include    "vos.h"
#include    "OmCommon.h"
#include    "NasEmmPubUGlobal.h"
#include    "NasEmmPubUMain.h"

#include    "NasEmmTauInterface.h"
#include    "NasEmmSerInterface.h"
#include    "LnasFtmInterface.h"
#include    "LnasErrlogInterface.h"


/*#include	"IpDhcpv4Server.h"*/
/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  #pragma pack(*)    �����ֽڶ��뷽ʽ
*****************************************************************************/
#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

/*****************************************************************************
  2 macro
*****************************************************************************/

#define NAS_EMM_AIR_DATA_ARRAY_LEN      4

#define NAS_EMM_TP_CMD_HEADER           0x00000000

#define NAS_EMM_EVEN_IMSI               0x01
#define NAS_EMM_ODD_IMSI                0x09
#define MAS_EMM_IMSI_PAD_LEN            0x02

#define NAS_EMM_LOW_4_BIT               0x0F
#define NAS_EMM_HIGH_4_BIT              0xF0

#define NAS_EMM_MNC_LEN2                2
#define NAS_EMM_MNC_LEN3                3
#define NAS_EMM_NV_READ_FAIL            0

#define NAS_EMM_DHCP_SERVER_BUFFER_LEN  400

/* niuxiufan DT begin */
#define NAS_DT_RPT_CTRL_MAX_ITEM_NUM         10
/* niuxiufan DT end */

#define NAS_SAVE_RECEIVE_MSG_INFO_NUM       16

/*LTE CS*/
#define NAS_LMM_LTE_CS_SERVICE_NOT_SUPPORT    0
#define NAS_LMM_LTE_CS_SERVICE_SUPPORT        1

/* niuxiufan DT begin */
#define NAS_EMM_GetNasDtRptCtrlItem(ulIndex)   (&(gastNasDtRptCtrlTbl[ulIndex]))

/* niuxiufan DT end */

extern VOS_UINT32                  g_NasEmmOmInfoIndFlag;


#define NAS_EMM_GetDrxCycleLenChangeFlag()   (g_ulDrxCycleLenChange)
#define NAS_EMM_SetDrxCycleLenChangeFlag(DrxCycleLenChangeFlag)\
                             ((g_ulDrxCycleLenChange) = (DrxCycleLenChangeFlag))


#define EMM_COMP_OMT_MSG_HEADER(pMsg,MsgLenthNoHeader)\
        {\
            (pMsg)->ulSenderCpuId       = VOS_LOCAL_CPUID;\
            (pMsg)->ulSenderPid         = PS_PID_MM;\
            (pMsg)->ulReceiverCpuId     = VOS_LOCAL_CPUID;\
            (pMsg)->ulReceiverPid       = PS_PID_OM;\
            (pMsg)->ulLength            = (MsgLenthNoHeader);\
        }

#define EMM_COMP_APP_MSG_VOS_HEADER(pDestMsg,MsgLenthNoHeader,pSourMsg)\
        {\
            (pDestMsg)->ulSenderCpuId   = VOS_LOCAL_CPUID;\
            (pDestMsg)->ulSenderPid     = PS_PID_MM;\
            (pDestMsg)->ulReceiverCpuId = VOS_LOCAL_CPUID;\
            (pDestMsg)->ulReceiverPid   = PS_PID_APP;\
            (pDestMsg)->ulLength        = (MsgLenthNoHeader);\
            (pDestMsg)->usOriginalId    = (pSourMsg)->usTerminalId;\
            (pDestMsg)->usTerminalId    = (pSourMsg)->usOriginalId;\
            (pDestMsg)->ulTimeStamp     = (pSourMsg)->ulTimeStamp;\
            (pDestMsg)->ulSN            = (pSourMsg)->ulSN;\
        }

#define EMM_COMP_APP_DT_IND_MSG_HEADER(pMsg,MsgLenthNoHeader)\
        {\
            (pMsg)->ulSenderCpuId       = VOS_LOCAL_CPUID;\
            (pMsg)->ulSenderPid         = PS_PID_MM;\
            (pMsg)->ulReceiverCpuId     = VOS_LOCAL_CPUID;\
            (pMsg)->ulReceiverPid       = L3_MSP_PID_DT_IND;\
            (pMsg)->ulLength            = (MsgLenthNoHeader);\
        }

#define EMM_COMP_APP_DT_CNF_MSG_VOS_HEADER(pDestMsg,MsgLenthNoHeader,pSourMsg)\
        {\
            (pDestMsg)->ulSenderCpuId   = VOS_LOCAL_CPUID;\
            (pDestMsg)->ulSenderPid     = PS_PID_MM;\
            (pDestMsg)->ulReceiverCpuId = VOS_LOCAL_CPUID;\
            (pDestMsg)->ulReceiverPid   = L3_MSP_PID_DT;\
            (pDestMsg)->ulLength        = (MsgLenthNoHeader);\
            (pDestMsg)->usOriginalId    = (pSourMsg)->usTerminalId;\
            (pDestMsg)->usTerminalId    = (pSourMsg)->usOriginalId;\
            (pDestMsg)->ulTimeStamp     = (pSourMsg)->ulTimeStamp;\
            (pDestMsg)->ulSN            = (pSourMsg)->ulSN;\
        }

/*�տ���Ϣ�͹ؼ��¼����APP��Ϣͷ*/
#define EMM_COMP_OMT_APP_MSG_HEADER(pMsg)\
        {\
            (pMsg)->usOriginalId  = (UE_MODULE_MM_ID);\
            (pMsg)->usTerminalId  = (UE_APP_SUBSYS_ID);\
        }

#define EMM_COMP_MM_APP_KEY_AIR_MSG_HEADER(pMsg)\
        {\
            (pMsg)->usOriginalId        = (UE_MODULE_MM_ID);\
            (pMsg)->usTerminalId        = (UE_APP_SUBSYS_ID);\
        }

#if 0
/*��ͨ�����͸���������APP��Ϣͷ��ulTimeStamp�Ļ�ȡ������Ҫ����*/
#define EMM_COMP_OMT_TP_APP_MSG_HEADER(pDestMsg, pSourMsg)\
        {\
            (pDestMsg)->usOriginalId    = (pSourMsg)->usTerminalId;\
            (pDestMsg)->usTerminalId    = (pSourMsg)->usOriginalId;\
            (pDestMsg)->ulTimeStamp     = (pSourMsg)->ulTimeStamp;\
            (pDestMsg)->ulSN            = (pSourMsg)->ulSN;\
        }

#define EMM_COMP_OMT_SET_CNF_MSG(pEmmOmSetCnfMsg,ID_EMM_OM_OT_CNF,OM_EMM_MAINTAIN_RST)\
        {\
            (pEmmOmSetCnfMsg)->ulMsgId  = (ID_EMM_OM_OT_CNF);\
            (pEmmOmSetCnfMsg)->ucRst    = (OM_EMM_MAINTAIN_RST);\
        }

#define EMM_COMP_MM_APP_MSG_HEADER(pDestMsg, pSourMsg)\
        {\
            (pDestMsg)->usOriginalId    = (pSourMsg)->usTerminalId;\
            (pDestMsg)->usTerminalId    = (pSourMsg)->usOriginalId;\
            (pDestMsg)->ulTimeStamp     = (pSourMsg)->ulTimeStamp;\
            (pDestMsg)->ulSN            = (pSourMsg)->ulSN;\
        }

#define EMM_COMP_APP_SET_CNF_MSG(pEmmOmSetCnfMsg,pSourMsg,OM_EMM_MAINTAIN_RST)\
        {\
            (pEmmOmSetCnfMsg)->ulMsgId  = (pSourMsg)->ulMsgId;\
            (pEmmOmSetCnfMsg)->ulRslt    = (OM_EMM_MAINTAIN_RST);\
            (pEmmOmSetCnfMsg)->ulOpId   = (pSourMsg)->ulOpId;\
        }
#endif

#define NAS_EMM_COMP_MM_2_APP_MSGID(pstEmmAppMsg, ID_EMM_APP_MSGID) \
            (pstEmmAppMsg->ulMsgId      = (ID_EMM_APP_MSGID))

#define EMM_TP_CMD_TYPE(enTpMsgType)    (enTpMsgType)


#define EMM_MSG_LENTH_NO_HEADER(EMM_XX_MSG_STRU)\
            (sizeof(EMM_XX_MSG_STRU)    - NAS_EMM_LEN_VOS_MSG_HEADER)


#define NAS_LMM_MEM_CMP(dest, src, count) PS_MEM_CMP((dest), (src), (count))


/*OMAͷ*/
#define OMA_HEADER        VOS_UINT16    usHeader;\
                          VOS_UINT16    usMsgLen;

#ifndef APP_MSG_HEADER

#define APP_MSG_HEADER    VOS_UINT16    usOriginalId;\
                          VOS_UINT16    usTerminalId;\
                          VOS_UINT32    ulTimeStamp; \
                          VOS_UINT32    ulSN;


#endif

/* ����TP_GINFO_CNF_MSG��DOPRA�������Ϣ�Ĳ��ֳ��ȣ�
�ó��Ȳ�����u �о����ȫ�ֱ����ĳ��� */
#define NAS_EMM_TP_GINFO_MSG_LENGTH_EXCEPT_IE(pstTpGInfoCnfMsg) \
                ((sizeof(APP_MM_TP_G_INFO_CNF_STRU)\
                 - NAS_EMM_LEN_VOS_MSG_HEADER)\
                 - (sizeof(pstTpGInfoCnfMsg->u)))



/* xiongxianghui00253310 modify for ftmerrlog begin */
#define NAS_EMM_GetFtmInfoManageAddr()      (&(g_astEmmFtmInfoManage))
#define NAS_EMM_GetEmmInfoAddr()            (&(g_astEmmInfo))
#define NAS_EMM_GetFtmInfoActionFlag()      (NAS_EMM_GetFtmInfoManageAddr()->ulFtmActionFlag)
#define NAS_EMM_GetFtmInfoMsgSN()           (NAS_EMM_GetFtmInfoManageAddr()->ulMsgSN)
#define NAS_EMM_GetFtmInfoCnRej()           (NAS_EMM_GetFtmInfoManageAddr()->enEmmCnRejCause)
#define NAS_EMM_SetFtmOmManageFtmActionFlag(Flag)  (NAS_EMM_GetFtmInfoActionFlag() = Flag)

#define NAS_EMM_GetErrlogManageAddr()       (&(g_astEmmErrlogInfoManage))
#define NAS_EMM_GetErrlogActionFlag()       (NAS_EMM_GetErrlogManageAddr()->ulActionFlag)
#define NAS_EMM_SetErrlogActionFlag(Flag)   (NAS_EMM_GetErrlogActionFlag() = Flag)
#define NAS_EMM_GetErrlogMsgSN()            (NAS_EMM_GetErrlogManageAddr()->ulMsgSN)
#define NAS_EMM_GetErrlogAlmLevel()         (NAS_EMM_GetErrlogManageAddr()->usALMLevel)
#define NAS_EMM_SetErrlogAlmLevel(usLevel)  (NAS_EMM_GetErrlogAlmLevel() = usLevel)
#define NAS_EMM_GetErrlogAlmType()          (NAS_EMM_GetErrlogManageAddr()->usALMType)
#define NAS_EMM_GetErrlogAlmLowSlice()      (NAS_EMM_GetErrlogManageAddr()->ulAlmLowSlice)
#define NAS_EMM_GetErrlogAlmHighSlice()     (NAS_EMM_GetErrlogManageAddr()->ulAlmHighSlice)
#define NAS_EMM_GetErrlogAmount()           (NAS_EMM_GetErrlogManageAddr()->ulErrLogAmount)
#define NAS_EMM_GetErrlogNextNullPos()      (NAS_EMM_GetErrlogManageAddr()->ulNextNullPos)
#define NAS_EMM_GetErrlogInfo(ulIndex)      (NAS_EMM_GetErrlogManageAddr()->stLmmErrInfoDetail[ulIndex])
#define NAS_EMM_GetErrlogMsgQueueAddr()     (&(NAS_EMM_GetErrlogManageAddr()->stMsgQueueInfo))

#define NAS_EMM_ERRLOG_MAX_NUM              (4)

#define NAS_LMM_COMP_OM_MSG_HEADER(pMsg,MsgLenthNoHeader)\
        {\
            (pMsg)->ulSenderCpuId       = VOS_LOCAL_CPUID;\
            (pMsg)->ulSenderPid         = PS_PID_MM;\
            (pMsg)->ulReceiverCpuId     = VOS_LOCAL_CPUID;\
            (pMsg)->ulReceiverPid       = ACPU_PID_OM;\
            (pMsg)->ulLength            = (MsgLenthNoHeader);\
        }


/* xiongxianghui00253310 modify for ftmerrlog end   */


/*****************************************************************************
  3 Massage Declare
*****************************************************************************/



/*****************************************************************************
  4 Enum
*****************************************************************************/

enum    NAS_EMM_FTM_ACTION_FLAG_ENUM
{
    NAS_EMM_FTM_ACTION_FLAG_CLOSE = 0,
    NAS_EMM_FTM_ACTION_FLAG_OPEN = 1,
    NAS_EMM_FTM_ACTION_FLAG_BUTT
};
typedef VOS_UINT32  NAS_EMM_FTM_ACTION_FLAG_ENUM_UINT32;

enum NAS_EMM_ERRLOG_ACTION_FLAG_ENUM
{
    NAS_EMM_ERRLOG_ACTION_FLAG_CLOSE  = 0,
    NAS_EMM_ERRLOG_ACTION_FLAG_OPEN   = 1,
    NAS_EMM_ERRLOG_ACTION_FLAG_BUTT
};
typedef VOS_UINT32  NAS_EMM_ERRLOG_ACTION_FLAG_ENUM_UINT32;

enum NAS_EMM_ERRLOG_LEVEL_ENUM
{
    NAS_EMM_ERRLOG_LEVEL_CRITICAL   = 1,/*�������*/
    NAS_EMM_ERRLOG_LEVEL_MAJOR      = 2,/*������Ҫ*/
    NAS_EMM_ERRLOG_LEVEL_MINOR      = 3,/*�����Ҫ*/
    NAS_EMM_ERRLOG_LEVEL_WARING     = 4,/*������ʾ��*/
    NAS_EMM_ERRLOG_LEVEL_BUTT
};
typedef VOS_UINT16  NAS_EMM_ERRLOG_LEVEL_ENUM_UINT16;

/* ERROR LOG����&��������*/
enum NAS_EMM_ERRLOG_TYPE_ENUM
{
    NAS_EMM_ERRLOG_TYPE_COMMUNICATION       = 0x00, /* ͨ��     */
    NAS_EMM_ERRLOG_TYPE_SERVING_QUALITY     = 0x01, /* ҵ������ */
    NAS_EMM_ERRLOG_TYPE_PROCESS_ERROR       = 0x02, /* ������� */
    NAS_EMM_ERRLOG_TYPE_EQUIPMENT_TROUBLE   = 0x03, /* �豸���� */
    NAS_EMM_ERRLOG_TYPE_ENVIRONMENT_TROUBLE = 0x04, /* �������� */
};
typedef VOS_UINT16 NAS_EMM_ERRLOG_TYPE_ENUM_UINT16;


enum    NAS_EMM_OMT_AIR_MSG_UP_DOWN_ENUM
{
    NAS_EMM_OMT_AIR_MSG_UP              = 0x00,             /* UPWARD */
    NAS_EMM_OMT_AIR_MSG_DOWN            = 0x01,             /* DOWNWARD */

    NAS_EMM_OMT_AIR_MSG_BUTT
};
typedef VOS_UINT8   NAS_LMM_OM_AIR_MSG_UP_DOWN_ENUM_UINT8;

enum    NAS_EMM_OMT_AIR_MSG_ID_ENUM
{
    NAS_EMM_ATTACH_REQ                  = MM_ATTACH_REQ,
    NAS_EMM_ATTACH_ACP                  = MM_ATTACH_ACP,
    NAS_EMM_ATTACH_CMP                  = MM_ATTACH_CMP,
    NAS_EMM_ATTACH_REJ                  = MM_ATTACH_REJ,

    NAS_EMM_DETACH_REQ_MT               = MM_DETACH_REQ_MT,
    NAS_EMM_DETACH_ACP_MT               = MM_DETACH_ACP_MT,
    NAS_EMM_DETACH_REQ_MO               = MM_DETACH_REQ_MO,
    NAS_EMM_DETACH_ACP_MO               = MM_DETACH_ACP_MO,

    NAS_EMM_TAU_REQ                     = MM_TAU_REQ,
    NAS_EMM_TAU_ACP                     = MM_TAU_ACP,
    NAS_EMM_TAU_CMP                     = MM_TAU_CMP,
    NAS_EMM_TAU_REJ                     = MM_TAU_REJ,

    NAS_EMM_SER_REQ                     = MM_SER_REQ,
    NAS_EMM_SER_REJ                     = MM_SER_REJ,
    NAS_EMM_SER_ACP                     = MM_SER_ACP,

    NAS_EMM_GUTI_CMD                    = MM_GUTI_CMD,
    NAS_EMM_GUTI_CMP                    = MM_GUTI_CMP,

    NAS_EMM_IDEN_REQ                    = MM_IDEN_REQ,
    NAS_EMM_IDEN_RSP                    = MM_IDEN_RSP,

    NAS_EMM_AUTH_REQ                    = MM_AUTH_REQ,
    NAS_EMM_AUTH_FAIL                   = MM_AUTH_FAIL,
    NAS_EMM_AUTH_RSP                    = MM_AUTH_RSP,
    NAS_EMM_AUTH_REJ                    = MM_AUTH_REJ,

    NAS_EMM_SECU_CMD                    = MM_SECU_CMD,
    NAS_EMM_SECU_CMP                    = MM_SECU_CMP,
    NAS_EMM_SECU_REJ                    = MM_SECU_REJ,

    NAS_EMM_STATUS                      = MM_EMM_STATUS,
    NAS_EMM_INFO                        = MM_EMM_INFO,

    NAS_EMM_DOWNLINK_NAS_TRANSPORT      = MM_EMM_DOWNLINK_NAS_TRANSPORT,
    NAS_EMM_UPLINK_NAS_TRANSPORT        = MM_EMM_UPLINK_NAS_TRANSPORT,

    NAS_EMM_EXTENDED_SER_REQ            = MM_EXTENDED_SER_REQ,                  /* 2012-02-24 */
    NAS_EMM_CS_SER_NOTIFICATION         = MM_CS_SER_NOTIFICATION,               /* 2012-02-24 */

    NAS_EMM_DOWNLINK_GENERIC_NAS_TRANSPORT = MM_EMM_DOWNLINK_GENERIC_NAS_TRANSPORT,
    NAS_EMM_UPLINK_GENERIC_NAS_TRANSPORT   = MM_EMM_UPLINK_GENERIC_NAS_TRANSPORT,
};
typedef VOS_UINT8   NAS_LMM_OM_AIR_MSG_ID_ENUM_UINT8;


enum    PS_EMM_OMT_KEY_EVENT_TYPE_ENUM
{   /* EMM�ؼ��¼�ȡֵ��Χ100 - 199 */

    EMM_OMT_KE_PLMN_SPEC_SEARCH_START   = MM_KEY_EVENT_PLMN_SPEC_SEARCH_START,
    EMM_OMT_KE_PLMN_LIST_SEARCH_START   = MM_KEY_EVENT_PLMN_LIST_SEARCH_START,
    EMM_OMT_KE_PLMN_SEARCH_SUCC         = MM_KEY_EVENT_PLMN_SEARCH_SUCC,
    EMM_OMT_KE_PLMN_SEARCH_FAIL         = MM_KEY_EVENT_PLMN_SEARCH_SUCC,
    EMM_OMT_KE_START_AUTO_RESEL         = MM_KEY_EVENT_START_AUTO_RESEL,
    EMM_OMT_KE_START_MANUAL_RESEL       = MM_KEY_EVENT_START_MANUAL_RESEL,
    EMM_OMT_KE_PLMN_RESEL_CNF           = MM_KEY_EVENT_PLMN_RESEL_CNF,
    EMM_OMT_KE_COVERAGE_LOST            = MM_KEY_EVENT_COVERAGE_LOST,

    EMM_OMT_KE_ATTACH_REQ               = MM_KEY_EVENT_ATTACH_REQ,
    EMM_OMT_KE_ATTACH_ACP               = MM_KEY_EVENT_ATTACH_ACP,
    EMM_OMT_KE_ATTACH_CMP               = MM_KEY_EVENT_ATTACH_CMP,
    EMM_OMT_KE_ATTACH_REJ               = MM_KEY_EVENT_ATTACH_REJ,

    EMM_OMT_KE_DETACH_REQ_MT            = MM_KEY_EVENT_ATTACH_REJ,
    EMM_OMT_KE_DETACH_ACP_MT            = MM_KEY_EVENT_DETACH_REQ_MT,
    EMM_OMT_KE_DETACH_REQ_MO            = MM_KEY_EVENT_DETACH_REQ_MT,
    EMM_OMT_KE_DETACH_ACP_MO            = MM_KEY_EVENT_DETACH_ACP_MT,

    EMM_OMT_KE_TAU_REQ                  = MM_KEY_EVENT_TAU_REQ,
    EMM_OMT_KE_TAU_ACP                  = MM_KEY_EVENT_TAU_ACP,
    EMM_OMT_KE_TAU_CMP                  = MM_KEY_EVENT_TAU_CMP,
    EMM_OMT_KE_TAU_REJ                  = MM_KEY_EVENT_TAU_REJ,

    EMM_OMT_KE_SER_REQ                  = MM_KEY_EVENT_SER_REQ,
    EMM_OMT_KE_SER_REJ                  = MM_KEY_EVENT_SER_REJ,
    EMM_OMT_KE_SER_ACP                  = MM_KEY_EVENT_SER_ACP,

    EMM_OMT_KE_GUTI_CMD                 = MM_KEY_EVENT_GUTI_CMD,
    EMM_OMT_KE_GUTI_CMP                 = MM_KEY_EVENT_GUTI_CMP,

    EMM_OMT_KE_IDEN_REQ                 = MM_KEY_EVENT_IDEN_REQ,
    EMM_OMT_KE_IDEN_RSP                 = MM_KEY_EVENT_IDEN_RSP,

    EMM_OMT_KE_AUTH_REQ                 = MM_KEY_EVENT_AUTH_REQ,
    EMM_OMT_KE_AUTH_FAIL                = MM_KEY_EVENT_AUTH_FAIL,
    EMM_OMT_KE_AUTH_RSP                 = MM_KEY_EVENT_AUTH_RSP,
    EMM_OMT_KE_AUTH_REJ                 = MM_KEY_EVENT_AUTH_REJ,

    EMM_OMT_KE_SECU_CMD                 = MM_KEY_EVENT_SECU_CMD,
    EMM_OMT_KE_SECU_CMP                 = MM_KEY_EVENT_SECU_CMP,
    EMM_OMT_KE_SECU_REJ                 = MM_KEY_EVENT_SECU_REJ,

    EMM_OMT_KE_EMM_STATUS               = MM_KEY_EVENT_EMM_STATUS,
    EMM_OMT_KE_EMM_INFO                 = MM_KEY_EVENT_EMM_INFO,

    EMM_OMT_KE_EMM_DOWNLINK_NAS_TRANSPORT  = MM_KEY_EVENT_EMM_DOWNLINK_NAS_TRANSPORT,
    EMM_OMT_KE_EMM_UPLINK_NAS_TRANSPORT    = MM_KEY_EVENT_EMM_UPLINK_NAS_TRANSPORT,        /*2011-11-15*/

    EMM_OMT_KE_EXTENDED_SER_REQ         = MM_KEY_EVENT_EXTENDED_SER_REQ,        /*2012-02-24*/
    EMM_OMT_KE_CS_SER_NOTIFICATION      = MM_KEY_EVENT_CS_SER_NOTIFICATION,     /*2012-02-24*/

    EMM_OMT_KE_EMM_DOWNLINK_GENERIC_NAS_TRANSPORT = MM_KEY_EVENT_EMM_DOWNLINK_GENERIC_NAS_TRANSPORT,
    EMM_OMT_KE_EMM_UPLINK_GENERIC_NAS_TRANSPORT   = MM_KEY_EVENT_EMM_UPLINK_GENERIC_NAS_TRANSPORT,

    EMM_OMT_KE_TYPE_BUTT
};
typedef VOS_UINT8   PS_EMM_OMT_KEY_EVENT_TYPE_ENUM_UINT8;

/* OM_EMM_MSG_ID_ENUM_UINT32����4����:

-----------------------------------------------------------
  �ֽڸ�λ                                      �ֽڵ�λ
-----------------------------------------------------------
| BYTE1��4λ + 0000 |  BYTE2    |   BYTE3    |    BYTE4   |
-----------------------------------------------------------
0000��Э��ģ�����Ϣ|  Դģ��� |  Ŀ��ģ���| ��Ϣ��0-255
-----------------------------------------------------------
0001��L2 LOG        |           |            |
-----------------------------------------------------------
0010���տ���Ϣ      |           |            |
-----------------------------------------------------------
0011����ͨ����      |           |            | OM_EMM_ORDER_TYPE_ENUM_UINT32
-----------------------------------------------------------
0100��ʵʱ�������  |           |            | OM_EMM_ORDER_TYPE_ENUM_UINT32
-----------------------------------------------------------
0101���ؼ��¼�      |           |            | ��������
-----------------------------------------------------------
*/
/*
typedef VOS_UINT32  OMT_EMM_MSG_ID_UINT32;
*/


/* OM��EMM���ά��������ִ�н������ */
enum    APP_EMM_MAINTAIN_RST_ENUM
{
    APP_EMM_MAINTAIN_RST_SUCC       = 0x00,
    APP_EMM_MAINTAIN_RST_FAIL,

    APP_EMM_MAINTAIN_RST_BUTT
};
typedef VOS_UINT32  APP_EMM_MAINTAIN_RST_ENUM_UINT32;



/*****************************************************************************
 ö����    : NAS_EMM_TP_MSG_TYPE_ENUM
 ö��˵��  : OM��EMM���͸������������
*****************************************************************************/

enum    NAS_EMM_TP_MSG_TYPE_ENUM
{
    NAS_EMM_TP_INQ_G_INFO_REQ           = 0x01,

    NAS_EMM_TP_MSG_TYPE_BUTT
};
typedef VOS_UINT32  NAS_EMM_TP_MSG_TYPE_ENUM_UINT32;

enum    APP_EMM_TP_CAUSE_ENUM
{
    APP_EMM_TP_CAUSE_TP_MSG_TYPE_UNKNOWN= 0x01,
    APP_EMM_TP_CAUSE_PARA_RANGE_ERR     = 0x02,
    APP_EMM_TP_CAUSE_BUTT               = 0xFF
};
typedef VOS_UINT32  APP_EMM_TP_CAUSE_ENUM_UINT32;


/*****************************************************************************
 ö����    : EMM_OM_G_INFO_TYPE_ENUM
 ö��˵��  : ��ѯ����Ϣ����
*****************************************************************************/
enum    EMM_OM_G_INFO_TYPE_ENUM
{
    /*״̬��*/
    EMM_OM_G_INFO_EMMUPSTATE           = 0x00000000,   /* */


    /*������Ϣ��*/
    EMM_OM_G_INFO_EPlMNLIST,
    EMM_OM_G_INFO_TAILIST,


    /*��ʱ��*/
    EMM_OM_G_INFO_T3402,
    EMM_OM_G_INFO_T3412,
    EMM_OM_G_INFO_TAU_T3411,

    /*TAU*/
    EMM_TAU_ATTEMPT_COUNTER,
    EMM_TAU_START_CAUSE,

    /*SER*/
    EMM_SER_START_CAUSE,

    /* DHCPV4 SERVER */
    EMM_DHCPV4_SERVER_DEFT_EPSBID,
    EMM_DHCPV4_SERVER_TMP_STATUS,
    EMM_DHCPV4_SERVER_ALLOC_STATUS,
    EMM_DHCPV4_SERVER_NW_PARA,
    EMM_DHCPV4_SERVER_BUFFER,

    /* IPF */
    EMM_IPF_UL_FILTER_NUM,
    /*�����*/
    /*��ȫ��*/
    /*����*/
    /*��ģ����Ʋ���: ATTACH, DETACH, TAU, SERIVCE, MMC*/
    /*leili modify for isr begin*/
    /*MML��TINֵ��Ϣ*/
    EMM_TIN_TYPE,

    /*MML�ĳ�����Ϣ*/
    EMM_PS_BEARER_INFO,
    /*leili modify for isr end*/
	/*L.V.R TAI*/
    EMM_LVR_TAI = 17,

    /*Reg Domain*/
    EMM_REG_DOMAIN = 18,
    EMM_OM_G_INFO_BUTT
};
typedef VOS_UINT32  EMM_OM_G_INFO_TYPE_ENUM_UINT32;

enum    NAS_EMM_DRX_CYCLE_LEN_CHANGE_ENUM
{
    NAS_EMM_DRX_CYCLE_LEN_CHANGED       = 0,
    NAS_EMM_DRX_CYCLE_LEN_NOT_CHANGED   = 1,
    NAS_EMM_DRX_CYCLE_LEN_CHANGE_BUTT
};
typedef VOS_UINT32   NAS_EMM_DRX_CYCLE_LEN_CHANGE_ENUM_UINT32;
/*leili modify for isr begin*/
enum NAS_LMM_TIN_TYPE_ENUM
{
    NAS_LMM_TIN_P_TMSI                  = 0x00000000,
    NAS_LMM_TIN_GUTI                    ,
    NAS_LMM_TIN_RAT_RELATED_TMSI        ,
    NAS_LMM_TIN_INVALID                 ,

    NAS_LMM_TIN_BUTT
};
typedef VOS_UINT32 NAS_LMM_TIN_TYPE_ENUM_UINT32;

/*leili modify for isr end*/
/*PC REPLAY MODIFY BY LEILI DELETE*/
/*****************************************************************************
  5 STRUCT
*****************************************************************************/
typedef struct
{
    NAS_LMM_MAIN_STATE_ENUM_UINT16       enMainState;
    NAS_LMM_SUB_STATE_ENUM_UINT16        enSubState;
    APP_EMM_MAIN_STATE_ENUM_UINT16      ulProtoclMainState;
    APP_EMM_SUB_STATE_ENUM_UINT16       ulProtoclSubState;
}NAS_EMM_PROTOCOL_STATE_MAP_STRU;


typedef struct
{
    VOS_MSG_HEADER                                  /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;    /*_H2ASN_Skip*/
}NAS_EMM_GENERAL_MSG_HEADER_STRU;




/*****************************************************************************
 �ṹ��    : APP_MM_TP_G_INFO_REQ_STRU
 �ṹ˵��  : MM��͸�������ͨ������ṹ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                  /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;    /*_H2ASN_Skip*/
    APP_MSG_HEADER

    NAS_EMM_TP_MSG_TYPE_ENUM_UINT32     ulMmTpCmdType;
    VOS_UINT8                           aucTransCmd[4];
}APP_MM_TP_CMD_REQ_STRU;
/*ID_APP_MM_TP_CMD_REQ*/


/*****************************************************************************
 �ṹ��    : APP_MM_TP_CMD_CNF_STRU
 �ṹ˵��  : MM��͸�������ͨ����Ӧ�ṹ�����ǲ�ѯG_INFO
                  �ĳɹ���CNF��Ϣ����APP_MM_TP_G_INFO_CNF_STRU
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                  /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;    /*_H2ASN_Skip*/
    APP_MSG_HEADER

    APP_EMM_MAINTAIN_RST_ENUM_UINT32    ulRslt;
    APP_EMM_TP_CAUSE_ENUM_UINT32        ulCause;
}APP_MM_TP_CMD_CNF_STRU;




/*****************************************************************************
 �ṹ����  : APP_MM_TP_G_INFO_REQ_STRU
 �ṹ˵��  : OM��ѯMM��ȫ�ֲ���ʱ����MM������ṹ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER


    NAS_EMM_TP_MSG_TYPE_ENUM_UINT32     enMmTpCmdType;      /*��ѯG_INFO����*/
    EMM_OM_G_INFO_TYPE_ENUM_UINT32      enGInfoType;        /*����ѯ����������*/
}APP_MM_TP_G_INFO_REQ_STRU;


/*****************************************************************************
 �ṹ��    : APP_MM_TP_G_INFO_CNF_STRU
 �ṹ˵��  : ��ѯMM��ȫ�ֲ���ʱ����OM��Ӧ�Ľṹ
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER

    APP_EMM_MAINTAIN_RST_ENUM_UINT32    ulRslt ;            /* ��ѯ�Ƿ�ɹ� */
    EMM_OM_G_INFO_TYPE_ENUM_UINT32      enGInfoType;
    union
    {
        NAS_EMM_UPDATE_STATE_ENUM_UINT8 ucEmmUpStat;
        NAS_MM_PLMN_LIST_STRU           stEPlmnList;
        NAS_MM_TA_LIST_STRU             stTaiList;
        NAS_LMM_TIMER_CTRL_STRU          stTimer;
        VOS_UINT32                      ulTauAttemptCounter;
        NAS_EMM_TAU_START_CAUSE_ENUM_UINT8 enTauStartCause;
        NAS_EMM_SER_START_CAUSE_ENUM_UINT8 enSerStartCause;
        VOS_UINT8                       ucDhcpv4ServerDeftEpsbId;
        VOS_UINT8                       ucDhcpv4SeverTmpStatus;
        VOS_UINT8                       ucDhcpv4SeverAllocStatus;
        /*IP_DHCPV4SERVER_NWPARAM_STRU    stDhcpv4ServerNWParam;*/
        VOS_UINT8                       aucDhcpv4ServerBuffer[NAS_EMM_DHCP_SERVER_BUFFER_LEN];
        VOS_UINT32                      ulIpfUlFilterNum;
		/*leili modify for isr begin*/
        NAS_LMM_TIN_TYPE_ENUM_UINT32    ulTinType;
        NAS_MML_PS_BEARER_CONTEXT_STRU  astPsBearerCtx[EMM_ESM_MAX_EPS_BEARER_NUM];
        /*leili modify for isr end*/
		NAS_MM_NETWORK_ID_STRU          stLvrTai;
        NAS_LMM_REG_DOMAIN_ENUM_UINT32  enRegDomain;
	}u;

}APP_MM_TP_G_INFO_CNF_STRU;



/*ԭ��д��interface.c*/
typedef struct
{
    VOS_UINT32                          ulNasMsgSize;       /* [1,RRC_NAS_MAX_SIZE_NAS_MSG],Byte */
    VOS_UINT8                           aucNasMsg[4];       /* ��Ϣ��ǰ�ĸ��ֽ�����              */
    /******************************************************************************************************
        _H2ASN_Array2String
    ******************************************************************************************************/
}NAS_MSG_STRU;


/* OM���������������Ͷ��� */
typedef VOS_VOID    ( * NAS_LMM_OM_ACTION_FUN )( MsgBlock  *  /* ��Ϣָ�� */);


/* OM������ṹ */
typedef struct
{
    APP_MM_MSG_TYPE_ENUM_UINT32         ulMsgId;     /* ��ϢID */
    NAS_LMM_OM_ACTION_FUN                pfFun;       /* ���� �� ��ѯ���� */
}NAS_LMM_OM_ACT_STRU;


#if 0
typedef NAS_LMM_COMM_ACTION_FUN          NAS_LMM_OM_ACTION_FUN;
typedef NAS_LMM_COMM_ACT_STRU            NAS_LMM_OM_ACT_STRU;
#endif

typedef NAS_LMM_COMM_ACTION_FUN          NAS_LMM_PRE_PROC_ACTION_FUN;
typedef NAS_LMM_COMM_ACT_STRU            NAS_LMM_PRE_PROC_ACT_STRU;

typedef struct
{
    VOS_UINT8                                   ucRsrv;
    NAS_EMM_CN_MSG_TYPE_ENUM_UINT8              enCnMsgType;
    NAS_LMM_OM_AIR_MSG_ID_ENUM_UINT8             enAirMsg;
    PS_EMM_OMT_KEY_EVENT_TYPE_ENUM_UINT8        enKeyEvent;

}NAS_LMM_OM_MSG_ID_CHANGE_STRU;

typedef struct
{
    APP_MM_MSG_TYPE_ENUM_UINT32         ulMsgId;
    VOS_UINT32                          ulOpId;
    VOS_UINT16                          usOriginalId;
    VOS_UINT16                          usTerminalId;
    VOS_UINT32                          ulTimeStamp;
    VOS_UINT32                          ulSN;
    VOS_UINT32                          ulRslt;

}NAS_LMM_APP_PARA_SET_RESLT_STRU;

typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    VOS_UINT32                          ulMsgId;            /*_H2ASN_Skip*/
    APP_MSG_HEADER
    VOS_UINT32                          ulOpId;             /*���β�����ʶ*/
    APP_MM_SET_TYPE_ENUM_UINT32         enSetType;          /*��������:0:����,1:ɾ��*/
}NAS_LMM_APP_PARA_SET_HEAD_STRU;

/* xiongxianghui00253310 modify for ftmerrlog begin */

typedef struct
{
    APP_EMM_MAIN_STATE_ENUM_UINT16       enMainState;
    APP_EMM_SUB_STATE_ENUM_UINT16        enSubState;

    NAS_EMM_PLMN_ID_STRU                 stPlmnId;
    NAS_EMM_GUTI_STRU                    stGuti;
    /* ��λms���ο����� NAS_LMM_ModifyPtlTimer()�Ĵ��� */
    VOS_UINT32                           ulTimerLen3412;
    NAS_EMM_TAC_STRU                      stTac;

    NAS_EMM_CN_CAUSE_ENUM_UINT8          enEmmCnRejCause;
    VOS_UINT8                            aucRsv[3];

}EMM_DATABASE_INFO_STRU;

typedef struct
{
    /* �򿪻��߹رչ���ģʽ�ϱ����� 0:close 1:open*/
    VOS_UINT32                          ulFtmActionFlag;
    /* EMM��OM���͵�ÿ������ģʽ��Ϣ���д˱�ţ��ϵ�Ϊ0��Ȼ�����μ� 1 */
    VOS_UINT32                          ulMsgSN;
    /* EMM����ģʽ����ṹ��ʱ�洢CnCause�������뱾�����ݿ���бȶ� */
    NAS_EMM_CN_CAUSE_ENUM_UINT8         enEmmCnRejCause;
    VOS_UINT8                           aucRsv[3];

}EMM_FTM_INFO_MANAGE_STRU;

typedef struct
{
    NAS_MM_CN_CAUSE_ENUM_UINT8            ulCauseId;    /*cause ID*/
    LNAS_OM_ERRLOG_ID_ENUM_UINT16         ulErrorlogID; /*error id*/
}NAS_LMM_CN_CAUSE_TRANS_STRU;

/*****************************************************************************
 �ṹ��    : NAS_LMM_RECIVE_MSG_STRU
 �ṹ˵��  : LMM �����Ϣ����Ϣ
*****************************************************************************/
typedef struct
{
    VOS_UINT32                   ulTimeStamp;  /* ʱ��� */
    VOS_UINT32                   ulSendPid;    /* ��������Pid */
    VOS_UINT32                   ulMsgName;    /* ������ϢID */
}NAS_LMM_RECIVE_MSG_STRU;


/*****************************************************************************
 �ṹ��    : NAS_LMM_SAVE_RECIVE_MSG_INFO_STRU
 Э����  :
 ASN.1���� :
 �ṹ˵��  : ��������ʱ��ģ���յ���������Ϣ
  ulNextIndex������һ���յ���Ϣ����������ulNextIndex = 1����astReciveMsgInfo[0]
 �����µ��յ�����ϢID
*****************************************************************************/
typedef struct
{
    VOS_UINT32                         ulNextIndex;
    NAS_LMM_RECIVE_MSG_STRU            astReciveMsgInfo[NAS_SAVE_RECEIVE_MSG_INFO_NUM];
} NAS_LMM_MSG_SAVE_INFO_STRU;

typedef struct
{
    VOS_UINT32                             ulActionFlag;
    VOS_UINT32                             ulMsgSN;
     /* ERR LOG�ϱ�����,ÿ��ģ���ÿ�������Ӧһ��ERR LOG��������
    ����&�澯����
    Warning�� 0x04������ʾ��
    Minor��   0x03�����Ҫ
    Major��   0x02�����Ҫ
    Critical��0x01�������    */
    NAS_EMM_ERRLOG_LEVEL_ENUM_UINT16       usALMLevel;
    NAS_EMM_ERRLOG_TYPE_ENUM_UINT16        usALMType;
    /* EMM����ERR LOG������ʱ��� */
    VOS_UINT32                             ulAlmLowSlice;/*ʱ���*/
    VOS_UINT32                             ulAlmHighSlice;

    VOS_UINT32                             ulErrLogAmount;
    VOS_UINT32                             ulNextNullPos;
    LMM_ERR_INFO_DETAIL_STRU               stLmmErrInfoDetail[NAS_EMM_ERRLOG_MAX_NUM];
    NAS_LMM_MSG_SAVE_INFO_STRU             stMsgQueueInfo;
}EMM_ERRLOG_INFO_MANAGE_STRU;

/* xiongxianghui00253310 modify for ftmerrlog end  */

/*MMģ�����ϱ�·�����Ϣ�������ϱ�������Ҳ���ڴ����*/
typedef struct
{
    NAS_MM_GUTI_STRU                         stGuti;           /*GUTI��Ϣ */
    NAS_OM_IMSI_INFO_STRU                    stImsi;           /*IMSI��Ϣ */
    NAS_OM_EMM_STATE_STRU                    stEmmState;        /*EMM״̬��Ϣ */
}APP_MM_DT_STRU;

/*****************************************************************************
  6 UNION
*****************************************************************************/


/*****************************************************************************
  7 Extern Global Variable
*****************************************************************************/
extern NAS_EMM_DRX_CYCLE_LEN_CHANGE_ENUM_UINT32    g_ulDrxCycleLenChange;
extern NAS_LMM_OM_MSG_ID_CHANGE_STRU g_stMmOmMsgIdStru[];
extern VOS_UINT32 g_ulSzieof_g_stMmOmMsgIdStru;
extern APP_MM_DT_REPORT_CTRL_STRU  gastNasDtRptCtrlTbl[NAS_DT_RPT_CTRL_MAX_ITEM_NUM];

extern APP_MS_CLASSMARK_STRU        g_aucMsClassMark;

/* xiongxianghui00253310 modify for ftmerrlog begin */
extern EMM_FTM_INFO_MANAGE_STRU              g_astEmmFtmInfoManage;
extern EMM_DATABASE_INFO_STRU                g_astEmmInfo;
extern EMM_ERRLOG_INFO_MANAGE_STRU           g_astEmmErrlogInfoManage;

/* xiongxianghui00253310 modify for ftmerrlog end   */



/*****************************************************************************
  8 Fuction Extern
*****************************************************************************/
extern  NAS_LMM_OM_ACTION_FUN  NAS_LMM_OmFindFun( VOS_UINT32          ulMsgId);
extern VOS_UINT32 NAS_LMM_DtCompareGuti(VOS_VOID);
extern VOS_UINT32 NAS_LMM_DtCompareImsi(VOS_VOID);
extern VOS_UINT32 NAS_LMM_DtCompareEmmState(VOS_VOID);
extern VOS_VOID NAS_LMM_DtJudgeifNeedRpt(VOS_VOID);
extern VOS_VOID  NAS_LMM_ReadImsaNvImsCapability( VOS_UINT8 *pucSmsOverIms, VOS_UINT8 *pucSrvcc);

/*
extern  VOS_VOID    NAS_LMM_MtTpSetOptUeNetCap ( MsgBlock    *pMsg);
extern  VOS_VOID    NAS_LMM_MtTpSetOptMsNetCap(  MsgBlock    *pMsg);
*/
extern  VOS_VOID  NAS_LMM_SetAppParaValue
(
    NAS_LMM_APP_PARA_SET_RESLT_STRU      *pstParaInfo,
    const NAS_LMM_APP_PARA_SET_HEAD_STRU       *pstAppMsg
);
extern  VOS_UINT32  NAS_LMM_OmMaintain(          MsgBlock    *pMsg);
extern VOS_VOID     NAS_LMM_PubmDaInqRegStat( MsgBlock    *pMsg );
extern VOS_UINT32 NAS_EMM_OmInfoIndMsgProc(MsgBlock    *pMsg );

extern VOS_UINT32  NAS_LMM_CompareEmmInfo( VOS_VOID );

extern VOS_VOID     NAS_LMM_PubmDaInqLteCs(   MsgBlock    *pMsg  );
extern VOS_VOID     NAS_LMM_PubmDaInqUeMode(   MsgBlock    *pMsg  );
extern  VOS_VOID    NAS_LMM_SendOmtKeyEvent(     PS_EMM_OMT_KEY_EVENT_TYPE_ENUM_UINT8  ucEmmOmtKeyEvent  );
extern  VOS_VOID    NAS_LMM_SendOmtAirMsg(       NAS_LMM_OM_AIR_MSG_UP_DOWN_ENUM_UINT8  ucDir,
                                                NAS_LMM_OM_AIR_MSG_ID_ENUM_UINT8       ucAirMsgId,
                                                NAS_MSG_STRU                          *pstNasMsg);
extern  VOS_VOID    NAS_LMM_FindAirMsgId(        VOS_UINT8 ucCnMsgType,
                                                VOS_UINT8 *ucAirMsg,
                                                VOS_UINT8 *ucKeyEvent);
/*extern  VOS_VOID    NAS_LMM_PubmDaSendSetCnf(
                                            VOS_UINT32                             ulRslt,
                                             const NAS_LMM_APP_PARA_STRU           *pAppPara);*/
VOS_VOID    NAS_LMM_PubmDaSendSetCnf( const NAS_LMM_APP_PARA_SET_RESLT_STRU *pstAppRslt);
extern  VOS_VOID NAS_EMM_SendIntraTauReq
(
    APP_MM_MSG_TYPE_ENUM_UINT32         ulOmMmMsgType,
    NAS_LMM_INTRA_TAU_TYPE_ENUM_UINT32  enIntraTauType
);

extern VOS_VOID    NAS_LMM_PubmGetAreaInfo
       (
           APP_GUTI_STRU                       *pstGuti,
           APP_TA_LIST_STRU                    *pstTaiList,
           APP_TA_STRU                         *pstLastTa
       );
extern VOS_VOID    NAS_LMM_PubmGetEmmInfo
       (
           APP_EMM_INFO_STRU                   *pstEmmInfoCnf
       );
extern VOS_VOID    NAS_LMM_PubmGetEmmState
       (
           APP_EMM_MAIN_STATE_ENUM_UINT16      *pulProtoclMainState,
           APP_EMM_SUB_STATE_ENUM_UINT16       *pulProtoclSubState,
           NAS_LMM_MAIN_STATE_ENUM_UINT16      enMainState,
           NAS_LMM_SUB_STATE_ENUM_UINT16       enSubState

       );
extern VOS_VOID    NAS_LMM_PubmGetEmmUpdateState
       (
           APP_EMM_UPDATE_STATE_ENUM_UINT32    *pulEmmUpState
       );
extern VOS_VOID    NAS_LMM_PubmIndEmmInfo(VOS_VOID);
extern VOS_UINT32  NAS_LMM_CheckAppDrxParam( CONST APP_DRX_STRU *pstAppDrxParam );
extern VOS_UINT32  NAS_LMM_IsDrxSplitOnCcchValid( VOS_UINT8 ucSplitOnCcch );
extern VOS_UINT32  NAS_LMM_IsDrxNonDrxTimerValid( VOS_UINT8 ucNonDrxTimer );
extern VOS_UINT32  NAS_LMM_IsDrxPgCycleCodeValid( VOS_UINT8 ucSplitPgCode );
extern VOS_UINT32  NAS_LMM_IsUeDrxCycleValid( VOS_UINT8 ucPsDrxLen );

/*PC REPLAY MODIFY BY LEILI DELETE*/
/* xiongxianghui00253310 modify for ftmerrlog begin */
extern VOS_VOID     NAS_LMM_OmInfoIndProc(VOS_VOID);
extern VOS_VOID     NAS_LMM_FtmInfoInit(VOS_VOID);
extern VOS_UINT32   NAS_LMM_RevOmFtmCtrlMsg(MsgBlock   *pMsgStru);
extern VOS_UINT32   NAS_LMM_CompareEmmDatabaseInfo(VOS_VOID);
extern VOS_VOID     NAS_LMM_UpdateEmmDatabaseInfo(VOS_VOID);
extern VOS_VOID     NAS_LMM_SendOmFtmMsg(VOS_VOID);

extern VOS_VOID     NAS_LMM_ErrlogInfoInit(VOS_VOID);
extern VOS_UINT32   NAS_LMM_RevOmErrlogCtrlMsg(MsgBlock   *pMsgStru);
extern VOS_UINT32   NAS_LMM_RevOmReadErrlogReq(const MsgBlock   *pMsgStru);
extern VOS_VOID     NAS_LMM_SendOmErrlogCnf(VOS_VOID);
extern VOS_VOID NAS_LMM_ErrlogInfoProc(VOS_VOID   *pMsgStru, LMM_ERRLOG_INFO_TYPE_ENUM_UINT32  ulLnasSignalType);
extern LNAS_OM_ERRLOG_ID_ENUM_UINT16  NAS_LMM_CnCauseProc(NAS_EMM_CN_CAUSE_ENUM_UINT8 ucCnCause);
extern VOS_VOID NAS_LMM_SaveRevMsgInfo(MsgBlock *pMsg);
extern VOS_VOID NAS_LMM_ExportRevMsgQueque2ExcLog(VOS_UINT32* pulExcLogAddr, VOS_UINT32 ulSaveSize);

/* xiongxianghui00253310 modify for ftmerrlog end   */

/* niuxiufan DT begin */
extern VOS_VOID  NAS_LMM_PubmDaInqGuti(MsgBlock *pMsg);
extern VOS_VOID  NAS_LMM_PubmDaInqImsi(MsgBlock *pMsg);
extern VOS_VOID  NAS_LMM_PubmDaInqEmmState(MsgBlock *pMsg);
extern VOS_VOID  NAS_LMM_PubmIndGuti(VOS_VOID);
extern VOS_VOID  NAS_LMM_PubmIndImsi(VOS_VOID);
extern VOS_VOID  NAS_LMM_PubmIndEmmState(VOS_VOID);
/* niuxiufan DT end */


/*****************************************************************************
  9 OTHERS
*****************************************************************************/

#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of NasLmmPubMOm.h*/
