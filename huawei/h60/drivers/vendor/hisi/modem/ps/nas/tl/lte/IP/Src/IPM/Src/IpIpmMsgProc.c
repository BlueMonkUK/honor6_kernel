/******************************************************************************

   Copyright(C)2008,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : IpIpmMsgProc.c
  Description     : IPMģ�����Ϣ�����ļ�
  History         :
     1.lihong00150010   2010-11-25   Draft Enact
******************************************************************************/


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include    "IpIpmMsgProc.h"
#include    "NasIpInterface.h"
#include    "IpIpmGlobal.h"
#include    "IpIpmOm.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_IPIPMMSGPROC_C
/*lint +e767*/


/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/


/*****************************************************************************
  3 Function
*****************************************************************************/

VOS_VOID IP_IPM_Init(VOS_VOID)
{
    #if 0
    /* IPv4��ʼ������ */
    IP_MEM_SET(IP_GET_IPV4SRC(), IP_NULL, IP_IPV4_ADDR_LEN);

    IP_MEM_SET(IP_GET_IPV4DES(), IP_NULL, IP_IPV4_ADDR_LEN);
    #endif
}

/*****************************************************************************
 Function Name   : IP_IPM_ProcEsmIpRelInd
 Description     : ����ESM_IP_REL_IND��Ϣ
 Input           : MsgBlock           *pstMsg
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-11-25  Draft Enact

*****************************************************************************/
VOS_VOID  IP_IPM_ProcEsmIpRelInd
(
    VOS_VOID                           *pstMsg
)
{
    (VOS_VOID)pstMsg;

    /* ���IPMģ����Դ */
    IP_IPM_Init();
}

/*****************************************************************************
 Function Name   : IP_IPM_MsgEntry
 Description     : ��������IPM��ģ�����Ϣ
 Input           : MsgBlock           *pstMsg
 Output          : None
 Return          : IP_ERR_ENUM_UINT32

 History         :
    1.lihong00150010      2010-11-25  Draft Enact

*****************************************************************************/
IP_ERR_ENUM_UINT32  IP_IPM_MsgEntry
(
    VOS_VOID                           *pstMsg
)
{
    /* ��������IPM��ģ�����Ϣ */
    switch (IP_GetMsgId(pstMsg))
    {
        case ID_ESM_IP_REL_IND:
            IP_IPM_ProcEsmIpRelInd(pstMsg);
            break;
        case ID_ESM_IP_START_REQ:
        case ID_ESM_IP_STOP_REQ:
            break;
        default:
            IP_LOG_ERR("IP_IPM_MsgEntry:Illegal msg type!");
            break;
    }

    return IP_MSG_HANDLED;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

