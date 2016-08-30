
/*lint --e{740,537,958,718,746,767,553,516,701}*/

#include "si_pih.h"
#include "UsimPsInterface.h"
#include "usimmbase.h"
#include "apminterface.h"
#include "AtOamInterface.h"
#include "om.h"
#include "softcrypto.h"
#include "FileSysInterface.h"
#include "OamXmlComm.h"
#include "csimagent.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/*****************************************************************************
    Э��ջ��ӡ��㷽ʽ�µ�.C�ļ��궨��
*****************************************************************************/
/*lint -e767*/
#define    THIS_FILE_ID PS_FILE_ID_PIH_PROC_C
/*lint +e767*/


SI_PIH_CTRL_INFO_STRU       g_stPIHCtrlInfo;

SI_PIH_POLL_TIME_STRU       g_stPIHPollTime[3];

SI_PIH_PROTECT_CTRL_STRU    g_stPIHProtectCtrl;

SI_PIH_CSIM_CTRL_STRU       g_stPIHCSIMCtrlInfo;

/*����Э��ջע���USIM��״̬��Ϣ,��ʼ��Ϊ0,����PID��ʼ������ʱ������*/
VOS_UINT32                  g_aulPIHUsimBCPid[SI_PIH_BCPID_REG_MAX] = {0};

/*����Э��ջע���Refresh��״̬��Ϣ,��ʼ��Ϊ0,����PID��ʼ������ʱ������*/
VOS_UINT32                  g_aulPIHRefreshBCPid[SI_PIH_BCPID_REG_MAX] = {0};

/*����Э��ջע��isim��״̬��Ϣ,��ʼ��Ϊ0,����PID��ʼ������ʱ������*/
VOS_UINT32                  g_aulPIHISIMBCPid[SI_PIH_BCPID_REG_MAX] = {0};

#if (FEATURE_ON == FEATURE_VSIM)
/*��¼��ǰ�ȴ�USIMȥ����ĳ��Դ���*/
VOS_UINT32                  g_ulPIHDeactiveRetryCnt     = 0;
HTIMER                      g_stPIHDeactiveCardTimer    = VOS_NULL_PTR;

SI_PIH_FWRITE_PARA_STRU     g_stPIHFileWriteGlobal;

#endif  /*(FEATURE_ON == FEATURE_VSIM)*/

/*****************************************************************************
�� �� ��  : SI_PIH_PollTimerInit
��������  : PIH��ʱ�����ܳ�ʼ��ʱ��ʼ��������
�������  : ��
�������  : ��
�� �� ֵ  : VOS_OK
���ú���  :
��������  :
�޶���¼  :
1.��    ��  : 2010��07��26��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_VOID SI_PIH_PollTimerInit(SI_VOID)
{
    USIMM_FEATURE_CFG_STRU              stUsimmFeatureCfg;

    g_stPIHPollTime[SI_PIH_TIMER_NAME_CHECKSTATUS].ulTimeLen    = SI_PIH_POLL_TIMER_LEN;
    g_stPIHPollTime[SI_PIH_TIMER_NAME_CHECKSTATUS].ulTimerName  = SI_PIH_TIMER_NAME_CHECKSTATUS;
    g_stPIHPollTime[SI_PIH_TIMER_NAME_CHECKSTATUS].enPollState  = SI_PIH_POLLTIMER_ENABLE;

    g_stPIHPollTime[SI_PIH_TIMER_NAME_CALL].ulTimeLen           = SI_PIH_CALL_TIMER_LEN;
    g_stPIHPollTime[SI_PIH_TIMER_NAME_CALL].ulTimerName         = SI_PIH_TIMER_NAME_CALL;
    g_stPIHPollTime[SI_PIH_TIMER_NAME_CALL].enPollState         = SI_PIH_POLLTIMER_DISABLE;

    /* Ĭ�ϲ���ѯ */
    g_stPIHPollTime[SI_PIH_TIMER_NAME_CHECKIMSI].ulTimeLen      = SI_PIH_IMSI_TIMER_LEN;
    g_stPIHPollTime[SI_PIH_TIMER_NAME_CHECKIMSI].ulTimerName    = SI_PIH_TIMER_NAME_CHECKIMSI;
    g_stPIHPollTime[SI_PIH_TIMER_NAME_CHECKIMSI].enPollState    = SI_PIH_POLLTIMER_DISABLE;

    if (VOS_OK != NV_Read(en_NV_Item_Usim_Support_Feature_Config,
                         (VOS_VOID*)&stUsimmFeatureCfg, sizeof(stUsimmFeatureCfg)))
    {
        return;
    }

    if (VOS_TRUE == stUsimmFeatureCfg.unCfg.stFeatureCfg.ulImsiPolling)
    {
        g_stPIHPollTime[SI_PIH_TIMER_NAME_CHECKIMSI].enPollState= SI_PIH_POLLTIMER_ENABLE;
    }

    return;
}

/*****************************************************************************
�� �� ��  : WuepsPIHPidInit
��������  : PIH���ܳ�ʼ��ʱ��ʼ��������
�������  : ��
�������  : ��
�� �� ֵ  : VOS_OK
���ú���  :
��������  :
�޶���¼  :
1.��    ��  : 2010��07��26��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_UINT32 WuepsPIHPidInit(enum VOS_INIT_PHASE_DEFINE InitPhrase)
{
    switch( InitPhrase )
    {
        case   VOS_IP_LOAD_CONFIG:
            VOS_MemSet(&g_stPIHCtrlInfo, 0, sizeof(g_stPIHCtrlInfo));
            VOS_MemSet(g_stPIHPollTime, 0, sizeof(g_stPIHPollTime));
            VOS_MemSet(&g_stPIHProtectCtrl, 0, sizeof(g_stPIHProtectCtrl));
            VOS_MemSet(&g_stPIHCSIMCtrlInfo, 0, sizeof(g_stPIHCSIMCtrlInfo));
#if (FEATURE_ON == FEATURE_VSIM)
            VOS_MemSet(&g_stPIHFileWriteGlobal, 0, sizeof(g_stPIHFileWriteGlobal));
#endif  /*(FEATURE_ON == FEATURE_VSIM)*/
            g_stPIHProtectCtrl.enProtectFun = SI_PIH_PROTECT_ENABLE;

            SI_PIH_PollTimerInit();

            break;

        case   VOS_IP_RESTART:
            /* A�˲���ע�ᣬPCSC��ע����PIH����� */
            PIH_RegUsimCardStatusIndMsg(ACPU_PID_PCSC);
            break;

        default:
            break;
    }

    return VOS_OK;
}

/*****************************************************************************
�� �� ��  : SI_PIH_MNTNDataHook
��������  : PIH��ά�ɲ����ݹ���
�������  : ulHookMsgName:��������Ϣ����
            ulHookDataLen:���ݳ���
            pucHookData:��������
�������  : ��
�� �� ֵ  : ��
���ú���  :
��������  :
�޶���¼  :
1.��    ��  : 2013��06��14��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_VOID SI_PIH_MNTNDataHook(VOS_UINT32               ulHookMsgName,
                                        VOS_UINT32              ulHookDataLen,
                                        VOS_UINT8               *pucHookData)
{
    SI_PIH_HOOK_MSG_STRU    *pstMsg;

    pstMsg = (SI_PIH_HOOK_MSG_STRU *)VOS_AllocMsg(MAPS_PIH_PID,
                                                  (sizeof(SI_PIH_HOOK_MSG_STRU) - VOS_MSG_HEAD_LENGTH)+ulHookDataLen);

    if (VOS_NULL_PTR == pstMsg)
    {
        PIH_WARNING_LOG("SI_PIH_MNTNDataHook: Alloc Msg is Failed");

        return;
    }

    pstMsg->ulReceiverPid   = MAPS_PIH_PID;
    pstMsg->ulMsgName       = ulHookMsgName;

    if (VOS_NULL != ulHookDataLen)
    {
        VOS_MemCpy(pstMsg->aucContent, pucHookData, ulHookDataLen);
    }

    (VOS_VOID)VOS_SendMsg(MAPS_PIH_PID, pstMsg);

    return;
}


VOS_UINT32 SI_PIH_ErrorCheck(VOS_VOID)
{
    if( SI_PIH_LOCKED == g_stPIHCtrlInfo.enPIHLock )
    {
        PIH_WARNING_LOG("SI_PIH_ErrorProc: The PIH is Locked");

        return TAF_ERR_SIM_BUSY;
    }
    else
    {
        return VOS_OK;
    }
}

/*****************************************************************************
�� �� ��  : SI_PIH_PollTimerPro
��������  : ����PIH��ʱ����صĿ������޸�ʱ���ȹ���
�������  : pstPIHTimer:��ʱ���ṹ
�������  : ��
�� �� ֵ  : VOS_OK
���ú���  :
��������  :
�޶���¼  :
1.��    ��  : 2010��07��26��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_VOID SI_PIH_PollTimerPro(SI_PIH_POLL_TIME_STRU *pstPIHTimer)
{
    VOS_UINT32 ulResult = VOS_OK;
    /* static VOS_UINT32 ulTimerType; */

    if(pstPIHTimer->stTimer != VOS_NULL_PTR)        /*��ʱ���������У���Ҫ��ֹͣ*/
    {
        PIH_NORMAL_LOG("SI_PIH_PollTimerPro: Need Stop Timer First.");

        if(SI_PIH_TIMER_NAME_CHECKSTATUS == pstPIHTimer->ulTimerName)
        {

			ulResult = VOS_StopRelTimer(&pstPIHTimer->stTimer);
        }
        else
        {
            ulResult = VOS_StopRelTimer(&pstPIHTimer->stTimer);
        }
    }

    if(pstPIHTimer->enPollState == SI_PIH_POLLTIMER_ENABLE) /*��ʱ����Ҫ����*/
    {
        PIH_NORMAL_LOG("SI_PIH_PollTimerPro: Need Start Timer First.");

        if (SI_PIH_TIMER_NAME_CHECKSTATUS == pstPIHTimer->ulTimerName)
        {
            
			ulResult = VOS_StartRelTimer(&pstPIHTimer->stTimer, MAPS_PIH_PID,
                                                            pstPIHTimer->ulTimeLen, pstPIHTimer->ulTimerName, 0,VOS_RELTIMER_NOLOOP, VOS_TIMER_PRECISION_0);
        }
        else
        {
         /* coverity[example_assign] */
            ulResult = SI_PIH_POLL_TIMER_START(&pstPIHTimer->stTimer, pstPIHTimer->ulTimeLen, pstPIHTimer->ulTimerName);
        }
    }
    /* coverity[example_checked] */
    if(ulResult != VOS_OK)
    {
        PIH_ERROR1_LOG("SI_PIH_PollTimerPro: Handle Timer Name %d Error.", (VOS_INT32)pstPIHTimer->ulTimerName);
    }

    return;
}

/*****************************************************************************
�� �� ��  :
��������  :
�������  : ��
�������  : ��
�� �� ֵ  : SI_UINT32 ����ִ�н��
���ú���  : ��
��������  : �ⲿ�ӿ�
History     :
1.��    ��  : 2010��08��20��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_UINT32 SI_PIH_EventCallBack(SI_PIH_EVENT_INFO_STRU  *pstEvent)
{
    pstEvent->ClientId  = g_stPIHCtrlInfo.usClient;
    pstEvent->EventType = g_stPIHCtrlInfo.ulEventType;
    pstEvent->OpId      = g_stPIHCtrlInfo.ucOpID;

    VOS_MemSet(&g_stPIHCtrlInfo, 0, sizeof(g_stPIHCtrlInfo));

    return SI_PIHCallback(pstEvent);
}


VOS_VOID SI_PIH_PCSCCmdCnf(VOS_UINT32 ulCmdType, VOS_UINT32 ulResult,
                                    VOS_UINT32 ulLen, VOS_UINT8 *pucContent)
{
    SI_PIH_PCSC_CNF_STRU  *pstMsg;

    pstMsg = (SI_PIH_PCSC_CNF_STRU *)VOS_AllocMsg(MAPS_PIH_PID,
                    ((sizeof(SI_PIH_PCSC_CNF_STRU) - VOS_MSG_HEAD_LENGTH) - 4) + ulLen);

    if (VOS_NULL_PTR == pstMsg)
    {
        PIH_ERROR_LOG("USIMM_PCSCCmdCnf: VOS_AllocMsg is Failed");

        return;
    }

    pstMsg->ulReceiverPid = ACPU_PID_PCSC;
    pstMsg->ulMsgName     = SI_PIH_PCSC_DATA_CNF;
    pstMsg->ulResult      = ulResult;
    pstMsg->ulCmdType     = ulCmdType;
    pstMsg->ulRspLen      = ulLen;

    if (VOS_NULL_PTR != pucContent)
    {
        VOS_MemCpy(pstMsg->aucContent, pucContent, ulLen);
    }

    if(VOS_OK != VOS_SendMsg(MAPS_PIH_PID, pstMsg))
    {
        PIH_ERROR_LOG("USIMM_SciDataHook: vos_MsgHook is Failed");
    }

    return;
}



VOS_VOID SI_PIH_EventReportError( VOS_UINT16       ClientId,
                                            VOS_UINT8       OpId,
                                            VOS_UINT32      SimError,
                                            VOS_UINT32      MsgName,
                                            VOS_UINT32      EventType)
{
    SI_PIH_EVENT_INFO_STRU   Event;

    /*������*/
    Event.ClientId                    = ClientId;
    Event.OpId                        = OpId;
    Event.PIHError                    = SimError;
    Event.EventType                   = EventType;

    /*����ظ�*/
    if(MsgName == SI_PIH_PCSC_DATA_REQ)
    {
        SI_PIH_PCSCCmdCnf(EventType, SimError, 0, VOS_NULL_PTR);
    }
    else
    {
        SI_PIHCallback(&Event);
    }

    return ;
}


VOS_UINT32  SI_PIH_FdnBdnStateReport(VOS_UINT32       ulState,
                                                VOS_UINT32       SimError,
                                                VOS_UINT32       ulCmdType)
{
    SI_PIH_EVENT_INFO_STRU  Event;

    /*������*/
    Event.PIHError                    = SimError;
    Event.PIHEvent.FDNCnf.FdnCmd      = ulCmdType;
    Event.PIHEvent.FDNCnf.FdnState    = ulState;

    /*����ظ�*/
    return SI_PIH_EventCallBack(&Event);
}


VOS_VOID SI_PIH_PCSCDataCnf(VOS_UINT32 ulCmdType, VOS_UINT32 ulResult,
                                    VOS_UINT32 ulLen, VOS_UINT8 *pucContent)
{
    SI_PIH_PCSCCmdCnf(ulCmdType, ulResult, ulLen, pucContent);

    VOS_MemSet(&g_stPIHCtrlInfo, 0, sizeof(g_stPIHCtrlInfo));

    return;
}


VOS_UINT32 SI_PIH_PCSCApduCheck(VOS_UINT32 *pulLen, VOS_UINT8 *pucApdu, VOS_UINT32 *pulRspLen, VOS_UINT8 *pucRsp)
{
    VOS_UINT32  ulResult        = VOS_OK;
    VOS_UINT8   aucAid[5]       = {0xA0, 0x00, 0x00, 0x00, 0x87};
    VOS_UINT8   aucSelectAid[7] = {0x00, 0xA4, 0x08, 0x04, 0x02, 0x7F, 0xFF};
    VOS_UINT8   ucCardType;

    ulResult = (VOS_UINT32)VOS_MemCmp(pucApdu + 5, aucAid, sizeof(aucAid));

    if ((VOS_OK == ulResult) && (CMD_INS_SELECT == pucApdu[INS]))
    {
        VOS_MemCpy(pucApdu, aucSelectAid, sizeof(aucSelectAid));

        *pulLen = sizeof(aucSelectAid);

        ulResult = VOS_OK;
    }
    else if((CMD_INS_SELECT == pucApdu[INS])&&(0x04 == pucApdu[P1]))
    {
        *pulRspLen = 2;

        USIMM_GetCardType(VOS_NULL_PTR, &ucCardType);

        if(USIMM_CARD_USIM == ucCardType)
        {
            pucRsp[0] = 0x6A;    /* ������ */
            pucRsp[1] = 0x82;
        }
        else
        {
            pucRsp[0] = 0x6B;
            pucRsp[1] = 0x00;
        }

        ulResult = VOS_ERR;
    }
    else if((CMD_INS_SELECT != pucApdu[INS])        && (CMD_INS_STATUS != pucApdu[INS])
         && (CMD_INS_READ_BINARY != pucApdu[INS])   && (CMD_INS_UPDATE_BINARY != pucApdu[INS])
         && (CMD_INS_READ_RECORD != pucApdu[INS])   && (CMD_INS_UPDATE_RECORD != pucApdu[INS])
         && (CMD_INS_SEARCH_RECORD != pucApdu[INS]) && (CMD_INS_DEACTIVATE_FILE != pucApdu[INS])
         && (CMD_INS_ACTIVATE_FILE != pucApdu[INS]) && (CMD_INS_AUTHENTICATE != pucApdu[INS])
         && (CMD_INS_GET_CHALLENGE != pucApdu[INS]) && (CMD_INS_ENVELOPE != pucApdu[INS])
         && (CMD_INS_FETCH != pucApdu[INS])         && (CMD_INS_TERMINAL_RESPONSE != pucApdu[INS])
         && (CMD_INS_GET_RESPONSE != pucApdu[INS])  && (CMD_INS_VERIFY != pucApdu[INS])
         && (CMD_INS_CHANGE_PIN != pucApdu[INS])    && (CMD_INS_DISABLE_PIN != pucApdu[INS])
         && (CMD_INS_ENABLE_PIN != pucApdu[INS])    && (CMD_INS_UNBLOCK_PIN != pucApdu[INS]))
    {
        *pulRspLen = 2;

        pucRsp[0] = 0x6F;    /* TECH PROBLEM */
        pucRsp[1] = 0x00;

        ulResult = VOS_ERR;
    }
    else
    {
        ulResult = VOS_OK;
    }

    return ulResult;
}

/*****************************************************************************
�� �� ��  :
��������  :
�������  : ��
�������  : ��
�� �� ֵ  : SI_UINT32 ����ִ�н��
���ú���  : ��
��������  : �ⲿ�ӿ�
History     :
1.��    ��  : 2010��08��20��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_UINT32 SI_PIH_GAccessReqProc(SI_PIH_GACCESS_REQ_STRU *pstMsg)
{
    SI_PIH_EVENT_INFO_STRU  stEvent;
    VOS_UINT32              ulResult;
    VOS_UINT32              ulDataLen;
    VOS_UINT8               *pucData;
    VOS_UINT8               aucData[2] = {0};
    VOS_UINT32              ulRspLen;
    USIMM_U8_LVDATA_STRU    stData;

    if(pstMsg->aucData[1] == 0xC0)
    {
        stEvent.PIHError = TAF_ERR_NO_ERROR;

        if(pstMsg->aucData[4] > g_stPIHCSIMCtrlInfo.usRspLen)
        {
            stEvent.PIHEvent.GAccessCnf.Len = 0;

            stEvent.PIHEvent.GAccessCnf.SW1 = 0x67;
            stEvent.PIHEvent.GAccessCnf.SW2 = 0x00;
        }
        else
        {
            if (0 == pstMsg->aucData[4])
            {
                stEvent.PIHEvent.GAccessCnf.Len = g_stPIHCSIMCtrlInfo.usRspLen;
                VOS_MemCpy(stEvent.PIHEvent.GAccessCnf.Command, g_stPIHCSIMCtrlInfo.aucRspCotent, g_stPIHCSIMCtrlInfo.usRspLen);
            }
            else
            {
                stEvent.PIHEvent.GAccessCnf.Len = pstMsg->aucData[4];
                VOS_MemCpy(stEvent.PIHEvent.GAccessCnf.Command, g_stPIHCSIMCtrlInfo.aucRspCotent, pstMsg->aucData[4]);
            }
            stEvent.PIHEvent.GAccessCnf.SW1 = 0x90;
            stEvent.PIHEvent.GAccessCnf.SW2 = 0x00;
        }

        return SI_PIH_EventCallBack(&stEvent);
    }
    else
    {
        pucData = (VOS_UINT8 *)VOS_MemAlloc(WUEPS_PID_USIM, DYNAMIC_MEM_PT, pstMsg->ulDataLen);

        if(VOS_NULL_PTR == pucData)
        {
            PIH_ERROR_LOG("SI_PIH_GAccessReqProc:VOS_MemAlloc is Failed.");
            return USIMM_API_ALLOCMEM_FAILED; /*���غ���������Ϣ*/
        }

        ulDataLen = pstMsg->ulDataLen;

        VOS_MemCpy(pucData, pstMsg->aucData, pstMsg->ulDataLen);

        ulResult = SI_PIH_PCSCApduCheck(&ulDataLen, pucData, &ulRspLen, aucData);

        if(VOS_OK != ulResult)
        {
            VOS_MemFree(WUEPS_PID_USIM, pucData);

            stEvent.PIHError = TAF_ERR_CMD_TYPE_ERROR;

            return SI_PIH_EventCallBack(&stEvent);
        }

        stData.pucData      = pucData;
        stData.ulDataLen    = ulDataLen;

        ulResult = USIMM_GenericAccessReq(MAPS_PIH_PID, &stData,
                                            g_stPIHCSIMCtrlInfo.usPathLen,
                                            g_stPIHCSIMCtrlInfo.ausPath);

        VOS_MemFree(WUEPS_PID_USIM, pucData);

        if(ulResult != USIMM_API_SUCCESS)
        {
            stEvent.PIHError = TAF_ERR_USIM_SIM_CARD_NOTEXIST;

            return SI_PIH_EventCallBack(&stEvent);
        }
    }

    return VOS_OK;
}
VOS_UINT32 SI_PIH_IsdbAccessReqProc(SI_PIH_ISDB_ACCESS_REQ_STRU *pstMsg)
{
    SI_PIH_EVENT_INFO_STRU              stEvent;
    VOS_UINT32                          ulResult;

    ulResult = USIMM_IsdbAccessReq(MAPS_PIH_PID, (VOS_UINT16)pstMsg->ulDataLen, pstMsg->aucData);

    if (USIMM_API_SUCCESS != ulResult)
    {
        stEvent.PIHError = TAF_ERR_SIM_FAIL;

        return SI_PIH_EventCallBack(&stEvent);
    }

    return VOS_OK;
}


VOS_UINT32 SI_PIH_CchoSetReqProc(SI_PIH_CCHO_SET_REQ_STRU *pstMsg)
{
    SI_PIH_EVENT_INFO_STRU              stEvent;
    VOS_UINT32                          ulResult;
    USIMM_FEATURE_CFG_STRU              stUsimmFeatureCfg;

    if (VOS_OK != NV_Read(en_NV_Item_Usim_Support_Feature_Config,
                         (VOS_VOID*)&stUsimmFeatureCfg, sizeof(stUsimmFeatureCfg)))
    {
        VOS_MemSet(stUsimmFeatureCfg.unCfg.aulValue , VOS_FALSE, sizeof(stUsimmFeatureCfg.unCfg.aulValue));
    }

    if (VOS_TRUE != stUsimmFeatureCfg.unCfg.stFeatureCfg.ulNFCFlg)
    {
        stEvent.PIHError = TAF_ERR_ERROR;

        return SI_PIH_EventCallBack(&stEvent);
    }

    ulResult = USIMM_OpenChannelReq(MAPS_PIH_PID, pstMsg->ulAIDLen, pstMsg->aucADFName);

    if (USIMM_API_SUCCESS != ulResult)
    {
        stEvent.PIHError = TAF_ERR_SIM_FAIL;

        return SI_PIH_EventCallBack(&stEvent);
    }

    return VOS_OK;
}


VOS_UINT32 SI_PIH_CchcSetReqProc(SI_PIH_CCHC_SET_REQ_STRU *pstMsg)
{
    SI_PIH_EVENT_INFO_STRU              stEvent;
    VOS_UINT32                          ulResult;
    USIMM_FEATURE_CFG_STRU              stUsimmFeatureCfg;

    if (VOS_OK != NV_Read(en_NV_Item_Usim_Support_Feature_Config,
                         (VOS_VOID*)&stUsimmFeatureCfg, sizeof(stUsimmFeatureCfg)))
    {
        VOS_MemSet(stUsimmFeatureCfg.unCfg.aulValue , VOS_FALSE, sizeof(stUsimmFeatureCfg.unCfg.aulValue));
    }

    if (VOS_TRUE != stUsimmFeatureCfg.unCfg.stFeatureCfg.ulNFCFlg)
    {
        stEvent.PIHError = TAF_ERR_ERROR;

        return SI_PIH_EventCallBack(&stEvent);
    }

    ulResult = USIMM_CloseChannelReq(MAPS_PIH_PID, pstMsg->ulSessionID);

    if (USIMM_API_SUCCESS != ulResult)
    {
        stEvent.PIHError = TAF_ERR_SIM_FAIL;

        return SI_PIH_EventCallBack(&stEvent);
    }

    return VOS_OK;

}


VOS_UINT32 SI_PIH_CglaSetReqProc(SI_PIH_CGLA_REQ_STRU *pstMsg)
{
    SI_PIH_EVENT_INFO_STRU              stEvent;
    VOS_UINT32                          ulResult;
    USIMM_FEATURE_CFG_STRU              stUsimmFeatureCfg;

    if (VOS_OK != NV_Read(en_NV_Item_Usim_Support_Feature_Config,
                         (VOS_VOID*)&stUsimmFeatureCfg, sizeof(stUsimmFeatureCfg)))
    {
        VOS_MemSet(stUsimmFeatureCfg.unCfg.aulValue , VOS_FALSE, sizeof(stUsimmFeatureCfg.unCfg.aulValue));
    }

    if (VOS_TRUE != stUsimmFeatureCfg.unCfg.stFeatureCfg.ulNFCFlg)
    {
        stEvent.PIHError = TAF_ERR_ERROR;

        return SI_PIH_EventCallBack(&stEvent);
    }

    ulResult = USIMM_AccessChannelReq(MAPS_PIH_PID, pstMsg->ulSessionID, pstMsg->ulDataLen, (VOS_UINT8 *)pstMsg->aucData);

    if (USIMM_API_SUCCESS != ulResult)
    {
        stEvent.PIHError = TAF_ERR_SIM_FAIL;

        return SI_PIH_EventCallBack(&stEvent);
    }

    return VOS_OK;
}


VOS_UINT32 SI_PIH_ATRQryReqProc(VOS_VOID)
{
    SI_PIH_EVENT_INFO_STRU              stEvent;
    VOS_UINT32                          ulATRLen = 0;
    VOS_UINT8                           aucATRData[SI_ATR_MAX_LEN] = {0};
    VOS_INT32                           lResult;
    SCI_ATRInfo                         stSCIATRInfo;

    VOS_MemSet(&stEvent, 0, sizeof(stEvent));

    lResult = (VOS_INT32)DRV_USIMMSCI_GET_ATR(&ulATRLen, aucATRData, &stSCIATRInfo);

    if ((VOS_OK != lResult)||(ulATRLen > SI_ATR_MAX_LEN))
    {
        PIH_ERROR1_LOG("SI_PIH_ATRQryReqProc: Get ATR fail.", (VOS_INT32)ulATRLen);

        stEvent.PIHError = TAF_ERR_SIM_FAIL;
    }
    else
    {
        stEvent.PIHError = TAF_ERR_NO_ERROR;

        stEvent.PIHEvent.stATRQryCnf.ulLen = ulATRLen;
        VOS_MemCpy(stEvent.PIHEvent.stATRQryCnf.aucCommand, aucATRData, sizeof(stEvent.PIHEvent.stATRQryCnf.aucCommand));
    }

    return SI_PIH_EventCallBack(&stEvent);
}


VOS_VOID SI_PIH_PCSC_PowerOn(SI_PIH_PCSC_REQ_STRU *pstMsg)
{
    VOS_INT32                           lSCIResult;
    VOS_UINT32                          ulATRLen = 0;
    VOS_UINT8                           aucATR[USIMM_ATR_MAX_LEN];
    SCI_ATRInfo                         stSCIATRInfo;

    lSCIResult = (VOS_INT32)DRV_USIMMSCI_GET_ATR(&ulATRLen, aucATR, &stSCIATRInfo);

    if (USIMM_SCI_SUCCESS != lSCIResult)
    {
        SI_PIH_PCSCDataCnf(pstMsg->ulCmdType, (VOS_UINT32)lSCIResult, VOS_NULL, VOS_NULL_PTR);
    }
    else
    {
        SI_PIH_PCSCDataCnf(pstMsg->ulCmdType, (VOS_UINT32)lSCIResult, ulATRLen, aucATR);
    }

    return;
}


VOS_VOID SI_PIH_PCSC_PowerOff(SI_PIH_PCSC_REQ_STRU *pstMsg)
{
    SI_PIH_PCSCDataCnf(pstMsg->ulCmdType, VOS_OK, 0, VOS_NULL_PTR);

    return;
}


VOS_VOID SI_PIH_PCSC_SimQuiry(SI_PIH_PCSC_REQ_STRU *pstMsg)
{
    VOS_UINT8 ucCardStatus;

    USIMM_GetCardType(&ucCardStatus, VOS_NULL_PTR);

    if (USIMM_CARD_SERVIC_ABSENT == ucCardStatus)
    {
        ucCardStatus = SI_PIH_PCSC_SIM_ABSENT;
    }
    else
    {
        ucCardStatus = SI_PIH_PCSC_SIM_PRESENT;
    }

    SI_PIH_PCSCDataCnf(pstMsg->ulCmdType, VOS_OK, sizeof(VOS_UINT8), &ucCardStatus);

    return;
}


VOS_VOID SI_PIH_PCSC_GetATR(SI_PIH_PCSC_REQ_STRU *pstMsg)
{
    VOS_INT32                           lSCIResult;
    VOS_UINT32                          ulATRLen = 0;
    VOS_UINT8                           aucATR[USIMM_ATR_MAX_LEN];
    SCI_ATRInfo                         stSCIATRInfo;

    lSCIResult = (VOS_INT32)DRV_USIMMSCI_GET_ATR(&ulATRLen, aucATR, &stSCIATRInfo);

    if (USIMM_SCI_SUCCESS != lSCIResult)
    {
        SI_PIH_PCSCDataCnf(pstMsg->ulCmdType, (VOS_UINT32)lSCIResult, VOS_NULL, VOS_NULL_PTR);
    }
    else
    {
        SI_PIH_PCSCDataCnf(pstMsg->ulCmdType, (VOS_UINT32)lSCIResult, ulATRLen, aucATR);
    }

    return;
}


VOS_VOID SI_PIH_PCSC_GetPara(SI_PIH_PCSC_REQ_STRU *pstMsg)
{
    VOS_UINT8  aucSimPara[5];

    DRV_PCSC_GET_SCI_PARA(aucSimPara);

    SI_PIH_PCSCDataCnf(pstMsg->ulCmdType, VOS_OK, sizeof(aucSimPara), aucSimPara);

    return;
}


VOS_VOID SI_PIH_PCSC_GetClkFreq(SI_PIH_PCSC_REQ_STRU *pstMsg)
{
    VOS_UINT8   aucSimClkFreq[16];
    VOS_UINT32  ulLen = 0;
    VOS_UINT32  ulResult;

    ulResult = DRV_PCSC_GET_CLK_FREQ(&ulLen, aucSimClkFreq);

    SI_PIH_PCSCDataCnf(pstMsg->ulCmdType, ulResult, ulLen, aucSimClkFreq);

    return;
}


VOS_VOID SI_PIH_PCSC_GetBaudRate(SI_PIH_PCSC_REQ_STRU *pstMsg)
{
    VOS_UINT8   aucSimBaudRate[16];
    VOS_UINT32  ulLen = 0;
    VOS_UINT32  ulResult;

    ulResult = DRV_PCSC_GET_BAUD_RATE(&ulLen, aucSimBaudRate);

    SI_PIH_PCSCDataCnf(pstMsg->ulCmdType, ulResult, ulLen, aucSimBaudRate);

    return;
}



VOS_VOID SI_PIH_PCSC_ApduCmd(SI_PIH_PCSC_REQ_STRU *pstMsg)
{
    VOS_UINT32                          ulResult = VOS_ERR;
    VOS_UINT32                          ulLen = 0;
    VOS_UINT8                           aucContent[260]={0};/* ������SW״̬�� */
    USIMM_U8_LVDATA_STRU                stData;

    ulResult = SI_PIH_PCSCApduCheck(&pstMsg->ulCmdLen, pstMsg->aucAPDU, &ulLen, aucContent);

    if(ulResult != VOS_OK)
    {
        SI_PIH_PCSCDataCnf(pstMsg->ulCmdType, VOS_OK, ulLen, aucContent);

        return;
    }

    /* ���ڻ����Get Response���ݣ�������һ��ִ�е��Ƕ�Ӧ��Case4 APDU��ֱ�ӻظ� */
    if ((0xC0 == pstMsg->aucAPDU[1]) && (0 != g_stPIHCSIMCtrlInfo.usRspLen))
    {
        VOS_MemCpy(aucContent, g_stPIHCSIMCtrlInfo.aucRspCotent, g_stPIHCSIMCtrlInfo.usRspLen);

        ulLen = ((pstMsg->aucAPDU[4] < g_stPIHCSIMCtrlInfo.usRspLen)?
                            pstMsg->aucAPDU[4] :g_stPIHCSIMCtrlInfo.usRspLen) + 2;

        aucContent[ulLen-2] = 0x90;    /* ������ǰ��SW�ֽ�������β */

        aucContent[ulLen-1] = 0x00;

        SI_PIH_PCSCDataCnf(pstMsg->ulCmdType, ulResult, ulLen, aucContent);

        return;
    }

    stData.pucData          = pstMsg->aucAPDU;
    stData.ulDataLen        = pstMsg->ulCmdLen;

    ulResult = USIMM_GenericAccessReq(MAPS_PIH_PID,
                                        &stData,
                                        g_stPIHCSIMCtrlInfo.usPathLen,
                                        g_stPIHCSIMCtrlInfo.ausPath);

    if(VOS_OK != ulResult)
    {
        PIH_ERROR_LOG("SI_PIH_PCSC_ApduCmd: The Command Type is Error");

        SI_PIH_PCSCDataCnf(pstMsg->ulCmdType, VOS_ERR, 0, VOS_NULL_PTR);
    }

    return;
}
static const PUSIMPCSCPROC  af_PIH_PCSCProc[] =
{
    SI_PIH_PCSC_PowerOn,
    SI_PIH_PCSC_PowerOff,
    SI_PIH_PCSC_SimQuiry,
    SI_PIH_PCSC_ApduCmd,
    SI_PIH_PCSC_GetATR,
    SI_PIH_PCSC_GetPara,
    SI_PIH_PCSC_GetClkFreq,
    SI_PIH_PCSC_GetBaudRate
};

VOS_UINT32 SI_PIH_PCSCCmdHandle(SI_PIH_PCSC_REQ_STRU *pstMsg)
{
    if (pstMsg->ulCmdType >= SI_PIH_PCSC_CMD_BUTT)
    {
        return VOS_ERR;
    }

    af_PIH_PCSCProc[pstMsg->ulCmdType](pstMsg);

    return VOS_OK;
}

#if (FEATURE_ON == FEATURE_VSIM)

VOS_VOID SI_PIH_VsimWriteableFileUpdate(VOS_UINT16 usFileId, VOS_UINT8 *pucFileContent)
{

    if (EFFPLMN == usFileId)
    {
        if (VOS_OK != NV_Write(en_NV_Item_VSIM_Fplmn_Info, pucFileContent, sizeof(VSIM_CARD_FPLMN_FILE_NV_STRU)))
        {
            PIH_WARNING_LOG("SI_PIH_VsimWriteableFileUpdate: write Fplmn Info failed.");
        }
    }
    else if (EFLOCI == usFileId)
    {
        if (VOS_OK != NV_Write(en_NV_Item_VSIM_Loci_Info, pucFileContent, sizeof(VSIM_CARD_LOCI_FILE_NV_STRU)))
        {
            PIH_WARNING_LOG("SI_PIH_VsimWriteableFileUpdate: write Loic Info failed.");
        }
    }
    else if ((EFPSLOCI == usFileId) || (EFLOCIGPRS == usFileId))
    {
        if (VOS_OK != NV_Write(en_NV_Item_VSIM_PsLoci_Info, pucFileContent, sizeof(VSIM_CARD_PSLOCI_FILE_NV_STRU)))
        {
            PIH_WARNING_LOG("SI_PIH_VsimWriteableFileUpdate: write PsLoic Info failed.");
        }
    }
    else
    {
        PIH_WARNING_LOG("SI_PIH_VsimWriteableFileUpdate: Wrong file ID.");
    }

    return;
}


VOS_VOID SI_PIH_VsimWriteableFileClear(VOS_VOID)
{
    VOS_UINT8                           i;
    VOS_UINT8                           aucFileContent[255];
    VOS_UINT16                          ausFileList[]={EFFPLMN, EFPSLOCI, EFLOCI, EFLOCIGPRS};

    VOS_MemSet(aucFileContent, (VOS_CHAR)0xFF, sizeof(aucFileContent));

    for (i = 0; i < ARRAYSIZE(ausFileList); i++)
    {
        SI_PIH_VsimWriteableFileUpdate(ausFileList[i], aucFileContent);
    }

    return;
}


VOS_UINT32 SI_PIH_DeactvieCardTimeOut(VOS_VOID)
{
    VOS_UINT8                   ucCardState;
    SI_PIH_EVENT_INFO_STRU      stEvent;

    ucCardState = USIMM_CARD_SERVIC_ABSENT;
    stEvent.PIHError = TAF_ERR_NO_ERROR; /* �ȸ�ֵ�ɹ� */

    /*���µ�ǰ�ȴ�ȥ����SIM���ĳ��Դ���*/
    g_ulPIHDeactiveRetryCnt++;

    /*��ȡ��ǰSIM����״̬*/
    USIMM_GetCardType(&ucCardState, VOS_NULL_PTR);

    /*����ǰ���Ѿ�ȥ���֪ͨAP�������*/
    if (USIMM_CARD_SERVIC_ABSENT == ucCardState)
    {
        /*��� NV�б����EF_LOCI\EF_FPLMN\EF_PSLOCI\EF_LOCIGPRS��Ϣ*/
        SI_PIH_VsimWriteableFileClear();
        return SI_PIH_EventCallBack(&stEvent);
    }
    else
    {
        /*���ȴ������ﵽ����Դ�����֪ͨAP������������������һ�εȴ�*/
        if (g_ulPIHDeactiveRetryCnt < SI_PIH_DEACTIVE_MAX_TIME)
        {
            VOS_StartRelTimer(&g_stPIHDeactiveCardTimer,
                               MAPS_PIH_PID,
                               SI_PIH_DEACTIVE_CARD_TIME,
                               SI_PIH_TIMER_NAME_DEACTIVECARD,
                               0,
                               VOS_RELTIMER_NOLOOP,
                               VOS_TIMER_PRECISION_10);
        }
        else
        {
            /*��� NV�б����EF_LOCI\EF_FPLMN\EF_PSLOCI\EF_LOCIGPRS��Ϣ*/
            SI_PIH_VsimWriteableFileClear();
            stEvent.PIHError = TAF_ERR_TIME_OUT;
            return SI_PIH_EventCallBack(&stEvent);
        }
    }

    return VOS_OK;
}
VOS_VOID SI_PIH_BcdNumToAsciiNum(VOS_UINT8 *pucAsciiNum, VOS_UINT8 *pucBcdNum, VOS_UINT8 ucBcdNumLen)
{
    VOS_UINT8       ucTmp;
    VOS_UINT8       ucFirstNumber;
    VOS_UINT8       ucSecondNumber;

    for (ucTmp = 0; ucTmp < ucBcdNumLen; ucTmp++)
    {
        ucFirstNumber = (VOS_UINT8)((*(pucBcdNum + ucTmp)) & 0x0f);             /*low four bits*/
        ucSecondNumber = (VOS_UINT8)(((*(pucBcdNum + ucTmp)) >> 4) & 0x0f);   /*high four bits*/
        if (ucFirstNumber <= 9)
        {
            *pucAsciiNum = ucFirstNumber + 0x30;
            pucAsciiNum++;
        }
        else if (0xa == ucFirstNumber)            /* the key is '*' */
        {
            *pucAsciiNum = 0x2a;
            pucAsciiNum++;
        }
        else if (0xb == ucFirstNumber)            /* the key is '#' */
        {
            *pucAsciiNum = 0x23;
            pucAsciiNum++;
        }
        else                                     /* the key is 'a','b'or 'c' */
        {
            *pucAsciiNum = ucFirstNumber + 0x57;
            pucAsciiNum++;
        }

        /* the following proceed the second number */
        if (0xf == ucSecondNumber)
        {
            break;
        }
        if (ucSecondNumber <= 9)
        {
            *pucAsciiNum = ucSecondNumber + 0x30;
            pucAsciiNum++;
        }
        else if (0xa == ucSecondNumber)            /* the key is '*' */
        {
            *pucAsciiNum = 0x2a;
            pucAsciiNum++;
        }
        else if (0xb == ucSecondNumber)            /* the key is '#' */
        {
            *pucAsciiNum = 0x23;
            pucAsciiNum++;
        }
        else                                     /* the key is 'a','b'or 'c' */
        {
            *pucAsciiNum = ucSecondNumber + 0x57;
            pucAsciiNum++;
        }
    }

    *pucAsciiNum = '\0';

    return;
}
VOS_UINT32 SI_PIH_ImsiBcd2Ascii(VOS_UINT8 ucLen, VOS_UINT8 * pucNum, VOS_UINT8 * pucAsciiStr)
{
    VOS_UINT32  ulI     = 0;
    VOS_UINT8   ucTmp   = 0;

    /*��Ч���ж�*/
    if ( (VOS_NULL_PTR == pucNum)
       ||(VOS_NULL_PTR == pucAsciiStr)
       ||(           0 == ucLen )
       )
    {
        PIH_WARNING_LOG("SI_PIH_ImsiBcd2Ascii():WARNING:NULL PTR");
        return VOS_ERR;
    }

    /*�����ƶ��û�ʶ����*/
    /*��մ���ַ����Ļ�����*/
    for ( ulI = 0; ulI < (VOS_UINT32)ucLen; ulI++ )
    {
        pucAsciiStr[ulI] = 0;
    }

    ucTmp = (*pucNum >> 4) & 0x0f;

    /*ȡ����һλ*/
    SI_PIH_BcdNumToAsciiNum(pucAsciiStr, &ucTmp, 1);

    /*����Bcd��Ascii��ת��*/
    SI_PIH_BcdNumToAsciiNum(pucAsciiStr + 1, pucNum + 1, (ucLen - 1)/2);

    return VOS_OK;
}
VOS_VOID SI_PIH_DecodeImsiHPLMN(VOS_UINT8 *pucImsi, VOS_UINT8 ucMNCLen, SI_PIH_CARD_CONTENT_STRU *pstCardCont)
{
    /* ��IMSIת��ΪASCII�� */
    SI_PIH_ImsiBcd2Ascii((VOS_UINT8)(pucImsi[0]*2), &pucImsi[1], pstCardCont->aucImsi);

    /* ���HPLMN */
    pstCardCont->astPlmn[0].ulMCC |= (pucImsi[1] & 0xF0) >> 0x04;
    pstCardCont->astPlmn[0].ulMCC |= (pucImsi[2] & 0x0F) << 0x08;
    pstCardCont->astPlmn[0].ulMCC |= (pucImsi[2] & 0xF0) << 0x0c;
    pstCardCont->astPlmn[0].ulMNC |= (pucImsi[3] & 0x0F);
    pstCardCont->astPlmn[0].ulMNC |= (pucImsi[3] & 0xF0) << 0x04;

    if (SI_PIH_MNC_TWO_BYTES_LEN == ucMNCLen)
    {
        pstCardCont->astPlmn[0].ulMNC |= (0x0f) << 0x10;
    }
    else
    {
        pstCardCont->astPlmn[0].ulMNC |= (pucImsi[4] & 0x0f) << 0x10;
    }

    pstCardCont->usPlmnNum++;

    return;
}


VOS_VOID SI_PIH_DecodeEHPLMN(VOS_UINT8 *pucData, VOS_UINT32 ulDateLen, SI_PIH_CARD_CONTENT_STRU *pstCardCont)
{
    VOS_UINT32                          ulEHplmnNum;
    VOS_UINT32                          ulIndex;

    /* ��ȡ�Ŀ��ļ����Ȳ��Ϸ���ֱ�ӷ���VOS_FALSE */
    if ((ulDateLen < SI_PIH_EHPLMN_BYTE_NUM)
     || (0 != (ulDateLen % SI_PIH_EHPLMN_BYTE_NUM)))
    {
        PIH_ERROR1_LOG("SI_PIH_DecodeEHPLMN: the EFEHPLMN file is incorrect.", (VOS_INT32)ulDateLen);
        return ;
    }

    ulEHplmnNum = ulDateLen/SI_PIH_EHPLMN_BYTE_NUM;

    /* ����HPLMN�Ѿ�ռ��һ����Ԫ��������Ҫ���м�1���� */
    if (ulEHplmnNum > (SI_PIH_HPLMN_MAX_NUM - 1))
    {
        ulEHplmnNum = (SI_PIH_HPLMN_MAX_NUM - 1);
    }

    for (ulIndex = 1; ulIndex <= ulEHplmnNum; ulIndex++)
    {
        pstCardCont->astPlmn[ulIndex].ulMCC |= (pucData[0] & 0x0F);
        pstCardCont->astPlmn[ulIndex].ulMCC |= (pucData[0] & 0xF0) << 0x04;
        pstCardCont->astPlmn[ulIndex].ulMCC |= (pucData[1] & 0x0F) << 0x10;

        pstCardCont->astPlmn[ulIndex].ulMNC |= (pucData[1] & 0xF0) << 0x0C;
        pstCardCont->astPlmn[ulIndex].ulMNC |= (pucData[2] & 0x0F);
        pstCardCont->astPlmn[ulIndex].ulMNC |= (pucData[2] & 0xF0) << 0x04;

        pucData += SI_PIH_EHPLMN_BYTE_NUM;
    }

    pstCardCont->usPlmnNum += (VOS_UINT16)ulEHplmnNum;

    return;
}


VOS_BOOL SI_PIH_GetEHPlmnNVState(VOS_VOID)
{
    VOS_UINT16                          usEHPlmnSupportFlg;

    /* ��ȡEHPLMN�Ƿ�ʹ�ܵ�NV�� */
    if (NV_OK != NV_Read(en_NV_Item_Standard_EHplmn_Support_Flg,
                         &usEHPlmnSupportFlg,
                         sizeof(VOS_UINT16)))
    {
        PIH_ERROR_LOG("SI_PIHGetEHPlmnNVState: NV_Read EHplmn_Support fail.");
        return VOS_FALSE;
    }

    /* EHPLMN��֧�� */
    if (VOS_TRUE != usEHPlmnSupportFlg)
    {
        PIH_WARNING_LOG("SI_PIHGetEHPlmnNVState: EHplmn not Support.");
        return VOS_FALSE;
    }

    return VOS_TRUE;
}


VOS_UINT8 SI_PIH_GetMncLen(VOS_UINT8 *pucAdContent, VOS_UINT32 ulFileSize)
{
    VOS_UINT8                           ucHplmnMncLen;

    ucHplmnMncLen = SI_PIH_MNC_TWO_BYTES_LEN;

    /* �ļ���С��С����Ĭ��MNCΪ2�ֽڳ��� */
    if(USIMM_EF_AD_MIN_LEN >= ulFileSize)
    {
        return ucHplmnMncLen;
    }

    /* �ж�MNC����,MNC����Ϊ2λ��3λ */
    if ((SI_PIH_MNC_TWO_BYTES_LEN == (pucAdContent[SI_PIH_AD_MNC_LEN_POS] & 0x0F))
        || (SI_PIH_MNC_THREE_BYTES_LEN == (pucAdContent[SI_PIH_AD_MNC_LEN_POS] & 0x0F)))
    {
        ucHplmnMncLen = pucAdContent[SI_PIH_AD_MNC_LEN_POS] & 0x0F;
    }

    return ucHplmnMncLen;
}


VOS_VOID SI_PIH_GetRSimContent(SI_PIH_CARD_CONTENT_STRU *pstCardCont)
{
    VOS_UINT8                           ucCardType;
    VOS_UINT8                           aucImsi[USIMM_EF_IMSI_LEN] = {0};
    VOS_UINT32                          ulDataLen;
    VOS_UINT8                          *pucData;
    VOS_UINT8                           ucHplmnMncLen;

    pstCardCont->ucIndex   = SI_PIH_SIM_REAL_SIM1;
    pstCardCont->ucCardCap = PIH_BIT_N(USIMM_CARD_SIM) | PIH_BIT_N(USIMM_CARD_USIM);

    /* ����VSIM���Ѿ�����������д�����ֶ� */
    if (VOS_TRUE == USIMM_VsimIsActive())
    {
        PIH_WARNING_LOG("SI_PIH_GetRSimContent: vSIM is Active.");
        return;
    }

    /* ��ȡ������ */
    if (USIMM_API_SUCCESS != USIMM_GetCardType(VOS_NULL_PTR, &ucCardType))
    {
        PIH_ERROR_LOG("SI_PIH_GetRSimContent: USIMM_GetCardType fail.");
        return;
    }

    /* �����ͳ�����Ҫ�ķ�Χ����������ǿ��ٿ������޿�Ӧ��ֱ�ӷ��� */
    if (ucCardType > USIMM_CARD_USIM)
    {
        PIH_WARNING_LOG("SI_PIH_GetRSimContent: Card type is Error.");
        return;
    }

    pstCardCont->ucCardType = (VOS_UINT8)PIH_BIT_N(ucCardType);

    /*  ��ȡIMSI���� */
    if (USIMM_API_SUCCESS != USIMM_GetCardIMSI(aucImsi))
    {
        PIH_ERROR_LOG("SI_PIH_GetRSimContent: USIMM_GetCardIMSI fail.");
        return;
    }

    /* �ж�IMSI�����Ƿ�Ϸ� */
    if (SI_PIH_IMSI_MAX_LEN < aucImsi[0])
    {
        PIH_ERROR1_LOG("SI_PIH_GetRSimContent: IMSI length is error.", (long)(aucImsi[0]));
        return;
    }

    /* ��AD�ļ��У���ȡHPLMN MNC���� */
    if (USIMM_API_SUCCESS != USIMM_GetCachedFile(EFAD, &ulDataLen, &pucData, USIMM_UNLIMIT_APP))
    {
        PIH_ERROR_LOG("SI_PIH_GetRSimContent: USIMM_GetCachedFile fail.");
        return;
    }

    ucHplmnMncLen = SI_PIH_GetMncLen(pucData, ulDataLen);

    SI_PIH_DecodeImsiHPLMN(aucImsi, ucHplmnMncLen, pstCardCont);

    if (USIMM_CARD_SIM == ucCardType)
    {
        PIH_NORMAL_LOG("SI_PIH_GetRSimContent: SIM Card do not need read EHPLMN.");
        return;
    }

    if(VOS_FALSE == SI_PIH_GetEHPlmnNVState())
    {
        PIH_NORMAL_LOG("SI_PIH_GetRSimContent: SI_PIHGetEHPlmnNVState not support.");
        return;
    }

    /* �ж�EHPLMN�����Ƿ�֧�� */
    if (PS_USIM_SERVICE_AVAILIABLE != USIMM_IsServiceAvailable(USIM_SVR_EHPLMN))
    {
        PIH_WARNING_LOG("SI_PIH_GetRSimContent: EHplmn's SST not Support.");
        return;
    }

    /* �ӻ������ж���EHPLMN�ļ����� */
    if (USIMM_API_SUCCESS != USIMM_GetCachedFile(EFEHPLMN, &ulDataLen, &pucData, USIMM_UNLIMIT_APP))
    {
        PIH_WARNING_LOG("SI_PIH_GetRSimContent: EHplmn not in SIM.");
        return;
    }

    SI_PIH_DecodeEHPLMN(pucData, ulDataLen, pstCardCont);

    return ;
}


VOS_UINT8* SI_PIH_GetVSimFileContent(OAM_XML_NODE_STRU *pstXmlNode, VOS_CHAR *pcEFName, VOS_UINT32 *pulDataLen)
{
    VOS_UINT8           *pucContent;
    VOS_UINT32          ulDataLen;
    OAM_XML_NODE_STRU   *pstXMLNopde;

    pstXMLNopde = OAM_XML_GetChildNodeByAttributeValue(pstXmlNode, pcEFName);

    if(VOS_NULL_PTR == pstXMLNopde)
    {
        PIH_WARNING_LOG("SI_PIH_GetVSimFileContent: OAM_XML_GetChildNodeByAttributeValue failed!");

        return VOS_NULL_PTR;
    }

    if((VOS_NULL == pstXMLNopde->ulValueLength)||(VOS_NULL_PTR == pstXMLNopde->pcNodeValue))
    {
        PIH_WARNING_LOG("SI_PIH_GetVSimFileContent: File Content is NULL.");

        return VOS_NULL_PTR;
    }

    pucContent = (VOS_UINT8*)VOS_MemAlloc(MAPS_PIH_PID, DYNAMIC_MEM_PT, pstXMLNopde->ulValueLength);

    if(VOS_NULL_PTR == pucContent)
    {
        PIH_WARNING_LOG("SI_PIH_GetVSimFileContent: VOS_MemAlloc is NULL!");

        return VOS_NULL_PTR;
    }

    /*���������ַ�*/
    if(VOS_OK != OAM_XML_GetIntArray(pstXMLNopde->pcNodeValue, pstXMLNopde->ulValueLength, pucContent, &ulDataLen))
    {
        PIH_WARNING_LOG("SI_PIH_GetVSimFileContent: OAM_XML_GetIntArray is Failed!");

        VOS_MemFree(MAPS_PIH_PID, pucContent);

        return VOS_NULL_PTR;
    }

    *pulDataLen = ulDataLen;

    return pucContent;
}
VOS_UINT32 SI_PIH_GetUsimUstState(VOS_UINT8 *pucUst, VOS_UINT32 ulUstLen, VOS_UINT32 ulBitNo)
{
    VOS_UINT32          ulResult;

    ulResult = USIMM_GetBitFromBuf(pucUst, ulBitNo, ulUstLen);

    if(USIMM_BITNOFOUNE == ulResult)
    {
        PIH_WARNING_LOG("SI_PIH_GetUsimUstState: The Service is Out of File");

        return PS_USIM_SERVICE_NOT_AVAILIABLE;
    }

    return ulResult;
}


VOS_UINT32 SI_PIH_DecodeVSimFile(VOS_UINT8 *pucXMLData, SI_PIH_CARD_CONTENT_STRU *pstCardCont)
{
    OAM_XML_CTX_STRU                    stXmlCtx;
    OAM_XML_NODE_STRU                  *pstXmlNode;    /*���ڵ�*/
    VOS_UINT8                          *pucData;
    USIMM_CARD_TYPE_ENUM_UINT32         enCardType;     /*��ǰ������*/
    VOS_UINT32                          ulDataLen;
    VOS_UINT8                           ucHplmnMncLen;

    /*����xml�ļ�*/
    VOS_MemSet(&stXmlCtx, 0x00, sizeof(stXmlCtx));

    /* ����XML������������,����XML�ṹ�� */
    if (OAM_XML_RESULT_SUCCEED != OAM_XML_InitXMLCtx(&stXmlCtx))
    {
        PIH_WARNING_LOG("SI_PIHDecodeVSimFile: Xml initialization failed!");

        return VOS_ERR;
    }

    pstXmlNode = OAM_XML_BuildXMLTree((VOS_CHAR*)pucXMLData, &stXmlCtx);

    if (VOS_NULL_PTR == pstXmlNode)
    {
        PIH_WARNING_LOG("SI_PIHDecodeVSimFile: Build xml tree failed!");

        OAM_XML_ClearXMLCtx(&stXmlCtx);

        return VOS_ERR;
    }

    /*�����ļ����ڵ�*/
    if(VOS_OK != USIMM_DecodeVsimRoot(pstXmlNode, &enCardType, VOS_NULL_PTR))
    {
        PIH_WARNING_LOG("SI_PIHDecodeVSimFile: USIMM_DecodeVsimRoot failed!");

        OAM_XML_ClearXMLCtx(&stXmlCtx);

        return VOS_ERR;
    }

    pstCardCont->ucCardType = (VOS_UINT8)PIH_BIT_N(enCardType);

    /*��ѯEFAD*/
    pucData = SI_PIH_GetVSimFileContent(pstXmlNode, "EFAD", &ulDataLen);

    if(VOS_NULL_PTR == pucData)
    {
        PIH_WARNING_LOG("SI_PIHDecodeVSimFile: Find EFAD failed!");

        OAM_XML_ClearXMLCtx(&stXmlCtx); /*���XML*/

        return VOS_ERR;
    }

    /* ��ȡMNC���� */
    ucHplmnMncLen = SI_PIH_GetMncLen(pucData, ulDataLen);

    VOS_MemFree(MAPS_PIH_PID, pucData);     /*�ͷ�EFAD ���ڴ�*/

    /*��ѯ EFIMSI*/
    pucData = SI_PIH_GetVSimFileContent(pstXmlNode, "EFIMSI", &ulDataLen);

    if(VOS_NULL_PTR == pucData)
    {
        PIH_WARNING_LOG("SI_PIHDecodeVSimFile: find EFIMSI failed!");

        OAM_XML_ClearXMLCtx(&stXmlCtx);     /*���XML*/

        return VOS_ERR;
    }

    /* EFIMSI �ļ����ȴ��� */
    if ((USIMM_EF_IMSI_LEN != ulDataLen)&&(SI_PIH_IMSI_MAX_LEN < pucData[0]))
    {
        PIH_WARNING_LOG("SI_PIHDecodeVSimFile: EF IMSI Len Error!");

        OAM_XML_ClearXMLCtx(&stXmlCtx);     /*���XML*/

        VOS_MemFree(MAPS_PIH_PID, pucData); /*�ͷ�EFIMSI ���ڴ�*/

        return VOS_ERR;
    }

    /*���� IMSI*/
    SI_PIH_DecodeImsiHPLMN(pucData, ucHplmnMncLen, pstCardCont);

    VOS_MemFree(MAPS_PIH_PID, pucData);     /*�ͷ�EFIMSI ���ڴ�*/

    if (USIMM_CARD_SIM == enCardType)
    {
        PIH_NORMAL_LOG("SI_PIHDecodeVSimFile: SIM Card do not need read EHPLMN.");

        OAM_XML_ClearXMLCtx(&stXmlCtx);     /*���XML*/

        return VOS_OK;
    }

    /*��� EHPLMN�Ƿ�֧��*/
    if(VOS_FALSE == SI_PIH_GetEHPlmnNVState())
    {
        PIH_NORMAL_LOG("SI_PIHDecodeVSimFile: SI_PIH_GetEHPlmnNVState return not support.");

        OAM_XML_ClearXMLCtx(&stXmlCtx);     /*���XML*/

        return VOS_OK;
    }

    /*��ѯ EFUST*/
    pucData = SI_PIH_GetVSimFileContent(pstXmlNode, "EFUST", &ulDataLen);

    if(VOS_NULL_PTR == pucData)
    {
        PIH_WARNING_LOG("SI_PIHDecodeVSimFile: Find EF UST failed!");

        OAM_XML_ClearXMLCtx(&stXmlCtx);

        return VOS_ERR;
    }

    /*��ѯ EFUST ����״̬*/
    if(PS_USIM_SERVICE_NOT_AVAILIABLE == SI_PIH_GetUsimUstState(pucData, ulDataLen, USIM_SVR_EHPLMN))
    {
        PIH_WARNING_LOG("SI_PIHDecodeVSimFile: Find EF UST failed!");

        OAM_XML_ClearXMLCtx(&stXmlCtx);     /*���XML*/

        VOS_MemFree(MAPS_PIH_PID, pucData); /*�ͷ�EF UST ���ڴ�*/

        return VOS_OK;
    }

    VOS_MemFree(MAPS_PIH_PID, pucData);     /*�ͷ�EF UST ���ڴ�*/

    /*��ѯ EF EHPLMN */
    pucData = SI_PIH_GetVSimFileContent(pstXmlNode, "EFEHPLMN", &ulDataLen);

    if(VOS_NULL_PTR == pucData)
    {
        PIH_WARNING_LOG("SI_PIHDecodeVSimFile: Find EF EHPLMN failed!");

        OAM_XML_ClearXMLCtx(&stXmlCtx);

        return VOS_OK;
    }

    SI_PIH_DecodeEHPLMN(pucData, ulDataLen, pstCardCont);

    OAM_XML_ClearXMLCtx(&stXmlCtx);    /*���XML*/

    VOS_MemFree(MAPS_PIH_PID, pucData);/*�ͷ�EF EHPLMN ���ڴ�*/

    return VOS_OK;
}
VOS_VOID SI_PIH_GetVSimContent(SI_PIH_CARD_CONTENT_STRU *pstCardCont)
{
    VOS_UINT8                           *pucXMLData;

    pstCardCont->ucIndex   = SI_PIH_SIM_VIRT_SIM1;
    pstCardCont->ucCardCap = PIH_BIT_N(USIMM_CARD_SIM) | PIH_BIT_N(USIMM_CARD_USIM);

    /* �ж����⿨�Ƿ���� */
    pucXMLData = USIMM_ReadVsimFile(MAPS_PIH_PID);

    if(VOS_NULL_PTR == pucXMLData)
    {
        PIH_WARNING_LOG("SI_PIH_GetVSimContent: USIMM_ReadVsimFile is failed.");
        return;
    }

    if(VOS_OK != SI_PIH_DecodeVSimFile(pucXMLData, pstCardCont))
    {
        PIH_WARNING_LOG("SI_PIH_GetVSimContent: SI_PIH_DecodeVSimFile is failed.");
    }

    /* pucXMLData�ڴ���USIMM_ReadVsimFile���䣬��Ҫ�����ͷŲ��� */
    VOS_MemFree(MAPS_PIH_PID, pucXMLData);

    return;
}

/*****************************************************************************
�� �� ��  : SI_PIH_HVSSTQueryHandle
��������  : AT/APP HVSST Query��������
�������  : pMsg ��Ϣ����
�������  : ��
�� �� ֵ  : SI_UINT32 ����ִ�н��
���ú���  : ��
��������  : �ⲿ�ӿ�
History     :
1.��    ��  : 2013��03��20��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_UINT32 SI_PIH_HVSSTQueryHandle(SI_PIH_MSG_HEADER_STRU *pMsg)
{
    SI_PIH_EVENT_INFO_STRU  stEvent;
    VOS_BOOL                bVSimState;
    VOS_UINT8               ucCardState;

    stEvent.PIHError = TAF_ERR_NO_ERROR;  /*��ѯ����Ĭ�Ϸ���OK*/

    bVSimState = USIMM_VsimIsActive();

#if defined (INSTANCE_1)
    stEvent.PIHEvent.HVSSTQueryCnf.enSlot = SI_PIH_SIM_SLOT2;   /*ʹ��Modem_1ʱ�����ؿ���2*/
#else
    stEvent.PIHEvent.HVSSTQueryCnf.enSlot = SI_PIH_SIM_SLOT1;   /*ʹ��Modem_0ʱ�����ؿ���1*/
#endif  /*end of defined (INSTANCE_1)*/

    USIMM_GetCardType(&ucCardState, VOS_NULL_PTR);

    /*vSIM״̬���*/
    if(VOS_TRUE == bVSimState)
    {
        stEvent.PIHEvent.HVSSTQueryCnf.enVSimState = SI_PIH_SIM_ENABLE;
    }
    else
    {
        stEvent.PIHEvent.HVSSTQueryCnf.enVSimState = SI_PIH_SIM_DISABLE;
    }

    /*ֻҪ�����޿�������Ϊ����*/
    if(USIMM_CARD_SERVIC_ABSENT == ucCardState)
    {
        stEvent.PIHEvent.HVSSTQueryCnf.enCardUse   = SI_PIH_CARD_NOUSE;
    }
    else
    {
        stEvent.PIHEvent.HVSSTQueryCnf.enCardUse   = SI_PIH_CARD_USE;
    }

    /*����ظ�*/
    return SI_PIH_EventCallBack(&stEvent);
}

/*****************************************************************************
�� �� ��  : SI_PIH_HVSSTStateCheck
��������  : �ж��Ƿ���Ҫ���м����ȥ���������״̬���
�������  : bIsHandleVSim  �������Ƿ������⿨
            enIsActiveCard �����ȥ�������
            pstEvent       ���ظ�ATģ����¼�
�������  : ��
�� �� ֵ  : VOS_TRUE : ��Ҫ�����ȥ�������
            VOS_FALSE: ����Ҫ�����ȥ�������
���ú���  : ��
��������  : �ⲿ�ӿ�
History     :
1.��    ��  : 2013��03��20��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_BOOL SI_PIH_HVSSTStateCheck(SI_PIH_HVSST_REQ_STRU *pMsg, SI_PIH_EVENT_INFO_STRU *pstEvent)
{
    SI_PIH_HVSST_HANDLE_STATE_ENUM_UINT8    enValue = 0;
    VOS_UINT8                               ucCardState;
    VOS_BOOL                                bVsimState;

    if(SI_PIH_SIM_ENABLE == pMsg->stHvSSTData.enSIMSet)
    {
        PIH_NORMAL_LOG("SI_PIH_HVSSTStateCheck: Active Card:");

        PIH_SET_BIT(enValue, BIT_ACTIVECARD);
    }

    /*�жϵ�ǰ�Ĳ������Ƿ�vSIM��*/
    if(pMsg->stHvSSTData.ucIndex != SI_PIH_SIM_REAL_SIM1)
    {
        PIH_NORMAL_LOG("SI_PIH_HVSSTStateCheck: Handle VSIM Card:");

        PIH_SET_BIT(enValue, BIT_HANDLEVSIM);
    }

    bVsimState = USIMM_VsimIsActive();

    if(VOS_TRUE == bVsimState)
    {
        PIH_NORMAL_LOG("SI_PIH_HVSSTStateCheck: Vsim is Active");

        PIH_SET_BIT(enValue, BIT_VSIMSTATE);
    }

    USIMM_GetCardType(&ucCardState, VOS_NULL_PTR);

    if(USIMM_CARD_SERVIC_ABSENT != ucCardState)
    {
        PIH_NORMAL1_LOG("SI_PIH_HVSSTStateCheck: Card is Active %d", (long)ucCardState);

        PIH_SET_BIT(enValue, BIT_CURCARDOK);
    }

    PIH_NORMAL1_LOG("SI_PIH_HVSSTStateCheck: HVSST Bit Map is :", (long)enValue);

    /*����Ҫ�����ķ��أ�������ȷ�ظ�*/
    if((SI_PIH_HVSST_DEACTIVE_RSIM_AGAIN        == enValue)
        ||(SI_PIH_HVSST_DEACTIVE_VSIM_AGAIN     == enValue)
        ||(SI_PIH_HVSST_ACTIVE_VSIM_AGAIN2      == enValue))
    {
        pstEvent->PIHError = TAF_ERR_NO_ERROR;

        return VOS_FALSE;
    }
    /*��Ҫ�����ķ���*/
    if((SI_PIH_HVSST_DEACTIVE_RSIM     == enValue)
        ||(SI_PIH_HVSST_DEACTIVE_VSIM       == enValue)
        ||(SI_PIH_HVSST_ACTIVE_RSIM         == enValue)
        ||(SI_PIH_HVSST_ACTIVE_VSIM         == enValue)
        ||(SI_PIH_HVSST_ACTIVE_VSIM_AGAIN   == enValue)
        ||(SI_PIH_HVSST_ACTIVE_RSIM_AGAIN2  == enValue))
    {
        return VOS_TRUE;
    }

    pstEvent->PIHError = TAF_ERR_PARA_ERROR;

    PIH_WARNING_LOG("SI_PIH_HVSSTStateCheck: Not able to Process Command");

    return VOS_FALSE;
}

/*****************************************************************************
�� �� ��  : SI_PIH_HVSSTQueryHandle
��������  : AT/APP HVSST Set��������
�������  : pMsg ��Ϣ����
�������  : ��
�� �� ֵ  : SI_UINT32 ����ִ�н��
���ú���  : ��
��������  : �ⲿ�ӿ�
History     :
1.��    ��  : 2013��03��20��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_UINT32 SI_PIH_HVSSTSetHandle(SI_PIH_HVSST_REQ_STRU *pMsg)
{
    SI_PIH_EVENT_INFO_STRU              stEvent;
    VOS_UINT32                          ulResult;
    SI_PIH_SIM_STATE_ENUM_UINT8         enVSIMSet;

    stEvent.PIHError = TAF_ERR_NO_ERROR; /* �ȸ�ֵ�ɹ� */

    ulResult = SI_PIH_HVSSTStateCheck(pMsg, &stEvent);

    /* �жϵ�ǰ״̬�Ƿ���Ҫ���п����� */
    if (VOS_FALSE == ulResult)
    {
        SI_PIH_EventCallBack(&stEvent);

        PIH_NORMAL_LOG("SI_PIH_HVSSTSetHandle: No need handle this msg");

        return VOS_OK;
    }

    /*��Ҫ��״̬��¼��NV��*/
    if(SI_PIH_SIM_ENABLE == pMsg->stHvSSTData.enSIMSet) /*ʹ��SIM ��*/
    {
        if(pMsg->stHvSSTData.ucIndex == SI_PIH_SIM_REAL_SIM1)   /*Ӳ��*/
        {
            enVSIMSet = SI_PIH_SIM_DISABLE;
        }
        else
        {
            enVSIMSet = SI_PIH_SIM_ENABLE;
        }

        if(NV_OK != NV_WritePart(en_NV_Item_VSIM_SUPPORT_FLAG, 0, &enVSIMSet, sizeof(VOS_UINT8)))
        {
            stEvent.PIHError = TAF_ERR_CAPABILITY_ERROR;

            SI_PIH_EventCallBack(&stEvent);

            return VOS_ERR;
        }
    }

    /*������ʼ����USIMM�ӿ�*/
    if(SI_PIH_SIM_ENABLE == pMsg->stHvSSTData.enSIMSet)
    {
        PIH_NORMAL_LOG("SI_PIH_HVSSTSetHandle: Active SIM Card");

        if(USIMM_API_SUCCESS != USIMM_ActiveCardReq(MAPS_PIH_PID))
        {
            stEvent.PIHError = TAF_ERR_ERROR;  /*��ֵʧ��*/

            PIH_WARNING_LOG("SI_PIH_HVSSTSetHandle: USIMM_ActiveCardReq Return Error");

            return SI_PIH_EventCallBack(&stEvent);
        }

        if(pMsg->stHvSSTData.ucIndex == SI_PIH_SIM_REAL_SIM1)
        {
            g_stPIHProtectCtrl.enProtectFun = SI_PIH_PROTECT_ENABLE;    /*����Ӳ�����´򿪱����Ը�λ����*/
        }
    }
    else
    {
        PIH_NORMAL_LOG("SI_PIH_HVSSTSetHandle: Deactive SIM Card");

        if(USIMM_API_SUCCESS != USIMM_DeactiveCardReq(MAPS_PIH_PID))
        {
            stEvent.PIHError = TAF_ERR_ERROR;  /*��ֵʧ��*/

            PIH_WARNING_LOG("SI_PIH_HVSSTSetHandle: USIMM_DeactiveCardReq Return Error");

            return SI_PIH_EventCallBack(&stEvent);
        }

        /* Modem���µ������¿��ܷ�������ȥ���������ȥ����ǰӦ�����Դ�����0*/
        g_ulPIHDeactiveRetryCnt = 0;
        /*USIMM_DeactiveCardReq��ʵ��Ϊ�첽���̣���Ҫ������ʱ�����ȴ��µ�������*/
        VOS_StartRelTimer(&g_stPIHDeactiveCardTimer,
                           MAPS_PIH_PID,
                           SI_PIH_DEACTIVE_CARD_TIME,
                           SI_PIH_TIMER_NAME_DEACTIVECARD,
                           0,
                           VOS_RELTIMER_NOLOOP,
                           VOS_TIMER_PRECISION_10);
    }

    PIH_NORMAL_LOG("SI_PIH_HVSSTSetHandle: Wait USIMM Return Msg");

    return VOS_OK;
}
VOS_UINT32 SI_PIH_HvsDHSetHandle(VOS_UINT32 ulDataLen, VOS_UINT8 *pucData)
{
    NVIM_VSIM_HVSDH_NV_STRU             stDhNv;
    SI_PIH_EVENT_INFO_STRU              stEvent;
    VOS_UINT32                          ulResult;

    stEvent.PIHError = TAF_ERR_ERROR;

    /* ��NV���ж�����Կ��Ϣ */
    ulResult = NV_Read(en_NV_Item_VSIM_HVSDH_INFO, &stDhNv, sizeof(stDhNv));

    if (NV_OK != ulResult)
    {
        PIH_ERROR1_LOG("SI_PIH_HvsDHSetReq: NV_Read fail.", (VOS_INT32)ulResult);

        return SI_PIH_EventCallBack(&stEvent);
    }

    VOS_MemCpy(stDhNv.stSPublicKey.aucKey, pucData, ulDataLen);

    stDhNv.stSPublicKey.ulKeyLen = ulDataLen;

    /* ����������Կд��NV���� */
    ulResult = NV_Write(en_NV_Item_VSIM_HVSDH_INFO, &stDhNv, sizeof(stDhNv));

    if (NV_OK != ulResult)
    {
        PIH_ERROR1_LOG("SI_PIH_HvsDHSetReq: NV_Write fail.", (VOS_INT32)ulResult);

        return SI_PIH_EventCallBack(&stEvent);
    }

    stEvent.PIHError = TAF_ERR_NO_ERROR;

    return SI_PIH_EventCallBack(&stEvent);
}


VOS_UINT32 SI_PIH_HvsDHQueryHandle(VOS_VOID)
{
    NVIM_VSIM_HVSDH_NV_STRU             stDhNv;
    SI_PIH_EVENT_INFO_STRU              stEvent;
    VOS_UINT32                          ulResult;
    VOS_UINT8                           aucPubKey[VSIM_DH_PUBIIC_KEY] = {0};
    DH_KEY                              stDhKey;

    VOS_MemSet(&stDhKey, 0, sizeof(stDhKey));

    stEvent.PIHError = TAF_ERR_ERROR;

    /* ��NV����ȡ����Կ��Ϣ */
    ulResult = NV_Read(en_NV_Item_VSIM_HVSDH_INFO, &stDhNv, sizeof(stDhNv));

    if (NV_OK != ulResult)
    {
        PIH_ERROR1_LOG("SI_PIH_HvsDHQryReq: NV_Read fail.", (VOS_INT32)ulResult);

        return SI_PIH_EventCallBack(&stEvent);
    }

    /* ���絥�����ԿΪ�գ������DH�ӿ�������Կ */
    if ((VOS_NULL == stDhNv.stCPrivateKey.ulKeyLen)
        ||(VSIM_DH_PRIVATE_KEY < stDhNv.stCPrivateKey.ulKeyLen)
        ||(VOS_NULL == stDhNv.stCPublicKey.ulKeyLen)
        ||(VSIM_DH_PUBIIC_KEY < stDhNv.stCPublicKey.ulKeyLen))
    {
        /* ��ʼ����Կ���� */
        DH_FillFixParams(&stDhKey);

        /* ����modem��Կ��˽Կ */
        ulResult = (VOS_UINT32)DH_GeneratePublicValue(aucPubKey, VSIM_DH_PRIVATE_KEY, &stDhKey);

        if (VOS_OK != ulResult)
        {
            PIH_ERROR1_LOG("SI_PIH_HvsDHQryReq: DH_GeneratePublicValue fail.", (VOS_INT32)ulResult);

            return SI_PIH_EventCallBack(&stEvent);
        }

        VOS_MemCpy(stDhNv.stCPublicKey.aucKey, aucPubKey, VSIM_DH_PUBIIC_KEY);

        stDhNv.stCPublicKey.ulKeyLen = VSIM_DH_PUBIIC_KEY;

        VOS_MemCpy(stDhNv.stCPrivateKey.aucKey, stDhKey.privateValue, VSIM_DH_PRIVATE_KEY);

        stDhNv.stCPrivateKey.ulKeyLen = VSIM_DH_PRIVATE_KEY;

        /* ���������Կд��NV���� */
        ulResult = NV_Write(en_NV_Item_VSIM_HVSDH_INFO, &stDhNv, sizeof(stDhNv));

        if (NV_OK != ulResult)
        {
            PIH_ERROR1_LOG("SI_PIH_HvsDHQryReq: NV_Write fail.", (VOS_INT32)ulResult);

            return SI_PIH_EventCallBack(&stEvent);
        }
    }

    /* �������㷨����ظ���Ϣ�� */
    stEvent.PIHEvent.HvsDHCnf.enAlgorithm = stDhNv.enAlgorithm;

    /* ��������๫Կ */
    VOS_MemCpy(&stEvent.PIHEvent.HvsDHCnf.stCkey, &stDhNv.stCPublicKey,
                    sizeof(stEvent.PIHEvent.HvsDHCnf.stCkey));

    /* ������������Կ */
    VOS_MemCpy(&stEvent.PIHEvent.HvsDHCnf.stSkey, &stDhNv.stSPublicKey,
                    sizeof(stEvent.PIHEvent.HvsDHCnf.stSkey));

    stEvent.PIHError = TAF_ERR_NO_ERROR;

    return SI_PIH_EventCallBack(&stEvent);
}


VOS_UINT32 SI_PIH_HvsContQueryHandle(VOS_VOID)
{
    SI_PIH_EVENT_INFO_STRU              stEvent;

    VOS_MemSet(&stEvent, 0, sizeof(stEvent));

    /* ����Ӳ�������� */
    SI_PIH_GetRSimContent(&stEvent.PIHEvent.HvsContCnf.astSimCard[SI_PIH_RSIM]);

    /* ������������ */
    SI_PIH_GetVSimContent(&stEvent.PIHEvent.HvsContCnf.astSimCard[SI_PIH_VSIM]);

    stEvent.PIHError = TAF_ERR_NO_ERROR;

    return SI_PIH_EventCallBack(&stEvent);
}

/*****************************************************************************
�� �� ��  : SI_PIH_ActiveSimCnfProc
��������  :
�������  : pstMsg
�������  : ��
�� �� ֵ  : SI_UINT32 ����ִ�н��
���ú���  : ��
��������  : �ⲿ�ӿ�
History     :
1.��    ��  : 2010��08��20��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_VOID SI_PIH_ActiveSimCnfProc(PS_USIM_SINGLECMD_CNF_STRU *pMsg)
{
    SI_PIH_EVENT_INFO_STRU  stEvent;

    if(VOS_OK == pMsg->ulResult)
    {
        stEvent.PIHError = TAF_ERR_NO_ERROR;
    }
    else
    {
        stEvent.PIHError = TAF_ERR_SIM_FAIL;
    }

    SI_PIH_EventCallBack(&stEvent);

    return;
}


VOS_VOID SI_PIH_ReDhNegotiateIndProc()
{
    SI_PIH_EVENT_INFO_STRU              stEvent;

    VOS_MemSet(&stEvent, 0, sizeof(SI_PIH_EVENT_INFO_STRU));

    stEvent.ClientId  = MN_CLIENT_ALL;/*�����ֵ*/
    stEvent.EventType = SI_PIH_EVENT_HVRDH_IND;
    stEvent.PIHError  = TAF_ERR_NO_ERROR;

    stEvent.PIHEvent.HvrdhInd.ulReDhFlag = VOS_TRUE;

    SI_PIHCallback(&stEvent);

    return;
}
VOS_UINT32 SI_PIH_FwriteParaCheck(SI_PIH_FILE_WRITE_REQ_STRU *pstMsg)
{
    /* ������� */
    if(SI_PIH_FILE_START_INDEX == pstMsg->ucIndex)  /*д���һ��*/
    {
        PIH_NORMAL_LOG("SI_PIH_FwriteParaCheck: Clear the Global Para.");

        if(VOS_NULL_PTR != g_stPIHFileWriteGlobal.fpFile)
        {
            PIH_NORMAL_LOG("SI_PIH_FwriteParaCheck: The File is not Close.");

            USIMM_File_Close(g_stPIHFileWriteGlobal.fpFile);
        }

        VOS_MemSet(&g_stPIHFileWriteGlobal, 0, sizeof(g_stPIHFileWriteGlobal));

    }
    else if(pstMsg->ucIndex > SI_PIH_FILE_START_INDEX)  /*д��ǵ�һ��*/
    {
        if(g_stPIHFileWriteGlobal.usRefNum != pstMsg->ucRef)
        {
           PIH_ERROR_LOG("SI_PIH_FwriteParaCheck: usRefNum is not same.");
           return VOS_ERR;
        }

        if((g_stPIHFileWriteGlobal.usIndex+1) != pstMsg->ucIndex)
        {
           PIH_ERROR_LOG("SI_PIH_FwriteParaCheck: Index is Error.");
           return VOS_ERR;
        }

        if(g_stPIHFileWriteGlobal.usTotalNum != pstMsg->ucTotalNum)
        {
           PIH_ERROR_LOG("SI_PIH_FwriteParaCheck: usTotalNum is not same.");
           return VOS_ERR;
        }
    }
    else    /*make pclint pass*/
    {
        PIH_ERROR_LOG("SI_PIH_FwriteParaCheck: Code Run Error.");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_UINT32 SI_PIH_ATFileWriteHandle(SI_PIH_FILE_WRITE_REQ_STRU *pstMsg)
{
    SI_PIH_EVENT_INFO_STRU              stEvent;
    VOS_INT32                          ulResult;
    VOS_UINT8                          ucCardState;

    USIMM_GetCardType(&ucCardState, VOS_NULL_PTR);

    /* ��ȡNV���ж�vsim�Ƿ���ʹ�� */
    if((VOS_TRUE == USIMM_VsimIsActive()) && (USIMM_CARD_SERVIC_ABSENT != ucCardState))
    {
        PIH_ERROR_LOG("SI_PIH_FileWriteProc: vSIM File is using.");

        stEvent.PIHError = TAF_ERR_SIM_BUSY;

        return SI_PIH_EventCallBack(&stEvent);
    }

    stEvent.PIHError = TAF_ERR_PARA_ERROR;

    if(VOS_OK != SI_PIH_FwriteParaCheck(pstMsg))    /*chenck gobal para*/
    {
        PIH_ERROR_LOG("SI_PIH_FileWriteProc: SI_PIH_FwriteParaCheck is Failed.");

        return SI_PIH_EventCallBack(&stEvent);
    }

    /* �ж��Ƿ���Ҫ���ļ� */
    if(VOS_NULL_PTR == g_stPIHFileWriteGlobal.fpFile)
    {
        /* ������ǵ�һ�����ش��� */
        if(SI_PIH_FILE_START_INDEX != pstMsg->ucIndex)
        {
            PIH_ERROR_LOG("SI_PIH_FileWriteProc: it is not first packages.");

            return SI_PIH_EventCallBack(&stEvent);
        }

        /* ��Ŀ¼�������򴴽� */
        if (VOS_OK != DRV_FILE_ACCESS(VSIM_XML_DIR_PATH, 0))
        {
            USIMM_Mkdir(VSIM_XML_DIR_PATH);
        }

        DRV_FILE_RMFILE(VSIM_XML_FILE_PATH);

        DRV_FILE_RMFILE(VSIM_XML_TEMP_PATH);

        g_stPIHFileWriteGlobal.fpFile = USIMM_File_Open(VSIM_XML_TEMP_PATH, USIMM_FILE_OPEN_MODE_ADD_W);

        if (VOS_NULL_PTR == g_stPIHFileWriteGlobal.fpFile)
        {
            stEvent.PIHError = TAF_ERR_NULL_PTR;

            PIH_ERROR_LOG("SI_PIH_FileWriteProc: XML Temp file open is Failed.");

            return SI_PIH_EventCallBack(&stEvent);
        }

        /* ����ȫ�ֱ��� */
        g_stPIHFileWriteGlobal.usRefNum = pstMsg->ucRef;
        g_stPIHFileWriteGlobal.usTotalNum = pstMsg->ucTotalNum;
    }

    ulResult = USIMM_File_Write(pstMsg->aucFileData, pstMsg->ulFileDataLen, VSIM_FILE_WRITE_COUNT, g_stPIHFileWriteGlobal.fpFile);

    stEvent.PIHError = TAF_ERR_ERROR;

    if (VSIM_FILE_WRITE_COUNT != ulResult)
    {
        PIH_ERROR_LOG("SI_PIH_FileWriteProc: XML Temp file Write is Failed.");

        return SI_PIH_EventCallBack(&stEvent);
    }

    g_stPIHFileWriteGlobal.usIndex  = pstMsg->ucIndex;

    /* �������һ�� */
    if(pstMsg->ucIndex == pstMsg->ucTotalNum)
    {
        if (VOS_OK != USIMM_File_Close(g_stPIHFileWriteGlobal.fpFile) )
        {
            PIH_ERROR_LOG("SI_PIH_FileWriteProc: XML Temp file Close is Failed.");

            return SI_PIH_EventCallBack(&stEvent);
        }

        VOS_MemSet(&g_stPIHFileWriteGlobal, 0, sizeof(g_stPIHFileWriteGlobal));

        USIMM_File_Rename(VSIM_XML_TEMP_PATH, VSIM_XML_FILE_PATH);
    }

    stEvent.PIHError = TAF_ERR_NO_ERROR;

    return SI_PIH_EventCallBack(&stEvent);
}

#endif  /*(FEATURE_ON == FEATURE_VSIM)*/

#if (FEATURE_ON == FEATURE_IMS)

VOS_UINT32 SI_PIH_UiccAuthHandle(SI_PIH_UICCAUTH_REQ_STRU *pstMsg)
{
    USIMM_APP_TYPE_ENUM_UINT32      enAppType;
    USIMM_AUTH_DATA_STRU            stAuth;

    VOS_MemSet(&stAuth, 0, sizeof(USIMM_AUTH_DATA_STRU));

    if (SI_PIH_UICCAPP_USIM == pstMsg->stAuthData.enAppType)
    {
        enAppType           = USIMM_UMTS_APP;
    }
    else if (SI_PIH_UICCAPP_ISIM == pstMsg->stAuthData.enAppType)
    {
        enAppType           = USIMM_ISIM_APP;
    }
    else
    {
        return USIMM_API_WRONG_PARA;
    }

    if (SI_PIH_UICCAUTH_AKA == pstMsg->stAuthData.enAuthType)
    {
        if (SI_PIH_UICCAPP_USIM == pstMsg->stAuthData.enAppType)
        {
            stAuth.enAuthType   = USIMM_3G_AUTH;
        }
        else
        {
            stAuth.enAuthType   = USIMM_IMS_AUTH;
        }

        stAuth.unAuthPara.st3GAuth.stRand.ulDataLen = pstMsg->stAuthData.uAuthData.stAka.ulRandLen;
        stAuth.unAuthPara.st3GAuth.stRand.pucData   = pstMsg->stAuthData.uAuthData.stAka.aucRand;
        stAuth.unAuthPara.st3GAuth.stAutn.ulDataLen = pstMsg->stAuthData.uAuthData.stAka.ulAuthLen;
        stAuth.unAuthPara.st3GAuth.stAutn.pucData   = pstMsg->stAuthData.uAuthData.stAka.aucAuth;
    }

    if (SI_PIH_UICCAUTH_GBA == pstMsg->stAuthData.enAuthType)
    {
        stAuth.enAuthType   = USIMM_GBA_AUTH;

        stAuth.unAuthPara.stGbaAuth.stRand.ulDataLen= pstMsg->stAuthData.uAuthData.stAka.ulRandLen;
        stAuth.unAuthPara.stGbaAuth.stRand.pucData  = pstMsg->stAuthData.uAuthData.stAka.aucRand;
        stAuth.unAuthPara.stGbaAuth.stAutn.ulDataLen= pstMsg->stAuthData.uAuthData.stAka.ulAuthLen;
        stAuth.unAuthPara.stGbaAuth.stAutn.pucData  = pstMsg->stAuthData.uAuthData.stAka.aucAuth;
    }

    if (SI_PIH_UICCAUTH_NAF == pstMsg->stAuthData.enAuthType)
    {
        stAuth.enAuthType   = USIMM_NAF_AUTH;

        stAuth.unAuthPara.stNafAuth.stNafID.ulDataLen= pstMsg->stAuthData.uAuthData.stKsNAF.ulNAFIDLen;
        stAuth.unAuthPara.stNafAuth.stNafID.pucData  = pstMsg->stAuthData.uAuthData.stKsNAF.aucNAFID;
        stAuth.unAuthPara.stNafAuth.stImpi.ulDataLen = pstMsg->stAuthData.uAuthData.stKsNAF.ulIMPILen;
        stAuth.unAuthPara.stNafAuth.stImpi.pucData   = pstMsg->stAuthData.uAuthData.stKsNAF.aucIMPI;
    }

    return USIMM_AuthReq(MAPS_PIH_PID, enAppType, &stAuth);
}


VOS_UINT32 SI_PIH_AccessFileHandle(SI_PIH_ACCESSFILE_REQ_STRU *pstMsg)
{
    USIMM_APP_TYPE_ENUM_UINT32          enAppType;
    USIMM_SET_FILE_INFO_STRU            stSetFileInfo;
    USIMM_GET_FILE_INFO_STRU            stGetFileInfo;

    if (SI_PIH_UICCAPP_USIM == pstMsg->stCmdData.enAppType)
    {
        enAppType           = USIMM_UMTS_APP;
    }
    else if (SI_PIH_UICCAPP_ISIM == pstMsg->stCmdData.enAppType)
    {
        enAppType           = USIMM_ISIM_APP;
    }
    else
    {
        return USIMM_API_WRONG_PARA;
    }

    if (SI_PIH_ACCESS_READ == pstMsg->stCmdData.enCmdType)
    {
        stGetFileInfo.enAppType     = enAppType;
        stGetFileInfo.usEfId        = pstMsg->stCmdData.usEfId;
        stGetFileInfo.ucRecordNum   = pstMsg->stCmdData.ucRecordNum;

        return USIMM_GetFileReq(MAPS_PIH_PID, 0, &stGetFileInfo);
    }
    else
    {
        stSetFileInfo.enAppType     = enAppType;
        stSetFileInfo.usEfId        = pstMsg->stCmdData.usEfId;
        stSetFileInfo.ucRecordNum   = pstMsg->stCmdData.ucRecordNum;
        stSetFileInfo.ulEfLen       = pstMsg->stCmdData.ulDataLen;
        stSetFileInfo.pucEfContent  = pstMsg->stCmdData.aucCommand;

        return USIMM_SetFileReq(MAPS_PIH_PID, 0, &stSetFileInfo);
    }
}

/*****************************************************************************
�� �� ��  : SI_PIH_AuthCnfProc
��������  : Authentication cnf ��Ϣ������
�������  : pstMsg:Authenctication cnf ��Ϣ
�������  : ��
�� �� ֵ  : ��
History     :
1.��    ��  : 2014��03��20��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_VOID SI_PIH_AuthCnfProc(USIMM_AUTH_CNF_STRU *pstMsg)
{
    SI_PIH_EVENT_INFO_STRU              stEvent;

    VOS_MemSet(&stEvent, 0, sizeof(SI_PIH_EVENT_INFO_STRU));

    if (USIMM_UMTS_APP == pstMsg->enAppType)
    {
        stEvent.PIHEvent.UiccAuthCnf.enAppType = SI_PIH_UICCAPP_USIM;
    }
    else if (USIMM_ISIM_APP == pstMsg->enAppType)
    {
        stEvent.PIHEvent.UiccAuthCnf.enAppType = SI_PIH_UICCAPP_ISIM;
    }
    else
    {
        stEvent.PIHError = TAF_ERR_ERROR;
    }

    if (USIMM_3G_AUTH == pstMsg->enAuthType)
    {
        stEvent.PIHEvent.UiccAuthCnf.enAuthType = SI_PIH_UICCAUTH_AKA;

        VOS_MemCpy(stEvent.PIHEvent.UiccAuthCnf.stAkaData.aucAuthRes, pstMsg->cnfdata.stTELECnf.aucAuthRes, sizeof(pstMsg->cnfdata.stTELECnf.aucAuthRes));
        VOS_MemCpy(stEvent.PIHEvent.UiccAuthCnf.stAkaData.aucAuts, pstMsg->cnfdata.stTELECnf.aucAuts, sizeof(pstMsg->cnfdata.stTELECnf.aucAuts));
        VOS_MemCpy(stEvent.PIHEvent.UiccAuthCnf.stAkaData.aucCK, pstMsg->cnfdata.stTELECnf.aucCK, sizeof(pstMsg->cnfdata.stTELECnf.aucCK));
        VOS_MemCpy(stEvent.PIHEvent.UiccAuthCnf.stAkaData.aucIK, pstMsg->cnfdata.stTELECnf.aucIK, sizeof(pstMsg->cnfdata.stTELECnf.aucIK));
    }
    else if (USIMM_IMS_AUTH == pstMsg->enAuthType)
    {
        stEvent.PIHEvent.UiccAuthCnf.enAuthType = SI_PIH_UICCAUTH_AKA;

        VOS_MemCpy(stEvent.PIHEvent.UiccAuthCnf.stAkaData.aucAuthRes, pstMsg->cnfdata.stIMSCnf.aucAuthRes, sizeof(pstMsg->cnfdata.stIMSCnf.aucAuthRes));
        VOS_MemCpy(stEvent.PIHEvent.UiccAuthCnf.stAkaData.aucAuts, pstMsg->cnfdata.stIMSCnf.aucAuts, sizeof(pstMsg->cnfdata.stIMSCnf.aucAuts));
        VOS_MemCpy(stEvent.PIHEvent.UiccAuthCnf.stAkaData.aucCK, pstMsg->cnfdata.stIMSCnf.aucCK, sizeof(pstMsg->cnfdata.stIMSCnf.aucCK));
        VOS_MemCpy(stEvent.PIHEvent.UiccAuthCnf.stAkaData.aucIK, pstMsg->cnfdata.stIMSCnf.aucIK, sizeof(pstMsg->cnfdata.stIMSCnf.aucIK));
    }
    else if (USIMM_GBA_AUTH == pstMsg->enAuthType)
    {
        stEvent.PIHEvent.UiccAuthCnf.enAuthType = SI_PIH_UICCAUTH_GBA;

        VOS_MemCpy(stEvent.PIHEvent.UiccAuthCnf.stAkaData.aucAuthRes, pstMsg->cnfdata.stIMSCnf.aucAuthRes, sizeof(pstMsg->cnfdata.stIMSCnf.aucAuthRes));
    }
    else if (USIMM_NAF_AUTH == pstMsg->enAuthType)
    {
        stEvent.PIHEvent.UiccAuthCnf.enAuthType = SI_PIH_UICCAUTH_NAF;

        VOS_MemCpy(stEvent.PIHEvent.UiccAuthCnf.stNAFData.aucKs_ext_NAF, pstMsg->cnfdata.stIMSCnf.aucKs_ext_NAF, pstMsg->cnfdata.stIMSCnf.aucKs_ext_NAF[0]+1);
    }
    else
    {
        stEvent.PIHError = TAF_ERR_ERROR;
    }

    if ((USIMM_AUTH_UMTS_SUCCESS == pstMsg->enResult)
        ||(USIMM_AUTH_IMS_SUCCESS == pstMsg->enResult)
        ||(USIMM_AUTH_GSM_SUCCESS == pstMsg->enResult))
    {
        stEvent.PIHError = TAF_ERR_NO_ERROR;
        stEvent.PIHEvent.UiccAuthCnf.enStatus = SI_PIH_AUTH_SUCCESS;
    }
    else if(USIMM_AUTH_SYNC_FAILURE == pstMsg->enResult)
    {
        stEvent.PIHError = TAF_ERR_NO_ERROR;
        stEvent.PIHEvent.UiccAuthCnf.enStatus = SI_PIH_AUTH_SYNC;
    }
    else if(USIMM_AUTH_MAC_FAILURE == pstMsg->enResult)
    {
        stEvent.PIHError = TAF_ERR_NO_ERROR;
        stEvent.PIHEvent.UiccAuthCnf.enStatus = SI_PIH_AUTH_FAIL;
    }
    else
    {
        stEvent.PIHError = TAF_ERR_ERROR;
    }

    SI_PIH_EventCallBack(&stEvent);

    return;
}

/*****************************************************************************
�� �� ��  : SI_PIH_GetFileCnfProc
��������  : Get file cnf ��Ϣ������
�������  : pstMsg:Get file cnf ��Ϣ
�������  : ��
�� �� ֵ  : ��
History     :
1.��    ��  : 2014��03��20��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_VOID SI_PIH_GetFileCnfProc(PS_USIM_GET_FILE_CNF_STRU *pstMsg)
{
    SI_PIH_EVENT_INFO_STRU              stEvent;

    VOS_MemSet(&stEvent, 0, sizeof(SI_PIH_EVENT_INFO_STRU));

    stEvent.PIHEvent.UiccAcsFileCnf.enCmdType   = SI_PIH_ACCESS_READ;
    stEvent.PIHEvent.UiccAcsFileCnf.ulResult    = pstMsg->ulResult;
    stEvent.PIHEvent.UiccAcsFileCnf.usEfId      = pstMsg->usEfId;
    stEvent.PIHEvent.UiccAcsFileCnf.ucRecordNum = pstMsg->ucRecordNum;

    if (VOS_OK == pstMsg->ulResult)
    {
        /* coverity[cond_at_least] */
        stEvent.PIHEvent.UiccAcsFileCnf.ulDataLen = (pstMsg->usEfLen > USIMM_T0_APDU_MAX_LEN)?(USIMM_T0_APDU_MAX_LEN):pstMsg->usEfLen;

        stEvent.PIHError = TAF_ERR_NO_ERROR;
        /* coverity[overrun_buffer_arg] */
        VOS_MemCpy(stEvent.PIHEvent.UiccAcsFileCnf.aucCommand, pstMsg->aucEf, stEvent.PIHEvent.UiccAcsFileCnf.ulDataLen);
    }
    else
    {
        stEvent.PIHError = TAF_ERR_CME_OPT_NOT_SUPPORTED;
    }

    SI_PIH_EventCallBack(&stEvent);

    return;
}

/*****************************************************************************
�� �� ��  : SI_PIH_SetFileCnfProc
��������  : Set file cnf ��Ϣ������
�������  : pstMsg:Set file cnf ��Ϣ
�������  : ��
�� �� ֵ  : ��
History     :
1.��    ��  : 2014��03��20��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_VOID SI_PIH_SetFileCnfProc(PS_USIM_SET_FILE_CNF_STRU *pstMsg)
{
    SI_PIH_EVENT_INFO_STRU              stEvent;

    VOS_MemSet(&stEvent, 0, sizeof(SI_PIH_EVENT_INFO_STRU));

    if (VOS_OK == pstMsg->ulResult)
    {
        stEvent.PIHError = TAF_ERR_NO_ERROR;
    }
    else
    {
        stEvent.PIHError = TAF_ERR_CME_OPT_NOT_SUPPORTED;
    }

    stEvent.PIHEvent.UiccAcsFileCnf.enCmdType   = SI_PIH_ACCESS_WRITE;
    stEvent.PIHEvent.UiccAcsFileCnf.ulResult    = pstMsg->ulResult;
    stEvent.PIHEvent.UiccAcsFileCnf.usEfId      = pstMsg->usEfId;
    stEvent.PIHEvent.UiccAcsFileCnf.ucRecordNum = pstMsg->ucRecordNum;

    SI_PIH_EventCallBack(&stEvent);

    return;
}

#endif  /*(FEATURE_ON == FEATURE_VSIM)*/


VOS_UINT32 SI_PIH_CardTypeQueryHandle(SI_PIH_MSG_HEADER_STRU *pMsg)
{
    SI_PIH_EVENT_INFO_STRU  stEvent;

    VOS_MemSet(&stEvent, 0, sizeof(SI_PIH_EVENT_INFO_STRU));

    if (USIMM_API_SUCCESS == USIMM_GetCardCGType(&stEvent.PIHEvent.CardTypeCnf.ucMode, 
                                                &stEvent.PIHEvent.CardTypeCnf.ucHasCModule, 
                                                &stEvent.PIHEvent.CardTypeCnf.ucHasGModule))
    {
        stEvent.PIHError = TAF_ERR_NO_ERROR;
    }
    else
    {
        stEvent.PIHError = TAF_ERR_PARA_ERROR;
    }

    /*����ظ�*/
    return SI_PIH_EventCallBack(&stEvent);
}
VOS_UINT32  SI_PIH_MsgProc(PS_SI_MSG_STRU *pMsg)
{
    VOS_UINT32              ulResult;
    SI_PIH_MSG_HEADER_STRU  *pstMsg;

    pstMsg = (SI_PIH_MSG_HEADER_STRU*)pMsg;

    /*������Ϣ�����¼����ֱ�ӷ���*/
    if ((SI_PIH_USIMREG_PID_HOOK == pMsg->ulMsgName)
        ||(SI_PIH_REFRESHREG_PID_HOOK == pMsg->ulMsgName)
        ||(SI_PIH_ISIMREG_PID_HOOK == pMsg->ulMsgName))
    {
        return VOS_OK;
    }

    ulResult = SI_PIH_ErrorCheck();     /*��鵱ǰ����Ϣ�Ƿ����ִ��*/

    if(VOS_OK != ulResult)
    {
        PIH_WARNING1_LOG("SI_SIM_AppMsgProc:WARNING:EventType:", (VOS_INT32)pstMsg->ulEventType);

        SI_PIH_EventReportError(pstMsg->usClient, pstMsg->ucOpID, ulResult, pMsg->ulMsgName, pstMsg->ulEventType);

        return ulResult;
    }

    /*����ִ����Ϣ����ز����������ٺ����Ļظ�ʱ����*/
    g_stPIHCtrlInfo.enPIHLock   = SI_PIH_LOCKED;
    g_stPIHCtrlInfo.usClient    = pstMsg->usClient;
    g_stPIHCtrlInfo.ucOpID      = pstMsg->ucOpID;
    g_stPIHCtrlInfo.enCmdType   = pstMsg->ulMsgName;
    g_stPIHCtrlInfo.ulEventType = pstMsg->ulEventType;

    switch(pMsg->ulMsgName)
    {
        case SI_PIH_FDN_ENABLE_REQ:
            PIH_NORMAL_LOG("SI_SIM_AppMsgProc:NORMAL:FDN_ENABLE_REQ.");
            ulResult = USIMM_FDNHandleReq(MAPS_PIH_PID, USIMM_FBDN_ACTIVE,
                                                ((SI_PIH_FDN_ENABLE_REQ_STRU*)pMsg)->aucPIN2);
            break;

        case SI_PIH_FDN_DISALBE_REQ:
            PIH_NORMAL_LOG("SI_SIM_AppMsgProc:NORMAL:FDN_DISALBE_REQ.");
            ulResult = USIMM_FDNHandleReq(MAPS_PIH_PID, USIMM_FBDN_DEACTIVE,
                                        ((SI_PIH_FDN_DISABLE_REQ_STRU*)pMsg)->aucPIN2);
            break;

        case SI_PIH_BDN_QUERY_REQ:
            USIMM_BdnQuery(&ulResult);
            ulResult = SI_PIH_FdnBdnStateReport(ulResult, TAF_ERR_NO_ERROR, SI_PIH_FDN_BDN_QUERY);
            break;

        case SI_PIH_FDN_QUERY_REQ:
            USIMM_FdnQuery(&ulResult);
            ulResult = SI_PIH_FdnBdnStateReport(ulResult, TAF_ERR_NO_ERROR, SI_PIH_FDN_BDN_QUERY);
            break;

        case SI_PIH_GACCESS_REQ:
            PIH_NORMAL_LOG("SI_SIM_AppMsgProc:NORMAL:SI_PIH_GACCESS_REQ.");
            ulResult = SI_PIH_GAccessReqProc((SI_PIH_GACCESS_REQ_STRU *)pMsg);
            break;

        case SI_PIH_PCSC_DATA_REQ:
            PIH_NORMAL_LOG("SI_SIM_AppMsgProc:NORMAL:SI_PIH_PCSC_REQ.");
            ulResult = SI_PIH_PCSCCmdHandle((SI_PIH_PCSC_REQ_STRU *)pMsg);
            break;

        case SI_PIH_ISDB_ACCESS_REQ:
            PIH_NORMAL_LOG("SI_SIM_AppMsgProc:NORMAL:SI_PIH_ISDB_ACCESS_REQ.");
            ulResult = SI_PIH_IsdbAccessReqProc((SI_PIH_ISDB_ACCESS_REQ_STRU *)pMsg);
            break;

        case SI_PIH_CCHO_SET_REQ:
            PIH_NORMAL_LOG("SI_SIM_AppMsgProc:NORMAL:SI_PIH_CCHO_SET_REQ.");
            ulResult = SI_PIH_CchoSetReqProc((SI_PIH_CCHO_SET_REQ_STRU *)pMsg);
            break;

        case SI_PIH_CCHC_SET_REQ:
            PIH_NORMAL_LOG("SI_SIM_AppMsgProc:NORMAL:SI_PIH_CCHC_SET_REQ.");
            ulResult = SI_PIH_CchcSetReqProc((SI_PIH_CCHC_SET_REQ_STRU *)pMsg);
            break;

        case SI_PIH_CGLA_SET_REQ:
            PIH_NORMAL_LOG("SI_SIM_AppMsgProc:NORMAL:SI_PIH_CGLA_SET_REQ.");
            ulResult = SI_PIH_CglaSetReqProc((SI_PIH_CGLA_REQ_STRU *)pMsg);
            break;

        case SI_PIH_CARD_ATR_QRY_REQ:
            PIH_NORMAL_LOG("SI_SIM_AppMsgProc:NORMAL:SI_PIH_CARD_ATR_QRY_REQ.");
            ulResult = SI_PIH_ATRQryReqProc();
            break;

#if (FEATURE_VSIM == FEATURE_ON)
        case SI_PIH_HVSST_QUERY_REQ:
            PIH_NORMAL_LOG("SI_SIM_AppMsgProc:NORMAL:SI_PIH_HVSST_QUERY_REQ.");
            ulResult = SI_PIH_HVSSTQueryHandle((SI_PIH_MSG_HEADER_STRU *)pMsg);
            break;

        case SI_PIH_HVSST_SET_REQ:
            PIH_NORMAL_LOG("SI_SIM_AppMsgProc:NORMAL:SI_PIH_HVSST_SET_REQ.");
            ulResult = SI_PIH_HVSSTSetHandle((SI_PIH_HVSST_REQ_STRU *)pMsg);
            break;

        case SI_PIH_HVSDH_SET_REQ:
            PIH_NORMAL_LOG("SI_SIM_AppMsgProc:NORMAL:SI_PIH_HVSDH_SET_REQ.");
            ulResult = SI_PIH_HvsDHSetHandle(((SI_PIH_HVSDH_SET_REQ_STRU*)pMsg)->ulDataLen, ((SI_PIH_HVSDH_SET_REQ_STRU*)pMsg)->aucData);
            break;

        case SI_PIH_HVSDH_QRY_REQ:
            PIH_NORMAL_LOG("SI_SIM_AppMsgProc:NORMAL:SI_PIH_HVSDH_QRY_REQ.");
            ulResult = SI_PIH_HvsDHQueryHandle();
            break;

        case SI_PIH_HVSCONT_QRY_REQ:
            PIH_NORMAL_LOG("SI_SIM_AppMsgProc:NORMAL:SI_PIH_HVSCONT_QRY_REQ.");
            ulResult = SI_PIH_HvsContQueryHandle();
            break;

        case SI_PIH_FILE_WRITE_REQ:
            PIH_NORMAL_LOG("SI_SIM_AppMsgProc:NORMAL:SI_PIH_FILE_WRITE_REQ.");
            ulResult = SI_PIH_ATFileWriteHandle((SI_PIH_FILE_WRITE_REQ_STRU *)pMsg);
            break;
#endif

#if (FEATURE_ON == FEATURE_IMS)
        case SI_PIH_UICCAUTH_REQ:
            PIH_NORMAL_LOG("SI_SIM_AppMsgProc:NORMAL:SI_PIH_UICCAUTH_REQ.");
            ulResult = SI_PIH_UiccAuthHandle((SI_PIH_UICCAUTH_REQ_STRU *)pMsg);
            break;

        case SI_PIH_URSM_REQ:
            PIH_NORMAL_LOG("SI_SIM_AppMsgProc:NORMAL:SI_PIH_URSM_REQ.");
            ulResult = SI_PIH_AccessFileHandle((SI_PIH_ACCESSFILE_REQ_STRU *)pMsg);
            break;
#endif

        case SI_PIH_CARDTYPE_QUERY_REQ:
            PIH_NORMAL_LOG("SI_SIM_AppMsgProc:NORMAL:SI_PIH_CARDTYPE_QUERY_REQ.");
            ulResult = SI_PIH_CardTypeQueryHandle((SI_PIH_MSG_HEADER_STRU *)pMsg);
            break;

        default:
            PIH_WARNING_LOG("SI_SIM_AppMsgProc:WARNING:DEFAULT Unkown Msg.");
            ulResult = VOS_ERR;
            break;
    }

    if(USIMM_API_SUCCESS != ulResult)
    {
        PIH_WARNING1_LOG("SI_SIM_AppMsgProc:WARNING:EventType:", (VOS_INT32)pstMsg->ulEventType);

        SI_PIH_EventReportError(g_stPIHCtrlInfo.usClient, g_stPIHCtrlInfo.ucOpID,
                                    TAF_ERR_UNSPECIFIED_ERROR, pMsg->ulMsgName,pstMsg->ulEventType);

        g_stPIHCtrlInfo.enPIHLock = SI_PIH_UNLOCK;
    }

    return ulResult;
}
VOS_VOID SI_PIH_CardUsimStatusInd(PS_USIM_STATUS_IND_STRU *pstMsg)
{

    PS_USIM_STATUS_IND_STRU            *pstUsimMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulRegPid;
    VOS_UINT32                          i;

    SI_PIH_MNTNDataHook(SI_PIH_USIMREG_PID_HOOK, sizeof(g_aulPIHUsimBCPid), (VOS_UINT8*)g_aulPIHUsimBCPid);

    /* ѭ�����͸�PID�ϱ���״̬ */
    for (i = 0; i < SI_PIH_BCPID_REG_MAX; i++)
    {
        if (VOS_NULL == g_aulPIHUsimBCPid[i])
        {
            continue;
        }

        ulRegPid = g_aulPIHUsimBCPid[i];

        pstUsimMsg = (PS_USIM_STATUS_IND_STRU *)VOS_AllocMsg(MAPS_PIH_PID,
                                                         sizeof(PS_USIM_STATUS_IND_STRU) - VOS_MSG_HEAD_LENGTH);

        if (VOS_NULL_PTR == pstUsimMsg)
        {
            PIH_ERROR_LOG("SI_PIH_CardStatusInd: VOS_AllocMsg is Failed");

            return;
        }

        pstUsimMsg->ulReceiverPid     = ulRegPid;
        pstUsimMsg->enCardStatus      = pstMsg->enCardStatus;
        pstUsimMsg->enCardType        = pstMsg->enCardType;
        pstUsimMsg->ulMsgName         = PS_USIM_GET_STATUS_IND;    /* ������Ϣ���� */
        pstUsimMsg->ucIMSILen         = pstMsg->ucIMSILen;

        if (VOS_NULL != pstMsg->ucIMSILen)
        {
            VOS_MemCpy(pstUsimMsg->aucIMSI, pstMsg->aucIMSI, sizeof(pstMsg->aucIMSI));
        }

        (VOS_VOID)VOS_SendMsg(pstUsimMsg->ulSenderPid, pstUsimMsg);
    }

    return;
}


VOS_VOID SI_PIH_CardIsimStatusInd(PS_USIM_STATUS_IND_STRU *pstMsg)
{
    PS_USIM_STATUS_IND_STRU            *pstUsimMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulRegPid;
    VOS_UINT32                          i;

    SI_PIH_MNTNDataHook(SI_PIH_ISIMREG_PID_HOOK, sizeof(g_aulPIHISIMBCPid), (VOS_UINT8*)g_aulPIHISIMBCPid);

    /* ѭ�����͸�PID�ϱ���״̬ */
    for (i = 0; i < SI_PIH_BCPID_REG_MAX; i++)
    {
        if (VOS_NULL == g_aulPIHISIMBCPid[i])
        {
            continue;
        }

        ulRegPid = g_aulPIHISIMBCPid[i];

        pstUsimMsg = (PS_USIM_STATUS_IND_STRU *)VOS_AllocMsg(MAPS_PIH_PID,
                                                         sizeof(PS_USIM_STATUS_IND_STRU) - VOS_MSG_HEAD_LENGTH);

        if (VOS_NULL_PTR == pstUsimMsg)
        {
            PIH_ERROR_LOG("SI_PIH_CardIsimStatusInd: VOS_AllocMsg is Failed");

            return;
        }

        pstUsimMsg->ulReceiverPid     = ulRegPid;
        pstUsimMsg->enCardStatus      = pstMsg->enCardStatus;
        pstUsimMsg->enCardType        = pstMsg->enCardType;
        pstUsimMsg->ulMsgName         = PS_USIM_GET_STATUS_IND;    /* ������Ϣ���� */

        (VOS_VOID)VOS_SendMsg(pstUsimMsg->ulSenderPid, pstUsimMsg);
    }

    return;
}


VOS_VOID SI_PIH_CardStatusInd(PS_USIM_STATUS_IND_STRU *pstMsg)
{
    PIH_NORMAL_LOG("SI_PIH_CardStatusInd: Need Report the Card State.");

    if(USIMM_CARD_NOCARD == pstMsg->enCardType)
    {
        SI_PIH_CardIsimStatusInd(pstMsg);
        SI_PIH_CardUsimStatusInd(pstMsg);

        return;
    }

    /* �޿���ʱ�򣬸�ISIM��ע��ӿڹ㲥�޿���Ϣ */
    if(USIMM_CARD_ISIM == pstMsg->enCardType)
    {
        SI_PIH_CardIsimStatusInd(pstMsg);
    }
    else
    {
        SI_PIH_CardUsimStatusInd(pstMsg);
    }

    return;
}

/*****************************************************************************
�� �� ��  : SI_PIH_SendMsg2DSPSingle
��������  : PIH���͸�GDSP�Ĺ��ʽ�����Ϣ
�������  : ��
�������  : ��
�� �� ֵ  : VOS_OK
���ú���  :
��������  :
�޶���¼  :
1.��    ��  : 2010��07��26��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_VOID SI_PIH_SendMsg2DSPSingle(SI_PIH_DSP_LIMIT_ENUM_UINT16 enLimit)
{
    SI_PIH_DSP_LIMIT_STRU   *pMsg;

#if defined (INSTANCE_1)
    if (VOS_RATMODE_GSM != SHPA_GetRateType(MODEM_ID_1))  /*��ǰֻ��GSM֧��*/
#else
    if (VOS_RATMODE_GSM != SHPA_GetRateType(MODEM_ID_0))  /*��ǰֻ��GSM֧��*/
#endif
    {
        PIH_WARNING_LOG("SI_PIH_SendMsg2DSP: The Wdsp not Support the Limit Power Msg");

        return;
    }

    pMsg = (SI_PIH_DSP_LIMIT_STRU*)VOS_AllocMsg(WUEPS_PID_USIM,
                                        sizeof(SI_PIH_DSP_LIMIT_STRU)-VOS_MSG_HEAD_LENGTH);

    if(VOS_NULL_PTR == pMsg)
    {
        PIH_ERROR_LOG("SI_PIH_SendMsg2DSP: VOS_AllocMsg is Failed");/*��ӡ����*/

        return; /*���غ���������Ϣ*/
    }

    pMsg->ulReceiverPid = DSP_PID_GPHY;
    pMsg->ulLength      = 2*sizeof(VOS_UINT16);
    pMsg->usMsgName     = SI_PIH_DSP_POWER_LIMIT;
    pMsg->enLimit       = enLimit;

    (VOS_VOID)VOS_SendMsg(WUEPS_PID_USIM, pMsg);

    PIH_WARNING_LOG("SI_PIH_SendMsg2DSP: Send the Limit Power Msg");/*������ӡ�澯*/

    return;
}

/*****************************************************************************
�� �� ��  : SI_PIH_SendMsg2DSPAll
��������  : PIH���͸�GDSP�Ĺ��ʽ�����Ϣ
�������  : ��
�������  : ��
�� �� ֵ  : VOS_OK
���ú���  :
��������  :
�޶���¼  :
1.��    ��  : 2010��07��26��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_VOID SI_PIH_SendMsg2DSPAll(SI_PIH_DSP_LIMIT_ENUM_UINT16 enLimit)
{
    SI_PIH_DSP_LIMIT_STRU   *pMsg;
    MODEM_ID_ENUM_UINT16    i;

    for (i = 0; i < MODEM_ID_BUTT; i++)
    {
        if (VOS_RATMODE_GSM != SHPA_GetRateType(i))  /*��ǰֻ��GSM֧��*/
        {
            PIH_WARNING_LOG("SI_PIH_SendMsg2DSP: The Wdsp not Support the Limit Power Msg");

            continue;
        }

        pMsg = (SI_PIH_DSP_LIMIT_STRU*)VOS_AllocMsg(WUEPS_PID_USIM,
                                            sizeof(SI_PIH_DSP_LIMIT_STRU)-VOS_MSG_HEAD_LENGTH);

        if (VOS_NULL_PTR == pMsg)
        {
            PIH_ERROR_LOG("SI_PIH_SendMsg2DSP: VOS_AllocMsg is Failed");/*��ӡ����*/

            return; /*���غ���������Ϣ*/
        }

        if (MODEM_ID_0 == i)
        {
            pMsg->ulReceiverPid = I0_DSP_PID_GPHY;
        }
#if  (FEATURE_MULTI_MODEM == FEATURE_ON)
        else    /*Ŀǰ��ֻ������GDSP*/
        {
            pMsg->ulReceiverPid = I1_DSP_PID_GPHY;
        }
#endif

        pMsg->ulLength      = 2*sizeof(VOS_UINT16);
        pMsg->usMsgName     = SI_PIH_DSP_POWER_LIMIT;
        pMsg->enLimit       = enLimit;

        (VOS_VOID)VOS_SendMsg(WUEPS_PID_USIM, pMsg);

        PIH_WARNING_LOG("SI_PIH_SendMsg2DSP: Send the Limit Power Msg");/*������ӡ�澯*/
    }

    return;
}

/*****************************************************************************
�� �� ��  : SI_PIH_SendMsg2DSP
��������  : PIH���͸�GDSP�Ĺ��ʽ�����Ϣ
�������  : ��
�������  : ��
�� �� ֵ  : VOS_OK
���ú���  :
��������  :
�޶���¼  :
1.��    ��  : 2010��07��26��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_VOID SI_PIH_SendMsg2DSP(SI_PIH_DSP_LIMIT_ENUM_UINT16 enLimit)
{
    if (VOS_TRUE != USIMM_IsSvlte())
    {
        SI_PIH_SendMsg2DSPSingle(enLimit);
    }
    else             /*SVLTE NV ��Multi Modem�򿪵�ʱ�������*/
    {
        SI_PIH_SendMsg2DSPAll(enLimit);
    }

    return;
}
VOS_UINT32 SI_PIH_ProtectReset(SI_VOID)
{
    VOS_UINT32                          ulResult;
    VOS_UINT8                           ucCardStatus;

    USIMM_GetCardType(&ucCardStatus, VOS_NULL_PTR);

    /* �Ѿ��������Ͳ�Ҫ������SIM�� */
    if (USIMM_CARD_SERVIC_ABSENT == ucCardStatus)
    {
        PIH_WARNING_LOG("SI_PIH_ProtectReset: SIM already removed, no need Protect Reset.");

        return SI_PIH_NO_NEED_REPORT;
    }

    PIH_WARNING_LOG("SI_PIH_ProtectReset: Start the Protect Reset.");

    SI_PIH_POLL_32K_TIMER_STOP(&g_stPIHPollTime[SI_PIH_TIMER_NAME_CHECKSTATUS].stTimer);/*ֹͣ���ܿ����Ķ�ʱ��*/
    VOS_StopRelTimer(&g_stPIHPollTime[SI_PIH_TIMER_NAME_CHECKIMSI].stTimer);
    VOS_StopRelTimer(&g_stPIHPollTime[SI_PIH_TIMER_NAME_CALL].stTimer);

    if(g_stPIHProtectCtrl.enProtectFun == SI_PIH_PROTECT_DISABLE)  /*��ǰ���ܹر�*/
    {
        g_stPIHProtectCtrl.enProtectState = SI_PIH_PROTECT_OFF;     /*�������������ٽ��б�����λ��*/

        PIH_WARNING_LOG("SI_PIH_ProtectReset: No Need the Protect Reset.");

        return SI_PIH_NEED_REPORT;                                  /*��ǰ��Ҫ�ϱ���״̬*/
    }

    g_stPIHProtectCtrl.enProtectState = SI_PIH_PROTECT_RUNING;      /*���ĵ�ǰ�����Ը�λ��״̬*/

    PIH_WARNING_LOG("SI_PIH_ProtectReset: Need Send the Prower Limit Msg.");

    SI_PIH_SendMsg2DSP(SI_PIH_DSP_POWER_LIMIT_ON);


    ulResult = USIMM_ProtectResetReq(MAPS_PIH_PID);           /*�·���λ����*/

    if(ulResult != VOS_OK)
    {
        PIH_WARNING_LOG("SI_PIH_ProtectReset: USIMM_ProtectResetReq return Error.");

        return SI_PIH_NEED_REPORT;
    }

    PIH_WARNING_LOG("SI_PIH_ProtectReset: End the Protect Reset.");

    return SI_PIH_NO_NEED_REPORT;
}
VOS_VOID SI_PIH_CardStateIndProc(PS_USIM_STATUS_IND_STRU *pstMsg)
{
    /* USIM/SIM������Ҫ�������²��� */
    if (USIMM_CARD_ISIM != pstMsg->enCardType)
    {
        if(pstMsg->enCardStatus == USIMM_CARD_SERVIC_AVAILABLE)     /*��״̬����*/
        {
            PIH_NORMAL_LOG("SI_PIH_CardStateIndProc: The Card State is USIMM_CARD_SERVIC_AVAILABLE");

            g_stPIHProtectCtrl.enProtectFun = SI_PIH_PROTECT_ENABLE;    /*�����Ը�λ�����ܿ���*/

            g_stPIHProtectCtrl.enProtectState = SI_PIH_PROTECT_ON;      /*������ر�������*/

            SI_PIH_PollTimerPro(&g_stPIHPollTime[SI_PIH_TIMER_NAME_CHECKSTATUS]);/*������ѯ��״̬��ʱ��*/

            SI_PIH_PollTimerPro(&g_stPIHPollTime[SI_PIH_TIMER_NAME_CHECKIMSI]); /*������ѯIMSI��ʱ��*/

            SI_PIH_PollTimerPro(&g_stPIHPollTime[SI_PIH_TIMER_NAME_CALL]);
        }
        else                                                                /*������״̬*/
        {
            VOS_StopRelTimer(&g_stPIHPollTime[SI_PIH_TIMER_NAME_CHECKIMSI].stTimer);
            VOS_StopRelTimer(&g_stPIHPollTime[SI_PIH_TIMER_NAME_CALL].stTimer);

            if(pstMsg->enCardStatus == USIMM_CARD_SERVIC_ABSENT)     /*��״̬�޿�*/
            {
                g_stPIHProtectCtrl.enProtectFun = SI_PIH_PROTECT_DISABLE;

                /*ֹͣ���ܿ����Ķ�ʱ��*/
                SI_PIH_POLL_32K_TIMER_STOP(&g_stPIHPollTime[SI_PIH_TIMER_NAME_CHECKSTATUS].stTimer);
            }
            else
            {
                /*������ѯ��״̬��ʱ��*/
                SI_PIH_PollTimerPro(&g_stPIHPollTime[SI_PIH_TIMER_NAME_CHECKSTATUS]);
            }

            PIH_NORMAL_LOG("SI_PIH_CardStateIndProc: The Card State is Other");
        }
    }

    SI_PIH_CardStatusInd(pstMsg);                                   /*�ϱ���MMA/PBģ��*/

    return;
}

VOS_VOID SI_PIH_RAccessCnfProc(PS_USIM_RESTRICTED_ACCESS_CNF_STRU *pstRAccessCnf)
{
    VOS_UINT32 ulResult;
    VOS_UINT8  ucCardStatus;
    VOS_UINT8  aucImsi[USIMM_EF_IMSI_LEN];

    /* ��ȡ����ǰ״̬ */
    USIMM_GetCardType(&ucCardStatus, VOS_NULL_PTR);

    if(USIMM_CARD_SERVIC_AVAILABLE != ucCardStatus)
    {
        PIH_WARNING_LOG("SI_PIH_GAccessProc: Card Status Wrong");

        return;
    }

    ulResult = pstRAccessCnf->ulErrorCode;

    if(USIMM_SW_OK == pstRAccessCnf->ulErrorCode)
    {
        /* ���ڴ��л�ȡ��ǰIMSI */
        if(USIMM_API_SUCCESS != USIMM_GetCardIMSI(aucImsi))
        {
            PIH_WARNING_LOG("SI_PIH_GAccessProc: read IMSI error");

            return;
        }

        /* �ڴ���IMSI�ź�USIM����IMSI�����ݲ�ƥ��*/
        if(VOS_OK != VOS_MemCmp(aucImsi, pstRAccessCnf->aucContent, sizeof(aucImsi)))
        {
            PIH_WARNING_LOG("SI_PIH_GAccessProc: The IMSI is Not Same");

            ulResult = USIMM_SW_SENDCMD_ERROR;
        }
    }

    /* ��ȡʧ�� */
    if((USIMM_SW_OK != ulResult) && (USIMM_SW_SECURITY_ERROR != ulResult))
    {
        PIH_WARNING_LOG("SI_PIH_GAccessProc: The Msg is Error, Need Protect Reset");

        if(g_stPIHProtectCtrl.enProtectState == SI_PIH_PROTECT_RUNING)
        {
            PIH_WARNING_LOG("SI_PIH_GAccessProc: The Protect is Running, No Need Protect Reset");

            return;
        }

        /*���ִ�����Ҫ��������*/
        ulResult = SI_PIH_ProtectReset();

        if(SI_PIH_NEED_REPORT == ulResult)
        {
            PIH_WARNING_LOG("SI_PIH_GAccessProc: Need Deactive Card");

            USIMM_DeactiveCardReq(MAPS_PIH_PID);
        }
    }
    else
    {
        /* ����Timer */
        SI_PIH_PollTimerPro(&g_stPIHPollTime[SI_PIH_TIMER_NAME_CHECKIMSI]);
    }

    return;
}


VOS_VOID SI_PIH_ProtectResetCnfProc(PS_USIM_SINGLECMD_CNF_STRU *pstMsg)
{
    VOS_UINT8  ucCardStatus;

    if(VOS_OK != pstMsg->ulResult)    /*��ѯ״̬OK�� ������ʱ��*/
    {
        PIH_WARNING_LOG("SI_PIH_StatusProc: The Msg is Other Error");

        g_stPIHProtectCtrl.enProtectFun = SI_PIH_PROTECT_DISABLE;   /*���ù��ܹرգ����ϱ���״̬��ʱ��������*/

        return;
    }

    /* �����Ը�λ�ɹ������ȫ�ֱ���״̬ */
    g_stPIHProtectCtrl.enProtectState = SI_PIH_PROTECT_ON;

    SI_PIH_SendMsg2DSP(SI_PIH_DSP_POWER_LIMIT_OFF);     /*�����ɹ�*/

    USIMM_GetCardType(&ucCardStatus, VOS_NULL_PTR);

    if(USIMM_CARD_SERVIC_AVAILABLE == ucCardStatus)
    {
        SI_PIH_PollTimerPro(&g_stPIHPollTime[SI_PIH_TIMER_NAME_CHECKSTATUS]);/*������ѯ��״̬��ʱ��*/

        SI_PIH_PollTimerPro(&g_stPIHPollTime[SI_PIH_TIMER_NAME_CHECKIMSI]); /*������ѯIMSI��ʱ��*/

        SI_PIH_PollTimerPro(&g_stPIHPollTime[SI_PIH_TIMER_NAME_CALL]);
    }

    return;
}

/*****************************************************************************
�� �� ��  : SI_PIH_StatusProc
��������  : ��ѯ��״̬��Ϣ�Ϳ���λ����ظ��ϱ�
�������  : pstStatusCnf: ��Ϣ����
�������  : ��
�� �� ֵ  :
���ú���  : ��
��������  : �ⲿ�ӿ�
History     :
1.��    ��  : 2010��07��26��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_VOID SI_PIH_StatusCnfProc(PS_USIM_SINGLECMD_CNF_STRU *pstMsg)
{
    VOS_UINT32 ulResult;

    if(pstMsg->ulResult == VOS_OK)    /*��ѯ״̬OK�� ������ʱ��*/
    {
        PIH_NORMAL_LOG("SI_PIH_StatusProc: The Msg is OK");

        SI_PIH_PollTimerPro(&g_stPIHPollTime[SI_PIH_TIMER_NAME_CHECKSTATUS]);   /*������ʱ��������һ���ڹ���*/

        SI_PIH_PollTimerPro(&g_stPIHPollTime[SI_PIH_TIMER_NAME_CALL]);
    }
    else if(pstMsg->ulResult == USIMM_SW_OTHER_ERROR)     /*��ǰ�����������󣬲��ٽ�����������*/
    {
        PIH_WARNING_LOG("SI_PIH_StatusProc: The Msg is Other Error");

        g_stPIHProtectCtrl.enProtectFun = SI_PIH_PROTECT_DISABLE;   /*���ù��ܹرգ����ϱ���״̬��ʱ��������*/

        USIMM_DeactiveCardReq(MAPS_PIH_PID);              /*ȥ���ǰ�Ŀ�*/
    }
    else
    {
        PIH_WARNING_LOG("SI_PIH_StatusProc: The Msg is Error, Need Protect Reset");

        if(g_stPIHProtectCtrl.enProtectState == SI_PIH_PROTECT_RUNING)
        {
            PIH_WARNING_LOG("SI_PIH_StatusProc: The Protect is Running, No Need Protect Reset");

            return;
        }

        ulResult = SI_PIH_ProtectReset();              /*���ִ�����Ҫ��������*/

        if(ulResult == SI_PIH_NEED_REPORT)
        {
            PIH_WARNING_LOG("SI_PIH_StatusProc: Need Deactive Card");

            USIMM_DeactiveCardReq(MAPS_PIH_PID);
        }
    }

    return;
}

/*****************************************************************************
�� �� ��  : SI_PIH_GAccessCnfProc
��������  :
�������  : pstMsg
�������  : ��
�� �� ֵ  : SI_UINT32 ����ִ�н��
���ú���  : ��
��������  : �ⲿ�ӿ�
History     :
1.��    ��  : 2010��08��20��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_VOID SI_PIH_PCSCCnfProc(PS_USIM_GENERIC_ACCESS_CNF_STRU *pstMsg)
{
    VOS_UINT8               aucContent[260];
    VOS_UINT8               ucCardType;
    VOS_UINT32              ulDataLen;

    if(pstMsg->ulResult == USIMM_SW_ERR)
    {
        SI_PIH_PCSCDataCnf(g_stPIHCtrlInfo.ulEventType, pstMsg->ulResult, 0, VOS_NULL_PTR);

        return;
    }

    USIMM_GetCardType(VOS_NULL_PTR, &ucCardType);

    g_stPIHCSIMCtrlInfo.usPathLen = pstMsg->usPathLen;

    VOS_MemCpy(g_stPIHCSIMCtrlInfo.ausPath, pstMsg->ausPath, pstMsg->usPathLen*sizeof(VOS_UINT16));

    if((pstMsg->ucINS == 0xA4)||(pstMsg->ucINS == 0x88)||(pstMsg->ucINS == 0x32)||(pstMsg->ucINS == 0xA2))
    {
        g_stPIHCSIMCtrlInfo.usRspLen = pstMsg->usLen;

        ulDataLen = 2;

        if(pstMsg->usLen != 0)
        {
            VOS_MemCpy(g_stPIHCSIMCtrlInfo.aucRspCotent, pstMsg->aucContent, pstMsg->usLen);

            aucContent[1] = (VOS_UINT8)pstMsg->usLen;

            if(ucCardType == USIMM_CARD_SIM)
            {
                aucContent[0] = 0x9F;
            }
            else
            {
                aucContent[0] = 0x61;
            }
        }
        else
        {
            aucContent[0] = pstMsg->ucSw1;
            aucContent[1] = pstMsg->ucSw2;
        }
    }
    else
    {
        g_stPIHCSIMCtrlInfo.usRspLen = 0;

        ulDataLen = pstMsg->usLen;

        VOS_MemCpy(aucContent, pstMsg->aucContent, pstMsg->usLen);

        aucContent[ulDataLen]   = pstMsg->ucSw1;
        aucContent[ulDataLen+1] = pstMsg->ucSw2;

        ulDataLen += 2;
    }

    SI_PIH_PCSCDataCnf(g_stPIHCtrlInfo.ulEventType, pstMsg->ulResult, ulDataLen, aucContent);

    return;
}

/*****************************************************************************
�� �� ��  : SI_PIH_GAccessCnfProc
��������  :
�������  : pstMsg
�������  : ��
�� �� ֵ  : SI_UINT32 ����ִ�н��
���ú���  : ��
��������  : �ⲿ�ӿ�
History     :
1.��    ��  : 2010��08��20��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_VOID SI_PIH_GAccessCnfProc(PS_USIM_GENERIC_ACCESS_CNF_STRU *pstMsg)
{
    SI_PIH_EVENT_INFO_STRU  stEvent;
    VOS_UINT8               ucCardType;

    if(pstMsg->ulResult == USIMM_SW_ERR)
    {
        stEvent.PIHError = TAF_ERR_PARA_ERROR;

        SI_PIH_EventCallBack(&stEvent);

        return;
    }

    USIMM_GetCardType(VOS_NULL_PTR, &ucCardType);

    stEvent.PIHError    = VOS_OK;       /*��֤���Դ�ӡ���*/

    g_stPIHCSIMCtrlInfo.usPathLen = pstMsg->usPathLen;

    VOS_MemCpy(g_stPIHCSIMCtrlInfo.ausPath, pstMsg->ausPath, pstMsg->usPathLen*sizeof(VOS_UINT16));

    if((pstMsg->ucINS == 0xA4)||(pstMsg->ucINS == 0x88)||(pstMsg->ucINS == 0x32)||(pstMsg->ucINS == 0xA2))
    {
        g_stPIHCSIMCtrlInfo.usRspLen = pstMsg->usLen;

        stEvent.PIHEvent.GAccessCnf.Len = 0;

        if(pstMsg->usLen != 0)
        {
            VOS_MemCpy(g_stPIHCSIMCtrlInfo.aucRspCotent, pstMsg->aucContent, pstMsg->usLen);

            stEvent.PIHEvent.GAccessCnf.SW2 = (VOS_UINT8)pstMsg->usLen;

            if(ucCardType == USIMM_CARD_SIM)
            {
                stEvent.PIHEvent.GAccessCnf.SW1 = 0x9F;
            }
            else
            {
                stEvent.PIHEvent.GAccessCnf.SW1 = 0x61;
            }
        }
        else
        {
            stEvent.PIHEvent.GAccessCnf.SW1 = pstMsg->ucSw1;
            stEvent.PIHEvent.GAccessCnf.SW2 = pstMsg->ucSw2;
        }
    }
    else
    {
        g_stPIHCSIMCtrlInfo.usRspLen = 0;

        stEvent.PIHEvent.GAccessCnf.SW1 = pstMsg->ucSw1;
        stEvent.PIHEvent.GAccessCnf.SW2 = pstMsg->ucSw2;
        stEvent.PIHEvent.GAccessCnf.Len = pstMsg->usLen;

        VOS_MemCpy(stEvent.PIHEvent.GAccessCnf.Command, pstMsg->aucContent, pstMsg->usLen);
    }

    SI_PIH_EventCallBack(&stEvent);

    return;
}
VOS_VOID SI_PIH_IsdbAccessCnfProc(PS_USIM_ISDB_ACCESS_CNF_STRU *pstMsg)
{
    SI_PIH_EVENT_INFO_STRU              stEvent;

    /* APDU��ʽ���Ϸ� */
    if (USIMM_SW_ERR == pstMsg->ulResult)
    {
        stEvent.PIHError = TAF_ERR_PARA_ERROR;
    }
    else
    {
        stEvent.PIHError                        =   TAF_ERR_NO_ERROR;
        stEvent.PIHEvent.IsdbAccessCnf.ucSW1    =   pstMsg->ucSw1;
        stEvent.PIHEvent.IsdbAccessCnf.ucSW2    =   pstMsg->ucSw2;
        stEvent.PIHEvent.IsdbAccessCnf.usLen    =   pstMsg->usLen;

        VOS_MemCpy(stEvent.PIHEvent.IsdbAccessCnf.aucCommand, pstMsg->aucContent, pstMsg->usLen);
    }

    SI_PIH_EventCallBack(&stEvent);

    return;
}
VOS_VOID SI_PIH_OpenChannelCnfProc(PS_USIM_OPEN_CHANNEL_CNF_STRU *pstMsg)
{
    SI_PIH_EVENT_INFO_STRU              stEvent;

    /* ����δ�ɹ� */
    stEvent.PIHError                        =   pstMsg->ulResult;
    stEvent.PIHEvent.ulSessionID            =   pstMsg->ulSessionId;

    SI_PIH_EventCallBack(&stEvent);

    return;
}
VOS_VOID SI_PIH_CloseChannelCnfProc(PS_USIM_CLOSE_CHANNEL_CNF_STRU *pstMsg)
{
    SI_PIH_EVENT_INFO_STRU              stEvent;

    stEvent.PIHError = pstMsg->ulResult;

    SI_PIH_EventCallBack(&stEvent);

    return;
}


VOS_VOID SI_PIH_AccessChannelCnfProc(PS_USIM_ACCESS_CHANNEL_CNF_STRU *pstMsg)
{
    SI_PIH_EVENT_INFO_STRU              stEvent;

    /* APDU��ʽ���Ϸ� */
    if (VOS_OK != pstMsg->ulResult)
    {
        stEvent.PIHError = TAF_ERR_ERROR;
    }
    else
    {
        stEvent.PIHError                       =   TAF_ERR_NO_ERROR;
        stEvent.PIHEvent.stCglaCmdCnf.ucSW1    =   pstMsg->ucSw1;
        stEvent.PIHEvent.stCglaCmdCnf.ucSW2    =   pstMsg->ucSw2;
        stEvent.PIHEvent.stCglaCmdCnf.usLen    =   pstMsg->usLen;

        VOS_MemCpy(stEvent.PIHEvent.stCglaCmdCnf.aucCommand, pstMsg->aucContent, pstMsg->usLen);
    }

    SI_PIH_EventCallBack(&stEvent);

    return;
}
VOS_VOID SI_PIH_RefreshIndProc(PS_USIM_REFRESH_IND_STRU *pstMsg)
{
    PS_USIM_REFRESH_IND_STRU           *pstUsimMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulRegPid;
    VOS_UINT32                          i;

    PIH_NORMAL_LOG("SI_PIH_RefreshIndProc: Need Report Refresh.");

    SI_PIH_MNTNDataHook(SI_PIH_REFRESHREG_PID_HOOK, sizeof(g_aulPIHRefreshBCPid), (VOS_UINT8*)g_aulPIHRefreshBCPid);

    /* ѭ�����͸�PID�ϱ���״̬ */
    for (i = 0; i < SI_PIH_BCPID_REG_MAX; i++)
    {
        if (VOS_NULL == g_aulPIHRefreshBCPid[i])
        {
            continue;
        }

        ulRegPid = g_aulPIHRefreshBCPid[i];

        pstUsimMsg = (PS_USIM_REFRESH_IND_STRU *)VOS_AllocMsg(MAPS_PIH_PID, pstMsg->ulLength);

        if (VOS_NULL_PTR == pstUsimMsg)
        {
            PIH_ERROR_LOG("SI_PIH_RefreshIndProc: VOS_AllocMsg is Failed");

            return;
        }

        VOS_MemCpy((VOS_UINT8*)pstUsimMsg + VOS_MSG_HEAD_LENGTH,
                   (VOS_UINT8*)pstMsg + VOS_MSG_HEAD_LENGTH,
                   pstMsg->ulLength);

        pstUsimMsg->ulReceiverPid = ulRegPid;

        (VOS_VOID)VOS_SendMsg(pstUsimMsg->ulSenderPid, pstUsimMsg);
    }

    return;
}
VOS_UINT32 SI_PIH_UsimMsgProc(PS_SI_MSG_STRU *pMsg)
{
    PS_USIM_FDN_CNF_STRU            *pstFDNCnf;
    VOS_UINT32                      ulResult = VOS_OK;
    VOS_UINT32                      ulRet = VOS_OK;

    switch(pMsg->ulMsgName)
    {
        case PS_USIM_GET_STATUS_IND:
            SI_PIH_CardStateIndProc((PS_USIM_STATUS_IND_STRU *)pMsg);
            break;

        case PS_UISM_FDN_CNF:
            pstFDNCnf = (PS_USIM_FDN_CNF_STRU*)pMsg;

            if(VOS_OK == pstFDNCnf->ulResult)
            {
                PIH_NORMAL_LOG("SI_SIM_CnfMsgProc:NORMAL: FDN CNF OK.");
                ulRet = SI_PIH_FdnBdnStateReport(pstFDNCnf->enFDNState, TAF_ERR_NO_ERROR, g_stPIHCtrlInfo.enCmdType);
            }
            else if (USIMM_SIM_PUK2_REQUIRED == pstFDNCnf->ulResult)
            {
                PIH_WARNING_LOG("SI_SIM_CnfMsgProc:NORMAL: FDN CNF PUK2 REQUIRED.");
                ulRet = SI_PIH_FdnBdnStateReport(pstFDNCnf->enFDNState, TAF_ERR_NEED_PUK2, g_stPIHCtrlInfo.enCmdType);
            }
            else if (USIMM_INCORRECT_PASSWORD == pstFDNCnf->ulResult)
            {
                PIH_WARNING_LOG("SI_SIM_CnfMsgProc:NORMAL: FDN CNF INCORRECT PASSWORD.");
                ulRet = SI_PIH_FdnBdnStateReport(pstFDNCnf->enFDNState, TAF_ERR_SIM_INCORRECT_PASSWORD, g_stPIHCtrlInfo.enCmdType);
            }
            else
            {
                PIH_WARNING_LOG("SI_SIM_CnfMsgProc:NORMAL: FDN CNF ERROR.");
                ulRet = SI_PIH_FdnBdnStateReport(pstFDNCnf->enFDNState, TAF_ERR_SIM_FAIL, g_stPIHCtrlInfo.enCmdType);
            }

            if (VOS_OK != ulRet)
            {
                PIH_INFO_LOG("SI_PIH_UsimMsgProc:Fdn Report fail");
            }

            break;

        case PS_USIM_STATUSCMD_CNF:
            SI_PIH_StatusCnfProc((PS_USIM_SINGLECMD_CNF_STRU*)pMsg);
            break;

        case PS_USIM_PROTECT_RESET_CNF:
            SI_PIH_ProtectResetCnfProc((PS_USIM_SINGLECMD_CNF_STRU*)pMsg);
            break;

        case PS_USIM_RESTRICTED_ACCESS_CNF:
            SI_PIH_RAccessCnfProc((PS_USIM_RESTRICTED_ACCESS_CNF_STRU*)pMsg);
            break;

        case PS_USIM_GENERIC_ACCESS_CNF:
            if(g_stPIHCtrlInfo.enCmdType == SI_PIH_PCSC_DATA_REQ)
            {
                SI_PIH_PCSCCnfProc((PS_USIM_GENERIC_ACCESS_CNF_STRU*)pMsg);
            }
            else
            {
                SI_PIH_GAccessCnfProc((PS_USIM_GENERIC_ACCESS_CNF_STRU*)pMsg);
            }
            break;

        case PS_USIMM_ISDB_ACCESS_CNF:
            SI_PIH_IsdbAccessCnfProc((PS_USIM_ISDB_ACCESS_CNF_STRU *)pMsg);
            break;

        case PS_USIMM_OPENCHANNEL_CNF:
            SI_PIH_OpenChannelCnfProc((PS_USIM_OPEN_CHANNEL_CNF_STRU *)pMsg);
            break;

        case PS_USIMM_CLOSECHANNEL_CNF:
            SI_PIH_CloseChannelCnfProc((PS_USIM_CLOSE_CHANNEL_CNF_STRU *)pMsg);
            break;

        case PS_USIMM_ACCESSCHANNEL_CNF:
            SI_PIH_AccessChannelCnfProc((PS_USIM_ACCESS_CHANNEL_CNF_STRU *)pMsg);
            break;

        case PS_USIM_REFRESH_IND:
            SI_PIH_RefreshIndProc((PS_USIM_REFRESH_IND_STRU *)pMsg);
            break;

#if (FEATURE_VSIM == FEATURE_ON)
        case PS_USIMM_ACTIVE_CNF:
            SI_PIH_ActiveSimCnfProc((PS_USIM_SINGLECMD_CNF_STRU*)pMsg);
            break;

        case PS_USIMM_VSIM_RDH_IND:
            SI_PIH_ReDhNegotiateIndProc();
            break;
#endif

#if (FEATURE_ON == FEATURE_IMS)
        case PS_USIM_AUTHENTICATION_CNF:
            SI_PIH_AuthCnfProc((USIMM_AUTH_CNF_STRU *)pMsg);
            break;

        case PS_USIM_GET_FILE_RSP:
            SI_PIH_GetFileCnfProc((PS_USIM_GET_FILE_CNF_STRU *)pMsg);
            break;

        case PS_USIM_SET_FILE_RSP:
            SI_PIH_SetFileCnfProc((PS_USIM_SET_FILE_CNF_STRU *)pMsg);
            break;
#endif
        default:
            PIH_WARNING_LOG("SI_SIM_CnfMsgProc:WARNING:Default Unknow Msg");
            ulResult = VOS_ERR;
            break;
    }

    return ulResult;
}


VOS_UINT32 SI_PIH_TimerMsgProc(REL_TIMER_MSG* pstMsg)
{
    VOS_UINT32                          ulResult = VOS_OK;
    TAF_SIM_RESTRIC_ACCESS_STRU         stRestricAccess;

    if(g_stPIHProtectCtrl.enProtectState == SI_PIH_PROTECT_RUNING)
    {
        PIH_WARNING_LOG("SI_PIH_TimerMsgProc:Default Protect Running Discard Timer Msg");
        return ulResult;
    }

    switch(pstMsg->ulName)
    {
        case SI_PIH_TIMER_NAME_CHECKSTATUS:
        case SI_PIH_TIMER_NAME_CALL:
            ulResult = USIMM_StatusReq(MAPS_PIH_PID, g_stPIHPollTime[pstMsg->ulName].enPollData);
            break;

        case SI_PIH_TIMER_NAME_CHECKIMSI:

            /* ������ݽṹ���Զ�ȡ�������ļ��ķ�ʽ��ȡ6F07�ļ���
               P1/P2/P3�ֱ�Ϊ0��0��9. �ļ�·��Ϊ�գ���Ĭ��·�������� */
            stRestricAccess.usCmdType   = USIMM_READ_BINARY;
            stRestricAccess.ucP1        = 0;
            stRestricAccess.ucP2        = 0;
            stRestricAccess.ucP3        = 9;
            stRestricAccess.usEfId      = 0x6F07;
            stRestricAccess.usPathLen   = 0;

            ulResult = USIMM_RestrictedAccessReq(MAPS_PIH_PID, 0, &stRestricAccess);
            break;

#if (FEATURE_ON == FEATURE_VSIM)
        case SI_PIH_TIMER_NAME_DEACTIVECARD:
            ulResult = SI_PIH_DeactvieCardTimeOut();
            break;
#endif
        default:
            PIH_WARNING_LOG("SI_PIH_TimerMsgProc:Default Unknow Timer Msg");
            break;
    }

    return ulResult;
}


VOS_VOID SI_PIH_PidMsgProc( PS_SI_MSG_STRU *pMsg )
{
    VOS_UINT32 ulResult = VOS_ERR;

    OM_RecordInfoStart(VOS_EXC_DUMP_MEM_NUM_2, pMsg->ulSenderPid, MAPS_PIH_PID, pMsg->ulMsgName);
    switch(pMsg->ulSenderPid)
    {
        case ACPU_PID_PCSC:
        case MAPS_PIH_PID:
        case WUEPS_PID_AT:
            ulResult = SI_PIH_MsgProc(pMsg);
            break;

        case WUEPS_PID_USIM:
            ulResult = SI_PIH_UsimMsgProc(pMsg);
            break;

        case VOS_PID_TIMER:
            ulResult = SI_PIH_TimerMsgProc((REL_TIMER_MSG*)pMsg);
            break;

        default:
            PIH_WARNING_LOG("SI_PIH_PidMsgProc:Default Unknow PID Msg");
            break;
    }

    if(ulResult == VOS_ERR)
    {
        PIH_ERROR_LOG("SI_PIH_PidMsgProc: The Msg Handle Error.");
    }

    OM_RecordInfoEnd(VOS_EXC_DUMP_MEM_NUM_2);

    return;
}


VOS_UINT32 SI_PIHCallback(SI_PIH_EVENT_INFO_STRU *pstEvent)
{
#ifndef COMM_ITT
    MN_APP_PIH_AT_CNF_STRU *pstMsg;

    pstMsg = (MN_APP_PIH_AT_CNF_STRU*)VOS_AllocMsg(MAPS_PIH_PID,
                sizeof(MN_APP_PIH_AT_CNF_STRU)-VOS_MSG_HEAD_LENGTH);

    if (VOS_NULL_PTR == pstMsg)
    {
        PIH_ERROR_LOG("SI_SimCallback: Alloc Msg Failed!");
        return VOS_ERR;
    }

    pstEvent->ClientId = MN_GetRealClientId(pstEvent->ClientId, MAPS_PIH_PID);

    VOS_MemCpy((VOS_VOID *)&pstMsg->stPIHAtEvent, pstEvent, sizeof(SI_PIH_EVENT_INFO_STRU));

    pstMsg->ulReceiverPid = WUEPS_PID_AT;
    pstMsg->ulMsgId = PIH_AT_EVENT_CNF;

    (VOS_VOID)VOS_SendMsg(MAPS_PIH_PID, pstMsg);

#endif

    return VOS_OK;
}

/*****************************************************************************
�� �� ��  : WuepsSIFidInit
��������  : SI����ĳ�ʼ������
�������  : ip ��ʼ������
�������  : ��
�� �� ֵ  : VOS_UINT32����ʾ����ִ�н��
���ú���  : ��
��������  : �ⲿ�ӿ�
History     :
1.��    ��  : 2012��12��28��
  ��    ��  : zhuli
  �޸�����  : Create
*****************************************************************************/
VOS_UINT32 WuepsSIFidInit(enum VOS_INIT_PHASE_DEFINE ip)
{
    VOS_UINT32          ulRslt;

    switch( ip )
    {
        case VOS_IP_LOAD_CONFIG:
        {
            /* PB ģ��PID ע�� */
            ulRslt = VOS_RegisterPIDInfo(MAPS_PB_PID,
                                        (Init_Fun_Type)WuepsPBPidInit,
                                        (Msg_Fun_Type)SI_PB_PidMsgProc);
            if ( VOS_OK != ulRslt )
            {
                return VOS_ERR;
            }

            /* PIH ģ��PID ע�� */
            ulRslt = VOS_RegisterPIDInfo(MAPS_PIH_PID,
                                        (Init_Fun_Type)WuepsPIHPidInit,
                                        (Msg_Fun_Type)SI_PIH_PidMsgProc);
            if ( VOS_OK != ulRslt)
            {
                return VOS_ERR;
            }

            /* STK ģ��PID ע�� */
            ulRslt = VOS_RegisterPIDInfo(MAPS_STK_PID,
                                        (Init_Fun_Type)WuepsSTKPidInit,
                                        (Msg_Fun_Type)SI_STK_PidMsgProc);
            if ( VOS_OK != ulRslt)
            {
                return VOS_ERR;
            }

            ulRslt = VOS_RegisterMsgTaskPrio(MAPS_SI_FID, VOS_PRIORITY_M5);
            if( VOS_OK != ulRslt )
            {
                return VOS_ERR;
            }

            break;
        }

        default:
            break;
    }

    return VOS_OK;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


