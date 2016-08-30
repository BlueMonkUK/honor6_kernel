

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
/*#include "IpIpmGlobal.h"*/
/*#include "IpIpmOm.h"*/

/*lint -e767 */
/* #define    THIS_FILE_ID        PS_FILE_ID_IPIPMGLOBAL_C */
/*lint +e767*/\

#if 0
/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/
/*ԴIP��ַ*/
VOS_UINT8                               g_aucIpSrc[IP_IPV4_ADDR_LEN];

/*Ŀ��IP��ַ*/
VOS_UINT8                               g_aucIpDes[IP_IPV4_ADDR_LEN];

extern VOS_UINT8* BSP_GetMacAddr( VOS_VOID );




/*****************************************************************************
  3 Function
*****************************************************************************/

VOS_VOID IP_CalculateIpHeadCheckSum
(
    const LUP_MEM_ST                   *pstIpMsg,
    VOS_UINT16                         *pusCheckSum
)
{
    VOS_UINT8                           ucIpHeadLen = IP_NULL;
    VOS_UINT16                          usTemp      = IP_NULL;
    VOS_UINT8                           ucTemp      = IP_NULL;
    VOS_UINT8                           ucCheckCur  = IP_NULL;
    VOS_UINT32                          ulSum       = IP_NULL;
    VOS_UINT16                          usCheckSum  = IP_NULL;

    ucTemp = *(pstIpMsg->pData);
    ucIpHeadLen = (ucTemp&IP_8_LOW_BYTE)*4;

    /* У�� */
    for (ucCheckCur = 0; ucCheckCur < ucIpHeadLen; ucCheckCur++)
    {
        ucTemp = *(pstIpMsg->pData + ucCheckCur);;
        ucCheckCur++;
        usTemp = ((VOS_UINT16)ucTemp<<IP_BITMOVE_8);
        usTemp = usTemp | *(pstIpMsg->pData + ucCheckCur);
        usTemp = ~usTemp;
        ulSum += usTemp;
    }

    usCheckSum = (VOS_UINT16)ulSum&IP_32_LOW_BYTE ;
    usCheckSum = usCheckSum + (VOS_UINT16)((ulSum & IP_32_HIGH_BYTE) >> IP_BITMOVE_16);
    *pusCheckSum = usCheckSum;
}
VOS_VOID   IP_VerifyUdpData
(
    IP_UDPFAKEHEAD_STRU                *pstUdpFakeHead,
    const VOS_UINT8                    *pucUdp,
    VOS_UINT16                         *pusRet
)
{
    VOS_UINT8                          *pucUdpFakeHead  = (VOS_UINT8 *)pstUdpFakeHead;
    VOS_UINT32                          ulSum           = IP_NULL;
    VOS_UINT16                          usCheckCur      = IP_NULL;
    VOS_UINT16                          usCheckEnd      = IP_NULL;
    VOS_UINT16                          usTemp          = IP_NULL;

    /*�жϼ�����Ƿ�Ϊ0����Ϊ0��˵�����ͷ�û�����ɼ����,��������½��ܷ�Ҳ������֤*/
    if ((pucUdp[6] == 0) && (pucUdp[7] == 0))
    {
        *pusRet = IP_CHECKSUM_SUCCESS;
        return;
    }

    /*16bitΪ��λ,��UDPα�ײ����ж����Ʒ������*/
    usCheckEnd = (IP_UDP_FAKEHEAD_LEN / 2) - 1;
    for (usCheckCur = 0; usCheckCur < usCheckEnd; usCheckCur++)
    {
        usTemp = ((VOS_UINT16)(*pucUdpFakeHead))<<IP_BITMOVE_8;
        pucUdpFakeHead++;
        usTemp = usTemp | *pucUdpFakeHead;

        /*��û���ⲽ,ֱ�Ӻ�usSum��ӵĻ�,�Ὣ~usTemp����32λ����ȡ��*/
        usTemp = ~usTemp;
        ulSum += usTemp;
        pucUdpFakeHead++;
    }
    usTemp = *((VOS_UINT16 *)(VOS_VOID*)pucUdpFakeHead);
    usTemp = ~usTemp;
    ulSum += usTemp;

    /*16bitΪ��λ,������UDP���ݱ����ж����Ʒ������*/
    usCheckEnd = pstUdpFakeHead->usLength / 2;
    for (usCheckCur = 0; usCheckCur < usCheckEnd; usCheckCur++)
    {
        usTemp = ((VOS_UINT16)(*pucUdp))<<IP_BITMOVE_8;
        pucUdp++;
        usTemp = usTemp | *pucUdp;
        usTemp = ~usTemp;
        ulSum += usTemp;
        pucUdp++;
    }

    /*����������ĩβ�����һ���ֽڵ�0�������ֽڲ�������*/
    if ((pstUdpFakeHead->usLength % 2) != 0)
    {
        usTemp = ((VOS_UINT16)(*pucUdp))<<IP_BITMOVE_8;
        usTemp = usTemp | 0;
        usTemp = ~usTemp;
        ulSum += usTemp;
    }

    usTemp = (VOS_UINT16)((ulSum & IP_32_HIGH_BYTE) >> IP_BITMOVE_16);
    *pusRet = usTemp + (VOS_UINT16)(ulSum & IP_32_LOW_BYTE);
}
VOS_UINT32 IP_ValidateIpv4UdpFcs
(
    const LUP_MEM_ST                   *pstUdpMsg,
    const VOS_UINT8                    *pucIpv4Des,
    const VOS_UINT8                    *pucIpv4Src
)
{
    VOS_UINT8                          *pucMsgData      = IP_NULL_PTR;
    VOS_UINT16                          usUdpCheckRet   = IP_NULL;
    IP_UDPFAKEHEAD_STRU                 stUdpFakeHead   = {IP_NULL};
    VOS_UINT16                          usTemp          = IP_NULL;
    VOS_UINT16                          usLength        = IP_NULL;

    pucMsgData = pstUdpMsg->pData;

    /*��ʼ��α�ײ��ṹ�����*/
    PS_MEM_CPY(stUdpFakeHead.aucIPDes, pucIpv4Des, IP_IPV4_ADDR_LEN);
    PS_MEM_CPY(stUdpFakeHead.aucIPSrc, pucIpv4Src, IP_IPV4_ADDR_LEN);
    stUdpFakeHead.ucAll0 = 0;
    stUdpFakeHead.ucProtocol = IP_HEAD_PROTOCOL_UDP;
    usTemp = (VOS_UINT16)(*(pucMsgData + 4)) << IP_BITMOVE_8;
    usLength= usTemp | (VOS_UINT16)(*(pucMsgData + 5));
    stUdpFakeHead.usLength = usLength;

    /*��UDP���ݱ����м���*/
    IP_VerifyUdpData(&stUdpFakeHead, pucMsgData, &usUdpCheckRet);

    if (IP_CHECKSUM_SUCCESS != usUdpCheckRet)
    {
        IP_LOG_ERR("IP_ValidateIpv4UdpFcs: UDP Checksum Error!\n");
        return PS_FAIL;
    }

    return PS_SUCC;
}


VOS_VOID IP_FormIpv4MacHead
(
    VOS_UINT8                          *pucEthHead,
    IP_CAST_TYPE_ENUM_UINT8             enEhernetCastType
)
{
    VOS_UINT16                          usFrmType = IP_GMAC_PAYLOAD_TYPE_IPV4;

    /* �γ�Ŀ��MAC��ַ */
    if (IP_CAST_TYPE_UNICAST == enEhernetCastType)
    {
        IP_MEM_CPY( pucEthHead,
                    IP_DHCPV4SERVER_GetClientMacAddrTmp(),
                    IP_MAC_ADDR_LEN);
    }
    else
    {
        IP_MEM_SET( pucEthHead,
                    (VOS_CHAR)0xFF,
                    IP_MAC_ADDR_LEN);
    }

    /* �γ�ԴMAC��ַ */
    IP_MEM_CPY( (pucEthHead + IP_MAC_ADDR_LEN),
                (VOS_VOID*)BSP_GetMacAddr(),
                IP_MAC_ADDR_LEN);

    /* �γ�MAC����ϲ�Э�� */
    pucEthHead[IP_MAC_ADDR_LEN * 2] = (VOS_UINT8)((usFrmType & IP_16_HIGH_BYTE) >> IP_BITMOVE_8);
    pucEthHead[(IP_MAC_ADDR_LEN * 2) + 1] = (VOS_UINT8)(usFrmType & IP_16_LOW_BYTE);
}
VOS_VOID IP_FormUdpFcs
(
    IP_UDPFAKEHEAD_STRU                *pstUdpFakeHead,
    VOS_UINT8                          *pucUdp
)
{
    VOS_UINT8                          *pucUdpFakeHead  = (VOS_UINT8 *)pstUdpFakeHead;
    VOS_UINT8                          *pucUdpTmp       = pucUdp;
    VOS_UINT32                          ulSum           = IP_NULL;
    VOS_UINT16                          usCheckCur      = IP_NULL;
    VOS_UINT16                          usCheckEnd      = IP_NULL;
    VOS_UINT16                          usTemp          = IP_NULL;

    /* 16bitΪ��λ,��UDPα�ײ����ж����Ʒ������ */
    usCheckEnd = (IP_UDP_FAKEHEAD_LEN / 2) - 1;
    for (usCheckCur = 0; usCheckCur < usCheckEnd; usCheckCur++)
    {
        usTemp = ((VOS_UINT16)(*pucUdpFakeHead))<<IP_BITMOVE_8;
        pucUdpFakeHead++;
        usTemp = usTemp | *pucUdpFakeHead;
        ulSum += usTemp;
        pucUdpFakeHead++;
    }
    usTemp = *((VOS_UINT16 *)(VOS_VOID*)pucUdpFakeHead);
    ulSum += usTemp;

    /*16bitΪ��λ,������UDP���ݱ����ж����Ʒ������*/
    usCheckEnd = pstUdpFakeHead->usLength / 2;
    for (usCheckCur = 0; usCheckCur < usCheckEnd; usCheckCur++)
    {
        usTemp = ((VOS_UINT16)(*pucUdpTmp))<<IP_BITMOVE_8;
        pucUdpTmp++;
        usTemp = usTemp | *pucUdpTmp;
        ulSum += usTemp;
        pucUdpTmp++;
    }

    /*����������ĩβ�����һ���ֽڵ�0�������ֽڲ�������*/
    if ((pstUdpFakeHead->usLength % 2) != 0)
    {
        usTemp = ((VOS_UINT16)(*pucUdpTmp))<<IP_BITMOVE_8;
        usTemp = usTemp | 0;
        ulSum += usTemp;
    }

    usTemp = (VOS_UINT16)((ulSum & IP_32_HIGH_BYTE) >> IP_BITMOVE_16);
    usTemp += (VOS_UINT16)(ulSum & IP_32_LOW_BYTE);
    usTemp = ~usTemp;

    /*�����ɵļ����Ϊ0������Ҫ����ֵ��Ϊ0xFFFF,��Ϊ����Ϊ0,���շ�����Ϊ���ͷ�û�����ɼ����*/
    if (usTemp == 0)
    {
        usTemp = 0xFFFF;
    }

    /* д����� */
    *(pucUdp+6) = (VOS_UINT8)((usTemp & IP_16_HIGH_BYTE)>>IP_BITMOVE_8);
    *(pucUdp+7) = (VOS_UINT8)(usTemp & IP_16_LOW_BYTE);
}
VOS_VOID IP_EncodeIpHead
(
    const IP_IPV4HEAD_STRU             *pstIpv4Head,
    VOS_UINT8                          *pucIpv4Head
)
{
    VOS_UINT8                           ucTemp = IP_NULL;
    VOS_UINT16                          usTemp = IP_NULL;


    /* �γ�IPv4��ͷ���� */
    ucTemp = ((VOS_UINT8)pstIpv4Head->bitVersion)<<IP_BITMOVE_4;
    pucIpv4Head[0] = ucTemp | (VOS_UINT8)pstIpv4Head->bitHeadLength;

    ucTemp = ((VOS_UINT8)pstIpv4Head->bitPriority)<<5;
    ucTemp |= ((VOS_UINT8)pstIpv4Head->bitD)<<4;
    ucTemp |= ((VOS_UINT8)pstIpv4Head->bitT)<<3;
    ucTemp |= ((VOS_UINT8)pstIpv4Head->bitR)<<2;
    ucTemp |= ((VOS_UINT8)pstIpv4Head->bitC)<<1;
    pucIpv4Head[1] = ucTemp | (VOS_UINT8)pstIpv4Head->bitReserved;

    pucIpv4Head[2] = (VOS_UINT8)((pstIpv4Head->usLength & IP_16_HIGH_BYTE)>>IP_BITMOVE_8);
    pucIpv4Head[3] = (VOS_UINT8)(pstIpv4Head->usLength & IP_16_LOW_BYTE);
    pucIpv4Head[4] = (VOS_UINT8)((pstIpv4Head->usIdentification & IP_16_HIGH_BYTE)>>IP_BITMOVE_8);
    pucIpv4Head[5] = (VOS_UINT8)(pstIpv4Head->usIdentification & IP_16_LOW_BYTE);

    usTemp = ((VOS_UINT16)pstIpv4Head->bitFlagMF)<<15;
    usTemp |= ((VOS_UINT16)pstIpv4Head->bitFlagDF)<<14;
    usTemp |= ((VOS_UINT16)pstIpv4Head->bitFlagReserved)<<13;
    usTemp |= (VOS_UINT16)pstIpv4Head->bitFlagmentExcursion;
    pucIpv4Head[6] = (VOS_UINT8)((usTemp & IP_16_HIGH_BYTE)>>IP_BITMOVE_8);
    pucIpv4Head[7] = (VOS_UINT8)(usTemp & IP_16_LOW_BYTE);

    pucIpv4Head[8] = pstIpv4Head->ucTTL;
    pucIpv4Head[9] = pstIpv4Head->ucProtocol;

    usTemp = pstIpv4Head->usFCS;
    pucIpv4Head[10] = (VOS_UINT8)((usTemp & IP_16_HIGH_BYTE)>>IP_BITMOVE_8);
    pucIpv4Head[11] = (VOS_UINT8)(usTemp & IP_16_LOW_BYTE);

    pucIpv4Head[12] = pstIpv4Head->aucIPSrc[0];
    pucIpv4Head[13] = pstIpv4Head->aucIPSrc[1];
    pucIpv4Head[14] = pstIpv4Head->aucIPSrc[2];
    pucIpv4Head[15] = pstIpv4Head->aucIPSrc[3];

    pucIpv4Head[16] = pstIpv4Head->aucIPDes[0];
    pucIpv4Head[17] = pstIpv4Head->aucIPDes[1];
    pucIpv4Head[18] = pstIpv4Head->aucIPDes[2];
    pucIpv4Head[19] = pstIpv4Head->aucIPDes[3];
}

/*****************************************************************************
 Function Name   : IP_IsValidIpv4UdpMsg
 Description     : �ж���Ϣ�Ƿ�Ϸ���UDP��Ϣ
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2010-11-23  Draft Enact

*****************************************************************************/
VOS_UINT32 IP_IsValidIpv4UdpMsg
(
    const LUP_MEM_ST                   *pstIpMsg
)
{
    VOS_UINT8                           ucProtocol      = IP_NULL;
    VOS_UINT16                          usCheckSum      = IP_NULL;
    VOS_UINT16                          usPLen          = IP_NULL;
    LUP_MEM_ST                          stTtfMem        = {IP_NULL};
    VOS_UINT8                           ucIpHeadLen     = IP_NULL;
    VOS_UINT8                           ucIpVersion     = IP_NULL;

    /* ���ȺϷ���� */
    if (pstIpMsg->usUsed < IP_IPV4_HEAD_LEN)
    {
        IP_LOG_ERR("IP_DHCPV4CLIENT_IsValidDhcpv4Msg: the ipv4 msg length is invalid!\n");
        return PS_FAIL;
    }

    /* ��ȡIP�汾�� */
    ucIpVersion = IP_GetIpVersion(pstIpMsg->pData);

    if (IP_VERSION_4 != ucIpVersion)
    {
        IP_LOG_ERR("IP_DHCPV4CLIENT_IsValidDhcpv4Msg: ip version is not v4!\n");
        return PS_FAIL;
    }

    /* ȡ��Э�� */
    ucProtocol = IP_GetProtocol(pstIpMsg->pData);

    if (IP_HEAD_PROTOCOL_UDP != ucProtocol)
    {
        IP_LOG_ERR("IP_DHCPV4CLIENT_IsValidDhcpv4Msg: ip head protocol is not udp!\n");
        return PS_FAIL;
    }

    /* ����IPͷ�ļ���� */
    IP_CalculateIpHeadCheckSum(pstIpMsg, &usCheckSum);

    /* ��֤������Ƿ���ȷ */
    if (IP_CHECKSUM_SUCCESS != usCheckSum)
    {
        IP_LOG_ERR("IP_DHCPV4CLIENT_IsValidDhcpv4Msg:IP checksum error!\n");
        return PS_FAIL;
    }

    IP_SET_IPV4SRC(pstIpMsg->pData + IP_HEAD_SRC_IP_OFF);                                     /* ����ȫ��ԴIP��ַ */
    IP_SET_IPV4DES(pstIpMsg->pData + IP_HEAD_DESC_IP_OFF);

    /* ȡ��ipͷ���� */
    ucIpHeadLen = IP_GetIpv4HeaderLen(pstIpMsg->pData);

    /* ȡ��ip���ݱ����� */
    IP_GetIpv4PacketLen(pstIpMsg->pData, usPLen);

    /* �ж�IPͷ��ָʾ�����ݰ�������L2ָʾ�����ݰ������Ƿ�һ�� */
    if (usPLen != pstIpMsg->usUsed)
    {
        IP_LOG_ERR("IP_DHCPV4CLIENT_IsValidDhcpv4Msg:ip packet len mismatch!\n");
        return PS_FAIL;
    }

    stTtfMem.usUsed = usPLen - ucIpHeadLen;
    stTtfMem.pData  = pstIpMsg->pData + ucIpHeadLen;

    /* ��֤UDP����� */
    if (PS_SUCC != IP_ValidateIpv4UdpFcs(   &stTtfMem,
                                            IP_GET_IPV4DES(),
                                            IP_GET_IPV4SRC()))
    {
        IP_LOG_ERR("IP_DHCPV4CLIENT_IsValidDhcpv4Msg:IP_ValidateIpv4UdpFcs failed!\n");
        return PS_FAIL;
    }

    return PS_SUCC;
}

/*****************************************************************************
 Function Name   : IP_GetDhcpv4MsgOptionItem
 Description     : ��ȡDHCPV4��Ϣ��ѡ��
 Input           : pstDhcpv4Msg------------DHCPv4��Ϣָ��
                   enOptionType------------��ѡ������
 Output          : pucOptionItem-----------��ѡ�������׵�ַ��ָ��
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2010-12-20  Draft Enact

*****************************************************************************/
VOS_UINT32  IP_GetDhcpv4MsgOptionItem
(
    const LUP_MEM_ST                   *pstDhcpMsg,
    IP_DHCPV4_OPTION_TYPE_ENUM_UINT8    enOptionType,
    VOS_UINT8                         **pucOptionItem
)
{
    VOS_UINT8                          *pucDhcpOption   = IP_NULL_PTR;
    VOS_UINT8                           ucCodeLen       = IP_NULL;
    VOS_UINT32                          ulTempCur       = IP_NULL;
    VOS_UINT32                          ulDhcpOptionLen = IP_NULL;
    VOS_UINT8                           ucFlag          = IP_NULL;

    /* �õ�DHCP options ����ʼλ�� */
    pucDhcpOption = pstDhcpMsg->pData + IP_DHCPV4_OPTION_OFFSET;

    /* ���1��Ϊ��ȥ����OPTION���ֵĽ�β�� */
    ulDhcpOptionLen = pstDhcpMsg->usUsed - (IP_DHCPV4_OPTION_OFFSET + 1);

    /* ��ѯ��ѡ�� */
    for (ulTempCur = 0; ulTempCur < ulDhcpOptionLen;)
    {
        if (enOptionType == *pucDhcpOption)
        {
            ucFlag = PS_TRUE;
            break;
        }

        ucCodeLen = *(pucDhcpOption + IP_DHCPV4_OPTION_CODE_LEN);
        pucDhcpOption += ucCodeLen + IP_DHCPV4_OPTION_CODELEN_LEN;
        ulTempCur += ucCodeLen + IP_DHCPV4_OPTION_CODELEN_LEN;
    }

    if (PS_TRUE != ucFlag)
    {
        IP_LOG_ERR("IP_GetDhcpv4MsgOptionItem:can not find mst type!\n");
        return PS_FAIL;
    }

    *pucOptionItem = (pucDhcpOption + IP_DHCPV4_OPTION_CODELEN_LEN);

    return PS_SUCC;
}
#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif



