/******************************************************************************

   Copyright(C)2008,Hisilicon Co. LTD.

 ******************************************************************************
  File Name       : IpIpmOm.c
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
#include    "IpComm.h"
#include    "IpIpmOm.h"
#include    "IpIpmGlobal.h"

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_IPIPMOM_C
/*lint +e767*/



/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/
/* ��ӡ������ */
VOS_CHAR                                g_acIpPrintBuf[IP_PRINT_BUFF_LEN] = "";


/* IP-ESM ��Ϣ��ӡ���� */
IP_PRINT_MSG_LIST_STRU g_astIpEsmMsgIdArray[] =
{
    {   ID_ESM_IP_CONFIG_PARA_REQ                   ,
            "MSG:  ID_ESM_IP_CONFIG_PARA_REQ\t\t"   ,
            IP_PrintEsmIpConfigParaReqMsg}          ,
    {   ID_ESM_IP_CONFIG_PARA_IND                   ,
            "MSG:  ID_ESM_IP_CONFIG_PARA_IND\t\t"   ,
            IP_PrintEsmIpConfigParaIndMsg}          ,
    {   ID_ESM_IP_PDP_RELEASE_IND                   ,
            "MSG:  ID_ESM_IP_PDP_RELEASE_IND\t\t"   ,
            IP_PrintEsmIpPdpReleaseIndMsg}          ,
    {   ID_ESM_IP_NW_PARA_IND                       ,
            "MSG:  ID_ESM_IP_NW_PARA_IND\t\t"       ,
            IP_NULL_PTR}                            ,
    {   ID_ESM_IP_REL_IND                           ,
            "MSG:  ID_ESM_IP_REL_IND\t\t"           ,
            IP_NULL_PTR}                            ,
    {   ID_ESM_IP_STATE_IND                         ,
            "MSG:  ID_ESM_IP_STATE_IND\t\t"         ,
            IP_NULL_PTR}                            ,
    {   ID_ESM_IP_START_REQ                         ,
            "MSG:  ID_ESM_IP_START_REQ\t\t"         ,
            IP_NULL_PTR}                            ,
    {   ID_ESM_IP_START_CNF                         ,
            "MSG:  ID_ESM_IP_START_CNF\t\t"         ,
            IP_NULL_PTR}                            ,
    {   ID_ESM_IP_STOP_REQ                          ,
            "MSG:  ID_ESM_IP_STOP_REQ\t\t"          ,
            IP_NULL_PTR}                            ,
    {   ID_ESM_IP_STOP_CNF                          ,
            "MSG:  ID_ESM_IP_STOP_CNF\t\t"          ,
            IP_NULL_PTR}
};

/* IP-RABM ��Ϣ��ӡ���� */
IP_PRINT_MSG_LIST_STRU g_astIpRabmMsgIdArray[] =
{
    {   ID_IP_RABM_START_FILTER_REQ                     ,
            "MSG:  ID_IP_RABM_START_FILTER_REQ\t\t"     ,
            IP_PrintIpRabmStartOrStopFilterReqMsg}      ,
    {   ID_IP_RABM_START_FILTER_CNF                     ,
            "MSG:  ID_IP_RABM_START_FILTER_CNF\t\t"     ,
            IP_NULL_PTR}                                ,
    {   ID_IP_RABM_STOP_FILTER_REQ                      ,
            "MSG:  ID_IP_RABM_STOP_FILTER_REQ\t\t"      ,
            IP_PrintIpRabmStartOrStopFilterReqMsg}      ,
    {   ID_IP_RABM_STOP_FILTER_CNF                      ,
            "MSG:  ID_IP_RABM_STOP_FILTER_CNF\t\t"      ,
            IP_NULL_PTR}                                ,
    {   ID_IP_RABM_DATA_IND                             ,
            "MSG:  ID_IP_RABM_DATA_IND\t\t"             ,
            IP_PrintIpRabmDataIndMsg}
};

#if 0
/* IP��ʱ����Ϣ��ӡ���� */
IP_PRINT_MSG_LIST_STRU g_astIpTimerArray[] =
{
    {   IP_DHCPV4_TIMER_DISCOVER                    ,
            "STATE TI:  IP_DHCPV4_TIMER_DISCOVER\t\t",
            IP_NULL_PTR}                            ,
    {   IP_DHCPV4_TIMER_REQUEST                     ,
            "STATE TI:  IP_DHCPV4_TIMER_REQUEST\t\t",
            IP_NULL_PTR}                            ,
    {   IP_DHCPV4_TIMER_LEASE                       ,
            "STATE TI:  IP_DHCPV4_TIMER_LEASE\t\t"  ,
            IP_NULL_PTR}                            ,
    {   IP_DHCPV4_TIMER_T1                          ,
            "STATE TI:  IP_DHCPV4_TIMER_T1\t\t"     ,
            IP_NULL_PTR}                            ,
    {   IP_DHCPV4_TIMER_T2                          ,
            "STATE TI:  IP_DHCPV4_TIMER_T2\t\t"     ,
            IP_NULL_PTR}                            ,
    {   IP_DHCPV4_TIMER_INFORM                      ,
            "STATE TI:  IP_DHCPV4_TIMER_INFORM\t\t" ,
            IP_NULL_PTR}
};
#endif




/*****************************************************************************
  3 Function
*****************************************************************************/
/*****************************************************************************
 Function Name   : IP_Nsprintf
 Description     : ��ӡ�ַ�����������
 Input           : pcBuff    -- ������
                   usOffset  -- �������еĴ�ӡ��ʼλ��
                   String    -- Ҫ��ӡ���ַ���
 Output          : pilOutLen -- ���ش�ӡ������ȥ�е��ַ��ĸ���
                                ��ӡ�����쳣ʱ������0
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-11-27  Draft Enact

*****************************************************************************/
/*lint -e960*/
/*lint -e961*/
VOS_VOID    IP_Nsprintf
(
    VOS_CHAR                           *pcBuff,
    VOS_UINT16                          usOffset,
    const VOS_CHAR                     *String,
    VOS_INT32                          *pilOutLen
)
{
    /* ��μ�� */
    if ( VOS_NULL_PTR == pilOutLen )
    {
        IP_LOG_ERR("IP_Nsprintf, pilOutLen is NULL pointer!");
        return;
    }

    if ( VOS_NULL_PTR == pcBuff )
    {
        IP_LOG_ERR("IP_Nsprintf, pcBuff is NULL pointer!");
        *pilOutLen = 0;
        return;
    }

    if ( VOS_NULL_PTR == String )
    {
        IP_LOG_ERR("IP_Nsprintf, String is NULL pointer!");
        *pilOutLen = 0;
        return;
    }

    if ((IP_PRINT_BUFF_LEN - 2) >= usOffset )
    {
        /* ��ҪԤ��һ���ַ���Ϊ������ */
        *pilOutLen = VOS_nsprintf(pcBuff + usOffset,
                                  (IP_PRINT_BUFF_LEN - 1) - usOffset,
                                  String);
        if( (0 == *pilOutLen) || (PS_NEG_ONE == *pilOutLen) )
        {
            IP_LOG1_WARN("IP_Nsprintf, *pilOutLen=", *pilOutLen);
            *pilOutLen = 0;
            return;
        }
    }
    else
    {

        IP_LOG_ERR("IP_Nsprintf, Print too long!");
        *pilOutLen = 0;
        return;
    }
}

/*****************************************************************************
 Function Name   : IP_Nsprintf_1
 Description     : ��ӡ�ַ�����������
 Input           : pcBuff    -- ������
                   usOffset  -- �������еĴ�ӡ��ʼλ��
                   String    -- Ҫ��ӡ���ַ���
                   para1     -- ����String��Ҫ�õ��Ĳ���
 Output          : pilOutLen -- ���ش�ӡ������ȥ�е��ַ��ĸ���
                                ��ӡ�����쳣ʱ������0
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-11-27  Draft Enact

*****************************************************************************/
VOS_VOID    IP_Nsprintf_1
(
    VOS_CHAR                           *pcBuff,
    VOS_UINT16                          usOffset,
    const VOS_CHAR                     *String,
    VOS_UINT32                          para1,
    VOS_INT32                          *pilOutLen
)
{
    /* ��μ�� */
    if ( VOS_NULL_PTR == pilOutLen )
    {
        IP_LOG_ERR("IP_Nsprintf_1, pilOutLen is NULL pointer!");
        return;
    }

    if ( VOS_NULL_PTR == pcBuff )
    {
        IP_LOG_ERR("IP_Nsprintf_1, pcBuff is NULL pointer!");
        *pilOutLen = 0;
        return;
    }

    if ( VOS_NULL_PTR == String )
    {
        IP_LOG_ERR("IP_Nsprintf_1, String is NULL pointer!");
        *pilOutLen = 0;
        return;
    }

    if ((IP_PRINT_BUFF_LEN - 2) >= usOffset )
    {
        /* ��ҪԤ��һ���ַ���Ϊ������ */
        *pilOutLen = VOS_nsprintf(pcBuff + usOffset,
                                  (IP_PRINT_BUFF_LEN - 1) - usOffset,
                                  String,
                                  para1);
        if( (0 == *pilOutLen) || (PS_NEG_ONE == *pilOutLen) )
        {
            IP_LOG1_WARN("IP_Nsprintf_1,*pilOutLen=", *pilOutLen);
            *pilOutLen = 0;
            return;
        }
    }
    else
    {
        IP_LOG_ERR("IP_Nsprintf_1, Print too long!");

        *pilOutLen = 0;
        return;
    }
}

/*****************************************************************************
 Function Name   : IP_Nsprintf_2
 Description     : ��ӡ�ַ�����������
 Input           : pcBuff    -- ������
                   usOffset  -- �������еĴ�ӡ��ʼλ��
                   String    -- Ҫ��ӡ���ַ���
                   para1     -- ����String��Ҫ�õ��Ĳ���
                   para2     -- ����String��Ҫ�õ��Ĳ���
 Output          : pilOutLen -- ���ش�ӡ������ȥ�е��ַ��ĸ���
                                ��ӡ�����쳣ʱ������0
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-11-27  Draft Enact

*****************************************************************************/
VOS_VOID    IP_Nsprintf_2
(
    VOS_CHAR                           *pcBuff,
    VOS_UINT16                          usOffset,
    const VOS_CHAR                     *String,
    VOS_UINT32                          para1,
    VOS_UINT32                          para2,
    VOS_INT32                          *pilOutLen
)
{
    /* ��μ�� */
    if ( VOS_NULL_PTR == pilOutLen )
    {
        IP_LOG_ERR("IP_Nsprintf_2, pilOutLen is NULL pointer!");
        return;
    }

    if ( VOS_NULL_PTR == pcBuff )
    {
        IP_LOG_ERR("IP_Nsprintf_2, pcBuff is NULL pointer!");
        *pilOutLen = 0;
        return;
    }

    if ( VOS_NULL_PTR == String )
    {
        IP_LOG_ERR("IP_Nsprintf_2, String is NULL pointer!");
        *pilOutLen = 0;
        return;
    }

    if ((IP_PRINT_BUFF_LEN - 2) >= usOffset )
    {
        /* ��ҪԤ��һ���ַ���Ϊ������ */
        *pilOutLen = VOS_nsprintf(pcBuff + usOffset,
                                  (IP_PRINT_BUFF_LEN - 1) - usOffset,
                                  String,
                                  para1,
                                  para2);
        if( (0 == *pilOutLen) || (PS_NEG_ONE == *pilOutLen) )
        {
            IP_LOG1_WARN("IP_Nsprintf_2,*pilOutLen=", *pilOutLen);
            *pilOutLen = 0;
            return;
        }
    }
    else
    {
        IP_LOG_ERR("IP_Nsprintf_2, Print too long!");

        *pilOutLen = 0;
        return;
    }
}

/*****************************************************************************
 Function Name   : IP_Print
 Description     : ���ӽ���������ӡ
 Input           : pcBuff -- Formatted buffer
                   usLen  -- Print length
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-11-27  Draft Enact

*****************************************************************************/
VOS_VOID    IP_Print
(
    VOS_CHAR                           *pcBuff,
    VOS_UINT16                          usLen
)
{
    /* ��μ�� */
    if ( VOS_NULL_PTR == pcBuff )
    {
        IP_LOG_ERR("IP_Print, Input param is NULL pointer!");
        return;
    }

    /* ��ҪԤ��һ���ַ���Ϊ������ */
    if ( usLen >= IP_PRINT_BUFF_LEN )
    {
        IP_LOG1_WARN("IP_Print,usLen=", usLen);
        return;
    }
    else
    {
        pcBuff[usLen] = '\0';
        IP_LOG_NORM(pcBuff);
        return;
    }
}


VOS_VOID   IP_PrintArray
(
    VOS_CHAR                           *pcBuff,
    const VOS_UINT8                    *pucArray,
    VOS_UINT32                          ulLen
)
{
    VOS_UINT16                          usTotalLen  = IP_NULL;
    VOS_INT32                           ilOutPutLen = IP_NULL;
    VOS_UINT16                          usLoop      = IP_NULL;
    VOS_UINT8                           ucData      = IP_NULL;


    /* ��ӡ����, һ�����16���ֽ� */
    for (usLoop = 0; usLoop < ulLen; usLoop++)
    {
        if ( (0 != usLoop) && (0 == (usLoop%IP_PRINT_MAX_BYTES_PER_LINE)) )
        {
            if(usTotalLen < IP_PRINT_BUFF_LEN)
            {
                pcBuff[usTotalLen] = '\n';
                usTotalLen++;
            }
        }
        ucData = pucArray[usLoop];
        IP_Nsprintf_1(  pcBuff,
                        usTotalLen,
                        "%02x ",
                        ucData,
                        &ilOutPutLen);
        if ( 0 == ilOutPutLen )
        {
            IP_LOG_WARN("IP_PrintArray: Print data exception.");
            return;
        }
        usTotalLen += (VOS_UINT16)ilOutPutLen;
    }

    IP_Print(pcBuff, usTotalLen);

    return;
}
VOS_INT32   IP_PrintData
(
    VOS_CHAR                           *pcBuff,
    VOS_UINT16                          usOffset,
    NAS_IP_MSG_STRU                    *pstMsg
)
{
    VOS_UINT16                          usTotalLen          = usOffset;
    VOS_INT32                           ilOutPutLen         = IP_NULL;
    VOS_UINT16                          usLoop              = IP_NULL;
    VOS_UINT32                          usDataLen           = pstMsg->ulIpMsgSize;
    VOS_UINT8                          *pCnMsgData          = pstMsg->aucIpMsg;
    VOS_UINT8                           ucCnMsgData         = IP_NULL;

    /* ��ӡ����, һ�����10���ֽ� */
    for (usLoop = 0; usLoop < usDataLen; usLoop++)
    {
        /* ÿ�н���ʱ��ӡ�к� */
        if ( (0 != usLoop) && (0 == (usLoop%IP_PRINT_MAX_BYTES_PER_LINE)) )
        {
            if(usTotalLen < IP_PRINT_BUFF_LEN)
            {
                pcBuff[usTotalLen] = '\n';
                usTotalLen++;
            }
        }

        ucCnMsgData = pCnMsgData[usLoop];
        IP_Nsprintf_1(  pcBuff,
                        usTotalLen,
                        "%02x ",
                        ucCnMsgData,
                        &ilOutPutLen);

        if ( 0 == ilOutPutLen )
        {
            IP_LOG_WARN("IP_PrintData, Print data exception.");
            return 0;
        }
        usTotalLen += (VOS_UINT16)ilOutPutLen;
    }

    return (usTotalLen - usOffset);
}


VOS_INT32  IP_PrintEsmIpMsg
(
    VOS_CHAR                            *pcBuff,
    VOS_UINT16                           usOffset,
    const PS_MSG_HEADER_STRU            *pstMsg
)
{
    VOS_UINT32                          ulLoop          = IP_NULL;
    VOS_UINT32                          ulEsmIpMsgNum   = IP_NULL;
    VOS_INT32                           ilOutPutLen     = IP_NULL;
    VOS_UINT16                          usTotalLen      = usOffset;


    /* �����Ϣ��ĳ��� */
    ulEsmIpMsgNum = sizeof(g_astIpEsmMsgIdArray)/sizeof(IP_PRINT_MSG_LIST_STRU);

    /* ���Ҷ�Ӧ����Ϣ */
    for (ulLoop = 0; ulLoop< ulEsmIpMsgNum ; ulLoop++)
    {
        if (pstMsg->ulMsgName == g_astIpEsmMsgIdArray[ulLoop].ulId)
        {
            break;
        }
    }

    /* ��ӡ��Ӧ����Ϣ */
    if ( ulLoop < ulEsmIpMsgNum )
    {
        IP_Nsprintf(    pcBuff,
                        usTotalLen,
                        (VOS_CHAR *)(g_astIpEsmMsgIdArray[ulLoop].aucPrintString),
                        &ilOutPutLen);
        usTotalLen += (VOS_UINT16)ilOutPutLen;

        IP_Nsprintf_1(  pcBuff,
                        usTotalLen,
                        "[ TICK : %ld ]",
                        VOS_GetTick(),
                        &ilOutPutLen);
        usTotalLen += (VOS_UINT16)ilOutPutLen;

        if (VOS_NULL_PTR != g_astIpEsmMsgIdArray[ulLoop].pfActionFun)
        {
            ilOutPutLen = g_astIpEsmMsgIdArray[ulLoop].pfActionFun(    pcBuff,
                                                                       usTotalLen,
                                                                       pstMsg);
        }

        usTotalLen += (VOS_UINT16)ilOutPutLen;
    }
    else
    {
        IP_LOG1_WARN("IP_PrintEsmIpMsg, Invalid enMsgId: ",pstMsg->ulMsgName);
    }

    return (usTotalLen - usOffset);
}
VOS_INT32  IP_PrintRabmIpMsg
(
    VOS_CHAR                            *pcBuff,
    VOS_UINT16                           usOffset,
    const PS_MSG_HEADER_STRU            *pstMsg
)
{
    VOS_UINT32                          ulLoop          = IP_NULL;
    VOS_UINT32                          ulRabmIpMsgNum  = IP_NULL;
    VOS_INT32                           ilOutPutLen     = IP_NULL;
    VOS_UINT16                          usTotalLen      = usOffset;


    /* �����Ϣ��ĳ��� */
    ulRabmIpMsgNum = sizeof(g_astIpRabmMsgIdArray)/sizeof(IP_PRINT_MSG_LIST_STRU);

    /* ���Ҷ�Ӧ����Ϣ */
    for (ulLoop = 0; ulLoop< ulRabmIpMsgNum ; ulLoop++)
    {
        if (pstMsg->ulMsgName == g_astIpRabmMsgIdArray[ulLoop].ulId)
        {
            break;
        }
    }

    /* ��ӡ��Ӧ����Ϣ */
    if ( ulLoop < ulRabmIpMsgNum )
    {
        IP_Nsprintf(    pcBuff,
                        usTotalLen,
                        (VOS_CHAR *)(g_astIpRabmMsgIdArray[ulLoop].aucPrintString),
                        &ilOutPutLen);
        usTotalLen += (VOS_UINT16)ilOutPutLen;

        IP_Nsprintf_1(  pcBuff,
                        usTotalLen,
                        "[ TICK : %ld ]",
                        VOS_GetTick(),
                        &ilOutPutLen);
        usTotalLen += (VOS_UINT16)ilOutPutLen;

        if (VOS_NULL_PTR != g_astIpRabmMsgIdArray[ulLoop].pfActionFun)
        {
            ilOutPutLen = g_astIpRabmMsgIdArray[ulLoop].pfActionFun(    pcBuff,
                                                                        usTotalLen,
                                                                        pstMsg);
        }

        usTotalLen += (VOS_UINT16)ilOutPutLen;
    }
    else
    {
        IP_LOG1_WARN("IP_PrintRabmIpMsg, Invalid enMsgId: ",pstMsg->ulMsgName);
    }

    return (usTotalLen - usOffset);
}
VOS_VOID IP_PrintIpRevMsg
(
    const PS_MSG_HEADER_STRU           *pstMsg,
    VOS_CHAR                           *pcBuff
)
{
    VOS_INT32                           ilOutPutLenHead     = 0;
    VOS_INT32                           ilOutPutLen         = 0;
    VOS_UINT16                          usTotalLen          = 0;

    IP_Nsprintf(    pcBuff,
                    usTotalLen,
                    "IP Receive Message:",
                    &ilOutPutLen);

    if ( 0 == ilOutPutLen )
    {
        IP_LOG_WARN("IP_PrintIpRevMsg, Print receive msg header exception.");
        return ;
    }

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    /* ��ӡ����PID����Ϣ */
    switch ( pstMsg->ulSenderPid )
    {
        case PS_PID_RABM:
            IP_Nsprintf(    pcBuff,
                            usTotalLen,
                            "RABM-->IP\t",
                            &ilOutPutLenHead);

            usTotalLen += (VOS_UINT16)ilOutPutLenHead;

            ilOutPutLen = IP_PrintRabmIpMsg(    pcBuff,
                                                usTotalLen,
                                                pstMsg);

            break;

        case PS_PID_ESM:
            IP_Nsprintf(    pcBuff,
                            usTotalLen,
                            "ESM-->IP\t",
                            &ilOutPutLenHead);

            usTotalLen += (VOS_UINT16)ilOutPutLenHead;

            ilOutPutLen = IP_PrintEsmIpMsg(pcBuff,
                                            usTotalLen,
                                            pstMsg);

            break;

        default:
            IP_LOG2_WARN("IP_PrintIpRevMsg,Invalid Pid, MsgId: ",
                          pstMsg->ulSenderPid,
                          pstMsg->ulMsgName);

            return ;
    }

    if ( 0 == ilOutPutLen )
    {
        IP_LOG2_WARN("IP_PrintIpRevMsg, print return zero length.SenderPid, MsgId: ",
                      pstMsg->ulSenderPid,
                      pstMsg->ulMsgName);
        return;
    }

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    IP_Print(pcBuff, usTotalLen);

    return ;

}
VOS_VOID IP_PrintIpSendMsg
(
    const PS_MSG_HEADER_STRU           *pstMsg,
    VOS_CHAR                           *pcBuff
)
{
    VOS_INT32                           ilOutPutLenHead     = 0;
    VOS_INT32                           ilOutPutLen         = 0;
    VOS_UINT16                          usTotalLen          = 0;

    IP_Nsprintf(    pcBuff,
                    usTotalLen,
                    "IP Send Message:",
                    &ilOutPutLen);

    if ( 0 == ilOutPutLen )
    {
        IP_LOG_WARN("IP_PrintIpSendMsg, Print send msg header exception.");
        return ;
    }

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    /* ��ӡ����PID����Ϣ */
    switch ( pstMsg->ulReceiverPid )
    {
        case PS_PID_RABM :
            IP_Nsprintf(    pcBuff,
                            usTotalLen,
                            "IP-->RABM\t",
                            &ilOutPutLenHead);

            usTotalLen += (VOS_UINT16)ilOutPutLenHead;

            ilOutPutLen = IP_PrintRabmIpMsg(    pcBuff,
                                                usTotalLen,
                                                pstMsg);
            break;

        case PS_PID_ESM :
            IP_Nsprintf(    pcBuff,
                            usTotalLen,
                            "IP-->ESM\t",
                            &ilOutPutLenHead);

            usTotalLen += (VOS_UINT16)ilOutPutLenHead;

            ilOutPutLen = IP_PrintEsmIpMsg(     pcBuff,
                                                usTotalLen,
                                                pstMsg);
            break;

        default:
            IP_LOG2_WARN("IP_PrintIpSendMsg,Invalid Pid, MsgId: ",
                            pstMsg->ulReceiverPid,
                            pstMsg->ulMsgName);
            return ;
    }

    if ( 0 == ilOutPutLen )
    {
        IP_LOG2_WARN("IP_PrintIpSendMsg, print return zero length. ReceiverPid, MsgId: ",
                            pstMsg->ulReceiverPid,
                            pstMsg->ulMsgName);
        return;
    }

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    IP_Print(pcBuff,usTotalLen);

    return ;

}
/*lint +e961*/
/*lint +e960*/

VOS_INT32  IP_PrintEsmIpConfigParaReqMsg
(
    VOS_CHAR                           *pcBuff,
    VOS_UINT16                          usOffset,
    const PS_MSG_HEADER_STRU           *pstMsg
)
{
    VOS_INT32                           ilOutPutLen = IP_NULL;
    VOS_UINT16                          usTotalLen  = usOffset;
    VOS_UINT32                          ulLoop      = IP_NULL;
    VOS_CHAR                           *pcPrintStr  = VOS_NULL_PTR;
    ESM_IP_CONFIG_PARA_REQ_STRU        *pstRcvMsg   = VOS_NULL_PTR;

    pstRcvMsg = (VOS_VOID *)pstMsg;

    pcPrintStr =  "\r\n ****************Begin :ESM_IP_CONFIG_PARA_REQ_STRU****************\r\n";

    /* ��ӡ��Ϣ�ṹ���� */
    IP_Nsprintf(    pcBuff,
                    usTotalLen,
                    pcPrintStr,
                    &ilOutPutLen);

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    /* ��ӡ���غ� */
    IP_Nsprintf_1(  pcBuff,
                    usTotalLen,
                    " ulEpsId = %d \r\n",
                    pstRcvMsg->ucEpsbId,
                    &ilOutPutLen);

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    /* ��ӡDHCPV4��ʶ */
    IP_Nsprintf_1(  pcBuff,
                    usTotalLen,
                    " enDhcpv4Flag = %d \r\n",
                    pstRcvMsg->enDhcpv4Flag,
                    &ilOutPutLen);

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    /* ��ӡIPV4��ַ */
    IP_Nsprintf(    pcBuff,
                    usTotalLen,
                    " Ipv4 Address : \r\n",
                    &ilOutPutLen);

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    for (ulLoop = 0; ulLoop < IP_IPV4_ADDR_LEN; ulLoop++)
    {
        IP_Nsprintf_2(  pcBuff,
                        usTotalLen,
                        " aucIpv4Addr[%d] = %d \r\n",
                        ulLoop,
                        pstRcvMsg->aucIpv4Addr[ulLoop],
                        &ilOutPutLen);

        usTotalLen += (VOS_UINT16)ilOutPutLen;
    }

    pcPrintStr =  " ****************End :ESM_IP_CONFIG_PARA_REQ_STRU****************";

    /* ��ӡ��Ϣ�Ľ�������*/
    IP_Nsprintf(    pcBuff,
                    usTotalLen,
                    pcPrintStr,
                    &ilOutPutLen);

   usTotalLen += (VOS_UINT16)ilOutPutLen;

    return (usTotalLen - usOffset) ;
}
VOS_INT32  IP_PrintEsmIpConfigParaIndMsg
(
    VOS_CHAR                           *pcBuff,
    VOS_UINT16                          usOffset,
    const PS_MSG_HEADER_STRU           *pstMsg
)
{
    VOS_INT32                           ilOutPutLen = IP_NULL;
    VOS_UINT16                          usTotalLen  = usOffset;
    VOS_UINT32                          ulLoop      = IP_NULL;
    VOS_CHAR                           *pcPrintStr  = VOS_NULL_PTR;
    ESM_IP_CONFIG_PARA_IND_STRU        *pstRcvMsg   = VOS_NULL_PTR;

    pstRcvMsg = (VOS_VOID *)pstMsg;

    pcPrintStr =  "\r\n ****************Begin :ESM_IP_CONFIG_PARA_IND_STRU****************\r\n";

    /* ��ӡ��Ϣ�ṹ���� */
    IP_Nsprintf(    pcBuff,
                    usTotalLen,
                    pcPrintStr,
                    &ilOutPutLen);

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    /* ��ӡ���غ� */
    IP_Nsprintf_1(  pcBuff,
                    usTotalLen,
                    " ulEpsId = %d \r\n",
                    pstRcvMsg->ucEpsbId,
                    &ilOutPutLen);

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    /* ��ӡbitOpIpv4Addr */
    IP_Nsprintf_1(  pcBuff,
                    usTotalLen,
                    " bitOpIpv4Addr = %d \r\n",
                    pstRcvMsg->bitOpIpv4Addr,
                    &ilOutPutLen);

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    /* ��ӡbitOpDnsServer */
    IP_Nsprintf_1(  pcBuff,
                    usTotalLen,
                    " bitOpIpv4Addr = %d \r\n",
                    pstRcvMsg->bitOpDnsServer,
                    &ilOutPutLen);

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    /* ��ӡIPV4��ַ */
    IP_Nsprintf(    pcBuff,
                    usTotalLen,
                    " Ipv4 Address : \r\n",
                    &ilOutPutLen);

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    for (ulLoop = 0; ulLoop < IP_IPV4_ADDR_LEN; ulLoop++)
    {
        IP_Nsprintf_2(  pcBuff,
                        usTotalLen,
                        " aucIpv4Addr[%d] = %d \r\n",
                        ulLoop,
                        pstRcvMsg->aucIpv4Addr[ulLoop],
                        &ilOutPutLen);

        usTotalLen += (VOS_UINT16)ilOutPutLen;
    }

    /* ��ӡDNS��������Ϣ */
    IP_Nsprintf_1(  pcBuff,
                    usTotalLen,
                    " DnsSerNum = %d \r\n",
                    pstRcvMsg->stDnsServer.ucDnsSerNum,
                    &ilOutPutLen);

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    pcPrintStr =  " ****************End :ESM_IP_CONFIG_PARA_IND_STRU****************";

    /* ��ӡ��Ϣ�Ľ�������*/
    IP_Nsprintf(    pcBuff,
                    usTotalLen,
                    pcPrintStr,
                    &ilOutPutLen);

   usTotalLen += (VOS_UINT16)ilOutPutLen;

    return (usTotalLen - usOffset) ;
}
VOS_INT32  IP_PrintEsmIpPdpReleaseIndMsg
(
    VOS_CHAR                           *pcBuff,
    VOS_UINT16                          usOffset,
    const PS_MSG_HEADER_STRU           *pstMsg
)
{
    VOS_INT32                           ilOutPutLen = IP_NULL;
    VOS_UINT16                          usTotalLen  = usOffset;
    VOS_CHAR                           *pcPrintStr  = VOS_NULL_PTR;
    ESM_IP_PDP_RELEASE_IND_STRU        *pstRcvMsg   = VOS_NULL_PTR;

    pstRcvMsg = (VOS_VOID *)pstMsg;

    pcPrintStr =  "\r\n ****************Begin :ESM_IP_PDP_RELEASE_IND_STRU****************\r\n";

    /* ��ӡ��Ϣ�ṹ���� */
    IP_Nsprintf(    pcBuff,
                    usTotalLen,
                    pcPrintStr,
                    &ilOutPutLen);

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    /* ��ӡ���غ� */
    IP_Nsprintf_1(  pcBuff,
                    usTotalLen,
                    " ulEpsId = %d \r\n",
                    pstRcvMsg->ucEpsbId,
                    &ilOutPutLen);

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    pcPrintStr =  " ****************End :ESM_IP_PDP_RELEASE_IND_STRU****************";

    /* ��ӡ��Ϣ�Ľ�������*/
    IP_Nsprintf(    pcBuff,
                    usTotalLen,
                    pcPrintStr,
                    &ilOutPutLen);

   usTotalLen += (VOS_UINT16)ilOutPutLen;

    return (usTotalLen - usOffset) ;
}
VOS_INT32  IP_PrintIpRabmStartOrStopFilterReqMsg
(
    VOS_CHAR                           *pcBuff,
    VOS_UINT16                          usOffset,
    const PS_MSG_HEADER_STRU           *pstMsg
)
{
    VOS_INT32                           ilOutPutLen = IP_NULL;
    VOS_UINT16                          usTotalLen  = usOffset;
    VOS_CHAR                           *pcPrintStr  = VOS_NULL_PTR;
    IP_RABM_START_FILTER_REQ_STRU      *pstRcvMsg   = VOS_NULL_PTR;

    pstRcvMsg = (VOS_VOID *)pstMsg;

    if (ID_IP_RABM_START_FILTER_REQ == pstRcvMsg->ulMsgId)
    {
        pcPrintStr =  "\r\n ****************Begin :IP_RABM_START_FILTER_REQ_STRU****************\r\n";
    }
    else
    {
        pcPrintStr =  "\r\n ****************Begin :IP_RABM_STOP_FILTER_REQ_STRU****************\r\n";
    }

    /* ��ӡ��Ϣ�ṹ���� */
    IP_Nsprintf(    pcBuff,
                    usTotalLen,
                    pcPrintStr,
                    &ilOutPutLen);

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    /* ��ӡ���������� */
    if (NAS_IP_FILTER_TYPE_DHCPV4 == pstRcvMsg->enFilterType)
    {
        pcPrintStr = " enFilterType = DHCPV4\r\n ";
    }
    else
    {
        pcPrintStr = " enFilterType = ICMPV6\r\n ";
    }

    IP_Nsprintf(    pcBuff,
                    usTotalLen,
                    pcPrintStr,
                    &ilOutPutLen);

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    if (ID_IP_RABM_START_FILTER_REQ == pstRcvMsg->ulMsgId)
    {
        pcPrintStr =  " ****************End :IP_RABM_START_FILTER_REQ_STRU****************";
    }
    else
    {
        pcPrintStr =  " ****************End :IP_RABM_STOP_FILTER_REQ_STRU****************";
    }

    /* ��ӡ��Ϣ�Ľ�������*/
    IP_Nsprintf(    pcBuff,
                    usTotalLen,
                    pcPrintStr,
                    &ilOutPutLen);

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    return (usTotalLen - usOffset) ;
}
VOS_INT32  IP_PrintIpRabmDataIndMsg
(
    VOS_CHAR                           *pcBuff,
    VOS_UINT16                          usOffset,
    const PS_MSG_HEADER_STRU           *pstMsg
)
{
    VOS_INT32                           ilOutPutLen = IP_NULL;
    VOS_UINT16                          usTotalLen  = usOffset;
    VOS_CHAR                           *pcPrintStr  = VOS_NULL_PTR;
    IP_RABM_DATA_IND_STRU              *pstRcvMsg   = VOS_NULL_PTR;

    pstRcvMsg = (VOS_VOID *)pstMsg;

    pcPrintStr =  "\r\n ****************Begin :IP_RABM_DATA_IND_STRU****************\r\n";

    /* ��ӡ��Ϣ�ṹ���� */
    IP_Nsprintf(    pcBuff,
                    usTotalLen,
                    pcPrintStr,
                    &ilOutPutLen);

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    /* ��ӡ���غ� */
    IP_Nsprintf_1(  pcBuff,
                    usTotalLen,
                    " ulEpsId = %d \r\n",
                    pstRcvMsg->ucEpsbId,
                    &ilOutPutLen);

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    /* ��ӡ��Ϣ���� */
    IP_Nsprintf_1(  pcBuff,
                    usTotalLen,
                    " ulIpMsgSize = %d \r\n",
                    pstRcvMsg->stIpMsg.ulIpMsgSize,
                    &ilOutPutLen);

    usTotalLen += (VOS_UINT16)ilOutPutLen;

    /* ��ӡCN��ϢЯ�������� */
    ilOutPutLen = IP_PrintData(pcBuff, usTotalLen, &pstRcvMsg->stIpMsg);

    usTotalLen += (VOS_UINT16)ilOutPutLen;


    pcPrintStr =  " ****************End :IP_RABM_DATA_IND_STRU****************";

    /* ��ӡ��Ϣ�Ľ�������*/
    IP_Nsprintf(    pcBuff,
                    usTotalLen,
                    pcPrintStr,
                    &ilOutPutLen);

   usTotalLen += (VOS_UINT16)ilOutPutLen;

    return (usTotalLen - usOffset) ;
}




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


