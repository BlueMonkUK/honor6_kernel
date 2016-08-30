

/*****************************************************************************
   1 ͷ�ļ�����
*****************************************************************************/
#include "MM_Inc.h"
#include "MmGsmDifMsg.h"


#ifdef  __cplusplus
  #if  __cplusplus
  extern "C"{
  #endif
#endif


/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_MM_GSMDIFMSG_C



/*****************************************************************************
   2 ȫ�ֱ�������
*****************************************************************************/

/*****************************************************************************
   3 ����ʵ��
*****************************************************************************/


VOS_UINT8 MM_RcvMmcGsmSysInfoInd(VOS_VOID *pRcvMsg)
{

    /* ����ֵ���� */
    VOS_UINT8                           ucRet = MM_TRUE;

    /* ��ʱ�� T3212 ��ʱ���� */
    VOS_UINT32                          ulT3212Value;

    /* ������յ�����Ϣ����ָ�� */
    MMCMM_GSM_SYS_INFO_IND_ST          *pSysInfoInd;

    VOS_UINT8                           ucLAIChangeFlag = MM_FALSE;
    VOS_UINT32                          ulNewCellId;

    /* ��Ϣת�� */
    pSysInfoInd = (MMCMM_GSM_SYS_INFO_IND_ST *)pRcvMsg;

    /* ����ȫ��ϵͳ��Ϣ���ñ�־ */
    g_MmGlobalInfo.ucSysInfoFlg         = MM_TRUE;

    g_MmGlobalInfo.MsCsInfo.ulCurFobidnFlg = pSysInfoInd->ulForbiddenFlg;

    /* ���浱ǰ PlmnId �� Lac */
    g_MmGlobalInfo.MsCsInfo.TempLai.PlmnId.ulMcc
                = g_MmGlobalInfo.MsCsInfo.CurLai.PlmnId.ulMcc;
    g_MmGlobalInfo.MsCsInfo.TempLai.PlmnId.ulMnc
                = g_MmGlobalInfo.MsCsInfo.CurLai.PlmnId.ulMnc;
    g_MmGlobalInfo.MsCsInfo.TempLai.ulLac
                = g_MmGlobalInfo.MsCsInfo.CurLai.ulLac;

    ulNewCellId = pSysInfoInd->ulCellId;
    if (ulNewCellId != g_MmGlobalInfo.MsCsInfo.ulCellId)
    {
        g_MmGlobalInfo.MsCsInfo.ulCellId = ulNewCellId;
        g_MmGlobalInfo.MsCsInfo.ucCellChgFlg = MM_TRUE;
    }
    else
    {
        g_MmGlobalInfo.MsCsInfo.ucCellChgFlg = MM_FALSE;
    }

    /* �����������б� */
    NAS_MM_ProcEccNumList_RcvSysinfo(pRcvMsg);

    /* PlmnId �� Lac �б仯 */
    if ((g_MmGlobalInfo.MsCsInfo.CurLai.ulLac
                            != pSysInfoInd->usLac)
                            || (g_MmGlobalInfo.MsCsInfo.CurLai.PlmnId.ulMcc
                            != pSysInfoInd->stPlmnId.ulMcc)
                            || (g_MmGlobalInfo.MsCsInfo.CurLai.PlmnId.ulMnc
                            != pSysInfoInd->stPlmnId.ulMnc)
                            )
    {
        /* ���յ�����Ϣ�ṹ�л�ȡ�µ� PlmnId �� Lac */
        g_MmGlobalInfo.MsCsInfo.CurLai.PlmnId.ulMcc
                    = pSysInfoInd->stPlmnId.ulMcc;
        g_MmGlobalInfo.MsCsInfo.CurLai.PlmnId.ulMnc
                    = pSysInfoInd->stPlmnId.ulMnc;
        g_MmGlobalInfo.MsCsInfo.CurLai.ulLac
                    = pSysInfoInd->usLac;
        ucLAIChangeFlag = MM_TRUE;
        if ( MM_TRUE == g_MmGlobalInfo.ucLikeNetmode2 )
        {
            g_MmGlobalInfo.ucLikeNetmode2 = MM_FALSE;
            Mm_TimerStop( MM_TIMER_T3211 );
            Mm_TimerStop( MM_TIMER_T3212 );
        }
        /* ���� LikeB ��־ */
        if (MM_TRUE == g_MmGlobalInfo.ucLikeB)
        {
            g_MmGlobalInfo.ucLikeB = MM_FALSE;
        }
    }

    /* ���յ�����Ϣ�ṹ�л�ȡ�µ� T3212 ��ʱ���� */
    ulT3212Value    = pSysInfoInd->ucLRTimerT3212 * MM_T3212_UNIT_TRANSFORM;     /* 6����->���� */
    if (ulT3212Value > MM_TIMER_T3212_MAX_VALUE)
    {
        ulT3212Value = MM_TIMER_T3212_MAX_VALUE;
    }

    /* �Զ�ʱ���ȷ������� */
    if ((MM_CONST_NUM_0 == ulT3212Value)
     || (MM_CONST_NUM_0 == gstMmTimer[MM_TIMER_T3212].ulTimerLen))                /* Ϊ�� */
    {
        /* ���� T3212 ��ʱ����Ϊ��Чֵ */
        gstMmTimer[MM_TIMER_T3212].ulTimerLen = ulT3212Value;
        Mm_TimerStop(MM_TIMER_T3212);
    }
    else /* ����ֵ */
    {
        if (gstMmTimer[MM_TIMER_T3212].ulTimerLen != ulT3212Value)
        {
            Mm_ComT3212(ulT3212Value);
        }
    }

    /* ������Ϣ�� Att ���ݣ�����ȫ�� Att ��־ */
    if (MM_CONST_NUM_0 == pSysInfoInd->ucAttachDetach)
    {
        g_MmGlobalInfo.ucAttFlg         = MM_ATT_NOT_NEED;
    }
    else
    {
        g_MmGlobalInfo.ucAttFlg         = MM_ATT_NEED;
    }

    if (MM_CONST_NUM_0 != pSysInfoInd->ucGprsSupportInd)
    {
        if ( MM_FALSE == ucLAIChangeFlag )
        {
            if ( g_MmGlobalInfo.usRac != pSysInfoInd->ucRac )
            {
                if ( MM_TRUE == g_MmGlobalInfo.ucLikeNetmode2 )
                {
                    g_MmGlobalInfo.ucLikeNetmode2 = MM_FALSE;
                    Mm_TimerStop( MM_TIMER_T3211 );
                    Mm_TimerStop( MM_TIMER_T3212 );
                }
            }
        }
        MM_AnalyRacofSysInfo(pSysInfoInd->ucRac);

        if (pSysInfoInd->ucNetworkOperationMode < MM_CONST_NUM_4)
        {
            MM_AnalyNtModeofSysInfo(pSysInfoInd->ucNetworkOperationMode);
        }
        else
        {
            NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_ERROR,
                        "MM_RcvMmcGsmSysInfoInd: ERROR: Network Operation Mode is unexpected!.");
        }
    }
    else
    {
        g_MmGlobalInfo.usRac = MM_UNSUPPORT_GPRS;
        g_MmGlobalInfo.ucNtMod          = MM_NET_MODE_INVALID;
        g_MmGlobalInfo.ucNewNtMod       = MM_NET_MODE_INVALID;
    }

    if (GRRMM_MSC_RELEASE98_OR_OLDER == pSysInfoInd->ucMsrRelease)
    {
        g_stMmNsd.ucNsdMod = MM_NSD_MOD_R98_OR_OLDER;
    }
    else
    {
        g_stMmNsd.ucNsdMod = MM_NSD_MOD_R99_ONWARDS;
    }

    /* ���ط������ */
    return ucRet;
}


VOS_VOID MM_AnalyRacofSysInfo(VOS_UINT8 ucRac)
{
    if (g_MmGlobalInfo.usRac != ucRac) /* Rac �иı� */
    {
        g_MmGlobalInfo.usRac = ucRac;

        g_MmGlobalInfo.ucLikeB = MM_FALSE;
    }

    return;
}
VOS_VOID MM_AnalyNtModeofSysInfo(VOS_UINT8 ucNetworkOperationMode)
{
    VOS_UINT8                           ucSimPsRegStatus;
    VOS_UINT8                           ucPsAttachAllow;

    ucSimPsRegStatus = NAS_MML_GetSimPsRegStatus();
    ucPsAttachAllow  = NAS_MML_GetPsAttachAllowFlg();

    /* ����ģʽ��Ϣ��Ч */
    if (MM_CONST_NUM_0 != ucNetworkOperationMode)
    {
        /*if (MM_CONST_NUM_3 == ucNetworkOperationMode)
        {
            ucNetworkOperationMode      = MM_CONST_NUM_2;
        }*/

        /* ��ȡ ucNewNtMod ֵ */
        g_MmGlobalInfo.ucNewNtMod = ucNetworkOperationMode;

        /* �����ǰ����ģʽδ֪,����Ϊ�»�ȡ����ģʽ */
        if (MM_CONST_NUM_0 == g_MmGlobalInfo.ucNtMod)
        {
            g_MmGlobalInfo.ucNtMod      = g_MmGlobalInfo.ucNewNtMod;
        }
    }
    else
    {
        NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_WARNING,
                        "MM_AnalyNtModeofSysInfo: WARNING: Net Mode is unexpected!.");

        g_MmGlobalInfo.ucNewNtMod = MM_NET_MODE_II;

        /* �����ǰ����ģʽδ֪,����Ϊ�»�ȡ����ģʽ */
        if (MM_CONST_NUM_0 == g_MmGlobalInfo.ucNtMod)
        {
            g_MmGlobalInfo.ucNtMod      = g_MmGlobalInfo.ucNewNtMod;
        }
    }

    /* ���������ģʽ I �£��� PS ҵ�񲻿��ã����ܷ������Ϲ��� */
    if (MM_NET_MODE_I == g_MmGlobalInfo.ucNewNtMod)
    {
        if ((VOS_TRUE != ucSimPsRegStatus)
            || (VOS_FALSE == ucPsAttachAllow))
        {
            /* ���ɷ������Ϲ��̱�־ */
            g_MmGlobalInfo.ucLikeB      = MM_TRUE;
        }
    }

    return;
}
VOS_UINT8 MM_RcvGsmRrcPagingInd(VOS_VOID *pRcvMsg)
{

    /* ����ֵ���� */
    VOS_UINT8                           ucRet = MM_TRUE;

    /* ������յ�����Ϣ����ָ�� */
    GRRMM_PAGING_IND_ST                *pMsg;

    /* ��Ϣת�� */
    pMsg = (GRRMM_PAGING_IND_ST *)pRcvMsg;

    /* ����Ϣ�Ϸ��Լ�� */
    if ((MM_CONST_NUM_1 < pMsg->ucPagingType)
        || (MM_CONST_NUM_2 < pMsg->ucMSIDType)) /* ��Ϣ�е�ȡֵ���Ϸ� */
    {
        /* ������Ϣ���ʧ�� */
        ucRet = MM_FALSE;

        /* ��ӡ������Ϣ */
        NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_ERROR,
                    "MM_RcvGsmRrcPagingInd: ERROR: Receive GRRMM_PAGING_IND error.");

    }
    else /* ��Ϣ�Ϸ� */
    {
        /* �洢��Ϣ�������Ϣ */
        if ((GAS_TMSI == pMsg->ucMSIDType)
            || (GAS_PTMSI == pMsg->ucMSIDType)) /* ��������Ϊ GAS_TMSI �� GAS_PTMSI */
        {
            /* ����ȫ�ֺ�������Ϊ MM_TMSI_PAGING */
            g_MmGlobalInfo.ucPagingRecordTypeId = MM_TMSI_PAGING;
        }
        else if (GAS_IMSI == pMsg->ucMSIDType) /* ��������Ϊ GAS_IMSI */
        {
            /* ����ȫ�ֺ�������Ϊ MM_IMSI_PAGING */
            g_MmGlobalInfo.ucPagingRecordTypeId = MM_IMSI_PAGING;
        }
        else
        {

        }
    }

    /* ���ؼ���� */
    return ucRet;
}

/* 4 GSM��Ϣ GAS_RR_SYNC_IND ��ʵ�� */
/* �޸� MAPS1000 �к��� Mm_RcvRrcSyncInd */
/*VOS_UINT8 Mm_RcvRrcSyncInd(void *pRcvMsg             �յ���ԭ��ͷָ�� * ) */
/* ���ļ� Mm_Rcv.c                                                            */


VOS_UINT8 MM_RcvGsmRrcSyncInd(VOS_VOID *pRcvMsg)
{

    /* ����ֵ���� */
    VOS_UINT8                           ucRet = MM_TRUE;

    /* ������յ�����Ϣ����ָ�� */
    GAS_RR_SYNC_IND_ST                 *pMsg;

    /* ��Ϣת�� */
    pMsg = (GAS_RR_SYNC_IND_ST *)pRcvMsg;

    /* ��ͬ����Ϣ�ķ������� */
    if (RR_SYN_CIPH == pMsg->ucSynCause) /* �����Ѿ����� */
    {
        g_RrMmSyncInd.ulRabCnt                              = MM_CONST_NUM_0;

        g_MmGlobalInfo.CsSecutityInfo.ucSecurityType        = RR_SYN_CIPH;

        /*g_MmGlobalInfo.CsSecutityInfo.ucSecutityFlg         = RRC_CIPHER_START;*/

        /* ���� ucSynValue ����ȫ�ּ����㷨 */
        g_MmGlobalInfo.ucCsCipherAlgor                      = pMsg->ucSynValue;
    }
#if 0
    else if (RR_SYN_TCH == pMsg->ucSynCause) /* ҵ��ͨ���Ѿ�ָ�� */
    {
        NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_NORMAL,
                    "MM_RcvGsmRrcSyncInd: NORMAL: Sync channal mode res.");

        g_MmGlobalInfo.CsSecutityInfo.ucSecurityType        = RR_SYN_TCH;
    }
    else if (RR_SYN_TCH_MODIFY == pMsg->ucSynCause) /* ҵ��ͨ��ģʽ�޸� */
    {
        NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_NORMAL,
                    "MM_RcvGsmRrcSyncInd: NORMAL: Sync channal mode change.");

        g_MmGlobalInfo.CsSecutityInfo.ucSecurityType        = RR_SYN_TCH_MODIFY;
    }
    else if ((RR_SYN_GSM_GSM_HANDOVER == pMsg->ucSynCause)
          || (RR_SYN_WCDMA_GSM_HANDOVER == pMsg->ucSynCause))
    {
        NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_NORMAL,
                    "MM_RcvGsmRrcSyncInd: NORMAL: Sync channal mode change.");

        g_MmGlobalInfo.CsSecutityInfo.ucSecurityType        = RR_SYN_TCH_MODIFY;
    }
    else /* ������� */
    {
        NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_ERROR,
                    "MM_RcvGsmRrcSyncInd: ERROR: Sync cause is unexpected!");

        ucRet = MM_FALSE;
    }

    if ((MM_FALSE != ucRet) && (RR_SYN_CIPH != pMsg->ucSynCause))
    {
        g_RrMmSyncInd.ulRabCnt                     = MM_CONST_NUM_1;
        g_RrMmSyncInd.NasSyncInfo[0].ulRabId       = MM_CONST_NUM_1;
        g_RrMmSyncInd.NasSyncInfo[0].ulCnDomainId  = RRC_NAS_CS_DOMAIN;
        /* ��MMCC_SYNC_IND��Ϣ��ԭ��ֵ����һ�� */
        g_RrMmSyncInd.NasSyncInfo[0].ulReason      = (VOS_UINT32)(pMsg->ucSynCause - 1);
        g_RrMmSyncInd.ulChannelMode                = (VOS_UINT32)pMsg->ucSynValue;
        g_RrMmSyncInd.ulTchRate                    = (VOS_UINT32)pMsg->ucTchRate;

        /* �Ƿ�����Ϣ�е�ͬ��ԭ�򣬴������� */
        g_RrMmSyncInd.NasSyncInfo[0].ulRabSyncInfo = MM_CONST_NUM_1;
    }
#endif

    /* ���ؽ�� */
    return ucRet;
}
VOS_UINT32 MM_SupplementNsdInfo(VOS_UINT8 *pData)
{
    VOS_UINT8 ucPd;

    ucPd = pData[0] & MM_IE_PD_MASK;
    if ((MM_IE_PD_MM_MSG == ucPd) || (MM_IE_PD_CALL_CONTROL == ucPd)
       || (MM_IE_PD_NON_CALL_RLT_SS_MSG == ucPd))
    {
        pData[1] |= (VOS_UINT8)(g_stMmNsd.ucNsd << 6);
        return MM_TRUE;
    }

    return MM_FALSE;
}


VOS_INT32 MM_WasRrDataReq(VOS_UINT8 ucCnDomain, VOS_UINT8 ucPriority,
                          VOS_UINT32 ulSize, VOS_INT8 *pData)
{

    VOS_INT32                           lRet;
    VOS_UINT32                          ulRet = MM_FALSE;

    if (VOS_NULL_PTR != pData)
    {
        ulRet = MM_SupplementNsdInfo((VOS_UINT8 *)pData);
    }

    lRet = As_RrDataReq(ucCnDomain, ucPriority, ulSize, pData);

    if ((MM_TRUE == ulRet) && (MM_TRUE == lRet))
    {
        g_stMmNsd.ucNsd ++;
        g_stMmNsd.ucNsd = g_stMmNsd.ucNsd % g_stMmNsd.ucNsdMod;
    }

    return lRet;
}



VOS_INT32 MM_GasRrDataReq(VOS_UINT8 ucCnDomain, VOS_UINT8 ucPriority,
                          VOS_UINT32 ulSize, VOS_INT8 *pData)
{
    /* �������VOS���ͺ����ķ���ֵ               */
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulTmp = MM_FALSE;

    /* �������ݳ���                              */
    VOS_UINT32                          ulTmpSize;

    /* ����ԭ������ָ��                          */
    RRMM_DATA_REQ_STRU                 *pMsg;

    /* ���β��л�����ݳ���                      */
    ulTmpSize       = ulSize;
    /* ���ٷ����ĸ��ֽ�����                      */
    if (ulTmpSize < 4)
    {
        ulTmpSize   = 4;
    }

    /* PS ����������                             */
    if (RRC_NAS_PS_DOMAIN == ucCnDomain)
    {
        /* ��ӡ������Ϣ */
        NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_ERROR,
                    "MM_GasRrDataReq: ERROR: Ps data req.");

        /* ����                                  */
        return MM_FALSE;
    }

    /* CS ����������                             */
    /* �����ڴ�ռ�                              */
    pMsg = (RRMM_DATA_REQ_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_MM,
                       (sizeof(RRMM_DATA_REQ_STRU) + ulTmpSize) - 4);

    /* �����ڴ�ʧ��                              */
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ������Ϣ */
        NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_ERROR,
                    "MM_GasRrDataReq: ERROR: Alloc memory fail.");

        /* ����                                  */
        return MM_FALSE;
    }

    /* �����Ϣͷ                                */
    pMsg->MsgHeader.ulReceiverCpuId     = VOS_LOCAL_CPUID;
    pMsg->MsgHeader.ulReceiverPid       = UEPS_PID_GAS;
    pMsg->MsgHeader.ulMsgName           = RRMM_DATA_REQ;

    /* CN ��                                     */
    pMsg->ulCnDomainId                  = ucCnDomain;
    /* ���ȼ�                                    */
    pMsg->ulPriorityInd                 = ucPriority;
    /* ���ݳ���                                  */
    pMsg->SendNasMsg.ulNasMsgSize       = ulSize;
    /* �����ݿ�������Ϣ��                        */

    if (VOS_NULL_PTR != pData)
    {
        PS_MEM_CPY(pMsg->SendNasMsg.aucNasMsg, pData, ulSize);
        ulTmp = MM_SupplementNsdInfo(pMsg->SendNasMsg.aucNasMsg);
    }

    /* ������Ϣ                                  */
    ulRet = PS_SEND_MSG(WUEPS_PID_MM, pMsg);
    /* ����ʧ��                                  */
    if (VOS_OK != ulRet)
    {
        /* ��ӡ������Ϣ */
        NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_ERROR,
                    "MM_GasRrDataReq: ERROR: Send Msg fail.");

        /* ����                                  */
        return MM_FALSE;
    }

    if (MM_TRUE == ulTmp)
    {
        g_stMmNsd.ucNsd ++;
        g_stMmNsd.ucNsd = g_stMmNsd.ucNsd % g_stMmNsd.ucNsdMod;
    }

    /* ����                                      */
    return MM_TRUE;
}
VOS_INT32 MM_WasRrEstReq(VOS_UINT32 ulOpId, VOS_UINT8 ucCnDomain,
                         VOS_UINT32 ulEstCause, IDNNS_STRU *pIdnnsInfo, RRC_PLMN_ID_STRU *pstPlmnId,
                         VOS_UINT32 ulSize, VOS_INT8 *pFisrstMsg)
{
    VOS_INT32                           lRet;
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulCallType;

    g_stMmNsd.ucNsd = MM_NSD_INITAL_VALUE;

    ulRet = MM_FALSE;
    if (VOS_NULL_PTR != pFisrstMsg)
    {
        ulRet = MM_SupplementNsdInfo((VOS_UINT8 *)pFisrstMsg);
    }

    if ( ( RRC_EST_CAUSE_ORIGIN_CONVERSAT_CALL == ulEstCause )
      || ( RRC_EST_CAUSE_CALL_RE_ESTABLISH == ulEstCause ))
    {
        ulCallType = g_MmCcEstReq.ulCallMode;
    }
    else if (( RRC_EST_CAUSE_REGISTRATION == ulEstCause )
          || ( RRC_EST_CAUSE_DETACH == ulEstCause ))
    {
        ulCallType = RRC_NAS_CALL_TYPE_OTHER;
    }
    else
    {
        ulCallType = RRC_NAS_CALL_TYPE_SPEECH;
    }

    lRet = As_RrEstReq(ulOpId, ucCnDomain, ulEstCause,ulCallType, pIdnnsInfo, pstPlmnId, ulSize, pFisrstMsg);

    if ((MM_TRUE == ulRet) && (MM_TRUE == lRet))
    {
        g_stMmNsd.ucNsd ++;
    }

    return lRet;
}
VOS_INT32 MM_GasRrEstReq(VOS_UINT32 ulOpId, VOS_UINT8 ucCnDomain,
                         VOS_UINT32 ulEstCause, IDNNS_STRU *pIdnnsInfo, RRC_PLMN_ID_STRU *pstPlmnId,
                         VOS_UINT32 ulSize, VOS_INT8 *pFisrstMsg)
{
    /* �������VOS���ͺ����ķ���ֵ               */
    VOS_UINT32                          ulRet;

    /* ����ԭ������ָ��                          */
    RRMM_EST_REQ_STRU                  *pMsg;

    /* �������ݳ���                              */
    VOS_UINT32                          ulTmpSize;

    /* ���β��л�����ݳ���                      */
    ulTmpSize = ulSize;
    /* ���ٷ����ĸ��ֽ�����                      */
    if (ulTmpSize < 4)
    {
        ulTmpSize = 4;
    }

    /* PS ����������                             */
    if (RRC_NAS_PS_DOMAIN == ucCnDomain)
    {
        /* ��ӡ������Ϣ */
        NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_ERROR,
                    "MM_GasRrEstReq: ERROR: Ps data req.");

        /* ����                                  */
        return MM_FALSE;
    }

    /* �����ڴ�ռ�                              */
    pMsg = (RRMM_EST_REQ_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_MM,
                       (sizeof(RRMM_EST_REQ_STRU) + ulTmpSize) - 4);
    if ( VOS_NULL_PTR == pMsg )
    {
        /* ��ӡ������Ϣ */
        NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_ERROR,
                    "MM_GasRrEstReq: ERROR: Alloc memory fail.");

        /* ����                                  */
        return MM_FALSE;
    }

    /* �����Ϣͷ                                */
    pMsg->MsgHeader.ulReceiverCpuId     = VOS_LOCAL_CPUID;
    pMsg->MsgHeader.ulReceiverPid       = UEPS_PID_GAS;
    pMsg->MsgHeader.ulMsgName           = RRMM_EST_REQ;

    /* ������ʶ                                  */
    pMsg->ulOpId                        = ulOpId;
    /* CN ��                                     */
    pMsg->ulCnDomainId                  = ucCnDomain;
    /* ����ԭ��                                  */
    pMsg->ulEstCause                    = ulEstCause;
    pMsg->enCallType                    = RRC_NAS_CALL_TYPE_SPEECH;
    /* IDDN����,GAS��ʹ�ã����ֽӿ�һ�� */
    pMsg->ulIdnnsType                   = pIdnnsInfo->ucIdnnsType;
    pMsg->enSplmnSameRplmn              = pIdnnsInfo->ucEnterPara;

    PS_MEM_CPY(&pMsg->stPlmnId, pstPlmnId, sizeof(RRC_PLMN_ID_STRU));

    /* NAS ��һ����Ϣ����                        */
    pMsg->FisrtNasMsg.ulNasMsgSize      = ulSize;
    /* �����ݿ�������Ϣ��                        */
    if (VOS_NULL_PTR != pFisrstMsg)
    {
        PS_MEM_CPY(pMsg->FisrtNasMsg.aucNasMsg, pFisrstMsg, ulSize);
    }
    else
    {
        PS_MEM_SET(pMsg->FisrtNasMsg.aucNasMsg, 0x00, 4);
    }

    /* ���ú���������Ϣ                          */
    ulRet = PS_SEND_MSG(WUEPS_PID_MM, pMsg);
    /* ����ʧ��                                  */
    if (VOS_OK != ulRet)
    {
        /* ��ӡ������Ϣ */
        NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_ERROR,
                    "MM_GasRrEstReq: ERROR: Send Msg fail.");

        return MM_FALSE;
    }

    g_stMmNsd.ucNsd = MM_NSD_INITAL_VALUE;

    if (VOS_NULL_PTR != pFisrstMsg)
    {
        g_stMmNsd.ucNsd ++;
    }

    /* �ɹ�����                                  */
    return MM_TRUE;
}


VOS_INT32 MM_GasRrRelReq(VOS_UINT8 ucCnDomain,RRC_CELL_BAR_ENUM_UINT32 enBarValidFlg)
{
    /* �������VOS���ͺ����ķ���ֵ               */
    VOS_UINT32                          ulRet;

    /* ����ԭ������ָ��                          */
    RRMM_REL_REQ_STRU                  *pMsg;

    /* PS ����������                             */
    if (RRC_NAS_PS_DOMAIN == ucCnDomain)
    {
        NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_ERROR,
                    "MM_GasRrRelReq: ERROR: Ps release RR connect req.");

        /* ����                                  */
        return MM_FALSE;
    }

    /* �����ڴ�ռ�                              */
    pMsg = (RRMM_REL_REQ_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_MM,
                       sizeof(RRMM_REL_REQ_STRU));
    if (VOS_NULL_PTR == pMsg)
    {
        /* ��ӡ������Ϣ */
        NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_ERROR,
                    "MM_GasRrRelReq: ERROR: Alloc memory fail.");

        /* ����                                  */
        return MM_FALSE;
    }

    /* �����Ϣͷ                                */
    pMsg->MsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pMsg->MsgHeader.ulReceiverPid   = UEPS_PID_GAS;
    pMsg->MsgHeader.ulMsgName           = RRMM_REL_REQ;

    /* CN ��                                     */
    pMsg->ulCnDomainId                  = ucCnDomain;
    /* ��ǰС���򼤻С��������־ */
    pMsg->enBarValidFlg                 = enBarValidFlg;
    /* ���ú���������Ϣ                          */
    ulRet = PS_SEND_MSG(WUEPS_PID_MM, pMsg);
    /* ����ʧ��                                  */
    if (VOS_OK != ulRet)
    {
        /* ��ӡ������Ϣ */
        NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_ERROR,
                    "MM_GasRrRelReq: ERROR: Send Msg fail.");

        /* ����                                  */
        return MM_FALSE;
    }

    /* �ɹ�����                                  */
    return MM_TRUE;
}


/* 6.MM ʵ���ṩ�� GAS ��ȡ��ȫ��Ϣ��API���� */


VOS_VOID MM_GetSecurityInfo(MM_CSPS_INFO_ST *pCsInfo)
{
    VOS_UINT8                          *pucImsi = VOS_NULL_PTR;

    if (VOS_NULL_PTR == pCsInfo)
    {
        NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_ERROR,
                    "MM_GetSecurityInfo: ERROR: Para check error!");

        return;
    }

    /* ��ȫ�ֽṹ����Ӧ��Ϣ�������ṹ pCsInfo �� */
    /* ��ʼ���ṹ�� InfoMask ���� */
    pCsInfo->ucInfoMask = 0x0;

    if (VOS_FALSE == NAS_MML_GetSimPresentStatus()) /* �������� */
    {
        NAS_LOG(WUEPS_PID_MM, MM_GSMDIFMSG, PS_PRINT_INFO,
                    "MM_GetSecurityInfo: INFO: SIM is not present!");

        return;
    }

    /* ���� Ck��Ik �� Kc */
    pCsInfo->ucInfoMask |= 0x1F;
    PS_MEM_CPY(pCsInfo->aucCk, NAS_MML_GetSimCsSecurityUmtsCk(), NAS_MML_UMTS_CK_LEN);

    PS_MEM_CPY(pCsInfo->aucIk, NAS_MML_GetSimCsSecurityUmtsIk(), NAS_MML_UMTS_IK_LEN);

    PS_MEM_CPY(pCsInfo->aucKc, NAS_MML_GetSimCsSecurityGsmKc(), NAS_MML_GSM_KC_LEN);

    /* ��ȡ IMSI */
    pucImsi = NAS_MML_GetSimImsi();
    PS_MEM_CPY(pCsInfo->aucImsi, pucImsi + 1, RRC_NAS_IMSI_MAX_LEN);

    pCsInfo->ucCksn = NAS_MML_GetSimCsSecurityCksn();
    if ( MM_MS_ID_TMSI_PRESENT & g_MmGlobalInfo.MsCsInfo.MobileId.ucMsIdFlg )
    {
        pCsInfo->ucInfoMask |= MM_SECURITY_P_TMSI_MASK;

        PS_MEM_CPY(pCsInfo->aucPTMSI,
                   NAS_MML_GetUeIdTmsi(),
                   NAS_MML_MAX_TMSI_LEN);
    }

    return;
}




#ifdef  __cplusplus
  #if  __cplusplus
  }
  #endif
#endif