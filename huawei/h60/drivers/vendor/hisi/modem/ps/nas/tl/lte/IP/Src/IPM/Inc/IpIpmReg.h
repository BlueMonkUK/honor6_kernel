

#ifndef __IPIPMREG_H__
#define __IPIPMREG_H__

/*****************************************************************************
  1 Include Headfile
*****************************************************************************/
#include "vos.h"
#include "IpComm.h"


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
  2 Macro
*****************************************************************************/
#define IP_MAX_USER_NUM                 (8)

#define IP_MAX_ROUTER_TER_NUM           (16)

#define IP_VOS_HEADER_LEN               (20)

/* ��һ�ֽ�01��ʾ�����Ϣͷ�������ֽڱ�ʾ�����Ϣ�ĺ�24λ */
#define IP_ROUTER_LAYERMSG_HEADER       (0x01000000)

/* ��һ�ֽ�02��ʾ��ʱ����Ϣͷ�������ֽڱ�ʾ��ʱ����Ϣ������ģ�� */
#define IP_ROUTER_TIMERMSG_HEADER       (0x02000000)


/* ��ȡ·�ɱ� */
#define IP_GetRouterTable()             (g_stIpRouterSubTable)

/* ��ȡ·�ɱ��¼���� */
#define IP_GetRouterTableItemNum()\
            (sizeof(g_stIpRouterSubTable) / sizeof(IP_ROUTER_SUB_ITEM_STRU))


#define IP_ROUTER_ITEM_REG(ulSubId, ulMsgId, pfFunMsgEntry)\
        {\
            ulSubId,\
            ulMsgId,\
            IP_VOS_HEADER_LEN,\
            pfFunMsgEntry\
        }

#define IP_ROUTER_LAYERMSG_ET(ulMsgId)\
           ((ulMsgId & 0x00ffffff) | IP_ROUTER_LAYERMSG_HEADER)

#define IP_ROUTER_TIMERMSG_ET(ulModuleId)\
           ((ulModuleId & 0x00ffffff) | IP_ROUTER_TIMERMSG_HEADER)


#define IP_ROUTER_LAYERMSG_REG(ulSubId, ulMsgId, pfFunMsgEntry)\
           IP_ROUTER_ITEM_REG(ulSubId, IP_ROUTER_LAYERMSG_ET(ulMsgId), pfFunMsgEntry)

#define IP_ROUTER_TIMERMSG_REG(ulSubId, ulMsgId, pfFunMsgEntry)\
           IP_ROUTER_ITEM_REG(ulSubId, IP_ROUTER_TIMERMSG_ET(ulMsgId), pfFunMsgEntry)


/*****************************************************************************
  3 Massage Declare
*****************************************************************************/


/*****************************************************************************
  4 Enum
*****************************************************************************/
/*****************************************************************************
 �ṹ��    : IP_WHE_NEED_SER_ENUM
 �ṹ˵��  : �û��Ƿ���Ҫĳ�ַ���
 *****************************************************************************/
enum    IP_WHE_NEED_SER_ENUM
{
    IP_SERVICE_NEED,
    IP_SERVICE_NOT_NEED,
    IP_SERVICE_BUTT
};
typedef VOS_UINT32  IP_WHE_NEED_SER_ENUM_UINT32;



/*****************************************************************************
 �ṹ��    : IP_SERVICE_TYPE_ENUM
 �ṹ˵��  : IPģ����û��ṩ�ķ�������
 *****************************************************************************/
enum    IP_SERVICE_TYPE_ENUM
{
    IP_SER_TYPE_DHCPV4_STATUS_RPT,       /* DHCPV4��״̬���� */
    IP_SER_TYPE_BUTT
};
typedef VOS_UINT32  IP_SERVICE_TYPE_ENUM_UINT32;


/*****************************************************************************
  5 STRUCT
*****************************************************************************/

/*****************************************************************************
 �ṹ��    : IP_USER_SUB_SERVICE_STRU
 �ṹ˵��  : IPģ����û�ע�ᡢ����ĳ��������ڽṹ��
             ÿ�ַ�������ǩԼ������һ��
 *****************************************************************************/
typedef struct
{
    /* ǩԼ�û�ID, ESMǩԼʱ����ID�� EPSID */
    VOS_UINT32                          ulSubId;

    VOS_UINT32                          ulSubPid;

    /* ĳ����� */
    IP_SERVICE_TYPE_ENUM_UINT32         ulServiceType;

}IP_USER_SUB_SERVICE_STRU;



/*****************************************************************************
 �ṹ��    : IP_USER_SUB_ITEM_STRU
 �ṹ˵��  : IPģ����û�ע�����Ϣ��ÿ�ַ�������ǩԼһ��
 *****************************************************************************/
typedef struct
{
    /* ǩԼ�û�ID, ESMǩԼʱ����ID�� EPSID */
    VOS_UINT32                          ulSubId;

    VOS_UINT32                          ulSubPid;

    /* �Ƿ�ǩԼʹ�õ�DHCPV4״̬���湦�� */
    IP_WHE_NEED_SER_ENUM_UINT32         ulWheNeedDhcpv4StaRpt;

}IP_USER_SUB_ITEM_STRU;

/*****************************************************************************
 �ṹ��    :IP_ROUTER_TERMINAL_ENUM
 �ṹ˵��  :IPģ��ö��
 *****************************************************************************/
enum    IP_ROUTER_TERMINAL_ENUM
{
    IP_ROUTER_IPM                       = 1,
    IP_ROUTER_DHCPV4CLIENT              = 2,
    IP_ROUTER_DHCPV4SERVER              = 3,
    IP_ROUTER_BUTT
};
typedef VOS_UINT32  IP_ROUTER_TERMINAL_ENUM_UINT32;


/*****************************************************************************
 �ṹ��    : IP_ROUTER_MEG_ENTRY_FUN
 �ṹ˵��  : Ŀ��·��ģ��ע�����Ϣ����ں���
 *****************************************************************************/
typedef IP_ERR_ENUM_UINT32 (*IP_ROUTER_MEG_ENTRY_FUN)
(
    VOS_VOID *  /* ��Ϣָ�룬��Ϣ�ṹ����ģ��Э�� */
);

/*****************************************************************************
 �ṹ��    : IP_ROUTER_SUB_ITEM_STRU
 �ṹ˵��  : IPģ��ת������Ŀ��·��ģ���ǩԼ��Ϣ
 *****************************************************************************/
typedef struct
{

    /* ǩԼROUTER ID */
    IP_ROUTER_TERMINAL_ENUM_UINT32      ulSubId;

    /* �����˺����İ��ı�ǩ��
       ����: DOPRA��MsgId: �˴��� ESM_IP_CONFIG_PARA_REQ */
    VOS_UINT32                          ulMsgId;

    /* ��ǩԼ�����İ��ı�ǩ����Ϣ�е��ֽ�����, ��Ϣͷ�ֽ�λ������Ϊ 0
       ����: DOPRA�����Ϣ��MSGID������λ��: �˴��� 20 */
    VOS_UINT32                          ulTagOffset;


    /* Ŀ��·��ģ���ṩ����Ϣ��ں��� */
    IP_ROUTER_MEG_ENTRY_FUN             pfFunMsgEntry;

}IP_ROUTER_SUB_ITEM_STRU;



/*****************************************************************************
  6 UNION
*****************************************************************************/


/*****************************************************************************
  7 Extern Global Variable
*****************************************************************************/



/*****************************************************************************
  8 Fuction Extern
*****************************************************************************/
extern IP_ROUTER_MEG_ENTRY_FUN IP_LayerMsgFindFun
(
    IP_ROUTER_TERMINAL_ENUM_UINT32      ulSubId,
    VOS_UINT32                          ulTag
);

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

#endif /* end of IpIpmReg.h */

