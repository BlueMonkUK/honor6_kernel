/******************************************************************************

   Copyright(C)2008,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : IpIpmIface.c
  Description     : IPģ����ļ�
  History         :
     1.hanlufeng 41410       2010-8-2   Draft Enact
******************************************************************************/


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include  "IpIpmIface.h"
#include  "IpIpmOm.h"
#include  "IpIpmGlobal.h"
#include  "IpIpmReg.h"
#include  "IpIpmMsgProc.h"
/*#include  "IpDhcpv4Server.h"*/

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_IPIPMIFACE_C
/*lint +e767*/



/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/



/*****************************************************************************
  3 Function
*****************************************************************************/
/*****************************************************************************
 Function Name   : IP_IsIpv4UdpPacket
 Description     : �ж��Ƿ�ΪIPV4�������ϲ�Э��ΪUDP
 Input           : pucIpMsg---------------------IP���ݰ��׵�ַ
                   ulIpMsgLen-------------------IP���ݰ�����
 Output          : VOS_VOID
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2010-11-29  Draft Enact

*****************************************************************************/
VOS_UINT32 IP_IsIpv4UdpPacket
(
    const VOS_UINT8                    *pucIpMsg,
    VOS_UINT32                          ulIpMsgLen
)
{
    VOS_UINT8                           ucProtocol      = IP_NULL;
    VOS_UINT8                           ucIpVersion     = IP_NULL;

    /* ���ȺϷ���� */
    if (ulIpMsgLen < IP_IPV4_HEAD_LEN)
    {
        return PS_FAIL;
    }

    /* ��ȡIP�汾�� */
    ucIpVersion = IP_GetIpVersion(pucIpMsg);

    /* ����汾�Ų���IPV4������DHCPV4�� */
    if (IP_VERSION_4 != ucIpVersion)
    {
        return PS_FAIL;
    }

    /* ȡ��Э�� */
    ucProtocol = IP_GetProtocol(pucIpMsg);

    /* ��Э�鲻��UDP������DHCPV4�� */
    if (IP_HEAD_PROTOCOL_UDP != ucProtocol)
    {
        return PS_FAIL;
    }

    return PS_SUCC;
}

/*****************************************************************************
 Function Name   : IP_IsDhcpv4ServerPacket
 Description     : �ж��Ƿ�ΪDHCPV4 SERVER��
 Input           : pucIpMsg---------------------IP���ݰ��׵�ַ
                   ulIpMsgLen-------------------IP���ݰ�����
 Output          : VOS_VOID
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2010-11-29  Draft Enact

*****************************************************************************/
/*lint -e960*/
/*lint -e961*/
VOS_UINT32 IP_IsDhcpv4ServerPacket
(
    const VOS_UINT8                    *pucIpMsg,
    VOS_UINT32                          ulIpMsgLen
)
{
    VOS_UINT8                           ucIpHeadLen     = IP_NULL;
    VOS_UINT16                          usPortDes       = IP_NULL;

    /* �ж��Ƿ�ΪIPV4�������ϲ�Э��ΪUDP */
    if (PS_SUCC != IP_IsIpv4UdpPacket(pucIpMsg, ulIpMsgLen))
    {
        return PS_FAIL;
    }

    /* ȡ��ipͷ���� */
    ucIpHeadLen = IP_GetIpv4HeaderLen(pucIpMsg);

    /* �ж϶˿ں��Ƿ�Ϊ67 */
    IP_GetUdpDesPort(pucIpMsg + ucIpHeadLen, usPortDes);
    if (IP_PORT_DHCPV4_SERVER != usPortDes)
    {
        return PS_FAIL;
    }

    return PS_SUCC;
}
VOS_VOID IP_Init(VOS_VOID)
{
    /* ��ӡ����ú��� */
    IP_LOG_INFO("IP_Init is entered.\n");

    /* IPM��ģ���ʼ�� */
    IP_IPM_Init();

    /* DHCPV4 SERVER��ģ���ʼ�� */
    /*IP_DHCPV4SEVER_Init();*/
}

/*****************************************************************************
 Function Name  : IP_PidInit
 Discription    : ip��PID��ʼ������
 Input          :
 Output         : None
 Return         : None
 History:
      1.lihong00150010      2010-11-29  Draft Enact

*****************************************************************************/
VOS_UINT32 IP_PidInit ( enum VOS_INIT_PHASE_DEFINE ip )
{
    switch( ip )
    {
        case    VOS_IP_LOAD_CONFIG:
                {
                    IP_Init();
                }
                break;

        case    VOS_IP_FARMALLOC:
        case    VOS_IP_INITIAL:
        case    VOS_IP_ENROLLMENT:
        case    VOS_IP_LOAD_DATA:
        case    VOS_IP_FETCH_DATA:
        case    VOS_IP_STARTUP:
        case    VOS_IP_RIVAL:
        case    VOS_IP_KICKOFF:
        case    VOS_IP_STANDBY:
        case    VOS_IP_BROADCAST_STATE:
        case    VOS_IP_RESTART:
        case    VOS_IP_BUTT:
                break;

        default:
                break;
    }

    return VOS_OK;
}


/*****************************************************************************
 Function Name   : IP_TaskEntry
 Description     : ���н���IPģ�����Ϣ�����������IPģ�飬�ɴ˺�������·����Ϣ
                   ��һ���ַ���IP����ģ�飬�����ø���ģ�����Ϣ���
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-11-22  Draft Enact

*****************************************************************************/
VOS_VOID  IP_TaskEntry
(
    MsgBlock                           *pstMsg
)
{
    /* ������Ϣͷָ��*/
    PS_MSG_HEADER_STRU                 *pHeader = IP_NULL_PTR;
    IP_ROUTER_MEG_ENTRY_FUN             pActFun = IP_NULL_PTR;
    VOS_UINT32                          ulMsgId = IP_NULL;
    IP_ERR_ENUM_UINT32                  enRslt  = IP_ERR_BUTT;

    /* �ж���ڲ����Ƿ�Ϸ�*/
    if( VOS_NULL_PTR == pstMsg)
    {
        IP_LOG_WARN("IP_TaskEntry:ERROR: No Msg!");
        return;
    }

    /* ��ȡ��Ϣͷָ��*/
    pHeader = (PS_MSG_HEADER_STRU *) pstMsg;

    /* ��ӡ�յ�����Ϣ���� */
    IP_PrintIpRevMsg(pHeader,IP_GET_IP_PRINT_BUF());

    /* ��ȡ��ϢID */
    IP_GetNameFromMsg(ulMsgId, (VOS_VOID*)pHeader);

    /* ����������Ϣ�Ƿ���ҪIPMģ�鴦�� */
    pActFun = IP_LayerMsgFindFun(IP_ROUTER_IPM, ulMsgId);
    if (IP_NULL_PTR != pActFun)
    {
        enRslt = pActFun((VOS_VOID*)pstMsg);
        IP_LOG1_NORM("IP_TaskEntry:The result of IPM function : " , enRslt);
    }

    /* ����������Ϣ�Ƿ���ҪDHCPV4 SERVERģ�鴦�� */
    pActFun = IP_LayerMsgFindFun(IP_ROUTER_DHCPV4SERVER, ulMsgId);
    if (IP_NULL_PTR != pActFun)
    {
        enRslt = pActFun((VOS_VOID*)pstMsg);
        IP_LOG1_NORM("IP_TaskEntry:The result of DHCPV4SERVER function : " , enRslt);
    }
}
/*lint +e961*/
/*lint +e960*/

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

