

/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include  "NasERabmPublic.h"
#include  "NasERabmIpFilter.h"
/*#include  "DrvInterface.h"*/
#include  "LUPAppItf.h"
/*#include  "NasERabmPdcpMsgProc.h"*/
#include  "NasERabmIpfOm.h"
#include  "NasIpInterface.h"
#include  "NasERabmAppMsgProc.h"
/*#include  "NasERabmIpMsgProc.h"*/
#include  "NasEsmPublic.h"
#include  "NasERabmMain.h"
/*#include  "DataDispatch.h"*/
#include  "CdsInterface.h"


/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASRABMIPFILTER_C
/*lint +e767*/


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/
/* IPF��������������Ϣ */
/*lint -e528*/
/*STATIC IPF_COMMON_PARA_S g_stNasERabmIpfConfigInfo = {0};*/

/*lint +e528*/
/* RABM IPF��Filter����, ����Ŀǰʹ�õ�����Filter */
NAS_ERABM_IPF_FILTER_ARRAY g_stNasERabmIpfFilterArray = {0};

#if 0
NAS_ERABM_IPF_DL_FILTER_INFO g_astNasERabmDlUserFilter[NAS_ERABM_IPF_MAX_DL_FILTER];

/*����IP Filter ID��Filter���ͣ�Filter ID�� Filter��ӦBID�� Filter����*/
STATIC VOS_UINT32 g_aulNasERabmIpfFilterTypeTable[][3] =
{
    {NAS_ERABM_IPF_DL_FITLER_DHCP4,    NAS_ERABM_IPF_DHCP4, NAS_IP_FILTER_TYPE_DHCPV4},
    {NAS_ERABM_IPF_DL_FITLER_DHCP6,    NAS_ERABM_IPF_DHCP6, NAS_IP_FILTER_TYPE_BUTT},
    {NAS_ERABM_IPF_DL_FITLER_RS,       NAS_ERABM_IPF_ICMP6, NAS_IP_FILTER_TYPE_ICMPV6},
    {NAS_ERABM_IPF_DL_FITLER_RA,       NAS_ERABM_IPF_ICMP6, NAS_IP_FILTER_TYPE_ICMPV6},
    {NAS_ERABM_IPF_DL_FITLER_NS,       NAS_ERABM_IPF_ICMP6, NAS_IP_FILTER_TYPE_ICMPV6},
    {NAS_ERABM_IPF_DL_FITLER_NA,       NAS_ERABM_IPF_ICMP6, NAS_IP_FILTER_TYPE_ICMPV6},
    {NAS_ERABM_IPF_DL_FITLER_REDIRECT, NAS_ERABM_IPF_ICMP6, NAS_IP_FILTER_TYPE_ICMPV6}
};
#endif

NAS_ERABM_UL_NDIS_FILTER_FLAG_ENUM_UINT32 g_enUlNdisFilterValidFlag = NAS_ERABM_UL_NDIS_FILTER_INVALID;

NAS_ERABM_IPF_UL_NDIS_FILTER_INFO g_astNasERabmUlNdisFilter[NAS_ERABM_IPF_MAX_UL_NDIS_FILTER];

/* ����NDIS Filter ID��Filter��ӦBID�İ󶨹�ϵ */
STATIC VOS_UINT32 g_aulNasERabmUlNdisFilterTypeTable[][2] =
{
    {NAS_ERABM_IPF_UL_NDIS_FILTER_DHCP4,    CDS_UL_IPF_BEARER_ID_DHCPV4},
    {NAS_ERABM_IPF_UL_NDIS_FILTER_DHCP6,    CDS_UL_IPF_BEARER_ID_DHCPV6},
    {NAS_ERABM_IPF_UL_NDIS_FILTER_RS,       CDS_UL_IPF_BEARER_ID_ICMPV6},
    {NAS_ERABM_IPF_UL_NDIS_FILTER_RA,       CDS_UL_IPF_BEARER_ID_ICMPV6},
    {NAS_ERABM_IPF_UL_NDIS_FILTER_NS,       CDS_UL_IPF_BEARER_ID_ICMPV6},
    {NAS_ERABM_IPF_UL_NDIS_FILTER_NA,       CDS_UL_IPF_BEARER_ID_ICMPV6},
    {NAS_ERABM_IPF_UL_NDIS_FILTER_REDIRECT, CDS_UL_IPF_BEARER_ID_ICMPV6},
    {NAS_ERABM_IPF_UL_NDIS_FILTER_LOC_PING6,CDS_UL_IPF_BEARER_ID_ICMPV6},
    {NAS_ERABM_IPF_UL_NDIS_FILTER_LOC_FE80, CDS_UL_IPF_BEARER_ID_LL_FE80},
    {NAS_ERABM_IPF_UL_NDIS_FILTER_LOC_FF,   CDS_UL_IPF_BEARER_ID_LL_FF}
};
/*****************************************************************************
  3 Function
*****************************************************************************/
#if 0

VOS_VOID NAS_ERABM_IpfDefDlFilter()
{
    NAS_ERABM_IPF_DL_FILTER_INFO *pstFilter;

    /*��ӡ����ú���*/
    NAS_ERABM_INFO_LOG("NAS_ERABM_IpfDefDlFilter is entered.");

    /* ���й��֧��8��Filter�� Ŀǰʹ����7����������ӳ���8������Ҫ�޸�
      ��Ӧ�궨�壬ע�ⲻҪ��� */

    /* ����DHCPv4 ����Filter */
    NAS_ERABM_IPF_DEF_DHCPV4_FILTER();

    /* ����DHCPv6���͵�����Filter */
    NAS_ERABM_IPF_DEF_DHCPV6_FILTER();

    /* ��������ICMP Router Solicitation Filter */
    NAS_ERABM_IPF_DEF_ICMP_RS_FILTER();

    /* ��������ICMP Router Advertisement Filter */
    NAS_ERABM_IPF_DEF_ICMP_RA_FILTER();

    /* ��������ICMP Neighbor Solicitation Filter */
    NAS_ERABM_IPF_DEF_ICMP_NS_FILTER();

    /* ��������ICMP Neighbor Advertisement Filter */
    NAS_ERABM_IPF_DEF_ICMP_NA_FILTER();

    /* ��������ICMP Redirect Filter */
    NAS_ERABM_IPF_DEF_ICMP_REDIRECT_FILTER();
}
#endif
/*lint -e960*/

VOS_VOID NAS_ERABM_IpfUlNdisFilterInit( VOS_VOID )
{
    NAS_ERABM_IPF_UL_NDIS_FILTER_INFO *pstFilter;

    /*��ӡ����ú���*/
    NAS_ERABM_INFO_LOG("NAS_ERABM_IpfUlNdisFilterInit is entered.");

    /* ���й��֧��12��Filter�� Ŀǰʹ����10����������ӳ���12������Ҫ�޸�
      ��Ӧ�궨�壬ע�ⲻҪ��� */

    /* ��������NDIS DHCPv4 ����Filter */
    NAS_ERABM_IPF_DEF_UL_NDIS_DHCPV4_FILTER();

    /* ��������NDIS DHCPv6���͵�����Filter */
    NAS_ERABM_IPF_DEF_UL_NDIS_DHCPV6_FILTER();

    /* ��������NDIS ICMP Router Solicitation Filter */
    NAS_ERABM_IPF_DEF_UL_NDIS_ICMP_RS_FILTER();

    /* ��������NDIS ICMP Router Advertisement Filter */
    NAS_ERABM_IPF_DEF_UL_NDIS_ICMP_RA_FILTER();

    /* ��������NDIS ICMP Neighbor Solicitation Filter */
    NAS_ERABM_IPF_DEF_UL_NDIS_ICMP_NS_FILTER();

    /* ��������NDIS ICMP Neighbor Advertisement Filter */
    NAS_ERABM_IPF_DEF_UL_NDIS_ICMP_NA_FILTER();

    /* ��������NDIS ICMP Redirect Filter */
    NAS_ERABM_IPF_DEF_UL_NDIS_ICMP_REDIRECT_FILTER();

    /* ��������NDIS ICMP ECHO REQUEST Filter */
    NAS_ERABM_IPF_DEF_UL_NDIS_LOC_PING6_FILTER();

    /* ��������NDIS Ŀ�ĵ�ַFE80��ͷ���� Filter */
    NAS_ERABM_IPF_DEF_UL_NDIS_LOC_FE80_FILTER();

    /* ��������NDIS Ŀ�ĵ�ַFF��ͷ���� Filter */
    NAS_ERABM_IPF_DEF_UL_NDIS_LOC_FF_FILTER();
}


IPF_FILTER_CONFIG_S * NAS_ERABM_IpfGetFreeFilter(NAS_ERABM_IPF_DIRECTION_UINT32 enDir)
{
    /* ��ȡ����Filter */
    if((NAS_ERABM_IPF_DIRECTION_UL == enDir)&&(!NAS_ERABM_IPF_IS_UL_FILTER_FULL()))
    {
        return NAS_ERABM_IPF_GET_UL_FILTER_PTR(NAS_ERABM_IPF_GET_UL_FILTER_NUM());
    }

    /* ��ȡ����Filter */
    if((NAS_ERABM_IPF_DIRECTION_DL == enDir)&&(!NAS_ERABM_IPF_IS_DL_FILTER_FULL()))
    {
        return NAS_ERABM_IPF_GET_DL_FILTER_PTR(NAS_ERABM_IPF_GET_DL_FILTER_NUM());
    }

    NAS_ERABM_ERR_LOG("NAS_ERABM_IpfGetFreeFilter, Filter Queue Full.");

    /* ����ͳ����Ϣ */
    NAS_ERABM_IPF_OM_ADD_FILTER_FULL_CNT();

    return VOS_NULL_PTR;
}
/*lint +e960*/
#if 0

VOS_UINT32  NAS_ERABM_IpfCommonConfig(VOS_VOID)
{
    IPF_COMMON_PARA_S *pstIpfConfig = NAS_ERABM_IPF_CONFIG_INFO_PTR();

    /*��ӡ����ú���*/
    NAS_ERABM_INFO_LOG("NAS_ERABM_IpfCommonConfig is entered.");

    PS_MEM_SET(pstIpfConfig, 0, sizeof(IPF_COMMON_PARA_S));

    /* ����ΪLTEģʽĬ�ϲ���������ο�IPF���� */
    pstIpfConfig->bEspSpiEnable = PS_FALSE;  /*esp����ƥ��*/
    pstIpfConfig->bAhSpiEnable = PS_FALSE;   /*ah����ƥ��*/
    pstIpfConfig->bEspAhSel = PS_FALSE;      /*esp����*/
    pstIpfConfig->bIpv6NextHdSel = PS_TRUE;  /*����ͷ�е�NH*/
    pstIpfConfig->eMaxBurst = IPF_BURST_16;
    pstIpfConfig->bSpWrrModeSel = PS_TRUE;
    pstIpfConfig->bSpPriSel = PS_TRUE;
    pstIpfConfig->bFltAddrReverse = PS_FALSE;
    pstIpfConfig->bFilterSeq = PS_TRUE;
    pstIpfConfig->u32WrrValue[0] = 2048;
    pstIpfConfig->u32WrrValue[1] = 2048;
    pstIpfConfig->stChlCtrl[0].bDataChain = PS_FALSE;
    pstIpfConfig->stChlCtrl[0].bEndian = PS_FALSE;
    pstIpfConfig->stChlCtrl[0].eIpfMode = IPF_MODE_FILTERANDTRANS;
    pstIpfConfig->stChlCtrl[1].bDataChain = PS_TRUE;
    pstIpfConfig->stChlCtrl[1].bEndian = PS_FALSE;
    pstIpfConfig->stChlCtrl[1].eIpfMode = IPF_MODE_FILTERANDTRANS;

    /* ���������ӿ�����IPF���� */
    if(PS_SUCC != BSP_IPF_Init(pstIpfConfig))
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IpfCommonConfig: BSP Init IPF Fail.");
        return PS_FAIL;
    }

    return PS_SUCC;
}
#endif
/*lint -e960*/
/*lint -e961*/
VOS_INT32 NAS_ERABM_IpfCompare(const VOS_VOID *arg1, const VOS_VOID *arg2)
{
    VOS_UINT16              usVar1;
    VOS_UINT16              usVar2;
    IPF_FILTER_CONFIG_S*    pstPara1;
    IPF_FILTER_CONFIG_S*    pstPara2;

    pstPara1 = (IPF_FILTER_CONFIG_S*) arg1;
    pstPara2 = (IPF_FILTER_CONFIG_S*) arg2;

    usVar1 = pstPara1->stMatchInfo.unFltChain.Bits.u16FltPri;
    usVar2 = pstPara2->stMatchInfo.unFltChain.Bits.u16FltPri;

    /* IP Filter�����ȼ���С�������� */
    if(usVar1 > usVar2)
    {
        return 1;
    }
    else if(usVar1 < usVar2)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
VOS_UINT32 NAS_ERABM_IpfGenerateBaseIpv4Filter
(
    const APP_ESM_IP_ADDR_STRU *pstPdnAddr,
    VOS_UINT8 ucEpsbId
)
{
    VOS_UINT32                      i           = 0;
    IPF_FILTER_CONFIG_S            *pstFilter   = VOS_NULL_PTR;

    /* ��Filter�������ҵ���һ�����е�Filter */
    pstFilter = NAS_ERABM_IpfGetFreeFilter(NAS_ERABM_IPF_DIRECTION_UL);
    if(VOS_NULL_PTR == pstFilter)
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IPF Get Free Filter Failed.");
        return PS_FAIL;
    }

    NAS_ERABM_MEM_SET(pstFilter, 0x0, sizeof(IPF_FILTER_CONFIG_S));

    /* ��дIP Filter���� */
    pstFilter->stMatchInfo.unFltRuleCtrl.Bits.FltType = NAS_ERABM_IPF_IPTYPE_IPV4;
    pstFilter->stMatchInfo.unFltRuleCtrl.Bits.FltLAddrEn = NAS_ERABM_OP_TRUE;

    for(i = 0; i < APP_MAX_IPV4_ADDR_LEN; i++)
    {
        pstFilter->stMatchInfo.u8SrcAddr[i] = pstPdnAddr->aucIpV4Addr[i];
        /*pstFilter->stMatchInfo.u8SrcMsk[i] = 0xFF;*/
    }
    /*lint -e40*/
    /*lint -e63*/
    pstFilter->stMatchInfo.u32LocalAddressMsk = 0;
    /*lint +e63*/
    /*lint +e40*/

    /* Precedence = 256 */
    pstFilter->stMatchInfo.unFltRuleCtrl.Bits.FltBid = ucEpsbId;
    pstFilter->stMatchInfo.unFltRuleCtrl.Bits.FltEn  = NAS_ERABM_OP_TRUE;
    pstFilter->stMatchInfo.unFltChain.Bits.u16FltPri = NAS_ERABM_IPF_LOWEST_PRI;

    /* Bearer Id */
    pstFilter->u32FilterID = NAS_ERABM_IPF_UL_IPV4_FILTER_ID(ucEpsbId, 0);

    /* ��������Filter��Ŀ */
    NAS_ERABM_IPF_ADD_UL_FILTER_NUM();

    return PS_SUCC;
}


VOS_UINT32 NAS_ERABM_IpfGenerateBaseIpv6Filter
(
    const APP_ESM_IP_ADDR_STRU *pstPdnAddr,
    VOS_UINT8 ucEpsbId
)
{
    VOS_UINT32                      i           = 0;
    IPF_FILTER_CONFIG_S            *pstFilter   = VOS_NULL_PTR;
    VOS_UINT8                       aucInvalidIpv6Prefix[NAS_ERABM_IPV6_PREFIX_LEN] = {0};

    /* ���IPv6ǰ׺�Ƿ���Ч�������Ч��������IPF��ֱ�ӷ��� */
    if(0 == NAS_ERABM_MEM_CMP(  pstPdnAddr->aucIpV6Addr,
                                aucInvalidIpv6Prefix,
                                NAS_ERABM_IPV6_PREFIX_LEN))
    {
        NAS_ERABM_NORM_LOG("NAS_ERABM_IpfGenerateBaseIpv6Filter IPv6 prefix invalid.");
        return PS_SUCC;
    }
    /* ��Filter�������ҵ���һ�����е�Filter */
    pstFilter = NAS_ERABM_IpfGetFreeFilter(NAS_ERABM_IPF_DIRECTION_UL);
    if(VOS_NULL_PTR == pstFilter)
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IpfGenerateBaseIpv6Filter Get Free Filter Failed.");
        return PS_FAIL;
    }

    NAS_ERABM_MEM_SET(pstFilter, 0x0, sizeof(IPF_FILTER_CONFIG_S));

    /* ��дIP Filter���� */
    pstFilter->stMatchInfo.unFltRuleCtrl.Bits.FltType = NAS_ERABM_IPF_IPTYPE_IPV6;
    pstFilter->stMatchInfo.unFltRuleCtrl.Bits.FltLAddrEn = NAS_ERABM_OP_TRUE;

    /* IPv6ֻ����64bitǰ׺ */
    for(i = 0; i < (APP_MAX_IPV6_ADDR_LEN/2); i++)
    {
        /*lint -e40*/
        /*lint -e63*/
        pstFilter->stMatchInfo.u8SrcAddr[i] = pstPdnAddr->aucIpV6Addr[i];
        /*lint +e63*/
        /*lint +e40*/
      /* pstFilter->stMatchInfo.u8SrcMsk[i] = 0xFF; */
    }
    /*lint -e40*/
    /*lint -e63*/
    pstFilter->stMatchInfo.u32LocalAddressMsk = 64;
    /*lint +e63*/
    /*lint +e40*/

    /* Precedence = 256 */
    pstFilter->stMatchInfo.unFltRuleCtrl.Bits.FltBid = ucEpsbId;
    pstFilter->stMatchInfo.unFltRuleCtrl.Bits.FltEn = NAS_ERABM_OP_TRUE;
    pstFilter->stMatchInfo.unFltChain.Bits.u16FltPri = NAS_ERABM_IPF_LOWEST_PRI;

    /* Bearer Id */
    pstFilter->u32FilterID = NAS_ERABM_IPF_UL_IPV6_FILTER_ID(ucEpsbId,0);

    /* ��������Filter��Ŀ */
    NAS_ERABM_IPF_ADD_UL_FILTER_NUM();

    return PS_SUCC;
}
VOS_UINT32 NAS_ERABM_IpfGenerateFilterWithoutTFT
(
    const APP_ESM_IP_ADDR_STRU *pstPdnAddr,
    VOS_UINT8 ucEpsbId
)
{
    /* check ip type: ipv4, ipv4v6 */
    if((APP_ESM_PDN_TYPE_IPV4 == pstPdnAddr->ucIpType)||
       (APP_ESM_PDN_TYPE_IPV4_IPV6 == pstPdnAddr->ucIpType))
    {
        if(PS_FAIL == NAS_ERABM_IpfGenerateBaseIpv4Filter(pstPdnAddr, ucEpsbId))
        {
            NAS_ERABM_ERR_LOG("NAS_ERABM_IpfGenerateFilterWithoutTFT: Generate Ipv4 Filter Fail.");
            return PS_FAIL;
        }
    }

    /* check ip type: ipv6, ipv4v6 */
    if((APP_ESM_PDN_TYPE_IPV6 == pstPdnAddr->ucIpType)||
       (APP_ESM_PDN_TYPE_IPV4_IPV6 == pstPdnAddr->ucIpType))
    {
        if(PS_FAIL ==  NAS_ERABM_IpfGenerateBaseIpv6Filter(pstPdnAddr, ucEpsbId))
        {
            NAS_ERABM_ERR_LOG("NAS_ERABM_IpfGenerateFilterWithoutTFT: Generate Ipv6 Filter Fail.");
            return PS_FAIL;
        }
    }

    return PS_SUCC;
}


VOS_VOID NAS_ERABM_IpfConvertOtherPartToFilter
(
    const ESM_ERABM_TFT_PF_STRU *pstTftPf,
    IPF_MATCH_INFO_S     *pstIpFilter
)
{
    /* Protocol id */
    if(NAS_ERABM_OP_TRUE == pstTftPf->bitOpProtocolId)
    {
        pstIpFilter->unNextHeader.u32Protocol = pstTftPf->ucProtocolId;

        pstIpFilter->unFltRuleCtrl.Bits.FltNhEn = NAS_ERABM_OP_TRUE;
    }

    /* local port */
    if(NAS_ERABM_OP_TRUE == pstTftPf->bitOpSingleLocalPort)
    {
        pstIpFilter->unSrcPort.Bits.u16SrcPortLo = pstTftPf->usSingleLcPort;
        pstIpFilter->unSrcPort.Bits.u16SrcPortHi = pstTftPf->usSingleLcPort;

        pstIpFilter->unFltRuleCtrl.Bits.FltLPortEn = NAS_ERABM_OP_TRUE;
    }

    /* local port range, low to high */
    if(NAS_ERABM_OP_TRUE == pstTftPf->bitOpLocalPortRange)
    {
        pstIpFilter->unSrcPort.Bits.u16SrcPortLo = pstTftPf->usLcPortLowLimit;
        pstIpFilter->unSrcPort.Bits.u16SrcPortHi = pstTftPf->usLcPortHighLimit;

        pstIpFilter->unFltRuleCtrl.Bits.FltLPortEn = NAS_ERABM_OP_TRUE;
    }

    /* remote port */
    if(NAS_ERABM_OP_TRUE == pstTftPf->bitOpSingleRemotePort)
    {
        pstIpFilter->unDstPort.Bits.u16DstPortLo = pstTftPf->usSingleRmtPort;
        pstIpFilter->unDstPort.Bits.u16DstPortHi = pstTftPf->usSingleRmtPort;

        pstIpFilter->unFltRuleCtrl.Bits.FltRPortEn = NAS_ERABM_OP_TRUE;
    }

    /* remote port range , low to high */
    if(NAS_ERABM_OP_TRUE == pstTftPf->bitOpRemotePortRange)
    {
        pstIpFilter->unDstPort.Bits.u16DstPortLo = pstTftPf->usRmtPortLowLimit;
        pstIpFilter->unDstPort.Bits.u16DstPortHi = pstTftPf->usRmtPortHighLimit;

        pstIpFilter->unFltRuleCtrl.Bits.FltRPortEn = NAS_ERABM_OP_TRUE;
    }

    /* type of server and mask for ipv4, traffic class and mask for ipv6*/
    if(NAS_ERABM_OP_TRUE == pstTftPf->bitOpTypeOfService)
    {
      /*pstIpFilter->unTrafficClass.u32ServiceType = pstTftPf->ucTypeOfService;
        pstIpFilter->unTrafficClassMsk.u32ServiceTypeMsk = pstTftPf->ucTypeOfServiceMask;*/
        /*lint -e40*/
        /*lint -e10*/
        /*lint -e63*/
        pstIpFilter->unTrafficClass.Bits.u8TrafficClass     = pstTftPf->ucTypeOfService;
        pstIpFilter->unTrafficClass.Bits.u8TrafficClassMask = pstTftPf->ucTypeOfServiceMask;
        /*lint +e63*/
        /*lint +e10*/
        /*lint +e40*/
        pstIpFilter->unFltRuleCtrl.Bits.FltTosEn = NAS_ERABM_OP_TRUE;
    }

    /* spi for esp or ah ? */
    if(NAS_ERABM_OP_TRUE == pstTftPf->bitOpSecuParaIndex)
    {
        pstIpFilter->u32FltSpi = pstTftPf->ulSecuParaIndex;

        pstIpFilter->unFltRuleCtrl.Bits.FltSpiEn = NAS_ERABM_OP_TRUE;
    }

    return;
}
VOS_UINT32 NAS_ERABM_IpfConvertIpv4TftToFilter
(
    const APP_ESM_IP_ADDR_STRU *pstPdnAddr,
    const ESM_ERABM_TFT_PF_STRU *pstTftPf,
    VOS_UINT8            ucEpsbId
)
{
    VOS_UINT32                  i               = 0;
    IPF_FILTER_CONFIG_S        *pstIpFilter     = VOS_NULL_PTR;
    IPF_MATCH_INFO_S           *pstFilter       = VOS_NULL_PTR;

    /* ��Filter�������ҵ���һ�����е�Filter */
    pstIpFilter = NAS_ERABM_IpfGetFreeFilter(NAS_ERABM_IPF_DIRECTION_UL);

    if(VOS_NULL_PTR == pstIpFilter)
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IPF Get Free Filter Failed.");
        return PS_FAIL;
    }

    NAS_ERABM_MEM_SET(pstIpFilter, 0x0, sizeof(IPF_FILTER_CONFIG_S));

    pstFilter = &pstIpFilter->stMatchInfo;

    /* source address and mask for ipv4 */
    for(i = 0;i < APP_MAX_IPV4_ADDR_LEN; i++)
    {
        pstFilter->u8SrcAddr[i] = pstPdnAddr->aucIpV4Addr[i];
     /* pstFilter->u8SrcMsk[i]  = 0xFF; */
    }
    /*lint -e40*/
    /*lint -e63*/
    pstFilter->u32LocalAddressMsk = 0;
    pstFilter->unFltRuleCtrl.Bits.FltLAddrEn = NAS_ERABM_OP_TRUE;
    /*lint +e63*/
    /*lint +e40*/

    /* destination address and mask for ipv4 */
    if(NAS_ERABM_OP_TRUE == pstTftPf->bitOpRmtIpv4AddrAndMask)
    {
        for(i = 0;i < APP_MAX_IPV4_ADDR_LEN; i++)
        {
            pstFilter->u8DstAddr[i] = pstTftPf->aucRmtIpv4Address[i];
            pstFilter->u8DstMsk[i] = pstTftPf->aucRmtIpv4Mask[i];
        }
        pstFilter->unFltRuleCtrl.Bits.FltRAddrEn = NAS_ERABM_OP_TRUE;
    }

    pstFilter->unFltRuleCtrl.Bits.FltType = NAS_ERABM_IPF_IPTYPE_IPV4;

    /* ��TFT�����е����ಿ��ת��ΪIP Filter��ʽ */
    NAS_ERABM_IpfConvertOtherPartToFilter(pstTftPf, &pstIpFilter->stMatchInfo);

    /* Filter ID */
    pstIpFilter->u32FilterID =
        NAS_ERABM_IPF_UL_IPV4_FILTER_ID(ucEpsbId, pstTftPf->ucNwPacketFilterId);
    /* Filter Precedence and Bearer Id */
    pstFilter->unFltChain.Bits.u16FltPri = pstTftPf->ucPrecedence;
    pstFilter->unFltRuleCtrl.Bits.FltBid = ucEpsbId;
    pstFilter->unFltRuleCtrl.Bits.FltEn = NAS_ERABM_OP_TRUE;

    /* ��������Filter��Ŀ */
    NAS_ERABM_IPF_ADD_UL_FILTER_NUM();

    return PS_SUCC;
}


VOS_UINT32 NAS_ERABM_IpfConvertIpv6TftToFilter
(
    const APP_ESM_IP_ADDR_STRU *pstPdnAddr,
    const ESM_ERABM_TFT_PF_STRU *pstTftPf,
    VOS_UINT8            ucEpsbId
)
{
    VOS_UINT32                  i               = 0;
    IPF_FILTER_CONFIG_S        *pstIpFilter     = VOS_NULL_PTR;
    IPF_MATCH_INFO_S           *pstFilter       = VOS_NULL_PTR;

    /* ��Filter�������ҵ���һ�����е�Filter */
    pstIpFilter = NAS_ERABM_IpfGetFreeFilter(NAS_ERABM_IPF_DIRECTION_UL);

    if(VOS_NULL_PTR == pstIpFilter)
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IPF Get Free Filter Failed.");
        return PS_FAIL;
    }

    NAS_ERABM_MEM_SET(pstIpFilter, 0x0, sizeof(IPF_FILTER_CONFIG_S));

    pstFilter = &pstIpFilter->stMatchInfo;

    /* source address and mask for ipv6 */
    /* IPv6ֻ����64bitǰ׺ */
    for(i = 0;i < (APP_MAX_IPV6_ADDR_LEN/2); i++)
    {
        pstFilter->u8SrcAddr[i] = pstPdnAddr->aucIpV6Addr[i];
     /* pstFilter->u8SrcMsk[i]  = 0xFF; */
    }
    /*lint -e40*/
    /*lint -e63*/
    pstFilter->u32LocalAddressMsk = 64;
    /*lint +e63*/
    /*lint +e40*/
    pstFilter->unFltRuleCtrl.Bits.FltLAddrEn = NAS_ERABM_OP_TRUE;

    /* destination address and mask for ipv6 */
    if(NAS_ERABM_OP_TRUE == pstTftPf->bitOpRmtIpv6AddrAndMask)
    {
        for(i = 0;i < APP_MAX_IPV6_ADDR_LEN; i++)
        {
            pstFilter->u8DstAddr[i] = pstTftPf->aucRmtIpv6Address[i];
            pstFilter->u8DstMsk[i] = pstTftPf->aucRmtIpv6Mask[i];
        }
        pstFilter->unFltRuleCtrl.Bits.FltRAddrEn = NAS_ERABM_OP_TRUE;
    }

    /* flowlabel for ipv6 */
    if(NAS_ERABM_OP_TRUE == pstTftPf->bitOpFlowLabelType)
    {
        pstFilter->u32FlowLable = pstTftPf->ulFlowLabelType;
        pstFilter->unFltRuleCtrl.Bits.FltFlEn = NAS_ERABM_OP_TRUE;
    }
    pstFilter->unFltRuleCtrl.Bits.FltType = NAS_ERABM_IPF_IPTYPE_IPV6;

    /* ��TFT�����е����ಿ��ת��ΪIP Filter��ʽ */
    NAS_ERABM_IpfConvertOtherPartToFilter(pstTftPf, &pstIpFilter->stMatchInfo);

    /* Filter ID */
    pstIpFilter->u32FilterID =
        NAS_ERABM_IPF_UL_IPV6_FILTER_ID(ucEpsbId, pstTftPf->ucNwPacketFilterId);
    /* Filter Precedence and Bearer Id */
    pstFilter->unFltChain.Bits.u16FltPri = pstTftPf->ucPrecedence;
    pstFilter->unFltRuleCtrl.Bits.FltBid = ucEpsbId;
    pstFilter->unFltRuleCtrl.Bits.FltEn = NAS_ERABM_OP_TRUE;

    /* ��������Filter��Ŀ */
    NAS_ERABM_IPF_ADD_UL_FILTER_NUM();

    return PS_SUCC;
}
VOS_UINT32 NAS_ERABM_IpfCheckFilterType
(
    const ESM_ERABM_TFT_PF_STRU *pstTftPf,
    VOS_UINT8 ucIpType
)
{
    if(APP_ESM_PDN_TYPE_IPV4 == ucIpType) /*PDN����ΪIPV4*/
    {
        return NAS_ERABM_IPF_IPTYPE_IPV4;
    }
    else if(APP_ESM_PDN_TYPE_IPV6 == ucIpType) /*PDN����ΪIPV6*/
    {
        return NAS_ERABM_IPF_IPTYPE_IPV6;
    }
    else if(APP_ESM_PDN_TYPE_IPV4_IPV6 == ucIpType)   /* PDN����ΪIPV4V6*/
    {
        /* ���PDN������IPv4v6���ͣ���Ҫ��TFT�������Ƿ����ܹ�����IP���͵���
          �ݣ���Ŀ�ĵ�ַ���ͻ�IPv6 FlowLabel�����û�У�����Ҫ����Ipv4��IPv6
          ����Filter */
        if((NAS_ERABM_OP_TRUE == pstTftPf->bitOpRmtIpv6AddrAndMask)
           ||(NAS_ERABM_OP_TRUE == pstTftPf->bitOpFlowLabelType))
        {
            return NAS_ERABM_IPF_IPTYPE_IPV6;
        }
        else if(NAS_ERABM_OP_TRUE == pstTftPf->bitOpRmtIpv4AddrAndMask)
        {
            return NAS_ERABM_IPF_IPTYPE_IPV4;
        }
        else
        {
            return NAS_ERABM_IPF_IPTYPE_IPV4V6;
        }
    }
    else
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IpfCheckFilterType: Wrong IP Type.");
        return NAS_ERABM_IPF_IPTYPE_INVALID;
    }
}


VOS_UINT32 NAS_ERABM_IpfConvertTftToFilter
(
    const APP_ESM_IP_ADDR_STRU   *pstPdnAddr,
    const ESM_ERABM_TFT_PF_STRU   *pstTftPf,
    VOS_UINT8                     ucEpsbId
)
{
    VOS_UINT32                  ulFilterType    = NAS_ERABM_IPF_IPTYPE_IPV4;

    /* ��Ҫ���ȼ��TFT��Ӧ��IP���� */
    ulFilterType = NAS_ERABM_IpfCheckFilterType(pstTftPf, pstPdnAddr->ucIpType);

    /* ���ȸ��ݲ�ͬ��������IP Filter */
    if((NAS_ERABM_IPF_IPTYPE_IPV4 == ulFilterType)||
       (NAS_ERABM_IPF_IPTYPE_IPV4V6 == ulFilterType))
    {
        if(PS_FAIL == NAS_ERABM_IpfConvertIpv4TftToFilter(pstPdnAddr, pstTftPf, ucEpsbId))
        {
            NAS_ERABM_ERR_LOG("NAS_ERABM_IpfConvertTftToFilter: Convert Ipv4 Filter Fail.");
            return PS_FAIL;
        }
    }

    if((NAS_ERABM_IPF_IPTYPE_IPV6 == ulFilterType)||
       (NAS_ERABM_IPF_IPTYPE_IPV4V6 == ulFilterType))
    {
        if(PS_FAIL == NAS_ERABM_IpfConvertIpv6TftToFilter(pstPdnAddr, pstTftPf, ucEpsbId))
        {
            NAS_ERABM_ERR_LOG("NAS_ERABM_IpfConvertTftToFilter: Convert Ipv6 Filter Fail.");
            return PS_FAIL;
        }
    }

    return PS_SUCC;
}


VOS_UINT32 NAS_ERABM_IpfGenerateFilterWithTFT
(
    const APP_ESM_IP_ADDR_STRU *pstPdnAddr,
    VOS_UINT8 ucEpsbId
)
{
    VOS_UINT32                      ulTftPfCnt      = NAS_ERABM_NULL;
    ESM_ERABM_TFT_PF_STRU           *pstTftInfo      = VOS_NULL_PTR;

    for(ulTftPfCnt = 0; ulTftPfCnt < NAS_ERABM_GetEpsbTftPfNum(ucEpsbId); ulTftPfCnt++)
    {
        pstTftInfo = NAS_ERABM_GetEpsbTftAddr(ucEpsbId, ulTftPfCnt);

        /* ����Ҫ��������TFT���� */
        if (APP_ESM_TRANSFER_DIRECTION_DOWNLINK == pstTftInfo->enDirection)
        {
            continue;
        }

        /* ��TFT����ת��ΪIP Filter��ʽ */
        if(PS_FAIL == NAS_ERABM_IpfConvertTftToFilter(pstPdnAddr, pstTftInfo, ucEpsbId))
        {
            NAS_ERABM_ERR_LOG("NAS_ERABM_IPF Convert TFT to IPF Filter Failed.");
            return PS_FAIL;
        }
    }

    return PS_SUCC;
}


VOS_UINT32 NAS_ERABM_IpfGenerateUlNidsFilters()
{
    VOS_UINT32                  i               = 0;

    /* ������UL NDIS�������ݰ���FILTER���õ�����FILTER������ */
    for (i=0; i<NAS_ERABM_IPF_CUR_UL_NDIS_FILTER_NUM; i++)
    {
        if (PS_FAIL == NAS_ERABM_IpfAddUlNdisFilter(i))
        {
            NAS_ERABM_LOG1("NAS_ERABM_IpfGenerateUlNidsFilters:NAS_ERABM_IpfAddUlNdisFilter Failed.",i);
            return PS_FAIL;
        }
    }

    return PS_SUCC;
}
VOS_UINT32 NAS_ERABM_IpfGenerateFilters()
{
    VOS_UINT8                       ucEpsbId        = NAS_ERABM_NULL;
    APP_ESM_IP_ADDR_STRU           *pstPdnAddr      = VOS_NULL_PTR;

    /* ��յ�ǰ����Filter */
    NAS_ERABM_IPF_SET_UL_FILTER_NUM(0);

    /* ������NDIS FILTER��Чʱ��������UL NDIS�������ݰ���FILTER���õ�����
       FILTER������ */
    if (NAS_ERABM_UL_NDIS_FILTER_VALID == NAS_ERABM_IPF_GET_UL_NDIS_FILTER_FLAG())
    {
        if (PS_SUCC != NAS_ERABM_IpfGenerateUlNidsFilters())
        {
            NAS_ERABM_ERR_LOG("NAS_ERABM_IpfGenerateFilters:NAS_ERABM_IpfGenerateUlNidsFilters Failed.");
            return PS_FAIL;
        }
    }

    for(ucEpsbId = NAS_ERABM_MIN_EPSB_ID; ucEpsbId <= NAS_ERABM_MAX_EPSB_ID; ucEpsbId++)
    {
        if (NAS_ERABM_EPSB_ACTIVE != NAS_ERABM_GetEpsbStateInfo(ucEpsbId))
        {
            /* ����δ������� */
            continue;
        }

        /* ����PDN���ӵ�ַ */
        pstPdnAddr = NAS_ERABM_GetEpsbPdnAddr(ucEpsbId);

        /* �ȴ���δ����TFT������Ĭ�ϳ��� */
        if(PS_FALSE == NAS_ERABM_IsExistUplinkPfinTft(ucEpsbId))
        {
            if(PS_FAIL == NAS_ERABM_IpfGenerateFilterWithoutTFT(pstPdnAddr, ucEpsbId))
            {
                NAS_ERABM_ERR_LOG("NAS_ERABM_IPF Generate Default Bearer Filter Failed.");
                return PS_FAIL;
            }
        }
        else
        {
            /* ����ר�г��ػ�������TFT������Ĭ�ϳ���,��Ҫ����TFT Filter����ת�� */
            if(PS_FAIL == NAS_ERABM_IpfGenerateFilterWithTFT(pstPdnAddr, ucEpsbId))
            {
                NAS_ERABM_ERR_LOG("NAS_ERABM_IPF Generate Dedicate Bearer Filter Failed.");
                return PS_FAIL;
            }
        }
    }

    return PS_SUCC;
}


VOS_VOID NAS_ERABM_IpfFilterSort(IPF_FILTER_CONFIG_S *pstFilter, VOS_UINT32 ulNum)
{
    /* ����C���ԵĿ⺯��qsort��pstFilter���п������� */
    qsort(pstFilter, ulNum, sizeof(IPF_FILTER_CONFIG_S), (NAS_IPF_COMP_FUNC)NAS_ERABM_IpfCompare);
}
VOS_UINT32 NAS_ERABM_IpfConfigUlFilter()
{
    VOS_UINT32                      ulIpFilterNum   = 0;
    IPF_FILTER_CONFIG_S            *pstFilterList   = 0;

    /*��ӡ����ú���*/
    NAS_ERABM_INFO_LOG("NAS_ERABM_IpfConfigFilter is entered.");

    /* ��������TFT������IP Filter */
    if(PS_FAIL == NAS_ERABM_IpfGenerateFilters())
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IpfConfigFilter: Generate All Filters Fail.");

        /* ����ͳ����Ϣ */
        NAS_ERABM_IPF_OM_ADD_UL_FILTER_ERR_CNT();
        return PS_FAIL;
    }

    /* ����qsort������Filter�������� */
    ulIpFilterNum = NAS_ERABM_IPF_GET_UL_FILTER_NUM();
    pstFilterList = NAS_ERABM_IPF_GET_UL_FILTER_PTR(0);

    NAS_ERABM_IpfFilterSort(pstFilterList, ulIpFilterNum);

    /* ���������ӿڣ� ��Filter�б� ���õ�Ӳ���������� */
    /*lint -e64*/
    if(PS_SUCC != BSP_IPF_SetFilter(IPF_MODEM0_ULFC, (IPF_FILTER_CONFIG_S*)pstFilterList, ulIpFilterNum))
    /*lint +e64*/
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IpfConfigFilter: BSP Config All Filters Fail.");
        return PS_FAIL;
    }
#if 0
    if(PS_SUCC != BSP_IPF_SetFilter(IPF_CHANNEL_UP, (IPF_FILTER_CONFIG_S *)pstFilterList, ulIpFilterNum))
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IpfConfigFilter: BSP Config All Filters Fail.");
        return PS_FAIL;
    }
#endif
    return PS_SUCC;
}
VOS_UINT8 NAS_ERABM_GetSrcIpv4MatchedDeftBearerId
(
    const NAS_ERABM_IPV4_HEADER_STRU   *pstIpv4HeaderInfo,
    VOS_UINT8                          *pucBearerId
)
{
    VOS_UINT32                          ulEpsbId    = NAS_ERABM_NULL;
    APP_ESM_IP_ADDR_STRU               *pstPdnAddr  = VOS_NULL_PTR;

    for (ulEpsbId = NAS_ERABM_MIN_EPSB_ID; ulEpsbId<= NAS_ERABM_MAX_EPSB_ID; ulEpsbId++)
    {
        if (NAS_ERABM_EPSB_ACTIVE != NAS_ERABM_GetEpsbStateInfo(ulEpsbId))
        {
            continue;
        }

        if (ESM_ERABM_BEARER_TYPE_DEDICATED == NAS_ERABM_GetEpsbBearerType(ulEpsbId))
        {
            continue;
        }

        pstPdnAddr = NAS_ERABM_GetEpsbPdnAddr(ulEpsbId);

        if ((NAS_ERABM_PDN_IPV6 == pstPdnAddr->ucIpType))
        {
            continue;
        }

        /* ƥ��ԴIPv4��ַ */
        if (0 == NAS_ERABM_MEM_CMP( pstPdnAddr->aucIpV4Addr,
                                    pstIpv4HeaderInfo->aucSrcIpV4Addr,
                                    NAS_ERABM_IPV4_ADDR_LEN))
        {
            *pucBearerId = (VOS_UINT8)ulEpsbId;
            return NAS_ERABM_SUCCESS;
        }
    }

    /* ���Ӷ���IPV4��Ƭ��ͳ����Ϣ */
    NAS_ERABM_IPF_OM_ADD_RCV_UL_DISCARD_IPV4_PACK_NUM();

    *pucBearerId = NAS_ERABM_IPF_INVALID_BEARER_ID;
    return NAS_ERABM_FAILURE;
}


VOS_UINT8 NAS_ERABM_GetSrcIpv6MatchedDeftBearerId
(
    const NAS_ERABM_IPV6_HEADER_STRU   *pstIpv6HeaderInfo,
    VOS_UINT8                          *pucBearerId
)
{
    VOS_UINT32                          ulEpsbId    = NAS_ERABM_NULL;
    APP_ESM_IP_ADDR_STRU               *pstPdnAddr  = VOS_NULL_PTR;

    for (ulEpsbId = NAS_ERABM_MIN_EPSB_ID; ulEpsbId<= NAS_ERABM_MAX_EPSB_ID; ulEpsbId++)
    {
        if (NAS_ERABM_EPSB_ACTIVE != NAS_ERABM_GetEpsbStateInfo(ulEpsbId))
        {
            continue;
        }

        pstPdnAddr = NAS_ERABM_GetEpsbPdnAddr(ulEpsbId);
        if ((NAS_ERABM_PDN_IPV4 == pstPdnAddr->ucIpType))
        {
            continue;
        }

        if (ESM_ERABM_BEARER_TYPE_DEDICATED == NAS_ERABM_GetEpsbBearerType(ulEpsbId))
        {
            continue;
        }

        /* ƥ��ԴIPv6ǰ׺ */
        if (0 == NAS_ERABM_MEM_CMP( pstPdnAddr->aucIpV6Addr,
                                    pstIpv6HeaderInfo->aucSrcIpV6Addr,
                                    NAS_ERABM_IPV6_PREFIX_LEN))
        {
            *pucBearerId = (VOS_UINT8)ulEpsbId;
            return NAS_ERABM_SUCCESS;
        }
    }

    /* ���Ӷ���IPV6��Ƭ����ͳ����Ϣ */
    NAS_ERABM_IPF_OM_ADD_RCV_UL_DISCARD_IPV6_PACK_NUM();

    *pucBearerId = NAS_ERABM_IPF_INVALID_BEARER_ID;

    return NAS_ERABM_FAILURE;
}



VOS_UINT8 NAS_ERABM_IpfIpv4HeadFragmentProcess
(
    const NAS_ERABM_IPV4_HEADER_STRU   *pucIpv4Data,
    VOS_UINT8                           ucBearerId,
    VOS_UINT8                           ucUpperLayerFlag
)
{
    VOS_UINT32                          ulRslt          = NAS_ERABM_NULL;
    VOS_UINT8                           ucBearerIdTmp   = NAS_ERABM_NULL;

    /* �ж��Ƿ�����ϲ�ͷ */
    if (NAS_ERABM_NO_UPPERLAYER != ucUpperLayerFlag)
    {
        /* �洢ԴIP��ַ��Ŀ��IP��ַ��identifier�Լ�ƥ�����е�usBearedId */
        NAS_ERABM_SaveIpv4SegDataInfo(pucIpv4Data, ucBearerId);

        return ucBearerId;
    }

    /* ����ͷ����Ƭ��ƥ����Ϣ������IP��Ƭ��ͳ����Ϣ */
    NAS_ERABM_IPF_OM_ADD_RCV_UL_IPV4_HEADER_NO_UPLAYER_PACK_NUM();

    /* ��������ȱʡ���أ�����ԴIP��ַƥ��ĳ��� */
    ulRslt = NAS_ERABM_GetSrcIpv4MatchedDeftBearerId(pucIpv4Data, &ucBearerIdTmp);

    if (NAS_ERABM_SUCCESS == ulRslt)
    {
        return ucBearerIdTmp;
    }

    return NAS_ERABM_IPF_INVALID_BEARER_ID;
}


    VOS_UINT8 NAS_ERABM_FindBearerIdInIpv4SegBuff
    (
        const NAS_ERABM_IPV4_HEADER_STRU   *pstIpv4HeaderInfo,
        VOS_UINT8                          *pucBearerIdTmp
    )
    {
        VOS_UINT32                          ulLoop           = NAS_ERABM_NULL;
        NAS_ERABM_IPV4_SEG_BUFF_ITEM_STRU  *pstSegDataInfo   = VOS_NULL_PTR;

        for (ulLoop = NAS_ERABM_NULL; ulLoop < NAS_ERABM_GetIpv4SegBuffItemNum(); ulLoop++)
        {
            pstSegDataInfo = NAS_ERABM_GetIpv4SegBuffItemAddr(ulLoop);

            /* ��ͬһ�������Ƭ�����ĸ���Ƭ�����кܶ���ͬ�����ԣ�����identifier��
               Դ��Ŀ��IP��ַ */
            if ((pstIpv4HeaderInfo->ulIdentifier == pstSegDataInfo->ulIdentifier)
               && (0 == NAS_ERABM_MEM_CMP(  pstIpv4HeaderInfo->aucSrcIpV4Addr,
                                            pstSegDataInfo->aucSrcIpV4Addr,
                                            NAS_ERABM_IPV4_ADDR_LEN))
               && (0 == NAS_ERABM_MEM_CMP(  pstIpv4HeaderInfo->aucDesIpV4Addr,
                                            pstSegDataInfo->aucDesIpV4Addr,
                                            NAS_ERABM_IPV4_ADDR_LEN)))
            {
                *pucBearerIdTmp = pstSegDataInfo->ucBearerId;

                return NAS_ERABM_SUCCESS;
            }
        }

        /* ����ͳ�������� */
        NAS_ERABM_IPF_OM_ADD_RCV_UL_IPV4_NON_HEADER_FIND_DEFT_FAIL_PACK_NUM();

        *pucBearerIdTmp = NAS_ERABM_IPF_INVALID_BEARER_ID;
        return NAS_ERABM_FAILURE;
    }
VOS_UINT8 NAS_ERABM_IpfIpv4NonHeadFragmentProcess
(
    const NAS_ERABM_IPV4_HEADER_STRU   *pucIpv4Data
)
{
    VOS_UINT32                          ulRslt          = NAS_ERABM_NULL;
    VOS_UINT8                           ucBearerIdTmp   = NAS_ERABM_NULL;

    /* ����ԴIP��ַ��Ŀ��IP��ַ��identifier��ƥ����Ϣ��������в��� */
    ulRslt = NAS_ERABM_FindBearerIdInIpv4SegBuff(pucIpv4Data , &ucBearerIdTmp);
    if (NAS_ERABM_SUCCESS == ulRslt)
    {
        return ucBearerIdTmp;
    }

    /* ��������ȱʡ���أ�����ԴIP��ַƥ��ĳ��� */
    ulRslt = NAS_ERABM_GetSrcIpv4MatchedDeftBearerId(pucIpv4Data, &ucBearerIdTmp);
    if (NAS_ERABM_SUCCESS == ulRslt)
    {
        return ucBearerIdTmp;
    }

    return NAS_ERABM_IPF_INVALID_BEARER_ID;
}



VOS_UINT8 NAS_ERABM_IpfIpv4FragmentProcess
(
    VOS_UINT8                          *pstDataAddr,
    VOS_UINT16                          usSegType,
    VOS_UINT8                           ucBearerId,
    VOS_UINT8                           ucUpperLayerFlag
)
{
    VOS_UINT8                          *pucIpv4Data = VOS_NULL_PTR;

    /* ����IPV4��ͷ���洢ԴIPV4��ַ��Ŀ��IPV4��ַ��identifier */
    pucIpv4Data = NAS_ERABM_GET_IP_HEADER_BUFFER();

    NAS_ERABM_DecodeIpV4Data(pstDataAddr, (NAS_ERABM_IPV4_HEADER_STRU *)pucIpv4Data);

    /* �ж��Ƿ���ͷ����Ƭ */
    if (NAS_ERABM_IPF_IPF_FRAGMENT_HEAD == usSegType)
    {
        /* ����IPV4ͷ����Ƭͳ����Ϣ */
        NAS_ERABM_IPF_OM_ADD_RCV_UL_IPV4_HEADER_PACK_NUM();

        return NAS_ERABM_IpfIpv4HeadFragmentProcess(    (NAS_ERABM_IPV4_HEADER_STRU *)pucIpv4Data,
                                                        ucBearerId,
                                                        ucUpperLayerFlag);
    }
    else
    {
        /* ����IPV4��ͷ����Ƭͳ����Ϣ */
        NAS_ERABM_IPF_OM_ADD_RCV_UL_IPV4_NON_HEADER_PACK_NUM();

        return NAS_ERABM_IpfIpv4NonHeadFragmentProcess((NAS_ERABM_IPV4_HEADER_STRU *)pucIpv4Data);
    }
}


VOS_UINT8 NAS_ERABM_IpfIpv6FragmentProcess
(
    VOS_UINT8                          *pstDataAddr,
    VOS_UINT16                          usSegType,
    VOS_UINT8                           ucMatchId,
    VOS_UINT8                           ucUpperLayerFlag
)
{
    VOS_UINT8                          *pucIpv6Data     = VOS_NULL_PTR;
    VOS_UINT32                          ulRslt          = NAS_ERABM_NULL;
    VOS_UINT8                           ucBearerIdTmp   = NAS_ERABM_NULL;

    (VOS_VOID)usSegType;
    (VOS_VOID)ucMatchId;
    (VOS_VOID)ucUpperLayerFlag;

    /* ����IPV6��ͷ���洢ԴIPV6��ַ��Ŀ��IPV6��ַ��identifier */
    pucIpv6Data = NAS_ERABM_GET_IP_HEADER_BUFFER();

    NAS_ERABM_DecodeIpV6Data(pstDataAddr, (NAS_ERABM_IPV6_HEADER_STRU *)pucIpv6Data);

    /* ��������ȱʡ���أ�����ԴIP��ַƥ��ĳ��� */
    ulRslt = NAS_ERABM_GetSrcIpv6MatchedDeftBearerId(   (NAS_ERABM_IPV6_HEADER_STRU *)pucIpv6Data,
                                                        &ucBearerIdTmp);
    if (NAS_ERABM_SUCCESS == ulRslt)
    {
        return ucBearerIdTmp;
    }

    return NAS_ERABM_IPF_INVALID_BEARER_ID;
}
#if 0

VOS_UINT8 NAS_ERABM_IpfDlIpFragmentProcess
(
    VOS_UINT8                          *pstDataAddr,
    VOS_UINT16                          usIpVer,
    VOS_UINT16                          usSegType,
    VOS_UINT8                           ucBearerId,
    VOS_UINT8                           ucUpperLayerFlag
)
{
    (VOS_VOID)pstDataAddr;
    (VOS_VOID)usIpVer;
    (VOS_VOID)usSegType;
    (VOS_VOID)ucBearerId;
    (VOS_VOID)ucUpperLayerFlag;

    return NAS_ERABM_IPF_INVALID;
}
#endif


VOS_UINT8 NAS_ERABM_IpfIpFragmentProcess
(
    VOS_UINT8                          *pstDataAddr,
    VOS_UINT16                          usIpVer,
    VOS_UINT16                          usSegType,
    VOS_UINT8                           ucBearerId,
    VOS_UINT8                           ucUpperLayerFlag
)
{
    if (NAS_ERABM_IPFILTER_VERSION_4 == usIpVer)
    {
        /* add ul ipv4 frament packet num */
        NAS_ERABM_IPF_OM_ADD_RCV_UL_IPV4_PACK_NUM();

        ucBearerId = NAS_ERABM_IpfIpv4FragmentProcess(pstDataAddr, usSegType, ucBearerId, ucUpperLayerFlag);
    }
    else
    {
        /* add ul ipv6 frament packet num */
        NAS_ERABM_IPF_OM_ADD_RCV_UL_IPV6_PACK_NUM();

        ucBearerId = NAS_ERABM_IpfIpv6FragmentProcess(pstDataAddr, usSegType, ucBearerId, ucUpperLayerFlag);
    }

    return ucBearerId;
}

#if 0

VOS_UINT32 NAS_ERABM_IpfConfigDlFilter()
{
    VOS_UINT32                      ulFilterNum         = 0;
    IPF_FILTER_CONFIG_S            *pstFilterList       = 0;

    pstFilterList = NAS_ERABM_IPF_GET_DL_FILTER_PTR(0);
    ulFilterNum = NAS_ERABM_IPF_GET_DL_FILTER_NUM();

    /* ���������ӿڣ� ��Filter�б� ���õ�Ӳ���������� */
    if(PS_SUCC != BSP_IPF_SetFilter(IPF_CHANNEL_DOWN, (IPF_FILTER_CONFIG_S *)pstFilterList, ulFilterNum))
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IpfConfigFilter: BSP Config All Filters Fail.");
        return PS_FAIL;
    }

    return PS_SUCC;
}
VOS_UINT32 NAS_ERABM_IpfAddDlFilter(const IPF_FILTER_CONFIG_S *pstFilter)
{
    IPF_FILTER_CONFIG_S        *pstIpFilter     = VOS_NULL_PTR;

    if(VOS_NULL_PTR == pstFilter)
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IpfAddDlFilter Input Null Ptr.");
        return PS_FAIL;
    }

    /* ��Filter�������ҵ���һ�����е�Filter */
    pstIpFilter = NAS_ERABM_IpfGetFreeFilter(NAS_ERABM_IPF_DIRECTION_DL);
    if(VOS_NULL_PTR == pstIpFilter)
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IPF Get Free Filter Failed.");
        return PS_FAIL;
    }

    NAS_ERABM_MEM_SET(pstIpFilter, 0x0, sizeof(IPF_FILTER_CONFIG_S));

    PS_MEM_CPY(pstIpFilter, pstFilter, sizeof(IPF_FILTER_CONFIG_S));

    /* ��������Filter��Ŀ */
    NAS_ERABM_IPF_ADD_DL_FILTER_NUM();

    /* �������е��������õ�Filter���õ�Ӳ�������� */
    return NAS_ERABM_IpfConfigDlFilter();
}
VOS_UINT32 NAS_ERABM_IpfRemoveDlFilter(VOS_UINT32 ulFilterId)
{
    VOS_UINT32                  i               = 0;
    VOS_UINT32                  j               = 0;
    VOS_UINT32                  ulFound         = PS_FALSE;
    VOS_UINT32                  ulDlFilterNum   = 0;
    IPF_FILTER_CONFIG_S        *pstIpFilter     = VOS_NULL_PTR;
    IPF_FILTER_CONFIG_S        *pstNextFilter   = VOS_NULL_PTR;

    /* ������Filter�������ҵ���ӦID��Filter, ��������Filter��ǰһλ���� */
    ulDlFilterNum = NAS_ERABM_IPF_GET_DL_FILTER_NUM();
    for(i = 0; i < ulDlFilterNum; i++)
    {
        pstIpFilter = NAS_ERABM_IPF_GET_DL_FILTER_PTR(i);

        if(ulFilterId == pstIpFilter->u32FilterID)
        {
            for(j = i; j < ulDlFilterNum - 1; j++)
            {
                pstIpFilter = NAS_ERABM_IPF_GET_DL_FILTER_PTR(j);
                pstNextFilter = NAS_ERABM_IPF_GET_DL_FILTER_PTR(j+1);

                PS_MEM_CPY(pstIpFilter, pstNextFilter, sizeof(IPF_FILTER_CONFIG_S));
            }
            /* ����ҵ���Filter����Ҫ����Filter��Ŀ���������õ�Ӳ�������� */
            ulFound = PS_TRUE;
            break;
        }
    }

    if(PS_TRUE == ulFound)
    {
        /* ��������Filter��ĿΪ FilterNum-1 */
        NAS_ERABM_IPF_SET_DL_FILTER_NUM(ulDlFilterNum-1);

        /* �������е��������õ�Filter���õ�Ӳ�������� */
        return NAS_ERABM_IpfConfigDlFilter();
    }

    return PS_SUCC;
}


VOS_VOID NAS_ERABM_IpfSendDataToIpm(const TTF_MEM_ST *pstData, VOS_UINT8 ucEpsbId)
{
    if(ucEpsbId <= NAS_ERABM_MAX_EPSB_ID)
    {
        /* ����IPM�ӿڷ������� */
        NAS_ERABM_SndIpRabmDataInd(ucEpsbId, pstData->pData, pstData->usUsed);

        NAS_ERABM_LOG1("NAS_ERABM_IpfSendDataToIpm, EPSB ID : ", ucEpsbId);
    }
    else
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IpfSendDataToIpm, Dl Data Will Be Discarded Outside.");
    }
}
VOS_UINT8 NAS_ERABM_IpfGetEpsbIdFromRbId(VOS_UINT8 ucRbId)
{
    VOS_UINT8           ucEpsbId   = NAS_ERABM_ILL_EPSB_ID;
    VOS_UINT32          ulFindEpsb = NAS_ERABM_EPSB_NOT_EXIST;

    /*�ҵ�RBID��Ӧ��EPS����*/
    for (ucEpsbId = NAS_ERABM_MIN_EPSB_ID; ucEpsbId <= NAS_ERABM_MAX_EPSB_ID; ucEpsbId++)
    {
        if (NAS_ERABM_GetEpsbRbIdInfo(ucEpsbId) == ucRbId)
        {
            ulFindEpsb = NAS_ERABM_EPSB_EXIST;
            break;
        }
    }

    /*���û���ҵ�RB��Ӧ��EPS���أ�����ü����EPS���ؽ������ݴ��䣬���Ҵ�ӡ������Ϣ*/
    if (NAS_ERABM_EPSB_NOT_EXIST == ulFindEpsb)
    {
        for (ucEpsbId = NAS_ERABM_MIN_EPSB_ID; ucEpsbId <= NAS_ERABM_MAX_EPSB_ID; ucEpsbId++)
        {
            if (NAS_ERABM_EPSB_ACTIVE == NAS_ERABM_GetEpsbStateInfo(ucEpsbId))
            {
                NAS_ERABM_WARN_LOG("NAS_ERABM_IpfSendDataToIpm:WARNING: RB non-exist!");
                break;
            }
        }
    }

    return ucEpsbId;
}


PS_BOOL_ENUM_UINT8 NAS_ERABM_IpfDlDataProcess
(
    const IPF_RESULT_STRU *pstIpfResult,
    const TTF_MEM_ST *pstData
)
{
    VOS_UINT8                       ucRbid      = 0;
    VOS_UINT8                       ucEpsbId    = 0;
    VOS_UINT8                       ucFilterId  = 0;
    VOS_UINT16                      usIpVer     = 0;
    NAS_ERABM_IPF_IPSEG_INFO_UINT16  usIpSeg     = 0;
    VOS_UINT8                       ucUpperlayer = 0;

    if((VOS_NULL_PTR == pstData)||(VOS_NULL_PTR == pstIpfResult))
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IpfDlDataProcess, Input Null Ptr.");
        return PS_FALSE;
    }

    /*ִ�б�Ҫ�Ĵ����飻*/
    if(NAS_ERABM_IPF_RESULT_ERR_CHECK(pstIpfResult))
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IpfDlDataProcess DL IP Format Error.");

        /* ����ͳ����Ϣ */
        NAS_ERABM_IPF_OM_ADD_DL_ERR_CNT();
        return PS_FALSE;
    }

    /* ����������ͳ�� */
    NAS_ERABM_IPF_OM_ADD_DL_PRCS_BYTES(pstData->usUsed);

    /* ��ȡIP�汾�ͷ�Ƭ��Ϣ */
    usIpVer         = pstIpfResult->usIpType;
    usIpSeg         = pstIpfResult->usIpSegInfo;
    ucFilterId      = pstIpfResult->usBearedId;
    ucUpperlayer    = pstIpfResult->usUpperLayer;

    /* IP��Ƭ���� */
    if(NAS_ERABM_IPF_IPF_NO_FRAGMENT != usIpSeg)
    {
        /* תIP��Ƭ������ */
        ucFilterId = NAS_ERABM_IpfDlIpFragmentProcess(  pstData->pData,
                                                        usIpVer,
                                                        usIpSeg,
                                                        ucFilterId,
                                                        ucUpperlayer);

        /* ����ͳ����Ϣ */
        NAS_ERABM_IPF_OM_ADD_DL_FRAG_CNT();
    }

    ucRbid      = (VOS_UINT8)pstData->ulForLte[1];
    ucEpsbId    = NAS_ERABM_IpfGetEpsbIdFromRbId(ucRbid);

    /*�����DHCP/ICMP���ݣ������ݿ���һ�ݣ�ת����IP��Ӧģ�飬��ָʾPS COM��*/
    if((NAS_ERABM_IPF_ICMP4 == ucFilterId)||
       (NAS_ERABM_IPF_ICMP6 == ucFilterId)||
       (NAS_ERABM_IPF_DHCP4 == ucFilterId)||
       (NAS_ERABM_IPF_DHCP6 == ucFilterId))
    {
        /* �����ݿ��������͸�IPMģ�� */
        NAS_ERABM_IpfSendDataToIpm(pstData, ucEpsbId);

        /* ����ͳ����Ϣ */
        NAS_ERABM_IPF_OM_ADD_DL_NON_USRDATA_CNT();

        return PS_FALSE;
    }

    /* ����ͳ����Ϣ */
    NAS_ERABM_IPF_OM_ADD_DL_USRDATA_CNT();

    /* �û������ݲ���Ҫ�������������߸����� */
    return PS_TRUE;
}


/*lint -e818*/
PS_BOOL_ENUM_UINT8 LNAS_ERABM_IpfDlRdProc(TTF_MEM_ST *pstTtfMem)
{
    NAS_ERABM_IPF_OM_ADD_DL_RCV_NUM();
    /* �����������ݴ����� */
    return NAS_ERABM_IpfDlDataProcess((IPF_RESULT_STRU*)&pstTtfMem->ulForLte[0], pstTtfMem);
}
/*lint +e818*/

VOS_UINT32 NAS_ERABM_IpfAddUserFilter(VOS_UINT32 ulFilterId)
{
    IPF_FILTER_CONFIG_S             stFilter;
    NAS_ERABM_IPF_DL_FILTER_INFO    *pstUserFilter;

    if(ulFilterId >= NAS_ERABM_IPF_DL_FITLER_BUTT)
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IpfAddUserFilter, Wrong Filter Type");
        return PS_FAIL;
    }

    pstUserFilter = NAS_ERABM_IPF_GET_DL_USER_FILTER(ulFilterId);

    /* ��User Filter������ת��Ϊ IP Filter��ʽ */
    NAS_ERABM_MEM_SET(&stFilter, 0x0, sizeof(IPF_FILTER_CONFIG_S));

    stFilter.u32FilterID = ulFilterId;

    stFilter.stMatchInfo.unFltCodeType.Bits.u16Code   = pstUserFilter->usIcmpCode;
    stFilter.stMatchInfo.unFltCodeType.Bits.u16Type   = pstUserFilter->usIcmpType;
    stFilter.stMatchInfo.unDstPort.Bits.u16DstPortHi  = pstUserFilter->usPortHi;
    stFilter.stMatchInfo.unDstPort.Bits.u16DstPortLo  = pstUserFilter->usPortLo;
    stFilter.stMatchInfo.unNextHeader.u32Protocol     = pstUserFilter->ucProtocolId;
    stFilter.stMatchInfo.unFltRuleCtrl.Bits.FltType   = pstUserFilter->ucIpType;
    stFilter.stMatchInfo.unFltRuleCtrl.Bits.FltCodeEn = pstUserFilter->bitOpCode;
    stFilter.stMatchInfo.unFltRuleCtrl.Bits.FltTypeEn = pstUserFilter->bitOpType;
    stFilter.stMatchInfo.unFltRuleCtrl.Bits.FltNhEn   = pstUserFilter->bitOpProtocolId;
    stFilter.stMatchInfo.unFltRuleCtrl.Bits.FltRPortEn= pstUserFilter->bitOpPort;
    stFilter.stMatchInfo.unFltRuleCtrl.Bits.FltBid    = NAS_ERABM_IPF_CONV_DLFILTER_TO_BID(ulFilterId);
    stFilter.stMatchInfo.unFltChain.Bits.u16FltPri    = 0;
    stFilter.stMatchInfo.unFltRuleCtrl.Bits.FltEn     = NAS_ERABM_OP_TRUE;

    return NAS_ERABM_IpfAddDlFilter(&stFilter);

}
VOS_UINT32 NAS_ERABM_IpfRmvUserFilter(VOS_UINT32 ulFilterId)
{
    return NAS_ERABM_IpfRemoveDlFilter(ulFilterId);
}


VOS_UINT32 NAS_ERABM_IpfExtractEpsbId(const LUP_MEM_ST *pstSdu, VOS_UINT32 *pulEpsbId)
{
    VOS_UINT16                          usIpVer         = 0;
    IPF_RESULT_STRU                    *pstIpfResult    = VOS_NULL_PTR;
    NAS_ERABM_IPF_IPSEG_INFO_UINT16     usIpSeg         = 0;
    VOS_UINT8                           ucUpperlayer    = 0;

    if(pstSdu == VOS_NULL_PTR)
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IpfExtractEpsbId, Input Data Null Ptr. ");
        return PS_FAIL;
    }

    pstIpfResult = (IPF_RESULT_STRU*)&pstSdu->rMem->aulRev[0];

    /* �����飬 �������˽����Ч��־�����ݽ�������IP���ȴ���IP�汾����
      ������������������ͷ��ڴ� */
    if(NAS_ERABM_IPF_RESULT_ERR_CHECK(pstIpfResult))
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IpfExtractEpsbId UL IP Format Error.");

        /* ����ͳ����Ϣ */
        NAS_ERABM_IPF_OM_ADD_UL_ERR_CNT();

        return PS_FAIL;
    }

    /* ��ȡIP�汾�ͷ�Ƭ��Ϣ */
    usIpVer         = pstIpfResult->usIpType;
    usIpSeg         = pstIpfResult->usIpSegInfo;
    *pulEpsbId      = (VOS_UINT32)pstIpfResult->usBearedId;
    ucUpperlayer    = pstIpfResult->usUpperLayer;

    if(NAS_ERABM_IPF_IPF_NO_FRAGMENT != usIpSeg)/* IP ��Ƭ��� */
    {
        /* תIP��Ƭ������ */
        *pulEpsbId = NAS_ERABM_IpfIpFragmentProcess(    pstSdu->pData,
                                                        usIpVer,
                                                        usIpSeg,
                                                        (VOS_UINT8)*pulEpsbId,
                                                        ucUpperlayer);

        /* ����ͳ����Ϣ */
        NAS_ERABM_IPF_OM_ADD_UL_FRAG_CNT();
    }

    if(NAS_ERABM_IPF_INVALID_BEARER_ID != *pulEpsbId)
    {
        /* ����ͳ����Ϣ */
        NAS_ERABM_IPF_OM_ADD_UL_RCV_NUM();
        NAS_ERABM_IPF_OM_ADD_UL_RCV_BYTES(pstSdu->usUsed);

        return PS_SUCC;
    }
    else
    {
        /* ���ָʾ��ƥ������Filter���������ݣ��ͷ��ڴ� */
        NAS_ERABM_ERR_LOG("NAS_ERABM_IpfExtractEpsbId, Does not Match Any Filter.");
        NAS_ERABM_IPF_OM_ADD_UL_DISCARD_CNT();

        return PS_FAIL;
    }
}
#endif
VOS_UINT32 NAS_ERABM_IpfAddUlNdisFilter(VOS_UINT32 ulFilterId)
{
    NAS_ERABM_IPF_UL_NDIS_FILTER_INFO  *pstUlNdisFilter = VOS_NULL_PTR;
    IPF_FILTER_CONFIG_S                *pstIpFilter     = VOS_NULL_PTR;

    if(ulFilterId >= NAS_ERABM_IPF_UL_NDIS_FILTER_BUTT)
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IpfAddUlNdisFilter, Wrong Filter Type");
        return PS_FAIL;
    }

    /* ��Filter�������ҵ���һ�����е�Filter */
    pstIpFilter = NAS_ERABM_IpfGetFreeFilter(NAS_ERABM_IPF_DIRECTION_UL);
    if(VOS_NULL_PTR == pstIpFilter)
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IpfAddUlNdisFilter Get Free Filter Failed.");
        return PS_FAIL;
    }

    NAS_ERABM_MEM_SET(pstIpFilter, 0x0, sizeof(IPF_FILTER_CONFIG_S));

    pstUlNdisFilter = NAS_ERABM_IPF_GET_UL_NDIS_USER_FILTER(ulFilterId);

    pstIpFilter->u32FilterID = ulFilterId;

    pstIpFilter->stMatchInfo.unFltCodeType.Bits.u16Code   = pstUlNdisFilter->usIcmpCode;
    pstIpFilter->stMatchInfo.unFltCodeType.Bits.u16Type   = pstUlNdisFilter->usIcmpType;
    pstIpFilter->stMatchInfo.unDstPort.Bits.u16DstPortHi  = pstUlNdisFilter->usPortHi;
    pstIpFilter->stMatchInfo.unDstPort.Bits.u16DstPortLo  = pstUlNdisFilter->usPortLo;
    pstIpFilter->stMatchInfo.unNextHeader.u32Protocol     = pstUlNdisFilter->ucProtocolId;

    if (NAS_ERABM_IPF_IPTYPE_IPV6 == pstUlNdisFilter->ucIpType)
    {
        NAS_ERABM_MEM_CPY(  pstIpFilter->stMatchInfo.u8DstAddr,
                            pstUlNdisFilter->aucDstAdrr,
                            APP_MAX_IPV6_ADDR_LEN);

        NAS_ERABM_MEM_CPY(  pstIpFilter->stMatchInfo.u8DstMsk,
                            pstUlNdisFilter->aucDstMask,
                            APP_MAX_IPV6_ADDR_LEN);
    }
    else
    {
        NAS_ERABM_MEM_CPY(  pstIpFilter->stMatchInfo.u8DstAddr,
                            pstUlNdisFilter->aucDstAdrr,
                            APP_MAX_IPV4_ADDR_LEN);

        NAS_ERABM_MEM_CPY(  pstIpFilter->stMatchInfo.u8DstMsk,
                            pstUlNdisFilter->aucDstMask,
                            APP_MAX_IPV4_ADDR_LEN);
    }

    pstIpFilter->stMatchInfo.unFltRuleCtrl.Bits.FltType   = pstUlNdisFilter->ucIpType;
    pstIpFilter->stMatchInfo.unFltRuleCtrl.Bits.FltCodeEn = pstUlNdisFilter->bitOpCode;
    pstIpFilter->stMatchInfo.unFltRuleCtrl.Bits.FltTypeEn = pstUlNdisFilter->bitOpType;
    pstIpFilter->stMatchInfo.unFltRuleCtrl.Bits.FltNhEn   = pstUlNdisFilter->bitOpProtocolId;
    pstIpFilter->stMatchInfo.unFltRuleCtrl.Bits.FltRPortEn= pstUlNdisFilter->bitOpPort;
    pstIpFilter->stMatchInfo.unFltRuleCtrl.Bits.FltRAddrEn= pstUlNdisFilter->bitOpDstAdrr;
    pstIpFilter->stMatchInfo.unFltRuleCtrl.Bits.FltBid    = NAS_ERABM_IPF_CONV_ULNDISFILTER_TO_BID(ulFilterId);
    pstIpFilter->stMatchInfo.unFltChain.Bits.u16FltPri    = pstUlNdisFilter->usPriority;
    pstIpFilter->stMatchInfo.unFltRuleCtrl.Bits.FltEn     = NAS_ERABM_OP_TRUE;

    /* ��������Filter��Ŀ */
    NAS_ERABM_IPF_ADD_UL_FILTER_NUM();

    return PS_SUCC;
}


VOS_UINT32 NAS_ERABM_IpfRmvUlNdisFilter(VOS_UINT32 ulFilterId)
{
    VOS_UINT32                  i               = 0;
    VOS_UINT32                  j               = 0;
    VOS_UINT32                  ulFound         = PS_FALSE;
    VOS_UINT32                  ulUlFilterNum   = 0;
    IPF_FILTER_CONFIG_S        *pstIpFilter     = VOS_NULL_PTR;
    IPF_FILTER_CONFIG_S        *pstNextFilter   = VOS_NULL_PTR;

    /* ������Filter�������ҵ���ӦID��Filter, ��������Filter��ǰһλ���� */
    ulUlFilterNum = NAS_ERABM_IPF_GET_UL_FILTER_NUM();
    for(i = 0; i < ulUlFilterNum; i++)
    {
        pstIpFilter = NAS_ERABM_IPF_GET_UL_FILTER_PTR(i);

        if(ulFilterId == pstIpFilter->u32FilterID)
        {
            for(j = i; j < (ulUlFilterNum - 1); j++)
            {
                pstIpFilter = NAS_ERABM_IPF_GET_UL_FILTER_PTR(j);
                pstNextFilter = NAS_ERABM_IPF_GET_UL_FILTER_PTR(j+1);

                PS_MEM_CPY(pstIpFilter, pstNextFilter, sizeof(IPF_FILTER_CONFIG_S));
            }

            /* ����ҵ���Filter����Ҫ����Filter��Ŀ */
            ulFound = PS_TRUE;
            break;
        }
    }

    if(PS_TRUE == ulFound)
    {
        /* ��������Filter��ĿΪ FilterNum-1 */
        NAS_ERABM_IPF_SET_UL_FILTER_NUM(ulUlFilterNum-1);
    }

    return PS_SUCC;
}


VOS_VOID NAS_ERABM_SetUlNdisFilterValidFlag
(
    NAS_ERABM_UL_NDIS_FILTER_FLAG_ENUM_UINT32   enUlNdisFilterFlag
)
{
    NAS_ERABM_IPF_SET_UL_NDIS_FILTER_FLAG(enUlNdisFilterFlag);
}


VOS_INT32 NAS_ERABM_CdsCompare(const VOS_VOID *arg1, const VOS_VOID *arg2)
{
    VOS_UINT16              usVar1;
    VOS_UINT16              usVar2;
    CDS_SOFTFILTER_INFO_STRU*   pstPara1;
    CDS_SOFTFILTER_INFO_STRU*   pstPara2;

    pstPara1 = (CDS_SOFTFILTER_INFO_STRU*) arg1;
    pstPara2 = (CDS_SOFTFILTER_INFO_STRU*) arg2;

    usVar1 = pstPara1->usPrecedence;
    usVar2 = pstPara2->usPrecedence;

    /* IP Filter�����ȼ���С�������� */
    if(usVar1 > usVar2)
    {
        return 1;
    }
    else if(usVar1 < usVar2)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}


/*****************************************************************************
 �� �� ��  : NAS_ERABM_CdsFilterSort
 ��������  : IP Filter ���ȼ��ȽϺ���
 �������  : CDS_SOFTFILTER_INFO_STRU *pstFilter
             VOS_UINT32 ulNum
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��12��
    ��    ��   : lihong00150010
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID NAS_ERABM_CdsFilterSort(CDS_SOFTFILTER_INFO_STRU *pstFilter, VOS_UINT32 ulNum)
{
    /* ����C���ԵĿ⺯��qsort��pstFilter���п������� */
    qsort(pstFilter, ulNum, sizeof(CDS_SOFTFILTER_INFO_STRU), (NAS_CDS_COMP_FUNC)NAS_ERABM_CdsCompare);
}
VOS_UINT32 NAS_ERABM_CdsGenerateBaseIpv4Filter
(
    NAS_ERABM_CDS_FILTER_ARRAY     *pstCdsFilterArryay,
    const APP_ESM_IP_ADDR_STRU     *pstPdnAddr,
    VOS_UINT8                       ucEpsbId
)
{
    VOS_UINT32                      i           = 0;
    CDS_SOFTFILTER_INFO_STRU       *pstFilter   = VOS_NULL_PTR;

    /* ��Filter�������ҵ���һ�����е�Filter */
    if (pstCdsFilterArryay->ulUpLinkUsedNum >= NAS_ERABM_CDS_MAX_UL_FILTER)
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IPF Get Free Filter Failed.");
        return PS_FAIL;
    }
    pstFilter = &pstCdsFilterArryay->astUlFilter[pstCdsFilterArryay->ulUpLinkUsedNum];

    NAS_ERABM_MEM_SET(pstFilter, 0x0, sizeof(CDS_SOFTFILTER_INFO_STRU));

    /* ��дIP Filter���� */
    pstFilter->ucRabId      = ucEpsbId;
    pstFilter->enFilterType = CDS_SOFTFILTER_TYPE_IPV4;

    for(i = 0; i < APP_MAX_IPV4_ADDR_LEN; i++)
    {
        pstFilter->unLocalIpv4Addr.aucIPAddr[i] = pstPdnAddr->aucIpV4Addr[i];
        pstFilter->unLocalIpv4Mask.aucIPAddr[i] = 0xFF;
    }

    /* ����ģʽ�²�ƥ��IP��ַ */
    if (NAS_ERABM_MODE_TYPE_NORMAL == NAS_ERABM_GetMode())
    {
        pstFilter->bitOpLcIpv4AddrAndMask   = NAS_ERABM_OP_TRUE;
    }
    else
    {
        pstFilter->bitOpLcIpv4AddrAndMask   = NAS_ERABM_OP_FALSE;
    }

    /* Precedence = 256 */
    pstFilter->usPrecedence = NAS_ERABM_CDS_LOWEST_PRI;

    /* Bearer Id */
    pstFilter->ulPacketFilterId = NAS_ERABM_IPF_UL_IPV4_FILTER_ID(ucEpsbId, 0);

    /* ��������Filter��Ŀ */
    pstCdsFilterArryay->ulUpLinkUsedNum++;

    return PS_SUCC;
}

/*****************************************************************************
 �� �� ��  : NAS_ERABM_CdsGenerateBaseIpv6Filter
 ��������  : ����IPv6��ʽ�Ļ���������Filter
 �������  : const APP_ESM_IP_ADDR_STRU *pstPdnAddr,
             VOS_UINT8 ucEpsbId
 �������  : NAS_ERABM_CDS_FILTER_ARRAY     *pstCdsFilterArryay
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��12��
    ��    ��   : lihong00150010
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 NAS_ERABM_CdsGenerateBaseIpv6Filter
(
    NAS_ERABM_CDS_FILTER_ARRAY     *pstCdsFilterArryay,
    const APP_ESM_IP_ADDR_STRU     *pstPdnAddr,
    VOS_UINT8                       ucEpsbId
)
{
    VOS_UINT32                      i           = 0;
    CDS_SOFTFILTER_INFO_STRU       *pstFilter   = VOS_NULL_PTR;
    VOS_UINT8                       aucInvalidIpv6Prefix[NAS_ERABM_IPV6_PREFIX_LEN] = {0};

    /* ���IPv6ǰ׺�Ƿ���Ч�������Ч��������IPF��ֱ�ӷ��� */
    if(0 == NAS_ERABM_MEM_CMP(  pstPdnAddr->aucIpV6Addr,
                                aucInvalidIpv6Prefix,
                                NAS_ERABM_IPV6_PREFIX_LEN))
    {
        NAS_ERABM_NORM_LOG("NAS_ERABM_CdsGenerateBaseIpv6Filter IPv6 prefix invalid.");
        return PS_SUCC;
    }

    /* ��Filter�������ҵ���һ�����е�Filter */
    if (pstCdsFilterArryay->ulUpLinkUsedNum >= NAS_ERABM_CDS_MAX_UL_FILTER)
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IPF Get Free Filter Failed.");
        return PS_FAIL;
    }
    pstFilter = &pstCdsFilterArryay->astUlFilter[pstCdsFilterArryay->ulUpLinkUsedNum];

    NAS_ERABM_MEM_SET(pstFilter, 0x0, sizeof(CDS_SOFTFILTER_INFO_STRU));

    /* ��дIP Filter���� */
    pstFilter->ucRabId      = ucEpsbId;
    pstFilter->enFilterType = CDS_SOFTFILTER_TYPE_IPV6;

    /* IPv6ֻ����64bitǰ׺ */
    for(i = 0; i < (APP_MAX_IPV6_ADDR_LEN/2); i++)
    {
        pstFilter->unLocalIpv6Addr.aucIPAddr[i] = pstPdnAddr->aucIpV6Addr[i];
        pstFilter->unLocalIpv6Mask.aucIPAddr[i] = 0xFF;
    }

    /* ����ģʽ�²�ƥ��IP��ַ */
    if (NAS_ERABM_MODE_TYPE_NORMAL == NAS_ERABM_GetMode())
    {
        pstFilter->bitOpLcIpv6AddrAndMask   = NAS_ERABM_OP_TRUE;
    }
    else
    {
        pstFilter->bitOpLcIpv6AddrAndMask   = NAS_ERABM_OP_FALSE;
    }

    /* Precedence = 256 */
    pstFilter->usPrecedence = NAS_ERABM_CDS_LOWEST_PRI;

    /* Bearer Id */
    pstFilter->ulPacketFilterId = NAS_ERABM_IPF_UL_IPV6_FILTER_ID(ucEpsbId,0);

    /* ��������Filter��Ŀ */
    pstCdsFilterArryay->ulUpLinkUsedNum++;

    return PS_SUCC;
}

/*****************************************************************************
 �� �� ��  : NAS_ERABM_CdsGenerateFilterWithoutTFT
 ��������  : ������Я��TFT�����ĳ��ص�FILTER��Ϣ
 �������  : APP_ESM_IP_ADDR_STRU *pstPdnAddr
             VOS_UINT8 ucEpsbId
 �������  : NAS_ERABM_CDS_FILTER_ARRAY     *pstCdsFilterArryay
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��12��
    ��    ��   : lihong00150010
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 NAS_ERABM_CdsGenerateFilterWithoutTFT
(
    NAS_ERABM_CDS_FILTER_ARRAY     *pstCdsFilterArryay,
    const APP_ESM_IP_ADDR_STRU     *pstPdnAddr,
    VOS_UINT8                       ucEpsbId
)
{
    /* check ip type: ipv4, ipv4v6 */
    if((APP_ESM_PDN_TYPE_IPV4 == pstPdnAddr->ucIpType)||
       (APP_ESM_PDN_TYPE_IPV4_IPV6 == pstPdnAddr->ucIpType))
    {
        if(PS_FAIL == NAS_ERABM_CdsGenerateBaseIpv4Filter(pstCdsFilterArryay, pstPdnAddr, ucEpsbId))
        {
            NAS_ERABM_ERR_LOG("NAS_ERABM_CdsGenerateFilterWithoutTFT: Generate Ipv4 Filter Fail.");
            return PS_FAIL;
        }
    }

    /* check ip type: ipv6, ipv4v6 */
    if((APP_ESM_PDN_TYPE_IPV6 == pstPdnAddr->ucIpType)||
       (APP_ESM_PDN_TYPE_IPV4_IPV6 == pstPdnAddr->ucIpType))
    {
        if(PS_FAIL ==  NAS_ERABM_CdsGenerateBaseIpv6Filter(pstCdsFilterArryay, pstPdnAddr, ucEpsbId))
        {
            NAS_ERABM_ERR_LOG("NAS_ERABM_CdsGenerateFilterWithoutTFT: Generate Ipv6 Filter Fail.");
            return PS_FAIL;
        }
    }

    return PS_SUCC;
}

/*****************************************************************************
 �� �� ��  : NAS_ERABM_CdsConvertOtherPartToFilter
 ��������  : �� TFT�����г�Դ��ַ/Ŀ�ĵ�ַ/FlowLabel ����V4V6��ز���֮���
             ����ת��ΪIP Filter��ʽ
 �������  : const ESM_ERABM_TFT_PF_STRU *pstTftPf,
             CDS_SOFTFILTER_INFO_STRU               *pstFilter
 �������  : ��
 �� �� ֵ  : VOS_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��12��
    ��    ��   : lihong00150010
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_VOID NAS_ERABM_CdsConvertOtherPartToFilter
(
    const ESM_ERABM_TFT_PF_STRU        *pstTftPf,
    CDS_SOFTFILTER_INFO_STRU           *pstFilter
)
{
    /* Protocol id */
    pstFilter->bitOpProtocolId          = pstTftPf->bitOpProtocolId;
    pstFilter->ucProtocolId             = pstTftPf->ucProtocolId;

    /* local port */
    pstFilter->bitOpSingleLocalPort     = pstTftPf->bitOpSingleLocalPort;
    pstFilter->usSingleLcPort           = pstTftPf->usSingleLcPort;

    /* local port range, low to high */
    pstFilter->bitOpLocalPortRange      = pstTftPf->bitOpLocalPortRange;
    pstFilter->usLcPortLowLimit         = pstTftPf->usLcPortLowLimit;
    pstFilter->usLcPortHighLimit        = pstTftPf->usLcPortHighLimit;

    /* remote port */
    pstFilter->bitOpSingleRemotePort    = pstTftPf->bitOpSingleRemotePort;
    pstFilter->usSingleRmtPort          = pstTftPf->usSingleRmtPort;

    /* remote port range , low to high */
    pstFilter->bitOpRemotePortRange     = pstTftPf->bitOpRemotePortRange;
    pstFilter->usRmtPortLowLimit        = pstTftPf->usRmtPortLowLimit;
    pstFilter->usRmtPortHighLimit       = pstTftPf->usRmtPortHighLimit;

    /* type of server and mask for ipv4, traffic class and mask for ipv6*/
    pstFilter->bitOpTypeOfService       = pstTftPf->bitOpTypeOfService;
    pstFilter->ucTypeOfService          = pstTftPf->ucTypeOfService;
    pstFilter->ucTypeOfServiceMask      = pstTftPf->ucTypeOfServiceMask;

    /* spi for esp or ah ? */
    pstFilter->bitOpSecuParaIndex       = pstTftPf->bitOpSecuParaIndex;
    pstFilter->ulSecuParaIndex          = pstTftPf->ulSecuParaIndex;

    return;
}


/*****************************************************************************
 �� �� ��  : NAS_ERABM_CdsConvertIpv4TftToFilter
 ��������  : ��IPv4��ʽTFTת��ΪIP filter��ʽ
 �������  : const APP_ESM_IP_ADDR_STRU *pstPdnAddr,
             const ESM_ERABM_TFT_PF_STRU *pstTftPf,
             VOS_UINT8            ucEpsbId
 �������  : NAS_ERABM_CDS_FILTER_ARRAY     *pstCdsFilterArryay,
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��12��
    ��    ��   : lihong00150010
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 NAS_ERABM_CdsConvertIpv4TftToFilter
(
    NAS_ERABM_CDS_FILTER_ARRAY     *pstCdsFilterArryay,
    const APP_ESM_IP_ADDR_STRU     *pstPdnAddr,
    const ESM_ERABM_TFT_PF_STRU    *pstTftPf,
    VOS_UINT8                       ucEpsbId
)
{
    VOS_UINT32                          i               = 0;
    CDS_SOFTFILTER_INFO_STRU           *pstFilter       = VOS_NULL_PTR;

    /* ��Filter�������ҵ���һ�����е�Filter */
    if (pstCdsFilterArryay->ulUpLinkUsedNum >= NAS_ERABM_CDS_MAX_UL_FILTER)
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IPF Get Free Filter Failed.");
        return PS_FAIL;
    }
    pstFilter = &pstCdsFilterArryay->astUlFilter[pstCdsFilterArryay->ulUpLinkUsedNum];

    NAS_ERABM_MEM_SET(pstFilter, 0x0, sizeof(CDS_SOFTFILTER_INFO_STRU));

    /* source address and mask for ipv4 */
    for(i = 0;i < APP_MAX_IPV4_ADDR_LEN; i++)
    {
        pstFilter->unLocalIpv4Addr.aucIPAddr[i] = pstPdnAddr->aucIpV4Addr[i];
        pstFilter->unLocalIpv4Mask.aucIPAddr[i] = 0xFF;
    }

    /* ����ģʽ�²�ƥ��IP��ַ */
    if (NAS_ERABM_MODE_TYPE_NORMAL == NAS_ERABM_GetMode())
    {
        pstFilter->bitOpLcIpv4AddrAndMask   = NAS_ERABM_OP_TRUE;
    }
    else
    {
        pstFilter->bitOpLcIpv4AddrAndMask   = NAS_ERABM_OP_FALSE;
    }

    /* destination address and mask for ipv4 */
    if(NAS_ERABM_OP_TRUE == pstTftPf->bitOpRmtIpv4AddrAndMask)
    {
        for(i = 0;i < APP_MAX_IPV4_ADDR_LEN; i++)
        {
            pstFilter->unRmtIpv4Addr.aucIPAddr[i]   = pstTftPf->aucRmtIpv4Address[i];
            pstFilter->unRmtIpv4Mask.aucIPAddr[i]   = pstTftPf->aucRmtIpv4Mask[i];
        }
        pstFilter->bitOpRmtIpv4AddrAndMask   = NAS_ERABM_OP_TRUE;
    }

    pstFilter->enFilterType = CDS_SOFTFILTER_TYPE_IPV4;

    /* ��TFT�����е����ಿ��ת��ΪIP Filter��ʽ */
    NAS_ERABM_CdsConvertOtherPartToFilter(pstTftPf, pstFilter);

    /* Filter ID */
    pstFilter->ulPacketFilterId = NAS_ERABM_IPF_UL_IPV4_FILTER_ID(  ucEpsbId,
                                                                    pstTftPf->ucNwPacketFilterId);

    /* Filter Precedence and Bearer Id */
    pstFilter->usPrecedence = pstTftPf->ucPrecedence;
    pstFilter->ucRabId      = ucEpsbId;

    /* ��������Filter��Ŀ */
    pstCdsFilterArryay->ulUpLinkUsedNum++;

    return PS_SUCC;
}

/*****************************************************************************
 �� �� ��  : NAS_ERABM_CdsConvertIpv6TftToFilter
 ��������  : ��IPv6��ʽTFTת��ΪIP Filter��ʽ
 �������  : const APP_ESM_IP_ADDR_STRU *pstPdnAddr,
             const ESM_ERABM_TFT_PF_STRU *pstTftPf,
             VOS_UINT8            ucEpsbId
 �������  : NAS_ERABM_CDS_FILTER_ARRAY     *pstCdsFilterArryay,
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��12��
    ��    ��   : lihong00150010
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 NAS_ERABM_CdsConvertIpv6TftToFilter
(
    NAS_ERABM_CDS_FILTER_ARRAY     *pstCdsFilterArryay,
    const APP_ESM_IP_ADDR_STRU     *pstPdnAddr,
    const ESM_ERABM_TFT_PF_STRU    *pstTftPf,
    VOS_UINT8                       ucEpsbId
)
{
    VOS_UINT32                          i               = 0;
    CDS_SOFTFILTER_INFO_STRU           *pstFilter       = VOS_NULL_PTR;

    /* ��Filter�������ҵ���һ�����е�Filter */
    if (pstCdsFilterArryay->ulUpLinkUsedNum >= NAS_ERABM_CDS_MAX_UL_FILTER)
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_IPF Get Free Filter Failed.");
        return PS_FAIL;
    }
    pstFilter = &pstCdsFilterArryay->astUlFilter[pstCdsFilterArryay->ulUpLinkUsedNum];

    NAS_ERABM_MEM_SET(pstFilter, 0x0, sizeof(CDS_SOFTFILTER_INFO_STRU));

    /* source address and mask for ipv6 */
    /* IPv6ֻ����64bitǰ׺ */
    for(i = 0;i < (APP_MAX_IPV6_ADDR_LEN/2); i++)
    {
        pstFilter->unLocalIpv6Addr.aucIPAddr[i] = pstPdnAddr->aucIpV6Addr[i];
        pstFilter->unLocalIpv6Mask.aucIPAddr[i] = 0xFF;
    }

    /* ����ģʽ�²�ƥ��IP��ַ */
    if (NAS_ERABM_MODE_TYPE_NORMAL == NAS_ERABM_GetMode())
    {
        pstFilter->bitOpLcIpv6AddrAndMask   = NAS_ERABM_OP_TRUE;
    }
    else
    {
        pstFilter->bitOpLcIpv6AddrAndMask   = NAS_ERABM_OP_FALSE;
    }

    /* destination address and mask for ipv6 */
    if(NAS_ERABM_OP_TRUE == pstTftPf->bitOpRmtIpv6AddrAndMask)
    {
        for(i = 0;i < APP_MAX_IPV6_ADDR_LEN; i++)
        {
            pstFilter->unRmtIpv6Addr.aucIPAddr[i]   = pstTftPf->aucRmtIpv6Address[i];
            pstFilter->unRmtIpv6Mask.aucIPAddr[i]   = pstTftPf->aucRmtIpv6Mask[i];
        }
        pstFilter->bitOpRmtIpv6AddrAndMask      = NAS_ERABM_OP_TRUE;
    }

    /* flowlabel for ipv6 */
    if(NAS_ERABM_OP_TRUE == pstTftPf->bitOpFlowLabelType)
    {
        pstFilter->ulFlowLabelType      = pstTftPf->ulFlowLabelType;
        pstFilter->bitOpFlowLabelType   = NAS_ERABM_OP_TRUE;
    }

    pstFilter->enFilterType = CDS_SOFTFILTER_TYPE_IPV6;

    /* ��TFT�����е����ಿ��ת��ΪIP Filter��ʽ */
    NAS_ERABM_CdsConvertOtherPartToFilter(pstTftPf, pstFilter);

    /* Filter ID */
    pstFilter->ulPacketFilterId = NAS_ERABM_IPF_UL_IPV4_FILTER_ID(  ucEpsbId,
                                                                    pstTftPf->ucNwPacketFilterId);

    /* Filter Precedence and Bearer Id */
    pstFilter->usPrecedence = pstTftPf->ucPrecedence;
    pstFilter->ucRabId      = ucEpsbId;

    /* ��������Filter��Ŀ */
    pstCdsFilterArryay->ulUpLinkUsedNum++;

    return PS_SUCC;
}


/*****************************************************************************
 �� �� ��  : NAS_ERABM_CdsConvertTftToFilter
 ��������  : ��ָ������TFT����ת��ΪIP Filter��ʽ
 �������  : const APP_ESM_IP_ADDR_STRU   *pstPdnAddr,
             const ESM_ERABM_TFT_PF_STRU   *pstTftPf,
             VOS_UINT8                     ucEpsbId
 �������  : NAS_ERABM_CDS_FILTER_ARRAY     *pstCdsFilterArryay,
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��12��
    ��    ��   : lihong00150010
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 NAS_ERABM_CdsConvertTftToFilter
(
    NAS_ERABM_CDS_FILTER_ARRAY     *pstCdsFilterArryay,
    const APP_ESM_IP_ADDR_STRU     *pstPdnAddr,
    const ESM_ERABM_TFT_PF_STRU    *pstTftPf,
    VOS_UINT8                       ucEpsbId
)
{
    VOS_UINT32                  ulFilterType    = NAS_ERABM_IPF_IPTYPE_IPV4;

    /* ��Ҫ���ȼ��TFT��Ӧ��IP���� */
    ulFilterType = NAS_ERABM_IpfCheckFilterType(pstTftPf, pstPdnAddr->ucIpType);

    /* ���ȸ��ݲ�ͬ��������IP Filter */
    if((NAS_ERABM_IPF_IPTYPE_IPV4 == ulFilterType)||
       (NAS_ERABM_IPF_IPTYPE_IPV4V6 == ulFilterType))
    {
        if(PS_FAIL == NAS_ERABM_CdsConvertIpv4TftToFilter(  pstCdsFilterArryay,
                                                            pstPdnAddr,
                                                            pstTftPf,
                                                            ucEpsbId))
        {
            NAS_ERABM_ERR_LOG("NAS_ERABM_CdsConvertTftToFilter: Convert Ipv4 Filter Fail.");
            return PS_FAIL;
        }
    }

    if((NAS_ERABM_IPF_IPTYPE_IPV6 == ulFilterType)||
       (NAS_ERABM_IPF_IPTYPE_IPV4V6 == ulFilterType))
    {
        if(PS_FAIL == NAS_ERABM_CdsConvertIpv6TftToFilter(  pstCdsFilterArryay,
                                                            pstPdnAddr,
                                                            pstTftPf,
                                                            ucEpsbId))
        {
            NAS_ERABM_ERR_LOG("NAS_ERABM_CdsConvertTftToFilter: Convert Ipv6 Filter Fail.");
            return PS_FAIL;
        }
    }

    return PS_SUCC;
}

/*****************************************************************************
 �� �� ��  : NAS_ERABM_CdsGenerateFilterWithTFT
 ��������  : �������ص�����TFT��������ת��ΪIP Filter
 �������  : const APP_ESM_IP_ADDR_STRU *pstPdnAddr,
             VOS_UINT8 ucEpsbId
 �������  : NAS_ERABM_CDS_FILTER_ARRAY     *pstCdsFilterArryay,
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��12��
    ��    ��   : lihong00150010
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 NAS_ERABM_CdsGenerateFilterWithTFT
(
    NAS_ERABM_CDS_FILTER_ARRAY     *pstCdsFilterArryay,
    const APP_ESM_IP_ADDR_STRU     *pstPdnAddr,
    VOS_UINT8                       ucEpsbId
)
{
    VOS_UINT32                      ulTftPfCnt      = NAS_ERABM_NULL;
    ESM_ERABM_TFT_PF_STRU           *pstTftInfo      = VOS_NULL_PTR;

    for(ulTftPfCnt = 0; ulTftPfCnt < NAS_ERABM_GetEpsbTftPfNum(ucEpsbId); ulTftPfCnt++)
    {
        pstTftInfo = NAS_ERABM_GetEpsbTftAddr(ucEpsbId, ulTftPfCnt);

        /* ����Ҫ��������TFT���� */
        if (APP_ESM_TRANSFER_DIRECTION_DOWNLINK == pstTftInfo->enDirection)
        {
            continue;
        }

        /* ��TFT����ת��ΪIP Filter��ʽ */
        if(PS_FAIL == NAS_ERABM_CdsConvertTftToFilter(pstCdsFilterArryay, pstPdnAddr, pstTftInfo, ucEpsbId))
        {
            NAS_ERABM_ERR_LOG("NAS_ERABM_CdsGenerateFilterWithTFT: Convert TFT to IPF Filter Failed.");
            return PS_FAIL;
        }
    }

    return PS_SUCC;
}


/*****************************************************************************
 �� �� ��  : NAS_ERABM_CdsGenerateFilters
 ��������  : �������г��أ�����IP FIlter����
 �������  : NAS_ERABM_CDS_FILTER_ARRAY     *pstCdsFilterArryay
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��12��
    ��    ��   : lihong00150010
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 NAS_ERABM_CdsGenerateFilters
(
    NAS_ERABM_CDS_FILTER_ARRAY     *pstCdsFilterArryay
)
{
    VOS_UINT8                       ucEpsbId        = NAS_ERABM_NULL;
    APP_ESM_IP_ADDR_STRU           *pstPdnAddr      = VOS_NULL_PTR;

    /* ��յ�ǰ����Filter */
    pstCdsFilterArryay->ulUpLinkUsedNum = 0;

    for(ucEpsbId = NAS_ERABM_MIN_EPSB_ID; ucEpsbId <= NAS_ERABM_MAX_EPSB_ID; ucEpsbId++)
    {
        if (NAS_ERABM_EPSB_ACTIVE != NAS_ERABM_GetEpsbStateInfo(ucEpsbId))
        {
            /* ����δ������� */
            continue;
        }

        /* ����PDN���ӵ�ַ */
        pstPdnAddr = NAS_ERABM_GetEpsbPdnAddr(ucEpsbId);

        /* �ȴ���δ����TFT������Ĭ�ϳ��� */
        if(PS_FALSE == NAS_ERABM_IsExistUplinkPfinTft(ucEpsbId))
        {
            if(PS_FAIL == NAS_ERABM_CdsGenerateFilterWithoutTFT(pstCdsFilterArryay,
                                                                pstPdnAddr,
                                                                ucEpsbId))
            {
                NAS_ERABM_ERR_LOG("NAS_ERABM_CDS Generate Default Bearer Filter Failed.");
                return PS_FAIL;
            }
        }
        else
        {
            /* ����ר�г��ػ�������TFT������Ĭ�ϳ���,��Ҫ����TFT Filter����ת�� */
            if(PS_FAIL == NAS_ERABM_CdsGenerateFilterWithTFT(pstCdsFilterArryay, pstPdnAddr, ucEpsbId))
            {
                NAS_ERABM_ERR_LOG("NAS_ERABM_CDS Generate Dedicate Bearer Filter Failed.");
                return PS_FAIL;
            }
        }
    }

    return PS_SUCC;
}


/*****************************************************************************
 �� �� ��  : NAS_ERABM_CdsConfigUlFilter
 ��������  : ������������TFT, ����IP Filter, �����õ�CDS
 �������  : ��
 �� �� ֵ  : VOS_UINT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��7��12��
    ��    ��   : lihong00150010
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32 NAS_ERABM_CdsConfigUlFilter( VOS_VOID )
{
    NAS_ERABM_CDS_FILTER_ARRAY     *pstCdsFilterArryay = VOS_NULL_PTR;

    /*��ӡ����ú���*/
    NAS_ERABM_INFO_LOG("NAS_ERABM_CdsConfigUlFilter is entered.");

    pstCdsFilterArryay = (NAS_ERABM_CDS_FILTER_ARRAY*)NAS_ERABM_MEM_ALLOC(sizeof(NAS_ERABM_CDS_FILTER_ARRAY));
    if (VOS_NULL_PTR == pstCdsFilterArryay)
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_CdsConfigUlFilter: mem alloc fail!.");
        return PS_FAIL;
    }

    /* ��������TFT������IP Filter */
    if(PS_FAIL == NAS_ERABM_CdsGenerateFilters(pstCdsFilterArryay))
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_CdsConfigUlFilter: Generate All Filters Fail.");

        NAS_ERABM_MEM_FREE(pstCdsFilterArryay);
        return PS_FAIL;
    }

    /*Coverity �澯���� 2013-9-26 Begin*/
    NAS_ERABM_CdsFilterSort(    &(pstCdsFilterArryay->astUlFilter[0]),
                                pstCdsFilterArryay->ulUpLinkUsedNum);
    /*Coverity �澯���� 2013-9-26 End*/

    /* ����������������ø�CDS */
    if(PS_SUCC != CDS_SetUlSoftFilter(  &(pstCdsFilterArryay->astUlFilter[0]),
                                        pstCdsFilterArryay->ulUpLinkUsedNum))
    {
        NAS_ERABM_ERR_LOG("NAS_ERABM_CdsConfigUlFilter: CDS_SetUlFilter Fail.");

        NAS_ERABM_MEM_FREE(pstCdsFilterArryay);
        return PS_FAIL;
    }

    NAS_ERABM_MEM_FREE(pstCdsFilterArryay);
    return PS_SUCC;
}
/*lint +e961*/
/*lint +e960*/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


