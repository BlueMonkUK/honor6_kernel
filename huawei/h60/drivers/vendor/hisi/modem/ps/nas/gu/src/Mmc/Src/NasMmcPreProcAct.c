/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : NasMmcPreProcAct.c
  �� �� ��   : ����
  ��    ��   : zhoujun /40661
  ��������   : 2010��11��15��
  ����޸�   :
  ��������   :Ԥ����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2010��11��12��
    ��    ��   : zhoujun /40661
    �޸�����   : �����ļ�

******************************************************************************/

/*****************************************************************************
  1 ͷ�ļ�����
*****************************************************************************/
#include "pslog.h"
#include "om.h"
#include "PsTypeDef.h"
#include "PsCommonDef.h"
#include "NasComm.h"
#include "NasCommDef.h"
#include "NasMmcPreProcAct.h"
#include "NasMmcProcNvim.h"
#include "NasMmcMain.h"
#include "NasMmcFsmSyscfg.h"
#include "NasMmcSndMma.h"
#include "NasMmcFsmMainTbl.h"
#include "NasMmcSndGmm.h"
#include "NasMmcSndMm.h"
#include "NasMmcSndGuAs.h"
#include "NasMmcSndMma.h"
#include "NasMmcSndOm.h"
#include "NasMmcSndInternalMsg.h"
#include "NasMmcFsmInterSysCcoTbl.h"
#include "NasMmcFsmInterSysOosTbl.h"
#include "NasMmcFsmInterSysCellReselTbl.h"
#include "NasMmcFsmPlmnSelection.h"
#include "NasMmcTimerMgmt.h"
#include "UsimPsInterface.h"
#include "NasMmlCtx.h"
#include "NasMmlLib.h"
#include "NasMmlMsgProc.h"
#include "NasMmcProcNvim.h"
#include "NasMmcProcUsim.h"
#include "NasMmcFsmSyscfg.h"
#include "NasMmcProcRegRslt.h"
/* ɾ��ExtAppMmcInterface.h*/
#include "NasMmcComFunc.h"
#include "NasMmcPlmnSelectionStrategy.h"
#include "NVIM_Interface.h"
#include "NasMmcComFunc.h"
#include "NasUtranCtrlInterface.h"
#include "NasUsimmApi.h"

#include  "NasMmcFsmPlmnSelectionTbl.h"
#include "NasMmcCtx.h"

#if (FEATURE_ON == FEATURE_PTM)
#include "NasErrorLog.h"
#endif

#if   (FEATURE_ON == FEATURE_LTE)
#include "NasMmcSndLmm.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_NAS_MMCBGPREPROC_C


/*****************************************************************************
  2 ȫ�ֱ�������
*****************************************************************************/


/*****************************************************************************
  3 ����ʵ��
*****************************************************************************/

#if defined (__PS_WIN32_RECUR__)

VOS_UINT32 NAS_MMC_RestoreFixedContextData_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_FIXED_CONTEXT_MSG_STRU                          stRcvMsgCB;
    NVIM_EQUIVALENT_PLMN_LIST_STRU                          stEquivPlmn;
    VOS_UINT8                                               i;
    NAS_MMC_NVIM_NETWORK_CAPABILITY_STRU                    stNetworkCap;/*network capability*/
    NAS_NVIM_RPLMN_WITH_RAT_STRU                            stRplmn;
    VOS_UINT32                                              ulSrcLen;
    VOS_UINT32                                              ulDataLen;
    NAS_MML_PC_REPLAY_COMPRESS_CONTEXT_STRU                *pstRcMsg;
    VOS_UINT8                                              *pucSrc;
    VOS_UINT32                                              ulCount;
    VOS_UINT32                                              ulNvLength;

    NAS_NVIM_FOLLOWON_OPENSPEED_FLAG_STRU               stQuickStartFlg;
    NAS_NVIM_AUTOATTACH_STRU                            stAutoattachFlag;
    NAS_NVIM_HPLMN_FIRST_TIMER_STRU                     stHplmnTimerLen;
    NAS_NVIM_SELPLMN_MODE_STRU                          stSelplmnMode;

    stQuickStartFlg.ulQuickStartSta = 0;
    PS_MEM_SET(&stAutoattachFlag, 0x00, sizeof(NAS_NVIM_AUTOATTACH_STRU));

    PS_MEM_SET(&stHplmnTimerLen, 0x00, sizeof(NAS_NVIM_HPLMN_FIRST_TIMER_STRU));
    PS_MEM_SET(&stSelplmnMode, 0x00, sizeof(NAS_NVIM_SELPLMN_MODE_STRU));
    ulNvLength = 0;

    ulSrcLen  = 0;
    ulDataLen = sizeof(NAS_MMC_FIXED_CONTEXT_MSG_STRU);

    pstRcMsg = (NAS_MML_PC_REPLAY_COMPRESS_CONTEXT_STRU *)pstMsg;
    pucSrc   = pstRcMsg->aucData;

    /* �����ܳ��� */
    PS_MEM_CPY(&ulCount, pucSrc, sizeof(VOS_UINT32));
    pucSrc += sizeof(VOS_UINT32);

    if( ulDataLen != ulCount )
    {
        return VOS_FALSE;
    }

    NAS_MML_UnCompressData(&stRcvMsgCB, &ulDataLen,
                           pucSrc, &ulSrcLen);

    /* NV��Ļָ�,�ָ�IMEI������ */
    NV_Write(en_NV_Item_IMEI, stRcvMsgCB.aucImeisv, NAS_MML_MAX_IMEISV_LEN);

    /* NV��Ļָ�,�ָ�����ģʽ */
    stSelplmnMode.usSelPlmnMode = (VOS_UINT16)stRcvMsgCB.enSelectionMode;
    NV_Write(en_NV_Item_SelPlmn_Mode, &stSelplmnMode, NV_ITEM_SELPLMN_MODE_SIZE);

    /* NV��Ļָ�,�ָ���ЧPLMN */
    PS_MEM_SET(&stEquivPlmn, 0, sizeof (NVIM_EQUIVALENT_PLMN_LIST_STRU));
    stEquivPlmn.ucCount = stRcvMsgCB.stEquPlmnInfo.ucEquPlmnNum;

    for (i=0; i<stEquivPlmn.ucCount; i++)
    {
        stEquivPlmn.struPlmnList[i].ucMcc[0]
            = (VOS_UINT8)(stRcvMsgCB.stEquPlmnInfo.astEquPlmnAddr[i].ulMcc & 0x0f);
        stEquivPlmn.struPlmnList[i].ucMcc[1]
            = (VOS_UINT8)((stRcvMsgCB.stEquPlmnInfo.astEquPlmnAddr[i].ulMcc>>8) & 0x0f);
        stEquivPlmn.struPlmnList[i].ucMcc[2]
            = (VOS_UINT8)((stRcvMsgCB.stEquPlmnInfo.astEquPlmnAddr[i].ulMcc>>16) & 0x0f);
        stEquivPlmn.struPlmnList[i].ucMnc[0]
            = (VOS_UINT8)(stRcvMsgCB.stEquPlmnInfo.astEquPlmnAddr[i].ulMnc & 0x0f);
        stEquivPlmn.struPlmnList[i].ucMnc[1]
            = (VOS_UINT8)((stRcvMsgCB.stEquPlmnInfo.astEquPlmnAddr[i].ulMnc>>8) & 0x0f);
        stEquivPlmn.struPlmnList[i].ucMnc[2]
            = (VOS_UINT8)((stRcvMsgCB.stEquPlmnInfo.astEquPlmnAddr[i].ulMnc>>16) & 0x0f);
    }

    NV_Write(en_NV_Item_EquivalentPlmn, &stEquivPlmn, sizeof(NVIM_EQUIVALENT_PLMN_LIST_STRU));

    /* NV��Ļָ�,�ָ���Ч�������� */
    stNetworkCap.aucNetworkCapability[0] = stRcvMsgCB.stMsNetworkCapability.ucNetworkCapabilityLen;

    if (stRcvMsgCB.stMsNetworkCapability.ucNetworkCapabilityLen > NAS_MML_MAX_NETWORK_CAPABILITY_LEN)
    {
        stNetworkCap.aucNetworkCapability[0] = NAS_MML_MAX_NETWORK_CAPABILITY_LEN;
    }

    /* ������������ */
    PS_MEM_CPY(&(stNetworkCap.aucNetworkCapability[1]),
               stRcvMsgCB.stMsNetworkCapability.aucNetworkCapability,
               stNetworkCap.aucNetworkCapability[0]);

    NV_Write(en_NV_Item_NetworkCapability, &stNetworkCap, NV_ITEM_NET_CAPABILITY_MAX_SIZE);

    /* NV��Ļָ�,�ָ�RPLMN��Ϣ */
    stRplmn.ucLastRplmnRat          = stRcvMsgCB.stRplmnCfg.enLastRplmnRat;
    stRplmn.ucLastRplmnRatEnableFlg = stRcvMsgCB.stRplmnCfg.ucLastRplmnRatFlg;
    stRplmn.stGRplmn.ulMcc          = stRcvMsgCB.stRplmnCfg.stGRplmnInNV.ulMcc;
    stRplmn.stGRplmn.ulMnc          = stRcvMsgCB.stRplmnCfg.stGRplmnInNV.ulMnc;
    stRplmn.stWRplmn.ulMcc          = stRcvMsgCB.stRplmnCfg.stWRplmnInNV.ulMcc;
    stRplmn.stWRplmn.ulMnc          = stRcvMsgCB.stRplmnCfg.stWRplmnInNV.ulMnc;

    NV_Write(en_NV_Item_RPlmnWithRat, &stRplmn, sizeof(stRplmn));

    /* �ָ� Ps Auto attach */
    NV_GetLength(en_NV_Item_Autoattach, &ulNvLength);
    stAutoattachFlag.usAutoattachFlag = (VOS_UINT16)stRcvMsgCB.ucPsAutoAttachFlg;
    NV_Write(en_NV_Item_Autoattach, &(stAutoattachFlag.usAutoattachFlag), ulNvLength);

    /* NV��Ļָ�,�ָ�HPLMN��ʱ��ʱ��,NV�������Է����������,ʵ����������ʱ��ʱ��Ҫ��ms������ */
    stHplmnTimerLen.ucHplmnTimerLen = (VOS_UINT8)((stRcvMsgCB.stBGSrchCfg.ulFirstStartHplmnTimerLen) / (60 * 1000));
    NV_Write(en_NV_Item_HPlmnFirstTimer, &(stHplmnTimerLen.ucHplmnTimerLen), NV_ITEM_HPLMN_FIRST_SEARCH_SIZE);

    /* NV��Ļָ�,�ָ����ٿ��� */
    stQuickStartFlg.ulQuickStartSta = stRcvMsgCB.ulQuickStartFlag;
    NV_Write(en_NV_Item_FollowOn_OpenSpeed_Flag, &(stQuickStartFlg.ulQuickStartSta), sizeof(NAS_NVIM_FOLLOWON_OPENSPEED_FLAG_STRU));


    NAS_INFO_LOG(WUEPS_PID_MMC, "NAS_MMC_RestoreFixedContextData_PreProc - data is restored.");

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RestoreContextData_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_CONTEXT_STRU                                    *pstMmcCtx;
    NAS_MML_CTX_STRU                                        *pstMmlCtx;
    NAS_MML_PC_REPLAY_COMPRESS_CONTEXT_STRU                 *pstRcMsg;
    VOS_UINT32                                               ulExpectCount;
    VOS_UINT32                                               ulCount;
    VOS_UINT32                                               ulItemCount;
    VOS_UINT8                                               *pucDest;
    VOS_UINT8                                               *pucSrc;
    VOS_UINT32                                               ulDestLen;

    pstMmcCtx     = NAS_MMC_GetMmcCtxAddr();
    pstMmlCtx     = NAS_MML_GetMmlCtx();
    ulExpectCount = 0;
    ulCount       = 0;
    ulItemCount   = 0;
    ulExpectCount = sizeof(NAS_MML_CTX_STRU)
                  + sizeof(MM_SUB_LAYER_SHARE_STRU)
                  + sizeof(NAS_MMC_CONTEXT_STRU)
                  + sizeof(TAF_PLMN_ID_STRU);

    /*ע��״̬��*/
    NAS_MMC_RegFsm();

    pstRcMsg = (NAS_MML_PC_REPLAY_COMPRESS_CONTEXT_STRU *)pstMsg;
    pucSrc   = pstRcMsg->aucData;

    /* �����ܳ��� */
    PS_MEM_CPY(&ulCount, pucSrc, sizeof(VOS_UINT32));
    pucSrc += sizeof(VOS_UINT32);

    if( ulExpectCount != ulCount )
    {
        return VOS_FALSE;
    }

    /* g_stNasMmlCtxĿ���ַ */
    pucDest     = (VOS_UINT8 *)pstMmlCtx;
    ulDestLen   = sizeof(NAS_MML_CTX_STRU);

    /* ��ѹ�� */
    if (VOS_FALSE == NAS_MML_UnCompressData(pucDest, &ulDestLen, pucSrc, &ulItemCount) )
    {
        return VOS_FALSE;
    }

    /* ����ԭ�ڲ���Ϣ����Ϊ�п����ڲ���Ϣ������������������ˣ�Ȼ��RECUR����Ҫ�����ڲ���Ϣ��*/
    pstMmlCtx->stInternalMsgQueue.ucHeader = 0;
    pstMmlCtx->stInternalMsgQueue.ucTail = 0;

    pucSrc += (ulItemCount);

    /* g_MmSubLyrShareĿ���ַ */
    pucDest     = (VOS_UINT8 *)&g_MmSubLyrShare;
    ulDestLen   = sizeof(MM_SUB_LAYER_SHARE_STRU);

    /* ��ѹ�� */
    if (VOS_FALSE == NAS_MML_UnCompressData(pucDest, &ulDestLen, pucSrc, &ulItemCount) )
    {
        return VOS_FALSE;
    }
    pucSrc += (ulItemCount);

    /* g_stNasMmcCtxĿ���ַ */
    pucDest     = (VOS_UINT8 *)pstMmcCtx;
    ulDestLen   = sizeof(NAS_MMC_CONTEXT_STRU);

    /* ��ѹ�� */
    if (VOS_FALSE == NAS_MML_UnCompressData(pucDest, &ulDestLen, pucSrc, &ulItemCount) )
    {
        return VOS_FALSE;
    }
    pucSrc += (ulItemCount);

    NAS_MMC_ReloadFsmDesc(NAS_MMC_GetCurFsmAddr(), pstMmcCtx->stCurFsm.enFsmId);

    /* ����PC�ط���Ϣ����PC������״̬����FsmStack��ַ���¸�ֵ */
    NAS_MMC_RestoreFsmStack(&pstMmcCtx->stFsmStack);

#if (FEATURE_ON == FEATURE_LTE)
#ifdef WIN32
    PS_MEM_CPY(&g_stSetRplmn,pucSrc,sizeof(TAF_PLMN_ID_STRU));
#endif
#endif
     pucSrc += sizeof(TAF_PLMN_ID_STRU);


    NAS_INFO_LOG(WUEPS_PID_MMC, "MMC: NAS_MMC_RestoreContextData - data is restored.");

    return VOS_TRUE;
}



VOS_VOID NAS_MMC_RestoreFsmStack(
    NAS_MMC_FSM_STACK_STRU             *pstPcFsmStack
)
{
    NAS_MMC_FSM_CTX_STRU               *pstFsmCtx   = VOS_NULL_PTR;
    NAS_MMC_FSM_STACK_STRU             *pstFsmStack = VOS_NULL_PTR;
    VOS_UINT32                          ulPcRecurFsmId;
    VOS_UINT32                          i;

    /* ���ݲ���״̬��ջ������״̬����ȡ��ǰ״̬��������ַ  */
    pstFsmStack                     = NAS_MMC_GetFsmStackAddr();

    PS_MEM_CPY(pstFsmStack, pstPcFsmStack, sizeof(NAS_MMC_FSM_STACK_STRU));

    /* ��ʼ��ջ�еĵ�ǰ״̬����FsmDesc */
    for ( i = 0 ; i < (pstPcFsmStack->usStackDepth); i++ )
    {
        pstFsmCtx                           = &(pstFsmStack->astFsmStack[i]);

        ulPcRecurFsmId                      = pstPcFsmStack->astFsmStack[i].enFsmId;

        NAS_MMC_ReloadFsmDesc(pstFsmCtx, ulPcRecurFsmId);
    }
}


VOS_VOID NAS_MMC_ReloadFsmDesc(
    NAS_MMC_FSM_CTX_STRU               *pstFsmCtx,
    VOS_UINT32                          ulPcRecurFsmId
)
{
    switch ( ulPcRecurFsmId )
    {
        case NAS_MMC_FSM_L1_MAIN :
            pstFsmCtx->pstFsmDesc       = NAS_MMC_GetMainFsmDescAddr();
            break;

        case NAS_MMC_FSM_SWITCH_ON :
            pstFsmCtx->pstFsmDesc       = NAS_MMC_GetSwitchOnFsmDescAddr();
            break;

        case NAS_MMC_FSM_POWER_OFF :
            pstFsmCtx->pstFsmDesc       = NAS_MMC_GetPowerOffFsmDescAddr();
            break;

        case NAS_MMC_FSM_PLMN_SELECTION :
            pstFsmCtx->pstFsmDesc       = NAS_MMC_GetPlmnSelectionFsmDescAddr();
            break;

        case NAS_MMC_FSM_ANYCELL_SEARCH :
            pstFsmCtx->pstFsmDesc       = NAS_MMC_GetAnyCellSearchFsmDescAddr();
            break;

        case NAS_MMC_FSM_SYSCFG:
            pstFsmCtx->pstFsmDesc       = NAS_MMC_GetSysCfgFsmDescAddr();
            break;

        case NAS_MMC_FSM_INTER_SYS_CELLRESEL:
            pstFsmCtx->pstFsmDesc       = NAS_MMC_GetInterSysCellReselFsmDescAddr();
            break;

        case NAS_MMC_FSM_INTER_SYS_OOS:
            pstFsmCtx->pstFsmDesc       = NAS_MMC_GetInterSysOosFsmDescAddr();
            break;

        case NAS_MMC_FSM_INTER_SYS_HO:
            pstFsmCtx->pstFsmDesc       = NAS_MMC_GetInterSysHoFsmDescAddr();
            break;

        case NAS_MMC_FSM_INTER_SYS_CCO:
            pstFsmCtx->pstFsmDesc       = NAS_MMC_GetInterSysCcoFsmDescAddr();
            break;

        case NAS_MMC_FSM_PLMN_LIST:
            pstFsmCtx->pstFsmDesc       = NAS_MMC_GetPlmnListFsmDescAddr();
            break;

        case NAS_MMC_FSM_BG_PLMN_SEARCH:
            pstFsmCtx->pstFsmDesc               = NAS_MMC_GetBgPlmnSearchFsmDescAddr();
            break;

        default:
            break;
    }

}


#endif
VOS_UINT32  NAS_MMC_RcvMmaModeChange_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                        *pstMsg
)
{

    MMA_MMC_MODE_CHANGE_REQ_STRU                           *pstModeChangeReq = VOS_NULL_PTR;
    NAS_MML_MS_MODE_ENUM_UINT8                              enMsMode;
    VOS_UINT8                                              *pucEf           = VOS_NULL_PTR;
    NAS_MML_MS_MODE_ENUM_UINT8                              enOldMsMode;
    NAS_MMC_CS_DOMAIN_CAPA_CHANGE_TYPE_ENUM_UINT8           enCsChgType;
    VOS_UINT32                                              ulEfLen;

#if (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                                              ulDisableLteRoamFlg;
#endif

    /* �������ĺϷ��� */
    pstModeChangeReq    = (MMA_MMC_MODE_CHANGE_REQ_STRU*)pstMsg;

    enMsMode = NAS_MMC_ConvertMmaMsModeToMmlMsMode(pstModeChangeReq->enMsMode);

    /* �쳣���:�������ʹ���ֱ�ӷ��� */
    if (NAS_MML_MS_MODE_BUTT == enMsMode)
    {
        return VOS_TRUE;
    }

    if (VOS_TRUE == NAS_MML_GetSvlteSupportFlag())
    {
        ulEfLen         = 0;

        /* ��ȡԭ�����ֻ�ģʽ */
        enOldMsMode = NAS_MML_GetMsMode();

        /* ��ȡCS�������仯���� */
        enCsChgType = NAS_MMC_GetCsDomainCapaChangeType(enOldMsMode, enMsMode);

        /* CS���ɲ�֧�ֵ�֧��ʱ����ӦSVLTE��˫���е����������
            MODEM0��Ҫͬ��һ��֮ǰMODEM1��USIM�ϸ��¹���CS��������Ϣ */
        if (NAS_MMC_CS_DOMAIN_CAPA_UNAVAIL_TO_AVAIL == enCsChgType)
        {
            /* ͬ��SIM���е�EFloci��EFkeys�ļ� */
            if (USIMM_API_SUCCESS == NAS_USIMMAPI_GetCachedFile(NAS_MML_READ_CS_LOC_FILE_ID, &ulEfLen, &pucEf, USIMM_UNLIMIT_APP))
            {
                /* ��ȡ�ļ��ɹ���MMC�ڲ�����Ӧ���� */
                NAS_MMC_ProcAgentUsimReadFileCnf_PreProc(NAS_MML_READ_CS_LOC_FILE_ID, (VOS_UINT16)ulEfLen, pucEf);
            }

            if (USIMM_API_SUCCESS == NAS_USIMMAPI_GetCachedFile(NAS_MML_READ_CS_CKIK_FILE_ID, &ulEfLen, &pucEf, USIMM_UNLIMIT_APP))
            {
                /* ��ȡ�ļ��ɹ���MMC�ڲ�����Ӧ���� */
                NAS_MMC_ProcAgentUsimReadFileCnf_PreProc(NAS_MML_READ_CS_CKIK_FILE_ID, (VOS_UINT16)ulEfLen, pucEf);
            }
        }
        else
        {
        }

    }

    /* ����MML�����MODE */
    NAS_MML_SetMsMode(enMsMode);

    /* ֪ͨGMMģ��ģʽ�ı� */
    NAS_MMC_SndGmmModeChangeReq(enMsMode);

    /* ֪ͨMMģ��ģʽ�ı� */
    NAS_MMC_SndMmModeChangeReq(enMsMode);

#if (FEATURE_ON == FEATURE_LTE)

    ulDisableLteRoamFlg = NAS_MML_GetDisableLteRoamFlg();

    /* �ж��Ƿ���Ҫdisable LTE */
    if (VOS_TRUE == NAS_MMC_IsNeedDisableLte_ModeChange())
    {
        if (NAS_MML_SIM_TYPE_SIM == NAS_MML_GetSimType())
        {
            NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_SIM_CARD_SWITCH_ON);
        }
        else
        {
             NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_PS_USIM_INVALID);
        }

        if ( NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
        {
            NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS);
        }
        else
        {
            /* ������ε��µ�disable�Ѿ�֪ͨ��GU�����,��ʱ����Ҫ���ظ�֪ͨ */
            if (VOS_FALSE == ulDisableLteRoamFlg)
            {
                /* ��WAS/GAS����disable LTE֪ͨ��Ϣ */
                NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_DISABLE);
                NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_DISABLE);

                /* ��LMM����disable LTE��Ϣ */
                NAS_MMC_SndLmmDisableLteNotify(NAS_MML_GetDisableLteReason());
            }

            /* ����disable LTE������� */
            NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS);
        }

        return VOS_TRUE;
    }

    /* �ж��Ƿ���Ҫenable LTE */
    if (VOS_TRUE == NAS_MMC_IsNeedEnableLte_ModeChange())
    {
        /* ������ڽ�ֹLTE�������ε��µ�disable LTE����δ�ص�����֮ǰ��ʱҲ��֪ͨenable LTE */
        if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == NAS_MML_GetLteCapabilityStatus())
         && (VOS_FALSE == ulDisableLteRoamFlg))
        {
            /* ��WAS/GAS����enable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);

            /* ��LMM����enable LTE��Ϣ */
            NAS_MMC_SndLmmEnableLteNotify();
        }

        /* ����disable LTE������� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }
#endif

    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvGmmInfo_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                        *pstMsg
)
{
    MMCGMM_INFO_IND_STRU                *pRcvMsg = VOS_NULL_PTR;

    pRcvMsg = (MMCGMM_INFO_IND_STRU *)pstMsg;

    NAS_MMC_SndMmaMmInfo(&pRcvMsg->stMmInfo);

    return VOS_TRUE;
}


/*****************************************************************************
 �� �� ��  : NAS_MMC_PreProcPdpStatusInd
 ��������  : Ԥ����MMCGMM_PDP_STATUS_IND��Ϣ
 �������  : ulEventType:��Ϣ����
             pstMsg:��ǰ�������Ϣ
 �������  : ��
 �� �� ֵ  : VOS_TRUE:����Ԥ�������
             VOS_FALSE:����Ԥ����δ���
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2010��11��16��
    ��    ��   : zhoujun /40661
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32  NAS_MMC_RcvGmmPdpStatusInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                        *pstMsg
)
{
    MMCGMM_PDP_STATUS_IND_STRU          *pstRcvMsg;

    pstRcvMsg = (MMCGMM_PDP_STATUS_IND_STRU*)pstMsg;
    if ( (MMC_GMM_ACTIVE_PDP_CONTEXT_EXIST != pstRcvMsg->ulPdpStatusFlg)
      && (MMC_GMM_ACTIVE_PDP_CONTEXT_NOT_EXIST != pstRcvMsg->ulPdpStatusFlg))
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvPdpStatusInd_PreProc:Msg is invalid.");
        return VOS_TRUE;
    }

    if ( MMC_GMM_ACTIVE_PDP_CONTEXT_EXIST == pstRcvMsg->ulPdpStatusFlg)
    {
        NAS_MMC_SndMmaDataTranAttri(MMA_MMC_DATATRAN_ATTRI_PDP_ACT);
    }
    else
    {
        NAS_MMC_SndMmaDataTranAttri(MMA_MMC_DATATRAN_ATTRI_PDP_DEACT);
    }

    return VOS_TRUE;

}




VOS_UINT32  NAS_MMC_RcvGmmServiceStatusInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                        *pstMsg
)
{
    MMCGMM_SERVICE_STATUS_IND_STRU     *pstServiceMsg;
    NAS_MMC_SERVICE_ENUM_UINT8          enPsServiceStatus;

    pstServiceMsg  = (MMCGMM_SERVICE_STATUS_IND_STRU*)pstMsg;

    enPsServiceStatus = NAS_MMC_ConverMmStatusToMmc(NAS_MMC_REG_DOMAIN_PS,
                                                    (NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8)pstServiceMsg->ulServiceStatus);

    NAS_MMC_SetPsServiceStatus(enPsServiceStatus);

    if (VOS_FALSE == NAS_MMC_GetPlmnSearchCtrl()->ucWrrLimitServiceInd)
    {
        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, pstServiceMsg->ulServiceStatus);
    }

    return VOS_TRUE;

}



VOS_UINT32  NAS_MMC_RcvMmInfo_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                        *pstMsg
)
{
    MMCMM_INFO_IND_STRU                 *pstRcvMsg;

    pstRcvMsg = (MMCMM_INFO_IND_STRU *)pstMsg;

    NAS_MMC_SndMmaMmInfo(&(pstRcvMsg->stMmInfo));

    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvMmServiceStatusInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMCMM_SERVICE_STATUS_IND_STRU                          *pstServiceMsg = VOS_NULL_PTR;
    NAS_MMC_SERVICE_ENUM_UINT8                              enCsServiceStatus;

    pstServiceMsg       = (MMCMM_SERVICE_STATUS_IND_STRU*)pstMsg;

    /* MM�ϱ�����״̬Ϊδ�ı�ʱ��ֱ�ӷ��� */
    if (MM_COM_SRVST_NO_CHANGE == pstServiceMsg->ulServiceStatus)
    {
        return VOS_TRUE;
    }

    enCsServiceStatus = NAS_MMC_ConverMmStatusToMmc(NAS_MMC_REG_DOMAIN_CS,
                                                    (NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8)(pstServiceMsg->ulServiceStatus));

    NAS_MMC_SetCsServiceStatus(enCsServiceStatus);

    if (VOS_FALSE == NAS_MMC_GetPlmnSearchCtrl()->ucWrrLimitServiceInd)
    {
        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS,
                                       pstServiceMsg->ulServiceStatus);
    }

    return VOS_TRUE;
}
VOS_UINT32  NAS_MMC_RcvGasScellRxInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                        *pstMsg
)
{
    GRRMM_SCELL_MEAS_IND_ST            *pstScellRx      = VOS_NULL_PTR;
    NAS_MML_CAMP_CELL_INFO_STRU        *pstCampCellInfo = VOS_NULL_PTR;
    VOS_UINT32                          ulReportRssiFlg;
    VOS_UINT32                          ulReportCellIdFlg;
    MMA_MMC_SCELL_MEAS_REPORT_TYPE_UN   unMeasReportType;

    pstCampCellInfo         =  NAS_MML_GetCampCellInfo();
    pstScellRx              = (GRRMM_SCELL_MEAS_IND_ST *)pstMsg;
    ulReportRssiFlg         = VOS_FALSE;
    ulReportCellIdFlg       = VOS_FALSE;

    /*�����ʳ���99���ź�ǿ�ȴ���0��С��-114����Ϊ����*/
    if ( (pstScellRx->ucChannelQual > NAS_MML_GSM_CHANNEL_QUAL_INVALID)
      || (pstScellRx->sRxlev > NAS_MML_RSSI_UPPER_LIMIT)
      || (pstScellRx->sRxlev < NAS_MML_RSSI_LOWER_LIMIT))
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC ,"NAS_MMC_PreProcGasScellRxInd():WARNING:Value is unvalid");
        return VOS_TRUE;
    }

    /* ���ж��Ƿ����ϴε���ֵһ��,��GAS���ȷ��,��ȫ����GAS�ϱ���ֵ */
    if (((GRRMM_SCELL_MEAS_TYPE_RXLEV == (pstScellRx->unMeasReportType.ucMeasReportType & GRRMM_SCELL_MEAS_TYPE_RXLEV))
      || (GRRMM_SCELL_MEAS_TYPE_NONE == pstScellRx->unMeasReportType.ucMeasReportType)) )
    {
        ulReportRssiFlg = VOS_TRUE;
    }

    /* ��ber�ı�ʱ����Ҫ֪ͨmma�ı�ȫ�ֱ��� */
    if ((GRRMM_SCELL_MEAS_TYPE_RXLEV     != (pstScellRx->unMeasReportType.ucMeasReportType & GRRMM_SCELL_MEAS_TYPE_RXLEV))
     || (GRRMM_SCELL_MEAS_TYPE_RXQUALITY == (pstScellRx->unMeasReportType.ucMeasReportType & GRRMM_SCELL_MEAS_TYPE_RXQUALITY)))
    {
        ulReportRssiFlg = VOS_TRUE;
    }
    

    /* פ��̬���ϱ�����ָʾ��CELL ID���ѯ�ϱ�ʱ,CELL ID�仯���ϱ�CELL ID */
    if ((NAS_MMC_AS_CELL_CAMP_ON == NAS_MMC_GetAsCellCampOn())
     && ((GRRMM_SCELL_MEAS_TYPE_CELLID == (pstScellRx->unMeasReportType.ucMeasReportType & GRRMM_SCELL_MEAS_TYPE_CELLID))
      || (GRRMM_SCELL_MEAS_TYPE_NONE == pstScellRx->unMeasReportType.ucMeasReportType)))
    {
        ulReportCellIdFlg = VOS_TRUE;
    }

    /*ͨ����Ϣ����ȫ�ֱ���*/
    pstCampCellInfo->ucCellNum                      = 1;
    pstCampCellInfo->ucRssiNum                      = 1;

    /*�����ź�����*/
    pstCampCellInfo->astRssiInfo[0].sRssiValue      = pstScellRx->sRxlev;

    /*����������*/
    pstCampCellInfo->astRssiInfo[0].ucChannalQual   = pstScellRx->ucChannelQual;
    pstCampCellInfo->sUeRfPower                     = pstScellRx->sUeRfPower;
    pstCampCellInfo->usCellDlFreq                   = pstScellRx->usCellDlFreq;
    pstCampCellInfo->usCellUlFreq                   = pstScellRx->usCellUlFreq;

    pstCampCellInfo->astCellInfo[0].sCellRssi       = pstScellRx->sRxlev;



    /* CELL ID�����仯 */
    if (VOS_TRUE == ulReportCellIdFlg)
    {
        pstCampCellInfo->astCellInfo[0].ulCellId = pstScellRx->usCellId;
        NAS_MMC_SndMmaSysInfo();
    }

    PS_MEM_SET(((VOS_UINT8*)&unMeasReportType), 0, sizeof(MMA_MMC_SCELL_MEAS_REPORT_TYPE_UN));
    unMeasReportType.ucMeasReportType = pstScellRx->unMeasReportType.ucMeasReportType;

    /*���ú�����TAF�ϱ����*/
    if ( VOS_TRUE == ulReportRssiFlg )
    {   
        NAS_MMC_SndMmaRssiInd(&unMeasReportType);
    }

    return VOS_TRUE;
}


VOS_VOID  NAS_MMC_RcvWasCellSubMode_PreProc(
    RRC_NAS_AT_MSG_SUBMODE_ENUM_UINT8   enSubMode
)
{
    /* �ϱ�W������ģʽ,R99 */
    if (RRC_NAS_AT_R99_ONLY == enSubMode)
    {
        NAS_MMC_SndMmaDataTranAttri(MMA_MMC_DATATRAN_ATTRI_R99_ONLY);
    }
    /* �ϱ�W������ģʽ,HSDPA */
    else if (RRC_NAS_AT_HSDPA_ONLY == enSubMode)
    {
        NAS_MMC_SndMmaDataTranAttri(MMA_MMC_DATATRAN_ATTRI_HSDPA);
    }
    /* �ϱ�W������ģʽ,HSUPA */
    else if (RRC_NAS_AT_HSUPA_ONLY == enSubMode)
    {
        NAS_MMC_SndMmaDataTranAttri(MMA_MMC_DATATRAN_ATTRI_HSUPA);
    }
    /* �ϱ�W������ģʽ,HSUPA+HSDPA */
    else if (RRC_NAS_AT_HSDPA_AND_HSUPA == enSubMode)
    {
        NAS_MMC_SndMmaDataTranAttri(MMA_MMC_DATATRAN_ATTRI_HSDPA_AND_HSUPA);
    }
    /* �ϱ�W������ģʽ,HSPA+ */
    else if (RRC_NAS_AT_HSPA_PLUS == enSubMode)
    {
        NAS_MMC_SndMmaDataTranAttri(MMA_MMC_DATATRAN_ATTRI_HSPA_PLUS);
    }
    else if (RRC_NAS_AT_DC_HSPA_PLUS == enSubMode)
    {
        NAS_MMC_SndMmaDataTranAttri(MMA_MMC_DATATRAN_ATTRI_DC_HSPA_PLUS);
    }
    else if ( RRC_NAS_AT_DC_MIMO == enSubMode )
    {
        NAS_MMC_SndMmaDataTranAttri(MMA_MMC_DATATRAN_ATTRI_DC_MIMO);
    }
    else
    {
        ;
    }
}
VOS_UINT32  NAS_MMC_RcvWasAtMsgInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                        *pstMsg
)
{
    RRMM_AT_MSG_IND_ST                  *pstAtMsg;

    pstAtMsg = (RRMM_AT_MSG_IND_ST *)pstMsg;

    /* �ϱ�С��ID */
    if ( RRC_NAS_AT_CELL_ID == (pstAtMsg->ucMsgType & RRC_NAS_AT_CELL_ID) )
    {
        NAS_MMC_RcvWasCellId_PreProc((RRC_NAS_CELL_ID_ST*)pstAtMsg->aucData);
    }

    /* �ϱ�С��Bler */
    if ( RRC_NAS_AT_CELL_BLER == (pstAtMsg->ucMsgType & RRC_NAS_AT_CELL_BLER) )
    {
        NAS_MMC_RcvWasCellBler_PreProc((RRC_NAS_CELL_TRCH_BLER_STRU_LIST*)pstAtMsg->aucData);
    }

    /* �ϱ�С���ź����� */
    if  (RRC_NAS_AT_CELL_SIGN == (pstAtMsg->ucMsgType & RRC_NAS_AT_CELL_SIGN) )
    {
        NAS_MMC_RcvWasCellRssi_PreProc((RRC_NAS_CELL_RSCP_STRU_LIST*)pstAtMsg->aucData);
    }

    if (RRC_NAS_AT_CELL_SUBMODE == (pstAtMsg->ucMsgType & RRC_NAS_AT_CELL_SUBMODE) )
    {
        NAS_MMC_RcvWasCellSubMode_PreProc((RRC_NAS_AT_MSG_SUBMODE_ENUM_UINT8)pstAtMsg->aucData[0]);
    }



    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvWasAcInfoChange_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_W_AC_INFO_CHANGE_IND_STRU     *pstWasAcInfoChangeInd = VOS_NULL_PTR;
    NAS_MML_NETWORK_INFO_STRU          *pstNetWorkInfo        = VOS_NULL_PTR;
    VOS_UINT8                           ucOldCsResTrictionRegister;
    VOS_UINT8                           ucOldPsResTrictionRegister;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurCsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldCsRestrictInfo;
    NAS_MML_ACCESS_RESTRICTION_STRU                         *pstCurPsRestrictInfo = VOS_NULL_PTR;
    NAS_MML_ACCESS_RESTRICTION_STRU                          stOldPsRestrictInfo;

    pstWasAcInfoChangeInd       = (RRMM_W_AC_INFO_CHANGE_IND_STRU*)pstMsg;
    pstNetWorkInfo              =  NAS_MML_GetNetworkInfo();
    ucOldCsResTrictionRegister  = NAS_MML_GetCsRestrictRegisterFlg();
    ucOldPsResTrictionRegister  = NAS_MML_GetPsRestrictRegisterFlg();

    pstCurCsRestrictInfo    = NAS_MML_GetCsAcRestrictionInfo();
    pstCurPsRestrictInfo    = NAS_MML_GetPsAcRestrictionInfo();

    PS_MEM_CPY(&stOldCsRestrictInfo, pstCurCsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));
    PS_MEM_CPY(&stOldPsRestrictInfo, pstCurPsRestrictInfo, sizeof(NAS_MML_ACCESS_RESTRICTION_STRU));

    /* ����CS PSע��������Ϣ */
    NAS_MMC_UpdateCsPsRestriction_AcChangeInd(pstWasAcInfoChangeInd, pstNetWorkInfo);

    /* ��MM����AC INFO CHANGE */
    NAS_MMC_SndMmWasAcInfoChangeInd(pstWasAcInfoChangeInd->stCsRestriction.enRestrictRegister,
                                    pstWasAcInfoChangeInd->stPsRestriction.enRestrictRegister,
                                    ucOldCsResTrictionRegister,
                                    ucOldPsResTrictionRegister);

    /* ��GMM����AC INFO CHANGE */
    NAS_MMC_SndGmmWasAcInfoChangeInd(pstWasAcInfoChangeInd->stCsRestriction.enRestrictRegister,
                                     pstWasAcInfoChangeInd->stPsRestriction.enRestrictRegister,
                                     ucOldCsResTrictionRegister,
                                     ucOldPsResTrictionRegister);

    /* פ��С���Ľ���������Ϣ�仯ʱ,֪ͨMMAģ�鵱ǰ����������Ϣ */
    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldCsRestrictInfo, pstCurCsRestrictInfo))
    {
        NAS_MMC_SndMmaAcInfoChangeInd(MMA_MMC_SRVDOMAIN_CS, pstCurCsRestrictInfo);
    }

    if (VOS_TRUE == NAS_MML_IsAcInfoChanged(&stOldPsRestrictInfo, pstCurPsRestrictInfo))
    {
        NAS_MMC_SndMmaAcInfoChangeInd(MMA_MMC_SRVDOMAIN_PS, pstCurPsRestrictInfo);
    }

    return VOS_TRUE;
}



VOS_UINT32  NAS_MMC_RcvWasAtMsgCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                        *pstMsg
)
{
    RRMM_AT_MSG_CNF_ST                  *pstAtMsg = VOS_NULL_PTR;

    pstAtMsg = (RRMM_AT_MSG_CNF_ST *)pstMsg;

    /* �ж���Ϣ����Ч��*/
    if ( RRC_NAS_AT_VALID != pstAtMsg->ucMsgFlg)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC ,"NAS_MMC_RcvWasAtMsgCnf_PreProc():WARNING:the msg is not valid!");

        return VOS_TRUE;
    }

    if (RRC_NAS_AT_START_FOREVER == pstAtMsg->ucActionType)
    {
        /* W��ѵ�ǰС���ź������ϱ������账���ȴ�at msg ind��Ϣ*/
        return VOS_TRUE;
    }

    if ( RRC_NAS_AT_CELL_ID == (pstAtMsg->ucMsgType & RRC_NAS_AT_CELL_ID) )
    {
        NAS_MMC_RcvWasCellId_PreProc((RRC_NAS_CELL_ID_ST*)pstAtMsg->aucData);
    }

    if ( RRC_NAS_AT_CELL_BLER == (pstAtMsg->ucMsgType & RRC_NAS_AT_CELL_BLER) )
    {
        NAS_MMC_RcvWasCellBler_PreProc((RRC_NAS_CELL_TRCH_BLER_STRU_LIST*)pstAtMsg->aucData);
    }

    if  (RRC_NAS_AT_CELL_SIGN == (pstAtMsg->ucMsgType & RRC_NAS_AT_CELL_SIGN) )
    {
        NAS_MMC_RcvWasCellRssi_PreProc((RRC_NAS_CELL_RSCP_STRU_LIST*)pstAtMsg->aucData);
    }

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : NAS_MMC_PreProcPlmnQryReq
 ��������  : ����WRR��GAS���͹�����PLMN INFO��ѯ��Ϣ
 �������  : ulEventType:��Ϣ����
             pstMsg:��ǰ�������Ϣ
 �������  : ��
 �� �� ֵ  : VOS_TRUE:����Ԥ�������
             VOS_FALSE:����Ԥ����δ���
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2010��11��29��
   ��    ��   : zhoujun /40661
   �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32  NAS_MMC_RcvRrMmPlmnQryReqPreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                        *pstMsg
)
{
    NAS_MMC_SndRrcPlmnQueryCnf(((RRMM_PLMN_QUERY_REQ_STRU *)pstMsg)->enQueryWPlmn);

    return VOS_TRUE;
}

/*****************************************************************************
 �� �� ��  : NAS_MMC_RcvRrMmNotCampOn_PreProc
 ��������  : �յ��ײ�NOT CAMP ON��Ϣ�Ĵ���
 �������  : ulEventType - ��Ϣ����
             pstMsg      - ��Ϣ���׵�ַ
 �������  : ��
 �� �� ֵ  : VOS_TRUE:����Ԥ�������
             VOS_FALSE:����Ԥ����δ���
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2011��07��20��
   ��    ��   : zhoujun \40661
   �޸�����   : �����ɺ���

*****************************************************************************/
VOS_UINT32  NAS_MMC_RcvRrMmNotCampOn_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);

    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvRrMmEquplmnQuery_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    EQUIVALENT_PLMN_STRU                                    stEplmn;
    VOS_UINT8                                               i;
    NAS_MML_EQUPLMN_INFO_STRU                              *pstEquPlmnList = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU                                   *pstPlmnId      = VOS_NULL_PTR;
    NAS_MML_DISABLED_RAT_PLMN_CFG_INFO_STRU                *pstDisabledRatPlmnCfg= VOS_NULL_PTR;
    RRMM_EPLMN_QUERY_REQ_STRU                              *pstEqueryMsg;
    NAS_MML_PLMN_WITH_RAT_STRU                              stPlmnWithRat;

    NAS_MML_PLMN_ID_STRU                                    stCurPlmnId;
    VOS_UINT32                                              ulOpCurPlmnId;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstUserSpecPlmnId = VOS_NULL_PTR;
    VOS_UINT32                                              ulEplmnValidFlg;
    VOS_UINT32                                              ulRegFlag;

    VOS_UINT32                                              ulIsPlmnIdInDestPlmnWithRatList;
    VOS_UINT32                                              ulIsPlmnIdInForbidPlmnList;

    ulOpCurPlmnId     = VOS_FALSE;
    stCurPlmnId.ulMcc = NAS_MML_INVALID_MCC;
    stCurPlmnId.ulMnc = NAS_MML_INVALID_MNC;
    pstUserSpecPlmnId = NAS_MMC_GetUserSpecPlmnId();
    ulEplmnValidFlg   = NAS_MML_GetEplmnValidFlg();
    ulRegFlag         = NAS_MMC_GetUserSpecPlmnRegisterStatus();

    /* ��ȡ��ֹ���뼼����PLMN��Ϣ */
    pstDisabledRatPlmnCfg = NAS_MML_GetDisabledRatPlmnCfg();

    pstEqueryMsg = (RRMM_EPLMN_QUERY_REQ_STRU*)pstMsg;

    PS_MEM_SET(&stEplmn, 0, sizeof(stEplmn));
    pstEquPlmnList  = NAS_MML_GetEquPlmnList();

    pstPlmnId       = NAS_MML_GetCurrCampPlmnId();

    if (WUEPS_PID_WRR == pstEqueryMsg->MsgHeader.ulSenderPid)
    {
        stPlmnWithRat.enRat         = NAS_MML_NET_RAT_TYPE_WCDMA;
    }
    else
    {
        stPlmnWithRat.enRat         = NAS_MML_NET_RAT_TYPE_GSM;
    }

    if (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
    {
        stPlmnWithRat.stPlmnId = pstUserSpecPlmnId->stPlmnId;
    }
    else
    {
        stPlmnWithRat.stPlmnId = *pstPlmnId;
    }

    ulIsPlmnIdInDestPlmnWithRatList = NAS_MML_IsBcchPlmnIdWithRatInDestPlmnWithRatList(&stPlmnWithRat,
                                                                         pstDisabledRatPlmnCfg->ulDisabledRatPlmnNum,
                                                                         pstDisabledRatPlmnCfg->astDisabledRatPlmnId);

    ulIsPlmnIdInForbidPlmnList = NAS_MML_IsPlmnIdInForbidPlmnList(&stPlmnWithRat.stPlmnId);

    if ((VOS_FALSE == ulIsPlmnIdInDestPlmnWithRatList)
     && (VOS_FALSE == ulIsPlmnIdInForbidPlmnList))
    {
        ulOpCurPlmnId     = VOS_TRUE;
        stCurPlmnId.ulMcc = stPlmnWithRat.stPlmnId.ulMcc;
        stCurPlmnId.ulMnc = stPlmnWithRat.stPlmnId.ulMnc;
    }

    if (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
    {
        /* �ֶ�����ģʽ�ж�eplmn�б��Ƿ���Ч����Ч�򷵻� */
        if ((VOS_FALSE == ulRegFlag)
         || (VOS_FALSE == ulEplmnValidFlg))
        {
            stEplmn.ulEquPlmnNum = 0;

            NAS_MMC_SndAsEquPlmnQueryCnf(&stEplmn, ulOpCurPlmnId, &stCurPlmnId, pstMsg->ulSenderPid);
            return VOS_TRUE;
        }
    }

    /* �ѵ�Чplmn������������ */
    for ( i = 0; i < pstEquPlmnList->ucEquPlmnNum; i++ )
    {
        if (VOS_TRUE == NAS_MML_IsBcchPlmnIdInDestSimPlmnList(&(pstEquPlmnList->astEquPlmnAddr[i]),
                         (VOS_UINT8)stEplmn.ulEquPlmnNum, (NAS_MML_PLMN_ID_STRU *)stEplmn.aEquPlmnIdList))
        {
            continue;
        }

        if (WUEPS_PID_WRR == pstEqueryMsg->MsgHeader.ulSenderPid)
        {
            stPlmnWithRat.enRat         = NAS_MML_NET_RAT_TYPE_WCDMA;

        }
        else
        {
            stPlmnWithRat.enRat         = NAS_MML_NET_RAT_TYPE_GSM;
        }

        /* ��������뼼����PLMN ID */
        stPlmnWithRat.stPlmnId      = pstEquPlmnList->astEquPlmnAddr[i];

        /* �жϵ�ЧPLMN�Ƿ��ڽ�ֹ���뼼����PLMN�б���,������򲻽���PLMN ID����RRC */
        if (VOS_TRUE == NAS_MML_IsBcchPlmnIdWithRatInDestPlmnWithRatList(&stPlmnWithRat,
                                                                     pstDisabledRatPlmnCfg->ulDisabledRatPlmnNum,
                                                                     pstDisabledRatPlmnCfg->astDisabledRatPlmnId))
        {
            continue;
        }

        if (VOS_TRUE == NAS_MML_IsPlmnIdInForbidPlmnList(&(pstEquPlmnList->astEquPlmnAddr[i])))
        {
            continue;
        }

        stEplmn.aEquPlmnIdList[stEplmn.ulEquPlmnNum].ulMcc = pstEquPlmnList->astEquPlmnAddr[i].ulMcc;
        stEplmn.aEquPlmnIdList[stEplmn.ulEquPlmnNum].ulMnc = pstEquPlmnList->astEquPlmnAddr[i].ulMnc;
        stEplmn.ulEquPlmnNum++;
    }


    NAS_MMC_SndAsEquPlmnQueryCnf(&stEplmn, ulOpCurPlmnId, &stCurPlmnId, pstMsg->ulSenderPid);

    return VOS_TRUE;

}
VOS_UINT32  NAS_MMC_RcvRrMmLimitServiceCamp_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    RRMM_LIMIT_SERVICE_CAMP_IND_STRU    *pstLimitServiceMsg;
    NAS_MMC_PLMN_SEARCH_CTRL_CTX_STRU   *pstSearchCtrl;

    NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8 enCsMmaServiceStatus;
    NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8 enPsMmaServiceStatus;
    NAS_MML_REG_STATUS_ENUM_UINT8        enCsRegStatus;
    NAS_MML_REG_STATUS_ENUM_UINT8        enPsRegStatus;

    pstSearchCtrl        = NAS_MMC_GetPlmnSearchCtrl();

    pstLimitServiceMsg   = (RRMM_LIMIT_SERVICE_CAMP_IND_STRU *)pstMsg;

    if ( NAS_MML_NET_RAT_TYPE_WCDMA != NAS_MML_GetCurrNetRatType())
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmLimitServiceCamp_PreProc: receive wrong message");
        return VOS_TRUE;
    }

    if(VOS_FALSE == NAS_MML_GetSimPresentStatus())
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmLimitServiceCamp_PreProc: sim is not present");
        return VOS_TRUE;
    }


    if (RRC_LIMIT_CAMP_ACTION_START == pstLimitServiceMsg->enLimitCampAction)
    {
        pstSearchCtrl->ucWrrLimitServiceInd     = VOS_TRUE;

        if (VOS_FALSE == pstLimitServiceMsg->ulServiceAvailFlag)
        {
            NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS,
                                           MMA_MMC_SERVICE_STATUS_NO_SERVICE);


            NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS,
                                           MMA_MMC_SERVICE_STATUS_NO_SERVICE);

        }
        else
        {
            NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS,
                                           MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);

            NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS,
                                           MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE);
        }

        enCsRegStatus = NAS_MML_GetCsRegStatus();
        enPsRegStatus = NAS_MML_GetPsRegStatus();

        if ( (NAS_MML_REG_REGISTERED_HOME_NETWORK == enCsRegStatus)
          || (NAS_MML_REG_REGISTERED_ROAM == enCsRegStatus) )
        {
            /* �ϱ�ע��״̬�����ı� */
            NAS_MMC_SndMmaRegStatusInd(MMA_MMC_SRVDOMAIN_CS, NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        if ( (NAS_MML_REG_REGISTERED_HOME_NETWORK == enPsRegStatus)
          || (NAS_MML_REG_REGISTERED_ROAM == enPsRegStatus) )
        {
            /* �ϱ�ע��״̬�����ı� */
            NAS_MMC_SndMmaRegStatusInd(MMA_MMC_SRVDOMAIN_PS, NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }

        /* ֻ���ڿ����ṩ����,����RRC����,��ǰ�Ĳ���Ϊstart,
           �����ϱ���PLMN������Ϊ0ʱ,��Ҫ��������,��״̬���������������״̬��������VOS_TRUE */
        if (VOS_FALSE == NAS_MMC_IsNeedPlmnSearch_Oos(pstLimitServiceMsg))
        {
            return VOS_TRUE;
        }

        return VOS_FALSE;
    }
    else if (RRC_LIMIT_CAMP_ACTION_END == pstLimitServiceMsg->enLimitCampAction)
    {
        pstSearchCtrl->ucWrrLimitServiceInd     = VOS_FALSE;

        /* ulServiceAvailFlagΪVOS_TRUE,�����ؽ����������ϱ�֮ǰ�ķ���״̬ */
        if (VOS_TRUE == pstLimitServiceMsg->ulServiceAvailFlag)
        {
            enCsMmaServiceStatus = NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_CS);
            enPsMmaServiceStatus = NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_PS);

            NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_CAMP_ON);

            NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS,
                                           enCsMmaServiceStatus);

            NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS,
                                           enPsMmaServiceStatus);

            /* �ϱ�ע��״̬�����ı� */
            NAS_MMC_SndMmaRegStatusInd(MMA_MMC_SRVDOMAIN_CS, NAS_MML_GetCsRegStatus());

            /* �ϱ�ע��״̬�����ı� */
            NAS_MMC_SndMmaRegStatusInd(MMA_MMC_SRVDOMAIN_PS, NAS_MML_GetPsRegStatus());
        }
        /* ulServiceAvailFlagΪVOS_FALSE�������������ģʽ�л����˴����ϱ�����״̬ */
        else
        {
        }

        return VOS_FALSE;
    }
    else
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvRrMmLimitServiceCamp_PreProc: receive wrong message");
        return VOS_TRUE;
    }
}


VOS_VOID NAS_MMC_ProcActingHplmnRefresh_PreProc(
    VOS_UINT8                              *pucBufferRefresh
)
{
    VOS_UINT32                              ulEfLen;
    VOS_UINT8                              *pucEf               = VOS_NULL_PTR;
    NAS_MML_SIM_EHPLMN_INFO_STRU           *pstEHplmnList       = VOS_NULL_PTR;

    *pucBufferRefresh = VOS_FALSE;


    ulEfLen = 0;

    /* �����������NVδ�򿪣�ֱ�ӷ��� */
    if (VOS_FALSE == NAS_MML_GetActingHplmnSupportFlg())
    {
        return;
    }

    pstEHplmnList                       = NAS_MML_GetSimEhplmnList();

    /* ����API��ȡUSIM�ļ� */
    if (USIMM_API_SUCCESS != NAS_USIMMAPI_GetCachedFile(NAS_MML_READ_ACTING_HPLMN_FILE_ID, &ulEfLen, &pucEf, USIMM_ATT_APP))
    {
        /*����ǰ�ļ�ʧ�ܣ���ȡ��һ���ļ�*/
        NAS_WARNING_LOG(WUEPS_PID_MMC,"NAS_MMC_ProcActingHplmnRefresh_PreProc():ERROR:READ FILE FAIL!");
        return;
    }

    NAS_MMC_SndOmGetCacheFile(NAS_MML_READ_ACTING_HPLMN_FILE_ID, ulEfLen, pucEf);

    /* BG��״̬���������� */
    if (NAS_MMC_FSM_BG_PLMN_SEARCH == NAS_MMC_GetCurrFsmId())
    {
        *pucBufferRefresh = VOS_TRUE;
    }

    /* ��ȡ�ļ��ɹ���MMC�ڲ�����Ӧ���� */
    NAS_MMC_ProcAgentUsimReadFileCnf_PreProc(NAS_MML_READ_ACTING_HPLMN_FILE_ID, (VOS_UINT16)ulEfLen, pucEf);

    /* ����HPLMN���뼼�� */
    NAS_MMC_UpdateEhplmnRat();

    /* ��HPLMN���ֹ�б���ɾ�� */
    if (VOS_FALSE == NAS_MML_DelForbPlmn(&(pstEHplmnList->astEhPlmnInfo[0].stPlmnId)))
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC,"NAS_MMC_ProcActingHplmnRefresh_PreProc():ERROR:NAS_MML_DelForbPlmn FAIL!");
    }

    /* ��ʼ�������ȼ������б� */
    NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_PLMN_SEARCH,
                                  VOS_NULL_PTR,
                                  NAS_MMC_GetHighPrioPlmnList());

    /* �����㷢�͵�ǰ��HPLMN��EHPLMN */
    NAS_MMC_SndAsHplmnReq(UEPS_PID_GAS);
    NAS_MMC_SndAsHplmnReq(WUEPS_PID_WRR);

    /* ֹͣ�����ȶ�ʱ���������Գ��Զ�ʱ�� */
    NAS_MMC_StopTimer(TI_NAS_MMC_HPLMN_TIMER);
    NAS_MMC_StopTimer(TI_NAS_MMC_PERIOD_TRYING_HIGH_PRIO_PLMN_SEARCH);
    NAS_MMC_StopTimer(TI_NAS_MMC_HIGH_PRIO_RAT_HPLMN_TIMER);
    NAS_MMC_ResetCurHighPrioRatHplmnTimerFirstSearchCount_L1Main();

    /* �ж��Ƿ���Ҫ����BG�� */


    if (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
    {
        return;
    }

    if (VOS_TRUE == NAS_MMC_IsCampOnHighestPrioPlmn())
    {
        return;
    }

    /* ���������ȼ����Զ�ʱ����ʱ��1s */
    NAS_MMC_StartTimer(TI_NAS_MMC_PERIOD_TRYING_HIGH_PRIO_PLMN_SEARCH, TI_NAS_MMC_ACTING_HPLMN_REFRESH_TRYING_HIGH_PRIO_PLMN_SEARCH_LEN);

    return;
}


VOS_UINT32  NAS_MMC_ProHighPrioPlmnRefreshInd_PreProc(VOS_VOID)
{

    /* ��ǰ״̬����ʶ */
    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;

    /* ��ǰ״̬����״̬ */
    VOS_UINT32                          ulState;

    enFsmId = NAS_MMC_GetCurrFsmId();
    ulState = NAS_MMC_GetFsmTopState();

    /* BG��״̬����������,���浱ǰ��Ϣ����BG״̬���˳����� */
    if ( NAS_MMC_FSM_BG_PLMN_SEARCH == enFsmId )
    {
        return VOS_FALSE;
    }

    /* ��ʼ�������ȼ������б� */
    NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_PLMN_SEARCH,
                                  VOS_NULL_PTR,
                                  NAS_MMC_GetHighPrioPlmnList());

    /* ����ϵͳ��ON Plmn״̬�������������ȼ�������ʱ�� */
    if ( (NAS_MMC_FSM_INTER_SYS_CELLRESEL == enFsmId)
      || (NAS_MMC_FSM_INTER_SYS_OOS       == enFsmId)
      || (NAS_MMC_FSM_INTER_SYS_HO        == enFsmId)
      || (NAS_MMC_FSM_INTER_SYS_CCO       == enFsmId)
      || ( (NAS_MMC_FSM_L1_MAIN           == enFsmId)
        && (NAS_MMC_L1_STA_ON_PLMN        == ulState) ) )
    {
        /* �ж��Ƿ�����HPLMN */
        if (VOS_TRUE == NAS_MMC_IsNeedStartHPlmnTimer())
        {
            /* ������ʱ�� */
            NAS_MMC_StartTimer(TI_NAS_MMC_HPLMN_TIMER, NAS_MMC_GetHplmnTimerLen());

            /* HPLMN TIMER�����¼��ϱ� */
            NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_HPLMN_TIMER_START,
                            VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

            /* ��HPLMN������������Ϊ���״����� */
            NAS_MMC_SetFirstStartHPlmnTimerFlg(VOS_FALSE);
        }

        if (VOS_TRUE == NAS_MMC_IsNeedStartHighPrioRatHPlmnTimer())
        {
            /* ������ʱ�� */
            if (VOS_TRUE == NAS_MMC_StartTimer(TI_NAS_MMC_HIGH_PRIO_RAT_HPLMN_TIMER, NAS_MMC_GetHighPrioRatHplmnTimerLen()))
            {
                NAS_MMC_AddCurHighPrioRatHplmnTimerFirstSearchCount_L1Main();
            }
        }
    }

    /* ��Ϣ������ɣ�ֱ���˳� */
    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvUsimRefreshFileInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    PS_USIM_REFRESH_IND_STRU           *pstUsimRefreshInd = VOS_NULL_PTR;
    VOS_UINT32                          i;
    VOS_UINT32                          ulHighPrioPlmnRefreshFlag;
    VOS_UINT16                          usEfId;
    VOS_UINT32                          ulEfLen;
    VOS_UINT8                          *pucEf = VOS_NULL_PTR;

    VOS_UINT8                           ucBufferRefreshFlg;

    VOS_UINT32                          ulNvOplmnAvail = VOS_FALSE;
    NAS_MML_SIM_EHPLMN_INFO_STRU       *pstEHPlmnInfo = VOS_NULL_PTR;

    pstEHPlmnInfo                       = NAS_MML_GetSimEhplmnList();

    pstUsimRefreshInd                   = (PS_USIM_REFRESH_IND_STRU *)pstMsg;

    if (USIMM_REFRESH_FILE_LIST != pstUsimRefreshInd->enRefreshType)
    {
        /* refresh��ϢĿǰֻ����ָ���ļ�������� */
        return VOS_TRUE;
    }

    ulHighPrioPlmnRefreshFlag   =  VOS_FALSE;

    ucBufferRefreshFlg          =  VOS_FALSE;


    ulEfLen                     = 0;

    /* һ�α���USIM�ϱ����ļ�ID */
    for (i = 0; i < pstUsimRefreshInd->usEfNum; i++ )
    {
        usEfId = pstUsimRefreshInd->astEfId[i].usFileId;

        if ((NAS_MML_READ_ACTING_HPLMN_FILE_ID == usEfId)
         && (USIMM_ATT_APP == pstUsimRefreshInd->astEfId[i].enAppType))
        {
            NAS_MMC_ProcActingHplmnRefresh_PreProc(&ucBufferRefreshFlg);
            continue;
        }

        /* ����API��ȡUSIM�ļ� */
        if (USIMM_API_SUCCESS != NAS_USIMMAPI_GetCachedFile(usEfId, &ulEfLen, &pucEf, pstUsimRefreshInd->astEfId[i].enAppType))
        {
            /*����ǰ�ļ�ʧ�ܣ���ȡ��һ���ļ�*/
            NAS_WARNING_LOG(WUEPS_PID_MMC,"NAS_MMC_RcvUsimRefreshFileInd_PreProc():ERROR:READ FILE FAIL!");
            continue;
        }

        NAS_MMC_SndOmGetCacheFile(usEfId, ulEfLen, pucEf);

        /* ����UPLMN��OPLMN�ĸ���Ϊ0����ʹ��NAS_MML_READ_PLMN_SEL_FILE_ID���ļ����ݳ�ʼ�������ȼ��б� */
        ulNvOplmnAvail = NAS_MML_IsNvimOplmnAvail();
        if ((NAS_MML_READ_UPLMN_SEL_FILE_ID == usEfId)
         || ( (VOS_FALSE == ulNvOplmnAvail)
             && (NAS_MML_READ_OPLMN_SEL_FILE_ID == usEfId) )
        || (NAS_MML_READ_PLMN_SEL_FILE_ID == usEfId))

        {
            ulHighPrioPlmnRefreshFlag = VOS_TRUE;
        }

        /* ��ȡ�ļ��ɹ���MMC�ڲ�����Ӧ���� */
        NAS_MMC_ProcAgentUsimReadFileCnf_PreProc(usEfId, (VOS_UINT16)ulEfLen, pucEf);

        if (NAS_MML_READ_EHPLMN_FILE_ID == usEfId)
        {
            /* ��Ҫ���¸����ȼ������б��� */
            ulHighPrioPlmnRefreshFlag = VOS_TRUE;

            /* ����HPLMN���뼼�� */
            NAS_MMC_UpdateEhplmnRat();

            /* ɾ��FPLMN�е�HPLMN */
            NAS_MMC_DelHplmnInFplmn();

            /* �����㷢�͵�ǰ��HPLMN��EHPLMN */
            NAS_MMC_SndAsHplmnReq(UEPS_PID_GAS);
            NAS_MMC_SndAsHplmnReq(WUEPS_PID_WRR);
        }

        if (NAS_MML_READ_HPLMN_SEL_FILE_ID == usEfId)
        {
            /* ��Ҫ���¸����ȼ������б��� */
            ulHighPrioPlmnRefreshFlag = VOS_TRUE;

            /* HOME PLMN WITH RAT������ˢ�º���Ҫ��SIM�е�EHplmn���з��򹹽���
              �������뼼�����г�ʼ��ΪĬ��ֵ��ɾ���ظ���PLMN ID */
            NAS_MML_ResumeSimEhplmnList(&pstEHPlmnInfo->ucEhPlmnNum,
                                         pstEHPlmnInfo->astEhPlmnInfo);

            /* ����HPLMN���뼼�� */
            NAS_MMC_UpdateEhplmnRat();
        }

        /* 6F31�ļ������������� */
        if (NAS_MML_READ_HPLMN_PERI_FILE_ID == usEfId)
        {
            NAS_MMC_ProcScanCtrl_PreProc((VOS_UINT16)ulEfLen, pucEf);
        }

    }



    /* H��U��O�ļ������仯��Ҫˢ��ʱ,���BG����״̬��������ʱ,Ϊ����Ե�ǰ�����б����Ӱ��
       �Ȼ��浱ǰUSIM�ļ�ˢ����Ϣ��BG����״̬���˳�����ˢ�¸����ȼ������б�;����ֱ��ˢ����
       �ȼ������б� */

    if (VOS_TRUE == ucBufferRefreshFlg)
    {
        return VOS_FALSE;
    }

    if (VOS_TRUE == ulHighPrioPlmnRefreshFlag)
    {
        return NAS_MMC_ProHighPrioPlmnRefreshInd_PreProc();
    }


    return VOS_FALSE;
}




VOS_UINT32  NAS_MMC_RcvMmaUpdateUplmnNtf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRslt;

    /* ���и����ȼ��б�ˢ�µ�Ԥ���� */
    ulRslt = NAS_MMC_ProHighPrioPlmnRefreshInd_PreProc();

    return ulRslt;
}
VOS_VOID  NAS_MMC_ProcAgentUsimReadFileCnf_PreProc(
    VOS_UINT16                          usEfId,
    VOS_UINT16                          usEfLen,
    VOS_UINT8                          *pucEf
)
{
    PS_USIM_GET_FILE_CNF_STRU          *pstUsimMmcMsg = VOS_NULL_PTR;

    pstUsimMmcMsg = (PS_USIM_GET_FILE_CNF_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC, (VOS_UINT32)(sizeof(PS_USIM_GET_FILE_CNF_STRU) - 4 + usEfLen));

    if (VOS_NULL_PTR == pstUsimMmcMsg)
    {
       NAS_WARNING_LOG(WUEPS_PID_MMC,"NAS_MMC_ProcAgentUsimReadFileCnf_PreProc():ERROR:MALLOC FAIL");
       return;
    }

    PS_MEM_SET(pstUsimMmcMsg, 0, (VOS_UINT32)(sizeof(PS_USIM_GET_FILE_CNF_STRU) - 4 + usEfLen));

    /* ��ȡ��USIM�ļ����ݿ������� */
    pstUsimMmcMsg->ulResult = VOS_OK;
    pstUsimMmcMsg->usEfLen  = usEfLen;
    pstUsimMmcMsg->usEfId   = usEfId;
    PS_MEM_CPY(pstUsimMmcMsg->aucEf, pucEf, usEfLen);

    NAS_MMC_RcvAgentUsimReadFileCnf((PS_USIM_GET_FILE_RSP_FOR_PCLINT_STRU*)pstUsimMmcMsg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstUsimMmcMsg);

}
VOS_UINT32 NAS_MMC_IsMmaAttachTypeValid(
    MMA_MMC_ATTACH_TYPE_ENUM_UINT32     enAttachType
)
{
    if ((MMA_MMC_ATTACH_TYPE_IMSI      != enAttachType)
     && (MMA_MMC_ATTACH_TYPE_GPRS      != enAttachType)
     && (MMA_MMC_ATTACH_TYPE_GPRS_IMSI != enAttachType))
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvMmaAttachReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ������Ҫ�����Attach����֪ͨMM,GMM����ע�� */
    MMA_MMC_ATTACH_REQ_STRU             *pstTafAttachReq;
    VOS_UINT32                          ulForbType = NAS_MML_PLMN_FORBID_NULL;

#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     ucCurrNetRatType;

    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;
#endif


    pstTafAttachReq = (MMA_MMC_ATTACH_REQ_STRU*)pstMsg;

#if (FEATURE_ON == FEATURE_LTE)
    ucCurrNetRatType = NAS_MML_GetCurrNetRatType();

    enLteCapabilityStatus = NAS_MML_GetLteCapabilityStatus();
#endif

    /* attach���ʹ��󣬶��� */
    if (VOS_FALSE == NAS_MMC_IsMmaAttachTypeValid(pstTafAttachReq->enAttachType))
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC,"NAS_MMC_RcvMmaAttachReq_PreProc: NORMAL: Attach Type is invalid.");
        return VOS_TRUE;
    }

#if (FEATURE_ON == FEATURE_LTE)
    if ((VOS_FALSE == NAS_MML_GetPsAttachAllowFlg())
     && (MMA_MMC_ATTACH_TYPE_IMSI != pstTafAttachReq->enAttachType))
    {
        if (VOS_TRUE == NAS_MMC_IsNeedEnableLte_AttachPs())
        {
            /* LTE�������ε��µ�disable LTE,��ʱ����֪ͨGU����� */
            if ((VOS_FALSE == NAS_MML_GetDisableLteRoamFlg())
             && (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLteCapabilityStatus))
            {
                /* ��WAS/GAS����enable LTE֪ͨ��Ϣ */
                NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);
                NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);

                /* ��LMM����enable LTE��Ϣ */
                NAS_MMC_SndLmmEnableLteNotify();
            }

            /* ����disable LTE������� */
            NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
        }
    }


#endif



    if (NAS_MMC_SPEC_PLMN_SEARCH_STOP == NAS_MMC_GetSpecPlmnSearchState())
    {
        ulForbType = NAS_MMC_GetCurrentLaiForbbidenType();
    }



#if (FEATURE_ON == FEATURE_LTE)
    /* ��ǰ��L�£���������ֹL��ʱ��֪ͨLMM����ATTACH,���յ�LTE��ATTACH CNFʱ���ж��Ƿ���Ҫ�������� */
    if ( (VOS_TRUE                 == NAS_MML_IsRatInForbiddenList(NAS_MML_NET_RAT_TYPE_LTE))
      && (NAS_MML_NET_RAT_TYPE_LTE == ucCurrNetRatType) )
    {
         NAS_MMC_SndLmmAttachReq(pstTafAttachReq->ulOpID, pstTafAttachReq->enAttachType, pstTafAttachReq->enEpsAttachReason);

#ifdef NAS_STUB
        if (MMA_MMC_ATTACH_TYPE_IMSI != pstTafAttachReq->enAttachType)
        {
            NAS_MML_SetPsAttachAllowFlg(VOS_TRUE);
        }
#endif
#ifdef __PC_TRACE_RECUR__
        if (MMA_MMC_ATTACH_TYPE_IMSI != pstTafAttachReq->enAttachType)
        {
            NAS_MML_SetPsAttachAllowFlg(VOS_TRUE);
        }
#endif
        return VOS_TRUE;
    }
#endif

    if (NAS_MML_PLMN_FORBID_NULL != ulForbType)
    {
        if (VOS_TRUE == NAS_MMC_ProcMmaAttachReqInForbLa_PreProc(pstTafAttachReq, ulForbType))
        {
            return VOS_TRUE;
        }
    }


#if   (FEATURE_ON == FEATURE_LTE)
    /* ��LMM����ATTACH���� */
    if ( NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
    {
        NAS_MMC_SndLmmAttachReq(pstTafAttachReq->ulOpID, pstTafAttachReq->enAttachType, pstTafAttachReq->enEpsAttachReason);

#ifdef NAS_STUB
        if (MMA_MMC_ATTACH_TYPE_IMSI != pstTafAttachReq->enAttachType)
        {
            NAS_MML_SetPsAttachAllowFlg(VOS_TRUE);
        }
#endif
#ifdef __PC_TRACE_RECUR__
        if (MMA_MMC_ATTACH_TYPE_IMSI != pstTafAttachReq->enAttachType)
        {
            NAS_MML_SetPsAttachAllowFlg(VOS_TRUE);
        }
#endif
    }

    else
#endif
    {
        /* ��MM����ATTACH���� */
        NAS_MMC_SndMmAttachReq(pstTafAttachReq->ulOpID, pstTafAttachReq->enAttachType);

        /* ��GMM����ATTACH���� */
        NAS_MMC_SndGmmAttachReq(pstTafAttachReq->ulOpID, pstTafAttachReq->enAttachType);
    }


    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvGmmGprsServiceInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
#if (FEATURE_ON == FEATURE_LTE)
    MMCGMM_GPRS_SERVICE_IND_STRU                           *pstGprsServiceInd;
    NAS_MML_LTE_CAPABILITY_STATUS_ENUM_UINT32               enLteCapabilityStatus;

    enLteCapabilityStatus = NAS_MML_GetLteCapabilityStatus();
    pstGprsServiceInd     = (MMCGMM_GPRS_SERVICE_IND_STRU *)pstMsg;

    if (NAS_MMC_GMM_GPRS_SERVICE_ATTACH == pstGprsServiceInd->enGprsServiceType)
    {
        if (VOS_TRUE == NAS_MMC_IsNeedEnableLte_AttachPs())
        {
            /* LTE�������ε��µ�disable LTE,��ʱ����֪ͨGU����� */
            if ((VOS_FALSE == NAS_MML_GetDisableLteRoamFlg())
             && (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == enLteCapabilityStatus))
            {
                /* ��WAS/GAS����enable LTE֪ͨ��Ϣ */
                NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);
                NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);

                /* ��LMM����enable LTE��Ϣ */
                NAS_MMC_SndLmmEnableLteNotify();
            }

            /* ����disable LTE������� */
            NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
        }
    }
#endif

    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvMmaSignReportReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMA_MMC_SIGN_REPORT_REQ_STRU        *pstSignReport = VOS_NULL_PTR;
    NAS_MMC_MAINTAIN_CTX_STRU           *pstMainCtx    = VOS_NULL_PTR;

    pstSignReport = (MMA_MMC_SIGN_REPORT_REQ_STRU *)pstMsg;


    if ( (pstSignReport->ucActionType > NAS_MMC_AT_ACTION_TYPE_STOP)
      || (pstSignReport->ucRrcMsgType > NAS_MML_SIGN_REPORT_MSG_TYPE))
    {

        NAS_WARNING_LOG(WUEPS_PID_MMC,"NAS_MMC_RcvMmaSignReportReq_PreProc: NORMAL: ucActionType or ucRrcMsgType is invalid.");
        return VOS_TRUE;
    }

    pstMainCtx = NAS_MMC_GetMaintainInfo();

    pstMainCtx->ucActionType  = pstSignReport->ucActionType;
    pstMainCtx->ucMsgType     = pstSignReport->ucRrcMsgType;

    if ((pstSignReport->ucSignThreshold != pstMainCtx->ucSignThreshold)
     || (pstSignReport->ucMinRptTimerInterval != pstMainCtx->ucMinRptTimerInterval))
    {
        pstMainCtx->ucSignThreshold       = pstSignReport->ucSignThreshold;
        pstMainCtx->ucMinRptTimerInterval = pstSignReport->ucMinRptTimerInterval;

        /* �ź��������޻��߼��ʱ��ı�ʱдnv */
        NAS_MMC_WriteCellSignReportCfgNvim(pstMainCtx->ucSignThreshold,pstMainCtx->ucMinRptTimerInterval);
    }

    /* ֪ͨ������ź��������޺�ʱ���� */
    NAS_MMC_SndWasInfoReportReq();
    NAS_MMC_SndGasInfoReportReq();

    return VOS_TRUE;
}



#if (FEATURE_MULTI_MODEM == FEATURE_ON)

VOS_UINT32  NAS_MMC_RcvMmaOtherModemInfoNotify_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
#if   (FEATURE_ON == FEATURE_LTE)
    NAS_MML_PLMN_ID_STRU                                    stOtherModemPlmnId;
    MMC_LMM_OTHER_MODEM_INFO_NOTIFY_STRU                    stSndLmmOtherModemInfoMsg;
#endif

    MMA_MMC_OTHER_MODEM_INFO_NOTIFY_STRU                   *pstOtherModemInfoNotify    = VOS_NULL_PTR;

    NAS_MMC_NCELL_SEARCH_INFO_STRU                         *pstMmcNcellSearchInfo = VOS_NULL_PTR;

    pstMmcNcellSearchInfo = NAS_MMC_GetNcellSearchInfo();

    pstOtherModemInfoNotify = (MMA_MMC_OTHER_MODEM_INFO_NOTIFY_STRU *)pstMsg;

    PS_MEM_SET(&stSndLmmOtherModemInfoMsg, 0, sizeof(stSndLmmOtherModemInfoMsg));

    if (VOS_TRUE == pstOtherModemInfoNotify->bitOpCurrCampPlmnId)
    {
        stOtherModemPlmnId.ulMcc = pstOtherModemInfoNotify->stCurrCampPlmnId.ulMcc;
        stOtherModemPlmnId.ulMnc = pstOtherModemInfoNotify->stCurrCampPlmnId.ulMnc;
        NAS_MMC_SetOtherModemPlmnId(&stOtherModemPlmnId);

#if (FEATURE_ON == FEATURE_LTE)
        /* ƽ̨����֧��L��֪ͨL */
        if (VOS_TRUE == NAS_MML_IsPlatformSupportLte())
        {
            stSndLmmOtherModemInfoMsg.bitOpCurrCampPlmnId = VOS_TRUE;
            NAS_MML_ConvertNasPlmnToLMMFormat(&(stSndLmmOtherModemInfoMsg.stCurrCampPlmnId), &stOtherModemPlmnId);

            /* ֪ͨL����פ��plmn��Ϣ */
            NAS_MMC_SndLmmOtherModemInfoNotify(&stSndLmmOtherModemInfoMsg);
        }
#endif
    }

    if (VOS_TRUE == pstOtherModemInfoNotify->bitOpEplmnInfo)
    {
        pstMmcNcellSearchInfo->stOtherModemEplmnInfo.ucEquPlmnNum = pstOtherModemInfoNotify->stEplmnInfo.ucEquPlmnNum;
        PS_MEM_CPY(pstMmcNcellSearchInfo->stOtherModemEplmnInfo.astEquPlmnAddr, pstOtherModemInfoNotify->stEplmnInfo.astEquPlmnAddr,
                   sizeof(pstMmcNcellSearchInfo->stOtherModemEplmnInfo.astEquPlmnAddr));
    }

    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvMmaNcellInfoNotify_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMA_MMC_NCELL_INFO_NOTIFY_STRU                         *pstNcellInfoInd = VOS_NULL_PTR;
    NAS_MMC_NCELL_SEARCH_INFO_STRU                         *pstMmcNcellSearchInfo = VOS_NULL_PTR;
    VOS_UINT8                                               ucIsNeedNcellSearch;
    VOS_UINT8                                               ucIsNcellInfoChgFromNone;

    ucIsNcellInfoChgFromNone = VOS_FALSE;
    ucIsNeedNcellSearch      = VOS_FALSE;

    pstNcellInfoInd = (MMA_MMC_NCELL_INFO_NOTIFY_STRU *)pstMsg;
    pstMmcNcellSearchInfo = NAS_MMC_GetNcellSearchInfo();

    if (((0 == pstMmcNcellSearchInfo->stTdsNcellInfo.ucTdsArfcnNum)
      && (0 == pstMmcNcellSearchInfo->stLteNcellInfo.ucLteArfcnNum) )
     && ((0 != pstNcellInfoInd->stTdsNcellInfo.ucTdsArfcnNum)
      || (0 != pstNcellInfoInd->stLteNcellInfo.ucLteArfcnNum) ) )
    {
        ucIsNcellInfoChgFromNone = VOS_TRUE;
    }

    /* ����mmc ���ֵ�ncell���ȫ�ֱ���*/
    pstMmcNcellSearchInfo->stTdsNcellInfo.ucTdsArfcnNum = pstNcellInfoInd->stTdsNcellInfo.ucTdsArfcnNum;
    PS_MEM_CPY(pstMmcNcellSearchInfo->stTdsNcellInfo.ausTdsArfcnList, pstNcellInfoInd->stTdsNcellInfo.ausTdsArfcnList,
               sizeof(pstMmcNcellSearchInfo->stTdsNcellInfo.ausTdsArfcnList));

    pstMmcNcellSearchInfo->stLteNcellInfo.ucLteArfcnNum = pstNcellInfoInd->stLteNcellInfo.ucLteArfcnNum;
    PS_MEM_CPY(pstMmcNcellSearchInfo->stLteNcellInfo.ausLteArfcnList, pstNcellInfoInd->stLteNcellInfo.ausLteArfcnList,
               sizeof(pstMmcNcellSearchInfo->stLteNcellInfo.ausLteArfcnList));


    /* �յ�tds��lte����Ƶ����Ϣ�������ʱ��ͨ��available��ʱ�������У�
       ����1 mian������Ҫֹͣ��������1s��ʱ����1s��ʱ����ʱ����ncell�� */
    if (NAS_MMC_FSM_L1_MAIN == NAS_MMC_GetCurrFsmId())
    {
        ucIsNeedNcellSearch = NAS_MMC_IsNeedNcellSearch();
    }

    if ((NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_AVAILABLE_TIMER))
     && (VOS_TRUE == ucIsNeedNcellSearch)
     && (VOS_TRUE == ucIsNcellInfoChgFromNone))
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}



VOS_UINT32  NAS_MMC_RcvMmaPsTransferInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMA_MMC_PS_TRANSFER_IND_STRU       *pstPsTransferInd    = VOS_NULL_PTR;
    NAS_MML_AVAIL_TIMER_CFG_STRU       *pstAvailTimerCfg    = VOS_NULL_PTR;
    NAS_MMC_TIMER_STATUS_ENUM_U8        enAvailableTimerStatus;
    VOS_UINT8                           ucPsAttachAllow;
    NAS_MML_MS_MODE_ENUM_UINT8          enMsMode;

    enAvailableTimerStatus = NAS_MMC_GetTimerStatus(TI_NAS_MMC_AVAILABLE_TIMER);
    ucPsAttachAllow        = NAS_MML_GetPsAttachAllowFlg();
    enMsMode               = NAS_MML_GetMsMode();

    pstPsTransferInd       = (MMA_MMC_PS_TRANSFER_IND_STRU *)pstMsg;

    /* ���pstransfer�ϱ�Ϊ1���ҵ�ǰavailable��ʱ�������У���ֹͣ��������available��ʱ�� */
    if ( (NAS_MMC_FSM_L1_MAIN == NAS_MMC_GetCurrFsmId())
      && (NAS_MML_MS_MODE_PS_ONLY == enMsMode)
      && (VOS_FALSE == ucPsAttachAllow)
      && (NAS_MMC_TIMER_STATUS_RUNING == enAvailableTimerStatus) )
    {
        NAS_MMC_StopTimer(TI_NAS_MMC_AVAILABLE_TIMER);

        /* ������ʽ��ѡ����������ʱ�� */
        if (MTC_PS_TRANSFER_OFF_AREA != pstPsTransferInd->enSolutionCfg)
        {
            NAS_MMC_StartTimer(TI_NAS_MMC_AVAILABLE_TIMER, NAS_MMC_GetNextAvailableTimerValue());
        }
        else
        {
            pstAvailTimerCfg           = NAS_MML_GetAvailTimerCfg();

            NAS_MMC_SetCurNormalAvailableTimerCount(pstAvailTimerCfg->ulFirstSearchTimeCount);

            NAS_MMC_StartTimer(TI_NAS_MMC_AVAILABLE_TIMER, NAS_MMC_GetNextAvailableTimerValue());
        }
    }

    return VOS_TRUE;
}


#endif


VOS_UINT32 NAS_MMC_ProcMmaAttachReqInForbLa_PreProc(
    MMA_MMC_ATTACH_REQ_STRU            *pstTafAttachReq,
    VOS_UINT32                          ulForbType
)
{
    NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8 enCsMmaServiceStatus;
    NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8 enPsMmaServiceStatus;

#if (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                           ulDisableLteRoamFlg;

    VOS_UINT32                           ulNeedEnableLte;
#endif

    VOS_UINT32                           ulResult;

#if (FEATURE_ON == FEATURE_LTE)
    ulDisableLteRoamFlg         = NAS_MML_GetDisableLteRoamFlg();
#endif

    enCsMmaServiceStatus        = NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_CS);
    enPsMmaServiceStatus        = NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_PS);
#if (FEATURE_ON == FEATURE_LTE)
    ulNeedEnableLte             = VOS_FALSE;
#endif

    ulResult                    = VOS_FALSE;

    switch (pstTafAttachReq->enAttachType)
    {
        case MMA_MMC_ATTACH_TYPE_GPRS:

            NAS_MML_SetPsAttachAllowFlg(VOS_TRUE);

            /* PS Attach��Fobid,�ظ�Attach��� */
            NAS_MMC_SndMmaAttachCnf(MMA_MMC_SRVDOMAIN_PS,
                                    enPsMmaServiceStatus,
                                    pstTafAttachReq->ulOpID);

#if (FEATURE_ON == FEATURE_LTE)
            ulNeedEnableLte = NAS_MMC_IsNeedEnableLte_AttachPs();
#endif

            /* ��ǰ���ڽ�ֹ����, ���ܷ��� PS ATTACH ����, ������� */
            ulResult        = VOS_TRUE;

            break;

        case MMA_MMC_ATTACH_TYPE_IMSI :

            if (NAS_MML_PLMN_FORBID_PLMN_FOR_GPRS != ulForbType)
            {
                NAS_MML_SetCsAttachAllowFlg(VOS_TRUE);

                /* ֱ�ӻظ�ATTACH ��� */
                NAS_MMC_SndMmaAttachCnf(MMA_MMC_SRVDOMAIN_CS,
                                        enCsMmaServiceStatus,
                                        pstTafAttachReq->ulOpID);


                /* ��������ܷ��� IMSI ATTACH, ������� */
                ulResult = VOS_TRUE;

            }
            else
            {
                /* GPRS��ֹ����¿��Է��� IMSI ATTACH, ��������Ҫ��������, δ������� */
                ulResult = VOS_FALSE;

            }

            break;

        case MMA_MMC_ATTACH_TYPE_GPRS_IMSI :
            if (NAS_MML_PLMN_FORBID_PLMN_FOR_GPRS == ulForbType)
            {
                NAS_MML_SetPsAttachAllowFlg(VOS_TRUE);

                /* CS+PS Attach,��gprs not allow,����CS Attach */
                NAS_MMC_SndMmaAttachCnf(MMA_MMC_SRVDOMAIN_PS,
                                        enPsMmaServiceStatus,
                                        pstTafAttachReq->ulOpID);
                pstTafAttachReq->enAttachType = MMA_MMC_ATTACH_TYPE_IMSI;


                /* ��������Է��� IMSI ATTACH, ��������Ҫ��������, δ������� */
                ulResult = VOS_FALSE;

            }
            else
            {
                NAS_MML_SetCsAttachAllowFlg(VOS_TRUE);
                NAS_MML_SetPsAttachAllowFlg(VOS_TRUE);

                /* ֱ�ӻظ���� */
                NAS_MMC_SndMmaAttachCnf(MMA_MMC_SRVDOMAIN_CS,
                                        enCsMmaServiceStatus,
                                        pstTafAttachReq->ulOpID);
                NAS_MMC_SndMmaAttachCnf(MMA_MMC_SRVDOMAIN_PS,
                                        enPsMmaServiceStatus,
                                        pstTafAttachReq->ulOpID);


                /* ��������ܷ��� GPRS_IMSI ATTACH, ������� */
                ulResult = VOS_TRUE;
            }

#if (FEATURE_ON == FEATURE_LTE)
            ulNeedEnableLte = NAS_MMC_IsNeedEnableLte_AttachCsPs();
#endif
            break;

        default:

            /* attach���ʹ��󣬶��� */
            NAS_WARNING_LOG(WUEPS_PID_MMC,"NAS_MMC_RcvMmaAttachReq_PreProc: NORMAL: Attach Type is invalid.");
            return VOS_TRUE;
    }

#if (FEATURE_ON == FEATURE_LTE)


    /* �ж��Ƿ���Ҫenable LTE */
    if (VOS_TRUE == ulNeedEnableLte)
    {
        ulDisableLteRoamFlg = NAS_MML_GetDisableLteRoamFlg();

        /* ������ڽ�ֹLTE�������ε��µ�disable LTE����δ�ص�����֮ǰ��ʱҲ��֪ͨenable LTE */
        if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == NAS_MML_GetLteCapabilityStatus())
         && (VOS_FALSE == ulDisableLteRoamFlg))
        {
            /* ��WAS/GAS����enable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);

            /* ��LMM����enable LTE��Ϣ */
            NAS_MMC_SndLmmEnableLteNotify();
        }

        if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER))
        {
            NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER);
        }

        /* ����disable LTE������� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }

#endif

    return ulResult;

}
VOS_UINT32  NAS_MMC_RcvMmaDetachReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                        *pstMsg
)
{
    MMA_MMC_DETACH_REQ_STRU             *pstTafDetachReq = VOS_NULL_PTR;
    VOS_UINT32                          ulRet;

#if   (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                          ulLteOnlyFlg;
#endif

    pstTafDetachReq = (MMA_MMC_DETACH_REQ_STRU *)pstMsg;

    /* �����������ĺϷ��� */
    if ( ( pstTafDetachReq->enDetachType > MMA_MMC_DETACH_TYPE_GPRS_IMSI )
      || ( pstTafDetachReq->enDetachType == MMA_MMC_DETACH_NULL ))
    {
        NAS_WARNING_LOG1(WUEPS_PID_MMC, "NAS_MMC_RcvMmaDetachReq_PreProc:invalid parm", pstTafDetachReq->enDetachType);
        return VOS_TRUE;
    }

    /* ������Ϣ���ȼ��Ƚ�,�жϵ�ǰ�Ƿ���Ҫ��ϵ�ǰ״̬���������Ϣ�Ļ��� */
    ulRet = NAS_MMC_ProcMsgPrio(ulEventType, pstMsg);

    /* ��Ҫ��ϵ�ǰ״̬���������Ϣ�Ļ���,���ʾԤ������� */
    if ( VOS_TRUE == ulRet )
    {
        /* ����LMM��MM/GMM����detach ����,����������Ϣʱ
           �ٷ���detach ���� */
        return VOS_TRUE;
    }

    /* ���ݵ�ǰ�Ľ���ģʽ����detach����,����Ϣ������ɲ��ٽ���״̬
       ������ */
#if   (FEATURE_ON == FEATURE_LTE)
    ulLteOnlyFlg = NAS_MML_IsLteOnlyMode(NAS_MML_GetMsPrioRatList());

    if (( NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
     || (VOS_TRUE == ulLteOnlyFlg))
    {
        /* ��LMM����DETACH���� */
        NAS_MMC_SndLmmDetachReq(pstTafDetachReq->ulOpID,(VOS_UINT8)pstTafDetachReq->enDetachType, pstTafDetachReq->enDetachReason);
    }
    else
#endif
    {
        /* ��MM����DETACH���� */
        NAS_MMC_SndMmDetachReq(pstTafDetachReq->ulOpID,(VOS_UINT8)pstTafDetachReq->enDetachType);

        /* ��GMM����DETACH���� */
        NAS_MMC_SndGmmDetachReq(pstTafDetachReq->ulOpID,(VOS_UINT8)pstTafDetachReq->enDetachType);
    }

    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvMmAttachCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMCMM_ATTACH_CNF_STRU              *pstMmAttachCnf;
    NAS_MMC_SERVICE_ENUM_UINT8          enService;


    pstMmAttachCnf = (MMCMM_ATTACH_CNF_STRU*)pstMsg;

    /* ��MM�ķ���״̬ת��ΪMMC ��*/
    enService = NAS_MMC_ConverMmStatusToMmc(NAS_MMC_REG_DOMAIN_CS,
                                        (NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8)pstMmAttachCnf->ulServiceStatus);
    /* ����CS����״̬ */
    NAS_MMC_SetCsServiceStatus(enService);

    /* ��װ��Ϣ����MMA����ATTACH��� */
    NAS_MMC_SndMmaAttachCnf(MMA_MMC_SRVDOMAIN_CS,
                            (NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8)pstMmAttachCnf->ulServiceStatus,
                            pstMmAttachCnf->ulOpid);


#if   (FEATURE_ON == FEATURE_LTE)
    if (VOS_TRUE == NAS_MMC_IsNeedDisableLte_AttachCs())
    {
        /* ��ǰ����ΪLTE������ʱ������disable LTE��Ϣ */
        if (NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
        {
            NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS);
        }
        else
        {
            /* ������ε��µ�disable�Ѿ�֪ͨ��GU�����,��ʱ����Ҫ���ظ�֪ͨ */
            if (VOS_FALSE == NAS_MML_GetDisableLteRoamFlg())
            {
                /* ��WAS/GAS����disable LTE֪ͨ��Ϣ */
                NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_DISABLE);
                NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_DISABLE);

                /* ��LMM����disable LTE��Ϣ */
                NAS_MMC_SndLmmDisableLteNotify(MMC_LMM_DISABLE_LTE_REASON_PS_USIM_INVALID);
            }

            /* ����disable LTE������� */
            NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS);
        }

        if (VOS_FALSE == NAS_MML_GetPsAttachAllowFlg())
        {
            NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_PS_DETACH);
        }
        else
        {
            NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_PS_USIM_INVALID);
        }

    }
#endif

    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvGmmAttachCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMCGMM_ATTACH_CNF_STRU             *pstGmmAttachCnf;
    NAS_MMC_SERVICE_ENUM_UINT8          enService;


    pstGmmAttachCnf = (MMCGMM_ATTACH_CNF_STRU*)pstMsg;

    /* ��GMM�ķ���״̬ת��ΪMMC ��*/
    enService = NAS_MMC_ConverMmStatusToMmc(NAS_MMC_REG_DOMAIN_PS,
                                        (NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8)pstGmmAttachCnf->ulServiceStatus);

    /* ����PS��ķ���״̬ */
    NAS_MMC_SetPsServiceStatus(enService);

    /* ��װ��Ϣ����MMA����ATTACH��� */
    NAS_MMC_SndMmaAttachCnf(MMA_MMC_SRVDOMAIN_PS,
                            (NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8)pstGmmAttachCnf->ulServiceStatus,
                            pstGmmAttachCnf->ulOpid);


    return VOS_TRUE;
}
VOS_UINT32  NAS_MMC_RcvMmDetachCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMCMM_DETACH_CNF_STRU              *pstMmDetachCnf = VOS_NULL_PTR;
    NAS_MMC_SERVICE_ENUM_UINT8          enCsServiceStatus;
#if (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                          ulDisableLteRoamFlg;
#endif

    VOS_UINT8                           ucSimCsRegStatus;

    pstMmDetachCnf = (MMCMM_DETACH_CNF_STRU*)pstMsg;

    enCsServiceStatus = NAS_MMC_ConverMmStatusToMmc(NAS_MMC_REG_DOMAIN_CS,
            (NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8)(pstMmDetachCnf->ulServiceStatus));

    /* ����CS����״̬ */
    NAS_MMC_SetCsServiceStatus(enCsServiceStatus);


    /* ��MMA����DETACH��� */
    NAS_MMC_SndMmaDetachCnf(MMA_MMC_SRVDOMAIN_CS,
                            (NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8)pstMmDetachCnf->ulServiceStatus,
                            pstMmDetachCnf->ulOpid);


    /* ���CS������ע�����CS����Ч���ϱ�csע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH*/
    ucSimCsRegStatus = NAS_MML_GetSimCsRegStatus();
    if ((VOS_FALSE == NAS_MML_GetCsAttachAllowFlg())
     && (VOS_TRUE == ucSimCsRegStatus))
    {
        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

#if (FEATURE_ON == FEATURE_LTE)
    if (VOS_FALSE == NAS_MML_GetCsAttachAllowFlg())
    {
        /* GU��MO detach �ɹ���Ҫ֪ͨLMM */
        NAS_MMC_SndLmmMoDetachIndActionResultReq(MMC_LMM_MO_DET_CS_ONLY);
    }

    if (VOS_TRUE == NAS_MMC_IsNeedEnableLte_DetachCs())
    {
        ulDisableLteRoamFlg = NAS_MML_GetDisableLteRoamFlg();

        /* ������ڽ�ֹLTE�������ε��µ�disable LTE����δ�ص�����֮ǰ��ʱҲ��֪ͨenable LTE */
        if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == NAS_MML_GetLteCapabilityStatus())
         && (VOS_FALSE == ulDisableLteRoamFlg))
        {
            /* ��WAS/GAS����enable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);

            /* ��LMM����enable LTE��Ϣ */
            NAS_MMC_SndLmmEnableLteNotify();
        }

        /* ����disable LTE������� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }
#endif

    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_IsNeedStartAvailableTimer_PsDetachCnf(VOS_VOID)
{
    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;
    VOS_UINT8                           ucIsSvlteSupportFlag;
    VOS_UINT8                           ucCsAttachAllowFlg;
    VOS_UINT8                           ucPsAttachAllowFlg;
    VOS_UINT32                          ulIsUsimStatusValid;
    NAS_MMC_TIMER_STATUS_ENUM_U8        enAvailableTimerStatus;

    enFsmId                = NAS_MMC_GetCurrFsmId();
    ucIsSvlteSupportFlag   = NAS_MML_GetSvlteSupportFlag();
    ucCsAttachAllowFlg     = NAS_MML_GetCsAttachAllowFlg();
    ucPsAttachAllowFlg     = NAS_MML_GetPsAttachAllowFlg();
    ulIsUsimStatusValid    = NAS_MML_IsUsimStausValid();
    enAvailableTimerStatus = NAS_MMC_GetTimerStatus(TI_NAS_MMC_AVAILABLE_TIMER);

    /* psǨ�Ƶ�modem0�����l��tds 15sδע��ɹ���Ӧ�ñ�����ʱ����ʱ�Ὣps��Ǩ��modem1��
       modem0 detach ps���������l��tdsû�б�������ϵͳ��Ϣ���ᵼ��available��ʱ��δ����psǨ�Ʋ���modem0
       mmc �յ�detach cnf�ж����svlte ������cs ps������ע�ᣬ����Ч����������״̬����available��ʱ��δ������
       ������available��ʱ�� */
    if ((VOS_TRUE == ucIsSvlteSupportFlag)
     && (VOS_FALSE == ucCsAttachAllowFlg)
     && (VOS_FALSE == ucPsAttachAllowFlg)
     && (VOS_TRUE == ulIsUsimStatusValid)
     && (NAS_MMC_FSM_PLMN_SELECTION != enFsmId)
     && (NAS_MMC_FSM_ANYCELL_SEARCH != enFsmId)
     && (NAS_MMC_TIMER_STATUS_RUNING != enAvailableTimerStatus))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_UINT32  NAS_MMC_RcvGmmDetachCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMCGMM_DETACH_CNF_STRU             *pstGmmDetachCnf;
    NAS_MMC_SERVICE_ENUM_UINT8          enPsServiceStatus;

    VOS_UINT8                           ucSimPsRegStatus;

#if (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                          ulDisableLteRoamFlg;

    ulDisableLteRoamFlg = NAS_MML_GetDisableLteRoamFlg();
#endif

    pstGmmDetachCnf = (MMCGMM_DETACH_CNF_STRU*)pstMsg;

    /* ����PS����״̬ */
    enPsServiceStatus = NAS_MMC_ConverMmStatusToMmc(NAS_MMC_REG_DOMAIN_PS,
             (NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8)pstGmmDetachCnf->ulServiceSts);

    NAS_MMC_SetPsServiceStatus(enPsServiceStatus);


    /* ��MMA����DETACH��� */
    NAS_MMC_SndMmaDetachCnf(MMA_MMC_SRVDOMAIN_PS,
                            (NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8)pstGmmDetachCnf->ulServiceSts,
                            pstGmmDetachCnf->ulOpid);


    /* ���PS������ע�����PS����Ч���ϱ�psע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH*/
    ucSimPsRegStatus = NAS_MML_GetSimPsRegStatus();
    if ((VOS_FALSE == NAS_MML_GetPsAttachAllowFlg())
     && (VOS_TRUE == ucSimPsRegStatus))
    {
        NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }

#if (FEATURE_ON == FEATURE_LTE)
    if (VOS_FALSE == NAS_MML_GetPsAttachAllowFlg())
    {
        /* GU��MO detach ��Ҫ֪ͨLMM */
        NAS_MMC_SndLmmMoDetachIndActionResultReq((MMC_LMM_MO_DETACH_TYPE_ENUM_UINT32)(pstGmmDetachCnf->ulDetachType));
    }

    /* �ж��Ƿ���Ҫdisable LTE */
    if ( VOS_TRUE == NAS_MMC_IsNeedDisableLte_DetachPs())
    {
        /* ������ε��µ�disable�Ѿ�֪ͨ��GU�����,��ʱ����Ҫ���ظ�֪ͨ */
        if (VOS_FALSE == ulDisableLteRoamFlg)
        {
            /* ��WAS/GAS����disable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_DISABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_DISABLE);

            /* ��LMM����disable LTE��Ϣ */
            NAS_MMC_SndLmmDisableLteNotify(MMC_LMM_DISABLE_LTE_REASON_PS_DETACH);
        }

        /* ����disable LTE������� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS);

        NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_PS_DETACH);
    }

    /* �յ�GMM��Detach Cnf������Ѿ�Detach CS,��ҪEnable Lte */
    if ( VOS_TRUE == NAS_MMC_IsNeedEnableLte_DetachPs())
    {
        /* ������ڽ�ֹLTE�������ε��µ�disable LTE����δ�ص�����֮ǰ��ʱҲ��֪ͨenable LTE */
        if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == NAS_MML_GetLteCapabilityStatus())
         && (VOS_FALSE == ulDisableLteRoamFlg))
        {
            /* ��WAS/GAS����enable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);

            /* ��LMM����enable LTE��Ϣ */
            NAS_MMC_SndLmmEnableLteNotify();
        }

        if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER))
        {
            NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER);
        }

        /* ����disable LTE������� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }
#endif

    if (VOS_TRUE == NAS_MMC_IsNeedStartAvailableTimer_PsDetachCnf())
    {
        NAS_MMC_StartTimer(TI_NAS_MMC_AVAILABLE_TIMER, NAS_MMC_GetNextAvailableTimerValue());
    }

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvGmmNetworkCapabilityInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    GMMMMC_NETWORK_CAPABILITY_INFO_IND_STRU    *pstRcvMsg = VOS_NULL_PTR;

    pstRcvMsg = (GMMMMC_NETWORK_CAPABILITY_INFO_IND_STRU *)pstMsg;

    NAS_MMC_SndMmaNetworkCapabilityInfoInd((MMA_MMC_NW_IMS_VOICE_CAP_ENUM_UINT8)pstRcvMsg->enNwImsVoCap,
                                           (MMA_MMC_NW_EMC_BS_CAP_ENUM_UINT8)pstRcvMsg->enNwEmcBsCap,
                                           (MMA_MMC_LTE_CS_CAPBILITY_ENUM_UINT8)pstRcvMsg->enLteCsCap);
    
    return VOS_TRUE;
}



#if   (FEATURE_ON == FEATURE_LTE)


VOS_UINT32  NAS_MMC_RcvGmmTinInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_TIN_TYPE_ENUM_UINT8         enGmmTinType;
    MMCGMM_TIN_TYPE_IND_STRU           *pstTinTypeMsg;
    NAS_MML_RPLMN_CFG_INFO_STRU        *pstRplmnCfgInfo = VOS_NULL_PTR;

    pstTinTypeMsg   = (MMCGMM_TIN_TYPE_IND_STRU*)pstMsg;

    enGmmTinType    = pstTinTypeMsg->enTinType;


    pstRplmnCfgInfo = NAS_MML_GetRplmnCfg();

    if ( enGmmTinType != pstRplmnCfgInfo->enTinType)
    {
        NAS_MMC_WriteTinInfoNvim(enGmmTinType, pstRplmnCfgInfo->aucLastImsi);
    }

    return VOS_TRUE;
}




VOS_UINT32  NAS_MMC_RcvLmmTinInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_TIN_TYPE_IND_STRU          *pstTinTypeMsg;
    NAS_MML_TIN_TYPE_ENUM_UINT8         enTinType;
    NAS_MML_RPLMN_CFG_INFO_STRU        *pstRplmnCfgInfo = VOS_NULL_PTR;

    pstTinTypeMsg  = (LMM_MMC_TIN_TYPE_IND_STRU*)pstMsg;

    /*��Ϣ��TINΪGUTIʱ*/
    if ( MMC_LMM_TIN_GUTI == pstTinTypeMsg->ulTinType )
    {
        /*����ȫ�ֱ�����TIN����ΪGUTI*/
        enTinType = NAS_MML_TIN_TYPE_GUTI;
    }
    else if ( MMC_LMM_TIN_RAT_RELATED_TMSI == pstTinTypeMsg->ulTinType )
    {
        /*��Ϣ��TINΪRAT-RELATED-TMSIʱ*/
        /*����ȫ�ֱ�����TIN����ΪRAT-RELATED-TMSI*/
        enTinType = NAS_MML_TIN_TYPE_RAT_RELATED_TMSI;
    }
    else if ( MMC_LMM_TIN_P_TMSI == pstTinTypeMsg->ulTinType )
    {
        /*��Ϣ��TINΪPTMSIʱ*/
        /*����ȫ�ֱ�����TIN����ΪPTMSI*/
        enTinType = NAS_MML_TIN_TYPE_PTMSI;
    }
    else
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "Invalid Tin Type");
        return VOS_TRUE;
    }

    pstRplmnCfgInfo     = NAS_MML_GetRplmnCfg();
    if ( enTinType != pstRplmnCfgInfo->enTinType )
    {
        NAS_MMC_WriteTinInfoNvim(enTinType, pstRplmnCfgInfo->aucLastImsi);
    }

    return VOS_TRUE;
}

VOS_UINT32  NAS_MMC_RcvLmmDetachCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_DETACH_CNF_STRU            *pstLmmDetachCnf     = VOS_NULL_PTR;
    VOS_UINT32                          ulRet;

    VOS_UINT8                           ucSimCsRegStatus;

    ulRet           = VOS_TRUE;

    pstLmmDetachCnf = (LMM_MMC_DETACH_CNF_STRU*)pstMsg;

    /* ��װ��Ϣ����MMA����DETACH��� */
    switch (pstLmmDetachCnf->ulReqType)
    {
        case MMC_LMM_MO_DET_PS_ONLY:

            ulRet = NAS_MMC_ProcEpsOnlyDetachCnf_PreProc(pstLmmDetachCnf);

            break;

        case MMC_LMM_MO_DET_CS_ONLY:

            ulRet = NAS_MMC_ProcCsOnlyDetachCnf_PreProc(pstLmmDetachCnf);

            break;

        case MMC_LMM_MO_DET_CS_PS:

            ulRet = NAS_MMC_ProcCsPsDetachCnf_PreProc(pstLmmDetachCnf);

            break;

        default:

            /* �쳣��ӡ */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmDetachCnf_PreProc:Unexpected detach request type!");
            break;
    }


    if (VOS_TRUE == NAS_MMC_IsNeedSndEplmn_LmmDetachCnf(pstLmmDetachCnf))
    {
        NAS_MMC_ProcEmcPdpRelease_DetachPs();
    }


    /* ���CS������ע�����CS����Ч���ϱ�csע��״̬ΪNAS_MML_REG_NOT_REGISTERED_NOT_SEARCH*/
    ucSimCsRegStatus = NAS_MML_GetSimCsRegStatus();
    if ((VOS_FALSE == NAS_MML_GetCsAttachAllowFlg())
     && (VOS_TRUE == ucSimCsRegStatus))
    {
        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
    }


    if (VOS_TRUE == NAS_MMC_IsNeedStartAvailableTimer_PsDetachCnf())
    {
        NAS_MMC_StartTimer(TI_NAS_MMC_AVAILABLE_TIMER, NAS_MMC_GetNextAvailableTimerValue());
    }

    return ulRet;
}
VOS_UINT32  NAS_MMC_RcvLmmAttachCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    LMM_MMC_ATTACH_CNF_STRU             *pstLmmAttachCnf;
    NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8 enCsMmaServiceStatus;
    NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8 enPsMmaServiceStatus;
    NAS_MMC_FSM_ID_ENUM_UINT32           enFsmId;
    VOS_UINT32                           ulDisableLteRoamFlg;

    VOS_UINT32                           ulNeedEnableLte;
    VOS_UINT32                           ulNeedDisableLte;


    pstLmmAttachCnf = (LMM_MMC_ATTACH_CNF_STRU*)pstMsg;

    /* ��ȡCS/PS����״̬ */
    enCsMmaServiceStatus = NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_CS);
    enPsMmaServiceStatus = NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_PS);

    /* ��װ��Ϣ����MMA����ATTACH��� */

    ulNeedEnableLte  = VOS_FALSE;
    ulNeedDisableLte = VOS_FALSE;


    switch (pstLmmAttachCnf->ulReqType)
    {
        case MMC_LMM_ATT_REQ_TYPE_PS_ONLY:
            NAS_MMC_SndMmaAttachCnf(MMA_MMC_SRVDOMAIN_PS,
                                    enPsMmaServiceStatus,
                                    pstLmmAttachCnf->ulOpId);

            ulNeedEnableLte = NAS_MMC_IsNeedEnableLte_AttachPs();
            break;

        case MMC_LMM_ATT_REQ_TYPE_CS_PS:
            NAS_MMC_SndMmaAttachCnf(MMA_MMC_SRVDOMAIN_CS,
                                    enCsMmaServiceStatus,
                                    pstLmmAttachCnf->ulOpId);

            NAS_MMC_SndMmaAttachCnf(MMA_MMC_SRVDOMAIN_PS,
                                    enPsMmaServiceStatus,
                                    pstLmmAttachCnf->ulOpId);

            ulNeedEnableLte = NAS_MMC_IsNeedEnableLte_AttachCsPs();
            ulNeedDisableLte = NAS_MMC_IsNeedDisableLte_AttachCsPs();

            break;

        case MMC_LMM_ATT_REQ_TYPE_CS_ONLY:
            NAS_MMC_SndMmaAttachCnf(MMA_MMC_SRVDOMAIN_CS,
                                    enCsMmaServiceStatus,
                                    pstLmmAttachCnf->ulOpId);

            ulNeedDisableLte = NAS_MMC_IsNeedDisableLte_AttachCs();
            break;

        default:
            /* �쳣��ӡ */
            break;
    }


    /* �ж��Ƿ���Ҫenable LTE */
    if (VOS_TRUE == ulNeedEnableLte)
    {
        ulDisableLteRoamFlg = NAS_MML_GetDisableLteRoamFlg();

        /* ������ڽ�ֹLTE�������ε��µ�disable LTE����δ�ص�����֮ǰ��ʱҲ��֪ͨenable LTE */
        if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == NAS_MML_GetLteCapabilityStatus())
         && (VOS_FALSE == ulDisableLteRoamFlg))
        {
            /* ��WAS/GAS����enable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);

            /* ��LMM����enable LTE��Ϣ */
            NAS_MMC_SndLmmEnableLteNotify();
        }

        if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER))
        {
            NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER);
        }

        /* ����disable LTE������� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
        return VOS_TRUE;
    }

    enFsmId = NAS_MMC_GetCurrFsmId();
    if (NAS_FSM_BUTT <= enFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    /* �ж��Ƿ���Ҫdisable LTE */
    if (VOS_TRUE == ulNeedDisableLte)
    {
        /* ��¼����disable LTE�ı��,����L��Ϊ��ģ��֪ͨLMM,WAS,GAS */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS);

        if (VOS_FALSE == NAS_MML_GetPsAttachAllowFlg())
        {
            NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_PS_DETACH);
        }
        else
        {
            NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_PS_USIM_INVALID);
        }

        /* ON PLMN��ֱ�ӿ��Դ��� */
        if ((NAS_MMC_L1_STA_ON_PLMN == NAS_MMC_GetFsmTopState())
        && (NAS_MMC_FSM_L1_MAIN == enFsmId))
        {
           /* �����ǰEPS�������Ӵ��ڣ��򻺴湹���������Ϣ */
           if ( VOS_TRUE == NAS_MML_IsRrcConnExist())
           {
               NAS_MMC_SetBufferedPlmnSearchInfo(VOS_TRUE, NAS_MMC_PLMN_SEARCH_SCENE_DISABLE_LTE);

               return VOS_TRUE;
           }
        }

        /* ����״̬�����д��� */
        return VOS_FALSE;
    }



    /* L->LWG�󣬻���DETACH CS/PS��MMA���յ�SYSCFG CNFʱ�����·�ATTACH CS/PS��ATTACHʧ��ʱ��Ҫ�������� */
    if ( VOS_TRUE == NAS_MMC_IsLmmAttachCnfTriggerPlmnSrch(pstLmmAttachCnf) )
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_VOID NAS_MMC_CovertLmmRegStatusToMmlRegStatus(
    MMC_LMM_REGISTER_STATUS_ENUM_UINT32 enLmmRegState,
    NAS_MML_REG_STATUS_ENUM_UINT8      *penMmlRegStatus
)
{
    /* ���ݵ�ǰ��ȡ��ע��״̬��ת��ΪMML���Ӧ��ע��״̬ */
    switch (enLmmRegState)
    {
        case   MMC_LMM_REGISTER_STATUS_NOT_REG_NOT_SRCHING:
            *penMmlRegStatus  =  NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH;
            break;

        case  MMC_LMM_REGISTER_STATUS_REG_HPLMN:
            *penMmlRegStatus  = NAS_MML_REG_REGISTERED_HOME_NETWORK;
            break;

        case MMC_LMM_REGISTER_STATUS_NOT_REG_SRCHING:
            *penMmlRegStatus  = NAS_MML_REG_NOT_REGISTERED_SEARCHING;
            break;

        case MMC_LMM_REGISTER_STATUS_REG_DENINED:
            *penMmlRegStatus  =  NAS_MML_REG_REGISTERED_DENIED;
            break;

        case MMC_LMM_REGISTER_STATUS_UNKNOWN:
            *penMmlRegStatus  =  NAS_MML_REG_UNKNOWN;
            break;

        case MMC_LMM_REGISTER_STATUS_REG_ROAMING:
            *penMmlRegStatus  = NAS_MML_REG_REGISTERED_ROAM;
            break;

        default:
            NAS_WARNING_LOG(WUEPS_PID_MMC ,"NAS_MMC_CovertLmmRegStatusToMmlRegStatus():enLmmRegState is err.");
            break;
    }

    return;
}

VOS_UINT32   NAS_MMC_RcvLmmRegStatusChangeInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU         *pstCurCampInfo    =  VOS_NULL_PTR ;
    LMM_MMC_REGISTER_STATUS_IND_STRU    *pstRegStateInfo     =  VOS_NULL_PTR;
    NAS_MML_REG_STATUS_ENUM_UINT8        enRegStatus;


    enRegStatus = NAS_MML_REG_STATUS_BUTT;

    pstRegStateInfo = (LMM_MMC_REGISTER_STATUS_IND_STRU*)pstMsg;

    /*���Я����ע��״̬*/
    if ( VOS_TRUE == pstRegStateInfo->bitOpRegisterStatus)
    {

        NAS_MMC_CovertLmmRegStatusToMmlRegStatus(pstRegStateInfo->ulRegisterStatus, &enRegStatus);

        NAS_MML_SetEpsRegStatus(enRegStatus);


        NAS_MML_SetPsRegStatus(enRegStatus);

        NAS_MMC_SndMmaRegStatusInd(MMA_MMC_SRVDOMAIN_PS, enRegStatus);
    }
    /* ���λ���������仯*/
    if ( VOS_TRUE == pstRegStateInfo->bitOpLocationInfo)
    {
        /* ��ȡ��ǰפ����������Ϣ */
        pstCurCampInfo                  =  NAS_MML_GetCurrCampPlmnInfo();

        /*���µ�ǰפ��λ������Ϣ*/
        pstCurCampInfo->stLai.aucLac[0] = pstRegStateInfo->stTac.ucTac;
        pstCurCampInfo->stLai.aucLac[1] = pstRegStateInfo->stTac.ucTacCnt;
        pstCurCampInfo->ucRac           = NAS_MML_RAC_INVALID;

        pstCurCampInfo->stCampCellInfo.astCellInfo[0].ulCellId  = pstRegStateInfo->ulCellId;
        /*lmm�ϱ���С������Ϊ1*/
        pstCurCampInfo->stCampCellInfo.ucCellNum            = 1;


        NAS_MMC_SndMmaSysInfo();

    }

    if ((VOS_FALSE == pstRegStateInfo->bitOpRegisterStatus)
     && (VOS_FALSE == pstRegStateInfo->bitOpLocationInfo))
    {
        /*�쳣��ӡ*/
        NAS_WARNING_LOG(PS_PID_MM ,"NAS_MMC_RcvLmmRegStatusChangeInd_PreProc():WARNING:pstRegStateInfo RegisterStatus and LocationInfo not take");

    }
    return VOS_TRUE;

}




VOS_VOID NAS_MMC_ConvertLmmServiceStatusToMmcServiceStatus(
    MMC_LMM_SERVICE_STATUS_ENUM_UINT32  *pulLmmServiceStatus,
    NAS_MMC_SERVICE_ENUM_UINT8          *pucMmcServiceStatus
)
{
    switch ( *pulLmmServiceStatus )
    {
        case MMC_LMM_SERVICE_STATUS_NO_SERVICE :
            *pucMmcServiceStatus = NAS_MMC_NO_SERVICE;
            break;

        case MMC_LMM_SERVICE_STATUS_LIMIT_SERVICE :
            *pucMmcServiceStatus = NAS_MMC_LIMITED_SERVICE;
            break;

        case  MMC_LMM_SERVICE_STATUS_NORMAL_SERVICE :
            *pucMmcServiceStatus = NAS_MMC_NORMAL_SERVICE;
            break;

        case  MMC_LMM_SERVICE_STATUS_LIMIT_SERVICE_REGION :
            *pucMmcServiceStatus = NAS_MMC_LIMITED_SERVICE_REGION;
            break;

        default :
            /*�쳣��ӡ*/
            NAS_WARNING_LOG(WUEPS_PID_MMC ,"NAS_MMC_ConvertLmmServiceStatusToMmcServiceStatus():WARNING: pulLmmServiceStatus is out of limit.");
            *pucMmcServiceStatus  = NAS_MMC_GetPsServiceStatus();
            break;
    }

    return;
}




VOS_VOID NAS_MMC_ConvertLmmServiceStatusToMmcReportSrvSt(
    MMC_LMM_SERVICE_STATUS_ENUM_UINT32  *pulLmmServiceStatus,
    NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8*penComSrvSt
)
{
    switch ( *pulLmmServiceStatus )
    {
        case MMC_LMM_SERVICE_STATUS_NO_SERVICE :
            *penComSrvSt = MMA_MMC_SERVICE_STATUS_NO_SERVICE;
            break;

        case MMC_LMM_SERVICE_STATUS_LIMIT_SERVICE :
            *penComSrvSt = MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE;
            break;

        case  MMC_LMM_SERVICE_STATUS_NORMAL_SERVICE :
            *penComSrvSt = MMA_MMC_SERVICE_STATUS_NORMAL_SERVICE;
            break;

        case  MMC_LMM_SERVICE_STATUS_LIMIT_SERVICE_REGION :
            *penComSrvSt = MMA_MMC_SERVICE_STATUS_LIMITED_SERVICE_REGION;
            break;

        default :
            /*�쳣��ӡ*/
            NAS_WARNING_LOG(WUEPS_PID_MMC ,"NAS_MMC_ConvertLmmServiceStatusToMmcReportSrvSt():WARNING: pulLmmServiceStatus is out of limit.");
            *penComSrvSt  = MMA_MMC_SERVICE_STATUS_NO_SERVICE;
            break;
    }

    return;
}

VOS_UINT32   NAS_MMC_RcvLmmServiceStatusInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_SERVICE_INFO_CTX_STRU       *pstServiceInfo       = VOS_NULL_PTR;
    LMM_MMC_SERVICE_STATUS_IND_STRU     *pstServiceStateInfo  =  VOS_NULL_PTR;
    NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8 enReportSrvSt;
    NAS_MML_REG_STATUS_ENUM_UINT8        enCsRegStatus;

    pstServiceStateInfo = (LMM_MMC_SERVICE_STATUS_IND_STRU*)pstMsg;

    /*��ȡ��ǰ����״̬*/
    pstServiceInfo     =  NAS_MMC_GetServiceInfo();

    if (VOS_TRUE == pstServiceStateInfo->bitOpPsStatus)
    {
        NAS_MMC_ConvertLmmServiceStatusToMmcServiceStatus(&pstServiceStateInfo->ulServiceStatus,
            &pstServiceInfo->enPsCurrService);



        enReportSrvSt = NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_PS);


        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_PS, enReportSrvSt);

    }

    if (VOS_TRUE == pstServiceStateInfo->bitOpCsStatus)
    {
        NAS_MMC_ConvertLmmServiceStatusToMmcServiceStatus(&pstServiceStateInfo->ulCsServiceStatus,
            &pstServiceInfo->enCsCurrService);

        /* �����ǰ����״̬��Ϊnormal service,����ע��״̬Ϊ��ע��,��ı�ע��
           ״̬Ϊδע�� */
        enCsRegStatus = NAS_MML_GetCsRegStatus();

        if ( (NAS_MMC_NORMAL_SERVICE              != pstServiceInfo->enCsCurrService)
         && ((NAS_MML_REG_REGISTERED_HOME_NETWORK == enCsRegStatus)
          || (NAS_MML_REG_REGISTERED_ROAM         == enCsRegStatus)) )
        {
            NAS_MML_SetCsRegStatus(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);

            NAS_MMC_SndMmaRegStatusInd(MMA_MMC_SRVDOMAIN_CS, NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);
        }
        
        /* csע��״̬��ע��ʧ�ܵ�normal serviceʱ���ϱ�csע��״̬ */
        if ( (NAS_MMC_NORMAL_SERVICE              == pstServiceInfo->enCsCurrService)
          && (NAS_MML_REG_REGISTERED_HOME_NETWORK != enCsRegStatus)
          && (NAS_MML_REG_REGISTERED_ROAM         != enCsRegStatus) )
        {
            NAS_MMC_UpdateCsRegStateCsRegSucc();
        }

        



        enReportSrvSt = NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_CS);


        NAS_MMC_SndMmaServiceStatusInd(MMA_MMC_SRVDOMAIN_CS, enReportSrvSt);


    }

    return VOS_TRUE;

}




VOS_UINT32  NAS_MMC_RcvLmmErrInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ����OAM�Ľӿ�ֱ�ӽ��и�λ  */

    NAS_MML_SoftReBoot();

    return VOS_TRUE;
}

VOS_UINT32  NAS_MMC_RcvLmmSuspendCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_SUSPEND_CNF_STRU           *pstLmmSuspendCnf = VOS_NULL_PTR;
    VOS_UINT8                           ucEmcFlg;
    VOS_UINT8                           ucImsVoiceAvail;

    /* Lģʱ�������¸�ʽת�� */
    pstLmmSuspendCnf = (LMM_MMC_SUSPEND_CNF_STRU*)pstMsg;

    /* ��������Ϊ�ɹ�������״̬������ */
    if ( MMC_LMM_SUCC != pstLmmSuspendCnf->ulRst)
    {
        return VOS_FALSE;
    }

    /* �ж��Ƿ���Ҫdisable LTE */
    if (NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS == NAS_MML_GetLteCapabilityStatus())
    {
        /* ������ε��µ�disable�Ѿ�֪ͨ��GU�����,��ʱ����Ҫ���ظ�֪ͨ */
        if (VOS_FALSE == NAS_MML_GetDisableLteRoamFlg())
        {
            /* ��WAS/GAS����disable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_DISABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_DISABLE);

            /* ��LMM����disable LTE��Ϣ */
            NAS_MMC_SndLmmDisableLteNotify(NAS_MML_GetDisableLteReason());
        }

        /* ����disable LTE������� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS);

        NAS_MML_SetDisableLteReason(NAS_MML_GetDisableLteReason());
    }
    else
    {
        /* �ڽ������в���ͨ��CSFB��GUʱ,����rel-11 24.301 4.3.1��Ҫdisable LTE.Ŀǰ������һ��disableʱ��Ϊ5MINS.
           5mins������enable LTE */
        /* 3GPP 24.301 Selection 4.3.1:
           if the UE needs to initiate a CS fallback emergency call but it is unable to perform CS fallback, 
           the UE shall attempt to select GERAN or UTRAN radio access technology, 
           and a UE with "IMS voice not available" should disable the E-UTRA capability (see subclause 4.5) to allow a potential callback, 
           and then progress the CS emergency call establishment;

           NOTE 2: Unable to perform CS fallback or 1xCS fallback means that either the UE was not allowed to attempt CS fallback or 1xCS fallback, 
           or CS fallback or 1xCS fallback attempt failed.
        */
        /* Disable LTEʱ��Ҫ�ж�IMS�Ƿ���ã�IMS����ʱ��Disable LTE */ 
        ucEmcFlg        = NAS_MML_GetCsEmergencyServiceFlg();
        ucImsVoiceAvail = NAS_MML_GetImsVoiceAvailFlg();
        if (((NAS_MML_CSFB_SERVICE_STATUS_MO_EMERGENCY_EXIST == NAS_MML_GetCsfbServiceStatus())
          || (VOS_TRUE == ucEmcFlg))
         && (VOS_FALSE  == ucImsVoiceAvail))
         
        {
            /* ����disable LTE������� */
            NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS);

            /* ��WAS/GAS����disable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_DISABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_DISABLE);

            /* ��LMM����disable LTE��Ϣ */
            NAS_MMC_SndLmmDisableLteNotify(MMC_LMM_DISABLE_LTE_REASON_EMERGENCY_CALL);
            NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_EMERGENCY_CALL);

            NAS_MMC_StartTimer(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER, NAS_MML_GetCsfbEmgCallEnableLteTimerLen());
        }
    }

    return VOS_FALSE;
}

VOS_UINT32  NAS_MMC_RcvLmmMmcDetachInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_DETACH_IND_STRU                                *pstDetachMsg = VOS_NULL_PTR;
    MMA_MMC_NETWORK_DETACH_TYPE_ENUM_UINT8                  enDetachType;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCause;

    pstDetachMsg        = (LMM_MMC_DETACH_IND_STRU*)pstMsg;

    enDetachType        = MMA_MMC_NETWORK_DETACH_TYPE_BUTT;
    enCause             = NAS_MML_REG_FAIL_CAUSE_BUTT;

    if ( VOS_FALSE == pstDetachMsg->bitOpCnCause )
    {
        enCause = NAS_MML_REG_FAIL_CAUSE_OTHER_CAUSE;
    }
    else
    {
        NAS_MMC_ConvertLmmCauseToMmCause((VOS_UINT8)pstDetachMsg->ucCnCause,
                                                &enCause);
    }
    /* UE����DETACH���磺ȥ�������г��أ�ͨ��DETACH������MMC_LMM_DETACH_LOCAL
    ֪ͨMMC����Я������ֵ */
    if (MMC_LMM_DETACH_LOCAL == pstDetachMsg->ulDetachType)
    {
        if (NAS_MMC_NO_SERVICE != NAS_MMC_GetPsServiceStatus())
        {
            /* ��¼����״̬: ���޷��� */
            NAS_MMC_SetPsServiceStatus(NAS_MMC_LIMITED_SERVICE);
        }

        /* ����PDN���Ӵ���ʱ:������Ӵ��ڱ�־����LMM/GUAS����EPLMN֪ͨ��ɾ��Forb Plmn */
        NAS_MMC_ProcEmcPdpRelease_DetachPs();

        /* ��GMM��MMת��ע������Ϣ */
        NAS_MMC_SndMmLmmLocalDetachInd(pstDetachMsg);
        NAS_MMC_SndGmmLmmLocalDetachInd(pstDetachMsg);

        NAS_MMC_ChangeCsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);

        return VOS_TRUE;
    }

    if ( VOS_TRUE == pstDetachMsg->bitOpCnReqType )
    {
        NAS_MMC_ConvertLmmDetachIndTypeToMmcType(pstDetachMsg->ulCnReqType, &enDetachType);

        NAS_MMC_SndMmaDetachInd(MMA_MMC_SRVDOMAIN_PS, (VOS_UINT32)enDetachType, (VOS_UINT32)enCause);
    }

    return VOS_FALSE;
}


VOS_UINT32  NAS_MMC_RcvLmmMmcStatusInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_STATUS_IND_STRU            *pstLmmStatusIndMsg = VOS_NULL_PTR;

    pstLmmStatusIndMsg = (LMM_MMC_STATUS_IND_STRU*)pstMsg;

    if ( VOS_FALSE == pstLmmStatusIndMsg->bitOpConnState)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmMmcStatusInd_PreProc:bitOpConnState IS VOS_FALSE");
        return VOS_TRUE;
    }

    NAS_MML_UpdateEpsConnectionStatus(pstLmmStatusIndMsg->ulConnState);


    NAS_MMC_SndGmmLmmStatusInd(pstLmmStatusIndMsg);


    if ( MMC_LMM_CONNECTED_DATA == pstLmmStatusIndMsg->ulConnState )
    {
        NAS_MMC_SndMmaDataTranAttri(MMA_MMC_DATATRAN_ATTRI_LTE_DATA);
    }
    else
    {
        NAS_MMC_SndMmaDataTranAttri(MMA_MMC_DATATRAN_ATTRI_LTE_IDLE);
    }

    /* ����δ������� */
    return VOS_FALSE;
}



VOS_UINT32  NAS_MMC_RcvLmmTimerStateNotify_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_TIMER_STATE_NOTIFY_STRU    *pstTimerInfoNotify;

    pstTimerInfoNotify = (LMM_MMC_TIMER_STATE_NOTIFY_STRU *)pstMsg;

    if ((VOS_TRUE == pstTimerInfoNotify->bitOpT3412)
     && (LMM_MMC_TIMER_BUTT != pstTimerInfoNotify->enT3412State))
    {
        NAS_MML_SetT3412Status((VOS_UINT8)(pstTimerInfoNotify->enT3412State));
    }

    if ((VOS_TRUE == pstTimerInfoNotify->bitOpT3423)
     && (LMM_MMC_TIMER_BUTT != pstTimerInfoNotify->enT3423State))
    {
        NAS_MML_SetT3423Status((VOS_UINT8)(pstTimerInfoNotify->enT3423State));
    }

    /* ��MM��GMMת����ʱ��״̬��Ϣ */
    NAS_MMC_SndGmmLmmTimerStateNotify(pstTimerInfoNotify);
    NAS_MMC_SndMmLmmTimerStateNotify(pstTimerInfoNotify);

    return VOS_TRUE;
}
VOS_UINT32  NAS_MMC_RcvLmmMmcNotCampOnInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMC_SetAsCellCampOn(NAS_MMC_AS_CELL_NOT_CAMP_ON);
    return VOS_TRUE;
}

#endif


VOS_UINT32  NAS_MMC_RcvOmOtaReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    OM_NAS_OTA_REQ_STRUCT              *pstOtaReq;
    VOS_UINT32                          ulErrCode;

    pstOtaReq   = ( OM_NAS_OTA_REQ_STRUCT* )pstMsg;

    /*����NAS OTA��Ϣ�Ŀ���*/
    NAS_INFO_LOG2(WUEPS_PID_MMC, "NAS_MMC_RcvOmOtaReq_PreProc: NORMAL: Specified NAS OTA switch is , Old switch is ",
                 (long)pstOtaReq->ulOnOff, NAS_GetNasOtaSwitch());

    /* �жϿտ���Ϣ�����Ƿ���Ч */
    if ( (NAS_OTA_SWITCH_ON != pstOtaReq->ulOnOff )
      && (NAS_OTA_SWITCH_OFF != pstOtaReq->ulOnOff))
    {
        /* �տ���Ϣ���ô��� */
        ulErrCode = NAS_OTA_CNF_ERRCODE_YES;
        NAS_MMC_SndOmOtaCnf(ulErrCode);
        return VOS_TRUE;

    }

    NAS_SetNasOtaSwitch(pstOtaReq->ulOnOff);
    ulErrCode = NAS_OTA_CNF_ERRCODE_NO;

    NAS_INFO_LOG2(WUEPS_PID_MMC, "NAS_MMC_RcvOmMsg_PreProc: NORMAL: Specified NAS OTA switch is , Old switch is ",
                 (long)pstOtaReq->ulOnOff, NAS_GetNasOtaSwitch());

    NAS_MMC_SndOmOtaCnf(ulErrCode);
    return VOS_TRUE;
}


/*****************************************************************************
 �� �� ��  : NAS_MMC_RcvOmInquireReq_PreProc
 ��������  : ��������OM�Ĳ�ѯ��Ϣ
 �������  : ulEventType - ��Ϣ����
             pstMsg      - ��Ϣ���׵�ַ
 �������  : ��
 �� �� ֵ  : VOS_TRUE:����Ԥ�������
             VOS_FALSE:����Ԥ����δ���
 ���ú���  :
 ��������  :

 �޸���ʷ      :
 1.��    ��   : 2011��7��23��
   ��    ��   :
   �޸�����   : �����ɺ���
*****************************************************************************/
VOS_UINT32 NAS_MMC_RcvOmInquireReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    ID_NAS_OM_INQUIRE_STRU             *pstOmMsg = VOS_NULL_PTR;

    pstOmMsg = (ID_NAS_OM_INQUIRE_STRU *)pstMsg;

    NAS_INFO_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvOmInquireReq_PreProc:  ");

    NAS_MMC_SndOmInquireCnfMsg(pstOmMsg);

    return VOS_TRUE;

}




VOS_VOID NAS_MMC_SndOmConfigTimerReportCnfMsg(VOS_UINT16 usToolsId)
{
    ID_NAS_OM_CNF_STRU                 *pstMmOmCnf = VOS_NULL_PTR;

    pstMmOmCnf = (ID_NAS_OM_CNF_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC, sizeof(ID_NAS_OM_CNF_STRU) );

    if (VOS_NULL_PTR == pstMmOmCnf)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_SndOmConfigTimerReportCnfMsg:Memory Allocate fail!");
        return;
    }

    PS_MEM_SET((VOS_INT8*)pstMmOmCnf, 0 ,sizeof(ID_NAS_OM_CNF_STRU));


    pstMmOmCnf->ucFuncType         = 4;
    pstMmOmCnf->ucReserve          = 0;
    pstMmOmCnf->usLength           = NAS_OM_DATA_OFFSET;
    pstMmOmCnf->ulSn               = 0;
    pstMmOmCnf->ulTimeStamp        = OM_GetSlice();
    pstMmOmCnf->usPrimId           = ID_NAS_OM_CONFIG_TIMER_REPORT_CNF;
    pstMmOmCnf->usToolsId          = usToolsId;

    OM_SendData((OM_RSP_PACKET_STRU*)pstMmOmCnf, sizeof(ID_NAS_OM_CNF_STRU));

    PS_MEM_FREE(WUEPS_PID_MMC, pstMmOmCnf);

}

VOS_UINT32 NAS_MMC_RcvOmConfigTimerReportReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    ID_NAS_OM_INQUIRE_STRU             *pstOmMsg             = VOS_NULL_PTR;
    OM_MMC_TIMER_REPORT_CFG_REQ_STRU   *pstTimerReportCfgReq = VOS_NULL_PTR;
    NAS_TIMER_EVENT_INFO_STRU          *pstTimerEventCfg     = VOS_NULL_PTR;
    VOS_UINT32                          ulTimerEventNum;

    pstOmMsg             = (ID_NAS_OM_INQUIRE_STRU *)pstMsg;
    pstTimerReportCfgReq = (OM_MMC_TIMER_REPORT_CFG_REQ_STRU *)pstOmMsg->aucData;


    pstTimerEventCfg    = NAS_GetTimerEventReportCfg();

    if ( NAS_MMC_REPORT_TIMER_OTA_EVENT_START == pstTimerReportCfgReq->ulCommand )
    {
        ulTimerEventNum = pstTimerReportCfgReq->stTimerMsg.ulItems;

        if(pstTimerReportCfgReq->stTimerMsg.ulItems > NAS_MAX_TIMER_EVENT)
        {
            ulTimerEventNum = NAS_MAX_TIMER_EVENT;
        }
        else
        {
            pstTimerEventCfg->ulItems = ulTimerEventNum;
        }

        PS_MEM_CPY(pstTimerEventCfg->aulTimerMsg, pstTimerReportCfgReq->stTimerMsg.aulTimerMsg, ulTimerEventNum*sizeof(VOS_UINT32));
    }
    else
    {
        pstTimerEventCfg->ulItems = 0;
    }

    NAS_MMC_SndOmConfigTimerReportCnfMsg(pstOmMsg->usToolsId);

    return VOS_TRUE;

}


VOS_VOID NAS_MMC_IsNeedUpdateHighPrioPlmnSearchList(
    MMA_MMC_UE_SUPPORT_FREQ_BAND_STRU   *pstSysCfgSetBand
)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList = VOS_NULL_PTR;
    NAS_MML_MS_BAND_INFO_STRU                              *pstMsBandInfo        = VOS_NULL_PTR;
    VOS_UINT32                                              i;

    pstMsBandInfo        = NAS_MML_GetMsSupportBand();
    pstPlmnSelectionList = NAS_MMC_GetHighPrioPlmnList();

    /* ���G��Ƶ�η����ı䣬��Ҫ���¸����ȼ������б������н��뼼��ΪG������״̬Ϊδ���� */
    if (pstMsBandInfo->unGsmBand.ulBand != pstSysCfgSetBand->unGsmBand.ulBand)
    {
        for (i = 0; i < pstPlmnSelectionList->usSearchPlmnNum; i++)
        {
            NAS_MMC_UpdateRatNetStatusInPlmnRatInfo(NAS_MML_NET_RAT_TYPE_GSM,
                                                    NAS_MMC_NET_STATUS_NO_SEARCHED,
                                                    &pstPlmnSelectionList->astPlmnSelectionList[i]);
        }
    }

    /* ���W��Ƶ�η����ı䣬��Ҫ���¸����ȼ������б������н��뼼��ΪW������״̬Ϊδ���� */
    if (pstMsBandInfo->unWcdmaBand.ulBand != pstSysCfgSetBand->unWcdmaBand.ulBand)
    {
        for (i = 0; i < pstPlmnSelectionList->usSearchPlmnNum; i++)
        {
            NAS_MMC_UpdateRatNetStatusInPlmnRatInfo(NAS_MML_NET_RAT_TYPE_WCDMA,
                                                   NAS_MMC_NET_STATUS_NO_SEARCHED,
                                                   &pstPlmnSelectionList->astPlmnSelectionList[i]);
        }
    }

#if (FEATURE_ON == FEATURE_LTE)

/* ���L��Ƶ�η����ı䣬��Ҫ���¸����ȼ������б������н��뼼��ΪL������״̬Ϊδ���� */
    if ((pstMsBandInfo->stLteBand.aulLteBand[0] != pstSysCfgSetBand->stLteBand.aulLteBand[0])
     || (pstMsBandInfo->stLteBand.aulLteBand[1] != pstSysCfgSetBand->stLteBand.aulLteBand[1]))
    {
        for (i = 0; i < pstPlmnSelectionList->usSearchPlmnNum; i++)
        {
            NAS_MMC_UpdateRatNetStatusInPlmnRatInfo(NAS_MML_NET_RAT_TYPE_LTE,
                                                   NAS_MMC_NET_STATUS_NO_SEARCHED,
                                                   &pstPlmnSelectionList->astPlmnSelectionList[i]);
        }
    }
#endif

    return;
}



VOS_UINT32  NAS_MMC_RcvTafSysCfgReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMA_MMC_SYS_CFG_SET_REQ_STRU        *pstSysCfgSetParm = VOS_NULL_PTR;
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulSyscfgNeedPlmnSrchRslt;
    VOS_UINT32                          ulCsServiceExist;

    NAS_MMC_FSM_ID_ENUM_UINT32          enCurrFsmId;

    pstSysCfgSetParm         = (MMA_MMC_SYS_CFG_SET_REQ_STRU*)pstMsg;

    /* �жϵ�ǰSYSCFG�����Ƿ���Ҫ�������� */
    ulSyscfgNeedPlmnSrchRslt = NAS_MMC_IsPlmnSelectionNeeded_SysCfg(pstSysCfgSetParm);

    /* �жϵ�ǰ�Ƿ���CSҵ����� */
    ulCsServiceExist         = NAS_MML_GetCsServiceExistFlg();

    /* ���SYSCFG������Ҫ�����ҵ�ǰ��ҵ����ڻ����л����ҵ�����
       ��ô������SYSCFG���ã���MMA����SYSCFG ����ʧ��*/


    /* GU�´���PSҵ��ʱ��MMC��MM/GMM�����ͷ����������ͷź󣬿��·�SYSCFG����L�Ĵ�����ͬ
       ����CSҵ��ʱ��MMC��MMA�ϱ�SYSCFG���Ϊ��CSҵ����� */
    if (VOS_TRUE == ulSyscfgNeedPlmnSrchRslt)
    {
        if (VOS_TRUE == ulCsServiceExist)
        {
            NAS_MMC_SndMmaSysCfgCnf(MMA_MMC_SYS_CFG_SET_FAILURE_CS_SERV_EXIST);
            return VOS_TRUE;
        }
        else
        {

        }
    }

    enCurrFsmId = NAS_MMC_GetCurrFsmId();
    if (NAS_FSM_BUTT <= enCurrFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    /* ���ڻ���Ľ��������ҵ�ǰ�������ڽ������д�����anycell״̬����syscfg������������ҲҪ�ȴ���������У�syscfg����ʧ��  */
    if ((VOS_TRUE == NAS_MML_IsExistBufferedEmgCall())
     && (NAS_MMC_FSM_ANYCELL_SEARCH == enCurrFsmId))
    {
        NAS_MMC_SndMmaSysCfgCnf(MMA_MMC_SYS_CFG_SET_FAILURE_OTHER);
        return VOS_TRUE;
    }

    /* ���Ƶ�η����ı䣬��Ҫ���¸����ȼ������б�������״̬ */
    NAS_MMC_IsNeedUpdateHighPrioPlmnSearchList((MMA_MMC_UE_SUPPORT_FREQ_BAND_STRU *)&pstSysCfgSetParm->stBand);

    /* �жϵ�ǰ�Ƿ���Ҫ��ϵ�ǰ״̬�� �������Ϣ�Ļ��� */
    ulRet = NAS_MMC_ProcMsgPrio(ulEventType, pstMsg);

    /* ��Ҫ��ϵ�ǰ״̬���������Ϣ�Ļ���,���ʾԤ������� */
    if ( VOS_TRUE == ulRet )
    {
        return VOS_TRUE;
    }

    /* ��Ҫ�������״̬������ */
    return VOS_FALSE;
}
VOS_UINT32  NAS_MMC_RcvTafAcqReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ǰ״̬����ʶ */
      NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;

      MMA_MMC_PLMN_ID_STRU                stPlmnId;

      enFsmId = NAS_MMC_GetCurrFsmId();

      /* ����ϵͳ��ON Plmn״̬�������������ȼ�������ʱ�� */
    if ( (NAS_MMC_FSM_INTER_SYS_CELLRESEL == enFsmId)
      || (NAS_MMC_FSM_INTER_SYS_OOS       == enFsmId)
      || (NAS_MMC_FSM_INTER_SYS_HO        == enFsmId)
      || (NAS_MMC_FSM_INTER_SYS_CCO       == enFsmId))
    {

       stPlmnId.ulMcc     = NAS_MML_INVALID_MCC;
       stPlmnId.ulMnc     = NAS_MML_INVALID_MNC;
       NAS_MMC_SndMmaAcqCnf(MMC_MMA_ACQ_RESULT_FAILURE, &stPlmnId, 0);

       return VOS_TRUE;
    }

    /* ��Ҫ�������״̬������ */
    return VOS_FALSE;
}
VOS_UINT32  NAS_MMC_RcvTafRegReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* ��ǰ״̬����ʶ */
      NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;

      MMA_MMC_PLMN_ID_STRU                stPlmnId;

      enFsmId = NAS_MMC_GetCurrFsmId();

      /* ����ϵͳ��ON Plmn״̬�������������ȼ�������ʱ�� */
    if ( (NAS_MMC_FSM_INTER_SYS_CELLRESEL == enFsmId)
      || (NAS_MMC_FSM_INTER_SYS_OOS       == enFsmId)
      || (NAS_MMC_FSM_INTER_SYS_HO        == enFsmId)
      || (NAS_MMC_FSM_INTER_SYS_CCO       == enFsmId))
    {

       stPlmnId.ulMcc     = NAS_MML_INVALID_MCC;
       stPlmnId.ulMnc     = NAS_MML_INVALID_MNC;
       NAS_MMC_SndMmaRegCnf(MMC_MMA_REG_RESULT_ACQ_FAILURE, &stPlmnId, 0x0);

       return VOS_TRUE;
    }

    /* ��Ҫ�������״̬������ */
    return VOS_FALSE;
}





VOS_UINT32  NAS_MMC_RcvTafUserSpecPlmnSearch_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMA_MMC_PLMN_SPECIAL_REQ_STRU      *pstUserSelReqMsg  = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU               *pstUserSpecPlmnId = VOS_NULL_PTR;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurrRatType;
    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;
    VOS_UINT32                          ulCsServiceStatusFlg;
    VOS_UINT32                          ulRslt;

    NAS_MML_PLMN_WITH_RAT_STRU          stUserSpecPlmnInfo;

    VOS_UINT32                          ulSpecPlmnSearchFlg;

    pstUserSelReqMsg  = (MMA_MMC_PLMN_SPECIAL_REQ_STRU*)pstMsg;

    /* ָ���û�ָ��������Ϣ�е�PlmnId */
    pstUserSpecPlmnId = (NAS_MML_PLMN_ID_STRU*)(&pstUserSelReqMsg->stPlmnId);

    /* ������� */
    if ( VOS_FALSE == NAS_MMC_CheckUserSpecPlmnSearchParamMsg_PreProc(pstMsg) )
    {
        /* ��MMA�����û�ָ�������ܾ�������������ģʽ */
        NAS_MMC_SndMmaUserSpecPlmnSearchRej();
        return VOS_TRUE;
    }

    /* �ػ�״̬���svlte nv����������cops����дnv������û�ָ��plmn��Ϣ,��mma�ظ��ɹ� */
    enFsmId = NAS_MMC_GetCurrFsmId();
    if ((NAS_MMC_L1_STA_NULL == NAS_MMC_GetFsmTopState())
     && (NAS_MMC_FSM_L1_MAIN == enFsmId))
    {
        NAS_MMC_SetPlmnSelectionMode(NAS_MMC_PLMN_SELECTION_MODE_MANUAL);

        /* ����ǰ��ѡģʽд��NVIM�� */
        NAS_MMC_WritePlmnSelectionModeNvim();

        stUserSpecPlmnInfo.stPlmnId.ulMcc = pstUserSpecPlmnId->ulMcc;
        stUserSpecPlmnInfo.stPlmnId.ulMnc = pstUserSpecPlmnId->ulMnc;
        stUserSpecPlmnInfo.enRat          = pstUserSelReqMsg->enAccessMode;

        NAS_MMC_SetUserSpecPlmnId(&stUserSpecPlmnInfo);

        /* ��MMA�����û�ָ�������ɹ� */
        NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_SUCCESS);

        return VOS_TRUE;
    }

    /* ������λ����Ч�Ĵ��� */
    if ( VOS_FALSE == NAS_MML_IsUsimStausValid() )
    {
        /* �����ֶ��������� */
        NAS_MMC_UpdateUserSpecPlmnSearchInfo_PreProc(pstUserSpecPlmnId,
                                                           pstUserSelReqMsg->enAccessMode);

        /* ��MMA�����û�ָ������ʧ�� */
        NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_FAIL);
         return VOS_TRUE;
    }

    enFsmId = NAS_MMC_GetCurrFsmId();
    if (NAS_FSM_BUTT <= enFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    /* �ػ�����·�����Чֵ */
    /* �����ѡ״̬�����յ��û�ָ���ѣ��û�ָ��������͵�ǰפ����������ͬ��
       ���ҵ�ǰפ�����������ṩ���������򲻴����ѡ״̬����ֱ�ӻظ������ɹ� */
    if ( ((NAS_MMC_L1_STA_ON_PLMN == NAS_MMC_GetFsmTopState())
       && (NAS_MMC_FSM_L1_MAIN == enFsmId))
      || (NAS_MMC_FSM_INTER_SYS_CELLRESEL == enFsmId) )
    {
        /* �ж��û�ָ��������͵�ǰפ���������Ƿ���ͬ */
        ulRslt = NAS_MML_CompareBcchPlmnwithSimPlmn(NAS_MML_GetCurrCampPlmnId(),
                                                    pstUserSpecPlmnId);
        enCurrRatType = NAS_MML_GetCurrNetRatType();
        if ( (VOS_TRUE == ulRslt)
          && (pstUserSelReqMsg->enAccessMode == enCurrRatType) )
        {

            /* ��ǰ������������״̬��δ���й������������ڸ����ȼ�ָ������ʱ
               ���߿���ָ����ʱ�����û�ָ���Ѵ�ϣ����û�ָ�����������ǰפ����
               ������ͬ�������ȼ�����״̬���˳��󣬻᲻�����û�ָ���� */
            ulSpecPlmnSearchFlg = NAS_MMC_GetSpecPlmnSearchState();

            if ( (VOS_FALSE == NAS_MMC_NeedTrigPlmnSrch_UserSpecCurrentPlmn())
              && (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING != ulSpecPlmnSearchFlg) )
            {

                /* �����ֶ��������� */
                NAS_MMC_UpdateUserSpecPlmnSearchInfo_PreProc(pstUserSpecPlmnId,
                                                                   pstUserSelReqMsg->enAccessMode);

                /* ���õ�ǰע��״̬Ϊ�ɹ� */
                NAS_MMC_SetUserSpecPlmnRegisterStatus(VOS_TRUE);

                /* ��MMA�����û�ָ�������ɹ� */
                NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_SUCCESS);
                return VOS_TRUE;
            }
        }
    }

    ulCsServiceStatusFlg = NAS_MML_GetCsServiceExistFlg();

    if (VOS_TRUE == ulCsServiceStatusFlg)
    {
        /* ��MMA�����û�ָ������ʧ�� */
        /* ��MMA�����û�ָ�������ܾ�������������ģʽ */
        NAS_MMC_SndMmaUserSpecPlmnSearchRej();
        return VOS_TRUE;
    }

    /* �жϵ�ǰ�Ƿ���Ҫ��ϵ�ǰ״̬�� */
    ulRslt = NAS_MMC_ProcMsgPrio(ulEventType, pstMsg);

    if (VOS_TRUE == ulRslt)
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32  NAS_MMC_RcvUserReselReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMA_MMC_PLMN_USER_RESEL_REQ_STRU              *pUserReselReq       = VOS_NULL_PTR;

    pUserReselReq  = (MMA_MMC_PLMN_USER_RESEL_REQ_STRU*)pstMsg;

    /* �û������ֶ���ѡ��Ŀǰ��֧�� */
    if (MMA_MMC_PLMN_SEL_MODE_AUTO != pUserReselReq->enPlmnSelMode)
    {
        /* �쳣��ӡ */
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvUserReselReq_PreProc:Entered!");
        NAS_MMC_SndMmaUserSpecPlmnSearchCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_REJ);

        return VOS_TRUE;
    }

    /* ��ǰ������������״̬ */
    if (VOS_TRUE != NAS_MMC_IsNormalServiceStatus())
    {
        return NAS_MMC_ProcUserReselReqUnNormalSrv_PreProc(pUserReselReq);
    }
    else
    {
        return NAS_MMC_ProcUserReselReqNormalService_PreProc(pUserReselReq);
    }

}



VOS_UINT32  NAS_MMC_RcvTiAvailTimerExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                                              ulRrcConnExistFlag;

#if   (FEATURE_ON == FEATURE_LTE)
    NAS_MML_SIM_FORBIDPLMN_INFO_STRU                       *pstForbidPlmnInfo = VOS_NULL_PTR;
    VOS_UINT32                                              ulRet;
#endif

    NAS_MMC_PLMN_SELECTION_MODE_ENUM_UINT8                  enSelectionMode;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstUserSpecPlmnId = VOS_NULL_PTR;
    VOS_UINT32                                              ulForbPlmnFlag;
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;            /* ��ǰ״̬����ʶ */
    VOS_UINT32                                              ulState;            /* ��ǰ״̬��״̬ */

    enSelectionMode   = NAS_MMC_GetPlmnSelectionMode();

    pstUserSpecPlmnId = NAS_MMC_GetUserSpecPlmnId();
    ulForbPlmnFlag    = NAS_MML_IsPlmnIdInForbidPlmnList( &(pstUserSpecPlmnId->stPlmnId) );

    /* ��ȡ��ǰ״̬��ID����״̬ */
    enFsmId           = NAS_MMC_GetCurrFsmId();
    if (NAS_FSM_BUTT <= enFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    ulState           = NAS_MMC_GetFsmTopState();

    /* ��ǰΪ�ֶ�ģʽ���û�ָ���������ڽ�ֹ�����б��У��ҵ�ǰ��ΪOOC״̬��
       ��˴�����û�����壬�ʲ�����AvailTimer��ʱ��Ϣ */
    if ( (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == enSelectionMode)
      && (VOS_TRUE                           == ulForbPlmnFlag)
      && (NAS_MMC_FSM_L1_MAIN                == enFsmId)
      && (NAS_MMC_L1_STA_OOC                 != ulState) )
    {
        return VOS_TRUE;
    }

#if   (FEATURE_ON == FEATURE_LTE)
    /* LTE ONLY ��ǰΪ�ֶ�ģʽ���û�ָ���������ڽ�ֹGPRS�б��У��ҵ�ǰ��ΪOOC״̬��
       ��˴�����û�����壬�ʲ�����AvailTimer��ʱ��Ϣ */
    pstForbidPlmnInfo = NAS_MML_GetForbidPlmnInfo();

    ulRet = NAS_MML_IsSimPlmnIdInDestBcchPlmnList(&(pstUserSpecPlmnId->stPlmnId),
                                                  pstForbidPlmnInfo->ucForbGprsPlmnNum,
                                                  pstForbidPlmnInfo->astForbGprsPlmnList);

    if ((VOS_TRUE == NAS_MML_IsLteOnlyMode(NAS_MML_GetMsPrioRatList()))
     && (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == enSelectionMode)
     && (VOS_TRUE == ulRet )
     && (NAS_MMC_FSM_L1_MAIN == enFsmId)
     && (NAS_MMC_L1_STA_OOC != ulState))
    {
        return VOS_TRUE;
    }
#endif

    /* �ж����������Ƿ��Ѿ�����ͷ� */
    ulRrcConnExistFlag          = NAS_MML_IsRrcConnExist();

    /* ������ʱ������CTX�еı�����¼��Ҫ����*/
    if (VOS_TRUE == ulRrcConnExistFlag)
    {
        NAS_MMC_SetBufferedPlmnSearchInfo(VOS_TRUE, NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED);

        return VOS_TRUE;
    }

    /* ��״̬���ﴥ������ */
    return VOS_FALSE;
}



VOS_VOID NAS_MMC_RcvWasCellId_PreProc(
    RRC_NAS_CELL_ID_ST                  *pstWasCellId
)
{
    NAS_MML_CAMP_CELL_INFO_STRU         *pstCampCellInfo     = VOS_NULL_PTR;
    NAS_MML_CAMP_PLMN_INFO_STRU         *pstCurrCampPlmnInfo = VOS_NULL_PTR;


    /* ������פ��̬�򲻴�����Ϣ�ϱ� */
    if (NAS_MMC_AS_CELL_NOT_CAMP_ON == NAS_MMC_GetAsCellCampOn() )
    {
        return;
    }


    /*�ϱ�С����������8����Ϊ���� */
    if (pstWasCellId->ucIDNum > RRC_NAS_AT_CELL_MAX_NUM)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC ,"NAS_MMC_RcvWasAtMsgCnf():WARNING:the NUM is out limit.");
        return;
    }


    pstCampCellInfo     = NAS_MML_GetCampCellInfo();
    pstCurrCampPlmnInfo = NAS_MML_GetCurrCampPlmnInfo();

    /* ��С�����ı䣬�����������ı�ʱ�����ϱ�*/
    if ( (pstCampCellInfo->astCellInfo[0].ulCellId == pstWasCellId->aulCellId[0])
       &&(pstCurrCampPlmnInfo->enSysSubMode != pstWasCellId->enSysSubMode))
    {
        /* ����WRR�ϱ��ĵ�ǰС������̬�µ�ϵͳ��ģʽ */
        pstCurrCampPlmnInfo->enSysSubMode = pstWasCellId->enSysSubMode;

        /*����С����Ϣ�ϱ��������ϱ�tafС����Ϣ*/
        NAS_MMC_SndMmaSysInfo();
    }


}




VOS_VOID NAS_MMC_RcvWasCellBler_PreProc(
    RRC_NAS_CELL_TRCH_BLER_STRU_LIST    *pstWasCellBler
)
{
    VOS_UINT32                           i;
    NAS_MML_CAMP_CELL_INFO_STRU         *pstCampCellInfo;
    MMA_MMC_SCELL_MEAS_REPORT_TYPE_UN    unMeasReportType;

    pstCampCellInfo  = NAS_MML_GetCampCellInfo();

    PS_MEM_SET(((VOS_UINT8*)&unMeasReportType), 0, sizeof(MMA_MMC_SCELL_MEAS_REPORT_TYPE_UN));
    unMeasReportType.ucMeasReportType |= MMA_MMC_SCELL_MEAS_TYPE_RXQUALITY;

    /*�����ʳ���8����Ϊ��Ϣ��Ч */
    if ( pstWasCellBler->ucTrchNum > RRC_NAS_AT_CELL_MAX_NUM)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvWasAtMsgCnf():WARNING:the BLER NUM is out limit.");
        return;
    }


    /*�����ϱ���������Ϣ*/
    pstCampCellInfo->ucRssiNum = pstWasCellBler->ucTrchNum;

    for ( i = 0; i < pstWasCellBler->ucTrchNum ; i++ )
    {
        if ( pstWasCellBler->stTrchBlerInfo[i].ucDlTrChBler > NAS_MML_UTRA_BLER_INVALID)
        {
            pstCampCellInfo->astRssiInfo[i].ucChannalQual = NAS_MML_UTRA_BLER_INVALID;
        }
        else
        {
            pstCampCellInfo->astRssiInfo[i].ucChannalQual = pstWasCellBler->stTrchBlerInfo[i].ucDlTrChBler;
        }
    }

    /*�����ϱ��ź�ǿ��*/
    NAS_MMC_SndMmaRssiInd(&unMeasReportType);
}

VOS_VOID NAS_MMC_RcvWasCellRssi_PreProc(
    RRC_NAS_CELL_RSCP_STRU_LIST         *pstWasCellRscp
)
{
    VOS_UINT32                           i;
    NAS_MML_CAMP_CELL_INFO_STRU         *pstCampCellInfo;
    MMA_MMC_SCELL_MEAS_REPORT_TYPE_UN    unMeasReportType;

    pstCampCellInfo  = NAS_MML_GetCampCellInfo();

    PS_MEM_SET(((VOS_UINT8*)&unMeasReportType), 0, sizeof(MMA_MMC_SCELL_MEAS_REPORT_TYPE_UN));
    unMeasReportType.ucMeasReportType |= MMA_MMC_SCELL_MEAS_TYPE_RXLEV;

    if ( pstWasCellRscp->usCellNum > RRC_NAS_AT_CELL_MAX_NUM)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC ,"NAS_MMC_RcvWasCellRssi_PreProc():WARNING:RSCP NUM is out limit.");
        return;
    }

    pstCampCellInfo->ucRssiNum  = (VOS_UINT8)pstWasCellRscp->usCellNum;

    for ( i = 0; i < pstWasCellRscp->usCellNum ; i++ )
    {
        if ( (pstWasCellRscp->stCellRscp[i].sCellRSCP > NAS_MML_RSCP_UPPER_LIMIT)
          || (pstWasCellRscp->stCellRscp[i].sCellRSCP < NAS_MML_RSCP_LOWER_LIMIT))
        {
            pstCampCellInfo->astRssiInfo[i].sRscpValue = NAS_MML_UTRA_RSCP_UNVALID;
        }
        else
        {
            pstCampCellInfo->astRssiInfo[i].sRscpValue = pstWasCellRscp->stCellRscp[i].sCellRSCP;
        }

        if ((pstWasCellRscp->stCellRscp[i].sRSSI > NAS_MML_RSSI_UPPER_LIMIT)
         || (pstWasCellRscp->stCellRscp[i].sRSSI < NAS_MML_RSSI_LOWER_LIMIT))
        {
            pstCampCellInfo->astRssiInfo[i].sRssiValue = NAS_MML_RSSI_UNVALID;
        }
        else
        {
            pstCampCellInfo->astRssiInfo[i].sRssiValue  = pstWasCellRscp->stCellRscp[i].sRSSI;
        }
    }

    pstCampCellInfo->usCellDlFreq   = pstWasCellRscp->usCellDlFreq;
    pstCampCellInfo->usCellUlFreq   = pstWasCellRscp->usCellUlFreq;
    pstCampCellInfo->sUeRfPower     = pstWasCellRscp->sUeRfPower;

    for ( i = 0; i < pstWasCellRscp->usCellNum ; i++ )
    {
        if ((pstWasCellRscp->stCellRscp[i].sCellRSCP > NAS_MML_RSCP_UPPER_LIMIT)
            ||(pstWasCellRscp->stCellRscp[i].sCellRSCP < NAS_MML_RSCP_LOWER_LIMIT))
        {
            pstCampCellInfo->astCellInfo[i].sCellRSCP = NAS_MML_UTRA_RSCP_UNVALID;
        }
        else
        {
            pstCampCellInfo->astCellInfo[i].sCellRSCP = pstWasCellRscp->stCellRscp[i].sCellRSCP;
        }
    }

    /*�����ϱ��ź�ǿ��*/
    NAS_MMC_SndMmaRssiInd(&unMeasReportType);
}



VOS_VOID  NAS_MMC_UpdateUserSpecPlmnSearchInfo_PreProc(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    VOS_UINT8                           ucAccessMode
)
{
    NAS_MML_PLMN_WITH_RAT_STRU          stUserSpecPlmn;

    /* �����û�ָ����PlmnID������뼼�� */
    stUserSpecPlmn.stPlmnId.ulMcc = pstPlmnId->ulMcc;
    stUserSpecPlmn.stPlmnId.ulMnc = pstPlmnId->ulMnc;

    stUserSpecPlmn.enRat          = ucAccessMode;
    NAS_MMC_SetUserSpecPlmnId(&stUserSpecPlmn);

    /* ����ָ������ǰפ����PlmnID�����뼼�� */
    NAS_MMC_SaveUserReselPlmnInfo();

    /* ��ǰģʽ����MANUAL���򲻸���NV */
    if (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
    {
        return;
    }

    /* ���õ�ǰע��״̬δע�� */
    NAS_MMC_SetUserSpecPlmnRegisterStatus(VOS_FALSE);

    /* ���õ�ǰ����ģʽΪ�ֶ�ģʽ */
    NAS_MMC_SetPlmnSelectionMode(NAS_MMC_PLMN_SELECTION_MODE_MANUAL);


    /* ��������ȼ�������ʱ�������й����У���ֹͣ�ö�ʱ�� */
    if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_HPLMN_TIMER))
    {
        NAS_MMC_StopTimer(TI_NAS_MMC_HPLMN_TIMER);

        /* HPLMN TIMER ֹͣ�¼��ϱ� */
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_HPLMN_TIMER_STOP,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);
    }

    /* ������ڸ����ȼ�������ʱ�������й����У���ֹͣ�ö�ʱ�� */
    if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_PERIOD_TRYING_HIGH_PRIO_PLMN_SEARCH))
    {
        NAS_MMC_StopTimer(TI_NAS_MMC_PERIOD_TRYING_HIGH_PRIO_PLMN_SEARCH);
    }

    if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_HIGH_PRIO_RAT_HPLMN_TIMER))
    {
        NAS_MMC_StopTimer(TI_NAS_MMC_HIGH_PRIO_RAT_HPLMN_TIMER);
        NAS_MMC_ResetCurHighPrioRatHplmnTimerFirstSearchCount_L1Main();
    }

    /* ����ǰ����ģʽд��NVIM�� */
    NAS_MMC_WritePlmnSelectionModeNvim();

    /* ����ģʽ�仯�ϱ�USIM */
    if (VOS_TRUE == NAS_MMC_IsNeedSndStkSearchModeChgEvt())
    {
        NAS_MMC_SndStkSearchModeChgEvt(NAS_MMC_PLMN_SELECTION_MODE_MANUAL);
    }

}


VOS_UINT32 NAS_MMC_CheckUserSpecPlmnSearchParamMsg_PreProc(
    struct MsgCB                       *pstMsg
)
{
    MMA_MMC_PLMN_SPECIAL_REQ_STRU      *pstUserSelReqMsg   = VOS_NULL_PTR;
    NAS_MML_PLMN_ID_STRU               *pstUserSpecPlmnId  = VOS_NULL_PTR;

    pstUserSelReqMsg = (MMA_MMC_PLMN_SPECIAL_REQ_STRU*)pstMsg;

    /* ָ���û�ָ��������Ϣ�е�PlmnId */
    pstUserSpecPlmnId  = (NAS_MML_PLMN_ID_STRU*)&(pstUserSelReqMsg->stPlmnId);

    /* �ж��û�ָ����Plmn�Ƿ���Ч����Ч���򷵻�VOS_FALSE */
    if (VOS_FALSE == NAS_MML_IsPlmnIdValid(pstUserSpecPlmnId))
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_CheckUserSpecPlmnSearchParamMsg_PlmnSelection():plmn invalid");
        return VOS_FALSE;
    }

    /* ��PLMN ID�Ƿ�������������PLMN �б��� */
    if (VOS_TRUE == NAS_MML_IsBcchPlmnIdInLockPlmnList(pstUserSpecPlmnId))
    {
        return VOS_FALSE;
    }

    /* ָ��PLMN ����������, ����VOS_FALSE */
    if ( VOS_TRUE != NAS_MML_IsPlmnIdRoamingAllowed(pstUserSpecPlmnId) )
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}



VOS_UINT32  NAS_MMC_RcvTafPlmnListReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    /* ��ǰ��PSҵ���жϲ�׼ȷ,�յ�LIST��������RRC�ж��ܷ���,
       CSҵ��ֱ�ӻظ�REJ */
    if (VOS_TRUE == NAS_MML_GetCsServiceExistFlg())
    {
        /* ����LIST REJ��MMA */
        NAS_Mmc_SndMmaPlmnListRej();

        return VOS_TRUE;
    }



    /* �����Գ����ڲ�LIST������ʱ�������У���ֹͣ */
    if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(
                                       TI_NAS_MMC_PERIOD_TRYING_INTER_PLMN_LIST))
    {
        /* ֹͣTI_NAS_MMC_PERIOD_TRYING_INTER_PLMN_LIST */
        NAS_MMC_StopTimer(TI_NAS_MMC_PERIOD_TRYING_INTER_PLMN_LIST);
    }

    /* ��״̬������״̬�����ܴ���������� */
    return VOS_FALSE;
}


VOS_UINT32  NAS_MMC_RcvTiPeriodTryingUserPlmnListExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{

    /* ��ǰ��PSҵ���жϲ�׼ȷ,�յ�LIST��������RRC�ж��ܷ���,
       CSҵ��ֱ�ӻظ�REJ */
    if (VOS_TRUE == NAS_MML_GetCsServiceExistFlg())
    {
        /* ����LIST REJ��MMA */
        NAS_Mmc_SndMmaPlmnListRej();

        return VOS_TRUE;
    }



    /* ��״̬������״̬�����ܴ���������� */
    return VOS_FALSE;
}
VOS_UINT32  NAS_MMC_RcvTiPeriodTryingInterPlmnListExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucCsSigConnStatusFlg;
    NAS_MMC_AS_CELL_CAMP_ON_ENUM_UINT8  enCampStaus;
    NAS_MMC_FSM_ID_ENUM_UINT32          enCurFsmId;

    VOS_UINT32                          ulSingleDomainNeedPlmnSrch;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurRatType;

    enCurRatType                = NAS_MML_GetCurrNetRatType();
#endif

    ulSingleDomainNeedPlmnSrch  = NAS_MMC_IsSingleDomainRegFailNeedPlmnSrch(MMA_MMC_SRVDOMAIN_PS);

    /* ����Ѿ�������Available��ʱ����������������Ҫ�����ڲ��б��ѳ�ʱ��Ϣ����������Ϣ */
    if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_AVAILABLE_TIMER))
    {
        return VOS_TRUE;
    }

    /* ��ǰ��������,��������Ϣ */
    if ( VOS_TRUE == NAS_MMC_IsNormalServiceStatus())
    {
#if   (FEATURE_ON == FEATURE_LTE)
        if (NAS_MML_NET_RAT_TYPE_LTE == enCurRatType)
        {
            if (VOS_FALSE == NAS_MMC_IsCsPsMode1NeedPeriodSearchGU())
            {
                return VOS_TRUE;
            }
        }
        else
        {
            if ((VOS_FALSE == NAS_MMC_IsSingleDomainRegFailNeedPlmnSrch(MMA_MMC_SRVDOMAIN_CS))
             && (VOS_FALSE == ulSingleDomainNeedPlmnSrch))
            {
                return VOS_TRUE;
            }
        }
#else
        if ((VOS_FALSE == NAS_MMC_IsSingleDomainRegFailNeedPlmnSrch(MMA_MMC_SRVDOMAIN_CS))
         && (VOS_FALSE == ulSingleDomainNeedPlmnSrch))
        {
            return VOS_TRUE;
        }
#endif
    }

    /* ��ǰ�޿�ʱ����������Ϣ */
    if (VOS_FALSE == NAS_MML_IsUsimStausValid())
    {
        return VOS_TRUE;
    }

    /* ��ǰ״̬��ID */
    enCurFsmId = NAS_MMC_GetCurrFsmId();
    if (NAS_FSM_BUTT <= enCurFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    /* ��״̬�����е��ж� */
    if ( NAS_MMC_FSM_L1_MAIN != enCurFsmId)
    {
        /* Ŀǰֻ��������״̬�����й����ж�������Ϣ */
        if ((NAS_MMC_FSM_PLMN_SELECTION == enCurFsmId)
          ||(NAS_MMC_FSM_ANYCELL_SEARCH == enCurFsmId))
        {
            return VOS_TRUE;
        }

        /* ����״̬������ʱ������ʱ�������Գ��� */
        NAS_MMC_StartTimer(TI_NAS_MMC_PERIOD_TRYING_INTER_PLMN_LIST, TI_NAS_MMC_PERIOD_TRYING_INTER_PLMN_LIST_LEN);

        return VOS_TRUE;
    }

    enCampStaus                 = NAS_MMC_GetAsCellCampOn();
    ucCsSigConnStatusFlg        = NAS_MML_GetCsSigConnStatusFlg();

    /* ��ǰ��PSҵ���жϲ�׼ȷ,�յ�LIST��������RRC�ж��ܷ���,
       CSҵ������CS�������ӻ�δפ��ʱ������������LIST��������ʱ������ */
    if ((VOS_TRUE                    == NAS_MML_GetCsServiceExistFlg())
     || (NAS_MMC_AS_CELL_NOT_CAMP_ON == enCampStaus)
     || (VOS_TRUE                    == ucCsSigConnStatusFlg))
    {
        /* ������ʱ��TI_NAS_MMC_PERIOD_TRYING_INTER_PLMN_LIST */
        NAS_MMC_StartTimer(TI_NAS_MMC_PERIOD_TRYING_INTER_PLMN_LIST, TI_NAS_MMC_PERIOD_TRYING_INTER_PLMN_LIST_LEN);

        return VOS_TRUE;
    }


    /* ����L1 MAIN���� */
    return VOS_FALSE;
}
VOS_UINT32  NAS_MMC_RcvMmcInterPlmnListReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucCsSigConnStatusFlg;
    NAS_MMC_AS_CELL_CAMP_ON_ENUM_UINT8  enCampStaus;
    VOS_UINT32                          ulSingleDomainNeedPlmnSrch;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurRatType;

    enCurRatType            = NAS_MML_GetCurrNetRatType();
#endif

    ulSingleDomainNeedPlmnSrch = NAS_MMC_IsSingleDomainRegFailNeedPlmnSrch(MMA_MMC_SRVDOMAIN_PS);

    /* �����Գ����û�LIST������ʱ�������У��򲻴����ڲ�LIST��,��������Ϣ */
    if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_PERIOD_TRYING_USER_PLMN_LIST))
    {
        return VOS_TRUE;
    }

    /* ��ǰ����������Ҳ���CS/PS mode 1פ��LTE����������GSM��WCDMA����,
       ��������Ϣ */
    if (VOS_TRUE == NAS_MMC_IsNormalServiceStatus())
    {
#if   (FEATURE_ON == FEATURE_LTE)
        if (NAS_MML_NET_RAT_TYPE_LTE == enCurRatType)
        {
            if (VOS_FALSE == NAS_MMC_IsCsPsMode1NeedPeriodSearchGU())
            {
                return VOS_TRUE;
            }
        }
        else
        {
            if ((VOS_FALSE == NAS_MMC_IsSingleDomainRegFailNeedPlmnSrch(MMA_MMC_SRVDOMAIN_CS))
             && (VOS_FALSE == ulSingleDomainNeedPlmnSrch))
            {
                return VOS_TRUE;
            }
        }
#else
        if ((VOS_FALSE == NAS_MMC_IsSingleDomainRegFailNeedPlmnSrch(MMA_MMC_SRVDOMAIN_CS))
         && (VOS_FALSE == ulSingleDomainNeedPlmnSrch))
        {
            return VOS_TRUE;
        }
#endif
    }

    /* ��ǰ�޿�ʱ����������Ϣ */
    if (VOS_FALSE == NAS_MML_IsUsimStausValid())
    {
        return VOS_TRUE;
    }

    enCampStaus                 = NAS_MMC_GetAsCellCampOn();
    ucCsSigConnStatusFlg        = NAS_MML_GetCsSigConnStatusFlg();

    /* ��ǰ��PSҵ���жϲ�׼ȷ,�յ�LIST��������RRC�ж��ܷ���,
       CSҵ������CS�������ӻ�δפ��ʱ������������LIST��������ʱ������ */
    if ((VOS_TRUE                    == NAS_MML_GetCsServiceExistFlg())
     || (NAS_MMC_AS_CELL_NOT_CAMP_ON == enCampStaus)
     || (VOS_TRUE                    == ucCsSigConnStatusFlg))
    {
        /* ������ʱ��TI_NAS_MMC_PERIOD_TRYING_INTER_PLMN_LIST */
        NAS_MMC_StartTimer(TI_NAS_MMC_PERIOD_TRYING_INTER_PLMN_LIST, TI_NAS_MMC_PERIOD_TRYING_INTER_PLMN_LIST_LEN);

        return VOS_TRUE;
    }


    /* L1 MAIN�ϣ���״̬���ﴥ������ */
    return VOS_FALSE;
}
VOS_UINT32  NAS_MMC_RcvTafPlmnListAbortReq_PlmnList_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulCacheMsgIdx;
    VOS_UINT32                          ulExistFlg;
    VOS_UINT32                          ulCacheEventType;
    NAS_MMC_FSM_ID_ENUM_UINT32          ulCurrentFsmId;
    VOS_UINT32                          ulFsmIdInFsmStackFlg;

    /* �û�LIST������Ϣ*/
    ulCacheEventType     = NAS_BuildEventType(WUEPS_PID_MMA, ID_MMA_MMC_PLMN_LIST_REQ);
    ulExistFlg           = NAS_MMC_IsExistCacheMsg(ulCacheEventType, &ulCacheMsgIdx);
    ulCurrentFsmId       = NAS_MMC_GetCurrFsmId();

    if (NAS_FSM_BUTT <= ulCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }

    ulFsmIdInFsmStackFlg = NAS_MMC_IsFsmIdInFsmStack(NAS_MMC_FSM_PLMN_LIST);

    /* ���û�LIST�������� */
    if (VOS_TRUE == ulExistFlg )
    {
        /* ������� */
        NAS_MMC_ClearCacheMsg(ulCacheEventType);

        /* ��MMA�ظ�ID_MMC_MMA_PLMN_LIST_ABORT_CNF */
        NAS_MMC_SndMmaPlmnListAbortCnf(VOS_NULL_PTR);

        return VOS_TRUE;
    }

    /* �����Գ����û�LIST������ʱ���Ƿ������� */
    if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_PERIOD_TRYING_USER_PLMN_LIST))
    {
        /* ֹͣTI_NAS_MMC_PERIOD_TRYING_USER_PLMN_LIST */
        NAS_MMC_StopTimer(TI_NAS_MMC_PERIOD_TRYING_USER_PLMN_LIST);

        /* ��MMA�ظ�ID_MMC_MMA_PLMN_LIST_ABORT_CNF */
        NAS_MMC_SndMmaPlmnListAbortCnf(VOS_NULL_PTR);

        return VOS_TRUE;
    }

    /* LIST����״̬��δ���У�Ϊ��֤ATͨ����������ʱҲ��MMA�ظ���� */
    if ((ulCurrentFsmId != NAS_MMC_FSM_PLMN_LIST)
     && (VOS_FALSE == ulFsmIdInFsmStackFlg))
    {
        /* �쳣��ӡ */
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTafPlmnListAbortReq_PlmnList_PreProc:ERROR");

        /* ��MMA�ظ�ID_MMC_MMA_PLMN_LIST_ABORT_CNF */
        NAS_MMC_SndMmaPlmnListAbortCnf(VOS_NULL_PTR);

        return VOS_TRUE;
    }

    /* ��״̬������ */
    return VOS_FALSE;
}




VOS_UINT32  NAS_MMC_RcvTafSpecPlmnSearchAbortReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulCacheMsgIdx;
    VOS_UINT32                          ulExistFlg;
    VOS_UINT32                          ulCacheEventType;
    NAS_MMC_FSM_ID_ENUM_UINT32          ulCurrentFsmId;
    VOS_UINT32                          ulFsmIdInFsmStackFlg;

    /* �û�ָ��������Ϣ*/
    ulCacheEventType     = NAS_BuildEventType(WUEPS_PID_MMA, ID_MMA_MMC_PLMN_SPECIAL_REQ);
    ulExistFlg           = NAS_MMC_IsExistCacheMsg(ulCacheEventType, &ulCacheMsgIdx);
    ulCurrentFsmId       = NAS_MMC_GetCurrFsmId();
    ulFsmIdInFsmStackFlg = NAS_MMC_IsFsmIdInFsmStack(NAS_MMC_FSM_PLMN_SELECTION);

    if (NAS_FSM_BUTT <= ulCurrentFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }

    /* ���û�ָ���������� */
    if (VOS_TRUE == ulExistFlg )
    {
        /* ������� */
        NAS_MMC_ClearCacheMsg(ulCacheEventType);

        /* ��MMA�ظ�TAFMMC_SPEC_PLMN_SEARCH_ABORT_CNF */
        NAS_MMC_SndMmaSpecPlmnSearchAbortCnf();
        return VOS_TRUE;
    }

    /* ָ������״̬��δ�����У�Ϊ��֤ATͨ����������ʱҲ��MMA�ظ���� */
    if ((ulCurrentFsmId != NAS_MMC_FSM_PLMN_SELECTION)
     && (VOS_FALSE == ulFsmIdInFsmStackFlg))
    {
        /* �쳣��ӡ */
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTafSpecPlmnSearchAbortReq_PreProc:ERROR");

        /* ��MMA�ظ�TAFMMC_SPEC_PLMN_SEARCH_ABORT_CNF */
        NAS_MMC_SndMmaSpecPlmnSearchAbortCnf();
        return VOS_TRUE;
    }

    /* ��״̬������ */
    return VOS_FALSE;
}
VOS_UINT32 NAS_MMC_IsConnExist_BgPlmnSearch_PreProc( VOS_VOID )
{
    NAS_MML_CONN_STATUS_INFO_STRU      *pstConnStatus = VOS_NULL_PTR;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurrRat;

    /* ȡ�õ�ǰ�Ľ���ģʽ */
    enCurrRat     = NAS_MML_GetCurrNetRatType();

    /* ȡ�õ�ǰ��������Ϣ */
    pstConnStatus = NAS_MML_GetConnStatus();

    /* ����CS�����������,��������BG���� */
    if (VOS_TRUE == pstConnStatus->ucCsSigConnStatusFlg)
    {
        return VOS_TRUE;
    }

    /* ����CS���ҵ��򻺴��ҵ��,��������BG���� */
    if (VOS_TRUE == NAS_MML_GetCsServiceExistFlg())
    {
        return VOS_TRUE;
    }

    /* ����PS��Ļ����ҵ��,��������BG���� */
    if ( VOS_TRUE == pstConnStatus->ucPsServiceBufferFlg)
    {
        return VOS_TRUE;
    }

    /* GSMģʽ��,TBF����,��������BG���� */
    if ((VOS_TRUE == pstConnStatus->ucPsTbfStatusFlg)
     && (NAS_MML_NET_RAT_TYPE_GSM == enCurrRat))
    {
        return VOS_TRUE;
    }

#if (FEATURE_ON == FEATURE_LTE)

    /* L��PS�����������ӻ�ҵ����ڣ���������BG���� */
    if ((NAS_MML_NET_RAT_TYPE_LTE == enCurrRat)
     && ((VOS_TRUE == pstConnStatus->ucEpsSigConnStatusFlg)
      || (VOS_TRUE == pstConnStatus->ucEpsServiceConnStatusFlg)))
    {
        return VOS_TRUE;
    }
#endif

    /* PS�򴥷���TC����ҵ�����,��������BG���� */
    if ( VOS_TRUE == pstConnStatus->ucPsTcServiceFlg )
    {
        return VOS_TRUE;
    }

    /* ���������Ϊ������ҵ�����������,���Է���BG���� */
    return VOS_FALSE;

}



VOS_UINT32 NAS_MMC_IsEnableBgPlmnSearch_PreProc( VOS_VOID )
{
    NAS_MML_CONN_STATUS_INFO_STRU      *pstConnStatus = VOS_NULL_PTR;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurrRat;

    /* ȡ�õ�ǰ�Ľ���ģʽ */
    enCurrRat     = NAS_MML_GetCurrNetRatType();

    /* ȡ�õ�ǰ��������Ϣ */
    pstConnStatus = NAS_MML_GetConnStatus();


    /* ����Ѿ�������Available��ʱ����������������Ҫ���𱳾����� */
    if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_AVAILABLE_TIMER))
    {
        return VOS_FALSE;
    }

    /* ��������Գ����û��б�������ʱ�������й����У��������𱳾����� */
    if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_PERIOD_TRYING_USER_PLMN_LIST))
    {
        return VOS_FALSE;
    }

    /* ��������Գ����ڲ��б�������ʱ�������й����У��������𱳾����� */
    if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_PERIOD_TRYING_INTER_PLMN_LIST))
    {
        return VOS_FALSE;
    }

    /* ���������ϱ�not camp on��Ϣ���������𱳾����� */
    if (NAS_MMC_AS_CELL_NOT_CAMP_ON == NAS_MMC_GetAsCellCampOn())
    {
        return VOS_FALSE;
    }

    /* Ϊ�˱��ⷴ�����ø����ȼ������Ĵ�����10s�����Գ��Զ�ʱ�������ڼ䲻�����̷��𱳾����� */
    if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_PERIOD_TRYING_HIGH_PRIO_PLMN_SEARCH))
    {
        return VOS_FALSE;
    }

    /* �����������ӻ�ҵ�����Ӳ������𱳾����� */
    if (VOS_TRUE == NAS_MMC_IsConnExist_BgPlmnSearch_PreProc())
    {
        return VOS_FALSE;
    }

    /* ����״̬��������,�����з��𱳾����� */
    if( NAS_MMC_FSM_L1_MAIN != NAS_MMC_GetCurrFsmId())
    {
        return VOS_FALSE;
    }

    /* GSMģʽ�£�RRC���Ӵ��ڲ������𱳾�������
       WCDMAģʽ�£�RRC���Ӵ���P̬,�����𱳾�������*/
    if ((VOS_TRUE == pstConnStatus->ucRrcStatusFlg)
     && (NAS_MML_NET_RAT_TYPE_WCDMA != enCurrRat))
    {
        return VOS_FALSE;
    }

    /* ����������Է���BG���� */
    return VOS_TRUE;

}



VOS_UINT32 NAS_MMC_ProcTiHplmnTimerExpired_PreProc(VOS_VOID)
{
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstHighPrioPlmnSearchList = VOS_NULL_PTR;
    NAS_MML_BG_SEARCH_CFG_INFO_STRU                        *pstBgSearchCfg            = VOS_NULL_PTR;


    /* ��ȡ�����ȼ������б���Ϣ */
    pstHighPrioPlmnSearchList = NAS_MMC_GetHighPrioPlmnList();


    /* HPLMN TIMER�����¼��ϱ� */
    NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_HPLMN_TIMER_EXPIRE,
                    VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);


    /* ��Ҫ�ж�HPLMN Timer��ʱ���˿��Ƿ���Ч����Ч��ֱ�Ӷ�������Ϣ */
    if ( VOS_FALSE == NAS_MMC_IsHighPrioPlmnSearchVaild() )
    {
        return VOS_TRUE;
    }

    pstBgSearchCfg            = NAS_MML_GetBgSearchCfg();

    /* �Ѿ�פ����������ȼ���������ֱ�ӷ��ض�������Ϣ */
    if ( VOS_TRUE == NAS_MMC_IsCampOnHighestPrioPlmn() )
    {
        /* �����ǿ��Hplmn����,���綨�����Կ��ش�: ����Hplmn Timer��ʱ��,
           ֻҪû��פ����home PLMN��,Ҳ��Ҫ�����˶�ʱ��,��һ������2Min,��������T Min����*/
        if ((VOS_FALSE == NAS_MMC_IsCampOnHPlmn())
         && (VOS_TRUE == pstBgSearchCfg->ucEnhancedHplmnSrchFlg))
        {
            NAS_MMC_StartTimer(TI_NAS_MMC_HPLMN_TIMER,  NAS_MMC_GetHplmnTimerLen());


            /* HPLMN TIMER�����¼��ϱ� */
            NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_HPLMN_TIMER_START,
                            VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        }
        else
        {
            /* �Ѿ�פ����������ȼ�������,������ǿHPLMN����δ����,�´ΰ��״�����HPLMN��ʱ�� */
            NAS_MMC_SetFirstStartHPlmnTimerFlg(VOS_TRUE);
        }

        return VOS_TRUE;
    }

    /* �޷����и����ȼ�����ʱ,����10s�����Գ��Ը����ȼ�������ʱ�� */
    if ( VOS_FALSE == NAS_MMC_IsEnableBgPlmnSearch_PreProc() )
    {
        NAS_MMC_StartTimer(TI_NAS_MMC_PERIOD_TRYING_HIGH_PRIO_PLMN_SEARCH, TI_NAS_MMC_PERIOD_TRYING_HIGH_PRIO_PLMN_SEARCH_LEN);

        /* ɾ��enable L���߼�,�ŵ�����ָ������������ */

        /* ��ʼ�������ȼ������б� */
        NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_PLMN_SEARCH,
                                      VOS_NULL_PTR,
                                      pstHighPrioPlmnSearchList);

        return VOS_TRUE;
    }



    return VOS_FALSE;
}
VOS_UINT32 NAS_MMC_ProcTiTryingHighPrioPlmnSearchExpired_PreProc(VOS_VOID)
{
    NAS_MML_BG_SEARCH_CFG_INFO_STRU    *pstBgSearchCfg  = VOS_NULL_PTR;

    NAS_MML_SIM_STATUS_STRU            *pstSimStatus    = VOS_NULL_PTR;

    pstSimStatus = NAS_MML_GetSimStatus();

    /* ��Ҫ�ж�10s�����Գ��Ը����ȼ�������ʱ���˿��Ƿ���Ч����Ч��ֱ�Ӷ�������Ϣ,Ԥ������� */

    /* �ֶ�ģʽ�·�����Ч */
    if (NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
    {
        return VOS_TRUE;
    }

    /* SIM������λ��CS��PS��Ŀ�����Чʱ������Ч */
    if ( VOS_FALSE == NAS_MML_GetSimPresentStatus() )
    {
        return VOS_TRUE;
    }

    if (( VOS_FALSE == pstSimStatus->ucSimPsRegStatus)
     && ( VOS_FALSE == pstSimStatus->ucSimCsRegStatus))
    {
        return VOS_TRUE;
    }

    pstBgSearchCfg            = NAS_MML_GetBgSearchCfg();

    /* �Ѿ�פ����������ȼ���������ֱ�ӷ��ض�������Ϣ */
    if ( VOS_TRUE == NAS_MMC_IsCampOnHighestPrioPlmn() )
    {
        /* �����ǿ��Hplmn����,���綨�����Կ��ش�: ����Hplmn Timer��ʱ��,
           ֻҪû��פ����home PLMN��,Ҳ��Ҫ�����˶�ʱ��,��һ������2Min,��������T Min����*/
        if ((VOS_FALSE == NAS_MMC_IsCampOnHPlmn())
         && (VOS_TRUE == pstBgSearchCfg->ucEnhancedHplmnSrchFlg))
        {
            NAS_MMC_StartTimer(TI_NAS_MMC_HPLMN_TIMER,  NAS_MMC_GetHplmnTimerLen());


            /* HPLMN TIMER�����¼��ϱ� */
            NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_HPLMN_TIMER_START,
                            VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        }
        else
        {
            /* �Ѿ�פ����������ȼ�������,������ǿHPLMN����δ����,�´ΰ��״�����HPLMN��ʱ�� */
            NAS_MMC_SetFirstStartHPlmnTimerFlg(VOS_TRUE);
        }

        return VOS_TRUE;
    }

    /* �޷����и����ȼ�����ʱ,����10s�����Գ��Ը����ȼ�������ʱ�� */
    if ( VOS_FALSE == NAS_MMC_IsEnableBgPlmnSearch_PreProc() )
    {
        NAS_MMC_StartTimer(TI_NAS_MMC_PERIOD_TRYING_HIGH_PRIO_PLMN_SEARCH, TI_NAS_MMC_PERIOD_TRYING_HIGH_PRIO_PLMN_SEARCH_LEN);
        return VOS_TRUE;
    }

    return VOS_FALSE;

}
VOS_UINT32 NAS_MMC_RcvTiHplmnTimerExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRst;

    ulRst = NAS_MMC_ProcTiHplmnTimerExpired_PreProc();
    return ulRst;
}


VOS_UINT32 NAS_MMC_RcvTiTryingHighPrioPlmnSearchExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulRst;

    ulRst = NAS_MMC_ProcTiTryingHighPrioPlmnSearchExpired_PreProc();

    return ulRst;
}



VOS_UINT32  NAS_MMC_RcvStkSteerRoamingInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                        *pstMsg
)
{
    VOS_UINT16                          usOplmnNum;
    VOS_UINT8                           ucUpdateUsimForbPlmnFlg;
    VOS_UINT32                          i;
    NAS_MML_SIM_OPERPLMN_INFO_STRU     *pstOperPlmnInfo   = VOS_NULL_PTR;
    NAS_MML_SIM_PLMN_WITH_RAT_STRU     *pstOplmnAddr      = VOS_NULL_PTR;
    STK_NAS_STEERING_OF_ROAMING_STRU   *pstSteerOfRoamMsg = VOS_NULL_PTR;

    pstSteerOfRoamMsg = (STK_NAS_STEERING_OF_ROAMING_STRU *)pstMsg;

    /* SteeringOfRoaming�����Կ��عرղ��������Ϣ */
    if (VOS_FALSE == NAS_MML_GetStkSteeringOfRoamingSupportFlg())
    {
        return VOS_TRUE;
    }

    if (VOS_TRUE == NAS_MML_IsNvimOplmnAvail())
    {
        return VOS_TRUE;
    }

    /* ��ȡȫ�ֱ����ĵ�ַ */
    pstOperPlmnInfo               = NAS_MML_GetSimOperPlmnList();
    pstOplmnAddr                  = pstOperPlmnInfo->astOperPlmnInfo;
    ucUpdateUsimForbPlmnFlg       = VOS_FALSE;

    usOplmnNum        = (VOS_UINT16)(pstSteerOfRoamMsg->usOplmnListLen / NAS_MML_PLMN_WITH_RAT_UNIT_LEN);

    if ( usOplmnNum > NAS_MML_MAX_OPERPLMN_NUM )
    {
        usOplmnNum = NAS_MML_MAX_OPERPLMN_NUM;
    }

    /* ��USIM�б����PLMN��Ϣת��Ϊ�ڴ��е���Ϣ */
    NAS_MMC_ConverPlmnInfoFromSim(pstSteerOfRoamMsg->aucOplmnList, usOplmnNum, pstOplmnAddr);

    if ( usOplmnNum > pstOperPlmnInfo->usOperPlmnNum )
    {
        /* ����OPLMN�ĸ��� */
        pstOperPlmnInfo->usOperPlmnNum = usOplmnNum;
    }

    for (i = 0; i < usOplmnNum; i++)
    {
        /* STK_NAS_STEERING_OF_ROAMING_IND��Ϣ��Я����OPLMN��ӽ�ֹ������ɾ�� */
        if (VOS_TRUE == NAS_MML_DelForbPlmn(&pstOplmnAddr[i].stPlmnId))
        {
            ucUpdateUsimForbPlmnFlg = VOS_TRUE;
        }

        NAS_MML_DelForbGprsPlmn(&pstOplmnAddr[i].stPlmnId);
    }

    /* ��ֹ�����б����ı䣬��Ҫ֪ͨUSIM */
    if (VOS_TRUE == ucUpdateUsimForbPlmnFlg)
    {
        NAS_MMC_SndUsimUpdateForbPlmnReq();
    }

    /* BG����״̬��������,������������Ϣ���ȼ��Ƚ� */
    if (NAS_MMC_FSM_BG_PLMN_SEARCH == NAS_MMC_GetCurrFsmId())
    {
        return VOS_FALSE;
    }

    /* ��ʼ�������ȼ������б� */
    NAS_MMC_InitPlmnSelectionList(NAS_MMC_PLMN_SEARCH_SCENE_HIGH_PRIO_PLMN_SEARCH,
                                  VOS_NULL_PTR,
                                  NAS_MMC_GetHighPrioPlmnList());

    /* ���HPLMN��ʱ����������,ֹͣ�ö�ʱ��,����1S��ʱ�� */
    if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_HPLMN_TIMER))
    {
        /* ֹͣHPLMN��ʱ��,����10s�����Գ��Ը����ȼ�������ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_HPLMN_TIMER);


        /* HPLMN TIMERֹͣ�¼��ϱ� */
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_HPLMN_TIMER_STOP,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);


        /* ��ǰUTRAN����ģʽ��ΪTDDʱ���������ڱ����� */
        NAS_MMC_StartTimer(TI_NAS_MMC_PERIOD_TRYING_HIGH_PRIO_PLMN_SEARCH, TI_NAS_MMC_STK_STEERING_TRYING_HIGH_PRIO_PLMN_SEARCH_LEN);

        return VOS_TRUE;
    }

    /* ���10s�����Գ��Ը����ȼ�������ʱ����������,��ֹͣ�ö�ʱ��,��������1S��ʱ�� */
    if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_PERIOD_TRYING_HIGH_PRIO_PLMN_SEARCH))
    {
        /* ֹͣ10s�����Գ��Ը����ȼ�������ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_PERIOD_TRYING_HIGH_PRIO_PLMN_SEARCH);
    }

    /* ��ǰUTRAN����ģʽ��ΪTDDʱ���������ڱ����� */
    /* ��ǰû��BG����״̬������ʱ��������,�򵱳�ʱ���д���,������1s��ʱ��ʱ�� */
    NAS_MMC_StartTimer(TI_NAS_MMC_PERIOD_TRYING_HIGH_PRIO_PLMN_SEARCH,
                       TI_NAS_MMC_STK_STEERING_TRYING_HIGH_PRIO_PLMN_SEARCH_LEN);

    return VOS_TRUE;
}




VOS_UINT32  NAS_MMC_RcvRrMmSuspendCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                        *pstMsg
)
{

#if ((FEATURE_ON == FEATURE_GCBS) || (FEATURE_ON == FEATURE_WCBS))

    RRMM_SUSPEND_CNF_ST                *pstSuspendCnf = VOS_NULL_PTR;

    pstSuspendCnf = (RRMM_SUSPEND_CNF_ST*)pstMsg;

    if (MMC_AS_SUSPEND_SUCCESS == pstSuspendCnf->ucResult)
    {
        /* CBS�ϱ�*/
        NAS_MMC_SndMmaMsgNoNetWorkInd();
    }

#endif

    return VOS_FALSE;
}



VOS_UINT32  NAS_MMC_RcvRrMmGetTransactionReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRC_NAS_CS_TRANSACTION_ENUM_UINT8                       enCsTransactionstatus;
    RRMM_TRANSACTION_INFO_ENQ_STRU                         *pstTransactionEnq = VOS_NULL_PTR;

    pstTransactionEnq = (RRMM_TRANSACTION_INFO_ENQ_STRU*)pstMsg;

    /* ��NASά����CS״̬��־����ת��ΪAS��ʽ��CS״̬��־ */
    NAS_MMC_GetMmlCsTransactionStatus(&enCsTransactionstatus);

    /* ֪ͨAS��ǰ��CS��־״̬ */
    NAS_MMC_SndAsTransactionStatus(enCsTransactionstatus, pstTransactionEnq->MsgHeader.ulSenderPid);

    return VOS_TRUE;

}
#if (FEATURE_ON == FEATURE_LTE)

VOS_UINT32  NAS_MMC_ProcDetachCnfSucc_PreProc(
    LMM_MMC_DETACH_CNF_STRU            *pstLmmDetachCnf
)
{
    NAS_MMC_FSM_ID_ENUM_UINT32           enFsmId;

    VOS_UINT32                           ulNeedEnableLte;
    VOS_UINT32                           ulNeedDisableLte;

    enFsmId         = NAS_MMC_GetCurrFsmId();
    if (NAS_FSM_BUTT <= enFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }

    ulNeedEnableLte  = VOS_FALSE;
    ulNeedDisableLte = VOS_FALSE;

    /* ��װ��Ϣ����MMA����DETACH��� */
    switch (pstLmmDetachCnf->ulReqType)
    {
        case MMC_LMM_MO_DET_PS_ONLY:

#if (defined(__PS_WIN32_RECUR__)) || ( VOS_WIN32 == VOS_OS_VER )
            /* ���л��������ֵ���������������Ϊ�˱�֤PC����ͨ�� */
            NAS_MML_SetPsAttachAllowFlg(VOS_FALSE);
#endif

            /* ת��detach�����GMM */
            NAS_MMC_SndGmmLmmMoDetachInd(pstLmmDetachCnf);

            ulNeedDisableLte = NAS_MMC_IsNeedDisableLte_DetachPs();

            break;

        case MMC_LMM_MO_DET_CS_ONLY:

            /* ת��detach�����MM */
            NAS_MMC_SndMmLmmMoDetachInd(pstLmmDetachCnf);

            ulNeedEnableLte = NAS_MMC_IsNeedEnableLte_DetachCs();
            break;

        case MMC_LMM_MO_DET_CS_PS:

#if (defined(__PS_WIN32_RECUR__)) || ( VOS_WIN32 == VOS_OS_VER )
            /* ���л��������ֵ���������������Ϊ�˱�֤PC����ͨ�� */
            NAS_MML_SetPsAttachAllowFlg(VOS_FALSE);
            NAS_MML_SetCsAttachAllowFlg(VOS_FALSE);
#endif

            /* ת��detach�����MM */
            NAS_MMC_SndMmLmmMoDetachInd(pstLmmDetachCnf);

            /* ת��detach�����GMM */
            NAS_MMC_SndGmmLmmMoDetachInd(pstLmmDetachCnf);

            ulNeedDisableLte = NAS_MMC_IsNeedDisableLte_DetachCsPs();
            ulNeedEnableLte = NAS_MMC_IsNeedEnableLte_DetachCsPs();

            break;

        default:

            /* �쳣��ӡ */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcDetachCnfSucc_PreProc:Unexpected detach request type!");

            break;
    }

    if (VOS_TRUE == ulNeedEnableLte)
    {
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }

    /* ��ǰ������disable LTE������ */
    if (VOS_FALSE == ulNeedDisableLte)
    {
        /* ����ֱ�ӷ��ز���Ҫ�������� */
        return VOS_TRUE;
    }

    /* ��¼����disable LTE�ı��,����L��Ϊ��ģ��֪ͨLMM,WAS,GAS */
    NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS);

    NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_PS_DETACH);

    /* ON PLMN��ֱ�ӿ��Դ��� */
    if ((NAS_MMC_L1_STA_ON_PLMN == NAS_MMC_GetFsmTopState())
    && (NAS_MMC_FSM_L1_MAIN == enFsmId))
    {
       /* �����ǰEPS�������Ӵ��ڣ��򻺴湹���������Ϣ */
       if ( VOS_TRUE == NAS_MML_IsRrcConnExist())
       {
           NAS_MMC_SetBufferedPlmnSearchInfo(VOS_TRUE, NAS_MMC_PLMN_SEARCH_SCENE_DISABLE_LTE);

           return VOS_TRUE;
       }

       return VOS_FALSE;
    }

    /* ����L1������״̬������ */
    return VOS_FALSE;
}


VOS_UINT32  NAS_MMC_ProcDetachCnfAuthRej_PreProc(
    LMM_MMC_DETACH_CNF_STRU            *pstLmmDetachCnf
)
{
    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;

    enFsmId = NAS_MMC_GetCurrFsmId();
    if (NAS_FSM_BUTT <= enFsmId)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "ERROR:FsmId Error");
    }
    /* ת��detach�����MM/GMM */
    NAS_MMC_SndMmLmmMoDetachInd(pstLmmDetachCnf);
    NAS_MMC_SndGmmLmmMoDetachInd(pstLmmDetachCnf);

    /* ���������л��ж�CS����Ч��־�������Ҫ���� */
    NAS_MML_SetSimCsRegStatus(VOS_FALSE);

    NAS_MMC_ChangeCsRegState(NAS_MML_REG_REGISTERED_DENIED);

    /* ON PLMN��ֱ�ӿ��Դ��� */
    if ((NAS_MMC_L1_STA_ON_PLMN == NAS_MMC_GetFsmTopState())
    && (NAS_MMC_FSM_L1_MAIN == enFsmId))
    {
       /* �����ǰEPS�������Ӵ��ڣ��򻺴湹���������Ϣ */
       if ( VOS_TRUE == NAS_MML_IsRrcConnExist())
       {
            NAS_MMC_SetPsRegAdditionalAction_L1Main(NAS_MMC_ADDITIONAL_ACTION_CSPS_ANYCELL);

           return VOS_TRUE;
       }

       return VOS_FALSE;
    }

    /* ����L1������״̬������ */
    return VOS_FALSE;
}
VOS_UINT32  NAS_MMC_ProcEpsOnlyDetachCnf_PreProc(
    LMM_MMC_DETACH_CNF_STRU            *pstLmmDetachCnf
)
{

    VOS_UINT32                           ulRet;
    NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8 enPsMmaServiceStatus;

    ulRet                = VOS_TRUE;
    enPsMmaServiceStatus = NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_PS);

    /* ��ȡ��ǰPS����״̬ */
    NAS_MMC_SndMmaDetachCnf(MMA_MMC_SRVDOMAIN_PS,
                            enPsMmaServiceStatus,
                            pstLmmDetachCnf->ulOpId);

    /* detach������� */
    switch (pstLmmDetachCnf->ulDetachRslt)
    {
        case MMC_LMM_DETACH_RSLT_SUCCESS:

            /* ����detach�ɹ����д��� */
            ulRet = NAS_MMC_ProcDetachCnfSucc_PreProc(pstLmmDetachCnf);

            break;

        case MMC_LMM_DETACH_RSLT_ACCESS_BARED:
        case MMC_LMM_DETACH_RSLT_FAILURE:

            /* �����κδ��� */
            break;

        case MMC_LMM_DETACH_RSLT_AUTH_REJ:

            /* ��Ȩ���ܵĴ��� */
            ulRet = NAS_MMC_ProcDetachCnfAuthRej_PreProc(pstLmmDetachCnf);

            break;

        default:

            /* �쳣��ӡ */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcEpsOnlyDetachCnf_PreProc:Unexpected detach result!");
            break;
    }

    return ulRet;
}


VOS_UINT32  NAS_MMC_ProcCsOnlyDetachCnf_PreProc(
    LMM_MMC_DETACH_CNF_STRU            *pstLmmDetachCnf
)
{

    VOS_UINT32                           ulRet;
    NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8 enCsMmaServiceStatus;


    ulRet   = VOS_TRUE;

    /* ��ȡ��ǰCS����״̬ */
    enCsMmaServiceStatus = NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_CS);

    NAS_MMC_SndMmaDetachCnf(MMA_MMC_SRVDOMAIN_CS,
                            enCsMmaServiceStatus,
                            pstLmmDetachCnf->ulOpId);


    /* detach������� */
    switch (pstLmmDetachCnf->ulDetachRslt)
    {
        case MMC_LMM_DETACH_RSLT_SUCCESS:

            /* ����detach�ɹ����д��� */
            ulRet = NAS_MMC_ProcDetachCnfSucc_PreProc(pstLmmDetachCnf);

            break;

        case MMC_LMM_DETACH_RSLT_ACCESS_BARED:
        case MMC_LMM_DETACH_RSLT_FAILURE:

            /* �����κδ��� */
            break;

        case MMC_LMM_DETACH_RSLT_AUTH_REJ:

            /* ��Ȩ���ܵĴ��� */
            ulRet = NAS_MMC_ProcDetachCnfAuthRej_PreProc(pstLmmDetachCnf);

            break;

        default:

            /* �쳣��ӡ */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcCsOnlyDetachCnf_PreProc:Unexpected detach result!");
            break;
    }

    return ulRet;
}


VOS_UINT32  NAS_MMC_ProcCsPsDetachCnf_PreProc(
    LMM_MMC_DETACH_CNF_STRU            *pstLmmDetachCnf
)
{

    VOS_UINT32                           ulRet;
    NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8 enCsMmaServiceStatus;
    NAS_MM_COM_SERVICE_STATUS_ENUM_UINT8 enPsMmaServiceStatus;

    ulRet                = VOS_TRUE;

    /* ��ȡ��ǰ�ϱ��õ�CS_PS����״̬ */
    enCsMmaServiceStatus = NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_CS);
    enPsMmaServiceStatus = NAS_MMC_GetServiceStatusForMma(MMA_MMC_SRVDOMAIN_PS);

    NAS_MMC_SndMmaDetachCnf(MMA_MMC_SRVDOMAIN_CS,
                            enCsMmaServiceStatus,
                            pstLmmDetachCnf->ulOpId);

    NAS_MMC_SndMmaDetachCnf(MMA_MMC_SRVDOMAIN_PS,
                            enPsMmaServiceStatus,
                            pstLmmDetachCnf->ulOpId);

    /* detach������� */
    switch (pstLmmDetachCnf->ulDetachRslt)
    {
        case MMC_LMM_DETACH_RSLT_SUCCESS:

            /* ����detach�ɹ����д��� */
            ulRet = NAS_MMC_ProcDetachCnfSucc_PreProc(pstLmmDetachCnf);

            break;

        case MMC_LMM_DETACH_RSLT_ACCESS_BARED:
        case MMC_LMM_DETACH_RSLT_FAILURE:

            /* �����κδ��� */
            break;

        case MMC_LMM_DETACH_RSLT_AUTH_REJ:

            /* ��Ȩ���ܵĴ��� */
            ulRet = NAS_MMC_ProcDetachCnfAuthRej_PreProc(pstLmmDetachCnf);

            break;

        default:

            /* �쳣��ӡ */
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcCsPsDetachCnf_PreProc:Unexpected detach result!");
            break;
    }

    return ulRet;
}


VOS_UINT32  NAS_MMC_RcvLmmServiceRsltInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                                              ulRet;
    LMM_MMC_SERVICE_RESULT_IND_STRU                        *pstSerRsltMsg = VOS_NULL_PTR;
    NAS_MML_REG_FAIL_CAUSE_ENUM_UINT16                      enCause;

    enCause       = NAS_MML_REG_FAIL_CAUSE_BUTT;

    pstSerRsltMsg = (LMM_MMC_SERVICE_RESULT_IND_STRU *)pstMsg;

    NAS_MMC_ConverLmmServiceRsltToMMLCause(pstSerRsltMsg,&enCause);

    NAS_MMC_SndMmaServRejRsltInd(MMA_MMC_SRVDOMAIN_PS, (VOS_UINT16)enCause);

    /* �жϵ�ǰ�Ƿ���Ҫ��ϵ�ǰ״̬�� �������Ϣ�Ļ��� */
    ulRet = NAS_MMC_ProcMsgPrio(ulEventType, pstMsg);

    /* ��Ҫ��ϵ�ǰ״̬���������Ϣ�Ļ���,���ʾԤ������� */
    if ( VOS_TRUE == ulRet )
    {
        return VOS_TRUE;
    }

    /* ��Ҫ�������״̬������ */
    return VOS_FALSE;
}



VOS_UINT32  NAS_MMC_RcvLmmEmmInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MM_INFO_IND_STRU               *pstSendMsg    = VOS_NULL_PTR;
    LMM_MMC_EMM_INFO_IND_STRU          *pstLmmEmmInfo = VOS_NULL_PTR;
    NAS_MML_OPERATOR_NAME_INFO_STRU    *pstCurrOperatorName = VOS_NULL_PTR;
    VOS_UINT8                           ucNvWriteFlg;
    NAS_MML_OPERATOR_NAME_INFO_STRU     stOldOperatorName;

    pstCurrOperatorName = NAS_MML_GetOperatorNameInfo();
    ucNvWriteFlg        = VOS_FALSE;
    PS_MEM_CPY(&stOldOperatorName, pstCurrOperatorName, sizeof(NAS_MML_OPERATOR_NAME_INFO_STRU));
    pstLmmEmmInfo = (LMM_MMC_EMM_INFO_IND_STRU *)pstMsg;

    if ((VOS_FALSE == pstLmmEmmInfo->stLteEmmInfo.bitOpDaylightTime)
     &&(VOS_FALSE == pstLmmEmmInfo->stLteEmmInfo.bitOpLocTimeZone)
     &&(VOS_FALSE == pstLmmEmmInfo->stLteEmmInfo.bitOpUniTimeLocTimeZone)
     &&(VOS_FALSE == pstLmmEmmInfo->stLteEmmInfo.bitOpLongName)
     &&(VOS_FALSE == pstLmmEmmInfo->stLteEmmInfo.bitOpShortName))
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmEmmInfoInd_PreProc():No userful info");
        return VOS_TRUE;
    }

    /* �����ڴ� */
    pstSendMsg = (NAS_MM_INFO_IND_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                                sizeof(NAS_MM_INFO_IND_STRU));

    if( VOS_NULL_PTR == pstSendMsg )
    {
       NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmEmmInfoInd_PreProc():ERROR:Memory Alloc Error.");
       return VOS_TRUE;
    }

    PS_MEM_SET(pstSendMsg, 0, sizeof(NAS_MM_INFO_IND_STRU));

    if (VOS_TRUE == pstLmmEmmInfo->stLteEmmInfo.bitOpLocTimeZone)
    {
        pstSendMsg->ucIeFlg |= NAS_MM_INFO_IE_LTZ;
        pstSendMsg->cLocalTimeZone = pstLmmEmmInfo->stLteEmmInfo.cLocTimeZone;
    }

    if (VOS_TRUE == pstLmmEmmInfo->stLteEmmInfo.bitOpUniTimeLocTimeZone)
    {
        pstSendMsg->ucIeFlg |= NAS_MM_INFO_IE_UTLTZ;
        pstSendMsg->stUniversalTimeandLocalTimeZone.ucYear    = pstLmmEmmInfo->stLteEmmInfo.stTimeZoneAndTime.ucYear;
        pstSendMsg->stUniversalTimeandLocalTimeZone.ucMonth   = pstLmmEmmInfo->stLteEmmInfo.stTimeZoneAndTime.ucMonth;
        pstSendMsg->stUniversalTimeandLocalTimeZone.ucDay     = pstLmmEmmInfo->stLteEmmInfo.stTimeZoneAndTime.ucDay;
        pstSendMsg->stUniversalTimeandLocalTimeZone.ucHour    = pstLmmEmmInfo->stLteEmmInfo.stTimeZoneAndTime.ucHour;
        pstSendMsg->stUniversalTimeandLocalTimeZone.ucMinute  = pstLmmEmmInfo->stLteEmmInfo.stTimeZoneAndTime.ucMinute;
        pstSendMsg->stUniversalTimeandLocalTimeZone.ucSecond  = pstLmmEmmInfo->stLteEmmInfo.stTimeZoneAndTime.ucSecond;
        pstSendMsg->stUniversalTimeandLocalTimeZone.cTimeZone = pstLmmEmmInfo->stLteEmmInfo.stTimeZoneAndTime.cTimeZone;
    }

    if (VOS_TRUE == pstLmmEmmInfo->stLteEmmInfo.bitOpDaylightTime)
    {
        pstSendMsg->ucIeFlg |= NAS_MM_INFO_IE_DST;
        pstSendMsg->ucDST = pstLmmEmmInfo->stLteEmmInfo.enDaylightSavingTime;
    }

    if (VOS_TRUE == pstLmmEmmInfo->stLteEmmInfo.bitOpLongName)
    {
        PS_MEM_CPY(pstCurrOperatorName->aucOperatorNameLong, pstLmmEmmInfo->stLteEmmInfo.aucOperatorNameLong, NAS_MML_MAX_OPER_LONG_NAME_LEN);

        ucNvWriteFlg = VOS_TRUE;
    }

    if (VOS_TRUE == pstLmmEmmInfo->stLteEmmInfo.bitOpShortName)
    {
        PS_MEM_CPY(pstCurrOperatorName->aucOperatorNameShort, pstLmmEmmInfo->stLteEmmInfo.aucOperatorNameShort, NAS_MML_MAX_OPER_SHORT_NAME_LEN);

        ucNvWriteFlg = VOS_TRUE;
    }

    if (VOS_TRUE == ucNvWriteFlg)
    {
        /* �����Ϣ���������������֮ǰ��������ݲ�ͬ,��ҪдNV */
        if (0 != VOS_MemCmp(pstCurrOperatorName, &stOldOperatorName, sizeof(NAS_MML_OPERATOR_NAME_INFO_STRU)))
        {
            if (NV_OK != NV_Write(en_NV_Item_Network_Name_From_MM_Info,
                                  pstCurrOperatorName,
                                  sizeof(NAS_MML_OPERATOR_NAME_INFO_STRU)))
            {
                NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvLmmEmmInfoInd_PreProc:Write NV fail.");
            }
        }
    }
    if (NAS_MM_INFO_IE_NULL ==pstSendMsg->ucIeFlg)
    {
        PS_MEM_FREE(WUEPS_PID_MMC, pstSendMsg);
        return VOS_TRUE;
    }

    NAS_MMC_SndMmaMmInfo(pstSendMsg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstSendMsg);

    return VOS_TRUE;
}
VOS_UINT32  NAS_MMC_RcvLmmEmcPdpStatusNotify_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_EMC_PDP_STATUS_NOTIFY_STRU *pstEmcPdpStatus = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU          *pstEquPlmnInfo = VOS_NULL_PTR;
    NAS_MML_EQUPLMN_INFO_STRU           stSndEquPlmnInfo;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enCurrNetRat;

    enCurrNetRat    = NAS_MML_GetCurrNetRatType();
    pstEmcPdpStatus = (LMM_MMC_EMC_PDP_STATUS_NOTIFY_STRU *)pstMsg;

    /* �������PDP���Ӽ��� */
    if (VOS_TRUE == pstEmcPdpStatus->ucIsEmcPdpActive)
    {
        NAS_MML_SetEmcPdpStatusFlg(VOS_TRUE);
    }
    else
    {

        /* �������PDN���ӱ�־ */
        NAS_MML_SetEmcPdpStatusFlg(VOS_FALSE);

        /* �����ڲ��ҵ�ǰ��ģ��Lģʱ��LMM����EPLMN֪ͨ��ɾ��Forb Plmn */
        if ( (VOS_TRUE == NAS_MML_GetSimPresentStatus())
          && (NAS_MML_NET_RAT_TYPE_LTE == enCurrNetRat) )
        {
            pstEquPlmnInfo = NAS_MML_GetEquPlmnList();
            PS_MEM_CPY(&stSndEquPlmnInfo, pstEquPlmnInfo, sizeof(NAS_MML_EQUPLMN_INFO_STRU));
            NAS_MMC_BuildSndLmmEquPlmnInfo(&stSndEquPlmnInfo);
            NAS_MMC_SndLmmEquPlmnReq(&stSndEquPlmnInfo);

            NAS_MMC_SndOmEquPlmn();
        }
    }

    return VOS_TRUE;
}


#endif
VOS_UINT32  NAS_MMC_RcvGmmLocalDetachInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
#if (FEATURE_ON == FEATURE_LTE)
    MMCGMM_LOCAL_DETACH_IND_STRU                           *pstLocalDetachInd;
    MMC_LMM_GU_LOCAL_DETACH_TYPE_ENUM_UINT32                ulLocalDetachType;


    pstLocalDetachInd = (MMCGMM_LOCAL_DETACH_IND_STRU *)pstMsg;

    if (NAS_MML_REG_FAIL_CAUSE_NTDTH_IMSI == pstLocalDetachInd->ulDetachCause)
    {
        ulLocalDetachType = MMC_LMM_GU_LOCAL_DET_IMSI_PAGING;

        /* ֪ͨL GU����imsi paging detach */
        NAS_MMC_SndLmmLocalDetachIndActionResultReq(ulLocalDetachType);
    }
#endif

    NAS_MMC_ChangePsRegState(NAS_MML_REG_NOT_REGISTERED_NOT_SEARCH);

    return VOS_TRUE;
}
VOS_UINT32  NAS_MMC_RcvRrMmRelInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                        *pstMsg
)
{
    RRMM_REL_IND_STRU                  *pstRrMmRelInd = VOS_NULL_PTR;

    pstRrMmRelInd = (RRMM_REL_IND_STRU*)pstMsg;

    if (RRC_RRC_CONN_STATUS_ABSENT == pstRrMmRelInd->ulRrcConnStatus)
    {
        /* ���RRC����״̬ */
        NAS_MML_SetRrcConnStatusFlg(VOS_FALSE);
    }

    return VOS_FALSE;
}
VOS_UINT32  NAS_MMC_RcvRrMmCipherInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_CIPHER_INFO_IND_STRU          *pstCipherInfoInd;

    /* ��ʼ����Ϣ */
    pstCipherInfoInd = (RRMM_CIPHER_INFO_IND_STRU*)pstMsg;

    /* ֱ�ӽ��յ���WAS��Cipher��Ϣ�ϱ���MMA */
    NAS_MMC_SndMmaWCipherInfoInd(pstCipherInfoInd);

    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvGmmCipherInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_GMM_CIPHER_INFO_IND_STRU       *pstCipherInfoInd;

    /* ��ʼ����Ϣ */
    pstCipherInfoInd = (MMC_GMM_CIPHER_INFO_IND_STRU*)pstMsg;

    /* ֱ�ӽ��յ���WAS��Cipher��Ϣ�ϱ���MMA */
    NAS_MMC_SndMmaGCipherInfoInd(pstCipherInfoInd);

    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvGmmPsRegResultInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
#if (FEATURE_ON == FEATURE_LTE)
    VOS_UINT32                                              ulDisablteRoamFlg;

    NAS_MML_LTE_UE_OPERATION_MODE_ENUM_UINT8                enLUeMode;

    enLUeMode            = NAS_MML_GetLteUeOperationMode();

    /* ����PS detachҲ�ᷢ�͸���Ϣ����������PS detach������Ҫ����Enable�Ĵ��� */
    if (VOS_FALSE == NAS_MML_GetPsAttachAllowFlg())
    {
        return VOS_FALSE;
    }

    if ((VOS_TRUE == NAS_MMC_IsNeedEnableLte_AttachPs())
     && (NAS_MML_LTE_UE_OPERATION_MODE_CS_PS_1 != enLUeMode))
    {
        ulDisablteRoamFlg = NAS_MML_GetDisableLteRoamFlg();

        /* ������ڽ�ֹLTE�������ε��µ�disable LTE����δ�ص�����֮ǰ��ʱҲ��֪ͨenable LTE */
        if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == NAS_MML_GetLteCapabilityStatus())
         && (VOS_FALSE == ulDisablteRoamFlg))
        {
            /* ��WAS/GAS����enable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);

            /* ��LMM����enable LTE��Ϣ */
            NAS_MMC_SndLmmEnableLteNotify();
        }

       /* ����disable LTE������� */
       NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }
#endif

    return VOS_FALSE;
}
VOS_UINT32  NAS_MMC_RcvTafOmMaintainInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMA_MMC_OM_MAINTAIN_INFO_IND_STRU   *pstOmMaintainInfoInd;

    pstOmMaintainInfoInd = (MMA_MMC_OM_MAINTAIN_INFO_IND_STRU*)pstMsg;

    /* ����SDT���ӱ�־ */
    NAS_MML_SetOmConnectFlg(pstOmMaintainInfoInd->ucOmConnectFlg);

    /* ����PC�ط�ʹ�ܱ�־ */
    NAS_MML_SetOmPcRecurEnableFlg(pstOmMaintainInfoInd->ucOmPcRecurEnableFlg);

    /* �������״̬������ */
    return VOS_TRUE;
}
VOS_UINT32  NAS_MMC_ProcUserReselReqUnNormalSrv_PreProc(
    MMA_MMC_PLMN_USER_RESEL_REQ_STRU      *pUserReselReq
)
{
    NAS_MMC_PLMN_SEARCH_SCENE_ENUM_UINT32                   enPlmnSearchScene;
    VOS_UINT32                                              ulCsServiceExist;
    VOS_UINT32                                              ulPsServiceExist;

    VOS_UINT32                                              ulIsNeedPlmnSearch;

    NAS_MML_BG_SEARCH_CFG_INFO_STRU                        *pstBgSearchCfg = VOS_NULL_PTR;

    pstBgSearchCfg = NAS_MML_GetBgSearchCfg();

    /* �жϵ�ǰ�Ƿ���CSҵ����� */
    ulCsServiceExist         = NAS_MML_GetCsServiceExistFlg();

    /* �жϵ�ǰ�Ƿ���PSҵ����� */
    ulPsServiceExist         = NAS_MML_GetPsServiceExistFlg();

    /* �ֶ����Զ�ģʽ��Ҫ���״�����hplmn��ʱ����־��Ϊtrue */
    if ( NAS_MMC_PLMN_SELECTION_MODE_MANUAL == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ���綨�ƣ���ǿ��Hplmn�������Դ�ʱ���Զ�ģʽ�޸�Ϊ�ֶ�ģʽ��
           ֹͣHPLMN��ʱ�����û��ٴν��ֶ�ģʽ�޸�Ϊ�Զ�ģʽ������HPLMN������������
           ����HPLMN��ʱ������Ϊ�����״�����HPLMN��ʱ��*/
        if (VOS_TRUE == pstBgSearchCfg->ucEnhancedHplmnSrchFlg)
        {
            NAS_MMC_SetFirstStartHPlmnTimerFlg(VOS_FALSE);
        }
        else
        {
            NAS_MMC_SetFirstStartHPlmnTimerFlg(VOS_TRUE);
        }
    }

    /* ��ҵ��ʱ������Ҫ�������� */
    if ( (VOS_FALSE == ulCsServiceExist)
      && (VOS_FALSE == ulPsServiceExist))
    {
        /* ��״̬���ﴥ������ */
        /* ��ǰΪ�Զ�ѡ��ģʽ��������������MMA�ظ������ɹ� */
        ulIsNeedPlmnSearch = NAS_MMC_IsNeedPlmnSearch_UserReselReq();

        if ((NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
         && (VOS_FALSE == ulIsNeedPlmnSearch))
        {
            NAS_MMC_SndMmaPlmnReselCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_SUCCESS);
            return VOS_TRUE;
        }

        return VOS_FALSE;
    }

    /* ��ǰΪ�Զ�ѡ��ģʽ */
    if ( NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        /* ��¼�������� */

        if (VOS_TRUE == NAS_MML_GetUserAutoReselActiveFlg())
        {
            enPlmnSearchScene = NAS_MMC_PLMN_SEARCH_SCENE_USER_AUTO_RESEL;
        }
        else
        {
            enPlmnSearchScene = NAS_MMC_PLMN_SEARCH_SCENE_AVAILABLE_TIMER_EXPIRED;
        }
    }
    else
    {
        /* ����ѡ��ģʽ */
        NAS_MMC_SetPlmnSelectionMode(NAS_MMC_PLMN_SELECTION_MODE_AUTO);

        /* ����ǰ��ѡģʽд��NVIM�� */
        NAS_MMC_WritePlmnSelectionModeNvim();

        /* ģʽ�ı����STK����¼� */
        if (VOS_TRUE == NAS_MMC_IsNeedSndStkSearchModeChgEvt())
        {
            NAS_MMC_SndStkSearchModeChgEvt(NAS_MMC_PLMN_SELECTION_MODE_AUTO);
        }

        /* ��¼�������� */

        if (VOS_TRUE == NAS_MML_GetUserAutoReselActiveFlg())
        {
            enPlmnSearchScene = NAS_MMC_PLMN_SEARCH_SCENE_USER_AUTO_RESEL;
        }
        else
        {
            enPlmnSearchScene = NAS_MMC_PLMN_SEARCH_SCENE_MANUAL_CHANGE_AUTO_MODE;
        }

    }


    /* ����������Ϣ */
    NAS_MMC_SetBufferedPlmnSearchInfo(VOS_TRUE, enPlmnSearchScene);

    /* ��MMA�ظ������ɹ� */
    NAS_MMC_SndMmaPlmnReselCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_SUCCESS);


    /* ��ҵ��ʱ�������������ٽ���״̬������ */
    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_ProcUserReselReqNormalServiceFun(VOS_VOID)
{
    NAS_MML_PLMN_ID_STRU               *pstCurrCampPlmnId = VOS_NULL_PTR;
    VOS_UINT32                          ulCampOnHplmnFlag;
    VOS_UINT32                          ulCsServiceExist;

    /* ȡ�õ�ǰפ����PLMN ID */
    pstCurrCampPlmnId = NAS_MML_GetCurrCampPlmnId();
    
    /* ��HPLMN��ͬ��������פ��HPLMN�� */
    ulCampOnHplmnFlag = NAS_MML_ComparePlmnIdWithHplmn(pstCurrCampPlmnId);

    /* �жϵ�ǰ�Ƿ���CSҵ����� */
    ulCsServiceExist         = NAS_MML_GetCsServiceExistFlg();

    if ((VOS_FALSE == NAS_MML_GetUserAutoReselActiveFlg())
     || (VOS_TRUE  == ulCampOnHplmnFlag))
    {
        NAS_MMC_SndMmaPlmnReselCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_SUCCESS);

        return VOS_TRUE;
    }
    else
    {   
        if (VOS_TRUE == ulCsServiceExist)
        {
            /* ������Ϣ���ظ��ɹ� */
            NAS_MMC_SetBufferedPlmnSearchInfo(VOS_TRUE, 
                        NAS_MMC_PLMN_SEARCH_SCENE_USER_AUTO_RESEL);
        
            NAS_MMC_SndMmaPlmnReselCnf(MMA_MMC_USER_PLMN_SEARCH_RESULT_SUCCESS);
        
            return VOS_TRUE;
        }
        else
        {
            /* ��״̬���ﴥ������ */
            return VOS_FALSE;
        }
    }
}
VOS_UINT32  NAS_MMC_ProcUserReselReqNormalService_PreProc(
    MMA_MMC_PLMN_USER_RESEL_REQ_STRU      *pUserReselReq
)
{
    NAS_MML_BG_SEARCH_CFG_INFO_STRU    *pstBgSearchCfg      = VOS_NULL_PTR;

    pstBgSearchCfg = NAS_MML_GetBgSearchCfg();



    /* ��ǰ�Ѿ����Զ�ѡ��ģʽ */
    if (NAS_MMC_PLMN_SELECTION_MODE_AUTO == NAS_MMC_GetPlmnSelectionMode())
    {
        return NAS_MMC_ProcUserReselReqNormalServiceFun();
    }

    /* ����ѡ��ģʽ */
    NAS_MMC_SetPlmnSelectionMode(NAS_MMC_PLMN_SELECTION_MODE_AUTO);

    /* ����ǰ��ѡģʽд��NVIM�� */
    NAS_MMC_WritePlmnSelectionModeNvim();

    /* ����ģʽ�仯�ϱ�USIM */
    if (VOS_TRUE == NAS_MMC_IsNeedSndStkSearchModeChgEvt())
    {
        NAS_MMC_SndStkSearchModeChgEvt(NAS_MMC_PLMN_SELECTION_MODE_AUTO);
    }

    /* �ֶ�ģʽ�޸�Ϊ�Զ�ģʽ����ǰ���������������£���Ҫ�ж��Ƿ���Ҫ����HPLMN��ʱ�� */
    if (VOS_TRUE == NAS_MMC_IsNeedStartHPlmnTimer())
    {
        /* ���綨�ƣ���ǿ��Hplmn�������Դ�ʱ���Զ�ģʽ�޸�Ϊ�ֶ�ģʽ��
           ֹͣHPLMN��ʱ�����û��ٴν��ֶ�ģʽ�޸�Ϊ�Զ�ģʽ������HPLMN������������
           ����HPLMN��ʱ������Ϊ�����״�����HPLMN��ʱ��*/
        if (VOS_TRUE == pstBgSearchCfg->ucEnhancedHplmnSrchFlg)
        {
            NAS_MMC_SetFirstStartHPlmnTimerFlg(VOS_FALSE);
        }
        else
        {
            NAS_MMC_SetFirstStartHPlmnTimerFlg(VOS_TRUE);
        }

        /* ������ʱ�� */
        NAS_MMC_StartTimer(TI_NAS_MMC_HPLMN_TIMER, NAS_MMC_GetHplmnTimerLen());


        /* HPLMN TIMER�����¼��ϱ� */
        NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_HPLMN_TIMER_START,
                        VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

        NAS_MMC_SetFirstStartHPlmnTimerFlg(VOS_FALSE);

    }

    if (VOS_TRUE == NAS_MMC_IsNeedStartHighPrioRatHPlmnTimer())
    {
        /* ������ʱ�� */
        if (VOS_TRUE == NAS_MMC_StartTimer(TI_NAS_MMC_HIGH_PRIO_RAT_HPLMN_TIMER, NAS_MMC_GetHighPrioRatHplmnTimerLen()))
        {
            NAS_MMC_AddCurHighPrioRatHplmnTimerFirstSearchCount_L1Main();
        }
    }


    return NAS_MMC_ProcUserReselReqNormalServiceFun();

}



VOS_VOID NAS_MMC_ProcScanCtrl_PreProc(
    VOS_UINT16                          usEfLen,
    VOS_UINT8                          *pucEf
)
{
    VOS_UINT32                          ulHplmnTimerLen;

    /* �����������NVδ�򿪣�ֱ�ӷ��� */
    if (VOS_FALSE == NAS_MML_GetScanCtrlEnableFlg())
    {
        NAS_NORMAL_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcScanCtrl_PreProc:ERROR: Scan Ctrl NV is deactive.");
        return;
    }

    /* �ļ����Ȳ���ȷ��ֱ�ӷ��� */
    if (NAS_MML_HPLMN_PERI_FILE_LEN != usEfLen)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_ProcScanCtrl_PreProc:ERROR: File length is invalid.");
        return;
    }


    /* ʱ������Ԥ������ˢ�£�ֱ�ӻ�ȡ */
    ulHplmnTimerLen = NAS_MMC_GetHplmnTimerLen();


    /* �����ǰ�����ȼ�������ʱ���������У�����ֹͣ */
    if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_HPLMN_TIMER))
    {
        /* ֹͣ��ʱ�� */
        NAS_MMC_StopTimer(TI_NAS_MMC_HPLMN_TIMER);

        /* ��ǰUTRAN����ģʽ��TDD,��ʱ����0���������� */
        if (0 != ulHplmnTimerLen)
        {
            NAS_MMC_StartTimer(TI_NAS_MMC_HPLMN_TIMER, ulHplmnTimerLen);
        }
    }
    else
    {
        /* ������״̬���������У�ֱ�ӷ��� */
        if (NAS_MMC_FSM_BG_PLMN_SEARCH == NAS_MMC_GetCurrFsmId())
        {
            return;
        }

        /* �����Գ��Ը����ȼ������Ķ�ʱ���������У�ˢ�º��ʱ��Ϊ0����Ҫֹͣ */
        if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_PERIOD_TRYING_HIGH_PRIO_PLMN_SEARCH))
        {
            if (0 == ulHplmnTimerLen)
            {
                NAS_MMC_StopTimer(TI_NAS_MMC_PERIOD_TRYING_HIGH_PRIO_PLMN_SEARCH);
            }

            return;
        }

        /* ����NAS_MMC_IsNeedStartHPlmnTimer�ж��Ƿ���Ҫ������ʱ�� */
        if (VOS_TRUE == NAS_MMC_IsNeedStartHPlmnTimer())
        {
            /* ������ʱ�� */
            NAS_MMC_StartTimer(TI_NAS_MMC_HPLMN_TIMER, NAS_MMC_GetHplmnTimerLen());

            /* HPLMN TIMER�����¼��ϱ� */
            NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_HPLMN_TIMER_START,
                            VOS_NULL_PTR, NAS_OM_EVENT_NO_PARA);

            /* ��HPLMN������������Ϊ���״����� */
            NAS_MMC_SetFirstStartHPlmnTimerFlg(VOS_FALSE);
        }

        if (VOS_TRUE == NAS_MMC_IsNeedStartHighPrioRatHPlmnTimer())
        {
            /* ������ʱ�� */
            if (VOS_TRUE == NAS_MMC_StartTimer(TI_NAS_MMC_HIGH_PRIO_RAT_HPLMN_TIMER, NAS_MMC_GetHighPrioRatHplmnTimerLen()))
            {
                NAS_MMC_AddCurHighPrioRatHplmnTimerFirstSearchCount_L1Main();
            }
        }
    }

    return;
}

#if   (FEATURE_ON == FEATURE_LTE)

VOS_UINT32  NAS_MMC_RcvCmServiceRejectInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                        *pstMsg
)
{
    MMCMM_CM_SERVICE_REJECT_IND_STRU   *pstRcvMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulDisablteRoamFlg;

    ulDisablteRoamFlg = NAS_MML_GetDisableLteRoamFlg();

    pstRcvMsg = (MMCMM_CM_SERVICE_REJECT_IND_STRU*)pstMsg;

    NAS_MMC_SndMmaServRejRsltInd(MMA_MMC_SRVDOMAIN_CS,
                                (VOS_UINT16) pstRcvMsg->ulCause);

    if ( (NAS_MML_REG_FAIL_CAUSE_ILLEGAL_ME != pstRcvMsg->ulCause)
      && (NAS_MML_REG_FAIL_CAUSE_IMSI_UNKNOWN_IN_VLR != pstRcvMsg->ulCause) )
    {
        return VOS_TRUE;
    }


    if (VOS_TRUE == NAS_MMC_IsNeedEnableLte_CmServiceRejectInd((VOS_UINT16)pstRcvMsg->ulCause))
    {
        /* ������ڽ�ֹLTE�������ε��µ�disable LTE����δ�ص�����֮ǰ��ʱҲ��֪ͨenable LTE */
        if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == NAS_MML_GetLteCapabilityStatus())
         && (VOS_FALSE == ulDisablteRoamFlg))
        {
            /* ��WAS/GAS����enable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);

            /* ��LMM����enable LTE��Ϣ */
            NAS_MMC_SndLmmEnableLteNotify();
        }

        if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER))
        {
            NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER);
        }

        /* ����disable LTE������� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }



    return VOS_FALSE;
}


VOS_UINT32  NAS_MMC_RcvMmAbortInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                        *pstMsg
)
{
    MMMMC_ABORT_IND_STRU   *pstRcvMsg = VOS_NULL_PTR;
    VOS_UINT32              ulDisablteRoamFlg;

    ulDisablteRoamFlg = NAS_MML_GetDisableLteRoamFlg();

    pstRcvMsg = (MMMMC_ABORT_IND_STRU*)pstMsg;

    if (VOS_TRUE == NAS_MMC_IsNeedEnableLte_MmAbortInd(pstRcvMsg->enAbortCause))
    {
        /* ������ڽ�ֹLTE�������ε��µ�disable LTE����δ�ص�����֮ǰ��ʱҲ��֪ͨenable LTE */
        if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == NAS_MML_GetLteCapabilityStatus())
         && (VOS_FALSE == ulDisablteRoamFlg))
        {
            /* ��WAS/GAS����enable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);

            /* ��LMM����enable LTE��Ϣ */
            NAS_MMC_SndLmmEnableLteNotify();
        }

        if (NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER))
        {
            NAS_MMC_StopTimer(TI_NAS_MMC_WAIT_ENABLE_LTE_TIMER);
        }

        /* ����disable LTE������� */
        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
    }

    return VOS_FALSE;
}


VOS_UINT32  NAS_MMC_RcvEnableLteExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                        *pstMsg
)
{
    VOS_UINT32              ulDisablteRoamFlg;

    ulDisablteRoamFlg = NAS_MML_GetDisableLteRoamFlg();


    if (VOS_TRUE == NAS_MMC_IsNeedEnableLte_EnableLteTimerExp())
    {
        /* ������ڽ�ֹLTE�������ε��µ�disable LTE����δ�ص�����֮ǰ��ʱҲ��֪ͨenable LTE */
        if ((NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS == NAS_MML_GetLteCapabilityStatus())
         && (VOS_FALSE == ulDisablteRoamFlg))
        {
            /* ��WAS/GAS����enable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_REENABLE);

            /* ��LMM����enable LTE��Ϣ */
            NAS_MMC_SndLmmEnableLteNotify();

            /* ����disable LTE������� */
            NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_REENABLE_NOTIFIED_AS);
            NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_BUTT);
        }
    }

    return VOS_FALSE;
}


#endif



VOS_UINT32  NAS_MMC_IsNeedPlmnSearchGUSearchState_UserReselReq(
    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId,
    VOS_UINT32                          ulUserAutoReselActiveFlg,
    VOS_UINT32                          ulIsCurrSearchingPlmnHplmn,
    VOS_UINT32                          ulCurrState
)
{
    /* ����״̬�������У���GU�����͹���״̬ʱ��Ҫ�жϵ�ǰ���������������Ƿ�Ϊhplmn��nv 9705��ʱ�����������hplmn�������� */
    if ( (NAS_MMC_FSM_PLMN_SELECTION == enFsmId)
      && (VOS_TRUE == ulUserAutoReselActiveFlg)
      && (VOS_TRUE == ulIsCurrSearchingPlmnHplmn)
      && ((NAS_MMC_PLMN_SELECTION_STA_WAIT_WAS_PLMN_SEARCH_CNF  ==  ulCurrState )
       || (NAS_MMC_PLMN_SELECTION_STA_WAIT_GAS_PLMN_SEARCH_CNF  ==  ulCurrState )
       || (NAS_MMC_PLMN_SELECTION_STA_WAIT_WAS_SUSPEND_CNF      ==  ulCurrState )
       || (NAS_MMC_PLMN_SELECTION_STA_WAIT_GAS_SUSPEND_CNF      ==  ulCurrState )))
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_IsNeedPlmnSearch_UserReselReq(VOS_VOID)
{
    VOS_UINT32                                              ulCurrState;
    NAS_MML_PLMN_WITH_RAT_STRU                             *pstCurrSearchingPlmn = VOS_NULL_PTR;
    VOS_UINT32                                              ulIsCurrSearchingPlmnHplmn;
    VOS_UINT32                                              ulUserAutoReselActiveFlg;
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;
    VOS_UINT32                                              ulIsUsimStatusValid;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MML_PLMN_ID_STRU                                   *pstCsPsMode1ReCampLtePlmn;
    VOS_UINT32                                              ulIsReCampLtePlmnHplmn;
#endif

    enFsmId                    = NAS_MMC_GetCurrFsmId();
    ulCurrState                = NAS_MMC_GetFsmTopState();
    ulUserAutoReselActiveFlg   = NAS_MML_GetUserAutoReselActiveFlg();
    pstCurrSearchingPlmn       = NAS_MMC_GetCurrSearchingPlmn_PlmnSelection();
    ulIsCurrSearchingPlmnHplmn = NAS_MML_ComparePlmnIdWithHplmn(&pstCurrSearchingPlmn->stPlmnId);
    ulIsUsimStatusValid        = NAS_MML_IsUsimStausValid();

#if (FEATURE_ON == FEATURE_LTE)
    pstCsPsMode1ReCampLtePlmn  = NAS_MMC_GetCsPsMode1ReCampLtePlmn_PlmnSelection();
    ulIsReCampLtePlmnHplmn     = NAS_MML_ComparePlmnIdWithHplmn(pstCsPsMode1ReCampLtePlmn);
#endif


    /* ����Ѿ����Զ�ģʽ��������״̬�����ڲ��б���״̬���򱳾���״̬�������ٴ�ϵ�ǰ״̬������������֧���û���ѡ���Ƶ�NV��9074�رգ�ֱ�ӻظ�ok */
    if ((VOS_FALSE == ulUserAutoReselActiveFlg)
     && ((NAS_MMC_FSM_PLMN_SELECTION == enFsmId)
      || (NAS_MMC_FSM_PLMN_LIST == enFsmId)
      || (NAS_MMC_FSM_BG_PLMN_SEARCH == enFsmId)))
    {
        return VOS_FALSE;
    }



    if ( VOS_FALSE == NAS_MMC_IsNeedPlmnSearchGUSearchState_UserReselReq(
                    enFsmId,ulUserAutoReselActiveFlg,ulIsCurrSearchingPlmnHplmn,ulCurrState) )
    {
        return VOS_FALSE;
    }

#if (FEATURE_ON == FEATURE_LTE)
    /* ����״̬�������У���L�����͹���״̬ʱ��Ҫ�жϵ�ǰ���������������Ƿ�Ϊhplmn��nv 9705��ʱ�����������hplmn�������� */
    if ((NAS_MMC_FSM_PLMN_SELECTION == enFsmId)
     && (VOS_TRUE == ulUserAutoReselActiveFlg))
    {
        if ((VOS_TRUE == ulIsCurrSearchingPlmnHplmn)
         && ((NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_PLMN_SEARCH_CNF  ==  ulCurrState )
          || (NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_SUSPEND_CNF ==  ulCurrState)))
        {
            return VOS_FALSE;
        }

        if ((VOS_TRUE ==ulIsReCampLtePlmnHplmn)
         && (NAS_MMC_PLMN_SELECTION_STA_WAIT_LMM_PLMN_SEARCH_CNF_CSPSMODE1_RECAMP_LTE == ulCurrState))
        {
            return VOS_FALSE;
        }

        return VOS_TRUE;
    }
#endif


    /* anycell״̬�������������ǰ����Ч�����������·���anycell�� */
    if ((NAS_MMC_FSM_ANYCELL_SEARCH == enFsmId)
     && (VOS_FALSE == ulIsUsimStatusValid))
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_IsNeedPlmnSearch_Oos(
    RRMM_LIMIT_SERVICE_CAMP_IND_STRU        *pstRrmmLimitCampOnMsg
)
{
    /* ����RRC����,��ǰ�Ĳ���Ϊstart,�����ϱ���PLMN������Ϊ0ʱ,��Ҫ�������� */

    if (RRC_RRC_CONN_STATUS_PRESENT != pstRrmmLimitCampOnMsg->enRrcConnStatus)
    {
        return VOS_FALSE;
    }

    if (VOS_FALSE == NAS_MML_IsUsimStausValid())
    {
        return VOS_FALSE;
    }

    if ((0 == pstRrmmLimitCampOnMsg->stPlmnIdList.ulHighPlmnNum)
     && (0 == pstRrmmLimitCampOnMsg->stPlmnIdList.ulLowPlmnNum))
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}



#if (FEATURE_ON == FEATURE_PTM)
VOS_UINT32 NAS_MMC_RcvAcpuOmErrLogRptReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_CHAR                           *pbuffer   = VOS_NULL_PTR;
    VOS_UINT32                          ulBufUseLen;
    VOS_UINT32                          ulRealLen;

    /* ��ѯһ��RING BUFFER���ж������ݣ��Ա�����ڴ� */
    ulBufUseLen = NAS_MML_GetErrLogRingBufUseBytes();

    /* ���RING BUFFER��û�����ݣ�Ҳ��Ҫ��OM������Ϣ */
    if (0 == ulBufUseLen)
    {
        /* ����ID_OM_ERR_LOG_REPORT_CNF����Ϊ�յ���Ϣ��OM */
        NAS_MMC_SndAcpuOmErrLogRptCnf(VOS_NULL_PTR, 0);
        return VOS_TRUE;
    }

    pbuffer = (VOS_CHAR *)PS_MEM_ALLOC(WUEPS_PID_MMC, ulBufUseLen);
    if (VOS_NULL_PTR == pbuffer)
    {
        /* ����ID_OM_ERR_LOG_REPORT_CNF����Ϊ�յ���Ϣ��OM */
        NAS_MMC_SndAcpuOmErrLogRptCnf(VOS_NULL_PTR, 0);
        return VOS_TRUE;
    }

    PS_MEM_SET(pbuffer, 0, ulBufUseLen);

    /* ��ȡRING BUFFER������ */
    ulRealLen = NAS_MML_GetErrLogRingBufContent(pbuffer, ulBufUseLen);
    if (ulRealLen != ulBufUseLen)
    {
        /* ����ID_OM_ERR_LOG_REPORT_CNF����Ϊ�յ���Ϣ��OM */
        NAS_MMC_SndAcpuOmErrLogRptCnf(VOS_NULL_PTR, 0);
        PS_MEM_FREE(WUEPS_PID_MMC, pbuffer);
        return VOS_TRUE;
    }

    /* ��ȡ���˺���Ҫ��RINGBUFFER��� */
    NAS_MML_CleanErrLogRingBuf();

    /* ����ID_OM_ERR_LOG_REPORT_CNF��Ϣ��ACPU OM */
    NAS_MMC_SndAcpuOmErrLogRptCnf(pbuffer, ulBufUseLen);

    PS_MEM_FREE(WUEPS_PID_MMC, pbuffer);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvAcpuOmErrLogCtrlInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    OM_ERROR_LOG_CTRL_IND_STRU         *pstRcvMsg = VOS_NULL_PTR;

    pstRcvMsg = (OM_ERROR_LOG_CTRL_IND_STRU*)pstMsg;

    /* ����ERRLOG���Ʊ�ʶ */
    if ((VOS_FALSE == pstRcvMsg->ucAlmStatus)
     || (VOS_TRUE  == pstRcvMsg->ucAlmStatus))
    {
        NAS_MML_SetErrlogCtrlFlag(pstRcvMsg->ucAlmStatus);
    }

    if ((pstRcvMsg->ucAlmLevel >= NAS_ERR_LOG_CTRL_LEVEL_CRITICAL)
     && (pstRcvMsg->ucAlmLevel <= NAS_ERR_LOG_CTRL_LEVEL_WARNING))
    {
        NAS_MML_SetErrlogAlmLevel(pstRcvMsg->ucAlmLevel);
    }

    return VOS_TRUE;
}
VOS_UINT32 NAS_MMC_RcvAcpuOmFtmCtrlInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    OM_FTM_CTRL_IND_STRU               *pstRcvMsg = VOS_NULL_PTR;

    pstRcvMsg = (OM_FTM_CTRL_IND_STRU*)pstMsg;

    /* ���¹��̲˵����Ʊ�ʶ */
    if ((VOS_FALSE == pstRcvMsg->ucActionFlag)
     || (VOS_TRUE  == pstRcvMsg->ucActionFlag))
    {
        NAS_MML_SetFtmCtrlFlag(pstRcvMsg->ucActionFlag);
    }

    /* ����ǹر�ָʾ��ʲô�������� */
    if (VOS_FALSE == pstRcvMsg->ucActionFlag)
    {
        return VOS_TRUE;
    }

    /* �յ��򿪹��̲˵���ָʾ����Ҫ����ǰ��TMSI/PTMSI�ϱ����ϲ�Ӧ�� */
    NAS_MMC_SndAcpuOmCurTmsi();
    NAS_MMC_SndAcpuOmCurPtmsi();

    return VOS_TRUE;
}
#endif


VOS_UINT32  NAS_MMC_RcvTafEOPlmnSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                                              ulLen;
    VOS_UINT16                                              usNewOPlmnNum;
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;
    VOS_UINT32                                              ucMmcTopState;
    VOS_UINT32                                              ulOffset;
    VOS_UINT32                                              ulOplmnNum;
    MMA_MMC_EOPLMN_SET_REQ_STRU                            *pstEOPlmnSetMsg     = VOS_NULL_PTR;
    NAS_MML_USER_CFG_OPLMN_INFO_STRU                       *pstUserCfgOPlmnInfo = VOS_NULL_PTR;
    NAS_MMC_NVIM_USER_CFG_OPLMN_EXTEND_STRU                *pstNvimCfgOPlmnInfo = VOS_NULL_PTR;

    pstEOPlmnSetMsg = (MMA_MMC_EOPLMN_SET_REQ_STRU *)pstMsg;

    ulLen           = 0;
    usNewOPlmnNum   = 0;

    /*д��NV*/
    pstNvimCfgOPlmnInfo = (NAS_MMC_NVIM_USER_CFG_OPLMN_EXTEND_STRU*)PS_MEM_ALLOC(
                                                      WUEPS_PID_MMC,
                                                      sizeof(NAS_MMC_NVIM_USER_CFG_OPLMN_EXTEND_STRU));

    if (VOS_NULL_PTR == pstNvimCfgOPlmnInfo)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTafEOPlmnSetReq_PreProc():ERROR: Memory Alloc Error");
        NAS_MMC_SndMmaEOPlmnSetCnf(VOS_ERR);
        return VOS_TRUE;
    }

    PS_MEM_SET(pstNvimCfgOPlmnInfo, 0x00, sizeof(NAS_MMC_NVIM_USER_CFG_OPLMN_EXTEND_STRU));

    /* ��NV��en_NV_Item_USER_CFG_OPLMN_EXTEND_LIST��ʧ�ܣ�ֱ�ӷ��� */
    NV_GetLength(en_NV_Item_USER_CFG_OPLMN_EXTEND_LIST, &ulLen);

    if (NV_OK != NV_Read(en_NV_Item_USER_CFG_OPLMN_EXTEND_LIST,
                         pstNvimCfgOPlmnInfo, ulLen))
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTafEOPlmnSetReq_PreProc():WARNING: read en_NV_Item_USER_CFG_OPLMN_EXTEND_LIST Error");
        /* �����Ϣ */
        NAS_MMC_SndMmaEOPlmnSetCnf(VOS_ERR);
        PS_MEM_FREE(WUEPS_PID_MMC, pstNvimCfgOPlmnInfo);
        return VOS_TRUE;
    }

    /* ���õ�1������(index =0 )ʱ����Ҫ�����NV�оɵ�OPLMN���� */
    if (0 == pstEOPlmnSetMsg->ucIndex)
    {
        pstNvimCfgOPlmnInfo->usOplmnListNum = 0;
        PS_MEM_SET(pstNvimCfgOPlmnInfo->aucOPlmnList, 0x00, NAS_MMC_NVIM_MAX_USER_CFG_OPLMN_DATA_EXTEND_LEN);
    }

    /* �����µķ�������ʱ����Ҫ���ǰ��ķ��������Ƿ����óɹ���ֻ֧���������� */
    if (NAS_MML_MAX_GROUP_CFG_OPLMN_NUM * pstEOPlmnSetMsg->ucIndex != pstNvimCfgOPlmnInfo->usOplmnListNum )
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTafEOPlmnSetReq_PreProc():WARNING: check old index data Error");
        /* �����Ϣ */
        NAS_MMC_SndMmaEOPlmnSetCnf(VOS_ERR);
        PS_MEM_FREE(WUEPS_PID_MMC, pstNvimCfgOPlmnInfo);
        return VOS_TRUE;
    }

    /* ���ú��µ�OPLMN�������ܴ���256�� */
    usNewOPlmnNum = pstNvimCfgOPlmnInfo->usOplmnListNum + (VOS_UINT16)pstEOPlmnSetMsg->ucOPlmnCount;
    if (NAS_MML_MAX_USER_CFG_OPLMN_NUM < usNewOPlmnNum)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTafEOPlmnSetReq_PreProc():WARNING: check max oplmn num Error");
        /* �����Ϣ */
        NAS_MMC_SndMmaEOPlmnSetCnf(VOS_ERR);
        PS_MEM_FREE(WUEPS_PID_MMC, pstNvimCfgOPlmnInfo);
        return VOS_TRUE;
    }


    PS_MEM_CPY(pstNvimCfgOPlmnInfo->aucVersion,
                pstEOPlmnSetMsg->aucVersion,
                NAS_MML_MAX_USER_OPLMN_VERSION_LEN * sizeof(VOS_INT8));

    /* ������OPLMN���ݱ��浽��Ӧ�ķ���λ�� */
    ulOffset    = NAS_MML_OPLMN_WITH_RAT_UNIT_LEN * pstNvimCfgOPlmnInfo->usOplmnListNum;
    ulOplmnNum  = NAS_MML_OPLMN_WITH_RAT_UNIT_LEN * pstEOPlmnSetMsg->ucOPlmnCount;

    if (NAS_MMC_NVIM_MAX_USER_CFG_OPLMN_DATA_EXTEND_LEN >= (ulOffset + ulOplmnNum))
    {

        PS_MEM_CPY(pstNvimCfgOPlmnInfo->aucOPlmnList + ulOffset,
                    pstEOPlmnSetMsg->aucOPlmnWithRat,
                    ulOplmnNum);
    }

    pstNvimCfgOPlmnInfo->usOplmnListNum = usNewOPlmnNum;

    if (NV_OK != NV_Write(en_NV_Item_USER_CFG_OPLMN_EXTEND_LIST,
                          pstNvimCfgOPlmnInfo,
                          sizeof(NAS_MMC_NVIM_USER_CFG_OPLMN_EXTEND_STRU)))
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvTafEOPlmnSetReq_PreProc():ERROR: Write NV Failed");
        NAS_MMC_SndMmaEOPlmnSetCnf(VOS_ERR);
        PS_MEM_FREE(WUEPS_PID_MMC, pstNvimCfgOPlmnInfo);
        return VOS_TRUE;
    }

    /*NV����ɹ��󼴿�֪ͨTAF*/
    NAS_MMC_SndMmaEOPlmnSetCnf(VOS_OK);

    enFsmId         = (VOS_UINT8)NAS_MMC_GetCurrFsmId();
    ucMmcTopState   = (VOS_UINT8)NAS_MMC_GetFsmTopState();

    /*�ػ�״̬�·�����Чֵ,����Ҫ֪ͨMMCˢ��OPLMN*/
    if ( ( NAS_MMC_FSM_L1_MAIN == enFsmId )
      && ( NAS_MMC_L1_STA_NULL == ucMmcTopState ))
    {
        PS_MEM_FREE(WUEPS_PID_MMC, pstNvimCfgOPlmnInfo);
        return VOS_TRUE;
    }

    /*����NVȫ�ֱ���*/
    pstUserCfgOPlmnInfo                 = NAS_MML_GetUserCfgOPlmnInfo();

    pstUserCfgOPlmnInfo->usOplmnListNum = usNewOPlmnNum;
    PS_MEM_CPY(pstUserCfgOPlmnInfo->aucVersion,
                pstEOPlmnSetMsg->aucVersion,
                NAS_MML_MAX_USER_OPLMN_VERSION_LEN * sizeof(VOS_INT8));


    PS_MEM_FREE(WUEPS_PID_MMC, pstNvimCfgOPlmnInfo);

    /*����MMC�ڲ�OPLMNˢ������*/
    NAS_MMC_SndInterNvimOPlmnRefreshInd();
    return VOS_TRUE;

}


VOS_UINT32 NAS_MMC_RcvMmcInterNvimOPlmnRefreshInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MML_USER_CFG_OPLMN_INFO_STRU                        *pstUserCfgOPlmnInfo = VOS_NULL_PTR;
    NAS_MMC_NVIM_USER_CFG_OPLMN_EXTEND_STRU                 *pstNvimCfgOPlmnInfo = VOS_NULL_PTR;
    NAS_MML_SIM_OPERPLMN_INFO_STRU                          *pstOperPlmnInfo = VOS_NULL_PTR;
    VOS_UINT32                                               ulEfLen;
    VOS_UINT8                                               *pucEf = VOS_NULL_PTR;
    VOS_UINT32                                               ulLen;
    VOS_UINT32                                               ulRet;

    ulEfLen = 0;
    ulLen   = 0;
    ulRet   = 0;

    /* ����MMC�е�OPLMN*/
    pstUserCfgOPlmnInfo = NAS_MML_GetUserCfgOPlmnInfo();

    if (VOS_TRUE == NAS_MML_IsNvimOplmnAvail())
    {
        pstNvimCfgOPlmnInfo = (NAS_MMC_NVIM_USER_CFG_OPLMN_EXTEND_STRU*)PS_MEM_ALLOC(
                                                          WUEPS_PID_MMC,
                                                          sizeof(NAS_MMC_NVIM_USER_CFG_OPLMN_EXTEND_STRU));

        if (VOS_NULL_PTR == pstNvimCfgOPlmnInfo)
        {
            NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvMmcInterNvimOPlmnRefreshInd_PreProc():ERROR: Memory Alloc Error");
            return VOS_TRUE;
        }

        PS_MEM_SET(pstNvimCfgOPlmnInfo, 0x00, sizeof(NAS_MMC_NVIM_USER_CFG_OPLMN_EXTEND_STRU));

        NV_GetLength(en_NV_Item_USER_CFG_OPLMN_EXTEND_LIST, &ulLen);

        /* ��NV��en_NV_Item_USER_CFG_OPLMN_EXTEND_LIST��ʧ�ܣ�ֱ�ӷ��� */
        if (NV_OK != NV_Read(en_NV_Item_USER_CFG_OPLMN_EXTEND_LIST,
                             pstNvimCfgOPlmnInfo, ulLen))
        {
            NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvMmcInterNvimOPlmnRefreshInd_PreProc():WARNING: read en_NV_Item_USER_CFG_OPLMN_EXTEND_LIST Error");
            PS_MEM_FREE(WUEPS_PID_MMC, pstNvimCfgOPlmnInfo);
            return VOS_TRUE;
        }

        NAS_MMC_UpdateOPlmnInfoFromNvim(pstNvimCfgOPlmnInfo->aucOPlmnList, pstUserCfgOPlmnInfo->usOplmnListNum);
        PS_MEM_FREE(WUEPS_PID_MMC, pstNvimCfgOPlmnInfo);

    }
    else /* �����ǰ��NV��Ч������Ҫ�ѿ��ڵ���Ϣ������*/
    {
        /* ���ڵ���Ϣ��������*/
        ulRet = NAS_USIMMAPI_IsServiceAvailable(USIM_SVR_OPLMN_SEL_WACT);

        if ( PS_USIM_SERVICE_AVAILIABLE == ulRet)
        {
            if (USIMM_API_SUCCESS != NAS_USIMMAPI_GetCachedFile(NAS_MML_READ_OPLMN_SEL_FILE_ID, &ulEfLen, &pucEf, USIMM_UNLIMIT_APP))
            {
                NAS_WARNING_LOG(WUEPS_PID_MMC,"NAS_MMC_RcvMmcInterNvimOPlmnRefreshInd_PreProc():ERROR:READ FILE FAIL!");

                /* ��ȡȫ�ֱ����ĵ�ַ */
                pstOperPlmnInfo = NAS_MML_GetSimOperPlmnList();

                /* ��ʼ��ȫ�ֱ����е�OPLMN��Ϣ */
                NAS_MML_InitSimOperPlmnInfo(pstOperPlmnInfo);
            }
            else
            {
                NAS_MMC_SndOmGetCacheFile(NAS_MML_READ_OPLMN_SEL_FILE_ID, ulEfLen, pucEf);

                /* ��ȡ�ļ��ɹ���MMC�ڲ�����Ӧ���� */
                NAS_MMC_ProcAgentUsimReadFileCnf_PreProc(NAS_MML_READ_OPLMN_SEL_FILE_ID, (VOS_UINT16)ulEfLen, pucEf);
            }
        }
        else /* ������ļ�Ҳ�����ã����������*/
        {
            /* ��ȡȫ�ֱ����ĵ�ַ */
            pstOperPlmnInfo = NAS_MML_GetSimOperPlmnList();

            /* ��ʼ��ȫ�ֱ����е�OPLMN��Ϣ */
            NAS_MML_InitSimOperPlmnInfo(pstOperPlmnInfo);

        }
    }

    /* ���¸����ȼ���������*/
    return NAS_MMC_ProHighPrioPlmnRefreshInd_PreProc();
}
VOS_UINT32 NAS_MMC_RcvMmCmServiceInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMCMM_CM_SERVICE_IND_STRU          *pstCmServiceInd = VOS_NULL_PTR;
    VOS_UINT8                           ucCsServiceConnStatusFlag;

    pstCmServiceInd           = (MMCMM_CM_SERVICE_IND_STRU *)pstMsg;
    ucCsServiceConnStatusFlag = NAS_MML_GetCsServiceConnStatusFlg();

    if ((MM_CS_SERV_EXIST == pstCmServiceInd->ulCsServFlg)
     && (VOS_TRUE == ucCsServiceConnStatusFlag))
    {
        NAS_MMC_SndMmaCsServiceConnStatusInd(VOS_TRUE);
        return VOS_FALSE;
    }

    if (MM_CS_SERV_NOT_EXIST == pstCmServiceInd->ulCsServFlg)
    {
        NAS_MMC_SndMmaCsServiceConnStatusInd(VOS_FALSE);
    }

    return VOS_FALSE;
}




VOS_UINT32 NAS_MMC_RcvMmSimAuthFail_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMCMM_SIM_AUTH_FAIL_IND_STRU       *pstSimAuthFail;

    pstSimAuthFail  = (MMCMM_SIM_AUTH_FAIL_IND_STRU *)pstMsg;

    NAS_MMC_SndMmaUsimAuthFailInd(pstSimAuthFail->enSrvDomain, pstSimAuthFail->enSimAuthFailValue);

    return VOS_TRUE;
}


VOS_UINT32 NAS_MMC_RcvGmmSimAuthFail_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMCGMM_SIM_AUTH_FAIL_IND_STRU      *pstSimAuthFail;

    pstSimAuthFail  = (MMCGMM_SIM_AUTH_FAIL_IND_STRU *)pstMsg;

    NAS_MMC_SndMmaUsimAuthFailInd(pstSimAuthFail->enSrvDomain, pstSimAuthFail->enSimAuthFailValue);

    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_IsNetScanConflictWithFsmId_PreProc(
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId
)
{
    /* �ڲ��б��ѡ�BG�ѡ���ϵͳʱ��ֱ�ӻظ�ERROR */
    if ( (NAS_MMC_FSM_PLMN_LIST             == enFsmId)
      || (NAS_MMC_FSM_BG_PLMN_SEARCH        == enFsmId)
      || (NAS_MMC_FSM_INTER_SYS_OOS         == enFsmId)
      || (NAS_MMC_FSM_INTER_SYS_HO          == enFsmId)
      || (NAS_MMC_FSM_INTER_SYS_CCO         == enFsmId)
      || (NAS_MMC_FSM_INTER_SYS_CELLRESEL   == enFsmId) )
    {
       return VOS_TRUE;
    }

    return VOS_FALSE;

}
VOS_UINT32 NAS_MMC_IsNetRatTypeWcdmaOrGsm(
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8     enRat
)
{
    /* ��ǰʱWCDMAʱ�򣬷���VOS_TRUE */
    if ( (NAS_UTRANCTRL_UTRAN_MODE_FDD == NAS_UTRANCTRL_GetCurrUtranMode())
      && (NAS_MML_NET_RAT_TYPE_WCDMA   == enRat) )
    {
        return VOS_TRUE;
    }

    /* ��ǰ��GSMʱ�򣬷���VOS_TRUE */
    if ( NAS_MML_NET_RAT_TYPE_GSM == enRat )
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_IsEnableEndNetScanReq_PreProc(
    MMC_MMA_NET_SCAN_CAUSE_ENUM_UINT8  *penCause
)
{
    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;
    VOS_UINT32                                              ulCsServiceExistFlg;
    VOS_UINT32                                              ulPsServiceExistFlg;
    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8               enSpecPlmnSearchState;

    ulCsServiceExistFlg   = NAS_MML_GetCsServiceExistFlg();
    ulPsServiceExistFlg   = NAS_MML_GetPsServiceExistFlg();
    enSpecPlmnSearchState = NAS_MMC_GetSpecPlmnSearchState();

    /* ����ģʽ���ԣ�ֱ�ӷ���VOS_TRUE */
    if ( VOS_FALSE == NAS_MMC_IsNetRatTypeWcdmaOrGsm(NAS_MML_GetCurrNetRatType()) )
    {
        *penCause = MMC_MMA_NET_SCAN_CAUSE_RAT_TYPE_ERROR;

        return VOS_TRUE;
    }

    /* ����ҵ�����ӣ���MMAģ��ظ����ʧ�ܣ�ԭ��ֵ��ͻ */
    if ( (VOS_TRUE == ulCsServiceExistFlg)
      || (VOS_TRUE == ulPsServiceExistFlg) )
    {
        *penCause = MMC_MMA_NET_SCAN_CAUSE_SERVICE_EXIST;

        return VOS_TRUE;
    }

    enFsmId = NAS_MMC_GetCurrFsmId();

    /* �ڲ��б��ѡ�BG�ѡ���ϵͳʱ��ֱ�ӻظ�ERROR */
    if ( VOS_TRUE == NAS_MMC_IsNetScanConflictWithFsmId_PreProc(enFsmId) )
    {
        *penCause = MMC_MMA_NET_SCAN_CAUSE_CONFLICT;

        return VOS_TRUE;
    }

    /* ��ǰ������״̬����OOCʱ���п�������Ϊδפ��������ERROR */
    if ( ( (NAS_MMC_L1_STA_OOC  == NAS_MMC_GetFsmTopState())
         && (NAS_MMC_FSM_L1_MAIN == enFsmId) )
      || (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING == enSpecPlmnSearchState) )
    {

        if (VOS_TRUE == NAS_MML_GetSimPresentStatus())
        {
            *penCause = MMC_MMA_NET_SCAN_CAUSE_NOT_CAMPED;

            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_IsNeedStartPeriodicNetScanTimer_PreProc(VOS_VOID)
{

    NAS_MMC_FSM_ID_ENUM_UINT32                              enFsmId;
    NAS_MMC_SPEC_PLMN_SEARCH_STATE_ENUM_UINT8               enSpecPlmnSearchState;

    enFsmId               = NAS_MMC_GetCurrFsmId();
    enSpecPlmnSearchState = NAS_MMC_GetSpecPlmnSearchState();

    /* ��ǰ������״̬����OOCʱ���޿������������Զ�ʱ�� */
    if ( ( (NAS_MMC_L1_STA_OOC  == NAS_MMC_GetFsmTopState())
         && (NAS_MMC_FSM_L1_MAIN == enFsmId) )
      || (NAS_MMC_SPEC_PLMN_SEARCH_RUNNING == enSpecPlmnSearchState) )
    {
        if (VOS_FALSE == NAS_MML_GetSimPresentStatus())
        {
            return VOS_TRUE;
        }
    }

    /* �������Ӵ��ڣ������������Զ�ʱ�� */
    if (VOS_TRUE == NAS_MML_IsRrcConnExist())
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_VOID NAS_MMC_ProcessNetScanReq_PreProc(
    MMA_MMC_NET_SCAN_REQ_STRU          *pstNetScanReq
)
{
    MMC_MMA_NET_SCAN_CAUSE_ENUM_UINT8   enCause;
    MMC_MMA_NET_SCAN_CNF_STRU           stNetScanCnf;

    /* ��ǰ�Ƿ���Ҫֱ�Ӹ�MMA�ظ� */
    if ( VOS_TRUE == NAS_MMC_IsEnableEndNetScanReq_PreProc(&enCause) )
    {
        PS_MEM_SET(&stNetScanCnf, 0X00, sizeof(MMC_MMA_NET_SCAN_CNF_STRU));

        stNetScanCnf.enResult = MMC_MMA_NET_SCAN_RESULT_FAILURE;
        stNetScanCnf.enCause  = enCause;
        NAS_MMC_SndMmaNetScanCnf(&stNetScanCnf);

        return;
    }

    /* ��ǰ�Ƿ���Ҫ����������ɨ�趨ʱ�� */
    if ( VOS_TRUE == NAS_MMC_IsNeedStartPeriodicNetScanTimer_PreProc() )
    {
        NAS_MMC_StartTimer(TI_NAS_MMC_PERIODIC_NET_SCAN_TIMER, TI_NAS_MMC_PERIODIC_NET_SCAN_TIMER_LEN);

        return;
    }

    /* ��GUAS����NETSCAN���� */
    NAS_MMC_SndAsNetScanReq(NAS_MML_GetCurrNetRatType(), pstNetScanReq);

    return;
}
VOS_UINT32  NAS_MMC_RcvMmaNetScanReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMA_MMC_NET_SCAN_REQ_STRU          *pstNetScanReq       = VOS_NULL_PTR;

    pstNetScanReq = (MMA_MMC_NET_SCAN_REQ_STRU*)pstMsg;

    /* ����MMAģ�鷢�͹�����NETSCAN���������Զ�ʱ����ʱ����ʹ�� */
    NAS_MMC_SetNetScanReq(pstNetScanReq);

    NAS_MMC_ProcessNetScanReq_PreProc(pstNetScanReq);

    return VOS_TRUE;
}
VOS_UINT32  NAS_MMC_RcvMmaAbortNetScanReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMC_MMA_ABORT_NET_SCAN_CNF_STRU     stAbortNetScanCnf;

    PS_MEM_SET(&stAbortNetScanCnf, 0x00, sizeof(stAbortNetScanCnf));

    if ( NAS_MMC_TIMER_STATUS_RUNING == NAS_MMC_GetTimerStatus(TI_NAS_MMC_PERIODIC_NET_SCAN_TIMER) )
    {
        NAS_MMC_StopTimer(TI_NAS_MMC_PERIODIC_NET_SCAN_TIMER);

        /* ��MMA���Ϳյ�ɨ���� */
        NAS_MMC_SndMmaAbortNetScanCnf(&stAbortNetScanCnf);
    }
    else
    {
        if ( VOS_TRUE == NAS_MMC_IsNetRatTypeWcdmaOrGsm(NAS_MML_GetCurrNetRatType()) )
        {
            /* ������㷢��ֹͣɨ������ */
            NAS_MMC_SndAsNetScanStopReq(NAS_MML_GetCurrNetRatType());
        }
        else
        {
            NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_RcvMmaAbortNetScanReq_PreProc:rcv unexpected abort net scan req!");
        }
    }

    return VOS_TRUE;
}


VOS_UINT32  NAS_MMC_RcvPeriodicNetScanExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMA_MMC_NET_SCAN_REQ_STRU          *pstNetScanReq = VOS_NULL_PTR;

    /* ��ȡ�Ѿ������NETSCAN REQ */
    pstNetScanReq         = NAS_MMC_GetNetScanReq();

    NAS_MMC_ProcessNetScanReq_PreProc(pstNetScanReq);

    return VOS_TRUE;
}
VOS_VOID NAS_MMC_ConvertNetScanRrcBandToNasFormat(
    VOS_UINT32                          ulBand,
    NAS_MML_MS_BAND_INFO_STRU          *pstNasBand
)
{
    MMA_MMC_NET_SCAN_REQ_STRU          *pstReq              = VOS_NULL_PTR;

    /* ��ȡ�����NETSCAN���� */
    pstReq                              = NAS_MMC_GetNetScanReq();

    if ( NAS_MML_NET_RAT_TYPE_GSM == pstReq->ucRat )
    {
        pstNasBand->unGsmBand.ulBand    = ulBand;

        return;
    }

    pstNasBand->unWcdmaBand.ulBand      = ulBand;

    return;
}
VOS_VOID NAS_MMC_ConvertRrcCauseToNasFormat(
    RRC_NAS_NET_SCAN_CAUSE_ENUM_UINT8   enRrcCause,
    MMC_MMA_NET_SCAN_CAUSE_ENUM_UINT8  *penMmcCause
)
{
    switch ( enRrcCause )
    {
        case RRC_NAS_NET_SCAN_CAUSE_SIGNAL_EXIST :
            *penMmcCause = MMC_MMA_NET_SCAN_CAUSE_SIGNAL_EXIST;
            break;

        case RRC_NAS_NET_SCAN_CAUSE_STATE_NOT_ALLOWED :
            *penMmcCause = MMC_MMA_NET_SCAN_CAUSE_STATE_NOT_ALLOWED;
            break;

        case RRC_NAS_NET_SCAN_CAUSE_FREQ_LOCK :
            *penMmcCause = MMC_MMA_NET_SCAN_CAUSE_FREQ_LOCK;
            break;

        case RRC_NAS_NET_SCAN_CAUSE_PARA_ERROR :
            *penMmcCause = MMC_MMA_NET_SCAN_CAUSE_PARA_ERROR;
            break;

        default:
            *penMmcCause = MMC_MMA_NET_SCAN_CAUSE_BUTT;
            break;
    }

    return;
}



VOS_VOID NAS_MMC_ConvertRrcNetScanCnfToNasFormat(
    RRMM_NET_SCAN_CNF_STRU             *pstRrmmNetScanCnfMsg,
    MMC_MMA_NET_SCAN_CNF_STRU          *pstNetScanCnf
)
{
    VOS_UINT32                          i;

    pstNetScanCnf->enResult             = pstRrmmNetScanCnfMsg->enResult;
    pstNetScanCnf->ucFreqNum            = pstRrmmNetScanCnfMsg->ucFreqNum;

    NAS_MMC_ConvertRrcCauseToNasFormat(pstRrmmNetScanCnfMsg->enCause, &pstNetScanCnf->enCause);

    if ( MMC_MMA_NET_SCAN_MAX_FREQ_NUM < pstNetScanCnf->ucFreqNum )
    {
        pstNetScanCnf->ucFreqNum        = MMC_MMA_NET_SCAN_MAX_FREQ_NUM;
    }

    for ( i = 0 ; i < pstNetScanCnf->ucFreqNum; i++ )
    {
        pstNetScanCnf->astNetScanInfo[i].usArfcn        = pstRrmmNetScanCnfMsg->astNetScanInfo[i].usArfcn;
        pstNetScanCnf->astNetScanInfo[i].usC1           = pstRrmmNetScanCnfMsg->astNetScanInfo[i].usC1;
        pstNetScanCnf->astNetScanInfo[i].usC2           = pstRrmmNetScanCnfMsg->astNetScanInfo[i].usC2;
        pstNetScanCnf->astNetScanInfo[i].usLac          = pstRrmmNetScanCnfMsg->astNetScanInfo[i].usLac;
        pstNetScanCnf->astNetScanInfo[i].ulMcc          = pstRrmmNetScanCnfMsg->astNetScanInfo[i].ulMcc;
        pstNetScanCnf->astNetScanInfo[i].ulMnc          = pstRrmmNetScanCnfMsg->astNetScanInfo[i].ulMnc;
        pstNetScanCnf->astNetScanInfo[i].usBsic         = pstRrmmNetScanCnfMsg->astNetScanInfo[i].usBsic;
        pstNetScanCnf->astNetScanInfo[i].sRxlev         = pstRrmmNetScanCnfMsg->astNetScanInfo[i].sRxlev;
        pstNetScanCnf->astNetScanInfo[i].sRssi          = pstRrmmNetScanCnfMsg->astNetScanInfo[i].sRssi;
        pstNetScanCnf->astNetScanInfo[i].ulCellId       = pstRrmmNetScanCnfMsg->astNetScanInfo[i].ulCellId;

        NAS_MMC_ConvertNetScanRrcBandToNasFormat(pstRrmmNetScanCnfMsg->astNetScanInfo[i].aulBand[0],
                                          &pstNetScanCnf->astNetScanInfo[i].stBand);
    }

    return;
}




VOS_UINT32  NAS_MMC_RcvRrMmNetScanCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_NET_SCAN_CNF_STRU             *pstRcvRrmmMsg       = VOS_NULL_PTR;
    MMC_MMA_NET_SCAN_CNF_STRU           stNetScanCnf;

    pstRcvRrmmMsg = (RRMM_NET_SCAN_CNF_STRU *)pstMsg;

    PS_MEM_SET(&stNetScanCnf, 0x00, sizeof(stNetScanCnf));

    /* �����������ӵ�ʱ����Ҫ�������ڳ��Զ�ʱ�� */
    if ( (RRC_NAS_NET_SCAN_RESULT_FAILURE     == pstRcvRrmmMsg->enResult)
      && (RRC_NAS_NET_SCAN_CAUSE_SIGNAL_EXIST == pstRcvRrmmMsg->enCause) )
    {
        NAS_MMC_StartTimer(TI_NAS_MMC_PERIODIC_NET_SCAN_TIMER, TI_NAS_MMC_PERIODIC_NET_SCAN_TIMER_LEN);

        return VOS_TRUE;
    }

    NAS_MMC_ConvertRrcNetScanCnfToNasFormat(pstRcvRrmmMsg, &stNetScanCnf);
    NAS_MMC_SndMmaNetScanCnf(&stNetScanCnf);

    return VOS_TRUE;
}
VOS_VOID NAS_MMC_ConvertRrcNetScanStopCnfToNasFormat(
    RRMM_NET_SCAN_STOP_CNF_STRU        *pstRrmmNetScanCnfMsg,
    MMC_MMA_ABORT_NET_SCAN_CNF_STRU    *pstNetScanCnf
)
{
    VOS_UINT32                          i;

    pstNetScanCnf->ucFreqNum = pstRrmmNetScanCnfMsg->ucFreqNum;

    if ( MMC_MMA_NET_SCAN_MAX_FREQ_NUM < pstNetScanCnf->ucFreqNum )
    {
        pstNetScanCnf->ucFreqNum = MMC_MMA_NET_SCAN_MAX_FREQ_NUM;
    }

    for ( i = 0 ; i < pstNetScanCnf->ucFreqNum; i++ )
    {
        pstNetScanCnf->astNetScanInfo[i].usArfcn        = pstRrmmNetScanCnfMsg->astNetScanInfo[i].usArfcn;
        pstNetScanCnf->astNetScanInfo[i].usC1           = pstRrmmNetScanCnfMsg->astNetScanInfo[i].usC1;
        pstNetScanCnf->astNetScanInfo[i].usC2           = pstRrmmNetScanCnfMsg->astNetScanInfo[i].usC2;
        pstNetScanCnf->astNetScanInfo[i].usLac          = pstRrmmNetScanCnfMsg->astNetScanInfo[i].usLac;
        pstNetScanCnf->astNetScanInfo[i].ulMcc          = pstRrmmNetScanCnfMsg->astNetScanInfo[i].ulMcc;
        pstNetScanCnf->astNetScanInfo[i].ulMnc          = pstRrmmNetScanCnfMsg->astNetScanInfo[i].ulMnc;
        pstNetScanCnf->astNetScanInfo[i].usBsic         = pstRrmmNetScanCnfMsg->astNetScanInfo[i].usBsic;
        pstNetScanCnf->astNetScanInfo[i].sRxlev         = pstRrmmNetScanCnfMsg->astNetScanInfo[i].sRxlev;
        pstNetScanCnf->astNetScanInfo[i].sRssi          = pstRrmmNetScanCnfMsg->astNetScanInfo[i].sRssi;
        pstNetScanCnf->astNetScanInfo[i].ulCellId       = pstRrmmNetScanCnfMsg->astNetScanInfo[i].ulCellId;

        NAS_MMC_ConvertNetScanRrcBandToNasFormat(pstRrmmNetScanCnfMsg->astNetScanInfo[i].aulBand[0],
                                          &pstNetScanCnf->astNetScanInfo[i].stBand);
    }

    return;
}



VOS_UINT32  NAS_MMC_RcvRrMmNetScanStopCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRMM_NET_SCAN_STOP_CNF_STRU        *pstRcvRrmmMsg       = VOS_NULL_PTR;
    MMC_MMA_ABORT_NET_SCAN_CNF_STRU     stAbortNetScanCnf;

    pstRcvRrmmMsg = (RRMM_NET_SCAN_STOP_CNF_STRU *)pstMsg;

    PS_MEM_SET(&stAbortNetScanCnf, 0x00, sizeof(MMC_MMA_ABORT_NET_SCAN_CNF_STRU));

    /* ��������ϱ���ֹͣɨ����ת����NAS�Ľṹ */
    NAS_MMC_ConvertRrcNetScanStopCnfToNasFormat(pstRcvRrmmMsg, &stAbortNetScanCnf);

    /* ��MMAģ��ת��������ϱ���STOP CNF���� */
    NAS_MMC_SndMmaAbortNetScanCnf(&stAbortNetScanCnf);

    return VOS_TRUE;
}



VOS_UINT32  NAS_MMC_RcvGasNcellMonitorInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    RRC_NAS_NCELL_MONITOR_IND_STRU     *pstNcellMonitorInd;

    pstNcellMonitorInd = (RRC_NAS_NCELL_MONITOR_IND_STRU*)pstMsg;

    if ( RRC_NAS_NCELL_STATE_3G == pstNcellMonitorInd->enNcellState)
    {
        NAS_MML_SetRrcUtranNcellExistFlg(VOS_TRUE);
        NAS_MML_SetRrcLteNcellExistFlg(VOS_FALSE);
    }
    else if ( RRC_NAS_NCELL_STATE_4G == pstNcellMonitorInd->enNcellState)
    {
        NAS_MML_SetRrcLteNcellExistFlg(VOS_TRUE);
        NAS_MML_SetRrcUtranNcellExistFlg(VOS_FALSE);
    }
    else if ( RRC_NAS_NCELL_STATE_3G4G == pstNcellMonitorInd->enNcellState)
    {
        NAS_MML_SetRrcUtranNcellExistFlg(VOS_TRUE);
        NAS_MML_SetRrcLteNcellExistFlg(VOS_TRUE);
    }
    else
    {
        NAS_MML_SetRrcUtranNcellExistFlg(VOS_FALSE);
        NAS_MML_SetRrcLteNcellExistFlg(VOS_FALSE);
    }


    return VOS_TRUE;
}



VOS_UINT32 NAS_MMC_RcvMmaImsVoiceCapInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMA_MMC_IMS_VOICE_CAP_IND_STRU     *pstImsVoiceInd = VOS_NULL_PTR;
#if (FEATURE_ON == FEATURE_LTE)
    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;
#endif

    pstImsVoiceInd = (MMA_MMC_IMS_VOICE_CAP_IND_STRU *)pstMsg;

    /* ��֮ǰ���,����ı�ʱ֪ͨLMM��ǰIMS VOICE�Ƿ���� */
    if (NAS_MML_GetImsVoiceAvailFlg() != pstImsVoiceInd->ucAvail)
    {
#if (FEATURE_ON == FEATURE_LTE)
        NAS_MMC_SndLmmImsVoiceCapChangeNtf(pstImsVoiceInd->ucAvail);
#endif

        /* �洢IMS�Ƿ������Ϣ��MML��ȫ�ֱ����� */
        NAS_MML_SetImsVoiceAvailFlg(pstImsVoiceInd->ucAvail);
    }

#if (FEATURE_ON == FEATURE_LTE)

    /* ON PLMN��ֱ�ӿ��Դ��� */
    enFsmId = NAS_MMC_GetCurrFsmId();

    if ((NAS_MMC_L1_STA_ON_PLMN == NAS_MMC_GetFsmTopState())
     && (NAS_MMC_FSM_L1_MAIN    == enFsmId))
    {
        if (VOS_TRUE == pstImsVoiceInd->ucAvail)
        {
            return VOS_TRUE;
        }

        /* ������IMS voice������ʱ��Disable Lģ����������һ������ */
        if (VOS_FALSE == NAS_MMC_IsNeedDisableLte_ImsVoiceNotAvail())
        {
            return VOS_TRUE;
        }

        NAS_MML_SetDisableLteReason(MMC_LMM_DISABLE_LTE_REASON_LTE_VOICE_NOT_AVAILABLE);

        /* GU�²���Ҫ�ٴδ�������,����ֱ��֪ͨRRC����Disable Lģ */
        if (NAS_MML_NET_RAT_TYPE_LTE != NAS_MML_GetCurrNetRatType())
        {
            /* ��WAS/GAS����disable LTE֪ͨ��Ϣ */
            NAS_MMC_SndAsLteCapabilityStatus(WUEPS_PID_WRR, RRC_NAS_LTE_CAPABILITY_STATUS_DISABLE);
            NAS_MMC_SndAsLteCapabilityStatus(UEPS_PID_GAS, RRC_NAS_LTE_CAPABILITY_STATUS_DISABLE);

            /* ��LMM����disable LTE��Ϣ */
            NAS_MMC_SndLmmDisableLteNotify(MMC_LMM_DISABLE_LTE_REASON_LTE_VOICE_NOT_AVAILABLE);

            NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_NOTIFIED_AS);

            return VOS_TRUE;
        }

        NAS_MML_SetLteCapabilityStatus(NAS_MML_LTE_CAPABILITY_STATUS_DISABLE_UNNOTIFY_AS);

        /* �����ǰL��EPS�������Ӵ��ڣ��򻺴湹���������Ϣ */
        if (VOS_TRUE == NAS_MML_IsRrcConnExist())
        {
            NAS_MMC_SetBufferedPlmnSearchInfo(VOS_TRUE, NAS_MMC_PLMN_SEARCH_SCENE_DISABLE_LTE);

            NAS_MMC_SndLmmRelReq();

            return VOS_TRUE;
        }
    }
    return VOS_FALSE;
#else
    return VOS_TRUE;
#endif
}


VOS_UINT32 NAS_MMC_RcvTiHighPrioRatHplmnSrchTimerExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (VOS_FALSE == NAS_MMC_IsHighPrioRatHplmnSearchVaild())
    {
        return VOS_TRUE;
    }

    /* פ������HPLMN�϶�������Ϣ */
    if (VOS_FALSE == NAS_MML_ComparePlmnIdWithHplmn(NAS_MML_GetCurrCampPlmnId()))
    {
        return VOS_TRUE;
    }

    /* פ����HPLMN��Ϊ������ȼ��Ľ��뼼����λ��ʱ���������� */
    if (VOS_TRUE == NAS_MMC_IsCampOnHighestPrioRatHplmn())
    {
        NAS_MMC_ResetCurHighPrioRatHplmnTimerFirstSearchCount_L1Main();

        return VOS_TRUE;
    }

    /* ���������ϱ�not camp on��Ϣ���������𱳾����� */
    if (NAS_MMC_AS_CELL_NOT_CAMP_ON == NAS_MMC_GetAsCellCampOn())
    {
        return VOS_FALSE;
    }

    /* �޷����и����ȼ�����ʱ���������ö�ʱ������ */
    if (VOS_FALSE == NAS_MMC_IsEnableBgPlmnSearch_PreProc())
    {
        NAS_MMC_StartTimer(TI_NAS_MMC_HIGH_PRIO_RAT_HPLMN_TIMER, NAS_MMC_GetHighPrioRatHplmnTimerRetryLen());

        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_RcvGmmServiceRequestResultInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    GMMMMC_SERVICE_REQUEST_RESULT_IND_STRU                 *pstGmmServReqRsltInd = VOS_NULL_PTR;

    pstGmmServReqRsltInd  = (GMMMMC_SERVICE_REQUEST_RESULT_IND_STRU*)pstMsg;

    /* ���񱻾ܳ�����Ҫ�����ϱ� */
    if (GMM_MMC_ACTION_RESULT_FAILURE == pstGmmServReqRsltInd->enActionResult)
    {
        NAS_MMC_SndMmaServRejRsltInd(MMA_MMC_SRVDOMAIN_PS,
                                     pstGmmServReqRsltInd->enRegFailCause);
    }

    return VOS_FALSE;
}
VOS_UINT32 NAS_MMC_RcvRrmmSuspendInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    
    RRMM_SUSPEND_IND_ST            *pstSuspendMsg = VOS_NULL_PTR;
    NAS_MMC_FSM_ID_ENUM_UINT32      enFsmId;

    pstSuspendMsg = (RRMM_SUSPEND_IND_ST*)pstMsg;
    
    if ( ( NAS_MML_NET_RAT_TYPE_WCDMA != NAS_MML_GetCurrNetRatType())
      && ( WUEPS_PID_WRR == pstSuspendMsg->MsgHeader.ulSenderPid ))
    {
        /* ��ǰ����פ����Wģʽ�£����ع���ʧ�� */
        NAS_MMC_SndAsSuspendRsp(RRC_NAS_SUSPEND_FAILURE, WUEPS_PID_WRR);

        return VOS_TRUE;
    }
    
    if ( ( NAS_MML_NET_RAT_TYPE_GSM != NAS_MML_GetCurrNetRatType())
      && ( UEPS_PID_GAS == pstSuspendMsg->MsgHeader.ulSenderPid ))
    {
        /* ��ǰ����פ����Gģʽ�£����ع���ʧ�� */
        NAS_MMC_SndAsSuspendRsp(RRC_NAS_SUSPEND_FAILURE, UEPS_PID_GAS);
        
        return VOS_TRUE;
    }
    
    if ( pstSuspendMsg->ucSuspendCause >= MMC_SUSPEND_CAUSE_BUTT )
    {
        /* ��������Ƿ� */
        NAS_MMC_SndAsSuspendRsp(RRC_NAS_SUSPEND_FAILURE, pstSuspendMsg->MsgHeader.ulSenderPid);
        
        return VOS_TRUE;
    }

    /* ���Ϊ��ǰģʽ,�Ѿ�����ϵͳ״̬����,�ٴ��յ�������Ϣ,ֱ�Ӹ�λ */
    enFsmId = NAS_MMC_GetCurrFsmId();
    if ( ( NAS_MMC_FSM_INTER_SYS_HO == enFsmId )
      || ( NAS_MMC_FSM_INTER_SYS_CELLRESEL == enFsmId )  
      || ( NAS_MMC_FSM_INTER_SYS_OOS == enFsmId ) )
    {
        if ( ( NAS_MML_NET_RAT_TYPE_WCDMA == NAS_MML_GetCurrNetRatType())
          && ( WUEPS_PID_WRR == pstSuspendMsg->MsgHeader.ulSenderPid ))
        {
            NAS_MML_SoftReBoot();
        }

        if ( ( NAS_MML_NET_RAT_TYPE_GSM == NAS_MML_GetCurrNetRatType())
          && ( UEPS_PID_GAS == pstSuspendMsg->MsgHeader.ulSenderPid ))
        {
            NAS_MML_SoftReBoot();
        }
        
        return VOS_TRUE;
    }

    /* ����״̬������ */    
    return VOS_FALSE;
}
VOS_UINT32 NAS_MMC_RcvRrmmResumeInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    
    RRMM_RESUME_IND_ST             *pstResumeMsg = VOS_NULL_PTR;
    NAS_MMC_FSM_ID_ENUM_UINT32      enFsmId;

    pstResumeMsg = (RRMM_RESUME_IND_ST*)pstMsg;
    
    /* ���Ϊ��ǰģʽ,�Ѿ�����ϵͳ״̬����,�ٴ��յ�������Ϣ,ֱ�Ӹ�λ */
    enFsmId = NAS_MMC_GetCurrFsmId();
    if ( ( NAS_MMC_FSM_INTER_SYS_HO != enFsmId )
      && ( NAS_MMC_FSM_INTER_SYS_CELLRESEL != enFsmId )  
      && ( NAS_MMC_FSM_INTER_SYS_OOS != enFsmId ) 
      && ( NAS_MMC_FSM_INTER_SYS_CCO != enFsmId ))
    {
        if ( NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
        {
            NAS_MML_SoftReBoot();
        }
        
        if ( ( NAS_MML_NET_RAT_TYPE_WCDMA == NAS_MML_GetCurrNetRatType())
          && ( WUEPS_PID_WRR != pstResumeMsg->MsgHeader.ulSenderPid ))
        {
            NAS_MML_SoftReBoot();
        }

        if ( ( NAS_MML_NET_RAT_TYPE_GSM == NAS_MML_GetCurrNetRatType())
          && ( UEPS_PID_GAS != pstResumeMsg->MsgHeader.ulSenderPid ))
        {
            NAS_MML_SoftReBoot();
        }
        
        return VOS_TRUE;
    }

    /* ����״̬������ */    
    return VOS_FALSE;
}


#if   (FEATURE_ON == FEATURE_LTE)

VOS_UINT32 NAS_MMC_RcvLmmSuspendInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    
    LMM_MMC_SUSPEND_IND_STRU           *pstSuspendMsg = VOS_NULL_PTR;
    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;

    pstSuspendMsg = (LMM_MMC_SUSPEND_IND_STRU*)pstMsg;
    
    if ( NAS_MML_NET_RAT_TYPE_LTE != NAS_MML_GetCurrNetRatType())
    {
        /* ��ǰ����פ����Lģʽ�£����ع���ʧ�� */
        NAS_MMC_SndLmmSuspendRsp(MMC_LMM_FAIL);

        return VOS_TRUE;
    }
    
    if ( pstSuspendMsg->ulSysChngType >= MMC_LMM_SUS_TYPE_BUTT )
    {
        /* ��������Ƿ� */
        NAS_MMC_SndLmmSuspendRsp(MMC_LMM_FAIL);
        
        return VOS_TRUE;
    }

    /* ���Ϊ��ǰģʽ,�Ѿ�����ϵͳ״̬����,�ٴ��յ�������Ϣ,ֱ�Ӹ�λ */
    enFsmId = NAS_MMC_GetCurrFsmId();
    if ( ( NAS_MMC_FSM_INTER_SYS_HO == enFsmId )
      || ( NAS_MMC_FSM_INTER_SYS_CELLRESEL == enFsmId )  
      || ( NAS_MMC_FSM_INTER_SYS_OOS == enFsmId ) )
    {
        if ( NAS_MML_NET_RAT_TYPE_LTE == NAS_MML_GetCurrNetRatType())
        {
            NAS_MML_SoftReBoot();
        }   

        return VOS_TRUE;
    }

    /* ����״̬������ */    
    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_RcvLmmResumeInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    
    NAS_MMC_FSM_ID_ENUM_UINT32          enFsmId;

    /* ���Ϊ��ǰģʽ,������ϵͳ״̬����,�ٴ��յ�����ģʽ�Ļָ���Ϣ,ֱ�Ӹ�λ */
    enFsmId = NAS_MMC_GetCurrFsmId();
    if ( ( NAS_MMC_FSM_INTER_SYS_HO != enFsmId )
      && ( NAS_MMC_FSM_INTER_SYS_CELLRESEL != enFsmId )  
      && ( NAS_MMC_FSM_INTER_SYS_OOS != enFsmId ) 
      && ( NAS_MMC_FSM_INTER_SYS_CCO != enFsmId ))
    {
        if ( NAS_MML_NET_RAT_TYPE_LTE != NAS_MML_GetCurrNetRatType())
        {
            NAS_MML_SoftReBoot();
        }   

        return VOS_TRUE;
    }

    /* ����״̬������ */    
    return VOS_FALSE;
}


VOS_UINT32 NAS_MMC_RcvLmmInfoChangeNotifyInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    LMM_MMC_INFO_CHANGE_NOTIFY_STRU                        *pstRcvMsg = VOS_NULL_PTR;
    MMCGMM_EMERGENCY_NUM_LIST_IND_STRU                      stEmergencyNumList;
    MMA_MMC_NW_EMC_BS_CAP_ENUM_UINT8                        enNwEmcBS;
    MMA_MMC_NW_IMS_VOICE_CAP_ENUM_UINT8                     enNwImsVoPS;  
    MMA_MMC_LTE_CS_CAPBILITY_ENUM_UINT8                     enLteCsCap;

    pstRcvMsg = (LMM_MMC_INFO_CHANGE_NOTIFY_STRU *)pstMsg;

    PS_MEM_SET(((VOS_UINT8*)&stEmergencyNumList) + VOS_MSG_HEAD_LENGTH, 0, sizeof(MMCGMM_EMERGENCY_NUM_LIST_IND_STRU) - VOS_MSG_HEAD_LENGTH); 

    /* update LTE CS capability */
    NAS_MML_SetAdditionUpdateRslt((NAS_MML_ADDITION_UPDATE_RSLT_INFO_ENUM_UINT8)pstRcvMsg->enLteCsCap);

    enNwEmcBS = (MMA_MMC_NW_EMC_BS_CAP_ENUM_UINT8)pstRcvMsg->enNwEmcBS;

    enNwImsVoPS = (MMA_MMC_NW_IMS_VOICE_CAP_ENUM_UINT8)pstRcvMsg->enNwImsVoPS;

    NAS_MML_SetLteNwImsVoiceSupportFlg((NAS_MML_NW_IMS_VOICE_CAP_ENUM_UINT8)enNwImsVoPS);

    switch (pstRcvMsg->enLteCsCap)
    {
        case LMM_MMC_LTE_CS_CAPBILITY_NO_ADDITION_INFO:
            enLteCsCap = MMA_MMC_LTE_CS_CAPBILITY_NO_ADDITION_INFO;
            break;
            
        case LMM_MMC_LTE_CS_CAPBILITY_CSFB_NOT_PREFER:
            enLteCsCap = MMA_MMC_LTE_CS_CAPBILITY_CSFB_NOT_PREFER;
            break;
            
        case LMM_MMC_LTE_CS_CAPBILITY_SMS_ONLY:
            enLteCsCap = MMA_MMC_LTE_CS_CAPBILITY_SMS_ONLY;        
            break;
            
        case LMM_MMC_LTE_CS_CAPBILITY_NOT_ATTACHED:
        default:        
            enLteCsCap = MMA_MMC_LTE_CS_CAPBILITY_NOT_SUPPORTED;
            break;     
    }

    NAS_MMC_SndMmaNetworkCapabilityInfoInd(enNwImsVoPS, enNwEmcBS, enLteCsCap);

    /* ��GMM���ͽ������б� */
    stEmergencyNumList.ucOpEmcNumList       = VOS_TRUE;
    stEmergencyNumList.ucEmergencyNumAmount = pstRcvMsg->ucEmergencyNumAmount;
    PS_MEM_CPY(stEmergencyNumList.astEmergencyNumList, pstRcvMsg->astEmergencyNumList, 
                sizeof(LMM_MMC_EMERGENCY_NUM_STRU) * LMM_MMC_EMERGENCY_NUM_LIST_MAX_RECORDS);
    
    NAS_MMC_SndGmmEmergencyNumList(&stEmergencyNumList);

    return VOS_TRUE;
}

#endif

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

