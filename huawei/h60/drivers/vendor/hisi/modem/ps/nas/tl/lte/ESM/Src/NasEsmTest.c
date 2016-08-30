



/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include  "NasEsmTest.h"
#include  "NasLmmTest.h"
#include  "NasERabmTest.h"
#if (VOS_OS_VER != VOS_WIN32)
#include  "LPsOm.h"
#endif

/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASESMTEST_C
/*lint +e767*/


/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/
/* ESM״̬����ʱ����ӡ���� */
NAS_ESM_TIMER_PRINT_LIST_STRU g_astEsmTimerArray[] =
{
    {   TI_NAS_ESM_T3480         ,
            "��ʱ��T3480��Ϣ����:"       },
    {   TI_NAS_ESM_T3481         ,
            "��ʱ��T3481��Ϣ����:"       },
    {   TI_NAS_ESM_T3482         ,
            "��ʱ��T3482��Ϣ����:"       },
    {   TI_NAS_ESM_T3492         ,
            "��ʱ��T3492��Ϣ����:"       },
    {   TI_NAS_ESM_WAIT_APP_CNF  ,
            "�ȴ�APP��Ӧ��ʱ����Ϣ����:" },
    {   TI_NAS_ESM_ATTACH_BEARER_REEST,
            "�ȴ�ע������ؽ�Ӧ��ʱ����Ϣ����:" },
};

/*****************************************************************************
  3 Function
*****************************************************************************/
/*****************************************************************************
 Function Name   : NAS_ESM_DebugInit
 Description     : ESM�����ʼ��
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-03-10  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_DebugInit(VOS_VOID)
{
    return ;
}


VOS_VOID NAS_ESM_ShowQos(const APP_ESM_EPS_QOS_INFO_STRU *pstSdfQos)
{

    vos_printf("QoS��Ϣ����:\r\n");
    vos_printf("%-30s : %d\r\n","QCIֵ",pstSdfQos->ucQCI);
    vos_printf("%-30s : %dkbps\r\n","�����������     ulULMaxRate",pstSdfQos->ulULMaxRate);
    vos_printf("%-30s : %dkbps\r\n","�����������     ulDLMaxRate",pstSdfQos->ulDLMaxRate);
    vos_printf("%-30s : %dkbps\r\n","���б�֤������� ulULGMaxRate",pstSdfQos->ulULGMaxRate);
    vos_printf("%-30s : %dkbps\r\n","���б�֤������� ulDLGMaxRate",pstSdfQos->ulDLGMaxRate);
}

/*****************************************************************************
 Function Name   : NAS_ESM_ShowCidInfoQos
 Description     : ��ӡCID��Ϣ�е�QOS��Ϣ
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-1-18  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ESM_ShowCidInfoQos(const NAS_ESM_SDF_PARA_STRU *pstSdfPara)
{
    if ( NAS_ESM_OP_TRUE == pstSdfPara->bitOpSdfQos )
    {
        NAS_ESM_ShowQos(&(pstSdfPara->stSdfQosInfo));
    }
    else
    {
        vos_printf("%-30s\r\n","��CID������SDF QoS");
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_ShowEpsQos
 Description     : ��ӡESP���ص�QOS��Ϣ
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-1-18  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_ShowEpsQos( const NAS_ESM_EPSB_CNTXT_INFO_STRU *pstEpsbInfo )
{
    if(NAS_ESM_OP_TRUE == pstEpsbInfo->bitOpEpsQos )
    {
        NAS_ESM_ShowQos(&(pstEpsbInfo->stEpsQoSInfo.stQosInfo));
    }
    else
    {
        vos_printf("�ó��ز�����QoS��Ϣ\r\n");
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_ShowLinkEpsbId
 Description     : ��ӡ����ȱʡ���غ�
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-1-18  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ESM_ShowLinkEpsbId( VOS_UINT32 ulLinkEpsbId )
{
    if ( (ulLinkEpsbId < NAS_ESM_MIN_EPSB_ID) || (ulLinkEpsbId > NAS_ESM_MAX_EPSB_ID) )
    {
        vos_printf("%-30s\r\n","�����ڹ���EPSBID");
    }
    else
    {
        vos_printf("%-30s : %d\r\n","����EPSBID",ulLinkEpsbId);
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_ShowBearerManagementType
 Description     : ��ӡ���ع���ʽ
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-1-18  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ESM_ShowBearerManagementType(  )
{
    NAS_ESM_PDP_MANAGE_INFO_STRU stBearerManType = pg_stNasEsmEntity->stEsmBearerManageInfo;

    if ((stBearerManType.enMode != NAS_ESM_BEARER_MANAGE_MODE_AUTO)
        && (stBearerManType.enMode != NAS_ESM_BEARER_MANAGE_MODE_MANUAL))
    {
        vos_printf("���ع���ʽ�޷�ʶ��!\r\n");
        return ;
    }

    if (stBearerManType.enMode == NAS_ESM_BEARER_MANAGE_MODE_MANUAL)
    {
        vos_printf("%-30s : %s\r\n","���ع���ʽ","�ֶ�");
        return ;
    }

    if (stBearerManType.enType == NAS_ESM_BEARER_MANAGE_TYPE_ACCEPT)
    {
        vos_printf("%-30s : %s\r\n","���ع���ʽ","�Զ�����");
    }
    else if (stBearerManType.enType == NAS_ESM_BEARER_MANAGE_TYPE_REJ)
    {
        vos_printf("%-30s : %s\r\n","���ع���ʽ","�Զ��ܾ�");
    }
    else
    {
        vos_printf("���ع���ʽ�޷�ʶ��!\r\n");
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_ShowEpsSdfId
 Description     : ��ӡ���ذ�����SDF��
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-1-18  Draft Enact

*****************************************************************************/
/*lint -e960*/
VOS_VOID NAS_ESM_ShowEpsSdfId( const NAS_ESM_EPSB_CNTXT_INFO_STRU *pstEpsbInfo )
{
    VOS_UINT32                          ulBitCId    = NAS_ESM_NULL;
    VOS_UINT8                           ucLoop      = NAS_ESM_NULL;
    PS_BOOL_ENUM_UINT8                  enIsFirSdf  = PS_TRUE;

    if(NAS_ESM_OP_TRUE != pstEpsbInfo->bitOpCId )
    {
         vos_printf("�˳����²�����SDF\r\n");
         return ;
    }

    ulBitCId = pstEpsbInfo->ulBitCId;

    vos_printf("%-30s : ","�ó��ذ�����SDF��CID��");

    for (ucLoop = NAS_ESM_MIN_CID; ucLoop <= NAS_ESM_MAX_CID; ucLoop++)
    {
        if (NAS_ESM_OP_TRUE != ((ulBitCId >> ucLoop) & NAS_ESM_BIT_0))
        {
            continue;
        }

        if (PS_TRUE == enIsFirSdf)
        {
            vos_printf("%d",ucLoop);
            enIsFirSdf = PS_FALSE;
        }
        else
        {
            vos_printf(",%d",ucLoop);
        }
    }

    vos_printf("\r\n");
}
VOS_VOID NAS_ESM_ShowLinkCid(const NAS_ESM_SDF_PARA_STRU *pstSdfPara)
{
    if (NAS_ESM_OP_TRUE == pstSdfPara->bitOpLinkdCId)
    {
        vos_printf("%-30s : %d\r\n","����cid",pstSdfPara->ulLinkdCId);
    }
    else
    {
        vos_printf("�����ڹ���cid\r\n");
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_ShowCidInfoTft
 Description     : ��ӡTFT��Ϣ
 Input           : pstBearerPara------------
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2010-01-18  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_ShowTft
(
    VOS_UINT32                          ulPfNum,
    NAS_ESM_CONTEXT_TFT_STRU		   *pstTft
)
{
    NAS_ESM_CONTEXT_TFT_STRU           *pstTftTmp   = VOS_NULL_PTR;
    VOS_UINT32                          ulLoop      = NAS_ESM_NULL;

    if ( NAS_ESM_NULL == ulPfNum )
    {
        vos_printf("������PF��Ϣ\r\n");
        return;
    }
    else
    {
        vos_printf("����%d��PF\r\n",ulPfNum);
    }

    for ( ulLoop = 0 ; ulLoop < ulPfNum; ulLoop++ )
    {
        pstTftTmp = pstTft + ulLoop;

        vos_printf("*******************��%d��PF��Ϣ*******************\r\n",(ulLoop + 1));

        vos_printf("%-30s : %d\r\n","�������ı���ID",pstTftTmp->ucPacketFilterId);
        vos_printf("%-30s : %d\r\n","���������������ID",pstTftTmp->ucNwPacketFilterId);

        vos_printf("%-30s : %d\r\n","PF�����ȼ�", pstTftTmp->ucPrecedence);

        if ( NAS_ESM_OP_TRUE == pstTftTmp->bitOpRmtIpv4AddrAndMask )
        {
            vos_printf("%-30s : %d.%d.%d.%d\r\n","PF��Ŀ��IPv4", pstTftTmp->aucRmtIpv4Address[0],
                                      pstTftTmp->aucRmtIpv4Address[1],
                                      pstTftTmp->aucRmtIpv4Address[2],
                                      pstTftTmp->aucRmtIpv4Address[3]);

            vos_printf("%-30s : %d.%d.%d.%d\r\n","PF������", pstTftTmp->aucRmtIpv4Mask[0],
                                      pstTftTmp->aucRmtIpv4Mask[1],
                                      pstTftTmp->aucRmtIpv4Mask[2],
                                      pstTftTmp->aucRmtIpv4Mask[3]);
        }

        if ( NAS_ESM_OP_TRUE == pstTftTmp->bitOpRmtIpv6AddrAndMask )
        {
            vos_printf("%-30s : %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\r\n","PF��Ŀ��IPv6",
                                      pstTftTmp->aucRmtIpv6Address[0],
                                      pstTftTmp->aucRmtIpv6Address[1],
                                      pstTftTmp->aucRmtIpv6Address[2],
                                      pstTftTmp->aucRmtIpv6Address[3],
                                      pstTftTmp->aucRmtIpv6Address[4],
                                      pstTftTmp->aucRmtIpv6Address[5],
                                      pstTftTmp->aucRmtIpv6Address[6],
                                      pstTftTmp->aucRmtIpv6Address[7],
                                      pstTftTmp->aucRmtIpv6Address[8],
                                      pstTftTmp->aucRmtIpv6Address[9],
                                      pstTftTmp->aucRmtIpv6Address[10],
                                      pstTftTmp->aucRmtIpv6Address[11],
                                      pstTftTmp->aucRmtIpv6Address[12],
                                      pstTftTmp->aucRmtIpv6Address[13],
                                      pstTftTmp->aucRmtIpv6Address[14],
                                      pstTftTmp->aucRmtIpv6Address[15]);

            vos_printf("%-30s : %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\r\n","PF������",
                                      pstTftTmp->aucRmtIpv6Mask[0],
                                      pstTftTmp->aucRmtIpv6Mask[1],
                                      pstTftTmp->aucRmtIpv6Mask[2],
                                      pstTftTmp->aucRmtIpv6Mask[3],
                                      pstTftTmp->aucRmtIpv6Mask[4],
                                      pstTftTmp->aucRmtIpv6Mask[5],
                                      pstTftTmp->aucRmtIpv6Mask[6],
                                      pstTftTmp->aucRmtIpv6Mask[7],
                                      pstTftTmp->aucRmtIpv6Mask[8],
                                      pstTftTmp->aucRmtIpv6Mask[9],
                                      pstTftTmp->aucRmtIpv6Mask[10],
                                      pstTftTmp->aucRmtIpv6Mask[11],
                                      pstTftTmp->aucRmtIpv6Mask[12],
                                      pstTftTmp->aucRmtIpv6Mask[13],
                                      pstTftTmp->aucRmtIpv6Mask[14],
                                      pstTftTmp->aucRmtIpv6Mask[15]);
        }

        if ( NAS_ESM_OP_TRUE == pstTftTmp->bitOpLocalPortRange )
        {
            vos_printf("%-30s : %d\r\n","PF�Ľ��˶˿���Сֵ", pstTftTmp->usLcPortLowLimit);
            vos_printf("%-30s : %d\r\n","PF�Ľ��˶˿����ֵ", pstTftTmp->usLcPortHighLimit);
        }

        if ( NAS_ESM_OP_TRUE == pstTftTmp->bitOpRemotePortRange)
        {
            vos_printf("%-30s : %d\r\n","PF��Զ�˶˿���Сֵ", pstTftTmp->usRmtPortLowLimit);
            vos_printf("%-30s : %d\r\n","PF��Զ�˶˿����ֵ", pstTftTmp->usRmtPortHighLimit);
        }

        if (NAS_ESM_OP_TRUE == pstTftTmp->bitOpProtocolId)
        {
            vos_printf("%-30s : %d\r\n","PF��Э��汾��", pstTftTmp->ucProtocolId);
        }

        if ( NAS_ESM_OP_TRUE == pstTftTmp->bitOpSingleLocalPort)
        {
            vos_printf("%-30s : %d\r\n","PF�Ľ��˶˿ں�", pstTftTmp->usSingleLcPort);
        }

        if ( NAS_ESM_OP_TRUE == pstTftTmp->bitOpSingleRemotePort)
        {
            vos_printf("%-30s : %d\r\n","PF��Զ�˶˿ں�", pstTftTmp->usSingleRmtPort);
        }

    }
}


VOS_VOID NAS_ESM_ShowPdnTpe(const NAS_ESM_SDF_PARA_STRU *pstSdfPara)
{
    if (NAS_ESM_OP_TRUE == pstSdfPara->bitOpPdnType)
    {
        if(APP_ESM_PDN_TYPE_IPV4 == pstSdfPara->enPdnType)
        {
            vos_printf("%-30s : %s\r\n","PDN����","ipv4");
        }
        else if(APP_ESM_PDN_TYPE_IPV6 == pstSdfPara->enPdnType)
        {
            vos_printf("%-30s : %s\r\n","PDN����","ipv6");
        }
        else if(APP_ESM_PDN_TYPE_IPV4_IPV6 == pstSdfPara->enPdnType)
        {
            vos_printf("%-30s : %s\r\n","PDN����","ipv4v6");
        }
        else
        {
            vos_printf("PDN���ʹ���\r\n");
        }
    }
    else
    {
        vos_printf("������PDN����\r\n");
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_ShowIpAllocType
 Description     : ��ӡIP��Ϣ��ȡ��ʽ
 Input           : pstSdfPara
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-1-18  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_ESM_ShowIpAllocType(const NAS_ESM_SDF_PARA_STRU *pstSdfPara)
{
    if (NAS_ESM_OP_TRUE == pstSdfPara->bitIpv4AddrAllocType)
    {
        if (pstSdfPara->enIpv4AddrAllocType == APP_ESM_IPV4_ADDR_ALLOC_TYPE_NAS_SIGNALING)
        {
            vos_printf("%-30s : %s\r\n","IP��ȡ��ʽ","NAS����");
        }
        else
        {
            vos_printf("%-30s : %s\r\n","IP��ȡ��ʽ","DHCP");
        }
    }
    else
    {
        vos_printf("������IP��ȡ��ʽ\r\n");
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_ShowEpsApnAmbr
 Description     : ��ӡ������Ϣ�е�APN-AMBR��Ϣ
 Input           : pstEpsbInfo
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010    2010-01-19  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_ShowEpsApnAmbr( const NAS_ESM_EPSB_CNTXT_INFO_STRU *pstEpsbInfo )
{
    if ((PS_TRUE == NAS_ESM_IsDefaultEpsBearerType(pstEpsbInfo->enBearerCntxtType))
        && (NAS_ESM_OP_TRUE == pstEpsbInfo->bitOpApnAmbr))
    {
        vos_printf("APN-AMBR��Ϣ����\r\n");
        vos_printf("%-30s : %d\r\n","DLApnAmbr",pstEpsbInfo->stApnAmbrInfo.ucDLApnAmbr);
        vos_printf("%-30s : %d\r\n","DLApnAmbrExt",pstEpsbInfo->stApnAmbrInfo.ucDLApnAmbrExt);
        vos_printf("%-30s : %d\r\n","DLApnAmbrExt2",pstEpsbInfo->stApnAmbrInfo.ucDLApnAmbrExt2);
        vos_printf("%-30s : %d\r\n","ULApnAmbr",pstEpsbInfo->stApnAmbrInfo.ucULApnAmbr);
        vos_printf("%-30s : %d\r\n","ULApnAmbrExt",pstEpsbInfo->stApnAmbrInfo.ucULApnAmbrExt);
        vos_printf("%-30s : %d\r\n","ULApnAmbrExt2",pstEpsbInfo->stApnAmbrInfo.ucULApnAmbrExt2);
    }
    else
    {
        vos_printf("������APN-AMBR\r\n");
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_ShowEpsApn
 Description     : ��ӡ������Ϣ�е�APN��Ϣ
 Input           : pstEpsbInfo
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010    2010-01-19  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_ShowEpsApn( const NAS_ESM_EPSB_CNTXT_INFO_STRU *pstEpsbInfo )
{
    VOS_UINT8                           aucApn[APP_MAX_APN_LEN];

    if ((PS_TRUE == NAS_ESM_IsDefaultEpsBearerType(pstEpsbInfo->enBearerCntxtType))
        && (NAS_ESM_OP_TRUE == pstEpsbInfo->bitOpApn))
    {
        NAS_ESM_MEM_CPY(aucApn, pstEpsbInfo->stApnInfo.aucApnName, pstEpsbInfo->stApnInfo.ucApnLen);
        aucApn[pstEpsbInfo->stApnInfo.ucApnLen] = '\0';

        vos_printf("����APN,APN��Ϣ����\r\n");
        vos_printf("%-30s : %d\r\n","APN����",pstEpsbInfo->stApnInfo.ucApnLen);
        vos_printf("%-30s : %s\r\n","APN����",aucApn);
    }
    else
    {
        vos_printf("������APN\r\n");
    }
}


VOS_VOID NAS_ESM_ShowCidInfoApn(const NAS_ESM_SDF_PARA_STRU *pstSdfPara)
{
    VOS_UINT8                           aucApn[APP_MAX_APN_LEN];

    if ((PS_TRUE == NAS_ESM_IsDefaultEpsBearerType(pstSdfPara->enBearerCntxtType))
        && (NAS_ESM_OP_TRUE == pstSdfPara->bitOpApn))
    {
        NAS_ESM_MEM_CPY(aucApn, pstSdfPara->stApnInfo.aucApnName, pstSdfPara->stApnInfo.ucApnLen);
        aucApn[pstSdfPara->stApnInfo.ucApnLen] = '\0';

        vos_printf("����APN,APN��Ϣ����\r\n");
        vos_printf("%-30s : %d\r\n","APN����",pstSdfPara->stApnInfo.ucApnLen);
        vos_printf("%-30s : %s\r\n","APN����",aucApn);
    }
    else
    {
        vos_printf("������APN\r\n");
    }
}
/*lint +e960*/
/*****************************************************************************
 Function Name   : NAS_ESM_ShowEpsPdn
 Description     : ��ӡ������Ϣ�е�PDN��Ϣ
 Input           : pstEpsbInfo
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010    2010-01-19  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_ShowEpsPdn( const NAS_ESM_EPSB_CNTXT_INFO_STRU *pstEpsbInfo )
{
    if ((PS_TRUE == NAS_ESM_IsDefaultEpsBearerType(pstEpsbInfo->enBearerCntxtType))
        && (NAS_ESM_OP_TRUE == pstEpsbInfo->bitOpPdnAddr))
    {
        if (( APP_ESM_PDN_TYPE_IPV4 == pstEpsbInfo->stPdnAddrInfo.ucIpType )
                || ( APP_ESM_PDN_TYPE_IPV4_IPV6 == pstEpsbInfo->stPdnAddrInfo.ucIpType ))
        {
            vos_printf("%-30s : %s\r\n","PDN����","IPv4");
            vos_printf("%-30s : %d.%d.%d.%d\r\n",
            "IP��ַ",
            pstEpsbInfo->stPdnAddrInfo.aucIpV4Addr[0],
            pstEpsbInfo->stPdnAddrInfo.aucIpV4Addr[1],
            pstEpsbInfo->stPdnAddrInfo.aucIpV4Addr[2],
            pstEpsbInfo->stPdnAddrInfo.aucIpV4Addr[3]);
        }
        if (( APP_ESM_PDN_TYPE_IPV6 == pstEpsbInfo->stPdnAddrInfo.ucIpType )
                || ( APP_ESM_PDN_TYPE_IPV4_IPV6 == pstEpsbInfo->stPdnAddrInfo.ucIpType ))
        {
            vos_printf("%-30s : %s\r\n","PDN����","IPv6");
            vos_printf("%-30s : %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\r\n",
            "IP��ַ",
            pstEpsbInfo->stPdnAddrInfo.aucIpV6Addr[0],
            pstEpsbInfo->stPdnAddrInfo.aucIpV6Addr[1],
            pstEpsbInfo->stPdnAddrInfo.aucIpV6Addr[2],
            pstEpsbInfo->stPdnAddrInfo.aucIpV6Addr[3],
            pstEpsbInfo->stPdnAddrInfo.aucIpV6Addr[4],
            pstEpsbInfo->stPdnAddrInfo.aucIpV6Addr[5],
            pstEpsbInfo->stPdnAddrInfo.aucIpV6Addr[6],
            pstEpsbInfo->stPdnAddrInfo.aucIpV6Addr[7],
            pstEpsbInfo->stPdnAddrInfo.aucIpV6Addr[8],
            pstEpsbInfo->stPdnAddrInfo.aucIpV6Addr[9],
            pstEpsbInfo->stPdnAddrInfo.aucIpV6Addr[10],
            pstEpsbInfo->stPdnAddrInfo.aucIpV6Addr[11],
            pstEpsbInfo->stPdnAddrInfo.aucIpV6Addr[12],
            pstEpsbInfo->stPdnAddrInfo.aucIpV6Addr[13],
            pstEpsbInfo->stPdnAddrInfo.aucIpV6Addr[14],
            pstEpsbInfo->stPdnAddrInfo.aucIpV6Addr[15]);
        }
    }
    else
    {
        vos_printf("������PDN\r\n");
    }
}


/*lint -e960*/
VOS_VOID NAS_ESM_ShowUeIp()
{
    VOS_UINT8                           ucIpType;
    VOS_UINT32                          ulLoop;
    VOS_UINT32                          ulEpsbIndex;
    VOS_UINT32                          ulIndex             = 0;

    for ( ulLoop = NAS_ESM_MIN_EPSB_ID; ulLoop <= NAS_ESM_MAX_EPSB_ID; ulLoop++ )
    {
        if ((NAS_ESM_BEARER_STATE_ACTIVE == NAS_ESM_GetBearCntxtState(ulLoop))
             && (PS_TRUE == NAS_ESM_IsDefaultEpsBearerType(NAS_ESM_GetBearCntxtType(ulLoop))))
        {
            ulEpsbIndex = ulLoop - NAS_ESM_MIN_EPSB_ID;
            ucIpType = pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.ucIpType;
            if ( (APP_ESM_PDN_TYPE_IPV4 == ucIpType) || (APP_ESM_PDN_TYPE_IPV4_IPV6 == ucIpType))
            {
                vos_printf("UE��IP��ַ%d                    : %d.%d.%d.%d\r\n",
                ++ulIndex,
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV4Addr[0],
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV4Addr[1],
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV4Addr[2],
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV4Addr[3]);
            }
            else if ((APP_ESM_PDN_TYPE_IPV6 == ucIpType) || (APP_ESM_PDN_TYPE_IPV4_IPV6 == ucIpType))
            {
                vos_printf("UE��IP��ַ%d                    : %d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d\r\n",
                ++ulIndex,
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV6Addr[0],
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV6Addr[1],
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV6Addr[2],
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV6Addr[3],
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV6Addr[4],
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV6Addr[5],
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV6Addr[6],
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV6Addr[7],
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV6Addr[8],
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV6Addr[9],
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV6Addr[10],
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV6Addr[11],
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV6Addr[12],
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV6Addr[13],
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV6Addr[14],
                pg_stNasEsmEntity->astEpsbCntxtInfo[ulEpsbIndex].stPdnAddrInfo.aucIpV6Addr[15]);
            }
            else
            {
                vos_printf("IP��ַ���ʹ���\r\n");
            }
        }
    }
}
VOS_VOID NAS_ESM_ShowRegisterStatus(  )
{
    if (NAS_ESM_PS_REGISTER_STATUS_ATTACHED == pg_stNasEsmEntity->enRegisterStatus)
    {
        vos_printf("%-30s : %s\r\n","ע��״̬","��ע��");
    }
    else if (NAS_ESM_PS_REGISTER_STATUS_DETACHED == pg_stNasEsmEntity->enRegisterStatus)
    {
        vos_printf("%-30s : %s\r\n","ע��״̬","δע��");
    }
    else
    {
        vos_printf("%-30s : %s\r\n","ע��״̬","ע����");
    }
}


VOS_VOID NAS_ESM_ShowBearerState(NAS_ESM_BEARER_STATE_ENUM_UINT8 enBearerCntxtState)
{
    if(NAS_ESM_BEARER_STATE_ACTIVE == enBearerCntxtState )
    {
        vos_printf("%-30s : %s\r\n","����״̬","����̬");
    }
    else
    {
        vos_printf("%-30s : %s\r\n","����״̬","�Ǽ���̬");
    }
}


VOS_VOID NAS_ESM_ShowBearerType(NAS_ESM_BEARER_TYPE_ENUM_UINT8 enBearerCntxtType)
{
    if(NAS_ESM_BEARER_TYPE_DEDICATED == enBearerCntxtType )
    {
        vos_printf("%-30s : %s\r\n","��������","ר�г���");
    }
    else
    {
        vos_printf("%-30s : %s\r\n","��������","ȱʡ����");
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_ShowSdfBearerType
 Description     : ��ӡ��������
 Input           : enBearerCntxtType-----------��������
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2011-12-21  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_ShowSdfBearerType(APP_ESM_BEARER_TYPE_ENUM_UINT32 enBearerCntxtType)
{
    if(APP_ESM_BEARER_TYPE_DEDICATED == enBearerCntxtType )
    {
        vos_printf("%-30s : %s\r\n","��������","ר�г���");
    }
    else
    {
        vos_printf("%-30s : %s\r\n","��������","ȱʡ����");
    }
}


/*****************************************************************************
 Function Name   : NAS_ESM_ShowPti
 Description     : ��ӡPTI
 Input           : pstStateTbl
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-1-19  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_ShowPti(const NAS_ESM_STATE_INFO_STRU *pstStateTbl)
{
    if (NAS_ESM_OP_TRUE == pstStateTbl->bitOpNwMsgRecord)
    {
        vos_printf("%-30s : %d\r\n","PTIֵ",pstStateTbl->stNwMsgRecord.ucPti);
    }
}


VOS_VOID NAS_ESM_ShowProcedureState
(
    NAS_ESM_PROC_TRANS_STATE_ENUM_UINT8 enProcTransTState
)
{
    if(NAS_ESM_BEARER_PROC_TRANS_STATE_PENDING == enProcTransTState )
    {
        vos_printf("%-30s : %s\r\n","procedure״̬","pending̬");
    }
    else
    {
        vos_printf("%-30s : %s\r\n","procedure״̬","�Ǽ���̬");
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_ShowIsWaitForUserAnswer
 Description     : ��ӡ�Ƿ��ڵȴ��û��ظ�
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-1-19  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_ShowIsWaitForUserAnswer( VOS_UINT8 ucIsWaitForUserAnswer )
{
    if (PS_TRUE == ucIsWaitForUserAnswer)
    {
        vos_printf("%-30s : %s\r\n","�Ƿ��ڵȴ��û��ظ�", "��");
    }
    else
    {
        vos_printf("%-30s : %s\r\n","�Ƿ��ڵȴ��û��ظ�", "��");
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_ShowHasDedecodedNwMsg
 Description     : ��ӡ�Ƿ����������������Ϣ
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-1-19  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_ShowHasDedecodedNwMsg
(
    const NAS_ESM_NW_MSG_STRU          *pstDecodedNwMsg
)
{
    if (VOS_NULL_PTR != pstDecodedNwMsg)
    {
        vos_printf("%-30s : %s\r\n","�Ƿ����������������Ϣ", "��");
    }
    else
    {
        vos_printf("%-30s : %s\r\n","�Ƿ����������������Ϣ", "��");
    }
}

/*****************************************************************************
 Function Name   : NAS_ESM_ShowTimerInfo
 Description     : ��ӡ��ʱ����Ϣ
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-1-19  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_ShowTimerInfo
(
    const NAS_ESM_TIMER_STRU           *pstTimerInfo
)
{
    VOS_UINT32                          ulLoop;
    VOS_UINT32                          ulStateTimerNum;

    ulStateTimerNum = sizeof(g_astEsmTimerArray)/sizeof(NAS_ESM_TIMER_PRINT_LIST_STRU);

    for (ulLoop = 0; ulLoop < ulStateTimerNum; ulLoop++)
    {
        if (g_astEsmTimerArray[ulLoop].ulTimerId == pstTimerInfo->enPara)
        {
            break;
        }
    }

    if ( ulLoop < ulStateTimerNum )
    {
        vos_printf("%-30s\r\n",g_astEsmTimerArray[ulLoop].aucPrintString);
    }
    else
    {
        vos_printf("��ǰû��������ʱ��\r\n");
        return;
    }

    if (VOS_NULL_PTR != pstTimerInfo->hTimer)
    {
        vos_printf("%-30s : %s\r\n","����״̬","����");
    }
    else
    {
        vos_printf("%-30s : %s\r\n","����״̬","�ر�");
    }

    vos_printf("%-30s : %d\r\n","��ʱ����",pstTimerInfo->ucExpireTimes);

}


VOS_VOID  NAS_ESM_ShowCommonInfo( )
{
    NAS_ESM_BUFF_MANAGE_INFO_STRU      *pstEsmBuffManInfo   = NAS_ESM_GetEsmBufferManageInfoAddr();

    vos_printf("\r\n");

    /* ��ӡע��״̬ */
    NAS_ESM_ShowRegisterStatus();

    /* ��ӡPDN������Ŀ */
    if (NAS_ESM_PS_REGISTER_STATUS_ATTACHED == NAS_ESM_GetEmmStatus())
    {
        vos_printf("%-30s : %d\r\n","UE��PDN��������",pg_stNasEsmEntity->ulCurPdnConNum);
    }

    /* ��ӡ���ع���ʽ */
    NAS_ESM_ShowBearerManagementType();

    /* ��ӡUE��IP��ַ */
    NAS_ESM_ShowUeIp();

    /* ��ӡESM�����¼�� */
    vos_printf("%-30s : %d\r\n","ESM ATTACHING���ͻ����¼��",pstEsmBuffManInfo->ucAttBuffItemNum);
    vos_printf("%-30s : %d\r\n","ESM PTI���ͻ����¼��",pstEsmBuffManInfo->ucPtiBuffItemNum);

    vos_printf("\r\n");
}
VOS_VOID Nas_ESM_ShowAllSdfDynamicInfo(  )
{
    NAS_ESM_SDF_CNTXT_INFO_STRU              *pstSdfInfo = VOS_NULL_PTR;
    VOS_UINT32                                ulCid = NAS_ESM_NULL;
    VOS_UINT32                                ulEpsbId = NAS_ESM_NULL;
    VOS_UINT32                                ulSdfCount = NAS_ESM_NULL;
    VOS_UINT32                                ulSdfIndex = NAS_ESM_NULL;

    for ( ulCid = NAS_ESM_MIN_CID; ulCid <= NAS_ESM_MAX_CID; ulCid++ )
    {
        pstSdfInfo = NAS_ESM_GetSdfCntxtInfo(ulCid);

        ulEpsbId = pstSdfInfo->ulEpsbId;

        if ((NAS_ESM_MIN_EPSB_ID <= ulEpsbId) && (NAS_ESM_MAX_EPSB_ID >= ulEpsbId))
        {
            ulSdfCount++;
        }
    }

    vos_printf("\r\n");
    vos_printf("%-30s : %d\r\n","ESM�����SDF����",ulSdfCount);
    vos_printf("\r\n");

    for ( ulCid = NAS_ESM_MIN_CID ; ulCid <= NAS_ESM_MAX_CID; ulCid++ )
    {
        pstSdfInfo = NAS_ESM_GetSdfCntxtInfo(ulCid);

        ulEpsbId = pstSdfInfo->ulEpsbId;

        if ((NAS_ESM_MIN_EPSB_ID <= ulEpsbId) && (NAS_ESM_MAX_EPSB_ID >= ulEpsbId))
        {
            vos_printf("��%d��SDF����Ϣ����\r\n",(++ulSdfIndex));

            vos_printf("%-30s : %d\r\n","��SDF��Ӧ��CID",ulCid);
            vos_printf("%-30s : %d\r\n","��SDF��Ӧ��EpsbId",pstSdfInfo->ulEpsbId);
            vos_printf("%-30s : %d\r\n","��SDF��Packetfilter����",pstSdfInfo->ulSdfPfNum);

            NAS_ESM_ShowTft(pstSdfInfo->ulSdfPfNum, pstSdfInfo->astSdfPfInfo);

            vos_printf("\r\n");
        }
    }

}


VOS_VOID  NAS_ESM_ShowSdfDynamicInfo( VOS_UINT32 ulCid )
{
    NAS_ESM_SDF_CNTXT_INFO_STRU              *pstSdfInfo = VOS_NULL_PTR;
    VOS_UINT32                                ulSdfPfNum = NAS_ESM_NULL;

    vos_printf("\r\n");

    if (NAS_ESM_MAX_CID < ulCid)
    {
        vos_printf("�����CIDֵ����CIDȡֵ��ΧΪ0..31 \r\n");
        return;
    }

    pstSdfInfo = NAS_ESM_GetSdfCntxtInfo(ulCid);

    vos_printf("*******************CIDΪ%d��SDF�Ķ�̬��Ϣ����******************\r\n",ulCid);
    vos_printf("%-30s : %d\r\n","��SDF��Ӧ��EpsbId",pstSdfInfo->ulEpsbId);
    vos_printf("\r\n");

    NAS_ESM_ShowQos(&pstSdfInfo->stSdfQosInfo.stQosInfo);
    vos_printf("\r\n");

    ulSdfPfNum = pstSdfInfo->ulSdfPfNum;

    if ( NAS_ESM_NULL == ulSdfPfNum)
    {
        vos_printf("�����CID������TFT��Ϣ!\r\n");
        vos_printf("\r\n");
        return;
    }
    else
    {
        vos_printf("��SDF��ӦTFT��Ϣ����\r\n");
        NAS_ESM_ShowTft(pstSdfInfo->ulSdfPfNum, pstSdfInfo->astSdfPfInfo);
        vos_printf("\r\n");
    }

    vos_printf("\r\n");
}

VOS_VOID NAS_ESM_ShowPCO(const NAS_ESM_EPSB_CNTXT_INFO_STRU *pstEpsbInfo)
{
    VOS_UINT8        ulCnt = NAS_ESM_NULL;

    if ((PS_TRUE == NAS_ESM_IsDefaultEpsBearerType(pstEpsbInfo->enBearerCntxtType))
        && (NAS_ESM_OP_TRUE == pstEpsbInfo->bitOpPco))
    {

        vos_printf("�ó��ش���PCO,PCO��Ϣ����:\r\n");
        vos_printf("%-30s : %d\r\n","IPv4 DNS����������      ",pstEpsbInfo->stPcoInfo.ucIpv4DnsSerNum);

        for (ulCnt = 0; ulCnt < pstEpsbInfo->stPcoInfo.ucIpv4DnsSerNum; ulCnt++)
        {
            vos_printf("��%d��IPv4 DNS��IP��ַ                    : %d.%d.%d.%d\r\n",
                ulCnt + 1,
                pstEpsbInfo->stPcoInfo.astIpv4DnsServer[ulCnt].aucIpV4Addr[0],
                pstEpsbInfo->stPcoInfo.astIpv4DnsServer[ulCnt].aucIpV4Addr[1],
                pstEpsbInfo->stPcoInfo.astIpv4DnsServer[ulCnt].aucIpV4Addr[2],
                pstEpsbInfo->stPcoInfo.astIpv4DnsServer[ulCnt].aucIpV4Addr[3]);
        }

        vos_printf("%-30s : %d\r\n","IPv6 DNS����������      ",pstEpsbInfo->stPcoInfo.ucIpv6DnsSerNum);

        for (ulCnt = 0; ulCnt < pstEpsbInfo->stPcoInfo.ucIpv6DnsSerNum; ulCnt++)
        {
            vos_printf("��%d��IPv6 DNS��IP��ַ                    : %d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d.%d\r\n", ulCnt + 1,
                pstEpsbInfo->stPcoInfo.astIpv6DnsServer[ulCnt].aucIpV6Addr[0],
                pstEpsbInfo->stPcoInfo.astIpv6DnsServer[ulCnt].aucIpV6Addr[1],
                pstEpsbInfo->stPcoInfo.astIpv6DnsServer[ulCnt].aucIpV6Addr[2],
                pstEpsbInfo->stPcoInfo.astIpv6DnsServer[ulCnt].aucIpV6Addr[3],
                pstEpsbInfo->stPcoInfo.astIpv6DnsServer[ulCnt].aucIpV6Addr[4],
                pstEpsbInfo->stPcoInfo.astIpv6DnsServer[ulCnt].aucIpV6Addr[5],
                pstEpsbInfo->stPcoInfo.astIpv6DnsServer[ulCnt].aucIpV6Addr[6],
                pstEpsbInfo->stPcoInfo.astIpv6DnsServer[ulCnt].aucIpV6Addr[7],
                pstEpsbInfo->stPcoInfo.astIpv6DnsServer[ulCnt].aucIpV6Addr[8],
                pstEpsbInfo->stPcoInfo.astIpv6DnsServer[ulCnt].aucIpV6Addr[9],
                pstEpsbInfo->stPcoInfo.astIpv6DnsServer[ulCnt].aucIpV6Addr[10],
                pstEpsbInfo->stPcoInfo.astIpv6DnsServer[ulCnt].aucIpV6Addr[11],
                pstEpsbInfo->stPcoInfo.astIpv6DnsServer[ulCnt].aucIpV6Addr[12],
                pstEpsbInfo->stPcoInfo.astIpv6DnsServer[ulCnt].aucIpV6Addr[13],
                pstEpsbInfo->stPcoInfo.astIpv6DnsServer[ulCnt].aucIpV6Addr[14],
                pstEpsbInfo->stPcoInfo.astIpv6DnsServer[ulCnt].aucIpV6Addr[15]);
        }

    }
    else
    {
        vos_printf("�ó��ز�����PCO��Ϣ\r\n");
    }

}

VOS_VOID NAS_ESM_ShowEpsbInfo(VOS_UINT32 ulEpsbId)
{
    NAS_ESM_EPSB_CNTXT_INFO_STRU       *pstEpsbInfo         = VOS_NULL_PTR;

    vos_printf("\r\n");

    if ((ulEpsbId < NAS_ESM_MIN_EPSB_ID) || (ulEpsbId > NAS_ESM_MAX_EPSB_ID))
    {
        vos_printf("�����EPSBIDֵ����EPSBIDȡֵ��ΧΪ5..15 \r\n");
        return;
    }

    pstEpsbInfo = NAS_ESM_GetEpsbCntxtInfoAddr(ulEpsbId);

    vos_printf("*******************EPS����%d�������Ϣ����******************\r\n",ulEpsbId);

    /* ��ӡ����״̬ */
    NAS_ESM_ShowBearerState(pstEpsbInfo->enBearerCntxtState);
    vos_printf("\r\n");

    /* ��ӡ�������� */
    NAS_ESM_ShowBearerType(pstEpsbInfo->enBearerCntxtType);
    vos_printf("\r\n");

    /* ��ӡ����ȱʡ���غ� */
    NAS_ESM_ShowLinkEpsbId(pstEpsbInfo->ulLinkedEpsbId);
    vos_printf("\r\n");

    /* ��ӡ����QOS��Ϣ */
    NAS_ESM_ShowEpsQos(pstEpsbInfo);
    vos_printf("\r\n");

    /* ��ӡ���ذ�����SDF ID */
    NAS_ESM_ShowEpsSdfId(pstEpsbInfo);
    vos_printf("\r\n");

    /* ��ӡPDN��Ϣ */
    NAS_ESM_ShowEpsPdn(pstEpsbInfo);
    vos_printf("\r\n");

    /* ��ӡAPN��Ϣ */
    NAS_ESM_ShowEpsApn(pstEpsbInfo);
    vos_printf("\r\n");

    /* ��ӡAPN-AMBR��Ϣ */
    NAS_ESM_ShowEpsApnAmbr(pstEpsbInfo);

    /* ��ӡPCO��Ϣ */
    NAS_ESM_ShowPCO(pstEpsbInfo);
}

/*****************************************************************************
 Function Name   : NAS_ESM_ShowAllEpsbInfo
 Description     : ��ӡ���г�����Ϣ����������ĺ�δ�����
 Input           : None
 Output          : None
 Return          : VOS_VOID

 History         :
    1.lihong00150010      2010-1-18  Draft Enact

*****************************************************************************/
VOS_VOID NAS_ESM_ShowAllEpsbInfo( VOS_VOID )
{
    VOS_UINT32                 ulEpsbId;

    for ( ulEpsbId = NAS_ESM_MIN_EPSB_ID; ulEpsbId <= NAS_ESM_MAX_EPSB_ID; ulEpsbId++ )
    {
        /* ��ʾ������Ϣ */
        NAS_ESM_ShowEpsbInfo(ulEpsbId);
    }
}
VOS_VOID  NAS_ESM_ShowAllActiveEpsbInfo( VOS_VOID )
{
    VOS_UINT32                 ulEpsbId;
    VOS_UINT32                 ulActiveNum = NAS_ESM_NULL;

    for ( ulEpsbId = NAS_ESM_MIN_EPSB_ID; ulEpsbId <= NAS_ESM_MAX_EPSB_ID; ulEpsbId++ )
    {
        if ( NAS_ESM_BEARER_STATE_INACTIVE == NAS_ESM_GetBearCntxtState(ulEpsbId))
        {
            continue;
        }
        else
        {
            ulActiveNum++;

            /* ��ʾ������Ϣ */
            NAS_ESM_ShowEpsbInfo(ulEpsbId);
        }
    }

    if ( NAS_ESM_NULL == ulActiveNum )
    {
        vos_printf("%-30s\r\n","��ǰ�����ڼ����EPS����");
    }
}
/*lint +e960*/

VOS_VOID NAS_ESM_ShowSdfStaticInfo( VOS_UINT32 ulCid )
{
    NAS_ESM_SDF_PARA_STRU  *pstSdfPara  = VOS_NULL_PTR;

    vos_printf("\r\n");

    if (NAS_ESM_MAX_UE_CONTRUL_CID < ulCid)
    {
        vos_printf("�����CIDֵ����CIDȡֵ��ΧΪ0..31 \r\n");
        return;
    }

    vos_printf("*******************CID %d�������Ϣ����********************\r\n",ulCid);

    /*��ȡ���ؽṹָ��*/
    pstSdfPara = NAS_ESM_GetSdfParaAddr(ulCid);

    /* ��ӡ����CID */
    NAS_ESM_ShowLinkCid(pstSdfPara);
    vos_printf("\r\n");

    /* ��ӡ�������� */
    NAS_ESM_ShowSdfBearerType(pstSdfPara->enBearerCntxtType);
    vos_printf("\r\n");

    /* ��ӡQOS��Ϣ */
    NAS_ESM_ShowCidInfoQos(pstSdfPara);
    vos_printf("\r\n");

    /* ��ӡTFT��Ϣ */
    NAS_ESM_ShowTft(pstSdfPara->ulPfNum, pstSdfPara->astCntxtTftInfo);
    vos_printf("\r\n");

    /* ��ӡPND���� */
    NAS_ESM_ShowPdnTpe(pstSdfPara);
    vos_printf("\r\n");

    /* ��ӡAPN��Ϣ */
    NAS_ESM_ShowCidInfoApn(pstSdfPara);
    vos_printf("\r\n");

    /* ��ӡIP��Ϣ��ȡ��ʽ */
    NAS_ESM_ShowIpAllocType(pstSdfPara);
    vos_printf("\r\n");

    vos_printf("cid in static sdf info: %d", pstSdfPara->ulCId);

    vos_printf("\r\n");
}


VOS_VOID Nas_ESM_ShowAllSdfStaticInfo(  )
{
    VOS_UINT32 ulCid;

    vos_printf("\r\n");

    vos_printf("******************CID 0-11 �������Ϣ����*******************\r\n");

    for ( ulCid = NAS_ESM_MIN_CID; ulCid <= 10; ulCid++ )
    {
        NAS_ESM_ShowSdfStaticInfo(ulCid);
    }
}


VOS_VOID NAS_ESM_ShowStateTblInfo(  )
{
    VOS_UINT32                          ulStateTblIndex;
    VOS_UINT32                          uStateTablelFlag    = NAS_ESM_OP_FALSE;
    NAS_ESM_STATE_INFO_STRU             *pstStateTbl        = VOS_NULL_PTR;

    vos_printf("\r\n");

    for( ulStateTblIndex = 0; ulStateTblIndex < NAS_ESM_MAX_STATETBL_NUM; ulStateTblIndex++ )
    {
        pstStateTbl = NAS_ESM_GetStateTblAddr(ulStateTblIndex);

        if (NAS_ESM_MAX_CID_NUM != pstStateTbl->ulCid)
        {
            uStateTablelFlag = NAS_ESM_OP_TRUE;

            vos_printf("*******************״̬��%d�������Ϣ����******************\r\n",ulStateTblIndex);
            vos_printf("%-30s : %d\r\n","cidֵ",pstStateTbl->ulCid);
            vos_printf("%-30s : %d\r\n","EPS����ID",pstStateTbl->ulEpsbId);
            vos_printf("%-30s : %d\r\n","����EPS����ID",pstStateTbl->ulLinkedEpsbId);
            vos_printf("\r\n");

            /* ��ӡ����״̬ */
            NAS_ESM_ShowProcedureState(pstStateTbl->enProcTransTState);

            /* ��ӡ����״̬ */
            NAS_ESM_ShowBearerState(pstStateTbl->enBearerCntxtState);

            /* ��ӡ�������� */
            NAS_ESM_ShowBearerType(pstStateTbl->enBearerCntxtType);

            /* ��ӡPTI */
            NAS_ESM_ShowPti(pstStateTbl);

            /* ��ӡ�Ƿ��ڵȴ��û��ظ� */
            NAS_ESM_ShowIsWaitForUserAnswer(pstStateTbl->ucIsWaitForUserAnswer);

            /* ��ӡ�Ƿ����������������Ϣ */
            NAS_ESM_ShowHasDedecodedNwMsg(pstStateTbl->pstDecodedNwMsg);

            /* ��ӡ��ʱ����Ϣ */
            NAS_ESM_ShowTimerInfo(&pstStateTbl->stTimerInfo);
            vos_printf("\r\n");

        }
    }

    if ( uStateTablelFlag == NAS_ESM_OP_FALSE )
    {
        vos_printf("%-30s\r\n","ESM��ǰ������״̬����Ϣ");
    }
}
VOS_VOID  NAS_ESM_CmdHelp(  )
{
    vos_printf("\r\n");
    vos_printf("********************** NAS ESM ��������б� *********************\r\n");
    vos_printf("%-30s : NAS ESM������Ϣ\r\n","NAS_ESM_ShowCommonInfo");
    vos_printf("%-30s : UE������SDF�ľ�̬��Ϣ\r\n","Nas_ESM_ShowAllSdfStaticInfo");
    vos_printf("%-30s : UE��ĳ��SDF(0..31)�ľ�̬��Ϣ\r\n","NAS_ESM_ShowSdfStaticInfo(cid)");
    vos_printf("%-30s : UE������SDF�Ķ�̬��Ϣ\r\n","Nas_ESM_ShowAllSdfDynamicInfo");
    vos_printf("%-30s : UE��ĳ��SDF(0..31)�Ķ�̬��Ϣ\r\n","NAS_ESM_ShowSdfDynamicInfo(cid)");
    vos_printf("%-30s : UE������EPS������Ϣ�����������δ����\r\n","NAS_ESM_ShowAllEpsbInfo");
    vos_printf("%-30s : UE�����м���EPS������Ϣ\r\n","NAS_ESM_ShowAllActiveEpsbInfo");
    vos_printf("%-30s : UE��ĳ��EPS������Ϣ(5..15)\r\n","NAS_ESM_ShowEpsbInfo(epsid)");
    vos_printf("%-30s : ESMģ��״̬��Ϣ\r\n","NAS_ESM_ShowStateTblInfo");
    vos_printf("******************************************************************\r\n");
    vos_printf("\r\n");
}

/*****************************************************************************
 Function Name   : NAS
 Description     : NAS����������
 Input           : VOS_VOID
 Output          : None
 Return          : VOS_VOID

 History         :
    1.sunbing49683      2010-4-20  Draft Enact

*****************************************************************************/
VOS_VOID NAS( NAS_OM_CMD_TYPE_ENUM_UINT8 enLayerName)
{
    switch ( enLayerName )
    {
        case NAS_OM_ALL :

            vos_printf("=====================UE NAS�����������==================\n");
            vos_printf("EMMģ�������Ϣ�������ѯ����:     NAS 1\n");
            vos_printf("ESMģ�������Ϣ�������ѯ����:     NAS 2\n");
            vos_printf("RABMģ�������Ϣ�������ѯ����:    NAS 3\n");

            NAS_LMM_CmdHelp();
            NAS_ESM_CmdHelp();
            NAS_ERABM_CmdHelp();
            break;
        case NAS_OM_MM:
            NAS_LMM_CmdHelp();
            break;
        case NAS_OM_ESM:
            NAS_ESM_CmdHelp();
            break;
        case NAS_OM_RABM:
            NAS_ERABM_CmdHelp();
            break;
        default:
            break;
    }

    return;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

