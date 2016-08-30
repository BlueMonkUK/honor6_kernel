

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "TafApsMntn.h"
#include "TafLog.h"
#include "MnComm.h"
#include "MnApsComm.h"
#include "TafApsApi.h"
#include "Taf_Aps.h"



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
/*lint -e767*/
#define    THIS_FILE_ID                 PS_FILE_ID_TAF_APS_MNTN_C
/*lint +e767*/


/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

VOS_VOID  TAF_APS_TraceMsg(
    VOS_VOID                           *pMsg
)
{
    /* ��Ϣ���� */
    OM_TraceMsgHook(pMsg);
}
VOS_VOID  TAF_APS_TraceSyncMsg(
    VOS_UINT32                          ulMsgId,
    VOS_UINT8                          *pucData,
    VOS_UINT32                          ulLength
)
{
    TAF_PS_MSG_STRU                    *pstMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulMsgLength;

    if (ulLength >= 4)
    {
        ulMsgLength = ulLength - 4;
    }
    else
    {
        ulMsgLength = 0;
    }

    pstMsg = (TAF_PS_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_TAF,
                                  sizeof(TAF_PS_MSG_STRU) + ulMsgLength);
    if (VOS_NULL_PTR == pstMsg)
    {
        MN_ERR_LOG("TAF_APS_TraceSyncMsg:ERROR:Alloc Mem Fail.");
        return;
    }

    /* ������Ϣ��ֵ */
    pstMsg->stHeader.ulSenderCpuId      = VOS_LOCAL_CPUID;
    pstMsg->stHeader.ulSenderPid        = WUEPS_PID_TAF;
    pstMsg->stHeader.ulReceiverCpuId    = VOS_LOCAL_CPUID;
    pstMsg->stHeader.ulReceiverPid      = WUEPS_PID_TAF;

    pstMsg->stHeader.ulLength           = ulMsgLength + 8;
    pstMsg->stHeader.ulMsgName          = ulMsgId;

    PS_MEM_CPY(pstMsg->aucContent, pucData, ulLength);

    /* ��Ϣ���� */
    TAF_APS_TraceMsg(pstMsg);
    PS_MEM_FREE(WUEPS_PID_TAF, pstMsg);

    return;
}



VOS_VOID  TAF_APS_TraceApsEvt(
    VOS_UINT32                          ulEvtId,
    VOS_UINT32                          ulLength,
    VOS_UINT8                          *pucData
)
{
    TAF_PS_EVT_STRU                    *pstMsg = VOS_NULL_PTR;

    pstMsg = (TAF_PS_EVT_STRU *)PS_MEM_ALLOC(WUEPS_PID_TAF,
                                  sizeof(TAF_PS_EVT_STRU) + ulLength - 4);
    if (VOS_NULL_PTR == pstMsg)
    {
        MN_ERR_LOG("TAF_APS_TraceApsEvt:ERROR:Alloc Mem Fail.");
        return;
    }

    /* ������Ϣ��ֵ */
    pstMsg->stHeader.ulSenderCpuId      = VOS_LOCAL_CPUID;
    pstMsg->stHeader.ulSenderPid        = WUEPS_PID_TAF;
    pstMsg->stHeader.ulReceiverCpuId    = VOS_LOCAL_CPUID;
    pstMsg->stHeader.ulReceiverPid      = WUEPS_PID_TAF;

    pstMsg->stHeader.ulLength           = ulLength + 8;
    pstMsg->stHeader.ulMsgName          = ulEvtId;
    pstMsg->ulEvtId                     = ulEvtId;

    PS_MEM_CPY(pstMsg->aucContent, pucData, ulLength);

    /* ��Ϣ���� */
    TAF_APS_TraceMsg(pstMsg);

    PS_MEM_FREE(WUEPS_PID_TAF, pstMsg);

    return;
}


VOS_VOID  TAF_APS_TraceTimer(
    TAF_APS_TIMER_STATUS_ENUM_U8        enTimerStatus,
    TAF_APS_TIMER_ID_ENUM_UINT32        enTimerId,
    VOS_UINT32                          ulLen,
    VOS_UINT32                          ulPara
)
{
    TAF_APS_TIMER_INFO_STRU            *pstMsg = VOS_NULL_PTR;

    pstMsg = (TAF_APS_TIMER_INFO_STRU*)PS_MEM_ALLOC(WUEPS_PID_TAF,
                             sizeof(TAF_APS_TIMER_INFO_STRU));
    if (VOS_NULL_PTR == pstMsg)
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF,"TAF_APS_TraceTimer:ERROR:Alloc Mem Fail.");
        return;
    }

    PS_MEM_SET(pstMsg, 0, sizeof(TAF_APS_TIMER_INFO_STRU));

    /* ��ʱ��������Ϣ��ֵ */
    pstMsg->stMsgHeader.ulSenderCpuId   = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulSenderPid     = WUEPS_PID_TAF;
    pstMsg->stMsgHeader.ulReceiverPid   = VOS_PID_TIMER;

    pstMsg->stMsgHeader.ulLength        = sizeof(TAF_APS_TIMER_INFO_STRU) - VOS_MSG_HEAD_LENGTH;

    pstMsg->stMsgHeader.ulMsgName       = enTimerId;
    pstMsg->enTimerStatus               = enTimerStatus;
    pstMsg->ulPara                      = ulPara;
    pstMsg->ulLen                       = ulLen;

    /* ��ʱ����Ϣ���� */
    TAF_APS_TraceMsg(pstMsg);

    PS_MEM_FREE(WUEPS_PID_TAF, pstMsg);

}


VOS_VOID TAF_APS_ShowPdpEntityInfo(
    VOS_UINT8                           ucPdpId
)
{
    APS_PDP_CONTEXT_ENTITY_ST          *pstPdpEntity;

    pstPdpEntity    = TAF_APS_GetPdpEntInfoAddr(ucPdpId);

    vos_printf("\n");
    vos_printf("*****************************\n");
    vos_printf("*BearerInfo: bitCid      %d\n", pstPdpEntity->stClientInfo.ulBitCidMask);
    vos_printf("*BearerInfo: Cid         %d\n", pstPdpEntity->stClientInfo.ucCid);
    vos_printf("*BearerInfo: ModuleId    %d\n", pstPdpEntity->stClientInfo.aulModuleId[pstPdpEntity->stClientInfo.ucCid]);
    vos_printf("*BearerInfo: PdpType     %d\n", pstPdpEntity->stClientInfo.aenPdpType[pstPdpEntity->stClientInfo.ucCid]);
    vos_printf("*BearerInfo: RabId       %d\n", pstPdpEntity->ucNsapi);
    vos_printf("*BearerInfo: LinkedNsapi %d\n", pstPdpEntity->ucLinkedNsapi);
    vos_printf("*BearerInfo: PdpType     %d\n", pstPdpEntity->PdpAddr.ucPdpTypeNum);
    vos_printf("*BearerInfo: Apn         %s\n", pstPdpEntity->PdpApn.aucValue);
    vos_printf("*****************************\n");

}

#if (FEATURE_ON == FEATURE_LTE)

VOS_VOID TAF_APS_ShowCidSdfParaInfo(
    VOS_UINT8                           ucCid,
    VOS_UINT8                           ucPfId
)
{
    APS_L4A_SDF_PARA_STRU              *pstSdfParaInfo;

    pstSdfParaInfo = (APS_L4A_SDF_PARA_STRU*)PS_MEM_ALLOC(
                            WUEPS_PID_TAF,
                            sizeof(APS_L4A_SDF_PARA_STRU));
    if (VOS_NULL_PTR == pstSdfParaInfo)
    {
        vos_printf("TAF_APS_ShowCidSdfParaInfo: ALLOC NULL PTR\n");
        return;
    }

    PS_MEM_SET(pstSdfParaInfo, 0x00, sizeof(APS_L4A_SDF_PARA_STRU));

    TAF_APS_GetCidSdfParaInfo(ucCid, pstSdfParaInfo);

    vos_printf("\n");
    vos_printf("****************** SDF INFO ********************************\n");
    vos_printf("* CID                                       %d\n", pstSdfParaInfo->ulCid);
    vos_printf("* bitOpPdnType                              %d\n", pstSdfParaInfo->bitOpPdnType);
    vos_printf("* bitOpApn                                  %d\n", pstSdfParaInfo->bitOpApn);
    vos_printf("* bitOpSdfQos                               %d\n", pstSdfParaInfo->bitOpSdfQos);
    vos_printf("* bitOpLinkdCId                             %d\n", pstSdfParaInfo->bitOpLinkdCId);
    vos_printf("* bitIpv4AddrAllocType                      %d\n", pstSdfParaInfo->bitIpv4AddrAllocType);
    vos_printf("* bitOpGwAuthInfo                           %d\n", pstSdfParaInfo->bitOpGwAuthInfo);
    vos_printf("************************************************************\n");
    vos_printf("* LinkdCid                                  %d\n", pstSdfParaInfo->ulLinkdCid);
    vos_printf("* PdnType                                   %d\n", pstSdfParaInfo->enPdnType);
    vos_printf("* Ipv4AddrAllocType                         %d\n", pstSdfParaInfo->enIpv4AddrAllocType);
    vos_printf("* BearerCntxtType                           %d\n", pstSdfParaInfo->enBearerCntxtType);
    vos_printf("* aucApnName                                %s\n", pstSdfParaInfo->stApnInfo.aucApnName);
    vos_printf("* ucApnLen                                  %d\n", pstSdfParaInfo->stApnInfo.ucApnLen);
    vos_printf("************************************************************\n");
    vos_printf("* enGwAuthType (0:PAP 1:CHAP)               %d\n", pstSdfParaInfo->stGwAuthInfo.enGwAuthType);
    vos_printf("* aucUserName                               %s\n", pstSdfParaInfo->stGwAuthInfo.aucUserName);
    vos_printf("* ucUserNameLen                             %d\n", pstSdfParaInfo->stGwAuthInfo.ucUserNameLen);
    vos_printf("* aucPwd                                    %s\n", pstSdfParaInfo->stGwAuthInfo.aucPwd);
    vos_printf("* ucPwdLen                                  %d\n", pstSdfParaInfo->stGwAuthInfo.ucPwdLen);
    vos_printf("************************************************************\n");
    vos_printf("* PfNum                                     %d\n", pstSdfParaInfo->ulPfNum);
    vos_printf("* bitOpRmtIpv4AddrAndMask                   %d\n", pstSdfParaInfo->astCntxtTftInfo[ucPfId].bitOpRmtIpv4AddrAndMask);
    vos_printf("* bitOpRmtIpv6AddrAndMask                   %d\n", pstSdfParaInfo->astCntxtTftInfo[ucPfId].bitOpRmtIpv6AddrAndMask);
    vos_printf("* bitOpProtocolId                           %d\n", pstSdfParaInfo->astCntxtTftInfo[ucPfId].bitOpProtocolId);
    vos_printf("* bitOpSingleLocalPort                      %d\n", pstSdfParaInfo->astCntxtTftInfo[ucPfId].bitOpSingleLocalPort);
    vos_printf("* bitOpLocalPortRange                       %d\n", pstSdfParaInfo->astCntxtTftInfo[ucPfId].bitOpLocalPortRange);
    vos_printf("* bitOpSingleRemotePort                     %d\n", pstSdfParaInfo->astCntxtTftInfo[ucPfId].bitOpSingleRemotePort);
    vos_printf("* bitOpRemotePortRange                      %d\n", pstSdfParaInfo->astCntxtTftInfo[ucPfId].bitOpRemotePortRange);
    vos_printf("* bitOpSecuParaIndex                        %d\n", pstSdfParaInfo->astCntxtTftInfo[ucPfId].bitOpSecuParaIndex);
    vos_printf("* bitOpTypeOfService                        %d\n", pstSdfParaInfo->astCntxtTftInfo[ucPfId].bitOpTypeOfService);
    vos_printf("* ucPacketFilterId                          %d\n", pstSdfParaInfo->astCntxtTftInfo[ucPfId].ucPacketFilterId);
    vos_printf("* ucNwPacketFilterId                        %d\n", pstSdfParaInfo->astCntxtTftInfo[ucPfId].ucNwPacketFilterId);
    vos_printf("* enDirection                               %d\n", pstSdfParaInfo->astCntxtTftInfo[ucPfId].enDirection);
    vos_printf("* ucPrecedence                              %d\n", pstSdfParaInfo->astCntxtTftInfo[ucPfId].ucPrecedence);
    vos_printf("* ucProtocolId                              %d\n", pstSdfParaInfo->astCntxtTftInfo[ucPfId].ucProtocolId);
    vos_printf("************************************************************\n");

    PS_MEM_FREE(WUEPS_PID_TAF, pstSdfParaInfo);

    return;
}
#endif


VOS_VOID TAF_APS_Help(
    VOS_UINT8                           ucCid
)
{
    vos_printf("\n");
    vos_printf("****************** APS�����Ϣ**********************************\n");
    vos_printf("* TAF_APS_ShowCidSdfParaInfo   ��ʾSDF����(����1:CID ����2:PfId)\n");
    vos_printf("* TAF_APS_ShowPdpEntityInfo    ��ʾAPSʵ����Ϣ(����:APSʵ������)\n");
    vos_printf("****************************************************************\n");

    return;
}

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

